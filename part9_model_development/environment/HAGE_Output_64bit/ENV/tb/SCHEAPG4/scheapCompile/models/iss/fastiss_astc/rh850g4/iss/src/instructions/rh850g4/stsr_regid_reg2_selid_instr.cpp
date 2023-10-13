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

#include "rh850g4/stsr_regid_reg2_selid_instr.h"
#include "rh850g4/core.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// stsr regID, reg2, selID implementation
//------------------------------------------------------------------------------

namespace rh850g4
{
  unsigned int stsr_regid_selid_reg2_helper(rh850g4::immediate_base_t *instr_base, rh850g4::SR_SELID_0_index_t regID, unsigned char selid_val)
  {
    unsigned int sr_val = 0;
    bool is_hvcfg_hve_valid = false;
    unsigned int cpu_type = get_cpu_milestone_helper((rh850g4::core*)(instr_base->read_core_ptr()));
    if((instr_base->read_HVCFG() & 0x1) == 0x1) is_hvcfg_hve_valid = true;
    switch (selid_val) {
      case 0: {
	if (regID == rh850g4::SR_SELID_0_INDEX_EIPC_0){
	  sr_val = instr_base->read_EIPC();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_EIPSW_1){
	  sr_val = instr_base->read_EIPSW();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_FEPC_2){
	  sr_val = instr_base->read_FEPC();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_FEPSW_3){
	  sr_val = instr_base->read_FEPSW();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_PSW_5){
	  sr_val = instr_base->read_PSW();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_EIIC_13){
	  sr_val = instr_base->read_EIIC();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_FEIC_14){
	  sr_val = instr_base->read_FEIC();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_EIWR_28){
	  sr_val = instr_base->read_EIWR();
	}else if (regID == rh850g4::SR_SELID_0_INDEX_FEWR_29){
	  sr_val = instr_base->read_FEWR();
	}else{
	  sr_val = instr_base->read_SR_SELID_0(regID);
	}
        break;
      }
      case 1: {
	if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_SPID_0){
	  sr_val = instr_base->read_SPID();
	}else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_SPIDLIST_1){
	  sr_val = instr_base->read_SPIDLIST();
	}else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_EBASE_3){
	  sr_val = instr_base->read_EBASE();
	}else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_INTBP_4){
	  sr_val = instr_base->read_INTBP();
	}else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_SVLOCK_8){
	  sr_val = instr_base->read_SVLOCK();
	}else{
	  sr_val = instr_base->read_SR_SELID_1((rh850g4::SR_SELID_1_index_t)regID);
	}
        break;
      }
      case 2: {
	if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_MEA_6){
	  sr_val = instr_base->read_MEA();
	}else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_MEI_8){
	  sr_val = instr_base->read_MEI();
	}else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_ICSR_12){
	  if ((cpu_type == rh850g4::core::FASTISS_G4MH_1_0) || 
	      (cpu_type == rh850g4::core::FASTISS_G4MH_1_1) ||
	      ((cpu_type == rh850g4::core::FASTISS_G4MH_2_0) && (!is_hvcfg_hve_valid))){
	    sr_val = instr_base->read_ICSR();
	  }
	}else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_INTCFG_13){
	  sr_val = instr_base->read_INTCFG();
	}else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_PLMR_14){
	  sr_val = instr_base->read_PLMR();
	}else{
	  sr_val = instr_base->read_SR_SELID_2((rh850g4::SR_SELID_2_index_t)regID);
	}
        break;
      }
      case 3: {
	sr_val = instr_base->read_SR_SELID_3((rh850g4::SR_SELID_3_index_t)regID);
        break;
      }
      case 4: {
	sr_val = instr_base->read_SR_SELID_4((rh850g4::SR_SELID_4_index_t)regID);
        break;
      }
      case 5: {
	if ((rh850g4::SR_SELID_5_index_t)regID == rh850g4::SR_SELID_5_INDEX_MPM_0){
	  sr_val = instr_base->read_MPM();
	}else{	  
	  sr_val = instr_base->read_SR_SELID_5((rh850g4::SR_SELID_5_index_t)regID);
	}
        break;
      }
      case 9: {
	sr_val = instr_base->read_SR_SELID_9((rh850g4::SR_SELID_9_index_t)regID);
        break;
      }
      case 10: {
	sr_val = instr_base->read_SR_SELID_10((rh850g4::SR_SELID_10_index_t)regID);
        break;
      }
      case 11: {
	if ((cpu_type == rh850g4::core::FASTISS_G4MH_1_0) || (cpu_type == rh850g4::core::FASTISS_G4MH_1_1)){
	  sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)regID);
	}else{
	  if (((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)16) &&
	      ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)17) &&
	      ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)18) &&
	      ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)19) &&
	      ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)20) &&
	      ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)21) &&
	      ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)22) &&
	      ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)23)){
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)regID);
	  }
	}
        break;
      }
      case 12: {
	sr_val = instr_base->read_SR_SELID_12((rh850g4::SR_SELID_12_index_t)regID);
        break;
      }
      case 13: {
	sr_val = instr_base->read_SR_SELID_13((rh850g4::SR_SELID_13_index_t)regID);
        break;
      }
      case 14: {
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  if ((rh850g4::SR_SELID_14_index_t)regID != ((rh850g4::SR_SELID_14_index_t)0)){
	    // PMCTRL0
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)17);
	  }else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)1){
	    // PMCTRL1
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)19);
	  }else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)2){
	    // PMCTRL2
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)21);
	  }else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)3){
	    // PMCTRL3
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)23);
	  }else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)16){
	    // PMCOUNT0
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)16);
	  }else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)17){
	    // PMCOUNT1
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)18);
	  }else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)18){
	    // PMCOUNT2
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)20);
	  }else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)19){
	    // PMCOUNT3
	    sr_val = instr_base->read_SR_SELID_11((rh850g4::SR_SELID_11_index_t)22);
	  }else{
	    sr_val = instr_base->read_SR_SELID_14((rh850g4::SR_SELID_14_index_t)regID);
	  }
	}
        break;
      }
      case 15: {
	sr_val = instr_base->read_SR_SELID_15((rh850g4::SR_SELID_15_index_t)regID);
        break;
      }
    }
    return sr_val;
  }
}

rh850g4::stsr_regid_reg2_selid_instr_t::stsr_regid_reg2_selid_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::operand_if_t *operand_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
) :
  rh850g4::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
  fastiss::func_args_type_t *stsr_regid_selid_reg2_helper_args = code_gen_impl->create_args_type();
  code_gen_impl->append_ptr(stsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i32(stsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i8(stsr_regid_selid_reg2_helper_args);
  code_gen_impl->add_helper("stsr_regid_selid_reg2_helper", (void*)&rh850g4::stsr_regid_selid_reg2_helper, code_gen_impl->create_i32_func(stsr_regid_selid_reg2_helper_args));
  code_gen_impl->delete_args_type(stsr_regid_selid_reg2_helper_args);
}

rh850g4::stsr_regid_reg2_selid_instr_t::~stsr_regid_reg2_selid_instr_t()
{
}

void rh850g4::stsr_regid_reg2_selid_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::stsr_regid_reg2_selid_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  SR_SELID_0_index_t regID = get_regID_u8_0_4(opcode);
  std::string regID_name = register_name_SR_SELID_0(regID);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  std::string reg2_name = register_name_GR(reg2);
  unsigned char selID = get_selID_u8_27_31(opcode);
  
  // Render and return result
  std::ostringstream disass_string;
  disass_string << "stsr 0x" << std::hex << regID << ", " << reg2_name << ", " << (unsigned int)selID;
  return disass_string.str();
}

unsigned int rh850g4::stsr_regid_reg2_selid_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::stsr_regid_reg2_selid_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  SR_SELID_0_index_t regID = get_regID_u8_0_4(opcode);
  GR_index_t reg2 = get_reg2_u8_11_15(opcode);
  fastiss::i8_t *selID = gen_get_selID_u8_27_31(opcode);

  fastiss::i8_ptr_t *instr_base = gen_i8_constant_ptr(dynamic_cast<rh850g4::immediate_base_t*>(this));
  fastiss::i32_t *sr_val = call_helper("stsr_regid_selid_reg2_helper", instr_base, gen_i32_constant(regID), selID, NULL);
  gen_write_GR(reg2, sr_val);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}



//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
