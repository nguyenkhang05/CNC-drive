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

#include <string>
#include <vector>
#include <map>
#include <utility>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./module_run.h"
#include "./module_guard.h"

class MemBody;
class MemLatencyInfoBase;

class FsBus_IF
{
public:
    virtual void AddCyclicHandler (CModuleVc* tgt_vc)= 0;
    virtual void RemoveCyclicHandler (CModuleVc* tgt_vc)= 0;
    virtual ~FsBus_IF(){}

};

class DLL_EXPORT_IMPORT FsBus : public ModuleRun, public FsBus_IF {
public:
    bool is_registered_smem;
    void RegisterSmem (void) { is_registered_smem = true; }

    /* バスターゲット構造体 */
    class BusTarget {
    public:
        bool         m_is_bridge;        // Target is cluster-bridge
        std::string  m_bus_name;         // Bus-Target name
        uint32_t     m_btgt_peid;        // Location of Bus-Target.
        uint32_t     m_btgt_clid;        // Location of Bus-Target.
        uint32_t     m_btgt_mem_peid;    // PEID of target Memory Body, only enabled when m_is_bridge is true.
        PhysAddr     m_btgt_start_addr;  // メモリマップの先頭アドレス
        PhysAddr     m_btgt_end_addr;    // メモリマップの終了アドレス
        CModuleVc*   m_btgt_vc;          // VCのIFクラスへのポインタ
        MemBody*     m_membody;          // VC保有のメモリ実体へのポインタ
        ModuleGuard* m_btgt_guard;       // ガードモジュールへのポインタ
        MemLatencyInfoBase* m_latency;   // アクセスレイテンシ情報
#ifdef CLUSTER
        MemRestrictType m_restrict_type; // It is used for cluster bridge access restrict
#endif // CLUSTER
    };


    typedef std::vector<BusTarget*> BusTargetVec;
    BusTargetVec m_bus_targets;
    // Vector list of Cluster-Bounded Target:
    // All of items should be typed as MemLatencyInfoExCluster
    BusTargetVec m_cluster_targets;

    BusTarget* m_vc_dummy;


    typedef std::vector<CModuleVc*> TgtVcVec;
    TgtVcVec m_run_targets;
    enum AddOrErase {
        ADD_TARGET = true,
        REM_TARGET = false
    };
    typedef std::pair<AddOrErase, CModuleVc*> TgtUpdate;
    std::vector<TgtUpdate> m_update_targets;

    class TgtBlock {
    public:
        PhysAddr   m_start_addr;       // メモリマップの先頭アドレス
        PhysAddr   m_end_addr;         // メモリマップの終了アドレス
        PhysOffset m_offset;
    };
    typedef std::vector<TgtBlock*> TgtBlockVec;
    TgtBlockVec m_self_blocks;
    TgtBlockVec m_mirr_blocks;

    TgtBlock* SearchSelfBlock (PhysAddr phys_addr) const;
    TgtBlock* SearchMirrorBlock (PhysAddr phys_addr) const;

    class TgtBlockDenied {
    public:
        PhysAddr   m_start_addr;       // メモリマップの先頭アドレス
        PhysAddr   m_end_addr;         // メモリマップの終了アドレス
    };
    typedef std::vector<TgtBlockDenied*> TgtBlockDeniedVec;
    TgtBlockDeniedVec m_deny_blocks;
    bool IsAccessDenied (PhysAddr addr) const;
    TgtBlockDenied* SearchAccessDeniedBlock (PhysAddr phys_addr) const;
    PhysAddr GetBusAddress (SideBand sideband, PhysAddr addr) const;
    PhysAddr TranslateSelfAddr (SideBand sideband, PhysAddr addr) const;
    PhysAddr TranslateMirrorAddr (SideBand sideband, PhysAddr addr) const;

    void UpdateExoPeLatency (uint32_t exo_latency_offset);

    FsBus ();
    ~FsBus ();

    bool IsLatencyInfoUsedOther (MemLatencyInfoBase* mem_latency);
    bool IsDeletableLatencyInfo (MemLatencyInfoBase* mem_latency);
    void ResetLatencyInfo (void);

    void AddCyclicHandler (CModuleVc* tgt_vc);
    void RemoveCyclicHandler (CModuleVc* tgt_vc);
    bool RemTargetVC (PhysAddr start_addr, CModuleVc* tgt_vc, MemBody* membody);
    bool AddTargetVC (MemLocationType pe_location, MemLocationType cl_location,
                      PhysAddr start_addr, PhysAddr end_addr, CModuleVc* tgt_vc,
                      MemBody* membody, ModuleGuard* guard, MemLatencyInfoBase* latency);
    bool AddTargetVC (PhysAddr start_addr, PhysAddr end_addr, CModuleVc* tgt_vc,
                      MemBody* membody, ModuleGuard* guard, MemLatencyInfoBase* latency);
#ifdef CLUSTER
    bool AddClusterTarget (char *bridge_name,
                           uint32_t clid,
                           PhysAddr start_addr,
                           PhysAddr end_addr,
                           MemRestrictType restrict_type,
                           ModuleGuard* guard,
                           MemLatencyInfoBase *latency);
#endif // CLUSTER

    bool SetSelfArea (PhysAddr start_addr, PhysAddr end_addr, PhysOffset offset);
    bool ClearSelfArea (PhysAddr addr);
    bool SetMirrorArea (PhysAddr start_addr, PhysAddr end_addr, PhysOffset offset);
    bool ClearMirrorArea (PhysAddr addr);
    bool SetAccessDeniedArea (PhysAddr start_addr, PhysAddr end_addr);
    bool ClearAccessDeniedArea (PhysAddr addr);

    BusTarget* SearchAccessTargetPhys (PhysAddr phys_addr);
    BusTarget* SearchAccessTarget (PhysAddr phys_addr);
#ifdef CLUSTER
    BusTarget* SearchAccessTarget (PhysAddr phys_addr, uint32_t peid, MemAccessCommand cmd);
    bool IsValidAccess (MemAccessCommand cmd, MemRestrictType restrict);
#endif // CLUSTER
    bool IsTargetValid (BusTarget* tgt) const { return tgt != m_vc_dummy; }

#ifdef CLUSTER
    void ShowAccessRoute (SideBand sideband, MemAccessCommand cmd, PhysAddr addr, BusTarget* tgt);
#endif // CLUSTER

    template <MemAccessMode acc_mode, MemAccessCommand cmd>
    SysErrCause BusAccessInternal (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size,
                            BusTarget* tgt);

    template <MemAccessMode acc_mode, MemAccessCommand cmd>
    SysErrCause BusAccessPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    template <MemAccessMode acc_mode, MemAccessCommand cmd>
    SysErrCause BusAccess (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    SysErrCause BusWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccess<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    }
    SysErrCause BusRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccess<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    SysErrCause BusFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccess<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    }
    SysErrCause BusWritePhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccessPhys<MACC_NORMAL, DATA_WRITE> (sideband, addr, data, size);
    }
    SysErrCause BusReadPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccessPhys<MACC_NORMAL, DATA_READ> (sideband, addr, data, size);
    }
    SysErrCause BusFetchPhys (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccessPhys<MACC_NORMAL, INST_READ> (sideband, addr, data, size);
    }
    SysErrCause BusWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccess<MACC_DEBUG, DATA_WRITE> (SideBand::DebugAccess(), addr, data, size);
    }
    SysErrCause BusReadDebug (PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccess<MACC_DEBUG, DATA_READ> (SideBand::DebugAccess(), addr, data, size);
    }
    SysErrCause BusWritePhysDebug (PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccessPhys<MACC_DEBUG, DATA_WRITE> (SideBand::DebugAccess(), addr, data, size);
    }
    SysErrCause BusReadPhysDebug (PhysAddr addr, uint8_t* data, ByteSize size) {
        return BusAccessPhys<MACC_DEBUG, DATA_READ> (SideBand::DebugAccess(), addr, data, size);
    }

    FILE* m_filep;
    uint64_t m_clk_cycle;
    uint32_t m_clk_ratio;
    static const int32_t FSBUS_SCID = 0x7FFF0000;

    void SetCpuTime (uint64_t wait_cycle);
    void SetCpuRatio (uint32_t ratio);

    IsElapsedTime RunModule (void);
    void ClockWait (uint64_t wait_cycle);
    void PreConsumeClock (void);
    void AddHaltCycle (uint64_t halt_cycle);
    void UpdateSnoozeState (void);
    bool IsBreakHitPreExec (void) const;
    bool IsBreakHitPostExec (void) const;
    bool IsHtRunable (void) const;
    bool IsSnooze (void) const;
    bool IsHtActive (void) const;
    bool IsIntRequested (void) const;
    bool IsInHalt (void) const;
#ifdef SUPPORT_SC_HEAP
    bool IsSuspendState (void) const;
#endif // SUPPORT_SC_HEAP
    bool IsCpu (void) const;
    FILE* GetFilePointer (void) const;
    int32_t GetScId (void) const;
    uint64_t GetCpuTime (void) const;
    uint64_t GetBusTime (void) const { return GetCpuTime (); }
    uint32_t GetCpuRatio (void) const;
    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

#ifdef CLUSTER
    bool m_is_bus_trace;
    void SetBusTrace (bool is_bus_trace) { m_is_bus_trace = is_bus_trace; }
#endif // CLUSTER
};


// エラー処理用ダミーモジュール
class DLL_EXPORT_IMPORT VcDummy : public CModuleVc
{
public:
    void CyclicHandler (void) {}
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void) {}
    VcDummy () {}
    virtual ~VcDummy() {}
};
