/*
 * (c) 2011 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <bitset>
#include "./config.h"
#include "forest_common.h"
#include "dbg_break.h"
#include "sregfile.h"
#include "trace_info.h"
#include "inst_id_list.h"

#ifdef ENABLE_DBG_BREAK
ExpCode DbgBreak::UpdateBreakPcbExcp (PhysAddr addr, TraceCtrl* trace)
{
    if (unlikely (m_srf->SrDIR1()->GetBEN() == 1 &&
                  IsBrkExcpAcceptable () == true)) {
        // To prevent SQ occur by 1 instruction, check by reverce order.
        // - In case of PCB set to same PC by CH0 and CH1.
        //   - If check from CH0, CH0 is matched and BPC0.EO is set to 1 at first.
        //     In next loop, CH1 is checked and then CH0.EO=1. SQ0 will occur by 1 inst wrongly.
        //   - If check by reverce order, CH1 is checked at first.
        //     Then, because CH0.EO is 0 yet, CH1 isn't matched. Only CH0 is matched correctly.
        for (int32_t ch = N_BPA - 1; ch >= 0; --ch) {
            SRegBPC_N* bpc = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
            if (bpc->GetFE() == 1) {
                if ((bpc->GetRE() == 1) || (bpc->GetWE() == 1)) {
                    CfMsg::DPrint (MSG_WAR, "<Warning: Unguaranteed setting in BPC%u RE/WE isn't 0 even FE=1>\n", ch);
                    continue;
                }
                // @@ VM isn't supported.
                if (IsSeqBrkHit (ch) == false) {
                    continue;
                }
                if (IsDebuggableMode(ch)) {
                    if (IsBrkAddrHit (ch, addr) == true) {
                      CfMsg::TPrint (MSG_INF, trace, "<Break: ch%d: PC Break(PCB): %08x>\n", ch, addr);
                      UpdateExpCodeAndSR (ch, EXP_PCB_DB, trace);
                    }
                }
            }
        }
        return trace->GetExpCode();
    }
    return EXP_NONE;
}


template <MemAccessCommand cmd>
ExpCode DbgBreak::UpdateBreakLsabExcp (PhysAddr addr, ByteSize size,
                                       TraceCtrl* trace, LsabType is_eiint)
{
    if (unlikely (m_srf->SrDIR1()->GetBEN() == 1 &&
                  IsBrkExcpAcceptable () == true)) {
        for (int32_t ch = N_BPA - 1; ch >= 0; --ch) {
            SRegBPC_N* bpc = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
            if (   (((cmd == DATA_READ)  || (cmd == DATA_RMW)) && (bpc->GetRE() == 1))
                || (((cmd == DATA_WRITE) || (cmd == DATA_RMW)) && (bpc->GetWE() == 1))) {
                if (bpc->GetFE() == 1) {
                    CfMsg::DPrint (MSG_WAR, "<Warning: Unguaranteed setting in BPC%u FE isn't 0 even RE/WE=1>\n", ch);
                    continue;
                }

                if (IsSeqBrkHit (ch) == false) {
                    continue;
                }
                if (IsDebuggableMode(ch)) {
                    if (IsBrkAddrSizeHit (ch, addr, size, trace) == true) {
                        CfMsg::TPrint (MSG_INF, trace, "<Break: ch%d: Address Break(LSAB): %08x>\n", ch, addr);
                        if (is_eiint == LSAB_NORMAL) {
                          UpdateExpCodeAndSR (ch, EXP_LSAB_DB, trace);
                        } else {
                          UpdateExpCodeAndSR (ch, EXP_LSAB_EIINT, trace);
                        }
                    }
                }
            }
        }
        return trace->GetExpCode();
    }
    return EXP_NONE;
}

bool DbgBreak::IsBrkAddrHit (uint32_t ch, PhysAddr addr) const
{
  PhysAddr ba_value = m_srf->GetSrPtr(CF_SR_BPAV0 + ch)->ReadBody ();
  PhysAddr ba_mask  = ~ m_srf->GetSrPtr(CF_SR_BPAM0 + ch)->ReadBody ();

  SRegBPC_N* bpc = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
  if (bpc->GetVA() == 0) {
    return (addr & ba_mask) == (ba_value & ba_mask);
  } else {
    return (addr & ba_mask) != (ba_value & ba_mask);
  }
}


bool DbgBreak::IsBrkSizeHit (uint32_t ch, ByteSize size, TraceCtrl* trace) const
{
    SRegBPC_N* bpc = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
    switch (bpc->GetTY()) {
    case 0: return true; break;
    case 1: return (size == SIZE_8BIT);  break;
    case 2: return (size == SIZE_16BIT); break;
    case 3: return (size == SIZE_32BIT); break;
    case 4: return (size == SIZE_64BIT); break;
    case 5: return (size == SIZE_128BIT); break;
    default:
        CfMsg::TPrint (MSG_WAR, trace, "<Warning: illegal BPC%d.TY value: %d>\n",
                  ch, bpc->GetTY());
        return false;
        break;
    }
}


bool DbgBreak::IsBrkAddrSizeHit (uint32_t ch, PhysAddr addr, ByteSize size, TraceCtrl* trace) const
{
    return (IsBrkAddrHit (ch, addr) == true &&
            IsBrkSizeHit (ch, size, trace) == true );
}


bool DbgBreak::IsSeqBrkHit (uint32_t ch) const
{
    switch (ch) {
    case 0: // CH0 supports sequentical break
        return     (m_srf->SrDIR1()->GetSQ0() == 0)
                || (IsSeqBrkValidComb (0) == true);
    case 2: // CH2 supports sequentical break
        return     (m_srf->SrDIR1()->GetSQ1() == 0)
                || (IsSeqBrkValidComb (2) == true);
        break;
    case 1: // CH1 is sequentical break with CH0
        return     (m_srf->SrDIR1()->GetSQ0() == 0)
                || ((m_srf->SrBPC0()->GetEO() == 1) && (IsSeqBrkValidComb (0) == true));
        break;
    case 3: // CH3 is sequentical break with CH2
        return     (m_srf->SrDIR1()->GetSQ1() == 0)
                || ((m_srf->SrBPC2()->GetEO() == 1) && (IsSeqBrkValidComb (2) == true));
        break;
    default: // Because other channels don't support sequential break, return true always.
        return true;
        break;
    }
}


bool DbgBreak::IsSeqBrkValidComb (uint32_t ch) const
{
    // Sequential break is valid only when both channels are PC break setting.
    SRegBPC_N* bpc_1st = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
    SRegBPC_N* bpc_2nd = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch + 1));
    return    (bpc_1st->GetFE() == 1) && (bpc_1st->GetRE() == 0) && (bpc_1st->GetWE() == 0)
           && (bpc_1st->GetBE() == 0) && (bpc_1st->GetTE() == 0)
           && (bpc_2nd->GetFE() == 1) && (bpc_2nd->GetRE() == 0) && (bpc_2nd->GetWE() == 0);
}


void DbgBreak::ClearPcbEoReserved (TraceCtrl* trace)
{
    std::vector<uint32_t>::iterator it = m_pcb_eo_reserve.begin ();
    while (it != m_pcb_eo_reserve.end ()) {
        uint32_t ch = *it;
        CfMsg::TPrint (MSG_INF, trace, "<Break: ch%d: PC Break(PCB): Event is canceled>\n", ch);
        ++ it;
    }
    m_pcb_eo_reserve.clear ();
}


void DbgBreak::ApplyPcbEoReserved (TraceCtrl* trace)
{
    std::vector<uint32_t>::iterator it = m_pcb_eo_reserve.begin ();
    while (it != m_pcb_eo_reserve.end ()) {
        uint32_t ch = *it;
        SRegBPC_N* bpc = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
        bpc->SetEO (1);
        trace->RecordSReg<TraceCtrl::REG_WRITE> (static_cast<CfSReg>(CF_SR_BPC0 + ch), bpc->ReadBody());
        ++ it;
    }
    m_pcb_eo_reserve.clear ();
}


void DbgBreak::ClearLsabEoReserved (TraceCtrl* trace)
{
    std::vector<uint32_t>::iterator it = m_lsab_eo_reserve.begin ();
    while (it != m_lsab_eo_reserve.end ()) {
        uint32_t ch = *it;
        CfMsg::TPrint (MSG_INF, trace, "<Break: ch%d: Address Break(LSAB): Event is canceled>\n", ch);
        ++ it;
    }
    m_lsab_eo_reserve.clear ();
}


void DbgBreak::ApplyLsabEoReserved (TraceCtrl* trace)
{
    std::vector<uint32_t>::iterator it = m_lsab_eo_reserve.begin ();
    while (it != m_lsab_eo_reserve.end ()) {
        uint32_t ch = *it;
        SRegBPC_N* bpc = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
        bpc->SetEO (1);
        trace->RecordSReg<TraceCtrl::REG_WRITE> (static_cast<CfSReg>(CF_SR_BPC0 + ch), bpc->ReadBody());
        ++ it;
    }
    m_lsab_eo_reserve.clear ();
}


void DbgBreak::InitEoReserved (void)
{
    m_pcb_eo_reserve.clear ();
    m_lsab_eo_reserve.clear ();
}


void DbgBreak::UpdateExpCodeAndSR (uint32_t ch, ExpCode exp_code, TraceCtrl* trace)
{
    SRegBPC_N* bpc = static_cast<SRegBPC_N*> (m_srf->GetSrPtr(CF_SR_BPC0 + ch));
    if (bpc->GetBE() == 1) {
        // Doen't break on BPC.BE=0
        bpc->SetEO (1);
        trace->RecordSReg<TraceCtrl::REG_WRITE> (static_cast<CfSReg>(CF_SR_BPC0 + ch), bpc->ReadBody());
        uint32_t bt = m_srf->SrDIR1()->GetBT ();
        bt |= (1 << ch);
        m_srf->SrDIR1()->SetBT (bt);
        trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_DIR1, m_srf->SrDIR1()->ReadBody());

        switch (exp_code) {
        case EXP_PCB_DB:
        case EXP_LSAB_DB:
        case EXP_LSAB_EIINT:
            // プレサイス例外は即時発行
            trace->SetExpCode (exp_code);
            break;
        default:
            CfMsg::TPrint (MSG_ERR, trace, "<Internal Error: Unreachable code>\n");
            break;
        }
    } else {
        if (exp_code == EXP_PCB_DB) {
            m_pcb_eo_reserve.push_back (ch);
        } else {
            m_lsab_eo_reserve.push_back (ch);
        }
    }
}


bool DbgBreak::IsBrkExcpAcceptable (void) const
{
    if (m_srf->SrDIR0()->GetDM() == 1) {
        return false;
    } else {
        return true;
    }
}

bool DbgBreak::IsDebuggableMode(uint32_t ch) const {
  if (!m_vm_supported) {
    return true; //                         Exit (a)
  } else {
    if (m_srf->SrHVCFG()->GetHVE() == 1) {
      bool mode = static_cast<bool>(m_srf->SrPSWH()->GetGM());
      if (mode) {
        // guest mode
        uint32_t gpid = m_srf->SrPSWH()->GetGPID();
        std::bitset<8> dbgen_ge = m_srf->SrDBGEN()->GetGE();
        if (dbgen_ge[gpid]) {
          SRegBPC_N* bpc = static_cast<SRegBPC_N*>(m_srf->GetSrPtr(CF_SR_BPC0 + ch));
          std::bitset<8> bpc_ge = bpc->GetGE();
          return bpc_ge[gpid]; // BPC.GEm   Eixt (c)
        } else {
          return false; // DBGEN not match  Eixt (d)
        }
      } else {
        // host mode
        bool dbgen_he = static_cast<bool>(m_srf->SrDBGEN()->GetHE());
        if (dbgen_he) {
          SRegBPC_N* bpc = static_cast<SRegBPC_N*>(m_srf->GetSrPtr(CF_SR_BPC0 + ch));
          bool bpc_he = bpc->GetHE();
          return bpc_he; // BPC.HE          Exit (e)
        } else {
          return false; // DBGEN not match  Eixt (f)
        }
      }
    } else {
      return true; // HVE == 0              Eixt (b)
    }
  }
}

DbgBreak::DbgBreak (SRegFile* srf, CoreType core_type, CoreVersion core_version)
  : m_srf (srf), m_core_type (core_type), m_core_version (core_version)
{
  m_vm_supported = ((core_type == CORE_G4MH) && (core_version >= CORE_VER_20));
}

template ExpCode DbgBreak::UpdateBreakLsabExcp<DATA_READ>(PhysAddr addr, ByteSize size,
                                                          TraceCtrl* trace, LsabType is_eiint);
template ExpCode DbgBreak::UpdateBreakLsabExcp<DATA_WRITE>(PhysAddr addr, ByteSize size,
                                                           TraceCtrl* trace, LsabType is_eiint);
template ExpCode DbgBreak::UpdateBreakLsabExcp<DATA_RMW>(PhysAddr addr, ByteSize size,
                                                         TraceCtrl* trace, LsabType is_eiint);
#endif // #ifdef ENABLE_DBG_BREAK
