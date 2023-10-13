/*************************************************************************
 * Copyright(c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  NSMVMEVV01_H
#define  NSMVMEVV01_H

#include "systemc.h"
#include "global.h"
#include "OSCI2.h"

template<unsigned int BUSWIDTH> class BACKWARD_DECODER;
template<unsigned int BUSWIDTH> class OSCI2DCDR;
class MevAdptr;
class MevArbtr;
class G0Mev;

class NSMVMEVV01:  public sc_module
{
public:
  /* ports for system */
  sc_in<bool>   resetz; // reset port
  sc_in<uint64> clk;    // clock port

  /* port for APB BUS */
  tlm::tlm_target_socket<BUS_WIDTH_APB,tlm::tlm_base_protocol_types,0>* ts; // APB BUS target socket
  /* using tlm::tlm_target_socket in order to use "*.MEV.ts" as target socket name in BUS.map file. */

  /* constructor */
  SC_HAS_PROCESS( NSMVMEVV01 );
  NSMVMEVV01( sc_module_name name, const char* pCfgFileName );

  /* destructor */
  virtual ~NSMVMEVV01( void );

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

  BACKWARD_DECODER<BUS_WIDTH_APB>* mpBwDec; // backword decoder
  OSCI2DCDR<BUS_WIDTH_APB>*        mpDec;   // decoder
  MevAdptr*                        mpAdp;   // adapter
  MevArbtr**                       mpArb;   // arbitor
  G0Mev**                          mpG0Mev; // register

  /* private functions */
  void end_of_elaboration( void );
  void readCfgFile( const char *filename );
  void resetFunc( void );

  /* processes */
  void ResetProc( void );
};

#endif // NSMVMEVV01_H
