/*************************************************************************
 *
 * Copyright(c) 2015-2018, 2020 Renesas Electronics Corporation
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
// or intended publication of such source code./
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/custom_cpu_state_impl.h"
#include "fastiss/code_gen_if.h"

//------------------------------------------------------------------------------
// rh850g4 custom CPU state implementation
//------------------------------------------------------------------------------

rh850g4::custom_cpu_state_impl_t::custom_cpu_state_impl_t() :
  rh850g4::cpu_state_impl_t()
{
}

rh850g4::custom_cpu_state_impl_t::~custom_cpu_state_impl_t()
{
}

// Over-ride the general purpose register name for the special case
// general registers, i.e. for the SP, GP, TP, EP, and LP cases
std::string rh850g4::custom_cpu_state_impl_t::register_name_r3(void)
{
  return std::string("sp");
}

std::string rh850g4::custom_cpu_state_impl_t::register_name_r4(void)
{
  return std::string("gp");
}

std::string rh850g4::custom_cpu_state_impl_t::register_name_r5(void)
{
  return std::string("tp");
}

std::string rh850g4::custom_cpu_state_impl_t::register_name_r30(void)
{
  return std::string("ep");
}

std::string rh850g4::custom_cpu_state_impl_t::register_name_r31(void)
{
  return std::string("lp");
}

// The RH850G4 r0 register always holds 0 and it's value should never be
// changed. Over-ride the CPU State functions that alter r0 to ensure this.
void rh850g4::custom_cpu_state_impl_t::write_r0(unsigned int value)
{
}

void rh850g4::custom_cpu_state_impl_t::gen_write_r0(fastiss::i32_t *value)
{
}

// A contant zero value can be returned for r0 as it cannot change value.
unsigned int rh850g4::custom_cpu_state_impl_t::read_r0()
{
  return 0;
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_r0()
{
  return m_code_gen_impl->gen_i32_constant(0);
}

// The RH850G4 PC register has bit 0 fixed to 0. Over-ride the CPU State
// functions that alter PC to mask the bit 0 value.
void rh850g4::custom_cpu_state_impl_t::write_PC(unsigned int value)
{
  cpu_state_impl_t::write_PC(value & 0xfffffffe);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffe);
  fastiss::i32_t *masked_pc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_PC(masked_pc_value);
}

// The RH850G4 EIPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter EIPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_EIPC(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EIPC(value & 0xffffffff);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_EIPC(value & 0xffffffff);
    }else{
      cpu_state_impl_t::write_GMEIPC(value & 0xffffffff);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EIPC(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
    fastiss::i32_t *masked_eipc_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_EIPC(masked_eipc_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
    fastiss::i32_t *masked_eipc_value = m_code_gen_impl->gen_and(value, mask);
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EIPC( m_code_gen_impl->gen_select(is_host_mode, masked_eipc_value, cpu_state_impl_t::gen_read_EIPC()));
    cpu_state_impl_t::gen_write_GMEIPC( m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEIPC(), masked_eipc_value));
  }
}

// The RH850G4 EIPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter EIPSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_EIPSW(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EIPSW(value & 0x400380ff);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      if((cpu_state_impl_t::read_INTCFG_EPL() & 0x1) ==  0x1){
	cpu_state_impl_t::write_EIPSW(value & 0x43f380ff);
      }else{
	cpu_state_impl_t::write_EIPSW(value & 0x400380ff);
      }
    }else{
      cpu_state_impl_t::write_GMEIPSW(value & 0x43f380ff);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EIPSW(fastiss::i32_t *value)
{

  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x400380ff);
    fastiss::i32_t *masked_eipsw_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_EIPSW(masked_eipsw_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));

    fastiss::i1_t *elp_val = m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_INTCFG_EPL(), cpu_state_impl_t::gen_read_GMINTCFG_EPL());
    fastiss::i32_t *mask = m_code_gen_impl->gen_select(elp_val, m_code_gen_impl->gen_i32_constant(0x43f380ff), m_code_gen_impl->gen_i32_constant(0x400380ff));
    fastiss::i32_t *masked_eipsw_value = m_code_gen_impl->gen_and(value, mask);

    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EIPSW(m_code_gen_impl->gen_select(is_host_mode, masked_eipsw_value, cpu_state_impl_t::gen_read_EIPSW()));
    cpu_state_impl_t::gen_write_GMEIPSW(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEIPSW(), masked_eipsw_value));
  }
}

// The RH850G4 FEPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter FEPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FEPC(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_FEPC(value & 0xffffffff);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_FEPC(value & 0xffffffff);
    }else{
      cpu_state_impl_t::write_GMFEPC(value & 0xffffffff);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FEPC(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
    fastiss::i32_t *masked_fepc_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_FEPC(masked_fepc_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
    fastiss::i32_t *masked_fepc_value = m_code_gen_impl->gen_and(value, mask);
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_FEPC(m_code_gen_impl->gen_select(is_host_mode, masked_fepc_value, cpu_state_impl_t::gen_read_FEPC()));
    cpu_state_impl_t::gen_write_GMFEPC(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMFEPC(), masked_fepc_value));
  }
}

// The RH850G4 FEPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter FEPSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FEPSW(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_FEPSW(value & 0x400380ff);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      if((cpu_state_impl_t::read_INTCFG_EPL() & 0x1) ==  0x1){
	cpu_state_impl_t::write_FEPSW(value & 0x43f380ff);
      }else{
	cpu_state_impl_t::write_FEPSW(value & 0x400380ff);
      }
    }else{
      cpu_state_impl_t::write_GMFEPSW(value & 0x43f380ff);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FEPSW(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask =  m_code_gen_impl->gen_i32_constant(0x400380ff);
    fastiss::i32_t *masked_fepsw_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_FEPSW(masked_fepsw_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), 
										       m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));

    fastiss::i1_t *elp_val = m_code_gen_impl->gen_select(is_host_mode, 
							 cpu_state_impl_t::gen_read_INTCFG_EPL(), 
							 cpu_state_impl_t::gen_read_GMINTCFG_EPL());
    fastiss::i32_t *mask = m_code_gen_impl->gen_select(elp_val, 
						       m_code_gen_impl->gen_i32_constant(0x43f380ff), 
						       m_code_gen_impl->gen_i32_constant(0x400380ff));
    fastiss::i32_t *masked_fepsw_value = m_code_gen_impl->gen_and(value, mask);

    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_FEPSW(m_code_gen_impl->gen_select(is_host_mode, masked_fepsw_value, cpu_state_impl_t::gen_read_FEPSW()));
    cpu_state_impl_t::gen_write_GMFEPSW(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMFEPSW(), masked_fepsw_value));
  }
}

// The RH850G4 PSW register has bits 31, 29-20, 18, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter PSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_PSW(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW(value & 0x400380ff);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      if((cpu_state_impl_t::read_INTCFG_EPL() & 0x1) ==  0x1){
	cpu_state_impl_t::write_PSW(value & 0x43f380ff);
      }else{
	cpu_state_impl_t::write_PSW(value & 0x400380ff);
      }
    }else{
      value = value | 0x00008000;
      unsigned int mask_val = 0x43f380ff;
      if(cpu_state_impl_t::read_GMCFG_GCU1() == 0) mask_val = mask_val & 0xfffdffff;
      if(cpu_state_impl_t::read_GMCFG_GCU0() == 0) mask_val = mask_val & 0xfffeffff;
      cpu_state_impl_t::write_GMPSW(value & mask_val);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask =  m_code_gen_impl->gen_i32_constant(0x400380ff);
    fastiss::i32_t *masked_psw_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_PSW(masked_psw_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), 
										       m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));

    fastiss::i1_t *elp_val = m_code_gen_impl->gen_select(is_host_mode, 
							 cpu_state_impl_t::gen_read_INTCFG_EPL(), 
							 cpu_state_impl_t::gen_read_GMINTCFG_EPL());
    fastiss::i32_t *mask =  m_code_gen_impl->gen_select(elp_val, 
							m_code_gen_impl->gen_i32_constant(0x43f380ff), 
							m_code_gen_impl->gen_i32_constant(0x400380ff));
    fastiss::i32_t *masked_psw_value = m_code_gen_impl->gen_and(value, mask);
 
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW(m_code_gen_impl->gen_select(is_host_mode, masked_psw_value, cpu_state_impl_t::gen_read_PSW()));

    mask =  m_code_gen_impl->gen_and(mask, m_code_gen_impl->gen_select(m_code_gen_impl->gen_icmp_eq(cpu_state_impl_t::gen_read_GMCFG_GCU1(), 
												    m_code_gen_impl->gen_i1_constant(0)),
								       m_code_gen_impl->gen_i32_constant(0xfffdffff), 
								       m_code_gen_impl->gen_i32_constant(0xffffffff)));
    masked_psw_value = m_code_gen_impl->gen_and(masked_psw_value, mask);
    mask =  m_code_gen_impl->gen_and(mask, m_code_gen_impl->gen_select(m_code_gen_impl->gen_icmp_eq(cpu_state_impl_t::gen_read_GMCFG_GCU0(), 
												    m_code_gen_impl->gen_i1_constant(0)),
								       m_code_gen_impl->gen_i32_constant(0xfffeffff), 
								       m_code_gen_impl->gen_i32_constant(0xffffffff)));					
    masked_psw_value = m_code_gen_impl->gen_and(masked_psw_value, mask);
    cpu_state_impl_t::gen_write_GMPSW(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW(), masked_psw_value));
  }
}

// Writing to the RH850G4 PSW register Z, S, OV, CY, SAT, ID, EP, NP, 
// EBV, CU0-2, EIMASK, UM fields
void rh850g4::custom_cpu_state_impl_t::write_PSW_Z(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_Z(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_Z(value);
    }else{
      cpu_state_impl_t::write_GMPSW_Z(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_Z(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_Z(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_Z(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_Z()));
    cpu_state_impl_t::gen_write_GMPSW_Z(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_Z(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_S(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_S(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_S(value);
    }else{
      cpu_state_impl_t::write_GMPSW_S(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_S(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_S(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_S(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_S()));
    cpu_state_impl_t::gen_write_GMPSW_S(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_S(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_OV(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_OV(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_OV(value);
    }else{
      cpu_state_impl_t::write_GMPSW_OV(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_OV(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_OV(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_OV(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_OV()));
    cpu_state_impl_t::gen_write_GMPSW_OV(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_OV(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_CY(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_CY(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_CY(value);
    }else{
      cpu_state_impl_t::write_GMPSW_CY(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_CY(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_CY(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_CY(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_CY()));
    cpu_state_impl_t::gen_write_GMPSW_CY(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_CY(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_SAT(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_SAT(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_SAT(value);
    }else{
      cpu_state_impl_t::write_GMPSW_SAT(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_SAT(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_SAT(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_SAT(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_SAT()));
    cpu_state_impl_t::gen_write_GMPSW_SAT(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_SAT(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_ID(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_ID(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_ID(value);
    }else{
      cpu_state_impl_t::write_GMPSW_ID(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_ID(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_ID(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_ID(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_ID()));
    cpu_state_impl_t::gen_write_GMPSW_ID(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_ID(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_EP(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_EP(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_EP(value);
    }else{
      cpu_state_impl_t::write_GMPSW_EP(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_EP(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_EP(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_EP(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_EP()));
    cpu_state_impl_t::gen_write_GMPSW_EP(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_EP(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_NP(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_NP(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_NP(value);
    }else{
      cpu_state_impl_t::write_GMPSW_NP(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_NP(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_NP(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_NP(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_NP()));
    cpu_state_impl_t::gen_write_GMPSW_NP(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_NP(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_EBV(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_EBV(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_EBV(value);
    }else{
      // GMPSW.EBV is fixed 1.
      cpu_state_impl_t::write_GMPSW_EBV(1);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_EBV(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_EBV(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_EBV(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_EBV()));
    // GMPSW.EBV is fixed 1.
    cpu_state_impl_t::gen_write_GMPSW_EBV(m_code_gen_impl->gen_i1_constant(1));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_CU0(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_CU0(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_CU0(value);
    }else{
      if(cpu_state_impl_t::read_GMCFG_GCU0() == 1){
	cpu_state_impl_t::write_GMPSW_CU0(value);
      }else{
	cpu_state_impl_t::write_GMPSW_CU0(0);
      }
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_CU0(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_CU0(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_CU0(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_CU0()));
    cpu_state_impl_t::gen_write_GMPSW_CU0(m_code_gen_impl->gen_select(is_host_mode, 
								      cpu_state_impl_t::gen_read_GMPSW_CU0(),
								      m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU0(),
												  value,
												  m_code_gen_impl->gen_i1_constant(0))));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_CU1(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_CU1(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_CU1(value);
    }else{
      if(cpu_state_impl_t::read_GMCFG_GCU1() == 1){
	cpu_state_impl_t::write_GMPSW_CU1(value);
      }else{
	cpu_state_impl_t::write_GMPSW_CU1(0);
      }
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_CU1(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_CU1(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), 
										       m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_CU1(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_CU1()));
    cpu_state_impl_t::gen_write_GMPSW_CU1(m_code_gen_impl->gen_select(is_host_mode, 
								      cpu_state_impl_t::gen_read_GMPSW_CU1(), 
								      m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU1(),
												  value,
												  m_code_gen_impl->gen_i1_constant(0))));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_CU2(unsigned char value)
{
  // PSW.CU2 and GMPSW.CU2 are fixed 0.
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_CU2(0);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_CU2(0);
    }else{
      cpu_state_impl_t::write_GMPSW_CU2(0);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_CU2(fastiss::i1_t *value)
{
  // PSW.CU2 and GMPSW.CU2 are fixed 0.
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_CU2(m_code_gen_impl->gen_i1_constant(0));
  }else{
    cpu_state_impl_t::gen_write_PSW_CU2(m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_GMPSW_CU2(m_code_gen_impl->gen_i1_constant(0));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_EIMASK(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_EIMASK(0);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      if((cpu_state_impl_t::read_INTCFG_EPL() & 0x1) ==  0x1){
	cpu_state_impl_t::write_PSW_EIMASK(value & 0x3f);
      }else{
	cpu_state_impl_t::write_PSW_EIMASK(0x0);
      }
    }else{
      cpu_state_impl_t::write_GMPSW_EIMASK(value & 0x3f);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_EIMASK(fastiss::i8_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_EIMASK(m_code_gen_impl->gen_i8_constant(0));
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));

    fastiss::i1_t *elp_val = m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_INTCFG_EPL(), cpu_state_impl_t::gen_read_GMINTCFG_EPL());
    fastiss::i8_t *mask = m_code_gen_impl->gen_select(elp_val, m_code_gen_impl->gen_i8_constant(0x3f), m_code_gen_impl->gen_i8_constant(0x0));
    fastiss::i8_t *masked_eimask_value = m_code_gen_impl->gen_and(value, mask);

    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_EIMASK(m_code_gen_impl->gen_select(is_host_mode, 
								       masked_eimask_value,
								       cpu_state_impl_t::gen_read_PSW_EIMASK()));
    cpu_state_impl_t::gen_write_GMPSW_EIMASK(m_code_gen_impl->gen_select(is_host_mode, 
									 cpu_state_impl_t::gen_read_GMPSW_EIMASK(), 
									 masked_eimask_value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSW_UM(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSW_UM(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PSW_UM(value);
    }else{
      cpu_state_impl_t::write_GMPSW_UM(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSW_UM(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSW_UM(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PSW_UM(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_PSW_UM()));
    cpu_state_impl_t::gen_write_GMPSW_UM(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPSW_UM(), value));
  }
}

// Writing to the RH850G4 FPSR register IF, XC, XP, CC, RM, or XE fields
// also writes the values to the corresponding fields in the FPST, FPCC,
// and FPCFG registers.
void rh850g4::custom_cpu_state_impl_t::write_FPSR_IF(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_IF(value);
  cpu_state_impl_t::write_FPST_IF(read_FPSR_IF());
}

void rh850g4::custom_cpu_state_impl_t::write_FPSR_XC(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_XC(value);
  cpu_state_impl_t::write_FPST_XC(read_FPSR_XC());
}

void rh850g4::custom_cpu_state_impl_t::write_FPSR_XP(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_XP(value);
  cpu_state_impl_t::write_FPST_XP(read_FPSR_XP());
}

void rh850g4::custom_cpu_state_impl_t::write_FPSR_CC(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_CC(value);
  cpu_state_impl_t::write_FPCC_CC(read_FPSR_CC());
}

void rh850g4::custom_cpu_state_impl_t::write_FPSR_RM(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_RM(value);
  cpu_state_impl_t::write_FPCFG_RM(read_FPSR_RM());
}

void rh850g4::custom_cpu_state_impl_t::write_FPSR_XE(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_XE(value);
  cpu_state_impl_t::write_FPCFG_XE(read_FPSR_XE());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPSR_IF(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_IF(value);
  cpu_state_impl_t::gen_write_FPST_IF(gen_read_FPSR_IF());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPSR_XC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_XC(value);
  cpu_state_impl_t::gen_write_FPST_XC(gen_read_FPSR_XC());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPSR_XP(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_XP(value);
  cpu_state_impl_t::gen_write_FPST_XP(gen_read_FPSR_XP());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPSR_CC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_CC(value);
  cpu_state_impl_t::gen_write_FPCC_CC(gen_read_FPSR_CC());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPSR_RM(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_RM(value);
  cpu_state_impl_t::gen_write_FPCFG_RM(gen_read_FPSR_RM());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPSR_XE(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_XE(value);
  cpu_state_impl_t::gen_write_FPCFG_XE(gen_read_FPSR_XE());
}

// The RH850G4 FPEPC register has bits 0 fixed to 0. Over-ride
// the CPU State functions that alter FPEPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FPEPC(unsigned int value)
{
  cpu_state_impl_t::write_FPEPC(value & 0xfffffffe);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPEPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffe);
  fastiss::i32_t *masked_fpepc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FPEPC(masked_fpepc_value);
}

// Writing to the RH850G4 FPST register IF, XC, or XP fields also writes the
// values to the corresponding fields in the FPSR register.
void rh850g4::custom_cpu_state_impl_t::write_FPST_IF(unsigned char value)
{
  cpu_state_impl_t::write_FPST_IF(value);
  cpu_state_impl_t::write_FPSR_IF(read_FPST_IF());
}

void rh850g4::custom_cpu_state_impl_t::write_FPST_XC(unsigned char value)
{
  cpu_state_impl_t::write_FPST_XC(value);
  cpu_state_impl_t::write_FPSR_XC(read_FPST_XC());
}

void rh850g4::custom_cpu_state_impl_t::write_FPST_XP(unsigned char value)
{
  cpu_state_impl_t::write_FPST_XP(value);
  cpu_state_impl_t::write_FPSR_XP(read_FPST_XP());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPST_IF(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_FPST_IF(value);
  cpu_state_impl_t::gen_write_FPSR_IF(gen_read_FPST_IF());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPST_XC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPST_XC(value);
  cpu_state_impl_t::gen_write_FPSR_XC(gen_read_FPST_XC());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPST_XP(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPST_XP(value);
  cpu_state_impl_t::gen_write_FPSR_XP(gen_read_FPST_XP());
}

// Writing to the RH850G4 FPCC register CC fields also writes the
// values to the corresponding fields in the FPSR register.
void rh850g4::custom_cpu_state_impl_t::write_FPCC_CC(unsigned char value)
{
  cpu_state_impl_t::write_FPCC_CC(value);
  cpu_state_impl_t::write_FPSR_CC(read_FPCC_CC());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPCC_CC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPCC_CC(value);
  cpu_state_impl_t::gen_write_FPSR_CC(gen_read_FPCC_CC());
}

// Writing to the RH850G4 FPCFG register RM or XE fields also writes the
// values to the corresponding fields in the FPSR register.
void rh850g4::custom_cpu_state_impl_t::write_FPCFG_RM(unsigned char value)
{
  cpu_state_impl_t::write_FPCFG_RM(value);
  cpu_state_impl_t::write_FPSR_RM(read_FPCFG_RM());
}

void rh850g4::custom_cpu_state_impl_t::write_FPCFG_XE(unsigned char value)
{
  cpu_state_impl_t::write_FPCFG_XE(value);
  cpu_state_impl_t::write_FPSR_XE(read_FPCFG_XE());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPCFG_RM(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPCFG_RM(value);
  cpu_state_impl_t::gen_write_FPSR_RM(gen_read_FPCFG_RM());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FPCFG_XE(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPCFG_XE(value);
  cpu_state_impl_t::gen_write_FPSR_XE(gen_read_FPCFG_XE());
}

// The RH850G4 EIIC register has the guest mode register.
// Over-ride the CPU State functions that alter EIIC to select the target regsiters.
void rh850g4::custom_cpu_state_impl_t::write_EIIC(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EIIC(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_EIIC(value);
    }else{
      cpu_state_impl_t::write_GMEIIC(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EIIC(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EIIC(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EIIC(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_EIIC()));
    cpu_state_impl_t::gen_write_GMEIIC(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEIIC(), value));
  }
}

// The RH850G4 FEIC register has the guest mode register.
// Over-ride the CPU State functions that alter FEIC to select the target regsiters.
void rh850g4::custom_cpu_state_impl_t::write_FEIC(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_FEIC(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_FEIC(value);
    }else{
      cpu_state_impl_t::write_GMFEIC(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FEIC(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_FEIC(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_FEIC(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_FEIC()));
    cpu_state_impl_t::gen_write_GMFEIC(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMFEIC(), value));
  }
}

// The RH850G4 CTPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter CTPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_CTPC(unsigned int value)
{
  cpu_state_impl_t::write_CTPC(value & 0xffffffff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_CTPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_ctpc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_CTPC(masked_ctpc_value);
}

// The RH850G4 CTPSW register has bits 31-5 fixed to 0. Over-ride
// the CPU State functions that alter CTPSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_CTPSW(unsigned int value)
{
  cpu_state_impl_t::write_CTPSW(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_CTPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_ctpsw_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_CTPSW(masked_ctpsw_value);
}

// The RH850G4 CTBP register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter CTBP to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_CTBP(unsigned int value)
{
  cpu_state_impl_t::write_CTBP(value & 0xfffffffe);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_CTBP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffe);
  fastiss::i32_t *masked_ctbp_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_CTBP(masked_ctbp_value);
}

// The RH850G4 SNZCFG register has bit 31-8 fixed to 0.
// the CPU State functions that alter SNZCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_SNZCFG(unsigned int value)
{
  cpu_state_impl_t::write_SNZCFG(value & 0x000000ff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_SNZCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
  fastiss::i32_t *masked_snzcfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_SNZCFG(masked_snzcfg_value);
}

// The RH850G4 EIWR register has the guest mode register.
// Over-ride the CPU State functions that alter EIWR to select the target regsiters.
void rh850g4::custom_cpu_state_impl_t::write_EIWR(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EIWR(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_EIWR(value);
    }else{
      cpu_state_impl_t::write_GMEIWR(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EIWR(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EIWR(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EIWR(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_EIWR()));
    cpu_state_impl_t::gen_write_GMEIWR(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEIWR(), value));
  }
}

// The RH850G4 FEWR register has the guest mode register.
// Over-ride the CPU State functions that alter FEWR to select the target regsiters.
void rh850g4::custom_cpu_state_impl_t::write_FEWR(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_FEWR(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_FEWR(value);
    }else{
      cpu_state_impl_t::write_GMFEWR(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FEWR(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_FEWR(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_FEWR(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_FEWR()));
    cpu_state_impl_t::gen_write_GMFEWR(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMFEWR(), value));
  }
}

// The RH850G4 SPID register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter SPID to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_SPID(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_SPID(value & 0x0000001f);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_SPID(value & 0x0000001f);
    }else{
      cpu_state_impl_t::write_GMSPID(value & 0x0000001f);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_SPID(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_spid_value = m_code_gen_impl->gen_and(value, mask);
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_SPID(masked_spid_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_SPID(m_code_gen_impl->gen_select(is_host_mode, masked_spid_value, cpu_state_impl_t::gen_read_SPID()));
    cpu_state_impl_t::gen_write_GMSPID(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_SPID(), masked_spid_value));
  }
}

// The RH850G4 SPIDLIST register has the guest mode register.
// Over-ride the CPU State functions that alter SPIDLIST to select the target regsiters.
void rh850g4::custom_cpu_state_impl_t::write_SPIDLIST(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_SPIDLIST(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_SPIDLIST(value);
    }else{
      cpu_state_impl_t::write_GMSPIDLIST(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_SPIDLIST(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_SPIDLIST(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_SPIDLIST(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_SPIDLIST()));
    cpu_state_impl_t::gen_write_GMSPIDLIST(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMSPIDLIST(), value));
  }
}

// The RH850G4 RBASE register has bit 8-2 fixed to 0. Over-ride
// the CPU State functions that alter RBASE to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_RBASE(unsigned int value)
{
  cpu_state_impl_t::write_RBASE(value & 0xfffffe03);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_RBASE(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffe03);
  fastiss::i32_t *masked_rbase_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_RBASE(masked_rbase_value);
}

// The RH850G4 EBASE register has bit 8-2 fixed to 0. Over-ride
// the CPU State functions that alter EBASE to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_EBASE(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EBASE(value & 0xfffffe03);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_EBASE(value & 0xfffffe03);
    }else{
      cpu_state_impl_t::write_GMEBASE(value & 0xfffffe03);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EBASE(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffe03);
  fastiss::i32_t *masked_ebase_value = m_code_gen_impl->gen_and(value, mask);
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EBASE(masked_ebase_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EBASE(m_code_gen_impl->gen_select(is_host_mode, masked_ebase_value, cpu_state_impl_t::gen_read_EBASE()));
    cpu_state_impl_t::gen_write_GMEBASE(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEBASE(), masked_ebase_value));
  }
}

// Writing to the RH850G4 EBASE register EBASE, DV, RINT fields
void rh850g4::custom_cpu_state_impl_t::write_EBASE_EBASE(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EBASE_EBASE(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_EBASE_EBASE(value);
    }else{
      cpu_state_impl_t::write_GMEBASE_EBASE(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EBASE_EBASE(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EBASE_EBASE(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EBASE_EBASE(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_EBASE_EBASE()));
    cpu_state_impl_t::gen_write_GMEBASE_EBASE(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEBASE_EBASE(), value));
  }
}
void rh850g4::custom_cpu_state_impl_t::write_EBASE_DV(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EBASE_DV(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_EBASE_DV(value);
    }else{
      cpu_state_impl_t::write_GMEBASE_DV(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EBASE_DV(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EBASE_DV(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EBASE_DV(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_EBASE_DV()));
    cpu_state_impl_t::gen_write_GMEBASE_DV(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEBASE_DV(), value));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_EBASE_RINT(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EBASE_RINT(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_EBASE_RINT(value);
    }else{
      cpu_state_impl_t::write_GMEBASE_RINT(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EBASE_RINT(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EBASE_RINT(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_EBASE_RINT(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_EBASE_RINT()));
    cpu_state_impl_t::gen_write_GMEBASE_RINT(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMEBASE_RINT(), value));
  }
}

// The RH850G4 INTBP register has bit 8-0 fixed to 0. Over-ride
// the CPU State functions that alter INTBP to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_INTBP(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_INTBP(value & 0xfffffe00);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_INTBP(value & 0xfffffe00);
    }else{
      cpu_state_impl_t::write_GMINTBP(value & 0xfffffe00);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_INTBP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffe00);
  fastiss::i32_t *masked_intbp_value = m_code_gen_impl->gen_and(value, mask);
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_INTBP(masked_intbp_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_INTBP(m_code_gen_impl->gen_select(is_host_mode, masked_intbp_value, cpu_state_impl_t::gen_read_INTBP()));
    cpu_state_impl_t::gen_write_GMINTBP(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMINTBP(), masked_intbp_value));
  }
}

// The RH850G4 MCTL register has bit 31-1 fixed to 0. Over-ride
// the CPU State functions that alter MCTL to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MCTL(unsigned int value)
{
  cpu_state_impl_t::write_MCTL(value & 0x1);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MCTL(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x1);
  fastiss::i32_t *masked_mctl_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MCTL(masked_mctl_value);
}

// The RH850G4 SVLOCK register has bit 31-1 fixed to 0. Over-ride
// the CPU State functions that alter SVLOCK to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_SVLOCK(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_SVLOCK(value & 0x1);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_SVLOCK(value & 0x1);
    }else{
      cpu_state_impl_t::write_GMSVLOCK(value & 0x1);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_SVLOCK(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x1);
  fastiss::i32_t *masked_svlock_value = m_code_gen_impl->gen_and(value, mask);
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_SVLOCK(masked_svlock_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_SVLOCK(m_code_gen_impl->gen_select(is_host_mode, masked_svlock_value, cpu_state_impl_t::gen_read_SVLOCK()));
    cpu_state_impl_t::gen_write_GMSVLOCK(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMSVLOCK(), masked_svlock_value));
  }
}

// The RH850G4 SCCFG register has bit 31-8 fixed to 0. Over-ride
// the CPU State functions that alter SCCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_SCCFG(unsigned int value)
{
  cpu_state_impl_t::write_SCCFG(value & 0x000000ff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_SCCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
  fastiss::i32_t *masked_sccfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_SCCFG(masked_sccfg_value);
}

// The RH850G4 SCBP register has bits 1 and 0 fixed to 0. Over-ride
// the CPU State functions that alter SCBP to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_SCBP(unsigned int value)
{
  cpu_state_impl_t::write_SCBP(value & 0xfffffffc);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_SCBP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
  fastiss::i32_t *masked_scbp_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_SCBP(masked_scbp_value);
}

// The RH850G4 GMCFG register has bits 31-19, 15-5 and 3-2 fixed to 0. Over-ride
// the immediate GMCFG write function to ensure this.
void rh850g4::custom_cpu_state_impl_t::write_GMCFG(unsigned int value)
{
  cpu_state_impl_t::write_GMCFG(value & 0x00070013);
  if(cpu_state_impl_t::read_GMCFG_GCU0() == 0) cpu_state_impl_t::write_GMPSW_CU0(0);
  if(cpu_state_impl_t::read_GMCFG_GCU1() == 0) cpu_state_impl_t::write_GMPSW_CU1(0);
}


void rh850g4::custom_cpu_state_impl_t::gen_write_GMCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00070013);
  fastiss::i32_t *masked_gmcfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMCFG(masked_gmcfg_value);
  cpu_state_impl_t::gen_write_GMPSW_CU0(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU0(),
								    cpu_state_impl_t::gen_read_GMPSW_CU0(),
								    m_code_gen_impl->gen_i1_constant(0)));
  cpu_state_impl_t::gen_write_GMPSW_CU1(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU1(),
								    cpu_state_impl_t::gen_read_GMPSW_CU1(),
								    m_code_gen_impl->gen_i1_constant(0)));
}

// Writing to the RH850G4 GMCFG register CU0-1 fields
void rh850g4::custom_cpu_state_impl_t::write_GMCFG_GCU0(unsigned char value)
{
  cpu_state_impl_t::write_GMCFG_GCU0(value);
  if(cpu_state_impl_t::read_GMCFG_GCU0() == 0){
    cpu_state_impl_t::write_GMPSW_CU0(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMCFG_GCU0(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_GMCFG_GCU0(value);
  cpu_state_impl_t::gen_write_GMPSW_CU0(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU0(),
								    cpu_state_impl_t::gen_read_GMPSW_CU0(),
								    m_code_gen_impl->gen_i1_constant(0)));
}

void rh850g4::custom_cpu_state_impl_t::write_GMCFG_GCU1(unsigned char value)
{
  cpu_state_impl_t::write_GMCFG_GCU1(value);
  if(cpu_state_impl_t::read_GMCFG_GCU1() == 0){
    cpu_state_impl_t::write_GMPSW_CU1(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMCFG_GCU1(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_GMCFG_GCU1(value);
  cpu_state_impl_t::gen_write_GMPSW_CU1(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU1(),
								    cpu_state_impl_t::gen_read_GMPSW_CU1(),
								    m_code_gen_impl->gen_i1_constant(0)));
}

// The RH850G4 PEID register has bits 31-3 fixed to 0. Over-ride
// the CPU State functions that alter PEID to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_PEID(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PEID(value & 0x00000007);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PEID(value & 0x0000001f);
    }else{
      cpu_state_impl_t::write_GMPEID(value & 0x0000001f);
    }
  }
}


void rh850g4::custom_cpu_state_impl_t::gen_write_PEID(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000007);
    fastiss::i32_t *masked_peid_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_PEID(masked_peid_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
    fastiss::i32_t *masked_peid_value = m_code_gen_impl->gen_and(value, mask);
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PEID(m_code_gen_impl->gen_select(is_host_mode, masked_peid_value, cpu_state_impl_t::gen_read_PEID()));
    cpu_state_impl_t::gen_write_GMPEID(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPEID(), masked_peid_value));
  }
}

// The RH850G4 BMID register has bits 31-5 fixed to 0. Over-ride
// the CPU State functions that alter BMID to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_BMID(unsigned int value)
{
  cpu_state_impl_t::write_BMID(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_BMID(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_bmid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_BMID(masked_bmid_value);
}

// The RH850G4 MEA register has the guest mode register.
// Over-ride the CPU State functions that alter MEA to select the target regsiters.
void rh850g4::custom_cpu_state_impl_t::write_MEA(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_MEA(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_MEA(value);
    }else{
      cpu_state_impl_t::write_GMMEA(value);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MEA(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_MEA(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_MEA(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_MEA()));
    cpu_state_impl_t::gen_write_GMMEA(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMMEA(), value));
  }
}

// The RH850G4 MEI register has bit 27-21, 15-12 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter MEI to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MEI(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_MEI(value & 0xf01f0f3f);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_MEI(value & 0xf01f0f3f);
    }else{
      cpu_state_impl_t::write_GMMEI(value & 0xf01f0f3f);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MEI(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xf01f0f3f);
  fastiss::i32_t *masked_mei_value = m_code_gen_impl->gen_and(value, mask);
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_MEI(masked_mei_value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_MEI(m_code_gen_impl->gen_select(is_host_mode, masked_mei_value, cpu_state_impl_t::gen_read_MEI()));
    cpu_state_impl_t::gen_write_GMMEI(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMMEI(), masked_mei_value));
  }
}

// The RH850G4 ISPR register has bit 31-16 fixed to 0. Over-ride
// the CPU State functions that alter ISPR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_ISPR(unsigned int value)
{
  cpu_state_impl_t::write_ISPR(value & 0x0000ffff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_ISPR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000ffff);
  fastiss::i32_t *masked_ispr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ISPR(masked_ispr_value);
}

// The RH850G4 ICSR register has bit 31-1 fixed to 0. Over-ride
// the CPU State functions that alter ICSR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_ICSR(unsigned int value)
{
  cpu_state_impl_t::write_ICSR(value & 0x00000001);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_ICSR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000001);
  fastiss::i32_t *masked_icsr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ICSR(masked_icsr_value);
}

// The RH850G4 INTCFG register has bit 31-1 fixed to 0. Over-ride
// the CPU State functions that alter INTCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_INTCFG(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_INTCFG(value & 0x00000001);
  }else{
    unsigned int mask_value;
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      if((value & 0x2) == 0x2){ // need to clear ISCR
	mask_value = 0x003f0002;
      }else{
	mask_value = 0x003f0003;
      }
      cpu_state_impl_t::write_INTCFG(value & mask_value); 
      if(cpu_state_impl_t::read_INTCFG_EPL() == 0){
	cpu_state_impl_t::write_EIPSW(cpu_state_impl_t::read_EIPSW() & 0xf00fffff);
	cpu_state_impl_t::write_FEPSW(cpu_state_impl_t::read_FEPSW() & 0xf00fffff);
	cpu_state_impl_t::write_PSW(cpu_state_impl_t::read_PSW() & 0xf00fffff); 
	cpu_state_impl_t::write_DBPSW(cpu_state_impl_t::read_DBPSW() & 0xf00fffff);
      } 
    }else{
      cpu_state_impl_t::write_GMINTCFG((value & 0x003f0000) | 0x2);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_INTCFG(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000001);
    fastiss::i32_t *masked_intcfg_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_INTCFG(masked_intcfg_value);
  }else{
    fastiss::i32_t *mask =  m_code_gen_impl->gen_select(m_code_gen_impl->gen_icmp_eq(m_code_gen_impl->gen_and(value, m_code_gen_impl->gen_i32_constant(0x00000002)), 
										     m_code_gen_impl->gen_i32_constant(0x00000002)),
							m_code_gen_impl->gen_i32_constant(0x003f0002),
							m_code_gen_impl->gen_i32_constant(0x003f0003));
    fastiss::i32_t *masked_intcfg_value = m_code_gen_impl->gen_and(value, mask);
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_INTCFG(m_code_gen_impl->gen_select(is_host_mode, masked_intcfg_value, cpu_state_impl_t::gen_read_INTCFG()));
    cpu_state_impl_t::gen_write_GMINTCFG(m_code_gen_impl->gen_select(is_host_mode, 
								     cpu_state_impl_t::gen_read_GMINTCFG(), 
								     m_code_gen_impl->gen_or(m_code_gen_impl->gen_and(masked_intcfg_value, 
														      m_code_gen_impl->gen_i32_constant(0xfffffffc)), 
											     m_code_gen_impl->gen_i32_constant(0x2))));
    cpu_state_impl_t::gen_write_EIPSW(m_code_gen_impl->gen_select(is_host_mode, 
								  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(),
														       m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_EIPSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_EIPSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_EIPSW()));
    cpu_state_impl_t::gen_write_FEPSW(m_code_gen_impl->gen_select(is_host_mode, 
								  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(),
														       m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_FEPSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_FEPSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_FEPSW()));
    cpu_state_impl_t::gen_write_PSW(m_code_gen_impl->gen_select(is_host_mode, 
								m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(),
														     m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_PSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_PSW()));
    cpu_state_impl_t::gen_write_DBPSW(m_code_gen_impl->gen_select(is_host_mode, 
								  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(),
														       m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_DBPSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_DBPSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_DBPSW()));
  }
}

// Writing to the RH850G4 INTCFG register ISPC, EPL, ULNR fields
void rh850g4::custom_cpu_state_impl_t::write_INTCFG_ISPC(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_INTCFG_ISPC(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      if (cpu_state_impl_t::read_INTCFG_EPL()==1){
	cpu_state_impl_t::write_INTCFG_ISPC(0);
      }else{
	cpu_state_impl_t::write_INTCFG_ISPC(value);
      }
    }else{
      cpu_state_impl_t::write_GMINTCFG_ISPC(0);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_INTCFG_ISPC(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_INTCFG_ISPC(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_INTCFG_ISPC(m_code_gen_impl->gen_select(is_host_mode, 
								        m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(), 
															     m_code_gen_impl->gen_i1_constant(1)),
												    m_code_gen_impl->gen_i1_constant(0),
												    value),
									cpu_state_impl_t::gen_read_INTCFG_ISPC()));
    cpu_state_impl_t::gen_write_GMINTCFG_ISPC(m_code_gen_impl->gen_i1_constant(0));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_INTCFG_EPL(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_INTCFG_EPL(0);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_INTCFG_EPL(value);
      if((value & 0x1) == 0x1)	cpu_state_impl_t::write_INTCFG_ISPC(0);
      if(cpu_state_impl_t::read_INTCFG_EPL() == 0){
	cpu_state_impl_t::write_EIPSW(cpu_state_impl_t::read_EIPSW() & 0xf00fffff);
	cpu_state_impl_t::write_FEPSW(cpu_state_impl_t::read_FEPSW() & 0xf00fffff);
	cpu_state_impl_t::write_PSW(cpu_state_impl_t::read_PSW() & 0xf00fffff); 
	cpu_state_impl_t::write_DBPSW(cpu_state_impl_t::read_DBPSW() & 0xf00fffff);
      } 
    }else{
      cpu_state_impl_t::write_GMINTCFG_EPL(1);
      cpu_state_impl_t::write_GMINTCFG_ISPC(0);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_INTCFG_EPL(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_INTCFG_EPL(m_code_gen_impl->gen_i1_constant(0));
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    fastiss::i1_t *ispc_value = m_code_gen_impl->gen_select(m_code_gen_impl->gen_icmp_eq(value,  m_code_gen_impl->gen_i1_constant(1)), 
							    m_code_gen_impl->gen_i1_constant(0),
							    m_code_gen_impl->gen_select(is_host_mode,
											cpu_state_impl_t::gen_read_INTCFG_ISPC(),
											cpu_state_impl_t::gen_read_GMINTCFG_ISPC()));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_INTCFG_EPL(m_code_gen_impl->gen_select(is_host_mode, value, cpu_state_impl_t::gen_read_INTCFG_EPL()));
    cpu_state_impl_t::gen_write_INTCFG_ISPC(m_code_gen_impl->gen_select(is_host_mode, ispc_value, cpu_state_impl_t::gen_read_INTCFG_ISPC()));
    cpu_state_impl_t::gen_write_GMINTCFG_EPL(m_code_gen_impl->gen_i1_constant(1));
    cpu_state_impl_t::gen_write_GMINTCFG_ISPC(m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_EIPSW(m_code_gen_impl->gen_select(is_host_mode, 
								  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(), 
														       m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_EIPSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_EIPSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_EIPSW()));
    cpu_state_impl_t::gen_write_FEPSW(m_code_gen_impl->gen_select(is_host_mode, 
								  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(),
														       m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_FEPSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_FEPSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_FEPSW()));
    cpu_state_impl_t::gen_write_PSW(m_code_gen_impl->gen_select(is_host_mode, 
								m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(),
														     m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_PSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_PSW()));
    cpu_state_impl_t::gen_write_DBPSW(m_code_gen_impl->gen_select(is_host_mode, 
								  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_INTCFG_EPL(),
														       m_code_gen_impl->gen_i1_constant(1)),
											      cpu_state_impl_t::gen_read_DBPSW(),
											      m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_DBPSW(), 
														       m_code_gen_impl->gen_i32_constant(0xf00fffff))),
								  cpu_state_impl_t::gen_read_DBPSW()));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_INTCFG_ULNR(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_INTCFG_ULNR(0);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_INTCFG_ULNR(value & 0x3f);
    }else{
      cpu_state_impl_t::write_GMINTCFG_ULNR(value & 0x3f);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_INTCFG_ULNR(fastiss::i8_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_INTCFG_ULNR(m_code_gen_impl->gen_i8_constant(0));
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    fastiss::i8_t *mask = m_code_gen_impl->gen_i8_constant(0x3f);
    fastiss::i8_t *masked_value = m_code_gen_impl->gen_and(value, mask);
    
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_INTCFG_ULNR(m_code_gen_impl->gen_select(is_host_mode, masked_value, cpu_state_impl_t::gen_read_INTCFG_ULNR()));
    cpu_state_impl_t::gen_write_GMINTCFG_ULNR(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMINTCFG_ULNR(), masked_value));
  }
}

// The RH850G4 PLMR register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter PLMRtto mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_PLMR(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PLMR(value & 0x0000001f);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PLMR(value & 0x0000003f);
    }else{
      cpu_state_impl_t::write_GMPLMR(value & 0x0000003f);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PLMR(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
    fastiss::i32_t *masked_plmr_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_PLMR(masked_plmr_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000003f);
    fastiss::i32_t *masked_plmr_value = m_code_gen_impl->gen_and(value, mask);
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PLMR(m_code_gen_impl->gen_select(is_host_mode, masked_plmr_value, cpu_state_impl_t::gen_read_PLMR()));
    cpu_state_impl_t::gen_write_GMPLMR(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMPLMR(), masked_plmr_value));
  }
}

// Writing to the RH850G4 PLMR register PLM fields
void rh850g4::custom_cpu_state_impl_t::write_PLMR_PLM(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PLMR_PLM(value);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_PLMR_PLM(value & 0x3f);
    }else{
      cpu_state_impl_t::write_GMPLMR_PLM(value & 0x3f);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PLMR_PLM(fastiss::i8_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
   cpu_state_impl_t::gen_write_PLMR_PLM(value);
  }else{
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_PLMR_PLM(m_code_gen_impl->gen_select(is_host_mode, 
								     m_code_gen_impl->gen_and(value, m_code_gen_impl->gen_i8_constant(0x3f)),
								     cpu_state_impl_t::gen_read_PLMR_PLM()));
    cpu_state_impl_t::gen_write_GMPLMR_PLM(m_code_gen_impl->gen_select(is_host_mode, 
								       cpu_state_impl_t::gen_read_GMPLMR_PLM(),
								       m_code_gen_impl->gen_and(value, m_code_gen_impl->gen_i8_constant(0x3f))));
  }
}


// The RH850G4 RBCR0 register has bit 31-17 fixed to 0. Over-ride
// the CPU State functions that alter RBCR0 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_RBCR0(unsigned int value)
{
  cpu_state_impl_t::write_RBCR0(value & 0x0001ffff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_RBCR0(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0001ffff);
  fastiss::i32_t *masked_rbcr0_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_RBCR0(masked_rbcr0_value);
}

// The RH850G4 RBCR1 register has bit 31-16 fixed to 0. Over-ride
// the CPU State functions that alter RBCR1 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_RBCR1(unsigned int value)
{
  cpu_state_impl_t::write_RBCR1(value & 0x0000ffff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_RBCR1(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000ffff);
  fastiss::i32_t *masked_rbcr1_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_RBCR1(masked_rbcr1_value);
}

// The RH850G4 RBNR register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter RBNR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_RBNR(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    value = value & 0x1f;
    if(value > 16) value=16;
    cpu_state_impl_t::write_RBNR(value);
  }else{
    cpu_state_impl_t::write_RBNR(value & 0x3f);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_RBNR(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, m_code_gen_impl->gen_i32_constant(0x1f));
    fastiss::i1_t *is_ge = m_code_gen_impl->gen_icmp_sge(masked_value, m_code_gen_impl->gen_i32_constant(0x00000010));
    fastiss::i32_t *new_value = m_code_gen_impl->gen_select(is_ge, m_code_gen_impl->gen_i32_constant(0x10), value);
    cpu_state_impl_t::gen_write_RBNR(new_value);
  }else{
    cpu_state_impl_t::gen_write_RBNR(m_code_gen_impl->gen_and(value, m_code_gen_impl->gen_i32_constant(0x3f)));
  }
}

// The RH850G4 RBIP register has bit 3-0 fixed to 0. Over-ride
// the CPU State functions that alter RBIP to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_RBIP(unsigned int value)
{
  cpu_state_impl_t::write_RBIP(value & 0xfffffff0);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_RBIP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffff0);
  fastiss::i32_t *masked_rbip_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_RBIP(masked_rbip_value);
}


// The RH850G4 ICTAGL register has bit 10-6, 3 and 1 fixed to 0. Over-ride
// the CPU State functions that alter ICTAGL to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_ICTAGL(unsigned int value)
{
  cpu_state_impl_t::write_ICTAGL(value & 0xfffff835);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_ICTAGL(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffff835);
  fastiss::i32_t *masked_ictagl_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ICTAGL(masked_ictagl_value);
}

// The RH850G4 ICTAGH register has bit 27-24, 15, 7-0 fixed to 0. Over-ride
// the CPU State functions that alter ICTAGH to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_ICTAGH(unsigned int value)
{
  cpu_state_impl_t::write_ICTAGH(value & 0xf0ff7f00);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_ICTAGH(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xf0ff7f00);
  fastiss::i32_t *masked_ictagh_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ICTAGH(masked_ictagh_value);
}

// The RH850G4 ICCTRL register has bit 31-17, 15-9 and 8-3 fixed to 0. Over-ride
// the CPU State functions that alter ICCTRL to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_ICCTRL(unsigned int value)
{
  cpu_state_impl_t::write_ICCTRL((value & 0x00010007) | 0x00010000);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_ICCTRL(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00010007);
  fastiss::i32_t *fixed_bit = m_code_gen_impl->gen_i32_constant(0x00010000);
  fastiss::i32_t *masked_icctrl_value = m_code_gen_impl->gen_and(value, mask);
  fastiss::i32_t *fixed_icctrl_value = m_code_gen_impl->gen_or(masked_icctrl_value, fixed_bit);
  cpu_state_impl_t::gen_write_ICCTRL(fixed_icctrl_value);
}

// The RH850G4 ICCFG register has bit 31-15 fixed to 0. Over-ride
// the CPU State functions that alter ICCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_ICCFG(unsigned int value)
{
  cpu_state_impl_t::write_ICCFG(value & 0x00007fff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_ICCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00007fff);
  fastiss::i32_t *masked_iccfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ICCFG(masked_iccfg_value);
}

// The RH850G4 ICERR register has bit 23, 15 and 1 fixed to 0. Over-ride
// the CPU State functions that alter ICERR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_ICERR(unsigned int value)
{
  cpu_state_impl_t::write_ICERR(value & 0xff7f7ffd);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_ICERR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xff7f7ffd);
  fastiss::i32_t *masked_icerr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ICERR(masked_icerr_value);
}

// The RH850G4 MPM register has bit 31-2 fixed to 0. Over-ride
// the CPU State functions that alter MPM to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPM(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_MPM(value & 0x00000003);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      cpu_state_impl_t::write_MPM(value & 0x00000007);
    }else{
      cpu_state_impl_t::write_GMMPM(value & 0x00000007);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPM(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
   fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000003);
   fastiss::i32_t *masked_mpm_value = m_code_gen_impl->gen_and(value, mask);
   cpu_state_impl_t::gen_write_MPM(masked_mpm_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000007);
    fastiss::i32_t *masked_mpm_value = m_code_gen_impl->gen_and(value, mask);
    // check the CPU mode
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    // If not in the selected mode, write the current value back to the register
    cpu_state_impl_t::gen_write_MPM(m_code_gen_impl->gen_select(is_host_mode, masked_mpm_value, cpu_state_impl_t::gen_read_MPM()));
    cpu_state_impl_t::gen_write_GMMPM(m_code_gen_impl->gen_select(is_host_mode, cpu_state_impl_t::gen_read_GMMPM(), masked_mpm_value));
  }
}

// The RH850G4 MPCFG register has bit 31-20 and 15-5 fixed to 0. Over-ride
// the CPU State functions that alter MPCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPCFG(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_MPCFG(value & 0x000f001f);
  }else{
    cpu_state_impl_t::write_MPCFG(value & 0x003f3f1f);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPCFG(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000f001f);
    fastiss::i32_t *masked_mpcfg_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPCFG(masked_mpcfg_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x003f3f1f);
    fastiss::i32_t *masked_mpcfg_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPCFG(masked_mpcfg_value);
  }
}

// The RH850G4 MCC register always holds 0 and it's value should never be
// changed. Over-ride the CPU State functions that alter MCC to ensure this.
void rh850g4::custom_cpu_state_impl_t::write_MCC(unsigned int value)
{
  cpu_state_impl_t::write_MCC(value);
  cpu_state_impl_t::write_MCC(0);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MCC(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_MCC(value);
  cpu_state_impl_t::gen_write_MCC(m_code_gen_impl->gen_i32_constant(0));
}

// A contant zero value can be returned for MCC as it cannot change value.
unsigned int rh850g4::custom_cpu_state_impl_t::read_MCC()
{
  return 0;
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_MCC()
{
  return m_code_gen_impl->gen_i32_constant(0);
}

// The RH850G4 MCR register has bit 31-9 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter MCR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MCR(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_MCR(value & 0x0000013f);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_MCR(value & 0x0000013f);
    }else{
      cpu_state_impl_t::write_MCR(value & 0x3f3f013f);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MCR(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000013f);
    fastiss::i32_t *masked_mcr_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MCR(masked_mcr_value);
  }else{
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), 
										      m_code_gen_impl->gen_i1_constant(1)),
							     m_code_gen_impl->gen_i1_constant(1), 
							     m_code_gen_impl->gen_i1_constant(0));
    fastiss::i32_t *mask = m_code_gen_impl->gen_select(is_hv_valid,
						       m_code_gen_impl->gen_i32_constant(0x3f3f013f),
						       m_code_gen_impl->gen_i32_constant(0x0000013f));
    fastiss::i32_t *masked_mcr_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MCR(masked_mcr_value);
  }
}

// The RH850G4 MCI register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MCI to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MCI(unsigned int value)
{
  cpu_state_impl_t::write_MCI(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MCI(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mci_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MCI(masked_mci_value);
}

// The RH850G4 MPIDX register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPIDX to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPIDX(unsigned int value)
{
  cpu_state_impl_t::write_MPIDX(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPIDX(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpidx_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPIDX(masked_mpidx_value);
}

// The RH850G4 MPLA register has bit 1-0 fixed to 0. Over-ride
// the CPU State functions that alter MPLA to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPLA(unsigned int value)
{
  cpu_state_impl_t::write_MPLA(value & 0xfffffffc);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPLA(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
  fastiss::i32_t *masked_mpla_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPLA(masked_mpla_value);
}

// The RH850G4 MPUA register has bit 1-0 fixed to 0. Over-ride
// the CPU State functions that alter MPUA to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPUA(unsigned int value)
{
  cpu_state_impl_t::write_MPUA(value & 0xfffffffc);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPUA(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
  fastiss::i32_t *masked_mpua_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPUA(masked_mpua_value);
}

// The RH850G4 MPID0 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID0 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID0(unsigned int value)
{
  cpu_state_impl_t::write_MPID0(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID0(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID0(masked_mpid_value);
}

// The RH850G4 MPID1 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID1 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID1(unsigned int value)
{
  cpu_state_impl_t::write_MPID1(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID1(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID1(masked_mpid_value);
}

// The RH850G4 MPID2 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID2 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID2(unsigned int value)
{
  cpu_state_impl_t::write_MPID2(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID2(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID2(masked_mpid_value);
}

// The RH850G4 MPID3 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID3 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID3(unsigned int value)
{
  cpu_state_impl_t::write_MPID3(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID3(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID3(masked_mpid_value);
}

// The RH850G4 MPID4 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID4 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID4(unsigned int value)
{
  cpu_state_impl_t::write_MPID4(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID4(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID4(masked_mpid_value);
}

// The RH850G4 MPID5 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID5 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID5(unsigned int value)
{
  cpu_state_impl_t::write_MPID5(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID5(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID5(masked_mpid_value);
}

// The RH850G4 MPID6 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID6 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID6(unsigned int value)
{
  cpu_state_impl_t::write_MPID6(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID6(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID6(masked_mpid_value);
}

// The RH850G4 MPID7 register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter MPID7 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_MPID7(unsigned int value)
{
  cpu_state_impl_t::write_MPID7(value & 0x0000001f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_MPID7(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_mpid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPID7(masked_mpid_value);
}


// The RH850G4 FXSR register has bits 31-24, 20 and 16 fixed to 0. Over-ride
// the CPU State functions that alter FXSR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FXSR(unsigned int value)
{
  cpu_state_impl_t::write_FXSR((value & 0x00eeffff) | 0x00200000);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXSR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00eeffff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FXSR(m_code_gen_impl->gen_or(masked_value, m_code_gen_impl->gen_i32_constant(0x00200000)));
}


// The RH850G4 FXST register has bits 31-14 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter FXST to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FXST(unsigned int value)
{
  cpu_state_impl_t::write_FXST(value & 0x00003f3f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXST(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00003f3f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FXST(masked_value);
}


// The RH850G4 FXINFO register has bits 31-2 fixed to 0. Over-ride
// the CPU State functions that alter FXINFO to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FXINFO(unsigned int value)
{
  cpu_state_impl_t::write_FXINFO(value & 0x00000003);
}
 
void rh850g4::custom_cpu_state_impl_t::gen_write_FXINFO(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000003);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FXINFO(masked_value);
}


// The RH850G4 FXCFG register has bits 31-10 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter FXCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FXCFG(unsigned int value)
{
  cpu_state_impl_t::write_FXCFG(value & 0x0000031f);
}
 
void rh850g4::custom_cpu_state_impl_t::gen_write_FXCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000031f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FXCFG(masked_value);
}


// The RH850G4 FXXC register has bits 31-30, 23-22, 15-14 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter FXXC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FXXC(unsigned int value)
{
  cpu_state_impl_t::write_FXXC(value & 0x3f3f3f3f);
}
 
void rh850g4::custom_cpu_state_impl_t::gen_write_FXXC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x3f3f3f3f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FXXC(masked_value);
}


// The RH850G4 FXXP register has bits 31-29, 23-21, 15-13 and 7-5 fixed to 0. Over-ride
// the CPU State functions that alter FXXP to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FXXP(unsigned int value)
{
  cpu_state_impl_t::write_FXXP(value & 0x1f1f1f1f);
}
 
void rh850g4::custom_cpu_state_impl_t::gen_write_FXXP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x1f1f1f1f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FXXP(masked_value);
}


// Writing to the RH850G4 FXSR register IF, RM, XC, XP or XE fields
// also writes the values to the corresponding fields in the FXST
// and FXCFG registers.
void rh850g4::custom_cpu_state_impl_t::write_FXSR_IF(unsigned char value)
{
  cpu_state_impl_t::write_FXSR_IF(value);
  cpu_state_impl_t::write_FXST_IF(read_FXSR_IF());
}

void rh850g4::custom_cpu_state_impl_t::write_FXSR_XC(unsigned char value)
{
  cpu_state_impl_t::write_FXSR_XC(value);
  cpu_state_impl_t::write_FXST_XC(read_FXSR_XC());
}

void rh850g4::custom_cpu_state_impl_t::write_FXSR_XP(unsigned char value)
{
  cpu_state_impl_t::write_FXSR_XP(value);
  cpu_state_impl_t::write_FXST_XP(read_FXSR_XP());
}

void rh850g4::custom_cpu_state_impl_t::write_FXSR_RM(unsigned char value)
{
  cpu_state_impl_t::write_FXSR_RM(value);
  cpu_state_impl_t::write_FXCFG_RM(read_FXSR_RM());
}

void rh850g4::custom_cpu_state_impl_t::write_FXSR_XE(unsigned char value)
{
  cpu_state_impl_t::write_FXSR_XE(value);
  cpu_state_impl_t::write_FXCFG_XE(read_FXSR_XE());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXSR_IF(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_FXSR_IF(value);
  cpu_state_impl_t::gen_write_FXST_IF(gen_read_FXSR_IF());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXSR_XC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXSR_XC(value);
  cpu_state_impl_t::gen_write_FXST_XC(gen_read_FXSR_XC());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXSR_XP(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXSR_XP(value);
  cpu_state_impl_t::gen_write_FXST_XP(gen_read_FXSR_XP());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXSR_RM(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXSR_RM(value);
  cpu_state_impl_t::gen_write_FXCFG_RM(gen_read_FXSR_RM());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXSR_XE(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXSR_XE(value);
  cpu_state_impl_t::gen_write_FXCFG_XE(gen_read_FPSR_XE());
}

// Writing to the RH850G4 FXST register IF, XC or XP fields
// also writes the values to the corresponding fields in the FXSR registers.
void rh850g4::custom_cpu_state_impl_t::write_FXST_IF(unsigned char value)
{
  cpu_state_impl_t::write_FXST_IF(value);
  cpu_state_impl_t::write_FXSR_IF(read_FXST_IF());
}

void rh850g4::custom_cpu_state_impl_t::write_FXST_XC(unsigned char value)
{
  cpu_state_impl_t::write_FXST_XC(value);
  cpu_state_impl_t::write_FXSR_XC(read_FXST_XC());
}

void rh850g4::custom_cpu_state_impl_t::write_FXST_XP(unsigned char value)
{
  cpu_state_impl_t::write_FXST_XP(value);
  cpu_state_impl_t::write_FXSR_XP(read_FXST_XP());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXST_IF(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_FXST_IF(value);
  cpu_state_impl_t::gen_write_FXSR_IF(gen_read_FXST_IF());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXST_XC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXST_XC(value);
  cpu_state_impl_t::gen_write_FXSR_XC(gen_read_FXST_XC());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXST_XP(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXST_XP(value);
  cpu_state_impl_t::gen_write_FXSR_XP(gen_read_FXST_XP());
}

// Writing to the RH850G4 FXCFG register RM or XR fields
// also writes the values to the corresponding fields in the FXSR registers.
void rh850g4::custom_cpu_state_impl_t::write_FXCFG_RM(unsigned char value)
{
  cpu_state_impl_t::write_FXCFG_RM(value);
  cpu_state_impl_t::write_FXSR_RM(read_FXCFG_RM());
}

void rh850g4::custom_cpu_state_impl_t::write_FXCFG_XE(unsigned char value)
{
  cpu_state_impl_t::write_FXCFG_XE(value);
  cpu_state_impl_t::write_FXSR_XE(read_FXCFG_XE());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXCFG_RM(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXCFG_RM(value);
  cpu_state_impl_t::gen_write_FXSR_RM(gen_read_FXCFG_RM());
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FXCFG_XE(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FXCFG_XE(value);
  cpu_state_impl_t::gen_write_FXSR_XE(gen_read_FPCFG_XE());
}

// The RH850G4 DBGEN register has bits 31-9 fixed to 0. Over-ride
// the CPU State functions that alter DBGEN to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DBGEN(unsigned int value)
{
  cpu_state_impl_t::write_DBGEN(value & 0x000001ff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DBGEN(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000001ff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DBGEN(masked_value);
}

// The RH850G4 DBPSWH register has bits 30-11 and 7-0 fixed to 0. Over-ride
// the CPU State functions that alter DBPSWH to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DBPSWH(unsigned int value)
{
  cpu_state_impl_t::write_DBPSWH(value & 0x80000700);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DBPSWH(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x80000700);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DBPSWH(masked_value);
}

// The RH850G4 DBPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter DBPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DBPC(unsigned int value)
{
  cpu_state_impl_t::write_DBPC(value & 0xffffffff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DBPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_dbpc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DBPC(masked_dbpc_value);
}

// The RH850G4 DBPSW register has bits 31, 29-19 and 14-8 fixed to 0. Over-ride
// the CPU State functions that alter DBPSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DBPSW(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_DBPSW(value & 0x400780ff);
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      if((cpu_state_impl_t::read_INTCFG_EPL() & 0x1) ==  0x1){
	cpu_state_impl_t::write_DBPSW(value & 0x43f780ff);
      }else{
	cpu_state_impl_t::write_DBPSW(value & 0x400780ff);
      }
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DBPSW(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x400780ff);
    fastiss::i32_t *masked_dbpsw_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_DBPSW(masked_dbpsw_value);
  }else{
    fastiss::i1_t *is_host_mode = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), 
										       m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(0), 
							      m_code_gen_impl->gen_i1_constant(1));
    fastiss::i1_t *elp_val = m_code_gen_impl->gen_select(is_host_mode, 
							 cpu_state_impl_t::gen_read_INTCFG_EPL(), 
							 cpu_state_impl_t::gen_read_GMINTCFG_EPL());
    fastiss::i32_t *mask =  m_code_gen_impl->gen_select(elp_val, 
							m_code_gen_impl->gen_i32_constant(0x43f380ff), 
							m_code_gen_impl->gen_i32_constant(0x400380ff));
    fastiss::i32_t *masked_dbpsw_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_DBPSW(m_code_gen_impl->gen_select(is_host_mode, masked_dbpsw_value, cpu_state_impl_t::gen_read_DBPSW()));
  }
}

// The RH850G4 DIR0 register has bits 31-10 and 7-3 fixed to 0. Over-ride
// the CPU State functions that alter DIR0 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DIR0(unsigned int value)
{
  cpu_state_impl_t::write_DIR0(value & 0x00000305);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DIR0(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000305);
  fastiss::i32_t *masked_dir0_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DIR0(masked_dir0_value);
}

// The RH850G4 DIR1 register has bits 31-20 and 3 fixed to 0. Over-ride
// the CPU State functions that alter DIR1 to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DIR1(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_DIR1(value & 0x000ffff7);
  }else{
    cpu_state_impl_t::write_DIR1(value & 0x000ffff1);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DIR1(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000ffff7);
    fastiss::i32_t *masked_dir1_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_DIR1(masked_dir1_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000ffff1);
    fastiss::i32_t *masked_dir1_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_DIR1(masked_dir1_value);
  }
}

// The RH850G4 BPC register has bits 31-13, 9 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter BPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_BPC(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_BPC(value & 0x00001d3f);
  }else{
    cpu_state_impl_t::write_BPC(value & 0x01ff1d0f);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_BPC(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00001d3f);
    fastiss::i32_t *masked_bpc_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_BPC(masked_bpc_value);
  }else{
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x01ff1d0f);
    fastiss::i32_t *masked_bpc_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_BPC(masked_bpc_value);
  }
}

// The RH850G4 DBCFG register has bits 31-4 fixed to 0. Over-ride
// the CPU State functions that alter DBCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DBCFG(unsigned int value)
{
  cpu_state_impl_t::write_DBCFG(value & 0x0000000f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DBCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000000f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DBCFG(masked_value);
}

// The RH850G4 LSCFG register has bits 31-1 fixed to 0. Over-ride
// the CPU State functions that alter LSCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_LSCFG(unsigned int value)
{
  cpu_state_impl_t::write_LSCFG(value & 0x00000001);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_LSCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000001);
  fastiss::i32_t *masked_lscfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_LSCFG(masked_lscfg_value);
}

// The RH850G4 L1RCFG register has bits 31-30 and 7-4 fixed to 0. Over-ride
// the CPU State functions that alter L1RCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_L1RCFG(unsigned int value)
{
  cpu_state_impl_t::write_L1RCFG(value & 0x3fffff0f);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_L1RCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x3fffff0f);
  fastiss::i32_t *masked_l1rcfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_L1RCFG(masked_l1rcfg_value);
}

// The RH850G4 DECFG register has bits 31-3 fixed to 0. Over-ride
// the CPU State functions that alter DECFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DECFG(unsigned int value)
{
  cpu_state_impl_t::write_DECFG(value & 0x00000007);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DECFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000007);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DECFG(masked_value);
}

// The RH850G4 DECTRL register has bits 31-1 fixed to 0. Over-ride
// the CPU State functions that alter DECTRL to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DECTRL(unsigned int value)
{
  cpu_state_impl_t::write_DECTRL(value & 0x1);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DECTRL(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000001);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DECTRL(masked_value);
}

// The RH850G4 DEVDS register has bits 31-17, 15-8 fixed to 0. Over-ride
// the CPU State functions that alter DEVDS to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_DEVDS(unsigned int value)
{
  cpu_state_impl_t::write_DEVDS(value & 0x000100ff);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_DEVDS(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000100ff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DEVDS(masked_value);
}

// The RH850G4 GMEIPC register has bits 0 fixed to 0. Over-ride
// the CPU State functions that alter GMEIPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMEIPC(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMEIPC(value & 0xffffffff);
  }else{
    cpu_state_impl_t::write_GMEIPC(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMEIPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMEIPC(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 masked_value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMEIPSW register has bits 31, 29-28, 19, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter GMEIPSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMEIPSW(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMEIPSW(value & 0x43f380ff);
  }else{
    cpu_state_impl_t::write_GMEIPSW(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMEIPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x43f380ff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMEIPSW(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 masked_value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMFEPC register has bits 0 fixed to 0. Over-ride
// the CPU State functions that alter GMFEPC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMFEPC(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMFEPC(value & 0xffffffff);
  }else{
    cpu_state_impl_t::write_GMFEPC(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMFEPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMFEPC(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 masked_value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMFEPSW register has bits 31, 29-28, 19, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter GMFEPSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMFEPSW(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMFEPSW(value & 0x43f380ff);
  }else{
    cpu_state_impl_t::write_GMFEPSW(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMFEPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x43f380ff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMFEPSW(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								  masked_value,
								  m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMPSW register has bits 31, 29-28, 19, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter GMPSW to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMPSW(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    value = value | 0x00008000;
    unsigned int mask_val = 0x43f380ff;
    if(cpu_state_impl_t::read_GMCFG_GCU1() == 0) mask_val = mask_val & 0xfffdffff;
    if(cpu_state_impl_t::read_GMCFG_GCU0() == 0) mask_val = mask_val & 0xfffeffff;
    cpu_state_impl_t::write_GMPSW(value & mask_val);
  }else{
    cpu_state_impl_t::write_GMPSW(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x43f380ff);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  mask =  m_code_gen_impl->gen_and(mask, m_code_gen_impl->gen_select(m_code_gen_impl->gen_icmp_eq(cpu_state_impl_t::gen_read_GMCFG_GCU1(), 
												  m_code_gen_impl->gen_i1_constant(0)),
								     m_code_gen_impl->gen_i32_constant(0xfffdffff), 
								     m_code_gen_impl->gen_i32_constant(0xffffffff)));
  masked_value = m_code_gen_impl->gen_and(value, mask);
  mask =  m_code_gen_impl->gen_and(mask, m_code_gen_impl->gen_select(m_code_gen_impl->gen_icmp_eq(cpu_state_impl_t::gen_read_GMCFG_GCU0(), 
												  m_code_gen_impl->gen_i1_constant(0)),
								     m_code_gen_impl->gen_i32_constant(0xfffeffff), 
								     m_code_gen_impl->gen_i32_constant(0xffffffff)));					
  masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMPSW(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								m_code_gen_impl->gen_or(masked_value, m_code_gen_impl->gen_i32_constant(0x00008000)),
								m_code_gen_impl->gen_i32_constant(0)));
}


// Writing to the RH850G4 GMPSW register EBV, CU0, CU1, CU2, EIMASK fields
void rh850g4::custom_cpu_state_impl_t::write_GMPSW_EBV(unsigned char value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMPSW_EBV(1);
  }else{
    cpu_state_impl_t::write_GMPSW_EBV(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPSW_EBV(fastiss::i1_t *value)
{
  // GMPSW.EBV is fixed 1.
  cpu_state_impl_t::gen_write_GMPSW_EBV(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								    m_code_gen_impl->gen_i1_constant(1),
								    m_code_gen_impl->gen_i1_constant(0)));
}

void rh850g4::custom_cpu_state_impl_t::write_GMPSW_CU0(unsigned char value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    if(cpu_state_impl_t::read_GMCFG_GCU0() == 1){
      cpu_state_impl_t::write_GMPSW_CU0(value);
    }else{
      cpu_state_impl_t::write_GMPSW_CU0(0);
    }
  }else{
    cpu_state_impl_t::write_GMPSW_CU0(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPSW_CU0(fastiss::i1_t *value)
{
  fastiss::i1_t *update_value = m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
							    value,
							    m_code_gen_impl->gen_i1_constant(0));
  cpu_state_impl_t::gen_write_GMPSW_CU0(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU0(),
								    update_value,
								    m_code_gen_impl->gen_i1_constant(0)));
}

void rh850g4::custom_cpu_state_impl_t::write_GMPSW_CU1(unsigned char value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    if(cpu_state_impl_t::read_GMCFG_GCU1() == 1){
      cpu_state_impl_t::write_GMPSW_CU1(value);
    }else{
      cpu_state_impl_t::write_GMPSW_CU1(0);
    }
  }else{
    cpu_state_impl_t::write_GMPSW_CU1(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPSW_CU1(fastiss::i1_t *value)
{
  fastiss::i1_t *update_value = m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
							    value,
							    m_code_gen_impl->gen_i1_constant(0));
  cpu_state_impl_t::gen_write_GMPSW_CU1(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_GMCFG_GCU1(),
								    update_value,
								    m_code_gen_impl->gen_i1_constant(0)));
}

void rh850g4::custom_cpu_state_impl_t::write_GMPSW_CU2(unsigned char value)
{
    cpu_state_impl_t::write_GMPSW_CU2(0);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPSW_CU2(fastiss::i1_t *value)
{
  // PSW.CU2 and GMPSW.CU2 are fixed 0.
  cpu_state_impl_t::gen_write_GMPSW_CU2(m_code_gen_impl->gen_i1_constant(0));
}

void rh850g4::custom_cpu_state_impl_t::write_GMPSW_EIMASK(unsigned char value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMPSW_EIMASK(value & 0x3f);
  }else{
    cpu_state_impl_t::write_GMPSW_EIMASK(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPSW_EIMASK(fastiss::i8_t *value)
{
  fastiss::i8_t *mask = m_code_gen_impl->gen_i8_constant(0x3f);
  fastiss::i8_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMPSW_EIMASK(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								       masked_value,
								       m_code_gen_impl->gen_i8_constant(0)));
}

// The RH850G4 GMMEA register has bits 31-0 fixed to 0, when HVCFG.HVE is 0.
// Over-ride the CPU State functions that alter GMMEI to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMMEA(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMMEA(value);
  }else{
    cpu_state_impl_t::write_GMMEA(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMMEA(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMMEA(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								value,
								m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMMEI register has bits 27-21, 15-12, and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter GMMEI to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMMEI(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMMEI(value & 0xf01f0f3f);
  }else{
    cpu_state_impl_t::write_GMMEI(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMMEI(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xf01f0f3f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMMEI(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								masked_value,
								m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMEIIC register has bits 31-0 fixed to 0, when HVCFG.HVE is 0.
// Over-ride the CPU State functions that alter GMEIIC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMEIIC(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMEIIC(value);
  }else{
    cpu_state_impl_t::write_GMEIIC(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMEIIC(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMEIIC(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMFEIC register has bits 31-0 fixed to 0, when HVCFG.HVE is 0.
// Over-ride the CPU State functions that alter GMFEIC to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMFEIC(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMFEIC(value);
  }else{
    cpu_state_impl_t::write_GMFEIC(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMFEIC(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMFEIC(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMSPID register has bits 31-5 fixed to 0. Over-ride
// the CPU State functions that alter GMSPID to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMSPID(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMSPID(value & 0x0000001f);
  }else{
    cpu_state_impl_t::write_GMSPID(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMSPID(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMSPID(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 masked_value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMSPIDLIST register has bits 31-0 fixed to 0, when HVCFG.HVE is 0.
// Over-ride the CPU State functions that alter GMSPIDLIST to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMSPIDLIST(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMSPIDLIST(value);
  }else{
    cpu_state_impl_t::write_GMSPIDLIST(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMSPIDLIST(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMSPIDLIST(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								     value,
								     m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMEBASE register has bits 31-0 fixed to 0, when HVCFG.HVE is 0.
// Over-ride the CPU State functions that alter GMEBASE to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMEBASE(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMEBASE(value);
  }else{
    cpu_state_impl_t::write_GMEBASE(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMEBASE(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMEBASE(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								  value,
								  m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMINTBP register has bit 8-0 fixed to 0. Over-ride
// the CPU State functions that alter GMINTBP to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMINTBP(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMINTBP(value & 0xfffffe00);
  }else{
    cpu_state_impl_t::write_GMINTBP(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMINTBP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffe00);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMINTBP(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								  masked_value,
								  m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMINTCFG register has bit 31-1 fixed to 0. Over-ride
// the CPU State functions that alter GMINTCFG to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMINTCFG(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMINTCFG((value & 0x003f0000) | 0x2);
  }else{
    cpu_state_impl_t::write_GMINTCFG(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMINTCFG(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMINTCFG(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								   m_code_gen_impl->gen_or(m_code_gen_impl->gen_and(value, 
														    m_code_gen_impl->gen_i32_constant(0x003f0000)), 
											   m_code_gen_impl->gen_i32_constant(0x2)),
								   m_code_gen_impl->gen_i32_constant(0)));
}

// Writing to the RH850G4 GMINTCFG register ISPC, EPL, ULNR fields
void rh850g4::custom_cpu_state_impl_t::write_GMINTCFG_ISPC(unsigned char value)
{
  cpu_state_impl_t::write_GMINTCFG_ISPC(0);
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMINTCFG_ISPC(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_GMINTCFG_ISPC(m_code_gen_impl->gen_i1_constant(0));
}

void rh850g4::custom_cpu_state_impl_t::write_GMINTCFG_EPL(unsigned char value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMINTCFG_EPL(1);
    cpu_state_impl_t::write_GMINTCFG_ISPC(0);
  }else{
    cpu_state_impl_t::write_GMINTCFG_EPL(0);
    cpu_state_impl_t::write_GMINTCFG_ISPC(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMINTCFG_EPL(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_GMINTCFG_EPL(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								       m_code_gen_impl->gen_i1_constant(1),
								       m_code_gen_impl->gen_i1_constant(0)));
  cpu_state_impl_t::gen_write_GMINTCFG_ISPC(m_code_gen_impl->gen_i1_constant(0));
}

void rh850g4::custom_cpu_state_impl_t::write_GMINTCFG_ULNR(unsigned char value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMINTCFG_ULNR(value & 0x3f);
  }else{
    cpu_state_impl_t::write_GMINTCFG_ULNR(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMINTCFG_ULNR(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_GMINTCFG_ULNR(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
									m_code_gen_impl->gen_and(value, m_code_gen_impl->gen_i8_constant(0x3f)),
									m_code_gen_impl->gen_i8_constant(0)));	
}

// The RH850G4 GMPLMR register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter GMPLMR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMPLMR(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMPLMR(value & 0x0000003f);
  }else{
    cpu_state_impl_t::write_GMPLMR(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPLMR(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMPLMR(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 m_code_gen_impl->gen_and(value, 
											  m_code_gen_impl->gen_i32_constant(0x0000003f)),
								 m_code_gen_impl->gen_i32_constant(0)));	
}

// Writing to the RH850G4 GMPLMR register PLM fields
void rh850g4::custom_cpu_state_impl_t::write_GMPLMR_PLM(unsigned char value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMPLMR_PLM(value & 0x3f);
  }else{
    cpu_state_impl_t::write_GMPLMR_PLM(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPLMR_PLM(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_GMPLMR_PLM(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								     m_code_gen_impl->gen_and(value, 
											      m_code_gen_impl->gen_i8_constant(0x3f)),
								     m_code_gen_impl->gen_i8_constant(0)));
}

// The RH850G4 GMSVLOCK register has bits 31-1 fixed to 0. Over-ride
// the CPU State functions that alter GMSPID to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMSVLOCK(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMSVLOCK(value & 0x00000001);
  }else{
    cpu_state_impl_t::write_GMSVLOCK(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMSVLOCK(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000001);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMSVLOCK(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								   masked_value,
								   m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMMPM register has bits 31-3 fixed to 0. Over-ride
// the CPU State functions that alter GMMPM to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMMPM(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMMPM(value & 0x00000007);
  }else{
    cpu_state_impl_t::write_GMMPM(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMMPM(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000007);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMMPM(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								masked_value,
								m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMEIWR register has bits 31-0 fixed to 0, when HVCFG.HVE is 0.
// Over-ride the CPU State functions that alter GMEIWR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMEIWR(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMEIWR(value);
  }else{
    cpu_state_impl_t::write_GMEIWR(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMEIWR(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMEIWR(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMFEWR register has bits 31-0 fixed to 0, when HVCFG.HVE is 0.
// Over-ride the CPU State functions that alter GMFEWR to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMFEWR(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMFEWR(value);
  }else{
    cpu_state_impl_t::write_GMFEWR(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMFEWR(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_GMFEWR(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 GMPEID register has bits 31-5 fixed to 0. Over-ride
// the CPU State functions that alter GMPEID to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_GMPEID(unsigned int value)
{
  if(cpu_state_impl_t::read_HVCFG_HVE() == 0x1){
    cpu_state_impl_t::write_GMPEID(value & 0x0000001f);
  }else{
    cpu_state_impl_t::write_GMPEID(0);
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_GMPEID(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_GMPEID(m_code_gen_impl->gen_select(cpu_state_impl_t::gen_read_HVCFG_HVE(),
								 masked_value,
								 m_code_gen_impl->gen_i32_constant(0)));
}

// The RH850G4 PSWH register has bits 30-11 and 7-0 fixed to 0. Over-ride
// the CPU State functions that alter PSWH to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_PSWH(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSWH(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_PSWH(0);
    }else{
      cpu_state_impl_t::write_PSWH(value & 0x80000700);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSWH(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSWH(m_code_gen_impl->gen_i32_constant(0));
  }else{
    fastiss::i32_t *mask =  m_code_gen_impl->gen_i32_constant(0x80000700);
    fastiss::i32_t *masked_pswh_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_PSWH(m_code_gen_impl->gen_select(is_hv_valid, masked_pswh_value, m_code_gen_impl->gen_i32_constant(0)));
  }
}

// Writing to the RH850G4 PSWH register GM, GPID fields
void rh850g4::custom_cpu_state_impl_t::write_PSWH_GM(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSWH_GM(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_PSWH_GM(0);
    }else{
      cpu_state_impl_t::write_PSWH_GM(value & 0x1);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSWH_GM(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSWH_GM(m_code_gen_impl->gen_i1_constant(0));
  }else{
    fastiss::i1_t *mask =  m_code_gen_impl->gen_i1_constant(0x1);
    fastiss::i1_t *masked_pswh_gm_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_PSWH_GM(m_code_gen_impl->gen_select(is_hv_valid, masked_pswh_gm_value, m_code_gen_impl->gen_i1_constant(0)));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_PSWH_GPID(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_PSWH_GPID(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_PSWH_GPID(0);
    }else{
      cpu_state_impl_t::write_PSWH_GPID(value & 0x7);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_PSWH_GPID(fastiss::i8_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_PSWH_GPID(m_code_gen_impl->gen_i8_constant(0));
  }else{
    fastiss::i8_t *mask =  m_code_gen_impl->gen_i8_constant(0x7);
    fastiss::i8_t *masked_pswh_gpid_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_PSWH_GPID(m_code_gen_impl->gen_select(is_hv_valid, masked_pswh_gpid_value, m_code_gen_impl->gen_i8_constant(0)));
  }
}

// The RH850G4 EIPSWH register has bits 30-11 and 7-0 fixed to 0. Over-ride
// the CPU State functions that alter EIPSWH to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_EIPSWH(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EIPSWH(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_EIPSWH(0);
    }else{
      cpu_state_impl_t::write_EIPSWH(value & 0x80000700);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EIPSWH(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EIPSWH(m_code_gen_impl->gen_i32_constant(0));
  }else{
    fastiss::i32_t *mask =  m_code_gen_impl->gen_i32_constant(0x80000700);
    fastiss::i32_t *masked_eipswh_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_EIPSWH(m_code_gen_impl->gen_select(is_hv_valid, masked_eipswh_value, m_code_gen_impl->gen_i32_constant(0)));
  }
}

// Writing to the RH850G4 EIPSWH register GM, GPID fields
void rh850g4::custom_cpu_state_impl_t::write_EIPSWH_GM(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EIPSWH_GM(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_EIPSWH_GM(0);
    }else{
      cpu_state_impl_t::write_EIPSWH_GM(value & 0x1);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EIPSWH_GM(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EIPSWH_GM(m_code_gen_impl->gen_i1_constant(0));
  }else{
    fastiss::i1_t *mask =  m_code_gen_impl->gen_i1_constant(0x1);
    fastiss::i1_t *masked_eipswh_gm_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_EIPSWH_GM(m_code_gen_impl->gen_select(is_hv_valid, masked_eipswh_gm_value, m_code_gen_impl->gen_i1_constant(0)));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_EIPSWH_GPID(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_EIPSWH_GPID(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_EIPSWH_GPID(0);
    }else{
      cpu_state_impl_t::write_EIPSWH_GPID(value & 0x7);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_EIPSWH_GPID(fastiss::i8_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_EIPSWH_GPID(m_code_gen_impl->gen_i8_constant(0));
  }else{
    fastiss::i8_t *mask =  m_code_gen_impl->gen_i8_constant(0x7);
    fastiss::i8_t *masked_eipswh_gpid_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_EIPSWH_GPID(m_code_gen_impl->gen_select(is_hv_valid, masked_eipswh_gpid_value, m_code_gen_impl->gen_i8_constant(0)));
  }
}

// The RH850G4 FEPSWH register has bits 30-11 and 7-0 fixed to 0. Over-ride
// the CPU State functions that alter FEPSWH to mask these bits values.
void rh850g4::custom_cpu_state_impl_t::write_FEPSWH(unsigned int value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_FEPSWH(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_FEPSWH(0);
    }else{
      cpu_state_impl_t::write_FEPSWH(value & 0x80000700);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FEPSWH(fastiss::i32_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_FEPSWH(m_code_gen_impl->gen_i32_constant(0));
  }else{
    fastiss::i32_t *mask =  m_code_gen_impl->gen_i32_constant(0x80000700);
    fastiss::i32_t *masked_fepswh_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_FEPSWH(m_code_gen_impl->gen_select(is_hv_valid, masked_fepswh_value, m_code_gen_impl->gen_i32_constant(0)));
  }
}

// Writing to the RH850G4 FEPSWH register GM, GPID fields
void rh850g4::custom_cpu_state_impl_t::write_FEPSWH_GM(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_FEPSWH_GM(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_FEPSWH_GM(0);
    }else{
      cpu_state_impl_t::write_FEPSWH_GM(value & 0x1);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FEPSWH_GM(fastiss::i1_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_FEPSWH_GM(m_code_gen_impl->gen_i1_constant(0));
  }else{
    fastiss::i1_t *mask =  m_code_gen_impl->gen_i1_constant(0x1);
    fastiss::i1_t *masked_fepswh_gm_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_FEPSWH_GM(m_code_gen_impl->gen_select(is_hv_valid, masked_fepswh_gm_value, m_code_gen_impl->gen_i1_constant(0)));
  }
}

void rh850g4::custom_cpu_state_impl_t::write_FEPSWH_GPID(unsigned char value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::write_FEPSWH_GPID(0);
  }else{
    if (cpu_state_impl_t::read_HVCFG_HVE()==0){
      cpu_state_impl_t::write_FEPSWH_GPID(0);
    }else{
      cpu_state_impl_t::write_FEPSWH_GPID(value & 0x7);
    }
  }
}

void rh850g4::custom_cpu_state_impl_t::gen_write_FEPSWH_GPID(fastiss::i8_t *value)
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    cpu_state_impl_t::gen_write_FEPSWH_GPID(m_code_gen_impl->gen_i8_constant(0));
  }else{
    fastiss::i8_t *mask =  m_code_gen_impl->gen_i8_constant(0x7);
    fastiss::i8_t *masked_fepswh_gpid_value = m_code_gen_impl->gen_and(value, mask);
    // check the HV valid
    fastiss::i1_t *is_hv_valid = m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_HVCFG_HVE(), m_code_gen_impl->gen_i1_constant(1)),
							      m_code_gen_impl->gen_i1_constant(1), 
							      m_code_gen_impl->gen_i1_constant(0));
    cpu_state_impl_t::gen_write_FEPSWH_GPID(m_code_gen_impl->gen_select(is_hv_valid, masked_fepswh_gpid_value, m_code_gen_impl->gen_i8_constant(0)));
  }
}


// The rh850g4 EIPC register has the guest mode register.
// Over-ride the generative EIPC read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_EIPC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_EIPC();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EIPC();
    }else{
      return cpu_state_impl_t::read_GMEIPC();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_EIPC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EIPC();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEIPC(), 
					cpu_state_impl_t::gen_read_EIPC());
  }  
}

// The rh850g4 EIPSW register has the guest mode register.
// Over-ride the generative EIPSW read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_EIPSW()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_EIPSW();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EIPSW();
    }else{
      return cpu_state_impl_t::read_GMEIPSW();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_EIPSW()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EIPSW();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEIPSW(), 
					cpu_state_impl_t::gen_read_EIPSW());
  }  
}


// The rh850g4 FEPC register has the guest mode register.
// Over-ride the generative FEPC read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_FEPC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_FEPC();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_FEPC();
    }else{
      return cpu_state_impl_t::read_GMFEPC();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_FEPC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_FEPC();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMFEPC(), 
					cpu_state_impl_t::gen_read_FEPC());
  }  
}

// The rh850g4 FEPSW register has the guest mode register.
// Over-ride the generative FEPSW read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_FEPSW()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_FEPSW();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_FEPSW();
    }else{
      return cpu_state_impl_t::read_GMFEPSW();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_FEPSW()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_FEPSW();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMFEPSW(), 
					cpu_state_impl_t::gen_read_FEPSW());
  }  
}

// The rh850g4 PSW register has the guest mode register.
// Over-ride the generative PSW read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_PSW()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW();
    }else{
      return cpu_state_impl_t::read_GMPSW();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW(), 
					cpu_state_impl_t::gen_read_PSW());
  }  
}

// Reading to the RH850G4 PSW register Z, S, OV, CY, SAT, ID, EP, NP, 
// EBV, CUU0-2, EIMASK, UM fields
unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_Z()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_Z();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_Z();
    }else{
      return cpu_state_impl_t::read_GMPSW_Z();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_Z()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_Z();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_Z(), 
					cpu_state_impl_t::gen_read_PSW_Z());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_S()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_S();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_S();
    }else{
      return cpu_state_impl_t::read_GMPSW_S();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_S()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_S();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_S(), 
					cpu_state_impl_t::gen_read_PSW_S());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_OV()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_OV();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_OV();
    }else{
      return cpu_state_impl_t::read_GMPSW_OV();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_OV()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_OV();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_OV(), 
					cpu_state_impl_t::gen_read_PSW_OV());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_CY()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_CY();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_CY();
    }else{
      return cpu_state_impl_t::read_GMPSW_CY();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_CY()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_CY();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_CY(), 
					cpu_state_impl_t::gen_read_PSW_CY());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_SAT()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_SAT();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_SAT();
    }else{
      return cpu_state_impl_t::read_GMPSW_SAT();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_SAT()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_SAT();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_SAT(), 
					cpu_state_impl_t::gen_read_PSW_SAT());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_ID()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_ID();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_ID();
    }else{
      return cpu_state_impl_t::read_GMPSW_ID();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_ID()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_ID();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_ID(), 
					cpu_state_impl_t::gen_read_PSW_ID());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_EP()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_EP();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_EP();
    }else{
      return cpu_state_impl_t::read_GMPSW_EP();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_EP()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_EP();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_EP(), 
					cpu_state_impl_t::gen_read_PSW_EP());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_NP()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_NP();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_NP();
    }else{
      return cpu_state_impl_t::read_GMPSW_NP();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_NP()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_NP();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_NP(), 
					cpu_state_impl_t::gen_read_PSW_NP());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_EBV()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_EBV();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_EBV();
    }else{
      return cpu_state_impl_t::read_GMPSW_EBV();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_EBV()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_EBV();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_EBV(), 
					cpu_state_impl_t::gen_read_PSW_EBV());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_CU0()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_CU0();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_CU0();
    }else{
      return cpu_state_impl_t::read_GMPSW_CU0();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_CU0()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_CU0();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_CU0(), 
					cpu_state_impl_t::gen_read_PSW_CU0());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_CU1()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_CU1();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_CU1();
    }else{
      return cpu_state_impl_t::read_GMPSW_CU1();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_CU1()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_CU1();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_CU1(), 
					cpu_state_impl_t::gen_read_PSW_CU1());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_CU2()
{
  // PSW.CU2 and GMPSW.CU2 are fixed 0.
  return 0;
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_CU2()
{
  // PSW.CU2 and GMPSW.CU2 are fixed 0.
  return  m_code_gen_impl->gen_i1_constant(0);
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_EIMASK()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_EIMASK();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_EIMASK();
    }else{
      return cpu_state_impl_t::read_GMPSW_EIMASK();
    }
  }
}

fastiss::i8_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_EIMASK()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_EIMASK();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_EIMASK(), 
					cpu_state_impl_t::gen_read_PSW_EIMASK());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_PSW_UM()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PSW_UM();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PSW_UM();
    }else{
      return cpu_state_impl_t::read_GMPSW_UM();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_PSW_UM()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PSW_UM();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPSW_UM(), 
					cpu_state_impl_t::gen_read_PSW_UM());
  }  
}

// The rh850g4 EIIC register has the guest mode register.
// Over-ride the generative EIIC read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_EIIC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_EIIC();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EIIC();
    }else{
      return cpu_state_impl_t::read_GMEIIC();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_EIIC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EIIC();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEIIC(), 
					cpu_state_impl_t::gen_read_EIIC());
  }  
}

// The rh850g4 FEIC register has the guest mode register.
// Over-ride the generative FEIC read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_FEIC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_FEIC();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_FEIC();
    }else{
      return cpu_state_impl_t::read_GMFEIC();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_FEIC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_FEIC();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMFEIC(), 
					cpu_state_impl_t::gen_read_FEIC());
  }  
}

// The rh850g4 EIWR register has the guest mode register.
// Over-ride the generative EIWR read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_EIWR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_EIWR();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EIWR();
    }else{
      return cpu_state_impl_t::read_GMEIWR();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_EIWR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EIWR();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEIWR(), 
					cpu_state_impl_t::gen_read_EIWR());
  }  
}

// The rh850g4 FEWR register has the guest mode register.
// Over-ride the generative FEWR read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_FEWR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_FEWR();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_FEWR();
    }else{
      return cpu_state_impl_t::read_GMFEWR();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_FEWR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_FEWR();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMFEWR(), 
					cpu_state_impl_t::gen_read_FEWR());
  }  
}

// The rh850g4 SPID register has the guest mode register.
// Over-ride the generative SPID read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_SPID()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_SPID();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_SPID();
    }else{
      return cpu_state_impl_t::read_GMSPID();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_SPID()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_SPID();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMSPID(), 
					cpu_state_impl_t::gen_read_SPID());
  }  
}

// The rh850g4 SPIDLIST register has the guest mode register.
// Over-ride the generative SPIDLIST read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_SPIDLIST()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_SPIDLIST();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_SPIDLIST();
    }else{
      return cpu_state_impl_t::read_GMSPIDLIST();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_SPIDLIST()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_SPIDLIST();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMSPIDLIST(), 
					cpu_state_impl_t::gen_read_SPIDLIST());
  }  
}

// The rh850g4 EBASE register has the guest mode register.
// Over-ride the generative EBASE read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_EBASE()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){ 
    return cpu_state_impl_t::read_EBASE();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EBASE();
    }else{
      return cpu_state_impl_t::read_GMEBASE();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_EBASE()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EBASE();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEBASE(), 
					cpu_state_impl_t::gen_read_EBASE());
  }  
}

// Reading to the RH850G4 EBASE register EBASE, DV, RINT fields
unsigned int rh850g4::custom_cpu_state_impl_t::read_EBASE_EBASE()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_EBASE_EBASE();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EBASE_EBASE();
    }else{
      return cpu_state_impl_t::read_GMEBASE_EBASE();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_EBASE_EBASE()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EBASE_EBASE();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEBASE_EBASE(), 
					cpu_state_impl_t::gen_read_EBASE_EBASE());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_EBASE_DV()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_EBASE_DV();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EBASE_DV();
    }else{
      return cpu_state_impl_t::read_GMEBASE_DV();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_EBASE_DV()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EBASE_DV();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEBASE_DV(), 
					cpu_state_impl_t::gen_read_EBASE_DV());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_EBASE_RINT()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_EBASE_RINT();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_EBASE_RINT();
    }else{
      return cpu_state_impl_t::read_GMEBASE_RINT();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_EBASE_RINT()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_EBASE_RINT();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMEBASE_RINT(), 
					cpu_state_impl_t::gen_read_EBASE_RINT());
  }  
}

// The rh850g4 INTBP register has the guest mode register.
// Over-ride the generative INTBP read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_INTBP()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_INTBP();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_INTBP();
    }else{
      return cpu_state_impl_t::read_GMINTBP();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_INTBP()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_INTBP();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMINTBP(), 
					cpu_state_impl_t::gen_read_INTBP());
  }  
}

// The rh850g4 SVLOCK register has the guest mode register.
// Over-ride the generative SVLOCK read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_SVLOCK()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_SVLOCK();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_SVLOCK();
    }else{
      return cpu_state_impl_t::read_GMSVLOCK();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_SVLOCK()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_SVLOCK();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMSVLOCK(), 
					cpu_state_impl_t::gen_read_SVLOCK());
  }  
}

// The rh850g4 PEID register has the guest mode register.
// Over-ride the generative PEID read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_PEID()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PEID();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PEID();
    }else{
      return cpu_state_impl_t::read_GMPEID();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_PEID()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PEID();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPEID(), 
					cpu_state_impl_t::gen_read_PEID());
  }  
}

// The rh850g4 MEA register has the guest mode register.
// Over-ride the generative MEA read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_MEA()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_MEA();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_MEA();
    }else{
      return cpu_state_impl_t::read_GMMEA();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_MEA()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_MEA();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMMEA(), 
					cpu_state_impl_t::gen_read_MEA());
  }  
}

// The rh850g4 MEI register has the guest mode register.
// Over-ride the generative MEI read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_MEI()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_MEI();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_MEI();
    }else{
      return cpu_state_impl_t::read_GMMEI();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_MEI()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_MEI();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMMEI(), 
					cpu_state_impl_t::gen_read_MEI());
  }  
}

// The rh850g4 INTCFG register has the guest mode register.
// Over-ride the generative INTCFG read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_INTCFG()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_INTCFG();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_INTCFG();
    }else{
      return cpu_state_impl_t::read_GMINTCFG();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_INTCFG()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_INTCFG();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMINTCFG(), 
					cpu_state_impl_t::gen_read_INTCFG());
  }  
}

// Reading to the RH850G4 INTCFG register ISPC, EPL, ULNR fields
unsigned char rh850g4::custom_cpu_state_impl_t::read_INTCFG_ISPC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_INTCFG_ISPC();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_INTCFG_ISPC();
    }else{
      return cpu_state_impl_t::read_GMINTCFG_ISPC();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_INTCFG_ISPC()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_INTCFG_ISPC();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMINTCFG_ISPC(), 
					cpu_state_impl_t::gen_read_INTCFG_ISPC());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_INTCFG_EPL()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_INTCFG_EPL();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_INTCFG_EPL();
    }else{
      return cpu_state_impl_t::read_GMINTCFG_EPL();
    }
  }
}

fastiss::i1_t *rh850g4::custom_cpu_state_impl_t::gen_read_INTCFG_EPL()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_INTCFG_EPL();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMINTCFG_EPL(), 
					cpu_state_impl_t::gen_read_INTCFG_EPL());
  }  
}

unsigned char rh850g4::custom_cpu_state_impl_t::read_INTCFG_ULNR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_INTCFG_ULNR();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_INTCFG_ULNR();
    }else{
      return cpu_state_impl_t::read_GMINTCFG_ULNR();
    }
  }
}

fastiss::i8_t *rh850g4::custom_cpu_state_impl_t::gen_read_INTCFG_ULNR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_INTCFG_ULNR();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMINTCFG_ULNR(), 
					cpu_state_impl_t::gen_read_INTCFG_ULNR());
  }  
}

// The rh850g4 PLMR register has the guest mode register.
// Over-ride the generative PLMR read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_PLMR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PLMR();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PLMR();
    }else{
      return cpu_state_impl_t::read_GMPLMR();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_PLMR()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PLMR();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPLMR(), 
					cpu_state_impl_t::gen_read_PLMR());
  }  
}

// Reading to the RH850G4 PLMR register PLM fields
unsigned char rh850g4::custom_cpu_state_impl_t::read_PLMR_PLM()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_PLMR_PLM();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_PLMR_PLM();
    }else{
      return cpu_state_impl_t::read_GMPLMR_PLM();
    }
  }
}

fastiss::i8_t *rh850g4::custom_cpu_state_impl_t::gen_read_PLMR_PLM()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_PLMR_PLM();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMPLMR_PLM(), 
					cpu_state_impl_t::gen_read_PLMR_PLM());
  }  
}

// The rh850g4 MPM register has the guest mode register.
// Over-ride the generative MPM read function to ensure this.
unsigned int rh850g4::custom_cpu_state_impl_t::read_MPM()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::read_MPM();
  }else{
    if (cpu_state_impl_t::read_PSWH_GM()==0){
      return cpu_state_impl_t::read_MPM();
    }else{
      return cpu_state_impl_t::read_GMMPM();
    }
  }
}

fastiss::i32_t *rh850g4::custom_cpu_state_impl_t::gen_read_MPM()
{
  if(cpu_state_impl_t::read_hv_func_support()==0){
    return cpu_state_impl_t::gen_read_MPM();
  }else{
    // check the CPU mode
    return  m_code_gen_impl->gen_select(m_code_gen_impl->gen_and(cpu_state_impl_t::gen_read_PSWH_GM(), m_code_gen_impl->gen_i1_constant(1)),
					cpu_state_impl_t::gen_read_GMMPM(), 
					cpu_state_impl_t::gen_read_MPM());
  }  
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------

