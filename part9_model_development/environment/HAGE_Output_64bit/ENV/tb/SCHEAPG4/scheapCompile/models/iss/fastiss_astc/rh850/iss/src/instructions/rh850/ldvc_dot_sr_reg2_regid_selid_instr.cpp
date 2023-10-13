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

#include "rh850/ldvc_dot_sr_reg2_regid_selid_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ldvc.sr reg2, regID, selID implementation
//------------------------------------------------------------------------------

rh850::ldvc_dot_sr_reg2_regid_selid_instr_t::ldvc_dot_sr_reg2_regid_selid_instr_t
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

rh850::ldvc_dot_sr_reg2_regid_selid_instr_t::~ldvc_dot_sr_reg2_regid_selid_instr_t()
{
}

void rh850::ldvc_dot_sr_reg2_regid_selid_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::ldvc_dot_sr_reg2_regid_selid_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_0_4(opcode);
  std::string reg2_name = register_name_GR(reg2);
  SR_SELID_0_index_t regID = get_regID_u8_11_15(opcode);
  std::string regID_name = register_name_SR_SELID_0(regID);
  unsigned char selID = get_selID_u8_27_31(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "ldvc.sr " << reg2_name << ", sr" << std::dec << regID << ", " << (unsigned int)selID; 
  return disass_string.str();
}

unsigned int rh850::ldvc_dot_sr_reg2_regid_selid_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::ldvc_dot_sr_reg2_regid_selid_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_0_4(opcode);
  SR_SELID_0_index_t regID = get_regID_u8_11_15(opcode);
  fastiss::i8_t *selID = gen_get_selID_u8_27_31(opcode);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
