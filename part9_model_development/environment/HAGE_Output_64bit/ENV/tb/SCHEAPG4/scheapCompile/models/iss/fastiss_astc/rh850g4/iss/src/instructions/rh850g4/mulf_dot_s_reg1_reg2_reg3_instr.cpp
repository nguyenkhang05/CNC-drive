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

#include "rh850g4/mulf_dot_s_reg1_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// mulf.s reg1, reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::mulf_dot_s_reg1_reg2_reg3_instr_t::mulf_dot_s_reg1_reg2_reg3_instr_t
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

rh850g4::mulf_dot_s_reg1_reg2_reg3_instr_t::~mulf_dot_s_reg1_reg2_reg3_instr_t()
{
}

void rh850g4::mulf_dot_s_reg1_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::mulf_dot_s_reg1_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "mulf.s " << reg1_name << ", " << reg2_name << ", " << reg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::mulf_dot_s_reg1_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::mulf_dot_s_reg1_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_op_branch = create_basic_block();
  fastiss::block_t *unimplemented_exp_res_branch = create_basic_block();
  fastiss::block_t *implemented_res_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  // Cast the operands to 32-bit floating point values
  fastiss::f32_t *reg1_float32 = gen_f32_cast(reg1_load);
  fastiss::f32_t *reg2_float32 = gen_f32_cast(reg2_load);

  // Check if the operand is a denormaled value and FPSR.FS=0
  gen_cond_branch(gen_or(is_unimplemented_exp(reg1_float32), is_unimplemented_exp(reg2_float32)), unimplemented_exp_op_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_op_branch);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_constant(0), gen_i8_constant(0x20), continue_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *reg1_float32_flushed = gen_flush_denorm(reg1_float32);
  fastiss::f32_t *reg2_float32_flushed = gen_flush_denorm(reg2_float32);
  
  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(reg1_float32), gen_is_signalling_nan(reg2_float32));
  
  // Check if one operand is infinity and the other zero
  fastiss::i1_t* is_reg1_infinity = gen_is_infinity(reg1_float32);
  fastiss::i1_t* is_reg2_infinity = gen_is_infinity(reg2_float32);
  fastiss::i1_t* is_reg1_zero = gen_is_zero(reg1_float32_flushed);
  fastiss::i1_t* is_reg2_zero = gen_is_zero(reg2_float32_flushed);
  fastiss::i1_t* is_one_infinity_other_zero = gen_or(gen_and(is_reg1_infinity, is_reg2_zero), gen_and(is_reg2_infinity, is_reg1_zero));

  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  // or both operands are infinity with opposite signs
  gen_cond_branch(gen_or(is_either_signalling_nan, is_one_infinity_other_zero), invalid_qnan_branch, perform_calc_branch);

  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_constant(0x7fffffff), gen_i8_constant(0x10), continue_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   
  
  // Extend the operands to 80-bit floating point values
  fastiss::f80_t *reg1_float80 = gen_f80_ext(reg1_float32_flushed);
  fastiss::f80_t *reg2_float80 = gen_f80_ext(reg2_float32_flushed);
  
  // Determine the MUL result - always round to nearest
  fastiss::f32_with_exception_flags_t mul_result = gen_f32_mul(reg1_float80, reg2_float80, gen_read_round_mode());
  
  // Check for denormal (underflow) results
  gen_underflow_denorm_update(mul_result);

  // Check if the operand is a denormaled value and FPSR.FS=0
  gen_cond_branch(gen_and(mul_result.underflow, gen_icmp_eq(gen_read_FPSR_FS(), gen_i1_constant(0))), 
		  unimplemented_exp_res_branch,
		  implemented_res_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_res_branch);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_constant(0), gen_i8_constant(0x20), continue_block);

  // not unimplementeded exception case
  set_gen_point(implemented_res_branch);

  mul_result.result = gen_select
  (
    mul_result.underflow,
    gen_f32_cast(gen_and(gen_i32_cast(mul_result.result), gen_i32_constant(0x80000000))),
    mul_result.result
  );
 
  // Check if the inexact flag must be explicitly cleared due to an enabled overflow/underflow
  mul_result.inexact = gen_inexact_enabled_overflow_underflow_update(mul_result.overflow, mul_result.underflow, mul_result.inexact);

  // Check if the inexact flag must be explicitly set
  mul_result.inexact = gen_inexact_denorm_update(mul_result.invalid, mul_result.divide_by_zero, mul_result.inexact, reg1_float32, reg2_float32);

  // Update the result, FPSR.XP, and handle exceptions
  mul_result.result = gen_select(gen_is_quiet_nan(reg2_float32),
				 reg2_float32,
				 gen_select(gen_is_quiet_nan(reg1_float32),
					    reg1_float32,
					    mul_result.result));
  fastiss::i8_t* mul_result_xp = gen_i8_compose(gen_false(), gen_false(), gen_false(), mul_result.invalid, mul_result.divide_by_zero, mul_result.overflow, mul_result.underflow, mul_result.inexact);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_cast(mul_result.result), mul_result_xp, continue_block);

  // Continue block
  set_gen_point(continue_block);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
