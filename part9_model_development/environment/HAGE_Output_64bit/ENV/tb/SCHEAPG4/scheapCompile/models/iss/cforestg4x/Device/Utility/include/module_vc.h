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
#include "./sideband.h"

class TRegModuleVc;

class CModuleVc
{
public:
    virtual SysErrCause TargetWrite (SideBand sideband, PhysAddr addr,
                                     uint8_t* data, ByteSize size) = 0;
    virtual SysErrCause TargetRead  (SideBand sideband, PhysAddr addr,
                                     uint8_t* data, ByteSize size) = 0;
    virtual SysErrCause TargetFetch (SideBand sideband, PhysAddr addr,
                                     uint8_t* data, ByteSize size) = 0;
    virtual SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size) = 0;
    virtual SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size) = 0;
    virtual void HardReset (void) = 0;
    virtual void CyclicHandler (void) = 0;

    virtual ~CModuleVc () {}
};
