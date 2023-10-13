/*
 * (c) 2011,2012 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */


#include "./cmp_runner.h"


/** \file cmp_runner.cpp
 * \brief CompRunner common interface source file.
 *
 * This file contains the internal functions in CompRunner: G4MHv2, ...
 */


/** \var ComprunnerG4MHv2 obj_cmp_g4mh_v2
 * \brief Object of CompRunner G4MHv2 class.
 *
 * This variable will call the implementation of CompRunner G4MHv2.
 */
ComprunnerG4MHv2 obj_cmp_g4mh_v2;


uint32_t DPI_CForestInit (uint32_t rtl_pe_number, uint32_t cmprunner_log, char *cmprunner_log_path, char *srec_file, char *python_name, uint32_t Cmprunner_Enable)
{
    return obj_cmp_g4mh_v2.G4MHv2_CForestInit(rtl_pe_number, cmprunner_log, cmprunner_log_path, srec_file, python_name, Cmprunner_Enable);
}


void DPI_CForestClose (void)
{
    return obj_cmp_g4mh_v2.G4MHv2_CForestClose();
}


uint32_t DPI_GetTotalError (uint32_t rtl_peid)
{
    return obj_cmp_g4mh_v2.G4MHv2_GetTotalError(rtl_peid);
}


uint32_t DPI_CmpStep (uint32_t rtl_peid, uint64_t rtl_dp_time, uint64_t rtl_ea_time, uint64_t rtl_cm_time, uint32_t rtl_cycle, uint32_t rtl_pc, uint32_t rtl_last_commit, uint32_t rtl_c2b1_commit, uint32_t rtl_ooo_commit,
                      uint32_t rtl_mcyc_commit, uint32_t rtl_regwrite_en, uint32_t rtl_reg_num, svLogicVecVal* rtl_gr_val, svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1,
                      svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3, uint32_t rtl_flagwrite_en, uint32_t rtl_flag_val, uint32_t rtl_brk_ch, uint32_t rtl_nblk_id,
                      uint32_t rtl_xdump_ctrl)
{
    return obj_cmp_g4mh_v2.G4MHv2_CmpStep (rtl_peid, rtl_dp_time, rtl_ea_time, rtl_cm_time, rtl_cycle, rtl_pc, rtl_last_commit, rtl_c2b1_commit, rtl_ooo_commit,
                                           rtl_mcyc_commit, rtl_regwrite_en, rtl_reg_num, rtl_gr_val, rtl_wr_val_0, rtl_wr_val_1,
                                           rtl_wr_val_2, rtl_wr_val_3, rtl_flagwrite_en, rtl_flag_val, rtl_brk_ch, rtl_nblk_id, rtl_xdump_ctrl);
}


    uint32_t DPI_CmpNbWb (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_nonblk_id, svLogicVecVal* rtl_gr_val,
                          svLogicVecVal* rtl_wr_val_0, svLogicVecVal* rtl_wr_val_1, svLogicVecVal* rtl_wr_val_2, svLogicVecVal* rtl_wr_val_3)
{
    return obj_cmp_g4mh_v2.G4MHv2_CmpNbWb (rtl_peid, rtl_time, rtl_cycle, rtl_nonblk_id, rtl_gr_val,
                                           rtl_wr_val_0, rtl_wr_val_1, rtl_wr_val_2,  rtl_wr_val_3);
}

uint32_t DPI_HaltSnoozeReleaseDump (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_Halt_dump_en)
{
    return obj_cmp_g4mh_v2.G4MHv2_HaltSnoozeReleaseDump (rtl_peid, rtl_time, rtl_cycle, rtl_Halt_dump_en);
}

uint32_t DPI_ExpSyserrNotify (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                              uint32_t rtl_syserr_cause, uint32_t rtl_operand_syserr, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid, uint32_t rtl_bus_slave_xdmp)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpSyserrNotify (rtl_peid, rtl_time, rtl_cycle, rtl_exp_pc, rtl_syserr_cause, rtl_operand_syserr, rtl_operand_syserr_mode, rtl_operand_syserr_gpid, rtl_bus_slave_xdmp);
}

uint32_t DPI_ExpOperandSyserrPend (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                                   uint32_t rtl_operand_syserr_cause, uint32_t rtl_operand_syserr_mode, uint32_t rtl_operand_syserr_gpid, uint32_t rtl_commit, uint32_t rtl_bus_slave_xdmp)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpOperandSyserrPend (rtl_peid, rtl_time, rtl_cycle, rtl_exp_pc, rtl_operand_syserr_cause, rtl_operand_syserr_mode, rtl_operand_syserr_gpid, rtl_commit, rtl_bus_slave_xdmp);
}

uint32_t DPI_ExpFeIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc,
                                 uint32_t rtl_fe_type, uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en, uint32_t rtl_commit)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpFeIntc1ReqToCpu (rtl_peid, rtl_exp_mode_id, rtl_time, rtl_cycle, rtl_exp_pc,
                                                      rtl_fe_type, rtl_feint_ch, rtl_bge_val, rtl_intc1_en, rtl_commit);
};


uint32_t DPI_ExpFeCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_fe_type, uint32_t rtl_feint_ch, uint32_t rtl_bge_val, uint32_t rtl_intc1_en)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpFeCpuAckToIntc1 (rtl_peid, rtl_exp_mode_id, rtl_time, rtl_cycle, rtl_exp_pc, rtl_fe_type,
                                                      rtl_feint_ch, rtl_bge_val, rtl_intc1_en);
}


uint32_t DPI_ExpEiIntc1ReqToCpu (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_ei_type,
                                uint32_t rtl_int_ch, uint32_t rtl_int_pri, uint32_t rtl_bgpr_val, uint32_t rtl_eitb_read, uint32_t rtl_intc1_en, uint32_t rtl_commit)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpEiIntc1ReqToCpu (rtl_peid, rtl_exp_mode_id, rtl_time, rtl_cycle, rtl_exp_pc, rtl_ei_type,
                                               rtl_int_ch, rtl_int_pri, rtl_bgpr_val, rtl_eitb_read, rtl_intc1_en, rtl_commit);
}


uint32_t DPI_ExpEiCpuAckToIntc1 (uint32_t rtl_peid, uint32_t rtl_exp_mode_id, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, uint32_t rtl_ei_type,
                                svLogicVecVal* rtl_int_ch, svLogicVecVal* rtl_int_pri, uint32_t rtl_bgpr_val, uint32_t rtl_eitb_read, uint32_t rtl_intc1_en)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpEiCpuAckToIntc1 (rtl_peid, rtl_exp_mode_id, rtl_time, rtl_cycle, rtl_exp_pc, rtl_ei_type,
                                               rtl_int_ch, rtl_int_pri, rtl_bgpr_val, rtl_eitb_read, rtl_intc1_en);
}

uint32_t DPI_ExpDbintRcuReqToCpu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, svLogicVecVal* rtl_brk_ch, uint32_t rtl_commit)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpDbintRcuReqToCpu (rtl_peid, rtl_time, rtl_cycle, rtl_exp_pc, rtl_brk_ch, rtl_commit);
}

uint32_t DPI_ExpDbCpuAckToRcu (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc, svLogicVecVal* rtl_db_int_type, svLogicVecVal* rtl_brk_ch)
{
    return obj_cmp_g4mh_v2.G4MHv2_ExpDbCpuAckToRcu (rtl_peid, rtl_time, rtl_cycle, rtl_exp_pc, rtl_db_int_type, rtl_brk_ch);
}

uint32_t DPI_CancelEiintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    return obj_cmp_g4mh_v2.G4MHv2_CancelEiintReq(rtl_peid, rtl_time, rtl_cycle);
}

uint32_t DPI_CancelFeintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    return obj_cmp_g4mh_v2.G4MHv2_CancelFeintReq(rtl_peid, rtl_time, rtl_cycle);
}

uint32_t DPI_CancelFenmiReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    return obj_cmp_g4mh_v2.G4MHv2_CancelFenmiReq(rtl_peid, rtl_time, rtl_cycle);
}

uint32_t DPI_CancelDbintReq(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle)
{
    return obj_cmp_g4mh_v2.G4MHv2_CancelDbintReq(rtl_peid, rtl_time, rtl_cycle);
}

uint32_t DPI_ResetPE(uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, uint32_t rtl_exp_pc)
{
    return obj_cmp_g4mh_v2.G4MHv2_ResetPE(rtl_peid, rtl_time, rtl_cycle, rtl_exp_pc);
}

uint32_t DPI_CmpSReg (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle,
                      svLogicVecVal* rtl_sreg_id, svLogicVecVal* rtl_sreg_ch,
                      svLogicVecVal* rtl_sreg_val, svLogicVecVal* rtl_sreg_mask)
{
    return obj_cmp_g4mh_v2.G4MHv2_CmpSReg(rtl_peid, rtl_time, rtl_cycle, rtl_sreg_id, rtl_sreg_ch, rtl_sreg_val, rtl_sreg_mask);
}


uint32_t DPI_MemoryDataCheck (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_mem_addr, svLogicVecVal* rtl_mem_val)
{
    return obj_cmp_g4mh_v2.G4MHv2_MemoryDataCheck (rtl_peid, rtl_time, rtl_cycle, rtl_mem_addr, rtl_mem_val);
}


uint32_t DPI_CmpPeState (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_cycle, svLogicVecVal* rtl_state)
{
    return obj_cmp_g4mh_v2.G4MHv2_CmpPeState (rtl_peid, rtl_time, rtl_cycle, rtl_state);
}


uint32_t DPI_CmpSetIssueCycle (uint32_t rtl_peid, uint64_t rtl_time, uint32_t rtl_inst_id)
{
    return obj_cmp_g4mh_v2.G4MHv2_CmpSetIssueCycle (rtl_peid, rtl_time, rtl_inst_id);
}


uint32_t DPI_CmpSetIssueFlush (uint32_t rtl_peid)
{
    return obj_cmp_g4mh_v2.G4MHv2_CmpSetIssueFlush(rtl_peid);
}
