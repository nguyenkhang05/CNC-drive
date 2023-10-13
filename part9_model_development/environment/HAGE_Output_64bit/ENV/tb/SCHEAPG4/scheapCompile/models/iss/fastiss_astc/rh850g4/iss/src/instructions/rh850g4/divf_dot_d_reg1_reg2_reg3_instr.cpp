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

#include "rh850g4/divf_dot_d_reg1_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// divf.d reg1, reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t::divf_dot_d_reg1_reg2_reg3_instr_t
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

rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t::~divf_dot_d_reg1_reg2_reg3_instr_t()
{
}

void rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_1_4_shift1(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_28_31_shift1(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "divf.d " << reg1_name << ", " << reg2_name << ", " << reg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_1_4_shift1(opcode);
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  GR_index_t reg3 = get_reg3_u8_28_31_shift1(opcode);

  // Determine paired register indices
  GR_index_t reg1b = GR_index_t(reg1 + 1);
  GR_index_t reg2b = GR_index_t(reg2 + 1);
  
  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *check_div_by_zero_branch = create_basic_block();
  fastiss::block_t *div_by_zero_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg1b_load = gen_read_GR(reg1b);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  fastiss::i32_t *reg2b_load = gen_read_GR(reg2b);

  // Create the 64-bit operand values
  fastiss::i64_t *reg1_pair = gen_i64_compose(reg1b_load, reg1_load);
  fastiss::i64_t *reg2_pair = gen_i64_compose(reg2b_load, reg2_load);

  // Cast the operands to 64-bit floating point values
  fastiss::f64_t *reg1_pair_float64 = gen_f64_cast(reg1_pair);
  fastiss::f64_t *reg2_pair_float64 = gen_f64_cast(reg2_pair);
  
  // Check if the operand is a denormaled value and FPSR.FS=0
  gen_cond_branch(gen_or(is_unimplemented_exp(reg1_pair_float64), is_unimplemented_exp(reg2_pair_float64)), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_constant(0), gen_i8_constant(0x20), continue_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f64_t *reg1_pair_float64_flushed = gen_flush_denorm(reg1_pair_float64);
  fastiss::f64_t *reg2_pair_float64_flushed = gen_flush_denorm(reg2_pair_float64);

  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(reg1_pair_float64), gen_is_signalling_nan(reg2_pair_float64));
  
  // Check if both operands are infinity or if both operands are zero
  fastiss::i1_t* is_reg2_pair_infinity = gen_is_infinity(reg2_pair_float64);
  fastiss::i1_t* is_both_infinity = gen_and(gen_is_infinity(reg1_pair_float64), is_reg2_pair_infinity);
  fastiss::i1_t* is_reg1_pair_zero = gen_is_zero(reg1_pair_float64_flushed);
  fastiss::i1_t* is_both_zero = gen_and(is_reg1_pair_zero, gen_is_zero(reg2_pair_float64_flushed));
  fastiss::i1_t* is_both_infinity_or_zero = gen_or(is_both_infinity, is_both_zero);

  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  // or both operands are infinity or zero
  gen_cond_branch(gen_or(is_either_signalling_nan, is_both_infinity_or_zero), invalid_qnan_branch, check_div_by_zero_branch);

  // Check if dividing by zero and reg2 is neither a quiet NaN or infinity
  set_gen_point(check_div_by_zero_branch);
  fastiss::i1_t* is_reg2_pair_qnan_or_infinity = gen_or(gen_is_quiet_nan(reg2_pair_float64), is_reg2_pair_infinity);
  fastiss::i1_t* is_reg1_pair_zero_reg2_pair_neither_qnan_nor_infinity = gen_and(is_reg1_pair_zero, gen_not(is_reg2_pair_qnan_or_infinity));
  gen_cond_branch(is_reg1_pair_zero_reg2_pair_neither_qnan_nor_infinity, div_by_zero_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_constant(0x7fffffffffffffffULL), gen_i8_constant(0x10), continue_block);
  
  // Handle divide by zero case
  set_gen_point(div_by_zero_branch);
  fastiss::i1_t* is_same_sign = gen_icmp_eq(gen_is_positive(reg1_pair_float64), gen_is_positive(reg2_pair_float64));
  fastiss::i64_t* div_val = gen_select(is_same_sign, gen_i64_constant(0x7ff0000000000000ULL), gen_i64_constant(0xfff0000000000000ULL));
  gen_update_dest_and_fpsr(false, reg3, div_val, gen_i8_constant(0x08), continue_block);  

  // Handle general operand case
  set_gen_point(perform_calc_branch);   
  
  // Determine the DIV result - always round to nearest
  fastiss::f64_with_exception_flags_t div_result = gen_f64_div(reg2_pair_float64_flushed, reg1_pair_float64_flushed, gen_read_round_mode());
    
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(div_result);

  // Check if the result is a denormaled or underflow value and FPSR.FS=0
  gen_cond_branch(gen_and(div_result.underflow, gen_icmp_eq(gen_read_FPSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_constant(0), gen_i8_constant(0x20), continue_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  div_result.result = gen_select
  (
    div_result.underflow,
    gen_f64_cast(gen_and(gen_i64_cast(div_result.result), gen_i64_constant(0x8000000000000000ULL))),
    div_result.result
  );
 
  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  div_result.inexact = gen_inexact_enabled_overflow_underflow_update(div_result.overflow, div_result.underflow, div_result.inexact);

  // Check if the inexact flag must be explicitly set
  div_result.inexact = gen_inexact_denorm_update(div_result.invalid, div_result.divide_by_zero, div_result.inexact, reg1_pair_float64, reg2_pair_float64);

  // Update the result, FPSR.XP, and handle exceptions
  div_result.result = gen_select(gen_is_quiet_nan(reg2_pair_float64),
				 reg2_pair_float64,
				 gen_select(gen_is_quiet_nan(reg1_pair_float64),
					    reg1_pair_float64,
					    div_result.result));
  fastiss::i8_t* div_result_xp = gen_i8_compose(gen_false(), gen_false(), gen_false(), div_result.invalid, div_result.divide_by_zero, div_result.overflow, div_result.underflow, div_result.inexact);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_cast(div_result.result), div_result_xp, continue_block);

  // Continue block
  set_gen_point(continue_block);
  
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}

unsigned int rh850g4::divf_dot_d_reg1_reg2_reg3_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 16;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
