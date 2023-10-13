/*
 * (c) 2011-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */


#pragma once


#include <stdio.h>
#include "./forest_common_api.h"

class TraceCtrl;
class ModuleGuard;
class SideBand;

class DLL_EXPORT_IMPORT ForestUtilApi {
public:
    virtual void CreateForest (void) = 0;
    virtual void GenPeripherals (void) = 0;
    virtual bool LoadSrec (const char* filename, PhysAddr offset) = 0;
    virtual void SetDebugMode (DebugMode mode) = 0;
    virtual void SetExtPin (uint32_t peid, ExtPinType pin, uint32_t data) = 0;
    virtual void SetFileOut (FILE* fp) = 0;
    virtual void SetPe (uint32_t clid, uint32_t peid, CoreType core_type, CoreVersion core_version) = 0;
    virtual void SetMessageLevel (MessageLevel lvl) = 0;

    virtual void HardReset (void) = 0;
#ifdef SUPPORT_SC_HEAP
    virtual BreakInfo* GetBreakInfo (void) = 0;
    virtual uint64_t CycleExecute (uint32_t peid, uint64_t cycle_goal,
                                   ExecType exec_type) = 0;
    virtual uint32_t GetUsedTraceSize (void) = 0;
    virtual CfApiRetern SetMaxTraceSize (uint32_t num) = 0;
    virtual const TraceCtrl* DequeueTrace (void) = 0;
    virtual ModuleGuard* GetGuardPtr (uint32_t peid, GuardAttr attr) = 0;
    virtual void AddBreakMatchPc (uint32_t peid, uint32_t addr) = 0;
    virtual void ClearBreakMatchPc (uint32_t peid, uint32_t addr) = 0;

    virtual GuardErrType FlashAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd) = 0;
    virtual GuardErrType FlashAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd) = 0;
    virtual GuardErrType CramAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd) = 0;
    virtual GuardErrType CramAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd) = 0;
    virtual SysErrCause BusWritePhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) = 0;
    virtual SysErrCause BusReadPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) = 0;

    virtual bool SetPeLramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr) = 0;
#endif // SUPPORT_SC_HEAP

    virtual void AddBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask) = 0;
    virtual void ClearBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask) = 0;
    virtual void AddBreakAccess (uint32_t peid, PhysAddr start_addr,
                                 PhysAddr end_addr, AccessAttr attr, uint32_t mask) = 0;
    virtual void ClearBreakAccess (uint32_t peid, PhysAddr start_addr,
                                   PhysAddr end_addr, AccessAttr attr, uint32_t mask) = 0;

    virtual bool RegWriteDebug (uint32_t peid, CReg regid, RegData64 data, RegData64 data_u64 = 0) = 0;
    virtual bool RegReadDebug (uint32_t peid, CReg regid, RegData64* data, RegData64* data_u64 = NULL) = 0;
    virtual void VMemWriteDebug (uint32_t peid, PhysAddr addr,
                                 uint32_t data, uint32_t bytes) = 0;
    virtual uint32_t VMemReadDebug (uint32_t peid, PhysAddr addr,
                                    uint32_t bytes) = 0;
    virtual void BusWriteDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes) = 0;
    virtual void BusReadDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes) = 0;

    virtual void SetRunTarget(uint32_t peid, bool is_run) = 0;
    virtual void SetFrequency(uint32_t peid, uint32_t ratio) = 0;
    virtual bool MemResourceAlloc (MemRestrictType restrict_type,
                                   uint32_t pe_fetch_latency,
                                   uint32_t pe_read_latency,
                                   uint32_t pe_write_latency,
                                   uint32_t cl_fetch_latency,
                                   uint32_t cl_read_latency,
                                   uint32_t cl_write_latency,
                                   uint32_t exo_cl_fetch_latency,
                                   uint32_t exo_cl_read_latency,
                                   uint32_t exo_cl_write_latency,
                                   PhysAddr start_addr,
                                   PhysAddr end_addr,
                                   MemMappedArea area_type,
                                   int32_t offset,
                                   MemLocationType pe_location,
                                   MemLocationType cl_location,
                                   uint32_t num_outstand,
                                   uint32_t repeat_rate,
                                   std::vector<ByteSize>* bank_size) = 0;
    virtual bool SetMirrorArea (PhysAddr start_addr, PhysAddr end_addr,
                                PhysOffset offset) = 0;
    virtual bool SetSelfArea (PhysAddr start_addr, PhysAddr end_addr,
                              PhysOffset offset) = 0;
    virtual void SetIcacheSize (uint32_t peid, ByteSize size) = 0;

    virtual uint64_t GetStepCount (uint32_t peid) const = 0;
    virtual uint64_t GetCycleCount (uint32_t peid) const = 0;
    virtual ExpCause GetExpCause (uint32_t peid) const = 0;

    virtual IntErrorCode ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                            uint32_t priority, bool eitb) = 0;
    virtual IntErrorCode ReqFenmiToCpu (uint32_t peid) = 0;
    virtual IntErrorCode ReqFeintToCpu (uint32_t peid, uint32_t channel) = 0;
    virtual IntErrorCode ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                            uint32_t priority, bool eitb,
                                            bool is_gm, uint32_t gpid, bool is_bgint) = 0;
    virtual IntErrorCode ReqFeintToCpu (uint32_t peid, uint32_t channel,
                                        bool is_gm, uint32_t gpid, bool is_bgint) = 0;
    virtual IntErrorCode CancelInterruptToCpu (uint32_t peid, uint32_t channel) = 0;
    virtual IntErrorCode CancelFenmiToCpu (uint32_t peid) = 0;
    virtual IntErrorCode CancelFeintToCpu (uint32_t peid) = 0;

    virtual uint64_t GetIcacheAccessCount (uint32_t peid) const = 0;
    virtual uint64_t GetIcacheHitCount (uint32_t peid) const = 0;
    virtual uint64_t GetIcacheMissCount (uint32_t peid) const = 0;

    virtual ~ForestUtilApi () {}
};
