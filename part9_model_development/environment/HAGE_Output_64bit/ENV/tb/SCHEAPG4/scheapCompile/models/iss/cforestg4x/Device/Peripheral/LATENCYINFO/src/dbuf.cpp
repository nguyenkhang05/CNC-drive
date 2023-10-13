/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This proGramWtb must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * proGramWtb may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <vector>
#include "./config.h"
#include "./dbuf.h"
#include "./forest_utility.h"
#include "./forest_message.h"

const uint32_t Dbuf::DBUF_HIT_LATENCY_G4MH = 2;
const uint32_t Dbuf::DBUF_HIT_LATENCY_G4KH = 3;


bool Dbuf::IsHitDbuf (uint32_t entry, PhysAddr load_addr) const
{
    // FlashROMにDBUFがあるのでレイテンシを削減
    return m_dbuf_entry[entry]->m_dbuf_paddr == (load_addr & DBUF_ADDR_MASK);
}


uint64_t Dbuf::GetDbufCycle (uint32_t entry, PhysAddr addr) const
{
    return m_dbuf_entry[entry]->m_dbuf_avail_cycle[GetWayId (addr)];
}


uint32_t Dbuf::GetWayId (PhysAddr load_addr) const
{
    return static_cast<uint32_t>((load_addr & (DBUF_LINE_BYTE - 1)) >>
            (1 + DBUF_ADDR_SHIFT - DBUF_FILL_BURST_SIZE)) & (DBUF_FILL_BURST_SIZE - 1);
}


uint64_t Dbuf::UpdateDbuf (uint32_t entry, PhysAddr load_addr, uint64_t ready_cycle,
        uint32_t repeat_cycle)
{
    uint32_t way_1st = GetWayId (load_addr);
    for (uint32_t i = way_1st; i < DBUF_FILL_BURST_SIZE; i++) {
        m_dbuf_entry[entry]->m_dbuf_avail_cycle[i] = ready_cycle;
        ready_cycle += repeat_cycle;
    }
    for (uint32_t i = 0; i < way_1st; i++) {
        m_dbuf_entry[entry]->m_dbuf_avail_cycle[i] = ready_cycle;
        ready_cycle += repeat_cycle;
    }

    m_dbuf_entry[entry]->m_dbuf_paddr = load_addr & DBUF_ADDR_MASK;
    SetLastFillAddr (load_addr & 0xFFFFFFE0);
    SetLastPreloadAddr (0xFFFFFFFF); // invalidate

    return ready_cycle - repeat_cycle;
}


void Dbuf::UpdateDbufFillNext (uint32_t entry, PhysAddr load_addr, uint64_t ready_cycle)
{
    PhysAddr base_addr_dbuf = load_addr & DBUF_ADDR_MASK;
    PhysAddr base_addr_flash = load_addr & ~(FLASH_SIZE - 1);
    if (base_addr_dbuf == base_addr_flash) {
        load_addr += DBUF_LINE_BYTE;
    } else {
        load_addr -= DBUF_LINE_BYTE;
    }

    uint32_t preload_entry = GetDbufEntry (load_addr);
    if (IsHitDbuf (preload_entry, load_addr) == true) {
        // DBUFヒット、または、エントリ数１なのでPreloadしない
        return;
    }
    uint32_t replace_entry = GetReplaceEntry (load_addr);

    // Flash256bit幅（または128bit*2接続)による、
    // 他DBUFエントリの128bitフィルの余りの128bitでフィル。
    // それ以外の構成は非サポート。
    // - DBUF_FILL_BURST_SIZE = 1でない構成では、サイクル数が想定より早くなるので注意
    for (uint32_t i = 0; i < DBUF_FILL_BURST_SIZE; i++) {
        m_dbuf_entry[replace_entry]->m_dbuf_avail_cycle[i] = ready_cycle;
    }
    m_dbuf_entry[replace_entry]->m_dbuf_paddr = load_addr & DBUF_ADDR_MASK;
    UpdateReplaceEntry ();
}


void Dbuf::UpdateDbufPreload (uint32_t entry, PhysAddr load_addr, uint64_t ready_cycle)
{
    // Only for Flash Width 256, DBUF=128
    PhysAddr preload_addr = load_addr & ~(PRELOAD_SIZE - 1);
    //switch (m_preload_type) {
    //case DBUF_PRLD_NO:    return; break;
    //case DBUF_PRLD_PLUS:  preload_addr += PRELOAD_SIZE; break;
    //case DBUF_PRLD_MINUS: preload_addr -= PRELOAD_SIZE; break;
    //}

    // printf ("PRELOAD_SIZE=%02x, DBUF_ADDR_MASK=%08x, DBUF_FILL_BURST_SIZE=%x, FLASH_SIZE=%x, DBUF_LINE_BYTE=%x\n",
    //    PRELOAD_SIZE, DBUF_ADDR_MASK, DBUF_FILL_BURST_SIZE, FLASH_SIZE, DBUF_LINE_BYTE);
    // PRELOAD_SIZE=0x20, DBUF_ADDR_MASK=fffffff0, DBUF_FILL_BURST_SIZE=0x1, FLASH_SIZE=0x20, DBUF_LINE_BYTE=0x10

    SetLastFillAddr (preload_addr);
    SetLastPreloadAddr (preload_addr);

    // 1st entry
    preload_addr &= DBUF_ADDR_MASK;
    uint32_t preload_entry = GetDbufEntry (preload_addr & DBUF_ADDR_MASK);
    if (IsHitDbuf (preload_entry, preload_addr) == false) {
        uint32_t replace_entry = GetReplaceEntry (preload_addr);
        for (uint32_t i = 0; i < DBUF_FILL_BURST_SIZE; i++) {
            m_dbuf_entry[replace_entry]->m_dbuf_avail_cycle[i] = ready_cycle;
        }
        m_dbuf_entry[replace_entry]->m_dbuf_paddr = preload_addr & DBUF_ADDR_MASK;
        UpdateReplaceEntry ();
    }

    if (FLASH_SIZE > DBUF_LINE_BYTE) {
        // 2nd entry
        // Flash256bit幅（または128bit*2接続)による、
        // 他DBUFエントリの128bitフィルの余りの128bitでフィル。
        // それ以外の構成は非サポート。
        // - DBUF_FILL_BURST_SIZE = 1でない構成では、サイクル数が想定より早くなるので注意
        PhysAddr preload_addr2 = preload_addr + DBUF_LINE_BYTE;
        uint32_t preload_entry2 = GetDbufEntry (preload_addr2 & DBUF_ADDR_MASK);
        if (IsHitDbuf (preload_entry2, preload_addr2) == false) {
            uint32_t replace_entry = GetReplaceEntry (preload_addr2);
            for (uint32_t i = 0; i < DBUF_FILL_BURST_SIZE; i++) {
                m_dbuf_entry[replace_entry]->m_dbuf_avail_cycle[i] = ready_cycle;
            }
            m_dbuf_entry[replace_entry]->m_dbuf_paddr = preload_addr2 & DBUF_ADDR_MASK;
            UpdateReplaceEntry ();
        }
    }
}


uint32_t Dbuf::GetDbufEntry (PhysAddr load_addr) const
{
    if (DBUF_REPLACE_TYPE == DBUF_REP_ROUNDR) {
        for (uint32_t entry = 0; entry < DBUF_DEPTH; entry++) {
            if (IsHitDbuf (entry, load_addr) == true) {
                return entry;
            }
        }
        return 0; // Miss. (Return any.)
    } else {
        return static_cast<uint32_t>((load_addr & DBUF_ADDR_MASK) >> DBUF_ADDR_SHIFT) &
                                     (DBUF_DEPTH - 1);
    }
}


uint32_t Dbuf::GetReplaceEntry (PhysAddr load_addr) const
{
    if (DBUF_REPLACE_TYPE == DBUF_REP_ROUNDR) {
        return m_round_robin_entry;
    } else {
        return GetDbufEntry (load_addr);
    }
}

void Dbuf::UpdateReplaceEntry (void)
{
    if (DBUF_REPLACE_TYPE == DBUF_REP_ROUNDR) {
        ++ m_round_robin_entry;
        if (DBUF_DEPTH == m_round_robin_entry) {
            m_round_robin_entry = 0;
        }
    }
}


void Dbuf::DumpDbuf (void) const
{
    CfMsg::DPrint (MSG_ERR, "DBUF(P%u) entry (depth=%d)\n", m_peid, DBUF_DEPTH);
    for (unsigned int j = 0; j < DBUF_DEPTH; j++) {
        for (unsigned int i = 0; i < DBUF_FILL_BURST_SIZE; i++) {
            CfMsg::DPrint (MSG_ERR, "[%d]=%08" PRIx32 " @ %08" PRIu64 "cycle\n",
                       j, m_dbuf_entry[j]->m_dbuf_paddr, m_dbuf_entry[j]->m_dbuf_avail_cycle[i]);
        }
    }
}


void Dbuf::HardReset (void)
{
    for (uint32_t i = 0; i < DBUF_DEPTH; i++) {
        m_dbuf_entry[i]->m_dbuf_paddr = ~DBUF_ADDR_MASK; // DBUFヒットしない値
        for (uint32_t j = 0; j < DBUF_FILL_BURST_SIZE; j++) {
            m_dbuf_entry[i]->m_dbuf_avail_cycle[j] = 0;
        }
    }

    ResetPerfInfo ();
    m_round_robin_entry = 0;
    m_last_fill_addr = 0xFFFFFFFF;
    m_last_preload_addr = 0xFFFFFFFF;
    m_is_preload_plus = true;
}


void Dbuf::ShowPerfInfo (void) const
{
    if (m_hit_count + m_miss_count != 0) {
        double hit_rate = ( static_cast<double>(m_hit_count)
                          / static_cast<double>(m_hit_count + m_miss_count)
                          ) * 100.0;
        CfMsg::DPrint (MSG_ERR,
                "  DBUF (P%u): Hit Rate=%3.3f%% (Hit=%8" PRIu64 ", Miss=%8" PRIu64 ")\n",
                m_peid, hit_rate, m_hit_count, m_miss_count);
    }
}


void Dbuf::ResetPerfInfo (void)
{
    m_hit_count = 0;
    m_miss_count = 0;
}


Dbuf::Dbuf (uint32_t peid, uint32_t depth, uint32_t line_shift, uint32_t fill_num,
            DbufReplaceType replace_type, uint32_t flash_width, CoreType core_type)
    : m_peid (peid & CL_PEID_MASK), DBUF_DEPTH (depth), DBUF_ADDR_SHIFT (line_shift),
      DBUF_FILL_BURST_SIZE (fill_num), DBUF_LINE_BYTE (1 << DBUF_ADDR_SHIFT),
      DBUF_ADDR_MASK (~(DBUF_LINE_BYTE - 1)), DBUF_REPLACE_TYPE (replace_type),
      FLASH_SIZE (flash_width), PRELOAD_SIZE (std::max (DBUF_LINE_BYTE, FLASH_SIZE))
{
    //printf ("depth=%u, line_shift=%u, fill_num=%u, flash_width=%u, DBUF_LINE_BYTE=%u\n", depth, line_shift, fill_num, flash_width, DBUF_LINE_BYTE);
    for (uint32_t i = 0; i < DBUF_DEPTH; i++) {
        m_dbuf_entry.push_back (new DbufEntry (DBUF_FILL_BURST_SIZE));
    }
#if _UNIT_TEST
    m_preload_type = DBUF_PRLD_NO;
#else // #if _UNIT_TEST
    m_preload_type = p_internals->GetDbufPreloadType ();
#endif // #if _UNIT_TEST
    HardReset ();
}


Dbuf::~Dbuf ()
{
    std::vector<DbufEntry*>::iterator it = m_dbuf_entry.begin ();
    while (it != m_dbuf_entry.end ()) {
        delete (*it);
        ++ it;
    }
}


Dbuf::DbufEntry::DbufEntry (uint32_t fill_burst_num)
{
    m_dbuf_avail_cycle.assign (fill_burst_num, 0UL);
}
