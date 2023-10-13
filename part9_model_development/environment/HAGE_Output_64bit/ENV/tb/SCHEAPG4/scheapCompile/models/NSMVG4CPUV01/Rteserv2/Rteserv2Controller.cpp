/*
 * Copyright(c) 2013-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */


// for debug
#ifdef DEBUG_MESSAGE_OUTPUT
# include <fstream>
  std::ofstream dmfout;
#endif // DEBUG_MESSAGE_OUTPUT

#include "PFRH850.hpp"
#include "G4_CaIss.h" 
#include "NSMVG4CPUV01.h"
#include "G4_NoDbgIF.h"
//#include "FastIssIF.h" // for FastISS
#include "dbg_typ.h"
#include "dbg_def.h"
#include "SysRegInfo.h"
#include "Rteserv2IF.h"
#include "Rteserv2Controller.h"
#include "forest_utility_api.h"

#ifdef WINDOWS_DEF
#  include <WinSock2.h>
#  include <ws2tcpip.h>
#  include <process.h>
#endif // WINDOWS_DEF

#ifdef LINUX_DEF
extern "C" {
#  include <sys/types.h>  /* basic system data types */
#  include <sys/socket.h> /* basic socket definitions */
#  include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#  include <arpa/inet.h>
#  include <errno.h>
}
#endif // LINUX_DEF

extern uint64       glb_freq;
extern int          scstopDoneFlag;
extern unsigned int token_to_uint( char*, const char*, const char* );
extern PFRH850*     PFRH850_0;

using namespace dbgTyp;

namespace PltfrmGnd
{
  extern sc_signal<bool>* reset_sig;
}


///<summary>constructor</summary>
///<param name="pCfgFileName">name of configuration file</param>
///<param name="pG4CPU">pointer to NSMVG4CPUV01</param>
///<returns>none</returns>
Rteserv2Controller::Rteserv2Controller( 
  const char*    pCfgFileName,
  NSMVG4CPUV01* pG4CPU ):
    G4_NoDbgIF( pCfgFileName, pG4CPU ),
    mMultiPort( 0 ),
    mSockFd( 0 ),
    mpG4CPU( pG4CPU ),
    mpRteserv2If( 0 ),
    mRteserv2IFNum( 0 ),
    mStopAnyCoreFlag( false ),
    mPeripheralFlag( false ),
    mRunningFlag( false ),
    mLastRunPeid( 0 ),
    mpSysRegInfo( 0 )
{
  dfopen("dmsg_mult.txt");
  dmsg("Rteserv2Controller::Rteserv2Controller");
  // get the parameters of configuration
  readCfgFile( pCfgFileName );

  // socket initialization
  initSocket();

  // get the pointer to the break-information
  if ( 0 != mpForestUtil ) {
    mpBreakInfoCA = mpForestUtil->GetBreakInfo();
    if ( 0 == mpBreakInfoCA ) {
      ErrMsg("Rteserv2Controller", "Rteserv2Controller", "mpBreakInfoCA is 0.");
    }
  }
  else {
    ErrMsg("Rteserv2Controller", "Rteserv2Controller", "ForestUtil pointer is 0.");
  }
/* for FastISS
  mpFastIssIf = pG4CPU->GetFastIssPtr();
  if ( 0 != mpFastIssIf ) {
    mpBreakInfoFast = mpFastIssIf->getBreakInfo();
    if ( 0 == mpBreakInfoFast ) {
      ErrMsg("Rteserv2Controller", "Rteserv2Controller", "mpBreakInfoFast is 0.");
    }
  }
  else {
    ErrMsg("Rteserv2Controller", "Rteserv2Controller", "FastISS pointer is 0.");
  }
*/

  // set the information of system-register
  mpSysRegInfo = new SysRegInfo();
  setSysRegInfo();

  // Rteserv2IF generation
  createRteserv2If();
}



///<summary>destructor</summary>
///<param name="none"></param>
///<returns>none</returns>
Rteserv2Controller::~Rteserv2Controller( void )
{
  dfclose();
  delete [] mpRteserv2If;
  delete mpSysRegInfo;
}



///<summary>start function of debugger-control</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::start( void )
{
  dmsg("Rteserv2Controller::start");
  ctrlSim();
}



///<summary>function to return the pointer to NSMVG4CPUV01</summary>
///<param name="none"></param>
///<returns>pointer to NSMVG4CPUV01</returns>
NSMVG4CPUV01*
Rteserv2Controller::getG4CpuPtr( void )
{
  return mpG4CPU;
}



///<summary>fonction to return the pointer to ForestUtil</summary>
///<param name="none"></param>
///<returns>pointer to ForestUtil</returns>
ForestUtilApi*
Rteserv2Controller::getForestUtilPtr( void )
{
  return mpForestUtil;
}



/* For FastISS
FastIssIF*
Rteserv2Controller::getFastIssPtr( void )
{
  return mpFastIssIf;
}
*/



#ifdef LINUX_DEF
///<summary>function to transrate to the struct of network-address-family</summary>
///<param name="family">address-family</param>
///<param name="pStr">pointer to the strings of IP-address</param>
///<param name="pAddr">pointer to the struct of network-address</param>
///<returns>1(OK)/0(INVALID)/-1(ERROR)</returns>
int
Rteserv2Controller::inet_pton(
  int         family, 
  const char* pStr, 
  void*       pAddr )
{
  int ret = 0;
  if ( AF_INET == family ) {
    struct in_addr  in_val;
    if ( inet_aton(pStr, &in_val) )  {
      memcpy(pAddr, &in_val, sizeof(struct in_addr));
      ret = 1;
    }
    else {
      ret = 0;
    }
  }
  else {
    errno = EAFNOSUPPORT;
    ret   = -1;
  }

  return ret;
}
#endif // LINUX_DEF



#ifdef WINDOWS_DEF
///<summary>function to transrate to the struct of network-address-family</summary>
///<param name="family">address-family</param>
///<param name="pStr">pointer to the strings of IP-address</param>
///<param name="pAddr">pointer to the struct of network-address</param>
///<returns>1(OK)/0(INVALID)/-1(ERROR)</returns>
int
Rteserv2Controller::inet_pton(
  int         family, 
  const char* pStr, 
  ULONG*      pAddr )
{
  int ret = 0;
  if ( AF_INET == family ) {
    unsigned int in_val = inet_addr( pStr ); 
    if ( INADDR_NONE != in_val ) {
       *(pAddr) = in_val;
       ret = 1;
    }
    else {
      ret = 0;
    }
  }
  else {
    WSASetLastError( WSAEAFNOSUPPORT );
    ret = -1;
  }

  return ret;
}
#endif // WINDOWS_DEF



///<summary>function to set the initial parameter for socket communication</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::initSocket( void )
{
  dmsg("Rteserv2Controller::initSocket");
  int ret = 0;

#ifdef WINDOWS_DEF
  WSADATA data;
  ret = WSAStartup( MAKEWORD(2,0), &data );
  dmsg("  WSAStartup:" << ret);
#endif //WINDOWS_DEF

  struct sockaddr_in srvAddr;
  srvAddr.sin_port = htons( mMultiPort );
  mSockFd           = socket( AF_INET, SOCK_STREAM, 0 );
  dmsg("  mSockFd:" << mSockFd);

  // Fillout the Input structure to the connect
  srvAddr.sin_family = AF_INET;

#ifdef LINUX_DEF
  ret = inet_pton( AF_INET, mMultiHostAddr, &srvAddr.sin_addr );
#endif // LINUX_DEF
#ifdef WINDOWS_DEF
  ret = inet_pton( AF_INET, mMultiHostAddr, &srvAddr.sin_addr.s_addr );
#endif // WINDOWS_DEF
  dmsg("  inet_pton:" << ret);

  struct timeval waitval;
  fd_set readfds;

  waitval.tv_sec  = 2;
  waitval.tv_usec = 0;

  FD_ZERO( &readfds );
  FD_SET ( mSockFd, &readfds );

  ret = select ( 0, &readfds, 0, 0, &waitval );
  dmsg("  select:" << ret);

  // Connect the socket
  ret = connect( mSockFd, (SA*)(&srvAddr), sizeof(srvAddr) );
  dmsg("  connect:" << ret);
}



///<summary>fonction to get the parameters of configuration</summary>
///<param name="pCfgFileName">name of configuration file</param>
///<returns>none</returns>
void
Rteserv2Controller::readCfgFile( const char* pCfgFileName )
{
  dmsg("Rteserv2Controller::readCfgFile");
  char  word[512];
  char  seps[] = " ,=();\t\n\r";
  char* token;

  ifstream  cfgFile( pCfgFileName );
  // read the config file
  while(1) {
    cfgFile.getline( word, 512, '\n' );
    token = strtok( word, seps );

    // ----------------------------------------
    // For EOF
    if ( cfgFile.eof() ) {
      break;
    }

    // ----------------------------------------
    // For comment
    if ( (NULL == token) || (0 == strncmp(token, "//", 2)) ) {
      continue;
    }

    // ----------------------------------------
    // detect end marker
    if ( 0 == strcmp(token, "[END]") ) {
      break;
    }

    // ----------------------------------------
    // get token
    // ----------------------------------------
    // For Socket Connection
    if ( 0 == strcmp( token, "[V850E2R_MULTI]" ) ) {
      token = strtok( 0, seps );
      if ( token != NULL ) {
        strcpy( mMultiHostAddr, token );
      }
      else {
        strcpy( mMultiHostAddr, "" );
      }
      continue;
    }

    if ( 0 == strcmp( token, "[V850E2R_MULTI_PORT]" ) ) {
      token = strtok( 0, seps );
      if ( NULL != token ) {
        mMultiPort = token_to_uint( token,
                                    "Rteserv2Controller",
                                    "[V850E2R_MULTI_PORT]" );
      }
      continue;
    }

    // For reset enable
    if ( 0 == strcmp( token, "[PERIPHERAL]" ) ) {
      token = strtok( 0, seps );
      if ( NULL != token ) {
        if ( 0 == strcmp(token, "PFRH850") ) {
          mPeripheralFlag = true;
        }
        else {
          mPeripheralFlag = false;
        }
      }
      continue;
    }


  }  // end of configuration file read loop(infinite)
}



///<summary>function to get the command from rteserv2</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::receiveCommand( void )
{
  dmsg("Rteserv2Controller::receiveCommand");
  char         rcvCharCmd = CMD_NULL;
  int          readCnt    = 0;
  char*        pCmdRcv    = mCmdRcv;
  unsigned int code       = 0;
  unsigned int id         = 0;

  while( 1 ) {
    readCnt = recv( mSockFd, &rcvCharCmd, sizeof(char), 0 );

    if ( (0 != readCnt) && (CMD_NULL != rcvCharCmd) ) {
      *pCmdRcv = rcvCharCmd;
      pCmdRcv++;
    }
    else {
      break;
    }

    if ( '\n' == rcvCharCmd ) {
      *pCmdRcv = CMD_NULL;
      if ( (('1'==mCmdRcv[0]) && (('0'==mCmdRcv[1])||('5'==mCmdRcv[1]))) ||
           (('2'==mCmdRcv[0]) && ('3'==mCmdRcv[1])) ) {
           // 10:HALT, 15:QUIT, 23:RESET
        sscanf( mCmdRcv, "%d ", &code );
        id = 0;
      }
      else {
        sscanf( mCmdRcv, "%d %d ", &code, &id );
        unsigned int  peid = RTE_PEID( id );
        unsigned int  vcid = RTE_VCID( id );
        unsigned int  tcid = RTE_TCID( id );
        char msg[32];
        if ( peid > mPeNum ) {
          sprintf(msg, "PEID(%d) is over the configured PE number(%d).", peid, mPeNum);
          ErrMsg("Rteserv2Controller", "receiveCommand", msg);
        }
        if ( vcid > 0 ) {
          sprintf(msg, "VCID(%d) is over the configured VM number(%d).", vcid, 0);
          ErrMsg("Rteserv2Controller", "receiveCommand", msg);
        }
        if ( tcid > 0 ) {
          sprintf(msg, "TCID(%d) is over the configured HT number(%d).", tcid, 0);
          ErrMsg("Rteserv2Controller", "receiveCommand", msg);
        }
      }
      break;
    }
  }
}



///<summary>function to process the command from rteserv2</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::processCommand( void )
{
  dmsg("Rteserv2Controller::processCommand  mCmdRcv:" << mCmdRcv);
  int cmdTyp = 0;

  if( CMD_NULL == mCmdRcv ) {
    return;
  }

  // Get the command code
  sscanf( mCmdRcv, "%d", &cmdTyp );
    
  // Parse the commands and get the additional parameters.
  switch( cmdTyp )
  {
    case RTE_CMD_HALT:          exHalt();          break;
    case RTE_CMD_QUIT:          exQuit();          break;
    case RTE_CMD_RESET:         exReset();         break;
    case RTE_CMD_READMEM:       exReadMem();       break;
    case RTE_CMD_WRITEMEM:      exWriteMem();      break;
    case RTE_CMD_READBLOCKMEM:  exReadBlockMem();  break;
    case RTE_CMD_WRITEBLOCKMEM: exWriteBlockMem(); break;
    case RTE_CMD_CHECKSTAT:     exCheckStat();     break;
    case RTE_CMD_READREG:       exReadReg();       break;
    case RTE_CMD_WRITEREG:      exWriteReg();      break;
    case RTE_CMD_SINGLESTEP:    exSingleStep();    break;
    case RTE_CMD_SETBP:         exSetPcBp();       break;
    case RTE_CMD_REMOVEBP:      exRemovePcBp();    break;
    case RTE_CMD_SETHWBP:       exSetHwBp();       break;
    case RTE_CMD_REMOVEHWBP:    exRemoveHwBp();    break;
    case RTE_CMD_REMOVEALLHWBP: exRemoveAllHwBp(); break;
    case RTE_CMD_INFOHWBP:      exInfoHwBp();      break;
    case RTE_CMD_CONT:          exCont();          break;
    default:                    break;
  }

  mCmdRcv[0] = CMD_NULL;
}



///<summary>fonction to respond to rteserv2</summary>
///<param name="none"></parwddam>
///<returns>none</returns>
void
Rteserv2Controller::sendResponse( void )
{
  dmsg("Rteserv2Controller::sendResponse");
#ifdef LINUX_DEF
  ssize_t n;
#endif // LINUX_DEF
#ifdef WINDOWS_DEF
  int     n;
#endif // WINDOWS_DEF
  int     nwritten;
  char*   pCmdRsp = mCmdRsp;

  // Check if the command response buffer is NULL
  if ( CMD_NULL != *pCmdRsp ) {
    // Get the length of the command response
    n = strlen( pCmdRsp );

    // Write the command to the socket
    nwritten = send( mSockFd, pCmdRsp, n, 0 );
    dmsg("  pCmdRsp: " << pCmdRsp);
  
    // Reinitialise Command response buffer to NULL
    pCmdRsp    = mCmdRsp;
    *pCmdRsp   = CMD_NULL;
  }
  else {
    // do nothing
  }
}



///<summary>fonction to geneate the instances of Rteserv2IF</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::createRteserv2If( void )
{
  dmsg("Rteserv2Controller::createRteserv2If  mPeNum:" << mPeNum);
  mRteserv2IFNum = mPeNum; // initial value is the number of native-thread(=number of PE)
  // the native thread is generated at the same time of PE generation by SetCore() in CForest
  for ( unsigned int i = 0; i < mPeNum; i++ ) {
#ifdef NATIVE_MODE_ENABLE
    if ( mNativeModePeId.end() == mNativeModePeId.find( i+1 ) ) {
      // case of normal mode(!=native mode) on PE
      mRteserv2IFNum += 0;
    }
#else  // NATIVE_MODE_ENABLE
    mRteserv2IFNum += 1;
#endif // NATIVE_MODE_ENABLE
    dmsg("  mpTcNum[" << i << "]:" << 0 << "  mpVcNum[" << i << "]:" << 0);
  }
  dmsg("  mRteserv2IFNum: " << mRteserv2IFNum);
  mpRteserv2If = new Rteserv2IF[mRteserv2IFNum];

  /* example of ifid(sequential number of Rteserv2IF)
     ・case of PE1: NT only、 PE2: NT+HTx1、PE3: NT+HTx2
       ---------------------------------------
       PE1  NT(0)    PE2  NT(1)    PE3  NT(3)
                          HT(2)         HT(4)
                                        HT(5)
       ---------------------------------------
       ※The number of "(x)" is ifid. ifid is also used in getRteserv2IfPtr() */
  unsigned int ifid = 0;
#ifdef NATIVE_MODE_ENABLE
  for ( unsigned int pe = 0; pe < mPeNum; pe++ ) {
    if ( mNativeModePeId.end() != mNativeModePeId.find( pe+1 ) ) {
      // case of native mode
      (mpRteserv2If[ifid]).setRteserv2ControllerPtr( this );
      (mpRteserv2If[ifid]).setCpuInfo( pe+1, -1 );
      ifid++;
    }
    else { // case of normal mode(!=native mode)
      for ( unsigned int ht = 0; ht < 1; ht++, ifid++ ) {
        (mpRteserv2If[ifid]).setRteserv2ControllerPtr( this );
        if ( 0 == ht ) { // NC
          (mpRteserv2If[ifid]).setCpuInfo( pe+1, -1 );
        }
        else { // PEID = 1,2,3...、TCID = 0,1,2 ...
          (mpRteserv2If[ifid]).setCpuInfo( pe+1, ht-1 );
        }
      }
    }
  }
  if ( (mPeNum-1) > ifid ) {
    ErrMsg("Rteserv2Controller", "createRteserv2If", "Setting of Rteserv2IF is failed.");
  }
#else  // NATIVE_MODE_ENABLE
  for ( unsigned int pe = 0; pe < mPeNum; pe++ ) {
    for ( unsigned int ht = 0; ht < 1; ht++, ifid++ ) {
      (mpRteserv2If[ifid]).setRteserv2ControllerPtr( this );
      if ( 0 == ht ) { // NC
        (mpRteserv2If[ifid]).setCpuInfo( pe+1, -1);
      }
      else { // PEID = 1,2,3...、TCID = 0,1,2 ...
        (mpRteserv2If[ifid]).setCpuInfo( pe+1, ht-1  );
      }
    }
  }
  if ( (mPeNum-1) > ifid ) {
    ErrMsg("Rteserv2Controller", "createRteserv2If", "Setting of Rteserv2IF is failed.");
  }
#endif // NATIVE_MODE_ENABLE
}



///<summary>function to get the pointer to Rteserv2IF</summary>
///<param name="cpuId">CPU ID(concatenated peid, vcid, tcid, v/n)</param>
///<returns>pointer to Rteserv2IF</returns>
Rteserv2IF*
Rteserv2Controller::getRteserv2IfPtr( unsigned int cpuId )
{
  dmsg("Rteserv2Controller::getRteserv2IfPtr");
  unsigned int peid   = RTE_PEID( cpuId );
  unsigned int tcid   = RTE_TCID( cpuId );
  bool         vnFlag = RTE_VN( cpuId );
  Rteserv2IF*  ret    = 0;
  unsigned int ifid   = 0;

  /* example of ifid(sequential number of Rteserv2IF)
     ・case of PE1: NT only、 PE2: NT+HTx1、PE3: NT+HTx2
       ---------------------------------------
       PE1  NT(0)    PE2  NT(1)    PE3  NT(3)
                          HT(2)         HT(4)
                                        HT(5)
       ---------------------------------------
       ※The number of "(x)" is ifid. ifid is also used in getRteserv2IfPtr() */
  ifid = 0;
  unsigned int i;
#ifdef NATIVE_MODE_ENABLE
  for ( i = 0; i < peid-1; i++ ) {
    if ( mNativeModePeId.end() != mNativeModePeId.find( i+1 ) ) { // case of native mode
      ifid++;
    }
    else { // case of normal mode(!=native mode)
      ifid += (1); // '+1' is NC
    }
  }
  if ( mNativeModePeId.end() == mNativeModePeId.find( i+1 ) ) { // case of 'not' native mode
    ifid += (vnFlag) ? (tcid + 1) : 0; // '+1' is for tcid(0,1,2 ...)
  }
#else // NATIVE_MODE_ENABLE
  for ( i = 0; i < peid-1; i++ ) {
    ifid += (1); // '+1' is NC
  }
  ifid += (vnFlag) ? (tcid + 1) : 0; // '+1' is for tcid(0,1,2 ...)
#endif // NATIVE_MODE_ENABLE

  ret = &(mpRteserv2If[ifid]);
  dmsg("  ret: " << ret << " ifid:" << ifid);
  return ret;
}



///<summary>function to get the pointer-list of Rteserv2IF which belongs to the specified VC in all HTs</summary>
///<param name="peId">PEID</param>
///<param name="vcId">VCID</param>
///<returns>list of the pointers to Rteserv2IF</returns>
void
Rteserv2Controller::getRteserv2IfIdx(
  unsigned int  peId,
  unsigned int  vcId,
  std::set<unsigned int>* pIdxLst )
{
  dmsg("Rteserv2Controller::getRteserv2IfIdx");
  unsigned int if_id  = 0;
  pIdxLst->clear();
  for ( unsigned int i = 0; i < mRteserv2IFNum; i++ ) {
    if_id = (mpRteserv2If[i]).getId();
    if ( (RTE_PEID( if_id ) == peId) &&
         (RTE_VCID( if_id ) == vcId) ) {
       pIdxLst->insert( i );
    }
  }
}



///<summary>fonction to check the hit the break-point or not</summary>
///<param name="none"></param>
///<returns>false(not hit)/true(hit)</returns>
bool
Rteserv2Controller::checkBreak( void )
{
  dmsg("Rteserv2Controller::checkBreak");
  char       msg[32];
  bool       ret     = false;
  BreakInfo* pBrkInf = 0;
  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    pBrkInf = mpBreakInfoCA;
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    pBrkInf = mpBreakInfoFast;
  }
*/
  else {
    ErrMsg("Rteserv2Controller", "checkBreak", "Invalid simulation mode has been specified.");
  }

  if ( BRK_INST_EXE   == (pBrkInf->attr & BRK_INST_EXE) ||
       BRK_DATA_READ  == (pBrkInf->attr & BRK_DATA_READ) ||
       BRK_DATA_WRITE == (pBrkInf->attr & BRK_DATA_WRITE)) { // case of RUN or access-break
    unsigned int peid  = pBrkInf->peid;
    //unsigned int vcid  = pBrkInf->vcid;
    unsigned int vcid  = -1;
    //unsigned int tcid  = pBrkInf->tcid;
    unsigned int tcid  = -1;

    if ( 0 < peid ) {
#if 0
      if ( NC == pBrkInf->tcid || NC == pBrkInf->vcid ) { // NC
        ret = getRteserv2IfPtr( RTE_CPUID(peid, vcid, tcid, 0) )->checkBreak();
      }
      else { // VC
        ret = getRteserv2IfPtr( RTE_CPUID(peid, vcid, tcid, 1) )->checkBreak();
      }
#else
      ret = getRteserv2IfPtr( RTE_CPUID(peid, vcid, tcid, 0) )->checkBreak();
#endif
    }
    else {
      dmsg("  pBrkInf->attr:" << pBrkInf->attr);
      sprintf(msg, "pBrkInf->peid(%d) is invalid.", pBrkInf->peid);
      ErrMsg("Rteserv2Controller", "checkBreak", msg);
    }
  }
  else { // case of not breaked
    // do nothing
    dmsg("  BRK_NONE or BRK_TIMEOUT or BRK_ACCEPT_INT");
  }

  return ret;
}



///<summary>fonction to remove the break-points on PC</summary>
///<param name="none"></param>
///<returns>false(not hit)/true(hit)</returns>
bool
Rteserv2Controller::rmvBpOnPc( void )
{
  dmsg("Rteserv2Controller::rmvBpOnPc");
  bool         ret = false;
  unsigned int pc  = 0;

  if ( mpG4CPU->isSyncMode() && !mRunningFlag ) {
    for ( unsigned int i = 0; i < mRteserv2IFNum; i++ ) {
      pc = (mpRteserv2If[i]).getPC();
      if ( (mpRteserv2If[i]).isSetPcBp(pc) ) {
        mBpOnPcLst[i] = pc;
        (mpRteserv2If[i]).rmvPcBreak( pc );
        ret = true;
        dmsg("  BP(0x" << pc << ") is removed.");
      }
      else {
        // do nothing
      }
    }
  }
  else {
    // do nothing
  }

  return ret;
}



///<summary>fonction to set the break-points which were removed on PC</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::setBpOnPc( void )
{
  dmsg("Rteserv2Controller::setBpOnPc");

  if ( !mBpOnPcLst.empty() ) {
    for ( unsigned int i = 0; i < mRteserv2IFNum; i++ ) {
      if ( mBpOnPcLst.end() != mBpOnPcLst.find(i) ) {
        (mpRteserv2If[i]).setPcBreak( mBpOnPcLst[i] );
        dmsg("  BP(0x" << mBpOnPcLst[i] << ") is set.");
      }
      else {
        // do nothing
      }
    }
    mBpOnPcLst.clear();
  }
  else {
    // do nothing
  }
}



///<summary>function to set the information of system-register</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::setSysRegInfo( void )
{
  mpSysRegInfo->setSysRegInfo(700, 256, "EIPC", "T");
  mpSysRegInfo->setSysRegInfo(701, 257, "EIPSW", "T");
  mpSysRegInfo->setSysRegInfo(702, 258, "FEPC", "T");
  mpSysRegInfo->setSysRegInfo(703, 259, "FEPSW", "T");
  mpSysRegInfo->setSysRegInfo(704, 260, "-", "-");
  mpSysRegInfo->setSysRegInfo(705, 261, "PSW", "T");
  mpSysRegInfo->setSysRegInfo(706, 262, "FPSR", "T");
  mpSysRegInfo->setSysRegInfo(707, 263, "FPEPC", "T");
  mpSysRegInfo->setSysRegInfo(708, 264, "FPST", "T");
  mpSysRegInfo->setSysRegInfo(709, 265, "FPCC", "T");
  mpSysRegInfo->setSysRegInfo(710, 266, "FPCFG", "T");
  mpSysRegInfo->setSysRegInfo(711, 267, "FPEC", "T");
  mpSysRegInfo->setSysRegInfo(712, 268, "SESR", "T");
  mpSysRegInfo->setSysRegInfo(713, 269, "EIIC", "T");
  mpSysRegInfo->setSysRegInfo(714, 270, "FEIC", "T");
  mpSysRegInfo->setSysRegInfo(715, 271, "-", "-");
  mpSysRegInfo->setSysRegInfo(716, 272, "CTPC", "T");
  mpSysRegInfo->setSysRegInfo(717, 273, "CTPSW", "T");
  mpSysRegInfo->setSysRegInfo(718, 274, "-", "-");
  mpSysRegInfo->setSysRegInfo(719, 275, "-", "-");
  mpSysRegInfo->setSysRegInfo(720, 276, "CTBP", "T");
  mpSysRegInfo->setSysRegInfo(721, 277, "-", "-");
  mpSysRegInfo->setSysRegInfo(722, 278, "-", "-");
  mpSysRegInfo->setSysRegInfo(723, 279, "-", "-");
  mpSysRegInfo->setSysRegInfo(724, 280, "-", "-");
  mpSysRegInfo->setSysRegInfo(725, 281, "-", "-");
  mpSysRegInfo->setSysRegInfo(726, 282, "-", "-");
  mpSysRegInfo->setSysRegInfo(727, 283, "-", "-");
  mpSysRegInfo->setSysRegInfo(728, 284, "EIWR", "T");
  mpSysRegInfo->setSysRegInfo(729, 285, "FEWR", "T");
  mpSysRegInfo->setSysRegInfo(730, 286, "-", "-");
  mpSysRegInfo->setSysRegInfo(731, 287, "(BSEL)", "T");
  mpSysRegInfo->setSysRegInfo(732, 288, "MCFG0", "V");
  mpSysRegInfo->setSysRegInfo(733, 289, "MCFG1", "V");
  mpSysRegInfo->setSysRegInfo(734, 290, "RBASE", "V");
  mpSysRegInfo->setSysRegInfo(735, 291, "EBASE", "V");
  mpSysRegInfo->setSysRegInfo(736, 292, "INTBP", "V");
  mpSysRegInfo->setSysRegInfo(737, 293, "MCTL", "V");
  mpSysRegInfo->setSysRegInfo(738, 294, "PID", "V");
  mpSysRegInfo->setSysRegInfo(739, 295, "FPIPR", "T");
  mpSysRegInfo->setSysRegInfo(740, 296, "-", "-");
  mpSysRegInfo->setSysRegInfo(741, 297, "-", "-");
  mpSysRegInfo->setSysRegInfo(742, 298, "TCSEL", "V");
  mpSysRegInfo->setSysRegInfo(743, 299, "SCCFG", "V");
  mpSysRegInfo->setSysRegInfo(744, 300, "SCBP", "V");
  mpSysRegInfo->setSysRegInfo(745, 301, "HVCCFG", "V");
  mpSysRegInfo->setSysRegInfo(746, 302, "HVCBP", "V");
  mpSysRegInfo->setSysRegInfo(747, 303, "VCSEL", "V");
  mpSysRegInfo->setSysRegInfo(748, 304, "VMPRT0", "T");  // NC only
  mpSysRegInfo->setSysRegInfo(749, 305, "VMPRT1", "T");  // NC only
  mpSysRegInfo->setSysRegInfo(750, 306, "VMPRT2", "T");  // NC only
  mpSysRegInfo->setSysRegInfo(751, 307, "-", "-");
  mpSysRegInfo->setSysRegInfo(752, 308, "-", "-");
  mpSysRegInfo->setSysRegInfo(753, 309, "-", "-");
  mpSysRegInfo->setSysRegInfo(754, 310, "-", "-");
  mpSysRegInfo->setSysRegInfo(755, 311, "VMSCCTL", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(756, 312, "VMSCTBL0", "T");// NC only
  mpSysRegInfo->setSysRegInfo(757, 313, "VMSCTBL1", "T");// NC only
  mpSysRegInfo->setSysRegInfo(758, 314, "VMSCTBL2", "T");// NC only
  mpSysRegInfo->setSysRegInfo(759, 315, "VMSCTBL3", "T");// NC only
  mpSysRegInfo->setSysRegInfo(760, 316, "-", "-");
  mpSysRegInfo->setSysRegInfo(761, 317, "-", "-");
  mpSysRegInfo->setSysRegInfo(762, 318, "-", "-");
  mpSysRegInfo->setSysRegInfo(763, 319, "-", "-");
  mpSysRegInfo->setSysRegInfo(764, 320, "HTCFG0", "T");
  mpSysRegInfo->setSysRegInfo(765, 321, "-", "-");
  mpSysRegInfo->setSysRegInfo(766, 322, "-", "-");
  mpSysRegInfo->setSysRegInfo(767, 323, "-", "-");
  mpSysRegInfo->setSysRegInfo(768, 324, "-", "-");
  mpSysRegInfo->setSysRegInfo(769, 325, "HTCTL", "T");
  mpSysRegInfo->setSysRegInfo(770, 326, "MEA", "T");
  mpSysRegInfo->setSysRegInfo(771, 327, "ASID", "T");
  mpSysRegInfo->setSysRegInfo(772, 328, "MEI", "T");
  mpSysRegInfo->setSysRegInfo(773, 329, "-", "-");
  mpSysRegInfo->setSysRegInfo(774, 330, "ISPR", "T");
  mpSysRegInfo->setSysRegInfo(775, 331, "PMR", "T");
  mpSysRegInfo->setSysRegInfo(776, 332, "ICSR", "T");
  mpSysRegInfo->setSysRegInfo(777, 333, "INTCFG", "T");
  mpSysRegInfo->setSysRegInfo(778, 334, "-", "-");
  mpSysRegInfo->setSysRegInfo(779, 335, "-", "-");
  mpSysRegInfo->setSysRegInfo(780, 336, "TLBSCH", "V");
  mpSysRegInfo->setSysRegInfo(781, 337, "-", "-");
  mpSysRegInfo->setSysRegInfo(782, 338, "-", "-");
  mpSysRegInfo->setSysRegInfo(783, 339, "-", "-");
  mpSysRegInfo->setSysRegInfo(784, 340, "-", "-");
  mpSysRegInfo->setSysRegInfo(785, 341, "-", "-");
  mpSysRegInfo->setSysRegInfo(786, 342, "-", "-");
  mpSysRegInfo->setSysRegInfo(787, 343, "HTSCCTL", "V");
  mpSysRegInfo->setSysRegInfo(788, 344, "HTSCTBL0", "V");
  mpSysRegInfo->setSysRegInfo(789, 345, "HTSCTBL1", "V");
  mpSysRegInfo->setSysRegInfo(790, 346, "HTSCTBL2", "V");
  mpSysRegInfo->setSysRegInfo(791, 347, "HTSCTBL3", "V");
  mpSysRegInfo->setSysRegInfo(792, 348, "HTSCTBL4", "V");
  mpSysRegInfo->setSysRegInfo(793, 349, "HTSCTBL5", "V");
  mpSysRegInfo->setSysRegInfo(794, 350, "HTSCTBL6", "V");
  mpSysRegInfo->setSysRegInfo(795, 351, "HTSCTBL7", "V");
  mpSysRegInfo->setSysRegInfo(796, 352, "VMSCPTR", "V");
  mpSysRegInfo->setSysRegInfo(797, 353, "HTSCPTR", "V");
  mpSysRegInfo->setSysRegInfo(798, 354, "-", "-");
  mpSysRegInfo->setSysRegInfo(799, 355, "-", "-");
  mpSysRegInfo->setSysRegInfo(800, 356, "-", "-");
  mpSysRegInfo->setSysRegInfo(801, 357, "-", "-");
  mpSysRegInfo->setSysRegInfo(802, 358, "-", "-");
  mpSysRegInfo->setSysRegInfo(803, 359, "-", "-");
  mpSysRegInfo->setSysRegInfo(804, 360, "-", "-");
  mpSysRegInfo->setSysRegInfo(805, 361, "-", "-");
  mpSysRegInfo->setSysRegInfo(806, 362, "-", "-");
  mpSysRegInfo->setSysRegInfo(807, 363, "-", "-");
  mpSysRegInfo->setSysRegInfo(808, 364, "-", "-");
  mpSysRegInfo->setSysRegInfo(809, 365, "-", "-");
  mpSysRegInfo->setSysRegInfo(810, 366, "-", "-");
  mpSysRegInfo->setSysRegInfo(811, 367, "DBIC", "V");
  mpSysRegInfo->setSysRegInfo(812, 368, "-", "-");
  mpSysRegInfo->setSysRegInfo(813, 369, "DBCMC", "V");
  mpSysRegInfo->setSysRegInfo(814, 370, "DBPC", "V");
  mpSysRegInfo->setSysRegInfo(815, 371, "DBPSW", "V");
  mpSysRegInfo->setSysRegInfo(816, 372, "DIR0", "V");
  mpSysRegInfo->setSysRegInfo(817, 373, "DIR1", "V");
  mpSysRegInfo->setSysRegInfo(818, 374, "BPC", "V");
  mpSysRegInfo->setSysRegInfo(819, 375, "-", "-");
  mpSysRegInfo->setSysRegInfo(820, 376, "BPAV", "V");
  mpSysRegInfo->setSysRegInfo(821, 377, "BPAM", "V");
  mpSysRegInfo->setSysRegInfo(822, 378, "BPDV", "V");
  mpSysRegInfo->setSysRegInfo(823, 379, "BPDM", "V");
  mpSysRegInfo->setSysRegInfo(824, 380, "-", "-");
  mpSysRegInfo->setSysRegInfo(825, 381, "-", "-");
  mpSysRegInfo->setSysRegInfo(826, 382, "DBWR", "V");
  mpSysRegInfo->setSysRegInfo(827, 383, "-", "-");
  mpSysRegInfo->setSysRegInfo(828, 384, "TLBIDX", "T");// NC only
  mpSysRegInfo->setSysRegInfo(829, 385, "-", "-");
  mpSysRegInfo->setSysRegInfo(830, 386, "-", "-");
  mpSysRegInfo->setSysRegInfo(831, 387, "-", "-");
  mpSysRegInfo->setSysRegInfo(832, 388, "TELO0", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(833, 389, "TELO1", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(834, 390, "TEHI0", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(835, 391, "TEHI1", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(836, 392, "-", "-");
  mpSysRegInfo->setSysRegInfo(837, 393, "-", "-");
  mpSysRegInfo->setSysRegInfo(838, 394, "TLBCFG", "T");// NC only
  mpSysRegInfo->setSysRegInfo(839, 395, "-", "-");
  mpSysRegInfo->setSysRegInfo(840, 396, "BWERRL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(841, 397, "BWERRH", "T");// NC only
  mpSysRegInfo->setSysRegInfo(842, 398, "BRERRL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(843, 399, "BRERRH", "T");// NC only
  mpSysRegInfo->setSysRegInfo(844, 400, "ICTAGL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(845, 401, "ICTAGH", "T");// NC only
  mpSysRegInfo->setSysRegInfo(846, 402, "ICDATL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(847, 403, "ICDATH", "T");// NC only
  mpSysRegInfo->setSysRegInfo(848, 404, "DCTAGL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(849, 405, "DCTAGH", "T");// NC only
  mpSysRegInfo->setSysRegInfo(850, 406, "DCDATL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(851, 407, "DCDATH", "T");// NC only
  mpSysRegInfo->setSysRegInfo(852, 408, "ICCTRL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(853, 409, "DCCTRL", "T");// NC only
  mpSysRegInfo->setSysRegInfo(854, 410, "ICCFG", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(855, 411, "DCCFG", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(856, 412, "ICERR", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(857, 413, "DCERR", "T"); // NC only
  mpSysRegInfo->setSysRegInfo(858, 414, "-", "-");
  mpSysRegInfo->setSysRegInfo(859, 415, "-", "-");
  mpSysRegInfo->setSysRegInfo(860, 416, "MPM", "V");
  mpSysRegInfo->setSysRegInfo(861, 417, "MPRC", "V");
  mpSysRegInfo->setSysRegInfo(862, 418, "-", "-");
  mpSysRegInfo->setSysRegInfo(863, 419, "-", "-");
  mpSysRegInfo->setSysRegInfo(864, 420, "MPBRGN", "V");
  mpSysRegInfo->setSysRegInfo(865, 421, "MPTRGN", "V");
  mpSysRegInfo->setSysRegInfo(866, 422, "-", "-");
  mpSysRegInfo->setSysRegInfo(867, 423, "-", "-");
  mpSysRegInfo->setSysRegInfo(868, 424, "MCA", "T");
  mpSysRegInfo->setSysRegInfo(869, 425, "MCS", "T");
  mpSysRegInfo->setSysRegInfo(870, 426, "MCC", "T");
  mpSysRegInfo->setSysRegInfo(871, 427, "MCR", "T");
  mpSysRegInfo->setSysRegInfo(872, 428, "-", "-");
  mpSysRegInfo->setSysRegInfo(873, 429, "-", "-");
  mpSysRegInfo->setSysRegInfo(874, 430, "-", "-");
  mpSysRegInfo->setSysRegInfo(875, 431, "-", "-");
  mpSysRegInfo->setSysRegInfo(876, 432, "-", "-");
  mpSysRegInfo->setSysRegInfo(877, 433, "-", "-");
  mpSysRegInfo->setSysRegInfo(878, 434, "-", "-");
  mpSysRegInfo->setSysRegInfo(879, 435, "-", "-");
  mpSysRegInfo->setSysRegInfo(880, 436, "MPPRT0", "T");// NC only
  mpSysRegInfo->setSysRegInfo(881, 437, "MPPRT1", "T");// NC only
  mpSysRegInfo->setSysRegInfo(882, 438, "MPPRT2", "T");// NC only
  mpSysRegInfo->setSysRegInfo(883, 439, "-", "-");
  mpSysRegInfo->setSysRegInfo(884, 440, "-", "-");
  mpSysRegInfo->setSysRegInfo(885, 441, "-", "-");
  mpSysRegInfo->setSysRegInfo(886, 442, "-", "-");
  mpSysRegInfo->setSysRegInfo(887, 443, "-", "-");
  mpSysRegInfo->setSysRegInfo(888, 444, "-", "-");
  mpSysRegInfo->setSysRegInfo(889, 445, "-", "-");
  mpSysRegInfo->setSysRegInfo(890, 446, "-", "-");
  mpSysRegInfo->setSysRegInfo(891, 447, "-", "-");
  mpSysRegInfo->setSysRegInfo(892, 448, "MPLA0", "V");
  mpSysRegInfo->setSysRegInfo(893, 449, "MPUA0", "V");
  mpSysRegInfo->setSysRegInfo(894, 450, "MPAT0", "V");
  mpSysRegInfo->setSysRegInfo(895, 451, "-", "-");
  mpSysRegInfo->setSysRegInfo(896, 452, "MPLA1", "V");
  mpSysRegInfo->setSysRegInfo(897, 453, "MPUA1", "V");
  mpSysRegInfo->setSysRegInfo(898, 454, "MPAT1", "V");
  mpSysRegInfo->setSysRegInfo(899, 455, "-", "-");
  mpSysRegInfo->setSysRegInfo(900, 456, "MPLA2", "V");
  mpSysRegInfo->setSysRegInfo(901, 457, "MPUA2", "V");
  mpSysRegInfo->setSysRegInfo(902, 458, "MPAT2", "V");
  mpSysRegInfo->setSysRegInfo(903, 459, "-", "-");
  mpSysRegInfo->setSysRegInfo(904, 460, "MPLA3", "V");
  mpSysRegInfo->setSysRegInfo(905, 461, "MPUA3", "V");
  mpSysRegInfo->setSysRegInfo(906, 462, "MPAT3", "V");
  mpSysRegInfo->setSysRegInfo(907, 463, "-", "-");
  mpSysRegInfo->setSysRegInfo(908, 464, "MPLA4", "V");
  mpSysRegInfo->setSysRegInfo(909, 465, "MPUA4", "V");
  mpSysRegInfo->setSysRegInfo(910, 466, "MPAT4", "V");
  mpSysRegInfo->setSysRegInfo(911, 467, "-", "-");
  mpSysRegInfo->setSysRegInfo(912, 468, "MPLA5", "V");
  mpSysRegInfo->setSysRegInfo(913, 469, "MPUA5", "V");
  mpSysRegInfo->setSysRegInfo(914, 470, "MPAT5", "V");
  mpSysRegInfo->setSysRegInfo(915, 471, "-", "-");
  mpSysRegInfo->setSysRegInfo(916, 472, "MPLA6", "V");
  mpSysRegInfo->setSysRegInfo(917, 473, "MPUA6", "V");
  mpSysRegInfo->setSysRegInfo(918, 474, "MPAT6", "V");
  mpSysRegInfo->setSysRegInfo(919, 475, "-", "-");
  mpSysRegInfo->setSysRegInfo(920, 476, "MPLA7", "V");
  mpSysRegInfo->setSysRegInfo(921, 477, "MPUA7", "V");
  mpSysRegInfo->setSysRegInfo(922, 478, "MPAT7", "V");
  mpSysRegInfo->setSysRegInfo(923, 479, "-", "-");
  mpSysRegInfo->setSysRegInfo(924, 480, "MPLA8", "V");
  mpSysRegInfo->setSysRegInfo(925, 481, "MPUA8", "V");
  mpSysRegInfo->setSysRegInfo(926, 482, "MPAT8", "V");
  mpSysRegInfo->setSysRegInfo(927, 483, "-", "-");
  mpSysRegInfo->setSysRegInfo(928, 484, "MPLA9", "V");
  mpSysRegInfo->setSysRegInfo(929, 485, "MPUA9", "V");
  mpSysRegInfo->setSysRegInfo(930, 486, "MPAT9", "V");
  mpSysRegInfo->setSysRegInfo(931, 487, "-", "-");
  mpSysRegInfo->setSysRegInfo(932, 488, "MPLA10", "V");
  mpSysRegInfo->setSysRegInfo(933, 489, "MPUA10", "V");
  mpSysRegInfo->setSysRegInfo(934, 490, "MPAT10", "V");
  mpSysRegInfo->setSysRegInfo(935, 491, "-", "-");
  mpSysRegInfo->setSysRegInfo(936, 492, "MPLA11", "V");
  mpSysRegInfo->setSysRegInfo(937, 493, "MPUA11", "V");
  mpSysRegInfo->setSysRegInfo(938, 494, "MPAT11", "V");
  mpSysRegInfo->setSysRegInfo(939, 495, "-", "-");
  mpSysRegInfo->setSysRegInfo(940, 496, "MPLA12", "V");
  mpSysRegInfo->setSysRegInfo(941, 497, "MPUA12", "V");
  mpSysRegInfo->setSysRegInfo(942, 498, "MPAT12", "V");
  mpSysRegInfo->setSysRegInfo(943, 499, "-", "-");
  mpSysRegInfo->setSysRegInfo(944, 500, "MPLA13", "V");
  mpSysRegInfo->setSysRegInfo(945, 501, "MPUA13", "V");
  mpSysRegInfo->setSysRegInfo(946, 502, "MPAT13", "V");
  mpSysRegInfo->setSysRegInfo(947, 503, "-", "-");
  mpSysRegInfo->setSysRegInfo(948, 504, "MPLA14", "V");
  mpSysRegInfo->setSysRegInfo(949, 505, "MPUA14", "V");
  mpSysRegInfo->setSysRegInfo(950, 506, "MPAT14", "V");
  mpSysRegInfo->setSysRegInfo(951, 507, "-", "-");
  mpSysRegInfo->setSysRegInfo(952, 508, "MPLA15", "V");
  mpSysRegInfo->setSysRegInfo(953, 509, "MPUA15", "V");
  mpSysRegInfo->setSysRegInfo(954, 510, "MPAT15", "V");
  mpSysRegInfo->setSysRegInfo(955, 511, "-", "-");
}



///<summary>basic function to control the simulation</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::ctrlSim( void )
{
  dmsg("Rteserv2Controller::ctrlSim");
  bool         isDatRcvd    = false;
  BreakHitAttribute breakType = BRK_NONE;
  bool         isReset      = false; // added by yoshinaga(for Defect-UT#7300)
  unsigned int peid         = 0;
  unsigned int htid         = 0;
  unsigned int code         = 0;
  unsigned int i            = 0;
  unsigned int runStateCnt  = 0;
  unsigned int stepStateCnt = 0;
  unsigned int stepIfIdx    = 0;
  unsigned int cpuid        = 0;
  fd_set       fdSet; // set of file-discripter including socket
  timeval      timeOut;
  SIM_MODE_T   simMode;
  SIM_STATE_T  simState = RTE_STATE_HALT;
  uint64_t     cycleNum;
  GuardErrType grd_err  = GD_NONE_ERR;

  // if sc_stop is called in Terminate(), go through the while() loop
  while( !scstopDoneFlag ) {
    cycleNum        = 0;
    timeOut.tv_sec  = (RTE_STATE_HALT == simState) ? 1 : 0;
    timeOut.tv_usec = 0;

    FD_ZERO( &fdSet );
    FD_SET ( mSockFd, &fdSet );
    select ( mSockFd+1, &fdSet, NULL, NULL, &timeOut );

    isDatRcvd = FD_ISSET(mSockFd, &fdSet);
    if ( isDatRcvd ){
      dmsg("isDatRcvd:" << isDatRcvd);
      receiveCommand();
      processCommand();
      sendResponse();
    }

    runStateCnt      = 0;
    stepStateCnt     = 0;
    stepIfIdx        = 0;
    cpuid            = 0;
    mStopAnyCoreFlag = false; // this is member-variable to use in exCheckStat()

    for ( i = 0; i < mRteserv2IFNum; i++ ) {
      switch ( (mpRteserv2If[i]).getSimState() ) {
        case RTE_STATE_HALT:
          dmsg("  RTE_STATE_HALT");
          mStopAnyCoreFlag = true;
          mpForestUtil->SetRunTarget(i+1, false);
          break;
        case RTE_STATE_RUN:
          dmsg("  RTE_STATE_RUN");
          runStateCnt++;
          mpForestUtil->SetRunTarget(i+1, true);
          break;
        case RTE_STATE_STEP:
          dmsg("  RTE_STATE_STEP");
          stepStateCnt++;
          stepIfIdx = i;
          mpForestUtil->SetRunTarget(i+1, true);
          break;
        default:
          break;
      }
      if ( mStopAnyCoreFlag && mpG4CPU->isSyncMode() ) {
        break;
      }
    }

    simMode = mpG4CPU->GetSimMode();
    dmsg("  runStateCnt:" << runStateCnt << " isAsyncMode():" << mpG4CPU->isAsyncMode());
    if ( (runStateCnt == mRteserv2IFNum) ||  // Sync RUN
         ((0<runStateCnt) && (0==stepStateCnt) && mpG4CPU->isAsyncMode()) ) { // Async RUN
      if ( SIM_MODE_CA == simMode ) {
        if ( !mIsUnderReset ) { // added by yoshinaga(for Defect-UT#7300)
          isReset = false;   // added by yoshinaga(for Defect-UT#7300)
          if ( rmvBpOnPc() ) { // for BP on PC (if no BP is on PC or sim-mode is AsyncMode, always returns false.)
            dmsg("  mpForestUtil->CycleExecute( " << mLastRunPeid << ", -1, mCaissSyncPeriod, EXEC_TYPE_STEP )");
            cycleNum += mpForestUtil->CycleExecute( mLastRunPeid, mCaissSyncPeriod, EXEC_TYPE_STEP );
            setBpOnPc();
          }
          else { // Normal RUN execution
            dmsg("  mpForestUtil->CycleExecute( " << mLastRunPeid << ", -1, mCaissSyncPeriod, EXEC_TYPE_RUN )");
            cycleNum = mpForestUtil->CycleExecute( mLastRunPeid, mCaissSyncPeriod, EXEC_TYPE_RUN );
          }       // added by yoshinaga(for Defect-UT#7300)
          flushTraceData();
          breakType = CheckBreakType( peid, code, grd_err );
          if ( checkBreak() ) {
            dmsg("  checkBreak():true cycleNum:0x" << cycleNum);
            simState = RTE_STATE_HALT;
            mRunningFlag = false;
          }
          else if ( BRK_STEP_EXE == (breakType & BRK_STEP_EXE) ) {
            simState = RTE_STATE_RUN;
            mRunningFlag = true;
          }
          else {
            dmsg("  checkBreak():false cycleNum:0x" << cycleNum);
            simState = RTE_STATE_RUN;
            mRunningFlag = true;
          }
        }
        else {  // added by yoshinaga(for Defect-UT#7300)
          isReset  = true;             // added by yoshinaga(for Defect-UT#7300)
          cycleNum = mCaissSyncPeriod; // added by yoshinaga(for Defect-UT#7300)
          simState = RTE_STATE_RUN;    // added by yoshinaga(for Defect-UT#7300)
          mRunningFlag = false;
        }      //  added by yoshinaga(for Defect-UT#7300)
      }
      else if ( SIM_MODE_FAST == simMode ) {
        // For FastISS
      }
      else {
        // do nothing
      }
    }
    else if ( (1 == stepStateCnt) && 
                (((mRteserv2IFNum-1) == runStateCnt)  // Sync STEP
                 || mpG4CPU->isAsyncMode()) ) {      // Async STEP
      simState = RTE_STATE_STEP;
      cpuid    = (mpRteserv2If[stepIfIdx]).getId();
      if ( SIM_MODE_CA == simMode ) {
        if ( !mIsUnderReset ) { // added by yoshinaga(for Defect-UT#7300)
          isReset    = false;  // added by yoshinaga(for Defect-UT#7300)
          rmvBpOnPc();
          if ( RTE_VN(cpuid) ) { // VC
            dmsg("  mpForestUtil->CycleExecute( " << RTE_PEID(cpuid) << ", " << RTE_TCID(cpuid) << ", mCaissSyncPeriod, EXEC_TYPE_STEP )");
            cycleNum += mpForestUtil->CycleExecute( RTE_PEID(cpuid), mCaissSyncPeriod, EXEC_TYPE_STEP );
          }
          else { // NC
            dmsg("  mpForestUtil->CycleExecute( " << RTE_PEID(cpuid) << ", NC, mCaissSyncPeriod, EXEC_TYPE_STEP )");
            cycleNum += mpForestUtil->CycleExecute( RTE_PEID(cpuid), mCaissSyncPeriod, EXEC_TYPE_STEP );
          }
          setBpOnPc();
          flushTraceData();
          breakType = CheckBreakType( peid, code, grd_err );
          if ( !checkBreak() && (BRK_STEP_EXE == (breakType & BRK_STEP_EXE)) ) {
            (mpRteserv2If[stepIfIdx]).setHaltState(); // stop the self-core
            mRunningFlag = false;
          }
          else {
            mRunningFlag = true;
            // The case the simulation is hit on the break-point,
            // return the same state as SW break at checkstat
          }
        }      // added by yoshinaga(for Defect-UT#7300)
        else { // added by yoshinaga(for Defect-UT#7300)
          isReset  = true;             // added by yoshinaga(for Defect-UT#7300)
          cycleNum = mCaissSyncPeriod; // added by yoshinaga(for Defect-UT#7300)
          mRunningFlag = false;
        } // added by yoshinaga(for Defect-UT#7300)
      }
      else if ( SIM_MODE_FAST == simMode ) {
        // For FastISS
      }
      else {
        // do nothing
      }
      stepStateCnt = 0;
      stepIfIdx    = 0;
    }
    else {
      simState = RTE_STATE_HALT;
      mRunningFlag = false;
    }

    if ( 0 != cycleNum ) {
      if ( SIM_MODE_CA == simMode ) {
        mpG4CPU->mpCaIss->elapseTime( cycleNum * mpClkPeriod[mFastestPE] );
      }
      else if ( SIM_MODE_FAST == simMode ) {
        // For FastISS
      }
      else {
        // do nothing
      }
    }
    if ( (BRK_ACCEPT_INT == (breakType & BRK_ACCEPT_INT)) && (!isReset) ) {
      SetAckInfo(peid, code);
    }
    else if( (BRK_PC_MATCH == (breakType & BRK_PC_MATCH)) && (!isReset) ) {
      mpG4CPU->SetSwdtClkOutEvFromCa(peid);
    }
    else if( (BRK_GUARD_ERR == (breakType & BRK_GUARD_ERR)) && (!isReset) ) {
      // Guard Error
      NSMVG4CPUV01::GRD_ERR_SRC_T grd_err_typ = (NSMVG4CPUV01::GRD_ERR_SRC_T)GetGrdSrc( grd_err );
      if(grd_err_typ == NSMVG4CPUV01::GRD_ERR_PE_GRD_S){
        unsigned int grd_id = peid;
        GetAccTypeAtGrdErr( peid, grd_id );
	mpG4CPU->SetGrdErrFromCa( grd_id, grd_err_typ );
      }
      else {
        mpG4CPU->SetGrdErrFromCa( peid, grd_err_typ );
      }
    }
  } // while( !scstopDoneFlag )
}



///<summary>function to process the HALT command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exHalt( void )
{
  dmsg("Rteserv2Controller::exHalt");
  if ( mpG4CPU->isSyncMode() ) {
    for ( unsigned int i = 0; i < mRteserv2IFNum; i++ ) {
      (mpRteserv2If[i]).setHaltState();
    }
  }
  else {
    unsigned int code = 0;
    unsigned int id   = 0;
    sscanf( mCmdRcv, "%d %d", &code, &id );
    Rteserv2IF* rteserv2if_ptr = getRteserv2IfPtr( id );
    rteserv2if_ptr->setHaltState();
  }
  sprintf( mCmdRsp, "%c\n", SUCCESS );
}



///<summary>function to process the QUIT command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exQuit( void )
{
  dmsg("Rteserv2Controller::exQuit");
  printf( "Quit!\n" );
  fflush( stdout );

#ifdef LINUX_DEF
  close( mSockFd );
#endif // LINUX_DEF
#ifdef WINDOWS_DEF
  closesocket( mSockFd );
  WSACleanup();
#endif // WINDOWS_DEF
  Terminate();
}



///<summary>function to process the RESET command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exReset( void )
{
  dmsg("Rteserv2Controller::exReset");

  if ( mIsUnderReset ) {
    // RESET signal has been active already.
  }
  else if ( mPeripheralFlag ) {
    if ( 0 == PFRH850_0 ) {
      // no peripheral
      ErrMsg("Rteserv2Controller", "exReset", "PFRH850 pointer is 0.");
    }
    else {
      dmsg("  PFRH850_0->RESETZ_sig.write( false )");
      PFRH850_0->RESETZ_sig.write( false ); // reset signal activate
      WAIT_CYCLE( 1 ); // wait 1 cycle
      dmsg("  PFRH850_0->RESETZ_sig.write( true )");
      PFRH850_0->RESETZ_sig.write( true ); // reset signal inactivate
    }
  }
  else {
    dmsg("  PltfrmGnd::reset_sig->write( false )");
    PltfrmGnd::reset_sig->write( false ); // reset signal activate
    WAIT_CYCLE( 1 ); // wait 1 cycle
    dmsg("  PltfrmGnd::reset_sig->write( true )");
    PltfrmGnd::reset_sig->write( true ); // reset signal inactivate
  }
  dmsg("  mCmdRsp:" << SUCCESS);
  sprintf( mCmdRsp, "%c\n", SUCCESS );
}



///<summary>function to process the Read-Memory command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exReadMem( void )
{
  dmsg("Rteserv2Controller::exReadMem");
  unsigned int code = 0;
  unsigned int id   = 0;
  unsigned int addr = 0;
  unsigned int size = 0;
  unsigned int data = 0; // supported 1/2/4 Byte access from MULTI

  sscanf( mCmdRcv, "%d %d %d %d", &code, &id, &addr, &size );
  unsigned int peid = RTE_PEID( id );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    data = mpForestUtil->VMemReadDebug( peid, addr, size );
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    data = mpFastIssIf->readMem( peid, addr, size );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exReadMem", msg);
  }
  sprintf( mCmdRsp, "%c %d \n", SUCCESS, data );
}



///<summary>function to process the Write-Memory command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exWriteMem( void )
{
  dmsg("Rteserv2Controller::exWriteMem");
  unsigned int code = 0;
  unsigned int id   = 0;
  unsigned int addr = 0;
  unsigned int size = 0;
  unsigned int data = 0;

  sscanf( mCmdRcv, "%d %d %d %d %d", &code, &id, &addr, &size, &data );
  unsigned int peid = RTE_PEID( id );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    mpForestUtil->VMemWriteDebug( peid, addr, data, size );
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    mpFastIssIf->writeMem( peid, addr, data, size );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exWriteMem", msg);
  }
  sprintf( mCmdRsp, "%c %d \n", SUCCESS, data );
}



///<summary>function to process the Read-BrockMemory command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exReadBlockMem( void )
{
  dmsg("Rteserv2Controller::exReadBlockMem");
  unsigned int  code = 0;
  unsigned int  id   = 0;
  unsigned int  addr = 0;
  unsigned int  num  = 0;
  unsigned int  size = 0;
  unsigned int  i    = 0;
  unsigned char tmp_data[4];
  char          msg[32];

  sscanf( mCmdRcv, "%d %d %d %d %d", &code, &id, &addr, &num, &size );
  dmsg("  code:" << code << "  id:" << id << "  addr:0x" << hex << addr
       << "  num:" << num << "  size:" << size);
  sprintf( mCmdRsp, "%c \n", SUCCESS );
  unsigned int  peid = RTE_PEID( id );

  dmsg("  mpG4CPU->GetSimMode()");
  SIM_MODE_T simMode = mpG4CPU->GetSimMode();
  dmsg("  simMode:" << simMode);

  if ( 1 == size ) {
    dmsg("size:1");
    unsigned char* data_ptr = (unsigned char*)(mCmdRsp+3);
    if ( SIM_MODE_CA == simMode ) {
      for ( i = 0; i < num; i++, data_ptr++, addr+=size ) {
        *data_ptr = mpForestUtil->VMemReadDebug( peid, addr, size );
      }
    }
/* for FastISS
    else if ( SIM_MODE_FAST == simMode ) {
      for ( i = 0; i < num; i++, data_ptr++, addr += size ) {
        *data_ptr = mpFastIssIf->readMem( peid, addr, size );
      }
    }
*/
    else {
      sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
      ErrMsg("Rteserv2Controller", "exReadBlockMem", msg);
    }
  }
  else if ( 2 == size ) {
    dmsg("size:2");
    unsigned short int* data_ptr = (unsigned short int*)(mCmdRsp+3);
    for ( i = 0; i < num; i++, data_ptr++, addr += size ) {
      if ( SIM_MODE_CA == simMode ) {
        *data_ptr = htons( mpForestUtil->VMemReadDebug( peid, addr, size ) );
      } // htons: host byte order to network byte order short
/* for FastISS
      else if ( SIM_MODE_FAST == simMode ) {
        unsigned int tdat = mpFastIssIf->readMem( peid, addr, size );
        *data_ptr = htons( (unsigned short int)tdat );
      }
*/
      else {
        sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
        ErrMsg("Rteserv2Controller", "exReadBlockMem", msg);
      }
      tmp_data[0] = *((unsigned char*)data_ptr+1);
      tmp_data[1] = *((unsigned char*)data_ptr);
      *data_ptr   = *(unsigned short int*)tmp_data;
      dmsg("  addr:0x" << hex << addr << " *data_ptr:" << (int)(*data_ptr));
    }
  }
  else if ( 4 == size ) {
    dmsg("size:4");
    unsigned int* data_ptr = (unsigned int*)(mCmdRsp+3);
    for ( i = 0; i < num; i++, data_ptr++, addr += size ) {
      if ( SIM_MODE_CA == simMode ) {
        *data_ptr = htonl( mpForestUtil->VMemReadDebug( peid, addr, size ) );
      } // htonl: host byte order to network byte order long
/* for FastISS
      else if ( SIM_MODE_FAST == simMode ) {
        *data_ptr = htonl((unsigned int)(mpFastIssIf->readMem( peid, (unsigned int)addr, (unsigned int)size )));
      }
*/
      else {
        sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
        ErrMsg("Rteserv2Controller", "exReadBlockMem", msg);
      }
      tmp_data[0] = *((unsigned char*)data_ptr+3);
      tmp_data[1] = *((unsigned char*)data_ptr+2);
      tmp_data[2] = *((unsigned char*)data_ptr+1);
      tmp_data[3] = *((unsigned char*)data_ptr);
      *data_ptr = *(unsigned int *)tmp_data;
      dmsg("  addr:0x" << hex << addr << " *data_ptr:" << (int)(*data_ptr));
    }
  }
  else {
    sprintf(msg, "Invalid access size(%d) has been set.", size);
    ErrMsg("Rteserv2Controller", "exReadBlockMem", msg);
  }
  send( mSockFd, mCmdRsp, num*size+3, 0 );
  mCmdRsp[0] = '\0';
}



///<summary>function to process the Write-BlockMemory command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exWriteBlockMem( void )
{
  dmsg("Rteserv2Controller::exWriteBlockMem");
  unsigned int   code = 0;
  unsigned int   id   = 0;
  unsigned int   addr = 0;
  unsigned int   num  = 0;
  unsigned int   size = 0;
  unsigned char  tmp_data[4];
  char           msg[32];

  sscanf( mCmdRcv, "%d %d %d %d %d", &code, &id, &addr, &num, &size );
  dmsg("  id:" << id << " addr:0x" << hex << addr << " num:" << num << " size:" << size);
  unsigned int peid = RTE_PEID( id );

  char*          recv_data_ptr = mCmdRcv;
  char           ch            = CMD_NULL;
  unsigned int   data          = 0;
  unsigned int   recv_cnt      = 0;
  unsigned int   data_cnt      = 0;
  unsigned int   total         = num * size;

  // receive comand
  while ( data_cnt < total ) {
    recv_cnt = recv( mSockFd, &ch, sizeof(char), 0 );
    if( recv_cnt > 0 ) {
      *recv_data_ptr = ch;
      recv_data_ptr++;
    }
    data_cnt++;
  }
  recv_data_ptr = mCmdRcv;
  data_cnt      = 0;

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  while ( data_cnt < num ) {
    // transform data
    if ( 1 == size ) {
      data = (unsigned int)(*((unsigned char*)recv_data_ptr));
    }
    else if ( 2 == size ) {
      data        = (unsigned int)ntohs( *((unsigned short *)recv_data_ptr) );
                    // ntohs: network byte order to host byte order short
      tmp_data[1] = *((unsigned char *)&data);
      tmp_data[0] = *((unsigned char *)&data + 1);
      data        = (unsigned int)(*((unsigned short*)tmp_data));
    }
    else if ( 4 == size ) {
      data        = ntohl( *(unsigned long *)recv_data_ptr );
                    // ntohl: network byte order to host byte order long
      tmp_data[3] = *((unsigned char *)&data);
      tmp_data[2] = *((unsigned char *)&data+1);
      tmp_data[1] = *((unsigned char *)&data+2);
      tmp_data[0] = *((unsigned char *)&data+3);
      data        = *(unsigned int *)(tmp_data);
    }
    else {
      sprintf(msg, "Invalid access size(%d) has been set.", size);
      ErrMsg("Rteserv2Controller", "exWriteBlockMem", msg);
    }

    // write data
    if ( SIM_MODE_CA == simMode ) {
      mpForestUtil->VMemWriteDebug( peid, addr, data, size );
    }
/* for FastISS
    else if ( SIM_MODE_FAST == simMode ) {
      mpFastIssIf->writeMem( peid, addr, data, size );
    }
*/
    else {
      sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
      ErrMsg("Rteserv2Controller", "exReadBlockMem", msg);
    }

    data_cnt      += size;
    recv_data_ptr += size;
    addr          += size;
  }

  sprintf( mCmdRsp, "%c \n", SUCCESS );
}



///<summary>function to process the Check-Status command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exCheckStat( void )
{
  dmsg("Rteserv2Controller::exCheckStat");
  unsigned int code = 0;
  unsigned int id   = 0;
  sscanf( mCmdRcv, "%d %d", &code, &id );
  Rteserv2IF* rteserv2if_ptr = getRteserv2IfPtr( id );
  dmsg("  code:" << code << " id:" << id << " state:"
        << rteserv2if_ptr->getSimState());

  // HW break
  if ( rteserv2if_ptr->isHitHwBpEx() ||
       rteserv2if_ptr->isHitHwBpAc() ) {
    dmsg("  HW Break");
    sprintf( mCmdRsp, "%c %d 10\n", SUCCESS, id );
  }
  // PC break
  else if ( rteserv2if_ptr->isHitPcBp() ) {
    dmsg("  PC Break");
    sprintf ( mCmdRsp, "%c %d 1\n", SUCCESS, id );
    rteserv2if_ptr->setPcBpFlag( false );
    rteserv2if_ptr->setHaltState();
  }
  // HALT
  else if ( rteserv2if_ptr->isHaltState() ) {
    dmsg("  HALT");
    sprintf( mCmdRsp, "%c %d 4\n", SUCCESS, id );
  }
  else if( (RTE_STATE_STEP == rteserv2if_ptr->getSimState()) &&
           mStopAnyCoreFlag && mpG4CPU->isSyncMode() ) {
    // STEP execution -> stopped at anothe HT
    // return the same state as PC break on Sync-mode
    dmsg("  PC Break or Step");
    sprintf( mCmdRsp, "%c %d 1\n", SUCCESS, id );
    rteserv2if_ptr->setHaltState();
  }
  // RUN
  else {
    dmsg("  Running");
    sprintf( mCmdRsp, "%c %d 0\n", SUCCESS, id );
  }
}



///<summary>function to process the Read-Register command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exReadReg( void )
{
  dmsg("Rteserv2Controller::exReadReg");
  unsigned int code     = 0;
  unsigned int id       = 0;
  unsigned int ghsRegId = 0;
  unsigned int cfRegId  = 0;
  unsigned int data     = 0;
  RegData64    data64   = 0;
  char         name[32];
  char         context[32];
  sscanf( mCmdRcv, "%d %d %d", &code, &id, &ghsRegId );
  dmsg("  code:0x" << hex << code << "  id:0x" << id << "  ghsRegId:0x" << ghsRegId);

  unsigned int peid = RTE_PEID(id);

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    // Program Register
    if ( (RTE_GREG_START <= ghsRegId) && (RTE_PC_ID >= ghsRegId) ) {
      mpForestUtil->RegReadDebug ( peid, (CReg)ghsRegId, &data64 );
      data = data64 & 0xFFFFFFFFUL;
    }
    // Vector Register
    else if ( (RTE_VREG_START <= ghsRegId) && (RTE_VREG_END >= ghsRegId) ) {
      // Not supported because this will be implemented when SIMD will be supported in CForest.
      // When this is supported, it must be considered how to access 64bit-register.
    }
    // System Register
    else if ( (RTE_SREG_START <= ghsRegId) && (RTE_SREG_END >= ghsRegId) ) {
      mpSysRegInfo->getSysRegInfo( ghsRegId, &cfRegId, name, context );
      dmsg("  ghsRegId:" << ghsRegId << " cfRegId:" << cfRegId
           << " sreg name:" << name << " context:" << context);
      if ( 0 == strcmp("T", context) ) { // registers belong to HT or NC only
        mpForestUtil->RegReadDebug ( peid, (CReg)cfRegId, &data64 ); 
        data = data64 & 0xFFFFFFFFUL;
      }
      else if ( 0 == strcmp("V", context) ) { // registers belong to VM
        mpForestUtil->RegReadDebug ( peid, (CReg)cfRegId, &data64 ); 
      }
      else { // undefined system-register(context == "-")
        // do nothing (read value=0)
      }
    }
    else { // registers belong to no area
      // do nothing (read value=0)
    }
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    data = mpFastIssIF->readReg( peid, tcid, ghsRegId );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exReadReg", msg);
  }

  dmsg("  data:0x" << hex << data);
  sprintf( mCmdRsp, "%c %d \n", SUCCESS, data );
  dmsg("  mCmdRsp:" << mCmdRsp);
}



///<summary>function to process the Write-Register command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exWriteReg( void ) {
  dmsg("Rteserv2Controller::exWriteReg");
  unsigned int code     = 0;
  unsigned int id       = 0;
  unsigned int ghsRegId = 0;
  unsigned int data     = 0;
  unsigned int cfRegId  = 0;
  char         name[32];
  char         context[32];

  sscanf( mCmdRcv, "%d %d %d %d", &code, &id, &ghsRegId, &data );
  dmsg("  code:" << code << "  id:" << id << "  ghsRegId:" << ghsRegId << " data:0x" << hex << data);

  unsigned int peid = RTE_PEID(id);

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    // Program Register
    if ( RTE_GREG_START <= ghsRegId && RTE_PC_ID >= ghsRegId ) {
      mpForestUtil->RegWriteDebug ( peid, (CReg)ghsRegId, (RegData64)data); 
    }
    // Vector Register
    else if ( RTE_VREG_START <= ghsRegId && RTE_VREG_END >= ghsRegId ) {
      // Not supported because this will be implemented when SIMD will be supported in CForest.
      // When this is supported, it must be considered how to access 64bit-register.
    }
    // System Register
    else if ( RTE_SREG_START <= ghsRegId && RTE_SREG_END >= ghsRegId ) {
      mpSysRegInfo->getSysRegInfo( ghsRegId, &cfRegId, name, context );
      dmsg("  ghsRegId:" << ghsRegId << " cfRegId:" << cfRegId
           << " sreg name:" << name << " context:" << context);
      if ( 0 == strcmp("T", context) ) {
        mpForestUtil->RegWriteDebug ( peid, (CReg)cfRegId, (RegData64)data);
      }
      else if ( 0 == strcmp("V", context) ) {
        mpForestUtil->RegWriteDebug ( peid,  (CReg)cfRegId, (RegData)data);
      }
      else { // undefined system-register(context == "-")
        // do nothing (no write)
      }
    }
    else { // registers belong to no area
      // do nothing (no write)
    }
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    data = mpFastIssIF->writeReg( peid, tcid, ghsRegId, data );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exWriteReg", msg);
  }

  sprintf( mCmdRsp, "%c %d \n", SUCCESS, data );
  dmsg("  mCmdRsp:" << mCmdRsp);
}



///<summary>function to process the STEP command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exSingleStep( void ) {
  dmsg("Rteserv2Controller::exSingleStep");
  unsigned int code    = 0;
  unsigned int id      = 0;
  unsigned int runCnt  = 0;
  SIM_MODE_T   simMode = mpG4CPU->GetSimMode();

  sscanf( mCmdRcv, "%d %d ", &code, &id );

  for ( unsigned int i = 0; i < mRteserv2IFNum; i++ ) {
    if ( RTE_STATE_RUN == (mpRteserv2If[i]).getSimState() ) {
       runCnt++;
    }
  }

  if ( mpG4CPU->isSyncMode() && 
       ((runCnt != mRteserv2IFNum-1) ||
        (RTE_STATE_HALT != (getRteserv2IfPtr(id))->getSimState())) ) {
    // do nothing (STEP should be executed when the target PE's state is HALT and others are RUN in SyncMode.)
  }
  else if ( SIM_MODE_CA == simMode ) {
    (getRteserv2IfPtr(id))->setStepState();
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    mpFastIssIf->step( RTE_PEID( id ), RTE_TCID( id ) );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exSingleStep", msg);
  }
  sprintf( mCmdRsp, "%c \n", SUCCESS );
}



///<summary>function to process the Set-SW-BreakPoint command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exSetPcBp( void ){
  dmsg("Rteserv2Controller::exSetPcBp");
  unsigned int code = 0;
  unsigned int id   = 0;
  unsigned int addr = 0;
  sscanf( mCmdRcv, "%d %d %d", &code, &id, &addr );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    (getRteserv2IfPtr(id))->setPcBreak( addr );
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    mpFastIssIF[ RTE_PEID( id ) ]->insert_bkpt( addr );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exSetPcBp", msg);
  }
  sprintf( mCmdRsp, "%c \n", SUCCESS );
}



///<summary>function to process the Remove-SW-BreakPoint command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exRemovePcBp( void ){
  dmsg("Rteserv2Controller::exRemovePcBp");
  unsigned int code = 0;
  unsigned int id   = 0;
  unsigned int addr = 0;
  sscanf( mCmdRcv, "%d %d %d", &code, &id, &addr );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    (getRteserv2IfPtr(id))->rmvPcBreak( addr );
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    mpFastIssIF[ RTE_PEID( id ) ]->clear_bkpt( addr );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exRemovePcBp", msg);
  }

  sprintf( mCmdRsp, "%c \n", SUCCESS );
}



///<summary>function to process the Set-HW-BreakPoint command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exSetHwBp( void ){
  dmsg("Rteserv2Controller::exSetHwBp");
  bool           isFail   = false;
  unsigned int   code     = 0;
  unsigned int   id       = 0;
  unsigned int   kind     = 0;
  unsigned int   addr     = 0;
  unsigned int   addrMask = 0;
  unsigned int   attr     = 0;
  unsigned int   size     = 0;
  unsigned int   count    = 0;
  unsigned int   hwBpId   = 0;
  AccessAttr     accAttr  = ACC_READ;
  char msg[32];

  sscanf( mCmdRcv, "%d %d %d %d %d %d %d %d ", 
          &code, &id, &kind, &addr, &addrMask, &attr, &size, &count);

  Rteserv2IF*  pRtesrv2if = getRteserv2IfPtr( id );
  SIM_MODE_T   simMode    = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) { // CForest
    if ( RTE_HWB_KIND_EXE == (RTE_HWB_KIND_T)kind ) { // Execution Break
      pRtesrv2if->setHwBreakEx( id, addr, addrMask, attr, &hwBpId );
      // if the kind of break is execution break, Count number is equal to attr
    }
    else if ( RTE_HWB_KIND_ACC == (RTE_HWB_KIND_T)kind ) { // Access Break
      switch ( attr ) {
      case 1 : accAttr = ACC_READ;      break;
      case 2 : accAttr = ACC_WRITE;     break;
      case 3 : accAttr = ACC_READWRITE; break;
      default: isFail  = true;          break;
      }
      if ( !isFail ) {
        pRtesrv2if->setHwBreakAc( id, addr, addrMask, accAttr, size, count, &hwBpId );
      }
      else {
        sprintf(msg, "HW Break attribute(%d) is invalid.", attr);
        ErrMsg("Rteserv2Controller", "exSetHwBp", msg);
      }
    }
    else {
      isFail = true;
      sprintf(msg, "HW Break kind(%d) is invalid.", kind);
      ErrMsg("Rteserv2Controller", "exSetHwBp", msg);
    }
  }
  else if ( SIM_MODE_FAST == simMode ) { // Fast ISS
    isFail = true; // HW-Break is not supported in FastISS
  }
  else {
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exSetHwBp", msg);
  }

  if ( isFail ) {
    sprintf( mCmdRsp, "%c %d\n", FAILURE, id );
  }
  else {
    sprintf( mCmdRsp, "%c %d %d\n", SUCCESS, id, hwBpId );
  }
}



///<summary>function to process the Remove-HW-BreakPoint command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exRemoveHwBp( void ){
  dmsg("Rteserv2Controller::exRemoveHwBp");
  bool         isFail  = false;
  unsigned int code    = 0;
  unsigned int id      = 0;
  unsigned int hwBrkId = 0;

  sscanf( mCmdRcv, "%d %d %d ", &code, &id, &hwBrkId );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    isFail = (getRteserv2IfPtr(id))->rmvHwBreak( hwBrkId );
  }
  else if ( SIM_MODE_FAST == simMode ) {
    isFail = true; // HW-Break is not supported in FastISS
  }
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exRemoveHwBp", msg);
  }

  if ( isFail ) {
    sprintf( mCmdRsp, "%c %d\n", FAILURE, id );
  }
  else {
    sprintf( mCmdRsp, "%c %d\n", SUCCESS, id );
  }
}



///<summary>function to process the Remove-All-HW-BreakPoint command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exRemoveAllHwBp( void ){
  dmsg("Rteserv2Controller::exRemoveAllHwBp");
  bool         isFail = false;
  unsigned int code   = 0;
  unsigned int id     = 0;

  sscanf( mCmdRcv, "%d %d ", &code, &id );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    isFail = (getRteserv2IfPtr(id))->rmvAllHwBreak();
  }
  else if ( SIM_MODE_FAST == simMode ) {
    isFail = true; // HW-Break is not supported in FastISS
  }
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exRemoveAllHwBp", msg);
  }

  if ( isFail ) {
    sprintf( mCmdRsp, "%c %d\n", FAILURE, id );
  }
  else {
    sprintf( mCmdRsp, "%c %d\n", SUCCESS, id );
  }
}



///<summary>function to process the get Information-HW-BreakPoint command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exInfoHwBp( void ){
  dmsg("Rteserv2Controller::exInfoHwBp");
  bool         isFail = false;
  unsigned int code   = 0;
  unsigned int id     = 0;
  unsigned int bpid   = 0;
  unsigned int attr   = 0;

  sscanf( mCmdRcv, "%d %d ", &code, &id );
  Rteserv2IF* rteserv2if_ptr = getRteserv2IfPtr( id );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    isFail = !(rteserv2if_ptr->isHitHwBp(&bpid, &attr));
    // the flag of HW-Break is inactivated in isHitHwBp()
  }
  else if ( SIM_MODE_FAST == simMode ) {
    isFail = true; // HW-Break is not supported in FastISS
  }
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exInfoHwBp", msg);
  }

  if ( isFail ) {
    sprintf( mCmdRsp, "%c %d\n", FAILURE, id );
  }
  else {
    sprintf( mCmdRsp, "%c %d %d %d\n", SUCCESS, id, bpid, attr );
  }
}



///<summary>function to process the Continue(RUN) command</summary>
///<param name="none"></param>
///<returns>none</returns>
void
Rteserv2Controller::exCont( void )
{
  dmsg("Rteserv2Controller::exCont");
  unsigned int code = 0;
  unsigned int id   = 0;

  sscanf( mCmdRcv, "%d %d ", &code, &id );
  dmsg("  code:" << code << "  id:" << id);
  sprintf( mCmdRsp, "%c\n", SUCCESS );

  SIM_MODE_T simMode = mpG4CPU->GetSimMode();

  if ( SIM_MODE_CA == simMode ) {
    Rteserv2IF* rteserv2if_ptr = getRteserv2IfPtr( id );
    rteserv2if_ptr->setRunState();
    mLastRunPeid = RTE_PEID( id );
  }
/* for FastISS
  else if ( SIM_MODE_FAST == simMode ) {
    unsigned int peid = RTE_PEID( id );
    unsigned int tcid = RTE_TCID( id );
    mpFastIssIf->cont( peid, tcid );
  }
*/
  else {
    char msg[32];
    sprintf(msg, "Invalid simulation mode(%d) has been set.", simMode);
    ErrMsg("Rteserv2Controller", "exCont", msg);
  }
}

