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

#include "rh850g4/immediate_base.h"

//------------------------------------------------------------------------------
// Immediate base class implementation
//------------------------------------------------------------------------------

// Constructor/destructor
rh850g4::immediate_base_t::immediate_base_t
(
  rh850g4::memory_if_t *memory_impl,
  rh850g4::cpu_state_if_t *cpu_state_impl,
  rh850g4::custom_if_t *custom_impl
) :
  rh850g4::memory_bridge_t(memory_impl), 
  rh850g4::cpu_state_bridge_t(cpu_state_impl),
  rh850g4::custom_bridge_t(custom_impl)
{
}

rh850g4::immediate_base_t::~immediate_base_t()
{
}

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
