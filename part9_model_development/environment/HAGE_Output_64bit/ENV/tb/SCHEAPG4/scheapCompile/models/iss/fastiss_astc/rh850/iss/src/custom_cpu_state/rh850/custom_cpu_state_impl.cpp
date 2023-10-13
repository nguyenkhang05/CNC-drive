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

#include "rh850/custom_cpu_state_impl.h"
#include "fastiss/code_gen_if.h"

#include <stdio.h>
#include <iostream>

//------------------------------------------------------------------------------
// rh850 custom CPU state implementation
//------------------------------------------------------------------------------

namespace rh850
{
  void print_helper_cpu_state_sysreg(unsigned int value, unsigned int sysreg)
  {
    printf("print_helper_cpu_state_sysreg\n");
    printf("  value=0x%x sysreg=0x%x\n", value, sysreg);
    fflush(stdout);
  }
}

rh850::custom_cpu_state_impl_t::custom_cpu_state_impl_t() :
  rh850::cpu_state_impl_t()
{
  //fastiss::func_args_type_t *print_helper_cpu_state_sysreg_args = m_code_gen_impl->create_args_type();
  //m_code_gen_impl->append_i32(print_helper_cpu_state_sysreg_args);
  //m_code_gen_impl->append_i32(print_helper_cpu_state_sysreg_args);
  //m_code_gen_impl->add_helper("print_helper_cpu_state_sysreg", (void*)&rh850::print_helper_cpu_state_sysreg, m_code_gen_impl->create_void_func(print_helper_cpu_state_sysreg_args));
  //m_code_gen_impl->delete_args_type(print_helper_cpu_state_sysreg_args);
}

rh850::custom_cpu_state_impl_t::~custom_cpu_state_impl_t()
{
}

// Over-ride the general purpose register name for the special case
// general registers, i.e. for the SP, GP, TP, EP, and LP cases
std::string rh850::custom_cpu_state_impl_t::register_name_r3(void)
{
  return std::string("sp");
}

std::string rh850::custom_cpu_state_impl_t::register_name_r4(void)
{
  return std::string("gp");
}

std::string rh850::custom_cpu_state_impl_t::register_name_r5(void)
{
  return std::string("tp");
}

std::string rh850::custom_cpu_state_impl_t::register_name_r30(void)
{
  return std::string("ep");
}

std::string rh850::custom_cpu_state_impl_t::register_name_r31(void)
{
  return std::string("lp");
}

// The RH850 r0 register always holds 0 and it's value should never be
// changed. Over-ride the CPU State functions that alter r0 to ensure this.
void rh850::custom_cpu_state_impl_t::write_r0(unsigned int value)
{
}

void rh850::custom_cpu_state_impl_t::gen_write_r0(fastiss::i32_t *value)
{
}

// A contant zero value can be returned for r0 as it cannot change value.
unsigned int rh850::custom_cpu_state_impl_t::read_r0()
{
  return 0;
}

fastiss::i32_t *rh850::custom_cpu_state_impl_t::gen_read_r0()
{
  return m_code_gen_impl->gen_i32_constant(0);
}

// The RH850 PC register has bit 0 fixed to 0. Over-ride the CPU State
// functions that alter PC to mask the bit 0 value.
void rh850::custom_cpu_state_impl_t::write_PC(unsigned int value)
{
  cpu_state_impl_t::write_PC(value & 0xfffffffe);
}

void rh850::custom_cpu_state_impl_t::gen_write_PC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffe);
  fastiss::i32_t *masked_pc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_PC(masked_pc_value);
}

// The RH850 EIPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter EIPC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_EIPC(unsigned int value)
{
  cpu_state_impl_t::write_EIPC(value & 0xffffffff);
}

void rh850::custom_cpu_state_impl_t::gen_write_EIPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_eipc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_EIPC(masked_eipc_value);
}

// The RH850 EIPSW register has bits 29-20, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter EIPSW to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_EIPSW(unsigned int value)
{
  cpu_state_impl_t::write_EIPSW(value & 0xc0098eff);
}

void rh850::custom_cpu_state_impl_t::gen_write_EIPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xc0098eff);
  fastiss::i32_t *masked_eipsw_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_EIPSW(masked_eipsw_value);
}

// The RH850 FEPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter FEPC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_FEPC(unsigned int value)
{
  cpu_state_impl_t::write_FEPC(value & 0xffffffff);
}

void rh850::custom_cpu_state_impl_t::gen_write_FEPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_fepc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FEPC(masked_fepc_value);
}

// The RH850 FEPSW register has bits 29-20, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter FEPSW to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_FEPSW(unsigned int value)
{
  cpu_state_impl_t::write_FEPSW(value & 0xc0098eff);
}

void rh850::custom_cpu_state_impl_t::gen_write_FEPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xc0098eff);
  fastiss::i32_t *masked_fepsw_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FEPSW(masked_fepsw_value);
}

// The RH850 PSW register has bits 29-20, 18, 14-12, and 8 fixed to 0. Over-ride
// the CPU State functions that alter PSW to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_PSW(unsigned int value)
{
  cpu_state_impl_t::write_PSW(value & 0xc0098eff);
}

void rh850::custom_cpu_state_impl_t::gen_write_PSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xc0098eff);
  fastiss::i32_t *masked_psw_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_PSW(masked_psw_value);
}

// Writing to the RH850 FPSR register IF, XC, XP, CC, RM, or XE fields
// also writes the values to the corresponding fields in the FPST, FPCC,
// and FPCFG registers.
void rh850::custom_cpu_state_impl_t::write_FPSR_IF(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_IF(value);
  cpu_state_impl_t::write_FPST_IF(read_FPSR_IF());
}

void rh850::custom_cpu_state_impl_t::write_FPSR_XC(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_XC(value);
  cpu_state_impl_t::write_FPST_XC(read_FPSR_XC());
}

void rh850::custom_cpu_state_impl_t::write_FPSR_XP(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_XP(value);
  cpu_state_impl_t::write_FPST_XP(read_FPSR_XP());
}

void rh850::custom_cpu_state_impl_t::write_FPSR_CC(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_CC(value);
  cpu_state_impl_t::write_FPCC_CC(read_FPSR_CC());
}

void rh850::custom_cpu_state_impl_t::write_FPSR_RM(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_RM(value);
  cpu_state_impl_t::write_FPCFG_RM(read_FPSR_RM());
}

void rh850::custom_cpu_state_impl_t::write_FPSR_XE(unsigned char value)
{
  cpu_state_impl_t::write_FPSR_XE(value);
  cpu_state_impl_t::write_FPCFG_XE(read_FPSR_XE());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPSR_IF(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_IF(value);
  cpu_state_impl_t::gen_write_FPST_IF(gen_read_FPSR_IF());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPSR_XC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_XC(value);
  cpu_state_impl_t::gen_write_FPST_XC(gen_read_FPSR_XC());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPSR_XP(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_XP(value);
  cpu_state_impl_t::gen_write_FPST_XP(gen_read_FPSR_XP());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPSR_CC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_CC(value);
  cpu_state_impl_t::gen_write_FPCC_CC(gen_read_FPSR_CC());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPSR_RM(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_RM(value);
  cpu_state_impl_t::gen_write_FPCFG_RM(gen_read_FPSR_RM());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPSR_XE(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPSR_XE(value);
  cpu_state_impl_t::gen_write_FPCFG_XE(gen_read_FPSR_XE());
}

// The RH850 FPEPC register has bits 0 fixed to 0. Over-ride
// the CPU State functions that alter FPEPC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_FPEPC(unsigned int value)
{
  cpu_state_impl_t::write_FPEPC(value & 0xfffffffe);
}

void rh850::custom_cpu_state_impl_t::gen_write_FPEPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffe);
  fastiss::i32_t *masked_fpepc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FPEPC(masked_fpepc_value);
}

// Writing to the RH850 FPST register IF, XC, or XP fields also writes the
// values to the corresponding fields in the FPSR register.
void rh850::custom_cpu_state_impl_t::write_FPST_IF(unsigned char value)
{
  cpu_state_impl_t::write_FPST_IF(value);
  cpu_state_impl_t::write_FPSR_IF(read_FPST_IF());
}

void rh850::custom_cpu_state_impl_t::write_FPST_XC(unsigned char value)
{
  cpu_state_impl_t::write_FPST_XC(value);
  cpu_state_impl_t::write_FPSR_XC(read_FPST_XC());
}

void rh850::custom_cpu_state_impl_t::write_FPST_XP(unsigned char value)
{
  cpu_state_impl_t::write_FPST_XP(value);
  cpu_state_impl_t::write_FPSR_XP(read_FPST_XP());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPST_IF(fastiss::i1_t *value)
{
  cpu_state_impl_t::gen_write_FPST_IF(value);
  cpu_state_impl_t::gen_write_FPSR_IF(gen_read_FPST_IF());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPST_XC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPST_XC(value);
  cpu_state_impl_t::gen_write_FPSR_XC(gen_read_FPST_XC());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPST_XP(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPST_XP(value);
  cpu_state_impl_t::gen_write_FPSR_XP(gen_read_FPST_XP());
}

// Writing to the RH850 FPCC register CC fields also writes the
// values to the corresponding fields in the FPSR register.
void rh850::custom_cpu_state_impl_t::write_FPCC_CC(unsigned char value)
{
  cpu_state_impl_t::write_FPCC_CC(value);
  cpu_state_impl_t::write_FPSR_CC(read_FPCC_CC());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPCC_CC(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPCC_CC(value);
  cpu_state_impl_t::gen_write_FPSR_CC(gen_read_FPCC_CC());
}

// Writing to the RH850 FPCFG register RM or XE fields also writes the
// values to the corresponding fields in the FPSR register.
void rh850::custom_cpu_state_impl_t::write_FPCFG_RM(unsigned char value)
{
  cpu_state_impl_t::write_FPCFG_RM(value);
  cpu_state_impl_t::write_FPSR_RM(read_FPCFG_RM());
}

void rh850::custom_cpu_state_impl_t::write_FPCFG_XE(unsigned char value)
{
  cpu_state_impl_t::write_FPCFG_XE(value);
  cpu_state_impl_t::write_FPSR_XE(read_FPCFG_XE());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPCFG_RM(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPCFG_RM(value);
  cpu_state_impl_t::gen_write_FPSR_RM(gen_read_FPCFG_RM());
}

void rh850::custom_cpu_state_impl_t::gen_write_FPCFG_XE(fastiss::i8_t *value)
{
  cpu_state_impl_t::gen_write_FPCFG_XE(value);
  cpu_state_impl_t::gen_write_FPSR_XE(gen_read_FPCFG_XE());
}

// The RH850 FPEC register has bits 31-1 fixed to 0. Over-ride
// the CPU State functions that alter FPEC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_FPEC(unsigned int value)
{
  cpu_state_impl_t::write_FPEC(value & 0x00000001);
}

void rh850::custom_cpu_state_impl_t::gen_write_FPEC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000001);
  fastiss::i32_t *masked_fpec_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FPEC(masked_fpec_value);
}

// The RH850 CTPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter CTPC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_CTPC(unsigned int value)
{
  cpu_state_impl_t::write_CTPC(value & 0xffffffff);
}

void rh850::custom_cpu_state_impl_t::gen_write_CTPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_ctpc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_CTPC(masked_ctpc_value);
}

// The RH850 CTPSW register has bits 31-5 fixed to 0. Over-ride
// the CPU State functions that alter CTPSW to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_CTPSW(unsigned int value)
{
  cpu_state_impl_t::write_CTPSW(value & 0x0000001f);
}

void rh850::custom_cpu_state_impl_t::gen_write_CTPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_ctpsw_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_CTPSW(masked_ctpsw_value);
}

// The RH850 CTBP register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter CTBP to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_CTBP(unsigned int value)
{
  cpu_state_impl_t::write_CTBP(value & 0xfffffffe);
}

void rh850::custom_cpu_state_impl_t::gen_write_CTBP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffe);
  fastiss::i32_t *masked_ctbp_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_CTBP(masked_ctbp_value);
}

// The RH850 BSEL register always holds 0 and it's value should never be
// changed. Over-ride the CPU State functions that alter BSEL to ensure this.
void rh850::custom_cpu_state_impl_t::write_BSEL(unsigned int value)
{
}

void rh850::custom_cpu_state_impl_t::gen_write_BSEL(fastiss::i32_t *value)
{
}

// A contant zero value can be returned for BSEL as it cannot change value.
unsigned int rh850::custom_cpu_state_impl_t::read_BSEL()
{
  return 0;
}

fastiss::i32_t *rh850::custom_cpu_state_impl_t::gen_read_BSEL()
{
  return m_code_gen_impl->gen_i32_constant(0);
}

// The RH850 MCFG0 register has bit 31-18 and 15-4 fixed to 0, bit2 fixed to 1(NC)
// Over-ride the CPU State functions that alter MCFG0 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MCFG0(unsigned int value)
{
  cpu_state_impl_t::write_MCFG0((value & 0x001f000f) | 0x4);
}

void rh850::custom_cpu_state_impl_t::gen_write_MCFG0(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x001f000f);
  fastiss::i32_t *masked_mcfg0_value = m_code_gen_impl->gen_and(value, mask);
  fastiss::i32_t *nc_hvp_value = m_code_gen_impl->gen_i32_constant(0x00000004);
  fastiss::i32_t *nc_mcfg0_value = m_code_gen_impl->gen_or(masked_mcfg0_value, nc_hvp_value);
  cpu_state_impl_t::gen_write_MCFG0(nc_mcfg0_value);
}

// The RH850 RBASE register has bit 8-1 fixed to 0. Over-ride
// the CPU State functions that alter RBASE to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_RBASE(unsigned int value)
{
  cpu_state_impl_t::write_RBASE(value & 0xfffffe01);
}

void rh850::custom_cpu_state_impl_t::gen_write_RBASE(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffe01);
  fastiss::i32_t *masked_rbase_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_RBASE(masked_rbase_value);
}

// The RH850 EBASE register has bit 8-1 fixed to 0. Over-ride
// the CPU State functions that alter EBASE to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_EBASE(unsigned int value)
{
  cpu_state_impl_t::write_EBASE(value & 0xfffffe01);
}

void rh850::custom_cpu_state_impl_t::gen_write_EBASE(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffe01);
  fastiss::i32_t *masked_ebase_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_EBASE(masked_ebase_value);
}

// The RH850 INTBP register has bit 8-0 fixed to 0. Over-ride
// the CPU State functions that alter INTBP to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_INTBP(unsigned int value)
{
  cpu_state_impl_t::write_INTBP(value & 0xfffffe00);
}

void rh850::custom_cpu_state_impl_t::gen_write_INTBP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffe00);
  fastiss::i32_t *masked_intbp_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_INTBP(masked_intbp_value);
}

// The RH850 MCTL register has bit 29-22 and 15-2 fixed to 0. Over-ride
// the CPU State functions that alter MCTL to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MCTL(unsigned int value)
{
  cpu_state_impl_t::write_MCTL((value & 0x3) | 0x80000000);
}

void rh850::custom_cpu_state_impl_t::gen_write_MCTL(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x3);
  fastiss::i32_t *masked_mctl_value = m_code_gen_impl->gen_and(value, mask);
  fastiss::i32_t *nc_en_value = m_code_gen_impl->gen_i32_constant(0x80000000);
  fastiss::i32_t *nc_mctl_value = m_code_gen_impl->gen_or(nc_en_value, nc_mctl_value);
  cpu_state_impl_t::gen_write_MCTL(masked_mctl_value);
}

// The RH850 FPIPR register has bit 31-5 fixed to 0. Over-ride
// the CPU State functions that alter FPIPR to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_FPIPR(unsigned int value)
{
  cpu_state_impl_t::write_FPIPR(value & 0x0000001f);
}

void rh850::custom_cpu_state_impl_t::gen_write_FPIPR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000001f);
  fastiss::i32_t *masked_fpipr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FPIPR(masked_fpipr_value);
}

// The RH850 FPIPR.FPIPR register's max value is 16
// the CPU State functions that alter FPIPR to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_FPIPR_FPIPR(unsigned char value)
{
  cpu_state_impl_t::write_FPIPR_FPIPR(value & 0x1f);
}

void rh850::custom_cpu_state_impl_t::gen_write_FPIPR_FPIPR(fastiss::i8_t *value)
{
  fastiss::i8_t *mask = m_code_gen_impl->gen_i8_constant(0x1f);
  fastiss::i8_t *masked_fpipr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_FPIPR_FPIPR(masked_fpipr_value);
}

// The RH850 SCCFG register has bit 31-8 fixed to 0. Over-ride
// the CPU State functions that alter SCCFG to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_SCCFG(unsigned int value)
{
  cpu_state_impl_t::write_SCCFG(value & 0x000000ff);
}

void rh850::custom_cpu_state_impl_t::gen_write_SCCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
  fastiss::i32_t *masked_sccfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_SCCFG(masked_sccfg_value);
}

// The RH850 SCBP register has bits 1 and 0 fixed to 0. Over-ride
// the CPU State functions that alter SCBP to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_SCBP(unsigned int value)
{
  cpu_state_impl_t::write_SCBP(value & 0xfffffffc);
}

void rh850::custom_cpu_state_impl_t::gen_write_SCBP(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
  fastiss::i32_t *masked_scbp_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_SCBP(masked_scbp_value);
}

// The RH850 HTCFG0 register has bit 31-19 and 14-11 fixed to 0. Over-ride
// the CPU State functions that alter HTCFG0 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_HTCFG0(unsigned int value)
{
  cpu_state_impl_t::write_HTCFG0(value & 0x000787ff);
}

void rh850::custom_cpu_state_impl_t::gen_write_HTCFG0(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000787ff);
  fastiss::i32_t *masked_htcfg0_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_HTCFG0(masked_htcfg0_value);
}

// The RH850 ASID register has bit 31-10 fixed to 0. Over-ride
// the CPU State functions that alter ASID to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_ASID(unsigned int value)
{
  cpu_state_impl_t::write_ASID(value & 0x000003ff);
}

void rh850::custom_cpu_state_impl_t::gen_write_ASID(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000003ff);
  fastiss::i32_t *masked_asid_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ASID(masked_asid_value);
}

// The RH850 MEI register has bit 31-21, 15-11 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter MEI to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MEI(unsigned int value)
{
  cpu_state_impl_t::write_MEI(value & 0x001f073f);
}

void rh850::custom_cpu_state_impl_t::gen_write_MEI(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x001f073f);
  fastiss::i32_t *masked_mei_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MEI(masked_mei_value);
}

// The RH850 ISPR register has bit 31-16 fixed to 0. Over-ride
// the CPU State functions that alter ISPR to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_ISPR(unsigned int value)
{
  cpu_state_impl_t::write_ISPR(value & 0x0000ffff);
}

void rh850::custom_cpu_state_impl_t::gen_write_ISPR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000ffff);
  fastiss::i32_t *masked_ispr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ISPR(masked_ispr_value);
}

// The RH850 PMR register has bit 31-16 fixed to 0. Over-ride
// the CPU State functions that alter PMR to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_PMR(unsigned int value)
{
  cpu_state_impl_t::write_PMR(value & 0x0000ffff);
}

void rh850::custom_cpu_state_impl_t::gen_write_PMR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000ffff);
  fastiss::i32_t *masked_pmr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_PMR(masked_pmr_value);
}

// The RH850 ICSR register has bit 31-2 fixed to 0. Over-ride
// the CPU State functions that alter ICSR to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_ICSR(unsigned int value)
{
  cpu_state_impl_t::write_ICSR(value & 0x00000003);
}

void rh850::custom_cpu_state_impl_t::gen_write_ICSR(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000003);
  fastiss::i32_t *masked_icsr_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_ICSR(masked_icsr_value);
}

// The RH850 INTCFG register has bit 31-1 fixed to 0. Over-ride
// the CPU State functions that alter INTCFG to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_INTCFG(unsigned int value)
{
  cpu_state_impl_t::write_INTCFG(value & 0x00000001);
}

void rh850::custom_cpu_state_impl_t::gen_write_INTCFG(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000001);
  fastiss::i32_t *masked_intcfg_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_INTCFG(masked_intcfg_value);
}

// The RH850 MPM register has bit 31-11 and 7-3 fixed to 0. Over-ride
// the CPU State functions that alter MPM to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPM(unsigned int value)
{
  cpu_state_impl_t::write_MPM(value & 0x00000703);
}

void rh850::custom_cpu_state_impl_t::gen_write_MPM(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x00000703);
  fastiss::i32_t *masked_mpm_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPM(masked_mpm_value);
}

// The RH850 MPRC register has bit 31-16 fixed to 0. Over-ride
// the CPU State functions that alter MPRC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPRC(unsigned int value)
{
  cpu_state_impl_t::write_MPRC(value & 0x0000ffff);
  cpu_state_impl_t::write_MPAT0_E(read_MPRC_E0());
  cpu_state_impl_t::write_MPAT1_E(read_MPRC_E1());
  cpu_state_impl_t::write_MPAT2_E(read_MPRC_E2());
  cpu_state_impl_t::write_MPAT3_E(read_MPRC_E3());
  cpu_state_impl_t::write_MPAT4_E(read_MPRC_E4());
  cpu_state_impl_t::write_MPAT5_E(read_MPRC_E5());
  cpu_state_impl_t::write_MPAT6_E(read_MPRC_E6());
  cpu_state_impl_t::write_MPAT7_E(read_MPRC_E7());
  cpu_state_impl_t::write_MPAT8_E(read_MPRC_E8());
  cpu_state_impl_t::write_MPAT9_E(read_MPRC_E9());
  cpu_state_impl_t::write_MPAT10_E(read_MPRC_E10());
  cpu_state_impl_t::write_MPAT11_E(read_MPRC_E11());
  cpu_state_impl_t::write_MPAT12_E(read_MPRC_E12());
  cpu_state_impl_t::write_MPAT13_E(read_MPRC_E13());
  cpu_state_impl_t::write_MPAT14_E(read_MPRC_E14());
  cpu_state_impl_t::write_MPAT15_E(read_MPRC_E15());
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0000ffff);
  fastiss::i32_t *masked_mprc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_MPRC(masked_mprc_value);
  cpu_state_impl_t::gen_write_MPAT0_E(gen_read_MPRC_E0());
  cpu_state_impl_t::gen_write_MPAT1_E(gen_read_MPRC_E1());
  cpu_state_impl_t::gen_write_MPAT2_E(gen_read_MPRC_E2());
  cpu_state_impl_t::gen_write_MPAT3_E(gen_read_MPRC_E3());
  cpu_state_impl_t::gen_write_MPAT4_E(gen_read_MPRC_E4());
  cpu_state_impl_t::gen_write_MPAT5_E(gen_read_MPRC_E5());
  cpu_state_impl_t::gen_write_MPAT6_E(gen_read_MPRC_E6());
  cpu_state_impl_t::gen_write_MPAT7_E(gen_read_MPRC_E7());
  cpu_state_impl_t::gen_write_MPAT8_E(gen_read_MPRC_E8());
  cpu_state_impl_t::gen_write_MPAT9_E(gen_read_MPRC_E9());
  cpu_state_impl_t::gen_write_MPAT10_E(gen_read_MPRC_E10());
  cpu_state_impl_t::gen_write_MPAT11_E(gen_read_MPRC_E11());
  cpu_state_impl_t::gen_write_MPAT12_E(gen_read_MPRC_E12());
  cpu_state_impl_t::gen_write_MPAT13_E(gen_read_MPRC_E13());
  cpu_state_impl_t::gen_write_MPAT14_E(gen_read_MPRC_E14());
  cpu_state_impl_t::gen_write_MPAT15_E(gen_read_MPRC_E15());
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E0(unsigned char value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E0(value & 0x1);
    cpu_state_impl_t::write_MPAT0_E(read_MPRC_E0());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E0(fastiss::i1_t *value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E0(value);
    cpu_state_impl_t::gen_write_MPAT0_E(gen_read_MPRC_E0());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E1(unsigned char value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E1(value & 0x1);
    cpu_state_impl_t::write_MPAT1_E(read_MPRC_E1());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E1(fastiss::i1_t *value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E1(value);
    cpu_state_impl_t::gen_write_MPAT1_E(gen_read_MPRC_E1());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E2(unsigned char value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E2(value & 0x1);
    cpu_state_impl_t::write_MPAT2_E(read_MPRC_E2());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E2(fastiss::i1_t *value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E2(value);
    cpu_state_impl_t::gen_write_MPAT2_E(gen_read_MPRC_E2());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E3(unsigned char value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E3(value & 0x1);
    cpu_state_impl_t::write_MPAT3_E(read_MPRC_E3());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E3(fastiss::i1_t *value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E3(value);
    cpu_state_impl_t::gen_write_MPAT3_E(gen_read_MPRC_E3());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E4(unsigned char value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E4(value & 0x1);
    cpu_state_impl_t::write_MPAT4_E(read_MPRC_E4());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E4(fastiss::i1_t *value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E4(value);
    cpu_state_impl_t::gen_write_MPAT4_E(gen_read_MPRC_E4());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E5(unsigned char value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E5(value & 0x1);
    cpu_state_impl_t::write_MPAT5_E(read_MPRC_E5());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E5(fastiss::i1_t *value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E5(value);
    cpu_state_impl_t::gen_write_MPAT5_E(gen_read_MPRC_E5());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E6(unsigned char value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E6(value & 0x1);
    cpu_state_impl_t::write_MPAT6_E(read_MPRC_E6());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E6(fastiss::i1_t *value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E6(value);
    cpu_state_impl_t::gen_write_MPAT6_E(gen_read_MPRC_E6());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E7(unsigned char value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E7(value & 0x1);
    cpu_state_impl_t::write_MPAT7_E(read_MPRC_E7());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E7(fastiss::i1_t *value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E7(value);
    cpu_state_impl_t::gen_write_MPAT7_E(gen_read_MPRC_E7());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E8(unsigned char value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E8(value & 0x1);
    cpu_state_impl_t::write_MPAT8_E(read_MPRC_E8());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E8(fastiss::i1_t *value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E8(value);
    cpu_state_impl_t::gen_write_MPAT8_E(gen_read_MPRC_E8());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E9(unsigned char value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E9(value & 0x1);
    cpu_state_impl_t::write_MPAT9_E(read_MPRC_E9());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E9(fastiss::i1_t *value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E9(value);
    cpu_state_impl_t::gen_write_MPAT9_E(gen_read_MPRC_E9());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E10(unsigned char value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E10(value & 0x1);
    cpu_state_impl_t::write_MPAT10_E(read_MPRC_E10());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E10(fastiss::i1_t *value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E10(value);
    cpu_state_impl_t::gen_write_MPAT10_E(gen_read_MPRC_E10());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E11(unsigned char value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E11(value & 0x1);
    cpu_state_impl_t::write_MPAT11_E(read_MPRC_E11());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E11(fastiss::i1_t *value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E11(value);
    cpu_state_impl_t::gen_write_MPAT11_E(gen_read_MPRC_E11());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E12(unsigned char value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E12(value & 0x1);
    cpu_state_impl_t::write_MPAT12_E(read_MPRC_E12());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E12(fastiss::i1_t *value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E12(value);
    cpu_state_impl_t::gen_write_MPAT12_E(gen_read_MPRC_E12());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E13(unsigned char value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E13(value & 0x1);
    cpu_state_impl_t::write_MPAT13_E(read_MPRC_E13());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E13(fastiss::i1_t *value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E13(value);
    cpu_state_impl_t::gen_write_MPAT13_E(gen_read_MPRC_E13());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E14(unsigned char value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E14(value & 0x1);
    cpu_state_impl_t::write_MPAT14_E(read_MPRC_E14());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E14(fastiss::i1_t *value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E14(value);
    cpu_state_impl_t::gen_write_MPAT14_E(gen_read_MPRC_E14());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPRC_E15(unsigned char value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPRC_E15(value & 0x1);
    cpu_state_impl_t::write_MPAT15_E(read_MPRC_E15());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPRC_E15(fastiss::i1_t *value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPRC_E15(value);
    cpu_state_impl_t::gen_write_MPAT15_E(gen_read_MPRC_E15());
  }
}

// The RH850 MPBRGN register has bit 31-4 fixed to 0. Over-ride
// the CPU State functions that alter MPBRGN to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPBRGN(unsigned int value)
{
  cpu_state_impl_t::write_MPBRGN(0);
}

void rh850::custom_cpu_state_impl_t::gen_write_MPBRGN(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_MPBRGN(m_code_gen_impl->gen_i32_constant(0));
}

// The RH850 MPTRGN register has bit 31-4 fixed to 0. Over-ride
// the CPU State functions that alter MPTRGN to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPTRGN(unsigned int value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()){
    cpu_state_impl_t::write_MPTRGN(cpu_state_impl_t::read_mpu_entry() - 1);
  }
  else{
    cpu_state_impl_t::write_MPTRGN(0);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPTRGN(fastiss::i32_t *value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()){
    fastiss::i8_t* mptrgn_value = m_code_gen_impl->gen_sub(cpu_state_impl_t::gen_read_mpu_entry(), m_code_gen_impl->gen_i8_constant(1));
    cpu_state_impl_t::gen_write_MPTRGN(m_code_gen_impl->gen_i32_zext(mptrgn_value));
  }
  else{
    cpu_state_impl_t::gen_write_MPTRGN(m_code_gen_impl->gen_i32_constant(0));
  }
}


// The RH850 MCR register has bit 31-9 and 7-6 fixed to 0. Over-ride
// the CPU State functions that alter MCR to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MCR(unsigned int value)
{
  cpu_state_impl_t::write_MCR(0x0000003f);
}

void rh850::custom_cpu_state_impl_t::gen_write_MCR(fastiss::i32_t *value)
{
  cpu_state_impl_t::gen_write_MCR(m_code_gen_impl->gen_i32_constant(0x0000003f));
}

// The RH850 MPLA0 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA0 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA0(unsigned int value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA0(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA0(fastiss::i32_t *value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla0_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA0(masked_mpla0_value);
  }
}

// The RH850 MPUA0 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA0 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA0(unsigned int value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA0(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA0(fastiss::i32_t *value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua0_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA0(masked_mpua0_value);
  }
}

// The RH850 MPAT0 E bit corres 
// the CPU State functions that alter MPAT0 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT0(unsigned int value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT0(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E0(read_MPAT0_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT0(fastiss::i32_t *value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT0(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E0(gen_read_MPAT0_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT0_E(unsigned char value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT0_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E0(read_MPAT0_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT0_E(fastiss::i1_t *value)
{
  if (0 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT0_E(value);
    cpu_state_impl_t::gen_write_MPRC_E0(gen_read_MPAT0_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT0_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT0_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT0_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT0_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT0_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT0_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA1 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA1 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA1(unsigned int value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA1(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA1(fastiss::i32_t *value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla1_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA1(masked_mpla1_value);
  }
}

// The RH850 MPUA1 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA1 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA1(unsigned int value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA1(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA1(fastiss::i32_t *value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua1_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA1(masked_mpua1_value);
  }
}

// The RH850 MPAT1 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT1 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT1(unsigned int value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT1(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E1(read_MPAT1_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT1(fastiss::i32_t *value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT1(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E1(gen_read_MPAT1_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT1_E(unsigned char value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT1_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E1(read_MPAT1_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT1_E(fastiss::i1_t *value)
{
  if (1 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT1_E(value);
    cpu_state_impl_t::gen_write_MPRC_E1(gen_read_MPAT1_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT1_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT1_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT1_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT1_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT1_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT1_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}
// The RH850 MPLA2 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA2 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA2(unsigned int value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA2(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA2(fastiss::i32_t *value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla2_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA2(masked_mpla2_value);
  }
}

// The RH850 MPUA2 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA2 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA2(unsigned int value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA2(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA2(fastiss::i32_t *value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua2_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA2(masked_mpua2_value);
  }
}

// The RH850 MPAT2 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT2 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT2(unsigned int value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT2(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E2(read_MPAT2_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT2(fastiss::i32_t *value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT2(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E2(gen_read_MPAT2_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT2_E(unsigned char value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT2_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E2(read_MPAT2_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT2_E(fastiss::i1_t *value)
{
  if (2 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT2_E(value);
    cpu_state_impl_t::gen_write_MPRC_E2(gen_read_MPAT2_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT2_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT2_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT2_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT2_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT2_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT2_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA3 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA3 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA3(unsigned int value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA3(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA3(fastiss::i32_t *value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla3_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA3(masked_mpla3_value);
  }
}

// The RH850 MPUA3 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA3 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA3(unsigned int value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA3(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA3(fastiss::i32_t *value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua3_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA3(masked_mpua3_value);
  }
}

// The RH850 MPAT3 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT3 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT3(unsigned int value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT3(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E3(read_MPAT3_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT3(fastiss::i32_t *value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT3(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E3(gen_read_MPAT3_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT3_E(unsigned char value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT3_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E3(read_MPAT3_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT3_E(fastiss::i1_t *value)
{
  if (3 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT3_E(value);
    cpu_state_impl_t::gen_write_MPRC_E3(gen_read_MPAT3_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT3_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT3_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT3_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT3_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT3_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT3_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA4 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA4 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA4(unsigned int value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA4(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA4(fastiss::i32_t *value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla4_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA4(masked_mpla4_value);
  }
}

// The RH850 MPUA4 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA4 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA4(unsigned int value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA4(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA4(fastiss::i32_t *value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua4_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA4(masked_mpua4_value);
  }
}

// The RH850 MPAT4 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT4 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT4(unsigned int value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT4(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E4(read_MPAT4_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT4(fastiss::i32_t *value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT4(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E4(gen_read_MPAT4_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT4_E(unsigned char value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT4_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E4(read_MPAT4_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT4_E(fastiss::i1_t *value)
{
  if (4 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT4_E(value);
    cpu_state_impl_t::gen_write_MPRC_E4(gen_read_MPAT4_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT4_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT4_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT4_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT4_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT4_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT4_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}


// The RH850 MPLA5 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA5 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA5(unsigned int value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA5(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA5(fastiss::i32_t *value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla0_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA5(masked_mpla0_value);
  }
}

// The RH850 MPUA5 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA5 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA5(unsigned int value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA5(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA5(fastiss::i32_t *value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua5_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA5(masked_mpua5_value);
  }
}

// The RH850 MPAT5 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT5 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT5(unsigned int value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT5(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E5(read_MPAT5_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT5(fastiss::i32_t *value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT5(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E5(gen_read_MPAT5_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT5_E(unsigned char value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT5_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E5(read_MPAT5_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT5_E(fastiss::i1_t *value)
{
  if (5 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT5_E(value);
    cpu_state_impl_t::gen_write_MPRC_E5(gen_read_MPAT5_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT5_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT5_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT5_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT5_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT5_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT5_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA6 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA6 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA6(unsigned int value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA6(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA6(fastiss::i32_t *value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla6_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA6(masked_mpla6_value);
  }
}

// The RH850 MPUA6 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA6 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA6(unsigned int value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA6(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA6(fastiss::i32_t *value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua6_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA6(masked_mpua6_value);
  }
}

// The RH850 MPAT6 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT6 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT6(unsigned int value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT6(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E6(read_MPAT6_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT6(fastiss::i32_t *value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT6(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E6(gen_read_MPAT6_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT6_E(unsigned char value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT6_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E6(read_MPAT6_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT6_E(fastiss::i1_t *value)
{
  if (6 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT6_E(value);
    cpu_state_impl_t::gen_write_MPRC_E6(gen_read_MPAT6_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT6_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT6_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT6_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT6_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT6_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT6_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA7 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA7 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA7(unsigned int value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA7(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA7(fastiss::i32_t *value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla7_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA7(masked_mpla7_value);
  }
}

// The RH850 MPUA7 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA7 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA7(unsigned int value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA7(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA7(fastiss::i32_t *value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua7_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA7(masked_mpua7_value);
  }
}

// The RH850 MPAT7 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT7 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT7(unsigned int value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT7(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E7(read_MPAT7_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT7(fastiss::i32_t *value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT7(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E7(gen_read_MPAT7_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT7_E(unsigned char value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT7_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E7(read_MPAT7_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT7_E(fastiss::i1_t *value)
{
  if (7 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT7_E(value);
    cpu_state_impl_t::gen_write_MPRC_E7(gen_read_MPAT7_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT7_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT7_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT7_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT7_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT7_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT7_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA8 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA8 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA8(unsigned int value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA8(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA8(fastiss::i32_t *value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla8_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA8(masked_mpla8_value);
  }
}

// The RH850 MPUA8 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA8 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA8(unsigned int value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA8(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA8(fastiss::i32_t *value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua8_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA8(masked_mpua8_value);
  }
}

// The RH850 MPAT8 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT8 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT8(unsigned int value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT8(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E8(read_MPAT8_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT8(fastiss::i32_t *value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT8(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E8(gen_read_MPAT8_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT8_E(unsigned char value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT8_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E8(read_MPAT8_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT8_E(fastiss::i1_t *value)
{
  if (8 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT8_E(value);
    cpu_state_impl_t::gen_write_MPRC_E8(gen_read_MPAT8_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT8_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT8_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT8_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT8_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT8_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT8_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA9 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA9 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA9(unsigned int value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA9(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA9(fastiss::i32_t *value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla9_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA9(masked_mpla9_value);
  }
}

// The RH850 MPUA9 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA9 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA9(unsigned int value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA9(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA9(fastiss::i32_t *value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua9_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA9(masked_mpua9_value);
  }
}

// The RH850 MPAT9 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT9 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT9(unsigned int value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT9(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E9(read_MPAT9_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT9(fastiss::i32_t *value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT9(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E9(gen_read_MPAT9_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT9_E(unsigned char value)
{
  if (9 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT9_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E9(read_MPAT9_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT9_E(fastiss::i1_t *value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT9_E(value);
    cpu_state_impl_t::gen_write_MPRC_E9(gen_read_MPAT9_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT9_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT9_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT9_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT9_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT9_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT9_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA10 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA10 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA10(unsigned int value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA10(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA10(fastiss::i32_t *value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla10_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA10(masked_mpla10_value);
  }
}

// The RH850 MPUA10 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA10 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA10(unsigned int value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA10(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA10(fastiss::i32_t *value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua10_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA10(masked_mpua10_value);
  }
}

// The RH850 MPAT10 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT10 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT10(unsigned int value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT10(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E10(read_MPAT10_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT10(fastiss::i32_t *value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT10(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E10(gen_read_MPAT10_E());
  }
}


void rh850::custom_cpu_state_impl_t::write_MPAT10_E(unsigned char value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT10_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E10(read_MPAT10_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT10_E(fastiss::i1_t *value)
{
  if (10 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT10_E(value);
    cpu_state_impl_t::gen_write_MPRC_E10(gen_read_MPAT10_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT10_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT10_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT10_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT10_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT10_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT10_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA11 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA11 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA11(unsigned int value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA11(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA11(fastiss::i32_t *value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla11_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA11(masked_mpla11_value);
  }
}

// The RH850 MPUA11 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA11 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA11(unsigned int value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA11(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA11(fastiss::i32_t *value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua11_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA11(masked_mpua11_value);
  }
}

// The RH850 MPAT11 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT11 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT11(unsigned int value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT11(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E11(read_MPAT11_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT11(fastiss::i32_t *value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT11(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E11(gen_read_MPAT11_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT11_E(unsigned char value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT11_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E11(read_MPAT11_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT11_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT11_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT11_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT11_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT11_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT11_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT11_E(fastiss::i1_t *value)
{
  if (11 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT11_E(value);
    cpu_state_impl_t::gen_write_MPRC_E11(gen_read_MPAT11_E());
  }
}


// The RH850 MPLA12 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA12 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA12(unsigned int value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA12(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA12(fastiss::i32_t *value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla12_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA12(masked_mpla12_value);
  }
}

// The RH850 MPUA12 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA12 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA12(unsigned int value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA12(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA12(fastiss::i32_t *value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua12_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA12(masked_mpua12_value);
  }
}

// The RH850 MPAT12 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT12 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT12(unsigned int value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT12(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E12(read_MPAT12_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT12(fastiss::i32_t *value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT12(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E12(gen_read_MPAT12_E());
  }
}


void rh850::custom_cpu_state_impl_t::write_MPAT12_E(unsigned char value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT12_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E12(read_MPAT12_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT12_E(fastiss::i1_t *value)
{
  if (12 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT12_E(value);
    cpu_state_impl_t::gen_write_MPRC_E12(gen_read_MPAT12_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT12_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT12_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT12_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT12_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT12_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT12_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA13 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA13 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA13(unsigned int value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA13(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA13(fastiss::i32_t *value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla13_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA13(masked_mpla13_value);
  }
}

// The RH850 MPUA13 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA13 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA13(unsigned int value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA13(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA13(fastiss::i32_t *value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua13_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA13(masked_mpua13_value);
  }
}

// The RH850 MPAT13 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT13 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT13(unsigned int value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT13(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E13(read_MPAT13_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT13(fastiss::i32_t *value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT13(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E13(gen_read_MPAT13_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT13_E(unsigned char value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT13_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E13(read_MPAT13_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT13_E(fastiss::i1_t *value)
{
  if (13 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT13_E(value);
    cpu_state_impl_t::gen_write_MPRC_E13(gen_read_MPAT13_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT13_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT13_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT13_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT13_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT13_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT13_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA14 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA14 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA14(unsigned int value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA14(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA14(fastiss::i32_t *value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla14_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA14(masked_mpla14_value);
  }
}

// The RH850 MPUA14 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA14 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA14(unsigned int value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA14(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA14(fastiss::i32_t *value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua14_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA14(masked_mpua14_value);
  }
}

// The RH850 MPAT14 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT11 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT14(unsigned int value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT14(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E14(read_MPAT14_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT14(fastiss::i32_t *value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT14(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E14(gen_read_MPAT14_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT14_E(unsigned char value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT14_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E14(read_MPAT14_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT14_E(fastiss::i1_t *value)
{
  if (14 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT14_E(value);
    cpu_state_impl_t::gen_write_MPRC_E14(gen_read_MPAT14_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT14_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT14_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT14_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT14_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT14_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT14_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 MPLA15 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPLA15 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPLA15(unsigned int value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPLA15(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPLA15(fastiss::i32_t *value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpla15_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPLA15(masked_mpla15_value);
  }
}

// The RH850 MPUA15 register has bit 31-2 fixed to 0. Over-ride 
// the CPU State functions that alter MPUA15 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPUA15(unsigned int value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPUA15(value & 0xfffffffc);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPUA15(fastiss::i32_t *value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffffffc);
    fastiss::i32_t *masked_mpua15_value = m_code_gen_impl->gen_and(value, mask);
    cpu_state_impl_t::gen_write_MPUA15(masked_mpua15_value);
  }
}

// The RH850 MPAT15 register has bit 31-26 and 15-8 fixed to 0. Over-ride 
// the CPU State functions that alter MPAT15 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_MPAT15(unsigned int value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    unsigned int masked_mpat_value;
    if (cpu_state_impl_t::read_mpat_g_support()){
       masked_mpat_value = (value & 0x03ff00ff);
    }
    else {
       masked_mpat_value = ((value & 0x000000ff) | 0x00000040);
    }
    cpu_state_impl_t::write_MPAT15(masked_mpat_value);
    cpu_state_impl_t::write_MPRC_E15(read_MPAT15_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT15(fastiss::i32_t *value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    fastiss::i32_t *mask;
    fastiss::i32_t *masked_mpat_value; 
    if (cpu_state_impl_t::read_mpat_g_support()) {
      mask = m_code_gen_impl->gen_i32_constant(0x03ff00ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
    }
    else {
      mask = m_code_gen_impl->gen_i32_constant(0x000000ff);
      masked_mpat_value = m_code_gen_impl->gen_and(value, mask);
      mask = m_code_gen_impl->gen_i32_constant(0x00000040);
      masked_mpat_value = m_code_gen_impl->gen_or(masked_mpat_value, mask);
    }
    cpu_state_impl_t::gen_write_MPAT15(masked_mpat_value);
    cpu_state_impl_t::gen_write_MPRC_E15(gen_read_MPAT15_E());
  }
}

void rh850::custom_cpu_state_impl_t::write_MPAT15_E(unsigned char value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::write_MPAT15_E(value & 0x1);
    cpu_state_impl_t::write_MPRC_E15(read_MPAT15_E());
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT15_E(fastiss::i1_t *value)
{
  if (15 < cpu_state_impl_t::read_mpu_entry()) {
    cpu_state_impl_t::gen_write_MPAT15_E(value);
    cpu_state_impl_t::gen_write_MPRC_E15(gen_read_MPAT15_E());
  }
}
void rh850::custom_cpu_state_impl_t::write_MPAT15_G(unsigned char value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::write_MPAT15_G(value & 0x1);
  }
  else {
    cpu_state_impl_t::write_MPAT15_G(0x1);
  }
}

void rh850::custom_cpu_state_impl_t::gen_write_MPAT15_G(fastiss::i1_t *value)
{
  if (cpu_state_impl_t::read_mpat_g_support()) {
    cpu_state_impl_t::gen_write_MPAT15_G(value);
  }
  else {
    cpu_state_impl_t::gen_write_MPAT15_G(m_code_gen_impl->gen_i1_constant(0x1));
  }
}

// The RH850 DBCMC register has bit 31-19 fixed to 0. Over-ride
// the CPU State functions that alter DBCMC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_DBCMC(unsigned int value)
{
  cpu_state_impl_t::write_DBCMC(value & 0x0007ffff);
}

void rh850::custom_cpu_state_impl_t::gen_write_DBCMC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x0007ffff);
  fastiss::i32_t *masked_dbcmc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DBCMC(masked_dbcmc_value);
}

// The RH850 DBPC register has bit 0 fixed to 0. Over-ride
// the CPU State functions that alter DBPC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_DBPC(unsigned int value)
{
  cpu_state_impl_t::write_DBPC(value & 0xffffffff);
}

void rh850::custom_cpu_state_impl_t::gen_write_DBPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xffffffff);
  fastiss::i32_t *masked_dbpc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DBPC(masked_dbpc_value);
}

// The RH850 DBPSW register has bits 29-20, 14-12, 10 and 8 fixed to 0. Over-ride
// the CPU State functions that alter DBPSW to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_DBPSW(unsigned int value)
{
  cpu_state_impl_t::write_DBPSW(value & 0xc0098aff);
}

void rh850::custom_cpu_state_impl_t::gen_write_DBPSW(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xc0098aff);
  fastiss::i32_t *masked_dbpsw_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DBPSW(masked_dbpsw_value);
}

// The RH850 DIR0 register has bits 11-10 and 7 fixed to 0. Over-ride
// the CPU State functions that alter DIR0 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_DIR0(unsigned int value)
{
  cpu_state_impl_t::write_DIR0(value & 0xfffff37f);
}

void rh850::custom_cpu_state_impl_t::gen_write_DIR0(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0xfffff37f);
  fastiss::i32_t *masked_dir0_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DIR0(masked_dir0_value);
}

// The RH850 DIR1 register has bits 31-20 and 3 fixed to 0. Over-ride
// the CPU State functions that alter DIR1 to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_DIR1(unsigned int value)
{
  cpu_state_impl_t::write_DIR1(value & 0x000ffff7);
}

void rh850::custom_cpu_state_impl_t::gen_write_DIR1(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x000ffff7);
  fastiss::i32_t *masked_dir1_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_DIR1(masked_dir1_value);
}

// The RH850 BPC register has bits 31-29 and 23-13 fixed to 0. Over-ride
// the CPU State functions that alter BPC to mask these bits values.
void rh850::custom_cpu_state_impl_t::write_BPC(unsigned int value)
{
  cpu_state_impl_t::write_BPC(value & 0x1f001fff);
}

void rh850::custom_cpu_state_impl_t::gen_write_BPC(fastiss::i32_t *value)
{
  fastiss::i32_t *mask = m_code_gen_impl->gen_i32_constant(0x1f001fff);
  fastiss::i32_t *masked_bpc_value = m_code_gen_impl->gen_and(value, mask);
  cpu_state_impl_t::gen_write_BPC(masked_bpc_value);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
