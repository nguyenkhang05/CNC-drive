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

#include "rh850/switch_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// switch reg1 implementation
//------------------------------------------------------------------------------

rh850::switch_reg1_instr_t::switch_reg1_instr_t
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

rh850::switch_reg1_instr_t::~switch_reg1_instr_t()
{
}

void rh850::switch_reg1_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850::switch_reg1_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "switch " << reg1_name;
  return disass_string.str();
}

unsigned int rh850::switch_reg1_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850::switch_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *curr_pc = gen_read_PC();

  // Determine the PC of the next instruction
  fastiss::i32_t *table_addr = gen_add(curr_pc, gen_i32_constant(2));

  // Determine the address to load from
  fastiss::i32_t *reg1_shifted = gen_shl(reg1_load, gen_i32_constant(1));
  fastiss::i32_t *address = gen_add(table_addr, reg1_shifted);
   
  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  
  // Check if MPU function enable
  if (is_mdp_enable()){
    fastiss::block_t *continue_block = create_basic_block();
    fastiss::i32_t *mpu_func_result = call_helper(
      "mpu_func_helper", 
      gen_read_core_ptr(), 
      gen_i32_constant(rh850::MPU_KIND_R), 
      gen_i32_constant(rh850::HALF_WORD_SIZE),
      address,
      gen_i32_constant(0x220),
      NULL
    );
    gen_cond_branch(
      gen_icmp_eq(mpu_func_result, gen_i32_constant(1)),
      continue_block,
      get_exit_block()
    );
    set_gen_point(continue_block);
  }


 
  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x01));

  // Load a half-word from the address
  fastiss::i16_t *hword = gen_i16_load(address);
  
  // Sign extend the half-word
  fastiss::i32_t *hword_sext = gen_i32_sext(hword);
  
  // Determine the new PC value
  fastiss::i32_t *hword_shifted = gen_shl(hword_sext, gen_i32_constant(1));
  fastiss::i32_t *new_pc = gen_add(table_addr, hword_shifted);
  gen_write_PC(new_pc);
}

bool rh850::switch_reg1_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850::switch_reg1_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 8;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
