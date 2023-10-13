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
#include "./config.h"
#include "./fs_controler.h"
#include "./cs_controler.h"
#include "./inst_queue.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./proc_element.h"
#include "./outstand_base.h"
#include "./cs_ibuf_g3mh.h"

#ifdef DEBUG_CS
#define DEBUG_CSIBUF
#endif // DEBUG_CS

// 命令バッファの初期化
void CsIbufG3MH::InitInstBuf (void)
{
    m_ifu_cycle = 0;
    m_fill_order = 0;
    m_biu_res_avail_cycle = 0;
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        m_ibuf [i]->m_req_cycle = 0;
        m_ibuf [i]->m_hit_ic = false;
        m_ibuf [i]->addr = LINE_MASK; // 決してヒットしない値に設定
        m_ibuf [i]->fill_order = 0;
        m_ibuf [i]->fill_cycle = 0;
        m_ibuf [i]->m_is_valid = false;
        m_ibuf [i]->m_released_cycle = 0;
    }

    m_fetch_latency = 0;

    IdBufType inv_entry = {0x7, 0x7, 0, 0, 0, 0, false};
    m_idbuf.assign (IDBUF_NUM, inv_entry);

    m_over_prefetched_addr.assign (NUM_RECORD_OVERRUN_FETCH, 0x7);

    m_fill_req_cycle.clear ();
    m_id_pointer.clear ();

    m_id_addr = 0x7;
    m_id_cycle = 0x0;
    m_activate_prefetch = false;
    IdBufType temp = {0x7, 0x7, 0, 0, 0, 0, false};
    m_prefetch.assign (2, temp);
    m_ia_biu.assign (2, temp);
    m_biu_req_avail_cycle = 0;
    m_req_ia_addr = 0x7;
    m_ia_addr = 0;
    m_last_fetch_route = ICACHE_MISS_ROUTE;
    m_last_ifq_addr = 0x7UL;
    m_branch_cycle = 0x0ULL;
    m_branch_tgt_ia = 0x7UL;
    m_last_executed_addr = 0x7UL;
    m_last_executed_order = 0;
    m_last_ic_line_addr = 0x7UL;
    m_last_ic_hit = false;
}


// 命令バッファに新規登録
// fill_latencyは実際にFS側で命令アクセスした時に格納。
// already_set_latencyはfill_latency格納済みかどうかの検出用
void CsIbufG3MH::SetInstBuf (uint32_t pos, PhysAddr addr, uint64_t cycle, bool is_hit_ic)
{
    // CfMsg::TPrint (MSG_INF, m_trace, "SetInstBuf: prev ibuf[%u] addr=%08x, fill_order=%lu\n",
    //          pos, m_ibuf[pos]->addr, m_ibuf[pos]->fill_order);

    m_ibuf[pos]->addr = addr;
    m_ibuf[pos]->fill_order = ++ m_fill_order;
    m_ibuf[pos]->m_req_cycle  = m_ifu_cycle;
    m_ibuf[pos]->fill_cycle = cycle;
    m_ibuf[pos]->m_hit_ic = is_hit_ic;
    m_ibuf[pos]->m_released_cycle = m_ifu_cycle + 1;
    m_ibuf[pos]->m_is_valid = true;

    // CLear duplicated entry.
    ClearIdbuf (addr);
}


// Find entry from begin of vector.
// アドレスに対応する命令バッファエントリ番号の取得
// ミスの場合、無効なエントリ番号(IBUF_NUM)を返す
uint32_t CsIbufG3MH::GetInstBuf (PhysAddr addr) const
{
    //CfMsg::TPrint (MSG_INF, m_trace, "GetInstBuf:addr=%08x\n", addr);
    addr &= 0xFFFFFFF8UL;
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


// Find entry from begin of vector.
// アドレスに対応する命令バッファエントリ番号の取得
// ミスの場合、無効なエントリ番号(IBUF_NUM)を返す
uint32_t CsIbufG3MH::GetInstBuf (PhysAddr addr, uint64_t fill_order) const
{
    //CfMsg::TPrint (MSG_INF, m_trace, "GetInstBuf:addr=%08x\n", addr);
    addr &= 0xFFFFFFF8UL;
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        //CfMsg::TPrint (MSG_INF, m_trace,
        //    "addr=%08x, m_ibuf[i]->addr=%08x, m_ibuf[i]->m_is_valid=%d\n",
        //    addr, m_ibuf[i]->addr, m_ibuf[i]->m_is_valid);
        // Actually, only fill_order comparison is enough to select.
        if ((addr == m_ibuf[i]->addr) && (m_ibuf[i]->m_is_valid) && (fill_order == m_ibuf[i]->fill_order)) {
            return i;
        }
    }

    return IBUF_NUM;
}

// Find entry from end of vector.
uint32_t CsIbufG3MH::GetInstBufReverse (PhysAddr addr) const
{
    //CfMsg::TPrint (MSG_INF, m_trace, "GetInstBufReverse:addr=%08x\n", addr);
    addr &= 0xFFFFFFF8UL;
    for (int32_t i = IBUF_NUM - 1; i >= 0; i--) {
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
bool CsIbufG3MH::IsHitInstBuf (PhysAddr addr) const
{
    uint32_t entry = GetInstBuf (addr);
    if (entry != IBUF_NUM) {
        // m_last_executed_order
        if (m_ibuf[entry]->fill_order == m_last_executed_order) {
            return true;
        }
    }

    addr &= 0xFFFFFFF8UL;
    if ((m_idbuf[IDBUF_READ].m_addr == addr) || (m_idbuf[IDBUF_SKID].m_addr == addr)) {
        return true;
    }
    return false;
}


// 命令バッファエントリのダンプ
void CsIbufG3MH::DumpInstBuf (void) const
{
    CfMsg::TPrint (MSG_INF, m_trace, "Dump InstBuf contents\n");
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        CfMsg::TPrint (MSG_INF, m_trace,
            "[%d] addr=%08x, order=%" PRIu64 ", hit_ic=%d, valid=%d, req_cycle=%"
            PRIu64 ", res_cycle=%" PRIu64 "\n",
            i, m_ibuf[i]->addr, m_ibuf[i]->fill_order, m_ibuf[i]->m_hit_ic,
            m_ibuf[i]->m_is_valid, m_ibuf[i]->m_req_cycle, m_ibuf[i]->fill_cycle);
    }

    for (uint32_t i = 0; i < IDBUF_NUM; i++) {
        CfMsg::TPrint (MSG_INF, m_trace,
            "IDB[%d] addr=%08x, req_cycle=%" PRIu64 ", res_cycle=%" PRIu64 "\n",
            i, m_idbuf[i].m_addr, m_idbuf[i].m_req_cycle, m_idbuf[i].m_fill_cycle);
    }

    int32_t i = 0;
    std::vector<IdPointer>::const_iterator itd = m_id_pointer.begin ();
    while (itd != m_id_pointer.end ()) {
        CfMsg::TPrint (MSG_INF, m_trace,
            "IDpointer[%d] addr=%08x, id_cycle=%" PRIu64 ", fill_order=%" PRIu64 "\n",
            i,   (*itd).m_id_addr,   (*itd).m_id_cycle, (*itd).m_fill_order);
        ++ itd;
        ++ i;
    }

    std::vector<PhysAddr>::const_iterator itp = m_over_prefetched_addr.begin ();
    while (itp != m_over_prefetched_addr.end ()) {
        CfMsg::TPrint (MSG_ERR, m_trace,
            "prefetched[%d] addr=%08x\n", i,   (*itp));
        ++ itp;
        ++ i;
    }

#if 0
    uint32_t prev_order = 0;
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        if (m_ibuf[i]->m_is_valid) {
            if (m_ibuf[i]->fill_order <= prev_order) {
                printf ("Internal Error: invalid IBUF order\n");
            }
            prev_order = m_ibuf[i]->fill_order;
        }
    }
    if ((m_idbuf[IDBUF_READ].m_addr != 0x7) && (m_idbuf[IDBUF_SKID].m_addr != 0x7)) {
        if (m_idbuf[IDBUF_READ].m_addr <= m_idbuf[IDBUF_SKID].m_addr) {
            printf ("Internal Error: invalid IDBUF order\n");
        }
    }
#endif
}


// 命令バッファからフェッチのレイテンシ情報を取得する。
uint64_t CsIbufG3MH::GetFetchCycleInstBuf (PhysAddr addr) const
{
    uint32_t entry = GetInstBufReverse (addr);
    if (entry == IBUF_NUM) {
        if (m_idbuf[IDBUF_READ].m_addr == addr) {
            return m_idbuf[IDBUF_READ].m_fill_cycle;
        } else if (m_idbuf[IDBUF_SKID].m_addr == addr) {
            return m_idbuf[IDBUF_SKID].m_fill_cycle;
        } else {
#ifdef DEBUG_CSIBUF
            CfMsg::TPrint (MSG_ERR, m_trace,
                           "<Internal Error: Miss at GetFetchCycleInstBuf>\n");
#endif // DEBUG_CSIBUF
            return m_ifu_cycle;
        }
    }
    return m_ibuf[entry]->fill_cycle;
}


void CsIbufG3MH::InvalidateIfq (PhysAddr ia_addr, uint64_t release_cycle,
                                bool copy_to_id, uint64_t fill_order)
{
    uint32_t entry = GetInstBuf (ia_addr, fill_order);
    if (entry != IBUF_NUM) {
        if (copy_to_id) {
            if ((m_idbuf[IDBUF_READ].m_fill_order > fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order > fill_order)) {
                // All IDBUF entry is later than the deleted entry. Don't overwrite.
            } else {
                uint32_t pos;
                if ((m_idbuf[IDBUF_READ].m_fill_order > fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order < fill_order)) {
                    // SKID is old, but READ is latest. Record to SKID.
                    pos = IDBUF_SKID;
                    // CfMsg::TPrint (MSG_INF, m_trace,"InvalidateIfq:kita1\n");
                } else if ((m_idbuf[IDBUF_READ].m_fill_order < fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order > fill_order)) {
                    // READ is old, but SKID is latest. Record to READ.
                    pos = IDBUF_READ;
                    // CfMsg::TPrint (MSG_INF, m_trace,"InvalidateIfq:kita2\n");
                } else { // if ((m_idbuf[IDBUF_READ].m_fill_order < fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order < fill_order)) {
                    // both entries are old. Rotate them and Record to READ
                    std::vector <IdBufType>::iterator it = m_idbuf.begin ();
                    rotate (it, it + 1, m_idbuf.end ());
                    pos = IDBUF_READ;
                    //CfMsg::TPrint (MSG_INF, m_trace,"InvalidateIfq:kita3:rotate\n");
                }
                m_idbuf[pos].m_fill_cycle = m_ibuf[entry]->fill_cycle;
                m_idbuf[pos].m_req_cycle = m_ibuf[entry]->m_req_cycle;
                m_idbuf[pos].m_addr = m_ibuf[entry]->addr;
                m_idbuf[pos].m_base_addr = m_ibuf[entry]->addr & 0xFFFFFFE0UL;
                m_idbuf[pos].m_fill_order = m_ibuf[entry]->fill_order;
                m_idbuf[pos].m_hit_ic = m_ibuf [entry]->m_hit_ic;
                //CfMsg::TPrint (MSG_INF, m_trace,
                //    "InvalidateIfq:Update idbuf[%u]=%u, addr=%08x, order=%"
                //    PRIu64 "\n",
                //    pos, m_idbuf[pos].m_addr , m_idbuf[pos].m_fill_order);
            }
            m_ibuf[entry]->m_released_cycle = release_cycle + 1;
            m_ibuf[entry]->m_is_valid = false;
            m_ibuf[entry]->addr = 0x7; // 決してヒットしない値に設定
            //CfMsg::TPrint (MSG_INF, m_trace,
            //    "InvalidateIfq:Copy entry=%u, addr=%08x, m_ifu_cycle=%"
            //        PRIu64 ", release_cycle=%lu\n",
            //        entry, ia_addr, m_ifu_cycle, release_cycle + 1);
        } else {
            if ((m_idbuf[IDBUF_READ].m_fill_order > fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order > fill_order)) {
                // All IDBUF entry is later than the deleted entry. Don't overwrite.
            } else {
                uint32_t pos;
                if ((m_idbuf[IDBUF_READ].m_fill_order > fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order < fill_order)) {
                    // SKID is old, but READ is latest. Record to SKID
                    pos = IDBUF_SKID;
                    //CfMsg::TPrint (MSG_INF, m_trace,"InvalidateIfq:kita4\n");
                } else if ((m_idbuf[IDBUF_READ].m_fill_order < fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order > fill_order)) {
                    // READ is old, but SKID is latest. Record to READ
                    pos = IDBUF_READ;
                    //CfMsg::TPrint (MSG_INF, m_trace,"InvalidateIfq:kita5\n");
                } else { // if ((m_idbuf[IDBUF_READ].m_fill_order < fill_order) &&  (m_idbuf[IDBUF_SKID].m_fill_order < fill_order)) {
                    // both entries are old. Rotate them and Record to READ
                    std::vector <IdBufType>::iterator it = m_idbuf.begin ();
                    rotate (it, it + 1, m_idbuf.end ());
                    pos = IDBUF_READ;
                    //CfMsg::TPrint (MSG_INF, m_trace,"InvalidateIfq:kita6\n");
                }
                m_idbuf[pos].m_addr = 0x7; // Invalidate
                //CfMsg::TPrint (MSG_INF, m_trace,
                //    "InvalidateIfq:NoCopy:IDBUF clear addr=%08x\n", m_idbuf[pos].m_addr);
            }
            //CfMsg::TPrint (MSG_INF, m_trace,
            //    "InvalidateIfq:NoCopy entry=%u, addr=%08x, m_ifu_cycle=%"
            //        PRIu64 ", release_cycle=%lu\n",
            //        entry, ia_addr, m_ifu_cycle, release_cycle + 1);

            m_ibuf[entry]->m_released_cycle = release_cycle + 1;
            m_ibuf[entry]->m_is_valid = false;
            m_ibuf[entry]->addr = 0x7; // 決してヒットしない値に設定
        }
    }
}


bool CsIbufG3MH::IsHitOverrunFetch (PhysAddr addr) const
{
    std::vector <PhysAddr>::const_iterator it
        = std::find (m_over_prefetched_addr.begin (), m_over_prefetched_addr.end (), addr & 0xFFFFFFF8);
    return (it != m_over_prefetched_addr.end ());
}


void CsIbufG3MH::RecordOverrunFetch (IbufType* ibuf)
{
    PhysAddr line_addr = ibuf->addr & 0xFFFFFFF8;
    if (!IsHitOverrunFetch (line_addr)) {
        m_over_prefetched_addr.erase (m_over_prefetched_addr.begin ());
        m_over_prefetched_addr.push_back (ibuf->addr & 0xFFFFFFF8);
    }
}


void CsIbufG3MH::CancelFetch (void)
{
    //bool is_first = true;
    std::vector <IbufType*>::iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        if (((*it)->fill_order > m_last_executed_order) && ((*it)->m_is_valid == true)) {
            // 無駄に先行フェッチされた、キャンセルされるべきエントリ
            // それ以外の先行エントリは、IDタイミングにより開放されるので、ここではなにもしない
            //if (is_first) {
                RecordOverrunFetch (*it);
            //    is_first = false;
            //}

            (*it)->m_is_valid = false;
            (*it)->addr = 0x6; // 決してヒットしない値に設定

            // 空きエントリを先頭に移動
            uint32_t pos = static_cast <uint32_t> (std::distance (m_ibuf.begin (), it));
            rotate (m_ibuf.begin (), it, it + 1);
            it = m_ibuf.begin () + pos;
        } else {
            ++ it;
        }
    }

    // 分岐ターゲットなら再フェッチなので、リクエスト済みも全部キャンセル
    std::vector <IdBufType>::iterator itb = m_ia_biu.begin ();
    while (itb != m_ia_biu.end ()) {
        (*itb).m_addr = 0x00000007UL;
        (*itb).m_base_addr = 0x00000007UL;
        ++ itb;
    }
    m_idbuf[IDBUF_READ].m_addr = 0x7;
    m_idbuf[IDBUF_SKID].m_addr = 0x7;
    //CfMsg::TPrint (MSG_INF, m_trace, "CancelFetch: Invalidate IDBUF\n");

    // Don't keep IFQ content in IDBuf on InvalidateIFq().
    std::vector<IdPointer>::iterator itd = m_id_pointer.begin ();
    while (itd != m_id_pointer.end ()) {
        (*itd).m_need_copy = false;
        ++ itd;
    }

    // Although IA is same as previous, refetch is needed on branch target instruction.
    m_last_ic_line_addr = 0x7;
    m_last_ic_hit = false;
    m_last_ifq_addr = 0x7;

    ++ m_last_executed_order;

    m_id_addr = 0x7;
}


void CsIbufG3MH::ClearPrefetchBuffer (PhysAddr branch_tgt_addr)
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


int32_t CsIbufG3MH::GetPrefetchedEntry (PhysAddr inst_addr) const
{
    inst_addr &= 0xFFFFFFE0UL;

    std::vector<IdBufType>::const_iterator it = m_prefetch.begin ();
    while (it != m_prefetch.end ()) {
        if ((*it).m_base_addr == inst_addr) {
            return static_cast <int32_t> (std::distance (m_prefetch.begin (), it));
        }
        ++ it;
    }
    return -1; // miss
}


int32_t CsIbufG3MH::GetIaReqedEntry (PhysAddr inst_addr) const
{
    inst_addr &= 0xFFFFFFE0UL;

    std::vector<IdBufType>::const_iterator it = m_ia_biu.begin ();
    while (it != m_ia_biu.end ()) {
        if ((*it).m_base_addr == inst_addr) {
            return static_cast <int32_t> (std::distance (m_ia_biu.begin (), it));
        }
        ++ it;
    }
    return -1; // miss
}


bool CsIbufG3MH::IsHitPrefetch (PhysAddr inst_addr) const
{
    return (GetPrefetchedEntry (inst_addr) != -1);
}


void CsIbufG3MH::SimPrefetch (PhysAddr inst_addr)
{
    if (m_activate_prefetch == true) {
        PhysAddr next_line_addr = (inst_addr + 0x20U) & 0xFFFFFFE0UL;
        if (IsHitPrefetch (next_line_addr) == false) {
            // Prefetch doesn't check ICache HIT/MISS.
            //CfMsg::TPrint (MSG_INF, m_trace, "kita40:prefetch_addr=%08x\n", next_line_addr);
            if (IsHitInstBuf (next_line_addr) == false) { // @@ ?
                // req prefetch
                //CfMsg::TPrint (MSG_INF, m_trace,
                //    "kita41:prefetch_addr=%08x\n", next_line_addr);
                if (IsHitPrefetch (next_line_addr) == false) {
                    //CfMsg::TPrint (MSG_INF, m_trace,
                    //    "kita42:prefetch_addr=%08x\n", next_line_addr);
                    if (IsHitPrefetch (next_line_addr)) {
                        //CfMsg::TPrint (MSG_INF, m_trace,
                        //    "kita43:prefetch_addr=%08x\n", next_line_addr);
                        // Prefetch from prefetched data
                        // - Previous prefetch is canceled.
                        //   But next prefetch is issued for same address.
                        //   Then, comes this line.
                        //   Actually, IC was already filled on previous prefetch.
                    } else {
                        // Prefetch from BIU(FlashROM)
                        //CfMsg::TPrint (MSG_INF, m_trace,
                        //    "kita44:PrefetchFromBiu:%08x\n", next_line_addr);
                        PrefetchFromBiu (next_line_addr);
                    }
                }
            }
            //m_prefetched_addr = next_line_addr;
            //CfMsg::TPrint (MSG_INF, m_trace,
            //    "kita45:m_prefetched_addr:%08x\n", m_prefetched_addr);
        }
    } else {
        //CfMsg::TPrint (MSG_INF, m_trace, "Prefetch:Deactivated\n");
    }
}


bool CsIbufG3MH::IsHitBiuReq (PhysAddr ia_addr) const
{
    return (  (m_ia_biu[0].m_base_addr == (ia_addr & 0xFFFFFFE0UL))
           || (m_ia_biu[1].m_base_addr == (ia_addr & 0xFFFFFFE0UL)) );
}


void CsIbufG3MH::FetchFromPrefetch (PhysAddr ia_addr)
{
    //CfMsg::TPrint (MSG_INF, m_trace, "FetchFromPrefetch\n");
    uint32_t replace_entry = GetInstBufEntryFromLru ();


    int32_t entry = GetPrefetchedEntry (ia_addr);
    if (entry == -1) {
#ifdef DEBUG_CSIBUF
        CfMsg::TPrint (MSG_ERR, m_trace, "<Internal Error: FetchFromPrefetch>\n");
#endif // DEBUG_CSIBUF
        return;
    }
    uint64_t res_cycle = std::max (m_prefetch[entry].m_fill_cycle, m_ifu_cycle + 3);
    //uint64_t res_cycle = m_biu_pref_res_avail_cycle;
    SetInstBuf (replace_entry, ia_addr, res_cycle, false);
    UpdateInstBufLru (replace_entry);
    //++ m_biu_pref_res_avail_cycle;
    m_prefetch[entry].m_fill_cycle += 1;

    //DumpInstBuf ();
}


void CsIbufG3MH::PrefetchFromBiu (PhysAddr ia_addr)
{
    //CfMsg::TPrint (MSG_INF, m_trace, "PrefetchFromBiu:addr=%08x\n", ia_addr);
    //uint64_t biu_cycle = m_ifu_cycle + 3; // あれ？
    uint64_t biu_cycle = m_ifu_cycle; // あれ？
    m_biu_req_avail_cycle = biu_cycle + 1;

    SetFetchLatency (ia_addr, biu_cycle + 3);
    uint64_t res_cycle = biu_cycle + m_fetch_latency;

    res_cycle = std::max (m_biu_res_avail_cycle, res_cycle);

    // Preloadでは+0x20のみなのでラップアラウンドの考慮不要
    //m_prefetched_addr = ia_addr;
    //m_biu_pref_res_avail_cycle = res_cycle;

    m_biu_res_avail_cycle = res_cycle + FILL_BURST_CYC;

    // Prefetch
    m_prefetch.erase (m_prefetch.begin ());
    IdBufType pref_lat = {ia_addr, ia_addr, res_cycle, biu_cycle, res_cycle + FILL_BURST_CYC, false};
    m_prefetch.push_back (pref_lat);
    m_fill_req_cycle.push_back (res_cycle + FILL_BURST_CYC);
    //CfMsg::TPrint (MSG_INF, m_trace,
    //    "PrefetchFromBiu:req_cycle=%lu, res_cycle=%lu, m_biu_res_avail_cycle=%lu\n",
    //    biu_cycle, res_cycle, m_biu_res_avail_cycle);
}


void CsIbufG3MH::FetchFromBiu (PhysAddr ia_addr)
{
    uint64_t biu_cycle = m_ifu_cycle + (STAGE_DP + 1);
    m_biu_req_avail_cycle = biu_cycle + 1;

    SetFetchLatency (ia_addr, biu_cycle);

    uint64_t res_cycle = biu_cycle + m_fetch_latency;

    //CfMsg::TPrint (MSG_INF, m_trace, "FetchFromBiu:res_cycle=%lu, m_biu_res_avail_cycle=%lu\n",
    //        res_cycle, m_biu_res_avail_cycle);

    res_cycle = std::max (m_biu_res_avail_cycle, res_cycle);

    // @@ 使用中（これから使用する）エントリをリプレースしたらまずい。どうする？
    //    自分より先のIAを抑止すべきか？
    uint32_t replace_entry = GetInstBufEntryFromLru ();
    SetInstBuf (replace_entry, ia_addr, res_cycle, false);
    UpdateInstBufLru (replace_entry);

    m_req_ia_addr = ia_addr;
    //m_req_line_addr = ia_addr & 0xFFFFFFE0UL;
    //m_biu_res_cycle = CalcNextResponse (ia_addr, ia_addr, res_cycle);
    //m_release_queue_cycle = res_cycle + 4;

    m_biu_res_avail_cycle = res_cycle + FILL_BURST_CYC;

    m_ia_biu.erase (m_ia_biu.begin ());
    IdBufType iareq_lat = {ia_addr, ia_addr & 0xFFFFFFE0U,
                           CalcNextResponse (ia_addr, ia_addr, res_cycle),
                           biu_cycle, res_cycle + FILL_BURST_CYC, false};
    m_ia_biu.push_back (iareq_lat);

    m_fill_req_cycle.push_back (res_cycle + FILL_BURST_CYC);
    //CfMsg::TPrint (MSG_INF, m_trace, "FetchFromBiu:res_cycle=%lu, m_biu_res_avail_cycle=%lu\n",
    //res_cycle, m_biu_res_avail_cycle);

    //DumpInstBuf ();
}


void CsIbufG3MH::FetchFromReqedBiu (PhysAddr ia_addr)
{
    // @@ 使用中（これから使用する）エントリをリプレースしたらまずい。どうする？
    uint32_t replace_entry = GetInstBufEntryFromLru ();

    int32_t entry = GetIaReqedEntry (ia_addr);
    if (entry == -1) {
#ifdef DEBUG_CSIBUF
        CfMsg::TPrint (MSG_ERR, m_trace,
            "<Internal Error: FetchFromReqedBiu:ia_addr=%08x>\n", ia_addr);
#endif // DEBUG_CSIBUF
        return;
    }

    //uint64_t res_cycle = m_biu_res_cycle;
    uint64_t res_cycle = m_ia_biu[entry].m_fill_cycle;
    SetInstBuf (replace_entry, ia_addr, std::max (res_cycle, m_ifu_cycle + (STAGE_DP + 1)), false);
    UpdateInstBufLru (replace_entry);
    m_ia_biu[entry].m_fill_cycle = CalcNextResponse (ia_addr, m_ia_biu[entry].m_addr, res_cycle);

    //SetInstBuf (replace_entry, ia_addr, res_cycle, false);
    //UpdateInstBufLru (replace_entry);
    //m_biu_res_cycle = CalcNextResponse (ia_addr, m_req_ia_addr, res_cycle);

    //CfMsg::TPrint (MSG_INF, m_trace, "FetchFromReqedBiu:res_cycle=%lu\n",
    //res_cycle);

    //DumpInstBuf ();
}


void CsIbufG3MH::FetchFromIc (PhysAddr ia_addr)
{
    uint32_t replace_entry = GetInstBufEntryFromLru ();
    uint64_t res_cycle = m_ifu_cycle + STAGE_IC;
    SetInstBuf (replace_entry, ia_addr, res_cycle, true);
    UpdateInstBufLru (replace_entry);

    //DumpInstBuf ();
}


void CsIbufG3MH::SetFetchLatency (PhysAddr fetch_addr, uint64_t ia_cycle)
{
#ifdef CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr, INST_READ);
#else // CLUSTER
    MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr);
#endif // CLUSTER
    //m_last_fetch_mem_info = mem_info;

    // @@ フェッチのoutstanding対応時には、データ側と同様のクロック比計算が必要
    //    現在、フェッチレイテンシのアウトスタンディング未対応のため計算省略

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


uint64_t CsIbufG3MH::CalcNextResponse (PhysAddr ia_addr, PhysAddr req_addr, uint64_t cycle) const
{
    // @@ 必要なのは分岐時の分岐先フェッチだけ。

    static const uint32_t rr_table [4][4] =
        {{ 1, 1, 1, 1},
         { 2, 0, 1, 1},
         { 1, 3, 0, 0},
         { 4, 0, 0, 0}};  // 0 entry is not used.

    // Calcurate rasponse cycle including raparound
    // PC[4:3]=0 00 01 10 11
    // PC[4:3]=1 01 00 10 11
    // PC[4:3]=2 10 11 00 01
    // PC[4:3]=3 11 10 00 01
    // - Skip cycle when address is raparounded,
    //   because raparounded response isn't used/stored into IFQ.
    uint32_t dw_pos = (req_addr >> 3) & 0x3;
    uint32_t curr_dw_pos = (ia_addr >> 3) & 0x3;
    return cycle + rr_table[dw_pos][curr_dw_pos];
}


uint64_t CsIbufG3MH::GetIaCycle (PhysAddr inst_start_addr, PhysAddr inst_end_addr) const
{
    inst_start_addr &= 0xFFFFFFF8UL;
    inst_end_addr &= 0xFFFFFFF8UL;
    if (inst_start_addr != inst_end_addr) {
        uint32_t pos_1st = GetInstBufReverse (inst_start_addr);
        uint32_t pos_2nd = GetInstBufReverse (inst_end_addr);
        uint64_t req_cycle_1st;
        if (pos_1st == IBUF_NUM) {
            if (m_idbuf[IDBUF_READ].m_addr == inst_start_addr) {
                req_cycle_1st = m_idbuf[IDBUF_READ].m_req_cycle;
            } else if (m_idbuf[IDBUF_SKID].m_addr == inst_start_addr) {
                req_cycle_1st = m_idbuf[IDBUF_SKID].m_req_cycle;
            } else {
#ifdef DEBUG_CSIBUF
                CfMsg::TPrint (MSG_ERR, m_trace,
                "<Internal Error: Miss at GetFetchCycleInstBuf at GetIaCycle1>\n");
#endif // DEBUG_CSIBUF
                req_cycle_1st = m_ifu_cycle;
            }
        } else {
            req_cycle_1st = m_ibuf[pos_1st]->m_req_cycle;
        }
        uint64_t req_cycle_2nd;
        if (pos_2nd == IBUF_NUM) {
            if (m_idbuf[IDBUF_READ].m_addr == inst_end_addr) {
                req_cycle_2nd = m_idbuf[IDBUF_READ].m_req_cycle;
            } else if (m_idbuf[IDBUF_SKID].m_addr == inst_end_addr) {
                req_cycle_2nd = m_idbuf[IDBUF_SKID].m_req_cycle;
            } else {
#ifdef DEBUG_CSIBUF
                CfMsg::TPrint (MSG_ERR, m_trace,
                "<Internal Error: Miss at GetFetchCycleInstBuf at GetIaCycle2>\n");
#endif // DEBUG_CSIBUF
                req_cycle_2nd = m_ifu_cycle;
            }
        } else {
            req_cycle_2nd = m_ibuf[pos_2nd]->m_req_cycle;
        }
        return std::max (req_cycle_1st, req_cycle_2nd);
    } else {
        uint32_t pos_1st = GetInstBufReverse (inst_start_addr);
        if (pos_1st == IBUF_NUM) {
            if (m_idbuf[IDBUF_READ].m_addr == inst_start_addr) {
                return m_idbuf[IDBUF_READ].m_req_cycle;
            } else if (m_idbuf[IDBUF_SKID].m_addr == inst_start_addr) {
                return m_idbuf[IDBUF_SKID].m_req_cycle;
            } else {
#ifdef DEBUG_CSIBUF
                CfMsg::TPrint (MSG_ERR, m_trace,
                "<Internal Error: Miss at GetFetchCycleInstBuf at GetIaCycle3>\n");
#endif // DEBUG_CSIBUF
                return m_ifu_cycle;
            }
        }
        return m_ibuf[pos_1st]->m_req_cycle;
    }
}


bool CsIbufG3MH::IsIcHit (PhysAddr inst_addr, TraceCtrl* trace) const
{
    inst_addr &= 0xFFFFFFE0UL;
    int32_t fetch_count = trace->GetFetchCount ();
    for (int32_t i = 0; i < fetch_count; i++) {
        if ((trace->GetFetchAddr (i) & 0xFFFFFFE0UL) == inst_addr) {
            return trace->GetFetchRoute (i) == ICACHE_HIT_ROUTE;
        }
    }
    // Doesn't have any IC info
    // - Executed but not fetched because IFQ is already fetched
    // - Unexecuted PC (Next inst of branch inst, etc)
    // Use previous IC hit/miss history.
    return m_last_ic_hit; // @@ いつ更新する？
}


bool CsIbufG3MH::IsIcHit (PhysAddr inst_start_addr, PhysAddr inst_end_addr) const
{
    inst_start_addr &= 0xFFFFFFF8UL;
    inst_end_addr &= 0xFFFFFFF8UL;
    if (inst_start_addr != inst_end_addr) {
        uint32_t pos_1st = GetInstBufReverse (inst_start_addr);
        uint32_t pos_2nd = GetInstBufReverse (inst_end_addr);
        if ((pos_1st == IBUF_NUM) || (pos_2nd == IBUF_NUM)) {
            bool is_hit_1st;
            bool is_hit_2nd;
            if (pos_1st == IBUF_NUM) {
                // Check IDBUF
                if (m_idbuf[IDBUF_READ].m_addr == inst_start_addr) {
                    is_hit_1st = m_idbuf[IDBUF_READ].m_hit_ic;
                } else if (m_idbuf[IDBUF_SKID].m_addr == inst_start_addr) {
                    is_hit_1st = m_idbuf[IDBUF_SKID].m_hit_ic;
                } else {
                    is_hit_1st = true;
#ifdef DEBUG_CSIBUF
                    CfMsg::TPrint (MSG_ERR, m_trace,
                    "<Internal Error: Miss at GetFetchCycleInstBuf at IsIcHit1st>\n");
#endif // DEBUG_CSIBUF
                }
            } else {
                is_hit_1st = m_ibuf[pos_1st]->m_hit_ic;
            }

            if (pos_2nd == IBUF_NUM) {
                if (m_idbuf[IDBUF_READ].m_addr == inst_end_addr) {
                    is_hit_2nd = m_idbuf[IDBUF_READ].m_hit_ic;
                } else if (m_idbuf[IDBUF_SKID].m_addr == inst_end_addr) {
                    is_hit_2nd = m_idbuf[IDBUF_SKID].m_hit_ic;
                } else {
                    is_hit_2nd = true;
#ifdef DEBUG_CSIBUF
                    CfMsg::TPrint (MSG_ERR, m_trace,
                    "<Internal Error: Miss at GetFetchCycleInstBuf at IsIcHit2nd>\n");
#endif // DEBUG_CSIBUF
                }
            } else {
                is_hit_2nd = m_ibuf[pos_2nd]->m_hit_ic;
            }
            return is_hit_1st & is_hit_2nd;
        }
        return m_ibuf[pos_1st]->m_hit_ic && m_ibuf[pos_2nd]->m_hit_ic;
    } else {
        uint32_t pos = GetInstBufReverse (inst_start_addr);
        bool is_hit;
        if (pos == IBUF_NUM) {
            // Check IDBUF
            if (m_idbuf[IDBUF_READ].m_addr == inst_start_addr) {
                is_hit = m_idbuf[IDBUF_READ].m_hit_ic;
            } else if (m_idbuf[IDBUF_SKID].m_addr == inst_start_addr) {
                is_hit = m_idbuf[IDBUF_SKID].m_hit_ic;
            } else {
                is_hit = true;
#ifdef DEBUG_CSIBUF
                CfMsg::TPrint (MSG_ERR, m_trace,
                "<Internal Error: Miss at GetFetchCycleInstBuf at IsIcHit3>\n");
#endif // DEBUG_CSIBUF
            }
        } else {
            is_hit = m_ibuf[pos]->m_hit_ic;
        }
        return is_hit;
    }
}


void CsIbufG3MH::SimIfq (PhysAddr ia_addr, TraceCtrl *trace)
{
    PhysAddr ifq_addr = ia_addr & 0xFFFFFFF8UL;
    //CfMsg::TPrint (MSG_INF, m_trace, "kita30a:ia_addr=%08x, ifq_addr=%08x, ifq_line=%08x\n",
    //         ia_addr, ifq_addr, ifq_addr & 0xFFFFFFE0UL);
    if (m_last_ifq_addr != ifq_addr) {
        // req fetch
        if (IsHitInstBuf (ifq_addr) == false) {
            bool hit_ic = IsIcHit (ifq_addr, trace) || IsHitOverrunFetch (ifq_addr);
            PhysAddr ia_ic_addr = ia_addr & 0xFFFFFFE0UL;
            //CfMsg::TPrint (MSG_INF, m_trace,
            //    "hit_ic=%d, m_last_ic_line_addr=%08x, ia_ic_addr=%08x, m_last_ic_hit=%d\n",
            //    hit_ic, m_last_ic_line_addr, ia_ic_addr, m_last_ic_hit);
            if ((hit_ic == true)
               && ((m_last_ic_line_addr != ia_ic_addr) || (m_last_ic_hit == true)) ) {
                m_last_ic_line_addr = ia_addr & 0xFFFFFFE0UL;
                m_last_ic_hit = true;
                //CfMsg::TPrint (MSG_INF, m_trace, "kita30:addr=%08lx, ", ifq_addr);
                // Deactivate prefetch when IC hit
                m_activate_prefetch = false;
                //CfMsg::TPrint (MSG_INF, m_trace, "Dectivate prefetch\n");
                FetchFromIc (ifq_addr);
            } else {
                //CfMsg::TPrint (MSG_INF, m_trace, "kita30b:ifq_addr=%08x\n", ifq_addr);
                if (IsHitPrefetch (ifq_addr) == true) {
                    // Fetch from prefetched data
                    //CfMsg::TPrint (MSG_INF, m_trace, "kita31:addr=%08lx\n", ifq_addr);
                    FetchFromPrefetch (ifq_addr);
                    m_last_ic_line_addr = ia_addr & 0xFFFFFFE0UL;
                } else if (IsHitBiuReq (ifq_addr)) {
                    //CfMsg::TPrint (MSG_INF, m_trace, "kita32:addr=%08lx\n", ifq_addr);
                    FetchFromReqedBiu (ifq_addr);
                } else if (CanIssueReq (m_ifu_cycle, 3)) {
                    // Fetch from BIU(FlashROM)
                    //CfMsg::TPrint (MSG_INF, m_trace, "kita33:addr=%08lx\n", ifq_addr);
                    FetchFromBiu (ifq_addr);

                    // Activate prefetch when IC miss && biu fetch.
                    m_activate_prefetch = true;
                    //CfMsg::TPrint (MSG_INF, m_trace, "Activate prefetch\n");

                    m_last_ic_line_addr = ia_addr & 0xFFFFFFE0UL;
                    m_last_ic_hit = false;
                } else {
                    //CfMsg::TPrint (MSG_INF, m_trace, "kita34\n");
                    return; // BIU is busy.
                }
            }
        } else {
            //CfMsg::TPrint (MSG_INF, m_trace, "kita35\n");
            // Hit IFQ.
            // @@ doesn't need to update LRU of IFQ?
        }
        IncIaAddr ();
        m_last_ifq_addr = ifq_addr; // @@ 必要か？
    } else {
        //CfMsg::TPrint (MSG_INF, m_trace, "kita36\n");
    }
    //CfMsg::TPrint (MSG_INF, m_trace, "SimIfq return\n");
}


void CsIbufG3MH::IncIaAddr (void)
{
    m_ia_addr = (m_ia_addr & 0xFFFFFFF8U) + 0x8U;
}


bool CsIbufG3MH::IsBiuUsable (void) const
{
    return m_biu_req_avail_cycle <= m_ifu_cycle + 3;
}


bool CsIbufG3MH::HasEmptyIfqEntry (void) const
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


bool CsIbufG3MH::CanIssueReq (uint64_t ia_cycle, uint32_t limit_num) const
{
    // Outstanding is limited to limit_num.
    uint32_t oo_count = 0;

    std::vector<IdBufType>::const_iterator it = m_prefetch.begin ();
    while (it != m_prefetch.end ()) {
        if ((*it).m_finish_cycle > ia_cycle) {
            ++ oo_count;
        }
        //CfMsg::TPrint (MSG_INF, m_trace,
        //    "m_prefetch:ia_cycle=%lu, addr=%08x, m_finish_cycle=%lu\n",
        //    (*it).m_req_cycle, (*it).m_addr, (*it).m_finish_cycle);
        ++ it;
    }

    std::vector<IdBufType>::const_iterator itb = m_ia_biu.begin ();
    while (itb != m_ia_biu.end ()) {
        if ((*itb).m_finish_cycle > ia_cycle) {
            ++ oo_count;
        }
        //CfMsg::TPrint (MSG_INF, m_trace,
        //        "m_ia_biu:ia_cycle=%lu, addr=%08x, m_finish_cycle=%lu\n",
        //        (*itb).m_req_cycle, (*itb).m_addr, (*itb).m_finish_cycle);
        ++ itb;
    }

    //if (m_release_queue_cycle > ia_cycle) {
    //    ++ oo_count;
    //}
    //CfMsg::TPrint (MSG_INF, m_trace,
    //    "ia_cycle=%lu, m_release_queue_cycle=%lu\n", ia_cycle, m_release_queue_cycle);

    if (oo_count >= limit_num) {
#ifdef DEBUG_CSIBUF
        if (oo_count > 3) {
            CfMsg::TPrint (MSG_ERR, m_trace,
                "<Internal Error: Outstanding count is exceeded:%u>\n", oo_count);
        }
#endif // DEBUG_CSIBUF
        //CfMsg::TPrint (MSG_INF, m_trace, "CanIssueReq=false (oo_count=%u)\n", oo_count);
        return false;
    } else {
        //CfMsg::TPrint (MSG_INF, m_trace, "CanIssueReq=true (oo_count=%u)\n", oo_count);
        return true;
    }
}


void CsIbufG3MH::SimFetchStep (PhysAddr ia_addr, TraceCtrl* trace)
{
    if (HasEmptyIfqEntry ()) {
        //CfMsg::TPrint (MSG_INF, m_trace, "SimIfq\n");
        SimIfq (ia_addr, trace);
    } else {
        if (IsHitInstBuf (ia_addr)) {
            // IFQにヒットしたらIFQ更新不要なので空きがなくてもIA更新
            IncIaAddr ();
        }
    }

    if (IsBiuUsable () && CanIssueReq (m_ifu_cycle, 2)) {
        //CfMsg::TPrint (MSG_INF, m_trace, "SimPrefetch\n");
        SimPrefetch (ia_addr);
    } else {
        //CfMsg::TPrint (MSG_INF, m_trace,
        //    "Skip SimPrefetch: IsBiuUsable ()=%d, CanIssueReq (%lu)=%d\n",
        //    IsBiuUsable (), m_ifu_cycle, CanIssueReq (m_ifu_cycle, 2));
    }
}


bool CsIbufG3MH::ConflictFillReq (void)
{
    // @@ kari
    //return (   (m_biu_pref_res_avail_cycle == m_ifu_cycle)
    //        || (m_biu_res_cycle == m_ifu_cycle) );
    //CfMsg::TPrint (MSG_INF, m_trace,
    //     "ConflictFillReq:biu_cycle=%lu, m_ifu_cycle=%lu\n", m_biu_res_cycle, m_ifu_cycle);
    std::vector<uint64_t>::iterator it
        = find (m_fill_req_cycle.begin (), m_fill_req_cycle.end (), m_ifu_cycle);
    if (it != m_fill_req_cycle.end ()) {
        m_fill_req_cycle.erase (m_fill_req_cycle.begin (), it);
        return true;
    } else {
        return false;
    }
}


void CsIbufG3MH::CheckIbufRelease (uint64_t ifu_cycle)
{
    std::vector<IdPointer>::iterator it = m_id_pointer.begin ();
    while (it != m_id_pointer.end ()) {
        if ((*it).m_id_cycle <= ifu_cycle) {
            InvalidateIfq ((*it).m_id_addr, (*it).m_id_cycle,
                           (*it).m_need_copy, (*it).m_fill_order);
            it = m_id_pointer.erase (it);
        } else {
            ++ it;
        }
    }
}


// IA側についても２連発がありうるので、
// prefetchみたいにvectorで管理しないとダメ。
// PrefetchとIAのreqが近接すると、
// どうしてもIAが優先になってしまう。
// Prefecthは後から出るのに先に処理される
// (IAがIAから始まって処理されるのは３cycle後、PはBIUにすぐでるから)
// PrefetchもBIUー３のタイミング（IA相当）の位置で発行制御できない？
// ->BIU-1／-2のタイミングでキャンセルされると困る。
//　 結局、できないか。

void CsIbufG3MH::SimFetchIa (PhysAddr inst_addr, TraceCtrl* trace)
{
    uint64_t limit_cycle = std::max (m_ifu_cycle, m_id_cycle) + 100000;

    //while ((m_ifu_cycle <= ia_cycle) && (inst_addr >= m_ia_addr)) {
    while (   ((inst_addr >= m_ia_addr) || (m_branch_cycle >= m_ifu_cycle))
           && (limit_cycle >= m_ifu_cycle)) {
        //CfMsg::TPrint (MSG_INF, m_trace,
        //    "SimFetchIa:ifu_cycle=%lu, branch_cycle=%lu, ia_addr=%08x, inst_addr=%08x\n",
        //    m_ifu_cycle, m_branch_cycle, m_ia_addr, inst_addr);
        CheckIbufRelease (m_ifu_cycle);
        if (m_branch_cycle == m_ifu_cycle) {
            // Branch executed. Update ia address.
            m_ia_addr = m_branch_tgt_ia;
            m_branch_cycle = 0x0ULL;

            // Invalidate current IFQ
            //ClearPrefetchBuffer (m_ia_addr);
            CancelFetch ();
            //CfMsg::TPrint (MSG_INF, m_trace, "CancelFetch\n");
        } else {
            //CfMsg::TPrint (MSG_INF, m_trace,
            //    "kita1:m_branch_cycle=%lu, m_ifu_cycle=%lu\n", m_branch_cycle, m_ifu_cycle);
        }
        SimFetchStep (m_ia_addr, trace);
        ++ m_ifu_cycle;

        // @@ FillReqとぶつかったらIAを1cycleストール
        if (ConflictFillReq ()) {
            //CfMsg::TPrint (MSG_INF, m_trace, "Conflict req:cycle=%lu\n", m_ifu_cycle);
            CheckIbufRelease (m_ifu_cycle);
            if (m_branch_cycle == m_ifu_cycle) {
                // Branch executed. Update ia address.
                m_ia_addr = m_branch_tgt_ia;
                m_branch_cycle = 0x0ULL;

                // Invalidate current IFQ
                //ClearPrefetchBuffer (m_ia_addr);
                CancelFetch ();
            } else {
                if (IsBiuUsable () && CanIssueReq (m_ifu_cycle, 2)) {
                    //CfMsg::TPrint (MSG_INF, m_trace, "SimPrefetch\n");
                    SimPrefetch (m_ia_addr);
                }
            }
            ++ m_ifu_cycle; // @@ ia_cycle < m_ifu_cycleの場合を除外しなくてよいか？
        }
#ifdef DEBUG_CSIBUF
        if (limit_cycle < m_ifu_cycle) {
            CfMsg::TPrint (MSG_ERR, m_trace,
            "<Internal Error: "
            "DeadLock at SimFetchIa:m_ifu_cycle=%" PRIu64 ", m_ia_addr=%08x, inst_addr=%08x\n",
            m_ifu_cycle, m_ia_addr, inst_addr);
        }
#endif // DEBUG_CSIBUF
    }
    //CfMsg::TPrint (MSG_INF, m_trace,
    //    "End: SimFetchIa:m_ifu_cycle=%lu, m_ia_addr=%08x, inst_addr=%08x\n",
    //    m_ifu_cycle, m_ia_addr, inst_addr);
    //DumpInstBuf ();
}


void CsIbufG3MH::ClearUnusedIdbuf (PhysAddr addr)
{
    if (m_idbuf [IDBUF_READ].m_addr != addr) {
        m_idbuf [IDBUF_READ].m_addr = 0x7;
        m_idbuf [IDBUF_READ].m_base_addr = 0x7;
    }
    if (m_idbuf [IDBUF_SKID].m_addr != addr) {
        m_idbuf [IDBUF_SKID].m_addr = 0x7;
        m_idbuf [IDBUF_SKID].m_base_addr = 0x7;
    }
}


void CsIbufG3MH::ClearUnusedIdbuf (PhysAddr addr1, PhysAddr addr2)
{
    if ((m_idbuf [IDBUF_READ].m_addr != addr1) && (m_idbuf [IDBUF_READ].m_addr != addr2)) {
        m_idbuf [IDBUF_READ].m_addr = 0x7;
        m_idbuf [IDBUF_READ].m_base_addr = 0x7;
    }
    if ((m_idbuf [IDBUF_SKID].m_addr != addr1) && (m_idbuf [IDBUF_SKID].m_addr != addr2)) {
        m_idbuf [IDBUF_SKID].m_addr = 0x7;
        m_idbuf [IDBUF_SKID].m_base_addr = 0x7;
    }
}


void CsIbufG3MH::ClearIdbuf (PhysAddr addr)
{
    if (m_idbuf [IDBUF_READ].m_addr == addr) {
        m_idbuf [IDBUF_READ].m_addr = 0x7;
        m_idbuf [IDBUF_READ].m_base_addr = 0x7;
    }
    if (m_idbuf [IDBUF_SKID].m_addr == addr) {
        m_idbuf [IDBUF_SKID].m_addr = 0x7;
        m_idbuf [IDBUF_SKID].m_base_addr = 0x7;
    }
}


void CsIbufG3MH::InvalidateUnusedIbuf (void)
{
    if (m_branch_cycle > m_ifu_cycle) {
        // 自分は分岐ターゲット
        // 以前のIFQは不要
        std::vector <IbufType*>::iterator it = m_ibuf.begin ();
        while (it != m_ibuf.end ()) {
            (*it)->m_is_valid = false;
            //(*it)->addr = 0x5; // 決してヒットしない値に設定
            ++ it;
        }
    }
}


void CsIbufG3MH::SimFetch (TraceCtrl* trace)
{
    //InvalidateUnusedIbuf ();
    m_trace = trace;

    PhysAddr inst_addr = trace->GetInstAddr ();
    ByteSize inst_size = trace->GetInstSize ();
    PhysAddr inst_end_addr = inst_addr + (inst_size - 1);
    PhysAddr inst_addr_base = inst_addr & 0xFFFFFFF8;
    PhysAddr inst_end_addr_base = inst_end_addr & 0xFFFFFFF8;
    SimFetchIa (inst_addr, trace);
    if (inst_addr_base != inst_end_addr_base) {
        SimFetchIa (inst_end_addr_base, trace);
    }
    // id_bufの更新
    if (inst_addr_base == inst_end_addr_base) {
        // skid バッファの無効化
        m_idbuf [IDBUF_SKID].m_addr = 0x7;
        m_idbuf [IDBUF_SKID].m_base_addr = 0x7;
        ClearUnusedIdbuf (inst_addr_base);
    } else { //(inst_addr_base != inst_end_addr_base) {
        // skid バッファは使用されるので、クリアしない
        ClearUnusedIdbuf (inst_addr_base, inst_end_addr_base);
    }

    // キャンセル処理のため、使用された最新のアドレスとFILL順番番号を記録
    m_last_executed_addr = inst_end_addr_base;
    m_last_executed_order = m_fill_order;
}


uint64_t CsIbufG3MH::GetFetchCycle (TraceCtrl* trace)
{
    PhysAddr inst_addr = trace->GetInstAddr ();
    ByteSize inst_size = trace->GetInstSize ();
    PhysAddr inst_end_addr = inst_addr + (inst_size - 1);
    PhysAddr inst_addr_base = inst_addr & 0xFFFFFFF8;
    PhysAddr inst_end_addr_base = inst_end_addr & 0xFFFFFFF8;

    uint64_t fetch_cycle = GetFetchCycleInstBuf (inst_addr_base);
    //uint64_t fetch_cycle = GetFetchCycleInstBuf (inst_addr);
    if (inst_addr_base != inst_end_addr_base) {
        fetch_cycle = std::max (fetch_cycle, GetFetchCycleInstBuf (inst_end_addr_base));
    }
    return fetch_cycle;
}


void CsIbufG3MH::UpdateIdAddress (PhysAddr id_addr, uint64_t id_cycle)
{
    //CfMsg::TPrint (MSG_INF, m_trace,
    //               "UpdateIdAddress:id_addr=%08x, id_cycle=%lu\n", id_addr, id_cycle);
    id_addr &= 0xFFFFFFF8UL;
    if (m_id_addr != id_addr) {
        uint64_t fill_order = m_last_executed_order;
        if (id_addr == m_last_executed_addr) {
            fill_order = m_last_executed_order;
        } else {
            fill_order = m_last_executed_order - 1;
        }
        m_id_pointer.push_back (IdPointer (id_addr, id_cycle, fill_order));
        //CfMsg::TPrint (MSG_INF, m_trace,
        //        "UpdateIdAddress: New id_addr=%08x, id_cycle=%lu, fill_order=%lu\n",
        //        id_addr, id_cycle, fill_order);
        m_id_addr = id_addr;
        m_id_cycle = id_cycle;
    }
}


void CsIbufG3MH::UpdateIaAddress (PhysAddr ia_addr, uint64_t ia_cycle)
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


void CsIbufG3MH::ForceUpdateIaAddress (PhysAddr ia_addr)
{
    m_ia_addr = ia_addr & 0xFFFFFFF8U;
    m_branch_tgt_ia = ia_addr & 0xFFFFFFF8U;
    m_branch_cycle = m_ifu_cycle;
}


// 命令バッファのLRUを更新
void CsIbufG3MH::UpdateInstBufLru (uint32_t entry)
{
    // @@ G3MHではLRUはないはず。単なるFILLオーダー順のはず。erase+push_backと同じ
    // 指定エントリを最後に移動。
    std::vector <IbufType*>::iterator it = m_ibuf.begin () + entry;
    if (it != m_ibuf.end ()) {
        rotate (it, it + 1, m_ibuf.end ());
    }
    //CfMsg::TPrint (MSG_INF, m_trace, "UpdateInstBufLru: replased_entry=%u\n", entry);
}


// FSからCSに切り替わった際、FS命令キュー内容をCS命令バッファに書き込む
// FS側で命令キューからフェッチされた場合、
// CS側命令バッファに存在しない命令アドレスが登場してしまうので。
void CsIbufG3MH::ForceSetInstBuf (PhysAddr addr)
{
    UpdateIaAddress (addr , 0);
}


CsIbufG3MH::CsIbufG3MH (uint32_t pe_id,
                        CoreType core_type,
                        FsCtrl* fsctrl,  CsCtrl* csctrl)
         : m_pe_id (pe_id),
           m_core_type (core_type),
           m_fsctrl (fsctrl),  m_csctrl (csctrl)
{
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        m_ibuf.push_back (new IbufType ());
    }
    m_clk_ratio = 1;

    InitInstBuf ();
}


CsIbufG3MH::~CsIbufG3MH ()
{
    std::vector<IbufType*>::iterator it = m_ibuf.begin ();
    while (it != m_ibuf.end ()) {
        delete (*it);
        ++ it;
    }
}


#if defined(_MSC_VER)
inline div_t div (uint32_t num, uint32_t denom)
{
    return div (static_cast<int> (num), static_cast<int> (denom));
}
#endif // _MSC_VER
