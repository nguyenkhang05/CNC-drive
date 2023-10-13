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
#include <vector>
#include "./config.h"
#include "./bar.h"
#include "./forest_internals.h"
#include "./forest_utility.h"
#if _UNIT_TEST
#include <stdexcept>
#include "gmock/gmock.h"
#include "mock_fsbus.h"
#else
#include "./fsbus.h"
#endif

void TRegBar::InitTRegMap (void)
{
    PhysAddr brinit_addr = BRINIT_OFFSET;
    for (uint32_t i = 0; i < N_G4MH_CH; i++) {
        TRegBRINIT* brinit = new TRegBRINIT (SIZE_8BIT, SIZE_8BIT, brinit_addr + i*0x10, i);
        brinit->Set_BRCHK = std::bind(&CBar::Set_BRCHK, m_bar, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        brinit->Set_BRSYN = std::bind(&CBar::Set_BRSYN, m_bar, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        m_trf.insert (TRegMap::value_type (brinit->GetTRegAddr(), brinit));
    }

    PhysAddr bren_addr = BREN_OFFSET;
    for (uint32_t i = 0; i < N_G4MH_CH; i++) {
        TRegBREN* bren = new TRegBREN (SIZE_8BIT, SIZE_8BIT, bren_addr + i*0x10, i);
        bren->Get_BRCHK = std::bind(&CBar::Get_BRCHK, m_bar, std::placeholders::_1, std::placeholders::_2);
        bren->Set_BRCHK = std::bind(&CBar::Set_BRCHK, m_bar, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        bren->Set_BRSYN = std::bind(&CBar::Set_BRSYN, m_bar, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        m_trf.insert (TRegMap::value_type (bren->GetTRegAddr(), bren));
    }

    for (uint32_t j = 0; j < 8; j++){
        PhysAddr brchk_addr = BRCHK_OFFSET;
        for (uint32_t i = 0; i < N_G4MH_CH; i++) {
            TRegBRCHK* brchk = new TRegBRCHK (SIZE_8BIT, SIZE_8BIT, brchk_addr + i*0x10 + j*0x100, i,j,m_fsbus);
            brchk->Get_BREN = std::bind(&CBar::Get_BREN, m_bar, std::placeholders::_1);
            brchk->Get_BRCHK = std::bind(&CBar::Get_BRCHK, m_bar, std::placeholders::_1, std::placeholders::_2);
            brchk->Set_BRCHK = std::bind(&CBar::Set_BRCHK, m_bar, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            brchk->Set_BRSYN = std::bind(&CBar::Set_BRSYN, m_bar, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
            m_trf.insert (TRegMap::value_type (brchk->GetTRegAddr(), brchk));
        }

        PhysAddr brsyn_addr = BRSYN_OFFSET;
        for (uint32_t i = 0; i < N_G4MH_CH; i++) {
            TRegBRSYN* brsyn = new TRegBRSYN (SIZE_8BIT, SIZE_8BIT, brsyn_addr + i*0x10 + j*0x100, i,j,m_fsbus);
            m_trf.insert (TRegMap::value_type (brsyn->GetTRegAddr(), brsyn));
        }
    }
}


PhysAddr TRegBar::TrBRINITAddr (uint32_t channel) const
{
    return BRINIT_OFFSET + channel*0x10;
}

TRegBRINIT* CBar::GetBRINITPtr (uint32_t channel) const
{
    TRegBar* trfs = static_cast<TRegBar*> (m_trf);
    PhysAddr brinit_addr = trfs->TrBRINITAddr (channel);
    return trfs->TrBRINIT (brinit_addr);
}

PhysAddr TRegBar::TrBRENAddr (uint32_t channel) const
{
    return BREN_OFFSET + channel*0x10;
}

TRegBREN* CBar::GetBRENPtr (uint32_t channel) const
{
    TRegBar* trfs = static_cast<TRegBar*> (m_trf);
    PhysAddr bren_addr = trfs->TrBRENAddr (channel);
    return trfs->TrBREN (bren_addr);
}

PhysAddr TRegBar::TrBRCHKAddr (uint32_t channel, uint32_t bar_id) const
{
    return BRCHK_OFFSET + channel*0x10 + bar_id*0x100;
}


TRegBRCHK* CBar::GetBRCHKPtr (uint32_t channel, uint32_t bar_id) const
{
    TRegBar* trfs = static_cast<TRegBar*> (m_trf);
    PhysAddr brchk_addr = trfs->TrBRCHKAddr (channel, bar_id);
    return trfs->TrBRCHK (brchk_addr);
}

PhysAddr TRegBar::TrBRSYNAddr (uint32_t channel, uint32_t bar_id) const
{
    return BRSYN_OFFSET + channel*0x10 + bar_id*0x100;
}


TRegBRSYN* CBar::GetBRSYNPtr (uint32_t channel, uint32_t bar_id) const
{
    TRegBar* trfs = static_cast<TRegBar*> (m_trf);
    PhysAddr brsyn_addr = trfs->TrBRSYNAddr (channel, bar_id);
    return trfs->TrBRSYN (brsyn_addr);
}

bool CBar::IsValidPeId (uint32_t peid) const
{
    if (peid < m_pes->size ()) {
        return (*m_pes)[peid] != NULL;
    } else {
        return false;
    }
}

PhysAddr CBar::ConvertSelfAddr(uint32_t peid, PhysAddr addr) {
   if ((BAR_S_OFFSET_START <= addr) && (addr < BAR_S_OFFSET_END)) {
     return (addr + (BAR_S_OFFSET_END - BAR_S_OFFSET_START) + BAR_PE_OFFSET*peid);
   }
   else {
     return addr;
   }
}

/*!
 * @brief Writes to the register.
 * @param sideband Sideband.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @return System error information.
 */
SysErrCause CBar::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
#if _UNIT_TEST
    uint32_t peid = 0;
#else
    uint32_t peid = p_forest_util->GetPeidFromBmid (sideband.GetBMID ());
#endif
    PhysAddr maddr = addr & BAR_REG_ADDR_MASK;
    //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
    //Masters except PEx can access to actual area, read/write normally as spec description
    if ((peid == CF_INVALID_ID) && (BAR_S_OFFSET_START <= maddr) && (maddr < BAR_S_OFFSET_END)) {
      return SYSERR_NONE;
    } 
    PhysAddr paddr = ConvertSelfAddr(peid, maddr);
    m_trf->GetTrPtr(paddr)->Write (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Reads from the register.
 * @param sideband Sideband.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @return System error information.
 */
SysErrCause CBar::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
#if _UNIT_TEST
    uint32_t peid = 0;
#else
    uint32_t peid = p_forest_util->GetPeidFromBmid (sideband.GetBMID ());
#endif
    PhysAddr maddr = addr & BAR_REG_ADDR_MASK;
    //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
    //Masters except PEx can access to actual area, read/write normally as spec description
    if ((peid == CF_INVALID_ID) && (BAR_S_OFFSET_START <= maddr) && (maddr < BAR_S_OFFSET_END)) {
      memset(data, 0, size);
      return SYSERR_NONE;
    } 
    PhysAddr paddr = ConvertSelfAddr(peid, maddr);
    m_trf->GetTrPtr(paddr)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Writes to the register with debugging.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @return System error information.
 */
SysErrCause CBar::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
#if _UNIT_TEST
  uint32_t peid = 0;
#else
  uint32_t peid = p_forest_util->GetPeidOfDbgAPI ();
#endif
    PhysAddr maddr = addr & BAR_REG_ADDR_MASK;
    //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
    //Masters except PEx can access to actual area, read/write normally as spec description
    if ((peid == CF_INVALID_PEID) && (BAR_S_OFFSET_START <= maddr) && (maddr < BAR_S_OFFSET_END)) {
      return SYSERR_NONE;
    } 
    PhysAddr paddr = ConvertSelfAddr(peid, maddr);
    m_trf->GetTrPtr(paddr)->WriteBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Reads from the register with debugging.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @return System error information.
 */
SysErrCause CBar::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
#if _UNIT_TEST
  uint32_t peid = 0;
#else
  uint32_t peid = p_forest_util->GetPeidOfDbgAPI ();
#endif
    PhysAddr maddr = addr & BAR_REG_ADDR_MASK;
    //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
    //Masters except PEx can access to actual area, read/write normally as spec description
    if ((peid == CF_INVALID_PEID) && (BAR_S_OFFSET_START <= maddr) && (maddr < BAR_S_OFFSET_END)) {
      memset(data, 0, size);
      return SYSERR_NONE;
    } 
    PhysAddr paddr = ConvertSelfAddr(peid, maddr);
    m_trf->GetTrPtr(paddr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


void CBar::HardReset (void)
{
    m_trf->ResetTRegs();
}


void CBar::Set_BRCHK(uint32_t channel, uint32_t bar_id, RegData data)
{
    TRegBRCHK* brchk = GetBRCHKPtr (channel, bar_id);
    brchk->SetData(data);
}

RegData CBar::Get_BRCHK(uint32_t channel, uint32_t bar_id)
{
    TRegBRCHK* brchk = GetBRCHKPtr (channel, bar_id);
    return (brchk->GetData()& 0xFFU);
}


void CBar::Set_BRSYN(uint32_t channel, uint32_t bar_id, RegData data)
{
    TRegBRSYN* brsyn = GetBRSYNPtr (channel,bar_id);
    brsyn->SetData(data);
}

RegData CBar::Get_BRSYN(uint32_t channel, uint32_t bar_id)
{
    TRegBRSYN* brsyn = GetBRSYNPtr (channel, bar_id);
    return (brsyn->GetData() & 0xFFU);
}

void CBar::Set_BRINIT(uint32_t channel, RegData data)
{
    TRegBRINIT* brinit = GetBRINITPtr (channel);
    brinit->SetData(data);
}

RegData CBar::Get_BRINIT(uint32_t channel)
{
    TRegBRINIT* brinit = GetBRINITPtr (channel);
    return (brinit->GetData() & 0xFFU);
}

void CBar::Set_BREN(uint32_t channel, RegData data)
{
    TRegBREN* bren = GetBRENPtr (channel);
    bren->SetData(data);
}

RegData CBar::Get_BREN(uint32_t channel)
{
    TRegBREN* bren = GetBRENPtr (channel);
    return (bren->GetData() & 0xFFU);
}

#if _UNIT_TEST
CBar::CBar (PeVec* pes, MockFsBus* fsbus, BarImplType type)
    : m_impl_type (type), m_pes(pes), m_fsbus(fsbus)
#else
CBar::CBar (PeVec* pes, FsBus* fsbus, BarImplType type)
    : m_impl_type (type), m_pes(pes), m_fsbus(fsbus)
#endif
{
    m_channel = CBar::INIT_CHANNEL;
    m_trf = new TRegBar (this, pes, fsbus);

}
