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

#include "rh850/set1_reg2_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// set1 reg2, [reg1] implementation
//------------------------------------------------------------------------------

rh850::set1_reg2_ind_reg1_instr_t::set1_reg2_ind_reg1_instr_t
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

rh850::set1_reg2_ind_reg1_instr_t::~set1_reg2_ind_reg1_instr_t()
{
}

void rh850::set1_reg2_ind_reg1_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::set1_reg2_ind_reg1_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "set1 " << reg2_name << ", [" << reg1_name << "]";
  return disass_string.str();
}

unsigned int rh850::set1_reg2_ind_reg1_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::set1_reg2_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  
  // Check if MPU function enable
  if (is_mdp_enable()){
    fastiss::block_t *continue_block = create_basic_block();
    fastiss::i32_t *mpu_func_result = call_helper(
      "mpu_func_helper", 
      gen_read_core_ptr(), 
      gen_i32_constant(rh850::MPU_KIND_RMW), 
      gen_i32_constant(rh850::BYTE_SIZE),
      reg1_load,
      gen_i32_constant(0x112),
      NULL
    );
    gen_cond_branch(
      gen_icmp_eq(mpu_func_result, gen_i32_constant(1)),
      continue_block,
      get_exit_block()
    );
    set_gen_point(continue_block);
  }
  // Update mem_acc_type register (BitOp-Set)
  gen_write_mem_acc_type(gen_i32_constant(0x08));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Load the byte of memory
  fastiss::i8_t *byte = gen_i8_load(reg1_load);

  // Determine the bit to test
  fastiss::i8_t *bit_index = gen_and(gen_i8_trunc(reg2_load), gen_i8_constant(0x07));

  // Determine the bit mask
  fastiss::i8_t *bit_mask = gen_shl(gen_i8_constant(0x1), bit_index);

  // Determine the result of the test
  fastiss::i8_t *result = gen_and(byte, bit_mask);;
  
  // Update the Z flag
  gen_write_PSW_Z(gen_calculate_PSW_Z(gen_i32_zext(result)));

  // Set the bit and store back to memory
  result = gen_or(byte, bit_mask);
  gen_i8_store(reg1_load, result);
    
  if(is_link_func_enable()){
    // clear the link(monitor) information
    call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), reg1_load, gen_i32_constant(1), NULL);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}

unsigned int rh850::set1_reg2_ind_reg1_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
