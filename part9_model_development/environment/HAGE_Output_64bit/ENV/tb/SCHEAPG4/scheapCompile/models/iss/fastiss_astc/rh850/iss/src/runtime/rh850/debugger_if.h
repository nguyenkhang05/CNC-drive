//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
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

#ifndef _RH850_DEBUGGER_IF_H_
#define _RH850_DEBUGGER_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <list>
#include <string>
#include "rh850/opcode.h"
#include "rh850/analysis_types.h"

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

namespace rh850
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

namespace rh850
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

namespace rh850
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
     * \brief Queries the value of the rh850 r0 register
     * \return Value of the rh850 r0 register
     */
    virtual unsigned int read_r0(void) = 0;
    /**
     * \brief Queries the value of the rh850 r1 register
     * \return Value of the rh850 r1 register
     */
    virtual unsigned int read_r1(void) = 0;
    /**
     * \brief Queries the value of the rh850 r2 register
     * \return Value of the rh850 r2 register
     */
    virtual unsigned int read_r2(void) = 0;
    /**
     * \brief Queries the value of the rh850 r3 register
     * \return Value of the rh850 r3 register
     */
    virtual unsigned int read_r3(void) = 0;
    /**
     * \brief Queries the value of the rh850 r4 register
     * \return Value of the rh850 r4 register
     */
    virtual unsigned int read_r4(void) = 0;
    /**
     * \brief Queries the value of the rh850 r5 register
     * \return Value of the rh850 r5 register
     */
    virtual unsigned int read_r5(void) = 0;
    /**
     * \brief Queries the value of the rh850 r6 register
     * \return Value of the rh850 r6 register
     */
    virtual unsigned int read_r6(void) = 0;
    /**
     * \brief Queries the value of the rh850 r7 register
     * \return Value of the rh850 r7 register
     */
    virtual unsigned int read_r7(void) = 0;
    /**
     * \brief Queries the value of the rh850 r8 register
     * \return Value of the rh850 r8 register
     */
    virtual unsigned int read_r8(void) = 0;
    /**
     * \brief Queries the value of the rh850 r9 register
     * \return Value of the rh850 r9 register
     */
    virtual unsigned int read_r9(void) = 0;
    /**
     * \brief Queries the value of the rh850 r10 register
     * \return Value of the rh850 r10 register
     */
    virtual unsigned int read_r10(void) = 0;
    /**
     * \brief Queries the value of the rh850 r11 register
     * \return Value of the rh850 r11 register
     */
    virtual unsigned int read_r11(void) = 0;
    /**
     * \brief Queries the value of the rh850 r12 register
     * \return Value of the rh850 r12 register
     */
    virtual unsigned int read_r12(void) = 0;
    /**
     * \brief Queries the value of the rh850 r13 register
     * \return Value of the rh850 r13 register
     */
    virtual unsigned int read_r13(void) = 0;
    /**
     * \brief Queries the value of the rh850 r14 register
     * \return Value of the rh850 r14 register
     */
    virtual unsigned int read_r14(void) = 0;
    /**
     * \brief Queries the value of the rh850 r15 register
     * \return Value of the rh850 r15 register
     */
    virtual unsigned int read_r15(void) = 0;
    /**
     * \brief Queries the value of the rh850 r16 register
     * \return Value of the rh850 r16 register
     */
    virtual unsigned int read_r16(void) = 0;
    /**
     * \brief Queries the value of the rh850 r17 register
     * \return Value of the rh850 r17 register
     */
    virtual unsigned int read_r17(void) = 0;
    /**
     * \brief Queries the value of the rh850 r18 register
     * \return Value of the rh850 r18 register
     */
    virtual unsigned int read_r18(void) = 0;
    /**
     * \brief Queries the value of the rh850 r19 register
     * \return Value of the rh850 r19 register
     */
    virtual unsigned int read_r19(void) = 0;
    /**
     * \brief Queries the value of the rh850 r20 register
     * \return Value of the rh850 r20 register
     */
    virtual unsigned int read_r20(void) = 0;
    /**
     * \brief Queries the value of the rh850 r21 register
     * \return Value of the rh850 r21 register
     */
    virtual unsigned int read_r21(void) = 0;
    /**
     * \brief Queries the value of the rh850 r22 register
     * \return Value of the rh850 r22 register
     */
    virtual unsigned int read_r22(void) = 0;
    /**
     * \brief Queries the value of the rh850 r23 register
     * \return Value of the rh850 r23 register
     */
    virtual unsigned int read_r23(void) = 0;
    /**
     * \brief Queries the value of the rh850 r24 register
     * \return Value of the rh850 r24 register
     */
    virtual unsigned int read_r24(void) = 0;
    /**
     * \brief Queries the value of the rh850 r25 register
     * \return Value of the rh850 r25 register
     */
    virtual unsigned int read_r25(void) = 0;
    /**
     * \brief Queries the value of the rh850 r26 register
     * \return Value of the rh850 r26 register
     */
    virtual unsigned int read_r26(void) = 0;
    /**
     * \brief Queries the value of the rh850 r27 register
     * \return Value of the rh850 r27 register
     */
    virtual unsigned int read_r27(void) = 0;
    /**
     * \brief Queries the value of the rh850 r28 register
     * \return Value of the rh850 r28 register
     */
    virtual unsigned int read_r28(void) = 0;
    /**
     * \brief Queries the value of the rh850 r29 register
     * \return Value of the rh850 r29 register
     */
    virtual unsigned int read_r29(void) = 0;
    /**
     * \brief Queries the value of the rh850 r30 register
     * \return Value of the rh850 r30 register
     */
    virtual unsigned int read_r30(void) = 0;
    /**
     * \brief Queries the value of the rh850 r31 register
     * \return Value of the rh850 r31 register
     */
    virtual unsigned int read_r31(void) = 0;
    /**
     * \brief Queries the value of the rh850 PC register
     * \return Value of the rh850 PC register
     */
    virtual unsigned int read_PC(void) = 0;
    /**
     * \brief Queries the value of the rh850 EIPC register
     * \return Value of the rh850 EIPC register
     */
    virtual unsigned int read_EIPC(void) = 0;
    /**
     * \brief Queries the value of the rh850 EIPSW register
     * \return Value of the rh850 EIPSW register
     */
    virtual unsigned int read_EIPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850 FEPC register
     * \return Value of the rh850 FEPC register
     */
    virtual unsigned int read_FEPC(void) = 0;
    /**
     * \brief Queries the value of the rh850 FEPSW register
     * \return Value of the rh850 FEPSW register
     */
    virtual unsigned int read_FEPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850 PSW register
     * \return Value of the rh850 PSW register
     */
    virtual unsigned int read_PSW(void) = 0;
    /**
     * \brief Queries the value of the rh850 FPSR register
     * \return Value of the rh850 FPSR register
     */
    virtual unsigned int read_FPSR(void) = 0;
    /**
     * \brief Queries the value of the rh850 FPEPC register
     * \return Value of the rh850 FPEPC register
     */
    virtual unsigned int read_FPEPC(void) = 0;
    /**
     * \brief Queries the value of the rh850 FPST register
     * \return Value of the rh850 FPST register
     */
    virtual unsigned int read_FPST(void) = 0;
    /**
     * \brief Queries the value of the rh850 FPCC register
     * \return Value of the rh850 FPCC register
     */
    virtual unsigned int read_FPCC(void) = 0;
    /**
     * \brief Queries the value of the rh850 FPCFG register
     * \return Value of the rh850 FPCFG register
     */
    virtual unsigned int read_FPCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850 FPEC register
     * \return Value of the rh850 FPEC register
     */
    virtual unsigned int read_FPEC(void) = 0;
    /**
     * \brief Queries the value of the rh850 EIIC register
     * \return Value of the rh850 EIIC register
     */
    virtual unsigned int read_EIIC(void) = 0;
    /**
     * \brief Queries the value of the rh850 FEIC register
     * \return Value of the rh850 FEIC register
     */
    virtual unsigned int read_FEIC(void) = 0;
    /**
     * \brief Queries the value of the rh850 CTPC register
     * \return Value of the rh850 CTPC register
     */
    virtual unsigned int read_CTPC(void) = 0;
    /**
     * \brief Queries the value of the rh850 CTPSW register
     * \return Value of the rh850 CTPSW register
     */
    virtual unsigned int read_CTPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850 CTBP register
     * \return Value of the rh850 CTBP register
     */
    virtual unsigned int read_CTBP(void) = 0;
    /**
     * \brief Queries the value of the rh850 EIWR register
     * \return Value of the rh850 EIWR register
     */
    virtual unsigned int read_EIWR(void) = 0;
    /**
     * \brief Queries the value of the rh850 FEWR register
     * \return Value of the rh850 FEWR register
     */
    virtual unsigned int read_FEWR(void) = 0;
    /**
     * \brief Queries the value of the rh850 BSEL register
     * \return Value of the rh850 BSEL register
     */
    virtual unsigned int read_BSEL(void) = 0;
    /**
     * \brief Queries the value of the rh850 MCFG0 register
     * \return Value of the rh850 MCFG0 register
     */
    virtual unsigned int read_MCFG0(void) = 0;
    /**
     * \brief Queries the value of the rh850 RBASE register
     * \return Value of the rh850 RBASE register
     */
    virtual unsigned int read_RBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850 EBASE register
     * \return Value of the rh850 EBASE register
     */
    virtual unsigned int read_EBASE(void) = 0;
    /**
     * \brief Queries the value of the rh850 INTBP register
     * \return Value of the rh850 INTBP register
     */
    virtual unsigned int read_INTBP(void) = 0;
    /**
     * \brief Queries the value of the rh850 MCTL register
     * \return Value of the rh850 MCTL register
     */
    virtual unsigned int read_MCTL(void) = 0;
    /**
     * \brief Queries the value of the rh850 PID register
     * \return Value of the rh850 PID register
     */
    virtual unsigned int read_PID(void) = 0;
    /**
     * \brief Queries the value of the rh850 FPIPR register
     * \return Value of the rh850 FPIPR register
     */
    virtual unsigned int read_FPIPR(void) = 0;
    /**
     * \brief Queries the value of the rh850 SCCFG register
     * \return Value of the rh850 SCCFG register
     */
    virtual unsigned int read_SCCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850 SCBP register
     * \return Value of the rh850 SCBP register
     */
    virtual unsigned int read_SCBP(void) = 0;
    /**
     * \brief Queries the value of the rh850 HTCFG0 register
     * \return Value of the rh850 HTCFG0 register
     */
    virtual unsigned int read_HTCFG0(void) = 0;
    /**
     * \brief Queries the value of the rh850 HVCCFG register
     * \return Value of the rh850 HVCCFG register
     */
    virtual unsigned int read_HVCCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850 HVCBP register
     * \return Value of the rh850 HVCBP register
     */
    virtual unsigned int read_HVCBP(void) = 0;
    /**
     * \brief Queries the value of the rh850 MEA register
     * \return Value of the rh850 MEA register
     */
    virtual unsigned int read_MEA(void) = 0;
    /**
     * \brief Queries the value of the rh850 ASID register
     * \return Value of the rh850 ASID register
     */
    virtual unsigned int read_ASID(void) = 0;
    /**
     * \brief Queries the value of the rh850 MEI register
     * \return Value of the rh850 MEI register
     */
    virtual unsigned int read_MEI(void) = 0;
    /**
     * \brief Queries the value of the rh850 ISPR register
     * \return Value of the rh850 ISPR register
     */
    virtual unsigned int read_ISPR(void) = 0;
    /**
     * \brief Queries the value of the rh850 PMR register
     * \return Value of the rh850 PMR register
     */
    virtual unsigned int read_PMR(void) = 0;
    /**
     * \brief Queries the value of the rh850 ICSR register
     * \return Value of the rh850 ICSR register
     */
    virtual unsigned int read_ICSR(void) = 0;
    /**
     * \brief Queries the value of the rh850 INTCFG register
     * \return Value of the rh850 INTCFG register
     */
    virtual unsigned int read_INTCFG(void) = 0;
    /**
     * \brief Queries the value of the rh850 DBIC register
     * \return Value of the rh850 DBIC register
     */
    virtual unsigned int read_DBIC(void) = 0;
    /**
     * \brief Queries the value of the rh850 DBCMC register
     * \return Value of the rh850 DBCMC register
     */
    virtual unsigned int read_DBCMC(void) = 0;
    /**
     * \brief Queries the value of the rh850 DBPC register
     * \return Value of the rh850 DBPC register
     */
    virtual unsigned int read_DBPC(void) = 0;
    /**
     * \brief Queries the value of the rh850 DBPSW register
     * \return Value of the rh850 DBPSW register
     */
    virtual unsigned int read_DBPSW(void) = 0;
    /**
     * \brief Queries the value of the rh850 DIR0 register
     * \return Value of the rh850 DIR0 register
     */
    virtual unsigned int read_DIR0(void) = 0;
    /**
     * \brief Queries the value of the rh850 DIR1 register
     * \return Value of the rh850 DIR1 register
     */
    virtual unsigned int read_DIR1(void) = 0;
    /**
     * \brief Queries the value of the rh850 BPC register
     * \return Value of the rh850 BPC register
     */
    virtual unsigned int read_BPC(void) = 0;
    /**
     * \brief Queries the value of the rh850 BPAV register
     * \return Value of the rh850 BPAV register
     */
    virtual unsigned int read_BPAV(void) = 0;
    /**
     * \brief Queries the value of the rh850 BPAM register
     * \return Value of the rh850 BPAM register
     */
    virtual unsigned int read_BPAM(void) = 0;
    /**
     * \brief Queries the value of the rh850 BPDV register
     * \return Value of the rh850 BPDV register
     */
    virtual unsigned int read_BPDV(void) = 0;
    /**
     * \brief Queries the value of the rh850 BPDM register
     * \return Value of the rh850 BPDM register
     */
    virtual unsigned int read_BPDM(void) = 0;
    /**
     * \brief Queries the value of the rh850 DBWR register
     * \return Value of the rh850 DBWR register
     */
    virtual unsigned int read_DBWR(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPM register
     * \return Value of the rh850 MPM register
     */
    virtual unsigned int read_MPM(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPRC register
     * \return Value of the rh850 MPRC register
     */
    virtual unsigned int read_MPRC(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPBRGN register
     * \return Value of the rh850 MPBRGN register
     */
    virtual unsigned int read_MPBRGN(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPTRGN register
     * \return Value of the rh850 MPTRGN register
     */
    virtual unsigned int read_MPTRGN(void) = 0;
    /**
     * \brief Queries the value of the rh850 MCR register
     * \return Value of the rh850 MCR register
     */
    virtual unsigned int read_MCR(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA0 register
     * \return Value of the rh850 MPLA0 register
     */
    virtual unsigned int read_MPLA0(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA0 register
     * \return Value of the rh850 MPUA0 register
     */
    virtual unsigned int read_MPUA0(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT0 register
     * \return Value of the rh850 MPAT0 register
     */
    virtual unsigned int read_MPAT0(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA1 register
     * \return Value of the rh850 MPLA1 register
     */
    virtual unsigned int read_MPLA1(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA1 register
     * \return Value of the rh850 MPUA1 register
     */
    virtual unsigned int read_MPUA1(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT1 register
     * \return Value of the rh850 MPAT1 register
     */
    virtual unsigned int read_MPAT1(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA2 register
     * \return Value of the rh850 MPLA2 register
     */
    virtual unsigned int read_MPLA2(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA2 register
     * \return Value of the rh850 MPUA2 register
     */
    virtual unsigned int read_MPUA2(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT2 register
     * \return Value of the rh850 MPAT2 register
     */
    virtual unsigned int read_MPAT2(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA3 register
     * \return Value of the rh850 MPLA3 register
     */
    virtual unsigned int read_MPLA3(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA3 register
     * \return Value of the rh850 MPUA3 register
     */
    virtual unsigned int read_MPUA3(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT3 register
     * \return Value of the rh850 MPAT3 register
     */
    virtual unsigned int read_MPAT3(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA4 register
     * \return Value of the rh850 MPLA4 register
     */
    virtual unsigned int read_MPLA4(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA4 register
     * \return Value of the rh850 MPUA4 register
     */
    virtual unsigned int read_MPUA4(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT4 register
     * \return Value of the rh850 MPAT4 register
     */
    virtual unsigned int read_MPAT4(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA5 register
     * \return Value of the rh850 MPLA5 register
     */
    virtual unsigned int read_MPLA5(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA5 register
     * \return Value of the rh850 MPUA5 register
     */
    virtual unsigned int read_MPUA5(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT5 register
     * \return Value of the rh850 MPAT5 register
     */
    virtual unsigned int read_MPAT5(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA6 register
     * \return Value of the rh850 MPLA6 register
     */
    virtual unsigned int read_MPLA6(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA6 register
     * \return Value of the rh850 MPUA6 register
     */
    virtual unsigned int read_MPUA6(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT6 register
     * \return Value of the rh850 MPAT6 register
     */
    virtual unsigned int read_MPAT6(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA7 register
     * \return Value of the rh850 MPLA7 register
     */
    virtual unsigned int read_MPLA7(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA7 register
     * \return Value of the rh850 MPUA7 register
     */
    virtual unsigned int read_MPUA7(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT7 register
     * \return Value of the rh850 MPAT7 register
     */
    virtual unsigned int read_MPAT7(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA8 register
     * \return Value of the rh850 MPLA8 register
     */
    virtual unsigned int read_MPLA8(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA8 register
     * \return Value of the rh850 MPUA8 register
     */
    virtual unsigned int read_MPUA8(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT8 register
     * \return Value of the rh850 MPAT8 register
     */
    virtual unsigned int read_MPAT8(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA9 register
     * \return Value of the rh850 MPLA9 register
     */
    virtual unsigned int read_MPLA9(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA9 register
     * \return Value of the rh850 MPUA9 register
     */
    virtual unsigned int read_MPUA9(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT9 register
     * \return Value of the rh850 MPAT9 register
     */
    virtual unsigned int read_MPAT9(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA10 register
     * \return Value of the rh850 MPLA10 register
     */
    virtual unsigned int read_MPLA10(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA10 register
     * \return Value of the rh850 MPUA10 register
     */
    virtual unsigned int read_MPUA10(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT10 register
     * \return Value of the rh850 MPAT10 register
     */
    virtual unsigned int read_MPAT10(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA11 register
     * \return Value of the rh850 MPLA11 register
     */
    virtual unsigned int read_MPLA11(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA11 register
     * \return Value of the rh850 MPUA11 register
     */
    virtual unsigned int read_MPUA11(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT11 register
     * \return Value of the rh850 MPAT11 register
     */
    virtual unsigned int read_MPAT11(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA12 register
     * \return Value of the rh850 MPLA12 register
     */
    virtual unsigned int read_MPLA12(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA12 register
     * \return Value of the rh850 MPUA12 register
     */
    virtual unsigned int read_MPUA12(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT12 register
     * \return Value of the rh850 MPAT12 register
     */
    virtual unsigned int read_MPAT12(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA13 register
     * \return Value of the rh850 MPLA13 register
     */
    virtual unsigned int read_MPLA13(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA13 register
     * \return Value of the rh850 MPUA13 register
     */
    virtual unsigned int read_MPUA13(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT13 register
     * \return Value of the rh850 MPAT13 register
     */
    virtual unsigned int read_MPAT13(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA14 register
     * \return Value of the rh850 MPLA14 register
     */
    virtual unsigned int read_MPLA14(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA14 register
     * \return Value of the rh850 MPUA14 register
     */
    virtual unsigned int read_MPUA14(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT14 register
     * \return Value of the rh850 MPAT14 register
     */
    virtual unsigned int read_MPAT14(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPLA15 register
     * \return Value of the rh850 MPLA15 register
     */
    virtual unsigned int read_MPLA15(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPUA15 register
     * \return Value of the rh850 MPUA15 register
     */
    virtual unsigned int read_MPUA15(void) = 0;
    /**
     * \brief Queries the value of the rh850 MPAT15 register
     * \return Value of the rh850 MPAT15 register
     */
    virtual unsigned int read_MPAT15(void) = 0;
    /**
     * \brief Queries the value of the rh850 mpu_entry register
     * \return Value of the rh850 mpu_entry register
     */
    virtual unsigned char read_mpu_entry(void) = 0;
    /**
     * \brief Queries the value of the rh850 mpat_g_support register
     * \return Value of the rh850 mpat_g_support register
     */
    virtual unsigned char read_mpat_g_support(void) = 0;
    /**
     * \brief Queries the value of the rh850 swd0pcad_exec_count register
     * \return Value of the rh850 swd0pcad_exec_count register
     */
    virtual unsigned int read_swd0pcad_exec_count(void) = 0;
    /**
     * \brief Queries the value of the rh850 swd0pcad_val register
     * \return Value of the rh850 swd0pcad_val register
     */
    virtual unsigned int read_swd0pcad_val(void) = 0;
    /**
     * \brief Queries the value of the rh850 mem_acc_type register
     * \return Value of the rh850 mem_acc_type register
     */
    virtual unsigned int read_mem_acc_type(void) = 0;

    /**
     * \brief Sets the value of the rh850 r0 register
     * \param value New value of the rh850 r0 register
     */
    virtual void write_r0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r1 register
     * \param value New value of the rh850 r1 register
     */
    virtual void write_r1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r2 register
     * \param value New value of the rh850 r2 register
     */
    virtual void write_r2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r3 register
     * \param value New value of the rh850 r3 register
     */
    virtual void write_r3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r4 register
     * \param value New value of the rh850 r4 register
     */
    virtual void write_r4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r5 register
     * \param value New value of the rh850 r5 register
     */
    virtual void write_r5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r6 register
     * \param value New value of the rh850 r6 register
     */
    virtual void write_r6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r7 register
     * \param value New value of the rh850 r7 register
     */
    virtual void write_r7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r8 register
     * \param value New value of the rh850 r8 register
     */
    virtual void write_r8(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r9 register
     * \param value New value of the rh850 r9 register
     */
    virtual void write_r9(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r10 register
     * \param value New value of the rh850 r10 register
     */
    virtual void write_r10(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r11 register
     * \param value New value of the rh850 r11 register
     */
    virtual void write_r11(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r12 register
     * \param value New value of the rh850 r12 register
     */
    virtual void write_r12(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r13 register
     * \param value New value of the rh850 r13 register
     */
    virtual void write_r13(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r14 register
     * \param value New value of the rh850 r14 register
     */
    virtual void write_r14(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r15 register
     * \param value New value of the rh850 r15 register
     */
    virtual void write_r15(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r16 register
     * \param value New value of the rh850 r16 register
     */
    virtual void write_r16(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r17 register
     * \param value New value of the rh850 r17 register
     */
    virtual void write_r17(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r18 register
     * \param value New value of the rh850 r18 register
     */
    virtual void write_r18(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r19 register
     * \param value New value of the rh850 r19 register
     */
    virtual void write_r19(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r20 register
     * \param value New value of the rh850 r20 register
     */
    virtual void write_r20(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r21 register
     * \param value New value of the rh850 r21 register
     */
    virtual void write_r21(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r22 register
     * \param value New value of the rh850 r22 register
     */
    virtual void write_r22(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r23 register
     * \param value New value of the rh850 r23 register
     */
    virtual void write_r23(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r24 register
     * \param value New value of the rh850 r24 register
     */
    virtual void write_r24(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r25 register
     * \param value New value of the rh850 r25 register
     */
    virtual void write_r25(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r26 register
     * \param value New value of the rh850 r26 register
     */
    virtual void write_r26(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r27 register
     * \param value New value of the rh850 r27 register
     */
    virtual void write_r27(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r28 register
     * \param value New value of the rh850 r28 register
     */
    virtual void write_r28(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r29 register
     * \param value New value of the rh850 r29 register
     */
    virtual void write_r29(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r30 register
     * \param value New value of the rh850 r30 register
     */
    virtual void write_r30(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 r31 register
     * \param value New value of the rh850 r31 register
     */
    virtual void write_r31(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 PC register
     * \param value New value of the rh850 PC register
     */
    virtual void write_PC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 EIPC register
     * \param value New value of the rh850 EIPC register
     */
    virtual void write_EIPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 EIPSW register
     * \param value New value of the rh850 EIPSW register
     */
    virtual void write_EIPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FEPC register
     * \param value New value of the rh850 FEPC register
     */
    virtual void write_FEPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FEPSW register
     * \param value New value of the rh850 FEPSW register
     */
    virtual void write_FEPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 PSW register
     * \param value New value of the rh850 PSW register
     */
    virtual void write_PSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FPSR register
     * \param value New value of the rh850 FPSR register
     */
    virtual void write_FPSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FPEPC register
     * \param value New value of the rh850 FPEPC register
     */
    virtual void write_FPEPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FPST register
     * \param value New value of the rh850 FPST register
     */
    virtual void write_FPST(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FPCC register
     * \param value New value of the rh850 FPCC register
     */
    virtual void write_FPCC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FPCFG register
     * \param value New value of the rh850 FPCFG register
     */
    virtual void write_FPCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FPEC register
     * \param value New value of the rh850 FPEC register
     */
    virtual void write_FPEC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 EIIC register
     * \param value New value of the rh850 EIIC register
     */
    virtual void write_EIIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FEIC register
     * \param value New value of the rh850 FEIC register
     */
    virtual void write_FEIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 CTPC register
     * \param value New value of the rh850 CTPC register
     */
    virtual void write_CTPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 CTPSW register
     * \param value New value of the rh850 CTPSW register
     */
    virtual void write_CTPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 CTBP register
     * \param value New value of the rh850 CTBP register
     */
    virtual void write_CTBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 EIWR register
     * \param value New value of the rh850 EIWR register
     */
    virtual void write_EIWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FEWR register
     * \param value New value of the rh850 FEWR register
     */
    virtual void write_FEWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 BSEL register
     * \param value New value of the rh850 BSEL register
     */
    virtual void write_BSEL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MCFG0 register
     * \param value New value of the rh850 MCFG0 register
     */
    virtual void write_MCFG0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 RBASE register
     * \param value New value of the rh850 RBASE register
     */
    virtual void write_RBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 EBASE register
     * \param value New value of the rh850 EBASE register
     */
    virtual void write_EBASE(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 INTBP register
     * \param value New value of the rh850 INTBP register
     */
    virtual void write_INTBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MCTL register
     * \param value New value of the rh850 MCTL register
     */
    virtual void write_MCTL(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 PID register
     * \param value New value of the rh850 PID register
     */
    virtual void write_PID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 FPIPR register
     * \param value New value of the rh850 FPIPR register
     */
    virtual void write_FPIPR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 SCCFG register
     * \param value New value of the rh850 SCCFG register
     */
    virtual void write_SCCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 SCBP register
     * \param value New value of the rh850 SCBP register
     */
    virtual void write_SCBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 HTCFG0 register
     * \param value New value of the rh850 HTCFG0 register
     */
    virtual void write_HTCFG0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 HVCCFG register
     * \param value New value of the rh850 HVCCFG register
     */
    virtual void write_HVCCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 HVCBP register
     * \param value New value of the rh850 HVCBP register
     */
    virtual void write_HVCBP(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MEA register
     * \param value New value of the rh850 MEA register
     */
    virtual void write_MEA(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 ASID register
     * \param value New value of the rh850 ASID register
     */
    virtual void write_ASID(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MEI register
     * \param value New value of the rh850 MEI register
     */
    virtual void write_MEI(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 ISPR register
     * \param value New value of the rh850 ISPR register
     */
    virtual void write_ISPR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 PMR register
     * \param value New value of the rh850 PMR register
     */
    virtual void write_PMR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 ICSR register
     * \param value New value of the rh850 ICSR register
     */
    virtual void write_ICSR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 INTCFG register
     * \param value New value of the rh850 INTCFG register
     */
    virtual void write_INTCFG(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 DBIC register
     * \param value New value of the rh850 DBIC register
     */
    virtual void write_DBIC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 DBCMC register
     * \param value New value of the rh850 DBCMC register
     */
    virtual void write_DBCMC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 DBPC register
     * \param value New value of the rh850 DBPC register
     */
    virtual void write_DBPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 DBPSW register
     * \param value New value of the rh850 DBPSW register
     */
    virtual void write_DBPSW(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 DIR0 register
     * \param value New value of the rh850 DIR0 register
     */
    virtual void write_DIR0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 DIR1 register
     * \param value New value of the rh850 DIR1 register
     */
    virtual void write_DIR1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 BPC register
     * \param value New value of the rh850 BPC register
     */
    virtual void write_BPC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 BPAV register
     * \param value New value of the rh850 BPAV register
     */
    virtual void write_BPAV(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 BPAM register
     * \param value New value of the rh850 BPAM register
     */
    virtual void write_BPAM(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 BPDV register
     * \param value New value of the rh850 BPDV register
     */
    virtual void write_BPDV(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 BPDM register
     * \param value New value of the rh850 BPDM register
     */
    virtual void write_BPDM(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 DBWR register
     * \param value New value of the rh850 DBWR register
     */
    virtual void write_DBWR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPM register
     * \param value New value of the rh850 MPM register
     */
    virtual void write_MPM(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPRC register
     * \param value New value of the rh850 MPRC register
     */
    virtual void write_MPRC(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPBRGN register
     * \param value New value of the rh850 MPBRGN register
     */
    virtual void write_MPBRGN(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPTRGN register
     * \param value New value of the rh850 MPTRGN register
     */
    virtual void write_MPTRGN(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MCR register
     * \param value New value of the rh850 MCR register
     */
    virtual void write_MCR(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA0 register
     * \param value New value of the rh850 MPLA0 register
     */
    virtual void write_MPLA0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA0 register
     * \param value New value of the rh850 MPUA0 register
     */
    virtual void write_MPUA0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT0 register
     * \param value New value of the rh850 MPAT0 register
     */
    virtual void write_MPAT0(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA1 register
     * \param value New value of the rh850 MPLA1 register
     */
    virtual void write_MPLA1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA1 register
     * \param value New value of the rh850 MPUA1 register
     */
    virtual void write_MPUA1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT1 register
     * \param value New value of the rh850 MPAT1 register
     */
    virtual void write_MPAT1(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA2 register
     * \param value New value of the rh850 MPLA2 register
     */
    virtual void write_MPLA2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA2 register
     * \param value New value of the rh850 MPUA2 register
     */
    virtual void write_MPUA2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT2 register
     * \param value New value of the rh850 MPAT2 register
     */
    virtual void write_MPAT2(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA3 register
     * \param value New value of the rh850 MPLA3 register
     */
    virtual void write_MPLA3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA3 register
     * \param value New value of the rh850 MPUA3 register
     */
    virtual void write_MPUA3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT3 register
     * \param value New value of the rh850 MPAT3 register
     */
    virtual void write_MPAT3(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA4 register
     * \param value New value of the rh850 MPLA4 register
     */
    virtual void write_MPLA4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA4 register
     * \param value New value of the rh850 MPUA4 register
     */
    virtual void write_MPUA4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT4 register
     * \param value New value of the rh850 MPAT4 register
     */
    virtual void write_MPAT4(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA5 register
     * \param value New value of the rh850 MPLA5 register
     */
    virtual void write_MPLA5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA5 register
     * \param value New value of the rh850 MPUA5 register
     */
    virtual void write_MPUA5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT5 register
     * \param value New value of the rh850 MPAT5 register
     */
    virtual void write_MPAT5(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA6 register
     * \param value New value of the rh850 MPLA6 register
     */
    virtual void write_MPLA6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA6 register
     * \param value New value of the rh850 MPUA6 register
     */
    virtual void write_MPUA6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT6 register
     * \param value New value of the rh850 MPAT6 register
     */
    virtual void write_MPAT6(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA7 register
     * \param value New value of the rh850 MPLA7 register
     */
    virtual void write_MPLA7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA7 register
     * \param value New value of the rh850 MPUA7 register
     */
    virtual void write_MPUA7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT7 register
     * \param value New value of the rh850 MPAT7 register
     */
    virtual void write_MPAT7(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA8 register
     * \param value New value of the rh850 MPLA8 register
     */
    virtual void write_MPLA8(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA8 register
     * \param value New value of the rh850 MPUA8 register
     */
    virtual void write_MPUA8(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT8 register
     * \param value New value of the rh850 MPAT8 register
     */
    virtual void write_MPAT8(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA9 register
     * \param value New value of the rh850 MPLA9 register
     */
    virtual void write_MPLA9(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA9 register
     * \param value New value of the rh850 MPUA9 register
     */
    virtual void write_MPUA9(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT9 register
     * \param value New value of the rh850 MPAT9 register
     */
    virtual void write_MPAT9(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA10 register
     * \param value New value of the rh850 MPLA10 register
     */
    virtual void write_MPLA10(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA10 register
     * \param value New value of the rh850 MPUA10 register
     */
    virtual void write_MPUA10(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT10 register
     * \param value New value of the rh850 MPAT10 register
     */
    virtual void write_MPAT10(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA11 register
     * \param value New value of the rh850 MPLA11 register
     */
    virtual void write_MPLA11(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA11 register
     * \param value New value of the rh850 MPUA11 register
     */
    virtual void write_MPUA11(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT11 register
     * \param value New value of the rh850 MPAT11 register
     */
    virtual void write_MPAT11(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA12 register
     * \param value New value of the rh850 MPLA12 register
     */
    virtual void write_MPLA12(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA12 register
     * \param value New value of the rh850 MPUA12 register
     */
    virtual void write_MPUA12(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT12 register
     * \param value New value of the rh850 MPAT12 register
     */
    virtual void write_MPAT12(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA13 register
     * \param value New value of the rh850 MPLA13 register
     */
    virtual void write_MPLA13(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA13 register
     * \param value New value of the rh850 MPUA13 register
     */
    virtual void write_MPUA13(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT13 register
     * \param value New value of the rh850 MPAT13 register
     */
    virtual void write_MPAT13(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA14 register
     * \param value New value of the rh850 MPLA14 register
     */
    virtual void write_MPLA14(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA14 register
     * \param value New value of the rh850 MPUA14 register
     */
    virtual void write_MPUA14(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT14 register
     * \param value New value of the rh850 MPAT14 register
     */
    virtual void write_MPAT14(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPLA15 register
     * \param value New value of the rh850 MPLA15 register
     */
    virtual void write_MPLA15(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPUA15 register
     * \param value New value of the rh850 MPUA15 register
     */
    virtual void write_MPUA15(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 MPAT15 register
     * \param value New value of the rh850 MPAT15 register
     */
    virtual void write_MPAT15(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 mpu_entry register
     * \param value New value of the rh850 mpu_entry register
     */
    virtual void write_mpu_entry(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850 mpat_g_support register
     * \param value New value of the rh850 mpat_g_support register
     */
    virtual void write_mpat_g_support(unsigned char value) = 0;
    /**
     * \brief Sets the value of the rh850 swd0pcad_exec_count register
     * \param value New value of the rh850 swd0pcad_exec_count register
     */
    virtual void write_swd0pcad_exec_count(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 swd0pcad_val register
     * \param value New value of the rh850 swd0pcad_val register
     */
    virtual void write_swd0pcad_val(unsigned int value) = 0;
    /**
     * \brief Sets the value of the rh850 mem_acc_type register
     * \param value New value of the rh850 mem_acc_type register
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
    virtual rh850::opcode_t get_decode_opcode() = 0;
    /**
     * \brief Reads the opcode of the instruction currently in the
     * execute translation phase.
     * \return Opcode value
     */
    virtual rh850::opcode_t get_execute_opcode() = 0;
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

#endif // _RH850_DEBUGGER_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
