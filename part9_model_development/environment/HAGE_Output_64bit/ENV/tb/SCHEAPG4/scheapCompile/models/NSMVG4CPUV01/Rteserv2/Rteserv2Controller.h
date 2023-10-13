/*
 * Copyright(c) 2013-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#ifndef _RTESERV2CONTROLLER_H_
#define _RTESERV2CONTROLLER_H_

#include <set>
#include <map>

#ifdef WINDOWS_DEF
#  include <windows.h>
#endif // WINDOWS_DEF

class  NSMVG4CPUV01;
class  G4_NoDbgIF;
class  Rteserv2IF;
class  ForestUtilApi;
//class  FastIssIF; // for FastISS
struct BreakInfo;
class  SysRegInfo;

class Rteserv2Controller : public G4_NoDbgIF
{
public:
  Rteserv2Controller( const char*    pCfgFileName,
                      NSMVG4CPUV01* pG4CPU );
  ~Rteserv2Controller( void );

  void           start( void );
  NSMVG4CPUV01* getG4CpuPtr( void );
  ForestUtilApi* getForestUtilPtr( void );
//  FastIssIF*     getFastIssPtr( void ); // for FastISS

private:
  char           mMultiHostAddr[1024]; // host address of rteserv2
  unsigned int   mMultiPort;       // port number of rteserv2
  char           mCmdRcv[4104];    // receive command from rteserv2
  char           mCmdRsp[4104];    // response to rteserv2
  int            mSockFd;          // file descripter bit of socket
  NSMVG4CPUV01* mpG4CPU;         // pointer of NSMVG4CPUV01
/*  FastIssIF*     mpFastIssIf;  for FastISS */
  Rteserv2IF*    mpRteserv2If;     // pointer of Rteserv2IF
  unsigned int   mRteserv2IFNum;   // total number of Rteserv2IF
  bool           mStopAnyCoreFlag; // flag which shows that there is any HALT state thread
  bool           mPeripheralFlag;  // true: connected peripheral, false: cpu core only
  bool           mRunningFlag;     // flag which shows that simulation is running or not
  unsigned int   mLastRunPeid;     // peid of last CONT command from rteserv2
  BreakInfo*     mpBreakInfoCA;    // pointer to break information class in CA ISS
/*  BreakInfo*     mpBreakInfoFast;  for FastISS */
  SysRegInfo*    mpSysRegInfo;     // pointer to system register information class
  std::map<unsigned int, unsigned int> mBpOnPcLst; // list of BP on PC

#ifdef LINUX_DEF
  int            inet_pton( int family, const char* pStr, void*  pAddr );
#endif // LINUX_DEF
#ifdef WINDOWS_DEF
  int            inet_pton( int family, const char* pStr, ULONG* pAddr );
#endif // WINDOWS_DEF
  void           initSocket( void );
  void           readCfgFile( const char* pCfgFileName );
  void           receiveCommand( void );
  void           processCommand( void );
  void           sendResponse( void );
  void           createRteserv2If( void );
  Rteserv2IF*    getRteserv2IfPtr( unsigned int cpuId );
  void           getRteserv2IfIdx( unsigned int peId,
                                   unsigned int vcId,
                                   std::set<unsigned int>* pIdxLst );
  void           setHaltState( void );
  bool           checkBreak( void );
  bool           rmvBpOnPc( void );
  void           setBpOnPc( void );
  void           setSysRegInfo( void );
  void           ctrlSim( void );

  // functions for each command from rteserv2
  void           exHalt( void );          // HALT
  void           exQuit( void );          // QUIT
  void           exReset( void );         // RESET
  void           exReadMem( void );       // READMEM
  void           exWriteMem( void );      // WRITEMEM
  void           exReadBlockMem( void );  // READBLOCKMEM
  void           exWriteBlockMem( void ); // WRITEBLOCKMEM
  void           exCheckStat( void );     // CHECKSTAT
  void           exReadReg( void );       // READREG
  void           exWriteReg( void );      // WRITEREG
  void           exSingleStep( void );    // SINGLESTEP
  void           exSetPcBp( void );       // SETBP
  void           exRemovePcBp( void );    // REMOVEBP
  void           exSetHwBp( void );       // SETHWBP
  void           exRemoveHwBp( void );    // REMOVEHWBP
  void           exRemoveAllHwBp( void ); // REMOVEALLHWBP
  void           exInfoHwBp( void );      // INFOHWBP
  void           exCont( void );          // CONT
};

#endif // _RTESERV2CONTROLLER_H_
