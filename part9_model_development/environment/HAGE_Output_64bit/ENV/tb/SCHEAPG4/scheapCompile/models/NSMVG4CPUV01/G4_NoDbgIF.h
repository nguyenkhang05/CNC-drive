/*
* Copyright(c) 2016-2018 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef G4_NODBGIF_H
#define G4_NODBGIF_H

//#define   NATIVE_MODE_ENABLE
#include <stdio.h>
#include <vector>

#include "forest_common_api.h"
#include "g4cpu_def.h"
#include "HexLoader.h"

#ifdef    NATIVE_MODE_ENABLE
#include  <set>
#endif // NATIVE_MODE_ENABLE

#define PE_INIT_NUM          1
#define VM_INIT_NUM          2
#define HT_INIT_NUM          2
#define MPU_INIT_NUM        24
#define MPU_MAX_NUM         32
#define SNZ_INIT_TIME       32
#define PEID_MAX_NUM         7
#define BMID_MAX_NUM        31
#define SPID_MAX_NUM        31
#define IC_INIT_SIZE         8
#define IC_INIT_WAY          4
#define CRG_CH_MAX_NUM       8
#define CSG_CH_MAX_NUM       8


#define BANK_LIST_MAX 20

#include "global.h"
#include "g4cpu_typ.h"

class NSMVG4CPUV01;
class ForestUtilApi;
class ForestUtilScheap;
class SideBand;
#ifndef FASTISS_NO_SUPPORT
class G4_FastIssIF;
#endif // #ifndef FASTISS_NO_SUPPORT
struct BreakInfo;

class G4_NoDbgIF : public HexLoader
{

protected:
  typedef enum {
    TRC_PC,
    TRC_PIPE,
    TRC_OLD_PC
  } TRC_MODE_T;

  typedef enum {
    FA_SYNC_PS,
    FA_SYNC_NS,
    FA_SYNC_US,
    FA_SYNC_MS,
    FA_SYNC_SEC
  } FA_SYNC_UNIT_T;
  
  typedef struct{
    unsigned int  base;         // base address
    unsigned int  size;         // size
    unsigned int  f_latency;    // fetch latency(own PE area)
    unsigned int  r_latency;    // read latency(own PE area)
    unsigned int  w_latency;    // write latency(own PE area)
    unsigned int  cl_f_latency; // fetch latency(own cluster area)
    unsigned int  cl_r_latency; // read latency(own cluster area)
    unsigned int  cl_w_latency; // write latency(own cluster area)
    unsigned int  ex_f_latency; // fetch latency(other cluster area)
    unsigned int  ex_r_latency; // read latency(other cluster area)
    unsigned int  ex_w_latency; // write latency(other cluster area)
    unsigned int  outstand_latency; // outstand latency
    unsigned int  repeat_latency;   // repeat latency
    int bank_size[BANK_LIST_MAX];   // for bank_size
  }mem_info;

  ForestUtilApi *mpForestUtil;      // pointer of ForestUtilApi's instance
  ForestUtilScheap  *mpForestUtilFull;  // pointer of ForestUtilScheap's instance
#ifndef FASTISS_NO_SUPPORT
  G4_FastIssIF     *mpFastIssIF;   // pointer of FastIssIF's instance
#endif // #ifndef FASTISS_NO_SUPPORT
  NSMVG4CPUV01 *mpG4CPU;      // pointer of NSMVG4CPUV01's instance

  unsigned int   mPeNum;       // effective PE number
  unsigned int   mClstrNum;       // effective Cluster number
  unsigned int   mFastestPE;   // the fastest PE number

  double         mpClkPeriod[PE_MAX_NUM];  // each PE's clock period

  unsigned int*  mpPeType;      // each PE's Vatual Machine number
  g4cpu_pe_info  mpPeInfo[PE_MAX_NUM];      // each PE's information
  bool*          mpFpuMode;     // each PE's FPU function mode(on or off)
  bool*          mpFxuMode;     // each PE's FPU-SIMD function mode(on or off)
  unsigned int*  mpClockRatio;  // clock frequency ratio
  bool*          mpDmdp;        // each PE's MPCFG.DMDP function mode(on or off)
  g4cpu_crg_info  mpCrgInfo[CL_MAX_NUM];    // each Cluster's CRG information

  bool           mIsGramWtbufEnable; // GRAM write through buffer function mode(on or off)
  bool           mMaMode;      // MA execution mode
  unsigned int   mTlbNum;      // TLB mumber(for MMU)
  unsigned int   mTlbSize;     // TLB size(for MMU)
  unsigned int   mDcSize;      // D-cache size
  unsigned int   mDcWay;       // D-cache way
  char           mUserProgramName[PE_MAX_NUM][1024]; // user program name
  TRC_MODE_T     mProfileTraceFormat;         // execution trace format
  unsigned int   mProfileTraceAddrRangeS;     // execution trace address range(start)
  unsigned int   mProfileTraceAddrRangeE;     // execution trace address range(end)
  unsigned long long mProfileTraceTimeRangeS; // execution trace time range(start)
  unsigned long long mProfileTraceTimeRangeE; // execution trace time range(end)
  unsigned int   mProfileMsgLevel;            // CAISS's internal message outpu level

  unsigned int   mCaissSyncPeriod;            // CAISS's sync period for CycleExecute
  unsigned int   mFaissSyncPeriod;            // FastISS's sync period
  unsigned int   mFaissSyncTimeUnit;          // FastISS's sync time unit
  unsigned int   mFaissBlockSize;             // FastISS's translation block size

  std::vector<mem_info*> mRomInfoQue[CL_MAX_NUM];  // for ROM information list
  std::vector<mem_info*> mCRamInfoQue[CL_MAX_NUM]; // for Cluster RAM information list
  std::vector<mem_info*> mLRamInfoQue[PE_MAX_NUM]; // for Local RAM information list


  FILE *fp_trace;   // output file for trace profile(execution trace)
  FILE *fp_memory;  // output file for memory profile(cache trace)

  FILE *fp_trace_summary;  // output file for execution summary
  FILE *fp_memory_summary; // output file for cache summary

  char mProfileTrace[1024];  // output file name of execution trace
  char mProfileMemory[1024]; // output file name of cache trace
  char mProfileTraceSummary[1024];  // output file name of execution summary
  char mProfileMemorySummary[1024]; // output file name of cache summary

  BreakInfo* mpBrkInfoCA;  // pointer of BreakInfo

  bool mIsUnderReset;      // reset status flag(true:under reset, false:not reset)

  unsigned int mSwd0pcadVal[PE_MAX_NUM];  // SWD0PCAD's value 
  bool mIsFaissLinkFuncEnable;  // FastISS's flag of link function for ldl.w/stc.w
  bool mIsSwdtEnable[PE_MAX_NUM];         // detection of the PC check function for SWDT 
  bool mIsTraceEnable;        // detection of the Trace output function

  bool mIsCfgEnable;           // for the Flash ROM Guard function flag
  bool mIsCfgIllAccTest;       // for the Flash ROM Guard Illegal Access Test flag
  bool mIsCrgIllAccTest;       // for the Cluster RAM Guard Access Test flag
  unsigned int mCfgProtRegion; // for the Flash ROM Secure Region
  bool mIsAutoMode;            // CPU control mode (true:AUTO, false:BOOTCTRL start)

#ifdef TEST_E3
  unsigned int mTestSetMpm;    // for the E3 Test
#endif

#ifdef    NATIVE_MODE_ENABLE
  std::set<unsigned int> mNativeModePeId; // PEIDs of the Native-mode PEs
#endif // NATIVE_MODE_ENABLE

  virtual void LoadSrecBody (unsigned int addr, unsigned char data, unsigned int peid);

public:

  G4_NoDbgIF( const char *config_file, NSMVG4CPUV01 *pG4CPU );
  ~G4_NoDbgIF();

  virtual ExecType GetNextExecType( void );
  virtual uint32_t GetNextExecPeid( void );
  virtual void ExecuteISS( void );
  ForestUtilApi* GetForestUtilPtr( void );
#ifndef FASTISS_NO_SUPPORT
  G4_FastIssIF*  GetFastIssIFPtr( void );
#endif // #ifndef FASTISS_NO_SUPPORT
  unsigned int ChangeCpuTypeVal(unsigned int type);
  void SetPeClkPeriod( unsigned int clstr_num, double period );

  void reset( void );
  void SetFenmiReq( unsigned int pe_id, bool int_req );
  void SetFeintReq( unsigned int pe_id, bool int_req, unsigned int int_ch, bool is_gm, unsigned int gpid, bool is_bgint );
  void SetEiintReq( unsigned int pe_id, bool int_req, unsigned int int_pri, unsigned int int_ch, bool int_eitb, bool is_gm, unsigned int gpid, bool is_bgint );

  void SetExecutionMode( unsigned int pe_id, bool isRun );

  void InitConfigParam( void );
  void CreateCForestG3M( void );
  void DeleteCForestG3M( void );
  void CreateFastIssIF( void );
  void InitFastIssIF( void );
  void DeleteFastIssIF( void );
  BreakHitAttribute CheckBreakType(unsigned int& peid, unsigned int& code, GuardErrType& gderr);
  void SetAckInfo(unsigned int peid, unsigned int code);
  void LoadHex ( void );

  ///<summary>instanstansiation of ForestUtil</summary>
  ///<returns>none</returns>
  void CreateForestUtil( void );
  ///<summary>Set ForetUtil's pointer to CaISS</summary>
  ///<returns>none</returns>
  void SetForestUtilPtr( void );
  ///<summary>Check message level</summary>
  ///<param name="token">message level for CForestG3M</param>
  ///<returns>true:valid message level, false invalid message level</returns>
  bool CheckMsgLevel( char* token );

  // When the compile version of SC-HEAP is changed to same of cforestg4x, the argument is changed to "FILE* fp"
  //void SetExTrcFp ( FILE* fp ); 
  void SetExTrcFp ( char* fname );
  void SetMemTrcFp ( FILE* fp );
  FILE* GetTraceFp();

  void read_config_file_PeNum( const char *filename );
  void read_config_file_PeType( const char *filename );
  void read_config_file( const char *filename );

  unsigned long long readMemValue(unsigned int peid, unsigned int addr, unsigned int byte);
  unsigned long long writeMemValue(unsigned int peid, unsigned int addr, unsigned long long value, unsigned int byte);

  void dispExInstNum(unsigned int peid, bool isToFile=false);
  void dispIcacheAccNum(unsigned int peid, bool isToFile=false);
  void flushTraceData();

  void SetClockRatio(unsigned int peid, unsigned int ratio);
  void SetDmdpMode(unsigned int peid, bool mode );

  void SetResetStatus(bool stat);

  unsigned long long getIcacheAccNum( unsigned int peid );
  unsigned long long getIcacheHitNum( unsigned int peid );
  unsigned long long getIcacheMissNum( unsigned int peid );
  void setSwdtEnable( unsigned int peid, bool mode );
  void SetSwd0pcadValue( unsigned int peid, unsigned int value );
  void setTraceEnable( bool mode );
  void clearFastissCache( unsigned int peid );

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

  unsigned int readGrValue( unsigned int peid, unsigned int regid ); 
  void writeGrValue( unsigned int peid, unsigned int regid, unsigned int value);
  unsigned long long readWrValue( unsigned int peid, unsigned int regid, bool isUpper ); 
  void writeWrValue( unsigned int peid, unsigned int regid, unsigned long long up_value, unsigned long long lo_value );
  unsigned int readSrValue( unsigned int peid, unsigned int regid, unsigned int selid ); 
  void writeSrValue( unsigned int peid, unsigned int regid, unsigned int selid, unsigned int value);

  ///<summary>set Syserr output request</summary>
  ///<param name="peid">pe id</param>
  ///<param name="code">error code</param>
  ///<returns>none</returns>
  void setSyserrOut( unsigned int peid, unsigned int code );

  ///<summary>set Syserr (Operand factor) output request</summary>
  ///<param name="peid">pe id</param>
  ///<param name="code">error code</param>
  ///<returns>none</returns>
  void setSyserrOperandOut( unsigned int peid,  unsigned int code );

  ///<summary>get current PC address</summary>
  ///<param name="peid">pe id</param>
  ///<returns>PC address</returns>
  unsigned long long getCurrentPc( unsigned int peid );

  ///<summary>get memory access count of the current PC's instruction</summary>
  ///<param name="peid">pe id</param>
  ///<param name="isWrite">access type (trus:write, false:read)</param>
  ///<returns>memory access count</returns>
  unsigned int getMemAccCntCurrentPc(unsigned int peid, bool isWrite );

  ///<summary>get memory access data and length of the current PC's instruction</summary>
  ///<param name="peid">pe id</param>
  ///<param name="acc_cnt">memory access number</param>
  ///<param name="data_l">data (lower 64bit)</param>
  ///<param name="data_u">data (upper 64bit)</param>
  ///<param name="data_length">data length</param>
  ///<param name="isWrite">access type (trus:write, false:read)</param>
  ///<returns>none</returns>
  void getMemAccDataCurrentPc( unsigned int peid, unsigned int acc_cnt, unsigned long long *data_l, unsigned long long *data_u, unsigned int *data_length, bool isWrite );

protected:
  void SetAddrMask ( unsigned int mask_val );
  void SetCpuInfo ( unsigned int pe_id, unsigned int pe_type );
  void SetHvInfo ( unsigned int pe_id, unsigned int pe_type, unsigned int hvcfg_val );
  void SetFpuMode ( unsigned int pe_id, bool isFpuOn );
  void SetFxuMode ( unsigned int pe_id, bool isFxuOn );
  void SetMpuInfo ( unsigned int pe_id, unsigned int mpu_area_num );
  void SetSnzTime ( unsigned int pe_id, unsigned int snz_time );
  void SetNcRbase ( unsigned int pe_id, unsigned int rbase_addr );
  void SetNcSpid ( unsigned int pe_id, unsigned int spid_val );
  void SetICacheInfo ( unsigned int pe_id, unsigned int ic_size, unsigned int ic_way );
  void SetDCacheInfo ( unsigned int dc_size, unsigned int dc_way );
  void SetExTrcFormat ( unsigned int ex_trc_format );
  void SetExTrcAddrRange ( unsigned int ex_trc_saddr, unsigned int ex_trc_eaddr );
  void SetExTrcTimeRange ( unsigned long long ex_trc_stime, unsigned long long ex_trc_etime );
  void SetMemTrcEvent ( unsigned int mem_trc_ev );
  void SetMemTrcAddrRange ( unsigned int mem_trc_saddr, unsigned int mem_trc_eaddr );
  void SetMemTrcTimeRange ( unsigned long long mem_trc_stime, unsigned long long mem_trc_etime );
  void SetMemMap ( void );
  void CheckMemMap ( void );

  ///<summary>set Gram Write Through Buffer's parameter of CAISS</summary>
  ///<returns>none</returns>
  void SetGramWtBufInfo ( void );

  ///<summary>set the code flash guard's parameter</summary>
  ///<returns>none</returns>
  void SetCfgInfo ( void );

  ///<summary>set the cluster ram guard's parameter</summary>
  ///<returns>none</returns>
  void SetCrgInfo ( void );
  
  ///<summary>set the PE guard's parameter</summary>
  ///<returns>none</returns>
  void SetPegInfo ( void );

  ///<summary>change the guard error value from CForestG3M's value to G3MCPU's value</summary>
  ///<param name="grd_err_t">CForestG3M's error value</param>
  ///<returns>G3MCPU's error value</returns>
  unsigned int GetGrdSrc(GuardErrType grd_err_t);

  ///<summary>get spid, when error occures</summary>
  ///<param name="pe_id">PE ID</param>
  ///<returns>SPID</returns>
  unsigned char GetSpIdAtGrdErr( unsigned int pe_id );

  ///<summary>get pswh, when CycleExecute breaks by BRK_CHANGE_PSWH</summary>
  ///<param name="pe_id">PE ID</param>
  ///<returns>PSWH</returns>
  unsigned int GetPswh( unsigned int pe_id );

  ///<summary>get access event, when error occures</summary>
  ///<param name="pe_id">PE ID</param>
  ///<param name="grd_id">Guard module's PE ID</param>
  ///<returns>none</returns>
  void GetAccTypeAtGrdErr( unsigned int pe_id, unsigned int& grd_id );

  ///<summary>check setting all PE's type </summary>
  ///<returns>none</returns>
  void CheckPeType( void );

  ///<summary>Cancel Eiint/BGEIINT/GMEIINT's request for CAISS</summary>
  ///<param name="pe_id">PE ID</param>
  /////<returns>None</returns>
  void CancelInterrupt( unsigned int pe_id );

}; // end of G4_NoDbgIF definition

#endif /* G4_NODBGIF_H */
