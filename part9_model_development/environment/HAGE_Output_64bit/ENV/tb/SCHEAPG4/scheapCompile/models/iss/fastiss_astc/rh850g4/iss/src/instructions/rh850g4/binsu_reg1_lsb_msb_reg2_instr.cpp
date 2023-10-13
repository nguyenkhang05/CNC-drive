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

#include "rh850g4/binsu_reg1_lsb_msb_reg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// binsu reg1, lsb, msb, reg2 implementation
//------------------------------------------------------------------------------

rh850g4::binsu_reg1_lsb_msb_reg2_instr_t::binsu_reg1_lsb_msb_reg2_instr_t
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

rh850g4::binsu_reg1_lsb_msb_reg2_instr_t::~binsu_reg1_lsb_msb_reg2_instr_t()
{
}

void rh850g4::binsu_reg1_lsb_msb_reg2_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::binsu_reg1_lsb_msb_reg2_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  unsigned int lsb = get_lsb_u32_27_17_19(opcode);
  unsigned int msb = get_msb_u32_28_31(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  unsigned int width = msb - lsb + 1;

  // Render and return result
  std::ostringstream disass_string;
  disass_string << "bins " << reg1_name <<", " << lsb << ", " << width << ", " << reg2_name; 
  return disass_string.str();
}

unsigned int rh850g4::binsu_reg1_lsb_msb_reg2_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::binsu_reg1_lsb_msb_reg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  fastiss::i32_t *lsb = gen_get_lsb_u32_27_17_19(opcode);
  fastiss::i32_t *msb = gen_get_msb_u32_28_31(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  // bins operation
  fastiss::i32_t *msb_new = gen_add(msb, gen_i32_constant(16));
  fastiss::i32_t *lsb_new = gen_add(lsb, gen_i32_constant(16));
  gen_bins_common(reg1, reg2, msb_new, lsb_new);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
