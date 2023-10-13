/*
 * (c) 2016, 2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <vector>
#include <string>
#include "./forest_utility_api.h"
#include "./forest_common.h"
#include "./trace_info.h"

class ForestUtil;
class FsBus;
class CModuleVc;

// This macro value is same as BANK_LIST_MAX of G4_NoDbgIF class
#define SCIF_BANK_LIST_MAX 20

class DLL_EXPORT_IMPORT ForestUtilScheap {
public:
  ForestUtil *mpForestUtil; // the instance pointer of ForestUtil
  FsBus      *mpFsBus;      // the instance pointer of Fsbus

  ForestUtilScheap();
  ~ForestUtilScheap();

  void SetMessageLevel (MessageLevel lvl);
  void HardReset (void);

#if 0
  bool MemResourceAlloc (MemRestrictType restrict_type,
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
			 std::vector<ByteSize>* bank_size);
#else
  bool MemResourceAlloc (MemRestrictType restrict_type,
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
			 ByteSize* bank_size_list);

#endif
    bool SetSelfArea (PhysAddr start_addr, PhysAddr end_addr,
                      PhysOffset offset);
    bool RegWriteDebug (uint32_t peid, CReg regid,
                          RegData64 data, RegData64 data_u64 = 0);
    bool RegReadDebug  (uint32_t peid, CReg regid,
                          RegData64* data, RegData64* data_u64 = NULL);
    void VMemWriteDebug (uint32_t peid, PhysAddr addr,
                         uint32_t data, uint32_t bytes);
    uint32_t VMemReadDebug (uint32_t peid, PhysAddr addr,
                            uint32_t bytes);
    void BusWriteDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes);
    void BusReadDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes);
    void SetExtPin (uint32_t peid, ExtPinType pin, uint32_t data);

    void SetDebugMode (DebugMode mode);

    void SetRunTarget(uint32_t peid, bool is_run);
    void SetFrequency(uint32_t peid, uint32_t ratio);
    int32_t GetScId (uint32_t peid) const;

    void SetIcacheSize (uint32_t peid, ByteSize size);
    uint64_t GetIcacheAccessCount (uint32_t peid) const;
    uint64_t GetIcacheHitCount (uint32_t peid) const;
    uint64_t GetIcacheMissCount (uint32_t peid) const;

    BreakInfo* GetBreakInfo (void);
    uint64_t CycleExecute (uint32_t peid, uint64_t cycle_goal, ExecType exec_type);
    void NotifyFetchLatency (SideBand sideband, PhysAddr addr, uint32_t latency);
    void NotifyReadLatency (SideBand sideband, PhysAddr addr, uint32_t latency, uint64_t data);
    void NotifyWriteLatency (SideBand sideband, PhysAddr addr, uint32_t latency);

    void AddBreakMatchPc (uint32_t peid, uint32_t addr);
    void ClearBreakMatchPc (uint32_t peid, uint32_t addr);
    GuardErrType FlashAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd);
    GuardErrType FlashAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd);
    GuardErrType CramAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd);
    GuardErrType CramAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd);
    SysErrCause CfgRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd);
    SysErrCause CrgRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd);
    SysErrCause PegRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd);
    SysErrCause BusWritePhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause BusReadPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool SetPeLramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr);

    void AddBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask);
    void ClearBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask);
    void AddBreakAccess (uint32_t peid, PhysAddr start_addr,
                         PhysAddr end_addr, AccessAttr attr, uint32_t mask);
    void ClearBreakAccess (uint32_t peid, PhysAddr start_addr,
                           PhysAddr end_addr, AccessAttr attr, uint32_t mask);

    const TraceInfo* GetTraceInfo (uint32_t scid) const;

    void CreateForest (void);
    void SetPe (uint32_t clid, uint32_t peid, CoreType core_type, CoreVersion core_version);
    bool SetGuard (uint32_t guard_id, uint32_t allocation_id, PhysAddr base_addr, bool is_valid);
    bool SetGuardParam (uint32_t guard_id, uint32_t allocation_id,
                        uint32_t param_id, uint32_t value);

    void GenPeripherals (void);

    uint64_t GetStepCount (uint32_t peid) const;
    uint64_t GetCycleCount (uint32_t peid) const;
    ExpCause GetExpCause (uint32_t peid) const;


    IntErrorCode NoticeOperandErrorResponse (uint32_t peid, uint32_t syserr_cause);
    IntErrorCode ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                    uint32_t priority, bool eitb);
    IntErrorCode ReqFenmiToCpu (uint32_t peid);
    IntErrorCode ReqFeintToCpu (uint32_t peid, uint32_t channel);
    IntErrorCode ReqSyserrToCpu (uint32_t peid, uint32_t syserr_cause);
    IntErrorCode ReqPseudoFetchSyserrToCpu (uint32_t peid, uint32_t syserr_cause);

    IntErrorCode CancelInterruptToCpu (uint32_t peid);
    IntErrorCode CancelInterruptToCpu (uint32_t peid, uint32_t channel);
    IntErrorCode CancelFenmiToCpu (uint32_t peid);
    IntErrorCode CancelFeintToCpu (uint32_t peid);
    IntErrorCode CancelSyserrToCpu (uint32_t peid);


    void SetRomArea (PhysAddr start_addr, PhysAddr end_addr);
    void SetCramArea (PhysAddr start, PhysAddr end);

    void SetCfgProtEnable(bool en);
    void SetCfgProtRegion(PhysAddr addr);

    FILE* SetFnameAndOpen(char* fname);
    void SetFsbusPointer(void);
    void AddTargetVC(PhysAddr start_addr, PhysAddr end_addr, CModuleVc* tgt_vc, uint32_t id, GuardAttr attr);

    ForestUtilApi* GetForestUtilApi(void);
};
