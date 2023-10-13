//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850G4_ANALYSIS_INTERFACE_INFO_H_
#define _RH850G4_ANALYSIS_INTERFACE_INFO_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// Core runtime header files
#include "rh850g4/core_base.h"

// Standard VLAB header files
#include <vworks/core_analysis_interface.hpp>

//------------------------------------------------------------------------------
// Data structures
//------------------------------------------------------------------------------
  
/**
 * \brief Data structure defining the top level analysis interface information.
 */
class AnalysisInterfaceInfo
{
public:
  /**
   * \brief Pointer to the core instance with which this core context is
   * associated.
   */
  rh850g4::core_base_t *m_core;
  
  /**
   * \brief Descriptor for the internal execution plug-in callback.
   */
  rh850g4::analysis_callback_descriptor *m_execute_callback;
  
  /**
   * \brief Descriptor for the internal execution plug-in callback.
   */
  rh850g4::analysis_callback_descriptor *m_memory_access_callback;

  /**
   * \brief Descriptor for the internal function call plug-in start of symbol callback.
   */
  rh850g4::analysis_callback_descriptor *m_start_symbol_callback;

  /**
   * \brief Descriptor for the internal function call plug-in known symbol callback.
   */
  rh850g4::analysis_callback_descriptor *m_known_symbol_callback;

  /**
   * \brief Descriptor for the internal function call plug-in unknown symbol callback.
   */
  rh850g4::analysis_callback_descriptor *m_unknown_symbol_callback;
  
  /**
   * \brief Core analysis interface structure a pointer to which is
   * returned to VLAB by the create_analysis_interface() operation.
   */
  CoreAnalysisInterface m_itf;
};

#endif // _RH850G4_ANALYSIS_INTERFACE_INFO_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
