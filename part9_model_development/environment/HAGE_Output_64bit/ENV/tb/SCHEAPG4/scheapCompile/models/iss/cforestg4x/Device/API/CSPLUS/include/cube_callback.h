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

#ifdef _MSC_VER
#define NOMINMAX // for std::max
#include <Windows.h>
#else // _MSC_VER
typedef void* HINSTANCE;
#endif // _MSC_VER

#include <string>
#include "./sideband.h"


extern "C" {
    APIErrorCode NotifyError (uint32_t pe_id, ErrorCode errid);
    APIErrorCode NotifyChangeStatus (uint32_t pe_id, PhysAddr pc,
                                     NotifySimState status);
    APIErrorCode NotifyMemWrite(uint32_t sideband, PhysAddr addr,
                                ByteSize size, uint32_t* data);
    APIErrorCode NotifyMemRead(uint32_t sideband, PhysAddr addr,
                               ByteSize size, uint32_t* data);
    APIErrorCode NotifyTraceInst(TraceInstType type);
    APIErrorCode NotifyCpuOperationMode(uint32_t pe_id, CpuOperationMode mode, uint32_t gpid);
    APIErrorCode NotifySPID(uint32_t pe_id, uint32_t spid);
}


typedef APIErrorCode (*NOTIFY_ERROR_TYP) (uint32_t pe_id, ErrorCode errid);
typedef APIErrorCode (*NOTIFY_CHANGE_STATUS_TYP) (uint32_t pe_id,
                                                  PhysAddr pc, NotifySimState status);
typedef APIErrorCode (*NOTIFY_MEM_WRITE_TYP) (uint32_t sideband, PhysAddr addr,
                                              ByteSize size, uint32_t* data);
typedef APIErrorCode (*NOTIFY_MEM_READ_TYP) (uint32_t sideband, PhysAddr addr,
                                             ByteSize size, uint32_t* data);
typedef APIErrorCode (*NOTIFY_TRACE_INST_TYP) (TraceInstType type);
typedef APIErrorCode (*NOTIFY_CPU_OPERATION_MODE_TYP) (uint32_t pe_id, CpuOperationMode mode, uint32_t gpid);
typedef APIErrorCode (*NOTIFY_SPID_TYPE) (uint32_t pe_id, uint32_t spid);

class CubeCallBack {
public:
    static HINSTANCE dll_handle;

    static NOTIFY_ERROR_TYP __NotifyError;
    static NOTIFY_CHANGE_STATUS_TYP __NotifyChangeStatus;
    static NOTIFY_MEM_WRITE_TYP __NotifyMemWrite;
    static NOTIFY_MEM_READ_TYP __NotifyMemRead;
    static NOTIFY_TRACE_INST_TYP __NotifyTraceInst;
    static NOTIFY_CPU_OPERATION_MODE_TYP __NotifyCpuOperationMode;
    static NOTIFY_SPID_TYPE __NotifySPID;

    static APIErrorCode OpenDllFunction (const wchar_t* dllname);
    static APIErrorCode CloseDllFunction (void);
};
