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

#include "rh850/ld_dot_b_disp16_ind_reg1_reg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ld.b disp16 [reg1], reg2 implementation
//------------------------------------------------------------------------------

rh850::ld_dot_b_disp16_ind_reg1_reg2_instr_t::ld_dot_b_disp16_ind_reg1_reg2_instr_t
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

rh850::ld_dot_b_disp16_ind_reg1_reg2_instr_t::~ld_dot_b_disp16_ind_reg1_reg2_instr_t()
{
}

void rh850::ld_dot_b_disp16_ind_reg1_reg2_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::ld_dot_b_disp16_ind_reg1_reg2_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  int disp16 = get_disp16_s32_16_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  if (disp16 < 0)
  {
    disass_string << "ld.b -0x" << std::hex << -disp16 << "[" << reg1_name << "], " << reg2_name;
  }
  else
  {
    disass_string << "ld.b 0x" << std::hex << disp16 << "[" << reg1_name << "], " << reg2_name;
  }
  return disass_string.str();
}

unsigned int rh850::ld_dot_b_disp16_ind_reg1_reg2_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::ld_dot_b_disp16_ind_reg1_reg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *disp16 = gen_get_disp16_s32_16_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  // Load operand registers
  fastiss::i32_t *reg1_load = gen_read_GR(reg1);

  // Calculate the address to load from
  fastiss::i32_t *address = gen_add(reg1_load, disp16);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  // Check if MPU function enable
  if (is_mdp_enable()){
    fastiss::block_t *continue_block = create_basic_block();
    fastiss::i32_t *mpu_func_result = call_helper(
      "mpu_func_helper", 
      gen_read_core_ptr(), 
      gen_i32_constant(rh850::MPU_KIND_R), 
      gen_i32_constant(rh850::BYTE_SIZE),
      address,
      gen_or(gen_shl(gen_i32_constant(reg2), gen_i32_constant(16)), gen_i32_constant(0x2)),
      NULL
    );
    gen_cond_branch(
      gen_icmp_eq(mpu_func_result, gen_i32_constant(1)),
      continue_block,
      get_exit_block()
    );
    set_gen_point(continue_block);
  }
  
  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x00));

  // Load the byte from memory
  fastiss::i8_t *loaded_val = gen_i8_load(address);

  // Sign extend the loaded value
  fastiss::i32_t *sext_val = gen_i32_sext(loaded_val);
  
  // Write the result to reg2
  gen_write_GR(reg2, sext_val);
  
  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
