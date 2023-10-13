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

#include "rh850/feret_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// feret implementation
//------------------------------------------------------------------------------

rh850::feret_instr_t::feret_instr_t
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

rh850::feret_instr_t::~feret_instr_t()
{
}

void rh850::feret_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::feret_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "feret"; 
  return disass_string.str();
}

unsigned int rh850::feret_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::feret_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Create sub-blocks
  fastiss::block_t *complete_irq_block = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Read in FEPC and FEPSW values.
  fastiss::i32_t *fepc_val = gen_read_FEPC();
  fastiss::i32_t *fepsw_val = gen_read_FEPSW();

  // Read in the PSW EP value
  fastiss::i1_t *psw_ep_val = gen_read_PSW_EP();
  
  // Restore PC to FEPC
  gen_write_PC(fepc_val);

  // Restore PSW to either FEPSW
  gen_write_PSW(fepsw_val);

  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), NULL);
  }

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Notify that the IRQ is complete if EP == 0
  gen_cond_branch(psw_ep_val, continue_block, complete_irq_block);
  set_gen_point(complete_irq_block);
  call_helper("terminate_quantum_helper", gen_read_core_ptr(), NULL);  
  gen_branch(continue_block);
  set_gen_point(continue_block);
  
  // Notify the interrupt complete analysis point
  notify_interrupt_complete();
}

bool rh850::feret_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850::feret_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 7;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
