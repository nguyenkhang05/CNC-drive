//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850G4_MEMORY_ACCESS_PLUGIN_H_
#define _RH850G4_MEMORY_ACCESS_PLUGIN_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

// Analysis interface header files
#include "rh850g4/analysis_interface_info.h"

// Standard VLAB header files
#include <vworks/core_analysis_interface.hpp>

//------------------------------------------------------------------------------
// Public operations
//------------------------------------------------------------------------------

/**
 * \brief Register callback functions for core memory accesses.
 *
 * \param core_context Pointer to core related data, passed to VLAB through
 *     the CoreAnalysisInterface, for VLAB to pass back to identify the required
 *     core.
 * \param analysis_context Pointer to VLAB managed analysis data, which will be
 *     passed back as a parameter to the callback invoked by the core.
 * \param write_func Callback function pointer invoked when a write access to
 *     memory has occurred. Set to NULL if not required.
 * \param read_func Callback function pointer invoked when a read access to
 *     memory has occurred. Set to NULL if not required.
 *
 * \return true if OK, false if an error occurred
 */
extern "C" bool add_memory_access_cb(
  void* core_context,
  void* analysis_context,
  memory_access_callback_func_t write_func,
  memory_access_callback_func_t read_func);

/**
 * \brief De-register callback functions for core memory accesses.
 *
 * \param analysis_context Pointer to VLAB managed analysis data, associated
 *     with callbacks which are to no longer be invoked by the core.
 *
 * \return true if OK, false if an error occurred
 */
extern "C" bool remove_memory_access_cb(void* analysis_context);

//------------------------------------------------------------------------------
// Analysis interface internal operations
//------------------------------------------------------------------------------
  
/**
 * \brief Initialise the memory access plug-in.
 *
 * \param Pointer to the analysis interface analysis interface structure.
 */
void initialise_memory_access_plugin(AnalysisInterfaceInfo *aii);

#endif // _RH850G4_MEMORY_ACCESS_PLUGIN_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
