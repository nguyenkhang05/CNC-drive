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

#include "rh850/mulh_reg1_reg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// mulh reg1, reg2 implementation
//------------------------------------------------------------------------------

rh850::mulh_reg1_reg2_instr_t::mulh_reg1_reg2_instr_t
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

rh850::mulh_reg1_reg2_instr_t::~mulh_reg1_reg2_instr_t()
{
}

void rh850::mulh_reg1_reg2_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850::mulh_reg1_reg2_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "mulh " << reg1_name << ", " << reg2_name;
  return disass_string.str();
}

unsigned int rh850::mulh_reg1_reg2_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850::mulh_reg1_reg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  // Extract lower halfword of both reg1 and reg2
  fastiss::i16_t *reg2_lower_hw = gen_i16_extract(reg2_load, gen_i32_constant(0));
  fastiss::i16_t *reg1_lower_hw = gen_i16_extract(reg1_load, gen_i32_constant(0));

  // Determine the MUL result
  fastiss::i32_t *mul_result = gen_mul(gen_i32_sext(reg2_lower_hw), gen_i32_sext(reg1_lower_hw));

  // Write the result to reg2
  gen_write_GR(reg2, mul_result);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
