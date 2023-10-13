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
#include "./cs_lsu_buf.h"
#include "./forest_message.h"
#include "./cs_controler.h"


bool CsLsuBuf::IsWaitSCHEAP (void) const
{
    return GetFinishCycle () == WAIT_SCHEAP;
}


uint64_t CsLsuBuf::GetFinishCycle (void) const
{
    // responce time of LSU is stored
    return m_buf_avail_cycle.back ();
}


uint64_t CsLsuBuf::GetAmuAvailCycle (void) const
{
    // request time of AMU is stored
    return m_buf_avail_cycle [m_num_lsu_buf];
}


uint64_t CsLsuBuf::GetSyncCycle (void) const
{
#ifdef SUPPORT_SC_HEAP
    // SYNCE/SYNCP/SYNCMのレイテンシ待ち
    // FS側IsReadLatencyReceivedFsで、SYNCに対応するSCHEAPレイテンシ情報は全て記録済み。
    // ただし、SYNCP/SYNCEではWriteを待たないので、Writeに対応するSCHEAPレイテンシは未記録。
    // 何もケアしないとWAIT_SCHEAP(0xffff....)が取得されてしまう。
    // SYNC非対象のWriteがWAIT_SCHEAPなので、
    // WAIT_SCHEAPを除いた最大レイテンシを返せばOK。
    std::vector<uint64_t>::const_iterator it = m_buf_avail_cycle.end ();
    while (it != m_buf_avail_cycle.begin ()) {
        -- it;
        if (*it != WAIT_SCHEAP) {
            return *it;
        }
    }

    // SCHEAPレイテンシしか存在しない、または、レイテンシ記録がempty
    return 0;
#else // SUPPORT_SC_HEAP
    return GetFinishCycle ();
#endif // SUPPORT_SC_HEAP
}


// バッファが空く時間を返す
uint64_t CsLsuBuf::GetBuffAvailCycle (void) const
{
    return m_buf_avail_cycle.front ();
}


uint64_t CsLsuBuf::GetBuffAvailCycleG3MH (bool is_parallel) const
{
    // RTLではLS命令バッファが２段あくまで、Ready=1にならない
    if (!is_parallel) {
        return m_buf_avail_cycle[0];
    } else {
        return m_buf_avail_cycle[1];
    }
}


uint64_t CsLsuBuf::GetBuffAvailCycleG3MH_LD (bool is_parallel) const
{
    // RTLではLS命令バッファが２段あくまで、Ready=1にならない
    if (!is_parallel) {
        return m_buf_avail_cycle[0 + NUM_LSU_BUF_ST_G4MH];
    } else {
        return m_buf_avail_cycle[1 + NUM_LSU_BUF_ST_G4MH];
    }
}


uint64_t CsLsuBuf::GetBuffAvailCycleG3KH (void) const
{
    return m_buf_avail_cycle.front ();
}


// 外バスへのリクエストが可能なcycle
uint64_t CsLsuBuf::GetReqAvailCycle (void) const
{
    // バスのキューがいっぱいで、内部でリクエストが滞留するcycle
    if (m_total_buf > m_num_lsu_buf) {
        return std::max (m_issue_cycle, m_buf_avail_cycle[m_num_lsu_buf]);
    } else {
        return std::max (m_issue_cycle, GetFinishCycle ());
    }
}


void CsLsuBuf::SetIssueCycle (uint64_t issue_cycle)
{
    // LRAM@ownPE : 2 access/cycle
    // CRAM : 2 access/cycle
    // FlahsROM : 2 access/cycle
    // LRAM@otherPE, peripherals : 1 access/cycle
    m_issue_cycle = std::max (issue_cycle, m_issue_cycle + 1);
}


void CsLsuBuf::ReserveBuffer (uint64_t release_cycle)
{
    // Release is 2cycle (1 : WB, 2: AMU releasse) later in G4MH
    // Release is InO
    if (release_cycle <= m_last_release_cycle) {
        release_cycle = m_last_release_cycle;
        if (m_is_used_1slot) {
            ++ m_last_release_cycle;
            m_is_used_1slot = false;
        } else {
            m_is_used_1slot = true;
        }
    } else {
        m_last_release_cycle = release_cycle;
        m_is_used_1slot = true;
    }

    // アクセス履歴更新
    // サイズは常にoutstanding数
    if (m_buf_avail_cycle.back () <= release_cycle) {
        // in-orderでアクセス完了
        // - 単純に最終要素に記録すれば良い。
        m_buf_avail_cycle.erase (m_buf_avail_cycle.begin ());
        m_buf_avail_cycle.push_back (release_cycle);
    } else {
        // アクセス完了の追い越しが発生
        // - キューの途中に挿入するため、並び替えが必要
        m_buf_avail_cycle.front () = release_cycle;
        std::sort (m_buf_avail_cycle.begin(), m_buf_avail_cycle.end());
    }

#ifdef DEBUG_CS_LSU
    DumpOutstandQueue ();
#endif // DEBUG_CS_LSU
}


void CsLsuBuf::ReserveBufferG3KH (uint64_t release_cycle)
{
    // アクセス履歴更新
    // サイズは常にoutstanding数
    if (m_buf_avail_cycle.back () <= release_cycle) {
        // in-orderでアクセス完了
        // - 単純に最終要素に記録すれば良い。
        m_buf_avail_cycle.erase (m_buf_avail_cycle.begin ());
        m_buf_avail_cycle.push_back (release_cycle);
    } else {
        // アクセス完了の追い越しが発生
        // - キューの途中に挿入するため、並び替えが必要
        m_buf_avail_cycle.front () = release_cycle;
        std::sort (m_buf_avail_cycle.begin(), m_buf_avail_cycle.end());
    }

    //DumpOutstandQueue ();
}


void CsLsuBuf::ReserveBufferScheap (void)
{
    // アクセス履歴更新
    // サイズは常にoutstanding数
    // SCHEAPレイテンシは最終要素に記録。
    m_buf_avail_cycle.erase (m_buf_avail_cycle.begin ());
    static const uint64_t wait_scheap = WAIT_SCHEAP;
    m_buf_avail_cycle.push_back (wait_scheap);

#ifdef DEBUG_CS_LSU
    DumpOutstandQueue ();
#endif // DEBUG_CS_LSU
}

void CsLsuBuf::ReleaseScheapWaiting (uint64_t release_cycle)
{
    // SCHEAP用の予約エントリはベクタの最終エントリ。
    // - キューの途中に挿入するため、並び替えが必要
    m_buf_avail_cycle.back () = release_cycle;
    std::sort (m_buf_avail_cycle.begin(), m_buf_avail_cycle.end());

#ifdef DEBUG_CS_LSU
    DumpOutstandQueue ();
#endif // DEBUG_CS_LSU
}


void CsLsuBuf::SetNotifiedLatencyScheap (uint64_t release_cycle)
{
    // SCHEAPからのNotifyによるレイテンシを反映
    if (IsWaitSCHEAP () == false) {
        // SCHEAPレイテンシがLSUバッファに存在しない
        CfMsg::DPrint (MSG_ERR, "<Internal Error: Can't find SC-HEAP entry in LSU buffer>\n");
        return;
    }

    // SCHEAP用の予約エントリはベクタの最終エントリ。
    // - キューの途中に挿入するため、並び替えが必要
    release_cycle += 2;
    ReleaseScheapWaiting (release_cycle);
}


void CsLsuBuf::DumpOutstandQueue (void) const
{
    int i = 0;
    CfMsg::DPrint (MSG_ERR, "Dump lsu outstand queue (depth=%d)\n",
                   static_cast<uint32_t> (m_buf_avail_cycle.size()));
    std::vector<uint64_t>::const_iterator it = m_buf_avail_cycle.begin ();
    while (it != m_buf_avail_cycle.end ()) {
        CfMsg::DPrint (MSG_ERR, "[%d]=%08" PRIu64 "\n", i, *it);
        ++ it;
        ++ i;
    }
}


void CsLsuBuf::InitBufCycle (void)
{
    m_buf_avail_cycle.clear ();
    if (m_total_buf > m_num_lsu_buf) {
        m_buf_avail_cycle.assign (m_total_buf, 0);
    } else {
        // エラー：NBのサイズ＝０は非サポート。
        // ここには来ない
        // m_buf_avail_cycle参照時に範囲外アクセスになることを防ぐため補正
        m_buf_avail_cycle.assign (m_num_lsu_buf + 1, 0);
    }
    m_issue_cycle = 0;
    m_last_release_cycle = 0;
    m_is_used_1slot = false;
}


CsLsuBuf::CsLsuBuf (uint32_t num_lsu_buf, uint32_t num_nb)
: m_num_lsu_buf (num_lsu_buf + NUM_LSU_BUF_ST_G4MH), m_total_buf (num_nb + num_lsu_buf + NUM_LSU_BUF_ST_G4MH)
{
    if (num_nb == 0) {
        // num_nb = 0の構成は未対応。
        // 対応時にはGetReqAvailCycleで配列範囲外になるのでケア必要。
        CfMsg::DPrint (MSG_ERR, "<Error: size of NB is 0>\n");
    }

    InitBufCycle ();
}


CsLsuBuf::~CsLsuBuf ()
{
}
