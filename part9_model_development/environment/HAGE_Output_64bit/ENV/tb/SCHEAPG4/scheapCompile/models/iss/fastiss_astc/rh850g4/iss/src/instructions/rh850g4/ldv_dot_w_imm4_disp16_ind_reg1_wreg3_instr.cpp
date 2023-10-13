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

#include "rh850g4/ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ldv.w imm4, disp16 [reg1], wreg3 implementation
//------------------------------------------------------------------------------

rh850g4::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t
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

rh850g4::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t::~ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t()
{
}

void rh850g4::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(6);  
}

std::string rh850g4::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int imm4 = get_imm4_u32_21_24(opcode);
  int disp16 = get_disp16_s32_34_47_shift2(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  WR_index_t wreg3 = get_wreg3_u8_27_31(opcode);
  std::string wreg3_name = register_name_WR(WR_index_t(wreg3*2));
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "ldv.w " << imm4 << ", 0x" << std::hex << disp16 << "[" << reg1_name << "], " << wreg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 6;
}

void rh850g4::ldv_dot_w_imm4_disp16_ind_reg1_wreg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *imm4 = gen_get_imm4_u32_21_24(opcode);
  fastiss::i32_t *disp16 = gen_get_disp16_s32_34_47_shift2(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  WR_index_t wreg3 = get_wreg3_u8_27_31(opcode);

  WR_index_t wreg3lo = WR_index_t(wreg3*2);
  WR_index_t wreg3up = WR_index_t((wreg3*2) + 1);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i64_t *wreg3lo_load = gen_read_WR(wreg3lo);
  fastiss::i64_t *wreg3up_load = gen_read_WR(wreg3up);

  // Calculate the address to load from
  fastiss::i32_t *address = gen_add(reg1_load, disp16);

  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x02));

  // Load the word from memory
  fastiss::i32_t *loaded_val = gen_i32_load(address);

  // Extract upper and lower words of the result
  fastiss::i32_t *w0_val = gen_i32_extract(wreg3lo_load, gen_i64_constant(0));
  fastiss::i32_t *w1_val = gen_i32_extract(wreg3lo_load, gen_i64_constant(1));
  fastiss::i32_t *w2_val = gen_i32_extract(wreg3up_load, gen_i64_constant(0));
  fastiss::i32_t *w3_val = gen_i32_extract(wreg3up_load, gen_i64_constant(1));

  // Determine the move result
  fastiss::i32_t *w0_val_result = gen_select(gen_icmp_eq(gen_and(imm4, gen_i32_constant(1)), gen_i32_constant(1)), loaded_val, w0_val);
  fastiss::i32_t *w1_val_result = gen_select(gen_icmp_eq(gen_and(imm4, gen_i32_constant(2)), gen_i32_constant(2)), loaded_val, w1_val);
  fastiss::i32_t *w2_val_result = gen_select(gen_icmp_eq(gen_and(imm4, gen_i32_constant(4)), gen_i32_constant(4)), loaded_val, w2_val);
  fastiss::i32_t *w3_val_result = gen_select(gen_icmp_eq(gen_and(imm4, gen_i32_constant(8)), gen_i32_constant(8)), loaded_val, w3_val);
  fastiss::i64_t *wreg_lo_result = gen_i64_compose(w1_val_result, w0_val_result);
  fastiss::i64_t *wreg_up_result = gen_i64_compose(w3_val_result, w2_val_result);

  // Write the result to wreg3
  gen_write_WR(wreg3lo, wreg_lo_result);
  gen_write_WR(wreg3up, wreg_up_result);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
