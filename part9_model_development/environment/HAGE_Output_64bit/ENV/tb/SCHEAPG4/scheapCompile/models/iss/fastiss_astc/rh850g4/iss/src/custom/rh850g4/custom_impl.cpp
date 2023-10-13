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
// Copyright (C) Australian Semiconductor Technology Company. 2017..
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <assert.h>
#include "rh850g4/custom_impl.h"

#include <iostream>

//------------------------------------------------------------------------------
// rh850g4 custom extension implementation
//------------------------------------------------------------------------------

rh850g4::custom_impl_t::custom_impl_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::analysis_point_if* interrupt_accepted_analysis_point,
  rh850g4::analysis_point_if* interrupt_completed_analysis_point,
  void *pending_fpu_imprecise_exception_flag,
  void *link_manager,
  bool link_func_enable,
  bool hv_func_support
) :
  fastiss::code_gen_bridge_t(code_gen_impl),
  rh850g4::memory_bridge_t(memory_impl),
  rh850g4::cpu_state_bridge_t(cpu_state_impl),
  m_exit_block(NULL),
  m_interrupt_accepted_analysis_point(interrupt_accepted_analysis_point),
  m_interrupt_completed_analysis_point(interrupt_completed_analysis_point),
  m_pending_fpu_imprecise_exception_flag(pending_fpu_imprecise_exception_flag),
  m_link_manager(link_manager),
  m_link_func_enable(link_func_enable),
  m_hv_func_support(hv_func_support)
{
}

rh850g4::custom_impl_t::~custom_impl_t()
{
}

void rh850g4::custom_impl_t::create_exit_block(void)
{
  if (m_exit_block != NULL)
  {
    gen_branch(m_exit_block);
    set_gen_point(m_exit_block);
    m_exit_block = NULL;
  }
}

fastiss::block_t *rh850g4::custom_impl_t::get_exit_block(void)
{
  if (m_exit_block == NULL)
  {
    m_exit_block = create_basic_block();
  }
  return m_exit_block;
}

void rh850g4::custom_impl_t::gen_increment_cycle_count(fastiss::i32_t *increment)
{
  gen_increment_time(gen_mul(gen_read_cycle_period(), gen_i64_zext(increment)));
}

void rh850g4::custom_impl_t::notify_interrupt_accepted(void)
{
  if (m_interrupt_accepted_analysis_point->has_attachments())
  {
    call_helper("activate_interrupt_accepted_analysis_point_helper", gen_read_core_ptr(), NULL);  
  }
}

void rh850g4::custom_impl_t::notify_interrupt_complete(void)
{
  if (m_interrupt_completed_analysis_point->has_attachments())
  {
    m_interrupt_completed_analysis_point->activate();
  }

}

bool rh850g4::custom_impl_t::is_condition_always(unsigned char condition)
{
  return (condition == T_COND_CODE);
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_calculate_PSW_S(fastiss::i32_t *op_result)
{ 
  // Determine the S flag value
  fastiss::i1_t *is_result_negative = gen_icmp_slt(op_result, gen_i32_constant(0));
  return is_result_negative;
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_calculate_PSW_Z(fastiss::i32_t *op_result)
{
  // Determine the Z flag value
  fastiss::i1_t *is_result_zero = gen_icmp_eq(op_result, gen_i32_constant(0));
  return is_result_zero;
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_calculate_PSW_CY_shift(fastiss::i32_t *val, fastiss::i32_t *shift, bool is_left_shift)
{
  // Create required constants
  fastiss::i1_t *i1_zero = gen_i1_constant(0);  
  fastiss::i32_t *i32_zero = gen_i32_constant(0);
  fastiss::i32_t *i32_one = gen_i32_constant(1);
  fastiss::i32_t *i32_32 = gen_i32_constant(32);
  
  // Determine the CY flag value
  fastiss::i1_t *is_shift_zero = gen_icmp_eq(shift, i32_zero);
  fastiss::i32_t *shr_amount;
  if (is_left_shift)
  {
    shr_amount = gen_sub(i32_32, shift);
  }
  else
  {
    shr_amount = gen_sub(shift, i32_one);
  }
  fastiss::i32_t *shr_result = gen_lshr(val, shr_amount);
  fastiss::i1_t *shifted_bit = gen_i1_trunc(shr_result);
  fastiss::i1_t *psw_cy_val = gen_select(is_shift_zero, i1_zero, shifted_bit);
  return psw_cy_val;
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_condition_satisfied(unsigned char condition)
{
  fastiss::i1_t *psw_z = gen_read_PSW_Z();
  fastiss::i1_t *psw_s = gen_read_PSW_S();
  fastiss::i1_t *psw_ov = gen_read_PSW_OV();
  fastiss::i1_t *psw_cy = gen_read_PSW_CY();
  fastiss::i1_t *psw_sat = gen_read_PSW_SAT();

  switch (condition) {
    case T_COND_CODE: {  // Unconditional
      return gen_true();
    } 
    case V_COND_CODE: {  // Overflow [OV = 1]
      return gen_icmp_eq(psw_ov, gen_i1_constant(1));
    }
    case C_COND_CODE: {  // Carry [CY = 1]
      return gen_icmp_eq(psw_cy, gen_i1_constant(1));
    }
    case Z_COND_CODE: {  // Zero [Z = 1]
      return gen_icmp_eq(psw_z, gen_i1_constant(1));
    }
    case NH_COND_CODE: { // Not higher (less than or equal) [(CY or Z) = 1]
      fastiss::i1_t *or_result = gen_or(psw_cy, psw_z);
      return gen_icmp_eq(or_result, gen_i1_constant(1));
    }
    case N_COND_CODE: {  // Negative [S = 1]
      return gen_icmp_eq(psw_s, gen_i1_constant(1));
    }
    case LT_COND_CODE: { // Less than signed [(S xor OV) = 1]
      fastiss::i1_t *xor_result = gen_xor(psw_s, psw_ov);
      return gen_icmp_eq(xor_result, gen_i1_constant(1));
    }
    case LE_COND_CODE: { // Less than or equal signed [((S xor OV) or Z) = 1]
      fastiss::i1_t *xor_result = gen_xor(psw_s, psw_ov);
      fastiss::i1_t *or_result = gen_or(xor_result, psw_z);
      return gen_icmp_eq(or_result, gen_i1_constant(1));
    }
    case NV_COND_CODE: { // No overflow [OV = 0]
      return gen_icmp_eq(psw_ov, gen_i1_constant(0));
    }
    case NC_COND_CODE: { // No carry [CY = 0]
      return gen_icmp_eq(psw_cy, gen_i1_constant(0));
    }
    case NZ_COND_CODE: { // Not zero [Z = 0]
      return gen_icmp_eq(psw_z, gen_i1_constant(0));
    }
    case H_COND_CODE: {  // Higher (greater than) [(CY or Z) = 0]
      fastiss::i1_t *or_result = gen_or(psw_cy, psw_z);
      return gen_icmp_eq(or_result, gen_i1_constant(0));
    }
    case P_COND_CODE: {  // Positive [S = 0]
      return gen_icmp_eq(psw_s, gen_i1_constant(0));
    }
    case SA_COND_CODE: { // Saturated [SAT = 1]
      return gen_icmp_eq(psw_sat, gen_i1_constant(1));
    }
    case GE_COND_CODE: { // Greater than or equal signed [(S xor OV) = 0]
      fastiss::i1_t *xor_result = gen_xor(psw_s, psw_ov);
      return gen_icmp_eq(xor_result, gen_i1_constant(0));
    }
    case GT_COND_CODE: { // Greater than signed [((S xor OV) or Z) = 0]
      fastiss::i1_t *xor_result = gen_xor(psw_s, psw_ov);
      fastiss::i1_t *or_result = gen_or(xor_result, psw_z);
      return gen_icmp_eq(or_result, gen_i1_constant(0));
    }
    default:
      assert(false);
      return NULL;
  }
}

void rh850g4::custom_impl_t::gen_div_reg1_reg2(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2)
{
  fastiss::i32_t *reg2_val = gen_read_GR(reg2);

  // Create sub-blocks
  fastiss::block_t *check_div_zero_block = create_basic_block();
  fastiss::block_t *div_zero_block = create_basic_block();
  fastiss::block_t *check_div_overflow_block = create_basic_block();
  fastiss::block_t *div_overflow_block = create_basic_block();
  fastiss::block_t *div_general_block = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  gen_branch(check_div_zero_block);

  // Handle division zero case
  set_gen_point(check_div_zero_block);
  gen_cond_branch(gen_icmp_eq(reg1_val, gen_i32_constant(0)), div_zero_block, check_div_overflow_block);

  set_gen_point(div_zero_block);
  // Update status flags
  gen_write_PSW_OV(gen_i1_constant(1));
  gen_branch(continue_block);

  set_gen_point(check_div_overflow_block);
  // Handle division overflow case
  fastiss::i1_t *reg2_is_int_min = gen_icmp_eq(reg2_val, gen_i32_constant(INT32_MIN));
  fastiss::i1_t *reg1_is_minus_one = gen_icmp_eq(reg1_val, gen_i32_constant(-1));
  gen_cond_branch(gen_and(reg2_is_int_min, reg1_is_minus_one), div_overflow_block, div_general_block);


  set_gen_point(div_overflow_block);
  // Update status flags
  gen_write_PSW_S(gen_calculate_PSW_S(reg2_val));
  gen_write_PSW_Z(gen_calculate_PSW_Z(reg2_val));
  gen_write_PSW_OV(gen_i1_constant(1));
  gen_branch(continue_block);

  // Handle general division case
  set_gen_point(div_general_block);
  // Calculate result
  reg2_val = gen_sdiv(reg2_val, reg1_val);
  // Write result back to register set
  gen_write_GR(reg2, reg2_val);
  // Update status flags
  gen_write_PSW_S(gen_calculate_PSW_S(reg2_val));
  gen_write_PSW_Z(gen_calculate_PSW_Z(reg2_val));
  gen_write_PSW_OV(gen_i1_constant(0));
  gen_branch(continue_block);

  set_gen_point(continue_block);
}
void rh850g4::custom_impl_t::gen_div_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2, rh850g4::GR_index_t reg3)
{
  fastiss::i32_t *reg2_val = gen_read_GR(reg2);

  // Create sub-blocks
  fastiss::block_t *check_div_zero_block = create_basic_block();
  fastiss::block_t *div_zero_block = create_basic_block();
  fastiss::block_t *check_div_overflow_block = create_basic_block();
  fastiss::block_t *div_overflow_block = create_basic_block();
  fastiss::block_t *div_general_block = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  gen_branch(check_div_zero_block);

  // Handle division zero case
  set_gen_point(check_div_zero_block);
  gen_cond_branch(gen_icmp_eq(reg1_val, gen_i32_constant(0)), div_zero_block, check_div_overflow_block);

  set_gen_point(div_zero_block);
  // Update status flags. 
  gen_write_PSW_OV(gen_i1_constant(1));

  gen_branch(continue_block);

  set_gen_point(check_div_overflow_block);
  // Handle division overflow case
  fastiss::i1_t *reg2_is_int_min = gen_icmp_eq(reg2_val, gen_i32_constant(INT32_MIN));
  fastiss::i1_t *reg1_is_minus_one = gen_icmp_eq(reg1_val, gen_i32_constant(-1));
  gen_cond_branch(gen_and(reg2_is_int_min, reg1_is_minus_one), div_overflow_block, div_general_block);

  set_gen_point(div_overflow_block);
  // Update status flags
  gen_write_PSW_S(gen_calculate_PSW_S(reg2_val));
  gen_write_PSW_Z(gen_calculate_PSW_Z(reg2_val));
  gen_write_PSW_OV(gen_i1_constant(1));
  gen_write_GR(reg3, gen_i32_constant(0));
  gen_branch(continue_block);

  // Handle general division case
  set_gen_point(div_general_block);

  // Calculate result
  fastiss::i32_t *reg3_val = gen_smod(reg2_val, reg1_val);
  reg2_val = gen_sdiv(reg2_val, reg1_val);

  // Write result back to register set
  gen_write_GR(reg2, reg2_val);
  gen_write_GR(reg3, reg3_val);

  // Update status flags.
  gen_write_PSW_S(gen_calculate_PSW_S(reg2_val));
  gen_write_PSW_Z(gen_calculate_PSW_Z(reg2_val));
  gen_write_PSW_OV(gen_i1_constant(0));

  gen_branch(continue_block);

  set_gen_point(continue_block);
}


void rh850g4::custom_impl_t::gen_divu_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2, rh850g4::GR_index_t reg3, bool is_halfword)
{
  fastiss::i32_t *reg2_val = gen_read_GR(reg2);

  // Create sub-blocks
  fastiss::block_t *check_div_zero_block = create_basic_block();
  fastiss::block_t *div_zero_block = create_basic_block();
  fastiss::block_t *div_general_block = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  gen_branch(check_div_zero_block);

  // Handle division zero case
  set_gen_point(check_div_zero_block);
  gen_cond_branch(gen_icmp_eq(reg1_val, gen_i32_constant(0)), div_zero_block, div_general_block);

  set_gen_point(div_zero_block);
  // Update status flags.
  gen_write_PSW_OV(gen_i1_constant(1));

  gen_branch(continue_block);

  // Handle general division case
  set_gen_point(div_general_block);

  // Calculate result
  fastiss::i32_t *reg3_val = gen_umod(reg2_val, reg1_val);
  reg2_val = gen_udiv(reg2_val, reg1_val);

  // Write result back to register set
  gen_write_GR(reg2, reg2_val);
  gen_write_GR(reg3, reg3_val);

  // Update status flags. 
  gen_write_PSW_S(gen_calculate_PSW_S(reg2_val));
  gen_write_PSW_Z(gen_calculate_PSW_Z(reg2_val));
  gen_write_PSW_OV(gen_i1_constant(0));

  gen_branch(continue_block);

  set_gen_point(continue_block);
}

fastiss::i1_t *rh850g4::custom_impl_t::is_unimplemented_exp(fastiss::f32_t *value)
{
  return gen_and(gen_is_denorm(value), gen_icmp_eq(gen_read_FPSR_FS(), gen_i1_constant(0)));
}

fastiss::i1_t *rh850g4::custom_impl_t::is_unimplemented_exp(fastiss::f64_t *value)
{
  return gen_and(gen_is_denorm(value), gen_icmp_eq(gen_read_FPSR_FS(), gen_i1_constant(0)));
}

fastiss::i1_t *rh850g4::custom_impl_t::is_unimplemented_fxu_exp(fastiss::f32_t *value)
{
  return gen_and(gen_is_denorm(value), gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0)));
}

fastiss::f32_t *rh850g4::custom_impl_t::gen_flush_denorm(fastiss::f32_t *value)
{
  // Update the IF bits of FPSR if input value is flashed
  gen_write_FPSR_IF(gen_or(gen_read_FPSR_IF(), gen_and(gen_read_FPSR_FS(), gen_is_denorm(value))));  
  
  return gen_select
  (
    gen_and(gen_read_FPSR_FS(), gen_is_denorm(value)),
    gen_f32_cast
    (
      gen_and
      (
        gen_i32_cast(value),
        gen_i32_constant(0x80000000)
      )
    ),
    value
  );
}

fastiss::f64_t *rh850g4::custom_impl_t::gen_flush_denorm(fastiss::f64_t *value)
{
  // Update the IF bits of FPSR if input value is flashed
  gen_write_FPSR_IF(gen_or(gen_read_FPSR_IF(), gen_and(gen_read_FPSR_FS(), gen_is_denorm(value))));  
  
  return gen_select
  (
    gen_and(gen_read_FPSR_FS(), gen_is_denorm(value)),
    gen_f64_cast
    (
      gen_and
      (
        gen_i64_cast(value),
        gen_i64_constant(0x8000000000000000ULL)
      )
    ),
    value
  );
}

fastiss::f32_t *rh850g4::custom_impl_t::gen_flush_denorm_fxu(fastiss::f32_t *value)
{
  // Update the IF bits of FPSR if input value is flashed
  gen_write_FXSR_IF(gen_or(gen_read_FXSR_IF(), gen_and(gen_read_FXSR_FS(), gen_is_denorm(value))));  
  
  return gen_select
  (
    gen_and(gen_read_FXSR_FS(), gen_is_denorm(value)),
    gen_f32_cast
    (
      gen_and
      (
        gen_i32_cast(value),
        gen_i32_constant(0x80000000)
      )
    ),
    value
  );
}

void rh850g4::custom_impl_t::gen_underflow_denorm_update(fastiss::f16_with_exception_flags_t& op_result)
{
  fastiss::i1_t *is_denormal = gen_is_denorm(op_result.result);
  op_result.underflow = gen_or(op_result.underflow, is_denormal);
  op_result.inexact = gen_or(op_result.inexact, is_denormal);
}

void rh850g4::custom_impl_t::gen_underflow_denorm_update(fastiss::f32_with_exception_flags_t& op_result)
{
  fastiss::i1_t *is_denormal = gen_is_denorm(op_result.result);
  op_result.underflow = gen_or(op_result.underflow, is_denormal);
  op_result.inexact = gen_or(op_result.inexact, is_denormal);
}

void rh850g4::custom_impl_t::gen_underflow_denorm_update(fastiss::f64_with_exception_flags_t& op_result)
{
  fastiss::i1_t *is_denormal = gen_is_denorm(op_result.result);
  op_result.underflow = gen_or(op_result.underflow, is_denormal);
  op_result.inexact = gen_or(op_result.inexact, is_denormal);
}

void rh850g4::custom_impl_t::gen_underflow_denorm_update(fastiss::f80_with_exception_flags_t& op_result)
{
  fastiss::i1_t *is_denormal = gen_is_denorm(op_result.result);
  op_result.underflow = gen_or(op_result.underflow, is_denormal);
  op_result.inexact = gen_or(op_result.inexact, is_denormal);
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg2)
{
  fastiss::i1_t* is_neither_invalid_nor_div_by_zero = gen_and
  (
    gen_not(invalid),
    gen_not(divide_by_zero)
  );
  return gen_select
  (
    gen_and
    (
      is_neither_invalid_nor_div_by_zero,
      gen_is_denorm(reg2)
    ),
    gen_true(),
    inexact
  );
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg2_pair)
{
  fastiss::i1_t* is_neither_invalid_nor_div_by_zero = gen_and
  (
    gen_not(invalid),
    gen_not(divide_by_zero)
  );
  return gen_select
  (
    gen_and
    (
      is_neither_invalid_nor_div_by_zero,
      gen_is_denorm(reg2_pair)
    ),
    gen_true(),
    inexact
  );
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2)
{
  fastiss::i1_t* is_neither_invalid_nor_div_by_zero = gen_and
  (
    gen_not(invalid),
    gen_not(divide_by_zero)
  );
  fastiss::i1_t* is_reg1_quiet_nan = gen_is_quiet_nan(reg1);
  fastiss::i1_t* is_reg2_quiet_nan = gen_is_quiet_nan(reg2);
  fastiss::i1_t* is_reg1_denorm_reg2_not_qnan = gen_and
  (
    gen_is_denorm(reg1),
    gen_not(is_reg2_quiet_nan)
  );
  fastiss::i1_t* is_reg2_denorm_reg1_not_qnan = gen_and
  (
    gen_is_denorm(reg2),
    gen_not(is_reg1_quiet_nan)
  );
  return gen_select
  (
    gen_and
    (
      is_neither_invalid_nor_div_by_zero,
      gen_or
      (
        is_reg1_denorm_reg2_not_qnan,
        is_reg2_denorm_reg1_not_qnan
      )
    ),
    gen_true(),
    inexact
  );
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg1_pair, fastiss::f64_t *reg2_pair)
{
  fastiss::i1_t* is_neither_invalid_nor_div_by_zero = gen_and
  (
    gen_not(invalid),
    gen_not(divide_by_zero)
  );
  fastiss::i1_t* is_reg1_pair_quiet_nan = gen_is_quiet_nan(reg1_pair);
  fastiss::i1_t* is_reg2_pair_quiet_nan = gen_is_quiet_nan(reg2_pair);
  fastiss::i1_t* is_reg1_pair_denorm_reg2_pair_not_qnan = gen_and
  (
    gen_is_denorm(reg1_pair),
    gen_not(is_reg2_pair_quiet_nan)
  );
  fastiss::i1_t* is_reg2_pair_denorm_reg1_pair_not_qnan = gen_and
  (
    gen_is_denorm(reg2_pair),
    gen_not(is_reg1_pair_quiet_nan)
  );
  return gen_select
  (
    gen_and
    (
      is_neither_invalid_nor_div_by_zero,
      gen_or
      (
        is_reg1_pair_denorm_reg2_pair_not_qnan,
        is_reg2_pair_denorm_reg1_pair_not_qnan
      )
    ),
    gen_true(),
    inexact
  );
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2, fastiss::f32_t *reg3)
{
  fastiss::i1_t* is_neither_invalid_nor_div_by_zero = gen_and
  (
    gen_not(invalid),
    gen_not(divide_by_zero)
  );
  fastiss::i1_t* is_reg1_quiet_nan = gen_is_quiet_nan(reg1);
  fastiss::i1_t* is_reg2_quiet_nan = gen_is_quiet_nan(reg2);
  fastiss::i1_t* is_reg3_quiet_nan = gen_is_quiet_nan(reg3);
  fastiss::i1_t* is_reg1_denorm_reg2_reg3_not_qnan = gen_and
  (
    gen_is_denorm(reg1),
    gen_and
    (
      gen_not(is_reg2_quiet_nan),
      gen_not(is_reg3_quiet_nan)
    )
  );
  fastiss::i1_t* is_reg2_denorm_reg1_reg3_not_qnan = gen_and
  (
    gen_is_denorm(reg2),
    gen_and
    (
      gen_not(is_reg1_quiet_nan),
      gen_not(is_reg3_quiet_nan)
    )
  );
  fastiss::i1_t* is_reg3_denorm_reg1_reg2_not_qnan = gen_and
  (
    gen_is_denorm(reg3),
    gen_and
    (
      gen_not(is_reg1_quiet_nan),
      gen_not(is_reg2_quiet_nan)
    )
  );
  return gen_select
  (
    gen_and
    (
      is_neither_invalid_nor_div_by_zero,
      gen_or
      (
        gen_or
        (
          is_reg1_denorm_reg2_reg3_not_qnan,
          is_reg2_denorm_reg1_reg3_not_qnan
        ),
        is_reg3_denorm_reg1_reg2_not_qnan
      )
    ),
    gen_true(),
    inexact
  );
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_inexact_enabled_overflow_underflow_update(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact)
{
  return gen_select
  (
    gen_or(
      gen_and
      (
        overflow,
        gen_i1_extract(gen_read_FPSR_XE(), gen_i8_constant(2))
      ),
      gen_and
      (
        underflow,
        gen_i1_extract(gen_read_FPSR_XE(), gen_i8_constant(1))
      )
    ),  
    gen_false(),
    inexact
  );
}

fastiss::i1_t *rh850g4::custom_impl_t::gen_inexact_enabled_overflow_underflow_update_fxu(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact)
{
  return gen_select
  (
    gen_or(
      gen_and
      (
        overflow,
        gen_i1_extract(gen_read_FXSR_XE(), gen_i8_constant(2))
      ),
      gen_and
      (
        underflow,
        gen_i1_extract(gen_read_FXSR_XE(), gen_i8_constant(1))
      )
    ),  
    gen_false(),
    inexact
  );
}

void rh850g4::custom_impl_t::gen_update_dest_and_fpsr(bool is_single_precision, rh850g4::GR_index_t dest_index, fastiss::i32_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block)
{
  // Create sub-blocks
  fastiss::block_t *exception_branch = create_basic_block();
  fastiss::block_t *no_exception_branch = create_basic_block();
  fastiss::block_t *acknowledged_exception_branch = create_basic_block();
  fastiss::block_t *delayed_acknowledged_exception_branch = create_basic_block();  
  
  // Determine if any exception has occurred and is also enabled
  fastiss::i1_t* is_enabled_exception = gen_icmp_ne(gen_and(gen_or(gen_i8_constant(0x20), gen_read_FPSR_XE()), exception_flags), gen_i8_constant(0));

  // Branch to either the exception or non-exception case
  gen_cond_branch(is_enabled_exception, exception_branch, no_exception_branch);

  // Handle non-exception case - write the result to the destination register
  // and update the preservation bits of FPSR
  set_gen_point(no_exception_branch);  
  gen_write_GR(dest_index, dest_val);
  gen_write_FPSR_XP(gen_or(gen_read_FPSR_XP(), exception_flags));
  gen_branch(continue_block);

  // Handle exception case
  set_gen_point(exception_branch);
  
  // Determine if precise or imprecise exception mode is selected
  fastiss::i1_t* is_precise = gen_read_FPSR_PEM();

  // Update the cause bits of FPSR if an enabled exception has occurred
  gen_write_FPSR_XC(exception_flags);  
  
  // Write the PC address to the FPEPC
  fastiss::i32_t* pc_load = gen_read_PC();
  gen_write_FPEPC(pc_load);

  // Branch to either the acknowledge exception branch or the delayed acknowledged branch
  gen_cond_branch(is_precise, acknowledged_exception_branch, delayed_acknowledged_exception_branch);
  
  // Handle delayed acknowledged exception case - update the pending floating point imprecise exception flag
  set_gen_point(delayed_acknowledged_exception_branch);
  fastiss::i32_ptr_t* pending_flag_address = gen_i32_constant_ptr(m_pending_fpu_imprecise_exception_flag);
  gen_store(gen_i32_constant(1), pending_flag_address);
  gen_branch(continue_block);
  
  // Handle acknowledged exception case
  set_gen_point(acknowledged_exception_branch);

  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());
 
  // Write the PC address to the EIPC and the PSW to the EIPSW
  gen_write_EIPC(pc_load);
  gen_write_EIPSW(gen_read_PSW());
  
  // Write the exception cause to the EIIC register
  gen_write_EIIC(gen_i32_constant(0x00000071));
  
  // Set the ID and EP fields of the PSW register
  gen_write_PSW_ID(gen_true());
  gen_write_PSW_EP(gen_true());

  // clear the UM flags
  gen_write_PSW_UM(gen_i1_constant(0));
    
  // Update the PC to the floating point exception handler address if an enabled exception has occurred
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x00000070)));

  if(m_link_func_enable){
    // clear the link(ll-bit) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }

  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();

  // Branch to the exit block
  gen_branch(get_exit_block());
}

void rh850g4::custom_impl_t::gen_update_dest_and_fpsr(bool is_single_precision, rh850g4::GR_index_t dest_index, fastiss::i64_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block)
{
  // Create sub-blocks
  fastiss::block_t *exception_branch = create_basic_block();
  fastiss::block_t *no_exception_branch = create_basic_block();
  fastiss::block_t *acknowledged_exception_branch = create_basic_block();
  fastiss::block_t *delayed_acknowledged_exception_branch = create_basic_block();  
  
  // Determine if any exception has occurred and is also enabled
  fastiss::i1_t* is_enabled_exception = gen_icmp_ne(gen_and(gen_or(gen_i8_constant(0x20), gen_read_FPSR_XE()), exception_flags), gen_i8_constant(0));

  // Branch to either the exception or non-exception case
  gen_cond_branch(is_enabled_exception, exception_branch, no_exception_branch);

  // Handle non-exception case - write the result to the destination register
  // pair and update the preservation bits of FPSR
  set_gen_point(no_exception_branch);
  GR_index_t dest_index_upper = GR_index_t(dest_index + 1);  
  gen_write_GR(dest_index, gen_i32_extract(dest_val, gen_i64_constant(0)));
  gen_write_GR(dest_index_upper, gen_i32_extract(dest_val, gen_i64_constant(1)));
  gen_write_FPSR_XP(gen_or(gen_read_FPSR_XP(), exception_flags));
  gen_branch(continue_block);

  // Handle exception case
  set_gen_point(exception_branch);
  
  // Determine if precise or imprecise exception mode is selected
  fastiss::i1_t* is_precise = gen_read_FPSR_PEM();

  // Update the cause bits of FPSR if an enabled exception has occurred
  gen_write_FPSR_XC(exception_flags);  
  
  // Write the PC address to the FPEPC
  fastiss::i32_t* pc_load = gen_read_PC();
  gen_write_FPEPC(pc_load);

  // Branch to either the acknowledge exception branch or the delayed acknowledged branch
  gen_cond_branch(is_precise, acknowledged_exception_branch, delayed_acknowledged_exception_branch);
  
  // Handle delayed acknowledged exception case - update the pending floating point imprecise exception flag
  set_gen_point(delayed_acknowledged_exception_branch);
  fastiss::i32_ptr_t* pending_flag_address = gen_i32_constant_ptr(m_pending_fpu_imprecise_exception_flag);
  gen_store(gen_i32_constant(1), pending_flag_address);
  gen_branch(continue_block);
  
  // Handle acknowledged exception case
  set_gen_point(acknowledged_exception_branch);

  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());

  // Write the PC address to the EIPC and the PSW to the EIPSW
  gen_write_EIPC(pc_load);
  gen_write_EIPSW(gen_read_PSW());
  
  // Write the exception cause to the EIIC register
  gen_write_EIIC(gen_i32_constant(0x00000071));
  
  // Set the ID and EP fields of the PSW register
  gen_write_PSW_ID(gen_true());
  gen_write_PSW_EP(gen_true());
  
  // clear the UM flags
  gen_write_PSW_UM(gen_i1_constant(0));
    
  // Update the PC to the floating point exception handler address if an enabled exception has occurred
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x00000070)));

  if(m_link_func_enable){
    // clear the link(ll-bit) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }

  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();

  // Branch to the exit block
  gen_branch(get_exit_block());
}

void rh850g4::custom_impl_t::gen_update_dest_and_fxsr(bool is_single_precision, rh850g4::WR_index_t dest_index, fastiss::i64_t* dest_lo_val, fastiss::i64_t* dest_up_val, fastiss::i8_t* w0_exp_flag, fastiss::i8_t* w1_exp_flag, fastiss::i8_t* w2_exp_flag, fastiss::i8_t* w3_exp_flag, fastiss::block_t *continue_block)
{
  // Create sub-blocks
  fastiss::block_t *exception_branch = create_basic_block();
  fastiss::block_t *no_exception_branch = create_basic_block();
  fastiss::block_t *acknowledged_exception_branch = create_basic_block();
  fastiss::block_t *delayed_acknowledged_exception_branch = create_basic_block();  
  
  fastiss::i8_t *exception_flags = gen_or(gen_or(gen_or(w0_exp_flag, w1_exp_flag), w2_exp_flag), w3_exp_flag);

  // Determine if any exception has occurred and is also enabled
  fastiss::i1_t* is_enabled_exception = gen_icmp_ne(gen_and(gen_or(gen_i8_constant(0x20), gen_read_FXSR_XE()), exception_flags), gen_i8_constant(0));

  // Branch to either the exception or non-exception case
  gen_cond_branch(is_enabled_exception, exception_branch, no_exception_branch);

  // Handle non-exception case - write the result to the destination register
  // and update the preservation bits of FXSR
  set_gen_point(no_exception_branch);  
  WR_index_t dest_index_upper = WR_index_t(dest_index + 1);  
  gen_write_WR(dest_index, dest_lo_val);
  gen_write_WR(dest_index_upper, dest_up_val);
  gen_write_FXSR_XP(gen_or(gen_read_FXSR_XP(), exception_flags));
  gen_write_FXXP_XP0(w0_exp_flag);
  gen_write_FXXP_XP1(w1_exp_flag);
  gen_write_FXXP_XP2(w2_exp_flag);
  gen_write_FXXP_XP3(w3_exp_flag);
  gen_branch(continue_block);

  // Handle exception case
  set_gen_point(exception_branch);
  
  // Determine if precise or imprecise exception mode is selected
  fastiss::i1_t* is_precise = gen_read_FXSR_PEM();

  // Update the cause bits of FXSR if an enabled exception has occurred
  gen_write_FXSR_XC(exception_flags);  
  gen_write_FXXC_XC0(w0_exp_flag);
  gen_write_FXXC_XC1(w1_exp_flag);
  gen_write_FXXC_XC2(w2_exp_flag);
  gen_write_FXXC_XC3(w3_exp_flag);
  
  // Branch to either the acknowledge exception branch or the delayed acknowledged branch
  gen_cond_branch(is_precise, acknowledged_exception_branch, delayed_acknowledged_exception_branch);
  
  // Handle delayed acknowledged exception case - update the pending floating point imprecise exception flag
  set_gen_point(delayed_acknowledged_exception_branch);
  //fastiss::i32_ptr_t* pending_flag_address = gen_i32_constant_ptr(m_pending_fpu_imprecise_exception_flag);
  //gen_store(gen_i32_constant(1), pending_flag_address);
  gen_branch(continue_block);
  
  // Handle acknowledged exception case
  set_gen_point(acknowledged_exception_branch);

  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());
 
  // Write the PC address to the EIPC and the PSW to the EIPSW
  fastiss::i32_t* pc_load = gen_read_PC();
  gen_write_EIPC(pc_load);
  gen_write_EIPSW(gen_read_PSW());
  
  // Write the exception cause to the EIIC register
  gen_write_EIIC(gen_i32_constant(0x00000075));
  
  // Set the ID and EP fields of the PSW register
  gen_write_PSW_ID(gen_true());
  gen_write_PSW_EP(gen_true());

  // clear the UM flags
  gen_write_PSW_UM(gen_i1_constant(0));
    
  // Update the PC to the floating point exception handler address if an enabled exception has occurred
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x00000070)));

  if(m_link_func_enable){
    // clear the link(ll-bit) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }

  // Branch to the exit block
  gen_branch(get_exit_block());
}

fastiss::i8_t *rh850g4::custom_impl_t::gen_read_round_mode(void)
{
  fastiss::i8_t *i8_rm = gen_read_FPSR_RM();
  return gen_select
    (
      gen_icmp_eq(i8_rm, gen_i8_constant(1)), 
        gen_i8_constant(3), 
        gen_select
        (
          gen_icmp_eq(i8_rm, gen_i8_constant(3)),
            gen_i8_constant(1),
            i8_rm
	)
    );
}

void rh850g4::custom_impl_t::gen_clear_fpi_flag(void)
{
  fastiss::i32_ptr_t* pending_flag_address = gen_i32_constant_ptr(m_pending_fpu_imprecise_exception_flag);
  gen_store(gen_i32_constant(0), pending_flag_address);
}

void rh850g4::custom_impl_t::gen_bins_common(rh850g4::GR_index_t reg1, rh850g4::GR_index_t reg2, fastiss::i32_t *msb_val, fastiss::i32_t *lsb_val)
{
  fastiss::i32_t *i32_zero = gen_i32_constant(0);
  fastiss::i32_t *i32_one = gen_i32_constant(1);
  fastiss::i32_t *i32_31 = gen_i32_constant(31);

  fastiss::i32_t *reg1_val = gen_read_GR(reg1);
  fastiss::i32_t *reg2_val = gen_read_GR(reg2);
  
  fastiss::i32_t *hi_pos = gen_add(msb_val, i32_one);
  fastiss::i32_t *md_pos = gen_sub(msb_val, lsb_val);
  fastiss::i32_t *width  = gen_add(md_pos, i32_one);
  fastiss::i32_t *pos    = lsb_val;

  fastiss::i32_t *res_hi = gen_i32_constant(0);
  fastiss::i32_t *res_md = gen_i32_constant(0);
  fastiss::i32_t *res_lo = gen_i32_constant(0);
  fastiss::i32_t *tmp1   = gen_i32_constant(0);

  rh850g4::GR_index_t temp_reg1 = GR_index_t(reg2 + 10);
  rh850g4::GR_index_t temp_reg2 = GR_index_t(reg2 + 11);

  if(msb_val != i32_31){
    tmp1 = gen_bit_extract(reg2_val, i32_31, hi_pos);
    res_hi = gen_shl(tmp1, hi_pos);
  }

  tmp1 = gen_bit_extract(reg1_val, md_pos, i32_zero);
  res_md = gen_shl(tmp1, pos);

  if(pos != i32_zero){
    tmp1 = gen_sub(pos, i32_one);
    res_lo = gen_bit_extract(reg2_val, tmp1, i32_zero);
  }

  tmp1 = gen_or(res_hi, res_md);
  fastiss::i32_t *res = gen_or(tmp1, res_lo);
  gen_write_GR(reg2, res);

  // Update status flags
  gen_write_PSW_S(gen_calculate_PSW_S(res));
  gen_write_PSW_Z(gen_calculate_PSW_Z(res));
  gen_write_PSW_OV(gen_i1_constant(0));
}

fastiss::i32_t *rh850g4::custom_impl_t::gen_bit_extract(fastiss::i32_t *x_val, fastiss::i32_t *l_val, fastiss::i32_t *r_val)
{
  fastiss::i32_t *i32_31   = gen_i32_constant(31);
  fastiss::i32_t *shift  = gen_sub(i32_31, l_val);;
  fastiss::i32_t *tmp_val1 = gen_shl(x_val, shift);
  tmp_val1 = gen_lshr(tmp_val1, shift);
  return (gen_lshr(tmp_val1, r_val));
}

fastiss::i32_t *rh850g4::custom_impl_t::gen_vector_shuffle(fastiss::i32_t *pos, fastiss::i32_t *imm12_val, rh850g4::WR_index_t wreg1, rh850g4::WR_index_t wreg2)
{
  // Load operand registers
  fastiss::i64_t *wreg1lo_load = gen_read_WR(wreg1);
  fastiss::i64_t *wreg1up_load = gen_read_WR(rh850g4::WR_index_t(wreg1 + 1));
  fastiss::i64_t *wreg2lo_load = gen_read_WR(wreg2);
  fastiss::i64_t *wreg2up_load = gen_read_WR(rh850g4::WR_index_t(wreg2 + 1));

  // Extract upper and lower words of the result
  fastiss::i32_t *wr1_w0_val = gen_i32_extract(wreg1lo_load, gen_i64_constant(0));
  fastiss::i32_t *wr1_w1_val = gen_i32_extract(wreg1lo_load, gen_i64_constant(1));
  fastiss::i32_t *wr1_w2_val = gen_i32_extract(wreg1up_load, gen_i64_constant(0));
  fastiss::i32_t *wr1_w3_val = gen_i32_extract(wreg1up_load, gen_i64_constant(1));
  fastiss::i32_t *wr2_w0_val = gen_i32_extract(wreg2lo_load, gen_i64_constant(0));
  fastiss::i32_t *wr2_w1_val = gen_i32_extract(wreg2lo_load, gen_i64_constant(1));
  fastiss::i32_t *wr2_w2_val = gen_i32_extract(wreg2up_load, gen_i64_constant(0));
  fastiss::i32_t *wr2_w3_val = gen_i32_extract(wreg2up_load, gen_i64_constant(1));

  fastiss::i32_t *sel_val = gen_and(gen_i32_constant(0x7), gen_lshr(imm12_val, gen_mul(pos, gen_i32_constant(3))));
  fastiss::i32_t *ret_val = gen_select(gen_icmp_eq(gen_i32_constant(0x7), sel_val), 
				       wr2_w3_val, 
				       gen_select(gen_icmp_eq(gen_i32_constant(0x6), sel_val),
						  wr2_w2_val,
						  gen_select(gen_icmp_eq(gen_i32_constant(0x5), sel_val),
							     wr2_w1_val,
							     gen_select(gen_icmp_eq(gen_i32_constant(0x4), sel_val),
									wr2_w0_val,
									gen_select(gen_icmp_eq(gen_i32_constant(0x3), sel_val),
										   wr1_w3_val,
										   gen_select(gen_icmp_eq(gen_i32_constant(0x2), sel_val),
											      wr1_w2_val,
											      gen_select(gen_icmp_eq(gen_i32_constant(0x1), sel_val),
													 wr1_w1_val,
													 wr1_w0_val)))))));
  return(ret_val);
}

void rh850g4::custom_impl_t::gen_fp_simd_add(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i1_t *is_neg)
{
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2));

  // Check if both operands are infinity with opposite signs
  fastiss::i1_t* is_both_infinity = gen_and(gen_is_infinity(input_val_1), gen_is_infinity(input_val_2));
  fastiss::i1_t* is_opposite_sign = gen_icmp_ne(gen_is_positive(input_val_1), gen_is_positive(input_val_2));
  fastiss::i1_t* is_opposite_infinities = gen_and(is_both_infinity, is_opposite_sign);

  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  // or both operands are infinity with opposite signs
  gen_cond_branch(gen_or(is_either_signalling_nan, is_opposite_infinities), invalid_qnan_branch, perform_calc_branch);

  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  
  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val_1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val_2_float80 = gen_f80_ext(val_2_flushed);
  
  // Determine the ADD result - always round to nearest
  fastiss::f32_with_exception_flags_t add_result = gen_f32_add(val_1_float80, val_2_float80, gen_read_round_mode());
    
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(add_result);

   // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_and(add_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  add_result.result = gen_select
  (
    add_result.underflow,
    gen_select(is_neg, 
	       gen_neg(gen_f32_cast(gen_and(gen_i32_cast(add_result.result), gen_i32_constant(0x80000000)))),
	       gen_f32_cast(gen_and(gen_i32_cast(add_result.result), gen_i32_constant(0x80000000)))),
    gen_select(is_neg, gen_neg(add_result.result), add_result.result)
  );
  
  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  add_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(add_result.overflow, add_result.underflow, add_result.inexact);
  
  // Check if the inexact flag must be explicitly set
  add_result.inexact = gen_inexact_denorm_update(add_result.invalid, add_result.divide_by_zero, add_result.inexact, input_val_1, input_val_2);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1),
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_i32_cast(add_result.result))),
	    res_val_ptr);

  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), add_result.invalid, add_result.divide_by_zero, add_result.overflow, add_result.underflow, add_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_sub(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i1_t *is_neg)
{
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2));
  
  // Check if both operands are infinity with opposite signs
  fastiss::i1_t* is_both_infinity = gen_and(gen_is_infinity(input_val_1), gen_is_infinity(input_val_2));
  fastiss::i1_t* is_same_sign = gen_icmp_eq(gen_is_positive(input_val_1), gen_is_positive(input_val_2));
  fastiss::i1_t* is_same_infinities = gen_and(is_both_infinity, is_same_sign);

  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  // or both operands are infinity with opposite signs
  gen_cond_branch(gen_or(is_either_signalling_nan, is_same_infinities), invalid_qnan_branch, perform_calc_branch);

  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  
  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val_1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val_2_float80 = gen_f80_ext(val_2_flushed);
  
  // Determine the SUB result - always round to nearest
  fastiss::f32_with_exception_flags_t sub_result = gen_f32_sub(val_1_float80, val_2_float80, gen_read_round_mode());
    
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(sub_result);

   // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_and(sub_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
                  unimplemented_exp_res_branch,
                  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  sub_result.result = gen_select
  (
    sub_result.underflow,
    gen_select(is_neg, 
	       gen_neg(gen_f32_cast(gen_and(gen_i32_cast(sub_result.result), gen_i32_constant(0x80000000)))),
	       gen_f32_cast(gen_and(gen_i32_cast(sub_result.result), gen_i32_constant(0x80000000)))),
    gen_select(is_neg, gen_neg(sub_result.result), sub_result.result)
  );
  
  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  sub_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(sub_result.overflow, sub_result.underflow, sub_result.inexact);
  
  // Check if the inexact flag must be explicitly set
  sub_result.inexact = gen_inexact_denorm_update(sub_result.invalid, sub_result.divide_by_zero, sub_result.inexact, input_val_1, input_val_2);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1),
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_i32_cast(sub_result.result))), 
	    res_val_ptr);

  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), sub_result.invalid, sub_result.divide_by_zero, sub_result.overflow, sub_result.underflow, sub_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_mul(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  
  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2));
  
  // Check if one operand is infinity and the other zero
  fastiss::i1_t* is_val1_infinity = gen_is_infinity(input_val_1);
  fastiss::i1_t* is_val2_infinity = gen_is_infinity(input_val_2);
  fastiss::i1_t* is_val1_zero = gen_is_zero(val_1_flushed);
  fastiss::i1_t* is_val2_zero = gen_is_zero(val_2_flushed);
  fastiss::i1_t* is_one_infinity_other_zero = gen_or(gen_and(is_val1_infinity, is_val2_zero), gen_and(is_val2_infinity, is_val1_zero));

  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  // or one operand is infinity and the other zero
  gen_cond_branch(gen_or(is_either_signalling_nan, is_one_infinity_other_zero), invalid_qnan_branch, perform_calc_branch);

  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val_1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val_2_float80 = gen_f80_ext(val_2_flushed);
  
  // Determine the MUL result - always round to nearest
  fastiss::f32_with_exception_flags_t mul_result = gen_f32_mul(val_1_float80, val_2_float80, gen_read_round_mode());
   
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(mul_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_and(mul_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  mul_result.result = gen_select
  (
    mul_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(mul_result.result), gen_i32_constant(0x80000000))),
    mul_result.result
  );
 
  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  mul_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(mul_result.overflow, mul_result.underflow, mul_result.inexact);
  
  // Check if the inexact flag must be explicitly set
  mul_result.inexact = gen_inexact_denorm_update(mul_result.invalid, mul_result.divide_by_zero, mul_result.inexact, input_val_1, input_val_2);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1),
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_i32_cast(mul_result.result))), 
	    res_val_ptr);

  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), mul_result.invalid, mul_result.divide_by_zero, mul_result.overflow, mul_result.underflow, mul_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_div(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *check_div_by_zero_branch = create_basic_block();
  fastiss::block_t *div_by_zero_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  
  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2));
  
  // Check if both operands are infinity or if both operands are zero
  fastiss::i1_t* is_val1_infinity = gen_is_infinity(input_val_1);
  fastiss::i1_t* is_both_infinity = gen_and(gen_is_infinity(input_val_2), is_val1_infinity);
  fastiss::i1_t* is_val2_zero = gen_is_zero(val_2_flushed);
  fastiss::i1_t* is_both_zero = gen_and(is_val2_zero, gen_is_zero(val_1_flushed));
  fastiss::i1_t* is_both_infinity_or_zero = gen_or(is_both_infinity, is_both_zero);

  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  // or both operands are infinity or zero
  gen_cond_branch(gen_or(is_either_signalling_nan, is_both_infinity_or_zero), invalid_qnan_branch, check_div_by_zero_branch);

  // Check if dividing by zero and reg2 is neither a quiet NaN or infinity
  set_gen_point(check_div_by_zero_branch);
  fastiss::i1_t* is_val1_qnan_or_infinity = gen_or(gen_is_quiet_nan(input_val_1), is_val1_infinity);
  fastiss::i1_t* is_val2_zero_val1_neither_qnan_nor_infinity = gen_and(is_val2_zero, gen_not(is_val1_qnan_or_infinity));
  gen_cond_branch(is_val2_zero_val1_neither_qnan_nor_infinity, div_by_zero_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle divide by zero case
  set_gen_point(div_by_zero_branch);
  fastiss::i1_t* is_same_sign = gen_icmp_eq(gen_is_positive(input_val_1), gen_is_positive(input_val_2));
  gen_store(gen_select(is_same_sign, gen_i32_constant(0x7f800000), gen_i32_constant(0xff800000)), res_val_ptr);
  gen_store(gen_i8_constant(0x08), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val_1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val_2_float80 = gen_f80_ext(val_2_flushed);
  
  // Determine the DIV result - always round to nearest
  fastiss::f32_with_exception_flags_t div_result = gen_f32_div(val_1_float80, val_2_float80, gen_read_round_mode());
   
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(div_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_and(div_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  div_result.result = gen_select
  (
    div_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(div_result.result), gen_i32_constant(0x80000000))),
    div_result.result
  );

 
  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  div_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(div_result.overflow, div_result.underflow, div_result.inexact);
  
  // Check if the inexact flag must be explicitly set
  div_result.inexact = gen_inexact_denorm_update(div_result.invalid, div_result.divide_by_zero, div_result.inexact, input_val_1, input_val_2);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1),
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_i32_cast(div_result.result))), 
	    res_val_ptr);

  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), div_result.invalid, div_result.divide_by_zero, div_result.overflow, div_result.underflow, div_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_max(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2));
 
  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  gen_cond_branch(is_either_signalling_nan, invalid_qnan_branch, check_qnan_branch);

  // Check if both operand are quiet NaN
  set_gen_point(check_qnan_branch);
  fastiss::i1_t* is_both_quiet_nan = gen_and(gen_is_quiet_nan(input_val_1), gen_is_quiet_nan(input_val_2));
  gen_cond_branch(is_both_quiet_nan, qnan_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle quiet NaN case
  set_gen_point(qnan_branch);
  gen_store(gen_i32_cast(input_val_1), res_val_ptr);
  gen_store(gen_i8_constant(0), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);

  // Determine the MAX result
  gen_store(gen_i32_cast(gen_select(gen_and(gen_is_zero(input_val_1), gen_is_zero(input_val_2)),
				    input_val_1,
				    gen_select(gen_is_quiet_nan(input_val_2), 
					       input_val_1, 
					       gen_select(gen_is_quiet_nan(input_val_1), 
							  input_val_2,
							  gen_select(gen_fcmp_uge(input_val_1, input_val_2), 
								     input_val_1,
								     input_val_2))))),
	    res_val_ptr);

  gen_store(gen_i8_constant(0), res_exp_ptr);
  gen_branch(cont_block);

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_min(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2));
 
  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  gen_cond_branch(is_either_signalling_nan, invalid_qnan_branch, check_qnan_branch);

  // Check if both operand are quiet NaN
  set_gen_point(check_qnan_branch);
  fastiss::i1_t* is_both_quiet_nan = gen_and(gen_is_quiet_nan(input_val_1), gen_is_quiet_nan(input_val_2));
  gen_cond_branch(is_both_quiet_nan, qnan_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle quiet NaN case
  set_gen_point(qnan_branch);
  gen_store(gen_i32_cast(input_val_1), res_val_ptr);
  gen_store(gen_i8_constant(0), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Determine the MIX result
  gen_store(gen_i32_cast(gen_select(gen_and(gen_is_zero(input_val_1), gen_is_zero(input_val_2)),
				    input_val_2,
				    gen_select(gen_is_quiet_nan(input_val_2), 
					       input_val_1, 
					       gen_select(gen_is_quiet_nan(input_val_1), 
							  input_val_2,
							  gen_select(gen_fcmp_ule(input_val_1, input_val_2), 
								     input_val_1,
								     input_val_2))))),
	    res_val_ptr);
  gen_store(gen_i8_constant(0), res_exp_ptr);
  
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_cvt_hs(fastiss::f16_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  // Create sub-blocks
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_signalling_nan = gen_is_signalling_nan(input_val_1);

  // Branch to the invalid quiet Nan case if the operand is a signalling NaN
  gen_cond_branch(is_signalling_nan, invalid_qnan_branch, perform_calc_branch);

  // Handle invalid nan case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operand to a 32-bit floating point value
  fastiss::f32_t *convert_result = gen_f32_ext(input_val_1);

  // Update the result, FPSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1), gen_select(gen_is_positive(input_val_1), gen_i32_constant(0x7fffe000), gen_i32_constant(0xffffe000)), gen_i32_cast(convert_result)), res_val_ptr);
  gen_store(gen_i8_constant(0), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_cvt_sh(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(is_unimplemented_fxu_exp(input_val_1), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_signalling_nan = gen_is_signalling_nan(input_val_1);

  // Branch to the invalid quiet Nan case if the operand is a signalling NaN
  gen_cond_branch(is_signalling_nan, invalid_qnan_branch, perform_calc_branch);

  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val1_float32_flushed = gen_flush_denorm_fxu(input_val_1);

  // Truncate the operand to 16-bits - always round to nearest
  fastiss::f16_with_exception_flags_t trunc_result = gen_f16_trunc(val1_float32_flushed, round_mode);
 
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(trunc_result);

  // Check if the result is a denormaled or underflow value and FXSR.FS=0
  gen_cond_branch(gen_and(trunc_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  trunc_result.result = gen_select
  (
    trunc_result.underflow,
    gen_f16_cast(gen_and(gen_i16_cast(trunc_result.result), gen_i16_constant(0x8000))),
    trunc_result.result
  );

  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  trunc_result.inexact = gen_inexact_enabled_overflow_underflow_update(trunc_result.overflow, trunc_result.underflow, trunc_result.inexact);
   
  // Check if the inexact flag must be explicitly set due to a denorm result
  trunc_result.inexact = gen_inexact_denorm_update(trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.inexact, input_val_1);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1), gen_select(gen_is_positive(input_val_1), gen_i32_constant(0x7fff), gen_i32_constant(0xffff)), gen_i32_zext(gen_i16_cast(trunc_result.result))), res_val_ptr);
  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.overflow, trunc_result.underflow, trunc_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);

}

void rh850g4::custom_impl_t::gen_fp_simd_cvt_suw(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_nan_branch = create_basic_block();
  fastiss::block_t *check_saturation_branch = create_basic_block();
  fastiss::block_t *saturation_branch = create_basic_block();  
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(is_unimplemented_fxu_exp(input_val_1), unimplemented_exp_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);

  // Treat negative zero the same as positive zero
  val_1_flushed = gen_select(gen_is_zero(val_1_flushed), gen_f32_constant(0), val_1_flushed);
  
  // Branch to the invalid nan case if the operand is a NaN or a negative value
  fastiss::i1_t *is_nan = gen_is_nan(input_val_1);
  fastiss::i1_t* is_negative = gen_is_negative(val_1_flushed);
  gen_cond_branch(gen_or(is_nan, is_negative), invalid_nan_branch, check_saturation_branch);
  
  // Check if the conversion will saturate. Saturation occurs when:
  //    - For a positive floating point number, if it is greater than 2^32-1 (represented as 0x4f800000 in single precision floating point)
  set_gen_point(check_saturation_branch);
  fastiss::i1_t *is_saturated = gen_fcmp_oge(val_1_flushed, gen_f32_cast(gen_i32_constant(0x4f800000)));
  gen_cond_branch(is_saturated, saturation_branch, perform_calc_branch);
  
  // Handle invalid nan case
  set_gen_point(invalid_nan_branch);
  gen_store(gen_i32_constant(0x00000000), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle saturation case
  set_gen_point(saturation_branch);
  gen_store(gen_i32_constant(0xffffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Convert the operand to a 32-bit integer value
  fastiss::i32_with_exception_flags_t convert_result = gen_i32_unsigned_convert(val_1_flushed, round_mode);

  // Check if the inexact flag must be explicitly set
  convert_result.inexact = gen_inexact_denorm_update(convert_result.invalid, convert_result.divide_by_zero, convert_result.inexact, input_val_1);
 
  // Update the result, FXSR.XP, and handle exceptions
  gen_store(convert_result.result, res_val_ptr);
  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), convert_result.invalid, convert_result.divide_by_zero, convert_result.overflow, convert_result.underflow, convert_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_cvt_sw(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_nan_branch = create_basic_block();
  fastiss::block_t *check_saturation_branch = create_basic_block();
  fastiss::block_t *saturation_branch = create_basic_block();  
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(is_unimplemented_fxu_exp(input_val_1), unimplemented_exp_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);

  // Branch to the invalid nan case if the operand is a NaN
  gen_cond_branch(gen_is_nan(input_val_1), invalid_nan_branch, check_saturation_branch);
  
  // Check if the conversion will saturate. Saturation occurs when:
  //    - For a positive floating point number, if it is greater than 2^31-1 (represented as 0x4f000000 in single precision floating point)
  //    - For a negative floating point number, if it is less than -2^31 (represented as 0xcf000000 in single precision floating point)
  set_gen_point(check_saturation_branch);
  fastiss::i1_t *is_positive = gen_is_positive(input_val_1);
  fastiss::i1_t *is_saturated = gen_select(is_positive, gen_fcmp_oge(val_1_flushed, gen_f32_cast(gen_i32_constant(0x4f000000))), gen_fcmp_ole(val_1_flushed, gen_f32_cast(gen_i32_constant(0xcf000000))));
  gen_cond_branch(is_saturated, saturation_branch, perform_calc_branch);
  
  // Handle invalid nan case
  set_gen_point(invalid_nan_branch);
  gen_store(gen_i32_constant(0x80000000), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle saturation case
  set_gen_point(saturation_branch);
  fastiss::i32_t* saturate_val = gen_select(is_positive, gen_i32_constant(0x7fffffff), gen_i32_constant(0x80000000));
  gen_store(saturate_val, res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);  

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Convert the operand to a 32-bit integer value
  fastiss::i32_with_exception_flags_t convert_result = gen_i32_signed_convert(val_1_flushed, round_mode);

  // Check if the inexact flag must be explicitly set
  convert_result.inexact = gen_inexact_denorm_update(convert_result.invalid, convert_result.divide_by_zero, convert_result.inexact, input_val_1);
 
  // Update the result, FXSR.XP, and handle exceptions
  gen_store(convert_result.result, res_val_ptr);
  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), convert_result.invalid, convert_result.divide_by_zero, convert_result.overflow, convert_result.underflow, convert_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_recip(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *qnan_branch = create_basic_block();
  fastiss::block_t *check_invalid_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *check_div_by_zero_branch = create_basic_block();
  fastiss::block_t *div_by_zero_branch = create_basic_block();
  fastiss::block_t *check_div_by_inf_branch = create_basic_block();
  fastiss::block_t *div_by_inf_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(is_unimplemented_fxu_exp(input_val_1), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
    
  // Check if the operand is a quiet NaN
  gen_cond_branch(gen_is_quiet_nan(input_val_1), qnan_branch, check_invalid_qnan_branch);

  // Check if the operand is a signalling NaN
  set_gen_point(check_invalid_qnan_branch);
  gen_cond_branch(gen_is_signalling_nan(input_val_1), invalid_qnan_branch, check_div_by_zero_branch);

  // Check if dividing by zero
  set_gen_point(check_div_by_zero_branch);
  gen_cond_branch(gen_is_zero(val_1_flushed), div_by_zero_branch, check_div_by_inf_branch);
 
  // Check if dividing by infinity
  set_gen_point(check_div_by_inf_branch);
  gen_cond_branch(gen_is_infinity(input_val_1), div_by_inf_branch, perform_calc_branch);

  // Handle quiet NaN case
  set_gen_point(qnan_branch);
  gen_store(gen_i32_cast(input_val_1), res_val_ptr);
  gen_branch(cont_block);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle divide by zero case
  set_gen_point(div_by_zero_branch);
  gen_store(gen_select(gen_is_positive(input_val_1), gen_i32_constant(0x7f800000), gen_i32_constant(0xff800000)), res_val_ptr);
  gen_store(gen_i8_constant(0x08), res_exp_ptr);
  gen_branch(cont_block);

  // Handle divide by infinity case
  set_gen_point(div_by_inf_branch);
  gen_store(gen_select(gen_is_positive(input_val_1), gen_i32_constant(0x00000000), gen_i32_constant(0x80000000)), res_val_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *dividend_one = gen_f80_ext(gen_f32_constant(1));
  fastiss::f80_t *val_1_float80 = gen_f80_ext(val_1_flushed);
  
  // Determine the DIV result - always round to nearest
  fastiss::f32_with_exception_flags_t div_result = gen_f32_div(dividend_one, val_1_float80, gen_i8_constant(0));
  
  // Always inexact exception occures, when reg2 is the normal value
  div_result.inexact = gen_i1_constant(1);

  // Check for denormal (underflow) results
  gen_underflow_denorm_update(div_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_and(div_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  div_result.result = gen_select
  (
    div_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(div_result.result), gen_i32_constant(0x80000000))),
    div_result.result
  );
 
  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  div_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(div_result.overflow, div_result.underflow, div_result.inexact);
       
  // Check if the inexact flag must be explicitly set
  div_result.inexact = gen_inexact_denorm_update(div_result.invalid, div_result.divide_by_zero, div_result.inexact, input_val_1);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_i32_cast(div_result.result), res_val_ptr);
  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), div_result.invalid, div_result.divide_by_zero, gen_false(), div_result.underflow, div_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_rsqrt(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *is_zero_branch = create_basic_block();  
  fastiss::block_t *check_is_invalid_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(is_unimplemented_fxu_exp(input_val_1), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  
  // Check if the operand is negative
  fastiss::i1_t* is_negative = gen_is_negative(input_val_1);

  // Branch to the divide by zero case if the operand is zero
  gen_cond_branch(gen_is_zero(val_1_flushed), is_zero_branch, check_is_invalid_qnan_branch);  
    
  // Check if the operand is a signalling NaN or a negative operand
  set_gen_point(check_is_invalid_qnan_branch);
  fastiss::i1_t* is_signalling_nan = gen_is_signalling_nan(input_val_1);
  fastiss::i1_t *is_negative_not_qnan = gen_and(is_negative, gen_not(gen_is_quiet_nan(input_val_1)));

  // Branch to the invalid quiet Nan case if the operand is a signalling NaN
  // or an invalid negative operand
  gen_cond_branch(gen_or(is_signalling_nan, is_negative_not_qnan), invalid_qnan_branch, perform_calc_branch);

  // Handle zero case
  set_gen_point(is_zero_branch);
  gen_store(gen_select(is_negative, gen_i32_constant(0xff800000), gen_i32_constant(0x7f800000)), res_val_ptr);
  gen_store(gen_i8_constant(0x08), res_exp_ptr);
  gen_branch(cont_block);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *dividend_one = gen_f80_ext(gen_f32_constant(1.0));
  fastiss::f80_t *val_1_float80 = gen_f80_ext(val_1_flushed);
  
  // Determine the SQRT result - always round to nearest
  fastiss::f80_with_exception_flags_t sqrt_result = gen_f80_sqrt(val_1_float80, gen_i8_constant(0));

  // Determine the RECIP result - always round to nearest
  fastiss::f32_with_exception_flags_t recip_result = gen_f32_div(dividend_one, sqrt_result.result, gen_i8_constant(0));

  // Always inexact exception occures, when reg2 is the normal value
  recip_result.inexact = gen_i1_constant(1);

  // Merge flags
  recip_result.invalid = gen_or(sqrt_result.invalid, recip_result.invalid);
  recip_result.divide_by_zero = gen_or(sqrt_result.divide_by_zero, recip_result.divide_by_zero);
  recip_result.overflow = gen_or(sqrt_result.overflow, recip_result.overflow);
  recip_result.underflow = gen_or(sqrt_result.underflow, recip_result.underflow);
  recip_result.inexact = gen_or(sqrt_result.inexact, recip_result.inexact);
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(sqrt_result);
  gen_underflow_denorm_update(recip_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  fastiss::i1_t* is_underflow = gen_or(sqrt_result.underflow, recip_result.underflow);
  gen_cond_branch(gen_and(is_underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  recip_result.result = gen_select
  (
    recip_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(recip_result.result), gen_i32_constant(0x80000000))),
    recip_result.result
  );

  // Merge the flags together for all of the operations and check if the inexact flag must be explicitly set
  recip_result.inexact = gen_inexact_denorm_update(recip_result.invalid, recip_result.divide_by_zero, recip_result.inexact, input_val_1);
 
  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_i32_cast(recip_result.result), res_val_ptr);
  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), recip_result.invalid, recip_result.divide_by_zero, gen_false(), gen_false(), recip_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_sqrt(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *is_negative_zero_or_negative_qnan_branch = create_basic_block();  
  fastiss::block_t *check_is_invalid_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(is_unimplemented_fxu_exp(input_val_1), unimplemented_exp_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  
  // Check if the operand is negative zero or negative quiet NaN
  fastiss::i1_t* is_negative = gen_is_negative(input_val_1);
  fastiss::i1_t* is_zero = gen_is_zero(val_1_flushed);
  fastiss::i1_t* is_quiet_nan = gen_is_quiet_nan(input_val_1);
  fastiss::i1_t* is_negative_zero_or_negative_qnan = gen_or(gen_and(is_negative, is_zero), gen_and(is_negative, is_quiet_nan));

  // Branch to the special case if the operand is negative zero or negative quiet NaN
  gen_cond_branch(is_negative_zero_or_negative_qnan, is_negative_zero_or_negative_qnan_branch, check_is_invalid_qnan_branch);  
    
  // Check if the operand is a signalling NaN or an invalid negative operand
  set_gen_point(check_is_invalid_qnan_branch);
  fastiss::i1_t* is_signalling_nan = gen_is_signalling_nan(input_val_1);

  // Branch to the invalid quiet Nan case if the operand is a signalling NaN
  // or an invalid negative operand
  gen_cond_branch(gen_or(is_signalling_nan, is_negative), invalid_qnan_branch, perform_calc_branch);

  // Handle negative zero or negative quiet NaN operand case
  set_gen_point(is_negative_zero_or_negative_qnan_branch);
  gen_store(gen_i32_cast(val_1_flushed), res_val_ptr);
  gen_store(gen_i8_zext(gen_is_denorm(input_val_1)), res_exp_ptr);
  gen_branch(cont_block);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val_1_float80 = gen_f80_ext(val_1_flushed);
  
  // Determine the SQRT result - always round to nearest
  fastiss::f32_with_exception_flags_t sqrt_result = gen_f32_sqrt(val_1_float80, gen_read_round_mode());

  // Check for denormal (underflow) results
  gen_underflow_denorm_update(sqrt_result);
  sqrt_result.result = gen_select
  (
    sqrt_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(sqrt_result.result), gen_i32_constant(0x80000000))),
    sqrt_result.result
  );

  // Check if the inexact flag must be explicitly set
  sqrt_result.inexact = gen_inexact_denorm_update(sqrt_result.invalid, sqrt_result.divide_by_zero, sqrt_result.inexact, input_val_1);
 
  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_i32_cast(sqrt_result.result), res_val_ptr);
  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), sqrt_result.invalid, sqrt_result.divide_by_zero, sqrt_result.overflow, sqrt_result.underflow, sqrt_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);  
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_cmp(fastiss::i8_t *fcond, fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *nan_branch = create_basic_block();
  fastiss::block_t *no_nan_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  // Determine if either operand is a NaN
  fastiss::i1_t *is_val_1_nan = gen_is_nan(input_val_1);
  fastiss::i1_t *is_val_2_nan = gen_is_nan(input_val_2);

  fastiss::i1_ptr_t *cmp_result = gen_i1_alloc();

  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_exp, res_exp_ptr);

  // Branch to the NaN case if either operand is a NaN
  gen_cond_branch(gen_or(is_val_1_nan, is_val_2_nan), nan_branch, no_nan_branch);

  // Handle NaN case
  set_gen_point(nan_branch);

  // Determine the comparison result
  gen_store(gen_i1_extract(fcond, gen_i8_constant(0)), cmp_result);

  // Determine the value of the invalid bit and check for exceptions. Note that
  // unlike other FPU instructions there is no result to write back to a general
  // purpose register. To allow the same custom operation to be used a value of
  // 0 is written back to the r0 register which is always zero anyhow.
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2));
  fastiss::i1_t *invalid_bit = gen_i1_extract(fcond, gen_i8_constant(3));
  gen_store(gen_select(gen_or(is_either_signalling_nan, invalid_bit), gen_i8_constant(0x10), gen_i8_constant(0x00)), res_exp_ptr);

  gen_branch(cont_block);

  // Handle no NaN case
  set_gen_point(no_nan_branch);
  
  // Determine the comparison result - use ordered comparison operations
  fastiss::i1_t *is_less_than = gen_fcmp_olt(input_val_1, input_val_2);
  fastiss::i1_t *is_equal = gen_fcmp_oeq(input_val_1, input_val_2);
  
  gen_store(gen_or(gen_and(gen_i1_extract(fcond, gen_i8_constant(2)), is_less_than), gen_and(gen_i1_extract(fcond, gen_i8_constant(1)), is_equal)), cmp_result);
  gen_branch(cont_block);  

  // Handle continue case
  set_gen_point(cont_block);

  // Update the result
  fastiss::i1_t *result = gen_load(cmp_result);
  *res_val = gen_select(result, gen_i32_constant(0xffffffff), gen_i32_constant(0));
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_fma(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  fastiss::f32_t *input_val_3 = gen_f32_cast(gen_load(res_val_ptr));

  // Check if the operand is a denormaled value and FXSR.FS=0
  fastiss::i1_t *some_unimplemented_exp = gen_or(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), is_unimplemented_fxu_exp(input_val_3));
  gen_cond_branch(some_unimplemented_exp, unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  fastiss::f32_t *val_3_flushed = gen_flush_denorm_fxu(input_val_3);

  // Test a number of conditions to see if an invalid quiet NaN case will result
  fastiss::i1_t* is_any_signalling_nan = gen_or(gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2)), gen_is_signalling_nan(input_val_3));
  fastiss::i1_t* is_val1_infinity = gen_is_infinity(input_val_1);
  fastiss::i1_t* is_val2_infinity = gen_is_infinity(input_val_2);
  fastiss::i1_t* is_val3_infinity = gen_is_infinity(input_val_3);
  fastiss::i1_t* is_val1_positive = gen_is_positive(input_val_1);
  fastiss::i1_t* is_val2_positive = gen_is_positive(input_val_2);
  fastiss::i1_t* is_val3_positive = gen_is_positive(input_val_3);
  fastiss::i1_t* is_val1_zero = gen_is_zero(val_1_flushed);
  fastiss::i1_t* is_val2_zero = gen_is_zero(val_2_flushed);
  fastiss::i1_t* is_val1_not_qnan = gen_not(gen_is_quiet_nan(input_val_1));
  fastiss::i1_t* is_val2_not_qnan = gen_not(gen_is_quiet_nan(input_val_2));
  fastiss::i1_t* is_val3_not_qnan = gen_not(gen_is_quiet_nan(input_val_3));
  
  fastiss::i1_t* is_mul_result_invalid = gen_and(is_val3_not_qnan, gen_or(gen_and(is_val2_zero, is_val1_infinity), gen_and(is_val2_infinity, is_val1_zero)));
  
  fastiss::i1_t* is_val1_or_val2_infinity = gen_or(gen_and(is_val2_infinity, is_val1_not_qnan), gen_and(is_val1_infinity, is_val2_not_qnan));
  fastiss::i1_t* is_mul_result_positive = gen_icmp_eq(is_val1_positive, is_val2_positive);
  fastiss::i1_t* is_mul_result_positive_infinity = gen_and(is_val1_or_val2_infinity, is_mul_result_positive);
  fastiss::i1_t* is_mul_result_negative_infinity = gen_and(is_val1_or_val2_infinity, gen_not(is_mul_result_positive));
  fastiss::i1_t* is_val3_positive_infinity = gen_and(is_val3_positive, is_val3_infinity);
  fastiss::i1_t* is_val3_negative_infinity = gen_and(gen_not(is_val3_positive), is_val3_infinity);
  fastiss::i1_t* is_add_result_invalid = gen_or(gen_and(is_val3_positive_infinity, is_mul_result_negative_infinity), gen_and(is_val3_negative_infinity, is_mul_result_positive_infinity));

  // Branch to the invalid quiet Nan case if any operand is a signalling NaN
  // or if the mul or add results are invalid
  gen_cond_branch(gen_or(is_any_signalling_nan, gen_or(is_mul_result_invalid, is_add_result_invalid)), invalid_qnan_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val2_float80 = gen_f80_ext(val_2_flushed);
  fastiss::f80_t *val3_float80 = gen_f80_ext(val_3_flushed);
  
  // Determine the MUL result - always round to nearest
  fastiss::f80_with_exception_flags_t mul_result = gen_f80_mul(val1_float80, val2_float80, gen_read_round_mode());

  // Check for results, if (zero * infinity) + QNaN, result is not ivalid.
  mul_result.invalid = gen_select
  (
    is_mul_result_invalid,
    mul_result.invalid,
    is_mul_result_invalid
  );
 
  // Determine the ADD result - always round to nearest
  fastiss::f32_with_exception_flags_t add_result = gen_f32_add(val3_float80, mul_result.result, gen_read_round_mode());

  // Merge flags
  add_result.invalid = gen_or(mul_result.invalid, add_result.invalid);
  add_result.divide_by_zero = gen_or(mul_result.divide_by_zero, add_result.divide_by_zero);
  add_result.overflow = gen_or(mul_result.overflow,  add_result.overflow);
  add_result.underflow = gen_or(mul_result.underflow, add_result.underflow);
  add_result.inexact = gen_or(mul_result.inexact, add_result.inexact);
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(add_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_and(add_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  add_result.result = gen_select
  (
    add_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(add_result.result), gen_i32_constant(0x80000000))),
    add_result.result
  );

  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  add_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(add_result.overflow, add_result.underflow, add_result.inexact);
    
  // Check if the inexact flag must be explicitly set
  add_result.inexact = gen_inexact_denorm_update(add_result.invalid, add_result.divide_by_zero, add_result.inexact, input_val_1, input_val_2, input_val_3);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1), 
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_select(gen_is_quiet_nan(input_val_3),
					     gen_i32_cast(input_val_3),
					     gen_i32_cast(add_result.result)))),
	    res_val_ptr);
	    
  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), add_result.invalid, add_result.divide_by_zero, add_result.overflow, add_result.underflow, add_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_fms(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();


  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  fastiss::f32_t *input_val_3 = gen_f32_cast(gen_load(res_val_ptr));

  // Check if the operand is a denormaled value and FXSR.FS=0
  fastiss::i1_t *some_unimplemented_exp = gen_or(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), is_unimplemented_fxu_exp(input_val_3));
  gen_cond_branch(some_unimplemented_exp, unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  fastiss::f32_t *val_3_flushed = gen_flush_denorm_fxu(input_val_3);

  // Test a number of conditions to see if an invalid quiet NaN case will result
  fastiss::i1_t* is_any_signalling_nan = gen_or(gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2)), gen_is_signalling_nan(input_val_3));
  fastiss::i1_t* is_val1_infinity = gen_is_infinity(input_val_1);
  fastiss::i1_t* is_val2_infinity = gen_is_infinity(input_val_2);
  fastiss::i1_t* is_val3_infinity = gen_is_infinity(input_val_3);
  fastiss::i1_t* is_val1_positive = gen_is_positive(input_val_1);
  fastiss::i1_t* is_val2_positive = gen_is_positive(input_val_2);
  fastiss::i1_t* is_val3_positive = gen_is_positive(input_val_3);
  fastiss::i1_t* is_val1_zero = gen_is_zero(val_1_flushed);
  fastiss::i1_t* is_val2_zero = gen_is_zero(val_2_flushed);
  fastiss::i1_t* is_val1_not_qnan = gen_not(gen_is_quiet_nan(input_val_1));
  fastiss::i1_t* is_val2_not_qnan = gen_not(gen_is_quiet_nan(input_val_2));
  fastiss::i1_t* is_val3_not_qnan = gen_not(gen_is_quiet_nan(input_val_3));
  
  fastiss::i1_t* is_mul_result_invalid = gen_and(is_val3_not_qnan, gen_or(gen_and(is_val2_zero, is_val1_infinity), gen_and(is_val2_infinity, is_val1_zero)));
  
  fastiss::i1_t* is_val1_or_val2_infinity = gen_or(gen_and(is_val2_infinity, is_val1_not_qnan), gen_and(is_val1_infinity, is_val2_not_qnan));
  fastiss::i1_t* is_mul_result_positive = gen_icmp_eq(is_val1_positive, is_val2_positive);
  fastiss::i1_t* is_mul_result_positive_infinity = gen_and(is_val1_or_val2_infinity, is_mul_result_positive);
  fastiss::i1_t* is_mul_result_negative_infinity = gen_and(is_val1_or_val2_infinity, gen_not(is_mul_result_positive));
  fastiss::i1_t* is_val3_positive_infinity = gen_and(is_val3_positive, is_val3_infinity);
  fastiss::i1_t* is_val3_negative_infinity = gen_and(gen_not(is_val3_positive), is_val3_infinity);
  fastiss::i1_t* is_add_result_invalid = gen_or(gen_and(is_val3_positive_infinity, is_mul_result_positive_infinity), gen_and(is_val3_negative_infinity, is_mul_result_negative_infinity));

  // Branch to the invalid quiet Nan case if any operand is a signalling NaN
  // or if the mul or add results are invalid
  gen_cond_branch(gen_or(is_any_signalling_nan, gen_or(is_mul_result_invalid, is_add_result_invalid)), invalid_qnan_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val2_float80 = gen_f80_ext(val_2_flushed);
  fastiss::f80_t *val3_float80 = gen_f80_ext(val_3_flushed);
  
  // Determine the MUL result - always round to nearest
  fastiss::f80_with_exception_flags_t mul_result = gen_f80_mul(val1_float80, val2_float80, gen_read_round_mode());

  // Check for results, if (zero * infinity) + QNaN, result is not ivalid.
  mul_result.invalid = gen_select
  (
    is_mul_result_invalid,
    mul_result.invalid,
    is_mul_result_invalid
  );
 
  // Determine the ADD result - always round to nearest
  fastiss::f32_with_exception_flags_t sub_result = gen_f32_sub(mul_result.result, val3_float80, gen_read_round_mode());
  
  // Merge flags
  sub_result.invalid = gen_or(mul_result.invalid, sub_result.invalid);
  sub_result.divide_by_zero = gen_or(mul_result.divide_by_zero, sub_result.divide_by_zero);
  sub_result.overflow = gen_or(mul_result.overflow,  sub_result.overflow);
  sub_result.underflow = gen_or(mul_result.underflow, sub_result.underflow);
  sub_result.inexact = gen_or(mul_result.inexact, sub_result.inexact);
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(sub_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  gen_cond_branch(gen_and(sub_result.underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  sub_result.result = gen_select
  (
    sub_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(sub_result.result), gen_i32_constant(0x80000000))),
    sub_result.result
  );

  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  sub_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(sub_result.overflow, sub_result.underflow, sub_result.inexact);
    
  // Check if the inexact flag must be explicitly set
  sub_result.inexact = gen_inexact_denorm_update(sub_result.invalid, sub_result.divide_by_zero, sub_result.inexact, input_val_1, input_val_2, input_val_3);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1), 
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_select(gen_is_quiet_nan(input_val_3),
					     gen_i32_cast(input_val_3),
					     gen_i32_cast(sub_result.result)))),
	    res_val_ptr);

  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), sub_result.invalid, sub_result.divide_by_zero, sub_result.overflow, sub_result.underflow, sub_result.inexact), res_exp_ptr);
  gen_branch(cont_block);  

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_fnma(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  fastiss::f32_t *input_val_3 = gen_f32_cast(gen_load(res_val_ptr));

  // Check if the operand is a denormaled value and FXSR.FS=0
  fastiss::i1_t *some_unimplemented_exp = gen_or(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), is_unimplemented_fxu_exp(input_val_3));
  gen_cond_branch(some_unimplemented_exp, unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  fastiss::f32_t *val_3_flushed = gen_flush_denorm_fxu(input_val_3);

  // Test a number of conditions to see if an invalid quiet NaN case will result
  fastiss::i1_t* is_any_signalling_nan = gen_or(gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2)), gen_is_signalling_nan(input_val_3));
  fastiss::i1_t* is_val1_infinity = gen_is_infinity(input_val_1);
  fastiss::i1_t* is_val2_infinity = gen_is_infinity(input_val_2);
  fastiss::i1_t* is_val3_infinity = gen_is_infinity(input_val_3);
  fastiss::i1_t* is_val1_positive = gen_is_positive(input_val_1);
  fastiss::i1_t* is_val2_positive = gen_is_positive(input_val_2);
  fastiss::i1_t* is_val3_positive = gen_is_positive(input_val_3);
  fastiss::i1_t* is_val1_zero = gen_is_zero(val_1_flushed);
  fastiss::i1_t* is_val2_zero = gen_is_zero(val_2_flushed);
  fastiss::i1_t* is_val1_not_qnan = gen_not(gen_is_quiet_nan(input_val_1));
  fastiss::i1_t* is_val2_not_qnan = gen_not(gen_is_quiet_nan(input_val_2));
  fastiss::i1_t* is_val3_not_qnan = gen_not(gen_is_quiet_nan(input_val_3));
  
  fastiss::i1_t* is_mul_result_invalid = gen_and(is_val3_not_qnan, gen_or(gen_and(is_val2_zero, is_val1_infinity), gen_and(is_val2_infinity, is_val1_zero)));
  
  fastiss::i1_t* is_val1_or_val2_infinity = gen_and(is_val3_not_qnan, gen_or(gen_and(is_val2_infinity, is_val1_not_qnan), gen_and(is_val1_infinity, is_val2_not_qnan)));
  fastiss::i1_t* is_mul_result_positive = gen_icmp_eq(is_val1_positive, is_val2_positive);
  fastiss::i1_t* is_mul_result_positive_infinity = gen_and(is_val1_or_val2_infinity, is_mul_result_positive);
  fastiss::i1_t* is_mul_result_negative_infinity = gen_and(is_val1_or_val2_infinity, gen_not(is_mul_result_positive));
  fastiss::i1_t* is_val3_positive_infinity = gen_and(is_val3_positive, is_val3_infinity);
  fastiss::i1_t* is_val3_negative_infinity = gen_and(gen_not(is_val3_positive), is_val3_infinity);
  fastiss::i1_t* is_add_result_invalid = gen_or(gen_and(is_val3_positive_infinity, is_mul_result_negative_infinity), gen_and(is_val3_negative_infinity, is_mul_result_positive_infinity));

  // Branch to the invalid quiet Nan case if any operand is a signalling NaN
  // or if the mul or add results are invalid
  gen_cond_branch(gen_or(is_any_signalling_nan, gen_or(is_mul_result_invalid, is_add_result_invalid)), invalid_qnan_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val2_float80 = gen_f80_ext(val_2_flushed);
  fastiss::f80_t *val3_float80 = gen_f80_ext(val_3_flushed);
  
  // Determine the MUL result - always round to nearest
  fastiss::f80_with_exception_flags_t mul_result = gen_f80_mul(val1_float80, val2_float80, gen_read_round_mode());

  // Determine the ADD result - always round to nearest
  fastiss::f80_with_exception_flags_t add_result = gen_f80_add(val3_float80, mul_result.result, gen_read_round_mode());

  // Check for results, if (zero * infinity) + QNaN, result is not ivalid.
  mul_result.invalid = gen_select
  (
    is_mul_result_invalid,
    mul_result.invalid,
    is_mul_result_invalid
  );
 
  // Invert the sign of the multiply/add result
  add_result.result = gen_neg(add_result.result);
  
  // Truncate the result to 32-bits - always round to nearest
  fastiss::f32_with_exception_flags_t trunc_result = gen_f32_trunc(add_result.result, gen_read_round_mode());  
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(mul_result);
  gen_underflow_denorm_update(add_result);
  gen_underflow_denorm_update(trunc_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  fastiss::i1_t* is_underflow = gen_or(gen_or(mul_result.underflow, add_result.underflow), trunc_result.underflow);
  gen_cond_branch(gen_and(is_underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  trunc_result.result = gen_select
  (
    gen_or(gen_or(mul_result.underflow, add_result.underflow), trunc_result.underflow),
    gen_f32_cast(gen_and(gen_i32_cast(trunc_result.result), gen_i32_constant(0x80000000))),
    trunc_result.result
  );

  // Merge the flags together for all of the operations and check if the inexact flag must be explicitly set
  mul_result.invalid = gen_or(gen_or(mul_result.invalid, add_result.invalid), trunc_result.invalid);
  mul_result.inexact = gen_inexact_denorm_update(mul_result.invalid, mul_result.divide_by_zero, mul_result.inexact, input_val_1, input_val_2, input_val_3);

  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  trunc_result.invalid = gen_or(gen_or(mul_result.invalid, add_result.invalid), trunc_result.invalid);
  trunc_result.divide_by_zero = gen_or(gen_or(mul_result.divide_by_zero, add_result.divide_by_zero), trunc_result.divide_by_zero);
  trunc_result.overflow = gen_or(gen_or(mul_result.overflow,  add_result.overflow), trunc_result.overflow);
  trunc_result.underflow = gen_or(gen_or(mul_result.underflow, add_result.underflow), trunc_result.underflow);
  trunc_result.inexact = gen_or(gen_or(mul_result.inexact, add_result.inexact), trunc_result.inexact);
  trunc_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(trunc_result.overflow, trunc_result.underflow, trunc_result.inexact);
     
  trunc_result.inexact = gen_inexact_denorm_update(trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.inexact, input_val_1, input_val_2, input_val_3);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1), 
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_select(gen_is_quiet_nan(input_val_3),
					     gen_i32_cast(input_val_3),
					     gen_i32_cast(trunc_result.result)))),
	    res_val_ptr);

  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.overflow, trunc_result.underflow, trunc_result.inexact), res_exp_ptr);
  gen_branch(cont_block);

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

void rh850g4::custom_impl_t::gen_fp_simd_fnms(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp)
{
  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *cont_block = create_basic_block();

  fastiss::i32_ptr_t* res_val_ptr = gen_i32_alloc();
  fastiss::i8_ptr_t* res_exp_ptr = gen_i8_alloc();
  gen_store(*res_val, res_val_ptr);
  gen_store(*res_exp, res_exp_ptr);

  fastiss::f32_t *input_val_3 = gen_f32_cast(gen_load(res_val_ptr));

  // Check if the operand is a denormaled value and FXSR.FS=0
  fastiss::i1_t *some_unimplemented_exp = gen_or(gen_or(is_unimplemented_fxu_exp(input_val_1), is_unimplemented_fxu_exp(input_val_2)), is_unimplemented_fxu_exp(input_val_3));
  gen_cond_branch(some_unimplemented_exp, unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *val_1_flushed = gen_flush_denorm_fxu(input_val_1);
  fastiss::f32_t *val_2_flushed = gen_flush_denorm_fxu(input_val_2);
  fastiss::f32_t *val_3_flushed = gen_flush_denorm_fxu(input_val_3);

  // Test a number of conditions to see if an invalid quiet NaN case will result
  fastiss::i1_t* is_any_signalling_nan = gen_or(gen_or(gen_is_signalling_nan(input_val_1), gen_is_signalling_nan(input_val_2)), gen_is_signalling_nan(input_val_3));
  fastiss::i1_t* is_val1_infinity = gen_is_infinity(input_val_1);
  fastiss::i1_t* is_val2_infinity = gen_is_infinity(input_val_2);
  fastiss::i1_t* is_val3_infinity = gen_is_infinity(input_val_3);
  fastiss::i1_t* is_val1_positive = gen_is_positive(input_val_1);
  fastiss::i1_t* is_val2_positive = gen_is_positive(input_val_2);
  fastiss::i1_t* is_val3_positive = gen_is_positive(input_val_3);
  fastiss::i1_t* is_val1_zero = gen_is_zero(val_1_flushed);
  fastiss::i1_t* is_val2_zero = gen_is_zero(val_2_flushed);
  fastiss::i1_t* is_val1_not_qnan = gen_not(gen_is_quiet_nan(input_val_1));
  fastiss::i1_t* is_val2_not_qnan = gen_not(gen_is_quiet_nan(input_val_2));
  fastiss::i1_t* is_val3_not_qnan = gen_not(gen_is_quiet_nan(input_val_3));

  fastiss::i1_t* is_mul_result_invalid = gen_and(is_val3_not_qnan, gen_or(gen_and(is_val2_zero, is_val1_infinity), gen_and(is_val2_infinity, is_val1_zero)));
  
  fastiss::i1_t* is_val1_or_val2_infinity = gen_and(is_val3_not_qnan, gen_or(gen_and(is_val2_infinity, is_val1_not_qnan), gen_and(is_val1_infinity, is_val2_not_qnan)));
  fastiss::i1_t* is_mul_result_positive = gen_icmp_eq(is_val1_positive, is_val2_positive);
  fastiss::i1_t* is_mul_result_positive_infinity = gen_and(is_val1_or_val2_infinity, is_mul_result_positive);
  fastiss::i1_t* is_mul_result_negative_infinity = gen_and(is_val1_or_val2_infinity, gen_not(is_mul_result_positive));
  fastiss::i1_t* is_val3_positive_infinity = gen_and(is_val3_positive, is_val3_infinity);
  fastiss::i1_t* is_val3_negative_infinity = gen_and(gen_not(is_val3_positive), is_val3_infinity);
  fastiss::i1_t* is_add_result_invalid = gen_or(gen_and(is_val3_positive_infinity, is_mul_result_positive_infinity), gen_and(is_val3_negative_infinity, is_mul_result_negative_infinity));

  // Branch to the invalid quiet Nan case if any operand is a signalling NaN
  // or if the mul or add results are invalid
  gen_cond_branch(gen_or(is_any_signalling_nan, gen_or(is_mul_result_invalid, is_add_result_invalid)), invalid_qnan_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_store(gen_i32_constant(0x7fffffff), res_val_ptr);
  gen_store(gen_i8_constant(0x10), res_exp_ptr);
  gen_branch(cont_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *val1_float80 = gen_f80_ext(val_1_flushed);
  fastiss::f80_t *val2_float80 = gen_f80_ext(val_2_flushed);
  fastiss::f80_t *val3_float80 = gen_f80_ext(val_3_flushed);
  
  // Determine the MUL result - always round to nearest
  fastiss::f80_with_exception_flags_t mul_result = gen_f80_mul(val1_float80, val2_float80, gen_read_round_mode());

  // Determine the ADD result - always round to nearest
  fastiss::f80_with_exception_flags_t sub_result = gen_f80_sub(mul_result.result, val3_float80, gen_read_round_mode());
  
  // Check for results, if (zero * infinity) + QNaN, result is not ivalid.
  mul_result.invalid = gen_select
  (
    is_mul_result_invalid,
    mul_result.invalid,
    is_mul_result_invalid
  );
 
  // Invert the sign of the multiply/add result
  sub_result.result = gen_neg(sub_result.result);
  
  // Truncate the result to 32-bits - always round to nearest
  fastiss::f32_with_exception_flags_t trunc_result = gen_f32_trunc(sub_result.result, gen_read_round_mode());  
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(mul_result);
  gen_underflow_denorm_update(sub_result);
  gen_underflow_denorm_update(trunc_result);

  // Check if the operand is a denormaled value and FXSR.FS=0
  fastiss::i1_t* is_underflow = gen_or(gen_or(mul_result.underflow, sub_result.underflow), trunc_result.underflow);
  gen_cond_branch(gen_and(is_underflow, gen_icmp_eq(gen_read_FXSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_store(gen_i8_constant(0x20), res_exp_ptr);
  gen_branch(cont_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  trunc_result.result = gen_select
  (
    gen_or(gen_or(mul_result.underflow, sub_result.underflow), trunc_result.underflow),
    gen_f32_cast(gen_and(gen_i32_cast(trunc_result.result), gen_i32_constant(0x80000000))),
    trunc_result.result
  );

  // Merge the flags together for all of the operations and check if the inexact flag must be explicitly set
  mul_result.invalid = gen_or(gen_or(mul_result.invalid, sub_result.invalid), trunc_result.invalid);
  mul_result.inexact = gen_inexact_denorm_update(mul_result.invalid, mul_result.divide_by_zero, mul_result.inexact, input_val_1, input_val_2, input_val_3);

  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  trunc_result.invalid = gen_or(gen_or(mul_result.invalid, sub_result.invalid), trunc_result.invalid);
  trunc_result.divide_by_zero = gen_or(gen_or(mul_result.divide_by_zero, sub_result.divide_by_zero), trunc_result.divide_by_zero);
  trunc_result.overflow = gen_or(gen_or(mul_result.overflow,  sub_result.overflow), trunc_result.overflow);
  trunc_result.underflow = gen_or(gen_or(mul_result.underflow, sub_result.underflow), trunc_result.underflow);
  trunc_result.inexact = gen_or(gen_or(mul_result.inexact, sub_result.inexact), trunc_result.inexact);
  trunc_result.inexact = gen_inexact_enabled_overflow_underflow_update_fxu(trunc_result.overflow, trunc_result.underflow, trunc_result.inexact);
     
  trunc_result.inexact = gen_inexact_denorm_update(trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.inexact, input_val_1, input_val_2, input_val_3);

  // Update the result, FXSR.XP, and handle exceptions
  gen_store(gen_select(gen_is_quiet_nan(input_val_1), 
		       gen_i32_cast(input_val_1),
		       gen_select(gen_is_quiet_nan(input_val_2),
				  gen_i32_cast(input_val_2),
				  gen_select(gen_is_quiet_nan(input_val_3),
					     gen_i32_cast(input_val_3),
					     gen_i32_cast(trunc_result.result)))),
	    res_val_ptr);

  gen_store(gen_i8_compose(gen_false(), gen_false(), gen_false(), trunc_result.invalid, trunc_result.divide_by_zero, trunc_result.overflow, trunc_result.underflow, trunc_result.inexact), res_exp_ptr);
  gen_branch(cont_block);

  // Continue block
  set_gen_point(cont_block);
  *res_val = gen_load(res_val_ptr);
  *res_exp = gen_load(res_exp_ptr);
}

fastiss::i1_t *rh850g4::custom_impl_t::is_host_mode(void)
{
  // If PSWH_GM is 1 (guest mode), return false. 
  // If PSWH_GM is 0 (host mode), return true.
  return gen_select(gen_and(gen_read_PSWH_GM(), gen_i1_constant(1)), gen_i1_constant(0), gen_i1_constant(1));
}

fastiss::i1_t *rh850g4::custom_impl_t::is_supervizer_mode(void)
{
  // If PSW_UM is 1 (user mode), return false. 
  // If PSW_UM is 0 (supervizer mode), return true.
  return gen_select(gen_and(gen_read_PSW_UM(), gen_i1_constant(1)), gen_i1_constant(0), gen_i1_constant(1));
}

fastiss::i1_t *rh850g4::custom_impl_t::is_valid_virtualization(void)
{
  // If HVCFG.HVE is 1 (Hardware-assisted Virtualization is valid), return true.
  // If HVCFG.HVE is 0 (Hardware-assisted Virtualization is invalid), return false.
  return gen_select(gen_and(gen_read_HVCFG_HVE(), gen_i1_constant(1)), gen_i1_constant(1), gen_i1_constant(0));
}

void rh850g4::custom_impl_t::gen_pie_rie_exept(fastiss::i32_t* curr_pc, fastiss::i32_t* base_addr, fastiss::i32_t* exept_code, fastiss::i1_t* current_pswh_gm)
{
  // Set the FEPC to current PC
  gen_write_FEPC(curr_pc);

  // Set the FEPSW
  gen_write_FEPSW(gen_read_PSW());
  
  // Set the exception code
  gen_write_FEIC(exept_code);
  
  // Set the PSWH
  gen_write_FEPSWH_GM(current_pswh_gm);
  gen_write_FEPSWH_GPID(gen_read_PSWH_GPID());
  
  // Set the EP, ID, and NP flags
  gen_write_PSW_EP(gen_i1_constant(1));
  gen_write_PSW_ID(gen_i1_constant(1));
  gen_write_PSW_NP(gen_i1_constant(1));

  // clear the UM flags
  gen_write_PSW_UM(gen_i1_constant(0));

  // Update PC
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), exept_code));

  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }

  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();

  // Branch to the exit block
  gen_branch(get_exit_block());
}

fastiss::i8_ptr_t *rh850g4::custom_impl_t::gen_read_link_manager_ptr(void)
{
  return gen_i8_constant_ptr(m_link_manager);
}

bool rh850g4::custom_impl_t::is_link_func_enable(void)
{
  return (m_link_func_enable);
}

bool rh850g4::custom_impl_t::is_hv_func_support(void)
{
  return (m_hv_func_support);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
