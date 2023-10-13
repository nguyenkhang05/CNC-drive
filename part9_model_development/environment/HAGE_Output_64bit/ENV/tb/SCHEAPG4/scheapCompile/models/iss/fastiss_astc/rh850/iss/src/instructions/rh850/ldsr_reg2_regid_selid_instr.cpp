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

#include "rh850/ldsr_reg2_regid_selid_instr.h"
#include "rh850/core.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ldsr reg2, regID, selID implementation
//------------------------------------------------------------------------------

namespace rh850
{
  void ldsr_regid_selid_reg2_helper(rh850::immediate_base_t *instr_base, rh850::SR_SELID_0_index_t regID, unsigned char selid_val, unsigned int gr_val)
  {
    unsigned int cpu_type = get_cpu_milestone_helper((rh850::core*)(instr_base->read_core_ptr()));
    switch (selid_val) {
      case 0: {
	if (regID == rh850::SR_SELID_0_INDEX_FPEC_11){
	  // G3K doesn't support FPU exception.
	  //if((instr_base->read_SR_SELID_0(regID) == 0x1)&&((gr_val & 0x1) == 0x0)){
	  //instr_base->write_ICSR_PMFP(0);
	  //}
	  if(gr_val == 0x0){ // clear only
	    instr_base->write_SR_SELID_0(regID, gr_val);
	  }
	}else if (regID == rh850::SR_SELID_0_INDEX_PSW_5){
	  unsigned int pre_psw_val = instr_base->read_SR_SELID_0(regID);
	  unsigned int set_psw_val = 0;
	  if((pre_psw_val & 0x00080000) == 0x00080000){
	    set_psw_val = gr_val;
	  }else{
	    set_psw_val = gr_val & 0xfff7ffff;
	  }
	  if (cpu_type == rh850::core::FASTISS_G3K_PCU){
	    instr_base->write_PSW(set_psw_val);
	  }else{
	    if((pre_psw_val & 0x80000000) == 0x80000000){
	      instr_base->write_PSW(set_psw_val | 0x80000000);
	    }else{
	      instr_base->write_PSW(set_psw_val & 0x7fffffff);
	    }
	  }
	  // added for end the quantam
	  terminate_quantum_helper((rh850::core*)(instr_base->read_core_ptr()));
	}else if (regID == rh850::SR_SELID_0_INDEX_FPSR_6){
	  if (cpu_type == rh850::core::FASTISS_G3MH){
	    instr_base->write_FPSR(gr_val | 0x00200000);
	  }else{
	    instr_base->write_FPSR(gr_val);
	  }
	}else{
	  instr_base->write_SR_SELID_0(regID, gr_val);
	}
        break;
      }
      case 1: {
	if ((rh850::SR_SELID_1_index_t)regID == rh850::SR_SELID_1_INDEX_MCFG0_0){
	  unsigned int org_val = instr_base->read_MCFG0();
	  if((org_val & 0x4) == 0x4) gr_val = gr_val | 0x4;
	  else                       gr_val = gr_val & 0xfffffffb;
	  if (cpu_type == rh850::core::FASTISS_G3M_V30){
	    if(((gr_val & 0x1f0000) == 0x0) || ((gr_val & 0x1f0000) == 0x10000)){
	      instr_base->write_SR_SELID_1((rh850::SR_SELID_1_index_t)regID, ((gr_val & 0xfffcffff) | 0x20000));
	    }else{
	      instr_base->write_SR_SELID_1((rh850::SR_SELID_1_index_t)regID, gr_val);
	    }
	  }else if (cpu_type == rh850::core::FASTISS_G3KH){
	    instr_base->write_SR_SELID_1((rh850::SR_SELID_1_index_t)regID, (gr_val & 0x3fff4));
	  }else{
	    instr_base->write_SR_SELID_1((rh850::SR_SELID_1_index_t)regID, gr_val);
	  }
	}else{
 	  if (cpu_type == rh850::core::FASTISS_G3K_PCU){
	    if ((rh850::SR_SELID_1_index_t)regID != rh850::SR_SELID_1_INDEX_RBASE_2 &&
		(rh850::SR_SELID_1_index_t)regID != rh850::SR_SELID_1_INDEX_PID_6)
	      {
		instr_base->write_SR_SELID_1((rh850::SR_SELID_1_index_t)regID, gr_val);
	      }
	  //}else{
	    //if ((rh850::SR_SELID_1_index_t)regID != rh850::SR_SELID_1_INDEX_RBASE_2 &&
		//(rh850::SR_SELID_1_index_t)regID != rh850::SR_SELID_1_INDEX_PID_6   &&
		//(rh850::SR_SELID_1_index_t)regID != rh850::SR_SELID_1_INDEX_MCFG1_1)
	      //{
		//instr_base->write_SR_SELID_1((rh850::SR_SELID_1_index_t)regID, gr_val);
	      //}
	  }
	}
        break;
      }
      case 2: {
	if ((rh850::SR_SELID_2_index_t)regID == rh850::SR_SELID_2_INDEX_ISPR_10){
	  if((instr_base->read_INTCFG_ISPC() & 0x1) == 0x1){
	    instr_base->write_SR_SELID_2((rh850::SR_SELID_2_index_t)regID, gr_val);
	    // added for end the quantam
	    terminate_quantum_helper((rh850::core*)(instr_base->read_core_ptr()));
	  }
	}else if ((rh850::SR_SELID_2_index_t)regID != rh850::SR_SELID_2_INDEX_HTCFG0_0 &&
		  (rh850::SR_SELID_2_index_t)regID != rh850::SR_SELID_2_INDEX_ICSR_12)
        {
	  instr_base->write_SR_SELID_2((rh850::SR_SELID_2_index_t)regID, gr_val);
	  if ((rh850::SR_SELID_2_index_t)regID == rh850::SR_SELID_2_INDEX_PMR_11){
	    // added for end the quantam
	    terminate_quantum_helper((rh850::core*)(instr_base->read_core_ptr()));
	  }
	}
        break;
      }
      case 3: {
	instr_base->write_SR_SELID_3((rh850::SR_SELID_3_index_t)regID, gr_val);
        break;
      }
      // G3K doesn't have selid4's system register
      //case 4: {
	//if ((rh850::SR_SELID_4_index_t)regID != rh850::SR_SELID_4_INDEX_TLBCFG_10 &&
	//    (rh850::SR_SELID_4_index_t)regID != rh850::SR_SELID_4_INDEX_ICCFG_26  &&
	//    (rh850::SR_SELID_4_index_t)regID != rh850::SR_SELID_4_INDEX_DCCFG_27)
	//{
	  //instr_base->write_SR_SELID_4((rh850::SR_SELID_4_index_t)regID, gr_val);
	//}
        //break;
      //}
      case 5: {
	if ((rh850::SR_SELID_5_index_t)regID != rh850::SR_SELID_5_INDEX_MPBRGN_4 &&
	    (rh850::SR_SELID_5_index_t)regID != rh850::SR_SELID_5_INDEX_MPTRGN_5)
	{
	  instr_base->write_SR_SELID_5((rh850::SR_SELID_5_index_t)regID, gr_val);
	}
        break;
      }
      case 6: {
	instr_base->write_SR_SELID_6((rh850::SR_SELID_6_index_t)regID, gr_val);

        break;
      }
      case 7: {
	instr_base->write_SR_SELID_7((rh850::SR_SELID_7_index_t)regID, gr_val);
        break;
      }
      // G3K doesn't have selid4's system register
      //case 13: {
	//instr_base->write_SR_SELID_13((rh850::SR_SELID_13_index_t)regID, gr_val);
        //break;
      //}
    }
  }
}

rh850::ldsr_reg2_regid_selid_instr_t::ldsr_reg2_regid_selid_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850::memory_if_t *memory_impl,
  rh850::operand_if_t *operand_impl,
  rh850::cpu_state_if_t *cpu_state_impl,
  rh850::custom_if_t *custom_impl
) :
  rh850::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
  fastiss::func_args_type_t *ldsr_regid_selid_reg2_helper_args = code_gen_impl->create_args_type();
  code_gen_impl->append_i32(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i32(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i8(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i32(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->add_helper("ldsr_regid_selid_reg2_helper", (void*)&rh850::ldsr_regid_selid_reg2_helper, code_gen_impl->create_void_func(ldsr_regid_selid_reg2_helper_args));
  code_gen_impl->delete_args_type(ldsr_regid_selid_reg2_helper_args);

}

rh850::ldsr_reg2_regid_selid_instr_t::~ldsr_reg2_regid_selid_instr_t()
{
}

void rh850::ldsr_reg2_regid_selid_instr_t::decode(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850::ldsr_reg2_regid_selid_instr_t::disassemble(rh850::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_0_4(opcode);
  std::string reg2_name = register_name_GR(reg2);
  SR_SELID_0_index_t regID = get_regID_u8_11_15(opcode);
  std::string regID_name = register_name_SR_SELID_0(regID);
  unsigned char selID = get_selID_u8_27_31(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "ldsr " << reg2_name << ", 0x" << std::hex << regID << ", " << std::dec << (unsigned int)selID;
  return disass_string.str();
}

unsigned int rh850::ldsr_reg2_regid_selid_instr_t::length(rh850::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850::ldsr_reg2_regid_selid_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  fastiss::block_t *fpi_flag_clear_branch = create_basic_block();
  fastiss::block_t *common_branch = create_basic_block();

  // Extract operands
  GR_index_t reg2 = get_reg2_u8_0_4(opcode);
  SR_SELID_0_index_t regID = get_regID_u8_11_15(opcode);
  fastiss::i8_t *selID = gen_get_selID_u8_27_31(opcode);

  fastiss::i8_ptr_t *instr_base = gen_i8_constant_ptr(dynamic_cast<rh850::immediate_base_t*>(this));
  call_helper("ldsr_regid_selid_reg2_helper", instr_base, gen_i32_constant(regID), selID, gen_read_GR(reg2), NULL);


  // check fpi flag(FPEC.FPIVD)
  gen_cond_branch(gen_icmp_eq(gen_read_FPEC(), gen_i32_constant(0)), fpi_flag_clear_branch, common_branch);
  
  // fpi flag clear
  set_gen_point(fpi_flag_clear_branch);
  gen_clear_fpi_flag();
  gen_branch(common_branch);

  // common operation
  set_gen_point(common_branch);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

bool rh850::ldsr_reg2_regid_selid_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

unsigned int rh850::ldsr_reg2_regid_selid_instr_t::cycle_count(opcode_t opcode, unsigned int trans_PC)
{
  return 3;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
