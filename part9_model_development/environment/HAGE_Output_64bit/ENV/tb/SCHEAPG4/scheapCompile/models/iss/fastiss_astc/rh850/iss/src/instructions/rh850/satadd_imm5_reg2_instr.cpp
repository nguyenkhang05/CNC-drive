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

#include "rh850/satadd_imm5_reg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// satadd imm5, reg2 implementation
//------------------------------------------------------------------------------

rh850::satadd_imm5_reg2_instr_t::satadd_imm5_reg2_instr_t
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

rh850::satadd_imm5_reg2_instr_t::~satadd_imm5_reg2_instr_t()
{
}

void rh850::satadd_imm5_reg2_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850::satadd_imm5_reg2_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  int imm5 = get_imm5_s32_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  if (imm5 < 0)
  {
    disass_string << "satadd -0x" << std::hex << -imm5 << ", " << reg2_name;
  }
  else
  {
    disass_string << "satadd 0x" << std::hex << imm5 << ", " << reg2_name;
  }
  return disass_string.str();
}

unsigned int rh850::satadd_imm5_reg2_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850::satadd_imm5_reg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *imm5 = gen_get_imm5_s32_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  // Perform the ADD
  fastiss::i32_with_carry_overflow_t add_result_struct = gen_add_carry_overflow(reg2_load, imm5);

  // If overflow update result to either int32_max or int32_min
  fastiss::i32_t *sat_value = gen_select(gen_icmp_sgt(imm5, gen_i32_constant(0)), gen_i32_constant(INT32_MAX), gen_i32_constant(INT32_MIN));
  fastiss::i32_t *add_result = gen_select(add_result_struct.overflow, sat_value, add_result_struct.result);

  // Write the result to reg2
  gen_write_GR(reg2, add_result);

  // Determine the CY flag value
  gen_write_PSW_CY(add_result_struct.carry);

  // Determine the OV flag value
  gen_write_PSW_OV(add_result_struct.overflow);

  // Determine the SAT flag value (1 if overflow, else existing value)
  gen_write_PSW_SAT(gen_select(add_result_struct.overflow, gen_i1_constant(1), gen_read_PSW_SAT()));

  // Write the S flag value
  gen_write_PSW_S(gen_calculate_PSW_S(add_result));
  
  // Write the Z flag value
  gen_write_PSW_Z(gen_calculate_PSW_Z(add_result));

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
