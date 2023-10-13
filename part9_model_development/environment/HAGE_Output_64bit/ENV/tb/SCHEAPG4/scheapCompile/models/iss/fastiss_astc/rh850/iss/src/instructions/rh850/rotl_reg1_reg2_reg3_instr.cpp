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

#include "rh850/rotl_reg1_reg2_reg3_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// rotl reg1, reg2, reg3 implementation
//------------------------------------------------------------------------------

rh850::rotl_reg1_reg2_reg3_instr_t::rotl_reg1_reg2_reg3_instr_t
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

rh850::rotl_reg1_reg2_reg3_instr_t::~rotl_reg1_reg2_reg3_instr_t()
{
}

void rh850::rotl_reg1_reg2_reg3_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::rotl_reg1_reg2_reg3_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
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
  disass_string << "rotl " << reg1_name  << ", " << reg2_name << ", " << reg3_name; 
  return disass_string.str();
}

unsigned int rh850::rotl_reg1_reg2_reg3_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::rotl_reg1_reg2_reg3_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);

  fastiss::block_t *rotl_non_branch = create_basic_block();
  fastiss::block_t *rotl_occur_branch = create_basic_block();  
  fastiss::block_t *continue_block = create_basic_block();

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  fastiss::i32_t *sft       = gen_and(reg1_load, gen_i32_constant(0x1F));
  fastiss::i32_t *tmp1      = gen_shl(reg2_load, sft);
  fastiss::i32_t *sht_r     = gen_sub(gen_i32_constant(31), sft);
  fastiss::i32_t *tmp2      = gen_lshr(reg2_load, sht_r);
  tmp2                      = gen_lshr(tmp2, gen_i32_constant(1));
  fastiss::i32_t *res       = gen_or(tmp1, tmp2);
  fastiss::i32_t *res_bit0  = gen_and(res, gen_i32_constant(1));
  
  // Write the result to reg3
  gen_write_GR(reg3, res);
  
  // Update status flags
  gen_write_PSW_S(gen_calculate_PSW_S(res));
  gen_write_PSW_Z(gen_calculate_PSW_Z(res));
  gen_write_PSW_OV(gen_i1_constant(0));

  gen_cond_branch(gen_icmp_eq(sft, gen_i32_constant(0)), rotl_non_branch, rotl_occur_branch);

  // Handle no rotate case
  set_gen_point(rotl_non_branch);
  gen_write_PSW_CY(gen_i1_constant(0));
  gen_branch(continue_block);

  // Handle rotate occur case
  set_gen_point(rotl_occur_branch);
  gen_write_PSW_CY(gen_icmp_eq(res_bit0, gen_i32_constant(1)));
  gen_branch(continue_block);

  // Continue block
  set_gen_point(continue_block);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
