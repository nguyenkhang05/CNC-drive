//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file instr_set_impl.h
 * \brief Defines the Instruction Set implementor class
 */

#ifndef _RH850G4_INSTR_SET_IMPL_H_
#define _RH850G4_INSTR_SET_IMPL_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/instr_set_if.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class code_gen_if_t;
}

namespace rh850g4
{
  class memory_if_t;
  class custom_if_t;
  class instruction_base_impl_t;
  class operand_if_t;
  class cpu_state_if_t;
}

//------------------------------------------------------------------------------
// Instruction set implementations
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief Instruction set implementor class providing access to all
   * instructions in the Fast-ISS.
   */
  class instr_set_impl_t : public rh850g4::instr_set_if_t
  {
  public:
    /**
     * \brief Instruction set implementor constructor
     * \param code_gen_impl A pointer to the code generator implementor
     * \param memory_impl A pointer to the code memory access implementor
     * \param operand_impl A pointer to the operand extraction implementor
     * \param cpu_state_impl A pointer to the cpu state implementor
     * \param custom_impl A pointer to the custom generator implementor
     */          
    instr_set_impl_t
    (
      fastiss::code_gen_if_t *code_gen_impl,
      rh850g4::memory_if_t *memory_impl,
      rh850g4::operand_if_t *operand_impl,
      rh850g4::cpu_state_if_t *cpu_state_impl,
      rh850g4::custom_if_t *custom_impl
    );
    /**
     * \brief Instruction set implementor destructor
     */          
    virtual ~instr_set_impl_t();

    /**
     * \brief Provides access to the "add reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_add_reg1_reg2(void);
    /**
     * \brief Provides access to the "add imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_add_imm5_reg2(void);
    /**
     * \brief Provides access to the "addi imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addi_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "adf cccc, reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_adf_cccc_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "and reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_and_reg1_reg2(void);
    /**
     * \brief Provides access to the "andi imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_andi_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "bcccc disp9" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_b_cccc_disp9(void);
    /**
     * \brief Provides access to the "bsh reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_bsh_reg2_reg3(void);
    /**
     * \brief Provides access to the "bsw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_bsw_reg2_reg3(void);
    /**
     * \brief Provides access to the "callt imm6" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_callt_imm6(void);
    /**
     * \brief Provides access to the "caxi [reg1], reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_caxi_ind_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "clr1 bit#3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_clr1_bit_3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "clr1 reg2, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_clr1_reg2_ind_reg1(void);
    /**
     * \brief Provides access to the "cmov cccc, reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmov_cccc_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "cmov cccc, imm5, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmov_cccc_imm5_reg2_reg3(void);
    /**
     * \brief Provides access to the "cmp reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmp_reg1_reg2(void);
    /**
     * \brief Provides access to the "cmp imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmp_imm5_reg2(void);
    /**
     * \brief Provides access to the "ctret" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ctret(void);
    /**
     * \brief Provides access to the "di" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_di(void);
    /**
     * \brief Provides access to the "dispose imm5, list12" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_dispose_imm5_list12(void);
    /**
     * \brief Provides access to the "dispose imm5, list12, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_dispose_imm5_list12_ind_reg1(void);
    /**
     * \brief Provides access to the "div reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_div_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "switch reg1" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_switch_reg1(void);
    /**
     * \brief Provides access to the "divh reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divh_reg1_reg2(void);
    /**
     * \brief Provides access to the "divh reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divh_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "divhu reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divhu_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "divq reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divq_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "divqu reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divqu_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "divu reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divu_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "ei" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ei(void);
    /**
     * \brief Provides access to the "eiret" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_eiret(void);
    /**
     * \brief Provides access to the "feret" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_feret(void);
    /**
     * \brief Provides access to the "fetrap vector" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fetrap_vector(void);
    /**
     * \brief Provides access to the "halt" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_halt(void);
    /**
     * \brief Provides access to the "hsh reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_hsh_reg2_reg3(void);
    /**
     * \brief Provides access to the "hsw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_hsw_reg2_reg3(void);
    /**
     * \brief Provides access to the "jr disp22" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_jr_disp22(void);
    /**
     * \brief Provides access to the "jr disp32" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_jr_disp32(void);
    /**
     * \brief Provides access to the "jarl disp22, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_jarl_disp22_reg2(void);
    /**
     * \brief Provides access to the "jarl disp32, reg1" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_jarl_disp32_reg1(void);
    /**
     * \brief Provides access to the "jmp [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_jmp_ind_reg1(void);
    /**
     * \brief Provides access to the "jmp disp32[reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_jmp_disp32_ind_reg1(void);
    /**
     * \brief Provides access to the "prepare list12, imm5" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_prepare_list12_imm5(void);
    /**
     * \brief Provides access to the "prepare list12, imm5, bits_ff" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_prepare_list12_imm5_bits_ff(void);
    /**
     * \brief Provides access to the "ld.b disp16 [reg1], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_b_disp16_ind_reg1_reg2(void);
    /**
     * \brief Provides access to the "ld.b disp23 [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_b_disp23_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ld.b [reg1]+, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_b_ind_reg1_inc_reg3(void);
    /**
     * \brief Provides access to the "ld.b [reg1]-, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_b_ind_reg1_dec_reg3(void);
    /**
     * \brief Provides access to the "ld.bu disp23 [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_bu_disp23_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ld.bu disp16 [reg1], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_bu_disp16_ind_reg1_reg2(void);
    /**
     * \brief Provides access to the "ld.bu [reg1]+, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_bu_ind_reg1_inc_reg3(void);
    /**
     * \brief Provides access to the "ld.bu [reg1]-, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_bu_ind_reg1_dec_reg3(void);
    /**
     * \brief Provides access to the "ld.h disp16 [reg1], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_h_disp16_ind_reg1_reg2(void);
    /**
     * \brief Provides access to the "ld.h disp23 [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_h_disp23_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ld.h [reg1]+, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_h_ind_reg1_inc_reg3(void);
    /**
     * \brief Provides access to the "ld.h [reg1]-, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_h_ind_reg1_dec_reg3(void);
    /**
     * \brief Provides access to the "ld.hu disp16 [reg1], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_hu_disp16_ind_reg1_reg2(void);
    /**
     * \brief Provides access to the "ld.hu disp23 [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_hu_disp23_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ld.hu [reg1]+, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_hu_ind_reg1_inc_reg3(void);
    /**
     * \brief Provides access to the "ld.hu [reg1]-, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_hu_ind_reg1_dec_reg3(void);
    /**
     * \brief Provides access to the "ld.w disp16 [reg1], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_w_disp16_ind_reg1_reg2(void);
    /**
     * \brief Provides access to the "ld.w disp23 [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_w_disp23_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ld.w [reg1]+, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_w_ind_reg1_inc_reg3(void);
    /**
     * \brief Provides access to the "ld.w [reg1]-, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_w_ind_reg1_dec_reg3(void);
    /**
     * \brief Provides access to the "ldsr reg2, regID, selID" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldsr_reg2_regid_selid(void);
    /**
     * \brief Provides access to the "mac reg1, reg2, reg3, reg4" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mac_reg1_reg2_reg3_reg4(void);
    /**
     * \brief Provides access to the "macu reg1, reg2, reg3, reg4" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_macu_reg1_reg2_reg3_reg4(void);
    /**
     * \brief Provides access to the "nop" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_nop(void);
    /**
     * \brief Provides access to the "mov reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mov_reg1_reg2(void);
    /**
     * \brief Provides access to the "mov imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mov_imm5_reg2(void);
    /**
     * \brief Provides access to the "mov imm32, reg1" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mov_imm32_reg1(void);
    /**
     * \brief Provides access to the "movea imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_movea_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "movhi imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_movhi_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "mul reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mul_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "sxh reg1" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sxh_reg1(void);
    /**
     * \brief Provides access to the "mul imm9, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mul_imm9_reg2_reg3(void);
    /**
     * \brief Provides access to the "mulh reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulh_reg1_reg2(void);
    /**
     * \brief Provides access to the "mulh imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulh_imm5_reg2(void);
    /**
     * \brief Provides access to the "mulhi imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulhi_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "mulu reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulu_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "mulu imm9, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulu_imm9_reg2_reg3(void);
    /**
     * \brief Provides access to the "not reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_not_reg1_reg2(void);
    /**
     * \brief Provides access to the "not1 bit#3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_not1_bit_3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "not1 reg2, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_not1_reg2_ind_reg1(void);
    /**
     * \brief Provides access to the "or reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_or_reg1_reg2(void);
    /**
     * \brief Provides access to the "ori imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ori_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "rie" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_rie(void);
    /**
     * \brief Provides access to the "rie imm5, imm4" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_rie_imm5_imm4(void);
    /**
     * \brief Provides access to the "sar reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sar_reg1_reg2(void);
    /**
     * \brief Provides access to the "sar imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sar_imm5_reg2(void);
    /**
     * \brief Provides access to the "sar reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sar_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "sasf cccc, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sasf_cccc_reg2(void);
    /**
     * \brief Provides access to the "sxb reg1" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sxb_reg1(void);
    /**
     * \brief Provides access to the "zxb reg1" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_zxb_reg1(void);
    /**
     * \brief Provides access to the "zxh reg1" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_zxh_reg1(void);
    /**
     * \brief Provides access to the "satadd reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_satadd_reg1_reg2(void);
    /**
     * \brief Provides access to the "satadd imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_satadd_imm5_reg2(void);
    /**
     * \brief Provides access to the "satadd reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_satadd_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "satsub reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_satsub_reg1_reg2(void);
    /**
     * \brief Provides access to the "satsub reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_satsub_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "satsubi imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_satsubi_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "satsubr reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_satsubr_reg1_reg2(void);
    /**
     * \brief Provides access to the "sbf cccc, reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sbf_cccc_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "sch0l reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sch0l_reg2_reg3(void);
    /**
     * \brief Provides access to the "sch0r reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sch0r_reg2_reg3(void);
    /**
     * \brief Provides access to the "sch1l reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sch1l_reg2_reg3(void);
    /**
     * \brief Provides access to the "sch1r reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sch1r_reg2_reg3(void);
    /**
     * \brief Provides access to the "set1 bit#3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_set1_bit_3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "set1 reg2, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_set1_reg2_ind_reg1(void);
    /**
     * \brief Provides access to the "setf cccc, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_setf_cccc_reg2(void);
    /**
     * \brief Provides access to the "shl reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_shl_reg1_reg2(void);
    /**
     * \brief Provides access to the "shl imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_shl_imm5_reg2(void);
    /**
     * \brief Provides access to the "shl reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_shl_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "shr reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_shr_reg1_reg2(void);
    /**
     * \brief Provides access to the "shr imm5, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_shr_imm5_reg2(void);
    /**
     * \brief Provides access to the "shr reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_shr_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "sld.b disp7 [ep], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sld_dot_b_disp7_ind_ep_reg2(void);
    /**
     * \brief Provides access to the "sld.bu disp4 [ep], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sld_dot_bu_disp4_ind_ep_reg2(void);
    /**
     * \brief Provides access to the "sld.h disp8 [ep], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sld_dot_h_disp8_ind_ep_reg2(void);
    /**
     * \brief Provides access to the "sld.hu disp5 [ep], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sld_dot_hu_disp5_ind_ep_reg2(void);
    /**
     * \brief Provides access to the "sld.w disp8 [ep], reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sld_dot_w_disp8_ind_ep_reg2(void);
    /**
     * \brief Provides access to the "sst.b reg2, disp7 [ep]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sst_dot_b_reg2_disp7_ind_ep(void);
    /**
     * \brief Provides access to the "sst.h reg2, disp8 [ep]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sst_dot_h_reg2_disp8_ind_ep(void);
    /**
     * \brief Provides access to the "sst.w reg2, disp8 [ep]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sst_dot_w_reg2_disp8_ind_ep(void);
    /**
     * \brief Provides access to the "st.b reg2, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_b_reg2_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "st.b reg3, disp23 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_b_reg3_disp23_ind_reg1(void);
    /**
     * \brief Provides access to the "st.b reg3, [reg1]+" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_b_reg3_ind_reg1_inc(void);
    /**
     * \brief Provides access to the "st.b reg3, [reg1]-" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_b_reg3_ind_reg1_dec(void);
    /**
     * \brief Provides access to the "st.h reg2, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_h_reg2_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "st.h reg3, disp23 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_h_reg3_disp23_ind_reg1(void);
    /**
     * \brief Provides access to the "st.h reg3, [reg1]+" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_h_reg3_ind_reg1_inc(void);
    /**
     * \brief Provides access to the "st.h reg3, [reg1]-" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_h_reg3_ind_reg1_dec(void);
    /**
     * \brief Provides access to the "st.w reg2, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_w_reg2_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "st.w reg3, disp23 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_w_reg3_disp23_ind_reg1(void);
    /**
     * \brief Provides access to the "st.w reg3, [reg1]+" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_w_reg3_ind_reg1_inc(void);
    /**
     * \brief Provides access to the "st.w reg3, [reg1]-" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_w_reg3_ind_reg1_dec(void);
    /**
     * \brief Provides access to the "stsr regID, reg2, selID" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stsr_regid_reg2_selid(void);
    /**
     * \brief Provides access to the "sub reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sub_reg1_reg2(void);
    /**
     * \brief Provides access to the "subr reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subr_reg1_reg2(void);
    /**
     * \brief Provides access to the "synce" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_synce(void);
    /**
     * \brief Provides access to the "syncm" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_syncm(void);
    /**
     * \brief Provides access to the "syncp" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_syncp(void);
    /**
     * \brief Provides access to the "syscall vector" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_syscall_vector(void);
    /**
     * \brief Provides access to the "trap vector" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trap_vector(void);
    /**
     * \brief Provides access to the "tst reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_tst_reg1_reg2(void);
    /**
     * \brief Provides access to the "tst1 bit#3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_tst1_bit_3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "tst1 reg2, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_tst1_reg2_ind_reg1(void);
    /**
     * \brief Provides access to the "xor reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_xor_reg1_reg2(void);
    /**
     * \brief Provides access to the "xori imm16, reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_xori_imm16_reg1_reg2(void);
    /**
     * \brief Provides access to the "absf.d reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_absf_dot_d_reg2_reg3(void);
    /**
     * \brief Provides access to the "absf.s reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_absf_dot_s_reg2_reg3(void);
    /**
     * \brief Provides access to the "addf.d reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addf_dot_d_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "addf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.dl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_dl_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.dul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_dul_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.duw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_duw_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.dw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_dw_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.sl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_sl_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.sul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_sul_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.suw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_suw_reg2_reg3(void);
    /**
     * \brief Provides access to the "ceilf.sw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_sw_reg2_reg3(void);
    /**
     * \brief Provides access to the "cmovf.d fcbit, reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmovf_dot_d_fcbit_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "cmovf.s fcbit, reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmovf_dot_s_fcbit_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "cmpf.d fcond, reg2, reg1, fcbit" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmpf_dot_d_fcond_reg2_reg1_fcbit(void);
    /**
     * \brief Provides access to the "cmpf.s fcond, reg2, reg1, fcbit" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmpf_dot_s_fcond_reg2_reg1_fcbit(void);
    /**
     * \brief Provides access to the "cvtf.dl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_dl_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.ds reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_ds_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.dul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_dul_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.duw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_duw_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.dw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_dw_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.ld reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_ld_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.ls reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_ls_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.sd reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_sd_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.sl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_sl_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.sul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_sul_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.suw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_suw_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.sw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_sw_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.uld reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_uld_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.uls reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_uls_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.uwd reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_uwd_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.uws reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_uws_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.wd reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_wd_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.ws reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_ws_reg2_reg3(void);
    /**
     * \brief Provides access to the "divf.d reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divf_dot_d_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "divf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.dl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_dl_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.dul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_dul_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.duw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_duw_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.dw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_dw_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.sl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_sl_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.sul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_sul_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.suw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_suw_reg2_reg3(void);
    /**
     * \brief Provides access to the "floorf.sw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_sw_reg2_reg3(void);
    /**
     * \brief Provides access to the "maddf.s reg1, reg2, reg3, reg4" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_maddf_dot_s_reg1_reg2_reg3_reg4(void);
    /**
     * \brief Provides access to the "maxf.d reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_maxf_dot_d_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "maxf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_maxf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "minf.d reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_minf_dot_d_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "minf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_minf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "msubf.s reg1, reg2, reg3, reg4" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_msubf_dot_s_reg1_reg2_reg3_reg4(void);
    /**
     * \brief Provides access to the "mulf.d reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulf_dot_d_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "mulf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "negf.d reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_negf_dot_d_reg2_reg3(void);
    /**
     * \brief Provides access to the "negf.s reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_negf_dot_s_reg2_reg3(void);
    /**
     * \brief Provides access to the "nmaddf.s reg1, reg2, reg3, reg4" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_nmaddf_dot_s_reg1_reg2_reg3_reg4(void);
    /**
     * \brief Provides access to the "nmsubf.s reg1, reg2, reg3, reg4" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_nmsubf_dot_s_reg1_reg2_reg3_reg4(void);
    /**
     * \brief Provides access to the "recipf.d reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_recipf_dot_d_reg2_reg3(void);
    /**
     * \brief Provides access to the "recipf.s reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_recipf_dot_s_reg2_reg3(void);
    /**
     * \brief Provides access to the "rsqrtf.d reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_rsqrtf_dot_d_reg2_reg3(void);
    /**
     * \brief Provides access to the "rsqrtf.s reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_rsqrtf_dot_s_reg2_reg3(void);
    /**
     * \brief Provides access to the "sqrtf.d reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sqrtf_dot_d_reg2_reg3(void);
    /**
     * \brief Provides access to the "sqrtf.s reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sqrtf_dot_s_reg2_reg3(void);
    /**
     * \brief Provides access to the "subf.d reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subf_dot_d_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "subf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "trfsr fcbit" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trfsr_fcbit(void);
    /**
     * \brief Provides access to the "trncf.dl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_dl_reg2_reg3(void);
    /**
     * \brief Provides access to the "trncf.dul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_dul_reg2_reg3(void);
    /**
     * \brief Provides access to the "trncf.duw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_duw_reg2_reg3(void);
    /**
     * \brief Provides access to the "trncf.dw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_dw_reg2_reg3(void);
    /**
     * \brief Provides access to the "trncf.sl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_sl_reg2_reg3(void);
    /**
     * \brief Provides access to the "trncf.sul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_sul_reg2_reg3(void);
    /**
     * \brief Provides access to the "trncf.suw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_suw_reg2_reg3(void);
    /**
     * \brief Provides access to the "trncf.sw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_sw_reg2_reg3(void);
    /**
     * \brief Provides access to the "bcccc disp17" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_b_cccc_disp17(void);
    /**
     * \brief Provides access to the "binsu reg1, lsb, msb, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_binsu_reg1_lsb_msb_reg2(void);
    /**
     * \brief Provides access to the "binsm reg1, lsb, msb, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_binsm_reg1_lsb_msb_reg2(void);
    /**
     * \brief Provides access to the "binsl reg1, lsb, msb, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_binsl_reg1_lsb_msb_reg2(void);
    /**
     * \brief Provides access to the "jarl [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_jarl_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ld.dw disp23 [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ld_dot_dw_disp23_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ldl.bu [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldl_dot_bu_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ldl.hu [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldl_dot_hu_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "ldl.w [reg1], reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldl_dot_w_ind_reg1_reg3(void);
    /**
     * \brief Provides access to the "loop reg1, disp16" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_loop_reg1_disp16(void);
    /**
     * \brief Provides access to the "popsp rh-rt" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_popsp_rh_dec_rt(void);
    /**
     * \brief Provides access to the "pushsp rh-rt" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_pushsp_rh_dec_rt(void);
    /**
     * \brief Provides access to the "rotl imm5, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_rotl_imm5_reg2_reg3(void);
    /**
     * \brief Provides access to the "rotl reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_rotl_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "snooze" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_snooze(void);
    /**
     * \brief Provides access to the "st.dw reg3, disp23 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_st_dot_dw_reg3_disp23_ind_reg1(void);
    /**
     * \brief Provides access to the "stc.b reg3, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stc_dot_b_reg3_ind_reg1(void);
    /**
     * \brief Provides access to the "stc.h reg3, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stc_dot_h_reg3_ind_reg1(void);
    /**
     * \brief Provides access to the "stc.w reg3, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stc_dot_w_reg3_ind_reg1(void);
    /**
     * \brief Provides access to the "synci" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_synci(void);
    /**
     * \brief Provides access to the "roundf.dl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_dl_reg2_reg3(void);
    /**
     * \brief Provides access to the "roundf.dul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_dul_reg2_reg3(void);
    /**
     * \brief Provides access to the "roundf.duw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_duw_reg2_reg3(void);
    /**
     * \brief Provides access to the "roundf.dw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_dw_reg2_reg3(void);
    /**
     * \brief Provides access to the "roundf.sl reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_sl_reg2_reg3(void);
    /**
     * \brief Provides access to the "roundf.sul reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_sul_reg2_reg3(void);
    /**
     * \brief Provides access to the "roundf.suw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_suw_reg2_reg3(void);
    /**
     * \brief Provides access to the "roundf.sw reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_sw_reg2_reg3(void);
    /**
     * \brief Provides access to the "cache cacheop, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cache_cacheop_ind_reg1(void);
    /**
     * \brief Provides access to the "pref prefop, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_pref_prefop_ind_reg1(void);
    /**
     * \brief Provides access to the "dbcp" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_dbcp(void);
    /**
     * \brief Provides access to the "dbpush rh-rt" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_dbpush_rh_dec_rt(void);
    /**
     * \brief Provides access to the "dbret" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_dbret(void);
    /**
     * \brief Provides access to the "dbtag imm10" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_dbtag_imm10(void);
    /**
     * \brief Provides access to the "dbtrap" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_dbtrap(void);
    /**
     * \brief Provides access to the "cvtf.hs reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_hs_reg2_reg3(void);
    /**
     * \brief Provides access to the "cvtf.sh reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_sh_reg2_reg3(void);
    /**
     * \brief Provides access to the "fmaf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fmaf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "fmsf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fmsf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "fnmaf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fnmaf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "fnmsf.s reg1, reg2, reg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fnmsf_dot_s_reg1_reg2_reg3(void);
    /**
     * \brief Provides access to the "cll" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cll(void);
    /**
     * \brief Provides access to the "resbank" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_resbank(void);
    /**
     * \brief Provides access to the "clip.b reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_clip_dot_b_reg1_reg2(void);
    /**
     * \brief Provides access to the "clip.bu reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_clip_dot_bu_reg1_reg2(void);
    /**
     * \brief Provides access to the "clip.h reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_clip_dot_h_reg1_reg2(void);
    /**
     * \brief Provides access to the "clip.hu reg1, reg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_clip_dot_hu_reg1_reg2(void);
    /**
     * \brief Provides access to the "ldv.w imm4, disp16 [reg1], wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldv_dot_w_imm4_disp16_ind_reg1_wreg3(void);
    /**
     * \brief Provides access to the "stv.w imm2, wreg3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stv_dot_w_imm2_wreg3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "ldv.qw disp16 [reg1], wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldv_dot_qw_disp16_ind_reg1_wreg3(void);
    /**
     * \brief Provides access to the "stv.qw wreg3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stv_dot_qw_wreg3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "ldv.dw imm2, disp16 [reg1], wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldv_dot_dw_imm2_disp16_ind_reg1_wreg3(void);
    /**
     * \brief Provides access to the "stv.dw imm1, wreg3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stv_dot_dw_imm1_wreg3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "ldvz.h4 disp16 [reg1], wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldvz_dot_h4_disp16_ind_reg1_wreg3(void);
    /**
     * \brief Provides access to the "stvz.h4 wreg3, disp16 [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stvz_dot_h4_wreg3_disp16_ind_reg1(void);
    /**
     * \brief Provides access to the "cmovf.w4 wreg4, wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "shflv.w4 imm12, wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_shflv_dot_w4_imm12_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "fmaf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fmaf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "fmsf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fmsf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "fnmaf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fnmaf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "fnmsf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_fnmsf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "cmpf.s4 fcond, wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cmpf_dot_s4_fcond_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "roundf.sw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_sw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "roundf.suw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_roundf_dot_suw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "trncf.sw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_sw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "trncf.suw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trncf_dot_suw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "ceilf.sw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_sw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "ceilf.suw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ceilf_dot_suw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "floorf.sw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_sw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "floorf.suw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_floorf_dot_suw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "cvtf.sw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_sw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "cvtf.suw4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_suw4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "cvtf.ws4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_ws4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "cvtf.uws4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_uws4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "cvtf.hs4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_hs4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "cvtf.sh4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_cvtf_dot_sh4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "absf.s4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_absf_dot_s4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "negf.s4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_negf_dot_s4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "sqrtf.s4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_sqrtf_dot_s4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "recipf.s4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_recipf_dot_s4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "rsqrtf.s4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_rsqrtf_dot_s4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "flpv.s4 imm2, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_flpv_dot_s4_imm2_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "movv.w4 wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_movv_dot_w4_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "trfsrv.w4 imm3, wreg2" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_trfsrv_dot_w4_imm3_wreg2(void);
    /**
     * \brief Provides access to the "addf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "subf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "mulf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "maxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_maxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "minf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_minf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "divf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_divf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "addrf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addrf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "subrf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subrf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "mulrf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulrf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "maxrf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_maxrf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "minrf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_minrf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "addxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "subxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "mulxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_mulxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "addsubf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addsubf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "subaddf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subaddf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "addsubxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addsubxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "subaddxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subaddxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "addsubnf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addsubnf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "subaddnf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subaddnf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "addsubnxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_addsubnxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "subaddnxf.s4 wreg1, wreg2, wreg3" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_subaddnxf_dot_s4_wreg1_wreg2_wreg3(void);
    /**
     * \brief Provides access to the "hvtrap vector" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_hvtrap_vector(void);
    /**
     * \brief Provides access to the "stm.gsr [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stm_dot_gsr_ind_reg1(void);
    /**
     * \brief Provides access to the "stm.mp eh-et, [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_stm_dot_mp_eh_dec_et_ind_reg1(void);
    /**
     * \brief Provides access to the "ldm.gsr [reg1]" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldm_dot_gsr_ind_reg1(void);
    /**
     * \brief Provides access to the "ldm.mp [reg1], eh-et" instruction
     * \return A pointer to the instruction
     */
    rh850g4::instruction_base_if_t *get_ldm_dot_mp_ind_reg1_eh_dec_et(void);
    
  private:
    /**
     * \brief The "add {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_add_reg1_reg2;
    /**
     * \brief The "add {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_add_imm5_reg2;
    /**
     * \brief The "addi {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addi_imm16_reg1_reg2;
    /**
     * \brief The "adf {cccc}, {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_adf_cccc_reg1_reg2_reg3;
    /**
     * \brief The "and {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_and_reg1_reg2;
    /**
     * \brief The "andi {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_andi_imm16_reg1_reg2;
    /**
     * \brief The "b{cccc} {disp9}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_b_cccc_disp9;
    /**
     * \brief The "bsh {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_bsh_reg2_reg3;
    /**
     * \brief The "bsw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_bsw_reg2_reg3;
    /**
     * \brief The "callt {imm6}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_callt_imm6;
    /**
     * \brief The "caxi [{reg1}], {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_caxi_ind_reg1_reg2_reg3;
    /**
     * \brief The "clr1 {bit}#3, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_clr1_bit_3_disp16_ind_reg1;
    /**
     * \brief The "clr1 {reg2}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_clr1_reg2_ind_reg1;
    /**
     * \brief The "cmov {cccc}, {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmov_cccc_reg1_reg2_reg3;
    /**
     * \brief The "cmov {cccc}, {imm5}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmov_cccc_imm5_reg2_reg3;
    /**
     * \brief The "cmp {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmp_reg1_reg2;
    /**
     * \brief The "cmp {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmp_imm5_reg2;
    /**
     * \brief The "ctret" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ctret;
    /**
     * \brief The "di" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_di;
    /**
     * \brief The "dispose {imm5}, {list12}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_dispose_imm5_list12;
    /**
     * \brief The "dispose {imm5}, {list12}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_dispose_imm5_list12_ind_reg1;
    /**
     * \brief The "div {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_div_reg1_reg2_reg3;
    /**
     * \brief The "switch {reg1}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_switch_reg1;
    /**
     * \brief The "divh {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divh_reg1_reg2;
    /**
     * \brief The "divh {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divh_reg1_reg2_reg3;
    /**
     * \brief The "divhu {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divhu_reg1_reg2_reg3;
    /**
     * \brief The "divq {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divq_reg1_reg2_reg3;
    /**
     * \brief The "divqu {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divqu_reg1_reg2_reg3;
    /**
     * \brief The "divu {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divu_reg1_reg2_reg3;
    /**
     * \brief The "ei" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ei;
    /**
     * \brief The "eiret" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_eiret;
    /**
     * \brief The "feret" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_feret;
    /**
     * \brief The "fetrap {vector}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fetrap_vector;
    /**
     * \brief The "halt" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_halt;
    /**
     * \brief The "hsh {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_hsh_reg2_reg3;
    /**
     * \brief The "hsw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_hsw_reg2_reg3;
    /**
     * \brief The "jr {disp22}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_jr_disp22;
    /**
     * \brief The "jr {disp32}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_jr_disp32;
    /**
     * \brief The "jarl {disp22}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_jarl_disp22_reg2;
    /**
     * \brief The "jarl {disp32}, {reg1}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_jarl_disp32_reg1;
    /**
     * \brief The "jmp [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_jmp_ind_reg1;
    /**
     * \brief The "jmp {disp32}[{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_jmp_disp32_ind_reg1;
    /**
     * \brief The "prepare {list12}, {imm5}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_prepare_list12_imm5;
    /**
     * \brief The "prepare {list12}, {imm5}, {bits_ff}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_prepare_list12_imm5_bits_ff;
    /**
     * \brief The "ld.b {disp16} [{reg1}], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_b_disp16_ind_reg1_reg2;
    /**
     * \brief The "ld.b {disp23} [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_b_disp23_ind_reg1_reg3;
    /**
     * \brief The "ld.b [{reg1}]+, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_b_ind_reg1_inc_reg3;
    /**
     * \brief The "ld.b [{reg1}]-, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_b_ind_reg1_dec_reg3;
    /**
     * \brief The "ld.bu {disp23} [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_bu_disp23_ind_reg1_reg3;
    /**
     * \brief The "ld.bu {disp16} [{reg1}], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_bu_disp16_ind_reg1_reg2;
    /**
     * \brief The "ld.bu [{reg1}]+, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_bu_ind_reg1_inc_reg3;
    /**
     * \brief The "ld.bu [{reg1}]-, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_bu_ind_reg1_dec_reg3;
    /**
     * \brief The "ld.h {disp16} [{reg1}], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_h_disp16_ind_reg1_reg2;
    /**
     * \brief The "ld.h {disp23} [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_h_disp23_ind_reg1_reg3;
    /**
     * \brief The "ld.h [{reg1}]+, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_h_ind_reg1_inc_reg3;
    /**
     * \brief The "ld.h [{reg1}]-, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_h_ind_reg1_dec_reg3;
    /**
     * \brief The "ld.hu {disp16} [{reg1}], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_hu_disp16_ind_reg1_reg2;
    /**
     * \brief The "ld.hu {disp23} [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_hu_disp23_ind_reg1_reg3;
    /**
     * \brief The "ld.hu [{reg1}]+, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_hu_ind_reg1_inc_reg3;
    /**
     * \brief The "ld.hu [{reg1}]-, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_hu_ind_reg1_dec_reg3;
    /**
     * \brief The "ld.w {disp16} [{reg1}], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_w_disp16_ind_reg1_reg2;
    /**
     * \brief The "ld.w {disp23} [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_w_disp23_ind_reg1_reg3;
    /**
     * \brief The "ld.w [{reg1}]+, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_w_ind_reg1_inc_reg3;
    /**
     * \brief The "ld.w [{reg1}]-, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_w_ind_reg1_dec_reg3;
    /**
     * \brief The "ldsr {reg2}, {regID}, {selID}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldsr_reg2_regid_selid;
    /**
     * \brief The "mac {reg1}, {reg2}, {reg3}, {reg4}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mac_reg1_reg2_reg3_reg4;
    /**
     * \brief The "macu {reg1}, {reg2}, {reg3}, {reg4}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_macu_reg1_reg2_reg3_reg4;
    /**
     * \brief The "nop" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_nop;
    /**
     * \brief The "mov {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mov_reg1_reg2;
    /**
     * \brief The "mov {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mov_imm5_reg2;
    /**
     * \brief The "mov {imm32}, {reg1}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mov_imm32_reg1;
    /**
     * \brief The "movea {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_movea_imm16_reg1_reg2;
    /**
     * \brief The "movhi {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_movhi_imm16_reg1_reg2;
    /**
     * \brief The "mul {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mul_reg1_reg2_reg3;
    /**
     * \brief The "sxh {reg1}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sxh_reg1;
    /**
     * \brief The "mul {imm9}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mul_imm9_reg2_reg3;
    /**
     * \brief The "mulh {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulh_reg1_reg2;
    /**
     * \brief The "mulh {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulh_imm5_reg2;
    /**
     * \brief The "mulhi {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulhi_imm16_reg1_reg2;
    /**
     * \brief The "mulu {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulu_reg1_reg2_reg3;
    /**
     * \brief The "mulu {imm9}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulu_imm9_reg2_reg3;
    /**
     * \brief The "not {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_not_reg1_reg2;
    /**
     * \brief The "not1 {bit}#3, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_not1_bit_3_disp16_ind_reg1;
    /**
     * \brief The "not1 {reg2}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_not1_reg2_ind_reg1;
    /**
     * \brief The "or {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_or_reg1_reg2;
    /**
     * \brief The "ori {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ori_imm16_reg1_reg2;
    /**
     * \brief The "rie" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_rie;
    /**
     * \brief The "rie {imm5}, {imm4}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_rie_imm5_imm4;
    /**
     * \brief The "sar {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sar_reg1_reg2;
    /**
     * \brief The "sar {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sar_imm5_reg2;
    /**
     * \brief The "sar {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sar_reg1_reg2_reg3;
    /**
     * \brief The "sasf {cccc}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sasf_cccc_reg2;
    /**
     * \brief The "sxb {reg1}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sxb_reg1;
    /**
     * \brief The "zxb {reg1}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_zxb_reg1;
    /**
     * \brief The "zxh {reg1}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_zxh_reg1;
    /**
     * \brief The "satadd {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_satadd_reg1_reg2;
    /**
     * \brief The "satadd {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_satadd_imm5_reg2;
    /**
     * \brief The "satadd {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_satadd_reg1_reg2_reg3;
    /**
     * \brief The "satsub {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_satsub_reg1_reg2;
    /**
     * \brief The "satsub {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_satsub_reg1_reg2_reg3;
    /**
     * \brief The "satsubi {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_satsubi_imm16_reg1_reg2;
    /**
     * \brief The "satsubr {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_satsubr_reg1_reg2;
    /**
     * \brief The "sbf {cccc}, {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sbf_cccc_reg1_reg2_reg3;
    /**
     * \brief The "sch0l {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sch0l_reg2_reg3;
    /**
     * \brief The "sch0r {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sch0r_reg2_reg3;
    /**
     * \brief The "sch1l {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sch1l_reg2_reg3;
    /**
     * \brief The "sch1r {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sch1r_reg2_reg3;
    /**
     * \brief The "set1 {bit}#3, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_set1_bit_3_disp16_ind_reg1;
    /**
     * \brief The "set1 {reg2}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_set1_reg2_ind_reg1;
    /**
     * \brief The "setf {cccc}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_setf_cccc_reg2;
    /**
     * \brief The "shl {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_shl_reg1_reg2;
    /**
     * \brief The "shl {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_shl_imm5_reg2;
    /**
     * \brief The "shl {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_shl_reg1_reg2_reg3;
    /**
     * \brief The "shr {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_shr_reg1_reg2;
    /**
     * \brief The "shr {imm5}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_shr_imm5_reg2;
    /**
     * \brief The "shr {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_shr_reg1_reg2_reg3;
    /**
     * \brief The "sld.b {disp7} [ep], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sld_dot_b_disp7_ind_ep_reg2;
    /**
     * \brief The "sld.bu {disp4} [ep], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sld_dot_bu_disp4_ind_ep_reg2;
    /**
     * \brief The "sld.h {disp8} [ep], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sld_dot_h_disp8_ind_ep_reg2;
    /**
     * \brief The "sld.hu {disp5} [ep], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sld_dot_hu_disp5_ind_ep_reg2;
    /**
     * \brief The "sld.w {disp8} [ep], {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sld_dot_w_disp8_ind_ep_reg2;
    /**
     * \brief The "sst.b {reg2}, {disp7} [ep]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sst_dot_b_reg2_disp7_ind_ep;
    /**
     * \brief The "sst.h {reg2}, {disp8} [ep]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sst_dot_h_reg2_disp8_ind_ep;
    /**
     * \brief The "sst.w {reg2}, {disp8} [ep]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sst_dot_w_reg2_disp8_ind_ep;
    /**
     * \brief The "st.b {reg2}, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_b_reg2_disp16_ind_reg1;
    /**
     * \brief The "st.b {reg3}, {disp23} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_b_reg3_disp23_ind_reg1;
    /**
     * \brief The "st.b {reg3}, [{reg1}]+" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_b_reg3_ind_reg1_inc;
    /**
     * \brief The "st.b {reg3}, [{reg1}]-" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_b_reg3_ind_reg1_dec;
    /**
     * \brief The "st.h {reg2}, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_h_reg2_disp16_ind_reg1;
    /**
     * \brief The "st.h {reg3}, {disp23} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_h_reg3_disp23_ind_reg1;
    /**
     * \brief The "st.h {reg3}, [{reg1}]+" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_h_reg3_ind_reg1_inc;
    /**
     * \brief The "st.h {reg3}, [{reg1}]-" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_h_reg3_ind_reg1_dec;
    /**
     * \brief The "st.w {reg2}, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_w_reg2_disp16_ind_reg1;
    /**
     * \brief The "st.w {reg3}, {disp23} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_w_reg3_disp23_ind_reg1;
    /**
     * \brief The "st.w {reg3}, [{reg1}]+" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_w_reg3_ind_reg1_inc;
    /**
     * \brief The "st.w {reg3}, [{reg1}]-" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_w_reg3_ind_reg1_dec;
    /**
     * \brief The "stsr {regID}, {reg2}, {selID}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stsr_regid_reg2_selid;
    /**
     * \brief The "sub {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sub_reg1_reg2;
    /**
     * \brief The "subr {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subr_reg1_reg2;
    /**
     * \brief The "synce" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_synce;
    /**
     * \brief The "syncm" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_syncm;
    /**
     * \brief The "syncp" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_syncp;
    /**
     * \brief The "syscall {vector}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_syscall_vector;
    /**
     * \brief The "trap {vector}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trap_vector;
    /**
     * \brief The "tst {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_tst_reg1_reg2;
    /**
     * \brief The "tst1 {bit}#3, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_tst1_bit_3_disp16_ind_reg1;
    /**
     * \brief The "tst1 {reg2}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_tst1_reg2_ind_reg1;
    /**
     * \brief The "xor {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_xor_reg1_reg2;
    /**
     * \brief The "xori {imm16}, {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_xori_imm16_reg1_reg2;
    /**
     * \brief The "absf.d {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_absf_dot_d_reg2_reg3;
    /**
     * \brief The "absf.s {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_absf_dot_s_reg2_reg3;
    /**
     * \brief The "addf.d {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addf_dot_d_reg1_reg2_reg3;
    /**
     * \brief The "addf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "ceilf.dl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_dl_reg2_reg3;
    /**
     * \brief The "ceilf.dul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_dul_reg2_reg3;
    /**
     * \brief The "ceilf.duw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_duw_reg2_reg3;
    /**
     * \brief The "ceilf.dw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_dw_reg2_reg3;
    /**
     * \brief The "ceilf.sl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_sl_reg2_reg3;
    /**
     * \brief The "ceilf.sul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_sul_reg2_reg3;
    /**
     * \brief The "ceilf.suw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_suw_reg2_reg3;
    /**
     * \brief The "ceilf.sw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_sw_reg2_reg3;
    /**
     * \brief The "cmovf.d {fcbit}, {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmovf_dot_d_fcbit_reg1_reg2_reg3;
    /**
     * \brief The "cmovf.s {fcbit}, {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmovf_dot_s_fcbit_reg1_reg2_reg3;
    /**
     * \brief The "cmpf.d {fcond}, {reg2}, {reg1}, {fcbit}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmpf_dot_d_fcond_reg2_reg1_fcbit;
    /**
     * \brief The "cmpf.s {fcond}, {reg2}, {reg1}, {fcbit}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmpf_dot_s_fcond_reg2_reg1_fcbit;
    /**
     * \brief The "cvtf.dl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_dl_reg2_reg3;
    /**
     * \brief The "cvtf.ds {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_ds_reg2_reg3;
    /**
     * \brief The "cvtf.dul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_dul_reg2_reg3;
    /**
     * \brief The "cvtf.duw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_duw_reg2_reg3;
    /**
     * \brief The "cvtf.dw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_dw_reg2_reg3;
    /**
     * \brief The "cvtf.ld {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_ld_reg2_reg3;
    /**
     * \brief The "cvtf.ls {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_ls_reg2_reg3;
    /**
     * \brief The "cvtf.sd {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_sd_reg2_reg3;
    /**
     * \brief The "cvtf.sl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_sl_reg2_reg3;
    /**
     * \brief The "cvtf.sul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_sul_reg2_reg3;
    /**
     * \brief The "cvtf.suw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_suw_reg2_reg3;
    /**
     * \brief The "cvtf.sw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_sw_reg2_reg3;
    /**
     * \brief The "cvtf.uld {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_uld_reg2_reg3;
    /**
     * \brief The "cvtf.uls {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_uls_reg2_reg3;
    /**
     * \brief The "cvtf.uwd {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_uwd_reg2_reg3;
    /**
     * \brief The "cvtf.uws {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_uws_reg2_reg3;
    /**
     * \brief The "cvtf.wd {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_wd_reg2_reg3;
    /**
     * \brief The "cvtf.ws {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_ws_reg2_reg3;
    /**
     * \brief The "divf.d {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divf_dot_d_reg1_reg2_reg3;
    /**
     * \brief The "divf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "floorf.dl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_dl_reg2_reg3;
    /**
     * \brief The "floorf.dul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_dul_reg2_reg3;
    /**
     * \brief The "floorf.duw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_duw_reg2_reg3;
    /**
     * \brief The "floorf.dw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_dw_reg2_reg3;
    /**
     * \brief The "floorf.sl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_sl_reg2_reg3;
    /**
     * \brief The "floorf.sul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_sul_reg2_reg3;
    /**
     * \brief The "floorf.suw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_suw_reg2_reg3;
    /**
     * \brief The "floorf.sw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_sw_reg2_reg3;
    /**
     * \brief The "maddf.s {reg1}, {reg2}, {reg3}, {reg4}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_maddf_dot_s_reg1_reg2_reg3_reg4;
    /**
     * \brief The "maxf.d {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_maxf_dot_d_reg1_reg2_reg3;
    /**
     * \brief The "maxf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_maxf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "minf.d {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_minf_dot_d_reg1_reg2_reg3;
    /**
     * \brief The "minf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_minf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "msubf.s {reg1}, {reg2}, {reg3}, {reg4}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_msubf_dot_s_reg1_reg2_reg3_reg4;
    /**
     * \brief The "mulf.d {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulf_dot_d_reg1_reg2_reg3;
    /**
     * \brief The "mulf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "negf.d {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_negf_dot_d_reg2_reg3;
    /**
     * \brief The "negf.s {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_negf_dot_s_reg2_reg3;
    /**
     * \brief The "nmaddf.s {reg1}, {reg2}, {reg3}, {reg4}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_nmaddf_dot_s_reg1_reg2_reg3_reg4;
    /**
     * \brief The "nmsubf.s {reg1}, {reg2}, {reg3}, {reg4}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_nmsubf_dot_s_reg1_reg2_reg3_reg4;
    /**
     * \brief The "recipf.d {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_recipf_dot_d_reg2_reg3;
    /**
     * \brief The "recipf.s {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_recipf_dot_s_reg2_reg3;
    /**
     * \brief The "rsqrtf.d {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_rsqrtf_dot_d_reg2_reg3;
    /**
     * \brief The "rsqrtf.s {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_rsqrtf_dot_s_reg2_reg3;
    /**
     * \brief The "sqrtf.d {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sqrtf_dot_d_reg2_reg3;
    /**
     * \brief The "sqrtf.s {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sqrtf_dot_s_reg2_reg3;
    /**
     * \brief The "subf.d {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subf_dot_d_reg1_reg2_reg3;
    /**
     * \brief The "subf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "trfsr {fcbit}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trfsr_fcbit;
    /**
     * \brief The "trncf.dl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_dl_reg2_reg3;
    /**
     * \brief The "trncf.dul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_dul_reg2_reg3;
    /**
     * \brief The "trncf.duw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_duw_reg2_reg3;
    /**
     * \brief The "trncf.dw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_dw_reg2_reg3;
    /**
     * \brief The "trncf.sl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_sl_reg2_reg3;
    /**
     * \brief The "trncf.sul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_sul_reg2_reg3;
    /**
     * \brief The "trncf.suw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_suw_reg2_reg3;
    /**
     * \brief The "trncf.sw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_sw_reg2_reg3;
    /**
     * \brief The "b{cccc} {disp17}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_b_cccc_disp17;
    /**
     * \brief The "binsu {reg1}, {lsb}, {msb}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_binsu_reg1_lsb_msb_reg2;
    /**
     * \brief The "binsm {reg1}, {lsb}, {msb}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_binsm_reg1_lsb_msb_reg2;
    /**
     * \brief The "binsl {reg1}, {lsb}, {msb}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_binsl_reg1_lsb_msb_reg2;
    /**
     * \brief The "jarl [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_jarl_ind_reg1_reg3;
    /**
     * \brief The "ld.dw {disp23} [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ld_dot_dw_disp23_ind_reg1_reg3;
    /**
     * \brief The "ldl.bu [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldl_dot_bu_ind_reg1_reg3;
    /**
     * \brief The "ldl.hu [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldl_dot_hu_ind_reg1_reg3;
    /**
     * \brief The "ldl.w [{reg1}], {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldl_dot_w_ind_reg1_reg3;
    /**
     * \brief The "loop {reg1}, {disp16}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_loop_reg1_disp16;
    /**
     * \brief The "popsp {rh}-{rt}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_popsp_rh_dec_rt;
    /**
     * \brief The "pushsp {rh}-{rt}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_pushsp_rh_dec_rt;
    /**
     * \brief The "rotl {imm5}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_rotl_imm5_reg2_reg3;
    /**
     * \brief The "rotl {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_rotl_reg1_reg2_reg3;
    /**
     * \brief The "snooze" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_snooze;
    /**
     * \brief The "st.dw {reg3}, {disp23} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_st_dot_dw_reg3_disp23_ind_reg1;
    /**
     * \brief The "stc.b {reg3}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stc_dot_b_reg3_ind_reg1;
    /**
     * \brief The "stc.h {reg3}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stc_dot_h_reg3_ind_reg1;
    /**
     * \brief The "stc.w {reg3}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stc_dot_w_reg3_ind_reg1;
    /**
     * \brief The "synci" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_synci;
    /**
     * \brief The "roundf.dl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_dl_reg2_reg3;
    /**
     * \brief The "roundf.dul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_dul_reg2_reg3;
    /**
     * \brief The "roundf.duw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_duw_reg2_reg3;
    /**
     * \brief The "roundf.dw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_dw_reg2_reg3;
    /**
     * \brief The "roundf.sl {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_sl_reg2_reg3;
    /**
     * \brief The "roundf.sul {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_sul_reg2_reg3;
    /**
     * \brief The "roundf.suw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_suw_reg2_reg3;
    /**
     * \brief The "roundf.sw {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_sw_reg2_reg3;
    /**
     * \brief The "cache {cacheop}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cache_cacheop_ind_reg1;
    /**
     * \brief The "pref {prefop}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_pref_prefop_ind_reg1;
    /**
     * \brief The "dbcp" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_dbcp;
    /**
     * \brief The "dbpush {rh}-{rt}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_dbpush_rh_dec_rt;
    /**
     * \brief The "dbret" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_dbret;
    /**
     * \brief The "dbtag {imm10}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_dbtag_imm10;
    /**
     * \brief The "dbtrap" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_dbtrap;
    /**
     * \brief The "cvtf.hs {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_hs_reg2_reg3;
    /**
     * \brief The "cvtf.sh {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_sh_reg2_reg3;
    /**
     * \brief The "fmaf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fmaf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "fmsf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fmsf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "fnmaf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fnmaf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "fnmsf.s {reg1}, {reg2}, {reg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fnmsf_dot_s_reg1_reg2_reg3;
    /**
     * \brief The "cll" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cll;
    /**
     * \brief The "resbank" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_resbank;
    /**
     * \brief The "clip.b {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_clip_dot_b_reg1_reg2;
    /**
     * \brief The "clip.bu {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_clip_dot_bu_reg1_reg2;
    /**
     * \brief The "clip.h {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_clip_dot_h_reg1_reg2;
    /**
     * \brief The "clip.hu {reg1}, {reg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_clip_dot_hu_reg1_reg2;
    /**
     * \brief The "ldv.w {imm4}, {disp16} [{reg1}], {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldv_dot_w_imm4_disp16_ind_reg1_wreg3;
    /**
     * \brief The "stv.w {imm2}, {wreg3}, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stv_dot_w_imm2_wreg3_disp16_ind_reg1;
    /**
     * \brief The "ldv.qw {disp16} [{reg1}], {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldv_dot_qw_disp16_ind_reg1_wreg3;
    /**
     * \brief The "stv.qw {wreg3}, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stv_dot_qw_wreg3_disp16_ind_reg1;
    /**
     * \brief The "ldv.dw {imm2}, {disp16} [{reg1}], {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldv_dot_dw_imm2_disp16_ind_reg1_wreg3;
    /**
     * \brief The "stv.dw {imm1}, {wreg3}, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stv_dot_dw_imm1_wreg3_disp16_ind_reg1;
    /**
     * \brief The "ldvz.h4 {disp16} [{reg1}], {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldvz_dot_h4_disp16_ind_reg1_wreg3;
    /**
     * \brief The "stvz.h4 {wreg3}, {disp16} [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stvz_dot_h4_wreg3_disp16_ind_reg1;
    /**
     * \brief The "cmovf.w4 {wreg4}, {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmovf_dot_w4_wreg4_wreg1_wreg2_wreg3;
    /**
     * \brief The "shflv.w4 {imm12}, {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_shflv_dot_w4_imm12_wreg1_wreg2_wreg3;
    /**
     * \brief The "fmaf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fmaf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "fmsf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fmsf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "fnmaf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fnmaf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "fnmsf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_fnmsf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "cmpf.s4 {fcond}, {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cmpf_dot_s4_fcond_wreg1_wreg2_wreg3;
    /**
     * \brief The "roundf.sw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_sw4_wreg2_wreg3;
    /**
     * \brief The "roundf.suw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_roundf_dot_suw4_wreg2_wreg3;
    /**
     * \brief The "trncf.sw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_sw4_wreg2_wreg3;
    /**
     * \brief The "trncf.suw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trncf_dot_suw4_wreg2_wreg3;
    /**
     * \brief The "ceilf.sw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_sw4_wreg2_wreg3;
    /**
     * \brief The "ceilf.suw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ceilf_dot_suw4_wreg2_wreg3;
    /**
     * \brief The "floorf.sw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_sw4_wreg2_wreg3;
    /**
     * \brief The "floorf.suw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_floorf_dot_suw4_wreg2_wreg3;
    /**
     * \brief The "cvtf.sw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_sw4_wreg2_wreg3;
    /**
     * \brief The "cvtf.suw4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_suw4_wreg2_wreg3;
    /**
     * \brief The "cvtf.ws4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_ws4_wreg2_wreg3;
    /**
     * \brief The "cvtf.uws4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_uws4_wreg2_wreg3;
    /**
     * \brief The "cvtf.hs4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_hs4_wreg2_wreg3;
    /**
     * \brief The "cvtf.sh4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_cvtf_dot_sh4_wreg2_wreg3;
    /**
     * \brief The "absf.s4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_absf_dot_s4_wreg2_wreg3;
    /**
     * \brief The "negf.s4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_negf_dot_s4_wreg2_wreg3;
    /**
     * \brief The "sqrtf.s4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_sqrtf_dot_s4_wreg2_wreg3;
    /**
     * \brief The "recipf.s4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_recipf_dot_s4_wreg2_wreg3;
    /**
     * \brief The "rsqrtf.s4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_rsqrtf_dot_s4_wreg2_wreg3;
    /**
     * \brief The "flpv.s4 {imm2}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_flpv_dot_s4_imm2_wreg2_wreg3;
    /**
     * \brief The "movv.w4 {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_movv_dot_w4_wreg2_wreg3;
    /**
     * \brief The "trfsrv.w4 {imm3}, {wreg2}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_trfsrv_dot_w4_imm3_wreg2;
    /**
     * \brief The "addf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "subf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "mulf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "maxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_maxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "minf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_minf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "divf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_divf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "addrf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addrf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "subrf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subrf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "mulrf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulrf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "maxrf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_maxrf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "minrf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_minrf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "addxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "subxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "mulxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_mulxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "addsubf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addsubf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "subaddf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subaddf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "addsubxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addsubxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "subaddxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subaddxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "addsubnf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addsubnf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "subaddnf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subaddnf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "addsubnxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_addsubnxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "subaddnxf.s4 {wreg1}, {wreg2}, {wreg3}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_subaddnxf_dot_s4_wreg1_wreg2_wreg3;
    /**
     * \brief The "hvtrap {vector}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_hvtrap_vector;
    /**
     * \brief The "stm.gsr [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stm_dot_gsr_ind_reg1;
    /**
     * \brief The "stm.mp {eh}-{et}, [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_stm_dot_mp_eh_dec_et_ind_reg1;
    /**
     * \brief The "ldm.gsr [{reg1}]" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldm_dot_gsr_ind_reg1;
    /**
     * \brief The "ldm.mp [{reg1}], {eh}-{et}" instruction instance
     */
    rh850g4::instruction_base_impl_t *m_ldm_dot_mp_ind_reg1_eh_dec_et;
  };
}

#endif // _RH850G4_INSTR_SET_IMPL_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
