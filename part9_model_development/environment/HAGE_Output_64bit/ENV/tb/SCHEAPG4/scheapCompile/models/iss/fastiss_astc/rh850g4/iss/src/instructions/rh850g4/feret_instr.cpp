/*************************************************************************
 *
 * Copyright(c) 2015-2018 Renesas Electronics Corporation
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

#include "rh850g4/feret_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// feret implementation
//------------------------------------------------------------------------------

rh850g4::feret_instr_t::feret_instr_t
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

rh850g4::feret_instr_t::~feret_instr_t()
{
}

void rh850g4::feret_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::feret_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "feret"; 
  return disass_string.str();
}

unsigned int rh850g4::feret_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::feret_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *complete_irq_block = create_basic_block();
  fastiss::block_t *exp_op_branch = create_basic_block();
  fastiss::block_t *normal_op_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Check if supervizer mode (PSW.UM or GMPSW.UM is 0)
  gen_cond_branch(gen_icmp_eq(is_supervizer_mode(), gen_i1_constant(1)), normal_op_branch, exp_op_branch);

  // FERET operation
  set_gen_point(normal_op_branch);

  // Read in FEPC and FEPSW values.
  fastiss::i32_t *fepc_val = gen_read_FEPC();
  fastiss::i32_t *fepsw_val = gen_read_FEPSW();
  // Read in EIPSWH values
  fastiss::i32_t *fepswh_val = gen_select(gen_and(is_supervizer_mode(), is_host_mode()), gen_read_FEPSWH(), gen_read_PSWH());

  // Read in the PSW EP value
  fastiss::i1_t *psw_ep_val = gen_read_PSW_EP();
  
  // Restore PC to FEPC
  gen_write_PC(fepc_val);

  // Restore PSW to either FEPSW
  gen_write_PSW(fepsw_val);

  // Restore PSWH to either FEPSWH
  gen_write_PSWH(fepswh_val);

  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }

  // Notify that the IRQ is complete if EP == 0
  gen_cond_branch(psw_ep_val, continue_block, complete_irq_block);


  // Complete IRQ block
  set_gen_point(complete_irq_block);
  call_helper("terminate_quantum_helper", gen_read_core_ptr(), NULL);  
  gen_branch(continue_block);


  // Handled exception case
  set_gen_point(exp_op_branch);

  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());

  // Cause the exeption
  gen_pie_rie_exept(gen_i32_constant(trans_PC), base_addr, gen_i32_constant(0xa0), gen_read_PSWH_GM());


  // Continue block
  set_gen_point(continue_block);
  
  // Notify INTC immediately that the cpu mode (PSWH) is changed
  call_helper("cpugminfo_update_helper", gen_read_core_ptr(), NULL);  

  // Notify the interrupt complete analysis point
  notify_interrupt_complete();
}

bool rh850g4::feret_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850g4::feret_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 8;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
