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
#include "./config.h"
#include "./forest_common.h"
#include "./breakpoint.h"
#include "./trace_info.h"
#include "./inst_id_list.h"
#include "./forest_internals.h"
#include "./forest_message.h"

bool BreakPoint::IsHitPreExec (void)
{
    const TraceCtrl* trace = m_trace;

#if defined(SUPPORT_SC_HEAP)
    // 実行完了していなければ、ブレークしない
    if (trace->GetSuspendState () != NO_SUSPEND) {
        return false;
    }
#endif // defined(SUPPORT_SC_HEAP)

    if ( unlikely (m_enable_brk_next_pc == true) ) {
        bool is_hit = IsHitBreakNextPc (trace);
        if ( unlikely (is_hit == true) ) { return true; }
    }

    return false;
}


#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
bool BreakPoint::IsHitPostExec (void)
{
    const TraceCtrl* trace = m_trace;

    // 実行完了していなければ、ブレークしない
    if (trace->GetSuspendState () != NO_SUSPEND) {
        return false;
    }

    // On SCHEAP, because multiple events occur in same time, check all event
    bool is_ecm_hit = IsHitBreakEcm (trace);

    bool is_hit_acc = false;
    if ( unlikely (m_enable_brk_access == true) ) {
        is_hit_acc = IsHitBreakAccess (trace);
    }

    bool is_hit_matchpc = false;
    if ( unlikely (m_matchpc_set.empty () == false) ) {
        // PC一致時にブレークする for SecureWDT
        is_hit_matchpc = IsMatchPc (trace);
    }

    // SCHEAPでは割り込み受付時にStepExecuteから抜ける
    bool is_hit_int = IsHitBreakInt (trace);

    bool is_hit_pswh = IsHitBreakPswhChanged (trace);

    bool is_hit_any = is_ecm_hit | is_hit_acc | is_hit_matchpc | is_hit_int | is_hit_pswh;
    if ( unlikely (is_hit_any == true) ) { return true; }

    return false;
}

#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

bool BreakPoint::IsHitPostExec (void)
{
    const TraceCtrl* trace = m_trace;

#if defined(SUPPORT_SC_HEAP)
    // For CT of SCHEAP dummy lib
    // 実行完了していなければ、ブレークしない
    if (trace->GetSuspendState () != NO_SUSPEND) {
        return false;
    }
#endif // defined(SUPPORT_SC_HEAP)

#ifdef ENABLE_SAME_PC_BREAK
    bool is_hit_same_pc = false;
    if ( unlikely (m_is_enabled_same_pc_max == true) ) {
        is_hit_same_pc = IsHitBreakSamePcMax (trace);
    }
    if ( unlikely (is_hit_same_pc == true) ) { return true; }
#endif // #ifdef ENABLE_SAME_PC_BREAK

    if ( unlikely (m_enable_brk_access == true) ) {
        bool is_hit = IsHitBreakAccess (trace);
        if ( unlikely (is_hit == true) ) { return true; }
    }

    {
        // Hit判定が軽量 なのでenable判定はしない
        bool is_hit = IsHitBreakMax (trace);
        if ( unlikely (is_hit == true) ) { return true; }
    }
    return false;
}
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)


bool BreakPoint::IsHitBreakNextPc (const TraceCtrl* trace)
{
    PhysAddr next_inst_addr = trace->GetNextInstAddr ();
    BreakNextPcVec::const_iterator it = m_brknextpc_vec.begin ();
    while (it != m_brknextpc_vec.end() ) {
        PhysAddr bp_addr = it->first;
        uint32_t mask = it->second;

        if ((next_inst_addr & mask) == (bp_addr & mask)) {
            // ブレーク条件に一致
            if (trace->GetSimState () != SIMULATE_ON) {
                // 実際に実行されるのは後なので、
                // Snooze/HALT中には次命令ブレークを無効化
                break;
            }

#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
            CfMsg::TPrint (MSG_ERR, trace, "<PC Break (Before execution) at 0x%08x>\n",
                           next_inst_addr);
#endif // SUPPORT_SC_HEAP
            p_internals->NotifyHitBreak (BREAK_PC, next_inst_addr);
            m_break_pc.push_back (next_inst_addr);
            return true;
        }
        ++ it;
    }

    // ブレーク条件に一致しなかった
    return false;
}


bool BreakPoint::IsHitBreakAccess (const TraceCtrl* trace)
{
    // リードアクセスチェック
    uint32_t read_count = trace->GetRmemCount();
    for (uint32_t i = 0; i < read_count; ++i) {
        PhysAddr check_addr = trace->GetRmemAddr (i);
        BreakAccVec::const_iterator it = m_brkacc_r_vec.begin ();
        while (it != m_brkacc_r_vec.end ()) {
            PhysAddr masked_addr = check_addr & (*it).m_mask;
            if ( ((*it).m_masked_s <= masked_addr) && (masked_addr <= (*it).m_masked_e) ) {
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
                CfMsg::TPrint (MSG_ERR, trace,
                               "<Access Break (Read from 0x%08x) at 0x%08x>\n",
                               check_addr, trace->GetInstAddr());
#endif // SUPPORT_SC_HEAP
                p_internals->NotifyHitBreakAcc (BREAK_ACC_READ, trace->GetInstAddr (),
                                                check_addr, trace->GetRmemSize (i));
                m_break_acc_r.push_back (check_addr);
                return true;
            }
            ++ it;
        }
    }

    // ライトアクセスチェック
    uint32_t write_count = trace->GetWmemCount();
    for (uint32_t i = 0; i < write_count; ++i) {
        PhysAddr check_addr = trace->GetWmemAddr (i);

        BreakAccVec::const_iterator it = m_brkacc_w_vec.begin ();
        while (it != m_brkacc_w_vec.end ()) {
            PhysAddr masked_addr = check_addr & (*it).m_mask;
            if ( ((*it).m_masked_s <= masked_addr) && (masked_addr <= (*it).m_masked_e) ) {
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
                CfMsg::TPrint (MSG_ERR, trace,
                               "<Access Break (Write to 0x%08x) at 0x%08x>\n",
                               check_addr, trace->GetInstAddr());
#endif // SUPPORT_SC_HEAP
                p_internals->NotifyHitBreakAcc (BREAK_ACC_WRITE, trace->GetInstAddr (),
                                                check_addr, trace->GetWmemSize (i));
                m_break_acc_w.push_back (check_addr);
                return true;
            }
            ++ it;
        }
    }

    return false;
}


bool BreakPoint::IsHitBreakMax (const TraceCtrl* trace) const
{
    if (trace->GetStepCount() >= m_brk_max_step) {
        CfMsg::TPrint (MSG_ERR, trace, "<Max Break at H'%08x>\n", trace->GetInstAddr());
        p_internals->NotifyHitBreak (BREAK_MAX, trace->GetInstAddr());
        return true;
    } else {
        return false;
    }
}


bool BreakPoint::IsHitBreakSamePcMax (const TraceCtrl* trace)
{
    if (trace->GetInstAddr() == trace->GetNextInstAddr ()) {
        ++ m_same_pc_count;
    } else {
        m_same_pc_count = 0;
    }

    if (m_same_pc_count >= m_same_pc_threashold) {
        CfMsg::TPrint (MSG_ERR, trace, "<Max Same PC Break at H'%08x>\n", trace->GetInstAddr());
        p_internals->NotifyHitBreak (BREAK_MAX, trace->GetInstAddr());
        return true;
    } else {
        return false;
    }
}

#ifdef SUPPORT_SC_HEAP
bool BreakPoint::IsHitBreakPswhChanged (const TraceCtrl* trace) const
{
    // When PSWH.GM or GPID is changed,
    // return from StepExecute to give control to SC-HEAP (To handle INTC updateing in SystemC side).
    if (trace->IsPswhChanged ()) {
        p_internals->NotifyHitBreak (BREAK_CHANGE_PSWH, trace->GetInstAddr());
        return true;
    }
    return false;
}


bool BreakPoint::IsHitBreakInt (const TraceCtrl* trace) const
{
    // SCHEAPでは割り込み受付時にStepExecuteから抜ける
    if (trace->GetBranchInfo () == INT_BRANCH) {
        p_internals->NotifyHitBreak (BREAK_ACCEPT_INT, trace->GetInstAddr());
        return true;
    }
    return false;
}


bool BreakPoint::IsHitBreakEcm (const TraceCtrl* trace) const
{
    // SCHEAPではECM通知時（ICUMへの割り込み通知含む)にStepExecuteから抜ける
    GuardErrType err = trace->GetGuardError ();
    if (err != GD_NONE_ERR) {
        p_internals->NotifyBreakEcm (err, trace->GetInstAddr ());
        return true;
    }
    return false;
}


// PC一致検出ブレーク(実行後)
// - SC-HEAPからSecureWDT用に依頼
bool BreakPoint::IsMatchPc (const TraceCtrl* trace) const
{
    PhysAddr inst_addr = trace->GetInstAddr ();
    MatchPcSet::const_iterator it = m_matchpc_set.find (inst_addr);
    bool is_hit = ( it != m_matchpc_set.end() );

    if ( is_hit ) {
        p_internals->NotifyMatchPc (inst_addr);
    }

    return is_hit;
}


void BreakPoint::AddMatchPc (PhysAddr address)
{
    m_matchpc_set.insert (address);
}


void BreakPoint::ClearMatchPc (PhysAddr address)
{
    m_matchpc_set.erase (address);
}
#endif // SUPPORT_SC_HEAP


void BreakPoint::AddBreakNextPc (PhysAddr address, uint32_t mask)
{
    m_enable_brk_next_pc = true;
    m_brknextpc_vec.push_back (BreakNextPcEntry(address, mask));
}


void BreakPoint::DisplayBreakPc (void)
{
    BreakNextPcVec::const_iterator it = m_brknextpc_vec.begin ();

    while (it != m_brknextpc_vec.end() ) {
        int k = 0;
        std::vector<PhysAddr>::const_iterator it_bp = m_break_pc.begin ();
        while (it_bp != m_break_pc.end ()) {
            if (it->first == *it_bp) {
                k ++;
            }
            ++ it_bp;
        }
        CfMsg::DPrint (MSG_ERR, "<break point at 0x%08x hits %u times>\n", it->first, k);
        ++ it;
    }
}


void BreakPoint::ClearBreakNextPc (PhysAddr address, uint32_t mask)
{
    BreakNextPcVec::iterator it =
    std::find (m_brknextpc_vec.begin (), m_brknextpc_vec.end (), BreakNextPcEntry(address, mask));
    if (it != m_brknextpc_vec.end ()) {
        m_brknextpc_vec.erase (it);
    }

    if (m_brknextpc_vec.empty() == true) {
        m_enable_brk_next_pc = false;
    }
}


void BreakPoint::AddBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask)
{
    m_enable_brk_access = true;

    AccessRange new_range (start, end, mask);
    if (attr != ACC_READ) {
        BreakAccVec::const_iterator it =
        std::find (m_brkacc_w_vec.begin (), m_brkacc_w_vec.end (), new_range);
        if ( it == m_brkacc_w_vec.end() ) {
            // Miss: 存在しない -> 新規登録
            m_brkacc_w_vec.push_back (new_range);
        }
    }
    if (attr != ACC_WRITE) {
        BreakAccVec::const_iterator it =
        std::find (m_brkacc_r_vec.begin (), m_brkacc_r_vec.end (), new_range);
        if ( it == m_brkacc_r_vec.end() ) {
            // Miss: 存在しない -> 新規登録
            m_brkacc_r_vec.push_back (new_range);
        }
    }
}


void BreakPoint::DisplayBreakAccess (void)
{
    BreakAccVec::const_iterator it;
    it = m_brkacc_r_vec.begin ();
    while (it != m_brkacc_r_vec.end ()) {
        int k = 0;
        std::vector<PhysAddr>::const_iterator it_bp = m_break_acc_r.begin ();
        while (it_bp != m_break_acc_r.end ()) {
            if (it->m_s == *it_bp) {
                k ++;
            }
            ++ it_bp;
        }
        CfMsg::DPrint (MSG_ERR, "<break access (read) at 0x%08x hits %u times>\n", it->m_s, k);
        ++ it;
    }

    it = m_brkacc_w_vec.begin ();
    while (it != m_brkacc_w_vec.end ()) {
        int k = 0;
        std::vector<PhysAddr>::const_iterator it_bp = m_break_acc_w.begin ();
        while (it_bp != m_break_acc_w.end ()) {
            if (it->m_s == *it_bp) {
                k ++;
            }
            ++ it_bp;
        }
        CfMsg::DPrint (MSG_ERR, "<break access (write) at 0x%08x hits %u times>\n", it->m_s, k);
        ++ it;
    }
}


void BreakPoint::ClearBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask)
{
    AccessRange clear_range (start, end, mask);
    if (attr != ACC_READ) {
        BreakAccVec::iterator it =
        std::find (m_brkacc_w_vec.begin (), m_brkacc_w_vec.end (), clear_range);
        if ( it != m_brkacc_w_vec.end() ) {
            m_brkacc_w_vec.erase (it);
        }
    }
    if (attr != ACC_WRITE) {
        BreakAccVec::iterator it =
        std::find (m_brkacc_r_vec.begin (), m_brkacc_r_vec.end (), clear_range);
        if ( it != m_brkacc_r_vec.end() ) {
            m_brkacc_r_vec.erase (it);
        }
    }

    if ((m_brkacc_w_vec.empty() == true) && (m_brkacc_r_vec.empty() == true)) {
        m_enable_brk_access = false;
    }
}


void BreakPoint::SetBreakMax (uint64_t max_step)
{
    m_brk_max_step = max_step;
}


uint64_t BreakPoint::GetBreakMax (void)
{
    return m_brk_max_step;
}


void BreakPoint::SetBreakSamePcMax (uint64_t max_step)
{
    m_is_enabled_same_pc_max = true;
    m_same_pc_threashold = max_step;
}


uint64_t BreakPoint::GetBreakSamePcMax (void) const
{
    return m_same_pc_threashold;
}
