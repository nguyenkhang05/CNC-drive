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
#include <algorithm>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include "./config.h"
#include "./fs_controler.h"
#include "./trace_info.h"
#include "./trace_print.h"
#include "./gregfile.h"
#include "./inst_queue.h"
#include "./statistics.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./cs_ibuf_g3mh.h"
#include "./cs_ibuf.h"
#include "./cs_ibuf_g3kh.h"
#include "./cs_controler.h"
#include "./proc_element.h"
#include "./outstand_base.h"
#include "./cs_lsu_buf.h"
#include "./icache.h"
#include "./lsu.h"

#ifdef SUPPORT_SC_HEAP
#include "./suspend_ctrl.h"
#endif // SUPPORT_SC_HEAP

#define BPMISS_FLUSH
#define STALL_WB_CONFLICT
#define FUSION_BP_STALL
#define STALL_RNFULL


#if defined(_MSC_VER) || (__GNUC__ >= 6)
inline div_t div (uint32_t num, uint32_t denom)
{
    return div (static_cast<int> (num), static_cast<int> (denom));
}
#endif // _MSC_VER


void CsCtrl::CheckPipeStatus (TraceCtrl* trace)
{
#ifdef DEBUG_CS
    if (trace->GetStageCycle(STAGE_DP) == trace->GetStageCycle(STAGE_EA)) {
        CfMsg::TPrint (MSG_ERR, trace, "<Internal Error: Abnormal same DP/EA stage: cycle=%lu>\n",
                       trace->GetStageCycle(STAGE_DP));
    }

    //if (trace->GetStageCycle(STAGE_DP) + 200U < trace->GetStageCycle(STAGE_EA)) {
    //    CfMsg::TPrint (MSG_ERR, trace, "<Internal Error (Doubtful behavior): stall cycle is huge DP=%lu, EX=%lu>\n",
    //                   trace->GetStageCycle(STAGE_DP), trace->GetStageCycle(STAGE_EA));
    //}

    if (IsFusion ()) {
        return;
    }
    {
        // Check DP stage 3 or more instructions
        bool is_found_dp = false;
        std::vector<PipeInfoType>::iterator it_dp = m_pipe_dp.begin ();
        while (it_dp != m_pipe_dp.end ()) {
            uint64_t cycle = (*it_dp).first;
            bool is_para = (*it_dp).second;
            if (trace->GetStageCycle(STAGE_DP) == cycle) {
                if (is_para) {
                    // This cycle already contains 2 instructions!
                    CfMsg::TPrint (MSG_ERR, trace, "<Internal Error: Abnormal DP stage: cycle=%lu>\n",
                                    cycle);
                } else {
                    (*it_dp).second = true;
                }
                is_found_dp = true;
                break;
            }
            ++ it_dp;
        }
        if (is_found_dp == false) {
            it_dp = m_pipe_dp.begin ();
            while (it_dp != m_pipe_dp.end ()) {
                uint64_t cycle = (*it_dp).first;
                if (cycle > trace->GetStageCycle(STAGE_DP)) {
                    m_pipe_dp.insert (it_dp, PipeInfoType (trace->GetStageCycle(STAGE_DP), false));
                    break;
                }
                ++ it_dp;
            }
            if (m_pipe_dp.empty () || it_dp == m_pipe_dp.end ()) {
                // initial entry or last entry
                m_pipe_dp.push_back (PipeInfoType (trace->GetStageCycle(STAGE_DP), false));
            }
        }

        // Limit size (delete old ently)
        it_dp = m_pipe_dp.begin ();
        while (it_dp != m_pipe_dp.end ()) {
            uint64_t cycle = (*it_dp).first;
            if (trace->GetStageCycle(STAGE_IA) > cycle) {
                it_dp = m_pipe_dp.erase (it_dp);
            } else {
                ++ it_dp;
            }
        }
    }
#if 0
    {
        // DP issue instractions doesn't use EX. Skip EX checking.
        int32_t inst_id = trace->GetInstId ();
        if (MicroArchDB::GetExecResType (inst_id, 0) == EX_RES_DP) {
            return;
        }

        // Check EA stage
        bool is_found_ex = false;
        std::vector<PipeInfoType>::iterator it_ex = m_pipe_ex.begin ();
        while (it_ex != m_pipe_ex.end ()) {
            uint64_t cycle = (*it_ex).first;
            bool is_para = (*it_ex).second;
            if (trace->GetStageCycle(STAGE_EA) == cycle) {
                if (is_para) {
                    // @@ TODO : Need to divide for each EX resource for this checking.
                    // This cycle already contains 2 instructions!
                    //CfMsg::TPrint (MSG_ERR, trace, "<Internal Error: Abnormal EA stage: cycle=%lu>\n",
                    //               cycle);
                } else {
                    (*it_ex).second = true;
                }
                is_found_ex = true;
                break;
            }
            ++ it_ex;
        }
        if (is_found_ex == false) {
            it_ex = m_pipe_ex.begin ();
            while (it_ex != m_pipe_ex.end ()) {
                uint64_t cycle = (*it_ex).first;
                if (cycle > trace->GetStageCycle(STAGE_EA)) {
                    m_pipe_ex.insert (it_ex, PipeInfoType (trace->GetStageCycle(STAGE_EA), false));
                    break;
                }
                ++ it_ex;
            }
            if (m_pipe_ex.empty () || it_ex == m_pipe_ex.end ()) {
                // initial entry or last entry
                m_pipe_ex.push_back (PipeInfoType (trace->GetStageCycle(STAGE_EA), false));
            }
        }
        // Limit size (delete old ently)
        it_ex = m_pipe_ex.begin ();
        while (it_ex != m_pipe_ex.end ()) {
            uint64_t cycle = (*it_ex).first;
            if (trace->GetStageCycle(STAGE_IA) > cycle) {
                it_ex = m_pipe_ex.erase (it_ex);
            } else {
                ++ it_ex;
            }
        }
    }
#endif //0
#endif // #ifdef DEBUG_CS
}




#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
void CsCtrl::CsTraceSimulate (TraceCtrl* trace)
{
#if defined(COMP_RUNNER)
    if (likely (trace->GetExpCode() == EXP_NONE) && (trace->IsCancelMcyc() == false)) {
#else // #if defined(COMP_RUNNER)
    if (likely (trace->GetExpCode() == EXP_NONE)) {
#endif //#if defined(COMP_RUNNER)
        if (m_core_type == CORE_G4KH) {
            m_ibuf->SetupFetchLatency (trace);
            UpdateCsInstFlag (trace);
            StageIF (trace);
            StageBranch<CORE_G4KH> (trace);
            InstIssueAnalysis (trace);
            DependencyAnalysis (trace);
            UpdateInstMemFlag (trace);
            CalcLsuLatency (trace);
            UpdateAvailCycle<CORE_G4KH> (trace);
            UpdateIssueRate<CORE_G4KH> (trace);
            trace->SetParallel (m_parallel_issue_flag);
        } else {
            UpdateCsInstFlag (trace);
            StageIFG3MH (trace);
            StageBranch<CORE_G4MH> (trace);
            InstDispatchAnalysisOoO (trace);
            DependencyAnalysisOoO (trace);
            UpdateInstMemFlag (trace);
            CalcLsuLatency (trace);
            UpdateAvailCycle<CORE_G4MH> (trace);
            UpdateIssueRate<CORE_G4MH> (trace);
            trace->SetParallel (m_parallel_dispatch_flag || IsFusion()); // @@ TODO : EX issue should be used.
        }
        m_cs_last_cycle_count = m_cs_cycle_count;

#ifdef DEBUG_CS
        CfMsg::TPrint (MSG_ERR, trace, "cs_cycle=%lu, dp=%lu, ea=%lu, lsu=%lu\n",
                       m_cs_cycle_count, trace->GetStageCycle(STAGE_DP),
                       trace->GetStageCycle(STAGE_EA), GetLsuIssueTime());
#endif // #ifdef DEBUG_CS
    } else {
        StageException (trace);
        if (m_core_type == CORE_G4KH) {
            trace->SetParallel (m_parallel_issue_flag);
        } else {
            trace->SetParallel (m_parallel_dispatch_flag); // @@ TODO : EX issue should be used.
        }
        m_cs_last_cycle_count = m_cs_cycle_count;
    }
#ifdef DEBUG_CS
    CheckPipeStatus (trace);
#endif // DEBUG_CS
}

void CsCtrl::CalcLsuLatency (TraceCtrl* trace)
{
    CalcLsuReadLatency (trace);
    CalcLsuWriteLatency (trace);

}
#endif // SUPPORT_SC_HEAP

void CsCtrl::StageException (TraceCtrl* trace)
{
    // @@ 例外発生時のサイクル更新未対応
    //    CubeSuite向けにTraceInfo.stage_cycleも更新すること
    // @@ 仮設定: TRAP例外命令のレイテンシに合わせて実装
    // Exception detecting timing is different for each exception type.
    // In CForest, EA_STAGE of current inst is used as trigger timing of the caused exception
    // - Completion Type : Should be same as normal but skipped currently.
    // - Suspend/Re-execution Type : EA stage basically.
    m_branch_target_pos = (1 + MicroArchDB::GetIssueRate (INST_ID_V_TRAP, m_archdb_id)
                             - m_length_if_to_id)
                        - 1;
    m_cs_cycle_count += 1;
    if (m_core_type != CORE_G4KH) {
        trace->SetStageCycle (STAGE_DP, m_cs_cycle_count);
        ++ m_cs_cycle_count;

#ifdef DEBUG_PRINT_STALL
        trace->SetLastIssueCycle (trace->GetStageCycle (STAGE_EA));
#endif // #ifdef DEBUG_PRINT_STALL
        trace->SetStageCycle (STAGE_EA, m_cs_cycle_count);

        // 例外発生命令と例外復帰後の1命令目が同時実行されないようにする
        m_parallel_issue_flag = false;
        m_parallel_dispatch_flag = false;
        first_para_group = PGRP_UNDEF;
        m_last_d_stage = m_cs_cycle_count;

        // Set branch flag to update IA address
        m_is_branch_target = true;
        m_is_branch_target_next = false;
        m_last_bp_result = BP_NONE;
        m_is_id_branch_inst = false;
        m_is_update_ibuf_time = true;
        //m_branch_target_pos =
        //    MicroArchDB::GetIssueRate (INST_ID_V_TRAP, m_archdb_id) - 6; // - STAGE_EA
    } else {
        trace->SetStageCycle (STAGE_DP, m_cs_cycle_count);
        trace->SetStageCycle (STAGE_EA, m_cs_cycle_count + 1);

        // 例外発生命令と例外復帰後の1命令目が同時実行されないようにする
        m_parallel_issue_flag = false;
        first_para_group = PGRP_UNDEF;
        m_last_d_stage = m_cs_cycle_count;
        m_is_branch_target = true;
    }
    SetFusion (false);
    m_prev_reg2 = 0xFFFFFFFF; // invalid reg number
    m_is_prev_fusionable_inst = FUSION_UNDEF;
}


void CsCtrl::RegisterRomRegDependency (CReg src_reg, CReg dest_reg)
{
    // 以前のROMロードのdstレジスタをsrcレジスタとして使用したら、
    // この命令もフェッチ競合レイテンシの反映対象に加える

    // 既存エントリを削除
    // - 本当は消さずに遅らせるべきだが、
    //   1個のレジスタが2重に影響を受けて過大な補正になることを防止するため
    if (m_has_reg_depend_list [dest_reg]) {
        std::vector<DepRegInfo>::iterator it = m_has_reg_depend.begin ();
        while (it != m_has_reg_depend.end ()) {
            if ((*it).second == dest_reg) {
                it = m_has_reg_depend.erase (it);
            } else {
                ++ it;
            }
        }
    }

    // 登録
    m_has_reg_depend.push_back (DepRegInfo(src_reg, dest_reg));
    m_has_reg_depend_list [dest_reg] = true;
}


void CsCtrl::StageIF (TraceCtrl* trace)
{
    if (m_core_type == CORE_G4KH) {
        if (m_is_branch_target == true){  //previous branch taken
            m_cs_fetch_cycle_count = m_cs_cycle_count + m_branch_target_pos;
            m_update_branch_target_fetch_cycle = false;
        }

        trace->SetBraTargetInfo(m_is_branch_target);
        ByteSize inst_size = trace->GetInstSize ();

        if ((m_is_branch_target) || ((inst_size > 4) && (m_is_prev_dw == 2))) {
            //update fetch cycle count branching
            //or when already 2 successive 48 bit - instructions and this is the 3rd
            //(treat limitation of IFU in case multiple 48 bit-instructions)
            m_ibuf_g3kh->SetIfuStartFetchAfterBranch (m_cs_cycle_count);
            m_is_prev_dw = 3;
        }

        //Fetch /Pre-fetch and IFU simulation with cycle information
        m_ibuf_g3kh->SimFetch(trace);

        // IF stage fixation
        trace->SetStageCycle (STAGE_IA, m_cs_fetch_cycle_count);

        uint64_t ibuf_cycle = m_ibuf_g3kh->GetFetchCycle(trace);
        // Update fetch cycle
        if (m_cs_fetch_cycle_count < ibuf_cycle) {
            m_cs_fetch_cycle_count = ibuf_cycle;
        }

        // The fetch time is updated.
        // The ID stage position is adjusted responding to the fetch timing.
        if (m_cs_cycle_count < m_cs_fetch_cycle_count + m_length_if_to_id) {
            m_cs_cycle_count = m_cs_fetch_cycle_count + m_length_if_to_id;
        } else if (m_cs_fetch_cycle_count <= m_prev_cs_fecth_count + 2) {
            // Issue control simultaneously by IQ dryness
            // When the instruction queue is consumed within 3 cycles,
            // It is not in time to forward it from the instruction queue.
            //- If it is +2cycle or less, 1cycle stole is worn from the last fetch.
            //- It applies only to the case that can be issued simultaneously.
            //- ..reset (.. at the divergence when diverging
            //Should I put the @@ 16byte stepping over in the rule?
        }
        m_prev_cs_fecth_count = m_cs_fetch_cycle_count;

        // When being calculate at the fetch time next time,
        // it records because ID stage passage cycle number is necessary.
        m_prev_fetch_cycle = m_cs_cycle_count;

        // The fetch compulsion demand is clear.
        m_is_force_fetch = false;

        //m_is_prev_dw: count successive 48bit - instructions
        //treat limitation of IFU in case multiple 48bit - instructions
        if (m_is_prev_dw == 3) {
            m_is_prev_dw = 0 ;
        }else {
            if (inst_size > 4){
                m_is_prev_dw ++;
            }else{
                m_is_prev_dw = 0;
            }
        }
    }
}


void CsCtrl::StageIFG3MH (TraceCtrl* trace)
{
    if (m_is_update_ibuf_time) {
        // 分岐予測HITやNTKNを除く分岐時にはdispatch時間を分岐命令のEXに連動させる
        if (m_last_d_stage != 0) {
            SyncDispatchCycle (m_last_d_stage - 1);
        }
    }

    PhysAddr inst_addr = trace->GetInstAddr ();

    // G3MHではIBUF空いていれば、どこまでも先行フェッチする
    // - 厄介なのは、先行フェッチのときに分岐予測先をどんどんたどっていくこと。
    //   EX側でストールすると、ものすごい先までフェッチする。分岐予測ミスのペナルティも大きい
    if ((m_is_branch_target) || (m_is_update_ibuf_time)) {
        //CfMsg::TPrint (MSG_INF, trace, "m_is_branch_target=%d, m_is_update_ibuf_time=%d\n",
        //               m_is_branch_target, m_is_update_ibuf_time);
        // 分岐tgtまたは分岐予測ミスのNTKN
        uint64_t ia_cycle;
        if (m_last_bp_result == BP_HIT) {
             // IC branch
            ia_cycle = m_ibuf_g3mh->GetIaCycle () - 1 + m_branch_target_pos;
            //CfMsg::TPrint (MSG_INF, trace,
            //    "kita1:ia_cycle=%lu, m_ibuf_g3mh->GetIaCycle ()=%lu, m_branch_target_pos=%lu\n",
            //    ia_cycle, m_ibuf_g3mh->GetIaCycle (), m_branch_target_pos);
        } else if (m_is_id_branch_inst) {
            // ID branch
            //ia_cycle = m_cs_fetch_cycle_count + 1;
            ia_cycle = m_last_id_cycle;
            m_is_id_branch_inst = false;
            //CfMsg::TPrint (MSG_INF, trace,
            //    "kita2:ia_cycle=%lu, m_cs_fetch_cycle_count=%lu, m_last_id_cycle=%lu\n",
            //    ia_cycle, m_cs_fetch_cycle_count, m_last_id_cycle);
        } else {
            // EX/WB branch
            ia_cycle = m_last_d_stage + m_branch_target_pos;
            //CfMsg::TPrint (MSG_INF, trace,
            //               "kita3:ia_cycle=%lu, m_last_d_stage=%lu, m_branch_target_pos=%lu\n",
            //               ia_cycle, m_last_d_stage, m_branch_target_pos);
        }
        m_ibuf_g3mh->UpdateIaAddress (inst_addr, ia_cycle);
        m_is_update_ibuf_time = false;
    } else {
        //ia_cycle = m_ibuf_g3mh->GetIaCycle ();
        //CfMsg::TPrint (MSG_INF, trace, "kita4\n");
        // @@ PCを書き換えたときとか。IAを更新しないとまずい。
    }

    m_ibuf_g3mh->SimFetch (trace);
    uint64_t fixed_ia_cycle = m_ibuf_g3mh->
                GetIaCycle (inst_addr, inst_addr + trace->GetInstSize () - 1);

#ifdef DEBUG_CS
    if (trace->GetStageCycle(STAGE_IA) > fixed_ia_cycle) {
        // Wrong cycle. Prev IF is larger than next IF.
        CfMsg::TPrint (MSG_ERR, trace, "<Internal Error: Prev STAGE_IA=%" PRIu64 ", New STAGE_IA=%" PRIu64 ">\n",
                trace->GetStageCycle (STAGE_IA), fixed_ia_cycle);
    }
#endif // #ifdef DEBUG_CS

    // IFステージ確定(=IA)
    trace->SetStageCycle (STAGE_IA, fixed_ia_cycle);

    uint64_t ibuf_cycle = m_ibuf_g3mh->GetFetchCycle (trace);
    // フェッチ時刻を更新
    if (m_cs_fetch_cycle_count < ibuf_cycle) {
        m_cs_fetch_cycle_count = ibuf_cycle;
    }

    trace->SetStageCycle (STAGE_IT, m_cs_fetch_cycle_count);
    //CfMsg::TPrint (MSG_INF, trace, "STAGE_IA=%lu, STAGE_IT=%lu\n",
    //               trace->GetStageCycle (STAGE_IA), trace->GetStageCycle (STAGE_IT));

#ifdef USE_STALL_COUNT
    uint64_t prev_dispatch_cycle = m_dispatch_cycle[0];
#endif // #ifdef USE_STALL_COUNT

    // フェッチタイミングに応じて、IDステージ位置を調整
    if (m_dispatch_cycle[0] < m_cs_fetch_cycle_count + m_length_ic_to_dp) {
        SyncDispatchCycle (m_cs_fetch_cycle_count + m_length_ic_to_dp);
    }
    m_prev_cs_fecth_count = m_cs_fetch_cycle_count;

    // 先行命令と自命令で（または自命令が）IFQ境界を跨いでいたら、IFQの更新を通知
    // TimingはIDステージ
    uint64_t id_cycle;
    if ((m_parallel_dispatch_flag == true) && (m_dispatch_cycle[1] == m_dispatch_cycle[0])) {
        // 1 + STAGE_DP - STAGE_ID
        // Prev insts are dispatched in parallel, this inst ID will delay 1 cycle.
        id_cycle = m_dispatch_cycle[0] - 1;
    } else {
        // STAGE_DP - STAGE_ID
        id_cycle = m_dispatch_cycle[0] - 2;
    }

    ByteSize inst_size = trace->GetInstSize ();
    PhysAddr inst_end_addr = inst_addr + (inst_size - 1);
    m_ibuf_g3mh->UpdateIdAddress (inst_addr, id_cycle);
    m_ibuf_g3mh->UpdateIdAddress (inst_end_addr, id_cycle);

    m_last_id_cycle = id_cycle;

    // 次回のフェッチ時間計算時にIDステージ経過サイクル数が必要なので記録
    m_prev_fetch_cycle = m_dispatch_cycle[0];

    //if (m_is_branch_predict_miss == true) {
        // 分岐先命令のDP=分岐命令のCMT+1
        // 実質的にはSYNC同様に、先行命令のCMT(RA)を待ってみる
    //    m_is_branch_predict_miss = false;

    //    m_dispatch_cycle = std::max (m_last_cmt_cycle + 1, m_dispatch_cycle);
    //}
#ifdef USE_STALL_COUNT
    if (prev_dispatch_cycle != m_dispatch_cycle[0]) {
        if (m_cs_cycle_count < m_dispatch_cycle[0] + 1) {
            if (m_is_branch_target) {
                m_stall_counter[STALL_CNT_BRANCH]->Count ((m_dispatch_cycle[0] + 1) -
                        m_cs_cycle_count);
            } else {
                m_stall_counter[STALL_CNT_FETCH]->Count ((m_dispatch_cycle[0] + 1) -
                        m_cs_cycle_count);
            }
        }
    }
#endif // #ifdef USE_STALL_COUNT
}


template <CoreType core_type>
void CsCtrl::StageBranch (TraceCtrl* trace)
{
    if (core_type != CORE_G4KH) {
        m_is_branch_target = false;
        if (m_fsctrl->m_branch_type != BR_NOT) {
            CalcBranchTargetCycleG3H (trace);
        }
    } else {
        // Updated flag to indicate whether it is branch target instruction
        if (m_is_branch_target) {
            m_is_branch_target_next = true;
            m_is_branch_target = false;
        } else if (m_is_branch_target_next) {
            m_is_branch_target_next = false;
        }

        // For branch instrcutions, calculate branch destiantion fetch timing
        if (m_fsctrl->m_branch_type != BR_NOT) {
            CalcBranchTargetCycle (trace);
        }
    }
}


void CsCtrl::CalcBranchTargetCycleG3H (TraceCtrl* trace)
{
    // 分岐予測ペナルティ反映
    // - 分岐予測対象命令なら分岐先のフェッチタイミングを更新する
    // - NotTaken分岐でも分岐予測失敗なら後続のフェッチタイミングをずらす
    // - 命令バッファにヒットした場合、分岐ペナルティを-１する
    m_is_update_ibuf_time = true;

    m_last_bp_result = trace->GetBpResult ();
    if (trace->GetInstId () == INST_ID_V_BCOND_D9) {
        m_is_bcond9 = true;
    } else {
        m_is_bcond9 = false;
    }

    uint32_t inst_id = trace->GetInstId ();
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);

    if (m_fsctrl->m_branch_type == BR_PRED) {
        // 分岐予測の対象命令
        // 予測対象命令だが条件揃わず、分岐予測しないケースも含む
        // BP_NONEの場合は、ntknなら予測ヒット、tknなら予測ミスの扱い

        SetVarIssueRate (1); // @@ kari

        BpResult bp_result = trace->GetBpResult ();
        bool is_branched = trace->IsBranched ();

        // ID branch isn't predicted actually.
        // IC hit  -> as BP hit,  IC = branch tgt IA
        // IC miss -> as BP miss, ID = branch tgt IA
        // G3MH doesn't predict branch on IC-MISS.
        //PhysAddr inst_addr = trace->GetInstAddr ();
        //ByteSize inst_size = trace->GetInstSize ();
        //if (m_ibuf_g3mh->IsIcHit (inst_addr, inst_addr + inst_size - 1) == false) {
        //    bp_result = BP_NONE;
        //    m_last_bp_result = BP_NONE;
        //}

        if (issue_rate == 3) {
            //printf("kita20:issue_rate=%lu\n", issue_rate);
            m_is_id_branch_inst = true;
            // ID branch isn't predicted actually.
            // IC hit  -> as BP hit,  IC = branch tgt IA
            // IC miss -> as BP miss, ID = branch tgt IA
            PhysAddr inst_addr = trace->GetInstAddr ();
            ByteSize inst_size = trace->GetInstSize ();
            if (m_ibuf_g3mh->IsIcHit (inst_addr, inst_addr + (inst_size - 1)) == true) {
                m_last_bp_result = BP_HIT;
            } else {
                m_last_bp_result = BP_NONE;
            }
        } else {
            //printf("kita21:issue_rate=%lu\n", issue_rate);
            m_is_id_branch_inst = false;
        }

        if (  (bp_result == BP_HIT)
           || ((bp_result == BP_NONE) && (is_branched == false)) ) {

            // 分岐先を先行フェッチ可能なので時間を更新しない
            m_is_update_ibuf_time = false;

            // 予測ヒットなら自命令のIC=分岐先IA
            m_branch_target_pos = 2; // IC-IA
        } else {
            // 分岐予測ミスでは、自命令のID/EX/WB=分岐先IA
            m_branch_target_pos = issue_rate - 6; // - STAGE_EA
            //m_is_branch_predict_miss = true;

#ifdef FUSION_BP_STALL
            // In case of BPMISS with Fusined bcond, m_branch_target_pos+=1.
            // Do it after Fusion checking.
            m_update_branch_target_pos_by_fusion = true;
#endif // #ifdef FUSION_BP_STALL
        }
        m_is_branch_target = is_branched;
        SetVarIssueRate (1); // @@ kari
    } else if (m_fsctrl->m_branch_type != BR_NOT) {
        // 特殊命令(Dispose/callt/trap/ctretなど)
        SetVarIssueRate (1); // @@ kari
        m_branch_target_pos = issue_rate - 6; // - STAGE_EA
        m_is_branch_target = true;
    } else {
        m_is_update_ibuf_time = false;
    }
}


void CsCtrl::CalcBranchTargetCycle (TraceCtrl* trace)
{
    // Reflect branch prediction penalty
    // - If it is a branch prediction target instruction, the fetch timing of the branch destination is updated.
    // - If the branch prediction fails even for NotTaken branches, the subsequent fetch timing is shifted.
    // - If the instruction buffer is hit, the branch penalty is decreased by 1.

    m_is_update_ibuf_time = true;
    if (m_fsctrl->m_branch_type == BR_PRED) {
        // Target instruction for branch prediction
        // Including the case where the instruction is a prediction target but the conditions are not met
        //   and the branch is not predicted
        // In the case of BP_NONE,
        //  - if ntkn, a prediction hit is handled.
        //  - if tkn, a prediction mistake is handled.

        {
            m_is_branch = true;
            //m_ibuf->PreloadInstBuf (m_cs_fetch_cycle_count);
            PhysAddr next_addr = trace->GetNextInstAddr () & 0xFFFFFFFC;

            if (trace->IsBranched () == true) {
                 m_is_taken = true;
                 m_branch_target_pos = 1;
            }else {
                  m_branch_target_pos = 0;
            }

             //m_update_branch_target_fetch_cycle = true;

             PhysAddr inst_addr = trace->GetInstAddr ();
             ByteSize inst_size = trace->GetInstSize ();
             PhysAddr inst_next_addr = inst_addr + inst_size;
             // If the branch destination and the next address and the branch
             // destination of the branch instruction are the same lines,
             // ROM fetch timing is not updated.
             if (next_addr == inst_next_addr) {
                   m_is_update_ibuf_time = false;
             }
             if (trace->IsBranched () == true) {
                   m_is_branch_target = true;
             }
        }
    } else if (m_fsctrl->m_branch_type != BR_NOT) {
        PhysAddr next_addr = trace->GetNextInstAddr () & ~LINE_MASK;

        // Branch destination fetch with ID + 1 of own instruction.
        // If it hits the instruction buffer, reduce it by 1 cycle.
        {
            m_is_taken = true;
            m_is_branch = true;
            m_branch_target_pos = 0;
        }
        m_update_branch_target_fetch_cycle = true;

        m_branch_target_addr = next_addr;
        m_is_branch_target = true;
        m_is_branch_target_next = false;

        PhysAddr inst_addr = trace->GetInstAddr ();
        ByteSize inst_size = trace->GetInstSize ();
        PhysAddr inst_next_addr = inst_addr + inst_size;
        if (next_addr == inst_next_addr) {
            m_is_update_ibuf_time = false;
        }
    }
}


void CsCtrl::InstIssueAnalysis (TraceCtrl* trace)
{
    static const bool parallel_table[PGRP_NUM][PGRP_NUM] = {
    /* Parallel execution availability table                                                   */
    /* 0: Can't、1: Can                                                           */
    /*        1st instruction                                                        */
    /* 2nd     1_1,1_2,1_3,1_4,1_5,2_1,2_2,2_3,2_4,2_5,2_6,2_7,2_8,3_1,3_2,3_3,3_4,4_1,4_2,4_3,4_4,5_1,5_2,5_3,5_4,5_5,6_1,7_1,8_1,UNDEF */
    /* 1_1 */ {  0,  1,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 1_2 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 1_3 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 1_4 */ {  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 1_5 */ {  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_1 */ {  0,  1,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_2 */ {  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_3 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_4 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_5 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_6 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_7 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 2_8 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 3_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 3_2 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 3_3 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 3_4 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 4_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 4_2 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 4_3 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 4_4 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 5_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 5_2 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 5_3 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 5_4 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 5_5 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 6_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 7_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /* 8_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    /*UNDEF*/ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,   0},
    };

    ParaGroupType para_group;
    para_group = MicroArchDB::GetParaGroupG3KH (trace->GetInstId ());

    if (m_parallel_issue_flag == true) {
        // Since the preceding instruction was executed at the same time,
        //  the instruction itself cannot be executed in parallel.
        // Since G3K is one issue, there is no parallel execution
        m_parallel_issue_flag = false;
    } else {
        /* Determines parallel execution of own instruction */
        /* - The own instruction is not the branch Taken destination */
        /* - The issue time of the preceding instruction is the same as the issue time of its own instruction */
        /* - Instruction combinations that can be executed in parallel */
        if ( (m_cs_last_cycle_count == m_cs_cycle_count)
          && (parallel_table[para_group][first_para_group] == 1) ) {
            // Parallel execution possible
            m_parallel_issue_flag = true;
        }
    }

    // If parallel issue is not possible and the time is the same as the preceding instruction,
    //  the number of cycles is incremented.
    if (m_parallel_issue_flag == false) {
        if (m_cs_last_cycle_count == m_cs_cycle_count) {
            m_cs_cycle_count += 1;
//        } else {
//            uint64_t residue = (m_cs_cycle_count - m_cs_last_cycle_count) % 1;
//            if (residue != 0) {
//                m_cs_cycle_count += 1 - residue;
//            }
        }
    }

    // Records a group of instructions to determine whether to issue the next instruction in parallel
    first_para_group = para_group;
}


// Fix DP stage timing by IA stage timing
// - Set all DP stage to the cycle
void CsCtrl::SyncDispatchCycle (uint64_t cycle)
{
    std::vector<uint64_t>::iterator it = m_dispatch_cycle.begin ();
    while (it != m_dispatch_cycle.end()) {
        *it = std::max (*it, cycle);
        ++ it;
    }
}


void CsCtrl::InstDispatchAnalysisOoO (TraceCtrl* trace)
{
    // 特殊命令やFPU(double)は2命令以上扱いなので、同時発行(dispatch/issueの両方)しない。
    // ReservationStationがいっぱいならDispatchできない。

#if 0
    static const bool parallel_table[PGRP_NUM][PGRP_NUM] = {
    /* 並列実行可否テーブル                                                   */
    /* 0:不可、1:可                                                           */
    /*        先行命令                                                        */
    /* 後続    1_1,1_2,1_3,1_4,1_5,2_1,2_2,2_3,2_4,2_5,2_6,2_7,2_8,3_1,3_2,   */
    /*         3_3,3_4,4_1,4_2,4_3,4_4,5_1,5_2,5_3,5_4,5_5,6_1,7_1,8_1,8_2,9_1,9_2,9_3,9_4,UNDEF */
    /* 1_1 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 1_2 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 1_3 */ {  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 1_4 */ {  1,  1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 1_5 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_1 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_2 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_3 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_4 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_5 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_6 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_7 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 2_8 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  1,    0},
    /* 3_1 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  0,  0,  1,    0},
    /* 3_2 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  1,    0},
    /* 3_3 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  0,  1,  0,  0,  1,    0},
    /* 3_4 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  1,    0},
    /* 4_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 4_2 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 4_3 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 4_4 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 5_1 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,    0},
    /* 5_2 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,    0},
    /* 5_3 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 5_4 */ {  1,  1,  0,  1,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,
                 1,  0,  1,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,    0},
    /* 5_5 */ {  1,  1,  0,  1,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1,  0,
                 1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  0,    0},
    /* 6_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 7_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 8_1 */ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    /* 8_2 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,    0}, // DIV
    /* 9_1 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,    0},
    /* 9_2 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,
                 0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,    0},
    /* 9_3 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,
                 0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1,  0,  0,  0,  1,  1,  0,  0,  1,    0},
    /* 9_4 */ {  1,  1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
                 1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,    0},
    /*UNDEF*/ {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,    0},
    };
#endif //0
    ParaGroupType para_group;

    FusionCheck (trace);

    CoproType copro_type = MicroArchDB::GetCoproType (trace->GetInstId());
    // 命令グループを取得
    if (trace->IsParaGroupSwitched () == true) {
        // 命令グループ切替条件成立時、命令グループが変化する
        para_group = MicroArchDB::GetParaGroupAlt (trace->GetInstId ());
    } else {
        // 標準の命令グループを使用
        para_group = MicroArchDB::GetParaGroup (trace->GetInstId());
    }

    if (IsFusion ()) {
        // In case of Fusion, instruction is executed with previous instruction
        trace->SetStageCycle (STAGE_DP, m_last_dispatch_cycle);
//printf ("Fusion\n");

        // 次の命令の並列発行判定のため、自命令のグループを記録
        first_para_group = para_group;
        m_prev_copro_type = copro_type;

#ifdef FUSION_BP_STALL
        // In case of BPMISS with Fusined bcond, m_branch_target_pos+=1.
        if (m_update_branch_target_pos_by_fusion) {
            m_update_branch_target_pos_by_fusion = false;
            m_branch_target_pos += 1;
        }
#endif // #ifdef FUSION_BP_STALL
        return;
    }

#ifdef FUSION_BP_STALL
    m_update_branch_target_pos_by_fusion = false;
#endif // #ifdef FUSION_BP_STALL
    // RleaseAndSortRs (trace, m_dispatch_cycle[0]);

    // RSが空くまではDispathできないので、RS空きまでのストールを計算
    uint32_t rs_stall = CalcDispatchLatency (trace);
    uint32_t stall = rs_stall + m_prev_issue_rate;
    //printf ("m_prev_issue_rate = %u\n", m_prev_issue_rate);
    m_prev_issue_rate = 0;
    DpDividedType dp_type = MicroArchDB::GetDpDividedType (trace->GetInstId ());
    if (   (m_dispatch_cycle[0] == m_dispatch_cycle[1])
        || (m_parallel_dispatch_flag == true)) {
        // 先行命令が同時dispatchされたので、自命令はdispatchできない
        m_parallel_dispatch_flag = false;
    } else {
        /* 自命令の並列実行を判定 */
        /* - 自命令が分岐Taken先でない */
        /* - 先行命令発行時刻と自命令の発行時刻が同じ */
        /* - 並列実行可能な命令組み合わせ */
        if ( (stall == 0) && (m_last_dispatch_cycle == m_dispatch_cycle[0])
          //&& (parallel_table[para_group][first_para_group] == 1)
          && (!m_use_2rs) && (dp_type == DPD_UNDEF) ) {
            // 並列実行可能
            m_parallel_dispatch_flag = true;
        }
    }

    if ((copro_type == COPRO_SIMD) && (m_prev_copro_type == COPRO_SIMD)) {
        m_parallel_dispatch_flag = false;
        // can dispatch next cycle from prev FXU inst
        if ((m_dispatch_cycle[0] + stall) < m_last_dispatch_cycle) {
            m_dispatch_cycle[0] = m_last_dispatch_cycle;
            stall = 0;
        }
    }

#ifdef USE_STALL_COUNT
    // ストール分析で、無駄な2重カウントを防止するためここで一度EX位置を調整
    //if (m_cs_cycle_count <= m_dispatch_cycle[0]) {
    //    m_cs_cycle_count = m_dispatch_cycle[0] + 1;
    //}
#endif //#ifdef USE_STALL_COUNT

    // @@ TODO : How treat issue cycle in G4MH?
    //           In OoO case, issue cycle isn't apply to following instruction in InO but preciding instruction in OoO.
    //           If the dispatch cycle is later than the instruction, issue latency should be applied.
    // 並列発行不可の場合に先行命令と同じ時刻なら、サイクル数をインクリメント
    if (m_parallel_dispatch_flag == false) {
        if (stall != 0) {
            m_dispatch_cycle[0] += stall;
#ifdef USE_STALL_COUNT
            //if (is_last_parallel == true) {
            //    // 同時発行後のサイクルインクリメント分を除外(mov-mov-mov...の場合でも出てしまう
            //    // @@ 本当は除外すべきでないー＞EXリソース数を増やせばストールしなくできるので。
            //    if ((m_cs_cycle_count + 1) <= m_dispatch_cycle[0]) {
            //        m_stall_counter[STALL_CNT_RSRES]->
            //            Count (m_dispatch_cycle[0] - m_cs_cycle_count - 1);
            //    }
            //} else {
            //    if (m_cs_cycle_count <= m_dispatch_cycle[0]) {
            //        m_stall_counter[STALL_CNT_RSRES]->
            //            Count (m_dispatch_cycle[0] + 1 - m_cs_cycle_count);
            //    }
            //}
#endif //#ifdef USE_STALL_COUNT
        } else {
            if (m_last_dispatch_cycle == m_dispatch_cycle[0]) {
                //if (m_dispatch_cycle[0] == m_dispatch_cycle[1]) {
                //    m_dispatch_cycle.assign (DISPATCH_WINDOW_SIZE, m_dispatch_cycle[0] + 1);
                //} else {
                      m_dispatch_cycle[0] += 1;
                //}
            }
        }
    }

    // CMTストール分析で、無駄な2重カウントを防止するためここで一度EX位置を調整
    //if (m_cs_cycle_count <= m_dispatch_cycle) {
    //    m_cs_cycle_count = m_dispatch_cycle + 1;
    //}
    // RNFull (CMT buffer full)
    //if (m_cmt_stage [m_inflight_id] + 2 > m_dispatch_cycle) {
#ifdef USE_STALL_COUNT
    //    if ((m_cmt_stage [m_inflight_id] + 2) >= m_cs_cycle_count) {
    //        m_stall_counter[STALL_CNT_CMTBUF]->Count (m_cmt_stage [m_inflight_id] + 3 -
    //                m_cs_cycle_count);
    //    }
#endif //#ifdef USE_STALL_COUNT
    //    m_dispatch_cycle = m_cmt_stage [m_inflight_id] + 2;
    //}

    // Dispatch available
    // - Update dispatch cycle
    //if (trace->GetStageCycle (STAGE_DP) > m_dispatch_cycle[0]) printf ("DP OoO\n");
#ifdef ENABLE_EXE_ORDER_CTRL
    if (m_is_ooo_g4mh == false) {
        // InO dispatch.
        if (m_dispatch_cycle[0] < m_last_dispatch_cycle) {
            m_dispatch_cycle[0] = m_last_dispatch_cycle;
        }
    }
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    trace->SetStageCycle (STAGE_DP, m_dispatch_cycle[0]);
    m_last_dispatch_cycle = m_dispatch_cycle[0];

    // Dispatch stage can dispatch instructions by OoO.
    // But, if previous inst is RN divided instruciton,
    // previous inst is divided to 2 instructions. Then, later instrcution can't be dispatched ooo.
    // Because the dispatch window size of DP stage is 2,
    // dispatch unit can dispatches 2 instructions from the top of dispatch queue.
    // TODO : How about ID and DP divided instructions?
    if (MicroArchDB::GetRnDividedType (trace->GetInstId ()) != RND_UNDEF) {
        SyncDispatchCycle (m_dispatch_cycle[0] + 1);
    } else {
        m_dispatch_cycle[0] += 1;
        std::sort (m_dispatch_cycle.begin (), m_dispatch_cycle.end());
    }

    // Calc the shortest issue cycle from dispatch cycle
    if (m_shortest_issue_cycle <= m_last_dispatch_cycle) {
        m_shortest_issue_cycle = m_last_dispatch_cycle + 1;
    }
    // Dispatch is InO to same RS.
    SyncDispatchAvailCycleRS (trace, m_last_dispatch_cycle);

    // 次の命令の並列発行判定のため、自命令のグループを記録
    first_para_group = para_group;
    m_prev_copro_type = copro_type;

#ifdef PRINT_FXU_PIPE
    trace->SetStageCycle (STAGE_VDSKID, 0);
#endif // #ifdef PRINT_FXU_PIPE
    if (MicroArchDB::GetCoproType (trace->GetInstId()) == COPRO_SIMD) {
        if (m_vd_issue_able_cycle > trace->GetStageCycle (STAGE_DP)) {
            // Use VD skid
            m_vd_skid_avail_cycle = m_vd_issue_able_cycle + 1;

#ifdef PRINT_FXU_PIPE
            trace->SetStageCycle (STAGE_VDSKID, m_vd_issue_able_cycle);
#endif // #ifdef PRINT_FXU_PIPE
            //printf ("STAGE_DP=%lu, m_vd_skid_avail_cycle=%lu\n", trace->GetStageCycle (STAGE_DP), m_vd_skid_avail_cycle);
        }
    }
}


uint32_t CsCtrl::CalcDispatchLatency (TraceCtrl* trace)
{
    //prepare/disposeはuOp命令に分割されるので、複数サイクルDispatch必要
    int32_t inst_id = trace->GetInstId();
    ResvResType rs_resource1 = MicroArchDB::GetResvResType (inst_id, 0);
    std::vector<RsInfoType>* res_cycle = m_rs_cycle[rs_resource1];

    uint64_t dispatchable_cycle = 0;
    if (m_use_2rs) {
        // ld.dwなどの2issue命令なら、全RSを使う
        // @@ 未対応: RS>=3の場合、2個だけ占有するように変更必要
        dispatchable_cycle = (res_cycle->back ()).first;
    } else {
        // Some instructions use different 2RS
        ResvResType rs_resource2 = MicroArchDB::GetResvResType (inst_id, 1);
        if ((rs_resource2 != RS_RES_FXUMOV) && (rs_resource2 != RS_RES_FXUOP)) {
            if (rs_resource2 != RS_RES_UNDEF) {
                dispatchable_cycle = ((m_rs_cycle[rs_resource2])->front ()).first;
                //printf ("m_rs_cycle[rs_resource2]=%lu\n",
                //        (m_rs_cycle[rs_resource2])->front ().first);
            }
        } else {
            // FXU has skid buf.
            // If rs is full, skid buf is used instead.
            uint64_t vd_cycle = std::min (((m_rs_cycle[rs_resource2])->front ()).first, m_vd_skid_avail_cycle);
            dispatchable_cycle = std::max (dispatchable_cycle, vd_cycle);
            //printf ("m_rs_cycle[rs_resource2]=%lu, m_vd_skid_avail_cycle=%lu\n",
            //        (m_rs_cycle[rs_resource2])->front (), m_vd_skid_avail_cycle);
        }

        if ((rs_resource1 != RS_RES_FXUMOV) && (rs_resource1 != RS_RES_FXUOP)) {
            dispatchable_cycle = std::max (dispatchable_cycle, (res_cycle->front ()).first);
#ifdef STALL_WB_CONFLICT
            m_rs_alu_id = (res_cycle->front ()).second;
#endif //STALL_WB_CONFLICT
            //printf ("m_rs_cycle[rs_resource1]=%lu\n",
            //        (m_rs_cycle[rs_resource1])->front ().first);
        } else {
            uint64_t vd_cycle = std::min ((res_cycle->front ()).first, m_vd_skid_avail_cycle);
            dispatchable_cycle = std::max (dispatchable_cycle, vd_cycle);
            //printf ("m_rs_cycle[rs_resource1]=%lu, m_vd_skid_avail_cycle=%lu\n",
            //        (m_rs_cycle[rs_resource1])->front (), m_vd_skid_avail_cycle);
        }
    }

#ifdef BPMISS_FLUSH
    dispatchable_cycle = std::max (dispatchable_cycle, m_bpmiss_flush_cycle);
#endif // BPMISS_FLUSH

#ifdef STALL_RNFULL
    uint32_t using_rn_id;
    if (m_use_2rs) {
        using_rn_id = m_curr_rn_id + 1;
        if (using_rn_id >= NUM_RN_BUF) {
            using_rn_id = 0;
        }
    } else {
        using_rn_id = m_curr_rn_id;
    }
    //if ((dispatchable_cycle < m_rn_id_avail_cycle [using_rn_id]) && (m_dispatch_cycle[0] < dispatchable_cycle)) {
    //    // stall by RN full
    //    printf ("RNFULL:stall=%lu, dispatchable_cycle=%lu, m_rn_id_avail_cycle=%lu\n",
    //            m_rn_id_avail_cycle [using_rn_id] - dispatchable_cycle, dispatchable_cycle, m_rn_id_avail_cycle [using_rn_id]);
    //}
    dispatchable_cycle = std::max (dispatchable_cycle, m_rn_id_avail_cycle [using_rn_id]);
#endif // STALL_RNFULL


    //printf ("dispatchable_cycle=%lu, m_dispatch_cycle[0]=%lu\n",
    //        dispatchable_cycle, m_dispatch_cycle[0]);

    int64_t stall = dispatchable_cycle - m_dispatch_cycle[0];
    if (stall < 0) {
        stall = 0;
    }
    //printf ("stall=%u\n", stall);
    return static_cast <uint32_t> (stall);
}


void CsCtrl::SyncDispatchAvailCycleRS (TraceCtrl* trace, uint64_t rs_stage)
{
    // If the RS is same between 2 instructions, RN->DP is InO.
    // - Set cycle to 2RS
    int32_t inst_id = trace->GetInstId();
    for (int i = 0; i < 2; ++i) {
        ResvResType rs_resource = MicroArchDB::GetResvResType (inst_id, i);
        std::vector<RsInfoType>* res_cycle = m_rs_cycle[rs_resource];
        std::vector<RsInfoType>::iterator it = res_cycle->begin ();
        while (it != res_cycle->end ()) {
            if ((*it).first < rs_stage) {
                //if (rs_resource == RS_RES_INT) {
                //    printf ("Update from %lu to %lu at SyncDispatchAvailCycleRS\n",
                //            (*it).first, rs_stage);
                //}
                (*it).first = rs_stage;
                // No need sort because order isn't changed
                // But, ID need to sort
                SortRsId (res_cycle);
            }
            ++ it;
        }
    }
}


void CsCtrl::SetDispatchAvailCycle (TraceCtrl* trace, uint64_t ex_start_stage)
{
    // EX発行までDPで滞留し、RS内で待機するはず。
    // +IssueRateだけではなく、ストールによるEX遅れの影響も受けるので注意。
    int32_t inst_id = trace->GetInstId();
    for (int i = 0; i < 2; ++i) {
        ResvResType rs_resource = MicroArchDB::GetResvResType (inst_id, i);
        std::vector<RsInfoType>* res_cycle = m_rs_cycle[rs_resource];
        if ((rs_resource != RS_RES_FXUMOV) && (rs_resource != RS_RES_FXUOP)) {
            if (m_use_2rs && (res_cycle->size() >= 2)) {
                (*res_cycle)[0].first = ex_start_stage;
                (*res_cycle)[1].first = ex_start_stage;
                break;
            } else {
                //if (rs_resource == RS_RES_INT) {
                //    printf ("Update from %lu to %lu at SetDispatchAvailCycle\n",
                //            (*res_cycle)[0].first, ex_start_stage);
                //}

                //res_cycle->erase (res_cycle->begin ());
                //res_cycle->push_back (ex_start_stage);
                (*res_cycle)[0].first = ex_start_stage;
//#ifdef SORT_RS
                std::sort (res_cycle->begin(), res_cycle->end());
                SortRsId (res_cycle);
//#endif //#ifdef SORT_RS
            }
        } else {
            //res_cycle->erase (res_cycle->begin ());
            //res_cycle->push_back (trace->GetStageCycle (STAGE_DP) + 1);
            //res_cycle->push_back (m_vd_issue_able_cycle + 1);
            (*res_cycle)[0].first = m_vd_issue_able_cycle + 1;
//#ifdef SORT_RS
            std::sort (res_cycle->begin(), res_cycle->end());
            SortRsId (res_cycle);
//#endif // #ifdef SORT_RS
        }
    }
}


void CsCtrl::RleaseAndSortRs (TraceCtrl* trace, uint64_t shortest_dispatch_cycle)
{
    int32_t inst_id = trace->GetInstId();
    ResvResType rs_resource = MicroArchDB::GetResvResType (inst_id, 0);
    std::vector<RsInfoType>* res_cycle = m_rs_cycle[rs_resource];
    if ((res_cycle->back ()).first < shortest_dispatch_cycle) {
        if (res_cycle->size () != 1) {
            (*res_cycle)[0].second = 0;
            (*res_cycle)[1].second = 1;
        }
    }
}

// Sort RSID
// When 2 RS time is same, RS0 needs to be used.
// reorder is necessary.
void CsCtrl::SortRsId (std::vector<RsInfoType>* rs_vec)
{
    if (rs_vec->size () != 1) {
        if ((*rs_vec)[0].first == (*rs_vec)[1].first) {
            (*rs_vec)[0].second = 0;
            (*rs_vec)[1].second = 1;
        }
    }

    // @@ TODO
    // If Both entry are empty, need to re-ID also.
    // How to support it?
}


void CsCtrl::OverwriteDispatchAvailCycleByFusionStall (TraceCtrl* trace, uint64_t new_ex_start_stage,
                                                       uint64_t old_ex_start_stage)
{
    //printf ("kita:new_ex_start_stage=%lu, old_ex_start_stage=%lu\n", new_ex_start_stage, old_ex_start_stage);
    int32_t inst_id = trace->GetInstId();
    for (int i = 0; i < 2; ++i) {
        ResvResType rs_resource = MicroArchDB::GetResvResType (inst_id, i);
        if (rs_resource != RS_RES_UNDEF) {
            std::vector<RsInfoType>* res_cycle = m_rs_cycle[rs_resource];
            std::vector<RsInfoType>::iterator it = res_cycle->begin ();
            while (it != res_cycle->end ()) {
                if ((*it).first == old_ex_start_stage) {
                    //if (rs_resource == RS_RES_INT) {
                    //    printf ("Update from %lu to %lu at OverwriteDispatchAvailCycleByFusionStall\n",
                    //            (*it).first, new_ex_start_stage);
                    //}
                    (*it).first = new_ex_start_stage;
                }
                ++ it;
            }
            std::sort (res_cycle->begin(), res_cycle->end());
        }
    }
}


#ifdef SUPPORT_SC_HEAP
void CsCtrl::CsTraceSimulate (TraceCtrl* trace)
{
    if ((trace->GetExpCode() == EXP_NONE) && (trace->IsCancel () == false)) {
        SuspendState state = m_suspend_ctrl->GetSuspendState ();
        if (state == RELEASED_FS) {
            //m_suspend_ctrl->ApplyNotifyFetchOwn (trace);
            //if (m_ibuf_g3mh->IsFetchLatencyReceived (trace) == false) {
            //    // レイテンシ未知のフェッチがあるのでSCHEAPからの通知を待つ
            //    // @@ FSのStageIF後/StageIDの前で待つべき。
            //    //    今はSC−HEAP側メモリからフェッチしないので未対応。
            //    m_suspend_ctrl->SetSuspendState (SUSPEND_CS_FETCH);
            //    m_suspend_ctrl->SetIsRunnable (false);
            //    return;
            //}
            state = RELEASED_CS_FETCH;
        }
        if (state == RELEASED_CS_FETCH) {
            // SUSPEND_FETCH状態の解除時、またはNO_SUSPEND
            UpdateCsInstFlag (trace);
            StageIFG3MH (trace);
            StageBranch<CORE_G4MH> (trace);
            InstDispatchAnalysisOoO (trace);
            if (m_suspend_ctrl->IsReadLatencyReceivedCs (trace) == false) {
                m_suspend_ctrl->SetSuspendState (SUSPEND_CS_EXEC);
                m_suspend_ctrl->SetIsRunnable (false);
                return;
            }
            state = RELEASED_CS_EXEC;
        }
        if (state == RELEASED_CS_EXEC) {
            DependencyAnalysisOoO (trace);
            UpdateInstMemFlag (trace);
            state = RELEASED_CS_LSU;
        }

        if (state == RELEASED_CS_LSU) {
            CalcLsuLatency (trace);
            if (m_suspend_ctrl->GetLsuRepeated ()) {
                m_suspend_ctrl->SetSuspendState (SUSPEND_CS_LSU);
                m_suspend_ctrl->SetIsRunnable (false);
                return;
            }
            state = RELEASED_CS;
        }
        if (state == RELEASED_CS) {
            UpdateAvailCycle<CORE_G4MH> (trace);
            UpdateIssueRate<CORE_G4MH> (trace);
            trace->SetParallel (m_parallel_dispatch_flag || IsFusion());
            m_cs_last_cycle_count = m_cs_cycle_count;
            m_suspend_ctrl->SetSuspendState (NO_SUSPEND);
            m_suspend_ctrl->ApplyNotifyOwn (trace);
            m_suspend_ctrl->InitNotifyOwn ();
        }
    } else if (trace->GetExpCode() != EXP_NONE) {
        StageException (trace);
        trace->SetParallel (m_parallel_dispatch_flag);
        m_cs_last_cycle_count = m_cs_cycle_count;
        m_suspend_ctrl->SetSuspendState (NO_SUSPEND);
        m_suspend_ctrl->ApplyNotifyOwn (trace);
        m_suspend_ctrl->InitNotifyOwn ();
    } else {
        CancelInst (trace);
        trace->SetParallel (m_parallel_issue_flag);
        m_cs_last_cycle_count = m_cs_cycle_count;
        m_suspend_ctrl->SetSuspendState (NO_SUSPEND);
        m_suspend_ctrl->ApplyNotifyOwn (trace);
        m_suspend_ctrl->InitNotifyOwn ();
    }
}


void CsCtrl::CancelInst (TraceCtrl* trace)
{
    // @@ No need to update ibuf?
    m_cs_cycle_count += 1;
    if (m_core_type == CORE_G4KH) {
        trace->SetStageCycle (STAGE_DP, m_cs_cycle_count);
        trace->SetStageCycle (STAGE_EA, m_cs_cycle_count + 1);

        // Prevent the exception generation instruction and the first instruction after exception return
        //   from being executed simultaneously 
        m_parallel_issue_flag = false;
        first_para_group = PGRP_UNDEF;
        m_last_d_stage = m_cs_cycle_count;
    } else {
        trace->SetStageCycle (STAGE_DP, m_cs_cycle_count);
        ++ m_cs_cycle_count;
#ifdef DEBUG_PRINT_STALL
        trace->SetLastIssueCycle (trace->GetStageCycle (STAGE_EA));
#endif // #ifdef DEBUG_PRINT_STALL
        trace->SetStageCycle (STAGE_EA, m_cs_cycle_count);

        // 例外発生命令と例外復帰後の1命令目が同時実行されないようにする
        m_parallel_issue_flag = false;
        m_parallel_dispatch_flag = false;
        first_para_group = PGRP_UNDEF;
        m_last_d_stage = m_cs_cycle_count;

        // Set branch flag to update IA address
        //m_is_branch_target = true;
        //m_is_branch_target_next = false;
        m_last_bp_result = BP_NONE;
        m_is_id_branch_inst = false;
        m_is_update_ibuf_time = true;

        SetFusion (false);
        m_prev_reg2 = 0xFFFFFFFF; // invalid reg number
        m_is_prev_fusionable_inst = FUSION_UNDEF;
    }
}



void CsCtrl::CalcLsuLatency (TraceCtrl* trace)
{
    if (m_is_read_finished == false) {
        if (m_mld_info.m_is_valid) {
            if (m_mld_info.m_mld_type == MultiLoadInfo::MLD_DISPOSE) {
                CsDisposeMain (trace);
            } else if (m_mld_info.m_mld_type == MultiLoadInfo::MLD_POPSP) {
                CsPopspMain (trace);
            } else if (m_mld_info.m_mld_type == MultiLoadInfo::MLD_RESBANK) {
                CsResbankMain (trace);
            } else if (m_mld_info.m_mld_type == MultiLoadInfo::MLD_LDM) {
                CsLdmMain (trace);
            }
            m_lsu_read_latency = 0;
            m_is_read_finished = (m_mld_info.m_is_valid == false);
            m_suspend_ctrl->SetLsuRepeated (m_mld_info.m_is_valid);
        } else {
            CalcLsuReadLatency (trace);
            m_is_read_finished = true;
        }
    }

    // elseではダメ
    if (m_is_read_finished) {
        CalcLsuWriteLatency (trace);
    }

    if (m_suspend_ctrl->GetLsuRepeated () == true) {
        // LSU latency calculation isn't finished yet.
        return;
    }
}


void CsCtrl::AddIssueRate (uint32_t latency)
{
    m_prev_issue_rate += latency;
}


void CsCtrl::AddRegActiveCycle (CReg reg, uint32_t latency)
{
    m_reg_active_cycle[reg] += latency;
}


void CsCtrl::SetSuspendCtrl (SuspendCtrl* suspend_ctrl)
{
    m_suspend_ctrl = suspend_ctrl;
}
#endif // SUPPORT_SC_HEAP


template <CoreType core_type>
uint32_t CsCtrl::CalcRegStallCycle (TraceCtrl* trace, uint64_t issue_cycle)
{
    uint32_t src_reg_stall = CalcSrcRegStall<core_type> (trace, issue_cycle);
    if (core_type == CORE_G4MH) {
#ifdef USE_DST_STALL
        uint32_t dst_reg_stall = CalcDstRegStall<core_type> (trace, issue_cycle);
        if (src_reg_stall >= dst_reg_stall) {
            return src_reg_stall;
        } else {
            return dst_reg_stall;
        }
#else // USE_DST_STALL
        CalcDstRegStall<core_type> (trace, issue_cycle);
        return src_reg_stall;
#endif // USE_DST_STALL
    } else {
        uint32_t dst_reg_stall = CalcDstRegStall<core_type> (trace, issue_cycle);
        if (src_reg_stall >= dst_reg_stall) {
            return src_reg_stall;
        } else {
            return dst_reg_stall;
        }
    }
}


void CsCtrl::FusionCheck (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId ();
    FusionInstType fusion_type = MicroArchDB::GetFusionInstType (inst_id);
    if (fusion_type != FUSION_UNDEF) {
        if (IsFusion () || (m_last_dispatch_cycle < m_cs_fetch_cycle_count + m_length_ic_to_dp)) {
//printf ("kita1:IsFusion()=%d, m_last_dispatch_cycle=%u, m_dispatch_cycle=%u\n", IsFusion(), m_last_dispatch_cycle, m_cs_fetch_cycle_count + m_length_ic_to_dp);
            // Previous instruction is executed by fuion already.
            // Or, fetch isn't finish until previous dispatch.
            // This instruction isn't executed by Fusion with previous instruction.
            SetFusion (false);
        } else if (   (    (m_is_prev_fusionable_inst == FUSION_ALU1ST)
                        || (m_is_prev_fusionable_inst == FUSION_ALU1ST_MOV2ND))
            && (fusion_type == FUSION_ALU2ND)) {
            // Fusion ALU + BCC
            SetFusion (true);
            m_prev_reg2 = 0xFFFFFFFF;
        } else if (    (m_is_prev_fusionable_inst == FUSION_MOV1ST)
                   &&  (fusion_type == FUSION_ALU1ST_MOV2ND)) {
            // Fusion MOV + ALU
            // Need to check src/dst register
            // - reg2 is same between MOV and ALU instructions
            uint64_t opcode = trace->GetInstCode ();
            int32_t reg2 = OperandExtract (opcode, 15, 11);
            if (m_prev_reg2 == reg2) {
                // Fusion
                SetFusion (true);
            } else {
                // Can't fusion
                SetFusion (false);
            }
        } else {
            SetFusion (false);
        }

        if (fusion_type == FUSION_MOV1ST) {
            uint64_t opcode = trace->GetInstCode ();
            // Record reg2 register number for next fusion checking
            m_prev_reg2 = OperandExtract (opcode, 15, 11);
        }
    } else {
        SetFusion (false);
    }
    m_is_prev_fusionable_inst = fusion_type;
}


template <CoreType core_type>
uint32_t CsCtrl::CalcSrcRegStall (TraceCtrl* trace, uint64_t issue_cycle)
{
    uint32_t stall_cycle = 0;
#ifdef USE_STALL_COUNT
    uint32_t m_stall_src[SRC_RES_NUM] = {0};
#endif // USE_STALL_COUNT
    m_st_req_avail_cycle = 1;

    /* 当該命令の全 RR に対する依存判定処理 */
    int32_t rr_stage;
    int32_t inst_id = trace->GetInstId();
    InstGroupType inst_group = MicroArchDB::GetInstGroup (inst_id);
    uint32_t reg_pos_db = 0;
    CRegVec::const_iterator it_rreg = trace->GetRregDepVec()->begin ();
    CRegVec::const_iterator it_rend = trace->GetRregDepVec()->end ();
    uint32_t cnt_r_arg = 0;
    CRegVec::const_iterator it_rreg_tmp = trace->GetRregDepVec()->begin ();
    CRegVec::const_iterator it_rend_tmp = trace->GetRregDepVec()->end ();
    CReg read_reg = R0;
    CRegVec::const_iterator it;
    it=m_dst_regs.begin();
    if (core_type == CORE_G4KH) {
        while (it_rreg_tmp != it_rend_tmp) {
               ++ cnt_r_arg;
               ++ it_rreg_tmp;
        }
    }
    it_rreg = trace->GetRregDepVec()->begin ();
    it_rend = trace->GetRregDepVec()->end ();
    while (it_rreg != it_rend) {
        read_reg = *it_rreg;
        rr_stage = MicroArchDB::GetRegSrcStage (inst_id, reg_pos_db, m_archdb_id);
        ++ reg_pos_db;

#ifdef CHECK_LACK_DBREG
        if (rr_stage == MicroArchDB::END_ENTRY) {
            // ここに来たら、ArchDBのレジスタ数＜traceのレジスタ数なのでArchDB異常
#ifdef DEBUG_CS
            CfMsg::TPrint (MSG_ERR, trace,
                "<Internal Error: num of srcreg in DB is less than num of trace (inst=%d)>\n",
                inst_id);
#endif // #ifdef DEBUG_CS
            break;
        }
#endif // CHECK_LACK_DBREG

        // R0は依存関係の対象外
        if (read_reg == R0) {
            ++ it_rreg;
            continue;
        }

#ifdef ENABLE_ROM_IDCONFLICT
        if (core_type == CORE_G4KH) {
            if (HasRomDependency (read_reg)) {
                CRegVec::const_iterator it_wreg = trace->GetWregDepVec()->begin();
                while (it_wreg != trace->GetWregDepVec()->end()) {
                    CReg write_reg = *it_wreg;
                    if (write_reg != R0) {
                        RegisterRomRegDependency (read_reg, write_reg);
                    }
                    ++ it_wreg;
                }
            }
        }
#endif //#ifdef ENABLE_ROM_IDCONFLICT

        if (core_type != CORE_G4KH) {
            if (IsFusion () && (read_reg == m_prev_reg2)) {
                // In case of Fusion, reg2 is same with fusioned previous instruction
                ++ it_rreg;
                continue;
            }
        }

        /* レジスタフロー依存判定 */
        uint64_t active_cycle = m_reg_active_cycle[read_reg];
        if (core_type == CORE_G4KH) {
            if (active_cycle > 0) {
                active_cycle-=1;
            }
        }
        // The data source register of ST instruction doesn't cause stall.
        // But, because request timing to BUS is delayed by RA cycle of the data source register.
        // Calculate RA of data reg for the request timing.
        // register at reg_pos_db=0 : address register
        // register at reg_pos_db=1 : data register
        // register at reg_pos_db=2 : data register (in case of DW)
        // Note: Because reg_pos_db for this loop is incremented already, "reg_pos_db-1" is used.
        if (core_type == CORE_G4MH) {
            if ((inst_group == IGRP_ST) && (reg_pos_db != 1)) {
                if (active_cycle > (issue_cycle + rr_stage)) {
                    m_st_req_avail_cycle = std::max (m_st_req_avail_cycle, active_cycle);
                }
                //printf ("m_st_req_avail_cycle=%lu, active_cycle[%d]=%lu\n",
                //        m_st_req_avail_cycle, read_reg, active_cycle);
                ++ it_rreg;
                continue;
            }
        }

        InstGroupType reg_write_inst = m_reg_write_resource[read_reg];
        bool reg_write_inst_lram = m_reg_load_lram[read_reg];
        if (core_type != CORE_G4KH) {
            if ((inst_group != IGRP_FPU) && (reg_write_inst == IGRP_FPU)) {
                // FPU演算結果を非FPUで使用する場合はレイテンシ+1
                active_cycle += 1;
            } else if (    (inst_group == IGRP_FPU)
                        && (reg_write_inst != IGRP_FPU)
                        && (reg_write_inst != IGRP_LD) ) {
                // G4MHでは
                // ALU演算結果をFPUで使用する場合はレイテンシ+1
                  // ただし、LSU転送結果をFPUで使用する場合はレイテンシ+0なので注意
                active_cycle += 1;
            }
        }

#ifdef USE_STALL_COUNT
        SourceResType res;
        switch (reg_write_inst) {
        case IGRP_FPU: res = SRC_RES_FPU; break;
        case IGRP_LD:  res = SRC_RES_LD;  break;
        default:       res = SRC_RES_ALU; break;
        }
#endif // USE_STALL_COUNT

        if (core_type == CORE_G4KH) {
            if ((inst_group == IGRP_FPU) && (reg_write_inst_lram)) {
            //Latency +1 when LD operation result is used in FPU-instruction
                active_cycle += 1;
            }

             ArgFormat arg_format = MicroArchDB::GetArgFormat (trace->GetInstId());
             ArgFormat reg_arg_inst = m_arg_resource[read_reg];
             if ((inst_group == IGRP_FPU) && (reg_write_inst == IGRP_FPU) && (cnt_r_arg != 3) && (reg_arg_inst != ARG_R2_DR3)) {
                 active_cycle -= 1;
             }else if ((inst_group == IGRP_FPU) && (reg_write_inst == IGRP_FPU) && (arg_format == ARG_DR2_R3) && (reg_arg_inst == ARG_R2_DR3)) {
                 active_cycle -= 1;
             }
        }

        uint32_t reg_active_diff;
        if (core_type == CORE_G4KH) {
            issue_cycle = m_cs_cycle_count;
        }
        if (active_cycle > (issue_cycle + rr_stage)) {
            reg_active_diff = static_cast <uint32_t> (active_cycle - (issue_cycle + rr_stage));
#ifdef USE_STALL_COUNT
            m_stall_src[res] = std::max (m_stall_src[res], reg_active_diff);
#endif // USE_STALL_COUNT
        } else {
            reg_active_diff = 0;
        }

#ifdef DEBUG_CS
        CfMsg::TPrint (MSG_ERR, trace,
            "reg_active_diff=%u, cs_cycle_count=%lu, rr_stage=%u, active_cycle=%lu, m_reg_active_cycle[%d]=%lu\n",
            reg_active_diff, issue_cycle, rr_stage, active_cycle, read_reg, m_reg_active_cycle[read_reg]);
#endif // #ifdef DEBUG_CS

        /* 依存関係があればストール */
        if (reg_active_diff > stall_cycle) {
            stall_cycle = reg_active_diff;
        }

        if (core_type == CORE_G4KH) {
            if (inst_group == IGRP_MUL) {
               m_is_prev_mul = true ;
            }

            if (cnt_r_arg == 3) {
                m_is_3_arg = true;
            } else {
                m_is_3_arg = false;
            }

            // 直前先行命令とレジスタ依存あれば同時発行できない
            if (m_parallel_issue_flag == true) {
                it = find (m_dst_regs.begin(), m_dst_regs.end(), read_reg);
                if (it != m_dst_regs.end () && (*it != R0)) {
                    // 先行命令のDSTをSRCとして使用(RAWハザードあり）
                    if (core_type != CORE_G4MH) {
                        // 0latency Forwarding 対象なら依存を無視
                        if ( (m_is_prev_mov != true)
                          || (MicroArchDB::GetParaGroupG3KH (inst_id) == PGRP_1_3) ) {
                            // 先行命令がMOV系で自命令がJMP系以外の場合、
                            // 先行命令MOVの結果を使う後続命令は同時発行可
                                 // それ以外のケースでは全て同時発行不可
                            m_parallel_issue_flag = false;
                        }
                    } else {
                        m_parallel_issue_flag = false;
                    }
                }
            }
        } else {
            // 直前先行命令とレジスタ依存あれば同時発行できない
            // In case of Fusion, this isn't stall by internal forwarding
            if ((m_parallel_issue_flag == true) && (!IsFusion ())) {
                it = find (m_dst_regs.begin(), m_dst_regs.end(), read_reg);
                if (it != m_dst_regs.end () && (*it != R0)) {
                    // 先行命令のDSTをSRCとして使用(RAWハザードあり）
                    if (core_type != CORE_G4MH) {
                        // 0latency Forwarding 対象なら依存を無視
                        if ( (m_is_prev_mov != true)
                                || (MicroArchDB::GetParaGroup (inst_id) == PGRP_1_3) ) {
                         // 先行命令がMOV系で自命令がJMP系以外の場合、
                         // 先行命令MOVの結果を使う後続命令は同時発行可
                            // それ以外のケースでは全て同時発行不可
                        m_parallel_issue_flag = false;
                        }
                    } else {
                        m_parallel_issue_flag = false;
                    }
                }
            }
        }
        ++ it_rreg;
    }

    if (core_type == CORE_G4KH) {
        if (m_parallel_issue_flag == true){
            if (it == m_dst_regs.end ()&& (m_dst_regs.begin () != m_dst_regs.end ()) && (MicroArchDB::GetInstGroup (inst_id) != IGRP_AL)
                    && (MicroArchDB::GetInstGroup  (inst_id) != IGRP_SHI)){
                m_parallel_issue_flag = false;
             }else if ((it == m_dst_regs.end () || (read_reg == R0)) && (inst_group == IGRP_ST)){
                m_parallel_issue_flag = false;
             }
        }
    }

    // 先行命令のMOVフラグをクリア
    m_is_prev_mov = false;

    // CMPFからTRFSRへのFPSR.CC依存によるストール
    // CMPFとTRFSRの場合だけで、CMPFとCMOVFの組み合わせではストールしない。
    // @@ G3MHではストールしない（同時発行しないけど）。RAはTRFSRのPSW.CCのRAに影響
    if (m_fpcc_rr != 0) {
        if (m_fpcc_active_cycle > (issue_cycle + m_fpcc_rr)) {
            uint32_t reg_active_diff
                = static_cast<uint32_t> (  m_fpcc_active_cycle
                                         - (issue_cycle + m_fpcc_rr) );

#ifdef USE_STALL_COUNT
            m_stall_src[SRC_RES_FPU] = std::max (m_stall_src[SRC_RES_FPU], reg_active_diff);
#endif // USE_STALL_COUNT
            // 依存関係があればストール
            if (reg_active_diff > stall_cycle) {
                stall_cycle = reg_active_diff;
            }
        }
        m_fpcc_rr = 0;
    }

    if (core_type == CORE_G4KH) {
        m_is_prev_branch = m_is_branch;
        m_is_branch = false ;
        m_is_taken = false;

        if ((m_fsctrl->m_branch_type == BR_NOT) && (m_is_prev_branch == true)) {
               m_is_prev_branch = false ;
        }

        if ((m_is_prev_mul == true)
                && ((inst_group == IGRP_BRA) || (inst_group == IGRP_BIT)
                                             || (inst_group == IGRP_AL)
                                             || (inst_group == IGRP_ALC)
                                             || (inst_group == IGRP_SHI)
                                             || (inst_group == IGRP_SAT)
                                             || (inst_group == IGRP_LP)
                                             || (inst_group == IGRP_LOG)
                                             || (inst_group == IGRP_BSC)
                                             || (inst_group == IGRP_HT)
                                             || (inst_group == IGRP_HV)
                                             || ((MicroArchDB::GetCycleCalcType (inst_id) == CCT_LAT_DISP))
                                             || ((MicroArchDB::GetCycleCalcType (inst_id) == CCT_LAT_PREP))
                                             )
                && (MicroArchDB::GetParaGroupG3KH (inst_id) != PGRP_3_1)
                && (MicroArchDB::GetParaGroupG3KH (inst_id) != PGRP_3_2)
                && (MicroArchDB::GetParaGroupG3KH (inst_id) != PGRP_3_3)
            ){

            if (stall_cycle < 2) {
                stall_cycle = 2;
            }
        }
        if ((inst_group != IGRP_MUL) && (m_is_prev_mul == true)) {
            m_is_prev_mul = false ;
        }
    }

    if ((inst_group == IGRP_FPU) && (core_type != CORE_G4MH)) {
        // 以前のFPU命令のIssueRateによる影響
        if (m_fpu_issue_avail_cycle > m_cs_cycle_count) {
            uint32_t reg_active_diff
                = static_cast<uint32_t> (m_fpu_issue_avail_cycle - m_cs_cycle_count);

            // 以前のFPU命令のIssueRate期間が経過していなければストール
            if (reg_active_diff > stall_cycle) {
                stall_cycle = reg_active_diff;
            }
        }
    }

    // Check PSW.CC dependency
    if ((!IsFusion() && (core_type == CORE_G4MH))
        || (core_type == CORE_G4KH)) { // In Fusion, 1st coupled inst update PSW.
                        // Staii doesn't occur on 2nd fisioned instruction.
        PswDepType psw_dep = MicroArchDB::GetPswDepType (inst_id);
        if ((psw_dep & PSW_DEP_USE) == PSW_DEP_USE) {
            if (core_type == CORE_G4KH) {
                issue_cycle = m_cs_cycle_count;
            }
            if (m_pswcc_active_cycle > (issue_cycle + 1)) {
                uint32_t reg_active_diff
                    = static_cast<uint32_t> (m_pswcc_active_cycle - (issue_cycle + 1));
#ifdef USE_STALL_COUNT
                // TRFSRはFPUとしてカウント
                if (m_psw_write_res == IGRP_FPU) {
                    m_stall_src[SRC_RES_FPU] = std::max (m_stall_src[SRC_RES_FPU], reg_active_diff);
                } else {
                    m_stall_src[SRC_RES_ALU] = std::max (m_stall_src[SRC_RES_ALU], reg_active_diff);
                }
#endif // USE_STALL_COUNT
                // 依存関係があればストール
                if (reg_active_diff > stall_cycle) {
                    stall_cycle = reg_active_diff;
                }
            }
        }
    }
#ifdef USE_STALL_COUNT
    m_stall_counter[STALL_CNT_DATA_ALU]->Count (m_stall_src[SRC_RES_ALU]);
    m_stall_counter[STALL_CNT_DATA_LD]->Count (m_stall_src[SRC_RES_LD]);
    m_stall_counter[STALL_CNT_DATA_FPU]->Count (m_stall_src[SRC_RES_FPU]);
#endif //#ifdef USE_STALL_COUNT
    return stall_cycle;
}


template <CoreType core_type>
uint32_t CsCtrl::CalcDstRegStall (TraceCtrl* trace, uint64_t issue_cycle)
{
    // ストールさせるかもしれないので同時発行された先行命令のDSTレジスタを保存
    // 同時発行しないなら記録不要
    if (m_parallel_issue_flag == true) {
        m_reg_dst_prev = m_dst_regs;
    } else {
        m_reg_dst_prev.clear();
    }

    /* ディスティネーションレジスタへの書き込み管理テーブルの初期化 */
    m_dst_regs.clear ();

    /* 当該命令の全 RA に対する依存判定処理 */
    uint32_t stall_cycle = 0;
    InstGroupType inst_group = MicroArchDB::GetInstGroup (trace->GetInstId ());
    CRegVec::const_iterator it_wreg = trace->GetWregDepVec()->begin();
    CRegVec::const_iterator it_wend = trace->GetWregDepVec()->end();
    while (it_wreg != it_wend) {
        CReg write_reg = *it_wreg;

        // R0を含めないとArchDBの順番とずれてしまう。
        // しかし、R0を含めてしまうとR0の比較が必要になる。
        // 比較とR0なし/ありREGテーブルのどちらが速度コスト低い？
        /* ディスティネーションレジスタを記録 */
        m_dst_regs.push_back (write_reg);

        // R0は依存関係の対象外
        if (write_reg == R0) {
            ++ it_wreg;
            continue;
        }

        /* レジスタ出力依存判定 */
        // 先行命令のDSTと後続命令のDSTが同じ場合、
        // 後続はIDステージでstall（後続がMOVの場合も含む)
        uint64_t active_cycle = m_reg_active_cycle[write_reg];
        InstGroupType reg_write_inst = m_reg_write_resource[write_reg];
        if (core_type != CORE_G4KH) {
            if ((inst_group != IGRP_FPU) && (reg_write_inst == IGRP_FPU)) {
                // FPU演算結果を非FPUで使用する場合はレイテンシ+1
                active_cycle += 1;
            }
        }

        if (core_type == CORE_G4KH) {
             //G4KH c2b1
             if (m_parallel_issue_flag == true) {
                CRegVec::const_iterator it
                = find (m_reg_dst_prev.begin(), m_reg_dst_prev.end(), write_reg);
                if ((it == m_reg_dst_prev.end ())) {
                    m_parallel_issue_flag = false;
                }
             }
        }

        uint32_t reg_active_diff;
        if (core_type == CORE_G4KH) {
            issue_cycle = m_cs_cycle_count;
        }
        if (active_cycle > (issue_cycle + 1)) {
            reg_active_diff = static_cast<uint32_t> (active_cycle - (issue_cycle + 1));

            // もしも直前命令のDSTと自命令のDSTが同じなら同時発行しない
            // In case of Fusion, can issue in parallel.
            if (!IsFusion () || (core_type == CORE_G4KH)) {
                CRegVec::const_iterator it
                    = find (m_reg_dst_prev.begin(), m_reg_dst_prev.end(), write_reg);
                if (it != m_reg_dst_prev.end ()) {
                    m_parallel_issue_flag = false;
                }
            }
        } else {
            reg_active_diff = 0;
        }
        if (reg_active_diff > stall_cycle) {
            stall_cycle = reg_active_diff;
        }

        ++ it_wreg;
    }

    /* ストールサイクルを更新 */
    if (core_type != CORE_G4MH) {
        return stall_cycle;
    } else {
        // @@ G3MH ではWAW発生しないのでとりあえず０(Renameレジスタ不足したら？）
        return 0;
    }
}


uint32_t CsCtrl::CalcResourceStall (TraceCtrl* trace, uint64_t issue_cycle)
{
    uint32_t stall_cycle = 0;
    int32_t inst_id = trace->GetInstId();
    InstGroupType inst_group = MicroArchDB::GetInstGroup (inst_id);

    if (inst_group == IGRP_DV) {
        // 以前のDIVが完了するまで、後続のDIVはEXに進めない
        if (m_div_ra_cycle > (issue_cycle + 1)) {
            uint32_t reg_active_diff
                = static_cast<uint32_t> (m_div_ra_cycle - (issue_cycle + 1));

            // 依存関係があればストール
            if (reg_active_diff > stall_cycle) {
                stall_cycle = reg_active_diff;
            }
        }
    }

    if (m_divf_latency != 0) {
        if (m_divf_ra_cycle > (issue_cycle + 1)) {
            uint32_t reg_active_diff
                = static_cast<uint32_t> (m_divf_ra_cycle - (issue_cycle + 1));

            // 依存関係があればストール
            if (reg_active_diff > stall_cycle) {
                stall_cycle = reg_active_diff;
            }
        }
    }

    if (m_rmw_latency != 0) {
        if (m_rmw_ra_cycle > (issue_cycle + 1)) {
            uint32_t reg_active_diff
                = static_cast<uint32_t> (m_rmw_ra_cycle - (issue_cycle + 1));

            // 依存関係があればストール
            if (reg_active_diff > stall_cycle) {
                stall_cycle = reg_active_diff;
            }
        }
    }

    // G3MHではMULF.Dは全後続FPU命令の発行を止める
    if (inst_group == IGRP_FPU) {
        if (m_mulfd_ra_cycle > (issue_cycle + 1)) {
            uint32_t reg_active_diff
                = static_cast<uint32_t> (m_mulfd_ra_cycle - (issue_cycle + 1));

            // 依存関係があればストール
            if (reg_active_diff > stall_cycle) {
                stall_cycle = reg_active_diff;
            }
        }
    }

    if (m_core_type == CORE_G4KH) {
        // if mac_latency != 0, stall will apply in RA.
        if ((m_mac_latency == 0) && (MicroArchDB::GetResvResType (inst_id, 0) == RS_RES_INT)) {
            // mac -> mac or non integer inst(LSU/FPU/etc): repeat rate is 2.
            // mac -> integer : repeat rate is 4
            // mac will release EX resource +4 cycles later.(Normal inst releases 1cycle later.)
            if (m_mac_use_cycle > (issue_cycle + 1)) {
                uint32_t reg_active_diff
                    = static_cast<uint32_t> (m_mac_use_cycle - (issue_cycle + 1));

                // 依存関係があればストール
                if (reg_active_diff > stall_cycle) {
                    stall_cycle = reg_active_diff;
                }
            }
        }
    }

    return stall_cycle;
}


void CsCtrl::UpdateRegAvailStage (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId();

    m_reg_active_stage.clear ();

    /* 当該命令の全 RA に対する依存判定処理 */
    uint32_t reg_pos_db = 0;
    CRegVec::const_iterator it = m_dst_regs.begin ();
    while (it != m_dst_regs.end ()) {
        int32_t ra_stage;
        if (IsVarLatency () == true) {
            ra_stage = GetWregVarLatency (reg_pos_db);
        } else {
            ra_stage = MicroArchDB::GetRegDstStage (inst_id, reg_pos_db, m_archdb_id);
        }
        ++ reg_pos_db;

#ifdef CHECK_LACK_DBREG
        if (ra_stage == MicroArchDB::END_ENTRY) {
            // ここに来たら、ArchDBのレジスタ数＜traceのレジスタ数なのでArchDB異常
#ifdef DEBUG_CS
            CfMsg::TPrint (MSG_ERR, trace,
                "<Internal Error: num of dstreg in DB is less than num of trace (inst=%d)>\n",
                inst_id);
#endif // #ifdef DEBUG_CS
            m_reg_active_stage.push_back (0);
            ++ it;
            continue;
        }
#endif // CHECK_LACK_DBREG

        if ((*it) == R0) {
            m_reg_active_stage.push_back (0);
            ++ it;
            continue;
        }

        if (MicroArchDB::GetInstGroup (inst_id) == IGRP_FPU) {
            ra_stage = m_mod_fpu_latency;
        }

        /* レジスタ逆フロー依存判定 */
        if (IsZeroLatency () == true) {
            // MOV系の0latency-forward:同時発行される後続命令で結果を利用可能
            m_reg_active_stage.push_back (0);
            // 次命令が命令グループ1_3（JMP系)で、MOVの結果をソースとする場合の特殊処理用
            // @@ 命令パックで先行デコード対応したら書きなおせる
            m_is_prev_mov = true;
        } else {
            m_reg_active_stage.push_back (ra_stage);
        }

        ++ it;
    }
}


void CsCtrl::SetZeroLatency (bool is_zero_latency)
{ m_is_zero_latency = is_zero_latency; }

void CsCtrl::SetVarIssue (bool is_var_issue)
{ m_is_var_issue = is_var_issue; }

void CsCtrl::InitVarLatency (void)
{ m_wreg_var_latency.clear (); }

void CsCtrl::AddWregVarLatency (int32_t latency)
{ m_wreg_var_latency.push_back (latency); }

void CsCtrl::SetVarIssueRate (int32_t issue_rate)
{
    m_issue_var_rate = issue_rate;
    SetVarIssue (true);
}


bool CsCtrl::IsZeroLatency (void) const
{ return m_is_zero_latency; }

bool CsCtrl::IsVarLatency (void) const
{ return ! (m_wreg_var_latency.empty ()); }

bool CsCtrl::IsVarIssue (void) const
{ return m_is_var_issue; }

int32_t CsCtrl::GetWregVarLatency (int32_t pos) const
{ return m_wreg_var_latency[pos]; }

int32_t CsCtrl::GetWregVarCount (void) const
{ return static_cast<int32_t> (m_wreg_var_latency.size ()); }

int32_t CsCtrl::GetVarIssueRate (void) const
{ return m_issue_var_rate; }


// レジスタ/IssueRateなどの特殊計算
void CsCtrl::UpdateCsInstFlag (TraceCtrl* trace)
{
    SetZeroLatency (false);
    InitVarLatency ();
    SetVarIssue (false);
    SetAtomic (false);
    m_use_2rs = false;
#ifdef SUPPORT_SC_HEAP
    SetLoadIncDec (false);
#endif // SUPPORT_SC_HEAP

    int32_t inst_id = trace->GetInstId();
    CycleCalcType calc_type = MicroArchDB::GetCycleCalcType (inst_id);
    if (calc_type != CCT_NORM) {
        switch (calc_type) {
        case CCT_LAT_DISP:
        case CCT_LAT_POP:
        case CCT_LAT_RESBANK:
        case CCT_LAT_SW:
        case CCT_LAT_CALL:
        case CCT_VLD_INDEX:
        case CCT_CACHE:
            // LDM/STM(except STM.GSR) is called both UpdateCsInstFlag and UpdateInstMemFlag for sync.
            // CalcLsuLatencyEachを使用する関数は後で処理。
            break;
        default:
            (this->*m_inst_cs[calc_type]) (trace);
            break;
        }
    }
}


// 特殊命令のフラグ設定（CalcLsuLatencyEachを内部で使用する関数用)
// CalcLsuLatencyEach()が内部でm_cs_cycle_countを使用するので、
// IDステージ確定後でないと駄目。分ける。
// @@ STのストールに対応した際は、PUSH/Prepareもここに追加する必要ありそう。
//    あまり量が増えると負荷が気になるので、やり方を考えるべき。
void CsCtrl::UpdateInstMemFlag (TraceCtrl* trace)
{
#ifdef SUPPORT_SC_HEAP
    m_suspend_ctrl->SetLsuRepeatCount (0);
    m_is_read_finished = false;
#endif //#ifdef SUPPORT_SC_HEAP

    int32_t inst_id = trace->GetInstId();
    CycleCalcType calc_type = MicroArchDB::GetCycleCalcType (inst_id);
    if (calc_type != CCT_NORM) {
        switch (calc_type) {
        case CCT_LAT_DISP:
        case CCT_LAT_POP:
        case CCT_LAT_RESBANK:
        case CCT_LAT_SW:
        case CCT_LAT_CALL:
        case CCT_VLD_INDEX:
        case CCT_RMW:
        case CCT_CACHE:
        case CCT_SYNC: // SYNC is called both UpdateCsInstFlag and UpdateInstMemFlag.
        case CCT_STMMP: // No need to call for STM.GSR.
        case CCT_LDM:
            (this->*m_inst_cs[calc_type]) (trace);
            break;
        default:
            break;
        }
    }
}


void CsCtrl::CsBUncond (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();
    if ((opcode & 0x7) == BR_CC_ALWAYS) {
        trace->SetParaGroupSwitched (true);
    }
}


void CsCtrl::CsLdstMpu (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();
    uint32_t selid = (opcode >> 27) & 0x1F;
    int32_t inst_id = trace->GetInstId();
    if (m_core_type != CORE_G4MH) {
        // G3KH2.0:MPUレジスタなら命令グループ変化
        if ( (selid >= 5) && (selid <= 7) ) {
            trace->SetParaGroupSwitched (true);
        }
    } else {
        // G4MH:SelID=0/1/2/3/5/10 has different
        switch (selid) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 10:
        case 13:
            // Use issue rate in ArchDB
            // Count PMC
            if (MicroArchDB::GetIssueRate (inst_id, m_archdb_id) > 1) {
                m_issue_stall_pmc += (MicroArchDB::GetIssueRate (inst_id, m_archdb_id) - 1);
            }
            break;
        default:
            // issue=1 except SELID=0/1/2/3/5/10
            SetVarIssueRate (1);
            break;
        }
    }
}


void CsCtrl::CsMov (TraceCtrl* trace)
{
    if (m_core_type != CORE_G4MH) {
        SetZeroLatency (true);
    }
}


void CsCtrl::CsMovR0 (TraceCtrl* trace)
{
    if (m_core_type != CORE_G4MH) {
        uint64_t opcode = trace->GetInstCode ();
        uint32_t reg1 = (opcode & 0x1F) + R0;
        if (reg1 == R0) {
            SetZeroLatency (true);
        }
    }
}


void CsCtrl::CsDisposePre (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();

    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t reglist = l20 | l24 | l28 | l30 | l31;

    bool is_64bit_aligned = false;
    bool check_first_reg = false;

    if (m_core_type == CORE_G4KH) {
        if (trace->GetRmemCount () != 0) {
            bool is_tgt_tcm = false;
            PhysAddr tgt_paddr = trace->GetRmemAddr (0);

            // 自PEのTCMとそれ以外を区別
            MemResourceInfo res_info;
    #ifdef CLUSTER
            p_internals->GetMemResourceInfo (m_pe_id, tgt_paddr, DATA_READ)->
                GetResourceInfo (&res_info, tgt_paddr);
    #else // CLUSTER
            p_internals->GetMemResourceInfo (m_pe_id, tgt_paddr)->
                GetResourceInfo (&res_info, tgt_paddr);
    #endif // CLUSTER

            if ((res_info.pe_read_latency == 0) && (res_info.pe_location == m_pe_id)) {
                is_tgt_tcm = true;
            }

            if ((tgt_paddr & 0x4U) == 0) {
                is_64bit_aligned = true;
            } else if (is_tgt_tcm) {
                // 対象がTCMの場合、内部バッファにより64bit境界以外でも2reg/cycle転送するので
                // 64bit境界転送とみなす。
                is_64bit_aligned = true;
                check_first_reg = true; // 2reg転送ならRAを+1するのでフラグをセット
            }
        }
    }

    m_mld_info.m_is_valid = true;
    m_mld_info.m_mld_type = MultiLoadInfo::MLD_DISPOSE;
    m_mld_info.m_is_scheap = IsTargetScheap (DATA_READ, trace);
    m_mld_info.m_is_64bit_aligned = is_64bit_aligned;
    m_mld_info.m_reglist = reglist;
    m_mld_info.m_pop_num = 0;
    m_mld_info.m_check_first_reg = check_first_reg;
    m_mld_info.m_mem_latency = 0;
    m_mld_info.m_last_issue_wait = 0;
    m_mld_info.m_dst_reg_latency = MicroArchDB::GetRegDstStage (trace->GetInstId(), 0, m_archdb_id);

#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    CsDisposeMain (trace);
#endif // SUPPORT_SC_HEAP
}


void CsCtrl::CsDisposeMain (TraceCtrl* trace)
{
#ifdef SUPPORT_SC_HEAP
    if (m_mld_info.m_is_scheap) {
        CsDisposeScheap (trace);
        return;
    }
#endif // SUPPORT_SC_HEAP

    // G3M
    // Disposeでは2個単位でレジスタに書きこむ(2n,2n+1)
    // 発行レート: base(2/4(jmp)) + 書き込み回数
    // レジスタSP：base(2) + 発行レート
    // レジスタ：base(4) + 書き込み回数

    // G3MH
    // Disposeではレジスタ番号に関係なく2個単位でレジスタに書きこむ(2n,2n+1でなくてもOK)
    // 発行レート: 1 + 書き込み回数
    // レジスタSP：発行レート
    // レジスタ：  書き込み回数
    bool is_64bit_aligned = m_mld_info.m_is_64bit_aligned;
    bool check_first_reg = m_mld_info.m_check_first_reg;
    uint32_t dst_reg_latency = m_mld_info.m_dst_reg_latency;
    uint32_t last_issue_wait = m_mld_info.m_last_issue_wait;
    uint32_t pop_num = m_mld_info.m_pop_num;
    uint32_t reglist = m_mld_info.m_reglist;
    if (m_core_type != CORE_G4MH) {
        while (reglist != 0) {
            if (reglist & 0x3) {
                if (IsLsuBufEmptyRecheck (trace) == false) {
                    // LSUに空きがないので中断
                    m_mld_info.m_reglist = reglist;
                    m_mld_info.m_check_first_reg = check_first_reg;
                    m_mld_info.m_dst_reg_latency = dst_reg_latency;
                    m_mld_info.m_last_issue_wait = last_issue_wait;
                    m_mld_info.m_pop_num = pop_num;
                    return;
                }
                bool is_2reg_transfer = ((reglist & 0x3) == 0x3) && (is_64bit_aligned);
                if ((check_first_reg) && (is_2reg_transfer)) {
                    // 非64bitアラインで、TCMに対する最初のレジスタ転送が2reg転送ならRAを+1する
                    ++ dst_reg_latency;
                }
                check_first_reg = false;

                last_issue_wait = GetStallByMemBufFullG3M (trace);
                uint32_t mem_latency = CalcLsuLatencyEach<DATA_READ> (trace, pop_num);
    #ifdef SUPPORT_SC_HEAP
                // 初回がCForest内メモリで、2回目以降でSC-HEAP側メモリにアクセス。
                // dummyレイテンシとして、待ち合わせ/書き戻しはしない。警告を表示しておく。
                if (mem_latency == TARGET_IS_SCHEAP) {
                    mem_latency = LATENCY_SCHEAP;
                    CfMsg::TPrint (MSG_WAR, trace,
                                   "<Warning detect: Dispose accessed 2 different memory area>\n");
                }
    #endif // SUPPORT_SC_HEAP
                // メモリレイテンシの基準はm_cs_cycle_count。
                // 各メモリアクセスの発行タイミングはpop数ごとに+1されるのでその分を引く
                if (pop_num <= mem_latency) {
                    mem_latency -= pop_num;
                } else {
                    mem_latency = 0;
                }

                if (is_2reg_transfer) {
                    AddWregVarLatency (dst_reg_latency + mem_latency);
                }
                AddWregVarLatency (dst_reg_latency + mem_latency);
                ++ dst_reg_latency;
                ++ pop_num;
            }
            if (is_64bit_aligned) {
                reglist >>= 2;
            } else {
                reglist >>= 1;
            }
        }
    } else {
        while (reglist != 0) {
            while ((reglist & 0x1) == 0) {
                reglist >>= 1;
            }
            if (IsLsuBufEmptyRecheck (trace) == false) {
                // LSUに空きがないので中断
                m_mld_info.m_reglist = reglist;
                m_mld_info.m_dst_reg_latency = dst_reg_latency;
                m_mld_info.m_last_issue_wait = last_issue_wait;
                m_mld_info.m_pop_num = pop_num;
                return;
            }
            bool is_2reg_transfer = false;
            if (reglist != 1) {
                // find second reg
                reglist >>= 1;
                while ((reglist & 0x1) == 0) {
                    reglist >>= 1;
                }
                is_2reg_transfer = true;
            }

            last_issue_wait = GetStallByMemBufFullG3MH (trace, trace->GetStageCycle (STAGE_EA));
            uint32_t mem_latency = CalcLsuLatencyEach<DATA_READ> (trace, pop_num);
#ifdef SUPPORT_SC_HEAP
            // 初回がCForest内メモリで、2回目以降でSC-HEAP側メモリにアクセス。
            // レイテンシ0として、待ち合わせ/書き戻しはしない。警告を表示しておく。
            if (mem_latency == TARGET_IS_SCHEAP) {
                mem_latency = 0;
                CfMsg::TPrint (MSG_WAR, trace,
                                "<Warning detect: Dispose accessed 2 different memory area>\n");
                m_cs_lsu_buf->ReleaseScheapWaiting (trace->GetStageCycle (STAGE_EA) + m_halt_cycle + dst_reg_latency);
            }
#endif // SUPPORT_SC_HEAP
            {
                if (mem_latency >= (STAGE_WB - STAGE_EA)) {
                    mem_latency -= STAGE_WB - STAGE_EA;
                } else {
                    mem_latency = 0;
                }
            }
            // メモリレイテンシの基準はSTAGE_EA。
            // 各メモリアクセスの発行タイミングはpop数ごとに+1されるのでその分を引く
            if (pop_num <= mem_latency) {
                mem_latency -= pop_num;
            } else {
                mem_latency = 0;
            }

            if (is_2reg_transfer) {
                AddWregVarLatency (dst_reg_latency + mem_latency);
            }
            AddWregVarLatency (dst_reg_latency + mem_latency);
            ++ dst_reg_latency;
            ++ pop_num;
            reglist >>= 1;
        }
    }
    m_mld_info.m_last_issue_wait = last_issue_wait;
    m_mld_info.m_pop_num = pop_num;
    CsDisposePost (trace);
}


void CsCtrl::CsDisposePost (TraceCtrl* trace)
{
    // メモリアクセスの発行が遅れた場合、命令発行もストール
    uint32_t base_latency = std::max (m_mld_info.m_pop_num, m_mld_info.m_last_issue_wait);

    int32_t inst_id = trace->GetInstId ();
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id) + base_latency;
    int32_t dst_sp_latency = MicroArchDB::GetRegDstStage (inst_id, 1, m_archdb_id) + base_latency;
    //CfMsg::TPrint (MSG_ERR, trace, "pop_num=%u, last_issue=%u, issue_rate=%d, dst_sp=%u\n",
    //               m_mld_info.m_pop_num, m_mld_info.m_last_issue_wait, issue_rate, dst_sp_latency);
    // 分岐付きDispose reg1では
    // Disposeによるreg1変更レイテンシが分岐先とSPレイテンシに加算される
    if ((trace->IsBranched () == true) && (m_mld_info.m_pop_num != 0)) {
        CReg reg1 = OperandExtract (trace->GetInstCode (), 20, 16) + R0;
        int32_t i = 0;
        CRegVec::const_iterator it_wreg = trace->GetWregDepVec()->begin();
        CRegVec::const_iterator it_wend = trace->GetWregDepVec()->end();
        if (reg1 != R3) {
            while (it_wreg != it_wend) {
                if (reg1 == *it_wreg) {
                    // reg1がDSTレジスタに含まれる。
                    // SP書込より2cycle早くreg1を用意出来れば、自己書込ストールなし
                    int32_t stall_reg1 = GetWregVarLatency (i) + 2 - dst_sp_latency;
                    if (stall_reg1 > 0) {
                        // 分岐先とSPを遅らせる（SP以外のDSTレジスタは遅れない）
                        issue_rate += stall_reg1;
                        dst_sp_latency += stall_reg1;
                    }
                    break;
                }
                ++ i;
                ++ it_wreg;
            }
        }
    }

    if (m_core_type == CORE_G4MH) {
        if (trace->IsBranched () == false) {
            SetVarIssueRate (issue_rate);
        } else if (m_last_bp_result != BP_HIT) {
            //printf("1:m_branch_target_pos=%u\n", m_branch_target_pos);
            m_branch_target_pos += m_mld_info.m_pop_num;
        } else {
            //printf("2:m_branch_target_pos=%u, issue_rate=%u\n", m_branch_target_pos, issue_rate);
            SetVarIssueRate (issue_rate);
        }
    } else {
        SetVarIssueRate (issue_rate);
    }

    AddWregVarLatency (dst_sp_latency);
    m_mld_info.m_is_valid = false;
}


#ifdef SUPPORT_SC_HEAP
void CsCtrl::CsDisposeScheap (TraceCtrl* trace)
{
    // @@ 現時点ではSC-HEAP領域へのDisposeは不要。SC-HEAP側にメモリついたら対応必要。
    //    現在は機能優先で、サイクル精度無視。
    // SC-HEAP領域へのアクセスの場合、FS側の動作に合わせ、常に1アクセス32bitで対応
    uint32_t dst_reg_latency = m_mld_info.m_dst_reg_latency;
    uint32_t last_issue_wait = m_mld_info.m_last_issue_wait;

    int32_t acc_cnt = trace->GetRmemCount ();
    CRegVec::const_iterator it_wreg = trace->GetWregDepVec()->begin();
    std::advance (it_wreg, m_mld_info.m_pop_num);
    for (int32_t i = m_mld_info.m_pop_num; i < acc_cnt; i++) {
        if (IsLsuBufEmptyRecheck (trace) == false) {
            // LSUバッファに空きがないので中断
            m_mld_info.m_pop_num = i;
            m_mld_info.m_last_issue_wait = last_issue_wait;
            m_mld_info.m_dst_reg_latency = dst_reg_latency;
            return;
        }
        last_issue_wait = GetStallByMemBufFullG3MH (trace, trace->GetStageCycle (STAGE_EA));
        uint32_t mem_latency = CalcLsuLatencyEach<DATA_READ> (trace, i);
        if (mem_latency == TARGET_IS_SCHEAP) {
            m_suspend_ctrl->RegisterLoadDependency (trace, i, *it_wreg, SuspendCtrl::NO_DEP_ISSUE,
                                                    trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
            mem_latency = 0;
        } else {
            // 初回がSC-HEAP側メモリで、2回目以降でCForest内メモリにアクセス。
            // 2個以上の領域にまたがってアクセスすることは考えにくいので、警告メッセージ表示
            CfMsg::TPrint (MSG_WAR, trace,
                           "<Warning detect: Dispose accessed 2 different memory area>\n");
        }
        AddWregVarLatency (dst_reg_latency + mem_latency);
        ++ dst_reg_latency;
        ++ it_wreg;
    }

    // 2access/1cycle
    if (acc_cnt > 0) {
        acc_cnt = (acc_cnt >> 1) + (acc_cnt & 1);
    }

    m_mld_info.m_last_issue_wait = last_issue_wait;
    m_mld_info.m_pop_num = acc_cnt;
    CsDisposePost (trace);
}
#endif // SUPPORT_SC_HEAP


void CsCtrl::CsPrepare (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();

    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t reglist = l20 | l24 | l28 | l30 | l31;

    // prepareでは4個単位でレジスタを書きだす(4n,...,4n+3)
    // ただし、非対象のレジスタが4n~4n+3の中にあると、アクセスが分かれる
    // 書き込み対象レジスタを示す4bitフィールドが、0なら0回、
    // 0x1/2/3/4/6/7/8/C/E/Fなら1回、0x5/9/A/B/Dだと2回に分けて書き出し。
    // マニュアルでは最大レイテンシ2+1~3になっているが嘘で、2+1~6が正解
    // 発行レート: base(2) + 書き出し回数
    // レジスタSP：base(2) + 書き出し回数
    // レジスタEP：base(2) + 書き出し回数
    // ただし、レジスタ指定数0だったら発行レート1、SPレイテンシ1
    int32_t dst_sp_latency;
    int32_t issue_rate;
    if (reglist == 0) {
        // レジスタ指定がない
        issue_rate = 1;
        dst_sp_latency = 1;
    } else {
        if (m_core_type != CORE_G4MH) {
              // レジスタ数に応じてレイテンシを設定
            uint32_t push_num = 0;
            while (reglist != 0) {
                switch (reglist & 0xF) {
                case 0x0:
                    break;
                case 0x5:
                case 0x9:
                case 0xA:
                case 0xB:
                case 0xD:
                    push_num += 2;
                    break;
                default:
                    ++ push_num;
                    break;
                }
                reglist >>= 4;
            }
            int32_t inst_id = trace->GetInstId();
            issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id) + push_num;
            dst_sp_latency  = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id) + push_num;
        } else {
              // レジスタ数に応じてレイテンシを設定
            uint32_t push_num = 0;
            uint32_t num = 0;
            while (reglist != 0) {
                while ((reglist & 0x1) == 0) {
                    reglist >>= 1;
                }
                ++ num;
                reglist >>= 1;
            }
            -- num; // 1レジスタ分は同時実行1cycleで終わるので引いておく
            push_num = (num >> 1) + (num & 1);
            int32_t inst_id = trace->GetInstId();
            issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id) + push_num;
            dst_sp_latency  = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id) + push_num;
        }
    }
    SetVarIssueRate (issue_rate);
    AddWregVarLatency (dst_sp_latency); // for SP

    // EPなしprepareでは無駄な設定だが、悪影響ないので一律設定
    AddWregVarLatency (dst_sp_latency); // for EP
}


void CsCtrl::CsPopspPre (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();
    uint32_t rh = OperandExtract (opcode,  4,  0);
    uint32_t rt = OperandExtract (opcode, 31, 27);

    m_mld_info.m_is_valid = true;
    m_mld_info.m_mld_type = MultiLoadInfo::MLD_POPSP;
    m_mld_info.m_is_scheap = IsTargetScheap (DATA_READ, trace);

    m_mld_info.m_pop_num = 0;
    m_mld_info.m_check_first_reg = false;
    m_mld_info.m_dst_reg_latency = MicroArchDB::GetRegDstStage (trace->GetInstId(), 0, m_archdb_id);
    m_mld_info.m_cur_pos = rt;
    m_mld_info.m_end_pos = rh;

#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    CsPopspMain (trace);
#endif // SUPPORT_SC_HEAP
}


void CsCtrl::CsPopspMain (TraceCtrl* trace)
{
#ifdef SUPPORT_SC_HEAP
    if (m_mld_info.m_is_scheap) {
        CsPopspScheap (trace);
        return;
    }
#endif //#ifdef SUPPORT_SC_HEAP

    // 命令発行レート：base(2)+レジスタ書き込み回数(最低1)
    // レジスタレイテンシ：R3=base(2)+レジスタ書き込み回数(最低1)
    // レジスタレイテンシ：Rn=base(11)+レジスタ書き込み回数(最低1)
    // レジスタ2n, 2n+1がセットで1度に書き込まれる
    uint32_t pop_num = m_mld_info.m_pop_num;
    uint32_t cur = m_mld_info.m_cur_pos;
    uint32_t end = m_mld_info.m_end_pos;
    uint32_t dst_reg_latency = m_mld_info.m_dst_reg_latency;
    while (cur >= end) {
        if (IsLsuBufEmptyRecheck (trace) == false) {
            // 中断
            m_mld_info.m_pop_num = pop_num;
            m_mld_info.m_cur_pos = cur;
            return;
        }
        uint32_t mem_latency = CalcLsuLatencyEach<DATA_READ> (trace, pop_num);
#ifdef SUPPORT_SC_HEAP
        if (mem_latency == TARGET_IS_SCHEAP) {
            // 初回がCForest内メモリで、2回目以降でSC-HEAP側メモリにアクセス。
            // レイテンシ0として、待ち合わせ/書き戻しはしない。警告を表示しておく。
            mem_latency = 0;
            CfMsg::TPrint (MSG_WAR, trace,
                           "<Warning detect: Dispose accessed 2 different memory area>\n");
            m_cs_lsu_buf->ReleaseScheapWaiting (trace->GetStageCycle (STAGE_EA) + m_halt_cycle + dst_reg_latency + pop_num);
        }
#endif // SUPPORT_SC_HEAP
        ++ pop_num;

        // ベースレイテンシとしてpop_numを加算するので、レイテンシからpop_num分を引く。
        if (pop_num < mem_latency) {
            mem_latency -= pop_num;
        } else {
            mem_latency = 0;
        }

        // 1レジスタ分を転送
        if ((cur != R3) || (m_core_type == CORE_G4MH)) {
            //! R3(SP)へは格納されない
            AddWregVarLatency (dst_reg_latency + pop_num + mem_latency);
        }
        if (cur == R0) {
            break;
        }

        // 可能であれば２レジスタ目も転送
        // @@ RTLsimのログを見ると、rt（開始cur)が偶数だと２レジスタ転送されない様に見える
        if ( ((cur & 0x1) == 1) && (cur > end) ) {
            -- cur;
            if ((cur != R3) || (m_core_type == CORE_G4MH)) {
                //! R3(SP)へは格納されない
                AddWregVarLatency (dst_reg_latency + pop_num + mem_latency);
            }
            if (cur == R0) {
                break;
            }
        }
        -- cur;
    }
    m_mld_info.m_pop_num = pop_num;
    CsPopspPost (trace);
}


void CsCtrl::CsPopspPost (TraceCtrl* trace)
{
    uint32_t pop_num = m_mld_info.m_pop_num;
    if (pop_num != 0) {
        int32_t inst_id = trace->GetInstId ();
        int32_t dst_sp_latency  = MicroArchDB::GetRegDstStage (inst_id, 1, m_archdb_id);
        AddWregVarLatency (dst_sp_latency + pop_num);
        int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
        SetVarIssueRate (issue_rate + pop_num);
    } else if (m_core_type == CORE_G4MH) {
        // G3MHではPOPなしでもSP更新。ArchDBの値を使用する。
        int32_t inst_id = trace->GetInstId ();
        int32_t dst_sp_latency  = MicroArchDB::GetRegDstStage (inst_id, 1, m_archdb_id);
        AddWregVarLatency (dst_sp_latency);
        int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
        SetVarIssueRate (issue_rate);
    } else {
        uint32_t mem_latency = 0; // No mem access
        uint32_t dst_reg_latency = m_mld_info.m_dst_reg_latency;
        AddWregVarLatency (dst_reg_latency + mem_latency);
        // レジスタ書き込みなしなら発行レート1
        SetVarIssueRate (1);
    }
    m_mld_info.m_is_valid = false;
}


#ifdef SUPPORT_SC_HEAP
void CsCtrl::CsPopspScheap (TraceCtrl* trace)
{
    // SC-HEAP領域へのアクセスの場合、FS側の動作に合わせ、常に1アクセス32bitで対応
    // サイクル数に誤差を生じるが、SC-HEAP対象領域へのPopspは頻度少ないはず。
    uint32_t cur = m_mld_info.m_cur_pos;
    uint32_t end = m_mld_info.m_end_pos;

    if (end <= cur) {
        int32_t inst_id = trace->GetInstId();
        int32_t dst_reg_latency  = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
        int32_t acc_cnt = trace->GetRmemCount ();
        for (int32_t i = m_mld_info.m_pop_num; i < acc_cnt; i++) {
            if (IsLsuBufEmptyRecheck (trace) == false) {
                // 中断
                m_mld_info.m_pop_num = i;
                m_mld_info.m_cur_pos = cur;
                return;
            }

            uint32_t mem_latency = CalcLsuLatencyEach<DATA_READ> (trace, i);
            if (mem_latency == TARGET_IS_SCHEAP) {
                if (cur != R3) {
                    m_suspend_ctrl->RegisterLoadDependency (trace, i, cur,
                                                            SuspendCtrl::NO_DEP_ISSUE,
                                                            trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
                } else {
                    // R3ならレジスタ書き込み無視。
                    // Readトランザクションに対する待ち合わせだけ設定したいので、R0と偽って登録
                    m_suspend_ctrl->RegisterLoadDependency (trace, i, R0, SuspendCtrl::NO_DEP_ISSUE,
                                                            trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
                }
                mem_latency = 0;
            } else {
                // 初回がSC-HEAP側メモリで、2回目以降でCForest内メモリにアクセス。
                // 2個以上の領域にまたがってアクセスすることは考えにくいので、警告メッセージ表示
                CfMsg::TPrint (MSG_WAR, trace,
                               "<Warning detect: Popsp accessed 2 different memory area>\n");
            }
            AddWregVarLatency (dst_reg_latency + i + mem_latency);

            if (cur == R0) {
                break;
            }
            -- cur;
        }
        m_mld_info.m_pop_num = acc_cnt;
    } else {
        m_mld_info.m_pop_num = 0;
    }

    CsPopspPost (trace);
}
#endif //#ifdef SUPPORT_SC_HEAP


void CsCtrl::CsPushsp (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();
    uint32_t rh = OperandExtract (opcode,  4,  0);
    uint32_t rt = OperandExtract (opcode, 31, 27);

    // pushでは4個づつレジスタライト(4n,...,4n+3)
    // 発行レート: base(2) + レジスタ書き込み回数(最低1)
    // レジスタSP：base(2) + レジスタ書き込み回数(最低1)
    int32_t inst_id = trace->GetInstId();
    int32_t push_num = 1;
    if (rh <= rt) {
        if (m_core_type != CORE_G4MH) {
            // 4nのnを切りだして、ライト回数を算出
            push_num = rt - rh + 1;
            int32_t dst_sp_latency  = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
            AddWregVarLatency (dst_sp_latency + push_num);
            int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
            SetVarIssueRate (issue_rate + push_num);
        } else {
            uint32_t num = (rt - rh) + 1 - 1; // -1 はベースの1cycleで同時実行可能な分
            push_num = (num >> 1) + (num & 1);
            int32_t dst_sp_latency  = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
            AddWregVarLatency (dst_sp_latency + push_num);
            int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
            SetVarIssueRate (issue_rate + push_num);
        }
    } else {
        // 書き込みなしなら発行レート1
        SetVarIssueRate (1);
    }
}


void CsCtrl::CsResbankPre (TraceCtrl* trace)
{
    m_mld_info.m_is_valid = true;
    m_mld_info.m_mld_type = MultiLoadInfo::MLD_RESBANK;
    m_mld_info.m_is_scheap = IsTargetScheap (DATA_READ, trace);
    m_mld_info.m_pop_num = 0;
    m_mld_info.m_dst_reg_latency = MicroArchDB::GetRegDstStage (trace->GetInstId(), 0, m_archdb_id);
    m_mld_info.m_cur_pos = 0;
    m_mld_info.m_end_pos = 0;

#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    CsResbankMain (trace);
#endif // SUPPORT_SC_HEAP
}


void CsCtrl::CsResbankMain (TraceCtrl* trace)
{
    uint32_t cur = m_mld_info.m_cur_pos;
    uint32_t dst_latency = m_mld_info.m_dst_reg_latency;
    uint32_t rmem_cnt = trace->GetRmemCount ();
    uint32_t issue_offset = cur >> 1;
    uint32_t mem_latency = 0;
    while (cur < rmem_cnt) {
        if (IsLsuBufEmptyRecheck (trace) == false) {
            // 中断
            m_mld_info.m_cur_pos = cur;
            return;
        }
        mem_latency = CalcLsuLatencyEach<DATA_READ> (trace, cur);
        issue_offset = cur >> 1;
#ifdef SUPPORT_SC_HEAP
        if (mem_latency == TARGET_IS_SCHEAP) {
            if (! m_mld_info.m_is_scheap) {
                // 初回がCForest内メモリで、2回目以降でSC-HEAP側メモリにアクセス。
                // レイテンシ0として、待ち合わせ/書き戻しはしない。警告を表示しておく。
                CfMsg::TPrint (MSG_WAR, trace,
                               "<Warning detect: Dispose accessed 2 different memory area>\n");
                m_cs_lsu_buf->ReleaseScheapWaiting (trace->GetStageCycle (STAGE_EA) + m_halt_cycle + dst_latency + issue_offset);
            } else {
                CReg dst_reg = FindResbankDstreg (trace, cur);
                m_suspend_ctrl->RegisterLoadDependency (trace, cur, dst_reg,
                                                        SuspendCtrl::NO_DEP_ISSUE,
                                                        trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
            }
            mem_latency = 0;
        } else if (m_mld_info.m_is_scheap) {
            // 初回がSC-HEAPで、2回目以降でCForestメモリにアクセス。
            // 警告を表示しておく。
            CfMsg::TPrint (MSG_WAR, trace,
                           "<Warning detect: Dispose accessed 2 different memory area>\n");
        }
#endif // SUPPORT_SC_HEAP
        AddWregVarLatency (dst_latency + issue_offset + mem_latency);
        ++ cur;
    }

    // Last load register is PC
    // Update branch target pos
    m_branch_target_pos = dst_latency + issue_offset + mem_latency;

    m_mld_info.m_cur_pos = cur;
    CsResbankPost (trace);
}


void CsCtrl::CsResbankPost (TraceCtrl* trace)
{
    int32_t issue_rate = MicroArchDB::GetIssueRate (trace->GetInstId (), m_archdb_id);
    uint32_t load_num = m_mld_info.m_cur_pos;
    // When MD=1 (load_num=35), issue rate is +5 than MD=0(load_num=24)
    if (load_num > 24) {
        issue_rate += 5;
    }
    SetVarIssueRate (issue_rate);
    m_mld_info.m_is_valid = false;
}


#ifdef SUPPORT_SC_HEAP
CReg CsCtrl::FindResbankDstreg (TraceCtrl* trace, uint32_t dst_cnt) const
{
    uint32_t max_cnt = trace->GetWregCount ();
    uint32_t greg_cnt = 0;
    for (uint32_t i = 0; i < max_cnt; ++i) {
        CReg reg = trace->GetWriteReg (i);
        if ((reg >= R1) && (reg <= R31)) {
            if (dst_cnt == greg_cnt) {
                return reg;
            }
            ++ greg_cnt;
        }
    }
    return R0; // for sysreg
}
#endif //#ifdef SUPPORT_SC_HEAP


void CsCtrl::CsSwitch (TraceCtrl* trace)
{
    // Switch命令は分岐先アドレスをメモリから取得するので、
    // 分岐先フェッチタイミングにデータアクセスレイテンシを加算する必要がある
    int32_t inst_id = trace->GetInstId();
    AddWregVarLatency (MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id)); // for PC

    // メモリレイテンシをIssueRateに加算
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
    uint32_t lsu_latency = CalcLsuLatencyEach<DATA_READ> (trace, 0);

#ifdef SUPPORT_SC_HEAP
    if (lsu_latency ==  TARGET_IS_SCHEAP) {
        lsu_latency = 0;
        m_suspend_ctrl->RegisterLoadDependency (trace, 0, SuspendCtrl::DEP_ISSUE,
                                                SuspendCtrl::HAS_DEP_ISSUE,
                                                trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
    }
#endif // SUPPORT_SC_HEAP
    if (m_core_type == CORE_G4MH) {
        if (m_last_bp_result != BP_HIT) {
            //printf("1:m_branch_target_pos=%u, lsu_latency=%u\n", m_branch_target_pos, lsu_latency);
            m_branch_target_pos += lsu_latency;
        } else {
            //printf("2:m_branch_target_pos=%u, lsu_latency=%u\n", m_branch_target_pos, lsu_latency);
            SetVarIssueRate (3 + lsu_latency + m_length_if_to_id);
        }
    } else {
        SetVarIssueRate (issue_rate + lsu_latency + 1);
    }
}


void CsCtrl::CsCall (TraceCtrl* trace)
{
    // 分岐先オフセットをメモリから取得するので、
    // アクセスレイテンシを発行レートに加算する。
    int32_t inst_id = trace->GetInstId();
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
    uint32_t lsu_latency = CalcLsuLatencyEach<DATA_READ> (trace, 0);
#ifdef SUPPORT_SC_HEAP
    if (lsu_latency ==  TARGET_IS_SCHEAP) {
        lsu_latency = 0;
        m_suspend_ctrl->RegisterLoadDependency (trace, 0, SuspendCtrl::DEP_ISSUE,
                                                SuspendCtrl::HAS_DEP_ISSUE,
                                                trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
    }
#endif // SUPPORT_SC_HEAP
    if (m_core_type == CORE_G4MH) {
        //printf("1:m_branch_target_pos=%u, lsu_latency=%u\n", m_branch_target_pos, lsu_latency);
        m_branch_target_pos += lsu_latency;
    } else {
        // 発行レートを設定（ArchDBの発行レート+メモリアクセス分）
        SetVarIssueRate (issue_rate + lsu_latency);
    }
    // ダミーを登録。通常のLoadレイテンシ計算ルーチンをスキップするために必要。
    AddWregVarLatency (issue_rate + lsu_latency);
}


void CsCtrl::CsDiv (TraceCtrl* trace)
{
    // G4MHではDIVを同時発行可能なので、命令グループを切り替える
    if (m_core_type == CORE_G4MH) {
        trace->SetParaGroupSwitched (true);
    }
}


void CsCtrl::CsDivq (TraceCtrl* trace)
{
    // 除数、被除数を取得
    int32_t v2 = static_cast<int32_t> (trace->GetRregValue (0));
    int32_t v1 = static_cast<int32_t> (trace->GetRregValue (1));

    // 絶対値に変換
    uint32_t v2_abs = abs (v2);
    uint32_t v1_abs = abs (v1);

    CsDivqCommon (trace, v2_abs, v1_abs);
}


void CsCtrl::CsDivqu (TraceCtrl* trace)
{
    // 除数、被除数を取得
    uint32_t v2 = static_cast<uint32_t> (trace->GetRregValue (0));
    uint32_t v1 = static_cast<uint32_t> (trace->GetRregValue (1));

    CsDivqCommon (trace, v2, v1);
}


void CsCtrl::CsDivqCommon (TraceCtrl* trace, uint32_t v2, uint32_t v1)
{
    // オーバーフロー時も通常計算と同じサイクル数（最短にはならない）
    // DIVQ/DIVQU命令のレイテンシ&発行レートは、ArchDBの値 + N
    // N = (((被除数v2の絶対値の有効bit数)-(除数v1の絶対値の有効bit数))/2)+1
    // ただし、N<1の場合はN=1、ゼロで割った場合はN=0となる。Nの範囲は0-16。

    // 有効bit数を計算(最上位の1を探す)
    int32_t v2_top = 0;
    for (int32_t i = 31; i >= 0; i--) {
        if ( ((v2 >> i) & 0x1) == 1) {
            v2_top = i + 1;
            break;
        }
    }
    int32_t v1_top = 0;
    for (int32_t i = 31; i >= 0; i--) {
        if ( ((v1 >> i) & 0x1) == 1) {
            v1_top = i + 1;
            break;
        }
    }

    // レイテンシNを算出
    int32_t latency;
    int32_t n;
    if (v1 != 0x0) {
        n = ((v2_top - v1_top) >> 1) + 1;
        if (n < 1) {
            n = 1;
        }
    } else {
        // 0除算
        n = 0;
    }
    latency = n;

    // DSTレジスタのレイテンシを記録
    int32_t inst_id = trace->GetInstId();
    int32_t dst_reg_latency;

    dst_reg_latency = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
    AddWregVarLatency (dst_reg_latency + latency); // for reg2
    dst_reg_latency = MicroArchDB::GetRegDstStage (inst_id, 1, m_archdb_id);
    AddWregVarLatency (dst_reg_latency + latency); // for reg3

    // 発行レートを記録
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
    if (m_core_type == CORE_G4MH) {
        SetVarIssueRate (issue_rate);
    } else {
        SetVarIssueRate (issue_rate + latency);
    }
    CsDiv (trace);
}


void CsCtrl::CsDivf (TraceCtrl* trace)
{
    if (m_core_type == CORE_G4MH) {
        int32_t inst_id = trace->GetInstId();
        m_divf_latency  = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
        SetVarIssueRate (1);
    }

    // FPU命令共通処理
    // - FPP例外モードのレイテンシ設定
    CsFpuFpp (trace);
}


void CsCtrl::CsMulfd (TraceCtrl* trace)
{
    if (m_core_type == CORE_G4MH) {
        int32_t inst_id = trace->GetInstId();
        m_mulfd_latency  = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
        SetVarIssueRate (1);
    }

    m_use_2rs = true;

    // FPU命令共通処理
    // - FPP例外モードのレイテンシ設定
    CsFpuFpp (trace);
}

void CsCtrl::CsCmpf (TraceCtrl* trace)
{
    //int32_t inst_id = trace->GetInstId();
    //m_fpcc_ra = MicroArchDB::GetRegDstStage (inst_id, 0);

    // FPU命令共通処理
    // - FPP例外モードのレイテンシ設定
    CsFpuFpp (trace);

    m_fpcc_ra = m_mod_fpu_latency;
}


void CsCtrl::CsTrfsr (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId();
    m_fpcc_rr = MicroArchDB::GetRegSrcStage (inst_id, 0, m_archdb_id);

    // FPU命令共通処理
    // - FPP例外モードのレイテンシ設定
    CsFpuFpp (trace);
}


void CsCtrl::CsFpuFpp (TraceCtrl* trace)
{
    // - --fpu-lat反映
    CsFpuLatency (trace);

    // プリサイス例外モードなら発行レートをDSTステージと同じにする
    if (m_core_type != CORE_G4MH) {
        if (m_fsctrl->IsFppMode () == true) {
            int32_t inst_id = trace->GetInstId();
            if (MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id) != MicroArchDB::END_ENTRY) {
                // 有効なDSTステージが存在したら、発行レートをDSTステージと同じに設定
                SetVarIssueRate (m_mod_fpu_latency);
            }
            // IssueRateの共通ルーチンを使う。FPU用特殊処理を使わないので0に設定
            m_fpu_issue_rate = 0;
        } else {
            // G3MでFPU命令のIssueRateは後続FPU命令だけに影響。
            // 後続がALU命令ならFPUのIssueRate=1相当として計算
            SetVarIssueRate (1);

            // FPUに対する発行レートを記録
            int32_t inst_id = trace->GetInstId();
            m_fpu_issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id) - 1;
        }
    }
}


void CsCtrl::CsFpuLatency (TraceCtrl* trace)
{
    // --fpu-lat オプションによるレイテンシ変更
    int32_t inst_id = trace->GetInstId();

    // 現在、RAは共通なので、0番目のエントリだけで判定
    int32_t dst_stage = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
    if (dst_stage != MicroArchDB::END_ENTRY) {
        // 有効なDSTステージが存在
        int32_t mod_latency;
        if (MicroArchDB::GetCycleCalcType (inst_id) == CCT_DIVF) {
            // DIVF命令だけ個別指定されたレイテンシを使用する
            mod_latency = dst_stage + m_divf_lat_offset;
        } else {
            mod_latency = dst_stage + m_fpu_lat_offset;
        }
        if (mod_latency < 0) {
            // マイナスなら0に補正
            mod_latency = 0;
        }
        m_mod_fpu_latency = mod_latency;
    }
}


void CsCtrl::CsCmov (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    CRegVec::const_iterator it_rreg = trace->GetRregDepVec()->begin ();
    CRegVec::const_iterator it_rend = trace->GetRregDepVec()->end ();
// printf("CsCmov\n");
    // cmov命令ではCCビットの値によって、reg1またはreg2がreg3に転送される。
    // 実際の動作がreg1を転送する場合、reg2依存のストールは発生する。
    // reg1/reg2のうち、使用されなかった方にも依存関係を設定
    if (std::find (it_rreg, it_rend, reg2 + R0) != it_rend) {
        // reg2が転送された。reg1を追加。
        // ただし、CMOVI5ではreg1がなくimm5なので登録しない。
        if (trace->GetInstId() != INST_ID_V_CMOV_I5) {
            trace->RecordRregDepVec (reg1);
        }
    } else {
        // reg1が転送された。reg2を追加。
        trace->RecordRregDepVec (reg2);
    }
}


void CsCtrl::CsCmovf (TraceCtrl* trace)
{
    uint64_t opcode = trace->GetInstCode ();
    int32_t inst_id = trace->GetInstId();
    uint32_t reg1;
    uint32_t reg2;
    if (inst_id == INST_ID_V_CMOVFD) {
        // CMOVF.D
        reg1  = OperandExtract (opcode,  4,  1) << 1;
        reg2  = OperandExtract (opcode, 15, 12) << 1;
    } else {
        // CMOVF.S
        reg1  = OperandExtract (opcode,  4,  0);
        reg2  = OperandExtract (opcode, 15, 11);
    }

    CRegVec::const_iterator it_rreg = trace->GetRregDepVec()->begin ();
    CRegVec::const_iterator it_rend = trace->GetRregDepVec()->end ();

    // cmov命令ではCCビットの値によって、reg1またはreg2がreg3に転送される。
    // 実際の動作がreg1を転送する場合、reg2依存のストールは発生する。
    // reg1/reg2のうち、使用されなかった方にも依存関係を設定
    if (std::find (it_rreg, it_rend, reg2 + R0) != it_rend) {
        // reg2が転送された。reg1を追加。
        trace->RecordRregDepVec (reg1);
        if (inst_id == INST_ID_V_CMOVFD) {
            trace->RecordRregDepVec (reg1 + 1);
        }
    } else {
        // reg1が転送された。reg2を追加。
        trace->RecordRregDepVec (reg2);
        if (inst_id == INST_ID_V_CMOVFD) {
            trace->RecordRregDepVec (reg2 + 1);
        }
    }

    // FPU命令共通処理
    // - FPP例外モードのレイテンシ設定
    CsFpuFpp (trace);
}


void CsCtrl::CsCmovfd (TraceCtrl* trace)
{
    CsCmovf (trace);
    m_use_2rs = true;
}


void CsCtrl::CsFpu2Rs (TraceCtrl* trace)
{
    CsFpuFpp (trace);
    m_use_2rs = true;
}


void CsCtrl::CsDivfd (TraceCtrl* trace)
{
    CsDivf (trace);
    m_use_2rs = true;
}


void CsCtrl::CsCmpfd (TraceCtrl* trace)
{
    CsCmpf (trace);
    m_use_2rs = true;
}


void CsCtrl::CsVld (TraceCtrl* trace)
{
    // indexレジスタのRAにメモリアクセスレイテンシを反映しないようにする。
    // VLD命令では、1個目がindex、それ以降がデータ
    int32_t inst_id = trace->GetInstId();
    AddWregVarLatency (MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id)); // for index reg
    int32_t lsu_latency = 0;
    if (m_core_type == CORE_G4KH) {
        lsu_latency = CalcLsuLatencyTotal<DATA_READ> (trace) - 1;
    }else{
        lsu_latency = CalcLsuLatencyTotal<DATA_READ> (trace) - (STAGE_WB - STAGE_EA);
    }
    if (lsu_latency < 0) {
        lsu_latency = 0;
    }

    int32_t data_dst_stage = MicroArchDB::GetRegDstStage (inst_id, 1, m_archdb_id) + lsu_latency;
    CRegVec::const_iterator it = m_dst_regs.begin ();
    // indexレジスタの文は設定済みなので、+1すすめる。
    if (it != m_dst_regs.end ()) {
        ++ it;
    }
    while (it != m_dst_regs.end ()) {
        // データ書き込み先レジスタの分を更新。
        AddWregVarLatency (data_dst_stage);
        ++ it;
    }
}


void CsCtrl::CsRetSync (TraceCtrl* trace)
{
    // To wait previous event.
    // PSWH.GM/GPID is changed in xxRET. To keep INTC event is updated before, I implemented this.
    // @@ TODO : This should wait as syncm for operand syserr. Need to wait before execute executing xxRET inst.
    m_shortest_issue_cycle = std::max(m_shortest_issue_cycle, m_cs_cycle_count) + 3;
}


void CsCtrl::CsSync (TraceCtrl* trace)
{
    if (m_core_type == CORE_G4MH) {
        if (m_is_first_call == true) {
            // In case of SYNC, stall the issue cycle for ownself (not next inst).
            // SYNCI : Sync with RA and refetch (refetch is counted in 2nd CsSync.)
            // SYNCM : Sync with memory access and RA
            // SYNCE : NOP (Doesn't come here)
            // SYNCP : Sync with RA
            // Because load instruction has destination register,
            // RA timign includes the load finished timing.
            int32_t inst_id = trace->GetInstId();
            int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);

            // For SYNCM, get release cycle for all LSU buffer to wait store access.
            // Note: LSU is shared with all HT. RA-cycle and cs_cycle must be treated with m_halt_time.
            //       Although G4MH doesn't have HT,
            //       keep this mechanishm for compatibility between G3x and G4x CS routine.
            uint64_t lsu_cycle = m_cs_lsu_buf->GetSyncCycle (); // Cycle which store/load is finished
            uint64_t ra_cycle = *(std::max_element(m_reg_active_cycle.begin(),
                                                   m_reg_active_cycle.end())); // cycle which calc inst and load are finished
            uint64_t release_cycle;
            if (inst_id == INST_ID_V_SYNCM) {
                // Wait both for RA and ST
                release_cycle = std::max (lsu_cycle, ra_cycle + m_halt_cycle);
            } else { // INST_ID_V_SYNCP, INST_ID_V_SYNCI
                // Wait RA
                release_cycle = ra_cycle + m_halt_cycle;
            }

            // To stall all following instructions, stall the EA stage of SYNC instruction.
            // Issue rate of SYNC in ArchDB is minimum issue rate.
            int64_t wait_cycle = release_cycle - (m_cs_cycle_count + m_halt_cycle);
            if (wait_cycle > issue_rate) { // issue_rate is >= 1 always
                m_shortest_issue_cycle = std::max(m_shortest_issue_cycle,
                                                  m_cs_cycle_count + static_cast<uint64_t>(wait_cycle));
                issue_rate = static_cast<int32_t>(wait_cycle);
            } else {
                m_shortest_issue_cycle = std::max(m_shortest_issue_cycle,
                                                  m_cs_cycle_count + static_cast<uint64_t>(issue_rate));
            }

            m_issue_stall_pmc += issue_rate;

            // Because incase of SYNC, stall count is added to the issue cycle of SYNC.
            // No need to wait issue rate to following instruction.
            SetVarIssueRate (1);

            m_is_first_call = false; // Init for second call.
        } else {
            m_is_first_call = true; // Init for next inst.

            // Prevent to issue following instruction with same cycle of SYNC.
            m_shortest_issue_cycle = std::max(m_shortest_issue_cycle, trace->GetStageCycle(STAGE_EA) + 1);

            // For SYNCI
            if (trace->GetInstId() == INST_ID_V_SYNCI) {
                // refetch next inst
                m_last_bp_result = BP_NONE;
                m_is_id_branch_inst = false;
                m_is_update_ibuf_time = true;
                m_branch_target_pos = 0; // SYNI is EA branch (EA = refetched IA)
            }
        }
    } else {
        // SYNCI : 命令発行の同期なので、RA/MEMとは同期しない（このルーチンには来ない）
        // SYNCM : メモリアクセスの同期
        // SYNCE : 実行パイプ・メモリアクセスの同期
        // SYNCP : 実行パイプ・メモリアクセスの同期
        int32_t inst_id = trace->GetInstId();
        int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);

        // LSUのバッファが開放されるサイクル数を取得
        // 注意：LSUバッファは全HTで共有されているので、RA時間とcs_cycleにm_halt_timeを加算
        uint64_t lsu_cycle = m_cs_lsu_buf->GetSyncCycle ();
        uint64_t release_cycle;
        if (inst_id == INST_ID_V_SYNCM) {
            release_cycle = lsu_cycle;
        } else {
            // INST_ID_V_SYNCE
            // INST_ID_V_SYNCP
            // RAステージとの待ち合わせ
            uint64_t ra_cycle = *(std::max_element(m_reg_active_cycle.begin(),
                                                   m_reg_active_cycle.end()));
            release_cycle = std::max (lsu_cycle, ra_cycle + m_halt_cycle);
        }

        int64_t wait_cycle = release_cycle - (m_cs_cycle_count + m_halt_cycle);
        if (wait_cycle > 0) {
            // ArchDBの発行レート、またはRA/メモリアクセス分の大きい方を使用
            issue_rate = std::max (issue_rate, static_cast<int32_t>(wait_cycle));
        }

        // 発行レートを設定
        SetVarIssueRate (issue_rate);
    }
}


void CsCtrl::CsRmw (TraceCtrl* trace)
{
    if (m_is_first_call == true) {
        if (m_core_type == CORE_G4MH) {
            m_use_2rs = true;

            // RMW->RMW repeat is 4 (Although G3MH spec is 1, actual is 7?)
            m_rmw_latency = 1; // Dummy to use m_rmw_ra_cycle
        }

        m_is_first_call = false;
        return;
    }
    m_is_first_call = true;

    // メモリから取得するがレジスタに格納しないので、
    // アクセスレイテンシを発行レートに加算する。
    int32_t inst_id = trace->GetInstId();

    // //WTBUF無効化のため、Atomic属性を最初に有効化
    // //ただし、TST1は除外
    //if ((inst_id != INST_ID_V_TST1_B3) && (inst_id != INST_ID_V_TST1)) {
        SetAtomic (true);
    //}

    // RMW inst is divided to 2 commits. 1st commit is memory access and 2nd is flag write-back.
    // RMW inst will issue access to bus after commit stage of 1st commit including TST1
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
    uint32_t latency = CalcLsuLatencyTotal<DATA_READ> (trace);
#ifdef SUPPORT_SC_HEAP
    if (latency == TARGET_IS_SCHEAP) {
        // RMWではSCHEAPから通知されたレイテンシが戻るので、ここには来ない。
        return;
    }
#endif // SUPPORT_SC_HEAP
    int32_t lsu_latency = std::max (static_cast<int32_t>(latency) - (STAGE_WB - STAGE_EA), 0);

    // CAXI: reg3, PSW.CC
    // CAXI以外:PSW.CC
    //printf("CsRmw:GetRegDstStage ()=%d, lsu_latency=%d\n", MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id), lsu_latency);
    AddWregVarLatency (MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id) + lsu_latency);

    // @@ TODO : CAXI will stop later inst.
    if (m_core_type == CORE_G4KH) {
        SetVarIssueRate (issue_rate + latency);
    }
}


void CsCtrl::CsCache (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId();
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
    for (int32_t i = 0; i < trace->GetIcCount (); ++i) {
        if (trace->GetIcEvent (i) == IC_EVT_FILL) {
            // FlashROM access is existed
            PhysAddr addr = trace->GetIcAddr (i);
            MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, addr);
            uint32_t issue_stall = GetBusIssueStallCycle<INST_READ> (trace);

            int64_t wait_cycle = m_pref_avail_cycle - trace->GetStageCycle (m_issue_stage);
            if (wait_cycle > 0) {
                issue_rate = std::max (issue_rate, static_cast<int32_t>(wait_cycle));
                issue_stall += static_cast<int32_t>(wait_cycle);
            }

            // GetReadLatencyに渡すissue_cycleはsystime基準。クロック比を考慮しないといけない
            // - 途中でクロック比が変わるかもしれないので、単純にクロック比をかけたら駄目
            int64_t issue_diff = ((trace->GetStageCycle (m_issue_stage) - m_cs_last_cycle_count) +
                    issue_stall) * m_fsctrl->m_clk_ratio;
            // メモリレイテンシの起点はEAなので、サイクル数をEA相当にする(EA=ID+1)
            uint64_t issue_cycle = m_fsctrl->m_cpu_time
#ifdef SUPPORT_SC_HEAP
                                    - (m_pre_consume_cycle * m_fsctrl->m_clk_ratio)
#endif // SUPPORT_SC_HEAP
                                    + issue_diff + m_fsctrl->m_clk_ratio;

#ifdef SYNC_PE_LSU_TIME
            uint64_t req_avail_time = mem_info->GetReqAcceptableCycle (m_pe_id, addr, IQ_BYTES);
            m_lsu_issue_time = std::max(m_lsu_issue_time, std::max(issue_cycle, req_avail_time));
#endif //#ifdef SYNC_PE_LSU_TIME

            uint32_t latency = mem_info->GetFetchLatency (m_pe_id, addr, issue_cycle, IQ_BYTES);

#ifdef SUPPORT_SC_HEAP
            if (latency == TARGET_IS_SCHEAP) {
                return;
            }
#endif // SUPPORT_SC_HEAP

            // latencyはsystime基準。PEのクロック基準に換算する
            uint32_t latency_pe_base = GetPeBaseCycleFromSystime (latency);

            // EA to ID
            latency_pe_base += 1;

            // 4 is the min repeat cycle for cache instruction from RTLsim result
            wait_cycle = std::max (4, static_cast<int32_t>(wait_cycle));
            m_pref_avail_cycle = trace->GetStageCycle (m_issue_stage) + latency_pe_base + wait_cycle;

            // 発行レートを設定
            SetVarIssueRate (issue_rate);
            return;
        }
    }

    // For ICHIT (ICNOP)
    int64_t wait_cycle = m_pref_avail_cycle - trace->GetStageCycle (m_issue_stage);
    if (wait_cycle > 0) {
        issue_rate = std::max (issue_rate, static_cast<int32_t>(wait_cycle));
    }
    // 発行レートを設定
    SetVarIssueRate (issue_rate);
    // 4 is the min repeat cycle for cache instruction from RTLsim result
    m_pref_avail_cycle = std::max (m_pref_avail_cycle, trace->GetStageCycle (m_issue_stage) + 4);
}


void CsCtrl::CsDwLdSt (TraceCtrl* trace)
{
    m_use_2rs = true;
    m_ld_st_dw = true;
    CsLongInst (trace);
}


void CsCtrl::CsInvBuf (TraceCtrl* trace)
{
    SetAtomic (true);
}


void CsCtrl::CsLongInst (TraceCtrl* trace)
{
    // G3MHでは32bit超命令の同時発行可能なので、命令グループを切り替える
    if (m_core_type == CORE_G4MH) {
        trace->SetParaGroupSwitched (true);
    }
}


void CsCtrl::CsNop (TraceCtrl* trace)
{
    // G3MHではNOPを同時発行可能なので、命令グループを切り替える
    if (m_core_type == CORE_G4MH) {
        trace->SetParaGroupSwitched (true);
    }
}


void CsCtrl::CsMac (TraceCtrl* trace)
{
    if (m_core_type == CORE_G4MH) {
        // G3MH : mac->mac or non integert repeat rate is 2. mac->integer repeat rate is 4.
        uint64_t inst_count = trace->GetStepCount ();
        if (m_mac_inst_count == inst_count) {
            // 連続2mac実行された
            //m_mac_latency = 4;
        }
        m_mac_inst_count = inst_count + 1;
    }
}


void CsCtrl::CsLdvQw (TraceCtrl* trace)
{
    SetVarIssueRate (1);
}


void CsCtrl::CsStvQw (TraceCtrl* trace)
{
    SetVarIssueRate (1);
}


void CsCtrl::CsLdIncDec (TraceCtrl* trace)
{
#ifdef SUPPORT_SC_HEAP
    // Because load response (NotifyLoadLatency()) from SC-HEAP modeule is later,
    // CForest will update the load target register when response is received from SC-HEAP.
    // Normally, the load target register is the last register in Trace record.
    //
    // But, in load with inc/dec instruction,
    // "last register - 1" is the load target (reg1). --- (1)
    // Additionally, if reg1 == reg3, because load target register (reg1) is overwritten by reg3,
    // CForest need to skip update reg1 when response is received from SC-HEAP. --- (2)
    SetLoadIncDec (true);
#endif // SUPPORT_SC_HEAP
}


void CsCtrl::CsStc (TraceCtrl* trace)
{
    // @@ TODO : Update for G4MH.
    // In G4MH, STC latency is different between success and fail.
    uint64_t opcode = trace->GetInstCode ();
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    int32_t add_latency = 0;

    // Check write reg history of reg3 by STC inst from trace info.
    // If there is reg3 and value is 1, STC is successful.
    const uint32_t wreg_count = trace->GetWregCount();
    uint32_t i;
    for (i = 0; i < wreg_count; i++) {
        uint32_t wreg = trace->GetWriteReg (i);
        if (wreg == reg3) {
            // reg3 was found
            uint32_t v3 = trace->GetWregValue (i);
            if (v3 == 0x1U) {
                // STC was successful.
                // Issue/Repeat/Latency are + 4 from MicroArchDB when STC was successed.
                add_latency = 4;
            }
        }
    }

    // Update latency and issue rate by succcess or fail of STC.
    uint32_t inst_id = trace->GetInstId ();
    int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
    SetVarIssueRate (issue_rate + add_latency); // issue rate
    AddWregVarLatency (MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id) + add_latency); // reg3
}


void CsCtrl::CsSyncPipe (void)
{
    // For Sync pipeline on LDM/STM

    // Because load instruction has destination register,
    // RA timign includes the load finished timing.
    uint64_t ra_cycle = *(std::max_element(m_reg_active_cycle.begin(),
                                           m_reg_active_cycle.end())); // cycle which calc inst and load are finished
    uint64_t release_cycle = ra_cycle + m_halt_cycle;

    // To stall all following instructions, stall the EA stage of SYNC instruction.
    // Issue rate of SYNC in ArchDB is minimum issue rate.
    int64_t wait_cycle = release_cycle - (m_cs_cycle_count + m_halt_cycle);
    if (wait_cycle > 0) {
        m_shortest_issue_cycle = std::max(m_shortest_issue_cycle,
                                          m_cs_cycle_count + static_cast<uint64_t>(wait_cycle));
        m_issue_stall_pmc += wait_cycle;
    } else {
        m_shortest_issue_cycle = std::max(m_shortest_issue_cycle,
                                          m_cs_cycle_count);
    }
}


void CsCtrl::CsStmMp (TraceCtrl* trace)
{
    // Need to calc Issue rate and Lsu latency.
    // Need to sync pipeline (as SYNCP).
    if (m_is_first_call == true) {
        // Calc DP stage of STM
        // Sync for pipeline
        CsSyncPipe ();
        m_is_first_call = false; // Update for second call.
    } else {
        m_is_first_call = true; // Init for next inst.

        // Calc STM issue rate
        uint64_t opcode = trace->GetInstCode ();
        uint32_t eh   = OperandExtract (opcode, 15, 11);
        uint32_t et   = OperandExtract (opcode, 31, 27);
        // Calc N+2
        // N=int(1.5*entry_num + 0.5)=int((3*entry_num+1)/2)
        uint32_t issue_offset;
        if (eh <= et) {
            uint32_t entry_num = (et - eh) + 1; // +1 : When eh = et, 1 entry (3reg) is transfered.
            issue_offset = ((3 * entry_num) + 1) >> 1;
        } else {
            // @@ TODO: Need to confirm on RTLsim
            // When no register transfer, issue will be 1
            issue_offset = 0;
        }
        int32_t inst_id = trace->GetInstId();
        int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
        SetVarIssueRate (issue_rate + issue_offset);
    }
}


void CsCtrl::CsLdmPre (TraceCtrl* trace)
{
    // Need to calc Issue rate and Lsu latency and ra stage.
    // Need to sync pipeline (as SYNCP).
    if (m_is_first_call == true) {
        // Calc DP stage of LDM
        // Sync for pipeline
        CsSyncPipe ();
        m_is_first_call = false; // Update for second call.
    } else {
        m_is_first_call = true; // Init for next inst.
        // Calc LDM latency
        m_mld_info.m_is_valid = true;
        m_mld_info.m_mld_type = MultiLoadInfo::MLD_LDM;
        m_mld_info.m_is_scheap = IsTargetScheap (DATA_READ, trace);
        m_mld_info.m_pop_num = 0;
        m_mld_info.m_dst_reg_latency = MicroArchDB::GetRegDstStage (trace->GetInstId(), 0, m_archdb_id);
        m_mld_info.m_cur_pos = 0;
        m_mld_info.m_end_pos = 0;
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
        CsLdmMain (trace);
#endif // SUPPORT_SC_HEAP
    }
}


void CsCtrl::CsLdmMain (TraceCtrl* trace)
{
    uint32_t cur = m_mld_info.m_cur_pos;
    uint32_t dst_latency = m_mld_info.m_dst_reg_latency;
    uint32_t rmem_cnt = trace->GetRmemCount ();
    while (cur < rmem_cnt) {
        if (IsLsuBufEmptyRecheck (trace) == false) {
            // 中断
            m_mld_info.m_cur_pos = cur;
            return;
        }
        uint32_t mem_latency = CalcLsuLatencyEach<DATA_READ> (trace, cur);
        uint32_t issue_offset = cur >> 1;
#ifdef SUPPORT_SC_HEAP
        if (mem_latency == TARGET_IS_SCHEAP) {
            if (! m_mld_info.m_is_scheap) {
                // 初回がCForest内メモリで、2回目以降でSC-HEAP側メモリにアクセス。
                // レイテンシ0として、待ち合わせ/書き戻しはしない。警告を表示しておく。
                CfMsg::TPrint (MSG_WAR, trace,
                               "<Warning detect: Dispose accessed 2 different memory area>\n");
                m_cs_lsu_buf->ReleaseScheapWaiting (trace->GetStageCycle (STAGE_EA) + m_halt_cycle + dst_latency + issue_offset);
            } else {
                CReg dst_reg = R0; // sysreg
                m_suspend_ctrl->RegisterLoadDependency (trace, cur, dst_reg,
                                                        SuspendCtrl::NO_DEP_ISSUE,
                                                        trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
            }
            mem_latency = 0;
        } else if (m_mld_info.m_is_scheap) {
            // 初回がSC-HEAPで、2回目以降でCForestメモリにアクセス。
            // 警告を表示しておく。
            CfMsg::TPrint (MSG_WAR, trace,
                           "<Warning detect: Dispose accessed 2 different memory area>\n");
        }
#endif // SUPPORT_SC_HEAP

        AddWregVarLatency (dst_latency + issue_offset + mem_latency);
        ++ cur;
    }
    m_mld_info.m_cur_pos = cur;
    CsLdmPost (trace);
}


void CsCtrl::CsLdmPost (TraceCtrl* trace)
{
    uint32_t load_num = m_mld_info.m_cur_pos;
    int32_t issue_rate = MicroArchDB::GetIssueRate (trace->GetInstId (), m_archdb_id);
    if (trace->GetInstId() == INST_ID_V_LDM_MP) {
        // LDM.MP : N+8 : N is int((load_num+1)/2), and +8 is in issue_rate in ArchDB.
        // LDM.GSR : 28 : Total issue rate is recorded in ArchDB. No need to consider pop_num.
        issue_rate += (load_num + 1) >> 1;
    }
    SetVarIssueRate (issue_rate); //  Should m_shortest_issue_cycle be updated?
    m_mld_info.m_is_valid = false;
}


void CsCtrl::SetCsFuncTable (void)
{
    m_inst_cs[CCT_GRP_UNC]  = (INSTCSP)(&CsCtrl::CsBUncond);
    m_inst_cs[CCT_LDXX_SELID]  = (INSTCSP)(&CsCtrl::CsLdstMpu);
    m_inst_cs[CCT_LAT_MV]   = (INSTCSP)(&CsCtrl::CsMov);
    m_inst_cs[CCT_LAT_MVR0] = (INSTCSP)(&CsCtrl::CsMovR0);
    m_inst_cs[CCT_LAT_DISP] = (INSTCSP)(&CsCtrl::CsDisposePre);
    m_inst_cs[CCT_LAT_PREP] = (INSTCSP)(&CsCtrl::CsPrepare);
    m_inst_cs[CCT_LAT_POP]  = (INSTCSP)(&CsCtrl::CsPopspPre);
    m_inst_cs[CCT_LAT_PUSH] = (INSTCSP)(&CsCtrl::CsPushsp);
    m_inst_cs[CCT_LAT_SW]   = (INSTCSP)(&CsCtrl::CsSwitch);
    m_inst_cs[CCT_LAT_CALL] = (INSTCSP)(&CsCtrl::CsCall);
    m_inst_cs[CCT_LAT_DIV]  = (INSTCSP)(&CsCtrl::CsDiv);
    m_inst_cs[CCT_LAT_DVQ]  = (INSTCSP)(&CsCtrl::CsDivq);
    m_inst_cs[CCT_LAT_DVQU] = (INSTCSP)(&CsCtrl::CsDivqu);
    m_inst_cs[CCT_UP_FPCC]  = (INSTCSP)(&CsCtrl::CsCmpf);
    m_inst_cs[CCT_UP_FPCCD]  = (INSTCSP)(&CsCtrl::CsCmpfd);
    m_inst_cs[CCT_USE_FPCC] = (INSTCSP)(&CsCtrl::CsTrfsr);
    m_inst_cs[CCT_FPU_FPP]  = (INSTCSP)(&CsCtrl::CsFpuFpp);
    m_inst_cs[CCT_FPU_2RS]  = (INSTCSP)(&CsCtrl::CsFpu2Rs);
    m_inst_cs[CCT_VLD_INDEX] = (INSTCSP)(&CsCtrl::CsVld);
    m_inst_cs[CCT_SYNC]     = (INSTCSP)(&CsCtrl::CsSync);
    m_inst_cs[CCT_RMW]      = (INSTCSP)(&CsCtrl::CsRmw);
    m_inst_cs[CCT_CMOV]     = (INSTCSP)(&CsCtrl::CsCmov);
    m_inst_cs[CCT_CMOVF]    = (INSTCSP)(&CsCtrl::CsCmovf);
    m_inst_cs[CCT_CMOVFD]    = (INSTCSP)(&CsCtrl::CsCmovfd);
    m_inst_cs[CCT_INV_BUF]  = (INSTCSP)(&CsCtrl::CsInvBuf);
    m_inst_cs[CCT_LONG_INST] = (INSTCSP)(&CsCtrl::CsLongInst);
    m_inst_cs[CCT_DIVF]     = (INSTCSP)(&CsCtrl::CsDivf);
    m_inst_cs[CCT_DIVFD]     = (INSTCSP)(&CsCtrl::CsDivfd);
    m_inst_cs[CCT_MULFD]    = (INSTCSP)(&CsCtrl::CsMulfd);
    m_inst_cs[CCT_DW]       = (INSTCSP)(&CsCtrl::CsDwLdSt);
    m_inst_cs[CCT_NOP]      = (INSTCSP)(&CsCtrl::CsNop);
    m_inst_cs[CCT_MAC]      = (INSTCSP)(&CsCtrl::CsMac);
    m_inst_cs[CCT_LDV_QW]   = (INSTCSP)(&CsCtrl::CsLdvQw);
    m_inst_cs[CCT_STV_QW]   = (INSTCSP)(&CsCtrl::CsStvQw);
    m_inst_cs[CCT_LOAD_INCDEC] = (INSTCSP)(&CsCtrl::CsLdIncDec);
    m_inst_cs[CCT_STC]   = (INSTCSP)(&CsCtrl::CsStc);
    m_inst_cs[CCT_LAT_RESBANK]  = (INSTCSP)(&CsCtrl::CsResbankPre);
    m_inst_cs[CCT_CACHE]    = (INSTCSP)(&CsCtrl::CsCache);
    m_inst_cs[CCT_RET_SYNC] = (INSTCSP)(&CsCtrl::CsRetSync);
    m_inst_cs[CCT_STMMP]    = (INSTCSP)(&CsCtrl::CsStmMp);
    m_inst_cs[CCT_LDM]   = (INSTCSP)(&CsCtrl::CsLdmPre);
    m_inst_cs[CCT_NORM]     = (INSTCSP)NULL;
}


bool CsCtrl::IsTargetScheap (MemAccessCommand cmd, TraceCtrl* trace) const
{
#ifdef SUPPORT_SC_HEAP
    // アクセス対象がSC-HEAPかどうかは、高速化のために、先頭の1アクセスだけで判定。
    // - Dispose/Popで、途中でアクセスターゲットが変わるのは通常ありえない。
    // - もしも発生した場合、以下のようにする
    //   (1) 初回＝CForest内メモリ、2回目以降＝SC-HEAP側メモリ : 待ち合わせ/書き戻ししない。
    //   (2) 初回＝SC-HEAP側メモリ、2回目以降＝CForest内メモリ : 待ち合わせ/書き戻しする。
    //   (1)ではロード結果が書き戻しされず、機能的に正しく動作しないかもだけど、
    //      通常ないケースなので問題ない。
    // @@ 通常ないケースだけど、もしも発生すると、SYNCの待ち合わせ対象にならず困る。
    //    全アクセス調べたほうが良いのでは？
    if (   ((cmd == DATA_READ) && (trace->GetRmemCount () == 0))
        || ((cmd == DATA_WRITE) && (trace->GetWmemCount () == 0))
        || ((cmd != DATA_READ) && (cmd != DATA_WRITE)) ) {
        return false; // メモリアクセスなし or フェッチ
    }

    PhysAddr acc_addr;
    if (cmd == DATA_READ) {
        acc_addr = trace->GetRmemAddr (0);
    } else {
        acc_addr = trace->GetWmemAddr (0);
    }
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, acc_addr);
    if (mem_info->IsScheap () == true) {
        return true;
    }
#endif // SUPPORT_SC_HEAP
    return false;
}


uint32_t CsCtrl::GetStallByMemBufFullG3M (TraceCtrl* trace)
{
    if ((trace->GetRmemCount () > 0) || (trace->GetWmemCount () > 0)) {
        // AvailCycleはAL/Respのタイミング。実際にIDが可能なのはWBステージなので+1
        int64_t stall = m_cs_lsu_buf->GetBuffAvailCycle () + 1 - (m_cs_cycle_count + m_halt_cycle);
        if (stall > 0) {
            return static_cast<uint32_t> (stall);
        }
    }
    return 0;
}


uint32_t CsCtrl::GetStallByMemBufFullG3MH (TraceCtrl* trace, uint64_t issue_cycle)
{
    if ((trace->GetRmemCount () > 0) || (trace->GetWmemCount () > 0)) {
        // AvailCycleはAL/Respのタイミング。実際にIDが可能なのはWBステージなので+1
        bool is_issue_parallel;
        if ((m_ex_ls[NUM_EX_LS - 1] != issue_cycle + 1) || (m_use_2rs)) {
            // wait until 2 empty resource (see [1] in GetBuffAvailCycle)
            is_issue_parallel = true;
        } else {
            is_issue_parallel = false;
        }

        uint32_t stall = 0;
        int64_t stall_amu;
        if (trace->GetRmemCount () > 0) {
            stall_amu = m_cs_lsu_buf->GetBuffAvailCycleG3MH_LD (is_issue_parallel) - (issue_cycle + m_halt_cycle);
        } else {
            stall_amu = m_cs_lsu_buf->GetBuffAvailCycleG3MH (is_issue_parallel) - (issue_cycle + m_halt_cycle);
        }
        if (stall_amu > 0) {
            stall = static_cast<uint32_t> (stall_amu);
        }

        CoproType copro_type = MicroArchDB::GetCoproType (trace->GetInstId ());
        if ((trace->GetRmemCount () > 0) && (copro_type == COPRO_SIMD)) {
            // FXU load uses LDBuffer.
            int64_t stall_fxu_ldb = m_fxu_ldv_active_cycle[0] - (issue_cycle + m_halt_cycle);
            //printf ("m_fxu_ldv_active_cycle[0] = %lu, stall_fxu_ldb=%ld\n", m_fxu_ldv_active_cycle[0], stall_fxu_ldb);
            if (stall_fxu_ldb > 0) {
                stall = std::max (stall, static_cast <uint32_t> (stall_fxu_ldb));
            }
        }
        return stall;
    }
    return 0;
}


uint32_t CsCtrl::GetStallByMemBufFullG3KH (TraceCtrl* trace)
{
    if ((trace->GetRmemCount () > 0) || (trace->GetWmemCount () > 0)) {
        // AvailCycleはAL/Respのタイミング。実際にIDが可能なのはWBステージなので+1
        int64_t stall = m_cs_lsu_buf->GetBuffAvailCycleG3KH () + 1 - (m_cs_cycle_count + m_halt_cycle);
        if (stall > 0) {
            return static_cast<uint32_t> (stall);
        }
    }
    return 0;
}


bool CsCtrl::IsLsuBufEmptyRecheck (TraceCtrl* trace)
{
#ifdef SUPPORT_SC_HEAP
    if (IsLsuBufEmpty (trace) == false) {
        // 毎回ApplyNotifyOwnをチェックしたくないので、LSUに空きがない場合だけ呼び出して再判定
        m_suspend_ctrl->ApplyNotifyOwn (trace);
        return IsLsuBufEmpty (trace);
    } else {
        return true;
    }
#else // SUPPORT_SC_HEAP
    return true;
#endif // SUPPORT_SC_HEAP
}


bool CsCtrl::IsLsuBufEmpty (TraceCtrl* trace) const
{
#ifdef SUPPORT_SC_HEAP
    // 空きあり(正確には空きではなく、SC-HEAPを待つ必要があるかどうか)
    // - cs_cycle_count > lsu_bufのバッファ空きあり
    // - cs_cycle_count > lsu_bufのバッファ空きなく、かつ、LSUバッファ内にSCHEAPレイテンシ待ちがない
    // 空きなし
    // - cs_cycle_count > lsu_bufのバッファ空きなく、かつ、LSUバッファ内にSCHEAPレイテンシ待ちがある
    //   SC-HEAPのレイテンシは１かもしれないし、100かもしれない。
    //   どのタイミングで空くかは、時間経過しないと不明。

    if (m_cs_lsu_buf->GetBuffAvailCycle () <= (trace->GetStageCycle (STAGE_EA) + m_halt_cycle)) {
        // 空き有り:LSUバッファに空きがある
        return true;
    }

    if (m_cs_lsu_buf->GetBuffAvailCycle () <= (m_extBusAvailCycle + m_halt_cycle)) {
        // 空き有り:LSUバッファに空きがある
        return true;
    }

    // LSUバッファにSC-HEAPレイテンシ待ちが存在するか確認。
    if (m_cs_lsu_buf->IsWaitSCHEAP () == false) {
        // 空き有り:現時点ではLSUバッファに空きないが、
        //          LSUバッファにSC-HEAPレイテンシ待ちが存在しないので、
        //          空くまで内部的に時間をすすめるだけでOK。
        return true;
    }

    // 空きなし: SC-HEAP側の時間を経過させる必要あり。
    return false;
#else // SUPPORT_SC_HEAP
    return true;
#endif // SUPPORT_SC_HEAP
}


void CsCtrl::SetNotifiedLatencyScheap (uint64_t release_cycle)
{
    m_cs_lsu_buf->SetNotifiedLatencyScheap (release_cycle);
}


// @@ 100行以上の長い関数になっているので書きなおす。
void CsCtrl::DependencyAnalysis (TraceCtrl* trace)
{
    if (m_core_type == CORE_G4KH) {
        /* メモリアクセスバッファ開放待ち */
        // 同時発行の命令も一緒にストールする
        uint32_t reg_stall;
        uint32_t membuf_full_stall;

        /* レジスタ依存関係によるストール量を取得 */
        membuf_full_stall = GetStallByMemBufFullG3KH (trace);
        uint64_t shortest_issue_cycle = CalcIssueCycle (trace);
        reg_stall = CalcRegStallCycle<CORE_G4KH> (trace, shortest_issue_cycle);

        uint32_t stall_cycle;
        if (membuf_full_stall > reg_stall) {
            stall_cycle = membuf_full_stall;
        } else {
            stall_cycle = reg_stall;
        }
        // @@ ここで実行パイプのサイクル数(m_cs_cycle_count)にスケジューラのクロック比を足し込む
        // @@ 命令パイプ側にも別途反映すべきか？
        // @@ IssueRateはissue_rate*スケジューラクロック比にしないと駄目？
        // @@   IssueRateはそのサイクル数分パイプを占有するはず。
        uint64_t d_stage;
        if (m_parallel_issue_flag == false) {
            /* 非同時実行 */
            // 非同時実行なので先行命令のIssueRateをストール量に反映
            if (m_prev_issue_rate > stall_cycle) {
                stall_cycle = m_prev_issue_rate;
            }
            m_prev_issue_rate = 0;

            if ( (stall_cycle == 0) && (m_last_d_stage == m_cs_cycle_count) ) {
                stall_cycle = 1;
            }
            if (stall_cycle != 0) {
                /* 依存関係あり */
                // Dステージ位置を算出
                // FREQ周期で時間を進めるため、stall量をFREQ単位に切り上げ
                d_stage  = m_cs_cycle_count + stall_cycle;
            } else {
                /* 依存関係なし */
                d_stage = m_cs_cycle_count;
            }
        } else {
            // 同時実行なので、自命令のID/EXの位置は先行命令のID/EXと同じ。
            if (stall_cycle != 0) {
                // 自命令がレジスタ依存回避のためにDステージでストールする場合、
                // 同時発行された先行命令のDステージもストールさせる。
                // 先行命令のログは出力済->レジスタ書込時間を遅らせて辻褄合わせ
                CfMsg::TPrint (MSG_INF, trace,
                               "<Previous instruction is internally stalled by inst packing>\n");

                // 先行命令のレジスタ書き込み時間をstall分だけ遅らせる
                // FREQ周期で時間を進めるため、stall量をFREQ単位に切り上げ
                uint32_t residue = stall_cycle % 1; // fix m_schedule_freq = 1 in G4KH
                if (residue != 0) {
                    stall_cycle += 1 - residue;
                }
                d_stage  = m_cs_cycle_count + stall_cycle;
                CRegVec::const_iterator it = m_reg_dst_prev.begin ();
                while (it != m_reg_dst_prev.end ()) {
                    CReg dst_reg = *it;
                    m_reg_active_cycle[dst_reg] += stall_cycle;
                    ++ it;
                }
            } else {
                d_stage  = m_last_d_stage;
            }
        }

        /* この時点で命令が実行された */
        // @@ 最後のメモリアクセスをLSUに積むまで、後続命令実行は待たされるはず。
        //    ここでの設定値は仮。LSUに積んでストールするなら再更新。

        m_cs_cycle_count = d_stage;
        trace->SetStageCycle (STAGE_DP, d_stage);
        trace->SetStageCycle (STAGE_EA, d_stage + 1);
        m_last_d_stage = d_stage;
    }
}


void CsCtrl::CalcLsuReadLatency (TraceCtrl* trace)
{
    // dispose/popspなどレジスタごとにレイテンシが異なる命令は個別ルーチンで処理
    // レジスタごとにレイテンシが変化しない命令なら、
    // 全アクセスレイテンシの合計を足し込む
    int32_t lsu_latency = 0;
    if (IsVarLatency () == false) {
        if (m_core_type == CORE_G4KH) {
            lsu_latency = CalcLsuLatencyTotal<DATA_READ> (trace) - 1;
        }else{
            lsu_latency = CalcLsuLatencyTotal<DATA_READ> (trace) - (STAGE_WB - STAGE_EA);
        }
        if (lsu_latency < 0) {
            lsu_latency = 0;
        }
    }
    m_lsu_read_latency = lsu_latency;
}


void CsCtrl::CalcLsuWriteLatency (TraceCtrl* trace)
{
#ifdef SUPPORT_SC_HEAP
    int32_t acc_cnt = trace->GetWmemCount ();
    for (int32_t i = m_suspend_ctrl->GetLsuRepeatCount (); i < acc_cnt; i++) {
        if (IsLsuBufEmptyRecheck (trace) == false) {
            // LSUに空きがないので、空くまで待つ
            m_suspend_ctrl->SetLsuRepeatCount (i);
            m_suspend_ctrl->SetLsuRepeated (true);
            return;
        }
        uint32_t curr_latency = CalcLsuLatencyEach<DATA_WRITE> (trace, i);
        if (curr_latency == TARGET_IS_SCHEAP) {
            m_suspend_ctrl->RegisterStoreDependency (trace, i, trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
            continue;
        } else {
            int32_t inst_id = trace->GetInstId();
            if (MicroArchDB::GetCycleCalcType (inst_id) == CCT_RMW) {
                if (curr_latency > (STAGE_WB - STAGE_EA)) {
                    // for CAXI/BitOP
                    m_lsu_read_latency = std::max (m_lsu_read_latency,
                                                   curr_latency - (STAGE_WB - STAGE_EA));
                }
            }
        }
    }

    // ここまで来たら全Writeアクセスを発行済み
    m_suspend_ctrl->SetLsuRepeated (false);

#else // SUPPORT_SC_HEAP
    int32_t lsu_latency = CalcLsuLatencyTotal<DATA_WRITE> (trace) - (STAGE_WB - STAGE_EA);
    if (lsu_latency > 0) {
//printf ("m_lsu_read_latency=%u, static_cast<uint32_t>(lsu_latency)=%u\n", m_lsu_read_latency, static_cast<uint32_t>(lsu_latency));
        m_lsu_read_latency = std::max (m_lsu_read_latency, static_cast<uint32_t>(lsu_latency));
    }
#endif // SUPPORT_SC_HEAP
}


template <CoreType core_type>
void CsCtrl::UpdateAvailCycle (TraceCtrl* trace)
{
    UpdateRegAvailStage (trace);

    int32_t inst_id = trace->GetInstId ();
    uint64_t d_stage = trace->GetStageCycle (m_issue_stage);
    uint32_t lsu_latency = m_lsu_read_latency + 1;
    InstGroupType inst_group = MicroArchDB::GetInstGroup (inst_id);

    if (core_type == CORE_G4KH) {
        //solve WB conflict
        if (m_parallel_issue_flag == false) {
            int32_t pos = 0;
            std::vector<uint64_t> dst_wb_cycle (31, 0);
            int32_t issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id);
            CRegVec::const_iterator it = m_dst_regs.begin ();
            while (it != m_dst_regs.end ()) {
                CReg write_reg = *it;
                //LD and ST have a separate port
                if ((write_reg != R0) && (inst_group != IGRP_LD) && (inst_group != IGRP_ST)) {
                    //calculate the cycle of wb stage of destination register
                    if ((inst_group == IGRP_MUL) || (inst_group == IGRP_FPU)|| (inst_group == IGRP_MAC)){
                        dst_wb_cycle[pos] = d_stage + m_reg_active_stage[pos]  ; //RA at WB
                    } else {
                        dst_wb_cycle[pos] = d_stage + m_reg_active_stage[pos] +1;
                    }
                    if (IsZeroLatency () == true){  //'cos m_reg_active_stage[] return 0 case 'mov'
                        dst_wb_cycle[pos] += 1;
                    }
                    //FPU 3 arguments: EX stage starts after ID stage is finished
                    if ((m_is_3_arg == true)&& (inst_group == IGRP_FPU)) {
                        dst_wb_cycle[pos] += (issue_rate - 1);
                    }
                }
                ++ pos;
                ++ it;
            }
            //back up the write back cycle to array for comparison with next instruction
            for (uint32_t i=0; i<31;i++){
                for (uint32_t j=0; j< WB_VAL_BUFF; j++){
                    if ((dst_wb_cycle[i] == m_reg_wb_cycle[j]) && (dst_wb_cycle[i] != 0)){
                        d_stage += 1;
                        dst_wb_cycle[i] += 1;
                        m_reg_wb_cycle[j] = 0;
                        j = 0;
                    } else if (d_stage > m_reg_wb_cycle[j]){
                        m_reg_wb_cycle[j] = 0;
                    }
                }
            }
            for (uint32_t i=0; i<31;i++){
                for (uint32_t j=0; j< WB_VAL_BUFF; j++){
                    if (m_reg_wb_cycle[j] == 0 ) {
                        m_reg_wb_cycle[j] = dst_wb_cycle[i];
                        break;
                    }
                }
            }
        }

        // wail till all registers available in pushsp and popsp
        if ((MicroArchDB::GetCycleCalcType (inst_id) == CCT_LAT_PUSH)
            || (MicroArchDB::GetCycleCalcType (inst_id) == CCT_LAT_POP)) {
            uint64_t max_value = *max_element(m_reg_active_cycle.begin(), m_reg_active_cycle.end());
            d_stage = std::max (d_stage, max_value);
        }

        // Update ID/EX stage until all registers are available before popsp/pushsp
        m_cs_cycle_count = d_stage;
        trace->SetStageCycle (STAGE_DP, d_stage);
        trace->SetStageCycle (STAGE_EA, d_stage + 1);
        m_last_d_stage = d_stage;
    }

    // Update register available cycle by the inst with updated d_stage.
    int32_t pos = 0;
    uint64_t max_ra = 0;
    CRegVec::const_iterator it = m_dst_regs.begin ();
    ArgFormat arg_format = MicroArchDB::GetArgFormat (inst_id);
    while (it != m_dst_regs.end ()) {
        CReg write_reg = *it;
        if (write_reg != R0) {
            m_reg_active_cycle[write_reg] = d_stage + m_reg_active_stage[pos] + lsu_latency;
#ifdef DEBUG_CS
            CfMsg::TPrint(MSG_ERR, trace, "ra_cycle[%d]=%lu, d_stage=%lu, ra_stage=%u, lsu=%u\n",
                          write_reg, m_reg_active_cycle[write_reg],
                          d_stage, m_reg_active_stage[pos], lsu_latency);
#endif // #ifdef DEBUG_CS
            max_ra = std::max (max_ra, m_reg_active_cycle[write_reg]);
            m_reg_write_resource[write_reg] = inst_group;
            m_reg_load_lram[write_reg] = (inst_group == IGRP_LD) && m_is_lram_access;
            m_arg_resource[write_reg] = arg_format;
        }
        ++ pos;
        ++ it;
    }
    m_is_lram_access = false; //Because ld.dw loads 2 regs, don't initialize in above loop.

    if (m_fpcc_ra != 0) {
        m_fpcc_active_cycle = d_stage + m_fpcc_ra + 1;
        max_ra = std::max (max_ra, m_fpcc_active_cycle);
        m_fpcc_ra = 0;
    }

    if (core_type != CORE_G4MH) {
        if (inst_group == IGRP_FPU) {
            m_fpu_issue_avail_cycle = d_stage + m_fpu_issue_rate + 1;
        }

        PswDepType psw_dep = MicroArchDB::GetPswDepType (inst_id);
        if ((psw_dep & PSW_DEP_SET) == PSW_DEP_SET) {
            if (inst_id == INST_ID_V_TRFSR) {
                m_pswcc_active_cycle = std::max (m_fpcc_active_cycle,
                                                 d_stage + m_pswcc_rr);
            } else {
                m_pswcc_active_cycle = d_stage + MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
            }
            m_psw_write_res = inst_group;
        }
    } else {
        if (inst_group == IGRP_DV) {
            // 以前のDIVが完了するまで、後続のDIVはEXに進めない
            if (pos != 0) { // == 0 になることはないはず
                // +1は不要
                m_div_ra_cycle = d_stage + m_reg_active_stage[0];
            }
        }

        if (m_divf_latency != 0) {
            m_divf_ra_cycle = d_stage + m_divf_latency + 1;
            m_divf_latency = 0;
        }

        if (m_mulfd_latency != 0) {
            m_mulfd_ra_cycle = d_stage + m_mulfd_latency + 1;
            m_mulfd_latency = 0;
        }

        if (m_rmw_latency != 0) {
            m_rmw_ra_cycle = d_stage + m_rmw_latency + 1;
            m_rmw_latency = 0;
        }

        //if (m_mac_latency != 0) {
        //    m_mac_use_cycle = d_stage + m_mac_latency + 1;
        //    m_mac_latency = 0;
        //}

        PswDepType psw_dep = MicroArchDB::GetPswDepType (inst_id);
        if ((psw_dep & PSW_DEP_SET) == PSW_DEP_SET) {
            ExecResType exec_resource = MicroArchDB::GetExecResType (inst_id, 0);
            if (inst_id == INST_ID_V_TRFSR) {
                // FPCC->PSW.CC
                // CMPF->TRFSR : ra=FPCC+1. TRFSR->CMP : ra+=1 additionally.
                m_pswcc_active_cycle = std::max (m_fpcc_active_cycle + 1,
                                                 d_stage + MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id) + 1 + 1);
                // @@ TODO: Update for G4MH
                // LSU->ALU : PSW flag latency is GR latency + 1.
                // BitOp, CAXI
                uint32_t ra_stage;
                if (IsVarLatency () == true) {
                    ra_stage = GetWregVarLatency (0);
                } else {
                    // Don't come here
                    ra_stage = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
                }
                m_pswcc_active_cycle = d_stage + ra_stage + 1 + 1;

            } else if (exec_resource == EX_RES_FXUMOV) {  // There is no inst update PSW.CC in EX_RES_FXUOP.
                // @@ TODO: Update for G4MH
                // FXU->ALU : flag latency is WR latency + 1.
                // CMPF.S4->TRFSRV.W4->PSW.CC
                uint32_t ra_stage;
                if (IsVarLatency () == true) {
                    ra_stage = GetWregVarLatency (0);
                } else {
                    ra_stage = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
                }
                m_pswcc_active_cycle = d_stage + ra_stage + 1 + 1;
            } else if (exec_resource == EX_RES_LS) {
                // @@ TODO: Update for G4MH
                // LSU->ALU : PSW flag latency is GR latency + 1.
                // BitOp, CAXI
                uint32_t ra_stage;
                if (IsVarLatency () == true) {
                    ra_stage = GetWregVarLatency (0);
                } else {
                    // Don't come here
                    ra_stage = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
                }
                m_pswcc_active_cycle = d_stage + ra_stage + 1 + 1;
                max_ra = d_stage + ra_stage + 1;
                m_rmw_lsu_issue_cycle = d_stage + ra_stage - 2;
                if (MicroArchDB::GetCycleCalcType (inst_id) == CCT_RMW) {
                    // CAXI/BitOp
//printf ("m_rmw_lsu_issue_cycle=%lu, d_stage + m_lsu_read_latency=%lu\n", m_rmw_lsu_issue_cycle, d_stage + m_lsu_read_latency + 8 - 2);
                    m_rmw_lsu_issue_cycle = std::max (m_rmw_lsu_issue_cycle, d_stage + m_lsu_read_latency + 8 - 2 );
                }
                // - Record for next BitOp access timing.
                // BitOP's access is issued at CMT stage.
                // Because CMT is InO, previous BitOP's WB must be finished before this BitOp's access.
                //m_bitop_block_cycle = d_stage + ra_stage + 1;
            } else {
                 uint32_t ra_stage;
                if (!m_reg_active_stage.empty ()) {
                    ra_stage = m_reg_active_stage[0];
                } else {
                    // TST, CMP, xxRET, LOOP
                    ra_stage = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
                }
                m_pswcc_active_cycle = d_stage + ra_stage + 1;
            }
            m_psw_write_res = inst_group;
        }

#if defined(BPMISS_FLUSH) || defined(STALL_RNFULL)
        // When BP miss, IA (from ID) is re-issued immediately at same cycle as EA of branch.
        // But, branch target's RN stage isn't issued until CMT+1 of branch.
        // (This timing is shown as "Flush" in depipe.log)
        // CMT stage
        // ALU: EX+2
        // ST : EX+3
        // LD : EX+3
        // MUL: EX+4
        // FPU: EX+5
        // STSR:EX+4
        // CMT: EX
        // CMT: NOP, JR, BR, CTRET
        // RNFull
        //uint64_t std_cmt_cycle =
        if (MicroArchDB::GetCycleCalcType (inst_id) == CCT_RMW) {
            // wait response for PSW.
        } else if (inst_group == IGRP_LD) {
            max_ra = d_stage + 2; // Don't wait response.
        } else if (inst_group == IGRP_ST) {
            max_ra = d_stage + 2;
        } else if (max_ra == 0) {
            // @@ TODO : + 2 is correct. But it is stalled to much. So, +0 trialy.
            // DP stage instruction may be caused this issue.
            // max_ra = d_stage + 0;
            max_ra = d_stage + 2;
        } else if (inst_group == IGRP_SP) {
            // todo : 1a. Prevent to wait response of dispose/popsp/etc for cmt timing too much
            //        -> In case of ST+LD after dispose, CMT timing of dispose will affect to the CMT timing of ST.
            //           If the CMT timing of dispose is delayed too much, ST+LD timign will be delayed too much also.
            //           Issue rate might be better to use? -> trial code
            int32_t issue_rate;
            if (IsVarIssue () == true) {
                issue_rate = GetVarIssueRate () - 1;
            } else {
                issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id) - 1;
            }
            max_ra = d_stage + 2 + issue_rate; // trial code
        }
        uint64_t cmt_cycle = max_ra + 1;

#if 0
        // NOTE : This code cause too much stall than expected. Disable temporally.
        // In case of Dual commit insts (divided 2 or more insts in RN/DP/WB and SingleCommitInst),
        // next inst can't commit in parallel.
        //   ID divided instructions have multiple commits.But it isn't guaranteed to prevent parallel cmt with other instructions.
        //   RN divided instructions always can't be CMT with others.
        //   DP divided instrucitons
        //   WB divided instructions // Need to check R0 for MUL instruction.
        //   Single CMT instructions always can't be CMT with others.
        if (   (MicroArchDB::GetRnDividedType (inst_id) != RND_UNDEF)
            || (MicroArchDB::GetDpDividedType (inst_id) != DPD_UNDEF)
            || (MicroArchDB::GetWbDividedType (inst_id) != WBD_UNDEF)
            || (MicroArchDB::GetSingleCmtType (inst_id) != SINGLE_CM_UNDEF) ) {
            // Can't commit this instruction with previos/later instructions
            cmt_cycle = std::max (cmt_cycle, m_last_cmt_cycle + 1);
            m_is_cmt_in_parallel = true; // prevent commit in parallel for next inst
        } else
#endif
        {
            if (m_is_cmt_in_parallel == true) {
                // previous 2insts are commited. This inst can't be commited with prev inst.
                cmt_cycle = std::max (cmt_cycle, m_last_cmt_cycle + 1);
                m_is_cmt_in_parallel = false;
            } else {
                if (m_last_cmt_cycle >= cmt_cycle) {
                    cmt_cycle = m_last_cmt_cycle;
                    m_is_cmt_in_parallel = true;
                }
            }
        }

#ifdef STALL_RNFULL
        // Renamed register is reserved in RN stage for operand register.
        // The register id is released in CMT stage. The id can be used from CMT+1.
        // When all rename id is used, instruction is stalled.
        if (!IsFusion ()) {
            uint32_t rn_id = m_curr_rn_id;
            m_rn_id_avail_cycle [rn_id] = cmt_cycle + 2; // CMT+1=RN. So next DP is +2.
            if (rn_id < NUM_RN_BUF - 1) {
                ++ m_curr_rn_id;
            } else {
                m_curr_rn_id = 0;
            }

            if (m_use_2rs) {
                // record 2nd rnid and update commit timing (Why commit timing is necessary to updated?)
                if ((!m_is_cmt_in_parallel) && (MicroArchDB::GetCycleCalcType (inst_id) != CCT_RMW)) {
                    m_is_cmt_in_parallel = true;
                } else {
                    cmt_cycle += 1;
                    m_is_cmt_in_parallel = false;
                }

                rn_id = m_curr_rn_id;
                m_rn_id_avail_cycle [rn_id] = cmt_cycle + 2; // CMT+1=RN. So next DP is +2.
                if (rn_id < NUM_RN_BUF - 1) {
                    ++ m_curr_rn_id;
                } else {
                    m_curr_rn_id = 0;
                }
            }
        }
#endif // #ifdef STALL_RNFULL
        trace->SetStageCycle (STAGE_WB, cmt_cycle);
        m_last_cmt_cycle = cmt_cycle;
#ifdef BPMISS_FLUSH
        if (m_fsctrl->m_branch_type != BR_NOT) {
            if (m_last_bp_result != BP_HIT) {
                m_bpmiss_flush_cycle = m_last_cmt_cycle + 2; // Fastest next DP stage (CMT+1=RN)
            }
        }
#endif // #ifdef BPMISS_FLUSH
#endif // #if defined(BPMISS_FLUSH) || defined(STALL_RNFULL)
    }
}


template <CoreType core_type>
void CsCtrl::UpdateIssueRate (TraceCtrl* trace)
{
    // 自命令の発行レートの分、次命令発行を遅らせる
    // ただし、次命令が同時発行される場合、次命令ではなく次命令+１を遅らせる
    // 同時発行される命令の大きい方のIssueRateを使用
    int32_t inst_id = trace->GetInstId();
    int32_t issue_rate;
    if (IsVarIssue () == true) {
        issue_rate = GetVarIssueRate () - 1;
    } else {
        issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id) - 1;
    }

    if (core_type == CORE_G4KH) {
        InstGroupType inst_group = MicroArchDB::GetInstGroup (inst_id);
        if (inst_group == IGRP_FPU){
            issue_rate = MicroArchDB::GetIssueRate (inst_id, m_archdb_id) - 1;
        }
    }

    // 分岐命令のissue_rateには分岐ターゲットのIF-IDまでの3cycleが含まれる
    // この期間はフェッチ部分で織り込み済みなので、ここでは引いておく。
    if (core_type != CORE_G4KH) {
        if (m_fsctrl->m_branch_type != BR_NOT) {
            issue_rate -= m_length_if_to_id;
        }
    }

    // 同時実行される2命令のうち、大きいissue_rateを使用
    // issue_rateはマイナスになり得るので注意(m_prev_issue_rateは必ず0以上)。
    if (issue_rate > static_cast<int32_t> (m_prev_issue_rate)) {
        m_prev_issue_rate = issue_rate;
    }

    // @@ TODO : support Divided stage for issue rate.

    if (core_type != CORE_G4KH) {
        if (IsFusion () == false) {
            // Not Fusion (or 1st instruction of Fusion)
            if (m_prev_issue_rate != 0) {
                SetIssueAvailCycleAll (trace, trace->GetStageCycle (STAGE_EA) + m_prev_issue_rate + 1);
                m_prev_issue_cycle = trace->GetStageCycle (STAGE_EA) + m_prev_issue_rate + 1;
            } else {
                SetIssueAvailCycle (trace, trace->GetStageCycle (STAGE_EA) + 1);
                m_prev_issue_cycle = trace->GetStageCycle (STAGE_EA) + 1;
            }
        } else {
            // Fusion
            if (m_is_prev_fusionable_inst != FUSION_ALU1ST_MOV2ND) {
                // alu+bcc
                // If fusined 2nd cmp is stalled by EX(BRU) resource conflict,
                // fusioned 1st ALU instruction will be stalled.
                // For ALU
                uint64_t new_cycle = trace->GetStageCycle (STAGE_EA) + m_prev_issue_rate + 1;
                if (m_prev_issue_cycle < new_cycle) {
                   if (m_prev_issue_rate != 0) {
                        SetIssueAvailCycleAll (trace, new_cycle);
                   } else {
                        OverwriteIssueAvailCycle (EX_RES_INT, new_cycle, m_prev_issue_cycle);
                   }
                }

                // For BCC
                if (m_prev_issue_rate != 0) {
                    SetIssueAvailCycleAll (trace, trace->GetStageCycle (STAGE_EA) + m_prev_issue_rate + 1);
                    m_prev_issue_cycle = trace->GetStageCycle (STAGE_EA) + m_prev_issue_rate + 1;
                } else {
                    SetIssueAvailCycle (trace, trace->GetStageCycle (STAGE_EA) + 1);
                    m_prev_issue_cycle = trace->GetStageCycle (STAGE_EA) + 1;
                }
            } else {
                // Fusion execution for MOV+ALU.
                // In this case, only 1 EX resource is used.
                // So, basically, EX stage updating for 2nd instrucion is skipped.
                // However, if 2nd inst is stalled, need to overwrite IssueAvailCycle by 1st inst.
                uint64_t new_cycle = trace->GetStageCycle (STAGE_EA) + m_prev_issue_rate + 1;
                if (m_prev_issue_cycle < new_cycle) {
                    if (m_prev_issue_rate != 0) {
                        SetIssueAvailCycleAll (trace, new_cycle);
                    } else {
                        OverwriteIssueAvailCycle (EX_RES_INT, new_cycle, m_prev_issue_cycle);
                    }
                }
            }
        }

        if (MicroArchDB::GetCoproType (inst_id) == COPRO_SIMD) {
            // In case of FXU, calcurate VD timing for issue (=EA) timing
            uint64_t ea_cycle = trace->GetStageCycle (STAGE_EA);
            SetFxuRsReleaseCycle (trace, ea_cycle, m_prev_issue_rate);

            // FXU inst is issued in EA stage.
            m_prev_issue_rate = 0;
        }
    }
}


void CsCtrl::UpdateVdIssueAbleCycle (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId();
    if (MicroArchDB::GetCoproType (inst_id) == COPRO_SIMD) {
        // In case of FXU, calcurate VD timing for issue (=EA) timing
        // Calc fastest VD cycle from RS avail cycle
        uint64_t rs_release_cycle = CalcFxuRsReleaseCycle (trace);
        m_vd_issue_able_cycle = std::max (rs_release_cycle, trace->GetStageCycle (STAGE_DP) + 1);
#ifdef PRINT_FXU_PIPE
        trace->SetStageCycle (STAGE_VD, m_vd_issue_able_cycle);
#endif // #ifdef PRINT_FXU_PIPE
        //printf ("m_vd_issue_able_cycle=%lu, rs_release_cycle=%lu, STAGE_DP+1=%lu\n",
        //        m_vd_issue_able_cycle, rs_release_cycle, trace->GetStageCycle (STAGE_DP) + 1);
    }
#ifdef PRINT_FXU_PIPE
    else {
        trace->SetStageCycle (STAGE_VD, 0);
    }
#endif // #ifdef PRINT_FXU_PIPE
}


uint64_t CsCtrl::CalcIssueCycle (TraceCtrl* trace) const
{
    // @@ 未対応
    // GHSではno_callt/no_disposeprepareオプションをつけているので、影響ありそうなのはmac。
    //複合命令はInst0でのみ発行する。
    //prepare/dispose
    //callt/syscall/switch
    //mac
    //set1/clr1/not1/tst1
    //caxi

    // Dispose/PrepareはLS/ALUの両方使うらしい。

    // @@ RS４個とかのケースもすぐに評価できるようにvectorにしたけど、
    //    2個までならサイクル数カウンタと同時実行フラグの方が楽そう。
    int32_t inst_id = trace->GetInstId ();
    ExecResType exec_resource = MicroArchDB::GetExecResType (inst_id, 0);

    // Skip this. Because DP issue is max 2 insts/cycle in G4MH as same as other EX resource.
    //if (exec_resource == EX_RES_DP){
    //    return ex_stage;
    //}

    std::vector<uint64_t>* res_cycle = m_ex_cycle[exec_resource];
    uint64_t executable_cycle = (*res_cycle) [0];

    // Check 2nd EX resource
    ExecResType exec_resource2 = MicroArchDB::GetExecResType (inst_id, 1);
    if (exec_resource2 != EX_RES_UNDEF) {
        //std::vector<uint64_t>* res_cycle = m_ex_cycle[exec_resource2];
        executable_cycle = std::max (executable_cycle, (*(m_ex_cycle[exec_resource2])) [0]);
    }

    if (MicroArchDB::GetCoproType (inst_id) == COPRO_SIMD) {
        // In case of FXU, calcurate VD timing for issue (=EA) timing
        // Calc fastest VD cycle from RS avail cycle
        executable_cycle = std::max (executable_cycle, m_vd_issue_able_cycle + 1);
    }

    return executable_cycle;
}


uint64_t CsCtrl::LimitExIssue2inst (uint64_t ex_stage)
{
    // 本当はここもリソース依存によるストールだけど、
    // 2命令に1度出てくるのはBusyなのでださない。
    // 対外的に公開したら必要かも。
#if 0
    // G3MH:RS->EXへの発行も2命令/cycleに制限
    if ((m_ex_issue_in_parallel == true) || (m_use_2rs)) {
        m_ex_issue_in_parallel = false;
        if (ex_stage > m_ex_issue_cycle) {
            m_ex_issue_cycle = ex_stage;
        } else {
            ++ m_ex_issue_cycle;
            ex_stage = m_ex_issue_cycle;
        }
        if (m_use_2rs) {
            m_ex_issue_in_parallel = true;
        }
    } else {
        if (ex_stage > m_ex_issue_cycle) {
            m_ex_issue_cycle = ex_stage;
        } else {
            m_ex_issue_in_parallel = true;
        }
    }
#endif //0
    return ex_stage;
}


// Judge the exec resource is OoO or Ino for EX issue.
bool CsCtrl::IsInoIssueTarget (ExecResType res) const
{
#if 0
    bool is_ino;
    switch (res) {
    case EX_RES_INT:  is_ino = false;
    case EX_RES_BRU:  is_ino = true;
    case EX_RES_SRU:  is_ino = true;
    case EX_RES_DIV:  is_ino = true;
    case EX_RES_MUL:  is_ino = true;
    case EX_RES_LS:   is_ino = true;
    case EX_RES_FPU:  is_ino = true;
    case EX_RES_SIMD: is_ino = true;
    case EX_RES_DP:   is_ino = true;
    case EX_RES_FXUMOV: is_ino = true;
    case EX_RES_FXUOP:  is_ino = true;
    //case EX_RES_UNDEF:
    //case EX_RES_NUM:
    default:          is_ino = true;
    }
    return is_ino;
#else
    return (res != EX_RES_INT);
#endif
}


void CsCtrl::SyncAllIssueAvailCycle (TraceCtrl* trace, uint64_t ex_stage)
{
    // Control issue tyming for InO/OoO in RS
    // - EXUI : OoO (some instructions are InO)
    //   But, 2 instructions which use same resource in MUL/DIV/BRU/SRU are InO.
    // - EXUF : InO
    // - LSUIF : InO
    // - FXU(VFP) : InO
    // - FXU(VMV) : InO
    // For InO resources, set other queue for current issue cycle.
    int32_t inst_id = trace->GetInstId ();
    for (int i = 0; i < 2; ++i) {
        ExecResType exec_resource = MicroArchDB::GetExecResType (inst_id, i);
        if (IsInoIssueTarget (exec_resource)) {
            if (exec_resource != EX_RES_UNDEF) {
                std::vector<uint64_t>* res_cycle = m_ex_cycle[exec_resource];
                std::vector<uint64_t>::iterator it = res_cycle->begin ();
                while (it != res_cycle->end ()) {
                    *it = std::max (*it, ex_stage);
                    ++ it;
                }
            }
        }
    }
}


void CsCtrl::SetIssueAvailCycleAll (TraceCtrl* trace, uint64_t ex_end_stage)
{
//printf ("SetIssueAvailCycleAll:ex_end_cycle=%lu\n", ex_end_stage);
    int32_t inst_id = trace->GetInstId();
    for (int i = 0; i < 2; ++i) {
        ExecResType exec_resource = MicroArchDB::GetExecResType (inst_id, i);
        std::vector<uint64_t>* res_cycle = m_ex_cycle[exec_resource];
        if (exec_resource != EX_RES_UNDEF) {
            //res_cycle->erase (res_cycle->begin ());
            //res_cycle->push_back (ex_end_stage); // ra相当
            //std::sort (res_cycle->begin(), res_cycle->end());
            size_t size = res_cycle->size ();
            res_cycle->assign (size, ex_end_stage);
        }
    }
}


void CsCtrl::SetIssueAvailCycle (TraceCtrl* trace, uint64_t ex_end_stage)
{
//printf ("SetIssueAvailCycle:ex_end_cycle=%lu\n", ex_end_stage);
    int32_t inst_id = trace->GetInstId();
    for (int i = 0; i < 2; ++i) {
        ExecResType exec_resource = MicroArchDB::GetExecResType (inst_id, i);
        if (exec_resource != EX_RES_UNDEF) {
            std::vector<uint64_t>* res_cycle = m_ex_cycle[exec_resource];

            res_cycle->erase (res_cycle->begin ());
            res_cycle->push_back (ex_end_stage); // ra相当
            std::sort (res_cycle->begin(), res_cycle->end());
        }
    }
}


void CsCtrl::OverwriteIssueAvailCycle (ExecResType exec_resource, uint64_t new_ex_end_stage,
                                       uint64_t old_ex_end_stage)
{
//printf ("SetIssueAvailCycle:ex_end_cycle=%lu\n", ex_end_stage);
    if (exec_resource != EX_RES_UNDEF) {
        std::vector<uint64_t>* res_cycle = m_ex_cycle[exec_resource];
        std::vector<uint64_t>::iterator it = res_cycle->begin ();
        while (it != res_cycle->end ()) {
            if (*it == old_ex_end_stage) {
                *it = new_ex_end_stage;
            }
            ++ it;
        }
        std::sort (res_cycle->begin(), res_cycle->end());
    }
}


uint64_t CsCtrl::CalcFxuRsReleaseCycle (TraceCtrl* trace) const
{
    // FXU has fxuop and fxumov RS.
    // Calc VD to RS latency.
    uint64_t rs_release_cycle = 0;
    int32_t inst_id = trace->GetInstId();
    ResvResType rs_resource1 = MicroArchDB::GetResvResType (inst_id, 0);
    if ((rs_resource1 == RS_RES_FXUMOV) || (rs_resource1 == RS_RES_FXUOP)) {
        rs_release_cycle = (m_rs_fxu_cycle[rs_resource1])->front ();
    }

    // Some instructions use different 2 different RS
    ResvResType rs_resource2 = MicroArchDB::GetResvResType (inst_id, 1);
    if ((rs_resource2 == RS_RES_FXUMOV) || (rs_resource2 == RS_RES_FXUOP)) {
        rs_release_cycle = std::max (rs_release_cycle, (m_rs_fxu_cycle[rs_resource2])->front ());
    }

    return rs_release_cycle;
}


void CsCtrl::SetFxuRsReleaseCycle (TraceCtrl* trace, uint64_t ex_stage, uint32_t issue_rate)
{
    // Next inst can use RS at EB of previous fxu inst
    // If issue rate is 2 or more, EB is delayed. (example: addf.s4 : issue=2, EA1|EA2|EB |)
    uint64_t rs_finish_cycle = ex_stage + issue_rate; // issue_rate is -1 already.
//printf ("rs_finish_cycle=%lu, ex_stage=%lu, issue_rate=%u\n", rs_finish_cycle, ex_stage, issue_rate);
    int32_t inst_id = trace->GetInstId();
    for (int i = 0; i < 2; ++i) {
        ResvResType rs_resource = MicroArchDB::GetResvResType (inst_id, i);
        if ((rs_resource == RS_RES_FXUMOV) || (rs_resource == RS_RES_FXUOP)) {
            std::vector<uint64_t>* res_cycle = m_rs_fxu_cycle[rs_resource];
            res_cycle->erase (res_cycle->begin ());
            res_cycle->push_back (rs_finish_cycle);
        }
    }
}


uint64_t CsCtrl::CalcWbConflictCycle (TraceCtrl* trace, uint64_t issue_cycle)
{
    int32_t inst_id = trace->GetInstId();
    ResvResType rs_resource = MicroArchDB::GetResvResType (inst_id, 0);
    if ((rs_resource == RS_RES_LS) || (rs_resource == RS_RES_FXUOP) || (rs_resource == RS_RES_FXUMOV) || IsFusion ()) {
        // Currently check WB conf for EXUI cases.
        return issue_cycle;
    }

    // Check instruction has valid dst registers
    CRegVec::const_iterator it_wreg = trace->GetWregDepVec()->begin();
    CRegVec::const_iterator it_wend = trace->GetWregDepVec()->end();
    if (it_wreg == it_wend) {
        return issue_cycle;
    }
    int32_t ra_stage = MicroArchDB::GetRegDstStage (inst_id, 0, m_archdb_id);
    if ((*it_wreg == R0) || (ra_stage == MicroArchDB::END_ENTRY)) {
       return issue_cycle;
    }
    if (IsVarLatency () == true) {
        ra_stage = GetWregVarLatency (0);
    }

    // Limit size (delete old ently)
    uint64_t ia_cycle = trace->GetStageCycle(STAGE_IA);
    for (uint32_t id = 0; id < NUM_RESERV_STATION; ++id) {
        std::vector<WbInfoType>::iterator it = m_wb_int_cycle[id].begin ();
        while (it != m_wb_int_cycle[id].end ()) {
            uint64_t cycle = (*it).first;
            if (ia_cycle > cycle) {
                it = m_wb_int_cycle[id].erase (it);
            } else {
                break;
            }
        }
    }

    uint64_t wb_cycle = issue_cycle + ra_stage;
    uint32_t id_start;
    uint32_t id_end;
    if (m_use_2rs) {
        id_start = 0;
        id_end   = NUM_RESERV_STATION;
    } else {
        id_start = m_rs_alu_id;
        id_end   = m_rs_alu_id + 1;
    }
    for (uint32_t id = id_start; id < id_end; ++id) {
        // printf ("Use RS%u\n", id_start);
        std::vector<WbInfoType>::iterator it = m_wb_int_cycle[id].begin ();
        while (it != m_wb_int_cycle[id].end ()) {
            uint64_t cycle = (*it).first;
            if (wb_cycle == cycle) {
                // Wb conflict occurs.
                // Stall EA issue.
                ++ wb_cycle;
                ++ issue_cycle;
                //printf ("WB conf\n");
            } else if (wb_cycle < cycle) {
                break;
            }
            ++ it;
        }
        // Record info
        m_wb_int_cycle[id].insert (it, WbInfoType (wb_cycle, false));
    }
    //printf ("Own WB =%lu\n", wb_cycle);

    return issue_cycle;
}


void CsCtrl::DependencyAnalysisOoO (TraceCtrl* trace)
{
    int32_t inst_id = trace->GetInstId();
    ExecResType exec_resource = MicroArchDB::GetExecResType (inst_id, 0);

    // Get shortest Issue avail cycle
    // In G4MH, OoO between other RS (except some special instrunctions. Currently ignore it.)
    // EXU : OoO in RS
    // FPU : InO in RS
    // LSU : InO in RS
    // FXU(VFP) : InO in RS (1 entry)
    // FXU(VMV) : InO in RS (4 entry)
    uint64_t shortest_issue_cycle;
    if (!IsFusion ()) {
        UpdateVdIssueAbleCycle (trace);
        shortest_issue_cycle = CalcIssueCycle (trace);
    } else {
        // If ALU+BCC, check BRU in EXUI.
        // Although don't use EX resource of BRU, it should be InO.
        if (exec_resource == EX_RES_BRU) {
            shortest_issue_cycle = std::max (CalcIssueCycle (trace), trace->GetStageCycle (STAGE_EA));
        } else {
            // Use prev EA cycle
            shortest_issue_cycle = trace->GetStageCycle (STAGE_EA);
        }
    }
    shortest_issue_cycle = std::max (m_shortest_issue_cycle, shortest_issue_cycle);
    shortest_issue_cycle = std::max (trace->GetStageCycle (STAGE_DP) + 1U, shortest_issue_cycle);

#ifdef ENABLE_EXE_ORDER_CTRL
    if (m_is_ooo_g4mh == false) {
        // InO Issue among RS.
        shortest_issue_cycle = std::max (shortest_issue_cycle, trace->GetStageCycle (STAGE_EA));
    }
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    // Check stall cycle
    // LSU buffer is full, stall all LS insts.
    uint32_t membuf_full_stall = 0;
    if ((exec_resource == EX_RES_LS) || (exec_resource == EX_RES_FXUMOV)) {
        // None-preceeding load access and all store access uses AMU from EA+4.
        // If buffer is free from EA+4, the inst is NOT stall.
        // But, because access target is checked later routine, it is difficult to check above here.
        // -> Treat all LD is preceeding load here.
        uint64_t lsu_amu_cycle;
        if (trace->GetRmemCount () != 0) {
            // LD (only preceeding access) uses AMU from EA.
            lsu_amu_cycle = shortest_issue_cycle;
        } else {
            // ST uses AMU from EA+4
            //lsu_amu_cycle = shortest_issue_cycle + 4;
            lsu_amu_cycle = shortest_issue_cycle + 0;
        }
        membuf_full_stall = GetStallByMemBufFullG3MH (trace, lsu_amu_cycle);

#ifdef USE_STALL_COUNT
        m_stall_counter[STALL_CNT_MEMBUF]->Count (membuf_full_stall);
#endif // #ifdef USE_STALL_COUNT
    }

    /* レジスタ依存関係によるストール量を取得 */
    uint32_t reg_stall = CalcRegStallCycle<CORE_G4MH> (trace, shortest_issue_cycle);

    // DIVF, DIVのリソースストール
    uint32_t res_stall = CalcResourceStall (trace, shortest_issue_cycle);
#ifdef USE_STALL_COUNT
    m_stall_counter[STALL_CNT_EXRES]->Count (res_stall);
#endif // #ifdef USE_STALL_COUNT

    uint32_t stall_cycle = std::max (res_stall, reg_stall);
    stall_cycle = std::max (stall_cycle, membuf_full_stall);

#ifdef DEBUG_CS
    CfMsg::TPrint (MSG_ERR, trace, "stall_cycle=%u, reg_stall=%u, res_stall=%u\n", stall_cycle, reg_stall, res_stall);
#endif // #ifdef DEBUG_CS
    uint64_t issue_cycle = shortest_issue_cycle + stall_cycle;

#ifdef USE_STALL_COUNT
    if (shortest_issue_cycle < issue_cycle) {
        m_stall_counter[STALL_CNT_EXRES]->Count (issue_cycle - shortest_issue_cycle);
    }
#endif // #ifdef USE_STALL_COUNT

#ifdef STALL_WB_CONFLICT
    // Check WB conflict at last
    issue_cycle = CalcWbConflictCycle (trace, issue_cycle);
#endif // #ifdef STALL_WB_CONFLICT

    // G3MHではRS->EXへの発行までは、EXリソースが異なっていてもIn-order
    // @@ 未対応: タイミングMETの関係で、一部OoOになっている (#9088)
    // - LSU命令は直前の整数,FPU命令を追い越す場合がある
    // - 整数命令(必要なのは分岐だけだが)は直前のLSU命令を追い越す場合がある ← 12/11追加
    // - FPU命令は直前のLSU命令を追い越す場合がある ← 12/13追加
    //if (m_core_type == CORE_G4MH) {

    /* この時点で命令が実行された */
#ifdef DEBUG_PRINT_STALL
    trace->SetLastIssueCycle (trace->GetStageCycle (STAGE_EA));
#endif // #ifdef DEBUG_PRINT_STALL
    //trace->SetStageCycle (STAGE_DP, m_dispatch_cycle);
    //if (trace->GetStageCycle (STAGE_EA) > issue_cycle) printf ("EX OoO\n");
    trace->SetStageCycle (STAGE_EA, issue_cycle);
    //if (m_core_type == CORE_G4MH) {
    m_cs_cycle_count = std::max (issue_cycle, m_cs_cycle_count); // @@ TODO : What is cs_cycle_count???
    //}
    //if ((IsFusion () == false) || (m_is_prev_fusionable_inst != FUSION_ALU1ST_MOV2ND)) {
    if (IsFusion () == false) {
        SetDispatchAvailCycle (trace, issue_cycle);
        //SyncAllIssueAvailCycle (trace, issue_cycle);
//printf ("kita10\n");
    } else if (m_last_d_stage != issue_cycle) {
        // If 2nd Fusioned inst is stalled, set of Fusion instructions are stalled.
        // Then, the updated info by 1st Fusioned instruction needs to be updated by 2nd issue_cycle
        OverwriteDispatchAvailCycleByFusionStall (trace, issue_cycle, m_last_d_stage);
//printf ("kita11\n");
    } else {
//printf ("kita12\n");
    }
    m_last_d_stage = issue_cycle;

    // In G4MH, EXUI is OoO in RS. Others are InO in RS.
    // In case of LSU, Ino in RS, but OoO in AMU.
    // @@ TODO : PSW.xx other sysreg dependency need to be checked additionally.
    //           cmp->bcond : bcond needs to wait cmp for PSW.CC.
    SyncAllIssueAvailCycle (trace, issue_cycle);
}


template <MemAccessCommand cmd>
uint32_t CsCtrl::CalcLsuLatencyTotal (TraceCtrl* trace)
{
    int32_t acc_cnt;
    if (cmd == DATA_READ) {
        acc_cnt = trace->GetRmemCount ();
    } else { // (cmd == DATA_WRITE)
        acc_cnt = trace->GetWmemCount ();
    }

    uint32_t highest_latency = 0;

    // @@ 本当にループが必要なのか？
    //    Dispose/Prepare/popsp/pushspなどを除くと1命令で最大1個なはず。
    for (int32_t i = 0; i < acc_cnt; i++) {
        uint32_t curr_latency = CalcLsuLatencyEach<cmd> (trace, i);
#ifdef SUPPORT_SC_HEAP
        if (curr_latency == TARGET_IS_SCHEAP) {
            if (cmd == DATA_READ) {
                // LoadTargetはtraceの最終エントリの有効な汎用レジスタ (GREG/VREG)
                // @@ 現在はたまたまロード結果の格納が最終なだけ。
                //    SH時代のLoadTargetRegのtrace記録を復活させた方がすっきりしそう。
                CReg target_reg;
                if (trace->GetWregDepVec()->empty () == false) {
                    if (IsLoadIncDec () == false) {
                        target_reg = trace->GetWregDepVec()->back();
                    } else {
                        // For Load with inc/dec instruction
                        // (1) "last register - 1" is the load target
                        // (2) If reg1 == reg3, CForest skip update reg1 by response from SC-HEAP
                        uint32_t pos = trace->GetWregDepVec()->size ();
                        if (pos >= 2) {
                            // check reg1 == reg3
                            uint64_t opcode = trace->GetInstCode ();
                            uint32_t reg1 = OperandExtract (opcode,  4,  0);
                            uint32_t reg3 = OperandExtract (opcode, 31, 27);
                            if (reg1 != reg3) {
                                target_reg = (*(trace->GetWregDepVec()))[pos-2];
                            } else {
                                // For (2), use R0 to prevent update.
                                target_reg = R0;
                            }
                        } else {
                            // no reach here. (because reg1 and reg3 are recorded always.)
                            target_reg = R0;
                        }
                    }
                } else {
                    // ここには来ないはず。
                    // vectorのサイズを判定せずに、back使用するのは怖いので念の為。
                    target_reg = R0;
                }
                m_suspend_ctrl->RegisterLoadDependency (trace, i, target_reg,
                                                        SuspendCtrl::NO_DEP_ISSUE,
                                                        trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
                curr_latency = 0;
            } else {
                m_suspend_ctrl->RegisterStoreDependency (trace, i, trace->GetStageCycle (STAGE_EA) + m_halt_cycle);
                continue;
            }
        }
#endif // SUPPORT_SC_HEAP
        if ((cmd == DATA_READ) && (highest_latency < curr_latency)) {
            highest_latency = curr_latency;
        }
        if (cmd == DATA_WRITE) {
            int32_t inst_id = trace->GetInstId();
            if (MicroArchDB::GetCycleCalcType (inst_id) == CCT_RMW) {
#ifdef SUPPORT_SC_HEAP
                if (curr_latency != TARGET_IS_SCHEAP)
#endif // #ifdef SUPPORT_SC_HEAP
                {
                    highest_latency = curr_latency;
                }
            }
        }
    }

    return highest_latency;
}


void CsCtrl::ShowAccessRoute (MemAccessCommand cmd, PhysAddr addr)
{
    // Generate trace log for bus as "<BusAccess ((1)) P(2)-->".
    // "-->cluster(bridge_cr0)-->C0MEM(fee908c8)>" is generated by cl_bridge and outstand class.
    // For example, complete format is as follow.
    // "<BusAccess (DR) P2-->cluster(bridge_cr0)-->C0MEM(fee908c8)>"
#ifdef CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, addr, cmd);
#else // CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, addr);
#endif // CLUSTER
    if ((cmd != INST_READ) && (mem_info->IsBufHitLast (addr) == true)) {
        // In case of DBUF hit, don't show buf trace
        return;
    }

    std::string cmd_str;
    switch (cmd) {
    case INST_READ  : cmd_str = "(IF)"; break;
    case DATA_READ  : cmd_str = "(DR)"; break;
    case DATA_WRITE : cmd_str = "(DW)"; break;
    case DATA_RMW   : cmd_str = "(RW)"; break; // don't come here
    default: cmd_str = ""; // don't come here
    }
    std::stringstream route_str;
    route_str << "<BusAccess " << cmd_str.c_str() << " P" << (m_pe_id & CL_PEID_MASK) << "-->";
    CfMsg::DPrint (MSG_INF, "%s", route_str.str().c_str());

    mem_info->ShowAccessRoute (m_pe_id & CL_PEID_MASK, cmd, addr);

    CfMsg::DPrintML (MSG_INF, ">\n");
}

template <MemAccessCommand cmd>
uint32_t CsCtrl::CalcLsuLatencyEach (TraceCtrl* trace, int32_t pos)
{
    // 命令フェッチについては、CsIbuf::SetupFetchLatencyを使用する
#ifdef ENABLE_DCACHE
    MemAccessRoute route;
#endif // #ifdef ENABLE_DCACHE
    PhysAddr acc_addr;
    ByteSize acc_size;
    if ((cmd == DATA_READ) && (pos < trace->GetRmemCount ())) {
#ifdef ENABLE_DCACHE
        route = trace->GetRmemRoute (pos);
#endif // #ifdef ENABLE_DCACHE
        acc_addr = trace->GetRmemAddr (pos);
        acc_size = trace->GetRmemSize (pos);
    } else if ((cmd == DATA_WRITE) && (pos < trace->GetWmemCount ())) {
#ifdef ENABLE_DCACHE
        route = trace->GetWmemRoute (pos);
#endif // #ifdef ENABLE_DCACHE
        acc_addr = trace->GetWmemAddr (pos);
        acc_size = trace->GetWmemSize (pos);
    } else {
        return 0;
    }

    // TSOの場合、ST命令のメモリアクセスはEXステージでなく、ストアデータの依存関係でストールさせる
    // @@ 実装簡単化のため、traceのEXステージを書き換える
    uint64_t bak_trace_ex_cycle = trace->GetStageCycle (m_issue_stage);
    int32_t inst_id = trace->GetInstId();
    InstGroupType inst_group = MicroArchDB::GetInstGroup (inst_id);
    uint64_t diff_issue_stage = 0;
    if (m_core_type == CORE_G4MH) {
        if (inst_group == IGRP_ST) {
            uint64_t offset = 2ULL; // if the data is fixed until EC stage, there is no stall for store data.
            if (m_st_req_avail_cycle > offset) {
               if ((m_st_req_avail_cycle - offset) > bak_trace_ex_cycle) {
                    diff_issue_stage = (m_st_req_avail_cycle - offset) - bak_trace_ex_cycle;
                    trace->SetStageCycle (m_issue_stage, m_st_req_avail_cycle - offset);
                    //printf ("m_st_req_avail_cycle=%lu, bak_trace_ex_cycle=%lu\n", m_st_req_avail_cycle, bak_trace_ex_cycle);
                }
            }
        }
    } else {
        trace->SetStageCycle (m_issue_stage, std::max(m_st_req_avail_cycle - 1,
                trace->GetStageCycle (m_issue_stage)) );
    }

    uint32_t ret_latency;
    // @@ 全DATAアクセスをDC対象と仮定
    //    （実際にはペリフェラル系をDC対象と対象としたらダメだけど、とりあえず暫定)
    //    DCは1cycle/回しか引けないので、DC_MISS/HITに関わらず、DCアクセスを制限
#ifdef ENABLE_DCACHE
    if (route == DCACHE_HIT_ROUTE) {
        // @@ 先行アクセスでAXC_MISS、後続のアクセスでHITの場合、
        //    直後のアクセスのほうが早くなってしまうのでインターロックが必要
        uint32_t issue_latency = GetBusIssueStallCycle<cmd> (trace);

        // DCは1cycle1回だけしか引けないので、同じサイクルにアクセスするならアクセスを遅らせる
        uint64_t issue_cycle = trace->GetStageCycle (m_issue_stage) + issue_latency;

        uint32_t latency = m_dc_hit_latency;
        int64_t diff = m_dc_avail_cycle - issue_cycle;
        if (diff > 0) {
            issue_cycle += static_cast<uint64_t>(diff);
        } else {
            diff = 0;
        }
        // @@ 2cycle引きの場合でも+1でよいのか？ -> Adachi-san: Yes.
        //    ただし、RAMが遅くてどうしようもなければ、スループットも落とすので、
        //    REDUCE_THROUGHPUT_DCHITで切り分け。
#ifdef REDUCE_THROUGHPUT_DCHIT
        m_dc_avail_cycle = issue_cycle + p_internals->GetDcacheHitLatency () + 1;
#else // REDUCE_THROUGHPUT_DCHIT
        m_dc_avail_cycle = issue_cycle + 1;
#endif // REDUCE_THROUGHPUT_DCHIT

        acc_addr &= Dcache::LINE_MASK;
        std::vector<DcFillInfo>::iterator it = m_dc_fill_info.begin ();
        while (it != m_dc_fill_info.end ()) {
            if (it->second < (issue_cycle + diff)) {
                it = m_dc_fill_info.erase (it);
            } else if (it->first == acc_addr) {
                latency += static_cast<uint32_t>(it->second - (issue_cycle + diff));
                break;
            } else {
                ++ it;
            }
        }

        m_cs_lsu_buf->ReserveBuffer (issue_cycle + m_halt_cycle + latency);
        m_cs_lsu_buf->SetIssueCycle (issue_cycle + m_halt_cycle);
        ret_latency = latency + static_cast<uint32_t>(diff);
    } else
#endif // ENABLE_DCACHE

#ifdef CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, acc_addr, cmd);
#else // CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, acc_addr);
#endif // CLUSTER

    // Stall by NonBlocking buffer in LSU (AMU and etc)
    uint32_t issue_stall = GetBusIssueStallCycle<cmd> (trace);

    // FXU load uses LDB
    CoproType copro_type = MicroArchDB::GetCoproType (trace->GetInstId ());
    if ((cmd == DATA_READ) && (copro_type == COPRO_SIMD)) {
        int64_t stall_fxu_ldb = m_fxu_ldv_active_cycle[0]
                          - (trace->GetStageCycle (m_issue_stage) + m_halt_cycle);
        if (stall_fxu_ldb > 0) {
            // バッファ待ちなら、前回AL／Respと同じサイクルにリクエスト
            issue_stall = std::max (static_cast<uint32_t> (stall_fxu_ldb), issue_stall);
            //printf ("issue_stall=%lu, cycle=%lu\n", issue_stall, trace->GetStageCycle (m_issue_stage) + m_halt_cycle);
        }
    }

    // Stall by memory ordering
    MemResourceType mem_resource = mem_info->GetMemResourceType (m_pe_id);
    BusInterfaceBase* bus_if = m_interface_avail_cycle [mem_resource];
    uint64_t if_issue_cycle = bus_if->GetAvailCycle (acc_addr, cmd);
    if (if_issue_cycle > (trace->GetStageCycle (m_issue_stage) + m_halt_cycle)) {
        uint32_t if_stall = static_cast<uint32_t> (
                                if_issue_cycle
                                - (trace->GetStageCycle (m_issue_stage) + m_halt_cycle)
                            );
        //printf ("if_stall=%u, issue_stall=%u, cycle=%lu\n", if_stall, issue_stall, trace->GetStageCycle (m_issue_stage) + m_halt_cycle);
        issue_stall = std::max (issue_stall, if_stall);
    }

    // Stall for TSO of LRAM
    // When ST and LD are issued in parallel and they are same word-bank, LD is issued after (or same timing) ST.
    // Although read req of LRAM is issued in EA by preceeding req normally, in above case, it is issued in CMT+1.
    // Then, Need to increase the lram read latency for "EA to CMT+1".
    // (Actually, because of same word-bank, req of LD is delayed +1 additionally (=CMT+2).
    //  This request stall for same word-bank is calculated in outstand.cpp side.)
    // LRAM read timing       |0  |1  |2  |3  |4  |5  |6  |7  |
    // pipeline stage of LD.W |EA |EB |EC |CMT|   |   |   |   |
    // Normal preceeding req  |CEN|RAM|AL |CMT|   |   |   |   |
    // Non-preceeding req     |EA |EB |EC |CMT|CEN|RAM|AL |   |
    // (e.g.) Timing when ST and LD access to same word bank and they are issued in parallel.
    //        Store req       |EA |EB |EC |CMT|CEN|RAM|   |   |
    //        LD req          |EA |EB |EC |CMT|   |CEN|RAM|AL |
    if (cmd == DATA_READ) {
        if (mem_resource == MEM_RES_LRAM) {
            // NOTE: Although dispatch is not issued in parallel, EA can be issued in parallel. How it should be treated?
            if ((m_last_lram_cmd == DATA_WRITE) && (m_last_lram_addr_word == (acc_addr & 0xCU))) {
                LoadType load_type = MicroArchDB::GetLoadType (trace->GetInstId ());
                switch (load_type) {
                case LDTYPE_S8:
                case LDTYPE_S16:
                case LDTYPE_U8:
                case LDTYPE_U16:
                case LDTYPE_U32:
                case LDTYPE_U64:
                    break;
                default:
                    // Inst is dispose, popsp, rmw, or such special inst.
                    // invalidate
                    m_last_lram_st_issue_cycle = 0;
                    m_last_lram_ea_cycle = 0;
                }

                if (m_parallel_dispatch_flag && (m_last_lram_ea_cycle == bak_trace_ex_cycle)) {
                //if (m_last_lram_ea_cycle == bak_trace_ex_cycle) {
                    const uint32_t LATENCY_EA_TO_CMT_PLUS_1 = 4U; // additional latency for EA to CMT+1 for Non-preceeding req.
                    if (m_last_lram_st_issue_cycle + LATENCY_EA_TO_CMT_PLUS_1 > (trace->GetStageCycle (m_issue_stage) + m_halt_cycle)) {
                        uint32_t stall_lram_tso = static_cast<uint32_t> (
                                                m_last_lram_st_issue_cycle + LATENCY_EA_TO_CMT_PLUS_1
                                                - (trace->GetStageCycle (m_issue_stage) + m_halt_cycle)
                                            );
                        //printf ("stall_lram_tso=%u, issue_stall=%u, cycle=%lu\n", stall_lram_tso, issue_stall, trace->GetStageCycle (m_issue_stage) + m_halt_cycle);
                        issue_stall = std::max (issue_stall, stall_lram_tso);
                    }
                }
            }
            m_last_lram_st_issue_cycle = 0;
            m_last_lram_addr_word = acc_addr & 0xCU;
            m_last_lram_cmd = DATA_READ;
            m_last_lram_ea_cycle = bak_trace_ex_cycle;
        } else {
            // invalidate
            m_last_lram_st_issue_cycle = 0;
            m_last_lram_addr_word = 0xFFFFFFFF; // Invalid word
            m_last_lram_cmd = NO_MEMACC; // Invalid cmd
            m_last_lram_ea_cycle = 0;
        }
    } else if (cmd == DATA_WRITE) {
        if (mem_resource == MEM_RES_LRAM) {
            LoadType load_type = MicroArchDB::GetLoadType (trace->GetInstId ());
            if (load_type != LDTYPE_UNDEF) {
                // Inst is prepare, pushsp, rmw, or such special inst.
                // invalidate
                m_last_lram_st_issue_cycle = 0;
                m_last_lram_addr_word = 0xFFFFFFFF; // Invalid word
                m_last_lram_cmd = NO_MEMACC; // Invalid cmd
                m_last_lram_ea_cycle = 0;
            } else {
                m_last_lram_st_issue_cycle = trace->GetStageCycle (m_issue_stage) + m_halt_cycle + issue_stall;
                m_last_lram_addr_word = acc_addr & 0xCU;
                m_last_lram_cmd = DATA_WRITE;
                m_last_lram_ea_cycle = bak_trace_ex_cycle;
            }
        } else {
            // invalidate
            m_last_lram_st_issue_cycle = 0;
            m_last_lram_addr_word = 0xFFFFFFFF; // Invalid word
            m_last_lram_cmd = NO_MEMACC; // Invalid cmd
            m_last_lram_ea_cycle = 0;
        }
    }

    //printf ("RecordAccessCycle:addr=%x, cmd=%x, cycle=%lu\n", acc_addr & 0xFFFFFFFU, cmd, trace->GetStageCycle (m_issue_stage) + m_halt_cycle + issue_stall);
    bus_if->RecordAccessCycle (acc_addr & 0xFFFFFFFFU, cmd,
                               trace->GetStageCycle (m_issue_stage) + m_halt_cycle + issue_stall);
    bus_if->DeleteOldEntry (trace->GetStageCycle (STAGE_IT));

    // GetReadLatencyに渡すissue_cycleはsystime基準。クロック比を考慮しないといけない
    // - 途中でクロック比が変わるかもしれないので、単純にクロック比をかけたら駄目
    int64_t issue_diff = ((trace->GetStageCycle (m_issue_stage) - m_cs_last_cycle_count) +
            issue_stall) * m_fsctrl->m_clk_ratio;
    // メモリレイテンシの起点はEAなので、サイクル数をEA相当にする(EA=ID+1)
    uint64_t issue_cycle = m_fsctrl->m_cpu_time
#ifdef SUPPORT_SC_HEAP
                            - (m_pre_consume_cycle * m_fsctrl->m_clk_ratio)
#endif // SUPPORT_SC_HEAP
                            + issue_diff + m_fsctrl->m_clk_ratio;

#ifdef SYNC_PE_LSU_TIME
    uint64_t req_avail_time = mem_info->GetReqAcceptableCycle (m_pe_id, acc_addr, acc_size);
    m_lsu_issue_time = std::max(m_lsu_issue_time, std::max(issue_cycle, req_avail_time));
#endif // #ifdef SYNC_PE_LSU_TIME

    uint32_t latency;
    if (cmd == DATA_READ) {
        latency = mem_info->GetReadLatency (m_pe_id,
                acc_addr, issue_cycle, acc_size);
        m_is_lram_access = mem_info->IsLramAccess(m_pe_id);
    } else {
        latency = mem_info->GetWriteLatency (m_pe_id,
                acc_addr, issue_cycle, acc_size);
    }

    bool is_scheap = false;
    uint32_t latency_pe_base = 0; // 0設定不要だがVS2010コンパイラ警告が出るので実施
#ifdef SUPPORT_SC_HEAP
    if (latency == TARGET_IS_SCHEAP) {
        if (  (cmd == DATA_READ)
            && (MicroArchDB::GetLoadType (trace->GetInstId ()) == LDTYPE_RMW) ) {
            // RMW命令についてはFS側でレイテンシ取得済みなので、
            // その値を引っ張る(SCHEAP対象の場合だけ)
            // SC-HEAPからの通知レイテンシはPE換算済み。
            latency_pe_base = m_suspend_ctrl->GetRmwLoadLatency ();
            is_scheap = true;
        } else {
            m_cs_lsu_buf->ReserveBufferScheap ();
            if (inst_group == IGRP_ST) {
                trace->SetStageCycle (m_issue_stage, bak_trace_ex_cycle);
            }
            return TARGET_IS_SCHEAP; // 呼び出し元で設定
        }
    }
#endif // SUPPORT_SC_HEAP

#ifdef ENABLE_DCACHE
    MemResourceInfo res_info;
    mem_info->GetResourceInfo (&res_info, acc_addr);
    bool is_tgt_tcm;
    if ((res_info.pe_read_latency <= 2) && (res_info.mem_location == m_pe_id)) {
        is_tgt_tcm = true;
    } else {
        is_tgt_tcm = false;
    }

    // TCMでなく、DC対象ならL1Cフィルコストを追加
    if ( (is_tgt_tcm == false) && m_fsctrl->m_dc->IsCachingArea (acc_addr) &&
            m_fsctrl->m_srf->SrDCCTRL()->GetDCHEN ()) {
        latency += p_internals->GetDcacheFillLatency ();

        // 同じキャッシュラインに対する次アクセスを遅らせるため、
        // DC_FILLの場合はエントリアドレスと時間を記録する
        PhysAddr dc_fill_addr = acc_addr & Dcache::LINE_MASK;
        // 128bitバスで1/2クロックで32Byteフィルするのでフィル完了は先頭フィルの2cycle後。
        PhysAddr dc_fill_end_cycle =
                trace->GetStageCycle (m_issue_stage) + issue_stall + latency + 2;

        // 同じアドレスが複数個登録されることを防止。
        std::vector<DcFillInfo>::iterator it = m_dc_fill_info.begin ();
        while (it != m_dc_fill_info.end ()) {
            if (it->first == dc_fill_addr) {
                m_dc_fill_info.erase (it);
                break;
            }
            ++ it;
        }
        m_dc_fill_info.push_back (DcFillInfo (dc_fill_addr, dc_fill_end_cycle));
    }
#endif // ENABLE_DCACHE

    // latencyはsystime基準。PEのクロック基準に換算する
    if (is_scheap == false) {
        latency_pe_base = GetPeBaseCycleFromSystime (latency);
    }

    // 両方同時に足すと多すぎたので、一方だけを使うように制限
    // @@ ここの部分は再考の余地あり。
    uint32_t additional_latency = 0;
    if (m_core_type == CORE_G4KH) {
    if (IsKeptReqInQueue (trace)) {
        // NBから溢れてLSUで滞留した場合、バッファ間のコピーに1cycleかかるのでレイテンシを+1
        if (cmd == DATA_READ) {
            latency_pe_base += 1;
        }
    } else {
        // IDからEAへの+1換算
        //latency_pe_base += 1;
        additional_latency = 1;
    }
    }

//    if (cmd == DATA_WRITE) {
//        // STならバッファ開放がおくれるようなので+1してみる
//        // @@ Although delay is only for Continuous SameBank ST,
//        //    apply it for all ST temporally.
//        additional_latency += 1;
//    }

    // LS命令バッファ煮詰まれた場合、1cycle/1issueに制限
    //uint64_t pe_issue_cycle = trace->GetStageCycle (m_issue_stage) + m_halt_cycle;
    //if (pe_issue_cycle < m_cs_lsu_buf->GetAmuAvailCycle ()) {
    //    if (cmd == DATA_WRITE) {
    //        m_cs_lsu_buf->SetIssueCycle (pe_issue_cycle + 1);
    //    } else {
    //        m_cs_lsu_buf->SetIssueCycle (pe_issue_cycle);
    //    }
    //}

    if (m_core_type != CORE_G4KH) {
    // LSUはPE内で共有。クロック比の考慮不要だが、HALT時間を含めた総スレッド時間で計算
    if (cmd == DATA_WRITE) {
        // AMU release timing is different between ld and st.
        // L1RAM
        //    |0 |1 |2 |3 |4 |5 |
        // ld |EA|EB|EC|WB|x |  | AMU is released at 4 (load latency is 2)
        // st |EA|EB|EC|CM|CE|RA| AMU is released at 4? (store latency is 5)
        m_cs_lsu_buf->ReserveBuffer (trace->GetStageCycle (m_issue_stage) + m_halt_cycle +
                                     issue_stall + latency_pe_base + additional_latency + 1);
#ifdef DEBUG_CS
        CfMsg::TPrint (MSG_ERR, trace, "latency=%u, latency_pe_base=%u, ReserveBuffer(%lu)\n",
                       latency, latency_pe_base, trace->GetStageCycle (m_issue_stage) + m_halt_cycle
                       + issue_stall + latency_pe_base + additional_latency + 1);
#endif // #ifdef DEBUG_CS
    } else {
        m_cs_lsu_buf->ReserveBuffer (trace->GetStageCycle (m_issue_stage) + m_halt_cycle +
                                     issue_stall + latency_pe_base + additional_latency + 2);
#ifdef DEBUG_CS
        CfMsg::TPrint (MSG_ERR, trace, "latency=%u, latency_pe_base=%u, ReserveBuffer(%lu)\n",
                       latency, latency_pe_base, trace->GetStageCycle (m_issue_stage) + m_halt_cycle
                       + issue_stall + latency_pe_base + additional_latency + 2);
#endif // #ifdef DEBUG_CS
    }

    additional_latency = 1;
    ret_latency = latency_pe_base + issue_stall + 1;
    } else { // G4KH only
        if (m_cs_lsu_cycle_pre == trace->GetStageCycle (m_issue_stage) + m_halt_cycle + issue_stall + latency_pe_base + additional_latency) {
            //1 pipe lsu in G3KH; the latency is +1 for the latter when 2 results are available at the same time.
            latency_pe_base += 1;
        }

        if (m_ld_st_dw == true) {
            //2 slots in NB used when using ld/st.dw
            m_cs_lsu_buf->ReserveBufferG3KH (trace->GetStageCycle (m_issue_stage) + m_halt_cycle +
                                                     issue_stall + latency_pe_base + additional_latency);
            m_cs_lsu_buf->ReserveBufferG3KH (trace->GetStageCycle (m_issue_stage) + m_halt_cycle +
                                                     issue_stall + latency_pe_base + additional_latency + 1);
            m_ld_st_dw = false;

        } else {
            m_cs_lsu_buf->ReserveBufferG3KH (trace->GetStageCycle (m_issue_stage) + m_halt_cycle +
                                     issue_stall + latency_pe_base + additional_latency);
        }
        m_cs_lsu_cycle_pre = trace->GetStageCycle (m_issue_stage) + m_halt_cycle + issue_stall + latency_pe_base + additional_latency;
        ret_latency = latency_pe_base + issue_stall;
    }
#ifdef DEBUG_CS
    CfMsg::TPrint (MSG_ERR, trace, "ret_latency=%u, issue_stall=%u\n", ret_latency, issue_stall);
#endif// #ifdef DEBUG_CS
#ifdef CLUSTER
    if (p_forest_util->IsEnableBusTrace ()) {
        ShowAccessRoute (cmd, acc_addr);
    }
#endif // #ifdef CLUSTER

    // FXU load uses LDB
    if ((m_core_type != CORE_G4KH) && (cmd == DATA_READ) && (copro_type == COPRO_SIMD)) {
        uint32_t responce_delay;
        if (p_forest_util->IsCramArea (acc_addr)) {
            // DW/cycle @ CRAM
            // latency + 1 additionally in CRAM read
            responce_delay = (acc_size >> 4) + 1;
        } else {
            // QW/cycle @ LRAM or DBUF
            responce_delay = 0;
        }

        // Transfer from LDB to register is InO
        uint64_t release_cycle = trace->GetStageCycle (m_issue_stage) + m_halt_cycle +
                                     issue_stall + latency_pe_base + additional_latency + 1 + responce_delay;
        uint64_t write_back_cycle = std::max (release_cycle, m_fxu_ldv_active_cycle.back () + 1);
        //printf ("m_fxu_ldv_active_cycle.back () + 1=%lu\n", m_fxu_ldv_active_cycle.back () + 1);
        responce_delay += static_cast<uint32_t>(write_back_cycle - release_cycle);
        m_fxu_ldv_active_cycle.erase (m_fxu_ldv_active_cycle.begin ());
        m_fxu_ldv_active_cycle.push_back (write_back_cycle);
        ret_latency += responce_delay;

        //printf ("write_back_cycle=%lu, ret_latency=%u\n", write_back_cycle, ret_latency);
    }

    if ((m_core_type != CORE_G4KH) && (inst_group == IGRP_ST)) {
        trace->SetStageCycle (m_issue_stage, bak_trace_ex_cycle);
        ret_latency += static_cast<uint32_t>(diff_issue_stage);
    }

    if (m_core_type == CORE_G4KH) {
        trace->SetStageCycle (m_issue_stage, bak_trace_ex_cycle);
    }
    return ret_latency;
}


uint32_t CsCtrl::GetPeBaseCycleFromSystime (uint32_t cycle_sys_base) const
{
    div_t result = div (cycle_sys_base, m_fsctrl->m_clk_ratio);
    uint32_t cycle_pe_base = result.quot;
    if (result.rem != 0) {
        cycle_pe_base += 1;
    }

    return cycle_pe_base;
}


template <MemAccessCommand cmd>
uint32_t CsCtrl::GetBusIssueStallCycle (TraceCtrl* trace) const
{
    uint64_t issue_cycle;
    if ((cmd == DATA_WRITE) || (cmd == INST_READ)) {
        issue_cycle = m_cs_lsu_buf->GetReqAvailCycle ();
    } else {
        issue_cycle = m_cs_lsu_buf->GetReqAvailCycle ();
    }
    if (IsAtomic()) {
        issue_cycle = std::max (issue_cycle, m_rmw_lsu_issue_cycle + m_halt_cycle);
    }
#if 1
    uint64_t issue_offset;
    if (cmd == DATA_WRITE) {
        // ST uses AMU from EA+4
        if (trace->GetWmemCount () >= 2) {
            issue_offset = 5;
        } else {
            issue_offset = 0;
        }
    } else {
        if (trace->GetRmemCount () >= 2) {
            issue_offset = 2;
        } else {
            issue_offset = 0;
        }
    }
    if ((trace->GetRmemCount () != 0) && (trace->GetWmemCount () != 0)) {
        issue_offset = 0;  // trial code
    }
#else
    uint64_t issue_offset = 0;
    if (cmd == DATA_WRITE) {
        // ST uses AMU from EA+4
        issue_offset = 0;
    } else {
        issue_offset = 0;
    }
#endif //0
    int64_t stall = issue_cycle
                  - (trace->GetStageCycle (m_issue_stage) + m_halt_cycle + issue_offset);
    if (stall > 0) {
        // バッファ待ちなら、前回AL／Respと同じサイクルにリクエスト
        return static_cast<uint32_t> (stall);
    } else {
        return 0;
    }
}


bool CsCtrl::IsKeptReqInQueue (TraceCtrl* trace) const
{
    return    m_cs_lsu_buf->GetReqAvailCycle ()
           >= (trace->GetStageCycle (m_issue_stage) + m_halt_cycle);
}


void CsCtrl::ClearCycleCount (void)
{
    m_cs_cycle_count = 0;
    m_lsu_issue_time = 0;
    m_cs_fetch_cycle_count = 0;
    m_ibuf->InitInstBuf ();
    m_ibuf_g3mh->InitInstBuf ();
    m_ibuf_g3kh->InitInstBuf ();
    m_ibuf_g3kh->InitFetchBuf ();
    m_ibuf_g3kh->InitBraBuf ();
    m_cs_last_cycle_count = 0;

    m_last_d_stage  = 0;
    m_prev_issue_rate = 0;
    m_is_prev_mov = false;
    m_is_taken = false;
    m_is_prev_taken = false;
    m_is_prev_branch = false;
    m_is_branch = false;
    ifid_irque_valid = 1;
    if_request = true;
    if_valid = true;
    reset_IFU_flag = true;
    m_is_3_arg = false;
    m_is_lram_access = false;
    m_is_prev_mul = false;
    m_cs_lsu_cycle_pre = 0;
    m_is_prev_dw = 0;

    m_bpmiss_flush_cycle = 0;
    m_latest_cmt_cycle = 0;

    m_extBusAvailCycle = 0;

    m_dc_avail_cycle = 0;

#ifdef SUPPORT_SC_HEAP
    m_pre_consume_cycle = 0;
#endif // SUPPORT_SC_HEAP

    CsInit ();
    m_cs_lsu_buf->InitBufCycle ();

    m_dispatch_cycle.assign (DISPATCH_WINDOW_SIZE, 0);
    m_last_dispatch_cycle = 0;
    m_parallel_dispatch_flag = false;

    m_prev_issue_cycle = 0;

    m_rs_int.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_int[i].second = i;
    }
    m_rs_ls.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_ls[i].second = i;
    }
    m_rs_fpu.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_fpu[i].second = i;
    }
    m_rs_simd.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_simd[i].second = i;
    }
    m_rs_dp.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_dp[i].second = i;
    }
    m_rs_fxu.assign (NUM_RESERV_STATION_FXU, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION_FXU; ++ i) {
        m_rs_fxu[i].second = i;
    }
    m_rs_undef.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_undef[i].second = i;
    }

    m_rs_dummy.assign (NUM_RESERV_STATION, 0);
    m_rs_fxumov.assign (NUM_RESERV_STATION_FXUMOV, 0);
    m_rs_fxuop.assign (NUM_RESERV_STATION_FXUOP, 0);

    m_rs_alu_id = 0;

    m_rs_cycle.clear ();
    m_rs_cycle.push_back (&m_rs_int);
    m_rs_cycle.push_back (&m_rs_ls);
    m_rs_cycle.push_back (&m_rs_fpu);
    m_rs_cycle.push_back (&m_rs_simd);
    m_rs_cycle.push_back (&m_rs_dp);
    m_rs_cycle.push_back (&m_rs_fxu); // Share RS in FXUOP and FXUMOV at DP stage
    m_rs_cycle.push_back (&m_rs_fxu); // Share RS in FXUOP and FXUMOV at DP stage
    m_rs_cycle.push_back (&m_rs_undef);

    m_rs_fxu_cycle.clear ();
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (INT)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (LS)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (FPU)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (SIMD)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (DP)
    m_rs_fxu_cycle.push_back (&m_rs_fxumov);
    m_rs_fxu_cycle.push_back (&m_rs_fxuop);
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (UNDEF)

    m_ex_int.assign (NUM_EX_INT, 0);
    m_ex_bru.assign (NUM_EX_BRU, 0);
    m_ex_sru.assign (NUM_EX_SRU, 0);
    m_ex_div.assign (NUM_EX_DIV, 0);
    m_ex_mul.assign (NUM_EX_MUL, 0);
    m_ex_ls.assign (NUM_EX_LS, 0);
    m_ex_fpu.assign (NUM_EX_FPU, 0);
    m_ex_simd.assign (NUM_EX_SIMD, 0);
    m_ex_dp.assign (NUM_EX_DP, 0);
    m_ex_fxumov.assign (NUM_EX_FXUMOV, 0);
    m_ex_fxuop.assign (NUM_EX_FXUOP, 0);
    m_ex_undef.assign (NUM_EX_UNDEF, 0);

    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_wb_int_cycle[i].clear();
    }

    m_fxu_ldv_active_cycle.assign (NUM_FXU_LDB, 4);

    m_issue_stall_pmc = 0;
    m_pref_avail_cycle = 0;
    m_shortest_issue_cycle = 0;

    m_pipe_ex.clear ();
    m_pipe_dp.clear ();

    m_vd_skid_avail_cycle = 0;
    m_vd_issue_able_cycle = 0;

    m_rn_id_avail_cycle.assign (NUM_RN_BUF, 0);
    m_curr_rn_id = 0;

    m_rmw_lsu_issue_cycle = 0;

    std::vector<BusInterfaceBase*>::iterator it_if = m_interface_avail_cycle.begin ();
    while (it_if != m_interface_avail_cycle.end ()) {
        (*it_if)->Init ();
        ++ it_if;
    }

    m_prev_copro_type = COPRO_NO;
#ifdef DEBUG_CS
    m_max_stall = 0;
#endif // DEBUG_CS

    m_last_lram_st_issue_cycle = 0;
    m_last_lram_addr_word = 0xFFFFFFFF; // Invalid word
    m_last_lram_cmd = NO_MEMACC; // Invalid cmd
    m_last_lram_ea_cycle = 0;
}


void CsCtrl::ElapsedCsTime (uint64_t erapsed_time)
{
    m_extBusAvailCycle += erapsed_time;
    m_cs_cycle_count   += erapsed_time;
    // TraceInfo.stage_cycleは次命令の実行時に更新

    m_cs_last_cycle_count = m_cs_cycle_count;
    m_last_d_stage = m_cs_cycle_count;
    SyncDispatchCycle (m_last_d_stage);
    first_para_group = PGRP_UNDEF;

    m_issue_stall_pmc += erapsed_time;
}


void CsCtrl::AddHaltCycle (uint64_t halt_cycle)
{
    m_halt_cycle += halt_cycle;
    m_issue_stall_pmc += halt_cycle;
}


uint64_t CsCtrl::CalcStallCount (void)
{
    uint64_t ext_bus_cycle = m_extBusAvailCycle;
    if (ext_bus_cycle < m_cs_cycle_count) {
        uint64_t stall_cycle = m_cs_cycle_count - ext_bus_cycle;
        m_extBusAvailCycle = m_cs_cycle_count;
#ifdef SUPPORT_SC_HEAP
        m_pre_consume_cycle = 0;
#endif // SUPPORT_SC_HEAP
#ifdef DEBUG_CS
        if (stall_cycle > 300) {
            CfMsg::TPrint (MSG_ERR,  m_fsctrl->m_trace, "<Internal Error (Doubtful behavior): stall cycle (%lu) is huge DP=%lu, EX=%lu>\n",
                           stall_cycle, m_fsctrl->m_trace->GetStageCycle(STAGE_DP), m_fsctrl->m_trace->GetStageCycle(STAGE_EA));
        }
        m_max_stall = std::max(m_max_stall, stall_cycle);
#endif // #ifdef DEBUG_CS
        return stall_cycle;
    } else {
#ifdef SUPPORT_SC_HEAP
        // 未経過なので経過分だけ更新
        m_pre_consume_cycle = ext_bus_cycle - m_cs_cycle_count;
#endif // SUPPORT_SC_HEAP
        return 0;
    }
}


void CsCtrl::CsInit (void)
{
    m_reg_wb_cycle.assign (WB_VAL_BUFF, 0);
	m_reg_active_cycle.assign (DEP_REG_MAX + 1, 1);
    m_reg_write_resource.assign (DEP_REG_MAX + 1, IGRP_UNDEF);
    m_reg_load_lram.assign (DEP_REG_MAX + 1, false);
    m_reg_active_stage.clear ();
    m_has_rom_depend.assign (DEP_REG_MAX + 1, false);
    m_has_reg_depend_list.assign (DEP_REG_MAX + 1, false);
    m_reg_dst_prev.clear();
    m_dst_regs.clear ();
    m_psw_write_res = IGRP_UNDEF;
	m_arg_resource.assign (DEP_REG_MAX + 1, ARG_ERROR);

    first_para_group = PGRP_UNDEF;

    m_cs_last_cycle_count = 0; /* @@SAITO_041124 */

    m_is_force_fetch = false;
    m_is_branch_predict_miss = false;

    m_is_branch_target = false;
    m_is_update_ibuf_time = false;
    m_is_branch_target_next = false;
    m_update_branch_target_pos_by_fusion = false;

    m_fpcc_ra = 0;
    m_fpcc_rr = 0;
    m_fpcc_active_cycle = 0;
    m_pswcc_active_cycle = 0;

    m_fpu_issue_avail_cycle = 0;
    m_fpu_issue_rate = 0;

    m_mac_use_cycle = 0;
    m_mac_latency = 0;
    m_mac_inst_count = 0xFFFFFFFFFFFFFFFFULL;

    m_prev_cs_fecth_count = 0;
    m_prev_fetch_cycle = 0;
    m_halt_cycle = 0;

    m_access_info_vec.clear ();
    m_has_reg_depend.clear ();

    m_is_atomic = false;
    m_is_zero_latency = false;
    m_is_var_issue = false;
    m_wreg_var_latency.clear ();
    m_issue_var_rate = 0;
    m_parallel_issue_flag = false;
    m_branch_target_pos = 0;
    m_is_id_branch_inst = false;
    m_mld_info.Init ();

    m_lsu_read_latency = 0;
    m_is_read_finished = false;
    m_last_bp_result = BP_NONE;
    m_is_bcond9 = false;
    m_dc_fill_info.clear ();

    m_inflight_id = 0;
    m_cmt_stage.assign (NUM_RN_BUF, 0);
    m_is_cmt_in_parallel = true;
    m_last_cmt_cycle = 0;

    m_div_ra_cycle = 0;
    m_mulfd_ra_cycle = 0;
    m_mulfd_latency = 0;
    m_divf_ra_cycle = 0;
    m_divf_latency = 0;
    m_rmw_ra_cycle = 0;
    m_rmw_latency = 0;
    m_mod_fpu_latency = 0;
    m_st_req_avail_cycle = 1;
    m_use_2rs = false;
    m_last_id_cycle = 0;
    m_is_first_call = true;
    m_is_fusion_exec = false;
    m_prev_reg2 = 0xFFFFFFFF; // invalid reg number
    m_is_prev_fusionable_inst = FUSION_UNDEF;
}


void CsCtrl::MultiLoadInfo::Init (void)
{
    m_is_valid = false;
    m_mld_type = MLD_DISPOSE;
    m_is_scheap = false;
    m_reglist = 0;
    m_pop_num = 0;
    m_is_64bit_aligned = false;
    m_check_first_reg = false;
    m_mem_latency = 0;
    m_last_issue_wait = 0;
    m_dst_reg_latency = 0;
    m_cur_pos = 0;
    m_end_pos = 0;
}


void CsCtrl::StallCounter::Count (uint64_t stall) {
    m_total_cycle += stall;
    //m_min_cycle = std::min (m_min_cycle, stall);
    //m_max_cycle = std::max (m_max_cycle, stall);
    //++ m_active_count;
    if (stall != 0) {
        CfMsg::StallPrint ("%s: %lu\n", GetName ().c_str (), stall);
    }
}


void CsCtrl::ShowPerfInfo (void) const
{
#ifdef USE_STALL_COUNT
    std::ostringstream oss;
    ContextId peid = m_fsctrl->GetPeId ();
    oss << "P" << std::setw(1) << peid;

    CfMsg::StallPrint ("  Stall Info (%s)\n",  oss.str().c_str());
    StallCountVec::const_iterator it = m_stall_counter.begin ();
    while (it != m_stall_counter.end ()) {
        CfMsg::StallPrint ("    %-17s: %8lu\n",
               (*it)->GetName ().c_str (), (*it)->GetTotalCycle ());
        ++ it;
    }
#endif // #ifdef USE_STALL_COUNT
}


void CsCtrl::ResetPerfInfo (void)
{
    StallCountVec::iterator it = m_stall_counter.begin ();
    while (it != m_stall_counter.end ()) {
        (*it)->InitCount ();
        ++ it;
    }
}


void CsCtrl::CsReset (void)
{
    CsInit ();
    ClearCycleCount();
}


void CsCtrl::ChangeFsToCs (PhysAddr inst_addr)
{
    // 事前の状態をクリア
    CsReset ();

    // FSの命令キューに記録されているアドレスをCS命令バッファに登録
    if (inst_addr != INVALID_VADDR) {
        m_ibuf->ForceSetInstBuf (inst_addr);
        m_ibuf_g3mh->ForceSetInstBuf (inst_addr);
        m_ibuf_g3kh->ForceSetInstBuf (inst_addr);
    }
    m_ibuf_g3mh->ForceUpdateIaAddress (inst_addr);
    m_ibuf_g3kh->ForceUpdateIaAddress (inst_addr);
}


void CsCtrl::SetClkRatio (uint32_t ratio)
{
    m_ibuf->SetClkRatio (ratio);
    m_ibuf_g3mh->SetClkRatio (ratio);
    m_ibuf_g3kh->SetClkRatio (ratio);
}


void CsCtrl::PreConsumeClock (TraceCtrl* trace)
{
    ++m_extBusAvailCycle;
#ifdef SUPPORT_SC_HEAP
    ++m_pre_consume_cycle;
    m_suspend_ctrl->UpdateSuspendState (trace);
#endif //#ifdef SUPPORT_SC_HEAP
}


void CsCtrl::ForceUpdateIaAddress (PhysAddr ia_addr)
{
    m_ibuf_g3mh->ForceUpdateIaAddress (ia_addr);
    m_ibuf_g3kh->ForceUpdateIaAddress (ia_addr);
}


CsCtrl::CsCtrl (uint32_t pe_id, FsCtrl* fsctrl, CsLsuBuf* cs_lsu_buf, CoreType core_type)
    : m_pe_id (pe_id), m_fsctrl (fsctrl), m_cs_lsu_buf (cs_lsu_buf), m_core_type (core_type),
    m_issue_stage ((core_type == CORE_G4MH) ? STAGE_EA : STAGE_DP),
    m_archdb_id ((core_type == CORE_G4MH) ? ARCH_DB_G3MH : ARCH_DB_G3KH)
{
    m_core_version = fsctrl->m_core_version;
    if (m_core_type == CORE_G4KH) {
        m_length_if_to_id = 2;
    } else {
        // G3MH
        uint32_t additional_fecth_stage = p_internals->GetAdditonalFetchStage ();
        // IAから実行ステージまでの距離
        m_length_if_to_id = 5 + additional_fecth_stage; // STAGE_DP - STAGE_IA;
        // @@ 両方にadditional_fecth_stageを足すと、分岐ペナルティが+２になる
        //    分岐予測が遅れるので次命令のIA位置が+１，命令パイプ自体が+1になるので
        //    IA位置からEXまでの距離も+１
        //    分岐ペナルティ+1を見るために、_length_id_to_next_ifに
        //    additional_fecth_stageを加算しない。
        //    additional_fecth_stage=2を指定すれば、分岐ペナルティ+２も見れるし。
        m_length_id_to_next_if = -3; // STAGE_IC - STAGE_DP; // ICの位置にIAがくる
        m_length_ic_to_dp = 3; // STAGE_DP - STAGE_IC
    }

    if (m_core_type == CORE_G4KH) {
        m_dc_hit_latency = p_internals->GetDcacheHitLatency () + 1;
    } else {
        m_dc_hit_latency = p_internals->GetDcacheHitLatency () + (STAGE_WB - STAGE_EA);
    }
    m_cs_cycle_count = 0;
    m_lsu_issue_time = 0;
    m_cs_fetch_cycle_count = 0;
    first_para_group = PGRP_UNDEF;
    m_parallel_issue_flag = false;
    SetCsFuncTable ();

    m_ibuf_g3mh = new CsIbufG3MH (pe_id, m_core_type, fsctrl, this);
    m_ibuf = new CsIbuf (pe_id, m_core_type, fsctrl, this);
    m_ibuf_g3kh = new CsIbufG3KH (pe_id, m_core_type, m_core_version, fsctrl, this);

    m_rs_int.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_int[i].second = i;
    }
    m_rs_ls.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_ls[i].second = i;
    }
    m_rs_fpu.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_fpu[i].second = i;
    }
    m_rs_simd.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_simd[i].second = i;
    }
    m_rs_dp.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_dp[i].second = i;
    }
    m_rs_fxu.assign (NUM_RESERV_STATION_FXU, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION_FXU; ++ i) {
        m_rs_fxu[i].second = i;
    }
    m_rs_undef.assign (NUM_RESERV_STATION, std::make_pair (0, 0));
    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_rs_undef[i].second = i;
    }

    m_rs_dummy.assign (NUM_RESERV_STATION, 0);
    m_rs_fxumov.assign (NUM_RESERV_STATION_FXUMOV, 0);
    m_rs_fxuop.assign (NUM_RESERV_STATION_FXUOP, 0);

    m_rs_cycle.push_back (&m_rs_int);
    m_rs_cycle.push_back (&m_rs_ls);
    m_rs_cycle.push_back (&m_rs_fpu);
    m_rs_cycle.push_back (&m_rs_simd);
    m_rs_cycle.push_back (&m_rs_dp);
    m_rs_cycle.push_back (&m_rs_fxu); // Share RS in FXUOP and FXUMOV at DP stage
    m_rs_cycle.push_back (&m_rs_fxu); // Share RS in FXUOP and FXUMOV at DP stage
    m_rs_cycle.push_back (&m_rs_undef);

    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (INT)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (LS)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (FPU)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (SIMD)
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (DP)
    m_rs_fxu_cycle.push_back (&m_rs_fxumov);
    m_rs_fxu_cycle.push_back (&m_rs_fxuop);
    m_rs_fxu_cycle.push_back (&m_rs_dummy); // DUMMY (UNDEF)

    m_ex_int.assign (NUM_EX_INT, 0);
    m_ex_bru.assign (NUM_EX_BRU, 0);
    m_ex_sru.assign (NUM_EX_SRU, 0);
    m_ex_div.assign (NUM_EX_DIV, 0);
    m_ex_mul.assign (NUM_EX_MUL, 0);
    m_ex_ls.assign (NUM_EX_LS, 0);
    m_ex_fpu.assign (NUM_EX_FPU, 0);
    m_ex_simd.assign (NUM_EX_SIMD, 0);
    m_ex_dp.assign (NUM_EX_DP, 0);
    m_ex_fxumov.assign (NUM_EX_FXUMOV, 0);
    m_ex_fxuop.assign (NUM_EX_FXUOP, 0);
    m_ex_undef.assign (NUM_EX_UNDEF, 0);

    m_ex_cycle.push_back (&m_ex_int);
    m_ex_cycle.push_back (&m_ex_bru);
    m_ex_cycle.push_back (&m_ex_sru);
    m_ex_cycle.push_back (&m_ex_div);
    m_ex_cycle.push_back (&m_ex_mul);
    m_ex_cycle.push_back (&m_ex_ls);
    m_ex_cycle.push_back (&m_ex_fpu);
    m_ex_cycle.push_back (&m_ex_simd);
    m_ex_cycle.push_back (&m_ex_dp);
    m_ex_cycle.push_back (&m_ex_fxumov);
    m_ex_cycle.push_back (&m_ex_fxuop);
    m_ex_cycle.push_back (&m_ex_undef);

    for (uint32_t i = 0; i < NUM_RESERV_STATION; ++ i) {
        m_wb_int_cycle[i].clear();
    }

    m_fxu_ldv_active_cycle.assign (NUM_FXU_LDB, 4);

    m_last_ex_start_cycle.assign (EX_RES_NUM, 0);

    m_fpu_lat_offset = ForestUtil::INIT_FPU_LATENCY;
    m_divf_lat_offset = ForestUtil::INIT_DIVF_LATENCY;

    m_rn_id_avail_cycle.assign (NUM_RN_BUF, 0);
    m_curr_rn_id = 0;

    // Order is same as MemResourceType.
    //enum MemResourceType {
    //    MEM_RES_LRAM = 0, // LRAM
    //    MEM_RES_CRAM = 1, // CRAM
    //    MEM_RES_FROM = 2, // FlashROM
    //    MEM_RES_OTHERS = 3, // Peripherals
    //};
    // For G4MH
    m_interface_avail_cycle.push_back (new BusInterfaceSTO ()); // LRAM is STO
    m_interface_avail_cycle.push_back (new BusInterfaceSC ()); // CRAM is SC
    m_interface_avail_cycle.push_back (new BusInterfaceWC ()); // FROM is WC
    m_interface_avail_cycle.push_back (new BusInterfaceSC ()); // OTHERS are SC

    static const std::string resource_str[STALL_CNT_NUM] = {
        "Branch Stall", "Fetch Stall",
        "Data(ALU) Dependency", "Data(LD) Dependency", "Data(FPU) Dependency",
        "RS stall",
        "Resource Conflict"
    };

    for (int i = 0; i < STALL_CNT_NUM; i++) {
        m_stall_counter.push_back
            (new StallCounter (resource_str[i], static_cast<StallCountType>(i)));
    }

    m_is_tso = p_internals->IsTso ();
#ifdef ENABLE_EXE_ORDER_CTRL
    m_is_ooo_g4mh = p_internals->IsExecOrderOoOG4MH ();
#endif // #ifdef ENABLE_EXE_ORDER_CTRL
}


CsCtrl::~CsCtrl (void)
{
    delete m_ibuf;
    delete m_ibuf_g3mh;
    delete m_ibuf_g3kh;
    StallCountVec::iterator it = m_stall_counter.begin ();
    while (it != m_stall_counter.end ()) {
        delete *it;
        ++ it;
    }

    std::vector<BusInterfaceBase*>::iterator it_if = m_interface_avail_cycle.begin ();
    while (it_if != m_interface_avail_cycle.end ()) {
        delete *it_if;
        ++ it_if;
    }

#ifdef DEBUG_CS
    CfMsg::DPrint (MSG_ERR, "P%u:Max stall is %lu\n", m_pe_id, m_max_stall);
#endif //#ifdef DEBUG_CS
}
