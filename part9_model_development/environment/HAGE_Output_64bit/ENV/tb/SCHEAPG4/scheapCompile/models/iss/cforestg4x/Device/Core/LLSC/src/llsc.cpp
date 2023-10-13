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
#include "./config.h"
#include "./llsc.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./exp_message.h"
#include "./trace_info.h"


void Llsc::CreateLink (int32_t scid, PhysAddr link_addr, ByteSize size, TraceCtrl* trace)
{
    // リンク成立はアドレス完全一致が必要。アドレスをマスクしない
    CfMsg::DPrint (MSG_INF, "<Info: PEID=%d : Link %08x, size %d is created>\n",
                   p_forest_util->GetPeId (scid), link_addr, size);
    m_link_targets.push_back (LinkTarget(scid, LinkBody (link_addr, size)));

#ifdef FROG_API_IF
    trace->SetLLBitAdd(link_addr, size);
#endif //FROG_API_IF
}


bool Llsc::IsHaveLink (int32_t scid, PhysAddr link_addr, ByteSize size) const
{
    std::vector<LinkTarget>::const_iterator it = m_link_targets.begin ();
    while (it != m_link_targets.end() ) {
        if (it->first == scid) {
            // リンク成立はアドレス完全一致が必要。アドレスをマスクせずに判定
            // Note : In G3KH condtion of Link satisfication is 4byte align.
            //      : It is configurable by setting m_link_cond_mask
            PhysAddr target_addr = it->second.first & m_link_cond_mask;
            ByteSize target_size = it->second.second;
            link_addr = link_addr & m_link_cond_mask;
            if ((target_addr == link_addr) && (target_size == size)) {
                return true;
            } else {
                ++ it;
            }
        } else {
            ++ it;
        }
    }

    return false;
}


bool Llsc::IsThereAnyLink (void) const
{
    return !m_link_targets.empty ();
}


void Llsc::DeleteLink (PhysAddr link_addr, TraceCtrl* trace)
{
    // リンク解除は完全一致でなく、一定範囲内なら解除
    link_addr &= LINK_ADDR_MASK;

    LinkTargetVec::iterator it = m_link_targets.begin ();
    while (it != m_link_targets.end() ) {
        if ((it->second.first & LINK_ADDR_MASK) == link_addr) {
            CfMsg::DPrint (MSG_INF, "<Info: Link Addr %08x is deleted>\n",
                           link_addr);
#ifdef FROG_API_IF
            trace->RecordDeleteLLBit(p_forest_util->GetPeId (it->first), it->second.first);
#endif //FROG_API_IF
            it = m_link_targets.erase (it);
        } else {
            ++ it;
        }
    }
}


void Llsc::DeleteOwnLink (int32_t scid, PhysAddr paddr, TraceCtrl* trace)
{
    LinkTargetVec::iterator it = m_link_targets.begin ();

    while (it != m_link_targets.end() ) {
        if ((it->first == scid) && ((it->second.first & LINK_ADDR_MASK) == paddr)) {
            CfMsg::DPrint (MSG_INF, "<Info: Own Link %08x is deleted>\n",
                           p_forest_util->GetPeId (scid), it->second.first);
#ifdef FROG_API_IF
            trace->RecordDeleteLLBit(p_forest_util->GetPeId (it->first), it->second.first);
#endif //FROG_API_IF
            it = m_link_targets.erase (it);
        } else {
            ++ it;
        }
    }
}


void Llsc::DeleteOwnLink (int32_t scid, TraceCtrl* trace)
{
    LinkTargetVec::iterator it = m_link_targets.begin ();

    while (it != m_link_targets.end() ) {
        if (it->first == scid) {
            CfMsg::DPrint (MSG_INF, "<Info: Own Link %08x is deleted>\n",
                           p_forest_util->GetPeId (scid), it->second.first);
#ifdef FROG_API_IF
            trace->RecordDeleteLLBit(p_forest_util->GetPeId (it->first), it->second.first);
#endif //FROG_API_IF
            it = m_link_targets.erase (it);
        } else {
            ++ it;
        }
    }
}


void Llsc::DeletePeLink (int32_t scid, TraceCtrl* trace)
{
    uint32_t del_peid = p_forest_util->GetPeId (scid);

    LinkTargetVec::iterator it = m_link_targets.begin ();
    while (it != m_link_targets.end() ) {
        uint32_t peid = p_forest_util->GetPeId (it->first);
        if (peid == del_peid) {
            CfMsg::DPrint (MSG_INF, "<Info: Own PE Link %08x is deleted>\n",
                           peid, it->second.first);
#ifdef FROG_API_IF
            trace->RecordDeleteLLBit(p_forest_util->GetPeId (it->first), it->second.first);
#endif //FROG_API_IF
            it = m_link_targets.erase (it);
        } else {
            ++ it;
        }
    }
}


void Llsc::DeleteVmLink (int32_t scid, TraceCtrl* trace)
{
    ContextId del_vcid = NC;
    uint32_t del_peid = p_forest_util->GetPeId (scid);

    LinkTargetVec::iterator it = m_link_targets.begin ();
    while (it != m_link_targets.end() ) {
        // 他PEに同じVCIDが存在するかもしれないので、自PEかどうかもチェック
        ContextId vcid = NC;
        uint32_t peid = p_forest_util->GetPeId (it->first);
        if ( (vcid == del_vcid) && (peid == del_peid) ) {
            CfMsg::DPrint (MSG_INF, "<Info: Own Vm Link %08x is deleted>\n",
                           peid, it->second.first);
#ifdef FROG_API_IF
            trace->RecordDeleteLLBit(p_forest_util->GetPeId (it->first), it->second.first);
#endif //FROG_API_IF
            it = m_link_targets.erase (it);
        } else {
            ++ it;
        }
    }
}


void Llsc::InitLink (void)
{
    m_link_targets.clear ();
}
