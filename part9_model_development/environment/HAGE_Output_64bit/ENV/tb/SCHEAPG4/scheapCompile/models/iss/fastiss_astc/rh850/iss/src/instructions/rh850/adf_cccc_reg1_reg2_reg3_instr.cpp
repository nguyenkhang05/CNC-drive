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

#include "rh850/adf_cccc_reg1_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// adf cccc, reg1, reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850::adf_cccc_reg1_reg2_reg3_instr_t::adf_cccc_reg1_reg2_reg3_instr_t
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

rh850::adf_cccc_reg1_reg2_reg3_instr_t::~adf_cccc_reg1_reg2_reg3_instr_t()
{
}

void rh850::adf_cccc_reg1_reg2_reg3_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::adf_cccc_reg1_reg2_reg3_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char cccc = get_cccc_u8_17_20(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);

  std::ostringstream disass_string;  
  disass_string << "adf " << condition_code_name[cccc] << ", " << reg1_name << ", " << reg2_name << ", " << reg3_name;
  return disass_string.str();
}

unsigned int rh850::adf_cccc_reg1_reg2_reg3_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::adf_cccc_reg1_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  unsigned char cond = get_cccc_u8_17_20(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  fastiss::i32_with_carry_overflow_t sum_struct = gen_add_carry_overflow(reg1_load, reg2_load);
  fastiss::i32_t *sum = sum_struct.result;

  fastiss::i1_t *cond_satisfied = gen_condition_satisfied(cond);

  // Select whether 1 or 0 is to be added to sum
  fastiss::i32_t *additional = gen_select(cond_satisfied, gen_i32_constant(1), gen_i32_constant(0));

  // Calculate result
  fastiss::i32_with_carry_overflow_t result_struct = gen_add_carry_overflow(sum, additional);
  fastiss::i32_t *result = result_struct.result;

  // Write data value to reg3
  gen_write_GR(reg3, result);

  // Determine the CY flag value
  gen_write_PSW_CY(gen_or(sum_struct.carry, result_struct.carry));

  // Determine the OV flag value
  gen_write_PSW_OV(gen_or(sum_struct.overflow, result_struct.overflow));

  // Write the S flag value
  gen_write_PSW_S(gen_calculate_PSW_S(result));
  
  // Write the Z flag value
  gen_write_PSW_Z(gen_calculate_PSW_Z(result));

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
