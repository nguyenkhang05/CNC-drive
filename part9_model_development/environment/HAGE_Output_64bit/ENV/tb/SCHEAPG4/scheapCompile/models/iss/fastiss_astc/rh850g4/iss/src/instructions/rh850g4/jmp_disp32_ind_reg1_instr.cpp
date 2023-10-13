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

#include "rh850g4/jmp_disp32_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// jmp disp32[reg1] implementation
//------------------------------------------------------------------------------

rh850g4::jmp_disp32_ind_reg1_instr_t::jmp_disp32_ind_reg1_instr_t
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

rh850g4::jmp_disp32_ind_reg1_instr_t::~jmp_disp32_ind_reg1_instr_t()
{
}

void rh850g4::jmp_disp32_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(6);  
}

std::string rh850g4::jmp_disp32_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  int disp32 = get_disp32_s32_17_47_shift1(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  if (disp32 < 0)
  {
    disass_string << "jmp -0x" << std::hex << -disp32 << "[" << reg1_name << "]";
  }
  else
  {
    disass_string << "jmp 0x" << std::hex << disp32 << "[" << reg1_name << "]";
  }
  return disass_string.str();
}

unsigned int rh850g4::jmp_disp32_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 6;
}

void rh850g4::jmp_disp32_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  fastiss::i32_t *disp32 = gen_get_disp32_s32_17_47_shift1(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);

  // Set the new PC value
  gen_write_PC(gen_add(reg1_load,disp32));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

bool rh850g4::jmp_disp32_ind_reg1_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850g4::jmp_disp32_ind_reg1_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
