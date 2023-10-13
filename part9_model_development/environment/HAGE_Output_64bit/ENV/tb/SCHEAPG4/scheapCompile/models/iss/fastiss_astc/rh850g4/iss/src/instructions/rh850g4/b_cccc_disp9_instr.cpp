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

#include "rh850g4/b_cccc_disp9_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// bcccc disp9 implementation
//------------------------------------------------------------------------------

rh850g4::b_cccc_disp9_instr_t::b_cccc_disp9_instr_t
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

rh850g4::b_cccc_disp9_instr_t::~b_cccc_disp9_instr_t()
{
}

void rh850g4::b_cccc_disp9_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char cccc = get_cccc_u8_0_3(opcode);
  int disp9 = get_disp9_s32_11_15_4_6_shift1(opcode);
  
  // Increment PC
  unsigned int pc_increment = 2;
  if (is_condition_always(cccc))
  {
    // For branch always, simply continue translating at the new PC
    pc_increment = disp9;
  }
  increment_trans_PC(pc_increment);  
}

std::string rh850g4::b_cccc_disp9_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char cccc = get_cccc_u8_0_3(opcode);
  int disp9 = get_disp9_s32_11_15_4_6_shift1(opcode);
  int branch_pc = read_trans_PC() + disp9;
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << branch_condition_code_name[cccc] << " 0x" << std::hex << branch_pc;
  return disass_string.str();
}

unsigned int rh850g4::b_cccc_disp9_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850g4::b_cccc_disp9_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  //fastiss::i32_t *disp9 = gen_get_disp9_s32_11_15_4_6_shift1(opcode);
  int disp9 = get_disp9_s32_11_15_4_6_shift1(opcode);

  unsigned char cond = get_cccc_u8_0_3(opcode);

  fastiss::i1_t *branch_taken = gen_condition_satisfied(cond);
 
  //fastiss::i32_t *pc_disp = gen_select(branch_taken, disp9, gen_i32_constant(2));
  fastiss::i32_t *pc_dest = gen_select(branch_taken, gen_i32_constant(trans_PC + disp9), gen_i32_constant(trans_PC + 2));

  //gen_increment_PC(pc_disp);
  gen_write_PC(pc_dest);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  
  // Exit the block if the branch is taken, except for a branch always
  if (!is_condition_always(cond))
  {
    fastiss::block_t *continue_block = create_basic_block();
    gen_cond_branch(branch_taken,  get_exit_block(), continue_block);
    set_gen_point(continue_block);
  }
}

unsigned int rh850g4::b_cccc_disp9_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
