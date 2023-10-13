/*************************************************************************
 *
 * Copyright(c) 2017-2018 Renesas Electronics Corporation
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

#include "rh850g4/hvtrap_vector_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// hvtrap vector implementation
//------------------------------------------------------------------------------

rh850g4::hvtrap_vector_instr_t::hvtrap_vector_instr_t
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

rh850g4::hvtrap_vector_instr_t::~hvtrap_vector_instr_t()
{
}

void rh850g4::hvtrap_vector_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::hvtrap_vector_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  unsigned char vector = get_vector_u8_0_4(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "hvtrap 0x" << std::hex << (unsigned int)vector; 
  return disass_string.str();
}

unsigned int rh850g4::hvtrap_vector_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::hvtrap_vector_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  fastiss::i8_t *vector = gen_get_vector_u8_0_4(opcode);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Create sub-blocks
  fastiss::block_t *exp_op_branch = create_basic_block();
  fastiss::block_t *normal_op_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // get the GM flags
  fastiss::i1_t *current_pswh_gm = gen_read_PSWH_GM();

  fastiss::i1_t *hvcfg_hve_val = is_valid_virtualization();
  fastiss::i1_t *is_except = gen_and(is_valid_virtualization(), is_supervizer_mode());
  
  // set Host Mode
  gen_write_PSWH_GM(gen_i1_constant(0));

  // Get base address
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());
  
  // Check if Hardware-assisted Virtualization is valid (HVCFG.HVE is 1) and supervizer mode (PSW.UM is 0)
  gen_cond_branch(is_except, normal_op_branch, exp_op_branch);

  // HVTRAP operation
  set_gen_point(normal_op_branch);

  // Set the EIPC to PC + 4
  fastiss::i32_t *restore_pc = gen_add(gen_i32_constant(trans_PC), gen_i32_constant(4));
  gen_write_EIPC(restore_pc);

  // Set the EIPSW
  gen_write_EIPSW(gen_read_PSW());

  // Set the exception code
  fastiss::i16_t *interrupt_code = gen_add(gen_i16_constant(0xF000), gen_i16_zext(vector));
  gen_write_EIIC(gen_i32_zext(interrupt_code));
  
  // Set the EP and ID flags
  gen_write_PSW_EP(gen_i1_constant(1));
  gen_write_PSW_ID(gen_i1_constant(1));

  // clear the UM flags
  gen_write_PSW_UM(gen_i1_constant(0));
  
  // set EIPSWH
  gen_write_EIPSWH_GM(current_pswh_gm);
  gen_write_EIPSWH_GPID(gen_read_PSWH_GPID());
  
  // Update PC
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x20)));

  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }

  // Notify INTC immediately that the cpu mode (PSWH) is changed
  call_helper("cpugminfo_update_helper", gen_read_core_ptr(), NULL);  

  // Notify the interrupt accepted analysis point
  notify_interrupt_accepted();

  gen_branch(continue_block);


  // Handled exception case
  set_gen_point(exp_op_branch);

  gen_write_PSWH_GM(current_pswh_gm);

  // Detrmin the exeption
  fastiss::i32_t *exept_code = gen_select(gen_icmp_eq(hvcfg_hve_val, gen_i1_constant(0)), gen_i32_constant(0x60), gen_i32_constant(0xa0));

  // Cause the exeption
  gen_pie_rie_exept(gen_i32_constant(trans_PC), base_addr, exept_code, current_pswh_gm);


  // Continue block
  set_gen_point(continue_block);
}

bool rh850g4::hvtrap_vector_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850g4::hvtrap_vector_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 8;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
