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

#include "rh850g4/trncf_dot_suw_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// trncf.suw reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::trncf_dot_suw_reg2_reg3_instr_t::trncf_dot_suw_reg2_reg3_instr_t
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

rh850g4::trncf_dot_suw_reg2_reg3_instr_t::~trncf_dot_suw_reg2_reg3_instr_t()
{
}

void rh850g4::trncf_dot_suw_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::trncf_dot_suw_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "trncf.suw " << reg2_name << ", " << reg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::trncf_dot_suw_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::trncf_dot_suw_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *unimplemented_exp_branch = create_basic_block();
  fastiss::block_t *check_qnan_branch = create_basic_block();
  fastiss::block_t *invalid_nan_branch = create_basic_block();
  fastiss::block_t *check_saturation_branch = create_basic_block();
  fastiss::block_t *saturation_branch = create_basic_block();  
  fastiss::block_t *perform_calc_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  // Cast the operands to 32-bit floating point values
  fastiss::f32_t *reg2_float32 = gen_f32_cast(reg2_load);

  // Check if the operand is a denormaled value and FPSR.FS=0
  gen_cond_branch(is_unimplemented_exp(reg2_float32), unimplemented_exp_branch, check_qnan_branch);

  // Handled unimplementeded exception case
  set_gen_point(unimplemented_exp_branch);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_constant(0), gen_i8_constant(0x20), continue_block);

  // Check if qnan
  set_gen_point(check_qnan_branch);

  // Flush denorm operands to zero (keeping the sign of the denorm)
  fastiss::f32_t *reg2_float32_flushed = gen_flush_denorm(reg2_float32);

  // Treat negative zero the same as positive zero
  reg2_float32_flushed = gen_select(gen_is_zero(reg2_float32_flushed), gen_f32_constant(0), reg2_float32_flushed);
  
  // Branch to the invalid nan case if the operand is a NaN or a negative value
  fastiss::i1_t *is_nan = gen_is_nan(reg2_float32);
  fastiss::i1_t* is_negative = gen_is_negative(reg2_float32_flushed);
  gen_cond_branch(gen_or(is_nan, is_negative), invalid_nan_branch, check_saturation_branch);
  
  // Check if the conversion will saturate. Saturation occurs when:
  //    - For a positive floating point number, if it is greater than 2^32-1 (represented as 0x4f800000 in single precision floating point)
  set_gen_point(check_saturation_branch);
  fastiss::i1_t *is_saturated = gen_fcmp_oge(reg2_float32_flushed, gen_f32_cast(gen_i32_constant(0x4f800000)));
  gen_cond_branch(is_saturated, saturation_branch, perform_calc_branch);
  
  // Handle invalid nan case
  set_gen_point(invalid_nan_branch);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_constant(0x00000000), gen_i8_constant(0x10), continue_block);

  // Handle saturation case
  set_gen_point(saturation_branch);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_constant(0xffffffff), gen_i8_constant(0x10), continue_block);  

  // Handle general operand case
  set_gen_point(perform_calc_branch);   

  // Convert the operand to a 32-bit integer value
  fastiss::i32_with_exception_flags_t convert_result = gen_i32_unsigned_convert(reg2_float32_flushed, gen_i8_constant(3));
 
  // Check if the inexact flag must be explicitly set
  convert_result.inexact = gen_inexact_denorm_update(convert_result.invalid, convert_result.divide_by_zero, convert_result.inexact, reg2_float32);
 
  // Update the result, FPSR.XP, and handle exceptions
  fastiss::i8_t* convert_result_xp = gen_i8_compose(gen_false(), gen_false(), gen_false(), convert_result.invalid, convert_result.divide_by_zero, convert_result.overflow, convert_result.underflow, convert_result.inexact);
  gen_update_dest_and_fpsr(true, reg3, convert_result.result, convert_result_xp, continue_block);

  // Continue block
  set_gen_point(continue_block);
    
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
