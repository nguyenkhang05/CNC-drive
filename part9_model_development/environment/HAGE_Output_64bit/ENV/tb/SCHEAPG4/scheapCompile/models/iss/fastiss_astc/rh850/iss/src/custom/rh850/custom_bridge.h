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
 * \file custom_bridge.h
 * \brief Defines the Custom Generator bridge class
 */

#ifndef _RH850_CUSTOM_BRIDGE_H_
#define _RH850_CUSTOM_BRIDGE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/custom_if.h"

//------------------------------------------------------------------------------
// rh850 custom extension bridge definition
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief Class implementing the custom extension interface that provides
   * a bridge to another implementation of the custom extension interface.
   * 
   * Essentially this class re-directs all operations called on it to
   * the corresponding operation of the specified implementation. This
   * is used to ensure that instances of the many classes derived from
   * this class are able to share a single underlying custom extension
   * implementation.
   * 
   * Note that the generative aspects of this interface are protected in
   * order to ensure that they are not available to helper functions.
   */

  class custom_bridge_t : public rh850::custom_if_t
  {
  public:
    /**
     * \brief Custom bridge constructor
     * \param custom_impl A pointer to the custom extension implementor
     */
    custom_bridge_t(rh850::custom_if_t *custom_impl);
    /**
     * \brief Custom bridge destructor
     */
    virtual ~custom_bridge_t();

    /**
     * \brief Determine if a condition is always (unconditional) satisfied
     * according to a condition code
     * \param condition The condition code
     * \return True if the condition code indicated that the condition is
     * always satisfied, false otherwise
     */
    bool is_condition_always(unsigned char condition);

  protected:
    /**
     * \brief Creates an exit block. This is used at the start of translating a
     * block of code to enable multiple exit points from the block.
     */
    void create_exit_block(void);
    
    /**
     * \brief Returns the exit block. This is used when a block exit point is
     * encountered during the translation of code.
     * \return The exit block
     */
    fastiss::block_t *get_exit_block(void);
    
    /**
     * \brief Generates code to increment the internal local time value.
     * \param increment Number of cycles by which to increment the internal local time value.
     */
    void gen_increment_cycle_count(fastiss::i32_t *increment);

    /**
     * \brief Notify the interrupt accepted analysis point.
     */
    void notify_interrupt_accepted(void);

    /**
     * \brief Notify the interrupt complete analysis point.
     */
    void notify_interrupt_complete(void);

    /**
     * \brief Generates a 1-bit value for the S (sign) bit of the PSW register
     * based on the 32-bit result of an operation. This allows the S calculation
     * to be standardised across all relevant instructions.
     * \param op_result The 32-bit result of an operation
     * \return A 1-bit value for the S bit of the PSW register
     */
    fastiss::i1_t *gen_calculate_PSW_S(fastiss::i32_t *op_result);
    /**
     * \brief Generates a 1-bit value for the Z (zero) bit of the PSW register
     * based on the 32-bit result of an operation. This allows the Z calculation
     * to be standardised across all relevant instructions.
     * \param op_result The 32-bit result of an operation
     * \return A 1-bit value for the Z bit of the PSW register
     */
    fastiss::i1_t *gen_calculate_PSW_Z(fastiss::i32_t *op_result);

    /**
     * \brief Generates a 1-bit value for the CY (carry) bit of the PSW register
     * based on a shift operation. This allows the CY calculation to be
     * standardised across all relevant instructions.
     * \param val A 32-bit value that is being shifted
     * \param shift A 32-bit value that is the shift amount
     * \param is_left_shift True is a left shift is being performed, false if a right shift
     * \return A 1-bit value for the CY bit of the PSW register
     */
    fastiss::i1_t *gen_calculate_PSW_CY_shift(fastiss::i32_t *val, fastiss::i32_t *shift, bool is_left_shift);

    /**
     * \brief Generates whether a condition is satisfied according to a condition
     * code and the CPU state
     * \param condition The condition code
     * \return A boolean value that is true if the condition is satisfied and
     * false otherwise
     */
    fastiss::i1_t *gen_condition_satisfied(unsigned char condition);

    /**
     * \brief Generates a division operation that also determines whether the
     * result is signed, zero, or overflow occurs. This allows the division to be
     * standardised across all relevant instructions. This function directly
     * updates all affected registers.
     * \param reg1_val A 32-bit reg1 value that is the divisor of the operation
     * \param reg2 Index of the reg2 register that is both the dividend of the
     * operation and the destination register of the divide result
     */
    void gen_div_reg1_reg2(fastiss::i32_t *reg1_val, rh850::GR_index_t reg2);
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
    void gen_div_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850::GR_index_t reg2, rh850::GR_index_t reg3);
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
    void gen_divu_reg1_reg2_reg3(fastiss::i32_t *reg1_val, rh850::GR_index_t reg2, rh850::GR_index_t reg3, bool is_halfword = false);

    /**
     * \brief Check the value whether unimplemnted exception occurs or not.
     * If the velue is denormalised value and FPSR.FS=0, the unimplemnted exception occurs.
     * \param value The 32-bit floating point value to be processed for flushing.
     * \return The true(unimplemnted exception occur) or false result.
     */
    fastiss::i1_t *is_unimplemented_exp(fastiss::f32_t *value);

    /**
     * \brief Check the value whether unimplemnted exception occurs or not.
     * If the velue is denormalised value and FPSR.FS=0, the unimplemnted exception occurs.
     * \param value The 64-bit floating point value to be processed for flushing.
     * \return The true(unimplemnted exception occur) or false result.
     */
    fastiss::i1_t *is_unimplemented_exp(fastiss::f64_t *value);

    /**
     * \brief Flush denormalised values to zero. Note that the sign of a denormalised
     * value is preserved and normal values are also preserved.
     * \param value The 32-bit floating point value to be processed for flushing.
     * \return The normal or zero result.
     */
    fastiss::f32_t *gen_flush_denorm(fastiss::f32_t *value);

    /**
     * \brief Flush denormalised values to zero. Note that the sign of a denormalised
     * value is preserved and normal values are also preserved.
     * \param value The 64-bit floating point value to be processed for flushing.
     * \return The normal or zero result.
     */
    fastiss::f64_t *gen_flush_denorm(fastiss::f64_t *value);
    
    /**
     * \brief Updates the underflow flag (and inexact flag) when a denormalised
     * value is the result of a floating point operation
     * \param op_result The 32-bit floating point result and exception flags of
     * an operation
     */
    void gen_underflow_denorm_update(fastiss::f32_with_exception_flags_t& op_result);

    /**
     * \brief Updates the underflow flag (and inexact flag) when a denormalised
     * value is the result of a floating point operation
     * \param op_result The 64-bit floating point result and exception flags of
     * an operation
     */
    void gen_underflow_denorm_update(fastiss::f64_with_exception_flags_t& op_result);

    /**
     * \brief Updates the underflow flag (and inexact flag) when a denormalised
     * value is the result of a floating point operation
     * \param op_result The 80-bit floating point result and exception flags of
     * an operation
     */
    void gen_underflow_denorm_update(fastiss::f80_with_exception_flags_t& op_result);

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
    fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg2);

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
    fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg2_pair);

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
    fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2);

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
    fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f64_t *reg1_pair, fastiss::f64_t *reg2_pair);

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
    fastiss::i1_t *gen_inexact_denorm_update(fastiss::i1_t *invalid, fastiss::i1_t *divide_by_zero, fastiss::i1_t *inexact, fastiss::f32_t *reg1, fastiss::f32_t *reg2, fastiss::f32_t *reg3);    

    /**
     * \brief Determines the inexact flag when either an enabled overflow
     * exception or an enabled underflow exception has resulted from a floating
     * point operation
     * \param overflow The 1-bit overflow flag from a floating point operation
     * \param underflow The 1-bit underflow flag from a floating point operation
     * \param inexact The 1-bit inexact flag from a floating point operation
     * \return The inexact flag result
     */
    fastiss::i1_t *gen_inexact_enabled_overflow_underflow_update(fastiss::i1_t *overflow, fastiss::i1_t *underflow, fastiss::i1_t *inexact);

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
    void gen_update_dest_and_fpsr(bool is_single_precision, rh850::GR_index_t dest_index, fastiss::i32_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block);

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
    void gen_update_dest_and_fpsr(bool is_single_precision, rh850::GR_index_t dest_index, fastiss::i64_t* dest_val, fastiss::i8_t* exception_flags, fastiss::block_t *continue_block);

    /**
     * \brief Generates get round mode from FPSR.RM,
     */
    fastiss::i8_t *gen_read_round_mode(void);

    /**
     * \brief Generates clear FPI flag,
     */
    void gen_clear_fpi_flag(void);

    /**
     * \brief Generates an bins operation,
     * \param reg1 Index of the reg1 register
     * \param reg2 Index of the reg2 register
     * \param msb_val A 32-bit msb value
     * \param lsb_val A 32-bit lsb value
     */
    void gen_bins_common(rh850::GR_index_t reg1, rh850::GR_index_t reg2, fastiss::i32_t *msb_val, fastiss::i32_t *lsb_val);

    /**
     * \brief Generates an sifht operation,,
     * \param x_val A 32-bit value
     * \param l_val A 32-bit value
     * \param r_val A 32-bit value
     */
    fastiss::i32_t *gen_bit_extract(fastiss::i32_t *x_val, fastiss::i32_t *l_val, fastiss::i32_t *r_val);

    /**
     * \brief Generates a pointer of LinkManager's instance
     */
    fastiss::i8_ptr_t* gen_read_link_manager_ptr(void);

    /**
     * \brief Generates a flag whether the Link-function is enable or not
     */
    bool is_link_func_enable(void);

    /**
     * \brief Generates a flag whether the Link-function is enable or not
     */
    unsigned int get_cpu_milestone(void);
    /**
     * \brief Generates a flag whether the MDP-function is enable or not
     */
    bool is_mdp_enable(void);
    /**
     * \set a flag whether the MDP-function is enable or not
     */
    void set_mdp_enable(bool is_mdp);

    /**
     * \brief A pointer to the custom extension implementor
     */
    rh850::custom_if_t *m_custom_impl;
  };
}

#endif // _RH850_CUSTOM_BRIDGE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
