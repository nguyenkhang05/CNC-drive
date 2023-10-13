/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  NSMVIPIRV02_H
#define  NSMVIPIRV02_H

#include <list>
#include "systemc.h"
#include "global.h"
#include "G4_intc_def.h"
#include "BusSlaveBase.h"
#include "BusTimeBase.h"

class  G4_IpirFunc;
struct G4_IpirRegWriteInfo;

class NSMVIPIRV02 : public sc_module, public BusSlaveBase<BUS_WIDTH_APB, 1>, public BusTimeBase
{
  enum DETECT_TYPE_T {
    DETECT_TYPE_EDGE  = 0,
    DETECT_TYPE_LEVEL = 1
  };

public:
  /* ports for system */
  sc_in<bool>   resetz; // reset port
  sc_in<uint64> clk;    // clock port

  /* ports for OR circuit */
  sc_out<bool>* ipir_int_pe_ch[PE_MAX_NUM][INTC_IPIR_CH_NUM]; // EIINT request port

  /* port for APB BUS */
  TlmTargetSocket<BUS_WIDTH_APB>* ts; // APB BUS target socket

  /* constructor */
  SC_HAS_PROCESS( NSMVIPIRV02 );
  NSMVIPIRV02( sc_module_name name, const char* pCfgFileName );

  /* destructor */
  virtual ~NSMVIPIRV02( void );

  /* set function */
  void setDetectTypeEdge ( void );
  void setDetectTypeLevel( void );
  void setRegAccSize( unsigned int ch, unsigned int size );
  void clrRegAccSize( unsigned int ch );

  /* public functions */
  void setWriteInfo( G4_IpirRegWriteInfo* pInfo );
  void reqInt( BusTime_t* pTime, bool debug );

  /* public variables */
  const char* mpModName; // module name

private:
  /* private variables */
  G4_IpirFunc*  mpIpirFunc;  // pointer to IpirFunc instance
  char          mErrMsg[64]; // for error message
  unsigned int  mPeNum;      // number of PE
  double        mPulseWidth; // width of interrupt pulse
  DETECT_TYPE_T mDetectType; // detection-type
  bool          mDebugFlag;  // flag for debug-write
  sc_event      meIntReq;    // event to activate IntReqProc process

  std::list<G4_IpirRegWriteInfo> mRegWInfoLst; // list of register write information

  /* private function */
  void   readCfgFile( const char *filename );
  void   resetFunc( void );
  void   setIntPort( void );
  void   clearIntPort( void );
  void   clearReg( void );
  double getClkWidth( void );

  /* processes */
  void InitProc( void );
  void ResetProc( void );
  void IntReqProc( void );
};

#endif // NSMVIPIRV02_H
