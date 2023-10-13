//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
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

#ifndef _RH850G4_CORE_BASE_H_
#define _RH850G4_CORE_BASE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include <list>
#include <set>
#include "systemc.h"
#include "tlm.h"
#include "rh850g4/debugger_if.h"
#include "rh850g4/translator_base.h"
#include "rh850g4/analysis_types.h"

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

namespace rh850g4
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

namespace rh850g4
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

namespace rh850g4
{
  /**
   * \brief The core base class is the external interface of the ISS for
   * integration into a platform. The core base provides a SystemC interface for
   * integration and customisation, and an interface to allow debugger operations.
   * This base class is responsible for the cache management.
   */
  class core_base_t : public sc_core::sc_module, public rh850g4::debugger_if_t
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
    unsigned int add_watchpoint(unsigned int address, unsigned int bus_interface_id, unsigned int size, rh850g4::watchpoint_kind_t kind);

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
     * \brief Queries the value of the rh850g4 FPSR register
     * \return Value of the rh850g4 FPSR register
     */
    unsigned int read_FPSR(void);
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
     * \brief Queries the value of the rh850g4 FPCC register
     * \return Value of the rh850g4 FPCC register
     */
    unsigned int read_FPCC(void);
    /**
     * \brief Queries the value of the rh850g4 FPCFG register
     * \return Value of the rh850g4 FPCFG register
     */
    unsigned int read_FPCFG(void);
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
     * \brief Queries the value of the rh850g4 FEPSWH register
     * \return Value of the rh850g4 FEPSWH register
     */
    unsigned int read_FEPSWH(void);
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
     * \brief Queries the value of the rh850g4 EBASE register
     * \return Value of the rh850g4 EBASE register
     */
    unsigned int read_EBASE(void);
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
     * \brief Queries the value of the rh850g4 GMCFG register
     * \return Value of the rh850g4 GMCFG register
     */
    unsigned int read_GMCFG(void);
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
     * \brief Queries the value of the rh850g4 RBCR1 register
     * \return Value of the rh850g4 RBCR1 register
     */
    unsigned int read_RBCR1(void);
    /**
     * \brief Queries the value of the rh850g4 RBNR register
     * \return Value of the rh850g4 RBNR register
     */
    unsigned int read_RBNR(void);
    /**
     * \brief Queries the value of the rh850g4 RBIP register
     * \return Value of the rh850g4 RBIP register
     */
    unsigned int read_RBIP(void);
    /**
     * \brief Queries the value of the rh850g4 ISPR register
     * \return Value of the rh850g4 ISPR register
     */
    unsigned int read_ISPR(void);
    /**
     * \brief Queries the value of the rh850g4 IMSR register
     * \return Value of the rh850g4 IMSR register
     */
    unsigned int read_IMSR(void);
    /**
     * \brief Queries the value of the rh850g4 ICSR register
     * \return Value of the rh850g4 ICSR register
     */
    unsigned int read_ICSR(void);
    /**
     * \brief Queries the value of the rh850g4 INTCFG register
     * \return Value of the rh850g4 INTCFG register
     */
    unsigned int read_INTCFG(void);
    /**
     * \brief Queries the value of the rh850g4 PLMR register
     * \return Value of the rh850g4 PLMR register
     */
    unsigned int read_PLMR(void);
    /**
     * \brief Queries the value of the rh850g4 FXSR register
     * \return Value of the rh850g4 FXSR register
     */
    unsigned int read_FXSR(void);
    /**
     * \brief Queries the value of the rh850g4 FXST register
     * \return Value of the rh850g4 FXST register
     */
    unsigned int read_FXST(void);
    /**
     * \brief Queries the value of the rh850g4 FXINFO register
     * \return Value of the rh850g4 FXINFO register
     */
    unsigned int read_FXINFO(void);
    /**
     * \brief Queries the value of the rh850g4 FXCFG register
     * \return Value of the rh850g4 FXCFG register
     */
    unsigned int read_FXCFG(void);
    /**
     * \brief Queries the value of the rh850g4 FXXC register
     * \return Value of the rh850g4 FXXC register
     */
    unsigned int read_FXXC(void);
    /**
     * \brief Queries the value of the rh850g4 FXXP register
     * \return Value of the rh850g4 FXXP register
     */
    unsigned int read_FXXP(void);
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
     * \brief Queries the value of the rh850g4 GMPLMR register
     * \return Value of the rh850g4 GMPLMR register
     */
    unsigned int read_GMPLMR(void);
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
     * \brief Queries the value of the rh850g4 MCI register
     * \return Value of the rh850g4 MCI register
     */
    unsigned int read_MCI(void);
    /**
     * \brief Queries the value of the rh850g4 MPIDX register
     * \return Value of the rh850g4 MPIDX register
     */
    unsigned int read_MPIDX(void);
    /**
     * \brief Queries the value of the rh850g4 MPBK register
     * \return Value of the rh850g4 MPBK register
     */
    unsigned int read_MPBK(void);
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
     * \brief Queries the value of the rh850g4 MPID0 register
     * \return Value of the rh850g4 MPID0 register
     */
    unsigned int read_MPID0(void);
    /**
     * \brief Queries the value of the rh850g4 MPID1 register
     * \return Value of the rh850g4 MPID1 register
     */
    unsigned int read_MPID1(void);
    /**
     * \brief Queries the value of the rh850g4 MPID2 register
     * \return Value of the rh850g4 MPID2 register
     */
    unsigned int read_MPID2(void);
    /**
     * \brief Queries the value of the rh850g4 MPID3 register
     * \return Value of the rh850g4 MPID3 register
     */
    unsigned int read_MPID3(void);
    /**
     * \brief Queries the value of the rh850g4 MPID4 register
     * \return Value of the rh850g4 MPID4 register
     */
    unsigned int read_MPID4(void);
    /**
     * \brief Queries the value of the rh850g4 MPID5 register
     * \return Value of the rh850g4 MPID5 register
     */
    unsigned int read_MPID5(void);
    /**
     * \brief Queries the value of the rh850g4 MPID6 register
     * \return Value of the rh850g4 MPID6 register
     */
    unsigned int read_MPID6(void);
    /**
     * \brief Queries the value of the rh850g4 MPID7 register
     * \return Value of the rh850g4 MPID7 register
     */
    unsigned int read_MPID7(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGL register
     * \return Value of the rh850g4 ICTAGL register
     */
    unsigned int read_ICTAGL(void);
    /**
     * \brief Queries the value of the rh850g4 ICTAGH register
     * \return Value of the rh850g4 ICTAGH register
     */
    unsigned int read_ICTAGH(void);
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
     * \brief Queries the value of the rh850g4 ICCFG register
     * \return Value of the rh850g4 ICCFG register
     */
    unsigned int read_ICCFG(void);
    /**
     * \brief Queries the value of the rh850g4 ICERR register
     * \return Value of the rh850g4 ICERR register
     */
    unsigned int read_ICERR(void);
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
     * \brief Sets the value of the rh850g4 FPSR register
     * \param value New value of the rh850g4 FPSR register
     */
    void write_FPSR(unsigned int value);
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
     * \brief Sets the value of the rh850g4 FPCC register
     * \param value New value of the rh850g4 FPCC register
     */
    void write_FPCC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FPCFG register
     * \param value New value of the rh850g4 FPCFG register
     */
    void write_FPCFG(unsigned int value);
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
     * \brief Sets the value of the rh850g4 FEPSWH register
     * \param value New value of the rh850g4 FEPSWH register
     */
    void write_FEPSWH(unsigned int value);
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
     * \brief Sets the value of the rh850g4 EBASE register
     * \param value New value of the rh850g4 EBASE register
     */
    void write_EBASE(unsigned int value);
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
     * \brief Sets the value of the rh850g4 GMCFG register
     * \param value New value of the rh850g4 GMCFG register
     */
    void write_GMCFG(unsigned int value);
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
     * \brief Sets the value of the rh850g4 RBCR1 register
     * \param value New value of the rh850g4 RBCR1 register
     */
    void write_RBCR1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBNR register
     * \param value New value of the rh850g4 RBNR register
     */
    void write_RBNR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 RBIP register
     * \param value New value of the rh850g4 RBIP register
     */
    void write_RBIP(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ISPR register
     * \param value New value of the rh850g4 ISPR register
     */
    void write_ISPR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 IMSR register
     * \param value New value of the rh850g4 IMSR register
     */
    void write_IMSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICSR register
     * \param value New value of the rh850g4 ICSR register
     */
    void write_ICSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 INTCFG register
     * \param value New value of the rh850g4 INTCFG register
     */
    void write_INTCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 PLMR register
     * \param value New value of the rh850g4 PLMR register
     */
    void write_PLMR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXSR register
     * \param value New value of the rh850g4 FXSR register
     */
    void write_FXSR(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXST register
     * \param value New value of the rh850g4 FXST register
     */
    void write_FXST(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXINFO register
     * \param value New value of the rh850g4 FXINFO register
     */
    void write_FXINFO(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXCFG register
     * \param value New value of the rh850g4 FXCFG register
     */
    void write_FXCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXXC register
     * \param value New value of the rh850g4 FXXC register
     */
    void write_FXXC(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 FXXP register
     * \param value New value of the rh850g4 FXXP register
     */
    void write_FXXP(unsigned int value);
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
     * \brief Sets the value of the rh850g4 GMPLMR register
     * \param value New value of the rh850g4 GMPLMR register
     */
    void write_GMPLMR(unsigned int value);
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
     * \brief Sets the value of the rh850g4 MCI register
     * \param value New value of the rh850g4 MCI register
     */
    void write_MCI(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPIDX register
     * \param value New value of the rh850g4 MPIDX register
     */
    void write_MPIDX(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPBK register
     * \param value New value of the rh850g4 MPBK register
     */
    void write_MPBK(unsigned int value);
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
     * \brief Sets the value of the rh850g4 MPID0 register
     * \param value New value of the rh850g4 MPID0 register
     */
    void write_MPID0(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID1 register
     * \param value New value of the rh850g4 MPID1 register
     */
    void write_MPID1(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID2 register
     * \param value New value of the rh850g4 MPID2 register
     */
    void write_MPID2(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID3 register
     * \param value New value of the rh850g4 MPID3 register
     */
    void write_MPID3(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID4 register
     * \param value New value of the rh850g4 MPID4 register
     */
    void write_MPID4(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID5 register
     * \param value New value of the rh850g4 MPID5 register
     */
    void write_MPID5(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID6 register
     * \param value New value of the rh850g4 MPID6 register
     */
    void write_MPID6(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 MPID7 register
     * \param value New value of the rh850g4 MPID7 register
     */
    void write_MPID7(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGL register
     * \param value New value of the rh850g4 ICTAGL register
     */
    void write_ICTAGL(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICTAGH register
     * \param value New value of the rh850g4 ICTAGH register
     */
    void write_ICTAGH(unsigned int value);
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
     * \brief Sets the value of the rh850g4 ICCFG register
     * \param value New value of the rh850g4 ICCFG register
     */
    void write_ICCFG(unsigned int value);
    /**
     * \brief Sets the value of the rh850g4 ICERR register
     * \param value New value of the rh850g4 ICERR register
     */
    void write_ICERR(unsigned int value);
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
    
    /**
     * \brief Reads the opcode of the instruction currently in the
     * decode translation phase.
     * \return Opcode value
     */
    rh850g4::opcode_t get_decode_opcode();
    /**
     * \brief Reads the opcode of the instruction currently in the
     * execute translation phase.
     * \return Opcode value
     */
    rh850g4::opcode_t get_execute_opcode();
    
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
    rh850g4::cpu_state_t *internal_cpu_state;
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
    rh850g4::translator_base_t * m_curr_trans;
    
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
    rh850g4::bus_interface_t *fastiss_bus_interface;
    /**
     * \brief A pointer to the memory access implementor
     */          
    rh850g4::memory_if_t *fastiss_memory;

    /**
     * \brief A pointer to the operand extractor implementor
     */          
    rh850g4::operand_if_t *operand;
    /**
     * \brief A pointer to the CPU state implementor
     */          
    rh850g4::cpu_state_if_t *cpu_state;
    /**
     * \brief A pointer to the step translator
     */          
    rh850g4::translator_base_t *step_translator;
    /**
     * \brief A pointer to the continue translator
     */          
    rh850g4::continue_translator_base_t *continue_translator;

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
    rh850g4::block_function_l1_cache_t *m_fn_l1_cache;
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
    rh850g4::block_function_l2_cache_t *m_fn_l2_cache;
   
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
    rh850g4::analysis_callback_manager *m_analysis_callback_manager;
    
  protected:
    /**
     * \brief Pointer to the completed advance analysis point
     */
    rh850g4::analysis_point_if* m_completed_advance_analysis_point;
    /**
     * \brief Pointer to the pre-translate loop analysis point
     */
    rh850g4::analysis_point_if* m_pre_translate_loop_analysis_point;
    /**
     * \brief Pointer to the post-translate loop analysis point
     */
    rh850g4::analysis_point_if* m_post_translate_loop_analysis_point;
    /**
     * \brief Pointer to the post-fetch analysis point
     */
    rh850g4::analysis_point_if* m_post_fetch_analysis_point;
    /**
     * \brief Pointer to the post-decode analysis
     */
    rh850g4::analysis_point_if* m_post_decode_analysis_point;
    /**
     * \brief Pointer to the post-execute analysis
     */
    rh850g4::analysis_point_if* m_post_execute_analysis_point;
    /**
     * \brief Pointer to the interrupt acceptance analysis point
     */
    rh850g4::analysis_point_if* m_interrupt_accepted_analysis_point;
    /**
     * \brief Pointer to the interrupt completion analysis point
     */
    rh850g4::analysis_point_if* m_interrupt_completed_analysis_point;
    /**
     * \brief Pointer to the reset completion analysis point
     */
    rh850g4::analysis_point_if* m_reset_completed_analysis_point;
    /**
     * \brief Pointer to the immediate context switch analysis point
     */
    rh850g4::analysis_point_if* m_immediate_context_switch_analysis_point;
    /**
     * \brief Pointer to the generative context switch analysis point
     */
    rh850g4::analysis_point_if* m_generative_context_switch_analysis_point;
    
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

#endif // _RH850G4_CORE_BASE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
