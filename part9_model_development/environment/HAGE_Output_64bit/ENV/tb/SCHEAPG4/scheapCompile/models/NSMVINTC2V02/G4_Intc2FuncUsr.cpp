/*************************************************************************
 *
 * $Id: G4_Intc2FuncUsr.cpp,v 1.5 2020/03/24 10:02:05 hadinhtran Exp $
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
#include "NSMVINTC2V02.h"
#include "G4_Intc2Wrapper.h"
#include "G4_Intc2FuncUsr.h"

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT

/// <summary>constructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_Intc2FuncUsr::G4_Intc2FuncUsr( void ) : mpWrapper( 0 )
{
  dmsgsc( "G4_Intc2FuncUsr::G4_Intc2FuncUsr()" );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_Intc2FuncUsr::~G4_Intc2FuncUsr()
{
  dmsgsc( "G4_Intc2FuncUsr::~G4_Intc2FuncUsr()" );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}

///<summary>set pointer to INTC2 wrapper</summary>
///<param name="ptr">pointer to INTC2 module</param>
///<returns>none</returns>
void 
G4_Intc2FuncUsr::setWrapperPtr( NSMVINTC2V02 *ptr )
{
  assert(ptr != NULL);
  dmsgsc( "G4_Intc2FuncUsr::setWrapperPtr()" );
  mpWrapper = reinterpret_cast<G4_Intc2Wrapper*>( ptr );
}


///<summary>called before calling TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc2FuncUsr::read_precallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc2FuncUsr::read_precallout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called after calling TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc2FuncUsr::read_postcallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc2FuncUsr::read_postcallout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called before calling TLM write function</summary>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc2FuncUsr::write_precallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc2FuncUsr::write_precallout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called after calling TLM write function</summary>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc2FuncUsr::write_postcallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc2FuncUsr::write_postcallout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to request EIINT is called</summary>
///<param name="peid">PEID</param>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc2FuncUsr::ReqEiintToIntc1_callout(
  uint32_t peid,
  uint32_t channel )
{
  dmsgsc("G4_Intc2FuncUsr::ReqEiintToIntc1_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to drop EIINT request is called</summary>
///<param name="peid">PEID</param>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc2FuncUsr::CancelEiintToIntc1_callout(
  uint32_t peid,
  uint32_t channel )
{
  dmsgsc("G4_Intc2FuncUsr::CancelEiintToIntc1_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}
