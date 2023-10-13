//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file cpu_state_if.h
 * \brief Defines the CPU state interface class
 */

#ifndef _RH850G4_CPU_STATE_IF_H_
#define _RH850G4_CPU_STATE_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "fastiss/cpu_state_base_if.h"
#include <string>

//------------------------------------------------------------------------------
// Code generator class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class i1_t;
  class i8_t;
  class i16_t;
  class i32_t;
  class i64_t;
}

//------------------------------------------------------------------------------
// rh850g4 CPU state interface definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850g4 specific components
 */
namespace rh850g4
{
  /**
   * \brief Indices for individual registers in the rh850g4 GR register group
   */
  typedef enum
  {
    GR_INDEX_R0_0 = 0,
    GR_INDEX_R1_1 = 1,
    GR_INDEX_R2_2 = 2,
    GR_INDEX_R3_3 = 3,
    GR_INDEX_R4_4 = 4,
    GR_INDEX_R5_5 = 5,
    GR_INDEX_R6_6 = 6,
    GR_INDEX_R7_7 = 7,
    GR_INDEX_R8_8 = 8,
    GR_INDEX_R9_9 = 9,
    GR_INDEX_R10_10 = 10,
    GR_INDEX_R11_11 = 11,
    GR_INDEX_R12_12 = 12,
    GR_INDEX_R13_13 = 13,
    GR_INDEX_R14_14 = 14,
    GR_INDEX_R15_15 = 15,
    GR_INDEX_R16_16 = 16,
    GR_INDEX_R17_17 = 17,
    GR_INDEX_R18_18 = 18,
    GR_INDEX_R19_19 = 19,
    GR_INDEX_R20_20 = 20,
    GR_INDEX_R21_21 = 21,
    GR_INDEX_R22_22 = 22,
    GR_INDEX_R23_23 = 23,
    GR_INDEX_R24_24 = 24,
    GR_INDEX_R25_25 = 25,
    GR_INDEX_R26_26 = 26,
    GR_INDEX_R27_27 = 27,
    GR_INDEX_R28_28 = 28,
    GR_INDEX_R29_29 = 29,
    GR_INDEX_R30_30 = 30,
    GR_INDEX_R31_31 = 31
  } GR_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 WR register group
   */
  typedef enum
  {
    WR_INDEX_WR0_0 = 0,
    WR_INDEX_WR0_U_1 = 1,
    WR_INDEX_WR1_2 = 2,
    WR_INDEX_WR1_U_3 = 3,
    WR_INDEX_WR2_4 = 4,
    WR_INDEX_WR2_U_5 = 5,
    WR_INDEX_WR3_6 = 6,
    WR_INDEX_WR3_U_7 = 7,
    WR_INDEX_WR4_8 = 8,
    WR_INDEX_WR4_U_9 = 9,
    WR_INDEX_WR5_10 = 10,
    WR_INDEX_WR5_U_11 = 11,
    WR_INDEX_WR6_12 = 12,
    WR_INDEX_WR6_U_13 = 13,
    WR_INDEX_WR7_14 = 14,
    WR_INDEX_WR7_U_15 = 15,
    WR_INDEX_WR8_16 = 16,
    WR_INDEX_WR8_U_17 = 17,
    WR_INDEX_WR9_18 = 18,
    WR_INDEX_WR9_U_19 = 19,
    WR_INDEX_WR10_20 = 20,
    WR_INDEX_WR10_U_21 = 21,
    WR_INDEX_WR11_22 = 22,
    WR_INDEX_WR11_U_23 = 23,
    WR_INDEX_WR12_24 = 24,
    WR_INDEX_WR12_U_25 = 25,
    WR_INDEX_WR13_26 = 26,
    WR_INDEX_WR13_U_27 = 27,
    WR_INDEX_WR14_28 = 28,
    WR_INDEX_WR14_U_29 = 29,
    WR_INDEX_WR15_30 = 30,
    WR_INDEX_WR15_U_31 = 31,
    WR_INDEX_WR16_32 = 32,
    WR_INDEX_WR16_U_33 = 33,
    WR_INDEX_WR17_34 = 34,
    WR_INDEX_WR17_U_35 = 35,
    WR_INDEX_WR18_36 = 36,
    WR_INDEX_WR18_U_37 = 37,
    WR_INDEX_WR19_38 = 38,
    WR_INDEX_WR19_U_39 = 39,
    WR_INDEX_WR20_40 = 40,
    WR_INDEX_WR20_U_41 = 41,
    WR_INDEX_WR21_42 = 42,
    WR_INDEX_WR21_U_43 = 43,
    WR_INDEX_WR22_44 = 44,
    WR_INDEX_WR22_U_45 = 45,
    WR_INDEX_WR23_46 = 46,
    WR_INDEX_WR23_U_47 = 47,
    WR_INDEX_WR24_48 = 48,
    WR_INDEX_WR24_U_49 = 49,
    WR_INDEX_WR25_50 = 50,
    WR_INDEX_WR25_U_51 = 51,
    WR_INDEX_WR26_52 = 52,
    WR_INDEX_WR26_U_53 = 53,
    WR_INDEX_WR27_54 = 54,
    WR_INDEX_WR27_U_55 = 55,
    WR_INDEX_WR28_56 = 56,
    WR_INDEX_WR28_U_57 = 57,
    WR_INDEX_WR29_58 = 58,
    WR_INDEX_WR29_U_59 = 59,
    WR_INDEX_WR30_60 = 60,
    WR_INDEX_WR30_U_61 = 61,
    WR_INDEX_WR31_62 = 62,
    WR_INDEX_WR31_U_63 = 63
  } WR_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_0 register group
   */
  typedef enum
  {
    SR_SELID_0_INDEX_EIPC_0 = 0,
    SR_SELID_0_INDEX_EIPSW_1 = 1,
    SR_SELID_0_INDEX_FEPC_2 = 2,
    SR_SELID_0_INDEX_FEPSW_3 = 3,
    SR_SELID_0_INDEX_PSW_5 = 5,
    SR_SELID_0_INDEX_FPSR_6 = 6,
    SR_SELID_0_INDEX_FPEPC_7 = 7,
    SR_SELID_0_INDEX_FPST_8 = 8,
    SR_SELID_0_INDEX_FPCC_9 = 9,
    SR_SELID_0_INDEX_FPCFG_10 = 10,
    SR_SELID_0_INDEX_EIIC_13 = 13,
    SR_SELID_0_INDEX_FEIC_14 = 14,
    SR_SELID_0_INDEX_PSWH_15 = 15,
    SR_SELID_0_INDEX_CTPC_16 = 16,
    SR_SELID_0_INDEX_CTPSW_17 = 17,
    SR_SELID_0_INDEX_EIPSWH_18 = 18,
    SR_SELID_0_INDEX_FEPSWH_19 = 19,
    SR_SELID_0_INDEX_CTBP_20 = 20,
    SR_SELID_0_INDEX_SNZCFG_21 = 21,
    SR_SELID_0_INDEX_EIWR_28 = 28,
    SR_SELID_0_INDEX_FEWR_29 = 29
  } SR_SELID_0_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_1 register group
   */
  typedef enum
  {
    SR_SELID_1_INDEX_SPID_0 = 0,
    SR_SELID_1_INDEX_SPIDLIST_1 = 1,
    SR_SELID_1_INDEX_RBASE_2 = 2,
    SR_SELID_1_INDEX_EBASE_3 = 3,
    SR_SELID_1_INDEX_INTBP_4 = 4,
    SR_SELID_1_INDEX_MCTL_5 = 5,
    SR_SELID_1_INDEX_PID_6 = 6,
    SR_SELID_1_INDEX_SVLOCK_8 = 8,
    SR_SELID_1_INDEX_SCCFG_11 = 11,
    SR_SELID_1_INDEX_SCBP_12 = 12,
    SR_SELID_1_INDEX_HVCFG_16 = 16,
    SR_SELID_1_INDEX_GMCFG_17 = 17,
    SR_SELID_1_INDEX_HVSB_20 = 20
  } SR_SELID_1_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_2 register group
   */
  typedef enum
  {
    SR_SELID_2_INDEX_PEID_0 = 0,
    SR_SELID_2_INDEX_BMID_1 = 1,
    SR_SELID_2_INDEX_MEA_6 = 6,
    SR_SELID_2_INDEX_MEI_8 = 8,
    SR_SELID_2_INDEX_ISPR_10 = 10,
    SR_SELID_2_INDEX_IMSR_11 = 11,
    SR_SELID_2_INDEX_ICSR_12 = 12,
    SR_SELID_2_INDEX_INTCFG_13 = 13,
    SR_SELID_2_INDEX_PLMR_14 = 14,
    SR_SELID_2_INDEX_RBCR0_15 = 15,
    SR_SELID_2_INDEX_RBCR1_16 = 16,
    SR_SELID_2_INDEX_RBNR_17 = 17,
    SR_SELID_2_INDEX_RBIP_18 = 18
  } SR_SELID_2_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_3 register group
   */
  typedef enum
  {
    SR_SELID_3_INDEX_DBGEN_0 = 0,
    SR_SELID_3_INDEX_DBPSWH_13 = 13,
    SR_SELID_3_INDEX_DBIC_15 = 15,
    SR_SELID_3_INDEX_DBPC_18 = 18,
    SR_SELID_3_INDEX_DBPSW_19 = 19,
    SR_SELID_3_INDEX_DIR0_20 = 20,
    SR_SELID_3_INDEX_DIR1_21 = 21,
    SR_SELID_3_INDEX_BPC_22 = 22,
    SR_SELID_3_INDEX_BPAV_24 = 24,
    SR_SELID_3_INDEX_BPAM_25 = 25,
    SR_SELID_3_INDEX_DBCFG_28 = 28,
    SR_SELID_3_INDEX_DBWR_30 = 30
  } SR_SELID_3_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_4 register group
   */
  typedef enum
  {
    SR_SELID_4_INDEX_ICTAGL_16 = 16,
    SR_SELID_4_INDEX_ICTAGH_17 = 17,
    SR_SELID_4_INDEX_ICDATL_18 = 18,
    SR_SELID_4_INDEX_ICDATH_19 = 19,
    SR_SELID_4_INDEX_ICCTRL_24 = 24,
    SR_SELID_4_INDEX_ICCFG_26 = 26,
    SR_SELID_4_INDEX_ICERR_28 = 28
  } SR_SELID_4_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_5 register group
   */
  typedef enum
  {
    SR_SELID_5_INDEX_MPM_0 = 0,
    SR_SELID_5_INDEX_MPCFG_2 = 2,
    SR_SELID_5_INDEX_MCA_8 = 8,
    SR_SELID_5_INDEX_MCS_9 = 9,
    SR_SELID_5_INDEX_MCC_10 = 10,
    SR_SELID_5_INDEX_MCR_11 = 11,
    SR_SELID_5_INDEX_MCI_12 = 12,
    SR_SELID_5_INDEX_MPIDX_16 = 16,
    SR_SELID_5_INDEX_MPBK_17 = 17,
    SR_SELID_5_INDEX_MPLA_20 = 20,
    SR_SELID_5_INDEX_MPUA_21 = 21,
    SR_SELID_5_INDEX_MPAT_22 = 22,
    SR_SELID_5_INDEX_MPID0_24 = 24,
    SR_SELID_5_INDEX_MPID1_25 = 25,
    SR_SELID_5_INDEX_MPID2_26 = 26,
    SR_SELID_5_INDEX_MPID3_27 = 27,
    SR_SELID_5_INDEX_MPID4_28 = 28,
    SR_SELID_5_INDEX_MPID5_29 = 29,
    SR_SELID_5_INDEX_MPID6_30 = 30,
    SR_SELID_5_INDEX_MPID7_31 = 31
  } SR_SELID_5_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_9 register group
   */
  typedef enum
  {
    SR_SELID_9_INDEX_GMEIPC_0 = 0,
    SR_SELID_9_INDEX_GMEIPSW_1 = 1,
    SR_SELID_9_INDEX_GMFEPC_2 = 2,
    SR_SELID_9_INDEX_GMFEPSW_3 = 3,
    SR_SELID_9_INDEX_GMPSW_5 = 5,
    SR_SELID_9_INDEX_GMMEA_6 = 6,
    SR_SELID_9_INDEX_GMMEI_8 = 8,
    SR_SELID_9_INDEX_GMEIIC_13 = 13,
    SR_SELID_9_INDEX_GMFEIC_14 = 14,
    SR_SELID_9_INDEX_GMSPID_16 = 16,
    SR_SELID_9_INDEX_GMSPIDLIST_17 = 17,
    SR_SELID_9_INDEX_GMEBASE_19 = 19,
    SR_SELID_9_INDEX_GMINTBP_20 = 20,
    SR_SELID_9_INDEX_GMINTCFG_21 = 21,
    SR_SELID_9_INDEX_GMPLMR_22 = 22,
    SR_SELID_9_INDEX_GMSVLOCK_24 = 24,
    SR_SELID_9_INDEX_GMMPM_25 = 25,
    SR_SELID_9_INDEX_GMEIWR_28 = 28,
    SR_SELID_9_INDEX_GMFEWR_29 = 29,
    SR_SELID_9_INDEX_GMPEID_30 = 30
  } SR_SELID_9_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_10 register group
   */
  typedef enum
  {
    SR_SELID_10_INDEX_FXSR_6 = 6,
    SR_SELID_10_INDEX_FXST_8 = 8,
    SR_SELID_10_INDEX_FXINFO_9 = 9,
    SR_SELID_10_INDEX_FXCFG_10 = 10,
    SR_SELID_10_INDEX_FXXC_12 = 12,
    SR_SELID_10_INDEX_FXXP_13 = 13
  } SR_SELID_10_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_11 register group
   */
  typedef enum
  {
    SR_SELID_11_INDEX_TSCOUNTL_0 = 0,
    SR_SELID_11_INDEX_TSCOUNTH_1 = 1,
    SR_SELID_11_INDEX_TSCTRL_2 = 2,
    SR_SELID_11_INDEX_PMUMCTRL_8 = 8,
    SR_SELID_11_INDEX_PMGMCTRL_9 = 9,
    SR_SELID_11_INDEX_PMCOUNT0_16 = 16,
    SR_SELID_11_INDEX_PMCTRL0_17 = 17,
    SR_SELID_11_INDEX_PMCOUNT1_18 = 18,
    SR_SELID_11_INDEX_PMCTRL1_19 = 19,
    SR_SELID_11_INDEX_PMCOUNT2_20 = 20,
    SR_SELID_11_INDEX_PMCTRL2_21 = 21,
    SR_SELID_11_INDEX_PMCOUNT3_22 = 22,
    SR_SELID_11_INDEX_PMCTRL3_23 = 23
  } SR_SELID_11_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_12 register group
   */
  typedef enum
  {
    SR_SELID_12_INDEX_LSTEST0_0 = 0,
    SR_SELID_12_INDEX_LSTEST1_1 = 1,
    SR_SELID_12_INDEX_LSCFG_2 = 2,
    SR_SELID_12_INDEX_ICBKEY_4 = 4,
    SR_SELID_12_INDEX_IFCR_5 = 5,
    SR_SELID_12_INDEX_IFCR1_6 = 6,
    SR_SELID_12_INDEX_BRPCTRL0_8 = 8,
    SR_SELID_12_INDEX_BRPCTRL1_9 = 9,
    SR_SELID_12_INDEX_BRPCFG_10 = 10,
    SR_SELID_12_INDEX_BRPACTRL_11 = 11,
    SR_SELID_12_INDEX_BRPDATA_12 = 12
  } SR_SELID_12_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_13 register group
   */
  typedef enum
  {
    SR_SELID_13_INDEX_DCBKEY_0 = 0,
    SR_SELID_13_INDEX_LSUCR_1 = 1,
    SR_SELID_13_INDEX_LSULNK0_2 = 2,
    SR_SELID_13_INDEX_LSULNK1_3 = 3,
    SR_SELID_13_INDEX_L1RLCR_8 = 8,
    SR_SELID_13_INDEX_L1RLNK0_9 = 9,
    SR_SELID_13_INDEX_L1RLNK1_10 = 10,
    SR_SELID_13_INDEX_L1RCFG_12 = 12,
    SR_SELID_13_INDEX_DECFG_16 = 16,
    SR_SELID_13_INDEX_DECTRL_17 = 17,
    SR_SELID_13_INDEX_DEVDS_18 = 18,
    SR_SELID_13_INDEX_RDBCR_24 = 24,
    SR_SELID_13_INDEX_RDBACR_25 = 25,
    SR_SELID_13_INDEX_RDBATAG_26 = 26,
    SR_SELID_13_INDEX_RDBADAT0_27 = 27,
    SR_SELID_13_INDEX_RDBADAT1_28 = 28,
    SR_SELID_13_INDEX_RDBADAT2_29 = 29,
    SR_SELID_13_INDEX_RDBADAT3_30 = 30,
    SR_SELID_13_INDEX_RDBSTAT_31 = 31
  } SR_SELID_13_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_14 register group
   */
  typedef enum
  {
    SR_SELID_14_INDEX_PMCTRL4_4 = 4,
    SR_SELID_14_INDEX_PMCTRL5_5 = 5,
    SR_SELID_14_INDEX_PMCTRL6_6 = 6,
    SR_SELID_14_INDEX_PMCTRL7_7 = 7,
    SR_SELID_14_INDEX_PMCOUNT4_20 = 20,
    SR_SELID_14_INDEX_PMCOUNT5_21 = 21,
    SR_SELID_14_INDEX_PMCOUNT6_22 = 22,
    SR_SELID_14_INDEX_PMCOUNT7_23 = 23
  } SR_SELID_14_index_t;

  /**
   * \brief Indices for individual registers in the rh850g4 SR_SELID_15 register group
   */
  typedef enum
  {
    SR_SELID_15_INDEX_PMSUBCND0_0 = 0,
    SR_SELID_15_INDEX_PMSUBCND1_1 = 1,
    SR_SELID_15_INDEX_PMSUBCND2_2 = 2,
    SR_SELID_15_INDEX_PMSUBCND3_3 = 3,
    SR_SELID_15_INDEX_PMSUBCND4_4 = 4,
    SR_SELID_15_INDEX_PMSUBCND5_5 = 5,
    SR_SELID_15_INDEX_PMSUBCND6_6 = 6,
    SR_SELID_15_INDEX_PMSUBCND7_7 = 7
  } SR_SELID_15_index_t;
  
  /**
   * \brief Interface providing immediate and generative operations on the
   * rh850g4 CPU state
   *
   * Also provides register name operations which are typically used to
   * generate instruction disassembly information
   */
  class cpu_state_if_t : public fastiss::cpu_state_base_if_t
  {
  public:
    /**
     * \defgroup AllCPUStateOperations CPU state operations
     * @{
     * \defgroup RegisterNameOperations Register name operations
     * @{
     * \defgroup IndividualRegisterNameOperations Individual register name operations
     * \defgroup RegisterGroupNameOperations Register group name operations
     * @}
     * \defgroup ImmediateCPUStateOperations Immediate CPU state operations
     * @{
     * \defgroup ImmediateIndividualRegisterReadOperations Immediate register and register field read operations
     * \defgroup ImmediateRegisterGroupReadOperations Immediate register group read operations
     * \defgroup ImmediateIndividualRegisterWriteOperations Immediate register and register field write operations
     * \defgroup ImmediateRegisterGroupWriteOperations Immediate register group write operations
     * \defgroup ImmediateIndividualRegisterIncrementOperations Immediate register and register field increment operations
     * \defgroup ImmediateRegisterGroupIncrementOperations Immediate register group increment operations
     * \defgroup ImmediateIndividualRegisterDecrementOperations Immediate register and register field decrement operations
     * \defgroup ImmediateRegisterGroupDecrementOperations Immediate register group decrement operations
     * @}
     * \defgroup GenerativeCPUStateOperations Generative CPU state operations
     * @{
     * \defgroup GenerativeIndividualRegisterReadOperations Generative register and register field read operations
     * \defgroup GenerativeRegisterGroupReadOperations Generative register group read operations
     * \defgroup GenerativeIndividualRegisterWriteOperations Generative register and register field write operations
     * \defgroup GenerativeRegisterGroupWriteOperations Generative register group write operations
     * \defgroup GenerativeIndividualRegisterIncrementOperations Generative register and register field increment operations
     * \defgroup GenerativeRegisterGroupIncrementOperations Generative register group increment operations
     * \defgroup GenerativeIndividualRegisterDecrementOperations Generative register and register field decrement operations
     * \defgroup GenerativeRegisterGroupDecrementOperations Generative register group decrement operations
     * @}
     * @}
     */
     
    /**
     * \addtogroup IndividualRegisterNameOperations
     * @{
     */
    /**
     * \brief Generates string representing name of the rh850g4 r0 register
     * \return String representing the name of the rh850g4 r0 register
     */
    virtual std::string register_name_r0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r1 register
     * \return String representing the name of the rh850g4 r1 register
     */
    virtual std::string register_name_r1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r2 register
     * \return String representing the name of the rh850g4 r2 register
     */
    virtual std::string register_name_r2(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r3 register
     * \return String representing the name of the rh850g4 r3 register
     */
    virtual std::string register_name_r3(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r4 register
     * \return String representing the name of the rh850g4 r4 register
     */
    virtual std::string register_name_r4(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r5 register
     * \return String representing the name of the rh850g4 r5 register
     */
    virtual std::string register_name_r5(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r6 register
     * \return String representing the name of the rh850g4 r6 register
     */
    virtual std::string register_name_r6(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r7 register
     * \return String representing the name of the rh850g4 r7 register
     */
    virtual std::string register_name_r7(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r8 register
     * \return String representing the name of the rh850g4 r8 register
     */
    virtual std::string register_name_r8(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r9 register
     * \return String representing the name of the rh850g4 r9 register
     */
    virtual std::string register_name_r9(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r10 register
     * \return String representing the name of the rh850g4 r10 register
     */
    virtual std::string register_name_r10(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r11 register
     * \return String representing the name of the rh850g4 r11 register
     */
    virtual std::string register_name_r11(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r12 register
     * \return String representing the name of the rh850g4 r12 register
     */
    virtual std::string register_name_r12(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r13 register
     * \return String representing the name of the rh850g4 r13 register
     */
    virtual std::string register_name_r13(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r14 register
     * \return String representing the name of the rh850g4 r14 register
     */
    virtual std::string register_name_r14(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r15 register
     * \return String representing the name of the rh850g4 r15 register
     */
    virtual std::string register_name_r15(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r16 register
     * \return String representing the name of the rh850g4 r16 register
     */
    virtual std::string register_name_r16(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r17 register
     * \return String representing the name of the rh850g4 r17 register
     */
    virtual std::string register_name_r17(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r18 register
     * \return String representing the name of the rh850g4 r18 register
     */
    virtual std::string register_name_r18(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r19 register
     * \return String representing the name of the rh850g4 r19 register
     */
    virtual std::string register_name_r19(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r20 register
     * \return String representing the name of the rh850g4 r20 register
     */
    virtual std::string register_name_r20(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r21 register
     * \return String representing the name of the rh850g4 r21 register
     */
    virtual std::string register_name_r21(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r22 register
     * \return String representing the name of the rh850g4 r22 register
     */
    virtual std::string register_name_r22(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r23 register
     * \return String representing the name of the rh850g4 r23 register
     */
    virtual std::string register_name_r23(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r24 register
     * \return String representing the name of the rh850g4 r24 register
     */
    virtual std::string register_name_r24(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r25 register
     * \return String representing the name of the rh850g4 r25 register
     */
    virtual std::string register_name_r25(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r26 register
     * \return String representing the name of the rh850g4 r26 register
     */
    virtual std::string register_name_r26(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r27 register
     * \return String representing the name of the rh850g4 r27 register
     */
    virtual std::string register_name_r27(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r28 register
     * \return String representing the name of the rh850g4 r28 register
     */
    virtual std::string register_name_r28(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r29 register
     * \return String representing the name of the rh850g4 r29 register
     */
    virtual std::string register_name_r29(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r30 register
     * \return String representing the name of the rh850g4 r30 register
     */
    virtual std::string register_name_r30(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 r31 register
     * \return String representing the name of the rh850g4 r31 register
     */
    virtual std::string register_name_r31(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PC register
     * \return String representing the name of the rh850g4 PC register
     */
    virtual std::string register_name_PC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr0 register
     * \return String representing the name of the rh850g4 wr0 register
     */
    virtual std::string register_name_wr0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr0_u register
     * \return String representing the name of the rh850g4 wr0_u register
     */
    virtual std::string register_name_wr0_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr1 register
     * \return String representing the name of the rh850g4 wr1 register
     */
    virtual std::string register_name_wr1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr1_u register
     * \return String representing the name of the rh850g4 wr1_u register
     */
    virtual std::string register_name_wr1_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr2 register
     * \return String representing the name of the rh850g4 wr2 register
     */
    virtual std::string register_name_wr2(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr2_u register
     * \return String representing the name of the rh850g4 wr2_u register
     */
    virtual std::string register_name_wr2_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr3 register
     * \return String representing the name of the rh850g4 wr3 register
     */
    virtual std::string register_name_wr3(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr3_u register
     * \return String representing the name of the rh850g4 wr3_u register
     */
    virtual std::string register_name_wr3_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr4 register
     * \return String representing the name of the rh850g4 wr4 register
     */
    virtual std::string register_name_wr4(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr4_u register
     * \return String representing the name of the rh850g4 wr4_u register
     */
    virtual std::string register_name_wr4_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr5 register
     * \return String representing the name of the rh850g4 wr5 register
     */
    virtual std::string register_name_wr5(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr5_u register
     * \return String representing the name of the rh850g4 wr5_u register
     */
    virtual std::string register_name_wr5_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr6 register
     * \return String representing the name of the rh850g4 wr6 register
     */
    virtual std::string register_name_wr6(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr6_u register
     * \return String representing the name of the rh850g4 wr6_u register
     */
    virtual std::string register_name_wr6_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr7 register
     * \return String representing the name of the rh850g4 wr7 register
     */
    virtual std::string register_name_wr7(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr7_u register
     * \return String representing the name of the rh850g4 wr7_u register
     */
    virtual std::string register_name_wr7_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr8 register
     * \return String representing the name of the rh850g4 wr8 register
     */
    virtual std::string register_name_wr8(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr8_u register
     * \return String representing the name of the rh850g4 wr8_u register
     */
    virtual std::string register_name_wr8_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr9 register
     * \return String representing the name of the rh850g4 wr9 register
     */
    virtual std::string register_name_wr9(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr9_u register
     * \return String representing the name of the rh850g4 wr9_u register
     */
    virtual std::string register_name_wr9_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr10 register
     * \return String representing the name of the rh850g4 wr10 register
     */
    virtual std::string register_name_wr10(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr10_u register
     * \return String representing the name of the rh850g4 wr10_u register
     */
    virtual std::string register_name_wr10_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr11 register
     * \return String representing the name of the rh850g4 wr11 register
     */
    virtual std::string register_name_wr11(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr11_u register
     * \return String representing the name of the rh850g4 wr11_u register
     */
    virtual std::string register_name_wr11_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr12 register
     * \return String representing the name of the rh850g4 wr12 register
     */
    virtual std::string register_name_wr12(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr12_u register
     * \return String representing the name of the rh850g4 wr12_u register
     */
    virtual std::string register_name_wr12_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr13 register
     * \return String representing the name of the rh850g4 wr13 register
     */
    virtual std::string register_name_wr13(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr13_u register
     * \return String representing the name of the rh850g4 wr13_u register
     */
    virtual std::string register_name_wr13_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr14 register
     * \return String representing the name of the rh850g4 wr14 register
     */
    virtual std::string register_name_wr14(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr14_u register
     * \return String representing the name of the rh850g4 wr14_u register
     */
    virtual std::string register_name_wr14_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr15 register
     * \return String representing the name of the rh850g4 wr15 register
     */
    virtual std::string register_name_wr15(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr15_u register
     * \return String representing the name of the rh850g4 wr15_u register
     */
    virtual std::string register_name_wr15_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr16 register
     * \return String representing the name of the rh850g4 wr16 register
     */
    virtual std::string register_name_wr16(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr16_u register
     * \return String representing the name of the rh850g4 wr16_u register
     */
    virtual std::string register_name_wr16_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr17 register
     * \return String representing the name of the rh850g4 wr17 register
     */
    virtual std::string register_name_wr17(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr17_u register
     * \return String representing the name of the rh850g4 wr17_u register
     */
    virtual std::string register_name_wr17_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr18 register
     * \return String representing the name of the rh850g4 wr18 register
     */
    virtual std::string register_name_wr18(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr18_u register
     * \return String representing the name of the rh850g4 wr18_u register
     */
    virtual std::string register_name_wr18_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr19 register
     * \return String representing the name of the rh850g4 wr19 register
     */
    virtual std::string register_name_wr19(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr19_u register
     * \return String representing the name of the rh850g4 wr19_u register
     */
    virtual std::string register_name_wr19_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr20 register
     * \return String representing the name of the rh850g4 wr20 register
     */
    virtual std::string register_name_wr20(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr20_u register
     * \return String representing the name of the rh850g4 wr20_u register
     */
    virtual std::string register_name_wr20_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr21 register
     * \return String representing the name of the rh850g4 wr21 register
     */
    virtual std::string register_name_wr21(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr21_u register
     * \return String representing the name of the rh850g4 wr21_u register
     */
    virtual std::string register_name_wr21_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr22 register
     * \return String representing the name of the rh850g4 wr22 register
     */
    virtual std::string register_name_wr22(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr22_u register
     * \return String representing the name of the rh850g4 wr22_u register
     */
    virtual std::string register_name_wr22_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr23 register
     * \return String representing the name of the rh850g4 wr23 register
     */
    virtual std::string register_name_wr23(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr23_u register
     * \return String representing the name of the rh850g4 wr23_u register
     */
    virtual std::string register_name_wr23_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr24 register
     * \return String representing the name of the rh850g4 wr24 register
     */
    virtual std::string register_name_wr24(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr24_u register
     * \return String representing the name of the rh850g4 wr24_u register
     */
    virtual std::string register_name_wr24_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr25 register
     * \return String representing the name of the rh850g4 wr25 register
     */
    virtual std::string register_name_wr25(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr25_u register
     * \return String representing the name of the rh850g4 wr25_u register
     */
    virtual std::string register_name_wr25_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr26 register
     * \return String representing the name of the rh850g4 wr26 register
     */
    virtual std::string register_name_wr26(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr26_u register
     * \return String representing the name of the rh850g4 wr26_u register
     */
    virtual std::string register_name_wr26_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr27 register
     * \return String representing the name of the rh850g4 wr27 register
     */
    virtual std::string register_name_wr27(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr27_u register
     * \return String representing the name of the rh850g4 wr27_u register
     */
    virtual std::string register_name_wr27_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr28 register
     * \return String representing the name of the rh850g4 wr28 register
     */
    virtual std::string register_name_wr28(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr28_u register
     * \return String representing the name of the rh850g4 wr28_u register
     */
    virtual std::string register_name_wr28_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr29 register
     * \return String representing the name of the rh850g4 wr29 register
     */
    virtual std::string register_name_wr29(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr29_u register
     * \return String representing the name of the rh850g4 wr29_u register
     */
    virtual std::string register_name_wr29_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr30 register
     * \return String representing the name of the rh850g4 wr30 register
     */
    virtual std::string register_name_wr30(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr30_u register
     * \return String representing the name of the rh850g4 wr30_u register
     */
    virtual std::string register_name_wr30_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr31 register
     * \return String representing the name of the rh850g4 wr31 register
     */
    virtual std::string register_name_wr31(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 wr31_u register
     * \return String representing the name of the rh850g4 wr31_u register
     */
    virtual std::string register_name_wr31_u(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 EIPC register
     * \return String representing the name of the rh850g4 EIPC register
     */
    virtual std::string register_name_EIPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 EIPSW register
     * \return String representing the name of the rh850g4 EIPSW register
     */
    virtual std::string register_name_EIPSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FEPC register
     * \return String representing the name of the rh850g4 FEPC register
     */
    virtual std::string register_name_FEPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FEPSW register
     * \return String representing the name of the rh850g4 FEPSW register
     */
    virtual std::string register_name_FEPSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PSW register
     * \return String representing the name of the rh850g4 PSW register
     */
    virtual std::string register_name_PSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FPSR register
     * \return String representing the name of the rh850g4 FPSR register
     */
    virtual std::string register_name_FPSR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FPEPC register
     * \return String representing the name of the rh850g4 FPEPC register
     */
    virtual std::string register_name_FPEPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FPST register
     * \return String representing the name of the rh850g4 FPST register
     */
    virtual std::string register_name_FPST(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FPCC register
     * \return String representing the name of the rh850g4 FPCC register
     */
    virtual std::string register_name_FPCC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FPCFG register
     * \return String representing the name of the rh850g4 FPCFG register
     */
    virtual std::string register_name_FPCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 EIIC register
     * \return String representing the name of the rh850g4 EIIC register
     */
    virtual std::string register_name_EIIC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FEIC register
     * \return String representing the name of the rh850g4 FEIC register
     */
    virtual std::string register_name_FEIC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PSWH register
     * \return String representing the name of the rh850g4 PSWH register
     */
    virtual std::string register_name_PSWH(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 CTPC register
     * \return String representing the name of the rh850g4 CTPC register
     */
    virtual std::string register_name_CTPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 CTPSW register
     * \return String representing the name of the rh850g4 CTPSW register
     */
    virtual std::string register_name_CTPSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 EIPSWH register
     * \return String representing the name of the rh850g4 EIPSWH register
     */
    virtual std::string register_name_EIPSWH(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FEPSWH register
     * \return String representing the name of the rh850g4 FEPSWH register
     */
    virtual std::string register_name_FEPSWH(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 CTBP register
     * \return String representing the name of the rh850g4 CTBP register
     */
    virtual std::string register_name_CTBP(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 SNZCFG register
     * \return String representing the name of the rh850g4 SNZCFG register
     */
    virtual std::string register_name_SNZCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 EIWR register
     * \return String representing the name of the rh850g4 EIWR register
     */
    virtual std::string register_name_EIWR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FEWR register
     * \return String representing the name of the rh850g4 FEWR register
     */
    virtual std::string register_name_FEWR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 SPID register
     * \return String representing the name of the rh850g4 SPID register
     */
    virtual std::string register_name_SPID(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 SPIDLIST register
     * \return String representing the name of the rh850g4 SPIDLIST register
     */
    virtual std::string register_name_SPIDLIST(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RBASE register
     * \return String representing the name of the rh850g4 RBASE register
     */
    virtual std::string register_name_RBASE(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 EBASE register
     * \return String representing the name of the rh850g4 EBASE register
     */
    virtual std::string register_name_EBASE(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 INTBP register
     * \return String representing the name of the rh850g4 INTBP register
     */
    virtual std::string register_name_INTBP(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MCTL register
     * \return String representing the name of the rh850g4 MCTL register
     */
    virtual std::string register_name_MCTL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PID register
     * \return String representing the name of the rh850g4 PID register
     */
    virtual std::string register_name_PID(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 SVLOCK register
     * \return String representing the name of the rh850g4 SVLOCK register
     */
    virtual std::string register_name_SVLOCK(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 SCCFG register
     * \return String representing the name of the rh850g4 SCCFG register
     */
    virtual std::string register_name_SCCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 SCBP register
     * \return String representing the name of the rh850g4 SCBP register
     */
    virtual std::string register_name_SCBP(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 HVCFG register
     * \return String representing the name of the rh850g4 HVCFG register
     */
    virtual std::string register_name_HVCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMCFG register
     * \return String representing the name of the rh850g4 GMCFG register
     */
    virtual std::string register_name_GMCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 HVSB register
     * \return String representing the name of the rh850g4 HVSB register
     */
    virtual std::string register_name_HVSB(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PEID register
     * \return String representing the name of the rh850g4 PEID register
     */
    virtual std::string register_name_PEID(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BMID register
     * \return String representing the name of the rh850g4 BMID register
     */
    virtual std::string register_name_BMID(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MEA register
     * \return String representing the name of the rh850g4 MEA register
     */
    virtual std::string register_name_MEA(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MEI register
     * \return String representing the name of the rh850g4 MEI register
     */
    virtual std::string register_name_MEI(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RBCR0 register
     * \return String representing the name of the rh850g4 RBCR0 register
     */
    virtual std::string register_name_RBCR0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RBCR1 register
     * \return String representing the name of the rh850g4 RBCR1 register
     */
    virtual std::string register_name_RBCR1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RBNR register
     * \return String representing the name of the rh850g4 RBNR register
     */
    virtual std::string register_name_RBNR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RBIP register
     * \return String representing the name of the rh850g4 RBIP register
     */
    virtual std::string register_name_RBIP(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ISPR register
     * \return String representing the name of the rh850g4 ISPR register
     */
    virtual std::string register_name_ISPR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 IMSR register
     * \return String representing the name of the rh850g4 IMSR register
     */
    virtual std::string register_name_IMSR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICSR register
     * \return String representing the name of the rh850g4 ICSR register
     */
    virtual std::string register_name_ICSR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 INTCFG register
     * \return String representing the name of the rh850g4 INTCFG register
     */
    virtual std::string register_name_INTCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PLMR register
     * \return String representing the name of the rh850g4 PLMR register
     */
    virtual std::string register_name_PLMR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FXSR register
     * \return String representing the name of the rh850g4 FXSR register
     */
    virtual std::string register_name_FXSR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FXST register
     * \return String representing the name of the rh850g4 FXST register
     */
    virtual std::string register_name_FXST(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FXINFO register
     * \return String representing the name of the rh850g4 FXINFO register
     */
    virtual std::string register_name_FXINFO(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FXCFG register
     * \return String representing the name of the rh850g4 FXCFG register
     */
    virtual std::string register_name_FXCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FXXC register
     * \return String representing the name of the rh850g4 FXXC register
     */
    virtual std::string register_name_FXXC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 FXXP register
     * \return String representing the name of the rh850g4 FXXP register
     */
    virtual std::string register_name_FXXP(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMEIPC register
     * \return String representing the name of the rh850g4 GMEIPC register
     */
    virtual std::string register_name_GMEIPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMEIPSW register
     * \return String representing the name of the rh850g4 GMEIPSW register
     */
    virtual std::string register_name_GMEIPSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMFEPC register
     * \return String representing the name of the rh850g4 GMFEPC register
     */
    virtual std::string register_name_GMFEPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMFEPSW register
     * \return String representing the name of the rh850g4 GMFEPSW register
     */
    virtual std::string register_name_GMFEPSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMPSW register
     * \return String representing the name of the rh850g4 GMPSW register
     */
    virtual std::string register_name_GMPSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMMEA register
     * \return String representing the name of the rh850g4 GMMEA register
     */
    virtual std::string register_name_GMMEA(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMMEI register
     * \return String representing the name of the rh850g4 GMMEI register
     */
    virtual std::string register_name_GMMEI(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMEIIC register
     * \return String representing the name of the rh850g4 GMEIIC register
     */
    virtual std::string register_name_GMEIIC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMFEIC register
     * \return String representing the name of the rh850g4 GMFEIC register
     */
    virtual std::string register_name_GMFEIC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMSPID register
     * \return String representing the name of the rh850g4 GMSPID register
     */
    virtual std::string register_name_GMSPID(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMSPIDLIST register
     * \return String representing the name of the rh850g4 GMSPIDLIST register
     */
    virtual std::string register_name_GMSPIDLIST(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMEBASE register
     * \return String representing the name of the rh850g4 GMEBASE register
     */
    virtual std::string register_name_GMEBASE(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMINTBP register
     * \return String representing the name of the rh850g4 GMINTBP register
     */
    virtual std::string register_name_GMINTBP(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMINTCFG register
     * \return String representing the name of the rh850g4 GMINTCFG register
     */
    virtual std::string register_name_GMINTCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMPLMR register
     * \return String representing the name of the rh850g4 GMPLMR register
     */
    virtual std::string register_name_GMPLMR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMSVLOCK register
     * \return String representing the name of the rh850g4 GMSVLOCK register
     */
    virtual std::string register_name_GMSVLOCK(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMMPM register
     * \return String representing the name of the rh850g4 GMMPM register
     */
    virtual std::string register_name_GMMPM(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMEIWR register
     * \return String representing the name of the rh850g4 GMEIWR register
     */
    virtual std::string register_name_GMEIWR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMFEWR register
     * \return String representing the name of the rh850g4 GMFEWR register
     */
    virtual std::string register_name_GMFEWR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 GMPEID register
     * \return String representing the name of the rh850g4 GMPEID register
     */
    virtual std::string register_name_GMPEID(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPM register
     * \return String representing the name of the rh850g4 MPM register
     */
    virtual std::string register_name_MPM(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPCFG register
     * \return String representing the name of the rh850g4 MPCFG register
     */
    virtual std::string register_name_MPCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MCA register
     * \return String representing the name of the rh850g4 MCA register
     */
    virtual std::string register_name_MCA(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MCS register
     * \return String representing the name of the rh850g4 MCS register
     */
    virtual std::string register_name_MCS(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MCC register
     * \return String representing the name of the rh850g4 MCC register
     */
    virtual std::string register_name_MCC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MCR register
     * \return String representing the name of the rh850g4 MCR register
     */
    virtual std::string register_name_MCR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MCI register
     * \return String representing the name of the rh850g4 MCI register
     */
    virtual std::string register_name_MCI(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPIDX register
     * \return String representing the name of the rh850g4 MPIDX register
     */
    virtual std::string register_name_MPIDX(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPBK register
     * \return String representing the name of the rh850g4 MPBK register
     */
    virtual std::string register_name_MPBK(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPLA register
     * \return String representing the name of the rh850g4 MPLA register
     */
    virtual std::string register_name_MPLA(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPUA register
     * \return String representing the name of the rh850g4 MPUA register
     */
    virtual std::string register_name_MPUA(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPAT register
     * \return String representing the name of the rh850g4 MPAT register
     */
    virtual std::string register_name_MPAT(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID0 register
     * \return String representing the name of the rh850g4 MPID0 register
     */
    virtual std::string register_name_MPID0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID1 register
     * \return String representing the name of the rh850g4 MPID1 register
     */
    virtual std::string register_name_MPID1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID2 register
     * \return String representing the name of the rh850g4 MPID2 register
     */
    virtual std::string register_name_MPID2(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID3 register
     * \return String representing the name of the rh850g4 MPID3 register
     */
    virtual std::string register_name_MPID3(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID4 register
     * \return String representing the name of the rh850g4 MPID4 register
     */
    virtual std::string register_name_MPID4(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID5 register
     * \return String representing the name of the rh850g4 MPID5 register
     */
    virtual std::string register_name_MPID5(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID6 register
     * \return String representing the name of the rh850g4 MPID6 register
     */
    virtual std::string register_name_MPID6(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 MPID7 register
     * \return String representing the name of the rh850g4 MPID7 register
     */
    virtual std::string register_name_MPID7(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICTAGL register
     * \return String representing the name of the rh850g4 ICTAGL register
     */
    virtual std::string register_name_ICTAGL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICTAGH register
     * \return String representing the name of the rh850g4 ICTAGH register
     */
    virtual std::string register_name_ICTAGH(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICDATL register
     * \return String representing the name of the rh850g4 ICDATL register
     */
    virtual std::string register_name_ICDATL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICDATH register
     * \return String representing the name of the rh850g4 ICDATH register
     */
    virtual std::string register_name_ICDATH(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICCTRL register
     * \return String representing the name of the rh850g4 ICCTRL register
     */
    virtual std::string register_name_ICCTRL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICCFG register
     * \return String representing the name of the rh850g4 ICCFG register
     */
    virtual std::string register_name_ICCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICERR register
     * \return String representing the name of the rh850g4 ICERR register
     */
    virtual std::string register_name_ICERR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 TSCOUNTL register
     * \return String representing the name of the rh850g4 TSCOUNTL register
     */
    virtual std::string register_name_TSCOUNTL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 TSCOUNTH register
     * \return String representing the name of the rh850g4 TSCOUNTH register
     */
    virtual std::string register_name_TSCOUNTH(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 TSCTRL register
     * \return String representing the name of the rh850g4 TSCTRL register
     */
    virtual std::string register_name_TSCTRL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMUMCTRL register
     * \return String representing the name of the rh850g4 PMUMCTRL register
     */
    virtual std::string register_name_PMUMCTRL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMGMCTRL register
     * \return String representing the name of the rh850g4 PMGMCTRL register
     */
    virtual std::string register_name_PMGMCTRL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT0 register
     * \return String representing the name of the rh850g4 PMCOUNT0 register
     */
    virtual std::string register_name_PMCOUNT0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL0 register
     * \return String representing the name of the rh850g4 PMCTRL0 register
     */
    virtual std::string register_name_PMCTRL0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT1 register
     * \return String representing the name of the rh850g4 PMCOUNT1 register
     */
    virtual std::string register_name_PMCOUNT1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL1 register
     * \return String representing the name of the rh850g4 PMCTRL1 register
     */
    virtual std::string register_name_PMCTRL1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT2 register
     * \return String representing the name of the rh850g4 PMCOUNT2 register
     */
    virtual std::string register_name_PMCOUNT2(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL2 register
     * \return String representing the name of the rh850g4 PMCTRL2 register
     */
    virtual std::string register_name_PMCTRL2(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT3 register
     * \return String representing the name of the rh850g4 PMCOUNT3 register
     */
    virtual std::string register_name_PMCOUNT3(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL3 register
     * \return String representing the name of the rh850g4 PMCTRL3 register
     */
    virtual std::string register_name_PMCTRL3(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL4 register
     * \return String representing the name of the rh850g4 PMCTRL4 register
     */
    virtual std::string register_name_PMCTRL4(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL5 register
     * \return String representing the name of the rh850g4 PMCTRL5 register
     */
    virtual std::string register_name_PMCTRL5(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL6 register
     * \return String representing the name of the rh850g4 PMCTRL6 register
     */
    virtual std::string register_name_PMCTRL6(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL7 register
     * \return String representing the name of the rh850g4 PMCTRL7 register
     */
    virtual std::string register_name_PMCTRL7(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT4 register
     * \return String representing the name of the rh850g4 PMCOUNT4 register
     */
    virtual std::string register_name_PMCOUNT4(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT5 register
     * \return String representing the name of the rh850g4 PMCOUNT5 register
     */
    virtual std::string register_name_PMCOUNT5(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT6 register
     * \return String representing the name of the rh850g4 PMCOUNT6 register
     */
    virtual std::string register_name_PMCOUNT6(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT7 register
     * \return String representing the name of the rh850g4 PMCOUNT7 register
     */
    virtual std::string register_name_PMCOUNT7(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND0 register
     * \return String representing the name of the rh850g4 PMSUBCND0 register
     */
    virtual std::string register_name_PMSUBCND0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND1 register
     * \return String representing the name of the rh850g4 PMSUBCND1 register
     */
    virtual std::string register_name_PMSUBCND1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND2 register
     * \return String representing the name of the rh850g4 PMSUBCND2 register
     */
    virtual std::string register_name_PMSUBCND2(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND3 register
     * \return String representing the name of the rh850g4 PMSUBCND3 register
     */
    virtual std::string register_name_PMSUBCND3(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND4 register
     * \return String representing the name of the rh850g4 PMSUBCND4 register
     */
    virtual std::string register_name_PMSUBCND4(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND5 register
     * \return String representing the name of the rh850g4 PMSUBCND5 register
     */
    virtual std::string register_name_PMSUBCND5(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND6 register
     * \return String representing the name of the rh850g4 PMSUBCND6 register
     */
    virtual std::string register_name_PMSUBCND6(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND7 register
     * \return String representing the name of the rh850g4 PMSUBCND7 register
     */
    virtual std::string register_name_PMSUBCND7(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DBGEN register
     * \return String representing the name of the rh850g4 DBGEN register
     */
    virtual std::string register_name_DBGEN(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DBPSWH register
     * \return String representing the name of the rh850g4 DBPSWH register
     */
    virtual std::string register_name_DBPSWH(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DBIC register
     * \return String representing the name of the rh850g4 DBIC register
     */
    virtual std::string register_name_DBIC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DBPC register
     * \return String representing the name of the rh850g4 DBPC register
     */
    virtual std::string register_name_DBPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DBPSW register
     * \return String representing the name of the rh850g4 DBPSW register
     */
    virtual std::string register_name_DBPSW(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DIR0 register
     * \return String representing the name of the rh850g4 DIR0 register
     */
    virtual std::string register_name_DIR0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DIR1 register
     * \return String representing the name of the rh850g4 DIR1 register
     */
    virtual std::string register_name_DIR1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BPC register
     * \return String representing the name of the rh850g4 BPC register
     */
    virtual std::string register_name_BPC(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BPAV register
     * \return String representing the name of the rh850g4 BPAV register
     */
    virtual std::string register_name_BPAV(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BPAM register
     * \return String representing the name of the rh850g4 BPAM register
     */
    virtual std::string register_name_BPAM(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DBCFG register
     * \return String representing the name of the rh850g4 DBCFG register
     */
    virtual std::string register_name_DBCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DBWR register
     * \return String representing the name of the rh850g4 DBWR register
     */
    virtual std::string register_name_DBWR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 LSTEST0 register
     * \return String representing the name of the rh850g4 LSTEST0 register
     */
    virtual std::string register_name_LSTEST0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 LSTEST1 register
     * \return String representing the name of the rh850g4 LSTEST1 register
     */
    virtual std::string register_name_LSTEST1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 LSCFG register
     * \return String representing the name of the rh850g4 LSCFG register
     */
    virtual std::string register_name_LSCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 ICBKEY register
     * \return String representing the name of the rh850g4 ICBKEY register
     */
    virtual std::string register_name_ICBKEY(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 IFCR register
     * \return String representing the name of the rh850g4 IFCR register
     */
    virtual std::string register_name_IFCR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 IFCR1 register
     * \return String representing the name of the rh850g4 IFCR1 register
     */
    virtual std::string register_name_IFCR1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BRPCTRL0 register
     * \return String representing the name of the rh850g4 BRPCTRL0 register
     */
    virtual std::string register_name_BRPCTRL0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BRPCTRL1 register
     * \return String representing the name of the rh850g4 BRPCTRL1 register
     */
    virtual std::string register_name_BRPCTRL1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BRPCFG register
     * \return String representing the name of the rh850g4 BRPCFG register
     */
    virtual std::string register_name_BRPCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BRPACTRL register
     * \return String representing the name of the rh850g4 BRPACTRL register
     */
    virtual std::string register_name_BRPACTRL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 BRPDATA register
     * \return String representing the name of the rh850g4 BRPDATA register
     */
    virtual std::string register_name_BRPDATA(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DCBKEY register
     * \return String representing the name of the rh850g4 DCBKEY register
     */
    virtual std::string register_name_DCBKEY(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 LSUCR register
     * \return String representing the name of the rh850g4 LSUCR register
     */
    virtual std::string register_name_LSUCR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 LSULNK0 register
     * \return String representing the name of the rh850g4 LSULNK0 register
     */
    virtual std::string register_name_LSULNK0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 LSULNK1 register
     * \return String representing the name of the rh850g4 LSULNK1 register
     */
    virtual std::string register_name_LSULNK1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 L1RLCR register
     * \return String representing the name of the rh850g4 L1RLCR register
     */
    virtual std::string register_name_L1RLCR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 L1RLNK0 register
     * \return String representing the name of the rh850g4 L1RLNK0 register
     */
    virtual std::string register_name_L1RLNK0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 L1RLNK1 register
     * \return String representing the name of the rh850g4 L1RLNK1 register
     */
    virtual std::string register_name_L1RLNK1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 L1RCFG register
     * \return String representing the name of the rh850g4 L1RCFG register
     */
    virtual std::string register_name_L1RCFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DECFG register
     * \return String representing the name of the rh850g4 DECFG register
     */
    virtual std::string register_name_DECFG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DECTRL register
     * \return String representing the name of the rh850g4 DECTRL register
     */
    virtual std::string register_name_DECTRL(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 DEVDS register
     * \return String representing the name of the rh850g4 DEVDS register
     */
    virtual std::string register_name_DEVDS(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBCR register
     * \return String representing the name of the rh850g4 RDBCR register
     */
    virtual std::string register_name_RDBCR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBACR register
     * \return String representing the name of the rh850g4 RDBACR register
     */
    virtual std::string register_name_RDBACR(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBATAG register
     * \return String representing the name of the rh850g4 RDBATAG register
     */
    virtual std::string register_name_RDBATAG(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT0 register
     * \return String representing the name of the rh850g4 RDBADAT0 register
     */
    virtual std::string register_name_RDBADAT0(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT1 register
     * \return String representing the name of the rh850g4 RDBADAT1 register
     */
    virtual std::string register_name_RDBADAT1(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT2 register
     * \return String representing the name of the rh850g4 RDBADAT2 register
     */
    virtual std::string register_name_RDBADAT2(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT3 register
     * \return String representing the name of the rh850g4 RDBADAT3 register
     */
    virtual std::string register_name_RDBADAT3(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 RDBSTAT register
     * \return String representing the name of the rh850g4 RDBSTAT register
     */
    virtual std::string register_name_RDBSTAT(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 hv_func_support register
     * \return String representing the name of the rh850g4 hv_func_support register
     */
    virtual std::string register_name_hv_func_support(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 swd0pcad_exec_count register
     * \return String representing the name of the rh850g4 swd0pcad_exec_count register
     */
    virtual std::string register_name_swd0pcad_exec_count(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 swd0pcad_val register
     * \return String representing the name of the rh850g4 swd0pcad_val register
     */
    virtual std::string register_name_swd0pcad_val(void) = 0;
    /**
     * \brief Generates string representing name of the rh850g4 mem_acc_type register
     * \return String representing the name of the rh850g4 mem_acc_type register
     */
    virtual std::string register_name_mem_acc_type(void) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup RegisterGroupNameOperations
     * @{
     */
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return String representing the name of the specified rh850g4 GR group register
     */
    virtual std::string register_name_GR(rh850g4::GR_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return String representing the name of the specified rh850g4 WR group register
     */
    virtual std::string register_name_WR(rh850g4::WR_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_0 group register
     */
    virtual std::string register_name_SR_SELID_0(rh850g4::SR_SELID_0_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_1 group register
     */
    virtual std::string register_name_SR_SELID_1(rh850g4::SR_SELID_1_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_2 group register
     */
    virtual std::string register_name_SR_SELID_2(rh850g4::SR_SELID_2_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_3 group register
     */
    virtual std::string register_name_SR_SELID_3(rh850g4::SR_SELID_3_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_4 group register
     */
    virtual std::string register_name_SR_SELID_4(rh850g4::SR_SELID_4_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_5 group register
     */
    virtual std::string register_name_SR_SELID_5(rh850g4::SR_SELID_5_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_9 group register
     */
    virtual std::string register_name_SR_SELID_9(rh850g4::SR_SELID_9_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_10 group register
     */
    virtual std::string register_name_SR_SELID_10(rh850g4::SR_SELID_10_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_11 group register
     */
    virtual std::string register_name_SR_SELID_11(rh850g4::SR_SELID_11_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_12 group register
     */
    virtual std::string register_name_SR_SELID_12(rh850g4::SR_SELID_12_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_13 group register
     */
    virtual std::string register_name_SR_SELID_13(rh850g4::SR_SELID_13_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_14 group register
     */
    virtual std::string register_name_SR_SELID_14(rh850g4::SR_SELID_14_index_t index) = 0;
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_15 group register
     */
    virtual std::string register_name_SR_SELID_15(rh850g4::SR_SELID_15_index_t index) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateIndividualRegisterReadOperations
     * @{
     */
    /**
     * \brief Queries the value of the rh850g4 translation time PC register
     * \return Value of the rh850g4 translation time PC register
     */
    virtual unsigned int read_trans_PC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r0 register
     * \return Value of the rh850g4 r0 register
     */
    virtual unsigned int read_r0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r1 register
     * \return Value of the rh850g4 r1 register
     */
    virtual unsigned int read_r1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r2 register
     * \return Value of the rh850g4 r2 register
     */
    virtual unsigned int read_r2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r3 register
     * \return Value of the rh850g4 r3 register
     */
    virtual unsigned int read_r3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r4 register
     * \return Value of the rh850g4 r4 register
     */
    virtual unsigned int read_r4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r5 register
     * \return Value of the rh850g4 r5 register
     */
    virtual unsigned int read_r5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r6 register
     * \return Value of the rh850g4 r6 register
     */
    virtual unsigned int read_r6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r7 register
     * \return Value of the rh850g4 r7 register
     */
    virtual unsigned int read_r7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r8 register
     * \return Value of the rh850g4 r8 register
     */
    virtual unsigned int read_r8(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r9 register
     * \return Value of the rh850g4 r9 register
     */
    virtual unsigned int read_r9(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r10 register
     * \return Value of the rh850g4 r10 register
     */
    virtual unsigned int read_r10(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r11 register
     * \return Value of the rh850g4 r11 register
     */
    virtual unsigned int read_r11(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r12 register
     * \return Value of the rh850g4 r12 register
     */
    virtual unsigned int read_r12(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r13 register
     * \return Value of the rh850g4 r13 register
     */
    virtual unsigned int read_r13(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r14 register
     * \return Value of the rh850g4 r14 register
     */
    virtual unsigned int read_r14(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r15 register
     * \return Value of the rh850g4 r15 register
     */
    virtual unsigned int read_r15(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r16 register
     * \return Value of the rh850g4 r16 register
     */
    virtual unsigned int read_r16(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r17 register
     * \return Value of the rh850g4 r17 register
     */
    virtual unsigned int read_r17(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r18 register
     * \return Value of the rh850g4 r18 register
     */
    virtual unsigned int read_r18(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r19 register
     * \return Value of the rh850g4 r19 register
     */
    virtual unsigned int read_r19(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r20 register
     * \return Value of the rh850g4 r20 register
     */
    virtual unsigned int read_r20(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r21 register
     * \return Value of the rh850g4 r21 register
     */
    virtual unsigned int read_r21(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r22 register
     * \return Value of the rh850g4 r22 register
     */
    virtual unsigned int read_r22(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r23 register
     * \return Value of the rh850g4 r23 register
     */
    virtual unsigned int read_r23(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r24 register
     * \return Value of the rh850g4 r24 register
     */
    virtual unsigned int read_r24(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r25 register
     * \return Value of the rh850g4 r25 register
     */
    virtual unsigned int read_r25(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r26 register
     * \return Value of the rh850g4 r26 register
     */
    virtual unsigned int read_r26(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r27 register
     * \return Value of the rh850g4 r27 register
     */
    virtual unsigned int read_r27(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r28 register
     * \return Value of the rh850g4 r28 register
     */
    virtual unsigned int read_r28(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r29 register
     * \return Value of the rh850g4 r29 register
     */
    virtual unsigned int read_r29(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r30 register
     * \return Value of the rh850g4 r30 register
     */
    virtual unsigned int read_r30(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 r31 register
     * \return Value of the rh850g4 r31 register
     */
    virtual unsigned int read_r31(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PC register
     * \return Value of the rh850g4 PC register
     */
    virtual unsigned int read_PC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr0 register
     * \return Value of the rh850g4 wr0 register
     */
    virtual unsigned long long read_wr0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr0_u register
     * \return Value of the rh850g4 wr0_u register
     */
    virtual unsigned long long read_wr0_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr1 register
     * \return Value of the rh850g4 wr1 register
     */
    virtual unsigned long long read_wr1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr1_u register
     * \return Value of the rh850g4 wr1_u register
     */
    virtual unsigned long long read_wr1_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr2 register
     * \return Value of the rh850g4 wr2 register
     */
    virtual unsigned long long read_wr2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr2_u register
     * \return Value of the rh850g4 wr2_u register
     */
    virtual unsigned long long read_wr2_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr3 register
     * \return Value of the rh850g4 wr3 register
     */
    virtual unsigned long long read_wr3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr3_u register
     * \return Value of the rh850g4 wr3_u register
     */
    virtual unsigned long long read_wr3_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr4 register
     * \return Value of the rh850g4 wr4 register
     */
    virtual unsigned long long read_wr4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr4_u register
     * \return Value of the rh850g4 wr4_u register
     */
    virtual unsigned long long read_wr4_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr5 register
     * \return Value of the rh850g4 wr5 register
     */
    virtual unsigned long long read_wr5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr5_u register
     * \return Value of the rh850g4 wr5_u register
     */
    virtual unsigned long long read_wr5_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr6 register
     * \return Value of the rh850g4 wr6 register
     */
    virtual unsigned long long read_wr6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr6_u register
     * \return Value of the rh850g4 wr6_u register
     */
    virtual unsigned long long read_wr6_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr7 register
     * \return Value of the rh850g4 wr7 register
     */
    virtual unsigned long long read_wr7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr7_u register
     * \return Value of the rh850g4 wr7_u register
     */
    virtual unsigned long long read_wr7_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr8 register
     * \return Value of the rh850g4 wr8 register
     */
    virtual unsigned long long read_wr8(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr8_u register
     * \return Value of the rh850g4 wr8_u register
     */
    virtual unsigned long long read_wr8_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr9 register
     * \return Value of the rh850g4 wr9 register
     */
    virtual unsigned long long read_wr9(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr9_u register
     * \return Value of the rh850g4 wr9_u register
     */
    virtual unsigned long long read_wr9_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr10 register
     * \return Value of the rh850g4 wr10 register
     */
    virtual unsigned long long read_wr10(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr10_u register
     * \return Value of the rh850g4 wr10_u register
     */
    virtual unsigned long long read_wr10_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr11 register
     * \return Value of the rh850g4 wr11 register
     */
    virtual unsigned long long read_wr11(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr11_u register
     * \return Value of the rh850g4 wr11_u register
     */
    virtual unsigned long long read_wr11_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr12 register
     * \return Value of the rh850g4 wr12 register
     */
    virtual unsigned long long read_wr12(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr12_u register
     * \return Value of the rh850g4 wr12_u register
     */
    virtual unsigned long long read_wr12_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr13 register
     * \return Value of the rh850g4 wr13 register
     */
    virtual unsigned long long read_wr13(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr13_u register
     * \return Value of the rh850g4 wr13_u register
     */
    virtual unsigned long long read_wr13_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr14 register
     * \return Value of the rh850g4 wr14 register
     */
    virtual unsigned long long read_wr14(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr14_u register
     * \return Value of the rh850g4 wr14_u register
     */
    virtual unsigned long long read_wr14_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr15 register
     * \return Value of the rh850g4 wr15 register
     */
    virtual unsigned long long read_wr15(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr15_u register
     * \return Value of the rh850g4 wr15_u register
     */
    virtual unsigned long long read_wr15_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr16 register
     * \return Value of the rh850g4 wr16 register
     */
    virtual unsigned long long read_wr16(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr16_u register
     * \return Value of the rh850g4 wr16_u register
     */
    virtual unsigned long long read_wr16_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr17 register
     * \return Value of the rh850g4 wr17 register
     */
    virtual unsigned long long read_wr17(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr17_u register
     * \return Value of the rh850g4 wr17_u register
     */
    virtual unsigned long long read_wr17_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr18 register
     * \return Value of the rh850g4 wr18 register
     */
    virtual unsigned long long read_wr18(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr18_u register
     * \return Value of the rh850g4 wr18_u register
     */
    virtual unsigned long long read_wr18_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr19 register
     * \return Value of the rh850g4 wr19 register
     */
    virtual unsigned long long read_wr19(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr19_u register
     * \return Value of the rh850g4 wr19_u register
     */
    virtual unsigned long long read_wr19_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr20 register
     * \return Value of the rh850g4 wr20 register
     */
    virtual unsigned long long read_wr20(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr20_u register
     * \return Value of the rh850g4 wr20_u register
     */
    virtual unsigned long long read_wr20_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr21 register
     * \return Value of the rh850g4 wr21 register
     */
    virtual unsigned long long read_wr21(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr21_u register
     * \return Value of the rh850g4 wr21_u register
     */
    virtual unsigned long long read_wr21_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr22 register
     * \return Value of the rh850g4 wr22 register
     */
    virtual unsigned long long read_wr22(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr22_u register
     * \return Value of the rh850g4 wr22_u register
     */
    virtual unsigned long long read_wr22_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr23 register
     * \return Value of the rh850g4 wr23 register
     */
    virtual unsigned long long read_wr23(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr23_u register
     * \return Value of the rh850g4 wr23_u register
     */
    virtual unsigned long long read_wr23_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr24 register
     * \return Value of the rh850g4 wr24 register
     */
    virtual unsigned long long read_wr24(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr24_u register
     * \return Value of the rh850g4 wr24_u register
     */
    virtual unsigned long long read_wr24_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr25 register
     * \return Value of the rh850g4 wr25 register
     */
    virtual unsigned long long read_wr25(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr25_u register
     * \return Value of the rh850g4 wr25_u register
     */
    virtual unsigned long long read_wr25_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr26 register
     * \return Value of the rh850g4 wr26 register
     */
    virtual unsigned long long read_wr26(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr26_u register
     * \return Value of the rh850g4 wr26_u register
     */
    virtual unsigned long long read_wr26_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr27 register
     * \return Value of the rh850g4 wr27 register
     */
    virtual unsigned long long read_wr27(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr27_u register
     * \return Value of the rh850g4 wr27_u register
     */
    virtual unsigned long long read_wr27_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr28 register
     * \return Value of the rh850g4 wr28 register
     */
    virtual unsigned long long read_wr28(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr28_u register
     * \return Value of the rh850g4 wr28_u register
     */
    virtual unsigned long long read_wr28_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr29 register
     * \return Value of the rh850g4 wr29 register
     */
    virtual unsigned long long read_wr29(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr29_u register
     * \return Value of the rh850g4 wr29_u register
     */
    virtual unsigned long long read_wr29_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr30 register
     * \return Value of the rh850g4 wr30 register
     */
    virtual unsigned long long read_wr30(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr30_u register
     * \return Value of the rh850g4 wr30_u register
     */
    virtual unsigned long long read_wr30_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr31 register
     * \return Value of the rh850g4 wr31 register
     */
    virtual unsigned long long read_wr31(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 wr31_u register
     * \return Value of the rh850g4 wr31_u register
     */
    virtual unsigned long long read_wr31_u(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EIPC register
     * \return Value of the rh850g4 EIPC register
     */
    virtual unsigned int read_EIPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EIPSW register
     * \return Value of the rh850g4 EIPSW register
     */
    virtual unsigned int read_EIPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FEPC register
     * \return Value of the rh850g4 FEPC register
     */
    virtual unsigned int read_FEPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FEPSW register
     * \return Value of the rh850g4 FEPSW register
     */
    virtual unsigned int read_FEPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW register
     * \return Value of the rh850g4 PSW register
     */
    virtual unsigned int read_PSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_Z register field
     * \return Value of the rh850g4 PSW_Z register field
     */
    virtual unsigned char read_PSW_Z(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_S register field
     * \return Value of the rh850g4 PSW_S register field
     */
    virtual unsigned char read_PSW_S(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_OV register field
     * \return Value of the rh850g4 PSW_OV register field
     */
    virtual unsigned char read_PSW_OV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_CY register field
     * \return Value of the rh850g4 PSW_CY register field
     */
    virtual unsigned char read_PSW_CY(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_SAT register field
     * \return Value of the rh850g4 PSW_SAT register field
     */
    virtual unsigned char read_PSW_SAT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_ID register field
     * \return Value of the rh850g4 PSW_ID register field
     */
    virtual unsigned char read_PSW_ID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_EP register field
     * \return Value of the rh850g4 PSW_EP register field
     */
    virtual unsigned char read_PSW_EP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_NP register field
     * \return Value of the rh850g4 PSW_NP register field
     */
    virtual unsigned char read_PSW_NP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_EBV register field
     * \return Value of the rh850g4 PSW_EBV register field
     */
    virtual unsigned char read_PSW_EBV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_CU0 register field
     * \return Value of the rh850g4 PSW_CU0 register field
     */
    virtual unsigned char read_PSW_CU0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_CU1 register field
     * \return Value of the rh850g4 PSW_CU1 register field
     */
    virtual unsigned char read_PSW_CU1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_CU2 register field
     * \return Value of the rh850g4 PSW_CU2 register field
     */
    virtual unsigned char read_PSW_CU2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_EIMASK register field
     * \return Value of the rh850g4 PSW_EIMASK register field
     */
    virtual unsigned char read_PSW_EIMASK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSW_UM register field
     * \return Value of the rh850g4 PSW_UM register field
     */
    virtual unsigned char read_PSW_UM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR register
     * \return Value of the rh850g4 FPSR register
     */
    virtual unsigned int read_FPSR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_CC register field
     * \return Value of the rh850g4 FPSR_CC register field
     */
    virtual unsigned char read_FPSR_CC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_FN register field
     * \return Value of the rh850g4 FPSR_FN register field
     */
    virtual unsigned char read_FPSR_FN(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_IF register field
     * \return Value of the rh850g4 FPSR_IF register field
     */
    virtual unsigned char read_FPSR_IF(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_PEM register field
     * \return Value of the rh850g4 FPSR_PEM register field
     */
    virtual unsigned char read_FPSR_PEM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_RM register field
     * \return Value of the rh850g4 FPSR_RM register field
     */
    virtual unsigned char read_FPSR_RM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_FS register field
     * \return Value of the rh850g4 FPSR_FS register field
     */
    virtual unsigned char read_FPSR_FS(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_XC register field
     * \return Value of the rh850g4 FPSR_XC register field
     */
    virtual unsigned char read_FPSR_XC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_XE register field
     * \return Value of the rh850g4 FPSR_XE register field
     */
    virtual unsigned char read_FPSR_XE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPSR_XP register field
     * \return Value of the rh850g4 FPSR_XP register field
     */
    virtual unsigned char read_FPSR_XP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPEPC register
     * \return Value of the rh850g4 FPEPC register
     */
    virtual unsigned int read_FPEPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPST register
     * \return Value of the rh850g4 FPST register
     */
    virtual unsigned int read_FPST(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPST_XC register field
     * \return Value of the rh850g4 FPST_XC register field
     */
    virtual unsigned char read_FPST_XC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPST_IF register field
     * \return Value of the rh850g4 FPST_IF register field
     */
    virtual unsigned char read_FPST_IF(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPST_XP register field
     * \return Value of the rh850g4 FPST_XP register field
     */
    virtual unsigned char read_FPST_XP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPCC register
     * \return Value of the rh850g4 FPCC register
     */
    virtual unsigned int read_FPCC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPCC_CC register field
     * \return Value of the rh850g4 FPCC_CC register field
     */
    virtual unsigned char read_FPCC_CC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPCFG register
     * \return Value of the rh850g4 FPCFG register
     */
    virtual unsigned int read_FPCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPCFG_RM register field
     * \return Value of the rh850g4 FPCFG_RM register field
     */
    virtual unsigned char read_FPCFG_RM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPCFG_XE register field
     * \return Value of the rh850g4 FPCFG_XE register field
     */
    virtual unsigned char read_FPCFG_XE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EIIC register
     * \return Value of the rh850g4 EIIC register
     */
    virtual unsigned int read_EIIC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FEIC register
     * \return Value of the rh850g4 FEIC register
     */
    virtual unsigned int read_FEIC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSWH register
     * \return Value of the rh850g4 PSWH register
     */
    virtual unsigned int read_PSWH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSWH_GPID register field
     * \return Value of the rh850g4 PSWH_GPID register field
     */
    virtual unsigned char read_PSWH_GPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PSWH_GM register field
     * \return Value of the rh850g4 PSWH_GM register field
     */
    virtual unsigned char read_PSWH_GM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 CTPC register
     * \return Value of the rh850g4 CTPC register
     */
    virtual unsigned int read_CTPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 CTPSW register
     * \return Value of the rh850g4 CTPSW register
     */
    virtual unsigned int read_CTPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EIPSWH register
     * \return Value of the rh850g4 EIPSWH register
     */
    virtual unsigned int read_EIPSWH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EIPSWH_GPID register field
     * \return Value of the rh850g4 EIPSWH_GPID register field
     */
    virtual unsigned char read_EIPSWH_GPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EIPSWH_GM register field
     * \return Value of the rh850g4 EIPSWH_GM register field
     */
    virtual unsigned char read_EIPSWH_GM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FEPSWH register
     * \return Value of the rh850g4 FEPSWH register
     */
    virtual unsigned int read_FEPSWH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FEPSWH_GPID register field
     * \return Value of the rh850g4 FEPSWH_GPID register field
     */
    virtual unsigned char read_FEPSWH_GPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FEPSWH_GM register field
     * \return Value of the rh850g4 FEPSWH_GM register field
     */
    virtual unsigned char read_FEPSWH_GM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 CTBP register
     * \return Value of the rh850g4 CTBP register
     */
    virtual unsigned int read_CTBP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 SNZCFG register
     * \return Value of the rh850g4 SNZCFG register
     */
    virtual unsigned int read_SNZCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EIWR register
     * \return Value of the rh850g4 EIWR register
     */
    virtual unsigned int read_EIWR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FEWR register
     * \return Value of the rh850g4 FEWR register
     */
    virtual unsigned int read_FEWR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 SPID register
     * \return Value of the rh850g4 SPID register
     */
    virtual unsigned int read_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 SPIDLIST register
     * \return Value of the rh850g4 SPIDLIST register
     */
    virtual unsigned int read_SPIDLIST(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBASE register
     * \return Value of the rh850g4 RBASE register
     */
    virtual unsigned int read_RBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBASE_RBASE register field
     * \return Value of the rh850g4 RBASE_RBASE register field
     */
    virtual unsigned int read_RBASE_RBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBASE_DV register field
     * \return Value of the rh850g4 RBASE_DV register field
     */
    virtual unsigned char read_RBASE_DV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBASE_RINT register field
     * \return Value of the rh850g4 RBASE_RINT register field
     */
    virtual unsigned char read_RBASE_RINT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EBASE register
     * \return Value of the rh850g4 EBASE register
     */
    virtual unsigned int read_EBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EBASE_EBASE register field
     * \return Value of the rh850g4 EBASE_EBASE register field
     */
    virtual unsigned int read_EBASE_EBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EBASE_DV register field
     * \return Value of the rh850g4 EBASE_DV register field
     */
    virtual unsigned char read_EBASE_DV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 EBASE_RINT register field
     * \return Value of the rh850g4 EBASE_RINT register field
     */
    virtual unsigned char read_EBASE_RINT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 INTBP register
     * \return Value of the rh850g4 INTBP register
     */
    virtual unsigned int read_INTBP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCTL register
     * \return Value of the rh850g4 MCTL register
     */
    virtual unsigned int read_MCTL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCTL_UIC register field
     * \return Value of the rh850g4 MCTL_UIC register field
     */
    virtual unsigned char read_MCTL_UIC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PID register
     * \return Value of the rh850g4 PID register
     */
    virtual unsigned int read_PID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 SVLOCK register
     * \return Value of the rh850g4 SVLOCK register
     */
    virtual unsigned int read_SVLOCK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 SCCFG register
     * \return Value of the rh850g4 SCCFG register
     */
    virtual unsigned int read_SCCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 SCCFG_SIZE register field
     * \return Value of the rh850g4 SCCFG_SIZE register field
     */
    virtual unsigned char read_SCCFG_SIZE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 SCBP register
     * \return Value of the rh850g4 SCBP register
     */
    virtual unsigned int read_SCBP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 HVCFG register
     * \return Value of the rh850g4 HVCFG register
     */
    virtual unsigned int read_HVCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 HVCFG_HVE register field
     * \return Value of the rh850g4 HVCFG_HVE register field
     */
    virtual unsigned char read_HVCFG_HVE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMCFG register
     * \return Value of the rh850g4 GMCFG register
     */
    virtual unsigned int read_GMCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GMP register field
     * \return Value of the rh850g4 GMCFG_GMP register field
     */
    virtual unsigned char read_GMCFG_GMP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMCFG_HMP register field
     * \return Value of the rh850g4 GMCFG_HMP register field
     */
    virtual unsigned char read_GMCFG_HMP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GSYSE register field
     * \return Value of the rh850g4 GMCFG_GSYSE register field
     */
    virtual unsigned char read_GMCFG_GSYSE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU0 register field
     * \return Value of the rh850g4 GMCFG_GCU0 register field
     */
    virtual unsigned char read_GMCFG_GCU0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU1 register field
     * \return Value of the rh850g4 GMCFG_GCU1 register field
     */
    virtual unsigned char read_GMCFG_GCU1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU2 register field
     * \return Value of the rh850g4 GMCFG_GCU2 register field
     */
    virtual unsigned char read_GMCFG_GCU2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 HVSB register
     * \return Value of the rh850g4 HVSB register
     */
    virtual unsigned int read_HVSB(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PEID register
     * \return Value of the rh850g4 PEID register
     */
    virtual unsigned int read_PEID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BMID register
     * \return Value of the rh850g4 BMID register
     */
    virtual unsigned int read_BMID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BMID_BMID register field
     * \return Value of the rh850g4 BMID_BMID register field
     */
    virtual unsigned char read_BMID_BMID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MEA register
     * \return Value of the rh850g4 MEA register
     */
    virtual unsigned int read_MEA(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MEI register
     * \return Value of the rh850g4 MEI register
     */
    virtual unsigned int read_MEI(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBCR0 register
     * \return Value of the rh850g4 RBCR0 register
     */
    virtual unsigned int read_RBCR0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBCR0_MD register field
     * \return Value of the rh850g4 RBCR0_MD register field
     */
    virtual unsigned char read_RBCR0_MD(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBCR0_BE register field
     * \return Value of the rh850g4 RBCR0_BE register field
     */
    virtual unsigned short read_RBCR0_BE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBCR1 register
     * \return Value of the rh850g4 RBCR1 register
     */
    virtual unsigned int read_RBCR1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBCR1_NC register field
     * \return Value of the rh850g4 RBCR1_NC register field
     */
    virtual unsigned short read_RBCR1_NC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBNR register
     * \return Value of the rh850g4 RBNR register
     */
    virtual unsigned int read_RBNR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBNR_BN register field
     * \return Value of the rh850g4 RBNR_BN register field
     */
    virtual unsigned char read_RBNR_BN(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBIP register
     * \return Value of the rh850g4 RBIP register
     */
    virtual unsigned int read_RBIP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBIP_RBIP register field
     * \return Value of the rh850g4 RBIP_RBIP register field
     */
    virtual unsigned int read_RBIP_RBIP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ISPR register
     * \return Value of the rh850g4 ISPR register
     */
    virtual unsigned int read_ISPR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ISPR_ISP register field
     * \return Value of the rh850g4 ISPR_ISP register field
     */
    virtual unsigned short read_ISPR_ISP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR register
     * \return Value of the rh850g4 IMSR register
     */
    virtual unsigned int read_IMSR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_EEIM register field
     * \return Value of the rh850g4 IMSR_EEIM register field
     */
    virtual unsigned char read_IMSR_EEIM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_EPLM register field
     * \return Value of the rh850g4 IMSR_EPLM register field
     */
    virtual unsigned char read_IMSR_EPLM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_EID register field
     * \return Value of the rh850g4 IMSR_EID register field
     */
    virtual unsigned char read_IMSR_EID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_ENP register field
     * \return Value of the rh850g4 IMSR_ENP register field
     */
    virtual unsigned char read_IMSR_ENP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_FNP register field
     * \return Value of the rh850g4 IMSR_FNP register field
     */
    virtual unsigned char read_IMSR_FNP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEEIM register field
     * \return Value of the rh850g4 IMSR_HEEIM register field
     */
    virtual unsigned char read_IMSR_HEEIM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEPLM register field
     * \return Value of the rh850g4 IMSR_HEPLM register field
     */
    virtual unsigned char read_IMSR_HEPLM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEID register field
     * \return Value of the rh850g4 IMSR_HEID register field
     */
    virtual unsigned char read_IMSR_HEID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_HENP register field
     * \return Value of the rh850g4 IMSR_HENP register field
     */
    virtual unsigned char read_IMSR_HENP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR_HFNP register field
     * \return Value of the rh850g4 IMSR_HFNP register field
     */
    virtual unsigned char read_IMSR_HFNP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICSR register
     * \return Value of the rh850g4 ICSR register
     */
    virtual unsigned int read_ICSR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICSR_PMEI register field
     * \return Value of the rh850g4 ICSR_PMEI register field
     */
    virtual unsigned char read_ICSR_PMEI(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 INTCFG register
     * \return Value of the rh850g4 INTCFG register
     */
    virtual unsigned int read_INTCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 INTCFG_ISPC register field
     * \return Value of the rh850g4 INTCFG_ISPC register field
     */
    virtual unsigned char read_INTCFG_ISPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 INTCFG_EPL register field
     * \return Value of the rh850g4 INTCFG_EPL register field
     */
    virtual unsigned char read_INTCFG_EPL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 INTCFG_ULNR register field
     * \return Value of the rh850g4 INTCFG_ULNR register field
     */
    virtual unsigned char read_INTCFG_ULNR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PLMR register
     * \return Value of the rh850g4 PLMR register
     */
    virtual unsigned int read_PLMR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PLMR_PLM register field
     * \return Value of the rh850g4 PLMR_PLM register field
     */
    virtual unsigned char read_PLMR_PLM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR register
     * \return Value of the rh850g4 FXSR register
     */
    virtual unsigned int read_FXSR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_FN register field
     * \return Value of the rh850g4 FXSR_FN register field
     */
    virtual unsigned char read_FXSR_FN(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_IF register field
     * \return Value of the rh850g4 FXSR_IF register field
     */
    virtual unsigned char read_FXSR_IF(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_PEM register field
     * \return Value of the rh850g4 FXSR_PEM register field
     */
    virtual unsigned char read_FXSR_PEM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_RM register field
     * \return Value of the rh850g4 FXSR_RM register field
     */
    virtual unsigned char read_FXSR_RM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_FS register field
     * \return Value of the rh850g4 FXSR_FS register field
     */
    virtual unsigned char read_FXSR_FS(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_XC register field
     * \return Value of the rh850g4 FXSR_XC register field
     */
    virtual unsigned char read_FXSR_XC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_XE register field
     * \return Value of the rh850g4 FXSR_XE register field
     */
    virtual unsigned char read_FXSR_XE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR_XP register field
     * \return Value of the rh850g4 FXSR_XP register field
     */
    virtual unsigned char read_FXSR_XP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXST register
     * \return Value of the rh850g4 FXST register
     */
    virtual unsigned int read_FXST(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXST_XC register field
     * \return Value of the rh850g4 FXST_XC register field
     */
    virtual unsigned char read_FXST_XC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXST_IF register field
     * \return Value of the rh850g4 FXST_IF register field
     */
    virtual unsigned char read_FXST_IF(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXST_XP register field
     * \return Value of the rh850g4 FXST_XP register field
     */
    virtual unsigned char read_FXST_XP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXINFO register
     * \return Value of the rh850g4 FXINFO register
     */
    virtual unsigned int read_FXINFO(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXINFO_NFPU register field
     * \return Value of the rh850g4 FXINFO_NFPU register field
     */
    virtual unsigned char read_FXINFO_NFPU(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXINFO_RSIZE register field
     * \return Value of the rh850g4 FXINFO_RSIZE register field
     */
    virtual unsigned char read_FXINFO_RSIZE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXCFG register
     * \return Value of the rh850g4 FXCFG register
     */
    virtual unsigned int read_FXCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXCFG_RM register field
     * \return Value of the rh850g4 FXCFG_RM register field
     */
    virtual unsigned char read_FXCFG_RM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXCFG_XE register field
     * \return Value of the rh850g4 FXCFG_XE register field
     */
    virtual unsigned char read_FXCFG_XE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXC register
     * \return Value of the rh850g4 FXXC register
     */
    virtual unsigned int read_FXXC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC3 register field
     * \return Value of the rh850g4 FXXC_XC3 register field
     */
    virtual unsigned char read_FXXC_XC3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC2 register field
     * \return Value of the rh850g4 FXXC_XC2 register field
     */
    virtual unsigned char read_FXXC_XC2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC1 register field
     * \return Value of the rh850g4 FXXC_XC1 register field
     */
    virtual unsigned char read_FXXC_XC1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC0 register field
     * \return Value of the rh850g4 FXXC_XC0 register field
     */
    virtual unsigned char read_FXXC_XC0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXP register
     * \return Value of the rh850g4 FXXP register
     */
    virtual unsigned int read_FXXP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP3 register field
     * \return Value of the rh850g4 FXXP_XP3 register field
     */
    virtual unsigned char read_FXXP_XP3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP2 register field
     * \return Value of the rh850g4 FXXP_XP2 register field
     */
    virtual unsigned char read_FXXP_XP2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP1 register field
     * \return Value of the rh850g4 FXXP_XP1 register field
     */
    virtual unsigned char read_FXXP_XP1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP0 register field
     * \return Value of the rh850g4 FXXP_XP0 register field
     */
    virtual unsigned char read_FXXP_XP0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEIPC register
     * \return Value of the rh850g4 GMEIPC register
     */
    virtual unsigned int read_GMEIPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEIPSW register
     * \return Value of the rh850g4 GMEIPSW register
     */
    virtual unsigned int read_GMEIPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMFEPC register
     * \return Value of the rh850g4 GMFEPC register
     */
    virtual unsigned int read_GMFEPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMFEPSW register
     * \return Value of the rh850g4 GMFEPSW register
     */
    virtual unsigned int read_GMFEPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW register
     * \return Value of the rh850g4 GMPSW register
     */
    virtual unsigned int read_GMPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_Z register field
     * \return Value of the rh850g4 GMPSW_Z register field
     */
    virtual unsigned char read_GMPSW_Z(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_S register field
     * \return Value of the rh850g4 GMPSW_S register field
     */
    virtual unsigned char read_GMPSW_S(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_OV register field
     * \return Value of the rh850g4 GMPSW_OV register field
     */
    virtual unsigned char read_GMPSW_OV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CY register field
     * \return Value of the rh850g4 GMPSW_CY register field
     */
    virtual unsigned char read_GMPSW_CY(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_SAT register field
     * \return Value of the rh850g4 GMPSW_SAT register field
     */
    virtual unsigned char read_GMPSW_SAT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_ID register field
     * \return Value of the rh850g4 GMPSW_ID register field
     */
    virtual unsigned char read_GMPSW_ID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EP register field
     * \return Value of the rh850g4 GMPSW_EP register field
     */
    virtual unsigned char read_GMPSW_EP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_NP register field
     * \return Value of the rh850g4 GMPSW_NP register field
     */
    virtual unsigned char read_GMPSW_NP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EBV register field
     * \return Value of the rh850g4 GMPSW_EBV register field
     */
    virtual unsigned char read_GMPSW_EBV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU0 register field
     * \return Value of the rh850g4 GMPSW_CU0 register field
     */
    virtual unsigned char read_GMPSW_CU0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU1 register field
     * \return Value of the rh850g4 GMPSW_CU1 register field
     */
    virtual unsigned char read_GMPSW_CU1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU2 register field
     * \return Value of the rh850g4 GMPSW_CU2 register field
     */
    virtual unsigned char read_GMPSW_CU2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EIMASK register field
     * \return Value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual unsigned char read_GMPSW_EIMASK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPSW_UM register field
     * \return Value of the rh850g4 GMPSW_UM register field
     */
    virtual unsigned char read_GMPSW_UM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMMEA register
     * \return Value of the rh850g4 GMMEA register
     */
    virtual unsigned int read_GMMEA(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMMEI register
     * \return Value of the rh850g4 GMMEI register
     */
    virtual unsigned int read_GMMEI(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEIIC register
     * \return Value of the rh850g4 GMEIIC register
     */
    virtual unsigned int read_GMEIIC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMFEIC register
     * \return Value of the rh850g4 GMFEIC register
     */
    virtual unsigned int read_GMFEIC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMSPID register
     * \return Value of the rh850g4 GMSPID register
     */
    virtual unsigned int read_GMSPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMSPIDLIST register
     * \return Value of the rh850g4 GMSPIDLIST register
     */
    virtual unsigned int read_GMSPIDLIST(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEBASE register
     * \return Value of the rh850g4 GMEBASE register
     */
    virtual unsigned int read_GMEBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_EBASE register field
     * \return Value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual unsigned int read_GMEBASE_EBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_DV register field
     * \return Value of the rh850g4 GMEBASE_DV register field
     */
    virtual unsigned char read_GMEBASE_DV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_RINT register field
     * \return Value of the rh850g4 GMEBASE_RINT register field
     */
    virtual unsigned char read_GMEBASE_RINT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMINTBP register
     * \return Value of the rh850g4 GMINTBP register
     */
    virtual unsigned int read_GMINTBP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG register
     * \return Value of the rh850g4 GMINTCFG register
     */
    virtual unsigned int read_GMINTCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_ISPC register field
     * \return Value of the rh850g4 GMINTCFG_ISPC register field
     */
    virtual unsigned char read_GMINTCFG_ISPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_EPL register field
     * \return Value of the rh850g4 GMINTCFG_EPL register field
     */
    virtual unsigned char read_GMINTCFG_EPL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_ULNR register field
     * \return Value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual unsigned char read_GMINTCFG_ULNR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPLMR register
     * \return Value of the rh850g4 GMPLMR register
     */
    virtual unsigned int read_GMPLMR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPLMR_PLM register field
     * \return Value of the rh850g4 GMPLMR_PLM register field
     */
    virtual unsigned char read_GMPLMR_PLM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMSVLOCK register
     * \return Value of the rh850g4 GMSVLOCK register
     */
    virtual unsigned int read_GMSVLOCK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMMPM register
     * \return Value of the rh850g4 GMMPM register
     */
    virtual unsigned int read_GMMPM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMEIWR register
     * \return Value of the rh850g4 GMEIWR register
     */
    virtual unsigned int read_GMEIWR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMFEWR register
     * \return Value of the rh850g4 GMFEWR register
     */
    virtual unsigned int read_GMFEWR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 GMPEID register
     * \return Value of the rh850g4 GMPEID register
     */
    virtual unsigned int read_GMPEID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPM register
     * \return Value of the rh850g4 MPM register
     */
    virtual unsigned int read_MPM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPCFG register
     * \return Value of the rh850g4 MPCFG register
     */
    virtual unsigned int read_MPCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPCFG_NMPUE register field
     * \return Value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual unsigned char read_MPCFG_NMPUE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPCFG_HBE register field
     * \return Value of the rh850g4 MPCFG_HBE register field
     */
    virtual unsigned char read_MPCFG_HBE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPCFG_ARCH register field
     * \return Value of the rh850g4 MPCFG_ARCH register field
     */
    virtual unsigned char read_MPCFG_ARCH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPCFG_NBK register field
     * \return Value of the rh850g4 MPCFG_NBK register field
     */
    virtual unsigned char read_MPCFG_NBK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPCFG_DMDP register field
     * \return Value of the rh850g4 MPCFG_DMDP register field
     */
    virtual unsigned char read_MPCFG_DMDP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCA register
     * \return Value of the rh850g4 MCA register
     */
    virtual unsigned int read_MCA(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCS register
     * \return Value of the rh850g4 MCS register
     */
    virtual unsigned int read_MCS(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCC register
     * \return Value of the rh850g4 MCC register
     */
    virtual unsigned int read_MCC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR register
     * \return Value of the rh850g4 MCR register
     */
    virtual unsigned int read_MCR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_HSXE register field
     * \return Value of the rh850g4 MCR_HSXE register field
     */
    virtual unsigned char read_MCR_HSXE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_HSWE register field
     * \return Value of the rh850g4 MCR_HSWE register field
     */
    virtual unsigned char read_MCR_HSWE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_HSRE register field
     * \return Value of the rh850g4 MCR_HSRE register field
     */
    virtual unsigned char read_MCR_HSRE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_HUXE register field
     * \return Value of the rh850g4 MCR_HUXE register field
     */
    virtual unsigned char read_MCR_HUXE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_HUWE register field
     * \return Value of the rh850g4 MCR_HUWE register field
     */
    virtual unsigned char read_MCR_HUWE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_HURE register field
     * \return Value of the rh850g4 MCR_HURE register field
     */
    virtual unsigned char read_MCR_HURE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_GSXE register field
     * \return Value of the rh850g4 MCR_GSXE register field
     */
    virtual unsigned char read_MCR_GSXE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_GSWE register field
     * \return Value of the rh850g4 MCR_GSWE register field
     */
    virtual unsigned char read_MCR_GSWE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_GSRE register field
     * \return Value of the rh850g4 MCR_GSRE register field
     */
    virtual unsigned char read_MCR_GSRE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_GUXE register field
     * \return Value of the rh850g4 MCR_GUXE register field
     */
    virtual unsigned char read_MCR_GUXE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_GUWE register field
     * \return Value of the rh850g4 MCR_GUWE register field
     */
    virtual unsigned char read_MCR_GUWE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_GURE register field
     * \return Value of the rh850g4 MCR_GURE register field
     */
    virtual unsigned char read_MCR_GURE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_OV register field
     * \return Value of the rh850g4 MCR_OV register field
     */
    virtual unsigned char read_MCR_OV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_SXE register field
     * \return Value of the rh850g4 MCR_SXE register field
     */
    virtual unsigned char read_MCR_SXE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_SWE register field
     * \return Value of the rh850g4 MCR_SWE register field
     */
    virtual unsigned char read_MCR_SWE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_SRE register field
     * \return Value of the rh850g4 MCR_SRE register field
     */
    virtual unsigned char read_MCR_SRE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_UXE register field
     * \return Value of the rh850g4 MCR_UXE register field
     */
    virtual unsigned char read_MCR_UXE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_UWE register field
     * \return Value of the rh850g4 MCR_UWE register field
     */
    virtual unsigned char read_MCR_UWE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCR_URE register field
     * \return Value of the rh850g4 MCR_URE register field
     */
    virtual unsigned char read_MCR_URE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCI register
     * \return Value of the rh850g4 MCI register
     */
    virtual unsigned int read_MCI(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MCI_SPID register field
     * \return Value of the rh850g4 MCI_SPID register field
     */
    virtual unsigned char read_MCI_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPIDX register
     * \return Value of the rh850g4 MPIDX register
     */
    virtual unsigned int read_MPIDX(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPIDX_IDX register field
     * \return Value of the rh850g4 MPIDX_IDX register field
     */
    virtual unsigned char read_MPIDX_IDX(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPBK register
     * \return Value of the rh850g4 MPBK register
     */
    virtual unsigned int read_MPBK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPBK_BK register field
     * \return Value of the rh850g4 MPBK_BK register field
     */
    virtual unsigned char read_MPBK_BK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPLA register
     * \return Value of the rh850g4 MPLA register
     */
    virtual unsigned int read_MPLA(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPUA register
     * \return Value of the rh850g4 MPUA register
     */
    virtual unsigned int read_MPUA(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT register
     * \return Value of the rh850g4 MPAT register
     */
    virtual unsigned int read_MPAT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID7 register field
     * \return Value of the rh850g4 MPAT_WMPID7 register field
     */
    virtual unsigned char read_MPAT_WMPID7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID6 register field
     * \return Value of the rh850g4 MPAT_WMPID6 register field
     */
    virtual unsigned char read_MPAT_WMPID6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID5 register field
     * \return Value of the rh850g4 MPAT_WMPID5 register field
     */
    virtual unsigned char read_MPAT_WMPID5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID4 register field
     * \return Value of the rh850g4 MPAT_WMPID4 register field
     */
    virtual unsigned char read_MPAT_WMPID4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID3 register field
     * \return Value of the rh850g4 MPAT_WMPID3 register field
     */
    virtual unsigned char read_MPAT_WMPID3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID2 register field
     * \return Value of the rh850g4 MPAT_WMPID2 register field
     */
    virtual unsigned char read_MPAT_WMPID2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID1 register field
     * \return Value of the rh850g4 MPAT_WMPID1 register field
     */
    virtual unsigned char read_MPAT_WMPID1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID0 register field
     * \return Value of the rh850g4 MPAT_WMPID0 register field
     */
    virtual unsigned char read_MPAT_WMPID0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID7 register field
     * \return Value of the rh850g4 MPAT_RMPID7 register field
     */
    virtual unsigned char read_MPAT_RMPID7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID6 register field
     * \return Value of the rh850g4 MPAT_RMPID6 register field
     */
    virtual unsigned char read_MPAT_RMPID6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID5 register field
     * \return Value of the rh850g4 MPAT_RMPID5 register field
     */
    virtual unsigned char read_MPAT_RMPID5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID4 register field
     * \return Value of the rh850g4 MPAT_RMPID4 register field
     */
    virtual unsigned char read_MPAT_RMPID4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID3 register field
     * \return Value of the rh850g4 MPAT_RMPID3 register field
     */
    virtual unsigned char read_MPAT_RMPID3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID2 register field
     * \return Value of the rh850g4 MPAT_RMPID2 register field
     */
    virtual unsigned char read_MPAT_RMPID2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID1 register field
     * \return Value of the rh850g4 MPAT_RMPID1 register field
     */
    virtual unsigned char read_MPAT_RMPID1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID0 register field
     * \return Value of the rh850g4 MPAT_RMPID0 register field
     */
    virtual unsigned char read_MPAT_RMPID0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_WG register field
     * \return Value of the rh850g4 MPAT_WG register field
     */
    virtual unsigned char read_MPAT_WG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_RG register field
     * \return Value of the rh850g4 MPAT_RG register field
     */
    virtual unsigned char read_MPAT_RG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_E register field
     * \return Value of the rh850g4 MPAT_E register field
     */
    virtual unsigned char read_MPAT_E(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_SX register field
     * \return Value of the rh850g4 MPAT_SX register field
     */
    virtual unsigned char read_MPAT_SX(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_SW register field
     * \return Value of the rh850g4 MPAT_SW register field
     */
    virtual unsigned char read_MPAT_SW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_SR register field
     * \return Value of the rh850g4 MPAT_SR register field
     */
    virtual unsigned char read_MPAT_SR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_UX register field
     * \return Value of the rh850g4 MPAT_UX register field
     */
    virtual unsigned char read_MPAT_UX(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_UW register field
     * \return Value of the rh850g4 MPAT_UW register field
     */
    virtual unsigned char read_MPAT_UW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPAT_UR register field
     * \return Value of the rh850g4 MPAT_UR register field
     */
    virtual unsigned char read_MPAT_UR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID0 register
     * \return Value of the rh850g4 MPID0 register
     */
    virtual unsigned int read_MPID0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID0_SPID register field
     * \return Value of the rh850g4 MPID0_SPID register field
     */
    virtual unsigned char read_MPID0_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID1 register
     * \return Value of the rh850g4 MPID1 register
     */
    virtual unsigned int read_MPID1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID1_SPID register field
     * \return Value of the rh850g4 MPID1_SPID register field
     */
    virtual unsigned char read_MPID1_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID2 register
     * \return Value of the rh850g4 MPID2 register
     */
    virtual unsigned int read_MPID2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID2_SPID register field
     * \return Value of the rh850g4 MPID2_SPID register field
     */
    virtual unsigned char read_MPID2_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID3 register
     * \return Value of the rh850g4 MPID3 register
     */
    virtual unsigned int read_MPID3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID3_SPID register field
     * \return Value of the rh850g4 MPID3_SPID register field
     */
    virtual unsigned char read_MPID3_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID4 register
     * \return Value of the rh850g4 MPID4 register
     */
    virtual unsigned int read_MPID4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID4_SPID register field
     * \return Value of the rh850g4 MPID4_SPID register field
     */
    virtual unsigned char read_MPID4_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID5 register
     * \return Value of the rh850g4 MPID5 register
     */
    virtual unsigned int read_MPID5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID5_SPID register field
     * \return Value of the rh850g4 MPID5_SPID register field
     */
    virtual unsigned char read_MPID5_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID6 register
     * \return Value of the rh850g4 MPID6 register
     */
    virtual unsigned int read_MPID6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID6_SPID register field
     * \return Value of the rh850g4 MPID6_SPID register field
     */
    virtual unsigned char read_MPID6_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID7 register
     * \return Value of the rh850g4 MPID7 register
     */
    virtual unsigned int read_MPID7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID7_SPID register field
     * \return Value of the rh850g4 MPID7_SPID register field
     */
    virtual unsigned char read_MPID7_SPID(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGL register
     * \return Value of the rh850g4 ICTAGL register
     */
    virtual unsigned int read_ICTAGL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_LPN register field
     * \return Value of the rh850g4 ICTAGL_LPN register field
     */
    virtual unsigned int read_ICTAGL_LPN(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_LRU register field
     * \return Value of the rh850g4 ICTAGL_LRU register field
     */
    virtual unsigned char read_ICTAGL_LRU(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_L register field
     * \return Value of the rh850g4 ICTAGL_L register field
     */
    virtual unsigned char read_ICTAGL_L(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_V register field
     * \return Value of the rh850g4 ICTAGL_V register field
     */
    virtual unsigned char read_ICTAGL_V(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH register
     * \return Value of the rh850g4 ICTAGH register
     */
    virtual unsigned int read_ICTAGH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_WD register field
     * \return Value of the rh850g4 ICTAGH_WD register field
     */
    virtual unsigned char read_ICTAGH_WD(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_PD register field
     * \return Value of the rh850g4 ICTAGH_PD register field
     */
    virtual unsigned char read_ICTAGH_PD(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_WT register field
     * \return Value of the rh850g4 ICTAGH_WT register field
     */
    virtual unsigned char read_ICTAGH_WT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_PT register field
     * \return Value of the rh850g4 ICTAGH_PT register field
     */
    virtual unsigned char read_ICTAGH_PT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_DATAECC register field
     * \return Value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual unsigned char read_ICTAGH_DATAECC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_TAGECC register field
     * \return Value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual unsigned char read_ICTAGH_TAGECC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICDATL register
     * \return Value of the rh850g4 ICDATL register
     */
    virtual unsigned int read_ICDATL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICDATH register
     * \return Value of the rh850g4 ICDATH register
     */
    virtual unsigned int read_ICDATH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCTRL register
     * \return Value of the rh850g4 ICCTRL register
     */
    virtual unsigned int read_ICCTRL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_PBS register field
     * \return Value of the rh850g4 ICCTRL_PBS register field
     */
    virtual unsigned char read_ICCTRL_PBS(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHCLR register field
     * \return Value of the rh850g4 ICCTRL_ICHCLR register field
     */
    virtual unsigned char read_ICCTRL_ICHCLR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEIV register field
     * \return Value of the rh850g4 ICCTRL_ICHEIV register field
     */
    virtual unsigned char read_ICCTRL_ICHEIV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEMK register field
     * \return Value of the rh850g4 ICCTRL_ICHEMK register field
     */
    virtual unsigned char read_ICCTRL_ICHEMK(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEN register field
     * \return Value of the rh850g4 ICCTRL_ICHEN register field
     */
    virtual unsigned char read_ICCTRL_ICHEN(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCFG register
     * \return Value of the rh850g4 ICCFG register
     */
    virtual unsigned int read_ICCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHSIZE register field
     * \return Value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual unsigned char read_ICCFG_ICHSIZE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHLINE register field
     * \return Value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual unsigned char read_ICCFG_ICHLINE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHWAY register field
     * \return Value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual unsigned char read_ICCFG_ICHWAY(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR register
     * \return Value of the rh850g4 ICERR register
     */
    virtual unsigned int read_ICERR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_CISTW register field
     * \return Value of the rh850g4 ICERR_CISTW register field
     */
    virtual unsigned char read_ICERR_CISTW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESAFE register field
     * \return Value of the rh850g4 ICERR_ESAFE register field
     */
    virtual unsigned char read_ICERR_ESAFE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESMH register field
     * \return Value of the rh850g4 ICERR_ESMH register field
     */
    virtual unsigned char read_ICERR_ESMH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESPBSE register field
     * \return Value of the rh850g4 ICERR_ESPBSE register field
     */
    virtual unsigned char read_ICERR_ESPBSE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESTE1 register field
     * \return Value of the rh850g4 ICERR_ESTE1 register field
     */
    virtual unsigned char read_ICERR_ESTE1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESTE2 register field
     * \return Value of the rh850g4 ICERR_ESTE2 register field
     */
    virtual unsigned char read_ICERR_ESTE2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESDC register field
     * \return Value of the rh850g4 ICERR_ESDC register field
     */
    virtual unsigned char read_ICERR_ESDC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESDE register field
     * \return Value of the rh850g4 ICERR_ESDE register field
     */
    virtual unsigned char read_ICERR_ESDE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMAFE register field
     * \return Value of the rh850g4 ICERR_ERMAFE register field
     */
    virtual unsigned char read_ICERR_ERMAFE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMMH register field
     * \return Value of the rh850g4 ICERR_ERMMH register field
     */
    virtual unsigned char read_ICERR_ERMMH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMPBSE register field
     * \return Value of the rh850g4 ICERR_ERMPBSE register field
     */
    virtual unsigned char read_ICERR_ERMPBSE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMTE1 register field
     * \return Value of the rh850g4 ICERR_ERMTE1 register field
     */
    virtual unsigned char read_ICERR_ERMTE1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMTE2 register field
     * \return Value of the rh850g4 ICERR_ERMTE2 register field
     */
    virtual unsigned char read_ICERR_ERMTE2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMDC register field
     * \return Value of the rh850g4 ICERR_ERMDC register field
     */
    virtual unsigned char read_ICERR_ERMDC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMDE register field
     * \return Value of the rh850g4 ICERR_ERMDE register field
     */
    virtual unsigned char read_ICERR_ERMDE(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHEWY register field
     * \return Value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual unsigned char read_ICERR_ICHEWY(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHEIX register field
     * \return Value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual unsigned char read_ICERR_ICHEIX(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHERQ register field
     * \return Value of the rh850g4 ICERR_ICHERQ register field
     */
    virtual unsigned char read_ICERR_ICHERQ(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHED register field
     * \return Value of the rh850g4 ICERR_ICHED register field
     */
    virtual unsigned char read_ICERR_ICHED(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHET register field
     * \return Value of the rh850g4 ICERR_ICHET register field
     */
    virtual unsigned char read_ICERR_ICHET(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHERR register field
     * \return Value of the rh850g4 ICERR_ICHERR register field
     */
    virtual unsigned char read_ICERR_ICHERR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 TSCOUNTL register
     * \return Value of the rh850g4 TSCOUNTL register
     */
    virtual unsigned int read_TSCOUNTL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 TSCOUNTH register
     * \return Value of the rh850g4 TSCOUNTH register
     */
    virtual unsigned int read_TSCOUNTH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 TSCTRL register
     * \return Value of the rh850g4 TSCTRL register
     */
    virtual unsigned int read_TSCTRL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMUMCTRL register
     * \return Value of the rh850g4 PMUMCTRL register
     */
    virtual unsigned int read_PMUMCTRL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMGMCTRL register
     * \return Value of the rh850g4 PMGMCTRL register
     */
    virtual unsigned int read_PMGMCTRL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT0 register
     * \return Value of the rh850g4 PMCOUNT0 register
     */
    virtual unsigned int read_PMCOUNT0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL0 register
     * \return Value of the rh850g4 PMCTRL0 register
     */
    virtual unsigned int read_PMCTRL0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT1 register
     * \return Value of the rh850g4 PMCOUNT1 register
     */
    virtual unsigned int read_PMCOUNT1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL1 register
     * \return Value of the rh850g4 PMCTRL1 register
     */
    virtual unsigned int read_PMCTRL1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT2 register
     * \return Value of the rh850g4 PMCOUNT2 register
     */
    virtual unsigned int read_PMCOUNT2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL2 register
     * \return Value of the rh850g4 PMCTRL2 register
     */
    virtual unsigned int read_PMCTRL2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT3 register
     * \return Value of the rh850g4 PMCOUNT3 register
     */
    virtual unsigned int read_PMCOUNT3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL3 register
     * \return Value of the rh850g4 PMCTRL3 register
     */
    virtual unsigned int read_PMCTRL3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL4 register
     * \return Value of the rh850g4 PMCTRL4 register
     */
    virtual unsigned int read_PMCTRL4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL5 register
     * \return Value of the rh850g4 PMCTRL5 register
     */
    virtual unsigned int read_PMCTRL5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL6 register
     * \return Value of the rh850g4 PMCTRL6 register
     */
    virtual unsigned int read_PMCTRL6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCTRL7 register
     * \return Value of the rh850g4 PMCTRL7 register
     */
    virtual unsigned int read_PMCTRL7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT4 register
     * \return Value of the rh850g4 PMCOUNT4 register
     */
    virtual unsigned int read_PMCOUNT4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT5 register
     * \return Value of the rh850g4 PMCOUNT5 register
     */
    virtual unsigned int read_PMCOUNT5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT6 register
     * \return Value of the rh850g4 PMCOUNT6 register
     */
    virtual unsigned int read_PMCOUNT6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT7 register
     * \return Value of the rh850g4 PMCOUNT7 register
     */
    virtual unsigned int read_PMCOUNT7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND0 register
     * \return Value of the rh850g4 PMSUBCND0 register
     */
    virtual unsigned int read_PMSUBCND0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND1 register
     * \return Value of the rh850g4 PMSUBCND1 register
     */
    virtual unsigned int read_PMSUBCND1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND2 register
     * \return Value of the rh850g4 PMSUBCND2 register
     */
    virtual unsigned int read_PMSUBCND2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND3 register
     * \return Value of the rh850g4 PMSUBCND3 register
     */
    virtual unsigned int read_PMSUBCND3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND4 register
     * \return Value of the rh850g4 PMSUBCND4 register
     */
    virtual unsigned int read_PMSUBCND4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND5 register
     * \return Value of the rh850g4 PMSUBCND5 register
     */
    virtual unsigned int read_PMSUBCND5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND6 register
     * \return Value of the rh850g4 PMSUBCND6 register
     */
    virtual unsigned int read_PMSUBCND6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND7 register
     * \return Value of the rh850g4 PMSUBCND7 register
     */
    virtual unsigned int read_PMSUBCND7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DBGEN register
     * \return Value of the rh850g4 DBGEN register
     */
    virtual unsigned int read_DBGEN(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DBPSWH register
     * \return Value of the rh850g4 DBPSWH register
     */
    virtual unsigned int read_DBPSWH(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DBIC register
     * \return Value of the rh850g4 DBIC register
     */
    virtual unsigned int read_DBIC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DBPC register
     * \return Value of the rh850g4 DBPC register
     */
    virtual unsigned int read_DBPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DBPSW register
     * \return Value of the rh850g4 DBPSW register
     */
    virtual unsigned int read_DBPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DIR0 register
     * \return Value of the rh850g4 DIR0 register
     */
    virtual unsigned int read_DIR0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DIR1 register
     * \return Value of the rh850g4 DIR1 register
     */
    virtual unsigned int read_DIR1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BPC register
     * \return Value of the rh850g4 BPC register
     */
    virtual unsigned int read_BPC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BPAV register
     * \return Value of the rh850g4 BPAV register
     */
    virtual unsigned int read_BPAV(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BPAM register
     * \return Value of the rh850g4 BPAM register
     */
    virtual unsigned int read_BPAM(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DBCFG register
     * \return Value of the rh850g4 DBCFG register
     */
    virtual unsigned int read_DBCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DBWR register
     * \return Value of the rh850g4 DBWR register
     */
    virtual unsigned int read_DBWR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 LSTEST0 register
     * \return Value of the rh850g4 LSTEST0 register
     */
    virtual unsigned int read_LSTEST0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 LSTEST1 register
     * \return Value of the rh850g4 LSTEST1 register
     */
    virtual unsigned int read_LSTEST1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 LSCFG register
     * \return Value of the rh850g4 LSCFG register
     */
    virtual unsigned int read_LSCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICBKEY register
     * \return Value of the rh850g4 ICBKEY register
     */
    virtual unsigned int read_ICBKEY(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IFCR register
     * \return Value of the rh850g4 IFCR register
     */
    virtual unsigned int read_IFCR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IFCR1 register
     * \return Value of the rh850g4 IFCR1 register
     */
    virtual unsigned int read_IFCR1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BRPCTRL0 register
     * \return Value of the rh850g4 BRPCTRL0 register
     */
    virtual unsigned int read_BRPCTRL0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BRPCTRL1 register
     * \return Value of the rh850g4 BRPCTRL1 register
     */
    virtual unsigned int read_BRPCTRL1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BRPCFG register
     * \return Value of the rh850g4 BRPCFG register
     */
    virtual unsigned int read_BRPCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BRPACTRL register
     * \return Value of the rh850g4 BRPACTRL register
     */
    virtual unsigned int read_BRPACTRL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 BRPDATA register
     * \return Value of the rh850g4 BRPDATA register
     */
    virtual unsigned int read_BRPDATA(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DCBKEY register
     * \return Value of the rh850g4 DCBKEY register
     */
    virtual unsigned int read_DCBKEY(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 LSUCR register
     * \return Value of the rh850g4 LSUCR register
     */
    virtual unsigned int read_LSUCR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 LSULNK0 register
     * \return Value of the rh850g4 LSULNK0 register
     */
    virtual unsigned int read_LSULNK0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 LSULNK1 register
     * \return Value of the rh850g4 LSULNK1 register
     */
    virtual unsigned int read_LSULNK1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 L1RLCR register
     * \return Value of the rh850g4 L1RLCR register
     */
    virtual unsigned int read_L1RLCR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 L1RLNK0 register
     * \return Value of the rh850g4 L1RLNK0 register
     */
    virtual unsigned int read_L1RLNK0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 L1RLNK1 register
     * \return Value of the rh850g4 L1RLNK1 register
     */
    virtual unsigned int read_L1RLNK1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 L1RCFG register
     * \return Value of the rh850g4 L1RCFG register
     */
    virtual unsigned int read_L1RCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DECFG register
     * \return Value of the rh850g4 DECFG register
     */
    virtual unsigned int read_DECFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DECTRL register
     * \return Value of the rh850g4 DECTRL register
     */
    virtual unsigned int read_DECTRL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 DEVDS register
     * \return Value of the rh850g4 DEVDS register
     */
    virtual unsigned int read_DEVDS(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBCR register
     * \return Value of the rh850g4 RDBCR register
     */
    virtual unsigned int read_RDBCR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBACR register
     * \return Value of the rh850g4 RDBACR register
     */
    virtual unsigned int read_RDBACR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBATAG register
     * \return Value of the rh850g4 RDBATAG register
     */
    virtual unsigned int read_RDBATAG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBADAT0 register
     * \return Value of the rh850g4 RDBADAT0 register
     */
    virtual unsigned int read_RDBADAT0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBADAT1 register
     * \return Value of the rh850g4 RDBADAT1 register
     */
    virtual unsigned int read_RDBADAT1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBADAT2 register
     * \return Value of the rh850g4 RDBADAT2 register
     */
    virtual unsigned int read_RDBADAT2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBADAT3 register
     * \return Value of the rh850g4 RDBADAT3 register
     */
    virtual unsigned int read_RDBADAT3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RDBSTAT register
     * \return Value of the rh850g4 RDBSTAT register
     */
    virtual unsigned int read_RDBSTAT(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 hv_func_support register
     * \return Value of the rh850g4 hv_func_support register
     */
    virtual unsigned char read_hv_func_support(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 swd0pcad_exec_count register
     * \return Value of the rh850g4 swd0pcad_exec_count register
     */
    virtual unsigned int read_swd0pcad_exec_count(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 swd0pcad_val register
     * \return Value of the rh850g4 swd0pcad_val register
     */
    virtual unsigned int read_swd0pcad_val(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 mem_acc_type register
     * \return Value of the rh850g4 mem_acc_type register
     */
    virtual unsigned int read_mem_acc_type(void) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateRegisterGroupReadOperations
     * @{
     */
    /**
     * \brief Queries the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return Value of the specified rh850g4 GR group register
     */
    virtual unsigned int read_GR(rh850g4::GR_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return Value of the specified rh850g4 WR group register
     */
    virtual unsigned long long read_WR(rh850g4::WR_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return Value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual unsigned int read_SR_SELID_0(rh850g4::SR_SELID_0_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return Value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual unsigned int read_SR_SELID_1(rh850g4::SR_SELID_1_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return Value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual unsigned int read_SR_SELID_2(rh850g4::SR_SELID_2_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return Value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual unsigned int read_SR_SELID_3(rh850g4::SR_SELID_3_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return Value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual unsigned int read_SR_SELID_4(rh850g4::SR_SELID_4_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return Value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual unsigned int read_SR_SELID_5(rh850g4::SR_SELID_5_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return Value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual unsigned int read_SR_SELID_9(rh850g4::SR_SELID_9_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return Value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual unsigned int read_SR_SELID_10(rh850g4::SR_SELID_10_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return Value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual unsigned int read_SR_SELID_11(rh850g4::SR_SELID_11_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return Value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual unsigned int read_SR_SELID_12(rh850g4::SR_SELID_12_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return Value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual unsigned int read_SR_SELID_13(rh850g4::SR_SELID_13_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return Value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual unsigned int read_SR_SELID_14(rh850g4::SR_SELID_14_index_t index) = 0;
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return Value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual unsigned int read_SR_SELID_15(rh850g4::SR_SELID_15_index_t index) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateIndividualRegisterWriteOperations
     * @{
     */
    /**
     * \brief Sets the value of the rh850g4 translation time PC register
     * \param value New value of the rh850g4 translation time PC register
     */
    virtual void write_trans_PC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r0 register
     * \param value New value of the rh850g4 r0 register
     */
    virtual void write_r0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r1 register
     * \param value New value of the rh850g4 r1 register
     */
    virtual void write_r1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r2 register
     * \param value New value of the rh850g4 r2 register
     */
    virtual void write_r2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r3 register
     * \param value New value of the rh850g4 r3 register
     */
    virtual void write_r3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r4 register
     * \param value New value of the rh850g4 r4 register
     */
    virtual void write_r4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r5 register
     * \param value New value of the rh850g4 r5 register
     */
    virtual void write_r5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r6 register
     * \param value New value of the rh850g4 r6 register
     */
    virtual void write_r6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r7 register
     * \param value New value of the rh850g4 r7 register
     */
    virtual void write_r7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r8 register
     * \param value New value of the rh850g4 r8 register
     */
    virtual void write_r8(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r9 register
     * \param value New value of the rh850g4 r9 register
     */
    virtual void write_r9(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r10 register
     * \param value New value of the rh850g4 r10 register
     */
    virtual void write_r10(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r11 register
     * \param value New value of the rh850g4 r11 register
     */
    virtual void write_r11(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r12 register
     * \param value New value of the rh850g4 r12 register
     */
    virtual void write_r12(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r13 register
     * \param value New value of the rh850g4 r13 register
     */
    virtual void write_r13(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r14 register
     * \param value New value of the rh850g4 r14 register
     */
    virtual void write_r14(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r15 register
     * \param value New value of the rh850g4 r15 register
     */
    virtual void write_r15(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r16 register
     * \param value New value of the rh850g4 r16 register
     */
    virtual void write_r16(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r17 register
     * \param value New value of the rh850g4 r17 register
     */
    virtual void write_r17(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r18 register
     * \param value New value of the rh850g4 r18 register
     */
    virtual void write_r18(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r19 register
     * \param value New value of the rh850g4 r19 register
     */
    virtual void write_r19(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r20 register
     * \param value New value of the rh850g4 r20 register
     */
    virtual void write_r20(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r21 register
     * \param value New value of the rh850g4 r21 register
     */
    virtual void write_r21(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r22 register
     * \param value New value of the rh850g4 r22 register
     */
    virtual void write_r22(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r23 register
     * \param value New value of the rh850g4 r23 register
     */
    virtual void write_r23(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r24 register
     * \param value New value of the rh850g4 r24 register
     */
    virtual void write_r24(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r25 register
     * \param value New value of the rh850g4 r25 register
     */
    virtual void write_r25(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r26 register
     * \param value New value of the rh850g4 r26 register
     */
    virtual void write_r26(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r27 register
     * \param value New value of the rh850g4 r27 register
     */
    virtual void write_r27(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r28 register
     * \param value New value of the rh850g4 r28 register
     */
    virtual void write_r28(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r29 register
     * \param value New value of the rh850g4 r29 register
     */
    virtual void write_r29(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r30 register
     * \param value New value of the rh850g4 r30 register
     */
    virtual void write_r30(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 r31 register
     * \param value New value of the rh850g4 r31 register
     */
    virtual void write_r31(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PC register
     * \param value New value of the rh850g4 PC register
     */
    virtual void write_PC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr0 register
     * \param value New value of the rh850g4 wr0 register
     */
    virtual void write_wr0(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr0_u register
     * \param value New value of the rh850g4 wr0_u register
     */
    virtual void write_wr0_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr1 register
     * \param value New value of the rh850g4 wr1 register
     */
    virtual void write_wr1(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr1_u register
     * \param value New value of the rh850g4 wr1_u register
     */
    virtual void write_wr1_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr2 register
     * \param value New value of the rh850g4 wr2 register
     */
    virtual void write_wr2(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr2_u register
     * \param value New value of the rh850g4 wr2_u register
     */
    virtual void write_wr2_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr3 register
     * \param value New value of the rh850g4 wr3 register
     */
    virtual void write_wr3(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr3_u register
     * \param value New value of the rh850g4 wr3_u register
     */
    virtual void write_wr3_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr4 register
     * \param value New value of the rh850g4 wr4 register
     */
    virtual void write_wr4(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr4_u register
     * \param value New value of the rh850g4 wr4_u register
     */
    virtual void write_wr4_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr5 register
     * \param value New value of the rh850g4 wr5 register
     */
    virtual void write_wr5(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr5_u register
     * \param value New value of the rh850g4 wr5_u register
     */
    virtual void write_wr5_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr6 register
     * \param value New value of the rh850g4 wr6 register
     */
    virtual void write_wr6(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr6_u register
     * \param value New value of the rh850g4 wr6_u register
     */
    virtual void write_wr6_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr7 register
     * \param value New value of the rh850g4 wr7 register
     */
    virtual void write_wr7(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr7_u register
     * \param value New value of the rh850g4 wr7_u register
     */
    virtual void write_wr7_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr8 register
     * \param value New value of the rh850g4 wr8 register
     */
    virtual void write_wr8(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr8_u register
     * \param value New value of the rh850g4 wr8_u register
     */
    virtual void write_wr8_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr9 register
     * \param value New value of the rh850g4 wr9 register
     */
    virtual void write_wr9(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr9_u register
     * \param value New value of the rh850g4 wr9_u register
     */
    virtual void write_wr9_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr10 register
     * \param value New value of the rh850g4 wr10 register
     */
    virtual void write_wr10(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr10_u register
     * \param value New value of the rh850g4 wr10_u register
     */
    virtual void write_wr10_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr11 register
     * \param value New value of the rh850g4 wr11 register
     */
    virtual void write_wr11(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr11_u register
     * \param value New value of the rh850g4 wr11_u register
     */
    virtual void write_wr11_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr12 register
     * \param value New value of the rh850g4 wr12 register
     */
    virtual void write_wr12(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr12_u register
     * \param value New value of the rh850g4 wr12_u register
     */
    virtual void write_wr12_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr13 register
     * \param value New value of the rh850g4 wr13 register
     */
    virtual void write_wr13(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr13_u register
     * \param value New value of the rh850g4 wr13_u register
     */
    virtual void write_wr13_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr14 register
     * \param value New value of the rh850g4 wr14 register
     */
    virtual void write_wr14(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr14_u register
     * \param value New value of the rh850g4 wr14_u register
     */
    virtual void write_wr14_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr15 register
     * \param value New value of the rh850g4 wr15 register
     */
    virtual void write_wr15(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr15_u register
     * \param value New value of the rh850g4 wr15_u register
     */
    virtual void write_wr15_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr16 register
     * \param value New value of the rh850g4 wr16 register
     */
    virtual void write_wr16(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr16_u register
     * \param value New value of the rh850g4 wr16_u register
     */
    virtual void write_wr16_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr17 register
     * \param value New value of the rh850g4 wr17 register
     */
    virtual void write_wr17(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr17_u register
     * \param value New value of the rh850g4 wr17_u register
     */
    virtual void write_wr17_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr18 register
     * \param value New value of the rh850g4 wr18 register
     */
    virtual void write_wr18(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr18_u register
     * \param value New value of the rh850g4 wr18_u register
     */
    virtual void write_wr18_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr19 register
     * \param value New value of the rh850g4 wr19 register
     */
    virtual void write_wr19(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr19_u register
     * \param value New value of the rh850g4 wr19_u register
     */
    virtual void write_wr19_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr20 register
     * \param value New value of the rh850g4 wr20 register
     */
    virtual void write_wr20(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr20_u register
     * \param value New value of the rh850g4 wr20_u register
     */
    virtual void write_wr20_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr21 register
     * \param value New value of the rh850g4 wr21 register
     */
    virtual void write_wr21(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr21_u register
     * \param value New value of the rh850g4 wr21_u register
     */
    virtual void write_wr21_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr22 register
     * \param value New value of the rh850g4 wr22 register
     */
    virtual void write_wr22(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr22_u register
     * \param value New value of the rh850g4 wr22_u register
     */
    virtual void write_wr22_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr23 register
     * \param value New value of the rh850g4 wr23 register
     */
    virtual void write_wr23(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr23_u register
     * \param value New value of the rh850g4 wr23_u register
     */
    virtual void write_wr23_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr24 register
     * \param value New value of the rh850g4 wr24 register
     */
    virtual void write_wr24(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr24_u register
     * \param value New value of the rh850g4 wr24_u register
     */
    virtual void write_wr24_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr25 register
     * \param value New value of the rh850g4 wr25 register
     */
    virtual void write_wr25(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr25_u register
     * \param value New value of the rh850g4 wr25_u register
     */
    virtual void write_wr25_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr26 register
     * \param value New value of the rh850g4 wr26 register
     */
    virtual void write_wr26(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr26_u register
     * \param value New value of the rh850g4 wr26_u register
     */
    virtual void write_wr26_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr27 register
     * \param value New value of the rh850g4 wr27 register
     */
    virtual void write_wr27(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr27_u register
     * \param value New value of the rh850g4 wr27_u register
     */
    virtual void write_wr27_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr28 register
     * \param value New value of the rh850g4 wr28 register
     */
    virtual void write_wr28(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr28_u register
     * \param value New value of the rh850g4 wr28_u register
     */
    virtual void write_wr28_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr29 register
     * \param value New value of the rh850g4 wr29 register
     */
    virtual void write_wr29(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr29_u register
     * \param value New value of the rh850g4 wr29_u register
     */
    virtual void write_wr29_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr30 register
     * \param value New value of the rh850g4 wr30 register
     */
    virtual void write_wr30(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr30_u register
     * \param value New value of the rh850g4 wr30_u register
     */
    virtual void write_wr30_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr31 register
     * \param value New value of the rh850g4 wr31 register
     */
    virtual void write_wr31(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 wr31_u register
     * \param value New value of the rh850g4 wr31_u register
     */
    virtual void write_wr31_u(unsigned long long value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EIPC register
     * \param value New value of the rh850g4 EIPC register
     */
    virtual void write_EIPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EIPSW register
     * \param value New value of the rh850g4 EIPSW register
     */
    virtual void write_EIPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FEPC register
     * \param value New value of the rh850g4 FEPC register
     */
    virtual void write_FEPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FEPSW register
     * \param value New value of the rh850g4 FEPSW register
     */
    virtual void write_FEPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW register
     * \param value New value of the rh850g4 PSW register
     */
    virtual void write_PSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_Z register field
     * \param value New value of the rh850g4 PSW_Z register field
     */
    virtual void write_PSW_Z(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_S register field
     * \param value New value of the rh850g4 PSW_S register field
     */
    virtual void write_PSW_S(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_OV register field
     * \param value New value of the rh850g4 PSW_OV register field
     */
    virtual void write_PSW_OV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_CY register field
     * \param value New value of the rh850g4 PSW_CY register field
     */
    virtual void write_PSW_CY(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_SAT register field
     * \param value New value of the rh850g4 PSW_SAT register field
     */
    virtual void write_PSW_SAT(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_ID register field
     * \param value New value of the rh850g4 PSW_ID register field
     */
    virtual void write_PSW_ID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_EP register field
     * \param value New value of the rh850g4 PSW_EP register field
     */
    virtual void write_PSW_EP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_NP register field
     * \param value New value of the rh850g4 PSW_NP register field
     */
    virtual void write_PSW_NP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_EBV register field
     * \param value New value of the rh850g4 PSW_EBV register field
     */
    virtual void write_PSW_EBV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_CU0 register field
     * \param value New value of the rh850g4 PSW_CU0 register field
     */
    virtual void write_PSW_CU0(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_CU1 register field
     * \param value New value of the rh850g4 PSW_CU1 register field
     */
    virtual void write_PSW_CU1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_CU2 register field
     * \param value New value of the rh850g4 PSW_CU2 register field
     */
    virtual void write_PSW_CU2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_EIMASK register field
     * \param value New value of the rh850g4 PSW_EIMASK register field
     */
    virtual void write_PSW_EIMASK(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSW_UM register field
     * \param value New value of the rh850g4 PSW_UM register field
     */
    virtual void write_PSW_UM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR register
     * \param value New value of the rh850g4 FPSR register
     */
    virtual void write_FPSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_CC register field
     * \param value New value of the rh850g4 FPSR_CC register field
     */
    virtual void write_FPSR_CC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_FN register field
     * \param value New value of the rh850g4 FPSR_FN register field
     */
    virtual void write_FPSR_FN(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_IF register field
     * \param value New value of the rh850g4 FPSR_IF register field
     */
    virtual void write_FPSR_IF(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_PEM register field
     * \param value New value of the rh850g4 FPSR_PEM register field
     */
    virtual void write_FPSR_PEM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_RM register field
     * \param value New value of the rh850g4 FPSR_RM register field
     */
    virtual void write_FPSR_RM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_FS register field
     * \param value New value of the rh850g4 FPSR_FS register field
     */
    virtual void write_FPSR_FS(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_XC register field
     * \param value New value of the rh850g4 FPSR_XC register field
     */
    virtual void write_FPSR_XC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_XE register field
     * \param value New value of the rh850g4 FPSR_XE register field
     */
    virtual void write_FPSR_XE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPSR_XP register field
     * \param value New value of the rh850g4 FPSR_XP register field
     */
    virtual void write_FPSR_XP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPEPC register
     * \param value New value of the rh850g4 FPEPC register
     */
    virtual void write_FPEPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPST register
     * \param value New value of the rh850g4 FPST register
     */
    virtual void write_FPST(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPST_XC register field
     * \param value New value of the rh850g4 FPST_XC register field
     */
    virtual void write_FPST_XC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPST_IF register field
     * \param value New value of the rh850g4 FPST_IF register field
     */
    virtual void write_FPST_IF(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPST_XP register field
     * \param value New value of the rh850g4 FPST_XP register field
     */
    virtual void write_FPST_XP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPCC register
     * \param value New value of the rh850g4 FPCC register
     */
    virtual void write_FPCC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPCC_CC register field
     * \param value New value of the rh850g4 FPCC_CC register field
     */
    virtual void write_FPCC_CC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPCFG register
     * \param value New value of the rh850g4 FPCFG register
     */
    virtual void write_FPCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPCFG_RM register field
     * \param value New value of the rh850g4 FPCFG_RM register field
     */
    virtual void write_FPCFG_RM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPCFG_XE register field
     * \param value New value of the rh850g4 FPCFG_XE register field
     */
    virtual void write_FPCFG_XE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EIIC register
     * \param value New value of the rh850g4 EIIC register
     */
    virtual void write_EIIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FEIC register
     * \param value New value of the rh850g4 FEIC register
     */
    virtual void write_FEIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSWH register
     * \param value New value of the rh850g4 PSWH register
     */
    virtual void write_PSWH(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSWH_GPID register field
     * \param value New value of the rh850g4 PSWH_GPID register field
     */
    virtual void write_PSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PSWH_GM register field
     * \param value New value of the rh850g4 PSWH_GM register field
     */
    virtual void write_PSWH_GM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 CTPC register
     * \param value New value of the rh850g4 CTPC register
     */
    virtual void write_CTPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 CTPSW register
     * \param value New value of the rh850g4 CTPSW register
     */
    virtual void write_CTPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EIPSWH register
     * \param value New value of the rh850g4 EIPSWH register
     */
    virtual void write_EIPSWH(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EIPSWH_GPID register field
     * \param value New value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void write_EIPSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EIPSWH_GM register field
     * \param value New value of the rh850g4 EIPSWH_GM register field
     */
    virtual void write_EIPSWH_GM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FEPSWH register
     * \param value New value of the rh850g4 FEPSWH register
     */
    virtual void write_FEPSWH(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FEPSWH_GPID register field
     * \param value New value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void write_FEPSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FEPSWH_GM register field
     * \param value New value of the rh850g4 FEPSWH_GM register field
     */
    virtual void write_FEPSWH_GM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 CTBP register
     * \param value New value of the rh850g4 CTBP register
     */
    virtual void write_CTBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 SNZCFG register
     * \param value New value of the rh850g4 SNZCFG register
     */
    virtual void write_SNZCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EIWR register
     * \param value New value of the rh850g4 EIWR register
     */
    virtual void write_EIWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FEWR register
     * \param value New value of the rh850g4 FEWR register
     */
    virtual void write_FEWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 SPID register
     * \param value New value of the rh850g4 SPID register
     */
    virtual void write_SPID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 SPIDLIST register
     * \param value New value of the rh850g4 SPIDLIST register
     */
    virtual void write_SPIDLIST(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBASE register
     * \param value New value of the rh850g4 RBASE register
     */
    virtual void write_RBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBASE_RBASE register field
     * \param value New value of the rh850g4 RBASE_RBASE register field
     */
    virtual void write_RBASE_RBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBASE_DV register field
     * \param value New value of the rh850g4 RBASE_DV register field
     */
    virtual void write_RBASE_DV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBASE_RINT register field
     * \param value New value of the rh850g4 RBASE_RINT register field
     */
    virtual void write_RBASE_RINT(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EBASE register
     * \param value New value of the rh850g4 EBASE register
     */
    virtual void write_EBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EBASE_EBASE register field
     * \param value New value of the rh850g4 EBASE_EBASE register field
     */
    virtual void write_EBASE_EBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EBASE_DV register field
     * \param value New value of the rh850g4 EBASE_DV register field
     */
    virtual void write_EBASE_DV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 EBASE_RINT register field
     * \param value New value of the rh850g4 EBASE_RINT register field
     */
    virtual void write_EBASE_RINT(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 INTBP register
     * \param value New value of the rh850g4 INTBP register
     */
    virtual void write_INTBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCTL register
     * \param value New value of the rh850g4 MCTL register
     */
    virtual void write_MCTL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCTL_UIC register field
     * \param value New value of the rh850g4 MCTL_UIC register field
     */
    virtual void write_MCTL_UIC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PID register
     * \param value New value of the rh850g4 PID register
     */
    virtual void write_PID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 SVLOCK register
     * \param value New value of the rh850g4 SVLOCK register
     */
    virtual void write_SVLOCK(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 SCCFG register
     * \param value New value of the rh850g4 SCCFG register
     */
    virtual void write_SCCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 SCCFG_SIZE register field
     * \param value New value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void write_SCCFG_SIZE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 SCBP register
     * \param value New value of the rh850g4 SCBP register
     */
    virtual void write_SCBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 HVCFG register
     * \param value New value of the rh850g4 HVCFG register
     */
    virtual void write_HVCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 HVCFG_HVE register field
     * \param value New value of the rh850g4 HVCFG_HVE register field
     */
    virtual void write_HVCFG_HVE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMCFG register
     * \param value New value of the rh850g4 GMCFG register
     */
    virtual void write_GMCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GMP register field
     * \param value New value of the rh850g4 GMCFG_GMP register field
     */
    virtual void write_GMCFG_GMP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMCFG_HMP register field
     * \param value New value of the rh850g4 GMCFG_HMP register field
     */
    virtual void write_GMCFG_HMP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GSYSE register field
     * \param value New value of the rh850g4 GMCFG_GSYSE register field
     */
    virtual void write_GMCFG_GSYSE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU0 register field
     * \param value New value of the rh850g4 GMCFG_GCU0 register field
     */
    virtual void write_GMCFG_GCU0(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU1 register field
     * \param value New value of the rh850g4 GMCFG_GCU1 register field
     */
    virtual void write_GMCFG_GCU1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU2 register field
     * \param value New value of the rh850g4 GMCFG_GCU2 register field
     */
    virtual void write_GMCFG_GCU2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 HVSB register
     * \param value New value of the rh850g4 HVSB register
     */
    virtual void write_HVSB(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PEID register
     * \param value New value of the rh850g4 PEID register
     */
    virtual void write_PEID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BMID register
     * \param value New value of the rh850g4 BMID register
     */
    virtual void write_BMID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BMID_BMID register field
     * \param value New value of the rh850g4 BMID_BMID register field
     */
    virtual void write_BMID_BMID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MEA register
     * \param value New value of the rh850g4 MEA register
     */
    virtual void write_MEA(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MEI register
     * \param value New value of the rh850g4 MEI register
     */
    virtual void write_MEI(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBCR0 register
     * \param value New value of the rh850g4 RBCR0 register
     */
    virtual void write_RBCR0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBCR0_MD register field
     * \param value New value of the rh850g4 RBCR0_MD register field
     */
    virtual void write_RBCR0_MD(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBCR0_BE register field
     * \param value New value of the rh850g4 RBCR0_BE register field
     */
    virtual void write_RBCR0_BE(unsigned short value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBCR1 register
     * \param value New value of the rh850g4 RBCR1 register
     */
    virtual void write_RBCR1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBCR1_NC register field
     * \param value New value of the rh850g4 RBCR1_NC register field
     */
    virtual void write_RBCR1_NC(unsigned short value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBNR register
     * \param value New value of the rh850g4 RBNR register
     */
    virtual void write_RBNR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBNR_BN register field
     * \param value New value of the rh850g4 RBNR_BN register field
     */
    virtual void write_RBNR_BN(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBIP register
     * \param value New value of the rh850g4 RBIP register
     */
    virtual void write_RBIP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBIP_RBIP register field
     * \param value New value of the rh850g4 RBIP_RBIP register field
     */
    virtual void write_RBIP_RBIP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ISPR register
     * \param value New value of the rh850g4 ISPR register
     */
    virtual void write_ISPR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ISPR_ISP register field
     * \param value New value of the rh850g4 ISPR_ISP register field
     */
    virtual void write_ISPR_ISP(unsigned short value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR register
     * \param value New value of the rh850g4 IMSR register
     */
    virtual void write_IMSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_EEIM register field
     * \param value New value of the rh850g4 IMSR_EEIM register field
     */
    virtual void write_IMSR_EEIM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_EPLM register field
     * \param value New value of the rh850g4 IMSR_EPLM register field
     */
    virtual void write_IMSR_EPLM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_EID register field
     * \param value New value of the rh850g4 IMSR_EID register field
     */
    virtual void write_IMSR_EID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_ENP register field
     * \param value New value of the rh850g4 IMSR_ENP register field
     */
    virtual void write_IMSR_ENP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_FNP register field
     * \param value New value of the rh850g4 IMSR_FNP register field
     */
    virtual void write_IMSR_FNP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEEIM register field
     * \param value New value of the rh850g4 IMSR_HEEIM register field
     */
    virtual void write_IMSR_HEEIM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEPLM register field
     * \param value New value of the rh850g4 IMSR_HEPLM register field
     */
    virtual void write_IMSR_HEPLM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEID register field
     * \param value New value of the rh850g4 IMSR_HEID register field
     */
    virtual void write_IMSR_HEID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_HENP register field
     * \param value New value of the rh850g4 IMSR_HENP register field
     */
    virtual void write_IMSR_HENP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR_HFNP register field
     * \param value New value of the rh850g4 IMSR_HFNP register field
     */
    virtual void write_IMSR_HFNP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICSR register
     * \param value New value of the rh850g4 ICSR register
     */
    virtual void write_ICSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICSR_PMEI register field
     * \param value New value of the rh850g4 ICSR_PMEI register field
     */
    virtual void write_ICSR_PMEI(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 INTCFG register
     * \param value New value of the rh850g4 INTCFG register
     */
    virtual void write_INTCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 INTCFG_ISPC register field
     * \param value New value of the rh850g4 INTCFG_ISPC register field
     */
    virtual void write_INTCFG_ISPC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 INTCFG_EPL register field
     * \param value New value of the rh850g4 INTCFG_EPL register field
     */
    virtual void write_INTCFG_EPL(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 INTCFG_ULNR register field
     * \param value New value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void write_INTCFG_ULNR(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PLMR register
     * \param value New value of the rh850g4 PLMR register
     */
    virtual void write_PLMR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PLMR_PLM register field
     * \param value New value of the rh850g4 PLMR_PLM register field
     */
    virtual void write_PLMR_PLM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR register
     * \param value New value of the rh850g4 FXSR register
     */
    virtual void write_FXSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_FN register field
     * \param value New value of the rh850g4 FXSR_FN register field
     */
    virtual void write_FXSR_FN(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_IF register field
     * \param value New value of the rh850g4 FXSR_IF register field
     */
    virtual void write_FXSR_IF(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_PEM register field
     * \param value New value of the rh850g4 FXSR_PEM register field
     */
    virtual void write_FXSR_PEM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_RM register field
     * \param value New value of the rh850g4 FXSR_RM register field
     */
    virtual void write_FXSR_RM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_FS register field
     * \param value New value of the rh850g4 FXSR_FS register field
     */
    virtual void write_FXSR_FS(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_XC register field
     * \param value New value of the rh850g4 FXSR_XC register field
     */
    virtual void write_FXSR_XC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_XE register field
     * \param value New value of the rh850g4 FXSR_XE register field
     */
    virtual void write_FXSR_XE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR_XP register field
     * \param value New value of the rh850g4 FXSR_XP register field
     */
    virtual void write_FXSR_XP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXST register
     * \param value New value of the rh850g4 FXST register
     */
    virtual void write_FXST(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXST_XC register field
     * \param value New value of the rh850g4 FXST_XC register field
     */
    virtual void write_FXST_XC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXST_IF register field
     * \param value New value of the rh850g4 FXST_IF register field
     */
    virtual void write_FXST_IF(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXST_XP register field
     * \param value New value of the rh850g4 FXST_XP register field
     */
    virtual void write_FXST_XP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXINFO register
     * \param value New value of the rh850g4 FXINFO register
     */
    virtual void write_FXINFO(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXINFO_NFPU register field
     * \param value New value of the rh850g4 FXINFO_NFPU register field
     */
    virtual void write_FXINFO_NFPU(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXINFO_RSIZE register field
     * \param value New value of the rh850g4 FXINFO_RSIZE register field
     */
    virtual void write_FXINFO_RSIZE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXCFG register
     * \param value New value of the rh850g4 FXCFG register
     */
    virtual void write_FXCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXCFG_RM register field
     * \param value New value of the rh850g4 FXCFG_RM register field
     */
    virtual void write_FXCFG_RM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXCFG_XE register field
     * \param value New value of the rh850g4 FXCFG_XE register field
     */
    virtual void write_FXCFG_XE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXC register
     * \param value New value of the rh850g4 FXXC register
     */
    virtual void write_FXXC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC3 register field
     * \param value New value of the rh850g4 FXXC_XC3 register field
     */
    virtual void write_FXXC_XC3(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC2 register field
     * \param value New value of the rh850g4 FXXC_XC2 register field
     */
    virtual void write_FXXC_XC2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC1 register field
     * \param value New value of the rh850g4 FXXC_XC1 register field
     */
    virtual void write_FXXC_XC1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC0 register field
     * \param value New value of the rh850g4 FXXC_XC0 register field
     */
    virtual void write_FXXC_XC0(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXP register
     * \param value New value of the rh850g4 FXXP register
     */
    virtual void write_FXXP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP3 register field
     * \param value New value of the rh850g4 FXXP_XP3 register field
     */
    virtual void write_FXXP_XP3(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP2 register field
     * \param value New value of the rh850g4 FXXP_XP2 register field
     */
    virtual void write_FXXP_XP2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP1 register field
     * \param value New value of the rh850g4 FXXP_XP1 register field
     */
    virtual void write_FXXP_XP1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP0 register field
     * \param value New value of the rh850g4 FXXP_XP0 register field
     */
    virtual void write_FXXP_XP0(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEIPC register
     * \param value New value of the rh850g4 GMEIPC register
     */
    virtual void write_GMEIPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEIPSW register
     * \param value New value of the rh850g4 GMEIPSW register
     */
    virtual void write_GMEIPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMFEPC register
     * \param value New value of the rh850g4 GMFEPC register
     */
    virtual void write_GMFEPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMFEPSW register
     * \param value New value of the rh850g4 GMFEPSW register
     */
    virtual void write_GMFEPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW register
     * \param value New value of the rh850g4 GMPSW register
     */
    virtual void write_GMPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_Z register field
     * \param value New value of the rh850g4 GMPSW_Z register field
     */
    virtual void write_GMPSW_Z(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_S register field
     * \param value New value of the rh850g4 GMPSW_S register field
     */
    virtual void write_GMPSW_S(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_OV register field
     * \param value New value of the rh850g4 GMPSW_OV register field
     */
    virtual void write_GMPSW_OV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CY register field
     * \param value New value of the rh850g4 GMPSW_CY register field
     */
    virtual void write_GMPSW_CY(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_SAT register field
     * \param value New value of the rh850g4 GMPSW_SAT register field
     */
    virtual void write_GMPSW_SAT(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_ID register field
     * \param value New value of the rh850g4 GMPSW_ID register field
     */
    virtual void write_GMPSW_ID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EP register field
     * \param value New value of the rh850g4 GMPSW_EP register field
     */
    virtual void write_GMPSW_EP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_NP register field
     * \param value New value of the rh850g4 GMPSW_NP register field
     */
    virtual void write_GMPSW_NP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EBV register field
     * \param value New value of the rh850g4 GMPSW_EBV register field
     */
    virtual void write_GMPSW_EBV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU0 register field
     * \param value New value of the rh850g4 GMPSW_CU0 register field
     */
    virtual void write_GMPSW_CU0(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU1 register field
     * \param value New value of the rh850g4 GMPSW_CU1 register field
     */
    virtual void write_GMPSW_CU1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU2 register field
     * \param value New value of the rh850g4 GMPSW_CU2 register field
     */
    virtual void write_GMPSW_CU2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EIMASK register field
     * \param value New value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void write_GMPSW_EIMASK(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPSW_UM register field
     * \param value New value of the rh850g4 GMPSW_UM register field
     */
    virtual void write_GMPSW_UM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMMEA register
     * \param value New value of the rh850g4 GMMEA register
     */
    virtual void write_GMMEA(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMMEI register
     * \param value New value of the rh850g4 GMMEI register
     */
    virtual void write_GMMEI(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEIIC register
     * \param value New value of the rh850g4 GMEIIC register
     */
    virtual void write_GMEIIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMFEIC register
     * \param value New value of the rh850g4 GMFEIC register
     */
    virtual void write_GMFEIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMSPID register
     * \param value New value of the rh850g4 GMSPID register
     */
    virtual void write_GMSPID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMSPIDLIST register
     * \param value New value of the rh850g4 GMSPIDLIST register
     */
    virtual void write_GMSPIDLIST(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEBASE register
     * \param value New value of the rh850g4 GMEBASE register
     */
    virtual void write_GMEBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_EBASE register field
     * \param value New value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void write_GMEBASE_EBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_DV register field
     * \param value New value of the rh850g4 GMEBASE_DV register field
     */
    virtual void write_GMEBASE_DV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_RINT register field
     * \param value New value of the rh850g4 GMEBASE_RINT register field
     */
    virtual void write_GMEBASE_RINT(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMINTBP register
     * \param value New value of the rh850g4 GMINTBP register
     */
    virtual void write_GMINTBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG register
     * \param value New value of the rh850g4 GMINTCFG register
     */
    virtual void write_GMINTCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_ISPC register field
     * \param value New value of the rh850g4 GMINTCFG_ISPC register field
     */
    virtual void write_GMINTCFG_ISPC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_EPL register field
     * \param value New value of the rh850g4 GMINTCFG_EPL register field
     */
    virtual void write_GMINTCFG_EPL(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_ULNR register field
     * \param value New value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void write_GMINTCFG_ULNR(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPLMR register
     * \param value New value of the rh850g4 GMPLMR register
     */
    virtual void write_GMPLMR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPLMR_PLM register field
     * \param value New value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void write_GMPLMR_PLM(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMSVLOCK register
     * \param value New value of the rh850g4 GMSVLOCK register
     */
    virtual void write_GMSVLOCK(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMMPM register
     * \param value New value of the rh850g4 GMMPM register
     */
    virtual void write_GMMPM(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMEIWR register
     * \param value New value of the rh850g4 GMEIWR register
     */
    virtual void write_GMEIWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMFEWR register
     * \param value New value of the rh850g4 GMFEWR register
     */
    virtual void write_GMFEWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 GMPEID register
     * \param value New value of the rh850g4 GMPEID register
     */
    virtual void write_GMPEID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPM register
     * \param value New value of the rh850g4 MPM register
     */
    virtual void write_MPM(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPCFG register
     * \param value New value of the rh850g4 MPCFG register
     */
    virtual void write_MPCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPCFG_NMPUE register field
     * \param value New value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void write_MPCFG_NMPUE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPCFG_HBE register field
     * \param value New value of the rh850g4 MPCFG_HBE register field
     */
    virtual void write_MPCFG_HBE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPCFG_ARCH register field
     * \param value New value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void write_MPCFG_ARCH(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPCFG_NBK register field
     * \param value New value of the rh850g4 MPCFG_NBK register field
     */
    virtual void write_MPCFG_NBK(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPCFG_DMDP register field
     * \param value New value of the rh850g4 MPCFG_DMDP register field
     */
    virtual void write_MPCFG_DMDP(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCA register
     * \param value New value of the rh850g4 MCA register
     */
    virtual void write_MCA(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCS register
     * \param value New value of the rh850g4 MCS register
     */
    virtual void write_MCS(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCC register
     * \param value New value of the rh850g4 MCC register
     */
    virtual void write_MCC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR register
     * \param value New value of the rh850g4 MCR register
     */
    virtual void write_MCR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_HSXE register field
     * \param value New value of the rh850g4 MCR_HSXE register field
     */
    virtual void write_MCR_HSXE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_HSWE register field
     * \param value New value of the rh850g4 MCR_HSWE register field
     */
    virtual void write_MCR_HSWE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_HSRE register field
     * \param value New value of the rh850g4 MCR_HSRE register field
     */
    virtual void write_MCR_HSRE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_HUXE register field
     * \param value New value of the rh850g4 MCR_HUXE register field
     */
    virtual void write_MCR_HUXE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_HUWE register field
     * \param value New value of the rh850g4 MCR_HUWE register field
     */
    virtual void write_MCR_HUWE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_HURE register field
     * \param value New value of the rh850g4 MCR_HURE register field
     */
    virtual void write_MCR_HURE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_GSXE register field
     * \param value New value of the rh850g4 MCR_GSXE register field
     */
    virtual void write_MCR_GSXE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_GSWE register field
     * \param value New value of the rh850g4 MCR_GSWE register field
     */
    virtual void write_MCR_GSWE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_GSRE register field
     * \param value New value of the rh850g4 MCR_GSRE register field
     */
    virtual void write_MCR_GSRE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_GUXE register field
     * \param value New value of the rh850g4 MCR_GUXE register field
     */
    virtual void write_MCR_GUXE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_GUWE register field
     * \param value New value of the rh850g4 MCR_GUWE register field
     */
    virtual void write_MCR_GUWE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_GURE register field
     * \param value New value of the rh850g4 MCR_GURE register field
     */
    virtual void write_MCR_GURE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_OV register field
     * \param value New value of the rh850g4 MCR_OV register field
     */
    virtual void write_MCR_OV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_SXE register field
     * \param value New value of the rh850g4 MCR_SXE register field
     */
    virtual void write_MCR_SXE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_SWE register field
     * \param value New value of the rh850g4 MCR_SWE register field
     */
    virtual void write_MCR_SWE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_SRE register field
     * \param value New value of the rh850g4 MCR_SRE register field
     */
    virtual void write_MCR_SRE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_UXE register field
     * \param value New value of the rh850g4 MCR_UXE register field
     */
    virtual void write_MCR_UXE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_UWE register field
     * \param value New value of the rh850g4 MCR_UWE register field
     */
    virtual void write_MCR_UWE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCR_URE register field
     * \param value New value of the rh850g4 MCR_URE register field
     */
    virtual void write_MCR_URE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCI register
     * \param value New value of the rh850g4 MCI register
     */
    virtual void write_MCI(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MCI_SPID register field
     * \param value New value of the rh850g4 MCI_SPID register field
     */
    virtual void write_MCI_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPIDX register
     * \param value New value of the rh850g4 MPIDX register
     */
    virtual void write_MPIDX(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPIDX_IDX register field
     * \param value New value of the rh850g4 MPIDX_IDX register field
     */
    virtual void write_MPIDX_IDX(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPBK register
     * \param value New value of the rh850g4 MPBK register
     */
    virtual void write_MPBK(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPBK_BK register field
     * \param value New value of the rh850g4 MPBK_BK register field
     */
    virtual void write_MPBK_BK(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPLA register
     * \param value New value of the rh850g4 MPLA register
     */
    virtual void write_MPLA(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPUA register
     * \param value New value of the rh850g4 MPUA register
     */
    virtual void write_MPUA(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT register
     * \param value New value of the rh850g4 MPAT register
     */
    virtual void write_MPAT(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID7 register field
     * \param value New value of the rh850g4 MPAT_WMPID7 register field
     */
    virtual void write_MPAT_WMPID7(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID6 register field
     * \param value New value of the rh850g4 MPAT_WMPID6 register field
     */
    virtual void write_MPAT_WMPID6(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID5 register field
     * \param value New value of the rh850g4 MPAT_WMPID5 register field
     */
    virtual void write_MPAT_WMPID5(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID4 register field
     * \param value New value of the rh850g4 MPAT_WMPID4 register field
     */
    virtual void write_MPAT_WMPID4(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID3 register field
     * \param value New value of the rh850g4 MPAT_WMPID3 register field
     */
    virtual void write_MPAT_WMPID3(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID2 register field
     * \param value New value of the rh850g4 MPAT_WMPID2 register field
     */
    virtual void write_MPAT_WMPID2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID1 register field
     * \param value New value of the rh850g4 MPAT_WMPID1 register field
     */
    virtual void write_MPAT_WMPID1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID0 register field
     * \param value New value of the rh850g4 MPAT_WMPID0 register field
     */
    virtual void write_MPAT_WMPID0(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID7 register field
     * \param value New value of the rh850g4 MPAT_RMPID7 register field
     */
    virtual void write_MPAT_RMPID7(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID6 register field
     * \param value New value of the rh850g4 MPAT_RMPID6 register field
     */
    virtual void write_MPAT_RMPID6(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID5 register field
     * \param value New value of the rh850g4 MPAT_RMPID5 register field
     */
    virtual void write_MPAT_RMPID5(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID4 register field
     * \param value New value of the rh850g4 MPAT_RMPID4 register field
     */
    virtual void write_MPAT_RMPID4(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID3 register field
     * \param value New value of the rh850g4 MPAT_RMPID3 register field
     */
    virtual void write_MPAT_RMPID3(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID2 register field
     * \param value New value of the rh850g4 MPAT_RMPID2 register field
     */
    virtual void write_MPAT_RMPID2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID1 register field
     * \param value New value of the rh850g4 MPAT_RMPID1 register field
     */
    virtual void write_MPAT_RMPID1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID0 register field
     * \param value New value of the rh850g4 MPAT_RMPID0 register field
     */
    virtual void write_MPAT_RMPID0(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_WG register field
     * \param value New value of the rh850g4 MPAT_WG register field
     */
    virtual void write_MPAT_WG(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_RG register field
     * \param value New value of the rh850g4 MPAT_RG register field
     */
    virtual void write_MPAT_RG(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_E register field
     * \param value New value of the rh850g4 MPAT_E register field
     */
    virtual void write_MPAT_E(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_SX register field
     * \param value New value of the rh850g4 MPAT_SX register field
     */
    virtual void write_MPAT_SX(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_SW register field
     * \param value New value of the rh850g4 MPAT_SW register field
     */
    virtual void write_MPAT_SW(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_SR register field
     * \param value New value of the rh850g4 MPAT_SR register field
     */
    virtual void write_MPAT_SR(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_UX register field
     * \param value New value of the rh850g4 MPAT_UX register field
     */
    virtual void write_MPAT_UX(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_UW register field
     * \param value New value of the rh850g4 MPAT_UW register field
     */
    virtual void write_MPAT_UW(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPAT_UR register field
     * \param value New value of the rh850g4 MPAT_UR register field
     */
    virtual void write_MPAT_UR(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID0 register
     * \param value New value of the rh850g4 MPID0 register
     */
    virtual void write_MPID0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID0_SPID register field
     * \param value New value of the rh850g4 MPID0_SPID register field
     */
    virtual void write_MPID0_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID1 register
     * \param value New value of the rh850g4 MPID1 register
     */
    virtual void write_MPID1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID1_SPID register field
     * \param value New value of the rh850g4 MPID1_SPID register field
     */
    virtual void write_MPID1_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID2 register
     * \param value New value of the rh850g4 MPID2 register
     */
    virtual void write_MPID2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID2_SPID register field
     * \param value New value of the rh850g4 MPID2_SPID register field
     */
    virtual void write_MPID2_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID3 register
     * \param value New value of the rh850g4 MPID3 register
     */
    virtual void write_MPID3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID3_SPID register field
     * \param value New value of the rh850g4 MPID3_SPID register field
     */
    virtual void write_MPID3_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID4 register
     * \param value New value of the rh850g4 MPID4 register
     */
    virtual void write_MPID4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID4_SPID register field
     * \param value New value of the rh850g4 MPID4_SPID register field
     */
    virtual void write_MPID4_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID5 register
     * \param value New value of the rh850g4 MPID5 register
     */
    virtual void write_MPID5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID5_SPID register field
     * \param value New value of the rh850g4 MPID5_SPID register field
     */
    virtual void write_MPID5_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID6 register
     * \param value New value of the rh850g4 MPID6 register
     */
    virtual void write_MPID6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID6_SPID register field
     * \param value New value of the rh850g4 MPID6_SPID register field
     */
    virtual void write_MPID6_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID7 register
     * \param value New value of the rh850g4 MPID7 register
     */
    virtual void write_MPID7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID7_SPID register field
     * \param value New value of the rh850g4 MPID7_SPID register field
     */
    virtual void write_MPID7_SPID(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGL register
     * \param value New value of the rh850g4 ICTAGL register
     */
    virtual void write_ICTAGL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_LPN register field
     * \param value New value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void write_ICTAGL_LPN(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_LRU register field
     * \param value New value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void write_ICTAGL_LRU(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_L register field
     * \param value New value of the rh850g4 ICTAGL_L register field
     */
    virtual void write_ICTAGL_L(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_V register field
     * \param value New value of the rh850g4 ICTAGL_V register field
     */
    virtual void write_ICTAGL_V(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH register
     * \param value New value of the rh850g4 ICTAGH register
     */
    virtual void write_ICTAGH(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_WD register field
     * \param value New value of the rh850g4 ICTAGH_WD register field
     */
    virtual void write_ICTAGH_WD(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_PD register field
     * \param value New value of the rh850g4 ICTAGH_PD register field
     */
    virtual void write_ICTAGH_PD(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_WT register field
     * \param value New value of the rh850g4 ICTAGH_WT register field
     */
    virtual void write_ICTAGH_WT(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_PT register field
     * \param value New value of the rh850g4 ICTAGH_PT register field
     */
    virtual void write_ICTAGH_PT(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_DATAECC register field
     * \param value New value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void write_ICTAGH_DATAECC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_TAGECC register field
     * \param value New value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void write_ICTAGH_TAGECC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICDATL register
     * \param value New value of the rh850g4 ICDATL register
     */
    virtual void write_ICDATL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICDATH register
     * \param value New value of the rh850g4 ICDATH register
     */
    virtual void write_ICDATH(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCTRL register
     * \param value New value of the rh850g4 ICCTRL register
     */
    virtual void write_ICCTRL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_PBS register field
     * \param value New value of the rh850g4 ICCTRL_PBS register field
     */
    virtual void write_ICCTRL_PBS(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHCLR register field
     * \param value New value of the rh850g4 ICCTRL_ICHCLR register field
     */
    virtual void write_ICCTRL_ICHCLR(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEIV register field
     * \param value New value of the rh850g4 ICCTRL_ICHEIV register field
     */
    virtual void write_ICCTRL_ICHEIV(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEMK register field
     * \param value New value of the rh850g4 ICCTRL_ICHEMK register field
     */
    virtual void write_ICCTRL_ICHEMK(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEN register field
     * \param value New value of the rh850g4 ICCTRL_ICHEN register field
     */
    virtual void write_ICCTRL_ICHEN(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCFG register
     * \param value New value of the rh850g4 ICCFG register
     */
    virtual void write_ICCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHSIZE register field
     * \param value New value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void write_ICCFG_ICHSIZE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHLINE register field
     * \param value New value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void write_ICCFG_ICHLINE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHWAY register field
     * \param value New value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void write_ICCFG_ICHWAY(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR register
     * \param value New value of the rh850g4 ICERR register
     */
    virtual void write_ICERR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_CISTW register field
     * \param value New value of the rh850g4 ICERR_CISTW register field
     */
    virtual void write_ICERR_CISTW(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESAFE register field
     * \param value New value of the rh850g4 ICERR_ESAFE register field
     */
    virtual void write_ICERR_ESAFE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESMH register field
     * \param value New value of the rh850g4 ICERR_ESMH register field
     */
    virtual void write_ICERR_ESMH(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESPBSE register field
     * \param value New value of the rh850g4 ICERR_ESPBSE register field
     */
    virtual void write_ICERR_ESPBSE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESTE1 register field
     * \param value New value of the rh850g4 ICERR_ESTE1 register field
     */
    virtual void write_ICERR_ESTE1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESTE2 register field
     * \param value New value of the rh850g4 ICERR_ESTE2 register field
     */
    virtual void write_ICERR_ESTE2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESDC register field
     * \param value New value of the rh850g4 ICERR_ESDC register field
     */
    virtual void write_ICERR_ESDC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESDE register field
     * \param value New value of the rh850g4 ICERR_ESDE register field
     */
    virtual void write_ICERR_ESDE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMAFE register field
     * \param value New value of the rh850g4 ICERR_ERMAFE register field
     */
    virtual void write_ICERR_ERMAFE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMMH register field
     * \param value New value of the rh850g4 ICERR_ERMMH register field
     */
    virtual void write_ICERR_ERMMH(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMPBSE register field
     * \param value New value of the rh850g4 ICERR_ERMPBSE register field
     */
    virtual void write_ICERR_ERMPBSE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMTE1 register field
     * \param value New value of the rh850g4 ICERR_ERMTE1 register field
     */
    virtual void write_ICERR_ERMTE1(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMTE2 register field
     * \param value New value of the rh850g4 ICERR_ERMTE2 register field
     */
    virtual void write_ICERR_ERMTE2(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMDC register field
     * \param value New value of the rh850g4 ICERR_ERMDC register field
     */
    virtual void write_ICERR_ERMDC(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMDE register field
     * \param value New value of the rh850g4 ICERR_ERMDE register field
     */
    virtual void write_ICERR_ERMDE(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHEWY register field
     * \param value New value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void write_ICERR_ICHEWY(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHEIX register field
     * \param value New value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void write_ICERR_ICHEIX(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHERQ register field
     * \param value New value of the rh850g4 ICERR_ICHERQ register field
     */
    virtual void write_ICERR_ICHERQ(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHED register field
     * \param value New value of the rh850g4 ICERR_ICHED register field
     */
    virtual void write_ICERR_ICHED(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHET register field
     * \param value New value of the rh850g4 ICERR_ICHET register field
     */
    virtual void write_ICERR_ICHET(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHERR register field
     * \param value New value of the rh850g4 ICERR_ICHERR register field
     */
    virtual void write_ICERR_ICHERR(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 TSCOUNTL register
     * \param value New value of the rh850g4 TSCOUNTL register
     */
    virtual void write_TSCOUNTL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 TSCOUNTH register
     * \param value New value of the rh850g4 TSCOUNTH register
     */
    virtual void write_TSCOUNTH(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 TSCTRL register
     * \param value New value of the rh850g4 TSCTRL register
     */
    virtual void write_TSCTRL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMUMCTRL register
     * \param value New value of the rh850g4 PMUMCTRL register
     */
    virtual void write_PMUMCTRL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMGMCTRL register
     * \param value New value of the rh850g4 PMGMCTRL register
     */
    virtual void write_PMGMCTRL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT0 register
     * \param value New value of the rh850g4 PMCOUNT0 register
     */
    virtual void write_PMCOUNT0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL0 register
     * \param value New value of the rh850g4 PMCTRL0 register
     */
    virtual void write_PMCTRL0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT1 register
     * \param value New value of the rh850g4 PMCOUNT1 register
     */
    virtual void write_PMCOUNT1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL1 register
     * \param value New value of the rh850g4 PMCTRL1 register
     */
    virtual void write_PMCTRL1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT2 register
     * \param value New value of the rh850g4 PMCOUNT2 register
     */
    virtual void write_PMCOUNT2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL2 register
     * \param value New value of the rh850g4 PMCTRL2 register
     */
    virtual void write_PMCTRL2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT3 register
     * \param value New value of the rh850g4 PMCOUNT3 register
     */
    virtual void write_PMCOUNT3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL3 register
     * \param value New value of the rh850g4 PMCTRL3 register
     */
    virtual void write_PMCTRL3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL4 register
     * \param value New value of the rh850g4 PMCTRL4 register
     */
    virtual void write_PMCTRL4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL5 register
     * \param value New value of the rh850g4 PMCTRL5 register
     */
    virtual void write_PMCTRL5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL6 register
     * \param value New value of the rh850g4 PMCTRL6 register
     */
    virtual void write_PMCTRL6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCTRL7 register
     * \param value New value of the rh850g4 PMCTRL7 register
     */
    virtual void write_PMCTRL7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT4 register
     * \param value New value of the rh850g4 PMCOUNT4 register
     */
    virtual void write_PMCOUNT4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT5 register
     * \param value New value of the rh850g4 PMCOUNT5 register
     */
    virtual void write_PMCOUNT5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT6 register
     * \param value New value of the rh850g4 PMCOUNT6 register
     */
    virtual void write_PMCOUNT6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT7 register
     * \param value New value of the rh850g4 PMCOUNT7 register
     */
    virtual void write_PMCOUNT7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND0 register
     * \param value New value of the rh850g4 PMSUBCND0 register
     */
    virtual void write_PMSUBCND0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND1 register
     * \param value New value of the rh850g4 PMSUBCND1 register
     */
    virtual void write_PMSUBCND1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND2 register
     * \param value New value of the rh850g4 PMSUBCND2 register
     */
    virtual void write_PMSUBCND2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND3 register
     * \param value New value of the rh850g4 PMSUBCND3 register
     */
    virtual void write_PMSUBCND3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND4 register
     * \param value New value of the rh850g4 PMSUBCND4 register
     */
    virtual void write_PMSUBCND4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND5 register
     * \param value New value of the rh850g4 PMSUBCND5 register
     */
    virtual void write_PMSUBCND5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND6 register
     * \param value New value of the rh850g4 PMSUBCND6 register
     */
    virtual void write_PMSUBCND6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND7 register
     * \param value New value of the rh850g4 PMSUBCND7 register
     */
    virtual void write_PMSUBCND7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DBGEN register
     * \param value New value of the rh850g4 DBGEN register
     */
    virtual void write_DBGEN(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DBPSWH register
     * \param value New value of the rh850g4 DBPSWH register
     */
    virtual void write_DBPSWH(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DBIC register
     * \param value New value of the rh850g4 DBIC register
     */
    virtual void write_DBIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DBPC register
     * \param value New value of the rh850g4 DBPC register
     */
    virtual void write_DBPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DBPSW register
     * \param value New value of the rh850g4 DBPSW register
     */
    virtual void write_DBPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DIR0 register
     * \param value New value of the rh850g4 DIR0 register
     */
    virtual void write_DIR0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DIR1 register
     * \param value New value of the rh850g4 DIR1 register
     */
    virtual void write_DIR1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BPC register
     * \param value New value of the rh850g4 BPC register
     */
    virtual void write_BPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BPAV register
     * \param value New value of the rh850g4 BPAV register
     */
    virtual void write_BPAV(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BPAM register
     * \param value New value of the rh850g4 BPAM register
     */
    virtual void write_BPAM(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DBCFG register
     * \param value New value of the rh850g4 DBCFG register
     */
    virtual void write_DBCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DBWR register
     * \param value New value of the rh850g4 DBWR register
     */
    virtual void write_DBWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 LSTEST0 register
     * \param value New value of the rh850g4 LSTEST0 register
     */
    virtual void write_LSTEST0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 LSTEST1 register
     * \param value New value of the rh850g4 LSTEST1 register
     */
    virtual void write_LSTEST1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 LSCFG register
     * \param value New value of the rh850g4 LSCFG register
     */
    virtual void write_LSCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICBKEY register
     * \param value New value of the rh850g4 ICBKEY register
     */
    virtual void write_ICBKEY(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IFCR register
     * \param value New value of the rh850g4 IFCR register
     */
    virtual void write_IFCR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IFCR1 register
     * \param value New value of the rh850g4 IFCR1 register
     */
    virtual void write_IFCR1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BRPCTRL0 register
     * \param value New value of the rh850g4 BRPCTRL0 register
     */
    virtual void write_BRPCTRL0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BRPCTRL1 register
     * \param value New value of the rh850g4 BRPCTRL1 register
     */
    virtual void write_BRPCTRL1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BRPCFG register
     * \param value New value of the rh850g4 BRPCFG register
     */
    virtual void write_BRPCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BRPACTRL register
     * \param value New value of the rh850g4 BRPACTRL register
     */
    virtual void write_BRPACTRL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 BRPDATA register
     * \param value New value of the rh850g4 BRPDATA register
     */
    virtual void write_BRPDATA(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DCBKEY register
     * \param value New value of the rh850g4 DCBKEY register
     */
    virtual void write_DCBKEY(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 LSUCR register
     * \param value New value of the rh850g4 LSUCR register
     */
    virtual void write_LSUCR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 LSULNK0 register
     * \param value New value of the rh850g4 LSULNK0 register
     */
    virtual void write_LSULNK0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 LSULNK1 register
     * \param value New value of the rh850g4 LSULNK1 register
     */
    virtual void write_LSULNK1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 L1RLCR register
     * \param value New value of the rh850g4 L1RLCR register
     */
    virtual void write_L1RLCR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 L1RLNK0 register
     * \param value New value of the rh850g4 L1RLNK0 register
     */
    virtual void write_L1RLNK0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 L1RLNK1 register
     * \param value New value of the rh850g4 L1RLNK1 register
     */
    virtual void write_L1RLNK1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 L1RCFG register
     * \param value New value of the rh850g4 L1RCFG register
     */
    virtual void write_L1RCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DECFG register
     * \param value New value of the rh850g4 DECFG register
     */
    virtual void write_DECFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DECTRL register
     * \param value New value of the rh850g4 DECTRL register
     */
    virtual void write_DECTRL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 DEVDS register
     * \param value New value of the rh850g4 DEVDS register
     */
    virtual void write_DEVDS(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBCR register
     * \param value New value of the rh850g4 RDBCR register
     */
    virtual void write_RDBCR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBACR register
     * \param value New value of the rh850g4 RDBACR register
     */
    virtual void write_RDBACR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBATAG register
     * \param value New value of the rh850g4 RDBATAG register
     */
    virtual void write_RDBATAG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBADAT0 register
     * \param value New value of the rh850g4 RDBADAT0 register
     */
    virtual void write_RDBADAT0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBADAT1 register
     * \param value New value of the rh850g4 RDBADAT1 register
     */
    virtual void write_RDBADAT1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBADAT2 register
     * \param value New value of the rh850g4 RDBADAT2 register
     */
    virtual void write_RDBADAT2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBADAT3 register
     * \param value New value of the rh850g4 RDBADAT3 register
     */
    virtual void write_RDBADAT3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RDBSTAT register
     * \param value New value of the rh850g4 RDBSTAT register
     */
    virtual void write_RDBSTAT(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 hv_func_support register
     * \param value New value of the rh850g4 hv_func_support register
     */
    virtual void write_hv_func_support(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850g4 swd0pcad_exec_count register
     * \param value New value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void write_swd0pcad_exec_count(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 swd0pcad_val register
     * \param value New value of the rh850g4 swd0pcad_val register
     */
    virtual void write_swd0pcad_val(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 mem_acc_type register
     * \param value New value of the rh850g4 mem_acc_type register
     */
    virtual void write_mem_acc_type(unsigned int value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateRegisterGroupWriteOperations
     * @{
     */
    /**
     * \brief Sets the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value New value of the specified rh850g4 GR group register
     */
    virtual void write_GR(rh850g4::GR_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value New value of the specified rh850g4 WR group register
     */
    virtual void write_WR(rh850g4::WR_index_t index, unsigned long long value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value New value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void write_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value New value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void write_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value New value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void write_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value New value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void write_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value New value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void write_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value New value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void write_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value New value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void write_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value New value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void write_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value New value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void write_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value New value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void write_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value New value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void write_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value New value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void write_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value) = 0;
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value New value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void write_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateIndividualRegisterIncrementOperations
     * @{
     */
    /**
     * \brief Increments the value of the rh850g4 translation time PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 translation time PC register
     */
    virtual void increment_trans_PC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r0 register
     */
    virtual void increment_r0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r1 register
     */
    virtual void increment_r1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r2 register
     */
    virtual void increment_r2(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r3 register
     */
    virtual void increment_r3(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r4 register
     */
    virtual void increment_r4(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r5 register
     */
    virtual void increment_r5(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r6 register
     */
    virtual void increment_r6(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r7 register
     */
    virtual void increment_r7(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r8 register
     */
    virtual void increment_r8(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r9 register
     */
    virtual void increment_r9(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r10 register
     */
    virtual void increment_r10(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r11 register
     */
    virtual void increment_r11(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r12 register
     */
    virtual void increment_r12(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r13 register
     */
    virtual void increment_r13(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r14 register
     */
    virtual void increment_r14(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r15 register
     */
    virtual void increment_r15(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r16 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r16 register
     */
    virtual void increment_r16(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r17 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r17 register
     */
    virtual void increment_r17(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r18 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r18 register
     */
    virtual void increment_r18(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r19 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r19 register
     */
    virtual void increment_r19(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r20 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r20 register
     */
    virtual void increment_r20(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r21 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r21 register
     */
    virtual void increment_r21(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r22 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r22 register
     */
    virtual void increment_r22(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r23 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r23 register
     */
    virtual void increment_r23(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r24 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r24 register
     */
    virtual void increment_r24(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r25 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r25 register
     */
    virtual void increment_r25(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r26 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r26 register
     */
    virtual void increment_r26(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r27 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r27 register
     */
    virtual void increment_r27(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r28 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r28 register
     */
    virtual void increment_r28(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r29 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r29 register
     */
    virtual void increment_r29(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r30 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r30 register
     */
    virtual void increment_r30(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 r31 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r31 register
     */
    virtual void increment_r31(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PC register
     */
    virtual void increment_PC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr0 register
     */
    virtual void increment_wr0(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr0_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr0_u register
     */
    virtual void increment_wr0_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr1 register
     */
    virtual void increment_wr1(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr1_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr1_u register
     */
    virtual void increment_wr1_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr2 register
     */
    virtual void increment_wr2(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr2_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr2_u register
     */
    virtual void increment_wr2_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr3 register
     */
    virtual void increment_wr3(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr3_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr3_u register
     */
    virtual void increment_wr3_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr4 register
     */
    virtual void increment_wr4(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr4_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr4_u register
     */
    virtual void increment_wr4_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr5 register
     */
    virtual void increment_wr5(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr5_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr5_u register
     */
    virtual void increment_wr5_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr6 register
     */
    virtual void increment_wr6(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr6_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr6_u register
     */
    virtual void increment_wr6_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr7 register
     */
    virtual void increment_wr7(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr7_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr7_u register
     */
    virtual void increment_wr7_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr8 register
     */
    virtual void increment_wr8(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr8_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr8_u register
     */
    virtual void increment_wr8_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr9 register
     */
    virtual void increment_wr9(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr9_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr9_u register
     */
    virtual void increment_wr9_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr10 register
     */
    virtual void increment_wr10(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr10_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr10_u register
     */
    virtual void increment_wr10_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr11 register
     */
    virtual void increment_wr11(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr11_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr11_u register
     */
    virtual void increment_wr11_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr12 register
     */
    virtual void increment_wr12(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr12_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr12_u register
     */
    virtual void increment_wr12_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr13 register
     */
    virtual void increment_wr13(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr13_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr13_u register
     */
    virtual void increment_wr13_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr14 register
     */
    virtual void increment_wr14(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr14_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr14_u register
     */
    virtual void increment_wr14_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr15 register
     */
    virtual void increment_wr15(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr15_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr15_u register
     */
    virtual void increment_wr15_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr16 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr16 register
     */
    virtual void increment_wr16(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr16_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr16_u register
     */
    virtual void increment_wr16_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr17 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr17 register
     */
    virtual void increment_wr17(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr17_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr17_u register
     */
    virtual void increment_wr17_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr18 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr18 register
     */
    virtual void increment_wr18(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr18_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr18_u register
     */
    virtual void increment_wr18_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr19 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr19 register
     */
    virtual void increment_wr19(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr19_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr19_u register
     */
    virtual void increment_wr19_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr20 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr20 register
     */
    virtual void increment_wr20(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr20_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr20_u register
     */
    virtual void increment_wr20_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr21 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr21 register
     */
    virtual void increment_wr21(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr21_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr21_u register
     */
    virtual void increment_wr21_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr22 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr22 register
     */
    virtual void increment_wr22(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr22_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr22_u register
     */
    virtual void increment_wr22_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr23 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr23 register
     */
    virtual void increment_wr23(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr23_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr23_u register
     */
    virtual void increment_wr23_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr24 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr24 register
     */
    virtual void increment_wr24(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr24_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr24_u register
     */
    virtual void increment_wr24_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr25 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr25 register
     */
    virtual void increment_wr25(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr25_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr25_u register
     */
    virtual void increment_wr25_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr26 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr26 register
     */
    virtual void increment_wr26(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr26_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr26_u register
     */
    virtual void increment_wr26_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr27 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr27 register
     */
    virtual void increment_wr27(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr27_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr27_u register
     */
    virtual void increment_wr27_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr28 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr28 register
     */
    virtual void increment_wr28(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr28_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr28_u register
     */
    virtual void increment_wr28_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr29 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr29 register
     */
    virtual void increment_wr29(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr29_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr29_u register
     */
    virtual void increment_wr29_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr30 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr30 register
     */
    virtual void increment_wr30(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr30_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr30_u register
     */
    virtual void increment_wr30_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr31 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr31 register
     */
    virtual void increment_wr31(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 wr31_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr31_u register
     */
    virtual void increment_wr31_u(unsigned long long value) = 0;
    /**
     * \brief Increments the value of the rh850g4 EIPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPC register
     */
    virtual void increment_EIPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 EIPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPSW register
     */
    virtual void increment_EIPSW(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPC register
     */
    virtual void increment_FEPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FEPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPSW register
     */
    virtual void increment_FEPSW(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PSW_EIMASK register field
     */
    virtual void increment_PSW_EIMASK(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_CC register field
     */
    virtual void increment_FPSR_CC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_RM register field
     */
    virtual void increment_FPSR_RM(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XC register field
     */
    virtual void increment_FPSR_XC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XE register field
     */
    virtual void increment_FPSR_XE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XP register field
     */
    virtual void increment_FPSR_XP(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPEPC register
     */
    virtual void increment_FPEPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPST_XC register field
     */
    virtual void increment_FPST_XC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPST_XP register field
     */
    virtual void increment_FPST_XP(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCC_CC register field
     */
    virtual void increment_FPCC_CC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCFG_RM register field
     */
    virtual void increment_FPCFG_RM(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCFG_XE register field
     */
    virtual void increment_FPCFG_XE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 EIIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIIC register
     */
    virtual void increment_EIIC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FEIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEIC register
     */
    virtual void increment_FEIC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PSWH_GPID register field
     */
    virtual void increment_PSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 CTPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTPC register
     */
    virtual void increment_CTPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 CTPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTPSW register
     */
    virtual void increment_CTPSW(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void increment_EIPSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void increment_FEPSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 CTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTBP register
     */
    virtual void increment_CTBP(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SNZCFG register
     */
    virtual void increment_SNZCFG(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 EIWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIWR register
     */
    virtual void increment_EIWR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FEWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEWR register
     */
    virtual void increment_FEWR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 SPID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SPID register
     */
    virtual void increment_SPID(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SPIDLIST register
     */
    virtual void increment_SPIDLIST(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBASE_RBASE register field
     */
    virtual void increment_RBASE_RBASE(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EBASE_EBASE register field
     */
    virtual void increment_EBASE_EBASE(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 INTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 INTBP register
     */
    virtual void increment_INTBP(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PID register
     */
    virtual void increment_PID(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SVLOCK register
     */
    virtual void increment_SVLOCK(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void increment_SCCFG_SIZE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 SCBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SCBP register
     */
    virtual void increment_SCBP(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 HVSB register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 HVSB register
     */
    virtual void increment_HVSB(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PEID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PEID register
     */
    virtual void increment_PEID(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BMID_BMID register field
     */
    virtual void increment_BMID_BMID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MEA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MEA register
     */
    virtual void increment_MEA(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MEI register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MEI register
     */
    virtual void increment_MEI(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBCR0_BE register field
     */
    virtual void increment_RBCR0_BE(unsigned short value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBCR1_NC register field
     */
    virtual void increment_RBCR1_NC(unsigned short value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBNR_BN register field
     */
    virtual void increment_RBNR_BN(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBIP_RBIP register field
     */
    virtual void increment_RBIP_RBIP(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ISPR_ISP register field
     */
    virtual void increment_ISPR_ISP(unsigned short value) = 0;
    /**
     * \brief Increments the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void increment_INTCFG_ULNR(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PLMR_PLM register field
     */
    virtual void increment_PLMR_PLM(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_RM register field
     */
    virtual void increment_FXSR_RM(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XC register field
     */
    virtual void increment_FXSR_XC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XE register field
     */
    virtual void increment_FXSR_XE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XP register field
     */
    virtual void increment_FXSR_XP(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXST_XC register field
     */
    virtual void increment_FXST_XC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXST_XP register field
     */
    virtual void increment_FXST_XP(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXCFG_RM register field
     */
    virtual void increment_FXCFG_RM(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXCFG_XE register field
     */
    virtual void increment_FXCFG_XE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC3 register field
     */
    virtual void increment_FXXC_XC3(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC2 register field
     */
    virtual void increment_FXXC_XC2(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC1 register field
     */
    virtual void increment_FXXC_XC1(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC0 register field
     */
    virtual void increment_FXXC_XC0(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP3 register field
     */
    virtual void increment_FXXP_XP3(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP2 register field
     */
    virtual void increment_FXXP_XP2(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP1 register field
     */
    virtual void increment_FXXP_XP1(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP0 register field
     */
    virtual void increment_FXXP_XP0(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIPC register
     */
    virtual void increment_GMEIPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIPSW register
     */
    virtual void increment_GMEIPSW(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEPC register
     */
    virtual void increment_GMFEPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEPSW register
     */
    virtual void increment_GMFEPSW(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void increment_GMPSW_EIMASK(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMMEA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMEA register
     */
    virtual void increment_GMMEA(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMMEI register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMEI register
     */
    virtual void increment_GMMEI(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIIC register
     */
    virtual void increment_GMEIIC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEIC register
     */
    virtual void increment_GMFEIC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMSPID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSPID register
     */
    virtual void increment_GMSPID(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSPIDLIST register
     */
    virtual void increment_GMSPIDLIST(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void increment_GMEBASE_EBASE(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMINTBP register
     */
    virtual void increment_GMINTBP(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void increment_GMINTCFG_ULNR(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void increment_GMPLMR_PLM(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSVLOCK register
     */
    virtual void increment_GMSVLOCK(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMMPM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMPM register
     */
    virtual void increment_GMMPM(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIWR register
     */
    virtual void increment_GMEIWR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEWR register
     */
    virtual void increment_GMFEWR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 GMPEID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPEID register
     */
    virtual void increment_GMPEID(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPM register
     */
    virtual void increment_MPM(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void increment_MPCFG_NMPUE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_HBE register field
     */
    virtual void increment_MPCFG_HBE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void increment_MPCFG_ARCH(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_NBK register field
     */
    virtual void increment_MPCFG_NBK(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MCA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCA register
     */
    virtual void increment_MCA(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MCS register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCS register
     */
    virtual void increment_MCS(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MCC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCC register
     */
    virtual void increment_MCC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCI_SPID register field
     */
    virtual void increment_MCI_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPIDX_IDX register field
     */
    virtual void increment_MPIDX_IDX(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPBK_BK register field
     */
    virtual void increment_MPBK_BK(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPLA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPLA register
     */
    virtual void increment_MPLA(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPUA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPUA register
     */
    virtual void increment_MPUA(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID0_SPID register field
     */
    virtual void increment_MPID0_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID1_SPID register field
     */
    virtual void increment_MPID1_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID2_SPID register field
     */
    virtual void increment_MPID2_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID3_SPID register field
     */
    virtual void increment_MPID3_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID4_SPID register field
     */
    virtual void increment_MPID4_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID5_SPID register field
     */
    virtual void increment_MPID5_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID6_SPID register field
     */
    virtual void increment_MPID6_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID7_SPID register field
     */
    virtual void increment_MPID7_SPID(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void increment_ICTAGL_LPN(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void increment_ICTAGL_LRU(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void increment_ICTAGH_DATAECC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void increment_ICTAGH_TAGECC(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICDATL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICDATL register
     */
    virtual void increment_ICDATL(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICDATH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICDATH register
     */
    virtual void increment_ICDATH(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void increment_ICCFG_ICHSIZE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void increment_ICCFG_ICHLINE(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void increment_ICCFG_ICHWAY(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void increment_ICERR_ICHEWY(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void increment_ICERR_ICHEIX(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCOUNTL register
     */
    virtual void increment_TSCOUNTL(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCOUNTH register
     */
    virtual void increment_TSCOUNTH(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCTRL register
     */
    virtual void increment_TSCTRL(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMUMCTRL register
     */
    virtual void increment_PMUMCTRL(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMGMCTRL register
     */
    virtual void increment_PMGMCTRL(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT0 register
     */
    virtual void increment_PMCOUNT0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL0 register
     */
    virtual void increment_PMCTRL0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT1 register
     */
    virtual void increment_PMCOUNT1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL1 register
     */
    virtual void increment_PMCTRL1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT2 register
     */
    virtual void increment_PMCOUNT2(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL2 register
     */
    virtual void increment_PMCTRL2(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT3 register
     */
    virtual void increment_PMCOUNT3(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL3 register
     */
    virtual void increment_PMCTRL3(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL4 register
     */
    virtual void increment_PMCTRL4(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL5 register
     */
    virtual void increment_PMCTRL5(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL6 register
     */
    virtual void increment_PMCTRL6(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL7 register
     */
    virtual void increment_PMCTRL7(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT4 register
     */
    virtual void increment_PMCOUNT4(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT5 register
     */
    virtual void increment_PMCOUNT5(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT6 register
     */
    virtual void increment_PMCOUNT6(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT7 register
     */
    virtual void increment_PMCOUNT7(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND0 register
     */
    virtual void increment_PMSUBCND0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND1 register
     */
    virtual void increment_PMSUBCND1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND2 register
     */
    virtual void increment_PMSUBCND2(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND3 register
     */
    virtual void increment_PMSUBCND3(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND4 register
     */
    virtual void increment_PMSUBCND4(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND5 register
     */
    virtual void increment_PMSUBCND5(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND6 register
     */
    virtual void increment_PMSUBCND6(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND7 register
     */
    virtual void increment_PMSUBCND7(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DBGEN register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBGEN register
     */
    virtual void increment_DBGEN(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPSWH register
     */
    virtual void increment_DBPSWH(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DBIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBIC register
     */
    virtual void increment_DBIC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DBPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPC register
     */
    virtual void increment_DBPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DBPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPSW register
     */
    virtual void increment_DBPSW(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DIR0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DIR0 register
     */
    virtual void increment_DIR0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DIR1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DIR1 register
     */
    virtual void increment_DIR1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPC register
     */
    virtual void increment_BPC(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BPAV register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPAV register
     */
    virtual void increment_BPAV(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BPAM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPAM register
     */
    virtual void increment_BPAM(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DBCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBCFG register
     */
    virtual void increment_DBCFG(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DBWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBWR register
     */
    virtual void increment_DBWR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSTEST0 register
     */
    virtual void increment_LSTEST0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSTEST1 register
     */
    virtual void increment_LSTEST1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 LSCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSCFG register
     */
    virtual void increment_LSCFG(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICBKEY register
     */
    virtual void increment_ICBKEY(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 IFCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 IFCR register
     */
    virtual void increment_IFCR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 IFCR1 register
     */
    virtual void increment_IFCR1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCTRL0 register
     */
    virtual void increment_BRPCTRL0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCTRL1 register
     */
    virtual void increment_BRPCTRL1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCFG register
     */
    virtual void increment_BRPCFG(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPACTRL register
     */
    virtual void increment_BRPACTRL(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPDATA register
     */
    virtual void increment_BRPDATA(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DCBKEY register
     */
    virtual void increment_DCBKEY(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 LSUCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSUCR register
     */
    virtual void increment_LSUCR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSULNK0 register
     */
    virtual void increment_LSULNK0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSULNK1 register
     */
    virtual void increment_LSULNK1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLCR register
     */
    virtual void increment_L1RLCR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLNK0 register
     */
    virtual void increment_L1RLNK0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLNK1 register
     */
    virtual void increment_L1RLNK1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RCFG register
     */
    virtual void increment_L1RCFG(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DECFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DECFG register
     */
    virtual void increment_DECFG(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DECTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DECTRL register
     */
    virtual void increment_DECTRL(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 DEVDS register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DEVDS register
     */
    virtual void increment_DEVDS(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBCR register
     */
    virtual void increment_RDBCR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBACR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBACR register
     */
    virtual void increment_RDBACR(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBATAG register
     */
    virtual void increment_RDBATAG(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT0 register
     */
    virtual void increment_RDBADAT0(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT1 register
     */
    virtual void increment_RDBADAT1(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT2 register
     */
    virtual void increment_RDBADAT2(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT3 register
     */
    virtual void increment_RDBADAT3(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBSTAT register
     */
    virtual void increment_RDBSTAT(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 hv_func_support register
     */
    virtual void increment_hv_func_support(unsigned char value) = 0;
    /**
     * \brief Increments the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void increment_swd0pcad_exec_count(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 swd0pcad_val register
     */
    virtual void increment_swd0pcad_val(unsigned int value) = 0;
    /**
     * \brief Increments the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 mem_acc_type register
     */
    virtual void increment_mem_acc_type(unsigned int value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateRegisterGroupIncrementOperations
     * @{
     */
    /**
     * \brief Increments the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Amount by which to increment the value of the specified rh850g4 GR group register
     */
    virtual void increment_GR(rh850g4::GR_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Amount by which to increment the value of the specified rh850g4 WR group register
     */
    virtual void increment_WR(rh850g4::WR_index_t index, unsigned long long value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void increment_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void increment_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void increment_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void increment_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void increment_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void increment_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void increment_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void increment_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void increment_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void increment_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void increment_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void increment_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value) = 0;
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void increment_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateIndividualRegisterDecrementOperations
     * @{
     */
    /**
     * \brief Decrements the value of the rh850g4 translation time PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 translation time PC register
     */
    virtual void decrement_trans_PC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r0 register
     */
    virtual void decrement_r0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r1 register
     */
    virtual void decrement_r1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r2 register
     */
    virtual void decrement_r2(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r3 register
     */
    virtual void decrement_r3(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r4 register
     */
    virtual void decrement_r4(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r5 register
     */
    virtual void decrement_r5(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r6 register
     */
    virtual void decrement_r6(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r7 register
     */
    virtual void decrement_r7(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r8 register
     */
    virtual void decrement_r8(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r9 register
     */
    virtual void decrement_r9(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r10 register
     */
    virtual void decrement_r10(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r11 register
     */
    virtual void decrement_r11(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r12 register
     */
    virtual void decrement_r12(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r13 register
     */
    virtual void decrement_r13(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r14 register
     */
    virtual void decrement_r14(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r15 register
     */
    virtual void decrement_r15(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r16 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r16 register
     */
    virtual void decrement_r16(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r17 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r17 register
     */
    virtual void decrement_r17(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r18 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r18 register
     */
    virtual void decrement_r18(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r19 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r19 register
     */
    virtual void decrement_r19(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r20 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r20 register
     */
    virtual void decrement_r20(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r21 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r21 register
     */
    virtual void decrement_r21(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r22 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r22 register
     */
    virtual void decrement_r22(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r23 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r23 register
     */
    virtual void decrement_r23(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r24 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r24 register
     */
    virtual void decrement_r24(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r25 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r25 register
     */
    virtual void decrement_r25(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r26 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r26 register
     */
    virtual void decrement_r26(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r27 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r27 register
     */
    virtual void decrement_r27(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r28 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r28 register
     */
    virtual void decrement_r28(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r29 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r29 register
     */
    virtual void decrement_r29(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r30 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r30 register
     */
    virtual void decrement_r30(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 r31 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r31 register
     */
    virtual void decrement_r31(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PC register
     */
    virtual void decrement_PC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr0 register
     */
    virtual void decrement_wr0(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr0_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr0_u register
     */
    virtual void decrement_wr0_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr1 register
     */
    virtual void decrement_wr1(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr1_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr1_u register
     */
    virtual void decrement_wr1_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr2 register
     */
    virtual void decrement_wr2(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr2_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr2_u register
     */
    virtual void decrement_wr2_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr3 register
     */
    virtual void decrement_wr3(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr3_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr3_u register
     */
    virtual void decrement_wr3_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr4 register
     */
    virtual void decrement_wr4(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr4_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr4_u register
     */
    virtual void decrement_wr4_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr5 register
     */
    virtual void decrement_wr5(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr5_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr5_u register
     */
    virtual void decrement_wr5_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr6 register
     */
    virtual void decrement_wr6(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr6_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr6_u register
     */
    virtual void decrement_wr6_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr7 register
     */
    virtual void decrement_wr7(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr7_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr7_u register
     */
    virtual void decrement_wr7_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr8 register
     */
    virtual void decrement_wr8(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr8_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr8_u register
     */
    virtual void decrement_wr8_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr9 register
     */
    virtual void decrement_wr9(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr9_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr9_u register
     */
    virtual void decrement_wr9_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr10 register
     */
    virtual void decrement_wr10(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr10_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr10_u register
     */
    virtual void decrement_wr10_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr11 register
     */
    virtual void decrement_wr11(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr11_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr11_u register
     */
    virtual void decrement_wr11_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr12 register
     */
    virtual void decrement_wr12(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr12_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr12_u register
     */
    virtual void decrement_wr12_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr13 register
     */
    virtual void decrement_wr13(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr13_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr13_u register
     */
    virtual void decrement_wr13_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr14 register
     */
    virtual void decrement_wr14(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr14_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr14_u register
     */
    virtual void decrement_wr14_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr15 register
     */
    virtual void decrement_wr15(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr15_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr15_u register
     */
    virtual void decrement_wr15_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr16 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr16 register
     */
    virtual void decrement_wr16(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr16_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr16_u register
     */
    virtual void decrement_wr16_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr17 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr17 register
     */
    virtual void decrement_wr17(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr17_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr17_u register
     */
    virtual void decrement_wr17_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr18 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr18 register
     */
    virtual void decrement_wr18(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr18_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr18_u register
     */
    virtual void decrement_wr18_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr19 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr19 register
     */
    virtual void decrement_wr19(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr19_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr19_u register
     */
    virtual void decrement_wr19_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr20 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr20 register
     */
    virtual void decrement_wr20(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr20_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr20_u register
     */
    virtual void decrement_wr20_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr21 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr21 register
     */
    virtual void decrement_wr21(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr21_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr21_u register
     */
    virtual void decrement_wr21_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr22 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr22 register
     */
    virtual void decrement_wr22(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr22_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr22_u register
     */
    virtual void decrement_wr22_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr23 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr23 register
     */
    virtual void decrement_wr23(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr23_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr23_u register
     */
    virtual void decrement_wr23_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr24 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr24 register
     */
    virtual void decrement_wr24(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr24_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr24_u register
     */
    virtual void decrement_wr24_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr25 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr25 register
     */
    virtual void decrement_wr25(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr25_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr25_u register
     */
    virtual void decrement_wr25_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr26 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr26 register
     */
    virtual void decrement_wr26(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr26_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr26_u register
     */
    virtual void decrement_wr26_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr27 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr27 register
     */
    virtual void decrement_wr27(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr27_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr27_u register
     */
    virtual void decrement_wr27_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr28 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr28 register
     */
    virtual void decrement_wr28(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr28_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr28_u register
     */
    virtual void decrement_wr28_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr29 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr29 register
     */
    virtual void decrement_wr29(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr29_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr29_u register
     */
    virtual void decrement_wr29_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr30 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr30 register
     */
    virtual void decrement_wr30(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr30_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr30_u register
     */
    virtual void decrement_wr30_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr31 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr31 register
     */
    virtual void decrement_wr31(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 wr31_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr31_u register
     */
    virtual void decrement_wr31_u(unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 EIPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPC register
     */
    virtual void decrement_EIPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 EIPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPSW register
     */
    virtual void decrement_EIPSW(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPC register
     */
    virtual void decrement_FEPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FEPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPSW register
     */
    virtual void decrement_FEPSW(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PSW_EIMASK register field
     */
    virtual void decrement_PSW_EIMASK(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_CC register field
     */
    virtual void decrement_FPSR_CC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_RM register field
     */
    virtual void decrement_FPSR_RM(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XC register field
     */
    virtual void decrement_FPSR_XC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XE register field
     */
    virtual void decrement_FPSR_XE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XP register field
     */
    virtual void decrement_FPSR_XP(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPEPC register
     */
    virtual void decrement_FPEPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPST_XC register field
     */
    virtual void decrement_FPST_XC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPST_XP register field
     */
    virtual void decrement_FPST_XP(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCC_CC register field
     */
    virtual void decrement_FPCC_CC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCFG_RM register field
     */
    virtual void decrement_FPCFG_RM(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCFG_XE register field
     */
    virtual void decrement_FPCFG_XE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 EIIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIIC register
     */
    virtual void decrement_EIIC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FEIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEIC register
     */
    virtual void decrement_FEIC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PSWH_GPID register field
     */
    virtual void decrement_PSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 CTPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTPC register
     */
    virtual void decrement_CTPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 CTPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTPSW register
     */
    virtual void decrement_CTPSW(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void decrement_EIPSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void decrement_FEPSWH_GPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 CTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTBP register
     */
    virtual void decrement_CTBP(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SNZCFG register
     */
    virtual void decrement_SNZCFG(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 EIWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIWR register
     */
    virtual void decrement_EIWR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FEWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEWR register
     */
    virtual void decrement_FEWR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 SPID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SPID register
     */
    virtual void decrement_SPID(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SPIDLIST register
     */
    virtual void decrement_SPIDLIST(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBASE_RBASE register field
     */
    virtual void decrement_RBASE_RBASE(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EBASE_EBASE register field
     */
    virtual void decrement_EBASE_EBASE(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 INTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 INTBP register
     */
    virtual void decrement_INTBP(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PID register
     */
    virtual void decrement_PID(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SVLOCK register
     */
    virtual void decrement_SVLOCK(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void decrement_SCCFG_SIZE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 SCBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SCBP register
     */
    virtual void decrement_SCBP(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 HVSB register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 HVSB register
     */
    virtual void decrement_HVSB(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PEID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PEID register
     */
    virtual void decrement_PEID(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BMID_BMID register field
     */
    virtual void decrement_BMID_BMID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MEA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MEA register
     */
    virtual void decrement_MEA(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MEI register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MEI register
     */
    virtual void decrement_MEI(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBCR0_BE register field
     */
    virtual void decrement_RBCR0_BE(unsigned short value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBCR1_NC register field
     */
    virtual void decrement_RBCR1_NC(unsigned short value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBNR_BN register field
     */
    virtual void decrement_RBNR_BN(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBIP_RBIP register field
     */
    virtual void decrement_RBIP_RBIP(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ISPR_ISP register field
     */
    virtual void decrement_ISPR_ISP(unsigned short value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void decrement_INTCFG_ULNR(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PLMR_PLM register field
     */
    virtual void decrement_PLMR_PLM(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_RM register field
     */
    virtual void decrement_FXSR_RM(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XC register field
     */
    virtual void decrement_FXSR_XC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XE register field
     */
    virtual void decrement_FXSR_XE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XP register field
     */
    virtual void decrement_FXSR_XP(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXST_XC register field
     */
    virtual void decrement_FXST_XC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXST_XP register field
     */
    virtual void decrement_FXST_XP(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXCFG_RM register field
     */
    virtual void decrement_FXCFG_RM(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXCFG_XE register field
     */
    virtual void decrement_FXCFG_XE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC3 register field
     */
    virtual void decrement_FXXC_XC3(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC2 register field
     */
    virtual void decrement_FXXC_XC2(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC1 register field
     */
    virtual void decrement_FXXC_XC1(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC0 register field
     */
    virtual void decrement_FXXC_XC0(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP3 register field
     */
    virtual void decrement_FXXP_XP3(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP2 register field
     */
    virtual void decrement_FXXP_XP2(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP1 register field
     */
    virtual void decrement_FXXP_XP1(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP0 register field
     */
    virtual void decrement_FXXP_XP0(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIPC register
     */
    virtual void decrement_GMEIPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIPSW register
     */
    virtual void decrement_GMEIPSW(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEPC register
     */
    virtual void decrement_GMFEPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEPSW register
     */
    virtual void decrement_GMFEPSW(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void decrement_GMPSW_EIMASK(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMMEA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMEA register
     */
    virtual void decrement_GMMEA(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMMEI register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMEI register
     */
    virtual void decrement_GMMEI(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIIC register
     */
    virtual void decrement_GMEIIC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEIC register
     */
    virtual void decrement_GMFEIC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMSPID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSPID register
     */
    virtual void decrement_GMSPID(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSPIDLIST register
     */
    virtual void decrement_GMSPIDLIST(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void decrement_GMEBASE_EBASE(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMINTBP register
     */
    virtual void decrement_GMINTBP(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void decrement_GMINTCFG_ULNR(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void decrement_GMPLMR_PLM(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSVLOCK register
     */
    virtual void decrement_GMSVLOCK(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMMPM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMPM register
     */
    virtual void decrement_GMMPM(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIWR register
     */
    virtual void decrement_GMEIWR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEWR register
     */
    virtual void decrement_GMFEWR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 GMPEID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPEID register
     */
    virtual void decrement_GMPEID(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPM register
     */
    virtual void decrement_MPM(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void decrement_MPCFG_NMPUE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_HBE register field
     */
    virtual void decrement_MPCFG_HBE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void decrement_MPCFG_ARCH(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_NBK register field
     */
    virtual void decrement_MPCFG_NBK(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MCA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCA register
     */
    virtual void decrement_MCA(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MCS register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCS register
     */
    virtual void decrement_MCS(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MCC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCC register
     */
    virtual void decrement_MCC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCI_SPID register field
     */
    virtual void decrement_MCI_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPIDX_IDX register field
     */
    virtual void decrement_MPIDX_IDX(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPBK_BK register field
     */
    virtual void decrement_MPBK_BK(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPLA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPLA register
     */
    virtual void decrement_MPLA(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPUA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPUA register
     */
    virtual void decrement_MPUA(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID0_SPID register field
     */
    virtual void decrement_MPID0_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID1_SPID register field
     */
    virtual void decrement_MPID1_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID2_SPID register field
     */
    virtual void decrement_MPID2_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID3_SPID register field
     */
    virtual void decrement_MPID3_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID4_SPID register field
     */
    virtual void decrement_MPID4_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID5_SPID register field
     */
    virtual void decrement_MPID5_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID6_SPID register field
     */
    virtual void decrement_MPID6_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID7_SPID register field
     */
    virtual void decrement_MPID7_SPID(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void decrement_ICTAGL_LPN(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void decrement_ICTAGL_LRU(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void decrement_ICTAGH_DATAECC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void decrement_ICTAGH_TAGECC(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICDATL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICDATL register
     */
    virtual void decrement_ICDATL(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICDATH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICDATH register
     */
    virtual void decrement_ICDATH(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void decrement_ICCFG_ICHSIZE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void decrement_ICCFG_ICHLINE(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void decrement_ICCFG_ICHWAY(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void decrement_ICERR_ICHEWY(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void decrement_ICERR_ICHEIX(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCOUNTL register
     */
    virtual void decrement_TSCOUNTL(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCOUNTH register
     */
    virtual void decrement_TSCOUNTH(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCTRL register
     */
    virtual void decrement_TSCTRL(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMUMCTRL register
     */
    virtual void decrement_PMUMCTRL(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMGMCTRL register
     */
    virtual void decrement_PMGMCTRL(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT0 register
     */
    virtual void decrement_PMCOUNT0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL0 register
     */
    virtual void decrement_PMCTRL0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT1 register
     */
    virtual void decrement_PMCOUNT1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL1 register
     */
    virtual void decrement_PMCTRL1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT2 register
     */
    virtual void decrement_PMCOUNT2(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL2 register
     */
    virtual void decrement_PMCTRL2(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT3 register
     */
    virtual void decrement_PMCOUNT3(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL3 register
     */
    virtual void decrement_PMCTRL3(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL4 register
     */
    virtual void decrement_PMCTRL4(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL5 register
     */
    virtual void decrement_PMCTRL5(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL6 register
     */
    virtual void decrement_PMCTRL6(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL7 register
     */
    virtual void decrement_PMCTRL7(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT4 register
     */
    virtual void decrement_PMCOUNT4(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT5 register
     */
    virtual void decrement_PMCOUNT5(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT6 register
     */
    virtual void decrement_PMCOUNT6(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT7 register
     */
    virtual void decrement_PMCOUNT7(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND0 register
     */
    virtual void decrement_PMSUBCND0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND1 register
     */
    virtual void decrement_PMSUBCND1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND2 register
     */
    virtual void decrement_PMSUBCND2(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND3 register
     */
    virtual void decrement_PMSUBCND3(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND4 register
     */
    virtual void decrement_PMSUBCND4(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND5 register
     */
    virtual void decrement_PMSUBCND5(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND6 register
     */
    virtual void decrement_PMSUBCND6(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND7 register
     */
    virtual void decrement_PMSUBCND7(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DBGEN register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBGEN register
     */
    virtual void decrement_DBGEN(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPSWH register
     */
    virtual void decrement_DBPSWH(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DBIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBIC register
     */
    virtual void decrement_DBIC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DBPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPC register
     */
    virtual void decrement_DBPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DBPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPSW register
     */
    virtual void decrement_DBPSW(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DIR0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DIR0 register
     */
    virtual void decrement_DIR0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DIR1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DIR1 register
     */
    virtual void decrement_DIR1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPC register
     */
    virtual void decrement_BPC(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BPAV register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPAV register
     */
    virtual void decrement_BPAV(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BPAM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPAM register
     */
    virtual void decrement_BPAM(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DBCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBCFG register
     */
    virtual void decrement_DBCFG(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DBWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBWR register
     */
    virtual void decrement_DBWR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSTEST0 register
     */
    virtual void decrement_LSTEST0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSTEST1 register
     */
    virtual void decrement_LSTEST1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 LSCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSCFG register
     */
    virtual void decrement_LSCFG(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICBKEY register
     */
    virtual void decrement_ICBKEY(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 IFCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 IFCR register
     */
    virtual void decrement_IFCR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 IFCR1 register
     */
    virtual void decrement_IFCR1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCTRL0 register
     */
    virtual void decrement_BRPCTRL0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCTRL1 register
     */
    virtual void decrement_BRPCTRL1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCFG register
     */
    virtual void decrement_BRPCFG(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPACTRL register
     */
    virtual void decrement_BRPACTRL(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPDATA register
     */
    virtual void decrement_BRPDATA(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DCBKEY register
     */
    virtual void decrement_DCBKEY(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 LSUCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSUCR register
     */
    virtual void decrement_LSUCR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSULNK0 register
     */
    virtual void decrement_LSULNK0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSULNK1 register
     */
    virtual void decrement_LSULNK1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLCR register
     */
    virtual void decrement_L1RLCR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLNK0 register
     */
    virtual void decrement_L1RLNK0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLNK1 register
     */
    virtual void decrement_L1RLNK1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RCFG register
     */
    virtual void decrement_L1RCFG(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DECFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DECFG register
     */
    virtual void decrement_DECFG(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DECTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DECTRL register
     */
    virtual void decrement_DECTRL(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 DEVDS register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DEVDS register
     */
    virtual void decrement_DEVDS(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBCR register
     */
    virtual void decrement_RDBCR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBACR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBACR register
     */
    virtual void decrement_RDBACR(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBATAG register
     */
    virtual void decrement_RDBATAG(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT0 register
     */
    virtual void decrement_RDBADAT0(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT1 register
     */
    virtual void decrement_RDBADAT1(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT2 register
     */
    virtual void decrement_RDBADAT2(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT3 register
     */
    virtual void decrement_RDBADAT3(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBSTAT register
     */
    virtual void decrement_RDBSTAT(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 hv_func_support register
     */
    virtual void decrement_hv_func_support(unsigned char value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void decrement_swd0pcad_exec_count(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 swd0pcad_val register
     */
    virtual void decrement_swd0pcad_val(unsigned int value) = 0;
    /**
     * \brief Decrements the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 mem_acc_type register
     */
    virtual void decrement_mem_acc_type(unsigned int value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup ImmediateRegisterGroupDecrementOperations
     * @{
     */
    /**
     * \brief Decrements the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Amount by which to decrement the value of the specified rh850g4 GR group register
     */
    virtual void decrement_GR(rh850g4::GR_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Amount by which to decrement the value of the specified rh850g4 WR group register
     */
    virtual void decrement_WR(rh850g4::WR_index_t index, unsigned long long value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void decrement_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void decrement_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void decrement_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void decrement_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void decrement_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void decrement_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void decrement_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void decrement_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void decrement_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void decrement_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void decrement_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void decrement_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value) = 0;
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void decrement_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeIndividualRegisterReadOperations
     * @{
     */
    /**
     * \brief Generates code to query the value of the rh850g4 r0 register
     * \return Object representing the value of the rh850g4 r0 register
     */
    virtual fastiss::i32_t *gen_read_r0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r1 register
     * \return Object representing the value of the rh850g4 r1 register
     */
    virtual fastiss::i32_t *gen_read_r1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r2 register
     * \return Object representing the value of the rh850g4 r2 register
     */
    virtual fastiss::i32_t *gen_read_r2(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r3 register
     * \return Object representing the value of the rh850g4 r3 register
     */
    virtual fastiss::i32_t *gen_read_r3(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r4 register
     * \return Object representing the value of the rh850g4 r4 register
     */
    virtual fastiss::i32_t *gen_read_r4(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r5 register
     * \return Object representing the value of the rh850g4 r5 register
     */
    virtual fastiss::i32_t *gen_read_r5(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r6 register
     * \return Object representing the value of the rh850g4 r6 register
     */
    virtual fastiss::i32_t *gen_read_r6(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r7 register
     * \return Object representing the value of the rh850g4 r7 register
     */
    virtual fastiss::i32_t *gen_read_r7(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r8 register
     * \return Object representing the value of the rh850g4 r8 register
     */
    virtual fastiss::i32_t *gen_read_r8(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r9 register
     * \return Object representing the value of the rh850g4 r9 register
     */
    virtual fastiss::i32_t *gen_read_r9(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r10 register
     * \return Object representing the value of the rh850g4 r10 register
     */
    virtual fastiss::i32_t *gen_read_r10(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r11 register
     * \return Object representing the value of the rh850g4 r11 register
     */
    virtual fastiss::i32_t *gen_read_r11(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r12 register
     * \return Object representing the value of the rh850g4 r12 register
     */
    virtual fastiss::i32_t *gen_read_r12(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r13 register
     * \return Object representing the value of the rh850g4 r13 register
     */
    virtual fastiss::i32_t *gen_read_r13(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r14 register
     * \return Object representing the value of the rh850g4 r14 register
     */
    virtual fastiss::i32_t *gen_read_r14(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r15 register
     * \return Object representing the value of the rh850g4 r15 register
     */
    virtual fastiss::i32_t *gen_read_r15(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r16 register
     * \return Object representing the value of the rh850g4 r16 register
     */
    virtual fastiss::i32_t *gen_read_r16(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r17 register
     * \return Object representing the value of the rh850g4 r17 register
     */
    virtual fastiss::i32_t *gen_read_r17(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r18 register
     * \return Object representing the value of the rh850g4 r18 register
     */
    virtual fastiss::i32_t *gen_read_r18(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r19 register
     * \return Object representing the value of the rh850g4 r19 register
     */
    virtual fastiss::i32_t *gen_read_r19(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r20 register
     * \return Object representing the value of the rh850g4 r20 register
     */
    virtual fastiss::i32_t *gen_read_r20(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r21 register
     * \return Object representing the value of the rh850g4 r21 register
     */
    virtual fastiss::i32_t *gen_read_r21(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r22 register
     * \return Object representing the value of the rh850g4 r22 register
     */
    virtual fastiss::i32_t *gen_read_r22(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r23 register
     * \return Object representing the value of the rh850g4 r23 register
     */
    virtual fastiss::i32_t *gen_read_r23(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r24 register
     * \return Object representing the value of the rh850g4 r24 register
     */
    virtual fastiss::i32_t *gen_read_r24(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r25 register
     * \return Object representing the value of the rh850g4 r25 register
     */
    virtual fastiss::i32_t *gen_read_r25(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r26 register
     * \return Object representing the value of the rh850g4 r26 register
     */
    virtual fastiss::i32_t *gen_read_r26(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r27 register
     * \return Object representing the value of the rh850g4 r27 register
     */
    virtual fastiss::i32_t *gen_read_r27(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r28 register
     * \return Object representing the value of the rh850g4 r28 register
     */
    virtual fastiss::i32_t *gen_read_r28(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r29 register
     * \return Object representing the value of the rh850g4 r29 register
     */
    virtual fastiss::i32_t *gen_read_r29(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r30 register
     * \return Object representing the value of the rh850g4 r30 register
     */
    virtual fastiss::i32_t *gen_read_r30(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 r31 register
     * \return Object representing the value of the rh850g4 r31 register
     */
    virtual fastiss::i32_t *gen_read_r31(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PC register
     * \return Object representing the value of the rh850g4 PC register
     */
    virtual fastiss::i32_t *gen_read_PC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr0 register
     * \return Object representing the value of the rh850g4 wr0 register
     */
    virtual fastiss::i64_t *gen_read_wr0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr0_u register
     * \return Object representing the value of the rh850g4 wr0_u register
     */
    virtual fastiss::i64_t *gen_read_wr0_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr1 register
     * \return Object representing the value of the rh850g4 wr1 register
     */
    virtual fastiss::i64_t *gen_read_wr1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr1_u register
     * \return Object representing the value of the rh850g4 wr1_u register
     */
    virtual fastiss::i64_t *gen_read_wr1_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr2 register
     * \return Object representing the value of the rh850g4 wr2 register
     */
    virtual fastiss::i64_t *gen_read_wr2(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr2_u register
     * \return Object representing the value of the rh850g4 wr2_u register
     */
    virtual fastiss::i64_t *gen_read_wr2_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr3 register
     * \return Object representing the value of the rh850g4 wr3 register
     */
    virtual fastiss::i64_t *gen_read_wr3(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr3_u register
     * \return Object representing the value of the rh850g4 wr3_u register
     */
    virtual fastiss::i64_t *gen_read_wr3_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr4 register
     * \return Object representing the value of the rh850g4 wr4 register
     */
    virtual fastiss::i64_t *gen_read_wr4(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr4_u register
     * \return Object representing the value of the rh850g4 wr4_u register
     */
    virtual fastiss::i64_t *gen_read_wr4_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr5 register
     * \return Object representing the value of the rh850g4 wr5 register
     */
    virtual fastiss::i64_t *gen_read_wr5(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr5_u register
     * \return Object representing the value of the rh850g4 wr5_u register
     */
    virtual fastiss::i64_t *gen_read_wr5_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr6 register
     * \return Object representing the value of the rh850g4 wr6 register
     */
    virtual fastiss::i64_t *gen_read_wr6(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr6_u register
     * \return Object representing the value of the rh850g4 wr6_u register
     */
    virtual fastiss::i64_t *gen_read_wr6_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr7 register
     * \return Object representing the value of the rh850g4 wr7 register
     */
    virtual fastiss::i64_t *gen_read_wr7(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr7_u register
     * \return Object representing the value of the rh850g4 wr7_u register
     */
    virtual fastiss::i64_t *gen_read_wr7_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr8 register
     * \return Object representing the value of the rh850g4 wr8 register
     */
    virtual fastiss::i64_t *gen_read_wr8(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr8_u register
     * \return Object representing the value of the rh850g4 wr8_u register
     */
    virtual fastiss::i64_t *gen_read_wr8_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr9 register
     * \return Object representing the value of the rh850g4 wr9 register
     */
    virtual fastiss::i64_t *gen_read_wr9(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr9_u register
     * \return Object representing the value of the rh850g4 wr9_u register
     */
    virtual fastiss::i64_t *gen_read_wr9_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr10 register
     * \return Object representing the value of the rh850g4 wr10 register
     */
    virtual fastiss::i64_t *gen_read_wr10(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr10_u register
     * \return Object representing the value of the rh850g4 wr10_u register
     */
    virtual fastiss::i64_t *gen_read_wr10_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr11 register
     * \return Object representing the value of the rh850g4 wr11 register
     */
    virtual fastiss::i64_t *gen_read_wr11(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr11_u register
     * \return Object representing the value of the rh850g4 wr11_u register
     */
    virtual fastiss::i64_t *gen_read_wr11_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr12 register
     * \return Object representing the value of the rh850g4 wr12 register
     */
    virtual fastiss::i64_t *gen_read_wr12(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr12_u register
     * \return Object representing the value of the rh850g4 wr12_u register
     */
    virtual fastiss::i64_t *gen_read_wr12_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr13 register
     * \return Object representing the value of the rh850g4 wr13 register
     */
    virtual fastiss::i64_t *gen_read_wr13(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr13_u register
     * \return Object representing the value of the rh850g4 wr13_u register
     */
    virtual fastiss::i64_t *gen_read_wr13_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr14 register
     * \return Object representing the value of the rh850g4 wr14 register
     */
    virtual fastiss::i64_t *gen_read_wr14(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr14_u register
     * \return Object representing the value of the rh850g4 wr14_u register
     */
    virtual fastiss::i64_t *gen_read_wr14_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr15 register
     * \return Object representing the value of the rh850g4 wr15 register
     */
    virtual fastiss::i64_t *gen_read_wr15(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr15_u register
     * \return Object representing the value of the rh850g4 wr15_u register
     */
    virtual fastiss::i64_t *gen_read_wr15_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr16 register
     * \return Object representing the value of the rh850g4 wr16 register
     */
    virtual fastiss::i64_t *gen_read_wr16(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr16_u register
     * \return Object representing the value of the rh850g4 wr16_u register
     */
    virtual fastiss::i64_t *gen_read_wr16_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr17 register
     * \return Object representing the value of the rh850g4 wr17 register
     */
    virtual fastiss::i64_t *gen_read_wr17(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr17_u register
     * \return Object representing the value of the rh850g4 wr17_u register
     */
    virtual fastiss::i64_t *gen_read_wr17_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr18 register
     * \return Object representing the value of the rh850g4 wr18 register
     */
    virtual fastiss::i64_t *gen_read_wr18(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr18_u register
     * \return Object representing the value of the rh850g4 wr18_u register
     */
    virtual fastiss::i64_t *gen_read_wr18_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr19 register
     * \return Object representing the value of the rh850g4 wr19 register
     */
    virtual fastiss::i64_t *gen_read_wr19(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr19_u register
     * \return Object representing the value of the rh850g4 wr19_u register
     */
    virtual fastiss::i64_t *gen_read_wr19_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr20 register
     * \return Object representing the value of the rh850g4 wr20 register
     */
    virtual fastiss::i64_t *gen_read_wr20(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr20_u register
     * \return Object representing the value of the rh850g4 wr20_u register
     */
    virtual fastiss::i64_t *gen_read_wr20_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr21 register
     * \return Object representing the value of the rh850g4 wr21 register
     */
    virtual fastiss::i64_t *gen_read_wr21(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr21_u register
     * \return Object representing the value of the rh850g4 wr21_u register
     */
    virtual fastiss::i64_t *gen_read_wr21_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr22 register
     * \return Object representing the value of the rh850g4 wr22 register
     */
    virtual fastiss::i64_t *gen_read_wr22(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr22_u register
     * \return Object representing the value of the rh850g4 wr22_u register
     */
    virtual fastiss::i64_t *gen_read_wr22_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr23 register
     * \return Object representing the value of the rh850g4 wr23 register
     */
    virtual fastiss::i64_t *gen_read_wr23(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr23_u register
     * \return Object representing the value of the rh850g4 wr23_u register
     */
    virtual fastiss::i64_t *gen_read_wr23_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr24 register
     * \return Object representing the value of the rh850g4 wr24 register
     */
    virtual fastiss::i64_t *gen_read_wr24(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr24_u register
     * \return Object representing the value of the rh850g4 wr24_u register
     */
    virtual fastiss::i64_t *gen_read_wr24_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr25 register
     * \return Object representing the value of the rh850g4 wr25 register
     */
    virtual fastiss::i64_t *gen_read_wr25(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr25_u register
     * \return Object representing the value of the rh850g4 wr25_u register
     */
    virtual fastiss::i64_t *gen_read_wr25_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr26 register
     * \return Object representing the value of the rh850g4 wr26 register
     */
    virtual fastiss::i64_t *gen_read_wr26(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr26_u register
     * \return Object representing the value of the rh850g4 wr26_u register
     */
    virtual fastiss::i64_t *gen_read_wr26_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr27 register
     * \return Object representing the value of the rh850g4 wr27 register
     */
    virtual fastiss::i64_t *gen_read_wr27(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr27_u register
     * \return Object representing the value of the rh850g4 wr27_u register
     */
    virtual fastiss::i64_t *gen_read_wr27_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr28 register
     * \return Object representing the value of the rh850g4 wr28 register
     */
    virtual fastiss::i64_t *gen_read_wr28(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr28_u register
     * \return Object representing the value of the rh850g4 wr28_u register
     */
    virtual fastiss::i64_t *gen_read_wr28_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr29 register
     * \return Object representing the value of the rh850g4 wr29 register
     */
    virtual fastiss::i64_t *gen_read_wr29(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr29_u register
     * \return Object representing the value of the rh850g4 wr29_u register
     */
    virtual fastiss::i64_t *gen_read_wr29_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr30 register
     * \return Object representing the value of the rh850g4 wr30 register
     */
    virtual fastiss::i64_t *gen_read_wr30(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr30_u register
     * \return Object representing the value of the rh850g4 wr30_u register
     */
    virtual fastiss::i64_t *gen_read_wr30_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr31 register
     * \return Object representing the value of the rh850g4 wr31 register
     */
    virtual fastiss::i64_t *gen_read_wr31(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 wr31_u register
     * \return Object representing the value of the rh850g4 wr31_u register
     */
    virtual fastiss::i64_t *gen_read_wr31_u(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 EIPC register
     * \return Object representing the value of the rh850g4 EIPC register
     */
    virtual fastiss::i32_t *gen_read_EIPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 EIPSW register
     * \return Object representing the value of the rh850g4 EIPSW register
     */
    virtual fastiss::i32_t *gen_read_EIPSW(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FEPC register
     * \return Object representing the value of the rh850g4 FEPC register
     */
    virtual fastiss::i32_t *gen_read_FEPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FEPSW register
     * \return Object representing the value of the rh850g4 FEPSW register
     */
    virtual fastiss::i32_t *gen_read_FEPSW(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PSW register
     * \return Object representing the value of the rh850g4 PSW register
     */
    virtual fastiss::i32_t *gen_read_PSW(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_Z register field
    * \return Object representing the value of the rh850g4 PSW_Z register field
    */
    virtual fastiss::i1_t *gen_read_PSW_Z(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_S register field
    * \return Object representing the value of the rh850g4 PSW_S register field
    */
    virtual fastiss::i1_t *gen_read_PSW_S(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_OV register field
    * \return Object representing the value of the rh850g4 PSW_OV register field
    */
    virtual fastiss::i1_t *gen_read_PSW_OV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CY register field
    * \return Object representing the value of the rh850g4 PSW_CY register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CY(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_SAT register field
    * \return Object representing the value of the rh850g4 PSW_SAT register field
    */
    virtual fastiss::i1_t *gen_read_PSW_SAT(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_ID register field
    * \return Object representing the value of the rh850g4 PSW_ID register field
    */
    virtual fastiss::i1_t *gen_read_PSW_ID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EP register field
    * \return Object representing the value of the rh850g4 PSW_EP register field
    */
    virtual fastiss::i1_t *gen_read_PSW_EP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_NP register field
    * \return Object representing the value of the rh850g4 PSW_NP register field
    */
    virtual fastiss::i1_t *gen_read_PSW_NP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EBV register field
    * \return Object representing the value of the rh850g4 PSW_EBV register field
    */
    virtual fastiss::i1_t *gen_read_PSW_EBV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU0 register field
    * \return Object representing the value of the rh850g4 PSW_CU0 register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CU0(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU1 register field
    * \return Object representing the value of the rh850g4 PSW_CU1 register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CU1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU2 register field
    * \return Object representing the value of the rh850g4 PSW_CU2 register field
    */
    virtual fastiss::i1_t *gen_read_PSW_CU2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EIMASK register field
    * \return Object representing the value of the rh850g4 PSW_EIMASK register field
    */
    virtual fastiss::i8_t *gen_read_PSW_EIMASK(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_UM register field
    * \return Object representing the value of the rh850g4 PSW_UM register field
    */
    virtual fastiss::i1_t *gen_read_PSW_UM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FPSR register
     * \return Object representing the value of the rh850g4 FPSR register
     */
    virtual fastiss::i32_t *gen_read_FPSR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_CC register field
    * \return Object representing the value of the rh850g4 FPSR_CC register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_CC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_FN register field
    * \return Object representing the value of the rh850g4 FPSR_FN register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_FN(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_IF register field
    * \return Object representing the value of the rh850g4 FPSR_IF register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_IF(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_PEM register field
    * \return Object representing the value of the rh850g4 FPSR_PEM register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_PEM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_RM register field
    * \return Object representing the value of the rh850g4 FPSR_RM register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_RM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_FS register field
    * \return Object representing the value of the rh850g4 FPSR_FS register field
    */
    virtual fastiss::i1_t *gen_read_FPSR_FS(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XC register field
    * \return Object representing the value of the rh850g4 FPSR_XC register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_XC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XE register field
    * \return Object representing the value of the rh850g4 FPSR_XE register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_XE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XP register field
    * \return Object representing the value of the rh850g4 FPSR_XP register field
    */
    virtual fastiss::i8_t *gen_read_FPSR_XP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FPEPC register
     * \return Object representing the value of the rh850g4 FPEPC register
     */
    virtual fastiss::i32_t *gen_read_FPEPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FPST register
     * \return Object representing the value of the rh850g4 FPST register
     */
    virtual fastiss::i32_t *gen_read_FPST(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_XC register field
    * \return Object representing the value of the rh850g4 FPST_XC register field
    */
    virtual fastiss::i8_t *gen_read_FPST_XC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_IF register field
    * \return Object representing the value of the rh850g4 FPST_IF register field
    */
    virtual fastiss::i1_t *gen_read_FPST_IF(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_XP register field
    * \return Object representing the value of the rh850g4 FPST_XP register field
    */
    virtual fastiss::i8_t *gen_read_FPST_XP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FPCC register
     * \return Object representing the value of the rh850g4 FPCC register
     */
    virtual fastiss::i32_t *gen_read_FPCC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPCC_CC register field
    * \return Object representing the value of the rh850g4 FPCC_CC register field
    */
    virtual fastiss::i8_t *gen_read_FPCC_CC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FPCFG register
     * \return Object representing the value of the rh850g4 FPCFG register
     */
    virtual fastiss::i32_t *gen_read_FPCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPCFG_RM register field
    * \return Object representing the value of the rh850g4 FPCFG_RM register field
    */
    virtual fastiss::i8_t *gen_read_FPCFG_RM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FPCFG_XE register field
    * \return Object representing the value of the rh850g4 FPCFG_XE register field
    */
    virtual fastiss::i8_t *gen_read_FPCFG_XE(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 EIIC register
     * \return Object representing the value of the rh850g4 EIIC register
     */
    virtual fastiss::i32_t *gen_read_EIIC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FEIC register
     * \return Object representing the value of the rh850g4 FEIC register
     */
    virtual fastiss::i32_t *gen_read_FEIC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PSWH register
     * \return Object representing the value of the rh850g4 PSWH register
     */
    virtual fastiss::i32_t *gen_read_PSWH(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSWH_GPID register field
    * \return Object representing the value of the rh850g4 PSWH_GPID register field
    */
    virtual fastiss::i8_t *gen_read_PSWH_GPID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PSWH_GM register field
    * \return Object representing the value of the rh850g4 PSWH_GM register field
    */
    virtual fastiss::i1_t *gen_read_PSWH_GM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 CTPC register
     * \return Object representing the value of the rh850g4 CTPC register
     */
    virtual fastiss::i32_t *gen_read_CTPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 CTPSW register
     * \return Object representing the value of the rh850g4 CTPSW register
     */
    virtual fastiss::i32_t *gen_read_CTPSW(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 EIPSWH register
     * \return Object representing the value of the rh850g4 EIPSWH register
     */
    virtual fastiss::i32_t *gen_read_EIPSWH(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 EIPSWH_GPID register field
    * \return Object representing the value of the rh850g4 EIPSWH_GPID register field
    */
    virtual fastiss::i8_t *gen_read_EIPSWH_GPID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 EIPSWH_GM register field
    * \return Object representing the value of the rh850g4 EIPSWH_GM register field
    */
    virtual fastiss::i1_t *gen_read_EIPSWH_GM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FEPSWH register
     * \return Object representing the value of the rh850g4 FEPSWH register
     */
    virtual fastiss::i32_t *gen_read_FEPSWH(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FEPSWH_GPID register field
    * \return Object representing the value of the rh850g4 FEPSWH_GPID register field
    */
    virtual fastiss::i8_t *gen_read_FEPSWH_GPID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FEPSWH_GM register field
    * \return Object representing the value of the rh850g4 FEPSWH_GM register field
    */
    virtual fastiss::i1_t *gen_read_FEPSWH_GM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 CTBP register
     * \return Object representing the value of the rh850g4 CTBP register
     */
    virtual fastiss::i32_t *gen_read_CTBP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 SNZCFG register
     * \return Object representing the value of the rh850g4 SNZCFG register
     */
    virtual fastiss::i32_t *gen_read_SNZCFG(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 EIWR register
     * \return Object representing the value of the rh850g4 EIWR register
     */
    virtual fastiss::i32_t *gen_read_EIWR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FEWR register
     * \return Object representing the value of the rh850g4 FEWR register
     */
    virtual fastiss::i32_t *gen_read_FEWR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 SPID register
     * \return Object representing the value of the rh850g4 SPID register
     */
    virtual fastiss::i32_t *gen_read_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 SPIDLIST register
     * \return Object representing the value of the rh850g4 SPIDLIST register
     */
    virtual fastiss::i32_t *gen_read_SPIDLIST(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RBASE register
     * \return Object representing the value of the rh850g4 RBASE register
     */
    virtual fastiss::i32_t *gen_read_RBASE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_RBASE register field
    * \return Object representing the value of the rh850g4 RBASE_RBASE register field
    */
    virtual fastiss::i32_t *gen_read_RBASE_RBASE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_DV register field
    * \return Object representing the value of the rh850g4 RBASE_DV register field
    */
    virtual fastiss::i1_t *gen_read_RBASE_DV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_RINT register field
    * \return Object representing the value of the rh850g4 RBASE_RINT register field
    */
    virtual fastiss::i1_t *gen_read_RBASE_RINT(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 EBASE register
     * \return Object representing the value of the rh850g4 EBASE register
     */
    virtual fastiss::i32_t *gen_read_EBASE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_EBASE register field
    * \return Object representing the value of the rh850g4 EBASE_EBASE register field
    */
    virtual fastiss::i32_t *gen_read_EBASE_EBASE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_DV register field
    * \return Object representing the value of the rh850g4 EBASE_DV register field
    */
    virtual fastiss::i1_t *gen_read_EBASE_DV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_RINT register field
    * \return Object representing the value of the rh850g4 EBASE_RINT register field
    */
    virtual fastiss::i1_t *gen_read_EBASE_RINT(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 INTBP register
     * \return Object representing the value of the rh850g4 INTBP register
     */
    virtual fastiss::i32_t *gen_read_INTBP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MCTL register
     * \return Object representing the value of the rh850g4 MCTL register
     */
    virtual fastiss::i32_t *gen_read_MCTL(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCTL_UIC register field
    * \return Object representing the value of the rh850g4 MCTL_UIC register field
    */
    virtual fastiss::i1_t *gen_read_MCTL_UIC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PID register
     * \return Object representing the value of the rh850g4 PID register
     */
    virtual fastiss::i32_t *gen_read_PID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 SVLOCK register
     * \return Object representing the value of the rh850g4 SVLOCK register
     */
    virtual fastiss::i32_t *gen_read_SVLOCK(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 SCCFG register
     * \return Object representing the value of the rh850g4 SCCFG register
     */
    virtual fastiss::i32_t *gen_read_SCCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 SCCFG_SIZE register field
    * \return Object representing the value of the rh850g4 SCCFG_SIZE register field
    */
    virtual fastiss::i8_t *gen_read_SCCFG_SIZE(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 SCBP register
     * \return Object representing the value of the rh850g4 SCBP register
     */
    virtual fastiss::i32_t *gen_read_SCBP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 HVCFG register
     * \return Object representing the value of the rh850g4 HVCFG register
     */
    virtual fastiss::i32_t *gen_read_HVCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 HVCFG_HVE register field
    * \return Object representing the value of the rh850g4 HVCFG_HVE register field
    */
    virtual fastiss::i1_t *gen_read_HVCFG_HVE(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMCFG register
     * \return Object representing the value of the rh850g4 GMCFG register
     */
    virtual fastiss::i32_t *gen_read_GMCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GMP register field
    * \return Object representing the value of the rh850g4 GMCFG_GMP register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GMP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_HMP register field
    * \return Object representing the value of the rh850g4 GMCFG_HMP register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_HMP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GSYSE register field
    * \return Object representing the value of the rh850g4 GMCFG_GSYSE register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GSYSE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU0 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU0 register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GCU0(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU1 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU1 register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GCU1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU2 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU2 register field
    */
    virtual fastiss::i1_t *gen_read_GMCFG_GCU2(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 HVSB register
     * \return Object representing the value of the rh850g4 HVSB register
     */
    virtual fastiss::i32_t *gen_read_HVSB(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PEID register
     * \return Object representing the value of the rh850g4 PEID register
     */
    virtual fastiss::i32_t *gen_read_PEID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BMID register
     * \return Object representing the value of the rh850g4 BMID register
     */
    virtual fastiss::i32_t *gen_read_BMID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 BMID_BMID register field
    * \return Object representing the value of the rh850g4 BMID_BMID register field
    */
    virtual fastiss::i8_t *gen_read_BMID_BMID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MEA register
     * \return Object representing the value of the rh850g4 MEA register
     */
    virtual fastiss::i32_t *gen_read_MEA(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MEI register
     * \return Object representing the value of the rh850g4 MEI register
     */
    virtual fastiss::i32_t *gen_read_MEI(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RBCR0 register
     * \return Object representing the value of the rh850g4 RBCR0 register
     */
    virtual fastiss::i32_t *gen_read_RBCR0(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR0_MD register field
    * \return Object representing the value of the rh850g4 RBCR0_MD register field
    */
    virtual fastiss::i1_t *gen_read_RBCR0_MD(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR0_BE register field
    * \return Object representing the value of the rh850g4 RBCR0_BE register field
    */
    virtual fastiss::i16_t *gen_read_RBCR0_BE(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RBCR1 register
     * \return Object representing the value of the rh850g4 RBCR1 register
     */
    virtual fastiss::i32_t *gen_read_RBCR1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR1_NC register field
    * \return Object representing the value of the rh850g4 RBCR1_NC register field
    */
    virtual fastiss::i16_t *gen_read_RBCR1_NC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RBNR register
     * \return Object representing the value of the rh850g4 RBNR register
     */
    virtual fastiss::i32_t *gen_read_RBNR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBNR_BN register field
    * \return Object representing the value of the rh850g4 RBNR_BN register field
    */
    virtual fastiss::i8_t *gen_read_RBNR_BN(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RBIP register
     * \return Object representing the value of the rh850g4 RBIP register
     */
    virtual fastiss::i32_t *gen_read_RBIP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 RBIP_RBIP register field
    * \return Object representing the value of the rh850g4 RBIP_RBIP register field
    */
    virtual fastiss::i32_t *gen_read_RBIP_RBIP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ISPR register
     * \return Object representing the value of the rh850g4 ISPR register
     */
    virtual fastiss::i32_t *gen_read_ISPR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ISPR_ISP register field
    * \return Object representing the value of the rh850g4 ISPR_ISP register field
    */
    virtual fastiss::i16_t *gen_read_ISPR_ISP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 IMSR register
     * \return Object representing the value of the rh850g4 IMSR register
     */
    virtual fastiss::i32_t *gen_read_IMSR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EEIM register field
    * \return Object representing the value of the rh850g4 IMSR_EEIM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_EEIM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EPLM register field
    * \return Object representing the value of the rh850g4 IMSR_EPLM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_EPLM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EID register field
    * \return Object representing the value of the rh850g4 IMSR_EID register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_EID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_ENP register field
    * \return Object representing the value of the rh850g4 IMSR_ENP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_ENP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_FNP register field
    * \return Object representing the value of the rh850g4 IMSR_FNP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_FNP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEEIM register field
    * \return Object representing the value of the rh850g4 IMSR_HEEIM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HEEIM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEPLM register field
    * \return Object representing the value of the rh850g4 IMSR_HEPLM register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HEPLM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEID register field
    * \return Object representing the value of the rh850g4 IMSR_HEID register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HEID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HENP register field
    * \return Object representing the value of the rh850g4 IMSR_HENP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HENP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HFNP register field
    * \return Object representing the value of the rh850g4 IMSR_HFNP register field
    */
    virtual fastiss::i1_t *gen_read_IMSR_HFNP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICSR register
     * \return Object representing the value of the rh850g4 ICSR register
     */
    virtual fastiss::i32_t *gen_read_ICSR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICSR_PMEI register field
    * \return Object representing the value of the rh850g4 ICSR_PMEI register field
    */
    virtual fastiss::i1_t *gen_read_ICSR_PMEI(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 INTCFG register
     * \return Object representing the value of the rh850g4 INTCFG register
     */
    virtual fastiss::i32_t *gen_read_INTCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_ISPC register field
    * \return Object representing the value of the rh850g4 INTCFG_ISPC register field
    */
    virtual fastiss::i1_t *gen_read_INTCFG_ISPC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_EPL register field
    * \return Object representing the value of the rh850g4 INTCFG_EPL register field
    */
    virtual fastiss::i1_t *gen_read_INTCFG_EPL(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_ULNR register field
    * \return Object representing the value of the rh850g4 INTCFG_ULNR register field
    */
    virtual fastiss::i8_t *gen_read_INTCFG_ULNR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PLMR register
     * \return Object representing the value of the rh850g4 PLMR register
     */
    virtual fastiss::i32_t *gen_read_PLMR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 PLMR_PLM register field
    * \return Object representing the value of the rh850g4 PLMR_PLM register field
    */
    virtual fastiss::i8_t *gen_read_PLMR_PLM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FXSR register
     * \return Object representing the value of the rh850g4 FXSR register
     */
    virtual fastiss::i32_t *gen_read_FXSR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_FN register field
    * \return Object representing the value of the rh850g4 FXSR_FN register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_FN(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_IF register field
    * \return Object representing the value of the rh850g4 FXSR_IF register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_IF(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_PEM register field
    * \return Object representing the value of the rh850g4 FXSR_PEM register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_PEM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_RM register field
    * \return Object representing the value of the rh850g4 FXSR_RM register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_RM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_FS register field
    * \return Object representing the value of the rh850g4 FXSR_FS register field
    */
    virtual fastiss::i1_t *gen_read_FXSR_FS(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XC register field
    * \return Object representing the value of the rh850g4 FXSR_XC register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_XC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XE register field
    * \return Object representing the value of the rh850g4 FXSR_XE register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_XE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XP register field
    * \return Object representing the value of the rh850g4 FXSR_XP register field
    */
    virtual fastiss::i8_t *gen_read_FXSR_XP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FXST register
     * \return Object representing the value of the rh850g4 FXST register
     */
    virtual fastiss::i32_t *gen_read_FXST(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_XC register field
    * \return Object representing the value of the rh850g4 FXST_XC register field
    */
    virtual fastiss::i8_t *gen_read_FXST_XC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_IF register field
    * \return Object representing the value of the rh850g4 FXST_IF register field
    */
    virtual fastiss::i1_t *gen_read_FXST_IF(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_XP register field
    * \return Object representing the value of the rh850g4 FXST_XP register field
    */
    virtual fastiss::i8_t *gen_read_FXST_XP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FXINFO register
     * \return Object representing the value of the rh850g4 FXINFO register
     */
    virtual fastiss::i32_t *gen_read_FXINFO(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXINFO_NFPU register field
    * \return Object representing the value of the rh850g4 FXINFO_NFPU register field
    */
    virtual fastiss::i1_t *gen_read_FXINFO_NFPU(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXINFO_RSIZE register field
    * \return Object representing the value of the rh850g4 FXINFO_RSIZE register field
    */
    virtual fastiss::i1_t *gen_read_FXINFO_RSIZE(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FXCFG register
     * \return Object representing the value of the rh850g4 FXCFG register
     */
    virtual fastiss::i32_t *gen_read_FXCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXCFG_RM register field
    * \return Object representing the value of the rh850g4 FXCFG_RM register field
    */
    virtual fastiss::i8_t *gen_read_FXCFG_RM(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXCFG_XE register field
    * \return Object representing the value of the rh850g4 FXCFG_XE register field
    */
    virtual fastiss::i8_t *gen_read_FXCFG_XE(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FXXC register
     * \return Object representing the value of the rh850g4 FXXC register
     */
    virtual fastiss::i32_t *gen_read_FXXC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC3 register field
    * \return Object representing the value of the rh850g4 FXXC_XC3 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC3(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC2 register field
    * \return Object representing the value of the rh850g4 FXXC_XC2 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC1 register field
    * \return Object representing the value of the rh850g4 FXXC_XC1 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC0 register field
    * \return Object representing the value of the rh850g4 FXXC_XC0 register field
    */
    virtual fastiss::i8_t *gen_read_FXXC_XC0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 FXXP register
     * \return Object representing the value of the rh850g4 FXXP register
     */
    virtual fastiss::i32_t *gen_read_FXXP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP3 register field
    * \return Object representing the value of the rh850g4 FXXP_XP3 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP3(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP2 register field
    * \return Object representing the value of the rh850g4 FXXP_XP2 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP1 register field
    * \return Object representing the value of the rh850g4 FXXP_XP1 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP0 register field
    * \return Object representing the value of the rh850g4 FXXP_XP0 register field
    */
    virtual fastiss::i8_t *gen_read_FXXP_XP0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIPC register
     * \return Object representing the value of the rh850g4 GMEIPC register
     */
    virtual fastiss::i32_t *gen_read_GMEIPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIPSW register
     * \return Object representing the value of the rh850g4 GMEIPSW register
     */
    virtual fastiss::i32_t *gen_read_GMEIPSW(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEPC register
     * \return Object representing the value of the rh850g4 GMFEPC register
     */
    virtual fastiss::i32_t *gen_read_GMFEPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEPSW register
     * \return Object representing the value of the rh850g4 GMFEPSW register
     */
    virtual fastiss::i32_t *gen_read_GMFEPSW(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMPSW register
     * \return Object representing the value of the rh850g4 GMPSW register
     */
    virtual fastiss::i32_t *gen_read_GMPSW(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_Z register field
    * \return Object representing the value of the rh850g4 GMPSW_Z register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_Z(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_S register field
    * \return Object representing the value of the rh850g4 GMPSW_S register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_S(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_OV register field
    * \return Object representing the value of the rh850g4 GMPSW_OV register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_OV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CY register field
    * \return Object representing the value of the rh850g4 GMPSW_CY register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CY(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_SAT register field
    * \return Object representing the value of the rh850g4 GMPSW_SAT register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_SAT(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_ID register field
    * \return Object representing the value of the rh850g4 GMPSW_ID register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_ID(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EP register field
    * \return Object representing the value of the rh850g4 GMPSW_EP register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_EP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_NP register field
    * \return Object representing the value of the rh850g4 GMPSW_NP register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_NP(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EBV register field
    * \return Object representing the value of the rh850g4 GMPSW_EBV register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_EBV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU0 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU0 register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CU0(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU1 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU1 register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CU1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU2 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU2 register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_CU2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EIMASK register field
    * \return Object representing the value of the rh850g4 GMPSW_EIMASK register field
    */
    virtual fastiss::i8_t *gen_read_GMPSW_EIMASK(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_UM register field
    * \return Object representing the value of the rh850g4 GMPSW_UM register field
    */
    virtual fastiss::i1_t *gen_read_GMPSW_UM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMMEA register
     * \return Object representing the value of the rh850g4 GMMEA register
     */
    virtual fastiss::i32_t *gen_read_GMMEA(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMMEI register
     * \return Object representing the value of the rh850g4 GMMEI register
     */
    virtual fastiss::i32_t *gen_read_GMMEI(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIIC register
     * \return Object representing the value of the rh850g4 GMEIIC register
     */
    virtual fastiss::i32_t *gen_read_GMEIIC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEIC register
     * \return Object representing the value of the rh850g4 GMFEIC register
     */
    virtual fastiss::i32_t *gen_read_GMFEIC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMSPID register
     * \return Object representing the value of the rh850g4 GMSPID register
     */
    virtual fastiss::i32_t *gen_read_GMSPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMSPIDLIST register
     * \return Object representing the value of the rh850g4 GMSPIDLIST register
     */
    virtual fastiss::i32_t *gen_read_GMSPIDLIST(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMEBASE register
     * \return Object representing the value of the rh850g4 GMEBASE register
     */
    virtual fastiss::i32_t *gen_read_GMEBASE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_EBASE register field
    * \return Object representing the value of the rh850g4 GMEBASE_EBASE register field
    */
    virtual fastiss::i32_t *gen_read_GMEBASE_EBASE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_DV register field
    * \return Object representing the value of the rh850g4 GMEBASE_DV register field
    */
    virtual fastiss::i1_t *gen_read_GMEBASE_DV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_RINT register field
    * \return Object representing the value of the rh850g4 GMEBASE_RINT register field
    */
    virtual fastiss::i1_t *gen_read_GMEBASE_RINT(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMINTBP register
     * \return Object representing the value of the rh850g4 GMINTBP register
     */
    virtual fastiss::i32_t *gen_read_GMINTBP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMINTCFG register
     * \return Object representing the value of the rh850g4 GMINTCFG register
     */
    virtual fastiss::i32_t *gen_read_GMINTCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_ISPC register field
    * \return Object representing the value of the rh850g4 GMINTCFG_ISPC register field
    */
    virtual fastiss::i1_t *gen_read_GMINTCFG_ISPC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_EPL register field
    * \return Object representing the value of the rh850g4 GMINTCFG_EPL register field
    */
    virtual fastiss::i1_t *gen_read_GMINTCFG_EPL(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_ULNR register field
    * \return Object representing the value of the rh850g4 GMINTCFG_ULNR register field
    */
    virtual fastiss::i8_t *gen_read_GMINTCFG_ULNR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMPLMR register
     * \return Object representing the value of the rh850g4 GMPLMR register
     */
    virtual fastiss::i32_t *gen_read_GMPLMR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 GMPLMR_PLM register field
    * \return Object representing the value of the rh850g4 GMPLMR_PLM register field
    */
    virtual fastiss::i8_t *gen_read_GMPLMR_PLM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMSVLOCK register
     * \return Object representing the value of the rh850g4 GMSVLOCK register
     */
    virtual fastiss::i32_t *gen_read_GMSVLOCK(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMMPM register
     * \return Object representing the value of the rh850g4 GMMPM register
     */
    virtual fastiss::i32_t *gen_read_GMMPM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIWR register
     * \return Object representing the value of the rh850g4 GMEIWR register
     */
    virtual fastiss::i32_t *gen_read_GMEIWR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEWR register
     * \return Object representing the value of the rh850g4 GMFEWR register
     */
    virtual fastiss::i32_t *gen_read_GMFEWR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 GMPEID register
     * \return Object representing the value of the rh850g4 GMPEID register
     */
    virtual fastiss::i32_t *gen_read_GMPEID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPM register
     * \return Object representing the value of the rh850g4 MPM register
     */
    virtual fastiss::i32_t *gen_read_MPM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPCFG register
     * \return Object representing the value of the rh850g4 MPCFG register
     */
    virtual fastiss::i32_t *gen_read_MPCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_NMPUE register field
    * \return Object representing the value of the rh850g4 MPCFG_NMPUE register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_NMPUE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_HBE register field
    * \return Object representing the value of the rh850g4 MPCFG_HBE register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_HBE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_ARCH register field
    * \return Object representing the value of the rh850g4 MPCFG_ARCH register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_ARCH(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_NBK register field
    * \return Object representing the value of the rh850g4 MPCFG_NBK register field
    */
    virtual fastiss::i8_t *gen_read_MPCFG_NBK(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_DMDP register field
    * \return Object representing the value of the rh850g4 MPCFG_DMDP register field
    */
    virtual fastiss::i1_t *gen_read_MPCFG_DMDP(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MCA register
     * \return Object representing the value of the rh850g4 MCA register
     */
    virtual fastiss::i32_t *gen_read_MCA(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MCS register
     * \return Object representing the value of the rh850g4 MCS register
     */
    virtual fastiss::i32_t *gen_read_MCS(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MCC register
     * \return Object representing the value of the rh850g4 MCC register
     */
    virtual fastiss::i32_t *gen_read_MCC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MCR register
     * \return Object representing the value of the rh850g4 MCR register
     */
    virtual fastiss::i32_t *gen_read_MCR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSXE register field
    * \return Object representing the value of the rh850g4 MCR_HSXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HSXE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSWE register field
    * \return Object representing the value of the rh850g4 MCR_HSWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HSWE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSRE register field
    * \return Object representing the value of the rh850g4 MCR_HSRE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HSRE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HUXE register field
    * \return Object representing the value of the rh850g4 MCR_HUXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HUXE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HUWE register field
    * \return Object representing the value of the rh850g4 MCR_HUWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HUWE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HURE register field
    * \return Object representing the value of the rh850g4 MCR_HURE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_HURE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSXE register field
    * \return Object representing the value of the rh850g4 MCR_GSXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GSXE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSWE register field
    * \return Object representing the value of the rh850g4 MCR_GSWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GSWE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSRE register field
    * \return Object representing the value of the rh850g4 MCR_GSRE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GSRE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GUXE register field
    * \return Object representing the value of the rh850g4 MCR_GUXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GUXE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GUWE register field
    * \return Object representing the value of the rh850g4 MCR_GUWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GUWE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GURE register field
    * \return Object representing the value of the rh850g4 MCR_GURE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_GURE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_OV register field
    * \return Object representing the value of the rh850g4 MCR_OV register field
    */
    virtual fastiss::i1_t *gen_read_MCR_OV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SXE register field
    * \return Object representing the value of the rh850g4 MCR_SXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_SXE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SWE register field
    * \return Object representing the value of the rh850g4 MCR_SWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_SWE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SRE register field
    * \return Object representing the value of the rh850g4 MCR_SRE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_SRE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_UXE register field
    * \return Object representing the value of the rh850g4 MCR_UXE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_UXE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_UWE register field
    * \return Object representing the value of the rh850g4 MCR_UWE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_UWE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_URE register field
    * \return Object representing the value of the rh850g4 MCR_URE register field
    */
    virtual fastiss::i1_t *gen_read_MCR_URE(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MCI register
     * \return Object representing the value of the rh850g4 MCI register
     */
    virtual fastiss::i32_t *gen_read_MCI(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MCI_SPID register field
    * \return Object representing the value of the rh850g4 MCI_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MCI_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPIDX register
     * \return Object representing the value of the rh850g4 MPIDX register
     */
    virtual fastiss::i32_t *gen_read_MPIDX(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPIDX_IDX register field
    * \return Object representing the value of the rh850g4 MPIDX_IDX register field
    */
    virtual fastiss::i8_t *gen_read_MPIDX_IDX(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPBK register
     * \return Object representing the value of the rh850g4 MPBK register
     */
    virtual fastiss::i32_t *gen_read_MPBK(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPBK_BK register field
    * \return Object representing the value of the rh850g4 MPBK_BK register field
    */
    virtual fastiss::i8_t *gen_read_MPBK_BK(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPLA register
     * \return Object representing the value of the rh850g4 MPLA register
     */
    virtual fastiss::i32_t *gen_read_MPLA(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPUA register
     * \return Object representing the value of the rh850g4 MPUA register
     */
    virtual fastiss::i32_t *gen_read_MPUA(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPAT register
     * \return Object representing the value of the rh850g4 MPAT register
     */
    virtual fastiss::i32_t *gen_read_MPAT(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID7 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID7 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID7(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID6 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID6 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID6(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID5 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID5 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID5(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID4 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID4 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID4(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID3 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID3 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID3(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID2 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID2 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID1 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID1 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID0 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID0 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WMPID0(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID7 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID7 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID7(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID6 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID6 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID6(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID5 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID5 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID5(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID4 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID4 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID4(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID3 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID3 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID3(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID2 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID2 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID1 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID1 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID0 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID0 register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RMPID0(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WG register field
    * \return Object representing the value of the rh850g4 MPAT_WG register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_WG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RG register field
    * \return Object representing the value of the rh850g4 MPAT_RG register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_RG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_E register field
    * \return Object representing the value of the rh850g4 MPAT_E register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_E(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SX register field
    * \return Object representing the value of the rh850g4 MPAT_SX register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_SX(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SW register field
    * \return Object representing the value of the rh850g4 MPAT_SW register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_SW(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SR register field
    * \return Object representing the value of the rh850g4 MPAT_SR register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_SR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UX register field
    * \return Object representing the value of the rh850g4 MPAT_UX register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_UX(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UW register field
    * \return Object representing the value of the rh850g4 MPAT_UW register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_UW(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UR register field
    * \return Object representing the value of the rh850g4 MPAT_UR register field
    */
    virtual fastiss::i1_t *gen_read_MPAT_UR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID0 register
     * \return Object representing the value of the rh850g4 MPID0 register
     */
    virtual fastiss::i32_t *gen_read_MPID0(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID0_SPID register field
    * \return Object representing the value of the rh850g4 MPID0_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID0_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID1 register
     * \return Object representing the value of the rh850g4 MPID1 register
     */
    virtual fastiss::i32_t *gen_read_MPID1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID1_SPID register field
    * \return Object representing the value of the rh850g4 MPID1_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID1_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID2 register
     * \return Object representing the value of the rh850g4 MPID2 register
     */
    virtual fastiss::i32_t *gen_read_MPID2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID2_SPID register field
    * \return Object representing the value of the rh850g4 MPID2_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID2_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID3 register
     * \return Object representing the value of the rh850g4 MPID3 register
     */
    virtual fastiss::i32_t *gen_read_MPID3(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID3_SPID register field
    * \return Object representing the value of the rh850g4 MPID3_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID3_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID4 register
     * \return Object representing the value of the rh850g4 MPID4 register
     */
    virtual fastiss::i32_t *gen_read_MPID4(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID4_SPID register field
    * \return Object representing the value of the rh850g4 MPID4_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID4_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID5 register
     * \return Object representing the value of the rh850g4 MPID5 register
     */
    virtual fastiss::i32_t *gen_read_MPID5(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID5_SPID register field
    * \return Object representing the value of the rh850g4 MPID5_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID5_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID6 register
     * \return Object representing the value of the rh850g4 MPID6 register
     */
    virtual fastiss::i32_t *gen_read_MPID6(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID6_SPID register field
    * \return Object representing the value of the rh850g4 MPID6_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID6_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 MPID7 register
     * \return Object representing the value of the rh850g4 MPID7 register
     */
    virtual fastiss::i32_t *gen_read_MPID7(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 MPID7_SPID register field
    * \return Object representing the value of the rh850g4 MPID7_SPID register field
    */
    virtual fastiss::i8_t *gen_read_MPID7_SPID(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICTAGL register
     * \return Object representing the value of the rh850g4 ICTAGL register
     */
    virtual fastiss::i32_t *gen_read_ICTAGL(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_LPN register field
    * \return Object representing the value of the rh850g4 ICTAGL_LPN register field
    */
    virtual fastiss::i32_t *gen_read_ICTAGL_LPN(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_LRU register field
    * \return Object representing the value of the rh850g4 ICTAGL_LRU register field
    */
    virtual fastiss::i8_t *gen_read_ICTAGL_LRU(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_L register field
    * \return Object representing the value of the rh850g4 ICTAGL_L register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGL_L(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_V register field
    * \return Object representing the value of the rh850g4 ICTAGL_V register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGL_V(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICTAGH register
     * \return Object representing the value of the rh850g4 ICTAGH register
     */
    virtual fastiss::i32_t *gen_read_ICTAGH(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_WD register field
    * \return Object representing the value of the rh850g4 ICTAGH_WD register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_WD(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_PD register field
    * \return Object representing the value of the rh850g4 ICTAGH_PD register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_PD(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_WT register field
    * \return Object representing the value of the rh850g4 ICTAGH_WT register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_WT(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_PT register field
    * \return Object representing the value of the rh850g4 ICTAGH_PT register field
    */
    virtual fastiss::i1_t *gen_read_ICTAGH_PT(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_DATAECC register field
    * \return Object representing the value of the rh850g4 ICTAGH_DATAECC register field
    */
    virtual fastiss::i8_t *gen_read_ICTAGH_DATAECC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_TAGECC register field
    * \return Object representing the value of the rh850g4 ICTAGH_TAGECC register field
    */
    virtual fastiss::i8_t *gen_read_ICTAGH_TAGECC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICDATL register
     * \return Object representing the value of the rh850g4 ICDATL register
     */
    virtual fastiss::i32_t *gen_read_ICDATL(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICDATH register
     * \return Object representing the value of the rh850g4 ICDATH register
     */
    virtual fastiss::i32_t *gen_read_ICDATH(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICCTRL register
     * \return Object representing the value of the rh850g4 ICCTRL register
     */
    virtual fastiss::i32_t *gen_read_ICCTRL(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_PBS register field
    * \return Object representing the value of the rh850g4 ICCTRL_PBS register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_PBS(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHCLR register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHCLR register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHCLR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEIV register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEIV register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHEIV(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEMK register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEMK register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHEMK(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEN register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEN register field
    */
    virtual fastiss::i1_t *gen_read_ICCTRL_ICHEN(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICCFG register
     * \return Object representing the value of the rh850g4 ICCFG register
     */
    virtual fastiss::i32_t *gen_read_ICCFG(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHSIZE register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHSIZE register field
    */
    virtual fastiss::i8_t *gen_read_ICCFG_ICHSIZE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHLINE register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHLINE register field
    */
    virtual fastiss::i8_t *gen_read_ICCFG_ICHLINE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHWAY register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHWAY register field
    */
    virtual fastiss::i8_t *gen_read_ICCFG_ICHWAY(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICERR register
     * \return Object representing the value of the rh850g4 ICERR register
     */
    virtual fastiss::i32_t *gen_read_ICERR(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_CISTW register field
    * \return Object representing the value of the rh850g4 ICERR_CISTW register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_CISTW(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESAFE register field
    * \return Object representing the value of the rh850g4 ICERR_ESAFE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESAFE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESMH register field
    * \return Object representing the value of the rh850g4 ICERR_ESMH register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESMH(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESPBSE register field
    * \return Object representing the value of the rh850g4 ICERR_ESPBSE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESPBSE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESTE1 register field
    * \return Object representing the value of the rh850g4 ICERR_ESTE1 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESTE1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESTE2 register field
    * \return Object representing the value of the rh850g4 ICERR_ESTE2 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESTE2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESDC register field
    * \return Object representing the value of the rh850g4 ICERR_ESDC register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESDC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESDE register field
    * \return Object representing the value of the rh850g4 ICERR_ESDE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ESDE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMAFE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMAFE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMAFE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMMH register field
    * \return Object representing the value of the rh850g4 ICERR_ERMMH register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMMH(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMPBSE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMPBSE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMPBSE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMTE1 register field
    * \return Object representing the value of the rh850g4 ICERR_ERMTE1 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMTE1(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMTE2 register field
    * \return Object representing the value of the rh850g4 ICERR_ERMTE2 register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMTE2(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMDC register field
    * \return Object representing the value of the rh850g4 ICERR_ERMDC register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMDC(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMDE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMDE register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ERMDE(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHEWY register field
    * \return Object representing the value of the rh850g4 ICERR_ICHEWY register field
    */
    virtual fastiss::i8_t *gen_read_ICERR_ICHEWY(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHEIX register field
    * \return Object representing the value of the rh850g4 ICERR_ICHEIX register field
    */
    virtual fastiss::i8_t *gen_read_ICERR_ICHEIX(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHERQ register field
    * \return Object representing the value of the rh850g4 ICERR_ICHERQ register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHERQ(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHED register field
    * \return Object representing the value of the rh850g4 ICERR_ICHED register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHED(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHET register field
    * \return Object representing the value of the rh850g4 ICERR_ICHET register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHET(void) = 0;
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHERR register field
    * \return Object representing the value of the rh850g4 ICERR_ICHERR register field
    */
    virtual fastiss::i1_t *gen_read_ICERR_ICHERR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 TSCOUNTL register
     * \return Object representing the value of the rh850g4 TSCOUNTL register
     */
    virtual fastiss::i32_t *gen_read_TSCOUNTL(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 TSCOUNTH register
     * \return Object representing the value of the rh850g4 TSCOUNTH register
     */
    virtual fastiss::i32_t *gen_read_TSCOUNTH(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 TSCTRL register
     * \return Object representing the value of the rh850g4 TSCTRL register
     */
    virtual fastiss::i32_t *gen_read_TSCTRL(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMUMCTRL register
     * \return Object representing the value of the rh850g4 PMUMCTRL register
     */
    virtual fastiss::i32_t *gen_read_PMUMCTRL(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMGMCTRL register
     * \return Object representing the value of the rh850g4 PMGMCTRL register
     */
    virtual fastiss::i32_t *gen_read_PMGMCTRL(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT0 register
     * \return Object representing the value of the rh850g4 PMCOUNT0 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL0 register
     * \return Object representing the value of the rh850g4 PMCTRL0 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT1 register
     * \return Object representing the value of the rh850g4 PMCOUNT1 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL1 register
     * \return Object representing the value of the rh850g4 PMCTRL1 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT2 register
     * \return Object representing the value of the rh850g4 PMCOUNT2 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT2(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL2 register
     * \return Object representing the value of the rh850g4 PMCTRL2 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL2(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT3 register
     * \return Object representing the value of the rh850g4 PMCOUNT3 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT3(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL3 register
     * \return Object representing the value of the rh850g4 PMCTRL3 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL3(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL4 register
     * \return Object representing the value of the rh850g4 PMCTRL4 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL4(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL5 register
     * \return Object representing the value of the rh850g4 PMCTRL5 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL5(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL6 register
     * \return Object representing the value of the rh850g4 PMCTRL6 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL6(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL7 register
     * \return Object representing the value of the rh850g4 PMCTRL7 register
     */
    virtual fastiss::i32_t *gen_read_PMCTRL7(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT4 register
     * \return Object representing the value of the rh850g4 PMCOUNT4 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT4(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT5 register
     * \return Object representing the value of the rh850g4 PMCOUNT5 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT5(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT6 register
     * \return Object representing the value of the rh850g4 PMCOUNT6 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT6(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT7 register
     * \return Object representing the value of the rh850g4 PMCOUNT7 register
     */
    virtual fastiss::i32_t *gen_read_PMCOUNT7(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND0 register
     * \return Object representing the value of the rh850g4 PMSUBCND0 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND1 register
     * \return Object representing the value of the rh850g4 PMSUBCND1 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND2 register
     * \return Object representing the value of the rh850g4 PMSUBCND2 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND2(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND3 register
     * \return Object representing the value of the rh850g4 PMSUBCND3 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND3(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND4 register
     * \return Object representing the value of the rh850g4 PMSUBCND4 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND4(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND5 register
     * \return Object representing the value of the rh850g4 PMSUBCND5 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND5(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND6 register
     * \return Object representing the value of the rh850g4 PMSUBCND6 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND6(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND7 register
     * \return Object representing the value of the rh850g4 PMSUBCND7 register
     */
    virtual fastiss::i32_t *gen_read_PMSUBCND7(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DBGEN register
     * \return Object representing the value of the rh850g4 DBGEN register
     */
    virtual fastiss::i32_t *gen_read_DBGEN(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DBPSWH register
     * \return Object representing the value of the rh850g4 DBPSWH register
     */
    virtual fastiss::i32_t *gen_read_DBPSWH(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DBIC register
     * \return Object representing the value of the rh850g4 DBIC register
     */
    virtual fastiss::i32_t *gen_read_DBIC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DBPC register
     * \return Object representing the value of the rh850g4 DBPC register
     */
    virtual fastiss::i32_t *gen_read_DBPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DBPSW register
     * \return Object representing the value of the rh850g4 DBPSW register
     */
    virtual fastiss::i32_t *gen_read_DBPSW(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DIR0 register
     * \return Object representing the value of the rh850g4 DIR0 register
     */
    virtual fastiss::i32_t *gen_read_DIR0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DIR1 register
     * \return Object representing the value of the rh850g4 DIR1 register
     */
    virtual fastiss::i32_t *gen_read_DIR1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BPC register
     * \return Object representing the value of the rh850g4 BPC register
     */
    virtual fastiss::i32_t *gen_read_BPC(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BPAV register
     * \return Object representing the value of the rh850g4 BPAV register
     */
    virtual fastiss::i32_t *gen_read_BPAV(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BPAM register
     * \return Object representing the value of the rh850g4 BPAM register
     */
    virtual fastiss::i32_t *gen_read_BPAM(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DBCFG register
     * \return Object representing the value of the rh850g4 DBCFG register
     */
    virtual fastiss::i32_t *gen_read_DBCFG(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DBWR register
     * \return Object representing the value of the rh850g4 DBWR register
     */
    virtual fastiss::i32_t *gen_read_DBWR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 LSTEST0 register
     * \return Object representing the value of the rh850g4 LSTEST0 register
     */
    virtual fastiss::i32_t *gen_read_LSTEST0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 LSTEST1 register
     * \return Object representing the value of the rh850g4 LSTEST1 register
     */
    virtual fastiss::i32_t *gen_read_LSTEST1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 LSCFG register
     * \return Object representing the value of the rh850g4 LSCFG register
     */
    virtual fastiss::i32_t *gen_read_LSCFG(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 ICBKEY register
     * \return Object representing the value of the rh850g4 ICBKEY register
     */
    virtual fastiss::i32_t *gen_read_ICBKEY(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 IFCR register
     * \return Object representing the value of the rh850g4 IFCR register
     */
    virtual fastiss::i32_t *gen_read_IFCR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 IFCR1 register
     * \return Object representing the value of the rh850g4 IFCR1 register
     */
    virtual fastiss::i32_t *gen_read_IFCR1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCTRL0 register
     * \return Object representing the value of the rh850g4 BRPCTRL0 register
     */
    virtual fastiss::i32_t *gen_read_BRPCTRL0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCTRL1 register
     * \return Object representing the value of the rh850g4 BRPCTRL1 register
     */
    virtual fastiss::i32_t *gen_read_BRPCTRL1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCFG register
     * \return Object representing the value of the rh850g4 BRPCFG register
     */
    virtual fastiss::i32_t *gen_read_BRPCFG(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BRPACTRL register
     * \return Object representing the value of the rh850g4 BRPACTRL register
     */
    virtual fastiss::i32_t *gen_read_BRPACTRL(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 BRPDATA register
     * \return Object representing the value of the rh850g4 BRPDATA register
     */
    virtual fastiss::i32_t *gen_read_BRPDATA(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DCBKEY register
     * \return Object representing the value of the rh850g4 DCBKEY register
     */
    virtual fastiss::i32_t *gen_read_DCBKEY(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 LSUCR register
     * \return Object representing the value of the rh850g4 LSUCR register
     */
    virtual fastiss::i32_t *gen_read_LSUCR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 LSULNK0 register
     * \return Object representing the value of the rh850g4 LSULNK0 register
     */
    virtual fastiss::i32_t *gen_read_LSULNK0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 LSULNK1 register
     * \return Object representing the value of the rh850g4 LSULNK1 register
     */
    virtual fastiss::i32_t *gen_read_LSULNK1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLCR register
     * \return Object representing the value of the rh850g4 L1RLCR register
     */
    virtual fastiss::i32_t *gen_read_L1RLCR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLNK0 register
     * \return Object representing the value of the rh850g4 L1RLNK0 register
     */
    virtual fastiss::i32_t *gen_read_L1RLNK0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLNK1 register
     * \return Object representing the value of the rh850g4 L1RLNK1 register
     */
    virtual fastiss::i32_t *gen_read_L1RLNK1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 L1RCFG register
     * \return Object representing the value of the rh850g4 L1RCFG register
     */
    virtual fastiss::i32_t *gen_read_L1RCFG(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DECFG register
     * \return Object representing the value of the rh850g4 DECFG register
     */
    virtual fastiss::i32_t *gen_read_DECFG(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DECTRL register
     * \return Object representing the value of the rh850g4 DECTRL register
     */
    virtual fastiss::i32_t *gen_read_DECTRL(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 DEVDS register
     * \return Object representing the value of the rh850g4 DEVDS register
     */
    virtual fastiss::i32_t *gen_read_DEVDS(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBCR register
     * \return Object representing the value of the rh850g4 RDBCR register
     */
    virtual fastiss::i32_t *gen_read_RDBCR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBACR register
     * \return Object representing the value of the rh850g4 RDBACR register
     */
    virtual fastiss::i32_t *gen_read_RDBACR(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBATAG register
     * \return Object representing the value of the rh850g4 RDBATAG register
     */
    virtual fastiss::i32_t *gen_read_RDBATAG(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT0 register
     * \return Object representing the value of the rh850g4 RDBADAT0 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT0(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT1 register
     * \return Object representing the value of the rh850g4 RDBADAT1 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT1(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT2 register
     * \return Object representing the value of the rh850g4 RDBADAT2 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT2(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT3 register
     * \return Object representing the value of the rh850g4 RDBADAT3 register
     */
    virtual fastiss::i32_t *gen_read_RDBADAT3(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 RDBSTAT register
     * \return Object representing the value of the rh850g4 RDBSTAT register
     */
    virtual fastiss::i32_t *gen_read_RDBSTAT(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 hv_func_support register
     * \return Object representing the value of the rh850g4 hv_func_support register
     */
    virtual fastiss::i8_t *gen_read_hv_func_support(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 swd0pcad_exec_count register
     * \return Object representing the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual fastiss::i32_t *gen_read_swd0pcad_exec_count(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 swd0pcad_val register
     * \return Object representing the value of the rh850g4 swd0pcad_val register
     */
    virtual fastiss::i32_t *gen_read_swd0pcad_val(void) = 0;
    /**
     * \brief Generates code to query the value of the rh850g4 mem_acc_type register
     * \return Object representing the value of the rh850g4 mem_acc_type register
     */
    virtual fastiss::i32_t *gen_read_mem_acc_type(void) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeRegisterGroupReadOperations
     * @{
     */
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return Object representing the value of the specified rh850g4 GR group register
     */
    virtual fastiss::i32_t *gen_read_GR(rh850g4::GR_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return Object representing the value of the specified rh850g4 WR group register
     */
    virtual fastiss::i64_t *gen_read_WR(rh850g4::WR_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_0(rh850g4::SR_SELID_0_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_1(rh850g4::SR_SELID_1_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_2(rh850g4::SR_SELID_2_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_3(rh850g4::SR_SELID_3_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_4(rh850g4::SR_SELID_4_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_5(rh850g4::SR_SELID_5_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_9(rh850g4::SR_SELID_9_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_10(rh850g4::SR_SELID_10_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_11(rh850g4::SR_SELID_11_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_12(rh850g4::SR_SELID_12_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_13(rh850g4::SR_SELID_13_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_14(rh850g4::SR_SELID_14_index_t index) = 0;
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual fastiss::i32_t *gen_read_SR_SELID_15(rh850g4::SR_SELID_15_index_t index) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeIndividualRegisterWriteOperations
     * @{
     */
    /**
     * \brief Generates code to set the value of the rh850g4 r0 register
     * \param value Object representing the new value of the rh850g4 r0 register
     */
    virtual void gen_write_r0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r1 register
     * \param value Object representing the new value of the rh850g4 r1 register
     */
    virtual void gen_write_r1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r2 register
     * \param value Object representing the new value of the rh850g4 r2 register
     */
    virtual void gen_write_r2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r3 register
     * \param value Object representing the new value of the rh850g4 r3 register
     */
    virtual void gen_write_r3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r4 register
     * \param value Object representing the new value of the rh850g4 r4 register
     */
    virtual void gen_write_r4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r5 register
     * \param value Object representing the new value of the rh850g4 r5 register
     */
    virtual void gen_write_r5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r6 register
     * \param value Object representing the new value of the rh850g4 r6 register
     */
    virtual void gen_write_r6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r7 register
     * \param value Object representing the new value of the rh850g4 r7 register
     */
    virtual void gen_write_r7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r8 register
     * \param value Object representing the new value of the rh850g4 r8 register
     */
    virtual void gen_write_r8(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r9 register
     * \param value Object representing the new value of the rh850g4 r9 register
     */
    virtual void gen_write_r9(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r10 register
     * \param value Object representing the new value of the rh850g4 r10 register
     */
    virtual void gen_write_r10(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r11 register
     * \param value Object representing the new value of the rh850g4 r11 register
     */
    virtual void gen_write_r11(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r12 register
     * \param value Object representing the new value of the rh850g4 r12 register
     */
    virtual void gen_write_r12(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r13 register
     * \param value Object representing the new value of the rh850g4 r13 register
     */
    virtual void gen_write_r13(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r14 register
     * \param value Object representing the new value of the rh850g4 r14 register
     */
    virtual void gen_write_r14(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r15 register
     * \param value Object representing the new value of the rh850g4 r15 register
     */
    virtual void gen_write_r15(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r16 register
     * \param value Object representing the new value of the rh850g4 r16 register
     */
    virtual void gen_write_r16(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r17 register
     * \param value Object representing the new value of the rh850g4 r17 register
     */
    virtual void gen_write_r17(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r18 register
     * \param value Object representing the new value of the rh850g4 r18 register
     */
    virtual void gen_write_r18(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r19 register
     * \param value Object representing the new value of the rh850g4 r19 register
     */
    virtual void gen_write_r19(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r20 register
     * \param value Object representing the new value of the rh850g4 r20 register
     */
    virtual void gen_write_r20(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r21 register
     * \param value Object representing the new value of the rh850g4 r21 register
     */
    virtual void gen_write_r21(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r22 register
     * \param value Object representing the new value of the rh850g4 r22 register
     */
    virtual void gen_write_r22(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r23 register
     * \param value Object representing the new value of the rh850g4 r23 register
     */
    virtual void gen_write_r23(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r24 register
     * \param value Object representing the new value of the rh850g4 r24 register
     */
    virtual void gen_write_r24(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r25 register
     * \param value Object representing the new value of the rh850g4 r25 register
     */
    virtual void gen_write_r25(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r26 register
     * \param value Object representing the new value of the rh850g4 r26 register
     */
    virtual void gen_write_r26(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r27 register
     * \param value Object representing the new value of the rh850g4 r27 register
     */
    virtual void gen_write_r27(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r28 register
     * \param value Object representing the new value of the rh850g4 r28 register
     */
    virtual void gen_write_r28(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r29 register
     * \param value Object representing the new value of the rh850g4 r29 register
     */
    virtual void gen_write_r29(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r30 register
     * \param value Object representing the new value of the rh850g4 r30 register
     */
    virtual void gen_write_r30(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 r31 register
     * \param value Object representing the new value of the rh850g4 r31 register
     */
    virtual void gen_write_r31(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PC register
     * \param value Object representing the new value of the rh850g4 PC register
     */
    virtual void gen_write_PC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr0 register
     * \param value Object representing the new value of the rh850g4 wr0 register
     */
    virtual void gen_write_wr0(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr0_u register
     * \param value Object representing the new value of the rh850g4 wr0_u register
     */
    virtual void gen_write_wr0_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr1 register
     * \param value Object representing the new value of the rh850g4 wr1 register
     */
    virtual void gen_write_wr1(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr1_u register
     * \param value Object representing the new value of the rh850g4 wr1_u register
     */
    virtual void gen_write_wr1_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr2 register
     * \param value Object representing the new value of the rh850g4 wr2 register
     */
    virtual void gen_write_wr2(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr2_u register
     * \param value Object representing the new value of the rh850g4 wr2_u register
     */
    virtual void gen_write_wr2_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr3 register
     * \param value Object representing the new value of the rh850g4 wr3 register
     */
    virtual void gen_write_wr3(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr3_u register
     * \param value Object representing the new value of the rh850g4 wr3_u register
     */
    virtual void gen_write_wr3_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr4 register
     * \param value Object representing the new value of the rh850g4 wr4 register
     */
    virtual void gen_write_wr4(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr4_u register
     * \param value Object representing the new value of the rh850g4 wr4_u register
     */
    virtual void gen_write_wr4_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr5 register
     * \param value Object representing the new value of the rh850g4 wr5 register
     */
    virtual void gen_write_wr5(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr5_u register
     * \param value Object representing the new value of the rh850g4 wr5_u register
     */
    virtual void gen_write_wr5_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr6 register
     * \param value Object representing the new value of the rh850g4 wr6 register
     */
    virtual void gen_write_wr6(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr6_u register
     * \param value Object representing the new value of the rh850g4 wr6_u register
     */
    virtual void gen_write_wr6_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr7 register
     * \param value Object representing the new value of the rh850g4 wr7 register
     */
    virtual void gen_write_wr7(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr7_u register
     * \param value Object representing the new value of the rh850g4 wr7_u register
     */
    virtual void gen_write_wr7_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr8 register
     * \param value Object representing the new value of the rh850g4 wr8 register
     */
    virtual void gen_write_wr8(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr8_u register
     * \param value Object representing the new value of the rh850g4 wr8_u register
     */
    virtual void gen_write_wr8_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr9 register
     * \param value Object representing the new value of the rh850g4 wr9 register
     */
    virtual void gen_write_wr9(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr9_u register
     * \param value Object representing the new value of the rh850g4 wr9_u register
     */
    virtual void gen_write_wr9_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr10 register
     * \param value Object representing the new value of the rh850g4 wr10 register
     */
    virtual void gen_write_wr10(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr10_u register
     * \param value Object representing the new value of the rh850g4 wr10_u register
     */
    virtual void gen_write_wr10_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr11 register
     * \param value Object representing the new value of the rh850g4 wr11 register
     */
    virtual void gen_write_wr11(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr11_u register
     * \param value Object representing the new value of the rh850g4 wr11_u register
     */
    virtual void gen_write_wr11_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr12 register
     * \param value Object representing the new value of the rh850g4 wr12 register
     */
    virtual void gen_write_wr12(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr12_u register
     * \param value Object representing the new value of the rh850g4 wr12_u register
     */
    virtual void gen_write_wr12_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr13 register
     * \param value Object representing the new value of the rh850g4 wr13 register
     */
    virtual void gen_write_wr13(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr13_u register
     * \param value Object representing the new value of the rh850g4 wr13_u register
     */
    virtual void gen_write_wr13_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr14 register
     * \param value Object representing the new value of the rh850g4 wr14 register
     */
    virtual void gen_write_wr14(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr14_u register
     * \param value Object representing the new value of the rh850g4 wr14_u register
     */
    virtual void gen_write_wr14_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr15 register
     * \param value Object representing the new value of the rh850g4 wr15 register
     */
    virtual void gen_write_wr15(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr15_u register
     * \param value Object representing the new value of the rh850g4 wr15_u register
     */
    virtual void gen_write_wr15_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr16 register
     * \param value Object representing the new value of the rh850g4 wr16 register
     */
    virtual void gen_write_wr16(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr16_u register
     * \param value Object representing the new value of the rh850g4 wr16_u register
     */
    virtual void gen_write_wr16_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr17 register
     * \param value Object representing the new value of the rh850g4 wr17 register
     */
    virtual void gen_write_wr17(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr17_u register
     * \param value Object representing the new value of the rh850g4 wr17_u register
     */
    virtual void gen_write_wr17_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr18 register
     * \param value Object representing the new value of the rh850g4 wr18 register
     */
    virtual void gen_write_wr18(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr18_u register
     * \param value Object representing the new value of the rh850g4 wr18_u register
     */
    virtual void gen_write_wr18_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr19 register
     * \param value Object representing the new value of the rh850g4 wr19 register
     */
    virtual void gen_write_wr19(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr19_u register
     * \param value Object representing the new value of the rh850g4 wr19_u register
     */
    virtual void gen_write_wr19_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr20 register
     * \param value Object representing the new value of the rh850g4 wr20 register
     */
    virtual void gen_write_wr20(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr20_u register
     * \param value Object representing the new value of the rh850g4 wr20_u register
     */
    virtual void gen_write_wr20_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr21 register
     * \param value Object representing the new value of the rh850g4 wr21 register
     */
    virtual void gen_write_wr21(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr21_u register
     * \param value Object representing the new value of the rh850g4 wr21_u register
     */
    virtual void gen_write_wr21_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr22 register
     * \param value Object representing the new value of the rh850g4 wr22 register
     */
    virtual void gen_write_wr22(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr22_u register
     * \param value Object representing the new value of the rh850g4 wr22_u register
     */
    virtual void gen_write_wr22_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr23 register
     * \param value Object representing the new value of the rh850g4 wr23 register
     */
    virtual void gen_write_wr23(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr23_u register
     * \param value Object representing the new value of the rh850g4 wr23_u register
     */
    virtual void gen_write_wr23_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr24 register
     * \param value Object representing the new value of the rh850g4 wr24 register
     */
    virtual void gen_write_wr24(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr24_u register
     * \param value Object representing the new value of the rh850g4 wr24_u register
     */
    virtual void gen_write_wr24_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr25 register
     * \param value Object representing the new value of the rh850g4 wr25 register
     */
    virtual void gen_write_wr25(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr25_u register
     * \param value Object representing the new value of the rh850g4 wr25_u register
     */
    virtual void gen_write_wr25_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr26 register
     * \param value Object representing the new value of the rh850g4 wr26 register
     */
    virtual void gen_write_wr26(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr26_u register
     * \param value Object representing the new value of the rh850g4 wr26_u register
     */
    virtual void gen_write_wr26_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr27 register
     * \param value Object representing the new value of the rh850g4 wr27 register
     */
    virtual void gen_write_wr27(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr27_u register
     * \param value Object representing the new value of the rh850g4 wr27_u register
     */
    virtual void gen_write_wr27_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr28 register
     * \param value Object representing the new value of the rh850g4 wr28 register
     */
    virtual void gen_write_wr28(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr28_u register
     * \param value Object representing the new value of the rh850g4 wr28_u register
     */
    virtual void gen_write_wr28_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr29 register
     * \param value Object representing the new value of the rh850g4 wr29 register
     */
    virtual void gen_write_wr29(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr29_u register
     * \param value Object representing the new value of the rh850g4 wr29_u register
     */
    virtual void gen_write_wr29_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr30 register
     * \param value Object representing the new value of the rh850g4 wr30 register
     */
    virtual void gen_write_wr30(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr30_u register
     * \param value Object representing the new value of the rh850g4 wr30_u register
     */
    virtual void gen_write_wr30_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr31 register
     * \param value Object representing the new value of the rh850g4 wr31 register
     */
    virtual void gen_write_wr31(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 wr31_u register
     * \param value Object representing the new value of the rh850g4 wr31_u register
     */
    virtual void gen_write_wr31_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EIPC register
     * \param value Object representing the new value of the rh850g4 EIPC register
     */
    virtual void gen_write_EIPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSW register
     * \param value Object representing the new value of the rh850g4 EIPSW register
     */
    virtual void gen_write_EIPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FEPC register
     * \param value Object representing the new value of the rh850g4 FEPC register
     */
    virtual void gen_write_FEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSW register
     * \param value Object representing the new value of the rh850g4 FEPSW register
     */
    virtual void gen_write_FEPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW register
     * \param value Object representing the new value of the rh850g4 PSW register
     */
    virtual void gen_write_PSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_Z register field
     * \param value Object representing the new value of the rh850g4 PSW_Z register field
     */
    virtual void gen_write_PSW_Z(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_S register field
     * \param value Object representing the new value of the rh850g4 PSW_S register field
     */
    virtual void gen_write_PSW_S(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_OV register field
     * \param value Object representing the new value of the rh850g4 PSW_OV register field
     */
    virtual void gen_write_PSW_OV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CY register field
     * \param value Object representing the new value of the rh850g4 PSW_CY register field
     */
    virtual void gen_write_PSW_CY(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_SAT register field
     * \param value Object representing the new value of the rh850g4 PSW_SAT register field
     */
    virtual void gen_write_PSW_SAT(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_ID register field
     * \param value Object representing the new value of the rh850g4 PSW_ID register field
     */
    virtual void gen_write_PSW_ID(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EP register field
     * \param value Object representing the new value of the rh850g4 PSW_EP register field
     */
    virtual void gen_write_PSW_EP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_NP register field
     * \param value Object representing the new value of the rh850g4 PSW_NP register field
     */
    virtual void gen_write_PSW_NP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EBV register field
     * \param value Object representing the new value of the rh850g4 PSW_EBV register field
     */
    virtual void gen_write_PSW_EBV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU0 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU0 register field
     */
    virtual void gen_write_PSW_CU0(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU1 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU1 register field
     */
    virtual void gen_write_PSW_CU1(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU2 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU2 register field
     */
    virtual void gen_write_PSW_CU2(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EIMASK register field
     * \param value Object representing the new value of the rh850g4 PSW_EIMASK register field
     */
    virtual void gen_write_PSW_EIMASK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_UM register field
     * \param value Object representing the new value of the rh850g4 PSW_UM register field
     */
    virtual void gen_write_PSW_UM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR register
     * \param value Object representing the new value of the rh850g4 FPSR register
     */
    virtual void gen_write_FPSR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_CC register field
     * \param value Object representing the new value of the rh850g4 FPSR_CC register field
     */
    virtual void gen_write_FPSR_CC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_FN register field
     * \param value Object representing the new value of the rh850g4 FPSR_FN register field
     */
    virtual void gen_write_FPSR_FN(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_IF register field
     * \param value Object representing the new value of the rh850g4 FPSR_IF register field
     */
    virtual void gen_write_FPSR_IF(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_PEM register field
     * \param value Object representing the new value of the rh850g4 FPSR_PEM register field
     */
    virtual void gen_write_FPSR_PEM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_RM register field
     * \param value Object representing the new value of the rh850g4 FPSR_RM register field
     */
    virtual void gen_write_FPSR_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_FS register field
     * \param value Object representing the new value of the rh850g4 FPSR_FS register field
     */
    virtual void gen_write_FPSR_FS(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XC register field
     * \param value Object representing the new value of the rh850g4 FPSR_XC register field
     */
    virtual void gen_write_FPSR_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XE register field
     * \param value Object representing the new value of the rh850g4 FPSR_XE register field
     */
    virtual void gen_write_FPSR_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XP register field
     * \param value Object representing the new value of the rh850g4 FPSR_XP register field
     */
    virtual void gen_write_FPSR_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPEPC register
     * \param value Object representing the new value of the rh850g4 FPEPC register
     */
    virtual void gen_write_FPEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPST register
     * \param value Object representing the new value of the rh850g4 FPST register
     */
    virtual void gen_write_FPST(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_XC register field
     * \param value Object representing the new value of the rh850g4 FPST_XC register field
     */
    virtual void gen_write_FPST_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_IF register field
     * \param value Object representing the new value of the rh850g4 FPST_IF register field
     */
    virtual void gen_write_FPST_IF(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_XP register field
     * \param value Object representing the new value of the rh850g4 FPST_XP register field
     */
    virtual void gen_write_FPST_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPCC register
     * \param value Object representing the new value of the rh850g4 FPCC register
     */
    virtual void gen_write_FPCC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPCC_CC register field
     * \param value Object representing the new value of the rh850g4 FPCC_CC register field
     */
    virtual void gen_write_FPCC_CC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG register
     * \param value Object representing the new value of the rh850g4 FPCFG register
     */
    virtual void gen_write_FPCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG_RM register field
     * \param value Object representing the new value of the rh850g4 FPCFG_RM register field
     */
    virtual void gen_write_FPCFG_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG_XE register field
     * \param value Object representing the new value of the rh850g4 FPCFG_XE register field
     */
    virtual void gen_write_FPCFG_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EIIC register
     * \param value Object representing the new value of the rh850g4 EIIC register
     */
    virtual void gen_write_EIIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FEIC register
     * \param value Object representing the new value of the rh850g4 FEIC register
     */
    virtual void gen_write_FEIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH register
     * \param value Object representing the new value of the rh850g4 PSWH register
     */
    virtual void gen_write_PSWH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 PSWH_GPID register field
     */
    virtual void gen_write_PSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH_GM register field
     * \param value Object representing the new value of the rh850g4 PSWH_GM register field
     */
    virtual void gen_write_PSWH_GM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 CTPC register
     * \param value Object representing the new value of the rh850g4 CTPC register
     */
    virtual void gen_write_CTPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 CTPSW register
     * \param value Object representing the new value of the rh850g4 CTPSW register
     */
    virtual void gen_write_CTPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH register
     * \param value Object representing the new value of the rh850g4 EIPSWH register
     */
    virtual void gen_write_EIPSWH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void gen_write_EIPSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH_GM register field
     * \param value Object representing the new value of the rh850g4 EIPSWH_GM register field
     */
    virtual void gen_write_EIPSWH_GM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH register
     * \param value Object representing the new value of the rh850g4 FEPSWH register
     */
    virtual void gen_write_FEPSWH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void gen_write_FEPSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH_GM register field
     * \param value Object representing the new value of the rh850g4 FEPSWH_GM register field
     */
    virtual void gen_write_FEPSWH_GM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 CTBP register
     * \param value Object representing the new value of the rh850g4 CTBP register
     */
    virtual void gen_write_CTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 SNZCFG register
     * \param value Object representing the new value of the rh850g4 SNZCFG register
     */
    virtual void gen_write_SNZCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EIWR register
     * \param value Object representing the new value of the rh850g4 EIWR register
     */
    virtual void gen_write_EIWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FEWR register
     * \param value Object representing the new value of the rh850g4 FEWR register
     */
    virtual void gen_write_FEWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 SPID register
     * \param value Object representing the new value of the rh850g4 SPID register
     */
    virtual void gen_write_SPID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 SPIDLIST register
     * \param value Object representing the new value of the rh850g4 SPIDLIST register
     */
    virtual void gen_write_SPIDLIST(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE register
     * \param value Object representing the new value of the rh850g4 RBASE register
     */
    virtual void gen_write_RBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_RBASE register field
     * \param value Object representing the new value of the rh850g4 RBASE_RBASE register field
     */
    virtual void gen_write_RBASE_RBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_DV register field
     * \param value Object representing the new value of the rh850g4 RBASE_DV register field
     */
    virtual void gen_write_RBASE_DV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 RBASE_RINT register field
     */
    virtual void gen_write_RBASE_RINT(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE register
     * \param value Object representing the new value of the rh850g4 EBASE register
     */
    virtual void gen_write_EBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_EBASE register field
     * \param value Object representing the new value of the rh850g4 EBASE_EBASE register field
     */
    virtual void gen_write_EBASE_EBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_DV register field
     * \param value Object representing the new value of the rh850g4 EBASE_DV register field
     */
    virtual void gen_write_EBASE_DV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 EBASE_RINT register field
     */
    virtual void gen_write_EBASE_RINT(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 INTBP register
     * \param value Object representing the new value of the rh850g4 INTBP register
     */
    virtual void gen_write_INTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCTL register
     * \param value Object representing the new value of the rh850g4 MCTL register
     */
    virtual void gen_write_MCTL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCTL_UIC register field
     * \param value Object representing the new value of the rh850g4 MCTL_UIC register field
     */
    virtual void gen_write_MCTL_UIC(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PID register
     * \param value Object representing the new value of the rh850g4 PID register
     */
    virtual void gen_write_PID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 SVLOCK register
     * \param value Object representing the new value of the rh850g4 SVLOCK register
     */
    virtual void gen_write_SVLOCK(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 SCCFG register
     * \param value Object representing the new value of the rh850g4 SCCFG register
     */
    virtual void gen_write_SCCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 SCCFG_SIZE register field
     * \param value Object representing the new value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void gen_write_SCCFG_SIZE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 SCBP register
     * \param value Object representing the new value of the rh850g4 SCBP register
     */
    virtual void gen_write_SCBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 HVCFG register
     * \param value Object representing the new value of the rh850g4 HVCFG register
     */
    virtual void gen_write_HVCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 HVCFG_HVE register field
     * \param value Object representing the new value of the rh850g4 HVCFG_HVE register field
     */
    virtual void gen_write_HVCFG_HVE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG register
     * \param value Object representing the new value of the rh850g4 GMCFG register
     */
    virtual void gen_write_GMCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GMP register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GMP register field
     */
    virtual void gen_write_GMCFG_GMP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_HMP register field
     * \param value Object representing the new value of the rh850g4 GMCFG_HMP register field
     */
    virtual void gen_write_GMCFG_HMP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GSYSE register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GSYSE register field
     */
    virtual void gen_write_GMCFG_GSYSE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU0 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU0 register field
     */
    virtual void gen_write_GMCFG_GCU0(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU1 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU1 register field
     */
    virtual void gen_write_GMCFG_GCU1(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU2 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU2 register field
     */
    virtual void gen_write_GMCFG_GCU2(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 HVSB register
     * \param value Object representing the new value of the rh850g4 HVSB register
     */
    virtual void gen_write_HVSB(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PEID register
     * \param value Object representing the new value of the rh850g4 PEID register
     */
    virtual void gen_write_PEID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BMID register
     * \param value Object representing the new value of the rh850g4 BMID register
     */
    virtual void gen_write_BMID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BMID_BMID register field
     * \param value Object representing the new value of the rh850g4 BMID_BMID register field
     */
    virtual void gen_write_BMID_BMID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MEA register
     * \param value Object representing the new value of the rh850g4 MEA register
     */
    virtual void gen_write_MEA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MEI register
     * \param value Object representing the new value of the rh850g4 MEI register
     */
    virtual void gen_write_MEI(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0 register
     * \param value Object representing the new value of the rh850g4 RBCR0 register
     */
    virtual void gen_write_RBCR0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0_MD register field
     * \param value Object representing the new value of the rh850g4 RBCR0_MD register field
     */
    virtual void gen_write_RBCR0_MD(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0_BE register field
     * \param value Object representing the new value of the rh850g4 RBCR0_BE register field
     */
    virtual void gen_write_RBCR0_BE(fastiss::i16_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR1 register
     * \param value Object representing the new value of the rh850g4 RBCR1 register
     */
    virtual void gen_write_RBCR1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR1_NC register field
     * \param value Object representing the new value of the rh850g4 RBCR1_NC register field
     */
    virtual void gen_write_RBCR1_NC(fastiss::i16_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBNR register
     * \param value Object representing the new value of the rh850g4 RBNR register
     */
    virtual void gen_write_RBNR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBNR_BN register field
     * \param value Object representing the new value of the rh850g4 RBNR_BN register field
     */
    virtual void gen_write_RBNR_BN(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBIP register
     * \param value Object representing the new value of the rh850g4 RBIP register
     */
    virtual void gen_write_RBIP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RBIP_RBIP register field
     * \param value Object representing the new value of the rh850g4 RBIP_RBIP register field
     */
    virtual void gen_write_RBIP_RBIP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ISPR register
     * \param value Object representing the new value of the rh850g4 ISPR register
     */
    virtual void gen_write_ISPR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ISPR_ISP register field
     * \param value Object representing the new value of the rh850g4 ISPR_ISP register field
     */
    virtual void gen_write_ISPR_ISP(fastiss::i16_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR register
     * \param value Object representing the new value of the rh850g4 IMSR register
     */
    virtual void gen_write_IMSR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EEIM register field
     * \param value Object representing the new value of the rh850g4 IMSR_EEIM register field
     */
    virtual void gen_write_IMSR_EEIM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EPLM register field
     * \param value Object representing the new value of the rh850g4 IMSR_EPLM register field
     */
    virtual void gen_write_IMSR_EPLM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EID register field
     * \param value Object representing the new value of the rh850g4 IMSR_EID register field
     */
    virtual void gen_write_IMSR_EID(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_ENP register field
     * \param value Object representing the new value of the rh850g4 IMSR_ENP register field
     */
    virtual void gen_write_IMSR_ENP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_FNP register field
     * \param value Object representing the new value of the rh850g4 IMSR_FNP register field
     */
    virtual void gen_write_IMSR_FNP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEEIM register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEEIM register field
     */
    virtual void gen_write_IMSR_HEEIM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEPLM register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEPLM register field
     */
    virtual void gen_write_IMSR_HEPLM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEID register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEID register field
     */
    virtual void gen_write_IMSR_HEID(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HENP register field
     * \param value Object representing the new value of the rh850g4 IMSR_HENP register field
     */
    virtual void gen_write_IMSR_HENP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HFNP register field
     * \param value Object representing the new value of the rh850g4 IMSR_HFNP register field
     */
    virtual void gen_write_IMSR_HFNP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICSR register
     * \param value Object representing the new value of the rh850g4 ICSR register
     */
    virtual void gen_write_ICSR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICSR_PMEI register field
     * \param value Object representing the new value of the rh850g4 ICSR_PMEI register field
     */
    virtual void gen_write_ICSR_PMEI(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG register
     * \param value Object representing the new value of the rh850g4 INTCFG register
     */
    virtual void gen_write_INTCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_ISPC register field
     * \param value Object representing the new value of the rh850g4 INTCFG_ISPC register field
     */
    virtual void gen_write_INTCFG_ISPC(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_EPL register field
     * \param value Object representing the new value of the rh850g4 INTCFG_EPL register field
     */
    virtual void gen_write_INTCFG_EPL(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_ULNR register field
     * \param value Object representing the new value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void gen_write_INTCFG_ULNR(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PLMR register
     * \param value Object representing the new value of the rh850g4 PLMR register
     */
    virtual void gen_write_PLMR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PLMR_PLM register field
     * \param value Object representing the new value of the rh850g4 PLMR_PLM register field
     */
    virtual void gen_write_PLMR_PLM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR register
     * \param value Object representing the new value of the rh850g4 FXSR register
     */
    virtual void gen_write_FXSR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_FN register field
     * \param value Object representing the new value of the rh850g4 FXSR_FN register field
     */
    virtual void gen_write_FXSR_FN(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_IF register field
     * \param value Object representing the new value of the rh850g4 FXSR_IF register field
     */
    virtual void gen_write_FXSR_IF(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_PEM register field
     * \param value Object representing the new value of the rh850g4 FXSR_PEM register field
     */
    virtual void gen_write_FXSR_PEM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_RM register field
     * \param value Object representing the new value of the rh850g4 FXSR_RM register field
     */
    virtual void gen_write_FXSR_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_FS register field
     * \param value Object representing the new value of the rh850g4 FXSR_FS register field
     */
    virtual void gen_write_FXSR_FS(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XC register field
     * \param value Object representing the new value of the rh850g4 FXSR_XC register field
     */
    virtual void gen_write_FXSR_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XE register field
     * \param value Object representing the new value of the rh850g4 FXSR_XE register field
     */
    virtual void gen_write_FXSR_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XP register field
     * \param value Object representing the new value of the rh850g4 FXSR_XP register field
     */
    virtual void gen_write_FXSR_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXST register
     * \param value Object representing the new value of the rh850g4 FXST register
     */
    virtual void gen_write_FXST(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_XC register field
     * \param value Object representing the new value of the rh850g4 FXST_XC register field
     */
    virtual void gen_write_FXST_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_IF register field
     * \param value Object representing the new value of the rh850g4 FXST_IF register field
     */
    virtual void gen_write_FXST_IF(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_XP register field
     * \param value Object representing the new value of the rh850g4 FXST_XP register field
     */
    virtual void gen_write_FXST_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO register
     * \param value Object representing the new value of the rh850g4 FXINFO register
     */
    virtual void gen_write_FXINFO(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO_NFPU register field
     * \param value Object representing the new value of the rh850g4 FXINFO_NFPU register field
     */
    virtual void gen_write_FXINFO_NFPU(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO_RSIZE register field
     * \param value Object representing the new value of the rh850g4 FXINFO_RSIZE register field
     */
    virtual void gen_write_FXINFO_RSIZE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG register
     * \param value Object representing the new value of the rh850g4 FXCFG register
     */
    virtual void gen_write_FXCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG_RM register field
     * \param value Object representing the new value of the rh850g4 FXCFG_RM register field
     */
    virtual void gen_write_FXCFG_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG_XE register field
     * \param value Object representing the new value of the rh850g4 FXCFG_XE register field
     */
    virtual void gen_write_FXCFG_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC register
     * \param value Object representing the new value of the rh850g4 FXXC register
     */
    virtual void gen_write_FXXC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC3 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC3 register field
     */
    virtual void gen_write_FXXC_XC3(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC2 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC2 register field
     */
    virtual void gen_write_FXXC_XC2(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC1 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC1 register field
     */
    virtual void gen_write_FXXC_XC1(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC0 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC0 register field
     */
    virtual void gen_write_FXXC_XC0(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP register
     * \param value Object representing the new value of the rh850g4 FXXP register
     */
    virtual void gen_write_FXXP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP3 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP3 register field
     */
    virtual void gen_write_FXXP_XP3(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP2 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP2 register field
     */
    virtual void gen_write_FXXP_XP2(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP1 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP1 register field
     */
    virtual void gen_write_FXXP_XP1(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP0 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP0 register field
     */
    virtual void gen_write_FXXP_XP0(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIPC register
     * \param value Object representing the new value of the rh850g4 GMEIPC register
     */
    virtual void gen_write_GMEIPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIPSW register
     * \param value Object representing the new value of the rh850g4 GMEIPSW register
     */
    virtual void gen_write_GMEIPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEPC register
     * \param value Object representing the new value of the rh850g4 GMFEPC register
     */
    virtual void gen_write_GMFEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEPSW register
     * \param value Object representing the new value of the rh850g4 GMFEPSW register
     */
    virtual void gen_write_GMFEPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW register
     * \param value Object representing the new value of the rh850g4 GMPSW register
     */
    virtual void gen_write_GMPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_Z register field
     * \param value Object representing the new value of the rh850g4 GMPSW_Z register field
     */
    virtual void gen_write_GMPSW_Z(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_S register field
     * \param value Object representing the new value of the rh850g4 GMPSW_S register field
     */
    virtual void gen_write_GMPSW_S(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_OV register field
     * \param value Object representing the new value of the rh850g4 GMPSW_OV register field
     */
    virtual void gen_write_GMPSW_OV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CY register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CY register field
     */
    virtual void gen_write_GMPSW_CY(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_SAT register field
     * \param value Object representing the new value of the rh850g4 GMPSW_SAT register field
     */
    virtual void gen_write_GMPSW_SAT(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_ID register field
     * \param value Object representing the new value of the rh850g4 GMPSW_ID register field
     */
    virtual void gen_write_GMPSW_ID(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EP register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EP register field
     */
    virtual void gen_write_GMPSW_EP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_NP register field
     * \param value Object representing the new value of the rh850g4 GMPSW_NP register field
     */
    virtual void gen_write_GMPSW_NP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EBV register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EBV register field
     */
    virtual void gen_write_GMPSW_EBV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU0 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU0 register field
     */
    virtual void gen_write_GMPSW_CU0(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU1 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU1 register field
     */
    virtual void gen_write_GMPSW_CU1(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU2 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU2 register field
     */
    virtual void gen_write_GMPSW_CU2(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EIMASK register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void gen_write_GMPSW_EIMASK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_UM register field
     * \param value Object representing the new value of the rh850g4 GMPSW_UM register field
     */
    virtual void gen_write_GMPSW_UM(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMMEA register
     * \param value Object representing the new value of the rh850g4 GMMEA register
     */
    virtual void gen_write_GMMEA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMMEI register
     * \param value Object representing the new value of the rh850g4 GMMEI register
     */
    virtual void gen_write_GMMEI(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIIC register
     * \param value Object representing the new value of the rh850g4 GMEIIC register
     */
    virtual void gen_write_GMEIIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEIC register
     * \param value Object representing the new value of the rh850g4 GMFEIC register
     */
    virtual void gen_write_GMFEIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMSPID register
     * \param value Object representing the new value of the rh850g4 GMSPID register
     */
    virtual void gen_write_GMSPID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMSPIDLIST register
     * \param value Object representing the new value of the rh850g4 GMSPIDLIST register
     */
    virtual void gen_write_GMSPIDLIST(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE register
     * \param value Object representing the new value of the rh850g4 GMEBASE register
     */
    virtual void gen_write_GMEBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_EBASE register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void gen_write_GMEBASE_EBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_DV register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_DV register field
     */
    virtual void gen_write_GMEBASE_DV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_RINT register field
     */
    virtual void gen_write_GMEBASE_RINT(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTBP register
     * \param value Object representing the new value of the rh850g4 GMINTBP register
     */
    virtual void gen_write_GMINTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG register
     * \param value Object representing the new value of the rh850g4 GMINTCFG register
     */
    virtual void gen_write_GMINTCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_ISPC register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_ISPC register field
     */
    virtual void gen_write_GMINTCFG_ISPC(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_EPL register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_EPL register field
     */
    virtual void gen_write_GMINTCFG_EPL(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_ULNR register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void gen_write_GMINTCFG_ULNR(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPLMR register
     * \param value Object representing the new value of the rh850g4 GMPLMR register
     */
    virtual void gen_write_GMPLMR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPLMR_PLM register field
     * \param value Object representing the new value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void gen_write_GMPLMR_PLM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMSVLOCK register
     * \param value Object representing the new value of the rh850g4 GMSVLOCK register
     */
    virtual void gen_write_GMSVLOCK(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMMPM register
     * \param value Object representing the new value of the rh850g4 GMMPM register
     */
    virtual void gen_write_GMMPM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIWR register
     * \param value Object representing the new value of the rh850g4 GMEIWR register
     */
    virtual void gen_write_GMEIWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEWR register
     * \param value Object representing the new value of the rh850g4 GMFEWR register
     */
    virtual void gen_write_GMFEWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 GMPEID register
     * \param value Object representing the new value of the rh850g4 GMPEID register
     */
    virtual void gen_write_GMPEID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPM register
     * \param value Object representing the new value of the rh850g4 MPM register
     */
    virtual void gen_write_MPM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG register
     * \param value Object representing the new value of the rh850g4 MPCFG register
     */
    virtual void gen_write_MPCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_NMPUE register field
     * \param value Object representing the new value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void gen_write_MPCFG_NMPUE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_HBE register field
     * \param value Object representing the new value of the rh850g4 MPCFG_HBE register field
     */
    virtual void gen_write_MPCFG_HBE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_ARCH register field
     * \param value Object representing the new value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void gen_write_MPCFG_ARCH(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_NBK register field
     * \param value Object representing the new value of the rh850g4 MPCFG_NBK register field
     */
    virtual void gen_write_MPCFG_NBK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_DMDP register field
     * \param value Object representing the new value of the rh850g4 MPCFG_DMDP register field
     */
    virtual void gen_write_MPCFG_DMDP(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCA register
     * \param value Object representing the new value of the rh850g4 MCA register
     */
    virtual void gen_write_MCA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCS register
     * \param value Object representing the new value of the rh850g4 MCS register
     */
    virtual void gen_write_MCS(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCC register
     * \param value Object representing the new value of the rh850g4 MCC register
     */
    virtual void gen_write_MCC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR register
     * \param value Object representing the new value of the rh850g4 MCR register
     */
    virtual void gen_write_MCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSXE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSXE register field
     */
    virtual void gen_write_MCR_HSXE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSWE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSWE register field
     */
    virtual void gen_write_MCR_HSWE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSRE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSRE register field
     */
    virtual void gen_write_MCR_HSRE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HUXE register field
     * \param value Object representing the new value of the rh850g4 MCR_HUXE register field
     */
    virtual void gen_write_MCR_HUXE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HUWE register field
     * \param value Object representing the new value of the rh850g4 MCR_HUWE register field
     */
    virtual void gen_write_MCR_HUWE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HURE register field
     * \param value Object representing the new value of the rh850g4 MCR_HURE register field
     */
    virtual void gen_write_MCR_HURE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSXE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSXE register field
     */
    virtual void gen_write_MCR_GSXE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSWE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSWE register field
     */
    virtual void gen_write_MCR_GSWE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSRE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSRE register field
     */
    virtual void gen_write_MCR_GSRE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GUXE register field
     * \param value Object representing the new value of the rh850g4 MCR_GUXE register field
     */
    virtual void gen_write_MCR_GUXE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GUWE register field
     * \param value Object representing the new value of the rh850g4 MCR_GUWE register field
     */
    virtual void gen_write_MCR_GUWE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GURE register field
     * \param value Object representing the new value of the rh850g4 MCR_GURE register field
     */
    virtual void gen_write_MCR_GURE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_OV register field
     * \param value Object representing the new value of the rh850g4 MCR_OV register field
     */
    virtual void gen_write_MCR_OV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SXE register field
     * \param value Object representing the new value of the rh850g4 MCR_SXE register field
     */
    virtual void gen_write_MCR_SXE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SWE register field
     * \param value Object representing the new value of the rh850g4 MCR_SWE register field
     */
    virtual void gen_write_MCR_SWE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SRE register field
     * \param value Object representing the new value of the rh850g4 MCR_SRE register field
     */
    virtual void gen_write_MCR_SRE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_UXE register field
     * \param value Object representing the new value of the rh850g4 MCR_UXE register field
     */
    virtual void gen_write_MCR_UXE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_UWE register field
     * \param value Object representing the new value of the rh850g4 MCR_UWE register field
     */
    virtual void gen_write_MCR_UWE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_URE register field
     * \param value Object representing the new value of the rh850g4 MCR_URE register field
     */
    virtual void gen_write_MCR_URE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCI register
     * \param value Object representing the new value of the rh850g4 MCI register
     */
    virtual void gen_write_MCI(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MCI_SPID register field
     * \param value Object representing the new value of the rh850g4 MCI_SPID register field
     */
    virtual void gen_write_MCI_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPIDX register
     * \param value Object representing the new value of the rh850g4 MPIDX register
     */
    virtual void gen_write_MPIDX(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPIDX_IDX register field
     * \param value Object representing the new value of the rh850g4 MPIDX_IDX register field
     */
    virtual void gen_write_MPIDX_IDX(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPBK register
     * \param value Object representing the new value of the rh850g4 MPBK register
     */
    virtual void gen_write_MPBK(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPBK_BK register field
     * \param value Object representing the new value of the rh850g4 MPBK_BK register field
     */
    virtual void gen_write_MPBK_BK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPLA register
     * \param value Object representing the new value of the rh850g4 MPLA register
     */
    virtual void gen_write_MPLA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPUA register
     * \param value Object representing the new value of the rh850g4 MPUA register
     */
    virtual void gen_write_MPUA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT register
     * \param value Object representing the new value of the rh850g4 MPAT register
     */
    virtual void gen_write_MPAT(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID7 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID7 register field
     */
    virtual void gen_write_MPAT_WMPID7(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID6 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID6 register field
     */
    virtual void gen_write_MPAT_WMPID6(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID5 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID5 register field
     */
    virtual void gen_write_MPAT_WMPID5(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID4 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID4 register field
     */
    virtual void gen_write_MPAT_WMPID4(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID3 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID3 register field
     */
    virtual void gen_write_MPAT_WMPID3(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID2 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID2 register field
     */
    virtual void gen_write_MPAT_WMPID2(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID1 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID1 register field
     */
    virtual void gen_write_MPAT_WMPID1(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID0 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID0 register field
     */
    virtual void gen_write_MPAT_WMPID0(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID7 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID7 register field
     */
    virtual void gen_write_MPAT_RMPID7(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID6 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID6 register field
     */
    virtual void gen_write_MPAT_RMPID6(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID5 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID5 register field
     */
    virtual void gen_write_MPAT_RMPID5(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID4 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID4 register field
     */
    virtual void gen_write_MPAT_RMPID4(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID3 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID3 register field
     */
    virtual void gen_write_MPAT_RMPID3(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID2 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID2 register field
     */
    virtual void gen_write_MPAT_RMPID2(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID1 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID1 register field
     */
    virtual void gen_write_MPAT_RMPID1(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID0 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID0 register field
     */
    virtual void gen_write_MPAT_RMPID0(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WG register field
     * \param value Object representing the new value of the rh850g4 MPAT_WG register field
     */
    virtual void gen_write_MPAT_WG(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RG register field
     * \param value Object representing the new value of the rh850g4 MPAT_RG register field
     */
    virtual void gen_write_MPAT_RG(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_E register field
     * \param value Object representing the new value of the rh850g4 MPAT_E register field
     */
    virtual void gen_write_MPAT_E(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SX register field
     * \param value Object representing the new value of the rh850g4 MPAT_SX register field
     */
    virtual void gen_write_MPAT_SX(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SW register field
     * \param value Object representing the new value of the rh850g4 MPAT_SW register field
     */
    virtual void gen_write_MPAT_SW(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SR register field
     * \param value Object representing the new value of the rh850g4 MPAT_SR register field
     */
    virtual void gen_write_MPAT_SR(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UX register field
     * \param value Object representing the new value of the rh850g4 MPAT_UX register field
     */
    virtual void gen_write_MPAT_UX(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UW register field
     * \param value Object representing the new value of the rh850g4 MPAT_UW register field
     */
    virtual void gen_write_MPAT_UW(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UR register field
     * \param value Object representing the new value of the rh850g4 MPAT_UR register field
     */
    virtual void gen_write_MPAT_UR(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID0 register
     * \param value Object representing the new value of the rh850g4 MPID0 register
     */
    virtual void gen_write_MPID0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID0_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID0_SPID register field
     */
    virtual void gen_write_MPID0_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID1 register
     * \param value Object representing the new value of the rh850g4 MPID1 register
     */
    virtual void gen_write_MPID1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID1_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID1_SPID register field
     */
    virtual void gen_write_MPID1_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID2 register
     * \param value Object representing the new value of the rh850g4 MPID2 register
     */
    virtual void gen_write_MPID2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID2_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID2_SPID register field
     */
    virtual void gen_write_MPID2_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID3 register
     * \param value Object representing the new value of the rh850g4 MPID3 register
     */
    virtual void gen_write_MPID3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID3_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID3_SPID register field
     */
    virtual void gen_write_MPID3_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID4 register
     * \param value Object representing the new value of the rh850g4 MPID4 register
     */
    virtual void gen_write_MPID4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID4_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID4_SPID register field
     */
    virtual void gen_write_MPID4_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID5 register
     * \param value Object representing the new value of the rh850g4 MPID5 register
     */
    virtual void gen_write_MPID5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID5_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID5_SPID register field
     */
    virtual void gen_write_MPID5_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID6 register
     * \param value Object representing the new value of the rh850g4 MPID6 register
     */
    virtual void gen_write_MPID6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID6_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID6_SPID register field
     */
    virtual void gen_write_MPID6_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID7 register
     * \param value Object representing the new value of the rh850g4 MPID7 register
     */
    virtual void gen_write_MPID7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 MPID7_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID7_SPID register field
     */
    virtual void gen_write_MPID7_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL register
     * \param value Object representing the new value of the rh850g4 ICTAGL register
     */
    virtual void gen_write_ICTAGL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_LPN register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void gen_write_ICTAGL_LPN(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_LRU register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void gen_write_ICTAGL_LRU(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_L register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_L register field
     */
    virtual void gen_write_ICTAGL_L(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_V register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_V register field
     */
    virtual void gen_write_ICTAGL_V(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH register
     * \param value Object representing the new value of the rh850g4 ICTAGH register
     */
    virtual void gen_write_ICTAGH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_WD register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_WD register field
     */
    virtual void gen_write_ICTAGH_WD(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_PD register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_PD register field
     */
    virtual void gen_write_ICTAGH_PD(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_WT register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_WT register field
     */
    virtual void gen_write_ICTAGH_WT(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_PT register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_PT register field
     */
    virtual void gen_write_ICTAGH_PT(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_DATAECC register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void gen_write_ICTAGH_DATAECC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_TAGECC register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void gen_write_ICTAGH_TAGECC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICDATL register
     * \param value Object representing the new value of the rh850g4 ICDATL register
     */
    virtual void gen_write_ICDATL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICDATH register
     * \param value Object representing the new value of the rh850g4 ICDATH register
     */
    virtual void gen_write_ICDATH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL register
     * \param value Object representing the new value of the rh850g4 ICCTRL register
     */
    virtual void gen_write_ICCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_PBS register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_PBS register field
     */
    virtual void gen_write_ICCTRL_PBS(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHCLR register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHCLR register field
     */
    virtual void gen_write_ICCTRL_ICHCLR(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEIV register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEIV register field
     */
    virtual void gen_write_ICCTRL_ICHEIV(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEMK register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEMK register field
     */
    virtual void gen_write_ICCTRL_ICHEMK(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEN register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEN register field
     */
    virtual void gen_write_ICCTRL_ICHEN(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG register
     * \param value Object representing the new value of the rh850g4 ICCFG register
     */
    virtual void gen_write_ICCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHSIZE register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void gen_write_ICCFG_ICHSIZE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHLINE register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void gen_write_ICCFG_ICHLINE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHWAY register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void gen_write_ICCFG_ICHWAY(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR register
     * \param value Object representing the new value of the rh850g4 ICERR register
     */
    virtual void gen_write_ICERR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_CISTW register field
     * \param value Object representing the new value of the rh850g4 ICERR_CISTW register field
     */
    virtual void gen_write_ICERR_CISTW(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESAFE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESAFE register field
     */
    virtual void gen_write_ICERR_ESAFE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESMH register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESMH register field
     */
    virtual void gen_write_ICERR_ESMH(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESPBSE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESPBSE register field
     */
    virtual void gen_write_ICERR_ESPBSE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESTE1 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESTE1 register field
     */
    virtual void gen_write_ICERR_ESTE1(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESTE2 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESTE2 register field
     */
    virtual void gen_write_ICERR_ESTE2(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESDC register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESDC register field
     */
    virtual void gen_write_ICERR_ESDC(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESDE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESDE register field
     */
    virtual void gen_write_ICERR_ESDE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMAFE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMAFE register field
     */
    virtual void gen_write_ICERR_ERMAFE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMMH register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMMH register field
     */
    virtual void gen_write_ICERR_ERMMH(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMPBSE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMPBSE register field
     */
    virtual void gen_write_ICERR_ERMPBSE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMTE1 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMTE1 register field
     */
    virtual void gen_write_ICERR_ERMTE1(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMTE2 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMTE2 register field
     */
    virtual void gen_write_ICERR_ERMTE2(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMDC register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMDC register field
     */
    virtual void gen_write_ICERR_ERMDC(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMDE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMDE register field
     */
    virtual void gen_write_ICERR_ERMDE(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHEWY register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void gen_write_ICERR_ICHEWY(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHEIX register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void gen_write_ICERR_ICHEIX(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHERQ register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHERQ register field
     */
    virtual void gen_write_ICERR_ICHERQ(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHED register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHED register field
     */
    virtual void gen_write_ICERR_ICHED(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHET register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHET register field
     */
    virtual void gen_write_ICERR_ICHET(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHERR register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHERR register field
     */
    virtual void gen_write_ICERR_ICHERR(fastiss::i1_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 TSCOUNTL register
     * \param value Object representing the new value of the rh850g4 TSCOUNTL register
     */
    virtual void gen_write_TSCOUNTL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 TSCOUNTH register
     * \param value Object representing the new value of the rh850g4 TSCOUNTH register
     */
    virtual void gen_write_TSCOUNTH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 TSCTRL register
     * \param value Object representing the new value of the rh850g4 TSCTRL register
     */
    virtual void gen_write_TSCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMUMCTRL register
     * \param value Object representing the new value of the rh850g4 PMUMCTRL register
     */
    virtual void gen_write_PMUMCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMGMCTRL register
     * \param value Object representing the new value of the rh850g4 PMGMCTRL register
     */
    virtual void gen_write_PMGMCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT0 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT0 register
     */
    virtual void gen_write_PMCOUNT0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL0 register
     * \param value Object representing the new value of the rh850g4 PMCTRL0 register
     */
    virtual void gen_write_PMCTRL0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT1 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT1 register
     */
    virtual void gen_write_PMCOUNT1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL1 register
     * \param value Object representing the new value of the rh850g4 PMCTRL1 register
     */
    virtual void gen_write_PMCTRL1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT2 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT2 register
     */
    virtual void gen_write_PMCOUNT2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL2 register
     * \param value Object representing the new value of the rh850g4 PMCTRL2 register
     */
    virtual void gen_write_PMCTRL2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT3 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT3 register
     */
    virtual void gen_write_PMCOUNT3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL3 register
     * \param value Object representing the new value of the rh850g4 PMCTRL3 register
     */
    virtual void gen_write_PMCTRL3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL4 register
     * \param value Object representing the new value of the rh850g4 PMCTRL4 register
     */
    virtual void gen_write_PMCTRL4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL5 register
     * \param value Object representing the new value of the rh850g4 PMCTRL5 register
     */
    virtual void gen_write_PMCTRL5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL6 register
     * \param value Object representing the new value of the rh850g4 PMCTRL6 register
     */
    virtual void gen_write_PMCTRL6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL7 register
     * \param value Object representing the new value of the rh850g4 PMCTRL7 register
     */
    virtual void gen_write_PMCTRL7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT4 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT4 register
     */
    virtual void gen_write_PMCOUNT4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT5 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT5 register
     */
    virtual void gen_write_PMCOUNT5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT6 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT6 register
     */
    virtual void gen_write_PMCOUNT6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT7 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT7 register
     */
    virtual void gen_write_PMCOUNT7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND0 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND0 register
     */
    virtual void gen_write_PMSUBCND0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND1 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND1 register
     */
    virtual void gen_write_PMSUBCND1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND2 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND2 register
     */
    virtual void gen_write_PMSUBCND2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND3 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND3 register
     */
    virtual void gen_write_PMSUBCND3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND4 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND4 register
     */
    virtual void gen_write_PMSUBCND4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND5 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND5 register
     */
    virtual void gen_write_PMSUBCND5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND6 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND6 register
     */
    virtual void gen_write_PMSUBCND6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND7 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND7 register
     */
    virtual void gen_write_PMSUBCND7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DBGEN register
     * \param value Object representing the new value of the rh850g4 DBGEN register
     */
    virtual void gen_write_DBGEN(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DBPSWH register
     * \param value Object representing the new value of the rh850g4 DBPSWH register
     */
    virtual void gen_write_DBPSWH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DBIC register
     * \param value Object representing the new value of the rh850g4 DBIC register
     */
    virtual void gen_write_DBIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DBPC register
     * \param value Object representing the new value of the rh850g4 DBPC register
     */
    virtual void gen_write_DBPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DBPSW register
     * \param value Object representing the new value of the rh850g4 DBPSW register
     */
    virtual void gen_write_DBPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DIR0 register
     * \param value Object representing the new value of the rh850g4 DIR0 register
     */
    virtual void gen_write_DIR0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DIR1 register
     * \param value Object representing the new value of the rh850g4 DIR1 register
     */
    virtual void gen_write_DIR1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BPC register
     * \param value Object representing the new value of the rh850g4 BPC register
     */
    virtual void gen_write_BPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BPAV register
     * \param value Object representing the new value of the rh850g4 BPAV register
     */
    virtual void gen_write_BPAV(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BPAM register
     * \param value Object representing the new value of the rh850g4 BPAM register
     */
    virtual void gen_write_BPAM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DBCFG register
     * \param value Object representing the new value of the rh850g4 DBCFG register
     */
    virtual void gen_write_DBCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DBWR register
     * \param value Object representing the new value of the rh850g4 DBWR register
     */
    virtual void gen_write_DBWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 LSTEST0 register
     * \param value Object representing the new value of the rh850g4 LSTEST0 register
     */
    virtual void gen_write_LSTEST0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 LSTEST1 register
     * \param value Object representing the new value of the rh850g4 LSTEST1 register
     */
    virtual void gen_write_LSTEST1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 LSCFG register
     * \param value Object representing the new value of the rh850g4 LSCFG register
     */
    virtual void gen_write_LSCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 ICBKEY register
     * \param value Object representing the new value of the rh850g4 ICBKEY register
     */
    virtual void gen_write_ICBKEY(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IFCR register
     * \param value Object representing the new value of the rh850g4 IFCR register
     */
    virtual void gen_write_IFCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 IFCR1 register
     * \param value Object representing the new value of the rh850g4 IFCR1 register
     */
    virtual void gen_write_IFCR1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCTRL0 register
     * \param value Object representing the new value of the rh850g4 BRPCTRL0 register
     */
    virtual void gen_write_BRPCTRL0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCTRL1 register
     * \param value Object representing the new value of the rh850g4 BRPCTRL1 register
     */
    virtual void gen_write_BRPCTRL1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCFG register
     * \param value Object representing the new value of the rh850g4 BRPCFG register
     */
    virtual void gen_write_BRPCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BRPACTRL register
     * \param value Object representing the new value of the rh850g4 BRPACTRL register
     */
    virtual void gen_write_BRPACTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 BRPDATA register
     * \param value Object representing the new value of the rh850g4 BRPDATA register
     */
    virtual void gen_write_BRPDATA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DCBKEY register
     * \param value Object representing the new value of the rh850g4 DCBKEY register
     */
    virtual void gen_write_DCBKEY(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 LSUCR register
     * \param value Object representing the new value of the rh850g4 LSUCR register
     */
    virtual void gen_write_LSUCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 LSULNK0 register
     * \param value Object representing the new value of the rh850g4 LSULNK0 register
     */
    virtual void gen_write_LSULNK0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 LSULNK1 register
     * \param value Object representing the new value of the rh850g4 LSULNK1 register
     */
    virtual void gen_write_LSULNK1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLCR register
     * \param value Object representing the new value of the rh850g4 L1RLCR register
     */
    virtual void gen_write_L1RLCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLNK0 register
     * \param value Object representing the new value of the rh850g4 L1RLNK0 register
     */
    virtual void gen_write_L1RLNK0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLNK1 register
     * \param value Object representing the new value of the rh850g4 L1RLNK1 register
     */
    virtual void gen_write_L1RLNK1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 L1RCFG register
     * \param value Object representing the new value of the rh850g4 L1RCFG register
     */
    virtual void gen_write_L1RCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DECFG register
     * \param value Object representing the new value of the rh850g4 DECFG register
     */
    virtual void gen_write_DECFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DECTRL register
     * \param value Object representing the new value of the rh850g4 DECTRL register
     */
    virtual void gen_write_DECTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 DEVDS register
     * \param value Object representing the new value of the rh850g4 DEVDS register
     */
    virtual void gen_write_DEVDS(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBCR register
     * \param value Object representing the new value of the rh850g4 RDBCR register
     */
    virtual void gen_write_RDBCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBACR register
     * \param value Object representing the new value of the rh850g4 RDBACR register
     */
    virtual void gen_write_RDBACR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBATAG register
     * \param value Object representing the new value of the rh850g4 RDBATAG register
     */
    virtual void gen_write_RDBATAG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT0 register
     * \param value Object representing the new value of the rh850g4 RDBADAT0 register
     */
    virtual void gen_write_RDBADAT0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT1 register
     * \param value Object representing the new value of the rh850g4 RDBADAT1 register
     */
    virtual void gen_write_RDBADAT1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT2 register
     * \param value Object representing the new value of the rh850g4 RDBADAT2 register
     */
    virtual void gen_write_RDBADAT2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT3 register
     * \param value Object representing the new value of the rh850g4 RDBADAT3 register
     */
    virtual void gen_write_RDBADAT3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 RDBSTAT register
     * \param value Object representing the new value of the rh850g4 RDBSTAT register
     */
    virtual void gen_write_RDBSTAT(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 hv_func_support register
     * \param value Object representing the new value of the rh850g4 hv_func_support register
     */
    virtual void gen_write_hv_func_support(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 swd0pcad_exec_count register
     * \param value Object representing the new value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void gen_write_swd0pcad_exec_count(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 swd0pcad_val register
     * \param value Object representing the new value of the rh850g4 swd0pcad_val register
     */
    virtual void gen_write_swd0pcad_val(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the rh850g4 mem_acc_type register
     * \param value Object representing the new value of the rh850g4 mem_acc_type register
     */
    virtual void gen_write_mem_acc_type(fastiss::i32_t *value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeRegisterGroupWriteOperations
     * @{
     */
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the new value of the specified rh850g4 GR group register
     */
    virtual void gen_write_GR(rh850g4::GR_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the new value of the specified rh850g4 WR group register
     */
    virtual void gen_write_WR(rh850g4::WR_index_t index, fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void gen_write_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void gen_write_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void gen_write_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void gen_write_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void gen_write_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void gen_write_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void gen_write_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void gen_write_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void gen_write_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void gen_write_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void gen_write_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void gen_write_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void gen_write_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeIndividualRegisterIncrementOperations
     * @{
     */
    /**
     * \brief Generates code to increment the value of the rh850g4 r0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r0 register
     */
    virtual void gen_increment_r0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r1 register
     */
    virtual void gen_increment_r1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r2 register
     */
    virtual void gen_increment_r2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r3 register
     */
    virtual void gen_increment_r3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r4 register
     */
    virtual void gen_increment_r4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r5 register
     */
    virtual void gen_increment_r5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r6 register
     */
    virtual void gen_increment_r6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r7 register
     */
    virtual void gen_increment_r7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r8 register
     */
    virtual void gen_increment_r8(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r9 register
     */
    virtual void gen_increment_r9(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r10 register
     */
    virtual void gen_increment_r10(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r11 register
     */
    virtual void gen_increment_r11(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r12 register
     */
    virtual void gen_increment_r12(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r13 register
     */
    virtual void gen_increment_r13(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r14 register
     */
    virtual void gen_increment_r14(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r15 register
     */
    virtual void gen_increment_r15(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r16 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r16 register
     */
    virtual void gen_increment_r16(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r17 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r17 register
     */
    virtual void gen_increment_r17(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r18 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r18 register
     */
    virtual void gen_increment_r18(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r19 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r19 register
     */
    virtual void gen_increment_r19(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r20 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r20 register
     */
    virtual void gen_increment_r20(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r21 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r21 register
     */
    virtual void gen_increment_r21(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r22 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r22 register
     */
    virtual void gen_increment_r22(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r23 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r23 register
     */
    virtual void gen_increment_r23(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r24 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r24 register
     */
    virtual void gen_increment_r24(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r25 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r25 register
     */
    virtual void gen_increment_r25(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r26 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r26 register
     */
    virtual void gen_increment_r26(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r27 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r27 register
     */
    virtual void gen_increment_r27(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r28 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r28 register
     */
    virtual void gen_increment_r28(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r29 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r29 register
     */
    virtual void gen_increment_r29(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r30 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r30 register
     */
    virtual void gen_increment_r30(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 r31 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r31 register
     */
    virtual void gen_increment_r31(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PC register
     */
    virtual void gen_increment_PC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr0 register
     */
    virtual void gen_increment_wr0(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr0_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr0_u register
     */
    virtual void gen_increment_wr0_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr1 register
     */
    virtual void gen_increment_wr1(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr1_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr1_u register
     */
    virtual void gen_increment_wr1_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr2 register
     */
    virtual void gen_increment_wr2(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr2_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr2_u register
     */
    virtual void gen_increment_wr2_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr3 register
     */
    virtual void gen_increment_wr3(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr3_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr3_u register
     */
    virtual void gen_increment_wr3_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr4 register
     */
    virtual void gen_increment_wr4(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr4_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr4_u register
     */
    virtual void gen_increment_wr4_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr5 register
     */
    virtual void gen_increment_wr5(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr5_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr5_u register
     */
    virtual void gen_increment_wr5_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr6 register
     */
    virtual void gen_increment_wr6(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr6_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr6_u register
     */
    virtual void gen_increment_wr6_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr7 register
     */
    virtual void gen_increment_wr7(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr7_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr7_u register
     */
    virtual void gen_increment_wr7_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr8 register
     */
    virtual void gen_increment_wr8(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr8_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr8_u register
     */
    virtual void gen_increment_wr8_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr9 register
     */
    virtual void gen_increment_wr9(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr9_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr9_u register
     */
    virtual void gen_increment_wr9_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr10 register
     */
    virtual void gen_increment_wr10(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr10_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr10_u register
     */
    virtual void gen_increment_wr10_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr11 register
     */
    virtual void gen_increment_wr11(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr11_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr11_u register
     */
    virtual void gen_increment_wr11_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr12 register
     */
    virtual void gen_increment_wr12(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr12_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr12_u register
     */
    virtual void gen_increment_wr12_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr13 register
     */
    virtual void gen_increment_wr13(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr13_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr13_u register
     */
    virtual void gen_increment_wr13_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr14 register
     */
    virtual void gen_increment_wr14(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr14_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr14_u register
     */
    virtual void gen_increment_wr14_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr15 register
     */
    virtual void gen_increment_wr15(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr15_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr15_u register
     */
    virtual void gen_increment_wr15_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr16 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr16 register
     */
    virtual void gen_increment_wr16(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr16_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr16_u register
     */
    virtual void gen_increment_wr16_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr17 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr17 register
     */
    virtual void gen_increment_wr17(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr17_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr17_u register
     */
    virtual void gen_increment_wr17_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr18 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr18 register
     */
    virtual void gen_increment_wr18(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr18_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr18_u register
     */
    virtual void gen_increment_wr18_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr19 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr19 register
     */
    virtual void gen_increment_wr19(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr19_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr19_u register
     */
    virtual void gen_increment_wr19_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr20 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr20 register
     */
    virtual void gen_increment_wr20(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr20_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr20_u register
     */
    virtual void gen_increment_wr20_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr21 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr21 register
     */
    virtual void gen_increment_wr21(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr21_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr21_u register
     */
    virtual void gen_increment_wr21_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr22 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr22 register
     */
    virtual void gen_increment_wr22(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr22_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr22_u register
     */
    virtual void gen_increment_wr22_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr23 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr23 register
     */
    virtual void gen_increment_wr23(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr23_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr23_u register
     */
    virtual void gen_increment_wr23_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr24 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr24 register
     */
    virtual void gen_increment_wr24(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr24_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr24_u register
     */
    virtual void gen_increment_wr24_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr25 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr25 register
     */
    virtual void gen_increment_wr25(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr25_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr25_u register
     */
    virtual void gen_increment_wr25_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr26 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr26 register
     */
    virtual void gen_increment_wr26(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr26_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr26_u register
     */
    virtual void gen_increment_wr26_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr27 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr27 register
     */
    virtual void gen_increment_wr27(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr27_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr27_u register
     */
    virtual void gen_increment_wr27_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr28 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr28 register
     */
    virtual void gen_increment_wr28(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr28_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr28_u register
     */
    virtual void gen_increment_wr28_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr29 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr29 register
     */
    virtual void gen_increment_wr29(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr29_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr29_u register
     */
    virtual void gen_increment_wr29_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr30 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr30 register
     */
    virtual void gen_increment_wr30(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr30_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr30_u register
     */
    virtual void gen_increment_wr30_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr31 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr31 register
     */
    virtual void gen_increment_wr31(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 wr31_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr31_u register
     */
    virtual void gen_increment_wr31_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 EIPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIPC register
     */
    virtual void gen_increment_EIPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 EIPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIPSW register
     */
    virtual void gen_increment_EIPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 FEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEPC register
     */
    virtual void gen_increment_FEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 FEPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEPSW register
     */
    virtual void gen_increment_FEPSW(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 PSW_EIMASK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PSW_EIMASK register field
    */
    virtual void gen_increment_PSW_EIMASK(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_CC register field
    */
    virtual void gen_increment_FPSR_CC(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_RM register field
    */
    virtual void gen_increment_FPSR_RM(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XC register field
    */
    virtual void gen_increment_FPSR_XC(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XE register field
    */
    virtual void gen_increment_FPSR_XE(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XP register field
    */
    virtual void gen_increment_FPSR_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 FPEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FPEPC register
     */
    virtual void gen_increment_FPEPC(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPST_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPST_XC register field
    */
    virtual void gen_increment_FPST_XC(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPST_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPST_XP register field
    */
    virtual void gen_increment_FPST_XP(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCC_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCC_CC register field
    */
    virtual void gen_increment_FPCC_CC(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCFG_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCFG_RM register field
    */
    virtual void gen_increment_FPCFG_RM(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCFG_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCFG_XE register field
    */
    virtual void gen_increment_FPCFG_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 EIIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIIC register
     */
    virtual void gen_increment_EIIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 FEIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEIC register
     */
    virtual void gen_increment_FEIC(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 PSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PSWH_GPID register field
    */
    virtual void gen_increment_PSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 CTPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTPC register
     */
    virtual void gen_increment_CTPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 CTPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTPSW register
     */
    virtual void gen_increment_CTPSW(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 EIPSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 EIPSWH_GPID register field
    */
    virtual void gen_increment_EIPSWH_GPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FEPSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FEPSWH_GPID register field
    */
    virtual void gen_increment_FEPSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 CTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTBP register
     */
    virtual void gen_increment_CTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SNZCFG register
     */
    virtual void gen_increment_SNZCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 EIWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIWR register
     */
    virtual void gen_increment_EIWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 FEWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEWR register
     */
    virtual void gen_increment_FEWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 SPID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SPID register
     */
    virtual void gen_increment_SPID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SPIDLIST register
     */
    virtual void gen_increment_SPIDLIST(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 RBASE_RBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBASE_RBASE register field
    */
    virtual void gen_increment_RBASE_RBASE(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 EBASE_EBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 EBASE_EBASE register field
    */
    virtual void gen_increment_EBASE_EBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 INTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 INTBP register
     */
    virtual void gen_increment_INTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PID register
     */
    virtual void gen_increment_PID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SVLOCK register
     */
    virtual void gen_increment_SVLOCK(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 SCCFG_SIZE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 SCCFG_SIZE register field
    */
    virtual void gen_increment_SCCFG_SIZE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 SCBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SCBP register
     */
    virtual void gen_increment_SCBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 HVSB register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 HVSB register
     */
    virtual void gen_increment_HVSB(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PEID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PEID register
     */
    virtual void gen_increment_PEID(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 BMID_BMID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 BMID_BMID register field
    */
    virtual void gen_increment_BMID_BMID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MEA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MEA register
     */
    virtual void gen_increment_MEA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MEI register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MEI register
     */
    virtual void gen_increment_MEI(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 RBCR0_BE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBCR0_BE register field
    */
    virtual void gen_increment_RBCR0_BE(fastiss::i16_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 RBCR1_NC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBCR1_NC register field
    */
    virtual void gen_increment_RBCR1_NC(fastiss::i16_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 RBNR_BN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBNR_BN register field
    */
    virtual void gen_increment_RBNR_BN(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 RBIP_RBIP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBIP_RBIP register field
    */
    virtual void gen_increment_RBIP_RBIP(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ISPR_ISP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ISPR_ISP register field
    */
    virtual void gen_increment_ISPR_ISP(fastiss::i16_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 INTCFG_ULNR register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 INTCFG_ULNR register field
    */
    virtual void gen_increment_INTCFG_ULNR(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 PLMR_PLM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PLMR_PLM register field
    */
    virtual void gen_increment_PLMR_PLM(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_RM register field
    */
    virtual void gen_increment_FXSR_RM(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XC register field
    */
    virtual void gen_increment_FXSR_XC(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XE register field
    */
    virtual void gen_increment_FXSR_XE(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XP register field
    */
    virtual void gen_increment_FXSR_XP(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXST_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXST_XC register field
    */
    virtual void gen_increment_FXST_XC(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXST_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXST_XP register field
    */
    virtual void gen_increment_FXST_XP(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXCFG_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXCFG_RM register field
    */
    virtual void gen_increment_FXCFG_RM(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXCFG_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXCFG_XE register field
    */
    virtual void gen_increment_FXCFG_XE(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC3 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC3 register field
    */
    virtual void gen_increment_FXXC_XC3(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC2 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC2 register field
    */
    virtual void gen_increment_FXXC_XC2(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC1 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC1 register field
    */
    virtual void gen_increment_FXXC_XC1(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC0 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC0 register field
    */
    virtual void gen_increment_FXXC_XC0(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP3 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP3 register field
    */
    virtual void gen_increment_FXXP_XP3(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP2 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP2 register field
    */
    virtual void gen_increment_FXXP_XP2(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP1 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP1 register field
    */
    virtual void gen_increment_FXXP_XP1(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP0 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP0 register field
    */
    virtual void gen_increment_FXXP_XP0(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIPC register
     */
    virtual void gen_increment_GMEIPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIPSW register
     */
    virtual void gen_increment_GMEIPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEPC register
     */
    virtual void gen_increment_GMFEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEPSW register
     */
    virtual void gen_increment_GMFEPSW(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMPSW_EIMASK register field
    */
    virtual void gen_increment_GMPSW_EIMASK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMEA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMEA register
     */
    virtual void gen_increment_GMMEA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMEI register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMEI register
     */
    virtual void gen_increment_GMMEI(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIIC register
     */
    virtual void gen_increment_GMEIIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEIC register
     */
    virtual void gen_increment_GMFEIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSPID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSPID register
     */
    virtual void gen_increment_GMSPID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSPIDLIST register
     */
    virtual void gen_increment_GMSPIDLIST(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMEBASE_EBASE register field
    */
    virtual void gen_increment_GMEBASE_EBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMINTBP register
     */
    virtual void gen_increment_GMINTBP(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMINTCFG_ULNR register field
    */
    virtual void gen_increment_GMINTCFG_ULNR(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 GMPLMR_PLM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMPLMR_PLM register field
    */
    virtual void gen_increment_GMPLMR_PLM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSVLOCK register
     */
    virtual void gen_increment_GMSVLOCK(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMPM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMPM register
     */
    virtual void gen_increment_GMMPM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIWR register
     */
    virtual void gen_increment_GMEIWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEWR register
     */
    virtual void gen_increment_GMFEWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 GMPEID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMPEID register
     */
    virtual void gen_increment_GMPEID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MPM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPM register
     */
    virtual void gen_increment_MPM(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_NMPUE register field
    */
    virtual void gen_increment_MPCFG_NMPUE(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_HBE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_HBE register field
    */
    virtual void gen_increment_MPCFG_HBE(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_ARCH register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_ARCH register field
    */
    virtual void gen_increment_MPCFG_ARCH(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_NBK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_NBK register field
    */
    virtual void gen_increment_MPCFG_NBK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MCA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCA register
     */
    virtual void gen_increment_MCA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MCS register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCS register
     */
    virtual void gen_increment_MCS(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MCC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCC register
     */
    virtual void gen_increment_MCC(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MCI_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MCI_SPID register field
    */
    virtual void gen_increment_MCI_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPIDX_IDX register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPIDX_IDX register field
    */
    virtual void gen_increment_MPIDX_IDX(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPBK_BK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPBK_BK register field
    */
    virtual void gen_increment_MPBK_BK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MPLA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPLA register
     */
    virtual void gen_increment_MPLA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 MPUA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPUA register
     */
    virtual void gen_increment_MPUA(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID0_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID0_SPID register field
    */
    virtual void gen_increment_MPID0_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID1_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID1_SPID register field
    */
    virtual void gen_increment_MPID1_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID2_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID2_SPID register field
    */
    virtual void gen_increment_MPID2_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID3_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID3_SPID register field
    */
    virtual void gen_increment_MPID3_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID4_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID4_SPID register field
    */
    virtual void gen_increment_MPID4_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID5_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID5_SPID register field
    */
    virtual void gen_increment_MPID5_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID6_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID6_SPID register field
    */
    virtual void gen_increment_MPID6_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID7_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID7_SPID register field
    */
    virtual void gen_increment_MPID7_SPID(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGL_LPN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGL_LPN register field
    */
    virtual void gen_increment_ICTAGL_LPN(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGL_LRU register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGL_LRU register field
    */
    virtual void gen_increment_ICTAGL_LRU(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGH_DATAECC register field
    */
    virtual void gen_increment_ICTAGH_DATAECC(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGH_TAGECC register field
    */
    virtual void gen_increment_ICTAGH_TAGECC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 ICDATL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICDATL register
     */
    virtual void gen_increment_ICDATL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 ICDATH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICDATH register
     */
    virtual void gen_increment_ICDATH(fastiss::i32_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHSIZE register field
    */
    virtual void gen_increment_ICCFG_ICHSIZE(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHLINE register field
    */
    virtual void gen_increment_ICCFG_ICHLINE(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHWAY register field
    */
    virtual void gen_increment_ICCFG_ICHWAY(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICERR_ICHEWY register field
    */
    virtual void gen_increment_ICERR_ICHEWY(fastiss::i8_t *value) = 0;
    /**
    * \brief Generates code to increment the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICERR_ICHEIX register field
    */
    virtual void gen_increment_ICERR_ICHEIX(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCOUNTL register
     */
    virtual void gen_increment_TSCOUNTL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCOUNTH register
     */
    virtual void gen_increment_TSCOUNTH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCTRL register
     */
    virtual void gen_increment_TSCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMUMCTRL register
     */
    virtual void gen_increment_PMUMCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMGMCTRL register
     */
    virtual void gen_increment_PMGMCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT0 register
     */
    virtual void gen_increment_PMCOUNT0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL0 register
     */
    virtual void gen_increment_PMCTRL0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT1 register
     */
    virtual void gen_increment_PMCOUNT1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL1 register
     */
    virtual void gen_increment_PMCTRL1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT2 register
     */
    virtual void gen_increment_PMCOUNT2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL2 register
     */
    virtual void gen_increment_PMCTRL2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT3 register
     */
    virtual void gen_increment_PMCOUNT3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL3 register
     */
    virtual void gen_increment_PMCTRL3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL4 register
     */
    virtual void gen_increment_PMCTRL4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL5 register
     */
    virtual void gen_increment_PMCTRL5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL6 register
     */
    virtual void gen_increment_PMCTRL6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL7 register
     */
    virtual void gen_increment_PMCTRL7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT4 register
     */
    virtual void gen_increment_PMCOUNT4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT5 register
     */
    virtual void gen_increment_PMCOUNT5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT6 register
     */
    virtual void gen_increment_PMCOUNT6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT7 register
     */
    virtual void gen_increment_PMCOUNT7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND0 register
     */
    virtual void gen_increment_PMSUBCND0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND1 register
     */
    virtual void gen_increment_PMSUBCND1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND2 register
     */
    virtual void gen_increment_PMSUBCND2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND3 register
     */
    virtual void gen_increment_PMSUBCND3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND4 register
     */
    virtual void gen_increment_PMSUBCND4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND5 register
     */
    virtual void gen_increment_PMSUBCND5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND6 register
     */
    virtual void gen_increment_PMSUBCND6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND7 register
     */
    virtual void gen_increment_PMSUBCND7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DBGEN register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBGEN register
     */
    virtual void gen_increment_DBGEN(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPSWH register
     */
    virtual void gen_increment_DBPSWH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DBIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBIC register
     */
    virtual void gen_increment_DBIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPC register
     */
    virtual void gen_increment_DBPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPSW register
     */
    virtual void gen_increment_DBPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DIR0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DIR0 register
     */
    virtual void gen_increment_DIR0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DIR1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DIR1 register
     */
    virtual void gen_increment_DIR1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPC register
     */
    virtual void gen_increment_BPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BPAV register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPAV register
     */
    virtual void gen_increment_BPAV(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BPAM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPAM register
     */
    virtual void gen_increment_BPAM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DBCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBCFG register
     */
    virtual void gen_increment_DBCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DBWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBWR register
     */
    virtual void gen_increment_DBWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSTEST0 register
     */
    virtual void gen_increment_LSTEST0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSTEST1 register
     */
    virtual void gen_increment_LSTEST1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 LSCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSCFG register
     */
    virtual void gen_increment_LSCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICBKEY register
     */
    virtual void gen_increment_ICBKEY(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 IFCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 IFCR register
     */
    virtual void gen_increment_IFCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 IFCR1 register
     */
    virtual void gen_increment_IFCR1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCTRL0 register
     */
    virtual void gen_increment_BRPCTRL0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCTRL1 register
     */
    virtual void gen_increment_BRPCTRL1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCFG register
     */
    virtual void gen_increment_BRPCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPACTRL register
     */
    virtual void gen_increment_BRPACTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPDATA register
     */
    virtual void gen_increment_BRPDATA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DCBKEY register
     */
    virtual void gen_increment_DCBKEY(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 LSUCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSUCR register
     */
    virtual void gen_increment_LSUCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSULNK0 register
     */
    virtual void gen_increment_LSULNK0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSULNK1 register
     */
    virtual void gen_increment_LSULNK1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLCR register
     */
    virtual void gen_increment_L1RLCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLNK0 register
     */
    virtual void gen_increment_L1RLNK0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLNK1 register
     */
    virtual void gen_increment_L1RLNK1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RCFG register
     */
    virtual void gen_increment_L1RCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DECFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DECFG register
     */
    virtual void gen_increment_DECFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DECTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DECTRL register
     */
    virtual void gen_increment_DECTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 DEVDS register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DEVDS register
     */
    virtual void gen_increment_DEVDS(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBCR register
     */
    virtual void gen_increment_RDBCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBACR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBACR register
     */
    virtual void gen_increment_RDBACR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBATAG register
     */
    virtual void gen_increment_RDBATAG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT0 register
     */
    virtual void gen_increment_RDBADAT0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT1 register
     */
    virtual void gen_increment_RDBADAT1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT2 register
     */
    virtual void gen_increment_RDBADAT2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT3 register
     */
    virtual void gen_increment_RDBADAT3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBSTAT register
     */
    virtual void gen_increment_RDBSTAT(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 hv_func_support register
     */
    virtual void gen_increment_hv_func_support(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void gen_increment_swd0pcad_exec_count(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 swd0pcad_val register
     */
    virtual void gen_increment_swd0pcad_val(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 mem_acc_type register
     */
    virtual void gen_increment_mem_acc_type(fastiss::i32_t *value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeRegisterGroupIncrementOperations
     * @{
     */
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 GR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 GR group register
     */
    virtual void gen_increment_GR(rh850g4::GR_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 WR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 WR group register
     */
    virtual void gen_increment_WR(rh850g4::WR_index_t index, fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void gen_increment_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void gen_increment_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void gen_increment_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void gen_increment_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_4 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void gen_increment_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void gen_increment_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_9 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void gen_increment_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_10 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void gen_increment_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_11 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void gen_increment_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_12 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void gen_increment_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_13 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void gen_increment_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_14 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void gen_increment_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_15 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void gen_increment_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeIndividualRegisterDecrementOperations
     * @{
     */
    /**
     * \brief Generates code to decrement the value of the rh850g4 r0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r0 register
     */
    virtual void gen_decrement_r0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r1 register
     */
    virtual void gen_decrement_r1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r2 register
     */
    virtual void gen_decrement_r2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r3 register
     */
    virtual void gen_decrement_r3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r4 register
     */
    virtual void gen_decrement_r4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r5 register
     */
    virtual void gen_decrement_r5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r6 register
     */
    virtual void gen_decrement_r6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r7 register
     */
    virtual void gen_decrement_r7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r8 register
     */
    virtual void gen_decrement_r8(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r9 register
     */
    virtual void gen_decrement_r9(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r10 register
     */
    virtual void gen_decrement_r10(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r11 register
     */
    virtual void gen_decrement_r11(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r12 register
     */
    virtual void gen_decrement_r12(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r13 register
     */
    virtual void gen_decrement_r13(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r14 register
     */
    virtual void gen_decrement_r14(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r15 register
     */
    virtual void gen_decrement_r15(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r16 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r16 register
     */
    virtual void gen_decrement_r16(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r17 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r17 register
     */
    virtual void gen_decrement_r17(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r18 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r18 register
     */
    virtual void gen_decrement_r18(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r19 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r19 register
     */
    virtual void gen_decrement_r19(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r20 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r20 register
     */
    virtual void gen_decrement_r20(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r21 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r21 register
     */
    virtual void gen_decrement_r21(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r22 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r22 register
     */
    virtual void gen_decrement_r22(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r23 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r23 register
     */
    virtual void gen_decrement_r23(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r24 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r24 register
     */
    virtual void gen_decrement_r24(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r25 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r25 register
     */
    virtual void gen_decrement_r25(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r26 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r26 register
     */
    virtual void gen_decrement_r26(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r27 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r27 register
     */
    virtual void gen_decrement_r27(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r28 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r28 register
     */
    virtual void gen_decrement_r28(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r29 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r29 register
     */
    virtual void gen_decrement_r29(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r30 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r30 register
     */
    virtual void gen_decrement_r30(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 r31 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r31 register
     */
    virtual void gen_decrement_r31(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PC register
     */
    virtual void gen_decrement_PC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr0 register
     */
    virtual void gen_decrement_wr0(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr0_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr0_u register
     */
    virtual void gen_decrement_wr0_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr1 register
     */
    virtual void gen_decrement_wr1(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr1_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr1_u register
     */
    virtual void gen_decrement_wr1_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr2 register
     */
    virtual void gen_decrement_wr2(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr2_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr2_u register
     */
    virtual void gen_decrement_wr2_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr3 register
     */
    virtual void gen_decrement_wr3(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr3_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr3_u register
     */
    virtual void gen_decrement_wr3_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr4 register
     */
    virtual void gen_decrement_wr4(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr4_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr4_u register
     */
    virtual void gen_decrement_wr4_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr5 register
     */
    virtual void gen_decrement_wr5(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr5_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr5_u register
     */
    virtual void gen_decrement_wr5_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr6 register
     */
    virtual void gen_decrement_wr6(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr6_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr6_u register
     */
    virtual void gen_decrement_wr6_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr7 register
     */
    virtual void gen_decrement_wr7(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr7_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr7_u register
     */
    virtual void gen_decrement_wr7_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr8 register
     */
    virtual void gen_decrement_wr8(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr8_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr8_u register
     */
    virtual void gen_decrement_wr8_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr9 register
     */
    virtual void gen_decrement_wr9(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr9_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr9_u register
     */
    virtual void gen_decrement_wr9_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr10 register
     */
    virtual void gen_decrement_wr10(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr10_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr10_u register
     */
    virtual void gen_decrement_wr10_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr11 register
     */
    virtual void gen_decrement_wr11(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr11_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr11_u register
     */
    virtual void gen_decrement_wr11_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr12 register
     */
    virtual void gen_decrement_wr12(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr12_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr12_u register
     */
    virtual void gen_decrement_wr12_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr13 register
     */
    virtual void gen_decrement_wr13(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr13_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr13_u register
     */
    virtual void gen_decrement_wr13_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr14 register
     */
    virtual void gen_decrement_wr14(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr14_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr14_u register
     */
    virtual void gen_decrement_wr14_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr15 register
     */
    virtual void gen_decrement_wr15(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr15_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr15_u register
     */
    virtual void gen_decrement_wr15_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr16 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr16 register
     */
    virtual void gen_decrement_wr16(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr16_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr16_u register
     */
    virtual void gen_decrement_wr16_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr17 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr17 register
     */
    virtual void gen_decrement_wr17(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr17_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr17_u register
     */
    virtual void gen_decrement_wr17_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr18 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr18 register
     */
    virtual void gen_decrement_wr18(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr18_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr18_u register
     */
    virtual void gen_decrement_wr18_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr19 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr19 register
     */
    virtual void gen_decrement_wr19(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr19_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr19_u register
     */
    virtual void gen_decrement_wr19_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr20 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr20 register
     */
    virtual void gen_decrement_wr20(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr20_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr20_u register
     */
    virtual void gen_decrement_wr20_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr21 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr21 register
     */
    virtual void gen_decrement_wr21(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr21_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr21_u register
     */
    virtual void gen_decrement_wr21_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr22 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr22 register
     */
    virtual void gen_decrement_wr22(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr22_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr22_u register
     */
    virtual void gen_decrement_wr22_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr23 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr23 register
     */
    virtual void gen_decrement_wr23(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr23_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr23_u register
     */
    virtual void gen_decrement_wr23_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr24 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr24 register
     */
    virtual void gen_decrement_wr24(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr24_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr24_u register
     */
    virtual void gen_decrement_wr24_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr25 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr25 register
     */
    virtual void gen_decrement_wr25(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr25_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr25_u register
     */
    virtual void gen_decrement_wr25_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr26 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr26 register
     */
    virtual void gen_decrement_wr26(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr26_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr26_u register
     */
    virtual void gen_decrement_wr26_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr27 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr27 register
     */
    virtual void gen_decrement_wr27(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr27_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr27_u register
     */
    virtual void gen_decrement_wr27_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr28 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr28 register
     */
    virtual void gen_decrement_wr28(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr28_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr28_u register
     */
    virtual void gen_decrement_wr28_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr29 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr29 register
     */
    virtual void gen_decrement_wr29(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr29_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr29_u register
     */
    virtual void gen_decrement_wr29_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr30 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr30 register
     */
    virtual void gen_decrement_wr30(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr30_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr30_u register
     */
    virtual void gen_decrement_wr30_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr31 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr31 register
     */
    virtual void gen_decrement_wr31(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr31_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr31_u register
     */
    virtual void gen_decrement_wr31_u(fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPC register
     */
    virtual void gen_decrement_EIPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPSW register
     */
    virtual void gen_decrement_EIPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPC register
     */
    virtual void gen_decrement_FEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPSW register
     */
    virtual void gen_decrement_FEPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PSW_EIMASK register field
     */
    virtual void gen_decrement_PSW_EIMASK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_CC register field
     */
    virtual void gen_decrement_FPSR_CC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_RM register field
     */
    virtual void gen_decrement_FPSR_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XC register field
     */
    virtual void gen_decrement_FPSR_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XE register field
     */
    virtual void gen_decrement_FPSR_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XP register field
     */
    virtual void gen_decrement_FPSR_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPEPC register
     */
    virtual void gen_decrement_FPEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPST_XC register field
     */
    virtual void gen_decrement_FPST_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPST_XP register field
     */
    virtual void gen_decrement_FPST_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCC_CC register field
     */
    virtual void gen_decrement_FPCC_CC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCFG_RM register field
     */
    virtual void gen_decrement_FPCFG_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCFG_XE register field
     */
    virtual void gen_decrement_FPCFG_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIIC register
     */
    virtual void gen_decrement_EIIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEIC register
     */
    virtual void gen_decrement_FEIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PSWH_GPID register field
     */
    virtual void gen_decrement_PSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTPC register
     */
    virtual void gen_decrement_CTPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTPSW register
     */
    virtual void gen_decrement_CTPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPSWH_GPID register field
     */
    virtual void gen_decrement_EIPSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPSWH_GPID register field
     */
    virtual void gen_decrement_FEPSWH_GPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTBP register
     */
    virtual void gen_decrement_CTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SNZCFG register
     */
    virtual void gen_decrement_SNZCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIWR register
     */
    virtual void gen_decrement_EIWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEWR register
     */
    virtual void gen_decrement_FEWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 SPID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SPID register
     */
    virtual void gen_decrement_SPID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SPIDLIST register
     */
    virtual void gen_decrement_SPIDLIST(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBASE_RBASE register field
     */
    virtual void gen_decrement_RBASE_RBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EBASE_EBASE register field
     */
    virtual void gen_decrement_EBASE_EBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 INTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 INTBP register
     */
    virtual void gen_decrement_INTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PID register
     */
    virtual void gen_decrement_PID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SVLOCK register
     */
    virtual void gen_decrement_SVLOCK(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SCCFG_SIZE register field
     */
    virtual void gen_decrement_SCCFG_SIZE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 SCBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SCBP register
     */
    virtual void gen_decrement_SCBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 HVSB register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 HVSB register
     */
    virtual void gen_decrement_HVSB(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PEID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PEID register
     */
    virtual void gen_decrement_PEID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BMID_BMID register field
     */
    virtual void gen_decrement_BMID_BMID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MEA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MEA register
     */
    virtual void gen_decrement_MEA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MEI register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MEI register
     */
    virtual void gen_decrement_MEI(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBCR0_BE register field
     */
    virtual void gen_decrement_RBCR0_BE(fastiss::i16_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBCR1_NC register field
     */
    virtual void gen_decrement_RBCR1_NC(fastiss::i16_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBNR_BN register field
     */
    virtual void gen_decrement_RBNR_BN(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBIP_RBIP register field
     */
    virtual void gen_decrement_RBIP_RBIP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ISPR_ISP register field
     */
    virtual void gen_decrement_ISPR_ISP(fastiss::i16_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 INTCFG_ULNR register field
     */
    virtual void gen_decrement_INTCFG_ULNR(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PLMR_PLM register field
     */
    virtual void gen_decrement_PLMR_PLM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_RM register field
     */
    virtual void gen_decrement_FXSR_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XC register field
     */
    virtual void gen_decrement_FXSR_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XE register field
     */
    virtual void gen_decrement_FXSR_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XP register field
     */
    virtual void gen_decrement_FXSR_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXST_XC register field
     */
    virtual void gen_decrement_FXST_XC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXST_XP register field
     */
    virtual void gen_decrement_FXST_XP(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXCFG_RM register field
     */
    virtual void gen_decrement_FXCFG_RM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXCFG_XE register field
     */
    virtual void gen_decrement_FXCFG_XE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC3 register field
     */
    virtual void gen_decrement_FXXC_XC3(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC2 register field
     */
    virtual void gen_decrement_FXXC_XC2(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC1 register field
     */
    virtual void gen_decrement_FXXC_XC1(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC0 register field
     */
    virtual void gen_decrement_FXXC_XC0(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP3 register field
     */
    virtual void gen_decrement_FXXP_XP3(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP2 register field
     */
    virtual void gen_decrement_FXXP_XP2(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP1 register field
     */
    virtual void gen_decrement_FXXP_XP1(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP0 register field
     */
    virtual void gen_decrement_FXXP_XP0(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIPC register
     */
    virtual void gen_decrement_GMEIPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIPSW register
     */
    virtual void gen_decrement_GMEIPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEPC register
     */
    virtual void gen_decrement_GMFEPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEPSW register
     */
    virtual void gen_decrement_GMFEPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPSW_EIMASK register field
     */
    virtual void gen_decrement_GMPSW_EIMASK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMEA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMEA register
     */
    virtual void gen_decrement_GMMEA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMEI register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMEI register
     */
    virtual void gen_decrement_GMMEI(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIIC register
     */
    virtual void gen_decrement_GMEIIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEIC register
     */
    virtual void gen_decrement_GMFEIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSPID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSPID register
     */
    virtual void gen_decrement_GMSPID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSPIDLIST register
     */
    virtual void gen_decrement_GMSPIDLIST(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEBASE_EBASE register field
     */
    virtual void gen_decrement_GMEBASE_EBASE(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMINTBP register
     */
    virtual void gen_decrement_GMINTBP(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMINTCFG_ULNR register field
     */
    virtual void gen_decrement_GMINTCFG_ULNR(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPLMR_PLM register field
     */
    virtual void gen_decrement_GMPLMR_PLM(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSVLOCK register
     */
    virtual void gen_decrement_GMSVLOCK(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMPM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMPM register
     */
    virtual void gen_decrement_GMMPM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIWR register
     */
    virtual void gen_decrement_GMEIWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEWR register
     */
    virtual void gen_decrement_GMFEWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPEID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPEID register
     */
    virtual void gen_decrement_GMPEID(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPM register
     */
    virtual void gen_decrement_MPM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_NMPUE register field
     */
    virtual void gen_decrement_MPCFG_NMPUE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_HBE register field
     */
    virtual void gen_decrement_MPCFG_HBE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_ARCH register field
     */
    virtual void gen_decrement_MPCFG_ARCH(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_NBK register field
     */
    virtual void gen_decrement_MPCFG_NBK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCA register
     */
    virtual void gen_decrement_MCA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCS register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCS register
     */
    virtual void gen_decrement_MCS(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCC register
     */
    virtual void gen_decrement_MCC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCI_SPID register field
     */
    virtual void gen_decrement_MCI_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPIDX_IDX register field
     */
    virtual void gen_decrement_MPIDX_IDX(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPBK_BK register field
     */
    virtual void gen_decrement_MPBK_BK(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPLA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPLA register
     */
    virtual void gen_decrement_MPLA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPUA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPUA register
     */
    virtual void gen_decrement_MPUA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID0_SPID register field
     */
    virtual void gen_decrement_MPID0_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID1_SPID register field
     */
    virtual void gen_decrement_MPID1_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID2_SPID register field
     */
    virtual void gen_decrement_MPID2_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID3_SPID register field
     */
    virtual void gen_decrement_MPID3_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID4_SPID register field
     */
    virtual void gen_decrement_MPID4_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID5_SPID register field
     */
    virtual void gen_decrement_MPID5_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID6_SPID register field
     */
    virtual void gen_decrement_MPID6_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID7_SPID register field
     */
    virtual void gen_decrement_MPID7_SPID(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGL_LPN register field
     */
    virtual void gen_decrement_ICTAGL_LPN(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGL_LRU register field
     */
    virtual void gen_decrement_ICTAGL_LRU(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGH_DATAECC register field
     */
    virtual void gen_decrement_ICTAGH_DATAECC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGH_TAGECC register field
     */
    virtual void gen_decrement_ICTAGH_TAGECC(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICDATL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICDATL register
     */
    virtual void gen_decrement_ICDATL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICDATH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICDATH register
     */
    virtual void gen_decrement_ICDATH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    virtual void gen_decrement_ICCFG_ICHSIZE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHLINE register field
     */
    virtual void gen_decrement_ICCFG_ICHLINE(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHWAY register field
     */
    virtual void gen_decrement_ICCFG_ICHWAY(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICERR_ICHEWY register field
     */
    virtual void gen_decrement_ICERR_ICHEWY(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICERR_ICHEIX register field
     */
    virtual void gen_decrement_ICERR_ICHEIX(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCOUNTL register
     */
    virtual void gen_decrement_TSCOUNTL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCOUNTH register
     */
    virtual void gen_decrement_TSCOUNTH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCTRL register
     */
    virtual void gen_decrement_TSCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMUMCTRL register
     */
    virtual void gen_decrement_PMUMCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMGMCTRL register
     */
    virtual void gen_decrement_PMGMCTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT0 register
     */
    virtual void gen_decrement_PMCOUNT0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL0 register
     */
    virtual void gen_decrement_PMCTRL0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT1 register
     */
    virtual void gen_decrement_PMCOUNT1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL1 register
     */
    virtual void gen_decrement_PMCTRL1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT2 register
     */
    virtual void gen_decrement_PMCOUNT2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL2 register
     */
    virtual void gen_decrement_PMCTRL2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT3 register
     */
    virtual void gen_decrement_PMCOUNT3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL3 register
     */
    virtual void gen_decrement_PMCTRL3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL4 register
     */
    virtual void gen_decrement_PMCTRL4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL5 register
     */
    virtual void gen_decrement_PMCTRL5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL6 register
     */
    virtual void gen_decrement_PMCTRL6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL7 register
     */
    virtual void gen_decrement_PMCTRL7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT4 register
     */
    virtual void gen_decrement_PMCOUNT4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT5 register
     */
    virtual void gen_decrement_PMCOUNT5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT6 register
     */
    virtual void gen_decrement_PMCOUNT6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT7 register
     */
    virtual void gen_decrement_PMCOUNT7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND0 register
     */
    virtual void gen_decrement_PMSUBCND0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND1 register
     */
    virtual void gen_decrement_PMSUBCND1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND2 register
     */
    virtual void gen_decrement_PMSUBCND2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND3 register
     */
    virtual void gen_decrement_PMSUBCND3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND4 register
     */
    virtual void gen_decrement_PMSUBCND4(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND5 register
     */
    virtual void gen_decrement_PMSUBCND5(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND6 register
     */
    virtual void gen_decrement_PMSUBCND6(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND7 register
     */
    virtual void gen_decrement_PMSUBCND7(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBGEN register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBGEN register
     */
    virtual void gen_decrement_DBGEN(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPSWH register
     */
    virtual void gen_decrement_DBPSWH(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBIC register
     */
    virtual void gen_decrement_DBIC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPC register
     */
    virtual void gen_decrement_DBPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPSW register
     */
    virtual void gen_decrement_DBPSW(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DIR0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DIR0 register
     */
    virtual void gen_decrement_DIR0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DIR1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DIR1 register
     */
    virtual void gen_decrement_DIR1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPC register
     */
    virtual void gen_decrement_BPC(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPAV register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPAV register
     */
    virtual void gen_decrement_BPAV(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPAM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPAM register
     */
    virtual void gen_decrement_BPAM(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBCFG register
     */
    virtual void gen_decrement_DBCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBWR register
     */
    virtual void gen_decrement_DBWR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSTEST0 register
     */
    virtual void gen_decrement_LSTEST0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSTEST1 register
     */
    virtual void gen_decrement_LSTEST1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSCFG register
     */
    virtual void gen_decrement_LSCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICBKEY register
     */
    virtual void gen_decrement_ICBKEY(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 IFCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 IFCR register
     */
    virtual void gen_decrement_IFCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 IFCR1 register
     */
    virtual void gen_decrement_IFCR1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCTRL0 register
     */
    virtual void gen_decrement_BRPCTRL0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCTRL1 register
     */
    virtual void gen_decrement_BRPCTRL1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCFG register
     */
    virtual void gen_decrement_BRPCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPACTRL register
     */
    virtual void gen_decrement_BRPACTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPDATA register
     */
    virtual void gen_decrement_BRPDATA(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DCBKEY register
     */
    virtual void gen_decrement_DCBKEY(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSUCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSUCR register
     */
    virtual void gen_decrement_LSUCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSULNK0 register
     */
    virtual void gen_decrement_LSULNK0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSULNK1 register
     */
    virtual void gen_decrement_LSULNK1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLCR register
     */
    virtual void gen_decrement_L1RLCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLNK0 register
     */
    virtual void gen_decrement_L1RLNK0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLNK1 register
     */
    virtual void gen_decrement_L1RLNK1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RCFG register
     */
    virtual void gen_decrement_L1RCFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DECFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DECFG register
     */
    virtual void gen_decrement_DECFG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DECTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DECTRL register
     */
    virtual void gen_decrement_DECTRL(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 DEVDS register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DEVDS register
     */
    virtual void gen_decrement_DEVDS(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBCR register
     */
    virtual void gen_decrement_RDBCR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBACR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBACR register
     */
    virtual void gen_decrement_RDBACR(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBATAG register
     */
    virtual void gen_decrement_RDBATAG(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT0 register
     */
    virtual void gen_decrement_RDBADAT0(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT1 register
     */
    virtual void gen_decrement_RDBADAT1(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT2 register
     */
    virtual void gen_decrement_RDBADAT2(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT3 register
     */
    virtual void gen_decrement_RDBADAT3(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBSTAT register
     */
    virtual void gen_decrement_RDBSTAT(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 hv_func_support register
     */
    virtual void gen_decrement_hv_func_support(fastiss::i8_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 swd0pcad_exec_count register
     */
    virtual void gen_decrement_swd0pcad_exec_count(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 swd0pcad_val register
     */
    virtual void gen_decrement_swd0pcad_val(fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 mem_acc_type register
     */
    virtual void gen_decrement_mem_acc_type(fastiss::i32_t *value) = 0;
    /**
     * @}
     */
    
    /**
     * \addtogroup GenerativeRegisterGroupDecrementOperations
     * @{
     */
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 GR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 GR group register
     */
    virtual void gen_decrement_GR(rh850g4::GR_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 WR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 WR group register
     */
    virtual void gen_decrement_WR(rh850g4::WR_index_t index, fastiss::i64_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_0 group register
     */
    virtual void gen_decrement_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_1 group register
     */
    virtual void gen_decrement_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_2 group register
     */
    virtual void gen_decrement_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_3 group register
     */
    virtual void gen_decrement_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_4 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_4 group register
     */
    virtual void gen_decrement_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_5 group register
     */
    virtual void gen_decrement_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_9 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_9 group register
     */
    virtual void gen_decrement_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_10 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_10 group register
     */
    virtual void gen_decrement_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_11 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_11 group register
     */
    virtual void gen_decrement_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_12 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_12 group register
     */
    virtual void gen_decrement_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_13 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_13 group register
     */
    virtual void gen_decrement_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_14 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_14 group register
     */
    virtual void gen_decrement_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value) = 0;
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_15 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_15 group register
     */
    virtual void gen_decrement_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value) = 0;
    /**
     * @}
     */
  };
}

#endif // _RH850G4_CPU_STATE_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
