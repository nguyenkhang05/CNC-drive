/*************************************************************************
 *
 * Copyright(c) 2015-2017 Renesas Electronics Corporation
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

#include "rh850g4/sqrtf_dot_d_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// sqrtf.d reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::sqrtf_dot_d_reg2_reg3_instr_t::sqrtf_dot_d_reg2_reg3_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::operand_if_t *operand_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
) :
  rh850g4::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
}

rh850g4::sqrtf_dot_d_reg2_reg3_instr_t::~sqrtf_dot_d_reg2_reg3_instr_t()
{
}

void rh850g4::sqrtf_dot_d_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::sqrtf_dot_d_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_28_31_shift1(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "sqrtf.d " << reg2_name << ", " << reg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::sqrtf_dot_d_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::sqrtf_dot_d_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  GR_index_t reg3 = get_reg3_u8_28_31_shift1(opcode);

  // Determine paired register indices
  GR_index_t reg2b = GR_index_t(reg2 + 1);
  
  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *is_negative_zero_or_negative_qnan_branch = create_basic_block();
  fastiss::block_t *check_is_invalid_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();  
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Load operand register
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  fastiss::i32_t *reg2b_load = gen_read_GR(reg2b);

  // Create the 64-bit operand value
  fastiss::i64_t *reg2_pair = gen_i64_compose(reg2b_load, reg2_load);
 
  // Cast the operand to 64-bit floating point values
  fastiss::f64_t *reg2_pair_float64 = gen_f64_cast(reg2_pair);

  // Check if the operand is a denormaled value and FPSR.FS=0
  gen_cond_branch(is_unimplemented_exp(reg2_pair_float64), unimplemented_exp_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_constant(0), gen_i8_constant(0x20), continue_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f64_t *reg2_pair_float64_flushed = gen_flush_denorm(reg2_pair_float64);
  
  // Check if the operand is negative zero or negative quiet NaN
  fastiss::i1_t* is_negative = gen_is_negative(reg2_pair_float64);
  fastiss::i1_t* is_zero = gen_is_zero(reg2_pair_float64_flushed);
  fastiss::i1_t* is_quiet_nan = gen_is_quiet_nan(reg2_pair_float64);
  fastiss::i1_t* is_negative_zero_or_negative_qnan = gen_or(gen_and(is_negative, is_zero), gen_and(is_negative, is_quiet_nan));

  // Branch to the special case if the operand is negative zero or negative quiet NaN
  gen_cond_branch(is_negative_zero_or_negative_qnan, is_negative_zero_or_negative_qnan_branch, check_is_invalid_qnan_branch);  
        
  // Check if the operand is a signalling NaN or an invalid negative operand
  set_gen_point(check_is_invalid_qnan_branch);
  fastiss::i1_t* is_signalling_nan = gen_is_signalling_nan(reg2_pair_float64);

  // Branch to the invalid quiet Nan case if the operand is a signalling NaN
  // or an invalid negative operand
  gen_cond_branch(gen_or(is_signalling_nan, is_negative), invalid_qnan_branch, perform_calc_branch);

  // Handle negative zero or negative quiet NaN operand case
  set_gen_point(is_negative_zero_or_negative_qnan_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_cast(reg2_pair_float64_flushed), gen_i8_zext(gen_is_denorm(reg2_pair_float64)), continue_block);  

  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_constant(0x7fffffffffffffffULL), gen_i8_constant(0x10), continue_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Determine the SQRT result - always round to nearest
  fastiss::f64_with_exception_flags_t sqrt_result = gen_f64_sqrt(reg2_pair_float64_flushed, gen_read_round_mode());
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(sqrt_result);
  sqrt_result.result = gen_select
  (
    sqrt_result.underflow,
    gen_f64_cast(gen_and(gen_i64_cast(sqrt_result.result), gen_i64_constant(0x8000000000000000ULL))),
    sqrt_result.result
  );

  // Check if the inexact flag must be explicitly set
  sqrt_result.inexact = gen_inexact_denorm_update(sqrt_result.invalid, sqrt_result.divide_by_zero, sqrt_result.inexact, reg2_pair_float64);
 
  // Update the result, FPSR.XP, and handle exceptions
  fastiss::i8_t* sqrt_result_xp = gen_i8_compose(gen_false(), gen_false(), gen_false(), sqrt_result.invalid, sqrt_result.divide_by_zero, sqrt_result.overflow, sqrt_result.underflow, sqrt_result.inexact);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_cast(sqrt_result.result), sqrt_result_xp, continue_block);

  // Continue block
  set_gen_point(continue_block);
  
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}

unsigned int rh850g4::sqrtf_dot_d_reg2_reg3_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 30;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
