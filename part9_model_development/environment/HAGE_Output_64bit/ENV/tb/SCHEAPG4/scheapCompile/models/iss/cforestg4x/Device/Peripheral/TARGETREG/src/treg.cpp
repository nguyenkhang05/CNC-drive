/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./treg.h"


bool TRegBase::IsRegAccessSizeValid (ByteSize size) const
{
    if (m_min_access_size <= size && m_reg_size >= size) {
        return true;
    } else {
        return false;
    }
}


TRegBase* TRegModuleVc::GetTrPtr (PhysAddr addr) const
{
    TRegMap::const_iterator ite = m_trf.find (addr);
    if (ite != m_trf.end ()) {
        return ite->second;
    } else {
        // 指定したアドレスより大きい要素が最初に現れる位置を取得
        ite = m_trf.upper_bound (addr);
        if (ite == m_trf.begin ()) {
            // マップの先頭が返る場合、該当する
            // ターゲットレジスタがないのでダミーを返す。
            return m_dummy;
        } else {
            -- ite;
            // 一つ前の要素が、該当するターゲットレジスタであるか判定
            if (ite->second->IsSearchHit (addr)) {
                return ite->second;
            } else {
                return m_dummy;
            }
        }
    }
}


template <class TR>
SysErrCause TRegDataBase<TR>::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size == SIZE_32BIT)      m_treg->Write32 (addr, data, m_reg_mask);
    else if (size == SIZE_16BIT) m_treg->Write16 (addr, data, m_reg_mask);
    else if (size == SIZE_8BIT)  m_treg->Write8  (addr, data, m_reg_mask);
    return SYSERR_NONE;
}


template <class TR>
SysErrCause TRegDataBase<TR>::Write (SideBand sideband,
        PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    } else {
        return WriteBody (addr, data, size);
    }
}


template <class TR>
SysErrCause TRegDataBase<TR>::ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (size == SIZE_32BIT)      m_treg->Read32 (addr, data);
    else if (size == SIZE_16BIT) m_treg->Read16 (addr, data);
    else if (size == SIZE_8BIT)  m_treg->Read8  (addr, data);
    return SYSERR_NONE;
}


template <class TR>
SysErrCause TRegDataBase<TR>::Read (SideBand sideband, PhysAddr addr, uint8_t* data,
                                    ByteSize size) const
{
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        std::fill_n (data, size, 0);
        return SYSERR_NONE;
    } else {
        return ReadBody (addr, data, size);
    }
}


void TRegData32::Write32 (PhysAddr addr, uint8_t* data, const TRegMask mask)
{
    m_data32 = (*(reinterpret_cast<uint32_t*> (data))) & mask.m_mask32;
}


void TRegData32::Read32 (PhysAddr addr, uint8_t* data) const
{
    *(reinterpret_cast<uint32_t*> (data)) = m_data32;
}


void TRegData32::Write16 (PhysAddr addr, uint8_t* data, const TRegMask mask)
{
    uint32_t index = (addr >> 1) & 0x1;
    m_data16[index] = (*(reinterpret_cast<uint16_t*> (data))) & mask.m_mask16[index];
}


void TRegData32::Read16 (PhysAddr addr, uint8_t* data) const
{
    uint32_t index = (addr >> 1) & 0x1;
    *(reinterpret_cast<uint16_t*> (data)) = m_data16[index];
}


void TRegData32::Write8 (PhysAddr addr, uint8_t* data, const TRegMask mask)
{
    uint32_t index = addr & 0x3;
    m_data8[index] = (*data) & mask.m_mask8[index];
}


void TRegData32::Read8 (PhysAddr addr, uint8_t* data) const
{
    uint32_t index = addr & 0x3;
    *data = m_data8[index];
}


void TRegData16::Write16 (PhysAddr addr, uint8_t* data, const TRegMask mask)
{
    m_data16 = (*(reinterpret_cast<uint16_t*> (data))) & mask.m_mask16[0];
}


void TRegData16::Read16 (PhysAddr addr, uint8_t* data) const
{
    *(reinterpret_cast<uint16_t*> (data)) = m_data16;
}


void TRegData16::Write8 (PhysAddr addr, uint8_t* data, const TRegMask mask)
{
    uint32_t index = addr & 0x1;
    m_data8[index] = (*data) & mask.m_mask8[index];
}


void TRegData16::Read8 (PhysAddr addr, uint8_t* data) const
{
    uint32_t index = addr & 0x1;
    *data = m_data8[index];
}


void TRegData8::Write8 (PhysAddr addr, uint8_t* data, const TRegMask mask)
{
    m_data8 = (*data) & mask.m_mask8[0];
}


void TRegData8::Read8 (PhysAddr addr, uint8_t* data) const
{
    *data = m_data8;
}


void TRegModuleVc::DelTRegMap (void)
{
    TRegMap::iterator ite = m_trf.begin();
    while (ite != m_trf.end()) {
        delete (ite->second);
        ++ ite;
    }
}


void TRegModuleVc::ResetTRegs (void)
{
    TRegMap::iterator ite = m_trf.begin();
    while (ite != m_trf.end()) {
        ite->second->Reset();
        ++ ite;
    }
}


template SysErrCause TRegDataBase<TRegData8>::WriteBody (PhysAddr addr,
        uint8_t* data, ByteSize size);
template SysErrCause TRegDataBase<TRegData16>::WriteBody (PhysAddr addr,
        uint8_t* data, ByteSize size);
template SysErrCause TRegDataBase<TRegData32>::WriteBody (PhysAddr addr,
        uint8_t* data, ByteSize size);
template SysErrCause TRegDataBase<TRegData8>::Write (SideBand sideband,
        PhysAddr addr, uint8_t* data, ByteSize size);
template SysErrCause TRegDataBase<TRegData16>::Write (SideBand sideband,
        PhysAddr addr, uint8_t* data, ByteSize size);
template SysErrCause TRegDataBase<TRegData32>::Write (SideBand sideband,
        PhysAddr addr, uint8_t* data, ByteSize size);
template SysErrCause TRegDataBase<TRegData8>::ReadBody (PhysAddr addr,
        uint8_t* data, ByteSize size) const;
template SysErrCause TRegDataBase<TRegData16>::ReadBody (PhysAddr addr,
        uint8_t* data, ByteSize size) const;
template SysErrCause TRegDataBase<TRegData32>::ReadBody (PhysAddr addr,
        uint8_t* data, ByteSize size) const;
template SysErrCause TRegDataBase<TRegData8>::Read (SideBand sideband,
        PhysAddr addr, uint8_t* data, ByteSize size) const;
template SysErrCause TRegDataBase<TRegData16>::Read (SideBand sideband,
        PhysAddr addr, uint8_t* data, ByteSize size) const;
template SysErrCause TRegDataBase<TRegData32>::Read (SideBand sideband,
        PhysAddr addr, uint8_t* data, ByteSize size) const;
