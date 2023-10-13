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
#include <utility>
#include "./forest_common.h"
#include "./inst_argtype.h"
#include "./trace_info.h"

class TraceCtrl;
class CsCtrl;
class FsCtrl;
class InstV850;
class GRegFile;

class SuspendCtrl {
private:
    FsCtrl* m_fsctrl;
    CsCtrl* m_csctrl;
    InstV850* m_insts;
    GRegFile* m_grf;

    uint32_t m_pe_id;
    SuspendState m_suspend_state;
    bool m_is_runnable;

public:
    static const int32_t DEP_ALL_REG = 0;
    // 発行レートにのみ影響。レジスタ登録ないのでいくつでも良い。
    static const int32_t DEP_ISSUE   = 0;

    typedef bool DependIssueType;
    static const DependIssueType HAS_DEP_ISSUE = true;
    static const DependIssueType NO_DEP_ISSUE = false;

    SuspendCtrl (FsCtrl* fsctrl, CsCtrl* csctrl,
                 InstV850* insts, GRegFile* grf, uint32_t pe_id);
    ~SuspendCtrl ();

    void SetIsRunnable (bool is_runnable) { m_is_runnable = is_runnable; }
    bool IsRunnable (void) const { return m_is_runnable; }
    bool HasSelfDepend (void) const { return m_has_self_dep; }
    uint32_t GetRmwLoadLatency (void) const { return m_rmw_read_latency; }

    void Init (void);
    void WaitForRegDepend (TraceCtrl* trace);
    void WaitForRegDependSelf (TraceCtrl* trace);
    void ResumeExec (TraceCtrl* trace);

    void RegisterStoreDependency (TraceCtrl* trace, int32_t pos, uint64_t start_cycle);
    void RegisterLoadDependency (TraceCtrl* trace, int32_t pos,
                                 CReg dest_reg, DependIssueType has_dep_issue,
                                 uint64_t start_cycle);
    bool IsReadLatencyReceivedCs (TraceCtrl* trace) const;

    void NotifyFetchLatency (TraceCtrl* trace, PhysAddr addr, uint32_t latency);
    void NotifyReadLatency  (TraceCtrl* trace, PhysAddr addr, uint32_t latency, uint64_t data);
    void NotifyWriteLatency (TraceCtrl* trace, PhysAddr addr, uint32_t latency);
    void ApplyNotifyOwn (TraceCtrl* trace);
    void InitNotifyOwn (void);

    void SetSuspendState (SuspendState state);
    SuspendState GetSuspendState (void) const { return m_suspend_state; }

    bool m_is_lsu_repeated;
    void SetLsuRepeated (bool is_repeated) { m_is_lsu_repeated = is_repeated; }
    bool GetLsuRepeated (void) const { return m_is_lsu_repeated; }
    uint32_t m_repeat_count;
    void SetLsuRepeatCount (uint32_t repeat_count) { m_repeat_count = repeat_count; }
    uint32_t GetLsuRepeatCount (void) const { return m_repeat_count; }
    void UpdateSuspendState (TraceCtrl* trace);

private:
    static const CReg BIT0_MASK = ~0x1; // 偶数レジスタ固定にするためのBIT0マスク

    CRegVec m_dep_regs;

    void CalcDepRegs (TraceCtrl* trace);
    void AddDependReg (CReg reg);
    void AddDependRegsList12 (TraceCtrl* trace);

    bool m_has_self_dep;
    uint32_t m_rmw_read_buf;
    uint32_t m_rmw_read_latency;

    enum SyncType {
        SYNC_NONE,
        SYNC_READ,
        SYNC_READ_WRITE
    };

    SyncType m_sync_type;

    typedef uint64_t TransactionId;
    typedef std::pair<PhysAddr, TransactionId> TrIdInfo;
    typedef std::vector<TrIdInfo> TrIdInfoVec;
    TrIdInfoVec m_trid_infos;

    typedef std::pair<TransactionId, CReg> ReadRegInfo;
    typedef std::vector<ReadRegInfo> ReadRegInfoVec;
    ReadRegInfoVec m_rreg_infos;

    typedef std::pair<TransactionId, LoadType> LoadTypeInfo;
    typedef std::vector<LoadTypeInfo> LoadTypeInfoVec;
    LoadTypeInfoVec m_loadtype_infos;

    uint64_t m_transaction_id;
    TransactionId RegisterTransaction (PhysAddr addr, LoadType ld_type);
    void RegisterLoadTarget (TransactionId trid, CReg reg);

    TrIdInfoVec m_write_trid_infos;
    TransactionId RegisterWriteTransaction (PhysAddr addr);
    bool SetStoreResult (PhysAddr addr, uint32_t latency);

    typedef std::pair<TransactionId, uint64_t> MemIssueCycleInfo;
    typedef std::vector<MemIssueCycleInfo> MemIssueCycleVec;
    MemIssueCycleVec m_mem_issue_cycle_infos;
    void RegisterMemIssueCycle (TransactionId trid, uint64_t start_cycle);

    bool IsInLoadTarget (CReg reg) const;
    bool SetLoadResult (PhysAddr addr, uint32_t latency, uint64_t data);
    bool IsReadLatencyReceivedRMW (void) const;
    bool IsReadLatencyReceivedFs (void) const;
    void ApplyLatencyToLsuBuf (TransactionId trid, uint32_t latency);

    void ApplyLoadResult (LoadType load_type, CReg reg, uint64_t data);
    void ApplyLoadResult_Ignore (CReg reg, uint64_t data);
    void ApplyLoadResult_S8 (CReg reg, uint64_t data);
    void ApplyLoadResult_S16 (CReg reg, uint64_t data);
    void ApplyLoadResult_U8 (CReg reg, uint64_t data);
    void ApplyLoadResult_U16 (CReg reg, uint64_t data);
    void ApplyLoadResult_U32 (CReg reg, uint64_t data);
    void ApplyLoadResult_U64 (CReg reg, uint64_t data);
    void UpdateGReg (CReg reg, uint32_t old_data, uint32_t new_data);

    bool m_has_notify_own;

    struct NotifyReadType {
        PhysAddr m_addr;
        uint32_t m_latency;
        uint64_t m_data;
        NotifyReadType (PhysAddr addr, uint32_t latency, uint64_t data)
        : m_addr (addr), m_latency (latency), m_data (data) {}
    };
    typedef std::vector<NotifyReadType> NotifyReadVec;
    NotifyReadVec m_notify_read_info;

    struct NotifyWriteType {
        PhysAddr m_addr;
        uint32_t m_latency;
        NotifyWriteType (PhysAddr addr, uint32_t latency)
        : m_addr (addr), m_latency (latency) {}
    };
    typedef std::vector<NotifyWriteType> NotifyWriteVec;
    NotifyWriteVec m_notify_write_info;

    void RegisterReadLatencyOwn (PhysAddr addr, uint32_t latency, uint64_t data);
    void RegisterWriteLatencyOwn (PhysAddr addr, uint32_t latency);

    // CF_ISSUE/CF_RMW_BUFの除外用
    bool IsRegDepAnaNecessity (CReg regno) const { return regno < DEP_REG_MAX; }

    inline uint32_t OperandExtract (uint64_t x, uint32_t l, uint32_t r) const {
        return static_cast<uint32_t> ((x >> r) & ~(~0x0UL << (l - r + 1)));
    }

    inline uint32_t SignExtend (uint32_t val, uint32_t width) const {
        uint32_t msb = 0x01U << (width - 1);
        return ((val ^ msb) - msb);
    }
};
