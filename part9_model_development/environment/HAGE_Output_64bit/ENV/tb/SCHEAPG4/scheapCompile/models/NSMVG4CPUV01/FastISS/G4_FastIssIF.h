/*
* Copyright(c) 2015-2018 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef G4_FASTISSIF_H
#define G4_FASTISSIF_H

#include "rh850g4/core.h"       // for rh850g4::core class
#include "rh850g4/memory_wrapper.h" // for rh850::memory_wrapper class
#include "rh850g4/execution_tracer.h" // for execution_tracer class
#include "global.h"
#include "g4cpu_typ.h"


class G4_FastIssIF
{
  
 public:
  rh850g4::core* mpFastIss_ASTC[PE_MAX_NUM]; // pointer of fast ISS instance
  rh850g4::memory_wrapper* mpFastIssMemWrapper_ASTC[PE_MAX_NUM]; // pointer of fast ISS memory wrappers

 protected:
  unsigned int mPeNum;            // PE number 
  CPU_KIND_T mPeType[PE_MAX_NUM]; // PE type(ex:G4MH)
  FILE *mFpTrace;                 // pointer of Trace file
  rh850g4::execution_tracer* ex_trace[PE_MAX_NUM]; // pointer of execution_tracer instance
  
 private:
  g4cpu_pe_info mPeInfo[PE_MAX_NUM]; // for save CPU information

 public:
  G4_FastIssIF( unsigned int peNum,  FILE* fp_trace );
  ~G4_FastIssIF();
  void CancelContEvent (unsigned int pe_id);
  void SetContEvent (unsigned int pe_id);
  void TestResetLog (void);
  void SetSyncPeriod (unsigned int period, sc_core::sc_time_unit unit); 
  void SetBlockSize ( unsigned int pe_id, unsigned int size );
  void SetTrcTitle ( void );
  void SetCpuInfo ( unsigned int pe_id, CPU_KIND_T type, g4cpu_pe_info* pe_info );
  void SetSwd0pcadValue( unsigned int pe_id, unsigned int value );
  void SetSwdtEnable( unsigned int pe_id, bool mode );
  void setLinkFuncEnable( bool mode );
  void SetSnzTime( unsigned int pe_id, unsigned int snz_time );
  void AttachAnalysisUnit( bool is_old_fromat );
  void DetachAnalysisUnit( void );
  void clearFastissCache( unsigned int pe_id );
  void ResetSysReg ( unsigned int pe_id );
  unsigned int GetSwd0pcadExecCountValue( unsigned int pe_id );
  unsigned char GetVmid( unsigned int pe_id );
  unsigned char GetTcid( unsigned int pe_id );
  unsigned char GetSpid( unsigned int pe_id );
  bool GetUserMode( unsigned int pe_id );
  bool GetVirtualMode( unsigned int pe_id );
  unsigned int GetMemAccEvType( unsigned int pe_id );
  void SetFastIssPtr( unsigned int index, rh850g4::core* prr );
  void SetFastIssMemWrapperPtr( unsigned int index, rh850g4::memory_wrapper* prr );
  unsigned int readGrValue( unsigned int peid, unsigned int regid ); 
  void writeGrValue( unsigned int peid, unsigned int regid, unsigned int value );
  unsigned long long readWrValue( unsigned int peid, unsigned int regid, bool isUpper ); 
  void writeWrValue( unsigned int peid, unsigned int regid, unsigned long long up_value, unsigned long long lo_value );
  unsigned int readSrValue( unsigned int peid, unsigned int regid, unsigned int selid ); 
  void writeSrValue( unsigned int peid, unsigned int regid, unsigned int selid, unsigned int value );

 private:
  unsigned long long readWrLowerValue( unsigned int peid, unsigned int regid ); 
  unsigned long long readWrUpperValue( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid0( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid1( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid2( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid3( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid4( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid5( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid9( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid10( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid11( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid12( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid13( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid14( unsigned int peid, unsigned int regid ); 
  unsigned int readSrSelid15( unsigned int peid, unsigned int regid ); 
  void writeSrSelid0( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid1( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid2( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid3( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid4( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid5( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid9( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid10( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid11( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid12( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid13( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid14( unsigned int peid, unsigned int regid, unsigned int value );
  void writeSrSelid15( unsigned int peid, unsigned int regid, unsigned int value );
}; // end of FastIssIF definition

#endif /* G4_FASTISSIF_H */
