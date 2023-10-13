//------------------------------------------------------------------------------
// Copyright (C) Australian Semiconductor Technology Company. 2020.
// All Rights Reserved.
//
// This is unpublished proprietary source code of the Australian Semiconductor
// Technology Company (ASTC).  The copyright notice does not evidence any actual
// or intended publication of such source code.
//------------------------------------------------------------------------------

/**
 * \file opcode.h
 * \brief Defines the rh850g4 opcode type
 */

#ifndef _RH850G4_OPCODE_H_
#define _RH850G4_OPCODE_H_

//------------------------------------------------------------------------------
// rh850g4 opcode type definition
//------------------------------------------------------------------------------

/**
 * \brief Contains rh850g4 specific components
 */  
namespace rh850g4
{
  /**
   * \brief The opcode type definition used for all rh850g4 decode and operand extraction operations
   */
  typedef unsigned long long opcode_t;
}

#endif // _RH850G4_OPCODE_H_

//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------
