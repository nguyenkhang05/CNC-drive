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

#include "rh850/loop_reg1_disp16_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// loop reg1, disp16 implementation
//------------------------------------------------------------------------------

rh850::loop_reg1_disp16_instr_t::loop_reg1_disp16_instr_t
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

rh850::loop_reg1_disp16_instr_t::~loop_reg1_disp16_instr_t()
{
}

void rh850::loop_reg1_disp16_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::loop_reg1_disp16_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  unsigned int disp16 = get_disp16_u32_17_31_shift1(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "loop " << reg1_name << ", 0x" << std::hex << disp16; 
  return disass_string.str();
}

unsigned int rh850::loop_reg1_disp16_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::loop_reg1_disp16_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  fastiss::i32_t *disp16 = gen_get_disp16_u32_17_31_shift1(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_with_carry_overflow_t add_result_struct = gen_add_carry_overflow(reg1_load, gen_i32_constant(0xffffffff));
  fastiss::i32_t *add_result = add_result_struct.result;

  // Write the result to reg2
  gen_write_GR(reg1, add_result);

  // Determine the CY flag value
  gen_write_PSW_CY(add_result_struct.carry);

  // Determine the OV flag value
  gen_write_PSW_OV(add_result_struct.overflow);

  // Write the S flag value
  gen_write_PSW_S(gen_calculate_PSW_S(add_result));
  
  // Write the Z flag value
  gen_write_PSW_Z(gen_calculate_PSW_Z(add_result));

  fastiss::i1_t *branch_taken = gen_icmp_eq(add_result, gen_i32_constant(0));

  fastiss::i32_t *pc_disp = gen_select(branch_taken, gen_i32_constant(4), gen_neg(disp16));

  // Update PC
  gen_increment_PC(pc_disp);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Exit the block if the branch is taken, except for a branch always
  fastiss::block_t *continue_block = create_basic_block();
  gen_cond_branch(branch_taken, continue_block, get_exit_block());
  set_gen_point(continue_block);
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
