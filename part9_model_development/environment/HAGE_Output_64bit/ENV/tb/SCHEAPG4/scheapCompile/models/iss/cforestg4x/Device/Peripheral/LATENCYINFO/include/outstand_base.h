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

#include <string>
#include "./forest_common.h"

struct DLL_EXPORT_IMPORT MemResourceInfo {
    uint32_t pe_fetch_latency;
    uint32_t pe_read_latency;
    uint32_t pe_write_latency;
    uint32_t cl_fetch_latency;
    uint32_t cl_read_latency;
    uint32_t cl_write_latency;
    uint32_t exo_cl_fetch_latency;
    uint32_t exo_cl_read_latency;
    uint32_t exo_cl_write_latency;
    MemLocationType pe_location;
    MemLocationType cl_location;
    uint32_t num_outstanding;
    uint32_t repeat_rate;
};

class DLL_EXPORT_IMPORT MemLatencyInfoBase
{
public:
    virtual ~MemLatencyInfoBase () {}

    virtual uint32_t GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) = 0;
    virtual uint32_t GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) = 0;
    virtual uint32_t GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) = 0;

    virtual uint64_t GetReqAcceptableCycle
                     (uint32_t peid, PhysAddr load_addr, ByteSize size) const = 0;

    virtual void InitMemCycle (void) = 0;
    virtual void UpdateExoPeLatency (uint32_t exo_latency_offset) = 0;
    virtual void GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const = 0;
    virtual bool IsScheap (void) const = 0;
    virtual bool IsBufHitLast (PhysAddr addr) const = 0;
    virtual bool IsBufHit (uint32_t peid, PhysAddr addr) const = 0;
    virtual bool IsLramAccess (uint32_t peid) const = 0;
    virtual MemResourceType GetMemResourceType (uint32_t peid) const = 0;
    virtual uint32_t GetConflictLatency (void) const = 0;
    virtual std::string GetName (PhysAddr addr) const = 0;
    virtual void ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const = 0;
};
