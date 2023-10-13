/*
 * (c) 2011-2015 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */


#include <algorithm>
#include <stdint.h>
#include <vector>
#include "./config.h"
#include "./outstand_base.h"
#include "./cl_bridge.h"
#include "./forest_message.h"
#include "./forest_internals.h"
#include "./forest_utility.h"
#include "./dbuf.h"


void MemLatencyInfoExCluster::SetExternalMemInfoBase (MemLatencyInfoBase *info_base)
{
    m_external_info_base = info_base;
}



MemLatencyInfoExCluster::MemLatencyInfoExCluster (std::string bridge_name,
                                                  uint32_t clid,
                                                  uint32_t exo_pe_read_latency,
                                                  uint32_t exo_pe_write_latency,
                                                  uint32_t exo_pe_fetch_latency,
                                                  uint32_t outstand_num,
                                                  uint32_t repeat_rate)
{
    m_exo_pe_read_latency   = exo_pe_read_latency;
    m_exo_pe_write_latency  = exo_pe_write_latency;
    m_exo_pe_fetch_latency  = exo_pe_fetch_latency;
    m_outstand_num          = outstand_num;
    m_repeat_interval_cycle = repeat_rate;

    m_external_info_base = NULL;

    m_bridge_name = bridge_name;

    ResetPerfInfo ();
    InitMemCycle ();

}


MemLatencyInfoExCluster::~MemLatencyInfoExCluster ()
{
}


void MemLatencyInfoExCluster::InitMemCycle (void)
{
    m_mem_avail_cycle.clear ();
    m_mem_avail_cycle.assign (m_outstand_num, 0);

    m_mem_issue_cycle.clear ();
    m_mem_issue_cycle.assign (m_outstand_num, 0);
}


void MemLatencyInfoExCluster::UpdateExoPeLatency (uint32_t exo_latency_offset)
{
}


void MemLatencyInfoExCluster::ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const
{
    CfMsg::DPrintML (MSG_INF, "cluster(%s)-->", m_bridge_name.c_str());
    m_external_info_base->ShowAccessRoute(peid, cmd, addr);
}


template <MemAccessCommand cmd>
uint32_t MemLatencyInfoExCluster::GetAccessLatency (uint32_t cl_peid, PhysAddr load_addr,
                                                    uint64_t start_cycle, ByteSize size)
{
    int32_t bridge_latency;
    // Set default Latency between cluster as "bridge_latency"
    if (cmd == DATA_READ) {
        bridge_latency = m_exo_pe_read_latency;
    } else if (cmd == DATA_WRITE) {
        bridge_latency = m_exo_pe_write_latency;
    } else if (cmd == INST_READ) {
        bridge_latency = m_exo_pe_fetch_latency;
    } else {
        bridge_latency = m_exo_pe_read_latency; // Don't come here
    }

    // 発行タイミングの計算
    uint32_t issue_stall; // リクエスト発行までのストールサイクル
    uint64_t issue_cycle; // リクエスト発行される時間
    int64_t stall_cycle = m_mem_avail_cycle.front () - start_cycle;
    if (stall_cycle > 0) {
        // outstandバッファの空きがないので、発行タイミングを遅らせる
        issue_stall = static_cast<uint32_t> (stall_cycle);
        issue_cycle = m_mem_avail_cycle.front ();
    } else {
        issue_stall = 0;
        issue_cycle = start_cycle;
    }

    // Calculate period staying in Cluster buffer
    uint32_t bridge_buff_latency = GetIssueLatency (issue_cycle);
    // "issue_cycle from CPU" + "buffer staying period" equal to issue_cycle from cluster boundary

#ifdef CLUSTER
    if (p_forest_util->IsEnableBusTrace ()) {
        if (m_last_cl_peid != cl_peid) {
            // Ignore conflict with ownself.
            if ((start_cycle != issue_cycle) || (bridge_buff_latency != 0)) {
                // conflict in cluster bridge.
                {
                    std::string cmd_str;
                    //switch (cmd) {
                    //case INST_READ  : cmd_str = "(IF)"; break;
                    //case DATA_READ  : cmd_str = "(DR)"; break;
                    //case DATA_WRITE : cmd_str = "(DW)"; break;
                    //case DATA_RMW   : cmd_str = "(RW)"; break;
                    //}
                    std::stringstream route_str;
                    //route_str << "<Conflict " << cmd_str.c_str() << " P" << (cl_peid & PEID_5BIT_MASK) << "-->";
                    route_str << "<Conflict Bridge " << "P" << (cl_peid & PEID_5BIT_MASK) << "-->";

                    //uint32_t ram_peid, ram_clid;
                    CfMsg::DPrint (MSG_INF, "%s", route_str.str().c_str());
                    CfMsg::DPrint (MSG_INF, "cluster(%s)-->%s(%08x)>\n",
                            m_bridge_name.c_str(), m_external_info_base->GetName(load_addr).c_str(), load_addr);
                    //if (tgt->m_is_bridge == true) {
                    //    CfMsg::DPrint (MSG_INF, "cluster(%s)-->", tgt->m_bus_name.c_str());
                    //    ram_peid = ForestUtil::ExtractPeId (tgt->m_btgt_mem_peid);
                    //    ram_clid = ForestUtil::ExtractClId (tgt->m_btgt_mem_peid);
                    //} else {
                    //    CfMsg::DPrint (MSG_INF, "cluster()-->");
                    //    ram_peid = ForestUtil::ExtractPeId (tgt->m_btgt_cl_peid);
                    //    ram_clid = ForestUtil::ExtractClId (tgt->m_btgt_cl_peid);
                    //}
                    //if (ram_peid == MEM_LOCATE_GLOBAL) {
                    //    CfMsg::DPrint (MSG_INF, "C%dRAM(%08x)>\n", ram_clid, load_addr);
                    //} else {
                    //    CfMsg::DPrint (MSG_INF, "P%dRAM(%08x)>\n", ram_peid, load_addr);
                    //}
                }
            }
        }
        m_last_cl_peid = cl_peid;

        if (cmd == DATA_WRITE) {
            IncWriteCount ();
        } else if (cmd == DATA_READ) {
            IncReadCount ();
        } else if (cmd == INST_READ) {
            IncFetchCount ();
        }
    }
#endif // CLUSTER

    uint64_t target_issue_cycle = bridge_latency + bridge_buff_latency + issue_cycle;
    UpdateMemIssueCycle (bridge_buff_latency + issue_cycle);
    uint32_t slave_latency;
    if (cmd == DATA_READ) {
        slave_latency = m_external_info_base->GetReadLatency (cl_peid, load_addr, target_issue_cycle, size);
    } else if (cmd == DATA_WRITE) {
        slave_latency = m_external_info_base->GetWriteLatency (cl_peid, load_addr, target_issue_cycle, size);
    } else if (cmd == INST_READ) {
        slave_latency = m_external_info_base->GetFetchLatency (cl_peid, load_addr, target_issue_cycle, size);
    } else {
        slave_latency = 0;
    }

#ifdef CLUSTER_DEBUG
    CfMsg::DPrint (MSG_INF, "UpdateMemAvailCycle (%d + %d)\n", target_issue_cycle, slave_latency);
#endif // CLUSTER_DEBUG
    // Available cycle is "issue_cycle from cluster boundary" + "latency inside of cluster"
    UpdateMemAvailCycle (target_issue_cycle + slave_latency);

    return slave_latency + bridge_latency + bridge_buff_latency + issue_stall;
}


void MemLatencyInfoExCluster::UpdateMemAvailCycle (uint64_t avail_cycle)
{
    // アクセス履歴更新
    // サイズは常にoutstanding数
    m_mem_avail_cycle.front () = avail_cycle;
    std::sort (m_mem_avail_cycle.begin(), m_mem_avail_cycle.end());
#ifdef CLUSTER_DEBUG
    CfMsg::DPrint (MSG_INF, "MemLatencyInfoExCluster::UpdateMemAvailCycle [");
    std::vector<uint64_t>::iterator it = m_mem_avail_cycle.begin();
    while (it != m_mem_avail_cycle.end()) {
        CfMsg::DPrint (MSG_INF, "%4d ", (*it));
        it++;
    }
    CfMsg::DPrint (MSG_INF, "]\n");
#endif // CLUSTER_DEBUG
}


void MemLatencyInfoExCluster::UpdateMemIssueCycle (uint64_t issue_cycle)
{
    // アクセス履歴更新
    // サイズは常にoutstanding数
    m_mem_issue_cycle.front () = issue_cycle;
    std::sort (m_mem_issue_cycle.begin(), m_mem_issue_cycle.end());
}


uint32_t MemLatencyInfoExCluster::GetIssueLatency (uint64_t issue_cycle)
{
    // リピートサイクルを反映
    // 処理の簡略化：リピート間隔を既存アクセスの最終終了サイクルを基準に計算。
    //               アクセス順追い越し完了するケースでは、
    //               実際よりレイテンシ大きくなってしまうが、
    //               全要素チェックしたら遅くなるのでやらない。
    int32_t new_buf_latency = static_cast<int32_t> (m_mem_issue_cycle.back () + m_repeat_interval_cycle - issue_cycle);
    if (new_buf_latency < 0) {
        new_buf_latency = 0;
    }

    int32_t new_target_latency = static_cast<int32_t> (m_mem_avail_cycle.front () + m_repeat_interval_cycle - issue_cycle);
    if (new_target_latency < 0) {
        new_target_latency = 0;
    }
    return std::max (new_buf_latency, new_target_latency);
}


uint32_t MemLatencyInfoExCluster::GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                                  ByteSize size)
{
#ifdef CLUSTER_DEBUG
    CfMsg::DPrint (MSG_INF, "<MemLatencyInfoExCluster::GetReadLatency>\n");
#endif // CLUSTER_DEBUG
    if (m_external_info_base->IsBufHit (peid & CL_PEID_MASK, load_addr)) {
        // When dbuf is hit, skip cl latency.
        return m_external_info_base->GetReadLatency (peid, load_addr, start_cycle, size);
    } else {
        return GetAccessLatency<DATA_READ> (peid, load_addr, start_cycle, size);
    }
}


uint32_t MemLatencyInfoExCluster::GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                                   ByteSize size)
{
#ifdef CLUSTER_DEBUG
    CfMsg::DPrint (MSG_INF, "<MemLatencyInfoExCluster::GetWriteLatency>\n");
#endif // CLUSTER_DEBUG
    return GetAccessLatency<DATA_WRITE> (peid, load_addr, start_cycle, size);
}


uint32_t MemLatencyInfoExCluster::GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                                   ByteSize size)
{
#ifdef CLUSTER_DEBUG
    CfMsg::DPrint (MSG_INF, "<MemLatencyInfoExCluster::GetFetchLatency>\n");
#endif // CLUSTER_DEBUG
    return GetAccessLatency<INST_READ> (peid, load_addr, start_cycle, size);
}


void MemLatencyInfoExCluster::GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const
{
#if 0
    res_info->pe_fetch_latency     = 0;
    res_info->pe_read_latency      = 0;
    res_info->pe_write_latency     = 0;
    res_info->exo_pe_fetch_latency = m_exo_pe_fetch_latency;
    res_info->exo_pe_read_latency  = m_exo_pe_read_latency;
    res_info->exo_pe_write_latency = m_exo_pe_write_latency;
    res_info->mem_location         = 0;
    res_info->num_outstanding      = m_outstand_num;
    res_info->repeat_rate          = m_repeat_interval_cycle;
#endif //
}


bool MemLatencyInfoExCluster::IsBufHitLast (PhysAddr addr) const
{
    return m_external_info_base->IsBufHitLast (addr);
}


bool MemLatencyInfoExCluster::IsBufHit (uint32_t peid, PhysAddr addr) const
{
    return m_external_info_base->IsBufHit (peid, addr);
}


uint32_t MemLatencyInfoExCluster::GetConflictLatency (void) const
{
    return m_external_info_base->GetConflictLatency ();
}


void MemLatencyInfoExCluster::ShowPerfInfo (void) const
{
    if ((m_fetch_count != 0) || (m_read_count != 0) || (m_write_count != 0)) {
        CfMsg::DPrint (MSG_ERR,
                       "  Cluster-Bridge (%s): Fetch=%8" PRIu64 ", Read=%8" PRIu64 ", Write=%8" PRIu64 " (counts)\n",
                       m_bridge_name.c_str(), m_fetch_count, m_read_count, m_write_count);
    }
    return;
}


void MemLatencyInfoExCluster::ResetPerfInfo (void)
{
    m_read_count  = 0;
    m_write_count = 0;
    m_fetch_count = 0;
    m_last_cl_peid = 0xFFFFFFFF;
    return;
}
