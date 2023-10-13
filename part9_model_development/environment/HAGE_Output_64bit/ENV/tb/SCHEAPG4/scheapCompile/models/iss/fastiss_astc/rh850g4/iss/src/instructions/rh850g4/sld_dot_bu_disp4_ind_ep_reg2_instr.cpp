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

#include "rh850g4/sld_dot_bu_disp4_ind_ep_reg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// sld.bu disp4 [ep], reg2 implementation
//------------------------------------------------------------------------------

rh850g4::sld_dot_bu_disp4_ind_ep_reg2_instr_t::sld_dot_bu_disp4_ind_ep_reg2_instr_t
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

rh850g4::sld_dot_bu_disp4_ind_ep_reg2_instr_t::~sld_dot_bu_disp4_ind_ep_reg2_instr_t()
{
}

void rh850g4::sld_dot_bu_disp4_ind_ep_reg2_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850g4::sld_dot_bu_disp4_ind_ep_reg2_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int disp4 = get_disp4_u32_0_3(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "sld.bu 0x" << std::hex << disp4 << "[ep], " << reg2_name;
  return disass_string.str();
}

unsigned int rh850g4::sld_dot_bu_disp4_ind_ep_reg2_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850g4::sld_dot_bu_disp4_ind_ep_reg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *disp4 = gen_get_disp4_u32_0_3(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  // Load operand registers
  fastiss::i32_t *ep_load = gen_read_r30();

  // Calculate the address to load from
  fastiss::i32_t *address = gen_add(ep_load, disp4);
  
  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x00));

  // Load the byte from memory
  fastiss::i8_t *loaded_val = gen_i8_load(address);

  // Zero-extend the byte
  fastiss::i32_t *result = gen_i32_zext(loaded_val);
    
  // Write the result to reg2
  gen_write_GR(reg2, result);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
