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
 * \file custom_if.h
 * \brief Defines the Custom Generator interface class
 */

#ifndef _RH850G4_CUSTOM_IF_H_
#define _RH850G4_CUSTOM_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "fastiss/code_gen_if.h"
#include "rh850g4/cpu_state_if.h"

//------------------------------------------------------------------------------
// Macros, type definitions, static LUTs etc
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \def INT32_MAX
   * \brief Definition of the maximum 32-bit integer value
   */
#define INT32_MAX (0x7FFFFFFF)
  /**
   * \def INT32_MIN
   * \brief Definition of the minimum 32-bit integer value
   */
#define INT32_MIN (-INT32_MAX - 1)
  /**
   * \def STM_LDM_SELID0_INDEX_NUM
   * \brief Definition of stm_ldm_selid0_index[]'s index size 
   */
#define STM_LDM_SELID0_INDEX_NUM (5)
  /**
   * \def STM_LDM_SELID1_INDEX_NUM
   * \brief Definition of stm_ldm_selid1_index[]'s index size 
   */
#define STM_LDM_SELID1_INDEX_NUM (3)
  /**
   * \def STM_LDM_SELID2_INDEX_NUM
   * \brief Definition of stm_ldm_selid2_index[]'s index size 
   */
#define STM_LDM_SELID2_INDEX_NUM (4)
  /**
   * \def STM_LDM_SELID5_INDEX_NUM
   * \brief Definition of stm_ldm_selid5_index[]'s index size 
   */
#define STM_LDM_SELID5_INDEX_NUM (5)
  /**
   * \def STM_LDM_SELID9_INDEX_NUM
   * \brief Definition of stm_ldm_selid9_index[]'s index size 
   */
#define STM_LDM_SELID9_INDEX_NUM (19)


  /**
   * \enum rh850g4::condition_code_t
   * \brief Enumerated type to represent an instruction condition code
   */
  typedef enum
  {
    V_COND_CODE =  0x0,
    C_COND_CODE =  0x1,
    Z_COND_CODE =  0x2,
    NH_COND_CODE = 0x3,
    N_COND_CODE =  0x4,
    T_COND_CODE =  0x5,
    LT_COND_CODE = 0x6,
    LE_COND_CODE = 0x7,
    NV_COND_CODE = 0x8,
    NC_COND_CODE = 0x9,
    NZ_COND_CODE = 0xa,
    H_COND_CODE =  0xb,
    P_COND_CODE =  0xc,
    SA_COND_CODE = 0xd,
    GE_COND_CODE = 0xe,
    GT_COND_CODE = 0xf
  } condition_code_t;

  /**
   * \var rh850g4::condition_code_name
   * \brief Array of the name of each instruction condition code
   */
  static const char *condition_code_name[16] =
  {
    "v",  // V_COND_CODE
    "c",  // C_COND_CODE
    "z",  // Z_COND_CODE
    "nh", // NH_COND_CODE
    "s",  // N_COND_CODE
    "t",  // T_COND_CODE
    "lt", // LT_COND_CODE
    "le", // LE_COND_CODE
    "nv", // NV_COND_CODE
    "nc", // NC_COND_CODE
    "nz", // NZ_COND_CODE
    "h",  // H_COND_CODE
    "ns", // P_COND_CODE
    "sa", // SA_COND_CODE
    "ge", // GE_COND_CODE
    "gt"  // GT_COND_CODE
  };
  
  /**
   * \var rh850g4::branch_condition_code_name
   * \brief Array of the name of each instruction branching condition code
   */
  static const char *branch_condition_code_name[16] =
  {
    "bv",  // V_COND_CODE
    "bc",  // C_COND_CODE
    "bz",  // Z_COND_CODE
    "bnh", // NH_COND_CODE
    "bn",  // N_COND_CODE
    "br",  // T_COND_CODE
    "blt", // LT_COND_CODE
    "ble", // LE_COND_CODE
    "bnv", // NV_COND_CODE
    "bnc", // NC_COND_CODE
    "bnz", // NZ_COND_CODE
    "bh",  // H_COND_CODE
    "bp",  // P_COND_CODE
    "bsa", // SA_COND_CODE
    "bge", // GE_COND_CODE
    "bgt"  // GT_COND_CODE
  };
  
  static const char *fpu_condition_code_name[16] =
  {
    "f",    // false
    "un",   // Unordered
    "eq",   // reg2 = reg1
    "ueq",  // reg2? = reg1
    "olt",  // reg2 < reg1
    "ult",  // reg2? < reg1
    "ole",  // reg2 <= reg1
    "ule",  // reg2? <= reg1
    "sf",   // false
    "ngle", // Unordered
    "seq",  // reg2 = reg1
    "ngl",  // reg2? = reg1
    "lt",   // reg2 < reg1
    "nge",  // reg2? < reg1
    "le",   // reg2 <= reg1
    "ngt"   // reg2? <= reg
  };
  
  /**
   * \brief Array of register masks used by the prepare and dispose instructions.
   * These masks are used to determine which registers in r20-r31 have been selected.
   */
  const unsigned short prepare_dispose_rx_mask[12] =
  {
    0x080,
    0x040,
    0x020,
    0x010,
    0x800,
    0x400,
    0x200,
    0x100,
    0x008,
    0x004,
    0x001,
    0x002    
  };

  /**
   * \brief Array of system register index used by the stm.gsr and ldm.gsr instructions.
   * These indexs are used to determine which system registers have been selected.
   */
  const unsigned int stm_ldm_selid0_index[STM_LDM_SELID0_INDEX_NUM] =
  {
    6,
    7,
    16,
    17,
    20
  };
  const unsigned int stm_ldm_selid1_index[STM_LDM_SELID1_INDEX_NUM] =
  {
    5,
    11,
    12
  };
  const unsigned int stm_ldm_selid2_index[STM_LDM_SELID2_INDEX_NUM] =
  {
    15,
    16,
    17,
    18
  };
  const unsigned int stm_ldm_selid5_index[STM_LDM_SELID5_INDEX_NUM] =
  {
    8,
    9,
    11,
    12,
    16
  };
  const unsigned int stm_ldm_selid9_index[STM_LDM_SELID9_INDEX_NUM] =
  {
    0,
    1,
    2,
    3,
    5,
    6,
    8,
    13,
    14,
    16,
    17,
    19,
    20,
    21,
    22,
    24,
    25,
    28,
    29
  };
}


//------------------------------------------------------------------------------
// Custom interface class
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief Interface providing custom extension operations for a Fast-ISS.
   * The operations provided include common PSW register flag calculations, division
   * operations, and condition code determination.
   */
  class custom_if_t
  {
  public:
    /**
     * \defgroup CustomExtensionOperations Custom extension operations
     * @{
     * \defgroup BlockMultipleExitOperations Block multiple exit operations
     * \defgroup CoreInterfaceHelperOperations Core interface helper operations
     * \defgroup PSWRegisterCalculationOperations PSW Register Calculation operations
     * \defgroup ConditionCodeOperations Condition code operations
     * \defgroup DivisionOperations Division operations
     * \defgroup FlushDenormOperations Flush floating point denormalised value operations
     * \defgroup UnderflowFlagUpdateOperations Floating point underflow flag update operations
     * \defgroup InexactFlagUpdateOperations Floating point inexact flag update operations
     * \defgroup DestinationAndStatusRegisterUpdateOperations Floating point destination and status register update operations
     * @}
     */

    /**
     * \addtogroup BlockMultipleExitOperations
     * @{
     */

    /**
     * \brief Creates an exit block. This is used at the start of translating a
     * block of code to enable multiple exit points from the block.
     */
    virtual void create_exit_block(void) = 0;
    /**
     * \brief Returns the exit block. This is used when a block exit point is
     * encountered during the translation of code.
     * \return The exit block
     */
    virtual fastiss::block_t *get_exit_block(void) = 0;
    /**
     * @}
     */

    /**
     * \brief Generates code to increment the internal local time value.
     * \param increment Number of cycles by which to increment the internal local time value.
     */
    virtual void gen_increment_cycle_count(fastiss::i32_t *increment) = 0;

    /**
     * \brief Notify the interrupt accepted analysis point.
     */
    virtual void notify_interrupt_accepted(void) = 0;

    /**
     * \brief Notify the interrupt complete analysis point.
     */
    virtual void notify_interrupt_complete(void) = 0;

    /**
     * \addtogroup PSWRegisterCalculationOperations
     * @{
     */
    
    /**
     * \brief Generates a 1-bit value for the S (sign) bit of the PSW register
     * based on the 32-bit result of an operation. This allows the S calculation
     * to be standardised across all relevant instructions.
     * \param op_result The 32-bit result of an operation
     * \return A 1-bit value for the S bit of the PSW register
     */
    virtual fastiss::i1_t *gen_calculate_PSW_S(fastiss::i32_t *op_result) = 0;
    /**
     * \brief Generates a 1-bit value for the Z (zero) bit of the PSW register
     * based on the 32-bit result of an operation. This allows the Z calculation
     * to be standardised across all relevant instructions.
     * \param op_result The 32-bit result of an operation
     * \return A 1-bit value for the Z bit of the PSW register
     */
    virtual fastiss::i1_t *gen_calculate_PSW_Z(fastiss::i32_t *op_result) = 0;
    
    /**
     * \brief Generates a 1-bit value for the CY (carry) bit of the PSW register
     * based on a shift operation. This allows the CY calculation to be
     * standardised across all relevant instructions.
     * \param val A 32-bit value that is being shifted
     * \param shift A 32-bit value that is the shift amount
     * \param is_left_shift True is a left shift is being performed, false if a right shift
     * \return A 1-bit value for the CY bit of the PSW register
     */
    virtual fastiss::i1_t *gen_calculate_PSW_CY_shift(fastiss::i32_t *val, fastiss::i32_t *shift, bool is_left_shift) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup ConditionCodeOperations
     * @{
     */

    /**
     * \brief Determine if a condition is always (unconditional) satisfied
     * according to a condition code
     * \param condition The condition code
     * \return True if the condition code indicated that the condition is
     * always satisfied, false otherwise
     */
    virtual bool is_condition_always(unsigned char condition) = 0;
    
    /**
     * \brief Generates whether a condition is satisfied according to a condition
     * code and the CPU state
     * \param condition The condition code
     * \return A boolean value that is true if the condition is satisfied and
     * false otherwise
     */
    virtual fastiss::i1_t *gen_condition_satisfied(unsigned char condition) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup DivisionOperations
     * @{
     */

    /**
     * \brief Generates a division operation that also determines whether the
     * result is signed, zero, or overflow occurs. This allows the division to be
     * standardised across all relevant instructions. This function directly
     * updates all affected registers.
     * \param reg1_val A 32-bit reg1 value that is the divisor of the operation
     * \param reg2 Index of the reg2 register that is both the dividend of the
     * operation and the destination register of the divide result
     */
    virtual void gen_div_reg1_reg2(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2) = 0;
    /**
     * \brief Generates a division operation that also determines whether the
     * result is signed, zero, or overflow occurs. This allows the division to be
     * standardised across all relevant instructions. This function directly
     * updates all affected registers.
     * \param reg1_val A 32-bit reg1 value that is the divisor of the operation
     * \param reg2 Index of the reg2 register that is both the dividend of the
     * operation and the destination register of the divide result
     * \param reg3 Index of the reg3 register that is the destination register of
     * the remainder of the divide result
     */
    virtual void gen_div_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2, rh850g4::GR_index_t reg3) = 0;
    /**
     * \brief Generates an unsigned division operation that also determines whether the
     * result is signed, zero, or overflow occurs. This allows the division to be
     * standardised across all relevant instructions. This function directly
     * updates all affected registers.
     * \param reg1_val A 32-bit reg1 value that is the divisor of the operation
     * \param reg2 Index of the reg2 register that is both the dividend of the
     * operation and the destination register of the divide result
     * \param reg3 Index of the reg3 register that is the destination register of
     * the remainder of the divide result
     * \param is_halfword True if a half-word (16-bit) divide operation, false if a
     * full-word (32-bit) divide operation.
     */
    virtual void gen_divu_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850g4::GR_index_t reg2, rh850g4::GR_index_t reg3, bool is_halfword = false) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup FlushDenormOperations
     * @{
     */

    /**
     * \brief Check the value whether unimplemnted exception occurs or not.
     * If the velue is denormalised value and FPSR.FS=0, the unimplemnted exception occurs.
     * \param value The 32-bit floating point value to be processed for flushing.
     * \return The true(unimplemnted exception occur) or false result.
     */
    virtual fastiss::i1_t *is_unimplemented_exp(fastiss::f32_t *value) = 0;

    /**
     * \brief Check the value whether unimplemnted exception occurs or not.
     * If the velue is denormalised value and FPSR.FS=0, the unimplemnted exception occurs.
     * \param value The 64-bit floating point value to be processed for flushing.
     * \return The true(unimplemnted exception occur) or false result.
     */
    virtual fastiss::i1_t *is_unimplemented_exp(fastiss::f64_t *value) = 0;

    /**
     * \brief Check the value whether unimplemnted exception occurs or not.
     * If the velue is denormalised value and FXSR.FS=0, the unimplemnted exception occurs.
     * \param value The 32-bit floating point value to be processed for flushing.
     * \return The true(unimplemnted exception occur) or false result.
     */
    virtual fastiss::i1_t *is_unimplemented_fxu_exp(fastiss::f32_t *value) = 0;

    /**
     * \brief Flush denormalised values to zero. Note that the sign of a denormalised
     * value is preserved and normal values are also preserved.
     * \param value The 32-bit floating point value to be processed for flushing.
     * \return The normal or zero result.
     */
    virtual fastiss::f32_t *gen_flush_denorm(fastiss::f32_t *value) = 0;

    /**
     * \brief Flush denormalised values to zero. Note that the sign of a denormalised
     * value is preserved and normal values are also preserved.
     * \param value The 64-bit floating point value to be processed for flushing.
     * \return The normal or zero result.
     */
    virtual fastiss::f64_t *gen_flush_denorm(fastiss::f64_t *value) = 0;

    /**
     * \brief Flush denormalised values to zero. Note that the sign of a denormalised
     * value is preserved and normal values are also preserved.
     * \param value The 32-bit floating point value to be processed for flushing.
     * \return The normal or zero result.
     */
    virtual fastiss::f32_t *gen_flush_denorm_fxu(fastiss::f32_t *value) = 0;
    /**
     * @}
     */
     
    /**
     * \addtogroup UnderflowFlagUpdateOperations
     * @{
     */

    /**
     * \brief Updates the underflow flag (and inexact flag) when a denormalised
     * value is the result of a floating point operation
     * \param op_result The 16-bit floating point result and exception flags of
     * an operation
     */
    virtual void gen_underflow_denorm_update(fastiss::f16_with_exception_flags_t& op_result) = 0;

    /**
     * \brief Updates the underflow flag (and inexact flag) when a denormalised
     * value is the result of a floating point operation
     * \param op_result The 32-bit floating point result and exception flags of
     * an operation
     */
    virtual void gen_underflow_denorm_update(fastiss::f32_with_exception_flags_t& op_result) = 0;

    /**
     * \brief Updates the underflow flag (and inexact flag) when a denormalised
     * value is the result of a floating point operation
     * \param op_result The 64-bit floating point result and exception flags of
     * an operation
     */
    virtual void gen_underflow_denorm_update(fastiss::f64_with_exception_flags_t& op_result) = 0;

    /**
     * \brief Updates the underflow flag (and inexact flag) when a denormalised
     * value is the result of a floating point operation
     * \param op_result The 80-bit floating point result and exception flags of
     * an operation
     */
    virtual void gen_underflow_denorm_update(fastiss::f80_with_exception_flags_t& op_result) = 0;
    /**
     * @}
     */
     
    /**
     * \addtogroup InexactFlagUpdateOperations
     * @{
     */

    /**
     * \brief Determines the inexact flag when neither an invalid exception nor a
     * divide by zero exception has resulted from a floating point operation,
     * and an operand denormalised number is flushed and the other operands are
     * not quiet NaN's
     * \param invalid The 1-bit invalid flag from a floating point operation
     * \param divide_by_zero The 1-bit divide-by-zero flag from a floating point
     * operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \param reg2 The original value of the operand register
     * \return The inexact flag result
     */
    virtual fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg2) = 0;

    /**
     * \brief Determines the inexact flag when neither an invalid exception nor a
     * divide by zero exception has resulted from a floating point operation,
     * and an operand denormalised number is flushed and the other operands are
     * not quiet NaN's
     * \param invalid The 1-bit invalid flag from a floating point operation
     * \param divide_by_zero The 1-bit divide-by-zero flag from a floating point
     * operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \param reg2_pair The original value of the operand register pair
     * \return The inexact flag result
     */
    virtual fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg2_pair) = 0;

    /**
     * \brief Determines the inexact flag when neither an invalid exception nor a
     * divide by zero exception has resulted from a floating point operation,
     * and an operand denormalised number is flushed and the other operands are
     * not quiet NaN's
     * \param invalid The 1-bit invalid flag from a floating point operation
     * \param divide_by_zero The 1-bit divide-by-zero flag from a floating point
     * operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \param reg1 The original value of the first operand register
     * \param reg2 The original value of the second operand register
     * \return The inexact flag result
     */
    virtual fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2) = 0;

    /**
     * \brief Determines the inexact flag when neither an invalid exception nor a
     * divide by zero exception has resulted from a floating point operation,
     * and an operand denormalised number is flushed and the other operands are
     * not quiet NaN's
     * \param invalid The 1-bit invalid flag from a floating point operation
     * \param divide_by_zero The 1-bit divide-by-zero flag from a floating point
     * operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \param reg1_pair The original value of the first operand register pair
     * \param reg2_pair The original value of the second operand register pair
     * \return The inexact flag result
     */
    virtual fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg1_pair, fastiss::f64_t *reg2_pair) = 0;

    /**
     * \brief Determines the inexact flag when neither an invalid exception nor a
     * divide by zero exception has resulted from a floating point operation,
     * and an operand denormalised number is flushed and the other operands are
     * not quiet NaN's
     * \param invalid The 1-bit invalid flag from a floating point operation
     * \param divide_by_zero The 1-bit divide-by-zero flag from a floating point
     * operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \param reg1 The original value of the first operand register
     * \param reg2 The original value of the second operand register
     * \param reg3 The original value of the third operand register
     * \return The inexact flag result
     */
    virtual fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2, fastiss::f32_t *reg3) = 0;    

    /**
     * \brief Determines the inexact flag when either an enabled overflow
     * exception or an enabled underflow exception has resulted from a floating
     * point operation
     * \param overflow The 1-bit overflow flag from a floating point operation
     * \param underflow The 1-bit underflow flag from a floating point operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \return The inexact flag result
     */

    virtual fastiss::i1_t *gen_inexact_enabled_overflow_underflow_update(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact) = 0;
    /**
     * \brief Determines the inexact flag when either an enabled overflow
     * exception or an enabled underflow exception has resulted from a floating
     * point operation
     * \param overflow The 1-bit overflow flag from a floating point operation
     * \param underflow The 1-bit underflow flag from a floating point operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \return The inexact flag result
     */
    virtual fastiss::i1_t *gen_inexact_enabled_overflow_underflow_update_fxu(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact) = 0;
    /**
     * @}
     */
     
    /**
     * \addtogroup DestinationAndStatusRegisterUpdateOperations
     * @{
     */

    /**
     * \brief Determines the handling of the result of a floating point
     * operation. This function will:
     *    - Determine if a floating point exception has occurred that is
     *      enabled, defined by the FPSR register enable (XE) fields
     *    - If there are no enabled exceptions then:
     *         - The destination register is updated with the operation result
     *         - The FPSR register preservation (XP) fields are updated with the
     *           exception flags
     *    - If there is at least one enabled exception then:
     *         - It is determined whether the exceptions are precise or imprecise
     *         - The FPSR register cause (XC) fields are updated with the
     *           exception flags
     *         - The PC address is written to the FPEPC register
     *         - It is determined whether the exception can be immediately
     *           acknowledged or not - a precise exception is always acknowledged
     *           immediately whilst an imprecise exception is acknowledged only if
     *           the PSW.ID and PSW.NP fields are both zero
     *         - If the exception cannot be immediately acknowledged then it is
     *           added as a pending exception
     *         - If the exception can be immediately acknowledged then exception
     *           processing is commenced
     *
     * Execution will branch to the exit block of the translated code if an
     * exception is immediately acknowledged or the continue block otherwise
     * \param is_single_precision True if the calling floating point instruction
     * is single precision, false if it is double precision
     * \param dest_index The index of the destination register
     * \param dest_val The 32-bit value to conditionally write to the destination
     * register
     * \param exception_flags An 8-bit value containing the five floating point
     * exception flags
     * \param continue_block The block to branch to and continue translation at
     */
    virtual void gen_update_dest_and_fpsr(bool is_single_precision, rh850g4::GR_index_t dest_index, fastiss::i32_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block) = 0;

    /**
     * \brief Determines the handling of the result of a floating point
     * operation. This function will:
     *    - Determine if a floating point exception has occurred that is
     *      enabled, defined by the FPSR register enable (XE) fields
     *    - If there are no enabled exceptions then:
     *         - The destination register pair is updated with the operation result
     *         - The FPSR register preservation (XP) fields are updated with the
     *           exception flags
     *    - If there is at least one enabled exception then:
     *         - It is determined whether the exceptions are precise or imprecise
     *         - The FPSR register cause (XC) fields are updated with the
     *           exception flags
     *         - The PC address is written to the FPEPC register
     *         - It is determined whether the exception can be immediately
     *           acknowledged or not - a precise exception is always acknowledged
     *           immediately whilst an imprecise exception is acknowledged only if
     *           the PSW.ID and PSW.NP fields are both zero
     *         - If the exception cannot be immediately acknowledged then it is
     *           added as a pending exception
     *         - If the exception can be immediately acknowledged then exception
     *           processing is commenced
     *
     * Execution will branch to the exit block of the translated code if an
     * exception is immediately acknowledged or the continue block otherwise
     * \param is_single_precision True if the calling floating point instruction
     * is single precision, false if it is double precision
     * \param dest_index The index of the first destination register in the
     * register pair
     * \param dest_val The 64-bit value to conditionally write to the destination
     * register pair
     * \param exception_flags An 8-bit value containing the five floating point
     * exception flags
     * \param continue_block The block to branch to and continue translation at
     */
    virtual void gen_update_dest_and_fpsr(bool is_single_precision, rh850g4::GR_index_t dest_index, fastiss::i64_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block) = 0;

    /**
     * \brief Determines the handling of the result of a expand floating point
     * operation. This function will:
     *    - Determine if a expand floating point exception has occurred that is
     *      enabled, defined by the FXSR register enable (XE) fields
     *    - If there are no enabled exceptions then:
     *         - The destination register is updated with the operation result
     *         - The FXSR register preservation (XP) fields are updated with the
     *           exception flags
     *    - If there is at least one enabled exception then:
     *         - It is determined whether the exceptions are precise or imprecise
     *         - The FXSR register cause (XC) fields are updated with the
     *           exception flags
     *         - It is determined whether the exception can be immediately
     *           acknowledged or not - a precise exception is always acknowledged
     *           immediately whilst an imprecise exception is acknowledged only if
     *           the PSW.ID and PSW.NP fields are both zero
     *         - If the exception cannot be immediately acknowledged then it is
     *           added as a pending exception
     *         - If the exception can be immediately acknowledged then exception
     *           processing is commenced
     *
     * Execution will branch to the exit block of the translated code if an
     * exception is immediately acknowledged or the continue block otherwise
     * \param is_single_precision True if the calling floating point instruction
     * is single precision, false if it is double precision
     * \param dest_index The index of the destination register
     * \param dest_val The 32-bit value to conditionally write to the destination
     * register
     * \param w0_exp_flags An 8-bit value containing the five floating point
     * exception flags for way 0
     * \param w1_exp_flags An 8-bit value containing the five floating point
     * exception flags for way 1
     * \param w2_exp_flags An 8-bit value containing the five floating point
     * exception flags for way 2
     * \param w3_exp_flags An 8-bit value containing the five floating point
     * \param exception_flags An 8-bit value containing the five floating point
     * exception flags
     * \param continue_block The block to branch to and continue translation at
     */
    virtual void gen_update_dest_and_fxsr(bool is_single_precision, rh850g4::WR_index_t dest_index, fastiss::i64_t* dest_lo_val, fastiss::i64_t* dest_up_val, fastiss::i8_t* w0_exp_flag, fastiss::i8_t* w1_exp_flag, fastiss::i8_t* w2_exp_flag, fastiss::i8_t* w3_exp_flag, fastiss::block_t *continue_block) = 0;


    /**
     * @}
     */

    /**
     * \brief Generates get round mode from FPSR.RM,
     */
    virtual fastiss::i8_t *gen_read_round_mode(void)= 0;

    /**
     * \brief Generates clear FPI flag,
     */
    virtual void gen_clear_fpi_flag(void)= 0;

    /**
     * \brief Generates an unsigned bins operation,
     * \param reg1 Index of the reg1 register
     * \param reg2 Index of the reg2 register
     * \param msb_val A 32-bit msb value
     * \param lsb_val A 32-bit lsb value
     */
    virtual void gen_bins_common(rh850g4::GR_index_t reg1, rh850g4::GR_index_t reg2, fastiss::i32_t *msb_val, fastiss::i32_t *lsb_val)= 0;
    /**
     * \brief Generates an sifht operation,,
     * \param x_val A 32-bit value
     * \param l_val A 32-bit value
     * \param r_val A 32-bit value
     */
    virtual fastiss::i32_t *gen_bit_extract(fastiss::i32_t *x_val, fastiss::i32_t *l_val, fastiss::i32_t *r_val) = 0;
    /**
     * \brief Generates an vector shuffle operation,,
     * \param pos A 32-bit value
     * \param imm12_val A 32-bit value
     * \param wreg1 Index of the wreg1 register
     * \param wreg2 Index of the wreg1 register
     */
    virtual fastiss::i32_t *gen_vector_shuffle(fastiss::i32_t *pos, fastiss::i32_t *imm12_val, rh850g4::WR_index_t wreg1, rh850g4::WR_index_t wreg2) = 0;
    /**
     * \brief Generates an FP SIMD add operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of add culculation
     * \param res_exp A result of exception
     * \param is_neg A flag whether the result need to be negative exchanged or not
     */
    virtual void gen_fp_simd_add(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i1_t *is_neg) = 0;
    /**
     * \brief Generates an FP SIMD sub operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of sub culculation
     * \param res_exp A result of exception
     * \param is_neg A flag whether the result need to be negative exchanged or not
     */
    virtual void gen_fp_simd_sub(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i1_t *is_neg) = 0;
    /**
     * \brief Generates an FP SIMD mul operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of mul culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_mul(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD div operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of div culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_div(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD max operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of max culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_max(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD min operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of min culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_min(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD covert half single to single operation,,
     * \param input_val_1 A Input operand 1 
     * \param res_val A result of covert culculation
     * \param res_exp A result of exception
     * \param round_mode A round mode
     */
    virtual void gen_fp_simd_cvt_hs(fastiss::f16_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode) = 0;
    /**
     * \brief Generates an FP SIMD covert single to half single operation,,
     * \param input_val_1 A Input operand 1 
     * \param res_val A result of covert culculation
     * \param res_exp A result of exception
     * \param round_mode A round mode
     */
    virtual void gen_fp_simd_cvt_sh(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode) = 0;
    /**
     * \brief Generates an FP SIMD covert single to unisigned word operation,,
     * \param input_val_1 A Input operand 1 
     * \param res_val A result of covert culculation
     * \param res_exp A result of exception
     * \param round_mode A round mode
     */
    virtual void gen_fp_simd_cvt_suw(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode) = 0;
    /**
     * \brief Generates an FP SIMD covert single to word operation,,
     * \param input_val_1 A Input operand 1 
     * \param res_val A result of covert culculation
     * \param res_exp A result of exception
     * \param round_mode A round mode
     */
    virtual void gen_fp_simd_cvt_sw(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp, fastiss::i8_t *round_mode) = 0;
    /**
     * \brief Generates an FP SIMD Reciprocal operation,,
     * \param input_val_1 A Input operand 1 
     * \param res_val A result of reciprocal culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_recip(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD Square Root Reciprocal operation,,
     * \param input_val_1 A Input operand 1 
     * \param res_val A result of square root reciprocal culculation
     * \param res_exp A result of exception
     * \param isReciprocal A flag for reciprocal
     */
    virtual void gen_fp_simd_rsqrt(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD Square Root operation,,
     * \param input_val_1 A Input operand 1 
     * \param res_val A result of square root  culculation
     * \param res_exp A result of exception
     * \param isReciprocal A flag for reciprocal
     */
    virtual void gen_fp_simd_sqrt(fastiss::f32_t *input_val_1, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD comparation operation,,
     * \param fcond A condition 
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of cmp culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_cmp(fastiss::i8_t *fcond, fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD Fused Multiply Add operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of fused multiply add culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_fma(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD Fused Multiply Sub operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of fused multiply sub culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_fms(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD Fused Negative Multiply Add operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of fused negative multiply add culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_fnma(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Generates an FP SIMD Fused Negative Multiply Sub operation,,
     * \param input_val_1 A Input operand 1 
     * \param input_val_2 A Input operand 2
     * \param res_val A result of fused negative multiply sub culculation
     * \param res_exp A result of exception
     */
    virtual void gen_fp_simd_fnms(fastiss::f32_t *input_val_1, fastiss::f32_t *input_val_2, fastiss::i32_t **res_val, fastiss::i8_t **res_exp) = 0;
    /**
     * \brief Check the value of PSWH.GM whether mode is host mode or guest mode.
     * If the  PSWH.GM is 0 the mode is host mode
     * \param value is none.
     * \return The true( host mode) or false (guest mode) result.
     */
    virtual fastiss::i1_t *is_host_mode(void) = 0;
    /**
     * \brief Check the value of PSW.UM whether mode is supervizer mode or user mode.
     * If the  PSW.UM is 0 the mode is supervizer mode
     * \param value is none.
     * \return The true( supervizer mode) or false (user mode) result.
     */
    virtual fastiss::i1_t *is_supervizer_mode(void) = 0;
    /**
     * \brief Check the value of HVCFG.HVE whether Hardware-assisted Virtualization is valid or not.
     * If the HVCFG.HVE is 1 the Hardware-assisted Virtualization is valid.
     * \param value is none.
     * \return The true(Hardware-assisted Virtualization is valid) or false (Hardware-assisted Virtualization is invalid) result.
     */
    virtual fastiss::i1_t *is_valid_virtualization(void) = 0;
    /**
     * \brief Cuase the suitable exeption for the Virtualization fuciton.
     * If the HVCFG.HVE is 0 (the Hardware-assisted Virtualization is invalid), cause the RIE exception.
     * If the PSWH.GM = 1 (guest mode) or PSW.UM is 1 (user mode), cause the PIE exception.
     * \param cur_pc A current PC value
     * \param base_addr A base address of the exeption handler
     * \param exept_code A exeption code
     * \param current_pswh_gm A previous CPU mode
     * \return value is nothing.
     */
    virtual void gen_pie_rie_exept(fastiss::i32_t* curr_pc, fastiss::i32_t* base_addr, fastiss::i32_t* exept_code, fastiss::i1_t* current_pswh_gm) = 0;
    /**
     * \brief Generates a pointer of LinkManager's instance
     */
    virtual fastiss::i8_ptr_t* gen_read_link_manager_ptr(void) = 0;
    /**
     * \brief Generates a flag whether the Link-function is enable or not
     */
    virtual bool is_link_func_enable(void) = 0;
    /**
     * \brief Generates a flag whether the Hardware-assisted Virtualization is supported or not
     */
    virtual bool is_hv_func_support(void) = 0;
  };
}

#endif // _RH850G4_CUSTOM_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
