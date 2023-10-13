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

#include "rh850/maxf_dot_d_reg1_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// maxf.d reg1, reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850::maxf_dot_d_reg1_reg2_reg3_instr_t::maxf_dot_d_reg1_reg2_reg3_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850::memory_if_t *memory_impl,
  rh850::operand_if_t *operand_impl,
  rh850::cpu_state_if_t *cpu_state_impl,
  rh850::custom_if_t *custom_impl
) :
  rh850::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
}

rh850::maxf_dot_d_reg1_reg2_reg3_instr_t::~maxf_dot_d_reg1_reg2_reg3_instr_t()
{
}

void rh850::maxf_dot_d_reg1_reg2_reg3_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::maxf_dot_d_reg1_reg2_reg3_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
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
  disass_string << "maxf.d " << reg1_name << ", " << reg2_name << ", " << reg3_name; 
  return disass_string.str();
}

unsigned int rh850::maxf_dot_d_reg1_reg2_reg3_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::maxf_dot_d_reg1_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
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
  gen_increment_cycle_count(gen_select(gen_read_FPSR_PEM(), gen_i32_constant(7), gen_i32_constant(1)));

  // Create sub-blocks
  fastiss::block_t *invalid_qnan_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *qnan_branch = create_basic_block();
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
  
  // Check if either operand is a signalling NaN
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(reg1_pair_float64), gen_is_signalling_nan(reg2_pair_float64));
 
  // Branch to the invalid quiet Nan case if either operand is a signalling NaN
  gen_cond_branch(is_either_signalling_nan, invalid_qnan_branch, check_qnan_branch);

  // Check if both operand are quiet NaN
  set_gen_point(check_qnan_branch);
  fastiss::i1_t* is_both_quiet_nan = gen_and(gen_is_quiet_nan(reg1_pair_float64), gen_is_quiet_nan(reg2_pair_float64));
  gen_cond_branch(is_both_quiet_nan, qnan_branch, perform_calc_branch);
  
  // Handle invalid quiet NaN case
  set_gen_point(invalid_qnan_branch);
  gen_update_dest_and_fpsr(false, reg3, gen_i64_constant(0x7fffffffffffffffULL), gen_i8_constant(0x10), continue_block);

  // Handle quiet NaN case
  set_gen_point(qnan_branch);
  gen_update_dest_and_fpsr(false, reg3, reg2_pair, gen_i8_constant(0x00), continue_block);

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Determine the MAX result
  fastiss::f64_t *max_result = gen_select(gen_is_quiet_nan(reg1_pair_float64), 
					  reg2_pair_float64, 
					  gen_select(gen_is_quiet_nan(reg2_pair_float64), 
						     reg1_pair_float64,
						     gen_select(gen_fcmp_uge(reg1_pair_float64, reg2_pair_float64), 
								reg1_pair_float64,
								reg2_pair_float64)));
  
  // Update the result, FPSR.XP, and handle exceptions
  gen_update_dest_and_fpsr(false, reg3, gen_i64_cast(max_result), gen_i8_constant(0), continue_block);
  
  // Continue block
  set_gen_point(continue_block);
  
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
