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
#include <algorithm>
#include "./forest_common.h"


class ForestUtil;
class ForestInternals;
class ModuleRun;
class ProcElement;
#include "proc_element_vector.h"
class FsCtrl;
class Cforest;
class MemLatencyInfoBase;
class SideBand;
class TRegModuleVc;
class Dbuf;
class TraceCtrl;
class TraceBuf;
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    class FlashCache;
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)


extern ForestInternals* p_internals;

class ForestInternals {
private:
    Cforest* m_forest;
    ForestUtil* m_forest_util;
    // typedef std::vector<ProcElement*> PeVec;
    typedef std::vector<FsCtrl*> HtVec;
    HtVec m_cedar; // 変更されないので、vectorのコピーを持つ
    typedef std::vector<ModuleRun*> RunList;
    RunList* m_cedar_run; // vector実体はForestUtilのm_cedar_run。更新を共有。

    MessageLevel m_message_level;

    // signal 設定値の一時退避用
    bool m_is_all_in_halt;
    bool m_srec_warn_flag;
    int m_exit_status;

    MemLatencyInfoBase* m_not_mem_latency;

    BreakEvent m_break_event;
    PhysAddr   m_break_pc;
    PhysAddr   m_break_acc_addr;
    ByteSize   m_break_acc_size;
    BreakInfo  m_break_info;

    FILE* m_outfp;

    void bubble_sort(RunList::iterator begin, RunList::iterator end);

public:
    MemLatencyInfoBase* CreateDefaultLatency (MemLocationType pe_location,
                                              MemLocationType cl_location);
    void SetupNotMemLatency (void);

    BreakInfo* GetBreakInfo (void) { return &m_break_info; }
    BreakEvent GetBreakEvent (void) const { return m_break_event; }
    PhysAddr GetBreakPc (void) const { return m_break_pc; }
    PhysAddr GetAccBreakAddr (void) const { return m_break_acc_addr; }
    void ClearBreakEvent (void) { m_break_event = BREAK_NONE; m_break_guard_err = GD_NONE_ERR; }

    void SetMessageLevel (MessageLevel msglvl) { m_message_level = msglvl; }
    MessageLevel GetMessageLevel (void) const { return m_message_level; }

    void SetFileOut (FILE* fp) { m_outfp = fp; }
    FILE* GetFileOut (void) const { return m_outfp; }
    FILE* OpenFile (std::string fname) const;

    int GetExitStatus (void) const { return m_exit_status; }
    void SetExitStatus (int exit_code) { m_exit_status = exit_code; }

    static const uint64_t INIT_BREAK_SYSTIME = 0xFFFFFFFFFFFFFFFFULL;
    uint64_t m_break_systime;
    void SetBreakSystime (uint64_t max_count) { m_break_systime = max_count; }
    uint64_t GetBreakSystime (void) const { return m_break_systime; }

    uint64_t GetSystemTime (void) const;
    void UpdateAllHaltState (void);
    bool IsAllInHalt (void) const { return m_is_all_in_halt; }
    bool IsHitBreak (void) const;
    void AddRunTarget (ModuleRun* mod);
    void RemoveRunTarget (ModuleRun* mod);
    void UpdateHeadHtOrder(ModuleRun* mod);
    void UpdateHtOrder(ModuleRun* mod);
#ifdef SUPPORT_SC_HEAP
    void Consume1Cycle (void);
    bool IsExecutable (void);
    bool IsPendableHtExec(void);
    void SetBreakByTimeout (void);
    void SetBreakTraceFull (void);
    bool IsBreakTraceFull (void) const { return ((m_break_event & BREAK_TRACE_FULL) == BREAK_TRACE_FULL); }

    static const uint32_t INIT_TRACE_SIZE = 10;
    TraceBuf* m_trace_buf;

    bool IsFullTraceBuf (void) const;
    CfApiRetern SetMaxTraceSize (uint32_t num);
    const TraceCtrl* DequeueTrace (void);
    TraceCtrl* ReserveTrace (void);
    void QueueTrace (TraceCtrl* trace);
    uint32_t GetUsedTraceSize (void) const;
    void InitTrace (void);

    void UpdateBreakInfo (int32_t scid);
    FsCtrl* GetHtFromSideband (SideBand sideband);

    void NotifyMatchPc (PhysAddr match_pc);
    void NotifyBreakEcm (GuardErrType err, PhysAddr break_pc);
#endif // SUPPORT_SC_HEAP
    GuardErrType m_break_guard_err;

    bool m_is_disp_systime;
    void SetDispSystime (bool is_disp) { m_is_disp_systime = is_disp; }
    bool IsDispSystime (void) const { return m_is_disp_systime; }

    void LoadSrecBody (PhysAddr addr, uint8_t data);
    void CloseIssTraceFile (void);
    FILE* GetFilePointer (uint32_t peid) const;
    void InitTime (void);
#ifdef CLUSTER
    MemLatencyInfoBase* GetMemResourceInfo (uint32_t peid, PhysAddr addr, MemAccessCommand cmd);
#endif // CLUSTER
    MemLatencyInfoBase* GetMemResourceInfo (uint32_t peid, PhysAddr addr);
    bool IsFlashRomExisted (PhysAddr addr) const;
    bool IsGramExisted (PhysAddr addr) const;
    void NotifyHitBreak (BreakEvent event, PhysAddr break_pc);
    void NotifyHitBreakAcc (BreakEvent event, PhysAddr break_pc,
                            PhysAddr break_acc_addr, ByteSize break_acc_size);
    void CauseError (SideBand sideband, ErrorCode err_code);

    //! @brief Guard function
    //! @details Method set/get for pointer of error capture in Guard function.
    void SetGuardCapErr(GuardModuleID guard_id, uint32_t location_id, TRegModuleVc* guard_cap_err);
    TRegModuleVc* GetGuardCapErr(GuardModuleID guard_id, uint32_t location_id);
private:
    void DeleteLinkBodyByExternal (PhysAddr addr, ByteSize size);
public:
    void DeleteLinkByExternal (PhysAddr addr, ByteSize size);

    void SetForestPointer (Cforest* forest) { m_forest = forest; }
    void SetForestUtilPointer (ForestUtil* forest_util) { m_forest_util = forest_util; }
    void SetCedarRunPointer (RunList* cedar_run) { m_cedar_run = cedar_run; }

    void WakeupSnooze (uint32_t peid);
    void UpdateNextInstAddr (uint32_t peid, ContextId tcid);
    void UpdateRunList (void);

    CacheWayType GetIcacheWayType (uint32_t peid) const;
    void SetIcacheWayType (uint32_t peid, CacheWayType way);

    CacheWayType GetDcacheWayType (uint32_t peid) const;
    void SetDcacheWayType (uint32_t peid, CacheWayType way);

    void InitialSetup(HtVec* cedar);
    bool IsValidPeId (uint32_t peid) const;
    bool IsValidBmId (uint32_t bmid) const;

    void SetTimeout (uint64_t cycle) { m_timeout_cycle = cycle; }
    uint64_t GetTimeout (void) const { return m_timeout_cycle; }
    uint64_t m_timeout_cycle;

#ifdef ENABLE_PEID_POS_CTRL
    bool m_is_peid_g4x;
    void SetPeidPosG4x (bool is_g4x) { m_is_peid_g4x = is_g4x; }
    bool IsPeidPosG4x (void) const { return m_is_peid_g4x; }
#endif // #ifdef ENABLE_PEID_POS_CTRL

    DebugMode GetPeDebugMode (uint32_t peid) const;

    Dbuf* GetDbufPtr (uint32_t peid);
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    FlashCache* GetFlcPtr (uint32_t peid);
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)

    static const uint32_t INIT_DC_FILL_LATENCY = 0; // 128bit bus width
    uint32_t m_dc_fill_latency;
    void SetDcacheFillLatency (uint32_t latency) { m_dc_fill_latency = latency; }
    uint32_t GetDcacheFillLatency (void) const { return m_dc_fill_latency; }

    static const uint32_t INIT_DC_HIT_LATENCY = 1;
    uint32_t m_dc_hit_latency;
    void SetDcacheHitLatency (uint32_t latency) { m_dc_hit_latency = latency; }
    uint32_t GetDcacheHitLatency (void) const { return m_dc_hit_latency; }

    static const uint32_t INIT_ADD_FETCH_STAGE = 0;
    uint32_t m_additonal_fetch_stage;
    void SetAdditonalFetchStage (uint32_t num) { m_additonal_fetch_stage = num; }
    uint32_t GetAdditonalFetchStage (void) const { return m_additonal_fetch_stage; }

    static const uint32_t INIT_DBUF_DEPTH = 8; // DBUF段数変更
    uint32_t m_dbuf_depth;
    void SetDbufDepth (uint32_t depth) { m_dbuf_depth = depth; }
    uint32_t GetDbufDepth (void) const { return m_dbuf_depth; }

    static const uint32_t INIT_DBUF_ADDR_SHIFT = 4; // DBUFラインサイズ変更
    uint32_t m_dbuf_line_shift;
    void SetDbufLineShift (uint32_t shift) { m_dbuf_line_shift = shift; }
    uint32_t GetDbufLineShift (void) const { return m_dbuf_line_shift; }

    static const uint32_t INIT_DBUF_FILL_BURST_SIZE = 1; // FILL回数変更
    uint32_t m_dbuf_fill_num;
    void SetDbufFillBurstNum (uint32_t num) { m_dbuf_fill_num = num; }
    uint32_t GetDbufFillBurstNum (void) const { return m_dbuf_fill_num; }

    static const DbufPreloadType INIT_DBUF_PRELOAD_TYPE = DBUF_PRLD_NO;
    DbufPreloadType m_dbuf_preload_type;
    void SetDbufPreloadType (DbufPreloadType type) { m_dbuf_preload_type = type; }
    DbufPreloadType GetDbufPreloadType (void) const { return m_dbuf_preload_type; }

#ifdef USE_IC_LINE_SIZE_CMDOPT
    static const uint32_t INIT_IC_LINE_BYTES = 32;  // キャッシュラインサイズ
    uint32_t m_ic_line_byte;
    void SetIcLineByte (uint32_t size) { m_ic_line_byte = size; }
    uint32_t GetIcLineByte (void) const { return m_ic_line_byte; }
#endif //#ifdef USE_IC_LINE_SIZE_CMDOPT

    static const uint32_t INIT_FLASH_WIDTH = 32;
    uint32_t m_flash_witdh;
    void SetFlashWidthByte (uint32_t size) { m_flash_witdh = size; }
    uint32_t GetFlashWidthByte (void) const { return m_flash_witdh; }

    static const uint32_t INIT_RAS_NUM = 1;
    uint32_t m_ras_num;
    void SetRasNum (uint32_t num) { m_ras_num = num; }
    uint32_t GetRasNum (void) const { return m_ras_num; }

    static const bool INIT_TSO_MODE = true;
    bool m_is_tso;
    void SetTso (bool is_tso) { m_is_tso = is_tso; }
    bool IsTso (void) const { return m_is_tso; }

#ifdef ENABLE_EXE_ORDER_CTRL
    static const bool INIT_OOO_MODE = true;
    bool m_is_ooo_g4mh;
    void SetExecOrderOoOG4MH (bool is_ooo) { m_is_ooo_g4mh = is_ooo; }
    bool IsExecOrderOoOG4MH (void) const { return m_is_ooo_g4mh; }
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    uint32_t GetDbufHitLatency (void);

    std::vector<uint32_t> m_peid_vec;
    uint32_t GetPeidFromBmid (uint32_t bmid) const { return m_peid_vec [bmid]; }
    uint32_t GetBmidFromPeid (uint32_t peid) const
    { return static_cast <uint32_t> (std::distance(m_peid_vec.begin (), std::find(m_peid_vec.begin (), m_peid_vec.end(), peid))); }

    std::vector<uint32_t> m_clid_vec;
    void RegClIdFromPeId (uint32_t clid, uint32_t peid);
    uint32_t GetClIdFromPeId (uint32_t peid) const { return m_clid_vec [peid]; }

    void UpdatePeidArray (void);

    uint64_t GetStepCount (uint32_t peid) const;
    uint64_t GetCycleCount (uint32_t peid) const; //without halt
    uint64_t GetCpuTime (uint32_t peid) const; // with halt
    uint64_t GetBranchCount (uint32_t peid) const;
    uint64_t GetBranchCondExecCount (uint32_t peid) const;
    uint64_t GetBranchCondMissCount (uint32_t peid) const;
    uint64_t GetEiintCount (uint32_t peid, uint32_t ch) const;
    uint64_t GetFeintCount (uint32_t peid, uint32_t ch) const;
    uint64_t GetExpAbortCount (uint32_t peid, uint32_t ch) const;
    uint64_t GetExpNonAbortCount (uint32_t peid, uint32_t ch) const;
    uint64_t GetBgintAbortCount (uint32_t peid, uint32_t ch) const;
    uint64_t GetBgintAccesptedCount (uint32_t peid, uint32_t ch) const;
    uint64_t GetTotalFetchCount (uint32_t peid) const;
    uint64_t GetRomFetchCount (uint32_t peid) const;
    uint64_t GetRomDataAccCount (uint32_t peid) const;
    uint64_t GetIcacheHitCount (uint32_t peid) const;
    uint64_t GetInstIssueStall (uint32_t peid) const;
    uint64_t GetInstCountPmc (uint32_t peid) const;

    void SetCpuGuestModeToIntc2 (uint32_t peid, bool is_gm);
    void SetCpuGpidToIntc2 (uint32_t peid, uint32_t gpid);

    explicit ForestInternals (ForestUtil* forest_util);
    ~ForestInternals ();
};
