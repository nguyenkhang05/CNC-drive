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
#include <string>
#include <algorithm>
#include "./fs_controler.h"
#include "./micro_arch_db.h"


class CsIbuf;
class CsIbufG3MH;
class CsIbufG3KH;
class FsCtrl;
class CsLsuBuf;
class SuspendCtrl;

class CsCtrl {
public:
    CsCtrl (uint32_t pe_id, FsCtrl* fsctrl, CsLsuBuf* cs_lsu_buf,
            CoreType core_type);
    ~CsCtrl ();

    uint64_t GetCycleCount (void) const { return m_cs_cycle_count; }

    void CsTraceSimulate (TraceCtrl* trace);
    void ElapsedCsTime (uint64_t erapsed_time);
    uint64_t CalcStallCount (void);
    void CsInit (void);
    void CsReset (void);
    void ChangeFsToCs (PhysAddr inst_addr);

    void AddHaltCycle (uint64_t cycle);
    void SetHaltCycle (uint64_t cycle) { m_halt_cycle = cycle; }
    void SetClkRatio (uint32_t ratio);
    void PreConsumeClock (TraceCtrl* trace);

    CsIbuf* m_ibuf;
    CsIbufG3MH* m_ibuf_g3mh;
    CsIbufG3KH* m_ibuf_g3kh;

    void RegisterRomAccess (uint64_t release_cycle, CReg reg, PhysAddr addr);
    void RegisterRomRegDependency (CReg src_reg, CReg dest_reg);
    void IncrementRomLatency (uint64_t curr_cycle, uint32_t conflict_latency);

    void SetNotifiedLatencyScheap (uint64_t release_cycle);
    bool IsLsuBufEmptyRecheck (TraceCtrl* trace);
    bool IsLsuBufEmpty (TraceCtrl* trace) const;
    uint32_t GetPeBaseCycleFromSystime (uint32_t cycle_sys_base) const;

    void SetFpuLatencyOffset (int32_t offset) { m_fpu_lat_offset = offset; }
    void SetDivfLatencyOffset (int32_t offset) { m_divf_lat_offset = offset; }

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

    void ForceUpdateIaAddress (PhysAddr ia_addr);
    void ShowAccessRoute (MemAccessCommand cmd, PhysAddr addr);
    uint64_t GetInstIssueStallPmc(void) const { return m_issue_stall_pmc; }

    uint64_t GetLsuIssueTime(void) const { return m_lsu_issue_time; }

private:
    static const uint32_t BR_CC_ALWAYS = 0x5;
    bool m_is_branch;   //previous instruction is branch
    bool m_is_taken;    //previous instruction is branch taken
    uint32_t  ifid_irque_valid;
    bool  if_valid;
    bool  if_request;
    bool m_is_prev_taken;   //previous instruction is branch taken
    bool m_is_prev_branch;  //previous instruction is branch
    bool reset_IFU_flag;
	bool m_is_prev_mul; //previous instruction is MUL
    bool m_is_3_arg;    //instruction has 3 argument
    bool m_is_lram_access;
    unsigned long m_is_prev_dw;

    std::vector<uint64_t> m_reg_wb_cycle;       //WB conflict buffer
    static const uint32_t WB_VAL_BUFF=40;  //buffer 40 elements stored cycle at WB stage: G4P
    std::vector<ArgFormat> m_arg_resource;

    uint32_t m_pe_id;
    FsCtrl* m_fsctrl;
    CsLsuBuf* m_cs_lsu_buf;

    uint64_t m_cs_cycle_count;
    uint64_t m_cs_lsu_cycle_pre;

    const CoreType m_core_type;
    CoreVersion m_core_version;

    uint32_t GetStallByMemBufFullG3M (TraceCtrl* trace);
    uint32_t GetStallByMemBufFullG3MH (TraceCtrl* trace, uint64_t cycle);
    uint32_t GetStallByMemBufFullG3KH (TraceCtrl* trace);

    typedef std::pair<uint64_t, CReg> RomAccessInfo;
    std::vector<RomAccessInfo> m_access_info_vec;
    std::vector<bool> m_has_rom_depend;
    std::vector<bool> m_has_reg_depend_list;
    typedef std::pair<CReg, CReg> DepRegInfo;
    std::vector<DepRegInfo> m_has_reg_depend;

    bool HasRomDependency (CReg src_reg) const { return m_has_rom_depend[src_reg]; }

    bool m_is_branch_predict_miss;
    bool m_is_force_fetch;
    uint32_t m_prev_issue_rate;
    uint64_t m_prev_issue_cycle;
    bool m_is_prev_mov;

    bool m_is_branch_target;  // 分岐ターゲット&分岐ターゲットと同時実行される命令に使用
    bool m_is_branch_target_next;
    uint32_t m_branch_target_addr;

    bool m_is_update_ibuf_time;

    bool    m_update_branch_target_fetch_cycle;
    int32_t m_branch_target_pos;
    bool m_update_branch_target_pos_by_fusion;

    bool m_is_zero_latency;                    //0latency-forward有効(要初期化）
    bool m_is_var_issue;                       // 可変発行レート有効（要初期化)
    std::vector<int32_t> m_wreg_var_latency;  // 可変レジスタ書込ステージ
    int32_t m_issue_var_rate;

    uint64_t m_last_d_stage;
    CRegVec m_dst_regs;

    std::vector<uint32_t> m_reg_active_stage;
    std::vector<uint64_t> m_reg_active_cycle;
    std::vector<InstGroupType> m_reg_write_resource;
    std::vector<bool> m_reg_load_lram;

    CRegVec m_reg_dst_prev;

    int32_t m_fpcc_ra;
    int32_t m_fpcc_rr;
    uint64_t m_fpcc_active_cycle;

    int32_t m_pswcc_rr;
    uint64_t m_pswcc_active_cycle;
    InstGroupType m_psw_write_res;

    uint64_t m_fpu_issue_avail_cycle;
    uint32_t m_fpu_issue_rate;

    uint64_t m_cs_last_cycle_count;
    uint64_t m_cs_fetch_cycle_count;
    uint64_t m_prev_cs_fecth_count;
    uint64_t m_extBusAvailCycle;

    uint64_t m_prev_fetch_cycle; // 前回フェッチから0x10境界経過までの期間計算用

    uint64_t m_halt_cycle;

    uint32_t m_dc_hit_latency;
    uint64_t m_dc_avail_cycle;

    static const uint32_t NUM_FXU_LDB = 4;
    std::vector<uint64_t> m_fxu_ldv_active_cycle;

    typedef std::pair<PhysAddr, uint64_t> DcFillInfo;
    std::vector<DcFillInfo> m_dc_fill_info;

    bool m_is_fusion_exec;
    int32_t m_prev_reg2;
    FusionInstType m_is_prev_fusionable_inst;
    void FusionCheck (TraceCtrl* trace);
    bool IsFusion (void) const { return m_is_fusion_exec; }
    void SetFusion (bool is_fusion) { m_is_fusion_exec = is_fusion; }
#ifdef DEBUG_CS
    uint64_t m_max_stall;
#endif // #ifdef DEBUG_CS

#ifdef SUPPORT_SC_HEAP

public:
    void SetSuspendCtrl (SuspendCtrl* suspend_ctrl);

    void AddIssueRate (uint32_t latency);
    void AddRegActiveCycle (CReg reg, uint32_t latency);
    bool IsParallelIssued (void) const { return m_parallel_issue_flag; }

    void CancelInst (TraceCtrl* trace);

private:
    SuspendCtrl* m_suspend_ctrl;
    uint64_t m_pre_consume_cycle;
    // access latency for unsupported case in scheap.
    static const uint32_t LATENCY_SCHEAP = 5; // Prevent fast access. 18 is read latency from GRAM.
#endif // SUPPORT_SC_HEAP

    uint64_t m_lsu_issue_time;

    ParaGroupType first_para_group;
    bool     m_parallel_issue_flag;

    bool m_is_id_branch_inst;
    uint32_t m_length_ic_to_dp;
    uint32_t m_length_if_to_id;
    int32_t m_length_id_to_next_if;

    void SetZeroLatency  (bool is_zero_latency);
    void InitVarLatency  (void);
    void SetVarIssue     (bool is_var_latency);
    void AddWregVarLatency  (int32_t latency);
    void SetVarIssueRate    (int32_t issue_rate);
    bool          IsZeroLatency   (void) const;
    bool          IsVarLatency    (void) const;
    bool          IsVarIssue      (void) const;
    int32_t       GetWregVarLatency  (int32_t pos) const;

public:
    int32_t       GetWregVarCount (void) const;

private:
    int32_t       GetVarIssueRate    (void) const;

    bool m_is_atomic;
    void SetAtomic (bool is_atomic) { m_is_atomic = is_atomic; }
    bool IsAtomic (void) const { return m_is_atomic; }

    uint64_t m_mac_use_cycle;
    uint64_t m_mac_inst_count;
    uint32_t m_mac_latency;

    bool m_is_load_incdec;
    void SetLoadIncDec (bool is_load_incdec) { m_is_load_incdec = is_load_incdec; }
    bool IsLoadIncDec (void) const { return m_is_load_incdec; }

    typedef void (CsCtrl::*INSTCSP) (TraceCtrl* trace);
    INSTCSP m_inst_cs[CCT_NUM];
    void CsBUncond (TraceCtrl* trace);
    void CsLdstMpu (TraceCtrl* trace);
    void CsMov (TraceCtrl* trace);
    void CsMovR0 (TraceCtrl* trace);
    void CsDisposePre (TraceCtrl* trace);
    void CsPrepare (TraceCtrl* trace);
    void CsPopspPre (TraceCtrl* trace);
    void CsPushsp (TraceCtrl* trace);
    void CsSwitch (TraceCtrl* trace);
    void CsCall (TraceCtrl* trace);
    void CsDiv (TraceCtrl* trace);
    void CsDivq (TraceCtrl* trace);
    void CsDivqu (TraceCtrl* trace);
    void CsDivqCommon (TraceCtrl* trace, uint32_t v2, uint32_t v1);
    void CsCmpf (TraceCtrl* trace);
    void CsCmpfd (TraceCtrl* trace);
    void CsTrfsr (TraceCtrl* trace);
    void CsFpuFpp (TraceCtrl* trace);
    void CsFpu2Rs (TraceCtrl* trace);
    void CsDivf (TraceCtrl* trace);
    void CsDivfd (TraceCtrl* trace);
    void CsMulfd (TraceCtrl* trace);
    void CsVld (TraceCtrl* trace);
    void CsRetSync (TraceCtrl* trace);
    void CsSync (TraceCtrl* trace);
    void CsRmw (TraceCtrl* trace);
    void CsDwLdSt (TraceCtrl* trace);
    void CsInvBuf (TraceCtrl* trace);
    void CsCmov (TraceCtrl* trace);
    void CsCmovf (TraceCtrl* trace);
    void CsCmovfd (TraceCtrl* trace);
    void CsLongInst (TraceCtrl* trace);
    void CsMac (TraceCtrl* trace);
    void CsNop (TraceCtrl* trace);
    void CsLdvQw (TraceCtrl* trace);
    void CsStvQw (TraceCtrl* trace);
    void CsLdIncDec (TraceCtrl* trace);
    void CsStc (TraceCtrl* trace);
    void CsStmMp (TraceCtrl* trace);
    void CsSyncPipe (void);
    void CsLdmPre (TraceCtrl* trace);
    void CsLdmMain (TraceCtrl* trace);
    void CsLdmPost (TraceCtrl* trace);
    void SetCsFuncTable (void);
    bool IsTargetScheap (MemAccessCommand cmd, TraceCtrl* trace) const;
    void CsDisposeScheap (TraceCtrl* trace);
    void CsPopspScheap (TraceCtrl* trace);

    void CsDisposeMain (TraceCtrl* trace);
    void CsDisposePost (TraceCtrl* trace);
    void CsPopspMain (TraceCtrl* trace);
    void CsPopspPost (TraceCtrl* trace);
    void CsResbankPre (TraceCtrl* trace);
    void CsResbankMain (TraceCtrl* trace);
    void CsResbankPost (TraceCtrl* trace);
    CReg FindResbankDstreg (TraceCtrl* trace, uint32_t dst_cnt) const;

    void CsCache (TraceCtrl* trace);

    bool IsKeptReqInQueue (TraceCtrl* trace) const;
    template <MemAccessCommand cmd>
    uint32_t GetBusIssueStallCycle (TraceCtrl* trace) const;
    template <MemAccessCommand cmd>
    uint32_t CalcLsuLatencyTotal (TraceCtrl* trace);
    template <MemAccessCommand cmd>
    uint32_t CalcLsuLatencyEach (TraceCtrl* trace, int32_t pos);
    void StageIF (TraceCtrl* trace);
    void StageIFG3MH (TraceCtrl* trace);
    void StageException (TraceCtrl* trace);
    template <CoreType core_type>
    void StageBranch (TraceCtrl* trace);
    void CalcBranchTargetCycleG3H (TraceCtrl* trace);
    void CalcBranchTargetCycle (TraceCtrl* trace);
    void InstIssueAnalysis (TraceCtrl* trace);
    template <CoreType core_type>
    uint32_t CalcRegStallCycle (TraceCtrl* trace, uint64_t issue_cycle);
    template <CoreType core_type>
    uint32_t CalcSrcRegStall (TraceCtrl* trace, uint64_t issue_cycle);
    template <CoreType core_type>
    uint32_t CalcDstRegStall (TraceCtrl* trace, uint64_t issue_cycle);
    uint32_t CalcResourceStall (TraceCtrl* trace, uint64_t issue_cycle);
    void UpdateRegAvailStage (TraceCtrl* trace);

    void CalcLsuLatency (TraceCtrl* trace);
    void CalcLsuReadLatency (TraceCtrl* trace);
    void CalcLsuWriteLatency (TraceCtrl* trace);

    uint32_t m_lsu_read_latency;

    template <CoreType core_type>
    void UpdateAvailCycle (TraceCtrl* trace);
    template <CoreType core_type>
    void UpdateIssueRate (TraceCtrl* trace);

    void UpdateCsInstFlag (TraceCtrl* trace);
    void UpdateInstMemFlag (TraceCtrl* trace);
    void DependencyAnalysis (TraceCtrl* trace);
    void ClearCycleCount (void);

    uint64_t m_last_lram_st_issue_cycle;
    PhysAddr m_last_lram_addr_word;
    MemAccessCommand m_last_lram_cmd;
    uint64_t m_last_lram_ea_cycle;

    // for SCHEAP Dispose/Prepare
    class MultiLoadInfo {
    public:
        enum MultiLoadType {
            MLD_DISPOSE,
            MLD_POPSP,
            MLD_RESBANK,
            MLD_LDM
        };

        bool m_is_valid;
        MultiLoadType m_mld_type;
        bool m_is_scheap;
        uint32_t m_reglist;
        uint32_t m_pop_num;
        bool m_is_64bit_aligned;
        bool m_check_first_reg;
        uint32_t m_mem_latency;
        uint32_t m_last_issue_wait;
        uint32_t m_dst_reg_latency;

        uint32_t m_cur_pos;
        uint32_t m_end_pos;

        void Init (void);
    };
    MultiLoadInfo m_mld_info;
    bool m_is_read_finished;

    inline uint32_t OperandExtract (uint64_t x, uint32_t l, uint32_t r) const {
        return static_cast<uint32_t> ((x >> r) & ~(~0x0ULL << (l - r + 1)));
    }

    // For Debug
    // Check wrong state
    // - There are 3 or more DP in a cycle
    // - There are 3 ore more EA in a cycle
    typedef std::pair<uint64_t, bool> PipeInfoType;
    std::vector<PipeInfoType> m_pipe_dp;
    std::vector<PipeInfoType> m_pipe_ex;
    void CheckPipeStatus (TraceCtrl* trace);

    // In-orderなのでvectorよりFIFOのほうが良い？
    typedef std::pair<uint64_t, uint32_t> RsInfoType; // cycle, rs_id
    std::vector<std::vector<RsInfoType>*> m_rs_cycle;
    std::vector<RsInfoType> m_rs_int;
    std::vector<RsInfoType> m_rs_ls;
    std::vector<RsInfoType> m_rs_fpu;
    std::vector<RsInfoType> m_rs_simd;
    std::vector<RsInfoType> m_rs_dp;
    std::vector<RsInfoType> m_rs_fxu;
    std::vector<RsInfoType> m_rs_undef;

    std::vector<uint64_t> m_rs_dummy;
    std::vector<uint64_t> m_rs_fxumov;
    std::vector<uint64_t> m_rs_fxuop;
    static const uint32_t NUM_RESERV_STATION = 2;
    static const uint32_t NUM_RESERV_STATION_FXUMOV = 4;
    static const uint32_t NUM_RESERV_STATION_FXUOP  = 1;
    static const uint32_t NUM_RESERV_STATION_FXU    = 1;

    uint64_t CalcFxuRsReleaseCycle (TraceCtrl* trace) const;
    void SetFxuRsReleaseCycle (TraceCtrl* trace, uint64_t ex_stage, uint32_t issue_rate);
    uint64_t m_vd_issue_able_cycle; // rs and vd is availavle.
    void UpdateVdIssueAbleCycle (TraceCtrl* trace);

    uint64_t CalcWbConflictCycle (TraceCtrl* trace, uint64_t issue_cycle);

    CoproType m_prev_copro_type;

    uint32_t m_rs_alu_id;
    typedef std::pair<uint64_t, bool> WbInfoType; // cycle, is_used
    std::vector<WbInfoType> m_wb_int_cycle[NUM_RESERV_STATION];

    void RleaseAndSortRs (TraceCtrl* trace, uint64_t shortest_dispatch_cycle);
    void SortRsId (std::vector<RsInfoType>* rs_vec);

    // FXU has RS at VD/EA stage
    std::vector<std::vector<uint64_t>*> m_rs_fxu_cycle;

    std::vector<std::vector<uint64_t>*> m_ex_cycle;
    std::vector<uint64_t> m_ex_int;
    std::vector<uint64_t> m_ex_bru;
    std::vector<uint64_t> m_ex_sru;
    std::vector<uint64_t> m_ex_div;
    std::vector<uint64_t> m_ex_mul;
    std::vector<uint64_t> m_ex_ls;
    std::vector<uint64_t> m_ex_fpu;
    std::vector<uint64_t> m_ex_simd;
    std::vector<uint64_t> m_ex_dp;
    std::vector<uint64_t> m_ex_fxumov;
    std::vector<uint64_t> m_ex_fxuop;
    std::vector<uint64_t> m_ex_undef;
    static const uint32_t NUM_EX_INT   = 2;
    static const uint32_t NUM_EX_MUL   = 1;
    static const uint32_t NUM_EX_SRU   = 1;
    static const uint32_t NUM_EX_BRU   = 1;
    static const uint32_t NUM_EX_DIV   = 1;
    static const uint32_t NUM_EX_LS    = 2;
    static const uint32_t NUM_EX_FPU   = 1;
    static const uint32_t NUM_EX_SIMD  = 1;
    static const uint32_t NUM_EX_DP    = 2; // DP execution (jr, br, nop)
    static const uint32_t NUM_EX_FXUMOV= 1;
    static const uint32_t NUM_EX_FXUOP = 1;
    static const uint32_t NUM_EX_UNDEF = 1;

    uint64_t m_vd_skid_avail_cycle;

    std::vector<uint64_t> m_last_ex_start_cycle;

    std::vector<uint64_t> m_rn_id_avail_cycle;
    uint32_t m_curr_rn_id;

    uint32_t m_inflight_id;
    static const uint32_t NUM_RN_BUF = 16;
    std::vector<uint64_t> m_cmt_stage;
    uint64_t m_last_cmt_cycle;
    bool m_is_cmt_in_parallel;

    uint64_t m_latest_cmt_cycle;
    uint64_t m_bpmiss_flush_cycle;

    uint64_t m_div_ra_cycle;

    uint64_t m_mulfd_ra_cycle;
    uint64_t m_mulfd_latency;

    uint64_t m_divf_ra_cycle;
    uint64_t m_divf_latency;

    uint64_t m_rmw_ra_cycle;
    uint64_t m_rmw_latency;
    uint64_t m_rmw_lsu_issue_cycle;
    //uint64_t m_bitop_block_cycle;
    bool m_is_first_call;

    void SyncDispatchCycle (uint64_t cycle);
    static const uint32_t DISPATCH_WINDOW_SIZE = 2;
    std::vector<uint64_t> m_dispatch_cycle;
    uint64_t m_last_dispatch_cycle;

    void SyncDispatchAvailCycleRS (TraceCtrl* trace, uint64_t rs_stage);

    BpResult m_last_bp_result;
    bool     m_is_bcond9;

    uint64_t m_shortest_issue_cycle;

    bool m_parallel_dispatch_flag;
    void InstDispatchAnalysisOoO (TraceCtrl* trace);
    uint32_t CalcDispatchLatency (TraceCtrl* trace);
    void SetDispatchAvailCycle (TraceCtrl* trace, uint64_t ex_end_stage);
    void OverwriteDispatchAvailCycleByFusionStall (TraceCtrl* trace, uint64_t new_ex_start_stage,
                                                   uint64_t old_ex_start_stage);
    void DependencyAnalysisOoO (TraceCtrl* trace);
    uint64_t CalcIssueCycle (TraceCtrl* trace) const;
    uint64_t LimitExIssue2inst (uint64_t ex_stage);
    void SyncAllIssueAvailCycle (TraceCtrl* trace, uint64_t ex_stage);
    bool IsInoIssueTarget (ExecResType res) const;
    void SetIssueAvailCycleAll (TraceCtrl* trace, uint64_t ex_end_stage);
    void SetIssueAvailCycle (TraceCtrl* trace, uint64_t ex_end_stage);
    void OverwriteIssueAvailCycle (ExecResType exec_resource, uint64_t new_ex_end_stage,
                                       uint64_t old_ex_end_stage);
    void CalcCommitCycle (TraceCtrl* trace, uint64_t ex_stage);

    int32_t m_mod_fpu_latency;
    int32_t m_fpu_lat_offset;
    int32_t m_divf_lat_offset;
    void CsFpuLatency (TraceCtrl* trace);

    bool m_use_2rs;
    bool m_ld_st_dw;

    enum StallCountType {
        STALL_CNT_BRANCH   = 0,
        STALL_CNT_FETCH    = 1,
        STALL_CNT_DATA_ALU = 2,
        STALL_CNT_DATA_LD  = 3,
        STALL_CNT_DATA_FPU = 4,
        STALL_CNT_RSRES    = 5,
        STALL_CNT_EXRES    = 6,
        STALL_CNT_MEMBUF   = 6,
        STALL_CNT_CMTBUF   = 6,
        STALL_CNT_NUM      = 7
    };

    class StallCounter {
    private:
        uint64_t m_total_cycle;
        //uint64_t m_min_cycle;
        //uint64_t m_max_cycle;
        //uint64_t m_active_count;
        std::string m_res_name;
        StallCountType m_id;

    public:
        uint64_t GetTotalCycle (void) const { return m_total_cycle; }
        //uint64_t GetMinCycle (void) const { return m_min_cycle; }
        //uint64_t GetMaxCycle (void) const { return m_max_cycle; }
        //uint64_t GetAvtiveCount (void) const { return m_active_count; }
        std::string GetName (void) const { return m_res_name; }
        void Count (uint64_t stall);
        void InitCount (void) {
            m_total_cycle = 0;
            //m_min_cycle = 0;
            //m_max_cycle = 0;
            //m_active_count = 0;
        }
        explicit StallCounter (std::string name, StallCountType id) {
            m_res_name = name;
            m_id = id;
            InitCount ();
        }
        ~StallCounter () {}
    };
    typedef std::vector<StallCounter*> StallCountVec;
    StallCountVec m_stall_counter;

    enum SourceResType {
        SRC_RES_ALU = 0,
        SRC_RES_LD = 1,
        SRC_RES_FPU = 2,
        SRC_RES_NUM = 3
    };

    uint64_t m_issue_stall_pmc;

    const uint32_t IQ_BYTES = 0x10;
    const PhysAddr LINE_MASK = IQ_BYTES - 1;

    bool m_is_tso;
    uint64_t m_st_req_avail_cycle;

#ifdef ENABLE_EXE_ORDER_CTRL
    bool m_is_ooo_g4mh;
#endif // #ifdef ENABLE_EXE_ORDER_CTRL

    const PipeStage m_issue_stage;
    const ArchDbType m_archdb_id;

    uint64_t m_last_id_cycle;
    uint64_t m_pref_avail_cycle;

    class BusInterfaceBase
    {
    public:
        virtual uint64_t GetAvailCycle (PhysAddr addr, MemAccessCommand cmd) const = 0;
        virtual void RecordAccessCycle (PhysAddr addr, MemAccessCommand cmd, uint64_t issue_cycle) = 0;
        virtual void DeleteOldEntry (uint64_t cycle) = 0;
        virtual void Init (void) = 0;
        virtual ~BusInterfaceBase () {}
    };
    std::vector<BusInterfaceBase*> m_interface_avail_cycle;

    // Note: Memory ordering
    // #XY means that subsequent Y is not moved before the preceding X.
    // SC (Seaquencial consistency) : #LoadStore | #LoadLoad | #StoreStore | #StoreLoad
    // TSO (Total Store Ordering) : #LoadLoad | #StoreStore | #LoadStore
    // STO (Store Ordering) : #StoreStore | #LoadStore
    // WC (Weak consistency) : (All access can be reordered)
    //enum MemOrderType {
    //MEM_ORDER_SC = 0, // Seaquencial consistency
    //MEM_ORDER_TSO = 1, // Relaxed consistency : Total Store Ordering
    //MEM_ORDER_STO = 2, // Relaxed consistency : Store Ordering (Store keeps access order.)
    //MEM_ORDER_WC = 3, // Weak consistency
    //};
    // G4MH1.x-2.x
    // L1RAM@OwnPE : STO (Although spec is TSO, STO is applied by impl bug.)
    // FAXI  : WC
    // Others: SC
    class BusInterfaceSC : public BusInterfaceBase
    {
    private:
        uint64_t m_avail_cycle;
    public:
        uint64_t GetAvailCycle (PhysAddr addr, MemAccessCommand cmd) const { return m_avail_cycle; }
        void RecordAccessCycle (PhysAddr addr, MemAccessCommand cmd, uint64_t issue_cycle)
        {
            // Although issue_cycle will be >= m_avail_cycle always, use std::max just to be sure.
            m_avail_cycle = std::max (m_avail_cycle, issue_cycle);
        }
        void DeleteOldEntry (uint64_t cycle) {};
        void Init (void)
        {
            m_avail_cycle = 0ULL;
        }
        BusInterfaceSC () { Init (); }
        ~BusInterfaceSC () {}
    };

    struct BusAccInfoType {
    PhysAddr      addr;
    MemAccessCommand cmd;
    uint64_t      issue_cycle;
    };

    // For G4MH LRAM@ownPE. For otherPE, SC is used.
    class BusInterfaceSTO : public BusInterfaceBase
    {
    private:
        std::vector<BusAccInfoType> m_acc_history;
    public:
        uint64_t GetAvailCycle (PhysAddr addr, MemAccessCommand cmd) const
        {
            uint64_t latest_cycle = 0;
            if (cmd == DATA_READ) {
                addr &= 0xFFFFFFFCU; // Check by word size address
                std::vector<BusAccInfoType>::const_iterator it = m_acc_history.begin ();
                while (it != m_acc_history.end ()) {
                    if (((*it).cmd == DATA_WRITE) && ((*it).addr == addr)) {
                        latest_cycle = (*it).issue_cycle;
                        break;
                    }
                    ++ it;
                }
            } else if (cmd == DATA_WRITE) {
                latest_cycle = m_acc_history.empty() ? 0 : (m_acc_history.front()).issue_cycle;
            }
            return latest_cycle;
        }
        void RecordAccessCycle (PhysAddr addr, MemAccessCommand cmd, uint64_t issue_cycle)
        {
            addr &= 0xFFFFFFFCU; // Use word size address
            //BusAccInfoType new_info = { addr, cmd, issue_cycle };
            // sort from the latest cycle
            std::vector<BusAccInfoType>::iterator it = m_acc_history.begin ();
            while (it != m_acc_history.end ()) {
                if ((*it).issue_cycle < issue_cycle) {
                    m_acc_history.insert (it, {addr, cmd, issue_cycle});
                    break;
                }
                ++ it;
            }
        }
        void DeleteOldEntry (uint64_t cycle)
        {
            std::vector<BusAccInfoType>::iterator it = m_acc_history.begin ();
            while (it != m_acc_history.end ()) {
                if ((*it).issue_cycle < cycle) {
                    m_acc_history.erase (it, m_acc_history.end());
                    break;
                }
                ++ it;
            }
        }
        void Init (void)
        {
            m_acc_history.clear ();
            m_acc_history.shrink_to_fit ();
        }
        BusInterfaceSTO () {}
        ~BusInterfaceSTO () {}
    };


    // In case of WC, No stall by memory ordering.
    class BusInterfaceWC : public BusInterfaceBase
    {
    public:
        uint64_t GetAvailCycle (PhysAddr addr, MemAccessCommand cmd) const { return 0ULL; }
        void RecordAccessCycle (PhysAddr addr, MemAccessCommand cmd, uint64_t issue_cycle) {}
        void DeleteOldEntry (uint64_t cycle) {};
        void Init (void) {}
        BusInterfaceWC () {}
        ~BusInterfaceWC () {}
    };
};
