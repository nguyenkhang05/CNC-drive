/*************************************************************************
 *
 * NSMVMEVV02.cpp
 *
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

#include <cstdio>
#include "Common.h"
#include "CmErrMsg.h"
#include "NSMVMEVV02.h"
#include "BACKWARD_DECODER.h"
#include "OSCI2DCDR.h"
#include "G4_MevAdptr.h"
#include "G4_MevArbtr.h"
#include "G4_G0Mev.h"
#include "g4_mev_def.h"
#include "g_reg.h"

using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  std::ofstream dmvfout;
  int NSMVMEVV02::countObj = 0;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <returns>none</returns>
NSMVMEVV02::NSMVMEVV02(
  sc_module_name name,
  const char*    pCfgFileName ):
    sc_module( name ),
    clk( "clk" ),
    resetz( "resetz" ),
    mpModName( 0 ),
    mPeNum( 1 ),
    mpMapName( 0 ),
    mpBwDec( 0 ),
    mpArb( 0 ),
    mpDec( 0 ),
    mpAdp( 0 ),
    mpG4_G0Mev( 0 ),
    mElbFlag( false )
{
#ifdef DEBUG_MESSAGE_OUTPUT
  if (NSMVMEVV02::countObj == 0) {
    dmvfopen("dmsg_mev.txt");
  }
  NSMVMEVV02::countObj ++;
#endif // DEBUG_MESSAGE_OUTPUT

  mpModName = this->name();
  dmsgmv( mpModName << ".NSMVMEVV02()" );
  char inst_name[128];

  readCfgFile( pCfgFileName );

  // create instance
  ts = new TlmTargetSocket<BUS_WIDTH_VCI32>( "ts" );

  mpBwDec = new BACKWARD_DECODER<BUS_WIDTH_VCI32>( "MevBwDec" );

  mpAdp = new G4_MevAdptr( "MevAdp" );
  mpAdp->setPeNum( mPeNum );

  if ( 0 != mpMapName ) {
    mpDec = new OSCI2DCDR<BUS_WIDTH_VCI32>( "MevDec", mpMapName );
  }
  else {
    sprintf( mErrMsg, "Map-file of MEV has not been specified." );
    printErrorMessage( mpModName, "NSMVMEVV02", mErrMsg );
    return;
  }

  mpArb = new G4_MevArbtr*[MEV_REG_NUM];
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    sprintf( inst_name, "MevArb%d", i );
    mpArb[i] = new G4_MevArbtr( inst_name );
    mpArb[i]->setPeNum( mPeNum );
    for ( unsigned int j = 0; j < mPeNum; j++ ) {
      mpArb[i]->setPriority( j, j );
    }
  }

  mpG4_G0Mev = new G4_G0Mev*[MEV_REG_NUM];
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    sprintf( inst_name, "G4_G0MEV%d", i );
    mpG4_G0Mev[i] = new G4_G0Mev( inst_name );
    mpG4_G0Mev[i]->tSocket32[0]->setBaseAddressSize( i*4, 4 );
    mpG4_G0Mev[i]->setRegAddr( i*4 );
    mpG4_G0Mev[i]->setRegAccSize( G_REG_SIZE_1BYTE ); // 1Byte
    mpG4_G0Mev[i]->setRegAccSize( G_REG_SIZE_2BYTE ); // 2Byte
    mpG4_G0Mev[i]->setRegAccSize( G_REG_SIZE_4BYTE ); // 4Byte
  }

  // connection
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    mpG4_G0Mev[i]->resetz( resetz );
    mpG4_G0Mev[i]->clk( clk ); // PE1 clock is fastest
    mpArb[i]->resetPort( resetz );
    mpArb[i]->freqPort( clk );
    mpArb[i]->is( *(mpG4_G0Mev[i]->ts) );
    mpDec->is( mpArb[i]->ts );
  }
  mpAdp->clk( clk );
  (*(mpAdp->is))( mpDec->ts );
  mpBwDec->is( *(mpAdp->ts) );
  (*ts)( mpBwDec->ts );

  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetz.neg();
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
NSMVMEVV02::~NSMVMEVV02( void )
{
  dmsgmv( mpModName << ".~NSMVMEVV02()" );
  delete ts;
  delete mpMapName;
  delete mpBwDec;
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    delete mpArb[i];
  }
  delete[] mpArb;
  delete mpDec;
  delete mpAdp;
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    delete mpG4_G0Mev[i];
  }
  delete[] mpG4_G0Mev;

#ifdef DEBUG_MESSAGE_OUTPUT
  NSMVMEVV02::countObj--;
  if (NSMVMEVV02::countObj == 0) {
    dmvfclose();
  }
#endif // DEBUG_MESSAGE_OUTPUT  
}


// set function
/// <summary>set clock division</summary>
/// <param name="peid">PEID</param>
/// <param name="clkDiv">clock division</param>
/// <returns>none</returns>
void
NSMVMEVV02::setClkDivision( unsigned int peid, unsigned int clkDiv )
{
  dmsgmv( mpModName << ".setClkDivision( 0x" << peid << ", " << clkDiv << " )" );
  if ( mPeNum <= peid ) {
    sprintf( mErrMsg, "Invalid PEID(%d).", peid );
    printWarningMessage( mpModName, "setClkDivision", mErrMsg );
  }
  else {
    mpAdp->setClkDivision( peid, clkDiv );
  }
}

/// <summary>set read-latency of register access</summary>
/// <param name="latency">read-latency of register access</param>
/// <returns>none</returns>
void
NSMVMEVV02::setReadRegLatency( uint64 latency )
{
  dmsgmv( mpModName << ".setReadRegLatency( 0x" << latency << " )" );
  if ( mElbFlag ) {
    double freq = (double)clk.read();
    if ( 0.0 == freq ) {
      sprintf( mErrMsg, "Clock frequency is 0." );
      printErrorMessage( mpModName, "setReadRegLatency", mErrMsg );
    }
    else {
      // do nothing
    }
  }
  else {
    // do nothing
  }

  mpAdp->setReadLatency( latency );
}

/// <summary>set write-latency of register access</summary>
/// <param name="latency">write-latency of register access</param>
/// <returns>none</returns>
void
NSMVMEVV02::setWriteRegLatency( uint64 latency )
{
  dmsgmv( mpModName << ".setWriteRegLatency( 0x" << latency << " )" );
  if ( mElbFlag ) {
    double freq = (double)clk.read();
    if ( 0.0 == freq ) {
      sprintf( mErrMsg, "Clock frequency is 0." );
      printErrorMessage( mpModName, "setWriteRegLatency", mErrMsg );
    }
    else {
      // do nothing
    }
  }
  else {
    // do nothing
  }

  mpAdp->setWriteLatency( latency );
}

/// <summary>set priority of arbitration</summary>
/// <param name="peid">PEID</param>
/// <param name="priority">priority of arbitration</param>
/// <returns>none</returns>
void
NSMVMEVV02::setPriority( unsigned int peid, unsigned int priority )
{
  dmsgmv( mpModName << ".setPriority()" );
  if ( mPeNum <= peid ) {
    sprintf( mErrMsg, "Invalid PEID(%d).", peid );
    printWarningMessage( mpModName, "setPriority", mErrMsg );
  }
  else {
    for ( int i = 0; i < MEV_REG_NUM; i++ ) {
      mpArb[i]->setPriority( peid, priority );
    }
  }
}

/// <summary>remove priority of arbitration</summary>
/// <param name="peid">PEID</param>
/// <returns>none</returns>
void
NSMVMEVV02::rmvPriority( unsigned int peid )
{
  dmsgmv( mpModName << ".rmvPriority()" );
  if ( mPeNum <= peid ) {
    sprintf( mErrMsg, "Invalid PEID(%d).", peid );
    printWarningMessage( mpModName, "rmvPriority", mErrMsg );
  }
  else {
    for ( int i = 0; i < MEV_REG_NUM; i++ ) {
      mpArb[i]->rmvPriority( peid );
    }
  }
}

/// <summary>clear priority of arbitration</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVMEVV02::clrPriority( void )
{
  dmsgmv( mpModName << ".clrPriority()" );
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    mpArb[i]->clrPriority();
  }
}

/// <summary>set access size of register</summary>
/// <param name="size">access size</param>
/// <returns>none</returns>
void
NSMVMEVV02::setRegAccSize( unsigned int size )
{
  dmsgmv( mpModName << ".setRegAccSize()" );
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    mpG4_G0Mev[i]->setRegAccSize( size );
  }
}

/// <summary>clear access size of register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVMEVV02::clrRegAccSize( void )
{
  dmsgmv( mpModName << ".clearRegAccSize()" );
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    mpG4_G0Mev[i]->clrRegAccSize();
  }
}


// public functions


// private functions
/// <summary>end of elaboration</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVMEVV02::end_of_elaboration( void )
{
  dmsgmv( mpModName << ".end_of_elaboration()" );
  mElbFlag = true;
}


/// <summary>set parameters from configuration file</summary>
/// <param name="filename">configuration file name</param>
/// <returns>none</returns>
void
NSMVMEVV02::readCfgFile( const char *filename )
{
  dmsgmv( mpModName << ".readCfgFile()" );
  char    word[512];
  char    seps[] = " ,=();\t\n\r";
  char*   token;

  ifstream  config_file(filename);
  // read the config file
  while( 1 ) {
    config_file.getline( word, 512, '\n' );
    token = strtok( word, seps );

    // ----------------------------------------
    // For EOF
    if ( config_file.eof() ) {
      break;
    }
    else { // not EOF
      // do nothing
    }

    // ----------------------------------------
    // For comment
    if ( (NULL == token) || (0 == strncmp(token, "//", 2)) ) {
      continue;
    }
    else { // not comment
      // do nothing
    }

    // ----------------------------------------
    // detect end marker
    // NOTICE:You have to check whether token is NULL first.
    if ( 0 == strcmp(token, "[END]") ) {
      break;
    }
    else { // not [END]
      // do nothing
    }

    // ----------------------------------------
    // get token
    // ----------------------------------------
    // For miscellaneous
    if ( 0 == strcmp(token, "[PE_NUM]") ) {
      token = strtok( 0, seps );
      if (token != NULL) {
        mPeNum = token_to_uint( token, "OrMev", "[PE_NUM]" );
        dmsgmv( "  mPeNum:0x" << mPeNum );
        char msg[128];
        if ( mPeNum > PE_MAX_NUM ) {
          sprintf( msg,
                   "[PE_NUM] must be under %d, but specified (%d).",
                   PE_MAX_NUM+1, mPeNum);
          printErrorMessage( mpModName, "readCfgFile", msg );
          exit( 1 );
        }
        else if ( 0 == mPeNum ) {
          sprintf( msg, "[PE_NUM] cannot be specified 0." );
          printErrorMessage( mpModName, "readCfgFile", msg );
          exit( 1 );
        }
        else { // mPeNum is under PE_MAX_NUM
          // do nothing
        }
      }
      else { // token is NULL
        // do nothing
      }
      continue;
    }
    else { // token is not [PE_NUM]
      // do nothing
    }
    // MEV map file name
    if( strcmp( token, "[MEV_MAP_FILE]" ) == 0 ) {
      token = strtok(0, seps);
      const char *fname = (!token)? "" : token;
      mpMapName = new char[128];
      strcpy( mpMapName, fname );
      continue;
    }

  }  // end of configuration file read loop(infinite)
}  // end of readCfgFile()


/// <summary>reset function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVMEVV02::resetFunc( void )
{
  dmsgmv( mpModName << ".resetFunc()" );
  for ( int i = 0; i < MEV_REG_NUM; i++ ) {
    mpG4_G0Mev[i]->resetFunc();
  }
}


// processes
/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVMEVV02::ResetProc( void )
{
  dmsgmv( mpModName << ".ResetProc()" );
  resetFunc();
}
