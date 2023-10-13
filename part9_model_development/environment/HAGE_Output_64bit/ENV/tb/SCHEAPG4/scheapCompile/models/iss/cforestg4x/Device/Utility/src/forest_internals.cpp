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
#include <string>
#include <vector>
#include <iostream>
#include "./config.h"
#include "./forest_internals.h"
#include "./forest.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./fs_controler.h"
#include "./cs_controler.h"
#include "./fsbus.h"
#include "./regid.h"
#include "./proc_element.h"
#include "./icache.h"
#include "./memory_body.h"
#ifdef SUPPORT_CUBESUITE
#include "./cube_callback.h"
#endif // #ifdef SUPPORT_CUBESUITE
#include "./outstand_mem.h"
#include "./dbuf.h"
#include "./trace_info.h"
#include "./smem.h"
#include "./dbuf.h"
#include "./llsc.h"
#include "./intc2.h"

#ifdef SUPPORT_SC_HEAP
#include "./trace_buf.h"
#endif // SUPPORT_SC_HEAP

ForestInternals::ForestInternals (ForestUtil* forest_util)
    : m_forest_util (forest_util)
{
    p_internals = this;
    m_outfp = stdout;
    m_message_level = MSG_ERR;
    m_is_all_in_halt = false;
    m_srec_warn_flag = false;
#ifdef USE_SIM_IO
    m_exit_status = EXIT_NOTSET;
#else // USE_SIM_IO
    // RESULTレジスタが存在しないので、初期値をOKにする
    m_exit_status = EXIT_PASS;
#endif //USE_SIM_IO

    m_timeout_cycle = 0;
    m_break_event = BREAK_NONE;
    m_break_guard_err = GD_NONE_ERR;
    m_break_pc    = 0xFFFFFFFF;
    m_break_acc_addr = 0xFFFFFFFF;
    m_break_acc_size = 0x0;
    memset (&m_break_info, 0x00, sizeof(m_break_info));

    m_is_disp_systime = false;
    m_not_mem_latency = NULL;

    m_dbuf_depth = INIT_DBUF_DEPTH;
    m_dbuf_line_shift = INIT_DBUF_ADDR_SHIFT;
    m_dbuf_fill_num = INIT_DBUF_FILL_BURST_SIZE;
    m_dbuf_preload_type = INIT_DBUF_PRELOAD_TYPE;
#ifdef USE_IC_LINE_SIZE_CMDOPT
    m_ic_line_byte = INIT_IC_LINE_BYTES;
#endif //#ifdef USE_IC_LINE_SIZE_CMDOPT
    m_flash_witdh = INIT_FLASH_WIDTH;
    m_ras_num = INIT_RAS_NUM;
    m_is_tso = INIT_TSO_MODE;
#ifdef ENABLE_EXE_ORDER_CTRL
    m_is_ooo_g4mh = INIT_OOO_MODE;
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    m_dc_fill_latency = INIT_DC_FILL_LATENCY;
    m_dc_hit_latency = INIT_DC_HIT_LATENCY;
    m_additonal_fetch_stage = INIT_ADD_FETCH_STAGE;

#ifdef ENABLE_PEID_POS_CTRL
    m_is_peid_g4x = true;
#endif // #ifdef ENABLE_PEID_POS_CTRL

    m_break_systime = INIT_BREAK_SYSTIME;
    m_forest = NULL;

#ifdef SUPPORT_SC_HEAP
    m_trace_buf = new TraceBuf (INIT_TRACE_SIZE);
#endif // SUPPORT_SC_HEAP
}


ForestInternals::~ForestInternals ()
{
    delete m_not_mem_latency;
#ifdef SUPPORT_SC_HEAP
    delete m_trace_buf;
#endif // SUPPORT_SC_HEAP
}


void ForestInternals::SetupNotMemLatency (void)
{
    m_not_mem_latency = CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL);
}


void ForestInternals::UpdatePeidArray (void)
{
    uint32_t max_bmid = 0;
    uint32_t valid_pe = false;
    PeVec::const_iterator it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            max_bmid = std::max ((*it)->GetBmId (), max_bmid);
            valid_pe = true;
        }
        ++ it;
    }

    m_peid_vec.clear ();

    if (valid_pe == false) {
        return;
    }

    // Allocate BMID length
    m_peid_vec.assign (max_bmid + 1, CF_INVALID_ID);

    // Assign peid to bmid pos
    it = m_forest->m_pe.begin ();
    while (it != m_forest->m_pe.end ()) {
        if ((*it) != NULL) {
            uint32_t bmid = (*it)->GetBmId ();
            uint32_t peid = (*it)->GetPeId ();

            if (m_peid_vec[bmid] != CF_INVALID_ID) {
                // Duplicated BMID
                CfMsg::DPrint (MSG_ERR,
                               "<Error: Duplicated BMID=%u was found on PEID=%u and PEID=%u>\n",
                               bmid, m_peid_vec[bmid], peid);
            }

            m_peid_vec[bmid] = peid;
        }
        ++ it;
    }
}


MemLatencyInfoBase* ForestInternals::CreateDefaultLatency (MemLocationType pe_location, MemLocationType cl_location)
{
    return new MemLatencyInfo<DBUF_SIMPLE_MEM> (10, 10, 10, 10, 10, 10, 10, 10, 10, pe_location, cl_location, 1, 2, MEM_RES_OTHERS, "peripheral");
}


uint64_t ForestInternals::GetSystemTime (void) const
{
    // FSBUSの時間をsystem_timeとして使用する
    return m_forest->fsbus->GetCpuTime ();
}

void ForestInternals::UpdateAllHaltState (void)
{
    if (m_forest_util->IsEnabledHaltBreak () == false) {
        // halt_break無効
        m_is_all_in_halt = false;
        return;
    }

    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        if ((*it)->IsInHalt () == false &&
            (*it)->IsHtActive () == true ) {
            m_is_all_in_halt = false;
            return;
        }
        ++ it;
    }
    m_is_all_in_halt = true;
}


bool ForestInternals::IsHitBreak (void) const
{
    if (unlikely (IsAllInHalt () == true) ) {
        // 全CPUがHaltまたはMSTP状態ならブレーク
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
        CfMsg::DPrint (MSG_ERR, "<All contexts are in HALT>\n");
#endif // SUPPORT_SC_HEAP
        return true;
    } else if (unlikely (ForestUtil::m_stop_req == true) ) {
        // signalブレーク
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
        CfMsg::DPrint (MSG_ERR, "<Ctrl-C Break>\n");
#endif // SUPPORT_SC_HEAP
        return true;
    } else if (unlikely (m_break_systime <= GetSystemTime ())) {
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
        CfMsg::DPrint (MSG_ERR, "<System time Break>\n");
#endif // SUPPORT_SC_HEAP
        return true;
    }

    return false;
}


void ForestInternals::AddRunTarget (ModuleRun* mod)
{
    // 多重登録の防止
    std::vector<ModuleRun*>::iterator it =
        std::find (m_cedar_run->begin (), m_cedar_run->end (), mod);
    if (it != m_cedar_run->end ()) {
        return; // 登録済み
    }

    // cpu_timeを現時間に合わせる
    uint64_t my_time = mod->GetCpuTime ();
    uint64_t target_time;
    if (m_cedar_run->empty ()) {
        target_time = GetSystemTime ();
    } else {
        target_time = m_cedar_run->front ()->GetCpuTime ();
    }
    if (target_time > my_time) {
        // 追加するmodのcpu_timeを現在時間に同期させる。
        uint64_t halt_period = target_time - my_time;
        uint32_t clk_ratio = mod->GetCpuRatio ();
        lldiv_t result = lldiv(halt_period, clk_ratio);
        uint64_t halt_period_clkbase = result.quot;
        if (result.rem != 0) {
            ++ halt_period_clkbase;
        }
        mod->ClockWait (halt_period_clkbase);
        mod->AddHaltCycle (halt_period_clkbase);
    }

    my_time = std::max(mod->GetCpuTime (), mod->GetBusTime ());
    it = m_cedar_run->begin ();
    while (it != m_cedar_run->end ()) {
        // 各マスタの時間順になるように、挿入位置を探索。
        // 同じ時間なら既存エントリの後に挿入。ただし、挿入位置がFSBUSなら前に挿入。
        // - SystemTime <= cputimeを保証するため、同じ時間内では常にFsBusを最終要素にするため。
        // - ここには滅多に来ない＆不具合を出したので、多少冗長でもわかりやすい判定にしておく。
        bool is_fsbus = (*it)->GetScId () == FsBus::FSBUS_SCID;
        uint64_t pos_time = std::max((*it)->GetCpuTime (), (*it)->GetBusTime ());
        if (  ((   is_fsbus) && (pos_time >= my_time))
           || (( ! is_fsbus) && (pos_time >  my_time)) ) {
            // 挿入位置を発見
            m_cedar_run->insert (it, mod);
            return;
        }
        ++ it;
    }

    // 見つからなかったということは、追加要素の時間>既存要素なので、最後に追加
    m_cedar_run->push_back (mod);
}


void ForestInternals::RemoveRunTarget (ModuleRun* mod)
{
    std::vector<ModuleRun*>::iterator it =
        std::find (m_cedar_run->begin (), m_cedar_run->end (), mod);
    if (it != m_cedar_run->end ()) {
        m_cedar_run->erase (it);
    }
}


void ForestInternals::bubble_sort(RunList::iterator begin, RunList::iterator end)
{
	bool done = false;
	while (begin != end && !done) {
		done = true;
		RunList::iterator it = begin;
		RunList::iterator next = begin;
		++next;
		while (next != end) {
			uint64_t next_time = std::max((*next)->GetCpuTime (), (*next)->GetBusTime ());
			uint64_t curr_time = std::max((*it)->GetCpuTime (), (*it)->GetBusTime ());
			if (next_time < curr_time) {
				std::swap(*next, *it);
				done = false;
			}
			++it;
			++next;
		}
		--end;
	}
}


//! 先頭HTのcpu_timeが更新された時のcpu_timeによるrunlistのソート。
void ForestInternals::UpdateHeadHtOrder(ModuleRun* ht)
{
    if (unlikely(ht != m_cedar_run->front ())) {
        // 新規スレッドの起動時、自スレッドの停止時など、先頭HTが自分以外
        UpdateHtOrder (ht);
        return;
    }

#if defined(COMP_RUNNER_G4MH)
    // In CompRunnerG4MH, Intc1 is included in verification
    // Interrupt from Intc1-SV side can send to CPU at any cycle
    // Therefore, Fsbus is required to excute at each commit stepexecute
	RunList::iterator it = m_cedar_run->begin ();
	RunList::iterator next = m_cedar_run->begin ();
	++next;
    std::swap(*next, *it);
#elif defined(COMP_RUNNER)
    // - Sort all contexts.
    // In CompRunner, Fsbus and designated context are executed and others are skipped.
    // Then, the order of skipped context isn't correct.
    // Need to sort all contexts here.
    bubble_sort (m_cedar_run->begin (), m_cedar_run->end ());
#else // defined(COMP_RUNNER)
    uint64_t my_time = std::max(ht->GetCpuTime (), ht->GetBusTime ());

    // リストの後ろに挿入する確率が高いので、後ろから探索する
    RunList::iterator it_dest = m_cedar_run->end ();
    RunList::iterator it_source = m_cedar_run->begin ();
    while (it_dest != it_source) {
        -- it_dest;
        if (std::max((*it_dest)->GetCpuTime (), (*it_dest)->GetBusTime ()) <= my_time) {
            // 挿入位置を発見
            // 挿入位置が自分なら並び替え不要
            if (it_dest != it_source) {
                // 実際に挿入する位置は発見位置の次要素
                // 先頭要素を指定位置に移動するのでrotateを使用
                rotate (it_source, it_source + 1, ++ it_dest);
            }
            return;
        }
    }
#endif // defined(COMP_RUNNER)
}


//! 新規スレッドの起動時、自スレッドの停止時など、
//! 先頭HTが並べ替え対象でない場合用のrunlistのソート。
//! めったに呼ばれない
void ForestInternals::UpdateHtOrder(ModuleRun* ht)
{
    RunList::iterator it_source = std::find (m_cedar_run->begin (), m_cedar_run->end (), ht);
    if (it_source == m_cedar_run->end ()) {
        // 該当スレッドは起動対象外なのでソート不要
        return;
    }

    uint64_t my_time = std::max (ht->GetCpuTime (), ht->GetBusTime());

    // リストの後ろに挿入する確率が高いので、後ろから探索する
    RunList::iterator it_dest = m_cedar_run->end ();
    while (it_dest != it_source) {
        -- it_dest;
        if (std::max ((*it_dest)->GetCpuTime (), (*it_dest)->GetBusTime()) <= my_time) {
            // 挿入位置を発見
            // 挿入位置が自分なら並び替え不要
            if (it_dest != it_source) {
                // 実際に挿入する位置は発見位置の次要素
                // 先頭要素を指定位置に移動するのでrotateを使用
                rotate (it_source, it_source + 1, ++ it_dest);
            }
            return;
        }
    }
}


#ifdef SUPPORT_SC_HEAP
void ForestInternals::Consume1Cycle (void)
{
    ModuleRun* ht = m_cedar_run->front ();
    uint64_t top_ht_time = std::max (ht->GetCpuTime (), ht->GetBusTime());
    while (std::max (ht->GetCpuTime (), ht->GetBusTime()) == top_ht_time) {
        if (ht->IsSuspendState () != true) {
            // 非Suspendスレッドは同じ時刻に存在しないので、ここには来ないはず
            break;
        }
        ht->PreConsumeClock();
        UpdateHeadHtOrder (ht);
        ht = m_cedar_run->front ();
    }
}


// 非SuspendなHTが存在するかチェック
bool ForestInternals::IsExecutable (void)
{
    if (m_cedar_run->front ()->IsSuspendState () == false) {
        return true;
    } else {
        // 他に同じ時刻で自分以外に実行可能なスレッドがなければブレーク
        //   SCHEAP管理のAPBアクセス時にだけこの条件は成立し、
        //   頻度低いはずなので、多少重くても精度重視で判定。
        if (IsPendableHtExec () == false) {
            return false;
        } else {
            return true;
        }
    }
}


// 先頭HTの一時的な実行スキップ for SCHEAP
// リスト先頭のcpu_timeとリスト後続のcpu_timeが同じなら、後続を先に実行
// ただし、後続HTもSCHEAPからのメモリアクセスレイテンシ通知待ちなら、何もしない
bool ForestInternals::IsPendableHtExec(void)
{
    if (m_cedar_run->size () <= 1) {
        // 自分以外に実行対象なし
        return false;
    } else {
        uint64_t own_ht_time = std::max (m_cedar_run->front ()->GetCpuTime (), m_cedar_run->front ()->GetBusTime());
        RunList::iterator it = m_cedar_run->begin ();
        ++ it; // 先頭は自分
        while (it != m_cedar_run->end ()) {
            if (std::max ((*it)->GetCpuTime (), (*it)->GetBusTime()) > own_ht_time) {
                // 同時刻に実行可能な他HTは存在しない
                break;
            }

            if ((*it)->IsSuspendState () == false) {
                // 実行可能な他HTを発見したので先頭に移動
                ModuleRun* target_ht = *it;
                m_cedar_run->erase (it);
                m_cedar_run->insert (m_cedar_run->begin (), target_ht);
                return true;
            }
            ++ it;
        }
        return false;
    }
}


void ForestInternals::SetBreakByTimeout (void)
{
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event |= BREAK_TIMEOUT;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event = BREAK_TIMEOUT;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
}


void ForestInternals::SetBreakTraceFull (void)
{
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event |= BREAK_TRACE_FULL;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event = BREAK_TRACE_FULL;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
}


void ForestInternals::InitTrace (void)
{
    m_trace_buf->Init ();
}


const TraceCtrl* ForestInternals::DequeueTrace (void)
{
    return m_trace_buf->Dequeue ();
}


TraceCtrl* ForestInternals::ReserveTrace (void)
{
    return m_trace_buf->Reserve ();
}


void ForestInternals::QueueTrace (TraceCtrl* trace)
{
    m_trace_buf->Queue (trace);
}


CfApiRetern ForestInternals::SetMaxTraceSize (uint32_t num)
{
    return m_trace_buf->SetMaxSize (num);
}


uint32_t ForestInternals::GetUsedTraceSize (void) const
{
    return m_trace_buf->GetUsedSize ();
}


bool ForestInternals::IsFullTraceBuf (void) const
{
    return m_trace_buf->IsFull ();
}


void ForestInternals::UpdateBreakInfo (int32_t scid)
{
    bool set_detail = false;
    BreakHitAttribute event = BRK_NONE;
    if (m_break_event == BREAK_NONE) {
        // no event
     } else {
        if ((m_break_event & BREAK_TRACE_FULL) == BREAK_TRACE_FULL) { event |= BRK_TRACE_FULL; }
        if ((m_break_event & BREAK_TIMEOUT) == BREAK_TIMEOUT)       { event |= BRK_TIMEOUT; }
        if ((m_break_event & BREAK_PC) == BREAK_PC)                 { event |= BRK_INST_EXE; set_detail = true; }
        if ((m_break_event & BREAK_ACC_READ) == BREAK_ACC_READ)     { event |= BRK_DATA_READ; set_detail = true; }
        if ((m_break_event & BREAK_ACC_WRITE) == BREAK_ACC_WRITE)   { event |= BRK_DATA_WRITE; set_detail = true; }
        if ((m_break_event & BREAK_ACCEPT_INT) == BREAK_ACCEPT_INT) { event |= BRK_ACCEPT_INT; set_detail = true; }
        //if ((m_break_event & BREAK_MAX) == BREAK_MAX)             { } // SCHEAP接続時、MAXブレークは無効化されて発生しない
        if ((m_break_event & BREAK_PC_MATCH) == BREAK_PC_MATCH)     { event |= BRK_PC_MATCH;  set_detail = true; }
        if ((m_break_event & BREAK_GUARD_ERR) == BREAK_GUARD_ERR)   { event |= BRK_GUARD_ERR;  set_detail = true; }
        if ((m_break_event & BREAK_STEP_EXE) == BREAK_STEP_EXE)     { event |= BRK_STEP_EXE;  set_detail = true; }
        if ((m_break_event & BREAK_CHANGE_PSWH) == BREAK_CHANGE_PSWH) { event |= BRK_CHANGE_PSWH;  set_detail = true; }
    }
    m_break_info.attr = event;

    if (set_detail == true) {
        uint32_t peid =  m_forest_util->GetPeId (scid);
        RegData64 pc_val;
        m_forest_util->RegReadDebug (peid, PC, &pc_val);
        m_break_info.peid = peid;
        m_break_info.inst_addr = pc_val & 0xFFFFFFFFU;
        m_break_info.acc_addr  = m_break_acc_addr;
        m_break_info.size = m_break_acc_size;
        m_break_info.gd_err = m_break_guard_err;
    } else {
        m_break_info.gd_err = GD_NONE_ERR;
    }
}


void ForestInternals::NotifyMatchPc (PhysAddr match_pc)
{
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event |= BREAK_PC_MATCH;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event = BREAK_PC_MATCH;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_pc    = match_pc;
}


void ForestInternals::NotifyBreakEcm (GuardErrType err, PhysAddr break_pc)
{
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event |= BREAK_GUARD_ERR;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event = BREAK_GUARD_ERR;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_guard_err = err;
    m_break_pc    = break_pc;
}


FsCtrl* ForestInternals::GetHtFromSideband (SideBand sideband)
{
    uint32_t peid = GetPeidFromBmid (sideband.GetBMID ());
    return m_forest->GetPePtr(peid)->GetHtPtr();
}
#endif // SUPPORT_SC_HEAP


void ForestInternals::LoadSrecBody (PhysAddr addr, uint8_t data)
{
    m_forest_util->BusWriteDebug (addr, &data, SIZE_8BIT);
}


void ForestInternals::CloseIssTraceFile (void)
{
    // デバッグログ用のファイルは、デバッガIF側で閉じる

    // isstrace用のファイル
    PeVec::const_iterator peit = m_forest->m_pe.begin ();
    while (peit != m_forest->m_pe.end ()) {
        if ((*peit) != NULL) {
            // NCのファイルポインタをclose
            FILE* tcfp_nc = (*peit)->GetHtPtr()->GetTraceFilePtrRTL ();
            fclose (tcfp_nc);
        }
        ++ peit;
    }
}


FILE* ForestInternals::GetFilePointer (uint32_t peid) const
{
    return  m_forest->GetPePtr(peid)->GetHtPtr()->GetFilePointer ();
}


//! CForest内時間の初期化
//! \sa FsCtrl::SetCpuTime
//! \sa FsCtrl::SetCpuRatio
void ForestInternals::InitTime (void)
{
    // システム時間の設定
    m_forest->fsbus->SetCpuTime (0);
    m_forest->fsbus->SetCpuRatio (1);

    // メモリ以外(IO空間)のレイテンシを初期化
    m_not_mem_latency->InitMemCycle ();
    m_forest->fsbus->ResetLatencyInfo ();

    // HT時間の設定
    HtVec::const_iterator it = m_cedar.begin ();
    while (it != m_cedar.end ()) {
        (*it)->SetCpuTime (0);
        (*it)->SetCpuRatio (1UL);
        (*it)->SetHaltCycle (0);
        ++ it;
    }
    UpdateRunList ();
}


#ifdef CLUSTER
MemLatencyInfoBase* ForestInternals::GetMemResourceInfo (uint32_t peid,
                                                         PhysAddr addr,
                                                         MemAccessCommand cmd)
{
    FsBus* fsbus = m_forest->fsbus;
    SideBand sideband;
    sideband.SetPEID (peid);
    PhysAddr bus_addr = m_forest->fsbus->GetBusAddress (sideband, addr);

    // 対象アドレスを含む周辺を検索
    FsBus::BusTarget* tgt = m_forest->fsbus->SearchAccessTarget (bus_addr, peid, cmd);

    MemLatencyInfoBase* mem_latency = tgt->m_latency;
    if (mem_latency != NULL) {
        return mem_latency;
    }

    // 対象にレイテンシ情報が設定されていない
    return m_not_mem_latency;
}
#endif // CLUSTER


MemLatencyInfoBase* ForestInternals::GetMemResourceInfo (uint32_t peid, PhysAddr addr)
{
    FsBus* fsbus = m_forest->fsbus;

    SideBand sideband;
    sideband.SetBMID (GetBmidFromPeid (peid));

    PhysAddr bus_addr = fsbus->GetBusAddress (sideband, addr);

    // 対象アドレスを含む周辺を検索
    FsBus::BusTarget* tgt = fsbus->SearchAccessTarget (bus_addr);

    MemLatencyInfoBase* mem_latency = tgt->m_latency;
    if (mem_latency != NULL) {
        return mem_latency;
    }

    // 対象にレイテンシ情報が設定されていない
    return m_not_mem_latency;
}


bool ForestInternals::IsFlashRomExisted (PhysAddr addr) const
{
    FsBus* fsbus = m_forest->fsbus;

    // FlashROMにミラーはないはず
    // Mirror->実体変換
    //FsBus::TgtBlock* tgtblock = fsbus->SearchMirrorBlock (addr);
    //if (tgtblock != NULL) {
    //    addr += tgtblock->m_offset;
    //}

    // 対象アドレスを含む周辺を検索
    FsBus::BusTarget* tgt = fsbus->SearchAccessTarget (addr);
    if ((fsbus->IsTargetValid (tgt) == true) && (m_forest->smem0 != tgt->m_btgt_vc)) {
        return true;
    } else {
        return false;
    }
}


bool ForestInternals::IsGramExisted (PhysAddr addr) const
{
    // GRAMガードのみ実装領域判定用
    // - GRAM以外のアクセスがGRAMガードに行く事はない。
    //   GRAM以外でも、とにかくターゲットがあればOK
    FsBus* fsbus = m_forest->fsbus;

    // GRAMにミラーはないはず
    // Mirror->実体変換
    //FsBus::TgtBlock* tgtblock = fsbus->SearchMirrorBlock (addr);
    //if (tgtblock != NULL) {
    //    addr += tgtblock->m_offset;
    //}

    // 対象アドレスを含む周辺を検索
    FsBus::BusTarget* tgt = fsbus->SearchAccessTarget (addr);
    if ((fsbus->IsTargetValid (tgt) == true) && (m_forest->smem0 != tgt->m_btgt_vc)) {
        return true;
    } else {
        return false;
    }
}


void ForestInternals::NotifyHitBreak (BreakEvent event, PhysAddr break_pc)
{
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event |= event;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event = event;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_pc    = break_pc;
    if (event == BREAK_MAX) {
        // MAX(=TIMEOUT)で終了したら正常終了とみなさない
        SetExitStatus (EXIT_MAX);
    }
}


void ForestInternals::NotifyHitBreakAcc (BreakEvent event, PhysAddr break_pc,
                                         PhysAddr break_acc_addr, ByteSize break_acc_size)
{
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event |= event;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_event = event;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_break_pc    = break_pc;
    m_break_acc_addr = break_acc_addr;
    m_break_acc_size = break_acc_size;
}

void ForestInternals::CauseError (SideBand sideband, ErrorCode err_code)
{
#ifdef SUPPORT_CUBESUITE
    // CubeSuiteにエラーを通知する
    // CPUのTraceInfoにerrを設定する
    uint32_t peid = m_forest_util->GetPeidFromBmid (sideband.GetBMID ());
    if (IsValidPeId (peid) == false) {
        peid = m_forest_util->GetPeMasterId (); // Set valid pe temporally.
    }
    switch (err_code) {
    case E_MEMORY_WRITE_PROTECT:
    case E_MEMORY_NOT_MAPPED:
    case E_MEMORY_FETCH_PROTECT:
    case E_OK:
        (*CubeCallBack::__NotifyError) (peid, err_code);
        break;
    default:
        break;
    }
    m_forest_util->SetCubeErrorCode (err_code);
#else // SUPPORT_CUBESUITE
    switch (err_code) {
    case E_MEMORY_WRITE_PROTECT:
        CfMsg::DPrint (MSG_WAR, "<Warning: write to ROM area>\n");
        break;
    case E_MEMORY_READ_PROTECT:
        CfMsg::DPrint (MSG_WAR, "<Warning: read from unpermitted area>\n");
        break;
    case E_MEMORY_NOT_INITIALIZED:
        CfMsg::DPrint (MSG_WAR, "<Warning: read from uninitialized area>\n");
        break;
    case E_MEMORY_FETCH_PROTECT:
        CfMsg::DPrint (MSG_WAR, "<Warning: fetch from unpermitted area>\n");
        break;
    default: break; // 未対応
    }
#endif // SUPPORT_CUBESUITE
}


void ForestInternals::SetGuardCapErr(GuardModuleID guard_id, uint32_t location_id, TRegModuleVc* p_guard_cap_err)
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }

    std::map<Cforest::GuardAddrKey, TRegModuleVc*>::iterator it
        = m_forest->m_guard_cap_err.find (Cforest::GuardAddrKey (guard_id, location_id));
    if (it != m_forest->m_guard_cap_err.end ()) {
        // overwrite in case it is assigned.
        it->second = p_guard_cap_err;
    } else {
        // newly add
        m_forest->m_guard_cap_err.insert (std::map<Cforest::GuardAddrKey, TRegModuleVc*>::value_type (
            Cforest::GuardAddrKey (guard_id, location_id), p_guard_cap_err)
        );
    }
}


TRegModuleVc* ForestInternals::GetGuardCapErr(GuardModuleID guard_id, uint32_t location_id)
{
    if ((guard_id == GMID_CFG) || (guard_id == GMID_CFGCAP_SX2FX)) {
        location_id = 0;
    }

    std::map<Cforest::GuardAddrKey, TRegModuleVc*>::iterator it
        = m_forest->m_guard_cap_err.find (Cforest::GuardAddrKey (guard_id, location_id));
    if (it != m_forest->m_guard_cap_err.end ()) {
        return it->second;
    } else {
        return NULL; // no error capture module
    }
}


void ForestInternals::DeleteLinkBodyByExternal (PhysAddr addr, ByteSize size)
{
    uint32_t peid = m_forest_util->GetPeMasterId ();
    if (IsValidPeId (peid)) {
        // Delete link
        m_forest->GetPePtr(peid)->GetHtPtr()->m_lsu->DeleteLinkByExternal (addr, size);
    }
}


void ForestInternals::DeleteLinkByExternal (PhysAddr addr, ByteSize size)
{
    ByteSize link_size = Llsc::LINK_ADDR_SIZE;
    if (size <= link_size) {
        DeleteLinkBodyByExternal (addr, size);
    } else {
        // check all addr for each link size
        ByteSize remain;
        for (remain = size; remain >= link_size; remain -= link_size) {
            DeleteLinkBodyByExternal (addr, link_size);
            addr += link_size;
        }
        if (remain > 0) {
            DeleteLinkBodyByExternal (addr, remain);
        }
    }
}


void ForestInternals::WakeupSnooze (uint32_t peid)
{
    // PE内全TCのSNOOZEを解除する
    HtVec::const_iterator it = m_forest->GetPePtr(peid)->GetHtsPtr()->begin ();
    while ( it != m_forest->GetPePtr(peid)->GetHtsPtr()->end () ) {
        (*it)->SetSnooze (false);
        ++it;
    }
}


//! 実行待ちリストの再生成
//! \sa FsCtrl::IsHtRunable
void ForestInternals::UpdateRunList (void)
{
    m_cedar_run->clear ();
    HtVec::iterator it = m_cedar.begin ();
    while ( it != m_cedar.end () ) {
        if ((*it)->IsHtRunable () == true) {
            AddRunTarget (*it);
        }
        ++it;
    }
    m_cedar_run->push_back (m_forest->fsbus);
}


void ForestInternals::UpdateNextInstAddr (uint32_t peid, ContextId tcid)
{
    m_forest->GetPePtr(peid)->GetHtPtr()->UpdateNextInstAddr ();
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::GetIcacheWayType
CacheWayType ForestInternals::GetIcacheWayType (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetIcPtr()->GetIcacheWayType ();
}


//! \sa ProcElement::GetIcPtr
//! \sa Icache::SetIcacheWayType
void ForestInternals::SetIcacheWayType (uint32_t peid, CacheWayType way)
{
    m_forest->GetPePtr(peid)->GetIcPtr()->SetIcacheWayType (way);
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::GetDcacheWayType
CacheWayType ForestInternals::GetDcacheWayType (uint32_t peid) const
{
    return WAY0;
}


//! \sa ProcElement::GetDcPtr
//! \sa Dcache::SetDcacheWayType
void ForestInternals::SetDcacheWayType (uint32_t peid, CacheWayType way)
{
}

//! ForestUtilクラスメンバの初期設定
void ForestInternals::InitialSetup(HtVec* cedar)
{
    cedar->clear ();
    // 全HTポインタを格納
    // 登場回数多いので、毎回m_forest->m_htと記述するのを省略するため。
    HtVec::const_iterator it = m_forest->m_ht.begin ();
    while (it != m_forest->m_ht.end ()) {
        cedar->push_back (*it);
        ++ it;
    }
    m_cedar = *cedar;
}


void ForestInternals::RegClIdFromPeId (uint32_t clid, uint32_t peid)
{
    if (m_clid_vec.size () <= peid) {
        m_clid_vec.resize (peid + 1, 0);
    }
    m_clid_vec [peid] = clid;
}


bool ForestInternals::IsValidBmId (uint32_t bmid) const
{
    if (m_peid_vec.size () <= bmid) {
        return false;
    }
    if (m_peid_vec [bmid] == CF_INVALID_ID) {
        return false;
    }
    return true;
}


bool ForestInternals::IsValidPeId (uint32_t peid) const
{
    if (m_forest->m_pe.size () > peid) {
        return m_forest->m_pe [peid] != NULL;
    } else {
        return false;
    }
}


FILE* ForestInternals::OpenFile (std::string fname) const
{
    FILE* fp;
    if (fname.empty ()) {
        fp = stdout;
    } else {
        fp = fopen (fname.c_str(), "w");
        if (fp == NULL) {
            CfMsg::DPrint (MSG_ERR, "<Error: file open error: %s>\n", fname.c_str());
        }
    }
    return (fp);
}


DebugMode ForestInternals::GetPeDebugMode (uint32_t peid) const
{
    if (m_forest_util->IsValidPeId (peid) == false) {
        return DBG_OFF;
    }

    FsCtrl* tc = m_forest->GetPePtr(peid)->GetHtPtr ();
    return tc->GetDebugMode ();
}


Dbuf* ForestInternals::GetDbufPtr (uint32_t peid)
{
    return m_forest->GetPePtr(peid)->GetDbufPtr ();
}


#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
FlashCache* ForestInternals::GetFlcPtr (uint32_t peid)
{
    return m_forest->GetPePtr(peid)->GetFlcPtr ();
}
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)


uint32_t ForestInternals::GetDbufHitLatency (void)
{
    return m_forest_util->m_dbuf_hit_latency;
}


uint64_t ForestInternals::GetStepCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->m_trace->GetStepCount ();
}

uint64_t ForestInternals::GetCpuTime (uint32_t peid) const
{
    return m_forest->GetPePtr (peid)->GetHtPtr ()->GetCpuTime ();
}


uint64_t ForestInternals::GetCycleCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->m_trace->GetStageCycle (STAGE_DP);
}


uint64_t ForestInternals::GetBranchCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetBranchCount ();
}


uint64_t ForestInternals::GetBranchCondExecCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetBranchCondExecCount ();
}


uint64_t ForestInternals::GetBranchCondMissCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetBranchCondMissCount ();
}


uint64_t ForestInternals::GetEiintCount (uint32_t peid, uint32_t ch) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetEiintCount (ch);
}


uint64_t ForestInternals::GetFeintCount (uint32_t peid, uint32_t ch) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetFeintCount (ch);
}


uint64_t ForestInternals::GetExpAbortCount (uint32_t peid, uint32_t ch) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetExpAbortCount (ch);
}


uint64_t ForestInternals::GetExpNonAbortCount (uint32_t peid, uint32_t ch) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetExpNonAbortCount (ch);
}


uint64_t ForestInternals::GetBgintAbortCount (uint32_t peid, uint32_t ch) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetBgintAbortCount (ch);
}


uint64_t ForestInternals::GetBgintAccesptedCount (uint32_t peid, uint32_t ch) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetBgintAccesptedCount (ch);
}


uint64_t ForestInternals::GetTotalFetchCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->m_lsu->GetTotalFetchCount ();
}


uint64_t ForestInternals::GetRomFetchCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->m_lsu->GetRomFetchCount ();
}


uint64_t ForestInternals::GetRomDataAccCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->m_lsu->GetRomDataAccCount ();
}


uint64_t ForestInternals::GetIcacheHitCount (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetIcPtr()->GetHitCount ();
}


uint64_t ForestInternals::GetInstIssueStall (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->m_csctrl->GetInstIssueStallPmc ();
}


uint64_t ForestInternals::GetInstCountPmc (uint32_t peid) const
{
    return m_forest->GetPePtr(peid)->GetHtPtr()->GetInstCountPmc ();
}


void ForestInternals::SetCpuGuestModeToIntc2 (uint32_t peid, bool is_gm)
{
    CIntc2* intc2 = m_forest->intc2;
    if (intc2 != NULL) {
        intc2->SetCpuGuestMode (peid, is_gm);
    }
}


void ForestInternals::SetCpuGpidToIntc2 (uint32_t peid, uint32_t gpid)
{
    CIntc2* intc2 = m_forest->intc2;
    if (intc2 != NULL) {
        intc2->SetCpuGpid (peid, gpid);
    }
}
