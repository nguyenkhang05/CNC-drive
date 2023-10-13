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
#include <string.h>
#include "./forest_common.h"
#include "./outstand_base.h"

class DLL_EXPORT_IMPORT MemLatencyInfoSCHEAP : public MemLatencyInfoBase
{
public:
    MemLatencyInfoSCHEAP () : m_target_name ("peripheral") {}
    ~MemLatencyInfoSCHEAP () {}

    uint32_t GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) {
        return TARGET_IS_SCHEAP;
    }
    uint32_t GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) {
        return TARGET_IS_SCHEAP;
    }
    uint32_t GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) {
        return TARGET_IS_SCHEAP;
    }
    uint64_t GetReqAcceptableCycle (uint32_t peid, PhysAddr load_addr, ByteSize size) const
    { return 0x0; }

    void InitMemCycle (void) {}
    void UpdateExoPeLatency (uint32_t exo_latency_offset) {}
    void GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const {
        memset (res_info, 0, sizeof (MemResourceInfo));
    }
    bool IsScheap (void) const { return true; }
    bool IsBufHitLast (PhysAddr addr) const { return false; }
    bool IsBufHit (uint32_t peid, PhysAddr addr) const { return false; }
    bool IsLramAccess (uint32_t peid) const {return false;}
    MemResourceType GetMemResourceType (uint32_t peid) const { return MEM_RES_OTHERS; }
    uint32_t GetConflictLatency (void) const { return 0; }
    std::string GetName (PhysAddr addr) const { return m_target_name; }
    const std::string m_target_name;
    void ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const;
};
