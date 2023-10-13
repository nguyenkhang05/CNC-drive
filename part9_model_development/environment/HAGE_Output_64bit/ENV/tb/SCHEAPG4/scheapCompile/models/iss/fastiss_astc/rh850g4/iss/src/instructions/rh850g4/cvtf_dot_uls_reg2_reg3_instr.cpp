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

#include "rh850g4/cvtf_dot_uls_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// cvtf.uls reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::cvtf_dot_uls_reg2_reg3_instr_t::cvtf_dot_uls_reg2_reg3_instr_t
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

rh850g4::cvtf_dot_uls_reg2_reg3_instr_t::~cvtf_dot_uls_reg2_reg3_instr_t()
{
}

void rh850g4::cvtf_dot_uls_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::cvtf_dot_uls_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "cvtf.uls " << reg2_name << ", " << reg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::cvtf_dot_uls_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::cvtf_dot_uls_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_12_15_shift1(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Determine paired register indices
  GR_index_t reg2b = GR_index_t(reg2 + 1);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *continue_block = create_basic_block();
   
  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  fastiss::i32_t *reg2b_load = gen_read_GR(reg2b);

  // Create the 64-bit operand value
  fastiss::i64_t *reg2_pair = gen_i64_compose(reg2b_load, reg2_load);

  // Convert the operand to a 32-bit floating point value
  fastiss::f32_with_exception_flags_t convert_result = gen_f32_unsigned_convert(reg2_pair, gen_read_round_mode());
 
  // Update the result, FPSR.XP, and handle exceptions
  fastiss::i8_t* convert_result_xp = gen_i8_compose(gen_false(), gen_false(), gen_false(), convert_result.invalid, convert_result.divide_by_zero, convert_result.overflow, convert_result.underflow, convert_result.inexact);
  gen_update_dest_and_fpsr(true, reg3, gen_i32_cast(convert_result.result), convert_result_xp, continue_block);

  // Continue block
  set_gen_point(continue_block);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
