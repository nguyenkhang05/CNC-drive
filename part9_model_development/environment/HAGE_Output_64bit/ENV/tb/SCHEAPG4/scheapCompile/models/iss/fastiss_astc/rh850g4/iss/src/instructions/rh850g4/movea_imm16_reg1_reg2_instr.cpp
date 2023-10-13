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

#include "rh850g4/movea_imm16_reg1_reg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// movea imm16, reg1, reg2 implementation
//------------------------------------------------------------------------------

rh850g4::movea_imm16_reg1_reg2_instr_t::movea_imm16_reg1_reg2_instr_t
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

rh850g4::movea_imm16_reg1_reg2_instr_t::~movea_imm16_reg1_reg2_instr_t()
{
}

void rh850g4::movea_imm16_reg1_reg2_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::movea_imm16_reg1_reg2_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  int imm16 = get_imm16_s32_16_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  if (imm16 < 0)
  {
    disass_string << "movea -0x" << std::hex << -imm16 << ", " << reg1_name << ", " << reg2_name;
  }
  else
  {
    disass_string << "movea 0x" << std::hex << imm16 << ", " << reg1_name << ", " << reg2_name;
  }
  return disass_string.str();
}

unsigned int rh850g4::movea_imm16_reg1_reg2_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::movea_imm16_reg1_reg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *imm16 = gen_get_imm16_s32_16_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  //fastiss::i32_t *loaded_val = gen_i32_load(gen_i32_constant(0xd14));

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);

  // Determine the add result
  fastiss::i32_t *add_result = gen_add(reg1_load, imm16);

  // Write the result to reg1
  gen_write_GR(reg2, add_result);
  
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
