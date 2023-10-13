/*************************************************************************
 *
 * MevAdptr.cpp
 *
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <cstdio>
#include "CmErrMsg.h"
#include "MevAdptr.h"
#include "mev_def.h"

using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream dmvfout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <returns>none</returns>
MevAdptr::MevAdptr( sc_module_name name ):
  sc_module( name ),
  is( 0 ),
  ts( 0 ),
  mPeNum( 1 ),
  mpClkDiv( 0 ),
  mRLatency( 3 ),
  mWLatency( 3 )
{
  mpModName = this->name();
  dmsgmv( mpModName << ".MevAdptr()" );
  is = new TlmInitiatorSocket<BUS_WIDTH_APB>( "is" );
  ts = new TlmTargetSocket<BUS_WIDTH_APB>( "ts" );

  (*is)( *this );
  (*ts)( *this );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
MevAdptr::~MevAdptr( void )
{
  dmsgmv( mpModName << ".~MevAdptr()" );
  delete is;
  delete ts;

  if ( 0 == mpClkDiv ) {
    for ( unsigned int i = 0; i < mPeNum; i++ ) {
      delete mpClkDiv[i];
    }
    delete[] mpClkDiv;
  }
  else {
    // do nothing
  }
}


// set function
/// <summary>set number of PE</summary>
/// <param name="peNum">number of PE</param>
/// <returns>none</returns>
void
MevAdptr::setPeNum( unsigned int peNum )
{
  dmsgmv( mpModName << ".setPeNum() peNum:" << peNum );

  if ( PE_MAX_NUM < peNum ) {
    sprintf( mErrMsg, "Invalid PE-number(%d).", peNum );
    printWarningMessage( mpModName, "setPeNum", mErrMsg );
  }
  else {
    if ( 0 == mpClkDiv ) {
      mpClkDiv = new unsigned int*[peNum];
      for ( unsigned int i = 0; i < peNum; i++ ) {
        mpClkDiv[i]  = new unsigned int;
        *mpClkDiv[i] = 1;
      }
      mPeNum = peNum;
    }
    else {
      sprintf( mErrMsg, "PE-number has been specified %d already.", mPeNum );
      printWarningMessage( mpModName, "setPeNum", mErrMsg );
    }
  }
}

/// <summary>set clock division</summary>
/// <param name="peNum">number of PE</param>
/// <param name="clkDiv">clock division</param>
/// <returns>none</returns>
void
MevAdptr::setClkDivision( unsigned int peid, unsigned int clkDiv )
{
  dmsgmv( mpModName << ".setClkDivision() peid:" << peid << " clkDiv:" << clkDiv );
  if ( 0 == mpClkDiv ) {
    sprintf( mErrMsg, "PE-number has not been specified." );
    printWarningMessage( mpModName, "setClkDivision", mErrMsg );
  }
  else if ( PE_MAX_NUM <= peid ) {
    sprintf( mErrMsg, "Invalid PEID(%d).", peid );
    printWarningMessage( mpModName, "setClkDivision", mErrMsg );
  }
  else {
    *mpClkDiv[peid] = clkDiv;
  }
}

/// <summary>set read-latency for clock division</summary>
/// <param name="latency">read-latency for clock division</param>
/// <returns>none</returns>
void
MevAdptr::setReadLatency( uint64 latency )
{
  dmsgmv( mpModName << ".setReadLatency() latency:" << latency );
  mRLatency = latency;
}

/// <summary>set write-latency for clock division</summary>
/// <param name="latency">write-latency for clock division</param>
/// <returns>none</returns>
void
MevAdptr::setWriteLatency( uint64 latency )
{
  dmsgmv( mpModName << ".setWriteLatency() latency:" << latency );
  mWLatency = latency;
}


// override functions
/// <summary>transport API for forward pass of AT</summary>
/// <param name="trans">transaction</param>
/// <param name="phase">TLM phase</param>
/// <param name="t">time</param>
/// <returns>synchronization information</returns>
TlmSyncEnum 
MevAdptr::nb_transport_fw(
  TlmTransactionType& trans,
  TlmPhase&           phase,
  sc_time&            t )
{
  dmsgmv( mpModName << ".nb_transport_fw() t:" << t << " addr:0x" << trans.get_address() );
  uint64 addr = trans.get_address() & 0xff;
  trans.set_address( addr );

  TlmG3mExtension* pG3mEx;
  trans.get_extension( pG3mEx );

  unsigned int peid = (unsigned int)(pG3mEx->getPeId());
  TlmSyncEnum  ret  = tlm::TLM_ACCEPTED;

  if ( mPeNum <= peid ) {
    sprintf( mErrMsg, "Invalid PEID(%d).", peid );
    printErrorMessage( mpModName, "nb_transport_fw", mErrMsg );
  }
  else {
    sc_time lt;
    if ( tlm::TLM_READ_COMMAND == trans.get_command() ) {
      lt = ( 2 < mRLatency ) ?
             (double)((*mpClkDiv[peid])*mRLatency-2)*getClkPeriod() : SC_ZERO_TIME;
      // "-2": arbitration=1clock, BusSlaveIf=1clock.
      dmsgmv( "  peid:" << peid << " *mpClkDiv:" << *mpClkDiv[peid] << " mRLatency:0x" << mRLatency << " lt:" << lt );
    }
    else if ( tlm::TLM_WRITE_COMMAND == trans.get_command() ) {
      lt = ( 2 < mWLatency ) ?
             (double)((*mpClkDiv[peid])*mWLatency-2)*getClkPeriod() : SC_ZERO_TIME;
      // "-2": arbitration=1clock, BusSlaveIf=1clock.
      dmsgmv( "  peid:" << peid << " *mpClkDiv:" << *mpClkDiv[peid] << " mWLatency:0x" << mWLatency << " lt:" << lt );
    }
    else {
      sprintf( mErrMsg, "Invalid transaction command(%d).", trans.get_command() );
      printErrorMessage( mpModName, "nb_transport_fw", mErrMsg );
    }

    if ( SC_ZERO_TIME == lt ) {
      ret = (*is)->nb_transport_fw( trans, phase, t );
    }
    else {
      t += lt;
      dmsgmv( "  PEID:" << peid << " t:" << t );
      ret = (*is)->nb_transport_fw( trans, phase, t );
    }
    dmsgmv( "  t:" << t << " addr:0x" << addr
            << " R:" << (tlm::TLM_READ_COMMAND==trans.get_command())
            << " W:" << (tlm::TLM_WRITE_COMMAND==trans.get_command()) );
  }

  return ret;
}

/// <summary>transport API for backward pass of AT</summary>
/// <param name="trans">transaction</param>
/// <param name="phase">TLM phase</param>
/// <param name="t">time</param>
/// <returns>synchronization information</returns>
TlmSyncEnum 
MevAdptr::nb_transport_bw(
  TlmTransactionType& trans,
  TlmPhase&           phase,
  sc_time&            t )
{
  dmsgmv( mpModName << ".nb_transport_bw() t:" << t << " addr:0x" << trans.get_address() );
  return (*ts)->nb_transport_bw( trans, phase, t );
}

/// <summary>transport API for LT</summary>
/// <param name="trans">transaction</param>
/// <param name="t">time</param>
/// <returns>none</returns>
void
MevAdptr::b_transport(
  TlmTransactionType& trans,
  sc_time&            t )
{
  dmsgmv( mpModName << ".b_transport() t:" << t << " addr:0x" << trans.get_address() );
  uint64 addr = trans.get_address() & 0xff;
  trans.set_address( addr );

  TlmG3mExtension* pG3mEx;
  trans.get_extension( pG3mEx );

  unsigned int peid = (unsigned int)(pG3mEx->getPeId());

  if ( mPeNum <= peid ) {
    sprintf( mErrMsg, "Invalid PEID(%d).", peid );
    printErrorMessage( mpModName, "b_transport", mErrMsg );
  }
  else {
    sc_time lt;
    if ( tlm::TLM_READ_COMMAND == trans.get_command() ) {
      lt = ( 2 < mRLatency ) ?
             (double)((*mpClkDiv[peid])*(mRLatency+1)-3)*getClkPeriod() : SC_ZERO_TIME;
      // "+1": add the clock to consume at master to BusSlabeIf
      // "-3": arbitration=1clock, BusSlaveIf=2clock.
      dmsgmv( "  peid:" << peid << " *mpClkDiv:" << *mpClkDiv[peid] << " mRLatency:0x" << mRLatency << " lt:" << lt );
    }
    else if ( tlm::TLM_WRITE_COMMAND == trans.get_command() ) {
      lt = ( 2 < mWLatency ) ?
             (double)((*mpClkDiv[peid])*(mWLatency+1)-3)*getClkPeriod() : SC_ZERO_TIME;
      // "+1": add the clock to consume at master to BusSlabeIf
      // "-3": arbitration=1clock, BusSlaveIf=2clock.
      dmsgmv( "  peid:" << peid << " *mpClkDiv:" << *mpClkDiv[peid] << " mWLatency:0x" << mWLatency << " lt:" << lt );
    }
    else {
      sprintf( mErrMsg, "Invalid transaction command(%d).", trans.get_command() );
      printErrorMessage( mpModName, "nb_transport_fw", mErrMsg );
    }

    if ( SC_ZERO_TIME == lt ) {
      (*is)->b_transport( trans, t );
    }
    else {
      t += lt;
      dmsgmv( "  PEID:" << peid << " t:" << t );
      (*is)->b_transport( trans, t );
    }
    dmsgmv( "b_transport() end  PEID:" << peid << " t:" << t << " addr:0x" << addr
            << " R:" << (tlm::TLM_READ_COMMAND==trans.get_command())
            << " W:" << (tlm::TLM_WRITE_COMMAND==trans.get_command()) );
  }
}

/// <summary>transport API for debug access</summary>
/// <param name="trans">transaction</param>
/// <returns>data number</returns>
unsigned int
MevAdptr::transport_dbg( TlmTransactionType& trans )
{
  dmsgmv( mpModName << ".transport_dbg() addr:0x" << trans.get_address() );
  uint64 addr = trans.get_address() & 0xff;
  trans.set_address( addr );

  dmsgmv( "  addr:0x" << addr
          << " R:" << (tlm::TLM_READ_COMMAND ==trans.get_command())
          << " W:" << (tlm::TLM_WRITE_COMMAND==trans.get_command()) );
  return( (*is)->transport_dbg( trans ) );
}

/// <summary>dummy</summary>
void
MevAdptr::invalidate_direct_mem_ptr(
  uint64 startRange,
  uint64 endRange )
{
  dmsgmv( mpModName << ".invalidate_direct_mem_ptr()" );
}

/// <summary>dummy</summary>
bool
MevAdptr::get_direct_mem_ptr(
  TlmTransactionType& trans,
  tlm::tlm_dmi&       dmiData )
{
  dmsgmv( mpModName << ".get_direct_mem_ptr()" );
  return false;
}


// private functions
/// <summary>get clock period</summary>
/// <param name="">none</param>
/// <returns>clock period</returns>
sc_time
MevAdptr::getClkPeriod( void )
{
  dmsgmv( mpModName << ".getClkPeriod()" );
  sc_time ret  = SC_ZERO_TIME;
  double  freq = (double)clk.read();

  if ( 0.0 == freq ) {
    sprintf( mErrMsg, "Clock frequency is 0." );
    printErrorMessage( mpModName, "getClkPeriod", mErrMsg );
  }
  else {
    ret = sc_time( 1.0/freq, SC_SEC );
  }

  return ret;
}
