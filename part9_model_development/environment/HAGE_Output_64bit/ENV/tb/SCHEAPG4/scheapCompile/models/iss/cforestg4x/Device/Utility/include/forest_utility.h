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

#include <vector>
#include <string>
#include "./forest_utility_api.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./forest_internals.h"
#include "./lsu.h"

class Cforest;
class ForestUtil;
class ProcElement;
class FsCtrl;
class ModuleRun;
class Csmem;

extern ForestUtil* p_forest_util;

#define OPT_STR_MAX 128

class DLL_EXPORT_IMPORT ForestUtil : public ForestUtilApi {
private:
    static const int32_t  SCID_MASK = 0xFFFF;
    static const uint32_t BREAK_ADDR_MASK = 0xFFFFFFFF;
    static const uint32_t IOR_OUTSTAND_NUM = 2;
    static const uint32_t IOR_REPEAT_RATE  = 4; // VPI=80MHz, CORE=320MHzの想定値
    static const uint32_t API_ERR_VAL = 0;

    static const PhysAddr GLOBAL_RAM_BGN = 0xFE000000U;
    static const PhysAddr GLOBAL_RAM_END = 0xFEFFFFFFU;

    DebugMode m_debug_mode;
    PhysAddr m_rom_start_addr;
    PhysAddr m_rom_end_addr;
    PhysAddr m_gram_start_addr;
    PhysAddr m_gram_end_addr;
    bool m_is_uart_instance;
    uint16_t m_uart_socket_port;
    uint32_t m_uart_channel_num;

#ifdef ENABLE_CLUSTER
    typedef std::pair<uint32_t, uint32_t> KeepClItem;      // <master, slave>
    typedef std::pair<uint32_t, KeepClItem> KeepClMapElm;  // <clid, <master, slave>>
    typedef std::map<uint32_t, KeepClItem> KeepClMap;
    KeepClMap m_keep_cl_latency;
    typedef std::pair<uint32_t, uint32_t> KeepPeItem;      // <master, slave>
    typedef std::pair<uint32_t, KeepPeItem> KeepPeMapElm;  // <peid, <master, slave>>
    typedef std::map<uint32_t, KeepPeItem> KeepPeMap;
    KeepPeMap m_keep_pe_latency;
#endif // ENABLE_CLUSTER

    uint32_t m_dbg_api_peid;
    uint32_t m_eiint_channel_offset;

public:
    static const uint32_t ROM_FETCH_LATENCY = 1; // rom_wait値が加算される
    static const uint32_t LRAM_FETCH_LATENCY = 8;

    static const uint16_t UART_PORT_ID_SOCKET = 0xFFFFU;

    Cforest* m_forest;
    typedef std::vector<FsCtrl*> HtVec;
    // typedef std::vector<ProcElement*> PeVec;
    #include "proc_element_vector.h"
    typedef std::vector<ModuleRun*> RunList;
    typedef std::vector<Csmem*> SmemVec;
    HtVec m_cedar;
    RunList m_cedar_run;
    ForestInternals* m_internals;
    void SetMessageLevel (MessageLevel lvl) { m_internals->SetMessageLevel (lvl); }
    MessageLevel GetMessageLevel (void) const { return m_internals->GetMessageLevel (); }

    uint64_t GetSystemTime (void) const;
    uint64_t GetCpuTime (uint32_t peid) const;

    static bool m_stop_req;
    static void ReqSimStop (int sig) { m_stop_req = true; }

    //exitステータス用
    int GetExitStatus (void) const;

    uint8_t m_memini_value;
    void SetMemInit (uint8_t memini) { m_memini_value = memini; }
    uint8_t GetMemInit (void) const { return m_memini_value; }

    bool m_isstrace_flag;

    bool m_is_break_halt_enabled;
    void EnableHaltBreak (bool enable) { m_is_break_halt_enabled = enable; }
    bool IsEnabledHaltBreak (void) const { return m_is_break_halt_enabled; }

    bool IsAllInHalt (void) { return m_internals->IsAllInHalt(); }

#ifdef CLUSTER
    static const bool INIT_EN_BUS_TRACE = false;
    bool m_is_en_bus_trace;
    void SetBusTrace (bool is_en) { m_is_en_bus_trace = is_en; }
    bool IsEnableBusTrace (void) const { return m_is_en_bus_trace; }
#endif // CLUSTER

    void EnableBreakAccStopio (void);

    static const uint32_t INIT_ROM_WAIT = 5;
    uint32_t m_rom_wait;
    void SetRomWait (uint32_t rom_wait) { m_rom_wait = rom_wait; }
    uint32_t GetRomWait (void) const { return m_rom_wait; }

    ErrorCode m_cube_err_code;

    ForestUtil ();
    ~ForestUtil ();

    template <RunMode run_mode>
    int32_t StepExecute (int32_t scid, uint64_t step_goal);
#if defined(COMP_RUNNER)
    void SetCanselTimingMcycInst (int32_t scid, int32_t completed_count);

    PipeStage m_cmp_stage;
    void SetCmpStage (PipeStage cmp_stage) { m_cmp_stage = cmp_stage; }
    PipeStage GetCmpStage (void)const { return m_cmp_stage; }
    uint64_t m_start_pc;
    uint64_t m_end_pc;
    void SetCycCmpStartPC (uint64_t start_pc) { m_start_pc = start_pc; }
    void SetCycCmpEndPC (uint64_t end_pc) { m_end_pc = end_pc; }
    uint64_t GetCycCmpStartPC (void) const { return m_start_pc; }
    uint64_t GetCycCmpEndPC (void) const { return m_end_pc; }
#endif // defined(COMP_RUNNER)
#if defined(COMP_RUNNER_G3KH)
    int32_t ForestStepModeExecute (int32_t scid);
#endif // defined(COMP_RUNNER_G3KH)
    void SkipHaltHt(void);
    void HardReset (void);
    /**
     * \fn void FsPeReset (uint32_t peid);
     * \brief Request to CForest to reset function of individual PE.
     *
     * This function is used by python command reset(self, *contextid)
     *                     and set_event (address, "pereset", context)
     *
     * @param[in] peid               The source will specify which PE will go to RESET state.
     * @return boolean value
     * @retval true                : The PE was reset completely. Last PC was updated to FEPC.
     * @retval false               : The PE can't reset completely.
     */
    bool FsPeReset (uint32_t peid);
    /**
     * \fn void HardReset (void);
     * \brief Request to reset function and cycle of individual PE.
     *
     * This function is used by python command reset().
     *
     * @param -.
     * @return None
     */
    void ResetPE (uint32_t peid);
    void ResetVM (uint32_t peid);

#ifdef ENABLE_CLUSTER
    void UpdateExoClLatency (uint32_t clid,
                             uint32_t master_latency_offset, uint32_t slave_latency_offset);
    void UpdateExoPeLatency (uint32_t  peid,
                             uint32_t master_latency_offset, uint32_t slave_latency_offset);
#else // ENABLE_CLUSTER
    void UpdateExoPeLatency (uint32_t exo_latency_offset);
#endif // ENABLE_CLUSTER

    bool IorResourceAlloc (MemRestrictType restrict_type,
                           PhysAddr start_addr,
                           PhysAddr end_addr,
                           uint32_t pe_read_latency,
                           uint32_t pe_write_latency,
                           uint32_t cl_read_latency,
                           uint32_t cl_write_latency,
                           uint32_t exo_cl_read_latency,
                           uint32_t exo_cl_write_latency,
                           MemLocationType pe_location,
                           MemLocationType cl_location,
                           uint32_t num_outstand,
                           uint32_t repeat_rate);
    bool IorResourceFree  (PhysAddr start_addr);
    bool SetPeripheralLatency (PhysAddr start_addr,
                               PhysAddr end_addr,
                               uint32_t pe_read_latency,
                               uint32_t pe_write_latency,
                               uint32_t cl_read_latency,
                               uint32_t cl_write_latency,
                               uint32_t exo_cl_read_latency,
                               uint32_t exo_cl_write_latency,
                               MemLocationType pe_location,
                               MemLocationType cl_location,
                               uint32_t num_outstand,
                               uint32_t repeat_rate);
    bool LocalIorResourceAlloc (MemRestrictType restrict_type,
                                PhysAddr start_addr,
                                PhysAddr end_addr,
                                uint32_t pe_read_latency,
                                uint32_t pe_write_latency,
                                uint32_t cl_read_latency,
                                uint32_t cl_write_latency,
                                uint32_t exo_cl_read_latency,
                                uint32_t exo_cl_write_latency,
                                MemLocationType pe_location,
                                MemLocationType cl_location,
                                uint32_t num_outstand,
                                uint32_t repeat_rate);
    bool LocalIorResourceFree  (PhysAddr start_addr, PhysAddr end_addr);
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
#ifdef CLUSTER
    bool UpdateClusterCycleInfo (char *bridge_name,
                                 uint32_t clid,
                                 PhysAddr start_addr,
                                 PhysAddr end_addr,
                                 MemRestrictType restrict_type,
                                 uint32_t read_latency,
                                 uint32_t write_latency,
                                 uint32_t fetch_latency,
                                 uint32_t outstand,
                                 uint32_t repeat_rate);
#endif // CLUSTER
    bool MemResourceFree (PhysAddr start_addr, PhysAddr end_addr);
    bool SetMirrorArea (PhysAddr start_addr, PhysAddr end_addr,
                        PhysOffset offset);
    bool ClearMirrorArea (PhysAddr addr);
    bool SetErrorArea (PhysAddr start_addr, PhysAddr end_addr);
    bool ClearErrorArea (PhysAddr addr);
    bool SetSelfArea (PhysAddr start_addr, PhysAddr end_addr,
                      PhysOffset offset);
    bool ClearSelfArea (PhysAddr addr);
    bool SetAccessDeniedArea (PhysAddr start_addr, PhysAddr end_addr);
    bool ClearAccessDeniedArea (PhysAddr addr);
    std::string MemResourceDisplay (void);
    std::string PeripheralMapDisplay (void);
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
    uint32_t GetExtPin (uint32_t peid, ExtPinType pin) const;

    uint32_t GetPeNum (void) const;
    uint32_t GetPeLastId (void) const;
    uint32_t GetPeMasterId (void) const;
    uint32_t GetPeidFromBmid (uint32_t bmid) const;
    uint32_t GetBmidFromPeid (uint32_t peid) const;
    uint32_t GetClidFromPeid (uint32_t peid) const;
    uint32_t GetClidFromBmid (uint32_t bmid) const;
    int32_t GetVcLastId (uint32_t peid) const;
    int32_t GetTcLastId (uint32_t peid) const;
    int32_t GetScNum (void) const;
    CoreType GetCoreType (uint32_t peid) const;
    CoreVersion GetCoreVersion (uint32_t peid) const;

    void SetTcDebugMode (uint32_t peid, DebugMode mode);
    void SetDebugMode (DebugMode mode);
    void DisableDebugOption (std::string option);
    DebugMode GetDebugMode (void) const { return m_debug_mode; }
    void UpdateExpMsgRange (uint32_t peid,
                            ExpCause start_cause, ExpCause end_cause, bool is_print);

    void SetRunTarget(uint32_t peid, bool is_run);
    void SetFrequency(uint32_t peid, uint32_t ratio);
    uint32_t GetFrequency(uint32_t peid);
    CReg RegGetRegno (uint32_t peid, std::string regname);
    uint32_t GetPeId (uint32_t scid) const;
    int32_t GetScId (uint32_t peid) const;

    uint32_t GetIcacheWayNum (uint32_t peid) const;
    void SetIcacheWayNum (uint32_t peid, uint32_t way);
    ByteSize GetIcacheSize (uint32_t peid) const;
    void SetIcacheSize (uint32_t peid, ByteSize size);
    bool GetIcacheEccState (uint32_t peid) const;
    void SetIcacheEccState (uint32_t peid, bool enable);
    uint64_t GetIcacheAccessCount (uint32_t peid) const;
    uint64_t GetIcacheHitCount (uint32_t peid) const;
    uint64_t GetIcacheMissCount (uint32_t peid) const;

    uint32_t GetDcacheWayNum (uint32_t peid) const;
    void SetDcacheWayNum (uint32_t peid, uint32_t way);
    ByteSize GetDcacheSize (uint32_t peid) const;
    void SetDcacheSize (uint32_t peid, ByteSize size);
    bool GetDcacheEccState (uint32_t peid) const;
    void SetDcacheEccState (uint32_t peid, bool enable);
    uint64_t GetDcacheAccessCount (uint32_t peid) const;
    uint64_t GetDcacheHitCount (uint32_t peid) const;
    uint64_t GetDcacheMissCount (uint32_t peid) const;
    uint64_t GetDcacheWbCount (uint32_t peid) const;
    void SetDcacheFillLatency (uint32_t latency);
    void SetDcacheHitLatency (uint32_t latency);

    void DumpBtbStatistics (uint32_t peid) const;
    void ResetBtbStatistics (uint32_t peid);

    ModuleGuard* GetGuardPtr (uint32_t id, GuardAttr attr);

#ifdef SUPPORT_SC_HEAP
    BreakInfo* GetBreakInfo (void);
    uint64_t CycleExecute (uint32_t peid, uint64_t cycle_goal, ExecType exec_type);
    void NotifyFetchLatency (SideBand sideband, PhysAddr addr, uint32_t latency);
    void NotifyReadLatency (SideBand sideband, PhysAddr addr, uint32_t latency, uint64_t data);
    void NotifyWriteLatency (SideBand sideband, PhysAddr addr, uint32_t latency);

    const TraceCtrl* DequeueTrace (void);
    uint32_t GetUsedTraceSize (void);
    CfApiRetern SetMaxTraceSize (uint32_t num);
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
    SysErrCause BusFetchPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool SetPeLramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr);
#endif // SUPPORT_SC_HEAP

    void AddBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask);
    void ClearBreakNextPc (uint32_t peid, uint32_t addr, uint32_t mask);
    void AddBreakAccess (uint32_t peid, PhysAddr start_addr,
                         PhysAddr end_addr, AccessAttr attr, uint32_t mask);
    void DisplayBreakAccess (uint32_t peid);
    void ClearBreakAccess (uint32_t peid, PhysAddr start_addr,
                           PhysAddr end_addr, AccessAttr attr, uint32_t mask);

    bool LoadSrec (const char* filename, PhysAddr offset);
    void EnableIssTrace (void);
    bool IsEnabledIssTrace () const {return m_isstrace_flag; }

    std::string GetVersionStr (void) const;
    void PrintVersion (FILE* fp) const;

    void SetFileOut (FILE* fp);

    FILE* m_global_log;
    std::vector<FILE*> m_logs;
    FILE* OpenFileOut (std::string fname);
    bool OpenSeparatedFileOut (std::string fname);

    int32_t GetNextScidOnPe (uint32_t peid) const;
    int32_t GetCurrentTcScId (void) const;
    const TraceInfo* GetTraceInfo (uint32_t scid) const;
    const TraceCtrl* GetTraceCtrl (uint32_t scid) const;
    void SetCubeErrorCode (ErrorCode err);
    ErrorCode GetCubeErrorCode (void);

    BreakEvent GetBreakEvent (void) const;
    PhysAddr GetBreakPc (void) const;
    void DisplayBreakPc (uint32_t peid);
    PhysAddr GetAccBreakAddr (void) const;
    void ClearBreakEvent (void);

    CfApiRetern GetStatus (uint32_t peid, NotifySimState* state) const;

    void CreateForest (void);
    void SetPe (uint32_t clid, uint32_t peid, CoreType core_type, CoreVersion core_version);

    bool IsValidGuardModuleId (uint32_t id) const;
    bool IsEnableGuard (GuardModuleID guard_id, uint32_t location_id) const;
    PhysAddr GetGuardAddr (GuardModuleID guard_id, uint32_t location_id) const;
    void SetGuardAddr (GuardModuleID guard_id, uint32_t location_id, PhysAddr addr);
    void SetGuardEnable (GuardModuleID guard_id, uint32_t location_id, bool is_enable);
    bool SetGuard (uint32_t guard_id, uint32_t allocation_id,
                   PhysAddr base_addr, bool is_valid);
    bool SetGuardParam (uint32_t guard_id, uint32_t allocation_id,
                        uint32_t param_id, uint32_t value);
    bool GetGuardParam (uint32_t guard_id, uint32_t allocation_id,
                        uint32_t param_id, uint32_t* value) const;
    bool IsValidPeipheralId (SetIORBaseAddrID id) const;
    bool IsValidGuardParamId (uint32_t guard_id, uint32_t param_id) const;
    bool IsValidGuardParamValue (uint32_t param_id, uint32_t value) const;

    void GenPeripherals (void);
    bool SetPeripheral (uint32_t id, PhysAddr base_addr, bool is_valid);
    void SetPeripheralOffsetAddr (SetIORBaseAddrID peri_id,
                                  PhysOffset offset_addr);
    PhysOffset GetPeripheralOffsetAddr (SetIORBaseAddrID peri_id) const;
    void SetPeripheralBaseAddr (SetIORBaseAddrID peri_id,
                                PhysAddr base_addr);
    PhysAddr GetPeripheralBaseAddr (SetIORBaseAddrID peri_id) const;
    void EnablePeripheral (SetIORBaseAddrID peri_id, bool is_enabled);
    bool IsPeripheralEnabled (SetIORBaseAddrID peri_id) const;
    void SetBreakMax (uint64_t max_count);
    uint64_t GetBreakMax (void);
    void SetBreakSamePcMax (uint64_t max_count);
    uint64_t GetBreakSamePcMax (void) const;
    void SetBreakSystime (uint64_t max_count);
    uint64_t GetBreakSystime (void) const;
    void UpdateNextTcInfo (int32_t curr_scid, int32_t next_scid);

    void CubeStepExecute (void);

    uint64_t GetStepCount (uint32_t peid) const;
    uint64_t GetCycleCount (uint32_t peid) const;
    ExpCause GetExpCause (uint32_t peid) const;

#if defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G4MH)
    bool m_comp_misr_show;
    void SetCompMisrShow (bool comp_misr_show) { m_comp_misr_show = comp_misr_show; }
    bool IsCompMisrShow (void)const {return m_comp_misr_show; }
#endif // defined(COMP_RUNNER_G3MH) || defined(COMP_RUNNER_G4MH)

#ifdef ENABLE_PIC
    void DisplayInstructionCount (uint32_t peid, FILE* local_file);
    void ClearInstructionCount (uint32_t peid);
#endif // ENABLE_PIC
    IntErrorCode ReqInterruptToIntc1 (uint32_t peid, uint32_t channel);
    IntErrorCode ReqInterruptToIntc2 (uint32_t channel);
    IntErrorCode ReqFenmiToIntc1 (uint32_t peid);
    IntErrorCode ReqFeintToIntc1 (uint32_t peid, uint32_t channel);
#if defined(COMP_RUNNER_G4MH)
    /**
     * \fn IntErrorCode ForestUtil::ReqPseudoInterruptToIntc1 (uint32_t peid, uint32_t channel);
     * \brief Eiint request from CompRunner to INCT1
     *
     * @param[in]  peid                PEID.
     * @param[in]  channel             EIINT channel.
     * @return None.
     */
    IntErrorCode ReqPseudoInterruptToIntc1 (uint32_t peid, uint32_t channel);

    /**
     * \fn IntErrorCode ForestUtil::ReqPseudoFenmiToIntc1 (uint32_t peid);
     * \brief FENMI request from CompRunner to INCT1
     *
     * @param[in]  peid                PEID.
     * @return None.
     */
    IntErrorCode ReqPseudoFenmiToIntc1 (uint32_t peid);

    /**
     * \fn IntErrorCode ForestUtil::ReqPseudoFeintToIntc1 (uint32_t peid, uint32_t channel);
     * \brief FEINT request from CompRunner to INCT1
     *
     * @param[in]  peid                PEID.
     * @param[in]  channel             FEINT channel.
     * @return None.
     */
    IntErrorCode ReqPseudoFeintToIntc1 (uint32_t peid, uint32_t channel);
#endif
    IntErrorCode ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                    uint32_t priority, bool eitb)
    {
       return ReqInterruptToCpu (peid, channel, priority, eitb, false, 0, false); // req EIINT
    }
    IntErrorCode ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                    uint32_t priority, bool eitb,
                                    bool is_gm, uint32_t gpid, bool is_bgint);
#if defined(COMP_RUNNER)
    bool ReqFpiToCpu (uint32_t peid);
    void ReqAsyncDataBreakToCpu (uint32_t peid);
    IntErrorCode ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                    uint32_t priority, bool eitb,
                                    bool is_instdone)
    {
        return ReqInterruptToCpu (peid, channel, priority, eitb, false, 0, false, is_instdone); // req EIINT
    }
    IntErrorCode ReqInterruptToCpu (uint32_t peid, uint32_t channel,
                                    uint32_t priority, bool eitb,
                                    bool is_gm, uint32_t gpid, bool is_bgint,
                                    bool is_instdone);
#endif // defined(COMP_RUNNER)
    IntErrorCode ReqFenmiToCpu (uint32_t peid);
    IntErrorCode ReqFeintToCpu (uint32_t peid, uint32_t channel)
    {
        return ReqFeintToCpu (peid, channel, false, 0, false); // req FEINT
    }
    IntErrorCode ReqFeintToCpu (uint32_t peid, uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint);
#if defined(COMP_RUNNER)
    IntErrorCode ReqFenmiToCpu (uint32_t peid, bool is_instdone);
    IntErrorCode ReqFeintToCpu (uint32_t peid, bool is_instdone, uint32_t channel)
    {
        return ReqFeintToCpu (peid, is_instdone, channel, false, 0, false); // req FEINT
    }
    IntErrorCode ReqFeintToCpu (uint32_t peid, bool is_instdone, uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint);
    /**
     * \fn IntErrorCode ReqHaltSnoozeReleaseToCpu(uint32_t peid);
     * \brief Request Halt/Snooze release to CPU
     *
     * @param[in]  peid          PEID.
     *
     * @return IntErrorCode.
     * @retval INT_E_CHANNEL     Invalid peid.
     * @retval INT_E_OK          There's no error.
     *
     */
    IntErrorCode ReqHaltSnoozeReleaseToCpu (uint32_t peid);
#endif // defined(COMP_RUNNER)
    IntErrorCode ReqPseudoFetchSyserrToCpu (uint32_t peid, uint32_t syserr_cause); // Cause fetch SYSERR for current mode of PE
    IntErrorCode ReqSyserrToCpu (uint32_t peid, uint32_t syserr_cause); // Cause operand SYSERR for current mode of PE (Clear all bit in DEVDS)
    IntErrorCode ReqSyserrToCpu (uint32_t peid, uint32_t syserr_cause, bool is_host, uint32_t gpid); // Notice operand SYSERR for designated mode (Clear 1bit in DEVDS)
#if defined(COMP_RUNNER_G4MH)
    /**
     * \fn IntErrorCode ReqOperandSyserrToCpu (uint32_t peid, uint32_t syserr_cause, bool is_host, uint32_t gpid);
     * \brief Request operand syserr to CPU
     *
     * @param[in]  peid          PEID.
     * @param[in]  syserr_cause  syserr cause.
     * @param[in]  is_host       syserr mode.
     * @param[in]  gpid          syserr gpid.
     *
     * @return IntErrorCode.
     * @retval INT_E_CHANNEL     Invalid peid.
     * @retval INT_E_OK          There's no error.
     *
     */
    IntErrorCode ReqOperandSyserrToCpu (uint32_t peid, uint32_t syserr_cause, bool is_host, uint32_t gpid);
    /**
     * \fn bool GetOperandSyserrStatus (bool is_host, uint32_t gpid) const;
     * \brief Get operand sysesrr status flag
     * This function returns status of operand syserr flag
     *
     * @param[in]  peid          PEID.
     * @param[in]  is_host       syserr mode.
     * @param[in]  gpid          syserr gpid.
     *
     * @return bool value.
     * @retval true         flag is set.
     * @retval false        flag is not set.
     *
     */
    bool GetOperandSyserrStatus (uint32_t peid, bool is_host, uint32_t gpid);
#endif
    IntErrorCode NoticeOperandErrorResponse (uint32_t peid, uint32_t syserr_cause);
    IntErrorCode NoticeOperandErrorResponse (uint32_t peid, uint32_t syserr_cause,
                                             bool is_gm, uint32_t gpid);
    IntErrorCode CancelInterruptToCpu (uint32_t peid, uint32_t channel);
    IntErrorCode CancelInterruptToCpu (uint32_t peid);
    IntErrorCode CancelFenmiToCpu (uint32_t peid);
    IntErrorCode CancelFeintToCpu (uint32_t peid);
    IntErrorCode CancelSyserrToCpu (uint32_t peid);
    IntErrorCode CancelSyserrToCpu (uint32_t peid, bool is_host, uint32_t gpid);
    IntErrorCode CancelPseudoFetchSyserrToCpu (uint32_t peid);

    IntErrorCode ReqRlbToCpu (uint32_t peid, uint32_t source_peid);
    IntErrorCode CancelRlbToCpu (uint32_t peid, uint32_t source_peid);
    IntErrorCode ReqDbnmiToCpu (uint32_t peid);
    IntErrorCode CancelDbnmiToCpu (uint32_t peid);
    IntErrorCode ReqDbintToCpu (uint32_t peid, uint32_t channel);
    IntErrorCode CancelDbintToCpu (uint32_t peid);
    IntErrorCode ReqRmintToCpu (uint32_t peid);
    IntErrorCode CancelRmintToCpu (uint32_t peid);
    /**
     * \fn IntErrorCode ReqAllInterruptDisableToCpu (void);
     * \brief Request to CPU for setting the disabled assertion in all interruption types.
     *
     * This function is called many times in the following cases..
     * 1. Support stopreq signal for FROG I/F operation.
     *
     * @param[in] rtl_peid           The source will specify which PE will be requested to CPU.
     * @return IntErrorCode
     * @retval #INT_E_OK           : This return value means there is no error in this request.
     *                               The operation will be proceeded normally.
     * @retval #INT_E_CHANNEL      : This return value means the channel id is illegal.
     *                               There is no operation for this case.
     * @retval #INT_E_NOT_RECEIVED : This return value means there is no-response channel for this request in normal case.
     *                               In case this is the request for disabling the assertion for all interruption types,
     *                               it means this bit is set before.
     * @retval #INT_E_SYSERR_CODE  : This return value means there is Illegal syserr code.
     *                               There is no operation for this case.
     */
    IntErrorCode ReqAllInterruptDisableToCpu (uint32_t peid);
    /**
     * \fn IntErrorCode CancelAllInterruptDisableToCpu (void);
     * \brief Request to CPU for canceling the disabled assertion in all interruption types.
     *
     * This function is called many times in the following cases..
     * 1. Support stopreq signal for FROG I/F operation.
     *
     * @param[in] rtl_peid           The source will specify which PE will be requested to CPU.
     * @return IntErrorCode
     * @retval #INT_E_OK           : This return value means there is no error in this request.
     *                               The operation will be proceeded normally.
     * @retval #INT_E_CHANNEL      : This return value means the channel id is illegal.
     *                               There is no operation for this case.
     * @retval #INT_E_NOT_RECEIVED : This return value means there is no-response channel for this request in normal case.
     *                               In case this is the request for disabling the assertion for all interruption types,
     *                               it means this bit is not set before.
     * @retval #INT_E_SYSERR_CODE  : This return value means there is Illegal syserr code.
     *                               There is no operation for this case.
     */
    IntErrorCode CancelAllInterruptDisableToCpu (uint32_t peid);

    bool HaveSyserrReq (uint32_t peid) const;
    bool HaveDbnmiReq (uint32_t peid) const;
    bool HaveDbintReq (uint32_t peid) const;
    bool HaveRmintReq (uint32_t peid) const;
    bool HaveFenmiReq (uint32_t peid) const;
    bool HaveFeintReq (uint32_t peid) const;

#ifdef FROG_API_IF
    bool IsRomArea (PhysAddr addr);
#endif //FROG_API_IF
    void SetRomArea (PhysAddr start_addr, PhysAddr end_addr);
    void SetLocalRamArea (PhysAddr start_addr, PhysAddr end_addr);
    void SetDebugArea (PhysAddr start_addr, PhysAddr end_addr, uint32_t peid);
    void SetGramArea (PhysAddr start, PhysAddr end);
    bool IsCramArea (PhysAddr addr) const { return (addr >= m_gram_start_addr) && (addr <= m_gram_end_addr); }

    bool SetIntc1 (uint32_t peid, PhysAddr start_addr);
    bool SetIntc2 (PhysAddr start_addr);
    bool ClearDebugArea (PhysAddr start_addr);
    void SetDCacheArea (PhysAddr start_addr1, PhysAddr end_addr1, PhysAddr start_addr2, PhysAddr end_addr2);
    void GetDCacheArea (PhysAddr* dc_area);
    void SetAdditonalFetchStage (uint32_t num);

    bool SetOstmLocal (uint32_t bind_pe, uint32_t ostmch, PhysAddr start_addr, uint32_t intch);
    bool SetOstmGlobal (PhysAddr start_addr, uint32_t intch);

    bool IsValidPeId (uint32_t peid) const;
    bool IsValidBmId (uint32_t bmid) const;
    bool IsValidScId (int32_t scid) const;

#ifdef CCG3M_DBUF
    static const uint32_t DBUF_HIT_LATENCY = 2;
#else // CCG3M_DBUF
    static const uint32_t DBUF_HIT_LATENCY = 5;
#endif // CCG3M_DBUF
    uint32_t m_dbuf_hit_latency;
    void SetDbufHitLatency (uint32_t latency) { m_dbuf_hit_latency = latency; }

    bool CreateUart (void);
    void SetIsUartInstance (bool instance) { m_is_uart_instance = instance; }
    void SetUartSocketPort (uint16_t port) { m_uart_socket_port = port; }
    void SetUartChannelNum (uint32_t num) { m_uart_channel_num = num; }
    bool IsUartInstance (void) const { return m_is_uart_instance; }
    uint16_t GetUartSocketPort (void) const { return m_uart_socket_port; }
    uint32_t GetUartChannelNum (void) const { return m_uart_channel_num; }

    bool m_show_staticstics;
    static const bool INIT_SHOW_STATISTICS = false;
    void SetShowStatistics (bool is_show) { m_show_staticstics = is_show; }
    bool GetShowStatistics (void) const { return m_show_staticstics; }
    void ShowReg ();

    static const int32_t INIT_FPU_LATENCY = 0;
    void SetFpuLatencyOffset (int32_t offset);
    static const int32_t INIT_DIVF_LATENCY = 0;
    void SetDivfLatencyOffset (int32_t offset);

    void ShowPerfInfo (uint32_t peid) const;
    void ResetPerfInfo (uint32_t peid);
#ifdef CLUSTER
    void ShowClPerfInfo (void) const;
    void ResetClPerfInfo (void);
#endif // CLUSTER


    bool m_work_perfinfo_with_perfcnt;
    void SetWorkPerfInfoWithPerfcnt (bool is_enabled)
    { m_work_perfinfo_with_perfcnt = is_enabled; }
    bool IsPerfInfoWorkWithPerfcnt (void) const { return m_work_perfinfo_with_perfcnt; }

    void ExecCfali (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr);

#ifdef ENABLE_PEID_POS_CTRL
    void SetPeidPosG4x (bool is_g4x);
#endif // #ifdef ENABLE_PEID_POS_CTRL

    void SetDbufDepth (uint32_t depth);
    void SetDbufLineShift (uint32_t shift);
    void SetDbufFillBurstNum (uint32_t num);
    void SetDbufPreloadType (DbufPreloadType type);
#ifdef USE_IC_LINE_SIZE_CMDOPT
    void SetIcLineByte (uint32_t size);
#endif //#ifdef USE_IC_LINE_SIZE_CMDOPT
    void SetFlashWidthByte (uint32_t size);
    void SetRasNum (uint32_t num);
    void SetTso (bool is_tso);
#ifdef ENABLE_EXE_ORDER_CTRL
    void SetExecOrderOoOG4MH (bool is_ooo);
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    static const uint32_t INIT_OSTM_RATIO = 1;
    uint32_t m_ostm_clk_ratio;
    void SetClkRatioOstm (uint32_t ratio);
    uint32_t GetClkRatioOstm (void) const { return m_ostm_clk_ratio; }

#ifdef FROG_API_IF
    void DeleteLLBit (int32_t scid, PhysAddr paddr);
    void CreateLLBit (int32_t scid, PhysAddr paddr, ByteSize size);
#endif //FROG_API_IF

    // Give PEID which specified by VMemWriteDebug/VMemReadDebug.
    // This is necessary for peripheral which use accessed PEID in debug API access.
    // When BusWriteDebug/BusReadDebug is called, the PEID is invalid (=CF_INVALID_PEID).
    uint32_t GetPeidOfDbgAPI (void) const { return m_dbg_api_peid; }

    // Get eiint channel offset: default is 0
    uint32_t GetEiintChannelOffset (void) const { return m_eiint_channel_offset; }

    // Set eiint channel offset
    void SetEiintChannelOffset (uint32_t offset);

    // add Yoshinaga for SC-HEAP
    FsBus* GetFsbusPointer(void);
    FILE* SetFnameAndOpen(char* fname) { return(OpenFileOut(std::string(fname))); }
};
