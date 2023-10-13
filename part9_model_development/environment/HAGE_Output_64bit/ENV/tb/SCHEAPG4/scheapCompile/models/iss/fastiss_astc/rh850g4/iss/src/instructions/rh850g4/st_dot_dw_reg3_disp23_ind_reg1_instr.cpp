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

#include "rh850g4/st_dot_dw_reg3_disp23_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// st.dw reg3, disp23 [reg1] implementation
//------------------------------------------------------------------------------

rh850g4::st_dot_dw_reg3_disp23_ind_reg1_instr_t::st_dot_dw_reg3_disp23_ind_reg1_instr_t
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

rh850g4::st_dot_dw_reg3_disp23_ind_reg1_instr_t::~st_dot_dw_reg3_disp23_ind_reg1_instr_t()
{
}

void rh850g4::st_dot_dw_reg3_disp23_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(6);  
}

std::string rh850g4::st_dot_dw_reg3_disp23_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  int disp23 = get_disp23_s32_32_47_21_26_shift1(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  if (disp23 < 0)
  {
    disass_string << "st.dw " << reg3_name << ", -0x" << std::hex << -disp23 << "[" << reg1_name << "]";
  }
  else
  {
    disass_string << "st.dw " << reg3_name << ", 0x" << std::hex << disp23 << "[" << reg1_name << "]";
  }
  return disass_string.str();
}

unsigned int rh850g4::st_dot_dw_reg3_disp23_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 6;
}

void rh850g4::st_dot_dw_reg3_disp23_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  fastiss::i32_t *disp23 = gen_get_disp23_s32_32_47_21_26_shift1(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Determine paired register indices
  GR_index_t reg3b = GR_index_t(reg3 + 1);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);
  fastiss::i32_t *reg3_load = gen_read_GR(reg3);
  fastiss::i32_t *reg3b_load = gen_read_GR(reg3b);

  // Calculate the address to store to
  fastiss::i32_t *addr = gen_add(reg1_load, disp23);

  // Calculate the store data
  fastiss::i64_t *data = gen_i64_compose(reg3b_load, reg3_load);

  // Update mem_acc_type register (write)
  gen_write_mem_acc_type(gen_i32_constant(0x07));

  // Store the word from reg2 to memory
  gen_i64_store(addr, data);

  if(is_link_func_enable()){
    // clear the link(monitor) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(8), NULL);
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