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
#include "./error_area.h"

SysErrCause ErrorArea::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{ return TargetWriteDebug (addr, data, size); }

SysErrCause ErrorArea::TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{ return TargetReadDebug (addr, data, size); }

SysErrCause ErrorArea::TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    memset (data, 0x00U, size);
    return SYSERR_ICFB;
}

SysErrCause ErrorArea::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{ return SYSERR_DBUS; }

SysErrCause ErrorArea::TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size)
{
    memset (data, 0x00U, size);
    return SYSERR_DBUS;
}
