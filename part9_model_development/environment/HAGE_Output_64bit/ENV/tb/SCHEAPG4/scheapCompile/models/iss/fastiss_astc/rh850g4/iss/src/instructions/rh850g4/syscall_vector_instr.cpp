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

#include "rh850g4/syscall_vector_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// syscall vector implementation
//------------------------------------------------------------------------------

rh850g4::syscall_vector_instr_t::syscall_vector_instr_t
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

rh850g4::syscall_vector_instr_t::~syscall_vector_instr_t()
{
}

void rh850g4::syscall_vector_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::syscall_vector_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char vector = get_vector_u8_27_29_0_4(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "syscall 0x" << std::hex << (unsigned int)vector;
  return disass_string.str();
}

unsigned int rh850g4::syscall_vector_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::syscall_vector_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i8_t *vector = gen_get_vector_u8_27_29_0_4(opcode);

  // Load registers
  fastiss::i32_t *curr_pc = gen_read_PC();
  fastiss::i32_t *scbp = gen_read_SCBP();

  // Set the EIPC to PC + 4
  fastiss::i32_t *restore_pc = gen_add(curr_pc, gen_i32_constant(4));
  gen_write_EIPC(restore_pc);

  // Set the EIPSW
  gen_write_EIPSW(gen_read_PSW());

  // Set the exception code
  fastiss::i16_t *interrupt_code = gen_add(gen_i16_constant(0x8000), gen_i16_zext(vector));
  gen_write_EIIC(gen_i32_zext(interrupt_code));

  // Set the EP and ID flags
  gen_write_PSW_EP(gen_i1_constant(1));
  gen_write_PSW_ID(gen_i1_constant(1));

  // clear the UM flags
  gen_write_PSW_UM(gen_i1_constant(0));
  
  // Determine table offset
  fastiss::i1_t *is_within_table = gen_icmp_ule(vector, gen_read_SCCFG_SIZE());
  fastiss::i32_t *offset = gen_select(is_within_table, gen_shl(gen_i32_zext(vector), gen_i32_constant(2)), gen_i32_constant(0));

  // Determine table address
  fastiss::i32_t *addr = gen_add(scbp, offset);
  
  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x02));

  // Load from table address
  fastiss::i32_t *i32_load = gen_i32_load(addr);

  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }

  // Update PC
  fastiss::i32_t *new_pc = gen_add(scbp, i32_load);
  gen_write_PC(new_pc);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  
  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();
}

bool rh850g4::syscall_vector_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850g4::syscall_vector_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 17;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
