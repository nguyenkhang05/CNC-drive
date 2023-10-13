/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string.h>
#include <stdlib.h>
#include "./config.h"
#include "./forest_utility.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./icache.h"
#include "./fsbus.h"
#include "./sregfile.h"
#include "./forest_message.h"
#include "./module_guard.h"
#include "./lsu.h"

/*!
 * @brief Calculates the total number of Entry
 * from Cache Size, Way number and Line Size.
 * @param ic_size Cache Size.
 * @param way_type Way Type.
 * @return Total number of Entry.
 */
uint32_t Icache::CalEntryNum (ByteSize ic_size, CacheWayType way_type) const
{
    switch (way_type) {
    case WAY0: return 0; break;
    case WAY1: return ic_size / (1 * LINE_BYTES); break;
    case WAY2: return ic_size / (2 * LINE_BYTES); break;
    default:   return ic_size / (4 * LINE_BYTES); break;
    }
}


/*!
 * @brief Sets the function pointer by the Way type.
 * @param way_type Way Type.
 * @note
 * Way type is switched dynamically by Software.
 * Then Icache should switch the routine by the Way type.
 * But switching doesn't happen frequently.
 * So, to avoid if-check cost for Way type at run time,
 * sets search-function as a function pointer and switches the address to call.
 */
void Icache::SetWayType (CacheWayType way_type)
{
    switch (way_type) {
    case WAY_PBS:
        pFindIcHitWay = &Icache::FindWayPbs;
        pGetLruWay    = &Icache::GetWayFromLruPbs;
        break;
    case WAY4:
        pFindIcHitWay = &Icache::FindWay4Way;
        pGetLruWay    = &Icache::GetWayFromLru4Way;
        break;
    case WAY2:
        pFindIcHitWay = &Icache::FindWay2Way;
        pGetLruWay    = &Icache::GetWayFromLru2Way;
        break;
    case WAY1:
        pFindIcHitWay = &Icache::FindWay1Way;
        pGetLruWay    = &Icache::GetWayFromLru1Way;
        break;
    case WAY0:
        pFindIcHitWay = &Icache::FindWay0Way;
        pGetLruWay    = &Icache::GetWayFromLru1Way;
        break;
    }
}


/*!
 * @brief  Fetches the data from Instruction Cache.
 * @param  vaddr Logical address to fetch.
 * @param  paddr Physical address to fetch.
 * @param  data Data Buffer of fetched data.
 * @param  iq_data_size data size of Instruction Que.
 * @param  sideband Side band.
 * @param  trace Trace information object for debugging.
 * @return System Error Source, Defined in Utility/forest_common.h, uint32_t number.
 */
SysErrCause Icache::FetchFromIC (PhysAddr vaddr, PhysAddr paddr, MemData8* data,
                          ByteSize iq_data_size, SideBand sideband, TraceCtrl* trace)
{
    // @@ This implimentation may not be elegant. Please find better way.

    // This routine supports only "the tag is always calclulated from paddr".
    // In G3M, in theory, the tag should be calculated from vaddr.
    // But in cforest, there is no MMU, then paddr == vaddr.
    // So it is OK to calculate the tag from paddr.
    // But if there is MMU, the tag will be calculated from paddr too, not vaddr.
    // Please note it.

    uint32_t tag   = GetTagFromAddr (paddr);
    uint32_t entry = GetEntryFromAddr (vaddr);

    uint32_t prty = 0;
    uint32_t way  = (this->*pFindIcHitWay) (tag, entry, &prty);

    uint32_t mru_way;
#ifdef FROG_API_IF
#else // FROG_API_IF
    if (likely (way != IC_MISS)) {
        // Cache Hit.
        mru_way = way;
        ++ m_hit_count;
        trace->RecordIcAccess (entry, way, IC_EVT_HIT, NO_ADDR);

        // Copies the data from IC(Instruction Cache).
        IcacheLine* hit_line = m_icache[entry]->m_line[mru_way];
        uint32_t data_idx = static_cast<uint32_t> (paddr & DATA_SHIFT_MASK);
        hit_line->DataWriteFromIC (data, data_idx, iq_data_size);
        // Updates LRU(Least Recently Used).
        UpdateLru (entry, mru_way);

        // Records a Cache Hit.
        m_fill_route = ICACHE_HIT_ROUTE;

        return SYSERR_NONE;
    } else {
#endif // FROG_API_IF
        // Cache Miss
        SysErrCause err = SYSERR_NONE;

        uint32_t lru_way = (this->*pGetLruWay) (entry, prty);
        IcacheLine* fill_line = m_icache[entry]->m_line[lru_way];

        // Checks that all Ways are locked.
        if (likely (fill_line->IsLock () == false)) {
            {
                err = fill_line->IcLineFill (paddr, tag, sideband);
#ifdef ENABLE_ERROR_AREA
                if (err != SYSERR_NONE) {
                    if (err == SYSERR_NOP) {
                        // If NOP, then not generate SYSERR, but reads 0 and normal end.
                    } else {
                        // @@ There is no support for SYSERR.
                        // For now, it will be harmless if not to use wrong-fetched data.
                        memset (data, 0x0, sizeof(uint8_t) * iq_data_size);

                        // Records a Cache Miss
                        m_fill_route = ICACHE_MISS_ROUTE;
                        return err;
                    }
                }
#endif // #ifdef ENABLE_ERROR_AREA
            }

            mru_way = lru_way;
            ++ m_miss_count;
            trace->RecordIcAccess (entry, mru_way, IC_EVT_MISS, NO_ADDR);

            // Copies the data from IC(Instruction Cache).
            uint32_t data_idx = static_cast<uint32_t> (paddr & DATA_SHIFT_MASK);
            fill_line->DataWriteFromIC (data, data_idx, iq_data_size);

            // Set L to 0 when a way which is L=1 and V=0 is filled.
            // Because don't come here when L=1 and V=1, don't need to check V value here.
            fill_line->SetL (false);

            // Updates LRU(Least Recently Used).
            UpdateLru (entry, mru_way);
        } else {
            // If "Cache Miss" and "All Ways are locked",
            // then reads the value from memory directly.
            // ("All Ways are locked" = "Target Way to update is locked")
            {
                m_fsbus->BusFetch (sideband, paddr, data, iq_data_size);
            }
        }

        // Records a Cache Miss
        m_fill_route = ICACHE_MISS_ROUTE;
        return err;
#ifdef FROG_API_IF
#else // FROG_API_IF
    }
#endif // FROG_API_IF
}


uint32_t Icache::FindWay4Way (uint32_t tag, uint32_t entry, uint32_t* prty) const
{
    if (m_icache[entry]->m_line[0]->IsHit (tag) == true) { return 0; }
    if (m_icache[entry]->m_line[1]->IsHit (tag) == true) { return 1; }
    if (m_icache[entry]->m_line[2]->IsHit (tag) == true) { return 2; }
    if (m_icache[entry]->m_line[3]->IsHit (tag) == true) { return 3; }
    return IC_MISS;  // Miss
}


uint32_t Icache::FindWay2Way (uint32_t tag, uint32_t entry, uint32_t* prty) const
{
    if (m_icache[entry]->m_line[0]->IsHit (tag) == true) { return 0; }
    if (m_icache[entry]->m_line[1]->IsHit (tag) == true) { return 1; }
    return IC_MISS;  // Miss
}


uint32_t Icache::FindWay1Way (uint32_t tag, uint32_t entry, uint32_t* prty) const
{
    if (m_icache[entry]->m_line[0]->IsHit (tag) == true) { return 0; }
    return IC_MISS;  // Miss
}


uint32_t Icache::FindWay0Way (uint32_t tag, uint32_t entry, uint32_t* prty) const
{
    return IC_MISS;  // Miss
}


uint32_t Icache::FindWayPbs (uint32_t tag, uint32_t entry, uint32_t* prty) const
{
    *prty = GetParity (tag);

    //! Even Parity VW0={WAY0,WAY1}
    //! Odd  Parity VW1={WAY2,WAY3}
    uint32_t way = *prty << 1;
    if (m_icache[entry]->m_line[way  ]->IsHit (tag) == true) { return way;   }
    if (m_icache[entry]->m_line[way+1]->IsHit (tag) == true) { return way+1; }
    return IC_MISS;  // Miss
}


uint32_t Icache::GetWayFromLru4Way (uint32_t entry, uint32_t prty) const
{
    //! Way0 : mask 111--- : chk_value 000---
    //! Way1 : mask 1--11- : chk_value 1--00-
    //! Way2 : mask -1-1-1 : chk_value -1-1-0
    //! Way3 : mask --1-11 : chk_value --1-11
    static const uint32_t lru_sel_mask[MAX_WAY] = { 0x38, 0x26, 0x15, 0x0B };
    static const uint32_t lru_chk_val[MAX_WAY]  = { 0x00, 0x20, 0x14, 0x0B };

    uint32_t lru_data = m_icache[entry]->m_lru;
    if ((lru_data & lru_sel_mask[0]) == lru_chk_val[0]) { return 0; }
    if ((lru_data & lru_sel_mask[1]) == lru_chk_val[1]) { return 1; }
    if ((lru_data & lru_sel_mask[2]) == lru_chk_val[2]) { return 2; }
    if ((lru_data & lru_sel_mask[3]) == lru_chk_val[3]) { return 3; }
    return IC_MISS;  // unreachable
}


uint32_t Icache::GetWayFromLru2Way (uint32_t entry, uint32_t prty) const
{
    //! Way0 : mask 1----- : chk_value 0-----
    //! Way1 : mask 1----- : chk_value 1-----
    //! Way2 : mask ------ : chk_value ------
    //! Way3 : mask ------ : chk_value ------
    static const uint32_t lru_sel_mask[MAX_WAY] = { 0x20, 0x20, 0x00, 0x00 };
    static const uint32_t lru_chk_val[MAX_WAY]  = { 0x00, 0x20, 0x00, 0x00 };

    uint32_t lru_data = m_icache[entry]->m_lru;
    if ((lru_data & lru_sel_mask[0]) == lru_chk_val[0]) { return 0; }
    if ((lru_data & lru_sel_mask[1]) == lru_chk_val[1]) { return 1; }
    return IC_MISS;  // unreachable
}


uint32_t Icache::GetWayFromLru1Way (uint32_t entry, uint32_t prty) const
{
    return 0;
}


uint32_t Icache::GetWayFromLruPbs (uint32_t entry, uint32_t prty) const
{
    //! Even Parity VW0={WAY0,WAY1}
    //! Odd  Parity VW1={WAY2,WAY3}
    //! Way0 : mask 1----- : chk_value 0-----
    //! Way1 : mask 1----- : chk_value 1-----
    //! Way2 : mask -----1 : chk_value -----0
    //! Way3 : mask -----1 : chk_value -----1
    static const uint32_t lru_sel_mask[MAX_WAY] = { 0x20, 0x20, 0x01, 0x01 };
    static const uint32_t lru_chk_val[MAX_WAY]  = { 0x00, 0x20, 0x00, 0x01 };

    uint32_t lru_data = m_icache[entry]->m_lru;
    uint32_t w = prty << 1;
    if ((lru_data & lru_sel_mask[w  ]) == lru_chk_val[w  ]) { return w;   }
    if ((lru_data & lru_sel_mask[w+1]) == lru_chk_val[w+1]) { return w+1; }
    return IC_MISS;  // unreachable
}


void Icache::UpdateLru (uint32_t entry, uint32_t way)
{
    //! Way0 : setval 111---
    //! Way1 : setval 0--11-
    //! Way2 : setval -0-0-1
    //! Way3 : setval --0-00
    uint32_t lru_data = m_icache[entry]->m_lru;
    uint32_t new_lru = (lru_data | m_icache[entry]->m_lru_on_mask[way])
                       & m_icache[entry]->m_lru_off_mask[way];
    m_icache[entry]->m_lru = new_lru;
}


void Icache::SetWayToLru (uint32_t entry, uint32_t way)
{
    //! Way0 : setval 000---
    //! Way1 : setval 1--00-
    //! Way2 : setval -1-1-0
    //! Way3 : setval --1-11
    static const uint32_t lru_on_mask_master[MAX_WAY] =
                                  { 0x38, 0x06, 0x01, 0x00 };
    static const uint32_t lru_off_mask_master[MAX_WAY] =
                                  { 0x3F, 0x1F, 0x2B, 0x34 };

    uint32_t lru_data = m_icache[entry]->m_lru;
    uint32_t new_lru = (lru_data | ~lru_off_mask_master[way])
                       & ~lru_on_mask_master[way];
    m_icache[entry]->m_lru = new_lru;
}


uint32_t Icache::GetTagFromAddr (PhysAddr paddr) const
{
    return static_cast<uint32_t> ((paddr >> m_tag_shift) & m_tag_mask);
}


uint32_t Icache::GetEntryFromAddr (PhysAddr vaddr) const
{
    return (vaddr >> m_entry_shift) & m_entry_mask;
}


uint32_t Icache::GetParity (uint32_t tag) const
{
    // Calculates the parity for 32 bit.
    uint32_t x = tag;
    x = x ^ (x >>  1);
    x = x ^ (x >>  2);
    x = x ^ (x >>  4);
    x = x ^ (x >>  8);
    x = x ^ (x >> 16);

    return (x & 1);
}


void Icache::IcacheLine::InitIcacheLine (void)
{
    m_tag      = 0x0U;
    m_v        = false;
    m_tag_ecc  = 0x0U;
    m_data_ecc = 0x0U;
    m_lru_lock = false;

    for (uint32_t i = 0; i < m_line_byte; ++i) {
        m_data[i] = 0x0U;
    }
}


bool Icache::IcacheLine::IsHit (uint32_t tag) const
{
    return (m_v == true) && (tag == m_tag);
};


void Icache::IcacheLine::DataWriteFromIC (MemData8* data, uint32_t data_idx, const ByteSize size)
{
    // Copies the data from Instruction Cache to Instruction Que.
    memcpy (data, m_data + data_idx, size);
}


Icache::IcacheLine::Data64 Icache::IcacheLine::GetLineData8B (uint32_t data_offset)
{
    void* void_ptr = m_data + data_offset;
    uint32_t* ld_data = static_cast<uint32_t*> (void_ptr);
    return Data64 (ld_data[1], ld_data[0]);
}


void Icache::IcacheLine::SetLineData8B (Data64 data, uint32_t data_offset)
{
    void* void_ptr = m_data + data_offset;
    uint32_t* st_data = static_cast<uint32_t*> (void_ptr);
    st_data[0] = data.m_l;
    st_data[1] = data.m_h;
}


SysErrCause Icache::IcacheLine::IcLineFill (PhysAddr fill_addr, uint32_t tag, SideBand sideband)
{
    // Set speculative type : For fetch, always enabled.
    sideband.SetSPCLD (Lsu::SPCLD_ON);

    SysErrCause err = m_fsbus->BusFetch (sideband, fill_addr & m_line_mask, m_data, m_line_byte);

    // Reset speculative type
    sideband.SetSPCLD (Lsu::SPCLD_OFF);

#ifdef ENABLE_ERROR_AREA
    if ((err == SYSERR_NONE) || (err == SYSERR_NOP))
#endif // #ifdef ENABLE_ERROR_AREA
    {
        m_tag = tag;
        m_v = true;
    }
#ifdef ENABLE_ERROR_AREA
    else {
        m_v = false; // invalidate the line if error occurs to prevent use error data.
    }
#endif // #ifdef ENABLE_ERROR_AREA
    return err;
}


void Icache::IcacheLine::FillZero (uint32_t tag)
{
    memset (m_data, 0x0, sizeof(uint8_t) * m_line_byte);
    m_tag = tag;
    m_v = true;
}


void Icache::IcacheArray::InitV (void)
{
    IcLineVec::const_iterator ite_line = m_line.begin ();
    while (ite_line != m_line.end()) {
        (*ite_line)->SetV (false);
        ++ite_line;
    }
}



void Icache::IcacheArray::InitLruAndLock (void)
{
    static const uint32_t lru_on_mask_master[MAX_WAY]  =
                                  { 0x38, 0x06, 0x01, 0x00 };
    static const uint32_t lru_off_mask_master[MAX_WAY] =
                                  { 0x3F, 0x1F, 0x2B, 0x34 };

    for (uint32_t i = 0; i < MAX_WAY; ++i) {
        m_line[i]->SetL(false);
        m_lru_on_mask [i] = lru_on_mask_master [i];
        m_lru_off_mask[i] = lru_off_mask_master[i];
    }
    m_lru = 0;
}


Icache::IcacheArray::IcacheArray (FsBus* fsbus, uint32_t line_byte)
    : m_lru (0x0)
{
    for (uint32_t i = 0; i < MAX_WAY; ++i) {
        m_line.push_back (new IcacheLine (fsbus, line_byte));
    }
}


Icache::IcacheArray::~IcacheArray ()
{
    IcLineVec::const_iterator ite = m_line.begin();
    while (ite != m_line.end()) {
        delete *ite;
        ++ite;
    }
}


/*!
 * @brief  Invalidates Cache line by Address.
 * @param  paddr Physical Address.
 * @param  vaddr Logical  Address.
 * @param  trace Trace information object for debugging.
 */
void Icache::OperateChbii (PhysAddr paddr, PhysAddr vaddr, TraceCtrl* trace)
{
    if (IsIcacheSizeZero ()) {
        return;
    }

    uint32_t tag   = GetTagFromAddr (paddr);
    uint32_t entry = GetEntryFromAddr (vaddr);

    // Searches the Way which hits a Cache.
    uint32_t prty = 0;
    uint32_t way  = (this->*pFindIcHitWay) (tag, entry, &prty);

    // If Cache Hit, then invalidates the way.
    if (way != IC_MISS) {
        IcacheLine* tgt_line = m_icache[entry]->m_line[way];

        IcacheLine* lru_line = m_icache[entry]->m_line[(this->*pGetLruWay) (entry, prty)];
        if (lru_line->IsLock ()) {
            // Only if all Ways are locked,
            // updates LRU to update target Ways by first priority.
            SetWayToLru (entry, way);
        }

        // If this Way is locked, then release it.
        if (tgt_line->IsLock ()) {
            UpdateLockWay (entry, way, false, trace);
        }
        tgt_line->SetL (false);

        // Sets this Way invalid.
        tgt_line->SetV (false);
        trace->RecordIcAccess (entry, way, IC_EVT_INV, NO_ADDR);
    }
}


/*!
 * @brief  Invalidate Cache line by Line.
 * @param  data Data to specify the Cache line.
 * @param  trace Trace information object for debugging.
 */
void Icache::OperateCibii (uint32_t data, TraceCtrl* trace)
{
    if (IsIcacheSizeZero ()) {
        return;
    }

    // Searches the Entry and the Way by the argument,
    uint32_t entry = (data >> m_entry_shift) & m_entry_mask;
    uint32_t way = (data >> m_way_shift) & m_way_mask;
    IcacheLine* tgt_line = m_icache[entry]->m_line[way];

    uint32_t prty = (way >> 1) & 0x1;
    IcacheLine* lru_line = m_icache[entry]->m_line[(this->*pGetLruWay) (entry, prty)];
    if (lru_line->IsLock ()) {
        // Only if all Ways are locked,
        // updates LRU to update target Ways by first priority.
        SetWayToLru (entry, way);
    }

    // If this Way was locked, then release it.
    if (tgt_line->IsLock ()) {
        UpdateLockWay (entry, way, false, trace);
    }
    tgt_line->SetL (false);

    // Sets this Way invalid.
    tgt_line->SetV (false);
    trace->RecordIcAccess (entry, way, IC_EVT_INV, NO_ADDR);

}

/*!
 * @brief  Operates for CFALI instruction.
 * @param  paddr   Physical Address.
 * @param  vaddr   Logical  Address.
 * @param  sideband Side band.
 * @param  trace Trace information object for debugging.
 */
void Icache::OperateCfali (PhysAddr paddr, PhysAddr vaddr, SideBand sideband, TraceCtrl* trace)
{
    m_fill_route = ICACHE_HIT_ROUTE; // show fill(MISS) or not fill(HIT)
    if (IsIcacheSizeZero ()) {
        return;
    }

    uint32_t tag   = GetTagFromAddr (paddr);
    uint32_t entry = GetEntryFromAddr (vaddr);

    // Searches the Way which hits a Cache.
    uint32_t prty = 0;
    uint32_t way = (this->*pFindIcHitWay) (tag, entry, &prty);

    // Catches Cache Hit.
    if (way != IC_MISS) {
        // If "Cache Hit" and "Way is not locked", then locks the Way.
        IcacheLine* tgt_line = m_icache[entry]->m_line[way];
        if (likely (tgt_line->IsLock () == false)) {
            // Locks the Way.
            UpdateLockWay (entry, way, true, trace);
            tgt_line->SetL (true);
            UpdateLru (entry, way);
        }
    } else {
        // If "Cache Miss" and "Not all Ways are locked",
        // fills the cache, and locks the Way.
        // Searches the target Way to update.
        uint32_t lru_way = (this->*pGetLruWay) (entry, prty);
        IcacheLine* lru_line = m_icache[entry]->m_line[lru_way];

        if (likely (lru_line->IsLock () == false)) {
            // Fills the Cache
            m_fill_route = ICACHE_MISS_ROUTE;
            {
                SysErrCause err_fill = lru_line->IcLineFill (paddr, tag, sideband);
#ifdef ENABLE_ERROR_AREA
                if (err_fill != SYSERR_NONE) {
                    if (err_fill == SYSERR_NOP) {
                        // If NOP, then not generate SYSERR, but reads 0 and normal end.
                    } else {
                        return;
                    }
                }
#endif // #ifdef ENABLE_ERROR_AREA
            }
            trace->RecordIcAccess (entry, lru_way, IC_EVT_FILL, paddr & LINE_MASK);

            // Locks the Way.
            UpdateLockWay (entry, lru_way, true, trace);
            lru_line->SetL (true);
            UpdateLru (entry, lru_way);
        }
    }
}

/*!
 * @brief  Locks the target Way.
 * @param  entry Target Entry.
 * @param  way   Target Way.
 * @param  lock  Lock setting.
 * @param  trace Trace information object for debugging.
 */
void Icache::UpdateLockWay (uint32_t entry, uint32_t way, bool lock, TraceCtrl* trace)
{
    static const uint32_t lru_change_mask[MAX_WAY] =
                                { 0x38, 0x26, 0x15, 0x0B };

    // Updates m_lru_on_mask and m_lru_off_mask.
    for (uint32_t i = 0; i < MAX_WAY; ++i) {
        // No need to update the Mask of the Target Way to Lock.
        if (i == way) { continue; }

        // Inverts the bits of the mask of LRU to update about Target Way to lock,
        // to set the lock-target Way always keeps latest status.
        uint32_t change_mask = lru_change_mask[i] & lru_change_mask[way];
        m_icache[entry]->m_lru_on_mask[i]  ^= change_mask;
        m_icache[entry]->m_lru_off_mask[i] ^= change_mask;
    }

    if (lock == true) {
        trace->RecordIcAccess (entry, way, IC_EVT_LOCK, NO_ADDR);
    } else {
        trace->RecordIcAccess (entry, way, IC_EVT_UNLOCK, NO_ADDR);
    }
}


/*!
 * @brief  Operates for CISTI instruction.
 * @param  data   Data to specify the Cache line.
 * @param  nc_srf SFR(Special Function Register) object.
 * @param  trace Trace information object for debugging.
 */
void Icache::OperateCisti (uint32_t data, NcSRegFile* nc_srf, TraceCtrl* trace)
{
    if (IsIcacheSizeZero ()) {
        return;
    }

    // Searches the Entry and the Way by the argument.
    uint32_t entry = (data >> m_entry_shift) & m_entry_mask;
    uint32_t way = (data >> m_way_shift) & m_way_mask;
    uint32_t data_offset = data & DATA_OFFSET_MASK;
    IcacheLine* tgt_line = m_icache[entry]->m_line[way];

    trace->RecordIcAccess (entry, way, IC_EVT_ST, NO_ADDR);

    // Records Lock status.
    bool is_lock = tgt_line->IsLock ();

    // Updates the tag.
    if (nc_srf->SrICTAGH()->GetWT ()) {
        uint32_t tag_value = nc_srf->SrICTAGL()->GetLPN ();
        tgt_line->SetTag (tag_value);
    }

    // Updates V bit.
    if (nc_srf->SrICTAGH()->GetWT ()) {
        tgt_line->SetV (nc_srf->SrICTAGL()->GetV());
    }

    // Updates the data.
    if (nc_srf->SrICTAGH()->GetWD ()) {
        IcacheLine::Data64 ic_data (0,0);
        ic_data.SetICDATH (nc_srf->SrICDATH()->ReadBody ());
        ic_data.SetICDATL (nc_srf->SrICDATL()->ReadBody ());
        tgt_line->SetLineData8B (ic_data, data_offset);
    }

    // Updates the ECC of tag.
    if (nc_srf->SrICTAGH()->GetPT()) {
        tgt_line->SetTagEcc (nc_srf->SrICTAGH()->GetTAGECC ());
    }

    // Updates the ECC of data.
    if (nc_srf->SrICTAGH()->GetPD()) {
        tgt_line->SetDataEcc (nc_srf->SrICTAGH()->GetDATAECC ());
    }

    // Updates the Way Lock.
    if (nc_srf->SrICTAGH()->GetWT ()) {
        bool l_value = nc_srf->SrICTAGL()->GetL () & nc_srf->SrICTAGL()->GetV();
        if (l_value != is_lock) {
            if (l_value == true) {
                UpdateLockWay (entry, way, true, trace);
                UpdateLru (entry, way);
            } else {
                uint32_t prty = (way >> 1) & 0x1;
                uint32_t lru_way = (this->*pGetLruWay) (entry, prty);
                if (m_icache[entry]->m_line[lru_way]->IsLock ()) {
                    // Only if all Ways are locked,
                    // updates LRU to update target Ways by first priority.
                    SetWayToLru (entry, way);
                }
                UpdateLockWay (entry, way, false, trace);
            }
        }
        tgt_line->SetL (nc_srf->SrICTAGL()->GetL ());
    }
}


/*!
 * @brief  Operates for CILDI instruction.
 * @param  data   Data to specify the Cache line.
 * @param  nc_srf SFR(Special Function Register) object.
 * @param  trace Trace information object for debugging.
 */
void Icache::OperateCildi (uint32_t data, NcSRegFile* nc_srf, TraceCtrl* trace)
{
    if (IsIcacheSizeZero ()) {
        return;
    }

    // Searches the Entry and the Way by the argument,
    uint32_t entry = (data >> m_entry_shift) & m_entry_mask;
    uint32_t way = (data >> m_way_shift) & m_way_mask;
    uint32_t data_offset = data & DATA_OFFSET_MASK;
    IcacheLine* tgt_line = m_icache[entry]->m_line[way];

    // Searches the Way number to update next, by the Entry.
    uint32_t prty = (way >> 1) & 0x1;
    uint32_t mru_way = (this->*pGetLruWay) (entry, prty);

    // Sets the value of ICTAGL.
    uint32_t tag_data  = tgt_line->GetTag ();
    uint32_t ppnl_mask = (1 << ((TAG_UPPER_POS + 1) - m_tag_shift)) - 1;
    nc_srf->SrICTAGL()->SetLPN  (tag_data & ppnl_mask);
    nc_srf->SrICTAGL()->SetLRU  (mru_way);
    nc_srf->SrICTAGL()->SetL    (tgt_line->GetL ());
    nc_srf->SrICTAGL()->SetV    (tgt_line->GetV ());

    // Sets the value of ICTAGH.
    nc_srf->SrICTAGH()->SetDATAECC (tgt_line->GetDataEcc ());
    nc_srf->SrICTAGH()->SetTAGECC  (tgt_line->GetTagEcc ());

    // Sets the values of ICDATL and ICDATH.
    IcacheLine::Data64 ic_data = tgt_line->GetLineData8B (data_offset);
    uint32_t icdatl = ic_data.GetICDATL ();
    uint32_t icdath = ic_data.GetICDATH ();
    if (m_core_type == CORE_G4KH) {
        static const uint32_t DATA_HW_SHIFT = 16;
        static const uint32_t DATA_HW_H_MASK = 0xFFFF0000U;
        static const uint32_t DATA_HW_L_MASK = 0x0000FFFFU;
        uint32_t data_tmp = ((icdath << DATA_HW_SHIFT) & DATA_HW_H_MASK)
                          | ((icdath >> DATA_HW_SHIFT) & DATA_HW_L_MASK);
        icdath = ((icdatl << DATA_HW_SHIFT) & DATA_HW_H_MASK)
               | ((icdatl >> DATA_HW_SHIFT) & DATA_HW_L_MASK);
        icdatl = data_tmp;
    }
    nc_srf->SrICDATL()->WriteBody (icdatl);
    nc_srf->SrICDATH()->WriteBody (icdath);

    trace->RecordIcAccess (entry, way, IC_EVT_LD, NO_ADDR);
}


/*!
 * @brief  Operates for Pre-Fetch Instruction.
 * @param  paddr Physical Address.
 * @param  vaddr Logical  Address.
 * @param  sideband Side Band.
 * @param  trace Trace information object for debugging.
 */
void Icache::OperatePref (PhysAddr paddr, PhysAddr vaddr, SideBand sideband, TraceCtrl* trace)
{
    m_fill_route = ICACHE_HIT_ROUTE; // show fill(MISS) or not fill(HIT)
    if (IsIcacheSizeZero ()) {
        return;
    }

    uint32_t tag   = GetTagFromAddr (paddr);
    uint32_t entry = GetEntryFromAddr (vaddr);

    // Searches the Way which hits a Cache.
    uint32_t prty = 0;
    uint32_t way = (this->*pFindIcHitWay) (tag, entry, &prty);

    if (way == IC_MISS) {
        // Cache Miss
        uint32_t lru_way = (this->*pGetLruWay) (entry, prty);
        IcacheLine* lru_line = m_icache[entry]->m_line[lru_way];
        m_fill_route = ICACHE_MISS_ROUTE;
        if (likely (lru_line->IsLock () == false)) {
            {
                SysErrCause err_fill = lru_line->IcLineFill (paddr, tag, sideband);
#ifdef ENABLE_ERROR_AREA
                if (err_fill != SYSERR_NONE) {
                    if (err_fill == SYSERR_NOP) {
                        // If NOP, then not generate SYSERR, but reads 0 and normal end.
                    } else {
                        return;
                    }
                }
#endif // #ifdef ENABLE_ERROR_AREA
            }
            // Set L to 0 when a way which is L=1 and V=0 is filled.
            // Because don't come here when L=1 and V=1, don't need to check V value here.
            lru_line->SetL (false);

            UpdateLru (entry, lru_way);
            trace->RecordIcAccess (entry, lru_way, IC_EVT_FILL, paddr & LINE_MASK);

        } // else : Do nothing if all Ways are locked.
    } else {
        // Cache hit : Only update LRU.
        UpdateLru (entry, way);
        trace->RecordIcAccess (entry, way, IC_EVT_NOP, NO_ADDR);
        // don't increment ic_hit_count.
    }
}


bool Icache::IsCachingArea (PhysAddr paddr) const
{
    // The whole ROM area is valid for caching.
    // Even if Any other PE's ROM areas become invalid for caching, CForest doesn't support.
    if (paddr >= m_rom_addrl && paddr <= m_rom_addru) {
        return true;
    } else {
        return false;
    }
}


void Icache::SetRomAreaAddr (PhysAddr rom_addrl, PhysAddr rom_addru)
{
    m_rom_addrl = rom_addrl;
    m_rom_addru = rom_addru;
}


void Icache::Invalidate (void)
{
    IcArrayVec::const_iterator ite_array = m_icache.begin();
    while (ite_array != m_icache.end()) {
        (*ite_array)->InitV ();
        (*ite_array)->InitLruAndLock ();
        ++ite_array;
    }
}


void Icache::ResetPerfCounter (void)
{
    m_hit_count  = 0;
    m_miss_count = 0;
}


void Icache::Reset (void)
{
    Invalidate ();
    ResetPerfCounter ();
}


/*!
 * @brief  Generates Read-data for ICCFG Register.
 * @return Read-data of ICCFG Register.
 */
uint32_t Icache::ReadIccfgData (void) const
{
    if (m_ic_size == 0) { return 0; }

    // Reads the size(ICHSIZE bits, in KB) of Instruction Cache.
    uint32_t ret = (m_ic_size / 1024) << 8;

    // Reads the total number of the entry(ICHLINE bits, Lines per 1 way).
    ret |= (m_entry_num >> 1);

    // Reads the Way type(ICHWAY bits, number of the Ways) .
    ret |= GetWayNum ();

    return ret;
}


uint32_t Icache::GetWayNum (void) const
{
    uint32_t way;

    // Reads the Way type.
    switch (m_way_type) {
    case WAY_PBS: way = 4; break;
    case WAY4:    way = 4; break;
    case WAY2:    way = 2; break;
    case WAY1:    way = 1; break;
    default:      way = 0; break;
    }

    return way;
}


void Icache::SetIcacheWayType (CacheWayType way)
{
    m_way_type = way;
    SetWayType (way);
    uint32_t waynum = GetWayNum ();
    if (waynum != 0) {
        m_way_mask = waynum - 1;
    } else {
        m_way_mask = 0;
    }

    UpdateEntryParam ();
}


void Icache::SetIcacheSize (ByteSize size)
{
    m_ic_size = size;

    UpdateEntryParam ();
}


bool Icache::IsIcacheSizeZero (void) const
{
    return m_entry_num == 0;
}


// When to update the Instruction Cache size and Way size.
void Icache::UpdateEntryParam (void)
{
    m_entry_num = CalEntryNum(m_ic_size, m_way_type);
    if (m_icache.empty () == false) {
        IcArrayVec::const_iterator ite = m_icache.begin();
        while (ite != m_icache.end()) {
            delete *ite;
            ++ite;
        }
    }
    m_icache.clear ();

    // Calculates the shift value of Tag/Way.
    {
        uint32_t en = m_entry_num;
        uint32_t pos = 0;
        while (en > 1) {
            ++ pos;
            en >>= 1;
        }
        m_tag_shift = m_way_shift = pos + m_entry_shift;
    }

    if (m_entry_num != 0) {
        for (uint32_t entry=0; entry<m_entry_num; ++entry) {
            m_icache.push_back (new IcacheArray (m_fsbus, LINE_BYTES));
        }
    } else {
        m_icache.push_back (new IcacheArray (m_fsbus, LINE_BYTES));
    }

    if (m_entry_num != 0) {
        m_entry_mask = m_entry_num - 1;  // If the total number of the entry is 256, then 0xFF.
    } else {
        m_entry_mask = 0;
    }

    Invalidate ();
}


void Icache::ShowPerfInfo (void) const
{
    uint64_t ic_access = GetAccessCount ();
    if (ic_access != 0) {
        uint64_t hit = GetHitCount ();
        uint64_t miss = GetMissCount ();
        double hit_rate = (static_cast<double>(hit) / static_cast<double>(ic_access)) * 100.0;
        CfMsg::DPrint (MSG_ERR,
                       "  I cache (P%u): Hit Rate=%3.3f%% (Hit=%8" PRIu64 ", Miss=%8" PRIu64 ")\n",
                       m_peid, hit_rate, hit, miss);
    }
}


void Icache::ResetPerfInfo (void)
{
    ResetPerfCounter ();
}


Icache::Icache (ByteSize ic_size, CacheWayType way_type, FsBus* fsbus,
                PhysAddr rom_addrl, PhysAddr rom_addru, uint32_t peid,
                uint32_t line_byte, CoreType core_type)
    : LINE_BYTES (line_byte), LINE_MASK (~(LINE_BYTES - 1)), DATA_SHIFT_MASK (LINE_BYTES - 1),
      DATA_OFFSET_MASK (LINE_BYTES - 0x08),
      m_core_type (core_type), m_ic_size (ic_size),
      m_way_type (way_type), m_fsbus (fsbus),
      m_rom_addrl (rom_addrl), m_rom_addru (rom_addru), m_peid (peid)
{
    m_icache.clear ();

    // Calculates the shift value(=log2(LINE_BYTES)) of the entry.
    uint32_t lb  = LINE_BYTES;
    uint32_t pos = 0;
    while (lb > 1) {
        ++ pos;
        lb >>= 1;
    }
    m_entry_shift = pos;

    m_tag_mask = 0xFFFFFFFFU;
    m_enable_ecc = true;

    SetIcacheWayType (way_type);
    Invalidate ();
    ResetPerfCounter ();
}


Icache::~Icache ()
{
    if (m_icache.empty () == false) {
        IcArrayVec::const_iterator ite = m_icache.begin();
        while (ite != m_icache.end()) {
            delete *ite;
            ++ite;
        }
    }
}


Icache::IcacheLine::IcacheLine (FsBus* fsbus, uint32_t line_byte)
    : m_fsbus (fsbus),
      m_line_byte (line_byte),
      m_line_mask (~(static_cast<PhysAddr> (line_byte) - 1))
{
    m_data = new MemData8[m_line_byte];
    InitIcacheLine ();
}


Icache::IcacheLine::~IcacheLine ()
{
    delete [] m_data;
}
