//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file cpu_state_bridge.h
 * \brief Defines the rh850g4 CPU state bridge class
 */

#ifndef _RH850G4_CPU_STATE_BRIDGE_H_
#define _RH850G4_CPU_STATE_BRIDGE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/cpu_state_if.h"

//------------------------------------------------------------------------------
// rh850g4 CPU state bridge definition - redirects calls on the CPU State
// Interface to the CPU state implementation.
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850g4 specific components
 */
namespace rh850g4
{  
  /**
   * \brief Class implementing the CPU state interface that provides
   * a bridge to another implementation of CPU state interface.
   * 
   * Essentially this class re-directs all operations called on it to
   * the corresponding operation of the specified implementation. This
   * is used to ensure that instances of the many classes derived from
   * this class are able to share a single underlying CPU state
   * implementation.
   * 
   * Note that the generative aspects of this interface are protected in
   * order to ensure that they are not available to helper functions.
   */
  class cpu_state_bridge_t : public cpu_state_if_t
  {
  public:
    /**
     * \brief CPU state bridge constructor
     * \param cpu_state_impl Handle to CPU state implementation to
     * which calls on the created CPU state interface will be directed.
     */
    cpu_state_bridge_t(rh850g4::cpu_state_if_t *cpu_state_impl);
    /**
     * \brief CPU state bridge destructor
     */
    virtual ~cpu_state_bridge_t();

    // Immediate instruction count operations
    /**
     * \brief Queries the number of instructions executed in the current quanta
     * \return Unsigned integer value representing the number of instructions executed in the current quanta 
     */
    unsigned int read_instruction_count(void);
    /**
     * \brief Set the number of instructions executed in the current quanta
     * \param value Unsigned integer value representing the new number of instructions executed in the current quanta 
     */
    void write_instruction_count(unsigned int value);

    // Immediate time operations
    /**
     * \brief Queries the time executed in the current quanta
     * \return Unsigned integer value representing the time executed in the current quanta
     */
    unsigned long long read_time(void);
    /**
     * \brief Set the time in the current quanta
     * \param value Unsigned integer value representing the new number of instructions executed in the current quanta
     */
    void write_time(unsigned long long value);
    /**
     * \brief Increment the time executed in the current quanta by the specified amount
     * \param increment Amount by which to increment the time executed in the current quanta
     */
    void increment_time(unsigned long long increment);
    /**
     * \brief Decrement the time executed in the current quanta by the specified amount
     * \param decrement Amount by which to decrement the time executed in the current quanta
     */
    void decrement_time(unsigned long long decrement);
    /**
     * \brief Update the accrued time value. The accrued time value is used
     * during TLM transactions to keep track of the part of local time that has
     * been 'accrued' against global time.
     * \param accrued_time New accrued time value
     */
    void write_accrued_time(unsigned long long accrued_time);
    /**
     * \brief Read the accrued time value. The accrued time value is used
     * during TLM transactions to keep track of the part of local time that has
     * been 'accrued' against global time.
     * \return Value representing the accrued time
     */
    unsigned long long read_accrued_time(void);

    // Immediate cycle period operations
    /**
     * \brief Queries the period of time taken to execute a cycle
     * \return Unsigned integer value representing the period of time taken to execute a cycle
     */
    unsigned long long read_cycle_period(void);
    /**
     * \brief Set the period of time taken to execute a cycle
     * \param value Unsigned integer value representing the period of time taken to execute a cycle
     */
    void write_cycle_period(unsigned long long value);
    /**
     * \brief Increment the period of time taken to execute a cycle by the specified amount
     * \param increment Amount by which to increment the period of time taken to execute a cycle
     */
    void increment_cycle_period(unsigned long long increment);
    /**
     * \brief Decrement the period of time taken to execute a cycle by the specified amount
     * \param decrement Amount by which to decrement the period of time taken to execute a cycle
     */
    void decrement_cycle_period(unsigned long long decrement);

    /**
     * \brief Queries the value of the pointer to the core instance
     * \return Value representing the pointer to the core instance
     */
    void* read_core_ptr(void);
    /**
     * \brief Set the value of the pointer to the core instance
     * \param value Value representing the pointer to the core instance
     */
    void write_core_ptr(void* value);

    /**
     * \brief Queries the value of the check watchpoint hit flag
     * \return Value representing the check watchpoint hit flag
     */
    unsigned char read_check_watchpoint_hit(void);
    /**
     * \brief Set the value of the check watchpoint hit flag
     * \param value Value representing the check watchpoint hit flag
     */
    void write_check_watchpoint_hit(unsigned char value);
    /**
     * \brief Queries the value of the watchpoint hit flag
     * \return Value representing the watchpoint hit flag
     */
    unsigned char read_watchpoint_hit(void);
    /**
     * \brief Set the value of the watchpoint hit flag
     * \param value Value representing the watchpoint hit flag
     */
    void write_watchpoint_hit(unsigned char value);
    
    // Register name operations
    /**
     * \brief Generates string representing name of the rh850g4 r0 register
     * \return String representing the name of the rh850g4 r0 register
     */
    std::string register_name_r0(void);
    /**
     * \brief Generates string representing name of the rh850g4 r1 register
     * \return String representing the name of the rh850g4 r1 register
     */
    std::string register_name_r1(void);
    /**
     * \brief Generates string representing name of the rh850g4 r2 register
     * \return String representing the name of the rh850g4 r2 register
     */
    std::string register_name_r2(void);
    /**
     * \brief Generates string representing name of the rh850g4 r3 register
     * \return String representing the name of the rh850g4 r3 register
     */
    std::string register_name_r3(void);
    /**
     * \brief Generates string representing name of the rh850g4 r4 register
     * \return String representing the name of the rh850g4 r4 register
     */
    std::string register_name_r4(void);
    /**
     * \brief Generates string representing name of the rh850g4 r5 register
     * \return String representing the name of the rh850g4 r5 register
     */
    std::string register_name_r5(void);
    /**
     * \brief Generates string representing name of the rh850g4 r6 register
     * \return String representing the name of the rh850g4 r6 register
     */
    std::string register_name_r6(void);
    /**
     * \brief Generates string representing name of the rh850g4 r7 register
     * \return String representing the name of the rh850g4 r7 register
     */
    std::string register_name_r7(void);
    /**
     * \brief Generates string representing name of the rh850g4 r8 register
     * \return String representing the name of the rh850g4 r8 register
     */
    std::string register_name_r8(void);
    /**
     * \brief Generates string representing name of the rh850g4 r9 register
     * \return String representing the name of the rh850g4 r9 register
     */
    std::string register_name_r9(void);
    /**
     * \brief Generates string representing name of the rh850g4 r10 register
     * \return String representing the name of the rh850g4 r10 register
     */
    std::string register_name_r10(void);
    /**
     * \brief Generates string representing name of the rh850g4 r11 register
     * \return String representing the name of the rh850g4 r11 register
     */
    std::string register_name_r11(void);
    /**
     * \brief Generates string representing name of the rh850g4 r12 register
     * \return String representing the name of the rh850g4 r12 register
     */
    std::string register_name_r12(void);
    /**
     * \brief Generates string representing name of the rh850g4 r13 register
     * \return String representing the name of the rh850g4 r13 register
     */
    std::string register_name_r13(void);
    /**
     * \brief Generates string representing name of the rh850g4 r14 register
     * \return String representing the name of the rh850g4 r14 register
     */
    std::string register_name_r14(void);
    /**
     * \brief Generates string representing name of the rh850g4 r15 register
     * \return String representing the name of the rh850g4 r15 register
     */
    std::string register_name_r15(void);
    /**
     * \brief Generates string representing name of the rh850g4 r16 register
     * \return String representing the name of the rh850g4 r16 register
     */
    std::string register_name_r16(void);
    /**
     * \brief Generates string representing name of the rh850g4 r17 register
     * \return String representing the name of the rh850g4 r17 register
     */
    std::string register_name_r17(void);
    /**
     * \brief Generates string representing name of the rh850g4 r18 register
     * \return String representing the name of the rh850g4 r18 register
     */
    std::string register_name_r18(void);
    /**
     * \brief Generates string representing name of the rh850g4 r19 register
     * \return String representing the name of the rh850g4 r19 register
     */
    std::string register_name_r19(void);
    /**
     * \brief Generates string representing name of the rh850g4 r20 register
     * \return String representing the name of the rh850g4 r20 register
     */
    std::string register_name_r20(void);
    /**
     * \brief Generates string representing name of the rh850g4 r21 register
     * \return String representing the name of the rh850g4 r21 register
     */
    std::string register_name_r21(void);
    /**
     * \brief Generates string representing name of the rh850g4 r22 register
     * \return String representing the name of the rh850g4 r22 register
     */
    std::string register_name_r22(void);
    /**
     * \brief Generates string representing name of the rh850g4 r23 register
     * \return String representing the name of the rh850g4 r23 register
     */
    std::string register_name_r23(void);
    /**
     * \brief Generates string representing name of the rh850g4 r24 register
     * \return String representing the name of the rh850g4 r24 register
     */
    std::string register_name_r24(void);
    /**
     * \brief Generates string representing name of the rh850g4 r25 register
     * \return String representing the name of the rh850g4 r25 register
     */
    std::string register_name_r25(void);
    /**
     * \brief Generates string representing name of the rh850g4 r26 register
     * \return String representing the name of the rh850g4 r26 register
     */
    std::string register_name_r26(void);
    /**
     * \brief Generates string representing name of the rh850g4 r27 register
     * \return String representing the name of the rh850g4 r27 register
     */
    std::string register_name_r27(void);
    /**
     * \brief Generates string representing name of the rh850g4 r28 register
     * \return String representing the name of the rh850g4 r28 register
     */
    std::string register_name_r28(void);
    /**
     * \brief Generates string representing name of the rh850g4 r29 register
     * \return String representing the name of the rh850g4 r29 register
     */
    std::string register_name_r29(void);
    /**
     * \brief Generates string representing name of the rh850g4 r30 register
     * \return String representing the name of the rh850g4 r30 register
     */
    std::string register_name_r30(void);
    /**
     * \brief Generates string representing name of the rh850g4 r31 register
     * \return String representing the name of the rh850g4 r31 register
     */
    std::string register_name_r31(void);
    /**
     * \brief Generates string representing name of the rh850g4 PC register
     * \return String representing the name of the rh850g4 PC register
     */
    std::string register_name_PC(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr0 register
     * \return String representing the name of the rh850g4 wr0 register
     */
    std::string register_name_wr0(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr0_u register
     * \return String representing the name of the rh850g4 wr0_u register
     */
    std::string register_name_wr0_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr1 register
     * \return String representing the name of the rh850g4 wr1 register
     */
    std::string register_name_wr1(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr1_u register
     * \return String representing the name of the rh850g4 wr1_u register
     */
    std::string register_name_wr1_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr2 register
     * \return String representing the name of the rh850g4 wr2 register
     */
    std::string register_name_wr2(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr2_u register
     * \return String representing the name of the rh850g4 wr2_u register
     */
    std::string register_name_wr2_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr3 register
     * \return String representing the name of the rh850g4 wr3 register
     */
    std::string register_name_wr3(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr3_u register
     * \return String representing the name of the rh850g4 wr3_u register
     */
    std::string register_name_wr3_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr4 register
     * \return String representing the name of the rh850g4 wr4 register
     */
    std::string register_name_wr4(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr4_u register
     * \return String representing the name of the rh850g4 wr4_u register
     */
    std::string register_name_wr4_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr5 register
     * \return String representing the name of the rh850g4 wr5 register
     */
    std::string register_name_wr5(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr5_u register
     * \return String representing the name of the rh850g4 wr5_u register
     */
    std::string register_name_wr5_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr6 register
     * \return String representing the name of the rh850g4 wr6 register
     */
    std::string register_name_wr6(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr6_u register
     * \return String representing the name of the rh850g4 wr6_u register
     */
    std::string register_name_wr6_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr7 register
     * \return String representing the name of the rh850g4 wr7 register
     */
    std::string register_name_wr7(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr7_u register
     * \return String representing the name of the rh850g4 wr7_u register
     */
    std::string register_name_wr7_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr8 register
     * \return String representing the name of the rh850g4 wr8 register
     */
    std::string register_name_wr8(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr8_u register
     * \return String representing the name of the rh850g4 wr8_u register
     */
    std::string register_name_wr8_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr9 register
     * \return String representing the name of the rh850g4 wr9 register
     */
    std::string register_name_wr9(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr9_u register
     * \return String representing the name of the rh850g4 wr9_u register
     */
    std::string register_name_wr9_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr10 register
     * \return String representing the name of the rh850g4 wr10 register
     */
    std::string register_name_wr10(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr10_u register
     * \return String representing the name of the rh850g4 wr10_u register
     */
    std::string register_name_wr10_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr11 register
     * \return String representing the name of the rh850g4 wr11 register
     */
    std::string register_name_wr11(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr11_u register
     * \return String representing the name of the rh850g4 wr11_u register
     */
    std::string register_name_wr11_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr12 register
     * \return String representing the name of the rh850g4 wr12 register
     */
    std::string register_name_wr12(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr12_u register
     * \return String representing the name of the rh850g4 wr12_u register
     */
    std::string register_name_wr12_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr13 register
     * \return String representing the name of the rh850g4 wr13 register
     */
    std::string register_name_wr13(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr13_u register
     * \return String representing the name of the rh850g4 wr13_u register
     */
    std::string register_name_wr13_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr14 register
     * \return String representing the name of the rh850g4 wr14 register
     */
    std::string register_name_wr14(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr14_u register
     * \return String representing the name of the rh850g4 wr14_u register
     */
    std::string register_name_wr14_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr15 register
     * \return String representing the name of the rh850g4 wr15 register
     */
    std::string register_name_wr15(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr15_u register
     * \return String representing the name of the rh850g4 wr15_u register
     */
    std::string register_name_wr15_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr16 register
     * \return String representing the name of the rh850g4 wr16 register
     */
    std::string register_name_wr16(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr16_u register
     * \return String representing the name of the rh850g4 wr16_u register
     */
    std::string register_name_wr16_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr17 register
     * \return String representing the name of the rh850g4 wr17 register
     */
    std::string register_name_wr17(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr17_u register
     * \return String representing the name of the rh850g4 wr17_u register
     */
    std::string register_name_wr17_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr18 register
     * \return String representing the name of the rh850g4 wr18 register
     */
    std::string register_name_wr18(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr18_u register
     * \return String representing the name of the rh850g4 wr18_u register
     */
    std::string register_name_wr18_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr19 register
     * \return String representing the name of the rh850g4 wr19 register
     */
    std::string register_name_wr19(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr19_u register
     * \return String representing the name of the rh850g4 wr19_u register
     */
    std::string register_name_wr19_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr20 register
     * \return String representing the name of the rh850g4 wr20 register
     */
    std::string register_name_wr20(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr20_u register
     * \return String representing the name of the rh850g4 wr20_u register
     */
    std::string register_name_wr20_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr21 register
     * \return String representing the name of the rh850g4 wr21 register
     */
    std::string register_name_wr21(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr21_u register
     * \return String representing the name of the rh850g4 wr21_u register
     */
    std::string register_name_wr21_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr22 register
     * \return String representing the name of the rh850g4 wr22 register
     */
    std::string register_name_wr22(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr22_u register
     * \return String representing the name of the rh850g4 wr22_u register
     */
    std::string register_name_wr22_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr23 register
     * \return String representing the name of the rh850g4 wr23 register
     */
    std::string register_name_wr23(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr23_u register
     * \return String representing the name of the rh850g4 wr23_u register
     */
    std::string register_name_wr23_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr24 register
     * \return String representing the name of the rh850g4 wr24 register
     */
    std::string register_name_wr24(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr24_u register
     * \return String representing the name of the rh850g4 wr24_u register
     */
    std::string register_name_wr24_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr25 register
     * \return String representing the name of the rh850g4 wr25 register
     */
    std::string register_name_wr25(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr25_u register
     * \return String representing the name of the rh850g4 wr25_u register
     */
    std::string register_name_wr25_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr26 register
     * \return String representing the name of the rh850g4 wr26 register
     */
    std::string register_name_wr26(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr26_u register
     * \return String representing the name of the rh850g4 wr26_u register
     */
    std::string register_name_wr26_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr27 register
     * \return String representing the name of the rh850g4 wr27 register
     */
    std::string register_name_wr27(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr27_u register
     * \return String representing the name of the rh850g4 wr27_u register
     */
    std::string register_name_wr27_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr28 register
     * \return String representing the name of the rh850g4 wr28 register
     */
    std::string register_name_wr28(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr28_u register
     * \return String representing the name of the rh850g4 wr28_u register
     */
    std::string register_name_wr28_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr29 register
     * \return String representing the name of the rh850g4 wr29 register
     */
    std::string register_name_wr29(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr29_u register
     * \return String representing the name of the rh850g4 wr29_u register
     */
    std::string register_name_wr29_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr30 register
     * \return String representing the name of the rh850g4 wr30 register
     */
    std::string register_name_wr30(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr30_u register
     * \return String representing the name of the rh850g4 wr30_u register
     */
    std::string register_name_wr30_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr31 register
     * \return String representing the name of the rh850g4 wr31 register
     */
    std::string register_name_wr31(void);
    /**
     * \brief Generates string representing name of the rh850g4 wr31_u register
     * \return String representing the name of the rh850g4 wr31_u register
     */
    std::string register_name_wr31_u(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIPC register
     * \return String representing the name of the rh850g4 EIPC register
     */
    std::string register_name_EIPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIPSW register
     * \return String representing the name of the rh850g4 EIPSW register
     */
    std::string register_name_EIPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEPC register
     * \return String representing the name of the rh850g4 FEPC register
     */
    std::string register_name_FEPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEPSW register
     * \return String representing the name of the rh850g4 FEPSW register
     */
    std::string register_name_FEPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 PSW register
     * \return String representing the name of the rh850g4 PSW register
     */
    std::string register_name_PSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPSR register
     * \return String representing the name of the rh850g4 FPSR register
     */
    std::string register_name_FPSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPEPC register
     * \return String representing the name of the rh850g4 FPEPC register
     */
    std::string register_name_FPEPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPST register
     * \return String representing the name of the rh850g4 FPST register
     */
    std::string register_name_FPST(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPCC register
     * \return String representing the name of the rh850g4 FPCC register
     */
    std::string register_name_FPCC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FPCFG register
     * \return String representing the name of the rh850g4 FPCFG register
     */
    std::string register_name_FPCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIIC register
     * \return String representing the name of the rh850g4 EIIC register
     */
    std::string register_name_EIIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEIC register
     * \return String representing the name of the rh850g4 FEIC register
     */
    std::string register_name_FEIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 PSWH register
     * \return String representing the name of the rh850g4 PSWH register
     */
    std::string register_name_PSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 CTPC register
     * \return String representing the name of the rh850g4 CTPC register
     */
    std::string register_name_CTPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 CTPSW register
     * \return String representing the name of the rh850g4 CTPSW register
     */
    std::string register_name_CTPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIPSWH register
     * \return String representing the name of the rh850g4 EIPSWH register
     */
    std::string register_name_EIPSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEPSWH register
     * \return String representing the name of the rh850g4 FEPSWH register
     */
    std::string register_name_FEPSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 CTBP register
     * \return String representing the name of the rh850g4 CTBP register
     */
    std::string register_name_CTBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 SNZCFG register
     * \return String representing the name of the rh850g4 SNZCFG register
     */
    std::string register_name_SNZCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 EIWR register
     * \return String representing the name of the rh850g4 EIWR register
     */
    std::string register_name_EIWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FEWR register
     * \return String representing the name of the rh850g4 FEWR register
     */
    std::string register_name_FEWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 SPID register
     * \return String representing the name of the rh850g4 SPID register
     */
    std::string register_name_SPID(void);
    /**
     * \brief Generates string representing name of the rh850g4 SPIDLIST register
     * \return String representing the name of the rh850g4 SPIDLIST register
     */
    std::string register_name_SPIDLIST(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBASE register
     * \return String representing the name of the rh850g4 RBASE register
     */
    std::string register_name_RBASE(void);
    /**
     * \brief Generates string representing name of the rh850g4 EBASE register
     * \return String representing the name of the rh850g4 EBASE register
     */
    std::string register_name_EBASE(void);
    /**
     * \brief Generates string representing name of the rh850g4 INTBP register
     * \return String representing the name of the rh850g4 INTBP register
     */
    std::string register_name_INTBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCTL register
     * \return String representing the name of the rh850g4 MCTL register
     */
    std::string register_name_MCTL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PID register
     * \return String representing the name of the rh850g4 PID register
     */
    std::string register_name_PID(void);
    /**
     * \brief Generates string representing name of the rh850g4 SVLOCK register
     * \return String representing the name of the rh850g4 SVLOCK register
     */
    std::string register_name_SVLOCK(void);
    /**
     * \brief Generates string representing name of the rh850g4 SCCFG register
     * \return String representing the name of the rh850g4 SCCFG register
     */
    std::string register_name_SCCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 SCBP register
     * \return String representing the name of the rh850g4 SCBP register
     */
    std::string register_name_SCBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 HVCFG register
     * \return String representing the name of the rh850g4 HVCFG register
     */
    std::string register_name_HVCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMCFG register
     * \return String representing the name of the rh850g4 GMCFG register
     */
    std::string register_name_GMCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 HVSB register
     * \return String representing the name of the rh850g4 HVSB register
     */
    std::string register_name_HVSB(void);
    /**
     * \brief Generates string representing name of the rh850g4 PEID register
     * \return String representing the name of the rh850g4 PEID register
     */
    std::string register_name_PEID(void);
    /**
     * \brief Generates string representing name of the rh850g4 BMID register
     * \return String representing the name of the rh850g4 BMID register
     */
    std::string register_name_BMID(void);
    /**
     * \brief Generates string representing name of the rh850g4 MEA register
     * \return String representing the name of the rh850g4 MEA register
     */
    std::string register_name_MEA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MEI register
     * \return String representing the name of the rh850g4 MEI register
     */
    std::string register_name_MEI(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBCR0 register
     * \return String representing the name of the rh850g4 RBCR0 register
     */
    std::string register_name_RBCR0(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBCR1 register
     * \return String representing the name of the rh850g4 RBCR1 register
     */
    std::string register_name_RBCR1(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBNR register
     * \return String representing the name of the rh850g4 RBNR register
     */
    std::string register_name_RBNR(void);
    /**
     * \brief Generates string representing name of the rh850g4 RBIP register
     * \return String representing the name of the rh850g4 RBIP register
     */
    std::string register_name_RBIP(void);
    /**
     * \brief Generates string representing name of the rh850g4 ISPR register
     * \return String representing the name of the rh850g4 ISPR register
     */
    std::string register_name_ISPR(void);
    /**
     * \brief Generates string representing name of the rh850g4 IMSR register
     * \return String representing the name of the rh850g4 IMSR register
     */
    std::string register_name_IMSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICSR register
     * \return String representing the name of the rh850g4 ICSR register
     */
    std::string register_name_ICSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 INTCFG register
     * \return String representing the name of the rh850g4 INTCFG register
     */
    std::string register_name_INTCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 PLMR register
     * \return String representing the name of the rh850g4 PLMR register
     */
    std::string register_name_PLMR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXSR register
     * \return String representing the name of the rh850g4 FXSR register
     */
    std::string register_name_FXSR(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXST register
     * \return String representing the name of the rh850g4 FXST register
     */
    std::string register_name_FXST(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXINFO register
     * \return String representing the name of the rh850g4 FXINFO register
     */
    std::string register_name_FXINFO(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXCFG register
     * \return String representing the name of the rh850g4 FXCFG register
     */
    std::string register_name_FXCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXXC register
     * \return String representing the name of the rh850g4 FXXC register
     */
    std::string register_name_FXXC(void);
    /**
     * \brief Generates string representing name of the rh850g4 FXXP register
     * \return String representing the name of the rh850g4 FXXP register
     */
    std::string register_name_FXXP(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIPC register
     * \return String representing the name of the rh850g4 GMEIPC register
     */
    std::string register_name_GMEIPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIPSW register
     * \return String representing the name of the rh850g4 GMEIPSW register
     */
    std::string register_name_GMEIPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEPC register
     * \return String representing the name of the rh850g4 GMFEPC register
     */
    std::string register_name_GMFEPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEPSW register
     * \return String representing the name of the rh850g4 GMFEPSW register
     */
    std::string register_name_GMFEPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMPSW register
     * \return String representing the name of the rh850g4 GMPSW register
     */
    std::string register_name_GMPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMMEA register
     * \return String representing the name of the rh850g4 GMMEA register
     */
    std::string register_name_GMMEA(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMMEI register
     * \return String representing the name of the rh850g4 GMMEI register
     */
    std::string register_name_GMMEI(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIIC register
     * \return String representing the name of the rh850g4 GMEIIC register
     */
    std::string register_name_GMEIIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEIC register
     * \return String representing the name of the rh850g4 GMFEIC register
     */
    std::string register_name_GMFEIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMSPID register
     * \return String representing the name of the rh850g4 GMSPID register
     */
    std::string register_name_GMSPID(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMSPIDLIST register
     * \return String representing the name of the rh850g4 GMSPIDLIST register
     */
    std::string register_name_GMSPIDLIST(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEBASE register
     * \return String representing the name of the rh850g4 GMEBASE register
     */
    std::string register_name_GMEBASE(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMINTBP register
     * \return String representing the name of the rh850g4 GMINTBP register
     */
    std::string register_name_GMINTBP(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMINTCFG register
     * \return String representing the name of the rh850g4 GMINTCFG register
     */
    std::string register_name_GMINTCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMPLMR register
     * \return String representing the name of the rh850g4 GMPLMR register
     */
    std::string register_name_GMPLMR(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMSVLOCK register
     * \return String representing the name of the rh850g4 GMSVLOCK register
     */
    std::string register_name_GMSVLOCK(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMMPM register
     * \return String representing the name of the rh850g4 GMMPM register
     */
    std::string register_name_GMMPM(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMEIWR register
     * \return String representing the name of the rh850g4 GMEIWR register
     */
    std::string register_name_GMEIWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMFEWR register
     * \return String representing the name of the rh850g4 GMFEWR register
     */
    std::string register_name_GMFEWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 GMPEID register
     * \return String representing the name of the rh850g4 GMPEID register
     */
    std::string register_name_GMPEID(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPM register
     * \return String representing the name of the rh850g4 MPM register
     */
    std::string register_name_MPM(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPCFG register
     * \return String representing the name of the rh850g4 MPCFG register
     */
    std::string register_name_MPCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCA register
     * \return String representing the name of the rh850g4 MCA register
     */
    std::string register_name_MCA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCS register
     * \return String representing the name of the rh850g4 MCS register
     */
    std::string register_name_MCS(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCC register
     * \return String representing the name of the rh850g4 MCC register
     */
    std::string register_name_MCC(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCR register
     * \return String representing the name of the rh850g4 MCR register
     */
    std::string register_name_MCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 MCI register
     * \return String representing the name of the rh850g4 MCI register
     */
    std::string register_name_MCI(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPIDX register
     * \return String representing the name of the rh850g4 MPIDX register
     */
    std::string register_name_MPIDX(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPBK register
     * \return String representing the name of the rh850g4 MPBK register
     */
    std::string register_name_MPBK(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPLA register
     * \return String representing the name of the rh850g4 MPLA register
     */
    std::string register_name_MPLA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPUA register
     * \return String representing the name of the rh850g4 MPUA register
     */
    std::string register_name_MPUA(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPAT register
     * \return String representing the name of the rh850g4 MPAT register
     */
    std::string register_name_MPAT(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID0 register
     * \return String representing the name of the rh850g4 MPID0 register
     */
    std::string register_name_MPID0(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID1 register
     * \return String representing the name of the rh850g4 MPID1 register
     */
    std::string register_name_MPID1(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID2 register
     * \return String representing the name of the rh850g4 MPID2 register
     */
    std::string register_name_MPID2(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID3 register
     * \return String representing the name of the rh850g4 MPID3 register
     */
    std::string register_name_MPID3(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID4 register
     * \return String representing the name of the rh850g4 MPID4 register
     */
    std::string register_name_MPID4(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID5 register
     * \return String representing the name of the rh850g4 MPID5 register
     */
    std::string register_name_MPID5(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID6 register
     * \return String representing the name of the rh850g4 MPID6 register
     */
    std::string register_name_MPID6(void);
    /**
     * \brief Generates string representing name of the rh850g4 MPID7 register
     * \return String representing the name of the rh850g4 MPID7 register
     */
    std::string register_name_MPID7(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICTAGL register
     * \return String representing the name of the rh850g4 ICTAGL register
     */
    std::string register_name_ICTAGL(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICTAGH register
     * \return String representing the name of the rh850g4 ICTAGH register
     */
    std::string register_name_ICTAGH(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICDATL register
     * \return String representing the name of the rh850g4 ICDATL register
     */
    std::string register_name_ICDATL(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICDATH register
     * \return String representing the name of the rh850g4 ICDATH register
     */
    std::string register_name_ICDATH(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICCTRL register
     * \return String representing the name of the rh850g4 ICCTRL register
     */
    std::string register_name_ICCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICCFG register
     * \return String representing the name of the rh850g4 ICCFG register
     */
    std::string register_name_ICCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICERR register
     * \return String representing the name of the rh850g4 ICERR register
     */
    std::string register_name_ICERR(void);
    /**
     * \brief Generates string representing name of the rh850g4 TSCOUNTL register
     * \return String representing the name of the rh850g4 TSCOUNTL register
     */
    std::string register_name_TSCOUNTL(void);
    /**
     * \brief Generates string representing name of the rh850g4 TSCOUNTH register
     * \return String representing the name of the rh850g4 TSCOUNTH register
     */
    std::string register_name_TSCOUNTH(void);
    /**
     * \brief Generates string representing name of the rh850g4 TSCTRL register
     * \return String representing the name of the rh850g4 TSCTRL register
     */
    std::string register_name_TSCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMUMCTRL register
     * \return String representing the name of the rh850g4 PMUMCTRL register
     */
    std::string register_name_PMUMCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMGMCTRL register
     * \return String representing the name of the rh850g4 PMGMCTRL register
     */
    std::string register_name_PMGMCTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT0 register
     * \return String representing the name of the rh850g4 PMCOUNT0 register
     */
    std::string register_name_PMCOUNT0(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL0 register
     * \return String representing the name of the rh850g4 PMCTRL0 register
     */
    std::string register_name_PMCTRL0(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT1 register
     * \return String representing the name of the rh850g4 PMCOUNT1 register
     */
    std::string register_name_PMCOUNT1(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL1 register
     * \return String representing the name of the rh850g4 PMCTRL1 register
     */
    std::string register_name_PMCTRL1(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT2 register
     * \return String representing the name of the rh850g4 PMCOUNT2 register
     */
    std::string register_name_PMCOUNT2(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL2 register
     * \return String representing the name of the rh850g4 PMCTRL2 register
     */
    std::string register_name_PMCTRL2(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT3 register
     * \return String representing the name of the rh850g4 PMCOUNT3 register
     */
    std::string register_name_PMCOUNT3(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL3 register
     * \return String representing the name of the rh850g4 PMCTRL3 register
     */
    std::string register_name_PMCTRL3(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL4 register
     * \return String representing the name of the rh850g4 PMCTRL4 register
     */
    std::string register_name_PMCTRL4(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL5 register
     * \return String representing the name of the rh850g4 PMCTRL5 register
     */
    std::string register_name_PMCTRL5(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL6 register
     * \return String representing the name of the rh850g4 PMCTRL6 register
     */
    std::string register_name_PMCTRL6(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCTRL7 register
     * \return String representing the name of the rh850g4 PMCTRL7 register
     */
    std::string register_name_PMCTRL7(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT4 register
     * \return String representing the name of the rh850g4 PMCOUNT4 register
     */
    std::string register_name_PMCOUNT4(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT5 register
     * \return String representing the name of the rh850g4 PMCOUNT5 register
     */
    std::string register_name_PMCOUNT5(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT6 register
     * \return String representing the name of the rh850g4 PMCOUNT6 register
     */
    std::string register_name_PMCOUNT6(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMCOUNT7 register
     * \return String representing the name of the rh850g4 PMCOUNT7 register
     */
    std::string register_name_PMCOUNT7(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND0 register
     * \return String representing the name of the rh850g4 PMSUBCND0 register
     */
    std::string register_name_PMSUBCND0(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND1 register
     * \return String representing the name of the rh850g4 PMSUBCND1 register
     */
    std::string register_name_PMSUBCND1(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND2 register
     * \return String representing the name of the rh850g4 PMSUBCND2 register
     */
    std::string register_name_PMSUBCND2(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND3 register
     * \return String representing the name of the rh850g4 PMSUBCND3 register
     */
    std::string register_name_PMSUBCND3(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND4 register
     * \return String representing the name of the rh850g4 PMSUBCND4 register
     */
    std::string register_name_PMSUBCND4(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND5 register
     * \return String representing the name of the rh850g4 PMSUBCND5 register
     */
    std::string register_name_PMSUBCND5(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND6 register
     * \return String representing the name of the rh850g4 PMSUBCND6 register
     */
    std::string register_name_PMSUBCND6(void);
    /**
     * \brief Generates string representing name of the rh850g4 PMSUBCND7 register
     * \return String representing the name of the rh850g4 PMSUBCND7 register
     */
    std::string register_name_PMSUBCND7(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBGEN register
     * \return String representing the name of the rh850g4 DBGEN register
     */
    std::string register_name_DBGEN(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBPSWH register
     * \return String representing the name of the rh850g4 DBPSWH register
     */
    std::string register_name_DBPSWH(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBIC register
     * \return String representing the name of the rh850g4 DBIC register
     */
    std::string register_name_DBIC(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBPC register
     * \return String representing the name of the rh850g4 DBPC register
     */
    std::string register_name_DBPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBPSW register
     * \return String representing the name of the rh850g4 DBPSW register
     */
    std::string register_name_DBPSW(void);
    /**
     * \brief Generates string representing name of the rh850g4 DIR0 register
     * \return String representing the name of the rh850g4 DIR0 register
     */
    std::string register_name_DIR0(void);
    /**
     * \brief Generates string representing name of the rh850g4 DIR1 register
     * \return String representing the name of the rh850g4 DIR1 register
     */
    std::string register_name_DIR1(void);
    /**
     * \brief Generates string representing name of the rh850g4 BPC register
     * \return String representing the name of the rh850g4 BPC register
     */
    std::string register_name_BPC(void);
    /**
     * \brief Generates string representing name of the rh850g4 BPAV register
     * \return String representing the name of the rh850g4 BPAV register
     */
    std::string register_name_BPAV(void);
    /**
     * \brief Generates string representing name of the rh850g4 BPAM register
     * \return String representing the name of the rh850g4 BPAM register
     */
    std::string register_name_BPAM(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBCFG register
     * \return String representing the name of the rh850g4 DBCFG register
     */
    std::string register_name_DBCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 DBWR register
     * \return String representing the name of the rh850g4 DBWR register
     */
    std::string register_name_DBWR(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSTEST0 register
     * \return String representing the name of the rh850g4 LSTEST0 register
     */
    std::string register_name_LSTEST0(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSTEST1 register
     * \return String representing the name of the rh850g4 LSTEST1 register
     */
    std::string register_name_LSTEST1(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSCFG register
     * \return String representing the name of the rh850g4 LSCFG register
     */
    std::string register_name_LSCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 ICBKEY register
     * \return String representing the name of the rh850g4 ICBKEY register
     */
    std::string register_name_ICBKEY(void);
    /**
     * \brief Generates string representing name of the rh850g4 IFCR register
     * \return String representing the name of the rh850g4 IFCR register
     */
    std::string register_name_IFCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 IFCR1 register
     * \return String representing the name of the rh850g4 IFCR1 register
     */
    std::string register_name_IFCR1(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPCTRL0 register
     * \return String representing the name of the rh850g4 BRPCTRL0 register
     */
    std::string register_name_BRPCTRL0(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPCTRL1 register
     * \return String representing the name of the rh850g4 BRPCTRL1 register
     */
    std::string register_name_BRPCTRL1(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPCFG register
     * \return String representing the name of the rh850g4 BRPCFG register
     */
    std::string register_name_BRPCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPACTRL register
     * \return String representing the name of the rh850g4 BRPACTRL register
     */
    std::string register_name_BRPACTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 BRPDATA register
     * \return String representing the name of the rh850g4 BRPDATA register
     */
    std::string register_name_BRPDATA(void);
    /**
     * \brief Generates string representing name of the rh850g4 DCBKEY register
     * \return String representing the name of the rh850g4 DCBKEY register
     */
    std::string register_name_DCBKEY(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSUCR register
     * \return String representing the name of the rh850g4 LSUCR register
     */
    std::string register_name_LSUCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSULNK0 register
     * \return String representing the name of the rh850g4 LSULNK0 register
     */
    std::string register_name_LSULNK0(void);
    /**
     * \brief Generates string representing name of the rh850g4 LSULNK1 register
     * \return String representing the name of the rh850g4 LSULNK1 register
     */
    std::string register_name_LSULNK1(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RLCR register
     * \return String representing the name of the rh850g4 L1RLCR register
     */
    std::string register_name_L1RLCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RLNK0 register
     * \return String representing the name of the rh850g4 L1RLNK0 register
     */
    std::string register_name_L1RLNK0(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RLNK1 register
     * \return String representing the name of the rh850g4 L1RLNK1 register
     */
    std::string register_name_L1RLNK1(void);
    /**
     * \brief Generates string representing name of the rh850g4 L1RCFG register
     * \return String representing the name of the rh850g4 L1RCFG register
     */
    std::string register_name_L1RCFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 DECFG register
     * \return String representing the name of the rh850g4 DECFG register
     */
    std::string register_name_DECFG(void);
    /**
     * \brief Generates string representing name of the rh850g4 DECTRL register
     * \return String representing the name of the rh850g4 DECTRL register
     */
    std::string register_name_DECTRL(void);
    /**
     * \brief Generates string representing name of the rh850g4 DEVDS register
     * \return String representing the name of the rh850g4 DEVDS register
     */
    std::string register_name_DEVDS(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBCR register
     * \return String representing the name of the rh850g4 RDBCR register
     */
    std::string register_name_RDBCR(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBACR register
     * \return String representing the name of the rh850g4 RDBACR register
     */
    std::string register_name_RDBACR(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBATAG register
     * \return String representing the name of the rh850g4 RDBATAG register
     */
    std::string register_name_RDBATAG(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT0 register
     * \return String representing the name of the rh850g4 RDBADAT0 register
     */
    std::string register_name_RDBADAT0(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT1 register
     * \return String representing the name of the rh850g4 RDBADAT1 register
     */
    std::string register_name_RDBADAT1(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT2 register
     * \return String representing the name of the rh850g4 RDBADAT2 register
     */
    std::string register_name_RDBADAT2(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBADAT3 register
     * \return String representing the name of the rh850g4 RDBADAT3 register
     */
    std::string register_name_RDBADAT3(void);
    /**
     * \brief Generates string representing name of the rh850g4 RDBSTAT register
     * \return String representing the name of the rh850g4 RDBSTAT register
     */
    std::string register_name_RDBSTAT(void);
    /**
     * \brief Generates string representing name of the rh850g4 hv_func_support register
     * \return String representing the name of the rh850g4 hv_func_support register
     */
    std::string register_name_hv_func_support(void);
    /**
     * \brief Generates string representing name of the rh850g4 swd0pcad_exec_count register
     * \return String representing the name of the rh850g4 swd0pcad_exec_count register
     */
    std::string register_name_swd0pcad_exec_count(void);
    /**
     * \brief Generates string representing name of the rh850g4 swd0pcad_val register
     * \return String representing the name of the rh850g4 swd0pcad_val register
     */
    std::string register_name_swd0pcad_val(void);
    /**
     * \brief Generates string representing name of the rh850g4 mem_acc_type register
     * \return String representing the name of the rh850g4 mem_acc_type register
     */
    std::string register_name_mem_acc_type(void);
    
    // Register group name operations
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return String representing the name of the specified rh850g4 GR group register
     */
    std::string register_name_GR(rh850g4::GR_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return String representing the name of the specified rh850g4 WR group register
     */
    std::string register_name_WR(rh850g4::WR_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_0 group register
     */
    std::string register_name_SR_SELID_0(rh850g4::SR_SELID_0_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_1 group register
     */
    std::string register_name_SR_SELID_1(rh850g4::SR_SELID_1_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_2 group register
     */
    std::string register_name_SR_SELID_2(rh850g4::SR_SELID_2_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_3 group register
     */
    std::string register_name_SR_SELID_3(rh850g4::SR_SELID_3_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_4 group register
     */
    std::string register_name_SR_SELID_4(rh850g4::SR_SELID_4_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_5 group register
     */
    std::string register_name_SR_SELID_5(rh850g4::SR_SELID_5_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_9 group register
     */
    std::string register_name_SR_SELID_9(rh850g4::SR_SELID_9_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_10 group register
     */
    std::string register_name_SR_SELID_10(rh850g4::SR_SELID_10_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_11 group register
     */
    std::string register_name_SR_SELID_11(rh850g4::SR_SELID_11_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_12 group register
     */
    std::string register_name_SR_SELID_12(rh850g4::SR_SELID_12_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_13 group register
     */
    std::string register_name_SR_SELID_13(rh850g4::SR_SELID_13_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_14 group register
     */
    std::string register_name_SR_SELID_14(rh850g4::SR_SELID_14_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return String representing the name of the specified rh850g4 SR_SELID_15 group register
     */
    std::string register_name_SR_SELID_15(rh850g4::SR_SELID_15_index_t index);
    
    // Immediate register and register field read operations
    /**
     * \brief Queries the value of the rh850g4 translation time PC register
     * \return Value of the rh850g4 translation time PC register
     */
    unsigned int read_trans_PC(void);
    /**
     * \brief Queries the value of the rh850g4 r0 register
     * \return Value of the rh850g4 r0 register
     */
    unsigned int read_r0(void);
    /**
     * \brief Queries the value of the rh850g4 r1 register
     * \return Value of the rh850g4 r1 register
     */
    unsigned int read_r1(void);
    /**
     * \brief Queries the value of the rh850g4 r2 register
     * \return Value of the rh850g4 r2 register
     */
    unsigned int read_r2(void);
    /**
     * \brief Queries the value of the rh850g4 r3 register
     * \return Value of the rh850g4 r3 register
     */
    unsigned int read_r3(void);
    /**
     * \brief Queries the value of the rh850g4 r4 register
     * \return Value of the rh850g4 r4 register
     */
    unsigned int read_r4(void);
    /**
     * \brief Queries the value of the rh850g4 r5 register
     * \return Value of the rh850g4 r5 register
     */
    unsigned int read_r5(void);
    /**
     * \brief Queries the value of the rh850g4 r6 register
     * \return Value of the rh850g4 r6 register
     */
    unsigned int read_r6(void);
    /**
     * \brief Queries the value of the rh850g4 r7 register
     * \return Value of the rh850g4 r7 register
     */
    unsigned int read_r7(void);
    /**
     * \brief Queries the value of the rh850g4 r8 register
     * \return Value of the rh850g4 r8 register
     */
    unsigned int read_r8(void);
    /**
     * \brief Queries the value of the rh850g4 r9 register
     * \return Value of the rh850g4 r9 register
     */
    unsigned int read_r9(void);
    /**
     * \brief Queries the value of the rh850g4 r10 register
     * \return Value of the rh850g4 r10 register
     */
    unsigned int read_r10(void);
    /**
     * \brief Queries the value of the rh850g4 r11 register
     * \return Value of the rh850g4 r11 register
     */
    unsigned int read_r11(void);
    /**
     * \brief Queries the value of the rh850g4 r12 register
     * \return Value of the rh850g4 r12 register
     */
    unsigned int read_r12(void);
    /**
     * \brief Queries the value of the rh850g4 r13 register
     * \return Value of the rh850g4 r13 register
     */
    unsigned int read_r13(void);
    /**
     * \brief Queries the value of the rh850g4 r14 register
     * \return Value of the rh850g4 r14 register
     */
    unsigned int read_r14(void);
    /**
     * \brief Queries the value of the rh850g4 r15 register
     * \return Value of the rh850g4 r15 register
     */
    unsigned int read_r15(void);
    /**
     * \brief Queries the value of the rh850g4 r16 register
     * \return Value of the rh850g4 r16 register
     */
    unsigned int read_r16(void);
    /**
     * \brief Queries the value of the rh850g4 r17 register
     * \return Value of the rh850g4 r17 register
     */
    unsigned int read_r17(void);
    /**
     * \brief Queries the value of the rh850g4 r18 register
     * \return Value of the rh850g4 r18 register
     */
    unsigned int read_r18(void);
    /**
     * \brief Queries the value of the rh850g4 r19 register
     * \return Value of the rh850g4 r19 register
     */
    unsigned int read_r19(void);
    /**
     * \brief Queries the value of the rh850g4 r20 register
     * \return Value of the rh850g4 r20 register
     */
    unsigned int read_r20(void);
    /**
     * \brief Queries the value of the rh850g4 r21 register
     * \return Value of the rh850g4 r21 register
     */
    unsigned int read_r21(void);
    /**
     * \brief Queries the value of the rh850g4 r22 register
     * \return Value of the rh850g4 r22 register
     */
    unsigned int read_r22(void);
    /**
     * \brief Queries the value of the rh850g4 r23 register
     * \return Value of the rh850g4 r23 register
     */
    unsigned int read_r23(void);
    /**
     * \brief Queries the value of the rh850g4 r24 register
     * \return Value of the rh850g4 r24 register
     */
    unsigned int read_r24(void);
    /**
     * \brief Queries the value of the rh850g4 r25 register
     * \return Value of the rh850g4 r25 register
     */
    unsigned int read_r25(void);
    /**
     * \brief Queries the value of the rh850g4 r26 register
     * \return Value of the rh850g4 r26 register
     */
    unsigned int read_r26(void);
    /**
     * \brief Queries the value of the rh850g4 r27 register
     * \return Value of the rh850g4 r27 register
     */
    unsigned int read_r27(void);
    /**
     * \brief Queries the value of the rh850g4 r28 register
     * \return Value of the rh850g4 r28 register
     */
    unsigned int read_r28(void);
    /**
     * \brief Queries the value of the rh850g4 r29 register
     * \return Value of the rh850g4 r29 register
     */
    unsigned int read_r29(void);
    /**
     * \brief Queries the value of the rh850g4 r30 register
     * \return Value of the rh850g4 r30 register
     */
    unsigned int read_r30(void);
    /**
     * \brief Queries the value of the rh850g4 r31 register
     * \return Value of the rh850g4 r31 register
     */
    unsigned int read_r31(void);
    /**
     * \brief Queries the value of the rh850g4 PC register
     * \return Value of the rh850g4 PC register
     */
    unsigned int read_PC(void);
    /**
     * \brief Queries the value of the rh850g4 wr0 register
     * \return Value of the rh850g4 wr0 register
     */
    unsigned long long read_wr0(void);
    /**
     * \brief Queries the value of the rh850g4 wr0_u register
     * \return Value of the rh850g4 wr0_u register
     */
    unsigned long long read_wr0_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr1 register
     * \return Value of the rh850g4 wr1 register
     */
    unsigned long long read_wr1(void);
    /**
     * \brief Queries the value of the rh850g4 wr1_u register
     * \return Value of the rh850g4 wr1_u register
     */
    unsigned long long read_wr1_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr2 register
     * \return Value of the rh850g4 wr2 register
     */
    unsigned long long read_wr2(void);
    /**
     * \brief Queries the value of the rh850g4 wr2_u register
     * \return Value of the rh850g4 wr2_u register
     */
    unsigned long long read_wr2_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr3 register
     * \return Value of the rh850g4 wr3 register
     */
    unsigned long long read_wr3(void);
    /**
     * \brief Queries the value of the rh850g4 wr3_u register
     * \return Value of the rh850g4 wr3_u register
     */
    unsigned long long read_wr3_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr4 register
     * \return Value of the rh850g4 wr4 register
     */
    unsigned long long read_wr4(void);
    /**
     * \brief Queries the value of the rh850g4 wr4_u register
     * \return Value of the rh850g4 wr4_u register
     */
    unsigned long long read_wr4_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr5 register
     * \return Value of the rh850g4 wr5 register
     */
    unsigned long long read_wr5(void);
    /**
     * \brief Queries the value of the rh850g4 wr5_u register
     * \return Value of the rh850g4 wr5_u register
     */
    unsigned long long read_wr5_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr6 register
     * \return Value of the rh850g4 wr6 register
     */
    unsigned long long read_wr6(void);
    /**
     * \brief Queries the value of the rh850g4 wr6_u register
     * \return Value of the rh850g4 wr6_u register
     */
    unsigned long long read_wr6_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr7 register
     * \return Value of the rh850g4 wr7 register
     */
    unsigned long long read_wr7(void);
    /**
     * \brief Queries the value of the rh850g4 wr7_u register
     * \return Value of the rh850g4 wr7_u register
     */
    unsigned long long read_wr7_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr8 register
     * \return Value of the rh850g4 wr8 register
     */
    unsigned long long read_wr8(void);
    /**
     * \brief Queries the value of the rh850g4 wr8_u register
     * \return Value of the rh850g4 wr8_u register
     */
    unsigned long long read_wr8_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr9 register
     * \return Value of the rh850g4 wr9 register
     */
    unsigned long long read_wr9(void);
    /**
     * \brief Queries the value of the rh850g4 wr9_u register
     * \return Value of the rh850g4 wr9_u register
     */
    unsigned long long read_wr9_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr10 register
     * \return Value of the rh850g4 wr10 register
     */
    unsigned long long read_wr10(void);
    /**
     * \brief Queries the value of the rh850g4 wr10_u register
     * \return Value of the rh850g4 wr10_u register
     */
    unsigned long long read_wr10_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr11 register
     * \return Value of the rh850g4 wr11 register
     */
    unsigned long long read_wr11(void);
    /**
     * \brief Queries the value of the rh850g4 wr11_u register
     * \return Value of the rh850g4 wr11_u register
     */
    unsigned long long read_wr11_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr12 register
     * \return Value of the rh850g4 wr12 register
     */
    unsigned long long read_wr12(void);
    /**
     * \brief Queries the value of the rh850g4 wr12_u register
     * \return Value of the rh850g4 wr12_u register
     */
    unsigned long long read_wr12_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr13 register
     * \return Value of the rh850g4 wr13 register
     */
    unsigned long long read_wr13(void);
    /**
     * \brief Queries the value of the rh850g4 wr13_u register
     * \return Value of the rh850g4 wr13_u register
     */
    unsigned long long read_wr13_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr14 register
     * \return Value of the rh850g4 wr14 register
     */
    unsigned long long read_wr14(void);
    /**
     * \brief Queries the value of the rh850g4 wr14_u register
     * \return Value of the rh850g4 wr14_u register
     */
    unsigned long long read_wr14_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr15 register
     * \return Value of the rh850g4 wr15 register
     */
    unsigned long long read_wr15(void);
    /**
     * \brief Queries the value of the rh850g4 wr15_u register
     * \return Value of the rh850g4 wr15_u register
     */
    unsigned long long read_wr15_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr16 register
     * \return Value of the rh850g4 wr16 register
     */
    unsigned long long read_wr16(void);
    /**
     * \brief Queries the value of the rh850g4 wr16_u register
     * \return Value of the rh850g4 wr16_u register
     */
    unsigned long long read_wr16_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr17 register
     * \return Value of the rh850g4 wr17 register
     */
    unsigned long long read_wr17(void);
    /**
     * \brief Queries the value of the rh850g4 wr17_u register
     * \return Value of the rh850g4 wr17_u register
     */
    unsigned long long read_wr17_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr18 register
     * \return Value of the rh850g4 wr18 register
     */
    unsigned long long read_wr18(void);
    /**
     * \brief Queries the value of the rh850g4 wr18_u register
     * \return Value of the rh850g4 wr18_u register
     */
    unsigned long long read_wr18_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr19 register
     * \return Value of the rh850g4 wr19 register
     */
    unsigned long long read_wr19(void);
    /**
     * \brief Queries the value of the rh850g4 wr19_u register
     * \return Value of the rh850g4 wr19_u register
     */
    unsigned long long read_wr19_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr20 register
     * \return Value of the rh850g4 wr20 register
     */
    unsigned long long read_wr20(void);
    /**
     * \brief Queries the value of the rh850g4 wr20_u register
     * \return Value of the rh850g4 wr20_u register
     */
    unsigned long long read_wr20_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr21 register
     * \return Value of the rh850g4 wr21 register
     */
    unsigned long long read_wr21(void);
    /**
     * \brief Queries the value of the rh850g4 wr21_u register
     * \return Value of the rh850g4 wr21_u register
     */
    unsigned long long read_wr21_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr22 register
     * \return Value of the rh850g4 wr22 register
     */
    unsigned long long read_wr22(void);
    /**
     * \brief Queries the value of the rh850g4 wr22_u register
     * \return Value of the rh850g4 wr22_u register
     */
    unsigned long long read_wr22_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr23 register
     * \return Value of the rh850g4 wr23 register
     */
    unsigned long long read_wr23(void);
    /**
     * \brief Queries the value of the rh850g4 wr23_u register
     * \return Value of the rh850g4 wr23_u register
     */
    unsigned long long read_wr23_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr24 register
     * \return Value of the rh850g4 wr24 register
     */
    unsigned long long read_wr24(void);
    /**
     * \brief Queries the value of the rh850g4 wr24_u register
     * \return Value of the rh850g4 wr24_u register
     */
    unsigned long long read_wr24_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr25 register
     * \return Value of the rh850g4 wr25 register
     */
    unsigned long long read_wr25(void);
    /**
     * \brief Queries the value of the rh850g4 wr25_u register
     * \return Value of the rh850g4 wr25_u register
     */
    unsigned long long read_wr25_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr26 register
     * \return Value of the rh850g4 wr26 register
     */
    unsigned long long read_wr26(void);
    /**
     * \brief Queries the value of the rh850g4 wr26_u register
     * \return Value of the rh850g4 wr26_u register
     */
    unsigned long long read_wr26_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr27 register
     * \return Value of the rh850g4 wr27 register
     */
    unsigned long long read_wr27(void);
    /**
     * \brief Queries the value of the rh850g4 wr27_u register
     * \return Value of the rh850g4 wr27_u register
     */
    unsigned long long read_wr27_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr28 register
     * \return Value of the rh850g4 wr28 register
     */
    unsigned long long read_wr28(void);
    /**
     * \brief Queries the value of the rh850g4 wr28_u register
     * \return Value of the rh850g4 wr28_u register
     */
    unsigned long long read_wr28_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr29 register
     * \return Value of the rh850g4 wr29 register
     */
    unsigned long long read_wr29(void);
    /**
     * \brief Queries the value of the rh850g4 wr29_u register
     * \return Value of the rh850g4 wr29_u register
     */
    unsigned long long read_wr29_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr30 register
     * \return Value of the rh850g4 wr30 register
     */
    unsigned long long read_wr30(void);
    /**
     * \brief Queries the value of the rh850g4 wr30_u register
     * \return Value of the rh850g4 wr30_u register
     */
    unsigned long long read_wr30_u(void);
    /**
     * \brief Queries the value of the rh850g4 wr31 register
     * \return Value of the rh850g4 wr31 register
     */
    unsigned long long read_wr31(void);
    /**
     * \brief Queries the value of the rh850g4 wr31_u register
     * \return Value of the rh850g4 wr31_u register
     */
    unsigned long long read_wr31_u(void);
    /**
     * \brief Queries the value of the rh850g4 EIPC register
     * \return Value of the rh850g4 EIPC register
     */
    unsigned int read_EIPC(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSW register
     * \return Value of the rh850g4 EIPSW register
     */
    unsigned int read_EIPSW(void);
    /**
     * \brief Queries the value of the rh850g4 FEPC register
     * \return Value of the rh850g4 FEPC register
     */
    unsigned int read_FEPC(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSW register
     * \return Value of the rh850g4 FEPSW register
     */
    unsigned int read_FEPSW(void);
    /**
     * \brief Queries the value of the rh850g4 PSW register
     * \return Value of the rh850g4 PSW register
     */
    unsigned int read_PSW(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_Z register field
     * \return Value of the rh850g4 PSW_Z register field
     */
    unsigned char read_PSW_Z(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_S register field
     * \return Value of the rh850g4 PSW_S register field
     */
    unsigned char read_PSW_S(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_OV register field
     * \return Value of the rh850g4 PSW_OV register field
     */
    unsigned char read_PSW_OV(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CY register field
     * \return Value of the rh850g4 PSW_CY register field
     */
    unsigned char read_PSW_CY(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_SAT register field
     * \return Value of the rh850g4 PSW_SAT register field
     */
    unsigned char read_PSW_SAT(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_ID register field
     * \return Value of the rh850g4 PSW_ID register field
     */
    unsigned char read_PSW_ID(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_EP register field
     * \return Value of the rh850g4 PSW_EP register field
     */
    unsigned char read_PSW_EP(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_NP register field
     * \return Value of the rh850g4 PSW_NP register field
     */
    unsigned char read_PSW_NP(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_EBV register field
     * \return Value of the rh850g4 PSW_EBV register field
     */
    unsigned char read_PSW_EBV(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CU0 register field
     * \return Value of the rh850g4 PSW_CU0 register field
     */
    unsigned char read_PSW_CU0(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CU1 register field
     * \return Value of the rh850g4 PSW_CU1 register field
     */
    unsigned char read_PSW_CU1(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_CU2 register field
     * \return Value of the rh850g4 PSW_CU2 register field
     */
    unsigned char read_PSW_CU2(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_EIMASK register field
     * \return Value of the rh850g4 PSW_EIMASK register field
     */
    unsigned char read_PSW_EIMASK(void);
    /**
     * \brief Queries the value of the rh850g4 PSW_UM register field
     * \return Value of the rh850g4 PSW_UM register field
     */
    unsigned char read_PSW_UM(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR register
     * \return Value of the rh850g4 FPSR register
     */
    unsigned int read_FPSR(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_CC register field
     * \return Value of the rh850g4 FPSR_CC register field
     */
    unsigned char read_FPSR_CC(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_FN register field
     * \return Value of the rh850g4 FPSR_FN register field
     */
    unsigned char read_FPSR_FN(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_IF register field
     * \return Value of the rh850g4 FPSR_IF register field
     */
    unsigned char read_FPSR_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_PEM register field
     * \return Value of the rh850g4 FPSR_PEM register field
     */
    unsigned char read_FPSR_PEM(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_RM register field
     * \return Value of the rh850g4 FPSR_RM register field
     */
    unsigned char read_FPSR_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_FS register field
     * \return Value of the rh850g4 FPSR_FS register field
     */
    unsigned char read_FPSR_FS(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_XC register field
     * \return Value of the rh850g4 FPSR_XC register field
     */
    unsigned char read_FPSR_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_XE register field
     * \return Value of the rh850g4 FPSR_XE register field
     */
    unsigned char read_FPSR_XE(void);
    /**
     * \brief Queries the value of the rh850g4 FPSR_XP register field
     * \return Value of the rh850g4 FPSR_XP register field
     */
    unsigned char read_FPSR_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FPEPC register
     * \return Value of the rh850g4 FPEPC register
     */
    unsigned int read_FPEPC(void);
    /**
     * \brief Queries the value of the rh850g4 FPST register
     * \return Value of the rh850g4 FPST register
     */
    unsigned int read_FPST(void);
    /**
     * \brief Queries the value of the rh850g4 FPST_XC register field
     * \return Value of the rh850g4 FPST_XC register field
     */
    unsigned char read_FPST_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FPST_IF register field
     * \return Value of the rh850g4 FPST_IF register field
     */
    unsigned char read_FPST_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FPST_XP register field
     * \return Value of the rh850g4 FPST_XP register field
     */
    unsigned char read_FPST_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FPCC register
     * \return Value of the rh850g4 FPCC register
     */
    unsigned int read_FPCC(void);
    /**
     * \brief Queries the value of the rh850g4 FPCC_CC register field
     * \return Value of the rh850g4 FPCC_CC register field
     */
    unsigned char read_FPCC_CC(void);
    /**
     * \brief Queries the value of the rh850g4 FPCFG register
     * \return Value of the rh850g4 FPCFG register
     */
    unsigned int read_FPCFG(void);
    /**
     * \brief Queries the value of the rh850g4 FPCFG_RM register field
     * \return Value of the rh850g4 FPCFG_RM register field
     */
    unsigned char read_FPCFG_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FPCFG_XE register field
     * \return Value of the rh850g4 FPCFG_XE register field
     */
    unsigned char read_FPCFG_XE(void);
    /**
     * \brief Queries the value of the rh850g4 EIIC register
     * \return Value of the rh850g4 EIIC register
     */
    unsigned int read_EIIC(void);
    /**
     * \brief Queries the value of the rh850g4 FEIC register
     * \return Value of the rh850g4 FEIC register
     */
    unsigned int read_FEIC(void);
    /**
     * \brief Queries the value of the rh850g4 PSWH register
     * \return Value of the rh850g4 PSWH register
     */
    unsigned int read_PSWH(void);
    /**
     * \brief Queries the value of the rh850g4 PSWH_GPID register field
     * \return Value of the rh850g4 PSWH_GPID register field
     */
    unsigned char read_PSWH_GPID(void);
    /**
     * \brief Queries the value of the rh850g4 PSWH_GM register field
     * \return Value of the rh850g4 PSWH_GM register field
     */
    unsigned char read_PSWH_GM(void);
    /**
     * \brief Queries the value of the rh850g4 CTPC register
     * \return Value of the rh850g4 CTPC register
     */
    unsigned int read_CTPC(void);
    /**
     * \brief Queries the value of the rh850g4 CTPSW register
     * \return Value of the rh850g4 CTPSW register
     */
    unsigned int read_CTPSW(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSWH register
     * \return Value of the rh850g4 EIPSWH register
     */
    unsigned int read_EIPSWH(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSWH_GPID register field
     * \return Value of the rh850g4 EIPSWH_GPID register field
     */
    unsigned char read_EIPSWH_GPID(void);
    /**
     * \brief Queries the value of the rh850g4 EIPSWH_GM register field
     * \return Value of the rh850g4 EIPSWH_GM register field
     */
    unsigned char read_EIPSWH_GM(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSWH register
     * \return Value of the rh850g4 FEPSWH register
     */
    unsigned int read_FEPSWH(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSWH_GPID register field
     * \return Value of the rh850g4 FEPSWH_GPID register field
     */
    unsigned char read_FEPSWH_GPID(void);
    /**
     * \brief Queries the value of the rh850g4 FEPSWH_GM register field
     * \return Value of the rh850g4 FEPSWH_GM register field
     */
    unsigned char read_FEPSWH_GM(void);
    /**
     * \brief Queries the value of the rh850g4 CTBP register
     * \return Value of the rh850g4 CTBP register
     */
    unsigned int read_CTBP(void);
    /**
     * \brief Queries the value of the rh850g4 SNZCFG register
     * \return Value of the rh850g4 SNZCFG register
     */
    unsigned int read_SNZCFG(void);
    /**
     * \brief Queries the value of the rh850g4 EIWR register
     * \return Value of the rh850g4 EIWR register
     */
    unsigned int read_EIWR(void);
    /**
     * \brief Queries the value of the rh850g4 FEWR register
     * \return Value of the rh850g4 FEWR register
     */
    unsigned int read_FEWR(void);
    /**
     * \brief Queries the value of the rh850g4 SPID register
     * \return Value of the rh850g4 SPID register
     */
    unsigned int read_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 SPIDLIST register
     * \return Value of the rh850g4 SPIDLIST register
     */
    unsigned int read_SPIDLIST(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE register
     * \return Value of the rh850g4 RBASE register
     */
    unsigned int read_RBASE(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE_RBASE register field
     * \return Value of the rh850g4 RBASE_RBASE register field
     */
    unsigned int read_RBASE_RBASE(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE_DV register field
     * \return Value of the rh850g4 RBASE_DV register field
     */
    unsigned char read_RBASE_DV(void);
    /**
     * \brief Queries the value of the rh850g4 RBASE_RINT register field
     * \return Value of the rh850g4 RBASE_RINT register field
     */
    unsigned char read_RBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE register
     * \return Value of the rh850g4 EBASE register
     */
    unsigned int read_EBASE(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE_EBASE register field
     * \return Value of the rh850g4 EBASE_EBASE register field
     */
    unsigned int read_EBASE_EBASE(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE_DV register field
     * \return Value of the rh850g4 EBASE_DV register field
     */
    unsigned char read_EBASE_DV(void);
    /**
     * \brief Queries the value of the rh850g4 EBASE_RINT register field
     * \return Value of the rh850g4 EBASE_RINT register field
     */
    unsigned char read_EBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850g4 INTBP register
     * \return Value of the rh850g4 INTBP register
     */
    unsigned int read_INTBP(void);
    /**
     * \brief Queries the value of the rh850g4 MCTL register
     * \return Value of the rh850g4 MCTL register
     */
    unsigned int read_MCTL(void);
    /**
     * \brief Queries the value of the rh850g4 MCTL_UIC register field
     * \return Value of the rh850g4 MCTL_UIC register field
     */
    unsigned char read_MCTL_UIC(void);
    /**
     * \brief Queries the value of the rh850g4 PID register
     * \return Value of the rh850g4 PID register
     */
    unsigned int read_PID(void);
    /**
     * \brief Queries the value of the rh850g4 SVLOCK register
     * \return Value of the rh850g4 SVLOCK register
     */
    unsigned int read_SVLOCK(void);
    /**
     * \brief Queries the value of the rh850g4 SCCFG register
     * \return Value of the rh850g4 SCCFG register
     */
    unsigned int read_SCCFG(void);
    /**
     * \brief Queries the value of the rh850g4 SCCFG_SIZE register field
     * \return Value of the rh850g4 SCCFG_SIZE register field
     */
    unsigned char read_SCCFG_SIZE(void);
    /**
     * \brief Queries the value of the rh850g4 SCBP register
     * \return Value of the rh850g4 SCBP register
     */
    unsigned int read_SCBP(void);
    /**
     * \brief Queries the value of the rh850g4 HVCFG register
     * \return Value of the rh850g4 HVCFG register
     */
    unsigned int read_HVCFG(void);
    /**
     * \brief Queries the value of the rh850g4 HVCFG_HVE register field
     * \return Value of the rh850g4 HVCFG_HVE register field
     */
    unsigned char read_HVCFG_HVE(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG register
     * \return Value of the rh850g4 GMCFG register
     */
    unsigned int read_GMCFG(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GMP register field
     * \return Value of the rh850g4 GMCFG_GMP register field
     */
    unsigned char read_GMCFG_GMP(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_HMP register field
     * \return Value of the rh850g4 GMCFG_HMP register field
     */
    unsigned char read_GMCFG_HMP(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GSYSE register field
     * \return Value of the rh850g4 GMCFG_GSYSE register field
     */
    unsigned char read_GMCFG_GSYSE(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU0 register field
     * \return Value of the rh850g4 GMCFG_GCU0 register field
     */
    unsigned char read_GMCFG_GCU0(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU1 register field
     * \return Value of the rh850g4 GMCFG_GCU1 register field
     */
    unsigned char read_GMCFG_GCU1(void);
    /**
     * \brief Queries the value of the rh850g4 GMCFG_GCU2 register field
     * \return Value of the rh850g4 GMCFG_GCU2 register field
     */
    unsigned char read_GMCFG_GCU2(void);
    /**
     * \brief Queries the value of the rh850g4 HVSB register
     * \return Value of the rh850g4 HVSB register
     */
    unsigned int read_HVSB(void);
    /**
     * \brief Queries the value of the rh850g4 PEID register
     * \return Value of the rh850g4 PEID register
     */
    unsigned int read_PEID(void);
    /**
     * \brief Queries the value of the rh850g4 BMID register
     * \return Value of the rh850g4 BMID register
     */
    unsigned int read_BMID(void);
    /**
     * \brief Queries the value of the rh850g4 BMID_BMID register field
     * \return Value of the rh850g4 BMID_BMID register field
     */
    unsigned char read_BMID_BMID(void);
    /**
     * \brief Queries the value of the rh850g4 MEA register
     * \return Value of the rh850g4 MEA register
     */
    unsigned int read_MEA(void);
    /**
     * \brief Queries the value of the rh850g4 MEI register
     * \return Value of the rh850g4 MEI register
     */
    unsigned int read_MEI(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR0 register
     * \return Value of the rh850g4 RBCR0 register
     */
    unsigned int read_RBCR0(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR0_MD register field
     * \return Value of the rh850g4 RBCR0_MD register field
     */
    unsigned char read_RBCR0_MD(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR0_BE register field
     * \return Value of the rh850g4 RBCR0_BE register field
     */
    unsigned short read_RBCR0_BE(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR1 register
     * \return Value of the rh850g4 RBCR1 register
     */
    unsigned int read_RBCR1(void);
    /**
     * \brief Queries the value of the rh850g4 RBCR1_NC register field
     * \return Value of the rh850g4 RBCR1_NC register field
     */
    unsigned short read_RBCR1_NC(void);
    /**
     * \brief Queries the value of the rh850g4 RBNR register
     * \return Value of the rh850g4 RBNR register
     */
    unsigned int read_RBNR(void);
    /**
     * \brief Queries the value of the rh850g4 RBNR_BN register field
     * \return Value of the rh850g4 RBNR_BN register field
     */
    unsigned char read_RBNR_BN(void);
    /**
     * \brief Queries the value of the rh850g4 RBIP register
     * \return Value of the rh850g4 RBIP register
     */
    unsigned int read_RBIP(void);
    /**
     * \brief Queries the value of the rh850g4 RBIP_RBIP register field
     * \return Value of the rh850g4 RBIP_RBIP register field
     */
    unsigned int read_RBIP_RBIP(void);
    /**
     * \brief Queries the value of the rh850g4 ISPR register
     * \return Value of the rh850g4 ISPR register
     */
    unsigned int read_ISPR(void);
    /**
     * \brief Queries the value of the rh850g4 ISPR_ISP register field
     * \return Value of the rh850g4 ISPR_ISP register field
     */
    unsigned short read_ISPR_ISP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR register
     * \return Value of the rh850g4 IMSR register
     */
    unsigned int read_IMSR(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_EEIM register field
     * \return Value of the rh850g4 IMSR_EEIM register field
     */
    unsigned char read_IMSR_EEIM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_EPLM register field
     * \return Value of the rh850g4 IMSR_EPLM register field
     */
    unsigned char read_IMSR_EPLM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_EID register field
     * \return Value of the rh850g4 IMSR_EID register field
     */
    unsigned char read_IMSR_EID(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_ENP register field
     * \return Value of the rh850g4 IMSR_ENP register field
     */
    unsigned char read_IMSR_ENP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_FNP register field
     * \return Value of the rh850g4 IMSR_FNP register field
     */
    unsigned char read_IMSR_FNP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEEIM register field
     * \return Value of the rh850g4 IMSR_HEEIM register field
     */
    unsigned char read_IMSR_HEEIM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEPLM register field
     * \return Value of the rh850g4 IMSR_HEPLM register field
     */
    unsigned char read_IMSR_HEPLM(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HEID register field
     * \return Value of the rh850g4 IMSR_HEID register field
     */
    unsigned char read_IMSR_HEID(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HENP register field
     * \return Value of the rh850g4 IMSR_HENP register field
     */
    unsigned char read_IMSR_HENP(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR_HFNP register field
     * \return Value of the rh850g4 IMSR_HFNP register field
     */
    unsigned char read_IMSR_HFNP(void);
    /**
     * \brief Queries the value of the rh850g4 ICSR register
     * \return Value of the rh850g4 ICSR register
     */
    unsigned int read_ICSR(void);
    /**
     * \brief Queries the value of the rh850g4 ICSR_PMEI register field
     * \return Value of the rh850g4 ICSR_PMEI register field
     */
    unsigned char read_ICSR_PMEI(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG register
     * \return Value of the rh850g4 INTCFG register
     */
    unsigned int read_INTCFG(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG_ISPC register field
     * \return Value of the rh850g4 INTCFG_ISPC register field
     */
    unsigned char read_INTCFG_ISPC(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG_EPL register field
     * \return Value of the rh850g4 INTCFG_EPL register field
     */
    unsigned char read_INTCFG_EPL(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG_ULNR register field
     * \return Value of the rh850g4 INTCFG_ULNR register field
     */
    unsigned char read_INTCFG_ULNR(void);
    /**
     * \brief Queries the value of the rh850g4 PLMR register
     * \return Value of the rh850g4 PLMR register
     */
    unsigned int read_PLMR(void);
    /**
     * \brief Queries the value of the rh850g4 PLMR_PLM register field
     * \return Value of the rh850g4 PLMR_PLM register field
     */
    unsigned char read_PLMR_PLM(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR register
     * \return Value of the rh850g4 FXSR register
     */
    unsigned int read_FXSR(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_FN register field
     * \return Value of the rh850g4 FXSR_FN register field
     */
    unsigned char read_FXSR_FN(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_IF register field
     * \return Value of the rh850g4 FXSR_IF register field
     */
    unsigned char read_FXSR_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_PEM register field
     * \return Value of the rh850g4 FXSR_PEM register field
     */
    unsigned char read_FXSR_PEM(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_RM register field
     * \return Value of the rh850g4 FXSR_RM register field
     */
    unsigned char read_FXSR_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_FS register field
     * \return Value of the rh850g4 FXSR_FS register field
     */
    unsigned char read_FXSR_FS(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_XC register field
     * \return Value of the rh850g4 FXSR_XC register field
     */
    unsigned char read_FXSR_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_XE register field
     * \return Value of the rh850g4 FXSR_XE register field
     */
    unsigned char read_FXSR_XE(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR_XP register field
     * \return Value of the rh850g4 FXSR_XP register field
     */
    unsigned char read_FXSR_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FXST register
     * \return Value of the rh850g4 FXST register
     */
    unsigned int read_FXST(void);
    /**
     * \brief Queries the value of the rh850g4 FXST_XC register field
     * \return Value of the rh850g4 FXST_XC register field
     */
    unsigned char read_FXST_XC(void);
    /**
     * \brief Queries the value of the rh850g4 FXST_IF register field
     * \return Value of the rh850g4 FXST_IF register field
     */
    unsigned char read_FXST_IF(void);
    /**
     * \brief Queries the value of the rh850g4 FXST_XP register field
     * \return Value of the rh850g4 FXST_XP register field
     */
    unsigned char read_FXST_XP(void);
    /**
     * \brief Queries the value of the rh850g4 FXINFO register
     * \return Value of the rh850g4 FXINFO register
     */
    unsigned int read_FXINFO(void);
    /**
     * \brief Queries the value of the rh850g4 FXINFO_NFPU register field
     * \return Value of the rh850g4 FXINFO_NFPU register field
     */
    unsigned char read_FXINFO_NFPU(void);
    /**
     * \brief Queries the value of the rh850g4 FXINFO_RSIZE register field
     * \return Value of the rh850g4 FXINFO_RSIZE register field
     */
    unsigned char read_FXINFO_RSIZE(void);
    /**
     * \brief Queries the value of the rh850g4 FXCFG register
     * \return Value of the rh850g4 FXCFG register
     */
    unsigned int read_FXCFG(void);
    /**
     * \brief Queries the value of the rh850g4 FXCFG_RM register field
     * \return Value of the rh850g4 FXCFG_RM register field
     */
    unsigned char read_FXCFG_RM(void);
    /**
     * \brief Queries the value of the rh850g4 FXCFG_XE register field
     * \return Value of the rh850g4 FXCFG_XE register field
     */
    unsigned char read_FXCFG_XE(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC register
     * \return Value of the rh850g4 FXXC register
     */
    unsigned int read_FXXC(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC3 register field
     * \return Value of the rh850g4 FXXC_XC3 register field
     */
    unsigned char read_FXXC_XC3(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC2 register field
     * \return Value of the rh850g4 FXXC_XC2 register field
     */
    unsigned char read_FXXC_XC2(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC1 register field
     * \return Value of the rh850g4 FXXC_XC1 register field
     */
    unsigned char read_FXXC_XC1(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC_XC0 register field
     * \return Value of the rh850g4 FXXC_XC0 register field
     */
    unsigned char read_FXXC_XC0(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP register
     * \return Value of the rh850g4 FXXP register
     */
    unsigned int read_FXXP(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP3 register field
     * \return Value of the rh850g4 FXXP_XP3 register field
     */
    unsigned char read_FXXP_XP3(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP2 register field
     * \return Value of the rh850g4 FXXP_XP2 register field
     */
    unsigned char read_FXXP_XP2(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP1 register field
     * \return Value of the rh850g4 FXXP_XP1 register field
     */
    unsigned char read_FXXP_XP1(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP_XP0 register field
     * \return Value of the rh850g4 FXXP_XP0 register field
     */
    unsigned char read_FXXP_XP0(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIPC register
     * \return Value of the rh850g4 GMEIPC register
     */
    unsigned int read_GMEIPC(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIPSW register
     * \return Value of the rh850g4 GMEIPSW register
     */
    unsigned int read_GMEIPSW(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEPC register
     * \return Value of the rh850g4 GMFEPC register
     */
    unsigned int read_GMFEPC(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEPSW register
     * \return Value of the rh850g4 GMFEPSW register
     */
    unsigned int read_GMFEPSW(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW register
     * \return Value of the rh850g4 GMPSW register
     */
    unsigned int read_GMPSW(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_Z register field
     * \return Value of the rh850g4 GMPSW_Z register field
     */
    unsigned char read_GMPSW_Z(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_S register field
     * \return Value of the rh850g4 GMPSW_S register field
     */
    unsigned char read_GMPSW_S(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_OV register field
     * \return Value of the rh850g4 GMPSW_OV register field
     */
    unsigned char read_GMPSW_OV(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CY register field
     * \return Value of the rh850g4 GMPSW_CY register field
     */
    unsigned char read_GMPSW_CY(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_SAT register field
     * \return Value of the rh850g4 GMPSW_SAT register field
     */
    unsigned char read_GMPSW_SAT(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_ID register field
     * \return Value of the rh850g4 GMPSW_ID register field
     */
    unsigned char read_GMPSW_ID(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EP register field
     * \return Value of the rh850g4 GMPSW_EP register field
     */
    unsigned char read_GMPSW_EP(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_NP register field
     * \return Value of the rh850g4 GMPSW_NP register field
     */
    unsigned char read_GMPSW_NP(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EBV register field
     * \return Value of the rh850g4 GMPSW_EBV register field
     */
    unsigned char read_GMPSW_EBV(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU0 register field
     * \return Value of the rh850g4 GMPSW_CU0 register field
     */
    unsigned char read_GMPSW_CU0(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU1 register field
     * \return Value of the rh850g4 GMPSW_CU1 register field
     */
    unsigned char read_GMPSW_CU1(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_CU2 register field
     * \return Value of the rh850g4 GMPSW_CU2 register field
     */
    unsigned char read_GMPSW_CU2(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_EIMASK register field
     * \return Value of the rh850g4 GMPSW_EIMASK register field
     */
    unsigned char read_GMPSW_EIMASK(void);
    /**
     * \brief Queries the value of the rh850g4 GMPSW_UM register field
     * \return Value of the rh850g4 GMPSW_UM register field
     */
    unsigned char read_GMPSW_UM(void);
    /**
     * \brief Queries the value of the rh850g4 GMMEA register
     * \return Value of the rh850g4 GMMEA register
     */
    unsigned int read_GMMEA(void);
    /**
     * \brief Queries the value of the rh850g4 GMMEI register
     * \return Value of the rh850g4 GMMEI register
     */
    unsigned int read_GMMEI(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIIC register
     * \return Value of the rh850g4 GMEIIC register
     */
    unsigned int read_GMEIIC(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEIC register
     * \return Value of the rh850g4 GMFEIC register
     */
    unsigned int read_GMFEIC(void);
    /**
     * \brief Queries the value of the rh850g4 GMSPID register
     * \return Value of the rh850g4 GMSPID register
     */
    unsigned int read_GMSPID(void);
    /**
     * \brief Queries the value of the rh850g4 GMSPIDLIST register
     * \return Value of the rh850g4 GMSPIDLIST register
     */
    unsigned int read_GMSPIDLIST(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE register
     * \return Value of the rh850g4 GMEBASE register
     */
    unsigned int read_GMEBASE(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_EBASE register field
     * \return Value of the rh850g4 GMEBASE_EBASE register field
     */
    unsigned int read_GMEBASE_EBASE(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_DV register field
     * \return Value of the rh850g4 GMEBASE_DV register field
     */
    unsigned char read_GMEBASE_DV(void);
    /**
     * \brief Queries the value of the rh850g4 GMEBASE_RINT register field
     * \return Value of the rh850g4 GMEBASE_RINT register field
     */
    unsigned char read_GMEBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTBP register
     * \return Value of the rh850g4 GMINTBP register
     */
    unsigned int read_GMINTBP(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG register
     * \return Value of the rh850g4 GMINTCFG register
     */
    unsigned int read_GMINTCFG(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_ISPC register field
     * \return Value of the rh850g4 GMINTCFG_ISPC register field
     */
    unsigned char read_GMINTCFG_ISPC(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_EPL register field
     * \return Value of the rh850g4 GMINTCFG_EPL register field
     */
    unsigned char read_GMINTCFG_EPL(void);
    /**
     * \brief Queries the value of the rh850g4 GMINTCFG_ULNR register field
     * \return Value of the rh850g4 GMINTCFG_ULNR register field
     */
    unsigned char read_GMINTCFG_ULNR(void);
    /**
     * \brief Queries the value of the rh850g4 GMPLMR register
     * \return Value of the rh850g4 GMPLMR register
     */
    unsigned int read_GMPLMR(void);
    /**
     * \brief Queries the value of the rh850g4 GMPLMR_PLM register field
     * \return Value of the rh850g4 GMPLMR_PLM register field
     */
    unsigned char read_GMPLMR_PLM(void);
    /**
     * \brief Queries the value of the rh850g4 GMSVLOCK register
     * \return Value of the rh850g4 GMSVLOCK register
     */
    unsigned int read_GMSVLOCK(void);
    /**
     * \brief Queries the value of the rh850g4 GMMPM register
     * \return Value of the rh850g4 GMMPM register
     */
    unsigned int read_GMMPM(void);
    /**
     * \brief Queries the value of the rh850g4 GMEIWR register
     * \return Value of the rh850g4 GMEIWR register
     */
    unsigned int read_GMEIWR(void);
    /**
     * \brief Queries the value of the rh850g4 GMFEWR register
     * \return Value of the rh850g4 GMFEWR register
     */
    unsigned int read_GMFEWR(void);
    /**
     * \brief Queries the value of the rh850g4 GMPEID register
     * \return Value of the rh850g4 GMPEID register
     */
    unsigned int read_GMPEID(void);
    /**
     * \brief Queries the value of the rh850g4 MPM register
     * \return Value of the rh850g4 MPM register
     */
    unsigned int read_MPM(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG register
     * \return Value of the rh850g4 MPCFG register
     */
    unsigned int read_MPCFG(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_NMPUE register field
     * \return Value of the rh850g4 MPCFG_NMPUE register field
     */
    unsigned char read_MPCFG_NMPUE(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_HBE register field
     * \return Value of the rh850g4 MPCFG_HBE register field
     */
    unsigned char read_MPCFG_HBE(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_ARCH register field
     * \return Value of the rh850g4 MPCFG_ARCH register field
     */
    unsigned char read_MPCFG_ARCH(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_NBK register field
     * \return Value of the rh850g4 MPCFG_NBK register field
     */
    unsigned char read_MPCFG_NBK(void);
    /**
     * \brief Queries the value of the rh850g4 MPCFG_DMDP register field
     * \return Value of the rh850g4 MPCFG_DMDP register field
     */
    unsigned char read_MPCFG_DMDP(void);
    /**
     * \brief Queries the value of the rh850g4 MCA register
     * \return Value of the rh850g4 MCA register
     */
    unsigned int read_MCA(void);
    /**
     * \brief Queries the value of the rh850g4 MCS register
     * \return Value of the rh850g4 MCS register
     */
    unsigned int read_MCS(void);
    /**
     * \brief Queries the value of the rh850g4 MCC register
     * \return Value of the rh850g4 MCC register
     */
    unsigned int read_MCC(void);
    /**
     * \brief Queries the value of the rh850g4 MCR register
     * \return Value of the rh850g4 MCR register
     */
    unsigned int read_MCR(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HSXE register field
     * \return Value of the rh850g4 MCR_HSXE register field
     */
    unsigned char read_MCR_HSXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HSWE register field
     * \return Value of the rh850g4 MCR_HSWE register field
     */
    unsigned char read_MCR_HSWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HSRE register field
     * \return Value of the rh850g4 MCR_HSRE register field
     */
    unsigned char read_MCR_HSRE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HUXE register field
     * \return Value of the rh850g4 MCR_HUXE register field
     */
    unsigned char read_MCR_HUXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HUWE register field
     * \return Value of the rh850g4 MCR_HUWE register field
     */
    unsigned char read_MCR_HUWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_HURE register field
     * \return Value of the rh850g4 MCR_HURE register field
     */
    unsigned char read_MCR_HURE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GSXE register field
     * \return Value of the rh850g4 MCR_GSXE register field
     */
    unsigned char read_MCR_GSXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GSWE register field
     * \return Value of the rh850g4 MCR_GSWE register field
     */
    unsigned char read_MCR_GSWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GSRE register field
     * \return Value of the rh850g4 MCR_GSRE register field
     */
    unsigned char read_MCR_GSRE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GUXE register field
     * \return Value of the rh850g4 MCR_GUXE register field
     */
    unsigned char read_MCR_GUXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GUWE register field
     * \return Value of the rh850g4 MCR_GUWE register field
     */
    unsigned char read_MCR_GUWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_GURE register field
     * \return Value of the rh850g4 MCR_GURE register field
     */
    unsigned char read_MCR_GURE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_OV register field
     * \return Value of the rh850g4 MCR_OV register field
     */
    unsigned char read_MCR_OV(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_SXE register field
     * \return Value of the rh850g4 MCR_SXE register field
     */
    unsigned char read_MCR_SXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_SWE register field
     * \return Value of the rh850g4 MCR_SWE register field
     */
    unsigned char read_MCR_SWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_SRE register field
     * \return Value of the rh850g4 MCR_SRE register field
     */
    unsigned char read_MCR_SRE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_UXE register field
     * \return Value of the rh850g4 MCR_UXE register field
     */
    unsigned char read_MCR_UXE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_UWE register field
     * \return Value of the rh850g4 MCR_UWE register field
     */
    unsigned char read_MCR_UWE(void);
    /**
     * \brief Queries the value of the rh850g4 MCR_URE register field
     * \return Value of the rh850g4 MCR_URE register field
     */
    unsigned char read_MCR_URE(void);
    /**
     * \brief Queries the value of the rh850g4 MCI register
     * \return Value of the rh850g4 MCI register
     */
    unsigned int read_MCI(void);
    /**
     * \brief Queries the value of the rh850g4 MCI_SPID register field
     * \return Value of the rh850g4 MCI_SPID register field
     */
    unsigned char read_MCI_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPIDX register
     * \return Value of the rh850g4 MPIDX register
     */
    unsigned int read_MPIDX(void);
    /**
     * \brief Queries the value of the rh850g4 MPIDX_IDX register field
     * \return Value of the rh850g4 MPIDX_IDX register field
     */
    unsigned char read_MPIDX_IDX(void);
    /**
     * \brief Queries the value of the rh850g4 MPBK register
     * \return Value of the rh850g4 MPBK register
     */
    unsigned int read_MPBK(void);
    /**
     * \brief Queries the value of the rh850g4 MPBK_BK register field
     * \return Value of the rh850g4 MPBK_BK register field
     */
    unsigned char read_MPBK_BK(void);
    /**
     * \brief Queries the value of the rh850g4 MPLA register
     * \return Value of the rh850g4 MPLA register
     */
    unsigned int read_MPLA(void);
    /**
     * \brief Queries the value of the rh850g4 MPUA register
     * \return Value of the rh850g4 MPUA register
     */
    unsigned int read_MPUA(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT register
     * \return Value of the rh850g4 MPAT register
     */
    unsigned int read_MPAT(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID7 register field
     * \return Value of the rh850g4 MPAT_WMPID7 register field
     */
    unsigned char read_MPAT_WMPID7(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID6 register field
     * \return Value of the rh850g4 MPAT_WMPID6 register field
     */
    unsigned char read_MPAT_WMPID6(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID5 register field
     * \return Value of the rh850g4 MPAT_WMPID5 register field
     */
    unsigned char read_MPAT_WMPID5(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID4 register field
     * \return Value of the rh850g4 MPAT_WMPID4 register field
     */
    unsigned char read_MPAT_WMPID4(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID3 register field
     * \return Value of the rh850g4 MPAT_WMPID3 register field
     */
    unsigned char read_MPAT_WMPID3(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID2 register field
     * \return Value of the rh850g4 MPAT_WMPID2 register field
     */
    unsigned char read_MPAT_WMPID2(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID1 register field
     * \return Value of the rh850g4 MPAT_WMPID1 register field
     */
    unsigned char read_MPAT_WMPID1(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WMPID0 register field
     * \return Value of the rh850g4 MPAT_WMPID0 register field
     */
    unsigned char read_MPAT_WMPID0(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID7 register field
     * \return Value of the rh850g4 MPAT_RMPID7 register field
     */
    unsigned char read_MPAT_RMPID7(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID6 register field
     * \return Value of the rh850g4 MPAT_RMPID6 register field
     */
    unsigned char read_MPAT_RMPID6(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID5 register field
     * \return Value of the rh850g4 MPAT_RMPID5 register field
     */
    unsigned char read_MPAT_RMPID5(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID4 register field
     * \return Value of the rh850g4 MPAT_RMPID4 register field
     */
    unsigned char read_MPAT_RMPID4(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID3 register field
     * \return Value of the rh850g4 MPAT_RMPID3 register field
     */
    unsigned char read_MPAT_RMPID3(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID2 register field
     * \return Value of the rh850g4 MPAT_RMPID2 register field
     */
    unsigned char read_MPAT_RMPID2(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID1 register field
     * \return Value of the rh850g4 MPAT_RMPID1 register field
     */
    unsigned char read_MPAT_RMPID1(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RMPID0 register field
     * \return Value of the rh850g4 MPAT_RMPID0 register field
     */
    unsigned char read_MPAT_RMPID0(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_WG register field
     * \return Value of the rh850g4 MPAT_WG register field
     */
    unsigned char read_MPAT_WG(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_RG register field
     * \return Value of the rh850g4 MPAT_RG register field
     */
    unsigned char read_MPAT_RG(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_E register field
     * \return Value of the rh850g4 MPAT_E register field
     */
    unsigned char read_MPAT_E(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_SX register field
     * \return Value of the rh850g4 MPAT_SX register field
     */
    unsigned char read_MPAT_SX(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_SW register field
     * \return Value of the rh850g4 MPAT_SW register field
     */
    unsigned char read_MPAT_SW(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_SR register field
     * \return Value of the rh850g4 MPAT_SR register field
     */
    unsigned char read_MPAT_SR(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_UX register field
     * \return Value of the rh850g4 MPAT_UX register field
     */
    unsigned char read_MPAT_UX(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_UW register field
     * \return Value of the rh850g4 MPAT_UW register field
     */
    unsigned char read_MPAT_UW(void);
    /**
     * \brief Queries the value of the rh850g4 MPAT_UR register field
     * \return Value of the rh850g4 MPAT_UR register field
     */
    unsigned char read_MPAT_UR(void);
    /**
     * \brief Queries the value of the rh850g4 MPID0 register
     * \return Value of the rh850g4 MPID0 register
     */
    unsigned int read_MPID0(void);
    /**
     * \brief Queries the value of the rh850g4 MPID0_SPID register field
     * \return Value of the rh850g4 MPID0_SPID register field
     */
    unsigned char read_MPID0_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID1 register
     * \return Value of the rh850g4 MPID1 register
     */
    unsigned int read_MPID1(void);
    /**
     * \brief Queries the value of the rh850g4 MPID1_SPID register field
     * \return Value of the rh850g4 MPID1_SPID register field
     */
    unsigned char read_MPID1_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID2 register
     * \return Value of the rh850g4 MPID2 register
     */
    unsigned int read_MPID2(void);
    /**
     * \brief Queries the value of the rh850g4 MPID2_SPID register field
     * \return Value of the rh850g4 MPID2_SPID register field
     */
    unsigned char read_MPID2_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID3 register
     * \return Value of the rh850g4 MPID3 register
     */
    unsigned int read_MPID3(void);
    /**
     * \brief Queries the value of the rh850g4 MPID3_SPID register field
     * \return Value of the rh850g4 MPID3_SPID register field
     */
    unsigned char read_MPID3_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID4 register
     * \return Value of the rh850g4 MPID4 register
     */
    unsigned int read_MPID4(void);
    /**
     * \brief Queries the value of the rh850g4 MPID4_SPID register field
     * \return Value of the rh850g4 MPID4_SPID register field
     */
    unsigned char read_MPID4_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID5 register
     * \return Value of the rh850g4 MPID5 register
     */
    unsigned int read_MPID5(void);
    /**
     * \brief Queries the value of the rh850g4 MPID5_SPID register field
     * \return Value of the rh850g4 MPID5_SPID register field
     */
    unsigned char read_MPID5_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID6 register
     * \return Value of the rh850g4 MPID6 register
     */
    unsigned int read_MPID6(void);
    /**
     * \brief Queries the value of the rh850g4 MPID6_SPID register field
     * \return Value of the rh850g4 MPID6_SPID register field
     */
    unsigned char read_MPID6_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 MPID7 register
     * \return Value of the rh850g4 MPID7 register
     */
    unsigned int read_MPID7(void);
    /**
     * \brief Queries the value of the rh850g4 MPID7_SPID register field
     * \return Value of the rh850g4 MPID7_SPID register field
     */
    unsigned char read_MPID7_SPID(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL register
     * \return Value of the rh850g4 ICTAGL register
     */
    unsigned int read_ICTAGL(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_LPN register field
     * \return Value of the rh850g4 ICTAGL_LPN register field
     */
    unsigned int read_ICTAGL_LPN(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_LRU register field
     * \return Value of the rh850g4 ICTAGL_LRU register field
     */
    unsigned char read_ICTAGL_LRU(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_L register field
     * \return Value of the rh850g4 ICTAGL_L register field
     */
    unsigned char read_ICTAGL_L(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL_V register field
     * \return Value of the rh850g4 ICTAGL_V register field
     */
    unsigned char read_ICTAGL_V(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH register
     * \return Value of the rh850g4 ICTAGH register
     */
    unsigned int read_ICTAGH(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_WD register field
     * \return Value of the rh850g4 ICTAGH_WD register field
     */
    unsigned char read_ICTAGH_WD(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_PD register field
     * \return Value of the rh850g4 ICTAGH_PD register field
     */
    unsigned char read_ICTAGH_PD(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_WT register field
     * \return Value of the rh850g4 ICTAGH_WT register field
     */
    unsigned char read_ICTAGH_WT(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_PT register field
     * \return Value of the rh850g4 ICTAGH_PT register field
     */
    unsigned char read_ICTAGH_PT(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_DATAECC register field
     * \return Value of the rh850g4 ICTAGH_DATAECC register field
     */
    unsigned char read_ICTAGH_DATAECC(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH_TAGECC register field
     * \return Value of the rh850g4 ICTAGH_TAGECC register field
     */
    unsigned char read_ICTAGH_TAGECC(void);
    /**
     * \brief Queries the value of the rh850g4 ICDATL register
     * \return Value of the rh850g4 ICDATL register
     */
    unsigned int read_ICDATL(void);
    /**
     * \brief Queries the value of the rh850g4 ICDATH register
     * \return Value of the rh850g4 ICDATH register
     */
    unsigned int read_ICDATH(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL register
     * \return Value of the rh850g4 ICCTRL register
     */
    unsigned int read_ICCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_PBS register field
     * \return Value of the rh850g4 ICCTRL_PBS register field
     */
    unsigned char read_ICCTRL_PBS(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHCLR register field
     * \return Value of the rh850g4 ICCTRL_ICHCLR register field
     */
    unsigned char read_ICCTRL_ICHCLR(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEIV register field
     * \return Value of the rh850g4 ICCTRL_ICHEIV register field
     */
    unsigned char read_ICCTRL_ICHEIV(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEMK register field
     * \return Value of the rh850g4 ICCTRL_ICHEMK register field
     */
    unsigned char read_ICCTRL_ICHEMK(void);
    /**
     * \brief Queries the value of the rh850g4 ICCTRL_ICHEN register field
     * \return Value of the rh850g4 ICCTRL_ICHEN register field
     */
    unsigned char read_ICCTRL_ICHEN(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG register
     * \return Value of the rh850g4 ICCFG register
     */
    unsigned int read_ICCFG(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHSIZE register field
     * \return Value of the rh850g4 ICCFG_ICHSIZE register field
     */
    unsigned char read_ICCFG_ICHSIZE(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHLINE register field
     * \return Value of the rh850g4 ICCFG_ICHLINE register field
     */
    unsigned char read_ICCFG_ICHLINE(void);
    /**
     * \brief Queries the value of the rh850g4 ICCFG_ICHWAY register field
     * \return Value of the rh850g4 ICCFG_ICHWAY register field
     */
    unsigned char read_ICCFG_ICHWAY(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR register
     * \return Value of the rh850g4 ICERR register
     */
    unsigned int read_ICERR(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_CISTW register field
     * \return Value of the rh850g4 ICERR_CISTW register field
     */
    unsigned char read_ICERR_CISTW(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESAFE register field
     * \return Value of the rh850g4 ICERR_ESAFE register field
     */
    unsigned char read_ICERR_ESAFE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESMH register field
     * \return Value of the rh850g4 ICERR_ESMH register field
     */
    unsigned char read_ICERR_ESMH(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESPBSE register field
     * \return Value of the rh850g4 ICERR_ESPBSE register field
     */
    unsigned char read_ICERR_ESPBSE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESTE1 register field
     * \return Value of the rh850g4 ICERR_ESTE1 register field
     */
    unsigned char read_ICERR_ESTE1(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESTE2 register field
     * \return Value of the rh850g4 ICERR_ESTE2 register field
     */
    unsigned char read_ICERR_ESTE2(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESDC register field
     * \return Value of the rh850g4 ICERR_ESDC register field
     */
    unsigned char read_ICERR_ESDC(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ESDE register field
     * \return Value of the rh850g4 ICERR_ESDE register field
     */
    unsigned char read_ICERR_ESDE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMAFE register field
     * \return Value of the rh850g4 ICERR_ERMAFE register field
     */
    unsigned char read_ICERR_ERMAFE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMMH register field
     * \return Value of the rh850g4 ICERR_ERMMH register field
     */
    unsigned char read_ICERR_ERMMH(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMPBSE register field
     * \return Value of the rh850g4 ICERR_ERMPBSE register field
     */
    unsigned char read_ICERR_ERMPBSE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMTE1 register field
     * \return Value of the rh850g4 ICERR_ERMTE1 register field
     */
    unsigned char read_ICERR_ERMTE1(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMTE2 register field
     * \return Value of the rh850g4 ICERR_ERMTE2 register field
     */
    unsigned char read_ICERR_ERMTE2(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMDC register field
     * \return Value of the rh850g4 ICERR_ERMDC register field
     */
    unsigned char read_ICERR_ERMDC(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ERMDE register field
     * \return Value of the rh850g4 ICERR_ERMDE register field
     */
    unsigned char read_ICERR_ERMDE(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHEWY register field
     * \return Value of the rh850g4 ICERR_ICHEWY register field
     */
    unsigned char read_ICERR_ICHEWY(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHEIX register field
     * \return Value of the rh850g4 ICERR_ICHEIX register field
     */
    unsigned char read_ICERR_ICHEIX(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHERQ register field
     * \return Value of the rh850g4 ICERR_ICHERQ register field
     */
    unsigned char read_ICERR_ICHERQ(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHED register field
     * \return Value of the rh850g4 ICERR_ICHED register field
     */
    unsigned char read_ICERR_ICHED(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHET register field
     * \return Value of the rh850g4 ICERR_ICHET register field
     */
    unsigned char read_ICERR_ICHET(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR_ICHERR register field
     * \return Value of the rh850g4 ICERR_ICHERR register field
     */
    unsigned char read_ICERR_ICHERR(void);
    /**
     * \brief Queries the value of the rh850g4 TSCOUNTL register
     * \return Value of the rh850g4 TSCOUNTL register
     */
    unsigned int read_TSCOUNTL(void);
    /**
     * \brief Queries the value of the rh850g4 TSCOUNTH register
     * \return Value of the rh850g4 TSCOUNTH register
     */
    unsigned int read_TSCOUNTH(void);
    /**
     * \brief Queries the value of the rh850g4 TSCTRL register
     * \return Value of the rh850g4 TSCTRL register
     */
    unsigned int read_TSCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 PMUMCTRL register
     * \return Value of the rh850g4 PMUMCTRL register
     */
    unsigned int read_PMUMCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 PMGMCTRL register
     * \return Value of the rh850g4 PMGMCTRL register
     */
    unsigned int read_PMGMCTRL(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT0 register
     * \return Value of the rh850g4 PMCOUNT0 register
     */
    unsigned int read_PMCOUNT0(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL0 register
     * \return Value of the rh850g4 PMCTRL0 register
     */
    unsigned int read_PMCTRL0(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT1 register
     * \return Value of the rh850g4 PMCOUNT1 register
     */
    unsigned int read_PMCOUNT1(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL1 register
     * \return Value of the rh850g4 PMCTRL1 register
     */
    unsigned int read_PMCTRL1(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT2 register
     * \return Value of the rh850g4 PMCOUNT2 register
     */
    unsigned int read_PMCOUNT2(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL2 register
     * \return Value of the rh850g4 PMCTRL2 register
     */
    unsigned int read_PMCTRL2(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT3 register
     * \return Value of the rh850g4 PMCOUNT3 register
     */
    unsigned int read_PMCOUNT3(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL3 register
     * \return Value of the rh850g4 PMCTRL3 register
     */
    unsigned int read_PMCTRL3(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL4 register
     * \return Value of the rh850g4 PMCTRL4 register
     */
    unsigned int read_PMCTRL4(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL5 register
     * \return Value of the rh850g4 PMCTRL5 register
     */
    unsigned int read_PMCTRL5(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL6 register
     * \return Value of the rh850g4 PMCTRL6 register
     */
    unsigned int read_PMCTRL6(void);
    /**
     * \brief Queries the value of the rh850g4 PMCTRL7 register
     * \return Value of the rh850g4 PMCTRL7 register
     */
    unsigned int read_PMCTRL7(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT4 register
     * \return Value of the rh850g4 PMCOUNT4 register
     */
    unsigned int read_PMCOUNT4(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT5 register
     * \return Value of the rh850g4 PMCOUNT5 register
     */
    unsigned int read_PMCOUNT5(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT6 register
     * \return Value of the rh850g4 PMCOUNT6 register
     */
    unsigned int read_PMCOUNT6(void);
    /**
     * \brief Queries the value of the rh850g4 PMCOUNT7 register
     * \return Value of the rh850g4 PMCOUNT7 register
     */
    unsigned int read_PMCOUNT7(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND0 register
     * \return Value of the rh850g4 PMSUBCND0 register
     */
    unsigned int read_PMSUBCND0(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND1 register
     * \return Value of the rh850g4 PMSUBCND1 register
     */
    unsigned int read_PMSUBCND1(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND2 register
     * \return Value of the rh850g4 PMSUBCND2 register
     */
    unsigned int read_PMSUBCND2(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND3 register
     * \return Value of the rh850g4 PMSUBCND3 register
     */
    unsigned int read_PMSUBCND3(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND4 register
     * \return Value of the rh850g4 PMSUBCND4 register
     */
    unsigned int read_PMSUBCND4(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND5 register
     * \return Value of the rh850g4 PMSUBCND5 register
     */
    unsigned int read_PMSUBCND5(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND6 register
     * \return Value of the rh850g4 PMSUBCND6 register
     */
    unsigned int read_PMSUBCND6(void);
    /**
     * \brief Queries the value of the rh850g4 PMSUBCND7 register
     * \return Value of the rh850g4 PMSUBCND7 register
     */
    unsigned int read_PMSUBCND7(void);
    /**
     * \brief Queries the value of the rh850g4 DBGEN register
     * \return Value of the rh850g4 DBGEN register
     */
    unsigned int read_DBGEN(void);
    /**
     * \brief Queries the value of the rh850g4 DBPSWH register
     * \return Value of the rh850g4 DBPSWH register
     */
    unsigned int read_DBPSWH(void);
    /**
     * \brief Queries the value of the rh850g4 DBIC register
     * \return Value of the rh850g4 DBIC register
     */
    unsigned int read_DBIC(void);
    /**
     * \brief Queries the value of the rh850g4 DBPC register
     * \return Value of the rh850g4 DBPC register
     */
    unsigned int read_DBPC(void);
    /**
     * \brief Queries the value of the rh850g4 DBPSW register
     * \return Value of the rh850g4 DBPSW register
     */
    unsigned int read_DBPSW(void);
    /**
     * \brief Queries the value of the rh850g4 DIR0 register
     * \return Value of the rh850g4 DIR0 register
     */
    unsigned int read_DIR0(void);
    /**
     * \brief Queries the value of the rh850g4 DIR1 register
     * \return Value of the rh850g4 DIR1 register
     */
    unsigned int read_DIR1(void);
    /**
     * \brief Queries the value of the rh850g4 BPC register
     * \return Value of the rh850g4 BPC register
     */
    unsigned int read_BPC(void);
    /**
     * \brief Queries the value of the rh850g4 BPAV register
     * \return Value of the rh850g4 BPAV register
     */
    unsigned int read_BPAV(void);
    /**
     * \brief Queries the value of the rh850g4 BPAM register
     * \return Value of the rh850g4 BPAM register
     */
    unsigned int read_BPAM(void);
    /**
     * \brief Queries the value of the rh850g4 DBCFG register
     * \return Value of the rh850g4 DBCFG register
     */
    unsigned int read_DBCFG(void);
    /**
     * \brief Queries the value of the rh850g4 DBWR register
     * \return Value of the rh850g4 DBWR register
     */
    unsigned int read_DBWR(void);
    /**
     * \brief Queries the value of the rh850g4 LSTEST0 register
     * \return Value of the rh850g4 LSTEST0 register
     */
    unsigned int read_LSTEST0(void);
    /**
     * \brief Queries the value of the rh850g4 LSTEST1 register
     * \return Value of the rh850g4 LSTEST1 register
     */
    unsigned int read_LSTEST1(void);
    /**
     * \brief Queries the value of the rh850g4 LSCFG register
     * \return Value of the rh850g4 LSCFG register
     */
    unsigned int read_LSCFG(void);
    /**
     * \brief Queries the value of the rh850g4 ICBKEY register
     * \return Value of the rh850g4 ICBKEY register
     */
    unsigned int read_ICBKEY(void);
    /**
     * \brief Queries the value of the rh850g4 IFCR register
     * \return Value of the rh850g4 IFCR register
     */
    unsigned int read_IFCR(void);
    /**
     * \brief Queries the value of the rh850g4 IFCR1 register
     * \return Value of the rh850g4 IFCR1 register
     */
    unsigned int read_IFCR1(void);
    /**
     * \brief Queries the value of the rh850g4 BRPCTRL0 register
     * \return Value of the rh850g4 BRPCTRL0 register
     */
    unsigned int read_BRPCTRL0(void);
    /**
     * \brief Queries the value of the rh850g4 BRPCTRL1 register
     * \return Value of the rh850g4 BRPCTRL1 register
     */
    unsigned int read_BRPCTRL1(void);
    /**
     * \brief Queries the value of the rh850g4 BRPCFG register
     * \return Value of the rh850g4 BRPCFG register
     */
    unsigned int read_BRPCFG(void);
    /**
     * \brief Queries the value of the rh850g4 BRPACTRL register
     * \return Value of the rh850g4 BRPACTRL register
     */
    unsigned int read_BRPACTRL(void);
    /**
     * \brief Queries the value of the rh850g4 BRPDATA register
     * \return Value of the rh850g4 BRPDATA register
     */
    unsigned int read_BRPDATA(void);
    /**
     * \brief Queries the value of the rh850g4 DCBKEY register
     * \return Value of the rh850g4 DCBKEY register
     */
    unsigned int read_DCBKEY(void);
    /**
     * \brief Queries the value of the rh850g4 LSUCR register
     * \return Value of the rh850g4 LSUCR register
     */
    unsigned int read_LSUCR(void);
    /**
     * \brief Queries the value of the rh850g4 LSULNK0 register
     * \return Value of the rh850g4 LSULNK0 register
     */
    unsigned int read_LSULNK0(void);
    /**
     * \brief Queries the value of the rh850g4 LSULNK1 register
     * \return Value of the rh850g4 LSULNK1 register
     */
    unsigned int read_LSULNK1(void);
    /**
     * \brief Queries the value of the rh850g4 L1RLCR register
     * \return Value of the rh850g4 L1RLCR register
     */
    unsigned int read_L1RLCR(void);
    /**
     * \brief Queries the value of the rh850g4 L1RLNK0 register
     * \return Value of the rh850g4 L1RLNK0 register
     */
    unsigned int read_L1RLNK0(void);
    /**
     * \brief Queries the value of the rh850g4 L1RLNK1 register
     * \return Value of the rh850g4 L1RLNK1 register
     */
    unsigned int read_L1RLNK1(void);
    /**
     * \brief Queries the value of the rh850g4 L1RCFG register
     * \return Value of the rh850g4 L1RCFG register
     */
    unsigned int read_L1RCFG(void);
    /**
     * \brief Queries the value of the rh850g4 DECFG register
     * \return Value of the rh850g4 DECFG register
     */
    unsigned int read_DECFG(void);
    /**
     * \brief Queries the value of the rh850g4 DECTRL register
     * \return Value of the rh850g4 DECTRL register
     */
    unsigned int read_DECTRL(void);
    /**
     * \brief Queries the value of the rh850g4 DEVDS register
     * \return Value of the rh850g4 DEVDS register
     */
    unsigned int read_DEVDS(void);
    /**
     * \brief Queries the value of the rh850g4 RDBCR register
     * \return Value of the rh850g4 RDBCR register
     */
    unsigned int read_RDBCR(void);
    /**
     * \brief Queries the value of the rh850g4 RDBACR register
     * \return Value of the rh850g4 RDBACR register
     */
    unsigned int read_RDBACR(void);
    /**
     * \brief Queries the value of the rh850g4 RDBATAG register
     * \return Value of the rh850g4 RDBATAG register
     */
    unsigned int read_RDBATAG(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT0 register
     * \return Value of the rh850g4 RDBADAT0 register
     */
    unsigned int read_RDBADAT0(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT1 register
     * \return Value of the rh850g4 RDBADAT1 register
     */
    unsigned int read_RDBADAT1(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT2 register
     * \return Value of the rh850g4 RDBADAT2 register
     */
    unsigned int read_RDBADAT2(void);
    /**
     * \brief Queries the value of the rh850g4 RDBADAT3 register
     * \return Value of the rh850g4 RDBADAT3 register
     */
    unsigned int read_RDBADAT3(void);
    /**
     * \brief Queries the value of the rh850g4 RDBSTAT register
     * \return Value of the rh850g4 RDBSTAT register
     */
    unsigned int read_RDBSTAT(void);
    /**
     * \brief Queries the value of the rh850g4 hv_func_support register
     * \return Value of the rh850g4 hv_func_support register
     */
    unsigned char read_hv_func_support(void);
    /**
     * \brief Queries the value of the rh850g4 swd0pcad_exec_count register
     * \return Value of the rh850g4 swd0pcad_exec_count register
     */
    unsigned int read_swd0pcad_exec_count(void);
    /**
     * \brief Queries the value of the rh850g4 swd0pcad_val register
     * \return Value of the rh850g4 swd0pcad_val register
     */
    unsigned int read_swd0pcad_val(void);
    /**
     * \brief Queries the value of the rh850g4 mem_acc_type register
     * \return Value of the rh850g4 mem_acc_type register
     */
    unsigned int read_mem_acc_type(void);

    // Immediate register group read operations
    /**
     * \brief Queries the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return Value of the specified rh850g4 GR group register
     */
    unsigned int read_GR(rh850g4::GR_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return Value of the specified rh850g4 WR group register
     */
    unsigned long long read_WR(rh850g4::WR_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return Value of the specified rh850g4 SR_SELID_0 group register
     */
    unsigned int read_SR_SELID_0(rh850g4::SR_SELID_0_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return Value of the specified rh850g4 SR_SELID_1 group register
     */
    unsigned int read_SR_SELID_1(rh850g4::SR_SELID_1_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return Value of the specified rh850g4 SR_SELID_2 group register
     */
    unsigned int read_SR_SELID_2(rh850g4::SR_SELID_2_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return Value of the specified rh850g4 SR_SELID_3 group register
     */
    unsigned int read_SR_SELID_3(rh850g4::SR_SELID_3_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return Value of the specified rh850g4 SR_SELID_4 group register
     */
    unsigned int read_SR_SELID_4(rh850g4::SR_SELID_4_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return Value of the specified rh850g4 SR_SELID_5 group register
     */
    unsigned int read_SR_SELID_5(rh850g4::SR_SELID_5_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return Value of the specified rh850g4 SR_SELID_9 group register
     */
    unsigned int read_SR_SELID_9(rh850g4::SR_SELID_9_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return Value of the specified rh850g4 SR_SELID_10 group register
     */
    unsigned int read_SR_SELID_10(rh850g4::SR_SELID_10_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return Value of the specified rh850g4 SR_SELID_11 group register
     */
    unsigned int read_SR_SELID_11(rh850g4::SR_SELID_11_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return Value of the specified rh850g4 SR_SELID_12 group register
     */
    unsigned int read_SR_SELID_12(rh850g4::SR_SELID_12_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return Value of the specified rh850g4 SR_SELID_13 group register
     */
    unsigned int read_SR_SELID_13(rh850g4::SR_SELID_13_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return Value of the specified rh850g4 SR_SELID_14 group register
     */
    unsigned int read_SR_SELID_14(rh850g4::SR_SELID_14_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return Value of the specified rh850g4 SR_SELID_15 group register
     */
    unsigned int read_SR_SELID_15(rh850g4::SR_SELID_15_index_t index);
    
    // Immediate register and register field write operations
    /**
     * \brief Sets the value of the rh850g4 translation time PC register
     * \param value New value of the rh850g4 translation time PC register
     */
    void write_trans_PC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r0 register
     * \param value New value of the rh850g4 r0 register
     */
    void write_r0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r1 register
     * \param value New value of the rh850g4 r1 register
     */
    void write_r1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r2 register
     * \param value New value of the rh850g4 r2 register
     */
    void write_r2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r3 register
     * \param value New value of the rh850g4 r3 register
     */
    void write_r3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r4 register
     * \param value New value of the rh850g4 r4 register
     */
    void write_r4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r5 register
     * \param value New value of the rh850g4 r5 register
     */
    void write_r5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r6 register
     * \param value New value of the rh850g4 r6 register
     */
    void write_r6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r7 register
     * \param value New value of the rh850g4 r7 register
     */
    void write_r7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r8 register
     * \param value New value of the rh850g4 r8 register
     */
    void write_r8(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r9 register
     * \param value New value of the rh850g4 r9 register
     */
    void write_r9(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r10 register
     * \param value New value of the rh850g4 r10 register
     */
    void write_r10(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r11 register
     * \param value New value of the rh850g4 r11 register
     */
    void write_r11(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r12 register
     * \param value New value of the rh850g4 r12 register
     */
    void write_r12(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r13 register
     * \param value New value of the rh850g4 r13 register
     */
    void write_r13(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r14 register
     * \param value New value of the rh850g4 r14 register
     */
    void write_r14(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r15 register
     * \param value New value of the rh850g4 r15 register
     */
    void write_r15(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r16 register
     * \param value New value of the rh850g4 r16 register
     */
    void write_r16(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r17 register
     * \param value New value of the rh850g4 r17 register
     */
    void write_r17(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r18 register
     * \param value New value of the rh850g4 r18 register
     */
    void write_r18(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r19 register
     * \param value New value of the rh850g4 r19 register
     */
    void write_r19(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r20 register
     * \param value New value of the rh850g4 r20 register
     */
    void write_r20(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r21 register
     * \param value New value of the rh850g4 r21 register
     */
    void write_r21(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r22 register
     * \param value New value of the rh850g4 r22 register
     */
    void write_r22(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r23 register
     * \param value New value of the rh850g4 r23 register
     */
    void write_r23(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r24 register
     * \param value New value of the rh850g4 r24 register
     */
    void write_r24(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r25 register
     * \param value New value of the rh850g4 r25 register
     */
    void write_r25(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r26 register
     * \param value New value of the rh850g4 r26 register
     */
    void write_r26(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r27 register
     * \param value New value of the rh850g4 r27 register
     */
    void write_r27(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r28 register
     * \param value New value of the rh850g4 r28 register
     */
    void write_r28(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r29 register
     * \param value New value of the rh850g4 r29 register
     */
    void write_r29(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r30 register
     * \param value New value of the rh850g4 r30 register
     */
    void write_r30(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 r31 register
     * \param value New value of the rh850g4 r31 register
     */
    void write_r31(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PC register
     * \param value New value of the rh850g4 PC register
     */
    void write_PC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 wr0 register
     * \param value New value of the rh850g4 wr0 register
     */
    void write_wr0(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr0_u register
     * \param value New value of the rh850g4 wr0_u register
     */
    void write_wr0_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr1 register
     * \param value New value of the rh850g4 wr1 register
     */
    void write_wr1(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr1_u register
     * \param value New value of the rh850g4 wr1_u register
     */
    void write_wr1_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr2 register
     * \param value New value of the rh850g4 wr2 register
     */
    void write_wr2(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr2_u register
     * \param value New value of the rh850g4 wr2_u register
     */
    void write_wr2_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr3 register
     * \param value New value of the rh850g4 wr3 register
     */
    void write_wr3(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr3_u register
     * \param value New value of the rh850g4 wr3_u register
     */
    void write_wr3_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr4 register
     * \param value New value of the rh850g4 wr4 register
     */
    void write_wr4(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr4_u register
     * \param value New value of the rh850g4 wr4_u register
     */
    void write_wr4_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr5 register
     * \param value New value of the rh850g4 wr5 register
     */
    void write_wr5(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr5_u register
     * \param value New value of the rh850g4 wr5_u register
     */
    void write_wr5_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr6 register
     * \param value New value of the rh850g4 wr6 register
     */
    void write_wr6(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr6_u register
     * \param value New value of the rh850g4 wr6_u register
     */
    void write_wr6_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr7 register
     * \param value New value of the rh850g4 wr7 register
     */
    void write_wr7(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr7_u register
     * \param value New value of the rh850g4 wr7_u register
     */
    void write_wr7_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr8 register
     * \param value New value of the rh850g4 wr8 register
     */
    void write_wr8(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr8_u register
     * \param value New value of the rh850g4 wr8_u register
     */
    void write_wr8_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr9 register
     * \param value New value of the rh850g4 wr9 register
     */
    void write_wr9(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr9_u register
     * \param value New value of the rh850g4 wr9_u register
     */
    void write_wr9_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr10 register
     * \param value New value of the rh850g4 wr10 register
     */
    void write_wr10(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr10_u register
     * \param value New value of the rh850g4 wr10_u register
     */
    void write_wr10_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr11 register
     * \param value New value of the rh850g4 wr11 register
     */
    void write_wr11(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr11_u register
     * \param value New value of the rh850g4 wr11_u register
     */
    void write_wr11_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr12 register
     * \param value New value of the rh850g4 wr12 register
     */
    void write_wr12(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr12_u register
     * \param value New value of the rh850g4 wr12_u register
     */
    void write_wr12_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr13 register
     * \param value New value of the rh850g4 wr13 register
     */
    void write_wr13(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr13_u register
     * \param value New value of the rh850g4 wr13_u register
     */
    void write_wr13_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr14 register
     * \param value New value of the rh850g4 wr14 register
     */
    void write_wr14(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr14_u register
     * \param value New value of the rh850g4 wr14_u register
     */
    void write_wr14_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr15 register
     * \param value New value of the rh850g4 wr15 register
     */
    void write_wr15(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr15_u register
     * \param value New value of the rh850g4 wr15_u register
     */
    void write_wr15_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr16 register
     * \param value New value of the rh850g4 wr16 register
     */
    void write_wr16(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr16_u register
     * \param value New value of the rh850g4 wr16_u register
     */
    void write_wr16_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr17 register
     * \param value New value of the rh850g4 wr17 register
     */
    void write_wr17(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr17_u register
     * \param value New value of the rh850g4 wr17_u register
     */
    void write_wr17_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr18 register
     * \param value New value of the rh850g4 wr18 register
     */
    void write_wr18(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr18_u register
     * \param value New value of the rh850g4 wr18_u register
     */
    void write_wr18_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr19 register
     * \param value New value of the rh850g4 wr19 register
     */
    void write_wr19(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr19_u register
     * \param value New value of the rh850g4 wr19_u register
     */
    void write_wr19_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr20 register
     * \param value New value of the rh850g4 wr20 register
     */
    void write_wr20(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr20_u register
     * \param value New value of the rh850g4 wr20_u register
     */
    void write_wr20_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr21 register
     * \param value New value of the rh850g4 wr21 register
     */
    void write_wr21(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr21_u register
     * \param value New value of the rh850g4 wr21_u register
     */
    void write_wr21_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr22 register
     * \param value New value of the rh850g4 wr22 register
     */
    void write_wr22(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr22_u register
     * \param value New value of the rh850g4 wr22_u register
     */
    void write_wr22_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr23 register
     * \param value New value of the rh850g4 wr23 register
     */
    void write_wr23(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr23_u register
     * \param value New value of the rh850g4 wr23_u register
     */
    void write_wr23_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr24 register
     * \param value New value of the rh850g4 wr24 register
     */
    void write_wr24(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr24_u register
     * \param value New value of the rh850g4 wr24_u register
     */
    void write_wr24_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr25 register
     * \param value New value of the rh850g4 wr25 register
     */
    void write_wr25(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr25_u register
     * \param value New value of the rh850g4 wr25_u register
     */
    void write_wr25_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr26 register
     * \param value New value of the rh850g4 wr26 register
     */
    void write_wr26(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr26_u register
     * \param value New value of the rh850g4 wr26_u register
     */
    void write_wr26_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr27 register
     * \param value New value of the rh850g4 wr27 register
     */
    void write_wr27(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr27_u register
     * \param value New value of the rh850g4 wr27_u register
     */
    void write_wr27_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr28 register
     * \param value New value of the rh850g4 wr28 register
     */
    void write_wr28(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr28_u register
     * \param value New value of the rh850g4 wr28_u register
     */
    void write_wr28_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr29 register
     * \param value New value of the rh850g4 wr29 register
     */
    void write_wr29(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr29_u register
     * \param value New value of the rh850g4 wr29_u register
     */
    void write_wr29_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr30 register
     * \param value New value of the rh850g4 wr30 register
     */
    void write_wr30(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr30_u register
     * \param value New value of the rh850g4 wr30_u register
     */
    void write_wr30_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr31 register
     * \param value New value of the rh850g4 wr31 register
     */
    void write_wr31(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 wr31_u register
     * \param value New value of the rh850g4 wr31_u register
     */
    void write_wr31_u(unsigned long long value);
    /**
     * \brief Sets the value of the rh850g4 EIPC register
     * \param value New value of the rh850g4 EIPC register
     */
    void write_EIPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIPSW register
     * \param value New value of the rh850g4 EIPSW register
     */
    void write_EIPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEPC register
     * \param value New value of the rh850g4 FEPC register
     */
    void write_FEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEPSW register
     * \param value New value of the rh850g4 FEPSW register
     */
    void write_FEPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSW register
     * \param value New value of the rh850g4 PSW register
     */
    void write_PSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSW_Z register field
     * \param value New value of the rh850g4 PSW_Z register field
     */
    void write_PSW_Z(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_S register field
     * \param value New value of the rh850g4 PSW_S register field
     */
    void write_PSW_S(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_OV register field
     * \param value New value of the rh850g4 PSW_OV register field
     */
    void write_PSW_OV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CY register field
     * \param value New value of the rh850g4 PSW_CY register field
     */
    void write_PSW_CY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_SAT register field
     * \param value New value of the rh850g4 PSW_SAT register field
     */
    void write_PSW_SAT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_ID register field
     * \param value New value of the rh850g4 PSW_ID register field
     */
    void write_PSW_ID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_EP register field
     * \param value New value of the rh850g4 PSW_EP register field
     */
    void write_PSW_EP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_NP register field
     * \param value New value of the rh850g4 PSW_NP register field
     */
    void write_PSW_NP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_EBV register field
     * \param value New value of the rh850g4 PSW_EBV register field
     */
    void write_PSW_EBV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CU0 register field
     * \param value New value of the rh850g4 PSW_CU0 register field
     */
    void write_PSW_CU0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CU1 register field
     * \param value New value of the rh850g4 PSW_CU1 register field
     */
    void write_PSW_CU1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_CU2 register field
     * \param value New value of the rh850g4 PSW_CU2 register field
     */
    void write_PSW_CU2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_EIMASK register field
     * \param value New value of the rh850g4 PSW_EIMASK register field
     */
    void write_PSW_EIMASK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSW_UM register field
     * \param value New value of the rh850g4 PSW_UM register field
     */
    void write_PSW_UM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR register
     * \param value New value of the rh850g4 FPSR register
     */
    void write_FPSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_CC register field
     * \param value New value of the rh850g4 FPSR_CC register field
     */
    void write_FPSR_CC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_FN register field
     * \param value New value of the rh850g4 FPSR_FN register field
     */
    void write_FPSR_FN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_IF register field
     * \param value New value of the rh850g4 FPSR_IF register field
     */
    void write_FPSR_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_PEM register field
     * \param value New value of the rh850g4 FPSR_PEM register field
     */
    void write_FPSR_PEM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_RM register field
     * \param value New value of the rh850g4 FPSR_RM register field
     */
    void write_FPSR_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_FS register field
     * \param value New value of the rh850g4 FPSR_FS register field
     */
    void write_FPSR_FS(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_XC register field
     * \param value New value of the rh850g4 FPSR_XC register field
     */
    void write_FPSR_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_XE register field
     * \param value New value of the rh850g4 FPSR_XE register field
     */
    void write_FPSR_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPSR_XP register field
     * \param value New value of the rh850g4 FPSR_XP register field
     */
    void write_FPSR_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPEPC register
     * \param value New value of the rh850g4 FPEPC register
     */
    void write_FPEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPST register
     * \param value New value of the rh850g4 FPST register
     */
    void write_FPST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPST_XC register field
     * \param value New value of the rh850g4 FPST_XC register field
     */
    void write_FPST_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPST_IF register field
     * \param value New value of the rh850g4 FPST_IF register field
     */
    void write_FPST_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPST_XP register field
     * \param value New value of the rh850g4 FPST_XP register field
     */
    void write_FPST_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPCC register
     * \param value New value of the rh850g4 FPCC register
     */
    void write_FPCC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPCC_CC register field
     * \param value New value of the rh850g4 FPCC_CC register field
     */
    void write_FPCC_CC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPCFG register
     * \param value New value of the rh850g4 FPCFG register
     */
    void write_FPCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPCFG_RM register field
     * \param value New value of the rh850g4 FPCFG_RM register field
     */
    void write_FPCFG_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FPCFG_XE register field
     * \param value New value of the rh850g4 FPCFG_XE register field
     */
    void write_FPCFG_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EIIC register
     * \param value New value of the rh850g4 EIIC register
     */
    void write_EIIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEIC register
     * \param value New value of the rh850g4 FEIC register
     */
    void write_FEIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSWH register
     * \param value New value of the rh850g4 PSWH register
     */
    void write_PSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PSWH_GPID register field
     * \param value New value of the rh850g4 PSWH_GPID register field
     */
    void write_PSWH_GPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PSWH_GM register field
     * \param value New value of the rh850g4 PSWH_GM register field
     */
    void write_PSWH_GM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 CTPC register
     * \param value New value of the rh850g4 CTPC register
     */
    void write_CTPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 CTPSW register
     * \param value New value of the rh850g4 CTPSW register
     */
    void write_CTPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIPSWH register
     * \param value New value of the rh850g4 EIPSWH register
     */
    void write_EIPSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIPSWH_GPID register field
     * \param value New value of the rh850g4 EIPSWH_GPID register field
     */
    void write_EIPSWH_GPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EIPSWH_GM register field
     * \param value New value of the rh850g4 EIPSWH_GM register field
     */
    void write_EIPSWH_GM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FEPSWH register
     * \param value New value of the rh850g4 FEPSWH register
     */
    void write_FEPSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEPSWH_GPID register field
     * \param value New value of the rh850g4 FEPSWH_GPID register field
     */
    void write_FEPSWH_GPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FEPSWH_GM register field
     * \param value New value of the rh850g4 FEPSWH_GM register field
     */
    void write_FEPSWH_GM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 CTBP register
     * \param value New value of the rh850g4 CTBP register
     */
    void write_CTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SNZCFG register
     * \param value New value of the rh850g4 SNZCFG register
     */
    void write_SNZCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EIWR register
     * \param value New value of the rh850g4 EIWR register
     */
    void write_EIWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FEWR register
     * \param value New value of the rh850g4 FEWR register
     */
    void write_FEWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SPID register
     * \param value New value of the rh850g4 SPID register
     */
    void write_SPID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SPIDLIST register
     * \param value New value of the rh850g4 SPIDLIST register
     */
    void write_SPIDLIST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBASE register
     * \param value New value of the rh850g4 RBASE register
     */
    void write_RBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBASE_RBASE register field
     * \param value New value of the rh850g4 RBASE_RBASE register field
     */
    void write_RBASE_RBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBASE_DV register field
     * \param value New value of the rh850g4 RBASE_DV register field
     */
    void write_RBASE_DV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 RBASE_RINT register field
     * \param value New value of the rh850g4 RBASE_RINT register field
     */
    void write_RBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EBASE register
     * \param value New value of the rh850g4 EBASE register
     */
    void write_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EBASE_EBASE register field
     * \param value New value of the rh850g4 EBASE_EBASE register field
     */
    void write_EBASE_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 EBASE_DV register field
     * \param value New value of the rh850g4 EBASE_DV register field
     */
    void write_EBASE_DV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 EBASE_RINT register field
     * \param value New value of the rh850g4 EBASE_RINT register field
     */
    void write_EBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTBP register
     * \param value New value of the rh850g4 INTBP register
     */
    void write_INTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCTL register
     * \param value New value of the rh850g4 MCTL register
     */
    void write_MCTL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCTL_UIC register field
     * \param value New value of the rh850g4 MCTL_UIC register field
     */
    void write_MCTL_UIC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PID register
     * \param value New value of the rh850g4 PID register
     */
    void write_PID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SVLOCK register
     * \param value New value of the rh850g4 SVLOCK register
     */
    void write_SVLOCK(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SCCFG register
     * \param value New value of the rh850g4 SCCFG register
     */
    void write_SCCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 SCCFG_SIZE register field
     * \param value New value of the rh850g4 SCCFG_SIZE register field
     */
    void write_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 SCBP register
     * \param value New value of the rh850g4 SCBP register
     */
    void write_SCBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 HVCFG register
     * \param value New value of the rh850g4 HVCFG register
     */
    void write_HVCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 HVCFG_HVE register field
     * \param value New value of the rh850g4 HVCFG_HVE register field
     */
    void write_HVCFG_HVE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG register
     * \param value New value of the rh850g4 GMCFG register
     */
    void write_GMCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GMP register field
     * \param value New value of the rh850g4 GMCFG_GMP register field
     */
    void write_GMCFG_GMP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_HMP register field
     * \param value New value of the rh850g4 GMCFG_HMP register field
     */
    void write_GMCFG_HMP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GSYSE register field
     * \param value New value of the rh850g4 GMCFG_GSYSE register field
     */
    void write_GMCFG_GSYSE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU0 register field
     * \param value New value of the rh850g4 GMCFG_GCU0 register field
     */
    void write_GMCFG_GCU0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU1 register field
     * \param value New value of the rh850g4 GMCFG_GCU1 register field
     */
    void write_GMCFG_GCU1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMCFG_GCU2 register field
     * \param value New value of the rh850g4 GMCFG_GCU2 register field
     */
    void write_GMCFG_GCU2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 HVSB register
     * \param value New value of the rh850g4 HVSB register
     */
    void write_HVSB(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PEID register
     * \param value New value of the rh850g4 PEID register
     */
    void write_PEID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BMID register
     * \param value New value of the rh850g4 BMID register
     */
    void write_BMID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BMID_BMID register field
     * \param value New value of the rh850g4 BMID_BMID register field
     */
    void write_BMID_BMID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MEA register
     * \param value New value of the rh850g4 MEA register
     */
    void write_MEA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MEI register
     * \param value New value of the rh850g4 MEI register
     */
    void write_MEI(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBCR0 register
     * \param value New value of the rh850g4 RBCR0 register
     */
    void write_RBCR0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBCR0_MD register field
     * \param value New value of the rh850g4 RBCR0_MD register field
     */
    void write_RBCR0_MD(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 RBCR0_BE register field
     * \param value New value of the rh850g4 RBCR0_BE register field
     */
    void write_RBCR0_BE(unsigned short value);
    /**
     * \brief Sets the value of the rh850g4 RBCR1 register
     * \param value New value of the rh850g4 RBCR1 register
     */
    void write_RBCR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBCR1_NC register field
     * \param value New value of the rh850g4 RBCR1_NC register field
     */
    void write_RBCR1_NC(unsigned short value);
    /**
     * \brief Sets the value of the rh850g4 RBNR register
     * \param value New value of the rh850g4 RBNR register
     */
    void write_RBNR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBNR_BN register field
     * \param value New value of the rh850g4 RBNR_BN register field
     */
    void write_RBNR_BN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 RBIP register
     * \param value New value of the rh850g4 RBIP register
     */
    void write_RBIP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBIP_RBIP register field
     * \param value New value of the rh850g4 RBIP_RBIP register field
     */
    void write_RBIP_RBIP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ISPR register
     * \param value New value of the rh850g4 ISPR register
     */
    void write_ISPR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ISPR_ISP register field
     * \param value New value of the rh850g4 ISPR_ISP register field
     */
    void write_ISPR_ISP(unsigned short value);
    /**
     * \brief Sets the value of the rh850g4 IMSR register
     * \param value New value of the rh850g4 IMSR register
     */
    void write_IMSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_EEIM register field
     * \param value New value of the rh850g4 IMSR_EEIM register field
     */
    void write_IMSR_EEIM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_EPLM register field
     * \param value New value of the rh850g4 IMSR_EPLM register field
     */
    void write_IMSR_EPLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_EID register field
     * \param value New value of the rh850g4 IMSR_EID register field
     */
    void write_IMSR_EID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_ENP register field
     * \param value New value of the rh850g4 IMSR_ENP register field
     */
    void write_IMSR_ENP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_FNP register field
     * \param value New value of the rh850g4 IMSR_FNP register field
     */
    void write_IMSR_FNP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEEIM register field
     * \param value New value of the rh850g4 IMSR_HEEIM register field
     */
    void write_IMSR_HEEIM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEPLM register field
     * \param value New value of the rh850g4 IMSR_HEPLM register field
     */
    void write_IMSR_HEPLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HEID register field
     * \param value New value of the rh850g4 IMSR_HEID register field
     */
    void write_IMSR_HEID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HENP register field
     * \param value New value of the rh850g4 IMSR_HENP register field
     */
    void write_IMSR_HENP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 IMSR_HFNP register field
     * \param value New value of the rh850g4 IMSR_HFNP register field
     */
    void write_IMSR_HFNP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICSR register
     * \param value New value of the rh850g4 ICSR register
     */
    void write_ICSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICSR_PMEI register field
     * \param value New value of the rh850g4 ICSR_PMEI register field
     */
    void write_ICSR_PMEI(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG register
     * \param value New value of the rh850g4 INTCFG register
     */
    void write_INTCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG_ISPC register field
     * \param value New value of the rh850g4 INTCFG_ISPC register field
     */
    void write_INTCFG_ISPC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG_EPL register field
     * \param value New value of the rh850g4 INTCFG_EPL register field
     */
    void write_INTCFG_EPL(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG_ULNR register field
     * \param value New value of the rh850g4 INTCFG_ULNR register field
     */
    void write_INTCFG_ULNR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 PLMR register
     * \param value New value of the rh850g4 PLMR register
     */
    void write_PLMR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PLMR_PLM register field
     * \param value New value of the rh850g4 PLMR_PLM register field
     */
    void write_PLMR_PLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR register
     * \param value New value of the rh850g4 FXSR register
     */
    void write_FXSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_FN register field
     * \param value New value of the rh850g4 FXSR_FN register field
     */
    void write_FXSR_FN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_IF register field
     * \param value New value of the rh850g4 FXSR_IF register field
     */
    void write_FXSR_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_PEM register field
     * \param value New value of the rh850g4 FXSR_PEM register field
     */
    void write_FXSR_PEM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_RM register field
     * \param value New value of the rh850g4 FXSR_RM register field
     */
    void write_FXSR_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_FS register field
     * \param value New value of the rh850g4 FXSR_FS register field
     */
    void write_FXSR_FS(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_XC register field
     * \param value New value of the rh850g4 FXSR_XC register field
     */
    void write_FXSR_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_XE register field
     * \param value New value of the rh850g4 FXSR_XE register field
     */
    void write_FXSR_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXSR_XP register field
     * \param value New value of the rh850g4 FXSR_XP register field
     */
    void write_FXSR_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXST register
     * \param value New value of the rh850g4 FXST register
     */
    void write_FXST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXST_XC register field
     * \param value New value of the rh850g4 FXST_XC register field
     */
    void write_FXST_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXST_IF register field
     * \param value New value of the rh850g4 FXST_IF register field
     */
    void write_FXST_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXST_XP register field
     * \param value New value of the rh850g4 FXST_XP register field
     */
    void write_FXST_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXINFO register
     * \param value New value of the rh850g4 FXINFO register
     */
    void write_FXINFO(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXINFO_NFPU register field
     * \param value New value of the rh850g4 FXINFO_NFPU register field
     */
    void write_FXINFO_NFPU(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXINFO_RSIZE register field
     * \param value New value of the rh850g4 FXINFO_RSIZE register field
     */
    void write_FXINFO_RSIZE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXCFG register
     * \param value New value of the rh850g4 FXCFG register
     */
    void write_FXCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXCFG_RM register field
     * \param value New value of the rh850g4 FXCFG_RM register field
     */
    void write_FXCFG_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXCFG_XE register field
     * \param value New value of the rh850g4 FXCFG_XE register field
     */
    void write_FXCFG_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC register
     * \param value New value of the rh850g4 FXXC register
     */
    void write_FXXC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC3 register field
     * \param value New value of the rh850g4 FXXC_XC3 register field
     */
    void write_FXXC_XC3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC2 register field
     * \param value New value of the rh850g4 FXXC_XC2 register field
     */
    void write_FXXC_XC2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC1 register field
     * \param value New value of the rh850g4 FXXC_XC1 register field
     */
    void write_FXXC_XC1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXC_XC0 register field
     * \param value New value of the rh850g4 FXXC_XC0 register field
     */
    void write_FXXC_XC0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP register
     * \param value New value of the rh850g4 FXXP register
     */
    void write_FXXP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP3 register field
     * \param value New value of the rh850g4 FXXP_XP3 register field
     */
    void write_FXXP_XP3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP2 register field
     * \param value New value of the rh850g4 FXXP_XP2 register field
     */
    void write_FXXP_XP2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP1 register field
     * \param value New value of the rh850g4 FXXP_XP1 register field
     */
    void write_FXXP_XP1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 FXXP_XP0 register field
     * \param value New value of the rh850g4 FXXP_XP0 register field
     */
    void write_FXXP_XP0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMEIPC register
     * \param value New value of the rh850g4 GMEIPC register
     */
    void write_GMEIPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEIPSW register
     * \param value New value of the rh850g4 GMEIPSW register
     */
    void write_GMEIPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEPC register
     * \param value New value of the rh850g4 GMFEPC register
     */
    void write_GMFEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEPSW register
     * \param value New value of the rh850g4 GMFEPSW register
     */
    void write_GMFEPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW register
     * \param value New value of the rh850g4 GMPSW register
     */
    void write_GMPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_Z register field
     * \param value New value of the rh850g4 GMPSW_Z register field
     */
    void write_GMPSW_Z(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_S register field
     * \param value New value of the rh850g4 GMPSW_S register field
     */
    void write_GMPSW_S(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_OV register field
     * \param value New value of the rh850g4 GMPSW_OV register field
     */
    void write_GMPSW_OV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CY register field
     * \param value New value of the rh850g4 GMPSW_CY register field
     */
    void write_GMPSW_CY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_SAT register field
     * \param value New value of the rh850g4 GMPSW_SAT register field
     */
    void write_GMPSW_SAT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_ID register field
     * \param value New value of the rh850g4 GMPSW_ID register field
     */
    void write_GMPSW_ID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EP register field
     * \param value New value of the rh850g4 GMPSW_EP register field
     */
    void write_GMPSW_EP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_NP register field
     * \param value New value of the rh850g4 GMPSW_NP register field
     */
    void write_GMPSW_NP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EBV register field
     * \param value New value of the rh850g4 GMPSW_EBV register field
     */
    void write_GMPSW_EBV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU0 register field
     * \param value New value of the rh850g4 GMPSW_CU0 register field
     */
    void write_GMPSW_CU0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU1 register field
     * \param value New value of the rh850g4 GMPSW_CU1 register field
     */
    void write_GMPSW_CU1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_CU2 register field
     * \param value New value of the rh850g4 GMPSW_CU2 register field
     */
    void write_GMPSW_CU2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_EIMASK register field
     * \param value New value of the rh850g4 GMPSW_EIMASK register field
     */
    void write_GMPSW_EIMASK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPSW_UM register field
     * \param value New value of the rh850g4 GMPSW_UM register field
     */
    void write_GMPSW_UM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMMEA register
     * \param value New value of the rh850g4 GMMEA register
     */
    void write_GMMEA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMMEI register
     * \param value New value of the rh850g4 GMMEI register
     */
    void write_GMMEI(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEIIC register
     * \param value New value of the rh850g4 GMEIIC register
     */
    void write_GMEIIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEIC register
     * \param value New value of the rh850g4 GMFEIC register
     */
    void write_GMFEIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMSPID register
     * \param value New value of the rh850g4 GMSPID register
     */
    void write_GMSPID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMSPIDLIST register
     * \param value New value of the rh850g4 GMSPIDLIST register
     */
    void write_GMSPIDLIST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE register
     * \param value New value of the rh850g4 GMEBASE register
     */
    void write_GMEBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_EBASE register field
     * \param value New value of the rh850g4 GMEBASE_EBASE register field
     */
    void write_GMEBASE_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_DV register field
     * \param value New value of the rh850g4 GMEBASE_DV register field
     */
    void write_GMEBASE_DV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMEBASE_RINT register field
     * \param value New value of the rh850g4 GMEBASE_RINT register field
     */
    void write_GMEBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMINTBP register
     * \param value New value of the rh850g4 GMINTBP register
     */
    void write_GMINTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG register
     * \param value New value of the rh850g4 GMINTCFG register
     */
    void write_GMINTCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_ISPC register field
     * \param value New value of the rh850g4 GMINTCFG_ISPC register field
     */
    void write_GMINTCFG_ISPC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_EPL register field
     * \param value New value of the rh850g4 GMINTCFG_EPL register field
     */
    void write_GMINTCFG_EPL(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMINTCFG_ULNR register field
     * \param value New value of the rh850g4 GMINTCFG_ULNR register field
     */
    void write_GMINTCFG_ULNR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMPLMR register
     * \param value New value of the rh850g4 GMPLMR register
     */
    void write_GMPLMR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPLMR_PLM register field
     * \param value New value of the rh850g4 GMPLMR_PLM register field
     */
    void write_GMPLMR_PLM(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 GMSVLOCK register
     * \param value New value of the rh850g4 GMSVLOCK register
     */
    void write_GMSVLOCK(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMMPM register
     * \param value New value of the rh850g4 GMMPM register
     */
    void write_GMMPM(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMEIWR register
     * \param value New value of the rh850g4 GMEIWR register
     */
    void write_GMEIWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMFEWR register
     * \param value New value of the rh850g4 GMFEWR register
     */
    void write_GMFEWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 GMPEID register
     * \param value New value of the rh850g4 GMPEID register
     */
    void write_GMPEID(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPM register
     * \param value New value of the rh850g4 MPM register
     */
    void write_MPM(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG register
     * \param value New value of the rh850g4 MPCFG register
     */
    void write_MPCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_NMPUE register field
     * \param value New value of the rh850g4 MPCFG_NMPUE register field
     */
    void write_MPCFG_NMPUE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_HBE register field
     * \param value New value of the rh850g4 MPCFG_HBE register field
     */
    void write_MPCFG_HBE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_ARCH register field
     * \param value New value of the rh850g4 MPCFG_ARCH register field
     */
    void write_MPCFG_ARCH(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_NBK register field
     * \param value New value of the rh850g4 MPCFG_NBK register field
     */
    void write_MPCFG_NBK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPCFG_DMDP register field
     * \param value New value of the rh850g4 MPCFG_DMDP register field
     */
    void write_MPCFG_DMDP(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCA register
     * \param value New value of the rh850g4 MCA register
     */
    void write_MCA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCS register
     * \param value New value of the rh850g4 MCS register
     */
    void write_MCS(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCC register
     * \param value New value of the rh850g4 MCC register
     */
    void write_MCC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCR register
     * \param value New value of the rh850g4 MCR register
     */
    void write_MCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HSXE register field
     * \param value New value of the rh850g4 MCR_HSXE register field
     */
    void write_MCR_HSXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HSWE register field
     * \param value New value of the rh850g4 MCR_HSWE register field
     */
    void write_MCR_HSWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HSRE register field
     * \param value New value of the rh850g4 MCR_HSRE register field
     */
    void write_MCR_HSRE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HUXE register field
     * \param value New value of the rh850g4 MCR_HUXE register field
     */
    void write_MCR_HUXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HUWE register field
     * \param value New value of the rh850g4 MCR_HUWE register field
     */
    void write_MCR_HUWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_HURE register field
     * \param value New value of the rh850g4 MCR_HURE register field
     */
    void write_MCR_HURE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GSXE register field
     * \param value New value of the rh850g4 MCR_GSXE register field
     */
    void write_MCR_GSXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GSWE register field
     * \param value New value of the rh850g4 MCR_GSWE register field
     */
    void write_MCR_GSWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GSRE register field
     * \param value New value of the rh850g4 MCR_GSRE register field
     */
    void write_MCR_GSRE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GUXE register field
     * \param value New value of the rh850g4 MCR_GUXE register field
     */
    void write_MCR_GUXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GUWE register field
     * \param value New value of the rh850g4 MCR_GUWE register field
     */
    void write_MCR_GUWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_GURE register field
     * \param value New value of the rh850g4 MCR_GURE register field
     */
    void write_MCR_GURE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_OV register field
     * \param value New value of the rh850g4 MCR_OV register field
     */
    void write_MCR_OV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_SXE register field
     * \param value New value of the rh850g4 MCR_SXE register field
     */
    void write_MCR_SXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_SWE register field
     * \param value New value of the rh850g4 MCR_SWE register field
     */
    void write_MCR_SWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_SRE register field
     * \param value New value of the rh850g4 MCR_SRE register field
     */
    void write_MCR_SRE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_UXE register field
     * \param value New value of the rh850g4 MCR_UXE register field
     */
    void write_MCR_UXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_UWE register field
     * \param value New value of the rh850g4 MCR_UWE register field
     */
    void write_MCR_UWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCR_URE register field
     * \param value New value of the rh850g4 MCR_URE register field
     */
    void write_MCR_URE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MCI register
     * \param value New value of the rh850g4 MCI register
     */
    void write_MCI(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MCI_SPID register field
     * \param value New value of the rh850g4 MCI_SPID register field
     */
    void write_MCI_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPIDX register
     * \param value New value of the rh850g4 MPIDX register
     */
    void write_MPIDX(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPIDX_IDX register field
     * \param value New value of the rh850g4 MPIDX_IDX register field
     */
    void write_MPIDX_IDX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPBK register
     * \param value New value of the rh850g4 MPBK register
     */
    void write_MPBK(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPBK_BK register field
     * \param value New value of the rh850g4 MPBK_BK register field
     */
    void write_MPBK_BK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPLA register
     * \param value New value of the rh850g4 MPLA register
     */
    void write_MPLA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPUA register
     * \param value New value of the rh850g4 MPUA register
     */
    void write_MPUA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPAT register
     * \param value New value of the rh850g4 MPAT register
     */
    void write_MPAT(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID7 register field
     * \param value New value of the rh850g4 MPAT_WMPID7 register field
     */
    void write_MPAT_WMPID7(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID6 register field
     * \param value New value of the rh850g4 MPAT_WMPID6 register field
     */
    void write_MPAT_WMPID6(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID5 register field
     * \param value New value of the rh850g4 MPAT_WMPID5 register field
     */
    void write_MPAT_WMPID5(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID4 register field
     * \param value New value of the rh850g4 MPAT_WMPID4 register field
     */
    void write_MPAT_WMPID4(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID3 register field
     * \param value New value of the rh850g4 MPAT_WMPID3 register field
     */
    void write_MPAT_WMPID3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID2 register field
     * \param value New value of the rh850g4 MPAT_WMPID2 register field
     */
    void write_MPAT_WMPID2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID1 register field
     * \param value New value of the rh850g4 MPAT_WMPID1 register field
     */
    void write_MPAT_WMPID1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WMPID0 register field
     * \param value New value of the rh850g4 MPAT_WMPID0 register field
     */
    void write_MPAT_WMPID0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID7 register field
     * \param value New value of the rh850g4 MPAT_RMPID7 register field
     */
    void write_MPAT_RMPID7(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID6 register field
     * \param value New value of the rh850g4 MPAT_RMPID6 register field
     */
    void write_MPAT_RMPID6(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID5 register field
     * \param value New value of the rh850g4 MPAT_RMPID5 register field
     */
    void write_MPAT_RMPID5(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID4 register field
     * \param value New value of the rh850g4 MPAT_RMPID4 register field
     */
    void write_MPAT_RMPID4(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID3 register field
     * \param value New value of the rh850g4 MPAT_RMPID3 register field
     */
    void write_MPAT_RMPID3(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID2 register field
     * \param value New value of the rh850g4 MPAT_RMPID2 register field
     */
    void write_MPAT_RMPID2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID1 register field
     * \param value New value of the rh850g4 MPAT_RMPID1 register field
     */
    void write_MPAT_RMPID1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RMPID0 register field
     * \param value New value of the rh850g4 MPAT_RMPID0 register field
     */
    void write_MPAT_RMPID0(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_WG register field
     * \param value New value of the rh850g4 MPAT_WG register field
     */
    void write_MPAT_WG(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_RG register field
     * \param value New value of the rh850g4 MPAT_RG register field
     */
    void write_MPAT_RG(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_E register field
     * \param value New value of the rh850g4 MPAT_E register field
     */
    void write_MPAT_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_SX register field
     * \param value New value of the rh850g4 MPAT_SX register field
     */
    void write_MPAT_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_SW register field
     * \param value New value of the rh850g4 MPAT_SW register field
     */
    void write_MPAT_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_SR register field
     * \param value New value of the rh850g4 MPAT_SR register field
     */
    void write_MPAT_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_UX register field
     * \param value New value of the rh850g4 MPAT_UX register field
     */
    void write_MPAT_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_UW register field
     * \param value New value of the rh850g4 MPAT_UW register field
     */
    void write_MPAT_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPAT_UR register field
     * \param value New value of the rh850g4 MPAT_UR register field
     */
    void write_MPAT_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID0 register
     * \param value New value of the rh850g4 MPID0 register
     */
    void write_MPID0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID0_SPID register field
     * \param value New value of the rh850g4 MPID0_SPID register field
     */
    void write_MPID0_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID1 register
     * \param value New value of the rh850g4 MPID1 register
     */
    void write_MPID1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID1_SPID register field
     * \param value New value of the rh850g4 MPID1_SPID register field
     */
    void write_MPID1_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID2 register
     * \param value New value of the rh850g4 MPID2 register
     */
    void write_MPID2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID2_SPID register field
     * \param value New value of the rh850g4 MPID2_SPID register field
     */
    void write_MPID2_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID3 register
     * \param value New value of the rh850g4 MPID3 register
     */
    void write_MPID3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID3_SPID register field
     * \param value New value of the rh850g4 MPID3_SPID register field
     */
    void write_MPID3_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID4 register
     * \param value New value of the rh850g4 MPID4 register
     */
    void write_MPID4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID4_SPID register field
     * \param value New value of the rh850g4 MPID4_SPID register field
     */
    void write_MPID4_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID5 register
     * \param value New value of the rh850g4 MPID5 register
     */
    void write_MPID5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID5_SPID register field
     * \param value New value of the rh850g4 MPID5_SPID register field
     */
    void write_MPID5_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID6 register
     * \param value New value of the rh850g4 MPID6 register
     */
    void write_MPID6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID6_SPID register field
     * \param value New value of the rh850g4 MPID6_SPID register field
     */
    void write_MPID6_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 MPID7 register
     * \param value New value of the rh850g4 MPID7 register
     */
    void write_MPID7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID7_SPID register field
     * \param value New value of the rh850g4 MPID7_SPID register field
     */
    void write_MPID7_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL register
     * \param value New value of the rh850g4 ICTAGL register
     */
    void write_ICTAGL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_LPN register field
     * \param value New value of the rh850g4 ICTAGL_LPN register field
     */
    void write_ICTAGL_LPN(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_LRU register field
     * \param value New value of the rh850g4 ICTAGL_LRU register field
     */
    void write_ICTAGL_LRU(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_L register field
     * \param value New value of the rh850g4 ICTAGL_L register field
     */
    void write_ICTAGL_L(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL_V register field
     * \param value New value of the rh850g4 ICTAGL_V register field
     */
    void write_ICTAGL_V(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH register
     * \param value New value of the rh850g4 ICTAGH register
     */
    void write_ICTAGH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_WD register field
     * \param value New value of the rh850g4 ICTAGH_WD register field
     */
    void write_ICTAGH_WD(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_PD register field
     * \param value New value of the rh850g4 ICTAGH_PD register field
     */
    void write_ICTAGH_PD(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_WT register field
     * \param value New value of the rh850g4 ICTAGH_WT register field
     */
    void write_ICTAGH_WT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_PT register field
     * \param value New value of the rh850g4 ICTAGH_PT register field
     */
    void write_ICTAGH_PT(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_DATAECC register field
     * \param value New value of the rh850g4 ICTAGH_DATAECC register field
     */
    void write_ICTAGH_DATAECC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH_TAGECC register field
     * \param value New value of the rh850g4 ICTAGH_TAGECC register field
     */
    void write_ICTAGH_TAGECC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICDATL register
     * \param value New value of the rh850g4 ICDATL register
     */
    void write_ICDATL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICDATH register
     * \param value New value of the rh850g4 ICDATH register
     */
    void write_ICDATH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL register
     * \param value New value of the rh850g4 ICCTRL register
     */
    void write_ICCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_PBS register field
     * \param value New value of the rh850g4 ICCTRL_PBS register field
     */
    void write_ICCTRL_PBS(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHCLR register field
     * \param value New value of the rh850g4 ICCTRL_ICHCLR register field
     */
    void write_ICCTRL_ICHCLR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEIV register field
     * \param value New value of the rh850g4 ICCTRL_ICHEIV register field
     */
    void write_ICCTRL_ICHEIV(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEMK register field
     * \param value New value of the rh850g4 ICCTRL_ICHEMK register field
     */
    void write_ICCTRL_ICHEMK(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCTRL_ICHEN register field
     * \param value New value of the rh850g4 ICCTRL_ICHEN register field
     */
    void write_ICCTRL_ICHEN(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG register
     * \param value New value of the rh850g4 ICCFG register
     */
    void write_ICCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHSIZE register field
     * \param value New value of the rh850g4 ICCFG_ICHSIZE register field
     */
    void write_ICCFG_ICHSIZE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHLINE register field
     * \param value New value of the rh850g4 ICCFG_ICHLINE register field
     */
    void write_ICCFG_ICHLINE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICCFG_ICHWAY register field
     * \param value New value of the rh850g4 ICCFG_ICHWAY register field
     */
    void write_ICCFG_ICHWAY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR register
     * \param value New value of the rh850g4 ICERR register
     */
    void write_ICERR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_CISTW register field
     * \param value New value of the rh850g4 ICERR_CISTW register field
     */
    void write_ICERR_CISTW(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESAFE register field
     * \param value New value of the rh850g4 ICERR_ESAFE register field
     */
    void write_ICERR_ESAFE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESMH register field
     * \param value New value of the rh850g4 ICERR_ESMH register field
     */
    void write_ICERR_ESMH(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESPBSE register field
     * \param value New value of the rh850g4 ICERR_ESPBSE register field
     */
    void write_ICERR_ESPBSE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESTE1 register field
     * \param value New value of the rh850g4 ICERR_ESTE1 register field
     */
    void write_ICERR_ESTE1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESTE2 register field
     * \param value New value of the rh850g4 ICERR_ESTE2 register field
     */
    void write_ICERR_ESTE2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESDC register field
     * \param value New value of the rh850g4 ICERR_ESDC register field
     */
    void write_ICERR_ESDC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ESDE register field
     * \param value New value of the rh850g4 ICERR_ESDE register field
     */
    void write_ICERR_ESDE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMAFE register field
     * \param value New value of the rh850g4 ICERR_ERMAFE register field
     */
    void write_ICERR_ERMAFE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMMH register field
     * \param value New value of the rh850g4 ICERR_ERMMH register field
     */
    void write_ICERR_ERMMH(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMPBSE register field
     * \param value New value of the rh850g4 ICERR_ERMPBSE register field
     */
    void write_ICERR_ERMPBSE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMTE1 register field
     * \param value New value of the rh850g4 ICERR_ERMTE1 register field
     */
    void write_ICERR_ERMTE1(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMTE2 register field
     * \param value New value of the rh850g4 ICERR_ERMTE2 register field
     */
    void write_ICERR_ERMTE2(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMDC register field
     * \param value New value of the rh850g4 ICERR_ERMDC register field
     */
    void write_ICERR_ERMDC(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ERMDE register field
     * \param value New value of the rh850g4 ICERR_ERMDE register field
     */
    void write_ICERR_ERMDE(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHEWY register field
     * \param value New value of the rh850g4 ICERR_ICHEWY register field
     */
    void write_ICERR_ICHEWY(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHEIX register field
     * \param value New value of the rh850g4 ICERR_ICHEIX register field
     */
    void write_ICERR_ICHEIX(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHERQ register field
     * \param value New value of the rh850g4 ICERR_ICHERQ register field
     */
    void write_ICERR_ICHERQ(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHED register field
     * \param value New value of the rh850g4 ICERR_ICHED register field
     */
    void write_ICERR_ICHED(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHET register field
     * \param value New value of the rh850g4 ICERR_ICHET register field
     */
    void write_ICERR_ICHET(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 ICERR_ICHERR register field
     * \param value New value of the rh850g4 ICERR_ICHERR register field
     */
    void write_ICERR_ICHERR(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 TSCOUNTL register
     * \param value New value of the rh850g4 TSCOUNTL register
     */
    void write_TSCOUNTL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 TSCOUNTH register
     * \param value New value of the rh850g4 TSCOUNTH register
     */
    void write_TSCOUNTH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 TSCTRL register
     * \param value New value of the rh850g4 TSCTRL register
     */
    void write_TSCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMUMCTRL register
     * \param value New value of the rh850g4 PMUMCTRL register
     */
    void write_PMUMCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMGMCTRL register
     * \param value New value of the rh850g4 PMGMCTRL register
     */
    void write_PMGMCTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT0 register
     * \param value New value of the rh850g4 PMCOUNT0 register
     */
    void write_PMCOUNT0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL0 register
     * \param value New value of the rh850g4 PMCTRL0 register
     */
    void write_PMCTRL0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT1 register
     * \param value New value of the rh850g4 PMCOUNT1 register
     */
    void write_PMCOUNT1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL1 register
     * \param value New value of the rh850g4 PMCTRL1 register
     */
    void write_PMCTRL1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT2 register
     * \param value New value of the rh850g4 PMCOUNT2 register
     */
    void write_PMCOUNT2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL2 register
     * \param value New value of the rh850g4 PMCTRL2 register
     */
    void write_PMCTRL2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT3 register
     * \param value New value of the rh850g4 PMCOUNT3 register
     */
    void write_PMCOUNT3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL3 register
     * \param value New value of the rh850g4 PMCTRL3 register
     */
    void write_PMCTRL3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL4 register
     * \param value New value of the rh850g4 PMCTRL4 register
     */
    void write_PMCTRL4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL5 register
     * \param value New value of the rh850g4 PMCTRL5 register
     */
    void write_PMCTRL5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL6 register
     * \param value New value of the rh850g4 PMCTRL6 register
     */
    void write_PMCTRL6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCTRL7 register
     * \param value New value of the rh850g4 PMCTRL7 register
     */
    void write_PMCTRL7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT4 register
     * \param value New value of the rh850g4 PMCOUNT4 register
     */
    void write_PMCOUNT4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT5 register
     * \param value New value of the rh850g4 PMCOUNT5 register
     */
    void write_PMCOUNT5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT6 register
     * \param value New value of the rh850g4 PMCOUNT6 register
     */
    void write_PMCOUNT6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMCOUNT7 register
     * \param value New value of the rh850g4 PMCOUNT7 register
     */
    void write_PMCOUNT7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND0 register
     * \param value New value of the rh850g4 PMSUBCND0 register
     */
    void write_PMSUBCND0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND1 register
     * \param value New value of the rh850g4 PMSUBCND1 register
     */
    void write_PMSUBCND1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND2 register
     * \param value New value of the rh850g4 PMSUBCND2 register
     */
    void write_PMSUBCND2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND3 register
     * \param value New value of the rh850g4 PMSUBCND3 register
     */
    void write_PMSUBCND3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND4 register
     * \param value New value of the rh850g4 PMSUBCND4 register
     */
    void write_PMSUBCND4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND5 register
     * \param value New value of the rh850g4 PMSUBCND5 register
     */
    void write_PMSUBCND5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND6 register
     * \param value New value of the rh850g4 PMSUBCND6 register
     */
    void write_PMSUBCND6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PMSUBCND7 register
     * \param value New value of the rh850g4 PMSUBCND7 register
     */
    void write_PMSUBCND7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBGEN register
     * \param value New value of the rh850g4 DBGEN register
     */
    void write_DBGEN(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBPSWH register
     * \param value New value of the rh850g4 DBPSWH register
     */
    void write_DBPSWH(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBIC register
     * \param value New value of the rh850g4 DBIC register
     */
    void write_DBIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBPC register
     * \param value New value of the rh850g4 DBPC register
     */
    void write_DBPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBPSW register
     * \param value New value of the rh850g4 DBPSW register
     */
    void write_DBPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DIR0 register
     * \param value New value of the rh850g4 DIR0 register
     */
    void write_DIR0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DIR1 register
     * \param value New value of the rh850g4 DIR1 register
     */
    void write_DIR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BPC register
     * \param value New value of the rh850g4 BPC register
     */
    void write_BPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BPAV register
     * \param value New value of the rh850g4 BPAV register
     */
    void write_BPAV(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BPAM register
     * \param value New value of the rh850g4 BPAM register
     */
    void write_BPAM(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBCFG register
     * \param value New value of the rh850g4 DBCFG register
     */
    void write_DBCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DBWR register
     * \param value New value of the rh850g4 DBWR register
     */
    void write_DBWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSTEST0 register
     * \param value New value of the rh850g4 LSTEST0 register
     */
    void write_LSTEST0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSTEST1 register
     * \param value New value of the rh850g4 LSTEST1 register
     */
    void write_LSTEST1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSCFG register
     * \param value New value of the rh850g4 LSCFG register
     */
    void write_LSCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICBKEY register
     * \param value New value of the rh850g4 ICBKEY register
     */
    void write_ICBKEY(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 IFCR register
     * \param value New value of the rh850g4 IFCR register
     */
    void write_IFCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 IFCR1 register
     * \param value New value of the rh850g4 IFCR1 register
     */
    void write_IFCR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPCTRL0 register
     * \param value New value of the rh850g4 BRPCTRL0 register
     */
    void write_BRPCTRL0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPCTRL1 register
     * \param value New value of the rh850g4 BRPCTRL1 register
     */
    void write_BRPCTRL1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPCFG register
     * \param value New value of the rh850g4 BRPCFG register
     */
    void write_BRPCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPACTRL register
     * \param value New value of the rh850g4 BRPACTRL register
     */
    void write_BRPACTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 BRPDATA register
     * \param value New value of the rh850g4 BRPDATA register
     */
    void write_BRPDATA(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DCBKEY register
     * \param value New value of the rh850g4 DCBKEY register
     */
    void write_DCBKEY(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSUCR register
     * \param value New value of the rh850g4 LSUCR register
     */
    void write_LSUCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSULNK0 register
     * \param value New value of the rh850g4 LSULNK0 register
     */
    void write_LSULNK0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 LSULNK1 register
     * \param value New value of the rh850g4 LSULNK1 register
     */
    void write_LSULNK1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RLCR register
     * \param value New value of the rh850g4 L1RLCR register
     */
    void write_L1RLCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RLNK0 register
     * \param value New value of the rh850g4 L1RLNK0 register
     */
    void write_L1RLNK0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RLNK1 register
     * \param value New value of the rh850g4 L1RLNK1 register
     */
    void write_L1RLNK1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 L1RCFG register
     * \param value New value of the rh850g4 L1RCFG register
     */
    void write_L1RCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DECFG register
     * \param value New value of the rh850g4 DECFG register
     */
    void write_DECFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DECTRL register
     * \param value New value of the rh850g4 DECTRL register
     */
    void write_DECTRL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 DEVDS register
     * \param value New value of the rh850g4 DEVDS register
     */
    void write_DEVDS(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBCR register
     * \param value New value of the rh850g4 RDBCR register
     */
    void write_RDBCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBACR register
     * \param value New value of the rh850g4 RDBACR register
     */
    void write_RDBACR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBATAG register
     * \param value New value of the rh850g4 RDBATAG register
     */
    void write_RDBATAG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT0 register
     * \param value New value of the rh850g4 RDBADAT0 register
     */
    void write_RDBADAT0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT1 register
     * \param value New value of the rh850g4 RDBADAT1 register
     */
    void write_RDBADAT1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT2 register
     * \param value New value of the rh850g4 RDBADAT2 register
     */
    void write_RDBADAT2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBADAT3 register
     * \param value New value of the rh850g4 RDBADAT3 register
     */
    void write_RDBADAT3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RDBSTAT register
     * \param value New value of the rh850g4 RDBSTAT register
     */
    void write_RDBSTAT(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 hv_func_support register
     * \param value New value of the rh850g4 hv_func_support register
     */
    void write_hv_func_support(unsigned char value);
    /**
     * \brief Sets the value of the rh850g4 swd0pcad_exec_count register
     * \param value New value of the rh850g4 swd0pcad_exec_count register
     */
    void write_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 swd0pcad_val register
     * \param value New value of the rh850g4 swd0pcad_val register
     */
    void write_swd0pcad_val(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 mem_acc_type register
     * \param value New value of the rh850g4 mem_acc_type register
     */
    void write_mem_acc_type(unsigned int value);

    // Immediate register group write operations
    /**
     * \brief Sets the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value New value of the specified rh850g4 GR group register
     */
    void write_GR(rh850g4::GR_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value New value of the specified rh850g4 WR group register
     */
    void write_WR(rh850g4::WR_index_t index, unsigned long long value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value New value of the specified rh850g4 SR_SELID_0 group register
     */
    void write_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value New value of the specified rh850g4 SR_SELID_1 group register
     */
    void write_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value New value of the specified rh850g4 SR_SELID_2 group register
     */
    void write_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value New value of the specified rh850g4 SR_SELID_3 group register
     */
    void write_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value New value of the specified rh850g4 SR_SELID_4 group register
     */
    void write_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value New value of the specified rh850g4 SR_SELID_5 group register
     */
    void write_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value New value of the specified rh850g4 SR_SELID_9 group register
     */
    void write_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value New value of the specified rh850g4 SR_SELID_10 group register
     */
    void write_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value New value of the specified rh850g4 SR_SELID_11 group register
     */
    void write_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value New value of the specified rh850g4 SR_SELID_12 group register
     */
    void write_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value New value of the specified rh850g4 SR_SELID_13 group register
     */
    void write_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value New value of the specified rh850g4 SR_SELID_14 group register
     */
    void write_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value New value of the specified rh850g4 SR_SELID_15 group register
     */
    void write_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value);

    // Immediate register and register field increment operations
    /**
     * \brief Increments the value of the rh850g4 translation time PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 translation time PC register
     */
    void increment_trans_PC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r0 register
     */
    void increment_r0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r1 register
     */
    void increment_r1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r2 register
     */
    void increment_r2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r3 register
     */
    void increment_r3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r4 register
     */
    void increment_r4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r5 register
     */
    void increment_r5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r6 register
     */
    void increment_r6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r7 register
     */
    void increment_r7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r8 register
     */
    void increment_r8(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r9 register
     */
    void increment_r9(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r10 register
     */
    void increment_r10(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r11 register
     */
    void increment_r11(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r12 register
     */
    void increment_r12(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r13 register
     */
    void increment_r13(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r14 register
     */
    void increment_r14(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r15 register
     */
    void increment_r15(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r16 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r16 register
     */
    void increment_r16(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r17 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r17 register
     */
    void increment_r17(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r18 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r18 register
     */
    void increment_r18(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r19 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r19 register
     */
    void increment_r19(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r20 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r20 register
     */
    void increment_r20(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r21 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r21 register
     */
    void increment_r21(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r22 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r22 register
     */
    void increment_r22(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r23 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r23 register
     */
    void increment_r23(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r24 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r24 register
     */
    void increment_r24(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r25 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r25 register
     */
    void increment_r25(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r26 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r26 register
     */
    void increment_r26(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r27 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r27 register
     */
    void increment_r27(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r28 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r28 register
     */
    void increment_r28(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r29 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r29 register
     */
    void increment_r29(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r30 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r30 register
     */
    void increment_r30(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 r31 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 r31 register
     */
    void increment_r31(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PC register
     */
    void increment_PC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 wr0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr0 register
     */
    void increment_wr0(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr0_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr0_u register
     */
    void increment_wr0_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr1 register
     */
    void increment_wr1(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr1_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr1_u register
     */
    void increment_wr1_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr2 register
     */
    void increment_wr2(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr2_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr2_u register
     */
    void increment_wr2_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr3 register
     */
    void increment_wr3(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr3_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr3_u register
     */
    void increment_wr3_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr4 register
     */
    void increment_wr4(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr4_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr4_u register
     */
    void increment_wr4_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr5 register
     */
    void increment_wr5(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr5_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr5_u register
     */
    void increment_wr5_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr6 register
     */
    void increment_wr6(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr6_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr6_u register
     */
    void increment_wr6_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr7 register
     */
    void increment_wr7(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr7_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr7_u register
     */
    void increment_wr7_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr8 register
     */
    void increment_wr8(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr8_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr8_u register
     */
    void increment_wr8_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr9 register
     */
    void increment_wr9(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr9_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr9_u register
     */
    void increment_wr9_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr10 register
     */
    void increment_wr10(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr10_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr10_u register
     */
    void increment_wr10_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr11 register
     */
    void increment_wr11(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr11_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr11_u register
     */
    void increment_wr11_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr12 register
     */
    void increment_wr12(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr12_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr12_u register
     */
    void increment_wr12_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr13 register
     */
    void increment_wr13(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr13_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr13_u register
     */
    void increment_wr13_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr14 register
     */
    void increment_wr14(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr14_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr14_u register
     */
    void increment_wr14_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr15 register
     */
    void increment_wr15(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr15_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr15_u register
     */
    void increment_wr15_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr16 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr16 register
     */
    void increment_wr16(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr16_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr16_u register
     */
    void increment_wr16_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr17 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr17 register
     */
    void increment_wr17(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr17_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr17_u register
     */
    void increment_wr17_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr18 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr18 register
     */
    void increment_wr18(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr18_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr18_u register
     */
    void increment_wr18_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr19 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr19 register
     */
    void increment_wr19(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr19_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr19_u register
     */
    void increment_wr19_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr20 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr20 register
     */
    void increment_wr20(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr20_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr20_u register
     */
    void increment_wr20_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr21 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr21 register
     */
    void increment_wr21(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr21_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr21_u register
     */
    void increment_wr21_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr22 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr22 register
     */
    void increment_wr22(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr22_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr22_u register
     */
    void increment_wr22_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr23 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr23 register
     */
    void increment_wr23(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr23_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr23_u register
     */
    void increment_wr23_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr24 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr24 register
     */
    void increment_wr24(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr24_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr24_u register
     */
    void increment_wr24_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr25 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr25 register
     */
    void increment_wr25(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr25_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr25_u register
     */
    void increment_wr25_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr26 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr26 register
     */
    void increment_wr26(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr26_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr26_u register
     */
    void increment_wr26_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr27 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr27 register
     */
    void increment_wr27(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr27_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr27_u register
     */
    void increment_wr27_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr28 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr28 register
     */
    void increment_wr28(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr28_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr28_u register
     */
    void increment_wr28_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr29 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr29 register
     */
    void increment_wr29(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr29_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr29_u register
     */
    void increment_wr29_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr30 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr30 register
     */
    void increment_wr30(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr30_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr30_u register
     */
    void increment_wr30_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr31 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr31 register
     */
    void increment_wr31(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 wr31_u register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 wr31_u register
     */
    void increment_wr31_u(unsigned long long value);
    /**
     * \brief Increments the value of the rh850g4 EIPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPC register
     */
    void increment_EIPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EIPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPSW register
     */
    void increment_EIPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPC register
     */
    void increment_FEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPSW register
     */
    void increment_FEPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PSW_EIMASK register field
     */
    void increment_PSW_EIMASK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_CC register field
     */
    void increment_FPSR_CC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_RM register field
     */
    void increment_FPSR_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XC register field
     */
    void increment_FPSR_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XE register field
     */
    void increment_FPSR_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPSR_XP register field
     */
    void increment_FPSR_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPEPC register
     */
    void increment_FPEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPST_XC register field
     */
    void increment_FPST_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPST_XP register field
     */
    void increment_FPST_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCC_CC register field
     */
    void increment_FPCC_CC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCFG_RM register field
     */
    void increment_FPCFG_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FPCFG_XE register field
     */
    void increment_FPCFG_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 EIIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIIC register
     */
    void increment_EIIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEIC register
     */
    void increment_FEIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PSWH_GPID register field
     */
    void increment_PSWH_GPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 CTPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTPC register
     */
    void increment_CTPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 CTPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTPSW register
     */
    void increment_CTPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIPSWH_GPID register field
     */
    void increment_EIPSWH_GPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEPSWH_GPID register field
     */
    void increment_FEPSWH_GPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 CTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 CTBP register
     */
    void increment_CTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SNZCFG register
     */
    void increment_SNZCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EIWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EIWR register
     */
    void increment_EIWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 FEWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FEWR register
     */
    void increment_FEWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SPID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SPID register
     */
    void increment_SPID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SPIDLIST register
     */
    void increment_SPIDLIST(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBASE_RBASE register field
     */
    void increment_RBASE_RBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 EBASE_EBASE register field
     */
    void increment_EBASE_EBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 INTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 INTBP register
     */
    void increment_INTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PID register
     */
    void increment_PID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SVLOCK register
     */
    void increment_SVLOCK(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SCCFG_SIZE register field
     */
    void increment_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 SCBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 SCBP register
     */
    void increment_SCBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 HVSB register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 HVSB register
     */
    void increment_HVSB(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PEID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PEID register
     */
    void increment_PEID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BMID_BMID register field
     */
    void increment_BMID_BMID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MEA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MEA register
     */
    void increment_MEA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MEI register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MEI register
     */
    void increment_MEI(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBCR0_BE register field
     */
    void increment_RBCR0_BE(unsigned short value);
    /**
     * \brief Increments the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBCR1_NC register field
     */
    void increment_RBCR1_NC(unsigned short value);
    /**
     * \brief Increments the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBNR_BN register field
     */
    void increment_RBNR_BN(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RBIP_RBIP register field
     */
    void increment_RBIP_RBIP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ISPR_ISP register field
     */
    void increment_ISPR_ISP(unsigned short value);
    /**
     * \brief Increments the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 INTCFG_ULNR register field
     */
    void increment_INTCFG_ULNR(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PLMR_PLM register field
     */
    void increment_PLMR_PLM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_RM register field
     */
    void increment_FXSR_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XC register field
     */
    void increment_FXSR_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XE register field
     */
    void increment_FXSR_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXSR_XP register field
     */
    void increment_FXSR_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXST_XC register field
     */
    void increment_FXST_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXST_XP register field
     */
    void increment_FXST_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXCFG_RM register field
     */
    void increment_FXCFG_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXCFG_XE register field
     */
    void increment_FXCFG_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC3 register field
     */
    void increment_FXXC_XC3(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC2 register field
     */
    void increment_FXXC_XC2(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC1 register field
     */
    void increment_FXXC_XC1(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXC_XC0 register field
     */
    void increment_FXXC_XC0(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP3 register field
     */
    void increment_FXXP_XP3(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP2 register field
     */
    void increment_FXXP_XP2(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP1 register field
     */
    void increment_FXXP_XP1(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 FXXP_XP0 register field
     */
    void increment_FXXP_XP0(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIPC register
     */
    void increment_GMEIPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIPSW register
     */
    void increment_GMEIPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEPC register
     */
    void increment_GMFEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEPSW register
     */
    void increment_GMFEPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPSW_EIMASK register field
     */
    void increment_GMPSW_EIMASK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMMEA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMEA register
     */
    void increment_GMMEA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMMEI register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMEI register
     */
    void increment_GMMEI(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIIC register
     */
    void increment_GMEIIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEIC register
     */
    void increment_GMFEIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMSPID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSPID register
     */
    void increment_GMSPID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSPIDLIST register
     */
    void increment_GMSPIDLIST(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEBASE_EBASE register field
     */
    void increment_GMEBASE_EBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMINTBP register
     */
    void increment_GMINTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMINTCFG_ULNR register field
     */
    void increment_GMINTCFG_ULNR(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPLMR_PLM register field
     */
    void increment_GMPLMR_PLM(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMSVLOCK register
     */
    void increment_GMSVLOCK(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMMPM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMMPM register
     */
    void increment_GMMPM(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMEIWR register
     */
    void increment_GMEIWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMFEWR register
     */
    void increment_GMFEWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 GMPEID register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 GMPEID register
     */
    void increment_GMPEID(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPM register
     */
    void increment_MPM(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_NMPUE register field
     */
    void increment_MPCFG_NMPUE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_HBE register field
     */
    void increment_MPCFG_HBE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_ARCH register field
     */
    void increment_MPCFG_ARCH(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPCFG_NBK register field
     */
    void increment_MPCFG_NBK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MCA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCA register
     */
    void increment_MCA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MCS register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCS register
     */
    void increment_MCS(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MCC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCC register
     */
    void increment_MCC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MCI_SPID register field
     */
    void increment_MCI_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPIDX_IDX register field
     */
    void increment_MPIDX_IDX(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPBK_BK register field
     */
    void increment_MPBK_BK(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPLA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPLA register
     */
    void increment_MPLA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPUA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPUA register
     */
    void increment_MPUA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID0_SPID register field
     */
    void increment_MPID0_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID1_SPID register field
     */
    void increment_MPID1_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID2_SPID register field
     */
    void increment_MPID2_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID3_SPID register field
     */
    void increment_MPID3_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID4_SPID register field
     */
    void increment_MPID4_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID5_SPID register field
     */
    void increment_MPID5_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID6_SPID register field
     */
    void increment_MPID6_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 MPID7_SPID register field
     */
    void increment_MPID7_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGL_LPN register field
     */
    void increment_ICTAGL_LPN(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGL_LRU register field
     */
    void increment_ICTAGL_LRU(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGH_DATAECC register field
     */
    void increment_ICTAGH_DATAECC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICTAGH_TAGECC register field
     */
    void increment_ICTAGH_TAGECC(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICDATL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICDATL register
     */
    void increment_ICDATL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICDATH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICDATH register
     */
    void increment_ICDATH(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    void increment_ICCFG_ICHSIZE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHLINE register field
     */
    void increment_ICCFG_ICHLINE(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICCFG_ICHWAY register field
     */
    void increment_ICCFG_ICHWAY(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICERR_ICHEWY register field
     */
    void increment_ICERR_ICHEWY(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICERR_ICHEIX register field
     */
    void increment_ICERR_ICHEIX(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCOUNTL register
     */
    void increment_TSCOUNTL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCOUNTH register
     */
    void increment_TSCOUNTH(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 TSCTRL register
     */
    void increment_TSCTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMUMCTRL register
     */
    void increment_PMUMCTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMGMCTRL register
     */
    void increment_PMGMCTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT0 register
     */
    void increment_PMCOUNT0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL0 register
     */
    void increment_PMCTRL0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT1 register
     */
    void increment_PMCOUNT1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL1 register
     */
    void increment_PMCTRL1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT2 register
     */
    void increment_PMCOUNT2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL2 register
     */
    void increment_PMCTRL2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT3 register
     */
    void increment_PMCOUNT3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL3 register
     */
    void increment_PMCTRL3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL4 register
     */
    void increment_PMCTRL4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL5 register
     */
    void increment_PMCTRL5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL6 register
     */
    void increment_PMCTRL6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCTRL7 register
     */
    void increment_PMCTRL7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT4 register
     */
    void increment_PMCOUNT4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT5 register
     */
    void increment_PMCOUNT5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT6 register
     */
    void increment_PMCOUNT6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMCOUNT7 register
     */
    void increment_PMCOUNT7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND0 register
     */
    void increment_PMSUBCND0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND1 register
     */
    void increment_PMSUBCND1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND2 register
     */
    void increment_PMSUBCND2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND3 register
     */
    void increment_PMSUBCND3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND4 register
     */
    void increment_PMSUBCND4(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND5 register
     */
    void increment_PMSUBCND5(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND6 register
     */
    void increment_PMSUBCND6(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 PMSUBCND7 register
     */
    void increment_PMSUBCND7(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBGEN register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBGEN register
     */
    void increment_DBGEN(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPSWH register
     */
    void increment_DBPSWH(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBIC register
     */
    void increment_DBIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPC register
     */
    void increment_DBPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBPSW register
     */
    void increment_DBPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DIR0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DIR0 register
     */
    void increment_DIR0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DIR1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DIR1 register
     */
    void increment_DIR1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPC register
     */
    void increment_BPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BPAV register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPAV register
     */
    void increment_BPAV(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BPAM register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BPAM register
     */
    void increment_BPAM(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBCFG register
     */
    void increment_DBCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DBWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DBWR register
     */
    void increment_DBWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSTEST0 register
     */
    void increment_LSTEST0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSTEST1 register
     */
    void increment_LSTEST1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSCFG register
     */
    void increment_LSCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 ICBKEY register
     */
    void increment_ICBKEY(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 IFCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 IFCR register
     */
    void increment_IFCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 IFCR1 register
     */
    void increment_IFCR1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCTRL0 register
     */
    void increment_BRPCTRL0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCTRL1 register
     */
    void increment_BRPCTRL1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPCFG register
     */
    void increment_BRPCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPACTRL register
     */
    void increment_BRPACTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 BRPDATA register
     */
    void increment_BRPDATA(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DCBKEY register
     */
    void increment_DCBKEY(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSUCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSUCR register
     */
    void increment_LSUCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSULNK0 register
     */
    void increment_LSULNK0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 LSULNK1 register
     */
    void increment_LSULNK1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLCR register
     */
    void increment_L1RLCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLNK0 register
     */
    void increment_L1RLNK0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RLNK1 register
     */
    void increment_L1RLNK1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 L1RCFG register
     */
    void increment_L1RCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DECFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DECFG register
     */
    void increment_DECFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DECTRL register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DECTRL register
     */
    void increment_DECTRL(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 DEVDS register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 DEVDS register
     */
    void increment_DEVDS(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBCR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBCR register
     */
    void increment_RDBCR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBACR register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBACR register
     */
    void increment_RDBACR(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBATAG register
     */
    void increment_RDBATAG(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT0 register
     */
    void increment_RDBADAT0(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT1 register
     */
    void increment_RDBADAT1(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT2 register
     */
    void increment_RDBADAT2(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBADAT3 register
     */
    void increment_RDBADAT3(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 RDBSTAT register
     */
    void increment_RDBSTAT(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 hv_func_support register
     */
    void increment_hv_func_support(unsigned char value);
    /**
     * \brief Increments the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 swd0pcad_exec_count register
     */
    void increment_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 swd0pcad_val register
     */
    void increment_swd0pcad_val(unsigned int value);
    /**
     * \brief Increments the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Amount by which to increment the value of the rh850g4 mem_acc_type register
     */
    void increment_mem_acc_type(unsigned int value);

    // Immediate register group increment operations
    /**
     * \brief Increments the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Amount by which to increment the value of the specified rh850g4 GR group register
     */
    void increment_GR(rh850g4::GR_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Amount by which to increment the value of the specified rh850g4 WR group register
     */
    void increment_WR(rh850g4::WR_index_t index, unsigned long long value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_0 group register
     */
    void increment_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_1 group register
     */
    void increment_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_2 group register
     */
    void increment_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_3 group register
     */
    void increment_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_4 group register
     */
    void increment_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_5 group register
     */
    void increment_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_9 group register
     */
    void increment_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_10 group register
     */
    void increment_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_11 group register
     */
    void increment_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_12 group register
     */
    void increment_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_13 group register
     */
    void increment_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_14 group register
     */
    void increment_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Amount by which to increment the value of the specified rh850g4 SR_SELID_15 group register
     */
    void increment_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value);

    // Immediate register and register field decrement operations
    /**
     * \brief Decrements the value of the rh850g4 translation time PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 translation time PC register
     */
    void decrement_trans_PC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r0 register
     */
    void decrement_r0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r1 register
     */
    void decrement_r1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r2 register
     */
    void decrement_r2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r3 register
     */
    void decrement_r3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r4 register
     */
    void decrement_r4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r5 register
     */
    void decrement_r5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r6 register
     */
    void decrement_r6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r7 register
     */
    void decrement_r7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r8 register
     */
    void decrement_r8(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r9 register
     */
    void decrement_r9(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r10 register
     */
    void decrement_r10(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r11 register
     */
    void decrement_r11(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r12 register
     */
    void decrement_r12(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r13 register
     */
    void decrement_r13(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r14 register
     */
    void decrement_r14(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r15 register
     */
    void decrement_r15(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r16 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r16 register
     */
    void decrement_r16(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r17 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r17 register
     */
    void decrement_r17(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r18 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r18 register
     */
    void decrement_r18(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r19 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r19 register
     */
    void decrement_r19(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r20 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r20 register
     */
    void decrement_r20(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r21 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r21 register
     */
    void decrement_r21(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r22 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r22 register
     */
    void decrement_r22(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r23 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r23 register
     */
    void decrement_r23(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r24 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r24 register
     */
    void decrement_r24(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r25 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r25 register
     */
    void decrement_r25(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r26 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r26 register
     */
    void decrement_r26(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r27 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r27 register
     */
    void decrement_r27(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r28 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r28 register
     */
    void decrement_r28(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r29 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r29 register
     */
    void decrement_r29(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r30 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r30 register
     */
    void decrement_r30(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 r31 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 r31 register
     */
    void decrement_r31(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PC register
     */
    void decrement_PC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 wr0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr0 register
     */
    void decrement_wr0(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr0_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr0_u register
     */
    void decrement_wr0_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr1 register
     */
    void decrement_wr1(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr1_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr1_u register
     */
    void decrement_wr1_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr2 register
     */
    void decrement_wr2(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr2_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr2_u register
     */
    void decrement_wr2_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr3 register
     */
    void decrement_wr3(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr3_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr3_u register
     */
    void decrement_wr3_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr4 register
     */
    void decrement_wr4(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr4_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr4_u register
     */
    void decrement_wr4_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr5 register
     */
    void decrement_wr5(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr5_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr5_u register
     */
    void decrement_wr5_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr6 register
     */
    void decrement_wr6(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr6_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr6_u register
     */
    void decrement_wr6_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr7 register
     */
    void decrement_wr7(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr7_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr7_u register
     */
    void decrement_wr7_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr8 register
     */
    void decrement_wr8(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr8_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr8_u register
     */
    void decrement_wr8_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr9 register
     */
    void decrement_wr9(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr9_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr9_u register
     */
    void decrement_wr9_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr10 register
     */
    void decrement_wr10(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr10_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr10_u register
     */
    void decrement_wr10_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr11 register
     */
    void decrement_wr11(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr11_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr11_u register
     */
    void decrement_wr11_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr12 register
     */
    void decrement_wr12(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr12_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr12_u register
     */
    void decrement_wr12_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr13 register
     */
    void decrement_wr13(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr13_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr13_u register
     */
    void decrement_wr13_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr14 register
     */
    void decrement_wr14(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr14_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr14_u register
     */
    void decrement_wr14_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr15 register
     */
    void decrement_wr15(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr15_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr15_u register
     */
    void decrement_wr15_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr16 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr16 register
     */
    void decrement_wr16(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr16_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr16_u register
     */
    void decrement_wr16_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr17 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr17 register
     */
    void decrement_wr17(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr17_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr17_u register
     */
    void decrement_wr17_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr18 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr18 register
     */
    void decrement_wr18(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr18_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr18_u register
     */
    void decrement_wr18_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr19 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr19 register
     */
    void decrement_wr19(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr19_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr19_u register
     */
    void decrement_wr19_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr20 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr20 register
     */
    void decrement_wr20(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr20_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr20_u register
     */
    void decrement_wr20_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr21 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr21 register
     */
    void decrement_wr21(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr21_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr21_u register
     */
    void decrement_wr21_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr22 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr22 register
     */
    void decrement_wr22(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr22_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr22_u register
     */
    void decrement_wr22_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr23 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr23 register
     */
    void decrement_wr23(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr23_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr23_u register
     */
    void decrement_wr23_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr24 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr24 register
     */
    void decrement_wr24(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr24_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr24_u register
     */
    void decrement_wr24_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr25 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr25 register
     */
    void decrement_wr25(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr25_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr25_u register
     */
    void decrement_wr25_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr26 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr26 register
     */
    void decrement_wr26(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr26_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr26_u register
     */
    void decrement_wr26_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr27 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr27 register
     */
    void decrement_wr27(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr27_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr27_u register
     */
    void decrement_wr27_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr28 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr28 register
     */
    void decrement_wr28(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr28_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr28_u register
     */
    void decrement_wr28_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr29 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr29 register
     */
    void decrement_wr29(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr29_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr29_u register
     */
    void decrement_wr29_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr30 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr30 register
     */
    void decrement_wr30(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr30_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr30_u register
     */
    void decrement_wr30_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr31 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr31 register
     */
    void decrement_wr31(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 wr31_u register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 wr31_u register
     */
    void decrement_wr31_u(unsigned long long value);
    /**
     * \brief Decrements the value of the rh850g4 EIPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPC register
     */
    void decrement_EIPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EIPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPSW register
     */
    void decrement_EIPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPC register
     */
    void decrement_FEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPSW register
     */
    void decrement_FEPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PSW_EIMASK register field
     */
    void decrement_PSW_EIMASK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_CC register field
     */
    void decrement_FPSR_CC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_RM register field
     */
    void decrement_FPSR_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XC register field
     */
    void decrement_FPSR_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XE register field
     */
    void decrement_FPSR_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPSR_XP register field
     */
    void decrement_FPSR_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPEPC register
     */
    void decrement_FPEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPST_XC register field
     */
    void decrement_FPST_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPST_XP register field
     */
    void decrement_FPST_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCC_CC register field
     */
    void decrement_FPCC_CC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCFG_RM register field
     */
    void decrement_FPCFG_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FPCFG_XE register field
     */
    void decrement_FPCFG_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 EIIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIIC register
     */
    void decrement_EIIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEIC register
     */
    void decrement_FEIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PSWH_GPID register field
     */
    void decrement_PSWH_GPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 CTPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTPC register
     */
    void decrement_CTPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 CTPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTPSW register
     */
    void decrement_CTPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIPSWH_GPID register field
     */
    void decrement_EIPSWH_GPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEPSWH_GPID register field
     */
    void decrement_FEPSWH_GPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 CTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 CTBP register
     */
    void decrement_CTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SNZCFG register
     */
    void decrement_SNZCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EIWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EIWR register
     */
    void decrement_EIWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 FEWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FEWR register
     */
    void decrement_FEWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SPID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SPID register
     */
    void decrement_SPID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SPIDLIST register
     */
    void decrement_SPIDLIST(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBASE_RBASE register field
     */
    void decrement_RBASE_RBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 EBASE_EBASE register field
     */
    void decrement_EBASE_EBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 INTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 INTBP register
     */
    void decrement_INTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PID register
     */
    void decrement_PID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SVLOCK register
     */
    void decrement_SVLOCK(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SCCFG_SIZE register field
     */
    void decrement_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 SCBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 SCBP register
     */
    void decrement_SCBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 HVSB register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 HVSB register
     */
    void decrement_HVSB(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PEID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PEID register
     */
    void decrement_PEID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BMID_BMID register field
     */
    void decrement_BMID_BMID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MEA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MEA register
     */
    void decrement_MEA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MEI register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MEI register
     */
    void decrement_MEI(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBCR0_BE register field
     */
    void decrement_RBCR0_BE(unsigned short value);
    /**
     * \brief Decrements the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBCR1_NC register field
     */
    void decrement_RBCR1_NC(unsigned short value);
    /**
     * \brief Decrements the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBNR_BN register field
     */
    void decrement_RBNR_BN(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RBIP_RBIP register field
     */
    void decrement_RBIP_RBIP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ISPR_ISP register field
     */
    void decrement_ISPR_ISP(unsigned short value);
    /**
     * \brief Decrements the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 INTCFG_ULNR register field
     */
    void decrement_INTCFG_ULNR(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PLMR_PLM register field
     */
    void decrement_PLMR_PLM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_RM register field
     */
    void decrement_FXSR_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XC register field
     */
    void decrement_FXSR_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XE register field
     */
    void decrement_FXSR_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXSR_XP register field
     */
    void decrement_FXSR_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXST_XC register field
     */
    void decrement_FXST_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXST_XP register field
     */
    void decrement_FXST_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXCFG_RM register field
     */
    void decrement_FXCFG_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXCFG_XE register field
     */
    void decrement_FXCFG_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC3 register field
     */
    void decrement_FXXC_XC3(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC2 register field
     */
    void decrement_FXXC_XC2(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC1 register field
     */
    void decrement_FXXC_XC1(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXC_XC0 register field
     */
    void decrement_FXXC_XC0(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP3 register field
     */
    void decrement_FXXP_XP3(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP2 register field
     */
    void decrement_FXXP_XP2(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP1 register field
     */
    void decrement_FXXP_XP1(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 FXXP_XP0 register field
     */
    void decrement_FXXP_XP0(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIPC register
     */
    void decrement_GMEIPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIPSW register
     */
    void decrement_GMEIPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEPC register
     */
    void decrement_GMFEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEPSW register
     */
    void decrement_GMFEPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPSW_EIMASK register field
     */
    void decrement_GMPSW_EIMASK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMMEA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMEA register
     */
    void decrement_GMMEA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMMEI register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMEI register
     */
    void decrement_GMMEI(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIIC register
     */
    void decrement_GMEIIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEIC register
     */
    void decrement_GMFEIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMSPID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSPID register
     */
    void decrement_GMSPID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSPIDLIST register
     */
    void decrement_GMSPIDLIST(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEBASE_EBASE register field
     */
    void decrement_GMEBASE_EBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMINTBP register
     */
    void decrement_GMINTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMINTCFG_ULNR register field
     */
    void decrement_GMINTCFG_ULNR(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPLMR_PLM register field
     */
    void decrement_GMPLMR_PLM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMSVLOCK register
     */
    void decrement_GMSVLOCK(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMMPM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMMPM register
     */
    void decrement_GMMPM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMEIWR register
     */
    void decrement_GMEIWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMFEWR register
     */
    void decrement_GMFEWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 GMPEID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 GMPEID register
     */
    void decrement_GMPEID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPM register
     */
    void decrement_MPM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_NMPUE register field
     */
    void decrement_MPCFG_NMPUE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_HBE register field
     */
    void decrement_MPCFG_HBE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_ARCH register field
     */
    void decrement_MPCFG_ARCH(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPCFG_NBK register field
     */
    void decrement_MPCFG_NBK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MCA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCA register
     */
    void decrement_MCA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MCS register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCS register
     */
    void decrement_MCS(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MCC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCC register
     */
    void decrement_MCC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MCI_SPID register field
     */
    void decrement_MCI_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPIDX_IDX register field
     */
    void decrement_MPIDX_IDX(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPBK_BK register field
     */
    void decrement_MPBK_BK(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPLA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPLA register
     */
    void decrement_MPLA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPUA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPUA register
     */
    void decrement_MPUA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID0_SPID register field
     */
    void decrement_MPID0_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID1_SPID register field
     */
    void decrement_MPID1_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID2_SPID register field
     */
    void decrement_MPID2_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID3_SPID register field
     */
    void decrement_MPID3_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID4_SPID register field
     */
    void decrement_MPID4_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID5_SPID register field
     */
    void decrement_MPID5_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID6_SPID register field
     */
    void decrement_MPID6_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 MPID7_SPID register field
     */
    void decrement_MPID7_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGL_LPN register field
     */
    void decrement_ICTAGL_LPN(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGL_LRU register field
     */
    void decrement_ICTAGL_LRU(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGH_DATAECC register field
     */
    void decrement_ICTAGH_DATAECC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICTAGH_TAGECC register field
     */
    void decrement_ICTAGH_TAGECC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICDATL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICDATL register
     */
    void decrement_ICDATL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICDATH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICDATH register
     */
    void decrement_ICDATH(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    void decrement_ICCFG_ICHSIZE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHLINE register field
     */
    void decrement_ICCFG_ICHLINE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICCFG_ICHWAY register field
     */
    void decrement_ICCFG_ICHWAY(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICERR_ICHEWY register field
     */
    void decrement_ICERR_ICHEWY(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICERR_ICHEIX register field
     */
    void decrement_ICERR_ICHEIX(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCOUNTL register
     */
    void decrement_TSCOUNTL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCOUNTH register
     */
    void decrement_TSCOUNTH(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 TSCTRL register
     */
    void decrement_TSCTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMUMCTRL register
     */
    void decrement_PMUMCTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMGMCTRL register
     */
    void decrement_PMGMCTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT0 register
     */
    void decrement_PMCOUNT0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL0 register
     */
    void decrement_PMCTRL0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT1 register
     */
    void decrement_PMCOUNT1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL1 register
     */
    void decrement_PMCTRL1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT2 register
     */
    void decrement_PMCOUNT2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL2 register
     */
    void decrement_PMCTRL2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT3 register
     */
    void decrement_PMCOUNT3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL3 register
     */
    void decrement_PMCTRL3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL4 register
     */
    void decrement_PMCTRL4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL5 register
     */
    void decrement_PMCTRL5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL6 register
     */
    void decrement_PMCTRL6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCTRL7 register
     */
    void decrement_PMCTRL7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT4 register
     */
    void decrement_PMCOUNT4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT5 register
     */
    void decrement_PMCOUNT5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT6 register
     */
    void decrement_PMCOUNT6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMCOUNT7 register
     */
    void decrement_PMCOUNT7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND0 register
     */
    void decrement_PMSUBCND0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND1 register
     */
    void decrement_PMSUBCND1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND2 register
     */
    void decrement_PMSUBCND2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND3 register
     */
    void decrement_PMSUBCND3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND4 register
     */
    void decrement_PMSUBCND4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND5 register
     */
    void decrement_PMSUBCND5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND6 register
     */
    void decrement_PMSUBCND6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 PMSUBCND7 register
     */
    void decrement_PMSUBCND7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBGEN register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBGEN register
     */
    void decrement_DBGEN(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPSWH register
     */
    void decrement_DBPSWH(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBIC register
     */
    void decrement_DBIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPC register
     */
    void decrement_DBPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBPSW register
     */
    void decrement_DBPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DIR0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DIR0 register
     */
    void decrement_DIR0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DIR1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DIR1 register
     */
    void decrement_DIR1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPC register
     */
    void decrement_BPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BPAV register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPAV register
     */
    void decrement_BPAV(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BPAM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BPAM register
     */
    void decrement_BPAM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBCFG register
     */
    void decrement_DBCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DBWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DBWR register
     */
    void decrement_DBWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSTEST0 register
     */
    void decrement_LSTEST0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSTEST1 register
     */
    void decrement_LSTEST1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSCFG register
     */
    void decrement_LSCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 ICBKEY register
     */
    void decrement_ICBKEY(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 IFCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 IFCR register
     */
    void decrement_IFCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 IFCR1 register
     */
    void decrement_IFCR1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCTRL0 register
     */
    void decrement_BRPCTRL0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCTRL1 register
     */
    void decrement_BRPCTRL1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPCFG register
     */
    void decrement_BRPCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPACTRL register
     */
    void decrement_BRPACTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 BRPDATA register
     */
    void decrement_BRPDATA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DCBKEY register
     */
    void decrement_DCBKEY(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSUCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSUCR register
     */
    void decrement_LSUCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSULNK0 register
     */
    void decrement_LSULNK0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 LSULNK1 register
     */
    void decrement_LSULNK1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLCR register
     */
    void decrement_L1RLCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLNK0 register
     */
    void decrement_L1RLNK0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RLNK1 register
     */
    void decrement_L1RLNK1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 L1RCFG register
     */
    void decrement_L1RCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DECFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DECFG register
     */
    void decrement_DECFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DECTRL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DECTRL register
     */
    void decrement_DECTRL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 DEVDS register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 DEVDS register
     */
    void decrement_DEVDS(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBCR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBCR register
     */
    void decrement_RDBCR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBACR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBACR register
     */
    void decrement_RDBACR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBATAG register
     */
    void decrement_RDBATAG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT0 register
     */
    void decrement_RDBADAT0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT1 register
     */
    void decrement_RDBADAT1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT2 register
     */
    void decrement_RDBADAT2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBADAT3 register
     */
    void decrement_RDBADAT3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 RDBSTAT register
     */
    void decrement_RDBSTAT(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 hv_func_support register
     */
    void decrement_hv_func_support(unsigned char value);
    /**
     * \brief Decrements the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 swd0pcad_exec_count register
     */
    void decrement_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 swd0pcad_val register
     */
    void decrement_swd0pcad_val(unsigned int value);
    /**
     * \brief Decrements the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Amount by which to decrement the value of the rh850g4 mem_acc_type register
     */
    void decrement_mem_acc_type(unsigned int value);

    // Immediate register group decrement operations
    /**
     * \brief Decrements the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Amount by which to decrement the value of the specified rh850g4 GR group register
     */
    void decrement_GR(rh850g4::GR_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Amount by which to decrement the value of the specified rh850g4 WR group register
     */
    void decrement_WR(rh850g4::WR_index_t index, unsigned long long value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_0 group register
     */
    void decrement_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_1 group register
     */
    void decrement_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_2 group register
     */
    void decrement_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_3 group register
     */
    void decrement_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_4 group register
     */
    void decrement_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_5 group register
     */
    void decrement_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_9 group register
     */
    void decrement_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_10 group register
     */
    void decrement_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_11 group register
     */
    void decrement_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_12 group register
     */
    void decrement_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_13 group register
     */
    void decrement_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_14 group register
     */
    void decrement_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Amount by which to decrement the value of the specified rh850g4 SR_SELID_15 group register
     */
    void decrement_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, unsigned int value);

  protected:  
    // Generative instruction count operations
    /**
     * \brief Generates code to increment the number of instructions executed in the current quanta by one
     */
    void gen_increment_instruction_count(void);

    // Generative time operations
    /**
     * \brief Generates code to query the time executed in the current quanta
     * \return Object representing the time executed in the current quanta
     */
    fastiss::i64_t* gen_read_time(void);

    /**
     * \brief Generates code to set the time executed in the current quanta
     * \param value Object representing the new time executed in the current quanta
     */
    void gen_write_time(fastiss::i64_t* value);
     /**
     * \brief Generates code to increment the time executed in the current quanta by the specified amount
     * \param increment Object representing the amount by which to increment the time executed in the current quanta
     */
    void gen_increment_time(fastiss::i64_t *increment);

    /**
     * \brief Generates code to decrement the time executed in the current quanta by the specified amount
     * \param decrement Object representing the amount by which to decrement the time executed in the current quanta
     */
    void gen_decrement_time(fastiss::i64_t *decrement);

    // Generative cycle period operations
    /**
     * \brief Generates code to query the period of time taken to execute a cycle
     * \return Object representing the period of time taken to execute a cycle
     */
    fastiss::i64_t* gen_read_cycle_period(void);

    /**
     * \brief Generates code to set the period of time taken to execute a cycle
     * \param value Object representing the new period of time taken to execute a cycle
     */
    void gen_write_cycle_period(fastiss::i64_t* value);
     /**
     * \brief Generates code to increment the period of time taken to execute a cycle
     * \param increment Object representing the amount by which to increment the period of time taken to execute a cycle
     */
    void gen_increment_cycle_period(fastiss::i64_t *increment);

    /**
     * \brief Generates code to decrement the time executed in the period of time taken to execute a cycle
     * \param decrement Object representing the amount by which to decrement the period of time taken to execute a cycle
     */
    void gen_decrement_cycle_period(fastiss::i64_t *decrement);

    /**
     * \brief Generates code to query the value of the pointer to the core instance
     * \return Object representing the value of the pointer to the core instance
     */
    fastiss::i8_ptr_t* gen_read_core_ptr(void);

    /**
     * \brief Generates code to query the value of the watchpoint hit flag
     * \return Object representing the value of the watchpoint hit flag
     */
    fastiss::i8_t* gen_read_watchpoint_hit(void);
    /**
     * \brief Generates code to set the value of the watchpoint hit flag
     * \param value Object representing the value of the watchpoint hit flag
     */
    void gen_write_watchpoint_hit(fastiss::i8_t* value);
    
    /**
     * \brief Generates code to query the value of the rh850g4 r0 register
     * \return Object representing the value of the rh850g4 r0 register
     */
    fastiss::i32_t *gen_read_r0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r1 register
     * \return Object representing the value of the rh850g4 r1 register
     */
    fastiss::i32_t *gen_read_r1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r2 register
     * \return Object representing the value of the rh850g4 r2 register
     */
    fastiss::i32_t *gen_read_r2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r3 register
     * \return Object representing the value of the rh850g4 r3 register
     */
    fastiss::i32_t *gen_read_r3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r4 register
     * \return Object representing the value of the rh850g4 r4 register
     */
    fastiss::i32_t *gen_read_r4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r5 register
     * \return Object representing the value of the rh850g4 r5 register
     */
    fastiss::i32_t *gen_read_r5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r6 register
     * \return Object representing the value of the rh850g4 r6 register
     */
    fastiss::i32_t *gen_read_r6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r7 register
     * \return Object representing the value of the rh850g4 r7 register
     */
    fastiss::i32_t *gen_read_r7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r8 register
     * \return Object representing the value of the rh850g4 r8 register
     */
    fastiss::i32_t *gen_read_r8(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r9 register
     * \return Object representing the value of the rh850g4 r9 register
     */
    fastiss::i32_t *gen_read_r9(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r10 register
     * \return Object representing the value of the rh850g4 r10 register
     */
    fastiss::i32_t *gen_read_r10(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r11 register
     * \return Object representing the value of the rh850g4 r11 register
     */
    fastiss::i32_t *gen_read_r11(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r12 register
     * \return Object representing the value of the rh850g4 r12 register
     */
    fastiss::i32_t *gen_read_r12(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r13 register
     * \return Object representing the value of the rh850g4 r13 register
     */
    fastiss::i32_t *gen_read_r13(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r14 register
     * \return Object representing the value of the rh850g4 r14 register
     */
    fastiss::i32_t *gen_read_r14(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r15 register
     * \return Object representing the value of the rh850g4 r15 register
     */
    fastiss::i32_t *gen_read_r15(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r16 register
     * \return Object representing the value of the rh850g4 r16 register
     */
    fastiss::i32_t *gen_read_r16(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r17 register
     * \return Object representing the value of the rh850g4 r17 register
     */
    fastiss::i32_t *gen_read_r17(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r18 register
     * \return Object representing the value of the rh850g4 r18 register
     */
    fastiss::i32_t *gen_read_r18(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r19 register
     * \return Object representing the value of the rh850g4 r19 register
     */
    fastiss::i32_t *gen_read_r19(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r20 register
     * \return Object representing the value of the rh850g4 r20 register
     */
    fastiss::i32_t *gen_read_r20(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r21 register
     * \return Object representing the value of the rh850g4 r21 register
     */
    fastiss::i32_t *gen_read_r21(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r22 register
     * \return Object representing the value of the rh850g4 r22 register
     */
    fastiss::i32_t *gen_read_r22(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r23 register
     * \return Object representing the value of the rh850g4 r23 register
     */
    fastiss::i32_t *gen_read_r23(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r24 register
     * \return Object representing the value of the rh850g4 r24 register
     */
    fastiss::i32_t *gen_read_r24(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r25 register
     * \return Object representing the value of the rh850g4 r25 register
     */
    fastiss::i32_t *gen_read_r25(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r26 register
     * \return Object representing the value of the rh850g4 r26 register
     */
    fastiss::i32_t *gen_read_r26(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r27 register
     * \return Object representing the value of the rh850g4 r27 register
     */
    fastiss::i32_t *gen_read_r27(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r28 register
     * \return Object representing the value of the rh850g4 r28 register
     */
    fastiss::i32_t *gen_read_r28(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r29 register
     * \return Object representing the value of the rh850g4 r29 register
     */
    fastiss::i32_t *gen_read_r29(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r30 register
     * \return Object representing the value of the rh850g4 r30 register
     */
    fastiss::i32_t *gen_read_r30(void);
    /**
     * \brief Generates code to query the value of the rh850g4 r31 register
     * \return Object representing the value of the rh850g4 r31 register
     */
    fastiss::i32_t *gen_read_r31(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PC register
     * \return Object representing the value of the rh850g4 PC register
     */
    fastiss::i32_t *gen_read_PC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr0 register
     * \return Object representing the value of the rh850g4 wr0 register
     */
    fastiss::i64_t *gen_read_wr0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr0_u register
     * \return Object representing the value of the rh850g4 wr0_u register
     */
    fastiss::i64_t *gen_read_wr0_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr1 register
     * \return Object representing the value of the rh850g4 wr1 register
     */
    fastiss::i64_t *gen_read_wr1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr1_u register
     * \return Object representing the value of the rh850g4 wr1_u register
     */
    fastiss::i64_t *gen_read_wr1_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr2 register
     * \return Object representing the value of the rh850g4 wr2 register
     */
    fastiss::i64_t *gen_read_wr2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr2_u register
     * \return Object representing the value of the rh850g4 wr2_u register
     */
    fastiss::i64_t *gen_read_wr2_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr3 register
     * \return Object representing the value of the rh850g4 wr3 register
     */
    fastiss::i64_t *gen_read_wr3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr3_u register
     * \return Object representing the value of the rh850g4 wr3_u register
     */
    fastiss::i64_t *gen_read_wr3_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr4 register
     * \return Object representing the value of the rh850g4 wr4 register
     */
    fastiss::i64_t *gen_read_wr4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr4_u register
     * \return Object representing the value of the rh850g4 wr4_u register
     */
    fastiss::i64_t *gen_read_wr4_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr5 register
     * \return Object representing the value of the rh850g4 wr5 register
     */
    fastiss::i64_t *gen_read_wr5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr5_u register
     * \return Object representing the value of the rh850g4 wr5_u register
     */
    fastiss::i64_t *gen_read_wr5_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr6 register
     * \return Object representing the value of the rh850g4 wr6 register
     */
    fastiss::i64_t *gen_read_wr6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr6_u register
     * \return Object representing the value of the rh850g4 wr6_u register
     */
    fastiss::i64_t *gen_read_wr6_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr7 register
     * \return Object representing the value of the rh850g4 wr7 register
     */
    fastiss::i64_t *gen_read_wr7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr7_u register
     * \return Object representing the value of the rh850g4 wr7_u register
     */
    fastiss::i64_t *gen_read_wr7_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr8 register
     * \return Object representing the value of the rh850g4 wr8 register
     */
    fastiss::i64_t *gen_read_wr8(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr8_u register
     * \return Object representing the value of the rh850g4 wr8_u register
     */
    fastiss::i64_t *gen_read_wr8_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr9 register
     * \return Object representing the value of the rh850g4 wr9 register
     */
    fastiss::i64_t *gen_read_wr9(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr9_u register
     * \return Object representing the value of the rh850g4 wr9_u register
     */
    fastiss::i64_t *gen_read_wr9_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr10 register
     * \return Object representing the value of the rh850g4 wr10 register
     */
    fastiss::i64_t *gen_read_wr10(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr10_u register
     * \return Object representing the value of the rh850g4 wr10_u register
     */
    fastiss::i64_t *gen_read_wr10_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr11 register
     * \return Object representing the value of the rh850g4 wr11 register
     */
    fastiss::i64_t *gen_read_wr11(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr11_u register
     * \return Object representing the value of the rh850g4 wr11_u register
     */
    fastiss::i64_t *gen_read_wr11_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr12 register
     * \return Object representing the value of the rh850g4 wr12 register
     */
    fastiss::i64_t *gen_read_wr12(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr12_u register
     * \return Object representing the value of the rh850g4 wr12_u register
     */
    fastiss::i64_t *gen_read_wr12_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr13 register
     * \return Object representing the value of the rh850g4 wr13 register
     */
    fastiss::i64_t *gen_read_wr13(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr13_u register
     * \return Object representing the value of the rh850g4 wr13_u register
     */
    fastiss::i64_t *gen_read_wr13_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr14 register
     * \return Object representing the value of the rh850g4 wr14 register
     */
    fastiss::i64_t *gen_read_wr14(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr14_u register
     * \return Object representing the value of the rh850g4 wr14_u register
     */
    fastiss::i64_t *gen_read_wr14_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr15 register
     * \return Object representing the value of the rh850g4 wr15 register
     */
    fastiss::i64_t *gen_read_wr15(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr15_u register
     * \return Object representing the value of the rh850g4 wr15_u register
     */
    fastiss::i64_t *gen_read_wr15_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr16 register
     * \return Object representing the value of the rh850g4 wr16 register
     */
    fastiss::i64_t *gen_read_wr16(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr16_u register
     * \return Object representing the value of the rh850g4 wr16_u register
     */
    fastiss::i64_t *gen_read_wr16_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr17 register
     * \return Object representing the value of the rh850g4 wr17 register
     */
    fastiss::i64_t *gen_read_wr17(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr17_u register
     * \return Object representing the value of the rh850g4 wr17_u register
     */
    fastiss::i64_t *gen_read_wr17_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr18 register
     * \return Object representing the value of the rh850g4 wr18 register
     */
    fastiss::i64_t *gen_read_wr18(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr18_u register
     * \return Object representing the value of the rh850g4 wr18_u register
     */
    fastiss::i64_t *gen_read_wr18_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr19 register
     * \return Object representing the value of the rh850g4 wr19 register
     */
    fastiss::i64_t *gen_read_wr19(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr19_u register
     * \return Object representing the value of the rh850g4 wr19_u register
     */
    fastiss::i64_t *gen_read_wr19_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr20 register
     * \return Object representing the value of the rh850g4 wr20 register
     */
    fastiss::i64_t *gen_read_wr20(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr20_u register
     * \return Object representing the value of the rh850g4 wr20_u register
     */
    fastiss::i64_t *gen_read_wr20_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr21 register
     * \return Object representing the value of the rh850g4 wr21 register
     */
    fastiss::i64_t *gen_read_wr21(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr21_u register
     * \return Object representing the value of the rh850g4 wr21_u register
     */
    fastiss::i64_t *gen_read_wr21_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr22 register
     * \return Object representing the value of the rh850g4 wr22 register
     */
    fastiss::i64_t *gen_read_wr22(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr22_u register
     * \return Object representing the value of the rh850g4 wr22_u register
     */
    fastiss::i64_t *gen_read_wr22_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr23 register
     * \return Object representing the value of the rh850g4 wr23 register
     */
    fastiss::i64_t *gen_read_wr23(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr23_u register
     * \return Object representing the value of the rh850g4 wr23_u register
     */
    fastiss::i64_t *gen_read_wr23_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr24 register
     * \return Object representing the value of the rh850g4 wr24 register
     */
    fastiss::i64_t *gen_read_wr24(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr24_u register
     * \return Object representing the value of the rh850g4 wr24_u register
     */
    fastiss::i64_t *gen_read_wr24_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr25 register
     * \return Object representing the value of the rh850g4 wr25 register
     */
    fastiss::i64_t *gen_read_wr25(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr25_u register
     * \return Object representing the value of the rh850g4 wr25_u register
     */
    fastiss::i64_t *gen_read_wr25_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr26 register
     * \return Object representing the value of the rh850g4 wr26 register
     */
    fastiss::i64_t *gen_read_wr26(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr26_u register
     * \return Object representing the value of the rh850g4 wr26_u register
     */
    fastiss::i64_t *gen_read_wr26_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr27 register
     * \return Object representing the value of the rh850g4 wr27 register
     */
    fastiss::i64_t *gen_read_wr27(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr27_u register
     * \return Object representing the value of the rh850g4 wr27_u register
     */
    fastiss::i64_t *gen_read_wr27_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr28 register
     * \return Object representing the value of the rh850g4 wr28 register
     */
    fastiss::i64_t *gen_read_wr28(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr28_u register
     * \return Object representing the value of the rh850g4 wr28_u register
     */
    fastiss::i64_t *gen_read_wr28_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr29 register
     * \return Object representing the value of the rh850g4 wr29 register
     */
    fastiss::i64_t *gen_read_wr29(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr29_u register
     * \return Object representing the value of the rh850g4 wr29_u register
     */
    fastiss::i64_t *gen_read_wr29_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr30 register
     * \return Object representing the value of the rh850g4 wr30 register
     */
    fastiss::i64_t *gen_read_wr30(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr30_u register
     * \return Object representing the value of the rh850g4 wr30_u register
     */
    fastiss::i64_t *gen_read_wr30_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr31 register
     * \return Object representing the value of the rh850g4 wr31 register
     */
    fastiss::i64_t *gen_read_wr31(void);
    /**
     * \brief Generates code to query the value of the rh850g4 wr31_u register
     * \return Object representing the value of the rh850g4 wr31_u register
     */
    fastiss::i64_t *gen_read_wr31_u(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIPC register
     * \return Object representing the value of the rh850g4 EIPC register
     */
    fastiss::i32_t *gen_read_EIPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIPSW register
     * \return Object representing the value of the rh850g4 EIPSW register
     */
    fastiss::i32_t *gen_read_EIPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEPC register
     * \return Object representing the value of the rh850g4 FEPC register
     */
    fastiss::i32_t *gen_read_FEPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEPSW register
     * \return Object representing the value of the rh850g4 FEPSW register
     */
    fastiss::i32_t *gen_read_FEPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PSW register
     * \return Object representing the value of the rh850g4 PSW register
     */
    fastiss::i32_t *gen_read_PSW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_Z register field
    * \return Object representing the value of the rh850g4 PSW_Z register field
    */
    fastiss::i1_t *gen_read_PSW_Z(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_S register field
    * \return Object representing the value of the rh850g4 PSW_S register field
    */
    fastiss::i1_t *gen_read_PSW_S(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_OV register field
    * \return Object representing the value of the rh850g4 PSW_OV register field
    */
    fastiss::i1_t *gen_read_PSW_OV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CY register field
    * \return Object representing the value of the rh850g4 PSW_CY register field
    */
    fastiss::i1_t *gen_read_PSW_CY(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_SAT register field
    * \return Object representing the value of the rh850g4 PSW_SAT register field
    */
    fastiss::i1_t *gen_read_PSW_SAT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_ID register field
    * \return Object representing the value of the rh850g4 PSW_ID register field
    */
    fastiss::i1_t *gen_read_PSW_ID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EP register field
    * \return Object representing the value of the rh850g4 PSW_EP register field
    */
    fastiss::i1_t *gen_read_PSW_EP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_NP register field
    * \return Object representing the value of the rh850g4 PSW_NP register field
    */
    fastiss::i1_t *gen_read_PSW_NP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EBV register field
    * \return Object representing the value of the rh850g4 PSW_EBV register field
    */
    fastiss::i1_t *gen_read_PSW_EBV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU0 register field
    * \return Object representing the value of the rh850g4 PSW_CU0 register field
    */
    fastiss::i1_t *gen_read_PSW_CU0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU1 register field
    * \return Object representing the value of the rh850g4 PSW_CU1 register field
    */
    fastiss::i1_t *gen_read_PSW_CU1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_CU2 register field
    * \return Object representing the value of the rh850g4 PSW_CU2 register field
    */
    fastiss::i1_t *gen_read_PSW_CU2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_EIMASK register field
    * \return Object representing the value of the rh850g4 PSW_EIMASK register field
    */
    fastiss::i8_t *gen_read_PSW_EIMASK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSW_UM register field
    * \return Object representing the value of the rh850g4 PSW_UM register field
    */
    fastiss::i1_t *gen_read_PSW_UM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPSR register
     * \return Object representing the value of the rh850g4 FPSR register
     */
    fastiss::i32_t *gen_read_FPSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_CC register field
    * \return Object representing the value of the rh850g4 FPSR_CC register field
    */
    fastiss::i8_t *gen_read_FPSR_CC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_FN register field
    * \return Object representing the value of the rh850g4 FPSR_FN register field
    */
    fastiss::i1_t *gen_read_FPSR_FN(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_IF register field
    * \return Object representing the value of the rh850g4 FPSR_IF register field
    */
    fastiss::i1_t *gen_read_FPSR_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_PEM register field
    * \return Object representing the value of the rh850g4 FPSR_PEM register field
    */
    fastiss::i1_t *gen_read_FPSR_PEM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_RM register field
    * \return Object representing the value of the rh850g4 FPSR_RM register field
    */
    fastiss::i8_t *gen_read_FPSR_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_FS register field
    * \return Object representing the value of the rh850g4 FPSR_FS register field
    */
    fastiss::i1_t *gen_read_FPSR_FS(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XC register field
    * \return Object representing the value of the rh850g4 FPSR_XC register field
    */
    fastiss::i8_t *gen_read_FPSR_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XE register field
    * \return Object representing the value of the rh850g4 FPSR_XE register field
    */
    fastiss::i8_t *gen_read_FPSR_XE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPSR_XP register field
    * \return Object representing the value of the rh850g4 FPSR_XP register field
    */
    fastiss::i8_t *gen_read_FPSR_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPEPC register
     * \return Object representing the value of the rh850g4 FPEPC register
     */
    fastiss::i32_t *gen_read_FPEPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPST register
     * \return Object representing the value of the rh850g4 FPST register
     */
    fastiss::i32_t *gen_read_FPST(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_XC register field
    * \return Object representing the value of the rh850g4 FPST_XC register field
    */
    fastiss::i8_t *gen_read_FPST_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_IF register field
    * \return Object representing the value of the rh850g4 FPST_IF register field
    */
    fastiss::i1_t *gen_read_FPST_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPST_XP register field
    * \return Object representing the value of the rh850g4 FPST_XP register field
    */
    fastiss::i8_t *gen_read_FPST_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPCC register
     * \return Object representing the value of the rh850g4 FPCC register
     */
    fastiss::i32_t *gen_read_FPCC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPCC_CC register field
    * \return Object representing the value of the rh850g4 FPCC_CC register field
    */
    fastiss::i8_t *gen_read_FPCC_CC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FPCFG register
     * \return Object representing the value of the rh850g4 FPCFG register
     */
    fastiss::i32_t *gen_read_FPCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPCFG_RM register field
    * \return Object representing the value of the rh850g4 FPCFG_RM register field
    */
    fastiss::i8_t *gen_read_FPCFG_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FPCFG_XE register field
    * \return Object representing the value of the rh850g4 FPCFG_XE register field
    */
    fastiss::i8_t *gen_read_FPCFG_XE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIIC register
     * \return Object representing the value of the rh850g4 EIIC register
     */
    fastiss::i32_t *gen_read_EIIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEIC register
     * \return Object representing the value of the rh850g4 FEIC register
     */
    fastiss::i32_t *gen_read_FEIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PSWH register
     * \return Object representing the value of the rh850g4 PSWH register
     */
    fastiss::i32_t *gen_read_PSWH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSWH_GPID register field
    * \return Object representing the value of the rh850g4 PSWH_GPID register field
    */
    fastiss::i8_t *gen_read_PSWH_GPID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PSWH_GM register field
    * \return Object representing the value of the rh850g4 PSWH_GM register field
    */
    fastiss::i1_t *gen_read_PSWH_GM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 CTPC register
     * \return Object representing the value of the rh850g4 CTPC register
     */
    fastiss::i32_t *gen_read_CTPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 CTPSW register
     * \return Object representing the value of the rh850g4 CTPSW register
     */
    fastiss::i32_t *gen_read_CTPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIPSWH register
     * \return Object representing the value of the rh850g4 EIPSWH register
     */
    fastiss::i32_t *gen_read_EIPSWH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EIPSWH_GPID register field
    * \return Object representing the value of the rh850g4 EIPSWH_GPID register field
    */
    fastiss::i8_t *gen_read_EIPSWH_GPID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EIPSWH_GM register field
    * \return Object representing the value of the rh850g4 EIPSWH_GM register field
    */
    fastiss::i1_t *gen_read_EIPSWH_GM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEPSWH register
     * \return Object representing the value of the rh850g4 FEPSWH register
     */
    fastiss::i32_t *gen_read_FEPSWH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FEPSWH_GPID register field
    * \return Object representing the value of the rh850g4 FEPSWH_GPID register field
    */
    fastiss::i8_t *gen_read_FEPSWH_GPID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FEPSWH_GM register field
    * \return Object representing the value of the rh850g4 FEPSWH_GM register field
    */
    fastiss::i1_t *gen_read_FEPSWH_GM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 CTBP register
     * \return Object representing the value of the rh850g4 CTBP register
     */
    fastiss::i32_t *gen_read_CTBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SNZCFG register
     * \return Object representing the value of the rh850g4 SNZCFG register
     */
    fastiss::i32_t *gen_read_SNZCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EIWR register
     * \return Object representing the value of the rh850g4 EIWR register
     */
    fastiss::i32_t *gen_read_EIWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FEWR register
     * \return Object representing the value of the rh850g4 FEWR register
     */
    fastiss::i32_t *gen_read_FEWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SPID register
     * \return Object representing the value of the rh850g4 SPID register
     */
    fastiss::i32_t *gen_read_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SPIDLIST register
     * \return Object representing the value of the rh850g4 SPIDLIST register
     */
    fastiss::i32_t *gen_read_SPIDLIST(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBASE register
     * \return Object representing the value of the rh850g4 RBASE register
     */
    fastiss::i32_t *gen_read_RBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_RBASE register field
    * \return Object representing the value of the rh850g4 RBASE_RBASE register field
    */
    fastiss::i32_t *gen_read_RBASE_RBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_DV register field
    * \return Object representing the value of the rh850g4 RBASE_DV register field
    */
    fastiss::i1_t *gen_read_RBASE_DV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBASE_RINT register field
    * \return Object representing the value of the rh850g4 RBASE_RINT register field
    */
    fastiss::i1_t *gen_read_RBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 EBASE register
     * \return Object representing the value of the rh850g4 EBASE register
     */
    fastiss::i32_t *gen_read_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_EBASE register field
    * \return Object representing the value of the rh850g4 EBASE_EBASE register field
    */
    fastiss::i32_t *gen_read_EBASE_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_DV register field
    * \return Object representing the value of the rh850g4 EBASE_DV register field
    */
    fastiss::i1_t *gen_read_EBASE_DV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 EBASE_RINT register field
    * \return Object representing the value of the rh850g4 EBASE_RINT register field
    */
    fastiss::i1_t *gen_read_EBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 INTBP register
     * \return Object representing the value of the rh850g4 INTBP register
     */
    fastiss::i32_t *gen_read_INTBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCTL register
     * \return Object representing the value of the rh850g4 MCTL register
     */
    fastiss::i32_t *gen_read_MCTL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCTL_UIC register field
    * \return Object representing the value of the rh850g4 MCTL_UIC register field
    */
    fastiss::i1_t *gen_read_MCTL_UIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PID register
     * \return Object representing the value of the rh850g4 PID register
     */
    fastiss::i32_t *gen_read_PID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SVLOCK register
     * \return Object representing the value of the rh850g4 SVLOCK register
     */
    fastiss::i32_t *gen_read_SVLOCK(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SCCFG register
     * \return Object representing the value of the rh850g4 SCCFG register
     */
    fastiss::i32_t *gen_read_SCCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 SCCFG_SIZE register field
    * \return Object representing the value of the rh850g4 SCCFG_SIZE register field
    */
    fastiss::i8_t *gen_read_SCCFG_SIZE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 SCBP register
     * \return Object representing the value of the rh850g4 SCBP register
     */
    fastiss::i32_t *gen_read_SCBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 HVCFG register
     * \return Object representing the value of the rh850g4 HVCFG register
     */
    fastiss::i32_t *gen_read_HVCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 HVCFG_HVE register field
    * \return Object representing the value of the rh850g4 HVCFG_HVE register field
    */
    fastiss::i1_t *gen_read_HVCFG_HVE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMCFG register
     * \return Object representing the value of the rh850g4 GMCFG register
     */
    fastiss::i32_t *gen_read_GMCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GMP register field
    * \return Object representing the value of the rh850g4 GMCFG_GMP register field
    */
    fastiss::i1_t *gen_read_GMCFG_GMP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_HMP register field
    * \return Object representing the value of the rh850g4 GMCFG_HMP register field
    */
    fastiss::i1_t *gen_read_GMCFG_HMP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GSYSE register field
    * \return Object representing the value of the rh850g4 GMCFG_GSYSE register field
    */
    fastiss::i1_t *gen_read_GMCFG_GSYSE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU0 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU0 register field
    */
    fastiss::i1_t *gen_read_GMCFG_GCU0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU1 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU1 register field
    */
    fastiss::i1_t *gen_read_GMCFG_GCU1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMCFG_GCU2 register field
    * \return Object representing the value of the rh850g4 GMCFG_GCU2 register field
    */
    fastiss::i1_t *gen_read_GMCFG_GCU2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 HVSB register
     * \return Object representing the value of the rh850g4 HVSB register
     */
    fastiss::i32_t *gen_read_HVSB(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PEID register
     * \return Object representing the value of the rh850g4 PEID register
     */
    fastiss::i32_t *gen_read_PEID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BMID register
     * \return Object representing the value of the rh850g4 BMID register
     */
    fastiss::i32_t *gen_read_BMID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 BMID_BMID register field
    * \return Object representing the value of the rh850g4 BMID_BMID register field
    */
    fastiss::i8_t *gen_read_BMID_BMID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MEA register
     * \return Object representing the value of the rh850g4 MEA register
     */
    fastiss::i32_t *gen_read_MEA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MEI register
     * \return Object representing the value of the rh850g4 MEI register
     */
    fastiss::i32_t *gen_read_MEI(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBCR0 register
     * \return Object representing the value of the rh850g4 RBCR0 register
     */
    fastiss::i32_t *gen_read_RBCR0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR0_MD register field
    * \return Object representing the value of the rh850g4 RBCR0_MD register field
    */
    fastiss::i1_t *gen_read_RBCR0_MD(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR0_BE register field
    * \return Object representing the value of the rh850g4 RBCR0_BE register field
    */
    fastiss::i16_t *gen_read_RBCR0_BE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBCR1 register
     * \return Object representing the value of the rh850g4 RBCR1 register
     */
    fastiss::i32_t *gen_read_RBCR1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBCR1_NC register field
    * \return Object representing the value of the rh850g4 RBCR1_NC register field
    */
    fastiss::i16_t *gen_read_RBCR1_NC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBNR register
     * \return Object representing the value of the rh850g4 RBNR register
     */
    fastiss::i32_t *gen_read_RBNR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBNR_BN register field
    * \return Object representing the value of the rh850g4 RBNR_BN register field
    */
    fastiss::i8_t *gen_read_RBNR_BN(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RBIP register
     * \return Object representing the value of the rh850g4 RBIP register
     */
    fastiss::i32_t *gen_read_RBIP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 RBIP_RBIP register field
    * \return Object representing the value of the rh850g4 RBIP_RBIP register field
    */
    fastiss::i32_t *gen_read_RBIP_RBIP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ISPR register
     * \return Object representing the value of the rh850g4 ISPR register
     */
    fastiss::i32_t *gen_read_ISPR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ISPR_ISP register field
    * \return Object representing the value of the rh850g4 ISPR_ISP register field
    */
    fastiss::i16_t *gen_read_ISPR_ISP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 IMSR register
     * \return Object representing the value of the rh850g4 IMSR register
     */
    fastiss::i32_t *gen_read_IMSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EEIM register field
    * \return Object representing the value of the rh850g4 IMSR_EEIM register field
    */
    fastiss::i1_t *gen_read_IMSR_EEIM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EPLM register field
    * \return Object representing the value of the rh850g4 IMSR_EPLM register field
    */
    fastiss::i1_t *gen_read_IMSR_EPLM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_EID register field
    * \return Object representing the value of the rh850g4 IMSR_EID register field
    */
    fastiss::i1_t *gen_read_IMSR_EID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_ENP register field
    * \return Object representing the value of the rh850g4 IMSR_ENP register field
    */
    fastiss::i1_t *gen_read_IMSR_ENP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_FNP register field
    * \return Object representing the value of the rh850g4 IMSR_FNP register field
    */
    fastiss::i1_t *gen_read_IMSR_FNP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEEIM register field
    * \return Object representing the value of the rh850g4 IMSR_HEEIM register field
    */
    fastiss::i1_t *gen_read_IMSR_HEEIM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEPLM register field
    * \return Object representing the value of the rh850g4 IMSR_HEPLM register field
    */
    fastiss::i1_t *gen_read_IMSR_HEPLM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HEID register field
    * \return Object representing the value of the rh850g4 IMSR_HEID register field
    */
    fastiss::i1_t *gen_read_IMSR_HEID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HENP register field
    * \return Object representing the value of the rh850g4 IMSR_HENP register field
    */
    fastiss::i1_t *gen_read_IMSR_HENP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 IMSR_HFNP register field
    * \return Object representing the value of the rh850g4 IMSR_HFNP register field
    */
    fastiss::i1_t *gen_read_IMSR_HFNP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICSR register
     * \return Object representing the value of the rh850g4 ICSR register
     */
    fastiss::i32_t *gen_read_ICSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICSR_PMEI register field
    * \return Object representing the value of the rh850g4 ICSR_PMEI register field
    */
    fastiss::i1_t *gen_read_ICSR_PMEI(void);
    /**
     * \brief Generates code to query the value of the rh850g4 INTCFG register
     * \return Object representing the value of the rh850g4 INTCFG register
     */
    fastiss::i32_t *gen_read_INTCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_ISPC register field
    * \return Object representing the value of the rh850g4 INTCFG_ISPC register field
    */
    fastiss::i1_t *gen_read_INTCFG_ISPC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_EPL register field
    * \return Object representing the value of the rh850g4 INTCFG_EPL register field
    */
    fastiss::i1_t *gen_read_INTCFG_EPL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 INTCFG_ULNR register field
    * \return Object representing the value of the rh850g4 INTCFG_ULNR register field
    */
    fastiss::i8_t *gen_read_INTCFG_ULNR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PLMR register
     * \return Object representing the value of the rh850g4 PLMR register
     */
    fastiss::i32_t *gen_read_PLMR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 PLMR_PLM register field
    * \return Object representing the value of the rh850g4 PLMR_PLM register field
    */
    fastiss::i8_t *gen_read_PLMR_PLM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXSR register
     * \return Object representing the value of the rh850g4 FXSR register
     */
    fastiss::i32_t *gen_read_FXSR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_FN register field
    * \return Object representing the value of the rh850g4 FXSR_FN register field
    */
    fastiss::i1_t *gen_read_FXSR_FN(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_IF register field
    * \return Object representing the value of the rh850g4 FXSR_IF register field
    */
    fastiss::i1_t *gen_read_FXSR_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_PEM register field
    * \return Object representing the value of the rh850g4 FXSR_PEM register field
    */
    fastiss::i1_t *gen_read_FXSR_PEM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_RM register field
    * \return Object representing the value of the rh850g4 FXSR_RM register field
    */
    fastiss::i8_t *gen_read_FXSR_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_FS register field
    * \return Object representing the value of the rh850g4 FXSR_FS register field
    */
    fastiss::i1_t *gen_read_FXSR_FS(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XC register field
    * \return Object representing the value of the rh850g4 FXSR_XC register field
    */
    fastiss::i8_t *gen_read_FXSR_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XE register field
    * \return Object representing the value of the rh850g4 FXSR_XE register field
    */
    fastiss::i8_t *gen_read_FXSR_XE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXSR_XP register field
    * \return Object representing the value of the rh850g4 FXSR_XP register field
    */
    fastiss::i8_t *gen_read_FXSR_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXST register
     * \return Object representing the value of the rh850g4 FXST register
     */
    fastiss::i32_t *gen_read_FXST(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_XC register field
    * \return Object representing the value of the rh850g4 FXST_XC register field
    */
    fastiss::i8_t *gen_read_FXST_XC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_IF register field
    * \return Object representing the value of the rh850g4 FXST_IF register field
    */
    fastiss::i1_t *gen_read_FXST_IF(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXST_XP register field
    * \return Object representing the value of the rh850g4 FXST_XP register field
    */
    fastiss::i8_t *gen_read_FXST_XP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXINFO register
     * \return Object representing the value of the rh850g4 FXINFO register
     */
    fastiss::i32_t *gen_read_FXINFO(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXINFO_NFPU register field
    * \return Object representing the value of the rh850g4 FXINFO_NFPU register field
    */
    fastiss::i1_t *gen_read_FXINFO_NFPU(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXINFO_RSIZE register field
    * \return Object representing the value of the rh850g4 FXINFO_RSIZE register field
    */
    fastiss::i1_t *gen_read_FXINFO_RSIZE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXCFG register
     * \return Object representing the value of the rh850g4 FXCFG register
     */
    fastiss::i32_t *gen_read_FXCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXCFG_RM register field
    * \return Object representing the value of the rh850g4 FXCFG_RM register field
    */
    fastiss::i8_t *gen_read_FXCFG_RM(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXCFG_XE register field
    * \return Object representing the value of the rh850g4 FXCFG_XE register field
    */
    fastiss::i8_t *gen_read_FXCFG_XE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXXC register
     * \return Object representing the value of the rh850g4 FXXC register
     */
    fastiss::i32_t *gen_read_FXXC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC3 register field
    * \return Object representing the value of the rh850g4 FXXC_XC3 register field
    */
    fastiss::i8_t *gen_read_FXXC_XC3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC2 register field
    * \return Object representing the value of the rh850g4 FXXC_XC2 register field
    */
    fastiss::i8_t *gen_read_FXXC_XC2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC1 register field
    * \return Object representing the value of the rh850g4 FXXC_XC1 register field
    */
    fastiss::i8_t *gen_read_FXXC_XC1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXC_XC0 register field
    * \return Object representing the value of the rh850g4 FXXC_XC0 register field
    */
    fastiss::i8_t *gen_read_FXXC_XC0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 FXXP register
     * \return Object representing the value of the rh850g4 FXXP register
     */
    fastiss::i32_t *gen_read_FXXP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP3 register field
    * \return Object representing the value of the rh850g4 FXXP_XP3 register field
    */
    fastiss::i8_t *gen_read_FXXP_XP3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP2 register field
    * \return Object representing the value of the rh850g4 FXXP_XP2 register field
    */
    fastiss::i8_t *gen_read_FXXP_XP2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP1 register field
    * \return Object representing the value of the rh850g4 FXXP_XP1 register field
    */
    fastiss::i8_t *gen_read_FXXP_XP1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 FXXP_XP0 register field
    * \return Object representing the value of the rh850g4 FXXP_XP0 register field
    */
    fastiss::i8_t *gen_read_FXXP_XP0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIPC register
     * \return Object representing the value of the rh850g4 GMEIPC register
     */
    fastiss::i32_t *gen_read_GMEIPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIPSW register
     * \return Object representing the value of the rh850g4 GMEIPSW register
     */
    fastiss::i32_t *gen_read_GMEIPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEPC register
     * \return Object representing the value of the rh850g4 GMFEPC register
     */
    fastiss::i32_t *gen_read_GMFEPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEPSW register
     * \return Object representing the value of the rh850g4 GMFEPSW register
     */
    fastiss::i32_t *gen_read_GMFEPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMPSW register
     * \return Object representing the value of the rh850g4 GMPSW register
     */
    fastiss::i32_t *gen_read_GMPSW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_Z register field
    * \return Object representing the value of the rh850g4 GMPSW_Z register field
    */
    fastiss::i1_t *gen_read_GMPSW_Z(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_S register field
    * \return Object representing the value of the rh850g4 GMPSW_S register field
    */
    fastiss::i1_t *gen_read_GMPSW_S(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_OV register field
    * \return Object representing the value of the rh850g4 GMPSW_OV register field
    */
    fastiss::i1_t *gen_read_GMPSW_OV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CY register field
    * \return Object representing the value of the rh850g4 GMPSW_CY register field
    */
    fastiss::i1_t *gen_read_GMPSW_CY(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_SAT register field
    * \return Object representing the value of the rh850g4 GMPSW_SAT register field
    */
    fastiss::i1_t *gen_read_GMPSW_SAT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_ID register field
    * \return Object representing the value of the rh850g4 GMPSW_ID register field
    */
    fastiss::i1_t *gen_read_GMPSW_ID(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EP register field
    * \return Object representing the value of the rh850g4 GMPSW_EP register field
    */
    fastiss::i1_t *gen_read_GMPSW_EP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_NP register field
    * \return Object representing the value of the rh850g4 GMPSW_NP register field
    */
    fastiss::i1_t *gen_read_GMPSW_NP(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EBV register field
    * \return Object representing the value of the rh850g4 GMPSW_EBV register field
    */
    fastiss::i1_t *gen_read_GMPSW_EBV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU0 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU0 register field
    */
    fastiss::i1_t *gen_read_GMPSW_CU0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU1 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU1 register field
    */
    fastiss::i1_t *gen_read_GMPSW_CU1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_CU2 register field
    * \return Object representing the value of the rh850g4 GMPSW_CU2 register field
    */
    fastiss::i1_t *gen_read_GMPSW_CU2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_EIMASK register field
    * \return Object representing the value of the rh850g4 GMPSW_EIMASK register field
    */
    fastiss::i8_t *gen_read_GMPSW_EIMASK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPSW_UM register field
    * \return Object representing the value of the rh850g4 GMPSW_UM register field
    */
    fastiss::i1_t *gen_read_GMPSW_UM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMMEA register
     * \return Object representing the value of the rh850g4 GMMEA register
     */
    fastiss::i32_t *gen_read_GMMEA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMMEI register
     * \return Object representing the value of the rh850g4 GMMEI register
     */
    fastiss::i32_t *gen_read_GMMEI(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIIC register
     * \return Object representing the value of the rh850g4 GMEIIC register
     */
    fastiss::i32_t *gen_read_GMEIIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEIC register
     * \return Object representing the value of the rh850g4 GMFEIC register
     */
    fastiss::i32_t *gen_read_GMFEIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMSPID register
     * \return Object representing the value of the rh850g4 GMSPID register
     */
    fastiss::i32_t *gen_read_GMSPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMSPIDLIST register
     * \return Object representing the value of the rh850g4 GMSPIDLIST register
     */
    fastiss::i32_t *gen_read_GMSPIDLIST(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEBASE register
     * \return Object representing the value of the rh850g4 GMEBASE register
     */
    fastiss::i32_t *gen_read_GMEBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_EBASE register field
    * \return Object representing the value of the rh850g4 GMEBASE_EBASE register field
    */
    fastiss::i32_t *gen_read_GMEBASE_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_DV register field
    * \return Object representing the value of the rh850g4 GMEBASE_DV register field
    */
    fastiss::i1_t *gen_read_GMEBASE_DV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMEBASE_RINT register field
    * \return Object representing the value of the rh850g4 GMEBASE_RINT register field
    */
    fastiss::i1_t *gen_read_GMEBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMINTBP register
     * \return Object representing the value of the rh850g4 GMINTBP register
     */
    fastiss::i32_t *gen_read_GMINTBP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMINTCFG register
     * \return Object representing the value of the rh850g4 GMINTCFG register
     */
    fastiss::i32_t *gen_read_GMINTCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_ISPC register field
    * \return Object representing the value of the rh850g4 GMINTCFG_ISPC register field
    */
    fastiss::i1_t *gen_read_GMINTCFG_ISPC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_EPL register field
    * \return Object representing the value of the rh850g4 GMINTCFG_EPL register field
    */
    fastiss::i1_t *gen_read_GMINTCFG_EPL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMINTCFG_ULNR register field
    * \return Object representing the value of the rh850g4 GMINTCFG_ULNR register field
    */
    fastiss::i8_t *gen_read_GMINTCFG_ULNR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMPLMR register
     * \return Object representing the value of the rh850g4 GMPLMR register
     */
    fastiss::i32_t *gen_read_GMPLMR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 GMPLMR_PLM register field
    * \return Object representing the value of the rh850g4 GMPLMR_PLM register field
    */
    fastiss::i8_t *gen_read_GMPLMR_PLM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMSVLOCK register
     * \return Object representing the value of the rh850g4 GMSVLOCK register
     */
    fastiss::i32_t *gen_read_GMSVLOCK(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMMPM register
     * \return Object representing the value of the rh850g4 GMMPM register
     */
    fastiss::i32_t *gen_read_GMMPM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMEIWR register
     * \return Object representing the value of the rh850g4 GMEIWR register
     */
    fastiss::i32_t *gen_read_GMEIWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMFEWR register
     * \return Object representing the value of the rh850g4 GMFEWR register
     */
    fastiss::i32_t *gen_read_GMFEWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 GMPEID register
     * \return Object representing the value of the rh850g4 GMPEID register
     */
    fastiss::i32_t *gen_read_GMPEID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPM register
     * \return Object representing the value of the rh850g4 MPM register
     */
    fastiss::i32_t *gen_read_MPM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPCFG register
     * \return Object representing the value of the rh850g4 MPCFG register
     */
    fastiss::i32_t *gen_read_MPCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_NMPUE register field
    * \return Object representing the value of the rh850g4 MPCFG_NMPUE register field
    */
    fastiss::i8_t *gen_read_MPCFG_NMPUE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_HBE register field
    * \return Object representing the value of the rh850g4 MPCFG_HBE register field
    */
    fastiss::i8_t *gen_read_MPCFG_HBE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_ARCH register field
    * \return Object representing the value of the rh850g4 MPCFG_ARCH register field
    */
    fastiss::i8_t *gen_read_MPCFG_ARCH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_NBK register field
    * \return Object representing the value of the rh850g4 MPCFG_NBK register field
    */
    fastiss::i8_t *gen_read_MPCFG_NBK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPCFG_DMDP register field
    * \return Object representing the value of the rh850g4 MPCFG_DMDP register field
    */
    fastiss::i1_t *gen_read_MPCFG_DMDP(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCA register
     * \return Object representing the value of the rh850g4 MCA register
     */
    fastiss::i32_t *gen_read_MCA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCS register
     * \return Object representing the value of the rh850g4 MCS register
     */
    fastiss::i32_t *gen_read_MCS(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCC register
     * \return Object representing the value of the rh850g4 MCC register
     */
    fastiss::i32_t *gen_read_MCC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCR register
     * \return Object representing the value of the rh850g4 MCR register
     */
    fastiss::i32_t *gen_read_MCR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSXE register field
    * \return Object representing the value of the rh850g4 MCR_HSXE register field
    */
    fastiss::i1_t *gen_read_MCR_HSXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSWE register field
    * \return Object representing the value of the rh850g4 MCR_HSWE register field
    */
    fastiss::i1_t *gen_read_MCR_HSWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HSRE register field
    * \return Object representing the value of the rh850g4 MCR_HSRE register field
    */
    fastiss::i1_t *gen_read_MCR_HSRE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HUXE register field
    * \return Object representing the value of the rh850g4 MCR_HUXE register field
    */
    fastiss::i1_t *gen_read_MCR_HUXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HUWE register field
    * \return Object representing the value of the rh850g4 MCR_HUWE register field
    */
    fastiss::i1_t *gen_read_MCR_HUWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_HURE register field
    * \return Object representing the value of the rh850g4 MCR_HURE register field
    */
    fastiss::i1_t *gen_read_MCR_HURE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSXE register field
    * \return Object representing the value of the rh850g4 MCR_GSXE register field
    */
    fastiss::i1_t *gen_read_MCR_GSXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSWE register field
    * \return Object representing the value of the rh850g4 MCR_GSWE register field
    */
    fastiss::i1_t *gen_read_MCR_GSWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GSRE register field
    * \return Object representing the value of the rh850g4 MCR_GSRE register field
    */
    fastiss::i1_t *gen_read_MCR_GSRE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GUXE register field
    * \return Object representing the value of the rh850g4 MCR_GUXE register field
    */
    fastiss::i1_t *gen_read_MCR_GUXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GUWE register field
    * \return Object representing the value of the rh850g4 MCR_GUWE register field
    */
    fastiss::i1_t *gen_read_MCR_GUWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_GURE register field
    * \return Object representing the value of the rh850g4 MCR_GURE register field
    */
    fastiss::i1_t *gen_read_MCR_GURE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_OV register field
    * \return Object representing the value of the rh850g4 MCR_OV register field
    */
    fastiss::i1_t *gen_read_MCR_OV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SXE register field
    * \return Object representing the value of the rh850g4 MCR_SXE register field
    */
    fastiss::i1_t *gen_read_MCR_SXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SWE register field
    * \return Object representing the value of the rh850g4 MCR_SWE register field
    */
    fastiss::i1_t *gen_read_MCR_SWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_SRE register field
    * \return Object representing the value of the rh850g4 MCR_SRE register field
    */
    fastiss::i1_t *gen_read_MCR_SRE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_UXE register field
    * \return Object representing the value of the rh850g4 MCR_UXE register field
    */
    fastiss::i1_t *gen_read_MCR_UXE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_UWE register field
    * \return Object representing the value of the rh850g4 MCR_UWE register field
    */
    fastiss::i1_t *gen_read_MCR_UWE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCR_URE register field
    * \return Object representing the value of the rh850g4 MCR_URE register field
    */
    fastiss::i1_t *gen_read_MCR_URE(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MCI register
     * \return Object representing the value of the rh850g4 MCI register
     */
    fastiss::i32_t *gen_read_MCI(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MCI_SPID register field
    * \return Object representing the value of the rh850g4 MCI_SPID register field
    */
    fastiss::i8_t *gen_read_MCI_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPIDX register
     * \return Object representing the value of the rh850g4 MPIDX register
     */
    fastiss::i32_t *gen_read_MPIDX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPIDX_IDX register field
    * \return Object representing the value of the rh850g4 MPIDX_IDX register field
    */
    fastiss::i8_t *gen_read_MPIDX_IDX(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPBK register
     * \return Object representing the value of the rh850g4 MPBK register
     */
    fastiss::i32_t *gen_read_MPBK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPBK_BK register field
    * \return Object representing the value of the rh850g4 MPBK_BK register field
    */
    fastiss::i8_t *gen_read_MPBK_BK(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPLA register
     * \return Object representing the value of the rh850g4 MPLA register
     */
    fastiss::i32_t *gen_read_MPLA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPUA register
     * \return Object representing the value of the rh850g4 MPUA register
     */
    fastiss::i32_t *gen_read_MPUA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPAT register
     * \return Object representing the value of the rh850g4 MPAT register
     */
    fastiss::i32_t *gen_read_MPAT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID7 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID7 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID7(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID6 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID6 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID6(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID5 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID5 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID5(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID4 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID4 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID4(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID3 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID3 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID2 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID2 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID1 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID1 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WMPID0 register field
    * \return Object representing the value of the rh850g4 MPAT_WMPID0 register field
    */
    fastiss::i1_t *gen_read_MPAT_WMPID0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID7 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID7 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID7(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID6 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID6 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID6(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID5 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID5 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID5(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID4 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID4 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID4(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID3 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID3 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID2 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID2 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID1 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID1 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RMPID0 register field
    * \return Object representing the value of the rh850g4 MPAT_RMPID0 register field
    */
    fastiss::i1_t *gen_read_MPAT_RMPID0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_WG register field
    * \return Object representing the value of the rh850g4 MPAT_WG register field
    */
    fastiss::i1_t *gen_read_MPAT_WG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_RG register field
    * \return Object representing the value of the rh850g4 MPAT_RG register field
    */
    fastiss::i1_t *gen_read_MPAT_RG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_E register field
    * \return Object representing the value of the rh850g4 MPAT_E register field
    */
    fastiss::i1_t *gen_read_MPAT_E(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SX register field
    * \return Object representing the value of the rh850g4 MPAT_SX register field
    */
    fastiss::i1_t *gen_read_MPAT_SX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SW register field
    * \return Object representing the value of the rh850g4 MPAT_SW register field
    */
    fastiss::i1_t *gen_read_MPAT_SW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_SR register field
    * \return Object representing the value of the rh850g4 MPAT_SR register field
    */
    fastiss::i1_t *gen_read_MPAT_SR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UX register field
    * \return Object representing the value of the rh850g4 MPAT_UX register field
    */
    fastiss::i1_t *gen_read_MPAT_UX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UW register field
    * \return Object representing the value of the rh850g4 MPAT_UW register field
    */
    fastiss::i1_t *gen_read_MPAT_UW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPAT_UR register field
    * \return Object representing the value of the rh850g4 MPAT_UR register field
    */
    fastiss::i1_t *gen_read_MPAT_UR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID0 register
     * \return Object representing the value of the rh850g4 MPID0 register
     */
    fastiss::i32_t *gen_read_MPID0(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID0_SPID register field
    * \return Object representing the value of the rh850g4 MPID0_SPID register field
    */
    fastiss::i8_t *gen_read_MPID0_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID1 register
     * \return Object representing the value of the rh850g4 MPID1 register
     */
    fastiss::i32_t *gen_read_MPID1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID1_SPID register field
    * \return Object representing the value of the rh850g4 MPID1_SPID register field
    */
    fastiss::i8_t *gen_read_MPID1_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID2 register
     * \return Object representing the value of the rh850g4 MPID2 register
     */
    fastiss::i32_t *gen_read_MPID2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID2_SPID register field
    * \return Object representing the value of the rh850g4 MPID2_SPID register field
    */
    fastiss::i8_t *gen_read_MPID2_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID3 register
     * \return Object representing the value of the rh850g4 MPID3 register
     */
    fastiss::i32_t *gen_read_MPID3(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID3_SPID register field
    * \return Object representing the value of the rh850g4 MPID3_SPID register field
    */
    fastiss::i8_t *gen_read_MPID3_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID4 register
     * \return Object representing the value of the rh850g4 MPID4 register
     */
    fastiss::i32_t *gen_read_MPID4(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID4_SPID register field
    * \return Object representing the value of the rh850g4 MPID4_SPID register field
    */
    fastiss::i8_t *gen_read_MPID4_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID5 register
     * \return Object representing the value of the rh850g4 MPID5 register
     */
    fastiss::i32_t *gen_read_MPID5(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID5_SPID register field
    * \return Object representing the value of the rh850g4 MPID5_SPID register field
    */
    fastiss::i8_t *gen_read_MPID5_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID6 register
     * \return Object representing the value of the rh850g4 MPID6 register
     */
    fastiss::i32_t *gen_read_MPID6(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID6_SPID register field
    * \return Object representing the value of the rh850g4 MPID6_SPID register field
    */
    fastiss::i8_t *gen_read_MPID6_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 MPID7 register
     * \return Object representing the value of the rh850g4 MPID7 register
     */
    fastiss::i32_t *gen_read_MPID7(void);
    /**
    * \brief Generates code to query the value of the rh850g4 MPID7_SPID register field
    * \return Object representing the value of the rh850g4 MPID7_SPID register field
    */
    fastiss::i8_t *gen_read_MPID7_SPID(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICTAGL register
     * \return Object representing the value of the rh850g4 ICTAGL register
     */
    fastiss::i32_t *gen_read_ICTAGL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_LPN register field
    * \return Object representing the value of the rh850g4 ICTAGL_LPN register field
    */
    fastiss::i32_t *gen_read_ICTAGL_LPN(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_LRU register field
    * \return Object representing the value of the rh850g4 ICTAGL_LRU register field
    */
    fastiss::i8_t *gen_read_ICTAGL_LRU(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_L register field
    * \return Object representing the value of the rh850g4 ICTAGL_L register field
    */
    fastiss::i1_t *gen_read_ICTAGL_L(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGL_V register field
    * \return Object representing the value of the rh850g4 ICTAGL_V register field
    */
    fastiss::i1_t *gen_read_ICTAGL_V(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICTAGH register
     * \return Object representing the value of the rh850g4 ICTAGH register
     */
    fastiss::i32_t *gen_read_ICTAGH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_WD register field
    * \return Object representing the value of the rh850g4 ICTAGH_WD register field
    */
    fastiss::i1_t *gen_read_ICTAGH_WD(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_PD register field
    * \return Object representing the value of the rh850g4 ICTAGH_PD register field
    */
    fastiss::i1_t *gen_read_ICTAGH_PD(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_WT register field
    * \return Object representing the value of the rh850g4 ICTAGH_WT register field
    */
    fastiss::i1_t *gen_read_ICTAGH_WT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_PT register field
    * \return Object representing the value of the rh850g4 ICTAGH_PT register field
    */
    fastiss::i1_t *gen_read_ICTAGH_PT(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_DATAECC register field
    * \return Object representing the value of the rh850g4 ICTAGH_DATAECC register field
    */
    fastiss::i8_t *gen_read_ICTAGH_DATAECC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICTAGH_TAGECC register field
    * \return Object representing the value of the rh850g4 ICTAGH_TAGECC register field
    */
    fastiss::i8_t *gen_read_ICTAGH_TAGECC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICDATL register
     * \return Object representing the value of the rh850g4 ICDATL register
     */
    fastiss::i32_t *gen_read_ICDATL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICDATH register
     * \return Object representing the value of the rh850g4 ICDATH register
     */
    fastiss::i32_t *gen_read_ICDATH(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICCTRL register
     * \return Object representing the value of the rh850g4 ICCTRL register
     */
    fastiss::i32_t *gen_read_ICCTRL(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_PBS register field
    * \return Object representing the value of the rh850g4 ICCTRL_PBS register field
    */
    fastiss::i1_t *gen_read_ICCTRL_PBS(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHCLR register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHCLR register field
    */
    fastiss::i1_t *gen_read_ICCTRL_ICHCLR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEIV register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEIV register field
    */
    fastiss::i1_t *gen_read_ICCTRL_ICHEIV(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEMK register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEMK register field
    */
    fastiss::i1_t *gen_read_ICCTRL_ICHEMK(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCTRL_ICHEN register field
    * \return Object representing the value of the rh850g4 ICCTRL_ICHEN register field
    */
    fastiss::i1_t *gen_read_ICCTRL_ICHEN(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICCFG register
     * \return Object representing the value of the rh850g4 ICCFG register
     */
    fastiss::i32_t *gen_read_ICCFG(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHSIZE register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHSIZE register field
    */
    fastiss::i8_t *gen_read_ICCFG_ICHSIZE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHLINE register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHLINE register field
    */
    fastiss::i8_t *gen_read_ICCFG_ICHLINE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICCFG_ICHWAY register field
    * \return Object representing the value of the rh850g4 ICCFG_ICHWAY register field
    */
    fastiss::i8_t *gen_read_ICCFG_ICHWAY(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICERR register
     * \return Object representing the value of the rh850g4 ICERR register
     */
    fastiss::i32_t *gen_read_ICERR(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_CISTW register field
    * \return Object representing the value of the rh850g4 ICERR_CISTW register field
    */
    fastiss::i1_t *gen_read_ICERR_CISTW(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESAFE register field
    * \return Object representing the value of the rh850g4 ICERR_ESAFE register field
    */
    fastiss::i1_t *gen_read_ICERR_ESAFE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESMH register field
    * \return Object representing the value of the rh850g4 ICERR_ESMH register field
    */
    fastiss::i1_t *gen_read_ICERR_ESMH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESPBSE register field
    * \return Object representing the value of the rh850g4 ICERR_ESPBSE register field
    */
    fastiss::i1_t *gen_read_ICERR_ESPBSE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESTE1 register field
    * \return Object representing the value of the rh850g4 ICERR_ESTE1 register field
    */
    fastiss::i1_t *gen_read_ICERR_ESTE1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESTE2 register field
    * \return Object representing the value of the rh850g4 ICERR_ESTE2 register field
    */
    fastiss::i1_t *gen_read_ICERR_ESTE2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESDC register field
    * \return Object representing the value of the rh850g4 ICERR_ESDC register field
    */
    fastiss::i1_t *gen_read_ICERR_ESDC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ESDE register field
    * \return Object representing the value of the rh850g4 ICERR_ESDE register field
    */
    fastiss::i1_t *gen_read_ICERR_ESDE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMAFE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMAFE register field
    */
    fastiss::i1_t *gen_read_ICERR_ERMAFE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMMH register field
    * \return Object representing the value of the rh850g4 ICERR_ERMMH register field
    */
    fastiss::i1_t *gen_read_ICERR_ERMMH(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMPBSE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMPBSE register field
    */
    fastiss::i1_t *gen_read_ICERR_ERMPBSE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMTE1 register field
    * \return Object representing the value of the rh850g4 ICERR_ERMTE1 register field
    */
    fastiss::i1_t *gen_read_ICERR_ERMTE1(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMTE2 register field
    * \return Object representing the value of the rh850g4 ICERR_ERMTE2 register field
    */
    fastiss::i1_t *gen_read_ICERR_ERMTE2(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMDC register field
    * \return Object representing the value of the rh850g4 ICERR_ERMDC register field
    */
    fastiss::i1_t *gen_read_ICERR_ERMDC(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ERMDE register field
    * \return Object representing the value of the rh850g4 ICERR_ERMDE register field
    */
    fastiss::i1_t *gen_read_ICERR_ERMDE(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHEWY register field
    * \return Object representing the value of the rh850g4 ICERR_ICHEWY register field
    */
    fastiss::i8_t *gen_read_ICERR_ICHEWY(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHEIX register field
    * \return Object representing the value of the rh850g4 ICERR_ICHEIX register field
    */
    fastiss::i8_t *gen_read_ICERR_ICHEIX(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHERQ register field
    * \return Object representing the value of the rh850g4 ICERR_ICHERQ register field
    */
    fastiss::i1_t *gen_read_ICERR_ICHERQ(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHED register field
    * \return Object representing the value of the rh850g4 ICERR_ICHED register field
    */
    fastiss::i1_t *gen_read_ICERR_ICHED(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHET register field
    * \return Object representing the value of the rh850g4 ICERR_ICHET register field
    */
    fastiss::i1_t *gen_read_ICERR_ICHET(void);
    /**
    * \brief Generates code to query the value of the rh850g4 ICERR_ICHERR register field
    * \return Object representing the value of the rh850g4 ICERR_ICHERR register field
    */
    fastiss::i1_t *gen_read_ICERR_ICHERR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 TSCOUNTL register
     * \return Object representing the value of the rh850g4 TSCOUNTL register
     */
    fastiss::i32_t *gen_read_TSCOUNTL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 TSCOUNTH register
     * \return Object representing the value of the rh850g4 TSCOUNTH register
     */
    fastiss::i32_t *gen_read_TSCOUNTH(void);
    /**
     * \brief Generates code to query the value of the rh850g4 TSCTRL register
     * \return Object representing the value of the rh850g4 TSCTRL register
     */
    fastiss::i32_t *gen_read_TSCTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMUMCTRL register
     * \return Object representing the value of the rh850g4 PMUMCTRL register
     */
    fastiss::i32_t *gen_read_PMUMCTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMGMCTRL register
     * \return Object representing the value of the rh850g4 PMGMCTRL register
     */
    fastiss::i32_t *gen_read_PMGMCTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT0 register
     * \return Object representing the value of the rh850g4 PMCOUNT0 register
     */
    fastiss::i32_t *gen_read_PMCOUNT0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL0 register
     * \return Object representing the value of the rh850g4 PMCTRL0 register
     */
    fastiss::i32_t *gen_read_PMCTRL0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT1 register
     * \return Object representing the value of the rh850g4 PMCOUNT1 register
     */
    fastiss::i32_t *gen_read_PMCOUNT1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL1 register
     * \return Object representing the value of the rh850g4 PMCTRL1 register
     */
    fastiss::i32_t *gen_read_PMCTRL1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT2 register
     * \return Object representing the value of the rh850g4 PMCOUNT2 register
     */
    fastiss::i32_t *gen_read_PMCOUNT2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL2 register
     * \return Object representing the value of the rh850g4 PMCTRL2 register
     */
    fastiss::i32_t *gen_read_PMCTRL2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT3 register
     * \return Object representing the value of the rh850g4 PMCOUNT3 register
     */
    fastiss::i32_t *gen_read_PMCOUNT3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL3 register
     * \return Object representing the value of the rh850g4 PMCTRL3 register
     */
    fastiss::i32_t *gen_read_PMCTRL3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL4 register
     * \return Object representing the value of the rh850g4 PMCTRL4 register
     */
    fastiss::i32_t *gen_read_PMCTRL4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL5 register
     * \return Object representing the value of the rh850g4 PMCTRL5 register
     */
    fastiss::i32_t *gen_read_PMCTRL5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL6 register
     * \return Object representing the value of the rh850g4 PMCTRL6 register
     */
    fastiss::i32_t *gen_read_PMCTRL6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCTRL7 register
     * \return Object representing the value of the rh850g4 PMCTRL7 register
     */
    fastiss::i32_t *gen_read_PMCTRL7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT4 register
     * \return Object representing the value of the rh850g4 PMCOUNT4 register
     */
    fastiss::i32_t *gen_read_PMCOUNT4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT5 register
     * \return Object representing the value of the rh850g4 PMCOUNT5 register
     */
    fastiss::i32_t *gen_read_PMCOUNT5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT6 register
     * \return Object representing the value of the rh850g4 PMCOUNT6 register
     */
    fastiss::i32_t *gen_read_PMCOUNT6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMCOUNT7 register
     * \return Object representing the value of the rh850g4 PMCOUNT7 register
     */
    fastiss::i32_t *gen_read_PMCOUNT7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND0 register
     * \return Object representing the value of the rh850g4 PMSUBCND0 register
     */
    fastiss::i32_t *gen_read_PMSUBCND0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND1 register
     * \return Object representing the value of the rh850g4 PMSUBCND1 register
     */
    fastiss::i32_t *gen_read_PMSUBCND1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND2 register
     * \return Object representing the value of the rh850g4 PMSUBCND2 register
     */
    fastiss::i32_t *gen_read_PMSUBCND2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND3 register
     * \return Object representing the value of the rh850g4 PMSUBCND3 register
     */
    fastiss::i32_t *gen_read_PMSUBCND3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND4 register
     * \return Object representing the value of the rh850g4 PMSUBCND4 register
     */
    fastiss::i32_t *gen_read_PMSUBCND4(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND5 register
     * \return Object representing the value of the rh850g4 PMSUBCND5 register
     */
    fastiss::i32_t *gen_read_PMSUBCND5(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND6 register
     * \return Object representing the value of the rh850g4 PMSUBCND6 register
     */
    fastiss::i32_t *gen_read_PMSUBCND6(void);
    /**
     * \brief Generates code to query the value of the rh850g4 PMSUBCND7 register
     * \return Object representing the value of the rh850g4 PMSUBCND7 register
     */
    fastiss::i32_t *gen_read_PMSUBCND7(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBGEN register
     * \return Object representing the value of the rh850g4 DBGEN register
     */
    fastiss::i32_t *gen_read_DBGEN(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBPSWH register
     * \return Object representing the value of the rh850g4 DBPSWH register
     */
    fastiss::i32_t *gen_read_DBPSWH(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBIC register
     * \return Object representing the value of the rh850g4 DBIC register
     */
    fastiss::i32_t *gen_read_DBIC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBPC register
     * \return Object representing the value of the rh850g4 DBPC register
     */
    fastiss::i32_t *gen_read_DBPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBPSW register
     * \return Object representing the value of the rh850g4 DBPSW register
     */
    fastiss::i32_t *gen_read_DBPSW(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DIR0 register
     * \return Object representing the value of the rh850g4 DIR0 register
     */
    fastiss::i32_t *gen_read_DIR0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DIR1 register
     * \return Object representing the value of the rh850g4 DIR1 register
     */
    fastiss::i32_t *gen_read_DIR1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BPC register
     * \return Object representing the value of the rh850g4 BPC register
     */
    fastiss::i32_t *gen_read_BPC(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BPAV register
     * \return Object representing the value of the rh850g4 BPAV register
     */
    fastiss::i32_t *gen_read_BPAV(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BPAM register
     * \return Object representing the value of the rh850g4 BPAM register
     */
    fastiss::i32_t *gen_read_BPAM(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBCFG register
     * \return Object representing the value of the rh850g4 DBCFG register
     */
    fastiss::i32_t *gen_read_DBCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DBWR register
     * \return Object representing the value of the rh850g4 DBWR register
     */
    fastiss::i32_t *gen_read_DBWR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSTEST0 register
     * \return Object representing the value of the rh850g4 LSTEST0 register
     */
    fastiss::i32_t *gen_read_LSTEST0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSTEST1 register
     * \return Object representing the value of the rh850g4 LSTEST1 register
     */
    fastiss::i32_t *gen_read_LSTEST1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSCFG register
     * \return Object representing the value of the rh850g4 LSCFG register
     */
    fastiss::i32_t *gen_read_LSCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 ICBKEY register
     * \return Object representing the value of the rh850g4 ICBKEY register
     */
    fastiss::i32_t *gen_read_ICBKEY(void);
    /**
     * \brief Generates code to query the value of the rh850g4 IFCR register
     * \return Object representing the value of the rh850g4 IFCR register
     */
    fastiss::i32_t *gen_read_IFCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 IFCR1 register
     * \return Object representing the value of the rh850g4 IFCR1 register
     */
    fastiss::i32_t *gen_read_IFCR1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCTRL0 register
     * \return Object representing the value of the rh850g4 BRPCTRL0 register
     */
    fastiss::i32_t *gen_read_BRPCTRL0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCTRL1 register
     * \return Object representing the value of the rh850g4 BRPCTRL1 register
     */
    fastiss::i32_t *gen_read_BRPCTRL1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPCFG register
     * \return Object representing the value of the rh850g4 BRPCFG register
     */
    fastiss::i32_t *gen_read_BRPCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPACTRL register
     * \return Object representing the value of the rh850g4 BRPACTRL register
     */
    fastiss::i32_t *gen_read_BRPACTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 BRPDATA register
     * \return Object representing the value of the rh850g4 BRPDATA register
     */
    fastiss::i32_t *gen_read_BRPDATA(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DCBKEY register
     * \return Object representing the value of the rh850g4 DCBKEY register
     */
    fastiss::i32_t *gen_read_DCBKEY(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSUCR register
     * \return Object representing the value of the rh850g4 LSUCR register
     */
    fastiss::i32_t *gen_read_LSUCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSULNK0 register
     * \return Object representing the value of the rh850g4 LSULNK0 register
     */
    fastiss::i32_t *gen_read_LSULNK0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 LSULNK1 register
     * \return Object representing the value of the rh850g4 LSULNK1 register
     */
    fastiss::i32_t *gen_read_LSULNK1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLCR register
     * \return Object representing the value of the rh850g4 L1RLCR register
     */
    fastiss::i32_t *gen_read_L1RLCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLNK0 register
     * \return Object representing the value of the rh850g4 L1RLNK0 register
     */
    fastiss::i32_t *gen_read_L1RLNK0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RLNK1 register
     * \return Object representing the value of the rh850g4 L1RLNK1 register
     */
    fastiss::i32_t *gen_read_L1RLNK1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 L1RCFG register
     * \return Object representing the value of the rh850g4 L1RCFG register
     */
    fastiss::i32_t *gen_read_L1RCFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DECFG register
     * \return Object representing the value of the rh850g4 DECFG register
     */
    fastiss::i32_t *gen_read_DECFG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DECTRL register
     * \return Object representing the value of the rh850g4 DECTRL register
     */
    fastiss::i32_t *gen_read_DECTRL(void);
    /**
     * \brief Generates code to query the value of the rh850g4 DEVDS register
     * \return Object representing the value of the rh850g4 DEVDS register
     */
    fastiss::i32_t *gen_read_DEVDS(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBCR register
     * \return Object representing the value of the rh850g4 RDBCR register
     */
    fastiss::i32_t *gen_read_RDBCR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBACR register
     * \return Object representing the value of the rh850g4 RDBACR register
     */
    fastiss::i32_t *gen_read_RDBACR(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBATAG register
     * \return Object representing the value of the rh850g4 RDBATAG register
     */
    fastiss::i32_t *gen_read_RDBATAG(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT0 register
     * \return Object representing the value of the rh850g4 RDBADAT0 register
     */
    fastiss::i32_t *gen_read_RDBADAT0(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT1 register
     * \return Object representing the value of the rh850g4 RDBADAT1 register
     */
    fastiss::i32_t *gen_read_RDBADAT1(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT2 register
     * \return Object representing the value of the rh850g4 RDBADAT2 register
     */
    fastiss::i32_t *gen_read_RDBADAT2(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBADAT3 register
     * \return Object representing the value of the rh850g4 RDBADAT3 register
     */
    fastiss::i32_t *gen_read_RDBADAT3(void);
    /**
     * \brief Generates code to query the value of the rh850g4 RDBSTAT register
     * \return Object representing the value of the rh850g4 RDBSTAT register
     */
    fastiss::i32_t *gen_read_RDBSTAT(void);
    /**
     * \brief Generates code to query the value of the rh850g4 hv_func_support register
     * \return Object representing the value of the rh850g4 hv_func_support register
     */
    fastiss::i8_t *gen_read_hv_func_support(void);
    /**
     * \brief Generates code to query the value of the rh850g4 swd0pcad_exec_count register
     * \return Object representing the value of the rh850g4 swd0pcad_exec_count register
     */
    fastiss::i32_t *gen_read_swd0pcad_exec_count(void);
    /**
     * \brief Generates code to query the value of the rh850g4 swd0pcad_val register
     * \return Object representing the value of the rh850g4 swd0pcad_val register
     */
    fastiss::i32_t *gen_read_swd0pcad_val(void);
    /**
     * \brief Generates code to query the value of the rh850g4 mem_acc_type register
     * \return Object representing the value of the rh850g4 mem_acc_type register
     */
    fastiss::i32_t *gen_read_mem_acc_type(void);

    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \return Object representing the value of the specified rh850g4 GR group register
     */
    fastiss::i32_t *gen_read_GR(rh850g4::GR_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \return Object representing the value of the specified rh850g4 WR group register
     */
    fastiss::i64_t *gen_read_WR(rh850g4::WR_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_0 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_0(rh850g4::SR_SELID_0_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_1 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_1(rh850g4::SR_SELID_1_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_2 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_2(rh850g4::SR_SELID_2_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_3 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_3(rh850g4::SR_SELID_3_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_4 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_4(rh850g4::SR_SELID_4_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_5 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_5(rh850g4::SR_SELID_5_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_9 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_9(rh850g4::SR_SELID_9_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_10 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_10(rh850g4::SR_SELID_10_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_11 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_11(rh850g4::SR_SELID_11_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_12 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_12(rh850g4::SR_SELID_12_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_13 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_13(rh850g4::SR_SELID_13_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_14 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_14(rh850g4::SR_SELID_14_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \return Object representing the value of the specified rh850g4 SR_SELID_15 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_15(rh850g4::SR_SELID_15_index_t index);

    /**
     * \brief Generates code to set the value of the rh850g4 r0 register
     * \param value Object representing the new value of the rh850g4 r0 register
     */
    void gen_write_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r1 register
     * \param value Object representing the new value of the rh850g4 r1 register
     */
    void gen_write_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r2 register
     * \param value Object representing the new value of the rh850g4 r2 register
     */
    void gen_write_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r3 register
     * \param value Object representing the new value of the rh850g4 r3 register
     */
    void gen_write_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r4 register
     * \param value Object representing the new value of the rh850g4 r4 register
     */
    void gen_write_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r5 register
     * \param value Object representing the new value of the rh850g4 r5 register
     */
    void gen_write_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r6 register
     * \param value Object representing the new value of the rh850g4 r6 register
     */
    void gen_write_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r7 register
     * \param value Object representing the new value of the rh850g4 r7 register
     */
    void gen_write_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r8 register
     * \param value Object representing the new value of the rh850g4 r8 register
     */
    void gen_write_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r9 register
     * \param value Object representing the new value of the rh850g4 r9 register
     */
    void gen_write_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r10 register
     * \param value Object representing the new value of the rh850g4 r10 register
     */
    void gen_write_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r11 register
     * \param value Object representing the new value of the rh850g4 r11 register
     */
    void gen_write_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r12 register
     * \param value Object representing the new value of the rh850g4 r12 register
     */
    void gen_write_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r13 register
     * \param value Object representing the new value of the rh850g4 r13 register
     */
    void gen_write_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r14 register
     * \param value Object representing the new value of the rh850g4 r14 register
     */
    void gen_write_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r15 register
     * \param value Object representing the new value of the rh850g4 r15 register
     */
    void gen_write_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r16 register
     * \param value Object representing the new value of the rh850g4 r16 register
     */
    void gen_write_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r17 register
     * \param value Object representing the new value of the rh850g4 r17 register
     */
    void gen_write_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r18 register
     * \param value Object representing the new value of the rh850g4 r18 register
     */
    void gen_write_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r19 register
     * \param value Object representing the new value of the rh850g4 r19 register
     */
    void gen_write_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r20 register
     * \param value Object representing the new value of the rh850g4 r20 register
     */
    void gen_write_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r21 register
     * \param value Object representing the new value of the rh850g4 r21 register
     */
    void gen_write_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r22 register
     * \param value Object representing the new value of the rh850g4 r22 register
     */
    void gen_write_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r23 register
     * \param value Object representing the new value of the rh850g4 r23 register
     */
    void gen_write_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r24 register
     * \param value Object representing the new value of the rh850g4 r24 register
     */
    void gen_write_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r25 register
     * \param value Object representing the new value of the rh850g4 r25 register
     */
    void gen_write_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r26 register
     * \param value Object representing the new value of the rh850g4 r26 register
     */
    void gen_write_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r27 register
     * \param value Object representing the new value of the rh850g4 r27 register
     */
    void gen_write_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r28 register
     * \param value Object representing the new value of the rh850g4 r28 register
     */
    void gen_write_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r29 register
     * \param value Object representing the new value of the rh850g4 r29 register
     */
    void gen_write_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r30 register
     * \param value Object representing the new value of the rh850g4 r30 register
     */
    void gen_write_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 r31 register
     * \param value Object representing the new value of the rh850g4 r31 register
     */
    void gen_write_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PC register
     * \param value Object representing the new value of the rh850g4 PC register
     */
    void gen_write_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr0 register
     * \param value Object representing the new value of the rh850g4 wr0 register
     */
    void gen_write_wr0(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr0_u register
     * \param value Object representing the new value of the rh850g4 wr0_u register
     */
    void gen_write_wr0_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr1 register
     * \param value Object representing the new value of the rh850g4 wr1 register
     */
    void gen_write_wr1(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr1_u register
     * \param value Object representing the new value of the rh850g4 wr1_u register
     */
    void gen_write_wr1_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr2 register
     * \param value Object representing the new value of the rh850g4 wr2 register
     */
    void gen_write_wr2(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr2_u register
     * \param value Object representing the new value of the rh850g4 wr2_u register
     */
    void gen_write_wr2_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr3 register
     * \param value Object representing the new value of the rh850g4 wr3 register
     */
    void gen_write_wr3(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr3_u register
     * \param value Object representing the new value of the rh850g4 wr3_u register
     */
    void gen_write_wr3_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr4 register
     * \param value Object representing the new value of the rh850g4 wr4 register
     */
    void gen_write_wr4(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr4_u register
     * \param value Object representing the new value of the rh850g4 wr4_u register
     */
    void gen_write_wr4_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr5 register
     * \param value Object representing the new value of the rh850g4 wr5 register
     */
    void gen_write_wr5(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr5_u register
     * \param value Object representing the new value of the rh850g4 wr5_u register
     */
    void gen_write_wr5_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr6 register
     * \param value Object representing the new value of the rh850g4 wr6 register
     */
    void gen_write_wr6(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr6_u register
     * \param value Object representing the new value of the rh850g4 wr6_u register
     */
    void gen_write_wr6_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr7 register
     * \param value Object representing the new value of the rh850g4 wr7 register
     */
    void gen_write_wr7(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr7_u register
     * \param value Object representing the new value of the rh850g4 wr7_u register
     */
    void gen_write_wr7_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr8 register
     * \param value Object representing the new value of the rh850g4 wr8 register
     */
    void gen_write_wr8(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr8_u register
     * \param value Object representing the new value of the rh850g4 wr8_u register
     */
    void gen_write_wr8_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr9 register
     * \param value Object representing the new value of the rh850g4 wr9 register
     */
    void gen_write_wr9(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr9_u register
     * \param value Object representing the new value of the rh850g4 wr9_u register
     */
    void gen_write_wr9_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr10 register
     * \param value Object representing the new value of the rh850g4 wr10 register
     */
    void gen_write_wr10(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr10_u register
     * \param value Object representing the new value of the rh850g4 wr10_u register
     */
    void gen_write_wr10_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr11 register
     * \param value Object representing the new value of the rh850g4 wr11 register
     */
    void gen_write_wr11(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr11_u register
     * \param value Object representing the new value of the rh850g4 wr11_u register
     */
    void gen_write_wr11_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr12 register
     * \param value Object representing the new value of the rh850g4 wr12 register
     */
    void gen_write_wr12(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr12_u register
     * \param value Object representing the new value of the rh850g4 wr12_u register
     */
    void gen_write_wr12_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr13 register
     * \param value Object representing the new value of the rh850g4 wr13 register
     */
    void gen_write_wr13(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr13_u register
     * \param value Object representing the new value of the rh850g4 wr13_u register
     */
    void gen_write_wr13_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr14 register
     * \param value Object representing the new value of the rh850g4 wr14 register
     */
    void gen_write_wr14(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr14_u register
     * \param value Object representing the new value of the rh850g4 wr14_u register
     */
    void gen_write_wr14_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr15 register
     * \param value Object representing the new value of the rh850g4 wr15 register
     */
    void gen_write_wr15(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr15_u register
     * \param value Object representing the new value of the rh850g4 wr15_u register
     */
    void gen_write_wr15_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr16 register
     * \param value Object representing the new value of the rh850g4 wr16 register
     */
    void gen_write_wr16(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr16_u register
     * \param value Object representing the new value of the rh850g4 wr16_u register
     */
    void gen_write_wr16_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr17 register
     * \param value Object representing the new value of the rh850g4 wr17 register
     */
    void gen_write_wr17(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr17_u register
     * \param value Object representing the new value of the rh850g4 wr17_u register
     */
    void gen_write_wr17_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr18 register
     * \param value Object representing the new value of the rh850g4 wr18 register
     */
    void gen_write_wr18(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr18_u register
     * \param value Object representing the new value of the rh850g4 wr18_u register
     */
    void gen_write_wr18_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr19 register
     * \param value Object representing the new value of the rh850g4 wr19 register
     */
    void gen_write_wr19(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr19_u register
     * \param value Object representing the new value of the rh850g4 wr19_u register
     */
    void gen_write_wr19_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr20 register
     * \param value Object representing the new value of the rh850g4 wr20 register
     */
    void gen_write_wr20(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr20_u register
     * \param value Object representing the new value of the rh850g4 wr20_u register
     */
    void gen_write_wr20_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr21 register
     * \param value Object representing the new value of the rh850g4 wr21 register
     */
    void gen_write_wr21(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr21_u register
     * \param value Object representing the new value of the rh850g4 wr21_u register
     */
    void gen_write_wr21_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr22 register
     * \param value Object representing the new value of the rh850g4 wr22 register
     */
    void gen_write_wr22(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr22_u register
     * \param value Object representing the new value of the rh850g4 wr22_u register
     */
    void gen_write_wr22_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr23 register
     * \param value Object representing the new value of the rh850g4 wr23 register
     */
    void gen_write_wr23(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr23_u register
     * \param value Object representing the new value of the rh850g4 wr23_u register
     */
    void gen_write_wr23_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr24 register
     * \param value Object representing the new value of the rh850g4 wr24 register
     */
    void gen_write_wr24(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr24_u register
     * \param value Object representing the new value of the rh850g4 wr24_u register
     */
    void gen_write_wr24_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr25 register
     * \param value Object representing the new value of the rh850g4 wr25 register
     */
    void gen_write_wr25(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr25_u register
     * \param value Object representing the new value of the rh850g4 wr25_u register
     */
    void gen_write_wr25_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr26 register
     * \param value Object representing the new value of the rh850g4 wr26 register
     */
    void gen_write_wr26(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr26_u register
     * \param value Object representing the new value of the rh850g4 wr26_u register
     */
    void gen_write_wr26_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr27 register
     * \param value Object representing the new value of the rh850g4 wr27 register
     */
    void gen_write_wr27(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr27_u register
     * \param value Object representing the new value of the rh850g4 wr27_u register
     */
    void gen_write_wr27_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr28 register
     * \param value Object representing the new value of the rh850g4 wr28 register
     */
    void gen_write_wr28(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr28_u register
     * \param value Object representing the new value of the rh850g4 wr28_u register
     */
    void gen_write_wr28_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr29 register
     * \param value Object representing the new value of the rh850g4 wr29 register
     */
    void gen_write_wr29(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr29_u register
     * \param value Object representing the new value of the rh850g4 wr29_u register
     */
    void gen_write_wr29_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr30 register
     * \param value Object representing the new value of the rh850g4 wr30 register
     */
    void gen_write_wr30(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr30_u register
     * \param value Object representing the new value of the rh850g4 wr30_u register
     */
    void gen_write_wr30_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr31 register
     * \param value Object representing the new value of the rh850g4 wr31 register
     */
    void gen_write_wr31(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 wr31_u register
     * \param value Object representing the new value of the rh850g4 wr31_u register
     */
    void gen_write_wr31_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPC register
     * \param value Object representing the new value of the rh850g4 EIPC register
     */
    void gen_write_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSW register
     * \param value Object representing the new value of the rh850g4 EIPSW register
     */
    void gen_write_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPC register
     * \param value Object representing the new value of the rh850g4 FEPC register
     */
    void gen_write_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSW register
     * \param value Object representing the new value of the rh850g4 FEPSW register
     */
    void gen_write_FEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW register
     * \param value Object representing the new value of the rh850g4 PSW register
     */
    void gen_write_PSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_Z register field
     * \param value Object representing the new value of the rh850g4 PSW_Z register field
     */
    void gen_write_PSW_Z(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_S register field
     * \param value Object representing the new value of the rh850g4 PSW_S register field
     */
    void gen_write_PSW_S(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_OV register field
     * \param value Object representing the new value of the rh850g4 PSW_OV register field
     */
    void gen_write_PSW_OV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CY register field
     * \param value Object representing the new value of the rh850g4 PSW_CY register field
     */
    void gen_write_PSW_CY(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_SAT register field
     * \param value Object representing the new value of the rh850g4 PSW_SAT register field
     */
    void gen_write_PSW_SAT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_ID register field
     * \param value Object representing the new value of the rh850g4 PSW_ID register field
     */
    void gen_write_PSW_ID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EP register field
     * \param value Object representing the new value of the rh850g4 PSW_EP register field
     */
    void gen_write_PSW_EP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_NP register field
     * \param value Object representing the new value of the rh850g4 PSW_NP register field
     */
    void gen_write_PSW_NP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EBV register field
     * \param value Object representing the new value of the rh850g4 PSW_EBV register field
     */
    void gen_write_PSW_EBV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU0 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU0 register field
     */
    void gen_write_PSW_CU0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU1 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU1 register field
     */
    void gen_write_PSW_CU1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_CU2 register field
     * \param value Object representing the new value of the rh850g4 PSW_CU2 register field
     */
    void gen_write_PSW_CU2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_EIMASK register field
     * \param value Object representing the new value of the rh850g4 PSW_EIMASK register field
     */
    void gen_write_PSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSW_UM register field
     * \param value Object representing the new value of the rh850g4 PSW_UM register field
     */
    void gen_write_PSW_UM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR register
     * \param value Object representing the new value of the rh850g4 FPSR register
     */
    void gen_write_FPSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_CC register field
     * \param value Object representing the new value of the rh850g4 FPSR_CC register field
     */
    void gen_write_FPSR_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_FN register field
     * \param value Object representing the new value of the rh850g4 FPSR_FN register field
     */
    void gen_write_FPSR_FN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_IF register field
     * \param value Object representing the new value of the rh850g4 FPSR_IF register field
     */
    void gen_write_FPSR_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_PEM register field
     * \param value Object representing the new value of the rh850g4 FPSR_PEM register field
     */
    void gen_write_FPSR_PEM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_RM register field
     * \param value Object representing the new value of the rh850g4 FPSR_RM register field
     */
    void gen_write_FPSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_FS register field
     * \param value Object representing the new value of the rh850g4 FPSR_FS register field
     */
    void gen_write_FPSR_FS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XC register field
     * \param value Object representing the new value of the rh850g4 FPSR_XC register field
     */
    void gen_write_FPSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XE register field
     * \param value Object representing the new value of the rh850g4 FPSR_XE register field
     */
    void gen_write_FPSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPSR_XP register field
     * \param value Object representing the new value of the rh850g4 FPSR_XP register field
     */
    void gen_write_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPEPC register
     * \param value Object representing the new value of the rh850g4 FPEPC register
     */
    void gen_write_FPEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST register
     * \param value Object representing the new value of the rh850g4 FPST register
     */
    void gen_write_FPST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_XC register field
     * \param value Object representing the new value of the rh850g4 FPST_XC register field
     */
    void gen_write_FPST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_IF register field
     * \param value Object representing the new value of the rh850g4 FPST_IF register field
     */
    void gen_write_FPST_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPST_XP register field
     * \param value Object representing the new value of the rh850g4 FPST_XP register field
     */
    void gen_write_FPST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCC register
     * \param value Object representing the new value of the rh850g4 FPCC register
     */
    void gen_write_FPCC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCC_CC register field
     * \param value Object representing the new value of the rh850g4 FPCC_CC register field
     */
    void gen_write_FPCC_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG register
     * \param value Object representing the new value of the rh850g4 FPCFG register
     */
    void gen_write_FPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG_RM register field
     * \param value Object representing the new value of the rh850g4 FPCFG_RM register field
     */
    void gen_write_FPCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FPCFG_XE register field
     * \param value Object representing the new value of the rh850g4 FPCFG_XE register field
     */
    void gen_write_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIIC register
     * \param value Object representing the new value of the rh850g4 EIIC register
     */
    void gen_write_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEIC register
     * \param value Object representing the new value of the rh850g4 FEIC register
     */
    void gen_write_FEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH register
     * \param value Object representing the new value of the rh850g4 PSWH register
     */
    void gen_write_PSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 PSWH_GPID register field
     */
    void gen_write_PSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PSWH_GM register field
     * \param value Object representing the new value of the rh850g4 PSWH_GM register field
     */
    void gen_write_PSWH_GM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 CTPC register
     * \param value Object representing the new value of the rh850g4 CTPC register
     */
    void gen_write_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 CTPSW register
     * \param value Object representing the new value of the rh850g4 CTPSW register
     */
    void gen_write_CTPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH register
     * \param value Object representing the new value of the rh850g4 EIPSWH register
     */
    void gen_write_EIPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 EIPSWH_GPID register field
     */
    void gen_write_EIPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIPSWH_GM register field
     * \param value Object representing the new value of the rh850g4 EIPSWH_GM register field
     */
    void gen_write_EIPSWH_GM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH register
     * \param value Object representing the new value of the rh850g4 FEPSWH register
     */
    void gen_write_FEPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH_GPID register field
     * \param value Object representing the new value of the rh850g4 FEPSWH_GPID register field
     */
    void gen_write_FEPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEPSWH_GM register field
     * \param value Object representing the new value of the rh850g4 FEPSWH_GM register field
     */
    void gen_write_FEPSWH_GM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 CTBP register
     * \param value Object representing the new value of the rh850g4 CTBP register
     */
    void gen_write_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SNZCFG register
     * \param value Object representing the new value of the rh850g4 SNZCFG register
     */
    void gen_write_SNZCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EIWR register
     * \param value Object representing the new value of the rh850g4 EIWR register
     */
    void gen_write_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FEWR register
     * \param value Object representing the new value of the rh850g4 FEWR register
     */
    void gen_write_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SPID register
     * \param value Object representing the new value of the rh850g4 SPID register
     */
    void gen_write_SPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SPIDLIST register
     * \param value Object representing the new value of the rh850g4 SPIDLIST register
     */
    void gen_write_SPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE register
     * \param value Object representing the new value of the rh850g4 RBASE register
     */
    void gen_write_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_RBASE register field
     * \param value Object representing the new value of the rh850g4 RBASE_RBASE register field
     */
    void gen_write_RBASE_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_DV register field
     * \param value Object representing the new value of the rh850g4 RBASE_DV register field
     */
    void gen_write_RBASE_DV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 RBASE_RINT register field
     */
    void gen_write_RBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE register
     * \param value Object representing the new value of the rh850g4 EBASE register
     */
    void gen_write_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_EBASE register field
     * \param value Object representing the new value of the rh850g4 EBASE_EBASE register field
     */
    void gen_write_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_DV register field
     * \param value Object representing the new value of the rh850g4 EBASE_DV register field
     */
    void gen_write_EBASE_DV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 EBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 EBASE_RINT register field
     */
    void gen_write_EBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTBP register
     * \param value Object representing the new value of the rh850g4 INTBP register
     */
    void gen_write_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCTL register
     * \param value Object representing the new value of the rh850g4 MCTL register
     */
    void gen_write_MCTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCTL_UIC register field
     * \param value Object representing the new value of the rh850g4 MCTL_UIC register field
     */
    void gen_write_MCTL_UIC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PID register
     * \param value Object representing the new value of the rh850g4 PID register
     */
    void gen_write_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SVLOCK register
     * \param value Object representing the new value of the rh850g4 SVLOCK register
     */
    void gen_write_SVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SCCFG register
     * \param value Object representing the new value of the rh850g4 SCCFG register
     */
    void gen_write_SCCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SCCFG_SIZE register field
     * \param value Object representing the new value of the rh850g4 SCCFG_SIZE register field
     */
    void gen_write_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 SCBP register
     * \param value Object representing the new value of the rh850g4 SCBP register
     */
    void gen_write_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 HVCFG register
     * \param value Object representing the new value of the rh850g4 HVCFG register
     */
    void gen_write_HVCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 HVCFG_HVE register field
     * \param value Object representing the new value of the rh850g4 HVCFG_HVE register field
     */
    void gen_write_HVCFG_HVE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG register
     * \param value Object representing the new value of the rh850g4 GMCFG register
     */
    void gen_write_GMCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GMP register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GMP register field
     */
    void gen_write_GMCFG_GMP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_HMP register field
     * \param value Object representing the new value of the rh850g4 GMCFG_HMP register field
     */
    void gen_write_GMCFG_HMP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GSYSE register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GSYSE register field
     */
    void gen_write_GMCFG_GSYSE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU0 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU0 register field
     */
    void gen_write_GMCFG_GCU0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU1 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU1 register field
     */
    void gen_write_GMCFG_GCU1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMCFG_GCU2 register field
     * \param value Object representing the new value of the rh850g4 GMCFG_GCU2 register field
     */
    void gen_write_GMCFG_GCU2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 HVSB register
     * \param value Object representing the new value of the rh850g4 HVSB register
     */
    void gen_write_HVSB(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PEID register
     * \param value Object representing the new value of the rh850g4 PEID register
     */
    void gen_write_PEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BMID register
     * \param value Object representing the new value of the rh850g4 BMID register
     */
    void gen_write_BMID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BMID_BMID register field
     * \param value Object representing the new value of the rh850g4 BMID_BMID register field
     */
    void gen_write_BMID_BMID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MEA register
     * \param value Object representing the new value of the rh850g4 MEA register
     */
    void gen_write_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MEI register
     * \param value Object representing the new value of the rh850g4 MEI register
     */
    void gen_write_MEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0 register
     * \param value Object representing the new value of the rh850g4 RBCR0 register
     */
    void gen_write_RBCR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0_MD register field
     * \param value Object representing the new value of the rh850g4 RBCR0_MD register field
     */
    void gen_write_RBCR0_MD(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR0_BE register field
     * \param value Object representing the new value of the rh850g4 RBCR0_BE register field
     */
    void gen_write_RBCR0_BE(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR1 register
     * \param value Object representing the new value of the rh850g4 RBCR1 register
     */
    void gen_write_RBCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBCR1_NC register field
     * \param value Object representing the new value of the rh850g4 RBCR1_NC register field
     */
    void gen_write_RBCR1_NC(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBNR register
     * \param value Object representing the new value of the rh850g4 RBNR register
     */
    void gen_write_RBNR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBNR_BN register field
     * \param value Object representing the new value of the rh850g4 RBNR_BN register field
     */
    void gen_write_RBNR_BN(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBIP register
     * \param value Object representing the new value of the rh850g4 RBIP register
     */
    void gen_write_RBIP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RBIP_RBIP register field
     * \param value Object representing the new value of the rh850g4 RBIP_RBIP register field
     */
    void gen_write_RBIP_RBIP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ISPR register
     * \param value Object representing the new value of the rh850g4 ISPR register
     */
    void gen_write_ISPR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ISPR_ISP register field
     * \param value Object representing the new value of the rh850g4 ISPR_ISP register field
     */
    void gen_write_ISPR_ISP(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR register
     * \param value Object representing the new value of the rh850g4 IMSR register
     */
    void gen_write_IMSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EEIM register field
     * \param value Object representing the new value of the rh850g4 IMSR_EEIM register field
     */
    void gen_write_IMSR_EEIM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EPLM register field
     * \param value Object representing the new value of the rh850g4 IMSR_EPLM register field
     */
    void gen_write_IMSR_EPLM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_EID register field
     * \param value Object representing the new value of the rh850g4 IMSR_EID register field
     */
    void gen_write_IMSR_EID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_ENP register field
     * \param value Object representing the new value of the rh850g4 IMSR_ENP register field
     */
    void gen_write_IMSR_ENP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_FNP register field
     * \param value Object representing the new value of the rh850g4 IMSR_FNP register field
     */
    void gen_write_IMSR_FNP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEEIM register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEEIM register field
     */
    void gen_write_IMSR_HEEIM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEPLM register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEPLM register field
     */
    void gen_write_IMSR_HEPLM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HEID register field
     * \param value Object representing the new value of the rh850g4 IMSR_HEID register field
     */
    void gen_write_IMSR_HEID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HENP register field
     * \param value Object representing the new value of the rh850g4 IMSR_HENP register field
     */
    void gen_write_IMSR_HENP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IMSR_HFNP register field
     * \param value Object representing the new value of the rh850g4 IMSR_HFNP register field
     */
    void gen_write_IMSR_HFNP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICSR register
     * \param value Object representing the new value of the rh850g4 ICSR register
     */
    void gen_write_ICSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICSR_PMEI register field
     * \param value Object representing the new value of the rh850g4 ICSR_PMEI register field
     */
    void gen_write_ICSR_PMEI(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG register
     * \param value Object representing the new value of the rh850g4 INTCFG register
     */
    void gen_write_INTCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_ISPC register field
     * \param value Object representing the new value of the rh850g4 INTCFG_ISPC register field
     */
    void gen_write_INTCFG_ISPC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_EPL register field
     * \param value Object representing the new value of the rh850g4 INTCFG_EPL register field
     */
    void gen_write_INTCFG_EPL(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 INTCFG_ULNR register field
     * \param value Object representing the new value of the rh850g4 INTCFG_ULNR register field
     */
    void gen_write_INTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PLMR register
     * \param value Object representing the new value of the rh850g4 PLMR register
     */
    void gen_write_PLMR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PLMR_PLM register field
     * \param value Object representing the new value of the rh850g4 PLMR_PLM register field
     */
    void gen_write_PLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR register
     * \param value Object representing the new value of the rh850g4 FXSR register
     */
    void gen_write_FXSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_FN register field
     * \param value Object representing the new value of the rh850g4 FXSR_FN register field
     */
    void gen_write_FXSR_FN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_IF register field
     * \param value Object representing the new value of the rh850g4 FXSR_IF register field
     */
    void gen_write_FXSR_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_PEM register field
     * \param value Object representing the new value of the rh850g4 FXSR_PEM register field
     */
    void gen_write_FXSR_PEM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_RM register field
     * \param value Object representing the new value of the rh850g4 FXSR_RM register field
     */
    void gen_write_FXSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_FS register field
     * \param value Object representing the new value of the rh850g4 FXSR_FS register field
     */
    void gen_write_FXSR_FS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XC register field
     * \param value Object representing the new value of the rh850g4 FXSR_XC register field
     */
    void gen_write_FXSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XE register field
     * \param value Object representing the new value of the rh850g4 FXSR_XE register field
     */
    void gen_write_FXSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXSR_XP register field
     * \param value Object representing the new value of the rh850g4 FXSR_XP register field
     */
    void gen_write_FXSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST register
     * \param value Object representing the new value of the rh850g4 FXST register
     */
    void gen_write_FXST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_XC register field
     * \param value Object representing the new value of the rh850g4 FXST_XC register field
     */
    void gen_write_FXST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_IF register field
     * \param value Object representing the new value of the rh850g4 FXST_IF register field
     */
    void gen_write_FXST_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXST_XP register field
     * \param value Object representing the new value of the rh850g4 FXST_XP register field
     */
    void gen_write_FXST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO register
     * \param value Object representing the new value of the rh850g4 FXINFO register
     */
    void gen_write_FXINFO(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO_NFPU register field
     * \param value Object representing the new value of the rh850g4 FXINFO_NFPU register field
     */
    void gen_write_FXINFO_NFPU(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXINFO_RSIZE register field
     * \param value Object representing the new value of the rh850g4 FXINFO_RSIZE register field
     */
    void gen_write_FXINFO_RSIZE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG register
     * \param value Object representing the new value of the rh850g4 FXCFG register
     */
    void gen_write_FXCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG_RM register field
     * \param value Object representing the new value of the rh850g4 FXCFG_RM register field
     */
    void gen_write_FXCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXCFG_XE register field
     * \param value Object representing the new value of the rh850g4 FXCFG_XE register field
     */
    void gen_write_FXCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC register
     * \param value Object representing the new value of the rh850g4 FXXC register
     */
    void gen_write_FXXC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC3 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC3 register field
     */
    void gen_write_FXXC_XC3(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC2 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC2 register field
     */
    void gen_write_FXXC_XC2(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC1 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC1 register field
     */
    void gen_write_FXXC_XC1(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXC_XC0 register field
     * \param value Object representing the new value of the rh850g4 FXXC_XC0 register field
     */
    void gen_write_FXXC_XC0(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP register
     * \param value Object representing the new value of the rh850g4 FXXP register
     */
    void gen_write_FXXP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP3 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP3 register field
     */
    void gen_write_FXXP_XP3(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP2 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP2 register field
     */
    void gen_write_FXXP_XP2(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP1 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP1 register field
     */
    void gen_write_FXXP_XP1(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 FXXP_XP0 register field
     * \param value Object representing the new value of the rh850g4 FXXP_XP0 register field
     */
    void gen_write_FXXP_XP0(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIPC register
     * \param value Object representing the new value of the rh850g4 GMEIPC register
     */
    void gen_write_GMEIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIPSW register
     * \param value Object representing the new value of the rh850g4 GMEIPSW register
     */
    void gen_write_GMEIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEPC register
     * \param value Object representing the new value of the rh850g4 GMFEPC register
     */
    void gen_write_GMFEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEPSW register
     * \param value Object representing the new value of the rh850g4 GMFEPSW register
     */
    void gen_write_GMFEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW register
     * \param value Object representing the new value of the rh850g4 GMPSW register
     */
    void gen_write_GMPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_Z register field
     * \param value Object representing the new value of the rh850g4 GMPSW_Z register field
     */
    void gen_write_GMPSW_Z(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_S register field
     * \param value Object representing the new value of the rh850g4 GMPSW_S register field
     */
    void gen_write_GMPSW_S(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_OV register field
     * \param value Object representing the new value of the rh850g4 GMPSW_OV register field
     */
    void gen_write_GMPSW_OV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CY register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CY register field
     */
    void gen_write_GMPSW_CY(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_SAT register field
     * \param value Object representing the new value of the rh850g4 GMPSW_SAT register field
     */
    void gen_write_GMPSW_SAT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_ID register field
     * \param value Object representing the new value of the rh850g4 GMPSW_ID register field
     */
    void gen_write_GMPSW_ID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EP register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EP register field
     */
    void gen_write_GMPSW_EP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_NP register field
     * \param value Object representing the new value of the rh850g4 GMPSW_NP register field
     */
    void gen_write_GMPSW_NP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EBV register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EBV register field
     */
    void gen_write_GMPSW_EBV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU0 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU0 register field
     */
    void gen_write_GMPSW_CU0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU1 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU1 register field
     */
    void gen_write_GMPSW_CU1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_CU2 register field
     * \param value Object representing the new value of the rh850g4 GMPSW_CU2 register field
     */
    void gen_write_GMPSW_CU2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_EIMASK register field
     * \param value Object representing the new value of the rh850g4 GMPSW_EIMASK register field
     */
    void gen_write_GMPSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPSW_UM register field
     * \param value Object representing the new value of the rh850g4 GMPSW_UM register field
     */
    void gen_write_GMPSW_UM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMMEA register
     * \param value Object representing the new value of the rh850g4 GMMEA register
     */
    void gen_write_GMMEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMMEI register
     * \param value Object representing the new value of the rh850g4 GMMEI register
     */
    void gen_write_GMMEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIIC register
     * \param value Object representing the new value of the rh850g4 GMEIIC register
     */
    void gen_write_GMEIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEIC register
     * \param value Object representing the new value of the rh850g4 GMFEIC register
     */
    void gen_write_GMFEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMSPID register
     * \param value Object representing the new value of the rh850g4 GMSPID register
     */
    void gen_write_GMSPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMSPIDLIST register
     * \param value Object representing the new value of the rh850g4 GMSPIDLIST register
     */
    void gen_write_GMSPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE register
     * \param value Object representing the new value of the rh850g4 GMEBASE register
     */
    void gen_write_GMEBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_EBASE register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_EBASE register field
     */
    void gen_write_GMEBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_DV register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_DV register field
     */
    void gen_write_GMEBASE_DV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEBASE_RINT register field
     * \param value Object representing the new value of the rh850g4 GMEBASE_RINT register field
     */
    void gen_write_GMEBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTBP register
     * \param value Object representing the new value of the rh850g4 GMINTBP register
     */
    void gen_write_GMINTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG register
     * \param value Object representing the new value of the rh850g4 GMINTCFG register
     */
    void gen_write_GMINTCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_ISPC register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_ISPC register field
     */
    void gen_write_GMINTCFG_ISPC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_EPL register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_EPL register field
     */
    void gen_write_GMINTCFG_EPL(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMINTCFG_ULNR register field
     * \param value Object representing the new value of the rh850g4 GMINTCFG_ULNR register field
     */
    void gen_write_GMINTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPLMR register
     * \param value Object representing the new value of the rh850g4 GMPLMR register
     */
    void gen_write_GMPLMR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPLMR_PLM register field
     * \param value Object representing the new value of the rh850g4 GMPLMR_PLM register field
     */
    void gen_write_GMPLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMSVLOCK register
     * \param value Object representing the new value of the rh850g4 GMSVLOCK register
     */
    void gen_write_GMSVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMMPM register
     * \param value Object representing the new value of the rh850g4 GMMPM register
     */
    void gen_write_GMMPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMEIWR register
     * \param value Object representing the new value of the rh850g4 GMEIWR register
     */
    void gen_write_GMEIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMFEWR register
     * \param value Object representing the new value of the rh850g4 GMFEWR register
     */
    void gen_write_GMFEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 GMPEID register
     * \param value Object representing the new value of the rh850g4 GMPEID register
     */
    void gen_write_GMPEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPM register
     * \param value Object representing the new value of the rh850g4 MPM register
     */
    void gen_write_MPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG register
     * \param value Object representing the new value of the rh850g4 MPCFG register
     */
    void gen_write_MPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_NMPUE register field
     * \param value Object representing the new value of the rh850g4 MPCFG_NMPUE register field
     */
    void gen_write_MPCFG_NMPUE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_HBE register field
     * \param value Object representing the new value of the rh850g4 MPCFG_HBE register field
     */
    void gen_write_MPCFG_HBE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_ARCH register field
     * \param value Object representing the new value of the rh850g4 MPCFG_ARCH register field
     */
    void gen_write_MPCFG_ARCH(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_NBK register field
     * \param value Object representing the new value of the rh850g4 MPCFG_NBK register field
     */
    void gen_write_MPCFG_NBK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPCFG_DMDP register field
     * \param value Object representing the new value of the rh850g4 MPCFG_DMDP register field
     */
    void gen_write_MPCFG_DMDP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCA register
     * \param value Object representing the new value of the rh850g4 MCA register
     */
    void gen_write_MCA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCS register
     * \param value Object representing the new value of the rh850g4 MCS register
     */
    void gen_write_MCS(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCC register
     * \param value Object representing the new value of the rh850g4 MCC register
     */
    void gen_write_MCC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR register
     * \param value Object representing the new value of the rh850g4 MCR register
     */
    void gen_write_MCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSXE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSXE register field
     */
    void gen_write_MCR_HSXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSWE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSWE register field
     */
    void gen_write_MCR_HSWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HSRE register field
     * \param value Object representing the new value of the rh850g4 MCR_HSRE register field
     */
    void gen_write_MCR_HSRE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HUXE register field
     * \param value Object representing the new value of the rh850g4 MCR_HUXE register field
     */
    void gen_write_MCR_HUXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HUWE register field
     * \param value Object representing the new value of the rh850g4 MCR_HUWE register field
     */
    void gen_write_MCR_HUWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_HURE register field
     * \param value Object representing the new value of the rh850g4 MCR_HURE register field
     */
    void gen_write_MCR_HURE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSXE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSXE register field
     */
    void gen_write_MCR_GSXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSWE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSWE register field
     */
    void gen_write_MCR_GSWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GSRE register field
     * \param value Object representing the new value of the rh850g4 MCR_GSRE register field
     */
    void gen_write_MCR_GSRE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GUXE register field
     * \param value Object representing the new value of the rh850g4 MCR_GUXE register field
     */
    void gen_write_MCR_GUXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GUWE register field
     * \param value Object representing the new value of the rh850g4 MCR_GUWE register field
     */
    void gen_write_MCR_GUWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_GURE register field
     * \param value Object representing the new value of the rh850g4 MCR_GURE register field
     */
    void gen_write_MCR_GURE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_OV register field
     * \param value Object representing the new value of the rh850g4 MCR_OV register field
     */
    void gen_write_MCR_OV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SXE register field
     * \param value Object representing the new value of the rh850g4 MCR_SXE register field
     */
    void gen_write_MCR_SXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SWE register field
     * \param value Object representing the new value of the rh850g4 MCR_SWE register field
     */
    void gen_write_MCR_SWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_SRE register field
     * \param value Object representing the new value of the rh850g4 MCR_SRE register field
     */
    void gen_write_MCR_SRE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_UXE register field
     * \param value Object representing the new value of the rh850g4 MCR_UXE register field
     */
    void gen_write_MCR_UXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_UWE register field
     * \param value Object representing the new value of the rh850g4 MCR_UWE register field
     */
    void gen_write_MCR_UWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCR_URE register field
     * \param value Object representing the new value of the rh850g4 MCR_URE register field
     */
    void gen_write_MCR_URE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCI register
     * \param value Object representing the new value of the rh850g4 MCI register
     */
    void gen_write_MCI(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MCI_SPID register field
     * \param value Object representing the new value of the rh850g4 MCI_SPID register field
     */
    void gen_write_MCI_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPIDX register
     * \param value Object representing the new value of the rh850g4 MPIDX register
     */
    void gen_write_MPIDX(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPIDX_IDX register field
     * \param value Object representing the new value of the rh850g4 MPIDX_IDX register field
     */
    void gen_write_MPIDX_IDX(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPBK register
     * \param value Object representing the new value of the rh850g4 MPBK register
     */
    void gen_write_MPBK(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPBK_BK register field
     * \param value Object representing the new value of the rh850g4 MPBK_BK register field
     */
    void gen_write_MPBK_BK(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPLA register
     * \param value Object representing the new value of the rh850g4 MPLA register
     */
    void gen_write_MPLA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPUA register
     * \param value Object representing the new value of the rh850g4 MPUA register
     */
    void gen_write_MPUA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT register
     * \param value Object representing the new value of the rh850g4 MPAT register
     */
    void gen_write_MPAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID7 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID7 register field
     */
    void gen_write_MPAT_WMPID7(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID6 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID6 register field
     */
    void gen_write_MPAT_WMPID6(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID5 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID5 register field
     */
    void gen_write_MPAT_WMPID5(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID4 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID4 register field
     */
    void gen_write_MPAT_WMPID4(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID3 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID3 register field
     */
    void gen_write_MPAT_WMPID3(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID2 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID2 register field
     */
    void gen_write_MPAT_WMPID2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID1 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID1 register field
     */
    void gen_write_MPAT_WMPID1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WMPID0 register field
     * \param value Object representing the new value of the rh850g4 MPAT_WMPID0 register field
     */
    void gen_write_MPAT_WMPID0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID7 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID7 register field
     */
    void gen_write_MPAT_RMPID7(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID6 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID6 register field
     */
    void gen_write_MPAT_RMPID6(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID5 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID5 register field
     */
    void gen_write_MPAT_RMPID5(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID4 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID4 register field
     */
    void gen_write_MPAT_RMPID4(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID3 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID3 register field
     */
    void gen_write_MPAT_RMPID3(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID2 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID2 register field
     */
    void gen_write_MPAT_RMPID2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID1 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID1 register field
     */
    void gen_write_MPAT_RMPID1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RMPID0 register field
     * \param value Object representing the new value of the rh850g4 MPAT_RMPID0 register field
     */
    void gen_write_MPAT_RMPID0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_WG register field
     * \param value Object representing the new value of the rh850g4 MPAT_WG register field
     */
    void gen_write_MPAT_WG(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_RG register field
     * \param value Object representing the new value of the rh850g4 MPAT_RG register field
     */
    void gen_write_MPAT_RG(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_E register field
     * \param value Object representing the new value of the rh850g4 MPAT_E register field
     */
    void gen_write_MPAT_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SX register field
     * \param value Object representing the new value of the rh850g4 MPAT_SX register field
     */
    void gen_write_MPAT_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SW register field
     * \param value Object representing the new value of the rh850g4 MPAT_SW register field
     */
    void gen_write_MPAT_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_SR register field
     * \param value Object representing the new value of the rh850g4 MPAT_SR register field
     */
    void gen_write_MPAT_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UX register field
     * \param value Object representing the new value of the rh850g4 MPAT_UX register field
     */
    void gen_write_MPAT_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UW register field
     * \param value Object representing the new value of the rh850g4 MPAT_UW register field
     */
    void gen_write_MPAT_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPAT_UR register field
     * \param value Object representing the new value of the rh850g4 MPAT_UR register field
     */
    void gen_write_MPAT_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID0 register
     * \param value Object representing the new value of the rh850g4 MPID0 register
     */
    void gen_write_MPID0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID0_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID0_SPID register field
     */
    void gen_write_MPID0_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID1 register
     * \param value Object representing the new value of the rh850g4 MPID1 register
     */
    void gen_write_MPID1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID1_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID1_SPID register field
     */
    void gen_write_MPID1_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID2 register
     * \param value Object representing the new value of the rh850g4 MPID2 register
     */
    void gen_write_MPID2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID2_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID2_SPID register field
     */
    void gen_write_MPID2_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID3 register
     * \param value Object representing the new value of the rh850g4 MPID3 register
     */
    void gen_write_MPID3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID3_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID3_SPID register field
     */
    void gen_write_MPID3_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID4 register
     * \param value Object representing the new value of the rh850g4 MPID4 register
     */
    void gen_write_MPID4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID4_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID4_SPID register field
     */
    void gen_write_MPID4_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID5 register
     * \param value Object representing the new value of the rh850g4 MPID5 register
     */
    void gen_write_MPID5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID5_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID5_SPID register field
     */
    void gen_write_MPID5_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID6 register
     * \param value Object representing the new value of the rh850g4 MPID6 register
     */
    void gen_write_MPID6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID6_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID6_SPID register field
     */
    void gen_write_MPID6_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID7 register
     * \param value Object representing the new value of the rh850g4 MPID7 register
     */
    void gen_write_MPID7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 MPID7_SPID register field
     * \param value Object representing the new value of the rh850g4 MPID7_SPID register field
     */
    void gen_write_MPID7_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL register
     * \param value Object representing the new value of the rh850g4 ICTAGL register
     */
    void gen_write_ICTAGL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_LPN register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_LPN register field
     */
    void gen_write_ICTAGL_LPN(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_LRU register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_LRU register field
     */
    void gen_write_ICTAGL_LRU(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_L register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_L register field
     */
    void gen_write_ICTAGL_L(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGL_V register field
     * \param value Object representing the new value of the rh850g4 ICTAGL_V register field
     */
    void gen_write_ICTAGL_V(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH register
     * \param value Object representing the new value of the rh850g4 ICTAGH register
     */
    void gen_write_ICTAGH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_WD register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_WD register field
     */
    void gen_write_ICTAGH_WD(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_PD register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_PD register field
     */
    void gen_write_ICTAGH_PD(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_WT register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_WT register field
     */
    void gen_write_ICTAGH_WT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_PT register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_PT register field
     */
    void gen_write_ICTAGH_PT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_DATAECC register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_DATAECC register field
     */
    void gen_write_ICTAGH_DATAECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICTAGH_TAGECC register field
     * \param value Object representing the new value of the rh850g4 ICTAGH_TAGECC register field
     */
    void gen_write_ICTAGH_TAGECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICDATL register
     * \param value Object representing the new value of the rh850g4 ICDATL register
     */
    void gen_write_ICDATL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICDATH register
     * \param value Object representing the new value of the rh850g4 ICDATH register
     */
    void gen_write_ICDATH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL register
     * \param value Object representing the new value of the rh850g4 ICCTRL register
     */
    void gen_write_ICCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_PBS register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_PBS register field
     */
    void gen_write_ICCTRL_PBS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHCLR register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHCLR register field
     */
    void gen_write_ICCTRL_ICHCLR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEIV register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEIV register field
     */
    void gen_write_ICCTRL_ICHEIV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEMK register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEMK register field
     */
    void gen_write_ICCTRL_ICHEMK(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCTRL_ICHEN register field
     * \param value Object representing the new value of the rh850g4 ICCTRL_ICHEN register field
     */
    void gen_write_ICCTRL_ICHEN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG register
     * \param value Object representing the new value of the rh850g4 ICCFG register
     */
    void gen_write_ICCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHSIZE register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHSIZE register field
     */
    void gen_write_ICCFG_ICHSIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHLINE register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHLINE register field
     */
    void gen_write_ICCFG_ICHLINE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICCFG_ICHWAY register field
     * \param value Object representing the new value of the rh850g4 ICCFG_ICHWAY register field
     */
    void gen_write_ICCFG_ICHWAY(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR register
     * \param value Object representing the new value of the rh850g4 ICERR register
     */
    void gen_write_ICERR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_CISTW register field
     * \param value Object representing the new value of the rh850g4 ICERR_CISTW register field
     */
    void gen_write_ICERR_CISTW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESAFE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESAFE register field
     */
    void gen_write_ICERR_ESAFE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESMH register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESMH register field
     */
    void gen_write_ICERR_ESMH(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESPBSE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESPBSE register field
     */
    void gen_write_ICERR_ESPBSE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESTE1 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESTE1 register field
     */
    void gen_write_ICERR_ESTE1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESTE2 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESTE2 register field
     */
    void gen_write_ICERR_ESTE2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESDC register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESDC register field
     */
    void gen_write_ICERR_ESDC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ESDE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ESDE register field
     */
    void gen_write_ICERR_ESDE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMAFE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMAFE register field
     */
    void gen_write_ICERR_ERMAFE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMMH register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMMH register field
     */
    void gen_write_ICERR_ERMMH(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMPBSE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMPBSE register field
     */
    void gen_write_ICERR_ERMPBSE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMTE1 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMTE1 register field
     */
    void gen_write_ICERR_ERMTE1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMTE2 register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMTE2 register field
     */
    void gen_write_ICERR_ERMTE2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMDC register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMDC register field
     */
    void gen_write_ICERR_ERMDC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ERMDE register field
     * \param value Object representing the new value of the rh850g4 ICERR_ERMDE register field
     */
    void gen_write_ICERR_ERMDE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHEWY register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHEWY register field
     */
    void gen_write_ICERR_ICHEWY(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHEIX register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHEIX register field
     */
    void gen_write_ICERR_ICHEIX(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHERQ register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHERQ register field
     */
    void gen_write_ICERR_ICHERQ(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHED register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHED register field
     */
    void gen_write_ICERR_ICHED(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHET register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHET register field
     */
    void gen_write_ICERR_ICHET(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICERR_ICHERR register field
     * \param value Object representing the new value of the rh850g4 ICERR_ICHERR register field
     */
    void gen_write_ICERR_ICHERR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 TSCOUNTL register
     * \param value Object representing the new value of the rh850g4 TSCOUNTL register
     */
    void gen_write_TSCOUNTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 TSCOUNTH register
     * \param value Object representing the new value of the rh850g4 TSCOUNTH register
     */
    void gen_write_TSCOUNTH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 TSCTRL register
     * \param value Object representing the new value of the rh850g4 TSCTRL register
     */
    void gen_write_TSCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMUMCTRL register
     * \param value Object representing the new value of the rh850g4 PMUMCTRL register
     */
    void gen_write_PMUMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMGMCTRL register
     * \param value Object representing the new value of the rh850g4 PMGMCTRL register
     */
    void gen_write_PMGMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT0 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT0 register
     */
    void gen_write_PMCOUNT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL0 register
     * \param value Object representing the new value of the rh850g4 PMCTRL0 register
     */
    void gen_write_PMCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT1 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT1 register
     */
    void gen_write_PMCOUNT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL1 register
     * \param value Object representing the new value of the rh850g4 PMCTRL1 register
     */
    void gen_write_PMCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT2 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT2 register
     */
    void gen_write_PMCOUNT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL2 register
     * \param value Object representing the new value of the rh850g4 PMCTRL2 register
     */
    void gen_write_PMCTRL2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT3 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT3 register
     */
    void gen_write_PMCOUNT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL3 register
     * \param value Object representing the new value of the rh850g4 PMCTRL3 register
     */
    void gen_write_PMCTRL3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL4 register
     * \param value Object representing the new value of the rh850g4 PMCTRL4 register
     */
    void gen_write_PMCTRL4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL5 register
     * \param value Object representing the new value of the rh850g4 PMCTRL5 register
     */
    void gen_write_PMCTRL5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL6 register
     * \param value Object representing the new value of the rh850g4 PMCTRL6 register
     */
    void gen_write_PMCTRL6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCTRL7 register
     * \param value Object representing the new value of the rh850g4 PMCTRL7 register
     */
    void gen_write_PMCTRL7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT4 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT4 register
     */
    void gen_write_PMCOUNT4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT5 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT5 register
     */
    void gen_write_PMCOUNT5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT6 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT6 register
     */
    void gen_write_PMCOUNT6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMCOUNT7 register
     * \param value Object representing the new value of the rh850g4 PMCOUNT7 register
     */
    void gen_write_PMCOUNT7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND0 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND0 register
     */
    void gen_write_PMSUBCND0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND1 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND1 register
     */
    void gen_write_PMSUBCND1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND2 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND2 register
     */
    void gen_write_PMSUBCND2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND3 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND3 register
     */
    void gen_write_PMSUBCND3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND4 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND4 register
     */
    void gen_write_PMSUBCND4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND5 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND5 register
     */
    void gen_write_PMSUBCND5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND6 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND6 register
     */
    void gen_write_PMSUBCND6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 PMSUBCND7 register
     * \param value Object representing the new value of the rh850g4 PMSUBCND7 register
     */
    void gen_write_PMSUBCND7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBGEN register
     * \param value Object representing the new value of the rh850g4 DBGEN register
     */
    void gen_write_DBGEN(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBPSWH register
     * \param value Object representing the new value of the rh850g4 DBPSWH register
     */
    void gen_write_DBPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBIC register
     * \param value Object representing the new value of the rh850g4 DBIC register
     */
    void gen_write_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBPC register
     * \param value Object representing the new value of the rh850g4 DBPC register
     */
    void gen_write_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBPSW register
     * \param value Object representing the new value of the rh850g4 DBPSW register
     */
    void gen_write_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DIR0 register
     * \param value Object representing the new value of the rh850g4 DIR0 register
     */
    void gen_write_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DIR1 register
     * \param value Object representing the new value of the rh850g4 DIR1 register
     */
    void gen_write_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BPC register
     * \param value Object representing the new value of the rh850g4 BPC register
     */
    void gen_write_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BPAV register
     * \param value Object representing the new value of the rh850g4 BPAV register
     */
    void gen_write_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BPAM register
     * \param value Object representing the new value of the rh850g4 BPAM register
     */
    void gen_write_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBCFG register
     * \param value Object representing the new value of the rh850g4 DBCFG register
     */
    void gen_write_DBCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DBWR register
     * \param value Object representing the new value of the rh850g4 DBWR register
     */
    void gen_write_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSTEST0 register
     * \param value Object representing the new value of the rh850g4 LSTEST0 register
     */
    void gen_write_LSTEST0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSTEST1 register
     * \param value Object representing the new value of the rh850g4 LSTEST1 register
     */
    void gen_write_LSTEST1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSCFG register
     * \param value Object representing the new value of the rh850g4 LSCFG register
     */
    void gen_write_LSCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 ICBKEY register
     * \param value Object representing the new value of the rh850g4 ICBKEY register
     */
    void gen_write_ICBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IFCR register
     * \param value Object representing the new value of the rh850g4 IFCR register
     */
    void gen_write_IFCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 IFCR1 register
     * \param value Object representing the new value of the rh850g4 IFCR1 register
     */
    void gen_write_IFCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCTRL0 register
     * \param value Object representing the new value of the rh850g4 BRPCTRL0 register
     */
    void gen_write_BRPCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCTRL1 register
     * \param value Object representing the new value of the rh850g4 BRPCTRL1 register
     */
    void gen_write_BRPCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPCFG register
     * \param value Object representing the new value of the rh850g4 BRPCFG register
     */
    void gen_write_BRPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPACTRL register
     * \param value Object representing the new value of the rh850g4 BRPACTRL register
     */
    void gen_write_BRPACTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 BRPDATA register
     * \param value Object representing the new value of the rh850g4 BRPDATA register
     */
    void gen_write_BRPDATA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DCBKEY register
     * \param value Object representing the new value of the rh850g4 DCBKEY register
     */
    void gen_write_DCBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSUCR register
     * \param value Object representing the new value of the rh850g4 LSUCR register
     */
    void gen_write_LSUCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSULNK0 register
     * \param value Object representing the new value of the rh850g4 LSULNK0 register
     */
    void gen_write_LSULNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 LSULNK1 register
     * \param value Object representing the new value of the rh850g4 LSULNK1 register
     */
    void gen_write_LSULNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLCR register
     * \param value Object representing the new value of the rh850g4 L1RLCR register
     */
    void gen_write_L1RLCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLNK0 register
     * \param value Object representing the new value of the rh850g4 L1RLNK0 register
     */
    void gen_write_L1RLNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RLNK1 register
     * \param value Object representing the new value of the rh850g4 L1RLNK1 register
     */
    void gen_write_L1RLNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 L1RCFG register
     * \param value Object representing the new value of the rh850g4 L1RCFG register
     */
    void gen_write_L1RCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DECFG register
     * \param value Object representing the new value of the rh850g4 DECFG register
     */
    void gen_write_DECFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DECTRL register
     * \param value Object representing the new value of the rh850g4 DECTRL register
     */
    void gen_write_DECTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 DEVDS register
     * \param value Object representing the new value of the rh850g4 DEVDS register
     */
    void gen_write_DEVDS(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBCR register
     * \param value Object representing the new value of the rh850g4 RDBCR register
     */
    void gen_write_RDBCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBACR register
     * \param value Object representing the new value of the rh850g4 RDBACR register
     */
    void gen_write_RDBACR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBATAG register
     * \param value Object representing the new value of the rh850g4 RDBATAG register
     */
    void gen_write_RDBATAG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT0 register
     * \param value Object representing the new value of the rh850g4 RDBADAT0 register
     */
    void gen_write_RDBADAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT1 register
     * \param value Object representing the new value of the rh850g4 RDBADAT1 register
     */
    void gen_write_RDBADAT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT2 register
     * \param value Object representing the new value of the rh850g4 RDBADAT2 register
     */
    void gen_write_RDBADAT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBADAT3 register
     * \param value Object representing the new value of the rh850g4 RDBADAT3 register
     */
    void gen_write_RDBADAT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 RDBSTAT register
     * \param value Object representing the new value of the rh850g4 RDBSTAT register
     */
    void gen_write_RDBSTAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 hv_func_support register
     * \param value Object representing the new value of the rh850g4 hv_func_support register
     */
    void gen_write_hv_func_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 swd0pcad_exec_count register
     * \param value Object representing the new value of the rh850g4 swd0pcad_exec_count register
     */
    void gen_write_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 swd0pcad_val register
     * \param value Object representing the new value of the rh850g4 swd0pcad_val register
     */
    void gen_write_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850g4 mem_acc_type register
     * \param value Object representing the new value of the rh850g4 mem_acc_type register
     */
    void gen_write_mem_acc_type(fastiss::i32_t *value);

    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 GR register group
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the new value of the specified rh850g4 GR group register
     */
    void gen_write_GR(rh850g4::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 WR register group
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the new value of the specified rh850g4 WR group register
     */
    void gen_write_WR(rh850g4::WR_index_t index, fastiss::i64_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_0 group register
     */
    void gen_write_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_1 group register
     */
    void gen_write_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_2 group register
     */
    void gen_write_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_3 group register
     */
    void gen_write_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_4 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_4 group register
     */
    void gen_write_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_5 group register
     */
    void gen_write_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_9 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_9 group register
     */
    void gen_write_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_10 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_10 group register
     */
    void gen_write_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_11 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_11 group register
     */
    void gen_write_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_12 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_12 group register
     */
    void gen_write_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_13 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_13 group register
     */
    void gen_write_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_14 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_14 group register
     */
    void gen_write_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850g4 SR_SELID_15 register group
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the new value of the specified rh850g4 SR_SELID_15 group register
     */
    void gen_write_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value);

    /**
     * \brief Generates code to increment the value of the rh850g4 r0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r0 register
     */
    void gen_increment_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r1 register
     */
    void gen_increment_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r2 register
     */
    void gen_increment_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r3 register
     */
    void gen_increment_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r4 register
     */
    void gen_increment_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r5 register
     */
    void gen_increment_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r6 register
     */
    void gen_increment_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r7 register
     */
    void gen_increment_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r8 register
     */
    void gen_increment_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r9 register
     */
    void gen_increment_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r10 register
     */
    void gen_increment_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r11 register
     */
    void gen_increment_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r12 register
     */
    void gen_increment_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r13 register
     */
    void gen_increment_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r14 register
     */
    void gen_increment_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r15 register
     */
    void gen_increment_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r16 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r16 register
     */
    void gen_increment_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r17 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r17 register
     */
    void gen_increment_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r18 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r18 register
     */
    void gen_increment_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r19 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r19 register
     */
    void gen_increment_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r20 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r20 register
     */
    void gen_increment_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r21 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r21 register
     */
    void gen_increment_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r22 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r22 register
     */
    void gen_increment_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r23 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r23 register
     */
    void gen_increment_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r24 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r24 register
     */
    void gen_increment_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r25 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r25 register
     */
    void gen_increment_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r26 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r26 register
     */
    void gen_increment_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r27 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r27 register
     */
    void gen_increment_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r28 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r28 register
     */
    void gen_increment_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r29 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r29 register
     */
    void gen_increment_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r30 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r30 register
     */
    void gen_increment_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 r31 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 r31 register
     */
    void gen_increment_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PC register
     */
    void gen_increment_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr0 register
     */
    void gen_increment_wr0(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr0_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr0_u register
     */
    void gen_increment_wr0_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr1 register
     */
    void gen_increment_wr1(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr1_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr1_u register
     */
    void gen_increment_wr1_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr2 register
     */
    void gen_increment_wr2(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr2_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr2_u register
     */
    void gen_increment_wr2_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr3 register
     */
    void gen_increment_wr3(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr3_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr3_u register
     */
    void gen_increment_wr3_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr4 register
     */
    void gen_increment_wr4(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr4_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr4_u register
     */
    void gen_increment_wr4_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr5 register
     */
    void gen_increment_wr5(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr5_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr5_u register
     */
    void gen_increment_wr5_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr6 register
     */
    void gen_increment_wr6(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr6_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr6_u register
     */
    void gen_increment_wr6_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr7 register
     */
    void gen_increment_wr7(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr7_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr7_u register
     */
    void gen_increment_wr7_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr8 register
     */
    void gen_increment_wr8(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr8_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr8_u register
     */
    void gen_increment_wr8_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr9 register
     */
    void gen_increment_wr9(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr9_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr9_u register
     */
    void gen_increment_wr9_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr10 register
     */
    void gen_increment_wr10(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr10_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr10_u register
     */
    void gen_increment_wr10_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr11 register
     */
    void gen_increment_wr11(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr11_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr11_u register
     */
    void gen_increment_wr11_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr12 register
     */
    void gen_increment_wr12(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr12_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr12_u register
     */
    void gen_increment_wr12_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr13 register
     */
    void gen_increment_wr13(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr13_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr13_u register
     */
    void gen_increment_wr13_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr14 register
     */
    void gen_increment_wr14(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr14_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr14_u register
     */
    void gen_increment_wr14_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr15 register
     */
    void gen_increment_wr15(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr15_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr15_u register
     */
    void gen_increment_wr15_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr16 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr16 register
     */
    void gen_increment_wr16(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr16_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr16_u register
     */
    void gen_increment_wr16_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr17 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr17 register
     */
    void gen_increment_wr17(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr17_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr17_u register
     */
    void gen_increment_wr17_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr18 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr18 register
     */
    void gen_increment_wr18(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr18_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr18_u register
     */
    void gen_increment_wr18_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr19 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr19 register
     */
    void gen_increment_wr19(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr19_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr19_u register
     */
    void gen_increment_wr19_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr20 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr20 register
     */
    void gen_increment_wr20(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr20_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr20_u register
     */
    void gen_increment_wr20_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr21 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr21 register
     */
    void gen_increment_wr21(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr21_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr21_u register
     */
    void gen_increment_wr21_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr22 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr22 register
     */
    void gen_increment_wr22(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr22_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr22_u register
     */
    void gen_increment_wr22_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr23 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr23 register
     */
    void gen_increment_wr23(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr23_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr23_u register
     */
    void gen_increment_wr23_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr24 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr24 register
     */
    void gen_increment_wr24(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr24_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr24_u register
     */
    void gen_increment_wr24_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr25 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr25 register
     */
    void gen_increment_wr25(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr25_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr25_u register
     */
    void gen_increment_wr25_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr26 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr26 register
     */
    void gen_increment_wr26(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr26_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr26_u register
     */
    void gen_increment_wr26_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr27 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr27 register
     */
    void gen_increment_wr27(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr27_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr27_u register
     */
    void gen_increment_wr27_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr28 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr28 register
     */
    void gen_increment_wr28(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr28_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr28_u register
     */
    void gen_increment_wr28_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr29 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr29 register
     */
    void gen_increment_wr29(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr29_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr29_u register
     */
    void gen_increment_wr29_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr30 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr30 register
     */
    void gen_increment_wr30(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr30_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr30_u register
     */
    void gen_increment_wr30_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr31 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr31 register
     */
    void gen_increment_wr31(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 wr31_u register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 wr31_u register
     */
    void gen_increment_wr31_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIPC register
     */
    void gen_increment_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIPSW register
     */
    void gen_increment_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEPC register
     */
    void gen_increment_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEPSW register
     */
    void gen_increment_FEPSW(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 PSW_EIMASK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PSW_EIMASK register field
    */
    void gen_increment_PSW_EIMASK(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_CC register field
    */
    void gen_increment_FPSR_CC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_RM register field
    */
    void gen_increment_FPSR_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XC register field
    */
    void gen_increment_FPSR_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XE register field
    */
    void gen_increment_FPSR_XE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPSR_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPSR_XP register field
    */
    void gen_increment_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FPEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FPEPC register
     */
    void gen_increment_FPEPC(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPST_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPST_XC register field
    */
    void gen_increment_FPST_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPST_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPST_XP register field
    */
    void gen_increment_FPST_XP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCC_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCC_CC register field
    */
    void gen_increment_FPCC_CC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCFG_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCFG_RM register field
    */
    void gen_increment_FPCFG_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FPCFG_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FPCFG_XE register field
    */
    void gen_increment_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIIC register
     */
    void gen_increment_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEIC register
     */
    void gen_increment_FEIC(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 PSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PSWH_GPID register field
    */
    void gen_increment_PSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 CTPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTPC register
     */
    void gen_increment_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 CTPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTPSW register
     */
    void gen_increment_CTPSW(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 EIPSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 EIPSWH_GPID register field
    */
    void gen_increment_EIPSWH_GPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FEPSWH_GPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FEPSWH_GPID register field
    */
    void gen_increment_FEPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 CTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 CTBP register
     */
    void gen_increment_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SNZCFG register
     */
    void gen_increment_SNZCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 EIWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 EIWR register
     */
    void gen_increment_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 FEWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 FEWR register
     */
    void gen_increment_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SPID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SPID register
     */
    void gen_increment_SPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SPIDLIST register
     */
    void gen_increment_SPIDLIST(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBASE_RBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBASE_RBASE register field
    */
    void gen_increment_RBASE_RBASE(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 EBASE_EBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 EBASE_EBASE register field
    */
    void gen_increment_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 INTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 INTBP register
     */
    void gen_increment_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PID register
     */
    void gen_increment_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SVLOCK register
     */
    void gen_increment_SVLOCK(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 SCCFG_SIZE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 SCCFG_SIZE register field
    */
    void gen_increment_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 SCBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 SCBP register
     */
    void gen_increment_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 HVSB register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 HVSB register
     */
    void gen_increment_HVSB(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PEID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PEID register
     */
    void gen_increment_PEID(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 BMID_BMID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 BMID_BMID register field
    */
    void gen_increment_BMID_BMID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MEA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MEA register
     */
    void gen_increment_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MEI register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MEI register
     */
    void gen_increment_MEI(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBCR0_BE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBCR0_BE register field
    */
    void gen_increment_RBCR0_BE(fastiss::i16_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBCR1_NC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBCR1_NC register field
    */
    void gen_increment_RBCR1_NC(fastiss::i16_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBNR_BN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBNR_BN register field
    */
    void gen_increment_RBNR_BN(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 RBIP_RBIP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 RBIP_RBIP register field
    */
    void gen_increment_RBIP_RBIP(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ISPR_ISP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ISPR_ISP register field
    */
    void gen_increment_ISPR_ISP(fastiss::i16_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 INTCFG_ULNR register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 INTCFG_ULNR register field
    */
    void gen_increment_INTCFG_ULNR(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 PLMR_PLM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 PLMR_PLM register field
    */
    void gen_increment_PLMR_PLM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_RM register field
    */
    void gen_increment_FXSR_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XC register field
    */
    void gen_increment_FXSR_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XE register field
    */
    void gen_increment_FXSR_XE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXSR_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXSR_XP register field
    */
    void gen_increment_FXSR_XP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXST_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXST_XC register field
    */
    void gen_increment_FXST_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXST_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXST_XP register field
    */
    void gen_increment_FXST_XP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXCFG_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXCFG_RM register field
    */
    void gen_increment_FXCFG_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXCFG_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXCFG_XE register field
    */
    void gen_increment_FXCFG_XE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC3 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC3 register field
    */
    void gen_increment_FXXC_XC3(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC2 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC2 register field
    */
    void gen_increment_FXXC_XC2(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC1 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC1 register field
    */
    void gen_increment_FXXC_XC1(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXC_XC0 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXC_XC0 register field
    */
    void gen_increment_FXXC_XC0(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP3 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP3 register field
    */
    void gen_increment_FXXP_XP3(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP2 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP2 register field
    */
    void gen_increment_FXXP_XP2(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP1 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP1 register field
    */
    void gen_increment_FXXP_XP1(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 FXXP_XP0 register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 FXXP_XP0 register field
    */
    void gen_increment_FXXP_XP0(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIPC register
     */
    void gen_increment_GMEIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIPSW register
     */
    void gen_increment_GMEIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEPC register
     */
    void gen_increment_GMFEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEPSW register
     */
    void gen_increment_GMFEPSW(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMPSW_EIMASK register field
    */
    void gen_increment_GMPSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMEA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMEA register
     */
    void gen_increment_GMMEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMEI register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMEI register
     */
    void gen_increment_GMMEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIIC register
     */
    void gen_increment_GMEIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEIC register
     */
    void gen_increment_GMFEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSPID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSPID register
     */
    void gen_increment_GMSPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSPIDLIST register
     */
    void gen_increment_GMSPIDLIST(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMEBASE_EBASE register field
    */
    void gen_increment_GMEBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMINTBP register
     */
    void gen_increment_GMINTBP(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMINTCFG_ULNR register field
    */
    void gen_increment_GMINTCFG_ULNR(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 GMPLMR_PLM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 GMPLMR_PLM register field
    */
    void gen_increment_GMPLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMSVLOCK register
     */
    void gen_increment_GMSVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMMPM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMMPM register
     */
    void gen_increment_GMMPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMEIWR register
     */
    void gen_increment_GMEIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMFEWR register
     */
    void gen_increment_GMFEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 GMPEID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 GMPEID register
     */
    void gen_increment_GMPEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MPM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPM register
     */
    void gen_increment_MPM(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_NMPUE register field
    */
    void gen_increment_MPCFG_NMPUE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_HBE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_HBE register field
    */
    void gen_increment_MPCFG_HBE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_ARCH register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_ARCH register field
    */
    void gen_increment_MPCFG_ARCH(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPCFG_NBK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPCFG_NBK register field
    */
    void gen_increment_MPCFG_NBK(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MCA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCA register
     */
    void gen_increment_MCA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MCS register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCS register
     */
    void gen_increment_MCS(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MCC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MCC register
     */
    void gen_increment_MCC(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MCI_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MCI_SPID register field
    */
    void gen_increment_MCI_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPIDX_IDX register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPIDX_IDX register field
    */
    void gen_increment_MPIDX_IDX(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPBK_BK register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPBK_BK register field
    */
    void gen_increment_MPBK_BK(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MPLA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPLA register
     */
    void gen_increment_MPLA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 MPUA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 MPUA register
     */
    void gen_increment_MPUA(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID0_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID0_SPID register field
    */
    void gen_increment_MPID0_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID1_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID1_SPID register field
    */
    void gen_increment_MPID1_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID2_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID2_SPID register field
    */
    void gen_increment_MPID2_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID3_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID3_SPID register field
    */
    void gen_increment_MPID3_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID4_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID4_SPID register field
    */
    void gen_increment_MPID4_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID5_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID5_SPID register field
    */
    void gen_increment_MPID5_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID6_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID6_SPID register field
    */
    void gen_increment_MPID6_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 MPID7_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 MPID7_SPID register field
    */
    void gen_increment_MPID7_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGL_LPN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGL_LPN register field
    */
    void gen_increment_ICTAGL_LPN(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGL_LRU register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGL_LRU register field
    */
    void gen_increment_ICTAGL_LRU(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGH_DATAECC register field
    */
    void gen_increment_ICTAGH_DATAECC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICTAGH_TAGECC register field
    */
    void gen_increment_ICTAGH_TAGECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 ICDATL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICDATL register
     */
    void gen_increment_ICDATL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 ICDATH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICDATH register
     */
    void gen_increment_ICDATH(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHSIZE register field
    */
    void gen_increment_ICCFG_ICHSIZE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHLINE register field
    */
    void gen_increment_ICCFG_ICHLINE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICCFG_ICHWAY register field
    */
    void gen_increment_ICCFG_ICHWAY(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICERR_ICHEWY register field
    */
    void gen_increment_ICERR_ICHEWY(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850g4 ICERR_ICHEIX register field
    */
    void gen_increment_ICERR_ICHEIX(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCOUNTL register
     */
    void gen_increment_TSCOUNTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCOUNTH register
     */
    void gen_increment_TSCOUNTH(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 TSCTRL register
     */
    void gen_increment_TSCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMUMCTRL register
     */
    void gen_increment_PMUMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMGMCTRL register
     */
    void gen_increment_PMGMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT0 register
     */
    void gen_increment_PMCOUNT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL0 register
     */
    void gen_increment_PMCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT1 register
     */
    void gen_increment_PMCOUNT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL1 register
     */
    void gen_increment_PMCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT2 register
     */
    void gen_increment_PMCOUNT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL2 register
     */
    void gen_increment_PMCTRL2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT3 register
     */
    void gen_increment_PMCOUNT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL3 register
     */
    void gen_increment_PMCTRL3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL4 register
     */
    void gen_increment_PMCTRL4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL5 register
     */
    void gen_increment_PMCTRL5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL6 register
     */
    void gen_increment_PMCTRL6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCTRL7 register
     */
    void gen_increment_PMCTRL7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT4 register
     */
    void gen_increment_PMCOUNT4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT5 register
     */
    void gen_increment_PMCOUNT5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT6 register
     */
    void gen_increment_PMCOUNT6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMCOUNT7 register
     */
    void gen_increment_PMCOUNT7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND0 register
     */
    void gen_increment_PMSUBCND0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND1 register
     */
    void gen_increment_PMSUBCND1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND2 register
     */
    void gen_increment_PMSUBCND2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND3 register
     */
    void gen_increment_PMSUBCND3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND4 register
     */
    void gen_increment_PMSUBCND4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND5 register
     */
    void gen_increment_PMSUBCND5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND6 register
     */
    void gen_increment_PMSUBCND6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 PMSUBCND7 register
     */
    void gen_increment_PMSUBCND7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBGEN register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBGEN register
     */
    void gen_increment_DBGEN(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPSWH register
     */
    void gen_increment_DBPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBIC register
     */
    void gen_increment_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPC register
     */
    void gen_increment_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBPSW register
     */
    void gen_increment_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DIR0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DIR0 register
     */
    void gen_increment_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DIR1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DIR1 register
     */
    void gen_increment_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPC register
     */
    void gen_increment_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BPAV register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPAV register
     */
    void gen_increment_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BPAM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BPAM register
     */
    void gen_increment_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBCFG register
     */
    void gen_increment_DBCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DBWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DBWR register
     */
    void gen_increment_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSTEST0 register
     */
    void gen_increment_LSTEST0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSTEST1 register
     */
    void gen_increment_LSTEST1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSCFG register
     */
    void gen_increment_LSCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 ICBKEY register
     */
    void gen_increment_ICBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 IFCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 IFCR register
     */
    void gen_increment_IFCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 IFCR1 register
     */
    void gen_increment_IFCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCTRL0 register
     */
    void gen_increment_BRPCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCTRL1 register
     */
    void gen_increment_BRPCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPCFG register
     */
    void gen_increment_BRPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPACTRL register
     */
    void gen_increment_BRPACTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 BRPDATA register
     */
    void gen_increment_BRPDATA(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DCBKEY register
     */
    void gen_increment_DCBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSUCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSUCR register
     */
    void gen_increment_LSUCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSULNK0 register
     */
    void gen_increment_LSULNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 LSULNK1 register
     */
    void gen_increment_LSULNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLCR register
     */
    void gen_increment_L1RLCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLNK0 register
     */
    void gen_increment_L1RLNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RLNK1 register
     */
    void gen_increment_L1RLNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 L1RCFG register
     */
    void gen_increment_L1RCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DECFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DECFG register
     */
    void gen_increment_DECFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DECTRL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DECTRL register
     */
    void gen_increment_DECTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 DEVDS register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 DEVDS register
     */
    void gen_increment_DEVDS(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBCR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBCR register
     */
    void gen_increment_RDBCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBACR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBACR register
     */
    void gen_increment_RDBACR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBATAG register
     */
    void gen_increment_RDBATAG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT0 register
     */
    void gen_increment_RDBADAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT1 register
     */
    void gen_increment_RDBADAT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT2 register
     */
    void gen_increment_RDBADAT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBADAT3 register
     */
    void gen_increment_RDBADAT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 RDBSTAT register
     */
    void gen_increment_RDBSTAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 hv_func_support register
     */
    void gen_increment_hv_func_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 swd0pcad_exec_count register
     */
    void gen_increment_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 swd0pcad_val register
     */
    void gen_increment_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850g4 mem_acc_type register
     */
    void gen_increment_mem_acc_type(fastiss::i32_t *value);

    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 GR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 GR group register
     */
    void gen_increment_GR(rh850g4::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 WR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 WR group register
     */
    void gen_increment_WR(rh850g4::WR_index_t index, fastiss::i64_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_0 group register
     */
    void gen_increment_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_1 group register
     */
    void gen_increment_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_2 group register
     */
    void gen_increment_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_3 group register
     */
    void gen_increment_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_4 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_4 group register
     */
    void gen_increment_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_5 group register
     */
    void gen_increment_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_9 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_9 group register
     */
    void gen_increment_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_10 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_10 group register
     */
    void gen_increment_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_11 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_11 group register
     */
    void gen_increment_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_12 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_12 group register
     */
    void gen_increment_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_13 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_13 group register
     */
    void gen_increment_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_14 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_14 group register
     */
    void gen_increment_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850g4 SR_SELID_15 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850g4 SR_SELID_15 group register
     */
    void gen_increment_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value);

    /**
     * \brief Generates code to decrement the value of the rh850g4 r0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r0 register
     */
    void gen_decrement_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r1 register
     */
    void gen_decrement_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r2 register
     */
    void gen_decrement_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r3 register
     */
    void gen_decrement_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r4 register
     */
    void gen_decrement_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r5 register
     */
    void gen_decrement_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r6 register
     */
    void gen_decrement_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r7 register
     */
    void gen_decrement_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r8 register
     */
    void gen_decrement_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r9 register
     */
    void gen_decrement_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r10 register
     */
    void gen_decrement_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r11 register
     */
    void gen_decrement_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r12 register
     */
    void gen_decrement_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r13 register
     */
    void gen_decrement_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r14 register
     */
    void gen_decrement_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r15 register
     */
    void gen_decrement_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r16 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r16 register
     */
    void gen_decrement_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r17 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r17 register
     */
    void gen_decrement_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r18 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r18 register
     */
    void gen_decrement_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r19 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r19 register
     */
    void gen_decrement_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r20 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r20 register
     */
    void gen_decrement_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r21 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r21 register
     */
    void gen_decrement_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r22 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r22 register
     */
    void gen_decrement_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r23 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r23 register
     */
    void gen_decrement_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r24 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r24 register
     */
    void gen_decrement_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r25 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r25 register
     */
    void gen_decrement_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r26 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r26 register
     */
    void gen_decrement_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r27 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r27 register
     */
    void gen_decrement_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r28 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r28 register
     */
    void gen_decrement_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r29 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r29 register
     */
    void gen_decrement_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r30 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r30 register
     */
    void gen_decrement_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 r31 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 r31 register
     */
    void gen_decrement_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PC register
     */
    void gen_decrement_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr0 register
     */
    void gen_decrement_wr0(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr0_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr0_u register
     */
    void gen_decrement_wr0_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr1 register
     */
    void gen_decrement_wr1(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr1_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr1_u register
     */
    void gen_decrement_wr1_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr2 register
     */
    void gen_decrement_wr2(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr2_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr2_u register
     */
    void gen_decrement_wr2_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr3 register
     */
    void gen_decrement_wr3(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr3_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr3_u register
     */
    void gen_decrement_wr3_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr4 register
     */
    void gen_decrement_wr4(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr4_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr4_u register
     */
    void gen_decrement_wr4_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr5 register
     */
    void gen_decrement_wr5(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr5_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr5_u register
     */
    void gen_decrement_wr5_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr6 register
     */
    void gen_decrement_wr6(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr6_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr6_u register
     */
    void gen_decrement_wr6_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr7 register
     */
    void gen_decrement_wr7(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr7_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr7_u register
     */
    void gen_decrement_wr7_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr8 register
     */
    void gen_decrement_wr8(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr8_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr8_u register
     */
    void gen_decrement_wr8_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr9 register
     */
    void gen_decrement_wr9(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr9_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr9_u register
     */
    void gen_decrement_wr9_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr10 register
     */
    void gen_decrement_wr10(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr10_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr10_u register
     */
    void gen_decrement_wr10_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr11 register
     */
    void gen_decrement_wr11(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr11_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr11_u register
     */
    void gen_decrement_wr11_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr12 register
     */
    void gen_decrement_wr12(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr12_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr12_u register
     */
    void gen_decrement_wr12_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr13 register
     */
    void gen_decrement_wr13(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr13_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr13_u register
     */
    void gen_decrement_wr13_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr14 register
     */
    void gen_decrement_wr14(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr14_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr14_u register
     */
    void gen_decrement_wr14_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr15 register
     */
    void gen_decrement_wr15(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr15_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr15_u register
     */
    void gen_decrement_wr15_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr16 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr16 register
     */
    void gen_decrement_wr16(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr16_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr16_u register
     */
    void gen_decrement_wr16_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr17 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr17 register
     */
    void gen_decrement_wr17(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr17_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr17_u register
     */
    void gen_decrement_wr17_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr18 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr18 register
     */
    void gen_decrement_wr18(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr18_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr18_u register
     */
    void gen_decrement_wr18_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr19 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr19 register
     */
    void gen_decrement_wr19(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr19_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr19_u register
     */
    void gen_decrement_wr19_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr20 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr20 register
     */
    void gen_decrement_wr20(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr20_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr20_u register
     */
    void gen_decrement_wr20_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr21 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr21 register
     */
    void gen_decrement_wr21(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr21_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr21_u register
     */
    void gen_decrement_wr21_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr22 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr22 register
     */
    void gen_decrement_wr22(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr22_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr22_u register
     */
    void gen_decrement_wr22_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr23 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr23 register
     */
    void gen_decrement_wr23(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr23_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr23_u register
     */
    void gen_decrement_wr23_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr24 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr24 register
     */
    void gen_decrement_wr24(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr24_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr24_u register
     */
    void gen_decrement_wr24_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr25 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr25 register
     */
    void gen_decrement_wr25(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr25_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr25_u register
     */
    void gen_decrement_wr25_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr26 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr26 register
     */
    void gen_decrement_wr26(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr26_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr26_u register
     */
    void gen_decrement_wr26_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr27 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr27 register
     */
    void gen_decrement_wr27(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr27_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr27_u register
     */
    void gen_decrement_wr27_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr28 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr28 register
     */
    void gen_decrement_wr28(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr28_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr28_u register
     */
    void gen_decrement_wr28_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr29 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr29 register
     */
    void gen_decrement_wr29(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr29_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr29_u register
     */
    void gen_decrement_wr29_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr30 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr30 register
     */
    void gen_decrement_wr30(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr30_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr30_u register
     */
    void gen_decrement_wr30_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr31 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr31 register
     */
    void gen_decrement_wr31(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 wr31_u register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 wr31_u register
     */
    void gen_decrement_wr31_u(fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPC register
     */
    void gen_decrement_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPSW register
     */
    void gen_decrement_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPC register
     */
    void gen_decrement_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPSW register
     */
    void gen_decrement_FEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PSW_EIMASK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PSW_EIMASK register field
     */
    void gen_decrement_PSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_CC register field
     */
    void gen_decrement_FPSR_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_RM register field
     */
    void gen_decrement_FPSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XC register field
     */
    void gen_decrement_FPSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XE register field
     */
    void gen_decrement_FPSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPSR_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPSR_XP register field
     */
    void gen_decrement_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPEPC register
     */
    void gen_decrement_FPEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPST_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPST_XC register field
     */
    void gen_decrement_FPST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPST_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPST_XP register field
     */
    void gen_decrement_FPST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCC_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCC_CC register field
     */
    void gen_decrement_FPCC_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCFG_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCFG_RM register field
     */
    void gen_decrement_FPCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FPCFG_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FPCFG_XE register field
     */
    void gen_decrement_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIIC register
     */
    void gen_decrement_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEIC register
     */
    void gen_decrement_FEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PSWH_GPID register field
     */
    void gen_decrement_PSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTPC register
     */
    void gen_decrement_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTPSW register
     */
    void gen_decrement_CTPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIPSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIPSWH_GPID register field
     */
    void gen_decrement_EIPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEPSWH_GPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEPSWH_GPID register field
     */
    void gen_decrement_FEPSWH_GPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 CTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 CTBP register
     */
    void gen_decrement_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SNZCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SNZCFG register
     */
    void gen_decrement_SNZCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EIWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EIWR register
     */
    void gen_decrement_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FEWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FEWR register
     */
    void gen_decrement_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SPID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SPID register
     */
    void gen_decrement_SPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SPIDLIST register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SPIDLIST register
     */
    void gen_decrement_SPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBASE_RBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBASE_RBASE register field
     */
    void gen_decrement_RBASE_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 EBASE_EBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 EBASE_EBASE register field
     */
    void gen_decrement_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 INTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 INTBP register
     */
    void gen_decrement_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PID register
     */
    void gen_decrement_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SVLOCK register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SVLOCK register
     */
    void gen_decrement_SVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SCCFG_SIZE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SCCFG_SIZE register field
     */
    void gen_decrement_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 SCBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 SCBP register
     */
    void gen_decrement_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 HVSB register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 HVSB register
     */
    void gen_decrement_HVSB(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PEID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PEID register
     */
    void gen_decrement_PEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BMID_BMID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BMID_BMID register field
     */
    void gen_decrement_BMID_BMID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MEA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MEA register
     */
    void gen_decrement_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MEI register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MEI register
     */
    void gen_decrement_MEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBCR0_BE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBCR0_BE register field
     */
    void gen_decrement_RBCR0_BE(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBCR1_NC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBCR1_NC register field
     */
    void gen_decrement_RBCR1_NC(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBNR_BN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBNR_BN register field
     */
    void gen_decrement_RBNR_BN(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RBIP_RBIP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RBIP_RBIP register field
     */
    void gen_decrement_RBIP_RBIP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ISPR_ISP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ISPR_ISP register field
     */
    void gen_decrement_ISPR_ISP(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 INTCFG_ULNR register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 INTCFG_ULNR register field
     */
    void gen_decrement_INTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PLMR_PLM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PLMR_PLM register field
     */
    void gen_decrement_PLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_RM register field
     */
    void gen_decrement_FXSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XC register field
     */
    void gen_decrement_FXSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XE register field
     */
    void gen_decrement_FXSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXSR_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXSR_XP register field
     */
    void gen_decrement_FXSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXST_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXST_XC register field
     */
    void gen_decrement_FXST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXST_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXST_XP register field
     */
    void gen_decrement_FXST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXCFG_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXCFG_RM register field
     */
    void gen_decrement_FXCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXCFG_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXCFG_XE register field
     */
    void gen_decrement_FXCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC3 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC3 register field
     */
    void gen_decrement_FXXC_XC3(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC2 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC2 register field
     */
    void gen_decrement_FXXC_XC2(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC1 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC1 register field
     */
    void gen_decrement_FXXC_XC1(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXC_XC0 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXC_XC0 register field
     */
    void gen_decrement_FXXC_XC0(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP3 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP3 register field
     */
    void gen_decrement_FXXP_XP3(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP2 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP2 register field
     */
    void gen_decrement_FXXP_XP2(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP1 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP1 register field
     */
    void gen_decrement_FXXP_XP1(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 FXXP_XP0 register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 FXXP_XP0 register field
     */
    void gen_decrement_FXXP_XP0(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIPC register
     */
    void gen_decrement_GMEIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIPSW register
     */
    void gen_decrement_GMEIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEPC register
     */
    void gen_decrement_GMFEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEPSW register
     */
    void gen_decrement_GMFEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPSW_EIMASK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPSW_EIMASK register field
     */
    void gen_decrement_GMPSW_EIMASK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMEA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMEA register
     */
    void gen_decrement_GMMEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMEI register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMEI register
     */
    void gen_decrement_GMMEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIIC register
     */
    void gen_decrement_GMEIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEIC register
     */
    void gen_decrement_GMFEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSPID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSPID register
     */
    void gen_decrement_GMSPID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSPIDLIST register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSPIDLIST register
     */
    void gen_decrement_GMSPIDLIST(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEBASE_EBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEBASE_EBASE register field
     */
    void gen_decrement_GMEBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMINTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMINTBP register
     */
    void gen_decrement_GMINTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMINTCFG_ULNR register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMINTCFG_ULNR register field
     */
    void gen_decrement_GMINTCFG_ULNR(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPLMR_PLM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPLMR_PLM register field
     */
    void gen_decrement_GMPLMR_PLM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMSVLOCK register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMSVLOCK register
     */
    void gen_decrement_GMSVLOCK(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMMPM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMMPM register
     */
    void gen_decrement_GMMPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMEIWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMEIWR register
     */
    void gen_decrement_GMEIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMFEWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMFEWR register
     */
    void gen_decrement_GMFEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 GMPEID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 GMPEID register
     */
    void gen_decrement_GMPEID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPM register
     */
    void gen_decrement_MPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_NMPUE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_NMPUE register field
     */
    void gen_decrement_MPCFG_NMPUE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_HBE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_HBE register field
     */
    void gen_decrement_MPCFG_HBE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_ARCH register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_ARCH register field
     */
    void gen_decrement_MPCFG_ARCH(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPCFG_NBK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPCFG_NBK register field
     */
    void gen_decrement_MPCFG_NBK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCA register
     */
    void gen_decrement_MCA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCS register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCS register
     */
    void gen_decrement_MCS(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCC register
     */
    void gen_decrement_MCC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MCI_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MCI_SPID register field
     */
    void gen_decrement_MCI_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPIDX_IDX register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPIDX_IDX register field
     */
    void gen_decrement_MPIDX_IDX(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPBK_BK register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPBK_BK register field
     */
    void gen_decrement_MPBK_BK(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPLA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPLA register
     */
    void gen_decrement_MPLA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPUA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPUA register
     */
    void gen_decrement_MPUA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID0_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID0_SPID register field
     */
    void gen_decrement_MPID0_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID1_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID1_SPID register field
     */
    void gen_decrement_MPID1_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID2_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID2_SPID register field
     */
    void gen_decrement_MPID2_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID3_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID3_SPID register field
     */
    void gen_decrement_MPID3_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID4_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID4_SPID register field
     */
    void gen_decrement_MPID4_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID5_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID5_SPID register field
     */
    void gen_decrement_MPID5_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID6_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID6_SPID register field
     */
    void gen_decrement_MPID6_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 MPID7_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 MPID7_SPID register field
     */
    void gen_decrement_MPID7_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGL_LPN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGL_LPN register field
     */
    void gen_decrement_ICTAGL_LPN(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGL_LRU register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGL_LRU register field
     */
    void gen_decrement_ICTAGL_LRU(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGH_DATAECC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGH_DATAECC register field
     */
    void gen_decrement_ICTAGH_DATAECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICTAGH_TAGECC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICTAGH_TAGECC register field
     */
    void gen_decrement_ICTAGH_TAGECC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICDATL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICDATL register
     */
    void gen_decrement_ICDATL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICDATH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICDATH register
     */
    void gen_decrement_ICDATH(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHSIZE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHSIZE register field
     */
    void gen_decrement_ICCFG_ICHSIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHLINE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHLINE register field
     */
    void gen_decrement_ICCFG_ICHLINE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICCFG_ICHWAY register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICCFG_ICHWAY register field
     */
    void gen_decrement_ICCFG_ICHWAY(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICERR_ICHEWY register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICERR_ICHEWY register field
     */
    void gen_decrement_ICERR_ICHEWY(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICERR_ICHEIX register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICERR_ICHEIX register field
     */
    void gen_decrement_ICERR_ICHEIX(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCOUNTL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCOUNTL register
     */
    void gen_decrement_TSCOUNTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCOUNTH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCOUNTH register
     */
    void gen_decrement_TSCOUNTH(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 TSCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 TSCTRL register
     */
    void gen_decrement_TSCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMUMCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMUMCTRL register
     */
    void gen_decrement_PMUMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMGMCTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMGMCTRL register
     */
    void gen_decrement_PMGMCTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT0 register
     */
    void gen_decrement_PMCOUNT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL0 register
     */
    void gen_decrement_PMCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT1 register
     */
    void gen_decrement_PMCOUNT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL1 register
     */
    void gen_decrement_PMCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT2 register
     */
    void gen_decrement_PMCOUNT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL2 register
     */
    void gen_decrement_PMCTRL2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT3 register
     */
    void gen_decrement_PMCOUNT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL3 register
     */
    void gen_decrement_PMCTRL3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL4 register
     */
    void gen_decrement_PMCTRL4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL5 register
     */
    void gen_decrement_PMCTRL5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL6 register
     */
    void gen_decrement_PMCTRL6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCTRL7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCTRL7 register
     */
    void gen_decrement_PMCTRL7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT4 register
     */
    void gen_decrement_PMCOUNT4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT5 register
     */
    void gen_decrement_PMCOUNT5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT6 register
     */
    void gen_decrement_PMCOUNT6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMCOUNT7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMCOUNT7 register
     */
    void gen_decrement_PMCOUNT7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND0 register
     */
    void gen_decrement_PMSUBCND0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND1 register
     */
    void gen_decrement_PMSUBCND1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND2 register
     */
    void gen_decrement_PMSUBCND2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND3 register
     */
    void gen_decrement_PMSUBCND3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND4 register
     */
    void gen_decrement_PMSUBCND4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND5 register
     */
    void gen_decrement_PMSUBCND5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND6 register
     */
    void gen_decrement_PMSUBCND6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 PMSUBCND7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 PMSUBCND7 register
     */
    void gen_decrement_PMSUBCND7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBGEN register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBGEN register
     */
    void gen_decrement_DBGEN(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPSWH register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPSWH register
     */
    void gen_decrement_DBPSWH(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBIC register
     */
    void gen_decrement_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPC register
     */
    void gen_decrement_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBPSW register
     */
    void gen_decrement_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DIR0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DIR0 register
     */
    void gen_decrement_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DIR1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DIR1 register
     */
    void gen_decrement_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPC register
     */
    void gen_decrement_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPAV register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPAV register
     */
    void gen_decrement_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BPAM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BPAM register
     */
    void gen_decrement_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBCFG register
     */
    void gen_decrement_DBCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DBWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DBWR register
     */
    void gen_decrement_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSTEST0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSTEST0 register
     */
    void gen_decrement_LSTEST0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSTEST1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSTEST1 register
     */
    void gen_decrement_LSTEST1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSCFG register
     */
    void gen_decrement_LSCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 ICBKEY register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 ICBKEY register
     */
    void gen_decrement_ICBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 IFCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 IFCR register
     */
    void gen_decrement_IFCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 IFCR1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 IFCR1 register
     */
    void gen_decrement_IFCR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCTRL0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCTRL0 register
     */
    void gen_decrement_BRPCTRL0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCTRL1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCTRL1 register
     */
    void gen_decrement_BRPCTRL1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPCFG register
     */
    void gen_decrement_BRPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPACTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPACTRL register
     */
    void gen_decrement_BRPACTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 BRPDATA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 BRPDATA register
     */
    void gen_decrement_BRPDATA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DCBKEY register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DCBKEY register
     */
    void gen_decrement_DCBKEY(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSUCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSUCR register
     */
    void gen_decrement_LSUCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSULNK0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSULNK0 register
     */
    void gen_decrement_LSULNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 LSULNK1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 LSULNK1 register
     */
    void gen_decrement_LSULNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLCR register
     */
    void gen_decrement_L1RLCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLNK0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLNK0 register
     */
    void gen_decrement_L1RLNK0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RLNK1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RLNK1 register
     */
    void gen_decrement_L1RLNK1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 L1RCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 L1RCFG register
     */
    void gen_decrement_L1RCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DECFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DECFG register
     */
    void gen_decrement_DECFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DECTRL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DECTRL register
     */
    void gen_decrement_DECTRL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 DEVDS register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 DEVDS register
     */
    void gen_decrement_DEVDS(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBCR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBCR register
     */
    void gen_decrement_RDBCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBACR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBACR register
     */
    void gen_decrement_RDBACR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBATAG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBATAG register
     */
    void gen_decrement_RDBATAG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT0 register
     */
    void gen_decrement_RDBADAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT1 register
     */
    void gen_decrement_RDBADAT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT2 register
     */
    void gen_decrement_RDBADAT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBADAT3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBADAT3 register
     */
    void gen_decrement_RDBADAT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 RDBSTAT register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 RDBSTAT register
     */
    void gen_decrement_RDBSTAT(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 hv_func_support register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 hv_func_support register
     */
    void gen_decrement_hv_func_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 swd0pcad_exec_count register
     */
    void gen_decrement_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 swd0pcad_val register
     */
    void gen_decrement_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850g4 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850g4 mem_acc_type register
     */
    void gen_decrement_mem_acc_type(fastiss::i32_t *value);

    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 GR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 GR register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 GR group register
     */
    void gen_decrement_GR(rh850g4::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 WR register group by the specified amount
     * \param index Index value of the required register from the rh850g4 WR register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 WR group register
     */
    void gen_decrement_WR(rh850g4::WR_index_t index, fastiss::i64_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_0 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_0 group register
     */
    void gen_decrement_SR_SELID_0(rh850g4::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_1 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_1 group register
     */
    void gen_decrement_SR_SELID_1(rh850g4::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_2 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_2 group register
     */
    void gen_decrement_SR_SELID_2(rh850g4::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_3 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_3 group register
     */
    void gen_decrement_SR_SELID_3(rh850g4::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_4 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_4 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_4 group register
     */
    void gen_decrement_SR_SELID_4(rh850g4::SR_SELID_4_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_5 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_5 group register
     */
    void gen_decrement_SR_SELID_5(rh850g4::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_9 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_9 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_9 group register
     */
    void gen_decrement_SR_SELID_9(rh850g4::SR_SELID_9_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_10 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_10 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_10 group register
     */
    void gen_decrement_SR_SELID_10(rh850g4::SR_SELID_10_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_11 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_11 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_11 group register
     */
    void gen_decrement_SR_SELID_11(rh850g4::SR_SELID_11_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_12 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_12 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_12 group register
     */
    void gen_decrement_SR_SELID_12(rh850g4::SR_SELID_12_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_13 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_13 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_13 group register
     */
    void gen_decrement_SR_SELID_13(rh850g4::SR_SELID_13_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_14 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_14 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_14 group register
     */
    void gen_decrement_SR_SELID_14(rh850g4::SR_SELID_14_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850g4 SR_SELID_15 register group by the specified amount
     * \param index Index value of the required register from the rh850g4 SR_SELID_15 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850g4 SR_SELID_15 group register
     */
    void gen_decrement_SR_SELID_15(rh850g4::SR_SELID_15_index_t index, fastiss::i32_t *value);

  private:
    /**
     * \brief Handle to CPU state implementation
     */
    rh850g4::cpu_state_if_t *m_cpu_state_impl;
  };
}

#endif // _RH850G4_CPU_STATE_BRIDGE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
