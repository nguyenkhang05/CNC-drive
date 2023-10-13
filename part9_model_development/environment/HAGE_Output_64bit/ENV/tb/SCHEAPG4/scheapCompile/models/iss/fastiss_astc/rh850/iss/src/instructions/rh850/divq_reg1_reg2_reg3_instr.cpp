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

#include "rh850/divq_reg1_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// divq reg1, reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850::divq_reg1_reg2_reg3_instr_t::divq_reg1_reg2_reg3_instr_t
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

rh850::divq_reg1_reg2_reg3_instr_t::~divq_reg1_reg2_reg3_instr_t()
{
}

void rh850::divq_reg1_reg2_reg3_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::divq_reg1_reg2_reg3_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
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
  disass_string << "divq " << reg1_name << ", " << reg2_name << ", " << reg3_name;
  return disass_string.str();
}

unsigned int rh850::divq_reg1_reg2_reg3_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::divq_reg1_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  // Perform div operation
  gen_div_reg1_reg2_reg3(reg1_load, reg2, reg3);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Determine the number of cycles
  // Cycles = N + 3 where N = ((dividend valid bits) - (divisor valid bits))/2 + 1
  // If the result for N < 1, N becomes 1. Division by 0 leads to N being 0. The range of N is from 0 to 16.
  fastiss::i32_t *one = gen_i32_constant(1);
  fastiss::i32_t *abs_reg1_load = gen_select(gen_icmp_slt(reg1_load, gen_i32_constant(0)), gen_neg(reg1_load), reg1_load);
  fastiss::i32_t *abs_reg2_load = gen_select(gen_icmp_slt(reg2_load, gen_i32_constant(0)), gen_neg(reg2_load), reg2_load);
  fastiss::i32_t *dividend_valid_bits = gen_sub(gen_i32_constant(32), gen_count_lead_zero(abs_reg2_load));
  fastiss::i32_t *divisor_valid_bits = gen_sub(gen_i32_constant(32), gen_count_lead_zero(abs_reg1_load));
  fastiss::i1_t *is_divisor_zero = gen_icmp_eq(reg1_load, gen_i32_constant(0));
  fastiss::i32_t *n = gen_add(gen_ashr(gen_sub(dividend_valid_bits, divisor_valid_bits), one), one);
  n = gen_select(gen_icmp_slt(n, one), one, n);
  n = gen_select(is_divisor_zero, gen_i32_constant(0), n);
  fastiss::i32_t *num_cycles = gen_add(n, gen_i32_constant(3));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(num_cycles);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------