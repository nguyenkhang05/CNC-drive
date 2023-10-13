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

#include "rh850g4/jr_disp32_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// jr disp32 implementation
//------------------------------------------------------------------------------

rh850g4::jr_disp32_instr_t::jr_disp32_instr_t
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

rh850g4::jr_disp32_instr_t::~jr_disp32_instr_t()
{
}

void rh850g4::jr_disp32_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  int disp32 = get_disp32_s32_17_47_shift1(opcode);
  
  // Calculate the new relative PC value
  increment_trans_PC(disp32);
}

std::string rh850g4::jr_disp32_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  int disp32 = get_disp32_s32_17_47_shift1(opcode);

  int branch_pc = read_trans_PC() + disp32;

  // Render and return result
  std::ostringstream disass_string;
  disass_string << "jr 0x" << std::hex << branch_pc;
  return disass_string.str();
}

unsigned int rh850g4::jr_disp32_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 6;
}

void rh850g4::jr_disp32_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  //fastiss::i32_t *disp32 = gen_get_disp32_s32_17_47_shift1(opcode);
  int disp32 = get_disp32_s32_17_47_shift1(opcode);

  // Increment the PC
  //gen_increment_PC(disp32);
  gen_write_PC(gen_i32_constant(trans_PC + disp32));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

unsigned int rh850g4::jr_disp32_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
