/*
* Copyright(c) 2015 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef G4_CAISS_H
#define G4_CAISS_H

#include "BusMasterBase.h"
#include "BusSlaveBase.h"
#include "BusTimeBase.h"
#include "global.h"
#include "forest_common_api.h"

#define CFOREST_MEM_ACC_MAX 4096

class NSMVG4CPUV01;
class ForestUtilScheap;
class SideBand;
class G4_IsVciIf;
class G4_IsLapbIf;
class G4_IsVpiIf;
class G4_TsIf;

extern sc_time_unit glb_resolution_unit;

class G4_CaIss :
  public sc_module,
  public BusMasterBase<BUS_WIDTH_VCI,(PE_MAX_NUM*4)>,
  public BusSlaveBase<BUS_WIDTH_FLSH,1>,
  public BusSlaveBase<BUS_WIDTH_VCI,2>,
  public BusTimeBase
{

 public:
  sc_in<sc_dt::uint64> *PE_sys_clk[PE_MAX_NUM]; // for Local APB bus' clock port
  sc_in<sc_dt::uint64> From_clk;  // for flash rom's clock port
  sc_in<sc_dt::uint64> Cram_clk;  // for cluster ram's clock port

  sc_in<bool>          sys_reset; // for reset port

  TlmInitiatorSocket<BUS_WIDTH_VCI> *PE_ish[PE_MAX_NUM];  // for H-Bus area
  TlmInitiatorSocket<BUS_WIDTH_VCI> *PE_isgp[PE_MAX_NUM]; // for P-Bus area
  TlmInitiatorSocket<BUS_WIDTH_VCI> *PE_isgc[PE_MAX_NUM]; // for I-Bus area
  TlmInitiatorSocket<BUS_WIDTH_VCI> *PE_isi[PE_MAX_NUM];  // for INTC1

  TlmTargetSocket<BUS_WIDTH_FLSH>   *tsf;                 // for flash I/F
  TlmTargetSocket<BUS_WIDTH_VCI>    *tss;                 // for System bus Master
  TlmTargetSocket<BUS_WIDTH_CRAM>   *tscr;                // for cluster ram I/F

  double mPeriodPE[PE_MAX_NUM]; // for local APB's cycle period
  double mPeriodFrom;           // for flash rom's cycle period
  double mPeriodCram;           // for cluster ram's cycle period

 private:
  NSMVG4CPUV01*  mpG4CPU;                // pointer of NSMVG4CPUV01's instance
  ForestUtilScheap* mpForestUtil;        // for the pointer of ForestUtilScheap's instance
  G4_IsVciIf*    mpIsVciIf;              // for the pointer of IsVciIf's instance (for H-Bus area)
  G4_IsLapbIf*   mpIsLapbIf[PE_MAX_NUM]; // for the pointer of IsLapbIf's instance (for INTC1)
  G4_IsVpiIf*    mpIsVpiIf;              // for the pointer of IsVpiIf's instance (for P-Bus area)
  G4_IsVpiIf*    mpIsVpiIf_gc;           // for the pointer of IsVpiIf's instance (for I-Bus area)
  G4_TsIf*       mpTsIf;                 // for the pointer of TsIf's instance
  double         mClkPeriod;             // for the base clock period
  unsigned int*  mpPeType;               // for each PE's cpu type
  unsigned int   mFcAddrS;               // for the Flash ROM area(start address)
  unsigned int   mFcAddrE;               // for the Flash ROM area(end address)
  unsigned int   mCramAddrS;             // for the Cluster RAM area(start address)
  unsigned int   mCramAddrE;             // for the Cluster RAM area(end address)
  unsigned int   mLramAddrS;             // for the Local RAM area(start address)
  unsigned int   mLramAddrE;             // for the Local RAM area(end address)
  unsigned int   mExMReadRomLatency;     // for the read latency of Flash ROM for External master 
  unsigned int   mExMWriteRomLatency;    // for the write latency of Flash ROM for External master 
  unsigned int   mExMReadCramLatency;     // for the read latency of Cluster RAM for External master 
  unsigned int   mExMWriteCramLatency;    // for the write latency of Cluster RAM for External master 
  unsigned int   mExMReadLramLatency;     // for the read latency of Local RAM for External master 
  unsigned int   mExMWriteLramLatency;    // for the write latency of Local RAM for External master 
  unsigned int   mExMReadLapbLatency;     // for the read latency of Local APB's IP for External master 
  unsigned int   mExMWriteLapbLatency;    // for the write latency of Local APB's IP for External master 

  SideBand GetSideBandInfo( TlmBasicPayload& trans );
  unsigned int GetSegSideVal( SideBand sb);
  unsigned int GetVciReqType( unsigned int bytes, TlmVciExtension::VciRequest_t type);
  void MemWriteToCForestBody( SideBand sideband, PhysAddr addr, unsigned char* data, unsigned int bytes, bool isDma, bool debug, TlmVciExtension::VciRequest_t type, sc_time* t );
  void MemReadToCForestBody( SideBand sideband, PhysAddr addr, unsigned char* data, unsigned int bytes, bool isDma, bool debug, TlmVciExtension::VciRequest_t type, sc_time* t );

 public:
  SC_HAS_PROCESS(G4_CaIss);

  G4_CaIss( sc_module_name module_name, NSMVG4CPUV01* g4cpuptr );
  ~G4_CaIss();

  void SetForestUtilScheapPtr(ForestUtilScheap* ptr);
  void SetClkPeriod( double period );
  void NfyTransSend( SideBand sideband, PhysAddr addr, BusTime_t time );
  void NfyTransComp( SideBand sideband, PhysAddr addr, BusTime_t time, unsigned long long data, unsigned int acc_ev );
  void MemWriteToCForest( TlmBasicPayload& trans, bool debug, sc_time* t );
  void MemReadToCForest( TlmBasicPayload& trans, bool debug, sc_time* t );
  void SetPeType( unsigned int pe_id, unsigned int pe_type);
  unsigned int GetPeType( unsigned int pe_id);
  unsigned int GetResourcePeFromAddress( unsigned int addr, unsigned int size );

  void SetFromArea(unsigned int s_addr, unsigned int e_addr);
  void SetCramArea(unsigned int s_addr, unsigned int e_addr);
  void SetLramArea(unsigned int s_addr, unsigned int e_addr);
  void WriteCfgReg( TlmBasicPayload& trans );
  void ReadCfgReg( TlmBasicPayload& trans );
  void WriteCrgReg( TlmBasicPayload& trans );
  void ReadCrgReg( TlmBasicPayload& trans );
  void WritePegReg( TlmBasicPayload& trans );
  void ReadPegReg( TlmBasicPayload& trans );

  void setExMReadRomLatency( unsigned int latency );
  void setExMWriteRomLatency( unsigned int latency );
  void setExMReadCramLatency( unsigned int latency );
  void setExMWriteCramLatency( unsigned int latency );
  void setExMReadLramLatency( unsigned int latency );
  void setExMWriteLramLatency( unsigned int latency );
  void setExMReadLapbLatency( unsigned int latency );
  void setExMWriteLapbLatency( unsigned int latency );
  unsigned int getExMReadRomLatency( void );
  unsigned int getExMWriteRomLatency( void );
  unsigned int getExMReadCramLatency( void );
  unsigned int getExMWriteCramLatency( void );
  unsigned int getExMReadLramLatency( void );
  unsigned int getExMWriteLramLatency( void );
  unsigned int getExMReadLapbLatency( void );
  unsigned int getExMWriteLapbLatency( void );

  void AddBusIf2Fsbus( void );
  double periodCal( sc_dt::uint64 new_freq);

  void reset( void );
  void ChangeFreqPE0( void );
  void ChangeFreqPE1( void );
  void ChangeFreqPE2( void );
  void ChangeFreqPE3( void );
  void ChangeFreqPE4( void );
  void ChangeFreqPE5( void );
  void ChangeFreqPE6( void );
  void ChangeFreqPE7( void );
  void ChangeFreqFrom( void );
  void ChangeFreqCram( void );

}; // end of G4_CaIss definition

#endif /* G4_CAISS_H */
