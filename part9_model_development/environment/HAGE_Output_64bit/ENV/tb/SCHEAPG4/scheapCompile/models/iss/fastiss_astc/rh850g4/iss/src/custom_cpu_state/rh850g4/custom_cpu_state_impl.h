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

/**
 * \file custom_cpu_state_impl.h
 * \brief Defines the customised CPU state implementation class
 */

#ifndef _RH850G4_CUSTOM_CPU_STATE_IMPL_H_
#define _RH850G4_CUSTOM_CPU_STATE_IMPL_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/cpu_state_impl.h"

//------------------------------------------------------------------------------
// rh850g4 custom CPU state definition
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief rh850g4 customised CPU state implementation. This class provides
   * customised implementations of the methods available in the CPU state
   * interface, allowing customised register specific write masks or behaviours.
   */
  class custom_cpu_state_impl_t : public rh850g4::cpu_state_impl_t
  {
  public:
    /**
     * \brief Constructor
     */
    custom_cpu_state_impl_t();
    /**
     * \brief Destructor
     */
    virtual ~custom_cpu_state_impl_t();

    /**
     * \brief Over-ride the name of the rh850g4 r3 register to be 'sp'
     * \return String representing the name of the rh850g4 r3 register
     */
    std::string register_name_r3(void);
    /**
     * \brief Over-ride the name of the rh850g4 r4 register to be 'gp'
     * \return String representing the name of the rh850g4 r4 register
     */
    std::string register_name_r4(void);
    /**
     * \brief Over-ride the name of the rh850g4 r5 register to be 'tp'
     * \return String representing the name of the rh850g4 r5 register
     */
    std::string register_name_r5(void);
    /**
     * \brief Over-ride the name of the rh850g4 r30 register to be 'ep'
     * \return String representing the name of the rh850g4 r30 register
     */
    std::string register_name_r30(void);
    /**
     * \brief Over-ride the name of the rh850g4 r31 register to be 'lp'
     * \return String representing the name of the rh850g4 r31 register
     */
    std::string register_name_r31(void);
    
    /**
     * \brief The rh850g4 r0 register always holds 0 and it's value should never
     * be changed. Over-ride the immediate r0 write function to ensure this.
     * \param value This value is ignored
     */
    void write_r0(unsigned int value);
    /**
     * \brief The rh850g4 r0 register always holds 0 and it's value should never
     * be changed. Over-ride the generative r0 write function to ensure this.
     * \param value This value is ignored
     */
    void gen_write_r0(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 r0 register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the immediate r0 read function
     * to ensure this.
     * \return A constant value of zero
     */
    unsigned int read_r0(void);
    /**
     * \brief The rh850g4 r0 register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the generative r0 read function
     * to ensure this.
     * \return A 32-bit constant value of zero
     */
    fastiss::i32_t *gen_read_r0(void);
    
    /**
     * \brief The rh850g4 PC register has bits 0 fixed to 0. Over-ride
     * the immediate PC write function to ensure this.
     * \param value New unmasked value of the rh850g4 PC register
     */
    void write_PC(unsigned int value);
    /**
     * \brief The rh850g4 PC register has bits 0 fixed to 0. Over-ride
     * the generative PC write function to ensure this.
     * \param value New unmasked value of the rh850g4 PC register
     */
    void gen_write_PC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 EIPC register has bits 0 fixed to 0. Over-ride
     * the immediate EIPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPC register
     */
    void write_EIPC(unsigned int value);
    /**
     * \brief The rh850g4 EIPC register has bits 0 fixed to 0. Over-ride
     * the generative EIPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPC register
     */
    void gen_write_EIPC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 EIPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the immediate EIPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPSW register
     */
    void write_EIPSW(unsigned int value);
    /**
     * \brief The rh850g4 EIPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative EIPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPSW register
     */
    void gen_write_EIPSW(fastiss::i32_t *value);
	
    /**
     * \brief The rh850g4 FEPC register has bits 0 fixed to 0. Over-ride
     * the immediate FEPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEPC register
     */
    void write_FEPC(unsigned int value);
    /**
     * \brief The rh850g4 FEPC register has bits 0 fixed to 0. Over-ride
     * the generative FEPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEPC register
     */
    void gen_write_FEPC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 FEPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0.
     * Over-ride the immediate FEPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEPSW register
     */
    void write_FEPSW(unsigned int value);
    /**
     * \brief The rh850g4 FEPSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative FEPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEPSW register
     */
    void gen_write_FEPSW(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 PSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the immediate PSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void write_PSW(unsigned int value);
    /**
     * \brief The rh850g4 PSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative PSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void gen_write_PSW(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 PSW register has the guest mode register.
     * Over-ride the generative PSW (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void write_PSW_Z(unsigned char value);
    void write_PSW_S(unsigned char value);
    void write_PSW_OV(unsigned char value);
    void write_PSW_CY(unsigned char value);
    void write_PSW_SAT(unsigned char value);
    void write_PSW_ID(unsigned char value);
    void write_PSW_EP(unsigned char value);
    void write_PSW_NP(unsigned char value);
    void write_PSW_EBV(unsigned char value);
    void write_PSW_CU0(unsigned char value);
    void write_PSW_CU1(unsigned char value);
    void write_PSW_CU2(unsigned char value);
    void write_PSW_EIMASK(unsigned char value);
    void write_PSW_UM(unsigned char value);
    /**
     * \brief The rh850g4 PSW register has the guest mode register.
     * Over-ride the generative PSW (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void gen_write_PSW_Z(fastiss::i1_t *value);
    void gen_write_PSW_S(fastiss::i1_t *value);
    void gen_write_PSW_OV(fastiss::i1_t *value);
    void gen_write_PSW_CY(fastiss::i1_t *value);
    void gen_write_PSW_SAT(fastiss::i1_t *value);
    void gen_write_PSW_ID(fastiss::i1_t *value);
    void gen_write_PSW_EP(fastiss::i1_t *value);
    void gen_write_PSW_NP(fastiss::i1_t *value);
    void gen_write_PSW_EBV(fastiss::i1_t *value);
    void gen_write_PSW_CU0(fastiss::i1_t *value);
    void gen_write_PSW_CU1(fastiss::i1_t *value);
    void gen_write_PSW_CU2(fastiss::i1_t *value);
    void gen_write_PSW_EIMASK(fastiss::i8_t *value);
    void gen_write_PSW_UM(fastiss::i1_t *value);

    /**
     * \brief Writing to the rh850g4 FPSR register IF, XC, XP, CC, RM, or XE fields
     * also writes the values to the corresponding fields in the FPST, FPCC,
     * and FPCFG registers. Over-ride the immediate FPSR write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FPSR register
     */
    void write_FPSR_IF(unsigned char value);
    void write_FPSR_XC(unsigned char value);
    void write_FPSR_XP(unsigned char value);
    void write_FPSR_CC(unsigned char value);
    void write_FPSR_RM(unsigned char value);
    void write_FPSR_XE(unsigned char value);
    /**
     * \brief Writing to the rh850g4 FPSR register IF, XC, XP, CC, RM, or XE fields
     * also writes the values to the corresponding fields in the FPST, FPCC,
     * and FPCFG registers. Over-ride the generative FPSR write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FPSR register
     */
    void gen_write_FPSR_IF(fastiss::i1_t *value);
    void gen_write_FPSR_XC(fastiss::i8_t *value);
    void gen_write_FPSR_XP(fastiss::i8_t *value);
    void gen_write_FPSR_CC(fastiss::i8_t *value);
    void gen_write_FPSR_RM(fastiss::i8_t *value);
    void gen_write_FPSR_XE(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 FPEPC register has bits 31-29 and 0 fixed to 0. Over-ride
     * the immediate FPEPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPEPC register
     */
    void write_FPEPC(unsigned int value);
    /**
     * \brief The rh850g4 FPEPC register has bits 31-29 and 0 fixed to 0. Over-ride
     * the generative FPEPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPEPC register
     */
    void gen_write_FPEPC(fastiss::i32_t *value);

    /**
     * \brief Writing to the rh850g4 FPST register IF, XC, or XP fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the immediate
     * FPST write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPST register
     */
    void write_FPST_IF(unsigned char value);
    void write_FPST_XC(unsigned char value);
    void write_FPST_XP(unsigned char value);
    /**
     * \brief Writing to the rh850g4 FPST register IF, XC, or XP fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the generative
     * FPST write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPST register
     */
    void gen_write_FPST_IF(fastiss::i1_t *value);
    void gen_write_FPST_XC(fastiss::i8_t *value);
    void gen_write_FPST_XP(fastiss::i8_t *value);

    /**
     * \brief Writing to the rh850g4 FPCC register CC fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the immediate
     * FPCC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPCC register
     */
    void write_FPCC_CC(unsigned char value);
    /**
     * \brief Writing to the rh850g4 FPCC register CC fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the generative
     * FPCC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPCC register
     */
    void gen_write_FPCC_CC(fastiss::i8_t *value);

    /**
     * \brief Writing to the rh850g4 FPCFG register RM or XE fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the immediate
     * FPCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPCFG register
     */
    void write_FPCFG_RM(unsigned char value);
    void write_FPCFG_XE(unsigned char value);
    /**
     * \brief Writing to the rh850g4 FPCFG register RM or XE fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the generative
     * FPCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 FPCFG register
     */
    void gen_write_FPCFG_RM(fastiss::i8_t *value);
    void gen_write_FPCFG_XE(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 EIIC register has the guest mode register.
     * Over-ride the generative EIIC write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIIC register
     */
    void write_EIIC(unsigned int value);
    /**
     * \brief The rh850g4 EIIC register has the guest mode register.
     * Over-ride the generative EIIC write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIIC register
     */
    void gen_write_EIIC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 FEIC register has the guest mode register.
     * Over-ride the generative FEIC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEIC register
     */
    void write_FEIC(unsigned int value);
    /**
     * \brief The rh850g4 FEIC register has the guest mode register.
     * Over-ride the generative FEIC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEIC register
     */
    void gen_write_FEIC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 CTPC register has bits 0 fixed to 0. Over-ride
     * the immediate CTPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 CTPC register
     */
    void write_CTPC(unsigned int value);
    /**
     * \brief The rh850g4 CTPC register has bits 0 fixed to 0. Over-ride
     * the generative CTPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 CTPC register
     */
    void gen_write_CTPC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 CTPSW register has bits 31-5 fixed to 0. Over-ride
     * the immediate CTPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 CTPSW register
     */
    void write_CTPSW(unsigned int value);
    /**
     * \brief The rh850g4 CTPSW register has bits 31-5 fixed to 0. Over-ride
     * the generative CTPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 CTPSW register
     */
    void gen_write_CTPSW(fastiss::i32_t *value);
  
    /**
     * \brief The rh850g4 CTBP register has bits 0 and 0 fixed to 0. Over-ride
     * the immediate CTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 CTBP register
     */
    void write_CTBP(unsigned int value);
    /**
     * \brief The rh850g4 CTBP register has bits 0 and 0 fixed to 0. Over-ride
     * the generative CTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 CTBP register
     */
    void gen_write_CTBP(fastiss::i32_t *value);
    
    /**
     * \brief The rh850g4 SNZCFG register has bits 31-8 fixed to 0. Over-ride
     * the generative CTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 SNZCFG register
     */
    void write_SNZCFG(unsigned int value);
    /**
     * \brief The rh850g4 SNZCFG register has bits 31-8 fixed to 0. Over-ride
     * the generative CTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 SNZCFG register
     */
    void gen_write_SNZCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 EIWR register has the guest mode register.
     * Over-ride the generative EIWR write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIWR register
     */
    void write_EIWR(unsigned int value);
    /**
     * \brief The rh850g4 EIWR register has the guest mode register.
     * Over-ride the generative EIWR write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIWR register
     */
    void gen_write_EIWR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 FEWR register has the guest mode register.
     * Over-ride the generative FEWR write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEWR register
     */
    void write_FEWR(unsigned int value);
    /**
     * \brief The rh850g4 FEWR register has the guest mode register.
     * Over-ride the generative FEWR write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEWR register
     */
    void gen_write_FEWR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 SPID register has bits 31-4 fixed to 0. Over-ride
     * the immediate RBASE write function to ensure this.
     * \param value New unmasked value of the rh850g4 SPID register
     */
    void write_SPID(unsigned int value);
    /**
     * \brief The rh850g4 SPID register has bits 31-5 fixed to 0. Over-ride
     * the generative RBASE write function to ensure this.
     * \param value New unmasked value of the rh850g4 SPID register
     */
    void gen_write_SPID(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 SPIDLIST register has the guest mode register.
     * Over-ride the generative SPIDLIST write function to ensure this.
     * \param value New unmasked value of the rh850g4 SPIDLIST register
     */
    void write_SPIDLIST(unsigned int value);
    /**
     * \brief The rh850g4 SPIDLIST register has the guest mode register.
     * Over-ride the generative SPIDLIST write function to ensure this.
     * \param value New unmasked value of the rh850g4 SPIDLIST register
     */
    void gen_write_SPIDLIST(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 RBASE register has bits 8-2 fixed to 0. Over-ride
     * the immediate RBASE write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBASE register
     */
    void write_RBASE(unsigned int value);
    /**
     * \brief The rh850g4 RBASE register has bits 8-2 fixed to 0. Over-ride
     * the generative RBASE write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBASE register
     */
    void gen_write_RBASE(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 EBASE register has bits 8-2 fixed to 0. Over-ride
     * the immediate EBASE write function to ensure this.
     * \param value New unmasked value of the rh850g4 EBASE register
     */
    void write_EBASE(unsigned int value);
    /**
     * \brief The rh850g4 EBASE register has bits 8-2 fixed to 0. Over-ride
     * the generative EBASE write function to ensure this.
     * \param value New unmasked value of the rh850g4 EBASE register
     */
    void gen_write_EBASE(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 EBASE register has the guest mode register.
     * Over-ride the generative EBASE (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 EBASE register
     */
    void write_EBASE_EBASE(unsigned int value);
    void write_EBASE_DV(unsigned char value);
    void write_EBASE_RINT(unsigned char value);

    /**
     * \brief The rh850g4 EBASE register has the guest mode register.
     * Over-ride the generative EBASE (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 EBASE register
     */
    void gen_write_EBASE_EBASE(fastiss::i32_t *value);
    void gen_write_EBASE_DV(fastiss::i1_t *value);
    void gen_write_EBASE_RINT(fastiss::i1_t *value);

    /**
     * \brief The rh850g4 INTBP register has bits 8-0 fixed to 0. Over-ride
     * the immediate INTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 INTBP register
     */
    void write_INTBP(unsigned int value);
    /**
     * \brief The rh850g4 INTBP register has bits 8-0 fixed to 0. Over-ride
     * the generative INTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 INTBP register
     */
    void gen_write_INTBP(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MCTL register has bits 31-1 fixed to 0. 
     * Over-ride the immediate MCTL write function to ensure this.
     * \param value New unmasked value of the rh850g4 MCTL register
     */
    void write_MCTL(unsigned int value);
    /**
     * \brief The rh850g4 MCTL register has bits 31-1 fixed to 0. 
     * Over-ride the generative MCTL write function to ensure this.
     * \param value New unmasked value of the rh850g4 MCTL register
     */
    void gen_write_MCTL(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 SVLOCK register has bits 31-1 fixed to 0. 
     * Over-ride the immediate SVLOCK write function to ensure this.
     * \param value New unmasked value of the rh850g4 SVLOCK register
     */
    void write_SVLOCK(unsigned int value);
    /**
     * \brief The rh850g4 SVLOCK register has bits 31-1 fixed to 0. 
     * Over-ride the generative SVLOCK write function to ensure this.
     * \param value New unmasked value of the rh850g4 SVLOCK register
     */
    void gen_write_SVLOCK(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 SCCFG register has bits 31-8 fixed to 0. 
     * Over-ride the immediate SCCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 SCCFG register
     */
    void write_SCCFG(unsigned int value);
    /**
     * \brief The rh850g4 SCCFG register has bits 31-8 fixed to 0. 
     * Over-ride the generative SCCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 SCCFG register
     */
    void gen_write_SCCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 SCBP register has bits 1 and 0 fixed to 0. Over-ride
     * the immediate SCBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 SCBP register
     */
    void write_SCBP(unsigned int value);
    /**
     * \brief The rh850g4 SCBP register has bits 1 and 0 fixed to 0. Over-ride
     * the generative SCBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 SCBP register
     */
    void gen_write_SCBP(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMCFG register has bits 31-19, 15-5 and 3-2 fixed to 0. Over-ride
     * the immediate GMCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMCFG register
     */
    void write_GMCFG(unsigned int value);
    /**
     * \brief The rh850g4 GMCFG register has bits 31-19, 15-5 and 3-2 fixed to 0. Over-ride
      * the generative GMCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMCFG register
     */
    void gen_write_GMCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMCFG register has some bit fields.
     * Over-ride the generative GMCFG (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMCFG register
     */
    void write_GMCFG_GCU0(unsigned char value);
    void write_GMCFG_GCU1(unsigned char value);

    /**
     * \brief The rh850g4 GMCFG register has some bit fields.
     * Over-ride the generative GMCFG (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMCFG register
     */
    void gen_write_GMCFG_GCU0(fastiss::i1_t *value);
    void gen_write_GMCFG_GCU1(fastiss::i1_t *value);

     /**
     * \brief The rh850g4 PEID register has bits 31-3 fixed to 0. Over-ride
     * the immediate PEID write function to ensure this.
     * \param value New unmasked value of the rh850g4 PEID register
     */
    void write_PEID(unsigned int value);
    /**
     * \brief The rh850g4 PEID register has bits 31-3 fixed to 0. Over-ride
     * the generative PEID write function to ensure this.
     * \param value New unmasked value of the rh850g4 PEID register
     */
    void gen_write_PEID(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 BMID register has bits 31-5 fixed to 0. Over-ride
     * the immediate BMID write function to ensure this.
     * \param value New unmasked value of the rh850g4 BMID register
     */
    void write_BMID(unsigned int value);
    /**
     * \brief The rh850g4 BMID register has bits 31-5 fixed to 0. Over-ride
     * the generative BMID write function to ensure this.
     * \param value New unmasked value of the rh850g4 BMID register
     */
    void gen_write_BMID(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MEA register has the guest mode register.
     * Over-ride the generative MEA write function to ensure this.
     * \param value New unmasked value of the rh850g4 MEA register
     */
    void write_MEA(unsigned int value);
    /**
     * \brief The rh850g4 MEA register has the guest mode register.
     * Over-ride the generative MEA write function to ensure this.
     * \param value New unmasked value of the rh850g4 MEA register
     */
    void gen_write_MEA(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MEI register has bits 27-21, 15-12 and 7-6 fixed to 0. 
     * Over-ride the immediate MEI write function to ensure this.
     * \param value New unmasked value of the rh850g4 MEI register
     */
    void write_MEI(unsigned int value);
    /**
     * \brief The rh850g4 MEI register has bits 27-21, 15-12 and 7-6 fixed to 0. 
     * Over-ride the generative MEI write function to ensure this.
     * \param value New unmasked value of the rh850g4 MEI register
     */
    void gen_write_MEI(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 ISPR register has bits 31-16 fixed to 0. 
     * Over-ride the immediate ISPR write function to ensure this.
     * \param value New unmasked value of the rh850g4 ISPR register
     */
    void write_ISPR(unsigned int value);
    /**
     * \brief The rh850g4 ISPR register has bits 31-16 fixed to 0. 
     * Over-ride the generative ISPR write function to ensure this.
     * \param value New unmasked value of the rh850g4 ISPR register
     */
    void gen_write_ISPR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 ICSR register has bits 31-1 fixed to 0. 
     * Over-ride the immediate ICSR write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICSR register
     */
    void write_ICSR(unsigned int value);
    /**
     * \brief The rh850g4 ICSR register has bits 31-1 fixed to 0. 
     * Over-ride the generative ICSR write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICSR register
     */
    void gen_write_ICSR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 INTCFG register has bits 31-1 fixed to 0. 
     * Over-ride the immediate INTCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 INTCFG register
     */
    void write_INTCFG(unsigned int value);
    /**
     * \brief The rh850g4 INTCFG register has bits 31-1 fixed to 0. 
     * Over-ride the generative INTCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 INTCFG register
     */
    void gen_write_INTCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 INTCFG register has the guest mode register.
     * Over-ride the generative INTCFG (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 INTCFG register
     */
    void write_INTCFG_ISPC(unsigned char value);
    void write_INTCFG_EPL(unsigned char value);
    void write_INTCFG_ULNR(unsigned char value);
    /**
     * \brief The rh850g4 INTCFG register has the guest mode register.
     * Over-ride the generative INTCFG (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 INTCFG register
     */
    void gen_write_INTCFG_ISPC(fastiss::i1_t *value);
    void gen_write_INTCFG_EPL(fastiss::i1_t *value);
    void gen_write_INTCFG_ULNR(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 PLMR register has bits 31-5 fixed to 0. 
     * Over-ride the immediate  PLMRwrite function to ensure this.
     * \param value New unmasked value of the rh850g4 PLMR register
     */
    void write_PLMR(unsigned int value);
    /**
     * \brief The rh850g4 PLMR register has bits 31-5 fixed to 0. 
     * Over-ride the generative PLMR write function to ensure this.
     * \param value New unmasked value of the rh850g4 PLMR register
     */
    void gen_write_PLMR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 PLMR register has the guest mode register.
     * Over-ride the generative PLMR (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 PLMR register
     */
    void write_PLMR_PLM(unsigned char value);
    /**
     * \brief The rh850g4 PLMR register has the guest mode register.
     * Over-ride the generative PLMR (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 PLMR register
     */
    void gen_write_PLMR_PLM(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 RBCR0 register has bits 31-17 fixed to 0. 
     * Over-ride the immediate RBCR0 write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBCR0 register
     */
    void write_RBCR0(unsigned int value);
    /**
     * \brief The rh850g4 RBCR0 register has bits 31-17 fixed to 0. 
     * Over-ride the generative RBCR0 write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBCR0 register
     */
    void gen_write_RBCR0(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 RBCR1 register has bits 31-16 fixed to 0. 
     * Over-ride the immediate RBCR1 write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBCR1 register
     */
    void write_RBCR1(unsigned int value);
    /**
     * \brief The rh850g4 RBCR1 register has bits 31-16 fixed to 0. 
     * Over-ride the generative RBCR1 write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBCR1 register
     */
    void gen_write_RBCR1(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 RBNR register has bits 31-5 fixed to 0. 
     * Over-ride the immediate RBNR write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBNR register
     */
    void write_RBNR(unsigned int value);
    /**
     * \brief The rh850g4 RBNR register has bits 31-5 fixed to 0. 
     * Over-ride the generative RBNR write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBNR register
     */
    void gen_write_RBNR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 RBIP register has bits 3-0 fixed to 0. 
     * Over-ride the immediate RBIP write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBIP register
     */
    void write_RBIP(unsigned int value);
    /**
     * \brief The rh850g4 RBIP register has bits 3-0 fixed to 0. 
     * Over-ride the generative RBIP write function to ensure this.
     * \param value New unmasked value of the rh850g4 RBIP register
     */
    void gen_write_RBIP(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 ICTAGL register has bits 11-6, 3 and 1 fixed to 0.  
     * Over-ride the immediate ICTAGL write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICTAGL register
     */
    void write_ICTAGL(unsigned int value);
    /**
     * \brief The rh850g4 ICTAGL register has bits 11-6, 3 and 1 fixed to 0.
     * Over-ride the generative ICTAGL write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICTAGL register
     */
    void gen_write_ICTAGL(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 ICTAGH register has bits 27-24 and 7-0 fixed to 0.  
     * Over-ride the immediate ICTAGH write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICTAGH register
     */
    void write_ICTAGH(unsigned int value);
    /**
     * \brief The rh850g4 ICTAGH register has bits 27-24 and 7-0  fixed to 0.
     * Over-ride the generative ICTAGH write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICTAGH register
     */
    void gen_write_ICTAGH(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 ICCTRL register has bits 31-17, 15-9 and 7-3 fixed to 0.  
     * Over-ride the immediate ICCTRL write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICCTRL register
     */
    void write_ICCTRL(unsigned int value);
    /**
     * \brief The rh850g4 ICCTRL register has bits 31-7, 15-9 and 7-3 fixed to 0.
     * Over-ride the generative ICCTRL write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICCTRL register
     */
    void gen_write_ICCTRL(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 ICCFG register has bits 31-15 fixed to 0.  
     * Over-ride the immediate ICCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICCFG register
     */
    void write_ICCFG(unsigned int value);

    /**
     * \brief The rh850g4 ICCFG register has bits 31-15 fixed to 0.
     * Over-ride the generative ICCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICCFG register
     */
    void gen_write_ICCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 ICERR register has bits 30, 23-22 and 1 fixed to 0.
     * Over-ride the immediate ICERR write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICERR register
     */
    void write_ICERR(unsigned int value);
    /**
     * \brief The rh850g4 ICERR register has bits 30, 23-22 and 1 fixed to 0.
     * Over-ride the generative ICERR write function to ensure this.
     * \param value New unmasked value of the rh850g4 ICEER register
     */
    void gen_write_ICERR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPM register has bits 31-2 fixed to 0.  
     * Over-ride the immediate MPM write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPM register
     */
    void write_MPM(unsigned int value);
    /**
     * \brief The rh850g4 MPM register has bits 31-2 fixed to 0.
     * Over-ride the generative MPM write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPM register
     */
    void gen_write_MPM(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPCFG register has bits 31-20 and 15-5 fixed to 0.  
     * Over-ride the immediate MPCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPCFG register
     */
    void write_MPCFG(unsigned int value);
    /**
     * \brief The rh850g4 MPCFG register has bits 31-20 and 15-5 fixed to 0.
     * Over-ride the generative MPCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPCFG register
     */
    void gen_write_MPCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MCC register always holds 0 and it's value should never
     * be changed. Over-ride the immediate MCC write function to ensure this.
     * \param value This value is ignored
     */
    void write_MCC(unsigned int value);
    /**
     * \brief The rh850g4 MCC register always holds 0 and it's value should never
     * be changed. Over-ride the generative MCC write function to ensure this.
     * \param value This value is ignored
     */
    void gen_write_MCC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MCC register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the immediate MCC read function
     * to ensure this.
     * \return A constant value of zero
     */
    unsigned int read_MCC(void);
    /**
     * \brief The rh850g4 MCC register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the generative MCC read function
     * to ensure this.
     * \return A 32-bit constant value of zero
     */
    fastiss::i32_t *gen_read_MCC(void);

    /**
     * \brief The rh850g4 MCR register has bits 31-9 and 7-6 fixed to 0.  
     * Over-ride the immediate MCR write function to ensure this.
     * \param value New unmasked value of the rh850g4 MCR register
     */
    void write_MCR(unsigned int value);
    /**
     * \brief The rh850g4 MCR register has bits 31-9 and 7-6 fixed to 0.
     * Over-ride the generative MCR write function to ensure this.
     * \param value New unmasked value of the rh850g4 MCR register
     */
    void gen_write_MCR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MCI register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MCI write function to ensure this.
     * \param value New unmasked value of the rh850g4 MCI register
     */
    void write_MCI(unsigned int value);
    /**
     * \brief The rh850g4 MCR register has bits 31-5 fixed to 0.
     * Over-ride the generative MCI write function to ensure this.
     * \param value New unmasked value of the rh850g4 MCI register
     */
    void gen_write_MCI(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPIDX register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPIDX write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPIDX register
     */
    void write_MPIDX(unsigned int value);
    /**
     * \brief The rh850g4 MPIDX register has bits 31-5 fixed to 0.
     * Over-ride the generative MPIDX write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPIDX register
     */
    void gen_write_MPIDX(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPLA register has bits 1-0 fixed to 0.  
     * Over-ride the immediate MPLA write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPLA register
     */
    void write_MPLA(unsigned int value);
    /**
     * \brief The rh850g4 MPLA register has bits 1-0 fixed to 0.
     * Over-ride the generative MPLA write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPLA register
     */
    void gen_write_MPLA(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPUA register has bits 1-0 fixed to 0.  
     * Over-ride the immediate MPUA write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPUA register
     */
    void write_MPUA(unsigned int value);
    /**
     * \brief The rh850g4 MPUA register has bits 1-0 fixed to 0.
     * Over-ride the generative MPUA write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPUA register
     */
    void gen_write_MPUA(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID0 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID0 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID0 register
     */
    void write_MPID0(unsigned int value);
    /**
     * \brief The rh850g4 MPID0 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID0 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID0 register
     */
    void gen_write_MPID0(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID1 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID1 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID1 register
     */
    void write_MPID1(unsigned int value);
    /**
     * \brief The rh850g4 MPID1 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID1 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID1 register
     */
    void gen_write_MPID1(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID2 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID2 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID2 register
     */
    void write_MPID2(unsigned int value);
    /**
     * \brief The rh850g4 MPID2 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID2 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID2 register
     */
    void gen_write_MPID2(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID3 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID3 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID3 register
     */
    void write_MPID3(unsigned int value);
    /**
     * \brief The rh850g4 MPID3 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID3 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID3 register
     */
    void gen_write_MPID3(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID4 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID4 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID4 register
     */
    void write_MPID4(unsigned int value);
    /**
     * \brief The rh850g4 MPID4 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID4 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID4 register
     */
    void gen_write_MPID4(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID5 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID5 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID5 register
     */
    void write_MPID5(unsigned int value);
    /**
     * \brief The rh850g4 MPID5 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID5 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID5 register
     */
    void gen_write_MPID5(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID6 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID6 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID6 register
     */
    void write_MPID6(unsigned int value);
    /**
     * \brief The rh850g4 MPID6 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID6 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID6 register
     */
    void gen_write_MPID6(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 MPID7 register has bits 31-5 fixed to 0.  
     * Over-ride the immediate MPID7 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID7 register
     */
    void write_MPID7(unsigned int value);
    /**
     * \brief The rh850g4 MPID7 register has bits 31-5 fixed to 0.
     * Over-ride the generative MPID7 write function to ensure this.
     * \param value New unmasked value of the rh850g4 MPID7 register
     */
    void gen_write_MPID7(fastiss::i32_t *value);

     /**
     * \brief The rh850g4 FXSR register has bits 31-24, 20 and 16 fixed to 0.  
     * Over-ride the immediate FXSR write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXSR register
     */
    void write_FXSR(unsigned int value);
    /**
     * \brief The rh850g4 FXSR register has bits 31-24, 20 and 16 fixed to 0.
     * Over-ride the generative FXSR write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXSR register
     */
    void gen_write_FXSR(fastiss::i32_t *value);

     /**
     * \brief The rh850g4 FXST register has bits 31-14 and 7-6 fixed to 0.  
     * Over-ride the immediate FXST write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXST register
     */
    void write_FXST(unsigned int value);
    /**
     * \brief The rh850g4 FXST register has bits 31-14 and 7-6 fixed to 0.
     * Over-ride the generative FXST write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXST register
     */
    void gen_write_FXST(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 FXINFO register has bits 31-2 fixed to 0.  
     * Over-ride the immediate FXINFO write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXINFO register
     */
    void write_FXINFO(unsigned int value);
    /**
     * \brief The rh850g4 FXINFO register has bits 31-2 fixed to 0.
     * Over-ride the generative FXINFO write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXINFO register
     */
    void gen_write_FXINFO(fastiss::i32_t *value);

     /**
     * \brief The rh850g4 FXCFG register has bits 31-10 and 7-6 fixed to 0.  
     * Over-ride the immediate FXCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXCFG register
     */
    void write_FXCFG(unsigned int value);
    /**
     * \brief The rh850g4 FXCFG register has bits 31-10 and 7-6 fixed to 0.
     * Over-ride the generative FXCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXCFG register
     */
    void gen_write_FXCFG(fastiss::i32_t *value);

     /**
     * \brief The rh850g4 FXXC register has bits 31-30, 23-22, 15-14 and 7-6 fixed to 0.  
     * Over-ride the immediate FXXC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXXC register
     */
    void write_FXXC(unsigned int value);
    /**
     * \brief The rh850g4 FXXC register has bits 31-30, 23-22, 15-14 and 7-6 fixed to 0.
     * Over-ride the generative FXXC write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXXC register
     */
    void gen_write_FXXC(fastiss::i32_t *value);

     /**
     * \brief The rh850g4 FXXP register has bits 31-29, 23-21, 15-13 and 7-5 fixed to 0.  
     * Over-ride the immediate FXXP write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXXP register
     */
    void write_FXXP(unsigned int value);
    /**
     * \brief The rh850g4 FXXP register has bits 31-29, 23-21, 15-13 and 7-5 fixed to 0.
     * Over-ride the generative FXXP write function to ensure this.
     * \param value New unmasked value of the rh850g4 FXXP register
     */
    void gen_write_FXXP(fastiss::i32_t *value);

    /**
     * \brief Writing to the rh850g4 FXSR register IF, RM, XC, XE or XP fields
     * also writes the values to the corresponding fields in the FXST,
     * and FXCFG registers. Over-ride the immediate FXSR write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FXSR register
     */
    void write_FXSR_IF(unsigned char value);
    void write_FXSR_RM(unsigned char value);
    void write_FXSR_XC(unsigned char value);
    void write_FXSR_XE(unsigned char value);
    void write_FXSR_XP(unsigned char value);
    /**
     * \brief Writing to the rh850g4 FXSR register IF, RM, XC, XE or XP fields
     * also writes the values to the corresponding fields in the FXST,
     * and FXCFG registers. Over-ride the generative FXSR write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FPSR register
     */
    void gen_write_FXSR_IF(fastiss::i1_t *value);
    void gen_write_FXSR_RM(fastiss::i8_t *value);
    void gen_write_FXSR_XC(fastiss::i8_t *value);
    void gen_write_FXSR_XE(fastiss::i8_t *value);
    void gen_write_FXSR_XP(fastiss::i8_t *value);

    /**
     * \brief Writing to the rh850g4 FXST register IF, XC or XP fields
     * also writes the values to the corresponding fields in the FXSR
     * registers. Over-ride the immediate FXST write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FXST register
     */
    void write_FXST_IF(unsigned char value);
    void write_FXST_XC(unsigned char value);
    void write_FXST_XP(unsigned char value);
    /**
     * \brief Writing to the rh850g4 FXST register IF, XC or XP fields
     * also writes the values to the corresponding fields in the FXST,
     * registers. Over-ride the generative FXST write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FXST register
     */
    void gen_write_FXST_IF(fastiss::i1_t *value);
    void gen_write_FXST_XC(fastiss::i8_t *value);
    void gen_write_FXST_XP(fastiss::i8_t *value);

    /**
     * \brief Writing to the rh850g4 FXCFG register RM or XE fields
     * also writes the values to the corresponding fields in the FXSR
     * registers. Over-ride the immediate FXCFG write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FXCFG register
     */
    void write_FXCFG_RM(unsigned char value);
    void write_FXCFG_XE(unsigned char value);
    /**
     * \brief Writing to the rh850g4 FXCFG register RM or XE fields
     * also writes the values to the corresponding fields in the FXST,
     * registers. Over-ride the generative FXCFG write function to ensure
     * this.
     * \param value New unmasked value of the rh850g4 FXCFG register
     */
    void gen_write_FXCFG_RM(fastiss::i8_t *value);
    void gen_write_FXCFG_XE(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 DBGEN register has bits 31-9 fixed to 0. Over-ride
     * the immediate DBGEN write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBGEN register
     */
    void write_DBGEN(unsigned int value);
    /**
     * \brief The rh850g4 DBGEN register has bits 31-0-9 fixed to 0. Over-ride
     * the generative DBGEN write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBGEN register
     */
    void gen_write_DBGEN(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DBPSWH register has bits 30-11 and 7-0 fixed to 0.
     *  Over-ride the immediate DBPSWH write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBPSWH register
     */
    void write_DBPSWH(unsigned int value);

    /**
     * \brief The rh850g4 DBPSWH register has bits 30-11 and 7-0 fixed to 0.
     *  Over-ride the immediate DBPSWH write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBPSWH register
     */
    void gen_write_DBPSWH(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DBPC register has bits 0 fixed to 0. Over-ride
     * the immediate DBPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBPC register
     */
    void write_DBPC(unsigned int value);
    /**
     * \brief The rh850g4 DBPC register has bits 0 fixed to 0. Over-ride
     * the generative DBPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBPC register
     */
    void gen_write_DBPC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DBPSW register has bits 31, 29-19 and 14-8 fixed to 0. 
     * Over-ride the immediate DBPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBPSW register
     */
    void write_DBPSW(unsigned int value);
    /**
     * \brief The rh850g4 DBPSW register has bits 31, 29-19 and 14-8 fixed to 0. 
     * Over-ride the generative DBPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBPSW register
     */
    void gen_write_DBPSW(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DIR0 register has bits 31-10 and 7-3 fixed to 0. 
     * Over-ride the immediate DIR0 write function to ensure this.
     * \param value New unmasked value of the rh850g4 DIR0 register
     */
    void write_DIR0(unsigned int value);
    /**
     * \brief The rh850g4 DIR0 register has bits 31-10 and 7-3 fixed to 0.
     * Over-ride the generative DIR0 write function to ensure this.
     * \param value New unmasked value of the rh850g4 DIR0 register
     */
    void gen_write_DIR0(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DIR1 register has bits 31-20 and 3 fixed to 0. 
     * Over-ride the immediate DIR1 write function to ensure this.
     * \param value New unmasked value of the rh850g4 DIR1 register
     */
    void write_DIR1(unsigned int value);
    /**
     * \brief The rh850g4 DIR1 register has bits 31-20 and 3 fixed to 0.
     * Over-ride the generative DIR1 write function to ensure this.
     * \param value New unmasked value of the rh850g4 DIR1 register
     */
    void gen_write_DIR1(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 BPC register has bits 31-13, 9 and 7-6 fixed to 0. 
     * Over-ride the immediate BPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 BPC register
     */
    void write_BPC(unsigned int value);
    /**
     * \brief The rh850g4 BPC register has bits 31-13, 9 and 7-6 fixed to 0.
     * Over-ride the generative BPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 BPC register
     */
    void gen_write_BPC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DBCFG register has bits 31-4 fixed to 0. 
     * Over-ride the immediate DBCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBCFG register
     */
    void write_DBCFG(unsigned int value);
    /**
     * \brief The rh850g4  DBCFGregister has bits 31-4 fixed to 0.
     * Over-ride the generative DBCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 DBCFG register
     */
    void gen_write_DBCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 LSCFG register has bits 31-1 fixed to 0.
     * Over-ride the immediate LSCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 LSCFG register
     */
    void write_LSCFG(unsigned int value);
    /**
     * \brief The rh850g4 LSCFG register has bits 31-30 and 7-4 fixed to 0.
     * Over-ride the generative LSCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 LSCFG register
     */
    void gen_write_LSCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 L1RCFG register has bits 31-30 and 7-4 fixed to 0.
     * Over-ride the immediate L1RCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 L1RCFG register
     */
    void write_L1RCFG(unsigned int value);
    /**
     * \brief The rh850g4 L1RCFG register has bits 31-30 and 7-4 fixed to 0.
     * Over-ride the generative L1RCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 L1RCFG register
     */
    void gen_write_L1RCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DECFG register has bits 31-3 fixed to 0.
     * Over-ride the immediate DECFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 DECFG register
     */
    void write_DECFG(unsigned int value);
    /**
     * \brief The rh850g4 DECFG register has bits 31-3 fixed to 0.
     * Over-ride the generative DECFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 DECFG register
     */
    void gen_write_DECFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DECTRL register has bits 31-1 fixed to 0.
     * Over-ride the immediate DECTRL write function to ensure this.
     * \param value New unmasked value of the rh850g4 DECTRL register
     */
    void write_DECTRL(unsigned int value);
    /**
     * \brief The rh850g4 DECTRL register has bits 31-1 fixed to 0.
     * Over-ride the generative DECTRL write function to ensure this.
     * \param value New unmasked value of the rh850g4 DECTRL register
     */
    void gen_write_DECTRL(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 DEVDS register has bits 31-17, 15-8 fixed to 0.
     * Over-ride the immediate DEVDS write function to ensure this.
     * \param value New unmasked value of the rh850g4 DEVDS register
     */
    void write_DEVDS(unsigned int value);
    /**
     * \brief The rh850g4 DEVDS register has bits 31-17, 15-8 fixed to 0.
     * Over-ride the generative DEVDS write function to ensure this.
     * \param value New unmasked value of the rh850g4 DEVDS register
     */
    void gen_write_DEVDS(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMEIPC register has bits 0 fixed to 0. Over-ride
     * the immediate GMEIPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMEIPC register
     */
    void write_GMEIPC(unsigned int value);
    /**
     * \brief The rh850g4 GMEIPC register has bits 0 fixed to 0. Over-ride
     * the generative GMEIPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMEIPC register
     */
    void gen_write_GMEIPC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMEIPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the immediate GMEIPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMEIPSW register
     */
    void write_GMEIPSW(unsigned int value);
    /**
     * \brief The rh850g4 GMEIPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative GMEIPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMEIPSW register
     */
    void gen_write_GMEIPSW(fastiss::i32_t *value);
	
    /**
     * \brief The rh850g4 GMFEPC register has bits 0 fixed to 0. Over-ride
     * the immediate GMFEPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMFEPC register
     */
    void write_GMFEPC(unsigned int value);
    /**
     * \brief The rh850g4 GMFEPC register has bits 0 fixed to 0. Over-ride
     * the generative GMFEPC write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMFEPC register
     */
    void gen_write_GMFEPC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMFEPSW register has bits 31, 29-20, 14-12, and 8 fixed to 0.
     * Over-ride the immediate GMFEPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMFEPSW register
     */
    void write_GMFEPSW(unsigned int value);
    /**
     * \brief The rh850g4 GMFEPSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative GMFEPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMFEPSW register
     */
    void gen_write_GMFEPSW(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMPSW register has bits 31, 29-28, 19, 14-8 fixed to 0.  
     * Over-ride the immediate GMPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPSW register
     */
    void write_GMPSW(unsigned int value);
    /**
     * \brief The rh850g4 GMPSW register has bits 31, 29-28, 19, 14-8 fixed to 0.  
     * Over-ride the generative GMPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPSW register
     */
    void gen_write_GMPSW(fastiss::i32_t *value);
	
    /**
     * \brief The rh850g4 GMPSW register has the guest mode register.
     * Over-ride the generative GMPSW (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPSW register
     */
    void write_GMPSW_CU0(unsigned char value);
    void write_GMPSW_CU1(unsigned char value);
    void write_GMPSW_CU2(unsigned char value);
    void write_GMPSW_EBV(unsigned char value);
    void write_GMPSW_EIMASK(unsigned char value);
     /**
     * \brief The rh850g4 GMPSW register has the guest mode register.
     * Over-ride the generative GMPSW (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPSW register
     */
    void gen_write_GMPSW_EBV(fastiss::i1_t *value);
    void gen_write_GMPSW_CU0(fastiss::i1_t *value);
    void gen_write_GMPSW_CU1(fastiss::i1_t *value);
    void gen_write_GMPSW_CU2(fastiss::i1_t *value);
    void gen_write_GMPSW_EIMASK(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 GMMEA register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * Over-ride the immediate GMMEA write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMMEA register
     */
    void write_GMMEA(unsigned int value);
    /**
     * \brief The rh850g4 GMMEA register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * \param value New unmasked value of the rh850g4 GMMEA register
     */
    void gen_write_GMMEA(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMMEI register has bits 27-21, 15-12, and 7-6 fixed to 0.
     * Over-ride the immediate GMMEI write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMMEI register
     */
    void write_GMMEI(unsigned int value);
    /**
     * \brief The rh850g4 GMMEI register has bits 27-21, 15-12, and 7-6 fixed to 0.
     * Over-ride the generative GMMEI write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMMEI register
     */
    void gen_write_GMMEI(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMEIIC register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * Over-ride the immediate GMEIIC write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMEIIC register
     */
    void write_GMEIIC(unsigned int value);
    /**
     * \brief The rh850g4 GMEIIC register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * \param value New unmasked value of the rh850g4 GMEIIC register
     */
    void gen_write_GMEIIC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMFEIC register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * Over-ride the immediate GMFEIC write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMFEIC register
     */
    void write_GMFEIC(unsigned int value);
    /**
     * \brief The rh850g4 GMFEIC register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * \param value New unmasked value of the rh850g4 GMFEIC register
     */
    void gen_write_GMFEIC(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMSPID register has bits 31-5 fixed to 0.
     * Over-ride the immediate GMSPID write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMSPID register
     */
    void write_GMSPID(unsigned int value);
    /**
     * \brief The rh850g4 GMSPID register has bits 31-5 fixed to 0.
     * Over-ride the generative GMSPID write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMSPID register
     */
    void gen_write_GMSPID(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMSPIDLIST register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * Over-ride the immediate GMSPIDLIST write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMSPIDLIST register
     */
    void write_GMSPIDLIST(unsigned int value);
    /**
     * \brief The rh850g4 GMSPIDLIST register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * \param value New unmasked value of the rh850g4 GMSPIDLIST register
     */
    void gen_write_GMSPIDLIST(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMEBASE register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * Over-ride the immediate GMEBASE write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMEBASE register
     */
    void write_GMEBASE(unsigned int value);
    /**
     * \brief The rh850g4 GMEBASE register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * \param value New unmasked value of the rh850g4 GMEBASE register
     */
    void gen_write_GMEBASE(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMINTBP register has bits 8-0 fixed to 0. Over-ride
     * the immediate GMINTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMINTBP register
     */
    void write_GMINTBP(unsigned int value);
    /**
     * \brief The rh850g4 GMINTBP register has bits 8-0 fixed to 0. Over-ride
     * the generative GMINTBP write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMINTBP register
     */
    void gen_write_GMINTBP(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMINTCFG register has bits 31-1 fixed to 0. 
     * Over-ride the immediate GMINTCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMINTCFG register
     */
    void write_GMINTCFG(unsigned int value);
    /**
     * \brief The rh850g4 GMINTCFG register has bits 31-1 fixed to 0. 
     * Over-ride the generative GMINTCFG write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMINTCFG register
     */
    void gen_write_GMINTCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMINTCFG register has the guest mode register.
     * Over-ride the generative GMINTCFG (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMINTCFG register
     */
    void write_GMINTCFG_ISPC(unsigned char value);
    void write_GMINTCFG_EPL(unsigned char value);
    void write_GMINTCFG_ULNR(unsigned char value);
    /**
     * \brief The rh850g4 GMINTCFG register has the guest mode register.
     * Over-ride the generative GMINTCFG (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMINTCFG register
     */
    void gen_write_GMINTCFG_ISPC(fastiss::i1_t *value);
    void gen_write_GMINTCFG_EPL(fastiss::i1_t *value);
    void gen_write_GMINTCFG_ULNR(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 GMPLMR register has bits 31-5 fixed to 0. 
     * Over-ride the immediate  GMPLMRwrite function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPLMR register
     */
    void write_GMPLMR(unsigned int value);
    /**
     * \brief The rh850g4 GMPLMR register has bits 31-5 fixed to 0. 
     * Over-ride the generative GMPLMR write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPLMR register
     */
    void gen_write_GMPLMR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMPLMR register has the guest mode register.
     * Over-ride the generative GMPLMR (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPLMR register
     */
    void write_GMPLMR_PLM(unsigned char value);
    /**
     * \brief The rh850g4 GMPLMR register has the guest mode register.
     * Over-ride the generative GMPLMR (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPLMR register
     */
    void gen_write_GMPLMR_PLM(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 GMSVLOCK register has bits 31-1 fixed to 0.
     * Over-ride the immediate GMSVLOCK write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMSVLOCK register
     */
    void write_GMSVLOCK(unsigned int value);
    /**
     * \brief The rh850g4 GMSVLOCK register has bits 31-1 fixed to 0.
     * Over-ride the immediate GMSVLOCK write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMSVLOCK register
     */
    void gen_write_GMSVLOCK(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMMPM register has bits 31-3 fixed to 0.
     * Over-ride the immediate GMMPM write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMMPM register
     */
    void write_GMMPM(unsigned int value);
    /**
     * \brief The rh850g4 GMMPM register has bits 31-3 fixed to 0.
     * Over-ride the immediate GMMPM write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMMPM register
     */
    void gen_write_GMMPM(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMEIWR register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * Over-ride the immediate GMEIWR write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMEIWR register
     */
    void write_GMEIWR(unsigned int value);
    /**
     * \brief The rh850g4 GMEIWR register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * \param value New unmasked value of the rh850g4 GMEIWR register
     */
    void gen_write_GMEIWR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMFEWR register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * Over-ride the immediate GMFEWR write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMFEWR register
     */
    void write_GMFEWR(unsigned int value);
    /**
     * \brief The rh850g4 GMFEWR register has bits 31-0 fixed to 0,
     * when HVCFG.HVE is 0.
     * \param value New unmasked value of the rh850g4 GMFEWR register
     */
    void gen_write_GMFEWR(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 GMPEID register has bits 31-5 fixed to 0.
     * Over-ride the immediate GMPEID write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPEID register
     */
    void write_GMPEID(unsigned int value);
    /**
     * \brief The rh850g4 GMPEID register has bits 31-5 fixed to 0.
     * Over-ride the generative GMPEID write function to ensure this.
     * \param value New unmasked value of the rh850g4 GMPEID register
     */
    void gen_write_GMPEID(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 PSWH register has bits 30-11, 7-0 fixed to 0. 
     * Over-ride the immediate PSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void write_PSWH(unsigned int value);
    /**
     * \brief The rh850g4 PSWH register has bits 30-11, 7-0 fixed to 0. 
     * Over-ride the generative PSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void gen_write_PSWH(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 PSWH register has the guest mode register.
     * Over-ride the generative PSWH (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSWH register
     */
    void write_PSWH_GM(unsigned char value);
    void write_PSWH_GPID(unsigned char value);

    /**
     * \brief The rh850g4 PSWH register has the guest mode register.
     * Over-ride the generative PSWH (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSWH register
     */
    void gen_write_PSWH_GM(fastiss::i1_t *value);
    void gen_write_PSWH_GPID(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 EIPSWH register has bits 30-11, 7-0 fixed to 0. 
     * Over-ride the immediate EIPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPSW register
     */
    void write_EIPSWH(unsigned int value);
    /**
     * \brief The rh850g4 EIPSWH register has bits 30-11, 7-0 fixed to 0. 
     * Over-ride the generative EIPSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPSW register
     */
    void gen_write_EIPSWH(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 EIPSWH register has the guest mode register.
     * Over-ride the generative EIPSWH (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPSWH register
     */
    void write_EIPSWH_GM(unsigned char value);
    void write_EIPSWH_GPID(unsigned char value);

    /**
     * \brief The rh850g4 EIPSWH register has the guest mode register.
     * Over-ride the generative EIPSWH (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 EIPSWH register
     */
    void gen_write_EIPSWH_GM(fastiss::i1_t *value);
    void gen_write_EIPSWH_GPID(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 FEPSWH register has bits 30-11, 7-0 fixed to 0. 
     * Over-ride the immediate PSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void write_FEPSWH(unsigned int value);
    /**
     * \brief The rh850g4 FEPSWH register has bits 30-11, 7-0 fixed to 0. 
     * Over-ride the generative PSW write function to ensure this.
     * \param value New unmasked value of the rh850g4 PSW register
     */
    void gen_write_FEPSWH(fastiss::i32_t *value);

    /**
     * \brief The rh850g4 FEPSWH register has the guest mode register.
     * Over-ride the generative FEPSWH (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEPSWH register
     */
    void write_FEPSWH_GM(unsigned char value);
    void write_FEPSWH_GPID(unsigned char value);

    /**
     * \brief The rh850g4 FEPSWH register has the guest mode register.
     * Over-ride the generative FEPSWH (each bit) write function to ensure this.
     * \param value New unmasked value of the rh850g4 FEPSWH register
     */
    void gen_write_FEPSWH_GM(fastiss::i1_t *value);
    void gen_write_FEPSWH_GPID(fastiss::i8_t *value);

    /**
     * \brief The rh850g4 EIPC register has the guest mode register.
     * Over-ride the generative EIPC read function to ensure this.
     */
    unsigned int read_EIPC(void);
    /**
     * \brief The rh850g4 EIPC register has the guest mode register.
     * Over-ride the generative EIPC read function to ensure this.
     */
    fastiss::i32_t *gen_read_EIPC(void);

    /**
     * \brief The rh850g4 EIPSW register has the guest mode register.
     * Over-ride the generative EIPSW read function to ensure this.
     */
    unsigned int read_EIPSW(void);
    /**
     * \brief The rh850g4 EIPSW register has the guest mode register.
     * Over-ride the generative EIPSW read function to ensure this.
     */
    fastiss::i32_t *gen_read_EIPSW(void);

    /**
     * \brief The rh850g4 FEPC register has the guest mode register.
     * Over-ride the generative FEPC read function to ensure this.
     */
    unsigned int read_FEPC(void);
    /**
     * \brief The rh850g4 FEPC register has the guest mode register.
     * Over-ride the generative FEPC read function to ensure this.
     */
    fastiss::i32_t *gen_read_FEPC(void);

    /**
     * \brief The rh850g4 FEPSW register has the guest mode register.
     * Over-ride the generative FEPSW read function to ensure this.
     */
    unsigned int read_FEPSW(void);
    /**
     * \brief The rh850g4 FEPSW register has the guest mode register.
     * Over-ride the generative FEPSW read function to ensure this.
     */
    fastiss::i32_t *gen_read_FEPSW(void);

    /**
     * \brief The rh850g4 PSW register has the guest mode register.
     * Over-ride the generative PSW read function to ensure this.
     */
    unsigned int read_PSW(void);
    /**
     * \brief The rh850g4 PSW register has the guest mode register.
     * Over-ride the generative PSW read function to ensure this.
     */
    fastiss::i32_t *gen_read_PSW(void);

    /**
     * \brief The rh850g4 PSW register has the guest mode register.
     * Over-ride the generative PSW (each bit) read function to ensure this.
     */
    unsigned char read_PSW_Z(void);
    unsigned char read_PSW_S(void);
    unsigned char read_PSW_OV(void);
    unsigned char read_PSW_CY(void);
    unsigned char read_PSW_SAT(void);
    unsigned char read_PSW_ID(void);
    unsigned char read_PSW_EP(void);
    unsigned char read_PSW_NP(void);
    unsigned char read_PSW_EBV(void);
    unsigned char read_PSW_CU0(void);
    unsigned char read_PSW_CU1(void);
    unsigned char read_PSW_CU2(void);
    unsigned char read_PSW_EIMASK(void);
    unsigned char read_PSW_UM(void);
    /**
     * \brief The rh850g4 PSW register has the guest mode register.
     * Over-ride the generative PSW (each bit) read function to ensure this.
     */
    fastiss::i1_t *gen_read_PSW_Z(void);
    fastiss::i1_t *gen_read_PSW_S(void);
    fastiss::i1_t *gen_read_PSW_OV(void);
    fastiss::i1_t *gen_read_PSW_CY(void);
    fastiss::i1_t *gen_read_PSW_SAT(void);
    fastiss::i1_t *gen_read_PSW_ID(void);
    fastiss::i1_t *gen_read_PSW_EP(void);
    fastiss::i1_t *gen_read_PSW_NP(void);
    fastiss::i1_t *gen_read_PSW_EBV(void);
    fastiss::i1_t *gen_read_PSW_CU0(void);
    fastiss::i1_t *gen_read_PSW_CU1(void);
    fastiss::i1_t *gen_read_PSW_CU2(void);
    fastiss::i8_t *gen_read_PSW_EIMASK(void);
    fastiss::i1_t *gen_read_PSW_UM(void);

    /**
     * \brief The rh850g4 EIIC register has the guest mode register.
     * Over-ride the generative EIIC read function to ensure this.
     */
    unsigned int read_EIIC(void);
    /**
     * \brief The rh850g4 EIIC register has the guest mode register.
     * Over-ride the generative EIIC read function to ensure this.
     */
    fastiss::i32_t *gen_read_EIIC(void);

    /**
     * \brief The rh850g4 FEIC register has the guest mode register.
     * Over-ride the generative FEIC read function to ensure this.
     */
    unsigned int read_FEIC(void);
    /**
     * \brief The rh850g4 FEIC register has the guest mode register.
     * Over-ride the generative FEIC read function to ensure this.
     */
    fastiss::i32_t *gen_read_FEIC(void);

    /**
     * \brief The rh850g4 EIWR register has the guest mode register.
     * Over-ride the generative EIWR read function to ensure this.
     */
    unsigned int read_EIWR(void);
    /**
     * \brief The rh850g4 EIWR register has the guest mode register.
     * Over-ride the generative EIWR read function to ensure this.
     */
    fastiss::i32_t *gen_read_EIWR(void);

    /**
     * \brief The rh850g4 FEWR register has the guest mode register.
     * Over-ride the generative FEWR read function to ensure this.
     */
    unsigned int read_FEWR(void);
    /**
     * \brief The rh850g4 FEWR register has the guest mode register.
     * Over-ride the generative FEWR read function to ensure this.
     */
    fastiss::i32_t *gen_read_FEWR(void);

    /**
     * \brief The rh850g4 SPID register has the guest mode register.
     * Over-ride the generative SPID read function to ensure this.
     */
    unsigned int read_SPID(void);
    /**
     * \brief The rh850g4 SPID register has the guest mode register.
     * Over-ride the generative SPID read function to ensure this.
     */
    fastiss::i32_t *gen_read_SPID(void);

    /**
     * \brief The rh850g4 SPIDLIST register has the guest mode register.
     * Over-ride the generative SPIDLIST read function to ensure this.
     */
    unsigned int read_SPIDLIST(void);
    /**
     * \brief The rh850g4 SPIDLIST register has the guest mode register.
     * Over-ride the generative SPIDLIST read function to ensure this.
     */
    fastiss::i32_t *gen_read_SPIDLIST(void);

    /**
     * \brief The rh850g4 EBASE register has the guest mode register.
     * Over-ride the generative EBASE read function to ensure this.
     */
    unsigned int read_EBASE(void);
    /**
     * \brief The rh850g4 EBASE register has the guest mode register.
     * Over-ride the generative EBASE read function to ensure this.
     */
    fastiss::i32_t *gen_read_EBASE(void);

    /**
     * \brief The rh850g4 EBASE register has the guest mode register.
     * Over-ride the generative EBASE (each bit) read function to ensure this.
     */
    unsigned int  read_EBASE_EBASE(void);
    unsigned char read_EBASE_DV(void);
    unsigned char read_EBASE_RINT(void);
    /**
     * \brief The rh850g4 EBASE register has the guest mode register.
     * Over-ride the generative EBASE (each bit) read function to ensure this.
     */
    fastiss::i32_t *gen_read_EBASE_EBASE(void);
    fastiss::i1_t  *gen_read_EBASE_DV(void);
    fastiss::i1_t  *gen_read_EBASE_RINT(void);

    /**
     * \brief The rh850g4 INTBP register has the guest mode register.
     * Over-ride the generative INTBP read function to ensure this.
     */
    unsigned int read_INTBP(void);
    /**
     * \brief The rh850g4 INTBP register has the guest mode register.
     * Over-ride the generative INTBP read function to ensure this.
     */
    fastiss::i32_t *gen_read_INTBP(void);

    /**
     * \brief The rh850g4 SVLOCK register has the guest mode register.
     * Over-ride the generative SVLOCK read function to ensure this.
     */
    unsigned int read_SVLOCK(void);
    /**
     * \brief The rh850g4 SVLOCK register has the guest mode register.
     * Over-ride the generative SVLOCK read function to ensure this.
     */
    fastiss::i32_t *gen_read_SVLOCK(void);

    /**
     * \brief The rh850g4 PEID register has the guest mode register.
     * Over-ride the generative PEID read function to ensure this.
     */
    unsigned int read_PEID(void);
    /**
     * \brief The rh850g4 PEID register has the guest mode register.
     * Over-ride the generative PEID read function to ensure this.
     */
    fastiss::i32_t *gen_read_PEID(void);

    /**
     * \brief The rh850g4 MEA register has the guest mode register.
     * Over-ride the generative MEA read function to ensure this.
     */
    unsigned int read_MEA(void);
    /**
     * \brief The rh850g4 MEA register has the guest mode register.
     * Over-ride the generative MEA read function to ensure this.
     */
    fastiss::i32_t *gen_read_MEA(void);

    /**
     * \brief The rh850g4 MEI register has the guest mode register.
     * Over-ride the generative MEI read function to ensure this.
     */
    unsigned int read_MEI(void);
    /**
     * \brief The rh850g4 MEI register has the guest mode register.
     * Over-ride the generative MEI read function to ensure this.
     */
    fastiss::i32_t *gen_read_MEI(void);

    /**
     * \brief The rh850g4 INTCFG register has the guest mode register.
     * Over-ride the generative INTCFG read function to ensure this.
     */
    unsigned int read_INTCFG(void);
    /**
     * \brief The rh850g4 INTCFG register has the guest mode register.
     * Over-ride the generative INTCFG read function to ensure this.
     */
    fastiss::i32_t *gen_read_INTCFG(void);

    /**
     * \brief The rh850g4 INTCFG register has the guest mode register.
     * Over-ride the generative INTCFG (each bit) read function to ensure this.
     */
    unsigned char read_INTCFG_ISPC(void);
    unsigned char read_INTCFG_EPL(void);
    unsigned char read_INTCFG_ULNR(void);
    /**
     * \brief The rh850g4 INTCFG register has the guest mode register.
     * Over-ride the generative INTCFG (each bit) read function to ensure this.
     */
    fastiss::i1_t *gen_read_INTCFG_ISPC(void);
    fastiss::i1_t *gen_read_INTCFG_EPL(void);
    fastiss::i8_t *gen_read_INTCFG_ULNR(void);

    /**
     * \brief The rh850g4 PLMR register has the guest mode register.
     * Over-ride the generative PLMR read function to ensure this.
     */
    unsigned int read_PLMR(void);
    /**
     * \brief The rh850g4 PLMR register has the guest mode register.
     * Over-ride the generative PLMR read function to ensure this.
     */
    fastiss::i32_t *gen_read_PLMR(void);

    /**
     * \brief The rh850g4 PLMR register has the guest mode register.
     * Over-ride the generative PLMR (each bit) read function to ensure this.
     */
    unsigned char read_PLMR_PLM(void);
    /**
     * \brief The rh850g4 PLMR register has the guest mode register.
     * Over-ride the generative PLMR (each bit) read function to ensure this.
     */
    fastiss::i8_t *gen_read_PLMR_PLM(void);

    /**
     * \brief The rh850g4 MPM register has the guest mode register.
     * Over-ride the generative MPM read function to ensure this.
     */
    unsigned int read_MPM(void);
    /**
     * \brief The rh850g4 MPM register has the guest mode register.
     * Over-ride the generative MPM read function to ensure this.
     */
    fastiss::i32_t *gen_read_MPM(void);

  };
}

#endif // _RH850G4_CUSTOM_CPU_STATE_IMPL_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
