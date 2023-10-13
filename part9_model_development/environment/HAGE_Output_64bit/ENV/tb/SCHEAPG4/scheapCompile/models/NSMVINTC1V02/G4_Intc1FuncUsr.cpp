/*************************************************************************
 *
 * $Id: G4_Intc1FuncUsr.cpp,v 1.6 2020/03/24 09:58:12 hadinhtran Exp $
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
#include "NSMVINTC1V02.h"
#include "G4_Intc1Wrapper.h"
#include "G4_Intc1FuncUsr.h"

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT

/// <summary>constructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_Intc1FuncUsr::G4_Intc1FuncUsr( void ) : mpWrapper( 0 )
{
  dmsgsc( "G4_Intc1FuncUsr::G4_Intc1FuncUsr()" );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
G4_Intc1FuncUsr::~G4_Intc1FuncUsr( void )
{
  dmsgsc( "G4_Intc1FuncUsr::~G4_Intc1FuncUsr()" );
  //------------ for user coding from here -------------------

  //------------ for user coding up to here ------------------
}

///<summary>set pointer to INTC1 wrapper</summary>
///<param name="ptr">pointer to INTC1 module</param>
///<returns>none</returns>
void 
G4_Intc1FuncUsr::setWrapperPtr( NSMVINTC1V02 *ptr )
{
  assert(ptr != NULL);
  dmsgsc( "G4_Intc1FuncUsr::setWrapperPtr()" );
  mpWrapper = reinterpret_cast<G4_Intc1Wrapper*>( ptr );
}


///<summary>called before calling TLM read function</summary>
///<param name="offsetAddress">offset address</param>
///<param name="trans">basic payload</param>
///<param name="t">pointer to variable of local time</param>
///<param name="debug">debug access or not</param>
///<returns>none</returns>
void
G4_Intc1FuncUsr::read_precallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc1FuncUsr::read_precallout()");
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
G4_Intc1FuncUsr::read_postcallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc1FuncUsr::read_postcallout()");
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
G4_Intc1FuncUsr::write_precallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc1FuncUsr::write_precallout()");
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
G4_Intc1FuncUsr::write_postcallout(
  TlmBasicPayload& trans,
  BusTime_t*       t,
  bool             debug )
{
  dmsgsc("G4_Intc1FuncUsr::write_postcallout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to request EIINT is called</summary>
///<param name="tcid">thread context id</param>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc1FuncUsr::ReqEiintToCpu_callout(
    uint32_t  channel,
    bool bindmode,
    uint32_t gpid,
    uint32_t bgpr)
{
  dmsgsc("G4_Intc1FuncUsr::ReqEiintToCpu_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to request FENMI is called</summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1FuncUsr::ReqFenmiToCpu_callout( )
{
  dmsgsc("G4_Intc1FuncUsr::ReqFenmiToCpu_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to request FEINT is called</summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1FuncUsr::ReqFeintToCpu_callout( uint32_t  channel, bool bindmode, uint32_t gpid, uint32_t bge)
{
  dmsgsc("G4_Intc1FuncUsr::ReqFeintToCpu_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to drop EIINT request is called</summary>
///<param name="tcid">thread context id</param>
///<param name="channel">number of channel</param>
///<returns>none</returns>
void
G4_Intc1FuncUsr::CancelEiintToCpu_callout( uint32_t  channel )
{
  dmsgsc("G4_Intc1FuncUsr::CancelEiintToCpu_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to drop FENMI request is called</summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1FuncUsr::CancelFenmiToCpu_callout( )
{
  dmsgsc("G4_Intc1FuncUsr::CancelFenmiToCpu_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}

///<summary>called when the function to drop FEINT request is called</summary>
///<param name="tcid">thread context id</param>
///<returns>none</returns>
void
G4_Intc1FuncUsr::CancelFeintToCpu_callout( void )
{
  dmsgsc("G4_Intc1FuncUsr::CancelFeintToCpu_callout()");
  //------------ for user coding from here -------------------
  // mpWrapper->user_function();

  //------------ for user coding up to here ------------------
}
