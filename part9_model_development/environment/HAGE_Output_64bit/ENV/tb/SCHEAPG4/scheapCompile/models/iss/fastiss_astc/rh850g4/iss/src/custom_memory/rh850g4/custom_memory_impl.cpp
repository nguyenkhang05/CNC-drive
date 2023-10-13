//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/custom_memory_impl.h"
#include "fastiss/code_gen_if.h"

//------------------------------------------------------------------------------
// rh850g4 custom memory implementation
//------------------------------------------------------------------------------

rh850g4::custom_memory_impl_t::custom_memory_impl_t() :
  rh850g4::memory_impl_t()
{
}

rh850g4::custom_memory_impl_t::~custom_memory_impl_t()
{
}

/* Memory initialisation operations */
void rh850g4::custom_memory_impl_t::initialise
(
  cpu_state_if_t *cpu_state,
  fastiss::code_gen_if_t *code_generator,
  bus_interface_t *bus_interface,
  bool tlm_response_error_as_error
)
{  
  // Call base operation
  rh850g4::memory_impl_t::initialise
    (
      cpu_state,
      code_generator,
      bus_interface,
      tlm_response_error_as_error
    );
}

/* Immediate memory access operations */
unsigned long long rh850g4::custom_memory_impl_t::calculate_effective_address(rh850g4::access_type_t type, unsigned int virtual_address)
{
  return ((unsigned long long) virtual_address);
}

bool rh850g4::custom_memory_impl_t::calculate_physical_address(rh850g4::access_type_t type,
    unsigned long long eff_address,
    unsigned int *physical_address,
    unsigned int *physical_region_start_address,
    unsigned int *physical_region_end_address)
{
  *physical_address = (unsigned int)eff_address;
  return false;
}

/* Generative memory access operations */
fastiss::i64_t* rh850g4::custom_memory_impl_t::gen_calculate_ignore_effective_address(fastiss::i32_t* virtual_address)
{
  return m_code_gen_impl->gen_i64_zext(virtual_address);
}

fastiss::i64_t* rh850g4::custom_memory_impl_t::gen_calculate_load_effective_address(fastiss::i32_t* virtual_address)
{
  return m_code_gen_impl->gen_i64_zext(virtual_address);
}

fastiss::i64_t* rh850g4::custom_memory_impl_t::gen_calculate_store_effective_address(fastiss::i32_t* virtual_address)
{ 
  return m_code_gen_impl->gen_i64_zext(virtual_address);
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
