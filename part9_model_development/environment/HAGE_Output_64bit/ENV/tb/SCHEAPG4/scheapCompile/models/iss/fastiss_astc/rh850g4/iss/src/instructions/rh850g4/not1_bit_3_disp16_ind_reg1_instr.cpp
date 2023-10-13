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

#include "rh850g4/not1_bit_3_disp16_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// not1 bit#3, disp16 [reg1] implementation
//------------------------------------------------------------------------------

rh850g4::not1_bit_3_disp16_ind_reg1_instr_t::not1_bit_3_disp16_ind_reg1_instr_t
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

rh850g4::not1_bit_3_disp16_ind_reg1_instr_t::~not1_bit_3_disp16_ind_reg1_instr_t()
{
}

void rh850g4::not1_bit_3_disp16_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::not1_bit_3_disp16_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char bit = get_bit_u8_11_13(opcode);
  int disp16 = get_disp16_s32_16_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  if (disp16 < 0)
  {
    disass_string << "not1 0x" << (unsigned int)bit << ", -0x" << std::hex << -disp16 << "[" << reg1_name << "]";
  }
  else
  {
    disass_string << "not1 0x" << (unsigned int)bit << ", 0x" << std::hex << disp16 << "[" << reg1_name << "]";
  }
  return disass_string.str();
}

unsigned int rh850g4::not1_bit_3_disp16_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::not1_bit_3_disp16_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i8_t *bit = gen_get_bit_u8_11_13(opcode);
  fastiss::i32_t *disp16 = gen_get_disp16_s32_16_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);

  // Calculate the address
  fastiss::i32_t *addr = gen_add(reg1_load, disp16);
  
  // Update mem_acc_type register (BitOp-Xor)
  gen_write_mem_acc_type(gen_i32_constant(0x0b));

  // Load the byte of memory
  fastiss::i8_t *byte = gen_i8_load(addr);
  
  // Determine the bit mask
  fastiss::i8_t *bit_mask = gen_shl(gen_i8_constant(0x1), bit);

  // Determine the result of the test
  fastiss::i8_t *result = gen_and(byte, bit_mask);;
  
  // Update the Z flag
  gen_write_PSW_Z(gen_calculate_PSW_Z(gen_i32_zext(result)));

  // Not the bit and store back to memory
  fastiss::i8_t *bit_set_result = gen_or(byte, bit_mask);
  fastiss::i8_t *bit_clr_result = gen_and(byte, gen_not(bit_mask));
  fastiss::i8_t *bit_not_result = gen_select(gen_read_PSW_Z(), bit_set_result, bit_clr_result);
  gen_i8_store(addr, bit_not_result);
  
  if(is_link_func_enable()){
    // clear the link(monitor) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(1), NULL);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
