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

#include "rh850g4/resbank_instr.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// resbank implementation
//------------------------------------------------------------------------------

namespace rh850g4
{
  void while_loop_helper(rh850g4::immediate_base_t *instr_base, unsigned int cur, unsigned int addr)
  {
    while(cur>0){
      instr_base->write_GR(GR_index_t(cur), instr_base->i32_load(addr));
      cur = cur - 1;
      addr = addr + 4;
    }
    instr_base->write_FPSR(0x200000 | instr_base->i32_load(addr));
    addr = addr + 4;
    instr_base->write_EIIC(instr_base->i32_load(addr));
    addr = addr + 4;
    instr_base->write_EIPSW(instr_base->i32_load(addr));
    addr = addr + 4;
    instr_base->write_EIPC(instr_base->i32_load(addr));
  }
}  

rh850g4::resbank_instr_t::resbank_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::operand_if_t *operand_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
) :
  rh850g4::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
  fastiss::func_args_type_t *while_loop_helper_args = code_gen_impl->create_args_type();
  code_gen_impl->append_ptr(while_loop_helper_args);
  code_gen_impl->append_i32(while_loop_helper_args);
  code_gen_impl->append_i32(while_loop_helper_args);
  code_gen_impl->add_helper("while_loop_helper", (void*)&rh850g4::while_loop_helper, code_gen_impl->create_void_func(while_loop_helper_args));
  code_gen_impl->delete_args_type(while_loop_helper_args);
}

rh850g4::resbank_instr_t::~resbank_instr_t()
{
}

void rh850g4::resbank_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::resbank_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "resbank"; 
  return disass_string.str();
}

unsigned int rh850g4::resbank_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::resbank_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  fastiss::i8_ptr_t *instr_base = gen_i8_constant_ptr(dynamic_cast<rh850g4::immediate_base_t*>(this));
  // Extract operands

  // Create sub-blocks
  fastiss::block_t *rbnr_bn_0_branch = create_basic_block();
  fastiss::block_t *rbnr_bn_not0_branch = create_basic_block();
  fastiss::block_t *rbcr0_md_0_branch = create_basic_block();
  fastiss::block_t *rbcr0_md_1_branch = create_basic_block();
  fastiss::block_t *rbnr_bn_not0_common_branch = create_basic_block();
  fastiss::block_t *continue_block = create_basic_block();

  // Check if the value RBNR.BN
  gen_cond_branch(gen_icmp_eq(gen_read_RBNR_BN(), gen_i8_constant(0)), rbnr_bn_0_branch, rbnr_bn_not0_branch);

  // Handled RBNR.BR=0
  set_gen_point(rbnr_bn_0_branch);

  fastiss::i1_t *current_mode =  gen_read_PSWH_GM();

  if(is_link_func_enable()){
    // clear the link(ll-bit) information
    // temp code !!!!!!! need re-check for Link
    call_helper("clear_llbit_helper", gen_read_link_manager_ptr(), gen_read_PEID(), NULL);
  }
  
  // Update system register
  gen_write_FEPSWH(gen_select(gen_icmp_eq(gen_read_HVCFG_HVE(), gen_i1_constant(0)),
			      gen_read_FEPSW(),
			      gen_read_PSWH()));
  gen_write_PSWH_GM(gen_select(gen_or(gen_icmp_eq(current_mode, gen_i1_constant(0)),
				      gen_icmp_eq(gen_read_HVCFG_HVE(), gen_i1_constant(0))),
			       gen_i1_constant(0),
			       gen_select(gen_icmp_eq(gen_read_GMCFG_GSYSE(), gen_i1_constant(1)),
					  gen_i1_constant(0), 
					  gen_i1_constant(1))));
  gen_write_FEPC(gen_read_PC());
  gen_write_FEPSW(gen_read_PSW());
  gen_write_FEIC(gen_i32_constant(0x0000001d));
  gen_write_PSW_UM(gen_i1_constant(0));
  gen_write_PSW_NP(gen_i1_constant(1));
  gen_write_PSW_EP(gen_i1_constant(1));
  gen_write_PSW_ID(gen_i1_constant(1));

  // Update PC
  fastiss::i32_t *base_addr = gen_select(gen_icmp_eq(gen_read_PSW_EBV(), gen_i1_constant(0)), gen_read_RBASE(), gen_read_EBASE());
  gen_write_PC(gen_add(gen_and(gen_i32_constant(0xfffffe00), base_addr), gen_i32_constant(0x10)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));

  // Branch to the exit block
  gen_branch(get_exit_block());
  
  // Handled link RBNR.BR>0
  set_gen_point(rbnr_bn_not0_branch);

  fastiss::i32_t *addr = gen_i32_constant(0);
  fastiss::i32_t *cur  = gen_i32_constant(0);

  // Check if the value RBCR0.MD
  gen_cond_branch(gen_icmp_eq(gen_read_RBCR0_MD(), gen_i1_constant(0)), rbcr0_md_0_branch, rbcr0_md_1_branch);

  // Handled link RBCR0.MD=0
  set_gen_point(rbcr0_md_0_branch);
  
  addr = gen_sub(gen_and(gen_read_RBIP(), gen_i32_constant(0xfffffff0)), 
		 gen_mul(gen_read_RBNR(), gen_i32_constant(0x60)));
  cur  = gen_i32_constant(19);

  // Update mem_acc_type register (read)
  gen_write_mem_acc_type(gen_i32_constant(0x02));

  // Load the word from memory
  fastiss::i32_t *loaded_val = gen_i32_load(addr);
  
  // Write the result to r30
  gen_write_GR(GR_index_t(30), loaded_val);
  addr = gen_add(addr, gen_i32_constant(4));
  call_helper("while_loop_helper", instr_base, cur, addr, NULL);
  gen_branch(rbnr_bn_not0_common_branch);

  // Handled link RBCR0.MD=1
  set_gen_point(rbcr0_md_1_branch);
  
  addr = gen_sub(gen_and(gen_read_RBIP(), gen_i32_constant(0xfffffff0)), 
		 gen_mul(gen_read_RBNR(), gen_i32_constant(0x90)));
  cur  = gen_i32_constant(31);
  addr = gen_add(addr, gen_i32_constant(4));
  call_helper("while_loop_helper", instr_base, cur, addr, NULL);
  gen_branch(rbnr_bn_not0_common_branch);
  
  // Handled link RBNR.BR>0 common operation
  set_gen_point(rbnr_bn_not0_common_branch);
  fastiss::i8_t *rbnr_bn_val = gen_read_RBNR_BN();
  rbnr_bn_val = gen_sub(rbnr_bn_val, gen_i8_constant(1));
  gen_write_RBNR_BN(rbnr_bn_val);
  gen_write_PSW_ID(gen_i1_constant(1));

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));
  gen_branch(continue_block);


  // Continue block
  set_gen_point(continue_block);

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_select(gen_read_RBCR0_MD(), gen_i32_constant(19), gen_i32_constant(13)));
}


//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
