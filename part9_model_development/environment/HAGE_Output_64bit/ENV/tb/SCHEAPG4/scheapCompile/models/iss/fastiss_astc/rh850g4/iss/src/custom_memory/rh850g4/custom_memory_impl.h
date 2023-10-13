//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file custom_memory_impl.h
 * \brief Defines the customised memory implementation class
 */

#ifndef _RH850G4_CUSTOM_MEMORY_IMPL_H_
#define _RH850G4_CUSTOM_MEMORY_IMPL_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/memory_impl.h"

//------------------------------------------------------------------------------
// rh850g4 custom memory definition
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \brief rh850g4 customised memory implementation. This class provides
   * customised implementations of the methods available in the memory interface, allowing
   * customised memory access behaviour.
   */
  class custom_memory_impl_t : public rh850g4::memory_impl_t
  {
  public:
    /**
     * \brief Constructor
     */
    custom_memory_impl_t();
    /**
     * \brief Destructor
     */
    virtual ~custom_memory_impl_t();
    
    /**
     * \brief Initialises the memory sub-system
     * \param cpu_state A pointer to the CPU state implementor
     * \param code_generator A pointer to the code generator implementor
     * \param bus_interface A pointer to the bus interface implementor
     * \param tlm_response_error_as_error Flag indicating whether to report TLM response errors as errors or warnings
     */
    void initialise
    (
      rh850g4::cpu_state_if_t *cpu_state,
      fastiss::code_gen_if_t* code_generator,
      rh850g4::bus_interface_t* bus_interface,
      bool tlm_response_error_as_error
    );
    
    /**
     * \brief Operation used to calculate the 64-bit effective address of a
     * transaction from the 32-bit virtual address.
     * \param type The transaction type (either a load, store, debug load, debug
     * store, fetch, or ignore (zero sized) access)
     * \param virtual_address The virtual address of the transaction
     * \return The 64-bit effective address
     */
    unsigned long long calculate_effective_address(rh850g4::access_type_t type, unsigned int virtual_address);
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
    bool calculate_physical_address(rh850g4::access_type_t type,
        unsigned long long eff_address,
        unsigned int *physical_address,
        unsigned int *physical_region_start_address,
        unsigned int *physical_region_end_address);
    /**
     * \brief Operation used to generate the calculation of the 64-bit effective
     * address of an ignore (zero sized) transaction from the 32-bit virtual address.
     * \param virtual_address The virtual address of the ignore (zero sized)
     * transaction
     * \return The 64-bit effective address
     */
    fastiss::i64_t* gen_calculate_ignore_effective_address(fastiss::i32_t* virtual_address);
    
    /**
     * \brief Operation used to generate the calculation of the 64-bit effective
     * address of a load transaction from the 32-bit virtual address.
     * \param virtual_address The virtual address of the load transaction
     * \return The 64-bit effective address
     */
    fastiss::i64_t* gen_calculate_load_effective_address(fastiss::i32_t* virtual_address);
    
    /**
     * \brief Operation used to generate the calculation of the 64-bit effective
     * address of a store transaction from the 32-bit virtual address.
     * \param virtual_address The virtual address of the store transaction
     * \return The 64-bit effective address
     */
    fastiss::i64_t* gen_calculate_store_effective_address(fastiss::i32_t* virtual_address);
  };
}

#endif // _RH850G4_CUSTOM_MEMORY_IMPL_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
