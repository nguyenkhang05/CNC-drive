/*************************************************************************
 *
 *  reg_super.h
 *
 * Copyright(c) 2010-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
*************************************************************************/
/// @reg_super.h
/// @brief Supply functions to access memory and to check register access status.
/// $Id: reg_super.h,v 1.13 2013/02/07 05:33:06 watanama Exp $
/// $Date: 2013/02/07 05:33:06 $
/// $Revison$
/// $Author: watanama $

#ifndef __REG_SUPER_H
#define __REG_SUPER_H

#include <string>

namespace vpcl
{

/// register super class
class reg_super
{

public:
  /// write call-back function
  /// @return none
  virtual void wr_cb(
    const unsigned int addr,           ///< [in] memory address
    unsigned int data                  ///< [in] written data to memory
  )
  {
    array[addr&0xFFFF] = data;
  }

  /// read call-back function
  /// @return value of specified memory
  virtual unsigned int rd_cb(
    const unsigned int addr            ///< [in] memory address
  )
  {
    return array[addr&0xFFFF];
  }

  /// check access limitaton dynamically
  /// @return true/false if specified register or bit allows/not allows to access
  virtual bool ChkAccess(
    const std::string channel_name,    ///< [in] channel name
    const std::string register_name,   ///< [in] register name
    const std::string bit_name         ///< [in] bit name
  )
  {
    return true;
  }
  /// check access limitaton dynamically
  /// @return true/false if specified register or bit allows/not allows to access
  virtual bool ChkAccess(
    const bool is_wr,                  ///< [in] write/read process 
    const std::string channel_name,    ///< [in] channel name
    const std::string register_name,   ///< [in] register name
    const std::string bit_name         ///< [in] bit name
  )
  {
    return ChkAccess(channel_name, register_name, bit_name);
  }

  /// constructor of register super class
  /// @return none
  reg_super() {
    for (unsigned int i=0 ; i<0x10000 ; i++) array[i] = 0;
  }

  /// destructor of register super class
  /// @return none
  virtual ~reg_super(){ ; }

  unsigned int array[0x10000];         ///< stock register value
};

} //namespace
#endif//__REG_SUPER_H
