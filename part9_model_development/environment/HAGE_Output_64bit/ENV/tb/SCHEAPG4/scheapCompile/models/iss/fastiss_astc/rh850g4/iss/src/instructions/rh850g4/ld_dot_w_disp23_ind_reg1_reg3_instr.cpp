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

#include "rh850g4/ld_dot_w_disp23_ind_reg1_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ld.w disp23 [reg1], reg3 implementation
//------------------------------------------------------------------------------

rh850g4::ld_dot_w_disp23_ind_reg1_reg3_instr_t::ld_dot_w_disp23_ind_reg1_reg3_instr_t
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

rh850g4::ld_dot_w_disp23_ind_reg1_reg3_instr_t::~ld_dot_w_disp23_ind_reg1_reg3_instr_t()
{
}

void rh850g4::ld_dot_w_disp23_ind_reg1_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(6);  
}

std::string rh850g4::ld_dot_w_disp23_ind_reg1_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  int disp23 = get_disp23_s32_32_47_21_26_shift1(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  if (disp23 < 0)
  {
    disass_string << "ld.w -0x" << std::hex << -disp23 << "[" << reg1_name << "], " << reg3_name;
  }
  else
  {
    disass_string << "ld.w 0x" << std::hex << disp23 << "[" << reg1_name << "], " << reg3_name;
  }
  return disass_string.str();
}

unsigned int rh850g4::ld_dot_w_disp23_ind_reg1_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 6;
}

void rh850g4::ld_dot_w_disp23_ind_reg1_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *disp23 = gen_get_disp23_s32_32_47_21_26_shift1(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);

  // Calculate the address to load from
  fastiss::i32_t *address = gen_add(reg1_load, disp23);

  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x02));

  // Load the word from memory
  fastiss::i32_t *loaded_val = gen_i32_load(address);
  
  // Write the result to reg3
  gen_write_GR(reg3, loaded_val);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
