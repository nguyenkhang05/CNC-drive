/*************************************************************************
 *
 * Copyright(c) 2015-2020 Renesas Electronics Corporation
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

#include "rh850g4/ldsr_reg2_regid_selid_instr.h"
#include "rh850g4/core.h"
#include <assert.h>
#include <sstream>

//------------------------------------------------------------------------------
// ldsr reg2, regID, selID implementation
//------------------------------------------------------------------------------

namespace rh850g4
{
  void ldsr_regid_selid_reg2_helper(rh850g4::immediate_base_t *instr_base, rh850g4::SR_SELID_0_index_t regID, unsigned char selid_val, unsigned int gr_val)
  {
    bool is_hvcfg_hve_valid = false;
    unsigned int cpu_type = get_cpu_milestone_helper((rh850g4::core*)(instr_base->read_core_ptr()));
    if((instr_base->read_HVCFG() & 0x1) == 0x1) is_hvcfg_hve_valid = true;
    switch (selid_val) {
    case 0: {
      if (regID == rh850g4::SR_SELID_0_INDEX_PSW_5){
	if ((cpu_type == rh850g4::core::FASTISS_G4MH_1_0) || (cpu_type == rh850g4::core::FASTISS_G4MH_1_1)){
	  instr_base->write_PSW(gr_val & 0x400380ff);
	}else{
	  if (instr_base->read_PSWH_GM()==0){
	    if((instr_base->read_INTCFG_EPL() & 0x1) ==  0x1){
	      instr_base->write_PSW(gr_val & 0x43f380ff);
	    }else{
	      instr_base->write_PSW(gr_val & 0x400380ff);
	    }
	  }else{
	      instr_base->write_PSW(gr_val & 0x43f380ff);
	  } 
	}
	// added for end the quantam
	terminate_quantum_helper((rh850g4::core*)(instr_base->read_core_ptr()));
      }else if (regID == rh850g4::SR_SELID_0_INDEX_EIPC_0){
	instr_base->write_EIPC(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_EIPSW_1){
	if ((cpu_type == rh850g4::core::FASTISS_G4MH_1_0) || (cpu_type == rh850g4::core::FASTISS_G4MH_1_1)){
	  instr_base->write_EIPSW(gr_val & 0x400380ff);
	}else{
	  if (instr_base->read_PSWH_GM()==0){
	    if((instr_base->read_INTCFG_EPL() & 0x1) ==  0x1){
	      instr_base->write_EIPSW(gr_val & 0x43f380ff);
	    }else{
	      instr_base->write_EIPSW(gr_val & 0x400380ff);
	    }
	  }else{
	    instr_base->write_EIPSW(gr_val & 0x43f380ff);
	  }
	}
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FEPC_2){
	instr_base->write_FEPC(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FEPSW_3){
	if ((cpu_type == rh850g4::core::FASTISS_G4MH_1_0) || (cpu_type == rh850g4::core::FASTISS_G4MH_1_1)){
	  instr_base->write_FEPSW(gr_val & 0x400380ff);
	}else{
	  if (instr_base->read_PSWH_GM()==0){
	    if((instr_base->read_INTCFG_EPL() & 0x1) ==  0x1){
	      instr_base->write_FEPSW(gr_val & 0x43f380ff);
	    }else{
	      instr_base->write_FEPSW(gr_val & 0x400380ff);
	    }
	  }else{
	      instr_base->write_FEPSW(gr_val & 0x43f380ff);
	  }
	}
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FPSR_6){
	instr_base->write_FPSR(gr_val | 0x00200000);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FPEPC_7){
	instr_base->write_FPEPC(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FPST_8){
	instr_base->write_FPST(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FPCC_9){
	instr_base->write_FPCC(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FPCFG_10){
	instr_base->write_FPCFG(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_EIIC_13){
	instr_base->write_EIIC(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FEIC_14){
	instr_base->write_FEIC(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_CTPSW_17){
	instr_base->write_CTPSW(gr_val);
      }else if ((regID == rh850g4::SR_SELID_0_INDEX_EIPSWH_18) ||
		(regID == rh850g4::SR_SELID_0_INDEX_FEPSWH_19)) {
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  if(is_hvcfg_hve_valid){
	    instr_base->write_SR_SELID_0(regID, gr_val);
	  }
	}
      }else if (regID == rh850g4::SR_SELID_0_INDEX_SNZCFG_21){
	instr_base->write_SNZCFG(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_EIWR_28){
	instr_base->write_EIWR(gr_val);
      }else if (regID == rh850g4::SR_SELID_0_INDEX_FEWR_29){
	instr_base->write_FEWR(gr_val);
      }else if (regID != rh850g4::SR_SELID_0_INDEX_PSWH_15){
	instr_base->write_SR_SELID_0(regID, gr_val);
      }
      break;
    }
    case 1: {
      if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_SPID_0){
	if(((instr_base->read_SVLOCK() & 0x1) == 0x0) &&
	   ((instr_base->read_SPIDLIST() & (0x1 << gr_val)) == (0x1 << gr_val)))
	  {
	    instr_base->write_SPID(gr_val);
	  }
      }else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_EBASE_3){
	instr_base->write_EBASE(gr_val);
      }else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_INTBP_4){
	instr_base->write_INTBP(gr_val);
      }else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_SVLOCK_8){
	instr_base->write_SVLOCK(gr_val);
      }else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_HVCFG_16){
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  instr_base->write_HVCFG(gr_val);
	}
      }else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_GMCFG_17){
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  if(is_hvcfg_hve_valid){
	    instr_base->write_GMCFG(gr_val & 0xfffbffff);
	  }
	}
      }else if ((rh850g4::SR_SELID_1_index_t)regID == rh850g4::SR_SELID_1_INDEX_HVSB_20){
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  if(is_hvcfg_hve_valid){
	    instr_base->write_SR_SELID_1((rh850g4::SR_SELID_1_index_t)regID, gr_val);
	  }
	}
      }else if (((rh850g4::SR_SELID_1_index_t)regID != rh850g4::SR_SELID_1_INDEX_RBASE_2) &&
		((rh850g4::SR_SELID_1_index_t)regID != rh850g4::SR_SELID_1_INDEX_SPIDLIST_1) &&
		((rh850g4::SR_SELID_1_index_t)regID != rh850g4::SR_SELID_1_INDEX_PID_6))
	{
	  instr_base->write_SR_SELID_1((rh850g4::SR_SELID_1_index_t)regID, gr_val);
	}
      break;
    }
    case 2: {
      if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_MEA_6){
	  instr_base->write_MEA(gr_val);
      }else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_MEI_8){
	  instr_base->write_MEI(gr_val);
      }else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_ISPR_10){
	if((instr_base->read_INTCFG_ISPC() & 0x1) == 0x1){
	  instr_base->write_SR_SELID_2((rh850g4::SR_SELID_2_index_t)regID, gr_val);
	  // added for end the quantam
	  terminate_quantum_helper((rh850g4::core*)(instr_base->read_core_ptr()));
	}
      }else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_INTCFG_13){
	instr_base->write_INTCFG(gr_val);  
      }else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_PLMR_14){
	  instr_base->write_PLMR(gr_val);
	  // added for end the quantam
	  terminate_quantum_helper((rh850g4::core*)(instr_base->read_core_ptr()));
      }else if ((rh850g4::SR_SELID_2_index_t)regID == rh850g4::SR_SELID_2_INDEX_RBNR_17){
	  instr_base->write_RBNR(gr_val);
      }else if (((rh850g4::SR_SELID_2_index_t)regID != rh850g4::SR_SELID_2_INDEX_PEID_0) &&
		((rh850g4::SR_SELID_2_index_t)regID != rh850g4::SR_SELID_2_INDEX_BMID_1) &&
		((rh850g4::SR_SELID_2_index_t)regID != rh850g4::SR_SELID_2_INDEX_IMSR_11) &&
		((rh850g4::SR_SELID_2_index_t)regID != rh850g4::SR_SELID_2_INDEX_ICSR_12))
        {
	  instr_base->write_SR_SELID_2((rh850g4::SR_SELID_2_index_t)regID, gr_val);
	}
      break;
    }
    case 3: {
      if ((rh850g4::SR_SELID_3_index_t)regID == rh850g4::SR_SELID_3_INDEX_DBGEN_0){
	instr_base->write_DBGEN(gr_val);
      }else if ((rh850g4::SR_SELID_3_index_t)regID == rh850g4::SR_SELID_3_INDEX_DBPSWH_13){
	instr_base->write_DBPSWH(gr_val);
      }else if ((rh850g4::SR_SELID_3_index_t)regID == rh850g4::SR_SELID_3_INDEX_DBPSW_19){
	if ((cpu_type == rh850g4::core::FASTISS_G4MH_1_0) || (cpu_type == rh850g4::core::FASTISS_G4MH_1_1)){
	  instr_base->write_DBPSW(gr_val & 0x400780ff);
	}else{
	  if (instr_base->read_PSWH_GM()==0){
	    if((instr_base->read_INTCFG_EPL() & 0x1) ==  0x1){
	      instr_base->write_DBPSW(gr_val & 0x43f780ff);
	    }else{
	      instr_base->write_DBPSW(gr_val & 0x400780ff);
	    }
	  }
	}
      }else if ((rh850g4::SR_SELID_3_index_t)regID == rh850g4::SR_SELID_3_INDEX_DIR0_20){
	instr_base->write_DIR0(gr_val);
      }else if ((rh850g4::SR_SELID_3_index_t)regID == rh850g4::SR_SELID_3_INDEX_DIR1_21){
	instr_base->write_DIR1(gr_val);
      }else if ((rh850g4::SR_SELID_3_index_t)regID == rh850g4::SR_SELID_3_INDEX_BPC_22){
	instr_base->write_BPC(gr_val);
      }else if ((rh850g4::SR_SELID_3_index_t)regID != rh850g4::SR_SELID_3_INDEX_DBCFG_28){
	instr_base->write_SR_SELID_3((rh850g4::SR_SELID_3_index_t)regID, gr_val);
      }
      break;
    }
    case 4: {
      if ((rh850g4::SR_SELID_4_index_t)regID != rh850g4::SR_SELID_4_INDEX_ICCFG_26)
	{
	  instr_base->write_SR_SELID_4((rh850g4::SR_SELID_4_index_t)regID, gr_val);
	}
      break;
    }
    case 5: {
      if ((rh850g4::SR_SELID_5_index_t)regID == rh850g4::SR_SELID_5_INDEX_MPCFG_2){
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  if(is_hvcfg_hve_valid){
	    instr_base->write_MPCFG(instr_base->read_MPCFG() | (gr_val & 0x00003f00) );
	  }
	}
      }else if ((rh850g4::SR_SELID_5_index_t)regID == rh850g4::SR_SELID_5_INDEX_MCR_11){
	if(is_hvcfg_hve_valid){
	  instr_base->write_MCR(gr_val);
	}else{
	  instr_base->write_MCR(gr_val & 0x0000013f);
	}
      }else if (((rh850g4::SR_SELID_5_index_t)regID != rh850g4::SR_SELID_5_INDEX_MPM_0) &&
		((rh850g4::SR_SELID_5_index_t)regID != rh850g4::SR_SELID_5_INDEX_MPIDX_16) &&
		((rh850g4::SR_SELID_5_index_t)regID != rh850g4::SR_SELID_5_INDEX_MPBK_17) &&
		((rh850g4::SR_SELID_5_index_t)regID != rh850g4::SR_SELID_5_INDEX_MPLA_20) &&
		((rh850g4::SR_SELID_5_index_t)regID != rh850g4::SR_SELID_5_INDEX_MPUA_21) &&
		((rh850g4::SR_SELID_5_index_t)regID != rh850g4::SR_SELID_5_INDEX_MPAT_22)){
	instr_base->write_SR_SELID_5((rh850g4::SR_SELID_5_index_t)regID, gr_val);
      } else {
	if((instr_base->read_SVLOCK() & 0x1) == 0x0)
	  {
	    if ((rh850g4::SR_SELID_5_index_t)regID == rh850g4::SR_SELID_5_INDEX_MPM_0){
	      instr_base->write_MPM(gr_val & 0xfffffffb);
	    }else if ((rh850g4::SR_SELID_5_index_t)regID == rh850g4::SR_SELID_5_INDEX_MPBK_17){
	      if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
		instr_base->write_MPBK(gr_val & 0x00000003);
	      }
	    }else{
	      instr_base->write_SR_SELID_5((rh850g4::SR_SELID_5_index_t)regID, gr_val);
	    }
	  }
      }
      break;
    }
    case 9: {
      if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	if(is_hvcfg_hve_valid){
	  if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMEIPSW_1){
	    instr_base->write_GMEIPSW(gr_val);
	  }else if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMFEPSW_3){
	    instr_base->write_GMFEPSW(gr_val);
	  }else if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMPSW_5){
	    instr_base->write_GMPSW(gr_val | 0x00008000);
	  }else if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMSPID_16){
	    instr_base->write_GMSPID(gr_val & 0x0000001f);
	  }else if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMINTCFG_21){
	    instr_base->write_GMINTCFG((gr_val & 0x003f0000) | 0x00000002);
	  }else if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMPLMR_22){
	    instr_base->write_GMPLMR(gr_val & 0x0000003f);
	    // added for end the quantam
	    terminate_quantum_helper((rh850g4::core*)(instr_base->read_core_ptr()));
	  }else if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMSVLOCK_24){
	    instr_base->write_GMSVLOCK(gr_val & 0x00000001);
	  }else if ((rh850g4::SR_SELID_9_index_t)regID == rh850g4::SR_SELID_9_INDEX_GMPEID_30){
	    instr_base->write_GMPEID(gr_val & 0x0000001f);
	  }else{
	    instr_base->write_SR_SELID_9((rh850g4::SR_SELID_9_index_t)regID, gr_val);
	  }
	}
      }
      break;
    }
    case 10: {
      if ((rh850g4::SR_SELID_10_index_t)regID == rh850g4::SR_SELID_10_INDEX_FXSR_6){
	instr_base->write_FXSR(gr_val);
      }else if ((rh850g4::SR_SELID_10_index_t)regID == rh850g4::SR_SELID_10_INDEX_FXST_8){
	instr_base->write_FXST(gr_val);
      }else if ((rh850g4::SR_SELID_10_index_t)regID == rh850g4::SR_SELID_10_INDEX_FXCFG_10){
	instr_base->write_FXCFG(gr_val);
      }else if ((rh850g4::SR_SELID_10_index_t)regID == rh850g4::SR_SELID_10_INDEX_FXXC_12){
	instr_base->write_FXXC(gr_val);
      }else if ((rh850g4::SR_SELID_10_index_t)regID == rh850g4::SR_SELID_10_INDEX_FXXP_13){
	instr_base->write_FXXP(gr_val);
      }
      // FXINFO is read only register.
      break;
    }
    case 11: {
      if ((cpu_type == rh850g4::core::FASTISS_G4MH_1_0) || (cpu_type == rh850g4::core::FASTISS_G4MH_1_1)){
	if ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)9){
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)regID, gr_val);
	}
      }else{
	if ((rh850g4::SR_SELID_11_index_t)regID == (rh850g4::SR_SELID_11_index_t)9){
	  if(is_hvcfg_hve_valid){
	    instr_base->write_PMGMCTRL(gr_val);
	  }
	}else if (((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)16) &&
	    ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)17) &&
	    ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)18) &&
	    ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)19) &&
	    ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)20) &&
	    ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)21) &&
	    ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)22) &&
	    ((rh850g4::SR_SELID_11_index_t)regID != (rh850g4::SR_SELID_11_index_t)23)){
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)regID, gr_val);
	}
      }
      break;
    }
    case 12: {	
      if (((rh850g4::SR_SELID_12_index_t)regID != (rh850g4::SR_SELID_12_index_t)2) &&
	  ((rh850g4::SR_SELID_12_index_t)regID != (rh850g4::SR_SELID_12_index_t)10)){
	instr_base->write_SR_SELID_12((rh850g4::SR_SELID_12_index_t)regID, gr_val);
      }
      break;
    }
    case 13: {
      if ((rh850g4::SR_SELID_13_index_t)regID == (rh850g4::SR_SELID_13_index_t)16){
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  instr_base->write_DECFG(gr_val & 0x7);
	}
      }else if ((rh850g4::SR_SELID_13_index_t)regID == (rh850g4::SR_SELID_13_index_t)17){
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  if(gr_val == 0){
	    instr_base->write_DECTRL(gr_val);
	  }
	}
      }else if ((rh850g4::SR_SELID_13_index_t)regID == (rh850g4::SR_SELID_13_index_t)18) {
	if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	  gr_val = gr_val & 0x100ff;
	  unsigned int update_val = instr_base->read_DEVDS() & (~gr_val);
	  instr_base->write_DEVDS(update_val);
	}
      }else if (((rh850g4::SR_SELID_13_index_t)regID != (rh850g4::SR_SELID_13_index_t)2) &&
		((rh850g4::SR_SELID_13_index_t)regID != (rh850g4::SR_SELID_13_index_t)3) &&
		((rh850g4::SR_SELID_13_index_t)regID != (rh850g4::SR_SELID_13_index_t)9) &&
		((rh850g4::SR_SELID_13_index_t)regID != (rh850g4::SR_SELID_13_index_t)10) &&
		((rh850g4::SR_SELID_13_index_t)regID != (rh850g4::SR_SELID_13_index_t)12)){
	instr_base->write_SR_SELID_13((rh850g4::SR_SELID_13_index_t)regID, gr_val);
      }
      break;
    }
    case 14: {
      if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)0){
	  // PMCTRL0
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)17, gr_val);
	}else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)1){
	  // PMCTRL1
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)19, gr_val);
	}else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)2){
	  // PMCTRL2
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)21, gr_val);
	}else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)3){
	  // PMCTRL3
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)23, gr_val);
	}else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)16){
	  // PMCOUNT0
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)16, gr_val);
	}else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)17){
	  // PMCOUNT1
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)18, gr_val);
	}else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)18){
	  // PMCOUNT2
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)20, gr_val);
	}else if ((rh850g4::SR_SELID_14_index_t)regID != (rh850g4::SR_SELID_14_index_t)19){
	  // PMCOUNT3
	  instr_base->write_SR_SELID_11((rh850g4::SR_SELID_11_index_t)22, gr_val);
	}else{
	  instr_base->write_SR_SELID_14((rh850g4::SR_SELID_14_index_t)regID, gr_val);
	}
      }
      break;
    }
    case 15: {
      if (cpu_type == rh850g4::core::FASTISS_G4MH_2_0){
	instr_base->write_SR_SELID_15((rh850g4::SR_SELID_15_index_t)regID, gr_val);
      }
      break;
    }
    }
  }
}

rh850g4::ldsr_reg2_regid_selid_instr_t::ldsr_reg2_regid_selid_instr_t
(
  fastiss::code_gen_if_t *code_gen_impl,
  rh850g4::memory_if_t *memory_impl,
  rh850g4::operand_if_t *operand_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
) :
  rh850g4::instruction_base_impl_t(code_gen_impl, memory_impl, operand_impl, cpu_state_impl, custom_impl)
{
  fastiss::func_args_type_t *ldsr_regid_selid_reg2_helper_args = code_gen_impl->create_args_type();
  code_gen_impl->append_ptr(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i32(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i8(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->append_i32(ldsr_regid_selid_reg2_helper_args);
  code_gen_impl->add_helper("ldsr_regid_selid_reg2_helper", (void*)&rh850g4::ldsr_regid_selid_reg2_helper, code_gen_impl->create_void_func(ldsr_regid_selid_reg2_helper_args));
  code_gen_impl->delete_args_type(ldsr_regid_selid_reg2_helper_args);
}

rh850g4::ldsr_reg2_regid_selid_instr_t::~ldsr_reg2_regid_selid_instr_t()
{
}

void rh850g4::ldsr_reg2_regid_selid_instr_t::decode(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  // Increment PC
  increment_trans_PC(4);  
}

std::string rh850g4::ldsr_reg2_regid_selid_instr_t::disassemble(rh850g4::opcode_t opcode, unsigned int trans_PC)
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

unsigned int rh850g4::ldsr_reg2_regid_selid_instr_t::length(rh850g4::opcode_t opcode, unsigned int trans_PC)
{
  return 4;
}

void rh850g4::ldsr_reg2_regid_selid_instr_t::execute(opcode_t opcode, unsigned int trans_PC)
{
  // Extract operands
  GR_index_t reg2 = get_reg2_u8_0_4(opcode);
  SR_SELID_0_index_t regID = get_regID_u8_11_15(opcode);
  fastiss::i8_t *selID = gen_get_selID_u8_27_31(opcode);

  fastiss::i8_ptr_t *instr_base = gen_i8_constant_ptr(dynamic_cast<rh850g4::immediate_base_t*>(this));
  call_helper("ldsr_regid_selid_reg2_helper", instr_base, gen_i32_constant(regID), selID, gen_read_GR(reg2), NULL);

  // Update PC
  gen_write_PC(gen_i32_constant(trans_PC + length(opcode, trans_PC)));

  // Update instruction and cycle counts
  gen_increment_instruction_count();
  gen_increment_cycle_count(gen_i32_constant(cycle_count(opcode, trans_PC)));
}

bool rh850g4::ldsr_reg2_regid_selid_instr_t::terminates_block(opcode_t opcode, unsigned int trans_PC)
{
  return true;
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
