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

#include "rh850g4/dispose_imm5_list12_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// dispose imm5, list12, [reg1] implementation
//------------------------------------------------------------------------------

rh850g4::dispose_imm5_list12_ind_reg1_instr_t::dispose_imm5_list12_ind_reg1_instr_t
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

rh850g4::dispose_imm5_list12_ind_reg1_instr_t::~dispose_imm5_list12_ind_reg1_instr_t()
{
}

void rh850g4::dispose_imm5_list12_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::dispose_imm5_list12_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int imm5 = get_imm5_u32_1_5_shift2(opcode);
  unsigned short list12 = get_list12_u16_21_31_0(opcode);
  GR_index_t reg1 = get_reg1_u8_16_20(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  bool list12_empty = true;
  disass_string << "dispose 0x" << std::hex << imm5;
  for (int list_id = 0; list_id < 12; list_id++)
  {
    if (list12 & prepare_dispose_rx_mask[list_id])
    {
      disass_string << ", " << register_name_GR(GR_index_t(list_id + 20));
      list12_empty = false;
    }
  }
  if (list12_empty)
  {
    // If there are no registers in the list12, then write 0x0
    disass_string << ", 0x0";
  }
  disass_string << ", [" << reg1_name << "]";
  return disass_string.str();
}

unsigned int rh850g4::dispose_imm5_list12_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::dispose_imm5_list12_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *imm5 = gen_get_imm5_u32_1_5_shift2(opcode);
  unsigned short list12 = get_list12_u16_21_31_0(opcode);
  GR_index_t reg1 = get_reg1_u8_16_20(opcode);
  
  // Load SP registers
  fastiss::i32_t *sp_val = gen_read_r3();

  // Update the stack pointer value
  sp_val = gen_add(sp_val, imm5);

  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x02));

  // Iterate over all registers in the range r20-r31
  for (int list_id = 11; list_id >= 0; list_id--)
  {
    if (list12 & prepare_dispose_rx_mask[list_id])
    {
      // Pop a value from the stack and write to the register if the register
      // is selected. The load address has the lower 2-bits masked to 0.
      fastiss::i32_t *masked_sp_val = gen_and(sp_val, gen_i32_constant(0xfffffffc));
      fastiss::i32_t *loaded_val = gen_i32_load(masked_sp_val);
      gen_write_GR(GR_index_t(list_id + 20), loaded_val);  
      sp_val = gen_add(sp_val, gen_i32_constant(4));
    }
  }

  // Update the stack pointer value
  gen_write_r3(sp_val);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);

  // Update PC
  gen_write_PC(reg1_load);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

bool rh850g4::dispose_imm5_list12_ind_reg1_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850g4::dispose_imm5_list12_ind_reg1_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  unsigned short list12 = get_list12_u16_21_31_0(opcode);

  // Determine the number of set bits in the register list
  unsigned int ones_count = 0;
  for (int list_id = 11; list_id >= 0; list_id--)
  {
    if (list12 & prepare_dispose_rx_mask[list_id])
    {
      ones_count++;
    }
  }
  // Two registers can be written per cycle, with a minimum of 1 cycle.
  // Two additional cycles are added due to the PC change.
  unsigned int return_value = (ones_count + 1) / 2;
  return (return_value ? (return_value + 2) : 3);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
