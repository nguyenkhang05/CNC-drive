//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/instr_set_impl.h"
#include "rh850/add_reg1_reg2_instr.h"
#include "rh850/add_imm5_reg2_instr.h"
#include "rh850/addi_imm16_reg1_reg2_instr.h"
#include "rh850/adf_cccc_reg1_reg2_reg3_instr.h"
#include "rh850/and_reg1_reg2_instr.h"
#include "rh850/andi_imm16_reg1_reg2_instr.h"
#include "rh850/b_cccc_disp9_instr.h"
#include "rh850/bsh_reg2_reg3_instr.h"
#include "rh850/bsw_reg2_reg3_instr.h"
#include "rh850/callt_imm6_instr.h"
#include "rh850/caxi_ind_reg1_reg2_reg3_instr.h"
#include "rh850/clr1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850/clr1_reg2_ind_reg1_instr.h"
#include "rh850/cmov_cccc_reg1_reg2_reg3_instr.h"
#include "rh850/cmov_cccc_imm5_reg2_reg3_instr.h"
#include "rh850/cmp_reg1_reg2_instr.h"
#include "rh850/cmp_imm5_reg2_instr.h"
#include "rh850/ctret_instr.h"
#include "rh850/di_instr.h"
#include "rh850/dispose_imm5_list12_instr.h"
#include "rh850/dispose_imm5_list12_ind_reg1_instr.h"
#include "rh850/div_reg1_reg2_reg3_instr.h"
#include "rh850/switch_reg1_instr.h"
#include "rh850/divh_reg1_reg2_instr.h"
#include "rh850/divh_reg1_reg2_reg3_instr.h"
#include "rh850/divhu_reg1_reg2_reg3_instr.h"
#include "rh850/divq_reg1_reg2_reg3_instr.h"
#include "rh850/divqu_reg1_reg2_reg3_instr.h"
#include "rh850/divu_reg1_reg2_reg3_instr.h"
#include "rh850/ei_instr.h"
#include "rh850/eiret_instr.h"
#include "rh850/feret_instr.h"
#include "rh850/fetrap_vector_instr.h"
#include "rh850/halt_instr.h"
#include "rh850/hsh_reg2_reg3_instr.h"
#include "rh850/hsw_reg2_reg3_instr.h"
#include "rh850/jr_disp22_instr.h"
#include "rh850/jr_disp32_instr.h"
#include "rh850/jarl_disp22_reg2_instr.h"
#include "rh850/jarl_disp32_reg1_instr.h"
#include "rh850/jmp_ind_reg1_instr.h"
#include "rh850/jmp_disp32_ind_reg1_instr.h"
#include "rh850/prepare_list12_imm5_instr.h"
#include "rh850/prepare_list12_imm5_bits_ff_instr.h"
#include "rh850/ld_dot_b_disp16_ind_reg1_reg2_instr.h"
#include "rh850/ld_dot_b_disp23_ind_reg1_reg3_instr.h"
#include "rh850/ld_dot_bu_disp23_ind_reg1_reg3_instr.h"
#include "rh850/ld_dot_bu_disp16_ind_reg1_reg2_instr.h"
#include "rh850/ld_dot_h_disp16_ind_reg1_reg2_instr.h"
#include "rh850/ld_dot_h_disp23_ind_reg1_reg3_instr.h"
#include "rh850/ld_dot_hu_disp16_ind_reg1_reg2_instr.h"
#include "rh850/ld_dot_hu_disp23_ind_reg1_reg3_instr.h"
#include "rh850/ld_dot_w_disp16_ind_reg1_reg2_instr.h"
#include "rh850/ld_dot_w_disp23_ind_reg1_reg3_instr.h"
#include "rh850/ldsr_reg2_regid_selid_instr.h"
#include "rh850/mac_reg1_reg2_reg3_reg4_instr.h"
#include "rh850/macu_reg1_reg2_reg3_reg4_instr.h"
#include "rh850/nop_instr.h"
#include "rh850/mov_reg1_reg2_instr.h"
#include "rh850/mov_imm5_reg2_instr.h"
#include "rh850/mov_imm32_reg1_instr.h"
#include "rh850/movea_imm16_reg1_reg2_instr.h"
#include "rh850/movhi_imm16_reg1_reg2_instr.h"
#include "rh850/mul_reg1_reg2_reg3_instr.h"
#include "rh850/sxh_reg1_instr.h"
#include "rh850/mul_imm9_reg2_reg3_instr.h"
#include "rh850/mulh_reg1_reg2_instr.h"
#include "rh850/mulh_imm5_reg2_instr.h"
#include "rh850/mulhi_imm16_reg1_reg2_instr.h"
#include "rh850/mulu_reg1_reg2_reg3_instr.h"
#include "rh850/mulu_imm9_reg2_reg3_instr.h"
#include "rh850/not_reg1_reg2_instr.h"
#include "rh850/not1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850/not1_reg2_ind_reg1_instr.h"
#include "rh850/or_reg1_reg2_instr.h"
#include "rh850/ori_imm16_reg1_reg2_instr.h"
#include "rh850/rie_instr.h"
#include "rh850/rie_imm5_imm4_instr.h"
#include "rh850/sar_reg1_reg2_instr.h"
#include "rh850/sar_imm5_reg2_instr.h"
#include "rh850/sar_reg1_reg2_reg3_instr.h"
#include "rh850/sasf_cccc_reg2_instr.h"
#include "rh850/sxb_reg1_instr.h"
#include "rh850/zxb_reg1_instr.h"
#include "rh850/zxh_reg1_instr.h"
#include "rh850/satadd_reg1_reg2_instr.h"
#include "rh850/satadd_imm5_reg2_instr.h"
#include "rh850/satadd_reg1_reg2_reg3_instr.h"
#include "rh850/satsub_reg1_reg2_instr.h"
#include "rh850/satsub_reg1_reg2_reg3_instr.h"
#include "rh850/satsubi_imm16_reg1_reg2_instr.h"
#include "rh850/satsubr_reg1_reg2_instr.h"
#include "rh850/sbf_cccc_reg1_reg2_reg3_instr.h"
#include "rh850/sch0l_reg2_reg3_instr.h"
#include "rh850/sch0r_reg2_reg3_instr.h"
#include "rh850/sch1l_reg2_reg3_instr.h"
#include "rh850/sch1r_reg2_reg3_instr.h"
#include "rh850/set1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850/set1_reg2_ind_reg1_instr.h"
#include "rh850/setf_cccc_reg2_instr.h"
#include "rh850/shl_reg1_reg2_instr.h"
#include "rh850/shl_imm5_reg2_instr.h"
#include "rh850/shl_reg1_reg2_reg3_instr.h"
#include "rh850/shr_reg1_reg2_instr.h"
#include "rh850/shr_imm5_reg2_instr.h"
#include "rh850/shr_reg1_reg2_reg3_instr.h"
#include "rh850/sld_dot_b_disp7_ind_ep_reg2_instr.h"
#include "rh850/sld_dot_bu_disp4_ind_ep_reg2_instr.h"
#include "rh850/sld_dot_h_disp8_ind_ep_reg2_instr.h"
#include "rh850/sld_dot_hu_disp5_ind_ep_reg2_instr.h"
#include "rh850/sld_dot_w_disp8_ind_ep_reg2_instr.h"
#include "rh850/sst_dot_b_reg2_disp7_ind_ep_instr.h"
#include "rh850/sst_dot_h_reg2_disp8_ind_ep_instr.h"
#include "rh850/sst_dot_w_reg2_disp8_ind_ep_instr.h"
#include "rh850/st_dot_b_reg2_disp16_ind_reg1_instr.h"
#include "rh850/st_dot_b_reg3_disp23_ind_reg1_instr.h"
#include "rh850/st_dot_h_reg2_disp16_ind_reg1_instr.h"
#include "rh850/st_dot_h_reg3_disp23_ind_reg1_instr.h"
#include "rh850/st_dot_w_reg2_disp16_ind_reg1_instr.h"
#include "rh850/st_dot_w_reg3_disp23_ind_reg1_instr.h"
#include "rh850/stsr_regid_reg2_selid_instr.h"
#include "rh850/sub_reg1_reg2_instr.h"
#include "rh850/subr_reg1_reg2_instr.h"
#include "rh850/synce_instr.h"
#include "rh850/syncm_instr.h"
#include "rh850/syncp_instr.h"
#include "rh850/syscall_vector_instr.h"
#include "rh850/trap_vector_instr.h"
#include "rh850/tst_reg1_reg2_instr.h"
#include "rh850/tst1_bit_3_disp16_ind_reg1_instr.h"
#include "rh850/tst1_reg2_ind_reg1_instr.h"
#include "rh850/xor_reg1_reg2_instr.h"
#include "rh850/xori_imm16_reg1_reg2_instr.h"
#include "rh850/cmpf_dot_d_fcond_reg2_reg1_fcbit_instr.h"
#include "rh850/maddf_dot_s_reg1_reg2_reg3_reg4_instr.h"
#include "rh850/maxf_dot_d_reg1_reg2_reg3_instr.h"
#include "rh850/b_cccc_disp17_instr.h"
#include "rh850/binsu_reg1_lsb_msb_reg2_instr.h"
#include "rh850/binsm_reg1_lsb_msb_reg2_instr.h"
#include "rh850/binsl_reg1_lsb_msb_reg2_instr.h"
#include "rh850/jarl_ind_reg1_reg3_instr.h"
#include "rh850/ld_dot_dw_disp23_ind_reg1_reg3_instr.h"
#include "rh850/ldl_dot_w_ind_reg1_reg3_instr.h"
#include "rh850/loop_reg1_disp16_instr.h"
#include "rh850/popsp_rh_dec_rt_instr.h"
#include "rh850/pushsp_rh_dec_rt_instr.h"
#include "rh850/rotl_imm5_reg2_reg3_instr.h"
#include "rh850/rotl_reg1_reg2_reg3_instr.h"
#include "rh850/snooze_instr.h"
#include "rh850/st_dot_dw_reg3_disp23_ind_reg1_instr.h"
#include "rh850/stc_dot_w_reg3_ind_reg1_instr.h"
#include "rh850/synci_instr.h"
#include "rh850/hvcall_vector_instr.h"
#include "rh850/hvtrap_vector_instr.h"
#include "rh850/ldvc_dot_sr_reg2_regid_selid_instr.h"
#include "rh850/stvc_dot_sr_regid_reg2_selid_instr.h"
#include "rh850/dst_instr.h"
#include "rh850/est_instr.h"
#include "rh850/ldtc_dot_gr_reg2_reg1_instr.h"
#include "rh850/ldtc_dot_vr_reg2_vreg1_instr.h"
#include "rh850/ldtc_dot_pc_reg2_instr.h"
#include "rh850/ldtc_dot_sr_reg2_regid_selid_instr.h"
#include "rh850/sttc_dot_gr_reg1_reg2_instr.h"
#include "rh850/sttc_dot_vr_vreg1_reg2_instr.h"
#include "rh850/sttc_dot_pc_reg2_instr.h"
#include "rh850/sttc_dot_sr_regid_reg2_selid_instr.h"
#include "rh850/cache_cacheop_ind_reg1_instr.h"
#include "rh850/pref_prefop_ind_reg1_instr.h"
#include "rh850/dbcp_instr.h"
#include "rh850/dbhvtrap_instr.h"
#include "rh850/dbpush_rh_dec_rt_instr.h"
#include "rh850/dbret_instr.h"
#include "rh850/dbtag_imm10_instr.h"
#include "rh850/dbtrap_instr.h"
#include "rh850/rmtrap_instr.h"
#include "rh850/cll_instr.h"

//------------------------------------------------------------------------------
// rh850 instructions definition
//------------------------------------------------------------------------------

rh850::instr_set_impl_t::instr_set_impl_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850::memory_if_t *memory_impl,
  rh850::operand_if_t *operand_impl,
  rh850::cpu_state_if_t *cpu_state_impl,
  rh850::custom_if_t *custom_impl
)
{
  m_add_reg1_reg2 = new rh850::add_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_add_imm5_reg2 = new rh850::add_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_addi_imm16_reg1_reg2 = new rh850::addi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_adf_cccc_reg1_reg2_reg3 = new rh850::adf_cccc_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_and_reg1_reg2 = new rh850::and_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_andi_imm16_reg1_reg2 = new rh850::andi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_b_cccc_disp9 = new rh850::b_cccc_disp9_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_bsh_reg2_reg3 = new rh850::bsh_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_bsw_reg2_reg3 = new rh850::bsw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_callt_imm6 = new rh850::callt_imm6_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_caxi_ind_reg1_reg2_reg3 = new rh850::caxi_ind_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clr1_bit_3_disp16_ind_reg1 = new rh850::clr1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_clr1_reg2_ind_reg1 = new rh850::clr1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmov_cccc_reg1_reg2_reg3 = new rh850::cmov_cccc_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmov_cccc_imm5_reg2_reg3 = new rh850::cmov_cccc_imm5_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmp_reg1_reg2 = new rh850::cmp_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmp_imm5_reg2 = new rh850::cmp_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ctret = new rh850::ctret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_di = new rh850::di_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dispose_imm5_list12 = new rh850::dispose_imm5_list12_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dispose_imm5_list12_ind_reg1 = new rh850::dispose_imm5_list12_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_div_reg1_reg2_reg3 = new rh850::div_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_switch_reg1 = new rh850::switch_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divh_reg1_reg2 = new rh850::divh_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divh_reg1_reg2_reg3 = new rh850::divh_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divhu_reg1_reg2_reg3 = new rh850::divhu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divq_reg1_reg2_reg3 = new rh850::divq_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divqu_reg1_reg2_reg3 = new rh850::divqu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_divu_reg1_reg2_reg3 = new rh850::divu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ei = new rh850::ei_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_eiret = new rh850::eiret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_feret = new rh850::feret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_fetrap_vector = new rh850::fetrap_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_halt = new rh850::halt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_hsh_reg2_reg3 = new rh850::hsh_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_hsw_reg2_reg3 = new rh850::hsw_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jr_disp22 = new rh850::jr_disp22_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jr_disp32 = new rh850::jr_disp32_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jarl_disp22_reg2 = new rh850::jarl_disp22_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jarl_disp32_reg1 = new rh850::jarl_disp32_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jmp_ind_reg1 = new rh850::jmp_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jmp_disp32_ind_reg1 = new rh850::jmp_disp32_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_prepare_list12_imm5 = new rh850::prepare_list12_imm5_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_prepare_list12_imm5_bits_ff = new rh850::prepare_list12_imm5_bits_ff_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_b_disp16_ind_reg1_reg2 = new rh850::ld_dot_b_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_b_disp23_ind_reg1_reg3 = new rh850::ld_dot_b_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_bu_disp23_ind_reg1_reg3 = new rh850::ld_dot_bu_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_bu_disp16_ind_reg1_reg2 = new rh850::ld_dot_bu_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_h_disp16_ind_reg1_reg2 = new rh850::ld_dot_h_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_h_disp23_ind_reg1_reg3 = new rh850::ld_dot_h_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_hu_disp16_ind_reg1_reg2 = new rh850::ld_dot_hu_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_hu_disp23_ind_reg1_reg3 = new rh850::ld_dot_hu_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_w_disp16_ind_reg1_reg2 = new rh850::ld_dot_w_disp16_ind_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_w_disp23_ind_reg1_reg3 = new rh850::ld_dot_w_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldsr_reg2_regid_selid = new rh850::ldsr_reg2_regid_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mac_reg1_reg2_reg3_reg4 = new rh850::mac_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_macu_reg1_reg2_reg3_reg4 = new rh850::macu_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_nop = new rh850::nop_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mov_reg1_reg2 = new rh850::mov_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mov_imm5_reg2 = new rh850::mov_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mov_imm32_reg1 = new rh850::mov_imm32_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_movea_imm16_reg1_reg2 = new rh850::movea_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_movhi_imm16_reg1_reg2 = new rh850::movhi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mul_reg1_reg2_reg3 = new rh850::mul_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sxh_reg1 = new rh850::sxh_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mul_imm9_reg2_reg3 = new rh850::mul_imm9_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulh_reg1_reg2 = new rh850::mulh_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulh_imm5_reg2 = new rh850::mulh_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulhi_imm16_reg1_reg2 = new rh850::mulhi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulu_reg1_reg2_reg3 = new rh850::mulu_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_mulu_imm9_reg2_reg3 = new rh850::mulu_imm9_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_not_reg1_reg2 = new rh850::not_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_not1_bit_3_disp16_ind_reg1 = new rh850::not1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_not1_reg2_ind_reg1 = new rh850::not1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_or_reg1_reg2 = new rh850::or_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ori_imm16_reg1_reg2 = new rh850::ori_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rie = new rh850::rie_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rie_imm5_imm4 = new rh850::rie_imm5_imm4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sar_reg1_reg2 = new rh850::sar_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sar_imm5_reg2 = new rh850::sar_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sar_reg1_reg2_reg3 = new rh850::sar_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sasf_cccc_reg2 = new rh850::sasf_cccc_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sxb_reg1 = new rh850::sxb_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_zxb_reg1 = new rh850::zxb_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_zxh_reg1 = new rh850::zxh_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satadd_reg1_reg2 = new rh850::satadd_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satadd_imm5_reg2 = new rh850::satadd_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satadd_reg1_reg2_reg3 = new rh850::satadd_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsub_reg1_reg2 = new rh850::satsub_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsub_reg1_reg2_reg3 = new rh850::satsub_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsubi_imm16_reg1_reg2 = new rh850::satsubi_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_satsubr_reg1_reg2 = new rh850::satsubr_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sbf_cccc_reg1_reg2_reg3 = new rh850::sbf_cccc_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch0l_reg2_reg3 = new rh850::sch0l_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch0r_reg2_reg3 = new rh850::sch0r_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch1l_reg2_reg3 = new rh850::sch1l_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sch1r_reg2_reg3 = new rh850::sch1r_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_set1_bit_3_disp16_ind_reg1 = new rh850::set1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_set1_reg2_ind_reg1 = new rh850::set1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_setf_cccc_reg2 = new rh850::setf_cccc_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shl_reg1_reg2 = new rh850::shl_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shl_imm5_reg2 = new rh850::shl_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shl_reg1_reg2_reg3 = new rh850::shl_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shr_reg1_reg2 = new rh850::shr_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shr_imm5_reg2 = new rh850::shr_imm5_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_shr_reg1_reg2_reg3 = new rh850::shr_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_b_disp7_ind_ep_reg2 = new rh850::sld_dot_b_disp7_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_bu_disp4_ind_ep_reg2 = new rh850::sld_dot_bu_disp4_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_h_disp8_ind_ep_reg2 = new rh850::sld_dot_h_disp8_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_hu_disp5_ind_ep_reg2 = new rh850::sld_dot_hu_disp5_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sld_dot_w_disp8_ind_ep_reg2 = new rh850::sld_dot_w_disp8_ind_ep_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sst_dot_b_reg2_disp7_ind_ep = new rh850::sst_dot_b_reg2_disp7_ind_ep_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sst_dot_h_reg2_disp8_ind_ep = new rh850::sst_dot_h_reg2_disp8_ind_ep_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sst_dot_w_reg2_disp8_ind_ep = new rh850::sst_dot_w_reg2_disp8_ind_ep_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_b_reg2_disp16_ind_reg1 = new rh850::st_dot_b_reg2_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_b_reg3_disp23_ind_reg1 = new rh850::st_dot_b_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_h_reg2_disp16_ind_reg1 = new rh850::st_dot_h_reg2_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_h_reg3_disp23_ind_reg1 = new rh850::st_dot_h_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_w_reg2_disp16_ind_reg1 = new rh850::st_dot_w_reg2_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_w_reg3_disp23_ind_reg1 = new rh850::st_dot_w_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stsr_regid_reg2_selid = new rh850::stsr_regid_reg2_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sub_reg1_reg2 = new rh850::sub_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_subr_reg1_reg2 = new rh850::subr_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_synce = new rh850::synce_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_syncm = new rh850::syncm_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_syncp = new rh850::syncp_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_syscall_vector = new rh850::syscall_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_trap_vector = new rh850::trap_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_tst_reg1_reg2 = new rh850::tst_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_tst1_bit_3_disp16_ind_reg1 = new rh850::tst1_bit_3_disp16_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_tst1_reg2_ind_reg1 = new rh850::tst1_reg2_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_xor_reg1_reg2 = new rh850::xor_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_xori_imm16_reg1_reg2 = new rh850::xori_imm16_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cmpf_dot_d_fcond_reg2_reg1_fcbit = new rh850::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_maddf_dot_s_reg1_reg2_reg3_reg4 = new rh850::maddf_dot_s_reg1_reg2_reg3_reg4_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_maxf_dot_d_reg1_reg2_reg3 = new rh850::maxf_dot_d_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_b_cccc_disp17 = new rh850::b_cccc_disp17_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_binsu_reg1_lsb_msb_reg2 = new rh850::binsu_reg1_lsb_msb_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_binsm_reg1_lsb_msb_reg2 = new rh850::binsm_reg1_lsb_msb_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_binsl_reg1_lsb_msb_reg2 = new rh850::binsl_reg1_lsb_msb_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_jarl_ind_reg1_reg3 = new rh850::jarl_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ld_dot_dw_disp23_ind_reg1_reg3 = new rh850::ld_dot_dw_disp23_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldl_dot_w_ind_reg1_reg3 = new rh850::ldl_dot_w_ind_reg1_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_loop_reg1_disp16 = new rh850::loop_reg1_disp16_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_popsp_rh_dec_rt = new rh850::popsp_rh_dec_rt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_pushsp_rh_dec_rt = new rh850::pushsp_rh_dec_rt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rotl_imm5_reg2_reg3 = new rh850::rotl_imm5_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rotl_reg1_reg2_reg3 = new rh850::rotl_reg1_reg2_reg3_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_snooze = new rh850::snooze_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_st_dot_dw_reg3_disp23_ind_reg1 = new rh850::st_dot_dw_reg3_disp23_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stc_dot_w_reg3_ind_reg1 = new rh850::stc_dot_w_reg3_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_synci = new rh850::synci_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_hvcall_vector = new rh850::hvcall_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_hvtrap_vector = new rh850::hvtrap_vector_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldvc_dot_sr_reg2_regid_selid = new rh850::ldvc_dot_sr_reg2_regid_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_stvc_dot_sr_regid_reg2_selid = new rh850::stvc_dot_sr_regid_reg2_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dst = new rh850::dst_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_est = new rh850::est_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldtc_dot_gr_reg2_reg1 = new rh850::ldtc_dot_gr_reg2_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldtc_dot_vr_reg2_vreg1 = new rh850::ldtc_dot_vr_reg2_vreg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldtc_dot_pc_reg2 = new rh850::ldtc_dot_pc_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_ldtc_dot_sr_reg2_regid_selid = new rh850::ldtc_dot_sr_reg2_regid_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sttc_dot_gr_reg1_reg2 = new rh850::sttc_dot_gr_reg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sttc_dot_vr_vreg1_reg2 = new rh850::sttc_dot_vr_vreg1_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sttc_dot_pc_reg2 = new rh850::sttc_dot_pc_reg2_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_sttc_dot_sr_regid_reg2_selid = new rh850::sttc_dot_sr_regid_reg2_selid_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cache_cacheop_ind_reg1 = new rh850::cache_cacheop_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_pref_prefop_ind_reg1 = new rh850::pref_prefop_ind_reg1_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbcp = new rh850::dbcp_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbhvtrap = new rh850::dbhvtrap_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbpush_rh_dec_rt = new rh850::dbpush_rh_dec_rt_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbret = new rh850::dbret_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbtag_imm10 = new rh850::dbtag_imm10_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_dbtrap = new rh850::dbtrap_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_rmtrap = new rh850::rmtrap_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
  m_cll = new rh850::cll_instr_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl);
}

rh850::instr_set_impl_t::~instr_set_impl_t()
{
  delete m_add_reg1_reg2;
  delete m_add_imm5_reg2;
  delete m_addi_imm16_reg1_reg2;
  delete m_adf_cccc_reg1_reg2_reg3;
  delete m_and_reg1_reg2;
  delete m_andi_imm16_reg1_reg2;
  delete m_b_cccc_disp9;
  delete m_bsh_reg2_reg3;
  delete m_bsw_reg2_reg3;
  delete m_callt_imm6;
  delete m_caxi_ind_reg1_reg2_reg3;
  delete m_clr1_bit_3_disp16_ind_reg1;
  delete m_clr1_reg2_ind_reg1;
  delete m_cmov_cccc_reg1_reg2_reg3;
  delete m_cmov_cccc_imm5_reg2_reg3;
  delete m_cmp_reg1_reg2;
  delete m_cmp_imm5_reg2;
  delete m_ctret;
  delete m_di;
  delete m_dispose_imm5_list12;
  delete m_dispose_imm5_list12_ind_reg1;
  delete m_div_reg1_reg2_reg3;
  delete m_switch_reg1;
  delete m_divh_reg1_reg2;
  delete m_divh_reg1_reg2_reg3;
  delete m_divhu_reg1_reg2_reg3;
  delete m_divq_reg1_reg2_reg3;
  delete m_divqu_reg1_reg2_reg3;
  delete m_divu_reg1_reg2_reg3;
  delete m_ei;
  delete m_eiret;
  delete m_feret;
  delete m_fetrap_vector;
  delete m_halt;
  delete m_hsh_reg2_reg3;
  delete m_hsw_reg2_reg3;
  delete m_jr_disp22;
  delete m_jr_disp32;
  delete m_jarl_disp22_reg2;
  delete m_jarl_disp32_reg1;
  delete m_jmp_ind_reg1;
  delete m_jmp_disp32_ind_reg1;
  delete m_prepare_list12_imm5;
  delete m_prepare_list12_imm5_bits_ff;
  delete m_ld_dot_b_disp16_ind_reg1_reg2;
  delete m_ld_dot_b_disp23_ind_reg1_reg3;
  delete m_ld_dot_bu_disp23_ind_reg1_reg3;
  delete m_ld_dot_bu_disp16_ind_reg1_reg2;
  delete m_ld_dot_h_disp16_ind_reg1_reg2;
  delete m_ld_dot_h_disp23_ind_reg1_reg3;
  delete m_ld_dot_hu_disp16_ind_reg1_reg2;
  delete m_ld_dot_hu_disp23_ind_reg1_reg3;
  delete m_ld_dot_w_disp16_ind_reg1_reg2;
  delete m_ld_dot_w_disp23_ind_reg1_reg3;
  delete m_ldsr_reg2_regid_selid;
  delete m_mac_reg1_reg2_reg3_reg4;
  delete m_macu_reg1_reg2_reg3_reg4;
  delete m_nop;
  delete m_mov_reg1_reg2;
  delete m_mov_imm5_reg2;
  delete m_mov_imm32_reg1;
  delete m_movea_imm16_reg1_reg2;
  delete m_movhi_imm16_reg1_reg2;
  delete m_mul_reg1_reg2_reg3;
  delete m_sxh_reg1;
  delete m_mul_imm9_reg2_reg3;
  delete m_mulh_reg1_reg2;
  delete m_mulh_imm5_reg2;
  delete m_mulhi_imm16_reg1_reg2;
  delete m_mulu_reg1_reg2_reg3;
  delete m_mulu_imm9_reg2_reg3;
  delete m_not_reg1_reg2;
  delete m_not1_bit_3_disp16_ind_reg1;
  delete m_not1_reg2_ind_reg1;
  delete m_or_reg1_reg2;
  delete m_ori_imm16_reg1_reg2;
  delete m_rie;
  delete m_rie_imm5_imm4;
  delete m_sar_reg1_reg2;
  delete m_sar_imm5_reg2;
  delete m_sar_reg1_reg2_reg3;
  delete m_sasf_cccc_reg2;
  delete m_sxb_reg1;
  delete m_zxb_reg1;
  delete m_zxh_reg1;
  delete m_satadd_reg1_reg2;
  delete m_satadd_imm5_reg2;
  delete m_satadd_reg1_reg2_reg3;
  delete m_satsub_reg1_reg2;
  delete m_satsub_reg1_reg2_reg3;
  delete m_satsubi_imm16_reg1_reg2;
  delete m_satsubr_reg1_reg2;
  delete m_sbf_cccc_reg1_reg2_reg3;
  delete m_sch0l_reg2_reg3;
  delete m_sch0r_reg2_reg3;
  delete m_sch1l_reg2_reg3;
  delete m_sch1r_reg2_reg3;
  delete m_set1_bit_3_disp16_ind_reg1;
  delete m_set1_reg2_ind_reg1;
  delete m_setf_cccc_reg2;
  delete m_shl_reg1_reg2;
  delete m_shl_imm5_reg2;
  delete m_shl_reg1_reg2_reg3;
  delete m_shr_reg1_reg2;
  delete m_shr_imm5_reg2;
  delete m_shr_reg1_reg2_reg3;
  delete m_sld_dot_b_disp7_ind_ep_reg2;
  delete m_sld_dot_bu_disp4_ind_ep_reg2;
  delete m_sld_dot_h_disp8_ind_ep_reg2;
  delete m_sld_dot_hu_disp5_ind_ep_reg2;
  delete m_sld_dot_w_disp8_ind_ep_reg2;
  delete m_sst_dot_b_reg2_disp7_ind_ep;
  delete m_sst_dot_h_reg2_disp8_ind_ep;
  delete m_sst_dot_w_reg2_disp8_ind_ep;
  delete m_st_dot_b_reg2_disp16_ind_reg1;
  delete m_st_dot_b_reg3_disp23_ind_reg1;
  delete m_st_dot_h_reg2_disp16_ind_reg1;
  delete m_st_dot_h_reg3_disp23_ind_reg1;
  delete m_st_dot_w_reg2_disp16_ind_reg1;
  delete m_st_dot_w_reg3_disp23_ind_reg1;
  delete m_stsr_regid_reg2_selid;
  delete m_sub_reg1_reg2;
  delete m_subr_reg1_reg2;
  delete m_synce;
  delete m_syncm;
  delete m_syncp;
  delete m_syscall_vector;
  delete m_trap_vector;
  delete m_tst_reg1_reg2;
  delete m_tst1_bit_3_disp16_ind_reg1;
  delete m_tst1_reg2_ind_reg1;
  delete m_xor_reg1_reg2;
  delete m_xori_imm16_reg1_reg2;
  delete m_cmpf_dot_d_fcond_reg2_reg1_fcbit;
  delete m_maddf_dot_s_reg1_reg2_reg3_reg4;
  delete m_maxf_dot_d_reg1_reg2_reg3;
  delete m_b_cccc_disp17;
  delete m_binsu_reg1_lsb_msb_reg2;
  delete m_binsm_reg1_lsb_msb_reg2;
  delete m_binsl_reg1_lsb_msb_reg2;
  delete m_jarl_ind_reg1_reg3;
  delete m_ld_dot_dw_disp23_ind_reg1_reg3;
  delete m_ldl_dot_w_ind_reg1_reg3;
  delete m_loop_reg1_disp16;
  delete m_popsp_rh_dec_rt;
  delete m_pushsp_rh_dec_rt;
  delete m_rotl_imm5_reg2_reg3;
  delete m_rotl_reg1_reg2_reg3;
  delete m_snooze;
  delete m_st_dot_dw_reg3_disp23_ind_reg1;
  delete m_stc_dot_w_reg3_ind_reg1;
  delete m_synci;
  delete m_hvcall_vector;
  delete m_hvtrap_vector;
  delete m_ldvc_dot_sr_reg2_regid_selid;
  delete m_stvc_dot_sr_regid_reg2_selid;
  delete m_dst;
  delete m_est;
  delete m_ldtc_dot_gr_reg2_reg1;
  delete m_ldtc_dot_vr_reg2_vreg1;
  delete m_ldtc_dot_pc_reg2;
  delete m_ldtc_dot_sr_reg2_regid_selid;
  delete m_sttc_dot_gr_reg1_reg2;
  delete m_sttc_dot_vr_vreg1_reg2;
  delete m_sttc_dot_pc_reg2;
  delete m_sttc_dot_sr_regid_reg2_selid;
  delete m_cache_cacheop_ind_reg1;
  delete m_pref_prefop_ind_reg1;
  delete m_dbcp;
  delete m_dbhvtrap;
  delete m_dbpush_rh_dec_rt;
  delete m_dbret;
  delete m_dbtag_imm10;
  delete m_dbtrap;
  delete m_rmtrap;
  delete m_cll;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_add_reg1_reg2()
{
  return m_add_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_add_imm5_reg2()
{
  return m_add_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_addi_imm16_reg1_reg2()
{
  return m_addi_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_adf_cccc_reg1_reg2_reg3()
{
  return m_adf_cccc_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_and_reg1_reg2()
{
  return m_and_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_andi_imm16_reg1_reg2()
{
  return m_andi_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_b_cccc_disp9()
{
  return m_b_cccc_disp9;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_bsh_reg2_reg3()
{
  return m_bsh_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_bsw_reg2_reg3()
{
  return m_bsw_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_callt_imm6()
{
  return m_callt_imm6;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_caxi_ind_reg1_reg2_reg3()
{
  return m_caxi_ind_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_clr1_bit_3_disp16_ind_reg1()
{
  return m_clr1_bit_3_disp16_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_clr1_reg2_ind_reg1()
{
  return m_clr1_reg2_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_cmov_cccc_reg1_reg2_reg3()
{
  return m_cmov_cccc_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_cmov_cccc_imm5_reg2_reg3()
{
  return m_cmov_cccc_imm5_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_cmp_reg1_reg2()
{
  return m_cmp_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_cmp_imm5_reg2()
{
  return m_cmp_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ctret()
{
  return m_ctret;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_di()
{
  return m_di;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dispose_imm5_list12()
{
  return m_dispose_imm5_list12;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dispose_imm5_list12_ind_reg1()
{
  return m_dispose_imm5_list12_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_div_reg1_reg2_reg3()
{
  return m_div_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_switch_reg1()
{
  return m_switch_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_divh_reg1_reg2()
{
  return m_divh_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_divh_reg1_reg2_reg3()
{
  return m_divh_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_divhu_reg1_reg2_reg3()
{
  return m_divhu_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_divq_reg1_reg2_reg3()
{
  return m_divq_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_divqu_reg1_reg2_reg3()
{
  return m_divqu_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_divu_reg1_reg2_reg3()
{
  return m_divu_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ei()
{
  return m_ei;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_eiret()
{
  return m_eiret;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_feret()
{
  return m_feret;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_fetrap_vector()
{
  return m_fetrap_vector;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_halt()
{
  return m_halt;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_hsh_reg2_reg3()
{
  return m_hsh_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_hsw_reg2_reg3()
{
  return m_hsw_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_jr_disp22()
{
  return m_jr_disp22;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_jr_disp32()
{
  return m_jr_disp32;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_jarl_disp22_reg2()
{
  return m_jarl_disp22_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_jarl_disp32_reg1()
{
  return m_jarl_disp32_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_jmp_ind_reg1()
{
  return m_jmp_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_jmp_disp32_ind_reg1()
{
  return m_jmp_disp32_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_prepare_list12_imm5()
{
  return m_prepare_list12_imm5;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_prepare_list12_imm5_bits_ff()
{
  return m_prepare_list12_imm5_bits_ff;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_b_disp16_ind_reg1_reg2()
{
  return m_ld_dot_b_disp16_ind_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_b_disp23_ind_reg1_reg3()
{
  return m_ld_dot_b_disp23_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_bu_disp23_ind_reg1_reg3()
{
  return m_ld_dot_bu_disp23_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_bu_disp16_ind_reg1_reg2()
{
  return m_ld_dot_bu_disp16_ind_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_h_disp16_ind_reg1_reg2()
{
  return m_ld_dot_h_disp16_ind_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_h_disp23_ind_reg1_reg3()
{
  return m_ld_dot_h_disp23_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_hu_disp16_ind_reg1_reg2()
{
  return m_ld_dot_hu_disp16_ind_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_hu_disp23_ind_reg1_reg3()
{
  return m_ld_dot_hu_disp23_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_w_disp16_ind_reg1_reg2()
{
  return m_ld_dot_w_disp16_ind_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_w_disp23_ind_reg1_reg3()
{
  return m_ld_dot_w_disp23_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ldsr_reg2_regid_selid()
{
  return m_ldsr_reg2_regid_selid;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mac_reg1_reg2_reg3_reg4()
{
  return m_mac_reg1_reg2_reg3_reg4;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_macu_reg1_reg2_reg3_reg4()
{
  return m_macu_reg1_reg2_reg3_reg4;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_nop()
{
  return m_nop;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mov_reg1_reg2()
{
  return m_mov_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mov_imm5_reg2()
{
  return m_mov_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mov_imm32_reg1()
{
  return m_mov_imm32_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_movea_imm16_reg1_reg2()
{
  return m_movea_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_movhi_imm16_reg1_reg2()
{
  return m_movhi_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mul_reg1_reg2_reg3()
{
  return m_mul_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sxh_reg1()
{
  return m_sxh_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mul_imm9_reg2_reg3()
{
  return m_mul_imm9_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mulh_reg1_reg2()
{
  return m_mulh_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mulh_imm5_reg2()
{
  return m_mulh_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mulhi_imm16_reg1_reg2()
{
  return m_mulhi_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mulu_reg1_reg2_reg3()
{
  return m_mulu_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_mulu_imm9_reg2_reg3()
{
  return m_mulu_imm9_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_not_reg1_reg2()
{
  return m_not_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_not1_bit_3_disp16_ind_reg1()
{
  return m_not1_bit_3_disp16_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_not1_reg2_ind_reg1()
{
  return m_not1_reg2_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_or_reg1_reg2()
{
  return m_or_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ori_imm16_reg1_reg2()
{
  return m_ori_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_rie()
{
  return m_rie;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_rie_imm5_imm4()
{
  return m_rie_imm5_imm4;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sar_reg1_reg2()
{
  return m_sar_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sar_imm5_reg2()
{
  return m_sar_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sar_reg1_reg2_reg3()
{
  return m_sar_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sasf_cccc_reg2()
{
  return m_sasf_cccc_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sxb_reg1()
{
  return m_sxb_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_zxb_reg1()
{
  return m_zxb_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_zxh_reg1()
{
  return m_zxh_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_satadd_reg1_reg2()
{
  return m_satadd_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_satadd_imm5_reg2()
{
  return m_satadd_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_satadd_reg1_reg2_reg3()
{
  return m_satadd_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_satsub_reg1_reg2()
{
  return m_satsub_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_satsub_reg1_reg2_reg3()
{
  return m_satsub_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_satsubi_imm16_reg1_reg2()
{
  return m_satsubi_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_satsubr_reg1_reg2()
{
  return m_satsubr_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sbf_cccc_reg1_reg2_reg3()
{
  return m_sbf_cccc_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sch0l_reg2_reg3()
{
  return m_sch0l_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sch0r_reg2_reg3()
{
  return m_sch0r_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sch1l_reg2_reg3()
{
  return m_sch1l_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sch1r_reg2_reg3()
{
  return m_sch1r_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_set1_bit_3_disp16_ind_reg1()
{
  return m_set1_bit_3_disp16_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_set1_reg2_ind_reg1()
{
  return m_set1_reg2_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_setf_cccc_reg2()
{
  return m_setf_cccc_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_shl_reg1_reg2()
{
  return m_shl_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_shl_imm5_reg2()
{
  return m_shl_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_shl_reg1_reg2_reg3()
{
  return m_shl_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_shr_reg1_reg2()
{
  return m_shr_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_shr_imm5_reg2()
{
  return m_shr_imm5_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_shr_reg1_reg2_reg3()
{
  return m_shr_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sld_dot_b_disp7_ind_ep_reg2()
{
  return m_sld_dot_b_disp7_ind_ep_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sld_dot_bu_disp4_ind_ep_reg2()
{
  return m_sld_dot_bu_disp4_ind_ep_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sld_dot_h_disp8_ind_ep_reg2()
{
  return m_sld_dot_h_disp8_ind_ep_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sld_dot_hu_disp5_ind_ep_reg2()
{
  return m_sld_dot_hu_disp5_ind_ep_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sld_dot_w_disp8_ind_ep_reg2()
{
  return m_sld_dot_w_disp8_ind_ep_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sst_dot_b_reg2_disp7_ind_ep()
{
  return m_sst_dot_b_reg2_disp7_ind_ep;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sst_dot_h_reg2_disp8_ind_ep()
{
  return m_sst_dot_h_reg2_disp8_ind_ep;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sst_dot_w_reg2_disp8_ind_ep()
{
  return m_sst_dot_w_reg2_disp8_ind_ep;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_st_dot_b_reg2_disp16_ind_reg1()
{
  return m_st_dot_b_reg2_disp16_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_st_dot_b_reg3_disp23_ind_reg1()
{
  return m_st_dot_b_reg3_disp23_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_st_dot_h_reg2_disp16_ind_reg1()
{
  return m_st_dot_h_reg2_disp16_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_st_dot_h_reg3_disp23_ind_reg1()
{
  return m_st_dot_h_reg3_disp23_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_st_dot_w_reg2_disp16_ind_reg1()
{
  return m_st_dot_w_reg2_disp16_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_st_dot_w_reg3_disp23_ind_reg1()
{
  return m_st_dot_w_reg3_disp23_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_stsr_regid_reg2_selid()
{
  return m_stsr_regid_reg2_selid;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sub_reg1_reg2()
{
  return m_sub_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_subr_reg1_reg2()
{
  return m_subr_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_synce()
{
  return m_synce;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_syncm()
{
  return m_syncm;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_syncp()
{
  return m_syncp;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_syscall_vector()
{
  return m_syscall_vector;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_trap_vector()
{
  return m_trap_vector;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_tst_reg1_reg2()
{
  return m_tst_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_tst1_bit_3_disp16_ind_reg1()
{
  return m_tst1_bit_3_disp16_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_tst1_reg2_ind_reg1()
{
  return m_tst1_reg2_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_xor_reg1_reg2()
{
  return m_xor_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_xori_imm16_reg1_reg2()
{
  return m_xori_imm16_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_cmpf_dot_d_fcond_reg2_reg1_fcbit()
{
  return m_cmpf_dot_d_fcond_reg2_reg1_fcbit;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_maddf_dot_s_reg1_reg2_reg3_reg4()
{
  return m_maddf_dot_s_reg1_reg2_reg3_reg4;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_maxf_dot_d_reg1_reg2_reg3()
{
  return m_maxf_dot_d_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_b_cccc_disp17()
{
  return m_b_cccc_disp17;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_binsu_reg1_lsb_msb_reg2()
{
  return m_binsu_reg1_lsb_msb_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_binsm_reg1_lsb_msb_reg2()
{
  return m_binsm_reg1_lsb_msb_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_binsl_reg1_lsb_msb_reg2()
{
  return m_binsl_reg1_lsb_msb_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_jarl_ind_reg1_reg3()
{
  return m_jarl_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ld_dot_dw_disp23_ind_reg1_reg3()
{
  return m_ld_dot_dw_disp23_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ldl_dot_w_ind_reg1_reg3()
{
  return m_ldl_dot_w_ind_reg1_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_loop_reg1_disp16()
{
  return m_loop_reg1_disp16;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_popsp_rh_dec_rt()
{
  return m_popsp_rh_dec_rt;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_pushsp_rh_dec_rt()
{
  return m_pushsp_rh_dec_rt;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_rotl_imm5_reg2_reg3()
{
  return m_rotl_imm5_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_rotl_reg1_reg2_reg3()
{
  return m_rotl_reg1_reg2_reg3;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_snooze()
{
  return m_snooze;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_st_dot_dw_reg3_disp23_ind_reg1()
{
  return m_st_dot_dw_reg3_disp23_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_stc_dot_w_reg3_ind_reg1()
{
  return m_stc_dot_w_reg3_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_synci()
{
  return m_synci;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_hvcall_vector()
{
  return m_hvcall_vector;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_hvtrap_vector()
{
  return m_hvtrap_vector;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ldvc_dot_sr_reg2_regid_selid()
{
  return m_ldvc_dot_sr_reg2_regid_selid;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_stvc_dot_sr_regid_reg2_selid()
{
  return m_stvc_dot_sr_regid_reg2_selid;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dst()
{
  return m_dst;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_est()
{
  return m_est;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ldtc_dot_gr_reg2_reg1()
{
  return m_ldtc_dot_gr_reg2_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ldtc_dot_vr_reg2_vreg1()
{
  return m_ldtc_dot_vr_reg2_vreg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ldtc_dot_pc_reg2()
{
  return m_ldtc_dot_pc_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_ldtc_dot_sr_reg2_regid_selid()
{
  return m_ldtc_dot_sr_reg2_regid_selid;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sttc_dot_gr_reg1_reg2()
{
  return m_sttc_dot_gr_reg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sttc_dot_vr_vreg1_reg2()
{
  return m_sttc_dot_vr_vreg1_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sttc_dot_pc_reg2()
{
  return m_sttc_dot_pc_reg2;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_sttc_dot_sr_regid_reg2_selid()
{
  return m_sttc_dot_sr_regid_reg2_selid;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_cache_cacheop_ind_reg1()
{
  return m_cache_cacheop_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_pref_prefop_ind_reg1()
{
  return m_pref_prefop_ind_reg1;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dbcp()
{
  return m_dbcp;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dbhvtrap()
{
  return m_dbhvtrap;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dbpush_rh_dec_rt()
{
  return m_dbpush_rh_dec_rt;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dbret()
{
  return m_dbret;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dbtag_imm10()
{
  return m_dbtag_imm10;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_dbtrap()
{
  return m_dbtrap;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_rmtrap()
{
  return m_rmtrap;
}

rh850::instruction_base_if_t *rh850::instr_set_impl_t::get_cll()
{
  return m_cll;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
