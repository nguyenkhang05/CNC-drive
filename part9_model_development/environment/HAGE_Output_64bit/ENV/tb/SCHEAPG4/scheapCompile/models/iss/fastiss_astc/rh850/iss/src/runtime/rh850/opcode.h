//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2019.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file opcode.h
 * \brief Defines the rh850 opcode type
 */

#ifndef _RH850_OPCODE_H_
#define _RH850_OPCODE_H_

//------------------------------------------------------------------------------
// rh850 opcode type definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850 specific components
 */  
namespace rh850
{
  /**
   * \brief The opcode type definition used for all rh850 decode and operand extraction operations
   */
  typedef unsigned long long opcode_t;
}

#endif // _RH850_OPCODE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
