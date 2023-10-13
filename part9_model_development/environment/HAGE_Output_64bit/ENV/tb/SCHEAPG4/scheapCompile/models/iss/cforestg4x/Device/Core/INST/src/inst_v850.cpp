/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <stdlib.h>
#include <limits>
#include "./config.h"
#include "./forest_common.h"
#include "./inst_v850.h"
#include "./proc_element.h"
#include "./trace_info.h"
#include "./regid.h"
#include "./gregfile.h"
#include "./sregfile.h"
#include "./mpu.h"
#include "./lsu.h"
#include "./llsc.h"
#include "./fs_controler.h"
#include "./forest_utility.h"
#include "./micro_arch_db.h"
#include "./exp_info.h"
#include "./icache.h"
#include "./btb_base.h"
#include "./exp_message.h"
#include "./forest_internals.h"
#include "./forest_message.h"
#ifdef SUPPORT_CUBESUITE
#include "./cube_callback.h"
#endif // SUPPORT_CUBESUITE

void InstV850::Inst_0 (uint64_t opcode)
{
    // デコードに失敗して到達
    // RIE or UCPOP判定
    // コプロ命令領域 op[31:0]= xxxx x1xx xxxx xxx0 xxxx x111 111x xxxx
    // 48bit FXU   op[31:0]= xxxx xxxx xxx1 1101 0000 0111 101x xxxx
    uint32_t op26    = OperandExtract (opcode, 26, 26);
    uint32_t op25_23 = OperandExtract (opcode, 25, 23);
    uint32_t op16    = OperandExtract (opcode, 16, 16);
    uint32_t op10_5  = OperandExtract (opcode, 10,  5);
    uint32_t op20_5  = OperandExtract (opcode, 20, 5);

    ExpCode expcode;
    if (op20_5 == 0xe83d) {
        // for 48bit FXU opcode
        // Cause UCPOP1 or RIE.
        uint32_t cu = m_srf->SrVPSW()->GetCU ();
        expcode = ((cu & 0x2) == 0) ? EXP_UCPOP1 : EXP_RIE;
    } else if ((op26 == 1) && (op16 == 0) && (op10_5 == 0x3f)) {
        // Copro instrution field
        CoproType copro_type;
        switch (op25_23 & 0x7) {
        case 0x0: copro_type = COPRO_FPU; break;
        case 0x1:
            switch (OperandExtract (opcode, 22, 21) & 0x3) {
            case 0x0: copro_type = COPRO_FPU; break;
            case 0x1: copro_type = COPRO_FPU; break;
            case 0x2: copro_type = COPRO_SIMD; break; // FXU
            case 0x3: copro_type = COPRO_FPU; break;
            }
            break;
        case 0x2: copro_type = COPRO_NO; break; // Not implemented
        case 0x3: copro_type = COPRO_SIMD; break; // FXU
        case 0x4: copro_type = COPRO_NO; break; // Not implemented
        case 0x5: copro_type = COPRO_NO; break; // Not implemented
        case 0x6: copro_type = COPRO_NO; break; // Not implemented
        case 0x7: copro_type = COPRO_NO; break; // Not implemented
        }

        uint32_t cu = m_srf->SrVPSW()->GetCU ();
        switch (copro_type) {
        case COPRO_NO:   expcode = EXP_UCPOP2; break;
        case COPRO_FPU: expcode = ((cu & 0x1) == 0) ? EXP_UCPOP0 : EXP_RIE; break;
        case COPRO_SIMD: expcode = ((cu & 0x2) == 0) ? EXP_UCPOP1 : EXP_RIE; break;
        }
    } else {
        // 予約命令例外
        expcode = EXP_RIE;
    }
    m_trace->SetExpCode (expcode);
}


void InstV850::Inst_ADD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 + v1;

    GRegWrite (reg2, res);

    SetPswCY (IsCarryAdd (v2, v1, res));
    SetPswOV (IsOvflwAdd (v2, v1, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_ADD_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t v2    = GRegRead (reg2);
    uint32_t s_imm = SignExtend (imm5, 5);
    uint32_t res   = v2 + s_imm;

    GRegWrite (reg2, res);

    SetPswCY (IsCarryAdd (v2, s_imm, res));
    SetPswOV (IsOvflwAdd (v2, s_imm, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_ADD_I16 (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    uint32_t v1    = GRegRead (reg1);
    uint32_t s_imm = SignExtend (imm16, 16);
    uint32_t res   = v1 + s_imm;

    GRegWrite (reg2, res);

    SetPswCY (IsCarryAdd (v1, s_imm, res));
    SetPswOV (IsOvflwAdd (v1, s_imm, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_ADF (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t cc4  = OperandExtract (opcode, 20, 17);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 + v1;

    bool cy = IsCarryAdd (v2, v1, res);
    bool ov = IsOvflwAdd (v2, v1, res);

    if (IsPswCondMatch (cc4)) {
        // 実際は res = v1 + v2 + 1 を計算したい
        // Carry, Overflow計算をするために分割
        cy ^= IsCarryAdd (res, 1U, res + 1);
        ov ^= IsOvflwAdd (res, static_cast<uint32_t>(1), res + 1);
        res++;
    }

    GRegWrite (reg3, res);

    SetPswCY (cy);
    SetPswOV (ov);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_AND (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 & v1;

    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_ANDI (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 & imm16;

    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  (0);
    SetPswZ  (res == 0);
}


void InstV850::Inst_BCOND_D9 (uint64_t opcode)
{
    uint32_t cc4     = OperandExtract (opcode,  3,  0);
    uint32_t disp_lo = OperandExtract (opcode,  6,  4) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 15, 11) << 4;
    uint32_t disp    = disp_hi | disp_lo;
    uint32_t s_disp  = SignExtend (disp, 9);

    PhysAddr pc = GRegReadPC ();
    PhysAddr next_pc = pc + s_disp;

    bool is_taken = false;
    if (IsPswCondMatch (cc4)) {
        // 分岐成功
        GRegWritePC (next_pc);
        m_trace->SetBranchInfo (BRANCH);
        is_taken = true;
    } else {
        // 分岐失敗
    }

#ifdef USE_HYBRID_BP
    // BR以外なら予測
    if (cc4 != VAL_CC4_BR) {
        BpResult bp_result = m_btb->PredictBranchAndStudyBht
                                        (pc, is_taken, ((s_disp >> 31) == 0));
        m_trace->SetBpResult (bp_result);
    } else {
        // 無条件分岐なら常に予測HIT
        m_btb->PredictBranch ();
        m_trace->SetBpResult (BP_HIT);
    }
#else // USE_HYBRID_BP
    PhysAddr target_pc;
    if (is_taken) {
        target_pc = next_pc;
    } else {
        target_pc = pc + 2;
    }
    if (cc4 != VAL_CC4_BR) {
        BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc,
                target_pc, is_taken, ((s_disp >> 31) == 0), false);
        m_trace->SetBpResult (bp_result);
    } else {
        BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht
                                        (pc, target_pc, true, true, true);
        m_trace->SetBpResult (bp_result);
    }
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_BR (uint64_t opcode)
{
    uint32_t disp_lo = OperandExtract (opcode,  6,  4) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 15, 11) << 4;
    uint32_t disp    = disp_hi | disp_lo;
    uint32_t s_disp  = SignExtend (disp, 9);

    PhysAddr pc = GRegReadPC ();
    PhysAddr next_pc = pc + s_disp;

    // 無条件分岐
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // 無条件分岐なら常に予測HIT
    m_btb->PredictBranch ();
    m_trace->SetBpResult (BP_HIT);
    m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
#else // USE_HYBRID_BP
    PhysAddr target_pc;
    if (is_taken) {
        target_pc = next_pc;
    } else {
        target_pc = pc + 2;
    }
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc, target_pc, true, true, true);
    m_trace->SetBpResult (bp_result);
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_BCOND_D17 (uint64_t opcode)
{
    uint32_t cc4     = OperandExtract (opcode,  3,  0);
    uint32_t disp_lo = OperandExtract (opcode, 31, 17) << 1;
    uint32_t disp_hi = OperandExtract (opcode,  4,  4) << 16;
    uint32_t disp    = disp_hi | disp_lo;
    uint32_t s_disp  = SignExtend (disp, 17);

    if (cc4 == 0x05) {
        // Bcond disp17 では BR(0101)を指定禁止
        m_trace->SetExpCode (EXP_RIE);
        return;
    }

    PhysAddr pc = GRegReadPC ();
    PhysAddr next_pc = pc + s_disp;

    bool is_taken = false;
    if (IsPswCondMatch (cc4)) {
        // 分岐成功
        GRegWritePC (next_pc);
        m_trace->SetBranchInfo (BRANCH);
        is_taken = true;
    } else {
        // 分岐失敗
    }

#ifdef USE_HYBRID_BP
    // BR以外なら予測 (ただし、BCOND17のR条件は現在例外扱いなので発生しない）
    if (cc4 != VAL_CC4_BR) {
        BpResult bp_result = m_btb->PredictBranchAndStudyBht
                                        (pc, is_taken, ((s_disp >> 31) == 0));
        m_trace->SetBpResult (bp_result);
    } else {
        // 無条件分岐なら常に予測HIT
        m_btb->PredictBranch ();
        m_trace->SetBpResult (BP_HIT);
    }
#else // USE_HYBRID_BP
    PhysAddr target_pc;
    if (is_taken) {
        target_pc = next_pc;
    } else {
        target_pc = pc + 4;
    }
    if (cc4 != VAL_CC4_BR) {
        BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc,
                target_pc, is_taken, ((s_disp >> 31) == 0), false);
        m_trace->SetBpResult (bp_result);
    } else {
        BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht
                                        (pc, target_pc, true, true, true);
        m_trace->SetBpResult (bp_result);
    }
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_BINSL (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
    uint32_t reg2   = OperandExtract (opcode, 15, 11);
    uint32_t msb    = OperandExtract (opcode, 31, 28);
    uint32_t lsb_hi = OperandExtract (opcode, 27, 27) << 3;
    uint32_t lsb_lo = OperandExtract (opcode, 19, 17);
    uint32_t lsb    = lsb_hi | lsb_lo;

    Inst_BINS_Common (reg1, reg2, msb, lsb);
}


void InstV850::Inst_BINSM (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
    uint32_t reg2   = OperandExtract (opcode, 15, 11);
    uint32_t msb    = OperandExtract (opcode, 31, 28) | 0x0010U;
    uint32_t lsb_hi = OperandExtract (opcode, 27, 27) << 3;
    uint32_t lsb_lo = OperandExtract (opcode, 19, 17);
    uint32_t lsb    = lsb_hi | lsb_lo;

    Inst_BINS_Common (reg1, reg2, msb, lsb);
}


void InstV850::Inst_BINSU (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
    uint32_t reg2   = OperandExtract (opcode, 15, 11);
    uint32_t msb    = OperandExtract (opcode, 31, 28) | 0x0010U;
    uint32_t lsb_hi = OperandExtract (opcode, 27, 27) << 3;
    uint32_t lsb_lo = OperandExtract (opcode, 19, 17);
    uint32_t lsb    = lsb_hi | lsb_lo | 0x0010;

    Inst_BINS_Common (reg1, reg2, msb, lsb);
}


void InstV850::Inst_BSH (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2 = GRegRead (reg2);
    uint32_t b0 = v2 & 0x000000FFU;
    uint32_t b1 = v2 & 0x0000FF00U;
    uint32_t b2 = v2 & 0x00FF0000U;
    uint32_t b3 = v2 & 0xFF000000U;

    uint32_t res = (b2 << 8) | (b3 >> 8) | (b0 << 8) | (b1 >> 8);
    GRegWrite (reg3, res);

    SetPswCY ((b0 == 0x0) | (b1 == 0x0));
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  ((res & 0x0000FFFF) == 0x0);
}


void InstV850::Inst_BSW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2 = GRegRead (reg2);
    uint32_t b0 = v2 & 0x000000FFU;
    uint32_t b1 = v2 & 0x0000FF00U;
    uint32_t b2 = v2 & 0x00FF0000U;
    uint32_t b3 = v2 & 0xFF000000U;

    uint32_t res = (b0 << 24) | (b1 << 8) | (b2 >> 8) | (b3 >> 24);
    GRegWrite (reg3, res);

    SetPswCY ((b0 == 0x0) | (b1 == 0x0) | (b2 == 0x0) | (b3 == 0x0));
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0x0);
}


void InstV850::Inst_CACHE (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode, 4, 0);
    uint32_t opl  = OperandExtract (opcode, 31, 27);
    uint32_t oph  = OperandExtract (opcode, 12, 11) << 5;
    uint32_t op   = oph | opl;

    // Doesn't occur PIE when there is no cache.
    switch (op) {
    case Icache::OP_CHBII:
    case Icache::OP_CIBII:
    case Icache::OP_CFALI:
    case Icache::OP_CISTI:
    case Icache::OP_CILDI:
        if (m_fsctrl->m_pe->GetIcPtr()->IsIcacheSizeZero ()) {
            // Need to check PIE even if there is no cache.
        }
        break;
    default: break;
    }

    uint32_t  v1 = GRegRead (reg1);
    PrivLevel priv = m_trace-> GetPrivLevel();

    switch (op) {
    case Icache::OP_CLL   : CacheCLL (); break;
    case Icache::OP_CHBII : CacheCHBII (v1); break;

    case Icache::OP_CIBII :
    case Icache::OP_CFALI :
    case Icache::OP_CISTI :
    case Icache::OP_CILDI :
        if ((m_fsctrl->m_pe->IsHvEnabled () == true)  && (HasPrivileged (PRIV_PERMIT_HV, priv) == false)) {
            //! Need HV privilege if virtualization is enabled
            CfMsg::TPrint (MSG_INF, m_trace, "<Privilege Error: HV : cache>\n");
            m_trace->SetExpCode (EXP_PIE);
        } else if ((m_fsctrl->m_pe->IsHvEnabled () == false) && (HasPrivileged (PRIV_PERMIT_SV, priv) == false)) {
            //! Need SV privilege if virtualization is disabled
            CfMsg::TPrint (MSG_INF, m_trace, "<Privilege Error: SV : cache>\n");
            m_trace->SetExpCode (EXP_PIE);
        } else {
            switch (op) {
            case Icache::OP_CIBII : CacheCIBII (v1); break;
            case Icache::OP_CFALI : CacheCFALI (v1); break;
            case Icache::OP_CISTI : CacheCISTI (v1); break;
            case Icache::OP_CILDI : CacheCILDI (v1); break;
            }
        }
        break;
    default :
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: illegal CACHE-operation>\n");
    }
}


void InstV850::Inst_CALLT (uint64_t opcode)
{
    uint32_t imm6 = OperandExtract (opcode,  5,  0);

    PhysAddr pc   = GRegReadPC ();
    uint32_t psw  = SRegRead (CF_SR_VPSW);
    uint32_t ctbp = SRegRead (CF_SR_CTBP);

    PhysAddr addr = ctbp + (imm6 << 1);

#if defined(COMP_RUNNER)
    if (NUM_CALL_PRE_LSAB > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, R0, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t val = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);

#if defined(COMP_RUNNER)
    if (NUM_CALL_POST_LSAB > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)

    SRegWrite (CF_SR_CTPC, pc + 2);
    SRegWrite (CF_SR_CTPSW, psw);
    PhysAddr next_pc = ctbp + val;
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);
}


void InstV850::Inst_CAXI (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
#endif // SUPPORT_SC_HEAP
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitRmwBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    // caxi/ldl.w/stc.wではMCTL.MA=1でもMAEは抑止されないので、最初にMAEをチェック
    if (unlikely (m_lsu->IsMemExceptionAtomic<SIZE_32BIT> (addr) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionRMW (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_READ);
            return;
        }
        // Come here if MDP is suppressed.
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }

    uint32_t token;
    bool is_read_cancelled;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemReadBitOp (addr, SIZE_32BIT, SideBand::BITOP_CAXI);
        is_read_cancelled = m_trace->IsCancel ();
        if (is_read_cancelled) {
            // CForest実装依存により、PEGでの判定がReadとWrite別々に実施されてしまう。
            // Read不許可ならWrite側も不許可として実施しない
        }
    } else {
        token = 0;
        is_read_cancelled = true;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadBitOpMdp (addr, SIZE_32BIT, SideBand::BITOP_CAXI);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        // All register set as 0 if MDP supressed
        GRegWrite (reg3, 0);
        SetPswCY (0);
        SetPswOV (0);
        SetPswS  (0);
        SetPswZ  (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t v2  = GRegRead (reg2);
    uint32_t res = v2 - token;

    uint32_t val;
    if (res == 0) {
        val = GRegRead (reg3);
    } else {
        val = token;
    }

    // MemWrite is done at least Read is Enable
    bool is_write_cancelled = false;
    if (!is_read_cancelled) {
        m_lsu->MemWriteBitOp (addr, val, SIZE_32BIT, SideBand::BITOP_CAXI);
        is_write_cancelled = m_trace->IsCancel ();
        // In G3MH, Read data from FlashROM area by CAXI is not flashed.
        // otherwise, Read data is flashed into zero.
        if (!m_lsu->IsFlashROMArea (addr) &&
            is_write_cancelled) {
            token = 0x0;
        }
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else if (is_mdp_suppressed == true) {
        m_lsu->MemWriteBitOpMdp (addr, val, SIZE_32BIT, SideBand::BITOP_CAXI);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    if (is_mdp_suppressed == false) {
        GRegWrite (reg3, token);
        SetPswCY (IsCarrySub (v2, token));
        SetPswOV (IsOvflwSub (v2, token, res));
        SetPswS  ((res >> 31) & 0x01U);
        SetPswZ  (res == 0);
    } else {
        // All register set as 0 if MDP supressed
        GRegWrite (reg3, 0);
        SetPswCY (0);
        SetPswOV (0);
        SetPswS  (0);
        SetPswZ  (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_CAXI_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = m_trace->GetRregValue(0) & 0xFFFFFFFF;
    PhysAddr addr = v1;

    uint32_t token = read_data;
    uint32_t v2  = GRegRead (reg2);
    uint32_t res = v2 - token;

    uint32_t val;
    if (res == 0) {
        val = GRegRead (reg3);
    } else {
        val = token;
    }

    m_lsu->MemWriteBitOp (addr, val, SIZE_32BIT, SideBand::BITOP_CAXI);
    GRegWrite (reg3, token);

    SetPswCY (IsCarrySub (v2, token));
    SetPswOV (IsOvflwSub (v2, token, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_CLL (uint64_t opcode)
{
    // 自スレッドのLLbitクリア
    // L1RAM領域/その他領域のLLBitを削除
    CacheCLL ();
}


void InstV850::CacheCLL (void)
{
    m_lsu->DeleteOwnLink ();
}


void InstV850::Inst_CLR1_B3 (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t bit3   = OperandExtract (opcode, 13, 11);
#endif // SUPPORT_SC_HEAP
    uint32_t disp   = OperandExtract (opcode, 31, 16);
    uint32_t s_disp = SignExtend (disp, 16);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitRmwBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionRMW (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            // G3MH, G3KH, G4P special HW implementation
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemReadBitOp (addr, SIZE_8BIT, SideBand::BITOP_CLR);
        if (m_trace->IsCancel ()) {
            // CForest実装依存により、PEGでの判定がReadとWrite別々に実施されてしまう。
            // Read不許可ならWrite側も不許可として実施しない
            return;
        }
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadBitOpMdp (addr, SIZE_8BIT, SideBand::BITOP_CLR);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        // register set as 0 if MDP supressed
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t pos = bit3;
    bool z = (~(token >> pos)) & 0x01;
    token &= ~(1 << pos);

    if (is_mdp_suppressed == false) {
        m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_CLR);
        SetPswZ (z);
    } else {
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemWriteBitOpMdp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_CLR);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

        // register set as 0 if MDP supressed
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_CLR1_B3_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t bit3   = OperandExtract (opcode, 13, 11);
    uint32_t disp   = OperandExtract (opcode, 31, 16);
    uint32_t s_disp = SignExtend (disp, 16);

    uint32_t v1   = m_trace->GetRregValue(0) & 0xFFFFFFFF;
    PhysAddr addr = v1 + s_disp;

    uint32_t token = read_data;
    uint32_t pos = bit3;
    bool z = (~(token >> pos)) & 0x01;
    token &= ~(1 << pos);

    m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_CLR);
    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_CLR1 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
#endif // SUPPORT_SC_HEAP

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitRmwBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionRMW (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            // G3MH, G3KH, G4P special HW implementation
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemReadBitOp (addr, SIZE_8BIT, SideBand::BITOP_CLR);
        if (m_trace->IsCancel ()) {
            // CForest実装依存により、PEGでの判定がReadとWrite別々に実施されてしまう。
            // Read不許可ならWrite側も不許可として実施しない
            return;
        }
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadBitOpMdp (addr, SIZE_8BIT, SideBand::BITOP_CLR);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;
    token &= ~(1 << pos);

    if (is_mdp_suppressed == false) {
        m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_CLR);
        SetPswZ (z);
    } else {
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemWriteBitOpMdp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_CLR);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_CLR1_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v1   = m_trace->GetRregValue(0) & 0xFFFFFFFF;
    PhysAddr addr = v1;

    uint32_t token = read_data;
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;
    token &= ~(1 << pos);

    m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_CLR);
    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_CMOV_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t cc4  = OperandExtract (opcode, 20, 17);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t res;
    if (IsPswCondMatch (cc4)) {
        res = SignExtend (imm5, 5);
    } else {
        res = GRegRead (reg2);
    }

    GRegWrite (reg3, res);
}


void InstV850::Inst_CMOV (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t cc4  = OperandExtract (opcode, 20, 17);

    uint32_t res;
    if (IsPswCondMatch (cc4)) {
        res = GRegRead (reg1);
    } else {
        res = GRegRead (reg2);
    }

    GRegWrite (reg3, res);
}


void InstV850::Inst_CMP_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t v2    = GRegRead (reg2);
    uint32_t s_imm = SignExtend (imm5, 5);
    uint32_t res   = v2 - s_imm;

    SetPswCY (IsCarrySub (v2, s_imm));
    SetPswOV (IsOvflwSub (v2, s_imm, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_CMP (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 - v1;

    SetPswCY (IsCarrySub (v2, v1));
    SetPswOV (IsOvflwSub (v2, v1, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_CTRET (uint64_t opcode)
{
    uint32_t ctpc  = SRegRead (CF_SR_CTPC);
    uint32_t ctpsw = SRegRead (CF_SR_CTPSW);

    PhysAddr next_pc = ctpc & 0xFFFFFFFEU;

    SetPswSAT ((ctpsw >> 4) & 0x01);
    SetPswCY  ((ctpsw >> 3) & 0x01);
    SetPswOV  ((ctpsw >> 2) & 0x01);
    SetPswS   ((ctpsw >> 1) & 0x01);
    SetPswZ   ((ctpsw >> 0) & 0x01);

    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);
}


void InstV850::Inst_DBCP (uint64_t opcode)
{
    //! DBCP outputs info to debug I/F.
    //! Doesn't affect any to CPU internal.
    // PC can be understood from log. Doesn't need to output PC additionally.
#ifdef SUPPORT_CUBESUITE
    (*CubeCallBack::__NotifyTraceInst) (TRACE_DBCP);
#endif // SUPPORT_CUBESUITE
}


void InstV850::Inst_DBPUSH (uint64_t opcode)
{
#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)

    //! DBPUSH outputs info to debug I/F.
    //! Doesn't affect any to CPU internal.
    uint32_t rh = OperandExtract (opcode,  4,  0);
    uint32_t rt = OperandExtract (opcode, 31, 27);


    if (rh <= rt) {
        uint32_t cur = rh;
        uint32_t end = rt;
        while (cur <= end) {
#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return; // cancel by async event
            }
            ++ completed_count;
#endif //#if defined(COMP_RUNNER)
            GRegRead (cur);
            cur += 1;
        }
#if defined(COMP_RUNNER)
        // for last commit
        if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            m_trace->SetCancelMcyc (true);
            return; // cancel by async event
        }
#endif // #if defined(COMP_RUNNER)
    } else {
        // in case of rh > rt, DBPUSH is finished by 1 commit.
        // No need to cancel.
        // For G4MH, output data is 0, register is rt & 0x1E (bit0 is fixed to 0);
        GRegRead (rt & 0x1E);
    }
#ifdef SUPPORT_CUBESUITE
    (*CubeCallBack::__NotifyTraceInst) (TRACE_DBPUSH);
#endif // SUPPORT_CUBESUITE
}


void InstV850::Inst_DBRET (uint64_t opcode)
{
    uint32_t dbpc  = SRegRead (CF_SR_DBPC);
    uint32_t dbpsw = SRegRead (CF_SR_DBPSW);

    m_srf->SrDIR0()->SetDM (0); // To write PSW.UM and PSWH.GM, set DM to 0 at first.

    SRegWrite (CF_SR_VPSW, dbpsw);

    PhysAddr next_pc = dbpc & 0xFFFFFFFEU;
    GRegWritePC (next_pc);

    if (m_fsctrl->m_pe->IsHvEnabled () && m_fsctrl->m_pe->IsHostMode()) {
        uint32_t dbpswh = SRegRead (CF_SR_DBPSWH);
        SetPswh (dbpswh);
    }

    // Record DIR0.DM info as same as order of ISA operation
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_DIR0, m_srf->SrDIR0()->ReadBody());

    m_trace->SetBranchInfo (RTE_BRANCH);
}


void InstV850::Inst_DBTAG (uint64_t opcode)
{
    //! DBTAG outputs info to debug I/F.
    //! Doesn't affect any to CPU internal.
    // PC and tag can be understood from log. Doesn't need to output additionally.
#ifdef SUPPORT_CUBESUITE
    (*CubeCallBack::__NotifyTraceInst) (TRACE_DBTAG);
#endif // SUPPORT_CUBESUITE
}


void InstV850::Inst_DBTRAP (uint64_t opcode)
{
    PhysAddr ret_pc    = m_trace->GetInstAddr () + 2;
    uint32_t exp_cause = ExpInfo::GetExpCause (EXP_DBTRAP);
    uint32_t hndl_ofst = ExpInfo::GetHdlOffset (EXP_DBTRAP);
    bool     is_excp   = true;  // PSW.EP用: true = 例外

    m_trace->SetExpCode (EXP_DBTRAP);
    ExceptionDB (exp_cause, ret_pc, hndl_ofst, is_excp, EXP_DBTRAP);
    m_fsctrl->m_exp_msg->ExpMsgPrint (EXP_DBTRAP, exp_cause, m_trace);
}


void InstV850::Inst_DI (uint64_t opcode)
{
    //! DIはスーパバイザ特権命令だが、MCTL.UIC==1の時ユーザ･モードで実行可能
    if (m_psw->GetUM() == 1 && m_srf->SrMCTL()->GetUIC() == 0) {
        //! 特権命令例外
        CfMsg::TPrint (MSG_INF, m_trace, "<Privilege Error : SV : di>\n");
        m_trace->SetExpCode (EXP_PIE);
        return;
    }

    SetPswID (1);
}


void InstV850::Inst_DISPOSE (uint64_t opcode)
{
    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;
    uint32_t imm5   = OperandExtract (opcode,  5,  1);

    bool exception = false;
    RegData sp_val = DisposeStack (list12, imm5, &exception);
    if (exception == false) {
        GRegWrite (R_SP, sp_val);
    }
}


void InstV850::Inst_DISPOSE_R (uint64_t opcode)
{
    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;
    uint32_t reg1   = OperandExtract (opcode, 20, 16);
    uint32_t imm5   = OperandExtract (opcode,  5,  1);

    bool exception = false;
    RegData sp_val = DisposeStack (list12, imm5, &exception);
    if (exception == true) {
        return;
    }

    // reg1==SP の場合は更新前のSP値がPCに代入される
    PhysAddr next_pc = GRegRead (reg1);
    GRegWrite (R_SP, sp_val);

    PhysAddr curr_pc = m_grf->ReadPC (); // 履歴に表示したくないので、直接アクセス

    if ((next_pc & 0x01) != 0) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: R%d value is odd>\n", reg1);
        next_pc &= ~0x01;
    }
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    BpResult bp_result;
    if (reg1 == R31) {
        // R31の場合はRAS予測
        bp_result = m_btb->PredictBranchAndPopRas (next_pc);
    } else {
        // R31以外ではBTAC予測
        bp_result = m_btb->PredictBranchAndStudyBtac (curr_pc, next_pc);
    }
    m_trace->SetBpResult (bp_result);
    if (bp_result == BP_MISS) {
        m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
    }
#else // USE_HYBRID_BP
    BpResult bp_result =
            m_btb->PredictBranchAndStudyBtacBht (curr_pc, next_pc, true, (reg1 != R31), true);
    m_trace->SetBpResult (bp_result);
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_DIV     (uint64_t opc) { ExecDIV<int32_t>  (opc, false, false); }
void InstV850::Inst_DIVU    (uint64_t opc) { ExecDIV<uint32_t> (opc, false, false); }
void InstV850::Inst_DIVH    (uint64_t opc) { ExecDIV<int32_t>  (opc, true,  false); }
void InstV850::Inst_DIVHU   (uint64_t opc) { ExecDIV<uint32_t> (opc, true,  false); }
void InstV850::Inst_DIVQ    (uint64_t opc) { ExecDIV<int32_t>  (opc, false, false); }
void InstV850::Inst_DIVQU   (uint64_t opc) { ExecDIV<uint32_t> (opc, false, false); }
void InstV850::Inst_DIVH_R2 (uint64_t opc) { ExecDIV<int32_t>  (opc, true,  true);  }


template<class TInt>
void InstV850::ExecDIV (uint64_t opcode, bool divh_flag, bool op2_flag)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    TInt v1;
    TInt v2 = static_cast<TInt> (GRegRead (reg2));
    uint32_t t_v1 = GRegRead (reg1);
    if (std::numeric_limits<TInt>::is_signed) {
        if (divh_flag) { t_v1 = SignExtend (t_v1 & 0x0000FFFF, 16); }
        v1 = static_cast<TInt> (t_v1);
    } else {
        if (divh_flag) { t_v1 &= 0x0000FFFF; }
        v1 = t_v1;
    }

    if (likely (v1 != 0x0)) {
        bool ov = false;
        if (std::numeric_limits<TInt>::is_signed) {
            ov = ((static_cast<uint32_t>(v2) == 0x80000000U) && (static_cast<int32_t>(v1) == -1));
        }

        uint32_t div;
        uint32_t mod;
        if (likely (ov == false)) {
            div = static_cast<uint32_t> (v2 / v1);
            mod = static_cast<uint32_t> (v2 % v1);
        } else {
            div = 0x80000000U;
            mod = 0x00000000U;
        }

        if (op2_flag == true) {
            GRegWrite (reg2, div);
        } else {
            GRegWrite (reg2, div);
            GRegWrite (reg3, mod);
        }

        SetPswOV (ov);
        SetPswS  ((div >> 31) & 0x01U);
        SetPswZ  (div == 0);

    } else {
        // div by 0
        // Quotient(reg2)=keep previous
        // remainder(reg3)=0
        // PSW.OV=1, PSW.S=0, PSW.Z=0
        GRegWrite (reg2, v2);
        if (op2_flag == false) {
            // "DIVH reg1, reg2" doesn't have reg3
            // div instructions update reg3 except divh(r2).
            GRegWrite (reg3, 0x00000000);
        }

        SetPswS (0);
        SetPswZ (0);
        SetPswOV (1);
    }
}


void InstV850::Inst_EI (uint64_t opcode)
{
    //! EIはスーパバイザ特権命令だが、MCTL.UIC==1の時ユーザ･モードで実行可能
    if (m_psw->GetUM() == 1 && m_srf->SrMCTL()->GetUIC() == 0) {
        //! 特権命令例外
        CfMsg::TPrint (MSG_INF, m_trace, "<Privilege Error : SV : ei>\n");
        m_trace->SetExpCode (EXP_PIE);
        return;
    }

    SetPswID (0);
}


void InstV850::Inst_EIRET (uint64_t opcode)
{
    uint32_t eipc  = SRegRead (CF_SR_VEIPC);
    uint32_t eipsw = SRegRead (CF_SR_VEIPSW);

    // @@ EP=0の場合、外部(割込みコントローラなど)に通知
    if (m_srf->SrVINTCFG()->GetEPL() == 0) {
        if (m_srf->SrVPSW()->GetEP() == 0) {
            if (m_srf->SrVINTCFG()->GetISPC() == 0) {
                uint32_t pri = m_fsctrl->GetIsprHighPriority();
                RegData val = m_srf->SrISPR()->GetISP();
                val &= ~(0x1<<pri);
                //最高優先度のビットを0クリア
                m_srf->SrISPR()->SetISP(val);
                m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_ISPR, m_srf->SrISPR()->ReadBody());
            }
        }
    }

    SRegWrite (CF_SR_VPSW, eipsw);

    PhysAddr next_pc = eipc & 0xFFFFFFFEU;
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (RTE_BRANCH);

    if (m_fsctrl->m_pe->IsHvEnabled () && m_fsctrl->m_pe->IsHostMode()) {
        uint32_t eipswh = SRegRead (CF_SR_EIPSWH);
        if (m_srf->SrDIR0()->GetDM () == 1) {
            // In DB mode, can't update PSW.UM or PSWH.GM. They are kept to 0 except DBRET.
            eipswh &= 0x7FFFFFFFU;
        }
        SetPswh (eipswh);
    }

    // L1RAM領域/その他領域のLLBitを削除
    m_lsu->DeleteOwnLink ();
}


void InstV850::Inst_FERET (uint64_t opcode)
{
    uint32_t fepc  = SRegRead (CF_SR_VFEPC);
    uint32_t fepsw = SRegRead (CF_SR_VFEPSW);

    SRegWrite (CF_SR_VPSW, fepsw);

    PhysAddr next_pc = fepc & 0xFFFFFFFEU;
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (RTE_BRANCH);

    if (m_fsctrl->m_pe->IsHvEnabled () && m_fsctrl->m_pe->IsHostMode()) {
        uint32_t fepswh = SRegRead (CF_SR_FEPSWH);
        if (m_srf->SrDIR0()->GetDM () == 1) {
            // In DB mode, can't update PSW.UM or PSWH.GM. They are kept to 0 except DBRET.
            fepswh &= 0x7FFFFFFFU;
        }
        SetPswh (fepswh);
    }

    // L1RAM領域/その他領域のLLBitを削除
    m_lsu->DeleteOwnLink ();
}


void InstV850::Inst_FETRAP (uint64_t opcode)
{
    uint32_t vec4 = OperandExtract (opcode, 14, 11);

    // vec4は0以外
    // FETRAP要因コードのベースが 31H なので、
    // vec4を-1した値をベクタとして渡す
    PhysAddr ret_pc    = m_trace->GetInstAddr () + 2;
    uint32_t exp_cause = ExpInfo::GetExpCause (EXP_FETRAP) + vec4 - 1;
    uint32_t hndl_ofst = ExpInfo::GetHdlOffset (EXP_FETRAP);
    bool     is_excp   = true;  // PSW.EP用: true = 例外

    m_trace->SetExpCode (EXP_FETRAP);
    ExceptionFE (exp_cause, ret_pc, hndl_ofst, is_excp);

    m_fsctrl->m_exp_msg->ExpMsgPrint (EXP_FETRAP, exp_cause, m_trace);
}


void InstV850::Inst_HALT (uint64_t opcode)
{
    if ((m_fsctrl->IsAnyAsyncException () == false) && (m_srf->SrDIR0()->GetDM () == 0)) {
        // @@ TODO : Set Halt state
        m_fsctrl->SetHaltState (true);
    }
}


void InstV850::Inst_HSH (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t h0  = v2 & 0x0000FFFFU;
    uint32_t res = v2;
    GRegWrite (reg3, v2);

    SetPswCY (h0 == 0x0);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (h0 == 0x0);
}


void InstV850::Inst_HSW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t h0  = v2 & 0x0000FFFFU;
    uint32_t h1  = v2 & 0xFFFF0000U;
    uint32_t res = (h0 << 16) | (h1 >> 16);
    GRegWrite (reg3, res);

    SetPswCY ((h0 == 0x0) | (h1 == 0x0));
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0x0);
}


void InstV850::SetupGsrVec (void)
{
    // Setup register list for LDM.GSR and STM.GSR
    m_gsr_vec.push_back (CF_SR_FPSR);
    m_gsr_vec.push_back (CF_SR_FPEPC);
    m_gsr_vec.push_back (CF_SR_CTPC);
    m_gsr_vec.push_back (CF_SR_CTPSW);
    m_gsr_vec.push_back (CF_SR_CTBP);
    m_gsr_vec.push_back (CF_SR_MCTL);
    m_gsr_vec.push_back (CF_SR_SCCFG);
    m_gsr_vec.push_back (CF_SR_SCBP);
    m_gsr_vec.push_back (CF_SR_RBCR0);
    m_gsr_vec.push_back (CF_SR_RBCR1);
    m_gsr_vec.push_back (CF_SR_RBNR);
    m_gsr_vec.push_back (CF_SR_RBIP);
    m_gsr_vec.push_back (CF_SR_MCA);
    m_gsr_vec.push_back (CF_SR_MCS);
    m_gsr_vec.push_back (CF_SR_MCR);
    m_gsr_vec.push_back (CF_SR_MCI);
    m_gsr_vec.push_back (CF_SR_MPIDX);
    m_gsr_vec.push_back (CF_SR_GMEIPC);
    m_gsr_vec.push_back (CF_SR_GMEIPSW);
    m_gsr_vec.push_back (CF_SR_GMFEPC);
    m_gsr_vec.push_back (CF_SR_GMFEPSW);
    m_gsr_vec.push_back (CF_SR_GMPSW);
    m_gsr_vec.push_back (CF_SR_GMMEA);
    m_gsr_vec.push_back (CF_SR_GMMEI);
    m_gsr_vec.push_back (CF_SR_GMEIIC);
    m_gsr_vec.push_back (CF_SR_GMFEIC);
    m_gsr_vec.push_back (CF_SR_GMSPID);
    m_gsr_vec.push_back (CF_SR_GMSPIDLIST);
    m_gsr_vec.push_back (CF_SR_GMEBASE);
    m_gsr_vec.push_back (CF_SR_GMINTBP);
    m_gsr_vec.push_back (CF_SR_GMINTCFG);
    m_gsr_vec.push_back (CF_SR_GMPLMR);
    m_gsr_vec.push_back (CF_SR_GMSVLOCK);
    m_gsr_vec.push_back (CF_SR_GMMPM);
    m_gsr_vec.push_back (CF_SR_GMEIWR);
    m_gsr_vec.push_back (CF_SR_GMFEWR);
}


bool InstV850::StmBody (PhysAddr addr, CfSReg reg, IsMdpCheck is_check_mdp)
{
    PhysAddr mask = ~(PhysAddr(0x03));

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr & mask, SIZE_32BIT) == true)) {
        // Set erro code to trace in IsHitWriteBrk()
        return false;
    }

    // check AE exception by original (none-masked) addr
    // In STM.MP:
    //     For access of MPLAn, need to check AE.
    //     For access of MPUA/MPAT, doesn't check AE.
    //     However,
    //     If AE is deteced, it is detected at 1st MPLA transfer always.
    //     No need special handling to skip AE for MPUA and MPAT. (= can check always)
    if (unlikely (m_lsu->IsAlignmentError (addr) == true)) {
        m_srf->SrDIR0()->SetAT (1);
        return false;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (is_check_mdp == MDP_CHECK_NEED) {
        if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr & mask, SIZE_32BIT) == true)) {
            if (m_trace->GetExpCode () != EXP_NONE) {
                SetMemErrorInfo (addr & mask, R0, SIZE_32BIT, MA_SIGN, DATA_WRITE);
                return false;
            }
            is_mdp_suppressed = true;
            m_lsu->DeleteOwnLink (addr & mask, SIZE_32BIT);
        }
    }

    uint32_t data;
    if (reg != CF_SR_DUMMY) {
        data = SRegRead (reg);
    } else {
        data = 0; // dummy transfer
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr & mask, data, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr & mask, data, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

#ifdef ENABLE_DBG_BREAK
    // Apply EO bit because access is done
    m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
    return true;
}


bool InstV850::LdmBody (PhysAddr addr, CfSReg reg, IsMdpCheck is_check_mdp, bool is_update_sreg)
{
    PhysAddr mask = ~(PhysAddr(0x03));

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr & mask, SIZE_32BIT) == true)) {
        // Set exp code to trace in IsHitReadBrk ().
        return false;
    }

    // check AE exception by original (none-masked) addr
    // In LDM.MP:
    //     For access of MPLAn, need to check AE.
    //     For access of MPUA/MPAT, doesn't check AE.
    //     However,
    //     If AE is deteced, it is detected at 1st MPLA transfer always.
    //     No need special handling to skip AE for MPUA and MPAT. (= can check always)
    if (unlikely (m_lsu->IsAlignmentError (addr) == true)) {
        m_srf->SrDIR0()->SetAT (1);
        return false;
    }
#endif // ENABLE_DBG_BREAK

    // In case of LDM.MP, need to check MDP
    if (is_check_mdp == MDP_CHECK_NEED) {
        if (unlikely (m_lsu->IsMemException<DATA_READ> (addr & mask, SIZE_32BIT) == true)) {
            SetMemErrorInfo (addr & mask, R0, SIZE_32BIT, MA_SIGN, DATA_READ);
            return false;
        }
    }

    uint32_t res = m_lsu->MemRead (addr & mask, SIZE_32BIT, Lsu::SPCLD_ON);

    if ((is_update_sreg) && (reg != CF_SR_DUMMY)) {
        SRegWrite (reg, res);
    }

#ifdef ENABLE_DBG_BREAK
    // Apply EO bit because access is done
    m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK

    return true;
}


void InstV850::Inst_STM_GSR (uint64_t opcode)
{
    if (m_fsctrl->m_pe->IsHvEnabled () == false) {
        // If HVE=0, RIE occurs
        m_trace->SetExpCode (EXP_RIE);
        if ( (m_fsctrl->m_pe->GetCoreType () != CORE_G4MH) || (m_fsctrl->m_pe->GetCoreVersion () < CORE_VER_20)) {
            // There is no STM in G4MH1.x
            // Overwrite trace to invalid inst_id
            m_trace->SetInstId (INST_ID_V_0);
        }
        return;
    }
    if (IsPieException (PRIV_PERMIT_HV)) {
        m_trace->SetExpCode (EXP_PIE);
        return;
    }

    uint32_t reg1 = OperandExtract (opcode, 4, 0);
    uint32_t v1  = GRegRead (reg1);

    PhysAddr addr = v1;

#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)

    std::vector<CfSReg>::const_iterator it = m_gsr_vec.begin ();
    while (it != m_gsr_vec.end ()) {

#if defined(COMP_RUNNER)
         if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
             m_trace->SetCancelMcyc (true);
             return ; // cancel by async event
         }
#endif //#if defined(COMP_RUNNER)

        if (StmBody (addr, *it, MDP_CHECK_NEED) == false) {
            break; // Exception occur
        }
        addr += 0x4U;
        ++ it;
#if defined(COMP_RUNNER)
         ++ completed_count;
#endif //#if defined(COMP_RUNNER)
    }
}


void InstV850::Inst_LDM_GSR (uint64_t opcode)
{
    if (m_fsctrl->m_pe->IsHvEnabled () == false) {
        // If HVE=0, RIE occurs
        m_trace->SetExpCode (EXP_RIE);
        if ( (m_fsctrl->m_pe->GetCoreType () != CORE_G4MH) || (m_fsctrl->m_pe->GetCoreVersion () < CORE_VER_20)) {
            // There is no LDM in G4MH1.x
            // Overwrite trace to invalid inst_id
            m_trace->SetInstId (INST_ID_V_0);
        }
        return;
    }
    if (IsPieException (PRIV_PERMIT_HV)) {
        m_trace->SetExpCode (EXP_PIE);
        return;
    }

    uint32_t reg1 = OperandExtract (opcode, 4, 0);
    uint32_t v1  = GRegRead (reg1);

    PhysAddr addr = v1;

#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)

    std::vector<CfSReg>::const_iterator it = m_gsr_vec.begin ();
    while (it != m_gsr_vec.end ()) {

#if defined(COMP_RUNNER)
         if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
             m_trace->SetCancelMcyc (true);
             return ; // cancel by async event
         }
#endif //#if defined(COMP_RUNNER)

        if (LdmBody (addr, *it, MDP_CHECK_NEED, true) == false) {
            break; // Exception occur
        }
        addr += 0x4U;
        ++ it;
#if defined(COMP_RUNNER)
         ++ completed_count;
#endif //#if defined(COMP_RUNNER)
    }
#if defined(COMP_RUNNER)
    //LDM.GSR includes one last commit for sync
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return ; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)
}


void InstV850::Inst_STM_MP (uint64_t opcode)
{
    if ( (m_fsctrl->m_pe->GetCoreType () != CORE_G4MH) || (m_fsctrl->m_pe->GetCoreVersion () < CORE_VER_20)) {
        // There is no STM in G4MH1.x
        // Overwrite trace to invalid inst_id
        m_trace->SetExpCode (EXP_RIE);
        m_trace->SetInstId (INST_ID_V_0);
        return;
    }

    if (IsPieException (PRIV_PERMIT_SV)) {
        m_trace->SetExpCode (EXP_PIE);
        return;
    }

    uint32_t reg1 = OperandExtract (opcode, 4, 0);
    uint32_t eh   = OperandExtract (opcode, 15, 11);
    uint32_t et   = OperandExtract (opcode, 31, 27);

    uint32_t v1  = GRegRead (reg1);
    PhysAddr addr = v1;

#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)

    if (m_psw->GetUM() == 0) { // UM is false
        while (eh <= et) {
#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return ; // cancel by async event
            }
#endif //#if defined(COMP_RUNNER)
            // MPLA
            CfSReg mpla = static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPLA0) + eh);
            if (StmBody(addr, mpla, MDP_CHECK_NEED) == false) {
                break; // Exception occur
            }
            addr += 0x4U;

#if defined(COMP_RUNNER)
            ++ completed_count;
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return ; // cancel by async event
            }
#endif //#if defined(COMP_RUNNER)

            // MPUA
            CfSReg mpua = static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPUA0) + eh);
            if (StmBody(addr, mpua, MDP_CHECK_NEED) == false) {
                break; // Exception occur
            }
            addr += 0x4U;

#if defined(COMP_RUNNER)
            ++ completed_count;
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return ; // cancel by async event
            }
#endif //#if defined(COMP_RUNNER)

            // MPAT
            CfSReg mpat = static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPAT0) + eh);
            if (StmBody(addr, mpat, MDP_CHECK_NEED) == false) {
                break; // Exception occur
            }
            addr += 0x4U;
            ++eh;
#if defined(COMP_RUNNER)
            ++ completed_count;
#endif //#if defined(COMP_RUNNER)
        }
#if defined(COMP_RUNNER)
        //STM.MP includes one last commit for sync
        if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            m_trace->SetCancelMcyc (true);
            return ; // cancel by async event
        }
#endif //#if defined(COMP_RUNNER)
    } else {
        m_trace->SetExpCode(EXP_PIE);
    }
}

void InstV850::Inst_LDM_MP (uint64_t opcode)
{
    if ( (m_fsctrl->m_pe->GetCoreType () != CORE_G4MH) || (m_fsctrl->m_pe->GetCoreVersion () < CORE_VER_20)) {
        // There is no LDM in G4MH1.x
        // Overwrite trace to invalid inst_id
        m_trace->SetExpCode (EXP_RIE);
        m_trace->SetInstId (INST_ID_V_0);
        return;
    }

    if (IsPieException (PRIV_PERMIT_SV)) {
        m_trace->SetExpCode (EXP_PIE);
        return;
    }

    uint32_t reg1 = OperandExtract (opcode, 4, 0);
    uint32_t eh   = OperandExtract (opcode, 15, 11);
    uint32_t et   = OperandExtract (opcode, 31, 27);

    uint32_t v1  = GRegRead (reg1);
    PhysAddr addr = v1;

#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)

    if (m_psw->GetUM() == 0) { // UM is false

        // Guest mode can't write to MPAT/MPUA/MPLA registers
        uint32_t host_entry_start_id = m_srf->SrMPCFG ()->GetHBE ();
        bool can_write_host_entry;
        if (   (m_fsctrl->m_pe->IsHvEnabled () == false)
            || (HasPrivileged (PRIV_PERMIT_HV, m_trace->GetPrivLevel ()) == true)) {
            // Current mode can write to host entry
            can_write_host_entry = true;
        } else {
            // Current mode can't write to host entry
            can_write_host_entry = false;
        }

        while (eh <= et) {

            bool update_entry;
            if ((eh >= host_entry_start_id) && (can_write_host_entry == false)) {
                update_entry = false;
            } else {
                update_entry = true;
            }

#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return ; // cancel by async event
            }
#endif //#if defined(COMP_RUNNER)

            // MPLA
            CfSReg mpla = static_cast<CfSReg>(static_cast <uint32_t> (CF_SR_MPLA0) + eh);
            if (LdmBody (addr, mpla, MDP_CHECK_NEED, update_entry) == false) {
                break; // Exception occur
            }
            addr += 0x4U;

#if defined(COMP_RUNNER)
            ++ completed_count;
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return ; // cancel by async event
            }
#endif //#if defined(COMP_RUNNER)

            // MPUA
            CfSReg mpua = static_cast<CfSReg>(static_cast <uint32_t> (CF_SR_MPUA0) + eh);
            if (LdmBody (addr, mpua, MDP_CHECK_NEED, update_entry) == false) {
                break; // Exception occur
            }
            addr += 0x4U;

#if defined(COMP_RUNNER)
            ++ completed_count;
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return ; // cancel by async event
            }
#endif //#if defined(COMP_RUNNER)

            // MPAT
            CfSReg mpat = static_cast<CfSReg>(static_cast <uint32_t> (CF_SR_MPAT0) + eh);
            if (LdmBody (addr, mpat, MDP_CHECK_NEED, update_entry) == false) {
                break; // Exception occur
            }
            addr += 0x4U;
            ++ eh;
#if defined(COMP_RUNNER)
            ++ completed_count;
#endif //#if defined(COMP_RUNNER)
        }
#if defined(COMP_RUNNER)
        //LDM.MP includes one last commit for sync
        if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            m_trace->SetCancelMcyc (true);
            return ; // cancel by async event
        }
#endif //#if defined(COMP_RUNNER)
    } else {
        m_trace->SetExpCode(EXP_PIE);
    }
}


bool InstV850::IsPieException (PrivLevel inst_priv) const
{
    PrivLevel current_priv = m_trace->GetPrivLevel ();
    if (HasPrivileged (inst_priv, current_priv) == false) {
        CfMsg::TPrint (MSG_INF, m_trace, "<Privilege Error: %s : current mode = %s>\n",
                       GetPrivName (inst_priv).c_str(), GetPrivName (current_priv).c_str());
        return true;
    }
    return false;
}


void InstV850::Inst_HVTRAP (uint64_t opcode)
{
    uint32_t vec5 = OperandExtract (opcode,  4,  0);

    if (m_fsctrl->m_pe->IsHvEnabled () == false) {
        // If HVE=0, RIE occurs
        m_trace->SetExpCode (EXP_RIE);
        if ( (m_fsctrl->m_pe->GetCoreType () != CORE_G4MH) || (m_fsctrl->m_pe->GetCoreVersion () < CORE_VER_20)) {
            // There is no HVTRAP in G4MH1.x
            // Overwrite trace to invalid inst_id
            m_trace->SetInstId (INST_ID_V_0);
        }
        return;
    }
    if (IsPieException (PRIV_PERMIT_SV)) {
        m_trace->SetExpCode (EXP_PIE);
        return;
    }

    PhysAddr ret_pc    = m_trace->GetInstAddr () + 4;
    uint32_t exp_cause = ExpInfo::GetExpCause (EXP_HVTRAP) + vec5;
    uint32_t hndl_ofst = ExpInfo::GetHdlOffset (EXP_HVTRAP);
    bool     is_excp   = true;  // PSW.EP用: true = 例外
    bool     is_table  = false; // 例外ハンドラ仕様 (false:標準 true:テーブル)

    m_trace->SetExpCode (EXP_HVTRAP);
    ExceptionEI (exp_cause, ret_pc, hndl_ofst, is_excp, is_table);
    m_fsctrl->m_exp_msg->ExpMsgPrint (EXP_HVTRAP, exp_cause, m_trace);
}


void InstV850::Inst_JARL (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    PhysAddr pc = GRegReadPC ();
    uint32_t v1 = GRegRead (reg1);

    PhysAddr next_pc = v1;
    if ((next_pc & 0x01) != 0) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: R%d value is odd>\n", reg1);
        next_pc &= ~0x01;
    }

    GRegWrite (reg3, pc + 4);
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // BTAC予測
    BpResult bp_result = m_btb->PredictBranchAndStudyBtac (pc, next_pc);
    m_trace->SetBpResult (bp_result);

    // RAS学習
    if (reg3 == R31) {
        m_btb->StudyRas (pc + 4);
    }
    if (bp_result == BP_MISS) {
        m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
    }
#else // USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc, next_pc, true, true, true);
    m_trace->SetBpResult (bp_result);

    // RAS学習
    if (reg3 == R31) {
        m_btb->StudyRas (pc + 4);
    }
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_JARL_D22 (uint64_t opcode)
{
    uint32_t reg2    = OperandExtract (opcode, 15, 11);
    uint32_t disp_lo = OperandExtract (opcode, 31, 17) << 1;
    uint32_t disp_hi = OperandExtract (opcode,  5,  0) << 16;
    uint32_t disp    = disp_hi | disp_lo;
    uint32_t s_disp  = SignExtend (disp, 22);

    PhysAddr pc = GRegReadPC ();
    PhysAddr next_pc = pc + s_disp;

    GRegWrite (reg2, pc + 4);
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // RAS学習
    if (reg2 == R31) {
        m_btb->StudyRas (pc + 4);
    }

    // 無条件分岐（レジスタ間接を除く)なら常に予測HIT
    m_btb->PredictBranch ();
    m_trace->SetBpResult (BP_HIT);
    m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
#else // USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc, next_pc, true, true, true);
    m_trace->SetBpResult (bp_result);

    // RAS学習
    if (reg2 == R31) {
        m_btb->StudyRas (pc + 4);
    }
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_JARL_D32 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t disp_lo = OperandExtract (opcode, 31, 17) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 16;
    uint32_t disp    = disp_hi | disp_lo;

    PhysAddr pc = GRegReadPC ();
    PhysAddr next_pc = pc + disp;

    GRegWrite (reg1, pc + 6);
    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // RAS学習
    if (reg1 == R31) {
        m_btb->StudyRas (pc + 6);
    }

    // 無条件分岐（レジスタ間接を除く)なら常に予測HIT
    m_btb->PredictBranch ();
    m_trace->SetBpResult (BP_HIT);
    m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
#else // USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc, next_pc, true, true, true);
    m_trace->SetBpResult (bp_result);

    // RAS学習
    if (reg1 == R31) {
        m_btb->StudyRas (pc + 6);
    }
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_JMP (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);

    PhysAddr next_pc = GRegRead (reg1);

    if ((next_pc & 0x01) != 0) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: R%d value is odd>\n", reg1);
        next_pc &= ~0x01;
    }

    PhysAddr curr_pc = m_grf->ReadPC (); // 履歴に表示したくないので、直接アクセス

    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    BpResult bp_result;
    if (reg1 == R31) {
        // R31の場合はRAS予測
        bp_result = m_btb->PredictBranchAndPopRas (next_pc);
    } else {
        // R31以外ではBTAC予測
        bp_result = m_btb->PredictBranchAndStudyBtac (curr_pc, next_pc);
    }
    m_trace->SetBpResult (bp_result);
    if (bp_result == BP_MISS) {
        m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
    }
#else // USE_HYBRID_BP
    BpResult bp_result =
            m_btb->PredictBranchAndStudyBtacBht (curr_pc, next_pc, true, (reg1 != R31), true);
    m_trace->SetBpResult (bp_result);
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_JMP_D32 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t disp_lo = OperandExtract (opcode, 31, 17) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 16;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1      = GRegRead (reg1);
    PhysAddr next_pc = v1 + disp;

    if ((next_pc & 0x01) != 0) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: R%d value is odd>\n", reg1);
        next_pc &= ~0x01;
    }

    PhysAddr curr_pc = m_grf->ReadPC (); // 履歴に表示したくないので、直接アクセス

    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // BTAC予測
    BpResult bp_result = m_btb->PredictBranchAndStudyBtac (curr_pc, next_pc);
    m_trace->SetBpResult (bp_result);
    if (bp_result == BP_MISS) {
        m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
    }
#else // USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (curr_pc, next_pc, true, true, true);
    m_trace->SetBpResult (bp_result);
#endif // USE_HYBRID_BP
}


void InstV850::Inst_JR_D22 (uint64_t opcode)
{
    uint32_t disp_lo = OperandExtract (opcode, 31, 17) << 1;
    uint32_t disp_hi = OperandExtract (opcode,  5,  0) << 16;
    uint32_t disp    = disp_hi | disp_lo;

    PhysAddr pc = GRegReadPC ();
    uint32_t s_disp  = SignExtend (disp, 22);
    PhysAddr next_pc = pc + s_disp;

    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // 無条件分岐（レジスタ間接を除く)なら常に予測HIT
    m_btb->PredictBranch ();
    m_trace->SetBpResult (BP_HIT);
    m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
#else // USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc, next_pc, true, true, true);
    m_trace->SetBpResult (bp_result);
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_JR_D32 (uint64_t opcode)
{
    uint32_t disp_lo = OperandExtract (opcode, 31, 17) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 16;
    uint32_t disp    = disp_hi | disp_lo;

    PhysAddr pc = GRegReadPC ();
    PhysAddr next_pc = pc + disp;

    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // 無条件分岐（レジスタ間接を除く)なら常に予測HIT
    m_btb->PredictBranch ();
    m_trace->SetBpResult (BP_HIT);
    m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
#else // USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc, next_pc, true, true, true);
    m_trace->SetBpResult (bp_result);
#endif //  USE_HYBRID_BP
}


void InstV850::Inst_LDB_D16 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode, 31, 16);

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_8BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 8);
    GRegWrite (reg2, s_res);
}


void InstV850::Inst_LDB_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 20);
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 8);
    GRegWrite (reg3, s_res);
}


void InstV850::Inst_LDBU_D16 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg2    = OperandExtract (opcode, 15, 11);
    uint32_t disp_lo = OperandExtract (opcode,  5,  5);
    uint32_t disp_hi = OperandExtract (opcode, 31, 17) << 1;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_8BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    GRegWrite (reg2, res);
}


void InstV850::Inst_LDBU_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 20);
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res);
}


void InstV850::Inst_LDH_D16 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode, 31, 17) << 1;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_16BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 16);
    GRegWrite (reg2, s_res);
}


void InstV850::Inst_LDH_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 21) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 16);
    GRegWrite (reg3, s_res);
}


void InstV850::Inst_LDHU_D16 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode, 31, 17) << 1;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    GRegWrite (reg2, res);
}


void InstV850::Inst_LDHU_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 21) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res);
}


void InstV850::Inst_LDW_D16 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode, 31, 17) << 1;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
    GRegWrite (reg2, res);
}


void InstV850::Inst_LDW_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 21) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res);
}


void InstV850::Inst_LDDW_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 21) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;

    if ((reg3 & 0x1) != 0) {
        // reg3に奇数が指定されたら警告
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: odd register is used for reg3 of LD.DW>\n");
    }

    // mask bit0 for odd register.
    reg3 &= 0x1EU;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_64BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_64BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_64BIT, MA_SIGN, DATA_READ);
        return;
    }

    MemData64 res = m_lsu->MemRead64 (addr);
    GRegWrite64 (reg3, res);
}


void InstV850::Inst_LDLW (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1 = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    // caxi/ldl.w/stc.wではMCTL.MA=1でもMAEは抑止されないので、最初にMAEをチェック
    if (unlikely (m_lsu->IsMemExceptionAtomic<SIZE_32BIT> (addr) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }
    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemReadWithLink (addr, SIZE_32BIT);
    GRegWrite (reg3, res);
}


void InstV850::Inst_LDSR (uint64_t opcode)
{
    uint32_t regid = OperandExtract (opcode, 15, 11);
    uint32_t reg2  = OperandExtract (opcode,  4,  0);
    uint32_t selid = OperandExtract (opcode, 31, 27);

    // @@ mapを使っているので遅いかも
    // @@ LDSR/STSR頻度が多いようなら要高速化

    uint32_t v2 = GRegRead (reg2);
    SRegWriteWithSel (selid, regid, v2);
}


void InstV850::Inst_LOOP (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
    uint32_t disp16 = OperandExtract (opcode, 31, 17) << 1;

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 - 1;
    GRegWrite (reg1, res);

    PhysAddr pc = GRegReadPC ();
    PhysAddr next_pc = pc - disp16;

    bool is_taken = false;
    if (res != 0) {
        GRegWritePC (next_pc);
        m_trace->SetBranchInfo (BRANCH);
        is_taken = true;
    } else {
        // 分岐失敗
    }

    //常に負の方向へ分岐するため、学習対象
#ifdef USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBht (pc, is_taken, false);
#else // USE_HYBRID_BP
    PhysAddr target_pc;
    if (is_taken == true) {
        target_pc = next_pc;
    } else {
        target_pc = pc + 4;
    }

    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (pc, target_pc, is_taken, false, false);
    m_trace->SetBpResult (bp_result);
#endif //  USE_HYBRID_BP
    m_trace->SetBpResult (bp_result);

    SetPswCY (IsCarryAdd (v1, ~0U, res));
    SetPswOV (IsOvflwAdd (v1, static_cast<uint32_t>(~0), res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_MAC (uint64_t opcode)
{
#if defined(COMP_RUNNER)
    // MAC is 3 commit instruction.
    if (NUM_MAC_COMMIT > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return ; // cancel by async event (EIINT)
    }
#endif //#if defined(COMP_RUNNER)

    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;
    uint32_t reg4 = OperandExtract (opcode, 20, 17) << 1;

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t v3h = GRegRead (reg3 + 1);
    uint32_t v3l = GRegRead (reg3);

    int64_t  v2_64  = static_cast<int64_t> (static_cast<int32_t> (v2));
    int64_t  v1_64  = static_cast<int64_t> (static_cast<int32_t> (v1));
    uint64_t v3h_64 = static_cast<uint64_t> (v3h);
    uint64_t v3l_64 = static_cast<uint64_t> (v3l);

    uint64_t acc = (v3h_64 << 32) + v3l_64;
    int64_t  res = v2_64 * v1_64 + acc;

    GRegWrite64 (reg4, res);

    if ((reg4 & 0x1) != 0) {
        // reg3に奇数が指定されたら警告
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: odd register is used for reg4 of MAC>\n");
    }
}


void InstV850::Inst_MACU (uint64_t opcode)
{
#if defined(COMP_RUNNER)
    // MAC is 3 commit instruction.
    if (NUM_MAC_COMMIT > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return ; // cancel by async event (EIINT)
    }
#endif //#if defined(COMP_RUNNER)

    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 28) << 1;
    uint32_t reg4 = OperandExtract (opcode, 20, 17) << 1;

    uint64_t v2  = static_cast<uint64_t> (GRegRead (reg2));
    uint64_t v1  = static_cast<uint64_t> (GRegRead (reg1));
    uint64_t v3h = static_cast<uint64_t> (GRegRead (reg3 + 1));
    uint64_t v3l = static_cast<uint64_t> (GRegRead (reg3));

    uint64_t acc = (v3h << 32) + v3l;
    uint64_t res = v2 * v1 + acc;

    GRegWrite64 (reg4, res);

    if ((reg4 & 0x1) != 0) {
        // reg3に奇数が指定されたら警告
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: odd register is used for reg4 of MACU>\n");
    }
}


void InstV850::Inst_MOV_I32 (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
    uint32_t imm_lo = OperandExtract (opcode, 31, 16);
    uint32_t imm_hi = OperandExtract (opcode, 47, 32) << 16;
    uint32_t res    = imm_hi | imm_lo;

    GRegWrite (reg1, res);
}


void InstV850::Inst_MOV_I5 (uint64_t opcode)
{
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm5  = OperandExtract (opcode,  4,  0);
    uint32_t s_imm = SignExtend (imm5, 5);

    GRegWrite (reg2, s_imm);
}


void InstV850::Inst_MOV (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v1 = GRegRead (reg1);

    GRegWrite (reg2, v1);
}


void InstV850::Inst_MOVEA (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    uint32_t v1    = GRegRead (reg1);
    uint32_t s_imm = SignExtend (imm16, 16);
    uint32_t res   = v1 + s_imm;

    GRegWrite (reg2, res);
}


void InstV850::Inst_MOVHI (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 + (imm16 << 16);

    GRegWrite (reg2, res);
}


void InstV850::Inst_MUL_I9 (uint64_t opcode)
{
    uint32_t reg2   = OperandExtract (opcode, 15, 11);
    uint32_t reg3   = OperandExtract (opcode, 31, 27);
    uint32_t imm_lo = OperandExtract (opcode,  4,  0);
    uint32_t imm_hi = OperandExtract (opcode, 21, 18) << 5;
    uint32_t imm9   = imm_hi | imm_lo;

    uint32_t v2     = GRegRead (reg2);
    uint32_t s_imm  = SignExtend (imm9, 9);

    int64_t  v2_64    = static_cast<int64_t> (static_cast<int32_t> (v2));
    int64_t  s_imm_64 = static_cast<int64_t> (static_cast<int32_t> (s_imm));
    int64_t  res      = v2_64 * s_imm_64;


    uint32_t res_l = static_cast<uint32_t> (res & 0x00FFFFFFFFULL);
    GRegWrite (reg2, res_l);

    uint32_t res_h = static_cast<uint32_t> (res >> 32) & 0x00FFFFFFFFULL;
    GRegWrite (reg3, res_h);
}


void InstV850::Inst_MUL (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2 = GRegRead (reg2);
    uint32_t v1 = GRegRead (reg1);

    int64_t  v2_64 = static_cast<int64_t> (static_cast<int32_t> (v2));
    int64_t  v1_64 = static_cast<int64_t> (static_cast<int32_t> (v1));
    int64_t  res   = v2_64 * v1_64;

    uint32_t res_l = static_cast<uint32_t> (res & 0x00FFFFFFFFULL);
    GRegWrite (reg2, res_l);

    uint32_t res_h = static_cast<uint32_t> (res >> 32) & 0x00FFFFFFFFULL;
    GRegWrite (reg3, res_h);
}


void InstV850::Inst_MULH_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t s_imm = SignExtend (imm5, 5);
    int32_t  v2    = GRegRead (reg2) & 0x0000FFFF;
    int32_t  s_v2  = SignExtend (v2, 16);
    int32_t  res   = s_v2 * s_imm;

    GRegWrite (reg2, static_cast<uint32_t> (res));
}


void InstV850::Inst_MULH (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    int32_t v2   = GRegRead (reg2) & 0x0000FFFF;
    int32_t v1   = GRegRead (reg1) & 0x0000FFFF;
    int32_t s_v1 = SignExtend (v1, 16);
    int32_t s_v2 = SignExtend (v2, 16);
    int32_t res  = s_v1 * s_v2;

    GRegWrite (reg2, static_cast<uint32_t> (res));
}


void InstV850::Inst_MULHI (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    int32_t v1      = GRegRead (reg1) & 0x0000FFFF;
    int32_t s_v1    = SignExtend (v1, 16);
    int32_t s_imm16 = SignExtend (imm16, 16);
    int32_t res     = s_v1 * s_imm16;

    GRegWrite (reg2, static_cast<uint32_t> (res));
}


void InstV850::Inst_MULU_I9 (uint64_t opcode)
{
    uint32_t reg2   = OperandExtract (opcode, 15, 11);
    uint32_t reg3   = OperandExtract (opcode, 31, 27);
    uint32_t imm_lo = OperandExtract (opcode,  4,  0);
    uint32_t imm_hi = OperandExtract (opcode, 21, 18) << 5;
    uint32_t imm9   = imm_hi | imm_lo;

    uint64_t v2    = static_cast<uint64_t> (GRegRead (reg2));
    uint64_t imm64 = static_cast<uint64_t> (imm9);
    uint64_t res   = v2 * imm64;

    uint32_t res_l = static_cast<uint32_t> ( res & 0x00FFFFFFFFULL);
    GRegWrite (reg2, res_l);

    uint32_t res_h = static_cast<uint32_t>  (res >> 32) & 0x00FFFFFFFFULL;
    GRegWrite (reg3, res_h);
}


void InstV850::Inst_MULU (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint64_t v2  = static_cast<uint64_t> (GRegRead (reg2));
    uint64_t v1  = static_cast<uint64_t> (GRegRead (reg1));
    uint64_t res = v2 * v1;

    uint32_t res_l = static_cast<uint32_t> ( res & 0x00FFFFFFFFULL);
    GRegWrite (reg2, res_l);

    uint32_t res_h = static_cast<uint32_t>  (res >> 32) & 0x00FFFFFFFFULL;
    GRegWrite (reg3, res_h);
}


void InstV850::Inst_NOP (uint64_t opcode)
{
}


void InstV850::Inst_NOT (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = ~v1;
    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_NOT1_B3 (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t bit3   = OperandExtract (opcode, 13, 11);
#endif // SUPPORT_SC_HEAP
    uint32_t disp   = OperandExtract (opcode, 31, 16);
    uint32_t s_disp = SignExtend (disp, 16);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitRmwBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionRMW (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemReadBitOp (addr, SIZE_8BIT, SideBand::BITOP_NOT);
        if (m_trace->IsCancel ()) {
            // CForest実装依存により、PEGでの判定がReadとWrite別々に実施されてしまう。
            // Read不許可ならWrite側も不許可として実施しない
            return;
        }
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadBitOpMdp (addr, SIZE_8BIT, SideBand::BITOP_NOT);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t pos = bit3;
    bool z = (~(token >> pos)) & 0x01;
    token ^= (1 << pos);

    if (is_mdp_suppressed == false) {
        m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_NOT);
        SetPswZ (z);
    } else {
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemWriteBitOpMdp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_NOT);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_NOT1_B3_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t bit3   = OperandExtract (opcode, 13, 11);
    uint32_t disp   = OperandExtract (opcode, 31, 16);
    uint32_t s_disp = SignExtend (disp, 16);

    uint32_t v1   = m_trace->GetRregValue(0) & 0xFFFFFFFF;
    PhysAddr addr = v1 + s_disp;

    uint32_t token = read_data;
    uint32_t pos = bit3;
    bool z = (~(token >> pos)) & 0x01;
    token ^= (1 << pos);

    m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_NOT);
    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_NOT1 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
#endif // SUPPORT_SC_HEAP

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitRmwBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionRMW (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemReadBitOp (addr, SIZE_8BIT, SideBand::BITOP_NOT);

        if (m_trace->IsCancel ()) {
            // CForest実装依存により、PEGでの判定がReadとWrite別々に実施されてしまう。
            // Read不許可ならWrite側も不許可として実施しない
            return;
        }
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadBitOpMdp (addr, SIZE_8BIT, SideBand::BITOP_NOT);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;
    token ^= (1 << pos);

    if (is_mdp_suppressed == false) {
        m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_NOT);
        SetPswZ (z);
    } else {
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemWriteBitOpMdp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_NOT);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_NOT1_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v1   = m_trace->GetRregValue(0) & 0xFFFFFFFF;
    PhysAddr addr = v1;

    uint32_t token = read_data;
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;
    token ^= (1 << pos);

    m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_NOT);
    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_OR (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 | v1;

    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_ORI (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 | imm16;

    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_POPSP (uint64_t opcode)
{
    uint32_t rh = OperandExtract (opcode,  4,  0);
    uint32_t rt = OperandExtract (opcode, 31, 27);
#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)
    if (rh <= rt) {
        uint32_t cur = rt;
        uint32_t end = rh;
        PhysAddr eaddr = GRegRead (R_SP);
        while (cur >= end) {
#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return ; // cancel by async event (EIINT)
            }
#endif //#if defined(COMP_RUNNER)

            PhysAddr addr = eaddr & ~(PhysAddr (0x03));
#ifdef ENABLE_DBG_BREAK
            if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
                return;
            }
            if (unlikely (m_lsu->IsAlignmentError (eaddr) == true)) {
                m_srf->SrDIR0()->SetAT (1);
                return;
            }
#endif // ENABLE_DBG_BREAK

            if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
                if (cur == R_SP) {  // if written addr is R_SP, MEI.REG = 0
                    cur = R0;
                }
                SetMemErrorInfo (addr, cur, SIZE_32BIT, MA_SIGN, DATA_READ);
                return;
            }
#if defined(COMP_RUNNER)
            //if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            //    m_trace->SetCancelMcyc (true);
            //    return; // cancel by synchronous events (MDP, LSAB, etc)
            //}
            ++ completed_count;
#endif //#if defined(COMP_RUNNER)

            uint32_t val = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
#ifdef ENABLE_DBG_BREAK
            // Apply EO bit because access is done
            m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
            if (cur != R_SP) {
                //! R3(SP)へは格納されない
                GRegWrite (cur, val);
            } else {
                GRegWrite (R0, val);
            }
            eaddr += 4;
            if (cur == R0) {
                break;
            }
            cur -= 1;
        }

#if defined(COMP_RUNNER)
        if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            m_trace->SetCancelMcyc (true);
            return; // cancel by async event (EIINT)
        }
#endif //#if defined(COMP_RUNNER)

        GRegWrite (R_SP, eaddr);
    } else {
        PhysAddr eaddr = GRegRead (R_SP);
        GRegWrite (R_SP, eaddr);
    }
}


void InstV850::Inst_PUSHSP (uint64_t opcode)
{
    uint32_t rh = OperandExtract (opcode,  4,  0);
    uint32_t rt = OperandExtract (opcode, 31, 27);

#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)

    if (rh <= rt) {
        uint32_t cur = rh;
        uint32_t end = rt;
        PhysAddr eaddr = GRegRead (R_SP);
        while (cur <= end) {
#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return; // cancel by async event (EIINT)
            }
#endif //#if defined(COMP_RUNNER)

            eaddr -= 4;
            RegData val = GRegRead (cur);
            PhysAddr addr = eaddr & ~(PhysAddr (0x03));

#ifdef ENABLE_DBG_BREAK
            if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
                return;
            }
            if (unlikely (m_lsu->IsAlignmentError (eaddr) == true)) {
                m_srf->SrDIR0()->SetAT (1);
                return;
            }
#endif // ENABLE_DBG_BREAK

            bool is_mdp_suppressed = false;
            if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
                if (m_trace->GetExpCode () != EXP_NONE) {
                    SetMemErrorInfo (addr, cur, SIZE_32BIT, MA_SIGN, DATA_WRITE);
                    return;
                }
                is_mdp_suppressed = true;
                m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
            }

            if (is_mdp_suppressed == false) {
#if defined(COMP_RUNNER)
                //if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                //    m_trace->SetCancelMcyc (true);
                //    return; // cancel by synchronous events (MDP, LSAB, etc)
                //}
                ++ completed_count;
#endif //#if defined(COMP_RUNNER)
                m_lsu->MemWrite (addr, val, SIZE_32BIT);
            }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
            else {
                m_lsu->MemWriteMdp (addr, val, SIZE_32BIT);
            }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
#ifdef ENABLE_DBG_BREAK
            // Apply EO bit because access is done
            m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
            cur += 1;
        }

#if defined(COMP_RUNNER)
        if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            m_trace->SetCancelMcyc (true);
            return; // cancel by async event (EIINT)
        }
#endif //#if defined(COMP_RUNNER)

        GRegWrite (R_SP, eaddr);
    } else {
        PhysAddr eaddr = GRegRead (R_SP);
        GRegWrite (R_SP, eaddr);
    }
}


void InstV850::Inst_PREF (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode, 4, 0);
    uint32_t op   = OperandExtract (opcode, 31, 27);

    uint32_t v1 = GRegRead (reg1);
    if (op == Icache::OP_PREFI) {
        if (unlikely (IsMemExceptionIcLine (v1) == true)) {
            // PREFでは例外を発行しないが PREF処理は実行しない
            // 例外情報をキャンセル
            m_trace->SetExpCode (EXP_NONE);
            return;
        }

        PhysAddr phys_line_addr = m_lsu->AddressTransfer (v1);
        if (m_ic->IsCachingArea (phys_line_addr)) {
            m_ic->OperatePref (phys_line_addr, v1, m_lsu->GetSideBand (), m_trace);
            if (m_ic->m_fill_route == ICACHE_MISS_ROUTE) {
                m_lsu->m_flashrom_fetch_count += 1; // PMCTRL.CND=50h
            }
        } else {
            CfMsg::TPrint (MSG_WAR, m_trace,
                           "<Warning: irregular PREFI-address (0x%08x)>\n", phys_line_addr);
        }
    } else {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: irregular PREF-operation (0x%02x)>\n", op);
    }
}


void InstV850::Inst_PREPARE (uint64_t opcode)
{
    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;
    uint32_t imm5   = OperandExtract (opcode,  5,  1);

    PrepareStack (list12, imm5);
}


void InstV850::Inst_PREPARE_SP (uint64_t opcode)
{
    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;
    uint32_t imm5   = OperandExtract (opcode,  5,  1);

    if (PrepareStack (list12, imm5) == PREP_OK) {
        uint32_t sp = GRegRead (R_SP);
        GRegWrite (R_EP, sp);
    }
}


void InstV850::Inst_PREPARE_LO16 (uint64_t opcode)
{
    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;
    uint32_t imm5   = OperandExtract (opcode,  5,  1);
    uint32_t imm16  = OperandExtract (opcode, 47, 32);

    if (PrepareStack (list12, imm5) == PREP_OK) {
        uint32_t s_imm = SignExtend (imm16, 16);
        GRegWrite (R_EP, s_imm);
    }
}


void InstV850::Inst_PREPARE_HI16 (uint64_t opcode)
{
    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;
    uint32_t imm5   = OperandExtract (opcode,  5,  1);
    uint32_t imm16  = OperandExtract (opcode, 47, 32);

    if (PrepareStack (list12, imm5) == PREP_OK) {
        uint32_t imm_hi = imm16 << 16;
        GRegWrite (R_EP, imm_hi);
    }
}


void InstV850::Inst_PREPARE_I32 (uint64_t opcode)
{
    // list12を r20-r31に並び替え
    uint32_t l20    = OperandExtract (opcode, 27, 24) << 8;
    uint32_t l24    = OperandExtract (opcode, 31, 28) << 4;
    uint32_t l28    = OperandExtract (opcode, 23, 22) << 2;
    uint32_t l30    = OperandExtract (opcode,  0,  0) << 1;
    uint32_t l31    = OperandExtract (opcode, 21, 21);
    uint32_t list12 = l20 | l24 | l28 | l30 | l31;
    uint32_t imm5   = OperandExtract (opcode,  5,  1);
    uint32_t imm32  = OperandExtract (opcode, 63, 32);

    if (PrepareStack (list12, imm5) == PREP_OK) {
        GRegWrite (R_EP, imm32);
    }
}


bool InstV850::PopGRegBody (PhysAddr addr, CReg reg)
{
#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        // Set exp code to trace in IsHitReadBrk ().
        return false;
    }
#endif // ENABLE_DBG_BREAK

#ifdef ENABLE_RB_MDP
    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, R0, SIZE_32BIT, MA_SIGN, DATA_READ);
        return false;
    }
#endif // #ifdef ENABLE_RB_MDP

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
    if ((reg >= R0) && (reg <= R31)) {
        // R0-R31
        GRegWrite (reg, res);
    } else {
        CfMsg::DPrint (MSG_ERR, "<Internal error: invalid register(%x) at PopGRegBody>\n", reg);
    }
#ifdef ENABLE_DBG_BREAK
    // Apply EO bit because access is done
    m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
    return true;
}


bool InstV850::PopSRegBody (PhysAddr addr, CfSReg reg, bool is_update_sreg)
{
#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        // Set exp code to trace in IsHitReadBrk ().
        return false;
    }
#endif // ENABLE_DBG_BREAK

#ifdef ENABLE_RB_MDP
    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, R0, SIZE_32BIT, MA_SIGN, DATA_READ);
        return false;
    }
#endif // #ifdef ENABLE_RB_MDP

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);

    if (is_update_sreg) {
        SRegWrite (reg, res);
    }

#ifdef ENABLE_DBG_BREAK
    // Apply EO bit because access is done
    m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK

    return true;
}


bool InstV850::PopRegBank (uint32_t bank_pos)
{
    bool pop_mode = m_srf->SrRBCR0 ()->GetMD ();
    PhysAddr rbip = m_srf->SrRBIP ()->ReadBody ();
    PhysAddr target_addr;
    PhysAddr size;

#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
    //fprintf (g_cmp_fp, "Get completed_count %d\n", m_fsctrl->GetCancelTimingMcycInst ());
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return false; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)

    if (pop_mode == 0) {
        size = 0x60;
    } else {
        size = 0x90;
    }
    target_addr = rbip - (bank_pos * size);
    if (pop_mode == 0) {
        if (PopGRegBody (target_addr, R30) == false) {
            return false;
        }
        target_addr += 4;

#if defined(COMP_RUNNER)
        ++ completed_count;
#endif //#if defined(COMP_RUNNER)

    } else {
        target_addr += 4;
        for (CReg regno = R31; regno >= R20; regno--) {
#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return false; // cancel by async event
            }
            ++ completed_count;
#endif //#if defined(COMP_RUNNER)
            if (PopGRegBody (target_addr, regno) == false) {
                return false;
            }
            target_addr += 4;
        }
    }
    for (CReg regno = R19; regno >= R1; regno--) {
#if defined(COMP_RUNNER)
        if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            m_trace->SetCancelMcyc (true);
            return false; // cancel by async event
        }
        ++ completed_count;
#endif //#if defined(COMP_RUNNER)
        if (PopGRegBody (target_addr, regno) == false) {
            return false;
        }
        target_addr += 4;
    }

    bool is_update_sreg = true;

#if defined(COMP_RUNNER)
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return false; // cancel by async event
    }
    ++ completed_count;

    // For system registers, 2 commit is needed.
    // 1 commit is to load, 1 commit is to ldsr to sreg.
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        // If 2nd commit is canceled, although load data, don't update sreg.
        is_update_sreg = false;
    }
    ++ completed_count;
#endif //#if defined(COMP_RUNNER)
    if (PopSRegBody (target_addr, CF_SR_FPSR, is_update_sreg) == false) {
        return false;
    }
    target_addr += 4;

#if defined(COMP_RUNNER)
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return false; // cancel by async event
    }
    ++ completed_count;

    // For system registers, 2 commit is needed.
    // 1 commit is to load, 1 commit is to ldsr to sreg.
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        // If 2nd commit is canceled, although load data, don't update sreg.
        is_update_sreg = false;
    }
    ++ completed_count;
#endif //#if defined(COMP_RUNNER)
    if (PopSRegBody (target_addr, CF_SR_VEIIC, is_update_sreg) == false) {
        return false;
    }
    target_addr += 4;

#if defined(COMP_RUNNER)
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return false; // cancel by async event
    }
    ++ completed_count;

    // For system registers, 2 commit is needed.
    // 1 commit is to load, 1 commit is to ldsr to sreg.
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        // If 2nd commit is canceled, although load data, don't update sreg.
        is_update_sreg = false;
    }
    ++ completed_count;
#endif //#if defined(COMP_RUNNER)
    if (PopSRegBody (target_addr, CF_SR_VEIPSW, is_update_sreg) == false) {
        return false;
    }
    target_addr += 4;

#if defined(COMP_RUNNER)
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return false; // cancel by async event
    }
    ++ completed_count;

    // For system registers, 2 commit is needed.
    // 1 commit is to load, 1 commit is to ldsr to sreg.
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        // If 2nd commit is canceled, although load data, don't update sreg.
        is_update_sreg = false;
    }
    ++ completed_count;
#endif //#if defined(COMP_RUNNER)
    if (PopSRegBody (target_addr, CF_SR_VEIPC, is_update_sreg) == false) {
        return false;
    }
    target_addr += 4;

#if defined(COMP_RUNNER)
    // to cancel last commit
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return false; // cancel by async event
    }
    ++ completed_count;
#endif //#if defined(COMP_RUNNER)

    return true;
}


void InstV850::Inst_RESBANK (uint64_t opcode)
{
    uint32_t bank_pos = m_srf->SrRBNR ()->GetBN ();
    if (bank_pos > 0) {
        if (PopRegBank (bank_pos) == true) {
            m_srf->SrRBNR ()->SetBN (bank_pos - 1);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_RBNR, m_srf->SrRBNR()->ReadBody());
            SetPswID (1);
        }
    } else {
        // Cause re-exec type SYSERR
        m_trace->SetExpCode (EXP_SYSERR_EX);
        return;
    }
}


void InstV850::Inst_RIE (uint64_t opcode)
{
    m_trace->SetExpCode (EXP_RIE);
}


void InstV850::Inst_RIE_I9 (uint64_t opcode)
{
    // 動作は RIEと同じ
    Inst_RIE (opcode);
}


void InstV850::Inst_ROTL_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t v2  = GRegRead (reg2);
    uint32_t res = (v2 << imm5) | (v2 >> (32 - imm5));

    bool cy;
    if (imm5 == 0) {
        cy = false;
    } else {
        cy = res & 0x01;
    }

    GRegWrite (reg3, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_ROTL_R3 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t sft = v1 & 0x0000001F;
    uint32_t res = (v2 << sft) | (v2 >> (32 - sft));

    bool cy;
    if (sft == 0) {
        cy = false;
    } else {
        cy = res & 0x01;
    }

    GRegWrite (reg3, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SAR_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    int32_t  v2  = static_cast<int32_t> (GRegRead (reg2));
    uint32_t res = static_cast<uint32_t> (v2 >> imm5);

    bool cy;
    if (imm5 == 0) {
        cy = false;
    } else {
        cy = (v2 >> (imm5 - 1)) & 0x00000001;
    }

    GRegWrite (reg2, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SAR_R3 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    int32_t  v2  = static_cast<int32_t> (GRegRead (reg2));
    uint32_t v1  = GRegRead (reg1);
    uint32_t sft = v1 & 0x0000001F;
    uint32_t res = static_cast<uint32_t> (v2 >> sft);

    bool cy;
    if (sft == 0) {
        cy = false;
    } else {
        cy = (v2 >> (sft - 1)) & 0x00000001;
    }

    GRegWrite (reg3, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SAR (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    int32_t  v2  = static_cast<int32_t> (GRegRead (reg2));
    uint32_t v1  = GRegRead (reg1);
    uint32_t sft = v1 & 0x0000001F;
    uint32_t res = static_cast<uint32_t> (v2 >> sft);

    bool cy;
    if (sft == 0) {
        cy = false;
    } else {
        cy = (v2 >> (sft - 1)) & 0x00000001;
    }

    GRegWrite (reg2, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SASF (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t cc4  = OperandExtract (opcode,  3,  0);

    uint32_t v2  = GRegRead (reg2);
    uint32_t res = (v2 << 1);

    if (IsPswCondMatch (cc4)) {
        res |= 0x00000001;
    }

    GRegWrite (reg2, res);
}


void InstV850::Inst_SATADD_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t v2    = GRegRead (reg2);
    uint32_t s_imm = SignExtend (imm5, 5);

    SaturateAdd (v2, s_imm, reg2);
}


void InstV850::Inst_SATADD (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2 = GRegRead (reg2);
    uint32_t v1 = GRegRead (reg1);

    SaturateAdd (v2, v1, reg2);
}


void InstV850::Inst_SATADD_R3 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2 = GRegRead (reg2);
    uint32_t v1 = GRegRead (reg1);

    SaturateAdd (v2, v1, reg3);
}


void InstV850::Inst_SATSUB (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2 = GRegRead (reg2);
    uint32_t v1 = GRegRead (reg1);

    SaturateSub (v2, v1, reg2);
}


void InstV850::Inst_SATSUB_R3 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2 = GRegRead (reg2);
    uint32_t v1 = GRegRead (reg1);

    SaturateSub (v2, v1, reg3);
}


void InstV850::Inst_SATSUBI (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    uint32_t v1    = GRegRead (reg1);
    uint32_t s_imm = SignExtend (imm16, 16);

    SaturateSub (v1, s_imm, reg2);
}


void InstV850::Inst_SATSUBR (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2 = GRegRead (reg2);
    uint32_t v1 = GRegRead (reg1);

    SaturateSub (v1, v2, reg2);
}


void InstV850::Inst_SBF (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t cc4  = OperandExtract (opcode, 20, 17);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 - v1;

    bool cy = IsCarrySub (v2, v1);
    bool ov = IsOvflwSub (v2, v1, res);

    if (IsPswCondMatch (cc4)) {
        // 実際は res = v2 - v1 - 1 を計算したい
        // Carry, Overflow計算をするために分割
        cy ^= IsCarrySub (res, 1U);
        ov ^= IsOvflwSub (res, static_cast<uint32_t>(1), res - 1);
        res--;
    }

    GRegWrite (reg3, res);

    SetPswCY (cy);
    SetPswOV (ov);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SCH0L (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t val = ~v2;

    uint32_t pos;
    for (pos = 1; pos <= 32; pos++) {
        if (val & 0x80000000U) break;
        val <<= 1;
    }

    // 0が見つからなかった場合は 0を格納
    uint32_t res = (pos == 33) ? 0 : pos;
    GRegWrite (reg3, res);

    SetPswCY (pos == 32);
    SetPswOV (0);
    SetPswS  (0);
    SetPswZ  (pos == 33);
}


void InstV850::Inst_SCH0R (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t val = ~v2;

    uint32_t pos;
    for (pos = 1; pos <= 32; pos++) {
        if (val & 0x00000001U) break;
        val >>= 1;
    }

    // 0が見つからなかった場合は 0を格納
    uint32_t res = (pos == 33) ? 0 : pos;
    GRegWrite (reg3, res);

    SetPswCY (pos == 32);
    SetPswOV (0);
    SetPswS  (0);
    SetPswZ  (pos == 33);
}


void InstV850::Inst_SCH1L (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t val = v2;

    uint32_t pos;
    for (pos = 1; pos <= 32; pos++) {
        if (val & 0x80000000U) break;
        val <<= 1;
    }

    // 1が見つからなかった場合は 0を格納
    uint32_t res = (pos == 33) ? 0 : pos;
    GRegWrite (reg3, res);

    SetPswCY (pos == 32);
    SetPswOV (0);
    SetPswS  (0);
    SetPswZ  (pos == 33);
}


void InstV850::Inst_SCH1R (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t val = v2;

    uint32_t pos;
    for (pos = 1; pos <= 32; pos++) {
        if (val & 0x00000001U) break;
        val >>= 1;
    }

    // 1が見つからなかった場合は 0を格納
    uint32_t res = (pos == 33) ? 0 : pos;
    GRegWrite (reg3, res);

    SetPswCY (pos == 32);
    SetPswOV (0);
    SetPswS  (0);
    SetPswZ  (pos == 33);
}


void InstV850::Inst_SET1_B3 (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t bit3   = OperandExtract (opcode, 13, 11);
#endif // SUPPORT_SC_HEAP
    uint32_t disp   = OperandExtract (opcode, 31, 16);
    uint32_t s_disp = SignExtend (disp, 16);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitRmwBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionRMW (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemReadBitOp (addr, SIZE_8BIT, SideBand::BITOP_SET);

        if (m_trace->IsCancel ()) {
            // CForest実装依存により、PEGでの判定がReadとWrite別々に実施されてしまう。
            // Read不許可ならWrite側も不許可として実施しない
            return;
        }
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadBitOpMdp (addr, SIZE_8BIT, SideBand::BITOP_SET);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t pos = bit3;
    bool z = (~(token >> pos)) & 0x01;
    token |= (1 << pos);

    if (is_mdp_suppressed == false) {
        m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_SET);
        SetPswZ (z);
    } else {
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemWriteBitOpMdp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_SET);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_SET1_B3_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t bit3   = OperandExtract (opcode, 13, 11);
    uint32_t disp   = OperandExtract (opcode, 31, 16);
    uint32_t s_disp = SignExtend (disp, 16);

    uint32_t v1   = m_trace->GetRregValue(0) & 0xFFFFFFFF;
    PhysAddr addr = v1 + s_disp;

    uint32_t token = read_data;
    uint32_t pos = bit3;
    bool z = (~(token >> pos)) & 0x01;
    token |= (1 << pos);

    m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_SET);
    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_SET1 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
#endif // SUPPORT_SC_HEAP

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitRmwBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionRMW (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemReadBitOp (addr, SIZE_8BIT, SideBand::BITOP_SET);

        if (m_trace->IsCancel ()) {
            // CForest実装依存により、PEGでの判定がReadとWrite別々に実施されてしまう。
            // Read不許可ならWrite側も不許可として実施しない
            return;
        }
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadBitOpMdp (addr, SIZE_8BIT, SideBand::BITOP_SET);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;
    token |= (1 << pos);

    if (is_mdp_suppressed == false) {
        m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_SET);
        SetPswZ (z);
    } else {
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemWriteBitOpMdp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_SET);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_SET1_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v1   = m_trace->GetRregValue(0) & 0xFFFFFFFF;
    PhysAddr addr = v1;

    uint32_t token = read_data;
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;
    token |= (1 << pos);

    m_lsu->MemWriteBitOp (addr, token & 0x00FF, SIZE_8BIT, SideBand::BITOP_SET);
    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_SETF (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t cc4  = OperandExtract (opcode,  3,  0);

    uint32_t res = (IsPswCondMatch (cc4)) ? 1 : 0;

    GRegWrite (reg2, res);
}


void InstV850::Inst_SHL_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t v2  = GRegRead (reg2);
    uint32_t res = v2 << imm5;

    bool cy;
    if (imm5 == 0) {
        cy = false;
    } else {
        cy = (v2 >> (32 - imm5)) & 0x00000001;
    }

    GRegWrite (reg2, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SHL (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t sft = v1 & 0x0000001F;
    uint32_t res = v2 << sft;

    bool cy;
    if (sft == 0) {
        cy = false;
    } else {
        cy = (v2 >> (32 - sft)) & 0x00000001;
    }

    GRegWrite (reg2, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SHL_R3 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t sft = v1 & 0x0000001F;
    uint32_t res = v2 << sft;

    bool cy;
    if (sft == 0) {
        cy = false;
    } else {
        cy = (v2 >> (32 - sft)) & 0x00000001;
    }

    GRegWrite (reg3, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SHR_I5 (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t imm5 = OperandExtract (opcode,  4,  0);

    uint32_t v2  = GRegRead (reg2);
    uint32_t res = v2 >> imm5;

    bool cy;
    if (imm5 == 0) {
        cy = false;
    } else {
        cy = (v2 >> (imm5 - 1)) & 0x00000001;
    }

    GRegWrite (reg2, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SHR (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t sft = v1 & 0x0000001F;
    uint32_t res = v2 >> sft;

    bool cy;
    if (sft == 0) {
        cy = false;
    } else {
        cy = (v2 >> (sft - 1)) & 0x00000001;
    }

    GRegWrite (reg2, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SHR_R3 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t sft = v1 & 0x0000001F;
    uint32_t res = v2 >> sft;

    bool cy;
    if (sft == 0) {
        cy = false;
    } else {
        cy = (v2 >> (sft - 1)) & 0x00000001;
    }

    GRegWrite (reg3, res);

    SetPswCY (cy);
    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SLDB (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  6,  0);

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_8BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 8);
    GRegWrite (reg2, s_res);
}


void InstV850::Inst_SLDBU (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  3,  0);

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_8BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    GRegWrite (reg2, res);
}


void InstV850::Inst_SLDH (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  6,  0) << 1;

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_16BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 16);
    GRegWrite (reg2, s_res);
}


void InstV850::Inst_SLDHU (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  3,  0) << 1;

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    GRegWrite (reg2, res);
}


void InstV850::Inst_SLDW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  6,  1) << 2;

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, reg2, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
    GRegWrite (reg2, res);
}


void InstV850::Inst_SNOOZE (uint64_t opcode)
{
    // Snooze期間が０でない、かつ、
    // 割り込みを要求されていなければSNOOZE状態をセット
    if ( (m_fsctrl->IsAnyAsyncException () == false)
        && (m_fsctrl->GetSnoozePeriod () != 0) ) {
        m_fsctrl->SetSnooze (true);
    }
}


void InstV850::Inst_SSTB (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  6,  0);

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;
    uint32_t v2   = GRegRead (reg2);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg2, SIZE_8BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2 & 0x00FF, SIZE_8BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2 & 0x00FF, SIZE_8BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_SSTH (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  6,  0) << 1;

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;
    uint32_t v2   = GRegRead (reg2);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_16BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg2, SIZE_16BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_16BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2 & 0x0000FFFF, SIZE_16BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2 & 0x0000FFFF, SIZE_16BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_SSTW (uint64_t opcode)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode,  6,  1) << 2;

    uint32_t ep   = GRegRead (R_EP);
    PhysAddr addr = ep + disp;
    uint32_t v2   = GRegRead (reg2);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg2, SIZE_32BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STB_D16 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode, 31, 16);

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;
    uint32_t v2     = GRegRead (reg2);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg2, SIZE_8BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2 & 0x00FF, SIZE_8BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2 & 0x00FF, SIZE_8BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STB_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 20);
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;
    uint32_t v2     = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2 & 0x00FF, SIZE_8BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2 & 0x00FF, SIZE_8BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STH_D16 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode, 31, 17) << 1;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;
    uint32_t v2     = GRegRead (reg2);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_16BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg2, SIZE_16BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_16BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2 & 0x0000FFFF, SIZE_16BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2 & 0x0000FFFF, SIZE_16BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STH_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 21) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;
    uint32_t v2     = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_16BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_16BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2 & 0x0000FFFF, SIZE_16BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2 & 0x0000FFFF, SIZE_16BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STW_D16 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
    uint32_t disp = OperandExtract (opcode, 31, 17) << 1;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 16);
    PhysAddr addr   = v1 + s_disp;
    uint32_t v2     = GRegRead (reg2);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg2, SIZE_32BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v2, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v2, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STW_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 21) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;
    uint32_t v3     = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v3, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v3, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STDW_D23 (uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t disp_lo = OperandExtract (opcode, 26, 21) << 1;
    uint32_t disp_hi = OperandExtract (opcode, 47, 32) << 7;
    uint32_t disp    = disp_hi | disp_lo;

    if ((reg3 & 0x1) != 0) {
        // reg3に奇数が指定されたら警告
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: odd register is used for reg3 of ST.DW>\n");
    }

    // mask bit0 for odd register.
    reg3 &= 0x1EU;

    uint32_t v1     = GRegRead (reg1);
    uint32_t s_disp = SignExtend (disp, 23);
    PhysAddr addr   = v1 + s_disp;
    RegData64 v3    = GRegRead64 (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_64BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_64BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_64BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_64BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite64 (addr, v3);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWrite64Mdp (addr, v3);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
}


void InstV850::Inst_STCW (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;
    uint32_t v3   = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    // SDB occurs despite LLbit=0.
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    // caxi/ldl.w/stc.wではMCTL.MA=1でもMAEは抑止されないので、最初にMAEをチェック
    if (unlikely (m_lsu->IsMemExceptionAtomic<SIZE_32BIT> (addr) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_WRITE);
        return;
    }
    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
    }

    if ((m_lsu->IsHaveLink (addr, SIZE_32BIT) == true) && (is_mdp_suppressed == false)) {
        m_lsu->MemWrite (addr, v3, SIZE_32BIT, true);
        if (m_trace->IsCancel()) {
            GRegWrite (reg3, 0x00U);
        } else {
            GRegWrite (reg3, 0x01U);
        }
    } else {
        GRegWrite (reg3, 0x00U);
    }

    // Delete LLBit
    m_lsu->DeleteOwnLink ();
}


void InstV850::Inst_STSR (uint64_t opcode)
{
    // reg2にて、reg1のビットフィールドを参照する点に注意
    uint32_t regid = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t selid = OperandExtract (opcode, 31, 27);

    // @@ mapを使っているので遅いかも
    // @@ LDSR/STSR頻度が多いようなら要高速化

    uint32_t res = SRegReadWithSel (selid, regid);
    if (m_trace->GetExpCode() == EXP_NONE) {
        GRegWrite (reg2, res);
    }
}


void InstV850::Inst_SUB (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 - v1;

    GRegWrite (reg2, res);

    SetPswCY (IsCarrySub (v2, v1));
    SetPswOV (IsOvflwSub (v2, v1, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SUBR (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 - v2;

    GRegWrite (reg2, res);

    SetPswCY (IsCarrySub (v1, v2));
    SetPswOV (IsOvflwSub (v1, v2, res));
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_SWITCH (uint64_t opcode)
{
    PhysAddr pc   = GRegReadPC ();
    uint32_t reg1 = OperandExtract (opcode,  4,  0);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = (pc + 2) + (v1 << 1);

#if defined(COMP_RUNNER)
    if (NUM_SWITCH_PRE_LSAB > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, R0, SIZE_16BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t val     = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);

#if defined(COMP_RUNNER)
    if (NUM_SWITCH_POST_LSAB > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)

    uint32_t s_val   = SignExtend (val, 16);
    PhysAddr next_pc = (pc + 2) + (s_val << 1);

    PhysAddr curr_pc = m_grf->ReadPC (); // 履歴に表示したくないので、直接アクセス

    GRegWritePC (next_pc);
    m_trace->SetBranchInfo (BRANCH);

#ifdef USE_HYBRID_BP
    // BTAC予測
    BpResult bp_result = m_btb->PredictBranchAndStudyBtac (curr_pc, next_pc);
    m_trace->SetBpResult (bp_result);
    if (bp_result == BP_MISS) {
        m_btb->ReserveUpdateGhr (false); // 条件分岐のTakenでだけtrue(1)で更新。それ以外の分岐ではfalse(0)で更新。
    }
#else // USE_HYBRID_BP
    BpResult bp_result = m_btb->PredictBranchAndStudyBtacBht (curr_pc, next_pc, true, true, true);
    m_trace->SetBpResult (bp_result);
#endif // USE_HYBRID_BP
}


void InstV850::Inst_SXB (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = SignExtend (v1 & 0x00FF, 8);

    GRegWrite (reg1, res);
}


void InstV850::Inst_SXH (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = SignExtend (v1 & 0x0000FFFF, 16);

    GRegWrite (reg1, res);
}


void InstV850::Inst_SYNCE (uint64_t opcode)
{
    // 機能は NOPと同じ サイクル動作が異なる
}


void InstV850::Inst_SYNCI (uint64_t opcode)
{
    // SYNCI実行時には命令キュー/バッファをクリア
    m_fsctrl->InitInstQueue ();
}


void InstV850::Inst_SYNCM (uint64_t opcode)
{
    // 機能は NOPと同じ サイクル動作が異なる
}


void InstV850::Inst_SYNCP (uint64_t opcode)
{
    // 機能は NOPと同じ サイクル動作が異なる
}


void InstV850::Inst_SYSCALL (uint64_t opcode)
{
    uint32_t vec_hi = OperandExtract (opcode, 29, 27) << 5;
    uint32_t vec_lo = OperandExtract (opcode,  4,  0);
    uint32_t vec8   = vec_hi | vec_lo;

    uint32_t sccfg_size = m_srf->SrSCCFG()->GetSIZE ();
    uint32_t scbp = SRegRead (CF_SR_SCBP);
    uint32_t addr = scbp;
    if (vec8 <= sccfg_size) {
        addr += (vec8 << 2);
    }

#if defined(COMP_RUNNER)
    if (NUM_CALL_PRE_LSAB > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    // PSW.UMを0にした上で保護判定を実施
    bool um_tmp = m_psw->GetUM ();
    m_psw->SetUM (0);
    bool is_mem_excp = m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT);
    m_psw->SetUM (um_tmp);
    if (unlikely (is_mem_excp == true)) {
        SetMemErrorInfo (addr, R0, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t val       = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);

#if defined(COMP_RUNNER)
    if (NUM_CALL_POST_LSAB > m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return; // cancel by async event
    }
#endif //#if defined(COMP_RUNNER)

    PhysAddr ret_pc    = m_trace->GetInstAddr () + 4;
    uint32_t exp_cause = ExpInfo::GetExpCause (EXP_SYSCALL) + vec8;
    uint32_t hndl_ofst = scbp + val;
    bool     is_excp   = true;  // PSW.EP用: true = 例外
    bool     is_table  = false; // 例外ハンドラ仕様 (false:標準 true:テーブル)

    m_trace->SetExpCode (EXP_SYSCALL);
    ExceptionEI (exp_cause, ret_pc, hndl_ofst, is_excp, is_table);
    m_fsctrl->m_exp_msg->ExpMsgPrint (EXP_SYSCALL, exp_cause, m_trace);
}


void InstV850::Inst_TRAP (uint64_t opcode)
{
    // vec5の最上位ビットで TRAP0/1 を切り替える
    // あらかじめ分離してデコード
    uint32_t vec1 = OperandExtract (opcode,  4,  4);
    uint32_t vec4 = OperandExtract (opcode,  3,  0);

    ExpCode exp_code = (vec1 == 0x0) ? EXP_TRAP0 : EXP_TRAP1;

    PhysAddr ret_pc    = m_trace->GetInstAddr () + 4;
    uint32_t exp_cause = ExpInfo::GetExpCause (exp_code) + vec4;
    uint32_t hndl_ofst = ExpInfo::GetHdlOffset (exp_code);
    bool     is_excp   = true;  // PSW.EP用: true = 例外
    bool     is_table  = false; // 例外ハンドラ仕様 (false:標準 true:テーブル)

    m_trace->SetExpCode (exp_code);
    ExceptionEI (exp_cause, ret_pc, hndl_ofst, is_excp, is_table);
    m_fsctrl->m_exp_msg->ExpMsgPrint (exp_code, exp_cause, m_trace);
}


void InstV850::Inst_TST (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 & v1;

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_TST1_B3 (uint64_t opcode)
{
    uint32_t reg1   = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t bit3   = OperandExtract (opcode, 13, 11);
#endif // SUPPORT_SC_HEAP
    uint32_t disp   = OperandExtract (opcode, 31, 16);
    uint32_t s_disp = SignExtend (disp, 16);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1 + s_disp;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionTST (addr) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_OFF);
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadMdp (addr, SIZE_8BIT, Lsu::SPCLD_OFF);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t pos = bit3;
    bool z = (~(token >> pos)) & 0x01;

    if (is_mdp_suppressed == false) {
        SetPswZ (z);
    } else {
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_TST1_B3_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t pos   = OperandExtract (opcode, 13, 11);
    uint32_t token = read_data;
    bool z = (~(token >> pos)) & 0x01;

    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_TST1 (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    uint32_t reg2 = OperandExtract (opcode, 15, 11);
#endif // SUPPORT_SC_HEAP
    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemExceptionTST (addr) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
            return;
        }
        is_mdp_suppressed = true;
    }

    uint32_t token;
    if (is_mdp_suppressed == false) {
        token = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_OFF);
    } else {
        token = 0;
        m_trace->SetCancel (true);
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
        m_lsu->MemReadMdp (addr, SIZE_8BIT, Lsu::SPCLD_OFF);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
    }

#ifdef SUPPORT_SC_HEAP
    m_rmw_buf = token;
    if (is_mdp_suppressed == true) {
        SetPswZ (0);
    }
#else // SUPPORT_SC_HEAP
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;

    if (is_mdp_suppressed == false) {
        SetPswZ (z);
    } else {
        SetPswZ (0);
    }
#endif // SUPPORT_SC_HEAP
}


#ifdef SUPPORT_SC_HEAP
void InstV850::Inst_TST1_MW (uint64_t opcode, uint32_t read_data)
{
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t token = read_data;
    uint32_t v2  = GRegRead (reg2);
    uint32_t pos = v2 & 0x00000007;
    bool z = (~(token >> pos)) & 0x01;

    SetPswZ (z);
}
#endif // SUPPORT_SC_HEAP


void InstV850::Inst_XOR (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    uint32_t v2  = GRegRead (reg2);
    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v2 ^ v1;

    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_XORI (uint64_t opcode)
{
    uint32_t reg1  = OperandExtract (opcode,  4,  0);
    uint32_t reg2  = OperandExtract (opcode, 15, 11);
    uint32_t imm16 = OperandExtract (opcode, 31, 16);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 ^ imm16;

    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


void InstV850::Inst_ZXB (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 & 0x000000FF;

    GRegWrite (reg1, res);
}


void InstV850::Inst_ZXH (uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);

    uint32_t v1  = GRegRead (reg1);
    uint32_t res = v1 & 0x0000FFFF;

    GRegWrite (reg1, res);
}


void InstV850::Inst_CLIP_B(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    RegData v1 = GRegRead (reg1);
    RegData res = ClipCommon<SIZE_8BIT, CLIP_SIGN>(v1);

    GRegWrite (reg2, res);

    SetPswCY (0);
    SetPswOV (res != v1);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
    if (res != v1) {
        SetPswSAT (1);
    } else {
        // Keep current value.
        // Although value isn't changed, write record is necessary for CompRunner.
        SetPswSAT (GetPswSAT());
    }
}


void InstV850::Inst_CLIP_BU(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    RegData v1 = GRegRead (reg1);
    RegData res = ClipCommon<SIZE_8BIT, CLIP_UNSIGN>(v1);

    GRegWrite (reg2, res);

    SetPswCY (0);
    SetPswOV (res != v1);
    SetPswS  (0);
    SetPswZ  (res == 0);
    if (res != v1) {
        SetPswSAT (1);
    } else {
        // Keep current value.
        // Although value isn't changed, write record is necessary for CompRunner.
        SetPswSAT (GetPswSAT());
    }
}


void InstV850::Inst_CLIP_H(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    RegData v1 = GRegRead (reg1);
    RegData res = ClipCommon<SIZE_16BIT, CLIP_SIGN>(v1);

    GRegWrite (reg2, res);

    SetPswCY (0);
    SetPswOV (res != v1);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
    if (res != v1) {
        SetPswSAT (1);
    } else {
        // Keep current value.
        // Although value isn't changed, write record is necessary for CompRunner.
        SetPswSAT (GetPswSAT());
    }
}


void InstV850::Inst_CLIP_HU(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg2 = OperandExtract (opcode, 15, 11);

    RegData v1 = GRegRead (reg1);
    RegData res = ClipCommon<SIZE_16BIT, CLIP_UNSIGN>(v1);

    GRegWrite (reg2, res);

    SetPswCY (0);
    SetPswOV (res != v1);
    SetPswS  (0);
    SetPswZ  (res == 0);
    if (res != v1) {
        SetPswSAT (1);
    } else {
        // Keep current value.
        // Although value isn't changed, write record is necessary for CompRunner.
        SetPswSAT (GetPswSAT());
    }
}


template <ByteSize size, InstV850::ClipSignType is_sign>
RegData InstV850::ClipCommon (RegData data)
{
    RegData ret;
    if (size == SIZE_8BIT) {
        if (is_sign == CLIP_SIGN) {
            if ((data > 0x0000007FUL) && (data <= 0x7FFFFFFFUL)) {
                ret = 0x0000007FUL;
            } else if ((data >= 0x80000000UL) && (data < 0xFFFFFF80UL)) {
                ret = 0xFFFFFF80UL;
            } else {
                ret = data;
            }
        } else {
            if (data > 0x000000FFUL) {
                ret = 0x000000FFUL;
            } else {
                ret = data;
            }
        }
    } else if (size == SIZE_16BIT) {
        if (is_sign == CLIP_SIGN) {
            if ((data > 0x00007FFFUL) && (data <= 0x7FFFFFFFUL)) {
                ret = 0x00007FFFUL;
            } else if ((data >= 0x80000000UL) && (data < 0xFFFF8000UL)) {
                ret = 0xFFFF8000UL;
            } else {
                ret = data;
            }
        } else {
            if (data > 0x0000FFFFUL) {
                ret = 0x0000FFFFUL;
            } else {
                ret = data;
            }
        }
    } else {
        // Unsupported size
        ret = 0;
    }

    return ret;
}


void InstV850::Inst_LDLBU(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1 = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK
    // Byte access doesn't need call this. But this is common function, call IsMemExceptionAtomic.
    if (unlikely (m_lsu->IsMemExceptionAtomic<SIZE_8BIT> (addr) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_UNSIGN, DATA_READ);
        return;
    }
    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemReadWithLink (addr, SIZE_8BIT);
    GRegWrite (reg3, res);
}


void InstV850::Inst_LDB_INC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 8);
    GRegWrite (reg3, s_res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 + 1U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.B(+)\n");
    }
}


void InstV850::Inst_LDBU_INC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 + 1U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.BU(+)\n");
    }
}


void InstV850::Inst_LDB_DEC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 8);
    GRegWrite (reg3, s_res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 - 1U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.B(-)\n");
    }
}


void InstV850::Inst_LDBU_DEC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);
    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_8BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 - 1U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.BU(-)\n");
    }
}


void InstV850::Inst_STCB(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;
    uint32_t v3   = GRegRead (reg3);

    const ByteSize size = SIZE_8BIT;

#ifdef ENABLE_DBG_BREAK
    // SDB occurs despite LLbit=0.
    if (unlikely (m_lsu->IsHitWriteBrk (addr, size) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    // caxi/ldl.w/stc.wではMCTL.MA=1でもMAEは抑止されないので、最初にMAEをチェック
    if (unlikely (m_lsu->IsMemExceptionAtomic<size> (addr) == true)) {
        SetMemErrorInfo (addr, reg3, size, MA_SIGN, DATA_WRITE);
        return;
    }
    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, size) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, size, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
    }

    if ((m_lsu->IsHaveLink (addr, size) == true) && (is_mdp_suppressed == false)) {
        m_lsu->MemWrite (addr, v3 & 0xFF, size, true);
        if (m_trace->IsCancel()) {
            GRegWrite (reg3, 0x00U);
        } else {
            GRegWrite (reg3, 0x01U);
        }
    } else {
        GRegWrite (reg3, 0x00U);
    }

    // Delete LLBit
    m_lsu->DeleteOwnLink ();
}


void InstV850::Inst_STB_INC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;
    uint32_t v3   = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v3 & 0x00FF, SIZE_8BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v3 & 0x00FF, SIZE_8BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    GRegWrite (reg1, v1 + 1U);
}


void InstV850::Inst_STB_DEC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;
    uint32_t v3   = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_8BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_8BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_8BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_8BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v3 & 0x00FF, SIZE_8BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v3 & 0x00FF, SIZE_8BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    GRegWrite (reg1, v1 - 1U);
}


void InstV850::Inst_LDLHU(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1 = GRegRead (reg1);
    PhysAddr addr = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    // caxi/ldl.w/stc.wではMCTL.MA=1でもMAEは抑止されないので、最初にMAEをチェック
    if (unlikely (m_lsu->IsMemExceptionAtomic<SIZE_16BIT> (addr) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }
    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_8BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemReadWithLink (addr, SIZE_16BIT);
    GRegWrite (reg3, res);
}


void InstV850::Inst_LDH_INC(uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t v1      = GRegRead (reg1);
    PhysAddr addr    = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 16);
    GRegWrite (reg3, s_res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 + 2U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.H(+)\n");
    }
}


void InstV850::Inst_LDHU_INC(uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t v1     = GRegRead (reg1);
    PhysAddr addr   = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 + 2U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.HU(+)\n");
    }
}


void InstV850::Inst_LDH_DEC(uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t v1      = GRegRead (reg1);
    PhysAddr addr    = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res   = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    uint32_t s_res = SignExtend (res, 16);
    GRegWrite (reg3, s_res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 - 2U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.H(-)\n");
    }
}


void InstV850::Inst_LDHU_DEC(uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t v1     = GRegRead (reg1);
    PhysAddr addr   = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_16BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_UNSIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_16BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 - 2U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.HU(-)\n");
    }
}


void InstV850::Inst_STCH(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;
    uint32_t v3   = GRegRead (reg3);

    const ByteSize size = SIZE_16BIT;

#ifdef ENABLE_DBG_BREAK
    // SDB occurs despite LLbit=0.
    if (unlikely (m_lsu->IsHitWriteBrk (addr, size) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    // caxi/ldl.w/stc.wではMCTL.MA=1でもMAEは抑止されないので、最初にMAEをチェック
    if (unlikely (m_lsu->IsMemExceptionAtomic<size> (addr) == true)) {
        SetMemErrorInfo (addr, reg3, size, MA_SIGN, DATA_WRITE);
        return;
    }
    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, size) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, size, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
    }

    if ((m_lsu->IsHaveLink (addr, size) == true) && (is_mdp_suppressed == false)) {
        m_lsu->MemWrite (addr, v3 & 0xFFFF, size, true);
        if (m_trace->IsCancel()) {
            GRegWrite (reg3, 0x00U);
        } else {
            GRegWrite (reg3, 0x01U);
        }
    } else {
        GRegWrite (reg3, 0x00U);
    }

    // Delete LLBit
    m_lsu->DeleteOwnLink ();
}


void InstV850::Inst_STH_INC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;
    uint32_t v3   = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_16BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_16BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v3 & 0x0000FFFF, SIZE_16BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v3 & 0x0000FFFF, SIZE_16BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    GRegWrite (reg1, v1 + 2U);
}


void InstV850::Inst_STH_DEC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1   = GRegRead (reg1);
    PhysAddr addr = v1;
    uint32_t v3   = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_16BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_16BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_16BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_16BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v3 & 0x0000FFFF, SIZE_16BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v3 & 0x0000FFFF, SIZE_16BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    GRegWrite (reg1, v1 - 2U);
}


void InstV850::Inst_LDW_INC(uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t v1     = GRegRead (reg1);
    PhysAddr addr   = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 + 4U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.W(+)\n");
    }
}


void InstV850::Inst_LDW_DEC(uint64_t opcode)
{
    uint32_t reg1    = OperandExtract (opcode,  4,  0);
    uint32_t reg3    = OperandExtract (opcode, 31, 27);
    uint32_t v1     = GRegRead (reg1);
    PhysAddr addr   = v1;

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitReadBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    if (unlikely (m_lsu->IsMemException<DATA_READ> (addr, SIZE_32BIT) == true)) {
        SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_READ);
        return;
    }

    uint32_t res = m_lsu->MemRead (addr, SIZE_32BIT, Lsu::SPCLD_ON);
    GRegWrite (reg3, res); // Load target register must be recorded at last-1 for SC-HEAP.
    GRegWrite (reg1, v1 - 4U);

    if (reg3 == reg1) {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning detected: reg3=reg1 is unpermitted on LD.W(-)\n");
    }
}


void InstV850::Inst_STW_INC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1     = GRegRead (reg1);
    PhysAddr addr   = v1;
    uint32_t v3     = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v3, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v3, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    GRegWrite (reg1, v1 + 4U);
}


void InstV850::Inst_STW_DEC(uint64_t opcode)
{
    uint32_t reg1 = OperandExtract (opcode,  4,  0);
    uint32_t reg3 = OperandExtract (opcode, 31, 27);

    uint32_t v1     = GRegRead (reg1);
    PhysAddr addr   = v1;
    uint32_t v3     = GRegRead (reg3);

#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT) == true)) {
        return;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
    if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if (m_trace->GetExpCode () != EXP_NONE) {
            SetMemErrorInfo (addr, reg3, SIZE_32BIT, MA_SIGN, DATA_WRITE);
            return;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }

    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, v3, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, v3, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    GRegWrite (reg1, v1 - 4U);
}


bool InstV850::IsPswCondMatch (uint32_t cc4)
{
    bool c;

    switch (cc4) {
    case 0xF: c = ! ((GetPswS() ^ GetPswOV()) | GetPswZ()); break;  // GT
    case 0xE: c = !  (GetPswS() ^ GetPswOV());              break;  // GE
    case 0xD: c =     GetPswSAT();                          break;  // SA
    case 0xC: c = !   GetPswS();                            break;  // NS/P
    case 0xB: c = !  (GetPswCY() | GetPswZ());              break;  // H
    case 0xA: c = !   GetPswZ();                            break;  // NZ
    case 0x9: c = !   GetPswCY();                           break;  // NC/NL
    case 0x8: c = !   GetPswOV();                           break;  // NV
    case 0x7: c =    (GetPswS() ^ GetPswOV()) | GetPswZ();  break;  // LE
    case 0x6: c =     GetPswS() ^ GetPswOV();               break;  // LT
    case 0x5: c =     true;                                 break;  // T
    case 0x4: c =     GetPswS();                            break;  // S/N
    case 0x3: c =     GetPswCY() | GetPswZ();               break;  // NH
    case 0x2: c =     GetPswZ();                            break;  // Z
    case 0x1: c =     GetPswCY();                           break;  // C/L
    case 0x0: c =     GetPswOV();                           break;  // V
    default:  c =     false;
        CfMsg::TPrint (MSG_ERR, m_trace, "<Internal Error: Unreachable code>\n");
        break;
    }

    return c;
}


void InstV850::SaturateAdd (uint32_t x, uint32_t y, uint32_t dst_reg)
{
    uint32_t res = x + y;

    bool ovp = IsOvflwAddPlus  (x, y, res);
    bool ovm = IsOvflwAddMinus (x, y, res);
    if (ovp) {
        res = 0x7FFFFFFF;
    } else if (ovm) {
        res = 0x80000000;
    }

    GRegWrite (dst_reg, res);

    SetPswCY (IsCarryAdd (x, y, res));
    SetPswOV (ovp | ovm);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);

    if (ovp | ovm) {
        SetPswSAT (1);
    } else {
        // Keep current value.
        // Although value isn't changed, write record is necessary for CompRunner.
        SetPswSAT (GetPswSAT());
    }
}


void InstV850::SaturateSub (uint32_t x, uint32_t y, uint32_t dst_reg)
{
    uint32_t res = x - y;

    bool ovp = IsOvflwSubPlus  (x, y, res);
    bool ovm = IsOvflwSubMinus (x, y, res);
    if (ovp) {
        res = 0x7FFFFFFF;
    } else if (ovm) {
        res = 0x80000000;
    }

    GRegWrite (dst_reg, res);

    SetPswCY (IsCarrySub (x, y));
    SetPswOV (ovp | ovm);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);

    if (ovp | ovm) {
        SetPswSAT (1);
    } else {
        // Keep current value.
        // Although value isn't changed, write record is necessary for CompRunner.
        SetPswSAT (GetPswSAT());
    }
}


InstV850::PrepResult InstV850::PrepareStack (uint32_t list12, uint32_t imm5)
{
    static const PhysAddr mask = ~(PhysAddr(0x03));
    RegData  sp   = GRegRead (R_SP);
    PhysAddr addr = sp;

    // list12は降順にソート済み
    // list12[11:0]がレジスタ r24,r25,...,r29,r30,r31に対応
#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)
    for (uint32_t regno = R20; regno <= R31; ++regno) {
#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                m_trace->SetCancelMcyc (true);
                return PREP_NG; // cancel by async event (EIINT)
            }
#endif //#if defined(COMP_RUNNER)

        // スタックストアは昇順 (list12の下位からストア)
        if (list12 & 0x0800) {
            addr -= 4;
            uint32_t val = GRegRead (regno);

#ifdef ENABLE_DBG_BREAK
            if (unlikely (m_lsu->IsHitWriteBrk (addr & mask, SIZE_32BIT) == true)) {
                return PREP_NG;
            }
            if (unlikely (m_lsu->IsAlignmentError (addr) == true)) {
                m_srf->SrDIR0()->SetAT (1);
                return PREP_NG;
            }
#endif // ENABLE_DBG_BREAK

            bool is_mdp_suppressed = false;
            if (unlikely (m_lsu->IsMemException<DATA_WRITE> (addr & mask, SIZE_32BIT) == true)) {
                if (m_trace->GetExpCode () != EXP_NONE) {
                    SetMemErrorInfo (addr & mask, regno, SIZE_32BIT, MA_SIGN, DATA_WRITE);
                    return PREP_NG;
                }
                is_mdp_suppressed = true;
                m_lsu->DeleteOwnLink (addr & mask, SIZE_32BIT);
            }
            if (is_mdp_suppressed == false) {
#if defined(COMP_RUNNER)
                //if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                //    m_trace->SetCancelMcyc (true);
                //    return PREP_NG; // cancel by synchronous events (MDP, LSAB, etc)
                //}
                ++ completed_count;
#endif //#if defined(COMP_RUNNER)
                m_lsu->MemWrite (addr & mask, val, SIZE_32BIT);
            }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
            else {
                m_lsu->MemWriteMdp (addr & mask, val, SIZE_32BIT);
            }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
#ifdef ENABLE_DBG_BREAK
            // Apply EO bit because access is done
            m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
        }
        list12 <<= 1;
    }

#if defined(COMP_RUNNER)
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        m_trace->SetCancelMcyc (true);
        return PREP_NG;
    }
#endif //#if defined(COMP_RUNNER)

    GRegWrite (R_SP, addr - (imm5 << 2));

    return PREP_OK;
}


RegData InstV850::DisposeStack (uint32_t list12, uint32_t imm5, bool *exception)
{
    static const PhysAddr mask = ~(PhysAddr(0x03));
    RegData  sp   = GRegRead (R_SP);
    PhysAddr addr = sp + (imm5 << 2);
    *exception = false;

#if defined(COMP_RUNNER)
    int32_t completed_count = 0;
#endif //#if defined(COMP_RUNNER)
    // list12は降順にソート済み
    // list12[11:0]がレジスタ r24,r25,...,r29,r30,r31に対応
    for (uint32_t regno = R31; regno >= R20; --regno) {
        // スタックロードは降順 (list12の下位からロード)
        if (list12 & 0x001) {
#if defined(COMP_RUNNER)
            if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
                // cancel by async event (EIINT)
                *exception = true;
                m_trace->SetCancelMcyc (true);
                return sp;
            }
#endif //#if defined(COMP_RUNNER)
#ifdef ENABLE_DBG_BREAK
            if (unlikely (m_lsu->IsHitReadBrk (addr & mask, SIZE_32BIT) == true)) {
                *exception = true;
                return sp;
            }
            if (unlikely (m_lsu->IsAlignmentError (addr) == true)) {
                m_srf->SrDIR0()->SetAT (1);
                *exception = true;
                return sp;
            }
#endif // ENABLE_DBG_BREAK

            if (unlikely (m_lsu->IsMemException<DATA_READ> (addr & mask, SIZE_32BIT) == true)) {
                SetMemErrorInfo (addr & mask, regno, SIZE_32BIT, MA_SIGN, DATA_READ);
                *exception = true;
                return sp;
            }
#if defined(COMP_RUNNER)
            //if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
            //    // cancel by synchronous events (MDP, LSAB, etc)
            //    m_trace->SetCancelMcyc (true);
            //    *exception = true;
            //    return sp;
            //}
            ++ completed_count;
#endif //#if defined(COMP_RUNNER)
            uint32_t val = m_lsu->MemRead (addr & mask, SIZE_32BIT, Lsu::SPCLD_ON);
            GRegWrite (regno, val);
            addr += 4;
#ifdef ENABLE_DBG_BREAK
            // Apply EO bit because access is done
            m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
        }
        list12 >>= 1;
    }

#if defined(COMP_RUNNER)
    if (completed_count >= m_fsctrl->GetCancelTimingMcycInst ()) {
        // cancel by async event (EIINT)
        *exception = true;
        m_trace->SetCancelMcyc (true);
        return sp;
    }
#endif //#if defined(COMP_RUNNER)

    return addr;
}


uint32_t InstV850::MTypeAddressing (uint32_t reg1, uint32_t reg2, uint32_t* next_addr)
{
    uint32_t v1 = GRegRead (reg1);
    uint32_t v2 = GRegRead (reg2);

    uint32_t v2_hi = v2 >> 16;
    uint32_t v2_lo = v2 & 0x0000FFFF;
    PhysAddr addr  = v1 + v2_lo;

    int32_t step  = static_cast<int32_t> (SignExtend (v2_hi, 16));
    int32_t index = static_cast<int32_t> (v2_lo);
    int32_t limit = GRegRead (reg2 + 1) & 0x0000FFFF;

    int32_t next_index;
    if (abs(step) <= limit) {
        if ((index + step) < 0) {
            next_index = (index + step) + (limit + 1);
        } else {
            next_index = (index + step) % (limit + 1);
        }
    } else {
        next_index = 0x0000;
    }

    uint32_t next_v2 = (v2 & 0xFFFF0000) | (next_index & 0x0000FFFFU);
    *next_addr = next_v2;

    return (addr);
}


uint32_t InstV850::RTypeAddressing (uint32_t reg1, uint32_t reg2, uint32_t *next_v2)
{
    uint32_t v2 = GRegRead (reg2);
    uint32_t v1 = GRegRead (reg1);

    int32_t v2_hi = v2 >> 16;
    int32_t v2_lo = v2 & 0x0000FFFF;

    // indexの下位16bitを bit反転
    uint32_t tmp = v2_lo;
    tmp = ((tmp & 0x00005555) << 1) | ((tmp & 0x0000AAAA) >> 1);
    tmp = ((tmp & 0x00003333) << 2) | ((tmp & 0x0000CCCC) >> 2);
    tmp = ((tmp & 0x00000F0F) << 4) | ((tmp & 0x0000F0F0) >> 4);
    tmp = ((tmp & 0x000000FF) << 8) | ((tmp & 0x0000FF00) >> 8);
    PhysAddr addr = v1 + tmp;

    int32_t step  = static_cast<int32_t> (SignExtend (v2_hi, 16));
    int32_t index = static_cast<int32_t> (v2_lo);

    uint32_t next_index = (index + step) & 0x0000FFFF;
    *next_v2 = (v2 & 0xFFFF0000) | next_index;

    return (addr);
}


bool InstV850::PushRegBody (PhysAddr addr, RegData data)
{
#ifdef ENABLE_DBG_BREAK
    if (unlikely (m_lsu->IsHitWriteBrk (addr, SIZE_32BIT, LSAB_EIINT) == true)) {
        // Set erro code to trace in IsHitWriteBrk()
        return false;
    }
#endif // ENABLE_DBG_BREAK

    bool is_mdp_suppressed = false;
#ifdef ENABLE_RB_MDP
    if (unlikely (m_lsu->IsMemExceptionEI<DATA_WRITE> (addr, SIZE_32BIT) == true)) {
        if ((m_trace->GetExpCode () != EXP_NONE) && (m_trace->GetExpCode () != EXP_EIINT)) {
            SetMemErrorInfo (addr, R0, SIZE_32BIT, MA_SIGN, DATA_WRITE, MEI_EIINT_REGBANK);
            return false;
        }
        is_mdp_suppressed = true;
        m_lsu->DeleteOwnLink (addr, SIZE_32BIT);
    }
#endif // #ifdef ENABLE_RB_MDP
    if (is_mdp_suppressed == false) {
        m_lsu->MemWrite (addr, data, SIZE_32BIT);
    }
#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    else {
        m_lsu->MemWriteMdp (addr, data, SIZE_32BIT);
    }
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)
#ifdef ENABLE_DBG_BREAK
    // Apply EO bit because access is done
    m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
    return true;
}



bool InstV850::PushRegBank (uint32_t bank_pos, uint32_t psw_data)
{
    bool push_mode = m_srf->SrRBCR0 ()->GetMD ();
    PhysAddr rbip = m_srf->SrRBIP ()->ReadBody ();
    PhysAddr target_addr;
    PhysAddr size;
    if (push_mode == 0) {
        size = 0x60;
    } else {
        size = 0x90;
    }
    target_addr = rbip - (bank_pos * size);
    target_addr -= 4;
    if (PushRegBody (target_addr, GRegReadPC ()) == false) {
        return false;
    }

    target_addr -= 4;
    // PSW.UM is updated to 0 temporally, current PSW value (=SRegRead (CF_SR_PSW)) can't be used.
    if (PushRegBody (target_addr, psw_data) == false) {
        return false;
    }
    target_addr -= 4;
    if (PushRegBody (target_addr, SRegRead (CF_SR_VEIIC)) == false) {
        return false;
    }
    target_addr -= 4;
    if (PushRegBody (target_addr, SRegRead (CF_SR_FPSR)) == false) {
        return false;
    }
    for (CReg regno = R1; regno <= R19; regno++) {
        target_addr -= 4;
        if (PushRegBody (target_addr, GRegRead (regno)) == false) {
            return false;
        }
    }
    if (push_mode == 0) {
        target_addr -= 4;
        if (PushRegBody (target_addr, GRegRead (R30)) == false) {
            return false;
        }
    } else {
        for (CReg regno = R20; regno <= R31; regno++) {
            target_addr -= 4;
            if (PushRegBody (target_addr, GRegRead (regno)) == false) {
                return false;
            }
        }
    }
    return true;
}


bool InstV850::IsHostTransitionException (uint32_t exp_code) const
{
    if ((m_fsctrl->m_pe->IsHvEnabled () == false) || (m_nc_srf->SrPSWH ()->GetGM () == 0)) {
        // Goto host mode when virtualization is disabled or PE is already on Host mode.
        return true;
    }

    switch (exp_code) {
    case EXP_RESET:         // リセット
    case EXP_DBNMI:         // DB: デバッグNMI割り込み
    case EXP_RLB:           // DB: リレー・ブレーク
    case EXP_DBINT:         // DB: デバッグ割り込み
    case EXP_FENMI:         // FENMI割り込み
    case EXP_FEINT:         // FEINT割り込み
    case EXP_SYSERR_HM:     // SYSERR (caused by Host)
    case EXP_BGFEINT:       // BGFEINT
    case EXP_EIINT:         // ユーザ割り込み
    case EXP_BGEIINT:       // BGEIINT
    case EXP_LSAB_EIINT:    // DB: ロード／ストア・アドレス・ブレーク @ EIINT
    case EXP_PCB_DB:        // DB: PCブレーク
    case EXP_LSAB_DB:       // DB: ロード／ストア・アドレス・ブレーク
    case EXP_AE:            // DB: アライメント・エラー
    case EXP_DBTRAP:        // デバッグ・トラップ
    case EXP_HVTRAP:        // HVTRAP
    case EXP_MDPHM_EIINT: // MDP(Host) @ EIINT by Host managed entry
    case EXP_MDPGM_EIINT: // MDP(Guest) @ EIINT by Guest managed entry
        return true; // Goto host always
        break;

    case EXP_MDPHM_GMEIINT:   // MDP(Host) @ GMEIINT by Host managed entry
    case EXP_MIPHM:           // メモリ保護例外 (実行権)
    case EXP_MDPHM:           // メモリ保護例外 (アクセス権)
        // check HMP
        return (m_srf->SrGMCFG ()->GetHMP () == 1);
        break;

    case EXP_MDPGM_GMEIINT:   // MDP(Guest) @ GMEIINT by Guest managed entry
    case EXP_MIPGM:           // メモリ保護例外 (実行権)
    case EXP_MDPGM:           // メモリ保護例外 (アクセス権)
        // check GMP
        return (m_srf->SrGMCFG ()->GetGMP () == 1);
        break;

    case EXP_SYSERR_GM:     // SYSERR (caused by Guest)
    case EXP_SYSERR_RB:     // System error (RB on EIINT)
    case EXP_SYSERR_IF:     // システム・エラー (フェッチ時)
    case EXP_SYSERR_EX:     // System error reexec type (Execute)
        // Check GSYSE
        return (m_srf->SrGMCFG ()->GetGSYSE () == 1);
        break;

    // case EXP_NONE:          // 正常処理
    // case EXP_RIE:           // 予約命令例外
    // case EXP_UCPOP0:        // コプロセッサ使用不可例外 (FPU)
    // case EXP_UCPOP1:        // コプロセッサ使用不可例外 (SIMD)
    // case EXP_UCPOP2:        // コプロセッサ使用不可例外 (未定義)
    // case EXP_PIE:           // 特権命令例外
    // case EXP_MAE:           // メモリ・アラインメント例外
    // case EXP_FPE:           // FPU例外 (プレサイス)
    // case EXP_FXE:           // FP-SIMD例外 (プレサイス)
    // case EXP_SYSCALL:       // システム・コール
    // case EXP_FETRAP:        // FEレベル・トラップ
    // case EXP_TRAP0:         // EIレベル・トラップ0
    // case EXP_TRAP1:         // EIレベル・トラップ1
    // case EXP_GUARD:         // 例外コード用ガードビット
    // case EXP_GMFEINT:       // GMFEINT
    // case EXP_GMEIINT:       // GMEIINT
    default:
        // No transition mode
        return false; // Goto guest (if host mode already, returned true by 1st condition.)
    }
}


void InstV850::UpdatePswReg (void)
{
    m_psw = m_srf->SrVPSW ();
}


void InstV850::ExceptionEI (uint32_t exp_cause, PhysAddr ret_pc,
                            uint32_t hndl_ofst, bool is_excp, bool is_table)
{
    // Read PSW, PSWH on current Host/Guest
    uint32_t ret_rb_psw = m_srf->SrVPSW ()->ReadBody ();
    uint32_t ret_psw_to_eipsw = ret_rb_psw;
    uint32_t ret_pswh;
    // In G4MH 2.0, there are 6 bits for bank position.
    // If RBNR.BN = 63, there is no more bank slot.
    uint32_t bank_pos_max_g4mh20 = 63;

    uint32_t intcfg_ulnr = 0;
    if ((m_fsctrl->m_pe->GetCoreType () == CORE_G4MH) && (m_fsctrl->m_pe->GetCoreVersion () >= CORE_VER_20)) {
        // Read INTCFG.ULNR for current mode.
        intcfg_ulnr = m_srf->SrVINTCFG ()->GetULNR ();
    }

    bool is_trans_to_host = IsHostTransitionException (m_trace->GetExpCode ());
    bool gm_tmp;
    if (m_fsctrl->m_pe->IsHvEnabled () == true) {
        ret_pswh = m_srf->SrPSWH ()->ReadBody ();
        gm_tmp = m_srf->SrPSWH ()->GetGM ();
        if (is_trans_to_host) {
            m_srf->SrPSWH ()->SetGM (0);
            // This is temporal updating. Don't record PSWH to trace.

            // If change to host mode, HMPSW is stored to EIPSW.
            // Becasue GMPSW is kept in GMPSW when change to host mode, GMPSW isn't stored to EIPSW.
            ret_psw_to_eipsw = m_nc_srf->SrHMPSW ()->ReadBody ();
        }
    } else {
        ret_pswh = 0;
        gm_tmp = 0;
    }

    bool is_regbank_usable_mode;
    if ((m_fsctrl->m_pe->IsHvEnabled () == false) || (is_trans_to_host == false)) {
        is_regbank_usable_mode = true;
    } else {
        is_regbank_usable_mode = false;
    }

    bool is_pswid1 = true;
    bool use_rb = false;
    uint32_t bank_pos;
    PhysAddr next_pc = hndl_ofst;
    if (m_trace->GetExpCode() == EXP_SYSCALL) {
        // SYSCALL では各命令でオフセットを加算済み
        next_pc = hndl_ofst;
    } else {
        if (is_table == false) {
            // 直接ベクタ方式
            PhysAddr base_addr;
            if (m_psw->GetEBV () == 0) {
                base_addr = m_srf->SrRBASE()->GetRBASE() << 9;
            } else {
                base_addr = m_srf->SrVEBASE()->GetEBASE() << 9;
            }
            next_pc = base_addr + hndl_ofst;
        } else {
            // Table ref EIINT
            //  1. Check SYSERR by RB
            //  2. Check LSAB on table access
            //  3. Check MDP on table access
            //  4. Check LSAB on RB
            //  5. Check MDP on RB
            //  6. If there is no exceptions, EI exception is processed.

            // Check SYSERR by RB
            uint32_t pri = m_fsctrl->GetEiintPriority();
            if (pri >= 16) {
                // pri16 and higher use RBCR0.BE, RBCR1.NC for pri15
                pri = 15;
            }
            if (   (is_excp == false)
                && (((m_srf->SrRBCR0()->GetBE () >> pri) & 0x1) == 1)
                && (is_regbank_usable_mode == true) ) {
                bank_pos = m_srf->SrRBNR ()->GetBN ();
                if ((m_fsctrl->m_pe->GetCoreType () == CORE_G4MH) && (m_fsctrl->m_pe->GetCoreVersion () >= CORE_VER_20)) {
                    if ((bank_pos > intcfg_ulnr) || (bank_pos == bank_pos_max_g4mh20)) {
                        // Cause Async SYSERR by overflow
                        m_trace->SetExpCode(EXP_SYSERR_RB);
                    }
                } else {
                    if (bank_pos > 15) {
                        // Cause Async SYSERR by overflow
                        m_trace->SetExpCode(EXP_SYSERR_RB);
                    }
                }
            }

            if (m_trace->GetExpCode() != EXP_SYSERR_RB) {
                // Table access
                PhysAddr table_addr = (m_srf->SrVINTBP()->GetINTBP() << 9) + hndl_ofst;

                bool um_tmp = m_psw->GetUM ();
                m_psw->SetUM (0);

#ifdef ENABLE_DBG_BREAK
                // Check LSAB on table access
                if (unlikely (m_lsu->IsHitReadBrk (table_addr, SIZE_32BIT, LSAB_EIINT) == true)) {
                    // Set LSAB event to trace in IsHitReadBrk().
                } else
#endif // ENABLE_DBG_BREAK
                {
                    // Check MDP on table access
                    // - Because SV priviledge is used on tabel access and register bank access,
                    //   SV is set temporally.
                    //bool um_tmp = m_psw->GetUM ();
                    //m_psw->SetUM (0);
                    bool is_mem_excp = m_lsu->IsMemExceptionEI<DATA_READ> (table_addr, SIZE_32BIT);
                    if (is_mem_excp == false) {
                        next_pc = m_lsu->MemRead (table_addr, SIZE_32BIT, Lsu::SPCLD_ON);
#ifdef ENABLE_DBG_BREAK
                        // Apply EO bit because access is done
                        m_lsu->ApplyLsabEoReserved (m_trace);
#endif // #ifdef ENABLE_DBG_BREAK
                        // In case of no MDP/LSAB on table read, start to push Register bank.
                        // Check LSAB/MDP on RB
                        if (   (is_excp == false)
                            && (((m_srf->SrRBCR0()->GetBE () >> pri) & 0x1) == 1)
                            && (is_regbank_usable_mode == true) ) {
                            bank_pos = m_srf->SrRBNR ()->GetBN ();
                            use_rb = PushRegBank (bank_pos, ret_rb_psw);
                            if ((use_rb == true) && (((m_srf->SrRBCR1()->GetNC () >> pri) & 0x1) == 0)) {
                                is_pswid1 = false;
                            }
                        }
                    } else {
                        // MDP on Table reference
                        SetMemErrorInfo (table_addr, R0, SIZE_32BIT, MA_SIGN, DATA_READ, MEI_EIINT_TABLE);
                    }
                }
                m_psw->SetUM (um_tmp);
            }
        }
    }

    // If syserr detected by RBNR.BN overflow,
    // EIPC and EIPSW aren't updated.
    // EIPC/EIPSW Register resources are selected on post-exception mode.
    if (m_trace->GetExpCode() != EXP_SYSERR_RB) {
        if (m_fsctrl->m_pe->GetCoreType () == CORE_G4KH) {
            if ((m_trace->GetExpCode() != EXP_LSAB_EIINT) && (m_trace->GetExpCode() != EXP_MDPGM_EIINT)) {
                SRegWrite (CF_SR_VEIPC,  ret_pc);
                SRegWrite (CF_SR_VEIPSW, ret_psw_to_eipsw);
            }
        } else {
            SRegWrite (CF_SR_VEIPC,  ret_pc);
            SRegWrite (CF_SR_VEIPSW, ret_psw_to_eipsw);
        }
        if ((is_trans_to_host) && (m_fsctrl->m_pe->IsHvEnabled ())) {
            SRegWrite (CF_SR_EIPSWH, ret_pswh);
        }
    }
    // revival org value
    if (is_trans_to_host && m_fsctrl->m_pe->IsHvEnabled ()) {
        m_srf->SrPSWH ()->SetGM (gm_tmp);
    }

    m_trace->SetExpCause (exp_cause);

    // Check exception in table ref
    ExpCode exp_code = m_trace->GetExpCode ();
    if (ExpInfo::GetExpLevel (exp_code) != ExpInfo::EXP_LV_EI) {
        m_fsctrl->ExceptionProcess (exp_code); // Goto other exception
        return;
    }

    // Update register for accepting exceptions
    if ((is_trans_to_host) && m_fsctrl->m_pe->IsHvEnabled ()) {
        m_srf->SrPSWH ()->SetGM (0);
        m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_PSWH, m_srf->SrPSWH()->ReadBody());
    }
    SRegWrite (CF_SR_VEIIC, exp_cause);

    if (use_rb) {
        m_srf->SrRBNR ()->SetBN (bank_pos + 1);
        m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_RBNR, m_srf->SrRBNR()->ReadBody());
    }

    SetPswUM (0);
    if (is_pswid1) {
        SetPswID (1);
    } else {
        SetPswID (0);
    }
    SetPswEP (is_excp);

    if (   (m_trace->GetExpCode() == EXP_EIINT)
        || (m_trace->GetExpCode() == EXP_GMEIINT) ) {
        if (m_srf->SrVINTCFG()->GetEPL() == 1) {
            // update EIMASK
            SetPswEIMASK (m_fsctrl->GetEiintPriority());
        }
    }

    GRegWritePC (next_pc);
    m_trace->SetNextInstAddr (next_pc);
    if (is_excp == true) {
        m_trace->SetBranchInfo (EXP_BRANCH);
    } else {
        m_trace->SetBranchInfo (INT_BRANCH);
    }

    // 例外発生時、自スレッドのリンクをクリア
    // L1RAM領域/その他領域のLLBitを削除
    m_lsu->DeleteOwnLink ();

    // 受け付け条件のない例外では、
    // ほかのEIレベル例外処理中(PSW.NP==1 or PSW.ID==1)も受け付け
    // コンテキスト退避処理以前に発生した場合、元の復帰PC, PSWを
    // 破壊する可能性あり  @@ 要Warning発行
}


void InstV850::ExceptionFE (uint32_t exp_cause, PhysAddr ret_pc,
                            uint32_t hndl_ofst, bool is_excp)
{
    // Read PSW, PSWH on current Host/Guest mode
    uint32_t ret_psw_to_fepsw = m_srf->SrVPSW ()->ReadBody ();
    uint32_t ret_pswh;

    bool is_trans_to_host = IsHostTransitionException (m_trace->GetExpCode ());
    if (m_fsctrl->m_pe->IsHvEnabled () == true) {
        ret_pswh = m_srf->SrPSWH ()->ReadBody ();
        if (is_trans_to_host) {
            m_srf->SrPSWH ()->SetGM (0);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_PSWH, m_srf->SrPSWH()->ReadBody());

            // If change to host mode, HMPSW is stored to FEPSW.
            // Becasue GMPSW is kept in GMPSW when change to host mode, GMPSW isn't stored to FEPSW.
            ret_psw_to_fepsw = m_nc_srf->SrHMPSW ()->ReadBody ();
        }
    } else {
        ret_pswh = 0;
    }

    SRegWrite (CF_SR_VFEPC,  ret_pc);
    SRegWrite (CF_SR_VFEPSW, ret_psw_to_fepsw);
    if (is_trans_to_host && m_fsctrl->m_pe->IsHvEnabled ()) {
        SRegWrite (CF_SR_FEPSWH, ret_pswh);
    }

    ExpCode exp_code = m_trace->GetExpCode ();
    if (   exp_code == EXP_MIPHM || exp_code == EXP_MDPHM || exp_code == EXP_MDPHM_EIINT || exp_code == EXP_MDPHM_GMEIINT
        || exp_code == EXP_MIPGM || exp_code == EXP_MDPGM || exp_code == EXP_MDPGM_EIINT || exp_code == EXP_MDPGM_GMEIINT) {
        exp_cause = m_lsu->GetMpuPtr()->GetMpuExpCause ();
    } else if (exp_code == EXP_SYSERR_IF) {
        exp_cause = m_fsctrl->m_syserr_fetch_cause;
    }

    SRegWrite (CF_SR_VFEIC, exp_cause);
    m_trace->SetExpCause (exp_cause);

    SetPswUM (0);
    SetPswID (1);
    SetPswNP (1);
    SetPswEP (is_excp);

    PhysAddr base_addr;
    if (m_psw->GetEBV () == 0) {
        base_addr = m_srf->SrRBASE()->GetRBASE() << 9;
    } else {
        base_addr = m_srf->SrVEBASE()->GetEBASE() << 9;
    }
    PhysAddr next_pc = base_addr + hndl_ofst;
    GRegWritePC (next_pc);
    m_trace->SetNextInstAddr (next_pc);
    if (is_excp == true) {
        m_trace->SetBranchInfo (EXP_BRANCH);
    } else {
        m_trace->SetBranchInfo (INT_BRANCH);
    }

    // 例外発生時、自スレッドのリンクをクリア
    // L1RAM領域/その他領域のLLBitを削除
    m_lsu->DeleteOwnLink ();

    // 受け付け条件のない例外では、
    // ほかのEIレベル例外処理中(PSW.NP==1 or PSW.ID==1)も受け付け
    // コンテキスト退避処理以前に発生した場合、元の復帰PC, PSWを
    // 破壊する可能性あり  @@ 要Warning発行
}


void InstV850::ExceptionDB (uint32_t exp_cause, PhysAddr ret_pc,
                            uint32_t hndl_ofst, bool is_excp, uint32_t exp_code)
{
    // Read PSW, PSWH on current Host/Guest mode
    uint32_t ret_psw_to_dbpsw = m_srf->SrVPSW ()->ReadBody ();
    uint32_t ret_pswh;

    bool is_trans_to_host = IsHostTransitionException (m_trace->GetExpCode ());
    if (m_fsctrl->m_pe->IsHvEnabled () == true) {
        ret_pswh = m_srf->SrPSWH ()->ReadBody ();
        if (is_trans_to_host) {
            m_srf->SrPSWH ()->SetGM (0);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_PSWH, m_srf->SrPSWH()->ReadBody());

            // If change to host mode, HMPSW is stored to DBPSW.
            // Becasue GMPSW is kept in GMPSW when change to host mode, GMPSW isn't stored to DBPSW.
            ret_psw_to_dbpsw = m_nc_srf->SrHMPSW ()->ReadBody ();
        }
    } else {
        ret_pswh = 0;
    }

    SRegWrite (CF_SR_DBPC,  ret_pc);
    SRegWrite (CF_SR_DBPSW, ret_psw_to_dbpsw);
    if (is_trans_to_host && m_fsctrl->m_pe->IsHvEnabled ()) {
        SRegWrite (CF_SR_DBPSWH, ret_pswh);
    }
    SRegWrite (CF_SR_DBIC,  exp_cause);
    m_trace->SetExpCause (exp_cause);

    SetPswUM (0);
    SetPswID (1);
    SetPswNP (1);
    SetPswEP (is_excp);

    // Set PSW.DN and PSW.DM
    switch (exp_code) {
    case EXP_PCB_DB:
    case EXP_LSAB_DB:
    case EXP_AE:
    case EXP_DBTRAP:
        if (m_srf->SrDIR0()->GetDM() == 1) {
            m_srf->SrDIR0()->SetDN (1);
        } else {
            m_srf->SrDIR0()->SetDM (1);
        }
        break;
    case EXP_DBNMI:
        m_srf->SrDIR0()->SetDN (1);
        m_srf->SrDIR0()->SetDM (1);
        break;
    default:
        m_srf->SrDIR0()->SetDM (1);
        break;
    }
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_DIR0, m_srf->SrDIR0()->ReadBody());

    PhysAddr base_addr;
    if (m_psw->GetEBV () == 0) {
        base_addr = m_srf->SrRBASE()->GetRBASE() << 9;
    } else {
        base_addr = m_srf->SrVEBASE()->GetEBASE() << 9;
    }
    PhysAddr next_pc = base_addr + hndl_ofst;
    GRegWritePC (next_pc);
    m_trace->SetNextInstAddr (next_pc);
    if (is_excp == true) {
        m_trace->SetBranchInfo (EXP_BRANCH);
    } else {
        m_trace->SetBranchInfo (INT_BRANCH);
    }

    // DB例外ではリンク解除しない
}


void InstV850::SetMemErrorInfo (PhysAddr addr, CReg regid, ByteSize size,
                                MemAccessSign sign, MemAccessCommand cmd, MeiType mei_type)
{
    // MEI,MEAの更新は行わない
    ExpCode exp_code = m_trace->GetExpCode ();
    if (  exp_code == EXP_MAE || exp_code == EXP_MDPHM || exp_code == EXP_MDPHM_EIINT || exp_code == EXP_MDPHM_GMEIINT
       || exp_code == EXP_MDPGM || exp_code == EXP_MDPGM_EIINT || exp_code == EXP_MDPGM_GMEIINT) {
        uint32_t ds;
        switch (size) {
        case SIZE_8BIT:  ds = 0x0; break;
        case SIZE_16BIT: ds = 0x1; break;
        case SIZE_32BIT: ds = 0x2; break;
        case SIZE_64BIT: ds = 0x3; break;
        case SIZE_128BIT: ds = 0x4; break;
        default:            ds = 0x0; break;  // Unreachable
        }
        uint32_t inst_id = m_trace->GetInstId ();
        uint32_t itype;
        uint32_t len;
        // 割込み時のテーブル参照の場合、inst_idはINST_ID_V_0
        if (mei_type == MEI_EIINT_TABLE) {
            itype = 21; // 割り込み(テーブル参照方式)時のITYPE値
            len = 0;
        } else if (mei_type == MEI_EIINT_REGBANK) {
            itype = 22; // Push Regbank
            len = 0;
        } else {
            itype = MicroArchDB::GetMeiInstType (inst_id);
            len = MicroArchDB::GetInstSize (inst_id);
        }
        uint32_t rw  = (cmd == DATA_READ) ? 0x0 : 0x1;
        uint32_t mei =
            (len   << 28) |
            (regid << 16) |
            (ds    <<  9) |
            (sign  <<  8) |
            (itype <<  1) |
            (rw    <<  0);

        // Update MEI/MEA when the exception is accepted by host mode or conventional mode.
        // Update GMMEI/GMMEA when the exception is accepted by guest mode.
        bool accepted_by_host;
        if (m_fsctrl->m_pe->IsHvEnabled () == false) {
            // conventional mode
            accepted_by_host = true;
        } else {
            accepted_by_host = false;
            if (m_fsctrl->m_pe->IsHostMode ()) {
                accepted_by_host = true;
            }
            if (((exp_code == EXP_MDPHM) || (exp_code == EXP_MDPHM_GMEIINT)) && ((m_nc_srf->SrGMCFG()->GetHMP () == 1))) {
                accepted_by_host = true;
            }
            if (((exp_code == EXP_MDPGM || exp_code == EXP_MDPGM_GMEIINT)) && ((m_nc_srf->SrGMCFG()->GetGMP () == 1))) {
                accepted_by_host = true;
            }
            if ((exp_code == EXP_MDPHM_EIINT) || (exp_code == EXP_MDPGM_EIINT)) {
                accepted_by_host = true;
            }
        }
        if (accepted_by_host) {
            // Update MEI/MEA when the exception is accepted by host mode
            m_nc_srf->SrHMMEI()->WriteBody (mei);
            m_nc_srf->SrHMMEA()->WriteBody (addr);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_HMMEI, mei);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_HMMEA, addr);
        } else {
            // Update GMMEI/GMMEA when the exception is accepted by guest mode
            m_nc_srf->SrGMMEI()->WriteBody (mei);
            m_nc_srf->SrGMMEA()->WriteBody (addr);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_GMMEI, mei);
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_GMMEA, addr);
        }
    }
}


void InstV850::Inst_BINS_Common (uint32_t reg1, uint32_t reg2,
                                 uint32_t msb, uint32_t lsb)
{
    uint32_t v2    = GRegRead (reg2);
    uint32_t v1    = GRegRead (reg1);
    uint32_t width = msb - lsb + 1;
    uint32_t pos   = lsb;

    uint32_t res_hi = (pos+width > 31) ?  0 : BitExtract(v2, 31, pos+width) << (pos+width);
    uint32_t res_md = (width > 31)     ? v1 : BitExtract(v1, width-1, 0) << pos;
    uint32_t res_lo = (pos == 0)       ?  0 : BitExtract(v2, pos-1, 0);

    uint32_t res = res_hi | res_md | res_lo;
    GRegWrite (reg2, res);

    SetPswOV (0);
    SetPswS  ((res >> 31) & 0x01U);
    SetPswZ  (res == 0);
}


//! コプロセッサ使用不可例外判定
bool InstV850::IsCoproUuusableException (CfSReg sregno) const
{
    CoproType copro_type = m_srf->GetSrPtr(sregno)->GetCoproType ();
    if (likely (copro_type == COPRO_NO)) {
        return false;

    } else if (copro_type == COPRO_FPU) {
        uint32_t cu = m_srf->SrVPSW()->GetCU ();
        if ((cu & 0x1) == 0) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Copro Error: FPU : PSW.CU = %d>\n", cu);
            m_trace->SetExpCode (EXP_UCPOP0);
            return true;
        } else {
            return false;
        }

    } else if (copro_type == COPRO_SIMD) {
        uint32_t cu = m_srf->SrVPSW()->GetCU ();
        if ((cu & 0x2) == 0) {
            CfMsg::TPrint (MSG_INF, m_trace, "<Copro Error: SIMD : PSW.CU = %d>\n", cu);
            m_trace->SetExpCode (EXP_UCPOP1);
            return true;
        } else {
            return false;
        }

    } else {
        return false;
    }
}


//! 特権命令例外判定
bool InstV850::IsPrivUserException (CfSReg sregno, PrivLevel priv) const
{
    // PIE checking must be checked by m_nc_srf.
    // Because, in case of PSW access in guest mode,
    // although GMPSW is accessed, priv for PSW (SV) is necessary instead of GMPSW (HV).
    if ((m_nc_srf->SrHVCFG()->GetHVE() == 1) && (m_nc_srf->GetSrPtr(sregno)->IsPie(priv) == true)) {
        m_trace->SetExpCode (EXP_PIE); // HvValid && UM/SV acesses to HV resource
        return true;
    }
    if (m_psw->GetUM() == 1 && m_srf->GetSrPtr(sregno)->IsPrivileged(PRIV_HAS_UM) == false) {
        m_trace->SetExpCode (EXP_PIE);
        return true;
    } else {
        return false;
    }
}


//! キャッシュラインのメモリ保護違反判定
//! 1ラインの全てがアクセス可能でなければ保護違反
bool InstV850::IsMemExceptionIcLine (PhysAddr addr) const
{
    PhysAddr mask = ~(m_ic->GetLineByte () - 1);
    PhysAddr line_addr = addr & mask;

    // Break function doesn't check in cache/pref inst.
    if (unlikely (m_lsu->IsMemException<DATA_READ> (line_addr, m_ic->GetLineByte ()) == true)) {
        return true;
    }

    return false;
}


void InstV850::GRegWritePC (PhysAddr pc)
{
    m_trace->RecordPC<TraceCtrl::REG_WRITE> (pc);
    m_grf->WritePC (pc & 0xFFFFFFFEU);
}


PhysAddr InstV850::GRegReadPC (void) const
{
    PhysAddr ret = m_grf->ReadPC ();
    m_trace->RecordPC<TraceCtrl::REG_READ> (ret);
    return ret;
}


void InstV850::GRegWrite (uint32_t regno, RegData data)
{
    m_trace->RecordGReg<TraceCtrl::REG_WRITE> (regno, data);
    m_grf->Write (regno, data);
}


void InstV850::GRegWrite64 (uint32_t regno, RegData64 data64)
{
    RegData64 data64_u = data64 >> 32;
    RegData   data_u   = data64_u & 0x00FFFFFFFFULL;
    RegData   data_l   = data64   & 0x00FFFFFFFFULL;
    GRegWrite (regno, data_l);
    GRegWrite (regno+1, data_u);
}


RegData InstV850::GRegRead (uint32_t regno) const
{
    RegData ret = m_grf->Read (regno);
    m_trace->RecordGReg<TraceCtrl::REG_READ> (regno, ret);
    return ret;
}


RegData64 InstV850::GRegRead64 (uint32_t regno) const
{
    RegData64 data_l = GRegRead (regno);
    RegData64 data_u = GRegRead (regno+1);
    RegData64 data   = (data_u << 32) | data_l;
    return data;
}


void InstV850::SRegWrite (CfSReg sregno, RegData data)
{
    // sregno in trace info : actual sregno is necessary. Need to translate to actual sregno if it is virtualized
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (m_srf->GetSrPtr(sregno)->GetCfSRegId (), data);
    m_srf->GetSrPtr(sregno)->WriteBody (data);
}


RegData InstV850::SRegRead (CfSReg sregno) const
{
    RegData ret = m_srf->GetSrPtr(sregno)->ReadBody ();
    // sregno in trace info : actual sregno is necessary. Need to translate to actual sregno if it is virtualized
    m_trace->RecordSReg<TraceCtrl::REG_READ> (m_srf->GetSrPtr(sregno)->GetCfSRegId (), ret);
    return ret;
}


void InstV850::SRegWriteWithSel (uint32_t selid, uint32_t regid, RegData data)
{
    CfSReg sregno = RegID::GetCfSRegIdFromSelId (selid, regid);

    //! コプロセッサ使用不可/特権命令例外判定
    if (IsCoproUuusableException (sregno) == true) return;
    PrivLevel priv = m_trace->GetPrivLevel ();
    if (IsPrivUserException (sregno, priv) == true) return;
    if (m_srf->HasSReg(sregno) == true) {
        m_srf->GetSrPtr(sregno)->Write (data, priv, NC);
        if (m_trace->GetExpCode() == EXP_NONE) {
            // @@ TODO : There are virtualized registers in case of HVCFG.HVE=1.
            //           Should CForest prints the actual name instead of virtualized name?
            //           For example, virtualized name is "PSW", actual name is "GMPSW".
            // sregno in trace info : actual sregno is necessary. Need to translate to actual sregno if it is virtualized
            m_trace->RecordSReg<TraceCtrl::REG_WRITE> (m_srf->GetSrPtr(sregno)->GetCfSRegId (), data);
        }
    } else {
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: invalid SR access : write ignored>\n");
    }
}


RegData InstV850::SRegReadWithSel (uint32_t selid, uint32_t regid) const
{
    CfSReg sregno = RegID::GetCfSRegIdFromSelId (selid, regid);

    //! コプロセッサ使用不可/特権命令例外判定
    if (IsCoproUuusableException (sregno) == true) return 0x0;
    PrivLevel priv = m_trace->GetPrivLevel ();
    if (IsPrivUserException (sregno, priv) == true) return 0x0;
    if (m_srf->HasSReg(sregno) == true) {
        RegData ret = m_srf->GetSrPtr(sregno)->Read (priv, NC);
        if (m_trace->GetExpCode() == EXP_NONE) {
            // @@ TODO : There are virtualized registers in case of HVCFG.HVE=1.
            //           Should CForest prints the actual name instead of virtualized name?
            //           For example, virtualized name is "PSW", actual name is "GMPSW".
            // sregno in trace info : actual sregno is necessary. Need to translate to actual sregno if it is virtualized
            m_trace->RecordSReg<TraceCtrl::REG_READ> (m_srf->GetSrPtr(sregno)->GetCfSRegId (), ret);
        }
        return ret;
    } else {
#if defined(COMP_RUNNER)
              m_trace->RecordSReg<TraceCtrl::REG_READ> (RegID::GetSRegId(selid, regid), 0x0);
#endif //#if defined(COMP_RUNNER)
        CfMsg::TPrint (MSG_WAR, m_trace, "<Warning: invalid SR access : read 0x0>\n");
        return 0;
    }
}


void InstV850::SetPswh (uint32_t pswh)
{
    if (m_srf->SrHVCFG ()->GetHVE () == 1) {
        bool gm = (pswh >> 31) & 0x1U;
        uint32_t gpid = (pswh >> 8) & 0x7U;
        m_srf->SrPSWH ()->SetGPID (gpid);
        m_srf->SrPSWH ()->SetGM (gm);
        m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_PSWH, m_srf->SrPSWH()->ReadBody());
    }
}


void InstV850::SetPswUM (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_UM, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_UM, data);
    }
    m_psw->SetUM (data);
}


void InstV850::SetPswEBV (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_EBV, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_EBV, data);
    }
    m_psw->SetEBV (data);
}


void InstV850::SetPswNP (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_NP, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_NP, data);
    }
    m_psw->SetNP (data);
}


void InstV850::SetPswEP (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_EP, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_EP, data);
    }
    m_psw->SetEP (data);
}


void InstV850::SetPswID (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_ID, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_ID, data);
    }
    m_psw->SetID (data);
}


void InstV850::SetPswSAT (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_SAT, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_SAT, data);
    }
    m_psw->SetSAT (data);
}


void InstV850::SetPswCY (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_CY, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_CY, data);
    }
    m_psw->SetCY (data);
}


void InstV850::SetPswOV (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_OV, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_OV, data);
    }
    m_psw->SetOV (data);
}


void InstV850::SetPswS (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_S, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_S, data);
    }
    m_psw->SetS (data);
}


void InstV850::SetPswZ (bool data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_Z, data);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_Z, data);
    }
    m_psw->SetZ (data);
}


void InstV850::SetPswEIMASK (uint32_t data)
{
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (GMPSW_EIMASK, data & 0xFFU);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_WRITE> (HMPSW_EIMASK, data & 0xFFU);
    }
    m_psw->SetEIMASK (data);
}


bool InstV850::GetPswSAT (void) const
{
    bool r = m_psw->GetSAT ();
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (GMPSW_SAT, r);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (HMPSW_SAT, r);
    }
    return r;
}


bool InstV850::GetPswCY (void) const
{
    bool r = m_psw->GetCY ();
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (GMPSW_CY, r);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (HMPSW_CY, r);
    }
    return r;
}


bool InstV850::GetPswOV (void) const
{
    bool r = m_psw->GetOV ();
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (GMPSW_OV, r);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (HMPSW_OV, r);
    }
    return r;
}


bool InstV850::GetPswS (void) const
{
    bool r = m_psw->GetS ();
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (GMPSW_S, r);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (HMPSW_S, r);
    }
    return r;
}


bool InstV850::GetPswZ (void) const
{
    bool r = m_psw->GetZ ();
    if (m_nc_srf->SrPSWH()->GetGM () == 1) {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (GMPSW_Z, r);
    } else {
        m_trace->RecordPsw<TraceCtrl::REG_READ> (HMPSW_Z, r);
    }
    return r;
}


void InstV850::CacheCHBII (uint32_t data)
{
    if (unlikely (IsMemExceptionIcLine (data) == true)) {
        // メモリアクセス例外 (MEI.{REG,DS,U}は不定なので 0相当の値を格納)
        SetMemErrorInfo (data, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
        return;
    }

    PhysAddr phys_line_addr = m_lsu->AddressTransfer (data);
    if (m_ic->IsCachingArea (phys_line_addr)) {
        m_ic->OperateChbii (phys_line_addr, data, m_trace);
    } else {
        CfMsg::TPrint (MSG_WAR, m_trace,
                       "<Warning: irregular CHBII-address (0x%08x)>\n", phys_line_addr);
    }
}


void InstV850::CacheCIBII (uint32_t data)
{
    m_ic->OperateCibii (data, m_trace);
}


void InstV850::CacheCFALI (uint32_t data)
{
    if (unlikely (IsMemExceptionIcLine (data) == true)) {
        // メモリアクセス例外 (MEI.{REG,DS,U}は不定なので 0相当の値を格納)
        SetMemErrorInfo (data, R0, SIZE_8BIT, MA_SIGN, DATA_READ);
        return;
    }

    PhysAddr phys_line_addr = m_lsu->AddressTransfer (data);
    if (m_ic->IsCachingArea (phys_line_addr)) {
        m_ic->OperateCfali (phys_line_addr, data, m_lsu->GetSideBand (), m_trace);
        if (m_ic->m_fill_route == ICACHE_MISS_ROUTE) {
            m_lsu->m_flashrom_fetch_count += 1; // PMCTRL.CND=50h
        }
    } else {
        CfMsg::TPrint (MSG_WAR, m_trace,
                       "<Warning: irregular CFALI-address (0x%08x)>\n", phys_line_addr);
    }
}


void InstV850::CacheCISTI (uint32_t data)
{
    // キャッシュラインへデータ書き込み
    m_ic->OperateCisti (data, m_nc_srf, m_trace);

    m_trace->RecordSReg<TraceCtrl::REG_READ> (CF_SR_ICTAGL, m_nc_srf->SrICTAGL()->ReadBody());
    m_trace->RecordSReg<TraceCtrl::REG_READ> (CF_SR_ICTAGH, m_nc_srf->SrICTAGH()->ReadBody());
    m_trace->RecordSReg<TraceCtrl::REG_READ> (CF_SR_ICDATL, m_nc_srf->SrICDATL()->ReadBody());
    m_trace->RecordSReg<TraceCtrl::REG_READ> (CF_SR_ICDATH, m_nc_srf->SrICDATH()->ReadBody());
}


void InstV850::CacheCILDI (uint32_t data)
{
    // キャッシュラインのデータ読み込み
    m_ic->OperateCildi (data, m_nc_srf, m_trace);

    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_ICTAGL, m_nc_srf->SrICTAGL()->ReadBody());
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_ICTAGH, m_nc_srf->SrICTAGH()->ReadBody());
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_ICDATL, m_nc_srf->SrICDATL()->ReadBody());
    m_trace->RecordSReg<TraceCtrl::REG_WRITE> (CF_SR_ICDATH, m_nc_srf->SrICDATH()->ReadBody());
}


void InstV850::SetTraceCtrl (TraceCtrl* trace)
{
    m_trace = trace;
    m_fpu_ope->SetTraceCtrl (trace);
}


InstV850::InstV850 (TraceCtrl* trace, GRegFile* gr, SRegFile* sr,
                    Lsu* lsu, Btb* btb, NcSRegFile* nc_srf,
                    GRegFileSet* grfs, Icache* ic, FsCtrl* fsctrl,
                    ArchClass arch_class)
    : m_trace (trace),
      m_grf (gr),
      m_srf (sr),
      m_lsu (lsu),
      m_btb (btb),
      m_nc_srf  (nc_srf),
      m_grfs (grfs),
      m_ic (ic),
      m_fsctrl (fsctrl)
{
    SetFuncTable ();
    SetupGsrVec ();

    m_psw  = m_srf->SrVPSW ();
    m_fpsr = m_srf->SrFPSR ();
    m_fxsr = m_srf->SrFXSR ();

    m_fpu_ope = new FpuOpeDevice (trace, m_fpsr);
    m_fpsimd_ope = new FpSimdOpeDevice (trace, m_fxsr);
}


InstV850::~InstV850 ()
{
    delete m_fpu_ope;
    delete m_fpsimd_ope;
}
