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

#include "rh850g4/tst1_reg2_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// tst1 reg2, [reg1] implementation
//------------------------------------------------------------------------------

rh850g4::tst1_reg2_ind_reg1_instr_t::tst1_reg2_ind_reg1_instr_t
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

rh850g4::tst1_reg2_ind_reg1_instr_t::~tst1_reg2_ind_reg1_instr_t()
{
}

void rh850g4::tst1_reg2_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::tst1_reg2_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "tst1 " << reg2_name << ", [" << reg1_name << "]";
  return disass_string.str();
}

unsigned int rh850g4::tst1_reg2_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::tst1_reg2_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  
  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x00));

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
    
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
