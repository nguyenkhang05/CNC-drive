//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2014.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850G4_FUNCTION_CALL_PLUGIN_H_
#define _RH850G4_FUNCTION_CALL_PLUGIN_H_

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
 * \brief Register callback functions for target software function calls and
 *     returns.
 *
 * \param core_context Pointer to core related data, passed to VLAB through
 *     the CoreAnalysisInterface, for VLAB to pass back to identify the required
 *     core.
 * \param analysis_context Pointer to VLAB managed analysis data, which will be
 *     passed back as a parameter to the callback invoked by the core.
 * \param call_func Callback function pointer invoked when a function call has
 *     occurred for a function of interest.
 * \param return_func Callback function pointer invoked when a function of
 *     interest is returned from or returned into. Set to NULL if not required.
 * \param query_func Callback function pointer used to determine if a given
 *     execution address is part of a function of interest.
 *
 * \return True if OK
 *         False if an error occurred
 */
extern "C" bool add_func_call_cb(
    void *core_context,
    void *analysis_context,
    func_callback_func_t call_func,
    func_callback_func_t return_func,
    func_query_callback_func_t query_func);

/**
 * \brief De-register callback functions for target software function calls and
 *     returns.
 *
 * \param analysis_context Pointer to VLAB managed analysis data, associated
 *     with callbacks which are to no longer be invoked by the core.
 *
 * \return True if OK
 *         False if an error occurred
 */
extern "C" bool remove_func_call_cb(void *analysis_context);

//------------------------------------------------------------------------------
// Analysis interface internal operations
//------------------------------------------------------------------------------

/**
 * \brief Initialise the function call plug-in.
 *
 * \param Pointer to the analysis interface analysis interface structure.
 */
void initialise_function_call_plugin(AnalysisInterfaceInfo *aii);

#endif // _RH850G4_FUNCTION_CALL_PLUGIN_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
