/*************************************************************************
 *
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
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

#include "rh850g4/trfsrv_dot_w4_imm3_wreg2_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// trfsrv.w4 imm3, wreg2 implementation
//------------------------------------------------------------------------------

rh850g4::trfsrv_dot_w4_imm3_wreg2_instr_t::trfsrv_dot_w4_imm3_wreg2_instr_t
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

rh850g4::trfsrv_dot_w4_imm3_wreg2_instr_t::~trfsrv_dot_w4_imm3_wreg2_instr_t()
{
}

void rh850g4::trfsrv_dot_w4_imm3_wreg2_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::trfsrv_dot_w4_imm3_wreg2_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int imm3 = get_imm3_u32_27_29(opcode);
  WR_index_t wreg2 = get_wreg2_u8_11_15(opcode);
  std::string wreg2_name = register_name_WR(WR_index_t(wreg2*2));
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "trfsrv.w4 " << imm3 << ", " << wreg2_name; 
  return disass_string.str();
}

unsigned int rh850g4::trfsrv_dot_w4_imm3_wreg2_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::trfsrv_dot_w4_imm3_wreg2_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned int imm3 = get_imm3_u32_27_29(opcode);
  WR_index_t wreg2 = get_wreg2_u8_11_15(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  WR_index_t wreg2lo = WR_index_t(wreg2*2);
  WR_index_t wreg2up = WR_index_t((wreg2*2) + 1);

  // Load operand registers
  fastiss::i64_t *wreg2lo_load = gen_read_WR(wreg2lo);
  fastiss::i64_t *wreg2up_load = gen_read_WR(wreg2up);

  // Extract upper and lower words of the result
  fastiss::i32_t *w0_val = gen_i32_extract(wreg2lo_load, gen_i64_constant(0));
  fastiss::i32_t *w1_val = gen_i32_extract(wreg2lo_load, gen_i64_constant(1));
  fastiss::i32_t *w2_val = gen_i32_extract(wreg2up_load, gen_i64_constant(0));
  fastiss::i32_t *w3_val = gen_i32_extract(wreg2up_load, gen_i64_constant(1));

  // Determine the PSW result
  if (imm3 == 0)
  {
    fastiss::i1_t *val0 = gen_select(gen_icmp_eq(w0_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    gen_write_PSW_Z(val0);
  }
  else if (imm3 == 1)
  {
    fastiss::i1_t *val1 = gen_select(gen_icmp_eq(w1_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    gen_write_PSW_Z(val1);
  }
  else if (imm3 == 2)
  {
    fastiss::i1_t *val2 = gen_select(gen_icmp_eq(w2_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    gen_write_PSW_Z(val2);
  }
  else if (imm3 == 3)
  {
    fastiss::i1_t *val3 = gen_select(gen_icmp_eq(w3_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    gen_write_PSW_Z(val3);
  }
  else if (imm3 == 4)
  {
    fastiss::i1_t *val0 = gen_select(gen_icmp_eq(w0_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    fastiss::i1_t *val1 = gen_select(gen_icmp_eq(w1_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    fastiss::i1_t *val2 = gen_select(gen_icmp_eq(w2_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    fastiss::i1_t *val3 = gen_select(gen_icmp_eq(w3_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    gen_write_PSW_Z(gen_and(gen_and(gen_and(val1, val0), val2), val3));
  }
  else if (imm3 == 5)
  {
    fastiss::i1_t *val0 = gen_select(gen_icmp_eq(w0_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    fastiss::i1_t *val1 = gen_select(gen_icmp_eq(w1_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    fastiss::i1_t *val2 = gen_select(gen_icmp_eq(w2_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    fastiss::i1_t *val3 = gen_select(gen_icmp_eq(w3_val, gen_i32_constant(0)), gen_i1_constant(0), gen_i1_constant(1));
    gen_write_PSW_Z(gen_or(gen_or(gen_or(val1, val0), val2), val3));
  }
  else
  {
    gen_write_PSW_Z(gen_i1_constant(0));
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
