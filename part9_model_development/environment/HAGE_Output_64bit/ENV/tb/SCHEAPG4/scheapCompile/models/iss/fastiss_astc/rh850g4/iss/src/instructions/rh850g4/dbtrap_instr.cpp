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

#include "rh850g4/dbtrap_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// dbtrap implementation
//------------------------------------------------------------------------------

rh850g4::dbtrap_instr_t::dbtrap_instr_t
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

rh850g4::dbtrap_instr_t::~dbtrap_instr_t()
{
}

void rh850g4::dbtrap_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850g4::dbtrap_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "dbtrap"; 
  return disass_string.str();
}

unsigned int rh850g4::dbtrap_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850g4::dbtrap_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands

  // Load registers
  fastiss::i32_t *curr_pc = gen_read_PC();

  // Set the DBPSW
  gen_write_DBPSWH(gen_read_PSWH());

  // clear the GM flags
  gen_write_PSWH_GM(gen_i1_constant(0));

  // Set the DBPC to PC + 2
  fastiss::i32_t *restore_pc = gen_add(curr_pc, gen_i32_constant(2));
  gen_write_DBPC(restore_pc);

  // Set the DBPSW
  gen_write_DBPSW(gen_read_PSW());

  // Set the exception code
  gen_write_DBIC(gen_i32_constant(0xb1));
  
  // clear the UM flags
  gen_write_PSW_UM(gen_i1_constant(0));

  // Set the NP, EP and ID flags
  gen_write_PSW_NP(gen_i1_constant(1));
  gen_write_PSW_EP(gen_i1_constant(1));
  gen_write_PSW_ID(gen_i1_constant(1));

  // Set the DIR0
  fastiss::i32_t *dir0_val = gen_read_DIR0();
  fastiss::i32_t *dir0_dm_val = gen_and(dir0_val, gen_i32_constant(0x00000001));
  fastiss::i32_t *dir0_dn_val = gen_select(gen_icmp_eq(dir0_dm_val, gen_i32_constant(1)), gen_i32_constant(0x4), gen_i32_constant(0x0));
  dir0_dm_val = gen_select(gen_icmp_eq(dir0_dm_val, gen_i32_constant(1)), gen_i32_constant(0x0), gen_i32_constant(0x1));
  gen_write_DIR0(gen_or(gen_or(dir0_dn_val, dir0_dm_val), dir0_val));

  // Update PC
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE()); 
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0xb0)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

bool rh850g4::dbtrap_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
