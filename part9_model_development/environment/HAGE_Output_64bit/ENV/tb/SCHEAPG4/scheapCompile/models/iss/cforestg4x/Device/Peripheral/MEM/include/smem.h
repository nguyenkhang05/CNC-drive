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

#include "./forest_common.h"
#include "./module_vc.h"
#include "./memory_body.h"
#include "./proc_element.h"

class Csmem : public CModuleVc
{
public:
    MemBody* mem_body;

    bool MemResourceAlloc (PhysAddr start_addr, PhysAddr end_addr);
    bool MemResourceFree  (PhysAddr start_addr, PhysAddr end_addr);

    void CyclicHandler (void) {}
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void) {}

    Csmem (MemRestrictType restrict_type, int8_t init_value);
    virtual ~Csmem();
};
