/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <stdlib.h>
#include "./config.h"
#include "./forest_if.h"
#include "./cube_callback.h"

// static members for call back function
NOTIFY_ERROR_TYP CubeCallBack::__NotifyError = NULL;
NOTIFY_CHANGE_STATUS_TYP CubeCallBack::__NotifyChangeStatus = NULL;
NOTIFY_MEM_WRITE_TYP CubeCallBack::__NotifyMemWrite = NULL;
NOTIFY_MEM_READ_TYP CubeCallBack::__NotifyMemRead = NULL;
NOTIFY_TRACE_INST_TYP CubeCallBack::__NotifyTraceInst = NULL;
NOTIFY_CPU_OPERATION_MODE_TYP CubeCallBack::__NotifyCpuOperationMode = NULL;
NOTIFY_SPID_TYPE CubeCallBack::__NotifySPID = NULL;

HINSTANCE CubeCallBack::dll_handle;

#ifdef _MSC_VER
#else // _MSC_VER
// Dummy Call backs (CubeSuiteが無い場合にCubeSuite向け記述をチェックするため）
APIErrorCode NotifyError (uint32_t pe_id, ErrorCode errid)
{return API_E_OK;}

APIErrorCode NotifyChangeStatus (uint32_t pe_id, PhysAddr pc,
                                 NotifySimState status)
{return API_E_OK;}

APIErrorCode NotifyMemWrite(uint32_t sideband, PhysAddr addr,
                            ByteSize size, uint32_t* data)
{return API_E_OK;}

APIErrorCode NotifyMemRead(uint32_t sideband, PhysAddr addr,
                           ByteSize size, uint32_t* data)
{return API_E_OK;}

APIErrorCode NotifyTraceInst(TraceInstType type)
{return API_E_OK;}

APIErrorCode NotifyCpuOperationMode(uint32_t pe_id, CpuOperationMode mode, uint32_t gpid)
{return API_E_OK;}

APIErrorCode NotifySPID(uint32_t pe_id, uint32_t spid)
{return API_E_OK;}
#endif // _MSC_VER


//! Open DLL and get call back function pointer.
/*!
  \param dllname Designate dll name to get call back function.
  \return error code
  \retval API_E_OK no error
  \retval API_E_PARAM error to open dll or to get call back func
*/
APIErrorCode CubeCallBack::OpenDllFunction (const wchar_t* dllname)
{
#ifdef _MSC_VER
    dll_handle = GetModuleHandle (dllname);
    if (dll_handle == NULL) {
        //Error
        return API_E_PARAM;
    }

    __NotifyChangeStatus = reinterpret_cast<NOTIFY_CHANGE_STATUS_TYP> (
                           GetProcAddress (dll_handle, "NotifyChangeStatus") );
    if (__NotifyChangeStatus == NULL) {
        //Error
        return API_E_PARAM;
    }

    __NotifyError = reinterpret_cast<NOTIFY_ERROR_TYP> (
                    GetProcAddress (dll_handle, "NotifyError") );
    if (__NotifyError == NULL) {
        //Error
        return API_E_PARAM;
    }

    __NotifyMemWrite = reinterpret_cast<NOTIFY_MEM_WRITE_TYP> (
                    GetProcAddress (dll_handle, "NotifyMemWrite") );
    if (__NotifyMemWrite == NULL) {
        //Error
        return API_E_PARAM;
    }

    __NotifyMemRead = reinterpret_cast<NOTIFY_MEM_READ_TYP> (
                    GetProcAddress (dll_handle, "NotifyMemRead") );
    if (__NotifyMemRead == NULL) {
        //Error
        return API_E_PARAM;
    }

    __NotifyTraceInst = reinterpret_cast<NOTIFY_TRACE_INST_TYP> (
                    GetProcAddress (dll_handle, "NotifyTraceInst") );
    if (__NotifyTraceInst == NULL) {
        //Error
        return API_E_PARAM;
    }

    __NotifyCpuOperationMode = reinterpret_cast<NOTIFY_CPU_OPERATION_MODE_TYP> (
                    GetProcAddress (dll_handle, "NotifyCpuOperationMode") );
    if (__NotifyCpuOperationMode == NULL) {
        //Error
        return API_E_PARAM;
    }

    __NotifySPID = reinterpret_cast<NOTIFY_SPID_TYPE> (
                    GetProcAddress (dll_handle, "NotifySPID") );
    if (__NotifySPID == NULL) {
        //Error
        return API_E_PARAM;
    }

#else // _MSC_VER
    // GCC環境ではダミー関数を差し込む
    __NotifyChangeStatus    = NotifyChangeStatus;
    __NotifyError           = NotifyError;
    __NotifyMemWrite        = NotifyMemWrite;
    __NotifyMemRead         = NotifyMemRead;
    __NotifyTraceInst       = NotifyTraceInst;
    __NotifyCpuOperationMode = NotifyCpuOperationMode;
    __NotifySPID = NotifySPID;
#endif // _MSC_VER

    return API_E_OK;
}


//! Close or invalidate call back function pointer.
/*!
  \return error code
  \retval API_E_OK no error
  Nullify call back function pointer to cause error when call them after close.
*/
APIErrorCode CubeCallBack::CloseDllFunction (void)
{
    __NotifyError = NULL;
    __NotifyChangeStatus = NULL;
    __NotifyMemWrite = NULL;
    __NotifyMemRead = NULL;
    __NotifyTraceInst = NULL;
    __NotifyCpuOperationMode = NULL;
    __NotifySPID = NULL;

    return API_E_OK;
}
