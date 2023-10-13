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

#include "rh850/sasf_cccc_reg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// sasf cccc, reg2 implementation
//------------------------------------------------------------------------------

rh850::sasf_cccc_reg2_instr_t::sasf_cccc_reg2_instr_t
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

rh850::sasf_cccc_reg2_instr_t::~sasf_cccc_reg2_instr_t()
{
}

void rh850::sasf_cccc_reg2_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::sasf_cccc_reg2_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char cccc = get_cccc_u8_0_3(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "sasf " << condition_code_name[cccc] << ", " << reg2_name;
  return disass_string.str();
}

unsigned int rh850::sasf_cccc_reg2_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::sasf_cccc_reg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  unsigned char cond = get_cccc_u8_0_3(opcode);

  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);

  fastiss::i1_t *cond_satisfied = gen_condition_satisfied(cond);

  fastiss::i32_t *reg2_lshifted = gen_shl(reg2_load, gen_i32_constant(1));

  // Select data value to store
  fastiss::i32_t *data = gen_select(cond_satisfied, gen_or(reg2_lshifted,gen_i32_constant(1)), gen_or(reg2_lshifted,gen_i32_constant(0)));

  // Write data value to reg2
  gen_write_GR(reg2, data);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
