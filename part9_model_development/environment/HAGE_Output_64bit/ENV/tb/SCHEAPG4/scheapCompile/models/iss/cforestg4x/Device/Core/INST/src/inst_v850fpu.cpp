/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./forest_common.h"
#include "./inst_v850.h"
#include "./proc_element.h"
#include "./trace_info.h"
#include "./gregfile.h"
#include "./sregfile.h"
#include "./mpu.h"
#include "./lsu.h"
#include "./fpu_base.h"
#include "./forest_message.h"
#include "./fs_controler.h"
#include "./exp_message.h"
#include "./exp_info.h"


void InstV850::Inst_ABSFD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Abs (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ABSFS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Abs (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ADDFD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    FpuRegDouble v1 (GRegRead64 (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Add (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ADDFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Add (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFDW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Ceil_F64_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFDUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Ceil_F64_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFDL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Ceil_F64_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFDUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Ceil_F64_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFSW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Ceil_F32_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFSUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Ceil_F32_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFSL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Ceil_F32_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CEILFSUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Ceil_F32_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CMOVFD (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2  = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3  = OperandExtract (opcode, 31, 28) << 1;
    uint32_t fcbit = OperandExtract (opcode, 19, 17);

    uint32_t fpscr_cc = m_fpsr->GetCC ();
    bool ccbit = (fpscr_cc >> fcbit) & 0x01;

    uint64_t res;
    if (ccbit) {
        res = GRegRead64 (reg1);
    } else {
        res = GRegRead64 (reg2);
    }
    GRegWrite64 (reg3, res);
}


void InstV850::Inst_CMOVFS (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t reg3  = OperandExtract (opcode, 31, 27);
    uint32_t fcbit = OperandExtract (opcode, 19, 17);

    uint32_t fpscr_cc = m_fpsr->GetCC ();
    bool ccbit = (fpscr_cc >> fcbit) & 0x01;

    uint32_t res;
    if (ccbit) {
        res = GRegRead (reg1);
    } else {
        res = GRegRead (reg2);
    }
    GRegWrite (reg3, res);
}


void InstV850::Inst_CMPFD (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2  = OperandExtract (opcode, 15, 12) << 1;
    uint32_t fcbit = OperandExtract (opcode, 19, 17);
    uint32_t fcond = OperandExtract (opcode, 30, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    FpuRegDouble v1 (GRegRead64 (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    bool r = m_fpu_ope->Cmp (v1, v2, fcond, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        SetFpsrXP (exp_code);
        uint32_t new_cc = (m_fpsr->GetCC () & ~(1 << fcbit)) | (r << fcbit);
        m_fpsr->SetCC (new_cc);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CMPFS (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t fcbit = OperandExtract (opcode, 19, 17);
    uint32_t fcond = OperandExtract (opcode, 30, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    bool r = m_fpu_ope->Cmp (v1, v2, fcond, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        SetFpsrXP (exp_code);
        uint32_t new_cc = (m_fpsr->GetCC () & ~(1 << fcbit)) | (r << fcbit);
        m_fpsr->SetCC (new_cc);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFDS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));

    FpuRegSingle res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res.SetFloat (m_fpu_ope->Cvt_DS (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFSD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));

    FpuRegDouble res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res.SetFloat (m_fpu_ope->Cvt_SD (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFDW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Cvt_F64_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFDUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Cvt_F64_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFDL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Cvt_F64_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFDUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Cvt_F64_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFSW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Cvt_F32_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFSUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Cvt_F32_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFSL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Cvt_F32_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFSUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Cvt_F32_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFWD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));

    FpuRegDouble res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    int32_t val = static_cast<int32_t> (v2.GetInteger());
    res = m_fpu_ope->Cvt_I32_F64 (val, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFUWD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));

    FpuRegDouble res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res = m_fpu_ope->Cvt_UI32_F64 (v2.GetInteger(), &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFLD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));

    FpuRegDouble res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    int64_t val = static_cast<int64_t> (v2.GetInteger());
    res = m_fpu_ope->Cvt_I64_F64 (val, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFULD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));

    FpuRegDouble res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res = m_fpu_ope->Cvt_UI64_F64 (v2.GetInteger(), &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFWS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    FpuRegSingle res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    int32_t val = static_cast<int32_t> (v2.GetInteger());
    res = m_fpu_ope->Cvt_I32_F32 (val, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFUWS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    FpuRegSingle res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res = m_fpu_ope->Cvt_UI32_F32 (v2.GetInteger(), &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFLS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));

    FpuRegSingle res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    int64_t val = static_cast<int64_t> (v2.GetInteger());
    res = m_fpu_ope->Cvt_I64_F32 (val, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFULS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));

    FpuRegSingle res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res = m_fpu_ope->Cvt_UI64_F32 (v2.GetInteger(), &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_DIVFD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    FpuRegDouble v1 (GRegRead64 (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Div (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_DIVFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Div (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFDW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Floor_F64_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFDUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Floor_F64_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFDL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Floor_F64_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFDUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Floor_F64_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFSW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Floor_F32_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFSUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Floor_F32_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFSL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Floor_F32_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FLOORFSUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Floor_F32_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_MAXFD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    FpuRegDouble v1 (GRegRead64 (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Max (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_MAXFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Max (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_MINFD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    FpuRegDouble v1 (GRegRead64 (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Min (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_MINFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Min (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_MULFD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    FpuRegDouble v1 (GRegRead64 (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Mul (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_MULFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Mul (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_NEGFD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Neg (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_NEGFS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Neg (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_RECIPFD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Recip (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_RECIPFS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Recip (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFDW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Round_F64_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFDUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Round_F64_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFDL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Round_F64_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFDUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Round_F64_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFSW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Round_F32_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFSUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Round_F32_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFSL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Round_F32_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_ROUNDFSUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Round_F32_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_RSQRTFD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Rsqrt (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_RSQRTFS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Rsqrt (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_SQRTFD (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Sqrt (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_SQRTFS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Sqrt (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_SUBFD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  1) << 1;
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    FpuRegDouble v1 (GRegRead64 (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegDouble v3 = m_fpu_ope->Sub (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_SUBFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v3 = m_fpu_ope->Sub (v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v3.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRFSR (uint64_t opcode)
{
    uint32_t fcbit = OperandExtract (opcode, 19, 17);

    uint32_t fpscr_cc = m_fpsr->GetCC ();
    bool ccbit = (fpscr_cc >> fcbit) & 0x01;

    SetPswZ (ccbit);

    // @@
    // ↓ 未検討
    // インプレサイス例外モードで浮動小数点演算例外が発生すると，
    // 例外が受け付けられるまでの間に後続の
    // 浮動小数点演算命令で例外を発生させないために，
    // 後続の浮動小数点演算命令の結果は反映されませんが，
    // TRFSR命令の結果は反映されます【ユーザ非公開】。
}


void InstV850::Inst_TRNCFDW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Trnc_F64_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRNCFDUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Trnc_F64_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRNCFDL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Trnc_F64_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRNCFDUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 12) << 1;
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegDouble v2 (GRegRead64 (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Trnc_F64_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuDoubleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRNCFSW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = static_cast<uint32_t> (m_fpu_ope->Trnc_F32_I32 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRNCFSUW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint32_t res = m_fpu_ope->Trnc_F32_UI32 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRNCFSL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = static_cast<uint64_t> (m_fpu_ope->Trnc_F32_I64 (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_TRNCFSUL (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;

    FpuRegSingle v2 (GRegRead (reg2));
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    uint64_t res = m_fpu_ope->Trnc_F32_UI64 (v2, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite64 (reg3, res);
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


// FPU-3.0 新規追加命令
void InstV850::Inst_FMAFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));
    FpuRegSingle v3 (GRegRead (reg3));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v4 = m_fpu_ope->Madd (v3, v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v4.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FMSFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));
    FpuRegSingle v3 (GRegRead (reg3));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v4 = m_fpu_ope->Msub (v3, v2, v1, &exp_code);

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v4.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FNMAFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));
    FpuRegSingle v3 (GRegRead (reg3));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v4 = m_fpu_ope->Madd (v3, v2, v1, &exp_code);
    uint32_t tmp_exp_code = exp_code;
    FpuRegSingle v5;
    // If any input is a SNAN, output will be QNAN[V] without do Mneg()
    if ((v4.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code  == FpuOpeBase::FPE_V)) {
        v5 = v4;
    } else {
        v5 = m_fpu_ope->Mneg (v4, &exp_code);
        exp_code |= tmp_exp_code;
    }

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v5.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_FNMSFS (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));
    FpuRegSingle v1 (GRegRead (reg1));
    FpuRegSingle v3 (GRegRead (reg3));

    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    FpuRegSingle v4 = m_fpu_ope->Msub (v3, v2, v1, &exp_code);
    uint32_t tmp_exp_code = exp_code;
    FpuRegSingle v5;
    // If any input is a SNAN, output will be QNAN[V] without do Mneg()
    if ((v4.GetInteger() == FpuRegSingle::FP_QNAN) && (exp_code  == FpuOpeBase::FPE_V)) {
        v5 = v4;
    } else {
        v5 = m_fpu_ope->Mneg (v4, &exp_code);
        exp_code |= tmp_exp_code;
    }

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, v5.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFHS (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegHalf v2 (static_cast<uint16_t>(GRegRead (reg2) & 0xffffU));

    FpuRegSingle res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res.SetInteger (m_fpu_ope->Cvt_HS (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


void InstV850::Inst_CVTFSH (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    FpuRegSingle v2 (GRegRead (reg2));

    FpuRegSingle res;
    uint32_t exp_code = FpuOpeBase::FPE_NONE;
    res.SetInteger (m_fpu_ope->Cvt_SH (v2, &exp_code));

    //浮動小数点例外判定
    if (IsFpuExceptionPermitted (exp_code)) {
        CauseFpuSingleException ();
        SetFpsrXC (exp_code);
    } else {
        if (exp_code != 0) {
            SetFpsrXP (exp_code);
        }
        GRegWrite (reg3, res.GetInteger());
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPSR, m_srf->SrFPSR()->ReadBody());
}


bool InstV850::IsFpuExceptionPermitted (uint32_t exp_code) const
{
    uint32_t xe = FpuOpeBase::FPE_E | m_srf->SrFPSR()->GetXE();
    uint32_t exp_factor = exp_code & xe;
    return (exp_factor != 0) ? true : false;
}


void InstV850::CauseFpuSingleException ()
{
    if (m_srf->SrFPSR()->GetPEM()) {
        m_trace->SetExpCode (EXP_FPE);
    } else {
        m_srf->SrFPEC()->SetFPIVD (1);
        m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPEC, m_srf->SrFPEC()->ReadBody());
    }
    m_srf->SrFPEPC()->WriteBody (m_grf->ReadPC ());
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPEPC, m_srf->SrFPEPC()->ReadBody());
}


void InstV850::CauseFpuDoubleException ()
{
    if (m_srf->SrFPSR()->GetPEM()) {
        m_trace->SetExpCode (EXP_FPE);
    } else {
        m_srf->SrFPEC()->SetFPIVD (1);
        m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPEC, m_srf->SrFPEC()->ReadBody());
    }
    m_srf->SrFPEPC()->WriteBody (m_grf->ReadPC ());
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_FPEPC, m_srf->SrFPEPC()->ReadBody());
}


void InstV850::SetFpsrXC (uint32_t exp_code)
{
    SRegFPSR* p_fpsr = m_srf->SrFPSR();
    p_fpsr->SetXC (exp_code & 0x3F);
}


void InstV850::SetFpsrXP (uint32_t exp_code)
{
    SRegFPSR* p_fpsr = m_srf->SrFPSR();
    p_fpsr->SetXP (p_fpsr->GetXP() | (exp_code & 0x1F));
}
