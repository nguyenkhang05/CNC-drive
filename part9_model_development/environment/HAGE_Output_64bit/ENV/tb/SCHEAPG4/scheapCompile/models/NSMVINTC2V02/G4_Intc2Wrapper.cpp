/*************************************************************************
 *
 * $Id: G4_Intc2Wrapper.cpp,v 1.4 2020/02/24 10:49:52 hadinhtran Exp $
 *
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include "G4_intc_def.h"
#include "G4_Intc2Wrapper.h"

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT

/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <param name="peid">PEID</param>
/// <returns>none</returns>
G4_Intc2Wrapper::G4_Intc2Wrapper(
  sc_module_name name,
  const char*    pCfgFileName ):
  NSMVINTC2V02( name, pCfgFileName )
{
  dmsgsc( "G4_Intc2Wrapper::G4_Intc2Wrapper()" );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_Intc2Wrapper::~G4_Intc2Wrapper( void )
{
  dmsgsc( "G4_Intc2Wrapper::~G4_Intc2Wrapper()" );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}


///<summary>read function</summary>
///<param name="addr">address</param>
///<param name="pData">pointer to data array</param>
///<param name="size">size of data array</param>
///<returns>none</returns>
void
G4_Intc2Wrapper::read (
  unsigned int   addr,
  unsigned char* pData,
  unsigned int   size )
{
  dmsgsc( "G4_Intc2Wrapper::read()" );
  TlmBasicPayload trans;
  BusTime_t       t;
  trans.set_command( tlm::TLM_READ_COMMAND );
  trans.set_address( addr );
  trans.set_data_length( size );
  if (pData != NULL){
      trans.set_data_ptr( pData );
  }
  (ts->get_base_interface()).transport_dbg( trans );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}

///<summary>write function</summary>
///<param name="addr">address</param>
///<param name="pData">pointer to data array</param>
///<param name="size">size of data array</param>
///<returns>none</returns>
void
G4_Intc2Wrapper::write (
  unsigned int   addr,
  unsigned char* pData,
  unsigned int   size )
{
  dmsgsc( "G4_Intc2Wrapper::write()" );
  TlmBasicPayload trans;
  BusTime_t       t;
  trans.set_command( tlm::TLM_WRITE_COMMAND );
  trans.set_address( addr );
  trans.set_data_length( size );
  if (pData != NULL){
      trans.set_data_ptr( pData );
  }
  (ts->get_base_interface()).transport_dbg( trans );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}


//------------ for user coding from here -------------------
/*
void
G4_Intc2Wrapper::user_function ( void )
{
  unsigned int base_addr = ts->getBaseAddress(); // to get base address
  cout << "Hello, world." << endl;
}
*/
//------------ for user coding up to here ------------------
