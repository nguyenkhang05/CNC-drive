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

class ModuleGuard
{
public:
    virtual SysErrCause IsAccPermited (SideBand sideband, PhysAddr addr, ByteSize size,
                                       MemAccessCommand cmd) = 0;
    virtual ~ModuleGuard () {}
};


class ModuleGuardDummy : public ModuleGuard
{
    SysErrCause IsAccPermited (SideBand sideband, PhysAddr addr, ByteSize size,
                               MemAccessCommand cmd) { return SYSERR_NONE; }
};
