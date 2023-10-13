//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file memory_if.h
 * \brief Defines the Memory interface class
 */

#ifndef _RH850_MEMORY_IF_H_
#define _RH850_MEMORY_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/debugger_if.h"

//------------------------------------------------------------------------------
// Class pre-declarations
//------------------------------------------------------------------------------

namespace fastiss
{
  class i8_t;
  class i16_t;
  class i32_t;
  class i64_t;
  class i1_ptr_t;
  class block_t;
  class code_gen_if_t;
}

namespace rh850
{
  class cpu_state_if_t;
  class bus_interface_t;
}

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \enum rh850::access_type_t
   * \brief Enumerated type to represent the type of a memory access
   */
  typedef enum
  {
    LOAD_ACCESS,        // Load access
    STORE_ACCESS,       // Store access
    DEBUG_LOAD_ACCESS,  // Debug load access
    DEBUG_STORE_ACCESS, // Debug store access
    FETCH_ACCESS,       // Fetch access
    IGNORE_ACCESS       // Ignore (zero sized) access
  } access_type_t;
}

//------------------------------------------------------------------------------
// Memory interface class
//------------------------------------------------------------------------------

namespace rh850
{
  /**
   * \brief Interface providing memory access operations for a Fast-ISS.
   * The operations provided include debugger, immediate, and generative memory
   * load and store operations. The memory access operations automatically
   * resolve the addresses as either to the internal ROM, internal RAM, or
   * external memory via a TLM bus
   */

  class memory_if_t {
  public:
    /**
     * \defgroup MemoryOperations Memory operations
     * @{
     * \defgroup FetchOperations Fetch operations
     * \defgroup DebuggerLoadStoreOperations Debugger load and store operations
     * \defgroup ImmediateLoadStoreOperations Immediate load and store operations
     * \defgroup GenerativeLoadStoreOperations Generative load and store operations
     * \defgroup ImmediateAddressCalculationOperations Immediate Address calculation operations
     * \defgroup GenerativeAddressCalculationOperations Generative Address calculation operations
     * @}
     */

    /**
     * \addtogroup MemoryOperations
     * @{
     */
    /**
     * \brief Initialises the memory sub-system
     * \param cpu_state A pointer to the CPU state implementor
     * \param code_generator A pointer to the code generator implementor
     * \param bus_interface A pointer to the bus interface implementor
     * \param tlm_response_error_as_error Flag indicating whether to report TLM response errors as errors or warnings
     */
    virtual void initialise
    (
      rh850::cpu_state_if_t *cpu_state,
      fastiss::code_gen_if_t* code_generator,
      rh850::bus_interface_t* bus_interface,
      bool tlm_response_error_as_error
    ) = 0;
    /**
     * @}
     */
     
    /**
     * \addtogroup DebuggerWatchpointOperations
     * @{
     */

    /**
     * \brief Add watchpoint operation.
     * \param address - Integer specifying start address of watchpoint region.
     * \param bus_interface_id - Identifier for bus interface on which the watchpoint should be set.
     * 0 - default bus
     * \param size - Integer specifying size of watchpoint region in bytes.
     * \param kind - Specifies the kind of transaction on which the watchpoint
     * should trigger, ie. read, write or value (either read or write).
     * \return Watchpoint descriptor for the new watchpoint, these values are used
     * by the erase_watchpoint() and pending_watchpoints() operations.
     */
    virtual unsigned int add_watchpoint(unsigned int address, unsigned int bus_interface_id, unsigned int size, rh850::watchpoint_kind_t kind) = 0;

    /**
     * \brief Erase watchpoint operation - removes first equivalent
     * watchpoint.
     * \param wp - Descriptor corresponding to the watchpoint to be deleted
     */
    virtual void erase_watchpoint( unsigned int wp ) = 0;
    
    /**
     * \brief Clear all watchpoints operation. Note that the translation cache
     * should always flush after this operation.
     */
    virtual void clear_all_watchpoints(void) = 0;

    /**
     * \brief Returns the number of current watchpoints
     */
    virtual unsigned int num_watchpoints(void) = 0;

    /**
     * \brief Memory subsystem operation which is used to indicate that an
     * immediate operation such as a helper function may have issued a memory
     * access transaction which could cause a watchpoint to be triggered and
     * denote that consequently the translator should insert a watchpoint
     * check.
     */
    virtual void set_check_watchpoint_hit(void) = 0;
    
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
     * \addtogroup DebuggerLoadStoreOperations
     * @{
     */
    /**
     * \brief Loads an arbitrary size value from memory using a debug read
     * \param addr The address for the read
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually read
     */
    virtual unsigned int debugger_load(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * \brief Loads an arbitrary size value from memory using a debug read at a physical address
     * \param addr The physical address for the read
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be read
     * \return The amount of data actually read
     */
    virtual unsigned int debugger_load_physical(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * \brief Stores an arbitrary size value to memory using a debug write
     * \param addr The address for the write
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be written
     * \return The amount of data actually written
     */
    virtual unsigned int debugger_store(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * \brief Stores an arbitrary size value to memory using a debug write at a physical address
     * \param addr The physical address for the write
     * \param data The buffer into which the read data is to be written
     * \param size The amount of data to be written
     * \return The amount of data actually written
     */
    virtual unsigned int debugger_store_physical(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup FetchOperations
     * @{
     */
    /**
     * \brief Fetches a 8-bit value from memory using a debug read operation
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned char i8_fetch(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Fetches a 16-bit value from memory using a debug read operation
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned short i16_fetch(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Fetches a 32-bit value from memory using a debug read operation
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned int i32_fetch(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Fetches a 64-bit value from memory using a debug read operation
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned long long i64_fetch(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Fetches a 128-bit value from memory using a debug read operation
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual rh850::uint128_t i128_fetch(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Fetches an arbitrary size value from memory using a debug read
     * \param addr The address for the read
     * \param size The amount of data to be read
     * \param data The buffer into which the read data is to be written
     * \return The amount of data actually read
     */
    virtual unsigned int iN_fetch(unsigned int addr, unsigned int size, unsigned char *data) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup IgnoreOperations
     * @{
     */
    /**
     * \brief Immediate operation used to issue a TLM_IGNORE transaction on the default bus.
     * \param addr The address at which to issue the ignore transaction
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void issue_ignore(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Generates the code to issue a TLM_IGNORE transaction on the default bus
     * \param addr The address at which to issue the ignore transaction
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void gen_issue_ignore(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL) = 0;
    /**
     * @}
     */

    /**
     * \addtogroup ImmediateLoadStoreOperations
     * @{
     */
    /**
     * \brief Loads a 8-bit value from memory using an immediate read on the default bus.
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned char i8_load(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Loads a 16-bit value from memory using an immediate read on the default bus.
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned short i16_load(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Loads a 32-bit value from memory using an immediate read on the default bus.
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned int i32_load(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Loads a 64-bit value from memory using an immediate read on the default bus.
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual unsigned long long i64_load(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Loads a 128-bit value from memory using an immediate read on the default bus.
     * \param addr The address from which to load a value
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \return The loaded value
     */
    virtual rh850::uint128_t i128_load(unsigned int addr, bool *is_error = NULL) = 0;
    /**
     * \brief Stores a 8-bit value to memory using an immediate write on the default bus.
     * \param addr The address to store a value to
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void i8_store(unsigned int addr, unsigned char data, bool *is_error = NULL) = 0;
    /**
     * \brief Stores a 16-bit value to memory using an immediate write on the default bus.
     * \param addr The address to store a value to
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void i16_store(unsigned int addr, unsigned short data, bool *is_error = NULL) = 0;
    /**
     * \brief Stores a 32-bit value to memory using an immediate write on the default bus.
     * \param addr The address to store a value to
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void i32_store(unsigned int addr, unsigned int data, bool *is_error = NULL) = 0;
    /**
     * \brief Stores a 64-bit value to memory using an immediate write on the default bus.
     * \param addr The address to store a value to
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void i64_store(unsigned int addr, unsigned long long data, bool *is_error = NULL) = 0;
    /**
     * \brief Stores a 128-bit value to memory using an immediate write on the default bus.
     * \param addr The address to store a value to
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the boolean flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     */
    virtual void i128_store(unsigned int addr, rh850::uint128_t data, bool *is_error = NULL) = 0;
    /**
     * @}
     */

     /**
     * \addtogroup GenerativeLoadStoreOperations
     * @{
     */
    /**
     * \brief Generates a load of a 8-bit value from memory
     * \param addr The address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual fastiss::i8_t* gen_i8_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * \brief Generates a load of a 16-bit value from memory
     * \param addr The address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual fastiss::i16_t* gen_i16_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * \brief Generates a load of a 32-bit value from memory
     * \param addr The address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual fastiss::i32_t* gen_i32_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * \brief Generates a load of a 64-bit value from memory
     * \param addr The address to load a value from (as a 32-bit value)
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual fastiss::i64_t* gen_i64_load(fastiss::i32_t* addr, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * \brief Generates a store of a 8-bit value to memory
     * \param addr The address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual void gen_i8_store(fastiss::i32_t* addr, fastiss::i8_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * \brief Generates a store of a 16-bit value to memory
     * \param addr The address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual void gen_i16_store(fastiss::i32_t* addr, fastiss::i16_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * \brief Generates a store of a 32-bit value to memory
     * \param addr The address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual void gen_i32_store(fastiss::i32_t* addr, fastiss::i32_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * \brief Generates a store of a 64-bit value to memory
     * \param addr The address to store a value to (as a 32-bit value)
     * \param data The value to be stored
     * \param is_error If a non-NULL pointer is provided then the 1-bit flag is
     * updated with true if an error occurred (e.g. due to virtual to physical
     * address translation or due to a TLM error) or false otherwise.
     * \param success_block If a non-NULL block is provided then execution
     * will branch to the supplied block in the case of an L1 DMI table hit
     * \return The loaded value
     */
    virtual void gen_i64_store(fastiss::i32_t* addr, fastiss::i64_t* data, fastiss::i1_ptr_t* is_error = NULL, fastiss::block_t* success_block = NULL) = 0;
    /**
     * @}
     */

     /**
     * \addtogroup ImmediateAddressCalculationOperations
     * @{
     */
    /**
     * \brief Operation used to calculate the 64-bit effective address of a
     * transaction from the 32-bit virtual address.
     * \param type The transaction type (either a load, store, debug load, debug
     * store, fetch, or ignore (zero sized) access)
     * \param virtual_address The virtual address of the transaction
     * \return The 64-bit effective address
     */
    virtual unsigned long long calculate_effective_address(rh850::access_type_t type, unsigned int virtual_address) = 0;
    /**
     * \brief Operation used to calculate the 32-bit physical address of a
     * transaction from the 64-bit effective address.
     * \param eff_address The effective address of the transaction
     * \param physical_address A pointer to the storage location to update with
     * the physical address
     * \param physical_region_start_address If not NULL, the pointed to storage
     * location is updated with the start of the physical address region (for
     * example the start of the MMU page or MPU region)
     * \param physical_region_end_address If not NULL, the pointed to storage
     * location is updated with the end of the physical address region (for
     * example the end of the MMU page or MPU region)
     * \return A boolean that is true if the physical address could be calculated,
     * and false otherwise if an error occurred. An error could occur for example
     * if memory protection settings prevent the access.
     */
    virtual bool calculate_physical_address(rh850::access_type_t type,
        unsigned long long eff_address,
        unsigned int *physical_address,
        unsigned int *physical_region_start_address,
        unsigned int *physical_region_end_address) = 0;
    /**
     * @}
     */
     
     /**
     * \addtogroup GenerativeAddressCalculationOperations
     * @{
     */
    /**
     * \brief Operation used to generate the calculation of the 64-bit effective
     * address of an ignore (zero sized) transaction from the 32-bit virtual address.
     * \param virtual_address The virtual address of the ignore (zero sized)
     * transaction
     * \return The 64-bit effective address
     */
    virtual fastiss::i64_t* gen_calculate_ignore_effective_address(fastiss::i32_t* virtual_address) = 0;
    
    /**
     * \brief Operation used to generate the calculation of the 64-bit effective
     * address of a load transaction from the 32-bit virtual address.
     * \param virtual_address The virtual address of the load transaction
     * \return The 64-bit effective address
     */
    virtual fastiss::i64_t* gen_calculate_load_effective_address(fastiss::i32_t* virtual_address) = 0;
    
    /**
     * \brief Operation used to generate the calculation of the 64-bit effective
     * address of a store transaction from the 32-bit virtual address.
     * \param virtual_address The virtual address of the store transaction
     * \return The 64-bit effective address
     */
    virtual fastiss::i64_t* gen_calculate_store_effective_address(fastiss::i32_t* virtual_address) = 0;
    /**
     * @}
     */
  };
}

#endif // _RH850_MEMORY_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
