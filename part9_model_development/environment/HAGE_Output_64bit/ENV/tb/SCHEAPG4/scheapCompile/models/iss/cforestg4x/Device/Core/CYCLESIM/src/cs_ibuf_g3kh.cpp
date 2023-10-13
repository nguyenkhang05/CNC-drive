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
#include <vector>
#include "./fs_controler.h"
#include "./cs_controler.h"
#include "./inst_queue.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./proc_element.h"
#include "./outstand.h"
#include "./cs_ibuf_g3kh.h"

#if defined(SUPPORT_CUBESUITE) || defined(SUPPORT_SC_HEAP)
#else // defined(SUPPORT_CUBESUITE) || defined(SUPPORT_SC_HEAP)
#define DEBUG_CSIBUF
#endif // defined(SUPPORT_CUBESUITE) || defined(SUPPORT_SC_HEAP)

// 命令バッファの初期化
void CsIbufG3KH::InitInstBuf (void)
{
    m_ifu_cycle = 0;
    m_fill_order = 0;
    m_if_valid = false;
    m_ifq_fetch = false;
    m_parallel_flag = false;
    m_is_branch_target = false;
    m_biu_res_avail_cycle = 0;
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        m_ibuf [i]->m_req_cycle = 0;
        m_ibuf [i]->m_hit_ic = false;  //for future
        m_ibuf [i]->addr = LINE_MASK;
        m_ibuf [i]->fill_order = 0;   // for future
        m_ibuf [i]->fill_cycle = 0;
        m_ibuf [i]->m_is_valid = false;
        m_ibuf [i]->m_released_cycle = 0;
    }
    m_fetch_latency = 0;

    m_id_addr = 0x7;
    m_id_cycle = 0x0;
    m_activate_prefetch = false;
    IdBufType prefetch = {0x7, 0x7, 0, 0, 0, 0};
    m_prefetch.assign (2, prefetch);
    m_biu_req_avail_cycle = 0;
    m_ia_addr = 0;
    m_last_fetch_route = ICACHE_MISS_ROUTE;
    m_last_ifq_addr = 0x7UL;
    m_branch_cycle = 0x0ULL;
    m_branch_tgt_ia = 0x7UL;
    m_last_ic_line_addr = 0x7UL;
    m_last_ic_hit = false;

    for (int32_t i = 0; i < REC_FETCH_NUM; i++) {
        m_scheap_latency [i] = 0;
    }
}

void CsIbufG3KH::InitFetchBuf (void)
{
    for (uint32_t i = 0; i < NUM_FETCH_LINE_G3KH20; i++) {
        m_fetchbuf [i]->addr = 0xffffffff;
        m_fetchbuf [i]->m_is_valid = false;
        m_fetchbuf [i]->prefetch_cyc = 0;
    }
}

void CsIbufG3KH::ResetFetchBuf (void)
{
    for (uint32_t i = 0; i < NUM_FETCH_LINE_G3KH20; i++) {
        m_fetchbuf [i]->addr = 0xffffffff;
        m_fetchbuf [i]->m_is_valid = false;
    }
}
//Addr 00 -> check branch cache line 0
//Addr 04 -> check branch cache line 1
//Addr 08 -> check branch cache line 2
//Addr 0c -> check branch cache line 3
//if current is tgt branch addr -> check whether target branch addr in 4 ways? -> yes: hit / no: miss -> fill data to
//CS simulation doesn't need to check opcode because when addr hits -> opcode available
//when branch$ hits -> latency of fetching opcode from branch$ = latency of fetching opcode available in IFU
void CsIbufG3KH::InitBraBuf (void)
{
    for (uint32_t i = 0; i < NUM_BRA_LINE_G3KH20; i++) {
        for (uint32_t j = 0; j < NUM_BRA_WAY_G3KH20; j++){
            m_brabuf [i]->addr[j] =(0xffffffff) & 0xFFFFFFFC;   //addr: 0x0/
            m_brabuf [i]->m_is_valid[j] = false;
        }
    }
}

// 命令バッファに新規登録
// fill_latencyは実際にFS側で命令アクセスした時に格納。
// already_set_latencyはfill_latency格納済みかどうかの検出用
void CsIbufG3KH::SetInstBuf (uint32_t pos, PhysAddr addr, uint64_t cycle, bool is_hit_ic)
{
    // CfMsg::TPrint (MSG_INF, m_trace, "SetInstBuf: prev ibuf[%u] addr=%08x, fill_order=%lu\n",
    //          pos, m_ibuf[pos]->addr, m_ibuf[pos]->fill_order);

    m_ibuf[pos]->addr = addr;
    m_ibuf[pos]->m_req_cycle  = m_ifu_cycle;
    m_ibuf[pos]->fill_cycle = cycle;
    m_ibuf[pos]->m_released_cycle = m_ifu_cycle + 1;
    m_ibuf[pos]->m_is_valid = true;

    // CLear duplicated entry.
    //ClearIdbuf (addr);
    uint32_t count = 0, index=0;
    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        //printf ("addr: 0x%x - fill_cycle: %d - pos: %d- m_is_valid: %d\n", (*it)->addr, (*it)->fill_cycle, index, (*it)->m_is_valid);
        index++;
        if (((*it)->m_is_valid == true) ) {
            count++;
        }
        ++ it;
    }
}


// Find entry from begin of vector.
// アドレスに対応する命令バッファエントリ番号の取得
// ミスの場合、無効なエントリ番号(IBUF_NUM)を返す
uint32_t CsIbufG3KH::GetInstBuf (PhysAddr addr) const
{
    //CfMsg::TPrint (MSG_INF, m_trace, "GetInstBuf:addr=%08x\n", addr);
    //addr &= 0xFFFFFFF8UL;
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        //CfMsg::TPrint (MSG_INF, m_trace,
        //    "addr=%08x, m_ibuf[i]->addr=%08x, m_ibuf[i]->m_is_valid=%d\n",
        //    addr, m_ibuf[i]->addr, m_ibuf[i]->m_is_valid);
        if ((addr == m_ibuf[i]->addr) && (m_ibuf[i]->m_is_valid)) {
            return i;
        }
    }
    return IBUF_NUM;
}


// 指定アドレスが命令バッファに存在するかチェック
bool CsIbufG3KH::IsHitInstBuf (PhysAddr addr) const
{
    uint32_t entry = GetInstBuf (addr);
    if (entry != IBUF_NUM) {
            return true;
    }
    return false;
}

// Find entry from begin of vector.
uint32_t CsIbufG3KH::GetBraBuf (PhysAddr addr) const
{
    uint32_t offset = addr & 0xc;
    //offset = 0 -> update line 0
    //offset = 4 -> update line 1
    //offset = 8 -> update line 2
    //offset = c -> update line 3
    for (uint32_t i = 0; i < NUM_BRA_WAY_G3KH20; i++) {
        if (((addr & 0xFFFFFFFC) == m_brabuf[offset>>2]->addr[i]) ) {
            return i;
        }
    }
    return NUM_BRA_WAY_G3KH20;
}

bool CsIbufG3KH::IsHitBraBuf (PhysAddr addr) const
{
    uint32_t way = GetBraBuf (addr);
    if (way != NUM_BRA_WAY_G3KH20) {
        return true;
    }

    return false;
}

void CsIbufG3KH::UpdateBraBuf (PhysAddr inst_addr)
{
    std::vector <BrabufType*>::const_iterator it = m_brabuf.begin ();
    if (m_bra_repl_type == BRABUF_FIFO) {
        uint32_t offset = inst_addr & 0xc;
        //offset = 0 -> update line 0
        //offset = 4 -> update line 1
        //offset = 8 -> update line 2
        //offset = c -> update line 3
        uint32_t way = m_way_index[offset>>2];
        m_brabuf[offset>>2]->addr[way] = (inst_addr & 0xFFFFFFFC);
        if (m_way_index[offset>>2] == 3){
            m_way_index[offset>>2] = 0;
        }else{
            m_way_index[offset>>2]++;
        }
    }else{
        //replace method is not FIFO
    }
}

// for debug
void CsIbufG3KH::DumpInstBuf (void) const
{
    CfMsg::TPrint (MSG_INF, m_trace, "Dump InstBuf contents\n");
    for (uint32_t i = 0; i< IBUF_NUM; i++){
        //printf ("m_ibuf[%d]->addr: 0x%x - fill_cycle: %d\n", i,m_ibuf[i]->addr, m_ibuf[i]->fill_cycle);
    }

    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        CfMsg::TPrint (MSG_INF, m_trace,
            "[%d] addr=%08x, " PRIu64 ", valid=%d, " PRIu64 ", res_cycle=%" PRIu64 "\n",
            i, m_ibuf[i]->addr, m_ibuf[i]->m_is_valid, m_ibuf[i]->fill_cycle);
    }

}


// 命令バッファからフェッチのレイテンシ情報を取得する。
uint64_t CsIbufG3KH::GetFetchCycleInstBuf (PhysAddr addr) const
{
    uint32_t entry = GetInstBuf (addr);
    if (entry == IBUF_NUM) {
#ifdef DEBUG_CSIBUF
            CfMsg::TPrint (MSG_ERR, m_trace,
                           "<Internal Error: Miss at GetFetchCycleInstBuf at GetIaCycle3>\n");
#endif // DEBUG_CSIBUF
           return m_ifu_cycle;
    }
    return m_ibuf[entry]->fill_cycle;
}


void CsIbufG3KH::ClearPrefetchBuffer (PhysAddr branch_tgt_addr)
{
    branch_tgt_addr &= 0xFFFFFFF8UL;
    std::vector<IdBufType>::iterator it = m_prefetch.begin ();
    while (it != m_prefetch.end ()) {
        if ((*it).m_addr != branch_tgt_addr) {
            (*it).m_addr = 0x00000007UL;
            (*it).m_base_addr = 0x00000007UL;
        }
        ++ it;
    }
}


int32_t CsIbufG3KH::GetPrefetchedEntry (PhysAddr inst_addr) const
{
    inst_addr &= 0xFFFFFFF0UL;
    std::vector<IdBufType>::const_iterator it = m_prefetch.begin ();
    while (it != m_prefetch.end ()) {
        if ((*it).m_base_addr == inst_addr) {
            return static_cast <int32_t> (std::distance (m_prefetch.begin (), it));
        }
        ++ it;
    }
    return -1; // miss
}

void CsIbufG3KH::SimPrefetch (PhysAddr inst_addr, uint64_t prefetch_cycle)
{
    PhysAddr next_line_addr = 0xffffffff;
    if (m_activate_prefetch == true) {
        if (m_is_branch_target == true) {
            next_line_addr = (inst_addr) & 0xFFFFFFF0UL;
        } else {
            next_line_addr = (inst_addr + 0x10U) & 0xFFFFFFF0UL;
        }
        //printf ("----next_line_addr: 0x%x\n", next_line_addr);
        PrefetchToFetchBuffer (next_line_addr, prefetch_cycle);
        m_activate_prefetch = false;

    }
}

void CsIbufG3KH::PrefetchToFetchBuffer (PhysAddr inst_addr, uint64_t cycle)
{
        PhysAddr ia_addr = inst_addr & 0xFFFFFFFC;
        std::vector <FetchbufType*>::const_iterator it = m_fetchbuf.begin ();
        while (it != m_fetchbuf.end ()) {
            if (((*it)-> addr == ia_addr) ) {
                //printf ("data already fetched: 0x%x\n",ia_addr);
                return;
            }
            ++ it;
        }

        uint64_t biu_cycle = cycle;
        m_biu_req_avail_cycle = biu_cycle + 1;
        SetFetchLatency (ia_addr, biu_cycle);
        uint64_t res_cycle = biu_cycle + m_fetch_latency;
        res_cycle = std::max (m_biu_res_avail_cycle, res_cycle);

        std::vector <FetchbufType*>::const_iterator it_2 = m_fetchbuf.begin ();
         while (it_2 != m_fetchbuf.end ()) {
        if (((*it_2)-> m_is_valid == false) ) {
            (*it_2)-> addr =  inst_addr;
            (*it_2)-> prefetch_cyc =  res_cycle;
            break;
        }
        ++ it_2;
    }
}

//opcode misses in data line, fetch it from ROM
void CsIbufG3KH::FetchFromBiu (PhysAddr ia_addr)
{
    uint64_t biu_cycle = m_ifu_cycle + 1;
    m_biu_req_avail_cycle = biu_cycle + 1;

    SetFetchLatency (ia_addr, biu_cycle);
    uint64_t res_cycle = biu_cycle + m_fetch_latency;

    res_cycle = std::max (m_biu_res_avail_cycle, res_cycle);
    uint32_t replace_entry = 0;

    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        if (((*it)->m_is_valid == false) ) {
           break;
        }
        ++replace_entry;
        ++ it;
        }
    if (replace_entry == IBUF_NUM) {
#ifdef DEBUG_CSIBUF
        CfMsg::TPrint (MSG_ERR, m_trace,
                                   "<Internal Error: IFU Buffer Full>\n");
#endif // #ifdef DEBUG_CSIBUF
    }
    SetInstBuf (replace_entry, ia_addr, res_cycle, false);

}

//G3KH10: opcode hits 100% in data line
void CsIbufG3KH::FetchFromFetchBuffer (PhysAddr ia_addr)
{
    uint32_t replace_entry = 0;

    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        if (((*it)->m_is_valid == false) ) {
            break;
        }
        ++replace_entry;
        ++ it;
    }

    uint64_t biu_cycle = m_ifu_cycle;
    m_biu_req_avail_cycle = biu_cycle + 1;

    SetFetchLatency (ia_addr, biu_cycle);
    uint64_t res_cycle = biu_cycle + m_fetch_latency;

    res_cycle = std::max (m_biu_res_avail_cycle, res_cycle);
    if (replace_entry == IBUF_NUM) {
#ifdef DEBUG_CSIBUF
        CfMsg::TPrint (MSG_ERR, m_trace,
                                   "<Internal Error: IFU Buffer Full>\n");
#endif // #ifdef DEBUG_CSIBUF
    }
    SetInstBuf (replace_entry, ia_addr, res_cycle, true);

}

//fetch opcode from data line [prefetch buffer]
void CsIbufG3KH::FetchFromPrefetchBuffer (PhysAddr ia_addr)
{
    uint32_t replace_entry = 0;

    PhysAddr inst_addr = ia_addr & 0xFFFFFFF0UL;
    uint64_t prefetch_cyc = 0;
    uint64_t res_cycle= 0;
        for (uint32_t i = 0; i < NUM_FETCH_LINE_G3KH20; i++) {
            //addr was on fetch buffer.
            if ((m_fetchbuf[i]->addr & 0xFFFFFFF0UL) == inst_addr) {
                prefetch_cyc = m_fetchbuf[i]->prefetch_cyc;
                break;
            }
        }

    res_cycle = std::max (m_ifu_cycle + 1, prefetch_cyc);

    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        if (((*it)->m_is_valid == false) ) {
            break;
        }
        ++replace_entry;
        ++ it;
    }
    if (replace_entry == IBUF_NUM) {
#ifdef DEBUG_CSIBUF
        CfMsg::TPrint (MSG_ERR, m_trace,
                                   "<Internal Error: IFU Buffer Full>\n");
#endif // #ifdef DEBUG_CSIBUF
    }
     SetInstBuf (replace_entry, ia_addr, res_cycle, true);

}

//branch cache hits -> get opcode from branch cache
void CsIbufG3KH::FetchFromBranchBuffer (PhysAddr ia_addr)
{
    uint32_t replace_entry = 0;

    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {

        if (((*it)->m_is_valid == false) ) {
            break;
        }
        ++replace_entry;
        ++ it;
    }
    if (replace_entry == IBUF_NUM) {
#ifdef DEBUG_CSIBUF
        CfMsg::TPrint (MSG_ERR, m_trace,
                                   "<Internal Error: IFU Buffer Full>\n");
#endif // #ifdef DEBUG_CSIBUF
    }
    uint64_t res_cycle = m_ifu_cycle + STAGE_IC;
    SetInstBuf (replace_entry, ia_addr, res_cycle, true);
}


void CsIbufG3KH::SetFetchInfoSCHEAP (TraceCtrl* trace)
{
#ifdef SUPPORT_SC_HEAP
    uint32_t fetch_count = trace->GetFetchCount ();
    for (uint32_t i = 0; i < fetch_count; ++i) {
        MemAccessRoute route = trace->GetFetchRoute (i);
        if ((route == ICACHE_HIT_ROUTE) || (route == AXC_HIT_ROUTE)) {
            m_scheap_latency[i] = 0;
        } else {
            PhysAddr fetch_addr = trace->GetFetchAddr (i);
#ifdef CLUSTER
            MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr, INST_READ);
#else // CLUSTER
            MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr);
#endif // CLUSTER

#ifdef WAIT_FETCH_SCHEAP
            if (mem_info->IsScheap () == true) {
                m_scheap_latency[i] = TARGET_IS_SCHEAP;
            } else {
                m_scheap_latency[i] = 0;
            }
#else //#ifdef WAIT_FETCH_SCHEAP
            m_scheap_latency[i] = 0;
#endif //#ifdef WAIT_FETCH_SCHEAP
        }
    }
#endif // SUPPORT_SC_HEAP
}


bool CsIbufG3KH::IsFetchLatencyReceived (TraceCtrl* trace)
{
#ifdef WAIT_FETCH_SCHEAP
    int32_t fetch_cnt = trace->GetFetchCount ();
    for (int32_t i = 0; i < fetch_cnt; i++) {
        if (m_scheap_latency[i] == TARGET_IS_SCHEAP) {
            return false;
        }
    }
#endif // #ifdef WAIT_FETCH_SCHEAP
    return true;
}


// Release waiting state for SC-HEAP.
bool CsIbufG3KH::SetFetchLatencySCHEAP (TraceCtrl* trace, PhysAddr addr, uint32_t latency)
{
#ifdef WAIT_FETCH_SCHEAP
    int32_t fetch_cnt = trace->GetFetchCount ();
    for (int32_t i = 0; i < fetch_cnt; i++) {
        if (addr == trace->GetFetchAddr (i)) {
            if (m_scheap_latency[i] == TARGET_IS_SCHEAP) {
                m_scheap_latency[i] = latency;
                return true;
            } else {
                break;
            }
        }
    }
    return false;
#else // #ifdef WAIT_FETCH_SCHEAP
    return true;
#endif // #ifdef WAIT_FETCH_SCHEAP
}


void CsIbufG3KH::SetFetchLatency (PhysAddr fetch_addr, uint64_t ia_cycle)
{
#ifdef CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr, INST_READ);
#else // CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr);
#endif // CLUSTER
    //m_last_fetch_mem_info = mem_info;

    // @@ When the fetch is outstanding, the same clock ratio calculation as the data side is required
    //    Currently, the calculation is omitted because fetch latency is not supported

    // GetFetchLatencyに渡すia_cycleはsystime基準。クロック比を考慮しないといけない
    // - 途中でクロック比が変わるかもしれないので、単純にクロック比をかけたら駄目
    uint64_t issue_ia_cycle;
    if (ia_cycle > m_csctrl->GetCycleCount ()) {
        uint64_t issue_diff = (ia_cycle - m_csctrl->GetCycleCount ()) * m_clk_ratio;
        issue_ia_cycle = m_fsctrl->m_cpu_time + issue_diff;
    } else {
        uint64_t issue_diff = (m_csctrl->GetCycleCount () - ia_cycle) * m_clk_ratio;
        issue_ia_cycle = m_fsctrl->m_cpu_time - issue_diff;
    }

    uint32_t latency =
            mem_info->GetFetchLatency (m_pe_id, fetch_addr, issue_ia_cycle, IQ_BYTES);
    //printf ("fetch latency: %d\n", latency);

#ifdef SUPPORT_SC_HEAP
    if (latency == TARGET_IS_SCHEAP) {
        m_fetch_latency = DUMMY_LATENCY_SCHEAP;
        return;
    }
#endif // SUPPORT_SC_HEAP

    // latencyはsystime基準。PEのクロック基準に換算する
    // FREQ周期で時間を進めるため、stall量をFREQ単位に切り上げ
    // @@ 現在outstand未対応なので、あらかじめ計算しておけるはず
    div_t result = div (latency, m_clk_ratio);

    uint32_t latency_pe_base = result.quot;
    if (result.rem != 0) {
        latency_pe_base += 1;
    }
    m_fetch_latency = latency_pe_base;
    //CfMsg::TPrint (MSG_INF, m_trace,
    //    "SetFetchLatency:addr=%08x, biu_req_cyc=%lu, fetch_latency=%u\n",
    //    fetch_addr, ia_cycle, m_fetch_latency);

#if 0
    uint32_t repeat_latency = mem_info->GetConflictLatency ();

    // latencyはsystime基準。PEのクロック基準に換算する
    // FREQ周期で時間を進めるため、stall量をFREQ単位に切り上げ
    // @@ 現在outstand未対応なので、あらかじめ計算しておけるはず
    div_t result_repeat = div (repeat_latency, m_clk_ratio);

    uint32_t repeat_pe_base = result_repeat.quot;
    if (result_repeat.rem != 0) {
        repeat_pe_base += 1;
    }
    m_fetch_repeat = repeat_pe_base; // repeat is fixed in a same target.
#endif // 0
#ifdef CLUSTER
    if (p_forest_util->IsEnableBusTrace ()) {
        m_csctrl->ShowAccessRoute (INST_READ, fetch_addr);
    }
#endif // #ifdef CLUSTER
}


bool CsIbufG3KH::IsFetchBufferHit (PhysAddr inst_addr, TraceCtrl* trace) const
{
    inst_addr &= 0xFFFFFFF0UL;
    for (uint32_t i = 0; i < NUM_FETCH_LINE_G3KH20; i++) {
        //addr was on fetch buffer.
        if ((m_fetchbuf[i]->addr & 0xFFFFFFF0UL) == inst_addr) {
            return true;
        }
    }

    return false;
}

void CsIbufG3KH::SimIfq (PhysAddr ia_addr, TraceCtrl *trace)
{
    PhysAddr ifq_addr = ia_addr & 0xFFFFFFFEUL;

    bool hit_fetchbuf = false;
    bool flag_branch = false;

    PhysAddr inst_addr = trace->GetInstAddr ();
    ByteSize inst_size = trace->GetInstSize ();

    m_corner_hit =false;
    m_is_branch_buff_hit = false;

    //instruction is not in IFU
    if (IsHitInstBuf (ifq_addr) == false) {
//        if (m_core_version == CORE_VER_20){
        if (1) { // G4KH
            //whether opcode is in data line
            hit_fetchbuf = IsFetchBufferHit (ifq_addr, trace);
        } else {
            //due to latency in G3KH1.0, opcode's always available in data line
            hit_fetchbuf = true;
        }

        if ((hit_fetchbuf == true)){
//            if (m_core_version == CORE_VER_20) {
            if (1) { // G4KH
                FetchFromPrefetchBuffer (ifq_addr);
                FetchFromPrefetchBuffer (ifq_addr + 2);
            }else {
                FetchFromFetchBuffer (ifq_addr);
                FetchFromFetchBuffer (ifq_addr + 2);
            }
        } else {//opcode is not in data line
            m_activate_prefetch = true;
            if ((m_is_branch_target == true)) {   //branch taken, this is branch target
                if (IsHitBraBuf(ifq_addr) == true){
                    //Branch cache hit
                    FetchFromBranchBuffer (ifq_addr);
                    FetchFromBranchBuffer (ifq_addr + 2);
                    m_is_branch_buff_hit = true;
                } else {
                    //Branch cache miss
                    FetchFromBiu(ifq_addr);
                    FetchFromBiu(ifq_addr + 2);
                    flag_branch = true;
                }
            }else {    //this is not branch target
                FetchFromBiu (ifq_addr);
                FetchFromBiu (ifq_addr + 2);
            }
            m_last_ic_line_addr = ia_addr & 0xFFFFFFF0UL;
            m_last_ic_hit = false;
       }
       m_last_ifq_addr = ifq_addr; // @@ 必要か？
    } else {


    }

    if (flag_branch == true){
        if (inst_size >= 6) {
            PhysAddr bra_addr = inst_addr & 0xFFFFFFFCUL;
            bool is_branch_hit = IsHitBraBuf (bra_addr);
            if (! is_branch_hit) {
                UpdateBraBuf(inst_addr);
            }
        } else {
            UpdateBraBuf(inst_addr);
        }
    }
    IncIaAddr ();
}


void CsIbufG3KH::IncIaAddr (void)
{
    m_ia_addr = (m_ia_addr & 0xFFFFFFFCU) + 0x4U;
}

bool CsIbufG3KH::HasEmptyIfqEntry (void) const
{
    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        if (((*it)->m_is_valid == false) && ((*it)->m_released_cycle <= m_ifu_cycle)) {
            return true;
        }
        ++ it;
    }
    return false;
}

//return the status of IFU: how many slots occupied
uint32_t CsIbufG3KH::IndexIfqEntry (void) const
{
    uint32_t count = 0;
    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        if (((*it)->m_is_valid == true) ) {
            count++;
        }
        ++ it;
    }
    return count;
}



void CsIbufG3KH::SimFetchStep (PhysAddr ia_addr, TraceCtrl* trace)
{
    SimIfq (ia_addr, trace);

//    if (m_core_version == CORE_VER_20) {
    if (1) { // G4KH
        //address at 0xc, prefetch was executed for next line
        if (((ia_addr & 0xc) == 0xc))  {
            m_activate_prefetch = true;
        }
    }
    SimPrefetch (ia_addr, m_ifu_cycle);
}

//release slot of IFU after using
void CsIbufG3KH::IbufEntryRelease (PhysAddr addr)
{
    std::vector <IbufType*>::const_iterator it = m_ibuf.begin ();
        while (it != m_ibuf.end ()) {
            if (((*it)->addr == addr) ) {
                (*it)->m_is_valid = false;
            }
            ++ it;
        }
}



void CsIbufG3KH::SimFetchIa (PhysAddr inst_addr, TraceCtrl* trace)
{
    if (IndexIfqEntry() == 0){    //iqu buffer empty
        m_ifq_fetch = true;
        m_if_valid = true;
    }else if (((IndexIfqEntry() == 4) && (m_if_valid == true))|| (IndexIfqEntry() >= 6)){
        m_ifq_fetch = false;
        m_if_valid = false;
    }else{
        m_ifq_fetch = true;
        m_if_valid = true;
    }

    m_is_branch_target = false;
    // it means that this is due to the limitation of IFU when branch is taken
    // opcode in data line depends the status of IFU at that time => MISS or HIT on data line
    // => MISS or HIT on data line when target branch is in short distance (+0x10 or + 0x20)
    // this code serves this corner case.
    if (prefetch_limit == true){
        m_activate_prefetch = true;
        SimPrefetch (0x10, m_ifu_cycle -2);    //dummy
        prefetch_limit = false;
    }

    if (trace->GetBraTargetInfo() == true) {
        // Branch executed. Update ia address.
        m_ia_addr = inst_addr & 0xFFFFFFFCU;
        m_is_branch_target = true;

        //treat limitation of timing between IFU and CPU request
//        if (m_core_version == CORE_VER_20) {
        if (1) { // G4KH
            if ((m_prev_bra_addr & 0xe) <= 4) {         //branch addr: 0x0, 0x04
                if ((m_prev_bra_addr & 0xc) == 0){
                    prefetch_limit = true;
                }
            } else if ((m_prev_bra_addr & 0xe) == 0x8) {        //branch addr: 0x8
                m_activate_prefetch = true;
                SimPrefetch (m_prev_bra_addr +0x10, m_ifu_cycle -2);
                m_corner_hit = true;
            } else if ((m_prev_bra_addr & 0xe) > 0xc) {        //branch addr: 0xc, 0xe
                m_activate_prefetch = true;
                SimPrefetch (m_prev_bra_addr + 0x10, m_ifu_cycle -2);
                m_corner_hit = true;
            }
            prev_branch_addr = m_prev_bra_addr;
        }
   }

   while ((inst_addr >= m_ia_addr) ){
       SimFetchStep (m_ia_addr, trace);

       if (m_ifq_fetch == 0) {
            m_ifu_cycle++;
       }
       if (IndexIfqEntry() == 0){    //iqu buffer empty
           m_ifq_fetch = true;
           m_if_valid = true;
       }else if (((IndexIfqEntry() == 4) && (m_if_valid == true))|| (IndexIfqEntry() >= 6)){
           m_ifq_fetch = false;
           m_if_valid = false;
       }else{
           m_ifq_fetch = true;
           m_if_valid = true;
       }
       ++ m_ifu_cycle;
   }
}


void CsIbufG3KH::ResetInstBuf (void)
{
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        m_ibuf [i]->m_req_cycle = 0;
        m_ibuf [i]->m_hit_ic = false;
        m_ibuf [i]->addr = LINE_MASK; // 決してヒットしない値に設定
        m_ibuf [i]->fill_order = 0;
        m_ibuf [i]->fill_cycle = 0;
        m_ibuf [i]->m_is_valid = false;
        m_ibuf [i]->m_released_cycle = 0;
    }
}

void CsIbufG3KH::SimFetch (TraceCtrl* trace)
{
    //InvalidateUnusedIbuf ();
    m_trace = trace;

    PhysAddr inst_addr = trace->GetInstAddr ();
    ByteSize inst_size = trace->GetInstSize ();
    PhysAddr inst_end_addr = inst_addr + (inst_size - 1);
    PhysAddr inst_addr_base = inst_addr & 0xFFFFFFFC;
    PhysAddr inst_end_addr_base = inst_end_addr & 0xFFFFFFFC;

    //if previous is branch instruction, IFU is cleared
    //Due to limitation of current implementation, IFU level will not reach level 4 in normal case
    //IFU level >=4: clear IFU and branch taken flag = true to support debugging, force PC address.
    if ((trace->GetBraTargetInfo() == true) || (IndexIfqEntry() >= 4)){
        ResetInstBuf();
        trace->SetBraTargetInfo(true);
    }

    //branch to the same dataline
    if ((trace->GetBraTargetInfo() == true) && ((inst_addr & 0x10) == (m_prev_bra_addr & 0x10) )) {
        ResetFetchBuf();
    }

    SimFetchIa (inst_addr_base, trace);
    //keep value of branch buff hit in case instructions are in 2 data lines
    m_is_branch_buff_hit_1st = m_is_branch_buff_hit;

    if (inst_addr_base != inst_end_addr_base) {
        SimFetchIa (inst_end_addr_base, trace);
    }

//    if (m_core_version == CORE_VER_20) {
    if (1) { // G4KH
        if ( ((m_is_branch_target == true) && (m_is_branch_buff_hit_1st == true))) {
            m_ifu_cycle = m_branch_cycle;
            if ((inst_size == 2)) {
                m_is_prev_branch_tgt_16 = true;
            }
        } else if (m_corner_hit == true) {
            m_ifu_cycle = m_branch_cycle;
        }
    }

    //corner case: branch target at 0xc
//    if (m_core_version == CORE_VER_20) {
    if (1) { // G4KH
        if ((m_is_branch_target == true)) {
            if  (((inst_addr & 0xe) >= 0xc) || (((inst_addr & 0xe) == 0xa) && (trace->GetInstSize ()>=4 ))){
                 //printf ("corner case: address 0xc at branch target\n");
                m_activate_prefetch = true;
                SimPrefetch(inst_addr + 0x10, m_ifu_cycle+1);
            }
        }
    }

    m_prev_bra_addr = inst_addr;
}


uint64_t CsIbufG3KH::GetFetchCycle (TraceCtrl* trace)
{
    PhysAddr inst_addr = trace->GetInstAddr ();
    ByteSize inst_size = trace->GetInstSize ();

    uint64_t fetch_cycle = 0;

    if ( ((m_is_branch_buff_hit_1st == true)) ){
        //branch cache hit
        fetch_cycle = GetFetchCycleInstBuf (inst_addr);
        for (ByteSize i = 0; i< inst_size; i+=2){
              fetch_cycle = std::min (fetch_cycle, GetFetchCycleInstBuf (inst_addr + i));
        }
    }else {
        for (ByteSize i = 0; i< inst_size; i+=2){
               fetch_cycle = std::max (fetch_cycle, GetFetchCycleInstBuf (inst_addr + i));

        }
        //G3KH20
        //fix corner case of IFU: next instructions at branch target are 2 instructions- 16bits
        //bench: pntrch
        if ((m_is_prev_branch_tgt_16 == true) && (m_is_branch_target == false)){
            m_is_prev_branch_tgt_16 = false;
            if (inst_size == 2) {
                fetch_cycle = fetch_cycle - 1;
            }
        }
    }

    for (ByteSize i = 0; i< inst_size; i+=2){
        IbufEntryRelease (inst_addr + i);
    }

    return fetch_cycle;
}


void CsIbufG3KH::UpdateIaAddress (PhysAddr ia_addr, uint64_t ia_cycle)
{
    //CfMsg::TPrint (MSG_INF, m_trace,
    //               "UpdateIaAddress:ia_addr=%08x, ia_cycle=%lu\n", ia_addr, ia_cycle);
    // @@ Although "ia_cycle < ifu_cycle" is bug, to prevent internal error, use max.
#ifdef DEBUG_CSIBUF
    if (ia_cycle < m_ifu_cycle) {
        CfMsg::TPrint (MSG_ERR, m_trace, "<Internal Error: IA cycle is back in UpdateIaAddress>\n");
    }
#endif // DEBUG_CSIBUF
    m_branch_cycle = std::max (ia_cycle, m_ifu_cycle);
    //m_branch_cycle = ia_cycle;

    m_branch_tgt_ia = ia_addr;
}


void CsIbufG3KH::ForceUpdateIaAddress (PhysAddr ia_addr)
{
    m_ia_addr = ia_addr & 0xFFFFFFF8U;
    m_branch_tgt_ia = ia_addr & 0xFFFFFFF8U;
    m_branch_cycle = m_ifu_cycle;
}


// LRUに基づいて命令バッファエントリを選択
uint32_t CsIbufG3KH::GetInstBufEntryFromLru (void) const
{
    return 0;
}


// FSからCSに切り替わった際、FS命令キュー内容をCS命令バッファに書き込む
// FS側で命令キューからフェッチされた場合、
// CS側命令バッファに存在しない命令アドレスが登場してしまうので。
void CsIbufG3KH::ForceSetInstBuf (PhysAddr addr)
{
    UpdateIaAddress (addr , 0);
}


CsIbufG3KH::CsIbufG3KH (uint32_t pe_id, CoreType core_type, CoreVersion core_ver,
                        FsCtrl* fsctrl,  CsCtrl* csctrl)
         : m_pe_id (pe_id), m_core_type (core_type), m_core_version (core_ver),
           m_fsctrl (fsctrl),  m_csctrl (csctrl)
{
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        m_ibuf.push_back (new IbufType ());
    }

    //initialize fetch buffer, storing prefetch opcode
    for (uint32_t j = 0; j < NUM_FETCH_LINE_G3KH20; j++) {
         m_fetchbuf.push_back (new FetchbufType ());
    }
    //initialize branch cache buffer, storing target branch address of previous branch taken
    for (uint32_t j = 0; j < NUM_BRA_LINE_G3KH20; j++) {
             m_brabuf.push_back (new BrabufType ());
             m_way_index[j] = 0;
    }
    m_clk_ratio = 1;
    m_prev_bra_addr = 0xfffffffe;
    m_corner_hit = false;
    m_is_branch_buff_hit = false;
    m_is_branch_buff_hit_1st = false;
    m_is_prev_branch_tgt_16 = false;
    prefetch_limit = false;
    prev_branch_addr = 0x0;

    m_bra_repl_type = BRABUF_FIFO;

    InitInstBuf ();    //IFU
    InitFetchBuf ();//buffer stored opcode after fetching from IBU
    InitBraBuf ();// buffer stored opcode of target branch after branch occurs
}


CsIbufG3KH::~CsIbufG3KH ()
{
    std::vector<IbufType*>::iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        delete (*it);
        ++ it;
    }

    std::vector<FetchbufType*>::iterator it_f = m_fetchbuf.begin ();
    while (it_f != m_fetchbuf.end ()) {
        delete (*it_f);
        ++ it_f;
    }

    std::vector<BrabufType*>::iterator it_bra = m_brabuf.begin ();
    while (it_bra != m_brabuf.end ()) {
        delete (*it_bra);
        ++ it_bra;
    }
}


#if defined(_MSC_VER)
inline div_t div (uint32_t num, uint32_t denom)
{
    return div (static_cast<int> (num), static_cast<int> (denom));
}
#endif // _MSC_VER
