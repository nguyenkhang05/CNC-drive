/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string>
#include <sstream>
#include "./config.h"
#include "./forest_common.h"
#include "./trace_info.h"
#include "./trace_operand.h"
#include "./micro_arch_db.h"
#include "./regid.h"
#include "./sregfile.h"


void TracePrintOperand::Conv0 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvADD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADD_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_4_0);
}

void TracePrintOperand::ConvADD_I16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_31_16);
}

void TracePrintOperand::ConvADF (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceCnd  (inst_op, inst_code, inst_pc, C_20_17);
}

void TracePrintOperand::ConvAND (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvANDI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_31_16);
}

void TracePrintOperand::ConvBCOND_D9 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceBcond (inst_op, inst_code, inst_pc, BD_15_11_6_4_S1);
}

void TracePrintOperand::ConvBCOND_D17 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceBcond (inst_op, inst_code, inst_pc, BD_4_4_31_17_S1);
}

void TracePrintOperand::ConvBINSL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWidth (inst_op, inst_code, inst_pc, W_31_28);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplacePos  (inst_op, inst_code, inst_pc, P_27_19_17);
}

void TracePrintOperand::ConvBINSM (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWidth (inst_op, inst_code, inst_pc, W_31_28);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplacePos  (inst_op, inst_code, inst_pc, P_27_19_17);
}

void TracePrintOperand::ConvBINSU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWidth (inst_op, inst_code, inst_pc, W_31_28);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplacePos  (inst_op, inst_code, inst_pc, P_27_19_17);
}

void TracePrintOperand::ConvBSH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvBSW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCACHE (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceCacheOp  (inst_op, inst_code, inst_pc, CA_12_11_31_27);
}

void TracePrintOperand::ConvCALLT (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_5_0);
}

void TracePrintOperand::ConvCAXI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCLR1_B3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_31_16);
    ReplaceHexU (inst_op, inst_code, inst_pc, B_13_11);
}

void TracePrintOperand::ConvCLR1 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCMOV_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_4_0);
    ReplaceCnd  (inst_op, inst_code, inst_pc, C_20_17);
}

void TracePrintOperand::ConvCMOV (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceCnd  (inst_op, inst_code, inst_pc, C_20_17);
}

void TracePrintOperand::ConvCMP_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_4_0);
}

void TracePrintOperand::ConvCMP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCTRET (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvDI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvDISPOSE (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceList (inst_op, inst_code, inst_pc, L_31_21_0);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_5_1);
}

void TracePrintOperand::ConvDISPOSE_R (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_20_16);
    ReplaceList (inst_op, inst_code, inst_pc, L_31_21_0);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_5_1);
}

void TracePrintOperand::ConvDIV (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVH_R2 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVHU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVQ (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVQU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvEI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvEIRET (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvFERET (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvFETRAP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceHexU (inst_op, inst_code, inst_pc, V_14_11);
}

void TracePrintOperand::ConvHALT (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvHSH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvHSW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvHVTRAP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceHexU (inst_op, inst_code, inst_pc, V_4_0);
}

void TracePrintOperand::ConvJARL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvJARL_D22 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceBra  (inst_op, inst_code, inst_pc, BRA_5_0_31_17_S1);
}

void TracePrintOperand::ConvJARL_D32 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_31_17_S1);
}

void TracePrintOperand::ConvJMP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvJMP_D32 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_31_17_S1);
}

void TracePrintOperand::ConvJR_D22 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceBra  (inst_op, inst_code, inst_pc, BRA_5_0_31_17_S1);
}

void TracePrintOperand::ConvJR_D32 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_31_17_S1);
}

void TracePrintOperand::ConvLDB_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_16);
}

void TracePrintOperand::ConvLDB_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_20);
}

void TracePrintOperand::ConvLDBU_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_17_5);
}

void TracePrintOperand::ConvLDBU_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_20);
}

void TracePrintOperand::ConvLDDW_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_21_S1);
}

void TracePrintOperand::ConvLDH_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_17_S1);
}

void TracePrintOperand::ConvLDH_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_21_S1);
}

void TracePrintOperand::ConvLDHU_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_17_S1);
}

void TracePrintOperand::ConvLDHU_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_21_S1);
}

void TracePrintOperand::ConvLDW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_17_S1);
}

void TracePrintOperand::ConvLDW_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_21_S1);
}

void TracePrintOperand::ConvLDLW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDSR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceSR   (inst_op, inst_code, inst_pc, SR_15_11);
    ReplaceSel  (inst_op, inst_code, inst_pc, SEL_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvLOOP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_17_S1);
}

void TracePrintOperand::ConvMAC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_20_17_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMACU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_20_17_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMOV_I32 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_47_32_31_16);
}

void TracePrintOperand::ConvMOV_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_4_0);
}

void TracePrintOperand::ConvMOV (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMOVEA (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_31_16);
}

void TracePrintOperand::ConvMOVHI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_31_16);
}

void TracePrintOperand::ConvMUL_I9 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_21_18_4_0);
}

void TracePrintOperand::ConvMUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMULH_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_4_0);
}

void TracePrintOperand::ConvMULH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMULHI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_31_16);
}

void TracePrintOperand::ConvMULU_I9 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_21_18_4_0);
}

void TracePrintOperand::ConvMULU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvNOP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvNOT (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvNOT1_B3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_31_16);
    ReplaceHexU (inst_op, inst_code, inst_pc, B_13_11);
}

void TracePrintOperand::ConvNOT1 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvOR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvORI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_31_16);
}

void TracePrintOperand::ConvPOPSP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvPREF (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplacePrefOp  (inst_op, inst_code, inst_pc, PO_31_27);
}

void TracePrintOperand::ConvPREPARE (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceList (inst_op, inst_code, inst_pc, L_31_21_0);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_5_1);
}

void TracePrintOperand::ConvPREPARE_SP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceList (inst_op, inst_code, inst_pc, L_31_21_0);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_5_1);
}

void TracePrintOperand::ConvPREPARE_LO16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceList (inst_op, inst_code, inst_pc, L_31_21_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_5_1);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_47_32);
}

void TracePrintOperand::ConvPREPARE_HI16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceList (inst_op, inst_code, inst_pc, L_31_21_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_5_1);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_47_32);
}

void TracePrintOperand::ConvPREPARE_I32 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceList (inst_op, inst_code, inst_pc, L_31_21_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_63_32);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_5_1);
}

void TracePrintOperand::ConvPUSHSP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvRIE (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvRIE_I9 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceDec  (inst_op, inst_code, inst_pc, I_3_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_15_11);
}

void TracePrintOperand::ConvROTL_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_4_0);
}

void TracePrintOperand::ConvROTL_R3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSAR_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_4_0);
}

void TracePrintOperand::ConvSAR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSAR_R3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSASF (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceCnd  (inst_op, inst_code, inst_pc, C_4_0);
}

void TracePrintOperand::ConvSATADD_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_4_0);
}

void TracePrintOperand::ConvSATADD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSATADD_R3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSATSUB (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSATSUB_R3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSATSUBI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceDec  (inst_op, inst_code, inst_pc, I_31_16);
}

void TracePrintOperand::ConvSATSUBR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSBF (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceCnd  (inst_op, inst_code, inst_pc, C_20_17);
}

void TracePrintOperand::ConvSCH0L (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSCH0R (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSCH1L (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSCH1R (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSET1_B3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_31_16);
    ReplaceHexU (inst_op, inst_code, inst_pc, B_13_11);
}

void TracePrintOperand::ConvSET1 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSETF (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceCnd  (inst_op, inst_code, inst_pc, C_4_0);
}

void TracePrintOperand::ConvSHL_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_4_0);
}

void TracePrintOperand::ConvSHL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSHL_R3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSHR_I5 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_4_0);
}

void TracePrintOperand::ConvSHR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSHR_R3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSLDB (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_6_0);
}

void TracePrintOperand::ConvSLDBU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_3_0);
}

void TracePrintOperand::ConvSLDH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_6_0_S1);
}

void TracePrintOperand::ConvSLDHU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_3_0_S1);
}

void TracePrintOperand::ConvSLDW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_6_1_S2);
}

void TracePrintOperand::ConvSNOOZE (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvSSTB (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_6_0);
}

void TracePrintOperand::ConvSSTH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_6_0_S1);
}

void TracePrintOperand::ConvSSTW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_6_1_S2);
}

void TracePrintOperand::ConvSTB_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_16);
}

void TracePrintOperand::ConvSTB_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_20);
}

void TracePrintOperand::ConvSTDW_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_21_S1);
}

void TracePrintOperand::ConvSTH_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_17_S1);
}

void TracePrintOperand::ConvSTH_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_21_S1);
}

void TracePrintOperand::ConvSTW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_31_17_S1);
}

void TracePrintOperand::ConvSTW_D23 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, DU_47_32_26_21_S1);
}

void TracePrintOperand::ConvSTCW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTSR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceSR   (inst_op, inst_code, inst_pc, SR_4_0);
    ReplaceSel  (inst_op, inst_code, inst_pc, SEL_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUB (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSWITCH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvSXB (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvSXH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvSYNCE (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvSYNCI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvSYNCM (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvSYNCP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvSYSCALL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceHexU (inst_op, inst_code, inst_pc, V_29_27_4_0);
}

void TracePrintOperand::ConvTRAP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceHexU (inst_op, inst_code, inst_pc, V_4_0);
}

void TracePrintOperand::ConvTST (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTST1_B3 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_31_16);
    ReplaceHexU (inst_op, inst_code, inst_pc, B_13_11);
}

void TracePrintOperand::ConvTST1 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvXOR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvXORI (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_31_16);
}

void TracePrintOperand::ConvZXB (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvZXH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvDBCP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvDBPUSH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvDBRET (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvDBTAG (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceDec  (inst_op, inst_code, inst_pc, I_31_27_4_0);
}

void TracePrintOperand::ConvDBTRAP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvABSFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvABSFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADDFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvADDFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCEILFDL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCEILFDUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCEILFDUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCEILFDW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCEILFSL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCEILFSUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCEILFSUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCEILFSW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCMOVFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
    ReplaceHexU (inst_op, inst_code, inst_pc, FCB_19_17);
}

void TracePrintOperand::ConvCMOVFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, FCB_19_17);
}

void TracePrintOperand::ConvCMPFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
    ReplaceFCO  (inst_op, inst_code, inst_pc, FCO_30_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, FCB_19_17);
}

void TracePrintOperand::ConvCMPFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceFCO  (inst_op, inst_code, inst_pc, FCO_30_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, FCB_19_17);
}

void TracePrintOperand::ConvCVTFDL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFDS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFDUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFDUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFDW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFLD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFLS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFSD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFULD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFULS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvCVTFUWD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFUWS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFWD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFWS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvDIVFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFLOORFDL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvFLOORFDUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvFLOORFDUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvFLOORFDW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvFLOORFSL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFLOORFSUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFLOORFSUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFLOORFSW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMAXFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvMAXFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMINFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvMINFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMULFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvMULFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvNEGFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvNEGFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvRECIPFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvRECIPFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvROUNDFDL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvROUNDFDUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvROUNDFDUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvROUNDFDW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvROUNDFSL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvROUNDFSUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvROUNDFSUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvROUNDFSW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvRSQRTFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvRSQRTFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSQRTFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvSQRTFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBFD (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_1_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvSUBFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTRFSR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceHexU (inst_op, inst_code, inst_pc, FCB_19_17);
}

void TracePrintOperand::ConvTRNCFDL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvTRNCFDUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvTRNCFDUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvTRNCFDW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_12_S1);
}

void TracePrintOperand::ConvTRNCFSL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTRNCFSUL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_28_S1);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTRNCFSUW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTRNCFSW (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFMAFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFMSFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFNMAFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFNMSFS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFHS (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvBR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceBcond (inst_op, inst_code, inst_pc, BD_15_11_6_4_S1);
}

void TracePrintOperand::ConvCMPFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceFCO  (inst_op, inst_code, inst_pc, FCO_20_17);
}

void TracePrintOperand::ConvROUNDFSW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvROUNDFSUW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTRNCFSW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTRNCFSUW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCEILFSW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCEILFSUW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFLOORFSW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFLOORFSUW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSUW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFWS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFUWS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFHS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCVTFSH4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvABSFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvNEGFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSQRTFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvRECIPFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvRSQRTFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFLPVS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_1_0);
}

void TracePrintOperand::ConvMOVVW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvTRFSRVW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_29_27);
}

void TracePrintOperand::ConvADDFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMULFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMAXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMINFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvDIVFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADDRFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBRFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMULRFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMAXRFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMINRFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADDXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvMULXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADDSUBFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBADDFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADDSUBXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBADDXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADDSUBNFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBADDNFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvADDSUBNXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvSUBADDNXFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFMAFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFMSFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFNMAFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvFNMSFS4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_15_11);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvLDVW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_24_21);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_34_S2);
}

void TracePrintOperand::ConvSTVW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_22_21);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_34_S2);
}

void TracePrintOperand::ConvLDVQW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_36_S4);
}

void TracePrintOperand::ConvSTVQW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_36_S4);
}

void TracePrintOperand::ConvLDVDW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_22_21);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_35_S3);
}

void TracePrintOperand::ConvSTVDW_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_21_21);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_35_S3);
}

void TracePrintOperand::ConvLDVZH4_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_35_S3);
}

void TracePrintOperand::ConvSTVZH4_D16 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceDec  (inst_op, inst_code, inst_pc, D_47_35_S3);
}

void TracePrintOperand::ConvCMOVFW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_47_43);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_36_32);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSHFLVW4 (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_4_0);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_47_43);
    ReplaceWReg (inst_op, inst_code, inst_pc, WR_31_27);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
    ReplaceHexU (inst_op, inst_code, inst_pc, IU_21_21_42_32);
}

void TracePrintOperand::ConvCLIP_B (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCLIP_BU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCLIP_H (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvCLIP_HU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_15_11);
}

void TracePrintOperand::ConvLDLBU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDB_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDBU_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDB_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDBU_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTCB (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTB_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTB_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDLHU (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDH_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDHU_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDH_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDHU_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTCH (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTH_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTH_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDW_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvLDW_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTW_INC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvSTW_DEC (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceReg  (inst_op, inst_code, inst_pc, R_31_27);
}

void TracePrintOperand::ConvRESBANK (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvCLL (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
}

void TracePrintOperand::ConvSTM_GSR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvSTM_MP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceEntry (inst_op, inst_code, inst_pc, E_31_27);
    ReplaceEntry (inst_op, inst_code, inst_pc, E_15_11);
}

void TracePrintOperand::ConvLDM_GSR (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
}

void TracePrintOperand::ConvLDM_MP (std::string* inst_op, uint64_t inst_code,
                                              uint32_t inst_pc) const
{
    ReplaceReg  (inst_op, inst_code, inst_pc, R_4_0);
    ReplaceEntry (inst_op, inst_code, inst_pc, E_31_27);
    ReplaceEntry (inst_op, inst_code, inst_pc, E_15_11);
}

TracePrintOperand::TracePrintOperand (void)
{
    m_inst_func[INST_ID_V_0]            = &TracePrintOperand::Conv0;
    m_inst_func[INST_ID_V_ADD]          = &TracePrintOperand::ConvADD;
    m_inst_func[INST_ID_V_ADD_I5]       = &TracePrintOperand::ConvADD_I5;
    m_inst_func[INST_ID_V_ADD_I16]      = &TracePrintOperand::ConvADD_I16;
    m_inst_func[INST_ID_V_ADF]          = &TracePrintOperand::ConvADF;
    m_inst_func[INST_ID_V_AND]          = &TracePrintOperand::ConvAND;
    m_inst_func[INST_ID_V_ANDI]         = &TracePrintOperand::ConvANDI;
    m_inst_func[INST_ID_V_BCOND_D9]     = &TracePrintOperand::ConvBCOND_D9;
    m_inst_func[INST_ID_V_BCOND_D17]    = &TracePrintOperand::ConvBCOND_D17;
    m_inst_func[INST_ID_V_BINSL]        = &TracePrintOperand::ConvBINSL;
    m_inst_func[INST_ID_V_BINSM]        = &TracePrintOperand::ConvBINSM;
    m_inst_func[INST_ID_V_BINSU]        = &TracePrintOperand::ConvBINSU;
    m_inst_func[INST_ID_V_BSH]          = &TracePrintOperand::ConvBSH;
    m_inst_func[INST_ID_V_BSW]          = &TracePrintOperand::ConvBSW;
    m_inst_func[INST_ID_V_CACHE]        = &TracePrintOperand::ConvCACHE;
    m_inst_func[INST_ID_V_CALLT]        = &TracePrintOperand::ConvCALLT;
    m_inst_func[INST_ID_V_CAXI]         = &TracePrintOperand::ConvCAXI;
    m_inst_func[INST_ID_V_CLR1_B3]      = &TracePrintOperand::ConvCLR1_B3;
    m_inst_func[INST_ID_V_CLR1]         = &TracePrintOperand::ConvCLR1;
    m_inst_func[INST_ID_V_CMOV_I5]      = &TracePrintOperand::ConvCMOV_I5;
    m_inst_func[INST_ID_V_CMOV]         = &TracePrintOperand::ConvCMOV;
    m_inst_func[INST_ID_V_CMP_I5]       = &TracePrintOperand::ConvCMP_I5;
    m_inst_func[INST_ID_V_CMP]          = &TracePrintOperand::ConvCMP;
    m_inst_func[INST_ID_V_CTRET]        = &TracePrintOperand::ConvCTRET;
    m_inst_func[INST_ID_V_DI]           = &TracePrintOperand::ConvDI;
    m_inst_func[INST_ID_V_DISPOSE]      = &TracePrintOperand::ConvDISPOSE;
    m_inst_func[INST_ID_V_DISPOSE_R]    = &TracePrintOperand::ConvDISPOSE_R;
    m_inst_func[INST_ID_V_DIV]          = &TracePrintOperand::ConvDIV;
    m_inst_func[INST_ID_V_DIVH_R2]      = &TracePrintOperand::ConvDIVH_R2;
    m_inst_func[INST_ID_V_DIVH]         = &TracePrintOperand::ConvDIVH;
    m_inst_func[INST_ID_V_DIVHU]        = &TracePrintOperand::ConvDIVHU;
    m_inst_func[INST_ID_V_DIVQ]         = &TracePrintOperand::ConvDIVQ;
    m_inst_func[INST_ID_V_DIVU]         = &TracePrintOperand::ConvDIVU;
    m_inst_func[INST_ID_V_DIVQU]        = &TracePrintOperand::ConvDIVQU;
    m_inst_func[INST_ID_V_EI]           = &TracePrintOperand::ConvEI;
    m_inst_func[INST_ID_V_EIRET]        = &TracePrintOperand::ConvEIRET;
    m_inst_func[INST_ID_V_FERET]        = &TracePrintOperand::ConvFERET;
    m_inst_func[INST_ID_V_FETRAP]       = &TracePrintOperand::ConvFETRAP;
    m_inst_func[INST_ID_V_HALT]         = &TracePrintOperand::ConvHALT;
    m_inst_func[INST_ID_V_HSH]          = &TracePrintOperand::ConvHSH;
    m_inst_func[INST_ID_V_HSW]          = &TracePrintOperand::ConvHSW;
    m_inst_func[INST_ID_V_HVTRAP]       = &TracePrintOperand::ConvHVTRAP;
    m_inst_func[INST_ID_V_JARL]         = &TracePrintOperand::ConvJARL;
    m_inst_func[INST_ID_V_JARL_D22]     = &TracePrintOperand::ConvJARL_D22;
    m_inst_func[INST_ID_V_JARL_D32]     = &TracePrintOperand::ConvJARL_D32;
    m_inst_func[INST_ID_V_JMP]          = &TracePrintOperand::ConvJMP;
    m_inst_func[INST_ID_V_JMP_D32]      = &TracePrintOperand::ConvJMP_D32;
    m_inst_func[INST_ID_V_JR_D22]       = &TracePrintOperand::ConvJR_D22;
    m_inst_func[INST_ID_V_JR_D32]       = &TracePrintOperand::ConvJR_D32;
    m_inst_func[INST_ID_V_LDB_D16]      = &TracePrintOperand::ConvLDB_D16;
    m_inst_func[INST_ID_V_LDB_D23]      = &TracePrintOperand::ConvLDB_D23;
    m_inst_func[INST_ID_V_LDBU_D16]     = &TracePrintOperand::ConvLDBU_D16;
    m_inst_func[INST_ID_V_LDBU_D23]     = &TracePrintOperand::ConvLDBU_D23;
    m_inst_func[INST_ID_V_LDDW_D23]     = &TracePrintOperand::ConvLDDW_D23;
    m_inst_func[INST_ID_V_LDH_D16]      = &TracePrintOperand::ConvLDH_D16;
    m_inst_func[INST_ID_V_LDH_D23]      = &TracePrintOperand::ConvLDH_D23;
    m_inst_func[INST_ID_V_LDHU_D16]     = &TracePrintOperand::ConvLDHU_D16;
    m_inst_func[INST_ID_V_LDHU_D23]     = &TracePrintOperand::ConvLDHU_D23;
    m_inst_func[INST_ID_V_LDW_D16]      = &TracePrintOperand::ConvLDW_D16;
    m_inst_func[INST_ID_V_LDW_D23]      = &TracePrintOperand::ConvLDW_D23;
    m_inst_func[INST_ID_V_LDLW]         = &TracePrintOperand::ConvLDLW;
    m_inst_func[INST_ID_V_LDSR]         = &TracePrintOperand::ConvLDSR;
    m_inst_func[INST_ID_V_LOOP]         = &TracePrintOperand::ConvLOOP;
    m_inst_func[INST_ID_V_MAC]          = &TracePrintOperand::ConvMAC;
    m_inst_func[INST_ID_V_MACU]         = &TracePrintOperand::ConvMACU;
    m_inst_func[INST_ID_V_MOV_I32]      = &TracePrintOperand::ConvMOV_I32;
    m_inst_func[INST_ID_V_MOV_I5]       = &TracePrintOperand::ConvMOV_I5;
    m_inst_func[INST_ID_V_MOV]          = &TracePrintOperand::ConvMOV;
    m_inst_func[INST_ID_V_MOVEA]        = &TracePrintOperand::ConvMOVEA;
    m_inst_func[INST_ID_V_MOVHI]        = &TracePrintOperand::ConvMOVHI;
    m_inst_func[INST_ID_V_MUL_I9]       = &TracePrintOperand::ConvMUL_I9;
    m_inst_func[INST_ID_V_MUL]          = &TracePrintOperand::ConvMUL;
    m_inst_func[INST_ID_V_MULH_I5]      = &TracePrintOperand::ConvMULH_I5;
    m_inst_func[INST_ID_V_MULH]         = &TracePrintOperand::ConvMULH;
    m_inst_func[INST_ID_V_MULHI]        = &TracePrintOperand::ConvMULHI;
    m_inst_func[INST_ID_V_MULU_I9]      = &TracePrintOperand::ConvMULU_I9;
    m_inst_func[INST_ID_V_MULU]         = &TracePrintOperand::ConvMULU;
    m_inst_func[INST_ID_V_NOP]          = &TracePrintOperand::ConvNOP;
    m_inst_func[INST_ID_V_NOT]          = &TracePrintOperand::ConvNOT;
    m_inst_func[INST_ID_V_NOT1_B3]      = &TracePrintOperand::ConvNOT1_B3;
    m_inst_func[INST_ID_V_NOT1]         = &TracePrintOperand::ConvNOT1;
    m_inst_func[INST_ID_V_OR]           = &TracePrintOperand::ConvOR;
    m_inst_func[INST_ID_V_ORI]          = &TracePrintOperand::ConvORI;
    m_inst_func[INST_ID_V_POPSP]        = &TracePrintOperand::ConvPOPSP;
    m_inst_func[INST_ID_V_PREF]         = &TracePrintOperand::ConvPREF;
    m_inst_func[INST_ID_V_PREPARE]      = &TracePrintOperand::ConvPREPARE;
    m_inst_func[INST_ID_V_PREPARE_SP]   = &TracePrintOperand::ConvPREPARE_SP;
    m_inst_func[INST_ID_V_PREPARE_LO16] = &TracePrintOperand::ConvPREPARE_LO16;
    m_inst_func[INST_ID_V_PREPARE_HI16] = &TracePrintOperand::ConvPREPARE_HI16;
    m_inst_func[INST_ID_V_PREPARE_I32]  = &TracePrintOperand::ConvPREPARE_I32;
    m_inst_func[INST_ID_V_PUSHSP]       = &TracePrintOperand::ConvPUSHSP;
    m_inst_func[INST_ID_V_RIE]          = &TracePrintOperand::ConvRIE;
    m_inst_func[INST_ID_V_RIE_I9]       = &TracePrintOperand::ConvRIE_I9;
    m_inst_func[INST_ID_V_ROTL_I5]      = &TracePrintOperand::ConvROTL_I5;
    m_inst_func[INST_ID_V_ROTL_R3]      = &TracePrintOperand::ConvROTL_R3;
    m_inst_func[INST_ID_V_SAR_I5]       = &TracePrintOperand::ConvSAR_I5;
    m_inst_func[INST_ID_V_SAR]          = &TracePrintOperand::ConvSAR;
    m_inst_func[INST_ID_V_SAR_R3]       = &TracePrintOperand::ConvSAR_R3;
    m_inst_func[INST_ID_V_SASF]         = &TracePrintOperand::ConvSASF;
    m_inst_func[INST_ID_V_SATADD_I5]    = &TracePrintOperand::ConvSATADD_I5;
    m_inst_func[INST_ID_V_SATADD]       = &TracePrintOperand::ConvSATADD;
    m_inst_func[INST_ID_V_SATADD_R3]    = &TracePrintOperand::ConvSATADD_R3;
    m_inst_func[INST_ID_V_SATSUB]       = &TracePrintOperand::ConvSATSUB;
    m_inst_func[INST_ID_V_SATSUB_R3]    = &TracePrintOperand::ConvSATSUB_R3;
    m_inst_func[INST_ID_V_SATSUBI]      = &TracePrintOperand::ConvSATSUBI;
    m_inst_func[INST_ID_V_SATSUBR]      = &TracePrintOperand::ConvSATSUBR;
    m_inst_func[INST_ID_V_SBF]          = &TracePrintOperand::ConvSBF;
    m_inst_func[INST_ID_V_SCH0L]        = &TracePrintOperand::ConvSCH0L;
    m_inst_func[INST_ID_V_SCH0R]        = &TracePrintOperand::ConvSCH0R;
    m_inst_func[INST_ID_V_SCH1L]        = &TracePrintOperand::ConvSCH1L;
    m_inst_func[INST_ID_V_SCH1R]        = &TracePrintOperand::ConvSCH1R;
    m_inst_func[INST_ID_V_SET1_B3]      = &TracePrintOperand::ConvSET1_B3;
    m_inst_func[INST_ID_V_SET1]         = &TracePrintOperand::ConvSET1;
    m_inst_func[INST_ID_V_SETF]         = &TracePrintOperand::ConvSETF;
    m_inst_func[INST_ID_V_SHL_I5]       = &TracePrintOperand::ConvSHL_I5;
    m_inst_func[INST_ID_V_SHL]          = &TracePrintOperand::ConvSHL;
    m_inst_func[INST_ID_V_SHL_R3]       = &TracePrintOperand::ConvSHL_R3;
    m_inst_func[INST_ID_V_SHR_I5]       = &TracePrintOperand::ConvSHR_I5;
    m_inst_func[INST_ID_V_SHR]          = &TracePrintOperand::ConvSHR;
    m_inst_func[INST_ID_V_SHR_R3]       = &TracePrintOperand::ConvSHR_R3;
    m_inst_func[INST_ID_V_SLDB]         = &TracePrintOperand::ConvSLDB;
    m_inst_func[INST_ID_V_SLDBU]        = &TracePrintOperand::ConvSLDBU;
    m_inst_func[INST_ID_V_SLDH]         = &TracePrintOperand::ConvSLDH;
    m_inst_func[INST_ID_V_SLDHU]        = &TracePrintOperand::ConvSLDHU;
    m_inst_func[INST_ID_V_SLDW]         = &TracePrintOperand::ConvSLDW;
    m_inst_func[INST_ID_V_SNOOZE]       = &TracePrintOperand::ConvSNOOZE;
    m_inst_func[INST_ID_V_SSTB]         = &TracePrintOperand::ConvSSTB;
    m_inst_func[INST_ID_V_SSTH]         = &TracePrintOperand::ConvSSTH;
    m_inst_func[INST_ID_V_SSTW]         = &TracePrintOperand::ConvSSTW;
    m_inst_func[INST_ID_V_STB_D16]      = &TracePrintOperand::ConvSTB_D16;
    m_inst_func[INST_ID_V_STB_D23]      = &TracePrintOperand::ConvSTB_D23;
    m_inst_func[INST_ID_V_STDW_D23]     = &TracePrintOperand::ConvSTDW_D23;
    m_inst_func[INST_ID_V_STH_D16]      = &TracePrintOperand::ConvSTH_D16;
    m_inst_func[INST_ID_V_STH_D23]      = &TracePrintOperand::ConvSTH_D23;
    m_inst_func[INST_ID_V_STW_D16]      = &TracePrintOperand::ConvSTW_D16;
    m_inst_func[INST_ID_V_STW_D23]      = &TracePrintOperand::ConvSTW_D23;
    m_inst_func[INST_ID_V_STCW]         = &TracePrintOperand::ConvSTCW;
    m_inst_func[INST_ID_V_STSR]         = &TracePrintOperand::ConvSTSR;
    m_inst_func[INST_ID_V_SUB]          = &TracePrintOperand::ConvSUB;
    m_inst_func[INST_ID_V_SUBR]         = &TracePrintOperand::ConvSUBR;
    m_inst_func[INST_ID_V_SWITCH]       = &TracePrintOperand::ConvSWITCH;
    m_inst_func[INST_ID_V_SXB]          = &TracePrintOperand::ConvSXB;
    m_inst_func[INST_ID_V_SXH]          = &TracePrintOperand::ConvSXH;
    m_inst_func[INST_ID_V_SYNCE]        = &TracePrintOperand::ConvSYNCE;
    m_inst_func[INST_ID_V_SYNCI]        = &TracePrintOperand::ConvSYNCI;
    m_inst_func[INST_ID_V_SYNCM]        = &TracePrintOperand::ConvSYNCM;
    m_inst_func[INST_ID_V_SYNCP]        = &TracePrintOperand::ConvSYNCP;
    m_inst_func[INST_ID_V_SYSCALL]      = &TracePrintOperand::ConvSYSCALL;
    m_inst_func[INST_ID_V_TRAP]         = &TracePrintOperand::ConvTRAP;
    m_inst_func[INST_ID_V_TST]          = &TracePrintOperand::ConvTST;
    m_inst_func[INST_ID_V_TST1_B3]      = &TracePrintOperand::ConvTST1_B3;
    m_inst_func[INST_ID_V_TST1]         = &TracePrintOperand::ConvTST1;
    m_inst_func[INST_ID_V_XOR]          = &TracePrintOperand::ConvXOR;
    m_inst_func[INST_ID_V_XORI]         = &TracePrintOperand::ConvXORI;
    m_inst_func[INST_ID_V_ZXB]          = &TracePrintOperand::ConvZXB;
    m_inst_func[INST_ID_V_ZXH]          = &TracePrintOperand::ConvZXH;
    m_inst_func[INST_ID_V_DBCP]         = &TracePrintOperand::ConvDBCP;
    m_inst_func[INST_ID_V_DBPUSH]       = &TracePrintOperand::ConvDBPUSH;
    m_inst_func[INST_ID_V_DBRET]        = &TracePrintOperand::ConvDBRET;
    m_inst_func[INST_ID_V_DBTAG]        = &TracePrintOperand::ConvDBTAG;
    m_inst_func[INST_ID_V_DBTRAP]       = &TracePrintOperand::ConvDBTRAP;
    m_inst_func[INST_ID_V_ABSFD]        = &TracePrintOperand::ConvABSFD;
    m_inst_func[INST_ID_V_ABSFS]        = &TracePrintOperand::ConvABSFS;
    m_inst_func[INST_ID_V_ADDFD]        = &TracePrintOperand::ConvADDFD;
    m_inst_func[INST_ID_V_ADDFS]        = &TracePrintOperand::ConvADDFS;
    m_inst_func[INST_ID_V_CEILFDL]      = &TracePrintOperand::ConvCEILFDL;
    m_inst_func[INST_ID_V_CEILFDUL]     = &TracePrintOperand::ConvCEILFDUL;
    m_inst_func[INST_ID_V_CEILFDUW]     = &TracePrintOperand::ConvCEILFDUW;
    m_inst_func[INST_ID_V_CEILFDW]      = &TracePrintOperand::ConvCEILFDW;
    m_inst_func[INST_ID_V_CEILFSL]      = &TracePrintOperand::ConvCEILFSL;
    m_inst_func[INST_ID_V_CEILFSUL]     = &TracePrintOperand::ConvCEILFSUL;
    m_inst_func[INST_ID_V_CEILFSUW]     = &TracePrintOperand::ConvCEILFSUW;
    m_inst_func[INST_ID_V_CEILFSW]      = &TracePrintOperand::ConvCEILFSW;
    m_inst_func[INST_ID_V_CMOVFD]       = &TracePrintOperand::ConvCMOVFD;
    m_inst_func[INST_ID_V_CMOVFS]       = &TracePrintOperand::ConvCMOVFS;
    m_inst_func[INST_ID_V_CMPFD]        = &TracePrintOperand::ConvCMPFD;
    m_inst_func[INST_ID_V_CMPFS]        = &TracePrintOperand::ConvCMPFS;
    m_inst_func[INST_ID_V_CVTFDL]       = &TracePrintOperand::ConvCVTFDL;
    m_inst_func[INST_ID_V_CVTFDS]       = &TracePrintOperand::ConvCVTFDS;
    m_inst_func[INST_ID_V_CVTFDUL]      = &TracePrintOperand::ConvCVTFDUL;
    m_inst_func[INST_ID_V_CVTFDUW]      = &TracePrintOperand::ConvCVTFDUW;
    m_inst_func[INST_ID_V_CVTFDW]       = &TracePrintOperand::ConvCVTFDW;
    m_inst_func[INST_ID_V_CVTFLD]       = &TracePrintOperand::ConvCVTFLD;
    m_inst_func[INST_ID_V_CVTFLS]       = &TracePrintOperand::ConvCVTFLS;
    m_inst_func[INST_ID_V_CVTFSD]       = &TracePrintOperand::ConvCVTFSD;
    m_inst_func[INST_ID_V_CVTFSL]       = &TracePrintOperand::ConvCVTFSL;
    m_inst_func[INST_ID_V_CVTFSUL]      = &TracePrintOperand::ConvCVTFSUL;
    m_inst_func[INST_ID_V_CVTFSUW]      = &TracePrintOperand::ConvCVTFSUW;
    m_inst_func[INST_ID_V_CVTFSW]       = &TracePrintOperand::ConvCVTFSW;
    m_inst_func[INST_ID_V_CVTFULD]      = &TracePrintOperand::ConvCVTFULD;
    m_inst_func[INST_ID_V_CVTFULS]      = &TracePrintOperand::ConvCVTFULS;
    m_inst_func[INST_ID_V_CVTFUWD]      = &TracePrintOperand::ConvCVTFUWD;
    m_inst_func[INST_ID_V_CVTFUWS]      = &TracePrintOperand::ConvCVTFUWS;
    m_inst_func[INST_ID_V_CVTFWD]       = &TracePrintOperand::ConvCVTFWD;
    m_inst_func[INST_ID_V_CVTFWS]       = &TracePrintOperand::ConvCVTFWS;
    m_inst_func[INST_ID_V_DIVFD]        = &TracePrintOperand::ConvDIVFD;
    m_inst_func[INST_ID_V_DIVFS]        = &TracePrintOperand::ConvDIVFS;
    m_inst_func[INST_ID_V_FLOORFDL]     = &TracePrintOperand::ConvFLOORFDL;
    m_inst_func[INST_ID_V_FLOORFDUL]    = &TracePrintOperand::ConvFLOORFDUL;
    m_inst_func[INST_ID_V_FLOORFDUW]    = &TracePrintOperand::ConvFLOORFDUW;
    m_inst_func[INST_ID_V_FLOORFDW]     = &TracePrintOperand::ConvFLOORFDW;
    m_inst_func[INST_ID_V_FLOORFSL]     = &TracePrintOperand::ConvFLOORFSL;
    m_inst_func[INST_ID_V_FLOORFSUL]    = &TracePrintOperand::ConvFLOORFSUL;
    m_inst_func[INST_ID_V_FLOORFSUW]    = &TracePrintOperand::ConvFLOORFSUW;
    m_inst_func[INST_ID_V_FLOORFSW]     = &TracePrintOperand::ConvFLOORFSW;
    m_inst_func[INST_ID_V_MAXFD]        = &TracePrintOperand::ConvMAXFD;
    m_inst_func[INST_ID_V_MAXFS]        = &TracePrintOperand::ConvMAXFS;
    m_inst_func[INST_ID_V_MINFD]        = &TracePrintOperand::ConvMINFD;
    m_inst_func[INST_ID_V_MINFS]        = &TracePrintOperand::ConvMINFS;
    m_inst_func[INST_ID_V_MULFD]        = &TracePrintOperand::ConvMULFD;
    m_inst_func[INST_ID_V_MULFS]        = &TracePrintOperand::ConvMULFS;
    m_inst_func[INST_ID_V_NEGFD]        = &TracePrintOperand::ConvNEGFD;
    m_inst_func[INST_ID_V_NEGFS]        = &TracePrintOperand::ConvNEGFS;
    m_inst_func[INST_ID_V_RECIPFD]      = &TracePrintOperand::ConvRECIPFD;
    m_inst_func[INST_ID_V_RECIPFS]      = &TracePrintOperand::ConvRECIPFS;
    m_inst_func[INST_ID_V_ROUNDFDL]     = &TracePrintOperand::ConvROUNDFDL;
    m_inst_func[INST_ID_V_ROUNDFDUL]    = &TracePrintOperand::ConvROUNDFDUL;
    m_inst_func[INST_ID_V_ROUNDFDUW]    = &TracePrintOperand::ConvROUNDFDUW;
    m_inst_func[INST_ID_V_ROUNDFDW]     = &TracePrintOperand::ConvROUNDFDW;
    m_inst_func[INST_ID_V_ROUNDFSL]     = &TracePrintOperand::ConvROUNDFSL;
    m_inst_func[INST_ID_V_ROUNDFSUL]    = &TracePrintOperand::ConvROUNDFSUL;
    m_inst_func[INST_ID_V_ROUNDFSUW]    = &TracePrintOperand::ConvROUNDFSUW;
    m_inst_func[INST_ID_V_ROUNDFSW]     = &TracePrintOperand::ConvROUNDFSW;
    m_inst_func[INST_ID_V_RSQRTFD]      = &TracePrintOperand::ConvRSQRTFD;
    m_inst_func[INST_ID_V_RSQRTFS]      = &TracePrintOperand::ConvRSQRTFS;
    m_inst_func[INST_ID_V_SQRTFD]       = &TracePrintOperand::ConvSQRTFD;
    m_inst_func[INST_ID_V_SQRTFS]       = &TracePrintOperand::ConvSQRTFS;
    m_inst_func[INST_ID_V_SUBFD]        = &TracePrintOperand::ConvSUBFD;
    m_inst_func[INST_ID_V_SUBFS]        = &TracePrintOperand::ConvSUBFS;
    m_inst_func[INST_ID_V_TRFSR]        = &TracePrintOperand::ConvTRFSR;
    m_inst_func[INST_ID_V_TRNCFDL]      = &TracePrintOperand::ConvTRNCFDL;
    m_inst_func[INST_ID_V_TRNCFDUL]     = &TracePrintOperand::ConvTRNCFDUL;
    m_inst_func[INST_ID_V_TRNCFDUW]     = &TracePrintOperand::ConvTRNCFDUW;
    m_inst_func[INST_ID_V_TRNCFDW]      = &TracePrintOperand::ConvTRNCFDW;
    m_inst_func[INST_ID_V_TRNCFSL]      = &TracePrintOperand::ConvTRNCFSL;
    m_inst_func[INST_ID_V_TRNCFSUL]     = &TracePrintOperand::ConvTRNCFSUL;
    m_inst_func[INST_ID_V_TRNCFSUW]     = &TracePrintOperand::ConvTRNCFSUW;
    m_inst_func[INST_ID_V_TRNCFSW]      = &TracePrintOperand::ConvTRNCFSW;
    m_inst_func[INST_ID_V_FMAFS]        = &TracePrintOperand::ConvFMAFS;
    m_inst_func[INST_ID_V_FMSFS]        = &TracePrintOperand::ConvFMSFS;
    m_inst_func[INST_ID_V_FNMAFS]       = &TracePrintOperand::ConvFNMAFS;
    m_inst_func[INST_ID_V_FNMSFS]       = &TracePrintOperand::ConvFNMSFS;
    m_inst_func[INST_ID_V_CVTFHS]       = &TracePrintOperand::ConvCVTFHS;
    m_inst_func[INST_ID_V_CVTFSH]       = &TracePrintOperand::ConvCVTFSH;
    m_inst_func[INST_ID_V_BR]           = &TracePrintOperand::ConvBR;
    m_inst_func[INST_ID_V_CMPFS4]       = &TracePrintOperand::ConvCMPFS4;
    m_inst_func[INST_ID_V_ROUNDFSW4]    = &TracePrintOperand::ConvROUNDFSW4;
    m_inst_func[INST_ID_V_ROUNDFSUW4]   = &TracePrintOperand::ConvROUNDFSUW4;
    m_inst_func[INST_ID_V_TRNCFSW4]     = &TracePrintOperand::ConvTRNCFSW4;
    m_inst_func[INST_ID_V_TRNCFSUW4]    = &TracePrintOperand::ConvTRNCFSUW4;
    m_inst_func[INST_ID_V_CEILFSW4]     = &TracePrintOperand::ConvCEILFSW4;
    m_inst_func[INST_ID_V_CEILFSUW4]    = &TracePrintOperand::ConvCEILFSUW4;
    m_inst_func[INST_ID_V_FLOORFSW4]    = &TracePrintOperand::ConvFLOORFSW4;
    m_inst_func[INST_ID_V_FLOORFSUW4]   = &TracePrintOperand::ConvFLOORFSUW4;
    m_inst_func[INST_ID_V_CVTFSW4]      = &TracePrintOperand::ConvCVTFSW4;
    m_inst_func[INST_ID_V_CVTFSUW4]     = &TracePrintOperand::ConvCVTFSUW4;
    m_inst_func[INST_ID_V_CVTFWS4]      = &TracePrintOperand::ConvCVTFWS4;
    m_inst_func[INST_ID_V_CVTFUWS4]     = &TracePrintOperand::ConvCVTFUWS4;
    m_inst_func[INST_ID_V_CVTFHS4]      = &TracePrintOperand::ConvCVTFHS4;
    m_inst_func[INST_ID_V_CVTFSH4]      = &TracePrintOperand::ConvCVTFSH4;
    m_inst_func[INST_ID_V_ABSFS4]       = &TracePrintOperand::ConvABSFS4;
    m_inst_func[INST_ID_V_NEGFS4]       = &TracePrintOperand::ConvNEGFS4;
    m_inst_func[INST_ID_V_SQRTFS4]      = &TracePrintOperand::ConvSQRTFS4;
    m_inst_func[INST_ID_V_RECIPFS4]     = &TracePrintOperand::ConvRECIPFS4;
    m_inst_func[INST_ID_V_RSQRTFS4]     = &TracePrintOperand::ConvRSQRTFS4;
    m_inst_func[INST_ID_V_FLPVS4]       = &TracePrintOperand::ConvFLPVS4;
    m_inst_func[INST_ID_V_MOVVW4]       = &TracePrintOperand::ConvMOVVW4;
    m_inst_func[INST_ID_V_TRFSRVW4]     = &TracePrintOperand::ConvTRFSRVW4;
    m_inst_func[INST_ID_V_ADDFS4]       = &TracePrintOperand::ConvADDFS4;
    m_inst_func[INST_ID_V_SUBFS4]       = &TracePrintOperand::ConvSUBFS4;
    m_inst_func[INST_ID_V_MULFS4]       = &TracePrintOperand::ConvMULFS4;
    m_inst_func[INST_ID_V_MAXFS4]       = &TracePrintOperand::ConvMAXFS4;
    m_inst_func[INST_ID_V_MINFS4]       = &TracePrintOperand::ConvMINFS4;
    m_inst_func[INST_ID_V_DIVFS4]       = &TracePrintOperand::ConvDIVFS4;
    m_inst_func[INST_ID_V_ADDRFS4]      = &TracePrintOperand::ConvADDRFS4;
    m_inst_func[INST_ID_V_SUBRFS4]      = &TracePrintOperand::ConvSUBRFS4;
    m_inst_func[INST_ID_V_MULRFS4]      = &TracePrintOperand::ConvMULRFS4;
    m_inst_func[INST_ID_V_MAXRFS4]      = &TracePrintOperand::ConvMAXRFS4;
    m_inst_func[INST_ID_V_MINRFS4]      = &TracePrintOperand::ConvMINRFS4;
    m_inst_func[INST_ID_V_ADDXFS4]      = &TracePrintOperand::ConvADDXFS4;
    m_inst_func[INST_ID_V_SUBXFS4]      = &TracePrintOperand::ConvSUBXFS4;
    m_inst_func[INST_ID_V_MULXFS4]      = &TracePrintOperand::ConvMULXFS4;
    m_inst_func[INST_ID_V_ADDSUBFS4]    = &TracePrintOperand::ConvADDSUBFS4;
    m_inst_func[INST_ID_V_SUBADDFS4]    = &TracePrintOperand::ConvSUBADDFS4;
    m_inst_func[INST_ID_V_ADDSUBXFS4]   = &TracePrintOperand::ConvADDSUBXFS4;
    m_inst_func[INST_ID_V_SUBADDXFS4]   = &TracePrintOperand::ConvSUBADDXFS4;
    m_inst_func[INST_ID_V_ADDSUBNFS4]   = &TracePrintOperand::ConvADDSUBNFS4;
    m_inst_func[INST_ID_V_SUBADDNFS4]   = &TracePrintOperand::ConvSUBADDNFS4;
    m_inst_func[INST_ID_V_ADDSUBNXFS4]  = &TracePrintOperand::ConvADDSUBNXFS4;
    m_inst_func[INST_ID_V_SUBADDNXFS4]  = &TracePrintOperand::ConvSUBADDNXFS4;
    m_inst_func[INST_ID_V_FMAFS4]       = &TracePrintOperand::ConvFMAFS4;
    m_inst_func[INST_ID_V_FMSFS4]       = &TracePrintOperand::ConvFMSFS4;
    m_inst_func[INST_ID_V_FNMAFS4]      = &TracePrintOperand::ConvFNMAFS4;
    m_inst_func[INST_ID_V_FNMSFS4]      = &TracePrintOperand::ConvFNMSFS4;
    m_inst_func[INST_ID_V_LDVW_D16]     = &TracePrintOperand::ConvLDVW_D16;
    m_inst_func[INST_ID_V_STVW_D16]     = &TracePrintOperand::ConvSTVW_D16;
    m_inst_func[INST_ID_V_LDVQW_D16]    = &TracePrintOperand::ConvLDVQW_D16;
    m_inst_func[INST_ID_V_STVQW_D16]    = &TracePrintOperand::ConvSTVQW_D16;
    m_inst_func[INST_ID_V_LDVDW_D16]    = &TracePrintOperand::ConvLDVDW_D16;
    m_inst_func[INST_ID_V_STVDW_D16]    = &TracePrintOperand::ConvSTVDW_D16;
    m_inst_func[INST_ID_V_LDVZH4_D16]   = &TracePrintOperand::ConvLDVZH4_D16;
    m_inst_func[INST_ID_V_STVZH4_D16]   = &TracePrintOperand::ConvSTVZH4_D16;
    m_inst_func[INST_ID_V_CMOVFW4]      = &TracePrintOperand::ConvCMOVFW4;
    m_inst_func[INST_ID_V_SHFLVW4]      = &TracePrintOperand::ConvSHFLVW4;
    m_inst_func[INST_ID_V_CLIP_B]       = &TracePrintOperand::ConvCLIP_B;
    m_inst_func[INST_ID_V_CLIP_BU]      = &TracePrintOperand::ConvCLIP_BU;
    m_inst_func[INST_ID_V_CLIP_H]       = &TracePrintOperand::ConvCLIP_H;
    m_inst_func[INST_ID_V_CLIP_HU]      = &TracePrintOperand::ConvCLIP_HU;
    m_inst_func[INST_ID_V_LDLBU]        = &TracePrintOperand::ConvLDLBU;
    m_inst_func[INST_ID_V_LDB_INC]      = &TracePrintOperand::ConvLDB_INC;
    m_inst_func[INST_ID_V_LDBU_INC]     = &TracePrintOperand::ConvLDBU_INC;
    m_inst_func[INST_ID_V_LDB_DEC]      = &TracePrintOperand::ConvLDB_DEC;
    m_inst_func[INST_ID_V_LDBU_DEC]     = &TracePrintOperand::ConvLDBU_DEC;
    m_inst_func[INST_ID_V_STCB]         = &TracePrintOperand::ConvSTCB;
    m_inst_func[INST_ID_V_STB_INC]      = &TracePrintOperand::ConvSTB_INC;
    m_inst_func[INST_ID_V_STB_DEC]      = &TracePrintOperand::ConvSTB_DEC;
    m_inst_func[INST_ID_V_LDLHU]        = &TracePrintOperand::ConvLDLHU;
    m_inst_func[INST_ID_V_LDH_INC]      = &TracePrintOperand::ConvLDH_INC;
    m_inst_func[INST_ID_V_LDHU_INC]     = &TracePrintOperand::ConvLDHU_INC;
    m_inst_func[INST_ID_V_LDH_DEC]      = &TracePrintOperand::ConvLDH_DEC;
    m_inst_func[INST_ID_V_LDHU_DEC]     = &TracePrintOperand::ConvLDHU_DEC;
    m_inst_func[INST_ID_V_STCH]         = &TracePrintOperand::ConvSTCH;
    m_inst_func[INST_ID_V_STH_INC]      = &TracePrintOperand::ConvSTH_INC;
    m_inst_func[INST_ID_V_STH_DEC]      = &TracePrintOperand::ConvSTH_DEC;
    m_inst_func[INST_ID_V_LDW_INC]      = &TracePrintOperand::ConvLDW_INC;
    m_inst_func[INST_ID_V_LDW_DEC]      = &TracePrintOperand::ConvLDW_DEC;
    m_inst_func[INST_ID_V_STW_INC]      = &TracePrintOperand::ConvSTW_INC;
    m_inst_func[INST_ID_V_STW_DEC]      = &TracePrintOperand::ConvSTW_DEC;
    m_inst_func[INST_ID_V_RESBANK]      = &TracePrintOperand::ConvRESBANK;
    m_inst_func[INST_ID_V_CLL]          = &TracePrintOperand::ConvCLL;
    m_inst_func[INST_ID_V_STM_GSR]      = &TracePrintOperand::ConvSTM_GSR;
    m_inst_func[INST_ID_V_STM_MP]       = &TracePrintOperand::ConvSTM_MP;
    m_inst_func[INST_ID_V_LDM_GSR]      = &TracePrintOperand::ConvLDM_GSR;
    m_inst_func[INST_ID_V_LDM_MP]       = &TracePrintOperand::ConvLDM_MP;

    SetReplaceInfoValue ( W_31_28             , "W[31:28]",
                          -1, -1, -1, -1, 31, 28, false, false,  0,  0,  4);
    SetReplaceInfoValue ( WR_4_0              , "WR[4:0]",
                          -1, -1, -1, -1,  4,  0, false, false,  0,  0,  5);
    SetReplaceInfoValue ( WR_47_43            , "WR[47:43]",
                          -1, -1, -1, -1, 47, 43, false, false,  0,  0,  5);
    SetReplaceInfoValue ( WR_36_32            , "WR[36:32]",
                          -1, -1, -1, -1, 36, 32, false, false,  0,  0,  5);
    SetReplaceInfoValue ( WR_31_27            , "WR[31:27]",
                          -1, -1, -1, -1, 31, 27, false, false,  0,  0,  5);
    SetReplaceInfoValue ( WR_15_11            , "WR[15:11]",
                          -1, -1, -1, -1, 15, 11, false, false,  0,  0,  5);
    SetReplaceInfoValue ( V_4_0               , "V[4:0]",
                          -1, -1, -1, -1,  4,  0, false, false,  0,  0,  5);
    SetReplaceInfoValue ( V_29_27_4_0         , "V[29:27.4:0]",
                          -1, -1, 29, 27,  4,  0, false,  true,  0,  0,  8);
    SetReplaceInfoValue ( V_14_11             , "V[14:11]",
                          -1, -1, -1, -1, 14, 11, false, false,  0,  0,  4);
    SetReplaceInfoValue ( SR_4_0              , "SR[4:0]",
                          -1, -1, -1, -1,  4,  0, false, false,  0,  0,  5);
    SetReplaceInfoValue ( SR_15_11            , "SR[15:11]",
                          -1, -1, -1, -1, 15, 11, false, false,  0,  0,  5);
    SetReplaceInfoValue ( SEL_31_27           , "SEL[31:27]",
                          -1, -1, -1, -1, 31, 27, false, false,  0,  0,  5);
    SetReplaceInfoValue ( R_4_1_S1            , "R[4:1]<<1",
                          -1, -1, -1, -1,  4,  1, false, false,  1,  0,  5);
    SetReplaceInfoValue ( R_4_0               , "R[4:0]",
                          -1, -1, -1, -1,  4,  0, false, false,  0,  0,  5);
    SetReplaceInfoValue ( R_31_28_S1          , "R[31:28]<<1",
                          -1, -1, -1, -1, 31, 28, false, false,  1,  0,  5);
    SetReplaceInfoValue ( R_31_27             , "R[31:27]",
                          -1, -1, -1, -1, 31, 27, false, false,  0,  0,  5);
    SetReplaceInfoValue ( R_20_17_S1          , "R[20:17]<<1",
                          -1, -1, -1, -1, 20, 17, false, false,  1,  0,  5);
    SetReplaceInfoValue ( R_20_16             , "R[20:16]",
                          -1, -1, -1, -1, 20, 16, false, false,  0,  0,  5);
    SetReplaceInfoValue ( R_15_12_S1          , "R[15:12]<<1",
                          -1, -1, -1, -1, 15, 12, false, false,  1,  0,  5);
    SetReplaceInfoValue ( R_15_11             , "R[15:11]",
                          -1, -1, -1, -1, 15, 11, false, false,  0,  0,  5);
    SetReplaceInfoValue ( P_27_19_17          , "P[27.19:17]",
                          -1, -1, 27, 27, 19, 17, false,  true,  0,  0,  4);
    SetReplaceInfoValue ( PO_31_27            , "PO[31:27]",
                          -1, -1, -1, -1, 31, 27, false, false,  0,  0,  5);
    SetReplaceInfoValue ( L_31_21_0           , "L[31:21.0]",
                          -1, -1, 31, 21,  0,  0, false,  true,  0,  0, 12);
    SetReplaceInfoValue ( I_63_32             , "I[63:32]",
                          -1, -1, -1, -1, 63, 32, false, false,  0,  0, 32);
    SetReplaceInfoValue ( I_5_1               , "I[5:1]",
                          -1, -1, -1, -1,  5,  1, false, false,  0,  0,  5);
    SetReplaceInfoValue ( I_4_0               , "I[4:0]",
                          -1, -1, -1, -1,  4,  0, false, false,  0,  0,  5);
    SetReplaceInfoValue ( I_47_32             , "I[47:32]",
                          -1, -1, -1, -1, 47, 32, false, false,  0,  0, 16);
    SetReplaceInfoValue ( I_47_32_31_16       , "I[47:32.31:16]",
                          -1, -1, 47, 32, 31, 16, false,  true,  0,  0, 32);
    SetReplaceInfoValue ( I_3_0               , "I[3:0]",
                          -1, -1, -1, -1,  3,  0, false, false,  0,  0,  4);
    SetReplaceInfoValue ( I_31_27_4_0         , "I[31:27.4:0]",
                          -1, -1, 31, 27,  4,  0, false,  true,  0,  0, 10);
    SetReplaceInfoValue ( I_31_16             , "I[31:16]",
                          -1, -1, -1, -1, 31, 16, false, false,  0,  0, 16);
    SetReplaceInfoValue ( I_21_18_4_0         , "I[21:18.4:0]",
                          -1, -1, 21, 18,  4,  0, false,  true,  0,  0,  9);
    SetReplaceInfoValue ( I_15_11             , "I[15:11]",
                          -1, -1, -1, -1, 15, 11, false, false,  0,  0,  5);
    SetReplaceInfoValue ( IU_5_1              , "IU[5:1]",
                          -1, -1, -1, -1,  5,  1, false, false,  0,  0,  5);
    SetReplaceInfoValue ( IU_5_0              , "IU[5:0]",
                          -1, -1, -1, -1,  5,  0, false, false,  0,  0,  6);
    SetReplaceInfoValue ( IU_4_0              , "IU[4:0]",
                          -1, -1, -1, -1,  4,  0, false, false,  0,  0,  5);
    SetReplaceInfoValue ( IU_31_16            , "IU[31:16]",
                          -1, -1, -1, -1, 31, 16, false, false,  0,  0, 16);
    SetReplaceInfoValue ( IU_29_27            , "IU[29:27]",
                          -1, -1, -1, -1, 29, 27, false, false,  0,  0,  3);
    SetReplaceInfoValue ( IU_24_21            , "IU[24:21]",
                          -1, -1, -1, -1, 24, 21, false, false,  0,  0,  4);
    SetReplaceInfoValue ( IU_22_21            , "IU[22:21]",
                          -1, -1, -1, -1, 22, 21, false, false,  0,  0,  2);
    SetReplaceInfoValue ( IU_21_21            , "IU[21:21]",
                          -1, -1, -1, -1, 21, 21, false, false,  0,  0,  1);
    SetReplaceInfoValue ( IU_21_21_42_32      , "IU[21:21.42:32]",
                          -1, -1, 21, 21, 42, 32, false,  true,  0,  0, 12);
    SetReplaceInfoValue ( IU_21_18_4_0        , "IU[21:18.4:0]",
                          -1, -1, 21, 18,  4,  0, false,  true,  0,  0,  9);
    SetReplaceInfoValue ( IU_1_0              , "IU[1:0]",
                          -1, -1, -1, -1,  1,  0, false, false,  0,  0,  2);
    SetReplaceInfoValue ( FCO_30_27           , "FCO[30:27]",
                          -1, -1, -1, -1, 30, 27, false, false,  0,  0,  4);
    SetReplaceInfoValue ( FCO_20_17           , "FCO[20:17]",
                          -1, -1, -1, -1, 20, 17, false, false,  0,  0,  4);
    SetReplaceInfoValue ( FCB_19_17           , "FCB[19:17]",
                          -1, -1, -1, -1, 19, 17, false, false,  0,  0,  3);
    SetReplaceInfoValue ( E_31_27             , "E[31:27]",
                          -1, -1, -1, -1, 31, 27, false, false,  0,  0,  5);
    SetReplaceInfoValue ( E_15_11             , "E[15:11]",
                          -1, -1, -1, -1, 15, 11, false, false,  0,  0,  5);
    SetReplaceInfoValue ( D_47_36_S4          , "D[47:36]<<4",
                          -1, -1, -1, -1, 47, 36, false, false,  4,  0, 16);
    SetReplaceInfoValue ( D_47_35_S3          , "D[47:35]<<3",
                          -1, -1, -1, -1, 47, 35, false, false,  3,  0, 16);
    SetReplaceInfoValue ( D_47_34_S2          , "D[47:34]<<2",
                          -1, -1, -1, -1, 47, 34, false, false,  2,  0, 16);
    SetReplaceInfoValue ( D_31_16             , "D[31:16]",
                          -1, -1, -1, -1, 31, 16, false, false,  0,  0, 16);
    SetReplaceInfoValue ( DU_6_1_S2           , "DU[6:1]<<2",
                          -1, -1, -1, -1,  6,  1, false, false,  2,  0,  8);
    SetReplaceInfoValue ( DU_6_0_S1           , "DU[6:0]<<1",
                          -1, -1, -1, -1,  6,  0, false, false,  1,  0,  8);
    SetReplaceInfoValue ( DU_6_0              , "DU[6:0]",
                          -1, -1, -1, -1,  6,  0, false, false,  0,  0,  7);
    SetReplaceInfoValue ( DU_47_32_31_17_S1   , "DU[47:32.31:17]<<1",
                          -1, -1, 47, 32, 31, 17, false,  true,  1,  0, 32);
    SetReplaceInfoValue ( DU_47_32_26_21_S1   , "DU[47:32.26:21]<<1",
                          -1, -1, 47, 32, 26, 21, false,  true,  1,  0, 23);
    SetReplaceInfoValue ( DU_47_32_26_20      , "DU[47:32.26:20]",
                          -1, -1, 47, 32, 26, 20, false,  true,  0,  0, 23);
    SetReplaceInfoValue ( DU_3_0_S1           , "DU[3:0]<<1",
                          -1, -1, -1, -1,  3,  0, false, false,  1,  0,  5);
    SetReplaceInfoValue ( DU_3_0              , "DU[3:0]",
                          -1, -1, -1, -1,  3,  0, false, false,  0,  0,  4);
    SetReplaceInfoValue ( DU_31_17_S1         , "DU[31:17]<<1",
                          -1, -1, -1, -1, 31, 17, false, false,  1,  0, 16);
    SetReplaceInfoValue ( DU_31_17_5          , "DU[31:17.5]",
                          -1, -1, 31, 17,  5,  5, false,  true,  0,  0, 16);
    SetReplaceInfoValue ( DU_31_16            , "DU[31:16]",
                          -1, -1, -1, -1, 31, 16, false, false,  0,  0, 16);
    SetReplaceInfoValue ( C_4_0               , "C[4:0]",
                          -1, -1, -1, -1,  4,  0, false, false,  0,  0,  5);
    SetReplaceInfoValue ( C_20_17             , "C[20:17]",
                          -1, -1, -1, -1, 20, 17, false, false,  0,  0,  4);
    SetReplaceInfoValue ( CA_12_11_31_27      , "CA[12:11.31:27]",
                          -1, -1, 12, 11, 31, 27, false,  true,  0,  0,  7);
    SetReplaceInfoValue ( B_13_11             , "B[13:11]",
                          -1, -1, -1, -1, 13, 11, false, false,  0,  0,  3);
    SetReplaceInfoValue ( BRA_5_0_31_17_S1    , "BRA[5:0.31:17]<<1",
                          -1, -1,  5,  0, 31, 17, false,  true,  1,  0, 22);
    SetReplaceInfoValue ( BD_4_4_31_17_S1     , "BD[4:4.31:17]<<1",
                          -1, -1,  4,  4, 31, 17, false,  true,  1,  0, 17);
    SetReplaceInfoValue ( BD_15_11_6_4_S1     , "BD[15:11.6:4]<<1",
                          -1, -1, 15, 11,  6,  4, false,  true,  1,  0,  9);
}
