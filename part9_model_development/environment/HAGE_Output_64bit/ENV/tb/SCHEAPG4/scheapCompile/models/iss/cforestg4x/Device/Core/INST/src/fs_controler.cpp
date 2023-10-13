/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <iomanip>
#include <string>
#include <sstream>
#include "./config.h"
#include "./fs_controler.h"
#include "./inst_v850.h"
#include "./trace_info.h"
#include "./trace_print.h"
#include "./gregfile.h"
#include "./sregfile.h"
#include "./mpu.h"
#include "./lsu.h"
#ifdef ENABLE_DBG_BREAK
#include "./dbg_break.h"
#endif // #ifdef ENABLE_DBG_BREAK
#include "./inst_queue.h"
#include "./btb_g3h.h"
#include "./breakpoint.h"
#include "./exp_info.h"
#include "./statistics.h"
#include "./cs_controler.h"
#include "./llsc.h"
#include "./forest_internals.h"
#include "./forest_message.h"
#include "./exp_message.h"
#include "./proc_element.h"
#include "./counter_base.h"
#include "./icache.h"

#ifdef SUPPORT_SC_HEAP
#include "./suspend_ctrl.h"
#endif // SUPPORT_SC_HEAP

/*!
 * @brief 機能モード：1ステップ実行
 * @param void
 * @return void
 * Snooze/例外割込発生/実行可能、以外の状態ではここには来ない
 */
IsElapsedTime FsCtrl::RunModule (void)
{
    if (unlikely (IsSnooze () == true) ) {
        // Snooze状態ならcpu_time＆snoozeカウント更新
        ClockWait (1);
        UpdateSnoozeState ();
        return TIME_ELAPSED;
    }

#ifdef ENABLE_TRACE_BUF
    if (m_suspend_ctrl->GetSuspendState () == NO_SUSPEND) {
        TraceCtrl* trace = GetNewTrace ();
        if (trace == NULL) {
            p_internals->SetBreakTraceFull ();
            return TIME_NOT_ELAPSED;
        }
        SetTraceCtrl (trace);
    }
#endif // ENABLE_TRACE_BUF

    IntFactorType factor = GetSelectedIntFactor ();

#ifdef DEBUG_CORE_INT
#ifdef SUPPORT_SC_HEAP
    if (m_suspend_ctrl->GetSuspendState () == NO_SUSPEND) {
#else // #ifdef SUPPORT_SC_HEAP
    {
#endif // #ifdef SUPPORT_SC_HEAP
        CfMsg::TPrint (MSG_ERR, m_trace,
                       "[Core]RunModule: GetSelectedIntFactor ()=%x\n",
                       GetSelectedIntFactor ());
    }
#endif // #ifdef DEBUG_CORE_INT

#ifdef SUPPORT_SC_HEAP
    if (unlikely ( (factor != INT_FAC_NONE)
                && (m_suspend_ctrl->GetSuspendState () == NO_SUSPEND) ) ) { // 命令先頭でのみ受付
#else // SUPPORT_SC_HEAP
    if ((unlikely (factor != INT_FAC_NONE) && (m_all_interrupt_disable == false)) ) {
#endif // SUPPORT_SC_HEAP
        m_trace->Clean ();
        if (factor != INT_FAC_FPI) {
            // 割り込み処理
            m_trace->SetInstId (INST_ID_V_0); // 実行前ブレーク用に命令情報をクリア
#ifdef ENABLE_DBG_PMC
            m_previous_is_dbmode = m_is_dbmode;
#endif // #ifdef ENABLE_DBG_PMC
            InterruptProcess ();
        } else {
            // FPU Imprecise Exception (FPI) processing

            // The inst_addr value is saved to xxPC when FPI exception is accepted.
            // Copy next_inst_addr to inst_addr.
            m_trace->SetInstAddr (m_trace->GetNextInstAddr ());

            m_srf->SrFPEC()->SetFPIVD(0);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPEC, m_srf->SrFPEC()->ReadBody());
            m_trace->SetExpCode (EXP_FPI);
            ExceptionProcess (EXP_FPI);
        }
        m_is_prev_branched = true;     // 後続命令のための情報
    } else {
        // 1ステップ実行
#ifdef SUPPORT_SC_HEAP
        if (m_suspend_ctrl->GetSuspendState () <= RELEASED_FS)
#endif // SUPPORT_SC_HEAP
        {
            FsStepExecute ();
#ifdef SUPPORT_SC_HEAP
            if (m_suspend_ctrl->GetSuspendState () != RELEASED_FS) {
                return TIME_NOT_ELAPSED; // FS実行が未完了
            }
#endif // SUPPORT_SC_HEAP
        }
    }

    // サイクル計算
    IsElapsedTime is_elapsed_time;
#ifdef ENABLE_FS_MODE
    if (m_simmode == FS_MODE) {
       ClockWait (1);
        is_elapsed_time = TIME_ELAPSED;
#ifdef SUPPORT_SC_HEAP
        m_suspend_ctrl->SetSuspendState (NO_SUSPEND);
#endif // SUPPORT_SC_HEAP
#ifdef ENABLE_TRACE_BUF
        // 命令実行が終わったので、トレースをキューに積んでDequeue可能にする
        p_internals->QueueTrace (m_trace);
#endif // ENABLE_TRACE_BUF
    } else
#endif // ENABLE_FS_MODE
    {
#ifdef SYNC_PE_LSU_TIME
        uint64_t pre_ht_time = std::max (GetCpuTime(), GetBusTime());
#endif // #ifdef SYNC_PE_LSU_TIME

        m_csctrl->CsTraceSimulate (m_trace);
#ifdef SUPPORT_SC_HEAP
        if (m_suspend_ctrl->GetSuspendState () != NO_SUSPEND) {
            return TIME_NOT_ELAPSED;
        }
#endif // SUPPORT_SC_HEAP
#ifdef ENABLE_TRACE_BUF
        // 命令実行が終わったので、トレースをキューに積んでDequeue可能にする
        p_internals->QueueTrace (m_trace);
#endif // ENABLE_TRACE_BUF
        if (m_trace->IsParallel () == true) {
            ++ m_para_issue_count;
        }
        uint64_t stall_cycle = m_csctrl->CalcStallCount ();
        if (stall_cycle != 0) {
            ClockWait (stall_cycle);
            is_elapsed_time = TIME_ELAPSED;
            if (IsSnooze ()) {
                AdjustSnoozeTime ();
            }
        } else {
            is_elapsed_time = TIME_NOT_ELAPSED; // 同時発行により時間経過なし
        }
#ifdef SYNC_PE_LSU_TIME
        SetBusTime (m_csctrl->GetLsuIssueTime());
        if (pre_ht_time < std::max (GetCpuTime(), GetBusTime())) {
            is_elapsed_time = TIME_ELAPSED;
        } else {
            // When SYNC is executed, ht order should be sync (other CPU or peripherals should run).
            // But, if LSU time is much larger than syncm stall cycle, others may not run.
            // To prevent it, don't overwrite by TIME_NOT_ELAPSED.
            //is_elapsed_time = TIME_NOT_ELAPSED;
        }
#endif //#ifdef SYNC_PE_LSU_TIME
    }

    CountPerformance (); // Count PmCtrl events.
    if (unlikely (m_update_pmc) ) {
        UpdateCounters ();
    }

    // デバッグモードならトレース情報を表示
    m_trace_print->Print (m_simmode, m_dbgmode, m_trace);

    return is_elapsed_time;
}


#ifdef SUPPORT_SC_HEAP
void FsCtrl::FsStepExecute (void)
{
    if (m_suspend_ctrl->GetSuspendState () == NO_SUSPEND) {
        // 不要トレース情報の初期化
        m_trace->Clean ();

        // 命令実行処理
        StageIF ();
        StageID ();

        // 使用される汎用レジスタが未完了の以前のロード結果に依存するなら中断
        m_suspend_ctrl->WaitForRegDepend (m_trace);
    }

    // 全レジスタ依存が解決済みなら命令実行
    if (m_suspend_ctrl->GetSuspendState () == RELEASED_FS_EXEC) {
        StageEX ();

        // RMW命令など自己依存があるなら中断
        if (m_suspend_ctrl->HasSelfDepend ()) {
            m_suspend_ctrl->WaitForRegDependSelf (m_trace);
        } else {
            m_suspend_ctrl->SetSuspendState (RELEASED_FS);
        }
    }

    // FSについてはここ以降で中断しない
    if (m_suspend_ctrl->GetSuspendState () == RELEASED_FS) {
        // StageEXが中断された場合、中断された箇所以降を実行。そうでない命令なら何もしない。
        // @@ ここに置くと、全命令で通る。Self依存のものだけに限定すべき。
        m_suspend_ctrl->ResumeExec (m_trace);

        // 共通処理
        m_trace->IncStepCount ();

#ifdef ENABLE_PIC
        m_statistics->CountInstructionCount (m_trace);
#endif // ENABLE_PIC
    }
}

#else // SUPPORT_SC_HEAP

void FsCtrl::FsStepExecute (void)
{
    // 不要トレース情報の初期化
    m_trace->Clean ();

    // 命令実行処理
    StageIF ();
    StageID ();
    StageEX ();

    m_trace->IncStepCount ();
#ifdef ENABLE_PIC
    m_statistics->CountInstructionCount (m_trace);
#endif // ENABLE_PIC
}
#endif // SUPPORT_SC_HEAP


/*!
 * @brief フェッチ処理
 * @par 更新トレース
 *      - inst_addr/paddr
 *      - fetch_addr/paddr
 *      - inst_code
 *      - exp_code
 */
void FsCtrl::StageIF (void)
{
    PhysAddr inst_addr = m_grf->ReadPC ();
    m_trace->SetInstAddr (inst_addr);

    m_inst_queue->InitSyserrTotal ();
    m_inst_queue->InitIcHitTotal ();

#ifdef ENABLE_DBG_BREAK
    if (m_dbgbrk->UpdateBreakPcbExcp (inst_addr, m_trace) != EXP_NONE) {
        m_trace->SetInstCode (0);
        return;
    }
#endif //#ifdef ENABLE_DBG_BREAK

    // 前の命令が分岐命令ならフェッチを発行
    // @@ キュー内前方分岐では不要になる
    // @@ 予測ミスの場合、命令キューはクリアされるのでフェッチ必要。
    //    この際に命令バッファからフェッチすることがある。
    // @@ また、リフェッチ命令の条件判定を追加
    // @@ デバイス仕様FIX後に実装する
    if (m_is_prev_branched == true) {
        m_inst_queue->IssueFetch (inst_addr);
    }

    m_inst_queue->FetchOpcode (inst_addr);

    // @@ 48bit超部分はデコードの後に実行
    // @@ RTL動作とは異なるので、CSでは辻褄をあわせること
}


/*!
 * @brief デコード処理
 * @par 更新トレース
 *      - inst_id
 *      - exp_code
 */
void FsCtrl::StageID (void)
{
#ifdef ENABLE_DBG_BREAK
    ExpCode exp_code = m_trace->GetExpCode ();
    if (unlikely (exp_code == EXP_PCB_DB)) {
        m_trace->SetInstId (INST_ID_V_0);
        m_trace->SetInstSize (0);
        return;
    }
#endif // #ifdef ENABLE_DBG_BREAK

    uint64_t inst_code = m_trace->GetInstCode();
    uint32_t inst_code32 = static_cast<uint32_t> (inst_code);

    uint32_t inst_id = MicroArchDB::GetInstID (inst_code32);
    m_trace->SetInstId (inst_id);
    // デコード失敗時は INST_ID_V_0 が格納されている

    //! 48/64bit命令の場合、オペランド部を追加でフェッチする\n
    //! オペランド部で例外発生時は inst_idが INST_ID_V_0 となる
    //! @sa MemInstFetch
    ByteSize inst_size;


    if (likely (inst_id != 0)) {
        inst_size = MicroArchDB::GetInstSize (inst_id);
    } else {
        if (m_inst_queue->IsInstSize16Bit (inst_code & 0xFFFFU)) {
            inst_size = SIZE_16BIT;
        } else if (m_inst_queue->IsInstSize48Bit (inst_code & 0xFFFFFFFFU)) {
            inst_size = SIZE_48BIT;
        } else {
            inst_size = SIZE_32BIT;
        }
    }

    m_trace->SetInstSize (inst_size);
    if (unlikely (inst_size >= SIZE_48BIT)) {
        PhysAddr inst_addr = m_trace->GetInstAddr ();
        uint64_t inst_code48 = m_inst_queue->FetchData (inst_addr + 4);
        inst_code |= (inst_code48 << 32);
        if (inst_size == SIZE_64BIT) {
            uint64_t inst_code64 = m_inst_queue->FetchData (inst_addr + 6);
            inst_code |= (inst_code64 << 48);
        }
        m_trace->SetInstCode (inst_code);
    }

    m_trace->SetInstIcHit (m_inst_queue->IsIcHitTotal ());

    // 分岐タイプを記録
    m_branch_type = MicroArchDB::GetBranchType (inst_id, m_archdb_id);

#ifdef STUDY_GHR_64BIT
    PhysAddr end_addr = (m_trace->GetInstAddr () + inst_size - 1) & 0xFFFFFFF8;
    if (m_line_addr != end_addr) {
        m_btb->UpdateGhr (0);
        m_line_addr = end_addr;
    }
#endif // STUDY_GHR_64BIT
}


/*!
 * @brief  命令実行処理
 * @param  void
 * @return void
 * @note
 * フェッチ/デコードで例外が発生していてもエラー判定せずに実行してOK。
 * because inst_id が INST_ID_V_0 もしくは NOP のため実行しても副作用ない。
 * @sa MemInstFetch
 * @par 更新トレース
 *      - rreg/wreg_*
 *      - rmem/wmem_*
 *      - priv_level
 *      - exp_code
 *      - is_branched
 *      - next_inst_addr
 */
void FsCtrl::StageEX (void)
{
    uint32_t inst_id = m_trace->GetInstId ();
    ByteSize inst_size = m_trace->GetInstSize ();
#ifdef ENABLE_DBG_PMC
    m_previous_is_dbmode = m_is_dbmode;
#endif // #ifdef ENABLE_DBG_PMC

#ifdef ENABLE_DBG_BREAK
    ExpCode exp_code_pcb = m_trace->GetExpCode ();
    if (likely (exp_code_pcb != EXP_PCB_DB))
#endif // #ifdef ENABLE_DBG_BREAK
    {
        // 例外優先度はMIP->SYSERR->UCPOP/PIE
        if (m_lsu->GetMpuPtr()->IsMpuPermitted<INST_READ> (m_trace->GetInstAddr(),
                                                           inst_size) == false) {
            // MIP is detected.
            if ((m_pe->IsHvEnabled () == true) && (m_pe->IsHostMode () == false)) {
                // Guest mode
                //   Check MIP again for each 16bit
                //   to judge MIP by host managed entry or MIP by guest managed entry.
                //   Lower address MIP is prior to higher address MIP.
                PhysAddr mip_addr = m_trace->GetInstAddr();
                for (ByteSize pos = 2; pos <= inst_size; pos += 2) {
                    if (m_lsu->GetMpuPtr()->IsMpuPermitted<INST_READ> (mip_addr, pos) == false) {
                        // Found MIP at lowest addr
                        break;
                    }
                }
            } else {
                // Host mode or Conventional mode
                // - Host mode
                //   Because all entry is host managed entry, no need to check for each 16bit.
                // - Conventional mode
                //   Because all entry is guest managed entry, no need to check for each 16bit
            }
            CfMsg::TPrint (MSG_INF, m_trace,
                           "<Memory Protection Error: 0x%08x>\n", m_trace->GetInstAddr());
            // すでに RIE,UCPOP,PIEが発生していても上書きされる
            // @@ わかりにくいので場所を変えたほうがいいかも
            //MIP例外が発生したPCをMEAに設定
            if (m_lsu->GetMpuPtr()->GetMpuExpPOS () == Mpu::M_GUESTENT) {
                m_trace->SetExpCode (EXP_MIPGM);
            } else {
                m_trace->SetExpCode (EXP_MIPHM);
            }

            // Update MEA when the exception is accepted by host mode or conventional mode.
            // Update GMMEA when the exception is accepted by guest mode.
            bool accepted_by_host;
            if (m_pe->IsHvEnabled () == false) {
                // conventional mode
                accepted_by_host = true;
            } else {
                accepted_by_host = false;
                if (m_pe->IsHostMode ()) {
                    accepted_by_host = true;
                }
                ExpCode exp_code = m_trace->GetExpCode ();
                if ((exp_code == EXP_MIPHM) && (m_nc_srf->SrGMCFG()->GetHMP () == 1)) {
                    accepted_by_host = true;
                }
                if ((exp_code == EXP_MIPGM) && (m_nc_srf->SrGMCFG()->GetGMP () == 1)) {
                    accepted_by_host = true;
                }
            }
            if (accepted_by_host) {
                m_nc_srf->SrHMMEA()->WriteBody (m_trace->GetInstAddr());
                m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_HMMEA, m_trace->GetInstAddr());
            } else {
                m_nc_srf->SrGMMEA()->WriteBody (m_trace->GetInstAddr());
                m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_GMMEA, m_trace->GetInstAddr());
            }
        } else {
            SysErrCause cause = m_inst_queue->GetSyserrTotal ();
            if (cause != SYSERR_NONE) {
                // PEG/CFG/GRG違反：SYSERR（FETCH）の発生
                // - Fetch起因は自コンテキストで受付
                m_syserr_fetch_cause = cause;
                m_trace->SetExpCode (EXP_SYSERR_IF);

                // 実行命令をNOP相当に変更
                // ただし、命令ログにNOPを表示したくないので、traceにはINST_ID_V_0を記録
                inst_id = INST_ID_V_NOP;
                m_trace->SetInstId (INST_ID_V_0);
                m_trace->SetCancel (true);
            } else {
                // UCPOP/PIEの判定
                SetPreExecException (inst_id);
            }
        }
    }

    ExpCode exp_code = m_trace->GetExpCode ();
    if (likely (exp_code == EXP_NONE)) {
        // 命令を実行
        // @@ 引数種別が多いので命令側で引数切り出しする方式を採用。
        // @@ 引数切り出しコードが各命令に必要となりコードサイズ増えるが、
        // @@ 関数コール時の引数が 1つですむ。
        // @@ トータル性能への影響はほぼ無いと判断。
        // @@ デコードキャッシュ適用時は再検討必要。
        uint64_t inst_code = m_trace->GetInstCode();
        m_insts->InstExec (inst_id, inst_code);
        exp_code = m_trace->GetExpCode ();
    }

#if defined(COMP_RUNNER)
    if (m_trace->IsCancelMcyc () == true) {
        m_dbgbrk->ClearPcbEoReserved (m_trace);
        m_dbgbrk->ClearLsabEoReserved (m_trace);
        return;
    }
#endif // #if defined(COMP_RUNNER)

#ifdef ENABLE_DBG_BREAK
    // Update BPC.EO
    // - When IsCancelMcyc==true, don't update BPC.EO for PCB.
    //   LSAB event for accessed access is updated in inst_v850.cpp.
    ExpCode exp_code_eo = m_trace->GetExpCode ();
    if (   (ExpInfo::GetExpType (exp_code_eo) != ExpInfo::EXP_T_REEXE)
        || (exp_code_eo == EXP_PCB_DB)
        || (exp_code_eo == EXP_LSAB_DB)
        || (exp_code_eo == EXP_LSAB_EIINT)) {
        m_dbgbrk->ApplyPcbEoReserved (m_trace);
        m_dbgbrk->ApplyLsabEoReserved (m_trace);
    } else {
        m_dbgbrk->ClearPcbEoReserved (m_trace);
        m_dbgbrk->ClearLsabEoReserved (m_trace);
    }
#endif // #ifdef ENABLE_DBG_BREAK

    if (likely (exp_code == EXP_NONE)) {
        // 正常終了
        UpdatePC ();

    } else {
        if (ExpInfo::GetExpType (exp_code) == ExpInfo::EXP_T_CMPLT) {
            // 完了型例外
            // 例外処理は実施済み
            UpdatePC ();
        } else {
            // 中断型、再実行型例外
            // PCは更新せずに例外処理を実施
            ExceptionProcess (exp_code);
            m_is_prev_branched = true;     // 後続命令のための情報
        }
    }
}


void FsCtrl::CountPerformance(void)
{
    // Count for PMCTRL
    // Not count here
    // 00h : Cycle count (in cs_controler.cpp)
    // 10h : instruction count (in fs_controler.cpp)
    // 28h : Cycle count except interruption process (in sreg_special.cpp)
    // 29h : Cycle count with PSW.ID=1 and ISPR=0 (in sreg_special.cpp)
    // 30h : Fetch count (in lsu.cpp)
    // 31h : IC hit count (in icache.cpp)
    // 40h : stall by dispatch (in cs_controler.cpp)
    // 50h : Fetch request to FlashROM (in lsu.cpp for fetch and inst_v850.cpp for CACHE/PREF inst)
    // 51h : Data access request to FlashROM (in lsu.cpp)
#ifdef ENABLE_DBG_PMC
    if (unlikely (m_previous_is_dbmode) ) {
        // uncounted.
        return;
    }
#endif //#ifdef ENABLE_DBG_PMC

    BranchInfo branch_info = m_trace->GetBranchInfo ();
    if (branch_info != NO_BRANCH) {
        if ((branch_info == BRANCH) || (branch_info == RTE_BRANCH)) {
            // Count taken BRANCH except INT/EXPtype branch
            // Becasue INT/EXP type branches have GetBranchInfo()=EXP_BRANCH/INT_BRANCH,
            // - INT/EXP type branches don't come here.
            // - NotTaken Bcond/loop aren't counted (they not have GetBranchInfo () == BRANCH/RTE_BRANCH)
            // - DBRET isn't counted
            uint32_t inst_id = m_trace->GetInstId ();
            switch (inst_id) {
#ifdef ENABLE_DBG_PMC
            case INST_ID_V_DBRET:
                break;
#endif // #ifdef ENABLE_DBG_PMC
            default:
                ++ m_branch_count; // PMCTRL.CND=18h
                break;
            }
        } else if (unlikely (branch_info == EXP_BRANCH)) {
            if (m_trace->GetInstId () == INST_ID_V_SYSCALL) {
                ++ m_branch_count; // PMCTRL.CND=18h
            }
        }

        if (unlikely (branch_info == INT_BRANCH)) {
            // FEINT/GMFEINT/BGFEINT/EIINT/GMFEINT/GMBGINT/DBINT/DBNMI/FENMI comes here.
            if ((m_core_type != CORE_G4MH) || (m_core_version < CORE_VER_20)) {
                ExpCode exp_code = m_trace->GetExpCode();
                if (exp_code == EXP_EIINT) {
                    // Count EIINT
                    uint64_t eiint_count = m_eiint_count[0] + 1;
                    m_eiint_count.assign (4, eiint_count); // PMCTRL.CND=20h
                } else if (exp_code == EXP_FEINT) {
                    // Count FEINT
                    uint64_t feint_count = m_feint_count[0] + 1;
                    m_feint_count.assign (4, feint_count); // PMCTRL.CND=21h
                }
            } else {
                uint32_t gpid = m_nc_srf->SrPSWH()->GetGPID ();
                bool is_host = (m_pe->IsHostMode () || (m_pe->IsHvEnabled() == false));
                uint32_t ch = 0;
                bool is_conventional_mode = (m_pe->IsHvEnabled() == false);
                std::vector <PmCounter*>::iterator it = m_pmcount.begin ();
                while (it != m_pmcount.end ()) {
                    bool is_count_host;
                    if (   ((is_host == true) && ((*it)->GetHeBody () == true))
                        || is_conventional_mode) {
                        // Host mode && count target for host is enabled, or conventional mode.
                        is_count_host = true;
                    } else {
                        // Guest mode
                        is_count_host = false;
                    }
                    bool is_count_guest;
                    if (   ((is_host == false) && ((*it)->IsGpidMatchBody (gpid)))
                        || is_conventional_mode) {
                        // Guest mode && count target for guest is enabled, or conventional mode.
                        is_count_guest = true;
                    } else {
                        // Host mode
                        is_count_guest = false;
                    }
                    ExpCode exp_code = m_trace->GetExpCode();
                    if ((exp_code == EXP_EIINT) && is_count_host) {
                        // Count EIINT
                        uint32_t subcond = (*it)->GetSubConditionBody ();
                        if ((((subcond >> 12) & 0x1) == 0) || ((subcond & 0xFFF) == m_accepted_eiint_ch)) {
                            ++ m_eiint_count[ch]; // PMCTRL.CND=20h (for HE=1)
                        }
                    } else if ((exp_code == EXP_GMEIINT) && is_count_guest) {
                        // Count GMEIINT
                        uint32_t subcond = (*it)->GetSubConditionBody ();
                        if ((((subcond >> 12) & 0x1) == 0) || ((subcond & 0xFFF) == m_accepted_eiint_ch)) {
                            ++ m_eiint_count[ch]; // PMCTRL.CND=20h (for GE=1)
                        }
                    } else if ((exp_code == EXP_FEINT) && is_count_host) {
                        // Count FEINT
                        uint32_t subcond = (*it)->GetSubConditionBody ();
                        if ((((subcond >> 4) & 0x1) == 0) || ((subcond & 0xF) == m_accepted_feint_ch)) {
                            ++ m_feint_count[ch]; // PMCTRL.CND=21h (for HE=1)
                        }
                    } else if ((exp_code == EXP_GMFEINT) && is_count_guest) {
                        // Count GMFEINT
                        uint32_t subcond = (*it)->GetSubConditionBody ();
                        if ((((subcond >> 4) & 0x1) == 0) || ((subcond & 0xF) == m_accepted_feint_ch)) {
                            ++ m_feint_count[ch]; // PMCTRL.CND=21h (for GE=1)
                        }
                    }
                    ++ ch;
                    ++ it;
                }
            }
        }
    }

    BpResult bp_result = m_trace->GetBpResult();
    if (bp_result != BP_NONE) {
        // Count Bcond and Loop (BR is included)
        uint32_t inst_id = m_trace->GetInstId ();
        switch (inst_id) {
        case INST_ID_V_BCOND_D9:
        case INST_ID_V_BCOND_D17:
        case INST_ID_V_LOOP:
            ++ m_branch_cond_exec_count; // PMCTRL.CND=19h
            if (m_trace->IsInstIcHit() == false) {
                // Although CForest enables branch prediction always,
                // branch prediction is disabled in following case actually.
                // - ICache is disabled or the branch instruction is ICache miss or uncachable
                // So, when Branch prediction is disabled, prediction result is Not Taken.
                // Then Taken Branch is counted in BP_MISS.
                if (m_trace->IsBranched()) {
                    ++ m_branch_cond_miss_count; // PMCTRL.CND=1Ah
                } else {
                    // BP hit because of NTKN branch.
                }
            } else if (bp_result == BP_MISS) {
                // When ICache hit, BP result in the trace is used.
                ++ m_branch_cond_miss_count; // PMCTRL.CND=1Ah
            }
            break;
        case INST_ID_V_BR:
            ++ m_branch_cond_exec_count; // PMCTRL.CND=19h
            // BP_MISS (PMCTRL.CND=1Ah) isn't counted by BR inst.
            break;
        default:
            break;
        }
    }

    ExpCode exp_code = m_trace->GetExpCode();
    if (unlikely (exp_code != EXP_NONE) ) {
        if ((m_core_type != CORE_G4MH) || (m_core_version < CORE_VER_20)) {
            // DB type is counted also.
            if (ExpInfo::GetExpType (exp_code) == ExpInfo::EXP_T_ABORT) {
                // Count abort type exceptions
                uint64_t exp_abort_count = m_exp_abort_count[0] + 1;
                m_exp_abort_count.assign (4, exp_abort_count); // PMCTRL.CND=22h
            } else {
                // Count reexec and complete type exceptions
                uint64_t exp_not_abort_count = m_exp_not_abort_count[0] + 1;
                m_exp_not_abort_count.assign (4, exp_not_abort_count); // PMCTRL.CND=23h
            }
        } else {
            // G4MH2.0
            uint32_t gpid = m_nc_srf->SrPSWH()->GetGPID ();
            bool is_host = (m_pe->IsHostMode () || (m_pe->IsHvEnabled() == false));
            uint32_t ch = 0;
            bool is_host_syserr;
            if (m_pe->IsHvEnabled ()) {
                if ((is_host == true) || (m_nc_srf->SrGMCFG()->GetGSYSE () == 1)) {
                    is_host_syserr = true;
                } else {
                    is_host_syserr = false;
                }
            } else {
                is_host_syserr = true;
            }

            bool is_host_mxphm;
            bool is_host_mxpgm;
            if (m_pe->IsHvEnabled ()) {
                if ((is_host == true)
                    || (  (m_nc_srf->SrGMCFG()->GetHMP () == 1)
                       && ((exp_code == EXP_MIPHM) || (exp_code == EXP_MDPHM) || (exp_code == EXP_MDPHM_GMEIINT)))) {
                    is_host_mxphm = true;
                } else {
                    is_host_mxphm = false;
                }
                if ((is_host == true)
                    || (  (m_nc_srf->SrGMCFG()->GetGMP () == 1)
                       && ((exp_code == EXP_MIPGM) || (exp_code == EXP_MDPGM) || (exp_code == EXP_MDPGM_GMEIINT)))) {
                    is_host_mxpgm = true;
                } else {
                    is_host_mxpgm = false;
                }
            } else {
                is_host_mxphm = true;
                is_host_mxpgm = true;
            }
            bool is_conventional_mode = (m_pe->IsHvEnabled() == false);
            std::vector <PmCounter*>::iterator it = m_pmcount.begin ();
            while (it != m_pmcount.end ()) {
                bool is_count_host;
                if (   ((is_host == true) && ((*it)->GetHeBody () == true))
                    || is_conventional_mode) {
                    // Host mode && count target for host is enabled, or conventional mode.
                    is_count_host = true;
                } else {
                    // guest mode
                    is_count_host = false;
                }
                bool is_count_guest;
                if (   ((is_host == false) && ((*it)->IsGpidMatchBody (gpid)))
                    || is_conventional_mode) {
                    // Guest mode && count target for guest is enabled, or conventional mode.
                    is_count_guest = true;
                } else {
                    // Host mode
                    is_count_guest = false;
                }
                // DB type is counted also.
                if (ExpInfo::GetExpType (exp_code) == ExpInfo::EXP_T_ABORT) {
                    // Count abort type exceptions
                    bool is_count = true;
                    if (!is_count_guest) {
                        if ((exp_code == EXP_GMEIINT) || (exp_code == EXP_GMFEINT)) {
                            is_count = false;
                        }
                        if ((is_host_syserr == false) && ((exp_code == EXP_SYSERR_RB) || (exp_code == EXP_SYSERR_GM))) {
                            is_count = false;
                        }
                        if ((is_host_mxphm == false) && ((is_host == false) && (exp_code == EXP_MDPHM_GMEIINT))) {
                            is_count = false;
                        }
                        if ((is_host_mxpgm == false) && ((is_host == false) && (exp_code == EXP_MDPGM_GMEIINT))) {
                            is_count = false;
                        }
                    }
                    if (!is_count_host) {
                        if ((exp_code == EXP_EIINT) || (exp_code == EXP_FEINT) || (exp_code == EXP_FENMI)
                            || (exp_code == EXP_BGEIINT) || (exp_code == EXP_BGFEINT)
                            || (exp_code == EXP_DBNMI) || (exp_code == EXP_DBINT) || (exp_code == EXP_LSAB_EIINT)
                            || (exp_code == EXP_MDPHM_EIINT) || (exp_code == EXP_MDPGM_EIINT)) {
                            is_count = false;
                        }
                        if ((exp_code == EXP_SYSERR_HM)
                        ||  (is_host_syserr && ((exp_code == EXP_SYSERR_RB) || (exp_code == EXP_SYSERR_GM)))) {
                            is_count = false;
                        }
                        if ((is_host_mxphm == true) && ((is_host == true) && (exp_code == EXP_MDPHM_GMEIINT))) {
                            is_count = false;
                        }
                        if ((is_host_mxpgm == true) && ((is_host == true) && (exp_code == EXP_MDPGM_GMEIINT))) {
                            is_count = false;
                        }
                    }
                    if (is_count) {
                        ++ m_exp_abort_count[ch]; // PMCTRL.CND=22h
                    }
                } else {
                    // Count reexec and complete type exceptions
                    bool is_count = true;
                    if (!is_count_guest) {
                        if ((is_host_syserr == false) && (is_host == false) && ((exp_code == EXP_SYSERR_IF) || (exp_code == EXP_SYSERR_EX))) {
                            is_count = false;
                        }
                        if ((is_host_mxphm == false) && ((is_host == false) && ((exp_code == EXP_MIPHM) || (exp_code == EXP_MDPHM)))) {
                            is_count = false;
                        }
                        if ((is_host_mxpgm == false) && ((is_host == false) && ((exp_code == EXP_MIPGM) || (exp_code == EXP_MDPGM)))) {
                            is_count = false;
                        }
                        if ((is_host == false) && ((exp_code == EXP_RIE) || (exp_code == EXP_UCPOP0) || (exp_code == EXP_UCPOP1)
                            || (exp_code == EXP_UCPOP2) || (exp_code == EXP_PIE) || (exp_code == EXP_MAE)
                            || (exp_code == EXP_FPE) || (exp_code == EXP_FXE) || (exp_code == EXP_SYSCALL)
                            || (exp_code == EXP_FETRAP) || (exp_code == EXP_TRAP0) || (exp_code == EXP_TRAP1)) ) {
                            // counted by current mode
                            is_count = false;
                        }
                    }
                    if (!is_count_host) {
                        if (   (exp_code == EXP_PCB_DB) || (exp_code == EXP_LSAB_DB) || (exp_code == EXP_AE)
                            || (exp_code == EXP_DBTRAP) || (exp_code == EXP_HVTRAP)) {
                            // Always goto host
                            is_count= false;
                        }
                        if (is_host_syserr && (is_host == true) && ((exp_code == EXP_SYSERR_IF) || (exp_code == EXP_SYSERR_EX))) {
                            is_count = false;
                        }
                        if ((is_host_mxphm == true) && ((is_host == true) && ((exp_code == EXP_MIPHM) || (exp_code == EXP_MDPHM)))) {
                            is_count = false;
                        }
                        if ((is_host_mxpgm == true) && ((is_host == true) && ((exp_code == EXP_MIPGM) || (exp_code == EXP_MDPGM)))) {
                            is_count = false;
                        }
                        if ((is_host == true) && ((exp_code == EXP_RIE) || (exp_code == EXP_UCPOP0) || (exp_code == EXP_UCPOP1)
                            || (exp_code == EXP_UCPOP2) || (exp_code == EXP_PIE) || (exp_code == EXP_MAE)
                            || (exp_code == EXP_FPE) || (exp_code == EXP_FXE) || (exp_code == EXP_SYSCALL)
                            || (exp_code == EXP_FETRAP) || (exp_code == EXP_TRAP0) || (exp_code == EXP_TRAP1)) ) {
                            // counted by current mode
                            is_count = false;
                        }
                    }
                    if (is_count) {
                        ++ m_exp_not_abort_count[ch]; // PMCTRL.CND=23h
                    }
                }
                ++ ch;
                ++ it;
            }
        }
    }
}


/*!
 * @brief  PC更新処理
 * @param  void
 * @return void
 * @par 更新トレース
 *      - next_inst_addr
 */
void FsCtrl::UpdatePC (void)
{
    PhysAddr curr_inst_pc = m_trace->GetInstAddr();
    m_last_completed_pc = curr_inst_pc; // Keep for HMFEPC after reset

    PhysAddr next_pc;
    if ( m_branch_type != BR_NOT && m_trace->IsBranched() ) {
        // 分岐成功: PCは命令実行時に更新済み
        next_pc = m_grf->ReadPC ();
        m_is_prev_branched = true;     // 後続命令のための情報
    } else {
        // 通常命令 or 分岐失敗
        ByteSize inst_size = m_trace->GetInstSize ();
        next_pc = curr_inst_pc + inst_size;
        m_grf->WritePC (next_pc);
        if (m_trace->GetBpResult () != BP_MISS) {
            m_is_prev_branched = false;    // 後続命令のための情報
        } else {
            m_is_prev_branched = true;    // 後続命令のための情報
        }
    }
    m_trace->SetNextInstAddr (next_pc);
#ifdef STUDY_GHR_64BIT
    next_pc &= 0xFFFFFFF8;
    if (m_line_addr != next_pc) {
        m_btb->UpdateGhr (0);
        m_line_addr = next_pc;
    }
#endif // STUDY_GHR_64BIT

#ifdef ENABLE_DBG_PMC
    if (likely (m_previous_is_dbmode == false) ) {
#else //#ifdef ENABLE_DBG_PMC
    {
#endif // #ifdef ENABLE_DBG_PMC
        ++ m_inst_count_pmc;
    }
}


/*!
 * @brief  中断型例外判定 (命令実行前)
 * @param  inst_id
 * @return void
 * @note
 * 判定対象は PIE と UCPOP
 * @par 更新トレース
 *      - exp_code
 */
void FsCtrl::SetPreExecException (int32_t inst_id)
{
#ifdef ENABLE_DBG_BREAK
    if (m_trace->GetExpCode () != EXP_NONE) {
        // 既にPCブレークにヒットしていたら例外判定を実行しない
        //@@ 書き換える
        return;
    }
#endif //#ifdef ENABLE_DBG_BREAK

    //@@ 全ての命令で判定するのはもったいない
    //@@ 判定が必要な命令のみでよいはず
    //@@ 関数ポインタをもたせるべきか
    if (IsExcpCoproOccured (inst_id) == true) {
        return;
    }
    if (IsExcpPrivOccured (inst_id) == true) {
        return;
    }
}


/*!
 * @brief  コプロセッサ使用不可例外判定
 * @param  inst_id
 * @return [true] 例外発生  [false] 正常終了
 */
bool FsCtrl::IsExcpCoproOccured (int32_t inst_id) const
{
    // @@ 関数ポインタ方式に全面差替え
    // @@ それまでは汚いけど放置
    CoproType copro_type = MicroArchDB::GetCoproType (inst_id);
    if (copro_type == COPRO_NO) {
        return false;

    } else if (copro_type == COPRO_FPU) {
        uint32_t cu = m_srf->SrVPSW()->GetCU ();
        if ((cu & 0x1) == 0) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Copro Error: FPU : PSW.CU = %d>\n", cu);
            m_trace->SetExpCode (EXP_UCPOP0);
            return true;
        } else {
            if (m_core_type == CORE_G4KH) {
                const uint32_t FPU_D_SHIFT = 20;
                const uint32_t FPU_D_MASK_BIT = 0x1;
                const uint32_t FPU_D_ENABLE = 1;
                bool type_fpu_d = (((m_trace->GetInstCode() >> FPU_D_SHIFT) & FPU_D_MASK_BIT) == FPU_D_ENABLE);
                if (type_fpu_d) {
                    CfMsg::TPrint (MSG_INF, m_trace, "<Copro Error: FPU : Double Precision Instruction>\n");
                    m_trace->SetExpCode (EXP_UCPOP0);
                    return true;
                }
            }
            return false;
        }

    } else if (copro_type == COPRO_SIMD) {
        uint32_t cu = m_srf->SrVPSW()->GetCU ();
        if ((cu & 0x2) == 0) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Copro Error: SIMD : PSW.CU = %d>\n", cu);
            m_trace->SetExpCode (EXP_UCPOP1);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


/*!
 * @brief  特権命令例外判定
 * @param  inst_id
 * @return [true] 例外発生  [false] 正常終了
 */
bool FsCtrl::IsExcpPrivOccured (int32_t inst_id) const
{
    // @@ 関数ポインタ方式に全面差替え
    // @@ それまでは汚いけど放置
    PrivLevel inst_priv = MicroArchDB::GetPrivType (inst_id);
    if (inst_priv == PRIV_PERMIT_UM) {
        //! 非特権命令では特権命令例外は発生しない
        return false;

    } else {
        PrivLevel current_priv = m_trace->GetPrivLevel ();
        if (HasPrivileged (inst_priv, current_priv) == false) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Privilege Error: %s : current mode = %s>\n",
                           GetPrivName (inst_priv).c_str(), GetPrivName (current_priv).c_str());
            m_trace->SetExpCode (EXP_PIE);
            return true;
        } else {
            return false;
        }
    }
}


/*!
 * @brief  電源/クロック停止状態設定
 * @param  active power state (true=ON, false=OFF)
 */
void FsCtrl::SetHtPowerState (bool active)
{
    if (m_is_power_on != active) {
        m_is_power_on = active;
        UpdateHtActive ();
    }
}


/*!
 * @brief  HT実行権の判定結果の更新
 * スケジューラ、電源、スレッド有効状態から実行権限を持つか判定
 */
void FsCtrl::UpdateHtActive (void)
{
    m_is_ht_active = m_is_power_on;
    p_internals->UpdateAllHaltState ();
    UpdateSimState ();
    UpdateHtRunable ();
}


/*!
 * @brief  HT実行権の判定結果を返す
 * @return HT実行権の判定結果
 * @retval true 実行可
 * @retval false 実行不可
 */
bool FsCtrl::IsHtActive (void) const
{
    return m_is_ht_active;
}


/*!
 * @brief  スレッドが実行可能か(=RunModule()を呼び出す)の最終判定
 * @return スレッドの実行可能状態
 * @retval true 可
 * @retval false 不可
 * 実行可能状態にはSnooze/受付可能な例外割込が存在する状態を含む
 */
void FsCtrl::UpdateHtRunable (void)
{
    bool new_runable;

    // スレッドの特殊な起動条件
    // 1. 自スレッドがNMの場合、HALTまたはInactive(VM実行）状態であっても、
    //    NMに対する受付可能な割り込み要求があればVM->NM遷移してNM起動。
    // 2. 自スレッドがVMの場合、NMに対する受付可能な割り込み/例外があれば、
    //    NMが優先的に割り込み／例外を受け付けるように、VMの起動を抑止
    // 3. PowerON/OFF設定は全てに優先
    if ( (((IsInHalt () == true) || (IsHtActive () == false))
         && (GetSelectedIntFactorToNc() == INT_FAC_NONE))
         || ((m_has_intreq_to_nc) && (GetTcId () != NC))
         || (m_is_power_on == false)) {
        new_runable = false;
    } else {
        new_runable = true;
    }

    // 実行リストに追加/削除 (コスト大きいので注意)
    if (m_is_ht_runable != new_runable) {
        m_is_ht_runable = new_runable;
        if (new_runable == true) {
            p_internals->AddRunTarget (this);
        } else {
            p_internals->RemoveRunTarget (this);
        }
    }
}


/*!
 * @brief  スレッドが実行可能か最終判定結果の取得
 * @return スレッドの実行可能状態
 * @retval true 可
 * @retval false 不可
 */
bool FsCtrl::IsHtRunable (void) const
{
    return m_is_ht_runable;
}

void FsCtrl::SetHaltState (bool is_halt)
{
    m_is_in_halt = is_halt;
    UpdateHaltState ();
}


/*!
 * @brief  スレッドのHalt状態更新
 */
void FsCtrl::UpdateHaltState (void)
{
    m_is_in_dbg = m_srf->SrDIR0()->GetDM ();
    p_internals->UpdateAllHaltState (); // All Halt ブレーク判定用
    UpdateSimState ();
    UpdateHtRunable ();
}


/*!
 * @brief  スレッドのHalt状態を取得
 */
bool FsCtrl::IsInHalt (void) const
{
#ifdef ENABLE_DBG_BREAK
    // HALT is ignored on debug mode
    return m_is_in_halt && !m_is_in_dbg;
#else // ENABLE_DBG_BREAK
    return m_is_in_halt;
#endif // ENABLE_DBG_BREAK
}


/*!
 * @brief  スレッドの権限更新
 */
void FsCtrl::UpdatePrivLevel (void)
{
    //! SV権限の条件
    //! ・PSW.UM = 0
    //! HV権限の条件
    //! ・ネイティブ・モードかつSV権限である場合
    //!   （PSW.VM = 0 かつ PSW.UM = 0）
    //! ・仮想マシンに対して，HV権限が付与されている場合
    //!   （PSW.VM = 1 かつ PSW.UM = 0 かつ MCFG0.HVP = 1）
    //! ・HVCALL呼び出し中である場合
    //!   （PSW.VM = 1 かつ PSW.UM = 0 かつ PSW.HVC = 1）
    //! DB権限の条件
    //! ・DIR0.DM = 1

    bool dbmode;
    PrivLevel priv;
    SRegBasePSW* psw = m_srf->SrVPSW ();
    SRegPSWH* pswh = m_nc_srf->SrPSWH ();
    SRegHVCFG* hvcfg = m_nc_srf->SrHVCFG ();
    bool is_hv = (pswh->GetGM () == 0) && (hvcfg->GetHVE () == 1);
    if (m_srf->SrDIR0()->GetDM () == 1) {
        if (psw->GetUM () == 0) {
            if (is_hv) {
                priv = PRIV_HAS_DBHV;
            } else {
                priv = PRIV_HAS_DBSV;
            }
        } else {
            priv = PRIV_HAS_DBUM;
        }
        dbmode = true;
    } else if (psw->GetUM () == 0) {
        if (is_hv) {
            priv = PRIV_HAS_HV;
        } else {
            priv = PRIV_HAS_SV;
        }
        dbmode = false;
    } else {
        priv = PRIV_HAS_UM;
        dbmode = false;
    }

    m_trace->SetPrivLevel (priv);
    m_is_dbmode = dbmode;
}


void FsCtrl::UpdateCounters (void)
{
    // To prevent count for current instruction by the updated setting,
    // set PMC condition at last.
    if (m_reserved_pmc_cond) {
        // TSCOUNT
        m_tscount->SetRegisterByReservedInfo ();

        // PMCOUNT
        std::vector <PmCounter*>::iterator it = m_pmcount.begin ();
        while (it != m_pmcount.end ()) {
            (*it)->SetRegisterByReservedInfo ();
            ++ it;
        }
        m_reserved_pmc_cond = false;
    }

    bool is_run_ndbg = (m_srf->SrDIR0()->GetDM () == 0);
    bool is_run_ch28;
    if ((m_core_type != CORE_G4MH) || (m_core_version < CORE_VER_20)) {
        is_run_ch28 = is_run_ndbg && (m_srf->SrISPR()->GetISP() == 0);
    } else {
        if (m_srf->SrVINTCFG ()->GetEPL() == 0) {
            is_run_ch28 = is_run_ndbg && (m_srf->SrISPR()->GetISP() == 0);
        } else {
            is_run_ch28 = is_run_ndbg && (m_srf->SrVPSW()->GetEIMASK() == 0x3F);
        }
    }
    bool is_run_ch29 = is_run_ch28 && (m_srf->SrVPSW ()->GetID() == 1);

    // TSCOUNT
    if (is_run_ndbg) {
        m_tscount->SetEnable ();
    } else {
        m_tscount->SetDisable ();
    }

    // PMCOUNT
    std::vector <PmCounter*>::iterator it = m_pmcount.begin ();
    while (it != m_pmcount.end ()) {
        // Update for guest mode
        PmCounter::PmCondType cond = (*it)->GetConditionBody ();
        bool is_enable;
        if (m_pe->IsHvEnabled ()) {
            // Host mode : count if PMCTRL.HE=1
            // Gust mode : count if PMCTRL.GE[PSWH.GPID]=1
            // CND=24/25 : count always (check PMCTRL.HE/GEn=1 in fs_exception.cpp)
            // CND=20/21/22/23 : Count always. Exceptions may cause mode transition. Events are counted after transitioned mode.
            if (   ((m_pe->IsHostMode () == true) && ((*it)->GetHe () == 1))
                || (cond == PmCounter::PM_CNT_BGINT_SUSPENDED)
                || (cond == PmCounter::PM_CNT_BGINT_ACCEPTED)
                || ((m_pe->IsHostMode () == false) && ((*it)->IsGpidMatch (m_srf->SrPSWH ()->GetGPID ())))
                || (cond == PmCounter::PM_CNT_EIINT) || (cond == PmCounter::PM_CNT_FEINT)
                || (cond == PmCounter::PM_CNT_SUSPEND) || (cond == PmCounter::PM_CNT_REEXEC)
               ) {
                is_enable = true;
            } else {
                is_enable = false;
            }
        } else {
            is_enable = true;
        }

        if ((is_enable == true) &&
            (  ((cond == PmCounter::PM_CNT_NORMALCYC) && (is_run_ch28))
            || ((cond == PmCounter::PM_CNT_PSWID0CYC) && (is_run_ch29))
            || ((cond != PmCounter::PM_CNT_NORMALCYC) && (cond != PmCounter::PM_CNT_PSWID0CYC) && (is_run_ndbg)))) {
            (*it)->SetEnable ();
        } else {
            (*it)->SetDisable ();
        }
        ++ it;
    }

    m_is_run_ndbg = is_run_ndbg;
    m_is_run_ch28 = is_run_ch28;
    m_is_run_ch29 = is_run_ch29;

    m_update_pmc = false;
}


/*!
 * @brief  スレッド内部状態初期化
 */
void FsCtrl::FsInit (void)
{
    m_is_ht_active       = false; // Init by UpdateHtActive();
    // m_is_ht_runable   = false; // Don't init. Because updating is checked in UpdateHtRunable.
    // m_is_in_halt      = false; // Init by SetHaltState(false);
    // m_is_in_dbg       = false; // Init by SetHaltState(false); (actually by UpdateHaltState())
    m_is_prev_branched   = false;
    m_is_int_reqed       = false;
    m_is_feint_reqed     = false;
    m_is_fenmi_reqed     = false;
    m_is_syserr_reqed_pseudo_if = false;
    m_is_syserr_reqed_op = false;
    m_is_any_async_excp  = false;
    m_selected_factor    = INT_FAC_NONE;
    m_selected_factor_to_nc = INT_FAC_NONE;
    m_is_snooze          = false;
    m_has_intreq_to_nc   = false;
    m_snooze_wakeup_time = 0;
    m_snooze_start_time = 0;
    m_branch_type = BR_NOT;
    m_line_addr = 0x7;
    m_syserr_fetch_cause = SYSERR_NONE;
    m_is_run_ndbg = true;
    m_is_run_ch28 = true;
    m_is_run_ch29 = true;
    m_accepted_eiint_ch = 0xFFFFFFFF;
    m_accepted_feint_ch = 0xFFFFFFFF;
#ifdef ENABLE_PIC
    ClearInstructionCount ();
#endif // ENABLE_PIC
    ClearIntReqQueue ();
    ClearFenmiReqQueue ();
    ClearFeintReqQueue ();
    ClearPseudoFetchSyserrReqQueue ();
    ClearOperandSyserrReqQueue ();
    m_csctrl->CsInit ();
    m_trace->Clean ();
    m_trace->InitTime ();
    ExpInit ();
    UpdateDir0DmStatus ();
    UpdateIntRequested ();
    UpdateIntAcceptable ();
    UpdateGmIntAcceptable ();
    UpdateBgIntAcceptable ();
    UpdateFeintAcceptable ();
    UpdateGmFeintAcceptable ();
    UpdateBgFeintAcceptable ();
    UpdateOperandSyserrAcceptable ();
    SetHaltState (false);
    UpdateHtRunable ();
#ifdef ENABLE_DBG_BREAK
    m_dbgbrk->InitEoReserved ();
#endif // #ifdef ENABLE_DBG_BREAK

    ResetPerfInfo ();
}


void FsCtrl::HardReset (void)
{
    ChangeSimState (SIMULATE_RESET);

    m_csctrl->CsReset ();
    FsBaseReset();
}


void FsCtrl::FsReset (void)
{
    ChangeSimState (SIMULATE_RESET);
    FsBaseReset();
}


void FsCtrl::FsBaseReset (void)
{
    m_bus_time = 0;
    m_btb->Reset ();

#ifdef SUPPORT_SC_HEAP
    m_suspend_ctrl->Init ();
#endif // SUPPORT_SC_HEAP

    m_lsu->InitSyserrEvent ();

    FsInit ();

    m_break_points->Reset ();

    uint32_t pc_val = m_srf->SrRBASE()->GetRBASE() << 9;
    m_grf->WritePC (pc_val);
    m_trace->SetNextInstAddr (pc_val);
    m_csctrl->ForceUpdateIaAddress (pc_val);
    m_inst_queue->Init();

    m_tscount->Reset ();
    std::vector <PmCounter*>::iterator it = m_pmcount.begin ();
    while (it != m_pmcount.end ()) {
        (*it)->Reset ();
        ++ it;
    }

    m_lsu->InitMemRoute ();
    m_lsu->DeletePeLink ();
    m_lsu->GetMpuPtr()->Reset ();
    ResetSideBand ();

    UpdatePrivLevel ();
    UpdateHtActive ();

    SetFppMode (m_srf->SrFPSR()->GetPEM ());
#if defined(COMP_RUNNER)
    m_cancel_timign_mcyc = 0x7FF;
#endif //#if defined(COMP_RUNNER)
}


void FsCtrl::ResetSideBand (void)
{
    SideBand* sb_ptr = m_lsu->GetSideBandPtr ();

    // Initialize access parameters which aren't related to system register settings
    sb_ptr->SetBitPos (0);
    sb_ptr->SetBitOp (SideBand::BITOP_NONE);
    sb_ptr->SetSPCLD (Lsu::SPCLD_OFF);

    // Initialize fixed param
    sb_ptr->SetSecure (0);

    // Update sideband contents which related with system register settings
    m_pe->UpdateSideband ();
}


void FsCtrl::InitInstQueue (void)
{
    m_inst_queue->Init();

    // @@ 滅多にないので、CS側の命令バッファは消去しない
}


/*!
 * @brief  VMリスタート
 */
void FsCtrl::RestartVm (void)
{
    m_lsu->DeleteVmLink ();

    // PC/PSW 初期化
    uint32_t pc_val = m_srf->SrRBASE()->GetRBASE() << 9;
    m_grf->WritePC (pc_val);
    m_trace->SetNextInstAddr (pc_val);
    // CSのフェッチモデルのIAを更新する
    m_csctrl->ForceUpdateIaAddress (pc_val);

    // VM保持。それ以外はReset相当。
    m_srf->SrVPSW()->SetUM (0x0U);
    m_srf->SrVPSW()->SetCU (0x0U);
    m_srf->SrVPSW()->SetEBV (0x0U);
    m_srf->SrVPSW()->SetNP (0x0U);
    m_srf->SrVPSW()->SetEP (0x0U);
    m_srf->SrVPSW()->SetID (0x1U);
    m_srf->SrVPSW()->SetSAT (0x0U);
    m_srf->SrVPSW()->SetCY (0x0U);
    m_srf->SrVPSW()->SetOV (0x0U);
    m_srf->SrVPSW()->SetS (0x0U);
    m_srf->SrVPSW()->SetZ (0x0U);

    // 中断型例外が要求されたらHALT解除
    SetHaltState (false);

    m_inst_queue->Init();
}


void FsCtrl::SetScId (int32_t sc_id)
{
    m_sc_id = sc_id;
    m_lsu->SetScId (sc_id);
}


void FsCtrl::ShowPerfInfo (void) const
{
    // 実行したコンテキストだけ表示
    if (m_trace->GetStepCount () != 0) {
        std::ostringstream oss;
        oss << std::right << std::setfill('0');
        oss << "  P:" << GetPeId ();
        oss << "  PC:0x" << std::setw (8) << std::hex << m_grf->ReadPC ();
        oss << std::right << std::setfill(' ');
        oss << "  Ins: " << std::setw (8) << std::dec << m_trace->GetStepCount ();
        // 命令数なので回数×2
        oss << "  ParaIns: " << std::setw (8) << std::dec << m_para_issue_count * 2;
        oss << "  Cyc: " << std::setw (8) << std::dec
            << m_trace->GetStageCycle (STAGE_EA) - m_cycle_count_offset;
        oss << std::endl;

        std::string ostr (oss.str());
        CfMsg::DPrint (MSG_ERR, "%s", ostr.c_str());
        if (m_simmode != FS_MODE) {
            m_csctrl->ShowPerfInfo ();
        }
        m_btb->ShowPerfInfo ();
    }
}


void FsCtrl::ResetPerfInfo (void)
{
    // Cycle数は厄介なので0初期化しない。クリア時のオフセットを記録して表示時に引き算する。
    m_trace->SetStepCount (0);
    m_cycle_count_offset = m_trace->GetStageCycle (STAGE_EA);
    m_para_issue_count = 0;
    m_csctrl->ResetPerfInfo ();
    m_btb->ResetPerfInfo ();

    // PMCounter events
    m_lsu->ResetPerf ();
    m_branch_count = 0;
    m_branch_cond_exec_count = 0;
    m_branch_cond_miss_count = 0;
    m_eiint_count.assign (8, 0);
    m_feint_count.assign (8, 0);
    m_exp_abort_count.assign(8, 0);
    m_exp_not_abort_count.assign(8, 0);
    m_bgint_abort_count.assign(8, 0);
    m_bgint_accepted_count.assign(8, 0);
    m_inst_count_pmc = 0;
    m_update_pmc = false;
    m_reserved_pmc_cond = false;
}


#if defined(COMP_RUNNER_G3KH)
void FsCtrl::SetINTState (bool int_nc) {
    if (m_tc_id != NC && int_nc == true) {
        m_pe->GetHtPtr(NC)->m_selected_factor = INT_FAC_INT;
        m_pe->GetHtPtr(NC)->m_selected_factor_to_nc = m_pe->GetHtPtr(NC)->m_selected_factor;
        m_pe->GetHtPtr(NC)->UpdateHtRunable();
    } else {
        m_selected_factor = INT_FAC_INT;
    }
}


void FsCtrl::SetINTState (bool int_nc, uint32_t pri, uint32_t ch) {
    if (m_tc_id != NC && int_nc == true) {
        m_pe->GetHtPtr(NC)->m_selected_factor = INT_FAC_INT;
        m_pe->GetHtPtr(NC)->m_selected_factor_to_nc = m_pe->GetHtPtr(NC)->m_selected_factor;
        m_pe->GetHtPtr(NC)->UpdateHtRunable();
    } else {
        m_selected_factor = INT_FAC_INT;
    }

    IntCause* cause = new IntCause (pri, ExpInfo::GetExpCause (EXP_EIINT) + ch, false, NULL);
    m_int_queue.push_back (cause);
}


void FsCtrl::SetFEINTState (bool int_nc) {
    if (m_tc_id != NC && int_nc == true) {
        m_pe->GetHtPtr(NC)->m_selected_factor = INT_FAC_FEINT;
        m_pe->GetHtPtr(NC)->m_selected_factor_to_nc = m_pe->GetHtPtr(NC)->m_selected_factor;
        m_pe->GetHtPtr(NC)->UpdateHtRunable();
    } else {
        m_selected_factor = INT_FAC_FEINT;
    }
}


void FsCtrl::SetSYSERRState (bool int_nc) {
    if (IsInHalt() == true && int_nc == true) {
        m_selected_factor_to_nc = m_selected_factor = INT_FAC_SYSERR;
        SetHaltState (false);
    } else if (m_tc_id != NC && int_nc == true) {
        m_pe->GetHtPtr(NC)->m_selected_factor = INT_FAC_SYSERR;
        m_pe->GetHtPtr(NC)->m_selected_factor_to_nc = m_pe->GetHtPtr(NC)->m_selected_factor;
        m_pe->GetHtPtr(NC)->UpdateHtRunable();
    } else {
        m_selected_factor = INT_FAC_SYSERR;
    }

    m_syserr_cause = (SyserrCause*) new SyserrCause(0x10);
}


void FsCtrl::SetSYSERRIFState (bool int_nc) {
    if (m_tc_id != NC && int_nc == true) {
        m_pe->GetHtPtr(NC)->m_selected_factor = INT_FAC_SYSERR;
        m_pe->GetHtPtr(NC)->m_selected_factor_to_nc = m_pe->GetHtPtr(NC)->m_selected_factor;
        m_pe->GetHtPtr(NC)->UpdateHtRunable();
    } else {
        m_selected_factor = INT_FAC_SYSERR;
    }

    m_syserr_cause = (SyserrCause*) new SyserrCause(0x18);
}
#endif // defined(COMP_RUNNER_G3KH)


FsCtrl::FsCtrl (uint32_t pe_id, ContextId vc_id, ContextId tc_id, ArchClass arch_class,
                FsBus* fsbus, Icache* ic, CIntc1* intc1,
#ifdef ENABLE_DBG_BREAK
                DbgBreak* dbgbrk,
#endif // #ifdef ENABLE_DBG_BREAK
                Llsc* llsc,
                Misr *misr,
                ProcElement* pe, GRegFile* grf, SRegFile* srf,
                NcSRegFile* nc_srf,
                GRegFileSet* grfs, CsLsuBuf* cs_lsu_buf, CoreType core_type, CoreVersion core_version)
    : m_pe_id (pe_id), m_vc_id (vc_id), m_tc_id (tc_id), m_arch_class (arch_class),
      m_core_type (core_type), m_core_version (core_version),
      m_ic (ic), m_intc1 (intc1),
#ifdef ENABLE_DBG_BREAK
      m_dbgbrk (dbgbrk),
#endif // #ifdef ENABLE_DBG_BREAK
      m_misr (misr), m_pe (pe), m_grf (grf), m_srf (srf), m_nc_srf (nc_srf), m_simmode (CS_MODE),
      m_archdb_id ((core_type == CORE_G4MH) ? ARCH_DB_G3MH: ARCH_DB_G3KH)
{
    m_dbgmode = DBG_OFF;
    m_is_in_halt = false;
    m_is_in_dbg = false;
    m_is_int_acceptable = false;
    m_is_gmint_acceptable = false;
    m_is_bgint_acceptable = false;
    m_is_feint_acceptable = false;
    m_is_host_op_syserr_acceptable = false;
    m_is_guest_op_syserr_acceptable = false;
    m_is_gmfeint_acceptable = false;
    m_is_bgfeint_acceptable = false;
    m_is_dir0dm_0 = false;
    m_is_ht_active = false;
    m_is_power_on = true;
    m_is_ht_runable = false;
    m_cpu_time = 0;
    m_bus_time = 0;
    m_clk_ratio = 1;
    m_fenmi_cause = NULL;
    m_feint_cause = NULL;
    m_syserr_cause_pseudo_if = NULL;
    m_syserr_cause_op = NULL;
    m_is_fpp_mode = false;
    m_log_file = p_internals->GetFileOut ();
    m_last_completed_pc = 0; // Don't initialize by Reset

    m_snooze_period = pe->GetSnoozePeriod ();
    m_support_table_ref_int = INIT_TABLE_REF_INT;

    m_trace_org = new TraceCtrl (pe_id);
    m_trace = m_trace_org;

    m_is_run_ndbg = true;
    m_is_run_ch28 = true;
    m_is_run_ch29 = true;
    m_tscount = new CycleCounter (pe_id, tc_id);
    m_pmcount.push_back (new PmCounter (pe_id, tc_id, 0));
    m_pmcount.push_back (new PmCounter (pe_id, tc_id, 1));
    m_pmcount.push_back (new PmCounter (pe_id, tc_id, 2));
    m_pmcount.push_back (new PmCounter (pe_id, tc_id, 3));
    if ((m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
        // enable 8 channels for G4MH2.0 and later
        m_pmcount.push_back (new PmCounter (pe_id, tc_id, 4));
        m_pmcount.push_back (new PmCounter (pe_id, tc_id, 5));
        m_pmcount.push_back (new PmCounter (pe_id, tc_id, 6));
        m_pmcount.push_back (new PmCounter (pe_id, tc_id, 7));
    }

    // Check SYSERR cause for Guest privilege error for lsu
    SysErrCause syserr_fetch_guest_error;
    bool enable_operand_syserr;
    if ((m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
        syserr_fetch_guest_error = SYSERR_I_GUEST;
        enable_operand_syserr = true;
    } else {
        // In G4MH1.x, SYSERR on fetch is always 0x11.
        syserr_fetch_guest_error = SYSERR_ICFB;
        enable_operand_syserr = false;
    }

    m_lsu = new Lsu (m_trace, fsbus, ic, srf,
#ifdef ENABLE_DBG_BREAK
            dbgbrk,
#endif // #ifdef ENABLE_DBG_BREAK
            llsc,
            misr, tc_id, MAA_TYPE2, pe, syserr_fetch_guest_error, enable_operand_syserr);
    if (core_type == CORE_G4MH) {
        m_btb = new BtbG3MH (m_pe_id, m_tc_id);
    } else {
        m_btb = new BtbG3M (m_pe_id, m_tc_id);
    }

    m_csctrl = new CsCtrl (pe_id, this, cs_lsu_buf, core_type);

    m_insts = new InstV850 (m_trace, m_grf, m_srf, m_lsu, m_btb,
                            nc_srf, grfs, m_ic, this, m_arch_class);
    m_inst_queue   = new InstQueue (m_trace, m_lsu, core_type);
    m_break_points = new BreakPoint (m_trace);

    m_exp_msg = new ExpMsg ();
    m_trace_print = new TracePrint (m_srf, core_type);

#ifdef SUPPORT_SC_HEAP
    m_suspend_ctrl = new SuspendCtrl (this, m_csctrl, m_insts, m_grf, pe_id);
    m_csctrl->SetSuspendCtrl (m_suspend_ctrl);
#endif // SUPPORT_SC_HEAP

#ifdef ENABLE_PIC
    m_statistics = new StatisticsCtrl (TOTAL_INST_NUM+1);
#endif // ENABLE_PIC

    // インスタンス時に特殊初期化が必要なレジスタ
    m_srf->SrRBASE()->WriteBody (0x0);

    FsInit ();

    // Traceのtcid内容を反映するために必要
    UpdateHtActive ();
}


FsCtrl::~FsCtrl (void)
{
#ifdef ENABLE_PIC
    delete m_statistics;
#endif // ENABLE_PIC

    ClearIntReqQueue ();
    ClearFenmiReqQueue ();
    ClearFeintReqQueue ();
    ClearOperandSyserrReqQueue ();
    ClearPseudoFetchSyserrReqQueue ();

    delete m_break_points;
    delete m_inst_queue;
    delete m_insts;
    delete m_btb;
    delete m_lsu;
    delete m_trace_org;
    delete m_trace_print;
    delete m_csctrl;
    delete m_exp_msg;

    delete m_tscount;
    std::vector <PmCounter*>::iterator it = m_pmcount.begin ();
    while (it != m_pmcount.end ()) {
        delete *it;
        ++ it;
    }
    m_pmcount.clear ();

#ifdef SUPPORT_SC_HEAP
    delete m_suspend_ctrl;
#endif // SUPPORT_SC_HEAP
}


uint64_t FsCtrl::GetBgintAbortCount (uint32_t ch) const
{
    if ((m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
        return m_bgint_abort_count[ch];
    } else {
        return 0;
    }
}


uint64_t FsCtrl::GetBgintAccesptedCount (uint32_t ch) const
{
    if ((m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
        return m_bgint_accepted_count[ch];
    } else {
        return 0;
    }
}
