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

#include "rh850g4/sst_dot_h_reg2_disp8_ind_ep_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// sst.h reg2, disp8 [ep] implementation
//------------------------------------------------------------------------------

rh850g4::sst_dot_h_reg2_disp8_ind_ep_instr_t::sst_dot_h_reg2_disp8_ind_ep_instr_t
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

rh850g4::sst_dot_h_reg2_disp8_ind_ep_instr_t::~sst_dot_h_reg2_disp8_ind_ep_instr_t()
{
}

void rh850g4::sst_dot_h_reg2_disp8_ind_ep_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850g4::sst_dot_h_reg2_disp8_ind_ep_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int disp8 = get_disp8_u32_0_6_shift1(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "sst.h " << reg2_name << ", 0x" << std::hex << disp8 << "[ep]";
  return disass_string.str();
}

unsigned int rh850g4::sst_dot_h_reg2_disp8_ind_ep_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850g4::sst_dot_h_reg2_disp8_ind_ep_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *disp8 = gen_get_disp8_u32_0_6_shift1(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  fastiss::i32_t *ep_load = gen_read_r30();

  // Calculate the address
  fastiss::i32_t *addr = gen_add(ep_load, disp8);

  // Determine the half-word to store
  fastiss::i16_t *hword = gen_i16_trunc(reg2_load);

  // Update mem_acc_type register (write)
  gen_write_mem_acc_type(gen_i32_constant(0x05));

  // Store the half-word from reg2 to memory
  gen_i16_store(addr, hword);

  if(is_link_func_enable()){
    // clear the link(monitor) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(2), NULL);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
