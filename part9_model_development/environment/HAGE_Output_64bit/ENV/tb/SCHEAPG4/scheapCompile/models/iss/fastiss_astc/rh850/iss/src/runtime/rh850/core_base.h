//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file core_base.h
 * \brief Defines the Core base class
 */

#ifndef _RH850_CORE_BASE_H_
#define _RH850_CORE_BASE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <list>
#include <set>
#include "systemc.h"
#include "tlm.h"
#include "rh850/debugger_if.h"
#include "rh850/translator_base.h"
#include "rh850/analysis_types.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

template class std::list< std::string >;
template class std::list< unsigned int >;

namespace fastiss
{
  class code_gen_if_t;
  class parallel_translator_t;
}

namespace rh850
{
  class block_function_l1_cache_t;
  class block_function_l2_cache_t;
  class core_base_t;
  class bus_interface_t;
  class memory_if_t;
  class cpu_state_t;
  class operand_if_t;
  class cpu_state_if_t;
  class continue_translator_base_t;
  class analysis_callback_manager;
  class analysis_point_if;
  class translator_base_t;
  /**
   * \brief Type definition for the step advance function.
   */
  typedef void (core_base_t::*step_advance_fn)(void);
}

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief Type definition used to identify the kind of execution
   * engine that will be employed.
   */
  enum execution_engine_type_t
  {
    /**
     * \brief Denotes that the interpretive execution engine has been selected.
     * (translation_engine = 0)
     */
    INTERPRETIVE,
    /**
     * \brief Denotes that the lightweight binary translating execution engine has 
     * been selected. (translation_engine = 1)
     */
    LIGHT_TRANSLATING, 
    /**
     * \brief Denotes that the heavy binary translating execution engine has been
     * selected. (translation_engine = 2)
     */
    HEAVY_TRANSLATING,
    /**
     * \brief Denotes that both light and heavy execution engines have been selected.
     * (translation_engine = 3)
     */
    FULL_TRANSLATING
  };
}

//------------------------------------------------------------------------------
// Core base
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief The core base class is the external interface of the ISS for
   * integration into a platform. The core base provides a SystemC interface for
   * integration and customisation, and an interface to allow debugger operations.
   * This base class is responsible for the cache management.
   */
  class core_base_t : public sc_core::sc_module, public rh850::debugger_if_t
  {
  public: 
    /**
     * \defgroup CoreOperations Core operations
     * @{
     * \defgroup SystemCInterface SystemC interface
     * \defgroup DebuggerInterfaceOperations Debugger interface operations
     * @{
     * \defgroup DebuggerBreakpointOperations Debugger breakpoint operations
     * \defgroup DebuggerWatchpointOperations Debugger watchpoint operations
     * \defgroup DebuggerMemoryAccessOperations Debugger memory access operations
     * \defgroup DebuggerRegisterAccessOperations Debugger register access operations
     * \defgroup AnalysisPointManagementAPI Analysis point management operations
	 * \defgroup AnalysisTranslationQueryAPI Analysis translation query operations
	 * \defgroup AnalysisTransactionQueryAPI Analysis transaction query operations
     * @}
     * @}
     */

    /**
     * \brief Core base class constructor
     * \param name Name of the ISS in the SystemC platform
     */
    core_base_t(sc_core::sc_module_name name);
    /**
     * \brief Core base class destructor
     */
    ~core_base_t();
    /**
    
     * \brief Core base initialisation, executed once all custom objects
     * are constructed.
     * \param blocking_tlm Flag indicating whether the memory sub-system should
     * issue blocking or non-blocking TLM transactions
     * \param tlm_response_error_as_error Flag indicating whether to report TLM
     * response errors as errors or warnings
     * \param translation_engine This encodes the translation engine to use.
     * A value of zero disables translation, i.e. blocks are only interpreted.
     * The value 1 selects the lightweight translator exclusively, while 2
     * means that only the "heavyweight" translator will be used. A value of 3
     * indicates that both light and heavy translators will be used.
     * \param adaptive_count This is the number of times a block is interpreted
     * before it is binary translated and cached. A value of zero means that all
     * blocks are always binary translated. A value of 0xffffffff means that all
     * blocks are always interpreted; the same effect can, and should, be 
     * achieved by setting translation_engine to zero.
     * \param translation_partition_mask Address mask used to enable the core to
     * determine the translation* partition base address when evaluating which
     * blocks should be flushed when a breakpoint is set/cleared or a write to
     * memory occurs.
     * \param translation_cache_max_list_entries The maximum number of entries
     * in a single linked list of block functions in the translation cache.
     * Once the size reaches this number the earliest translated block is
     * evicted and deleted. This must have a value greater than 1.
     * \param count_instructions Boolean flag that indicates if instructions
     * should be counted (if true) or not (if false)     
     */
    void initialise
    (
      bool blocking_tlm,
      bool tlm_response_error_as_error,
      unsigned int translation_engine,
      unsigned int adaptive_count,
      unsigned int translation_partition_mask,
      unsigned int translation_cache_max_list_entries,
      bool count_instructions
    );

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
    unsigned int add_watchpoint(unsigned int address, unsigned int bus_interface_id, unsigned int size, rh850::watchpoint_kind_t kind);

    /**
     * \brief Memory subsystem erase watchpoint operation - removes first equivalent
     * watchpoint.
     * \param wp - Descriptor corresponding to the watchpoint to be deleted
     */
    void erase_watchpoint(unsigned int wp);

    /**
     * \brief Memory subsystem clear all watchpoints operation. Note that the translation cache
     * should always flush after this operation.
     */
    void clear_all_watchpoints();

    /**
     * \brief Memory subsystem operation which returns a list of pending watchpoint
     * descriptors, note that calling this operation clears the list of pending watchpoints.
     * \return List of pending watchpoint descriptors
     */
    std::list< unsigned int > pending_watchpoints(void);
    
    /**
     * \brief Initialises the block function cache. This must be done after all
     * components are constructed.
     */
    void initialise_cache(void);

    /**
     * \brief Initialises the parallel translator. This must be done after all
     * helper functions have been registered.
     */
    void initialise_parallel_translator(void);
    
    /**
     * \brief Function called when a fetch exception occurs due to MMU/MPU
     * protections
     */
    virtual void fetch_exception(void) = 0;

    /**
     * \brief Translates a block function when the binary translation engine is
     * being used for an advance by continue. This function is called from a helper
     * in a translated block that block function cache is initialised with. This
     * means the block lookup and execute loop can be agnostic of the execution
     * engine being used.
     */
    void binary_translate_continue_block(void);
    /**
     * \brief Translates a block function when the adaptive translation engine is
     * being used for an advance by continue. The block function cache is
     * initialised with this function. This means the block lookup and execute
     * loop can be agnostic of the execution engine being used.
     */
    void adaptive_translate_continue_block(void);
    /**
     * \brief Translates a block function when the interpretive translation engine is
     * being used for an advance by continue. The block function cache is
     * initialised with this function. This means the block lookup and execute
     * loop can be agnostic of the execution engine being used.
     */
    void interpretive_translate_continue_block(void);

    /**
     * \brief Translates a block function when the binary translation engine is
     * being used for an advance by step. This means the advance by step can be
     * agnostic of the execution engine being used.
     */
    void binary_translate_step_block(void);
    /**
     * \brief Translates a block function when the adaptive translation engine is
     * being used for an advance by step. This means the advance by step can be
     * agnostic of the execution engine being used.
     */
    void adaptive_translate_step_block(void);
    /**
     * \brief Translates a block function when the interpretive translation engine is
     * being used for an advance by step. This means the advance by step can be
     * agnostic of the execution engine being used.
     */
    void interpretive_translate_step_block(void);
    
    /**
     * \addtogroup DebuggerInterfaceOperations
     * @{
     */

    /**
     * \brief SystemC event used by the ISS to indicate the completion
     * of the current operation (either a step or a continue) to the debugger
     */
    sc_core::sc_event operation_complete_event;

    /**
     * \brief Helper function called when a breakpoint is hit whilst the
     * core is executing. The function terminates the current quanta.
     */
    void hit_breakpoint(void);
    /**
     * @}
     */

    /**
     * \brief Returns the total number of instructions that have been executed
     * (for all quanta's)
     * \return The total number of instructions executed
     */
    unsigned long long get_instruction_count(void);

    /**
     * \brief Request by the external platform to asynchronously stop the
     * ISS execution (during a continue operation)
     */
    void request_stop(void);

    /**
     * \brief Request by the external platform to clear an asynchronous stop
     * request of the ISS
     */
    void clear_stop(void);

    /**
     * \brief Returns the local time - the current time offset from
     * global simulation time. This function should only be used
     * while the ISS is not executing instructions.
     * \return The local time
     */          
    sc_core::sc_time get_local_time(void);

    /**
     * \brief Returns the elapse time - the time elapsed by instruction
     * execution since the start of the quanta. This function should only
     * be used while the ISS is not executing instructions.
     * \return The elapsed time
     */          
    sc_core::sc_time get_elapsed_time(void);

    /**
     * \brief Flushes the translation cache, clearing all translated blocks
     * and re-initialising
     */          
    void flush_cache(void);

    /**
     * \brief Sets the flush required flag, which queues the flushing of the
     * translation cache for the start of the next quantum. At the start of the
     * next quantum all translated blocks are cleared and re-initialised.
     */          
    void set_flush_required(void);

  protected:    
    /**
     * \brief Flushes the translation cache for specific translation partitions,
     * clearing all translated blocks in those partitions and re-initialising
     */          
    void flush_partitions(void);
    /**
     * \brief Flushes the L1 block function cache
     */          
    void flush_l1_cache(void);

  public:
    /**
     * \brief Inserts an address into the set of breakpoints. Inserting
     * a breakpoint results in the flushing of the translated block cache.
     * \param bkpt_addr The program counter address to add as a breakpoint
     */          
    void insert_bkpt(unsigned int bkpt_addr);
    /**
     * \brief Clears an address from the set of breakpoints. Clearing
     * a breakpoint results in the flushing of the translated block cache.
     * \param bkpt_addr The program counter address to remove as a breakpoint
     */          
    void clear_bkpt(unsigned int bkpt_addr);

    /**
     * \brief Loads an arbitrary size value from memory
     * \param addr The address for the read
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually read
     */          
    unsigned int iN_load(unsigned int addr, unsigned int size, unsigned char *data);

    /**
     * \brief Loads an arbitrary size value from memory at a physical address
     * \param addr The physical address for the read
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually read
     */          
    unsigned int iN_load_physical(unsigned int addr, unsigned int size, unsigned char *data);

    /**
     * \brief Stores an arbitrary size value to memory
     * \param addr The address for the write
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually written
     */          
    unsigned int iN_store(unsigned int addr, unsigned int size, unsigned char *data);

    /**
     * \brief Stores an arbitrary size value to memory at a physical address
     * \param addr The physical address for the write
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually written
     */          
    unsigned int iN_store_physical(unsigned int addr, unsigned int size, unsigned char *data);

    

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
     * \brief Queries the value of the rh850 FPSR register
     * \return Value of the rh850 FPSR register
     */
    unsigned int read_FPSR(void);
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
     * \brief Queries the value of the rh850 FPCC register
     * \return Value of the rh850 FPCC register
     */
    unsigned int read_FPCC(void);
    /**
     * \brief Queries the value of the rh850 FPCFG register
     * \return Value of the rh850 FPCFG register
     */
    unsigned int read_FPCFG(void);
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
     * \brief Queries the value of the rh850 RBASE register
     * \return Value of the rh850 RBASE register
     */
    unsigned int read_RBASE(void);
    /**
     * \brief Queries the value of the rh850 EBASE register
     * \return Value of the rh850 EBASE register
     */
    unsigned int read_EBASE(void);
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
     * \brief Queries the value of the rh850 SCCFG register
     * \return Value of the rh850 SCCFG register
     */
    unsigned int read_SCCFG(void);
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
     * \brief Queries the value of the rh850 MEI register
     * \return Value of the rh850 MEI register
     */
    unsigned int read_MEI(void);
    /**
     * \brief Queries the value of the rh850 ISPR register
     * \return Value of the rh850 ISPR register
     */
    unsigned int read_ISPR(void);
    /**
     * \brief Queries the value of the rh850 PMR register
     * \return Value of the rh850 PMR register
     */
    unsigned int read_PMR(void);
    /**
     * \brief Queries the value of the rh850 ICSR register
     * \return Value of the rh850 ICSR register
     */
    unsigned int read_ICSR(void);
    /**
     * \brief Queries the value of the rh850 INTCFG register
     * \return Value of the rh850 INTCFG register
     */
    unsigned int read_INTCFG(void);
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
     * \brief Queries the value of the rh850 MPRC register
     * \return Value of the rh850 MPRC register
     */
    unsigned int read_MPRC(void);
    /**
     * \brief Queries the value of the rh850 MPBRGN register
     * \return Value of the rh850 MPBRGN register
     */
    unsigned int read_MPBRGN(void);
    /**
     * \brief Queries the value of the rh850 MPTRGN register
     * \return Value of the rh850 MPTRGN register
     */
    unsigned int read_MPTRGN(void);
    /**
     * \brief Queries the value of the rh850 MCR register
     * \return Value of the rh850 MCR register
     */
    unsigned int read_MCR(void);
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
     * \brief Sets the value of the rh850 FPSR register
     * \param value New value of the rh850 FPSR register
     */
    void write_FPSR(unsigned int value);
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
     * \brief Sets the value of the rh850 FPCC register
     * \param value New value of the rh850 FPCC register
     */
    void write_FPCC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 FPCFG register
     * \param value New value of the rh850 FPCFG register
     */
    void write_FPCFG(unsigned int value);
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
     * \brief Sets the value of the rh850 RBASE register
     * \param value New value of the rh850 RBASE register
     */
    void write_RBASE(unsigned int value);
    /**
     * \brief Sets the value of the rh850 EBASE register
     * \param value New value of the rh850 EBASE register
     */
    void write_EBASE(unsigned int value);
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
     * \brief Sets the value of the rh850 SCCFG register
     * \param value New value of the rh850 SCCFG register
     */
    void write_SCCFG(unsigned int value);
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
     * \brief Sets the value of the rh850 MEI register
     * \param value New value of the rh850 MEI register
     */
    void write_MEI(unsigned int value);
    /**
     * \brief Sets the value of the rh850 ISPR register
     * \param value New value of the rh850 ISPR register
     */
    void write_ISPR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 PMR register
     * \param value New value of the rh850 PMR register
     */
    void write_PMR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 ICSR register
     * \param value New value of the rh850 ICSR register
     */
    void write_ICSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850 INTCFG register
     * \param value New value of the rh850 INTCFG register
     */
    void write_INTCFG(unsigned int value);
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
     * \brief Sets the value of the rh850 MPRC register
     * \param value New value of the rh850 MPRC register
     */
    void write_MPRC(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPBRGN register
     * \param value New value of the rh850 MPBRGN register
     */
    void write_MPBRGN(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MPTRGN register
     * \param value New value of the rh850 MPTRGN register
     */
    void write_MPTRGN(unsigned int value);
    /**
     * \brief Sets the value of the rh850 MCR register
     * \param value New value of the rh850 MCR register
     */
    void write_MCR(unsigned int value);
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
    
    /**
     * \brief Reads the opcode of the instruction currently in the
     * decode translation phase.
     * \return Opcode value
     */
    rh850::opcode_t get_decode_opcode();
    /**
     * \brief Reads the opcode of the instruction currently in the
     * execute translation phase.
     * \return Opcode value
     */
    rh850::opcode_t get_execute_opcode();
    
    /**
     * \brief Reads the value of PC for the instruction currently in the
     * decode translation phase.
     * \return PC value
     */
    unsigned int get_decode_PC();
    /**
     * \brief Reads the value of PC for the instruction currently in the
     * execute translation phase.
     * \return PC value
     */
    unsigned int get_execute_PC();
    
    /**
     * \brief Gets a disassembly string for the instruction currently in the
     * decode translation phase.
     * \return Disassembly string
     */
    std::string get_decode_disass();
    /**
     * \brief Gets a disassembly string for the instruction currently in the
     * execute translation phase.
     * \return Disassembly string
     */
    std::string get_execute_disass();
    
    /**
     * \brief Reads the value of the 'terminates_block' attribute of the instruction currently in the
     * decode translation phase.
     * \return Boolean representing value of instruction 'terminates_block' attribute
     */
    bool get_decode_terminates_block();
    /**
     * \brief Reads the value of the 'cycle_count' attribute of the instruction currently in the
     * decode translation phase.
     * \return Unsigned 32-bit integer representing value of instruction 'cycle_count' attribute
     */
    unsigned int get_decode_cycle_count();
    /**
     * \brief Reads the value of the 'terminates_block' attribute of the instruction currently in the
     * execute translation phase.
     * \return Boolean representing value of instruction 'terminates_block' attribute
     */
    bool get_execute_terminates_block();
    /**
     * \brief Reads the value of the 'cycle_count' attribute of the instruction currently in the
     * execute translation phase.
     * \return Unsigned 32-bit integer representing value of instruction 'cycle_count' attribute
     */
    unsigned int get_execute_cycle_count();

    /**
     * \brief Reads the length in bytes of the instruction currently in the
     * decode translation phase.
     * \return Integer value representing length of the instruction
     */
    unsigned int get_decode_length();
    /**
     * \brief Reads the length in bytes of the instruction currently in the
     * execute translation phase.
     * \return Integer value representing length of the instruction
     */
    unsigned int get_execute_length();
    
    /**
     * \brief Indicates whether the instruction currently in the
     * decode translation phase is valid.
     * \return Boolean indicating whether the instruction is valid
     */
    bool is_decode_valid();
    /**
     * \brief Indicates whether the instruction currently in the
     * execute translation phase is valid.
     * \return Boolean indicating whether the instruction is valid
     */
    bool is_execute_valid();

  private:
    /**
     * \brief A pointer to the internal CPU state structure. This direct
     * pointer is used for optimised performance.
     */          
    rh850::cpu_state_t *internal_cpu_state;
    /**
     * \brief A pointer to the internal instruction count storage. This direct
     * pointer is used for optimised performance.
     */          
    unsigned int *instruction_count_ptr;
    /**
     * \brief A pointer to the internal core local time. This direct
     * pointer is used for optimised performance.
     */
    unsigned long long *local_time_ptr;
    /**
     * \brief A pointer to the accrued core local time. This direct
     * pointer is used for optimised performance.
     */
    unsigned long long *accrued_time_ptr;
    /**
     * \brief A pointer to the internal PC storage. This direct
     * pointer is used for optimised performance.
     */
    unsigned int *PC_ptr;

    /**
     * \brief Pointer to the current translator.
     */
    rh850::translator_base_t * m_curr_trans;
    
  protected:
    /**
     * \brief Returns the number of instructions executed in the
     * current quanta. This in-lined function is provided for optimised
     * performance of block lookup and execution in the derived core.
     * \return The number of instructions executed in the current quanta
     */
    inline unsigned int get_quanta_instruction_count(void)
    {
      return *instruction_count_ptr;
    }

  protected:
    /**
     * \brief Continue event handler function, executed when the continue event
     * occurs in the simulation, calls the core level continue handler
     */
    void base_continue_handler(void);
    /**
     * \brief Step event handler function, executed when the step event
     * occurs in the simulation, calls the core level step handler
     */
    void base_step_handler(void);
    
    /**
     * \brief Continue event handler function, executed when the continue event
     * occurs in the simulation
     */
    virtual void continue_handler(void) = 0;
    /**
     * \brief Step event handler function, executed when the step event
     * occurs in the simulation
     */
    virtual void step_handler(void) = 0;
    
  public:    
    /**
     * \brief Causes the ISS to continue execution until the specified time
     * limit is reached
     * \param time_limit Local time limit specifying period for which to continue
     * execution
     */
    void advance_by_continue(unsigned long long time_limit);

    /**
     * \brief Causes the ISS to execute a single instruction and advance local
     * time accordingly
     */
    void advance_by_step(void);
    
    /**
     * \brief Show DMI region information by bus interface.
     */
    void show_regions(void);
    
  protected:
    /**
     * \brief The maximum amount of time that can be executed in any
     * quanta.
     */
    unsigned long long time_limit;
    /**
     * \brief A pointer to the code generator implementor
     */          
    fastiss::code_gen_if_t *fastiss_code_gen;
    /**
     * \brief A pointer to the parallel translator
     */          
    fastiss::parallel_translator_t *parallel_translator;
    /**
    * \brief A pointer to the bus interface
    */
    rh850::bus_interface_t *fastiss_bus_interface;
    /**
     * \brief A pointer to the memory access implementor
     */          
    rh850::memory_if_t *fastiss_memory;

    /**
     * \brief A pointer to the operand extractor implementor
     */          
    rh850::operand_if_t *operand;
    /**
     * \brief A pointer to the CPU state implementor
     */          
    rh850::cpu_state_if_t *cpu_state;
    /**
     * \brief A pointer to the step translator
     */          
    rh850::translator_base_t *step_translator;
    /**
     * \brief A pointer to the continue translator
     */          
    rh850::continue_translator_base_t *continue_translator;

    /**
     * \brief The maximum amount of time that can be executed in the
     * current quanta
     */
    unsigned long long local_time_limit;
    /**
     * \brief The total number of instructions that have been executed (for all
     * quanta's)
     */
    unsigned long long instr_count;
    /**
     * \brief Boolean flag indicating whether the external platform has
     * requested the ISS to stop execution.
     */
    bool stop_requested;
    /**
     * \brief Boolean flag indicating that a pending translation cache flush is
     * required at the start of the next quantum.
     */
    bool flush_required;
    /**
     * \brief The set of translation partitions (identified by partition start
     * address) that require flushing.
     */    
    std::set<unsigned int> partitions_to_flush;
    /**
     * \brief Address mask used to enable the core to determine the translation
     * partition base address when evaluating which blocks should be flushed when
     * a breakpoint is set/cleared or a write to memory occurs.
     */
    unsigned int m_translation_partition_mask;
    /**
     * \brief A pointer to the level 1 block function cache, which contains a
     * mapping from virtual addresses to translated blocks. A lookup in the L1
     * block function cache is guaranteed to succeed. If a block does not exist
     * in the cache then the L1 cache does a lookup in the L2 block function
     * cache which will create and return a new block if one does not exist. The
     * translation from virtual addresses to physical addresses is dependent on
     * the current MMU/MPU configuration, so this cache is completely flushed
     * when the MMU/MPU configuration changes. The L1 cache does not create nor own
     * translated blocks, when it is flushed it does not delete any translated
     * blocks.
     */          
    rh850::block_function_l1_cache_t *m_fn_l1_cache;
    /**
     * \brief A pointer to the level 2 block function cache, which contains a
     * mapping from physical addresses to translated blocks. Complete
     * translated blocks are inserted into this cache to ensure each code block
     * is translated once only. A lookup in the L2 block function cache is
     * guaranteed to succeed. If a block does not exist in the cache then a new
     * block will be created and returned. The contents of memory at physical
     * addresses does not change when the MMU/MPU configuration changes, so this
     * cache is not flushed when the MMU/MPU configuration changes.
     */          
    rh850::block_function_l2_cache_t *m_fn_l2_cache;
   
  private:
    /**
     * \brief Enumeration indicating which execution engine to use. This is
     * determined from the translation engine and adaptive count values, and 
     * stored as an enumerated value to prevent re-evaluation during execution.
     */
    execution_engine_type_t m_execution_engine_type;
    /**
     * \brief When a binary-translating engine is selected, this is the number 
     * of times a block is interpreted before it is binary translated and cached. 
     * A value of zero means that all blocks are always binary translated. 
     * A value of 0xffffffff means that all blocks are always interpreted; the 
     * same effect can, and should, be achieved by selecting execution engine 0.
     */
    unsigned int m_adaptive_count;
    /**
     * \brief A pointer to the function to use for stepping. The function to use
     * is dependent on the selected execution engine.
     */
    step_advance_fn m_step_advance_fn;

    /**
     * \brief A pointer to the analysis callback manager
     */
    rh850::analysis_callback_manager *m_analysis_callback_manager;
    
  protected:
    /**
     * \brief Pointer to the completed advance analysis point
     */
    rh850::analysis_point_if* m_completed_advance_analysis_point;
    /**
     * \brief Pointer to the pre-translate loop analysis point
     */
    rh850::analysis_point_if* m_pre_translate_loop_analysis_point;
    /**
     * \brief Pointer to the post-translate loop analysis point
     */
    rh850::analysis_point_if* m_post_translate_loop_analysis_point;
    /**
     * \brief Pointer to the post-fetch analysis point
     */
    rh850::analysis_point_if* m_post_fetch_analysis_point;
    /**
     * \brief Pointer to the post-decode analysis
     */
    rh850::analysis_point_if* m_post_decode_analysis_point;
    /**
     * \brief Pointer to the post-execute analysis
     */
    rh850::analysis_point_if* m_post_execute_analysis_point;
    /**
     * \brief Pointer to the interrupt acceptance analysis point
     */
    rh850::analysis_point_if* m_interrupt_accepted_analysis_point;
    /**
     * \brief Pointer to the interrupt completion analysis point
     */
    rh850::analysis_point_if* m_interrupt_completed_analysis_point;
    /**
     * \brief Pointer to the reset completion analysis point
     */
    rh850::analysis_point_if* m_reset_completed_analysis_point;
    /**
     * \brief Pointer to the immediate context switch analysis point
     */
    rh850::analysis_point_if* m_immediate_context_switch_analysis_point;
    /**
     * \brief Pointer to the generative context switch analysis point
     */
    rh850::analysis_point_if* m_generative_context_switch_analysis_point;
    
    /**
     * \brief Create an analysis point to which analysis callbacks and predicates may be attached.
     * Note that analysis points are owned and deleted by the analysis callback manager.
     * \param analysis_point_selector Name used to identify analysis point
     * \param accepts_predicates Boolean value indicating whether the analysis point accepts the attachment of analysis predicates
     * \return Handle to analysis point
     */
    analysis_point_if *create_analysis_point(std::string analysis_point_selector, bool accepts_predicates);
    
  public:    
    /**
     * \brief Returns list of all registered analysis point selector values
     * \return List of registered analysis point selector values
     */
    std::list<std::string> get_analysis_point_selectors(void);

    /**
     * \brief Queries whether analysis hook specified by the analysis hook selector accepts attachment of analysis predicate operations
     * \param analysis_point_selector Name used to identify analysis point
     * \return Boolean value indicating whether the analysis point accepts attachment of analysis predicate operations
     */
    bool accepts_predicates(std::string analysis_point_selector);

#if !defined(__VLAB_MIT__)    
    /**
     * \brief Registers an analysis callback with the analysis callback manager.
     * This analysis callback can then be referred to by an analysis predicate operation using the returned analysis callback descriptor.
     * Note that only those analysis callbacks referred to by an analysis predicate operation need be registered with the analysis callback manager.
     * Analysis callbacks that are only directly attached to an analysis point do not require prior registration.
     * Note that the analysis callback descriptors are owned by the analysis callback manager
     */
    analysis_callback_descriptor *register_analysis_callback(analysis_callback callback);
    
    /**
     * \brief Attaches an analysis callback to the specified analysis point.
     * This causes a call to the specified analysis callback to be generated when the specified analysis point is activated.
     * This operation may be used with all analysis points
     * \param analysis_point_selector Name used to identify analysis point
     * \param callback Handle to analysis callback operation to be invoked when the analysis point is activated
     * \param callback_data Data to be passed to analysis callback when the analysis point is activated
     * \return Analysis attachment descriptor used for subsequent calls to detach().
     */
    analysis_attachment_descriptor *attach(std::string analysis_point_selector, analysis_callback callback, void *callback_data); 
    
    /**
     * \brief Attaches an analysis predicate to the specified analysis point.
     * This causes a call to the specified analysis predicate to be generated when the specified analysis point is activated.
     * The response value from the analysis predicate is used to generate a call to the registered analysis callback or increment the analysis counter identified by the analysis predicate.
     * Note that this operation should only be used with analysis points that accept the attachment of analysis predicates
     * \param analysis_point_selector Name used to identify analysis point
     * \param predicate Handle to analysis predicate operation to be invoked when the analysis point is activated
     * \param predicate_data Data to be passed to analysis predicate when the analysis point is activated
     * \return Analysis attachment descriptor used for subsequent calls to detach().
     */
    analysis_attachment_descriptor *attach(std::string analysis_point_selector, analysis_predicate predicate, void *predicate_data); 
        
    /**
     * \brief Removes an analysis callback or analysis predicate operation from an analysis point
     * \param attachment Analysis attachment descriptor generated by a prior call to attach().
     * This identified both the analysis callback or analysis predicate and the analysis point that are to be disassociated
     */
    void detach(analysis_attachment_descriptor *attachment);
#endif
    
    /**
     * \brief Query the last transaction address.
     * Note that this only valid within an analysis callback associated with a memory transaction.
     * \return Last transaction address.
     */
    unsigned int read_last_access_addr();

    /**
     * \brief Query the last transaction size.
     * Note that this only valid within an analysis callback associated with a memory transaction.
     * \return Last transaction size.
     */
    unsigned char read_last_access_size();

    /**
     * \brief Query the last transaction data.
     * Note that this only valid within an analysis callback associated with a memory transaction.
     * \return Last transaction data.
     */
    unsigned long long read_last_access_data();
    
    /**
     * \brief Queries the version of PMS that was used to generate the model.
     * \return String detailing the version of PMS version that was used to
     * generate the model.
     */
    std::string get_pms_build_version();
  };
}

#endif // _RH850_CORE_BASE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
