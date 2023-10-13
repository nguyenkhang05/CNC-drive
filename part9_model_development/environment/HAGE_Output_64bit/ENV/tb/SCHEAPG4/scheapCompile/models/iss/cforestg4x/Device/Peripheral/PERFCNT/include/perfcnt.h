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

#include <string.h>
#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./forest.h"
#include "./sideband.h"
#include "./treg.h"

// typedef std::vector<ProcElement*> PeVec;
#include "proc_element_vector.h"

class Cperfcnt : public CModuleVc
{
public:
    static const PhysAddr PERFCNT_AREA_SIZE = 0x100U;

private:
    static const PhysAddr PERFCNT_ADDR_MASK = PERFCNT_AREA_SIZE - 1;
    static const PhysAddr PERF_CTRL_ADDR    = 0xF0U;
    static const PhysAddr PERF_CNT_ADDR     = 0xF4U;

    uint32_t m_clid;
    uint32_t m_peid;
    uint32_t m_cnten;
    uint32_t m_counter;
    uint32_t m_count_start_time;

    void WriteCNTCTRL (uint8_t* data, ByteSize size);
    void ReadCNTCTRL (uint8_t* data, ByteSize size);
    void WriteCNTDAT (uint8_t* data, ByteSize size);
    void ReadCNTDAT (uint8_t* data, ByteSize size);

    void CountWrite (uint32_t cycle);
    uint32_t CountRead (void);
    void CountStart (void);
    void CountStop (void);
    void CountUpdate (void);

public:
    void CyclicHandler(void) {}
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);

    explicit Cperfcnt (uint32_t clid, uint32_t peid);
    ~Cperfcnt();
};
