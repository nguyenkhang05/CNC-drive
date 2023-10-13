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

#include "rh850/popsp_rh_dec_rt_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// popsp rh-rt implementation
//------------------------------------------------------------------------------

rh850::popsp_rh_dec_rt_instr_t::popsp_rh_dec_rt_instr_t
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

rh850::popsp_rh_dec_rt_instr_t::~popsp_rh_dec_rt_instr_t()
{
}

void rh850::popsp_rh_dec_rt_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::popsp_rh_dec_rt_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
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

unsigned int rh850::popsp_rh_dec_rt_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::popsp_rh_dec_rt_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t rh = get_rh_u8_0_4(opcode);
  GR_index_t rt = get_rt_u8_27_31(opcode);

  // Load SP registers
  fastiss::i32_t *sp_val = gen_read_r3();

  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x02));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Execute popsp
  if(rh <= rt){
    GR_index_t cur = rt;
    GR_index_t end = rh;
    //fastiss::block_t *continue_block[12];
    while (cur >= end)
    {
      // Pop a value from the stack and write to the register.
      // The load address has the lower 2-bits masked to 0.
      fastiss::i32_t *addr = gen_and(sp_val, gen_i32_constant(0xfffffffc));
      // Check if MPU function enable
      if (is_mdp_enable()){
        fastiss::block_t *continue_block = create_basic_block();
        fastiss::i32_t *mpu_func_result = call_helper(
          "mpu_func_helper", 
          gen_read_core_ptr(), 
          gen_i32_constant(rh850::MPU_KIND_BL_R), 
          gen_i32_constant(rh850::WORD_SIZE),
          addr,
          gen_i32_constant(0x51a),
          NULL
        );
        gen_cond_branch(
          gen_icmp_eq(mpu_func_result, gen_i32_constant(1)),
          continue_block,
          get_exit_block()
        );
        set_gen_point(continue_block);
      }
 
      fastiss::i32_t *loaded_val = gen_i32_load(addr);
      gen_write_GR(GR_index_t(cur), loaded_val);  
      cur = GR_index_t(cur - 1);
      sp_val = gen_add(sp_val, gen_i32_constant(4));
    }
    gen_write_r3(sp_val);
  }

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

}

unsigned int rh850::popsp_rh_dec_rt_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
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
