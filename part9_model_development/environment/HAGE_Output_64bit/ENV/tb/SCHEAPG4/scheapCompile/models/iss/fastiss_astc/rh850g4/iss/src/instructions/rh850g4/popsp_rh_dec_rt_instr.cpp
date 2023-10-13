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

#include "rh850g4/popsp_rh_dec_rt_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// popsp rh-rt implementation
//------------------------------------------------------------------------------

rh850g4::popsp_rh_dec_rt_instr_t::popsp_rh_dec_rt_instr_t
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

rh850g4::popsp_rh_dec_rt_instr_t::~popsp_rh_dec_rt_instr_t()
{
}

void rh850g4::popsp_rh_dec_rt_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::popsp_rh_dec_rt_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t rh = get_rh_u8_0_4(opcode);
  std::string rh_name = register_name_GR(rh);
  GR_index_t rt = get_rt_u8_27_31(opcode);
  std::string rt_name = register_name_GR(rt);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "popsp " << rh_name << "-" << rt_name; 
  return disass_string.str();
}

unsigned int rh850g4::popsp_rh_dec_rt_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::popsp_rh_dec_rt_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t rh = get_rh_u8_0_4(opcode);
  GR_index_t rt = get_rt_u8_27_31(opcode);

  // Load SP registers
  fastiss::i32_t *sp_val = gen_read_r3();

  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x02));

  // Execute popsp
  if(rh <= rt){
    GR_index_t cur = rt;
    GR_index_t end = rh;
    while (cur >= end)
    {
      // Pop a value from the stack and write to the register.
      // The load address has the lower 2-bits masked to 0.
      fastiss::i32_t *addr = gen_and(sp_val, gen_i32_constant(0xfffffffc));
      fastiss::i32_t *loaded_val = gen_i32_load(addr);
      gen_write_GR(GR_index_t(cur), loaded_val);  
      cur = GR_index_t(cur - 1);
      sp_val = gen_add(sp_val, gen_i32_constant(4));
    }
    gen_write_r3(sp_val);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

unsigned int rh850g4::popsp_rh_dec_rt_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  GR_index_t rh = get_rh_u8_0_4(opcode);
  GR_index_t rt = get_rt_u8_27_31(opcode);

  // Determine the number of pops
  unsigned int num_pops = ((rh <= rt) ? (rt - rh) : 0);
  return (num_pops + 1);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
