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

#include "rh850/b_cccc_disp17_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// bcccc disp17 implementation
//------------------------------------------------------------------------------

rh850::b_cccc_disp17_instr_t::b_cccc_disp17_instr_t
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

rh850::b_cccc_disp17_instr_t::~b_cccc_disp17_instr_t()
{
}

void rh850::b_cccc_disp17_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char cccc = get_cccc_u8_0_3(opcode);
  int disp17 = get_disp17_s32_4_17_31_shift1(opcode);
  
  // Increment PC
  unsigned int pc_increment = 4;
  if (is_condition_always(cccc))
  {
    // For branch always, simply continue translating at the new PC
    pc_increment = disp17;
  }
  
  // Increment PC
  increment_trans_PC(pc_increment);  
}

std::string rh850::b_cccc_disp17_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char cccc = get_cccc_u8_0_3(opcode);
  int disp17 = get_disp17_s32_4_17_31_shift1(opcode);
  int branch_pc = read_trans_PC() + disp17;
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << branch_condition_code_name[cccc] << " 0x" << std::hex << branch_pc;
  return disass_string.str();
}

unsigned int rh850::b_cccc_disp17_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::b_cccc_disp17_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *disp17 = gen_get_disp17_s32_4_17_31_shift1(opcode);

  unsigned char cond = get_cccc_u8_0_3(opcode);


  fastiss::i1_t *branch_taken = gen_condition_satisfied(cond);

  fastiss::i32_t *pc_disp = gen_select(branch_taken, disp17, gen_i32_constant(4));

  // Update PC
  gen_increment_PC(pc_disp);

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



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
