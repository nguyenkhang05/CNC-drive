/*************************************************************************
 *
 * Copyright(c) 2017-2018 Renesas Electronics Corporation
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

#include "rh850g4/stm_dot_gsr_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// stm.gsr [reg1] implementation
//------------------------------------------------------------------------------

rh850g4::stm_dot_gsr_ind_reg1_instr_t::stm_dot_gsr_ind_reg1_instr_t
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

rh850g4::stm_dot_gsr_ind_reg1_instr_t::~stm_dot_gsr_ind_reg1_instr_t()
{
}

void rh850g4::stm_dot_gsr_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::stm_dot_gsr_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "stm.gsr [" << reg1_name << "]"; 
  return disass_string.str();
}

unsigned int rh850g4::stm_dot_gsr_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::stm_dot_gsr_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *exp_op_branch = create_basic_block();
  fastiss::block_t *normal_op_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Check if Hardware-assisted Virtualization is valid (HVCFG.HVE is 1) and supervizer mode (PSW.UM is 0) and host mode (PSWH.GM is 0) 
  gen_cond_branch(gen_and(gen_and(is_valid_virtualization(), is_supervizer_mode()), is_host_mode()), normal_op_branch, exp_op_branch);

  // STM.gsr operation
  set_gen_point(normal_op_branch);

  // Load operand registers
  fastiss::i32_t *addr = gen_and(gen_read_GR(reg1), gen_i32_constant(0xfffffffc));

  // Update mem_acc_type register (write)
  gen_write_mem_acc_type(gen_i32_constant(0x06));

  // Store the system registers in the selID0 to the memory (sr6, sr7, sr16, sr17, sr20)
  for (int list_id = 0; list_id < STM_LDM_SELID0_INDEX_NUM; list_id++)
  {
    fastiss::i32_t *reg_val = gen_read_SR_SELID_0(SR_SELID_0_index_t(stm_ldm_selid0_index[list_id]));
    // Store the system register value to memory
    gen_i32_store(addr, reg_val);
    if(is_link_func_enable()){
      // clear the link(monitor) information
      call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
    }
    addr = gen_add(addr, gen_i32_constant(4));
  }

  // Store the system registers in the selID1 to the memory (sr5, sr11, sr12)
  for (int list_id = 0; list_id < STM_LDM_SELID1_INDEX_NUM; list_id++)
  {
    fastiss::i32_t *reg_val = gen_read_SR_SELID_1(SR_SELID_1_index_t(stm_ldm_selid1_index[list_id]));
    // Store the system register value to memory
    gen_i32_store(addr, reg_val);
    if(is_link_func_enable()){
      // clear the link(monitor) information
      call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
    }
    addr = gen_add(addr, gen_i32_constant(4));
  }

  // Store the system registers in the selID2 to the memory (sr15, sr16, sr17, sr18)
  for (int list_id = 0; list_id < STM_LDM_SELID2_INDEX_NUM; list_id++)
  {
    fastiss::i32_t *reg_val = gen_read_SR_SELID_2(SR_SELID_2_index_t(stm_ldm_selid2_index[list_id]));
    // Store the system register value to memory
    gen_i32_store(addr, reg_val);
    if(is_link_func_enable()){
      // clear the link(monitor) information
      call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
    }
    addr = gen_add(addr, gen_i32_constant(4));
  }

  // Store the system registers in the selID5 to the memory (sr8, sr9, sr11, sr12, sr16)
  for (int list_id = 0; list_id < STM_LDM_SELID5_INDEX_NUM; list_id++)
  {
    fastiss::i32_t *reg_val = gen_read_SR_SELID_5(SR_SELID_5_index_t(stm_ldm_selid5_index[list_id]));
    // Store the system register value to memory
    gen_i32_store(addr, reg_val);
    if(is_link_func_enable()){
      // clear the link(monitor) information
      call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
    }
    addr = gen_add(addr, gen_i32_constant(4));
  }

  // Store the system registers in the selID9 to the memory
  // (sr0, sr1, sr2, sr3, sr5, sr6, sr8, sr13, sr14, sr16, sr17, sr19, sr20, sr21, sr22, sr24, sr25, sr28, sr29)
  for (int list_id = 0; list_id < STM_LDM_SELID9_INDEX_NUM; list_id++)
  {
    fastiss::i32_t *reg_val = gen_read_SR_SELID_9(SR_SELID_9_index_t(stm_ldm_selid9_index[list_id]));
    // Store the system register value to memory
    gen_i32_store(addr, reg_val);
    if(is_link_func_enable()){
      // clear the link(monitor) information
      call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
    }
    addr = gen_add(addr, gen_i32_constant(4));
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  gen_branch(continue_block);


  // Handled exception case
  set_gen_point(exp_op_branch);

  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());

  // Detrmin the exeption
  fastiss::i32_t *exept_code = gen_select(gen_icmp_eq(is_valid_virtualization(), gen_i1_constant(0)), gen_i32_constant(0x60), gen_i32_constant(0xa0));

  // Cause the exeption
  gen_pie_rie_exept(gen_i32_constant(trans_PC), base_addr, exept_code, gen_read_PSWH_GM());


  // Continue block
  set_gen_point(continue_block);
}

unsigned int rh850g4::stm_dot_gsr_ind_reg1_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 1;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
