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

#include "rh850/sst_dot_w_reg2_disp8_ind_ep_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// sst.w reg2, disp8 [ep] implementation
//------------------------------------------------------------------------------

rh850::sst_dot_w_reg2_disp8_ind_ep_instr_t::sst_dot_w_reg2_disp8_ind_ep_instr_t
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

rh850::sst_dot_w_reg2_disp8_ind_ep_instr_t::~sst_dot_w_reg2_disp8_ind_ep_instr_t()
{
}

void rh850::sst_dot_w_reg2_disp8_ind_ep_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850::sst_dot_w_reg2_disp8_ind_ep_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int disp8 = get_disp8_u32_1_6_shift2(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "sst.w " << reg2_name << ", 0x" << std::hex << disp8 << "[ep]";
  return disass_string.str();
}

unsigned int rh850::sst_dot_w_reg2_disp8_ind_ep_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850::sst_dot_w_reg2_disp8_ind_ep_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *disp8 = gen_get_disp8_u32_1_6_shift2(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);

  // Load operand registers
  fastiss::i32_t *reg2_load = gen_read_GR(reg2);
  fastiss::i32_t *ep_load = gen_read_r30();

  // Calculate the address
  fastiss::i32_t *addr = gen_add(ep_load, disp8);
  
  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  
  // Check if MPU function enable
  if (is_mdp_enable()){
    fastiss::block_t *continue_block = create_basic_block();
    fastiss::i32_t *mpu_func_result = call_helper(
      "mpu_func_helper", 
      gen_read_core_ptr(), 
      gen_i32_constant(rh850::MPU_KIND_W), 
      gen_i32_constant(rh850::WORD_SIZE),
      addr,
      gen_or(gen_shl(gen_i32_constant(30), gen_i32_constant(16)), gen_i32_constant(0x401)),
      NULL
    );
    gen_cond_branch(
      gen_icmp_eq(mpu_func_result, gen_i32_constant(1)),
      continue_block,
      get_exit_block()
    );
    set_gen_point(continue_block);
  }



  // Update mem_acc_type register (write)
  gen_write_mem_acc_type(gen_i32_constant(0x06));

  // Store the word from reg2 to memory
  gen_i32_store(addr, reg2_load);

  if(is_link_func_enable()){
    // clear the link(monitor) information
    call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), addr, gen_i32_constant(4), NULL);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
