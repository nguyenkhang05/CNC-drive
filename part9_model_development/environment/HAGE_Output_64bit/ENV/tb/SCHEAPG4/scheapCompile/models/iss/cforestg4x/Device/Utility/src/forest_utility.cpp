/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#define SIMNAME "G4x"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "./config.h"
#include "./forest_common.h"
#include "./forest.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./fsbus.h"
#include "./smem.h"
#include "./fs_controler.h"
#include "./proc_element.h"
#include "./regid.h"
#include "./gregfile.h"
#ifdef SUPPORT_CUBESUITE
#include "./cube_callback.h"
#include "./cube_ior.h"
#endif // #ifdef SUPPORT_CUBESUITE
#include "./memory_body.h"
#include "./intc1.h"
#include "./icache.h"
#include "./exp_info.h"
#include "./exp_message.h"
#include "./peg.h"
#ifdef ENABLE_OSTM
#include "./os_timer.h"
#endif // #ifdef ENABLE_OSTM
#include "./dbg_area.h"
#include "./error_area.h"
#include "./outstand_mem.h"
#ifdef ENABLE_CLUSTER
#include "./cl_bridge.h"
#endif // #ifdef ENABLE_CLUSTER
#include "./crg.h"
#ifdef ENABLE_UART
#include "./uart.h"
#endif // #ifdef ENABLE_UART
#ifdef USE_SIM_IO
#include "./simio.h"
#endif // USE_SIM_IO

#ifdef SUPPORT_SC_HEAP
#include "./invalid_lram.h"
#endif // #ifdef SUPPORT_SC_HEAP

bool ForestUtil::m_stop_req = false;

uint64_t ForestUtil::GetSystemTime (void) const
{
    return m_internals->GetSystemTime ();
}


uint64_t ForestUtil::GetCpuTime (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_internals->GetCpuTime (peid);
}


#if defined(COMP_RUNNER)
void ForestUtil::SetCanselTimingMcycInst (int32_t scid, int32_t completed_count)
{
    if (IsValidScId (scid) == false) {
        return;
    }

    m_cedar[scid]->SetCanselTimingMcycInst (completed_count);
}
#endif // #if defined(COMP_RUNNER)


//! CForest用Step実行ルーチン
template <RunMode run_mode>
int32_t ForestUtil::StepExecute (int32_t scid, uint64_t step_goal)
{
    if (IsValidScId (scid) == false) {
        return -1;
    }

    m_stop_req = false;

    int32_t break_scid = -1;
    uint64_t step_cnt = 0;

#if defined(COMP_RUNNER)
    int ht_no = m_cedar_run.size ();
#endif // defined(COMP_RUNNER)

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    uint64_t finish_time = m_internals->GetSystemTime () + m_internals->GetTimeout ();
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

    while (1) {
        ModuleRun* ht = m_cedar_run.front ();
#if defined(COMP_RUNNER)
        if (!(ht->IsSnooze ()) && ht->IsCpu() && (ht->GetScId () != scid)) {
            int current_ht = 0;
            RunList::iterator it_source = m_cedar_run.begin ();
            RunList::iterator it_dest = m_cedar_run.end ();
            while (!((*it_source)->IsSnooze ()) && (*it_source)->IsCpu() && ((*it_source)->GetScId () != scid)) {
                rotate (it_source, it_source + 1, it_dest);
                it_source = m_cedar_run.begin ();
                current_ht++;
                if (current_ht == ht_no) {
                    // std::cout << std::endl << "### Error: SCID not found!!! ###" << std::endl;
                    break;
                }
            }
            ht = (*it_source);
        }
#endif // defined(COMP_RUNNER)
        if (ht->IsSnooze ()) {
            // Snooze中なら命令実行しないのでstep数を更新しない
            // ブレークもしない
            ht->RunModule ();
            m_internals->UpdateHeadHtOrder (ht);
        } else {
            if (unlikely (ht->IsBreakHitPreExec () == true) ) {
                if (ht->IsCpu ()) {
                    break_scid = ht->GetScId ();
                }
                break;
            }
//            CfMsg::DPrint (MSG_ERR, "RunModule executed HT->SCID=%d, SCID=%d\n",
//                    ht->GetScId (), scid);
#if defined(COMP_RUNNER_G4MH)
            // In CompRunnerG4MH, Intc1 is included in verification
            // Interrupt from Intc1-SV side can send to CPU at any cycle
            // Therefore, Fsbus is required to excute at each commit stepexecute
            ht->RunModule ();
            m_internals->UpdateHeadHtOrder (ht);
#else
            if (ht->RunModule () == TIME_ELAPSED) {
                // 時間経過があれば実行順を更新
                m_internals->UpdateHeadHtOrder (ht);
            }
#endif

#ifdef ENABLE_TRACE_BUF
            if (m_internals->IsBreakTraceFull ()) {
                break_scid = ht->GetScId ();
                break;
            }
#endif // ENABLE_TRACE_BUF

            if (unlikely (ht->IsBreakHitPostExec () == true)) {
                // ブレーク再開後はシステム時間に到達していないCPUから
                // 実行が再開されるため、挙動はあう
                // GDBにブレークしたHTを通知するためにIDを取得
                if (ht->IsCpu ()) {
                    break_scid = ht->GetScId ();
                }
                break;
#if defined(COMP_RUNNER)
            } else if (ht->GetScId () == scid) {
                break;
#endif // defined(COMP_RUNNER)
            } else if (run_mode != RUN_MODE) {
                // STEPモードで 指定CPUならSTEPカウント更新
                // STEP_ANYモードならいずれかのCPUが実行されたら更新
                if (  ((run_mode == STEP_MODE) && (ht->GetScId () == scid))
                   || ((run_mode == STEP_ANY_MODE) && ht->IsCpu ()) ) {
#if defined(SUPPORT_SC_HEAP)
                    // step数は実行完了時にカウント
                    if (static_cast<FsCtrl*>(ht)->GetSuspendState () == NO_SUSPEND) {
#endif // defined(SUPPORT_SC_HEAP)
                        ++ step_cnt;
                        if (step_cnt == step_goal) {
                            // システム時間に未到達の状態で中断
                            // 再開時はIDの若いCPUから再開
#if defined(SUPPORT_SC_HEAP)
                            PhysAddr inst_addr = static_cast<FsCtrl*>(ht)->
                                                 GetTraceCtrl ()->GetInstAddr ();
                            m_internals->NotifyHitBreak (BREAK_STEP_EXE, inst_addr);
                            break_scid = ht->GetScId ();
#endif // defined(SUPPORT_SC_HEAP)
                            break;
                        }
#if defined(SUPPORT_SC_HEAP)
                    }
#endif // defined(SUPPORT_SC_HEAP)
                }
            }
        }

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
        ht = m_cedar_run.front ();
        // 未完了のメモリアクセスが存在するならSCHEAPに制御を戻す
        if (ht->IsSuspendState () == true) {
            // 他に同じ時刻で自分以外に実行可能なスレッドがなければブレーク
            //   SCHEAP管理のAPBアクセス時にだけこの条件は成立し、
            //   頻度低いはずなので、多少重くても精度重視で判定。
            if (m_internals->IsPendableHtExec () == false) {
                break;
            } else {
                ht = m_cedar_run.front ();
            }
        }

        // 指定時間を経過したらブレーク
        if (m_internals->GetSystemTime () >= finish_time) {
            if (step_cnt == 0){
                // 1命令も実行せずに指定時間を経過したらタイムアウト扱い
                m_internals->SetBreakByTimeout ();
            }
            break;
        }
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    }

    return(break_scid);
}


#if defined(COMP_RUNNER_G3KH)
int32_t ForestUtil::ForestStepModeExecute (int32_t scid)
{
    m_stop_req = false;
    int32_t break_scid = -1;
    int ht_no = m_cedar_run.size ();
    int ht_cycle = 0;

    while (1) {
        ModuleRun* ht = m_cedar_run.front ();

        if (!(ht->IsSnooze ()) && ht->IsCpu() && (ht->GetScId () != scid)) {
            int current_ht = 0;
            RunList::iterator it_source = m_cedar_run.begin ();
            RunList::iterator it_dest = m_cedar_run.end ();
            while ((*it_source)->GetScId() != scid) {
                rotate (it_source, it_source + 1, it_dest);
                current_ht++;
                if (current_ht == ht_no) {
                        std::cout << "< Error: SCID " << GetPeId(scid) << " is not found >";
                    break;
                }
            }
            ht = (*it_source);
        }
        if (ht->IsSnooze ()) {
            ht->RunModule ();
            m_internals->UpdateHeadHtOrder (ht);
        } else {
            if (unlikely (ht->IsBreakHitPreExec () == true) ) {
                if (ht->IsCpu ()) {
                    break_scid = ht->GetScId ();
                }
                break;
            }
            if (ht->RunModule () == TIME_ELAPSED) {
                m_internals->UpdateHeadHtOrder (ht);
            }
            if (unlikely (ht->IsBreakHitPostExec () == true) ) {
                if (ht->IsCpu ()) {
                    break_scid = ht->GetScId ();
                }
                break;
            } else if (ht->GetScId () == scid) {
                break;
            }
            ht_cycle++;
            if (ht_cycle > 1000) {
                std::cout<< std::endl << "### Error: SCID over 10 times!!! " << std::endl;
                break;
            }
        }
    }
    return(break_scid);
}

#endif // defined(COMP_RUNNER_G3KH)


//! CubeSuite用ステップ実行
void ForestUtil::CubeStepExecute (void)
{
    ModuleRun* ht = m_cedar_run.front ();

    if (ht->RunModule () == TIME_ELAPSED) {
        // 時間経過があれば実行順を更新
        m_internals->UpdateHeadHtOrder (ht);
    }
}


//! 次に実行可能なHTまで実行を進める
//! Snooze中のHTがいたら、全HTが寝ていても進める。
void ForestUtil::SkipHaltHt(void)
{
    while (1) {
        ModuleRun* ht = m_cedar_run.front ();
        if ( (ht->IsCpu () == true) && (ht->IsSnooze () == false) ) {
            // 実行可能TCをみつけた
            // Snoozeなら空回しする
            return;
        }

        // 対象が非CPU、またはCPUだがSNOOZE中だったので時間経過
        if (ht->RunModule () == TIME_ELAPSED) {
            // 時間経過があれば実行順を更新
            m_internals->UpdateHeadHtOrder (ht);
        }

        ht = m_cedar_run.front ();

        if (unlikely (m_internals->IsAllInHalt () == true) ) {
            return;
        }
    }
}


ModuleGuard* ForestUtil::GetGuardPtr (uint32_t id, GuardAttr attr)
{
    //In case of attr=GUARD_NONE, any peid is permitted.
    //In case of attr=GUARD_PEG, id is peid of valid PE.
    //In case of attr=GUARD_CRG, id is clid of valid Cluster (any id is ok).
    //In case of attr=GUARD_CFG, any id is permitted.
    if ((attr == GUARD_PEG) && (IsValidPeId (id) == false)) {
        return NULL; // error
    }
    // @@ TODO : CFG is located to where (each cluster has CFG? or CFG is 1 in system)?

    ModuleGuard* guard;
    switch (attr) {
    case GUARD_NONE:
        guard = m_forest->m_dummy_guard;
        break;
    case GUARD_PEG:
        if (IsEnableGuard (GMID_PEG_PEnCLm, id)) { // id is PEID
            guard = m_forest->GetPePtr(id)->GetPegPtr()->GetPegGuard ();
        } else {
            guard = m_forest->m_dummy_guard;
        }
        break;
    case GUARD_CRG:
        if (IsEnableGuard (GMID_CRG_CLm, id)) { // id is CLID
            guard = m_forest->m_dummy_guard;
            Cforest::CrgVec::const_iterator it = m_forest->m_crg.begin ();
            while (it != m_forest->m_crg.end ()) {
                if ((*it)->GetClid () == id) {
                    guard = (*it)->GetCrgGuard ();
                    break;
                }
                ++ it;
            }
        } else {
            guard = m_forest->m_dummy_guard;
        }
        break;
    case GUARD_CFG:
        guard = m_forest->m_dummy_guard;
          break;
    default:
        guard = NULL; // error
    }
    return guard;
}


#ifdef SUPPORT_SC_HEAP
uint64_t ForestUtil::CycleExecute (uint32_t peid,
                                   uint64_t cycle_goal, ExecType exec_type)
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    uint64_t start_time = m_internals->GetSystemTime ();
    ClearBreakEvent ();
    int32_t break_scid = 0;

    m_internals->SetTimeout (cycle_goal);

    uint64_t step_count;
    if (exec_type == EXEC_TYPE_STEP) {
        step_count = 1;
    } else {
        step_count = 0;
    }

    int32_t scid = GetScId (peid);
    if (m_internals->IsExecutable () == true) {
        break_scid = StepExecute<STEP_MODE> (scid, step_count);
    } else {
        // 非SuspendなHTが存在しなければ、時間を+1進めて再実行。
        //   SC_HEAP側でSystemC時間が経過しているはずなので。
        m_internals->Consume1Cycle ();
        if (m_internals->IsExecutable () == true) {
            break_scid = StepExecute<STEP_MODE> (scid, step_count);
        }
    }
    fflush(m_global_log); // add Yoshinaga for SC-HEAP
    m_internals->UpdateBreakInfo (break_scid);
    return m_internals->GetSystemTime () - start_time;
}


BreakInfo* ForestUtil::GetBreakInfo (void)
{
    return m_internals->GetBreakInfo ();
}


void ForestUtil::NotifyFetchLatency (SideBand sideband, PhysAddr addr, uint32_t latency)
{
    if (IsValidBmId (sideband.GetBMID ()) == false) {
        return;
    }

    m_internals->GetHtFromSideband (sideband)->NotifyFetchLatency (addr, latency);
}


void ForestUtil::NotifyReadLatency (SideBand sideband, PhysAddr addr, uint32_t latency,
                                    uint64_t data)
{
    if (IsValidBmId (sideband.GetBMID ()) == false) {
        return;
    }

    m_internals->GetHtFromSideband (sideband)->NotifyReadLatency (addr, latency, data);
}


void ForestUtil::NotifyWriteLatency (SideBand sideband, PhysAddr addr, uint32_t latency)
{
    if (IsValidBmId (sideband.GetBMID ()) == false) {
        return;
    }

    m_internals->GetHtFromSideband (sideband)->NotifyWriteLatency (addr, latency);
}


const TraceCtrl* ForestUtil::DequeueTrace (void)
{
    return m_internals->DequeueTrace ();
}


uint32_t ForestUtil::GetUsedTraceSize (void)
{
    return m_internals->GetUsedTraceSize ();
}


CfApiRetern ForestUtil::SetMaxTraceSize (uint32_t num)
{
    return m_internals->SetMaxTraceSize (num);
}


GuardErrType ForestUtil::FlashAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
    if ((addr < m_rom_start_addr) || (addr > m_rom_end_addr)) {
        // Out of ROM area
        return GD_INVALID;
    }

    uint32_t peid = GetPeidFromBmid (sideband.GetBMID ());
    if (IsValidPeId (peid) == false) {
        return GD_INVALID;
    }

    // CFG of E2x isn't supported

    SysErrCause err;
    if (cmd == DATA_WRITE) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    } else if (cmd == INST_READ) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    } else {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    if (err != SYSERR_NONE) {
        if (cmd != DATA_WRITE) {
            memset (data, 0x0, sizeof(uint8_t) * size);
        }
        if ((err & SYSERR_CRG) == SYSERR_CRG) {
            return GD_CRG_ERR; // don't come here
        } else if ((err & SYSERR_PEG) == SYSERR_PEG) {
            if ((err & SYSERR_REC_MASTER) == SYSERR_REC_MASTER) {
                return GD_PEG_ERR_M; // don't come here
            } else {
                return GD_PEG_ERR_S; // don't come here
            }
        } else {
            return GD_INVALID; // Access failure (invalid address or etc)
        }
    } else {
        return GD_NONE_ERR;
    }
}


GuardErrType ForestUtil::FlashAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
    if ((addr < m_rom_start_addr) || (addr > m_rom_end_addr)) {
        // Out of ROM area
        return GD_INVALID;
    }

    // CFG of E2x isn't supported

    SysErrCause err;
    if (cmd == DATA_WRITE) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    } else if (cmd == INST_READ) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    } else {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    if (err != SYSERR_NONE) {
        if (cmd != DATA_WRITE) {
            memset (data, 0x0, sizeof(uint8_t) * size);
        }
        if ((err & SYSERR_CRG) == SYSERR_CRG) {
            return GD_CRG_ERR; // don't come here
        } else if ((err & SYSERR_PEG) == SYSERR_PEG) {
            if ((err & SYSERR_REC_MASTER) == SYSERR_REC_MASTER) {
                return GD_PEG_ERR_M; // don't come here
            } else {
                return GD_PEG_ERR_S; // don't come here
            }
        } else {
            return GD_INVALID; // Access failure (invalid address or etc)
        }
    } else {
        return GD_NONE_ERR;
    }
}


GuardErrType ForestUtil::CramAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
    if ((addr < m_gram_start_addr) || (addr > m_gram_end_addr)) {
        // Out of CRAM area
        return GD_CRG_ILLACC;
    }

    uint32_t peid = GetPeidFromBmid (sideband.GetBMID ());
    if (IsValidPeId (peid) == false) {
        return GD_INVALID;
    }

    SysErrCause err;
    if (cmd == DATA_WRITE) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    } else if (cmd == INST_READ) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    } else {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    if (err != SYSERR_NONE) {
        if (cmd != DATA_WRITE) {
            memset (data, 0x0, sizeof(uint8_t) * size);
        }
        if ((err & SYSERR_CRG) == SYSERR_CRG) {
            return GD_CRG_ERR;
        } else if ((err & SYSERR_PEG) == SYSERR_PEG) {
            if ((err & SYSERR_REC_MASTER) == SYSERR_REC_MASTER) {
                return GD_PEG_ERR_M; // don't come here
            } else {
                return GD_PEG_ERR_S; // don't come here
            }
        } else {
            return GD_CRG_ILLACC; // Access failure (invalid address or etc)
        }
    } else {
        if (cmd == DATA_WRITE) {
            m_internals->DeleteLinkByExternal (addr, size);
        }
        return GD_NONE_ERR;
    }
}


GuardErrType ForestUtil::CramAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
    if ((addr < m_gram_start_addr) || (addr > m_gram_end_addr)) {
        // Out of CRAM area
        return GD_CRG_ILLACC;
    }

    SysErrCause err;
    if (cmd == DATA_WRITE) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    } else if (cmd == INST_READ) {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    } else {
        err = m_forest->fsbus->BusAccessPhys<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    if (err != SYSERR_NONE) {
        if (cmd != DATA_WRITE) {
            memset (data, 0x0, sizeof(uint8_t) * size);
        }
        if ((err & SYSERR_CRG) == SYSERR_CRG) {
            return GD_CRG_ERR;
        } else if ((err & SYSERR_PEG) == SYSERR_PEG) {
            if ((err & SYSERR_REC_MASTER) == SYSERR_REC_MASTER) {
                return GD_PEG_ERR_M; // don't come here
            } else {
                return GD_PEG_ERR_S; // don't come here
            }
        } else {
            return GD_CRG_ILLACC; // Access failure (invalid address or etc)
        }
    } else {
        if (cmd == DATA_WRITE) {
            m_internals->DeleteLinkByExternal (addr, size);
        }
        return GD_NONE_ERR;
    }
}


SysErrCause ForestUtil::CfgRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
    // CFG of E2x isn't supported
    if (cmd != DATA_WRITE) {
        memset (data, 0x0, sizeof(uint8_t) * size);
    }
    return SYSERR_NONE;
}


SysErrCause ForestUtil::CrgRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
    if (cmd != DATA_WRITE) {
        memset (data, 0x0, sizeof(uint8_t) * size);
    }

    SysErrCause err;
    if (cmd == DATA_WRITE) {
        err = m_forest->m_crg_bus->BusAccessPhys<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    } else if (cmd == INST_READ) {
        err = m_forest->m_crg_bus->BusAccessPhys<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    } else {
        err = m_forest->m_crg_bus->BusAccessPhys<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    if (err != SYSERR_NONE) {
        if (cmd != DATA_WRITE) {
            memset (data, 0x0, sizeof(uint8_t) * size);
        }
    }
    return err;
}


SysErrCause ForestUtil::PegRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
    if (cmd != DATA_WRITE) {
        memset (data, 0x0, sizeof(uint8_t) * size);
    }

    SysErrCause err;
    if (cmd == DATA_WRITE) {
        err = m_forest->m_peg_bus->BusAccessPhys<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    } else if (cmd == INST_READ) {
        err = m_forest->m_peg_bus->BusAccessPhys<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    } else {
        err = m_forest->m_peg_bus->BusAccessPhys<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    if (err != SYSERR_NONE) {
        if (cmd != DATA_WRITE) {
            memset (data, 0x0, sizeof(uint8_t) * size);
        }
    }
    return err;
}


SysErrCause ForestUtil::BusWritePhys (SideBand sideband, PhysAddr addr,
                                      uint8_t* data, ByteSize size)
{
    m_internals->DeleteLinkByExternal (addr, size);
    return m_forest->fsbus->BusWritePhys (sideband, addr, data, size);
}


SysErrCause ForestUtil::BusReadPhys (SideBand sideband, PhysAddr addr,
                                     uint8_t* data, ByteSize size)
{
    return m_forest->fsbus->BusReadPhys (sideband, addr, data, size);
}


SysErrCause ForestUtil::BusFetchPhys (SideBand sideband, PhysAddr addr,
                                     uint8_t* data, ByteSize size)
{
    return m_forest->fsbus->BusFetchPhys (sideband, addr, data, size);
}


bool ForestUtil::SetPeLramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr)
{
    if ((m_forest->m_invalid_lram == NULL) || (start_addr > end_addr)) {
        return false;
    }

    if (m_forest->m_invalid_lram->RegisterLramArea (peid, start_addr, end_addr) == false) {
        // 登録済みとぶつかる
        return false;
    }

    if (IsValidPeId (peid) == true) {
        // If valid PE, PE local RAM with PEG
        m_forest->fsbus->AddTargetVC (start_addr, end_addr, m_forest->m_invalid_lram, NULL,
                                      GetGuardPtr (peid, GUARD_PEG),
                                      m_internals->CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
    } else {
        // If invalid PEID, alloc at global
        m_forest->fsbus->AddTargetVC (start_addr, end_addr, m_forest->m_invalid_lram, NULL,
                                      m_forest->m_dummy_guard,
                                      m_internals->CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
    }
    return true;
}


#endif // SUPPORT_SC_HEAP


void ForestUtil::HardReset (void)
{
    m_forest->Reset ();
    m_internals->UpdatePeidArray ();
    m_internals->InitTime ();

#ifdef SUPPORT_SC_HEAP
    m_internals->InitTrace ();
#endif // SUPPORT_SC_HEAP
}


//! \sa ProcElement::FsReset
bool ForestUtil::FsPeReset (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    return m_forest->GetPePtr(peid)->FsReset ();
}


//! \sa ProcElement::HardReset
void ForestUtil::ResetPE (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    m_forest->GetPePtr(peid)->HardReset ();
}


//! \sa FsCtrl::RestartVm
void ForestUtil::ResetVM (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    HtVec::const_iterator it = m_forest->GetPePtr(peid)->GetHtsPtr()->begin ();
    while (it != m_forest->GetPePtr(peid)->GetHtsPtr()->end ()) {
        if ((*it)->m_vc_id == NC) {
            (*it)->RestartVm ();
        }
        ++ it;
    }
}


#ifdef ENABLE_CLUSTER
ForestUtil::KeepClItem ForestUtil::GetClLatency(uint32_t peid) const
{
    KeepClMap::const_iterator itr_cl;
    MemLocationType mem_location(peid);
    itr_cl = m_keep_cl_latency.find(mem_location.GetBindCL());
    if (itr_cl != m_keep_cl_latency.end()) {
        return itr_cl->second;
    }
    return KeepClItem(0, 0);
}


ForestUtil::KeepPeItem ForestUtil::GetPeLatency(uint32_t peid) const
{
    KeepPeMap::const_iterator itr_pe;
    MemLocationType mem_location(peid);
    itr_pe = m_keep_pe_latency.find(mem_location.Get());
    if (itr_pe != m_keep_pe_latency.end()) {
        return itr_pe->second;
    }
    return KeepPeItem(0, 0);
}


void ForestUtil::UpdateExoClLatency (uint32_t clid,
                                    uint32_t master_latency_offset, uint32_t slave_latency_offset)
{
#if defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    // SC-HEAP/CubeSuiteではクラスタレイテンシは非サポートなので更新しない。
    return;
#else // defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    std::pair<KeepClMap::iterator, bool> result;
    result = m_keep_cl_latency.insert(
                 KeepClMapElm(clid, KeepClItem(master_latency_offset, slave_latency_offset))
             );
    if (!result.second ) {
        m_keep_cl_latency.erase(result.first);
        m_keep_cl_latency.insert(
            KeepClMapElm(clid, KeepClItem(master_latency_offset, slave_latency_offset))
        );
    }
    return;

    // Update Memory latency
    // Although this API is CL latency setting, UpdateExoPeLatency() is called.
    // Because this is to call MemLatencyInfo::SetupMemModuleLatency().
    m_forest->fsbus->UpdateExoPeLatency (0); // 0 is dummy value.
#endif
}


void ForestUtil::UpdateExoPeLatency (uint32_t  peid,
                                    uint32_t master_latency_offset, uint32_t slave_latency_offset)
{
#if defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    // SC-HEAP/CubeSuiteではexo_peレイテンシを直接指定するので、
    // pe_latencyとexo_pe_latencyは独立。再計算できないので更新しない。
    return;
#else // defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    std::pair<KeepPeMap::iterator, bool> result;
    result = m_keep_pe_latency.insert(
                 KeepPeMapElm(peid, KeepPeItem (master_latency_offset, slave_latency_offset))
             );
    if (!result.second) {
        m_keep_pe_latency.erase(result.first);
        m_keep_pe_latency.insert(
             KeepPeMapElm(peid, KeepPeItem (master_latency_offset, slave_latency_offset))
        );
    }

    // Update Memory latency
    m_forest->fsbus->UpdateExoPeLatency (0); // 0 is dummy value.
#endif // defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
}


#else // ENABLE_CLUSTER
void ForestUtil::UpdateExoPeLatency (uint32_t exo_latency_offset)
{
#if defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    // SC-HEAP/CubeSuiteではexo_peレイテンシを直接指定するので、
    // pe_latencyとexo_pe_latencyは独立。再計算できないので更新しない。
    return;
#else // defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    m_forest->fsbus->UpdateExoPeLatency (exo_latency_offset);
#endif // defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
}
#endif // ENABLE_CLUSTER


bool ForestUtil::IorResourceAlloc (MemRestrictType restrict_type,
                                   PhysAddr start_addr,
                                   PhysAddr end_addr,
                                   uint32_t pe_read_latency,
                                   uint32_t pe_write_latency,
                                   uint32_t cl_read_latency,
                                   uint32_t cl_write_latency,
                                   uint32_t exo_cl_read_latency,
                                   uint32_t exo_cl_write_latency,
                                   MemLocationType pe_location,
                                   MemLocationType cl_location,
                                   uint32_t num_outstand,
                                   uint32_t repeat_rate)
{
#ifdef SUPPORT_CUBESUITE
    if (start_addr > end_addr) return false;

    CubeIoReg* cube_ior = new CubeIoReg (start_addr);
    MemLatencyInfoBase* mem_latency = new MemLatencyInfo<DBUF_SIMPLE_MEM>
                                          (pe_read_latency,
                                           pe_read_latency,
                                           pe_write_latency,
                                           cl_read_latency,
                                           cl_read_latency,
                                           cl_write_latency,
                                           exo_cl_read_latency,
                                           exo_cl_read_latency,
                                           exo_cl_write_latency,
                                           pe_location,
                                           cl_location,
                                           num_outstand,
                                           repeat_rate,
                                           MEM_RES_OTHERS,
                                           "ior");
    bool ret = m_forest->fsbus->AddTargetVC (static_cast<uint32_t>(pe_location),
                                             MemLocationInternal,
                                             start_addr, end_addr, cube_ior, NULL,
                                             m_forest->m_dummy_guard, mem_latency);
    if (ret == false) {
        // 追加失敗したのでdelete。
        delete cube_ior;
        delete mem_latency;
    } else {
        m_forest->m_cube_iors.push_back (cube_ior);

        // サポート済み周辺：アドレス範囲内に存在するサポート済み周辺のレイテンシを変更
        SetPeripheralLatency (start_addr, end_addr,
                              pe_read_latency, pe_write_latency,
                              cl_read_latency, cl_write_latency,
                              exo_cl_read_latency, exo_cl_write_latency,
                              pe_location, cl_location, num_outstand, repeat_rate);
    }
    return ret;
#endif // SUPPORT_CUBESUITE
    return true;
}


bool ForestUtil::IorResourceFree (PhysAddr start_addr)
{
#ifdef SUPPORT_CUBESUITE
    std::vector<CubeIoReg*>::iterator it =  m_forest->m_cube_iors.begin ();
    while (it != m_forest->m_cube_iors.end ()) {
        if ((*it)->m_start_addr == start_addr) {
            bool ret = m_forest->fsbus->RemTargetVC (start_addr, (*it), NULL);
            if (ret == true) {
                delete (*it);
                m_forest->m_cube_iors.erase (it);
            }
            return ret;
        }
        ++ it;
    }
    return false;
#endif // SUPPORT_CUBESUITE
    return true;
}


bool ForestUtil::SetPeripheralLatency (PhysAddr start_addr, PhysAddr end_addr,
                                       uint32_t pe_read_latency,
                                       uint32_t pe_write_latency,
                                       uint32_t cl_read_latency,
                                       uint32_t cl_write_latency,
                                       uint32_t exo_cl_read_latency,
                                       uint32_t exo_cl_write_latency,
                                       MemLocationType pe_location,
                                       MemLocationType cl_location,
                                       uint32_t num_outstand,
                                       uint32_t repeat_rate)
{
    // アドレス範囲に含まれる周辺モデルのレイテンシを更新
    bool is_found = false;
    FsBus::BusTargetVec::const_iterator it = m_forest->fsbus->m_bus_targets.begin();
    while (it != m_forest->fsbus->m_bus_targets.end()) {
        if ((*it)->m_membody == NULL) {
            PhysAddr tgt_start = (*it)->m_btgt_start_addr;
            PhysAddr tgt_end = (*it)->m_btgt_end_addr;
            if (  (start_addr <= tgt_start && end_addr >= tgt_start)
               || (start_addr <= tgt_end && end_addr >= tgt_end)
               || (start_addr >= tgt_start && end_addr <= tgt_end) ) {
                // 発見
                is_found = true;
                MemLatencyInfoBase* mem_latency =
                    new MemLatencyInfo<DBUF_SIMPLE_MEM> (pe_read_latency,
                                                      pe_read_latency,
                                                      pe_write_latency,
                                                      cl_read_latency,
                                                      cl_read_latency,
                                                      cl_write_latency,
                                                      exo_cl_read_latency,
                                                      exo_cl_read_latency,
                                                      exo_cl_write_latency,
                                                      pe_location,
                                                      cl_location,
                                                      num_outstand,
                                                      repeat_rate,
                                                      MEM_RES_OTHERS,
                                                      "peripheral");
                // 既存をdeleteしてからセット
                MemLatencyInfoBase* curr = (*it)->m_latency;
                // NULLに設定しないと、IsDeletableLatencyInfoで自分自身が引っかかる。
                (*it)->m_latency = NULL;
                if (m_forest->fsbus->IsDeletableLatencyInfo (curr)) {
                    delete curr;
                }
                (*it)->m_latency = mem_latency;
            }
        }
        ++ it;
    }
    return is_found;
}


// CubeSuiteのLocalIOR設定用
// 既存領域との競合を無視して、smemを配置。
// これにより、サポート済み周辺なら周辺に、非サポート周辺ならsmemにアクセスが行く。
bool ForestUtil::LocalIorResourceAlloc (MemRestrictType restrict_type,
                                        PhysAddr start_addr, PhysAddr end_addr,
                                        uint32_t pe_read_latency,
                                        uint32_t pe_write_latency,
                                        uint32_t cl_read_latency,
                                        uint32_t cl_write_latency,
                                        uint32_t exo_cl_read_latency,
                                        uint32_t exo_cl_write_latency,
                                        MemLocationType pe_location,
                                        MemLocationType cl_location,
                                        uint32_t num_outstand,
                                        uint32_t repeat_rate)
{
#ifdef SUPPORT_CUBESUITE
    if (start_addr > end_addr) return false;

    // 非サポート周辺：ダミーメモリを配置
    Csmem* smem = new Csmem (restrict_type, GetMemInit ());
    MemLatencyInfoBase* mem_latency = new MemLatencyInfo<DBUF_SIMPLE_MEM>
                                          (pe_read_latency,
                                           pe_read_latency,
                                           pe_write_latency,
                                           cl_read_latency,
                                           cl_read_latency,
                                           cl_write_latency,
                                           exo_cl_read_latency,
                                           exo_cl_read_latency,
                                           exo_cl_write_latency,
                                           pe_location,
                                           cl_location,
                                           num_outstand,
                                           repeat_rate,
                                           MEM_RES_OTHERS,
                                           "peripheral");

    // メモリ確保
    bool ret = smem->MemResourceAlloc (start_addr, end_addr);
    if (ret == false) {
        // 追加失敗したのでdelete。
        delete smem;
        delete mem_latency;
    } else {
        ret = m_forest->fsbus->AddTargetVC (static_cast<uint32_t>(pe_location),
                                            MemLocationInternal,
                                            start_addr, end_addr, smem, smem->mem_body,
                                            m_forest->m_dummy_guard, mem_latency);
        if (ret == false) {
            // 追加失敗したのでdelete。
            delete smem;
            delete mem_latency;
        } else {
            m_forest->m_cube_lior.push_back (Cforest::CubeLiorInfo(start_addr, smem));

            // サポート済み周辺：アドレス範囲内に存在するサポート済み周辺のレイテンシを変更
            SetPeripheralLatency (start_addr, end_addr,
                                  pe_read_latency, pe_write_latency,
                                  cl_read_latency, cl_write_latency,
                                  exo_cl_read_latency, exo_cl_write_latency,
                                  pe_location, cl_location, num_outstand, repeat_rate);
        }
    }
    return ret;
#endif // SUPPORT_CUBESUITE
    return true;
}


bool ForestUtil::LocalIorResourceFree (PhysAddr start_addr, PhysAddr end_addr)
{
#ifdef SUPPORT_CUBESUITE
    std::vector<Cforest::CubeLiorInfo>::iterator it =  m_forest->m_cube_lior.begin ();
    while (it != m_forest->m_cube_lior.end ()) {
        if (it->first == start_addr) {
            bool ret = m_forest->fsbus->RemTargetVC (start_addr, it->second, it->second->mem_body);
            if (ret == true) {
                delete it->second;
                m_forest->m_cube_lior.erase (it);
            }
            return ret;
        }
        ++ it;
    }
    return false;
#endif // SUPPORT_CUBESUITE
    return true;
}


//! Allocate memory resource
/*!
  \return Error code.
  \retval true  Allocate memory correctly.
  \retval false Fail to allocate memory.
*/
bool ForestUtil::MemResourceAlloc (MemRestrictType restrict_type,
                                   uint32_t pe_fetch_latency,
                                   uint32_t pe_read_latency,
                                   uint32_t pe_write_latency,
                                   uint32_t cl_fetch_latency,
                                   uint32_t cl_read_latency,
                                   uint32_t cl_write_latency,
                                   uint32_t exo_cl_fetch_latency,
                                   uint32_t exo_cl_read_latency,
                                   uint32_t exo_cl_write_latency,
                                   PhysAddr start_addr,
                                   PhysAddr end_addr,
                                   MemMappedArea area_type,
                                   int32_t  offset,
                                   MemLocationType pe_location,
                                   MemLocationType cl_location,
                                   uint32_t num_outstand,
                                   uint32_t repeat_rate,
                                   std::vector<ByteSize>* bank_size)
{
    if (start_addr > end_addr) return false;

    uint32_t pe_id = pe_location;
    if ((pe_id != MEM_LOCATE_GLOBAL) && (IsValidPeId (pe_id) == false)) {
        return false;
    }

    // For CLID, any ID is permitted.
    uint32_t cl_id = cl_location;

    if (num_outstand == 0) {
        // Outstand数は1以上
        return false;
    }

    // 対象アドレスに配置された周辺モデルを確認
    FsBus::BusTarget* tgt = m_forest->fsbus->SearchAccessTarget (start_addr);
    if ((m_forest->fsbus->IsTargetValid (tgt) == true) && (m_forest->smem0 != tgt->m_btgt_vc)) {
        // 対象は確保済みsmem領域または非メモリ
        // - 確保済みsmemの拡張/縮小には非対応で、エラー扱い
        return false;
    }

    // 対象領域にモジュールが配置されていない or 全領域smem
    // メモリを確保する前に、新規smemを配置する
    Csmem* smem = new Csmem (restrict_type, GetMemInit ());

    std::stringstream route_str;
    if (cl_id == MEM_LOCATE_GLOBAL) {
        ; // No suffix
    } else if (pe_id == MEM_LOCATE_GLOBAL) {
        route_str << "C" << cl_id;
    } else {
        route_str << "P" << pe_id;
    }
    if ((restrict_type & MRT_WRITE_ENABLE) == 0x0) {
        // Write access is unpermitted.
        route_str << "ROM";
    } else {
        route_str << "RAM";
    }
    std::string tgt_name = route_str.str();

    // メモリ確保
    bool ret = smem->MemResourceAlloc (start_addr, end_addr);
    if (ret == true) {
        // 確保成功した場合だけ設定
        m_forest->m_smem.push_back (smem);
        MemLatencyInfoBase* mem_latency;
        if ( (start_addr >= m_rom_start_addr)
          && (start_addr < m_rom_end_addr) ) {
            // ROM領域なら特殊経路があるのでレイテンシを削減,DBUF有効
            if (GetCoreType (pe_id) == CORE_G4MH) {
                if (bank_size->size () > 1) {
                    mem_latency = new MemLatencyInfoBank<DBUF_FROM_G4MH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    bank_size,
                                                    start_addr,
                                                    end_addr,
                                                    MEM_RES_FROM,
                                                    tgt_name);
                } else {
                    mem_latency = new MemLatencyInfo<DBUF_FROM_G4MH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    MEM_RES_FROM,
                                                    tgt_name);
                }
            } else if (GetCoreType (pe_id) == CORE_G4KH) {
                if (bank_size->size () > 1) {
                    mem_latency = new MemLatencyInfoBank<DBUF_FROM_G4KH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    bank_size,
                                                    start_addr,
                                                    end_addr,
                                                    MEM_RES_FROM,
                                                    tgt_name);
                } else {
                    mem_latency = new MemLatencyInfo<DBUF_FROM_G4KH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    MEM_RES_FROM,
                                                    tgt_name);
                }
            } else {
                // Unreachable
                mem_latency = NULL; // To avoid compiler warning.
            }
        } else if (pe_location == MEM_LOCATE_GLOBAL) {
            // CRAM
            pe_id = GetPeMasterId ();
            if (GetCoreType (pe_id) == CORE_G4MH) {
                if (bank_size->size () > 1) {
                    mem_latency = new MemLatencyInfoBank<DBUF_CRAM_G4MH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    bank_size,
                                                    start_addr,
                                                    end_addr,
                                                    MEM_RES_CRAM,
                                                    tgt_name);
                } else {
                    mem_latency = new MemLatencyInfoGram
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    tgt_name);
                }
            } else {
                // G4KH, G3K
                if (bank_size->size () > 1) {
                    mem_latency = new MemLatencyInfoBank<DBUF_CRAM_G4KH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    bank_size,
                                                    start_addr,
                                                    end_addr,
                                                    MEM_RES_CRAM,
                                                    tgt_name);
                } else {
                    mem_latency = new MemLatencyInfo <DBUF_CRAM_G4KH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    MEM_RES_CRAM,
                                                    tgt_name);
                }
            }
        } else {
            // ROM領域以外はDBUF無効
            if (GetCoreType (pe_id) == CORE_G4MH) {
                if (bank_size->size () > 1) {
                    mem_latency = new MemLatencyInfoBank<DBUF_LRAM_G4MH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    bank_size,
                                                    start_addr,
                                                    end_addr,
                                                    MEM_RES_LRAM,
                                                    tgt_name);
                } else {
                    mem_latency = new MemLatencyInfoLram
                                                     (pe_fetch_latency,
                                                     pe_read_latency,
                                                     pe_write_latency,
                                                     cl_fetch_latency,
                                                     cl_read_latency,
                                                     cl_write_latency,
                                                     exo_cl_fetch_latency,
                                                     exo_cl_read_latency,
                                                     exo_cl_write_latency,
                                                     pe_location,
                                                     cl_location,
                                                     num_outstand,
                                                     repeat_rate,
                                                     tgt_name);
                }
            } else {
                // G4KH, G3K
                if (bank_size->size () > 1) {
                    mem_latency = new MemLatencyInfoBank<DBUF_LRAM_G4KH>
                                                    (pe_fetch_latency,
                                                    pe_read_latency,
                                                    pe_write_latency,
                                                    cl_fetch_latency,
                                                    cl_read_latency,
                                                    cl_write_latency,
                                                    exo_cl_fetch_latency,
                                                    exo_cl_read_latency,
                                                    exo_cl_write_latency,
                                                    pe_location,
                                                    cl_location,
                                                    num_outstand,
                                                    repeat_rate,
                                                    bank_size,
                                                    start_addr,
                                                    end_addr,
                                                    MEM_RES_LRAM,
                                                    tgt_name);
                } else {
                    mem_latency = new MemLatencyInfo<DBUF_LRAM_G4KH>
                                                     (pe_fetch_latency,
                                                     pe_read_latency,
                                                     pe_write_latency,
                                                     cl_fetch_latency,
                                                     cl_read_latency,
                                                     cl_write_latency,
                                                     exo_cl_fetch_latency,
                                                     exo_cl_read_latency,
                                                     exo_cl_write_latency,
                                                     pe_location,
                                                     cl_location,
                                                     num_outstand,
                                                     repeat_rate,
                                                     MEM_RES_LRAM,
                                                     tgt_name);
                }
            }
        }

        if (   (cl_location != MEM_LOCATE_GLOBAL)
            && (area_type == MI_CF_CRAM)) {
            // ClusterRAM with CRG
            m_forest->fsbus->AddTargetVC (pe_location, cl_location,
                                          start_addr, end_addr, smem, smem->mem_body,
                                          GetGuardPtr (cl_location, GUARD_CRG), mem_latency);
        } else if (area_type == MI_CF_FROM) {
            // CodeFlashROM with CFG
            // CFG of E2x isn't supported
            m_forest->fsbus->AddTargetVC (pe_location, cl_location,
                                          start_addr, end_addr, smem, smem->mem_body,
                                          GetGuardPtr (cl_location, GUARD_CFG), mem_latency);
        } else if (    (pe_location != MEM_LOCATE_GLOBAL)
                    && (area_type == MI_CF_LRAM)) {
            // LocalRAM with PEG
            m_forest->fsbus->AddTargetVC (pe_location, cl_location,
                                          start_addr, end_addr, smem, smem->mem_body,
                                          GetGuardPtr (pe_location, GUARD_PEG), mem_latency);
        } else {
            // memory or peripherals with no guard
            m_forest->fsbus->AddTargetVC (pe_location, cl_location,
                                          start_addr, end_addr, smem, smem->mem_body,
                                          m_forest->m_dummy_guard, mem_latency);
        }
    } else {
        // 確保失敗したので、生成済みのsmemを削除
        delete smem;
    }

    return ret;
}

#ifdef CLUSTER
bool ForestUtil::UpdateClusterCycleInfo (char* bridge_name,
                                         uint32_t clid,
                                         PhysAddr start_addr,
                                         PhysAddr end_addr,
                                         MemRestrictType restrict_type,
                                         uint32_t read_latency,
                                         uint32_t write_latency,
                                         uint32_t fetch_latency,
                                         uint32_t outstand,
                                         uint32_t repeat_rate)
{
    if (IsValidClId (clid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Not Valid ClId = %d>\n", clid);
        return false;
    }

    MemLatencyInfoBase* mem_latency;
    mem_latency = new MemLatencyInfoExCluster (bridge_name,
                                               clid,
                                               read_latency,
                                               write_latency,
                                               fetch_latency,
                                               outstand,
                                               repeat_rate);

    m_forest->fsbus->AddClusterTarget (bridge_name,
                                       clid,
                                       start_addr,
                                       end_addr,
                                       restrict_type,
                                       m_forest->m_dummy_guard,
                                       mem_latency);

    return true;
}
#endif // CLUSTER


//! Free memory resource
/*!
  \return Error code.
  \retval true  Allocate memory correctly.
  \retval false Fail to allocate memory.
*/
bool ForestUtil::MemResourceFree (PhysAddr start_addr, PhysAddr end_addr)
{
    // 対象アドレスに配置された周辺モデルを確認
    FsBus::BusTarget* tgt = m_forest->fsbus->SearchAccessTarget (start_addr);
    if (m_forest->fsbus->IsTargetValid (tgt) == false) {
        // 対象は未確保
        return false;
    }

    // 対象アドレスの周辺モデルがsmemか確認
    SmemVec::iterator it = std::find (m_forest->m_smem.begin(), m_forest->m_smem.end(),
                                      tgt->m_btgt_vc);
    if (it == m_forest->m_smem.end ()) {
        // 対象は非メモリ
        return false;
    }

    // メモリ開放
    Csmem* smem = *it;
    if (m_forest->smem0 != smem) {
        // 対象が全領域smemでない場合、FsBus登録からsmemを削除
        // 複数アドレスにsmemを接続する構成は非対応なので、単純に削除すれば良い
        PhysAddr remove_area = tgt->m_btgt_start_addr;

        // 旧FsBusエントリを削除
        m_forest->fsbus->RemTargetVC (remove_area, smem, smem->mem_body);
        m_forest->m_smem.erase (it);
        delete smem;
        return true;
    } else {
        // 全領域smemなら、該当メモリリソースを削除
        return smem->MemResourceFree (start_addr, end_addr);
    }
}


bool ForestUtil::SetAccessDeniedArea (PhysAddr start_addr, PhysAddr end_addr)
{
    return m_forest->fsbus->SetAccessDeniedArea (start_addr, end_addr);
}


bool ForestUtil::ClearAccessDeniedArea (PhysAddr addr)
{
    return m_forest->fsbus->ClearAccessDeniedArea (addr);
}


bool ForestUtil::SetMirrorArea (PhysAddr start_addr, PhysAddr end_addr,
                                PhysOffset offset)
{
    return m_forest->fsbus->SetMirrorArea (start_addr, end_addr, offset);
}


bool ForestUtil::ClearMirrorArea (PhysAddr addr)
{
    return m_forest->fsbus->ClearMirrorArea (addr);
}


bool ForestUtil::SetErrorArea (PhysAddr start_addr, PhysAddr end_addr)
{
    return m_forest->fsbus->AddTargetVC (start_addr, end_addr, m_forest->m_error_area,
           NULL, m_forest->m_dummy_guard, m_internals->CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
}


bool ForestUtil::ClearErrorArea (PhysAddr addr)
{
    // Check the address
    FsBus::BusTarget* tgt = m_forest->fsbus->SearchAccessTarget (addr);
    if (m_forest->fsbus->IsTargetValid (tgt) == false) {
        // No target
        return false;
    }

    // remove the error area
    if (m_forest->fsbus->RemTargetVC (addr, m_forest->m_error_area, NULL)) {
        return true;
    }

    // Fail to remove target
    return false;
}


bool ForestUtil::SetSelfArea (PhysAddr start_addr, PhysAddr end_addr,
                              PhysOffset offset)
{
    return m_forest->fsbus->SetSelfArea (start_addr, end_addr, offset);
}


bool ForestUtil::ClearSelfArea (PhysAddr addr)
{
    return m_forest->fsbus->ClearSelfArea (addr);
}


std::string ForestUtil::MemResourceDisplay (void)
{
    std::ostringstream mem_info;
    /* メモリを持つターゲットすべてを表示 */
    CfMsg::DPrint (MSG_ERR, "---- Allocated Memory Map ----\n");
    CfMsg::DPrint (MSG_ERR,
            "  <START>      <END>      <RW> <FETCH> <READ> <WRITE> "
            "<CL-FETCH> <CL-READ> <CL-WRITE> <ExtFETCH> <ExtREAD> <ExtWRITE> <PE> <CL> <OUTSTAND> <REPEAT>\n");
    FsBus::BusTargetVec::const_iterator it = m_forest->fsbus->m_bus_targets.begin ();
    mem_info << "Memory ";
    while (it != m_forest->fsbus->m_bus_targets.end ()) {
        MemBody* p_memory = (*it)->m_membody;
        if (p_memory != NULL) {
            MemLatencyInfoBase* mem_latency = (*it)->m_latency;
            if (mem_latency != NULL) {
                MemResourceInfo res_info;
                mem_latency->GetResourceInfo (&res_info, 0);
                mem_info << p_memory->MemResourceDisplay (res_info);
            }
        }
        ++ it;
    }

    if (!m_forest->fsbus->m_self_blocks.empty ()) {
        CfMsg::DPrint (MSG_ERR, "---- Self Area Map ----\n");
        CfMsg::DPrint (MSG_ERR, "  <START>      <END>       <OFFSET>\n");
        mem_info << "Self ";
        FsBus::TgtBlockVec::const_iterator it_self = m_forest->fsbus->m_self_blocks.begin ();
        while (it_self != m_forest->fsbus->m_self_blocks.end ()) {
            if ((*it_self)->m_offset >= 0) {
                CfMsg::DPrint (MSG_ERR, "  0x%08" PRIx32 " - 0x%08" PRIx32 "  0x%08" PRIx32 "\n",
                               (*it_self)->m_start_addr,
                               (*it_self)->m_end_addr,
                               (*it_self)->m_offset);
                mem_info << (*it_self)->m_start_addr << " " <<
                               (*it_self)->m_end_addr << " " <<
                               (*it_self)->m_offset << " | ";
            } else {
                CfMsg::DPrint (MSG_ERR, "  0x%08" PRIx32 " - 0x%08" PRIx32 " -0x%08" PRIx32 "\n",
                               (*it_self)->m_start_addr,
                               (*it_self)->m_end_addr,
                               (*it_self)->m_offset * -1);
                mem_info << (*it_self)->m_start_addr << " " <<
                               (*it_self)->m_end_addr << " " <<
                               (*it_self)->m_offset * -1 << " | ";
            }
            ++ it_self;
        }
    }

    if (!m_forest->fsbus->m_mirr_blocks.empty ()) {
        CfMsg::DPrint (MSG_ERR, "---- Mirror Area Map ----\n");
        CfMsg::DPrint (MSG_ERR, "  <START>      <END>       <OFFSET>\n");
        mem_info << "Mirror ";
        FsBus::TgtBlockVec::const_iterator it_mirr = m_forest->fsbus->m_mirr_blocks.begin ();
        while (it_mirr != m_forest->fsbus->m_mirr_blocks.end ()) {
            if ((*it_mirr)->m_offset >= 0) {
                CfMsg::DPrint (MSG_ERR, "  0x%08" PRIx32 " - 0x%08" PRIx32 "  0x%08" PRIx32 "\n",
                               (*it_mirr)->m_start_addr,
                               (*it_mirr)->m_end_addr,
                               (*it_mirr)->m_offset);
                mem_info << (*it_mirr)->m_start_addr << " " <<
                               (*it_mirr)->m_end_addr << " " <<
                               (*it_mirr)->m_offset << " | ";
            } else {
                CfMsg::DPrint (MSG_ERR, "  0x%08" PRIx32 " - 0x%08" PRIx32 " -0x%08" PRIx32 "\n",
                               (*it_mirr)->m_start_addr,
                               (*it_mirr)->m_end_addr,
                               (*it_mirr)->m_offset * -1);
                mem_info << (*it_mirr)->m_start_addr << " " <<
                               (*it_mirr)->m_end_addr << " " <<
                               (*it_mirr)->m_offset * -1 << " | ";
            }
            ++ it_mirr;
        }
    }
    return mem_info.str();
}



std::string ForestUtil::PeripheralMapDisplay (void)
{
    std::ostringstream peri_info;
    // 周辺(メモリを持たないターゲット)を検索
    std::map <PhysAddr, FsBus::BusTarget*> bus_target;
    FsBus::BusTargetVec::const_iterator it = m_forest->fsbus->m_bus_targets.begin ();
    while (it != m_forest->fsbus->m_bus_targets.end ()) {
        if ((*it)->m_membody == NULL) {
            // 周辺表示用に格納(アドレス順に整列)
            bus_target[(*it)->m_btgt_start_addr] = (*it);
        }
        ++ it;
    }

    // 周辺(メモリを持たないターゲット)を表示
    CfMsg::DPrint (MSG_ERR, "---- Peripherals Memory Map ----\n");
    CfMsg::DPrint (MSG_ERR, "  <START>      <END>      <READ> <WRITE> "
            "<CL-READ> <CL-WRITE> <ExtREAD> <ExtWRITE> <PE> <CL> <OUTSTAND> <REPEAT>\n");
    std::map <PhysAddr, FsBus::BusTarget*>::const_iterator
        itr = bus_target.begin();
    peri_info << "Peripheral ";
    while (itr != bus_target.end ()) {
        MemLatencyInfoBase* mem_latency = itr->second->m_latency;
        if (mem_latency != NULL) {
            MemResourceInfo res_info;
            mem_latency->GetResourceInfo (&res_info, 0);
            if (res_info.cl_location == MEM_LOCATE_GLOBAL) {
                CfMsg::DPrint (MSG_ERR,
                        "  0x%08" PRIx32 " - 0x%08"
                        PRIx32 " %-3u    %-3u     %-3u       %-3u        %-3u       %-3u        %-1s    %-1s    %-3u        %-3u\n",
                        itr->second->m_btgt_start_addr, itr->second->m_btgt_end_addr,
                        res_info.pe_read_latency,
                        res_info.pe_write_latency,
                        res_info.cl_read_latency,
                        res_info.cl_write_latency,
                        res_info.exo_cl_read_latency,
                        res_info.exo_cl_write_latency,
                        "-",
                        "-",
                        res_info.num_outstanding,
                        res_info.repeat_rate);
            } else if (res_info.pe_location == MEM_LOCATE_GLOBAL) {
                CfMsg::DPrint (MSG_ERR,
                        "  0x%08" PRIx32 " - 0x%08"
                        PRIx32 " %-3u    %-3u     %-3u       %-3u        %-3u       %-3u        %-1s    %-3u  %-3u        %-3u\n",
                        itr->second->m_btgt_start_addr, itr->second->m_btgt_end_addr,
                        res_info.pe_read_latency,
                        res_info.pe_write_latency,
                        res_info.cl_read_latency,
                        res_info.cl_write_latency,
                        res_info.exo_cl_read_latency,
                        res_info.exo_cl_write_latency,
                        "-",
                        res_info.cl_location,
                        res_info.num_outstanding,
                        res_info.repeat_rate);
            } else {
                CfMsg::DPrint (MSG_ERR,
                        "  0x%08" PRIx32 " - 0x%08"
                        PRIx32 " %-3u    %-3u     %-3u       %-3u        %-3u       %-3u        %-3u  %-3u  %-3u        %-3u\n",
                        itr->second->m_btgt_start_addr, itr->second->m_btgt_end_addr,
                        res_info.pe_read_latency,
                        res_info.pe_write_latency,
                        res_info.cl_read_latency,
                        res_info.cl_write_latency,
                        res_info.exo_cl_read_latency,
                        res_info.exo_cl_write_latency,
                        res_info.pe_location,
                        res_info.cl_location,
                        res_info.num_outstanding,
                        res_info.repeat_rate);
            }
            peri_info << itr->second->m_btgt_start_addr << " " <<
                           itr->second->m_btgt_end_addr << " " <<
                           res_info.pe_read_latency << " " <<
                           res_info.pe_write_latency << " " <<
                           res_info.cl_read_latency << " " <<
                           res_info.cl_write_latency << " " <<
                           res_info.exo_cl_read_latency << " " <<
                           res_info.exo_cl_write_latency << " " <<
                           res_info.pe_location << " " <<
                           res_info.cl_location << " " <<
                           res_info.num_outstanding << " " <<
                           res_info.repeat_rate << " | ";
        } else {
            CfMsg::DPrint (MSG_ERR, "  0x%08" PRIx32 " - 0x%08" PRIx32 "\n",
                           itr->second->m_btgt_start_addr, itr->second->m_btgt_end_addr);
            peri_info << itr->second->m_btgt_start_addr << " " << itr->second->m_btgt_end_addr << " | ";
        }
        ++ itr;
    }
    return peri_info.str();
}


//! \sa ProcElement::GetGrfPtr
//! \sa ProcElement::GetSrfPtr
bool ForestUtil::RegWriteDebug (uint32_t peid, CReg regid,
                                  RegData64 data, RegData64 data_u64)
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    RegData data32 = static_cast<RegData>(data & 0xFFFFFFFFU);
    if (regid == PC) {
        m_forest->GetPePtr(peid)->GetGrfPtr(NC)->WritePC (data32 & 0xFFFFFFFEU);
        m_forest->GetPePtr(peid)->GetHtPtr()->UpdateNextInstAddr ();
        return true;
    } else if (regid >= R0 && regid <= R31) {
        m_forest->GetPePtr(peid)->GetGrfPtr(NC)->Write (regid, data32);
        return true;
    } else if (regid >= WR0 && regid <= WR31) {
        uint32_t wregid = regid - WR0;
        m_forest->GetPePtr(peid)->GetGrfPtr(NC)->WriteWR (wregid, 0, static_cast<uint32_t>(data & 0xFFFFFFFFU));
        m_forest->GetPePtr(peid)->GetGrfPtr(NC)->WriteWR (wregid, 1, static_cast<uint32_t>((data >> 32) & 0xFFFFFFFFU));
        m_forest->GetPePtr(peid)->GetGrfPtr(NC)->WriteWR (wregid, 2, static_cast<uint32_t>(data_u64 & 0xFFFFFFFFU));
        m_forest->GetPePtr(peid)->GetGrfPtr(NC)->WriteWR (wregid, 3, static_cast<uint32_t>((data_u64 >> 32) & 0xFFFFFFFFU));
        return true;
    } else if (RegID::IsSReg(regid) == true) {
        CfSReg cfsrid = RegID::GetCfSRegId (regid);
        if (m_forest->GetPePtr(peid)->GetNcSrfPtr()->HasSReg (cfsrid) == true) {
            m_forest->GetPePtr(peid)->GetNcSrfPtr()->GetSrPtr(cfsrid)->WriteBody (data32);

            // PMCTRLn and TSCTRL has special behavior.
            switch (cfsrid) {
            case CF_SR_TSCTRL:
            case CF_SR_PMCTRL0_G4X1X:
            case CF_SR_PMCTRL1_G4X1X:
            case CF_SR_PMCTRL2_G4X1X:
            case CF_SR_PMCTRL3_G4X1X:
            case CF_SR_PMCTRL0_G4MH20:
            case CF_SR_PMCTRL1_G4MH20:
            case CF_SR_PMCTRL2_G4MH20:
            case CF_SR_PMCTRL3_G4MH20:
            case CF_SR_PMCTRL4_G4MH20:
            case CF_SR_PMCTRL5_G4MH20:
            case CF_SR_PMCTRL6_G4MH20:
            case CF_SR_PMCTRL7_G4MH20:
                m_forest->GetPePtr(peid)->GetHtPtr()->UpdateCounters ();
                break;
            default:
                break;
            }
            return true;
        }
    }

    // 対象レジスタが見つからないのでエラー
    return false;
}


//! \sa ProcElement::GetGrfPtr
//! \sa ProcElement::GetSrfPtr
bool ForestUtil::RegReadDebug (uint32_t peid, CReg regid,
                                 RegData64* data, RegData64* data_u64)
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    if (regid == PC) {
        *data = m_forest->GetPePtr(peid)->GetGrfPtr(NC)->ReadPC ();
        return true;
    } else if (regid >= R0 && regid <= R31) {
        *data = m_forest->GetPePtr(peid)->GetGrfPtr(NC)->Read (regid);
        return true;
    } else if (regid >= WR0 && regid <= WR31) {
        uint32_t wregid = regid - WR0;
        uint64_t data_w0 = m_forest->GetPePtr(peid)->GetGrfPtr(NC)->ReadWR (wregid, 0);
        uint64_t data_w1 = m_forest->GetPePtr(peid)->GetGrfPtr(NC)->ReadWR (wregid, 1);
        uint64_t data_w2 = m_forest->GetPePtr(peid)->GetGrfPtr(NC)->ReadWR (wregid, 2);
        uint64_t data_w3 = m_forest->GetPePtr(peid)->GetGrfPtr(NC)->ReadWR (wregid, 3);

        *data = (data_w1 << 32) | data_w0;
        if (data_u64 != NULL) {
            *data_u64 = (data_w3 << 32) | data_w2;
        } else {
            CfMsg::DPrint (MSG_ERR, "<Error detected: Can't read upper 64bit of WR%u.>\n", wregid);
        }
        return true;
    } else if (RegID::IsSReg(regid) == true) {
        CfSReg cfsrid = RegID::GetCfSRegId (regid);
        if (m_forest->GetPePtr(peid)->GetNcSrfPtr()->HasSReg (cfsrid) == true) {
            *data = m_forest->GetPePtr(peid)->GetNcSrfPtr()->GetSrPtr(cfsrid)->ReadBody ();
            return true;
        }
    }

    // 対象レジスタが見つからないのでエラー
    return false;
}


//! \sa FsCtrl::MemWriteDebug
void ForestUtil::VMemWriteDebug (uint32_t peid, PhysAddr addr,
                                 uint32_t data, uint32_t bytes)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    m_dbg_api_peid = peid; // Record PEID for barrier peripheral which has self area in it.

    m_forest->GetPePtr(peid)->GetHtPtr ()->MemWriteDebug (addr, data, bytes);

    m_dbg_api_peid = CF_INVALID_PEID; // Initialize PEID for other debug access API.
}


//! \sa FsCtrl::MemReadDebug
uint32_t ForestUtil::VMemReadDebug (uint32_t peid, PhysAddr addr,
                                    uint32_t bytes)
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    m_dbg_api_peid = peid; // Record PEID for barrier peripheral which has self area in it.

    uint32_t ret = m_forest->GetPePtr(peid)->GetHtPtr ()->MemReadDebug (addr, bytes);

    m_dbg_api_peid = CF_INVALID_PEID; // Initialize PEID for other debug access API.

    return ret;
}


void ForestUtil::BusWriteDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes)
{
    m_forest->fsbus->BusWritePhysDebug (paddr, data, bytes);
}


void ForestUtil::BusReadDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes)
{
    m_forest->fsbus->BusReadPhysDebug (paddr, data, bytes);
}


//! \sa ProcElement::SetExtPin
void ForestUtil::SetExtPin (uint32_t peid, ExtPinType pin, uint32_t data)
{
    if (IsValidPeId (peid) == false) {
        return;
    }
    m_forest->GetPePtr(peid)->SetExtPin (pin, data);
}


//! \sa ProcElement::GetExtPin
uint32_t ForestUtil::GetExtPin (uint32_t peid, ExtPinType pin) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_forest->GetPePtr(peid)->GetExtPin (pin);
}


uint32_t ForestUtil::GetPeNum (void) const
{
    uint32_t num = 0;
    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            ++ num;
        }
        ++ it;
    }
    return num;
}


//! \sa ProcElement::GetPeLastId
uint32_t ForestUtil::GetPeLastId (void) const
{
    if (m_forest->m_pe.empty() == false) {
        return m_forest->m_pe.back()->GetPeId ();
    } else {
        return 0;
    }
}


//! \sa ProcElement::GetPeMasterId
uint32_t ForestUtil::GetPeMasterId (void) const
{
    uint32_t peid = 0;

    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            peid = (*it)->GetPeId ();
            break;
        }
        ++ it;
    }
    return peid;
}


uint32_t ForestUtil::GetPeidFromBmid (uint32_t bmid) const
{
    if (IsValidBmId (bmid) == false) {
        return CF_INVALID_ID;
    }
    return m_internals->GetPeidFromBmid (bmid);
}


uint32_t ForestUtil::GetBmidFromPeid (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return CF_INVALID_ID;
    }
    return m_internals->GetBmidFromPeid (peid);
}


uint32_t ForestUtil::GetClidFromPeid (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return CF_INVALID_ID;
    }
    return m_internals->GetClIdFromPeId (peid);
}


uint32_t ForestUtil::GetClidFromBmid (uint32_t bmid) const
{
    if (IsValidBmId (bmid) == false) {
        return CF_INVALID_ID;
    }
    uint32_t peid = m_internals->GetPeidFromBmid (bmid);
    return m_internals->GetClIdFromPeId (peid);
}


//! \sa ProcElement::GetVcLastId
int32_t ForestUtil::GetVcLastId (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return NC;
    }

    return NC;
}


//! \sa ProcElement::GetTcLastId
int32_t ForestUtil::GetTcLastId (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return NC;
    }

    return NC;
}


int32_t ForestUtil::GetScNum (void) const
{
    return static_cast<int32_t> (m_cedar.size ());
}


CoreType ForestUtil::GetCoreType (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return CORE_G4MH;
    }

    return m_forest->GetPePtr(peid)->GetCoreType ();
}


CoreVersion ForestUtil::GetCoreVersion (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return CORE_VER_10;
    }

    return m_forest->GetPePtr(peid)->GetCoreVersion ();
}


//! \sa FsCtrl::GetPeId
uint32_t ForestUtil::GetPeId (uint32_t scid) const
{
    if (IsValidScId (scid) == false) {
        return API_ERR_VAL;
    }

    return m_cedar[scid]->GetPeId ();
}


//! \sa ProcElement::GetHtPtr
//! \sa FsCtrl::GetScId
int32_t ForestUtil::GetScId (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_forest->GetPePtr (peid)->GetHtPtr ()->GetScId ();
}


uint32_t ForestUtil::GetIcacheWayNum (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    CacheWayType way_type = m_internals->GetIcacheWayType (peid);

    uint32_t way;

    // wayタイプの読み込み
    switch (way_type) {
    case WAY_PBS: way = 4; break;
    case WAY4:    way = 4; break;
    case WAY2:    way = 2; break;
    case WAY1:    way = 1; break;
    default:      way = 0; break;
    }

    return way;
}


void ForestUtil::SetIcacheWayNum (uint32_t peid, uint32_t way)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

#if defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    if ((way != 0) && (way != 4)) {
        // For SC-HEAP and CS+, only way0 and way4 are valid.
        return;
    }
#endif // #if defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)

    CacheWayType way_type;

    switch (way) {
    case 4:   way_type = WAY_PBS; break; // RTLに合わせてWAY4 PBS モードにする
    case 2:   way_type = WAY2; break;
    case 1:   way_type = WAY1; break;
    case 0:   way_type = WAY0; break;
    default:  return;
    }

    m_internals->SetIcacheWayType (peid, way_type);
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::GetIcacheSize
ByteSize ForestUtil::GetIcacheSize (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_forest->GetPePtr(peid)->GetIcPtr()->GetIcacheSize ();
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::SetIcacheSize
void ForestUtil::SetIcacheSize (uint32_t peid, ByteSize size)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

#if defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)
    if ((size != 0) && (size != 8192) && (size != 16384) && (size != 32768)) {
        // For SC-HEAP and CS+, only 0/8KB/16KB/32KB are valid.
        return;
    }
#endif // #if defined (SUPPORT_SC_HEAP) || defined (SUPPORT_CUBESUITE)

    m_forest->GetPePtr(peid)->GetIcPtr()->SetIcacheSize (size);
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::GetEccState
bool ForestUtil::GetIcacheEccState (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    return m_forest->GetPePtr(peid)->GetIcPtr()->GetEccState ();
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::SetEccState
void ForestUtil::SetIcacheEccState (uint32_t peid, bool enable)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    m_forest->GetPePtr(peid)->GetIcPtr()->SetEccState (enable);
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::GetAccessCount
uint64_t ForestUtil::GetIcacheAccessCount (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_forest->GetPePtr(peid)->GetIcPtr()->GetAccessCount ();
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::GetHitCount
uint64_t ForestUtil::GetIcacheHitCount (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_internals->GetIcacheHitCount (peid);
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::GetMissCount
uint64_t ForestUtil::GetIcacheMissCount (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_forest->GetPePtr(peid)->GetIcPtr()->GetMissCount ();
}


uint32_t ForestUtil::GetDcacheWayNum (uint32_t peid) const
{
    CacheWayType way_type = m_internals->GetDcacheWayType (peid);

    uint32_t way;

    // wayタイプの読み込み
    switch (way_type) {
    case WAY_PBS: way = 4; break;
    case WAY4:    way = 4; break;
    case WAY2:    way = 2; break;
    case WAY1:    way = 1; break;
    default:      way = 0; break;
    }

    return way;
}


void ForestUtil::SetDcacheWayNum (uint32_t peid, uint32_t way)
{
    CacheWayType way_type;

    switch (way) {
    case 4:   way_type = WAY_PBS; break; // RTLに合わせてWAY4 PBS モードにする
    case 2:   way_type = WAY2; break;
    case 1:   way_type = WAY1; break;
    case 0:   way_type = WAY0; break;
    default:  return;
    }

    m_internals->SetDcacheWayType (peid, way_type);
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::GetDcacheSize
ByteSize ForestUtil::GetDcacheSize (uint32_t peid) const
{
    return 0;
}


//! \sa ProcElement::GetIcPtr
//! \sa Dcache::SetDcacheSize
void ForestUtil::SetDcacheSize (uint32_t peid, ByteSize size)
{
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::GetEccState
bool ForestUtil::GetDcacheEccState (uint32_t peid) const
{
    return false;
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::SetEccState
void ForestUtil::SetDcacheEccState (uint32_t peid, bool enable)
{
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::GetAccessCount
uint64_t ForestUtil::GetDcacheAccessCount (uint32_t peid) const
{
    return 0;
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::GetHitCount
uint64_t ForestUtil::GetDcacheHitCount (uint32_t peid) const
{
    return 0;
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::GetMissCount
uint64_t ForestUtil::GetDcacheMissCount (uint32_t peid) const
{
    return 0;
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::GetHitCount
uint64_t ForestUtil::GetDcacheWbCount (uint32_t peid) const
{
    return 0;
}


void ForestUtil::DumpBtbStatistics (uint32_t peid) const
{
    m_forest->GetPePtr (peid)->GetHtPtr ()->DumpBtbStatistics ();
}


void ForestUtil::ResetBtbStatistics (uint32_t peid)
{
    m_forest->GetPePtr (peid)->GetHtPtr ()->ResetBtbCounter ();
}


void ForestUtil::ShowPerfInfo (uint32_t peid) const
{
    m_forest->GetPePtr (peid)->ShowPerfInfo ();
}


#ifdef CLUSTER
void ForestUtil::ShowClPerfInfo (void) const
{
    m_forest->fsbus->ShowPerfInfo ();
}


void ForestUtil::ResetClPerfInfo (void)
{
    m_forest->fsbus->ResetPerfInfo ();
}
#endif // CLUSTER


void ForestUtil::ResetPerfInfo (uint32_t peid)
{
    m_forest->GetPePtr (peid)->ResetPerfInfo ();
}


void ForestUtil::ExecCfali (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr)
{
    if (IsValidPeId (peid) == false) {
        return;
    }
    FsCtrl* ht = m_forest->GetPePtr (peid)->GetHtPtr ();
    uint32_t ic_line_size = m_forest->GetPePtr (peid)->GetIcPtr ()->GetLineByte ();
    PhysAddr ic_line_mask = ~(ic_line_size - 1);

    start_addr &= ic_line_mask;
    end_addr &= ic_line_mask;
    for (PhysAddr addr = start_addr; addr <= end_addr; addr += ic_line_size) {
        ht->ExecCfali (addr);
    }
}


#ifdef SUPPORT_SC_HEAP
void ForestUtil::AddBreakMatchPc (uint32_t peid, uint32_t addr)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->AddBreakMatchPc (addr);
}


void ForestUtil::ClearBreakMatchPc (uint32_t peid, uint32_t addr)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->ClearBreakMatchPc (addr);
}
#endif // SUPPORT_SC_HEAP


void ForestUtil::AddBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->AddBreakNextPc (addr, mask);
}


void ForestUtil::ClearBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->ClearBreakNextPc (addr, mask);
}


void ForestUtil::AddBreakAccess (uint32_t peid, PhysAddr start_addr,
                                 PhysAddr end_addr, AccessAttr attr, uint32_t mask)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->AddBreakAccess (start_addr, end_addr, attr, mask);
}


void ForestUtil::DisplayBreakAccess (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid TCID>\n");
        exit(0);
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->DisplayBreakAccess ();
}


void ForestUtil::ClearBreakAccess (uint32_t peid, PhysAddr start_addr,
                                   PhysAddr end_addr, AccessAttr attr, uint32_t mask)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->ClearBreakAccess (start_addr, end_addr, attr, mask);
}


BreakEvent ForestUtil::GetBreakEvent (void) const
{
    return m_internals->GetBreakEvent ();
}


PhysAddr ForestUtil::GetBreakPc (void) const
{
    return m_internals->GetBreakPc ();
}


void ForestUtil::DisplayBreakPc (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid TCID>\n");
        exit(0);
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr();
    tc->DisplayBreakPc ();
}


PhysAddr ForestUtil::GetAccBreakAddr (void) const
{
    return m_internals->GetAccBreakAddr ();
}


void ForestUtil::ClearBreakEvent (void)
{
    m_internals->ClearBreakEvent ();
}


bool ForestUtil::LoadSrec (const char* filename, PhysAddr offset)
{
    std::ifstream loadfs;
    loadfs.open (filename);
    if ( !loadfs ) {
        CfMsg::DPrint (MSG_ERR, "<Error: file open error: %s>\n", filename);
        return false;
    }

    std::string buff;
    while ( getline (loadfs, buff) ) {
        if (buff.size() == 0) break;   // ファイル終端
        if (buff[0] != 'S') continue;  // 行頭が S でない行は無視

        // S1BBAAAAxxxxxxxxxx....xxxxxxSS
        // S2BBAAAAAAxxxxxxxx....xxxxxxSS
        // S3BBAAAAAAAAxxxxxx....xxxxxxSS
        // S4BBAAAAA..AAAAAxx....xxxxxxSS

        // データタイプ
        uint32_t addr_length;
        switch (buff[1]) {
        case '1': addr_length =  4; break;
        case '2': addr_length =  6; break;
        case '3': addr_length =  8; break;
        case '4': addr_length = 16; break;
        default: continue;    // データレコード以外は無視
        }

        // データ数
        uint32_t byte_count;
        std::istringstream bcnt_is (buff.substr(2, 2));
        bcnt_is >> std::hex >> byte_count;

        // データアドレス
        PhysAddr write_addr;
        std::istringstream waddr_is (buff.substr(4, addr_length));
        waddr_is >> std::hex >> write_addr;
        write_addr += offset;

        // データ領域 (チェックサムは無視)
        uint32_t data_pos = addr_length + 4;
        for (uint32_t i = 0; i < byte_count - (addr_length/2) - 1; i++) {
            uint32_t write_data32;
            std::istringstream wdata_is (buff.substr(data_pos, 2));
            wdata_is >> std::hex >> write_data32;
            uint8_t write_data = static_cast<uint8_t> (write_data32);

            PhysAddr waddr = write_addr + i;
            BusWriteDebug (waddr, &write_data, SIZE_8BIT);
            data_pos += 2;
        }
    }

    loadfs.close ();
    return true;
}


std::string ForestUtil::GetVersionStr (void) const
{
    std::ostringstream os;
    os << "CForest " << SIMNAME << " " << VERSION << " (rev:" << REVISION << ")";
    return os.str();
}


void ForestUtil::PrintVersion (FILE* fp) const
{
    fprintf (fp, "%s\n", GetVersionStr ().c_str ());
    fprintf (fp, "Copyright (c) 2011-2018 Renesas Electronics Corporation.\n");
}


//! デバッグログ、メッセージの出力先を設定
//! \sa FsCtrl::SetFileOut
void ForestUtil::SetFileOut (FILE* fp)
{
    // Forestの出力先を初期化
    m_internals->SetFileOut (fp);

    // 各HTの出力先を初期化
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->SetFilePointer (fp);
        ++ it;
    }
}


FILE* ForestUtil::OpenFileOut (std::string fname)
{
    FILE* fp = m_internals->OpenFile (fname);
    if (fp == NULL) {
        return NULL;
    }

    // 以前に自分がopenしたログをclose
    // スレッド別ログはm_logsに保持されているのでここではclose不要
    if (m_global_log != stdout) {
        fclose (m_global_log);
    }
    m_global_log = fp;
    SetFileOut (fp);

    return fp;
}


bool ForestUtil::OpenSeparatedFileOut (std::string fname)
{
    // 以前に自分がopenしたログをclose
    // 非スレッド用ログはm_global_logに保持されているのでここではclose不要
    std::vector<FILE*>::const_iterator it = m_logs.begin ();
    while (it != m_logs.end ()) {
        fclose (*it);
        ++ it;
    }
    m_logs.clear();

    std::string fname_body;
    std::string fname_ext;

    if (fname.empty () == false) {
        // ファイル名（パス含む）と拡張子を分離
        std::string::size_type index = fname.rfind(".");
        if (index != std::string::npos) {
            // 拡張子を含む
            fname_body = fname.substr (0, index);
            fname_ext  = fname.substr (index);
        } else {
            // 拡張子を含まない
            fname_body = fname;
            fname_ext  = "";
        }
    }

    uint32_t sc_num = GetScNum ();
    for (uint32_t scid = 0; scid < sc_num; scid ++) {
        FILE* fp;
        uint32_t  peid = GetPeId (scid);
        if (fname.empty () == false) {
            std::ostringstream context_ss;
            context_ss << "." << "p" << peid << "nc";
            fp = m_internals->OpenFile (fname_body + context_ss.str () + fname_ext);
            if (fp == NULL) {
                SetFileOut (stdout);
                return false;
            }
            m_logs.push_back(fp);
        } else {
            fp = stdout;
        }

        m_forest->GetPePtr(peid)->GetHtPtr ()->SetFilePointer (fp);
    }
    return true;
}


int ForestUtil::GetExitStatus(void) const
{
    return m_internals->GetExitStatus ();
}


void ForestUtil::EnableIssTrace (void)
{
    m_isstrace_flag = true;

    // PE数の読み込み
    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            uint32_t peid = (*it)->GetPeId ();

            // ネイティブのポインタを取得
            FsCtrl* nc_ptr = m_forest->GetPePtr(peid)->GetHtPtr();

            // NC用のファイルポインタを生成
            std::ostringstream out_fname_nc;
            out_fname_nc << "cftrace_p" << peid << "t0.out";
            FILE* trace_file_nc = fopen (out_fname_nc.str().c_str(), "w");
            if (trace_file_nc == NULL) {
                std::cout << "file open error: " << out_fname_nc.str() << std::endl;
                exit (EXIT_NORMAL);
            }
            m_forest->GetPePtr(peid)->GetHtPtr()->InitTracePrintRTL (nc_ptr, trace_file_nc);
        }
        ++ it;
    }
}


//! \sa FsCtrl::GetScId
//! \sa ModuleRun::IsCpu
//! \sa ModuleRun::GetScId
int32_t ForestUtil::GetNextScidOnPe (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    // 実行リストから、指定PEのうち、最初に実行されるコンテキストのscidを返す
    RunList::const_iterator it = m_cedar_run.begin ();
    while (it != m_cedar_run.end ()) {
        if ((*it)->IsCpu ()) {
            FsCtrl* ht = reinterpret_cast<FsCtrl*> (*it);
            if (ht->GetPeId () == peid) {
                return ht->GetScId ();
            }
        }
        ++ it;
    }

    // ここに来たら、指定PEのコンテキストは全て実行対象外なので、指定PEのNCのscidを返す
    // - SimStateを見ているはずなので本当は何を返しても良いけど、念の為、NCに固定しておく。
    // - CubeSuiteでは次回実行コンテキストの情報取得に本APIを使用。
    //   他PEのNCを返すと、そのPEのNCが次実行対象だった場合にtraceを上書きしてしまうかもしれない。
    HtVec* htvec = m_forest->m_pe[peid]->GetHtsPtr ();
    if (htvec->empty ()) {
        return API_ERR_VAL;  // ここには来ないはず
    } else {
        return htvec->front ()->GetScId (); // 指定PEの最初のコンテキストはNC
    }
}


//! \sa ModuleRun::GetScId
int32_t ForestUtil::GetCurrentTcScId (void) const
{
    // 先頭が非CPUの場合、マスクによりNCのSCIDを返す
    return m_cedar_run.front ()->GetScId () & SCID_MASK;
}


//! \sa FsCtrl::GetTraceInfo
const TraceInfo* ForestUtil::GetTraceInfo (uint32_t scid) const
{
    if (IsValidScId (scid) == false) {
        return NULL;
    }

    return m_cedar[scid]->GetTraceInfo ();
}


//! \sa FsCtrl::GetTraceCtrl
const TraceCtrl* ForestUtil::GetTraceCtrl (uint32_t scid) const
{
    if (IsValidScId (scid) == false) {
        return NULL;
    }

    return m_cedar[scid]->GetTraceCtrl ();
}


//! CubeSuiteのAPI_E_NOT_MAPPED通知用
//! \sa CauseError
//! \sa MemBody::CheckPermitOperation
//! \sa
void ForestUtil::SetCubeErrorCode (ErrorCode err_code)
{
    m_cube_err_code = err_code;
}


//! CubeSuiteのAPI_E_NOT_MAPPED通知用
//! \sa CauseError
ErrorCode ForestUtil::GetCubeErrorCode (void)
{
    return m_cube_err_code;
}


void ForestUtil::CreateForest (void)
{
    m_forest = new Cforest ();
    m_internals->SetForestPointer (m_forest);
    m_forest->CreateCforest ();
    m_internals->SetupNotMemLatency ();
    m_internals->InitialSetup (&m_cedar);
    m_internals->UpdateRunList (); // m_cedar_runにfsbusを登録するため
}


void ForestUtil::SetPe (uint32_t clid, uint32_t peid, CoreType core_type, CoreVersion core_verson)
{
    // 引数peidはこれから確保するpeid。
    // 指定されたpeidが既に存在したら多重生成なのでエラー
    if (IsValidPeId (peid)) {
        return;
    }

    m_internals->RegClIdFromPeId (clid, peid);

    m_forest->SetPe (peid, clid, core_type, core_verson, 0, 0);
    m_internals->InitialSetup (&m_cedar); // PE生成したのでcedarベクタを再設定
}


void ForestUtil::GenPeripherals (void)
{
    m_forest->GenPeripherals ();

    HardReset ();
    m_internals->UpdateRunList ();
}


bool ForestUtil::IsValidGuardModuleId (uint32_t id) const
{
    switch (id) {
    case GMID_PEG_PEnCLm:       // PEID
    case GMID_PEGCAP_M_PEnCLm:  // PEID
    case GMID_PEGCAP_S_PEnCLm:  // PEID
    case GMID_CRGCAP_PEnCLm:    // PEID
    case GMID_MDPCAP_PEnCLm:    // PEID
    case GMID_CFGCAP_IPEnCLm:   // PEID
    case GMID_CFGCAP_DPEnCLm:   // PEID
        break;
    case GMID_CRG_CLm:          // CLID
    case GMID_CRG_IVC_CLm:      // CLID
    case GMID_CRG_CSG_CLm:      // CLID
    case GMID_CRGCAP_CRAMHCLm:  // CLID
    case GMID_CRGCAP_CRAMLCLm:  // CLID
    case GMID_CRGCAP_SX2MBHCLm: // CLID
    case GMID_CRGCAP_SX2MBLCLm: // CLID
        // allocation id is CLID
        // For CLID, any CLID is acceptable.
        // Because CL1 doesn't have any PE, there is CRAM @ CL1 on E2xFCC1.
        break;
    case GMID_CFG:              // ignored
    case GMID_CFGCAP_SX2FX:     // ignored
        // allocation id is ignored
        break;
    default:
        return false;
    }
    return true;
}


bool ForestUtil::IsValidGuardParamId (uint32_t guard_id, uint32_t param_id) const
{
    switch (guard_id) {
    case GMID_CRG_CLm:
        switch (param_id) {
        case GPID_CRG_ADDR_WIDTH:
        case GPID_CRG_CRG_CH_NUM:
        case GPID_CRG_CSG_CH_NUM:
        case GPID_CRG_KEYCODE:
            return true; // OK
        default:
            return false; // Unknown param_id
        }
    default:
        // SetGuardParam only support CRG now.
        // Because error by guard_id is detected by IsValidGuardModuleId(), it is ignored here.
        return true; // OK (Unsupported guard_id)
    }
    return true;
}


bool ForestUtil::IsValidGuardParamValue (uint32_t param_id, uint32_t value) const
{
    bool ret;
    switch (param_id) {
    case GPID_CRG_ADDR_WIDTH:
        // pMADDRW (18~21)
        switch (value) {
        case 18:
        case 19:
        case 20:
        case 21:
            ret = true;
            break;
        default:
            ret = false;
            break;
        }
        break;
    case GPID_CRG_CRG_CH_NUM:
        // pCRGNUM (1~8)
        if ((1 <= value) && (value <= 8)) {
            ret = true;
        } else {
            ret = false;
        }
        break;
    case GPID_CRG_CSG_CH_NUM:
        // pCSGNUM (1~8)
        if ((1 <= value) && (value <= 8)) {
            ret = true;
        } else {
            ret = false;
        }
        break;
    case GPID_CRG_KEYCODE:
        // pCRGKEYCODE (any 32bit value)
        ret = true;
        break;
    default:
        ret = false; // Unknown param_id
    }
    return ret;
}


bool ForestUtil::IsEnableGuard (GuardModuleID guard_id, uint32_t location_id) const
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }

    std::map<Cforest::GuardAddrKey, Cforest::GuardAddrVal>::iterator it
        = m_forest->m_guard_addr.find (Cforest::GuardAddrKey (guard_id, location_id));
    if (it != m_forest->m_guard_addr.end ()) {
        return it->second.second;
    } else {
        return false;
    }
}


PhysAddr ForestUtil::GetGuardAddr (GuardModuleID guard_id, uint32_t location_id) const
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }

    std::map<Cforest::GuardAddrKey, Cforest::GuardAddrVal>::iterator it
        = m_forest->m_guard_addr.find (Cforest::GuardAddrKey (guard_id, location_id));
    if (it != m_forest->m_guard_addr.end ()) {
        return it->second.first;
    } else {
        return 0; // error
    }
}


void ForestUtil::SetGuardAddr (GuardModuleID guard_id, uint32_t location_id, PhysAddr addr)
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }

    std::map<Cforest::GuardAddrKey, Cforest::GuardAddrVal>::iterator it
        = m_forest->m_guard_addr.find (Cforest::GuardAddrKey (guard_id, location_id));
    if (it != m_forest->m_guard_addr.end ()) {
        // overwrite
        it->second.first = addr;
    } else {
        // newly add
        m_forest->m_guard_addr.insert (std::map<Cforest::GuardAddrKey, Cforest::GuardAddrVal>::value_type (
            Cforest::GuardAddrKey (guard_id, location_id), Cforest::GuardAddrVal (addr, false))
        );
    }
}


void ForestUtil::SetGuardEnable (GuardModuleID guard_id, uint32_t location_id, bool is_enable)
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }

    std::map<Cforest::GuardAddrKey, Cforest::GuardAddrVal>::iterator it
        = m_forest->m_guard_addr.find (Cforest::GuardAddrKey (guard_id, location_id));
    if (it != m_forest->m_guard_addr.end ()) {
        // overwrite
        it->second.second = is_enable;
    } else {
        // newly add
        m_forest->m_guard_addr.insert (std::map<Cforest::GuardAddrKey, Cforest::GuardAddrVal>::value_type (
            Cforest::GuardAddrKey (guard_id, location_id), Cforest::GuardAddrVal (0, is_enable))
        );
    }
}


bool ForestUtil::SetGuard (uint32_t guard_id, uint32_t allocation_id,
                           PhysAddr addr, bool is_valid)
{
    if (IsValidGuardModuleId (guard_id) == false) {
        return false;
    }

    SetGuardAddr (static_cast<GuardModuleID>(guard_id), allocation_id, addr);
    SetGuardEnable (static_cast<GuardModuleID>(guard_id), allocation_id, is_valid);

#ifdef DEBUG_GMIDARRAY
    // Dump for debug
    printf ("Dump guard entry\n");
    printf ("GMID ID   addr      enable\n");
    std::map<Cforest::GuardAddrKey, Cforest::GuardAddrVal>::iterator it;
    for (it = m_forest->m_guard_addr.begin(); it != m_forest->m_guard_addr.end(); it++) {
        // print id
        printf ("%3u  %3u  %08x  %d\n", it->first.m_gmid, it->first.m_id, it->second.first, it->second.second); 
    }
#endif // #ifdef DEBUG_GMIDARRAY
    return true;
}


bool ForestUtil::SetGuardParam (uint32_t guard_id, uint32_t allocation_id,
                                uint32_t param_id, uint32_t value)
{
    if (IsValidGuardModuleId (guard_id) == false) {
        return false;
    }

    if (IsValidGuardParamId (guard_id, param_id) == false) {
        return false;
    }

    if (IsValidGuardParamValue (param_id, value) == false) {
        return false;
    }

    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        allocation_id = 0;
    }

    std::vector<Cforest::CrgParamPair>::iterator it_crg_param
        = std::find_if (m_forest->m_crg_param_vec.begin (), m_forest->m_crg_param_vec.end (),
                       [allocation_id](Cforest::CrgParamPair &param_pair) {
                           return (param_pair.first == allocation_id);
                       });
    if (it_crg_param != m_forest->m_crg_param_vec.end ()) {
        // Overwrite : same allocation_id (clid) is registered already.
        (*it_crg_param).second.SetParam (static_cast<GuardParamID>(param_id), value);
    } else {
        // Add new clid : no same allocation_id entry in vector.
        Cforest::CrgParamPair param = {allocation_id, Cforest::CrgParam()};
        param.second.SetParam (static_cast<GuardParamID>(param_id), value);
        m_forest->m_crg_param_vec.push_back (param);
    }

#ifdef DEBUG_GMPARAM
    // Dump for debug
    printf ("Dump guard param entry\n");
    printf ("clid  addr_width  num_crg  num_csg  keycode\n");
    std::vector<Cforest::CrgParamPair>::iterator it;
    for (it = m_forest->m_crg_param_vec.begin(); it != m_forest->m_crg_param_vec.end(); it++) {
        // print id
        printf ("%4u  %10u  %7u  %7u  %08x\n", it->first, it->second.GetAddrWidth(), it->second.GetNumOfCrg(), it->second.GetNumOfCsg(), it->second.GetKeycode());
    }
#endif // #ifdef DEBUG_GMPARAM
    return true;
}


bool ForestUtil::GetGuardParam (uint32_t guard_id, uint32_t allocation_id,
                                uint32_t param_id, uint32_t* value) const
{
    if (IsValidGuardModuleId (guard_id) == false) {
        return false;
    }

    if (IsValidGuardParamId (guard_id, param_id) == false) {
        return false;
    }

    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        allocation_id = 0;
    }

    std::vector<Cforest::CrgParamPair>::iterator it_crg_param
        = std::find_if (m_forest->m_crg_param_vec.begin (), m_forest->m_crg_param_vec.end (),
                       [allocation_id](Cforest::CrgParamPair &param_pair) {
                           return (param_pair.first == allocation_id);
                       });
    if (it_crg_param != m_forest->m_crg_param_vec.end ()) {
        // Found entry : same allocation_id (clid) is registered already.
        *value = (*it_crg_param).second.GetParam (static_cast<GuardParamID>(param_id));
        return true;
    } else {
        // No entry: Return default value
        Cforest::CrgParam init_crgparam = Cforest::CrgParam ();
        *value = init_crgparam.GetParam (static_cast<GuardParamID>(param_id));
        return true;
    }
}


bool ForestUtil::IsValidPeipheralId (SetIORBaseAddrID id) const
{
    if ((id < BaseAddr_INTC1) || (BaseAddr_NUM <= id)) {
        // Out of range
        return false;
    } else {
        return true;
    }
}


bool ForestUtil::SetPeripheral (uint32_t id, PhysAddr base_addr, bool is_valid)
{
    SetIORBaseAddrID peri_id = static_cast<SetIORBaseAddrID> (id);
    if (IsValidPeipheralId (peri_id) == false) {
        // Out of range
        return false;
    }

    EnablePeripheral (static_cast<SetIORBaseAddrID> (id), is_valid);
    SetPeripheralBaseAddr (static_cast<SetIORBaseAddrID> (id), base_addr);
    return true;
}


void ForestUtil::SetPeripheralBaseAddr (SetIORBaseAddrID peri_id,
                                        PhysAddr base_addr)
{
    if (IsValidPeipheralId (peri_id) == false) {
        // Out of range
        return;
    }
    m_forest->m_peri_addr[peri_id].m_base_addr = base_addr;
}


PhysAddr ForestUtil::GetPeripheralBaseAddr (SetIORBaseAddrID peri_id) const
{
    if (IsValidPeipheralId (peri_id) == false) {
        // Out of range
        return INVALID_PADDR;
    }
    return m_forest->m_peri_addr[peri_id].m_base_addr;
}


void ForestUtil::SetPeripheralOffsetAddr (SetIORBaseAddrID peri_id,
                                          PhysOffset offset_addr)
{
    if (IsValidPeipheralId (peri_id) == false) {
        // Out of range
        return;
    }
    m_forest->m_peri_addr[peri_id].m_offset_addr = offset_addr;
}


PhysOffset ForestUtil::GetPeripheralOffsetAddr (SetIORBaseAddrID peri_id) const
{
    if (IsValidPeipheralId (peri_id) == false) {
        // Out of range
        return 0;
    }
    return m_forest->m_peri_addr[peri_id].m_offset_addr;
}


void ForestUtil::EnablePeripheral (SetIORBaseAddrID peri_id, bool is_enabled)
{
    if (IsValidPeipheralId (peri_id) == false) {
        // Out of range
        return;
    }

    m_forest->m_peri_addr[peri_id].m_is_enabled = is_enabled;
}


bool ForestUtil::IsPeripheralEnabled (SetIORBaseAddrID peri_id) const
{
    if (IsValidPeipheralId (peri_id) == false) {
        // Out of range
        return false;
    }
    return m_forest->m_peri_addr[peri_id].m_is_enabled;
}


void ForestUtil::SetTcDebugMode (uint32_t peid, DebugMode mode)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr ();
    tc->EnableDebugMode (mode);
}


void ForestUtil::SetDebugMode (DebugMode mode)
{
    m_debug_mode = mode;

    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->EnableDebugMode (mode);
        ++ it;
    }

    if ((mode & DBG_TIME) != DBG_OFF) {
        // 現在、DBG_ON以外の属性は一度有効になったら有効なまま
        m_internals->SetDispSystime (true);
    }
}


void ForestUtil::DisableDebugOption (std::string option)
{
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->DisableDebugOption (option);
        ++ it;
    }
}


void ForestUtil::UpdateExpMsgRange (uint32_t peid,
                                    ExpCause start_cause, ExpCause end_cause, bool is_print)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    m_forest->GetPePtr(peid)->GetHtPtr()->
        m_exp_msg->UpdateExpMsgRange (peid, NC, start_cause, end_cause, is_print);
}


//! \sa FsCtrl::SetBreakMax
void ForestUtil::SetBreakMax (uint64_t max_count)
{
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->SetBreakMax (max_count);
        ++ it;
    }
}


//! \sa FsCtrl::GetBreakMax
uint64_t ForestUtil::GetBreakMax ()
{
    HtVec::const_iterator it = m_cedar.begin ();

    return (*it)->GetBreakMax ();
}


//! \sa FsCtrl::SetBreakSamePcMax
void ForestUtil::SetBreakSamePcMax (uint64_t max_count)
{
#ifdef ENABLE_SAME_PC_BREAK
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->SetBreakSamePcMax (max_count);
        ++ it;
    }
#else // ENABLE_SAME_PC_BREAK
    CfMsg::DPrint (MSG_ERR, "Break by executing same pc is disabled in this CForest\n");
#endif // ENABLE_SAME_PC_BREAK
}


//! \sa FsCtrl::GetBreakSamePcMax
uint64_t ForestUtil::GetBreakSamePcMax () const
{
    HtVec::const_iterator it = m_cedar.begin ();

    return (*it)->GetBreakSamePcMax ();
}


//! \sa FsCtrl::SetBreakSystime
void ForestUtil::SetBreakSystime (uint64_t cycle_count)
{
    m_internals->SetBreakSystime (cycle_count);
}


//! \sa FsCtrl::GetBreakSystime
uint64_t ForestUtil::GetBreakSystime () const
{
    return m_internals->GetBreakSystime ();
}


//! \sa FsCtrl::SetHtPowerState
void ForestUtil::SetRunTarget(uint32_t peid, bool is_run)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    // 指定されたPEに一致するHTを探し、ON/OFFに設定する
    HtVec::const_iterator it = m_forest->GetPePtr(peid)->GetHtsPtr()->begin ();
    while ( it != m_forest->GetPePtr(peid)->GetHtsPtr()->end () ) {
        (*it)->SetHtPowerState (is_run);
        ++it;
    }
}


//! \sa FsCtrl::SetHtPowerState
//! \sa FsCtrl::SetCpuRatio
void ForestUtil::SetFrequency(uint32_t peid, uint32_t ratio)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    // クロック0なら対象PEの動作を停止
    bool enable;
    if (ratio == 0) {
        // クロック0なら対象PEの動作を停止
        enable = false;
        ratio = 1;
    } else {
        enable = true;
    }

    // 指定されたPEに一致するHTを探し、ON/OFFに設定する
    HtVec::const_iterator it = m_forest->GetPePtr(peid)->GetHtsPtr()->begin ();
    while ( it != m_forest->GetPePtr(peid)->GetHtsPtr()->end () ) {
        (*it)->SetHtPowerState (enable);
        (*it)->SetCpuRatio (ratio);
        ++it;
    }
}


//! \sa FsCtrl::GetFrequency
uint32_t ForestUtil::GetFrequency(uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    HtVec::const_iterator it = m_forest->GetPePtr(peid)->GetHtsPtr()->begin ();
    return (*it)->GetCpuRatio ();
}


//! \sa FsCtrl::UpdateNextTcInfo
void ForestUtil::UpdateNextTcInfo (int32_t curr_scid, int32_t next_scid)
{
#ifdef SUPPORT_CUBESUITE
    if (IsValidScId (curr_scid) && IsValidScId (next_scid)) {
        m_cedar[curr_scid]->UpdateNextTcInfo (m_cedar[next_scid]);
    }
#endif // SUPPORT_CUBESUITE
}


CfApiRetern ForestUtil::GetStatus (uint32_t peid, NotifySimState* state) const
{
    if (IsValidPeId (peid) == false) {
        return CF_API_NG;
    }

    int32_t scid = m_forest->GetPePtr (peid)->GetHtPtr ()->GetScId ();
    *state = GetTraceCtrl (scid)->GetSimState ();

    return CF_API_OK;
}


CReg ForestUtil::RegGetRegno (uint32_t peid, std::string regname)
{
    if (IsValidPeId (peid) == false) {
        return GR_NONE;
    }

    return  m_forest->GetPePtr(peid)->GetHtPtr()->RegGetRegno (regname);
}


#ifdef ENABLE_PIC
void ForestUtil::DisplayInstructionCount (uint32_t peid, FILE* local_file)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    m_forest->GetPePtr(peid)->GetHtPtr()->DisplayInstructionCount (local_file);
}


void ForestUtil::ClearInstructionCount (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return;
    }

    m_forest->GetPePtr(peid)->GetHtPtr()->ClearInstructionCount ();
}
#endif // ENABLE_PIC


IntErrorCode ForestUtil::ReqInterruptToIntc1 (uint32_t peid, uint32_t channel)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetIntc1Ptr()->ReqIntByPeripheral (channel);
}


IntErrorCode ForestUtil::ReqInterruptToIntc2 (uint32_t channel)
{
    return m_forest->intc2->ReqIntByPeripheral (channel);
}


IntErrorCode ForestUtil::ReqFenmiToIntc1 (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetIntc1Ptr()->ReqIntByFenmi ();
}


IntErrorCode ForestUtil::ReqFeintToIntc1 (uint32_t peid,uint32_t channel)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetIntc1Ptr()->ReqIntByFeint (channel);
}

#if defined (COMP_RUNNER_G4MH)
IntErrorCode ForestUtil::ReqPseudoInterruptToIntc1 (uint32_t peid, uint32_t channel)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    
    IntErrorCode error_code = m_forest->GetPePtr(peid)->GetIntc1Ptr()->ReqIntByPeripheral (channel);
    //Fsbus is always at front position in CmpRunnerG4M20
    ModuleRun* ht = m_cedar_run.front ();
    if (ht->IsCpu()) {
        CfMsg::DPrint (MSG_ERR, "<Error: Pseudo EIINT cannot request to CPU>\n");
        return error_code;
    }
    //Make sure request from INTC1 sent to CPU when there's no masking condition
    ht->RunModule ();
    return error_code ;
}    

IntErrorCode ForestUtil::ReqPseudoFenmiToIntc1 (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    IntErrorCode error_code = m_forest->GetPePtr(peid)->GetIntc1Ptr()->ReqIntByFenmi ();
    //Fsbus is always at front position in CmpRunnerG4M20
    ModuleRun* ht = m_cedar_run.front ();
    if (ht->IsCpu()) {
        CfMsg::DPrint (MSG_ERR, "<Error: Pseudo FENMI cannot request to CPU>\n");
        return error_code;
    }
    //Make sure request from INTC1 sent to CPU when there's no masking condition
    ht->RunModule ();
    return error_code ;
}


IntErrorCode ForestUtil::ReqPseudoFeintToIntc1 (uint32_t peid,uint32_t channel)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    IntErrorCode error_code = m_forest->GetPePtr(peid)->GetIntc1Ptr()->ReqIntByFeint (channel);
    //Fsbus is always at front position in CmpRunnerG4M20
    ModuleRun* ht = m_cedar_run.front ();
    if (ht->IsCpu()) {
        CfMsg::DPrint (MSG_ERR, "<Error: Pseudo FEINT cannot request to CPU>\n");
        return error_code;
    }
    //Make sure request from INTC1 sent to CPU when there's no masking condition
    ht->RunModule ();
    return error_code ;
}
#endif


IntErrorCode ForestUtil::ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                            uint32_t priority, bool eitb,
                                            bool is_gm, uint32_t gpid, bool is_bgint)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    if (channel < TRegIntc2::N_G4MH_EIC) {
        ExpCode eiint_type;
        if (is_gm == false) {
             eiint_type = EXP_EIINT;
        } else if (is_bgint == false) {
            eiint_type = EXP_GMEIINT;
        } else {
            eiint_type = EXP_BGEIINT;
        }

        int32_t int_cause = ExpInfo::GetExpCause (eiint_type) + channel;
#if defined(COMP_RUNNER_G4MH)
        return m_forest->GetPePtr(peid)->GetHtPtr()->ReqInterrupt (priority, int_cause,
                                                                   eitb, is_gm, gpid, is_bgint, &(CIntc1::ResIntFromCpu));
#else  // defined(COMP_RUNNER_G4MH)
        return m_forest->GetPePtr(peid)->GetHtPtr()->ReqInterrupt (priority, int_cause,
                                                                   eitb, is_gm, gpid, is_bgint, NULL);
#endif // defined(COMP_RUNNER_G4MH)
    } else {
        return INT_E_CHANNEL;
    }
}

#if defined(COMP_RUNNER)
bool ForestUtil::ReqFpiToCpu (uint32_t peid)
{
    // No FPI in G4MH
    return false;
}

void ForestUtil::ReqAsyncDataBreakToCpu (uint32_t peid)
{
    m_forest->GetPePtr(peid)->GetHtPtr()->CauseAsyncDataBreak();
}


IntErrorCode ForestUtil::ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                            uint32_t priority, bool eitb,
                                            bool is_gm, uint32_t gpid, bool is_bgint,
                                            bool is_instdone)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    if (channel < (TRegIntc1::N_EIC + TRegIntc2::N_EIC)) {
        ExpCode eiint_type;
        if (is_gm == false) {
             eiint_type = EXP_EIINT;
        } else if (is_bgint == false) {
            eiint_type = EXP_GMEIINT;
        } else {
            eiint_type = EXP_BGEIINT;
        }
        int32_t int_cause = ExpInfo::GetExpCause (eiint_type) + channel;
        return m_forest->GetPePtr(peid)->GetHtPtr()->ReqInterrupt (priority, int_cause,
                                                                   eitb, is_gm, gpid, is_bgint, &(CIntc1::ResIntFromCpu), is_instdone);
    } else {
        return INT_E_CHANNEL;
    }
}


IntErrorCode ForestUtil::ReqFenmiToCpu (uint32_t peid, bool is_instdone)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqFenmi (NULL, is_instdone);
}


IntErrorCode ForestUtil::ReqFeintToCpu (uint32_t peid, bool is_instdone, uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqFeint (NULL, is_instdone, channel, is_gm, gpid, is_bgint);
}

// Request Halt/Snooze Release
IntErrorCode ForestUtil::ReqHaltSnoozeReleaseToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    m_forest->GetPePtr(peid)->GetHtPtr()->SetHaltState(false);
    m_forest->GetPePtr(peid)->GetHtPtr()->SetSnooze(false);
    return INT_E_OK;
}
#endif // defined(COMP_RUNNER)


IntErrorCode ForestUtil::ReqFenmiToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

#if defined(COMP_RUNNER_G4MH)
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqFenmi (&(CIntc1::ResFenmiFromCpu));
#else // #if defined(COMP_RUNNER_G4MH)
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqFenmi (NULL);
#endif // #if defined(COMP_RUNNER_G4MH)
}


IntErrorCode ForestUtil::ReqFeintToCpu (uint32_t peid, uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

#if defined(COMP_RUNNER_G4MH)
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqFeint (&(CIntc1::ResFeintFromCpu), channel, is_gm, gpid, is_bgint);
#else // #if defined(COMP_RUNNER_G4MH)
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqFeint (NULL, channel, is_gm, gpid, is_bgint);
#endif // #if defined(COMP_RUNNER_G4MH)
}


bool ForestUtil::HaveSyserrReq (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    if (m_forest->GetPePtr(peid)->GetHtPtr()->m_syserr_cause_op != NULL) {
        return true;
    } else {
        return false;
    }
}


bool ForestUtil::HaveDbnmiReq (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->m_req_dbnmi;
}


bool ForestUtil::HaveDbintReq (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->m_req_dbint;
}


bool ForestUtil::HaveRmintReq (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    return false;
}


bool ForestUtil::HaveFenmiReq (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    if (m_forest->GetPePtr(peid)->GetHtPtr()->m_fenmi_cause != NULL) {
        return true;
    } else {
        return false;
    }
}


bool ForestUtil::HaveFeintReq (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return false;
    }

    if (m_forest->GetPePtr(peid)->GetHtPtr()->m_feint_cause != NULL) {
        return true;
    } else {
        return false;
    }
}

// Notice Pseudo fetch SYSERR for designated (host/guest) mode
IntErrorCode ForestUtil::ReqPseudoFetchSyserrToCpu (uint32_t peid, uint32_t syserr_cause)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqPseudoFetchSyserr (syserr_cause);
}


// Cause SYSERR for current mode of PE
IntErrorCode ForestUtil::ReqSyserrToCpu (uint32_t peid, uint32_t syserr_cause)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    if ((m_forest->GetPePtr(peid)->GetCoreType () == CORE_G4MH)
     && (m_forest->GetPePtr(peid)->GetCoreVersion () >= CORE_VER_20)
     && (m_forest->GetPePtr(peid)->IsHvEnabled () == true)) {
        // Notice SYSERR to current mode
        if (m_forest->GetPePtr(peid)->IsHostMode () == true) {
            // host
            return m_forest->GetPePtr(peid)->GetHtPtr()->ReqOperandSyserr (syserr_cause, true, 0);
        } else {
            // guest
            uint32_t gpid = m_forest->GetPePtr(peid)->GetHtPtr()->m_srf->SrPSWH ()->GetGPID ();
            return m_forest->GetPePtr(peid)->GetHtPtr()->ReqOperandSyserr (syserr_cause, false, gpid);
        }
    }

    // Notice SYSERR to Host: HV is disabled or not G4MH2.x
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqOperandSyserr (syserr_cause, true, 0);
}


// Notice SYSERR for designated (host/guest) mode
IntErrorCode ForestUtil::ReqSyserrToCpu (uint32_t peid, uint32_t syserr_cause, bool is_host, uint32_t gpid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqOperandSyserr (syserr_cause, is_host, gpid);
}

#if defined(COMP_RUNNER_G4MH)
IntErrorCode ForestUtil::ReqOperandSyserrToCpu (uint32_t peid, uint32_t syserr_cause, bool is_host, uint32_t gpid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    m_forest->GetPePtr(peid)->GetHtPtr()->m_lsu->SetSyserrCause(is_host, gpid, syserr_cause);
    return INT_E_OK;
}

bool ForestUtil::GetOperandSyserrStatus(uint32_t peid, bool is_host, uint32_t gpid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->m_lsu->GetOperandSyserrStatus(is_host, gpid);
}
#endif


// Notify error response for current mode.
// When corresponding DECFG.ExE is 1, DEVDS.x will be 1
IntErrorCode ForestUtil::NoticeOperandErrorResponse (uint32_t peid, uint32_t syserr_cause)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    if ((m_forest->GetPePtr(peid)->GetCoreType () == CORE_G4MH)
     && (m_forest->GetPePtr(peid)->GetCoreVersion () >= CORE_VER_20)) {
        if (m_forest->GetPePtr(peid)->IsHvEnabled () == true) {
            // Notice SYSERR to current mode
            if (m_forest->GetPePtr(peid)->IsHostMode () == true) {
                // host
                return m_forest->GetPePtr(peid)->GetHtPtr()->NoticeOperandErrorResponse (syserr_cause, true, 0);
            } else {
                // guest
                uint32_t gpid = m_forest->GetPePtr(peid)->GetHtPtr()->m_srf->SrPSWH ()->GetGPID ();
                return m_forest->GetPePtr(peid)->GetHtPtr()->NoticeOperandErrorResponse (syserr_cause, false, gpid);
            }
        } else {
            // Notice SYSERR to Host: HV is disabled or not G4MH2.x
            return m_forest->GetPePtr(peid)->GetHtPtr()->NoticeOperandErrorResponse (syserr_cause, true, 0);
        }
    }
    return INT_E_NOT_RECEIVED;
}


// Notify error response to specific mode.
// When corresponding DECFG.ExE is 1, DEVDS.x will be 1
IntErrorCode ForestUtil::NoticeOperandErrorResponse (uint32_t peid, uint32_t syserr_cause,
                                                     bool is_gm, uint32_t gpid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    if ((m_forest->GetPePtr(peid)->GetCoreType () == CORE_G4MH)
     && (m_forest->GetPePtr(peid)->GetCoreVersion () >= CORE_VER_20)) {
        // Operand syserr is valid only on G4MH2.0
        // Notice SYSERR to the specific mode
        if (is_gm == false) {
            // host or conventional mode
            return m_forest->GetPePtr(peid)->GetHtPtr()->NoticeOperandErrorResponse (syserr_cause, true, 0);
        } else {
            // guest
            return m_forest->GetPePtr(peid)->GetHtPtr()->NoticeOperandErrorResponse (syserr_cause, false, gpid);
        }
    }
    return INT_E_NOT_RECEIVED;
}


// Cancel operand SYSERR
IntErrorCode ForestUtil::CancelSyserrToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelOperandSyserr ();
}


// Cancel operand SYSERR
IntErrorCode ForestUtil::CancelSyserrToCpu (uint32_t peid, bool is_host, uint32_t gpid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelOperandSyserr (is_host, gpid);
}


// Cancel pseudo fetch SYSERR
IntErrorCode ForestUtil::CancelPseudoFetchSyserrToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelPseudoFetchSyserr ();
}


IntErrorCode ForestUtil::CancelInterruptToCpu (uint32_t peid, uint32_t channel)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    if (channel < TRegIntc2::N_G4MH_EIC) {
        FsCtrl* ht = m_forest->GetPePtr (peid)->GetHtPtr ();
        IntErrorCode ret_ei = ht->CancelInterrupt (ExpInfo::GetExpCause (EXP_EIINT) + channel);
        IntErrorCode ret_gm = ht->CancelInterrupt (ExpInfo::GetExpCause (EXP_GMEIINT) + channel);
        IntErrorCode ret_bg = ht->CancelInterrupt (ExpInfo::GetExpCause (EXP_BGEIINT) + channel);
        if ((ret_ei == INT_E_OK) || (ret_gm == INT_E_OK) || (ret_bg == INT_E_OK)) {
            // Request was found and deleted.
            return INT_E_OK;
        } else {
            // No request
            return INT_E_NOT_RECEIVED;
        }
    } else {
        return INT_E_CHANNEL;
    }
}


IntErrorCode ForestUtil::CancelInterruptToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelInterrupt ();
}


IntErrorCode ForestUtil::CancelFenmiToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelFenmi ();
}


IntErrorCode ForestUtil::CancelFeintToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelFeint ();
}


IntErrorCode ForestUtil::ReqRlbToCpu (uint32_t peid, uint32_t source_peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqRlb (source_peid);
}


IntErrorCode ForestUtil::CancelRlbToCpu (uint32_t peid, uint32_t source_peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelRlb (source_peid);
}


IntErrorCode ForestUtil::ReqDbnmiToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqDbnmi ();
}


IntErrorCode ForestUtil::CancelDbnmiToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelDbnmi ();
}


IntErrorCode ForestUtil::ReqDbintToCpu (uint32_t peid, uint32_t channel)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqDbint (channel);
}


IntErrorCode ForestUtil::CancelDbintToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelDbint ();
}


IntErrorCode ForestUtil::ReqRmintToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqRmint ();
}


IntErrorCode ForestUtil::CancelRmintToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelRmint ();
}


IntErrorCode ForestUtil::ReqAllInterruptDisableToCpu(uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->ReqAllInterruptDisable ();
}


IntErrorCode ForestUtil::CancelAllInterruptDisableToCpu (uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return INT_E_CHANNEL;
    }
    return m_forest->GetPePtr(peid)->GetHtPtr()->CancelAllInterruptDisable ();
}


bool ForestUtil::IsValidPeId (uint32_t peid) const
{
    return m_internals->IsValidPeId (peid);
}


bool ForestUtil::IsValidBmId (uint32_t bmid) const
{
    return m_internals->IsValidBmId (bmid);
}


bool ForestUtil::IsValidScId (int32_t scid) const
{
    return (scid >= 0) && (scid < GetScNum ());
}


void ForestUtil::EnableBreakAccStopio (void)
{
#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
#ifdef USE_SIM_IO
    // Set access break for STOPIO
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->AddBreakAccess (Csimio::SIM_IO_BASE + Csimio::SIM_STOPIO,
                               Csimio::SIM_IO_BASE + Csimio::SIM_STOPIO, ACC_READWRITE, BREAK_ADDR_MASK);
        ++ it;
    }
#endif // USE_SIM_IO
#endif // SUPPORT_CUBESUITE
}


void ForestUtil::SetDcacheFillLatency (uint32_t latency)
{
    m_internals->SetDcacheFillLatency (latency);
}


void ForestUtil::SetDcacheHitLatency (uint32_t latency)
{
    m_internals->SetDcacheHitLatency (latency);
}


void ForestUtil::SetAdditonalFetchStage (uint32_t num)
{
    m_internals->SetAdditonalFetchStage (num);
}

#ifdef FROG_API_IF
bool ForestUtil::IsRomArea (PhysAddr addr)
{
    return (addr >= m_rom_start_addr) && (addr <= m_rom_end_addr);
}
#endif //FROG_API_IF

// 現状必要性がないので、他PEのROM領域をキャッシング不可にする構成はCForest非サポート
void ForestUtil::SetRomArea (PhysAddr start_addr, PhysAddr end_addr)
{
    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            uint32_t peid = (*it)->GetPeId ();
            ProcElement* pe = m_forest->GetPePtr (peid);
            pe->GetIcPtr()->SetRomAreaAddr (start_addr, end_addr);
        }
        ++ it;
    }
    m_rom_start_addr = start_addr;
    m_rom_end_addr   = end_addr;
}


void ForestUtil::SetGramArea (PhysAddr start_addr, PhysAddr end_addr)
{
    m_gram_start_addr = start_addr;
    m_gram_end_addr   = end_addr;

    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            uint32_t peid = (*it)->GetPeId ();
            m_forest->GetPePtr(peid)->SetGramArea (start_addr, end_addr);
        }
        ++ it;
    }
}


void ForestUtil::SetLocalRamArea (PhysAddr start_addr, PhysAddr end_addr)
{
    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            uint32_t peid = (*it)->GetPeId ();
            m_forest->GetPePtr(peid)->SetLramArea (start_addr, end_addr);
        }
        ++ it;
    }
}

bool ForestUtil::SetIntc1 (uint32_t peid, PhysAddr start_addr)
{
    PhysAddr end_addr = start_addr + (CIntc1::INTC1_REG_SIZE - 1);
    ProcElement* pe = m_forest->GetPePtr(peid);
    if (pe == NULL) {
        CfMsg::DPrint (MSG_ERR, "<Error: PE Absence.>\n");
        return false;
    }
    if (pe->GetIntc1Ptr() != NULL) {
        CfMsg::DPrint (MSG_ERR, "<Error: Multiple Definition.>\n");
        return false;
    }
    pe->SetIntc1 ();
    CModuleVc* modvc = pe->GetIntc1Ptr();
    CIntc2* intc2 = m_forest->intc2;
    if (intc2 != NULL) {
        pe->SetIntc2Ptr (intc2);
    }
    ModuleGuard* lapbg = GetGuardPtr (peid, GUARD_PEG);
    m_forest->fsbus->AddTargetVC (static_cast<MemLocationType>(peid),
                                  MemLocationInternal,
                                  start_addr, end_addr, modvc, NULL,
                                  lapbg,
                                  m_internals->CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
    return true;
}

bool ForestUtil::SetIntc2 (PhysAddr start_addr)
{
    return m_forest->SetIntc2 (start_addr);
}


bool ForestUtil::SetOstmLocal (uint32_t bind_pe, uint32_t ostmch, PhysAddr start_addr, uint32_t intch)
{
#ifdef ENABLE_OSTM
    PhysAddr end_addr = start_addr + OstmIO::m_map_size - 1;
    ProcElement* pe = m_forest->GetPePtr(bind_pe);
    if (pe == NULL) {
        CfMsg::DPrint (MSG_ERR, "<Error: PE Absence.>\n");
        return false;
    }
    if (ostmch >= ProcElement::MAX_OSTM_NUM) {
        CfMsg::DPrint (MSG_ERR, "<Error: ostm channel must be < %d.>\n", ProcElement::MAX_OSTM_NUM);
        return false;
    }
    if (intch > INTC_CH_MAX) {
        // Illegal cases
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid intch %u.>\n", intch);
        return false;
    }
    if (pe->GetOstmPtr(ostmch) != NULL) {
        CfMsg::DPrint (MSG_ERR, "<Error: Multiple Definition %d.>\n",ostmch);
        return false;
    }

    pe->SetOstm (ostmch, start_addr, intch, m_forest->intc2);
    CModuleVc* modvc = pe->GetOstmPtr(ostmch);
    ModuleGuard* lapbg = m_forest->m_dummy_guard;
    m_forest->fsbus->AddTargetVC (static_cast<MemLocationType>(bind_pe),
                                  MemLocationInternal,
                                  start_addr, end_addr, modvc, NULL,
                                  lapbg,
                                  m_internals->CreateDefaultLatency (bind_pe, p_internals->GetClIdFromPeId (bind_pe)));
    return true;
#else // #ifdef ENABLE_OSTM
    return false;
#endif // #ifdef ENABLE_OSTM
}


bool ForestUtil::SetOstmGlobal (PhysAddr start_addr, uint32_t intch)
{
#ifdef ENABLE_OSTM
    PhysAddr end_addr = start_addr + OstmIO::m_map_size - 1;
    if (intch > INTC_CH_MAX) {
        // Illegal cases
        CfMsg::DPrint (MSG_ERR, "<Error: Invalid intch %u.>\n", intch);
        return false;
    }

    OstmIO* ostm = new OstmIO(0, NULL, m_forest->intc2, start_addr, intch,
                                static_cast<uint32_t>(m_forest->m_ostm.size()),
                                m_forest->fsbus, GetClkRatioOstm ());
    m_forest->m_ostm.push_back (ostm);
    ostm->HardReset();
    m_forest->fsbus->AddTargetVC (static_cast<uint32_t>(MEM_LOCATE_GLOBAL),
                                  MemLocationInternal,
                                  start_addr, end_addr, ostm, NULL,
                                  m_forest->m_dummy_guard,
                                  m_internals->CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
    return true;
#else // #ifdef ENABLE_OSTM
    return false;
#endif // #ifdef ENABLE_OSTM
}


void ForestUtil::SetDCacheArea (PhysAddr start_addr1, PhysAddr end_addr1,
        PhysAddr start_addr2, PhysAddr end_addr2)
{
}


void ForestUtil::GetDCacheArea (PhysAddr* dc_area)
{
    dc_area [0] = 0;
    dc_area [1] = 0;
    dc_area [2] = 0;
    dc_area [3] = 0;
}


void ForestUtil::SetDebugArea (PhysAddr start_addr, PhysAddr end_addr, uint32_t peid)
{
    if (IsValidPeId (peid) == false) {
        return;
    }
    ModuleGuard* lapbg = m_forest->m_dummy_guard;
    m_forest->fsbus->AddTargetVC (static_cast<uint32_t>(MEM_LOCATE_GLOBAL),
                                  MemLocationInternal,
                                  start_addr, end_addr, m_forest->m_dbg_area, NULL,
                                  lapbg,
                                  m_internals->CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
}


bool ForestUtil::ClearDebugArea (PhysAddr start_addr)
{
    // 対象アドレスに配置された周辺モデルを確認
    FsBus::BusTarget* tgt = m_forest->fsbus->SearchAccessTarget (start_addr);
    if (m_forest->fsbus->IsTargetValid (tgt) == false) {
        // 対象は未確保
        return false;
    }

    // Debug領域の削除
    if (m_forest->fsbus->RemTargetVC (start_addr, m_forest->m_dbg_area, NULL)) {
        return true;
    }

    // ここまできたら削除失敗
    return false;
}


uint64_t ForestUtil::GetStepCount (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_internals->GetStepCount (peid);
}


uint64_t ForestUtil::GetCycleCount (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_internals->GetCycleCount (peid);
}


void ForestUtil::ShowReg ()
{
    std::string regname;
    CfMsg::DPrint (MSG_ERR, "<List of register name>\n");
    CfMsg::DPrint (MSG_ERR, "     Instruction address register\n");
    for (CReg i = R0; i <= R31; i++) {
        regname = RegID::GetName (i);
        CfMsg::DPrint (MSG_ERR, "        %s\n", regname.c_str ());
    }
    for (CReg i = WR0; i <= WR31; i++) {
        regname = RegID::GetName (i);
        CfMsg::DPrint (MSG_ERR, "        %s\n", regname.c_str ());
    }
    CfMsg::DPrint (MSG_ERR, "     PSW register\n");
    for (CReg i = HMPSW_Z; i <= HMPSW_NP; i++) {
        regname = RegID::GetName (i);
        CfMsg::DPrint (MSG_ERR, "        %s\n", regname.c_str ());
    }
    CfMsg::DPrint (MSG_ERR, "     PC register\n");
    regname = RegID::GetName (PC);
    CfMsg::DPrint (MSG_ERR, "        %s\n", regname.c_str ());
    CfMsg::DPrint (MSG_ERR, "     System register\n");
    for (CReg srid = CF_SR_DUMMY + 1; srid < CF_SR_GUARD; ++srid) {
        CReg sregid = RegID::GetSRegId (static_cast<CfSReg> (srid));
        regname = RegID::GetName (sregid);
        CfMsg::DPrint (MSG_ERR, "        %s\n", regname.c_str ());
    }
}


ExpCause ForestUtil::GetExpCause (uint32_t peid) const
{
    if (IsValidPeId (peid) == false) {
        return API_ERR_VAL;
    }

    return m_forest->GetPePtr(peid)->GetHtPtr()->m_trace->GetExpCause ();
}


void ForestUtil::SetFpuLatencyOffset (int32_t offset)
{
    // 全HTのFPUレイテンシを更新
    HtVec::iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->SetFpuLatencyOffset (offset);
        ++ it;
    }
}


void ForestUtil::SetDivfLatencyOffset (int32_t offset)
{
    // 全HTのFPUレイテンシを更新
    HtVec::iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->SetDivfLatencyOffset (offset);
        ++ it;
    }
}


#ifdef ENABLE_PEID_POS_CTRL
void ForestUtil::SetPeidPosG4x (bool is_g4x)
{
    m_internals->SetPeidPosG4x (is_g4x);
}
#endif // #ifdef ENABLE_PEID_POS_CTRL


void ForestUtil::SetDbufDepth (uint32_t depth)
{
    m_internals->SetDbufDepth (depth);
}


void ForestUtil::SetDbufLineShift (uint32_t shift)
{
    m_internals->SetDbufLineShift (shift);
}


void ForestUtil::SetDbufFillBurstNum (uint32_t num)
{
    m_internals->SetDbufFillBurstNum (num);
}


void ForestUtil::SetDbufPreloadType (DbufPreloadType type)
{
    m_internals->SetDbufPreloadType (type);
}


#ifdef USE_IC_LINE_SIZE_CMDOPT
void ForestUtil::SetIcLineByte (uint32_t size)
{
    m_internals->SetIcLineByte (size);
}
#endif //#ifdef USE_IC_LINE_SIZE_CMDOPT

void ForestUtil::SetFlashWidthByte (uint32_t size)
{
    m_internals->SetFlashWidthByte (size);
}


void ForestUtil::SetRasNum (uint32_t num)
{
    m_internals->SetRasNum (num);
}


void ForestUtil::SetTso (bool is_tso)
{
    m_internals->SetTso (is_tso);
}


#ifdef ENABLE_EXE_ORDER_CTRL
void ForestUtil::SetExecOrderOoOG4MH (bool is_ooo)
{
    m_internals->SetExecOrderOoOG4MH (is_ooo);
}
#endif // #ifdef ENABLE_EXE_ORDER_CTRL


void ForestUtil::SetClkRatioOstm (uint32_t ratio)
{
#ifdef ENABLE_OSTM
    m_ostm_clk_ratio = ratio;

    // CPU local OSTM
    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            for (uint32_t i = 0; i < ProcElement::MAX_OSTM_NUM; ++i) {
                OstmIO* ostm = (*it)->GetOstmPtr (i);
                if (ostm != NULL) {
                    ostm->SetClkRatio (ratio);
                }
            }
        }
        ++ it;
    }

    // Global OSTM
    std::vector<OstmIO*>::const_iterator it_ostm = m_forest->m_ostm.begin ();
    while (it_ostm != m_forest->m_ostm.end()) {
        (*it_ostm)->SetClkRatio (ratio);
        ++ it_ostm;
    }
#endif // #ifdef ENABLE_OSTM
}

//! \sa Set eiint channel offset
void ForestUtil::SetEiintChannelOffset (uint32_t offset)
{
    m_eiint_channel_offset = offset;
}

#ifdef FROG_API_IF
void ForestUtil::DeleteLLBit (int32_t scid, PhysAddr paddr)
{
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        if ((*it)->GetScId() == scid) {
            (*it)->DeleteLLBit(paddr);
            break;
        } else {
            ++ it;
        }
    }
}

void ForestUtil::CreateLLBit (int32_t scid, PhysAddr paddr, ByteSize size)
{
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        if ((*it)->GetScId() == scid) {
            (*it)->CreateLLBit(scid, paddr, size);
            break;
        } else {
            ++ it;
        }
    }
}
#endif //FROG_API_IF


bool ForestUtil::CreateUart (void)
{
    if (m_is_uart_instance == true) {
        // UART is created in GenPeripherals
        return false;
    }

#ifdef ENABLE_UART
    PhysAddr uart_base_addr = GetPeripheralBaseAddr (BaseAddr_UART);
    m_forest->uart = new Uart(m_forest->fsbus, m_forest->intc2, uart_base_addr);
    PhysAddr uart_addr = m_forest->m_peri_addr[BaseAddr_UART].GetAddr (0);
    m_forest->fsbus->AddTargetVC (uart_addr, uart_addr + (Uart::UART_AREA_SIZE - 1),
                        m_forest->uart, NULL, m_forest->m_dummy_guard,
                        m_forest->CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));

    return true;
#else  // ENABLE_UART
    return false;
#endif // ENABLE_UART
}


ForestUtil::ForestUtil ()
{
    p_forest_util = this;
    m_forest = NULL;
    m_memini_value = 0xFF;
    m_isstrace_flag = false;
    m_is_break_halt_enabled = true;
    m_rom_wait = INIT_ROM_WAIT;
    m_global_log = stdout;
    m_is_uart_instance = false;
    m_uart_channel_num = 0;
    m_uart_socket_port = UART_PORT_ID_SOCKET;
    m_debug_mode = DBG_OFF;
    m_show_staticstics = INIT_SHOW_STATISTICS;
    m_ostm_clk_ratio = INIT_OSTM_RATIO;
    m_dbuf_hit_latency = DBUF_HIT_LATENCY;

    m_work_perfinfo_with_perfcnt = false;

    m_cube_err_code = E_OK;

    m_rom_start_addr = Lsu::ROM_BGN;
    m_rom_end_addr   = Lsu::ROM_END;

    m_gram_start_addr = GLOBAL_RAM_BGN;
    m_gram_end_addr   = GLOBAL_RAM_END;

    m_dbg_api_peid = CF_INVALID_PEID;
    m_eiint_channel_offset = INTC_EIINT_CHANNEL_OFFSET;

#ifdef CLUSTER
    m_is_en_bus_trace = INIT_EN_BUS_TRACE;
#endif // #ifdef CLUSTER

    m_internals = new ForestInternals (this);
    m_internals->SetCedarRunPointer (&m_cedar_run);
}


ForestUtil::~ForestUtil ()
{
    if (m_isstrace_flag == true) {
        m_internals->CloseIssTraceFile ();
    }

    delete m_forest;
    delete m_internals;

    if (m_global_log != stdout) {
        fclose (m_global_log);
    }

    std::vector<FILE*>::const_iterator it = m_logs.begin ();
    while (it != m_logs.end ()) {
        fclose (*it);
        ++ it;
    }
    m_logs.clear();
}

// add Yoshinaga for SC-HEAP
FsBus* ForestUtil::GetFsbusPointer(void) {
    return m_forest->fsbus;
}


template int32_t ForestUtil::StepExecute<RUN_MODE> (int32_t scid, uint64_t step_goal);
template int32_t ForestUtil::StepExecute<STEP_MODE> (int32_t scid, uint64_t step_goal);
template int32_t ForestUtil::StepExecute<STEP_ANY_MODE> (int32_t scid, uint64_t step_goal);
