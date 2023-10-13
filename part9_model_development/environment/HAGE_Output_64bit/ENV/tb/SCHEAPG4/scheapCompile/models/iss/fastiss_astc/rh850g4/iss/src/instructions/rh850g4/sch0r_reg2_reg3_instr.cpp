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

#include "rh850g4/sch0r_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// sch0r reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850g4::sch0r_reg2_reg3_instr_t::sch0r_reg2_reg3_instr_t
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

rh850g4::sch0r_reg2_reg3_instr_t::~sch0r_reg2_reg3_instr_t()
{
}

void rh850g4::sch0r_reg2_reg3_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::sch0r_reg2_reg3_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "sch0r " << reg2_name << ", " << reg3_name;
  return disass_string.str();
}

unsigned int rh850g4::sch0r_reg2_reg3_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::sch0r_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  fastiss::i32_t *trailing_ones = gen_count_trail_one(reg2_load);
  fastiss::i32_t *result = gen_add(trailing_ones, gen_i32_constant(1));

  fastiss::i1_t *found_bit0 = gen_icmp_ne(result, gen_i32_constant(33)); // Result is 33 if not found

  fastiss::i1_t *bit31_zero = gen_icmp_eq(result, gen_i32_constant(32)); // Result is 32 if only bit31 is 0

  result = gen_select(found_bit0, result, gen_i32_constant(0)); // We want zero instead of 33 if not found

  // Write the result to reg3
  gen_write_GR(reg3, result);

  // Determine the CY flag value
  gen_write_PSW_CY(bit31_zero);

  // Determine the OV flag value
  gen_write_PSW_OV(gen_i1_constant(0));

  // Write the S flag value
  gen_write_PSW_S(gen_i1_constant(0));

  // Write the Z flag value
  gen_write_PSW_Z(gen_not(found_bit0));

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
