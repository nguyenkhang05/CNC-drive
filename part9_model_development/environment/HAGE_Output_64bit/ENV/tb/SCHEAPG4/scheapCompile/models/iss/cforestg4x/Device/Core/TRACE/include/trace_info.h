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
#include "./config.h"
#include "./sreg_enum.h"
#include "./trace_info_if.h"

typedef uint32_t GuardErrType;

// パイプラインステージ列挙型
enum PipeStage {
    STAGE_IA  = 0,  // I
    STAGE_IT  = 1,  // P
    STAGE_DP  = 2,  // D
    STAGE_EA  = 3,  // E
    STAGE_MEM = 4,  // M
    STAGE_AL  = 5,  // A
    STAGE_WB  = 6,  // W
#ifdef PRINT_FXU_PIPE
    STAGE_VD  = 7,  // V
    STAGE_VDSKID  = 8,  // S
    STAGE_NUM = 9,  // Guard
#else // #ifdef PRINT_FXU_PIPE
    STAGE_NUM = 7
#endif // #ifdef PRINT_FXU_PIPE
};


// 分岐予測イベント列挙型
enum BpResult {
    BP_NONE = 0, //分岐予測未判定
    BP_HIT  = 1, //分岐予測ヒット
    BP_MISS = 2  //分岐予測ミス
};


enum BranchInfo {
    NO_BRANCH = 0,  // Normal execution
    BRANCH = 1,     // Branch is happened. Interruption/Exception isn't happened.
    INT_BRANCH = 2, // Interruption is happened.
    EXP_BRANCH = 3, // Exception is happened
    RTE_BRANCH = 4  // Interrupt return
};


enum IcEventType {
    IC_EVT_MISS   = 0,  // Instruction cache miss
    IC_EVT_HIT    = 1,  // Instruction cache hit
    IC_EVT_INV    = 2,  // Instruction cache invalidate
    IC_EVT_FILL   = 3,  // Instruction cache fill
    IC_EVT_LOCK   = 4,  // Instruction cache lock
    IC_EVT_UNLOCK = 5,  // Instruction cache unlock
    IC_EVT_ST     = 6,  // Instruction cache store
    IC_EVT_LD     = 7,  // Instruction cache load
    IC_EVT_NOP    = 8,  // Instruction cache prefi but ICHIT
    IC_EVT_NUM    = 9
};


// @@ T.B.D
enum MemAccessRoute {
    NONE_CACHE_ROUTE  = 0,
    ICACHE_MISS_ROUTE = 1,
    ICACHE_HIT_ROUTE  = 2,
    AXC_HIT_ROUTE     = 4,
    DCACHE_MISS_ROUTE = 5,
    DCACHE_HIT_ROUTE  = 6
};

#ifdef SUPPORT_SC_HEAP
enum SuspendState {
    NO_SUSPEND           = 0,
    SUSPEND_FS_EXEC      = 1,
    RELEASED_FS_EXEC     = 2,
    SUSPEND_FS_EXEC_SELF = 3,
    RELEASED_FS          = 4,
    SUSPEND_CS_FETCH     = 5,
    RELEASED_CS_FETCH    = 6,
    SUSPEND_CS_EXEC      = 7,
    RELEASED_CS_EXEC     = 8,
    SUSPEND_CS_LSU       = 9,
    RELEASED_CS_LSU      = 10,
    RELEASED_CS          = 11
};
#endif // SUPPORT_SC_HEAP


typedef std::vector<CReg> CRegVec;


// トレース情報構造体
class TraceInfo {
public:
    uint32_t  pe_id;                       // Processor Element ID

    PhysAddr  inst_addr;                   // 命令アドレス
    PhysAddr  next_inst_addr;              // 次の命令アドレス
    bool m_is_branch_target;               // G4KH only

    union {
        uint64_t inst_code;      //   instruction code
        struct {
            uint32_t inst_code_l32;  // lower 32bit of instruction code
            uint32_t inst_code_u32;  // upper 32bit of instruction code
        };
    };
    ByteSize  inst_size;                   // byte of instruction code
    int32_t   inst_id;                     // 命令ID
    bool      is_branched;                 // 分岐成功フラグ  // 初期化必須
    BranchInfo branch_info;                // branch category // 初期化必須
    BpResult  bp_result;                   // 分岐予測結果  // 初期化必須
    bool      is_para_grp_switched;        // 並列発行グループ切替フラグ // 初期化必須
    bool      is_parallel;                 // 並列実行フラグ
    PrivLevel priv_level;                  // 特権モードレベル
    ExpCode   exp_code;                    // 例外情報
    ExpCause  exp_cause;                   // 例外要因コード
    bool      is_cancel;                   // ガードIPによるSYSERRなどで実行キャンセル

    int32_t   rreg_count;                  // レジスタリード回数  // 初期化必須
    CReg      read_reg[REC_RREG_NUM];      // 読み込みレジスタ番号
    RegData   rreg_value[REC_RREG_NUM];    // Read value from GR/sysreg
    RegData   rreg_wr_value[REC_RREG_NUM][WREG_WAY_NUM]; // Read value from WR
    int32_t   wreg_count;                  // レジスタライト回数  // 初期化必須
    CReg      write_reg[REC_WREG_NUM];     // 書き込みレジスタ番号
    RegData   wreg_value[REC_WREG_NUM];    // Write value to GR/sysreg
    RegData   wreg_wr_value[REC_WREG_NUM][WREG_WAY_NUM];  // Write value to WR

    int32_t   fetch_count;
    PhysAddr  fetch_addr[REC_FETCH_NUM];     // 命令フェッチアドレス

    int32_t   ic_count;
    uint32_t  ic_entry[REC_IC_NUM];          // ICアクセス対象エントリ
    uint32_t  ic_way[REC_IC_NUM];            // ICアクセス対象ウェイ
    IcEventType ic_event[REC_IC_NUM];        // ICアクセス種類
    PhysAddr  ic_addr[REC_RMEM_NUM];         // ICアクセスアドレス (for icfill by prefi/cfali)

    int32_t   rmem_count;                    // メモリリード回数  // 初期化必須
    PhysAddr  rmem_addr[REC_RMEM_NUM];       // メモリリードアドレス
    MemData64 rmem_value_l64[REC_RMEM_NUM];  // メモリから読み込む値
    MemData64 rmem_value_u64[REC_RMEM_NUM];  // メモリから読み込む値
    ByteSize  rmem_size[REC_RMEM_NUM];       // メモリリードアクセスサイズ
    MemMappedArea rmem_target[REC_RMEM_NUM]; // メモリリード先のメモリタイプ

    int32_t   wmem_count;                    // メモリライト回数  // 初期化必須
    PhysAddr  wmem_addr[REC_WMEM_NUM];       // メモリライトアドレス
    MemData64 wmem_value_l64[REC_WMEM_NUM];  // メモリに書き込む値
    MemData64 wmem_value_u64[REC_WMEM_NUM];  // メモリに書き込む値
    ByteSize  wmem_size[REC_WMEM_NUM];       // メモリライトアクセスサイズ
    MemMappedArea wmem_target[REC_WMEM_NUM]; // メモリライト先のメモリタイプ

#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    int32_t   rmem_mdp_count;                    // メモリリード回数  // 初期化必須
    PhysAddr  rmem_mdp_addr[REC_RMEM_NUM];       // メモリリードアドレス
    MemData64 rmem_mdp_value_l64[REC_RMEM_NUM];  // メモリから読み込む値
    MemData64 rmem_mdp_value_u64[REC_RMEM_NUM];  // メモリから読み込む値
    ByteSize  rmem_mdp_size[REC_RMEM_NUM];       // メモリリードアクセスサイズ
    MemMappedArea rmem_mdp_target[REC_RMEM_NUM]; // メモリリード先のメモリタイプ
    MemAccessRoute rmem_mdp_route[REC_RMEM_NUM]; // access route of memory read
    bool      rmem_mdp_issued[REC_RMEM_NUM];     // 1 : access normally. 0 : MDP is suppressed

    int32_t   wmem_mdp_count;                    // メモリライト回数  // 初期化必須
    PhysAddr  wmem_mdp_addr[REC_WMEM_NUM];       // メモリライトアドレス
    MemData64 wmem_mdp_value_l64[REC_WMEM_NUM];  // メモリに書き込む値
    MemData64 wmem_mdp_value_u64[REC_WMEM_NUM];  // メモリに書き込む値
    ByteSize  wmem_mdp_size[REC_WMEM_NUM];       // メモリライトアクセスサイズ
    MemMappedArea wmem_mdp_target[REC_WMEM_NUM]; // メモリライト先のメモリタイプ
    MemAccessRoute wmem_mdp_route[REC_WMEM_NUM]; // access route of memory write
    bool      wmem_mdp_issued[REC_WMEM_NUM];     // 1 : access normally. 0 : MDP is suppressed
#endif //#if defined (ENABLE_TRACE_MDP_SUPPRESS)

#ifdef FROG_API_IF
    std::pair <PhysAddr, ByteSize> add_llbit;
    int32_t   del_llbit_count;
    std::pair <int32_t, PhysAddr>  del_llbit_paddr  [REC_LLBIT_NUM];
#endif //FROG_API_IF

    uint64_t  step_count;                    // トータル実行命令数
    uint64_t  stage_cycle[STAGE_NUM];        // 各ステージへの到達サイクル
#ifdef DEBUG_PRINT_STALL
    uint64_t  m_last_issue_cycle;
#endif // #ifdef DEBUG_PRINT_STALL

#ifdef SUPPORT_CUBESUITE
    uint32_t  next_pe_id;                    // PE id for next instruction
#endif //#ifdef SUPPORT_CUBESUITE

    NotifySimState  sim_state;

    MemAccessRoute fetch_route[REC_FETCH_NUM]; // access route of inst fetch
    MemAccessRoute rmem_route[REC_RMEM_NUM]; // access route of memory read
    MemAccessRoute wmem_route[REC_WMEM_NUM]; // access route of memory write
    CRegVec rdep_regs;       // レジスタ依存判定リスト(Read)   // 初期化必須
    CRegVec wdep_regs;       // レジスタ依存判定リスト(Write)  // 初期化必須

#ifdef SUPPORT_SC_HEAP
    // 実行完了時以外にブレークすることを抑止するために必要
    SuspendState suspend_state;
    GuardErrType guard_err;
    bool is_pswh_changed;
#endif // SUPPORT_SC_HEAP

#if defined(COMP_RUNNER)
    bool is_cancel_mcyc;
#endif // #if defined(COMP_RUNNER)
    bool first_fusion_group;
    bool is_fusion;
    bool is_inst_ic_hit;
};


class RegName;


class TraceCtrl : public TraceCtrlIF {
public:
    //! レジスタアクセスコマンド列挙型
    enum RegCommand {
        REG_READ,
        REG_WRITE,
    };

    template <RegCommand cmd> void RecordPC   (PhysAddr addr);
    template <RegCommand cmd> void RecordPsw  (CReg regno, RegData data);
    template <RegCommand cmd> void RecordSesr (CReg regno, bool data);
    template <RegCommand cmd> void RecordGReg (uint32_t regno, RegData data);
    template <RegCommand cmd> void RecordWReg (uint32_t regno, RegData64 data_u, RegData64 data_l);
    template <RegCommand cmd> void RecordSReg (CfSReg cfsrid, RegData data);

#if defined(COMP_RUNNER)
    template <RegCommand cmd> void RecordSReg (CReg sregid, RegData data);
#endif //#if defined(COMP_RUNNER)

    void RecordMemFetch (PhysAddr addr, MemAccessRoute route);
    void RecordIcAccess (uint32_t entry, uint32_t way, IcEventType event, PhysAddr addr);

    template <MemAccessCommand cmd>
    void RecordMemAccess (PhysAddr addr, MemData64 data_u, MemData64 data_l,
                          ByteSize size, MemAccessRoute route);

#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    template <MemAccessCommand cmd>
    void RecordMemAccessMdp (PhysAddr addr, MemData64 data_u, MemData64 data_l,
                          ByteSize size, MemAccessRoute route, bool is_issued);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

#ifdef FROG_API_IF
    void RecordDeleteLLBit (int32_t pe_id, PhysAddr paddr);

    void SetLLBitAdd     (PhysAddr llbit_paddr, ByteSize llbit_size);
#endif //FROG_API_IF
    void SetPeId         (uint32_t pe_id);
    void SetInstAddr     (PhysAddr inst_addr);
    void SetFetchAddr    (PhysAddr fetch_addr, int32_t i);
    void SetInstPhysAddr (PhysAddr inst_paddr);
    void SetNextInstAddr (PhysAddr next_inst_addr);
    void SetInstCode     (uint64_t inst_code);
    void SetInstSize     (ByteSize inst_size);
    void SetInstId       (int32_t inst_id);
    void SetPrivLevel    (PrivLevel priv_level);
    void SetExpCode      (ExpCode exp_code);
    void SetExpCause     (ExpCause exp_cause);
    void SetBranchInfo   (BranchInfo branch_info);
    void SetBpResult     (BpResult bp_result);
    void SetParallel     (bool is_parallel);
    void SetStageCycle   (PipeStage stage, uint64_t cycle);
    void SetStepCount    (uint64_t step);
    void IncStepCount (void);
    void SetParaGroupSwitched (bool is_switched);
#ifdef SUPPORT_CUBESUITE
    void SetNextPeId     (uint32_t next_pe_id);
#endif // SUPPORT_CUBESUITE
    void SetSimState (NotifySimState state);
    void RecordRregDepVec  (uint32_t regno);
    void SetCancel (bool is_cancel);
#if defined(COMP_RUNNER)
    void SetCancelMcyc (bool is_cancel);
#endif // #if defined(COMP_RUNNER)
#ifdef SUPPORT_SC_HEAP
    void SetSuspendState (SuspendState state);
    void SetGuardError   (GuardErrType err);
    void SetPswhChanged  (bool is_changed) { m_tr.is_pswh_changed = is_changed; }
#endif // SUPPORT_SC_HEAP
    void SetInstIcHit (bool is_hit) { m_tr.is_inst_ic_hit = is_hit; }

    uint32_t      GetPeId         (void) const { return m_tr.pe_id; }
#ifdef CLUSTER
    uint32_t      GetClId         (void) const;
#endif // CLUSTER
    PhysAddr      GetInstAddr     (void) const { return m_tr.inst_addr; }
    PhysAddr      GetFetchAddr    (int32_t i) const { return m_tr.fetch_addr[i]; }
    PhysAddr      GetNextInstAddr (void) const { return m_tr.next_inst_addr; }
    uint64_t      GetInstCode     (void) const { return m_tr.inst_code; }
    ByteSize      GetInstSize     (void) const { return m_tr.inst_size; }
    int32_t       GetInstId       (void) const { return m_tr.inst_id; }
    PrivLevel     GetPrivLevel    (void) const { return m_tr.priv_level; }
    ExpCode       GetExpCode      (void) const { return m_tr.exp_code; }
    ExpCause      GetExpCause     (void) const { return m_tr.exp_cause; }
    uint64_t      GetStepCount    (void) const { return m_tr.step_count; }
    uint64_t      GetStageCycle   (PipeStage stage) const { return m_tr.stage_cycle[stage]; }
    bool          IsBranched      (void) const { return m_tr.branch_info != NO_BRANCH; }
    BranchInfo    GetBranchInfo   (void) const { return m_tr.branch_info; }
    BpResult      GetBpResult     (void) const { return m_tr.bp_result; }
    bool          IsParallel      (void) const { return m_tr.is_parallel; }
    int32_t       GetRregCount    (void) const { return m_tr.rreg_count; }
    int32_t       GetWregCount    (void) const { return m_tr.wreg_count; }
    RegData       GetRregValue    (int32_t i) const { return m_tr.rreg_value[i];}
    RegData       GetRWregValue   (int32_t i, uint32_t way) const { return m_tr.rreg_wr_value[i][way];}
    RegData       GetWregValue    (int32_t i) const { return m_tr.wreg_value[i];}
    RegData       GetWWregValue   (int32_t i, uint32_t way) const { return m_tr.wreg_wr_value[i][way];}
    int32_t       GetFetchCount   (void) const { return m_tr.fetch_count; }
    int32_t       GetIcCount      (void) const { return m_tr.ic_count; }
    uint32_t      GetIcEntry      (int32_t i) const { return m_tr.ic_entry[i]; }
    uint32_t      GetIcWay        (int32_t i) const { return m_tr.ic_way[i]; }
    IcEventType   GetIcEvent      (int32_t i) const { return m_tr.ic_event[i]; }
    PhysAddr      GetIcAddr       (int32_t i) const { return m_tr.ic_addr[i]; }

    MemAccessRoute GetFetchRoute  (int32_t i) const { return m_tr.fetch_route[i]; }
    MemAccessRoute GetRmemRoute   (int32_t i) const { return m_tr.rmem_route[i]; }
    MemAccessRoute GetWmemRoute   (int32_t i) const { return m_tr.wmem_route[i]; }
    int32_t       GetRmemCount    (void) const { return m_tr.rmem_count; }
    int32_t       GetWmemCount    (void) const { return m_tr.wmem_count; }
    MemData64     GetRmemValueL64 (int32_t i) const { return m_tr.rmem_value_l64[i];}
    MemData64     GetRmemValueU64 (int32_t i) const { return m_tr.rmem_value_u64[i];}
    MemData64     GetWmemValueL64 (int32_t i) const { return m_tr.wmem_value_l64[i];}
    MemData64     GetWmemValueU64 (int32_t i) const { return m_tr.wmem_value_u64[i];}
    CReg          GetReadReg      (int32_t i) const { return m_tr.read_reg[i]; }
    CReg          GetWriteReg     (int32_t i) const { return m_tr.write_reg[i]; }
    PhysAddr      GetRmemAddr     (int32_t i) const { return m_tr.rmem_addr[i]; }
    PhysAddr      GetWmemAddr     (int32_t i) const { return m_tr.wmem_addr[i]; }
    ByteSize      GetRmemSize     (int32_t i) const { return m_tr.rmem_size[i]; }
    ByteSize      GetWmemSize     (int32_t i) const { return m_tr.wmem_size[i]; }
    MemMappedArea GetRmemTarget   (int32_t i) const { return m_tr.rmem_target[i]; }
    MemMappedArea GetWmemTarget   (int32_t i) const { return m_tr.wmem_target[i]; }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    MemAccessRoute GetRmemMdpRoute   (int32_t i) const;
    MemAccessRoute GetWmemMdpRoute   (int32_t i) const;
    int32_t       GetRmemMdpCount    (void) const;
    int32_t       GetWmemMdpCount    (void) const;
    bool          GetRmemMdpIssued   (int32_t i) const;
    bool          GetWmemMdpIssued   (int32_t i) const;
    MemData64     GetRmemMdpValueL64 (int32_t i) const;
    MemData64     GetRmemMdpValueU64 (int32_t i) const;
    MemData64     GetWmemMdpValueL64 (int32_t i) const;
    MemData64     GetWmemMdpValueU64 (int32_t i) const;
    PhysAddr      GetRmemMdpAddr     (int32_t i) const;
    PhysAddr      GetWmemMdpAddr     (int32_t i) const;
    ByteSize      GetRmemMdpSize     (int32_t i) const;
    ByteSize      GetWmemMdpSize     (int32_t i) const;
    MemMappedArea GetRmemMdpTarget   (int32_t i) const;
    MemMappedArea GetWmemMdpTarget   (int32_t i) const;
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    bool          IsParaGroupSwitched (void) const { return m_tr.is_para_grp_switched; }
#ifdef FROG_API_IF
    std::pair <PhysAddr, ByteSize> GetLLBitAdd (void) const;
    int32_t       GetDeletedLLBitCount     (void) const;
    std::pair <int32_t, PhysAddr> GetDeletedLLBit  (int32_t i) const;
    bool          DelLLBit    (int32_t peid, PhysAddr paddr);
#endif //FROG_API_IF
    NotifySimState GetSimState (void) const { return m_tr.sim_state; }
    const CRegVec* GetRregDepVec  (void) const { return &m_tr.rdep_regs; }
    const CRegVec* GetWregDepVec  (void) const { return &m_tr.wdep_regs; }
    const TraceInfo* GetTraceInfo (void) const { return &m_tr; }
    bool IsCancel (void) const { return m_tr.is_cancel; }
#if defined(COMP_RUNNER)
    bool IsCancelMcyc (void) const { return m_tr.is_cancel_mcyc; }
#endif // #if defined(COMP_RUNNER)

#ifdef SUPPORT_SC_HEAP
    SuspendState GetSuspendState (void) const;
    GuardErrType GetGuardError   (void) const;
    bool IsPswhChanged (void) const { return m_tr.is_pswh_changed; }
#endif // SUPPORT_SC_HEAP

#ifdef DEBUG_PRINT_STALL
    void SetLastIssueCycle (uint64_t cycle) { m_tr.m_last_issue_cycle = cycle; }
    uint64_t GetLastIssueCycle (void) const { return m_tr.m_last_issue_cycle; }
#endif // #ifdef DEBUG_PRINT_STALL

    bool IsInstIcHit (void) const { return m_tr.is_inst_ic_hit; }

    void Clean (void);
    void InitTime (void);
    //G4KH only
    void SetBraTargetInfo (bool is_branch_target);
    bool GetBraTargetInfo (void) const { return m_tr.m_is_branch_target; }

    explicit TraceCtrl (uint32_t pe_id);
    explicit TraceCtrl (const TraceInfo *tr);
    ~TraceCtrl () {}

#ifdef SUPPORT_SC_HEAP
    TraceCtrl () {}
#endif // SUPPORT_SC_HEAP

private:
    TraceInfo m_tr;
    template <RegCommand cmd> void RecordCReg (CReg creg, RegData data);
};
