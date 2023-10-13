/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./smem.h"
#include "./memory_body.h"

SysErrCause Csmem::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef ENABLE_MISR
    SysErrCause ret =
#endif // ENABLE_MISR
    mem_body->MemBodyWrite (sideband, addr, data, size);
#ifdef ENABLE_MISR
    return ret;
#else // ENABLE_MISR
    return SYSERR_NONE;
#endif // ENABLE_MISR
}


SysErrCause Csmem::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef ENABLE_MISR
    SysErrCause ret =
#endif // ENABLE_MISR
    mem_body->MemBodyRead (sideband, addr, data, size);
#ifdef ENABLE_MISR
    return ret;
#else // ENABLE_MISR
    return SYSERR_NONE;
#endif // ENABLE_MISR
}


SysErrCause Csmem::TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef ENABLE_MISR
    SysErrCause ret =
#endif // ENABLE_MISR
    mem_body->MemBodyFetch (sideband, addr, data, size);
#ifdef ENABLE_MISR
    return ret;
#else // ENABLE_MISR
    return SYSERR_NONE;
#endif // ENABLE_MISR
}


SysErrCause Csmem::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef ENABLE_MISR
    SysErrCause ret =
#endif // ENABLE_MISR
    mem_body->MemBodyWriteDebug (addr, data, size);
#ifdef ENABLE_MISR
    return ret;
#else // ENABLE_MISR
    return SYSERR_NONE;
#endif // ENABLE_MISR
}


SysErrCause Csmem::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
#ifdef ENABLE_MISR
    SysErrCause ret =
#endif // ENABLE_MISR
    mem_body->MemBodyReadDebug (addr, data, size);
#ifdef ENABLE_MISR
    return ret;
#else // ENABLE_MISR
    return SYSERR_NONE;
#endif // ENABLE_MISR
}


bool Csmem::MemResourceAlloc (PhysAddr start_addr, PhysAddr end_addr)
{
    // @@ アドレス範囲をMemBlock単位に区切って確保する
    PhysAddr  base_addr   = start_addr & ~(MemBody::MEM_BLOCK_MASK);
    PhysAddr  finish_addr = end_addr | MemBody::MEM_BLOCK_MASK;

    bool ret = true;
    for (PhysAddr addr = base_addr; addr < finish_addr; addr += MemBody::MEM_BLOCK_SIZE) {
        ret &= mem_body->MemResourceAlloc (addr);
    }
    return ret;
}


bool Csmem::MemResourceFree (PhysAddr start_addr, PhysAddr end_addr)
{
    // @@ アドレス範囲をMemBlock単位に区切って確保する
    PhysAddr  base_addr   = start_addr & ~(MemBody::MEM_BLOCK_MASK);
    PhysAddr  finish_addr = end_addr | MemBody::MEM_BLOCK_MASK;

    bool ret = true;
    for (PhysAddr addr = base_addr; addr < finish_addr; addr += MemBody::MEM_BLOCK_SIZE) {
        ret &= mem_body->MemResourceFree (addr);
    }
    return ret;
}


Csmem::Csmem (MemRestrictType restrict_type, int8_t init_value)
{
    mem_body = new MemBody (restrict_type, init_value);
}


Csmem::~Csmem ()
{
    delete mem_body;
}
