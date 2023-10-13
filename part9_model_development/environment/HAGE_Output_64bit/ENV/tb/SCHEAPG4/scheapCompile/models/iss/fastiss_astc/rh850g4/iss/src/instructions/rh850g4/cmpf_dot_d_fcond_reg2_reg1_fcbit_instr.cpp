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

#include "rh850g4/cmpf_dot_d_fcond_reg2_reg1_fcbit_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// cmpf.d fcond, reg2, reg1, fcbit implementation
//------------------------------------------------------------------------------

rh850g4::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t
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

rh850g4::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t::~cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t()
{
}

void rh850g4::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_1_4_shift1(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  std::string reg2_name = register_name_GR(reg2);
  unsigned char fcbit = get_fcbit_u8_17_19(opcode);
  unsigned char fcond = get_fcond_u8_27_30(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "cmpf.d " << fpu_condition_code_name[fcond] << ", " << reg2_name << ", " << reg1_name << ", 0x" << std::hex << (unsigned int)fcbit;
  return disass_string.str();
}

unsigned int rh850g4::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::cmpf_dot_d_fcond_reg2_reg1_fcbit_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_1_4_shift1(opcode);
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  fastiss::i8_t *fcbit = gen_get_fcbit_u8_17_19(opcode);
  fastiss::i8_t *fcond = gen_get_fcond_u8_27_30(opcode);

  // Determine paired register indices
  GR_index_t reg1b = GR_index_t(reg1 + 1);
  GR_index_t reg2b = GR_index_t(reg2 + 1);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *nan_branch = create_basic_block();
  fastiss::block_t *no_nan_branch = create_basic_block();
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

  // Determine if either operand is a NaN
  fastiss::i1_t *is_reg1_nan = gen_is_nan(reg1_pair_float64);
  fastiss::i1_t *is_reg2_nan = gen_is_nan(reg2_pair_float64);

  fastiss::i8_ptr_t *cmp_result = gen_i8_alloc();

  // Branch to the NaN case if either operand is a NaN
  gen_cond_branch(gen_or(is_reg1_nan, is_reg2_nan), nan_branch, no_nan_branch);

  // Handle NaN case
  set_gen_point(nan_branch);

  // Determine the comparison result
  gen_store(gen_i8_zext(gen_i1_extract(fcond, gen_i8_constant(0))), cmp_result);

  // Determine the value of the invalid bit and check for exceptions. Note that
  // unlike other FPU instructions there is no result to write back to a general
  // purpose register. To allow the same custom operation to be used a value of
  // 0 is written back to the r0 register which is always zero anyhow.
  fastiss::i1_t* is_either_signalling_nan = gen_or(gen_is_signalling_nan(reg1_pair_float64), gen_is_signalling_nan(reg2_pair_float64));
  fastiss::i1_t *invalid_bit = gen_i1_extract(fcond, gen_i8_constant(3));
  fastiss::i8_t *res_exp = gen_select(gen_or(is_either_signalling_nan, invalid_bit), gen_i8_constant(0x10), gen_i8_constant(0x00));
  gen_update_dest_and_fpsr(false, GR_INDEX_R0_0, gen_i32_constant(0), res_exp, continue_block);

  // Handle no NaN case
  set_gen_point(no_nan_branch);
  
  // Determine the comparison result - use ordered comparison operations
  fastiss::i1_t *is_less_than = gen_fcmp_olt(reg2_pair_float64, reg1_pair_float64);
  fastiss::i1_t *is_equal = gen_fcmp_oeq(reg2_pair_float64, reg1_pair_float64);
  
  gen_store(gen_i8_zext(gen_or(gen_and(gen_i1_extract(fcond, gen_i8_constant(2)), is_less_than), gen_and(gen_i1_extract(fcond, gen_i8_constant(1)), is_equal))), cmp_result);
  gen_branch(continue_block);  

  // Handle continue case
  set_gen_point(continue_block);

  // Read the FPSR condition code bits
  fastiss::i8_t *fpsr_cc_load = gen_read_FPSR_CC();
  
  // Determine the resultant FPSR condition code value
  fastiss::i8_t *selected_bit_clear_mask = gen_not(gen_shl(gen_i8_constant(1), fcbit));
  fastiss::i8_t *cleared_fpsr_cc = gen_and(fpsr_cc_load, selected_bit_clear_mask);
  fastiss::i8_t *shifted_cmp_result = gen_shl(gen_load(cmp_result), fcbit);
  fastiss::i8_t *new_fpsr_cc = gen_or(cleared_fpsr_cc, shifted_cmp_result);
    
  // Write the comparison result to the specified FPSR condition bit
  gen_write_FPSR_CC(new_fpsr_cc);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
