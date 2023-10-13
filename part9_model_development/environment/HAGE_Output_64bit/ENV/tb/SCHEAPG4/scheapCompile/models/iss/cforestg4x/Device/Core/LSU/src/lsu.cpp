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
#include "./config.h"
#include "./forest_common.h"
#include "./mpu.h"
#include "./lsu.h"
#include "./trace_info.h"
#include "./fsbus.h"
#include "./icache.h"
#include "./sregfile.h"
#ifdef ENABLE_DBG_BREAK
#include "./dbg_break.h"
#endif // #ifdef ENABLE_DBG_BREAK
#include "./fs_controler.h"
#include "./llsc.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./proc_element.h"
#include "./inst_queue.h"
#include "./misr.h"

//! \sa Lsu::MemAccess
void Lsu::MemWrite (PhysAddr vaddr, MemData data, ByteSize size, bool isBitOp)
{
    MemDataU32 wdata (data);
#ifdef ENABLE_MISR
    MemDataU32 wdata_misr (data);  // backup for MISR calculation
#endif // ENABLE_MISR
    MemAccess<MACC_NORMAL, DATA_WRITE> (vaddr, wdata.d8, size);
    m_trace->RecordMemAccess<DATA_WRITE> (vaddr, 0, wdata.d32, size, m_mem_route);

    // Checks address which LLSC is used, for LRAM or for others.
    if (m_llsc->IsThereAnyLink () == true) {
        PhysAddr llsc_paddr;
        if (IsPeLocalArea (vaddr)) {
            llsc_paddr = GetBusAddress (vaddr);
        } else {
            llsc_paddr = vaddr;
        }
        PhysAddr base_addr = llsc_paddr & Llsc::LINK_ADDR_MASK;
        m_llsc->DeleteLink (base_addr, m_trace);

        // Delete next align
        PhysAddr last_addr = (llsc_paddr + (size - 1)) & Llsc::LINK_ADDR_MASK;
        if (base_addr != last_addr) {
            m_llsc->DeleteLink (last_addr, m_trace);
        }
    }

#ifdef ENABLE_MISR
    if (isBitOp == false && (!m_trace->IsCancel() || (m_peg_error == true))) {
        // Physical Address is used regardless of any exception
        PhysAddr paddr = AddressTransfer (vaddr);
        m_misr->CalcSignMonitor (paddr, wdata_misr.d8, size);
    }
#endif // ENABLE_MISR
}


//! \sa Lsu::MemAccess
MemData Lsu::MemRead (PhysAddr vaddr, ByteSize size, SpcldType is_spcld)
{
    // Set speculative type
    if (IsSpcldFixed0Area (vaddr) == false) {
        m_sideband.SetSPCLD (is_spcld);
    }

    MemDataU32 rdata (0x0U);
    MemAccess<MACC_NORMAL, DATA_READ> (vaddr, rdata.d8, size);
    m_trace->RecordMemAccess<DATA_READ> (vaddr, 0, rdata.d32, size, m_mem_route);

    // Reset speculative type for next access
    m_sideband.SetSPCLD (SPCLD_OFF);

    return rdata.d32;
}


//! \sa Lsu::MemAccess
void Lsu::MemWriteBitOp (PhysAddr vaddr, MemData data, ByteSize size, SideBand::BitOpType bitop)
{
    // By BitOp instruction, sets Bit Position and BitOp type on the sideband.
    m_sideband.SetBitPos (vaddr); // No need to mask here, because it masked at sideband.
    m_sideband.SetBitOp (bitop);

    MemWrite (vaddr, data, size, true);

    // Restores a sideband signal for normal Read/Write.
    m_sideband.SetBitPos (0);
    m_sideband.SetBitOp (SideBand::BITOP_NONE);
}


//! \sa Lsu::MemAccess
MemData Lsu::MemReadBitOp (PhysAddr vaddr, ByteSize size, SideBand::BitOpType bitop)
{
    // By BitOp instruction, sets Bit Position and BitOp type on the sideband.
    m_sideband.SetBitPos (vaddr);  // No need to mask here, because it masked at sideband.
    m_sideband.SetBitOp (bitop);

    MemData rdata = MemRead (vaddr, size, SPCLD_OFF);

    // Restores a sideband signal for normal Read/Write.
    m_sideband.SetBitPos (0);
    m_sideband.SetBitOp (SideBand::BITOP_NONE);

    return rdata;
}


//! \sa Lsu::MemAccess
void Lsu::MemWrite64 (PhysAddr vaddr, MemData64 data)
{
    MemDataU64 wdata (data);
#ifdef ENABLE_MISR
    MemDataU64 wdata_misr (data);  // backup for MISR calculation
#endif // ENABLE_MISR
    MemAccess<MACC_NORMAL, DATA_WRITE> (vaddr, wdata.d8, SIZE_64BIT);
    m_trace->RecordMemAccess<DATA_WRITE> (vaddr, 0, wdata.d64, SIZE_64BIT, m_mem_route);

    // Checks address which LLSC is used, for LRAM or for others.
    if (m_llsc->IsThereAnyLink () == true) {
        PhysAddr llsc_paddr;
        if (IsPeLocalArea (vaddr)) {
            llsc_paddr = GetBusAddress (vaddr);
        } else {
            llsc_paddr = vaddr;
        }
        PhysAddr base_addr = llsc_paddr & Llsc::LINK_ADDR_MASK;
        m_llsc->DeleteLink (base_addr, m_trace);

        // Check latter half for accrossing access between 2 links
        PhysAddr last_addr = (llsc_paddr + (SIZE_64BIT - 1)) & Llsc::LINK_ADDR_MASK;
        if (base_addr != last_addr) {
            m_llsc->DeleteLink (last_addr, m_trace);
        }
    }

#ifdef ENABLE_MISR
    if (!m_trace->IsCancel() || (m_peg_error == true)) {
        // Physical Address is used regardless of any exception
        PhysAddr paddr = AddressTransfer (vaddr);
        m_misr->CalcSignMonitor (paddr, wdata_misr.d8, SIZE_64BIT);
    }
#endif // ENABLE_MISR
}


//! \sa Lsu::MemAccess
void Lsu::MemWrite128 (PhysAddr vaddr, MemData64 data_u, MemData64 data_l)
{
    MemDataU128 wdata (data_u, data_l);
#ifdef ENABLE_MISR
    MemDataU128 wdata_misr (data_u, data_l);  // backup for MISR calculation
#endif // ENABLE_MISR
    MemAccess<MACC_NORMAL, DATA_WRITE> (vaddr, wdata.d8, SIZE_128BIT);
    m_trace->RecordMemAccess<DATA_WRITE> (vaddr, wdata.d64[1], wdata.d64[0], SIZE_128BIT, m_mem_route);

    // Checks address which LLSC is used, for LRAM or for others.
    if (m_llsc->IsThereAnyLink () == true) {
        PhysAddr llsc_paddr;
        if (IsPeLocalArea (vaddr)) {
            llsc_paddr = GetBusAddress (vaddr);
        } else {
            llsc_paddr = vaddr;
        }
        PhysAddr base_addr = llsc_paddr & Llsc::LINK_ADDR_MASK;
        m_llsc->DeleteLink (base_addr, m_trace);

        // Check latter half for accrossing access between 2 links
        PhysAddr last_addr = (llsc_paddr + (SIZE_128BIT - 1)) & Llsc::LINK_ADDR_MASK;
        if (base_addr != last_addr) {
            m_llsc->DeleteLink (last_addr, m_trace);
        }
    }

#ifdef ENABLE_MISR
    if (!m_trace->IsCancel() || (m_peg_error == true)) {
        // Physical Address is used regardless of any exception
        PhysAddr paddr = AddressTransfer (vaddr);
        m_misr->CalcSignMonitor (paddr, wdata_misr.d8, SIZE_64BIT);
    }
#endif // ENABLE_MISR
}


//! \sa Lsu::MemAccess
std::pair<MemData64, MemData64> Lsu::MemRead128 (PhysAddr vaddr)
{
    // Set speculative type
    if (IsSpcldFixed0Area (vaddr) == false) {
        m_sideband.SetSPCLD (SPCLD_ON); // enable in LDV
    }

    MemDataU128 rdata (0x0UL, 0x0UL);
    MemAccess<MACC_NORMAL, DATA_READ> (vaddr, rdata.d8, SIZE_128BIT);
    m_trace->RecordMemAccess<DATA_READ> (vaddr, rdata.d64[1], rdata.d64[0], SIZE_128BIT, m_mem_route);

    // Reset speculative type for next access
    m_sideband.SetSPCLD (SPCLD_OFF);

    return std::make_pair(rdata.d64[1], rdata.d64[0]);
}


//! \sa Lsu::MemAccess
MemData64 Lsu::MemRead64 (PhysAddr vaddr)
{
    // Set speculative type
    if (IsSpcldFixed0Area (vaddr) == false) {
        m_sideband.SetSPCLD (SPCLD_ON); // enable in LDV/LD.DW
    }

    MemDataU64 rdata (0x0UL);
    MemAccess<MACC_NORMAL, DATA_READ> (vaddr, rdata.d8, SIZE_64BIT);
    m_trace->RecordMemAccess<DATA_READ> (vaddr, 0, rdata.d64, SIZE_64BIT, m_mem_route);

    // Reset speculative type for next access
    m_sideband.SetSPCLD (SPCLD_OFF);

    return rdata.d64;
}


#if defined (ENABLE_TRACE_MDP_SUPPRESS)
void Lsu::MemReadBitOpMdp (PhysAddr vaddr, ByteSize size, SideBand::BitOpType bitop)
{
    MemReadMdp (vaddr, size, false);
}


void Lsu::MemReadMdp (PhysAddr vaddr, ByteSize size, SpcldType is_spcld)
{
    MemDataU32 rdata (0x0U);
    m_trace->RecordMemAccessMdp<DATA_READ> (vaddr, 0, rdata.d32, size, m_mem_route, false);
}


void Lsu::MemWriteBitOpMdp (PhysAddr vaddr, MemData data, ByteSize size, SideBand::BitOpType bitop)
{
    MemWriteMdp (vaddr, data, size, true);
}


void Lsu::MemWriteMdp (PhysAddr vaddr, MemData data, ByteSize size, bool isBitOp)
{
    MemDataU32 wdata (data);
    m_trace->RecordMemAccessMdp<DATA_WRITE> (vaddr, 0, wdata.d32, size, NONE_CACHE_ROUTE, false);
}


void Lsu::MemWrite64Mdp (PhysAddr vaddr, MemData64 data)
{
    MemDataU64 wdata (data);
    m_trace->RecordMemAccessMdp<DATA_WRITE> (vaddr, 0, wdata.d64, SIZE_64BIT, NONE_CACHE_ROUTE, false);
}


void Lsu::MemWrite128Mdp (PhysAddr vaddr, MemData64 data_u, MemData64 data_l)
{
    MemDataU128 wdata (data_u, data_l);
    m_trace->RecordMemAccessMdp<DATA_WRITE> (vaddr, wdata.d64[1], wdata.d64[0], SIZE_128BIT, NONE_CACHE_ROUTE, false);
}
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)


//! \sa Lsu::MemAccess
void Lsu::MemWriteDebug (PhysAddr vaddr, MemData data, ByteSize size)
{
    MemDataU32 wdata (data);
    MemAccess<MACC_DEBUG, DATA_WRITE> (vaddr, wdata.d8, size);
}


//! \sa Lsu::MemAccess
MemData Lsu::MemReadDebug (PhysAddr vaddr, ByteSize size)
{
    MemDataU32 rdata (0x0U);
    MemAccess<MACC_DEBUG, DATA_READ> (vaddr, rdata.d8, size);
    return rdata.d32;
}


//! \sa Lsu::MemAccess
MemData Lsu::MemFetchDebug (PhysAddr vaddr, ByteSize size)
{
    MemDataU32 rdata (0x0U);
    MemAccess<MACC_DEBUG, INST_READ> (vaddr, rdata.d8, size);
    return rdata.d32;
}


/*!
 * @brief  Fetches a instruction from Instruction Cache or Bus.
 * @param  line_vaddr The address to fetch.
 * @param  line_data  The pointer to Instruction Que(1 line).
 * @param  line_size  The data size(in Byte) of Instruction Que(1 line).
 * @return Physical address after BAT conversion.
 *
 * \sa Lsu::BusAccessRapArround
 */
PhysAddr Lsu::MemInstFetch (PhysAddr line_vaddr, MemData8* line_data,
                            uint32_t line_size)
{
    PhysAddr line_paddr = AddressTransfer (line_vaddr);

    ++ m_fetch_count; // PMCTRL.CND=30h
    m_inst_queue->SetIcHit (false);

    MemAccessRoute fetch_route;
#ifdef ENABLE_DBG_BREAK
    // IC is disabled when DIR0.DM=1
    if (m_ic->IsCachingArea (line_paddr) && m_pe->IsIcEnabled () && (m_srf->SrDIR0()->GetDM() == 0)) {
#else // #ifdef ENABLE_DBG_BREAK
    // Ignore DIR0.DM when DBG is disabled for simulation speed.
    if (m_ic->IsCachingArea (line_paddr) && m_pe->IsIcEnabled ()) {
#endif // #ifdef ENABLE_DBG_BREAK
        // Set speculative type : For fetch from FlashROM, always enabled.
        m_sideband.SetSPCLD (SPCLD_ON);

#if defined(ENABLE_ERROR_AREA)
        SysErrCause sys_err_cause =
#endif // defined(ENABLE_ERROR_AREA)
        m_ic->FetchFromIC (line_vaddr, line_paddr, line_data, line_size, m_sideband, m_trace);

        // Reset speculative type
        m_sideband.SetSPCLD (SPCLD_OFF);

#if defined(ENABLE_ERROR_AREA)
        if (sys_err_cause != SYSERR_NONE) {
            // ERROR ocurrs :
            //! @par ERROR case : clears the data for safety.
            memset (line_data, 0x0, sizeof(uint8_t) * line_size);

            if (sys_err_cause == SYSERR_NOP) {
                // If SYSERR_NOP, behavior is same as SYSERR_NONE except read value is 0.
                m_trace->RecordMemFetch (line_vaddr, m_ic->m_fill_route);
                return line_paddr;
            } else {
                if ((sys_err_cause & SYSERR_MASK) == SYSERR_I_GUEST) {
                    m_inst_queue->SetSyserr (m_syserr_cause_fetch_guest);
                } else {
                    m_inst_queue->SetSyserr (SYSERR_ICFB); // GuardError is 0x11 in G4MH
                }
                m_trace->SetCancel (true);
                m_trace->RecordMemFetch (line_vaddr, m_ic->m_fill_route);
                return INVALID_PADDR;
            }
        }
#endif // defined(ENABLE_ERROR_AREA)
        fetch_route = m_ic->m_fill_route;
        if (unlikely(fetch_route == ICACHE_MISS_ROUTE)) {
            ++ m_flashrom_fetch_count; // PMCTRL.CND=50h
        } else {
            m_inst_queue->SetIcHit (true);
        }
    } else {
        if (m_ic->IsCachingArea (line_paddr)) {
            ++ m_flashrom_fetch_count; // PMCTRL.CND=50h

            // Set speculative type : For fetch from FlashROM, always enabled.
            // Fetch from other than FlashROM area is NOT speculative access.
            m_sideband.SetSPCLD (SPCLD_ON);
        }

        SysErrCause sys_err_cause
            = BusAccessRapArround<MACC_NORMAL, INST_READ> (line_paddr, line_data, line_size);

        // Reset speculative type.
        m_sideband.SetSPCLD (SPCLD_OFF);

        if (unlikely (sys_err_cause != SYSERR_NONE)) {
            //! @par Exception/Error case :
            //! Fills the memory with harmless NOP instruction(0x0000).
            //! In Exception case, it's OK even if the que data is trustless,
            //! because it re-fetched in the handler of the branch.\n
            //! In real operation case, it just generates SYSERRR to Instruction Que.
            memset (line_data, 0x0, sizeof(uint8_t) * line_size);
#ifdef SUPPORT_SC_HEAP
            if ((sys_err_cause & SYSERR_PEG) == SYSERR_PEG) {
                if ((sys_err_cause & SYSERR_REC_MASTER) == SYSERR_REC_MASTER) {
                    m_trace->SetGuardError (GD_PEG_ERR_M);
                } else {
                    m_trace->SetGuardError (GD_PEG_ERR_S);
                }
            } else if ((sys_err_cause & SYSERR_CRG) == SYSERR_CRG) {
                m_trace->SetGuardError (GD_CRG_ERR);
            }
#endif // SUPPORT_SC_HEAP
            if ((sys_err_cause & SYSERR_MASK) == SYSERR_I_GUEST) {
                m_inst_queue->SetSyserr (m_syserr_cause_fetch_guest);
            } else {
                m_inst_queue->SetSyserr (SYSERR_ICFB); // GuardError is 0x11 in G4MH
            }
            m_trace->SetCancel (true);
            m_trace->RecordMemFetch (line_vaddr, NONE_CACHE_ROUTE);
            return INVALID_PADDR;
        }

        fetch_route = NONE_CACHE_ROUTE;
    }

    // @@ How the record of fetch-access goes, when Exception occurs?
    // @@ Please check the device-behavior when you implements CS mode.
    m_trace->RecordMemFetch (line_vaddr, fetch_route);

    return line_paddr;
}


void Lsu::HandleErrorResponseForOperand (SysErrCause sys_err_cause)
{
    // Although error response is returned from target/guard,
    // Syserr doesn't occur by data access on G4MH1.x.
    if ((sys_err_cause & SYSERR_PEG) == SYSERR_PEG) {
        m_peg_error = true;
#ifdef SUPPORT_SC_HEAP
        if ((sys_err_cause & SYSERR_REC_MASTER) == SYSERR_REC_MASTER) {
            m_trace->SetGuardError (GD_PEG_ERR_M);
        } else {
            m_trace->SetGuardError (GD_PEG_ERR_S);
        }
#endif // SUPPORT_SC_HEAP
    }
#ifdef SUPPORT_SC_HEAP
    else if ((sys_err_cause & SYSERR_CRG) == SYSERR_CRG) {
        m_trace->SetGuardError (GD_CRG_ERR);
    }
#endif // SUPPORT_SC_HEAP
    m_trace->SetCancel (true);

    // Set SYSERR event
    if (IsEnabledOperandSyserr()) { // Only for G4MH2.0
        SysErrCause masked_err = sys_err_cause & SYSERR_MASK;
        SysErrCause err_cause;
        if ((masked_err == SYSERR_D_GUEST) && (m_srf->SrDECFG()->GetEGE () == true)) {
            err_cause = SYSERR_D_GUEST;
        } else if ((masked_err == SYSERR_SAFETY) && (m_srf->SrDECFG()->GetEHE () == true)){
            err_cause = SYSERR_SAFETY;
        } else if (m_srf->SrDECFG()->GetESE () == true) { // SYSERR_BSLAVE and others
            err_cause = SYSERR_BSLAVE;
        } else {
            // Operand syserr is disabled by DECFG.
            err_cause = SYSERR_NONE;
        }

        // operand syserr is recorded into DEVDS when the syserr is enabled by DECFG.
        if (err_cause != SYSERR_NONE) {
            bool is_host  = ((m_pe->IsHvEnabled () == false) || (m_pe->GetNcSrfPtr()->SrPSWH ()->GetGM () == 0));
            uint32_t gpid;
            if (is_host) {
                gpid = 0;
            } else {
                gpid = m_srf->SrPSWH ()->GetGPID ();
            }

            // When there is the SYSERR request for the current mode already,
            // the existed request is kept and new request is ignored.
            if (GetSyserrCause (is_host, gpid) == SYSERR_NONE) {
                // There is no existed SYSERR request for the current mode.
                // Request SYSERR for the current mode
                if (is_host) {
                    // Host mode or conventional mode
#if defined(COMP_RUNNER_G4MH)
                    // Due to timing mismatch between RTL and Cforest when operand syserr occurs,
                    // Cforest restrains request syserr. Instead, it turns on a status flag for
                    // CmpRunner check later when syserr event sent from RTL
                    m_host_operand_syserr_status = true;
#else
                    SetSyserrCause (true, 0, err_cause);
#endif
                } else {
#if defined(COMP_RUNNER_G4MH)
                    // Due to timing mismatch between RTL and Cforest when operand syserr occurs,
                    // Cforest restrains request syserr. Instead, it turns on a status flag for
                    // CmpRunner check later when syserr event sent from RTL
                    if (gpid <= 7) {
                        m_guest_operand_syserr_status[gpid] = true ;
                    }
#else
                    SetSyserrCause (false, gpid, err_cause);
#endif
                }
                m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_DEVDS, m_srf->SrDEVDS()->ReadBody());
                m_pe->GetHtPtr()->UpdateOperandSyserrEvent ();
            }
        }
    }
}

//! \sa BusAccess
template <MemAccessMode acc_mode, MemAccessCommand cmd>
PhysAddr Lsu::MemAccess (PhysAddr vaddr, MemData8* data, ByteSize size)
{
    m_peg_error = false;
    PhysAddr paddr = AddressTransfer (vaddr);

    if (m_ic->IsCachingArea (paddr)) {
#ifdef ENABLE_ROM_W_ERROR
        // Write to ROM isn't issued to ROM and it causes SYSERR in G4MH2.0.
        if ((acc_mode != MACC_DEBUG) && (cmd == DATA_WRITE)) {
            if (  (m_pe->GetCoreType () == CORE_G4MH)
               && (m_pe->GetCoreVersion () >= CORE_VER_20)) {
                // Clear data isn't necessary because of DATA_WRITE.
                // memset (data, 0x0, sizeof(uint8_t) * size);

                HandleErrorResponseForOperand (SYSERR_BSLAVE);
                return paddr;
            }
        }
#endif // #ifdef ENABLE_ROM_W_ERROR

        if (acc_mode != MACC_DEBUG) {
            if (likely(size <= 4)) {
                ++ m_flashrom_data_count; // PMCTRL.CND=51h
            } else {
                m_flashrom_data_count += (size >> 2); // PMCTRL.CND=51h
            }
        }
    }

    SysErrCause sys_err_cause = BusAccess<acc_mode, cmd> (paddr, data, size);
    if (acc_mode != MACC_DEBUG) {
        if (unlikely (sys_err_cause != SYSERR_NONE)) {
            if (cmd == DATA_READ) {
                // Clear data for Read data
                memset (data, 0x0, sizeof(uint8_t) * size);
            }

            // Handle Error response
            HandleErrorResponseForOperand (sys_err_cause);
            return paddr;
        }
        m_mem_route = NONE_CACHE_ROUTE;
    } else {
        // data is initialized to 0 on MemReadDebug() already.
    }
    return paddr;
}


/*!
 * Reads or Writes the data in the block which includes specified address to access.
 * @param paddr  Specified address to access.
 * @param data   The pointer to the block data where write-data was stored,
 *               or read-data will be stored.
 * @param bytes  The size of the block data.
 *
 * In the real rap-around operation,
 * it starts from specified address and rounds from the bottom to the top.
 * But in this implementation, it starts from the top position of the block,
 * and accesses it by that size.
 * If you need to start with sure address,(for example, connect to SystemC)
 * please take care of this matter.
 *
 * \sa Lsu::BusAccess
 */
template <MemAccessMode acc_mode, MemAccessCommand cmd>
SysErrCause Lsu::BusAccessRapArround (PhysAddr paddr, MemData8* data, uint32_t bytes) const
{
    // @@ Not rap around. Access from the top of the blokck.
    // @@ Please mark it is different from real device.
    PhysAddr rap_arround_mask = bytes - 1;
    PhysAddr base_addr = paddr & ~rap_arround_mask;

    return BusAccess<acc_mode, cmd> (base_addr, data, bytes);
}


//! \sa Lsu::BusAccessRapArround
template <MemAccessMode acc_mode, MemAccessCommand cmd>
SysErrCause Lsu::BusAccess (PhysAddr paddr, MemData8* data, uint32_t bytes) const
{
    return m_fsbus->BusAccess<acc_mode, cmd> (m_sideband, paddr, data, bytes);
}


bool Lsu::IsHitReadBrk (PhysAddr vaddr, ByteSize size, LsabType is_eiint)
{
#ifdef ENABLE_DBG_BREAK
    ExpCode exp_code = m_dbgbrk->UpdateBreakLsabExcp<DATA_READ> (vaddr, size, m_trace, is_eiint);
    if (exp_code != EXP_NONE && m_dbgbrk->IsDbSyncExcp (exp_code) == true) {
        return true;
    } else {
        return false;
    }
#else // ENABLE_DBG_BREAK
    return false;
#endif // ENABLE_DBG_BREAK
}


bool Lsu::IsHitWriteBrk (PhysAddr vaddr, ByteSize size, LsabType is_eiint)
{
#ifdef ENABLE_DBG_BREAK
    ExpCode exp_code = m_dbgbrk->UpdateBreakLsabExcp<DATA_WRITE> (vaddr, size, m_trace, is_eiint);
    if (exp_code != EXP_NONE && m_dbgbrk->IsDbSyncExcp (exp_code) == true) {
        return true;
    } else {
        return false;
    }
#else // ENABLE_DBG_BREAK
    return false;
#endif // ENABLE_DBG_BREAK
}


bool Lsu::IsHitRmwBrk (PhysAddr vaddr, ByteSize size)
{
#ifdef ENABLE_DBG_BREAK
    ExpCode exp_code = m_dbgbrk->UpdateBreakLsabExcp<DATA_RMW> (vaddr, size, m_trace, false);
    if (exp_code != EXP_NONE && m_dbgbrk->IsDbSyncExcp (exp_code) == true) {
        return true;
    } else {
        return false;
    }
#else // ENABLE_DBG_BREAK
    return false;
#endif // ENABLE_DBG_BREAK
}


void Lsu::ApplyLsabEoReserved (TraceCtrl* trace)
{
#ifdef ENABLE_DBG_BREAK
    m_dbgbrk->ApplyLsabEoReserved (trace);
#endif // #ifdef ENABLE_DBG_BREAK
}


bool Lsu::IsAlignmentError (PhysAddr vaddr)
{
#ifdef ENABLE_DBG_BREAK
    // Detect AE exception (NOT "MAE") on PREPARE/DISPOSE/PUSHSP/POPSP.
    // When DIR0.DM=1, AE isn't detected.
    if ((m_srf->SrDIR0()->GetDM() == 1) || (m_srf->SrDIR0()->GetAEE () == 0) || ((vaddr & 0x3) == 0x0)) {
        // No error
        return false;
    } else {
        m_trace->SetExpCode (EXP_AE);
        CfMsg::TPrint (MSG_INF, m_trace, "<Alignment Error: 0x%08x>\n", vaddr);
        return true;
    }
#else // ENABLE_DBG_BREAK
    return false;
#endif // ENABLE_DBG_BREAK
}


template <MemAccessCommand cmd>
bool Lsu::IsMemException (PhysAddr vaddr, ByteSize size)
{
    if (IsMemAlignError (vaddr, size) == true) {
        m_trace->SetExpCode (EXP_MAE);
        CfMsg::TPrint (MSG_INF, m_trace, "<Memory Alignment Error: 0x%08x>\n", vaddr);
        return true;
    }

    if (m_mpu->IsMpuPermitted<cmd> (vaddr, size) == false) {
        if (m_mdp_supress == false) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error: 0x%08x>\n", vaddr);
            if (m_mpu->GetMpuExpPOS () == Mpu::M_GUESTENT) {
                m_trace->SetExpCode (EXP_MDPGM);
            } else {
                m_trace->SetExpCode (EXP_MDPHM);
            }
            return true;
        } else {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error (Suppressed): 0x%08x>\n", vaddr);
            if (cmd == DATA_WRITE) {
                // Exception doesn't occur and access isn't issued in write access.
                return true;
            }
        }
    }

    return false;
}


template <MemAccessCommand cmd>
bool Lsu::IsMemExceptionEI (PhysAddr vaddr, ByteSize size)
{
    // No MAE in table access and register bank access in EIINT.
    if (m_mpu->IsMpuPermittedEI<cmd> (vaddr, size) == false) {
        if (m_mdp_supress == false) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error: 0x%08x>\n", vaddr);
            if (m_mpu->GetMpuExpPOS () == Mpu::M_GUESTENT) {
                // MDP occurs on guest managed entry.
                // PE goes to host mode temporally on EIINT, and PE keeps guest mode on GMEIINT
                if ((m_pe->IsHvEnabled () == false) || (m_pe->IsHvEnabled () && m_pe->IsHostMode ())) {
                    // MDP(Guest entry) on EIINT in host mode
                    m_trace->SetExpCode (EXP_MDPGM_EIINT);
                } else {
                    // MDP(Guest entry) on GMEIINT in guest mode
                    m_trace->SetExpCode (EXP_MDPGM_GMEIINT);
                }
            } else {
                // MDP occurs on host managed entry or conventional mode.
                // PE goes to host mode temporally on EIINT, and PE keeps guest mode on GMEIINT
                if ((m_pe->IsHvEnabled () == false) || (m_pe->IsHvEnabled () && m_pe->IsHostMode ())) {
                    // MDP(Host entry) on EIINT in host mode or conventional mode
                    m_trace->SetExpCode (EXP_MDPHM_EIINT);
                } else {
                    // MDP(Host entry) on GMEIINT in guest mode
                    m_trace->SetExpCode (EXP_MDPHM_GMEIINT);
                }
            }
            return true;
        } else {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error (Suppressed): 0x%08x>\n", vaddr);
            if (cmd == DATA_WRITE) {
                // Exception doesn't occur and access isn't issued in write access.
                return true;
            }
        }
    }

    return false;
}


bool Lsu::IsMemExceptionRMW (PhysAddr vaddr, ByteSize size)
{
    if (IsMemAlignError (vaddr, size) == true) {
        m_trace->SetExpCode (EXP_MAE);
        CfMsg::TPrint (MSG_INF, m_trace, "<Memory Alignment Error: 0x%08x>\n", vaddr);
        return true;
    }

    if (m_mpu->IsMpuPermittedRMW (vaddr, size) == false) {
        if (m_mdp_supress == false) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error: 0x%08x>\n", vaddr);
            if (m_mpu->GetMpuExpPOS () == Mpu::M_GUESTENT) {
                m_trace->SetExpCode (EXP_MDPGM);
            } else {
                m_trace->SetExpCode (EXP_MDPHM);
            }
            return true;
        } else {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error (Suppressed): 0x%08x>\n", vaddr);
            // Exception doesn't occur and access isn't issued in RMW access.
            return true;
        }
    }

    return false;
}

bool Lsu::IsMemExceptionTST (PhysAddr vaddr)
{
    // No need to check MAE becasue TST is 1Byte access.

    if (m_mpu->IsMpuPermitted<DATA_READ> (vaddr, SIZE_1BYTE) == false) {
        if (m_mdp_supress == false) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error: 0x%08x>\n", vaddr);
            if (m_mpu->GetMpuExpPOS () == Mpu::M_GUESTENT) {
                m_trace->SetExpCode (EXP_MDPGM);
            } else {
                m_trace->SetExpCode (EXP_MDPHM);
            }
            return true;
        } else {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error (Suppressed): 0x%08x>\n", vaddr);
            // Exception doesn't occur and access isn't issued in TST (read) access.
            return true;
        }
    }

    return false;
}


bool Lsu::IsMemAlignError (PhysAddr vaddr, ByteSize size) const
{
    if (likely ((vaddr & (size - 1)) == 0x0)) {
        // No alignment error.
        return false;

    } else {
        if (size == SIZE_64BIT && (vaddr & (SIZE_32BIT-1)) == 0x0) {
            // If 64 bit access, then re-check 32 bit align.
            return false;
        }

        // Aligment error.
        return true;
    }
}


template <ByteSize size>
bool Lsu::IsMemExceptionAtomic (PhysAddr vaddr)
{
    if (IsMemAlignErrorAtomic<size> (vaddr) == true) {
        m_trace->SetExpCode (EXP_MAE);
        CfMsg::TPrint (MSG_INF, m_trace, "<Memory Alignment Error: 0x%08x>\n", vaddr);
        return true;
    }

    return false;
}


template <ByteSize size>
bool Lsu::IsMemAlignErrorAtomic (PhysAddr vaddr) const
{
    // Check 64bit align in case of ldl.dw (diffrent from ld.dw.)
    if (likely ((vaddr & (size - 1)) == 0x0)) {
        // No alignment error.
        return false;
    } else {
        // Alignment error
        // In case of caxi/ldl.w/stc.w, the miss-alignment isn't suppressed.
        return true;
    }
}


template <MemAccessCommand cmd>
bool Lsu::IsMemExceptionFXU (PhysAddr vaddr, ByteSize size)
{
    if (IsMemAlignErrorFXU (vaddr, size) == true) {
        m_trace->SetExpCode (EXP_MAE);
        CfMsg::TPrint (MSG_INF, m_trace, "<Memory Alignment Error: 0x%08x>\n", vaddr);
        return true;
    }

    if (m_mpu->IsMpuPermitted<cmd> (vaddr, size) == false) {
        if (m_mdp_supress == false) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error: 0x%08x>\n", vaddr);
            if (m_mpu->GetMpuExpPOS () == Mpu::M_GUESTENT) {
                m_trace->SetExpCode (EXP_MDPGM);
            } else {
                m_trace->SetExpCode (EXP_MDPHM);
            }
            return true;
        } else {
            CfMsg::TPrint (MSG_INF, m_trace, "<Memory Protection Error (Suppressed): 0x%08x>\n", vaddr);
            if (cmd == DATA_WRITE) {
                // Exception doesn't occur and access isn't issued in write access.
                return true;
            }
        }
    }

    return false;
}


bool Lsu::IsMemAlignErrorFXU (PhysAddr vaddr, ByteSize size) const
{
    if (likely ((vaddr & (size - 1)) == 0x0)) {
        // No alignment error.
        return false;
    } else {
        // Don't permit 32bit align on 64bit access in FXU
        return true;
    }
}


PhysAddr Lsu::MemReadWithLinkCommon (PhysAddr vaddr, MemData8* data, ByteSize size)
{
    // In G4MH, compare register address value in LDL and STC.
    // Record address without address translation (self/mirror)
    SetLLMasterAddr (vaddr);

    PhysAddr paddr = AddressTransfer (vaddr);

    PhysAddr llsc_paddr;
    if (IsPeLocalArea (paddr)) {
        llsc_paddr = GetBusAddress (paddr);
    } else {
        llsc_paddr = paddr;
    }

    // delete own link
    m_llsc->DeleteOwnLink (m_sc_id, m_trace);

    // No need update sideband.SPCLD because SPCLD is SPLD_OFF in ldl

    paddr = MemAccess<MACC_NORMAL, DATA_READ> (vaddr, data, size);

    // Never generate a link in Flash ROM area.
    if (IsFlashROMArea (llsc_paddr) == false &&
        !m_trace->IsCancel()) {
        m_llsc->CreateLink (m_sc_id, llsc_paddr, size, m_trace);
    }
    return paddr;
}


MemData Lsu::MemReadWithLink (PhysAddr vaddr, ByteSize size)
{
    MemDataU32 rdata (0x0U);
    MemReadWithLinkCommon (vaddr, rdata.d8, size);
    m_trace->RecordMemAccess<DATA_READ> (vaddr, 0, rdata.d32, size, m_mem_route);
    return rdata.d32;
}


MemData64 Lsu::MemRead64WithLink (PhysAddr vaddr)
{
    MemDataU64 rdata (0x0UL);
    MemReadWithLinkCommon (vaddr, rdata.d8, SIZE_64BIT);
    m_trace->RecordMemAccess<DATA_READ> (vaddr, 0, rdata.d64, SIZE_64BIT, m_mem_route);
    return rdata.d64;
}


bool Lsu::IsHaveLink (PhysAddr addr, ByteSize size) const
{
    PhysAddr llsc_paddr;
    if (IsPeLocalArea (addr)) {
        llsc_paddr = GetBusAddress (addr);
    } else {
        llsc_paddr = addr;
    }

    // LLSC: control llbit by translated address (=llsc_paddr).
    // G4MH: Need to use completely same address in LDL and STC (without self translation).
    return    m_llsc->IsHaveLink (m_sc_id, llsc_paddr, size)
           && IsMatchLLMasterAddr (addr, m_llsc->GetCondMask());
}


#ifdef FROG_API_IF
void Lsu::CreateLink (int32_t scid, PhysAddr paddr, ByteSize size)
{
    m_llsc->DeleteOwnLink(scid, m_trace);
    m_llsc->CreateLink(scid, paddr, size, m_trace);
    m_trace->SetLLBitAdd(paddr, size);
}
#endif //FROG_API_IF


void Lsu::DeleteLink (PhysAddr paddr)
{
    m_llsc->DeleteLink (paddr, m_trace);
}


void Lsu::DeleteOwnLink (PhysAddr paddr, ByteSize size)
{
    PhysAddr llsc_paddr;
    if (IsPeLocalArea (paddr)) {
        llsc_paddr = GetBusAddress (paddr);
    } else {
        llsc_paddr = paddr;
    }

    PhysAddr base_addr = llsc_paddr & Llsc::LINK_ADDR_MASK;
    m_llsc->DeleteOwnLink (m_sc_id, base_addr, m_trace);

    // Delete next align
    PhysAddr last_addr = (llsc_paddr + (size - 1)) & Llsc::LINK_ADDR_MASK;
    if (base_addr != last_addr) {
        m_llsc->DeleteOwnLink (m_sc_id, last_addr, m_trace);
    }
}


void Lsu::DeleteOwnLink (void)
{
    m_llsc->DeleteOwnLink (m_sc_id, m_trace);
}


void Lsu::DeletePeLink ()
{
    m_llsc->DeletePeLink (m_sc_id, m_trace);
}


void Lsu::DeleteVmLink ()
{
    m_llsc->DeleteVmLink (m_sc_id, m_trace);
}


void Lsu::DeleteAllLink (void)
{
    m_llsc->InitLink ();
}


void Lsu::DeleteLinkByExternal (PhysAddr paddr, ByteSize size)
{
    // Becasue of access by external master (not PE), don't convert address for self area
    if (unlikely(m_llsc->IsThereAnyLink () == true)) {
        PhysAddr base_addr = paddr & Llsc::LINK_ADDR_MASK;
        m_llsc->DeleteLink (base_addr, m_trace);

        // Check latter half for accrossing access between 2 links
        PhysAddr last_addr = (paddr + (size - 1)) & Llsc::LINK_ADDR_MASK;
        if (base_addr != last_addr) {
            m_llsc->DeleteLink (last_addr, m_trace);
        }
    }
}


void Lsu::SetScId (int32_t sc_id)
{
    m_sc_id = sc_id;
}


//! Returns the physical address which is asserted to the bus, from the physical address in CPU
//  after self/mirror conversion.
PhysAddr Lsu::GetBusAddress (PhysAddr addr) const
{
    return m_fsbus->GetBusAddress (m_sideband, addr);
}


bool Lsu::IsPeLocalArea (PhysAddr addr) const
{
    return ((addr >= PE_LOCAL_BGN) &&
            (addr <= PE_LOCAL_END));
}


bool Lsu::IsFlashROMArea (PhysAddr addr) const
{
    if (addr >= ROM_BGN && addr <= ROM_END) {
        return true;
    } else {
        return false;
    }
}


void Lsu::InitMemRoute (void)
{
    m_mem_route = NONE_CACHE_ROUTE;
}


void Lsu::ResetPerf (void)
{
    m_fetch_count = 0;
    m_flashrom_fetch_count = 0;
    m_flashrom_data_count = 0;
}


void Lsu::ExecCfali (PhysAddr vaddr)
{
    PhysAddr paddr = AddressTransfer (vaddr);
    if (m_ic->IsCachingArea (paddr)) {
        m_ic->OperateCfali (paddr, vaddr, m_sideband, m_trace);
    }
}


void Lsu::SetTraceCtrl (TraceCtrl* trace)
{
    m_trace = trace;
    m_mpu->SetTraceCtrl (trace);
}


// Need to call UpdateOperandSyserrEvent() by caller function after this function.
void Lsu::InitSyserrEvent (void)
{
#if defined(COMP_RUNNER_G4MH)
    //Init operand syserr flag for host mode.
    m_host_operand_syserr_status = false;
    //Init operand syserr flag for guest mode corresponding to gpid 0->7.
    m_guest_operand_syserr_status.assign (8, false);
#endif
    m_host_syserr_cause = SYSERR_NONE;
    m_guest_syserr_cause.assign (8, SYSERR_NONE);
}

#if defined(COMP_RUNNER_G4MH)
bool Lsu::GetOperandSyserrStatus(bool is_host, uint32_t gpid) const
{
    if (is_host) {
        return m_host_operand_syserr_status;
    } else if (gpid <= 7) {
        return m_guest_operand_syserr_status[gpid];
    } else {
        CfMsg::TPrint (MSG_INF, m_trace, "<Error: Invalid guest gpid 0x%08x>", gpid);
        return false;
    }
}

void Lsu::ClearOperandSyserrStatus(bool is_host, uint32_t gpid)
{
    if (is_host) {
        m_host_operand_syserr_status = false;
    } else if (gpid <= 7) {
        m_guest_operand_syserr_status[gpid] = false;
    } else {
        CfMsg::TPrint (MSG_INF, m_trace, "<Error: Invalid guest gpid 0x%08x>", gpid);
    }
}
#endif

SysErrCause Lsu::GetSyserrCause (bool is_host, uint32_t gpid) const
{
    if (is_host) {
        return m_host_syserr_cause;
    } else if (gpid <= 7) {
        return m_guest_syserr_cause[gpid];
    } else {
        return SYSERR_NONE; // Never come here.
    }
}


void Lsu::SetSyserrCause (bool is_host, uint32_t gpid, SysErrCause cause)
{
    if (is_host) {
        m_host_syserr_cause = cause;
    } else if (gpid <= 7) {
        m_guest_syserr_cause[gpid] = cause;
    }

#if defined(COMP_RUNNER_G4MH)
    if (cause == SYSERR_NONE) {
        ClearOperandSyserrStatus(is_host, gpid);
    }
#endif
    m_pe->GetHtPtr()->UpdateOperandSyserrEvent();
}


Lsu::Lsu (TraceCtrl* trace, FsBus* fsbus, Icache* ic, SRegFile* srf,
#ifdef ENABLE_DBG_BREAK
          DbgBreak* dbgbrk,
#endif // #ifdef ENABLE_DBG_BREAK
          Llsc* llsc, Misr* misr, ContextId tc_id,
          MissAlignType maa, ProcElement* pe,
          SysErrCause syserr_fetch_guest_error, bool enable_operand_syserr)
         : m_trace (trace), m_fsbus (fsbus), m_ic (ic), m_srf (srf),
#ifdef ENABLE_DBG_BREAK
           m_dbgbrk (dbgbrk),
#endif // ENABLE_DBG_BREAK
           m_llsc (llsc),
           m_misr (misr), m_tc_id (tc_id),
           m_miss_align_type (maa), m_pe (pe), m_mem_route (NONE_CACHE_ROUTE),
           m_syserr_cause_fetch_guest (syserr_fetch_guest_error),
           m_enable_operand_syserr (enable_operand_syserr)
{
    m_mpu = new Mpu (trace, srf, pe->GetCoreType(), pe->GetCoreVersion());
    m_gram_bgn_addr = L2RAM_BGN;
    m_gram_end_addr = L2RAM_END;
    m_lram_bgn_addr = PE_LOCAL_BGN;
    m_lram_end_addr = L2RAM_BGN - 1UL;
    m_mdp_supress = false;
    m_llmaster_addr = 0;
    InitSyserrEvent ();
}

Lsu::~Lsu (void)
{
    delete m_mpu;
}


template bool Lsu::IsMemExceptionAtomic<SIZE_8BIT> (PhysAddr vaddr);
template bool Lsu::IsMemExceptionAtomic<SIZE_16BIT> (PhysAddr vaddr);
template bool Lsu::IsMemExceptionAtomic<SIZE_32BIT> (PhysAddr vaddr);
template bool Lsu::IsMemException<DATA_READ> (PhysAddr vaddr, ByteSize size);
template bool Lsu::IsMemException<DATA_WRITE> (PhysAddr vaddr, ByteSize size);
template bool Lsu::IsMemException<INST_READ> (PhysAddr vaddr, ByteSize size);
template bool Lsu::IsMemExceptionFXU<DATA_READ> (PhysAddr vaddr, ByteSize size);
template bool Lsu::IsMemExceptionFXU<DATA_WRITE> (PhysAddr vaddr, ByteSize size);
template bool Lsu::IsMemExceptionEI<DATA_READ> (PhysAddr vaddr, ByteSize size);
template bool Lsu::IsMemExceptionEI<DATA_WRITE> (PhysAddr vaddr, ByteSize size);
