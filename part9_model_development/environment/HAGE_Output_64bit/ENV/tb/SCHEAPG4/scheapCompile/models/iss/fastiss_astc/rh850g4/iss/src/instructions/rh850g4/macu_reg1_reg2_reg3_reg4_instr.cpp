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

#include "rh850g4/macu_reg1_reg2_reg3_reg4_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// macu reg1, reg2, reg3, reg4 implementation
//------------------------------------------------------------------------------

rh850g4::macu_reg1_reg2_reg3_reg4_instr_t::macu_reg1_reg2_reg3_reg4_instr_t
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

rh850g4::macu_reg1_reg2_reg3_reg4_instr_t::~macu_reg1_reg2_reg3_reg4_instr_t()
{
}

void rh850g4::macu_reg1_reg2_reg3_reg4_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::macu_reg1_reg2_reg3_reg4_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  GR_index_t reg4 = get_reg4_u8_17_20_shift1(opcode);
  std::string reg4_name = register_name_GR(reg4);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "macu " << reg1_name << ", " << reg2_name << ", " << reg3_name << ", " << reg4_name;
  return disass_string.str();
}

unsigned int rh850g4::macu_reg1_reg2_reg3_reg4_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::macu_reg1_reg2_reg3_reg4_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  GR_index_t reg4 = get_reg4_u8_17_20_shift1(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  fastiss::i32_t *reg3_load = gen_read_GR(reg3);
  fastiss::i32_t *reg3_plus1_load = gen_read_GR(rh850g4::GR_index_t((reg3 + 1) & 0x1f));

  // Calculate result
  fastiss::i64_t *mul_res = gen_mul(gen_i64_zext(reg2_load), gen_i64_zext(reg1_load));  
  fastiss::i64_t *result = gen_add(mul_res, gen_i64_compose(reg3_plus1_load, reg3_load));

  // Write the result
  gen_write_GR(reg4, gen_i32_extract(result, gen_i64_constant(0)));
  gen_write_GR(rh850g4::GR_index_t(reg4+1), gen_i32_extract(result, gen_i64_constant(1)));

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

unsigned int rh850g4::macu_reg1_reg2_reg3_reg4_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
