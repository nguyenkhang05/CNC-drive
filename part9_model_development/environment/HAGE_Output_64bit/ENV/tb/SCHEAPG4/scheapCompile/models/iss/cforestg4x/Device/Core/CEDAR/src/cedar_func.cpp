/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <iterator>
#include <string>
#include "./config.h"
#include "./fs_controler.h"
#include "./trace_print.h"
#include "./regid.h"
#include "./gregfile.h"
#include "./sregfile.h"
#include "./breakpoint.h"
#include "./statistics.h"
#include "./mpu.h"
#include "./lsu.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#ifdef SUPPORT_CUBESUITE
#include "./cube_callback.h"
#endif // #ifdef SUPPORT_CUBESUITE
#include "./cs_controler.h"
#include "./inst_queue.h"
#include "./inst_v850.h"

#ifdef SUPPORT_SC_HEAP
#include "./suspend_ctrl.h"
#endif // SUPPORT_SC_HEAP

#include "./btb_base.h"


void FsCtrl::EnableDebugMode (DebugMode mode)
{
    if (mode == DBG_OFF) {
        m_dbgmode &= ~ DBG_ON;
    } else {
        m_dbgmode = m_dbgmode | mode;
        m_trace_print->EnableTracePrint ();
    }
}


void FsCtrl::DisableDebugOption (std::string option)
{
    if (option == "rw") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_RW);
    } else if (option == "pipe") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_PIPE);
    } else if (option == "dep") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_DEP);
    } else if (option == "bra") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_BRA);
    } else if (option == "systime") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_TIME);
    } else if (option == "cache") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_CACHE);
    } else if (option == "stall") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_STALL);
    } else if (option == "timer") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_TIMER);
    } else if (option == "perfcnt") {
            m_dbgmode = m_dbgmode & (DBG_MASK ^ DBG_PERF);
    } else {
            CfMsg::DPrint (MSG_ERR, "<Error: Invalid debug option (%s)>\n", option.c_str ());
    }
}


DebugMode FsCtrl::GetDebugMode (void)
{
    return m_dbgmode;
}


void FsCtrl::InitTracePrintRTL (FsCtrl* nptr, FILE* fp)
{
    TracePrintRTL* trc_ptr = nptr->m_trace_print->GetPtrRTL ();
    m_trace_print->InitPrintRTL (fp, trc_ptr);
    m_trace_print->EnableTracePrint ();
}


CReg FsCtrl::RegGetRegno (std::string regname)
{
    // 文字列を大文字に変換
    std::string regname_big;
    transform (regname.begin (), regname.end (),
               back_inserter(regname_big), toupper);

    CReg regno = RegID::GetId (regname_big);
    if (regno != GR_NONE) {
        if (RegID::IsSReg(regno) == true) {
            std::string cmp_str = "PMC";
            if (   (regname_big.size () >= cmp_str.size())
                && std::equal(std::begin(cmp_str), std::end(cmp_str), std::begin (regname_big))) {
                // regname is started from "PMC"xxx.
                // It will be PMCTRL/PMCOUNT
                CfSReg cfsrid = RegID::GetCfSRegId (regno);
                CfSReg translated_cfsrid = GetCorrectCfSRegID (m_pe_id, cfsrid);
                if ((translated_cfsrid != CF_SR_DUMMY) && (cfsrid != translated_cfsrid)) {
                    regno = RegID::GetSRegId (translated_cfsrid);
                }
            }
        }
        return regno;
    }
    CfMsg::DPrint (MSG_ERR, "<Error: Invalid Register : %s>\n", regname.c_str ());

    return (GR_NONE);
}


CfSReg FsCtrl::GetCorrectCfSRegID (uint32_t peid, CfSReg regid) const
{
    // Convert correct register id.
    // There is same register name with different regid/selid in different core-type/core-version,
    // For example, regid/selid of PMCOUNTn and PMCTRLn are different betwqeen G4MH1.x and G4MH2.x.
    // 
    // Although some functions search register name, it may select wrong regid/selid for different core.
    // Therefore, this fucntion converts it to correct register id.

    // No need to check validation of PEID. It is checked before calling this function.
    // @@ TODO : When add new core_type or core_version, this condition must be updated.
    CfSReg translated_regid;
    if (   (m_core_type == CORE_G4MH)
        && (m_core_version >= CORE_VER_20) ) {
        // G4MH2.x
        switch (regid) {
        case CF_SR_PMCTRL0_G4X1X: translated_regid = CF_SR_PMCTRL0_G4MH20; break;
        case CF_SR_PMCTRL1_G4X1X: translated_regid = CF_SR_PMCTRL1_G4MH20; break;
        case CF_SR_PMCTRL2_G4X1X: translated_regid = CF_SR_PMCTRL2_G4MH20; break;
        case CF_SR_PMCTRL3_G4X1X: translated_regid = CF_SR_PMCTRL3_G4MH20; break;
        case CF_SR_PMCOUNT0_G4X1X: translated_regid = CF_SR_PMCOUNT0_G4MH20; break;
        case CF_SR_PMCOUNT1_G4X1X: translated_regid = CF_SR_PMCOUNT1_G4MH20; break;
        case CF_SR_PMCOUNT2_G4X1X: translated_regid = CF_SR_PMCOUNT2_G4MH20; break;
        case CF_SR_PMCOUNT3_G4X1X: translated_regid = CF_SR_PMCOUNT3_G4MH20; break;
        default: translated_regid = regid; break;
        }
    } else if (   (m_core_type == CORE_G4MH)
               && (m_core_version < CORE_VER_20) ) {
        // G4MH1.x
        switch (regid) {
        case CF_SR_PMCTRL0_G4MH20: translated_regid = CF_SR_PMCTRL0_G4X1X; break;
        case CF_SR_PMCTRL1_G4MH20: translated_regid = CF_SR_PMCTRL1_G4X1X; break;
        case CF_SR_PMCTRL2_G4MH20: translated_regid = CF_SR_PMCTRL2_G4X1X; break;
        case CF_SR_PMCTRL3_G4MH20: translated_regid = CF_SR_PMCTRL3_G4X1X; break;
        case CF_SR_PMCTRL4_G4MH20: translated_regid = CF_SR_DUMMY; break;
        case CF_SR_PMCTRL5_G4MH20: translated_regid = CF_SR_DUMMY; break;
        case CF_SR_PMCTRL6_G4MH20: translated_regid = CF_SR_DUMMY; break;
        case CF_SR_PMCTRL7_G4MH20: translated_regid = CF_SR_DUMMY; break;
        case CF_SR_PMCOUNT0_G4MH20: translated_regid = CF_SR_PMCOUNT0_G4X1X; break;
        case CF_SR_PMCOUNT1_G4MH20: translated_regid = CF_SR_PMCOUNT1_G4X1X; break;
        case CF_SR_PMCOUNT2_G4MH20: translated_regid = CF_SR_PMCOUNT2_G4X1X; break;
        case CF_SR_PMCOUNT3_G4MH20: translated_regid = CF_SR_PMCOUNT3_G4X1X; break;
        case CF_SR_PMCOUNT4_G4MH20: translated_regid = CF_SR_DUMMY; break;
        case CF_SR_PMCOUNT5_G4MH20: translated_regid = CF_SR_DUMMY; break;
        case CF_SR_PMCOUNT6_G4MH20: translated_regid = CF_SR_DUMMY; break;
        case CF_SR_PMCOUNT7_G4MH20: translated_regid = CF_SR_DUMMY; break;
        default: translated_regid = regid; break;
        }
    } else {
        CfMsg::DPrint (MSG_ERR, "<Internal Error: Unsupported core is detected at GetCorrectCfSRegID\n>");
        translated_regid = regid;
    }
    return translated_regid;
}


void FsCtrl::MemWriteDebug (PhysAddr vrtl_addr, MemData data, ByteSize size) const
{
    m_lsu->MemWriteDebug (vrtl_addr, data, size);
}


MemData FsCtrl::MemReadDebug (PhysAddr vrtl_addr, ByteSize size) const
{
    return m_lsu->MemReadDebug (vrtl_addr, size);
}


bool FsCtrl::IsBreakHitPreExec (void) const
{
    return m_break_points->IsHitPreExec ();
}


bool FsCtrl::IsBreakHitPostExec (void) const
{
    return m_break_points->IsHitPostExec ();
}


void FsCtrl::AddBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask)
{
    m_break_points->AddBreakAccess (start, end, attr, mask);
}


void FsCtrl::DisplayBreakAccess (void)
{
    m_break_points->DisplayBreakAccess ();
}


void FsCtrl::ClearBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask)
{
    m_break_points->ClearBreakAccess (start, end, attr, mask);
}


void FsCtrl::AddBreakNextPc (uint32_t address, uint32_t mask)
{
    m_break_points->AddBreakNextPc (address, mask);
}


void FsCtrl::DisplayBreakPc (void)
{
    m_break_points->DisplayBreakPc ();
}


void FsCtrl::ClearBreakNextPc (uint32_t address, uint32_t mask)
{
    m_break_points->ClearBreakNextPc (address, mask);
}


void FsCtrl::SetBreakMax (uint64_t count)
{
    m_break_points->SetBreakMax (count);
}


uint64_t FsCtrl::GetBreakMax (void)
{
    return m_break_points->GetBreakMax ();
}


void FsCtrl::SetBreakSamePcMax (uint64_t count)
{
    m_break_points->SetBreakSamePcMax (count);
}


uint64_t FsCtrl::GetBreakSamePcMax (void) const
{
    return m_break_points->GetBreakSamePcMax ();
}


const TraceInfo* FsCtrl::GetTraceInfo (void) const
{
    return m_trace->GetTraceInfo ();
}


#ifdef ENABLE_PIC
void FsCtrl::DisplayInstructionCount (FILE* local_file)
{
    m_statistics->DisplayInstructionCount (m_pe_id, m_tc_id, local_file);
}


void FsCtrl::ClearInstructionCount (void)
{
    m_statistics->ClearInstructionCount ();
}
#endif // ENABLE_PIC


void FsCtrl::ClockWait (uint64_t wait_cycle)
{
    uint64_t wait_time = m_clk_ratio * wait_cycle;
    m_cpu_time += wait_time;
}


void FsCtrl::AddHaltCycle (uint64_t halt_cycle)
{
    m_csctrl->AddHaltCycle (halt_cycle);

    // スレッド停止期間を更新したら、SNOOZE時間も更新する
    // - スレッド遷移やシングルスレッドモードなどで自スレッドが停止していた期間を
    //   SNOOZE期間から除外する
    uint64_t halt_time = halt_cycle * m_clk_ratio;
    m_snooze_wakeup_time += halt_time;
    m_snooze_start_time += halt_time;
}


void FsCtrl::SetHaltCycle (uint64_t halt_cycle)
{
    m_csctrl->SetHaltCycle (halt_cycle);
}


void FsCtrl::SetCpuTime (uint64_t num)
{
    m_cpu_time = num;
}


void FsCtrl::PreConsumeClock (void)
{
    ClockWait (1);
    m_csctrl->PreConsumeClock (m_trace);
}


void FsCtrl::SetCpuRatio (uint32_t num)
{
    m_clk_ratio = num;
    m_csctrl->SetClkRatio (num);
}


void FsCtrl::SetFilePointer (FILE* fp)
{
    m_log_file = fp;
}


uint64_t FsCtrl::GetCpuTime() const
{
    return m_cpu_time;
}


uint64_t FsCtrl::GetBusTime() const
{
    return m_bus_time;
}


void FsCtrl::SetBusTime (uint64_t systime)
{
    m_bus_time = systime;
}


uint64_t FsCtrl::GetCycleCount (void) const
{
    return m_csctrl->GetCycleCount ();
}


uint32_t FsCtrl::GetCpuRatio () const
{
    return m_clk_ratio;
}


FILE* FsCtrl::GetFilePointer () const
{
    return m_log_file;
}


FILE* FsCtrl::GetTraceFilePtrRTL () const
{
    return m_trace_print->GetTraceFilePtrRTL ();
}


int32_t FsCtrl::GetScId (void) const
{
    return m_sc_id;
}


ContextId FsCtrl::GetTcId (void) const
{
    return NC;
}


ContextId FsCtrl::GetVcId (void) const
{
    return NC;
}


uint32_t FsCtrl::GetPeId (void) const
{
    return m_trace->GetPeId ();
}


bool FsCtrl::IsSnooze (void) const
{
    return m_is_snooze;
}


void FsCtrl::DumpBtbStatistics (void) const
{
    m_btb->ShowPerfInfo ();
}


void FsCtrl::ResetBtbCounter (void)
{
    m_btb->ResetPerfInfo ();
}


void FsCtrl::SetSnooze (bool is_snooze)
{
    if (is_snooze != m_is_snooze) {
        if (is_snooze == true) {
            m_snooze_wakeup_time = m_cpu_time + GetSnoozePeriod () * m_clk_ratio;
            m_snooze_start_time = m_cpu_time;
        } else {
            // Snooze中の時間分だけCS時間を進める
            if (m_simmode != FS_MODE) {
                // snooze期間はsystime基準なのでclkratioでPEの時間に換算する
                // ただし、経過時間は必ずclkratioとスケジュール周期の公倍数なので、余りの加算は不要
                uint64_t snooze_period = (m_cpu_time - m_snooze_start_time) / m_clk_ratio;
                m_csctrl->ElapsedCsTime (snooze_period);
            }
        }
    }
    m_is_snooze = is_snooze;
    UpdateSimState ();
    UpdateHtRunable ();
}


void FsCtrl::AdjustSnoozeTime (void)
{
    // FSでのSNOOZE実行時にSetSnooze関数で計算した時間には、
    // SNOOZE命令自身のストールが未反映。
    // CS実行時はSNOOZEのIDステージ基準でSNOOZE期間を再計算する。

    // RTLsim:32cycleを設定すると34cycle止まって、35cycle目に次命令を実行
    // CForestではGetSnoozePeriod（）止まり、次のサイクルに次命令を実行
    // -> STAGE_ID - 1を入れて、RTLsimと同じサイクルに調整
    // SnoozePeriod = 0 ならSnooze状態への遷移をスキップするのでケア不要
    m_snooze_wakeup_time = m_cpu_time + (GetSnoozePeriod () - 1) * m_clk_ratio;
    m_snooze_start_time = m_cpu_time;
}


void FsCtrl::UpdateSnoozeState (void)
{
    if (m_is_snooze == false) {
        return;
    }
    if (m_snooze_wakeup_time <= m_cpu_time) {
        SetSnooze (false);
    }
}


/*!
 * @brief  CubeSuite向け:スレッド状態の更新
 */
void FsCtrl::UpdateSimState (void)
{
    NotifySimState new_state;

    if (m_is_power_on == false) {
        // CubeSuiteからCLKOFF通知
        new_state = SIMULATE_STOP;
    } else if (IsHtActive () == false) {
        // 実行対象外コンテキスト
        new_state = SIMULATE_DISABLE;
    } else if (IsInHalt () == true){
        new_state = SIMULATE_HALT;
    } else if (m_is_snooze == true) {
        new_state = SIMULATE_SNOOZE;
    } else {
        new_state = SIMULATE_ON;
    }

    ChangeSimState (new_state);
}


/*!
 * @brief  CubeSuite向け:スレッド状態の通知
 */
void FsCtrl::ChangeSimState (NotifySimState state)
{
    if (m_trace->GetSimState () != state) {
        m_trace->SetSimState (state);
#ifdef SUPPORT_CUBESUITE
        (*CubeCallBack::__NotifyChangeStatus)
            (GetPeId (), m_grf->ReadPC (), state);
#endif // SUPPORT_CUBESUITE
    }
}


void FsCtrl::UpdateNextInstAddr (void)
{
    // リセット、PC更新などによりNextInstAddrを信用できないのでPCを再読込
    m_trace->SetNextInstAddr (m_grf->ReadPC ());

    // CSのフェッチモデルのIAを更新する
    m_csctrl->ForceUpdateIaAddress (m_grf->ReadPC ());
}


#ifdef SUPPORT_CUBESUITE
/*!
 * @brief  CubeSuite向け:トレースのnext_xxxの更新
 */
void FsCtrl::UpdateNextTcInfo (FsCtrl* next_tc)
{
    // StepExecute実行後、またはGetTraceInfoが呼ばれた場合に使用
    uint32_t  next_pe_id = next_tc->GetPeId ();
    m_trace->SetNextPeId (next_pe_id);

    // ここでnext_xxxは、次にStepExecuteの際に実行されるTCの情報。
    // 自TCのnextではないので注意。
    next_tc->UpdateNextInst ();
    const TraceInfo* trace_next = next_tc->GetTraceInfo ();
    m_trace->SetNextInstAddr (trace_next->next_inst_addr);
}


/*!
 * @brief  CubeSuite向け:自スレッドの次に実行される命令をデコードまで実施
 */
void FsCtrl::UpdateNextInst (void)
{
    // リセット、PC更新などによりNextInstAddrを信用できないのでPCを再読込
    PhysAddr next_inst_addr = m_grf->ReadPC ();
    m_trace->SetNextInstAddr (next_inst_addr);
}
#endif // SUPPORT_CUBESUITE


#ifdef SUPPORT_SC_HEAP
bool FsCtrl::IsSuspendState (void) const
{
    return ! (m_suspend_ctrl->IsRunnable ());
}


SuspendState FsCtrl::GetSuspendState (void) const
{
    return m_trace->GetSuspendState ();
}


void FsCtrl::NotifyFetchLatency (PhysAddr addr, uint32_t latency)
{
    m_suspend_ctrl->NotifyFetchLatency (m_trace, addr, latency);
}


void FsCtrl::NotifyReadLatency  (PhysAddr addr, uint32_t latency, uint64_t data)
{
    m_suspend_ctrl->NotifyReadLatency (m_trace, addr, latency, data);
}


void FsCtrl::NotifyWriteLatency (PhysAddr addr, uint32_t latency)
{
    m_suspend_ctrl->NotifyWriteLatency (m_trace, addr, latency);
}


void FsCtrl::SetTraceCtrl (TraceCtrl* next_trace)
{
    // トレース付け替え時に、保持すべきメンバをコピーする
    // - TraceCtrl::Clean ()で初期化されるメンバ、毎回必ずセットされるメンバはコピー不要。
    // pe_id, vc_id, tc_id, next_inst_addr, is_parallel, priv_level,
    // step_count, stage_cycle[STAGE_NUM], sim_state
    next_trace->SetPeId (m_trace->GetPeId ());
    next_trace->SetNextInstAddr (m_trace->GetNextInstAddr ());
    next_trace->SetParallel (m_trace->IsParallel ());
    next_trace->SetPrivLevel (m_trace->GetPrivLevel ());
    next_trace->SetStepCount (m_trace->GetStepCount ());
    for (uint32_t i = 0; i < STAGE_NUM; ++i) {
        PipeStage stage = static_cast<PipeStage> (i);
        next_trace->SetStageCycle (stage, m_trace->GetStageCycle (stage));
    }
    next_trace->SetSimState (m_trace->GetSimState ());
    m_trace = next_trace;

    // 他クラス内のトレースを付け替える
    m_lsu->SetTraceCtrl (next_trace);
    m_insts->SetTraceCtrl (next_trace);
    m_inst_queue->SetTraceCtrl (next_trace);
    m_break_points->SetTraceCtrl (next_trace);
};


TraceCtrl* FsCtrl::GetNewTrace (void)
{
    return p_internals->ReserveTrace ();
}


void FsCtrl::AddBreakMatchPc (uint32_t address)
{
    m_break_points->AddMatchPc (address);
}


void FsCtrl::ClearBreakMatchPc (uint32_t address)
{
    m_break_points->ClearMatchPc (address);
}
#endif // SUPPORT_SC_HEAP

void FsCtrl::SetFpuLatencyOffset (int32_t offset)
{
    m_csctrl->SetFpuLatencyOffset (offset);
}

void FsCtrl::SetDivfLatencyOffset (int32_t offset)
{
    m_csctrl->SetDivfLatencyOffset (offset);
}

#ifdef FROG_API_IF
void FsCtrl::DeleteLLBit (PhysAddr paddr)
{
    m_lsu->DeleteOwnLink();
}

void FsCtrl::CreateLLBit (int32_t scid, PhysAddr paddr, ByteSize size)
{
    m_lsu->CreateLink(scid, paddr, size);
}
#endif // FROG_API_IF

void FsCtrl::SetGramArea (PhysAddr start_addr, PhysAddr end_addr)
{
    m_lsu->SetGramArea (start_addr, end_addr);
}

void FsCtrl::SetLramArea (PhysAddr start_addr, PhysAddr end_addr)
{
    m_lsu->SetLramArea (start_addr, end_addr);
}

void FsCtrl::ExecCfali (PhysAddr addr)
{
    m_lsu->ExecCfali (addr);
}
