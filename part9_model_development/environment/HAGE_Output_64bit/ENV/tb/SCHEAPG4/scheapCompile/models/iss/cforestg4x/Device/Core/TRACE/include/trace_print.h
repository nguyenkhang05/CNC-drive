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

#include "./trace_operand.h"

class RegName;
class TraceCtrl;
class SRegFile;


class TracePrintCForest {
public:
    void Print (SimulationMode sim_mode, uint32_t dbg_mode, const TraceCtrl* trace) const;
    TracePrintCForest (CoreType core_type) : m_core_type (core_type) {}
    virtual ~TracePrintCForest () {}

#if defined(COMP_RUNNER) || defined(FROG_API_IF)
    void PrintRegMemTrace (std::ostringstream* oss, const TraceCtrl* trace) const;
    void PrintBranchTrace (std::ostringstream* oss, const TraceCtrl* trace) const;
    void PrintPipeTrace   (std::ostringstream* oss, const TraceCtrl* trace) const;
#endif // defined(COMP_RUNNER) || defined(FROG_API_IF)

private:
    static const int32_t PIPE_DISP_LEN    =   60;
    static const int32_t MNEMONIC_CUT_LEN =   53;
    CoreType m_core_type;

    TracePrintOperand m_print_operand;

    // SC-HEAPでの上書き用に仮想関数化
    virtual void PrintCycleTrace (std::ostringstream* oss,
                           SimulationMode sim_mode, const TraceCtrl* trace) const;
    virtual void PrintOpcodeTrace (std::ostringstream* oss,
                           SimulationMode sim_mode, const TraceCtrl* trace) const;
    void PrintBasicTrace  (std::ostringstream* oss,
                           SimulationMode sim_mode, const TraceCtrl* trace) const;
    void PrintExpTrace    (std::ostringstream* oss,
                           SimulationMode sim_mode, const TraceCtrl* trace) const;
#if defined(COMP_RUNNER) || defined(FROG_API_IF)
#else  // defined(COMP_RUNNER) || defined(FROG_API_IF)
    void PrintRegMemTrace (std::ostringstream* oss, const TraceCtrl* trace) const;
    void PrintBranchTrace (std::ostringstream* oss, const TraceCtrl* trace) const;
    void PrintPipeTrace   (std::ostringstream* oss, const TraceCtrl* trace) const;
#if defined(MAKE_FP_UNIT_TEST)
    void PrintUT32info    (std::ostringstream* oss, const TraceCtrl* trace) const;
    void PrintUT128info   (std::ostringstream* oss, const TraceCtrl* trace) const;
#endif
#endif // defined(COMP_RUNNER) || defined(FROG_API_IF)
    void PrintIcTrace (std::ostringstream* oss, const TraceCtrl* trace) const;
    void PrintPipeBlank   (std::ostringstream* oss, const TraceCtrl* trace) const;
    void PrintDependTrace (std::ostringstream* oss, const TraceCtrl* trace) const;
    //void PrintBranchTrace (std::ostringstream* oss, const TraceCtrl* trace) const;
};


class TracePrintRTL {
public:
    void Print (const TraceCtrl* trace) const;

    void PrintExp (const TraceCtrl* trace) const;

    FILE* GetTraceFilePtr (void) const { return m_trace_file; }

    uint64_t GetStep (void) { return m_inst_step; }
    void     IncStep (void) { m_inst_step++; }

    void InitPrintRTL (FILE* fp, TracePrintRTL* nc_ptr);

    explicit TracePrintRTL (SRegFile* sr);
    ~TracePrintRTL () { ; }

private:
    static const int32_t MAX_LOG_LEN = 256;

    TracePrintOperand m_print_operand;

    // TC0 から NC のステップ数を参照するために用いる
    TracePrintRTL* m_nc_ptr;

    // TC毎に個別のシステムレジスタを参照する
    SRegFile* m_srf;

    FILE* m_trace_file;
    const CReg m_psw_regno;
    mutable uint64_t m_inst_step;

    void PrintContextID (const TraceCtrl* trace, std::ostringstream* cntext_str) const;
    const char* PrintExpName (ExpCode exp_code) const;
};


class TracePrint {
public:
    void Print (SimulationMode sim_mode, uint32_t dbg_mode, const TraceCtrl* trace) const;

    TracePrintRTL* GetPtrRTL (void) { return m_print_rtl; }

    FILE* GetTraceFilePtrRTL () { return m_print_rtl->GetTraceFilePtr (); }

    void EnableTracePrint (void) { m_print_func = &TracePrint::PrintDebug; }

    void InitPrintRTL (FILE* fp, TracePrintRTL* nc_ptr);

    TracePrint (SRegFile* srf, CoreType core_type);
    ~TracePrint ();

private:
    TracePrintCForest* m_print_cf;
    TracePrintRTL*     m_print_rtl;

    TracePrintOperand* m_print_operand;

    bool m_enable_print_rtl;

    // トレース表示ON/OFF判定用の関数ポインタ
    void (TracePrint::*m_print_func) (SimulationMode sim_mode, uint32_t dbg_mode,
                                      const TraceCtrl* trace) const;

    // トレース表示ON用の関数
    void PrintDebug (SimulationMode sim_mode, uint32_t dbg_mode,
                     const TraceCtrl* trace) const;

    // トレースモードOFF用の関数(何も行わない)
    void PrintDummy (SimulationMode sim_mode, uint32_t dbg_mode,
                     const TraceCtrl* trace) const {}
};
