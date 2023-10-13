/*************************************************************************
 *
 * NSMVIPIRV02.cpp
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
#include "NSMVIPIRV02.h"
#include "G4_IpirFunc.h"
#include "Common.h"
#include "CmErrMsg.h"

using namespace std;
using namespace CmErrMsg;

// for debug output
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  extern std::ofstream difout;
#endif // DEBUG_MESSAGE_OUTPUT


/// <summary>constructor</summary>
/// <param name="name">module name</param>
/// <param name="pCfgFileName">configuration file name</param>
/// <returns>none</returns>
NSMVIPIRV02::NSMVIPIRV02( 
  sc_module_name name,
  const char*    pCfgFileName ):
    sc_module( name ),
    BusSlaveBase<BUS_WIDTH_APB, 1>(),
    BusTimeBase(),
    resetz( "resetz" ),
    clk( "clk" ),
    mpModName( 0 ),
    mpIpirFunc( 0 ),
    mPeNum( 1 ),
    mPulseWidth( 1.0 ),
    mDetectType( DETECT_TYPE_EDGE ),
    mDebugFlag( false )
{
  mpModName = this->name();
  dmsgsc( mpModName << ".NSMVIPIRV02()" );

  readCfgFile( pCfgFileName );
  setSlaveResetPort32( &resetz );
  setSlaveFreqPort32( &clk );
  setTargetSocket32( "ts" );

  ts = tSocket32[0];

  char inst_name[128];
  sprintf( inst_name, "%s.G4_IpirFunc", mpModName );
  mpIpirFunc = new G4_IpirFunc( inst_name );
  mpIpirFunc->setIpirPtr( this );

  mBusSlaveIf32[0]->setFuncModulePtr( mpIpirFunc );
  mBusSlaveIf32[0]->setBusProtocol( BUS_APB );
  mBusSlaveIf32[0]->setTransNmbLmt( 1 );
  mBusSlaveIf32[0]->setReadLatency( 1 );
  mBusSlaveIf32[0]->setWriteLatency( 1 );

  // creat the instances of ports
  for ( int j = 0; j < PE_MAX_NUM; j++ ) {
    for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
      sprintf( inst_name, "ipir_int_pe%d_ch%d", j, i );
      ipir_int_pe_ch[j][i] = new sc_out<bool>( inst_name );
    }
  }

  // initialize process
  SC_METHOD( InitProc );

  // reset process
  SC_METHOD( ResetProc );
  sensitive << resetz.neg();
  dont_initialize();

  // EIINT request process
  SC_THREAD( IntReqProc );
}

/// <summary>destructor</summary>
/// <param name="">none</param>
/// <returns>none</returns>
NSMVIPIRV02::~NSMVIPIRV02( void )
{
  dmsgsc( mpModName << ".~NSMVIPIRV02()" );
  delete mpIpirFunc;
  for ( int j = 0; j < PE_MAX_NUM; j++ ) {
    for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
      delete ipir_int_pe_ch[j][i];
    }
  }
}

/// <summary>set detection type as edge-detection</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::setDetectTypeEdge( void )
{
  dmsgsc( mpModName << ".setDetectTypeEdge()" );
  mDetectType = DETECT_TYPE_EDGE;
}

/// <summary>set detection type as edge-detection</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::setDetectTypeLevel( void )
{
  dmsgsc( mpModName << ".setDetectTypeLevel()" );
  mDetectType = DETECT_TYPE_LEVEL;
}

/// <summary>set access size of register</summary>
/// <param name="ch">channel of register</param>
/// <param name="size">access size</param>
/// <returns>none</returns>
void
NSMVIPIRV02::setRegAccSize(
  unsigned int ch,
  unsigned int size )
{
  dmsgsc( mpModName << ".setRegAccSize()" );
  switch ( size ) {
  case 1: (mpIpirFunc->getRegPtr("IPIEN", ch))->SetAccessSize(G_REG_SIZE_1BYTE);
          (mpIpirFunc->getRegPtr("IPIFLG", ch))->SetAccessSize(G_REG_SIZE_1BYTE);
          (mpIpirFunc->getRegPtr("IPIFCLR", ch))->SetAccessSize(G_REG_SIZE_1BYTE);
          (mpIpirFunc->getRegPtr("IPIREQ", ch))->SetAccessSize(G_REG_SIZE_1BYTE);
          (mpIpirFunc->getRegPtr("IPIRCLR", ch))->SetAccessSize(G_REG_SIZE_1BYTE);
          break;
  case 2: (mpIpirFunc->getRegPtr("IPIEN", ch))->SetAccessSize(G_REG_SIZE_2BYTE);
          (mpIpirFunc->getRegPtr("IPIFLG", ch))->SetAccessSize(G_REG_SIZE_2BYTE);
          (mpIpirFunc->getRegPtr("IPIFCLR", ch))->SetAccessSize(G_REG_SIZE_2BYTE);
          (mpIpirFunc->getRegPtr("IPIREQ", ch))->SetAccessSize(G_REG_SIZE_2BYTE);
          (mpIpirFunc->getRegPtr("IPIRCLR", ch))->SetAccessSize(G_REG_SIZE_2BYTE);
          break;
  case 4: (mpIpirFunc->getRegPtr("IPIEN", ch))->SetAccessSize(G_REG_SIZE_4BYTE);
          (mpIpirFunc->getRegPtr("IPIFLG", ch))->SetAccessSize(G_REG_SIZE_4BYTE);
          (mpIpirFunc->getRegPtr("IPIFCLR", ch))->SetAccessSize(G_REG_SIZE_4BYTE);
          (mpIpirFunc->getRegPtr("IPIREQ", ch))->SetAccessSize(G_REG_SIZE_4BYTE);
          (mpIpirFunc->getRegPtr("IPIRCLR", ch))->SetAccessSize(G_REG_SIZE_4BYTE);
          break;
  default:
    sprintf( mErrMsg, "Invalid access size(%d).", size );
    printErrorMessage( mpModName, "setRegAccSize", mErrMsg );
    break;
  }
}

/// <summary>clear access size of register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::clrRegAccSize( unsigned int ch )
{
  dmsgsc( mpModName << ".clrRegAccSize()" );
  (mpIpirFunc->getRegPtr("IPIEN", ch))->ClearAccessSize();
  (mpIpirFunc->getRegPtr("IPIFLG", ch))->ClearAccessSize();
  (mpIpirFunc->getRegPtr("IPIFCLR", ch))->ClearAccessSize();
  (mpIpirFunc->getRegPtr("IPIREQ", ch))->ClearAccessSize();
  (mpIpirFunc->getRegPtr("IPIRCLR", ch))->ClearAccessSize();
}


// public functions


// private functions
/// <summary>set parameters from configuration file</summary>
/// <param name="filename">configuration file name</param>
/// <returns>none</returns>
void
NSMVIPIRV02::readCfgFile( const char *filename )
{
  dmsgsc( mpModName << ".readCfgFile()" );
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
        mPeNum = token_to_uint( token, "OrIpir", "[PE_NUM]" );
        dmsgsc( "  mPeNum:0x" << mPeNum );
        char msg[1024];
        if ( mPeNum > PE_MAX_NUM ) {
          sprintf( msg,
                   "[PE_NUM] must be under %d, but specified (%d).",
                   PE_MAX_NUM+1, mPeNum );
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
  }  // end of configuration file read loop(infinite)
}  // end of readCfgFile()


/// <summary>set write-information to list for edge-deection type</summary>
/// <param name="t">pointer to write-information</param>
/// <returns>none</returns>
void
NSMVIPIRV02::setWriteInfo( G4_IpirRegWriteInfo* pInfo )
{
  dmsgsc( mpModName << ".G4_IpirRegWriteInfo()" );

  if ( resetz.read() ) { // not reset
    mRegWInfoLst.push_back( *pInfo );
  }
  else { // reset
    // do nothing
  }
}

/// <summary>request eiint to INTC1</summary>
/// <param name="t">pointer to bus time</param>
/// <returns>none</returns>
void
NSMVIPIRV02::reqInt(
  BusTime_t* pTime,
  bool       debug )
{
  dmsgsc( mpModName << ".reqInt() *pTime:0x" << *pTime << " debug:" << debug );

  if ( resetz.read() ) { // not reset
    if ( DETECT_TYPE_EDGE == mDetectType ) { // edge detection
      sc_time pulse_time( mPulseWidth*getClkWidth(), SC_SEC );
      *pTime += pulse_time;
    }
    else { // high level detection
      // do nothing
    }
    mDebugFlag = debug;

    meIntReq.notify();
  }
  else { // reset
    // do nothing
  }
}


// private functions
/// <summary>reset function</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::resetFunc( void )
{
  dmsgsc( mpModName << ".resetFunc()" );
  mDebugFlag = false;
  mRegWInfoLst.clear();
  mpIpirFunc->reset();
  clearIntPort();
}

/// <summary>set ipir_int_pe_ch port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::setIntPort( void )
{
  dmsgsc( mpModName << ".setIntPort()" );
  unsigned int regData = 0;
  bool         regBit  = false;

  if ( resetz.read() ) { // not reset
    for ( unsigned int j = 0; j < mPeNum; j++ ) {
      for ( unsigned int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
        regData = mpIpirFunc->readReg8( i*4, true );
        regBit  = ( 0x1 == ((regData >> j) & 0x1) );
        dmsgsc("  ch:" << i << " regBit:" << regBit
               << " ipir_int_pe_ch:" << ipir_int_pe_ch[j][i]->read());
        if ( ((DETECT_TYPE_LEVEL == mDetectType) ||
              ((DETECT_TYPE_EDGE == mDetectType) && (0 != regData))) &&
             (ipir_int_pe_ch[j][i]->read() != regBit) ) {
          ipir_int_pe_ch[j][i]->write( regBit );
        }
        else {
          // do nothing
        }
      }
    }
  }
  else { // reset
    // do nothing
  }
}

/// <summary>clear ipir_int_pe_ch port</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::clearIntPort( void )
{
  dmsgsc( mpModName << ".clearIntPort()" );
  for ( unsigned int j = 0; j < mPeNum; j++ ) {
    for ( unsigned int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
      ipir_int_pe_ch[j][i]->write( false );
    }
  }
}

/// <summary>clear register</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::clearReg( void )
{
  dmsgsc( mpModName << ".clearReg()" );
  unsigned char zeroData[2] = { 0, 0 };

  for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
    mpIpirFunc->writeReg( i*4, zeroData, 2, true );
  }
}

/// <summary>to get clock width</summary>
/// <param name="">none</param>
/// <returns>clock width(sec)</returns>
double
NSMVIPIRV02::getClkWidth( void )
{
  dmsgsc( mpModName << ".getClkWidth()" );
  double clkw = 0.0;
  double freq = (double)clk.read();
  if ( 0.0 == freq ) {
    sprintf( mErrMsg, "Clock frequency is 0." );
    printErrorMessage( mpModName, "getClkWidth", mErrMsg );
  }
  else {
    clkw = 1.0 / freq;
  }

  dmsgsc( "  clkw:" << clkw << " sec" );
  return clkw;
}


// processes
/// <summary>initialization process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::InitProc( void )
{
  dmsgsc( mpModName << ".InitProc()" );
  resetFunc();
  for ( int i = 0; i < INTC_IPIR_CH_NUM; i++ ) {
    dmsgsc("  ch:" << i << " regdata:" << mpIpirFunc->readReg8( i, true ));
  }
}

/// <summary>reset process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::ResetProc( void )
{
  dmsgsc( mpModName << ".ResetProc()" );
  resetFunc();
}

/// <summary>EIINT(to INTC1) request process</summary>
/// <param name="">none</param>
/// <returns>none</returns>
void
NSMVIPIRV02::IntReqProc( void )
{
  while ( 1 ) {
    wait( meIntReq ); // wait event of register-write
    dmsgsc( mpModName << ".IntReqProc() is activated." ); 

    if ( resetz.read() ) { // not reset

      /***** set data to register(normal-write only) *****/
      if ( !mDebugFlag && !mRegWInfoLst.empty() ) {
        G4_IpirRegWriteInfo rInf = mRegWInfoLst.front();
        mpIpirFunc->writeReg( rInf.mAddr, rInf.mData, rInf.mSize, false );
      }
      else {
        mDebugFlag = false;
      }

      /***** set data to port *****/
      setIntPort();

      /***** wait pulse-width of EIINT, then clear register and port(edge-detection only) *****/
      if ( DETECT_TYPE_EDGE == mDetectType ) { // edge detection
        if ( elapseTime( mPulseWidth*getClkWidth(), SC_SEC ) ) { // reset is active
          dmsgsc("  reset signal is activated in elapseTime()");
          continue;
        }
        else { // not reset
          // do nothing
        }
        clearIntPort();
        clearReg();
      }
      else { // high level detection
        // do nothing
      }

      dmsgsc("  mRegWInfoLst.size():" << mRegWInfoLst.size());
      if ( 1 == mRegWInfoLst.size() ) {
        mRegWInfoLst.pop_front();
      }
      else if ( !mRegWInfoLst.empty() ) {
        mRegWInfoLst.pop_front();
        meIntReq.notify( sc_time(getClkWidth(), SC_NS) );
      }
      else {
        // do nothing
      }

    }
    else { // reset is active
      // do nothing
    }
  }
}
