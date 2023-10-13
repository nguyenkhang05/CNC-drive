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

#include "rh850g4/caxi_ind_reg1_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// caxi [reg1], reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::caxi_ind_reg1_reg2_reg3_instr_t::caxi_ind_reg1_reg2_reg3_instr_t
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

rh850g4::caxi_ind_reg1_reg2_reg3_instr_t::~caxi_ind_reg1_reg2_reg3_instr_t()
{
}

void rh850g4::caxi_ind_reg1_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::caxi_ind_reg1_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "caxi " << "[" << reg1_name << "]" << ", " << reg2_name << ", " << reg3_name;
  return disass_string.str();
}

unsigned int rh850g4::caxi_ind_reg1_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::caxi_ind_reg1_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  // Calculate the address (masking lower 2 bits to zero)
  fastiss::i32_t *addr = gen_and(reg1_load, gen_i32_constant(0xFFFFFFFC));

  // Update mem_acc_type register (CAXI)
  gen_write_mem_acc_type(gen_i32_constant(0x10));

  // Load the word of memory
  fastiss::i32_t *token = gen_i32_load(addr);

  // subtract the word of memory with reg2 value
  fastiss::i32_with_carry_overflow_t result = gen_sub_carry_overflow(reg2_load, token);

  // Compare the result with 0
  fastiss::i1_t *is_zero = gen_icmp_eq(result.result, gen_i32_constant(0));

  // Determine the store value
  fastiss::i32_t *store_val = gen_select(is_zero, gen_read_GR(reg3), token);

  // Store to address
  gen_i32_store(addr, store_val);

  if(is_link_func_enable()){
    // clear the link(monitor) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
  }

  // Write the read word to reg3
  gen_write_GR(reg3, token);

  // Update the CY flag
  gen_write_PSW_CY(result.carry);

  // Update the OV flag
  gen_write_PSW_OV(result.overflow);

  // Update the S flag
  gen_write_PSW_S(gen_calculate_PSW_S(result.result));

  // Update the Z flag
  gen_write_PSW_Z(gen_calculate_PSW_Z(result.result));

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
