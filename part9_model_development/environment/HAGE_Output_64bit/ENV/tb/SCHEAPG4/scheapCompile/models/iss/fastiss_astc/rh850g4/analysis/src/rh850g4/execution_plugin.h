//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850G4_EXECUTION_PLUGIN_H_
#define _RH850G4_EXECUTION_PLUGIN_H_

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
 * \brief Register pc execution callback
 *
 * Register a callback function pointer. The registered function is called
 * whenever a pc of interest is executed.
 *
 * \param core_context Pointer to core related data, passed to VLAB through
 *     the CoreAnalysisInterface, for VLAB to pass back to identify the required
 *     core.
 * \param analysis_context Pointer to VLAB managed analysis data, which will be
 *     passed back as a parameter to the callback invoked by the core.
 * \param pc_exec_func Callback function pointer invoked on execution of an
 *     address of interest.
 * \param pc_query_func Callback function pointer used to determine if a given
 *     execution address is of interest.
 *
 * \return true if OK, false if an error occurred
 */
extern "C" bool add_execution_cb(
    void *core_context,
    void *analysis_context,
    pc_exec_callback_func_t pc_exec_func,
    pc_query_callback_func_t pc_query_func);

/**
 * \brief De-register pc execution callback
 *
 * \param analysis_context Pointer to VLAB managed analysis data, associated
 *     with callbacks which are to no longer be invoked by the core.
 *
 * \return true if OK, false if an error occurred
 */
extern "C" bool remove_execution_cb(void *analysis_context);

//------------------------------------------------------------------------------
// Analysis interface internal operations
//------------------------------------------------------------------------------

/**
 * \brief Initialise the execution plug-in.
 *
 * \param Pointer to the analysis interface analysis interface structure.
 */
void initialise_execution_plugin(AnalysisInterfaceInfo *aii);

#endif // _RH850G4_EXECUTION_PLUGIN_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
