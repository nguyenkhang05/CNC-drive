/*************************************************************************
 * $Id: NSMVINTC2V02.h,v 1.6 2020/03/24 10:02:05 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_NSMVINTC2V02_H
#define  G4_NSMVINTC2V02_H

#include <list>
#include <set>
#include "systemc.h"
#include "global.h"
#include "G4_intc_def.h"
#include "G4_intc_typ.h"
#include "BusSlaveBase.h"
#include "BusTimeBase.h"

class G4_Intc2Func;
class PeidInfo;

class NSMVINTC2V02 : public sc_module, public BusSlaveBase<BUS_WIDTH_APB, 1>, public BusTimeBase
{
public:
  /* ports for system */
  sc_in<bool>   busresetz; // reset port
  sc_in<uint64> busclk;    // clock port

  /* ports for peripheral */
  sc_in<bool>* eiintl[INTC2_CH_MAX_NUM];     // EIINT request port
  sc_in<bool>* eiint_type[INTC2_CH_MAX_NUM]; // EIINT detection type port

  /* ports for INTC1 */
  sc_out<uint64>*      i2_g_eiint[PE_MAX_NUM];
  sc_out<bool>*        i2_bcastrq[INTC_BCAST_NUM];
  sc_in<unsigned int>* i1_p_intack[PE_MAX_NUM];

  /* port for CPUs info */
  sc_in<unsigned int>* cpu_gm_info[PE_MAX_NUM];

  /* ports for  APB BUS */
  TlmTargetSocket<BUS_WIDTH_APB>* ts;


  SC_HAS_PROCESS( NSMVINTC2V02 );
  /* constructor */
  NSMVINTC2V02( sc_module_name name, const char* pCfgFileName );
  /* destructor */
  virtual ~NSMVINTC2V02( void );

  /* set function */
  void setReadRegLatency  ( uint64 rLatency );
  void setWriteRegLatency ( uint64 wLatency );
  void setInterruptLatency( uint64 iLatency );
  void setTraceEnable( sc_trace_file* tf );

  /* public functions */
  void setEiintReqPort( unsigned int peid, uint64 val, G4IntcTyp::PRI_PROC_T procPri );
  void setBcastReqPort( unsigned int portNum, bool   val, G4IntcTyp::PRI_PROC_T procPri, bool isReset );
  void notifyExecEvent( void );
  bool getCPUGM (unsigned int cpu_index);
  unsigned int getCPUGPID (unsigned int cpu_index);

  const char*             mpModName;   // module name
  std::set<unsigned int>  mReEntryReqList; // list of the low-priority requests for re-entry into the queue
  void notifyResendEiintReqEvent( void );
  bool resendEiint2Forbid;

private:
  /* private variables */
  G4_Intc2Func*           mpIntc2Func; // pointer to Intc2Func instance
  bool                    mPrevEiintl[INTC2_CH_MAX_NUM]; // previous value of EIINT request
  unsigned int            mPrevIntack[PE_MAX_NUM]; // previous value of EIINT acknowledge
  unsigned int            mPeNum;    // number of PE
  unsigned int            mIntc2Type;// type of INTC2 (for CC-Cube or Standard)
  std::vector<PeidInfo*>  mPeidInfo; // vector of PeidInfo pointer(PeidInfo is dummy of CForest ProcElement class)
  sc_event                meEiReqForLv[INTC2_CH_MAX_NUM]; // event to drive EiintReqProc() process for high level detection type
  sc_event                meExec;      // event to drive ExecProc() process
  sc_event                meResendEiintReq;      // event to drive ResendEiintProc() process
  uint64                  mILatency;   // latency of interrupt request
  char                    mErrMsg[64]; // for error message
  bool                    mCPUGM[PE_MAX_NUM];   // Indicate current mode of CPUs <-- added by chanle 20180131 for G4MH2.0
  unsigned int            mCPUGPID[PE_MAX_NUM]; // Indicate GPID of CPUs <-- added by chanle 20180131 for G4MH2.0

  G4IntcTyp::PortAccInfo<uint64> mEiReqInfo[PE_MAX_NUM]; // write data to EIINT request port
  G4IntcTyp::PortAccInfo<bool>   mBcastInfo[INTC_BCAST_NUM];   // write data to broadcast request port
  std::set<uint64>               mEiCancelInfo[PE_MAX_NUM]; // cancel information
  sc_event meSetEiReq; // event to drive SetEiintReqPortProc() process
  sc_event meSetBcast; // event to drive SetBcastReqPortProc() process

  /* private function */
  void readCfgFile( const char *filename );
  void initReg( void );

  /* processes */
  void InitProc( void ); /* initial settings for Dynamic Process */
  void ResetProc( void );
  void EiintReqProc( unsigned int );  /* Dynamic Process */
  void EiintAckProc( void );
  /* following "Set***PortProc()" are write port processes to avoid the error of a sc_signal
     driven by plural processes execution process. */
  void SetEiintReqPortProc( void );
  void SetBcastReqPortProc( void );
  void ExecProc( void );
  void ResendEiintProc(void);
  void MonitorCPUInfoMethod( void );
};

#endif // G4_NSMVINTC2V02_H
