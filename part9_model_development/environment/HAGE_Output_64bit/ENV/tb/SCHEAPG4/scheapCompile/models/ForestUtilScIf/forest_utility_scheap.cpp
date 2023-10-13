/*
 * (c) 2016, 2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */
#include <stdio.h>   // for sprintf

#include "forest_utility_scheap.h"
#include "forest_utility.h"
#include "module_vc.h"
#include "fsbus.h"
#include "outstand.h"

///<summary>Constructor</summary>
///<returns>none</returns>
ForestUtilScheap::ForestUtilScheap ()
  :mpFsBus( (FsBus *) 0)
{
  mpForestUtil = new ForestUtil();
}


///<summary>Destructor</summary>
///<returns>none</returns>
ForestUtilScheap::~ForestUtilScheap ()
{
  delete mpForestUtil;
  if (mpFsBus != NULL){
    delete mpFsBus;
  }
}


///<summary>set Message Level</summary>
///<param name="lvl">message level (MSG_ERR, MEG_WAR, MSG_EXP)</param>
///<returns>PE type (G4MH)</returns>
void ForestUtilScheap::SetMessageLevel (MessageLevel lvl)
{
  mpForestUtil->SetMessageLevel(lvl);
}


///<summary>Start simulation</summary>
///<param name="peid">Targe PE</param>
///<param name="cycle_goal">Maximum execution period</param>
///<param name="exec_type">select step or continuous execution (EXEC_TYPE_STEP, EXEC_TYPE_RUN)</param>
///<returns>Elapsed cycles for execution</returns>
uint64_t ForestUtilScheap::CycleExecute (uint32_t peid, uint64_t cycle_goal, ExecType exec_type)
{
  return mpForestUtil->CycleExecute(peid, cycle_goal, exec_type);
}


///<summary>Get the information of the last break</summary>
///<returns>Information of the last break</returns>
BreakInfo* ForestUtilScheap::GetBreakInfo (void)
{
    return mpForestUtil->GetBreakInfo ();
}


///<summary>Notify the fetch latency</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of fetch access</param>
///<param name="latency">access latency</param>
///<returns></returns>
void ForestUtilScheap::NotifyFetchLatency (SideBand sideband, PhysAddr addr, uint32_t latency)
{
  mpForestUtil->NotifyFetchLatency (sideband, addr, latency);
}


///<summary>Notify the read latency</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of fetch access</param>
///<param name="latency">access latency</param>
///<returns></returns>
void ForestUtilScheap::NotifyReadLatency (SideBand sideband, PhysAddr addr, uint32_t latency, uint64_t data)
{
  mpForestUtil->NotifyReadLatency (sideband, addr, latency, data);
}


///<summary>Notify the write latency</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of fetch access</param>
///<param name="latency">access latency</param>
///<returns></returns>
void ForestUtilScheap::NotifyWriteLatency (SideBand sideband, PhysAddr addr, uint32_t latency)
{
  mpForestUtil->NotifyWriteLatency (sideband, addr, latency);
}


///<summary>Access code flash from PE. If CFG is enabled in the target, CFG checks the access permission</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code flash rom</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<param name="cmd">R/W access type</param>
///<returns>Access result</returns>
GuardErrType ForestUtilScheap::FlashAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
  return mpForestUtil->FlashAccessPe (sideband, addr, data, size, cmd);
}


///<summary>Access code flash from SAXI bus. If CFG is enabled in the target, CFG checks the access permission</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code flash rom</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<param name="cmd">R/W access type</param>
///<returns>Access result</returns>
GuardErrType ForestUtilScheap::FlashAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
  return mpForestUtil->FlashAccessSAxi (sideband, addr, data, size, cmd);
}


///<summary>Access Cram from PE. If CRG is enabled in the target, CRG checks the access permission</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code flash rom</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<param name="cmd">R/W access type</param>
///<returns>Access result</returns>
GuardErrType ForestUtilScheap::CramAccessPe (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
  return mpForestUtil->CramAccessPe (sideband, addr, data, size, cmd);
}


///<summary>Access Cram from SAXI bus. If CRG is enabled in the target, CRG checks the access permission</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code cram</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<param name="cmd">R/W access type</param>
///<returns>Access result</returns>
GuardErrType ForestUtilScheap::CramAccessSAxi (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
  return mpForestUtil->CramAccessSAxi (sideband, addr, data, size, cmd);
}


///<summary>Access register of CFG</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code cram</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<param name="cmd">R/W access type</param>
///<returns>Syserr cause</returns>
SysErrCause ForestUtilScheap::CfgRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
  return mpForestUtil->CfgRegAccess (sideband, addr, data, size, cmd);
}


///<summary>Access register of CRG</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code cram</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<param name="cmd">R/W access type</param>
///<returns>Syserr cause</returns>
SysErrCause ForestUtilScheap::CrgRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
  return mpForestUtil->CrgRegAccess (sideband, addr, data, size, cmd);
}


///<summary>Access register of PEG</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code cram</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<param name="cmd">R/W access type</param>
///<returns>Syserr cause</returns>
SysErrCause ForestUtilScheap::PegRegAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size, MemAccessCommand cmd)
{
  return mpForestUtil->PegRegAccess (sideband, addr, data, size, cmd);
}


///<summary>Write data voa Fsbus (which is bus in CForest)</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code cram</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<returns>Syserr cause</returns>
SysErrCause ForestUtilScheap::BusWritePhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
  return mpForestUtil->BusWritePhys (sideband, addr, data, size);
}


///<summary>Read data voa Fsbus (which is bus in CForest)</summary>
///<param name="sideband">sideband parameter for access</param>
///<param name="addr">Physical address of accessing code cram</param>
///<param name="data">pointer for accessing data</param>
///<param name="size">access size (1 and above)</param>
///<returns>Syserr cause</returns>
SysErrCause ForestUtilScheap::BusReadPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    return mpForestUtil->BusReadPhys (sideband, addr, data, size);
}


///<summary>Register LRAM area for the designated PE.</summary>
///<param name="peid">PEID to specify LRAM area</param>
///<param name="start_addr">LRAM start address for PE</param>
///<param name="end_addr">LRAM end address for PE</param>
///<returns>true:siccess, false:fail</returns>
bool ForestUtilScheap::SetPeLramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr)
{
  return mpForestUtil->SetPeLramArea (peid, start_addr, end_addr);
}


///<summary>Reset all PEs and all peripherals, Initialize cycle information</summary>
///<returns>None</returns>
void ForestUtilScheap::HardReset (void)
{
  mpForestUtil->HardReset();
}


#if 0
bool ForestUtilScheap::MemResourceAlloc (MemRestrictType restrict_type,
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
                                   int32_t  offset,
                                   MemLocationType pe_location,
                                   MemLocationType cl_location,
                                   uint32_t num_outstand,
                                   uint32_t repeat_rate,
                                   std::vector<ByteSize>* bank_size)
#else
///<summary>Allocate memory. If the access comes from the own PE designated by mem_location, pe_fetch_latency/pe_read_latency/pe_write_latency is used as access latency. If the access comes from different PE designated by mem_location or mem_location is MEM_LOCATE_GLOBAL, exo_pe_fetch_latency/exo_pe_read_latency/exo_pe_write_latency is used as access latency.</summary>
///<param name="restrict_type">mrmoty access restriction</param>
///<param name="pe_fetch_latency">memory latency for own PE local access (fetch)</param>
///<param name="pe_read_latency">memory latency for own PE local access (read)</param>
///<param name="pe_write_latency">memory latency for own PE local access (write)</param>
///<param name="cl_fetch_latency">memory latency from other PE in same cluster (fetch)</param>
///<param name="cl_read_latency">memory latency from other PE in same cluster (read)</param>
///<param name="cl_write_latency">memory latency from other PE in same cluster (write)</param>
///<param name="exo_cl_fetch_latency">memory latency from other Cluster or for global memory which is placed at outside of Cluster (fetch)</param>
///<param name="exo_cl_read_latency">memory latency from other Cluster or for global memory which is placed at outside of Cluster (read)</param>
///<param name="exo_cl_write_latency">memory latency from other Cluster or for global memory which is placed at outside of Cluster (write)</param>
///<param name="start_addr">physical start address of memory</param>
///<param name="end_addr">physical end address of memory</param>
///<param name="area_type">memory type (MI_CF_FROM/LRAM/CRAM/OTHER)</param>
///<param name="offset">prease set 0</param>
///<param name="pe_location">If resource is placed at PE local, set PEID. Otherwise, set MEM_LOCATE_GLOBAL</param>
///<param name="cl_location">: If memory is placed at Cluster or PE local, Set CLID. Otherwise, set MEM_LOCATE_GLOBAL</param>
///<param name="outstand">the number of outstanding (1 or more)</param>
///<param name="repeat_rate">repeat rate</param>
///<param name="bank_size_list">bank size(byte) for each bank</param>
///<returns>true: Success, false: Failure. Other memory or peripherals are allocated at same address. Illegal parameter is used.</returns>
bool ForestUtilScheap::MemResourceAlloc (MemRestrictType restrict_type,
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
                                   int32_t  offset,
                                   MemLocationType pe_location,
                                   MemLocationType cl_location,
                                   uint32_t num_outstand,
                                   uint32_t repeat_rate,
                                   ByteSize* bank_size_list)
#endif
{
  std::vector<ByteSize> v_bank_size_list;
  for (int i=0; i<SCIF_BANK_LIST_MAX; i++){
    if (bank_size_list[i] == 0) break;
    v_bank_size_list.push_back(bank_size_list[i]);
  }
  return mpForestUtil->MemResourceAlloc ( restrict_type,
					  pe_fetch_latency,
					  pe_read_latency,
					  pe_write_latency,
					  cl_fetch_latency,
					  cl_read_latency,
					  cl_write_latency,
					  exo_cl_fetch_latency,
					  exo_cl_read_latency,
					  exo_cl_write_latency,
					  start_addr,
					  end_addr,
					  area_type,
					  offset,
					  pe_location,
					  cl_location,
					  num_outstand,
					  repeat_rate,
					  &v_bank_size_list);
}


///<summary>Allocate self area</summary>
///<param name="start_addr">Physical address of self area (start address)</param>
///<param name="end_addr">Physical address of self area (end address)</param>
///<param name="offset">address offset from self area to acual area</param>
///<returns></returns>
bool ForestUtilScheap::SetSelfArea (PhysAddr start_addr, PhysAddr end_addr, PhysOffset offset)
{
  return mpForestUtil->SetSelfArea (start_addr, end_addr, offset);
}


///<summary>Write data to the designated register of the designated PE. All protection is ignored on this access.</summary>
///<param name="peid">Target PE</param>
///<param name="regid">target register</param>
///<param name="data">write data for [63:0]</param>
///<param name="data_u64">write data for [127:64] if register is 128bit (for WR)</param>
///<returns>true: Success, false: Failure. There is no register correspond with the regid</returns>
bool ForestUtilScheap::RegWriteDebug (uint32_t peid, CReg regid, RegData64 data, RegData64 data_u64)
{
  return mpForestUtil->RegWriteDebug (peid, regid, data, data_u64);
}


///<summary>Read data to the designated register of the designated PE. All protection is ignored on this access.</summary>
///<param name="peid">Target PE</param>
///<param name="regid">target register</param>
///<param name="data">pointer to store the read data [63:0]</param>
///<param name="data_u64">pointer to store the read data[127:64] if register is 128bit (for WR)</param>
///<returns>true: Success, false: Failure. There is no register correspond with the regid</returns>
bool ForestUtilScheap::RegReadDebug (uint32_t peid, CReg regid, RegData64* data, RegData64* data_u64)
{
  return mpForestUtil->RegReadDebug (peid, regid, data, data_u64);
}


///<summary>Write data to the designated address. If the address is self area, address is translated based on peid. All protection including MemRestrictType is ignored on this access.If processor has cache, this access might break the cache coherency. It must be kept by user.</summary>
///<param name="peid">Target PE</param>
///<param name="addr">address of writing memory</param>
///<param name="data">write data</param>
///<param name="bytes">access size (1, 2, 4)</param>
///<returns>None</returns>
void ForestUtilScheap::VMemWriteDebug (uint32_t peid, PhysAddr addr, uint32_t data, uint32_t bytes)
{
  return mpForestUtil->VMemWriteDebug (peid, addr, data, bytes);
}


///<summary>Read data from the designated address. If the address is self area, address is translated based on peid. All protection including MemRestrictType is ignored on this access.</summary>
///<param name="peid">Target PE</param>
///<param name="addr">address of reading memory</param>
///<param name="bytes">access size (1, 2, 4)</param>
///<returns>read data </returns>
uint32_t ForestUtilScheap::VMemReadDebug (uint32_t peid, PhysAddr addr, uint32_t bytes)
{
  return mpForestUtil->VMemReadDebug (peid, addr, bytes);
}


///<summary>Write data to the designated physical address. All protection including MemRestrictType is ignored on this access.If processor has cache, this access might break the cache coherency. It must be kept by user.</summary>
///<param name="paddr">Physycal address of writing memory</param>
///<param name="data">pointer foe write data</param>
///<param name="bytes">access size (1 and above)</param>
///<returns>None</returns>
void ForestUtilScheap::BusWriteDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes)
{
  mpForestUtil->BusWriteDebug (paddr, data, bytes);
}


///<summary>Read data from the designated physical address. All protection including MemRestrictType is ignored on this access.</summary>
///<param name="paddr">Physycal address of writing memory</param>
///<param name="data">pointer foe reading data</param>
///<param name="bytes">access size (1 and above)</param>
///<returns>None</returns>
void ForestUtilScheap::BusReadDebug (PhysAddr paddr, uint8_t* data, uint32_t bytes)
{
  mpForestUtil->BusReadDebug (paddr, data, bytes);
}


///<summary>Set the parameter of the designated PE.The parameter will affect to registers after HardReset.</summary>
///<param name="peid">peid of targe PE</param>
///<param name="pin">parameter type</param>
///<param name="data">value of pin</param>
///<returns>None</returns>
void ForestUtilScheap::SetExtPin (uint32_t peid, ExtPinType pin, uint32_t data)
{
  mpForestUtil->SetExtPin (peid, pin, data);
}


///<summary>Get SCID</summary>
///<param name="peid">peid of target PE</param>
///<returns>SCID value</returns>
int32_t ForestUtilScheap::GetScId (uint32_t peid) const
{
  return mpForestUtil->GetScId (peid);
}


///<summary>Designate the instruction cache size of target PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="size">instruction cache size by byte (0, 1024, 2048, 4096, ..., 65536)</param>
///<returns>NNone</returns>
void ForestUtilScheap::SetIcacheSize (uint32_t peid, ByteSize size)
{
  mpForestUtil->SetIcacheSize (peid, size);
}


///<summary>Get total access count of instruction cache for the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<returns>count value</returns>
uint64_t ForestUtilScheap::GetIcacheAccessCount (uint32_t peid) const
{
  return mpForestUtil->GetIcacheAccessCount (peid);
}


///<summary>Get hit count of instruction cache for the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<returns>count value</returns>
uint64_t ForestUtilScheap::GetIcacheHitCount (uint32_t peid) const
{
  return mpForestUtil->GetIcacheHitCount (peid);
}


///<summary>Get miss count of instruction cache for the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<returns>count value</returns>
uint64_t ForestUtilScheap::GetIcacheMissCount (uint32_t peid) const
{
  return mpForestUtil->GetIcacheMissCount (peid);
}


///<summary>Set break PC for the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="addr">break PC address</param>
///<returns>None</returns>
void ForestUtilScheap::AddBreakMatchPc (uint32_t peid, uint32_t addr)
{
  mpForestUtil->AddBreakMatchPc (peid, addr);
}


///<summary>Clear the designated break PC for designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="addr">breakpoint address</param>
///<returns>None</returns>
void ForestUtilScheap::ClearBreakMatchPc (uint32_t peid, uint32_t addr)
{
  mpForestUtil->ClearBreakMatchPc (peid, addr);
}


///<summary>Set breakpoint for the designated PE. In CycleExecute (), if a "PC of instruction & mask" is equal to "addr & mask", stop the simulation with breakpoint event before execution of the PC.</summary>
///<param name="peid">peid of target PE</param>
///<param name="addr">breakpoint address</param>
///<param name="mask">address mask</param>
///<returns>None</returns>
void ForestUtilScheap::AddBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask)
{
  mpForestUtil->AddBreakNextPc (peid, addr, mask);
}


///<summary>Clear the designated break point for designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="addr">breakpoint address</param>
///<param name="mask">address mask</param>
///<returns>None</returns>
void ForestUtilScheap::ClearBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask)
{
  mpForestUtil->ClearBreakNextPc (peid, addr, mask);
}


///<summary>Set access break for the designated PE. In CycleExecute (), if a "access address & mask" is inside of "start_addr & mask" and "end_addr & mask", stop the simulation with access break event after execution.</summary>
///<param name="peid">peid of target PE</param>
///<param name="start_addr">address break range (start)</param>
///<param name="end_addr">address break range (end)</param>
///<param name="attr">designate Read/Write/Read and Write</param>
///<param name="mask">address mask</param>
///<returns>None</returns>
void ForestUtilScheap::AddBreakAccess (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr, AccessAttr attr, uint32_t mask)
{
  mpForestUtil->AddBreakAccess (peid, start_addr, end_addr, attr, mask);
}


///<summary>Clear the designated access break for the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="start_addr">address break range (start)</param>
///<param name="end_addr">address break range (end)</param>
///<param name="attr">designate Read/Write/Read and Write</param>
///<param name="mask">address mask</param>
///<returns>None</returns>
void ForestUtilScheap::ClearBreakAccess (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr, AccessAttr attr, uint32_t mask)
{
  mpForestUtil->ClearBreakAccess (peid, start_addr, end_addr, attr, mask);
}


///<summary>Get the trace information</summary>
///<param name="scid">target scid</param>
///<returns>Trance information</returns>
const TraceInfo* ForestUtilScheap::GetTraceInfo (uint32_t scid) const
{
  return mpForestUtil->GetTraceInfo (scid);
}


///<summary>Instantiate CForest and setup internal parameters.</summary>
///<returns>None</returns>
void ForestUtilScheap::CreateForest (void)
{
  mpForestUtil->CreateForest ();
}


///<summary>Instantiate PE.</summary>
///<param name="clid">clid of target Cluster</param>
///<param name="peid">peid of target PE (0x0~0x7)</param>
///<param name="core_type">core type of the PE</param>
///<param name="core_verson">core version of the PE</param>
///<returns>None</returns>
void ForestUtilScheap::SetPe (uint32_t clid, uint32_t peid, CoreType core_type, CoreVersion core_verson)
{
  mpForestUtil->SetPe (clid, peid, core_type, core_verson);
}


///<summary>Designate base address for the guards.</summary>
///<param name="guard_id">guard id</param>
///<param name="allocation_id">allocation of guard</param>
///<param name="base_addr">Physical address of guard</param>
///<param name="is_valid">Existence of guard</param>
///<returns>true: Success, false: Failure. Invalid guard_id or allocation_id</returns>
bool ForestUtilScheap::SetGuard (uint32_t guard_id, uint32_t allocation_id, PhysAddr base_addr, bool is_valid)
{
  return mpForestUtil->SetGuard (guard_id, allocation_id, base_addr, is_valid);
}


///<summary>Designate parameter for the guards.</summary>
///<param name="guard_id">guard id</param>
///<param name="allocation_id">allocation of guard</param>
///<param name="param_id">parameter ID (0x0:CRG_ADDR_WIDTH, 0x1:CRG_CRG_CH_NUM, 0x2:CRG_CSG_CH_NUM, 0x3:CRG_KEYCODE)</param>
///<param name="value">setting value</param>
///<returns>true: Success, false: Failure. Invalid guard_id or allocation_id</returns>
bool ForestUtilScheap::SetGuardParam(uint32_t guard_id, uint32_t allocation_id, uint32_t param_id, uint32_t value)
{
  return mpForestUtil->SetGuardParam (guard_id, allocation_id, param_id, value);
}


///<summary>Instantiate peripherals.</summary>
///<returns>None</returns>
void ForestUtilScheap::GenPeripherals (void)
{
  mpForestUtil-> GenPeripherals();
}


///<summary>Information level message isn’t display when DEBUG_OFF is set.</summary>
///<param name="mode">Debug Mode(DBG_OFF | DBG_ON | DBG_RW | DBG_PIPE)</param>
///<returns>None</returns>
void ForestUtilScheap::SetDebugMode (DebugMode mode)
{
  mpForestUtil->SetDebugMode (mode);
}


///<summary>Run/Stop simulation of the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="is_run">true=run, fales=stop</param>
///<returns>None</returns>
void ForestUtilScheap::SetRunTarget(uint32_t peid, bool is_run)
{
  mpForestUtil->SetRunTarget (peid, is_run);
}


///<summary>Designate the clock ratio of target PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="ratio">clock ration of target PE</param>
///<returns>None</returns>
void ForestUtilScheap::SetFrequency(uint32_t peid, uint32_t ratio)
{
  mpForestUtil->SetFrequency (peid, ratio);
}


///<summary>Notice a error response for an operand access to the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<param name="syserr_cause">Cause code of Sysserr exception</param>
///<returns>None</returns>
IntErrorCode ForestUtilScheap::NoticeOperandErrorResponse (uint32_t peid, uint32_t syserr_cause)
{
  return mpForestUtil->NoticeOperandErrorResponse(peid, syserr_cause);
}

///<summary>Request the pseudo EIINT to the designated PE directly. INTC setting is ignored.</summary>
///<param name="peid">peid of target PE</param>
///<param name="channel">Interruption channel  number (0-2047)</param>
///<param name="priority">Interruption priority (0-15. 0 is high priority.)</param>
///<param name="eitb">Vector method (false: direct vector, true: table reference)</param>
///<returns>INT_E_OK: No error. INT_E_NOT_RECEIVED: Overwrite the request which is requested already. INT_E_CHANNEL: Invalid channel</returns>
IntErrorCode ForestUtilScheap::ReqInterruptToCpu (uint32_t peid, uint32_t channel, uint32_t priority, bool eitb)
{
  return mpForestUtil->ReqInterruptToCpu (peid, channel, priority, eitb);
}


///<summary>Request the pseudo FENMI to the designated PE directly. INTC setting is ignored.</summary>
///<param name="peid">peid of target PE</param>
///<returns>INT_E_OK: No error. INT_E_NOT_RECEIVED: FENMI is requested already.</returns>
IntErrorCode ForestUtilScheap::ReqFenmiToCpu (uint32_t peid)
{
  return mpForestUtil->ReqFenmiToCpu (peid);
}


///<summary>Request the pseudo FEINT to the designated PE directly. INTC setting is ignored.</summary>
///<param name="peid">peid of target PE</param>
///<param name="channel">Interruption channel  number (0-15)</param>
///<returns>INT_E_OK: No error. INT_E_NOT_RECEIVED: FEINT is requested already.</returns>
IntErrorCode ForestUtilScheap::ReqFeintToCpu (uint32_t peid, uint32_t channel)
{
  return mpForestUtil->ReqFeintToCpu (peid, channel);
}


///<summary>Request the pseudo SYSERR to the designated PE directly for G4MH 2.0.</summary>
///<param name="peid">peid of target PE</param>
///<param name="syserr_cause">syserr couse code</param>
///<returns>INT_E_OK: No error.</returns>
IntErrorCode ForestUtilScheap::ReqSyserrToCpu (uint32_t peid, uint32_t syserr_cause)
{
  return mpForestUtil->ReqSyserrToCpu (peid, syserr_cause);
}


///<summary>Request the pseudo SYSERR to the designated PE directly for G4MH 1.x.</summary>
///<param name="peid">peid of target PE</param>
///<param name="syserr_cause">syserr couse code</param>
///<returns>INT_E_OK: No error.</returns>
IntErrorCode ForestUtilScheap::ReqPseudoFetchSyserrToCpu (uint32_t peid, uint32_t syserr_cause)
{
  return mpForestUtil->ReqPseudoFetchSyserrToCpu (peid, syserr_cause);
}


///<summary>Cansel the pasedo SYSERR to the designated PE directly.</summary>
///<param name="peid">peid of target PE</param>
///<returns>INT_E_OK: No error.</returns>
IntErrorCode ForestUtilScheap::CancelSyserrToCpu (uint32_t peid)
{
  return mpForestUtil->CancelSyserrToCpu (peid);
}


///<summary>Cancel the pseudo EIINT/BGEIINT/GMEIINT request for the designated PE directly. INTC setting is ignored</summary>
///<param name="peid">peid of target PE</param>
////<returns>INT_E_OK: No error. INT_E_CHANNEL: Invalid channel</returns>
IntErrorCode ForestUtilScheap::CancelInterruptToCpu (uint32_t peid)
{
  return mpForestUtil->CancelInterruptToCpu (peid);
}


///<summary>Cancel the pseudo EIINT request for the designated PE directly. INTC setting is ignored</summary>
///<param name="peid">peid of target PE</param>
///<param name="channel">Interruption channel number (0-2047)</param>
///<returns>INT_E_OK: No error. INT_E_CHANNEL: Invalid channel</returns>
IntErrorCode ForestUtilScheap::CancelInterruptToCpu (uint32_t peid, uint32_t channel)
{
  return mpForestUtil->CancelInterruptToCpu (peid, channel);
}


///<summary>Cancel the pseudo FENMI request for the designated PE directly. INTC setting is ignored.</summary>
///<param name="peid">peid of target PE</param>
///<returns>INT_E_OK: No error.</returns>
IntErrorCode ForestUtilScheap::CancelFenmiToCpu (uint32_t peid)
{
  return mpForestUtil->CancelFenmiToCpu (peid);
}


///<summary>Cancel the pseudo FEINT request for the designated PE directly. INTC setting is ignored</summary>
///<param name="peid">peid of target PE</param>
///<returns>INT_E_OK: No error.</returns>
IntErrorCode ForestUtilScheap::CancelFeintToCpu (uint32_t peid)
{
  return mpForestUtil->CancelFeintToCpu (peid);
}


///<summary>Set ROM area</summary>
///<param name="start_addr">ROM range (start address)</param>
///<param name="end_addr">ROM range (end address)</param>
///<returns>None</returns>
void ForestUtilScheap::SetRomArea (PhysAddr start_addr, PhysAddr end_addr)
{
  mpForestUtil->SetRomArea (start_addr, end_addr);
}


///<summary>Set CRAM area</summary>
///<param name="start_addr">ROM range (start address)</param>
///<param name="end_addr">ROM range (end address)</param>
///<returns>None</returns>
void ForestUtilScheap::SetCramArea (PhysAddr start_addr, PhysAddr end_addr)
{
  mpForestUtil->SetGramArea (start_addr, end_addr);
}


///<summary>Set CFG function enable</summary>
///<param name="en">CFG function enable (not used at E2xFCC1)</param>
///<returns>None</returns>
void ForestUtilScheap::SetCfgProtEnable(bool en)
{
}


///<summary>Set CFG protection region</summary>
///<param name="addr">CFG protecion region address (not used at E2xFCC1)</param>
///<returns>None</returns>
void ForestUtilScheap::SetCfgProtRegion(PhysAddr addr)
{
}


///<summary>Get an executed instruction count of the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<returns>Executed instruction count</returns>
uint64_t ForestUtilScheap::GetStepCount (uint32_t peid) const
{
  return mpForestUtil->GetStepCount (peid);
}


///<summary>Get an elapsed cycle count of the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<returns>Elapsed cycle count</returns>
uint64_t ForestUtilScheap::GetCycleCount (uint32_t peid) const
{
  return mpForestUtil->GetCycleCount (peid);
}


///<summary>Get an exception cause code of previous instruction on the designated PE.</summary>
///<param name="peid">peid of target PE</param>
///<returns>EXP_NONE : No exception, Other : Exception cause code</returns>
ExpCause ForestUtilScheap::GetExpCause (uint32_t peid) const
{
  return mpForestUtil->GetExpCause (peid);
}


///<summary>Set Fsbus's instance pointer</summary>
///<returns>None</returns>
void ForestUtilScheap::SetFsbusPointer(void)
{
  mpFsBus = mpForestUtil->GetFsbusPointer ();
}


///<summary>Open the trace file</summary>
///<param name="fname">the neme of trace file</param>
///<returns>file pointer of the trace file</returns>
FILE* ForestUtilScheap::SetFnameAndOpen(char* fname)
{
  return mpForestUtil->SetFnameAndOpen(fname);
}


///<summary>Add the bus I/F IP to fsbus</summary>
///<param name="start_addr">IP range (start address)</param>
///<param name="end_addr">IP range (end address)</param>
///<param name="tgt_vc">the pointer of bus I/F IP</param>
///<param name="id">peid of targe PE</param>
///<param name="attr">Gruard attribute</param>
///<returns>None</returns>
void ForestUtilScheap::AddTargetVC(PhysAddr start_addr, PhysAddr end_addr, CModuleVc* tgt_vc, uint32_t id, GuardAttr attr)
{
  if (mpFsBus != NULL){
    if (attr == GUARD_NONE){
      ModuleGuardDummy* m_dummy_guard = new ModuleGuardDummy();
      mpFsBus->AddTargetVC (start_addr, end_addr, tgt_vc, NULL, m_dummy_guard, new MemLatencyInfoSCHEAP ());
    }else{
      mpFsBus->AddTargetVC (start_addr, end_addr, tgt_vc, NULL, mpForestUtil->GetGuardPtr(id, attr), new MemLatencyInfoSCHEAP ());
    }
  }
}


///<summary>get the pointer of ForestUtilApi</summary>
///<returns>the pointer of ForestUtilApi</returns>
ForestUtilApi* ForestUtilScheap::GetForestUtilApi(void)
{
  return static_cast<ForestUtilApi*> (mpForestUtil);
}
