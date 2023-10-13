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
#include "./forest_common.h"
#include "./exp_info.h"
#include "./exp_message.h"
#include "./forest_message.h"


void ExpMsg::UpdateExpMsgRange (uint32_t peid, ContextId tcid,
                                ExpCause start_cause, ExpCause end_cause, bool is_enable)
{
    if (start_cause > end_cause ||
        start_cause > MAX_EXP_CODE   ||
        end_cause   > MAX_EXP_CODE) {
        // 指定対象外の設定は無視
        return;
    }

    if (is_enable == true) {
        AddExpMsgRange (start_cause, end_cause);
    } else {
        DelExpMsgRange (start_cause, end_cause);
    }

    std::sort (m_exp_msg_vec.begin(), m_exp_msg_vec.end());

    ShowExpMsgRange (peid, tcid);
}


void ExpMsg::AddExpMsgRange (ExpCause start_cause, ExpCause end_cause)
{
    ExpMsgVec::iterator it = m_exp_msg_vec.begin();
    while (it != m_exp_msg_vec.end()) {
        if (it->IsOverlap (ExpMsgRange (start_cause, end_cause)) == true) {
            break;
        }
        ++it;
    }

    if (it == m_exp_msg_vec.end()) {
        // overlap なし、追加
        m_exp_msg_vec.push_back (ExpMsgRange (start_cause, end_cause));
    } else {
        // overlapした要素を削除した上で、連結して再登録
        ExpCause new_start = std::min (start_cause, it->m_s);
        ExpCause new_end = std::max (end_cause, it->m_e);
        m_exp_msg_vec.erase (it);
        // overlap がなくなるまで繰り返す
        AddExpMsgRange (new_start, new_end);
    }
}


void ExpMsg::DelExpMsgRange (ExpCause start_cause, ExpCause end_cause)
{
    ExpMsgVec::iterator it = m_exp_msg_vec.begin();
    while (it != m_exp_msg_vec.end()) {
        if (it->IsOverlap (ExpMsgRange (start_cause, end_cause)) == true) {
            break;
        }
        ++it;
    }

    if (it == m_exp_msg_vec.end()) {
        // overlap なし、削除不要
        return;
    } else {
        // overlap
        ExpCause s = it->m_s;
        ExpCause e = it->m_e;
        if (start_cause <= s && end_cause >= e) {
            // 包含する： 対象を削除
            m_exp_msg_vec.erase (it);
        } else if (start_cause > s && end_cause < e) {
            // 包含される： 対象を削除し、残った両端を再登録
            m_exp_msg_vec.erase (it);
            AddExpMsgRange (s, start_cause - 1);
            AddExpMsgRange (end_cause + 1, e);
        } else if (start_cause > s && start_cause <= e) {
            // 後半に重複する： 対象を削除し、残った前半を再登録
            m_exp_msg_vec.erase (it);
            AddExpMsgRange (s, start_cause - 1);
        } else if (end_cause >= s && end_cause < e) {
            // 前半に重複する： 対象を削除し、残った後半を再登録
            m_exp_msg_vec.erase (it);
            AddExpMsgRange (end_cause + 1, e);
        }
        // overlap がなくなるまで繰り返す
        DelExpMsgRange (start_cause, end_cause);
    }
}


void ExpMsg::ShowExpMsgRange (uint32_t peid, ContextId tcid) const
{
    CfMsg::DPrint (MSG_ERR, "---- Exception Cause to Display [P%d", peid);
    if (tcid == NC) {
        CfMsg::DPrint (MSG_ERR, "NC] ----\n");
    } else {
        CfMsg::DPrint (MSG_ERR, "T%d] ----\n", tcid);
    }

    ExpMsgVec::const_iterator it = m_exp_msg_vec.begin();
    while (it != m_exp_msg_vec.end()) {
        CfMsg::DPrint (MSG_ERR, "  %04x - %04x\n", it->m_s, it->m_e);
        ++it;
    }
}


bool ExpMsg::IsExpMsgRange (ExpCause exp_cause) const
{
    ExpMsgVec::const_iterator it = std::find_if (m_exp_msg_vec.begin(), m_exp_msg_vec.end(),
                                                 ExpMsgRange (exp_cause));
    return it != m_exp_msg_vec.end();
}


void ExpMsg::ExpMsgPrint (ExpCode exp_code, ExpCause exp_cause, TraceCtrl* trace) const
{
    exp_cause &= MASK_EXP_CODE;
    if (IsExpMsgRange (exp_cause) == true) {
        CfMsg::TPrint (MSG_EXP, trace, "<Exception: %s : cause code = 0x%04x>\n",
                       ExpInfo::GetName(exp_code).c_str(), exp_cause);
    }
}
