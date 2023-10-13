/*************************************************************************
 *
 * Copyright(c) 2015-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/custom_bridge.h"

//------------------------------------------------------------------------------
// rh850g4 custom extension bridge implementation
//------------------------------------------------------------------------------

rh850g4::custom_bridge_t::custom_bridge_t(custom_if_t *custom_impl) :
  m_custom_impl(custom_impl)
{
}

rh850g4::custom_bridge_t::~custom_bridge_t()
{
}

void rh850g4::custom_bridge_t::create_exit_block(void)
{
  m_custom_impl->create_exit_block();
}

fastiss::block_t *rh850g4::custom_bridge_t::get_exit_block(void)
{
  return m_custom_impl->get_exit_block();
}

void rh850g4::custom_bridge_t::gen_increment_cycle_count(fastiss::i32_t *increment)
{
  m_custom_impl->gen_increment_cycle_count(increment);
}

void rh850g4::custom_bridge_t::notify_interrupt_accepted(void)
{
  m_custom_impl->notify_interrupt_accepted();
}

void rh850g4::custom_bridge_t::notify_interrupt_complete(void)
{
  m_custom_impl->notify_interrupt_complete();
}

bool rh850g4::custom_bridge_t::is_condition_always(unsigned char condition)
{
  return m_custom_impl->is_condition_always(condition);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_calculate_PSW_S(fastiss::i32_t *op_result)
{
  return m_custom_impl->gen_calculate_PSW_S(op_result);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_calculate_PSW_Z(fastiss::i32_t *op_result)
{
  return m_custom_impl->gen_calculate_PSW_Z(op_result);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_calculate_PSW_CY_shift(fastiss::i32_t *val, fastiss::i32_t *shift, bool is_left_shift)
{
  return m_custom_impl->gen_calculate_PSW_CY_shift(val, shift, is_left_shift);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_condition_satisfied(unsigned char condition)
{
  return m_custom_impl->gen_condition_satisfied(condition);
}

void rh850g4::custom_bridge_t::gen_div_reg1_reg2(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2)
{
  m_custom_impl->gen_div_reg1_reg2(reg1_val, reg2);
}

void rh850g4::custom_bridge_t::gen_div_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2, rh850g4::GR_index_t reg3)
{
  m_custom_impl->gen_div_reg1_reg2_reg3(reg1_val, reg2, reg3);
}

void rh850g4::custom_bridge_t::gen_divu_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2, rh850g4::GR_index_t reg3, bool is_halfword)
{
  m_custom_impl->gen_divu_reg1_reg2_reg3(reg1_val, reg2, reg3, is_halfword);
}

fastiss::i1_t *rh850g4::custom_bridge_t::is_unimplemented_exp(fastiss::f32_t *value)
{
  return m_custom_impl->is_unimplemented_exp(value);
}

fastiss::i1_t *rh850g4::custom_bridge_t::is_unimplemented_exp(fastiss::f64_t *value)
{
  return m_custom_impl->is_unimplemented_exp(value);
}

fastiss::i1_t *rh850g4::custom_bridge_t::is_unimplemented_fxu_exp(fastiss::f32_t *value)
{
  return m_custom_impl->is_unimplemented_fxu_exp(value);
}

fastiss::f32_t *rh850g4::custom_bridge_t::gen_flush_denorm(fastiss::f32_t *value)
{
  return m_custom_impl->gen_flush_denorm(value);
}

fastiss::f64_t *rh850g4::custom_bridge_t::gen_flush_denorm(fastiss::f64_t *value)
{
  return m_custom_impl->gen_flush_denorm(value);
}

fastiss::f32_t *rh850g4::custom_bridge_t::gen_flush_denorm_fxu(fastiss::f32_t *value)
{
  return m_custom_impl->gen_flush_denorm_fxu(value);
}

void rh850g4::custom_bridge_t::gen_underflow_denorm_update(fastiss::f16_with_exception_flags_t& op_result)
{
  m_custom_impl->gen_underflow_denorm_update(op_result);
}

void rh850g4::custom_bridge_t::gen_underflow_denorm_update(fastiss::f32_with_exception_flags_t& op_result)
{
  m_custom_impl->gen_underflow_denorm_update(op_result);
}

void rh850g4::custom_bridge_t::gen_underflow_denorm_update(fastiss::f64_with_exception_flags_t& op_result)
{
  m_custom_impl->gen_underflow_denorm_update(op_result);
}

void rh850g4::custom_bridge_t::gen_underflow_denorm_update(fastiss::f80_with_exception_flags_t& op_result)
{
  m_custom_impl->gen_underflow_denorm_update(op_result);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg2)
{
  return m_custom_impl->gen_inexact_denorm_update(invalid, divide_by_zero, inexact, reg2);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg2_pair)
{
  return m_custom_impl->gen_inexact_denorm_update(invalid, divide_by_zero, inexact, reg2_pair);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2)
{
  return m_custom_impl->gen_inexact_denorm_update(invalid, divide_by_zero, inexact, reg1, reg2);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg1_pair, fastiss::f64_t *reg2_pair)
{
  return m_custom_impl->gen_inexact_denorm_update(invalid, divide_by_zero, inexact, reg1_pair, reg2_pair);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2, fastiss::f32_t *reg3)
{
  return m_custom_impl->gen_inexact_denorm_update(invalid, divide_by_zero, inexact, reg1, reg2, reg3);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_inexact_enabled_overflow_underflow_update(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact)
{
  return m_custom_impl->gen_inexact_enabled_overflow_underflow_update(overflow, underflow, inexact);
}

fastiss::i1_t *rh850g4::custom_bridge_t::gen_inexact_enabled_overflow_underflow_update_fxu(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact)
{
  return m_custom_impl->gen_inexact_enabled_overflow_underflow_update_fxu(overflow, underflow, inexact);
}

void rh850g4::custom_bridge_t::gen_update_dest_and_fpsr(bool is_single_precision, rh850g4::GR_index_t dest_index, fastiss::i32_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block)
{
  m_custom_impl->gen_update_dest_and_fpsr(is_single_precision, dest_index, dest_val, exception_flags, continue_block);
}

void rh850g4::custom_bridge_t::gen_update_dest_and_fpsr(bool is_single_precision, rh850g4::GR_index_t dest_index, fastiss::i64_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block)
{
  m_custom_impl->gen_update_dest_and_fpsr(is_single_precision, dest_index, dest_val, exception_flags, continue_block);
}

void rh850g4::custom_bridge_t::gen_update_dest_and_fxsr(bool is_single_precision, rh850g4::WR_index_t dest_index, fastiss::i64_t* dest_lo_val, fastiss::i64_t* dest_up_val, fastiss::i8_t* w0_exp_flag, fastiss::i8_t* w1_exp_flag, fastiss::i8_t* w2_exp_flag, fastiss::i8_t* w3_exp_flag, fastiss::block_t *continue_block)
{
  m_custom_impl->gen_update_dest_and_fxsr(is_single_precision, dest_index, dest_lo_val, dest_up_val, w0_exp_flag, w1_exp_flag, w2_exp_flag, w3_exp_flag, continue_block);
}

fastiss::i8_t *rh850g4::custom_bridge_t::gen_read_round_mode(void)
{
  return m_custom_impl->gen_read_round_mode();
}

void rh850g4::custom_bridge_t::gen_clear_fpi_flag(void)
{
  return m_custom_impl->gen_clear_fpi_flag();
}

void rh850g4::custom_bridge_t::gen_bins_common(rh850g4::GR_index_t reg1, rh850g4::GR_index_t reg2, fastiss::i32_t *msb_val, fastiss::i32_t *lsb_val)
{
  return m_custom_impl->gen_bins_common(reg1, reg2, msb_val, lsb_val);
}

fastiss::i32_t *rh850g4::custom_bridge_t::gen_bit_extract(fastiss::i32_t *x_val, fastiss::i32_t *l_val, fastiss::i32_t *r_val)
{
  return m_custom_impl->gen_bit_extract(x_val, l_val, r_val);
}

fastiss::i32_t *rh850g4::custom_bridge_t::gen_vector_shuffle(fastiss::i32_t *pos, fastiss::i32_t *imm12_val, rh850g4::WR_index_t wreg1, rh850g4::WR_index_t wreg2)
{
  return m_custom_impl->gen_vector_shuffle(pos, imm12_val, wreg1, wreg2);
}

void rh850g4::custom_bridge_t::gen_fp_simd_add(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i1_t *is_neg)
{
  m_custom_impl->gen_fp_simd_add(input_val_1, input_val_2, res_val, res_exp, is_neg);
}

void rh850g4::custom_bridge_t::gen_fp_simd_sub(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i1_t *is_neg)
{
  m_custom_impl->gen_fp_simd_sub(input_val_1, input_val_2, res_val, res_exp, is_neg);
}

void rh850g4::custom_bridge_t::gen_fp_simd_mul(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_mul(input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_div(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_div(input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_max(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_max(input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_min(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_min(input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_cvt_hs(fastiss::f16_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  m_custom_impl->gen_fp_simd_cvt_hs(input_val_1, res_val, res_exp, round_mode);
}

void rh850g4::custom_bridge_t::gen_fp_simd_cvt_sh(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  m_custom_impl->gen_fp_simd_cvt_sh(input_val_1, res_val, res_exp, round_mode);
}

void rh850g4::custom_bridge_t::gen_fp_simd_cvt_suw(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  m_custom_impl->gen_fp_simd_cvt_suw(input_val_1, res_val, res_exp, round_mode);
}

void rh850g4::custom_bridge_t::gen_fp_simd_cvt_sw(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  m_custom_impl->gen_fp_simd_cvt_sw(input_val_1, res_val, res_exp, round_mode);
}

void rh850g4::custom_bridge_t::gen_fp_simd_recip(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_recip(input_val_1, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_rsqrt(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_rsqrt(input_val_1, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_sqrt(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_sqrt(input_val_1, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_cmp(fastiss::i8_t *fcond, fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_cmp(fcond, input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_fma(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_fma(input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_fms(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_fms(input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_fnma(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_fnma(input_val_1, input_val_2, res_val, res_exp);
}

void rh850g4::custom_bridge_t::gen_fp_simd_fnms(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  m_custom_impl->gen_fp_simd_fnms(input_val_1, input_val_2, res_val, res_exp);
}

fastiss::i1_t *rh850g4::custom_bridge_t::is_host_mode(void)
{
  return m_custom_impl->is_host_mode();
}

fastiss::i1_t *rh850g4::custom_bridge_t::is_supervizer_mode(void)
{
  return m_custom_impl->is_supervizer_mode();
}

fastiss::i1_t *rh850g4::custom_bridge_t::is_valid_virtualization(void)
{
  return m_custom_impl->is_valid_virtualization();
}

void rh850g4::custom_bridge_t::gen_pie_rie_exept(fastiss::i32_t* curr_pc, fastiss::i32_t* base_addr, fastiss::i32_t* exept_code, fastiss::i1_t* current_pswh_gm)
{
  m_custom_impl->gen_pie_rie_exept(curr_pc, base_addr, exept_code, current_pswh_gm);
}

fastiss::i8_ptr_t *rh850g4::custom_bridge_t::gen_read_link_manager_ptr(void)
{
  return m_custom_impl->gen_read_link_manager_ptr();
}

bool rh850g4::custom_bridge_t::is_link_func_enable(void)
{
  return m_custom_impl->is_link_func_enable();
}

bool rh850g4::custom_bridge_t::is_hv_func_support(void)
{
  return m_custom_impl->is_hv_func_support();
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
