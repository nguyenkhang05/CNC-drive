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

#include "rh850g4/stm_dot_mp_eh_dec_et_ind_reg1_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// stm.mp eh-et, [reg1] implementation
//------------------------------------------------------------------------------

rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t::stm_dot_mp_eh_dec_et_ind_reg1_instr_t
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

rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t::~stm_dot_mp_eh_dec_et_ind_reg1_instr_t()
{
}

void rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  SR_SELID_5_index_t eh = get_eh_u8_11_14(opcode);
  std::string eh_name = register_name_SR_SELID_5(eh);
  SR_SELID_5_index_t et = get_et_u8_27_31(opcode);
  std::string et_name = register_name_SR_SELID_5(et);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);
  std::string reg1_name = register_name_GR(reg1);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "stm.mp " << eh << "-" << et << ", [" << reg1_name << "]";
  return disass_string.str();
}

unsigned int rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  SR_SELID_5_index_t eh = get_eh_u8_11_14(opcode);
  SR_SELID_5_index_t et = get_et_u8_27_31(opcode);
  GR_index_t reg1 = get_reg1_u8_0_4(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  
  // Create sub-blocks
  fastiss::block_t *exp_op_branch = create_basic_block();
  fastiss::block_t *normal_op_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();
  
  // Check if supervizer mode (PSW.UM is 0) 
  gen_cond_branch(gen_icmp_eq(is_supervizer_mode(), gen_i1_constant(1)), normal_op_branch, exp_op_branch);
  
  // STM.MP operation
  set_gen_point(normal_op_branch);
  
  // Load operand registers
  fastiss::i32_t *addr = gen_and(gen_read_GR(reg1), gen_i32_constant(0xfffffffc));
  
  // Update mem_acc_type register (write)
  gen_write_mem_acc_type(gen_i32_constant(0x06));
  
  // Store the system registers (MPLA, MPUA, MPAT) to the memory
  if(eh <= et){
    SR_SELID_5_index_t cur = eh;
    SR_SELID_5_index_t end = et;
    while (cur <= end) {
      // Store the system register value from  memory
      // The load address has the lower 2-bits masked to 0.
      gen_i32_store(addr, gen_read_MPLA());  
      if(is_link_func_enable()){
	// clear the link(monitor) information
	call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
      }
      
      addr = gen_add(addr, gen_i32_constant(4));
      gen_i32_store(addr, gen_read_MPUA());  
      if(is_link_func_enable()){
	// clear the link(monitor) information
	call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
      }
      
      addr = gen_add(addr, gen_i32_constant(4));
      gen_i32_store(addr, gen_read_MPAT());  
      if(is_link_func_enable()){
	// clear the link(monitor) information
	call_helper("clear_monitor_helper", gen_read_link_manager_ptr(), gen_read_PEID(), addr, gen_i32_constant(4), NULL);
      }
      
      addr = gen_add(addr, gen_i32_constant(4));
      cur = SR_SELID_5_index_t(cur + 1);
    }
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
  
  gen_branch(continue_block);


  // Handled exception case
  set_gen_point(exp_op_branch);
  
  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());
  
  // Cause the exeption
  gen_pie_rie_exept(gen_i32_constant(trans_PC), base_addr, gen_i32_constant(0xa0), gen_read_PSWH_GM());
  
  // Continue block
  set_gen_point(continue_block);
}

unsigned int rh850g4::stm_dot_mp_eh_dec_et_ind_reg1_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 1;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
