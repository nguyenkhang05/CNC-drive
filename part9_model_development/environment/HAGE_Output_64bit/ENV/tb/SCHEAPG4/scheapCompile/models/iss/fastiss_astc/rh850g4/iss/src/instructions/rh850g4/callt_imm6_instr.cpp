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

#include "rh850g4/callt_imm6_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// callt imm6 implementation
//------------------------------------------------------------------------------

rh850g4::callt_imm6_instr_t::callt_imm6_instr_t
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

rh850g4::callt_imm6_instr_t::~callt_imm6_instr_t()
{
}

void rh850g4::callt_imm6_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(2);  
}

std::string rh850g4::callt_imm6_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int imm6 = get_imm6_u32_0_5_shift1(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "callt 0x" << std::hex << (imm6 >> 1);
  return disass_string.str();
}

unsigned int rh850g4::callt_imm6_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 2;
}

void rh850g4::callt_imm6_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i32_t *imm6 = gen_get_imm6_u32_0_5_shift1(opcode);

  // Save the restore PC to CTPC
  fastiss::i32_t *restore_PC = gen_add(gen_read_PC(), gen_i32_constant(2));
  gen_write_CTPC(restore_PC);

  // Save the restore PSW to CTPSW
  gen_write_CTPSW(gen_read_PSW());

  // Read CTBP
  fastiss::i32_t *CTBP_val = gen_read_CTBP();

  // Generate 32-bit table entry address
  fastiss::i32_t *addr = gen_add(CTBP_val, imm6);

  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x1));

  // Generate 32-bit target address
  fastiss::i32_t *target_PC = gen_add(CTBP_val, gen_i32_zext(gen_i16_load(addr)));

  // Update PC
  gen_write_PC(target_PC);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

bool rh850g4::callt_imm6_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850g4::callt_imm6_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 17;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
