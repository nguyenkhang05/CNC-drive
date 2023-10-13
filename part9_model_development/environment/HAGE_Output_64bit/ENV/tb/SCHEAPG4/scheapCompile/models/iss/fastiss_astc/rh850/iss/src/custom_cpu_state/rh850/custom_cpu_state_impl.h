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

/**
 * \file custom_cpu_state_impl.h
 * \brief Defines the customised CPU state implementation class
 */

#ifndef _RH850_CUSTOM_CPU_STATE_IMPL_H_
#define _RH850_CUSTOM_CPU_STATE_IMPL_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/cpu_state_impl.h"

//------------------------------------------------------------------------------
// rh850 custom CPU state definition
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief rh850 customised CPU state implementation. This class provides
   * customised implementations of the methods available in the CPU state
   * interface, allowing customised register specific write masks or behaviours.
   */
  class custom_cpu_state_impl_t : public rh850::cpu_state_impl_t
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
     * \brief Over-ride the name of the rh850 r3 register to be 'sp'
     * \return String representing the name of the rh850 r3 register
     */
    std::string register_name_r3(void);
    /**
     * \brief Over-ride the name of the rh850 r4 register to be 'gp'
     * \return String representing the name of the rh850 r4 register
     */
    std::string register_name_r4(void);
    /**
     * \brief Over-ride the name of the rh850 r5 register to be 'tp'
     * \return String representing the name of the rh850 r5 register
     */
    std::string register_name_r5(void);
    /**
     * \brief Over-ride the name of the rh850 r30 register to be 'ep'
     * \return String representing the name of the rh850 r30 register
     */
    std::string register_name_r30(void);
    /**
     * \brief Over-ride the name of the rh850 r31 register to be 'lp'
     * \return String representing the name of the rh850 r31 register
     */
    std::string register_name_r31(void);
    
    /**
     * \brief The rh850 r0 register always holds 0 and it's value should never
     * be changed. Over-ride the immediate r0 write function to ensure this.
     * \param value This value is ignored
     */
    void write_r0(unsigned int value);
    /**
     * \brief The rh850 r0 register always holds 0 and it's value should never
     * be changed. Over-ride the generative r0 write function to ensure this.
     * \param value This value is ignored
     */
    void gen_write_r0(fastiss::i32_t *value);

    /**
     * \brief The rh850 r0 register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the immediate r0 read function
     * to ensure this.
     * \return A constant value of zero
     */
    unsigned int read_r0(void);
    /**
     * \brief The rh850 r0 register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the generative r0 read function
     * to ensure this.
     * \return A 32-bit constant value of zero
     */
    fastiss::i32_t *gen_read_r0(void);
    
    /**
     * \brief The rh850 PC register has bits 0 fixed to 0. Over-ride
     * the immediate PC write function to ensure this.
     * \param value New unmasked value of the rh850 PC register
     */
    void write_PC(unsigned int value);
    /**
     * \brief The rh850 PC register has bits 0 fixed to 0. Over-ride
     * the generative PC write function to ensure this.
     * \param value New unmasked value of the rh850 PC register
     */
    void gen_write_PC(fastiss::i32_t *value);

    /**
     * \brief The rh850 EIPC register has bits 0 fixed to 0. Over-ride
     * the immediate EIPC write function to ensure this.
     * \param value New unmasked value of the rh850 EIPC register
     */
    void write_EIPC(unsigned int value);
    /**
     * \brief The rh850 EIPC register has bits 0 fixed to 0. Over-ride
     * the generative EIPC write function to ensure this.
     * \param value New unmasked value of the rh850 EIPC register
     */
    void gen_write_EIPC(fastiss::i32_t *value);

    /**
     * \brief The rh850 EIPSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the immediate EIPSW write function to ensure this.
     * \param value New unmasked value of the rh850 EIPSW register
     */
    void write_EIPSW(unsigned int value);
    /**
     * \brief The rh850 EIPSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative EIPSW write function to ensure this.
     * \param value New unmasked value of the rh850 EIPSW register
     */
    void gen_write_EIPSW(fastiss::i32_t *value);
	
    /**
     * \brief The rh850 FEPC register has bits 0 fixed to 0. Over-ride
     * the immediate FEPC write function to ensure this.
     * \param value New unmasked value of the rh850 FEPC register
     */
    void write_FEPC(unsigned int value);
    /**
     * \brief The rh850 FEPC register has bits 0 fixed to 0. Over-ride
     * the generative FEPC write function to ensure this.
     * \param value New unmasked value of the rh850 FEPC register
     */
    void gen_write_FEPC(fastiss::i32_t *value);

    /**
     * \brief The rh850 FEPSW register has bits 29-20, 14-12, and 8 fixed to 0.
     * Over-ride the immediate FEPSW write function to ensure this.
     * \param value New unmasked value of the rh850 FEPSW register
     */
    void write_FEPSW(unsigned int value);
    /**
     * \brief The rh850 FEPSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative FEPSW write function to ensure this.
     * \param value New unmasked value of the rh850 FEPSW register
     */
    void gen_write_FEPSW(fastiss::i32_t *value);

    /**
     * \brief The rh850 PSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the immediate PSW write function to ensure this.
     * \param value New unmasked value of the rh850 PSW register
     */
    void write_PSW(unsigned int value);
    /**
     * \brief The rh850 PSW register has bits 29-20, 14-12, and 8 fixed to 0. 
     * Over-ride the generative PSW write function to ensure this.
     * \param value New unmasked value of the rh850 PSW register
     */
    void gen_write_PSW(fastiss::i32_t *value);

    /**
     * \brief Writing to the rh850 FPSR register IF, XC, XP, CC, RM, or XE fields
     * also writes the values to the corresponding fields in the FPST, FPCC,
     * and FPCFG registers. Over-ride the immediate FPSR write function to ensure
     * this.
     * \param value New unmasked value of the rh850 FPSR register
     */
    void write_FPSR_IF(unsigned char value);
    void write_FPSR_XC(unsigned char value);
    void write_FPSR_XP(unsigned char value);
    void write_FPSR_CC(unsigned char value);
    void write_FPSR_RM(unsigned char value);
    void write_FPSR_XE(unsigned char value);
    /**
     * \brief Writing to the rh850 FPSR register IF, XC, XP, CC, RM, or XE fields
     * also writes the values to the corresponding fields in the FPST, FPCC,
     * and FPCFG registers. Over-ride the generative FPSR write function to ensure
     * this.
     * \param value New unmasked value of the rh850 FPSR register
     */
    void gen_write_FPSR_IF(fastiss::i1_t *value);
    void gen_write_FPSR_XC(fastiss::i8_t *value);
    void gen_write_FPSR_XP(fastiss::i8_t *value);
    void gen_write_FPSR_CC(fastiss::i8_t *value);
    void gen_write_FPSR_RM(fastiss::i8_t *value);
    void gen_write_FPSR_XE(fastiss::i8_t *value);

    /**
     * \brief The rh850 FPEPC register has bits 31-29 and 0 fixed to 0. Over-ride
     * the immediate FPEPC write function to ensure this.
     * \param value New unmasked value of the rh850 FPEPC register
     */
    void write_FPEPC(unsigned int value);
    /**
     * \brief The rh850 FPEPC register has bits 31-29 and 0 fixed to 0. Over-ride
     * the generative FPEPC write function to ensure this.
     * \param value New unmasked value of the rh850 FPEPC register
     */
    void gen_write_FPEPC(fastiss::i32_t *value);

    /**
     * \brief Writing to the rh850 FPST register IF, XC, or XP fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the immediate
     * FPST write function to ensure this.
     * \param value New unmasked value of the rh850 FPST register
     */
    void write_FPST_IF(unsigned char value);
    void write_FPST_XC(unsigned char value);
    void write_FPST_XP(unsigned char value);
    /**
     * \brief Writing to the rh850 FPST register IF, XC, or XP fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the generative
     * FPST write function to ensure this.
     * \param value New unmasked value of the rh850 FPST register
     */
    void gen_write_FPST_IF(fastiss::i1_t *value);
    void gen_write_FPST_XC(fastiss::i8_t *value);
    void gen_write_FPST_XP(fastiss::i8_t *value);

    /**
     * \brief Writing to the rh850 FPCC register CC fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the immediate
     * FPCC write function to ensure this.
     * \param value New unmasked value of the rh850 FPCC register
     */
    void write_FPCC_CC(unsigned char value);
    /**
     * \brief Writing to the rh850 FPCC register CC fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the generative
     * FPCC write function to ensure this.
     * \param value New unmasked value of the rh850 FPCC register
     */
    void gen_write_FPCC_CC(fastiss::i8_t *value);

    /**
     * \brief Writing to the rh850 FPCFG register RM or XE fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the immediate
     * FPCFG write function to ensure this.
     * \param value New unmasked value of the rh850 FPCFG register
     */
    void write_FPCFG_RM(unsigned char value);
    void write_FPCFG_XE(unsigned char value);
    /**
     * \brief Writing to the rh850 FPCFG register RM or XE fields also writes the
     * values to the corresponding fields in the FPSR register. Over-ride the generative
     * FPCFG write function to ensure this.
     * \param value New unmasked value of the rh850 FPCFG register
     */
    void gen_write_FPCFG_RM(fastiss::i8_t *value);
    void gen_write_FPCFG_XE(fastiss::i8_t *value);

    /**
     * \brief The rh850 FPEC register has bits 31-1 fixed to 0. Over-ride
     * the immediate FPEC write function to ensure this.
     * \param value New unmasked value of the rh850 FPEC register
     */
    void write_FPEC(unsigned int value);
    /**
     * \brief The rh850 FPEC register has bits 31-1 fixed to 0. Over-ride
     * the generative FPEC write function to ensure this.
     * \param value New unmasked value of the rh850 FPEC register
     */
    void gen_write_FPEC(fastiss::i32_t *value);    

    /**
     * \brief The rh850 CTPC register has bits 0 fixed to 0. Over-ride
     * the immediate CTPC write function to ensure this.
     * \param value New unmasked value of the rh850 CTPC register
     */
    void write_CTPC(unsigned int value);
    /**
     * \brief The rh850 CTPC register has bits 0 fixed to 0. Over-ride
     * the generative CTPC write function to ensure this.
     * \param value New unmasked value of the rh850 CTPC register
     */
    void gen_write_CTPC(fastiss::i32_t *value);

    /**
     * \brief The rh850 CTPSW register has bits 31-5 fixed to 0. Over-ride
     * the immediate CTPSW write function to ensure this.
     * \param value New unmasked value of the rh850 CTPSW register
     */
    void write_CTPSW(unsigned int value);
    /**
     * \brief The rh850 CTPSW register has bits 31-5 fixed to 0. Over-ride
     * the generative CTPSW write function to ensure this.
     * \param value New unmasked value of the rh850 CTPSW register
     */
    void gen_write_CTPSW(fastiss::i32_t *value);
  
    /**
     * \brief The rh850 CTBP register has bits 0 and 0 fixed to 0. Over-ride
     * the immediate CTBP write function to ensure this.
     * \param value New unmasked value of the rh850 CTBP register
     */
    void write_CTBP(unsigned int value);
    /**
     * \brief The rh850 CTBP register has bits 0 and 0 fixed to 0. Over-ride
     * the generative CTBP write function to ensure this.
     * \param value New unmasked value of the rh850 CTBP register
     */
    void gen_write_CTBP(fastiss::i32_t *value);
    
    /**
     * \brief The rh850 BSEL register always holds 0 and it's value should never
     * be changed. Over-ride the immediate BSEL write function to ensure this.
     * \param value This value is ignored
     */
    void write_BSEL(unsigned int value);
    /**
     * \brief The rh850 BSEL register always holds 0 and it's value should never
     * be changed. Over-ride the generative BSEL write function to ensure this.
     * \param value This value is ignored
     */
    void gen_write_BSEL(fastiss::i32_t *value);

    /**
     * \brief The rh850 BSEL register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the immediate BSEL read function
     * to ensure this.
     * \return A constant value of zero
     */
    unsigned int read_BSEL(void);
    /**
     * \brief The rh850 BSEL register always holds 0 and a constant value can
     * be returned for optimisation. Over-ride the generative BSEL read function
     * to ensure this.
     * \return A 32-bit constant value of zero
     */
    fastiss::i32_t *gen_read_BSEL(void);

    /**
     * \brief The rh850 MCFG0 register has bits 31-18 and 15-4 fixed to 0. Over-ride
     * the immediate MCFG0 write function to ensure this.
     * \param value New unmasked value of the rh850 MCFG0 register
     */
    void write_MCFG0(unsigned int value);
    /**
     * \brief The rh850 MCFG0 register has bits 31-18 and 15-4 fixed to 0. Over-ride
     * the generative MCFG0 write function to ensure this.
     * \param value New unmasked value of the rh850 MCFG0 register
     */
    void gen_write_MCFG0(fastiss::i32_t *value);

    /**
     * \brief The rh850 RBASE register has bits 8-1 fixed to 0. Over-ride
     * the immediate RBASE write function to ensure this.
     * \param value New unmasked value of the rh850 RBASE register
     */
    void write_RBASE(unsigned int value);
    /**
     * \brief The rh850 RBASE register has bits 8-1 fixed to 0. Over-ride
     * the generative RBASE write function to ensure this.
     * \param value New unmasked value of the rh850 RBASE register
     */
    void gen_write_RBASE(fastiss::i32_t *value);

    /**
     * \brief The rh850 EBASE register has bits 8-1 fixed to 0. Over-ride
     * the immediate EBASE write function to ensure this.
     * \param value New unmasked value of the rh850 EBASE register
     */
    void write_EBASE(unsigned int value);
    /**
     * \brief The rh850 EBASE register has bits 8-1 fixed to 0. Over-ride
     * the generative EBASE write function to ensure this.
     * \param value New unmasked value of the rh850 EBASE register
     */
    void gen_write_EBASE(fastiss::i32_t *value);

    /**
     * \brief The rh850 INTBP register has bits 8-1 fixed to 0. Over-ride
     * the immediate INTBP write function to ensure this.
     * \param value New unmasked value of the rh850 INTBP register
     */
    void write_INTBP(unsigned int value);
    /**
     * \brief The rh850 INTBP register has bits 8-1 fixed to 0. Over-ride
     * the generative INTBP write function to ensure this.
     * \param value New unmasked value of the rh850 INTBP register
     */
    void gen_write_INTBP(fastiss::i32_t *value);

    /**
     * \brief The rh850 MCTL register has bits 29-22 and 15-2 fixed to 0. 
     * Over-ride the immediate MCTL write function to ensure this.
     * \param value New unmasked value of the rh850 MCTL register
     */
    void write_MCTL(unsigned int value);
    /**
     * \brief The rh850 MCTL register has bits 29-22 and 15-2 fixed to 0. 
     * Over-ride the generative MCTL write function to ensure this.
     * \param value New unmasked value of the rh850 MCTL register
     */
    void gen_write_MCTL(fastiss::i32_t *value);

    /**
     * \brief The rh850 FPIPR register has bits 31-5 fixed to 0. 
     * Over-ride the immediate FPIPR write function to ensure this.
     * \param value New unmasked value of the rh850 FPIPR register
     */
    void write_FPIPR(unsigned int value);
    /**
     * \brief The rh850 FPIPR register has bits 31-5 fixed to 0. 
     * Over-ride the generative FPIPR write function to ensure this.
     * \param value New unmasked value of the rh850 FPIPR register
     */
    void gen_write_FPIPR(fastiss::i32_t *value);

    /**
     * \brief The rh850 FPIPR_FPIPR register's max value is 16. 
     * Over-ride the immediate FPIPR write function to ensure this.
     * \param value New unmasked value of the rh850 FPIPR register
     */
    void write_FPIPR_FPIPR(unsigned char value);
    /**
     * \brief The rh850 FPIPR_FPIPR register's max value is 16. 
     * Over-ride the generative FPIPR write function to ensure this.
     * \param value New unmasked value of the rh850 FPIPR register
     */
    void gen_write_FPIPR_FPIPR(fastiss::i8_t *value);

    /**
     * \brief The rh850 SCCFG register has bits 31-8 fixed to 0. 
     * Over-ride the immediate SCCFG write function to ensure this.
     * \param value New unmasked value of the rh850 SCCFG register
     */
    void write_SCCFG(unsigned int value);
    /**
     * \brief The rh850 SCCFG register has bits 31-8 fixed to 0. 
     * Over-ride the generative SCCFG write function to ensure this.
     * \param value New unmasked value of the rh850 SCCFG register
     */
    void gen_write_SCCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850 SCBP register has bits 1 and 0 fixed to 0. Over-ride
     * the immediate SCBP write function to ensure this.
     * \param value New unmasked value of the rh850 SCBP register
     */
    void write_SCBP(unsigned int value);
    /**
     * \brief The rh850 SCBP register has bits 1 and 0 fixed to 0. Over-ride
     * the generative SCBP write function to ensure this.
     * \param value New unmasked value of the rh850 SCBP register
     */
    void gen_write_SCBP(fastiss::i32_t *value);

    /**
     * \brief The rh850 HTCFG0 register has bits 31-19 and 14-11 fixed to 0. 
     * Over-ride the immediate HTCFG0 write function to ensure this.
     * \param value New unmasked value of the rh850 HTCFG0 register
     */
    void write_HTCFG0(unsigned int value);
    /**
     * \brief The rh850 HTCFG0 register has bits 31-19 and 14-11 fixed to 0. 
     * Over-ride the generative HTCFG0 write function to ensure this.
     * \param value New unmasked value of the rh850 HTCFG0 register
     */
    void gen_write_HTCFG0(fastiss::i32_t *value);

    /**
     * \brief The rh850 ASID register has bits 31-10 fixed to 0. 
     * Over-ride the immediate ASID write function to ensure this.
     * \param value New unmasked value of the rh850 ASID register
     */
    void write_ASID(unsigned int value);
    /**
     * \brief The rh850 ASID register has bits 31-10 fixed to 0. 
     * Over-ride the generative ASID write function to ensure this.
     * \param value New unmasked value of the rh850 ASID register
     */
    void gen_write_ASID(fastiss::i32_t *value);

    /**
     * \brief The rh850 MEI register has bits 31-21, 15-11 and 7-6 fixed to 0. 
     * Over-ride the immediate MEI write function to ensure this.
     * \param value New unmasked value of the rh850 MEI register
     */
    void write_MEI(unsigned int value);
    /**
     * \brief The rh850 MEI register has bits 31-21, 15-11 and 7-6 fixed to 0. 
     * Over-ride the generative MEI write function to ensure this.
     * \param value New unmasked value of the rh850 MEI register
     */
    void gen_write_MEI(fastiss::i32_t *value);

    /**
     * \brief The rh850 ISPR register has bits 31-16 fixed to 0. 
     * Over-ride the immediate ISPR write function to ensure this.
     * \param value New unmasked value of the rh850 ISPR register
     */
    void write_ISPR(unsigned int value);
    /**
     * \brief The rh850 ISPR register has bits 31-16 fixed to 0. 
     * Over-ride the generative ISPR write function to ensure this.
     * \param value New unmasked value of the rh850 ISPR register
     */
    void gen_write_ISPR(fastiss::i32_t *value);

    /**
     * \brief The rh850 PMR register has bits 31-16 fixed to 0. 
     * Over-ride the immediate PMR write function to ensure this.
     * \param value New unmasked value of the rh850 PMR register
     */
    void write_PMR(unsigned int value);
    /**
     * \brief The rh850 PMR register has bits 31-16 fixed to 0. 
     * Over-ride the generative PMR write function to ensure this.
     * \param value New unmasked value of the rh850 PMR register
     */
    void gen_write_PMR(fastiss::i32_t *value);

    /**
     * \brief The rh850 ICSR register has bits 31-2 fixed to 0. 
     * Over-ride the immediate ICSR write function to ensure this.
     * \param value New unmasked value of the rh850 ICSR register
     */
    void write_ICSR(unsigned int value);
    /**
     * \brief The rh850 ICSR register has bits 31-2 fixed to 0. 
     * Over-ride the generative ICSR write function to ensure this.
     * \param value New unmasked value of the rh850 ICSR register
     */
    void gen_write_ICSR(fastiss::i32_t *value);

    /**
     * \brief The rh850 INTCFG register has bits 31-1 fixed to 0. 
     * Over-ride the immediate INTCFG write function to ensure this.
     * \param value New unmasked value of the rh850 INTCFG register
     */
    void write_INTCFG(unsigned int value);
    /**
     * \brief The rh850 INTCFG register has bits 31-1 fixed to 0. 
     * Over-ride the generative INTCFG write function to ensure this.
     * \param value New unmasked value of the rh850 INTCFG register
     */
    void gen_write_INTCFG(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPM register has bits 31-11 and 7-3 fixed to 0.  
     * Over-ride the immediate MPM write function to ensure this.
     * \param value New unmasked value of the rh850 MPM register
     */
    void write_MPM(unsigned int value);
    /**
     * \brief The rh850 MPM register has bits 31-11 and 7-3 fixed to 0.
     * Over-ride the generative MPM write function to ensure this.
     * \param value New unmasked value of the rh850 MPM register
     */
    void gen_write_MPM(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPRC register has bits 31-16 fixed to 0.  
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New unmasked value of the rh850 MPRC register
     */
    void write_MPRC(unsigned int value);

    /**
     * \brief Sets the value of the rh850 MPRC_E15 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E15 register field
     */
    void write_MPRC_E15(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E14 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E14 register field
     */
    void write_MPRC_E14(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E13 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E13 register field
     */
    void write_MPRC_E13(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E12 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E12 register field
     */
    void write_MPRC_E12(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E11 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E11 register field
     */
    void write_MPRC_E11(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E10 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E10 register field
     */
    void write_MPRC_E10(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E9 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E9 register field
     */
    void write_MPRC_E9(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E8 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E8 register field
     */
    void write_MPRC_E8(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E7 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E7 register field
     */
    void write_MPRC_E7(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E6 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E6 register field
     */
    void write_MPRC_E6(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E5 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E5 register field
     */
    void write_MPRC_E5(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E4 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E4 register field
     */
    void write_MPRC_E4(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E3 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E3 register field
     */
    void write_MPRC_E3(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E2 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E2 register field
     */
    void write_MPRC_E2(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E1 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E1 register field
     */
    void write_MPRC_E1(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E0 register field
     * Over-ride the immediate MPRC write function to ensure this.
     * \param value New value of the rh850 MPRC_E0 register field
     */
    void write_MPRC_E0(unsigned char value);


    /**
     * \brief The rh850 MPRC register has bits 31-16 fixed to 0.
     * Over-ride the generative MPRC write function to ensure this.
     * \param value New unmasked value of the rh850 MPRC register
     */
    void gen_write_MPRC(fastiss::i32_t *value);
/**
     * \brief Generates code to set the value of the rh850 MPRC_E15 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E15 register field
     */
    void gen_write_MPRC_E15(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E14 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E14 register field
     */
    void gen_write_MPRC_E14(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E13 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E13 register field
     */
    void gen_write_MPRC_E13(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E12 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E12 register field
     */
    void gen_write_MPRC_E12(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E11 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E11 register field
     */
    void gen_write_MPRC_E11(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E10 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E10 register field
     */
    void gen_write_MPRC_E10(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E9 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E9 register field
     */
    void gen_write_MPRC_E9(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E8 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E8 register field
     */
    void gen_write_MPRC_E8(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E7 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E7 register field
     */
    void gen_write_MPRC_E7(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E6 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E6 register field
     */
    void gen_write_MPRC_E6(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E5 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E5 register field
     */
    void gen_write_MPRC_E5(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E4 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E4 register field
     */
    void gen_write_MPRC_E4(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E3 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E3 register field
     */
    void gen_write_MPRC_E3(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E2 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E2 register field
     */
    void gen_write_MPRC_E2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E1 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E1 register field
     */
    void gen_write_MPRC_E1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E0 register field
     * Over-ride the generative MPRC write function to ensure this.
     * \param value Object representing the new value of the rh850 MPRC_E0 register field
     */
    void gen_write_MPRC_E0(fastiss::i1_t *value);


    /**
     * \brief The rh850 MPBRGN register has bits 31-4 fixed to 0.  
     * Over-ride the immediate MPBRGN write function to ensure this.
     * \param value New unmasked value of the rh850 MPBRGN register
     */
    void write_MPBRGN(unsigned int value);
    /**
     * \brief The rh850 MPBRGN register has bits 31-4 fixed to 0.
     * Over-ride the generative MPBRGN write function to ensure this.
     * \param value New unmasked value of the rh850 MPBRGN register
     */
    void gen_write_MPBRGN(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPTRGN register has bits 31-4 fixed to 0.  
     * Over-ride the immediate MPTRGN write function to ensure this.
     * \param value New unmasked value of the rh850 MPTRGN register
     */
    void write_MPTRGN(unsigned int value);
    /**
     * \brief The rh850 MPTRGN register has bits 31-4 fixed to 0.
     * Over-ride the generative MPTRGN write function to ensure this.
     * \param value New unmasked value of the rh850 MPTRGN register
     */
    void gen_write_MPTRGN(fastiss::i32_t *value);


    /**
     * \brief The rh850 MCR register has bits 31-9 and 7-6 fixed to 0.  
     * Over-ride the immediate MCR write function to ensure this.
     * \param value New unmasked value of the rh850 MCR register
     */
    void write_MCR(unsigned int value);
    /**
     * \brief The rh850 MCR register has bits 31-9 and 7-6 fixed to 0.
     * Over-ride the generative MCR write function to ensure this.
     * \param value New unmasked value of the rh850 MCR register
     */
    void gen_write_MCR(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPLA0 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA0 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA0 register
     */
    void write_MPLA0(unsigned int value);
    /**
     * \brief The rh850 MPLA0 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA0 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA0 register
     */
    void gen_write_MPLA0(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA0 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA0 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA0 register
     */
    void write_MPUA0(unsigned int value);
    /**
     * \brief The rh850 MPUA0 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA0 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA0 register
     */
    void gen_write_MPUA0(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT0 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT0 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT0 register
     */
    void write_MPAT0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT0_E register field
     * Over-ride the immediate MPAT0 write function to ensure this.
     * \param value New value of the rh850 MPAT0_E register field
     */
    void write_MPAT0_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_E register field
     * Over-ride the immediate MPAT0 write function to ensure this.
     * \param value New value of the rh850 MPAT0_E register field
     */
    void write_MPAT0_E(unsigned char value);
    /**
     * \brief The rh850 MPAT0 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT0 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT0 register
     */
    void gen_write_MPAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_G register field
     * Over-ride the generative MPAT0 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT0_G register field
     */
    void gen_write_MPAT0_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_E register field
     * Over-ride the generative MPAT0 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT0_E register field
     */
    void gen_write_MPAT0_E(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA1 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA1 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA1 register
     */
    void write_MPLA1(unsigned int value);
    /**
     * \brief The rh850 MPLA1 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA1 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA1 register
     */
    void gen_write_MPLA1(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA1 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA1 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA1 register
     */
    void write_MPUA1(unsigned int value);
    /**
     * \brief The rh850 MPUA1 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA1 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA1 register
     */
    void gen_write_MPUA1(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT1 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT1 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT1 register
     */
    void write_MPAT1(unsigned int value);
    /**
     * \brief The rh850 MPAT1 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT1 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT1 register
     */
    void gen_write_MPAT1(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT1_E register field
     * Over-ride the immediate MPAT1 write function to ensure this.
     * \param value New value of the rh850 MPAT1_E register field
     */
    void write_MPAT1_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_E register field
     * Over-ride the generative MPAT1 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT1_E register field
     */
    void gen_write_MPAT1_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT1_G register field
     * Over-ride the immediate MPAT1 write function to ensure this.
     * \param value New value of the rh850 MPAT1_G register field
     */
    void write_MPAT1_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_G register field
     * Over-ride the generative MPAT1 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT1_G register field
     */
    void gen_write_MPAT1_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA2 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA2 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA2 register
     */
    void write_MPLA2(unsigned int value);
    /**
     * \brief The rh850 MPLA2 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA2 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA2 register
     */
    void gen_write_MPLA2(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA2 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA2 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA2 register
     */
    void write_MPUA2(unsigned int value);
    /**
     * \brief The rh850 MPUA2 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA2 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA2 register
     */
    void gen_write_MPUA2(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT2 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT2 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT2 register
     */
    void write_MPAT2(unsigned int value);
    /**
     * \brief The rh850 MPAT2 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT2 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT2 register
     */
    void gen_write_MPAT2(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT1_E register field
     * Over-ride the immediate MPAT1 write function to ensure this.
     * \param value New value of the rh850 MPAT1_E register field
     */
    void write_MPAT2_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_E register field
     * Over-ride the generative MPAT1 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT1_E register field
     */
    void gen_write_MPAT2_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT2_G register field
     * Over-ride the immediate MPAT2 write function to ensure this.
     * \param value New value of the rh850 MPAT2_G register field
     */
    void write_MPAT2_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_G register field
     * Over-ride the generative MPAT2 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT2_G register field
     */
    void gen_write_MPAT2_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA3 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA3 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA3 register
     */
    void write_MPLA3(unsigned int value);
    /**
     * \brief The rh850 MPLA3 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA3 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA3 register
     */
    void gen_write_MPLA3(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA3 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA3 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA3 register
     */
    void write_MPUA3(unsigned int value);
    /**
     * \brief The rh850 MPUA3 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA3 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA3 register
     */
    void gen_write_MPUA3(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT3 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT3 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT3 register
     */
    void write_MPAT3(unsigned int value);
    /**
     * \brief The rh850 MPAT3 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT3 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT3 register
     */
    void gen_write_MPAT3(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT1_E register field
     * Over-ride the immediate MPAT1 write function to ensure this.
     * \param value New value of the rh850 MPAT1_E register field
     */
    void write_MPAT3_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_E register field
     * Over-ride the generative MPAT1 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT1_E register field
     */
    void gen_write_MPAT3_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT3_G register field
     * Over-ride the immediate MPAT3 write function to ensure this.
     * \param value New value of the rh850 MPAT3_G register field
     */
    void write_MPAT3_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_G register field
     * Over-ride the generative MPAT3 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT3_G register field
     */
    void gen_write_MPAT3_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA4 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA4 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA4 register
     */
    void write_MPLA4(unsigned int value);
    /**
     * \brief The rh850 MPLA4 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA4 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA4 register
     */
    void gen_write_MPLA4(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA4 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA4 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA4 register
     */
    void write_MPUA4(unsigned int value);
    /**
     * \brief The rh850 MPUA4 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA4 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA4 register
     */
    void gen_write_MPUA4(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT4 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT4 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT4 register
     */
    void write_MPAT4(unsigned int value);
    /**
     * \brief The rh850 MPAT4 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT4 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT4 register
     */
    void gen_write_MPAT4(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT1_E register field
     * Over-ride the immediate MPAT1 write function to ensure this.
     * \param value New value of the rh850 MPAT1_E register field
     */
    void write_MPAT4_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_E register field
     * Over-ride the generative MPAT1 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT1_E register field
     */
    void gen_write_MPAT4_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT4_G register field
     * Over-ride the immediate MPAT4 write function to ensure this.
     * \param value New value of the rh850 MPAT4_G register field
     */
    void write_MPAT4_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_G register field
     * Over-ride the generative MPAT4 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT4_G register field
     */
    void gen_write_MPAT4_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA5 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA5 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA5 register
     */
    void write_MPLA5(unsigned int value);
    /**
     * \brief The rh850 MPLA5 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA5 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA5 register
     */
    void gen_write_MPLA5(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA5 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA5 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA5 register
     */
    void write_MPUA5(unsigned int value);
    /**
     * \brief The rh850 MPUA5 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA5 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA5 register
     */
    void gen_write_MPUA5(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT5 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT5 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT5 register
     */
    void write_MPAT5(unsigned int value);
    /**
     * \brief The rh850 MPAT5 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT5 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT5 register
     */
    void gen_write_MPAT5(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT5_E register field
     * Over-ride the immediate MPAT5 write function to ensure this.
     * \param value New value of the rh850 MPAT1_E register field
     */
    void write_MPAT5_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_E register field
     * Over-ride the generative MPAT5 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT1_E register field
     */
    void gen_write_MPAT5_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT5_G register field
     * Over-ride the immediate MPAT5 write function to ensure this.
     * \param value New value of the rh850 MPAT1_G register field
     */
    void write_MPAT5_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_G register field
     * Over-ride the generative MPAT5 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT5_G register field
     */
    void gen_write_MPAT5_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA6 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA6 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA6 register
     */
    void write_MPLA6(unsigned int value);
    /**
     * \brief The rh850 MPLA6 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA6 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA6 register
     */
    void gen_write_MPLA6(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA6 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA6 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA6 register
     */
    void write_MPUA6(unsigned int value);
    /**
     * \brief The rh850 MPUA6 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA6 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA6 register
     */
    void gen_write_MPUA6(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT6 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT6 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT6 register
     */
    void write_MPAT6(unsigned int value);
    /**
     * \brief The rh850 MPAT6 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT6 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT6 register
     */
    void gen_write_MPAT6(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT6_E register field
     * Over-ride the immediate MPAT6 write function to ensure this.
     * \param value New value of the rh850 MPAT6_E register field
     */
    void write_MPAT6_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_G register field
     * Over-ride the generative MPAT6 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT6_G register field
     */
    void gen_write_MPAT6_G(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT6_E register field
     * Over-ride the immediate MPAT6 write function to ensure this.
     * \param value New value of the rh850 MPAT6_E register field
     */
    void write_MPAT6_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_E register field
     * Over-ride the generative MPAT6 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT6_E register field
     */
    void gen_write_MPAT6_E(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA7 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA7 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA7 register
     */
    void write_MPLA7(unsigned int value);
    /**
     * \brief The rh850 MPLA7 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA7 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA7 register
     */
    void gen_write_MPLA7(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA7 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA7 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA7 register
     */
    void write_MPUA7(unsigned int value);
    /**
     * \brief The rh850 MPUA7 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA7 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA7 register
     */
    void gen_write_MPUA7(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT7 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT7 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT7 register
     */
    void write_MPAT7(unsigned int value);
    /**
     * \brief The rh850 MPAT7 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT7 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT7 register
     */
    void gen_write_MPAT7(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT7_E register field
     * Over-ride the immediate MPAT7 write function to ensure this.
     * \param value New value of the rh850 MPAT7_E register field
     */
    void write_MPAT7_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_E register field
     * Over-ride the generative MPAT7 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT7_E register field
     */
    void gen_write_MPAT7_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT7_G register field
     * Over-ride the immediate MPAT7 write function to ensure this.
     * \param value New value of the rh850 MPAT7_G register field
     */
    void write_MPAT7_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_G register field
     * Over-ride the generative MPAT7 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT7_G register field
     */
    void gen_write_MPAT7_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA8 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA8 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA8 register
     */
    void write_MPLA8(unsigned int value);
    /**
     * \brief The rh850 MPLA8 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA8 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA8 register
     */
    void gen_write_MPLA8(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA8 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA8 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA8 register
     */
    void write_MPUA8(unsigned int value);
    /**
     * \brief The rh850 MPUA8 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA8 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA8 register
     */
    void gen_write_MPUA8(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT8 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT8 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT8 register
     */
    void write_MPAT8(unsigned int value);
    /**
     * \brief The rh850 MPAT8 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT8 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT8 register
     */
    void gen_write_MPAT8(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT8_E register field
     * Over-ride the immediate MPAT8 write function to ensure this.
     * \param value New value of the rh850 MPAT8_E register field
     */
    void write_MPAT8_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_E register field
     * Over-ride the generative MPAT8 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT8_E register field
     */
    void gen_write_MPAT8_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT8_G register field
     * Over-ride the immediate MPAT8 write function to ensure this.
     * \param value New value of the rh850 MPAT8_G register field
     */
    void write_MPAT8_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_G register field
     * Over-ride the generative MPAT8 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT8_G register field
     */
    void gen_write_MPAT8_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA9 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA9 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA9 register
     */
    void write_MPLA9(unsigned int value);
    /**
     * \brief The rh850 MPLA9 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA9 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA9 register
     */
    void gen_write_MPLA9(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA9 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA9 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA9 register
     */
    void write_MPUA9(unsigned int value);
    /**
     * \brief The rh850 MPUA9 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA9 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA9 register
     */
    void gen_write_MPUA9(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT9 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT9 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT9 register
     */
    void write_MPAT9(unsigned int value);
    /**
     * \brief The rh850 MPAT9 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT9 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT9 register
     */
    void gen_write_MPAT9(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT9_E register field
     * Over-ride the immediate MPAT9 write function to ensure this.
     * \param value New value of the rh850 MPAT9_E register field
     */
    void write_MPAT9_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_E register field
     * Over-ride the generative MPAT9 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT9_E register field
     */
    void gen_write_MPAT9_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT9_G register field
     * Over-ride the immediate MPAT9 write function to ensure this.
     * \param value New value of the rh850 MPAT9_G register field
     */
    void write_MPAT9_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_G register field
     * Over-ride the generative MPAT9 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT9_G register field
     */
    void gen_write_MPAT9_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA10 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA10 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA10 register
     */
    void write_MPLA10(unsigned int value);
    /**
     * \brief The rh850 MPLA10 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA10 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA10 register
     */
    void gen_write_MPLA10(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA10 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA10 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA10 register
     */
    void write_MPUA10(unsigned int value);
    /**
     * \brief The rh850 MPUA10 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA10 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA10 register
     */
    void gen_write_MPUA10(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT10 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT10 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT10 register
     */
    void write_MPAT10(unsigned int value);
    /**
     * \brief The rh850 MPAT10 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT10 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT10 register
     */
    void gen_write_MPAT10(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT10_E register field
     * Over-ride the immediate MPAT10 write function to ensure this.
     * \param value New value of the rh850 MPAT10_E register field
     */
    void write_MPAT10_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_E register field
     * Over-ride the generative MPAT10 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT10_E register field
     */
    void gen_write_MPAT10_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT10_G register field
     * Over-ride the immediate MPAT10 write function to ensure this.
     * \param value New value of the rh850 MPAT10_G register field
     */
    void write_MPAT10_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_G register field
     * Over-ride the generative MPAT10 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT10_G register field
     */
    void gen_write_MPAT10_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA11 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA11 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA11 register
     */
    void write_MPLA11(unsigned int value);
    /**
     * \brief The rh850 MPLA11 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA11 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA11 register
     */
    void gen_write_MPLA11(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA11 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA11 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA11 register
     */
    void write_MPUA11(unsigned int value);
    /**
     * \brief The rh850 MPUA11 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA11 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA11 register
     */
    void gen_write_MPUA11(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT11 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT11 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT11 register
     */
    void write_MPAT11(unsigned int value);
    /**
     * \brief The rh850 MPAT11 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT11 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT11 register
     */
    void gen_write_MPAT11(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT11_E register field
     * Over-ride the immediate MPAT11 write function to ensure this.
     * \param value New value of the rh850 MPAT11_E register field
     */
    void write_MPAT11_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_E register field
     * Over-ride the generative MPAT6 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT6_E register field
     */
    void gen_write_MPAT11_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT11_G register field
     * Over-ride the immediate MPAT11 write function to ensure this.
     * \param value New value of the rh850 MPAT11_G register field
     */
    void write_MPAT11_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_G register field
     * Over-ride the generative MPAT11 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT11_G register field
     */
    void gen_write_MPAT11_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA12 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA12 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA12 register
     */
    void write_MPLA12(unsigned int value);
    /**
     * \brief The rh850 MPLA12 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA12 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA12 register
     */
    void gen_write_MPLA12(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA12 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA12 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA12 register
     */
    void write_MPUA12(unsigned int value);
    /**
     * \brief The rh850 MPUA12 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA12 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA12 register
     */
    void gen_write_MPUA12(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT12 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT12 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT12 register
     */
    void write_MPAT12(unsigned int value);
    /**
     * \brief The rh850 MPAT12 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT12 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT12 register
     */
    void gen_write_MPAT12(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT12_E register field
     * Over-ride the immediate MPAT12 write function to ensure this.
     * \param value New value of the rh850 MPAT12_E register field
     */
    void write_MPAT12_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_E register field
     * Over-ride the generative MPAT12 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT12_E register field
     */
    void gen_write_MPAT12_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT12_G register field
     * Over-ride the immediate MPAT12 write function to ensure this.
     * \param value New value of the rh850 MPAT12_G register field
     */
    void write_MPAT12_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_G register field
     * Over-ride the generative MPAT12 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT12_G register field
     */
    void gen_write_MPAT12_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA13 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA13 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA13 register
     */
    void write_MPLA13(unsigned int value);
    /**
     * \brief The rh850 MPLA13 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA13 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA13 register
     */
    void gen_write_MPLA13(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA13 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA13 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA13 register
     */
    void write_MPUA13(unsigned int value);
    /**
     * \brief The rh850 MPUA13 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA13 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA13 register
     */
    void gen_write_MPUA13(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT13 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT13 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT13 register
     */
    void write_MPAT13(unsigned int value);
    /**
     * \brief The rh850 MPAT13 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT13 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT13 register
     */
    void gen_write_MPAT13(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT13_E register field
     * Over-ride the immediate MPAT13 write function to ensure this.
     * \param value New value of the rh850 MPAT13_E register field
     */
    void write_MPAT13_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_E register field
     * Over-ride the generative MPAT13 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT13_E register field
     */
    void gen_write_MPAT13_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT13_G register field
     * Over-ride the immediate MPAT13 write function to ensure this.
     * \param value New value of the rh850 MPAT13_G register field
     */
    void write_MPAT13_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_G register field
     * Over-ride the generative MPAT13 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT13_G register field
     */
    void gen_write_MPAT13_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA14 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA14 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA14 register
     */
    void write_MPLA14(unsigned int value);
    /**
     * \brief The rh850 MPLA14 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA14 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA14 register
     */
    void gen_write_MPLA14(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA14 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA14 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA14 register
     */
    void write_MPUA14(unsigned int value);
    /**
     * \brief The rh850 MPUA14 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA14 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA14 register
     */
    void gen_write_MPUA14(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT14 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT14 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT14 register
     */
    void write_MPAT14(unsigned int value);
    /**
     * \brief The rh850 MPAT14 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT14 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT14 register
     */
    void gen_write_MPAT14(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT13_E register field
     * Over-ride the immediate MPAT13 write function to ensure this.
     * \param value New value of the rh850 MPAT13_E register field
     */
    void write_MPAT14_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_E register field
     * Over-ride the generative MPAT14 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT14_E register field
     */
    void gen_write_MPAT14_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT14_G register field
     * Over-ride the immediate MPAT14 write function to ensure this.
     * \param value New value of the rh850 MPAT14_G register field
     */
    void write_MPAT14_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_G register field
     * Over-ride the generative MPAT14 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT14_G register field
     */
    void gen_write_MPAT14_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 MPLA15 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPLA15 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA15 register
     */
    void write_MPLA15(unsigned int value);
    /**
     * \brief The rh850 MPLA15 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPLA15 write function to ensure this.
     * \param value New unmasked value of the rh850 MPLA15 register
     */
    void gen_write_MPLA15(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPUA15 register has bits 31-2 fixed to 0. 
     * Over-ride the immediate MPUA15 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA15 register
     */
    void write_MPUA15(unsigned int value);
    /**
     * \brief The rh850 MPUA15 register has bits 31-2 fixed to 0.
     * Over-ride the generative MPUA15 write function to ensure this.
     * \param value New unmasked value of the rh850 MPUA15 register
     */
    void gen_write_MPUA15(fastiss::i32_t *value);

    /**
     * \brief The rh850 MPAT15 register has bits 31-26 and 15-8 fixed to 0. 
     * Over-ride the immediate MPAT15 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT15 register
     */
    void write_MPAT15(unsigned int value);
    /**
     * \brief The rh850 MPAT15 register has bits 31-26 and 15-8 fixed to 0.
     * Over-ride the generative MPAT15 write function to ensure this.
     * \param value New unmasked value of the rh850 MPAT15 register
     */
    void gen_write_MPAT15(fastiss::i32_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT15_E register field
     * Over-ride the immediate MPAT15 write function to ensure this.
     * \param value New value of the rh850 MPAT15_E register field
     */
    void write_MPAT15_E(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_E register field
     * Over-ride the generative MPAT15 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT15_E register field
     */
    void gen_write_MPAT15_E(fastiss::i1_t *value);
    /**
     * \brief Sets the value of the rh850 MPAT15_G register field
     * Over-ride the immediate MPAT15 write function to ensure this.
     * \param value New value of the rh850 MPAT15_G register field
     */
    void write_MPAT15_G(unsigned char value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_E register field
     * Over-ride the generative MPAT15 write function to ensure this.
     * \param value Object representing the new value of the rh850 MPAT15_E register field
     */
    void gen_write_MPAT15_G(fastiss::i1_t *value);

    /**
     * \brief The rh850 DBCMC register has bits 31-19 fixed to 0. 
     * Over-ride the immediate DBCMC write function to ensure this.
     * \param value New unmasked value of the rh850 DBCMC register
     */
    void write_DBCMC(unsigned int value);
    /**
     * \brief The rh850 DBCMC register has bits 31-19 fixed to 0.
     * Over-ride the generative DBCMC write function to ensure this.
     * \param value New unmasked value of the rh850 DBCMC register
     */
    void gen_write_DBCMC(fastiss::i32_t *value);
  	
    /**
     * \brief The rh850 DBPC register has bits 0 fixed to 0. Over-ride
     * the immediate DBPC write function to ensure this.
     * \param value New unmasked value of the rh850 DBPC register
     */
    void write_DBPC(unsigned int value);
    /**
     * \brief The rh850 DBPC register has bits 0 fixed to 0. Over-ride
     * the generative DBPC write function to ensure this.
     * \param value New unmasked value of the rh850 DBPC register
     */
    void gen_write_DBPC(fastiss::i32_t *value);

    /**
     * \brief The rh850 DBPSW register has bits 29-20, 14-12, 10 and 8 fixed to 0. 
     * Over-ride the immediate DBPSW write function to ensure this.
     * \param value New unmasked value of the rh850 DBPSW register
     */
    void write_DBPSW(unsigned int value);
    /**
     * \brief The rh850 DBPSW register has bits 29-20, 14-12, 10 and 8 fixed to 0. 
     * Over-ride the generative DBPSW write function to ensure this.
     * \param value New unmasked value of the rh850 DBPSW register
     */
    void gen_write_DBPSW(fastiss::i32_t *value);

    /**
     * \brief The rh850 DIR0 register has bits 11-10 and 7 fixed to 0. 
     * Over-ride the immediate DIR0 write function to ensure this.
     * \param value New unmasked value of the rh850 DIR0 register
     */
    void write_DIR0(unsigned int value);
    /**
     * \brief The rh850 DIR0 register has bits 11-10 and 7 fixed to 0.
     * Over-ride the generative DIR0 write function to ensure this.
     * \param value New unmasked value of the rh850 DIR0 register
     */
    void gen_write_DIR0(fastiss::i32_t *value);

    /**
     * \brief The rh850 DIR1 register has bits 31-20 and 3 fixed to 0. 
     * Over-ride the immediate DIR1 write function to ensure this.
     * \param value New unmasked value of the rh850 DIR1 register
     */
    void write_DIR1(unsigned int value);
    /**
     * \brief The rh850 DIR1 register has bits 31-20 and 3 fixed to 0.
     * Over-ride the generative DIR1 write function to ensure this.
     * \param value New unmasked value of the rh850 DIR1 register
     */
    void gen_write_DIR1(fastiss::i32_t *value);

    /**
     * \brief The rh850 BPC register has bits 31-29 and 23-13 fixed to 0. 
     * Over-ride the immediate BPC write function to ensure this.
     * \param value New unmasked value of the rh850 BPC register
     */
    void write_BPC(unsigned int value);
    /**
     * \brief The rh850 BPC register has bits 31-29 and 23-13 fixed to 0.
     * Over-ride the generative BPC write function to ensure this.
     * \param value New unmasked value of the rh850 BPC register
     */
    void gen_write_BPC(fastiss::i32_t *value);
   };
}

#endif // _RH850_CUSTOM_CPU_STATE_IMPL_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
