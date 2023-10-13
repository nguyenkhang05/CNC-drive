/*************************************************************************
 * Copyright(c) 2016-2017 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  NSMVMEVV02_H
#define  NSMVMEVV02_H

#include "systemc.h"
#include "global.h"
#include "OSCI2.h"

template<unsigned int BUSWIDTH> class BACKWARD_DECODER;
template<unsigned int BUSWIDTH> class OSCI2DCDR;
class G4_MevAdptr;
class G4_MevArbtr;
class G4_G0Mev;

class NSMVMEVV02:  public sc_module
{
public:

#ifdef DEBUG_MESSAGE_OUTPUT
  static int countObj;
#endif // DEBUG_MESSAGE_OUTPUT

  /* ports for system */
  sc_in<bool>   resetz; // reset port
  sc_in<uint64> clk;    // clock port

  /* port for APB BUS */
  tlm::tlm_target_socket<BUS_WIDTH_VCI32,tlm::tlm_base_protocol_types,0>* ts; // APB BUS target socket
  /* using tlm::tlm_target_socket in order to use "*.MEV.ts" as target socket name in BUS.map file. */

  /* constructor */
  SC_HAS_PROCESS( NSMVMEVV02 );
  NSMVMEVV02( sc_module_name name, const char* pCfgFileName );

  /* destructor */
  virtual ~NSMVMEVV02( void );

  /* set function */
  void setClkDivision( unsigned int peid, unsigned int clkDiv );
  void setReadRegLatency ( uint64 latency );
  void setWriteRegLatency( uint64 latency );
  void setPriority( unsigned int peid, unsigned int priority );
  void rmvPriority( unsigned int peid );
  void clrPriority( void );
  void setRegAccSize( unsigned int size );
  void clrRegAccSize( void );

  /* public functions */

  /* public variables */
  const char* mpModName; // module name

private:
  /* private variables */
  char          mErrMsg[128]; // for error message
  unsigned int  mPeNum;       // number of PE
  char*         mpMapName;    // name of MEV map file
  bool          mElbFlag;

  BACKWARD_DECODER<BUS_WIDTH_VCI32>* mpBwDec; // backword decoder
  OSCI2DCDR<BUS_WIDTH_VCI32>*        mpDec;   // decoder
  G4_MevAdptr*                        mpAdp;   // adapter
  G4_MevArbtr**                       mpArb;   // arbitor
  G4_G0Mev**                          mpG4_G0Mev; // register

  /* private functions */
  void end_of_elaboration( void );
  void readCfgFile( const char *filename );
  void resetFunc( void );

  /* processes */
  void ResetProc( void );
};

#endif // NSMVMEVV02_H
