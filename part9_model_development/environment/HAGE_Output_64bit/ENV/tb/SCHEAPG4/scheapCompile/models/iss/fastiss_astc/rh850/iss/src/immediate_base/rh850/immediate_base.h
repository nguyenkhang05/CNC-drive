//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file immediate_base.h
 * \brief Defines common base class that provides helper functions with access
 * to immediate operations but allows derived instructions access to both
 * immediate and generative operations
 */

#ifndef _RH850_IMMEDIATE_BASE_H_
#define _RH850_IMMEDIATE_BASE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850/memory_bridge.h"
#include "rh850/cpu_state_bridge.h"
#include "rh850/custom_bridge.h"

//------------------------------------------------------------------------------
// rh850 instruction base class definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850 specific components
 */  
namespace rh850
{
  /**
   * \brief Base class that provides helper functions with access to immediate
   * operations but allows derived instructions access to both immediate and
   * generative operations
   */

  class immediate_base_t :
    public rh850::memory_bridge_t, 
    public rh850::cpu_state_bridge_t,
    public rh850::custom_bridge_t
  {
  public:
    /**
     * \brief Constructor
     * \param memory_impl Handle to memory access sub-system
     * \param cpu_state_impl Handle to CPU state instance
     * \param custom_impl Handle to custom operations implementation
     */
    immediate_base_t
    (
      rh850::memory_if_t *memory_impl,
      rh850::cpu_state_if_t *cpu_state_impl,
      rh850::custom_if_t *custom_impl
    );
    
    /**
     * \brief Destructor
     */
    virtual ~immediate_base_t();
  };
}

#endif // _RH850_IMMEDIATE_BASE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
