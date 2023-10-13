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

#include "rh850g4/dbret_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// dbret implementation
//------------------------------------------------------------------------------

rh850g4::dbret_instr_t::dbret_instr_t
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

rh850g4::dbret_instr_t::~dbret_instr_t()
{
}

void rh850g4::dbret_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::dbret_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "dbret"; 
  return disass_string.str();
}

unsigned int rh850g4::dbret_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::dbret_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Create sub-blocks
  fastiss::block_t *complete_irq_block = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Read in DBPC and DBPSW values.
  fastiss::i32_t *dbpc_val = gen_read_DBPC();
  fastiss::i32_t *dbpsw_val = gen_read_DBPSW();
  // Read in DBPSWH values
  fastiss::i32_t *dbpswh_val = gen_read_DBPSWH();

  // Read in the PSW EP value
  fastiss::i1_t *psw_ep_val = gen_read_PSW_EP();
  
  // Restore PC to DBPC
  gen_write_PC(dbpc_val);

  // Restore PSW to either DBPSW
  gen_write_PSW(dbpsw_val);

  // Restore PSWH to either DBPSWH
  gen_write_PSWH(dbpswh_val);

  // Clear DIR0.DM
  gen_write_DIR0(gen_and(gen_read_DIR0(), gen_i32_constant(0xfffffffe)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Notify that the IRQ is complete if EP == 0
  gen_cond_branch(psw_ep_val, continue_block, complete_irq_block);

  // Complete IRQ block
  set_gen_point(complete_irq_block);
  call_helper("terminate_quantum_helper", gen_read_core_ptr(), NULL);  
  gen_branch(continue_block);

  // Continue block
  set_gen_point(continue_block);
  
  // Notify INTC immediately that the cpu mode (PSWH) is changed
  call_helper("cpugminfo_update_helper", gen_read_core_ptr(), NULL);  

  // Notify the interrupt complete analysis point
  notify_interrupt_complete();
}

bool rh850g4::dbret_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
