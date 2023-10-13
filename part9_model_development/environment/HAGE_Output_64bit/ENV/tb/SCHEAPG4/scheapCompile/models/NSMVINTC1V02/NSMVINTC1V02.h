/*************************************************************************
 * $Id: NSMVINTC1V02.h,v 1.8 2020/03/24 09:58:12 hadinhtran Exp $
 * Copyright(c) 2016-2020 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *************************************************************************/

#ifndef  G4_NSMVINTC1V02_H
#define  G4_NSMVINTC1V02_H

#include <map>
#include <vector>
#include <list>
#include <set>
#include "systemc.h"
#include "global.h"
#include "G4_intc_def.h"
#include "G4_intc_typ.h"
#include "BusSlaveBase.h"
#include "BusTimeBase.h"

class G4_Intc1Func;
class IntReqInfoBase;
class TRegEIC;
class TRegEEIC;
class TRegEIBG;
class EIBD;
template <class TR> class TRegBND;
class Intc2Dummy;
class PeidInfo;

class Intc2ReqInfo {
public:
  Intc2ReqInfo( unsigned int intc2_type );
  ~Intc2ReqInfo( void );
  void            reset( void );
  void            setReqInfo( G4IntcTyp::EiintReq intReq, unsigned int pe_id);
  IntReqInfoBase* getReqInfo( unsigned int channel );
  TRegEIC*        getEicPtr ( unsigned int channel );
  TRegEEIC*       getEeicPtr ( unsigned int channel );
  TRegEIBG*       getEibgPtr (unsigned int peid);
#ifndef __VLAB_MIT__
  TRegBND<EIBD>*  getEibdPtr( unsigned int channel );
#endif // __VLAB_MIT__

  Intc2Dummy* mpIntc2Dummy; // dummy of CForest INTC2

private:
  void initReg( void );

  char mErrMsg[64];  // for error message
  std::vector<PeidInfo*>* mpPesDummy;    // vector of PEID information(declared in CForest INTC2)
  PeidInfo*               mpPeInfoDummy; // dummy of CForest ProcElement
};


class NSMVINTC1V02 : public sc_module, public BusSlaveBase<BUS_WIDTH_VCI32, 1>, public BusTimeBase  
{
public:
  /* ports for system */
  sc_in<bool>   resetz; // reset port
  sc_in<uint64> clk;    // clock port

  /* ports for peripheral */
  sc_in<bool>* eiintl[INTC1_CH_NUM];     // EIINT request port
  sc_in<bool>* eiint_type[INTC1_CH_NUM]; // EIINT detection type port
  sc_in<bool>  fenmi; // FENMI request port
  sc_in<bool>* feint[INTC1_FEINT_CH_NUM]; // FEINT request port
  sc_in<bool>* feint_type[INTC1_FEINT_CH_NUM]; // FEINT detection type port

  /* ports for CPU */
  sc_out<uint64>              t_eiint_1l; // EIINT request port
  sc_in<unsigned int>         t_intack_1l; // EIINT acknowledge port
  sc_out<bool>                fenmirq_1l; // FENMI request port
  sc_in<bool>                 fenmiack_1l; // FENMI acknowledge port
  sc_out<unsigned int>        t_feintrq_1l; // FEINT request port
  sc_in<unsigned int>         t_feintack_1l; // FEINT acknowledge port
  sc_in<unsigned int>         cpu_gm_info; // CPU GM/GPID notify port (added by yoshinaga for G4MH 2.0)

  /* ports for INTC2 */
  sc_in<uint64>         *i2_g_eiint; // EIINT request port
  sc_out<unsigned int>  *i1_t_intack; // EIINT acknowledge port

  /* port for APB BUS */
  TlmTargetSocket<BUS_WIDTH_VCI32>* ts; // APB BUS target socket 


  /* constructor */
  SC_HAS_PROCESS( NSMVINTC1V02 );
  NSMVINTC1V02( sc_module_name name, const char* pCfgFileName, unsigned int peid );

  /* destructor */
  virtual ~NSMVINTC1V02( void );

  /* set function */
  void setReadRegLatency  ( uint64 rLatency );
  void setWriteRegLatency ( uint64 wLatency );
  void setInterruptLatency( uint64 iLatency );
  void setTraceEnable( sc_trace_file* tf );

  /* public functions */
  void setEiintReqPort( uint64 val, G4IntcTyp::PRI_PROC_T procPri );
  void setEiintAckPort( unsigned int val, G4IntcTyp::PRI_PROC_T procPri );
  void setFenmiReqPort( bool val, G4IntcTyp::PRI_PROC_T procPri );
  void setFeintReqPort( unsigned int val, G4IntcTyp::PRI_PROC_T procPri );
  void notifyExecEvent( void );
  void setFeintForceCanceFlag( bool isSet ); 
  bool getCPUGM(void);
  unsigned int getCPUGPID(void);

  /* public variables */
  const char*             mpModName;   // module name
  void resendEiintReqPort( void );
  void resendFeintReqPort( void );
  bool resendEiintForbid;
  bool resendFeintForbid;
  /******************************************************************************
   * CForest INTC1 refers the information of interrupt from INTC2
   * through the pointer to INTC2 object directly.
   * But SC-HEAP INTC1 is separated from INTC2.
   * Therefore, INTC1 stores the data of EIINT request from INTC2 in data-base.
   ******************************************************************************/
  Intc2ReqInfo            *mpIntc2ReqInfo; // pointer to the data-base of EIINT request from INTC2
  std::set<unsigned int> mReEntryReqList; // list of the low-priority requests for re-entry into the queue
protected:
  /* protected variables */
  G4_Intc1Func*    mpIntc1Func; // pointer to G4_Intc1Func instance

private:
  /* private variables */
  uint64        mPrevI2EiReq; // previous value of EIINT(from INTC2) request
  unsigned int  mPrevI2EiAck; // previous value of EIINT(from INTC2) acknowledge
  bool          mPrevExEiReq[INTC1_CH_NUM]; // previous value of EIINT(from peripheral) request
  bool          mPrevExFnReq; // previous value of FENMI request
  bool          mPrevExFiReq[INTC1_FEINT_CH_NUM]; // previous value of FEINT request
  bool          mPrevExFiAck; // previous value of FEINT acknowledge
  bool          mFeintCancelForce; // force cancel flag for Feint 
  unsigned int  mPeid;       // PEID
  unsigned int  mIntc2Type;  // type of INTC2 (for CC-Cube or Standard)
  uint64        mILatency;   // latency of interrupt request
  char          mErrMsg[64]; // for error message
  bool          mCPUGM;      // Indicate current mode of CPU <-- added by chanle 20180131 G4MH2.0
  unsigned int  mCPUGPID;    // Indicate GPID of CPU <-- added by chanle 20180131 G4MH2.0

  // added by Yoshinaga for G4MH 2.0 (new cforestg4)
  unsigned int  mCoreVersion; // core version (for cforestg4)
  unsigned int  mHvcfgIniVal; // Initial value of HVCFG

  G4IntcTyp::PortAccInfo<uint64>       mEiReqInfo; // write data to EIINT request port
  G4IntcTyp::PortAccInfo<unsigned int> mEiAckInfo; // write data to EIINT acknowledge port
  G4IntcTyp::PortAccInfo<bool>         mFnReqInfo; // write data to FENMI request port
  G4IntcTyp::PortAccInfo<unsigned int>         mFiReqInfo; // write data to FEINT request port
  std::set<uint64>                   mEiCancelInfo; // cancel information
  std::set<uint64>                   mFeintCancelInfo; // cancel information 
  sc_event meSetEiReq; // event to drive SetEiReqPortProc() process
  sc_event meSetEiAck; // event to drive SetEiAckPortProc() process
  sc_event meSetFnReq; // event to drive SetFnReqPortProc() process
  sc_event meSetFiReq; // event to drive SetFiReqPortProc() process
  sc_event meExec;     // event to drive ExecProc() process
  sc_event meEiReq1ForLvDetect; // High-level detection event for eiint(from peripheral)
  sc_event meFiReqForLvDetect;  // High-level detection event for feint

  /* private function */
  void readCfgFile( const char *filename );
  void initReg( void );

  /* processes */
  void ResetProc( void );
  void I2EiintReqProc( void );
  void EiintAckProc( void );
  void ExEiintReqProc( void );
  void ExFenmiReqProc( void );
  void ExFenmiAckProc( void );
  void ExFeintReqProc( void );
  void ExFeintAckProc( void );
  /* following "Set***PortProc()" are write port processes to avoid the error of a sc_signal
     driven by plural processes execution process. */
  void SetEiintReqPortProc( void );
  void SetEiintAckPortProc( void );
  void SetFenmiReqPortProc( void );
  void SetFeintReqPortProc( void );
  void ExecProc( void );
  void MonitorCPUInfoMethod( void );
};

#endif // G4_NSMVINTC1V02_H
