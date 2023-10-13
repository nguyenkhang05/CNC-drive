//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2017.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

#ifndef _RH850G4_ANALYSIS_INTERFACE_H_
#define _RH850G4_ANALYSIS_INTERFACE_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/core_base.h"
#include <vworks/core_analysis_interface.hpp>

//------------------------------------------------------------------------------
// Analysis interface creation
//------------------------------------------------------------------------------
  
/**
 * \brief Returns a pointer to a core analysis interface that may be used to
 * perform global variable tracing using the specified core. Other analyses
 * are not supported by the returned core analysis interface.
 * \param core Pointer to an rh850g4 instance
 * \return Core analysis interface for the specified core instance.
 */
CoreAnalysisInterface *create_analysis_interface(rh850g4::core_base_t *core);

#endif // _RH850G4_ANALYSIS_INTERFACE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
