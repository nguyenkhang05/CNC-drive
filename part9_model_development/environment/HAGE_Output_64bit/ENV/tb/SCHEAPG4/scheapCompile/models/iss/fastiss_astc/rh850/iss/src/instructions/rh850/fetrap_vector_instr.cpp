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

#include "rh850/fetrap_vector_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// fetrap vector implementation
//------------------------------------------------------------------------------

rh850::fetrap_vector_instr_t::fetrap_vector_instr_t
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

rh850::fetrap_vector_instr_t::~fetrap_vector_instr_t()
{
}

void rh850::fetrap_vector_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850::fetrap_vector_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char vector = get_vector_u8_11_14(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "fetrap 0x" << std::hex << (unsigned int)vector;
  return disass_string.str();
}

unsigned int rh850::fetrap_vector_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850::fetrap_vector_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i8_t *vector = gen_get_vector_u8_11_14(opcode);

  // Load registers
  fastiss::i32_t *curr_pc = gen_read_PC();

  // Set the FEPC to PC + 2
  fastiss::i32_t *restore_pc = gen_add(curr_pc, gen_i32_constant(2));
  gen_write_FEPC(restore_pc);

  // Set the FEPSW
  gen_write_FEPSW(gen_read_PSW());
  
  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());

  // Set the exception code
  fastiss::i16_t *interrupt_code = gen_add(gen_i16_constant(0x30), gen_i16_zext(vector));
  gen_write_FEIC(gen_i32_zext(interrupt_code));
  
  // Set the EP, ID, and NP flags
  gen_write_PSW_EP(gen_i1_constant(1));
  gen_write_PSW_ID(gen_i1_constant(1));
  gen_write_PSW_NP(gen_i1_constant(1));

  // clear the UM and HVC flags
  gen_write_PSW_UM(gen_i1_constant(0));
  //gen_write_PSW_HVC(gen_i1_constant(0)); G3K doen't have this bit.
  
  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_HTCFG0_PEID(), NULL);
  }

  // Update PC
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x30)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
  
  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();
}

bool rh850::fetrap_vector_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850::fetrap_vector_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 7;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
