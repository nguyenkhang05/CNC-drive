//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file debugger_if.h
 * \brief Defines the debugger interface class
 */

#ifndef _RH850G4_DEBUGGER_IF_H_
#define _RH850G4_DEBUGGER_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <list>
#include <string>
#include "rh850g4/opcode.h"
#include "rh850g4/analysis_types.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace sc_core
{
  class sc_time;
}

//------------------------------------------------------------------------------
// 128 bit value type definition
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief 128-bit type definition, used to enable 128-bit debug access
   * operations to pass parameters and return results by value.
   */
  typedef struct
  {
    /**
     * \brief Character array used to reserve 16 bytes/128-bits of storage.
     */
    unsigned char bytes[16];
  } uint128_t;
}

//------------------------------------------------------------------------------
// Watchpoint kind type definition
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief Public type used to denote the kind of transaction on which
   * a watchpoint should trigger.
   */
  typedef enum
  {
    /**
     * \brief Used to denote that the watchpoint should trigger on read
     * transactions.
     */
    READ_WATCHPOINT = 0,
    /**
     * \brief Used to denote that the watchpoint should trigger on write
     * transactions
     */
    WRITE_WATCHPOINT = 1,
    /**
     * \brief Used to denote that the watchpoint should trigger on both
     * read and write transactions.
     */
    VALUE_WATCHPOINT = 2
  } watchpoint_kind_t;
}

//------------------------------------------------------------------------------
// Debugger interface class
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief Interface providing operations to an external debugger
   */
  class debugger_if_t
  {
  public:
    /**
     * \addtogroup DebuggerInterfaceOperations
     * @{
     */

    /**
     * \brief Returns the total number of instructions that have been executed
     * (for all quanta's)
     * \return The total number of instructions executed
     */
    virtual unsigned long long get_instruction_count(void) = 0;

    /**
     * \brief Request by the external platform to asynchronously stop the
     * ISS execution (during a continue operation)
     */
    virtual void request_stop(void) = 0;

    /**
     * \brief Request by the external platform to clear an asynchronous stop
     * request of the ISS
     */
    virtual void clear_stop(void) = 0;

    /**
     * \brief Returns the local time - the time elapsed by instruction
     * execution since the start of the quanta. This function should only
     * be used whilst the ISS is not executing instructions.
     * \return The local time
     */          
    virtual sc_core::sc_time get_local_time(void) = 0;
    /**
     * @}
     */

    /**
     * \brief Flushes the translation cache, clearing all translated blocks
     * and re-initialising
     */          
    virtual void flush_cache(void) = 0;

    /**
     * \brief Sets the flush required flag, which queues the flushing of the
     * translation cache for the start of the next quantum. At the start of the
     * next quantum all translated blocks are cleared and re-initialised.
     */          
    virtual void set_flush_required(void) = 0;

    /**
     * \addtogroup DebuggerBreakpointOperations
     * @{
     */

    /**
     * \brief Inserts an address into the set of breakpoints. Inserting
     * a breakpoint results in the flushing of the translated block cache.
     * \param bkpt_addr The program counter address to add as a breakpoint
     */          
    virtual void insert_bkpt(unsigned int bkpt_addr) = 0;
    /**
     * \brief Clears an address from the set of breakpoints. Clearing
     * a breakpoint results in the flushing of the translated block cache.
     * \param bkpt_addr The program counter address to remove as a breakpoint
     */          
    virtual void clear_bkpt(unsigned int bkpt_addr) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup DebuggerWatchpointOperations
     * @{
     */

    
    /**
     * \brief Memory subsystem add watchpoint operation.
     * \param address - Integer specifying start address of watchpoint region.
     * \param bus_interface_id - Identifier for bus interface on which the watchpoint should be set.
    * 0 - default bus
     * \param size - Integer specifying size of watchpoint region in bytes.
     * \param kind - Specifies the kind of transaction on which the watchpoint
     * should trigger, ie. read, write or value (either read or write).
     * \return Watchpoint descriptor for the new watchpoint, these values are used
     * by the erase_watchpoint() and pending_watchpoints() operations.
     */
    virtual unsigned int add_watchpoint(unsigned int address, unsigned int bus_interface_id, unsigned int size, watchpoint_kind_t kind) = 0;

    /**
     * \brief Memory subsystem erase watchpoint operation - removes first equivalent
     * watchpoint.
     * \param wp - Descriptor corresponding to the watchpoint to be deleted
     */
    virtual void erase_watchpoint(unsigned int wp) = 0;

    /**
     * \brief Memory subsystem clear all watchpoints operation. Note that the translation cache
     * should always flush after this operation.
     */
    virtual void clear_all_watchpoints() = 0;

    /**
     * \brief Memory subsystem operation which returns a list of pending watchpoint
     * descriptors, note that calling this operation clears the list of pending watchpoints.
     * \return List of pending watchpoint descriptors
     */
    virtual std::list< unsigned int > pending_watchpoints(void) = 0;

    /**
     * @}
     */

    /**
     * \addtogroup DebuggerMemoryAccessOperations
     * @{
     */
    
    /**
     * \brief Loads an arbitrary size value from memory
     * \param addr The address for the read
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually read
     */          
    virtual unsigned int iN_load(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * \brief Loads an arbitrary size value from memory at a physical address
     * \param addr The physical address for the read
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually read
     */          
    virtual unsigned int iN_load_physical(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * \brief Stores an arbitrary size value to memory
     * \param addr The address for the read
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually written
     */          
    virtual unsigned int iN_store(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * \brief Stores an arbitrary size value to memory at a physical address
     * \param addr The physical address for the write
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually written
     */          
    virtual unsigned int iN_store_physical(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    
    
    /**
     * @}
     */

    /**
     * \addtogroup DebuggerRegisterAccessOperations
     * @{
     */

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
     * \brief Queries the value of the rh850g4 FPSR register
     * \return Value of the rh850g4 FPSR register
     */
    virtual unsigned int read_FPSR(void) = 0;
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
     * \brief Queries the value of the rh850g4 FPCC register
     * \return Value of the rh850g4 FPCC register
     */
    virtual unsigned int read_FPCC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FPCFG register
     * \return Value of the rh850g4 FPCFG register
     */
    virtual unsigned int read_FPCFG(void) = 0;
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
     * \brief Queries the value of the rh850g4 FEPSWH register
     * \return Value of the rh850g4 FEPSWH register
     */
    virtual unsigned int read_FEPSWH(void) = 0;
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
     * \brief Queries the value of the rh850g4 EBASE register
     * \return Value of the rh850g4 EBASE register
     */
    virtual unsigned int read_EBASE(void) = 0;
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
     * \brief Queries the value of the rh850g4 GMCFG register
     * \return Value of the rh850g4 GMCFG register
     */
    virtual unsigned int read_GMCFG(void) = 0;
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
     * \brief Queries the value of the rh850g4 RBCR1 register
     * \return Value of the rh850g4 RBCR1 register
     */
    virtual unsigned int read_RBCR1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBNR register
     * \return Value of the rh850g4 RBNR register
     */
    virtual unsigned int read_RBNR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 RBIP register
     * \return Value of the rh850g4 RBIP register
     */
    virtual unsigned int read_RBIP(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ISPR register
     * \return Value of the rh850g4 ISPR register
     */
    virtual unsigned int read_ISPR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 IMSR register
     * \return Value of the rh850g4 IMSR register
     */
    virtual unsigned int read_IMSR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICSR register
     * \return Value of the rh850g4 ICSR register
     */
    virtual unsigned int read_ICSR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 INTCFG register
     * \return Value of the rh850g4 INTCFG register
     */
    virtual unsigned int read_INTCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 PLMR register
     * \return Value of the rh850g4 PLMR register
     */
    virtual unsigned int read_PLMR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXSR register
     * \return Value of the rh850g4 FXSR register
     */
    virtual unsigned int read_FXSR(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXST register
     * \return Value of the rh850g4 FXST register
     */
    virtual unsigned int read_FXST(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXINFO register
     * \return Value of the rh850g4 FXINFO register
     */
    virtual unsigned int read_FXINFO(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXCFG register
     * \return Value of the rh850g4 FXCFG register
     */
    virtual unsigned int read_FXCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXC register
     * \return Value of the rh850g4 FXXC register
     */
    virtual unsigned int read_FXXC(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 FXXP register
     * \return Value of the rh850g4 FXXP register
     */
    virtual unsigned int read_FXXP(void) = 0;
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
     * \brief Queries the value of the rh850g4 GMPLMR register
     * \return Value of the rh850g4 GMPLMR register
     */
    virtual unsigned int read_GMPLMR(void) = 0;
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
     * \brief Queries the value of the rh850g4 MCI register
     * \return Value of the rh850g4 MCI register
     */
    virtual unsigned int read_MCI(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPIDX register
     * \return Value of the rh850g4 MPIDX register
     */
    virtual unsigned int read_MPIDX(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPBK register
     * \return Value of the rh850g4 MPBK register
     */
    virtual unsigned int read_MPBK(void) = 0;
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
     * \brief Queries the value of the rh850g4 MPID0 register
     * \return Value of the rh850g4 MPID0 register
     */
    virtual unsigned int read_MPID0(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID1 register
     * \return Value of the rh850g4 MPID1 register
     */
    virtual unsigned int read_MPID1(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID2 register
     * \return Value of the rh850g4 MPID2 register
     */
    virtual unsigned int read_MPID2(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID3 register
     * \return Value of the rh850g4 MPID3 register
     */
    virtual unsigned int read_MPID3(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID4 register
     * \return Value of the rh850g4 MPID4 register
     */
    virtual unsigned int read_MPID4(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID5 register
     * \return Value of the rh850g4 MPID5 register
     */
    virtual unsigned int read_MPID5(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID6 register
     * \return Value of the rh850g4 MPID6 register
     */
    virtual unsigned int read_MPID6(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 MPID7 register
     * \return Value of the rh850g4 MPID7 register
     */
    virtual unsigned int read_MPID7(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGL register
     * \return Value of the rh850g4 ICTAGL register
     */
    virtual unsigned int read_ICTAGL(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICTAGH register
     * \return Value of the rh850g4 ICTAGH register
     */
    virtual unsigned int read_ICTAGH(void) = 0;
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
     * \brief Queries the value of the rh850g4 ICCFG register
     * \return Value of the rh850g4 ICCFG register
     */
    virtual unsigned int read_ICCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850g4 ICERR register
     * \return Value of the rh850g4 ICERR register
     */
    virtual unsigned int read_ICERR(void) = 0;
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
     * \brief Sets the value of the rh850g4 FPSR register
     * \param value New value of the rh850g4 FPSR register
     */
    virtual void write_FPSR(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 FPCC register
     * \param value New value of the rh850g4 FPCC register
     */
    virtual void write_FPCC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FPCFG register
     * \param value New value of the rh850g4 FPCFG register
     */
    virtual void write_FPCFG(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 FEPSWH register
     * \param value New value of the rh850g4 FEPSWH register
     */
    virtual void write_FEPSWH(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 EBASE register
     * \param value New value of the rh850g4 EBASE register
     */
    virtual void write_EBASE(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 GMCFG register
     * \param value New value of the rh850g4 GMCFG register
     */
    virtual void write_GMCFG(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 RBCR1 register
     * \param value New value of the rh850g4 RBCR1 register
     */
    virtual void write_RBCR1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBNR register
     * \param value New value of the rh850g4 RBNR register
     */
    virtual void write_RBNR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 RBIP register
     * \param value New value of the rh850g4 RBIP register
     */
    virtual void write_RBIP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ISPR register
     * \param value New value of the rh850g4 ISPR register
     */
    virtual void write_ISPR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 IMSR register
     * \param value New value of the rh850g4 IMSR register
     */
    virtual void write_IMSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICSR register
     * \param value New value of the rh850g4 ICSR register
     */
    virtual void write_ICSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 INTCFG register
     * \param value New value of the rh850g4 INTCFG register
     */
    virtual void write_INTCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 PLMR register
     * \param value New value of the rh850g4 PLMR register
     */
    virtual void write_PLMR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXSR register
     * \param value New value of the rh850g4 FXSR register
     */
    virtual void write_FXSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXST register
     * \param value New value of the rh850g4 FXST register
     */
    virtual void write_FXST(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXINFO register
     * \param value New value of the rh850g4 FXINFO register
     */
    virtual void write_FXINFO(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXCFG register
     * \param value New value of the rh850g4 FXCFG register
     */
    virtual void write_FXCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXC register
     * \param value New value of the rh850g4 FXXC register
     */
    virtual void write_FXXC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 FXXP register
     * \param value New value of the rh850g4 FXXP register
     */
    virtual void write_FXXP(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 GMPLMR register
     * \param value New value of the rh850g4 GMPLMR register
     */
    virtual void write_GMPLMR(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 MCI register
     * \param value New value of the rh850g4 MCI register
     */
    virtual void write_MCI(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPIDX register
     * \param value New value of the rh850g4 MPIDX register
     */
    virtual void write_MPIDX(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPBK register
     * \param value New value of the rh850g4 MPBK register
     */
    virtual void write_MPBK(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 MPID0 register
     * \param value New value of the rh850g4 MPID0 register
     */
    virtual void write_MPID0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID1 register
     * \param value New value of the rh850g4 MPID1 register
     */
    virtual void write_MPID1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID2 register
     * \param value New value of the rh850g4 MPID2 register
     */
    virtual void write_MPID2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID3 register
     * \param value New value of the rh850g4 MPID3 register
     */
    virtual void write_MPID3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID4 register
     * \param value New value of the rh850g4 MPID4 register
     */
    virtual void write_MPID4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID5 register
     * \param value New value of the rh850g4 MPID5 register
     */
    virtual void write_MPID5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID6 register
     * \param value New value of the rh850g4 MPID6 register
     */
    virtual void write_MPID6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 MPID7 register
     * \param value New value of the rh850g4 MPID7 register
     */
    virtual void write_MPID7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGL register
     * \param value New value of the rh850g4 ICTAGL register
     */
    virtual void write_ICTAGL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICTAGH register
     * \param value New value of the rh850g4 ICTAGH register
     */
    virtual void write_ICTAGH(unsigned int value) = 0;
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
     * \brief Sets the value of the rh850g4 ICCFG register
     * \param value New value of the rh850g4 ICCFG register
     */
    virtual void write_ICCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850g4 ICERR register
     * \param value New value of the rh850g4 ICERR register
     */
    virtual void write_ICERR(unsigned int value) = 0;
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
     * \addtogroup AnalysisPointManagementAPI
     * @{
     */
    /**
     * \brief Returns list of all registered analysis point selector values
     * \return List of registered analysis point selector values
     */
    virtual std::list<std::string> get_analysis_point_selectors(void) = 0;

    /**
     * \brief Queries whether analysis hook specified by the analysis hook
     * selector accepts attachment of analysis predicate operations
     * \param analysis_point_selector Name used to identify analysis point
     * \return Boolean value indicating whether the analysis point accepts
     * attachment of analysis predicate operations
     */
    virtual bool accepts_predicates(std::string analysis_point_selector) = 0;

#if !defined(__VLAB_MIT__)    
    /**
     * \brief Attaches an analysis callback to the specified analysis point.
     * This causes a call to the specified analysis callback to be generated
     * when the specified analysis point is activated. This operation may be
     * used with all analysis points
     * \param analysis_point_selector Name used to identify analysis point
     * \param callback Handle to analysis callback operation to be invoked when
     * the analysis point is activated
     * \param callback_data Data to be passed to analysis callback when the
     * analysis point is activated
     * \return Analysis attachment descriptor used for subsequent calls to detach().
     */
    virtual analysis_attachment_descriptor *attach(std::string analysis_point_selector, analysis_callback callback, void *callback_data) = 0; 
    
    /**
     * \brief Attaches an analysis predicate to the specified analysis point.
     * This causes a call to the specified analysis predicate to be generated
     * when the specified analysis point is activated. The response value from
     * the analysis predicate is used to generate a call to the registered
     * analysis callback or increment the analysis counter identified by the
     * analysis predicate. Note that this operation should only be used with
     * analysis points that accept the attachment of analysis predicates
     * \param analysis_point_selector Name used to identify analysis point
     * \param predicate Handle to analysis predicate operation to be invoked
     * when the analysis point is activated
     * \param predicate_data Data to be passed to analysis predicate when the
     * analysis point is activated
     * \return Analysis attachment descriptor used for subsequent calls to detach().
     */
    virtual analysis_attachment_descriptor *attach(std::string analysis_point_selector, analysis_predicate predicate, void *predicate_data) = 0; 
        
    /**
     * \brief Removes an analysis callback or analysis predicate operation
     * from an analysis point
     * \param attachment Analysis attachment descriptor generated by a prior
     * call to attach(). This identified both the analysis callback or analysis
     * predicate and the analysis point that are to be disassociated
     */
    virtual void detach(analysis_attachment_descriptor *attachment) = 0;
#endif
    /**
     * @}
     */

    /**
     * \addtogroup AnalysisTransactionQueryAPI
     * @{
     */

    /**
     * \brief Query the last transaction address. Note that this is only valid
     * within a callout associated with a memory transaction.
     * \return Last transaction address.
     */
    virtual unsigned int read_last_access_addr() = 0;

    /**
     * \brief Query the last transaction size. Note that this is only valid
     * within a callout associated with a memory transaction.
     * \return Last transaction size.
     */
    virtual unsigned char read_last_access_size() = 0;
    
    /**
     * \brief Query the last transaction data. Note that this is only valid
     * within a callout associated with a memory transaction.
     * \return Last transaction data.
     */
    virtual unsigned long long read_last_access_data() = 0;
    /**
     * @}
     */

    /**
     * \addtogroup AnalysisTranslationQueryAPI
     * @{
     */
     
    /**
     * \brief Reads the opcode of the instruction currently in the
     * decode translation phase.
     * \return Opcode value
     */
    virtual rh850g4::opcode_t get_decode_opcode() = 0;
    /**
     * \brief Reads the opcode of the instruction currently in the
     * execute translation phase.
     * \return Opcode value
     */
    virtual rh850g4::opcode_t get_execute_opcode() = 0;
    /**
     * \brief Reads the value of PC for the instruction currently in the
     * decode translation phase.
     * \return PC value
     */
    virtual unsigned int get_decode_PC() = 0;
    /**
     * \brief Reads the value of PC for the instruction currently in the
     * execute translation phase.
     * \return PC value
     */
    virtual unsigned int get_execute_PC() = 0;
    /**
     * \brief Gets a disassembly string for the instruction currently in the
     * decode translation phase.
     * \return Disassembly string
     */
    virtual std::string get_decode_disass() = 0;
    /**
     * \brief Gets a disassembly string for the instruction currently in the
     * execute translation phase.
     * \return Disassembly string
     */
    virtual std::string get_execute_disass() = 0;
    /**
     * \brief Reads the value of the 'terminates_block' attribute of the instruction currently in the
     * decode translation phase.
     * \return Boolean representing value of instruction 'terminates_block' attribute
     */
    virtual bool get_decode_terminates_block() = 0;
    /**
     * \brief Reads the value of the 'cycle_count' attribute of the instruction currently in the
     * decode translation phase.
     * \return Unsigned 32-bit integer representing value of instruction 'cycle_count' attribute
     */
    virtual unsigned int get_decode_cycle_count() = 0;
    /**
     * \brief Reads the value of the 'terminates_block' attribute of the instruction currently in the
     * execute translation phase.
     * \return Boolean representing value of instruction 'terminates_block' attribute
     */
    virtual bool get_execute_terminates_block() = 0;
    /**
     * \brief Reads the value of the 'cycle_count' attribute of the instruction currently in the
     * execute translation phase.
     * \return Unsigned 32-bit integer representing value of instruction 'cycle_count' attribute
     */
    virtual unsigned int get_execute_cycle_count() = 0;
    /**
     * \brief Reads the length in bytes of the instruction currently in the
     * decode translation phase.
     * \return Integer value representing length of the instruction
     */
    virtual unsigned int get_decode_length() = 0;
    /**
     * \brief Reads the length in bytes of the instruction currently in the
     * execute translation phase.
     * \return Integer value representing length of the instruction
     */
    virtual unsigned int get_execute_length() = 0;
    /**
     * \brief Indicates whether the instruction currently in the
     * decode translation phase is valid.
     * \return Boolean indicating whether the instruction is valid
     */
    virtual bool is_decode_valid() = 0;
    /**
     * \brief Indicates whether the instruction currently in the
     * execute translation phase is valid.
     * \return Boolean indicating whether the instruction is valid
     */
    virtual bool is_execute_valid() = 0;
    /**
     * @}
     */
  };
}

#endif // _RH850G4_DEBUGGER_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
