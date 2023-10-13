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
#include "./cube_ior.h"
#include "./cube_callback.h"
#include "./fsbus.h"
#include "./forest_internals.h"
#include "./forest_utility.h"

SysErrCause CubeIoReg::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size > MAX_ACC_SIZE) {
        return SYSERR_NONE;
    }

    uint32_t data_u32[MAX_ACC_SIZE >> 2] = {0};
    memcpy (data_u32, data, size);
    (*CubeCallBack::__NotifyMemWrite) (sideband.GetValue (), addr, size, data_u32);
    return SYSERR_NONE;
}


SysErrCause CubeIoReg::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size > MAX_ACC_SIZE) {
        memset (data, 0xFF, size);
        return SYSERR_NONE;
    }

    uint32_t data_u32[MAX_ACC_SIZE >> 2] = {0};
    (*CubeCallBack::__NotifyMemRead) (sideband.GetValue (), addr, size, data_u32);
    memcpy (data, data_u32, size);
    return SYSERR_NONE;
}


SysErrCause CubeIoReg::TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    // IORへのフェッチはエラー
    memset (data, p_forest_util->GetMemInit (), size);
    p_internals->CauseError (sideband, E_MEMORY_FETCH_PROTECT);
    return SYSERR_NONE;
}


SysErrCause CubeIoReg::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetWrite (SideBand::DebugAccess(), addr, data, size);
}


SysErrCause CubeIoReg::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetRead (SideBand::DebugAccess(), addr, data, size);
}
