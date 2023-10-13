/*
 * (c) 2011-2015 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <string>
#include <vector>
#include "./forest_common.h"
#include "./outstand_base.h"

class MemLatencyInfoExCluster : public MemLatencyInfoBase
{
    MemLatencyInfoBase *m_external_info_base;

public:
    MemLatencyInfoExCluster (std::string bridge_name,
                             uint32_t clid,
                             uint32_t exo_pe_read_latency,
                             uint32_t exo_pe_write_latency,
                             uint32_t exo_pe_fetch_latency,
                             uint32_t outstand_num,
                             uint32_t repeat_rate);
    ~MemLatencyInfoExCluster ();

    uint32_t GetIssueLatency (uint64_t issue_cycle);
    uint32_t GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,  ByteSize size);
    uint32_t GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    uint32_t GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    void InitMemCycle (void);
    void UpdateExoPeLatency (uint32_t exo_latency_offset);
    void GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const;
    bool IsScheap (void) const { return false; }
    bool IsBufHitLast (PhysAddr addr) const;
    bool IsBufHit (uint32_t peid, PhysAddr addr) const;
    uint32_t GetConflictLatency (void) const;
    std::string GetName (PhysAddr addr) const { return m_bridge_name; }
    void ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const;

    void SetExternalMemInfoBase (MemLatencyInfoBase *info_base);

    void UpdateMemIssueCycle (uint64_t issue_cycle);
    void UpdateMemAvailCycle (uint64_t avail_cycle);

    uint64_t GetReadCount  (void) { return m_read_count;  }
    uint64_t GetWriteCount (void) { return m_write_count; }
    uint64_t GetFetchCount (void) { return m_fetch_count; }

    void IncReadCount  (void) { m_read_count++;  }
    void IncWriteCount (void) { m_write_count++; }
    void IncFetchCount (void) { m_fetch_count++; }

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

private:
    // Cycle Information of Cluster (fetch/read/write)
    uint32_t m_exo_pe_read_latency;
    uint32_t m_exo_pe_write_latency;
    uint32_t m_exo_pe_fetch_latency;

    uint32_t m_outstand_num;
    uint32_t m_repeat_interval_cycle;

    std::vector<uint64_t> m_mem_avail_cycle, m_mem_issue_cycle;

    template <MemAccessCommand cmd>
    uint32_t GetAccessLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                               ByteSize size);

    // Bus-Access records
    std::string m_bridge_name;
    uint64_t m_read_count, m_write_count, m_fetch_count;

    uint32_t m_last_cl_peid;
};
