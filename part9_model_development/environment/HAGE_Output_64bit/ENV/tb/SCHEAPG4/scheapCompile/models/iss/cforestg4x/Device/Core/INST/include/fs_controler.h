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
#include "./forest_common.h"
#include "./trace_info.h"
#include "./sregfile.h"
#include "./module_run.h"
#include "./micro_arch_db.h"
#include "./fs_controler_base.h"

class RegName;
class InstV850;
class FsBus;
class Icache;
class Lsu;
class CIntc1;
class CIntcBase;
#ifdef ENABLE_DBG_BREAK
class DbgBreak;
#endif // #ifdef ENABLE_DBG_BREAK
class TraceCtrl;
class TracePrint;
class GRegFile;
class InstQueue;
class Btb;
class BreakPoint;
class StatisticsCtrl;
class GRegFileSet;
class CsCtrl;
class Llsc;
class CsLsuBuf;
class Misr;
class ExpMsg;
class SuspendCtrl;
class CycleCounter;
class PmCounter;
class ProcElement;

class FsCtrl : public ModuleRun, public FsCtrl_interface {
public:
    int32_t   m_sc_id;  //! システム・コンテキストID

    uint32_t  m_pe_id;
    ContextId m_vc_id;
    ContextId m_tc_id;
    ArchClass  m_arch_class;
    bool      m_is_power_on;   // Clock/Power のON/OFF
    bool      m_is_ht_active;  // power_on & レジスタのht_enable設定
    bool      m_is_ht_runable; // FsExecuteの実行条件 (ht_active,intreq,snooze)
    bool      m_is_in_halt;
    bool      m_is_in_dbg;
    CoreType  m_core_type;
    CoreVersion m_core_version;

// V850
    // PE resources
    InstV850* m_insts;
    Icache*   m_ic;
    Lsu*      m_lsu;
    CIntc1*   m_intc1;
#ifdef ENABLE_DBG_BREAK
    DbgBreak* m_dbgbrk;
#endif // #ifdef ENABLE_DBG_BREAK
    Llsc*     m_llsc;
    Misr*     m_misr;          // Special PE outside resources
    ProcElement* m_pe;

    // VM resources

    // HT resources
    TraceCtrl*  m_trace;
    TraceCtrl*  m_trace_org;
public:
    TracePrint* m_trace_print;
    GRegFile*   m_grf;
    SRegFile*   m_srf; // Virtualized
    NcSRegFile* m_nc_srf; // Body
    InstQueue*  m_inst_queue;
    Btb*        m_btb;
    BreakPoint* m_break_points;
    StatisticsCtrl* m_statistics;
    CsCtrl*     m_csctrl;

    bool       m_is_prev_branched;
    BranchType m_branch_type;
    DebugMode  m_dbgmode;
    const SimulationMode  m_simmode;

    class IntCause {
    public:
        uint32_t GetGPID (void) const { return m_int_gpid; }
        uint32_t m_int_priority;
        uint32_t m_int_cause;
        bool m_eitb;
        bool     m_is_int_gm;
        uint32_t m_int_gpid;
        bool     m_is_int_bgint;
        void (*m_resfunc)(uint32_t channel, CIntc1* p_intc1);

        ExpCode GetEiintType (void) const
        {
            if (m_is_int_gm == false) {
                return EXP_EIINT;
            } else if (m_is_int_bgint == false) {
                return EXP_GMEIINT;
            } else {
                return EXP_BGEIINT;
            }
        }

#if defined(COMP_RUNNER)
        bool m_isinstdone;
#endif // defined(COMP_RUNNER)
        IntCause(uint32_t int_priority, uint32_t int_cause, bool eitb,
                 bool is_gm, uint32_t gpid, bool is_bgint,
                 void (*resfunc)(uint32_t channel, CIntc1* p_intc1))
            : m_int_priority (int_priority), m_int_cause (int_cause),
              m_eitb (eitb), m_is_int_gm (is_gm), m_int_gpid (gpid), m_is_int_bgint (is_bgint), m_resfunc (resfunc)
        {
#if defined(COMP_RUNNER)
            m_isinstdone = true;
#endif // defined(COMP_RUNNER)
        }
#if defined(COMP_RUNNER)
        IntCause(uint32_t int_priority, uint32_t int_cause, bool eitb,
                 bool is_gm, uint32_t gpid, bool is_bgint,
                 void (*resfunc)(uint32_t channel, CIntc1* p_intc1),
                 bool isinstdone)
            : m_int_priority (int_priority), m_int_cause (int_cause),
            m_eitb (eitb), m_is_int_gm (is_gm), m_int_gpid (gpid), m_is_int_bgint (is_bgint),
            m_resfunc (resfunc), m_isinstdone (isinstdone)
             {}
#endif // defined(COMP_RUNNER)
    };

    std::vector<IntCause*> m_int_queue;
    bool m_is_int_reqed;
    bool m_is_feint_reqed;
    bool m_is_int_acceptable;
    bool m_is_gmint_acceptable;
    bool m_is_bgint_acceptable;

    bool m_is_any_async_excp;
    bool IsAnyAsyncException (void) const { return m_is_any_async_excp; }
    void SetAnyAsyncException (bool is_excp) { m_is_any_async_excp = is_excp; }

    void ClearImsrAndIcsr (void);
    enum EiintMaskType {
        EIINT_MASK_GMPSWNP,   // Masked by GMPSW.NP
        EIINT_MASK_HMPSWNP,   // Masked by HMPSW.NP
        EIINT_MASK_GMPSWID,   // Masked by GMPSW.ID
        EIINT_MASK_HMPSWID,   // Masked by HMPSW.ID
        EIINT_MASK_PLMR,      // Masked by PLMR.PLM
        EIINT_MASK_ISPR,      // Masked by ISPR.ISP
        EIINT_MASK_PSWEIMASK = EIINT_MASK_ISPR, // Masked by PSW.EIMASK
    };
    void UpdateImsrAndIcsrByEiint (ExpCode eiint_type, EiintMaskType mask_type);

    enum FeintMaskType {
        FEINT_MASK_GMPSWNP,   // Masked by GMPSW.NP
        FEINT_MASK_HMPSWNP,   // Masked by HMPSW.NP
        FEINT_MASK_HMPSWID,   // Masked by PSW.ID for BGFEINT
    };
    void UpdateImsrByFeint (ExpCode feint_type, FeintMaskType mask_type);

    class FenmiCause {
    public:
        void (*m_resfunc)(CIntc1* intc1);
#if defined(COMP_RUNNER)
        bool m_isinstdone;
#endif // defined(COMP_RUNNER)
        FenmiCause (void (*resfunc)(CIntc1* p_intc1)) : m_resfunc (resfunc) {
#if defined(COMP_RUNNER)
            m_isinstdone = true;
#endif // defined(COMP_RUNNER)
        }
#if defined(COMP_RUNNER)
        FenmiCause (void (*resfunc)(CIntc1* p_intc1), bool isinstdone)
            : m_resfunc (resfunc), m_isinstdone (isinstdone)  {}
#endif // defined(COMP_RUNNER)
    };
    FenmiCause* m_fenmi_cause;
    bool m_is_fenmi_reqed;

    class FeintCause {
    public:
        uint32_t GetGPID (void) const { return m_feint_gpid; }
        void (*m_resfunc)(uint32_t channel, CIntc1* intc1);
        uint32_t m_feint_channel;
        bool     m_is_feint_gm;
        uint32_t m_feint_gpid;
        bool     m_is_feint_bgint;
        ExpCode GetFeintType (void) const
        {
            if (m_is_feint_gm == false) {
                return EXP_FEINT;
            } else if (m_is_feint_bgint == false) {
                return EXP_GMFEINT;
            } else {
                return EXP_BGFEINT;
            }
        }
#if defined(COMP_RUNNER)
        bool m_isinstdone;
#endif // defined(COMP_RUNNER)
        FeintCause (void (*resfunc)(uint32_t channel, CIntc1* p_intc1), uint32_t channel,
                    bool is_gm, uint32_t gpid, bool is_bgint)
                   : m_resfunc (resfunc), m_feint_channel (channel),
                     m_is_feint_gm (is_gm), m_feint_gpid (gpid), m_is_feint_bgint (is_bgint)
#ifdef  COMP_RUNNER
                    ,m_isinstdone (true)
#endif // defined(COMP_RUNNER)
        {}

#if defined(COMP_RUNNER)
        FeintCause (void (*resfunc)(uint32_t channel, CIntc1* p_intc1), uint32_t channel,
                    bool is_gm, uint32_t gpid, bool is_bgint, bool isinstdone)
            : m_resfunc (resfunc), m_feint_channel (channel),
              m_is_feint_gm (is_gm), m_feint_gpid (gpid), m_isinstdone (isinstdone)  {}
#endif // defined(COMP_RUNNER)
    };
    FeintCause* m_feint_cause;
    bool m_is_feint_acceptable;
    bool m_is_gmfeint_acceptable;
    bool m_is_bgfeint_acceptable;
    bool m_is_host_op_syserr_acceptable;
    bool m_is_guest_op_syserr_acceptable;

    class SyserrCause {
    public:
        uint32_t m_syserr_cause;
        bool     m_is_caused_by_host;
        bool     m_is_accepted_by_host;
        SyserrCause (uint32_t syserr_cause, bool is_caused_by_host, bool is_accepted_by_host)
        : m_syserr_cause (syserr_cause), m_is_caused_by_host (is_caused_by_host)
        , m_is_accepted_by_host (is_accepted_by_host) {}
    };
    SyserrCause* m_syserr_cause_pseudo_if;
    SyserrCause* m_syserr_cause_op;

    bool m_is_syserr_reqed_pseudo_if;
    bool m_is_syserr_reqed_op;

    bool m_req_rlb;
    bool m_req_dbnmi;
    bool m_req_dbint;
    uint32_t m_channel_dbint;
    bool m_is_dir0dm_0;

    /**
     * \var bool m_all_interrupt_disable
     * \brief This variable will disable the assertion for all interruption types.
     *        It is used in the following cases.
     *        1. Support stopreq signal for FROG I/F operation.
     */
    bool m_all_interrupt_disable;

    typedef uint32_t IntFactorType;
    static const IntFactorType INT_FAC_NONE = 0x0;
    static const IntFactorType INT_FAC_INT  = 0x1;
    static const IntFactorType INT_FAC_FPI  = 0x2;
    static const IntFactorType INT_FAC_FENMI = 0x4;
    static const IntFactorType INT_FAC_FEINT = 0x5;
    static const IntFactorType INT_FAC_SYSERR_OP = 0x6;
    static const IntFactorType INT_FAC_RMINT = 0x7;
    static const IntFactorType INT_FAC_DBINT = 0x8;
    static const IntFactorType INT_FAC_DBNMI = 0x9;
    static const IntFactorType INT_FAC_RLB   = 0xA;
    static const IntFactorType INT_FAC_PSEUDO_SYSERR_IF = 0xB; // Dummy SYSERR fetch by ext debugger

    typedef uint32_t IntType;
    static const IntType INT_TYPE_NONE = 0;
    static const IntType INT_TYPE_GM = 1;
    static const IntType INT_TYPE_BG = 2;
    static const IntType INT_TYPE_HOST = 3;

    IntFactorType m_selected_factor;
    IntFactorType m_selected_factor_to_nc;

    static const uint32_t INT_PRI_MAX = 0x10;


    void SetScId (int32_t sc_id);

#if defined(COMP_RUNNER_G3KH)
    void SetINTState (bool int_nc);
    void SetINTState (bool int_nc, uint32_t pri, uint32_t ch);
    void SetFEINTState (bool int_nc);
    void SetSYSERRState (bool int_nc);
    void SetSYSERRIFState (bool int_nc);
#endif // defined(COMP_RUNNER_G3KH)

    void FsExecute (void);
    void FsStepExecute (void);
    void FsInit (void);
    /**
     * \fn void HardReset (void);
     * \brief Change State of simulator to RESET. Request to reset function and cycle of individual PE. This is a.k.a Power Reset.
     *
     * This function is used by python command reset().
     *
     * @param -.
     * @return None
     */
    void HardReset (void);
    /**
     * \fn void FsReset (void);
     * \brief Change State of simulator to RESET. Then, it send the request to reset individual PE function.
     *
     * This function is used by python command reset(self, *contextid)
     *                     and set_event (address, "pereset", context).
     *
     * @param -.
     * @return None
     */
    void FsReset (void);
    /**
     * \fn void FsBaseReset (void);
     * \brief Reset basic operation current PE. There is no change in cycle count.
     *
     * @param -.
     * @return None
     */
    void FsBaseReset (void);
    void RestartVm (void);
    void ResetSideBand (void);

    void SetHtPowerState (bool active);
    void UpdateHtActive (void);
    void UpdatePrivLevel (void);
    void UpdateHtRunable (void);
    PrivLevel GetPrivLevel (void) const;

    void EnableDebugMode (DebugMode mode);
    void DisableDebugOption (std::string option);
    DebugMode GetDebugMode (void);
    void InitTracePrintRTL (FsCtrl* nptr, FILE* fp);
    bool IsDebugMode (DebugMode mode) { return ((m_dbgmode & mode) != 0x0); }
    bool IsDebugModeAny (void) { return (m_dbgmode != 0x0); }

    IntErrorCode ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                               bool is_gm, uint32_t gpid, bool is_bgint,
                               void (*resfunc)(uint32_t channel, CIntc1* p_intc1));

    IntErrorCode ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                               void (*resfunc)(uint32_t channel, CIntc1* p_intc1))
    {
        return ReqInterrupt (int_priority, int_cause, eitb, false, 0, false, resfunc);
    }
#if defined(COMP_RUNNER)
    IntErrorCode ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                               bool is_gm, uint32_t gpid, bool is_bgint,
                               void (*resfunc)(uint32_t channel, CIntc1* intc1),
                               bool is_initdone);
    IntErrorCode ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                               void (*resfunc)(uint32_t channel, CIntc1* intc1),
                               bool is_initdone)
    {
        return ReqInterrupt (int_priority, int_cause, eitb, false, 0, false, resfunc, is_initdone);
    }

    IntErrorCode ReqFenmi (void (*resfunc)(CIntc1* p_intc1), bool is_initdone);
    IntErrorCode ReqFeint (void (*resfunc)(uint32_t channel, CIntc1* p_intc1), bool is_initdone,
                           uint32_t channel, bool is_gm, uint32_t gpid, bool is_bgint);
    IntErrorCode ReqFeint (void (*resfunc)(uint32_t channel, CIntc1* p_intc1), bool is_initdone,
                           uint32_t channel)
    {
        return ReqFeint (resfunc, is_initdone, channel, false, 0, false); // Cause FEINT
    }
#endif // defined(COMP_RUNNER)
    IntErrorCode CancelInterrupt (uint32_t int_cause);
    IntErrorCode CancelInterrupt (void);
    void ClearIntReqQueue (void);
    static bool GreaterIntPriority (const IntCause* left, const IntCause* right);
    IntErrorCode ReqFenmi (void (*resfunc)(CIntc1* p_intc1));
    IntErrorCode CancelFenmi (void);
    IntErrorCode ReqFeint (void (*resfunc)(uint32_t channel, CIntc1* p_intc1), uint32_t channel,
                           bool is_gm, uint32_t gpid, bool is_bgint);
    IntErrorCode ReqFeint (void (*resfunc)(uint32_t , CIntc1* ), uint32_t channel)
    {
        return ReqFeint (resfunc, channel, false, 0, false); // Cause FEINT
    }
    IntErrorCode CancelFeint (void);
    IntErrorCode ReqOperandSyserr (uint32_t syserr_cause, bool is_host, uint32_t gpid);
    IntErrorCode NoticeOperandErrorResponse (uint32_t cause, bool is_host, uint32_t gpid);
    IntErrorCode CancelOperandSyserr (void);
    IntErrorCode CancelOperandSyserr (bool is_host, uint32_t gpid);
    IntErrorCode ReqPseudoFetchSyserr (uint32_t syserr_cause);
    IntErrorCode CancelPseudoFetchSyserr (void);

private:
    IntErrorCode ReqOperandSyserrInternal (uint32_t cause,
                                           bool is_caused_by_host, bool is_accepted_by_host);
    void ClearFenmiReqQueue (void);
    void ClearFeintReqQueue (void);
    void ClearOperandSyserrReqQueue (void);
    void ClearPseudoFetchSyserrReqQueue (void);

public:
    void UpdateOperandSyserrEvent (void);
    IntErrorCode ReqRlb (uint32_t source_peid);
    IntErrorCode CancelRlb (uint32_t source_peid);
    IntErrorCode ReqDbnmi (void);
    IntErrorCode CancelDbnmi (void);
    IntErrorCode ReqDbint (uint32_t channel);
    IntErrorCode CancelDbint (void);
    IntErrorCode ReqRmint (void);
    IntErrorCode CancelRmint (void);

    /**
     * \fn IntErrorCode ReqAllInterruptDisable (void);
     * \brief Request for setting the disabled assertion in all interruption types.
     *
     * This function is called many times in the following cases..
     * 1. Support stopreq signal for FROG I/F operation.
     *
     * @param[in]  -
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
    IntErrorCode ReqAllInterruptDisable (void);
    /**
     * \fn IntErrorCode CancelAllInterruptDisable (void);
     * \brief Request to CPU for canceling the disabled assertion in all interruption types.
     *
     * This function is called many times in the following cases..
     * 1. Support stopreq signal for FROG I/F operation.
     *
     * @param[in]  -
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
    IntErrorCode CancelAllInterruptDisable (void);
    bool IsEnableDebugMonitor (void) const;
    bool IsIntAcceptable (void) const { return m_is_int_acceptable; }
    bool IsGmIntAcceptable (void) const { return m_is_gmint_acceptable; }
    bool IsBgIntAcceptable (void) const { return m_is_bgint_acceptable; }
    IntFactorType GetSelectedIntFactor (void) const { return m_selected_factor; }
    IntFactorType GetSelectedIntFactorToNc (void) const
    { return m_selected_factor_to_nc; }
    bool m_has_intreq_to_nc;
    void SetHasIntreqToNc (bool has_intreq_to_nc)
        { m_has_intreq_to_nc = has_intreq_to_nc; }
    void UpdateIntAcceptable (void);
    void UpdateGmIntAcceptable (void);
    void UpdateBgIntAcceptable (void);
    void UpdateFeintAcceptable (void);
    void UpdateGmFeintAcceptable (void);
    void UpdateBgFeintAcceptable (void);
    void UpdateOperandSyserrAcceptable (void);
    void UpdateIntRequested (void);
    void UpdateIntStatus (void);
    IntCause* GetIntCause (void);
    uint32_t GetEiintPriority (void);
    uint32_t GetFpiPriority (void);
    bool IsIsprMasked (uint32_t pri);
    bool IsHmPlmrMasked (uint32_t pri);
    bool IsGmPlmrMasked (uint32_t pri);
    bool IsHmEimaskMasked (uint32_t pri);
    bool IsGmEimaskMasked (uint32_t pri);
    uint32_t GetIsprHighPriority (void);

    void CauseAsyncDataBreak (void);
    void ExceptionProcess (ExpCode exp_code);
    void InterruptProcess (void);
    void ExpInit (void);

    ExpMsg* m_exp_msg;

//cedar_func.c
    FILE*     m_log_file;  // CPU 個別のログ出力先
    uint32_t  m_clk_ratio; // CPU周期
    uint64_t  m_cpu_time;  // CPU時間（CPUOFFでもカウント)
    uint64_t  m_bus_time;  // LSU時間（CPUOFFでもカウント)

    // SnoozeState
    bool m_is_snooze;
    uint64_t m_snooze_wakeup_time;
    uint64_t m_snooze_start_time;
    void SetSnooze (bool state);
    uint16_t m_snooze_period;
    void SetSnoozePeriod (uint16_t period) { m_snooze_period = period; }
    uint16_t GetSnoozePeriod (void) const { return m_snooze_period; }

    static const bool INIT_TABLE_REF_INT = true;
    bool m_support_table_ref_int;
    void SetSupportTableRefInt (bool is_support) { m_support_table_ref_int = is_support; }
    bool IsTableRefIntSupported (void) const { return m_support_table_ref_int; }

    SimulationMode GetSimMode (void) { return m_simmode; }

    CReg RegGetRegno (std::string regname);
    CfSReg GetCorrectCfSRegID (uint32_t peid, CfSReg regid) const;
    void MemWriteDebug (PhysAddr vrtl_addr, MemData data, ByteSize size) const;
    MemData MemReadDebug (PhysAddr vrtl_addr, ByteSize size) const;
    void AddBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask);
    void DisplayBreakAccess (void);
    void ClearBreakAccess (PhysAddr start, PhysAddr end, AccessAttr attr, uint32_t mask);
    void AddBreakNextPc (uint32_t address, uint32_t mask);
    void DisplayBreakPc (void);
    void ClearBreakNextPc (uint32_t address, uint32_t mask);
    void SetBreakMax (uint64_t count);
    uint64_t GetBreakMax (void);
    void SetBreakSamePcMax (uint64_t count);
    uint64_t GetBreakSamePcMax (void) const;
    void UpdateHaltState (void);
    const TraceInfo* GetTraceInfo (void) const;
    TraceCtrl* GetTraceCtrl (void) const { return m_trace; }
    void CountCpuTime(uint64_t wait_cycle);
    void SetCpuTime (uint64_t num);
    void SetCpuRatio (uint32_t num);
    void SetFilePointer (FILE* fp);
    uint32_t GetCpuRatio (void) const;
    FILE* GetTraceFilePtrRTL (void) const;
    ContextId GetTcId (void) const;
    ContextId GetVcId (void) const;
    uint32_t  GetPeId (void) const;
    void UpdateNextInstAddr (void);
    void UpdateNextTcInfo (FsCtrl* next_tc);
    void UpdateNextInst (void);
    void UpdateSimState (void);
    void ChangeSimState (NotifySimState state);
    uint64_t GetCycleCount (void) const;

    bool m_is_fpp_mode;
    void SetFppMode (bool is_fpp_mode) { m_is_fpp_mode = is_fpp_mode; }
    bool IsFppMode (void) const { return m_is_fpp_mode; }

    void InitInstQueue (void);

#ifdef ENABLE_PIC
    void DisplayInstructionCount (FILE* local_file);
    void ClearInstructionCount (void);
#endif // ENABLE_PIC

    PhysAddr m_line_addr;

#ifdef SUPPORT_SC_HEAP
    SuspendCtrl* m_suspend_ctrl;

    virtual bool IsSuspendState (void) const;
    SuspendState GetSuspendState (void) const;
    void NotifyFetchLatency (PhysAddr addr, uint32_t latency);
    void NotifyReadLatency  (PhysAddr addr, uint32_t latency, uint64_t data);
    void NotifyWriteLatency (PhysAddr addr, uint32_t latency);

    TraceCtrl* GetNewTrace (void);
    void AddBreakMatchPc (uint32_t address);
    void ClearBreakMatchPc (uint32_t address);
#endif // SUPPORT_SC_HEAP

    void PreConsumeClock (void);
    void SetTraceCtrl (TraceCtrl* next_trace);

    void ExecCfali (PhysAddr addr);

    FsCtrl (uint32_t pe_id, ContextId vc_id, ContextId tc_id, ArchClass arch_class,
            FsBus* fsbus, Icache* ic, CIntc1* intc1,
#ifdef ENABLE_DBG_BREAK
            DbgBreak* dbgbrk,
#endif // #ifdef ENABLE_DBG_BREAK
            Llsc* llsc,
            Misr* misr, ProcElement* pe,
            GRegFile* grf, SRegFile* srf,
            NcSRegFile* nc_srf,
            GRegFileSet* grfs, CsLsuBuf* cs_lsu_buf, CoreType core_type, CoreVersion core_version);
    ~FsCtrl ();

    IsElapsedTime RunModule (void);
    void ClockWait (uint64_t wait_cycle);
    void AddHaltCycle (uint64_t halt_cycle);
    void SetHaltCycle (uint64_t cycle);
    void UpdateSnoozeState (void);
    bool IsBreakHitPreExec (void) const;
    bool IsBreakHitPostExec (void) const;
    bool IsHtRunable (void) const;
    bool IsSnooze (void) const;
    bool IsHtActive (void) const;

#ifdef MASK_INT_BY_GPID
    bool IsMaskGuestEIINT (void) const;
    bool IsMaskGuestFEINT (void) const;
#endif // #ifdef MASK_INT_BY_GPID

    bool IsIntRequested (void) const { return m_is_int_reqed; }
    bool IsFenmiRequested (void) const { return m_is_fenmi_reqed; }
    bool IsFeintRequested (void) const { return m_is_feint_reqed; }
    bool IsOperandSyserrRequested (void) const { return m_is_syserr_reqed_op; }
    bool IsPseudoFetchSyserrRequested (void) const { return m_is_syserr_reqed_pseudo_if; }
    bool IsFeintAcceptable (void) const { return m_is_feint_acceptable; }
    bool IsGmFeintAcceptable (void) const { return m_is_gmfeint_acceptable; }
    bool IsBgFeintAcceptable (void) const { return m_is_bgfeint_acceptable; }
    bool IsHostOperandSyserrAcceptable (void) const { return m_is_host_op_syserr_acceptable; }
    bool IsGuestOperandSyserrAcceptable (void) const { return m_is_guest_op_syserr_acceptable; }
    bool IsRlbRequested (void) const { return m_req_rlb; }
    bool IsDbnmiRequested (void) const { return m_req_dbnmi; }
    bool IsDbintRequested (void) const { return m_req_dbint; }
    bool IsDir0DM0 (void) const { return m_is_dir0dm_0; }
    void UpdateDir0DmStatus (void);
    void SetHaltState (bool is_halt);
    bool IsInHalt (void) const;
    bool IsCpu (void) const { return true;}
    FILE* GetFilePointer (void) const;
    int32_t GetScId (void) const;
    uint64_t GetCpuTime (void) const;
    uint64_t GetBusTime (void) const;
    void SetGramArea (PhysAddr start_addr, PhysAddr end_addr);
    void SetLramArea (PhysAddr start_addr, PhysAddr end_addr);

    void SetBusTime (uint64_t bus_time);

    uint32_t m_syserr_fetch_cause;

    void DumpBtbStatistics (void) const;
    void ResetBtbCounter (void);

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

    void SetFpuLatencyOffset (int32_t offset);
    void SetDivfLatencyOffset (int32_t offset);
#ifdef FROG_API_IF
    void DeleteLLBit (PhysAddr paddr);
    void CreateLLBit (int32_t scid, PhysAddr paddr, ByteSize size);
#endif // FROG_API_IF

    CycleCounter* GetTsCounter (void) { return m_tscount; }
    PmCounter* GetPmCounter (uint32_t id) { return m_pmcount[id]; }

#if defined(COMP_RUNNER)
    int32_t m_cancel_timign_mcyc;
    void SetCanselTimingMcycInst (int32_t count) { m_cancel_timign_mcyc = count; }
    int32_t GetCancelTimingMcycInst (void) const { return m_cancel_timign_mcyc; }
#endif // #if defined(COMP_RUNNER)

    uint64_t GetBranchCount (void) const { return m_branch_count; }
    uint64_t GetBranchCondExecCount (void) const { return m_branch_cond_exec_count; }
    uint64_t GetBranchCondMissCount (void) const { return m_branch_cond_miss_count; }
    uint64_t GetEiintCount (uint32_t ch) const { return m_eiint_count[ch]; }
    uint64_t GetFeintCount (uint32_t ch) const { return m_feint_count[ch]; }
    uint64_t GetExpAbortCount (uint32_t ch) const { return m_exp_abort_count[ch]; }
    uint64_t GetExpNonAbortCount (uint32_t ch) const { return m_exp_not_abort_count[ch]; }
    uint64_t GetBgintAbortCount (uint32_t ch) const;
    uint64_t GetBgintAccesptedCount (uint32_t ch) const;
    uint64_t GetInstCountPmc (void) const { return m_inst_count_pmc; }

    void CountBgintEvent (uint32_t accepted_gpid);

    bool m_update_pmc;
    void SetPmcUpdateFlag (void) { m_update_pmc = true; }
    void UpdateCounters (void);
    bool m_reserved_pmc_cond;
    void SetPmcReservedFlag (void) { m_reserved_pmc_cond = true; }

    PhysAddr GetLastCompletedPc (void) const { return m_last_completed_pc; }

private:
    void StageIF (void);
    void StageID (void);
    void StageEX (void);
    void UpdatePC (void);
    void SetPreExecException (int32_t inst_id);
    bool IsExcpPrivOccured   (int32_t inst_id) const;
    bool IsExcpCoproOccured  (int32_t inst_id) const;
    void AdjustSnoozeTime (void);

    uint64_t m_cycle_count_offset;
    uint64_t m_para_issue_count;

    PhysAddr m_last_completed_pc; // Don't initialize by Reset

    const ArchDbType m_archdb_id;

    FsCtrl ();

    void CountPerformance(void);
    bool m_is_run_ndbg;
    bool m_is_run_ch28;
    bool m_is_run_ch29;

    uint32_t m_accepted_eiint_ch;
    uint32_t m_accepted_feint_ch;

    CycleCounter* m_tscount;
    std::vector <PmCounter*> m_pmcount;

    uint64_t m_branch_count; // PMCTRL.CND=18h
    uint64_t m_branch_cond_exec_count; // PMCTRL.CND=19h
    uint64_t m_branch_cond_miss_count; // PMCTRL.CND=1Ah
    std::vector <uint64_t> m_eiint_count; // PMCTRL.CND=20h
    std::vector <uint64_t> m_feint_count; // PMCTRL.CND=21h
    std::vector <uint64_t> m_exp_abort_count; // PMCTRL.CND=22h
    std::vector <uint64_t> m_exp_not_abort_count; // PMCTRL.CND=23h
    std::vector <uint64_t> m_bgint_abort_count; // PMCTRL.CND=24h
    std::vector <uint64_t> m_bgint_accepted_count; // PMCTRL.CND=25h
    uint64_t m_inst_count_pmc; // PMCTRL.CND=10

    bool m_previous_is_dbmode;
    bool m_is_dbmode;

    bool HasPrivileged (PrivLevel req_priv, PrivLevel context_priv) const {
        return ((req_priv & context_priv) != PRIV_PERMIT_NONE);
    }

    std::string GetPrivName (PrivLevel priv) const {
        std::string priv_str;
        switch (priv) {
        case PRIV_PERMIT_UM:
        case PRIV_HAS_UM:
            priv_str = "UM";
            break;
        case PRIV_PERMIT_SV:
        case PRIV_HAS_SV:
            priv_str = "SV";
            break;
        case PRIV_PERMIT_HV:
        case PRIV_HAS_HV:
            priv_str = "HV";
            break;
        case PRIV_PERMIT_DBUM:
            priv_str = "DB";
            break;
        case PRIV_HAS_DBUM:
            priv_str = "DBUM";
            break;
        case PRIV_PERMIT_DBSV:
        case PRIV_HAS_DBSV:
        case PRIV_PERMIT_DBHV:
        //case PRIV_HAS_DBHV: // same value with PRIV_PERMIT_DBHV
            priv_str = "DB";
            break;
        default:
            priv_str = "Unknown"; // don't come here
            break;
        }
        return priv_str;
    }
};
