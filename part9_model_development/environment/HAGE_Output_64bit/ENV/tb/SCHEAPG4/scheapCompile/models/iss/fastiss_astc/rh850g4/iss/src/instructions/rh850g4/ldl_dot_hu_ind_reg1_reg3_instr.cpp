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

#include "rh850g4/ldl_dot_hu_ind_reg1_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ldl.hu [reg1], reg3 implementation
//------------------------------------------------------------------------------

rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t::ldl_dot_hu_ind_reg1_reg3_instr_t
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

rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t::~ldl_dot_hu_ind_reg1_reg3_instr_t()
{
}

void rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "ldl.hu [" << reg1_name <<"], " <<reg3_name; 
  return disass_string.str();
}

unsigned int rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Calculate the address to load from
  fastiss::i32_t *address = gen_read_GR(reg1);

  // Update mem_acc_type register (Execution Transfer)
  gen_write_mem_acc_type(gen_i32_constant(0x14));

  // Load the byte from memory
  fastiss::i16_t *loaded_val = gen_i16_load(address);

  // Zero extend the loaded value
  fastiss::i32_t *zext_val = gen_i32_zext(loaded_val);
  
  // Write the result to reg3
  gen_write_GR(reg3, zext_val);

  if(is_link_func_enable()){
    // Set the link information
    // temp code !!!!!!! need re-check for Link
    call_helper("set_link_helper", gen_read_link_manager_ptr(), gen_read_PEID(), address, gen_i32_constant(2), NULL);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

unsigned int rh850g4::ldl_dot_hu_ind_reg1_reg3_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 11;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
