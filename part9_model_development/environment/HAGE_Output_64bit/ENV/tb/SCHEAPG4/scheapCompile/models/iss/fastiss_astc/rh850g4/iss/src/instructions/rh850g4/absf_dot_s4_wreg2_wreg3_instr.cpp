/*************************************************************************
 *
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
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

#include "rh850g4/absf_dot_s4_wreg2_wreg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// absf.s4 wreg2, wreg3 implementation
//------------------------------------------------------------------------------

rh850g4::absf_dot_s4_wreg2_wreg3_instr_t::absf_dot_s4_wreg2_wreg3_instr_t
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

rh850g4::absf_dot_s4_wreg2_wreg3_instr_t::~absf_dot_s4_wreg2_wreg3_instr_t()
{
}

void rh850g4::absf_dot_s4_wreg2_wreg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::absf_dot_s4_wreg2_wreg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  WR_index_t wreg2 = get_wreg2_u8_11_15(opcode);
  std::string wreg2_name = register_name_WR(WR_index_t(wreg2*2));
  WR_index_t wreg3 = get_wreg3_u8_27_31(opcode);
  std::string wreg3_name = register_name_WR(WR_index_t(wreg3*2));
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "absf.s4 " << wreg2_name << ", " << wreg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::absf_dot_s4_wreg2_wreg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::absf_dot_s4_wreg2_wreg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  WR_index_t wreg2 = get_wreg2_u8_11_15(opcode);
  WR_index_t wreg3 = get_wreg3_u8_27_31(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  WR_index_t wreg2lo = WR_index_t(wreg2*2);
  WR_index_t wreg2up = WR_index_t((wreg2*2) + 1);
  WR_index_t wreg3lo = WR_index_t(wreg3*2);

  // Create sub-blocks
  fastiss::block_t *continue_block = create_basic_block();

  // Load operand registers
  fastiss::i64_t *wreg2lo_load = gen_read_WR(wreg2lo);
  fastiss::i64_t *wreg2up_load = gen_read_WR(wreg2up);

  // Extract upper and lower words of the result
  fastiss::i32_t *w0_val = gen_i32_extract(wreg2lo_load, gen_i64_constant(0));
  fastiss::i32_t *w1_val = gen_i32_extract(wreg2lo_load, gen_i64_constant(1));
  fastiss::i32_t *w2_val = gen_i32_extract(wreg2up_load, gen_i64_constant(0));
  fastiss::i32_t *w3_val = gen_i32_extract(wreg2up_load, gen_i64_constant(1));

  // Cast the operand to a 32-bit floating point values
  fastiss::f32_t *w0_float32 = gen_f32_cast(w0_val);
  fastiss::f32_t *w1_float32 = gen_f32_cast(w1_val);
  fastiss::f32_t *w2_float32 = gen_f32_cast(w2_val);
  fastiss::f32_t *w3_float32 = gen_f32_cast(w3_val);

  // Determine the ABS result - clear the sign bit for all operands except quiet NaN's
  fastiss::i32_t *abs_w0_result = gen_and(gen_i32_cast(w0_float32), gen_i32_constant(0x7fffffff));
  fastiss::i32_t *abs_w1_result = gen_and(gen_i32_cast(w1_float32), gen_i32_constant(0x7fffffff));
  fastiss::i32_t *abs_w2_result = gen_and(gen_i32_cast(w2_float32), gen_i32_constant(0x7fffffff));
  fastiss::i32_t *abs_w3_result = gen_and(gen_i32_cast(w3_float32), gen_i32_constant(0x7fffffff));
  fastiss::i64_t *abs_lo_result = gen_i64_compose(abs_w1_result, abs_w0_result);
  fastiss::i64_t *abs_up_result = gen_i64_compose(abs_w3_result, abs_w2_result);

  // Update the result, FXSR.XP, and handle exceptions
  gen_update_dest_and_fxsr(true, wreg3lo, abs_lo_result, abs_up_result, gen_i8_constant(0),  gen_i8_constant(0),  gen_i8_constant(0),  gen_i8_constant(0), continue_block);
  
  // Continue block
  set_gen_point(continue_block);
  
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
