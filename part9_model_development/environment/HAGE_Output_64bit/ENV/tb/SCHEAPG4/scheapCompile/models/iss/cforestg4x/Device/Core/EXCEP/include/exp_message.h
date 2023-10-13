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
#include "./forest_common.h"


class TraceCtrl;


class ExpMsg {
private:
    static const uint32_t MAX_EXP_CODE = 0xFFFFU;
    static const uint32_t MASK_EXP_CODE = 0xFFFFU;
    class ExpMsgRange {
    public:
        ExpCause m_s;
        ExpCause m_e;
        bool IsOverlap (const ExpMsgRange& x) const
            { return   (m_s <= x.m_s && x.m_s <= m_e)
                    || (m_s <= x.m_e && x.m_e <= m_e)
                    || (m_s >  x.m_s && x.m_e >  m_e); }
        bool operator< (const ExpMsgRange& x) const { return m_s < x.m_s; }
        bool operator()(const ExpMsgRange& x) const { return x.m_s <= m_s && m_e <= x.m_e; }
        ExpMsgRange (ExpCause s, ExpCause e) : m_s (s), m_e (e) {}
        explicit ExpMsgRange (ExpCause x) : m_s (x), m_e (x) {}
    };

    typedef std::vector<ExpMsgRange> ExpMsgVec;
    ExpMsgVec m_exp_msg_vec;

    void AddExpMsgRange (ExpCause start_cause, ExpCause end_cause);
    void DelExpMsgRange (ExpCause start_cause, ExpCause end_cause);
    bool IsExpMsgRange (ExpCause exp_cause) const;
    void ShowExpMsgRange (uint32_t peid, ContextId tcid) const;

public:
    void UpdateExpMsgRange (uint32_t peid, ContextId tcid,
                            ExpCause start_cause, ExpCause end_cause, bool is_enable);
    void ExpMsgPrint (ExpCode exp_code, ExpCause exp_cause, TraceCtrl* trace) const;

    explicit ExpMsg () { AddExpMsgRange (0x0, MAX_EXP_CODE); }
    ~ExpMsg () {}
};
