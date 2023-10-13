/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <vector>
#include "./config.h"
#include "./bar.h"
#include "./forest_internals.h"
#include "./fsbus.h"
#if _UNIT_TEST
#include <stdexcept>
#include "gmock/gmock.h"
#include "mock_fsbus.h"
#endif



template <class TR>
SysErrCause TRegBarBase<TR>::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_NONE;
    }
    if (this->IsRegAccessSizeValid (size) == false) {
        this->PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    return this->WriteBody (addr, data, size);
}


bool TRegCNTBarBase::IsRegWriteEnable (SideBand sideband) const
{
    return true;
}


TRegBRINIT::TRegBRINIT (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel)
           :TRegCNTBarBase (reg_size, min_access_size, reg_addr, TRegMask(BRINIT_MASK)),m_channel (channel)
{
    m_treg = &m_data;
}

SysErrCause TRegBRINIT::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    }
  uint8_t wdata = *data;
  if ((wdata & 0x1U) != 0){
       for (uint32_t i = 0; i < 8; i++){
           this->Set_BRCHK(m_channel, i, 0x0);    //clear BRnCHK
           this->Set_BRSYN(m_channel, i, 0x0);    //clear BRnSYN
        }
    }
    return SYSERR_NONE;
}
SysErrCause TRegBRINIT::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_NONE;
    }
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    }

    return WriteBody(addr,data,size);
}

TRegBREN::TRegBREN (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel)
           :TRegCNTBarBase (reg_size, min_access_size, reg_addr, TRegMask(BREN_MASK)),m_channel (channel)
{
    m_treg = &m_data;
}

SysErrCause TRegBREN::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    }
    uint8_t wdata = *data;
    SetData(wdata);
    if ((wdata & 0xFF) != TRegBar::BAR_FUNC_DISABLE){
        // This variable is used to keep the Barrier synchronization status.
        bool bar_sync = true;
        for (uint32_t bar_id = 0; bar_id < TRegBar::N_G4MH_BAR_PE; bar_id++){
            if (((wdata >> bar_id) & 0x1U) == TRegBar::BAR_CHK_SET){
                if ((Get_BRCHK(m_channel,bar_id)) == TRegBar::BAR_CHK_CLR) {
                    // If any PEs didn't reach to Barrier, the synchronization is cancelled.
                    bar_sync = false;
                    break;
                }
            }
        }
        if (bar_sync){
            // In case Barrier synchronization at channel n,
            // all BRnCHKm bits will be cleared,
            // and all PEs reached to Barrier will be set BRnSYNCm.
            for (uint32_t bar_id = 0; bar_id < TRegBar::N_G4MH_BAR_PE; bar_id++){
                Set_BRCHK(m_channel, bar_id, TRegBar::BAR_CHK_CLR);
                if (((wdata >> bar_id) & 0x1U) == TRegBar::BAR_CHK_SET){
                    Set_BRSYN(m_channel, bar_id, TRegBar::BAR_CHK_SET);
                }
            }
        }
    } else {
        // As u2a-eva1_spec_rev0.9, in the note for BRnCHKm,
        // If all bits of the BRnEN register are 0, the hardware handles the channel n as synchronization established.
        // Therefore all the BRCHK bit in this channel will be cleared.
        for (uint32_t bar_id = 0; bar_id < TRegBar::N_G4MH_BAR_PE; bar_id++){
            Set_BRCHK(m_channel, bar_id, TRegBar::BAR_CHK_CLR);
        }
    }
    return SYSERR_NONE;
}

SysErrCause TRegBREN::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_NONE;
    }
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
        return WriteBody(addr,data,size);
    }
}

#if _UNIT_TEST
TRegBRCHK::TRegBRCHK (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel, uint32_t bar_id, MockFsBus* fsbus)
           :TRegCNTBarBase (reg_size, min_access_size, reg_addr, TRegMask(BRCHK_MASK)),
          m_channel (channel), m_bar_id(bar_id), m_fsbus(fsbus)
#else
TRegBRCHK::TRegBRCHK (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel, uint32_t bar_id, FsBus* fsbus)
           :TRegCNTBarBase (reg_size, min_access_size, reg_addr, TRegMask(BRCHK_MASK)),
          m_channel (channel), m_bar_id(bar_id), m_fsbus(fsbus)
#endif
{
    m_treg = &m_data;
}

SysErrCause TRegBRCHK::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    }

    RegData bren = this->Get_BREN(m_channel);
    //If all bits of the BRnEN register are 0, BRCHK bit cannot be set
    if ((bren & 0xFFU) != TRegBar::BAR_FUNC_DISABLE){
        SetData(TRegBar::BAR_CHK_SET);
        // This variable is used to keep the Barrier synchronization status.
        bool bar_sync = true;
        for (uint32_t bar_id = 0; bar_id < TRegBar::N_G4MH_BAR_PE; bar_id++){
            if (((bren >> bar_id) & 0x1U) == TRegBar::BAR_CHK_SET){
                if ((Get_BRCHK(m_channel,bar_id)) == TRegBar::BAR_CHK_CLR) {
                    // If any PEs didn't reach to Barrier, the synchronization is cancelled.
                    bar_sync = false;
                    break;
                }
            }
        }
        if (bar_sync){
            // In case Barrier synchronization at channel n,
            // all BRnCHKm bits will be cleared,
            // and all PEs reached to Barrier will be set BRnSYNCm.
            for (uint32_t bar_id = 0; bar_id < TRegBar::N_G4MH_BAR_PE; bar_id++){
                Set_BRCHK(m_channel, bar_id, TRegBar::BAR_CHK_CLR);
                if (((bren >> bar_id) & 0x1U) == TRegBar::BAR_CHK_SET){
                    Set_BRSYN(m_channel, bar_id, TRegBar::BAR_CHK_SET);
                }
            }
        }
    }
    // In case BRnEN = 0, BRnCHKm can't be set.
    return SYSERR_NONE;
}

SysErrCause TRegBRCHK::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_NONE;
    }
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
        return WriteBody (addr, data, size);
    }
}

#if _UNIT_TEST
TRegBRSYN::TRegBRSYN (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel, uint32_t bar_id, MockFsBus* fsbus)
           :TRegCNTBarBase (reg_size, min_access_size, reg_addr, TRegMask(BRSYN_MASK)),
            m_channel (channel), m_bar_id(bar_id), m_fsbus(fsbus)
#else
TRegBRSYN::TRegBRSYN (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel, uint32_t bar_id, FsBus* fsbus)
           :TRegCNTBarBase (reg_size, min_access_size, reg_addr, TRegMask(BRSYN_MASK)),
            m_channel (channel), m_bar_id(bar_id), m_fsbus(fsbus)
#endif
{
    m_treg = &m_data;
}

SysErrCause TRegBRSYN::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    }

    uint8_t wdata = *data;
    SetData(wdata);

    return SYSERR_NONE;
}

SysErrCause TRegBRSYN::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_NONE;
    }
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    }
    return WriteBody(addr,data,size);
}
