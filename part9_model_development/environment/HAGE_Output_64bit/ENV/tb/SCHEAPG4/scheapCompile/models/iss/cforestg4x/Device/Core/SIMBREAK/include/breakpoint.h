/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <vector>
#include <map>
#include <set>
#include <utility>

class TraceCtrl;


class BreakPoint {
public:
    bool IsHitPreExec (void);
    bool IsHitPostExec (void);
    void AddBreakNextPc (PhysAddr address, uint32_t mask);
    void DisplayBreakPc (void);
    void ClearBreakNextPc (PhysAddr address, uint32_t mask);
    void AddBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask);
    void DisplayBreakAccess (void);
    void ClearBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask);
    void SetBreakMax (uint64_t count);
    uint64_t GetBreakMax (void);
    void SetBreakSamePcMax (uint64_t count);
    uint64_t GetBreakSamePcMax (void) const;
    void SetTraceCtrl (TraceCtrl* trace) { m_trace = trace; }

#ifdef SUPPORT_SC_HEAP
    void AddMatchPc (PhysAddr address);
    void ClearMatchPc (PhysAddr address);
#endif // SUPPORT_SC_HEAP

    explicit BreakPoint (TraceCtrl* trace) :
        m_trace (trace),
        m_enable_brk_next_pc (false),
        m_enable_brk_access (false),
        m_brk_max_step (~0x0UL),
        m_is_enabled_same_pc_max (false),
        m_same_pc_threashold (0)
        { Reset(); }
    ~BreakPoint () {}

    void Reset(void)
    {
        // initialize history record
        m_same_pc_count = 0;
    }

private:
    TraceCtrl* m_trace;
    bool m_enable_brk_next_pc;
    bool m_enable_brk_access;

    std::vector<PhysAddr> m_break_acc_r;
    std::vector<PhysAddr> m_break_acc_w;
    std::vector<PhysAddr> m_break_pc;

    uint64_t m_brk_max_step;

    typedef std::pair<PhysAddr, uint32_t> BreakNextPcEntry;
    typedef std::vector<BreakNextPcEntry> BreakNextPcVec;
    BreakNextPcVec m_brknextpc_vec;

    class AccessRange {
    public:
        PhysAddr m_s;
        PhysAddr m_e;
        uint32_t m_mask;
        PhysAddr m_masked_s;
        PhysAddr m_masked_e;

        bool operator== (const AccessRange& x) const
            { return (m_s == x.m_s && m_e == x.m_e && m_mask == x.m_mask); }
        AccessRange (PhysAddr s, PhysAddr e, uint32_t mask) {
            m_s = s;
            m_e = e;
            m_mask = mask;
            m_masked_s = s & mask;
            m_masked_e = e & mask;
        }
    };

    typedef std::vector<AccessRange> BreakAccVec;
    BreakAccVec m_brkacc_r_vec;
    BreakAccVec m_brkacc_w_vec;

    bool m_is_enabled_same_pc_max;
    uint64_t m_same_pc_threashold;
    uint64_t m_same_pc_count;

    bool IsHitBreakNextPc (const TraceCtrl* trace);
    bool IsHitBreakAccess (const TraceCtrl* trace);
    bool IsHitBreakMax (const TraceCtrl* trace) const;
    bool IsHitBreakSamePcMax (const TraceCtrl* trace);

#ifdef SUPPORT_SC_HEAP
    bool IsHitBreakPswhChanged (const TraceCtrl* trace) const;
    bool IsHitBreakInt (const TraceCtrl* trace) const;

    typedef std::set<PhysAddr> MatchPcSet;
    MatchPcSet m_matchpc_set;
    bool IsMatchPc (const TraceCtrl* trace) const;
    bool IsHitBreakEcm (const TraceCtrl* trace) const;
#endif // SUPPORT_SC_HEAP
};
