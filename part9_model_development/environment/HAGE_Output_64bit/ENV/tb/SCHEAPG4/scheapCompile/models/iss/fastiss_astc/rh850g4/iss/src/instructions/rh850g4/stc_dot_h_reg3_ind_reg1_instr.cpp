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

#include "rh850g4/stc_dot_h_reg3_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// stc.h reg3, [reg1] implementation
//------------------------------------------------------------------------------

rh850g4::stc_dot_h_reg3_ind_reg1_instr_t::stc_dot_h_reg3_ind_reg1_instr_t
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

rh850g4::stc_dot_h_reg3_ind_reg1_instr_t::~stc_dot_h_reg3_ind_reg1_instr_t()
{
}

void rh850g4::stc_dot_h_reg3_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::stc_dot_h_reg3_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "stc.h " << reg3_name << ", [" << reg1_name <<"]"; 
  return disass_string.str();
}

unsigned int rh850g4::stc_dot_h_reg3_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::stc_dot_h_reg3_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Load operand registers
  fastiss::i32_t *reg3_load = gen_read_GR(reg3);

  // Calculate the address
  fastiss::i32_t *addr = gen_read_GR(reg1);

  // Create sub-blocks
  fastiss::block_t *link_enable_branch = create_basic_block();
  fastiss::block_t *link_disable_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // clear the link information
  call_helper("force_wait_helper", gen_read_core_ptr(), NULL);

  fastiss::i32_t *link_stat = gen_i32_constant(0);
  if(is_link_func_enable()){
    // Get the link information and clear the link
    // temp code !!!!!!! need re-check for Link
    link_stat = call_helper("is_link_valid_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(2), NULL);
  }

  // Check if the operand is a denormaled value and FPSR.FS=0
  gen_cond_branch(gen_icmp_eq(link_stat, gen_i32_constant(1)), link_enable_branch, link_disable_branch);

  // Handled link enable case
  set_gen_point(link_enable_branch);

  // Update mem_acc_type register (Execution Transfer)
  gen_write_mem_acc_type(gen_i32_constant(0x15));

  // Determine the byte to store
  fastiss::i16_t *hword = gen_i16_trunc(reg3_load);

  // Store the word from reg2 to memory
  gen_i16_store(addr, hword);

  // Add an additional 3 cycles as the store has succeeded
  gen_increment_cycle_count(gen_i32_constant(3));

  // Write the result to reg3
  gen_write_GR(reg3, gen_i32_constant(1));
  gen_branch(continue_block);

  // Handled link disable case
  set_gen_point(link_disable_branch);

  // Write the result to reg3
  gen_write_GR(reg3, gen_i32_constant(0));
  gen_branch(continue_block);

  // Continue block
  set_gen_point(continue_block);

  if(is_link_func_enable()){
    // clear the link information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_link_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(2), NULL);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

unsigned int rh850g4::stc_dot_h_reg3_ind_reg1_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 8;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
