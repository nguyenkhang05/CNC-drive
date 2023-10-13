/*************************************************************************
 *
 * Copyright(c) 2019 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

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

#include <assert.h>
#include "rh850/custom_impl.h"

#include <stdio.h>
#include <iostream>

//------------------------------------------------------------------------------
// rh850 custom extension implementation
//------------------------------------------------------------------------------


rh850::custom_impl_t::custom_impl_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850::memory_if_t *memory_impl,
  rh850::cpu_state_if_t *cpu_state_impl,
  rh850::analysis_point_if* interrupt_accepted_analysis_point,
  rh850::analysis_point_if* interrupt_completed_analysis_point,
  void *pending_fpu_imprecise_exception_flag,
  void *link_manager,
  bool link_func_enable,
  unsigned int cpu_milestone
) :
  fastiss::code_gen_bridge_t(code_gen_impl),
  rh850::memory_bridge_t(memory_impl),
  rh850::cpu_state_bridge_t(cpu_state_impl),
  m_exit_block(NULL),
  m_interrupt_accepted_analysis_point(interrupt_accepted_analysis_point),
  m_interrupt_completed_analysis_point(interrupt_completed_analysis_point),
  m_pending_fpu_imprecise_exception_flag(pending_fpu_imprecise_exception_flag),
  m_link_manager(link_manager),
  m_link_func_enable(link_func_enable),
  m_cpu_milestone(cpu_milestone),
  m_mdp_enable(false)
{
}

rh850::custom_impl_t::~custom_impl_t()
{
}

void rh850::custom_impl_t::create_exit_block(void)
{
  if (m_exit_block != NULL)
  {
    gen_branch(m_exit_block);
    set_gen_point(m_exit_block);
    m_exit_block = NULL;
  }
}

fastiss::block_t *rh850::custom_impl_t::get_exit_block(void)
{
  if (m_exit_block == NULL)
  {
    m_exit_block = create_basic_block();
  }
  return m_exit_block;
}

void rh850::custom_impl_t::gen_increment_cycle_count(fastiss::i32_t *increment)
{
  gen_increment_time(gen_mul(gen_read_cycle_period(), gen_i64_zext(increment)));
}

void rh850::custom_impl_t::notify_interrupt_accepted(void)
{
  if (m_interrupt_accepted_analysis_point->has_attachments())
  {
    call_helper("activate_interrupt_accepted_analysis_point_helper", gen_read_core_ptr(), NULL);  
  }
}

void rh850::custom_impl_t::notify_interrupt_complete(void)
{
  if (m_interrupt_completed_analysis_point->has_attachments())
  {
    m_interrupt_completed_analysis_point->activate();
  }

}

bool rh850::custom_impl_t::is_condition_always(unsigned char condition)
{
  return (condition == T_COND_CODE);
}

fastiss::i1_t *rh850::custom_impl_t::gen_calculate_PSW_S(fastiss::i32_t *op_result)
{ 
  // Determine the S flag value
  fastiss::i1_t *is_result_negative = gen_icmp_slt(op_result, gen_i32_constant(0));
  return is_result_negative;
}

fastiss::i1_t *rh850::custom_impl_t::gen_calculate_PSW_Z(fastiss::i32_t *op_result)
{
  // Determine the Z flag value
  fastiss::i1_t *is_result_zero = gen_icmp_eq(op_result, gen_i32_constant(0));
  return is_result_zero;
}

fastiss::i1_t *rh850::custom_impl_t::gen_calculate_PSW_CY_shift(fastiss::i32_t *val, fastiss::i32_t *shift, bool is_left_shift)
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

fastiss::i1_t *rh850::custom_impl_t::gen_condition_satisfied(unsigned char condition)
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

void rh850::custom_impl_t::gen_div_reg1_reg2(fastiss::i32_t *reg1_val, rh850::GR_index_t reg2)
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
void rh850::custom_impl_t::gen_div_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850::GR_index_t reg2, rh850::GR_index_t reg3)
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


void rh850::custom_impl_t::gen_divu_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850::GR_index_t reg2, rh850::GR_index_t reg3, bool is_halfword)
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

fastiss::i1_t *rh850::custom_impl_t::is_unimplemented_exp(fastiss::f32_t *value)
{
  return gen_and(gen_is_denorm(value), gen_icmp_eq(gen_read_FPSR_FS(), gen_i1_constant(0)));
}

fastiss::i1_t *rh850::custom_impl_t::is_unimplemented_exp(fastiss::f64_t *value)
{
  return gen_and(gen_is_denorm(value), gen_icmp_eq(gen_read_FPSR_FS(), gen_i1_constant(0)));
}

fastiss::f32_t *rh850::custom_impl_t::gen_flush_denorm(fastiss::f32_t *value)
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

fastiss::f64_t *rh850::custom_impl_t::gen_flush_denorm(fastiss::f64_t *value)
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

void rh850::custom_impl_t::gen_underflow_denorm_update(fastiss::f32_with_exception_flags_t& op_result)
{
  fastiss::i1_t *is_denormal = gen_is_denorm(op_result.result);
  op_result.underflow = gen_or(op_result.underflow, is_denormal);
  op_result.inexact = gen_or(op_result.inexact, is_denormal);
}

void rh850::custom_impl_t::gen_underflow_denorm_update(fastiss::f64_with_exception_flags_t& op_result)
{
  fastiss::i1_t *is_denormal = gen_is_denorm(op_result.result);
  op_result.underflow = gen_or(op_result.underflow, is_denormal);
  op_result.inexact = gen_or(op_result.inexact, is_denormal);
}

void rh850::custom_impl_t::gen_underflow_denorm_update(fastiss::f80_with_exception_flags_t& op_result)
{
  fastiss::i1_t *is_denormal = gen_is_denorm(op_result.result);
  op_result.underflow = gen_or(op_result.underflow, is_denormal);
  op_result.inexact = gen_or(op_result.inexact, is_denormal);
}

fastiss::i1_t *rh850::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg2)
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

fastiss::i1_t *rh850::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg2_pair)
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

fastiss::i1_t *rh850::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2)
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

fastiss::i1_t *rh850::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg1_pair, fastiss::f64_t *reg2_pair)
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

fastiss::i1_t *rh850::custom_impl_t::gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2, fastiss::f32_t *reg3)
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

fastiss::i1_t *rh850::custom_impl_t::gen_inexact_enabled_overflow_underflow_update(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact)
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

void rh850::custom_impl_t::gen_update_dest_and_fpsr(bool is_single_precision, rh850::GR_index_t dest_index, fastiss::i32_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block)
{
  // Create sub-blocks
  fastiss::block_t *exec_inst_branch = create_basic_block();
  fastiss::block_t *exception_branch = create_basic_block();
  fastiss::block_t *no_exception_branch = create_basic_block();
  fastiss::block_t *acknowledged_exception_branch = create_basic_block();
  fastiss::block_t *delayed_acknowledged_exception_branch = create_basic_block();  
  
  // Detrmine if FPEC.FPIVD has been set by the previous FPU instruction
  fastiss::i1_t* is_fpivd_set = gen_icmp_eq(gen_and(gen_read_FPEC(), gen_i32_constant(1)), gen_i32_constant(1));

  // Branch to ignoring the instruction or checking the exception
  gen_cond_branch(is_fpivd_set, continue_block, exec_inst_branch);

  set_gen_point(exec_inst_branch);  

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
  gen_write_FPEC(gen_i32_constant(1));
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
  
  // Update the PC to the floating point exception handler address if an enabled exception has occurred
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x00000070)));

  if(m_link_func_enable){
    // clear the link(ll-bit) information
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), NULL);
  }

  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();

  // Branch to the exit block
  gen_branch(get_exit_block());
}

void rh850::custom_impl_t::gen_update_dest_and_fpsr(bool is_single_precision, rh850::GR_index_t dest_index, fastiss::i64_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block)
{
  // Create sub-blocks
  fastiss::block_t *exec_inst_branch = create_basic_block();
  fastiss::block_t *exception_branch = create_basic_block();
  fastiss::block_t *no_exception_branch = create_basic_block();
  fastiss::block_t *acknowledged_exception_branch = create_basic_block();
  fastiss::block_t *delayed_acknowledged_exception_branch = create_basic_block();  
  
  // Detrmine if FPEC.FPIVD has been set by the previous FPU instruction
  fastiss::i1_t* is_fpivd_set = gen_icmp_eq(gen_and(gen_read_FPEC(), gen_i32_constant(1)), gen_i32_constant(1));

  // Branch to ignoring the instruction or checking the exception
  gen_cond_branch(is_fpivd_set, continue_block, exec_inst_branch);

  set_gen_point(exec_inst_branch);  

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
  gen_write_FPEC(gen_i32_constant(1));
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
  
  // Update the PC to the floating point exception handler address if an enabled exception has occurred
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x00000070)));

  if(m_link_func_enable){
    // clear the link(ll-bit) information
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), NULL);
  }

  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();

  // Branch to the exit block
  gen_branch(get_exit_block());
}

void rh850::custom_impl_t::set_mdp_enable(bool is_mdp)
{ 
  m_mdp_enable = is_mdp;
}

bool rh850::custom_impl_t::is_mdp_enable(void)
{
  return m_mdp_enable;
}

fastiss::i8_t *rh850::custom_impl_t::gen_read_round_mode(void)
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

void rh850::custom_impl_t::gen_clear_fpi_flag(void)
{
  fastiss::i32_ptr_t* pending_flag_address = gen_i32_constant_ptr(m_pending_fpu_imprecise_exception_flag);
  gen_store(gen_i32_constant(0), pending_flag_address);
}

void rh850::custom_impl_t::gen_bins_common(rh850::GR_index_t reg1, rh850::GR_index_t reg2, fastiss::i32_t *msb_val, fastiss::i32_t *lsb_val)
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

  rh850::GR_index_t temp_reg1 = GR_index_t(reg2 + 10);
  rh850::GR_index_t temp_reg2 = GR_index_t(reg2 + 11);

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

fastiss::i32_t *rh850::custom_impl_t::gen_bit_extract(fastiss::i32_t *x_val, fastiss::i32_t *l_val, fastiss::i32_t *r_val)
{
  fastiss::i32_t *i32_31   = gen_i32_constant(31);
  fastiss::i32_t *shift  = gen_sub(i32_31, l_val);;
  fastiss::i32_t *tmp_val1 = gen_shl(x_val, shift);
  tmp_val1 = gen_lshr(tmp_val1, shift);
  return (gen_lshr(tmp_val1, r_val));
}

fastiss::i8_ptr_t *rh850::custom_impl_t::gen_read_link_manager_ptr(void)
{
  return gen_i8_constant_ptr(m_link_manager);
}

bool rh850::custom_impl_t::is_link_func_enable(void)
{
  return (m_link_func_enable);
}

unsigned int rh850::custom_impl_t::get_cpu_milestone(void)
{
  return (m_cpu_milestone);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
