/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string.h>
#include "./config.h"
#include "./dbg_area.h"

SysErrCause DbgArea::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{ return TargetWriteDebug (addr, data, size); }

SysErrCause DbgArea::TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{ return TargetReadDebug (addr, data, size); }

SysErrCause DbgArea::TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{ return TargetRead (sideband, addr, data, size); }

SysErrCause DbgArea::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{ return SYSERR_NONE; }

SysErrCause DbgArea::TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size)
{
    memset (data, 0x00U, size);
    return SYSERR_NONE;
}
