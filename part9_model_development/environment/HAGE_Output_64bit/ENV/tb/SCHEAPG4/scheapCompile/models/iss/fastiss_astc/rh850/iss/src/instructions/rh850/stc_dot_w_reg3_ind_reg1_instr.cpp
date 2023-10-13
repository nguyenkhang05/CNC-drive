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

#include "rh850/stc_dot_w_reg3_ind_reg1_instr.h"
#include "rh850/core.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// stc.w reg3, [reg1] implementation
//------------------------------------------------------------------------------

rh850::stc_dot_w_reg3_ind_reg1_instr_t::stc_dot_w_reg3_ind_reg1_instr_t
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

rh850::stc_dot_w_reg3_ind_reg1_instr_t::~stc_dot_w_reg3_ind_reg1_instr_t()
{
}

void rh850::stc_dot_w_reg3_ind_reg1_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::stc_dot_w_reg3_ind_reg1_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  std::string reg3_name = register_name_GR(reg3);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "stc.w " << reg3_name << ", [" << reg1_name <<"]"; 
  return disass_string.str();
}

unsigned int rh850::stc_dot_w_reg3_ind_reg1_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::stc_dot_w_reg3_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg3 = get_reg3_u8_27_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Load operand registers
  fastiss::i32_t *reg3_load = gen_read_GR(reg3);

  // Calculate the address
  fastiss::i32_t *addr = gen_read_GR(reg1);
  
  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  
  // Check if MPU function enable
  if (is_mdp_enable()){
    fastiss::block_t *continue_block1 = create_basic_block();
    fastiss::i32_t *mpu_func_result = call_helper(
      "mpu_func_helper", 
      gen_read_core_ptr(), 
      gen_i32_constant(rh850::MPU_KIND_W), 
      gen_i32_constant(rh850::WORD_SIZE),
      addr,
      gen_or(gen_shl(gen_i32_constant(reg1), gen_i32_constant(16)), gen_i32_constant(0x40f)),
      NULL
    );
    gen_cond_branch(
      gen_icmp_eq(mpu_func_result, gen_i32_constant(1)),
      continue_block1,
      get_exit_block()
    );
    set_gen_point(continue_block1);
  }


  // Create sub-blocks
  fastiss::block_t *continue_block2 = create_basic_block();

  // clear the link information
  call_helper("force_wait_helper", gen_read_core_ptr(), NULL);

  if(get_cpu_milestone() == rh850::core::FASTISS_G3K_PCU){
    // Update mem_acc_type register (Execution Transfer)
    gen_write_mem_acc_type(gen_i32_constant(0x06));
    // Store the word from reg2 to memory
    gen_i32_store(addr, reg3_load);
    gen_branch(continue_block2);
  }else{
    fastiss::block_t *link_enable_branch = create_basic_block();
    fastiss::block_t *link_disable_branch = create_basic_block();

    fastiss::i32_t *link_stat = gen_i32_constant(0);
    if(is_link_func_enable()){
      // Get the link information and clear the link
      link_stat = call_helper("is_link_valid_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), addr, NULL);
    }
  
    // Check if the operand is a denormaled value and FPSR.FS=0
    gen_cond_branch(gen_icmp_eq(link_stat, gen_i32_constant(1)), link_enable_branch, link_disable_branch);
    
    // Handled link enable case
    set_gen_point(link_enable_branch);
    
    // Update mem_acc_type register (Execution Transfer)
    gen_write_mem_acc_type(gen_i32_constant(0x15));
    
    // Store the word from reg2 to memory
    gen_i32_store(addr, reg3_load);
    
    // Write the result to reg3
    gen_write_GR(reg3, gen_i32_constant(1));
    gen_branch(continue_block2);
    
    // Handled link disable case
    set_gen_point(link_disable_branch);
    
    // Write the result to reg3
    gen_write_GR(reg3, gen_i32_constant(0));
    gen_branch(continue_block2);
  }

  // Continue block
  set_gen_point(continue_block2);

  if((is_link_func_enable()) && 
     (get_cpu_milestone() != rh850::core::FASTISS_G3K_PCU)){
    // clear the link information
    call_helper("clear_link_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), addr, NULL);
  }


  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
