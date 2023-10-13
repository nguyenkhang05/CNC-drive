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

#include "rh850/stsr_regid_reg2_selid_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// stsr regID, reg2, selID implementation
//------------------------------------------------------------------------------

namespace rh850
{
  unsigned int stsr_regid_selid_reg2_helper(rh850::immediate_base_t *instr_base, rh850::SR_SELID_0_index_t regID, unsigned char selid_val)
  {
    unsigned int sr_val = 0;
    switch (selid_val) {
      case 0: {
	sr_val = instr_base->read_SR_SELID_0(regID);
        break;
      }
      case 1: {
	sr_val = instr_base->read_SR_SELID_1((rh850::SR_SELID_1_index_t)regID);
        break;
      }
      case 2: {
	sr_val = instr_base->read_SR_SELID_2((rh850::SR_SELID_2_index_t)regID);
        break;
      }
      case 3: {
	sr_val = instr_base->read_SR_SELID_3((rh850::SR_SELID_3_index_t)regID);
        break;
      }
      // G3K doesn't have selid4's system register
      //case 4: {
	// sr_val = instr_base->read_SR_SELID_4((rh850::SR_SELID_4_index_t)regID);
        //break;
      //}
      case 5: {
	sr_val = instr_base->read_SR_SELID_5((rh850::SR_SELID_5_index_t)regID);
        break;
      }
      case 6: {
	sr_val = instr_base->read_SR_SELID_6((rh850::SR_SELID_6_index_t)regID);
        break;
      }
      case 7: {
	sr_val = instr_base->read_SR_SELID_7((rh850::SR_SELID_7_index_t)regID);
        break;
      }
      // G3K doesn't have selid13's system register
      //case 13: {
	//sr_val = instr_base->read_SR_SELID_13((rh850::SR_SELID_13_index_t)regID);
        //break;
      //}
    }
    return sr_val;
  }
}

rh850::stsr_regid_reg2_selid_instr_t::stsr_regid_reg2_selid_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850::memory_if_t *memory_impl,
  rh850::operand_if_t *operand_impl,
  rh850::cpu_state_if_t *cpu_state_impl,
  rh850::custom_if_t *custom_impl
) :
  rh850::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
  fastiss::func_args_type_t *stsr_regid_selid_reg2_helper_args = code_gen_impl->create_args_type();
  code_gen_impl->append_i32(stsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i32(stsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i8(stsr_regid_selid_reg2_helper_args);
  code_gen_impl->add_helper("stsr_regid_selid_reg2_helper", (void*)&rh850::stsr_regid_selid_reg2_helper, code_gen_impl->create_i32_func(stsr_regid_selid_reg2_helper_args));
  code_gen_impl->delete_args_type(stsr_regid_selid_reg2_helper_args);
}

rh850::stsr_regid_reg2_selid_instr_t::~stsr_regid_reg2_selid_instr_t()
{
}

void rh850::stsr_regid_reg2_selid_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::stsr_regid_reg2_selid_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  SR_SELID_0_index_t regID = get_regID_u8_0_4(opcode);
  std::string regID_name = register_name_SR_SELID_0(regID);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  unsigned char selID = get_selID_u8_27_31(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "stsr 0x" << std::hex << regID << ", " << reg2_name << ", " << (unsigned int)selID;
  return disass_string.str();
}

unsigned int rh850::stsr_regid_reg2_selid_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::stsr_regid_reg2_selid_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  SR_SELID_0_index_t regID = get_regID_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  fastiss::i8_t *selID = gen_get_selID_u8_27_31(opcode);

  fastiss::i8_ptr_t *instr_base = gen_i8_constant_ptr(dynamic_cast<rh850::immediate_base_t*>(this));
  fastiss::i32_t *sr_val = call_helper("stsr_regid_selid_reg2_helper", instr_base, gen_i32_constant(regID), selID, NULL);
  gen_write_GR(reg2, sr_val);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
