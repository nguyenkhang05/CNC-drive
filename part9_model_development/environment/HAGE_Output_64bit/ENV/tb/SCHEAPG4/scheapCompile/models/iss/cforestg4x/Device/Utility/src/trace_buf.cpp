/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#ifdef SUPPORT_SC_HEAP
#include "./trace_buf.h"
#include "./trace_info.h"


// 毎回newすると遅いので、TraceCtrlの実体をまとめて生成。
// その実体へのポインタを使用中キュー、未使用キューに登録して使う。
// - 注意点
//   SC-HEAPでは中断がありうるので、
//   未使用キューからの取り出し順と使用中キューに積む順番は異なる。

CfApiRetern TraceBuf::SetMaxSize (uint32_t num)
{
    if (m_traces_body.max_size () < num) {
        // vectorの最大サイズを超えているので確保できない
        return CF_API_NG;
    }

    try {
        m_traces_body.resize (num, TraceCtrl ());
        Init ();
        return CF_API_OK;
    } catch (std::bad_alloc exception) {
        return CF_API_NG;
    }
}


const TraceCtrl* TraceBuf::Dequeue (void)
{
    if (IsEmpty ()) {
        // Popすべき情報はない
        return NULL;
    }

    TraceCtrl* trace = m_used_buf.front ();
    m_used_buf.pop ();

    m_unused_buf.push (trace); // 空きリストに積む。
    return trace;
}


TraceCtrl* TraceBuf::Reserve (void)
{
    if (IsFull ()) {
        return NULL;
    }

    TraceCtrl* trace = m_unused_buf.front ();
    m_unused_buf.pop ();
    return trace;
}


void TraceBuf::Queue (TraceCtrl* trace)
{
    m_used_buf.push (trace);
}


bool TraceBuf::IsFull (void) const
{
    // 未使用キューがemptyなら、全部使用中
    return m_unused_buf.empty ();
}


bool TraceBuf::IsEmpty (void) const
{
    return m_used_buf.empty ();
}


uint32_t TraceBuf::GetUsedSize (void) const
{
    return static_cast<uint32_t> (m_used_buf.size ());
}


void TraceBuf::Init (void)
{
    // queueにはclearがないので
    // @@ 直接dequeを使えば良いけど。
    while(!m_used_buf.empty()) {
        m_used_buf.pop();
    }
    while(!m_unused_buf.empty()) {
        m_unused_buf.pop();
    }

    std::vector<TraceCtrl>::iterator it = m_traces_body.begin ();
    while (it != m_traces_body.end ()) {
        TraceCtrl* trace = &(*it);
        m_unused_buf.push (trace);
        ++ it;
    }
}


TraceBuf::TraceBuf (uint32_t num)
{
    SetMaxSize (num);
}
#endif // #ifdef SUPPORT_SC_HEAP
