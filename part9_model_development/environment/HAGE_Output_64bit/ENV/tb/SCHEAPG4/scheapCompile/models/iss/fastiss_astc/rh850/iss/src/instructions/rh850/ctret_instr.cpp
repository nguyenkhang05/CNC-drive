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

#include "rh850/ctret_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ctret implementation
//------------------------------------------------------------------------------

rh850::ctret_instr_t::ctret_instr_t
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

rh850::ctret_instr_t::~ctret_instr_t()
{
}

void rh850::ctret_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::ctret_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "ctret"; 
  return disass_string.str();
}

unsigned int rh850::ctret_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::ctret_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands

  // Restore PC from CTPC
  gen_write_PC(gen_read_CTPC());

  // Restore PSW from CTPSW
  fastiss::i32_t *pws_load = gen_read_PSW();
  fastiss::i32_t *tmp_psw = gen_and(pws_load, gen_i32_constant(0xffffffe0));

  gen_write_PSW(gen_or(tmp_psw, gen_read_CTPSW()));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

bool rh850::ctret_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850::ctret_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 7;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
