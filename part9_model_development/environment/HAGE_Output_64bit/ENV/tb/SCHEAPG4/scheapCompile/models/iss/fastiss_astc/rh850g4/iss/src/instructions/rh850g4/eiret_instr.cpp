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

#include "rh850g4/eiret_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// eiret implementation
//------------------------------------------------------------------------------

namespace rh850g4
{
  void eiret_helper(rh850g4::immediate_base_t *instr_base, unsigned int ispr_val)
  {
    for(int i=0; i<16; i++){
      if(ispr_val & (0x1 << i)){
	instr_base->write_ISPR(ispr_val & ~(0x1 << i));
	return;
      }
    }
  }
}

rh850g4::eiret_instr_t::eiret_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::operand_if_t *operand_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
) :
  rh850g4::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
  fastiss::func_args_type_t *eiret_helper_args = code_gen_impl->create_args_type();
  code_gen_impl->append_ptr(eiret_helper_args);
  code_gen_impl->append_i32(eiret_helper_args);
  code_gen_impl->add_helper("eiret_helper", (void*)&rh850g4::eiret_helper, code_gen_impl->create_void_func(eiret_helper_args));
  code_gen_impl->delete_args_type(eiret_helper_args);
}

rh850g4::eiret_instr_t::~eiret_instr_t()
{
}

void rh850g4::eiret_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::eiret_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "eiret"; 
  return disass_string.str();
}

unsigned int rh850g4::eiret_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::eiret_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *update_ispr_block = create_basic_block();
  fastiss::block_t *keep_ispr_block = create_basic_block();
  fastiss::block_t *complete_irq_block = create_basic_block();
  fastiss::block_t *exp_op_branch = create_basic_block();
  fastiss::block_t *normal_op_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Check if supervizer mode (PSW.UM or GMPSW.UM is 0)
  gen_cond_branch(gen_icmp_eq(is_supervizer_mode(), gen_i1_constant(1)), normal_op_branch, exp_op_branch);

  // EIRET operation
  set_gen_point(normal_op_branch);

  // Read in EIPC and EIPSW values.
  fastiss::i32_t *eipc_val = gen_read_EIPC();
  fastiss::i32_t *eipsw_val = gen_read_EIPSW();
  
  // Read in EIPSWH values
  fastiss::i32_t *eipswh_val = gen_select(gen_and(is_supervizer_mode(), is_host_mode()), gen_read_EIPSWH(), gen_read_PSWH());

  // Read in the PSW EP value
  fastiss::i1_t *psw_ep_val = gen_read_PSW_EP();
  
  // Restore PC to EIPC
  gen_write_PC(eipc_val);

  // Restore PSW to either EIPSW
  gen_write_PSW(eipsw_val);

  fastiss::i1_t *is_update_ispr = gen_and(gen_and(gen_icmp_eq(gen_read_INTCFG_ISPC(), gen_i1_constant(0)), gen_icmp_eq(psw_ep_val, gen_i1_constant(0))), gen_icmp_eq(gen_read_INTCFG_EPL(), gen_i1_constant(0)));

  // Restore PSWH to either EIPSWH
  gen_write_PSWH(eipswh_val);

  // check conditon of uppdate ispr
  gen_cond_branch(is_update_ispr, update_ispr_block, keep_ispr_block);

  // update ISPR value
  set_gen_point(update_ispr_block);
  fastiss::i8_ptr_t *instr_base = gen_i8_constant_ptr(dynamic_cast<rh850g4::immediate_base_t*>(this));
  call_helper("eiret_helper", instr_base, gen_read_ISPR(), NULL);
  gen_branch(keep_ispr_block);

  // keep ISPR value
  set_gen_point(keep_ispr_block);

  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    // temp code !!!!!!! need re-check for Link
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

bool rh850g4::eiret_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850g4::eiret_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 8;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
