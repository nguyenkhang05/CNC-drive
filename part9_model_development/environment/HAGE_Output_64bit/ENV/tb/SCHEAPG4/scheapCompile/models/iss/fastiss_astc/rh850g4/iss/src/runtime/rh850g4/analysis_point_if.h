//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file analysis_point_if.h
 * \brief Definitions of analysis point interface
 */

#ifndef _RH850G4_ANALYSIS_POINT_IF_H_
#define _RH850G4_ANALYSIS_POINT_IF_H_

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "rh850g4/analysis_types.h"
#include "fastiss/code_gen_exceptions.h"

//------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------

namespace rh850g4
{
  /**
   * \class rh850g4::analysis_point_error
   * \brief Exception raised when errors conditions are detected by the analysis point hook
   */
  class analysis_point_error : public fastiss::error_base_t
  {
  public:
    /**
     * \brief Analysis point error constructor
     * \param message Message giving detailed error information
     */
    analysis_point_error(const char *const& message);
  };
  
  /**
   * \class rh850g4::analysis_point_if
   * \brief Public interface to analysis point
   */
  class analysis_point_if
  {
  public:
    /**
     * \brief Used to determine whether an analysis callback has been attached to the attachment point
     * \return Boolean value indicating whether an analysis callback has been attached to the attachment point
     */
    virtual bool has_attachments(void) = 0;
    
    /**
     * \brief Causes the all analysis callback operations associated with the analysis point to be invoked
     */
    virtual void activate(void) = 0;
  };
}

#endif // _RH850G4_ANALYSIS_POINT_IF_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
