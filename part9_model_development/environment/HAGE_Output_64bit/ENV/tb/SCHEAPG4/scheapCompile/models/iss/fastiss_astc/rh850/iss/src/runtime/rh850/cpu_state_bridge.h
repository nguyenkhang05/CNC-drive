//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file cpu_state_bridge.h
 * \brief Defines the rh850 CPU state bridge class
 */

#ifndef _RH850_CPU_STATE_BRIDGE_H_
#define _RH850_CPU_STATE_BRIDGE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/cpu_state_if.h"

//------------------------------------------------------------------------------
// rh850 CPU state bridge definition - redirects calls on the CPU State
// Interface to the CPU state implementation.
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850 specific components
 */
namespace rh850
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
    cpu_state_bridge_t(rh850::cpu_state_if_t *cpu_state_impl);
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
     * \brief Generates string representing name of the rh850 r0 register
     * \return String representing the name of the rh850 r0 register
     */
    std::string register_name_r0(void);
    /**
     * \brief Generates string representing name of the rh850 r1 register
     * \return String representing the name of the rh850 r1 register
     */
    std::string register_name_r1(void);
    /**
     * \brief Generates string representing name of the rh850 r2 register
     * \return String representing the name of the rh850 r2 register
     */
    std::string register_name_r2(void);
    /**
     * \brief Generates string representing name of the rh850 r3 register
     * \return String representing the name of the rh850 r3 register
     */
    std::string register_name_r3(void);
    /**
     * \brief Generates string representing name of the rh850 r4 register
     * \return String representing the name of the rh850 r4 register
     */
    std::string register_name_r4(void);
    /**
     * \brief Generates string representing name of the rh850 r5 register
     * \return String representing the name of the rh850 r5 register
     */
    std::string register_name_r5(void);
    /**
     * \brief Generates string representing name of the rh850 r6 register
     * \return String representing the name of the rh850 r6 register
     */
    std::string register_name_r6(void);
    /**
     * \brief Generates string representing name of the rh850 r7 register
     * \return String representing the name of the rh850 r7 register
     */
    std::string register_name_r7(void);
    /**
     * \brief Generates string representing name of the rh850 r8 register
     * \return String representing the name of the rh850 r8 register
     */
    std::string register_name_r8(void);
    /**
     * \brief Generates string representing name of the rh850 r9 register
     * \return String representing the name of the rh850 r9 register
     */
    std::string register_name_r9(void);
    /**
     * \brief Generates string representing name of the rh850 r10 register
     * \return String representing the name of the rh850 r10 register
     */
    std::string register_name_r10(void);
    /**
     * \brief Generates string representing name of the rh850 r11 register
     * \return String representing the name of the rh850 r11 register
     */
    std::string register_name_r11(void);
    /**
     * \brief Generates string representing name of the rh850 r12 register
     * \return String representing the name of the rh850 r12 register
     */
    std::string register_name_r12(void);
    /**
     * \brief Generates string representing name of the rh850 r13 register
     * \return String representing the name of the rh850 r13 register
     */
    std::string register_name_r13(void);
    /**
     * \brief Generates string representing name of the rh850 r14 register
     * \return String representing the name of the rh850 r14 register
     */
    std::string register_name_r14(void);
    /**
     * \brief Generates string representing name of the rh850 r15 register
     * \return String representing the name of the rh850 r15 register
     */
    std::string register_name_r15(void);
    /**
     * \brief Generates string representing name of the rh850 r16 register
     * \return String representing the name of the rh850 r16 register
     */
    std::string register_name_r16(void);
    /**
     * \brief Generates string representing name of the rh850 r17 register
     * \return String representing the name of the rh850 r17 register
     */
    std::string register_name_r17(void);
    /**
     * \brief Generates string representing name of the rh850 r18 register
     * \return String representing the name of the rh850 r18 register
     */
    std::string register_name_r18(void);
    /**
     * \brief Generates string representing name of the rh850 r19 register
     * \return String representing the name of the rh850 r19 register
     */
    std::string register_name_r19(void);
    /**
     * \brief Generates string representing name of the rh850 r20 register
     * \return String representing the name of the rh850 r20 register
     */
    std::string register_name_r20(void);
    /**
     * \brief Generates string representing name of the rh850 r21 register
     * \return String representing the name of the rh850 r21 register
     */
    std::string register_name_r21(void);
    /**
     * \brief Generates string representing name of the rh850 r22 register
     * \return String representing the name of the rh850 r22 register
     */
    std::string register_name_r22(void);
    /**
     * \brief Generates string representing name of the rh850 r23 register
     * \return String representing the name of the rh850 r23 register
     */
    std::string register_name_r23(void);
    /**
     * \brief Generates string representing name of the rh850 r24 register
     * \return String representing the name of the rh850 r24 register
     */
    std::string register_name_r24(void);
    /**
     * \brief Generates string representing name of the rh850 r25 register
     * \return String representing the name of the rh850 r25 register
     */
    std::string register_name_r25(void);
    /**
     * \brief Generates string representing name of the rh850 r26 register
     * \return String representing the name of the rh850 r26 register
     */
    std::string register_name_r26(void);
    /**
     * \brief Generates string representing name of the rh850 r27 register
     * \return String representing the name of the rh850 r27 register
     */
    std::string register_name_r27(void);
    /**
     * \brief Generates string representing name of the rh850 r28 register
     * \return String representing the name of the rh850 r28 register
     */
    std::string register_name_r28(void);
    /**
     * \brief Generates string representing name of the rh850 r29 register
     * \return String representing the name of the rh850 r29 register
     */
    std::string register_name_r29(void);
    /**
     * \brief Generates string representing name of the rh850 r30 register
     * \return String representing the name of the rh850 r30 register
     */
    std::string register_name_r30(void);
    /**
     * \brief Generates string representing name of the rh850 r31 register
     * \return String representing the name of the rh850 r31 register
     */
    std::string register_name_r31(void);
    /**
     * \brief Generates string representing name of the rh850 PC register
     * \return String representing the name of the rh850 PC register
     */
    std::string register_name_PC(void);
    /**
     * \brief Generates string representing name of the rh850 EIPC register
     * \return String representing the name of the rh850 EIPC register
     */
    std::string register_name_EIPC(void);
    /**
     * \brief Generates string representing name of the rh850 EIPSW register
     * \return String representing the name of the rh850 EIPSW register
     */
    std::string register_name_EIPSW(void);
    /**
     * \brief Generates string representing name of the rh850 FEPC register
     * \return String representing the name of the rh850 FEPC register
     */
    std::string register_name_FEPC(void);
    /**
     * \brief Generates string representing name of the rh850 FEPSW register
     * \return String representing the name of the rh850 FEPSW register
     */
    std::string register_name_FEPSW(void);
    /**
     * \brief Generates string representing name of the rh850 PSW register
     * \return String representing the name of the rh850 PSW register
     */
    std::string register_name_PSW(void);
    /**
     * \brief Generates string representing name of the rh850 FPSR register
     * \return String representing the name of the rh850 FPSR register
     */
    std::string register_name_FPSR(void);
    /**
     * \brief Generates string representing name of the rh850 FPEPC register
     * \return String representing the name of the rh850 FPEPC register
     */
    std::string register_name_FPEPC(void);
    /**
     * \brief Generates string representing name of the rh850 FPST register
     * \return String representing the name of the rh850 FPST register
     */
    std::string register_name_FPST(void);
    /**
     * \brief Generates string representing name of the rh850 FPCC register
     * \return String representing the name of the rh850 FPCC register
     */
    std::string register_name_FPCC(void);
    /**
     * \brief Generates string representing name of the rh850 FPCFG register
     * \return String representing the name of the rh850 FPCFG register
     */
    std::string register_name_FPCFG(void);
    /**
     * \brief Generates string representing name of the rh850 FPEC register
     * \return String representing the name of the rh850 FPEC register
     */
    std::string register_name_FPEC(void);
    /**
     * \brief Generates string representing name of the rh850 EIIC register
     * \return String representing the name of the rh850 EIIC register
     */
    std::string register_name_EIIC(void);
    /**
     * \brief Generates string representing name of the rh850 FEIC register
     * \return String representing the name of the rh850 FEIC register
     */
    std::string register_name_FEIC(void);
    /**
     * \brief Generates string representing name of the rh850 CTPC register
     * \return String representing the name of the rh850 CTPC register
     */
    std::string register_name_CTPC(void);
    /**
     * \brief Generates string representing name of the rh850 CTPSW register
     * \return String representing the name of the rh850 CTPSW register
     */
    std::string register_name_CTPSW(void);
    /**
     * \brief Generates string representing name of the rh850 CTBP register
     * \return String representing the name of the rh850 CTBP register
     */
    std::string register_name_CTBP(void);
    /**
     * \brief Generates string representing name of the rh850 EIWR register
     * \return String representing the name of the rh850 EIWR register
     */
    std::string register_name_EIWR(void);
    /**
     * \brief Generates string representing name of the rh850 FEWR register
     * \return String representing the name of the rh850 FEWR register
     */
    std::string register_name_FEWR(void);
    /**
     * \brief Generates string representing name of the rh850 BSEL register
     * \return String representing the name of the rh850 BSEL register
     */
    std::string register_name_BSEL(void);
    /**
     * \brief Generates string representing name of the rh850 MCFG0 register
     * \return String representing the name of the rh850 MCFG0 register
     */
    std::string register_name_MCFG0(void);
    /**
     * \brief Generates string representing name of the rh850 RBASE register
     * \return String representing the name of the rh850 RBASE register
     */
    std::string register_name_RBASE(void);
    /**
     * \brief Generates string representing name of the rh850 EBASE register
     * \return String representing the name of the rh850 EBASE register
     */
    std::string register_name_EBASE(void);
    /**
     * \brief Generates string representing name of the rh850 INTBP register
     * \return String representing the name of the rh850 INTBP register
     */
    std::string register_name_INTBP(void);
    /**
     * \brief Generates string representing name of the rh850 MCTL register
     * \return String representing the name of the rh850 MCTL register
     */
    std::string register_name_MCTL(void);
    /**
     * \brief Generates string representing name of the rh850 PID register
     * \return String representing the name of the rh850 PID register
     */
    std::string register_name_PID(void);
    /**
     * \brief Generates string representing name of the rh850 FPIPR register
     * \return String representing the name of the rh850 FPIPR register
     */
    std::string register_name_FPIPR(void);
    /**
     * \brief Generates string representing name of the rh850 SCCFG register
     * \return String representing the name of the rh850 SCCFG register
     */
    std::string register_name_SCCFG(void);
    /**
     * \brief Generates string representing name of the rh850 SCBP register
     * \return String representing the name of the rh850 SCBP register
     */
    std::string register_name_SCBP(void);
    /**
     * \brief Generates string representing name of the rh850 HTCFG0 register
     * \return String representing the name of the rh850 HTCFG0 register
     */
    std::string register_name_HTCFG0(void);
    /**
     * \brief Generates string representing name of the rh850 HVCCFG register
     * \return String representing the name of the rh850 HVCCFG register
     */
    std::string register_name_HVCCFG(void);
    /**
     * \brief Generates string representing name of the rh850 HVCBP register
     * \return String representing the name of the rh850 HVCBP register
     */
    std::string register_name_HVCBP(void);
    /**
     * \brief Generates string representing name of the rh850 MEA register
     * \return String representing the name of the rh850 MEA register
     */
    std::string register_name_MEA(void);
    /**
     * \brief Generates string representing name of the rh850 ASID register
     * \return String representing the name of the rh850 ASID register
     */
    std::string register_name_ASID(void);
    /**
     * \brief Generates string representing name of the rh850 MEI register
     * \return String representing the name of the rh850 MEI register
     */
    std::string register_name_MEI(void);
    /**
     * \brief Generates string representing name of the rh850 ISPR register
     * \return String representing the name of the rh850 ISPR register
     */
    std::string register_name_ISPR(void);
    /**
     * \brief Generates string representing name of the rh850 PMR register
     * \return String representing the name of the rh850 PMR register
     */
    std::string register_name_PMR(void);
    /**
     * \brief Generates string representing name of the rh850 ICSR register
     * \return String representing the name of the rh850 ICSR register
     */
    std::string register_name_ICSR(void);
    /**
     * \brief Generates string representing name of the rh850 INTCFG register
     * \return String representing the name of the rh850 INTCFG register
     */
    std::string register_name_INTCFG(void);
    /**
     * \brief Generates string representing name of the rh850 DBIC register
     * \return String representing the name of the rh850 DBIC register
     */
    std::string register_name_DBIC(void);
    /**
     * \brief Generates string representing name of the rh850 DBCMC register
     * \return String representing the name of the rh850 DBCMC register
     */
    std::string register_name_DBCMC(void);
    /**
     * \brief Generates string representing name of the rh850 DBPC register
     * \return String representing the name of the rh850 DBPC register
     */
    std::string register_name_DBPC(void);
    /**
     * \brief Generates string representing name of the rh850 DBPSW register
     * \return String representing the name of the rh850 DBPSW register
     */
    std::string register_name_DBPSW(void);
    /**
     * \brief Generates string representing name of the rh850 DIR0 register
     * \return String representing the name of the rh850 DIR0 register
     */
    std::string register_name_DIR0(void);
    /**
     * \brief Generates string representing name of the rh850 DIR1 register
     * \return String representing the name of the rh850 DIR1 register
     */
    std::string register_name_DIR1(void);
    /**
     * \brief Generates string representing name of the rh850 BPC register
     * \return String representing the name of the rh850 BPC register
     */
    std::string register_name_BPC(void);
    /**
     * \brief Generates string representing name of the rh850 BPAV register
     * \return String representing the name of the rh850 BPAV register
     */
    std::string register_name_BPAV(void);
    /**
     * \brief Generates string representing name of the rh850 BPAM register
     * \return String representing the name of the rh850 BPAM register
     */
    std::string register_name_BPAM(void);
    /**
     * \brief Generates string representing name of the rh850 BPDV register
     * \return String representing the name of the rh850 BPDV register
     */
    std::string register_name_BPDV(void);
    /**
     * \brief Generates string representing name of the rh850 BPDM register
     * \return String representing the name of the rh850 BPDM register
     */
    std::string register_name_BPDM(void);
    /**
     * \brief Generates string representing name of the rh850 DBWR register
     * \return String representing the name of the rh850 DBWR register
     */
    std::string register_name_DBWR(void);
    /**
     * \brief Generates string representing name of the rh850 MPM register
     * \return String representing the name of the rh850 MPM register
     */
    std::string register_name_MPM(void);
    /**
     * \brief Generates string representing name of the rh850 MPRC register
     * \return String representing the name of the rh850 MPRC register
     */
    std::string register_name_MPRC(void);
    /**
     * \brief Generates string representing name of the rh850 MPBRGN register
     * \return String representing the name of the rh850 MPBRGN register
     */
    std::string register_name_MPBRGN(void);
    /**
     * \brief Generates string representing name of the rh850 MPTRGN register
     * \return String representing the name of the rh850 MPTRGN register
     */
    std::string register_name_MPTRGN(void);
    /**
     * \brief Generates string representing name of the rh850 MCR register
     * \return String representing the name of the rh850 MCR register
     */
    std::string register_name_MCR(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA0 register
     * \return String representing the name of the rh850 MPLA0 register
     */
    std::string register_name_MPLA0(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA0 register
     * \return String representing the name of the rh850 MPUA0 register
     */
    std::string register_name_MPUA0(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT0 register
     * \return String representing the name of the rh850 MPAT0 register
     */
    std::string register_name_MPAT0(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA1 register
     * \return String representing the name of the rh850 MPLA1 register
     */
    std::string register_name_MPLA1(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA1 register
     * \return String representing the name of the rh850 MPUA1 register
     */
    std::string register_name_MPUA1(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT1 register
     * \return String representing the name of the rh850 MPAT1 register
     */
    std::string register_name_MPAT1(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA2 register
     * \return String representing the name of the rh850 MPLA2 register
     */
    std::string register_name_MPLA2(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA2 register
     * \return String representing the name of the rh850 MPUA2 register
     */
    std::string register_name_MPUA2(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT2 register
     * \return String representing the name of the rh850 MPAT2 register
     */
    std::string register_name_MPAT2(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA3 register
     * \return String representing the name of the rh850 MPLA3 register
     */
    std::string register_name_MPLA3(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA3 register
     * \return String representing the name of the rh850 MPUA3 register
     */
    std::string register_name_MPUA3(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT3 register
     * \return String representing the name of the rh850 MPAT3 register
     */
    std::string register_name_MPAT3(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA4 register
     * \return String representing the name of the rh850 MPLA4 register
     */
    std::string register_name_MPLA4(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA4 register
     * \return String representing the name of the rh850 MPUA4 register
     */
    std::string register_name_MPUA4(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT4 register
     * \return String representing the name of the rh850 MPAT4 register
     */
    std::string register_name_MPAT4(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA5 register
     * \return String representing the name of the rh850 MPLA5 register
     */
    std::string register_name_MPLA5(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA5 register
     * \return String representing the name of the rh850 MPUA5 register
     */
    std::string register_name_MPUA5(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT5 register
     * \return String representing the name of the rh850 MPAT5 register
     */
    std::string register_name_MPAT5(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA6 register
     * \return String representing the name of the rh850 MPLA6 register
     */
    std::string register_name_MPLA6(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA6 register
     * \return String representing the name of the rh850 MPUA6 register
     */
    std::string register_name_MPUA6(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT6 register
     * \return String representing the name of the rh850 MPAT6 register
     */
    std::string register_name_MPAT6(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA7 register
     * \return String representing the name of the rh850 MPLA7 register
     */
    std::string register_name_MPLA7(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA7 register
     * \return String representing the name of the rh850 MPUA7 register
     */
    std::string register_name_MPUA7(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT7 register
     * \return String representing the name of the rh850 MPAT7 register
     */
    std::string register_name_MPAT7(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA8 register
     * \return String representing the name of the rh850 MPLA8 register
     */
    std::string register_name_MPLA8(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA8 register
     * \return String representing the name of the rh850 MPUA8 register
     */
    std::string register_name_MPUA8(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT8 register
     * \return String representing the name of the rh850 MPAT8 register
     */
    std::string register_name_MPAT8(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA9 register
     * \return String representing the name of the rh850 MPLA9 register
     */
    std::string register_name_MPLA9(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA9 register
     * \return String representing the name of the rh850 MPUA9 register
     */
    std::string register_name_MPUA9(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT9 register
     * \return String representing the name of the rh850 MPAT9 register
     */
    std::string register_name_MPAT9(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA10 register
     * \return String representing the name of the rh850 MPLA10 register
     */
    std::string register_name_MPLA10(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA10 register
     * \return String representing the name of the rh850 MPUA10 register
     */
    std::string register_name_MPUA10(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT10 register
     * \return String representing the name of the rh850 MPAT10 register
     */
    std::string register_name_MPAT10(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA11 register
     * \return String representing the name of the rh850 MPLA11 register
     */
    std::string register_name_MPLA11(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA11 register
     * \return String representing the name of the rh850 MPUA11 register
     */
    std::string register_name_MPUA11(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT11 register
     * \return String representing the name of the rh850 MPAT11 register
     */
    std::string register_name_MPAT11(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA12 register
     * \return String representing the name of the rh850 MPLA12 register
     */
    std::string register_name_MPLA12(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA12 register
     * \return String representing the name of the rh850 MPUA12 register
     */
    std::string register_name_MPUA12(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT12 register
     * \return String representing the name of the rh850 MPAT12 register
     */
    std::string register_name_MPAT12(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA13 register
     * \return String representing the name of the rh850 MPLA13 register
     */
    std::string register_name_MPLA13(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA13 register
     * \return String representing the name of the rh850 MPUA13 register
     */
    std::string register_name_MPUA13(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT13 register
     * \return String representing the name of the rh850 MPAT13 register
     */
    std::string register_name_MPAT13(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA14 register
     * \return String representing the name of the rh850 MPLA14 register
     */
    std::string register_name_MPLA14(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA14 register
     * \return String representing the name of the rh850 MPUA14 register
     */
    std::string register_name_MPUA14(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT14 register
     * \return String representing the name of the rh850 MPAT14 register
     */
    std::string register_name_MPAT14(void);
    /**
     * \brief Generates string representing name of the rh850 MPLA15 register
     * \return String representing the name of the rh850 MPLA15 register
     */
    std::string register_name_MPLA15(void);
    /**
     * \brief Generates string representing name of the rh850 MPUA15 register
     * \return String representing the name of the rh850 MPUA15 register
     */
    std::string register_name_MPUA15(void);
    /**
     * \brief Generates string representing name of the rh850 MPAT15 register
     * \return String representing the name of the rh850 MPAT15 register
     */
    std::string register_name_MPAT15(void);
    /**
     * \brief Generates string representing name of the rh850 mpu_entry register
     * \return String representing the name of the rh850 mpu_entry register
     */
    std::string register_name_mpu_entry(void);
    /**
     * \brief Generates string representing name of the rh850 mpat_g_support register
     * \return String representing the name of the rh850 mpat_g_support register
     */
    std::string register_name_mpat_g_support(void);
    /**
     * \brief Generates string representing name of the rh850 swd0pcad_exec_count register
     * \return String representing the name of the rh850 swd0pcad_exec_count register
     */
    std::string register_name_swd0pcad_exec_count(void);
    /**
     * \brief Generates string representing name of the rh850 swd0pcad_val register
     * \return String representing the name of the rh850 swd0pcad_val register
     */
    std::string register_name_swd0pcad_val(void);
    /**
     * \brief Generates string representing name of the rh850 mem_acc_type register
     * \return String representing the name of the rh850 mem_acc_type register
     */
    std::string register_name_mem_acc_type(void);
    
    // Register group name operations
    /**
     * \brief Generates string representing name of the specified register from the rh850 GR register group
     * \param index Index value of the required register from the rh850 GR register group
     * \return String representing the name of the specified rh850 GR group register
     */
    std::string register_name_GR(rh850::GR_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \return String representing the name of the specified rh850 SR_SELID_0 group register
     */
    std::string register_name_SR_SELID_0(rh850::SR_SELID_0_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \return String representing the name of the specified rh850 SR_SELID_1 group register
     */
    std::string register_name_SR_SELID_1(rh850::SR_SELID_1_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \return String representing the name of the specified rh850 SR_SELID_2 group register
     */
    std::string register_name_SR_SELID_2(rh850::SR_SELID_2_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \return String representing the name of the specified rh850 SR_SELID_3 group register
     */
    std::string register_name_SR_SELID_3(rh850::SR_SELID_3_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \return String representing the name of the specified rh850 SR_SELID_5 group register
     */
    std::string register_name_SR_SELID_5(rh850::SR_SELID_5_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850 SR_SELID_6 register group
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \return String representing the name of the specified rh850 SR_SELID_6 group register
     */
    std::string register_name_SR_SELID_6(rh850::SR_SELID_6_index_t index);
    /**
     * \brief Generates string representing name of the specified register from the rh850 SR_SELID_7 register group
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \return String representing the name of the specified rh850 SR_SELID_7 group register
     */
    std::string register_name_SR_SELID_7(rh850::SR_SELID_7_index_t index);
    
    // Immediate register and register field read operations
    /**
     * \brief Queries the value of the rh850 translation time PC register
     * \return Value of the rh850 translation time PC register
     */
    unsigned int read_trans_PC(void);
    /**
     * \brief Queries the value of the rh850 r0 register
     * \return Value of the rh850 r0 register
     */
    unsigned int read_r0(void);
    /**
     * \brief Queries the value of the rh850 r1 register
     * \return Value of the rh850 r1 register
     */
    unsigned int read_r1(void);
    /**
     * \brief Queries the value of the rh850 r2 register
     * \return Value of the rh850 r2 register
     */
    unsigned int read_r2(void);
    /**
     * \brief Queries the value of the rh850 r3 register
     * \return Value of the rh850 r3 register
     */
    unsigned int read_r3(void);
    /**
     * \brief Queries the value of the rh850 r4 register
     * \return Value of the rh850 r4 register
     */
    unsigned int read_r4(void);
    /**
     * \brief Queries the value of the rh850 r5 register
     * \return Value of the rh850 r5 register
     */
    unsigned int read_r5(void);
    /**
     * \brief Queries the value of the rh850 r6 register
     * \return Value of the rh850 r6 register
     */
    unsigned int read_r6(void);
    /**
     * \brief Queries the value of the rh850 r7 register
     * \return Value of the rh850 r7 register
     */
    unsigned int read_r7(void);
    /**
     * \brief Queries the value of the rh850 r8 register
     * \return Value of the rh850 r8 register
     */
    unsigned int read_r8(void);
    /**
     * \brief Queries the value of the rh850 r9 register
     * \return Value of the rh850 r9 register
     */
    unsigned int read_r9(void);
    /**
     * \brief Queries the value of the rh850 r10 register
     * \return Value of the rh850 r10 register
     */
    unsigned int read_r10(void);
    /**
     * \brief Queries the value of the rh850 r11 register
     * \return Value of the rh850 r11 register
     */
    unsigned int read_r11(void);
    /**
     * \brief Queries the value of the rh850 r12 register
     * \return Value of the rh850 r12 register
     */
    unsigned int read_r12(void);
    /**
     * \brief Queries the value of the rh850 r13 register
     * \return Value of the rh850 r13 register
     */
    unsigned int read_r13(void);
    /**
     * \brief Queries the value of the rh850 r14 register
     * \return Value of the rh850 r14 register
     */
    unsigned int read_r14(void);
    /**
     * \brief Queries the value of the rh850 r15 register
     * \return Value of the rh850 r15 register
     */
    unsigned int read_r15(void);
    /**
     * \brief Queries the value of the rh850 r16 register
     * \return Value of the rh850 r16 register
     */
    unsigned int read_r16(void);
    /**
     * \brief Queries the value of the rh850 r17 register
     * \return Value of the rh850 r17 register
     */
    unsigned int read_r17(void);
    /**
     * \brief Queries the value of the rh850 r18 register
     * \return Value of the rh850 r18 register
     */
    unsigned int read_r18(void);
    /**
     * \brief Queries the value of the rh850 r19 register
     * \return Value of the rh850 r19 register
     */
    unsigned int read_r19(void);
    /**
     * \brief Queries the value of the rh850 r20 register
     * \return Value of the rh850 r20 register
     */
    unsigned int read_r20(void);
    /**
     * \brief Queries the value of the rh850 r21 register
     * \return Value of the rh850 r21 register
     */
    unsigned int read_r21(void);
    /**
     * \brief Queries the value of the rh850 r22 register
     * \return Value of the rh850 r22 register
     */
    unsigned int read_r22(void);
    /**
     * \brief Queries the value of the rh850 r23 register
     * \return Value of the rh850 r23 register
     */
    unsigned int read_r23(void);
    /**
     * \brief Queries the value of the rh850 r24 register
     * \return Value of the rh850 r24 register
     */
    unsigned int read_r24(void);
    /**
     * \brief Queries the value of the rh850 r25 register
     * \return Value of the rh850 r25 register
     */
    unsigned int read_r25(void);
    /**
     * \brief Queries the value of the rh850 r26 register
     * \return Value of the rh850 r26 register
     */
    unsigned int read_r26(void);
    /**
     * \brief Queries the value of the rh850 r27 register
     * \return Value of the rh850 r27 register
     */
    unsigned int read_r27(void);
    /**
     * \brief Queries the value of the rh850 r28 register
     * \return Value of the rh850 r28 register
     */
    unsigned int read_r28(void);
    /**
     * \brief Queries the value of the rh850 r29 register
     * \return Value of the rh850 r29 register
     */
    unsigned int read_r29(void);
    /**
     * \brief Queries the value of the rh850 r30 register
     * \return Value of the rh850 r30 register
     */
    unsigned int read_r30(void);
    /**
     * \brief Queries the value of the rh850 r31 register
     * \return Value of the rh850 r31 register
     */
    unsigned int read_r31(void);
    /**
     * \brief Queries the value of the rh850 PC register
     * \return Value of the rh850 PC register
     */
    unsigned int read_PC(void);
    /**
     * \brief Queries the value of the rh850 EIPC register
     * \return Value of the rh850 EIPC register
     */
    unsigned int read_EIPC(void);
    /**
     * \brief Queries the value of the rh850 EIPSW register
     * \return Value of the rh850 EIPSW register
     */
    unsigned int read_EIPSW(void);
    /**
     * \brief Queries the value of the rh850 FEPC register
     * \return Value of the rh850 FEPC register
     */
    unsigned int read_FEPC(void);
    /**
     * \brief Queries the value of the rh850 FEPSW register
     * \return Value of the rh850 FEPSW register
     */
    unsigned int read_FEPSW(void);
    /**
     * \brief Queries the value of the rh850 PSW register
     * \return Value of the rh850 PSW register
     */
    unsigned int read_PSW(void);
    /**
     * \brief Queries the value of the rh850 PSW_Z register field
     * \return Value of the rh850 PSW_Z register field
     */
    unsigned char read_PSW_Z(void);
    /**
     * \brief Queries the value of the rh850 PSW_S register field
     * \return Value of the rh850 PSW_S register field
     */
    unsigned char read_PSW_S(void);
    /**
     * \brief Queries the value of the rh850 PSW_OV register field
     * \return Value of the rh850 PSW_OV register field
     */
    unsigned char read_PSW_OV(void);
    /**
     * \brief Queries the value of the rh850 PSW_CY register field
     * \return Value of the rh850 PSW_CY register field
     */
    unsigned char read_PSW_CY(void);
    /**
     * \brief Queries the value of the rh850 PSW_SAT register field
     * \return Value of the rh850 PSW_SAT register field
     */
    unsigned char read_PSW_SAT(void);
    /**
     * \brief Queries the value of the rh850 PSW_ID register field
     * \return Value of the rh850 PSW_ID register field
     */
    unsigned char read_PSW_ID(void);
    /**
     * \brief Queries the value of the rh850 PSW_EP register field
     * \return Value of the rh850 PSW_EP register field
     */
    unsigned char read_PSW_EP(void);
    /**
     * \brief Queries the value of the rh850 PSW_NP register field
     * \return Value of the rh850 PSW_NP register field
     */
    unsigned char read_PSW_NP(void);
    /**
     * \brief Queries the value of the rh850 PSW_CM register field
     * \return Value of the rh850 PSW_CM register field
     */
    unsigned char read_PSW_CM(void);
    /**
     * \brief Queries the value of the rh850 PSW_SS register field
     * \return Value of the rh850 PSW_SS register field
     */
    unsigned char read_PSW_SS(void);
    /**
     * \brief Queries the value of the rh850 PSW_EBV register field
     * \return Value of the rh850 PSW_EBV register field
     */
    unsigned char read_PSW_EBV(void);
    /**
     * \brief Queries the value of the rh850 PSW_CU0 register field
     * \return Value of the rh850 PSW_CU0 register field
     */
    unsigned char read_PSW_CU0(void);
    /**
     * \brief Queries the value of the rh850 PSW_CU1 register field
     * \return Value of the rh850 PSW_CU1 register field
     */
    unsigned char read_PSW_CU1(void);
    /**
     * \brief Queries the value of the rh850 PSW_CU2 register field
     * \return Value of the rh850 PSW_CU2 register field
     */
    unsigned char read_PSW_CU2(void);
    /**
     * \brief Queries the value of the rh850 PSW_HVC register field
     * \return Value of the rh850 PSW_HVC register field
     */
    unsigned char read_PSW_HVC(void);
    /**
     * \brief Queries the value of the rh850 PSW_UM register field
     * \return Value of the rh850 PSW_UM register field
     */
    unsigned char read_PSW_UM(void);
    /**
     * \brief Queries the value of the rh850 FPSR register
     * \return Value of the rh850 FPSR register
     */
    unsigned int read_FPSR(void);
    /**
     * \brief Queries the value of the rh850 FPSR_CC register field
     * \return Value of the rh850 FPSR_CC register field
     */
    unsigned char read_FPSR_CC(void);
    /**
     * \brief Queries the value of the rh850 FPSR_FN register field
     * \return Value of the rh850 FPSR_FN register field
     */
    unsigned char read_FPSR_FN(void);
    /**
     * \brief Queries the value of the rh850 FPSR_IF register field
     * \return Value of the rh850 FPSR_IF register field
     */
    unsigned char read_FPSR_IF(void);
    /**
     * \brief Queries the value of the rh850 FPSR_PEM register field
     * \return Value of the rh850 FPSR_PEM register field
     */
    unsigned char read_FPSR_PEM(void);
    /**
     * \brief Queries the value of the rh850 FPSR_RM register field
     * \return Value of the rh850 FPSR_RM register field
     */
    unsigned char read_FPSR_RM(void);
    /**
     * \brief Queries the value of the rh850 FPSR_FS register field
     * \return Value of the rh850 FPSR_FS register field
     */
    unsigned char read_FPSR_FS(void);
    /**
     * \brief Queries the value of the rh850 FPSR_XC register field
     * \return Value of the rh850 FPSR_XC register field
     */
    unsigned char read_FPSR_XC(void);
    /**
     * \brief Queries the value of the rh850 FPSR_XE register field
     * \return Value of the rh850 FPSR_XE register field
     */
    unsigned char read_FPSR_XE(void);
    /**
     * \brief Queries the value of the rh850 FPSR_XP register field
     * \return Value of the rh850 FPSR_XP register field
     */
    unsigned char read_FPSR_XP(void);
    /**
     * \brief Queries the value of the rh850 FPEPC register
     * \return Value of the rh850 FPEPC register
     */
    unsigned int read_FPEPC(void);
    /**
     * \brief Queries the value of the rh850 FPST register
     * \return Value of the rh850 FPST register
     */
    unsigned int read_FPST(void);
    /**
     * \brief Queries the value of the rh850 FPST_XC register field
     * \return Value of the rh850 FPST_XC register field
     */
    unsigned char read_FPST_XC(void);
    /**
     * \brief Queries the value of the rh850 FPST_IF register field
     * \return Value of the rh850 FPST_IF register field
     */
    unsigned char read_FPST_IF(void);
    /**
     * \brief Queries the value of the rh850 FPST_XP register field
     * \return Value of the rh850 FPST_XP register field
     */
    unsigned char read_FPST_XP(void);
    /**
     * \brief Queries the value of the rh850 FPCC register
     * \return Value of the rh850 FPCC register
     */
    unsigned int read_FPCC(void);
    /**
     * \brief Queries the value of the rh850 FPCC_CC register field
     * \return Value of the rh850 FPCC_CC register field
     */
    unsigned char read_FPCC_CC(void);
    /**
     * \brief Queries the value of the rh850 FPCFG register
     * \return Value of the rh850 FPCFG register
     */
    unsigned int read_FPCFG(void);
    /**
     * \brief Queries the value of the rh850 FPCFG_RM register field
     * \return Value of the rh850 FPCFG_RM register field
     */
    unsigned char read_FPCFG_RM(void);
    /**
     * \brief Queries the value of the rh850 FPCFG_XE register field
     * \return Value of the rh850 FPCFG_XE register field
     */
    unsigned char read_FPCFG_XE(void);
    /**
     * \brief Queries the value of the rh850 FPEC register
     * \return Value of the rh850 FPEC register
     */
    unsigned int read_FPEC(void);
    /**
     * \brief Queries the value of the rh850 EIIC register
     * \return Value of the rh850 EIIC register
     */
    unsigned int read_EIIC(void);
    /**
     * \brief Queries the value of the rh850 FEIC register
     * \return Value of the rh850 FEIC register
     */
    unsigned int read_FEIC(void);
    /**
     * \brief Queries the value of the rh850 CTPC register
     * \return Value of the rh850 CTPC register
     */
    unsigned int read_CTPC(void);
    /**
     * \brief Queries the value of the rh850 CTPSW register
     * \return Value of the rh850 CTPSW register
     */
    unsigned int read_CTPSW(void);
    /**
     * \brief Queries the value of the rh850 CTBP register
     * \return Value of the rh850 CTBP register
     */
    unsigned int read_CTBP(void);
    /**
     * \brief Queries the value of the rh850 EIWR register
     * \return Value of the rh850 EIWR register
     */
    unsigned int read_EIWR(void);
    /**
     * \brief Queries the value of the rh850 FEWR register
     * \return Value of the rh850 FEWR register
     */
    unsigned int read_FEWR(void);
    /**
     * \brief Queries the value of the rh850 BSEL register
     * \return Value of the rh850 BSEL register
     */
    unsigned int read_BSEL(void);
    /**
     * \brief Queries the value of the rh850 MCFG0 register
     * \return Value of the rh850 MCFG0 register
     */
    unsigned int read_MCFG0(void);
    /**
     * \brief Queries the value of the rh850 MCFG0_SPID register field
     * \return Value of the rh850 MCFG0_SPID register field
     */
    unsigned char read_MCFG0_SPID(void);
    /**
     * \brief Queries the value of the rh850 MCFG0_HVP register field
     * \return Value of the rh850 MCFG0_HVP register field
     */
    unsigned char read_MCFG0_HVP(void);
    /**
     * \brief Queries the value of the rh850 RBASE register
     * \return Value of the rh850 RBASE register
     */
    unsigned int read_RBASE(void);
    /**
     * \brief Queries the value of the rh850 RBASE_RBASE register field
     * \return Value of the rh850 RBASE_RBASE register field
     */
    unsigned int read_RBASE_RBASE(void);
    /**
     * \brief Queries the value of the rh850 RBASE_RINT register field
     * \return Value of the rh850 RBASE_RINT register field
     */
    unsigned char read_RBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850 EBASE register
     * \return Value of the rh850 EBASE register
     */
    unsigned int read_EBASE(void);
    /**
     * \brief Queries the value of the rh850 EBASE_EBASE register field
     * \return Value of the rh850 EBASE_EBASE register field
     */
    unsigned int read_EBASE_EBASE(void);
    /**
     * \brief Queries the value of the rh850 EBASE_RINT register field
     * \return Value of the rh850 EBASE_RINT register field
     */
    unsigned char read_EBASE_RINT(void);
    /**
     * \brief Queries the value of the rh850 INTBP register
     * \return Value of the rh850 INTBP register
     */
    unsigned int read_INTBP(void);
    /**
     * \brief Queries the value of the rh850 MCTL register
     * \return Value of the rh850 MCTL register
     */
    unsigned int read_MCTL(void);
    /**
     * \brief Queries the value of the rh850 MCTL_EN register field
     * \return Value of the rh850 MCTL_EN register field
     */
    unsigned char read_MCTL_EN(void);
    /**
     * \brief Queries the value of the rh850 MCTL_MT register field
     * \return Value of the rh850 MCTL_MT register field
     */
    unsigned char read_MCTL_MT(void);
    /**
     * \brief Queries the value of the rh850 MCTL_STID register field
     * \return Value of the rh850 MCTL_STID register field
     */
    unsigned char read_MCTL_STID(void);
    /**
     * \brief Queries the value of the rh850 MCTL_MA register field
     * \return Value of the rh850 MCTL_MA register field
     */
    unsigned char read_MCTL_MA(void);
    /**
     * \brief Queries the value of the rh850 MCTL_UIC register field
     * \return Value of the rh850 MCTL_UIC register field
     */
    unsigned char read_MCTL_UIC(void);
    /**
     * \brief Queries the value of the rh850 PID register
     * \return Value of the rh850 PID register
     */
    unsigned int read_PID(void);
    /**
     * \brief Queries the value of the rh850 FPIPR register
     * \return Value of the rh850 FPIPR register
     */
    unsigned int read_FPIPR(void);
    /**
     * \brief Queries the value of the rh850 FPIPR_FPIPR register field
     * \return Value of the rh850 FPIPR_FPIPR register field
     */
    unsigned char read_FPIPR_FPIPR(void);
    /**
     * \brief Queries the value of the rh850 SCCFG register
     * \return Value of the rh850 SCCFG register
     */
    unsigned int read_SCCFG(void);
    /**
     * \brief Queries the value of the rh850 SCCFG_SIZE register field
     * \return Value of the rh850 SCCFG_SIZE register field
     */
    unsigned char read_SCCFG_SIZE(void);
    /**
     * \brief Queries the value of the rh850 SCBP register
     * \return Value of the rh850 SCBP register
     */
    unsigned int read_SCBP(void);
    /**
     * \brief Queries the value of the rh850 HTCFG0 register
     * \return Value of the rh850 HTCFG0 register
     */
    unsigned int read_HTCFG0(void);
    /**
     * \brief Queries the value of the rh850 HTCFG0_PEID register field
     * \return Value of the rh850 HTCFG0_PEID register field
     */
    unsigned char read_HTCFG0_PEID(void);
    /**
     * \brief Queries the value of the rh850 HTCFG0_NC register field
     * \return Value of the rh850 HTCFG0_NC register field
     */
    unsigned char read_HTCFG0_NC(void);
    /**
     * \brief Queries the value of the rh850 HTCFG0_TCID register field
     * \return Value of the rh850 HTCFG0_TCID register field
     */
    unsigned char read_HTCFG0_TCID(void);
    /**
     * \brief Queries the value of the rh850 HVCCFG register
     * \return Value of the rh850 HVCCFG register
     */
    unsigned int read_HVCCFG(void);
    /**
     * \brief Queries the value of the rh850 HVCBP register
     * \return Value of the rh850 HVCBP register
     */
    unsigned int read_HVCBP(void);
    /**
     * \brief Queries the value of the rh850 MEA register
     * \return Value of the rh850 MEA register
     */
    unsigned int read_MEA(void);
    /**
     * \brief Queries the value of the rh850 ASID register
     * \return Value of the rh850 ASID register
     */
    unsigned int read_ASID(void);
    /**
     * \brief Queries the value of the rh850 ASID_ASID register field
     * \return Value of the rh850 ASID_ASID register field
     */
    unsigned short read_ASID_ASID(void);
    /**
     * \brief Queries the value of the rh850 MEI register
     * \return Value of the rh850 MEI register
     */
    unsigned int read_MEI(void);
    /**
     * \brief Queries the value of the rh850 MEI_REG register field
     * \return Value of the rh850 MEI_REG register field
     */
    unsigned char read_MEI_REG(void);
    /**
     * \brief Queries the value of the rh850 MEI_DS register field
     * \return Value of the rh850 MEI_DS register field
     */
    unsigned char read_MEI_DS(void);
    /**
     * \brief Queries the value of the rh850 MEI_U register field
     * \return Value of the rh850 MEI_U register field
     */
    unsigned char read_MEI_U(void);
    /**
     * \brief Queries the value of the rh850 MEI_ITYPE register field
     * \return Value of the rh850 MEI_ITYPE register field
     */
    unsigned char read_MEI_ITYPE(void);
    /**
     * \brief Queries the value of the rh850 MEI_RW register field
     * \return Value of the rh850 MEI_RW register field
     */
    unsigned char read_MEI_RW(void);
    /**
     * \brief Queries the value of the rh850 ISPR register
     * \return Value of the rh850 ISPR register
     */
    unsigned int read_ISPR(void);
    /**
     * \brief Queries the value of the rh850 ISPR_ISP register field
     * \return Value of the rh850 ISPR_ISP register field
     */
    unsigned char read_ISPR_ISP(void);
    /**
     * \brief Queries the value of the rh850 PMR register
     * \return Value of the rh850 PMR register
     */
    unsigned int read_PMR(void);
    /**
     * \brief Queries the value of the rh850 PMR_PM register field
     * \return Value of the rh850 PMR_PM register field
     */
    unsigned char read_PMR_PM(void);
    /**
     * \brief Queries the value of the rh850 ICSR register
     * \return Value of the rh850 ICSR register
     */
    unsigned int read_ICSR(void);
    /**
     * \brief Queries the value of the rh850 ICSR_PMEI register field
     * \return Value of the rh850 ICSR_PMEI register field
     */
    unsigned char read_ICSR_PMEI(void);
    /**
     * \brief Queries the value of the rh850 INTCFG register
     * \return Value of the rh850 INTCFG register
     */
    unsigned int read_INTCFG(void);
    /**
     * \brief Queries the value of the rh850 INTCFG_ISPC register field
     * \return Value of the rh850 INTCFG_ISPC register field
     */
    unsigned char read_INTCFG_ISPC(void);
    /**
     * \brief Queries the value of the rh850 DBIC register
     * \return Value of the rh850 DBIC register
     */
    unsigned int read_DBIC(void);
    /**
     * \brief Queries the value of the rh850 DBCMC register
     * \return Value of the rh850 DBCMC register
     */
    unsigned int read_DBCMC(void);
    /**
     * \brief Queries the value of the rh850 DBPC register
     * \return Value of the rh850 DBPC register
     */
    unsigned int read_DBPC(void);
    /**
     * \brief Queries the value of the rh850 DBPSW register
     * \return Value of the rh850 DBPSW register
     */
    unsigned int read_DBPSW(void);
    /**
     * \brief Queries the value of the rh850 DIR0 register
     * \return Value of the rh850 DIR0 register
     */
    unsigned int read_DIR0(void);
    /**
     * \brief Queries the value of the rh850 DIR1 register
     * \return Value of the rh850 DIR1 register
     */
    unsigned int read_DIR1(void);
    /**
     * \brief Queries the value of the rh850 BPC register
     * \return Value of the rh850 BPC register
     */
    unsigned int read_BPC(void);
    /**
     * \brief Queries the value of the rh850 BPAV register
     * \return Value of the rh850 BPAV register
     */
    unsigned int read_BPAV(void);
    /**
     * \brief Queries the value of the rh850 BPAM register
     * \return Value of the rh850 BPAM register
     */
    unsigned int read_BPAM(void);
    /**
     * \brief Queries the value of the rh850 BPDV register
     * \return Value of the rh850 BPDV register
     */
    unsigned int read_BPDV(void);
    /**
     * \brief Queries the value of the rh850 BPDM register
     * \return Value of the rh850 BPDM register
     */
    unsigned int read_BPDM(void);
    /**
     * \brief Queries the value of the rh850 DBWR register
     * \return Value of the rh850 DBWR register
     */
    unsigned int read_DBWR(void);
    /**
     * \brief Queries the value of the rh850 MPM register
     * \return Value of the rh850 MPM register
     */
    unsigned int read_MPM(void);
    /**
     * \brief Queries the value of the rh850 MPM_DX register field
     * \return Value of the rh850 MPM_DX register field
     */
    unsigned char read_MPM_DX(void);
    /**
     * \brief Queries the value of the rh850 MPM_DW register field
     * \return Value of the rh850 MPM_DW register field
     */
    unsigned char read_MPM_DW(void);
    /**
     * \brief Queries the value of the rh850 MPM_DR register field
     * \return Value of the rh850 MPM_DR register field
     */
    unsigned char read_MPM_DR(void);
    /**
     * \brief Queries the value of the rh850 MPM_SVP register field
     * \return Value of the rh850 MPM_SVP register field
     */
    unsigned char read_MPM_SVP(void);
    /**
     * \brief Queries the value of the rh850 MPM_MPE register field
     * \return Value of the rh850 MPM_MPE register field
     */
    unsigned char read_MPM_MPE(void);
    /**
     * \brief Queries the value of the rh850 MPRC register
     * \return Value of the rh850 MPRC register
     */
    unsigned int read_MPRC(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E15 register field
     * \return Value of the rh850 MPRC_E15 register field
     */
    unsigned char read_MPRC_E15(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E14 register field
     * \return Value of the rh850 MPRC_E14 register field
     */
    unsigned char read_MPRC_E14(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E13 register field
     * \return Value of the rh850 MPRC_E13 register field
     */
    unsigned char read_MPRC_E13(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E12 register field
     * \return Value of the rh850 MPRC_E12 register field
     */
    unsigned char read_MPRC_E12(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E11 register field
     * \return Value of the rh850 MPRC_E11 register field
     */
    unsigned char read_MPRC_E11(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E10 register field
     * \return Value of the rh850 MPRC_E10 register field
     */
    unsigned char read_MPRC_E10(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E9 register field
     * \return Value of the rh850 MPRC_E9 register field
     */
    unsigned char read_MPRC_E9(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E8 register field
     * \return Value of the rh850 MPRC_E8 register field
     */
    unsigned char read_MPRC_E8(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E7 register field
     * \return Value of the rh850 MPRC_E7 register field
     */
    unsigned char read_MPRC_E7(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E6 register field
     * \return Value of the rh850 MPRC_E6 register field
     */
    unsigned char read_MPRC_E6(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E5 register field
     * \return Value of the rh850 MPRC_E5 register field
     */
    unsigned char read_MPRC_E5(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E4 register field
     * \return Value of the rh850 MPRC_E4 register field
     */
    unsigned char read_MPRC_E4(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E3 register field
     * \return Value of the rh850 MPRC_E3 register field
     */
    unsigned char read_MPRC_E3(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E2 register field
     * \return Value of the rh850 MPRC_E2 register field
     */
    unsigned char read_MPRC_E2(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E1 register field
     * \return Value of the rh850 MPRC_E1 register field
     */
    unsigned char read_MPRC_E1(void);
    /**
     * \brief Queries the value of the rh850 MPRC_E0 register field
     * \return Value of the rh850 MPRC_E0 register field
     */
    unsigned char read_MPRC_E0(void);
    /**
     * \brief Queries the value of the rh850 MPBRGN register
     * \return Value of the rh850 MPBRGN register
     */
    unsigned int read_MPBRGN(void);
    /**
     * \brief Queries the value of the rh850 MPBRGN_MPBRGN register field
     * \return Value of the rh850 MPBRGN_MPBRGN register field
     */
    unsigned char read_MPBRGN_MPBRGN(void);
    /**
     * \brief Queries the value of the rh850 MPTRGN register
     * \return Value of the rh850 MPTRGN register
     */
    unsigned int read_MPTRGN(void);
    /**
     * \brief Queries the value of the rh850 MPTRGN_MPTRGN register field
     * \return Value of the rh850 MPTRGN_MPTRGN register field
     */
    unsigned char read_MPTRGN_MPTRGN(void);
    /**
     * \brief Queries the value of the rh850 MCR register
     * \return Value of the rh850 MCR register
     */
    unsigned int read_MCR(void);
    /**
     * \brief Queries the value of the rh850 MCR_SXE register field
     * \return Value of the rh850 MCR_SXE register field
     */
    unsigned char read_MCR_SXE(void);
    /**
     * \brief Queries the value of the rh850 MCR_SWE register field
     * \return Value of the rh850 MCR_SWE register field
     */
    unsigned char read_MCR_SWE(void);
    /**
     * \brief Queries the value of the rh850 MCR_SRE register field
     * \return Value of the rh850 MCR_SRE register field
     */
    unsigned char read_MCR_SRE(void);
    /**
     * \brief Queries the value of the rh850 MCR_UXE register field
     * \return Value of the rh850 MCR_UXE register field
     */
    unsigned char read_MCR_UXE(void);
    /**
     * \brief Queries the value of the rh850 MCR_UWE register field
     * \return Value of the rh850 MCR_UWE register field
     */
    unsigned char read_MCR_UWE(void);
    /**
     * \brief Queries the value of the rh850 MCR_URE register field
     * \return Value of the rh850 MCR_URE register field
     */
    unsigned char read_MCR_URE(void);
    /**
     * \brief Queries the value of the rh850 MPLA0 register
     * \return Value of the rh850 MPLA0 register
     */
    unsigned int read_MPLA0(void);
    /**
     * \brief Queries the value of the rh850 MPUA0 register
     * \return Value of the rh850 MPUA0 register
     */
    unsigned int read_MPUA0(void);
    /**
     * \brief Queries the value of the rh850 MPAT0 register
     * \return Value of the rh850 MPAT0 register
     */
    unsigned int read_MPAT0(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_ASID register field
     * \return Value of the rh850 MPAT0_ASID register field
     */
    unsigned short read_MPAT0_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_E register field
     * \return Value of the rh850 MPAT0_E register field
     */
    unsigned char read_MPAT0_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_G register field
     * \return Value of the rh850 MPAT0_G register field
     */
    unsigned char read_MPAT0_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_SX register field
     * \return Value of the rh850 MPAT0_SX register field
     */
    unsigned char read_MPAT0_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_SW register field
     * \return Value of the rh850 MPAT0_SW register field
     */
    unsigned char read_MPAT0_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_SR register field
     * \return Value of the rh850 MPAT0_SR register field
     */
    unsigned char read_MPAT0_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_UX register field
     * \return Value of the rh850 MPAT0_UX register field
     */
    unsigned char read_MPAT0_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_UW register field
     * \return Value of the rh850 MPAT0_UW register field
     */
    unsigned char read_MPAT0_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT0_UR register field
     * \return Value of the rh850 MPAT0_UR register field
     */
    unsigned char read_MPAT0_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA1 register
     * \return Value of the rh850 MPLA1 register
     */
    unsigned int read_MPLA1(void);
    /**
     * \brief Queries the value of the rh850 MPUA1 register
     * \return Value of the rh850 MPUA1 register
     */
    unsigned int read_MPUA1(void);
    /**
     * \brief Queries the value of the rh850 MPAT1 register
     * \return Value of the rh850 MPAT1 register
     */
    unsigned int read_MPAT1(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_ASID register field
     * \return Value of the rh850 MPAT1_ASID register field
     */
    unsigned short read_MPAT1_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_E register field
     * \return Value of the rh850 MPAT1_E register field
     */
    unsigned char read_MPAT1_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_G register field
     * \return Value of the rh850 MPAT1_G register field
     */
    unsigned char read_MPAT1_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_SX register field
     * \return Value of the rh850 MPAT1_SX register field
     */
    unsigned char read_MPAT1_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_SW register field
     * \return Value of the rh850 MPAT1_SW register field
     */
    unsigned char read_MPAT1_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_SR register field
     * \return Value of the rh850 MPAT1_SR register field
     */
    unsigned char read_MPAT1_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_UX register field
     * \return Value of the rh850 MPAT1_UX register field
     */
    unsigned char read_MPAT1_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_UW register field
     * \return Value of the rh850 MPAT1_UW register field
     */
    unsigned char read_MPAT1_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT1_UR register field
     * \return Value of the rh850 MPAT1_UR register field
     */
    unsigned char read_MPAT1_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA2 register
     * \return Value of the rh850 MPLA2 register
     */
    unsigned int read_MPLA2(void);
    /**
     * \brief Queries the value of the rh850 MPUA2 register
     * \return Value of the rh850 MPUA2 register
     */
    unsigned int read_MPUA2(void);
    /**
     * \brief Queries the value of the rh850 MPAT2 register
     * \return Value of the rh850 MPAT2 register
     */
    unsigned int read_MPAT2(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_ASID register field
     * \return Value of the rh850 MPAT2_ASID register field
     */
    unsigned short read_MPAT2_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_E register field
     * \return Value of the rh850 MPAT2_E register field
     */
    unsigned char read_MPAT2_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_G register field
     * \return Value of the rh850 MPAT2_G register field
     */
    unsigned char read_MPAT2_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_SX register field
     * \return Value of the rh850 MPAT2_SX register field
     */
    unsigned char read_MPAT2_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_SW register field
     * \return Value of the rh850 MPAT2_SW register field
     */
    unsigned char read_MPAT2_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_SR register field
     * \return Value of the rh850 MPAT2_SR register field
     */
    unsigned char read_MPAT2_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_UX register field
     * \return Value of the rh850 MPAT2_UX register field
     */
    unsigned char read_MPAT2_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_UW register field
     * \return Value of the rh850 MPAT2_UW register field
     */
    unsigned char read_MPAT2_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT2_UR register field
     * \return Value of the rh850 MPAT2_UR register field
     */
    unsigned char read_MPAT2_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA3 register
     * \return Value of the rh850 MPLA3 register
     */
    unsigned int read_MPLA3(void);
    /**
     * \brief Queries the value of the rh850 MPUA3 register
     * \return Value of the rh850 MPUA3 register
     */
    unsigned int read_MPUA3(void);
    /**
     * \brief Queries the value of the rh850 MPAT3 register
     * \return Value of the rh850 MPAT3 register
     */
    unsigned int read_MPAT3(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_ASID register field
     * \return Value of the rh850 MPAT3_ASID register field
     */
    unsigned short read_MPAT3_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_E register field
     * \return Value of the rh850 MPAT3_E register field
     */
    unsigned char read_MPAT3_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_G register field
     * \return Value of the rh850 MPAT3_G register field
     */
    unsigned char read_MPAT3_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_SX register field
     * \return Value of the rh850 MPAT3_SX register field
     */
    unsigned char read_MPAT3_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_SW register field
     * \return Value of the rh850 MPAT3_SW register field
     */
    unsigned char read_MPAT3_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_SR register field
     * \return Value of the rh850 MPAT3_SR register field
     */
    unsigned char read_MPAT3_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_UX register field
     * \return Value of the rh850 MPAT3_UX register field
     */
    unsigned char read_MPAT3_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_UW register field
     * \return Value of the rh850 MPAT3_UW register field
     */
    unsigned char read_MPAT3_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT3_UR register field
     * \return Value of the rh850 MPAT3_UR register field
     */
    unsigned char read_MPAT3_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA4 register
     * \return Value of the rh850 MPLA4 register
     */
    unsigned int read_MPLA4(void);
    /**
     * \brief Queries the value of the rh850 MPUA4 register
     * \return Value of the rh850 MPUA4 register
     */
    unsigned int read_MPUA4(void);
    /**
     * \brief Queries the value of the rh850 MPAT4 register
     * \return Value of the rh850 MPAT4 register
     */
    unsigned int read_MPAT4(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_ASID register field
     * \return Value of the rh850 MPAT4_ASID register field
     */
    unsigned short read_MPAT4_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_E register field
     * \return Value of the rh850 MPAT4_E register field
     */
    unsigned char read_MPAT4_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_G register field
     * \return Value of the rh850 MPAT4_G register field
     */
    unsigned char read_MPAT4_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_SX register field
     * \return Value of the rh850 MPAT4_SX register field
     */
    unsigned char read_MPAT4_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_SW register field
     * \return Value of the rh850 MPAT4_SW register field
     */
    unsigned char read_MPAT4_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_SR register field
     * \return Value of the rh850 MPAT4_SR register field
     */
    unsigned char read_MPAT4_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_UX register field
     * \return Value of the rh850 MPAT4_UX register field
     */
    unsigned char read_MPAT4_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_UW register field
     * \return Value of the rh850 MPAT4_UW register field
     */
    unsigned char read_MPAT4_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT4_UR register field
     * \return Value of the rh850 MPAT4_UR register field
     */
    unsigned char read_MPAT4_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA5 register
     * \return Value of the rh850 MPLA5 register
     */
    unsigned int read_MPLA5(void);
    /**
     * \brief Queries the value of the rh850 MPUA5 register
     * \return Value of the rh850 MPUA5 register
     */
    unsigned int read_MPUA5(void);
    /**
     * \brief Queries the value of the rh850 MPAT5 register
     * \return Value of the rh850 MPAT5 register
     */
    unsigned int read_MPAT5(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_ASID register field
     * \return Value of the rh850 MPAT5_ASID register field
     */
    unsigned short read_MPAT5_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_E register field
     * \return Value of the rh850 MPAT5_E register field
     */
    unsigned char read_MPAT5_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_G register field
     * \return Value of the rh850 MPAT5_G register field
     */
    unsigned char read_MPAT5_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_SX register field
     * \return Value of the rh850 MPAT5_SX register field
     */
    unsigned char read_MPAT5_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_SW register field
     * \return Value of the rh850 MPAT5_SW register field
     */
    unsigned char read_MPAT5_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_SR register field
     * \return Value of the rh850 MPAT5_SR register field
     */
    unsigned char read_MPAT5_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_UX register field
     * \return Value of the rh850 MPAT5_UX register field
     */
    unsigned char read_MPAT5_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_UW register field
     * \return Value of the rh850 MPAT5_UW register field
     */
    unsigned char read_MPAT5_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT5_UR register field
     * \return Value of the rh850 MPAT5_UR register field
     */
    unsigned char read_MPAT5_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA6 register
     * \return Value of the rh850 MPLA6 register
     */
    unsigned int read_MPLA6(void);
    /**
     * \brief Queries the value of the rh850 MPUA6 register
     * \return Value of the rh850 MPUA6 register
     */
    unsigned int read_MPUA6(void);
    /**
     * \brief Queries the value of the rh850 MPAT6 register
     * \return Value of the rh850 MPAT6 register
     */
    unsigned int read_MPAT6(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_ASID register field
     * \return Value of the rh850 MPAT6_ASID register field
     */
    unsigned short read_MPAT6_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_E register field
     * \return Value of the rh850 MPAT6_E register field
     */
    unsigned char read_MPAT6_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_G register field
     * \return Value of the rh850 MPAT6_G register field
     */
    unsigned char read_MPAT6_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_SX register field
     * \return Value of the rh850 MPAT6_SX register field
     */
    unsigned char read_MPAT6_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_SW register field
     * \return Value of the rh850 MPAT6_SW register field
     */
    unsigned char read_MPAT6_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_SR register field
     * \return Value of the rh850 MPAT6_SR register field
     */
    unsigned char read_MPAT6_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_UX register field
     * \return Value of the rh850 MPAT6_UX register field
     */
    unsigned char read_MPAT6_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_UW register field
     * \return Value of the rh850 MPAT6_UW register field
     */
    unsigned char read_MPAT6_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT6_UR register field
     * \return Value of the rh850 MPAT6_UR register field
     */
    unsigned char read_MPAT6_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA7 register
     * \return Value of the rh850 MPLA7 register
     */
    unsigned int read_MPLA7(void);
    /**
     * \brief Queries the value of the rh850 MPUA7 register
     * \return Value of the rh850 MPUA7 register
     */
    unsigned int read_MPUA7(void);
    /**
     * \brief Queries the value of the rh850 MPAT7 register
     * \return Value of the rh850 MPAT7 register
     */
    unsigned int read_MPAT7(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_ASID register field
     * \return Value of the rh850 MPAT7_ASID register field
     */
    unsigned short read_MPAT7_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_E register field
     * \return Value of the rh850 MPAT7_E register field
     */
    unsigned char read_MPAT7_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_G register field
     * \return Value of the rh850 MPAT7_G register field
     */
    unsigned char read_MPAT7_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_SX register field
     * \return Value of the rh850 MPAT7_SX register field
     */
    unsigned char read_MPAT7_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_SW register field
     * \return Value of the rh850 MPAT7_SW register field
     */
    unsigned char read_MPAT7_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_SR register field
     * \return Value of the rh850 MPAT7_SR register field
     */
    unsigned char read_MPAT7_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_UX register field
     * \return Value of the rh850 MPAT7_UX register field
     */
    unsigned char read_MPAT7_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_UW register field
     * \return Value of the rh850 MPAT7_UW register field
     */
    unsigned char read_MPAT7_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT7_UR register field
     * \return Value of the rh850 MPAT7_UR register field
     */
    unsigned char read_MPAT7_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA8 register
     * \return Value of the rh850 MPLA8 register
     */
    unsigned int read_MPLA8(void);
    /**
     * \brief Queries the value of the rh850 MPUA8 register
     * \return Value of the rh850 MPUA8 register
     */
    unsigned int read_MPUA8(void);
    /**
     * \brief Queries the value of the rh850 MPAT8 register
     * \return Value of the rh850 MPAT8 register
     */
    unsigned int read_MPAT8(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_ASID register field
     * \return Value of the rh850 MPAT8_ASID register field
     */
    unsigned short read_MPAT8_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_E register field
     * \return Value of the rh850 MPAT8_E register field
     */
    unsigned char read_MPAT8_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_G register field
     * \return Value of the rh850 MPAT8_G register field
     */
    unsigned char read_MPAT8_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_SX register field
     * \return Value of the rh850 MPAT8_SX register field
     */
    unsigned char read_MPAT8_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_SW register field
     * \return Value of the rh850 MPAT8_SW register field
     */
    unsigned char read_MPAT8_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_SR register field
     * \return Value of the rh850 MPAT8_SR register field
     */
    unsigned char read_MPAT8_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_UX register field
     * \return Value of the rh850 MPAT8_UX register field
     */
    unsigned char read_MPAT8_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_UW register field
     * \return Value of the rh850 MPAT8_UW register field
     */
    unsigned char read_MPAT8_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT8_UR register field
     * \return Value of the rh850 MPAT8_UR register field
     */
    unsigned char read_MPAT8_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA9 register
     * \return Value of the rh850 MPLA9 register
     */
    unsigned int read_MPLA9(void);
    /**
     * \brief Queries the value of the rh850 MPUA9 register
     * \return Value of the rh850 MPUA9 register
     */
    unsigned int read_MPUA9(void);
    /**
     * \brief Queries the value of the rh850 MPAT9 register
     * \return Value of the rh850 MPAT9 register
     */
    unsigned int read_MPAT9(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_ASID register field
     * \return Value of the rh850 MPAT9_ASID register field
     */
    unsigned short read_MPAT9_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_E register field
     * \return Value of the rh850 MPAT9_E register field
     */
    unsigned char read_MPAT9_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_G register field
     * \return Value of the rh850 MPAT9_G register field
     */
    unsigned char read_MPAT9_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_SX register field
     * \return Value of the rh850 MPAT9_SX register field
     */
    unsigned char read_MPAT9_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_SW register field
     * \return Value of the rh850 MPAT9_SW register field
     */
    unsigned char read_MPAT9_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_SR register field
     * \return Value of the rh850 MPAT9_SR register field
     */
    unsigned char read_MPAT9_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_UX register field
     * \return Value of the rh850 MPAT9_UX register field
     */
    unsigned char read_MPAT9_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_UW register field
     * \return Value of the rh850 MPAT9_UW register field
     */
    unsigned char read_MPAT9_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT9_UR register field
     * \return Value of the rh850 MPAT9_UR register field
     */
    unsigned char read_MPAT9_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA10 register
     * \return Value of the rh850 MPLA10 register
     */
    unsigned int read_MPLA10(void);
    /**
     * \brief Queries the value of the rh850 MPUA10 register
     * \return Value of the rh850 MPUA10 register
     */
    unsigned int read_MPUA10(void);
    /**
     * \brief Queries the value of the rh850 MPAT10 register
     * \return Value of the rh850 MPAT10 register
     */
    unsigned int read_MPAT10(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_ASID register field
     * \return Value of the rh850 MPAT10_ASID register field
     */
    unsigned short read_MPAT10_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_E register field
     * \return Value of the rh850 MPAT10_E register field
     */
    unsigned char read_MPAT10_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_G register field
     * \return Value of the rh850 MPAT10_G register field
     */
    unsigned char read_MPAT10_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_SX register field
     * \return Value of the rh850 MPAT10_SX register field
     */
    unsigned char read_MPAT10_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_SW register field
     * \return Value of the rh850 MPAT10_SW register field
     */
    unsigned char read_MPAT10_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_SR register field
     * \return Value of the rh850 MPAT10_SR register field
     */
    unsigned char read_MPAT10_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_UX register field
     * \return Value of the rh850 MPAT10_UX register field
     */
    unsigned char read_MPAT10_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_UW register field
     * \return Value of the rh850 MPAT10_UW register field
     */
    unsigned char read_MPAT10_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT10_UR register field
     * \return Value of the rh850 MPAT10_UR register field
     */
    unsigned char read_MPAT10_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA11 register
     * \return Value of the rh850 MPLA11 register
     */
    unsigned int read_MPLA11(void);
    /**
     * \brief Queries the value of the rh850 MPUA11 register
     * \return Value of the rh850 MPUA11 register
     */
    unsigned int read_MPUA11(void);
    /**
     * \brief Queries the value of the rh850 MPAT11 register
     * \return Value of the rh850 MPAT11 register
     */
    unsigned int read_MPAT11(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_ASID register field
     * \return Value of the rh850 MPAT11_ASID register field
     */
    unsigned short read_MPAT11_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_E register field
     * \return Value of the rh850 MPAT11_E register field
     */
    unsigned char read_MPAT11_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_G register field
     * \return Value of the rh850 MPAT11_G register field
     */
    unsigned char read_MPAT11_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_SX register field
     * \return Value of the rh850 MPAT11_SX register field
     */
    unsigned char read_MPAT11_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_SW register field
     * \return Value of the rh850 MPAT11_SW register field
     */
    unsigned char read_MPAT11_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_SR register field
     * \return Value of the rh850 MPAT11_SR register field
     */
    unsigned char read_MPAT11_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_UX register field
     * \return Value of the rh850 MPAT11_UX register field
     */
    unsigned char read_MPAT11_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_UW register field
     * \return Value of the rh850 MPAT11_UW register field
     */
    unsigned char read_MPAT11_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT11_UR register field
     * \return Value of the rh850 MPAT11_UR register field
     */
    unsigned char read_MPAT11_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA12 register
     * \return Value of the rh850 MPLA12 register
     */
    unsigned int read_MPLA12(void);
    /**
     * \brief Queries the value of the rh850 MPUA12 register
     * \return Value of the rh850 MPUA12 register
     */
    unsigned int read_MPUA12(void);
    /**
     * \brief Queries the value of the rh850 MPAT12 register
     * \return Value of the rh850 MPAT12 register
     */
    unsigned int read_MPAT12(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_ASID register field
     * \return Value of the rh850 MPAT12_ASID register field
     */
    unsigned short read_MPAT12_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_E register field
     * \return Value of the rh850 MPAT12_E register field
     */
    unsigned char read_MPAT12_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_G register field
     * \return Value of the rh850 MPAT12_G register field
     */
    unsigned char read_MPAT12_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_SX register field
     * \return Value of the rh850 MPAT12_SX register field
     */
    unsigned char read_MPAT12_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_SW register field
     * \return Value of the rh850 MPAT12_SW register field
     */
    unsigned char read_MPAT12_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_SR register field
     * \return Value of the rh850 MPAT12_SR register field
     */
    unsigned char read_MPAT12_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_UX register field
     * \return Value of the rh850 MPAT12_UX register field
     */
    unsigned char read_MPAT12_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_UW register field
     * \return Value of the rh850 MPAT12_UW register field
     */
    unsigned char read_MPAT12_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT12_UR register field
     * \return Value of the rh850 MPAT12_UR register field
     */
    unsigned char read_MPAT12_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA13 register
     * \return Value of the rh850 MPLA13 register
     */
    unsigned int read_MPLA13(void);
    /**
     * \brief Queries the value of the rh850 MPUA13 register
     * \return Value of the rh850 MPUA13 register
     */
    unsigned int read_MPUA13(void);
    /**
     * \brief Queries the value of the rh850 MPAT13 register
     * \return Value of the rh850 MPAT13 register
     */
    unsigned int read_MPAT13(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_ASID register field
     * \return Value of the rh850 MPAT13_ASID register field
     */
    unsigned short read_MPAT13_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_E register field
     * \return Value of the rh850 MPAT13_E register field
     */
    unsigned char read_MPAT13_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_G register field
     * \return Value of the rh850 MPAT13_G register field
     */
    unsigned char read_MPAT13_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_SX register field
     * \return Value of the rh850 MPAT13_SX register field
     */
    unsigned char read_MPAT13_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_SW register field
     * \return Value of the rh850 MPAT13_SW register field
     */
    unsigned char read_MPAT13_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_SR register field
     * \return Value of the rh850 MPAT13_SR register field
     */
    unsigned char read_MPAT13_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_UX register field
     * \return Value of the rh850 MPAT13_UX register field
     */
    unsigned char read_MPAT13_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_UW register field
     * \return Value of the rh850 MPAT13_UW register field
     */
    unsigned char read_MPAT13_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT13_UR register field
     * \return Value of the rh850 MPAT13_UR register field
     */
    unsigned char read_MPAT13_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA14 register
     * \return Value of the rh850 MPLA14 register
     */
    unsigned int read_MPLA14(void);
    /**
     * \brief Queries the value of the rh850 MPUA14 register
     * \return Value of the rh850 MPUA14 register
     */
    unsigned int read_MPUA14(void);
    /**
     * \brief Queries the value of the rh850 MPAT14 register
     * \return Value of the rh850 MPAT14 register
     */
    unsigned int read_MPAT14(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_ASID register field
     * \return Value of the rh850 MPAT14_ASID register field
     */
    unsigned short read_MPAT14_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_E register field
     * \return Value of the rh850 MPAT14_E register field
     */
    unsigned char read_MPAT14_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_G register field
     * \return Value of the rh850 MPAT14_G register field
     */
    unsigned char read_MPAT14_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_SX register field
     * \return Value of the rh850 MPAT14_SX register field
     */
    unsigned char read_MPAT14_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_SW register field
     * \return Value of the rh850 MPAT14_SW register field
     */
    unsigned char read_MPAT14_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_SR register field
     * \return Value of the rh850 MPAT14_SR register field
     */
    unsigned char read_MPAT14_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_UX register field
     * \return Value of the rh850 MPAT14_UX register field
     */
    unsigned char read_MPAT14_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_UW register field
     * \return Value of the rh850 MPAT14_UW register field
     */
    unsigned char read_MPAT14_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT14_UR register field
     * \return Value of the rh850 MPAT14_UR register field
     */
    unsigned char read_MPAT14_UR(void);
    /**
     * \brief Queries the value of the rh850 MPLA15 register
     * \return Value of the rh850 MPLA15 register
     */
    unsigned int read_MPLA15(void);
    /**
     * \brief Queries the value of the rh850 MPUA15 register
     * \return Value of the rh850 MPUA15 register
     */
    unsigned int read_MPUA15(void);
    /**
     * \brief Queries the value of the rh850 MPAT15 register
     * \return Value of the rh850 MPAT15 register
     */
    unsigned int read_MPAT15(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_ASID register field
     * \return Value of the rh850 MPAT15_ASID register field
     */
    unsigned short read_MPAT15_ASID(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_E register field
     * \return Value of the rh850 MPAT15_E register field
     */
    unsigned char read_MPAT15_E(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_G register field
     * \return Value of the rh850 MPAT15_G register field
     */
    unsigned char read_MPAT15_G(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_SX register field
     * \return Value of the rh850 MPAT15_SX register field
     */
    unsigned char read_MPAT15_SX(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_SW register field
     * \return Value of the rh850 MPAT15_SW register field
     */
    unsigned char read_MPAT15_SW(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_SR register field
     * \return Value of the rh850 MPAT15_SR register field
     */
    unsigned char read_MPAT15_SR(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_UX register field
     * \return Value of the rh850 MPAT15_UX register field
     */
    unsigned char read_MPAT15_UX(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_UW register field
     * \return Value of the rh850 MPAT15_UW register field
     */
    unsigned char read_MPAT15_UW(void);
    /**
     * \brief Queries the value of the rh850 MPAT15_UR register field
     * \return Value of the rh850 MPAT15_UR register field
     */
    unsigned char read_MPAT15_UR(void);
    /**
     * \brief Queries the value of the rh850 mpu_entry register
     * \return Value of the rh850 mpu_entry register
     */
    unsigned char read_mpu_entry(void);
    /**
     * \brief Queries the value of the rh850 mpat_g_support register
     * \return Value of the rh850 mpat_g_support register
     */
    unsigned char read_mpat_g_support(void);
    /**
     * \brief Queries the value of the rh850 swd0pcad_exec_count register
     * \return Value of the rh850 swd0pcad_exec_count register
     */
    unsigned int read_swd0pcad_exec_count(void);
    /**
     * \brief Queries the value of the rh850 swd0pcad_val register
     * \return Value of the rh850 swd0pcad_val register
     */
    unsigned int read_swd0pcad_val(void);
    /**
     * \brief Queries the value of the rh850 mem_acc_type register
     * \return Value of the rh850 mem_acc_type register
     */
    unsigned int read_mem_acc_type(void);

    // Immediate register group read operations
    /**
     * \brief Queries the value of the specified register from the rh850 GR register group
     * \param index Index value of the required register from the rh850 GR register group
     * \return Value of the specified rh850 GR group register
     */
    unsigned int read_GR(rh850::GR_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \return Value of the specified rh850 SR_SELID_0 group register
     */
    unsigned int read_SR_SELID_0(rh850::SR_SELID_0_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \return Value of the specified rh850 SR_SELID_1 group register
     */
    unsigned int read_SR_SELID_1(rh850::SR_SELID_1_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \return Value of the specified rh850 SR_SELID_2 group register
     */
    unsigned int read_SR_SELID_2(rh850::SR_SELID_2_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \return Value of the specified rh850 SR_SELID_3 group register
     */
    unsigned int read_SR_SELID_3(rh850::SR_SELID_3_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \return Value of the specified rh850 SR_SELID_5 group register
     */
    unsigned int read_SR_SELID_5(rh850::SR_SELID_5_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850 SR_SELID_6 register group
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \return Value of the specified rh850 SR_SELID_6 group register
     */
    unsigned int read_SR_SELID_6(rh850::SR_SELID_6_index_t index);
    /**
     * \brief Queries the value of the specified register from the rh850 SR_SELID_7 register group
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \return Value of the specified rh850 SR_SELID_7 group register
     */
    unsigned int read_SR_SELID_7(rh850::SR_SELID_7_index_t index);
    
    // Immediate register and register field write operations
    /**
     * \brief Sets the value of the rh850 translation time PC register
     * \param value New value of the rh850 translation time PC register
     */
    void write_trans_PC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r0 register
     * \param value New value of the rh850 r0 register
     */
    void write_r0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r1 register
     * \param value New value of the rh850 r1 register
     */
    void write_r1(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r2 register
     * \param value New value of the rh850 r2 register
     */
    void write_r2(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r3 register
     * \param value New value of the rh850 r3 register
     */
    void write_r3(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r4 register
     * \param value New value of the rh850 r4 register
     */
    void write_r4(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r5 register
     * \param value New value of the rh850 r5 register
     */
    void write_r5(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r6 register
     * \param value New value of the rh850 r6 register
     */
    void write_r6(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r7 register
     * \param value New value of the rh850 r7 register
     */
    void write_r7(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r8 register
     * \param value New value of the rh850 r8 register
     */
    void write_r8(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r9 register
     * \param value New value of the rh850 r9 register
     */
    void write_r9(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r10 register
     * \param value New value of the rh850 r10 register
     */
    void write_r10(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r11 register
     * \param value New value of the rh850 r11 register
     */
    void write_r11(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r12 register
     * \param value New value of the rh850 r12 register
     */
    void write_r12(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r13 register
     * \param value New value of the rh850 r13 register
     */
    void write_r13(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r14 register
     * \param value New value of the rh850 r14 register
     */
    void write_r14(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r15 register
     * \param value New value of the rh850 r15 register
     */
    void write_r15(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r16 register
     * \param value New value of the rh850 r16 register
     */
    void write_r16(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r17 register
     * \param value New value of the rh850 r17 register
     */
    void write_r17(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r18 register
     * \param value New value of the rh850 r18 register
     */
    void write_r18(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r19 register
     * \param value New value of the rh850 r19 register
     */
    void write_r19(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r20 register
     * \param value New value of the rh850 r20 register
     */
    void write_r20(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r21 register
     * \param value New value of the rh850 r21 register
     */
    void write_r21(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r22 register
     * \param value New value of the rh850 r22 register
     */
    void write_r22(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r23 register
     * \param value New value of the rh850 r23 register
     */
    void write_r23(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r24 register
     * \param value New value of the rh850 r24 register
     */
    void write_r24(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r25 register
     * \param value New value of the rh850 r25 register
     */
    void write_r25(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r26 register
     * \param value New value of the rh850 r26 register
     */
    void write_r26(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r27 register
     * \param value New value of the rh850 r27 register
     */
    void write_r27(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r28 register
     * \param value New value of the rh850 r28 register
     */
    void write_r28(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r29 register
     * \param value New value of the rh850 r29 register
     */
    void write_r29(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r30 register
     * \param value New value of the rh850 r30 register
     */
    void write_r30(unsigned int value);
    /**
     * \brief Sets the value of the rh850 r31 register
     * \param value New value of the rh850 r31 register
     */
    void write_r31(unsigned int value);
    /**
     * \brief Sets the value of the rh850 PC register
     * \param value New value of the rh850 PC register
     */
    void write_PC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 EIPC register
     * \param value New value of the rh850 EIPC register
     */
    void write_EIPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 EIPSW register
     * \param value New value of the rh850 EIPSW register
     */
    void write_EIPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FEPC register
     * \param value New value of the rh850 FEPC register
     */
    void write_FEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FEPSW register
     * \param value New value of the rh850 FEPSW register
     */
    void write_FEPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850 PSW register
     * \param value New value of the rh850 PSW register
     */
    void write_PSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850 PSW_Z register field
     * \param value New value of the rh850 PSW_Z register field
     */
    void write_PSW_Z(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_S register field
     * \param value New value of the rh850 PSW_S register field
     */
    void write_PSW_S(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_OV register field
     * \param value New value of the rh850 PSW_OV register field
     */
    void write_PSW_OV(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_CY register field
     * \param value New value of the rh850 PSW_CY register field
     */
    void write_PSW_CY(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_SAT register field
     * \param value New value of the rh850 PSW_SAT register field
     */
    void write_PSW_SAT(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_ID register field
     * \param value New value of the rh850 PSW_ID register field
     */
    void write_PSW_ID(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_EP register field
     * \param value New value of the rh850 PSW_EP register field
     */
    void write_PSW_EP(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_NP register field
     * \param value New value of the rh850 PSW_NP register field
     */
    void write_PSW_NP(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_CM register field
     * \param value New value of the rh850 PSW_CM register field
     */
    void write_PSW_CM(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_SS register field
     * \param value New value of the rh850 PSW_SS register field
     */
    void write_PSW_SS(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_EBV register field
     * \param value New value of the rh850 PSW_EBV register field
     */
    void write_PSW_EBV(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_CU0 register field
     * \param value New value of the rh850 PSW_CU0 register field
     */
    void write_PSW_CU0(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_CU1 register field
     * \param value New value of the rh850 PSW_CU1 register field
     */
    void write_PSW_CU1(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_CU2 register field
     * \param value New value of the rh850 PSW_CU2 register field
     */
    void write_PSW_CU2(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_HVC register field
     * \param value New value of the rh850 PSW_HVC register field
     */
    void write_PSW_HVC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PSW_UM register field
     * \param value New value of the rh850 PSW_UM register field
     */
    void write_PSW_UM(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR register
     * \param value New value of the rh850 FPSR register
     */
    void write_FPSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPSR_CC register field
     * \param value New value of the rh850 FPSR_CC register field
     */
    void write_FPSR_CC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_FN register field
     * \param value New value of the rh850 FPSR_FN register field
     */
    void write_FPSR_FN(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_IF register field
     * \param value New value of the rh850 FPSR_IF register field
     */
    void write_FPSR_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_PEM register field
     * \param value New value of the rh850 FPSR_PEM register field
     */
    void write_FPSR_PEM(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_RM register field
     * \param value New value of the rh850 FPSR_RM register field
     */
    void write_FPSR_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_FS register field
     * \param value New value of the rh850 FPSR_FS register field
     */
    void write_FPSR_FS(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_XC register field
     * \param value New value of the rh850 FPSR_XC register field
     */
    void write_FPSR_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_XE register field
     * \param value New value of the rh850 FPSR_XE register field
     */
    void write_FPSR_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPSR_XP register field
     * \param value New value of the rh850 FPSR_XP register field
     */
    void write_FPSR_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPEPC register
     * \param value New value of the rh850 FPEPC register
     */
    void write_FPEPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPST register
     * \param value New value of the rh850 FPST register
     */
    void write_FPST(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPST_XC register field
     * \param value New value of the rh850 FPST_XC register field
     */
    void write_FPST_XC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPST_IF register field
     * \param value New value of the rh850 FPST_IF register field
     */
    void write_FPST_IF(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPST_XP register field
     * \param value New value of the rh850 FPST_XP register field
     */
    void write_FPST_XP(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPCC register
     * \param value New value of the rh850 FPCC register
     */
    void write_FPCC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPCC_CC register field
     * \param value New value of the rh850 FPCC_CC register field
     */
    void write_FPCC_CC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPCFG register
     * \param value New value of the rh850 FPCFG register
     */
    void write_FPCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPCFG_RM register field
     * \param value New value of the rh850 FPCFG_RM register field
     */
    void write_FPCFG_RM(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPCFG_XE register field
     * \param value New value of the rh850 FPCFG_XE register field
     */
    void write_FPCFG_XE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 FPEC register
     * \param value New value of the rh850 FPEC register
     */
    void write_FPEC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 EIIC register
     * \param value New value of the rh850 EIIC register
     */
    void write_EIIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FEIC register
     * \param value New value of the rh850 FEIC register
     */
    void write_FEIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 CTPC register
     * \param value New value of the rh850 CTPC register
     */
    void write_CTPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 CTPSW register
     * \param value New value of the rh850 CTPSW register
     */
    void write_CTPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850 CTBP register
     * \param value New value of the rh850 CTBP register
     */
    void write_CTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850 EIWR register
     * \param value New value of the rh850 EIWR register
     */
    void write_EIWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FEWR register
     * \param value New value of the rh850 FEWR register
     */
    void write_FEWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 BSEL register
     * \param value New value of the rh850 BSEL register
     */
    void write_BSEL(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MCFG0 register
     * \param value New value of the rh850 MCFG0 register
     */
    void write_MCFG0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MCFG0_SPID register field
     * \param value New value of the rh850 MCFG0_SPID register field
     */
    void write_MCFG0_SPID(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCFG0_HVP register field
     * \param value New value of the rh850 MCFG0_HVP register field
     */
    void write_MCFG0_HVP(unsigned char value);
    /**
     * \brief Sets the value of the rh850 RBASE register
     * \param value New value of the rh850 RBASE register
     */
    void write_RBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850 RBASE_RBASE register field
     * \param value New value of the rh850 RBASE_RBASE register field
     */
    void write_RBASE_RBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850 RBASE_RINT register field
     * \param value New value of the rh850 RBASE_RINT register field
     */
    void write_RBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850 EBASE register
     * \param value New value of the rh850 EBASE register
     */
    void write_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850 EBASE_EBASE register field
     * \param value New value of the rh850 EBASE_EBASE register field
     */
    void write_EBASE_EBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850 EBASE_RINT register field
     * \param value New value of the rh850 EBASE_RINT register field
     */
    void write_EBASE_RINT(unsigned char value);
    /**
     * \brief Sets the value of the rh850 INTBP register
     * \param value New value of the rh850 INTBP register
     */
    void write_INTBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MCTL register
     * \param value New value of the rh850 MCTL register
     */
    void write_MCTL(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MCTL_EN register field
     * \param value New value of the rh850 MCTL_EN register field
     */
    void write_MCTL_EN(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCTL_MT register field
     * \param value New value of the rh850 MCTL_MT register field
     */
    void write_MCTL_MT(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCTL_STID register field
     * \param value New value of the rh850 MCTL_STID register field
     */
    void write_MCTL_STID(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCTL_MA register field
     * \param value New value of the rh850 MCTL_MA register field
     */
    void write_MCTL_MA(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCTL_UIC register field
     * \param value New value of the rh850 MCTL_UIC register field
     */
    void write_MCTL_UIC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PID register
     * \param value New value of the rh850 PID register
     */
    void write_PID(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPIPR register
     * \param value New value of the rh850 FPIPR register
     */
    void write_FPIPR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPIPR_FPIPR register field
     * \param value New value of the rh850 FPIPR_FPIPR register field
     */
    void write_FPIPR_FPIPR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 SCCFG register
     * \param value New value of the rh850 SCCFG register
     */
    void write_SCCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850 SCCFG_SIZE register field
     * \param value New value of the rh850 SCCFG_SIZE register field
     */
    void write_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 SCBP register
     * \param value New value of the rh850 SCBP register
     */
    void write_SCBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850 HTCFG0 register
     * \param value New value of the rh850 HTCFG0 register
     */
    void write_HTCFG0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 HTCFG0_PEID register field
     * \param value New value of the rh850 HTCFG0_PEID register field
     */
    void write_HTCFG0_PEID(unsigned char value);
    /**
     * \brief Sets the value of the rh850 HTCFG0_NC register field
     * \param value New value of the rh850 HTCFG0_NC register field
     */
    void write_HTCFG0_NC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 HTCFG0_TCID register field
     * \param value New value of the rh850 HTCFG0_TCID register field
     */
    void write_HTCFG0_TCID(unsigned char value);
    /**
     * \brief Sets the value of the rh850 HVCCFG register
     * \param value New value of the rh850 HVCCFG register
     */
    void write_HVCCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850 HVCBP register
     * \param value New value of the rh850 HVCBP register
     */
    void write_HVCBP(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MEA register
     * \param value New value of the rh850 MEA register
     */
    void write_MEA(unsigned int value);
    /**
     * \brief Sets the value of the rh850 ASID register
     * \param value New value of the rh850 ASID register
     */
    void write_ASID(unsigned int value);
    /**
     * \brief Sets the value of the rh850 ASID_ASID register field
     * \param value New value of the rh850 ASID_ASID register field
     */
    void write_ASID_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MEI register
     * \param value New value of the rh850 MEI register
     */
    void write_MEI(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MEI_REG register field
     * \param value New value of the rh850 MEI_REG register field
     */
    void write_MEI_REG(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MEI_DS register field
     * \param value New value of the rh850 MEI_DS register field
     */
    void write_MEI_DS(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MEI_U register field
     * \param value New value of the rh850 MEI_U register field
     */
    void write_MEI_U(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MEI_ITYPE register field
     * \param value New value of the rh850 MEI_ITYPE register field
     */
    void write_MEI_ITYPE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MEI_RW register field
     * \param value New value of the rh850 MEI_RW register field
     */
    void write_MEI_RW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 ISPR register
     * \param value New value of the rh850 ISPR register
     */
    void write_ISPR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 ISPR_ISP register field
     * \param value New value of the rh850 ISPR_ISP register field
     */
    void write_ISPR_ISP(unsigned char value);
    /**
     * \brief Sets the value of the rh850 PMR register
     * \param value New value of the rh850 PMR register
     */
    void write_PMR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 PMR_PM register field
     * \param value New value of the rh850 PMR_PM register field
     */
    void write_PMR_PM(unsigned char value);
    /**
     * \brief Sets the value of the rh850 ICSR register
     * \param value New value of the rh850 ICSR register
     */
    void write_ICSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 ICSR_PMEI register field
     * \param value New value of the rh850 ICSR_PMEI register field
     */
    void write_ICSR_PMEI(unsigned char value);
    /**
     * \brief Sets the value of the rh850 INTCFG register
     * \param value New value of the rh850 INTCFG register
     */
    void write_INTCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850 INTCFG_ISPC register field
     * \param value New value of the rh850 INTCFG_ISPC register field
     */
    void write_INTCFG_ISPC(unsigned char value);
    /**
     * \brief Sets the value of the rh850 DBIC register
     * \param value New value of the rh850 DBIC register
     */
    void write_DBIC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 DBCMC register
     * \param value New value of the rh850 DBCMC register
     */
    void write_DBCMC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 DBPC register
     * \param value New value of the rh850 DBPC register
     */
    void write_DBPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 DBPSW register
     * \param value New value of the rh850 DBPSW register
     */
    void write_DBPSW(unsigned int value);
    /**
     * \brief Sets the value of the rh850 DIR0 register
     * \param value New value of the rh850 DIR0 register
     */
    void write_DIR0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 DIR1 register
     * \param value New value of the rh850 DIR1 register
     */
    void write_DIR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850 BPC register
     * \param value New value of the rh850 BPC register
     */
    void write_BPC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 BPAV register
     * \param value New value of the rh850 BPAV register
     */
    void write_BPAV(unsigned int value);
    /**
     * \brief Sets the value of the rh850 BPAM register
     * \param value New value of the rh850 BPAM register
     */
    void write_BPAM(unsigned int value);
    /**
     * \brief Sets the value of the rh850 BPDV register
     * \param value New value of the rh850 BPDV register
     */
    void write_BPDV(unsigned int value);
    /**
     * \brief Sets the value of the rh850 BPDM register
     * \param value New value of the rh850 BPDM register
     */
    void write_BPDM(unsigned int value);
    /**
     * \brief Sets the value of the rh850 DBWR register
     * \param value New value of the rh850 DBWR register
     */
    void write_DBWR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPM register
     * \param value New value of the rh850 MPM register
     */
    void write_MPM(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPM_DX register field
     * \param value New value of the rh850 MPM_DX register field
     */
    void write_MPM_DX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPM_DW register field
     * \param value New value of the rh850 MPM_DW register field
     */
    void write_MPM_DW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPM_DR register field
     * \param value New value of the rh850 MPM_DR register field
     */
    void write_MPM_DR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPM_SVP register field
     * \param value New value of the rh850 MPM_SVP register field
     */
    void write_MPM_SVP(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPM_MPE register field
     * \param value New value of the rh850 MPM_MPE register field
     */
    void write_MPM_MPE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC register
     * \param value New value of the rh850 MPRC register
     */
    void write_MPRC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPRC_E15 register field
     * \param value New value of the rh850 MPRC_E15 register field
     */
    void write_MPRC_E15(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E14 register field
     * \param value New value of the rh850 MPRC_E14 register field
     */
    void write_MPRC_E14(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E13 register field
     * \param value New value of the rh850 MPRC_E13 register field
     */
    void write_MPRC_E13(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E12 register field
     * \param value New value of the rh850 MPRC_E12 register field
     */
    void write_MPRC_E12(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E11 register field
     * \param value New value of the rh850 MPRC_E11 register field
     */
    void write_MPRC_E11(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E10 register field
     * \param value New value of the rh850 MPRC_E10 register field
     */
    void write_MPRC_E10(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E9 register field
     * \param value New value of the rh850 MPRC_E9 register field
     */
    void write_MPRC_E9(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E8 register field
     * \param value New value of the rh850 MPRC_E8 register field
     */
    void write_MPRC_E8(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E7 register field
     * \param value New value of the rh850 MPRC_E7 register field
     */
    void write_MPRC_E7(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E6 register field
     * \param value New value of the rh850 MPRC_E6 register field
     */
    void write_MPRC_E6(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E5 register field
     * \param value New value of the rh850 MPRC_E5 register field
     */
    void write_MPRC_E5(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E4 register field
     * \param value New value of the rh850 MPRC_E4 register field
     */
    void write_MPRC_E4(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E3 register field
     * \param value New value of the rh850 MPRC_E3 register field
     */
    void write_MPRC_E3(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E2 register field
     * \param value New value of the rh850 MPRC_E2 register field
     */
    void write_MPRC_E2(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E1 register field
     * \param value New value of the rh850 MPRC_E1 register field
     */
    void write_MPRC_E1(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPRC_E0 register field
     * \param value New value of the rh850 MPRC_E0 register field
     */
    void write_MPRC_E0(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPBRGN register
     * \param value New value of the rh850 MPBRGN register
     */
    void write_MPBRGN(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPBRGN_MPBRGN register field
     * \param value New value of the rh850 MPBRGN_MPBRGN register field
     */
    void write_MPBRGN_MPBRGN(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPTRGN register
     * \param value New value of the rh850 MPTRGN register
     */
    void write_MPTRGN(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPTRGN_MPTRGN register field
     * \param value New value of the rh850 MPTRGN_MPTRGN register field
     */
    void write_MPTRGN_MPTRGN(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCR register
     * \param value New value of the rh850 MCR register
     */
    void write_MCR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MCR_SXE register field
     * \param value New value of the rh850 MCR_SXE register field
     */
    void write_MCR_SXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCR_SWE register field
     * \param value New value of the rh850 MCR_SWE register field
     */
    void write_MCR_SWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCR_SRE register field
     * \param value New value of the rh850 MCR_SRE register field
     */
    void write_MCR_SRE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCR_UXE register field
     * \param value New value of the rh850 MCR_UXE register field
     */
    void write_MCR_UXE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCR_UWE register field
     * \param value New value of the rh850 MCR_UWE register field
     */
    void write_MCR_UWE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MCR_URE register field
     * \param value New value of the rh850 MCR_URE register field
     */
    void write_MCR_URE(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA0 register
     * \param value New value of the rh850 MPLA0 register
     */
    void write_MPLA0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA0 register
     * \param value New value of the rh850 MPUA0 register
     */
    void write_MPUA0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT0 register
     * \param value New value of the rh850 MPAT0 register
     */
    void write_MPAT0(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT0_ASID register field
     * \param value New value of the rh850 MPAT0_ASID register field
     */
    void write_MPAT0_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT0_E register field
     * \param value New value of the rh850 MPAT0_E register field
     */
    void write_MPAT0_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_G register field
     * \param value New value of the rh850 MPAT0_G register field
     */
    void write_MPAT0_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_SX register field
     * \param value New value of the rh850 MPAT0_SX register field
     */
    void write_MPAT0_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_SW register field
     * \param value New value of the rh850 MPAT0_SW register field
     */
    void write_MPAT0_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_SR register field
     * \param value New value of the rh850 MPAT0_SR register field
     */
    void write_MPAT0_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_UX register field
     * \param value New value of the rh850 MPAT0_UX register field
     */
    void write_MPAT0_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_UW register field
     * \param value New value of the rh850 MPAT0_UW register field
     */
    void write_MPAT0_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT0_UR register field
     * \param value New value of the rh850 MPAT0_UR register field
     */
    void write_MPAT0_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA1 register
     * \param value New value of the rh850 MPLA1 register
     */
    void write_MPLA1(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA1 register
     * \param value New value of the rh850 MPUA1 register
     */
    void write_MPUA1(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT1 register
     * \param value New value of the rh850 MPAT1 register
     */
    void write_MPAT1(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT1_ASID register field
     * \param value New value of the rh850 MPAT1_ASID register field
     */
    void write_MPAT1_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT1_E register field
     * \param value New value of the rh850 MPAT1_E register field
     */
    void write_MPAT1_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT1_G register field
     * \param value New value of the rh850 MPAT1_G register field
     */
    void write_MPAT1_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT1_SX register field
     * \param value New value of the rh850 MPAT1_SX register field
     */
    void write_MPAT1_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT1_SW register field
     * \param value New value of the rh850 MPAT1_SW register field
     */
    void write_MPAT1_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT1_SR register field
     * \param value New value of the rh850 MPAT1_SR register field
     */
    void write_MPAT1_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT1_UX register field
     * \param value New value of the rh850 MPAT1_UX register field
     */
    void write_MPAT1_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT1_UW register field
     * \param value New value of the rh850 MPAT1_UW register field
     */
    void write_MPAT1_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT1_UR register field
     * \param value New value of the rh850 MPAT1_UR register field
     */
    void write_MPAT1_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA2 register
     * \param value New value of the rh850 MPLA2 register
     */
    void write_MPLA2(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA2 register
     * \param value New value of the rh850 MPUA2 register
     */
    void write_MPUA2(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT2 register
     * \param value New value of the rh850 MPAT2 register
     */
    void write_MPAT2(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT2_ASID register field
     * \param value New value of the rh850 MPAT2_ASID register field
     */
    void write_MPAT2_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT2_E register field
     * \param value New value of the rh850 MPAT2_E register field
     */
    void write_MPAT2_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT2_G register field
     * \param value New value of the rh850 MPAT2_G register field
     */
    void write_MPAT2_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT2_SX register field
     * \param value New value of the rh850 MPAT2_SX register field
     */
    void write_MPAT2_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT2_SW register field
     * \param value New value of the rh850 MPAT2_SW register field
     */
    void write_MPAT2_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT2_SR register field
     * \param value New value of the rh850 MPAT2_SR register field
     */
    void write_MPAT2_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT2_UX register field
     * \param value New value of the rh850 MPAT2_UX register field
     */
    void write_MPAT2_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT2_UW register field
     * \param value New value of the rh850 MPAT2_UW register field
     */
    void write_MPAT2_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT2_UR register field
     * \param value New value of the rh850 MPAT2_UR register field
     */
    void write_MPAT2_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA3 register
     * \param value New value of the rh850 MPLA3 register
     */
    void write_MPLA3(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA3 register
     * \param value New value of the rh850 MPUA3 register
     */
    void write_MPUA3(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT3 register
     * \param value New value of the rh850 MPAT3 register
     */
    void write_MPAT3(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT3_ASID register field
     * \param value New value of the rh850 MPAT3_ASID register field
     */
    void write_MPAT3_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT3_E register field
     * \param value New value of the rh850 MPAT3_E register field
     */
    void write_MPAT3_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT3_G register field
     * \param value New value of the rh850 MPAT3_G register field
     */
    void write_MPAT3_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT3_SX register field
     * \param value New value of the rh850 MPAT3_SX register field
     */
    void write_MPAT3_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT3_SW register field
     * \param value New value of the rh850 MPAT3_SW register field
     */
    void write_MPAT3_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT3_SR register field
     * \param value New value of the rh850 MPAT3_SR register field
     */
    void write_MPAT3_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT3_UX register field
     * \param value New value of the rh850 MPAT3_UX register field
     */
    void write_MPAT3_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT3_UW register field
     * \param value New value of the rh850 MPAT3_UW register field
     */
    void write_MPAT3_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT3_UR register field
     * \param value New value of the rh850 MPAT3_UR register field
     */
    void write_MPAT3_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA4 register
     * \param value New value of the rh850 MPLA4 register
     */
    void write_MPLA4(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA4 register
     * \param value New value of the rh850 MPUA4 register
     */
    void write_MPUA4(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT4 register
     * \param value New value of the rh850 MPAT4 register
     */
    void write_MPAT4(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT4_ASID register field
     * \param value New value of the rh850 MPAT4_ASID register field
     */
    void write_MPAT4_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT4_E register field
     * \param value New value of the rh850 MPAT4_E register field
     */
    void write_MPAT4_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT4_G register field
     * \param value New value of the rh850 MPAT4_G register field
     */
    void write_MPAT4_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT4_SX register field
     * \param value New value of the rh850 MPAT4_SX register field
     */
    void write_MPAT4_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT4_SW register field
     * \param value New value of the rh850 MPAT4_SW register field
     */
    void write_MPAT4_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT4_SR register field
     * \param value New value of the rh850 MPAT4_SR register field
     */
    void write_MPAT4_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT4_UX register field
     * \param value New value of the rh850 MPAT4_UX register field
     */
    void write_MPAT4_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT4_UW register field
     * \param value New value of the rh850 MPAT4_UW register field
     */
    void write_MPAT4_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT4_UR register field
     * \param value New value of the rh850 MPAT4_UR register field
     */
    void write_MPAT4_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA5 register
     * \param value New value of the rh850 MPLA5 register
     */
    void write_MPLA5(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA5 register
     * \param value New value of the rh850 MPUA5 register
     */
    void write_MPUA5(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT5 register
     * \param value New value of the rh850 MPAT5 register
     */
    void write_MPAT5(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT5_ASID register field
     * \param value New value of the rh850 MPAT5_ASID register field
     */
    void write_MPAT5_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT5_E register field
     * \param value New value of the rh850 MPAT5_E register field
     */
    void write_MPAT5_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT5_G register field
     * \param value New value of the rh850 MPAT5_G register field
     */
    void write_MPAT5_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT5_SX register field
     * \param value New value of the rh850 MPAT5_SX register field
     */
    void write_MPAT5_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT5_SW register field
     * \param value New value of the rh850 MPAT5_SW register field
     */
    void write_MPAT5_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT5_SR register field
     * \param value New value of the rh850 MPAT5_SR register field
     */
    void write_MPAT5_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT5_UX register field
     * \param value New value of the rh850 MPAT5_UX register field
     */
    void write_MPAT5_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT5_UW register field
     * \param value New value of the rh850 MPAT5_UW register field
     */
    void write_MPAT5_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT5_UR register field
     * \param value New value of the rh850 MPAT5_UR register field
     */
    void write_MPAT5_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA6 register
     * \param value New value of the rh850 MPLA6 register
     */
    void write_MPLA6(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA6 register
     * \param value New value of the rh850 MPUA6 register
     */
    void write_MPUA6(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT6 register
     * \param value New value of the rh850 MPAT6 register
     */
    void write_MPAT6(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT6_ASID register field
     * \param value New value of the rh850 MPAT6_ASID register field
     */
    void write_MPAT6_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT6_E register field
     * \param value New value of the rh850 MPAT6_E register field
     */
    void write_MPAT6_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT6_G register field
     * \param value New value of the rh850 MPAT6_G register field
     */
    void write_MPAT6_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT6_SX register field
     * \param value New value of the rh850 MPAT6_SX register field
     */
    void write_MPAT6_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT6_SW register field
     * \param value New value of the rh850 MPAT6_SW register field
     */
    void write_MPAT6_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT6_SR register field
     * \param value New value of the rh850 MPAT6_SR register field
     */
    void write_MPAT6_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT6_UX register field
     * \param value New value of the rh850 MPAT6_UX register field
     */
    void write_MPAT6_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT6_UW register field
     * \param value New value of the rh850 MPAT6_UW register field
     */
    void write_MPAT6_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT6_UR register field
     * \param value New value of the rh850 MPAT6_UR register field
     */
    void write_MPAT6_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA7 register
     * \param value New value of the rh850 MPLA7 register
     */
    void write_MPLA7(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA7 register
     * \param value New value of the rh850 MPUA7 register
     */
    void write_MPUA7(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT7 register
     * \param value New value of the rh850 MPAT7 register
     */
    void write_MPAT7(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT7_ASID register field
     * \param value New value of the rh850 MPAT7_ASID register field
     */
    void write_MPAT7_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT7_E register field
     * \param value New value of the rh850 MPAT7_E register field
     */
    void write_MPAT7_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT7_G register field
     * \param value New value of the rh850 MPAT7_G register field
     */
    void write_MPAT7_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT7_SX register field
     * \param value New value of the rh850 MPAT7_SX register field
     */
    void write_MPAT7_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT7_SW register field
     * \param value New value of the rh850 MPAT7_SW register field
     */
    void write_MPAT7_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT7_SR register field
     * \param value New value of the rh850 MPAT7_SR register field
     */
    void write_MPAT7_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT7_UX register field
     * \param value New value of the rh850 MPAT7_UX register field
     */
    void write_MPAT7_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT7_UW register field
     * \param value New value of the rh850 MPAT7_UW register field
     */
    void write_MPAT7_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT7_UR register field
     * \param value New value of the rh850 MPAT7_UR register field
     */
    void write_MPAT7_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA8 register
     * \param value New value of the rh850 MPLA8 register
     */
    void write_MPLA8(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA8 register
     * \param value New value of the rh850 MPUA8 register
     */
    void write_MPUA8(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT8 register
     * \param value New value of the rh850 MPAT8 register
     */
    void write_MPAT8(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT8_ASID register field
     * \param value New value of the rh850 MPAT8_ASID register field
     */
    void write_MPAT8_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT8_E register field
     * \param value New value of the rh850 MPAT8_E register field
     */
    void write_MPAT8_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT8_G register field
     * \param value New value of the rh850 MPAT8_G register field
     */
    void write_MPAT8_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT8_SX register field
     * \param value New value of the rh850 MPAT8_SX register field
     */
    void write_MPAT8_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT8_SW register field
     * \param value New value of the rh850 MPAT8_SW register field
     */
    void write_MPAT8_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT8_SR register field
     * \param value New value of the rh850 MPAT8_SR register field
     */
    void write_MPAT8_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT8_UX register field
     * \param value New value of the rh850 MPAT8_UX register field
     */
    void write_MPAT8_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT8_UW register field
     * \param value New value of the rh850 MPAT8_UW register field
     */
    void write_MPAT8_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT8_UR register field
     * \param value New value of the rh850 MPAT8_UR register field
     */
    void write_MPAT8_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA9 register
     * \param value New value of the rh850 MPLA9 register
     */
    void write_MPLA9(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA9 register
     * \param value New value of the rh850 MPUA9 register
     */
    void write_MPUA9(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT9 register
     * \param value New value of the rh850 MPAT9 register
     */
    void write_MPAT9(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT9_ASID register field
     * \param value New value of the rh850 MPAT9_ASID register field
     */
    void write_MPAT9_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT9_E register field
     * \param value New value of the rh850 MPAT9_E register field
     */
    void write_MPAT9_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT9_G register field
     * \param value New value of the rh850 MPAT9_G register field
     */
    void write_MPAT9_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT9_SX register field
     * \param value New value of the rh850 MPAT9_SX register field
     */
    void write_MPAT9_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT9_SW register field
     * \param value New value of the rh850 MPAT9_SW register field
     */
    void write_MPAT9_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT9_SR register field
     * \param value New value of the rh850 MPAT9_SR register field
     */
    void write_MPAT9_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT9_UX register field
     * \param value New value of the rh850 MPAT9_UX register field
     */
    void write_MPAT9_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT9_UW register field
     * \param value New value of the rh850 MPAT9_UW register field
     */
    void write_MPAT9_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT9_UR register field
     * \param value New value of the rh850 MPAT9_UR register field
     */
    void write_MPAT9_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA10 register
     * \param value New value of the rh850 MPLA10 register
     */
    void write_MPLA10(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA10 register
     * \param value New value of the rh850 MPUA10 register
     */
    void write_MPUA10(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT10 register
     * \param value New value of the rh850 MPAT10 register
     */
    void write_MPAT10(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT10_ASID register field
     * \param value New value of the rh850 MPAT10_ASID register field
     */
    void write_MPAT10_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT10_E register field
     * \param value New value of the rh850 MPAT10_E register field
     */
    void write_MPAT10_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT10_G register field
     * \param value New value of the rh850 MPAT10_G register field
     */
    void write_MPAT10_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT10_SX register field
     * \param value New value of the rh850 MPAT10_SX register field
     */
    void write_MPAT10_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT10_SW register field
     * \param value New value of the rh850 MPAT10_SW register field
     */
    void write_MPAT10_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT10_SR register field
     * \param value New value of the rh850 MPAT10_SR register field
     */
    void write_MPAT10_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT10_UX register field
     * \param value New value of the rh850 MPAT10_UX register field
     */
    void write_MPAT10_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT10_UW register field
     * \param value New value of the rh850 MPAT10_UW register field
     */
    void write_MPAT10_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT10_UR register field
     * \param value New value of the rh850 MPAT10_UR register field
     */
    void write_MPAT10_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA11 register
     * \param value New value of the rh850 MPLA11 register
     */
    void write_MPLA11(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA11 register
     * \param value New value of the rh850 MPUA11 register
     */
    void write_MPUA11(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT11 register
     * \param value New value of the rh850 MPAT11 register
     */
    void write_MPAT11(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT11_ASID register field
     * \param value New value of the rh850 MPAT11_ASID register field
     */
    void write_MPAT11_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT11_E register field
     * \param value New value of the rh850 MPAT11_E register field
     */
    void write_MPAT11_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT11_G register field
     * \param value New value of the rh850 MPAT11_G register field
     */
    void write_MPAT11_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT11_SX register field
     * \param value New value of the rh850 MPAT11_SX register field
     */
    void write_MPAT11_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT11_SW register field
     * \param value New value of the rh850 MPAT11_SW register field
     */
    void write_MPAT11_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT11_SR register field
     * \param value New value of the rh850 MPAT11_SR register field
     */
    void write_MPAT11_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT11_UX register field
     * \param value New value of the rh850 MPAT11_UX register field
     */
    void write_MPAT11_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT11_UW register field
     * \param value New value of the rh850 MPAT11_UW register field
     */
    void write_MPAT11_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT11_UR register field
     * \param value New value of the rh850 MPAT11_UR register field
     */
    void write_MPAT11_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA12 register
     * \param value New value of the rh850 MPLA12 register
     */
    void write_MPLA12(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA12 register
     * \param value New value of the rh850 MPUA12 register
     */
    void write_MPUA12(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT12 register
     * \param value New value of the rh850 MPAT12 register
     */
    void write_MPAT12(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT12_ASID register field
     * \param value New value of the rh850 MPAT12_ASID register field
     */
    void write_MPAT12_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT12_E register field
     * \param value New value of the rh850 MPAT12_E register field
     */
    void write_MPAT12_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT12_G register field
     * \param value New value of the rh850 MPAT12_G register field
     */
    void write_MPAT12_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT12_SX register field
     * \param value New value of the rh850 MPAT12_SX register field
     */
    void write_MPAT12_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT12_SW register field
     * \param value New value of the rh850 MPAT12_SW register field
     */
    void write_MPAT12_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT12_SR register field
     * \param value New value of the rh850 MPAT12_SR register field
     */
    void write_MPAT12_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT12_UX register field
     * \param value New value of the rh850 MPAT12_UX register field
     */
    void write_MPAT12_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT12_UW register field
     * \param value New value of the rh850 MPAT12_UW register field
     */
    void write_MPAT12_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT12_UR register field
     * \param value New value of the rh850 MPAT12_UR register field
     */
    void write_MPAT12_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA13 register
     * \param value New value of the rh850 MPLA13 register
     */
    void write_MPLA13(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA13 register
     * \param value New value of the rh850 MPUA13 register
     */
    void write_MPUA13(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT13 register
     * \param value New value of the rh850 MPAT13 register
     */
    void write_MPAT13(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT13_ASID register field
     * \param value New value of the rh850 MPAT13_ASID register field
     */
    void write_MPAT13_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT13_E register field
     * \param value New value of the rh850 MPAT13_E register field
     */
    void write_MPAT13_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT13_G register field
     * \param value New value of the rh850 MPAT13_G register field
     */
    void write_MPAT13_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT13_SX register field
     * \param value New value of the rh850 MPAT13_SX register field
     */
    void write_MPAT13_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT13_SW register field
     * \param value New value of the rh850 MPAT13_SW register field
     */
    void write_MPAT13_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT13_SR register field
     * \param value New value of the rh850 MPAT13_SR register field
     */
    void write_MPAT13_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT13_UX register field
     * \param value New value of the rh850 MPAT13_UX register field
     */
    void write_MPAT13_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT13_UW register field
     * \param value New value of the rh850 MPAT13_UW register field
     */
    void write_MPAT13_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT13_UR register field
     * \param value New value of the rh850 MPAT13_UR register field
     */
    void write_MPAT13_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA14 register
     * \param value New value of the rh850 MPLA14 register
     */
    void write_MPLA14(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA14 register
     * \param value New value of the rh850 MPUA14 register
     */
    void write_MPUA14(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT14 register
     * \param value New value of the rh850 MPAT14 register
     */
    void write_MPAT14(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT14_ASID register field
     * \param value New value of the rh850 MPAT14_ASID register field
     */
    void write_MPAT14_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT14_E register field
     * \param value New value of the rh850 MPAT14_E register field
     */
    void write_MPAT14_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT14_G register field
     * \param value New value of the rh850 MPAT14_G register field
     */
    void write_MPAT14_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT14_SX register field
     * \param value New value of the rh850 MPAT14_SX register field
     */
    void write_MPAT14_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT14_SW register field
     * \param value New value of the rh850 MPAT14_SW register field
     */
    void write_MPAT14_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT14_SR register field
     * \param value New value of the rh850 MPAT14_SR register field
     */
    void write_MPAT14_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT14_UX register field
     * \param value New value of the rh850 MPAT14_UX register field
     */
    void write_MPAT14_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT14_UW register field
     * \param value New value of the rh850 MPAT14_UW register field
     */
    void write_MPAT14_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT14_UR register field
     * \param value New value of the rh850 MPAT14_UR register field
     */
    void write_MPAT14_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPLA15 register
     * \param value New value of the rh850 MPLA15 register
     */
    void write_MPLA15(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPUA15 register
     * \param value New value of the rh850 MPUA15 register
     */
    void write_MPUA15(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT15 register
     * \param value New value of the rh850 MPAT15 register
     */
    void write_MPAT15(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPAT15_ASID register field
     * \param value New value of the rh850 MPAT15_ASID register field
     */
    void write_MPAT15_ASID(unsigned short value);
    /**
     * \brief Sets the value of the rh850 MPAT15_E register field
     * \param value New value of the rh850 MPAT15_E register field
     */
    void write_MPAT15_E(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT15_G register field
     * \param value New value of the rh850 MPAT15_G register field
     */
    void write_MPAT15_G(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT15_SX register field
     * \param value New value of the rh850 MPAT15_SX register field
     */
    void write_MPAT15_SX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT15_SW register field
     * \param value New value of the rh850 MPAT15_SW register field
     */
    void write_MPAT15_SW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT15_SR register field
     * \param value New value of the rh850 MPAT15_SR register field
     */
    void write_MPAT15_SR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT15_UX register field
     * \param value New value of the rh850 MPAT15_UX register field
     */
    void write_MPAT15_UX(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT15_UW register field
     * \param value New value of the rh850 MPAT15_UW register field
     */
    void write_MPAT15_UW(unsigned char value);
    /**
     * \brief Sets the value of the rh850 MPAT15_UR register field
     * \param value New value of the rh850 MPAT15_UR register field
     */
    void write_MPAT15_UR(unsigned char value);
    /**
     * \brief Sets the value of the rh850 mpu_entry register
     * \param value New value of the rh850 mpu_entry register
     */
    void write_mpu_entry(unsigned char value);
    /**
     * \brief Sets the value of the rh850 mpat_g_support register
     * \param value New value of the rh850 mpat_g_support register
     */
    void write_mpat_g_support(unsigned char value);
    /**
     * \brief Sets the value of the rh850 swd0pcad_exec_count register
     * \param value New value of the rh850 swd0pcad_exec_count register
     */
    void write_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Sets the value of the rh850 swd0pcad_val register
     * \param value New value of the rh850 swd0pcad_val register
     */
    void write_swd0pcad_val(unsigned int value);
    /**
     * \brief Sets the value of the rh850 mem_acc_type register
     * \param value New value of the rh850 mem_acc_type register
     */
    void write_mem_acc_type(unsigned int value);

    // Immediate register group write operations
    /**
     * \brief Sets the value of the specified register from the rh850 GR register group
     * \param index Index value of the required register from the rh850 GR register group
     * \param value New value of the specified rh850 GR group register
     */
    void write_GR(rh850::GR_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \param value New value of the specified rh850 SR_SELID_0 group register
     */
    void write_SR_SELID_0(rh850::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \param value New value of the specified rh850 SR_SELID_1 group register
     */
    void write_SR_SELID_1(rh850::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \param value New value of the specified rh850 SR_SELID_2 group register
     */
    void write_SR_SELID_2(rh850::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \param value New value of the specified rh850 SR_SELID_3 group register
     */
    void write_SR_SELID_3(rh850::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \param value New value of the specified rh850 SR_SELID_5 group register
     */
    void write_SR_SELID_5(rh850::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850 SR_SELID_6 register group
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \param value New value of the specified rh850 SR_SELID_6 group register
     */
    void write_SR_SELID_6(rh850::SR_SELID_6_index_t index, unsigned int value);
    /**
     * \brief Sets the value of the specified register from the rh850 SR_SELID_7 register group
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \param value New value of the specified rh850 SR_SELID_7 group register
     */
    void write_SR_SELID_7(rh850::SR_SELID_7_index_t index, unsigned int value);

    // Immediate register and register field increment operations
    /**
     * \brief Increments the value of the rh850 translation time PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 translation time PC register
     */
    void increment_trans_PC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r0 register
     */
    void increment_r0(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r1 register
     */
    void increment_r1(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r2 register
     */
    void increment_r2(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r3 register
     */
    void increment_r3(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r4 register
     */
    void increment_r4(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r5 register
     */
    void increment_r5(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r6 register
     */
    void increment_r6(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r7 register
     */
    void increment_r7(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r8 register
     */
    void increment_r8(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r9 register
     */
    void increment_r9(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r10 register
     */
    void increment_r10(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r11 register
     */
    void increment_r11(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r12 register
     */
    void increment_r12(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r13 register
     */
    void increment_r13(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r14 register
     */
    void increment_r14(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r15 register
     */
    void increment_r15(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r16 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r16 register
     */
    void increment_r16(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r17 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r17 register
     */
    void increment_r17(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r18 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r18 register
     */
    void increment_r18(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r19 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r19 register
     */
    void increment_r19(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r20 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r20 register
     */
    void increment_r20(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r21 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r21 register
     */
    void increment_r21(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r22 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r22 register
     */
    void increment_r22(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r23 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r23 register
     */
    void increment_r23(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r24 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r24 register
     */
    void increment_r24(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r25 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r25 register
     */
    void increment_r25(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r26 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r26 register
     */
    void increment_r26(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r27 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r27 register
     */
    void increment_r27(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r28 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r28 register
     */
    void increment_r28(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r29 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r29 register
     */
    void increment_r29(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r30 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r30 register
     */
    void increment_r30(unsigned int value);
    /**
     * \brief Increments the value of the rh850 r31 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 r31 register
     */
    void increment_r31(unsigned int value);
    /**
     * \brief Increments the value of the rh850 PC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 PC register
     */
    void increment_PC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 EIPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 EIPC register
     */
    void increment_EIPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 EIPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850 EIPSW register
     */
    void increment_EIPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850 FEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 FEPC register
     */
    void increment_FEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 FEPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850 FEPSW register
     */
    void increment_FEPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850 FPSR_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPSR_CC register field
     */
    void increment_FPSR_CC(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPSR_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPSR_RM register field
     */
    void increment_FPSR_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPSR_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPSR_XC register field
     */
    void increment_FPSR_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPSR_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPSR_XE register field
     */
    void increment_FPSR_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPSR_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPSR_XP register field
     */
    void increment_FPSR_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPEPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPEPC register
     */
    void increment_FPEPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 FPST_XC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPST_XC register field
     */
    void increment_FPST_XC(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPST_XP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPST_XP register field
     */
    void increment_FPST_XP(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPCC_CC register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPCC_CC register field
     */
    void increment_FPCC_CC(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPCFG_RM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPCFG_RM register field
     */
    void increment_FPCFG_RM(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPCFG_XE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPCFG_XE register field
     */
    void increment_FPCFG_XE(unsigned char value);
    /**
     * \brief Increments the value of the rh850 FPEC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPEC register
     */
    void increment_FPEC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 EIIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 EIIC register
     */
    void increment_EIIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 FEIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 FEIC register
     */
    void increment_FEIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 CTPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 CTPC register
     */
    void increment_CTPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 CTPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850 CTPSW register
     */
    void increment_CTPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850 CTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850 CTBP register
     */
    void increment_CTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850 EIWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850 EIWR register
     */
    void increment_EIWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850 FEWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850 FEWR register
     */
    void increment_FEWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850 BSEL register by the specified amount
     * \param value Amount by which to increment the value of the rh850 BSEL register
     */
    void increment_BSEL(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MCFG0_SPID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MCFG0_SPID register field
     */
    void increment_MCFG0_SPID(unsigned char value);
    /**
     * \brief Increments the value of the rh850 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 RBASE_RBASE register field
     */
    void increment_RBASE_RBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 EBASE_EBASE register field
     */
    void increment_EBASE_EBASE(unsigned int value);
    /**
     * \brief Increments the value of the rh850 INTBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850 INTBP register
     */
    void increment_INTBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MCTL_STID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MCTL_STID register field
     */
    void increment_MCTL_STID(unsigned char value);
    /**
     * \brief Increments the value of the rh850 PID register by the specified amount
     * \param value Amount by which to increment the value of the rh850 PID register
     */
    void increment_PID(unsigned int value);
    /**
     * \brief Increments the value of the rh850 FPIPR_FPIPR register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 FPIPR_FPIPR register field
     */
    void increment_FPIPR_FPIPR(unsigned char value);
    /**
     * \brief Increments the value of the rh850 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 SCCFG_SIZE register field
     */
    void increment_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Increments the value of the rh850 SCBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850 SCBP register
     */
    void increment_SCBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850 HTCFG0_PEID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 HTCFG0_PEID register field
     */
    void increment_HTCFG0_PEID(unsigned char value);
    /**
     * \brief Increments the value of the rh850 HTCFG0_TCID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 HTCFG0_TCID register field
     */
    void increment_HTCFG0_TCID(unsigned char value);
    /**
     * \brief Increments the value of the rh850 HVCCFG register by the specified amount
     * \param value Amount by which to increment the value of the rh850 HVCCFG register
     */
    void increment_HVCCFG(unsigned int value);
    /**
     * \brief Increments the value of the rh850 HVCBP register by the specified amount
     * \param value Amount by which to increment the value of the rh850 HVCBP register
     */
    void increment_HVCBP(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MEA register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MEA register
     */
    void increment_MEA(unsigned int value);
    /**
     * \brief Increments the value of the rh850 ASID_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 ASID_ASID register field
     */
    void increment_ASID_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MEI_REG register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MEI_REG register field
     */
    void increment_MEI_REG(unsigned char value);
    /**
     * \brief Increments the value of the rh850 MEI_DS register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MEI_DS register field
     */
    void increment_MEI_DS(unsigned char value);
    /**
     * \brief Increments the value of the rh850 MEI_ITYPE register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MEI_ITYPE register field
     */
    void increment_MEI_ITYPE(unsigned char value);
    /**
     * \brief Increments the value of the rh850 ISPR_ISP register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 ISPR_ISP register field
     */
    void increment_ISPR_ISP(unsigned char value);
    /**
     * \brief Increments the value of the rh850 PMR_PM register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 PMR_PM register field
     */
    void increment_PMR_PM(unsigned char value);
    /**
     * \brief Increments the value of the rh850 DBIC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 DBIC register
     */
    void increment_DBIC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 DBCMC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 DBCMC register
     */
    void increment_DBCMC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 DBPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 DBPC register
     */
    void increment_DBPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 DBPSW register by the specified amount
     * \param value Amount by which to increment the value of the rh850 DBPSW register
     */
    void increment_DBPSW(unsigned int value);
    /**
     * \brief Increments the value of the rh850 DIR0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 DIR0 register
     */
    void increment_DIR0(unsigned int value);
    /**
     * \brief Increments the value of the rh850 DIR1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 DIR1 register
     */
    void increment_DIR1(unsigned int value);
    /**
     * \brief Increments the value of the rh850 BPC register by the specified amount
     * \param value Amount by which to increment the value of the rh850 BPC register
     */
    void increment_BPC(unsigned int value);
    /**
     * \brief Increments the value of the rh850 BPAV register by the specified amount
     * \param value Amount by which to increment the value of the rh850 BPAV register
     */
    void increment_BPAV(unsigned int value);
    /**
     * \brief Increments the value of the rh850 BPAM register by the specified amount
     * \param value Amount by which to increment the value of the rh850 BPAM register
     */
    void increment_BPAM(unsigned int value);
    /**
     * \brief Increments the value of the rh850 BPDV register by the specified amount
     * \param value Amount by which to increment the value of the rh850 BPDV register
     */
    void increment_BPDV(unsigned int value);
    /**
     * \brief Increments the value of the rh850 BPDM register by the specified amount
     * \param value Amount by which to increment the value of the rh850 BPDM register
     */
    void increment_BPDM(unsigned int value);
    /**
     * \brief Increments the value of the rh850 DBWR register by the specified amount
     * \param value Amount by which to increment the value of the rh850 DBWR register
     */
    void increment_DBWR(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPBRGN_MPBRGN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPBRGN_MPBRGN register field
     */
    void increment_MPBRGN_MPBRGN(unsigned char value);
    /**
     * \brief Increments the value of the rh850 MPTRGN_MPTRGN register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPTRGN_MPTRGN register field
     */
    void increment_MPTRGN_MPTRGN(unsigned char value);
    /**
     * \brief Increments the value of the rh850 MPLA0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA0 register
     */
    void increment_MPLA0(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA0 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA0 register
     */
    void increment_MPUA0(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT0_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT0_ASID register field
     */
    void increment_MPAT0_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA1 register
     */
    void increment_MPLA1(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA1 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA1 register
     */
    void increment_MPUA1(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT1_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT1_ASID register field
     */
    void increment_MPAT1_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA2 register
     */
    void increment_MPLA2(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA2 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA2 register
     */
    void increment_MPUA2(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT2_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT2_ASID register field
     */
    void increment_MPAT2_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA3 register
     */
    void increment_MPLA3(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA3 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA3 register
     */
    void increment_MPUA3(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT3_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT3_ASID register field
     */
    void increment_MPAT3_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA4 register
     */
    void increment_MPLA4(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA4 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA4 register
     */
    void increment_MPUA4(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT4_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT4_ASID register field
     */
    void increment_MPAT4_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA5 register
     */
    void increment_MPLA5(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA5 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA5 register
     */
    void increment_MPUA5(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT5_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT5_ASID register field
     */
    void increment_MPAT5_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA6 register
     */
    void increment_MPLA6(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA6 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA6 register
     */
    void increment_MPUA6(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT6_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT6_ASID register field
     */
    void increment_MPAT6_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA7 register
     */
    void increment_MPLA7(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA7 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA7 register
     */
    void increment_MPUA7(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT7_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT7_ASID register field
     */
    void increment_MPAT7_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA8 register
     */
    void increment_MPLA8(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA8 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA8 register
     */
    void increment_MPUA8(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT8_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT8_ASID register field
     */
    void increment_MPAT8_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA9 register
     */
    void increment_MPLA9(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA9 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA9 register
     */
    void increment_MPUA9(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT9_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT9_ASID register field
     */
    void increment_MPAT9_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA10 register
     */
    void increment_MPLA10(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA10 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA10 register
     */
    void increment_MPUA10(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT10_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT10_ASID register field
     */
    void increment_MPAT10_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA11 register
     */
    void increment_MPLA11(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA11 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA11 register
     */
    void increment_MPUA11(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT11_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT11_ASID register field
     */
    void increment_MPAT11_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA12 register
     */
    void increment_MPLA12(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA12 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA12 register
     */
    void increment_MPUA12(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT12_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT12_ASID register field
     */
    void increment_MPAT12_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA13 register
     */
    void increment_MPLA13(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA13 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA13 register
     */
    void increment_MPUA13(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT13_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT13_ASID register field
     */
    void increment_MPAT13_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA14 register
     */
    void increment_MPLA14(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA14 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA14 register
     */
    void increment_MPUA14(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT14_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT14_ASID register field
     */
    void increment_MPAT14_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 MPLA15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPLA15 register
     */
    void increment_MPLA15(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPUA15 register by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPUA15 register
     */
    void increment_MPUA15(unsigned int value);
    /**
     * \brief Increments the value of the rh850 MPAT15_ASID register field by the specified amount
     * \param value Amount by which to increment the value of the rh850 MPAT15_ASID register field
     */
    void increment_MPAT15_ASID(unsigned short value);
    /**
     * \brief Increments the value of the rh850 mpu_entry register by the specified amount
     * \param value Amount by which to increment the value of the rh850 mpu_entry register
     */
    void increment_mpu_entry(unsigned char value);
    /**
     * \brief Increments the value of the rh850 mpat_g_support register by the specified amount
     * \param value Amount by which to increment the value of the rh850 mpat_g_support register
     */
    void increment_mpat_g_support(unsigned char value);
    /**
     * \brief Increments the value of the rh850 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to increment the value of the rh850 swd0pcad_exec_count register
     */
    void increment_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Increments the value of the rh850 swd0pcad_val register by the specified amount
     * \param value Amount by which to increment the value of the rh850 swd0pcad_val register
     */
    void increment_swd0pcad_val(unsigned int value);
    /**
     * \brief Increments the value of the rh850 mem_acc_type register by the specified amount
     * \param value Amount by which to increment the value of the rh850 mem_acc_type register
     */
    void increment_mem_acc_type(unsigned int value);

    // Immediate register group increment operations
    /**
     * \brief Increments the value of the specified register from the rh850 GR register group
     * \param index Index value of the required register from the rh850 GR register group
     * \param value Amount by which to increment the value of the specified rh850 GR group register
     */
    void increment_GR(rh850::GR_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \param value Amount by which to increment the value of the specified rh850 SR_SELID_0 group register
     */
    void increment_SR_SELID_0(rh850::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \param value Amount by which to increment the value of the specified rh850 SR_SELID_1 group register
     */
    void increment_SR_SELID_1(rh850::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \param value Amount by which to increment the value of the specified rh850 SR_SELID_2 group register
     */
    void increment_SR_SELID_2(rh850::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \param value Amount by which to increment the value of the specified rh850 SR_SELID_3 group register
     */
    void increment_SR_SELID_3(rh850::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \param value Amount by which to increment the value of the specified rh850 SR_SELID_5 group register
     */
    void increment_SR_SELID_5(rh850::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850 SR_SELID_6 register group
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \param value Amount by which to increment the value of the specified rh850 SR_SELID_6 group register
     */
    void increment_SR_SELID_6(rh850::SR_SELID_6_index_t index, unsigned int value);
    /**
     * \brief Increments the value of the specified register from the rh850 SR_SELID_7 register group
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \param value Amount by which to increment the value of the specified rh850 SR_SELID_7 group register
     */
    void increment_SR_SELID_7(rh850::SR_SELID_7_index_t index, unsigned int value);

    // Immediate register and register field decrement operations
    /**
     * \brief Decrements the value of the rh850 translation time PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 translation time PC register
     */
    void decrement_trans_PC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r0 register
     */
    void decrement_r0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r1 register
     */
    void decrement_r1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r2 register
     */
    void decrement_r2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r3 register
     */
    void decrement_r3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r4 register
     */
    void decrement_r4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r5 register
     */
    void decrement_r5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r6 register
     */
    void decrement_r6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r7 register
     */
    void decrement_r7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r8 register
     */
    void decrement_r8(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r9 register
     */
    void decrement_r9(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r10 register
     */
    void decrement_r10(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r11 register
     */
    void decrement_r11(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r12 register
     */
    void decrement_r12(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r13 register
     */
    void decrement_r13(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r14 register
     */
    void decrement_r14(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r15 register
     */
    void decrement_r15(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r16 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r16 register
     */
    void decrement_r16(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r17 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r17 register
     */
    void decrement_r17(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r18 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r18 register
     */
    void decrement_r18(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r19 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r19 register
     */
    void decrement_r19(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r20 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r20 register
     */
    void decrement_r20(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r21 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r21 register
     */
    void decrement_r21(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r22 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r22 register
     */
    void decrement_r22(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r23 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r23 register
     */
    void decrement_r23(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r24 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r24 register
     */
    void decrement_r24(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r25 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r25 register
     */
    void decrement_r25(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r26 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r26 register
     */
    void decrement_r26(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r27 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r27 register
     */
    void decrement_r27(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r28 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r28 register
     */
    void decrement_r28(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r29 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r29 register
     */
    void decrement_r29(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r30 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r30 register
     */
    void decrement_r30(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 r31 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 r31 register
     */
    void decrement_r31(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 PC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 PC register
     */
    void decrement_PC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 EIPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 EIPC register
     */
    void decrement_EIPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 EIPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 EIPSW register
     */
    void decrement_EIPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 FEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FEPC register
     */
    void decrement_FEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 FEPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FEPSW register
     */
    void decrement_FEPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 FPSR_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPSR_CC register field
     */
    void decrement_FPSR_CC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPSR_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPSR_RM register field
     */
    void decrement_FPSR_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPSR_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPSR_XC register field
     */
    void decrement_FPSR_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPSR_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPSR_XE register field
     */
    void decrement_FPSR_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPSR_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPSR_XP register field
     */
    void decrement_FPSR_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPEPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPEPC register
     */
    void decrement_FPEPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 FPST_XC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPST_XC register field
     */
    void decrement_FPST_XC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPST_XP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPST_XP register field
     */
    void decrement_FPST_XP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPCC_CC register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPCC_CC register field
     */
    void decrement_FPCC_CC(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPCFG_RM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPCFG_RM register field
     */
    void decrement_FPCFG_RM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPCFG_XE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPCFG_XE register field
     */
    void decrement_FPCFG_XE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 FPEC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPEC register
     */
    void decrement_FPEC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 EIIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 EIIC register
     */
    void decrement_EIIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 FEIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FEIC register
     */
    void decrement_FEIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 CTPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 CTPC register
     */
    void decrement_CTPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 CTPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 CTPSW register
     */
    void decrement_CTPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 CTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 CTBP register
     */
    void decrement_CTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 EIWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 EIWR register
     */
    void decrement_EIWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 FEWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FEWR register
     */
    void decrement_FEWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 BSEL register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 BSEL register
     */
    void decrement_BSEL(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MCFG0_SPID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MCFG0_SPID register field
     */
    void decrement_MCFG0_SPID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 RBASE_RBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 RBASE_RBASE register field
     */
    void decrement_RBASE_RBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 EBASE_EBASE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 EBASE_EBASE register field
     */
    void decrement_EBASE_EBASE(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 INTBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 INTBP register
     */
    void decrement_INTBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MCTL_STID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MCTL_STID register field
     */
    void decrement_MCTL_STID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 PID register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 PID register
     */
    void decrement_PID(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 FPIPR_FPIPR register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 FPIPR_FPIPR register field
     */
    void decrement_FPIPR_FPIPR(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 SCCFG_SIZE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 SCCFG_SIZE register field
     */
    void decrement_SCCFG_SIZE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 SCBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 SCBP register
     */
    void decrement_SCBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 HTCFG0_PEID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 HTCFG0_PEID register field
     */
    void decrement_HTCFG0_PEID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 HTCFG0_TCID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 HTCFG0_TCID register field
     */
    void decrement_HTCFG0_TCID(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 HVCCFG register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 HVCCFG register
     */
    void decrement_HVCCFG(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 HVCBP register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 HVCBP register
     */
    void decrement_HVCBP(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MEA register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MEA register
     */
    void decrement_MEA(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 ASID_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 ASID_ASID register field
     */
    void decrement_ASID_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MEI_REG register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MEI_REG register field
     */
    void decrement_MEI_REG(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 MEI_DS register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MEI_DS register field
     */
    void decrement_MEI_DS(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 MEI_ITYPE register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MEI_ITYPE register field
     */
    void decrement_MEI_ITYPE(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 ISPR_ISP register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 ISPR_ISP register field
     */
    void decrement_ISPR_ISP(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 PMR_PM register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 PMR_PM register field
     */
    void decrement_PMR_PM(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 DBIC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 DBIC register
     */
    void decrement_DBIC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 DBCMC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 DBCMC register
     */
    void decrement_DBCMC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 DBPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 DBPC register
     */
    void decrement_DBPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 DBPSW register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 DBPSW register
     */
    void decrement_DBPSW(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 DIR0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 DIR0 register
     */
    void decrement_DIR0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 DIR1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 DIR1 register
     */
    void decrement_DIR1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 BPC register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 BPC register
     */
    void decrement_BPC(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 BPAV register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 BPAV register
     */
    void decrement_BPAV(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 BPAM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 BPAM register
     */
    void decrement_BPAM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 BPDV register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 BPDV register
     */
    void decrement_BPDV(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 BPDM register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 BPDM register
     */
    void decrement_BPDM(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 DBWR register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 DBWR register
     */
    void decrement_DBWR(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPBRGN_MPBRGN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPBRGN_MPBRGN register field
     */
    void decrement_MPBRGN_MPBRGN(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 MPTRGN_MPTRGN register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPTRGN_MPTRGN register field
     */
    void decrement_MPTRGN_MPTRGN(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 MPLA0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA0 register
     */
    void decrement_MPLA0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA0 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA0 register
     */
    void decrement_MPUA0(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT0_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT0_ASID register field
     */
    void decrement_MPAT0_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA1 register
     */
    void decrement_MPLA1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA1 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA1 register
     */
    void decrement_MPUA1(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT1_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT1_ASID register field
     */
    void decrement_MPAT1_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA2 register
     */
    void decrement_MPLA2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA2 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA2 register
     */
    void decrement_MPUA2(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT2_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT2_ASID register field
     */
    void decrement_MPAT2_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA3 register
     */
    void decrement_MPLA3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA3 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA3 register
     */
    void decrement_MPUA3(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT3_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT3_ASID register field
     */
    void decrement_MPAT3_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA4 register
     */
    void decrement_MPLA4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA4 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA4 register
     */
    void decrement_MPUA4(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT4_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT4_ASID register field
     */
    void decrement_MPAT4_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA5 register
     */
    void decrement_MPLA5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA5 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA5 register
     */
    void decrement_MPUA5(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT5_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT5_ASID register field
     */
    void decrement_MPAT5_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA6 register
     */
    void decrement_MPLA6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA6 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA6 register
     */
    void decrement_MPUA6(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT6_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT6_ASID register field
     */
    void decrement_MPAT6_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA7 register
     */
    void decrement_MPLA7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA7 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA7 register
     */
    void decrement_MPUA7(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT7_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT7_ASID register field
     */
    void decrement_MPAT7_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA8 register
     */
    void decrement_MPLA8(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA8 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA8 register
     */
    void decrement_MPUA8(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT8_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT8_ASID register field
     */
    void decrement_MPAT8_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA9 register
     */
    void decrement_MPLA9(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA9 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA9 register
     */
    void decrement_MPUA9(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT9_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT9_ASID register field
     */
    void decrement_MPAT9_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA10 register
     */
    void decrement_MPLA10(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA10 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA10 register
     */
    void decrement_MPUA10(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT10_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT10_ASID register field
     */
    void decrement_MPAT10_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA11 register
     */
    void decrement_MPLA11(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA11 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA11 register
     */
    void decrement_MPUA11(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT11_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT11_ASID register field
     */
    void decrement_MPAT11_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA12 register
     */
    void decrement_MPLA12(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA12 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA12 register
     */
    void decrement_MPUA12(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT12_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT12_ASID register field
     */
    void decrement_MPAT12_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA13 register
     */
    void decrement_MPLA13(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA13 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA13 register
     */
    void decrement_MPUA13(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT13_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT13_ASID register field
     */
    void decrement_MPAT13_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA14 register
     */
    void decrement_MPLA14(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA14 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA14 register
     */
    void decrement_MPUA14(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT14_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT14_ASID register field
     */
    void decrement_MPAT14_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 MPLA15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPLA15 register
     */
    void decrement_MPLA15(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPUA15 register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPUA15 register
     */
    void decrement_MPUA15(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 MPAT15_ASID register field by the specified amount
     * \param value Amount by which to decrement the value of the rh850 MPAT15_ASID register field
     */
    void decrement_MPAT15_ASID(unsigned short value);
    /**
     * \brief Decrements the value of the rh850 mpu_entry register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 mpu_entry register
     */
    void decrement_mpu_entry(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 mpat_g_support register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 mpat_g_support register
     */
    void decrement_mpat_g_support(unsigned char value);
    /**
     * \brief Decrements the value of the rh850 swd0pcad_exec_count register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 swd0pcad_exec_count register
     */
    void decrement_swd0pcad_exec_count(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 swd0pcad_val register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 swd0pcad_val register
     */
    void decrement_swd0pcad_val(unsigned int value);
    /**
     * \brief Decrements the value of the rh850 mem_acc_type register by the specified amount
     * \param value Amount by which to decrement the value of the rh850 mem_acc_type register
     */
    void decrement_mem_acc_type(unsigned int value);

    // Immediate register group decrement operations
    /**
     * \brief Decrements the value of the specified register from the rh850 GR register group
     * \param index Index value of the required register from the rh850 GR register group
     * \param value Amount by which to decrement the value of the specified rh850 GR group register
     */
    void decrement_GR(rh850::GR_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \param value Amount by which to decrement the value of the specified rh850 SR_SELID_0 group register
     */
    void decrement_SR_SELID_0(rh850::SR_SELID_0_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \param value Amount by which to decrement the value of the specified rh850 SR_SELID_1 group register
     */
    void decrement_SR_SELID_1(rh850::SR_SELID_1_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \param value Amount by which to decrement the value of the specified rh850 SR_SELID_2 group register
     */
    void decrement_SR_SELID_2(rh850::SR_SELID_2_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \param value Amount by which to decrement the value of the specified rh850 SR_SELID_3 group register
     */
    void decrement_SR_SELID_3(rh850::SR_SELID_3_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \param value Amount by which to decrement the value of the specified rh850 SR_SELID_5 group register
     */
    void decrement_SR_SELID_5(rh850::SR_SELID_5_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850 SR_SELID_6 register group
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \param value Amount by which to decrement the value of the specified rh850 SR_SELID_6 group register
     */
    void decrement_SR_SELID_6(rh850::SR_SELID_6_index_t index, unsigned int value);
    /**
     * \brief Decrements the value of the specified register from the rh850 SR_SELID_7 register group
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \param value Amount by which to decrement the value of the specified rh850 SR_SELID_7 group register
     */
    void decrement_SR_SELID_7(rh850::SR_SELID_7_index_t index, unsigned int value);

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
     * \brief Generates code to query the value of the rh850 r0 register
     * \return Object representing the value of the rh850 r0 register
     */
    fastiss::i32_t *gen_read_r0(void);
    /**
     * \brief Generates code to query the value of the rh850 r1 register
     * \return Object representing the value of the rh850 r1 register
     */
    fastiss::i32_t *gen_read_r1(void);
    /**
     * \brief Generates code to query the value of the rh850 r2 register
     * \return Object representing the value of the rh850 r2 register
     */
    fastiss::i32_t *gen_read_r2(void);
    /**
     * \brief Generates code to query the value of the rh850 r3 register
     * \return Object representing the value of the rh850 r3 register
     */
    fastiss::i32_t *gen_read_r3(void);
    /**
     * \brief Generates code to query the value of the rh850 r4 register
     * \return Object representing the value of the rh850 r4 register
     */
    fastiss::i32_t *gen_read_r4(void);
    /**
     * \brief Generates code to query the value of the rh850 r5 register
     * \return Object representing the value of the rh850 r5 register
     */
    fastiss::i32_t *gen_read_r5(void);
    /**
     * \brief Generates code to query the value of the rh850 r6 register
     * \return Object representing the value of the rh850 r6 register
     */
    fastiss::i32_t *gen_read_r6(void);
    /**
     * \brief Generates code to query the value of the rh850 r7 register
     * \return Object representing the value of the rh850 r7 register
     */
    fastiss::i32_t *gen_read_r7(void);
    /**
     * \brief Generates code to query the value of the rh850 r8 register
     * \return Object representing the value of the rh850 r8 register
     */
    fastiss::i32_t *gen_read_r8(void);
    /**
     * \brief Generates code to query the value of the rh850 r9 register
     * \return Object representing the value of the rh850 r9 register
     */
    fastiss::i32_t *gen_read_r9(void);
    /**
     * \brief Generates code to query the value of the rh850 r10 register
     * \return Object representing the value of the rh850 r10 register
     */
    fastiss::i32_t *gen_read_r10(void);
    /**
     * \brief Generates code to query the value of the rh850 r11 register
     * \return Object representing the value of the rh850 r11 register
     */
    fastiss::i32_t *gen_read_r11(void);
    /**
     * \brief Generates code to query the value of the rh850 r12 register
     * \return Object representing the value of the rh850 r12 register
     */
    fastiss::i32_t *gen_read_r12(void);
    /**
     * \brief Generates code to query the value of the rh850 r13 register
     * \return Object representing the value of the rh850 r13 register
     */
    fastiss::i32_t *gen_read_r13(void);
    /**
     * \brief Generates code to query the value of the rh850 r14 register
     * \return Object representing the value of the rh850 r14 register
     */
    fastiss::i32_t *gen_read_r14(void);
    /**
     * \brief Generates code to query the value of the rh850 r15 register
     * \return Object representing the value of the rh850 r15 register
     */
    fastiss::i32_t *gen_read_r15(void);
    /**
     * \brief Generates code to query the value of the rh850 r16 register
     * \return Object representing the value of the rh850 r16 register
     */
    fastiss::i32_t *gen_read_r16(void);
    /**
     * \brief Generates code to query the value of the rh850 r17 register
     * \return Object representing the value of the rh850 r17 register
     */
    fastiss::i32_t *gen_read_r17(void);
    /**
     * \brief Generates code to query the value of the rh850 r18 register
     * \return Object representing the value of the rh850 r18 register
     */
    fastiss::i32_t *gen_read_r18(void);
    /**
     * \brief Generates code to query the value of the rh850 r19 register
     * \return Object representing the value of the rh850 r19 register
     */
    fastiss::i32_t *gen_read_r19(void);
    /**
     * \brief Generates code to query the value of the rh850 r20 register
     * \return Object representing the value of the rh850 r20 register
     */
    fastiss::i32_t *gen_read_r20(void);
    /**
     * \brief Generates code to query the value of the rh850 r21 register
     * \return Object representing the value of the rh850 r21 register
     */
    fastiss::i32_t *gen_read_r21(void);
    /**
     * \brief Generates code to query the value of the rh850 r22 register
     * \return Object representing the value of the rh850 r22 register
     */
    fastiss::i32_t *gen_read_r22(void);
    /**
     * \brief Generates code to query the value of the rh850 r23 register
     * \return Object representing the value of the rh850 r23 register
     */
    fastiss::i32_t *gen_read_r23(void);
    /**
     * \brief Generates code to query the value of the rh850 r24 register
     * \return Object representing the value of the rh850 r24 register
     */
    fastiss::i32_t *gen_read_r24(void);
    /**
     * \brief Generates code to query the value of the rh850 r25 register
     * \return Object representing the value of the rh850 r25 register
     */
    fastiss::i32_t *gen_read_r25(void);
    /**
     * \brief Generates code to query the value of the rh850 r26 register
     * \return Object representing the value of the rh850 r26 register
     */
    fastiss::i32_t *gen_read_r26(void);
    /**
     * \brief Generates code to query the value of the rh850 r27 register
     * \return Object representing the value of the rh850 r27 register
     */
    fastiss::i32_t *gen_read_r27(void);
    /**
     * \brief Generates code to query the value of the rh850 r28 register
     * \return Object representing the value of the rh850 r28 register
     */
    fastiss::i32_t *gen_read_r28(void);
    /**
     * \brief Generates code to query the value of the rh850 r29 register
     * \return Object representing the value of the rh850 r29 register
     */
    fastiss::i32_t *gen_read_r29(void);
    /**
     * \brief Generates code to query the value of the rh850 r30 register
     * \return Object representing the value of the rh850 r30 register
     */
    fastiss::i32_t *gen_read_r30(void);
    /**
     * \brief Generates code to query the value of the rh850 r31 register
     * \return Object representing the value of the rh850 r31 register
     */
    fastiss::i32_t *gen_read_r31(void);
    /**
     * \brief Generates code to query the value of the rh850 PC register
     * \return Object representing the value of the rh850 PC register
     */
    fastiss::i32_t *gen_read_PC(void);
    /**
     * \brief Generates code to query the value of the rh850 EIPC register
     * \return Object representing the value of the rh850 EIPC register
     */
    fastiss::i32_t *gen_read_EIPC(void);
    /**
     * \brief Generates code to query the value of the rh850 EIPSW register
     * \return Object representing the value of the rh850 EIPSW register
     */
    fastiss::i32_t *gen_read_EIPSW(void);
    /**
     * \brief Generates code to query the value of the rh850 FEPC register
     * \return Object representing the value of the rh850 FEPC register
     */
    fastiss::i32_t *gen_read_FEPC(void);
    /**
     * \brief Generates code to query the value of the rh850 FEPSW register
     * \return Object representing the value of the rh850 FEPSW register
     */
    fastiss::i32_t *gen_read_FEPSW(void);
    /**
     * \brief Generates code to query the value of the rh850 PSW register
     * \return Object representing the value of the rh850 PSW register
     */
    fastiss::i32_t *gen_read_PSW(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_Z register field
    * \return Object representing the value of the rh850 PSW_Z register field
    */
    fastiss::i1_t *gen_read_PSW_Z(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_S register field
    * \return Object representing the value of the rh850 PSW_S register field
    */
    fastiss::i1_t *gen_read_PSW_S(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_OV register field
    * \return Object representing the value of the rh850 PSW_OV register field
    */
    fastiss::i1_t *gen_read_PSW_OV(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_CY register field
    * \return Object representing the value of the rh850 PSW_CY register field
    */
    fastiss::i1_t *gen_read_PSW_CY(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_SAT register field
    * \return Object representing the value of the rh850 PSW_SAT register field
    */
    fastiss::i1_t *gen_read_PSW_SAT(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_ID register field
    * \return Object representing the value of the rh850 PSW_ID register field
    */
    fastiss::i1_t *gen_read_PSW_ID(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_EP register field
    * \return Object representing the value of the rh850 PSW_EP register field
    */
    fastiss::i1_t *gen_read_PSW_EP(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_NP register field
    * \return Object representing the value of the rh850 PSW_NP register field
    */
    fastiss::i1_t *gen_read_PSW_NP(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_CM register field
    * \return Object representing the value of the rh850 PSW_CM register field
    */
    fastiss::i1_t *gen_read_PSW_CM(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_SS register field
    * \return Object representing the value of the rh850 PSW_SS register field
    */
    fastiss::i1_t *gen_read_PSW_SS(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_EBV register field
    * \return Object representing the value of the rh850 PSW_EBV register field
    */
    fastiss::i1_t *gen_read_PSW_EBV(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_CU0 register field
    * \return Object representing the value of the rh850 PSW_CU0 register field
    */
    fastiss::i1_t *gen_read_PSW_CU0(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_CU1 register field
    * \return Object representing the value of the rh850 PSW_CU1 register field
    */
    fastiss::i1_t *gen_read_PSW_CU1(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_CU2 register field
    * \return Object representing the value of the rh850 PSW_CU2 register field
    */
    fastiss::i1_t *gen_read_PSW_CU2(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_HVC register field
    * \return Object representing the value of the rh850 PSW_HVC register field
    */
    fastiss::i1_t *gen_read_PSW_HVC(void);
    /**
    * \brief Generates code to query the value of the rh850 PSW_UM register field
    * \return Object representing the value of the rh850 PSW_UM register field
    */
    fastiss::i1_t *gen_read_PSW_UM(void);
    /**
     * \brief Generates code to query the value of the rh850 FPSR register
     * \return Object representing the value of the rh850 FPSR register
     */
    fastiss::i32_t *gen_read_FPSR(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_CC register field
    * \return Object representing the value of the rh850 FPSR_CC register field
    */
    fastiss::i8_t *gen_read_FPSR_CC(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_FN register field
    * \return Object representing the value of the rh850 FPSR_FN register field
    */
    fastiss::i1_t *gen_read_FPSR_FN(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_IF register field
    * \return Object representing the value of the rh850 FPSR_IF register field
    */
    fastiss::i1_t *gen_read_FPSR_IF(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_PEM register field
    * \return Object representing the value of the rh850 FPSR_PEM register field
    */
    fastiss::i1_t *gen_read_FPSR_PEM(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_RM register field
    * \return Object representing the value of the rh850 FPSR_RM register field
    */
    fastiss::i8_t *gen_read_FPSR_RM(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_FS register field
    * \return Object representing the value of the rh850 FPSR_FS register field
    */
    fastiss::i1_t *gen_read_FPSR_FS(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_XC register field
    * \return Object representing the value of the rh850 FPSR_XC register field
    */
    fastiss::i8_t *gen_read_FPSR_XC(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_XE register field
    * \return Object representing the value of the rh850 FPSR_XE register field
    */
    fastiss::i8_t *gen_read_FPSR_XE(void);
    /**
    * \brief Generates code to query the value of the rh850 FPSR_XP register field
    * \return Object representing the value of the rh850 FPSR_XP register field
    */
    fastiss::i8_t *gen_read_FPSR_XP(void);
    /**
     * \brief Generates code to query the value of the rh850 FPEPC register
     * \return Object representing the value of the rh850 FPEPC register
     */
    fastiss::i32_t *gen_read_FPEPC(void);
    /**
     * \brief Generates code to query the value of the rh850 FPST register
     * \return Object representing the value of the rh850 FPST register
     */
    fastiss::i32_t *gen_read_FPST(void);
    /**
    * \brief Generates code to query the value of the rh850 FPST_XC register field
    * \return Object representing the value of the rh850 FPST_XC register field
    */
    fastiss::i8_t *gen_read_FPST_XC(void);
    /**
    * \brief Generates code to query the value of the rh850 FPST_IF register field
    * \return Object representing the value of the rh850 FPST_IF register field
    */
    fastiss::i1_t *gen_read_FPST_IF(void);
    /**
    * \brief Generates code to query the value of the rh850 FPST_XP register field
    * \return Object representing the value of the rh850 FPST_XP register field
    */
    fastiss::i8_t *gen_read_FPST_XP(void);
    /**
     * \brief Generates code to query the value of the rh850 FPCC register
     * \return Object representing the value of the rh850 FPCC register
     */
    fastiss::i32_t *gen_read_FPCC(void);
    /**
    * \brief Generates code to query the value of the rh850 FPCC_CC register field
    * \return Object representing the value of the rh850 FPCC_CC register field
    */
    fastiss::i8_t *gen_read_FPCC_CC(void);
    /**
     * \brief Generates code to query the value of the rh850 FPCFG register
     * \return Object representing the value of the rh850 FPCFG register
     */
    fastiss::i32_t *gen_read_FPCFG(void);
    /**
    * \brief Generates code to query the value of the rh850 FPCFG_RM register field
    * \return Object representing the value of the rh850 FPCFG_RM register field
    */
    fastiss::i8_t *gen_read_FPCFG_RM(void);
    /**
    * \brief Generates code to query the value of the rh850 FPCFG_XE register field
    * \return Object representing the value of the rh850 FPCFG_XE register field
    */
    fastiss::i8_t *gen_read_FPCFG_XE(void);
    /**
     * \brief Generates code to query the value of the rh850 FPEC register
     * \return Object representing the value of the rh850 FPEC register
     */
    fastiss::i32_t *gen_read_FPEC(void);
    /**
     * \brief Generates code to query the value of the rh850 EIIC register
     * \return Object representing the value of the rh850 EIIC register
     */
    fastiss::i32_t *gen_read_EIIC(void);
    /**
     * \brief Generates code to query the value of the rh850 FEIC register
     * \return Object representing the value of the rh850 FEIC register
     */
    fastiss::i32_t *gen_read_FEIC(void);
    /**
     * \brief Generates code to query the value of the rh850 CTPC register
     * \return Object representing the value of the rh850 CTPC register
     */
    fastiss::i32_t *gen_read_CTPC(void);
    /**
     * \brief Generates code to query the value of the rh850 CTPSW register
     * \return Object representing the value of the rh850 CTPSW register
     */
    fastiss::i32_t *gen_read_CTPSW(void);
    /**
     * \brief Generates code to query the value of the rh850 CTBP register
     * \return Object representing the value of the rh850 CTBP register
     */
    fastiss::i32_t *gen_read_CTBP(void);
    /**
     * \brief Generates code to query the value of the rh850 EIWR register
     * \return Object representing the value of the rh850 EIWR register
     */
    fastiss::i32_t *gen_read_EIWR(void);
    /**
     * \brief Generates code to query the value of the rh850 FEWR register
     * \return Object representing the value of the rh850 FEWR register
     */
    fastiss::i32_t *gen_read_FEWR(void);
    /**
     * \brief Generates code to query the value of the rh850 BSEL register
     * \return Object representing the value of the rh850 BSEL register
     */
    fastiss::i32_t *gen_read_BSEL(void);
    /**
     * \brief Generates code to query the value of the rh850 MCFG0 register
     * \return Object representing the value of the rh850 MCFG0 register
     */
    fastiss::i32_t *gen_read_MCFG0(void);
    /**
    * \brief Generates code to query the value of the rh850 MCFG0_SPID register field
    * \return Object representing the value of the rh850 MCFG0_SPID register field
    */
    fastiss::i8_t *gen_read_MCFG0_SPID(void);
    /**
    * \brief Generates code to query the value of the rh850 MCFG0_HVP register field
    * \return Object representing the value of the rh850 MCFG0_HVP register field
    */
    fastiss::i1_t *gen_read_MCFG0_HVP(void);
    /**
     * \brief Generates code to query the value of the rh850 RBASE register
     * \return Object representing the value of the rh850 RBASE register
     */
    fastiss::i32_t *gen_read_RBASE(void);
    /**
    * \brief Generates code to query the value of the rh850 RBASE_RBASE register field
    * \return Object representing the value of the rh850 RBASE_RBASE register field
    */
    fastiss::i32_t *gen_read_RBASE_RBASE(void);
    /**
    * \brief Generates code to query the value of the rh850 RBASE_RINT register field
    * \return Object representing the value of the rh850 RBASE_RINT register field
    */
    fastiss::i1_t *gen_read_RBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850 EBASE register
     * \return Object representing the value of the rh850 EBASE register
     */
    fastiss::i32_t *gen_read_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850 EBASE_EBASE register field
    * \return Object representing the value of the rh850 EBASE_EBASE register field
    */
    fastiss::i32_t *gen_read_EBASE_EBASE(void);
    /**
    * \brief Generates code to query the value of the rh850 EBASE_RINT register field
    * \return Object representing the value of the rh850 EBASE_RINT register field
    */
    fastiss::i1_t *gen_read_EBASE_RINT(void);
    /**
     * \brief Generates code to query the value of the rh850 INTBP register
     * \return Object representing the value of the rh850 INTBP register
     */
    fastiss::i32_t *gen_read_INTBP(void);
    /**
     * \brief Generates code to query the value of the rh850 MCTL register
     * \return Object representing the value of the rh850 MCTL register
     */
    fastiss::i32_t *gen_read_MCTL(void);
    /**
    * \brief Generates code to query the value of the rh850 MCTL_EN register field
    * \return Object representing the value of the rh850 MCTL_EN register field
    */
    fastiss::i1_t *gen_read_MCTL_EN(void);
    /**
    * \brief Generates code to query the value of the rh850 MCTL_MT register field
    * \return Object representing the value of the rh850 MCTL_MT register field
    */
    fastiss::i1_t *gen_read_MCTL_MT(void);
    /**
    * \brief Generates code to query the value of the rh850 MCTL_STID register field
    * \return Object representing the value of the rh850 MCTL_STID register field
    */
    fastiss::i8_t *gen_read_MCTL_STID(void);
    /**
    * \brief Generates code to query the value of the rh850 MCTL_MA register field
    * \return Object representing the value of the rh850 MCTL_MA register field
    */
    fastiss::i1_t *gen_read_MCTL_MA(void);
    /**
    * \brief Generates code to query the value of the rh850 MCTL_UIC register field
    * \return Object representing the value of the rh850 MCTL_UIC register field
    */
    fastiss::i1_t *gen_read_MCTL_UIC(void);
    /**
     * \brief Generates code to query the value of the rh850 PID register
     * \return Object representing the value of the rh850 PID register
     */
    fastiss::i32_t *gen_read_PID(void);
    /**
     * \brief Generates code to query the value of the rh850 FPIPR register
     * \return Object representing the value of the rh850 FPIPR register
     */
    fastiss::i32_t *gen_read_FPIPR(void);
    /**
    * \brief Generates code to query the value of the rh850 FPIPR_FPIPR register field
    * \return Object representing the value of the rh850 FPIPR_FPIPR register field
    */
    fastiss::i8_t *gen_read_FPIPR_FPIPR(void);
    /**
     * \brief Generates code to query the value of the rh850 SCCFG register
     * \return Object representing the value of the rh850 SCCFG register
     */
    fastiss::i32_t *gen_read_SCCFG(void);
    /**
    * \brief Generates code to query the value of the rh850 SCCFG_SIZE register field
    * \return Object representing the value of the rh850 SCCFG_SIZE register field
    */
    fastiss::i8_t *gen_read_SCCFG_SIZE(void);
    /**
     * \brief Generates code to query the value of the rh850 SCBP register
     * \return Object representing the value of the rh850 SCBP register
     */
    fastiss::i32_t *gen_read_SCBP(void);
    /**
     * \brief Generates code to query the value of the rh850 HTCFG0 register
     * \return Object representing the value of the rh850 HTCFG0 register
     */
    fastiss::i32_t *gen_read_HTCFG0(void);
    /**
    * \brief Generates code to query the value of the rh850 HTCFG0_PEID register field
    * \return Object representing the value of the rh850 HTCFG0_PEID register field
    */
    fastiss::i8_t *gen_read_HTCFG0_PEID(void);
    /**
    * \brief Generates code to query the value of the rh850 HTCFG0_NC register field
    * \return Object representing the value of the rh850 HTCFG0_NC register field
    */
    fastiss::i1_t *gen_read_HTCFG0_NC(void);
    /**
    * \brief Generates code to query the value of the rh850 HTCFG0_TCID register field
    * \return Object representing the value of the rh850 HTCFG0_TCID register field
    */
    fastiss::i8_t *gen_read_HTCFG0_TCID(void);
    /**
     * \brief Generates code to query the value of the rh850 HVCCFG register
     * \return Object representing the value of the rh850 HVCCFG register
     */
    fastiss::i32_t *gen_read_HVCCFG(void);
    /**
     * \brief Generates code to query the value of the rh850 HVCBP register
     * \return Object representing the value of the rh850 HVCBP register
     */
    fastiss::i32_t *gen_read_HVCBP(void);
    /**
     * \brief Generates code to query the value of the rh850 MEA register
     * \return Object representing the value of the rh850 MEA register
     */
    fastiss::i32_t *gen_read_MEA(void);
    /**
     * \brief Generates code to query the value of the rh850 ASID register
     * \return Object representing the value of the rh850 ASID register
     */
    fastiss::i32_t *gen_read_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 ASID_ASID register field
    * \return Object representing the value of the rh850 ASID_ASID register field
    */
    fastiss::i16_t *gen_read_ASID_ASID(void);
    /**
     * \brief Generates code to query the value of the rh850 MEI register
     * \return Object representing the value of the rh850 MEI register
     */
    fastiss::i32_t *gen_read_MEI(void);
    /**
    * \brief Generates code to query the value of the rh850 MEI_REG register field
    * \return Object representing the value of the rh850 MEI_REG register field
    */
    fastiss::i8_t *gen_read_MEI_REG(void);
    /**
    * \brief Generates code to query the value of the rh850 MEI_DS register field
    * \return Object representing the value of the rh850 MEI_DS register field
    */
    fastiss::i8_t *gen_read_MEI_DS(void);
    /**
    * \brief Generates code to query the value of the rh850 MEI_U register field
    * \return Object representing the value of the rh850 MEI_U register field
    */
    fastiss::i1_t *gen_read_MEI_U(void);
    /**
    * \brief Generates code to query the value of the rh850 MEI_ITYPE register field
    * \return Object representing the value of the rh850 MEI_ITYPE register field
    */
    fastiss::i8_t *gen_read_MEI_ITYPE(void);
    /**
    * \brief Generates code to query the value of the rh850 MEI_RW register field
    * \return Object representing the value of the rh850 MEI_RW register field
    */
    fastiss::i1_t *gen_read_MEI_RW(void);
    /**
     * \brief Generates code to query the value of the rh850 ISPR register
     * \return Object representing the value of the rh850 ISPR register
     */
    fastiss::i32_t *gen_read_ISPR(void);
    /**
    * \brief Generates code to query the value of the rh850 ISPR_ISP register field
    * \return Object representing the value of the rh850 ISPR_ISP register field
    */
    fastiss::i8_t *gen_read_ISPR_ISP(void);
    /**
     * \brief Generates code to query the value of the rh850 PMR register
     * \return Object representing the value of the rh850 PMR register
     */
    fastiss::i32_t *gen_read_PMR(void);
    /**
    * \brief Generates code to query the value of the rh850 PMR_PM register field
    * \return Object representing the value of the rh850 PMR_PM register field
    */
    fastiss::i8_t *gen_read_PMR_PM(void);
    /**
     * \brief Generates code to query the value of the rh850 ICSR register
     * \return Object representing the value of the rh850 ICSR register
     */
    fastiss::i32_t *gen_read_ICSR(void);
    /**
    * \brief Generates code to query the value of the rh850 ICSR_PMEI register field
    * \return Object representing the value of the rh850 ICSR_PMEI register field
    */
    fastiss::i1_t *gen_read_ICSR_PMEI(void);
    /**
     * \brief Generates code to query the value of the rh850 INTCFG register
     * \return Object representing the value of the rh850 INTCFG register
     */
    fastiss::i32_t *gen_read_INTCFG(void);
    /**
    * \brief Generates code to query the value of the rh850 INTCFG_ISPC register field
    * \return Object representing the value of the rh850 INTCFG_ISPC register field
    */
    fastiss::i1_t *gen_read_INTCFG_ISPC(void);
    /**
     * \brief Generates code to query the value of the rh850 DBIC register
     * \return Object representing the value of the rh850 DBIC register
     */
    fastiss::i32_t *gen_read_DBIC(void);
    /**
     * \brief Generates code to query the value of the rh850 DBCMC register
     * \return Object representing the value of the rh850 DBCMC register
     */
    fastiss::i32_t *gen_read_DBCMC(void);
    /**
     * \brief Generates code to query the value of the rh850 DBPC register
     * \return Object representing the value of the rh850 DBPC register
     */
    fastiss::i32_t *gen_read_DBPC(void);
    /**
     * \brief Generates code to query the value of the rh850 DBPSW register
     * \return Object representing the value of the rh850 DBPSW register
     */
    fastiss::i32_t *gen_read_DBPSW(void);
    /**
     * \brief Generates code to query the value of the rh850 DIR0 register
     * \return Object representing the value of the rh850 DIR0 register
     */
    fastiss::i32_t *gen_read_DIR0(void);
    /**
     * \brief Generates code to query the value of the rh850 DIR1 register
     * \return Object representing the value of the rh850 DIR1 register
     */
    fastiss::i32_t *gen_read_DIR1(void);
    /**
     * \brief Generates code to query the value of the rh850 BPC register
     * \return Object representing the value of the rh850 BPC register
     */
    fastiss::i32_t *gen_read_BPC(void);
    /**
     * \brief Generates code to query the value of the rh850 BPAV register
     * \return Object representing the value of the rh850 BPAV register
     */
    fastiss::i32_t *gen_read_BPAV(void);
    /**
     * \brief Generates code to query the value of the rh850 BPAM register
     * \return Object representing the value of the rh850 BPAM register
     */
    fastiss::i32_t *gen_read_BPAM(void);
    /**
     * \brief Generates code to query the value of the rh850 BPDV register
     * \return Object representing the value of the rh850 BPDV register
     */
    fastiss::i32_t *gen_read_BPDV(void);
    /**
     * \brief Generates code to query the value of the rh850 BPDM register
     * \return Object representing the value of the rh850 BPDM register
     */
    fastiss::i32_t *gen_read_BPDM(void);
    /**
     * \brief Generates code to query the value of the rh850 DBWR register
     * \return Object representing the value of the rh850 DBWR register
     */
    fastiss::i32_t *gen_read_DBWR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPM register
     * \return Object representing the value of the rh850 MPM register
     */
    fastiss::i32_t *gen_read_MPM(void);
    /**
    * \brief Generates code to query the value of the rh850 MPM_DX register field
    * \return Object representing the value of the rh850 MPM_DX register field
    */
    fastiss::i1_t *gen_read_MPM_DX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPM_DW register field
    * \return Object representing the value of the rh850 MPM_DW register field
    */
    fastiss::i1_t *gen_read_MPM_DW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPM_DR register field
    * \return Object representing the value of the rh850 MPM_DR register field
    */
    fastiss::i1_t *gen_read_MPM_DR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPM_SVP register field
    * \return Object representing the value of the rh850 MPM_SVP register field
    */
    fastiss::i1_t *gen_read_MPM_SVP(void);
    /**
    * \brief Generates code to query the value of the rh850 MPM_MPE register field
    * \return Object representing the value of the rh850 MPM_MPE register field
    */
    fastiss::i1_t *gen_read_MPM_MPE(void);
    /**
     * \brief Generates code to query the value of the rh850 MPRC register
     * \return Object representing the value of the rh850 MPRC register
     */
    fastiss::i32_t *gen_read_MPRC(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E15 register field
    * \return Object representing the value of the rh850 MPRC_E15 register field
    */
    fastiss::i1_t *gen_read_MPRC_E15(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E14 register field
    * \return Object representing the value of the rh850 MPRC_E14 register field
    */
    fastiss::i1_t *gen_read_MPRC_E14(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E13 register field
    * \return Object representing the value of the rh850 MPRC_E13 register field
    */
    fastiss::i1_t *gen_read_MPRC_E13(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E12 register field
    * \return Object representing the value of the rh850 MPRC_E12 register field
    */
    fastiss::i1_t *gen_read_MPRC_E12(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E11 register field
    * \return Object representing the value of the rh850 MPRC_E11 register field
    */
    fastiss::i1_t *gen_read_MPRC_E11(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E10 register field
    * \return Object representing the value of the rh850 MPRC_E10 register field
    */
    fastiss::i1_t *gen_read_MPRC_E10(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E9 register field
    * \return Object representing the value of the rh850 MPRC_E9 register field
    */
    fastiss::i1_t *gen_read_MPRC_E9(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E8 register field
    * \return Object representing the value of the rh850 MPRC_E8 register field
    */
    fastiss::i1_t *gen_read_MPRC_E8(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E7 register field
    * \return Object representing the value of the rh850 MPRC_E7 register field
    */
    fastiss::i1_t *gen_read_MPRC_E7(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E6 register field
    * \return Object representing the value of the rh850 MPRC_E6 register field
    */
    fastiss::i1_t *gen_read_MPRC_E6(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E5 register field
    * \return Object representing the value of the rh850 MPRC_E5 register field
    */
    fastiss::i1_t *gen_read_MPRC_E5(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E4 register field
    * \return Object representing the value of the rh850 MPRC_E4 register field
    */
    fastiss::i1_t *gen_read_MPRC_E4(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E3 register field
    * \return Object representing the value of the rh850 MPRC_E3 register field
    */
    fastiss::i1_t *gen_read_MPRC_E3(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E2 register field
    * \return Object representing the value of the rh850 MPRC_E2 register field
    */
    fastiss::i1_t *gen_read_MPRC_E2(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E1 register field
    * \return Object representing the value of the rh850 MPRC_E1 register field
    */
    fastiss::i1_t *gen_read_MPRC_E1(void);
    /**
    * \brief Generates code to query the value of the rh850 MPRC_E0 register field
    * \return Object representing the value of the rh850 MPRC_E0 register field
    */
    fastiss::i1_t *gen_read_MPRC_E0(void);
    /**
     * \brief Generates code to query the value of the rh850 MPBRGN register
     * \return Object representing the value of the rh850 MPBRGN register
     */
    fastiss::i32_t *gen_read_MPBRGN(void);
    /**
    * \brief Generates code to query the value of the rh850 MPBRGN_MPBRGN register field
    * \return Object representing the value of the rh850 MPBRGN_MPBRGN register field
    */
    fastiss::i8_t *gen_read_MPBRGN_MPBRGN(void);
    /**
     * \brief Generates code to query the value of the rh850 MPTRGN register
     * \return Object representing the value of the rh850 MPTRGN register
     */
    fastiss::i32_t *gen_read_MPTRGN(void);
    /**
    * \brief Generates code to query the value of the rh850 MPTRGN_MPTRGN register field
    * \return Object representing the value of the rh850 MPTRGN_MPTRGN register field
    */
    fastiss::i8_t *gen_read_MPTRGN_MPTRGN(void);
    /**
     * \brief Generates code to query the value of the rh850 MCR register
     * \return Object representing the value of the rh850 MCR register
     */
    fastiss::i32_t *gen_read_MCR(void);
    /**
    * \brief Generates code to query the value of the rh850 MCR_SXE register field
    * \return Object representing the value of the rh850 MCR_SXE register field
    */
    fastiss::i1_t *gen_read_MCR_SXE(void);
    /**
    * \brief Generates code to query the value of the rh850 MCR_SWE register field
    * \return Object representing the value of the rh850 MCR_SWE register field
    */
    fastiss::i1_t *gen_read_MCR_SWE(void);
    /**
    * \brief Generates code to query the value of the rh850 MCR_SRE register field
    * \return Object representing the value of the rh850 MCR_SRE register field
    */
    fastiss::i1_t *gen_read_MCR_SRE(void);
    /**
    * \brief Generates code to query the value of the rh850 MCR_UXE register field
    * \return Object representing the value of the rh850 MCR_UXE register field
    */
    fastiss::i1_t *gen_read_MCR_UXE(void);
    /**
    * \brief Generates code to query the value of the rh850 MCR_UWE register field
    * \return Object representing the value of the rh850 MCR_UWE register field
    */
    fastiss::i1_t *gen_read_MCR_UWE(void);
    /**
    * \brief Generates code to query the value of the rh850 MCR_URE register field
    * \return Object representing the value of the rh850 MCR_URE register field
    */
    fastiss::i1_t *gen_read_MCR_URE(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA0 register
     * \return Object representing the value of the rh850 MPLA0 register
     */
    fastiss::i32_t *gen_read_MPLA0(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA0 register
     * \return Object representing the value of the rh850 MPUA0 register
     */
    fastiss::i32_t *gen_read_MPUA0(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT0 register
     * \return Object representing the value of the rh850 MPAT0 register
     */
    fastiss::i32_t *gen_read_MPAT0(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_ASID register field
    * \return Object representing the value of the rh850 MPAT0_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT0_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_E register field
    * \return Object representing the value of the rh850 MPAT0_E register field
    */
    fastiss::i1_t *gen_read_MPAT0_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_G register field
    * \return Object representing the value of the rh850 MPAT0_G register field
    */
    fastiss::i1_t *gen_read_MPAT0_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_SX register field
    * \return Object representing the value of the rh850 MPAT0_SX register field
    */
    fastiss::i1_t *gen_read_MPAT0_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_SW register field
    * \return Object representing the value of the rh850 MPAT0_SW register field
    */
    fastiss::i1_t *gen_read_MPAT0_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_SR register field
    * \return Object representing the value of the rh850 MPAT0_SR register field
    */
    fastiss::i1_t *gen_read_MPAT0_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_UX register field
    * \return Object representing the value of the rh850 MPAT0_UX register field
    */
    fastiss::i1_t *gen_read_MPAT0_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_UW register field
    * \return Object representing the value of the rh850 MPAT0_UW register field
    */
    fastiss::i1_t *gen_read_MPAT0_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT0_UR register field
    * \return Object representing the value of the rh850 MPAT0_UR register field
    */
    fastiss::i1_t *gen_read_MPAT0_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA1 register
     * \return Object representing the value of the rh850 MPLA1 register
     */
    fastiss::i32_t *gen_read_MPLA1(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA1 register
     * \return Object representing the value of the rh850 MPUA1 register
     */
    fastiss::i32_t *gen_read_MPUA1(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT1 register
     * \return Object representing the value of the rh850 MPAT1 register
     */
    fastiss::i32_t *gen_read_MPAT1(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_ASID register field
    * \return Object representing the value of the rh850 MPAT1_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT1_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_E register field
    * \return Object representing the value of the rh850 MPAT1_E register field
    */
    fastiss::i1_t *gen_read_MPAT1_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_G register field
    * \return Object representing the value of the rh850 MPAT1_G register field
    */
    fastiss::i1_t *gen_read_MPAT1_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_SX register field
    * \return Object representing the value of the rh850 MPAT1_SX register field
    */
    fastiss::i1_t *gen_read_MPAT1_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_SW register field
    * \return Object representing the value of the rh850 MPAT1_SW register field
    */
    fastiss::i1_t *gen_read_MPAT1_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_SR register field
    * \return Object representing the value of the rh850 MPAT1_SR register field
    */
    fastiss::i1_t *gen_read_MPAT1_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_UX register field
    * \return Object representing the value of the rh850 MPAT1_UX register field
    */
    fastiss::i1_t *gen_read_MPAT1_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_UW register field
    * \return Object representing the value of the rh850 MPAT1_UW register field
    */
    fastiss::i1_t *gen_read_MPAT1_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT1_UR register field
    * \return Object representing the value of the rh850 MPAT1_UR register field
    */
    fastiss::i1_t *gen_read_MPAT1_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA2 register
     * \return Object representing the value of the rh850 MPLA2 register
     */
    fastiss::i32_t *gen_read_MPLA2(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA2 register
     * \return Object representing the value of the rh850 MPUA2 register
     */
    fastiss::i32_t *gen_read_MPUA2(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT2 register
     * \return Object representing the value of the rh850 MPAT2 register
     */
    fastiss::i32_t *gen_read_MPAT2(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_ASID register field
    * \return Object representing the value of the rh850 MPAT2_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT2_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_E register field
    * \return Object representing the value of the rh850 MPAT2_E register field
    */
    fastiss::i1_t *gen_read_MPAT2_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_G register field
    * \return Object representing the value of the rh850 MPAT2_G register field
    */
    fastiss::i1_t *gen_read_MPAT2_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_SX register field
    * \return Object representing the value of the rh850 MPAT2_SX register field
    */
    fastiss::i1_t *gen_read_MPAT2_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_SW register field
    * \return Object representing the value of the rh850 MPAT2_SW register field
    */
    fastiss::i1_t *gen_read_MPAT2_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_SR register field
    * \return Object representing the value of the rh850 MPAT2_SR register field
    */
    fastiss::i1_t *gen_read_MPAT2_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_UX register field
    * \return Object representing the value of the rh850 MPAT2_UX register field
    */
    fastiss::i1_t *gen_read_MPAT2_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_UW register field
    * \return Object representing the value of the rh850 MPAT2_UW register field
    */
    fastiss::i1_t *gen_read_MPAT2_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT2_UR register field
    * \return Object representing the value of the rh850 MPAT2_UR register field
    */
    fastiss::i1_t *gen_read_MPAT2_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA3 register
     * \return Object representing the value of the rh850 MPLA3 register
     */
    fastiss::i32_t *gen_read_MPLA3(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA3 register
     * \return Object representing the value of the rh850 MPUA3 register
     */
    fastiss::i32_t *gen_read_MPUA3(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT3 register
     * \return Object representing the value of the rh850 MPAT3 register
     */
    fastiss::i32_t *gen_read_MPAT3(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_ASID register field
    * \return Object representing the value of the rh850 MPAT3_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT3_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_E register field
    * \return Object representing the value of the rh850 MPAT3_E register field
    */
    fastiss::i1_t *gen_read_MPAT3_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_G register field
    * \return Object representing the value of the rh850 MPAT3_G register field
    */
    fastiss::i1_t *gen_read_MPAT3_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_SX register field
    * \return Object representing the value of the rh850 MPAT3_SX register field
    */
    fastiss::i1_t *gen_read_MPAT3_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_SW register field
    * \return Object representing the value of the rh850 MPAT3_SW register field
    */
    fastiss::i1_t *gen_read_MPAT3_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_SR register field
    * \return Object representing the value of the rh850 MPAT3_SR register field
    */
    fastiss::i1_t *gen_read_MPAT3_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_UX register field
    * \return Object representing the value of the rh850 MPAT3_UX register field
    */
    fastiss::i1_t *gen_read_MPAT3_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_UW register field
    * \return Object representing the value of the rh850 MPAT3_UW register field
    */
    fastiss::i1_t *gen_read_MPAT3_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT3_UR register field
    * \return Object representing the value of the rh850 MPAT3_UR register field
    */
    fastiss::i1_t *gen_read_MPAT3_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA4 register
     * \return Object representing the value of the rh850 MPLA4 register
     */
    fastiss::i32_t *gen_read_MPLA4(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA4 register
     * \return Object representing the value of the rh850 MPUA4 register
     */
    fastiss::i32_t *gen_read_MPUA4(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT4 register
     * \return Object representing the value of the rh850 MPAT4 register
     */
    fastiss::i32_t *gen_read_MPAT4(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_ASID register field
    * \return Object representing the value of the rh850 MPAT4_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT4_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_E register field
    * \return Object representing the value of the rh850 MPAT4_E register field
    */
    fastiss::i1_t *gen_read_MPAT4_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_G register field
    * \return Object representing the value of the rh850 MPAT4_G register field
    */
    fastiss::i1_t *gen_read_MPAT4_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_SX register field
    * \return Object representing the value of the rh850 MPAT4_SX register field
    */
    fastiss::i1_t *gen_read_MPAT4_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_SW register field
    * \return Object representing the value of the rh850 MPAT4_SW register field
    */
    fastiss::i1_t *gen_read_MPAT4_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_SR register field
    * \return Object representing the value of the rh850 MPAT4_SR register field
    */
    fastiss::i1_t *gen_read_MPAT4_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_UX register field
    * \return Object representing the value of the rh850 MPAT4_UX register field
    */
    fastiss::i1_t *gen_read_MPAT4_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_UW register field
    * \return Object representing the value of the rh850 MPAT4_UW register field
    */
    fastiss::i1_t *gen_read_MPAT4_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT4_UR register field
    * \return Object representing the value of the rh850 MPAT4_UR register field
    */
    fastiss::i1_t *gen_read_MPAT4_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA5 register
     * \return Object representing the value of the rh850 MPLA5 register
     */
    fastiss::i32_t *gen_read_MPLA5(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA5 register
     * \return Object representing the value of the rh850 MPUA5 register
     */
    fastiss::i32_t *gen_read_MPUA5(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT5 register
     * \return Object representing the value of the rh850 MPAT5 register
     */
    fastiss::i32_t *gen_read_MPAT5(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_ASID register field
    * \return Object representing the value of the rh850 MPAT5_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT5_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_E register field
    * \return Object representing the value of the rh850 MPAT5_E register field
    */
    fastiss::i1_t *gen_read_MPAT5_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_G register field
    * \return Object representing the value of the rh850 MPAT5_G register field
    */
    fastiss::i1_t *gen_read_MPAT5_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_SX register field
    * \return Object representing the value of the rh850 MPAT5_SX register field
    */
    fastiss::i1_t *gen_read_MPAT5_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_SW register field
    * \return Object representing the value of the rh850 MPAT5_SW register field
    */
    fastiss::i1_t *gen_read_MPAT5_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_SR register field
    * \return Object representing the value of the rh850 MPAT5_SR register field
    */
    fastiss::i1_t *gen_read_MPAT5_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_UX register field
    * \return Object representing the value of the rh850 MPAT5_UX register field
    */
    fastiss::i1_t *gen_read_MPAT5_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_UW register field
    * \return Object representing the value of the rh850 MPAT5_UW register field
    */
    fastiss::i1_t *gen_read_MPAT5_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT5_UR register field
    * \return Object representing the value of the rh850 MPAT5_UR register field
    */
    fastiss::i1_t *gen_read_MPAT5_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA6 register
     * \return Object representing the value of the rh850 MPLA6 register
     */
    fastiss::i32_t *gen_read_MPLA6(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA6 register
     * \return Object representing the value of the rh850 MPUA6 register
     */
    fastiss::i32_t *gen_read_MPUA6(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT6 register
     * \return Object representing the value of the rh850 MPAT6 register
     */
    fastiss::i32_t *gen_read_MPAT6(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_ASID register field
    * \return Object representing the value of the rh850 MPAT6_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT6_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_E register field
    * \return Object representing the value of the rh850 MPAT6_E register field
    */
    fastiss::i1_t *gen_read_MPAT6_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_G register field
    * \return Object representing the value of the rh850 MPAT6_G register field
    */
    fastiss::i1_t *gen_read_MPAT6_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_SX register field
    * \return Object representing the value of the rh850 MPAT6_SX register field
    */
    fastiss::i1_t *gen_read_MPAT6_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_SW register field
    * \return Object representing the value of the rh850 MPAT6_SW register field
    */
    fastiss::i1_t *gen_read_MPAT6_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_SR register field
    * \return Object representing the value of the rh850 MPAT6_SR register field
    */
    fastiss::i1_t *gen_read_MPAT6_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_UX register field
    * \return Object representing the value of the rh850 MPAT6_UX register field
    */
    fastiss::i1_t *gen_read_MPAT6_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_UW register field
    * \return Object representing the value of the rh850 MPAT6_UW register field
    */
    fastiss::i1_t *gen_read_MPAT6_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT6_UR register field
    * \return Object representing the value of the rh850 MPAT6_UR register field
    */
    fastiss::i1_t *gen_read_MPAT6_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA7 register
     * \return Object representing the value of the rh850 MPLA7 register
     */
    fastiss::i32_t *gen_read_MPLA7(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA7 register
     * \return Object representing the value of the rh850 MPUA7 register
     */
    fastiss::i32_t *gen_read_MPUA7(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT7 register
     * \return Object representing the value of the rh850 MPAT7 register
     */
    fastiss::i32_t *gen_read_MPAT7(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_ASID register field
    * \return Object representing the value of the rh850 MPAT7_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT7_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_E register field
    * \return Object representing the value of the rh850 MPAT7_E register field
    */
    fastiss::i1_t *gen_read_MPAT7_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_G register field
    * \return Object representing the value of the rh850 MPAT7_G register field
    */
    fastiss::i1_t *gen_read_MPAT7_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_SX register field
    * \return Object representing the value of the rh850 MPAT7_SX register field
    */
    fastiss::i1_t *gen_read_MPAT7_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_SW register field
    * \return Object representing the value of the rh850 MPAT7_SW register field
    */
    fastiss::i1_t *gen_read_MPAT7_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_SR register field
    * \return Object representing the value of the rh850 MPAT7_SR register field
    */
    fastiss::i1_t *gen_read_MPAT7_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_UX register field
    * \return Object representing the value of the rh850 MPAT7_UX register field
    */
    fastiss::i1_t *gen_read_MPAT7_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_UW register field
    * \return Object representing the value of the rh850 MPAT7_UW register field
    */
    fastiss::i1_t *gen_read_MPAT7_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT7_UR register field
    * \return Object representing the value of the rh850 MPAT7_UR register field
    */
    fastiss::i1_t *gen_read_MPAT7_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA8 register
     * \return Object representing the value of the rh850 MPLA8 register
     */
    fastiss::i32_t *gen_read_MPLA8(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA8 register
     * \return Object representing the value of the rh850 MPUA8 register
     */
    fastiss::i32_t *gen_read_MPUA8(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT8 register
     * \return Object representing the value of the rh850 MPAT8 register
     */
    fastiss::i32_t *gen_read_MPAT8(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_ASID register field
    * \return Object representing the value of the rh850 MPAT8_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT8_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_E register field
    * \return Object representing the value of the rh850 MPAT8_E register field
    */
    fastiss::i1_t *gen_read_MPAT8_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_G register field
    * \return Object representing the value of the rh850 MPAT8_G register field
    */
    fastiss::i1_t *gen_read_MPAT8_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_SX register field
    * \return Object representing the value of the rh850 MPAT8_SX register field
    */
    fastiss::i1_t *gen_read_MPAT8_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_SW register field
    * \return Object representing the value of the rh850 MPAT8_SW register field
    */
    fastiss::i1_t *gen_read_MPAT8_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_SR register field
    * \return Object representing the value of the rh850 MPAT8_SR register field
    */
    fastiss::i1_t *gen_read_MPAT8_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_UX register field
    * \return Object representing the value of the rh850 MPAT8_UX register field
    */
    fastiss::i1_t *gen_read_MPAT8_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_UW register field
    * \return Object representing the value of the rh850 MPAT8_UW register field
    */
    fastiss::i1_t *gen_read_MPAT8_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT8_UR register field
    * \return Object representing the value of the rh850 MPAT8_UR register field
    */
    fastiss::i1_t *gen_read_MPAT8_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA9 register
     * \return Object representing the value of the rh850 MPLA9 register
     */
    fastiss::i32_t *gen_read_MPLA9(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA9 register
     * \return Object representing the value of the rh850 MPUA9 register
     */
    fastiss::i32_t *gen_read_MPUA9(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT9 register
     * \return Object representing the value of the rh850 MPAT9 register
     */
    fastiss::i32_t *gen_read_MPAT9(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_ASID register field
    * \return Object representing the value of the rh850 MPAT9_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT9_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_E register field
    * \return Object representing the value of the rh850 MPAT9_E register field
    */
    fastiss::i1_t *gen_read_MPAT9_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_G register field
    * \return Object representing the value of the rh850 MPAT9_G register field
    */
    fastiss::i1_t *gen_read_MPAT9_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_SX register field
    * \return Object representing the value of the rh850 MPAT9_SX register field
    */
    fastiss::i1_t *gen_read_MPAT9_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_SW register field
    * \return Object representing the value of the rh850 MPAT9_SW register field
    */
    fastiss::i1_t *gen_read_MPAT9_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_SR register field
    * \return Object representing the value of the rh850 MPAT9_SR register field
    */
    fastiss::i1_t *gen_read_MPAT9_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_UX register field
    * \return Object representing the value of the rh850 MPAT9_UX register field
    */
    fastiss::i1_t *gen_read_MPAT9_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_UW register field
    * \return Object representing the value of the rh850 MPAT9_UW register field
    */
    fastiss::i1_t *gen_read_MPAT9_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT9_UR register field
    * \return Object representing the value of the rh850 MPAT9_UR register field
    */
    fastiss::i1_t *gen_read_MPAT9_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA10 register
     * \return Object representing the value of the rh850 MPLA10 register
     */
    fastiss::i32_t *gen_read_MPLA10(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA10 register
     * \return Object representing the value of the rh850 MPUA10 register
     */
    fastiss::i32_t *gen_read_MPUA10(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT10 register
     * \return Object representing the value of the rh850 MPAT10 register
     */
    fastiss::i32_t *gen_read_MPAT10(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_ASID register field
    * \return Object representing the value of the rh850 MPAT10_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT10_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_E register field
    * \return Object representing the value of the rh850 MPAT10_E register field
    */
    fastiss::i1_t *gen_read_MPAT10_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_G register field
    * \return Object representing the value of the rh850 MPAT10_G register field
    */
    fastiss::i1_t *gen_read_MPAT10_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_SX register field
    * \return Object representing the value of the rh850 MPAT10_SX register field
    */
    fastiss::i1_t *gen_read_MPAT10_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_SW register field
    * \return Object representing the value of the rh850 MPAT10_SW register field
    */
    fastiss::i1_t *gen_read_MPAT10_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_SR register field
    * \return Object representing the value of the rh850 MPAT10_SR register field
    */
    fastiss::i1_t *gen_read_MPAT10_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_UX register field
    * \return Object representing the value of the rh850 MPAT10_UX register field
    */
    fastiss::i1_t *gen_read_MPAT10_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_UW register field
    * \return Object representing the value of the rh850 MPAT10_UW register field
    */
    fastiss::i1_t *gen_read_MPAT10_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT10_UR register field
    * \return Object representing the value of the rh850 MPAT10_UR register field
    */
    fastiss::i1_t *gen_read_MPAT10_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA11 register
     * \return Object representing the value of the rh850 MPLA11 register
     */
    fastiss::i32_t *gen_read_MPLA11(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA11 register
     * \return Object representing the value of the rh850 MPUA11 register
     */
    fastiss::i32_t *gen_read_MPUA11(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT11 register
     * \return Object representing the value of the rh850 MPAT11 register
     */
    fastiss::i32_t *gen_read_MPAT11(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_ASID register field
    * \return Object representing the value of the rh850 MPAT11_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT11_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_E register field
    * \return Object representing the value of the rh850 MPAT11_E register field
    */
    fastiss::i1_t *gen_read_MPAT11_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_G register field
    * \return Object representing the value of the rh850 MPAT11_G register field
    */
    fastiss::i1_t *gen_read_MPAT11_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_SX register field
    * \return Object representing the value of the rh850 MPAT11_SX register field
    */
    fastiss::i1_t *gen_read_MPAT11_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_SW register field
    * \return Object representing the value of the rh850 MPAT11_SW register field
    */
    fastiss::i1_t *gen_read_MPAT11_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_SR register field
    * \return Object representing the value of the rh850 MPAT11_SR register field
    */
    fastiss::i1_t *gen_read_MPAT11_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_UX register field
    * \return Object representing the value of the rh850 MPAT11_UX register field
    */
    fastiss::i1_t *gen_read_MPAT11_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_UW register field
    * \return Object representing the value of the rh850 MPAT11_UW register field
    */
    fastiss::i1_t *gen_read_MPAT11_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT11_UR register field
    * \return Object representing the value of the rh850 MPAT11_UR register field
    */
    fastiss::i1_t *gen_read_MPAT11_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA12 register
     * \return Object representing the value of the rh850 MPLA12 register
     */
    fastiss::i32_t *gen_read_MPLA12(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA12 register
     * \return Object representing the value of the rh850 MPUA12 register
     */
    fastiss::i32_t *gen_read_MPUA12(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT12 register
     * \return Object representing the value of the rh850 MPAT12 register
     */
    fastiss::i32_t *gen_read_MPAT12(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_ASID register field
    * \return Object representing the value of the rh850 MPAT12_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT12_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_E register field
    * \return Object representing the value of the rh850 MPAT12_E register field
    */
    fastiss::i1_t *gen_read_MPAT12_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_G register field
    * \return Object representing the value of the rh850 MPAT12_G register field
    */
    fastiss::i1_t *gen_read_MPAT12_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_SX register field
    * \return Object representing the value of the rh850 MPAT12_SX register field
    */
    fastiss::i1_t *gen_read_MPAT12_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_SW register field
    * \return Object representing the value of the rh850 MPAT12_SW register field
    */
    fastiss::i1_t *gen_read_MPAT12_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_SR register field
    * \return Object representing the value of the rh850 MPAT12_SR register field
    */
    fastiss::i1_t *gen_read_MPAT12_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_UX register field
    * \return Object representing the value of the rh850 MPAT12_UX register field
    */
    fastiss::i1_t *gen_read_MPAT12_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_UW register field
    * \return Object representing the value of the rh850 MPAT12_UW register field
    */
    fastiss::i1_t *gen_read_MPAT12_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT12_UR register field
    * \return Object representing the value of the rh850 MPAT12_UR register field
    */
    fastiss::i1_t *gen_read_MPAT12_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA13 register
     * \return Object representing the value of the rh850 MPLA13 register
     */
    fastiss::i32_t *gen_read_MPLA13(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA13 register
     * \return Object representing the value of the rh850 MPUA13 register
     */
    fastiss::i32_t *gen_read_MPUA13(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT13 register
     * \return Object representing the value of the rh850 MPAT13 register
     */
    fastiss::i32_t *gen_read_MPAT13(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_ASID register field
    * \return Object representing the value of the rh850 MPAT13_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT13_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_E register field
    * \return Object representing the value of the rh850 MPAT13_E register field
    */
    fastiss::i1_t *gen_read_MPAT13_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_G register field
    * \return Object representing the value of the rh850 MPAT13_G register field
    */
    fastiss::i1_t *gen_read_MPAT13_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_SX register field
    * \return Object representing the value of the rh850 MPAT13_SX register field
    */
    fastiss::i1_t *gen_read_MPAT13_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_SW register field
    * \return Object representing the value of the rh850 MPAT13_SW register field
    */
    fastiss::i1_t *gen_read_MPAT13_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_SR register field
    * \return Object representing the value of the rh850 MPAT13_SR register field
    */
    fastiss::i1_t *gen_read_MPAT13_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_UX register field
    * \return Object representing the value of the rh850 MPAT13_UX register field
    */
    fastiss::i1_t *gen_read_MPAT13_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_UW register field
    * \return Object representing the value of the rh850 MPAT13_UW register field
    */
    fastiss::i1_t *gen_read_MPAT13_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT13_UR register field
    * \return Object representing the value of the rh850 MPAT13_UR register field
    */
    fastiss::i1_t *gen_read_MPAT13_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA14 register
     * \return Object representing the value of the rh850 MPLA14 register
     */
    fastiss::i32_t *gen_read_MPLA14(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA14 register
     * \return Object representing the value of the rh850 MPUA14 register
     */
    fastiss::i32_t *gen_read_MPUA14(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT14 register
     * \return Object representing the value of the rh850 MPAT14 register
     */
    fastiss::i32_t *gen_read_MPAT14(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_ASID register field
    * \return Object representing the value of the rh850 MPAT14_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT14_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_E register field
    * \return Object representing the value of the rh850 MPAT14_E register field
    */
    fastiss::i1_t *gen_read_MPAT14_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_G register field
    * \return Object representing the value of the rh850 MPAT14_G register field
    */
    fastiss::i1_t *gen_read_MPAT14_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_SX register field
    * \return Object representing the value of the rh850 MPAT14_SX register field
    */
    fastiss::i1_t *gen_read_MPAT14_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_SW register field
    * \return Object representing the value of the rh850 MPAT14_SW register field
    */
    fastiss::i1_t *gen_read_MPAT14_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_SR register field
    * \return Object representing the value of the rh850 MPAT14_SR register field
    */
    fastiss::i1_t *gen_read_MPAT14_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_UX register field
    * \return Object representing the value of the rh850 MPAT14_UX register field
    */
    fastiss::i1_t *gen_read_MPAT14_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_UW register field
    * \return Object representing the value of the rh850 MPAT14_UW register field
    */
    fastiss::i1_t *gen_read_MPAT14_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT14_UR register field
    * \return Object representing the value of the rh850 MPAT14_UR register field
    */
    fastiss::i1_t *gen_read_MPAT14_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 MPLA15 register
     * \return Object representing the value of the rh850 MPLA15 register
     */
    fastiss::i32_t *gen_read_MPLA15(void);
    /**
     * \brief Generates code to query the value of the rh850 MPUA15 register
     * \return Object representing the value of the rh850 MPUA15 register
     */
    fastiss::i32_t *gen_read_MPUA15(void);
    /**
     * \brief Generates code to query the value of the rh850 MPAT15 register
     * \return Object representing the value of the rh850 MPAT15 register
     */
    fastiss::i32_t *gen_read_MPAT15(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_ASID register field
    * \return Object representing the value of the rh850 MPAT15_ASID register field
    */
    fastiss::i16_t *gen_read_MPAT15_ASID(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_E register field
    * \return Object representing the value of the rh850 MPAT15_E register field
    */
    fastiss::i1_t *gen_read_MPAT15_E(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_G register field
    * \return Object representing the value of the rh850 MPAT15_G register field
    */
    fastiss::i1_t *gen_read_MPAT15_G(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_SX register field
    * \return Object representing the value of the rh850 MPAT15_SX register field
    */
    fastiss::i1_t *gen_read_MPAT15_SX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_SW register field
    * \return Object representing the value of the rh850 MPAT15_SW register field
    */
    fastiss::i1_t *gen_read_MPAT15_SW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_SR register field
    * \return Object representing the value of the rh850 MPAT15_SR register field
    */
    fastiss::i1_t *gen_read_MPAT15_SR(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_UX register field
    * \return Object representing the value of the rh850 MPAT15_UX register field
    */
    fastiss::i1_t *gen_read_MPAT15_UX(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_UW register field
    * \return Object representing the value of the rh850 MPAT15_UW register field
    */
    fastiss::i1_t *gen_read_MPAT15_UW(void);
    /**
    * \brief Generates code to query the value of the rh850 MPAT15_UR register field
    * \return Object representing the value of the rh850 MPAT15_UR register field
    */
    fastiss::i1_t *gen_read_MPAT15_UR(void);
    /**
     * \brief Generates code to query the value of the rh850 mpu_entry register
     * \return Object representing the value of the rh850 mpu_entry register
     */
    fastiss::i8_t *gen_read_mpu_entry(void);
    /**
     * \brief Generates code to query the value of the rh850 mpat_g_support register
     * \return Object representing the value of the rh850 mpat_g_support register
     */
    fastiss::i8_t *gen_read_mpat_g_support(void);
    /**
     * \brief Generates code to query the value of the rh850 swd0pcad_exec_count register
     * \return Object representing the value of the rh850 swd0pcad_exec_count register
     */
    fastiss::i32_t *gen_read_swd0pcad_exec_count(void);
    /**
     * \brief Generates code to query the value of the rh850 swd0pcad_val register
     * \return Object representing the value of the rh850 swd0pcad_val register
     */
    fastiss::i32_t *gen_read_swd0pcad_val(void);
    /**
     * \brief Generates code to query the value of the rh850 mem_acc_type register
     * \return Object representing the value of the rh850 mem_acc_type register
     */
    fastiss::i32_t *gen_read_mem_acc_type(void);

    /**
     * \brief Generates code to query the value of the specified register from the rh850 GR register group
     * \param index Index value of the required register from the rh850 GR register group
     * \return Object representing the value of the specified rh850 GR group register
     */
    fastiss::i32_t *gen_read_GR(rh850::GR_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \return Object representing the value of the specified rh850 SR_SELID_0 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_0(rh850::SR_SELID_0_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \return Object representing the value of the specified rh850 SR_SELID_1 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_1(rh850::SR_SELID_1_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \return Object representing the value of the specified rh850 SR_SELID_2 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_2(rh850::SR_SELID_2_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \return Object representing the value of the specified rh850 SR_SELID_3 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_3(rh850::SR_SELID_3_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \return Object representing the value of the specified rh850 SR_SELID_5 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_5(rh850::SR_SELID_5_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850 SR_SELID_6 register group
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \return Object representing the value of the specified rh850 SR_SELID_6 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_6(rh850::SR_SELID_6_index_t index);
    /**
     * \brief Generates code to query the value of the specified register from the rh850 SR_SELID_7 register group
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \return Object representing the value of the specified rh850 SR_SELID_7 group register
     */
    fastiss::i32_t *gen_read_SR_SELID_7(rh850::SR_SELID_7_index_t index);

    /**
     * \brief Generates code to set the value of the rh850 r0 register
     * \param value Object representing the new value of the rh850 r0 register
     */
    void gen_write_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r1 register
     * \param value Object representing the new value of the rh850 r1 register
     */
    void gen_write_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r2 register
     * \param value Object representing the new value of the rh850 r2 register
     */
    void gen_write_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r3 register
     * \param value Object representing the new value of the rh850 r3 register
     */
    void gen_write_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r4 register
     * \param value Object representing the new value of the rh850 r4 register
     */
    void gen_write_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r5 register
     * \param value Object representing the new value of the rh850 r5 register
     */
    void gen_write_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r6 register
     * \param value Object representing the new value of the rh850 r6 register
     */
    void gen_write_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r7 register
     * \param value Object representing the new value of the rh850 r7 register
     */
    void gen_write_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r8 register
     * \param value Object representing the new value of the rh850 r8 register
     */
    void gen_write_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r9 register
     * \param value Object representing the new value of the rh850 r9 register
     */
    void gen_write_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r10 register
     * \param value Object representing the new value of the rh850 r10 register
     */
    void gen_write_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r11 register
     * \param value Object representing the new value of the rh850 r11 register
     */
    void gen_write_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r12 register
     * \param value Object representing the new value of the rh850 r12 register
     */
    void gen_write_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r13 register
     * \param value Object representing the new value of the rh850 r13 register
     */
    void gen_write_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r14 register
     * \param value Object representing the new value of the rh850 r14 register
     */
    void gen_write_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r15 register
     * \param value Object representing the new value of the rh850 r15 register
     */
    void gen_write_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r16 register
     * \param value Object representing the new value of the rh850 r16 register
     */
    void gen_write_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r17 register
     * \param value Object representing the new value of the rh850 r17 register
     */
    void gen_write_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r18 register
     * \param value Object representing the new value of the rh850 r18 register
     */
    void gen_write_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r19 register
     * \param value Object representing the new value of the rh850 r19 register
     */
    void gen_write_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r20 register
     * \param value Object representing the new value of the rh850 r20 register
     */
    void gen_write_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r21 register
     * \param value Object representing the new value of the rh850 r21 register
     */
    void gen_write_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r22 register
     * \param value Object representing the new value of the rh850 r22 register
     */
    void gen_write_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r23 register
     * \param value Object representing the new value of the rh850 r23 register
     */
    void gen_write_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r24 register
     * \param value Object representing the new value of the rh850 r24 register
     */
    void gen_write_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r25 register
     * \param value Object representing the new value of the rh850 r25 register
     */
    void gen_write_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r26 register
     * \param value Object representing the new value of the rh850 r26 register
     */
    void gen_write_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r27 register
     * \param value Object representing the new value of the rh850 r27 register
     */
    void gen_write_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r28 register
     * \param value Object representing the new value of the rh850 r28 register
     */
    void gen_write_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r29 register
     * \param value Object representing the new value of the rh850 r29 register
     */
    void gen_write_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r30 register
     * \param value Object representing the new value of the rh850 r30 register
     */
    void gen_write_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 r31 register
     * \param value Object representing the new value of the rh850 r31 register
     */
    void gen_write_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PC register
     * \param value Object representing the new value of the rh850 PC register
     */
    void gen_write_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 EIPC register
     * \param value Object representing the new value of the rh850 EIPC register
     */
    void gen_write_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 EIPSW register
     * \param value Object representing the new value of the rh850 EIPSW register
     */
    void gen_write_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FEPC register
     * \param value Object representing the new value of the rh850 FEPC register
     */
    void gen_write_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FEPSW register
     * \param value Object representing the new value of the rh850 FEPSW register
     */
    void gen_write_FEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW register
     * \param value Object representing the new value of the rh850 PSW register
     */
    void gen_write_PSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_Z register field
     * \param value Object representing the new value of the rh850 PSW_Z register field
     */
    void gen_write_PSW_Z(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_S register field
     * \param value Object representing the new value of the rh850 PSW_S register field
     */
    void gen_write_PSW_S(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_OV register field
     * \param value Object representing the new value of the rh850 PSW_OV register field
     */
    void gen_write_PSW_OV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_CY register field
     * \param value Object representing the new value of the rh850 PSW_CY register field
     */
    void gen_write_PSW_CY(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_SAT register field
     * \param value Object representing the new value of the rh850 PSW_SAT register field
     */
    void gen_write_PSW_SAT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_ID register field
     * \param value Object representing the new value of the rh850 PSW_ID register field
     */
    void gen_write_PSW_ID(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_EP register field
     * \param value Object representing the new value of the rh850 PSW_EP register field
     */
    void gen_write_PSW_EP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_NP register field
     * \param value Object representing the new value of the rh850 PSW_NP register field
     */
    void gen_write_PSW_NP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_CM register field
     * \param value Object representing the new value of the rh850 PSW_CM register field
     */
    void gen_write_PSW_CM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_SS register field
     * \param value Object representing the new value of the rh850 PSW_SS register field
     */
    void gen_write_PSW_SS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_EBV register field
     * \param value Object representing the new value of the rh850 PSW_EBV register field
     */
    void gen_write_PSW_EBV(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_CU0 register field
     * \param value Object representing the new value of the rh850 PSW_CU0 register field
     */
    void gen_write_PSW_CU0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_CU1 register field
     * \param value Object representing the new value of the rh850 PSW_CU1 register field
     */
    void gen_write_PSW_CU1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_CU2 register field
     * \param value Object representing the new value of the rh850 PSW_CU2 register field
     */
    void gen_write_PSW_CU2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_HVC register field
     * \param value Object representing the new value of the rh850 PSW_HVC register field
     */
    void gen_write_PSW_HVC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PSW_UM register field
     * \param value Object representing the new value of the rh850 PSW_UM register field
     */
    void gen_write_PSW_UM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR register
     * \param value Object representing the new value of the rh850 FPSR register
     */
    void gen_write_FPSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_CC register field
     * \param value Object representing the new value of the rh850 FPSR_CC register field
     */
    void gen_write_FPSR_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_FN register field
     * \param value Object representing the new value of the rh850 FPSR_FN register field
     */
    void gen_write_FPSR_FN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_IF register field
     * \param value Object representing the new value of the rh850 FPSR_IF register field
     */
    void gen_write_FPSR_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_PEM register field
     * \param value Object representing the new value of the rh850 FPSR_PEM register field
     */
    void gen_write_FPSR_PEM(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_RM register field
     * \param value Object representing the new value of the rh850 FPSR_RM register field
     */
    void gen_write_FPSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_FS register field
     * \param value Object representing the new value of the rh850 FPSR_FS register field
     */
    void gen_write_FPSR_FS(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_XC register field
     * \param value Object representing the new value of the rh850 FPSR_XC register field
     */
    void gen_write_FPSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_XE register field
     * \param value Object representing the new value of the rh850 FPSR_XE register field
     */
    void gen_write_FPSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPSR_XP register field
     * \param value Object representing the new value of the rh850 FPSR_XP register field
     */
    void gen_write_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPEPC register
     * \param value Object representing the new value of the rh850 FPEPC register
     */
    void gen_write_FPEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPST register
     * \param value Object representing the new value of the rh850 FPST register
     */
    void gen_write_FPST(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPST_XC register field
     * \param value Object representing the new value of the rh850 FPST_XC register field
     */
    void gen_write_FPST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPST_IF register field
     * \param value Object representing the new value of the rh850 FPST_IF register field
     */
    void gen_write_FPST_IF(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPST_XP register field
     * \param value Object representing the new value of the rh850 FPST_XP register field
     */
    void gen_write_FPST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPCC register
     * \param value Object representing the new value of the rh850 FPCC register
     */
    void gen_write_FPCC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPCC_CC register field
     * \param value Object representing the new value of the rh850 FPCC_CC register field
     */
    void gen_write_FPCC_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPCFG register
     * \param value Object representing the new value of the rh850 FPCFG register
     */
    void gen_write_FPCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPCFG_RM register field
     * \param value Object representing the new value of the rh850 FPCFG_RM register field
     */
    void gen_write_FPCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPCFG_XE register field
     * \param value Object representing the new value of the rh850 FPCFG_XE register field
     */
    void gen_write_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPEC register
     * \param value Object representing the new value of the rh850 FPEC register
     */
    void gen_write_FPEC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 EIIC register
     * \param value Object representing the new value of the rh850 EIIC register
     */
    void gen_write_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FEIC register
     * \param value Object representing the new value of the rh850 FEIC register
     */
    void gen_write_FEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 CTPC register
     * \param value Object representing the new value of the rh850 CTPC register
     */
    void gen_write_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 CTPSW register
     * \param value Object representing the new value of the rh850 CTPSW register
     */
    void gen_write_CTPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 CTBP register
     * \param value Object representing the new value of the rh850 CTBP register
     */
    void gen_write_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 EIWR register
     * \param value Object representing the new value of the rh850 EIWR register
     */
    void gen_write_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FEWR register
     * \param value Object representing the new value of the rh850 FEWR register
     */
    void gen_write_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 BSEL register
     * \param value Object representing the new value of the rh850 BSEL register
     */
    void gen_write_BSEL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCFG0 register
     * \param value Object representing the new value of the rh850 MCFG0 register
     */
    void gen_write_MCFG0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCFG0_SPID register field
     * \param value Object representing the new value of the rh850 MCFG0_SPID register field
     */
    void gen_write_MCFG0_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCFG0_HVP register field
     * \param value Object representing the new value of the rh850 MCFG0_HVP register field
     */
    void gen_write_MCFG0_HVP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 RBASE register
     * \param value Object representing the new value of the rh850 RBASE register
     */
    void gen_write_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 RBASE_RBASE register field
     * \param value Object representing the new value of the rh850 RBASE_RBASE register field
     */
    void gen_write_RBASE_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 RBASE_RINT register field
     * \param value Object representing the new value of the rh850 RBASE_RINT register field
     */
    void gen_write_RBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 EBASE register
     * \param value Object representing the new value of the rh850 EBASE register
     */
    void gen_write_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 EBASE_EBASE register field
     * \param value Object representing the new value of the rh850 EBASE_EBASE register field
     */
    void gen_write_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 EBASE_RINT register field
     * \param value Object representing the new value of the rh850 EBASE_RINT register field
     */
    void gen_write_EBASE_RINT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 INTBP register
     * \param value Object representing the new value of the rh850 INTBP register
     */
    void gen_write_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCTL register
     * \param value Object representing the new value of the rh850 MCTL register
     */
    void gen_write_MCTL(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCTL_EN register field
     * \param value Object representing the new value of the rh850 MCTL_EN register field
     */
    void gen_write_MCTL_EN(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCTL_MT register field
     * \param value Object representing the new value of the rh850 MCTL_MT register field
     */
    void gen_write_MCTL_MT(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCTL_STID register field
     * \param value Object representing the new value of the rh850 MCTL_STID register field
     */
    void gen_write_MCTL_STID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCTL_MA register field
     * \param value Object representing the new value of the rh850 MCTL_MA register field
     */
    void gen_write_MCTL_MA(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCTL_UIC register field
     * \param value Object representing the new value of the rh850 MCTL_UIC register field
     */
    void gen_write_MCTL_UIC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PID register
     * \param value Object representing the new value of the rh850 PID register
     */
    void gen_write_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPIPR register
     * \param value Object representing the new value of the rh850 FPIPR register
     */
    void gen_write_FPIPR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 FPIPR_FPIPR register field
     * \param value Object representing the new value of the rh850 FPIPR_FPIPR register field
     */
    void gen_write_FPIPR_FPIPR(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 SCCFG register
     * \param value Object representing the new value of the rh850 SCCFG register
     */
    void gen_write_SCCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 SCCFG_SIZE register field
     * \param value Object representing the new value of the rh850 SCCFG_SIZE register field
     */
    void gen_write_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 SCBP register
     * \param value Object representing the new value of the rh850 SCBP register
     */
    void gen_write_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 HTCFG0 register
     * \param value Object representing the new value of the rh850 HTCFG0 register
     */
    void gen_write_HTCFG0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 HTCFG0_PEID register field
     * \param value Object representing the new value of the rh850 HTCFG0_PEID register field
     */
    void gen_write_HTCFG0_PEID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 HTCFG0_NC register field
     * \param value Object representing the new value of the rh850 HTCFG0_NC register field
     */
    void gen_write_HTCFG0_NC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 HTCFG0_TCID register field
     * \param value Object representing the new value of the rh850 HTCFG0_TCID register field
     */
    void gen_write_HTCFG0_TCID(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 HVCCFG register
     * \param value Object representing the new value of the rh850 HVCCFG register
     */
    void gen_write_HVCCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 HVCBP register
     * \param value Object representing the new value of the rh850 HVCBP register
     */
    void gen_write_HVCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MEA register
     * \param value Object representing the new value of the rh850 MEA register
     */
    void gen_write_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 ASID register
     * \param value Object representing the new value of the rh850 ASID register
     */
    void gen_write_ASID(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 ASID_ASID register field
     * \param value Object representing the new value of the rh850 ASID_ASID register field
     */
    void gen_write_ASID_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MEI register
     * \param value Object representing the new value of the rh850 MEI register
     */
    void gen_write_MEI(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MEI_REG register field
     * \param value Object representing the new value of the rh850 MEI_REG register field
     */
    void gen_write_MEI_REG(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MEI_DS register field
     * \param value Object representing the new value of the rh850 MEI_DS register field
     */
    void gen_write_MEI_DS(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MEI_U register field
     * \param value Object representing the new value of the rh850 MEI_U register field
     */
    void gen_write_MEI_U(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MEI_ITYPE register field
     * \param value Object representing the new value of the rh850 MEI_ITYPE register field
     */
    void gen_write_MEI_ITYPE(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MEI_RW register field
     * \param value Object representing the new value of the rh850 MEI_RW register field
     */
    void gen_write_MEI_RW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 ISPR register
     * \param value Object representing the new value of the rh850 ISPR register
     */
    void gen_write_ISPR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 ISPR_ISP register field
     * \param value Object representing the new value of the rh850 ISPR_ISP register field
     */
    void gen_write_ISPR_ISP(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PMR register
     * \param value Object representing the new value of the rh850 PMR register
     */
    void gen_write_PMR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 PMR_PM register field
     * \param value Object representing the new value of the rh850 PMR_PM register field
     */
    void gen_write_PMR_PM(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 ICSR register
     * \param value Object representing the new value of the rh850 ICSR register
     */
    void gen_write_ICSR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 ICSR_PMEI register field
     * \param value Object representing the new value of the rh850 ICSR_PMEI register field
     */
    void gen_write_ICSR_PMEI(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 INTCFG register
     * \param value Object representing the new value of the rh850 INTCFG register
     */
    void gen_write_INTCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 INTCFG_ISPC register field
     * \param value Object representing the new value of the rh850 INTCFG_ISPC register field
     */
    void gen_write_INTCFG_ISPC(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 DBIC register
     * \param value Object representing the new value of the rh850 DBIC register
     */
    void gen_write_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 DBCMC register
     * \param value Object representing the new value of the rh850 DBCMC register
     */
    void gen_write_DBCMC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 DBPC register
     * \param value Object representing the new value of the rh850 DBPC register
     */
    void gen_write_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 DBPSW register
     * \param value Object representing the new value of the rh850 DBPSW register
     */
    void gen_write_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 DIR0 register
     * \param value Object representing the new value of the rh850 DIR0 register
     */
    void gen_write_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 DIR1 register
     * \param value Object representing the new value of the rh850 DIR1 register
     */
    void gen_write_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 BPC register
     * \param value Object representing the new value of the rh850 BPC register
     */
    void gen_write_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 BPAV register
     * \param value Object representing the new value of the rh850 BPAV register
     */
    void gen_write_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 BPAM register
     * \param value Object representing the new value of the rh850 BPAM register
     */
    void gen_write_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 BPDV register
     * \param value Object representing the new value of the rh850 BPDV register
     */
    void gen_write_BPDV(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 BPDM register
     * \param value Object representing the new value of the rh850 BPDM register
     */
    void gen_write_BPDM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 DBWR register
     * \param value Object representing the new value of the rh850 DBWR register
     */
    void gen_write_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPM register
     * \param value Object representing the new value of the rh850 MPM register
     */
    void gen_write_MPM(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPM_DX register field
     * \param value Object representing the new value of the rh850 MPM_DX register field
     */
    void gen_write_MPM_DX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPM_DW register field
     * \param value Object representing the new value of the rh850 MPM_DW register field
     */
    void gen_write_MPM_DW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPM_DR register field
     * \param value Object representing the new value of the rh850 MPM_DR register field
     */
    void gen_write_MPM_DR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPM_SVP register field
     * \param value Object representing the new value of the rh850 MPM_SVP register field
     */
    void gen_write_MPM_SVP(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPM_MPE register field
     * \param value Object representing the new value of the rh850 MPM_MPE register field
     */
    void gen_write_MPM_MPE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC register
     * \param value Object representing the new value of the rh850 MPRC register
     */
    void gen_write_MPRC(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E15 register field
     * \param value Object representing the new value of the rh850 MPRC_E15 register field
     */
    void gen_write_MPRC_E15(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E14 register field
     * \param value Object representing the new value of the rh850 MPRC_E14 register field
     */
    void gen_write_MPRC_E14(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E13 register field
     * \param value Object representing the new value of the rh850 MPRC_E13 register field
     */
    void gen_write_MPRC_E13(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E12 register field
     * \param value Object representing the new value of the rh850 MPRC_E12 register field
     */
    void gen_write_MPRC_E12(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E11 register field
     * \param value Object representing the new value of the rh850 MPRC_E11 register field
     */
    void gen_write_MPRC_E11(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E10 register field
     * \param value Object representing the new value of the rh850 MPRC_E10 register field
     */
    void gen_write_MPRC_E10(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E9 register field
     * \param value Object representing the new value of the rh850 MPRC_E9 register field
     */
    void gen_write_MPRC_E9(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E8 register field
     * \param value Object representing the new value of the rh850 MPRC_E8 register field
     */
    void gen_write_MPRC_E8(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E7 register field
     * \param value Object representing the new value of the rh850 MPRC_E7 register field
     */
    void gen_write_MPRC_E7(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E6 register field
     * \param value Object representing the new value of the rh850 MPRC_E6 register field
     */
    void gen_write_MPRC_E6(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E5 register field
     * \param value Object representing the new value of the rh850 MPRC_E5 register field
     */
    void gen_write_MPRC_E5(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E4 register field
     * \param value Object representing the new value of the rh850 MPRC_E4 register field
     */
    void gen_write_MPRC_E4(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E3 register field
     * \param value Object representing the new value of the rh850 MPRC_E3 register field
     */
    void gen_write_MPRC_E3(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E2 register field
     * \param value Object representing the new value of the rh850 MPRC_E2 register field
     */
    void gen_write_MPRC_E2(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E1 register field
     * \param value Object representing the new value of the rh850 MPRC_E1 register field
     */
    void gen_write_MPRC_E1(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPRC_E0 register field
     * \param value Object representing the new value of the rh850 MPRC_E0 register field
     */
    void gen_write_MPRC_E0(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPBRGN register
     * \param value Object representing the new value of the rh850 MPBRGN register
     */
    void gen_write_MPBRGN(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPBRGN_MPBRGN register field
     * \param value Object representing the new value of the rh850 MPBRGN_MPBRGN register field
     */
    void gen_write_MPBRGN_MPBRGN(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPTRGN register
     * \param value Object representing the new value of the rh850 MPTRGN register
     */
    void gen_write_MPTRGN(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPTRGN_MPTRGN register field
     * \param value Object representing the new value of the rh850 MPTRGN_MPTRGN register field
     */
    void gen_write_MPTRGN_MPTRGN(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCR register
     * \param value Object representing the new value of the rh850 MCR register
     */
    void gen_write_MCR(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCR_SXE register field
     * \param value Object representing the new value of the rh850 MCR_SXE register field
     */
    void gen_write_MCR_SXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCR_SWE register field
     * \param value Object representing the new value of the rh850 MCR_SWE register field
     */
    void gen_write_MCR_SWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCR_SRE register field
     * \param value Object representing the new value of the rh850 MCR_SRE register field
     */
    void gen_write_MCR_SRE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCR_UXE register field
     * \param value Object representing the new value of the rh850 MCR_UXE register field
     */
    void gen_write_MCR_UXE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCR_UWE register field
     * \param value Object representing the new value of the rh850 MCR_UWE register field
     */
    void gen_write_MCR_UWE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MCR_URE register field
     * \param value Object representing the new value of the rh850 MCR_URE register field
     */
    void gen_write_MCR_URE(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA0 register
     * \param value Object representing the new value of the rh850 MPLA0 register
     */
    void gen_write_MPLA0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA0 register
     * \param value Object representing the new value of the rh850 MPUA0 register
     */
    void gen_write_MPUA0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0 register
     * \param value Object representing the new value of the rh850 MPAT0 register
     */
    void gen_write_MPAT0(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_ASID register field
     * \param value Object representing the new value of the rh850 MPAT0_ASID register field
     */
    void gen_write_MPAT0_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_E register field
     * \param value Object representing the new value of the rh850 MPAT0_E register field
     */
    void gen_write_MPAT0_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_G register field
     * \param value Object representing the new value of the rh850 MPAT0_G register field
     */
    void gen_write_MPAT0_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_SX register field
     * \param value Object representing the new value of the rh850 MPAT0_SX register field
     */
    void gen_write_MPAT0_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_SW register field
     * \param value Object representing the new value of the rh850 MPAT0_SW register field
     */
    void gen_write_MPAT0_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_SR register field
     * \param value Object representing the new value of the rh850 MPAT0_SR register field
     */
    void gen_write_MPAT0_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_UX register field
     * \param value Object representing the new value of the rh850 MPAT0_UX register field
     */
    void gen_write_MPAT0_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_UW register field
     * \param value Object representing the new value of the rh850 MPAT0_UW register field
     */
    void gen_write_MPAT0_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT0_UR register field
     * \param value Object representing the new value of the rh850 MPAT0_UR register field
     */
    void gen_write_MPAT0_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA1 register
     * \param value Object representing the new value of the rh850 MPLA1 register
     */
    void gen_write_MPLA1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA1 register
     * \param value Object representing the new value of the rh850 MPUA1 register
     */
    void gen_write_MPUA1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1 register
     * \param value Object representing the new value of the rh850 MPAT1 register
     */
    void gen_write_MPAT1(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_ASID register field
     * \param value Object representing the new value of the rh850 MPAT1_ASID register field
     */
    void gen_write_MPAT1_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_E register field
     * \param value Object representing the new value of the rh850 MPAT1_E register field
     */
    void gen_write_MPAT1_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_G register field
     * \param value Object representing the new value of the rh850 MPAT1_G register field
     */
    void gen_write_MPAT1_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_SX register field
     * \param value Object representing the new value of the rh850 MPAT1_SX register field
     */
    void gen_write_MPAT1_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_SW register field
     * \param value Object representing the new value of the rh850 MPAT1_SW register field
     */
    void gen_write_MPAT1_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_SR register field
     * \param value Object representing the new value of the rh850 MPAT1_SR register field
     */
    void gen_write_MPAT1_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_UX register field
     * \param value Object representing the new value of the rh850 MPAT1_UX register field
     */
    void gen_write_MPAT1_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_UW register field
     * \param value Object representing the new value of the rh850 MPAT1_UW register field
     */
    void gen_write_MPAT1_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT1_UR register field
     * \param value Object representing the new value of the rh850 MPAT1_UR register field
     */
    void gen_write_MPAT1_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA2 register
     * \param value Object representing the new value of the rh850 MPLA2 register
     */
    void gen_write_MPLA2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA2 register
     * \param value Object representing the new value of the rh850 MPUA2 register
     */
    void gen_write_MPUA2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2 register
     * \param value Object representing the new value of the rh850 MPAT2 register
     */
    void gen_write_MPAT2(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_ASID register field
     * \param value Object representing the new value of the rh850 MPAT2_ASID register field
     */
    void gen_write_MPAT2_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_E register field
     * \param value Object representing the new value of the rh850 MPAT2_E register field
     */
    void gen_write_MPAT2_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_G register field
     * \param value Object representing the new value of the rh850 MPAT2_G register field
     */
    void gen_write_MPAT2_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_SX register field
     * \param value Object representing the new value of the rh850 MPAT2_SX register field
     */
    void gen_write_MPAT2_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_SW register field
     * \param value Object representing the new value of the rh850 MPAT2_SW register field
     */
    void gen_write_MPAT2_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_SR register field
     * \param value Object representing the new value of the rh850 MPAT2_SR register field
     */
    void gen_write_MPAT2_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_UX register field
     * \param value Object representing the new value of the rh850 MPAT2_UX register field
     */
    void gen_write_MPAT2_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_UW register field
     * \param value Object representing the new value of the rh850 MPAT2_UW register field
     */
    void gen_write_MPAT2_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT2_UR register field
     * \param value Object representing the new value of the rh850 MPAT2_UR register field
     */
    void gen_write_MPAT2_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA3 register
     * \param value Object representing the new value of the rh850 MPLA3 register
     */
    void gen_write_MPLA3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA3 register
     * \param value Object representing the new value of the rh850 MPUA3 register
     */
    void gen_write_MPUA3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3 register
     * \param value Object representing the new value of the rh850 MPAT3 register
     */
    void gen_write_MPAT3(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_ASID register field
     * \param value Object representing the new value of the rh850 MPAT3_ASID register field
     */
    void gen_write_MPAT3_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_E register field
     * \param value Object representing the new value of the rh850 MPAT3_E register field
     */
    void gen_write_MPAT3_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_G register field
     * \param value Object representing the new value of the rh850 MPAT3_G register field
     */
    void gen_write_MPAT3_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_SX register field
     * \param value Object representing the new value of the rh850 MPAT3_SX register field
     */
    void gen_write_MPAT3_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_SW register field
     * \param value Object representing the new value of the rh850 MPAT3_SW register field
     */
    void gen_write_MPAT3_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_SR register field
     * \param value Object representing the new value of the rh850 MPAT3_SR register field
     */
    void gen_write_MPAT3_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_UX register field
     * \param value Object representing the new value of the rh850 MPAT3_UX register field
     */
    void gen_write_MPAT3_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_UW register field
     * \param value Object representing the new value of the rh850 MPAT3_UW register field
     */
    void gen_write_MPAT3_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT3_UR register field
     * \param value Object representing the new value of the rh850 MPAT3_UR register field
     */
    void gen_write_MPAT3_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA4 register
     * \param value Object representing the new value of the rh850 MPLA4 register
     */
    void gen_write_MPLA4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA4 register
     * \param value Object representing the new value of the rh850 MPUA4 register
     */
    void gen_write_MPUA4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4 register
     * \param value Object representing the new value of the rh850 MPAT4 register
     */
    void gen_write_MPAT4(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_ASID register field
     * \param value Object representing the new value of the rh850 MPAT4_ASID register field
     */
    void gen_write_MPAT4_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_E register field
     * \param value Object representing the new value of the rh850 MPAT4_E register field
     */
    void gen_write_MPAT4_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_G register field
     * \param value Object representing the new value of the rh850 MPAT4_G register field
     */
    void gen_write_MPAT4_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_SX register field
     * \param value Object representing the new value of the rh850 MPAT4_SX register field
     */
    void gen_write_MPAT4_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_SW register field
     * \param value Object representing the new value of the rh850 MPAT4_SW register field
     */
    void gen_write_MPAT4_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_SR register field
     * \param value Object representing the new value of the rh850 MPAT4_SR register field
     */
    void gen_write_MPAT4_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_UX register field
     * \param value Object representing the new value of the rh850 MPAT4_UX register field
     */
    void gen_write_MPAT4_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_UW register field
     * \param value Object representing the new value of the rh850 MPAT4_UW register field
     */
    void gen_write_MPAT4_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT4_UR register field
     * \param value Object representing the new value of the rh850 MPAT4_UR register field
     */
    void gen_write_MPAT4_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA5 register
     * \param value Object representing the new value of the rh850 MPLA5 register
     */
    void gen_write_MPLA5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA5 register
     * \param value Object representing the new value of the rh850 MPUA5 register
     */
    void gen_write_MPUA5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5 register
     * \param value Object representing the new value of the rh850 MPAT5 register
     */
    void gen_write_MPAT5(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_ASID register field
     * \param value Object representing the new value of the rh850 MPAT5_ASID register field
     */
    void gen_write_MPAT5_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_E register field
     * \param value Object representing the new value of the rh850 MPAT5_E register field
     */
    void gen_write_MPAT5_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_G register field
     * \param value Object representing the new value of the rh850 MPAT5_G register field
     */
    void gen_write_MPAT5_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_SX register field
     * \param value Object representing the new value of the rh850 MPAT5_SX register field
     */
    void gen_write_MPAT5_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_SW register field
     * \param value Object representing the new value of the rh850 MPAT5_SW register field
     */
    void gen_write_MPAT5_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_SR register field
     * \param value Object representing the new value of the rh850 MPAT5_SR register field
     */
    void gen_write_MPAT5_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_UX register field
     * \param value Object representing the new value of the rh850 MPAT5_UX register field
     */
    void gen_write_MPAT5_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_UW register field
     * \param value Object representing the new value of the rh850 MPAT5_UW register field
     */
    void gen_write_MPAT5_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT5_UR register field
     * \param value Object representing the new value of the rh850 MPAT5_UR register field
     */
    void gen_write_MPAT5_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA6 register
     * \param value Object representing the new value of the rh850 MPLA6 register
     */
    void gen_write_MPLA6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA6 register
     * \param value Object representing the new value of the rh850 MPUA6 register
     */
    void gen_write_MPUA6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6 register
     * \param value Object representing the new value of the rh850 MPAT6 register
     */
    void gen_write_MPAT6(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_ASID register field
     * \param value Object representing the new value of the rh850 MPAT6_ASID register field
     */
    void gen_write_MPAT6_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_E register field
     * \param value Object representing the new value of the rh850 MPAT6_E register field
     */
    void gen_write_MPAT6_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_G register field
     * \param value Object representing the new value of the rh850 MPAT6_G register field
     */
    void gen_write_MPAT6_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_SX register field
     * \param value Object representing the new value of the rh850 MPAT6_SX register field
     */
    void gen_write_MPAT6_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_SW register field
     * \param value Object representing the new value of the rh850 MPAT6_SW register field
     */
    void gen_write_MPAT6_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_SR register field
     * \param value Object representing the new value of the rh850 MPAT6_SR register field
     */
    void gen_write_MPAT6_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_UX register field
     * \param value Object representing the new value of the rh850 MPAT6_UX register field
     */
    void gen_write_MPAT6_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_UW register field
     * \param value Object representing the new value of the rh850 MPAT6_UW register field
     */
    void gen_write_MPAT6_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT6_UR register field
     * \param value Object representing the new value of the rh850 MPAT6_UR register field
     */
    void gen_write_MPAT6_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA7 register
     * \param value Object representing the new value of the rh850 MPLA7 register
     */
    void gen_write_MPLA7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA7 register
     * \param value Object representing the new value of the rh850 MPUA7 register
     */
    void gen_write_MPUA7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7 register
     * \param value Object representing the new value of the rh850 MPAT7 register
     */
    void gen_write_MPAT7(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_ASID register field
     * \param value Object representing the new value of the rh850 MPAT7_ASID register field
     */
    void gen_write_MPAT7_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_E register field
     * \param value Object representing the new value of the rh850 MPAT7_E register field
     */
    void gen_write_MPAT7_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_G register field
     * \param value Object representing the new value of the rh850 MPAT7_G register field
     */
    void gen_write_MPAT7_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_SX register field
     * \param value Object representing the new value of the rh850 MPAT7_SX register field
     */
    void gen_write_MPAT7_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_SW register field
     * \param value Object representing the new value of the rh850 MPAT7_SW register field
     */
    void gen_write_MPAT7_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_SR register field
     * \param value Object representing the new value of the rh850 MPAT7_SR register field
     */
    void gen_write_MPAT7_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_UX register field
     * \param value Object representing the new value of the rh850 MPAT7_UX register field
     */
    void gen_write_MPAT7_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_UW register field
     * \param value Object representing the new value of the rh850 MPAT7_UW register field
     */
    void gen_write_MPAT7_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT7_UR register field
     * \param value Object representing the new value of the rh850 MPAT7_UR register field
     */
    void gen_write_MPAT7_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA8 register
     * \param value Object representing the new value of the rh850 MPLA8 register
     */
    void gen_write_MPLA8(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA8 register
     * \param value Object representing the new value of the rh850 MPUA8 register
     */
    void gen_write_MPUA8(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8 register
     * \param value Object representing the new value of the rh850 MPAT8 register
     */
    void gen_write_MPAT8(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_ASID register field
     * \param value Object representing the new value of the rh850 MPAT8_ASID register field
     */
    void gen_write_MPAT8_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_E register field
     * \param value Object representing the new value of the rh850 MPAT8_E register field
     */
    void gen_write_MPAT8_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_G register field
     * \param value Object representing the new value of the rh850 MPAT8_G register field
     */
    void gen_write_MPAT8_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_SX register field
     * \param value Object representing the new value of the rh850 MPAT8_SX register field
     */
    void gen_write_MPAT8_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_SW register field
     * \param value Object representing the new value of the rh850 MPAT8_SW register field
     */
    void gen_write_MPAT8_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_SR register field
     * \param value Object representing the new value of the rh850 MPAT8_SR register field
     */
    void gen_write_MPAT8_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_UX register field
     * \param value Object representing the new value of the rh850 MPAT8_UX register field
     */
    void gen_write_MPAT8_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_UW register field
     * \param value Object representing the new value of the rh850 MPAT8_UW register field
     */
    void gen_write_MPAT8_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT8_UR register field
     * \param value Object representing the new value of the rh850 MPAT8_UR register field
     */
    void gen_write_MPAT8_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA9 register
     * \param value Object representing the new value of the rh850 MPLA9 register
     */
    void gen_write_MPLA9(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA9 register
     * \param value Object representing the new value of the rh850 MPUA9 register
     */
    void gen_write_MPUA9(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9 register
     * \param value Object representing the new value of the rh850 MPAT9 register
     */
    void gen_write_MPAT9(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_ASID register field
     * \param value Object representing the new value of the rh850 MPAT9_ASID register field
     */
    void gen_write_MPAT9_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_E register field
     * \param value Object representing the new value of the rh850 MPAT9_E register field
     */
    void gen_write_MPAT9_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_G register field
     * \param value Object representing the new value of the rh850 MPAT9_G register field
     */
    void gen_write_MPAT9_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_SX register field
     * \param value Object representing the new value of the rh850 MPAT9_SX register field
     */
    void gen_write_MPAT9_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_SW register field
     * \param value Object representing the new value of the rh850 MPAT9_SW register field
     */
    void gen_write_MPAT9_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_SR register field
     * \param value Object representing the new value of the rh850 MPAT9_SR register field
     */
    void gen_write_MPAT9_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_UX register field
     * \param value Object representing the new value of the rh850 MPAT9_UX register field
     */
    void gen_write_MPAT9_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_UW register field
     * \param value Object representing the new value of the rh850 MPAT9_UW register field
     */
    void gen_write_MPAT9_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT9_UR register field
     * \param value Object representing the new value of the rh850 MPAT9_UR register field
     */
    void gen_write_MPAT9_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA10 register
     * \param value Object representing the new value of the rh850 MPLA10 register
     */
    void gen_write_MPLA10(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA10 register
     * \param value Object representing the new value of the rh850 MPUA10 register
     */
    void gen_write_MPUA10(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10 register
     * \param value Object representing the new value of the rh850 MPAT10 register
     */
    void gen_write_MPAT10(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_ASID register field
     * \param value Object representing the new value of the rh850 MPAT10_ASID register field
     */
    void gen_write_MPAT10_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_E register field
     * \param value Object representing the new value of the rh850 MPAT10_E register field
     */
    void gen_write_MPAT10_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_G register field
     * \param value Object representing the new value of the rh850 MPAT10_G register field
     */
    void gen_write_MPAT10_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_SX register field
     * \param value Object representing the new value of the rh850 MPAT10_SX register field
     */
    void gen_write_MPAT10_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_SW register field
     * \param value Object representing the new value of the rh850 MPAT10_SW register field
     */
    void gen_write_MPAT10_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_SR register field
     * \param value Object representing the new value of the rh850 MPAT10_SR register field
     */
    void gen_write_MPAT10_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_UX register field
     * \param value Object representing the new value of the rh850 MPAT10_UX register field
     */
    void gen_write_MPAT10_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_UW register field
     * \param value Object representing the new value of the rh850 MPAT10_UW register field
     */
    void gen_write_MPAT10_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT10_UR register field
     * \param value Object representing the new value of the rh850 MPAT10_UR register field
     */
    void gen_write_MPAT10_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA11 register
     * \param value Object representing the new value of the rh850 MPLA11 register
     */
    void gen_write_MPLA11(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA11 register
     * \param value Object representing the new value of the rh850 MPUA11 register
     */
    void gen_write_MPUA11(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11 register
     * \param value Object representing the new value of the rh850 MPAT11 register
     */
    void gen_write_MPAT11(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_ASID register field
     * \param value Object representing the new value of the rh850 MPAT11_ASID register field
     */
    void gen_write_MPAT11_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_E register field
     * \param value Object representing the new value of the rh850 MPAT11_E register field
     */
    void gen_write_MPAT11_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_G register field
     * \param value Object representing the new value of the rh850 MPAT11_G register field
     */
    void gen_write_MPAT11_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_SX register field
     * \param value Object representing the new value of the rh850 MPAT11_SX register field
     */
    void gen_write_MPAT11_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_SW register field
     * \param value Object representing the new value of the rh850 MPAT11_SW register field
     */
    void gen_write_MPAT11_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_SR register field
     * \param value Object representing the new value of the rh850 MPAT11_SR register field
     */
    void gen_write_MPAT11_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_UX register field
     * \param value Object representing the new value of the rh850 MPAT11_UX register field
     */
    void gen_write_MPAT11_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_UW register field
     * \param value Object representing the new value of the rh850 MPAT11_UW register field
     */
    void gen_write_MPAT11_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT11_UR register field
     * \param value Object representing the new value of the rh850 MPAT11_UR register field
     */
    void gen_write_MPAT11_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA12 register
     * \param value Object representing the new value of the rh850 MPLA12 register
     */
    void gen_write_MPLA12(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA12 register
     * \param value Object representing the new value of the rh850 MPUA12 register
     */
    void gen_write_MPUA12(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12 register
     * \param value Object representing the new value of the rh850 MPAT12 register
     */
    void gen_write_MPAT12(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_ASID register field
     * \param value Object representing the new value of the rh850 MPAT12_ASID register field
     */
    void gen_write_MPAT12_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_E register field
     * \param value Object representing the new value of the rh850 MPAT12_E register field
     */
    void gen_write_MPAT12_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_G register field
     * \param value Object representing the new value of the rh850 MPAT12_G register field
     */
    void gen_write_MPAT12_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_SX register field
     * \param value Object representing the new value of the rh850 MPAT12_SX register field
     */
    void gen_write_MPAT12_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_SW register field
     * \param value Object representing the new value of the rh850 MPAT12_SW register field
     */
    void gen_write_MPAT12_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_SR register field
     * \param value Object representing the new value of the rh850 MPAT12_SR register field
     */
    void gen_write_MPAT12_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_UX register field
     * \param value Object representing the new value of the rh850 MPAT12_UX register field
     */
    void gen_write_MPAT12_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_UW register field
     * \param value Object representing the new value of the rh850 MPAT12_UW register field
     */
    void gen_write_MPAT12_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT12_UR register field
     * \param value Object representing the new value of the rh850 MPAT12_UR register field
     */
    void gen_write_MPAT12_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA13 register
     * \param value Object representing the new value of the rh850 MPLA13 register
     */
    void gen_write_MPLA13(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA13 register
     * \param value Object representing the new value of the rh850 MPUA13 register
     */
    void gen_write_MPUA13(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13 register
     * \param value Object representing the new value of the rh850 MPAT13 register
     */
    void gen_write_MPAT13(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_ASID register field
     * \param value Object representing the new value of the rh850 MPAT13_ASID register field
     */
    void gen_write_MPAT13_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_E register field
     * \param value Object representing the new value of the rh850 MPAT13_E register field
     */
    void gen_write_MPAT13_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_G register field
     * \param value Object representing the new value of the rh850 MPAT13_G register field
     */
    void gen_write_MPAT13_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_SX register field
     * \param value Object representing the new value of the rh850 MPAT13_SX register field
     */
    void gen_write_MPAT13_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_SW register field
     * \param value Object representing the new value of the rh850 MPAT13_SW register field
     */
    void gen_write_MPAT13_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_SR register field
     * \param value Object representing the new value of the rh850 MPAT13_SR register field
     */
    void gen_write_MPAT13_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_UX register field
     * \param value Object representing the new value of the rh850 MPAT13_UX register field
     */
    void gen_write_MPAT13_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_UW register field
     * \param value Object representing the new value of the rh850 MPAT13_UW register field
     */
    void gen_write_MPAT13_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT13_UR register field
     * \param value Object representing the new value of the rh850 MPAT13_UR register field
     */
    void gen_write_MPAT13_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA14 register
     * \param value Object representing the new value of the rh850 MPLA14 register
     */
    void gen_write_MPLA14(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA14 register
     * \param value Object representing the new value of the rh850 MPUA14 register
     */
    void gen_write_MPUA14(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14 register
     * \param value Object representing the new value of the rh850 MPAT14 register
     */
    void gen_write_MPAT14(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_ASID register field
     * \param value Object representing the new value of the rh850 MPAT14_ASID register field
     */
    void gen_write_MPAT14_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_E register field
     * \param value Object representing the new value of the rh850 MPAT14_E register field
     */
    void gen_write_MPAT14_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_G register field
     * \param value Object representing the new value of the rh850 MPAT14_G register field
     */
    void gen_write_MPAT14_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_SX register field
     * \param value Object representing the new value of the rh850 MPAT14_SX register field
     */
    void gen_write_MPAT14_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_SW register field
     * \param value Object representing the new value of the rh850 MPAT14_SW register field
     */
    void gen_write_MPAT14_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_SR register field
     * \param value Object representing the new value of the rh850 MPAT14_SR register field
     */
    void gen_write_MPAT14_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_UX register field
     * \param value Object representing the new value of the rh850 MPAT14_UX register field
     */
    void gen_write_MPAT14_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_UW register field
     * \param value Object representing the new value of the rh850 MPAT14_UW register field
     */
    void gen_write_MPAT14_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT14_UR register field
     * \param value Object representing the new value of the rh850 MPAT14_UR register field
     */
    void gen_write_MPAT14_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPLA15 register
     * \param value Object representing the new value of the rh850 MPLA15 register
     */
    void gen_write_MPLA15(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPUA15 register
     * \param value Object representing the new value of the rh850 MPUA15 register
     */
    void gen_write_MPUA15(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15 register
     * \param value Object representing the new value of the rh850 MPAT15 register
     */
    void gen_write_MPAT15(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_ASID register field
     * \param value Object representing the new value of the rh850 MPAT15_ASID register field
     */
    void gen_write_MPAT15_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_E register field
     * \param value Object representing the new value of the rh850 MPAT15_E register field
     */
    void gen_write_MPAT15_E(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_G register field
     * \param value Object representing the new value of the rh850 MPAT15_G register field
     */
    void gen_write_MPAT15_G(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_SX register field
     * \param value Object representing the new value of the rh850 MPAT15_SX register field
     */
    void gen_write_MPAT15_SX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_SW register field
     * \param value Object representing the new value of the rh850 MPAT15_SW register field
     */
    void gen_write_MPAT15_SW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_SR register field
     * \param value Object representing the new value of the rh850 MPAT15_SR register field
     */
    void gen_write_MPAT15_SR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_UX register field
     * \param value Object representing the new value of the rh850 MPAT15_UX register field
     */
    void gen_write_MPAT15_UX(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_UW register field
     * \param value Object representing the new value of the rh850 MPAT15_UW register field
     */
    void gen_write_MPAT15_UW(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 MPAT15_UR register field
     * \param value Object representing the new value of the rh850 MPAT15_UR register field
     */
    void gen_write_MPAT15_UR(fastiss::i1_t *value);
    /**
     * \brief Generates code to set the value of the rh850 mpu_entry register
     * \param value Object representing the new value of the rh850 mpu_entry register
     */
    void gen_write_mpu_entry(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 mpat_g_support register
     * \param value Object representing the new value of the rh850 mpat_g_support register
     */
    void gen_write_mpat_g_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to set the value of the rh850 swd0pcad_exec_count register
     * \param value Object representing the new value of the rh850 swd0pcad_exec_count register
     */
    void gen_write_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 swd0pcad_val register
     * \param value Object representing the new value of the rh850 swd0pcad_val register
     */
    void gen_write_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the rh850 mem_acc_type register
     * \param value Object representing the new value of the rh850 mem_acc_type register
     */
    void gen_write_mem_acc_type(fastiss::i32_t *value);

    /**
     * \brief Generates code to set the value of the specified register from the rh850 GR register group
     * \param index Index value of the required register from the rh850 GR register group
     * \param value Object representing the new value of the specified rh850 GR group register
     */
    void gen_write_GR(rh850::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850 SR_SELID_0 register group
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \param value Object representing the new value of the specified rh850 SR_SELID_0 group register
     */
    void gen_write_SR_SELID_0(rh850::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850 SR_SELID_1 register group
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \param value Object representing the new value of the specified rh850 SR_SELID_1 group register
     */
    void gen_write_SR_SELID_1(rh850::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850 SR_SELID_2 register group
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \param value Object representing the new value of the specified rh850 SR_SELID_2 group register
     */
    void gen_write_SR_SELID_2(rh850::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850 SR_SELID_3 register group
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \param value Object representing the new value of the specified rh850 SR_SELID_3 group register
     */
    void gen_write_SR_SELID_3(rh850::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850 SR_SELID_5 register group
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \param value Object representing the new value of the specified rh850 SR_SELID_5 group register
     */
    void gen_write_SR_SELID_5(rh850::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850 SR_SELID_6 register group
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \param value Object representing the new value of the specified rh850 SR_SELID_6 group register
     */
    void gen_write_SR_SELID_6(rh850::SR_SELID_6_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to set the value of the specified register from the rh850 SR_SELID_7 register group
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \param value Object representing the new value of the specified rh850 SR_SELID_7 group register
     */
    void gen_write_SR_SELID_7(rh850::SR_SELID_7_index_t index, fastiss::i32_t *value);

    /**
     * \brief Generates code to increment the value of the rh850 r0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r0 register
     */
    void gen_increment_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r1 register
     */
    void gen_increment_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r2 register
     */
    void gen_increment_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r3 register
     */
    void gen_increment_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r4 register
     */
    void gen_increment_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r5 register
     */
    void gen_increment_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r6 register
     */
    void gen_increment_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r7 register
     */
    void gen_increment_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r8 register
     */
    void gen_increment_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r9 register
     */
    void gen_increment_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r10 register
     */
    void gen_increment_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r11 register
     */
    void gen_increment_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r12 register
     */
    void gen_increment_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r13 register
     */
    void gen_increment_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r14 register
     */
    void gen_increment_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r15 register
     */
    void gen_increment_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r16 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r16 register
     */
    void gen_increment_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r17 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r17 register
     */
    void gen_increment_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r18 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r18 register
     */
    void gen_increment_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r19 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r19 register
     */
    void gen_increment_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r20 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r20 register
     */
    void gen_increment_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r21 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r21 register
     */
    void gen_increment_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r22 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r22 register
     */
    void gen_increment_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r23 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r23 register
     */
    void gen_increment_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r24 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r24 register
     */
    void gen_increment_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r25 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r25 register
     */
    void gen_increment_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r26 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r26 register
     */
    void gen_increment_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r27 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r27 register
     */
    void gen_increment_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r28 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r28 register
     */
    void gen_increment_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r29 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r29 register
     */
    void gen_increment_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r30 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r30 register
     */
    void gen_increment_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 r31 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 r31 register
     */
    void gen_increment_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 PC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 PC register
     */
    void gen_increment_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 EIPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 EIPC register
     */
    void gen_increment_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 EIPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 EIPSW register
     */
    void gen_increment_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 FEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 FEPC register
     */
    void gen_increment_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 FEPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 FEPSW register
     */
    void gen_increment_FEPSW(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPSR_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPSR_CC register field
    */
    void gen_increment_FPSR_CC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPSR_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPSR_RM register field
    */
    void gen_increment_FPSR_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPSR_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPSR_XC register field
    */
    void gen_increment_FPSR_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPSR_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPSR_XE register field
    */
    void gen_increment_FPSR_XE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPSR_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPSR_XP register field
    */
    void gen_increment_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 FPEPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 FPEPC register
     */
    void gen_increment_FPEPC(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPST_XC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPST_XC register field
    */
    void gen_increment_FPST_XC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPST_XP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPST_XP register field
    */
    void gen_increment_FPST_XP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPCC_CC register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPCC_CC register field
    */
    void gen_increment_FPCC_CC(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPCFG_RM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPCFG_RM register field
    */
    void gen_increment_FPCFG_RM(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPCFG_XE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPCFG_XE register field
    */
    void gen_increment_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 FPEC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 FPEC register
     */
    void gen_increment_FPEC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 EIIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 EIIC register
     */
    void gen_increment_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 FEIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 FEIC register
     */
    void gen_increment_FEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 CTPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 CTPC register
     */
    void gen_increment_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 CTPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 CTPSW register
     */
    void gen_increment_CTPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 CTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 CTBP register
     */
    void gen_increment_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 EIWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 EIWR register
     */
    void gen_increment_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 FEWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 FEWR register
     */
    void gen_increment_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 BSEL register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 BSEL register
     */
    void gen_increment_BSEL(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MCFG0_SPID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MCFG0_SPID register field
    */
    void gen_increment_MCFG0_SPID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 RBASE_RBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 RBASE_RBASE register field
    */
    void gen_increment_RBASE_RBASE(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 EBASE_EBASE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 EBASE_EBASE register field
    */
    void gen_increment_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 INTBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 INTBP register
     */
    void gen_increment_INTBP(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MCTL_STID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MCTL_STID register field
    */
    void gen_increment_MCTL_STID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 PID register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 PID register
     */
    void gen_increment_PID(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 FPIPR_FPIPR register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 FPIPR_FPIPR register field
    */
    void gen_increment_FPIPR_FPIPR(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 SCCFG_SIZE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 SCCFG_SIZE register field
    */
    void gen_increment_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 SCBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 SCBP register
     */
    void gen_increment_SCBP(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 HTCFG0_PEID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 HTCFG0_PEID register field
    */
    void gen_increment_HTCFG0_PEID(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 HTCFG0_TCID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 HTCFG0_TCID register field
    */
    void gen_increment_HTCFG0_TCID(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 HVCCFG register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 HVCCFG register
     */
    void gen_increment_HVCCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 HVCBP register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 HVCBP register
     */
    void gen_increment_HVCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MEA register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MEA register
     */
    void gen_increment_MEA(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 ASID_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 ASID_ASID register field
    */
    void gen_increment_ASID_ASID(fastiss::i16_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MEI_REG register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MEI_REG register field
    */
    void gen_increment_MEI_REG(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MEI_DS register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MEI_DS register field
    */
    void gen_increment_MEI_DS(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MEI_ITYPE register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MEI_ITYPE register field
    */
    void gen_increment_MEI_ITYPE(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 ISPR_ISP register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 ISPR_ISP register field
    */
    void gen_increment_ISPR_ISP(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 PMR_PM register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 PMR_PM register field
    */
    void gen_increment_PMR_PM(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 DBIC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 DBIC register
     */
    void gen_increment_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 DBCMC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 DBCMC register
     */
    void gen_increment_DBCMC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 DBPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 DBPC register
     */
    void gen_increment_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 DBPSW register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 DBPSW register
     */
    void gen_increment_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 DIR0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 DIR0 register
     */
    void gen_increment_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 DIR1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 DIR1 register
     */
    void gen_increment_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 BPC register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 BPC register
     */
    void gen_increment_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 BPAV register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 BPAV register
     */
    void gen_increment_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 BPAM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 BPAM register
     */
    void gen_increment_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 BPDV register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 BPDV register
     */
    void gen_increment_BPDV(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 BPDM register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 BPDM register
     */
    void gen_increment_BPDM(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 DBWR register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 DBWR register
     */
    void gen_increment_DBWR(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPBRGN_MPBRGN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPBRGN_MPBRGN register field
    */
    void gen_increment_MPBRGN_MPBRGN(fastiss::i8_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPTRGN_MPTRGN register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPTRGN_MPTRGN register field
    */
    void gen_increment_MPTRGN_MPTRGN(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA0 register
     */
    void gen_increment_MPLA0(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA0 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA0 register
     */
    void gen_increment_MPUA0(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT0_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT0_ASID register field
    */
    void gen_increment_MPAT0_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA1 register
     */
    void gen_increment_MPLA1(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA1 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA1 register
     */
    void gen_increment_MPUA1(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT1_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT1_ASID register field
    */
    void gen_increment_MPAT1_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA2 register
     */
    void gen_increment_MPLA2(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA2 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA2 register
     */
    void gen_increment_MPUA2(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT2_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT2_ASID register field
    */
    void gen_increment_MPAT2_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA3 register
     */
    void gen_increment_MPLA3(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA3 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA3 register
     */
    void gen_increment_MPUA3(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT3_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT3_ASID register field
    */
    void gen_increment_MPAT3_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA4 register
     */
    void gen_increment_MPLA4(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA4 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA4 register
     */
    void gen_increment_MPUA4(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT4_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT4_ASID register field
    */
    void gen_increment_MPAT4_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA5 register
     */
    void gen_increment_MPLA5(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA5 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA5 register
     */
    void gen_increment_MPUA5(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT5_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT5_ASID register field
    */
    void gen_increment_MPAT5_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA6 register
     */
    void gen_increment_MPLA6(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA6 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA6 register
     */
    void gen_increment_MPUA6(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT6_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT6_ASID register field
    */
    void gen_increment_MPAT6_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA7 register
     */
    void gen_increment_MPLA7(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA7 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA7 register
     */
    void gen_increment_MPUA7(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT7_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT7_ASID register field
    */
    void gen_increment_MPAT7_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA8 register
     */
    void gen_increment_MPLA8(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA8 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA8 register
     */
    void gen_increment_MPUA8(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT8_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT8_ASID register field
    */
    void gen_increment_MPAT8_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA9 register
     */
    void gen_increment_MPLA9(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA9 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA9 register
     */
    void gen_increment_MPUA9(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT9_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT9_ASID register field
    */
    void gen_increment_MPAT9_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA10 register
     */
    void gen_increment_MPLA10(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA10 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA10 register
     */
    void gen_increment_MPUA10(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT10_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT10_ASID register field
    */
    void gen_increment_MPAT10_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA11 register
     */
    void gen_increment_MPLA11(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA11 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA11 register
     */
    void gen_increment_MPUA11(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT11_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT11_ASID register field
    */
    void gen_increment_MPAT11_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA12 register
     */
    void gen_increment_MPLA12(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA12 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA12 register
     */
    void gen_increment_MPUA12(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT12_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT12_ASID register field
    */
    void gen_increment_MPAT12_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA13 register
     */
    void gen_increment_MPLA13(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA13 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA13 register
     */
    void gen_increment_MPUA13(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT13_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT13_ASID register field
    */
    void gen_increment_MPAT13_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA14 register
     */
    void gen_increment_MPLA14(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA14 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA14 register
     */
    void gen_increment_MPUA14(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT14_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT14_ASID register field
    */
    void gen_increment_MPAT14_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPLA15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPLA15 register
     */
    void gen_increment_MPLA15(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 MPUA15 register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 MPUA15 register
     */
    void gen_increment_MPUA15(fastiss::i32_t *value);
    /**
    * \brief Generates code to increment the value of the rh850 MPAT15_ASID register field by the specified amount
    * \param value Object representing the amount by which to increment the value of the rh850 MPAT15_ASID register field
    */
    void gen_increment_MPAT15_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 mpu_entry register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 mpu_entry register
     */
    void gen_increment_mpu_entry(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 mpat_g_support register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 mpat_g_support register
     */
    void gen_increment_mpat_g_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 swd0pcad_exec_count register
     */
    void gen_increment_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 swd0pcad_val register
     */
    void gen_increment_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the rh850 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to increment the value of the rh850 mem_acc_type register
     */
    void gen_increment_mem_acc_type(fastiss::i32_t *value);

    /**
     * \brief Generates code to increment the value of the specified register from the rh850 GR register group by the specified amount
     * \param index Index value of the required register from the rh850 GR register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 GR group register
     */
    void gen_increment_GR(rh850::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 SR_SELID_0 group register
     */
    void gen_increment_SR_SELID_0(rh850::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 SR_SELID_1 group register
     */
    void gen_increment_SR_SELID_1(rh850::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 SR_SELID_2 group register
     */
    void gen_increment_SR_SELID_2(rh850::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 SR_SELID_3 group register
     */
    void gen_increment_SR_SELID_3(rh850::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 SR_SELID_5 group register
     */
    void gen_increment_SR_SELID_5(rh850::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850 SR_SELID_6 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 SR_SELID_6 group register
     */
    void gen_increment_SR_SELID_6(rh850::SR_SELID_6_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to increment the value of the specified register from the rh850 SR_SELID_7 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \param value Object representing the amount by which to increment the value of the specified rh850 SR_SELID_7 group register
     */
    void gen_increment_SR_SELID_7(rh850::SR_SELID_7_index_t index, fastiss::i32_t *value);

    /**
     * \brief Generates code to decrement the value of the rh850 r0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r0 register
     */
    void gen_decrement_r0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r1 register
     */
    void gen_decrement_r1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r2 register
     */
    void gen_decrement_r2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r3 register
     */
    void gen_decrement_r3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r4 register
     */
    void gen_decrement_r4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r5 register
     */
    void gen_decrement_r5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r6 register
     */
    void gen_decrement_r6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r7 register
     */
    void gen_decrement_r7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r8 register
     */
    void gen_decrement_r8(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r9 register
     */
    void gen_decrement_r9(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r10 register
     */
    void gen_decrement_r10(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r11 register
     */
    void gen_decrement_r11(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r12 register
     */
    void gen_decrement_r12(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r13 register
     */
    void gen_decrement_r13(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r14 register
     */
    void gen_decrement_r14(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r15 register
     */
    void gen_decrement_r15(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r16 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r16 register
     */
    void gen_decrement_r16(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r17 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r17 register
     */
    void gen_decrement_r17(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r18 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r18 register
     */
    void gen_decrement_r18(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r19 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r19 register
     */
    void gen_decrement_r19(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r20 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r20 register
     */
    void gen_decrement_r20(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r21 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r21 register
     */
    void gen_decrement_r21(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r22 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r22 register
     */
    void gen_decrement_r22(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r23 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r23 register
     */
    void gen_decrement_r23(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r24 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r24 register
     */
    void gen_decrement_r24(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r25 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r25 register
     */
    void gen_decrement_r25(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r26 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r26 register
     */
    void gen_decrement_r26(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r27 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r27 register
     */
    void gen_decrement_r27(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r28 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r28 register
     */
    void gen_decrement_r28(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r29 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r29 register
     */
    void gen_decrement_r29(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r30 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r30 register
     */
    void gen_decrement_r30(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 r31 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 r31 register
     */
    void gen_decrement_r31(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 PC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 PC register
     */
    void gen_decrement_PC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 EIPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 EIPC register
     */
    void gen_decrement_EIPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 EIPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 EIPSW register
     */
    void gen_decrement_EIPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FEPC register
     */
    void gen_decrement_FEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FEPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FEPSW register
     */
    void gen_decrement_FEPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPSR_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPSR_CC register field
     */
    void gen_decrement_FPSR_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPSR_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPSR_RM register field
     */
    void gen_decrement_FPSR_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPSR_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPSR_XC register field
     */
    void gen_decrement_FPSR_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPSR_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPSR_XE register field
     */
    void gen_decrement_FPSR_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPSR_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPSR_XP register field
     */
    void gen_decrement_FPSR_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPEPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPEPC register
     */
    void gen_decrement_FPEPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPST_XC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPST_XC register field
     */
    void gen_decrement_FPST_XC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPST_XP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPST_XP register field
     */
    void gen_decrement_FPST_XP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPCC_CC register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPCC_CC register field
     */
    void gen_decrement_FPCC_CC(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPCFG_RM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPCFG_RM register field
     */
    void gen_decrement_FPCFG_RM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPCFG_XE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPCFG_XE register field
     */
    void gen_decrement_FPCFG_XE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPEC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPEC register
     */
    void gen_decrement_FPEC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 EIIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 EIIC register
     */
    void gen_decrement_EIIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FEIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FEIC register
     */
    void gen_decrement_FEIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 CTPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 CTPC register
     */
    void gen_decrement_CTPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 CTPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 CTPSW register
     */
    void gen_decrement_CTPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 CTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 CTBP register
     */
    void gen_decrement_CTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 EIWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 EIWR register
     */
    void gen_decrement_EIWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FEWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FEWR register
     */
    void gen_decrement_FEWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 BSEL register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 BSEL register
     */
    void gen_decrement_BSEL(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MCFG0_SPID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MCFG0_SPID register field
     */
    void gen_decrement_MCFG0_SPID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 RBASE_RBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 RBASE_RBASE register field
     */
    void gen_decrement_RBASE_RBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 EBASE_EBASE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 EBASE_EBASE register field
     */
    void gen_decrement_EBASE_EBASE(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 INTBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 INTBP register
     */
    void gen_decrement_INTBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MCTL_STID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MCTL_STID register field
     */
    void gen_decrement_MCTL_STID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 PID register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 PID register
     */
    void gen_decrement_PID(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 FPIPR_FPIPR register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 FPIPR_FPIPR register field
     */
    void gen_decrement_FPIPR_FPIPR(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 SCCFG_SIZE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 SCCFG_SIZE register field
     */
    void gen_decrement_SCCFG_SIZE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 SCBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 SCBP register
     */
    void gen_decrement_SCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 HTCFG0_PEID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 HTCFG0_PEID register field
     */
    void gen_decrement_HTCFG0_PEID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 HTCFG0_TCID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 HTCFG0_TCID register field
     */
    void gen_decrement_HTCFG0_TCID(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 HVCCFG register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 HVCCFG register
     */
    void gen_decrement_HVCCFG(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 HVCBP register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 HVCBP register
     */
    void gen_decrement_HVCBP(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MEA register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MEA register
     */
    void gen_decrement_MEA(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 ASID_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 ASID_ASID register field
     */
    void gen_decrement_ASID_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MEI_REG register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MEI_REG register field
     */
    void gen_decrement_MEI_REG(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MEI_DS register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MEI_DS register field
     */
    void gen_decrement_MEI_DS(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MEI_ITYPE register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MEI_ITYPE register field
     */
    void gen_decrement_MEI_ITYPE(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 ISPR_ISP register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 ISPR_ISP register field
     */
    void gen_decrement_ISPR_ISP(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 PMR_PM register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 PMR_PM register field
     */
    void gen_decrement_PMR_PM(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 DBIC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 DBIC register
     */
    void gen_decrement_DBIC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 DBCMC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 DBCMC register
     */
    void gen_decrement_DBCMC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 DBPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 DBPC register
     */
    void gen_decrement_DBPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 DBPSW register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 DBPSW register
     */
    void gen_decrement_DBPSW(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 DIR0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 DIR0 register
     */
    void gen_decrement_DIR0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 DIR1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 DIR1 register
     */
    void gen_decrement_DIR1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 BPC register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 BPC register
     */
    void gen_decrement_BPC(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 BPAV register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 BPAV register
     */
    void gen_decrement_BPAV(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 BPAM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 BPAM register
     */
    void gen_decrement_BPAM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 BPDV register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 BPDV register
     */
    void gen_decrement_BPDV(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 BPDM register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 BPDM register
     */
    void gen_decrement_BPDM(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 DBWR register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 DBWR register
     */
    void gen_decrement_DBWR(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPBRGN_MPBRGN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPBRGN_MPBRGN register field
     */
    void gen_decrement_MPBRGN_MPBRGN(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPTRGN_MPTRGN register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPTRGN_MPTRGN register field
     */
    void gen_decrement_MPTRGN_MPTRGN(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA0 register
     */
    void gen_decrement_MPLA0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA0 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA0 register
     */
    void gen_decrement_MPUA0(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT0_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT0_ASID register field
     */
    void gen_decrement_MPAT0_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA1 register
     */
    void gen_decrement_MPLA1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA1 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA1 register
     */
    void gen_decrement_MPUA1(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT1_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT1_ASID register field
     */
    void gen_decrement_MPAT1_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA2 register
     */
    void gen_decrement_MPLA2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA2 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA2 register
     */
    void gen_decrement_MPUA2(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT2_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT2_ASID register field
     */
    void gen_decrement_MPAT2_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA3 register
     */
    void gen_decrement_MPLA3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA3 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA3 register
     */
    void gen_decrement_MPUA3(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT3_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT3_ASID register field
     */
    void gen_decrement_MPAT3_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA4 register
     */
    void gen_decrement_MPLA4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA4 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA4 register
     */
    void gen_decrement_MPUA4(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT4_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT4_ASID register field
     */
    void gen_decrement_MPAT4_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA5 register
     */
    void gen_decrement_MPLA5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA5 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA5 register
     */
    void gen_decrement_MPUA5(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT5_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT5_ASID register field
     */
    void gen_decrement_MPAT5_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA6 register
     */
    void gen_decrement_MPLA6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA6 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA6 register
     */
    void gen_decrement_MPUA6(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT6_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT6_ASID register field
     */
    void gen_decrement_MPAT6_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA7 register
     */
    void gen_decrement_MPLA7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA7 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA7 register
     */
    void gen_decrement_MPUA7(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT7_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT7_ASID register field
     */
    void gen_decrement_MPAT7_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA8 register
     */
    void gen_decrement_MPLA8(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA8 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA8 register
     */
    void gen_decrement_MPUA8(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT8_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT8_ASID register field
     */
    void gen_decrement_MPAT8_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA9 register
     */
    void gen_decrement_MPLA9(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA9 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA9 register
     */
    void gen_decrement_MPUA9(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT9_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT9_ASID register field
     */
    void gen_decrement_MPAT9_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA10 register
     */
    void gen_decrement_MPLA10(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA10 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA10 register
     */
    void gen_decrement_MPUA10(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT10_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT10_ASID register field
     */
    void gen_decrement_MPAT10_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA11 register
     */
    void gen_decrement_MPLA11(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA11 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA11 register
     */
    void gen_decrement_MPUA11(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT11_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT11_ASID register field
     */
    void gen_decrement_MPAT11_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA12 register
     */
    void gen_decrement_MPLA12(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA12 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA12 register
     */
    void gen_decrement_MPUA12(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT12_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT12_ASID register field
     */
    void gen_decrement_MPAT12_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA13 register
     */
    void gen_decrement_MPLA13(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA13 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA13 register
     */
    void gen_decrement_MPUA13(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT13_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT13_ASID register field
     */
    void gen_decrement_MPAT13_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA14 register
     */
    void gen_decrement_MPLA14(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA14 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA14 register
     */
    void gen_decrement_MPUA14(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT14_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT14_ASID register field
     */
    void gen_decrement_MPAT14_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPLA15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPLA15 register
     */
    void gen_decrement_MPLA15(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPUA15 register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPUA15 register
     */
    void gen_decrement_MPUA15(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 MPAT15_ASID register field by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 MPAT15_ASID register field
     */
    void gen_decrement_MPAT15_ASID(fastiss::i16_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 mpu_entry register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 mpu_entry register
     */
    void gen_decrement_mpu_entry(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 mpat_g_support register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 mpat_g_support register
     */
    void gen_decrement_mpat_g_support(fastiss::i8_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 swd0pcad_exec_count register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 swd0pcad_exec_count register
     */
    void gen_decrement_swd0pcad_exec_count(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 swd0pcad_val register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 swd0pcad_val register
     */
    void gen_decrement_swd0pcad_val(fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the rh850 mem_acc_type register by the specified amount
     * \param value Object representing the amount by which to decrement the value of the rh850 mem_acc_type register
     */
    void gen_decrement_mem_acc_type(fastiss::i32_t *value);

    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 GR register group by the specified amount
     * \param index Index value of the required register from the rh850 GR register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 GR group register
     */
    void gen_decrement_GR(rh850::GR_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 SR_SELID_0 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_0 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 SR_SELID_0 group register
     */
    void gen_decrement_SR_SELID_0(rh850::SR_SELID_0_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 SR_SELID_1 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_1 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 SR_SELID_1 group register
     */
    void gen_decrement_SR_SELID_1(rh850::SR_SELID_1_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 SR_SELID_2 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_2 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 SR_SELID_2 group register
     */
    void gen_decrement_SR_SELID_2(rh850::SR_SELID_2_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 SR_SELID_3 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_3 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 SR_SELID_3 group register
     */
    void gen_decrement_SR_SELID_3(rh850::SR_SELID_3_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 SR_SELID_5 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_5 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 SR_SELID_5 group register
     */
    void gen_decrement_SR_SELID_5(rh850::SR_SELID_5_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 SR_SELID_6 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_6 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 SR_SELID_6 group register
     */
    void gen_decrement_SR_SELID_6(rh850::SR_SELID_6_index_t index, fastiss::i32_t *value);
    /**
     * \brief Generates code to decrement the value of the specified register from the rh850 SR_SELID_7 register group by the specified amount
     * \param index Index value of the required register from the rh850 SR_SELID_7 register group
     * \param value Object representing the amount by which to decrement the value of the specified rh850 SR_SELID_7 group register
     */
    void gen_decrement_SR_SELID_7(rh850::SR_SELID_7_index_t index, fastiss::i32_t *value);

  private:
    /**
     * \brief Handle to CPU state implementation
     */
    rh850::cpu_state_if_t *m_cpu_state_impl;
  };
}

#endif // _RH850_CPU_STATE_BRIDGE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
