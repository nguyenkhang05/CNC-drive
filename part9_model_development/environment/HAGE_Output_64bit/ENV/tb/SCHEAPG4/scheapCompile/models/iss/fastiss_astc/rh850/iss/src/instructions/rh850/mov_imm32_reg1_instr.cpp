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

#include "rh850/mov_imm32_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// mov imm32, reg1 implementation
//------------------------------------------------------------------------------

rh850::mov_imm32_reg1_instr_t::mov_imm32_reg1_instr_t
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

rh850::mov_imm32_reg1_instr_t::~mov_imm32_reg1_instr_t()
{
}

void rh850::mov_imm32_reg1_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(6);  
}

std::string rh850::mov_imm32_reg1_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  int imm32 = get_imm32_s32_16_47(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  if (imm32 < 0)
  {
    disass_string << "mov -0x" << std::hex << -imm32 << ", " << reg1_name;
  }
  else
  {
    disass_string << "mov 0x" << std::hex << imm32 << ", " << reg1_name;
  }
  return disass_string.str();
}

unsigned int rh850::mov_imm32_reg1_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 6;
}

void rh850::mov_imm32_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  fastiss::i32_t *imm32 = gen_get_imm32_s32_16_47(opcode);
  
  // Write the result to reg1
  gen_write_GR(reg1, imm32);
  
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
