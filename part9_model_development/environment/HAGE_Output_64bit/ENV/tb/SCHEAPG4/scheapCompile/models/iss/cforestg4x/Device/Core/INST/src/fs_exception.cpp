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
#include <algorithm>
#include "./config.h"
#include "./forest_common.h"
#include "./inst_v850.h"
#include "./trace_info.h"
#include "./gregfile.h"
#include "./sregfile.h"
#include "./fs_controler.h"
#include "./exp_info.h"
#include "./intc1.h"
#include "./exp_message.h"
#include "./proc_element.h"
#include "./dbg_break.h"
#include "./counter_base.h"

/*
 * @brief 例外処理
 * @par 更新トレース
 *      - is_branched
 *      - next_inst_addr
 */
void FsCtrl::ExceptionProcess (ExpCode exp_code)
{
    // 完了型例外は各命令で実施済み (ここには来ない)
    PhysAddr ret_pc     = m_trace->GetInstAddr ();
    uint32_t exp_cause  = ExpInfo::GetExpCause (exp_code);
    uint32_t hndl_ofst  = ExpInfo::GetHdlOffset (exp_code);
    bool     is_excp    = true;  // PSW.EP用: true = 例外
    bool     is_table   = false; // 例外ハンドラ仕様 (false:標準 true:テーブル)

    switch (ExpInfo::GetExpLevel (exp_code)) {
    case ExpInfo::EXP_LV_EI:
        m_insts->ExceptionEI (exp_cause, ret_pc, hndl_ofst, is_excp, is_table);
        break;
    case ExpInfo::EXP_LV_FE:
        m_insts->ExceptionFE (exp_cause, ret_pc, hndl_ofst, is_excp);
        break;
    case ExpInfo::EXP_LV_DB:
        m_insts->ExceptionDB (exp_cause, ret_pc, hndl_ofst, is_excp, exp_code);
        break;
    default:
        // RS はここには来ない
        break;
    }

    m_exp_msg->ExpMsgPrint (exp_code, m_trace->GetExpCause (), m_trace);
}


void FsCtrl::CountBgintEvent (uint32_t accepted_gpid)
{
    // PMCOUNT
    uint32_t ch = 0;
    uint32_t aborted_gpid = m_nc_srf->SrPSWH()->GetGPID ();
    std::vector <PmCounter*>::iterator it = m_pmcount.begin ();
    while (it != m_pmcount.end ()) {
        // Update for guest mode
        PmCounter::PmCondType cond = (*it)->GetConditionBody ();

        // CND=24/25 : count if (PMCTRL.GM=1&&PMCTRL.GPID=BGINT.GPID)
        if (   (cond == PmCounter::PM_CNT_BGINT_SUSPENDED)
            && ((m_pe->IsHostMode () == false) && ((*it)->IsGpidMatch (aborted_gpid) == true)) ) {
            ++ m_bgint_abort_count [ch]; // PMCTRL.CND=24h
        }

        if (   (cond == PmCounter::PM_CNT_BGINT_ACCEPTED)
            && ((m_pe->IsHostMode () == false) && ((*it)->IsGpidMatch (accepted_gpid) == true)) ) {
            ++ m_bgint_accepted_count [ch]; // PMCTRL.CND=25h
        }
        ++ it;
        ++ ch;
    }
    m_update_pmc = true;
}

/*
 * @brief 割り込み処理
 * @par 更新トレース
 *      - inst_addr
 *      - exp_code
 *      - is_branched
 *      - next_inst_addr
 */
void FsCtrl::InterruptProcess (void)
{
    IntCause* int_cause = NULL;
    uint32_t exp_cause;
    ExpInfo::ExpLevel exp_level;
    uint32_t int_priority = 0;
    bool eitb = false;
    uint32_t channel = 0;

#if defined(COMP_RUNNER)
    bool allcommit_done = true;
#endif // defined(COMP_RUNNER)

    uint32_t org_int_priority = 0;
    if (m_selected_factor == INT_FAC_INT) {
        int_cause = m_int_queue.back();
        int_priority = int_cause->m_int_priority;
        org_int_priority = int_priority;
        if (int_priority >= 16) {
            // pri16 and higher use vector, ISPR for pri15
            int_priority = 15;
        }
        exp_cause = int_cause->m_int_cause;
        eitb = int_cause->m_eitb;
        ExpCode eiint_type = int_cause->GetEiintType ();
#if defined(COMP_RUNNER)
        allcommit_done = int_cause->m_isinstdone;
#endif // defined(COMP_RUNNER)

        exp_level = ExpInfo::GetExpLevel (eiint_type);
        channel = exp_cause - ExpInfo::GetExpCause (eiint_type);
        m_trace->SetExpCode (eiint_type);

        if (eiint_type == EXP_BGEIINT) {
            // set GPID to EIIC[18:16]
            uint32_t bgeiint_gpid = (int_cause->GetGPID()) & 0x7;
            exp_cause |= bgeiint_gpid << 16;

            // PMCOUNT for BGEIINT
            CountBgintEvent (bgeiint_gpid);
        }

    } else if (m_selected_factor == INT_FAC_FENMI) {
        exp_cause = ExpInfo::GetExpCause (EXP_FENMI);
        exp_level = ExpInfo::GetExpLevel (EXP_FENMI);
        m_trace->SetExpCode (EXP_FENMI);

#if defined(COMP_RUNNER)
        allcommit_done = m_fenmi_cause->m_isinstdone;
#endif // defined(COMP_RUNNER)

        if (m_fenmi_cause->m_resfunc != NULL) {
            (m_fenmi_cause->m_resfunc)(m_intc1);
        }

        delete (m_fenmi_cause);
        m_fenmi_cause = NULL;
        UpdateIntRequested ();
        UpdateHtRunable ();

    } else if (m_selected_factor == INT_FAC_PSEUDO_SYSERR_IF) {
        exp_cause = m_syserr_cause_pseudo_if->m_syserr_cause;
        exp_level = ExpInfo::GetExpLevel (EXP_SYSERR_IF);
        m_syserr_fetch_cause = exp_cause; // Set syserr_if cause for ExceptionFE.
        m_trace->SetExpCode (EXP_SYSERR_IF);
        delete (m_syserr_cause_pseudo_if);
        m_syserr_cause_pseudo_if = NULL;
        UpdateIntRequested ();

    } else if (m_selected_factor == INT_FAC_SYSERR_OP) {
        exp_cause = m_syserr_cause_op->m_syserr_cause;
        uint32_t gpid;
        bool is_caused_by_host = m_syserr_cause_op->m_is_caused_by_host;
        if (is_caused_by_host) {
            // Acepted operand syserr is caused by host mode or conventional mode.
            gpid = 0;
            exp_level = ExpInfo::GetExpLevel (EXP_SYSERR_HM);
            m_trace->SetExpCode (EXP_SYSERR_HM);
        } else {
            // Acepted operand syserr is caused by guest mode.
            gpid = m_srf->SrPSWH ()->GetGPID ();
            exp_level = ExpInfo::GetExpLevel (EXP_SYSERR_GM);
            m_trace->SetExpCode (EXP_SYSERR_GM);
        }
        delete (m_syserr_cause_op);
        m_syserr_cause_op = NULL;

        // Update DEVDS (Clear accepted syserr)
        //   Because operand syserr requested only for current mode,
        //   The operand syserr which caused by different mode doesn't occur.
        //   So, clear flag for the current mode always.
#if defined(COMP_RUNNER_G4MH)
        //Clear opearand syserr flag for guest mode by corresponding gpid
        m_lsu->ClearOperandSyserrStatus(is_caused_by_host, gpid);
#endif
        m_lsu->SetSyserrCause (is_caused_by_host, gpid, SYSERR_NONE);
        UpdateIntRequested ();
    } else if (m_selected_factor == INT_FAC_DBINT) {
        exp_cause = ExpInfo::GetExpCause (EXP_DBINT) + m_channel_dbint;
        exp_level = ExpInfo::GetExpLevel (EXP_DBINT);
        m_trace->SetExpCode (EXP_DBINT);

        m_req_dbint = false;
        m_channel_dbint = 0;
        UpdateIntRequested ();
        UpdateHtRunable ();
    } else if (m_selected_factor == INT_FAC_DBNMI) {
        exp_cause = ExpInfo::GetExpCause (EXP_DBNMI);
        exp_level = ExpInfo::GetExpLevel (EXP_DBNMI);
        m_trace->SetExpCode (EXP_DBNMI);

        m_req_dbnmi = false;
        UpdateIntRequested ();
        UpdateHtRunable ();
    } else if (m_selected_factor == INT_FAC_RLB) {
        exp_cause = ExpInfo::GetExpCause (EXP_RLB);
        exp_level = ExpInfo::GetExpLevel (EXP_RLB);
        m_trace->SetExpCode (EXP_RLB);

        m_req_rlb = false;
        UpdateIntRequested ();
        UpdateHtRunable ();
    } else if (m_selected_factor == INT_FAC_FEINT) {
        // FEINT, GMFEINT, BGFEINT
        uint32_t feint_channel = m_feint_cause->m_feint_channel;
        ExpCode feint_type = m_feint_cause->GetFeintType ();
        exp_cause = ExpInfo::GetExpCause (feint_type) + feint_channel;
        if (feint_type == EXP_BGFEINT) {
            // set GPID to EIIC[18:16]
            uint32_t gpid = (m_feint_cause->GetGPID()) & 0x7;
            exp_cause |= gpid << 16;
            CountBgintEvent (gpid);
        }
        exp_level = ExpInfo::GetExpLevel (feint_type);
        m_trace->SetExpCode (feint_type);

#if defined(COMP_RUNNER)
        allcommit_done = m_feint_cause->m_isinstdone;
#endif // defined(COMP_RUNNER)

        if (feint_type != EXP_BGFEINT) {
            if (m_feint_cause->m_resfunc != NULL) {
                (m_feint_cause->m_resfunc)(feint_channel, m_intc1);
            }
            delete (m_feint_cause);
            m_feint_cause = NULL;
        }
        UpdateIntRequested ();
        UpdateHtRunable ();

        // For PMCTRL.CND=21
        m_accepted_feint_ch = feint_channel;
    } else {
        // Doesn't come here
        exp_cause = EXP_NONE;
        exp_level = ExpInfo::EXP_LV_RS;
    }

#if defined(COMP_RUNNER)
    PhysAddr current_pc = m_trace->GetInstAddr ();
#endif // defined(COMP_RUNNER)

    PhysAddr pc = m_grf->ReadPC ();
    m_trace->SetInstAddr (pc);

    PhysAddr ret_pc  = pc;
#if defined(COMP_RUNNER)
    ret_pc = (allcommit_done == true || ((m_trace->GetExpCode() != EXP_EIINT) && (m_trace->GetExpCode() != EXP_GMEIINT) && (m_trace->GetExpCode() != EXP_BGEIINT))) ? pc :
        current_pc;
#endif // defined(COMP_RUNNER)

    bool     is_excp = false; // PSW.EP用: false = 割り込み
    bool     is_table = false;
    uint32_t hndl_ofst;

    ExpCode exp_code = m_trace->GetExpCode ();
    if (exp_code == EXP_FENMI) {
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_FENMI);
    } else if (exp_code == EXP_SYSERR_IF) { // Pseudo SYSERR_IF
        is_excp = true;
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_SYSERR_IF);
    } else if (exp_code == EXP_SYSERR_HM) {
        is_excp = true;
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_SYSERR_HM);
    } else if (exp_code == EXP_SYSERR_GM) {
        is_excp = true;
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_SYSERR_GM);
    } else if (exp_code == EXP_FEINT) {
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_FEINT);
    } else if (exp_code == EXP_GMFEINT) {
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_GMFEINT);
    } else if (exp_code == EXP_BGFEINT) {
        is_excp = true; // BGFEINT is treated as exception not interruption.
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_BGFEINT);
    } else if (exp_code == EXP_RLB) {
        is_excp = true; // Because RLB comes from not INTC.
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_RLB);
    } else if (exp_code == EXP_DBINT) {
        is_excp = true; // Because DBINT comes from RCU not INTC.
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_DBINT);
    } else if (exp_code == EXP_DBNMI) {
        is_excp = true; // Because DBNMI comes from RCU not INTC.
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_DBNMI);
    } else if (exp_code == EXP_BGEIINT) {
        is_excp = true; // BGEIINT is treated as exception not interruption.
        hndl_ofst = ExpInfo::GetHdlOffset (EXP_BGEIINT);
    } else if (exp_code == EXP_GMEIINT) {
        bool rint = m_nc_srf->SrGMEBASE()->GetRINT ();
        bool dv   = m_nc_srf->SrGMEBASE()->GetDV ();
        if (dv == 1 || eitb == false) {
            // 標準仕様(優先度に基づいた直接分岐方式)
            is_table = false;
            if (rint == 1) {
                // 例外ハンドラ縮小モード
                hndl_ofst = ExpInfo::GetHdlOffset (EXP_GMEIINT);
            } else {
                hndl_ofst = ExpInfo::GetHdlOffset (EXP_GMEIINT) + (int_priority << 4);
            }
        } else {
            // 拡張仕様(テーブル参照方式)
            is_table = true;
            hndl_ofst = channel * 4;
        }
    } else { // if (exp_code == EXP_EIINT) {
        bool rint;
        bool dv;
        if (m_nc_srf->SrHMPSW()->GetEBV () == 0) {
            rint = m_nc_srf->SrRBASE()->GetRINT ();
            dv   = m_nc_srf->SrRBASE()->GetDV ();
        } else {
            rint = m_nc_srf->SrHMEBASE()->GetRINT ();
            dv   = m_nc_srf->SrHMEBASE()->GetDV ();
        }
#ifdef DEBUG_CORE_INT
        CfMsg::TPrint (MSG_ERR, m_trace, "[Core]InterruptProcess: EIINT (eitb=%x): HMPSW.EBV=%x, RBASE.RINT=%x, DV=%x, EBASE.RINT=%x, DV=%x\n",
                       eitb, m_nc_srf->SrHMPSW()->GetEBV (),
                       m_nc_srf->SrRBASE()->GetRINT (), m_nc_srf->SrRBASE()->GetDV (),
                       m_nc_srf->SrHMEBASE()->GetRINT (), m_nc_srf->SrHMEBASE()->GetDV ());
#endif // DEBUG_CORE_INT
        if (dv == 1 || eitb == false) {
            // 標準仕様(優先度に基づいた直接分岐方式)
            is_table = false;
            if (rint == 1) {
                // 例外ハンドラ縮小モード
                hndl_ofst = ExpInfo::GetHdlOffset (EXP_EIINT);
            } else {
                hndl_ofst = ExpInfo::GetHdlOffset (EXP_EIINT) + (int_priority << 4);
            }
        } else {
            // 拡張仕様(テーブル参照方式)
            is_table = true;
            hndl_ofst = channel * 4;
        }
    }

    switch (exp_level) {
    case ExpInfo::EXP_LV_EI:
        m_insts->ExceptionEI (exp_cause, ret_pc, hndl_ofst, is_excp, is_table);
        // BGEIINT/BGFEINT don't send ack to INTC (INTC keeps EIINT/FEINT request).
        // After background GM is resotored by HV software, GMEIINT/GMFEINT occurs again.
        // BGFEINT is EI level.
        if ((m_trace->GetExpCode() == EXP_EIINT) || (m_trace->GetExpCode() == EXP_GMEIINT) || (m_trace->GetExpCode() == EXP_BGEIINT)) {
            if ((m_trace->GetExpCode() == EXP_EIINT) || (m_trace->GetExpCode() == EXP_GMEIINT)) {
                if (int_cause->m_resfunc != NULL) {
                    (int_cause->m_resfunc)(channel, m_intc1);
                    // In CompRunner G4MH, please doesn't use this condition because it can cause segmentation fault.
#if defined(COMP_RUNNER_G3KH) || defined(COMP_RUNNER_G3MH)
                    if (m_int_queue.size() > 0) { // This condition prevents segmentation fault error from force EIINT model in RTL env.
                        delete (int_cause);
                        m_int_queue.pop_back ();
                        UpdateIntRequested ();
                        UpdateHtRunable ();
                    }
#endif // defined(COMP_RUNNER_G3KH) || defined(COMP_RUNNER_G3MH)
                } else {
                    delete (int_cause);
                    m_int_queue.pop_back ();
                    UpdateIntRequested ();
                    UpdateHtRunable ();
                }
                // For PMCTRL.CND=20
                m_accepted_eiint_ch = channel;
            }
            if ((m_trace->GetExpCode() == EXP_EIINT) && (m_srf->SrVINTCFG()->GetEPL() == 0) && (m_srf->SrVINTCFG()->GetISPC() == 0)) {
                // ISPR.ISPビットのうち、受け付けた割り込みの優先度に対応するビットに1をセット
                // If interruption priority is 16-63 on EPL=0, ISP isn't updated.
                if (org_int_priority < 16) {
                    RegData val = m_srf->SrISPR()->GetISP();
                    val |= (0x1 << int_priority);
                    m_srf->SrISPR()->SetISP(val);
                    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_ISPR, m_srf->SrISPR()->ReadBody());
                }
            }
            // In CompRunner G4MH, please doesn't use this condition because it can cause segmentation fault.
#if defined(COMP_RUNNER_G3KH) || defined(COMP_RUNNER_G3MH)
        } else if ((m_trace->GetExpCode() == EXP_MDP) || (m_trace->GetExpCode() == EXP_MDP_EIINT)) {
            if (m_int_queue.size() > 0) { // This condition prevents segmentation fault error from force EIINT model in RTL env.
                delete (int_cause);
                m_int_queue.pop_back ();
                UpdateIntRequested ();
                UpdateHtRunable ();
            }
#endif // defined(COMP_RUNNER_G3KH) || defined(COMP_RUNNER_G3MH)
        }
        break;
    case ExpInfo::EXP_LV_FE:
        m_insts->ExceptionFE (exp_cause, ret_pc, hndl_ofst, is_excp);
        break;
    case ExpInfo::EXP_LV_DB:
        m_insts->ExceptionDB (exp_cause, ret_pc, hndl_ofst, is_excp, exp_code);
        break;
    default:
        break;
    }

    // Exp messages for interruptions and SYSERR_HM/GM needs to be shown here.
    // Exp messages for exceptions are shown in ExceptionProcess().
    switch (m_trace->GetExpCode ()) {
    case EXP_EIINT:
    case EXP_FENMI:
    case EXP_SYSERR_IF: // Pseudo SYSERR_IF
    case EXP_SYSERR_HM:
    case EXP_SYSERR_GM:
    case EXP_FEINT:
    case EXP_GMFEINT:
    case EXP_BGFEINT:
    case EXP_GMEIINT:
    case EXP_BGEIINT:
    case EXP_DBNMI:
    case EXP_DBINT:
        m_exp_msg->ExpMsgPrint (exp_code, exp_cause, m_trace);
        break;
    default:
        break;
    }

#ifdef ENABLE_DBG_BREAK
    // When access is issued accutually,
    // BPC.EO is set already in inst_v850.cpp.
    // Access isn't issued when MDP occur. Then, need to clear reserved info.
    // If exception is LASB, apply reserved info to EO bit.
    ExpCode exp_code_eo = m_trace->GetExpCode ();
    if ((exp_code_eo == EXP_LSAB_DB)
        || (exp_code_eo == EXP_LSAB_EIINT)) {
        m_dbgbrk->ApplyLsabEoReserved (m_trace);
    } else {
        m_dbgbrk->ClearLsabEoReserved (m_trace);
    }
#endif // #ifdef ENABLE_DBG_BREAK
}


/*!
 * @brief  割り込み要求の通知
 */
IntErrorCode FsCtrl::ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                                   bool is_gm, uint32_t gpid, bool is_bgint,
                                   void (*resfunc)(uint32_t channel, CIntc1* intc1))
{
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqInterrupt: called: pri=0x%x, cause=0x%x, eitb=%x, gm=%x, gpid=0x%x, bg=%x\n",
                   int_priority, int_cause, eitb, is_gm, gpid, is_bgint);
#endif // DEBUG_CORE_INT

    std::vector<IntCause*>::iterator itr = m_int_queue.begin ();
    while (itr != m_int_queue.end ()) {
        if ((*itr)->m_int_cause == int_cause) {
            // @@ 暫定対応
            // @@ 同じ発行元からの割込みがキューに存在する場合は何もしない
            // @@ ほんとは良くないのでしっかり実装する
            if ((*itr)->m_eitb != eitb) {
                (*itr)->m_eitb = eitb;
            }
            if ((*itr)->m_is_int_gm != is_gm) {
                (*itr)->m_is_int_gm = is_gm;
            }
            if ((*itr)->m_int_gpid != gpid) {
                (*itr)->m_int_gpid = gpid;
            }
            if ((*itr)->m_is_int_bgint != is_bgint) {
                (*itr)->m_is_int_bgint = is_bgint;
            }
            if ((*itr)->m_int_priority != int_priority) {
                (*itr)->m_int_priority = int_priority;
                // 優先度が変更されたため、割込みキューのソートが必要
                sort (m_int_queue.begin(), m_int_queue.end(), GreaterIntPriority);
                UpdateIntRequested ();
            }
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqInterrupt: error: INT_E_NOT_RECEIVED\n");
#endif // DEBUG_CORE_INT
            return INT_E_NOT_RECEIVED;
        }
        ++ itr;
    }

#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqInterrupt: register: pri=0x%x, cause=0x%x, eitb=%x, gm=%x, gpid=0x%x, bg=%x\n",
                   int_priority, int_cause, eitb, is_gm, gpid, is_bgint);
#endif // DEBUG_CORE_INT
    // @@ 優先度判定はここ？ INTC？ 後で検討
    IntCause* cause = new IntCause (int_priority, int_cause, eitb, is_gm, gpid, is_bgint, resfunc);
    m_int_queue.push_back (cause);
    // 割込みキューのソート
    sort (m_int_queue.begin(), m_int_queue.end(), GreaterIntPriority);
    UpdateIntRequested ();

    return INT_E_OK;
}


#if defined(COMP_RUNNER)
/*!
 * @brief  割り込み要求の通知
 */
IntErrorCode FsCtrl::ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                                   bool is_gm, uint32_t gpid, bool is_bgint,
                                   void (*resfunc)(uint32_t channel, CIntc1* intc1),
                                   bool is_initdone)
{
    std::vector<IntCause*>::iterator itr = m_int_queue.begin ();
    while (itr != m_int_queue.end ()) {
        if ((*itr)->m_int_cause == int_cause) {
            // @@ 暫定対応
            // @@ 同じ発行元からの割込みがキューに存在する場合は何もしない
            // @@ ほんとは良くないのでしっかり実装する
            if ((*itr)->m_eitb != eitb) {
                (*itr)->m_eitb = eitb;
            }
            if ((*itr)->m_is_int_gm != is_gm) {
                (*itr)->m_is_int_gm = is_gm;
            }
            if ((*itr)->m_int_gpid != gpid) {
                (*itr)->m_int_gpid = gpid;
            }
            if ((*itr)->m_is_int_bgint != is_bgint) {
                (*itr)->m_is_int_bgint = is_bgint;
            }
            if ((*itr)->m_int_priority != int_priority) {
                (*itr)->m_int_priority = int_priority;
                // 優先度が変更されたため、割込みキューのソートが必要
                sort (m_int_queue.begin(), m_int_queue.end(), GreaterIntPriority);
                UpdateIntRequested ();
            }
            return INT_E_NOT_RECEIVED;
        }
        ++ itr;
    }

    // @@ 優先度判定はここ？ INTC？ 後で検討
    IntCause* cause = new IntCause (int_priority, int_cause, eitb, is_gm, gpid, is_bgint, resfunc, is_initdone);
    m_int_queue.push_back (cause);
    // 割込みキューのソート
    sort (m_int_queue.begin(), m_int_queue.end(), GreaterIntPriority);
    UpdateIntRequested ();

    return INT_E_OK;
}
#endif // defined(COMP_RUNNER)

/*!
 * @brief  割り込み要求のキャンセル
 */
IntErrorCode FsCtrl::CancelInterrupt (uint32_t int_cause)
{
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelInterrupt: called: cause=0x%x\n", int_cause);
#endif // DEBUG_CORE_INT

    std::vector<IntCause*>::iterator itr = m_int_queue.begin ();
    while (itr != m_int_queue.end ()) {
        if ((*itr)->m_int_cause == int_cause) {
            delete (*itr);
            m_int_queue.erase (itr);
#ifdef DEBUG_CORE_INT
            CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelInterrupt: delete: cause=0x%x\n", int_cause);
#endif // DEBUG_CORE_INT
            UpdateIntRequested ();
            UpdateHtRunable ();
            break;
        }
        ++ itr;
    }
    return INT_E_OK;
}


/*!
 * @brief  割り込み要求のキャンセル (remove all)
 */
IntErrorCode FsCtrl::CancelInterrupt (void)
{
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelInterrupt(all): called\n");
#endif // DEBUG_CORE_INT

    ClearIntReqQueue ();
    UpdateIntRequested ();
    UpdateHtRunable ();
    return INT_E_OK;
}


/*!
 * @brief 割り込みキューをクリア
 * @param void
 * @return void
 */
void FsCtrl::ClearIntReqQueue (void)
{
    std::vector<IntCause*>::iterator itr = m_int_queue.begin ();
    while (itr != m_int_queue.end ()) {
#ifdef DEBUG_CORE_INT
        CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelInterrupt: delete: cause=0x%x\n", (*itr)->m_int_cause);
#endif // DEBUG_CORE_INT
        delete (*itr);
        ++ itr;
    }
    m_int_queue.clear();
    m_is_int_reqed = false;
}


/*!
 * @brief  割込みキューを優先度の降順ソートする際の判断基準
 */
bool FsCtrl::GreaterIntPriority (const IntCause* left, const IntCause* right)
{
    if (left->m_int_priority == right->m_int_priority) {
        // 割込み優先度が等しい場合、チャネル番号に対し降順ソート
        return left->m_int_cause > right->m_int_cause;
    } else {
        // 割込み優先度に対し、降順ソート
        return left->m_int_priority > right->m_int_priority;
    }
}


/*!
 * @brief  FENMIの通知
 */
#if _UNIT_TEST
IntErrorCode FsCtrl_Interface::ReqFenmi (void (*resfunc)(CIntc1* intc1))
#else
IntErrorCode FsCtrl::ReqFenmi (void (*resfunc)(CIntc1* intc1))
#endif
{
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqFenmi: called\n");
#endif // DEBUG_CORE_INT

    if (m_fenmi_cause != NULL) {
#ifdef DEBUG_CORE_INT
        CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqFenmi: error: INT_E_NOT_RECEIVED\n");
#endif // DEBUG_CORE_INT
        return INT_E_NOT_RECEIVED;
    }

#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqFenmi: register\n");
#endif // DEBUG_CORE_INT

    m_fenmi_cause = new FenmiCause (resfunc);
    UpdateIntRequested ();

    return INT_E_OK;
}


#if defined(COMP_RUNNER)

/*!
 * @brief  FENMIの通知
 */
IntErrorCode FsCtrl::ReqFenmi (void (*resfunc)(CIntc1* intc1), bool is_instdone)
{
    if (m_fenmi_cause != NULL) {
        return INT_E_NOT_RECEIVED;
    }

    m_fenmi_cause = new FenmiCause (resfunc, is_instdone);
    UpdateIntRequested ();

    return INT_E_OK;
}


/*!
 * @brief  FEINTの通知
 */
IntErrorCode FsCtrl::ReqFeint (void (*resfunc)(uint32_t channel, CIntc1* intc1), bool is_instdone,
                               uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint)
{
    if (m_feint_cause != NULL) {
        return INT_E_NOT_RECEIVED;
    }

    m_feint_cause = new FeintCause (resfunc, channel, is_gm, gpid, is_bgint, is_instdone);
    UpdateIntRequested ();

    return INT_E_OK;
}

#endif // defined(COMP_RUNNER)


/*!
 * @brief  FENMIのキャンセル
 */
#if _UNIT_TEST
IntErrorCode FsCtrl_Interface::CancelFenmi (void)
#else
IntErrorCode FsCtrl::CancelFenmi (void)
#endif
{
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelFenmi: called\n");
#endif // DEBUG_CORE_INT

    if (m_fenmi_cause  != NULL) {
#ifdef DEBUG_CORE_INT
        CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelFenmi: delete\n");
#endif // DEBUG_CORE_INT
        delete (m_fenmi_cause);
        m_fenmi_cause = NULL;
        UpdateIntRequested ();
        UpdateHtRunable ();
    }
    return INT_E_OK;
}


/*!
 * @brief  FEINTの通知
 */
#if _UNIT_TEST
IntErrorCode FsCtrl_Interface::ReqFeint (void (*resfunc)(uint32_t channel, CIntc1* intc1),
                                         uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint)
#else
IntErrorCode FsCtrl::ReqFeint (void (*resfunc)(uint32_t channel, CIntc1* intc1), uint32_t channel,
bool is_gm, uint32_t gpid, bool is_bgint)
#endif
{
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqFeint: called\n");
#endif // DEBUG_CORE_INT

    if ((m_feint_cause != NULL) || (channel > 15)) {
#ifdef DEBUG_CORE_INT
        CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqFeint: error: INT_E_NOT_RECEIVED\n");
#endif // DEBUG_CORE_INT
        return INT_E_NOT_RECEIVED;
    }

#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]ReqFeint: register: ch=0x%x, gm=%x, gpid=0x%x, bg=%x\n",
                   channel, is_gm, gpid, is_bgint);
#endif // DEBUG_CORE_INT

    m_feint_cause = new FeintCause (resfunc, channel, is_gm, gpid, is_bgint);
    UpdateIntRequested ();

    return INT_E_OK;
}


/*!
 * @brief  FEINTのキャンセル
 */
#if _UNIT_TEST
IntErrorCode FsCtrl_Interface::CancelFeint (void)
#else
IntErrorCode FsCtrl::CancelFeint (void)
#endif
{
#ifdef DEBUG_CORE_INT
    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelFeint: called\n");
#endif // DEBUG_CORE_INT

    if (m_feint_cause != NULL) {
#ifdef DEBUG_CORE_INT
        CfMsg::TPrint (MSG_ERR, m_trace, "[Core]CancelFeint: delete: ch=0x%x\n",
                       m_feint_cause->m_feint_channel);
#endif // DEBUG_CORE_INT
        delete (m_feint_cause);
        m_feint_cause = NULL;
        UpdateIntRequested ();
        UpdateHtRunable ();
    }
    return INT_E_OK;
}


/*!
 * @brief  Notify System error
 *         This function doesn't update DEVDS. If necessary to update DEVDS, use ReqSyserrOperand.
 */
IntErrorCode FsCtrl::ReqOperandSyserrInternal (uint32_t cause,
                                               bool is_caused_by_host, bool is_accepted_by_host)
{
    if (m_lsu->IsEnabledOperandSyserr() == false) {
        return INT_E_NOT_RECEIVED;
    }

    uint32_t exp_cause;
    if (is_caused_by_host) {
        exp_cause = ExpInfo::GetExpCause (EXP_SYSERR_HM);
    } else {
        exp_cause = ExpInfo::GetExpCause (EXP_SYSERR_GM);
    }
    if ((cause >= exp_cause) && (cause < (exp_cause + 0x10))) {
        if (m_syserr_cause_op != NULL) {
#ifdef SUPPORT_CUBESUITE
            return INT_E_NOT_RECEIVED;
#else // SUPPORT_CUBESUITE
            ClearOperandSyserrReqQueue ();
#endif // SUPPORT_CUBESUITE
        }
        m_syserr_cause_op = new SyserrCause (cause, is_caused_by_host, is_accepted_by_host);
        UpdateIntRequested ();
        return INT_E_OK;
    } else {
        // 不正システムエラー例外要因コード
        return INT_E_SYSERR_CODE;
    }
}


/*!
 * @brief  Notify operand type system error from external (Set 1 to DEVDS)
 */
IntErrorCode FsCtrl::ReqOperandSyserr (uint32_t cause, bool is_host, uint32_t gpid)
{
    if (m_lsu->IsEnabledOperandSyserr() == false) {
        return INT_E_NOT_RECEIVED;
    }

    uint32_t exp_cause;
    if (is_host) {
        exp_cause = ExpInfo::GetExpCause (EXP_SYSERR_HM);
    } else {
        exp_cause = ExpInfo::GetExpCause (EXP_SYSERR_GM);
    }
    if ((cause >= exp_cause) && (cause < (exp_cause + 0x10))) {
        m_lsu->SetSyserrCause(is_host, gpid, cause);
        return INT_E_OK;
    } else {
        // Illegal SYSERR cause
        return INT_E_SYSERR_CODE;
    }
}


/*!
 * @brief  Notify error response for operand type system error from external (Set 1 to DEVDS when DECFG.x=1)
 */
IntErrorCode FsCtrl::NoticeOperandErrorResponse (uint32_t cause, bool is_host, uint32_t gpid)
{
    if (m_lsu->IsEnabledOperandSyserr() == false) {
        return INT_E_NOT_RECEIVED;
    }

    switch (cause) {
    case SYSERR_D_GUEST:
        if (m_srf->SrDECFG()->GetEGE () == true) {
            m_lsu->SetSyserrCause(is_host, gpid, cause);
        }
        return INT_E_OK;
    case SYSERR_SAFETY:
        if (m_srf->SrDECFG()->GetEHE () == true) {
            m_lsu->SetSyserrCause(is_host, gpid, cause);
        }
        return INT_E_OK;
    case SYSERR_BSLAVE:
        if (m_srf->SrDECFG()->GetESE () == true) {
            m_lsu->SetSyserrCause(is_host, gpid, cause);
        }
        return INT_E_OK;
    default:
        // Illegal SYSERR cause
        return INT_E_SYSERR_CODE;
    }
}


/*!
 * @brief Notify pseudo Fetch System error
 */
IntErrorCode FsCtrl::ReqPseudoFetchSyserr (uint32_t cause)
{
    uint32_t exp_cause;
    bool is_accepted_host;
    bool is_host = m_pe->IsHostMode ();
    if (is_host) {
        exp_cause = ExpInfo::GetExpCause (EXP_SYSERR_HM);
        is_accepted_host = true;
    } else {
        exp_cause = ExpInfo::GetExpCause (EXP_SYSERR_GM);
        if (m_pe->IsHvEnabled() == false) {
            is_accepted_host = true;
        } else {
            is_accepted_host = (m_srf->SrGMCFG ()->GetGSYSE () == 1);
        }
    }
    if ((cause >= exp_cause) && (cause < (exp_cause + 0x10))) {
        if (m_syserr_cause_pseudo_if != NULL) {
#ifdef SUPPORT_CUBESUITE
            return INT_E_NOT_RECEIVED;
#else // SUPPORT_CUBESUITE
            ClearPseudoFetchSyserrReqQueue ();
#endif // SUPPORT_CUBESUITE
        }
        // SYSERR_IF occurs on current mode always.
        m_syserr_cause_pseudo_if = new SyserrCause (cause, is_host, is_accepted_host);
        UpdateIntRequested ();

        return INT_E_OK;
    } else {
        // 不正システムエラー例外要因コード
        return INT_E_SYSERR_CODE;
    }
}


void FsCtrl::UpdateOperandSyserrEvent (void)
{
    // For G4MH2.0.
    // SYSERR by operand access is controled by system registers.
    // Based on the registers, generate and cancel Syserr requests.
    if (m_lsu->IsEnabledOperandSyserr() == false) {
        return;
    }

    ClearOperandSyserrReqQueue ();
    UpdateIntRequested ();

    SysErrCause syserr_cause;
    bool is_caused_by_host;
    bool is_sent_to_host;
    // Request only operand syserr which is caused by current mode.
    if (  ((m_pe->IsHvEnabled() == true) && (m_nc_srf->SrPSWH()->GetGM () == 0))
       || (m_pe->IsHvEnabled() == false)) {
        // host mode or conventional mode
        syserr_cause = m_lsu->GetSyserrCause (true, 0);
        is_caused_by_host = true;
        is_sent_to_host = true;
    } else {
        // guest mode
        syserr_cause = m_lsu->GetSyserrCause (false, m_srf->SrPSWH()->GetGPID ());
        is_caused_by_host = false;
        is_sent_to_host = (m_srf->SrGMCFG ()->GetGSYSE () == 1);
    }
    switch (syserr_cause) {
    case SYSERR_D_GUEST:
        ReqOperandSyserrInternal (syserr_cause, is_caused_by_host, is_sent_to_host);
        return;
    case SYSERR_SAFETY:
        ReqOperandSyserrInternal (syserr_cause, is_caused_by_host, is_sent_to_host);
        return;
    case SYSERR_BSLAVE:
        ReqOperandSyserrInternal (syserr_cause, is_caused_by_host, is_sent_to_host);
        return;
    default: //SYSERR_NONE, SYSERR_NOP
        break;
    }
}


// Clear all operand syserr (Clear DEVDS)
IntErrorCode FsCtrl::CancelOperandSyserr (void)
{
    m_lsu->InitSyserrEvent ();
    UpdateOperandSyserrEvent();
    UpdateIntRequested ();
    return INT_E_OK;
}


// Clear an operand syserr (Clear 1bit in DEVDS)
IntErrorCode FsCtrl::CancelOperandSyserr (bool is_host, uint32_t gpid)
{
    m_lsu->SetSyserrCause (is_host, gpid, SYSERR_NONE);
    UpdateIntRequested ();
    return INT_E_OK;
}


IntErrorCode FsCtrl::CancelPseudoFetchSyserr (void)
{
    if (m_syserr_cause_pseudo_if != NULL) {
        ClearPseudoFetchSyserrReqQueue ();
        UpdateIntRequested ();
    }
    return INT_E_OK;
}


/*!
 * @brief FENMIキューをクリア
 * @param void
 * @return void
 */
void FsCtrl::ClearFenmiReqQueue (void)
{
    if (m_fenmi_cause != NULL) {
        delete (m_fenmi_cause);
    }
    m_fenmi_cause = NULL;
    m_is_fenmi_reqed = false;
}


/*!
 * @brief FEINTキューをクリア
 * @param void
 * @return void
 */
void FsCtrl::ClearFeintReqQueue (void)
{
    if (m_feint_cause != NULL) {
        delete (m_feint_cause);
    }
    m_feint_cause = NULL;
    m_is_feint_reqed = false;
}


/*!
 * @brief SYSERRキューをクリア
 * @param void
 * @return void
 */
void FsCtrl::ClearOperandSyserrReqQueue (void)
{
    if (m_syserr_cause_op != NULL) {
        delete (m_syserr_cause_op);
    }
    m_syserr_cause_op = NULL;
}


/*!
 * @brief SYSERRキューをクリア
 * @param void
 * @return void
 */
void FsCtrl::ClearPseudoFetchSyserrReqQueue (void)
{
    if (m_syserr_cause_pseudo_if != NULL) {
        delete (m_syserr_cause_pseudo_if);
    }
    m_syserr_cause_pseudo_if = NULL;
}


IntErrorCode FsCtrl::ReqRlb (uint32_t peid)
{
    m_req_rlb = true;
    UpdateIntRequested ();
    UpdateHtRunable ();
    return INT_E_OK;
}


IntErrorCode FsCtrl::CancelRlb (uint32_t peid)
{
    m_req_rlb = false;
    UpdateIntRequested ();
    UpdateHtRunable ();
    return INT_E_OK;
}


IntErrorCode FsCtrl::ReqDbnmi (void)
{
    if (m_req_dbnmi) {
        return INT_E_NOT_RECEIVED;
    }
    m_req_dbnmi = true;
    UpdateIntRequested ();

    return INT_E_OK;
}


IntErrorCode FsCtrl::CancelDbnmi (void)
{
    if (!m_req_dbnmi) {
        return INT_E_NOT_RECEIVED;
    }
    m_req_dbnmi = false;
    UpdateIntRequested ();
    UpdateHtRunable ();
    return INT_E_OK;
}


IntErrorCode FsCtrl::ReqDbint (uint32_t channel)
{
    if ((m_req_dbint) || (channel > 15)) {
        return INT_E_NOT_RECEIVED;
    }
    m_req_dbint = true;
    m_channel_dbint = channel;
    UpdateIntRequested ();

    return INT_E_OK;
}


IntErrorCode FsCtrl::CancelDbint (void)
{
    if (!m_req_dbint) {
        return INT_E_NOT_RECEIVED;
    }
    m_req_dbint = false;
    m_channel_dbint = 0;
    UpdateIntRequested ();
    UpdateHtRunable ();
    return INT_E_OK;
}


IntErrorCode FsCtrl::ReqRmint (void)
{
    return INT_E_NOT_RECEIVED;
}


IntErrorCode FsCtrl::CancelRmint (void)
{
    return INT_E_NOT_RECEIVED;
}


IntErrorCode FsCtrl::ReqAllInterruptDisable (void)
{
    if (m_all_interrupt_disable == true)
        return INT_E_NOT_RECEIVED;
    m_all_interrupt_disable = true;
    return INT_E_OK;
}


IntErrorCode FsCtrl::CancelAllInterruptDisable (void)
{
    if (m_all_interrupt_disable == false)
        return INT_E_NOT_RECEIVED;
    m_all_interrupt_disable = false;
    return INT_E_OK;
}


bool FsCtrl::IsEnableDebugMonitor (void) const
{
    // FENMI/SYSERR/FEINT/EIINT are masked on following.
    // - DIR0.DM=1
    return  m_srf->SrDIR0 ()->GetDM () == 1;
}


/*!
 * @brief  割り込み受付許可状態の更新
 */
void FsCtrl::UpdateIntAcceptable (void)
{
    // If "Int" is EIINT, HMPSW must be checked.
    SRegHMPSW* hmpsw = m_nc_srf->SrHMPSW ();
    m_is_int_acceptable = (hmpsw->GetID () == 0 && hmpsw->GetNP () == 0 && IsEnableDebugMonitor() == false);

    // Even if m_is_int_acceptable isn't updated,
    // need to call UpdateIntStatus to update IMSR
    UpdateIntStatus ();
}


/*!
 * @brief  割り込み受付許可状態の更新
 */
void FsCtrl::UpdateGmIntAcceptable (void)
{
    // If "Int" is GMEIINT, GMPSW.ID/NP must be checked.
    SRegPSWH* pswh = m_nc_srf->SrPSWH ();
    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
    m_is_gmint_acceptable = (pswh->GetGM () == 1 && gmpsw->GetID () == 0 && gmpsw->GetNP () == 0
                           && IsEnableDebugMonitor() == false);

    // Even if m_is_gmint_acceptable isn't updated,
    // need to call UpdateIntStatus to update IMSR
    UpdateIntStatus ();
}


/*!
 * @brief  割り込み受付許可状態の更新
 */
void FsCtrl::UpdateBgIntAcceptable (void)
{
    // If "Int" is BGEIINT, HMPSW.ID/NP and GMPSW.ID/NP must be checked.
    SRegPSWH* pswh = m_nc_srf->SrPSWH ();
    SRegHMPSW* hmpsw = m_nc_srf->SrHMPSW ();
    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
    m_is_bgint_acceptable = (pswh->GetGM () == 1 && hmpsw->GetID () == 0 && hmpsw->GetNP () == 0
                           && gmpsw->GetID () == 0 && gmpsw->GetNP () == 0
                           && IsEnableDebugMonitor() == false);

    // Even if m_is_bgint_acceptable isn't updated,
    // need to call UpdateIntStatus to update IMSR
    UpdateIntStatus ();
}


/*!
 * @brief  FEINT受付許可状態の更新
 */
void FsCtrl::UpdateFeintAcceptable (void)
{
    SRegHMPSW* psw = m_nc_srf->SrHMPSW ();
    m_is_feint_acceptable = (psw->GetNP () == 0 && IsEnableDebugMonitor() == false);

    // Even if m_is_feint_acceptable isn't updated,
    // need to call UpdateIntStatus to update IMSR
    UpdateIntStatus ();
}


/*!
 * @brief  Update GMFEINT acceptance
 */
void FsCtrl::UpdateGmFeintAcceptable (void)
{
    SRegPSWH* pswh = m_nc_srf->SrPSWH ();
    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
    m_is_gmfeint_acceptable = ((pswh->GetGM () == 1) && (gmpsw->GetNP () == 0) && (IsEnableDebugMonitor() == false));

    // Even if m_is_gmfeint_acceptable isn't updated,
    // need to call UpdateIntStatus to update IMSR
    UpdateIntStatus ();
}


/*!
 * @brief  Update BGFEINT acceptance
 */
void FsCtrl::UpdateBgFeintAcceptable (void)
{
    SRegPSWH* pswh = m_nc_srf->SrPSWH ();
    SRegHMPSW* hmpsw = m_nc_srf->SrHMPSW ();
    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
    m_is_bgfeint_acceptable = ((pswh->GetGM () == 1)
                               && (hmpsw->GetID () == 0) && (hmpsw->GetNP () == 0)
                               && (gmpsw->GetNP () == 0)
                               && (IsEnableDebugMonitor() == false));

    // Even if m_is_bgfeint_acceptable isn't updated,
    // need to call UpdateIntStatus to update IMSR
    UpdateIntStatus ();
}


/*!
 * @brief  Update operand SYSERR acceptance
 */
void FsCtrl::UpdateOperandSyserrAcceptable (void)
{
    SRegHMPSW* hmpsw = m_nc_srf->SrHMPSW ();
    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
    m_is_host_op_syserr_acceptable = ((hmpsw->GetNP () == 0) && (IsEnableDebugMonitor() == false));
    m_is_guest_op_syserr_acceptable = ((gmpsw->GetNP () == 0) && (IsEnableDebugMonitor() == false));

    UpdateIntStatus ();
}


/*!
 * @brief  Update acceptable flag for DBINT/RMINT
 */
void FsCtrl::UpdateDir0DmStatus (void)
{
    m_is_dir0dm_0 = (m_srf->SrDIR0 ()->GetDM () == 0);

    // Update interruption status when the acceptance is changed.
    UpdateIntStatus ();
}


void FsCtrl::ClearImsrAndIcsr (void)
{
    // Although ICSR is invalid in HVE=1, update always to have latest info internally.
    m_nc_srf->SrICSR()->SetPMEI (0);
    m_nc_srf->SrIMSR ()->Reset ();
}


void FsCtrl::UpdateImsrByFeint (ExpCode feint_type, FeintMaskType mask_type)
{
    // Update IMSR.HFNP, FNP by FEINT
    // Update IMSR.FNP by GMFEINT
    // Update IMSR.HENP, HEID, FNP by BGFEINT
    SRegIMSR* imsr = m_nc_srf->SrIMSR ();
    bool is_guest_mode = (m_pe->IsHostMode () == false) && (m_pe->IsHvEnabled () == true);
    if (feint_type == EXP_FEINT) {
        if (mask_type == FEINT_MASK_HMPSWNP) {
            if (is_guest_mode) {
                imsr->SetHFNP (1);
            } else {
                imsr->SetFNP (1);
            }
        } else {
            // Masked by DIR0.DM
        }
    } else if (feint_type == EXP_GMFEINT) {
        if (mask_type == FEINT_MASK_GMPSWNP) {
            if (is_guest_mode) {
                imsr->SetFNP (1);
            } else {
                // Don't come here
                // GMFEINT doesn't occur except guest mode.
            }
        } else {
            // Masked by DIR0.DM or PSWH.GM
        }
    } else { // BGFEINT
        if (mask_type == FEINT_MASK_HMPSWID) {
            if (is_guest_mode) {
                imsr->SetHEID (1);
            } else {
                // Don't come here.
                // BGFEINT doesn't occur except guest mode.
            }
        } else if (mask_type == FEINT_MASK_HMPSWNP) {
            if (is_guest_mode) {
                imsr->SetHENP (1);
            } else {
                // Don't come here.
                // BGFEINT doesn't occur except guest mode.
            }
        } else if (mask_type == FEINT_MASK_GMPSWNP) {
            if (is_guest_mode) {
                imsr->SetFNP (1);
            } else {
                // Don't come here.
                // BGFEINT doesn't occur except guest mode.
            }
        } else {
            // Masked by DIR0.DM or PSWH.GM
        }
    }
}


void FsCtrl::UpdateImsrAndIcsrByEiint (ExpCode eiint_type, EiintMaskType mask_type)
{
    SRegIMSR* imsr = m_nc_srf->SrIMSR ();
    bool is_guest_mode = (m_pe->IsHostMode () == false) && (m_pe->IsHvEnabled () == true);
    if (eiint_type == EXP_EIINT) {
        // Update IMSR.HENP, HEID, HEPLM, HEEIM, ENP, EID, EPLM, EEIM by EIINT
        if (mask_type == EIINT_MASK_HMPSWNP) {
            if (is_guest_mode) {
                imsr->SetHENP (1);
            } else {
                imsr->SetENP (1);
            }
        } else if (mask_type == EIINT_MASK_HMPSWID) {
            if (is_guest_mode) {
                imsr->SetHEID (1);
            } else {
                imsr->SetEID (1);
            }
        } else if (mask_type == EIINT_MASK_PLMR) {
            if (is_guest_mode) {
                imsr->SetHEPLM (1);
            } else {
                // Although ICSR is invalid in HVE=1, update always to have latest info internally.
                m_nc_srf->SrICSR()->SetPMEI(1);
                imsr->SetEPLM (1);
            }
        } else if ((mask_type == EIINT_MASK_ISPR) || (mask_type == EIINT_MASK_PSWEIMASK)) {
            if (is_guest_mode) {
                imsr->SetHEEIM (1);
            } else {
                imsr->SetEEIM (1);
            }
        } else {
            // Masked by DIR0.DM
        }
    } else if (eiint_type == EXP_GMEIINT) {
        // Update IMSR.ENP, EID, EPLM, EEIM by GMEIINT
        if (mask_type == EIINT_MASK_GMPSWNP) {
            if (is_guest_mode) {
                imsr->SetENP (1);
            } else {
                // Don't come here
                // GMEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_GMPSWID) {
            if (is_guest_mode) {
                imsr->SetEID (1);
            } else {
                // Don't come here
                // GMEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_PLMR) {
            if (is_guest_mode) {
                // Although ICSR is invalid in HVE=1, update always to have latest info internally.
                m_nc_srf->SrICSR()->SetPMEI(1);
                imsr->SetEPLM (1);
            } else {
                // Don't come here
                // GMEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_PSWEIMASK) {
            if (is_guest_mode) {
                imsr->SetEEIM (1);
            } else {
                // Don't come here
                // GMEIINT doesn't occur except guest mode.
            }
        } else {
            // Masked by DIR0.DM or PSWH.GM
        }
    } else { // BGEIINT
        // Update IMSR.HENP, HEID, ENP, EID, EPLM, EEIM by BGEIINT
        if (mask_type == EIINT_MASK_GMPSWNP) {
            if (is_guest_mode) {
                imsr->SetENP (1);
            } else {
                // Don't come here.
                // BGEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_HMPSWNP) {
            if (is_guest_mode) {
                imsr->SetHENP (1);
            } else {
                // Don't come here.
                // BGEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_GMPSWID) {
            if (is_guest_mode) {
                imsr->SetEID (1);
            } else {
                // Don't come here.
                // BGEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_HMPSWID) {
            if (is_guest_mode) {
                imsr->SetHEID (1);
            } else {
                // Don't come here.
                // BGEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_PLMR) {
            if (is_guest_mode) {
                // Although ICSR is invalid in HVE=1, update always to have latest info internally.
                m_nc_srf->SrICSR()->SetPMEI(1);
                imsr->SetEPLM (1);
            } else {
                // Don't come here.
                // BGEIINT doesn't occur except guest mode.
            }
        } else if (mask_type == EIINT_MASK_PSWEIMASK) {
            if (is_guest_mode) {
                imsr->SetEEIM (1);
            } else {
                // Don't come here.
                // BGEIINT doesn't occur except guest mode.
            }
        } else {
            // Masked by DIR0.DM or PSWH.GM
        }
    }

}



/*!
 * @brief  割り込み受付状態の更新
 */
void FsCtrl::UpdateIntRequested (void)
{
    m_is_fenmi_reqed  = (m_fenmi_cause != NULL) ? true : false;
    m_is_syserr_reqed_op = (m_syserr_cause_op != NULL) ? true : false;
    m_is_syserr_reqed_pseudo_if = (m_syserr_cause_pseudo_if != NULL) ? true : false;

#ifdef MASK_INT_BY_GPID
    // Special feature for the FROG (ref-rvc #89815)
    //     CForest doesn't accept (mask) followings.
    //     - GMxxINT with different GPID on guest mode
    //     - BGxxINT with same GPID on guest mode
    //     - Note: GMxxINT/BGxxINT on host/conventional mode is masked by ISA spec. Don't mask here.
    // BackGround of this feature
    //     By FROG, GMxxINT with different GPID and BGxxINT with same GPID are requested some cases.
    //     Although they must not request correctlly, it is difficult to control by FROG.
    //     So, CForest masks them internally.
    //     Although this function is needed only for FROG,
    //     because FROG generated patterns are run on CForest and RTLsim,
    //     this dunction is enabled in CForest standalone and CompRunner and CS+, SC-HEAP.
    // Additional information for the implementation related this feature
    // (1) Content of the registers for masked information such as IMSR
    //     When the requests are masked by this feature, CForest treats it as no-request.
    //     For example, IMSR doesn't have the information for the request when it is masked.
    // (2) Don't release HALT status when this feature masks the reuqest (and there is no other req)
    bool is_feint_reqed  = ((m_feint_cause != NULL) && (IsMaskGuestFEINT () == false)) ? true : false;
    bool is_int_reqed = ((m_int_queue.empty () == false) && (IsMaskGuestEIINT () == false));
    if (m_core_type == CORE_G4KH) {
        is_int_reqed = is_int_reqed || m_srf->SrFPEC()->GetFPIVD();
    }
#else // #ifdef MASK_INT_BY_GPID
    bool is_feint_reqed  = (m_feint_cause != NULL) ? true : false;
    bool is_int_reqed = (m_int_queue.empty () == false);
#endif // #ifdef MASK_INT_BY_GPID
    m_is_feint_reqed = is_feint_reqed;
    m_is_int_reqed = is_int_reqed;

    bool is_any_async_excp  = (is_int_reqed
                            || m_is_fenmi_reqed
                            || m_is_syserr_reqed_op
                            // || m_is_syserr_reqed_pseudo_if // not async.
                            || is_feint_reqed
#ifdef ENABLE_DBG_BREAK
                            || m_req_rlb
                            || m_req_dbnmi
                            || m_req_dbint
#endif // ENABLE_DBG_BREAK
                            );
    SetAnyAsyncException (is_any_async_excp);

    if (IsAnyAsyncException ()) {
        // 中断型例外が要求されたらHALT解除
        SetHaltState (false);
        SetSnooze (false);
    }

    UpdateIntStatus ();
}


#ifdef MASK_INT_BY_GPID
bool FsCtrl::IsMaskGuestEIINT (void) const
{
    // Becasue INTC or FROG has delay to switch intreq, some illegal cases from ISA spec are masked.
    // GMxxINT on guest mode with different GPID is masked here (Correctlly, it is switched to BGEIINT by INTC soon.)
    // BGxxINT on guest mode with same GPID is masked here. (Correctlly, it is switched to GMEIINT by INTC soon.)
    // GMxxINT/BGxxINT on host/conventional mode is masked another part. Ignore this case here.
    bool is_masked = false;
    bool is_int_reqed = (m_int_queue.empty () == false);
    if (is_int_reqed && m_pe->IsGuestMode()) {
        ExpCode eiint_type = m_int_queue.back()->GetEiintType ();
        bool is_gpid_matched = m_pe->IsGuestModeWithGpid(m_int_queue.back()->GetGPID ());
        if ((eiint_type == EXP_GMEIINT) && (is_gpid_matched == false)) {
            is_masked = true; // GMEIINT with different GPID is masked.
        }
        if ((eiint_type == EXP_BGEIINT) && (is_gpid_matched == true)) {
            is_masked = true; // BGEIINT with same GPID is masked.
        }
    }
    return is_masked;
}


bool FsCtrl::IsMaskGuestFEINT (void) const
{
    // Becasue INTC or FROG has delay to switch intreq, some illegal cases from ISA spec are masked.
    // GMxxINT on guest mode with different GPID is masked here (Correctlly, it is switched to BGEIINT by INTC soon.)
    // BGxxINT on guest mode with same GPID is masked here. (Correctlly, it is switched to GMEIINT by INTC soon.)
    // GMxxINT/BGxxINT on host/conventional mode is masked another part. Ignore this case here.
    bool is_masked = false;
    bool is_feint_reqed  = (m_feint_cause != NULL) ? true : false;
    if (is_feint_reqed && m_pe->IsGuestMode()) {
        ExpCode feint_type = m_feint_cause->GetFeintType ();
        bool is_gpid_matched = m_pe->IsGuestModeWithGpid(m_feint_cause->GetGPID());
        if ((feint_type == EXP_GMFEINT) && (is_gpid_matched == false)) {
            is_masked = true; // GMFEINT with different GPID is masked.
        }
        if ((feint_type == EXP_BGFEINT) && (is_gpid_matched == true)) {
            is_masked = true; // BGFEINT with same GPID is masked.
        }
    }
    return is_masked;
}
#endif // MASK_INT_BY_GPID


/*!
 * @brief  受け付ける割り込みを更新
 * 割り込みの許可状態、受付状態から受け付ける割り込みを更新
 */
void FsCtrl::UpdateIntStatus (void)
{
    if (m_core_type != CORE_G4KH) {
        // EIINT/GMEIINT/BGEIINT checking:
        // (0) Masked by PSWH.GM for GMEIINT/BGEIINT on G4MH2.x
        // (1) Masked by ISPR/EIMASK
        // (2) Masked by PLMR
        // (3) Masked by HMPSW.ID and GMPSW.ID
        // (4) Masked by HMPSW.NP and GMPSW.NP
        // (5) Masked by DIR0.DM
        // For BGEIINT, HMPSW.ID/NP and GMPSW.ID/NP are checked in parallel.
        // (3) Masked by HMPSW.ID (4) HMPSW.NP
        // (3) Masked by GMPSW.ID (4) GMPSW.NP
        // (5) Masked by DIR0.DM

        ClearImsrAndIcsr ();
        // Check EIINT, GMEIINT, BGEIINT
        IntType eiint_factor = INT_TYPE_NONE;
        if (IsIntRequested() == true) {
            // ここに来るならINTは要求されている
            // Check only first priority EIINT (including EIINT/GMEIINT/BGEIINT)
            uint32_t pri = GetEiintPriority();
            ExpCode eiint_type = m_int_queue.back()->GetEiintType ();
            if (eiint_type == EXP_EIINT) {
                bool epl = m_nc_srf->SrHMINTCFG()->GetEPL();
                uint32_t pri_for_ispr_check = pri;
                if ((epl == 0) && (pri >= 16)) {
                    // For EIINT, Check HMINTCFG.EPL.
                    // EPL=0: Pri15 is used for ISPR checking for pri16 and higher
                    //        Org priority (pri=0-63) is used for PLMR checking.
                    pri_for_ispr_check = 15;
                }

                // 優先度別マスクによる最終的な割り込み要因選択
                if ((epl == 0) && (IsIsprMasked (pri_for_ispr_check) == true)) {
                    // Masked by ISPR
                    eiint_factor = INT_TYPE_NONE;
                    UpdateImsrAndIcsrByEiint (EXP_EIINT, EIINT_MASK_ISPR);
#ifdef DEBUG_CORE_INT
                    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]UpdateIntStatus: EIINT is masked by ISPR\n");
#endif // DEBUG_CORE_INT
                } else if ((epl == 1) && (IsHmEimaskMasked (pri) == true)) {
                    // Masked by HMPSW.EIMASK
                    eiint_factor = INT_TYPE_NONE;
                    UpdateImsrAndIcsrByEiint (EXP_EIINT, EIINT_MASK_PSWEIMASK);
#ifdef DEBUG_CORE_INT
                    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]UpdateIntStatus: EIINT is masked by HMPSW.EIMASK\n");
#endif // DEBUG_CORE_INT
                } else if (IsHmPlmrMasked (pri) == true) {
                    // Masked by HMPLMR
                    UpdateImsrAndIcsrByEiint (EXP_EIINT, EIINT_MASK_PLMR);
                    eiint_factor = INT_TYPE_NONE;
#ifdef DEBUG_CORE_INT
                    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]UpdateIntStatus: EIINT is masked by HMPLMR\n");
#endif // DEBUG_CORE_INT
                } else if (IsIntAcceptable() == true) {
                    // EIINT is acceptable
                    eiint_factor = INT_TYPE_HOST;
#ifdef DEBUG_CORE_INT
                    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]UpdateIntStatus: EIINT is not masked\n");
#endif // DEBUG_CORE_INT
                } else {
                    eiint_factor = INT_TYPE_NONE;
                    SRegHMPSW* hmpsw = m_nc_srf->SrHMPSW ();
                    if (hmpsw->GetID () == 1) {
                        // Masked by HMPSW.ID
                        UpdateImsrAndIcsrByEiint (EXP_EIINT, EIINT_MASK_HMPSWID);
                    } else if (hmpsw->GetNP () == 1) {
                        // Masked by HMPSW.NP
                        UpdateImsrAndIcsrByEiint (EXP_EIINT, EIINT_MASK_HMPSWNP);
                    } else {
                        // Masked by DIR0.DM
                        // No need to set IMSR.
                    }
#ifdef DEBUG_CORE_INT
                    CfMsg::TPrint (MSG_ERR, m_trace, "[Core]UpdateIntStatus: EIINT is masked by HMPSW.ID=%x, NP=%x, DIR0.DM=%x\n",
                                   hmpsw->GetID (), hmpsw->GetNP (), m_srf->SrDIR0 ()->GetDM ());
#endif // DEBUG_CORE_INT
                }
            } else if (eiint_type == EXP_GMEIINT) {
                // 優先度別マスクによる最終的な割り込み要因選択
                if (m_nc_srf->SrPSWH ()->GetGM () == 0) {
                    // Masked by PSWH.GM
                    eiint_factor = INT_TYPE_NONE;
                    // No need to set IMSR
                } else if (IsGmEimaskMasked (pri) == true) {
                    // Masked by GMPSW.EIMASK
                    eiint_factor = INT_TYPE_NONE;
                    UpdateImsrAndIcsrByEiint (EXP_GMEIINT, EIINT_MASK_PSWEIMASK);
                } else if (IsGmPlmrMasked (pri) == true) {
                    // Masked by GMPLMR
                    eiint_factor = INT_TYPE_NONE;
                    UpdateImsrAndIcsrByEiint (EXP_GMEIINT, EIINT_MASK_PLMR);
                } else if (IsGmIntAcceptable() == true) {
                    // GMEIINT is acceptable
                    eiint_factor = INT_TYPE_GM;
                } else {
                    eiint_factor = INT_TYPE_NONE;
                    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
                    if (gmpsw->GetID () == 1) {
                        // Masked by GMPSW.ID
                        UpdateImsrAndIcsrByEiint (EXP_GMEIINT, EIINT_MASK_GMPSWID);
                    } else if (gmpsw->GetNP () == 1) {
                        // Masked by GMPSW.NP
                        UpdateImsrAndIcsrByEiint (EXP_GMEIINT, EIINT_MASK_GMPSWNP);
                    } else {
                        // Masked by DIR0.DM
                        // No need to set IMSR.
                    }
                }
            } else { //(eiint_type == EXP_BGEIINT) {
                // 優先度別マスクによる最終的な割り込み要因選択
                if (m_nc_srf->SrPSWH ()->GetGM () == 0) {
                    // Masked by PSWH.GM
                    eiint_factor = INT_TYPE_NONE;
                    // No need to set IMSR
                } else if (IsGmEimaskMasked (pri) == true) {
                    // Masked by GMPSW.EIMASK
                    eiint_factor = INT_TYPE_NONE;
                    UpdateImsrAndIcsrByEiint (EXP_BGEIINT, EIINT_MASK_PSWEIMASK);
                } else if (IsGmPlmrMasked (pri) == true) {
                    // Masked by GMPLMR
                    eiint_factor = INT_TYPE_NONE;
                    UpdateImsrAndIcsrByEiint (EXP_BGEIINT, EIINT_MASK_PLMR);
                } else if (IsBgIntAcceptable() == true) {
                    // BGEIINT is acceptable
                    eiint_factor = INT_TYPE_BG;
                } else {
                    eiint_factor = INT_TYPE_NONE;
                    SRegHMPSW* hmpsw = m_nc_srf->SrHMPSW ();
                    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
                    // (3) Masked by HMPSW.ID (4) HMPSW.NP
                    if (hmpsw->GetID () == 1) {
                        // Masked by HMPSW.ID
                        UpdateImsrAndIcsrByEiint (EXP_BGEIINT, EIINT_MASK_HMPSWID);
                    } else if (hmpsw->GetNP () == 1) {
                        // Masked by HMPSW.NP
                        UpdateImsrAndIcsrByEiint (EXP_BGEIINT, EIINT_MASK_HMPSWNP);
                    }

                    // (3) Masked by GMPSW.ID (4) GMPSW.NP
                    if (gmpsw->GetID () == 1) {
                        // Masked by GMPSW.ID
                        UpdateImsrAndIcsrByEiint (EXP_BGEIINT, EIINT_MASK_GMPSWID);
                    } else if (gmpsw->GetNP () == 1) {
                        // Masked by GMPSW.NP
                        UpdateImsrAndIcsrByEiint (EXP_BGEIINT, EIINT_MASK_GMPSWNP);
                    }

                    // (5) Masked by DIR0.DM
                    if (   (hmpsw->GetID () == 0)
                        && (gmpsw->GetID () == 0)
                        && (hmpsw->GetNP () == 0)
                        && (gmpsw->GetNP () == 0)) {
                        // Masked by DIR0.DM
                        // No need to set IMSR.
                    }
                }
            }
        } else {
            eiint_factor = INT_TYPE_NONE;
        }

        IntFactorType feint_factor = INT_TYPE_NONE;
        if (IsFeintRequested() == true) {
            ExpCode feint_type = m_feint_cause->GetFeintType ();
            if (feint_type == EXP_FEINT) {
                if (IsFeintAcceptable() == true) {
                    // FEINT is acceptable
                    feint_factor = INT_TYPE_HOST;
                } else {
                    // FEINT is masked
                    if (m_nc_srf->SrHMPSW ()->GetNP () == 1) {
                        // Masked by PSW.NP
                        UpdateImsrByFeint (EXP_FEINT, FEINT_MASK_HMPSWNP);
                    } else {
                        // Masked by DIR0.DM
                        // No need to set IMSR
                    }
                }
            } else if (feint_type == EXP_GMFEINT) {
                if (IsGmFeintAcceptable() == true) {
                    // GMFEINT is acceptable
                    feint_factor = INT_TYPE_GM;
                } else {
                    // GMFEINT is masked
                    if (m_nc_srf->SrPSWH ()->GetGM () == 0) {
                        // Masked by PSWH.GM
                        // No need to set IMSR
                    } else if (m_nc_srf->SrGMPSW ()->GetNP () == 1) {
                        // Masked by GMPSW.NP
                        UpdateImsrByFeint (EXP_GMFEINT, FEINT_MASK_GMPSWNP);
                    } else {
                        // Masked by DIR0.DM
                        // No need to set IMSR
                    }
                }
            } else { // EXP_BGFEINT
                if (IsBgFeintAcceptable() == true) {
                    // BGFEINT is acceptable
                    feint_factor = INT_TYPE_BG;
                } else {
                    // BGFEINT is masked
                    SRegHMPSW* hmpsw = m_nc_srf->SrHMPSW ();
                    SRegGMPSW* gmpsw = m_nc_srf->SrGMPSW ();
                    if (m_nc_srf->SrPSWH ()->GetGM () == 0) {
                        // Masked by PSWH.GM
                        // No need to set IMSR
                    } else {
                        // HMPSW.ID/NP and GMPSW.NP is checked in parallel.
                        if (hmpsw->GetID () == 1) {
                            // Masked by HMPSW.ID
                            UpdateImsrByFeint (EXP_BGFEINT, FEINT_MASK_HMPSWID);
                        } else if (hmpsw->GetNP () == 1) {
                            // Masked by HMPSW.NP
                            UpdateImsrByFeint (EXP_BGFEINT, FEINT_MASK_HMPSWNP);
                        }
                        if (gmpsw->GetNP () == 1) {
                            // Masked by GMPSW.NP
                            UpdateImsrByFeint (EXP_BGFEINT, FEINT_MASK_GMPSWNP);
                        }

                        // When BGFEINT is masked by DIR0.DM,
                        // No need to set IMSR
                    }
                }
            }
        }

        m_selected_factor = INT_FAC_NONE;

        // To cause SYSERR by fetch by CS+/SC-HEAP.
        // Becasue SYSERR (ECC or etc) isn't supported, to cause SYSERR fetch on user soft, this function is necessary.
        if (IsPseudoFetchSyserrRequested() == true) { // SYSERR caused by fetch access
            m_selected_factor = INT_FAC_PSEUDO_SYSERR_IF;
        }


        if (eiint_factor == INT_TYPE_GM) {
            m_selected_factor = INT_FAC_INT;
        }


        if ((IsOperandSyserrRequested() == true) && (IsGuestOperandSyserrAcceptable () == true)) { // SYSERR caused by Guest
            if (m_syserr_cause_op->m_is_accepted_by_host == false) {
                m_selected_factor = INT_FAC_SYSERR_OP;
            }
        }


        if (feint_factor == INT_TYPE_GM) {
             m_selected_factor = INT_FAC_FEINT;
        }

        if (eiint_factor == INT_TYPE_BG) {
            m_selected_factor = INT_FAC_INT;
        }

        if (feint_factor == INT_TYPE_BG) {
             m_selected_factor = INT_FAC_FEINT;
        }

        if (eiint_factor == INT_TYPE_HOST) {
            m_selected_factor = INT_FAC_INT;
        }

        if ((IsOperandSyserrRequested() == true) && (IsHostOperandSyserrAcceptable () == true)) { // SYSERR caused by Host or accepted by host
            if (m_syserr_cause_op->m_is_accepted_by_host == true) {
                m_selected_factor = INT_FAC_SYSERR_OP;
            }
        }

        if (feint_factor == INT_TYPE_HOST) {
             m_selected_factor = INT_FAC_FEINT;
        }
    } else {
        // EIINT and FPI checking in G4KH spec:
        //(1) Masked by ISPR
        //(2) Masked by PMR
        //(3) Compare priority of EIINT and FPI.

        IntFactorType fpi_factor = INT_FAC_NONE;
        IntFactorType eiint_factor = INT_FAC_NONE;
        uint32_t fpi_pri = GetFpiPriority();
        if (m_srf->SrFPEC()->GetFPIVD() == true) {
            if (IsIsprMasked (fpi_pri) == true) {
                // FPI checking: (1) Masked by ISPR
            } else if (IsHmPlmrMasked (fpi_pri) == true) {
                // FPI checking: (2) Masked by PLMR
                m_srf->SrICSR()->SetPMFP(1);
            } else {
                // FPI is not masked by ISPR and PLMR.
                m_srf->SrICSR()->SetPMFP(0);
                fpi_factor = INT_FAC_FPI;
            }
        } else {
            // FPI is not occurred.
            m_srf->SrICSR()->SetPMFP(0);
        }
        uint32_t eiint_pri; // No need to init.
        if (m_int_queue.empty() == false) {
            eiint_pri = GetEiintPriority();
            if (IsIsprMasked (eiint_pri) == true) {
                // EIINT checking: (1) Masked by ISPR
            } else if (IsHmPlmrMasked (eiint_pri) == true) {
                // EIINT checking: (2) Masked by PMR
                m_srf->SrICSR()->SetPMEI(1);
            } else {
                // EIINT is not masked by ISPR and PMR.
                m_srf->SrICSR()->SetPMEI(0);
                eiint_factor = INT_FAC_INT;
            }
        } else {
            // EIINT is not occurred.
            m_srf->SrICSR()->SetPMEI(0);
            eiint_pri = 0; // No need actually. But init for compiler warning.
        }

        if ( (IsIntRequested() == true) && (IsIntAcceptable() == true) ) {
            // EIINT and FPI checking: (3) Compare priority of EIINT and FPI.
            // * Note: FPI priority = EIINT priority => FPI is processed by CPU.
            if ((fpi_factor == INT_FAC_FPI) && (eiint_factor == INT_FAC_INT)) {
                if (eiint_pri >= fpi_pri) {
                    m_selected_factor = INT_FAC_FPI;
                } else {
                    m_selected_factor = INT_FAC_INT;
                }
            } else if (fpi_factor == INT_FAC_FPI) {
                m_selected_factor = INT_FAC_FPI;
            } else if (eiint_factor == INT_FAC_INT) {
                m_selected_factor = INT_FAC_INT;
            } else {
                m_selected_factor = INT_FAC_NONE;
            }
            // Clear ICSR value if EIINT or FPI is asserted.
            if (m_selected_factor == INT_FAC_FPI) {
                m_srf->SrICSR()->SetPMFP(0);
            } else if (m_selected_factor == INT_FAC_INT) {
                m_srf->SrICSR()->SetPMEI(0);
            }
        } else {
            m_selected_factor = INT_FAC_NONE;
        }

        if ((IsFeintRequested() == true) && (IsFeintAcceptable() == true)) {
            m_selected_factor = INT_FAC_FEINT;
        }
    }
    if (IsFenmiRequested() == true && IsEnableDebugMonitor() == false) {
        m_selected_factor = INT_FAC_FENMI;
    }

#ifdef ENABLE_DBG_BREAK
    if (IsDbintRequested() && IsDir0DM0()) {
        m_selected_factor = INT_FAC_DBINT;
    }

    if (IsRlbRequested() && IsDir0DM0()) {
        m_selected_factor = INT_FAC_RLB;
    }

    if (IsDbnmiRequested()) {
        m_selected_factor = INT_FAC_DBNMI;
    }
#endif // ENABLE_DBG_BREAK

    if (m_tc_id == NC) {
        m_selected_factor_to_nc = m_selected_factor;
        m_pe->UpdateRunnableByIntreq ();
    }
}


uint32_t FsCtrl::GetEiintPriority (void)
{
    // 呼び出し元でm_int_queueのemptyをチェックすること。
    // ここでのempty判定は不要
    return m_int_queue.back()->m_int_priority;
}


uint32_t FsCtrl::GetFpiPriority (void)
{
    // In current specification of G4KH-FPI, there is not support multiple priority.
    return 0;
}


bool FsCtrl::IsIsprMasked (uint32_t pri)
{
    // ISPRから最高優先度を取得
    uint32_t ispr_pri = GetIsprHighPriority();

    // When ISPR.IPS=0, ispr_pri is 17. ISPR.ISP!=0, it is 0-15.
    // EPL=0 && PRI=16-63 is masked when ISPR.ISP!=0. And not masked in case of ISP=0.

    if (ispr_pri <= pri) {
        //割り込みをマスク
        return true;
    } else {
        //割り込みをマスクしない
        return false;
    }
}


bool FsCtrl::IsHmEimaskMasked (uint32_t pri)
{
    uint32_t eimask = m_nc_srf->SrHMPSW ()->GetEIMASK ();
    if (eimask <= pri) {
        // masked
        return true;
    } else {
        // not masked
        return false;
    }
}


bool FsCtrl::IsGmEimaskMasked (uint32_t pri)
{
    uint32_t eimask = m_nc_srf->SrGMPSW ()->GetEIMASK ();
    if (eimask <= pri) {
        // masked
        return true;
    } else {
        // Not masked
        return false;
    }
}


bool FsCtrl::IsHmPlmrMasked (uint32_t pri)
{
    RegData plmr = m_nc_srf->SrHMPLMR()->GetPLM();

    if (plmr <= pri) {
        //割り込みをマスク
        return true;
    } else {
        //割り込みをマスクしない
        return false;
    }
}


bool FsCtrl::IsGmPlmrMasked (uint32_t pri)
{
    RegData plmr = m_nc_srf->SrGMPLMR()->GetPLM();

    if (plmr <= pri) {
        //割り込みをマスク
        return true;
    } else {
        //割り込みをマスクしない
        return false;
    }
}


uint32_t FsCtrl::GetIsprHighPriority (void)
{
    uint32_t pri = INT_PRI_MAX + 1; // Return 17 to unmask the req of EPL=0 && pri=16 when ISP=0.

    RegData ispr = m_srf->SrISPR()->GetISP();
    for (uint32_t i = 0; i < INT_PRI_MAX; i++) {
        if (((ispr>>i)&0x1) == 0x1) {
            pri = i;
            break;
        }
    }
    return pri; // Return value is 0-15, 17(ISP=0). There is no 16.
}


void FsCtrl::CauseAsyncDataBreak (void)
{
    // EXP_LDB_DB,        // DB: Load Data Break
    // EXP_LDB_RM,        // RM: Load Data Break
    // EXP_RMWB_DB,       // DB: RMW Write Break
    // EXP_RMWB_RM,       // RM: RMW Write Break
    // On G3MH/G3KH additionally,
    // EXP_SDB_DB,        // DB: Store Data Break
    // EXP_SDB_RM,        // RM: Store Data Break
    ExpCode exp_code = m_trace->GetExpCode ();
    PhysAddr ret_pc;
    if ( m_branch_type != BR_NOT && m_trace->IsBranched() ) {
        // Taken Branch : PC is updated already.
        ret_pc = m_grf->ReadPC ();
    } else {
        // Normal inst or NTKN branch
        ByteSize inst_size = m_trace->GetInstSize ();
        ret_pc = m_trace->GetInstAddr() + inst_size;
    }
    uint32_t exp_cause  = ExpInfo::GetExpCause (exp_code);
    uint32_t hndl_ofst  = ExpInfo::GetHdlOffset (exp_code);
    bool     is_excp    = true;
    m_insts->ExceptionDB (exp_cause, ret_pc, hndl_ofst, is_excp, exp_code);
    m_exp_msg->ExpMsgPrint (exp_code, m_trace->GetExpCause (), m_trace);
}


void FsCtrl::ExpInit (void)
{
    m_req_rlb   = false;
    m_req_dbnmi = false;
    m_req_dbint = false;
    m_channel_dbint = 0;
    m_all_interrupt_disable = false;
}
