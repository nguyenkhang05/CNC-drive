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

#ifndef NSMVG4CPUV01_H
#define NSMVG4CPUV01_H

#include "tlm.h"      // for OSCI TLM2.0
#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0
#include "global.h"
#include "g4cpu_typ.h"
#include "G4_PEG_ERR_INFO.h"

#ifdef IEEE_1666_SYSTEMC
#define SC_SIGNAL(T) sc_signal< T , SC_UNCHECKED_WRITERS>
#else
#define SC_SIGNAL(T) sc_signal< T >
#endif

template< unsigned int BUSWIDTH > class DummySlave;
template< unsigned int BUSWIDTH > class DummyMaster;
template< unsigned int BUSWIDTH > class OSCI2DCDR;
template< unsigned int IS_BUSWIDTH, unsigned int TS_BUSWIDTH > class I_BRIDGE;
template< unsigned int BUSWIDTH > class I_CONNECTOR;
template< unsigned int BUSWIDTH, unsigned int LT_BUSWIDTH > class T_SELECTOR;

class G4_NoDbgIF;
class G4_CaIss;
class G4_FastIssIF;
class CLUSTERSS;
class G4_PEGERR_OR;

class G4_LinkManager;
class Rteserv2Controller; // For MULTI

extern sc_time_unit glb_resolution_unit;

class NSMVG4CPUV01 : public sc_module
{
  protected:
    typedef struct{
      bool          isEnable;     // EMU3 enable flag
      unsigned int  peid;         // peid for EMU3
      unsigned int  base;         // base address
      unsigned int  size;         // size
    }emu3_info;
    
  public:
    typedef enum {
      DBG_NONE,
      DBG_MULTI,
      DBG_CUBESUITE,
      DBG_VLAB
    } DBG_KIND_T;

    typedef enum {
      GRD_ERR_NONE,
      GRD_ERR_CF_GRD,
      GRD_ERR_CF_ILL_ACC_PE,
      GRD_ERR_CRAM_GRD,
      GRD_ERR_CRAM_ILL_ACC_PE,
      GRD_ERR_PE_GRD_M,
      GRD_ERR_PE_GRD_S,
      GRD_ERR_CF_ILL_ACC_SAXI,
      GRD_ERR_CRAM_ILL_ACC_SAXI,
    } GRD_ERR_SRC_T;
    
    class FenmiAdapter : public sc_module{
    public:
      
      NSMVG4CPUV01* mpG4CPU; // pointer of NSMVG4CPUV01's instance
      unsigned int   mPeId;    // PE information
      bool           mAckVal;  // Acknowlidge value

      SC_SIGNAL(bool)  fenmireq_iss_sig; // signal of fenmireq to fast iss
      SC_SIGNAL(bool)  fenmiack_iss_sig; // signal of fenmiack from fast iss

      sc_in<bool>      fenmireq_intc1; // for fenmi request port from INTC1
      sc_out<bool>     fenmiack_intc1; // for fenmi ack port to INTC1
      sc_out<bool>     fenmireq_faiss; // for fenmi request port to FastISS 
      sc_in<bool>      fenmiack_faiss; // for fenmi ack port from FastISS

      sc_event         ack_caiss_event; // for fenmi ack event from Ca ISS

    public:
      SC_HAS_PROCESS(FenmiAdapter);
      FenmiAdapter( sc_module_name module_name );
      ~FenmiAdapter();
      void FenmiReqProcess(void);
      void FenmiAckProcess(void);
      void SetPeId(unsigned int peid);
      void SetG4cpuPtr(NSMVG4CPUV01* pG4CPU);
      void Reset(void);
    };

    class FeintAdapter : public sc_module{
    public:
      NSMVG4CPUV01* mpG4CPU; // pointer of NSMVG4CPUV01's instance
      unsigned int   mPeId;    // PE information
      unsigned int   mAckVal;  // Acknowlidge value

      SC_SIGNAL(unsigned int)  feintreq_iss_sig; // signal of feintreq to fast iss
      SC_SIGNAL(unsigned int)  feintack_iss_sig; // signal of feintack from fast iss

      sc_in<unsigned int>      feintreq_intc1; // for feint request port from INTC1
      sc_out<unsigned int>     feintack_intc1; // for feint ack port to INTC1
      sc_out<unsigned int>     feintreq_faiss; // for feint request port to FastISS 
      sc_in<unsigned int>      feintack_faiss; // for feint ack port from FastISS

      sc_event                 ack_caiss_event; // for feint ack event from Ca ISS

    public:
      SC_HAS_PROCESS(FeintAdapter);
      FeintAdapter( sc_module_name module_name );
      ~FeintAdapter();
      void FeintReqProcess(void);
      void FeintAckProcess(void);
      void SetPeId(unsigned int peid);
      void SetG4cpuPtr(NSMVG4CPUV01* pG4CPU);
      void Reset(void);
    };

    class EiintAdapter : public sc_module{
    public:
      NSMVG4CPUV01* mpG4CPU; // pointer of NSMVG4CPUV01's instance
      unsigned int   mPeId;    // PE information
      unsigned int   mAckVal;  // Acknowlidge value

      SC_SIGNAL(sc_dt::uint64) eiintreq_iss_sig; // signal of eiintreq to fast iss
      SC_SIGNAL(unsigned int)  eiintack_iss_sig; // signal of eiintack from fast iss

      sc_in<sc_dt::uint64>  eiintreq_intc1; // for eiint request port from INTC1
      sc_out<unsigned int>  eiintack_intc1; // for eiint ack port to INTC1
      sc_out<sc_dt::uint64> eiintreq_faiss; // for eiint request port to FastISS 
      sc_in<unsigned int>   eiintack_faiss; // for eiint ack port from FastISS

      sc_event         ack_caiss_event; // for eiint ack event from Ca ISS

    public:
      SC_HAS_PROCESS(EiintAdapter);
      EiintAdapter( sc_module_name module_name );
      ~EiintAdapter();
      void EiintReqProcess(void);
      void EiintAckProcess(void);
      void SetPeId(unsigned int peid);
      void SetG4cpuPtr(NSMVG4CPUV01* pG4CPU);
      void Reset(void);
    };

    class GrdErrAdapter : public sc_module{
    public:
      NSMVG4CPUV01*    mpG4CPU;         // pointer of NSMVG4CPUV01's instance
      unsigned int     mPeId;           // PE information
      bool             mErrReqMode;     // Error request mode
      sc_in<bool>      err_fastiss;     // for guard error notify port form FastISS PEG
      sc_out<bool>     err_req;         // for guard error request port to ECM
      sc_event         err_req_event;   // for guard error request control event from CAISS's CRG (including FastISS access) and PEG 
      SC_SIGNAL(bool)* err_fastiss_sig; // signal of cf illegal access error request from fastISS

    public:
      SC_HAS_PROCESS(GrdErrAdapter);
      GrdErrAdapter( sc_module_name module_name );
      ~GrdErrAdapter();
      void ErrReqProcess(void);
      void SetPeId(unsigned int peid);
      void SetG4cpuPtr(NSMVG4CPUV01* pG4CPU);
      void Reset(void);
    };

    class CpuInitAdapter : public sc_module{
    public:
      NSMVG4CPUV01*  mpG4CPU;     // pointer of NSMVG4CPUV01's instance
      unsigned int   mPeId;       // PE information
      bool           mIsAutoMode; // CPU control mode (true:AUTO, false:BOOTCTRL start)
      bool           mIsStart;    // CPU simiulation start flag (true:under execution, false:waiting the start request)
      sc_in<bool>    sys_cpinit;  // for CPU start request port from BOOTCTRL

    public:
      SC_HAS_PROCESS(CpuInitAdapter);
      CpuInitAdapter( sc_module_name module_name );
      ~CpuInitAdapter();
      void CpuStartProcess(void);
      void SetCpuStartMode(bool isAuto);
      void SetPeId(unsigned int peid);
      void SetG4cpuPtr(NSMVG4CPUV01* pG4CPU);
      void Reset(void);
    };

    class CpuGmInfoAdapter : public sc_module{
    public:
      NSMVG4CPUV01* mpG4CPU;     // pointer of NSMVG4CPUV01's instance
      unsigned int  mPeId;       // PE information
      unsigned int  mCpuGmInfo;  // CPU GM information

      SC_SIGNAL(unsigned int)  cpu_gm_info_sig; // signal of CPU GM information from fast iss

      sc_in<unsigned int>   cpu_gm_info_faiss; // for CPU GM information port from FastISS
      sc_out<unsigned int>  cpu_gm_info_intc;  // for CPU GM information port to INTC1

      sc_event         cpugminfo_caiss_event;  // for CPU GM information event from Ca ISS

    public:
      SC_HAS_PROCESS(CpuGmInfoAdapter);
      CpuGmInfoAdapter( sc_module_name module_name );
      ~CpuGmInfoAdapter();
      void CpuGmInfoProcess(void);
      void SetPeId(unsigned int peid);
      void SetG4cpuPtr(NSMVG4CPUV01* pG4CPU);
      void Reset(void);
    };


  private: 
    DBG_KIND_T     mDebuggerType; // for using debegger type
    SIM_MODE_T     mIssMode;      // for the simulation mode 
    FASTISS_MODE_T mFastIssMode;  // for the simulation mode 
    const char    *mConfigFile;   // for the name of configulation file
    unsigned int   mSwd0pcadVal[PE_MAX_NUM];  // SWD0PCAD's value 
    std::string    mPeType[PE_MAX_NUM]; // for core type
    bool           mProgramLoaded; // program loaded flag(true:loaded, fales:not loaded) 
    bool           mAsyncFlag;     // true: Async, false: Sync
    g4cpu_pe_info  mpPeInfo[PE_MAX_NUM];      // each PE's information
    bool           mIsOldTraceFormat;        // execution trace format for FASTISS
    bool           mIsAutoMode;    // CPU control mode (true:AUTO, false:BOOTCTRL start)

    sc_event       reset_test_event; // for reset test ISS

    SC_SIGNAL(bool)* hi_activ_reset_sig;     // reset signal for high active(SEG, PEG, IPG)
    SC_SIGNAL(G4_PEG_Cerrinfo)*  mpPegInfoOut_sig[PE_MAX_NUM];    // signal of PEGERR info from PEG 
    SC_SIGNAL(G4_PEG_Cerrinfo)*  mpPegInfoIn_sig[PE_MAX_NUM];     // signal of PEGERR info to master PE's PEGCAP_M

  public:
    G4_CaIss*           mpCaIss;       // for the pointer of CaISS's instance
    G4_FastIssIF*       mpFastIssIF;   // pointer of FastIssIF's instance
    G4_NoDbgIF*         mpNoDbgIF;     // for the pointer of NoDbgIF's instance
    G4_LinkManager*     mpLinkManager; // pointer of G4_LinkManager's instance
    Rteserv2Controller* mpRtesrv2Ctrl; // for the pointer of Rteserv2Controller's instance // For MULTI
    CLUSTERSS                *mpCluster[CL_MAX_NUM]; // pointer of CLUSTERSS's instance
    OSCI2DCDR<BUS_WIDTH_VCI32> *mpIsRouter;     // pointer of OSCI2DCDR's instance(for System Bus)
    OSCI2DCDR<BUS_WIDTH_VCI> *mpTssRouter;      // pointer of OSCI2DCDR's instance(for System Bus)
    I_BRIDGE<BUS_WIDTH_CRAM, BUS_WIDTH_VCI32> *mpTscrBridge;       // pointer of I_BRIDGE's instance(for CAISS.tsgr)
    I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32> *mpIsgpBridge;        // pointer of I_BRIDGE's instance(for isgp)
    I_BRIDGE<BUS_WIDTH_VCI, BUS_WIDTH_VCI32> *mpIshBridge;         // pointer of I_BRIDGE's instance(for ish)
    I_BRIDGE<BUS_WIDTH_VCI32, BUS_WIDTH_VCI> *mpIsgcBridge;        // pointer of I_BRIDGE's instance(for CL0_isgc)
    I_CONNECTOR<BUS_WIDTH_VCI> *mpIsgpConnector; // pointer of I_CONNECTOR's instance(for P-Bus area)
    I_CONNECTOR<BUS_WIDTH_VCI> *mpIshConnector;  // pointer of I_CONNECTOR's instance(for H-Bus area)
    I_CONNECTOR<BUS_WIDTH_VCI32> *mpIsgcConnector; // pointer of I_CONNECTOR's instance(for I-Bus area)
    T_SELECTOR<BUS_WIDTH_FLSH, BUS_WIDTH_VCI32> *mpTsfSelector;    // pointer of T_SELECTOR's instance(for Flash)
    T_SELECTOR<BUS_WIDTH_VCI,  BUS_WIDTH_VCI32> *mpTssSelector;    // pointer of T_SELECTOR's instance(for System Bus)
    T_SELECTOR<BUS_WIDTH_CRAM, BUS_WIDTH_VCI32> *mpTscrSelector;   // pointer of T_SELECTOR's instance(for Cluster RAM)

    DummySlave<BUS_WIDTH_VCI> *mpSlvDummy_isgp[PE_MAX_NUM]; // for Dummy memory for CAISS isgp
    DummySlave<BUS_WIDTH_VCI> *mpSlvDummy_isgc[PE_MAX_NUM]; // for Dummy memory for CAISS isgc
    DummySlave<BUS_WIDTH_VCI> *mpSlvDummy_ish[PE_MAX_NUM]; // for Dummy memory for CAISS ish
    DummySlave<BUS_WIDTH_VCI> *mpSlvDummy_isi[PE_MAX_NUM]; // for Dummy memory for CAISS isi
    DummyMaster<BUS_WIDTH_VCI> *mpMstIsgpDummy[CL_MAX_NUM]; // for Dummy Master for Cluster's isgp
    DummyMaster<BUS_WIDTH_VCI> *mpMstIshDummy[CL_MAX_NUM]; // for Dummy Master for Cluster's ish
    DummyMaster<BUS_WIDTH_VCI32> *mpMstIsgcDummy[CL_MAX_NUM]; // for Dummy Master for Cluster's isgc
    DummyMaster<BUS_WIDTH_VCI32> *mpMstIsiDummy[PE_MAX_NUM]; // for Dummy Master for PE's isi

    FenmiAdapter*       mpFenmiAdapter[PE_MAX_NUM];     // for the pointer of FenmiAdapter's instance
    FeintAdapter*       mpFeintAdapter[PE_MAX_NUM];     // for the pointer of FeintAdapter's instance
    EiintAdapter*       mpEiintAdapter[PE_MAX_NUM];     // for the pointer of EiintAdapter's instance
    CpuInitAdapter*     mpCpuInitAdapter[PE_MAX_NUM];   // for the pointer of CpuInitAdapter's instance
    CpuGmInfoAdapter*   mpCpuGmInfoAdapter[PE_MAX_NUM]; // for the pointer of CpuGmInfoAdapter's instance
  
    GrdErrAdapter*      mpPeCramAreaerrAdapter[PE_MAX_NUM];       // for the pointer of GrdErrAdapter's instance(CRAM illegal access by PE)
    GrdErrAdapter*      mpExMCramAreaerrAdapter;                  // for the pointer of GrdErrAdapter's instance(CRAM illegal access via SAXI)
    GrdErrAdapter*      mpCramGrderrAdapter;                      // for the pointer of GrdErrAdapter's instance(CRAM guard error)
    GrdErrAdapter*      mpPePegErrMAdapter[PE_MAX_NUM];           // for the pointer of GrdErrAdapter's instance(PE guard error from PEGCAP_M)
    GrdErrAdapter*      mpPePegErrSAdapter[PE_MAX_NUM];           // for the pointer of GrdErrAdapter's instance(PE guard error from PEGCAP_S)

    G4_PEGERR_OR*       mpPegErrOr;  // for error or circuit of fastISS's PEG 

    // clock port
    sc_in<sc_dt::uint64> *CL_sys_clk[CL_MAX_NUM]; // for PE's clock port
    sc_in<sc_dt::uint64> Sys_bus_clk; // for System bus clock port
    sc_in<sc_dt::uint64> VCI_clk;  // for VCI bus's clock port
    sc_in<sc_dt::uint64> From_clk; // for flash rom's clock port
    sc_in<sc_dt::uint64> Cram_clk; // for cluster ram's clock port

    // reset port
    sc_in<bool>          sys_reset; // for reset port

    // cpu start control signal port
    sc_in<bool>          *sys_cpinit[PE_MAX_NUM]; // for CPU start control signal port from BOOTCTRL

    // bus socket : TlmInitiatorSocket (tlm::tlm_initiator_socket<>)
    TlmInitiatorSocket<BUS_WIDTH_VCI> ish;                    // for H-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI> isgp;                   // for P-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI> *CL_ish[CL_MAX_NUM];    // for H-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI> *CL_isgp[CL_MAX_NUM];   // for P-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI32> *CL_isgc[CL_MAX_NUM]; // for I-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI32> *PE_isi[PE_MAX_NUM];  // for INTC1

    // bus socket : TlmTargetSocket (tlm::tlm_initiator_socket<>)
    TlmTargetSocket<BUS_WIDTH_FLSH> tsf;  // for flash I/F
    TlmTargetSocket<BUS_WIDTH_VCI>  tss;  // for System Bus Master
    TlmTargetSocket<BUS_WIDTH_CRAM> tscr; // for cluster ram I/F

    // interrupt port
    sc_in<bool>          *PE_FenmiReq[PE_MAX_NUM]; // for fenmi request port
    sc_in<unsigned int>  *PE_FeintReq[PE_MAX_NUM]; // for feint request port
    sc_in<sc_dt::uint64> *PE_EiintReq[PE_MAX_NUM]; // for eiint request port
    sc_out<bool>         *PE_FenmiAck[PE_MAX_NUM]; // for fenmi ack port
    sc_out<unsigned int> *PE_FeintAck[PE_MAX_NUM]; // for feint ack port
    sc_out<unsigned int> *PE_EiintAck[PE_MAX_NUM]; // for eiint ack port
    sc_out<unsigned int> *PE_CpuGmInfo[PE_MAX_NUM]; // for CPU mode info notification port

    // SWDT clock port
    sc_out<bool>          *Swdt_clk[PE_MAX_NUM];           // for SWDT clock port
    sc_event              swdt_clk_out_event[PE_MAX_NUM]; // for swdt clock out
    bool                  mIsSwdtClkHi[PE_MAX_NUM];          // Swdt_Clk signal status

    sc_out<bool>         *PE_PegErrM[PE_MAX_NUM];     // for PE guard error port (from PEGCAP_M)
    sc_out<bool>         *PE_PegErrS[PE_MAX_NUM];     // for PE guard error port (from PEGCAP_S)
    sc_out<bool>          CramGrderr;                 // for CRAM guard error port
    sc_out<bool>         *PE_CramAreaerr[PE_MAX_NUM]; // for Illegal Address error(CRAM illegal access by PE) output port
    sc_out<bool>          ExM_CramAreaerr;            // for Illegal Address error(CRAM illegal access via SAXI) output port

    double mPeriodCL[CL_MAX_NUM]; // for PE's cycle period
    double mPeriodSysBus;         // for System Bus's cycle period
    double mPeriodVci;            // for VCI's cycle period
    double mPeriodFrom;           // for flash rom's cycle period
    double mPeriodCram;           // for cluster ram's cycle period

    unsigned int mPeNum;          // effective PE number
    unsigned int mClstrNum;       // effective Cluster number
    //emu3_info    mEmu3Info;       // EMU3's sub CPU information

    char mFastissRouterMapFname[1024];   // map file name for FastISS
    char mCaissRouterMapFname[1024];     // map file name for CAISS's tss

    SC_HAS_PROCESS(NSMVG4CPUV01);

    double periodCal( sc_dt::uint64 new_freq);
    void SetPeClkPd2DbgIF(unsigned int pe_num, double period);
    void ChangeFreqClstr0(void);
    void ChangeFreqClstr1(void);
    void ChangeFreqClstr2(void);
    void ChangeFreqClstr3(void);
    void ChangeFreqSysBus(void);
    void ChangeFreqVci(void);
    void ChangeFreqFrom(void);
    void ChangeFreqCram(void);
    void reset(void);
    void reset_test(void);
    void SwdtClkOutPe0(void);
    void SwdtClkOutPe1(void);
    void SwdtClkOutPe2(void);
    void SwdtClkOutPe3(void);
    void SwdtClkOutPe4(void);
    void SwdtClkOutPe5(void);
    void SwdtClkOutPe6(void);
    void SwdtClkOutPe7(void);
    void SwdtClkOut(unsigned int pe_id);

    void execProcess( void );

    NSMVG4CPUV01( sc_module_name module_name, const char *config_file );

    ~NSMVG4CPUV01();

    SIM_MODE_T GetSimMode( void );
    DBG_KIND_T GetDbgMode( void );
    std::string GetPeType( unsigned int pe_id );
    FASTISS_MODE_T GetFastIssMode( void );
    void SetFenmiAckFromCa( unsigned int pe_id, bool int_ack );
    void SetFeintAckFromCa( unsigned int pe_id, bool int_ack, unsigned int int_ch );
    void SetEiintAckFromCa( unsigned int pe_id, bool int_ack, unsigned int int_ch );
    void SetCpuGmInfoFromCa( unsigned int pe_id, bool cpu_gm, unsigned int cpu_gpid );
    void SetSwdtClkOutEvFromCa( unsigned int pe_id );
    void SetGrdErrFromCa( unsigned int pe_id, GRD_ERR_SRC_T error_type );

    void start_of_simulation();
    void end_of_simulation();
    void InitVal( void );
    void CreateDbgIF( void );
    void read_config_file_PeNum( const char *filename );
    void read_config_file( const char *filename );
    void SetSimMode( SIM_MODE_T iss_mode );
    void SetFastIssMode( FASTISS_MODE_T fastiss_mode );

    FILE* GetTraceFp();
    
    unsigned long long readMemValue( unsigned int peid, 
                                     unsigned int addr, 
                                     unsigned int byte );
    unsigned long long writeMemValue( unsigned int peid, 
                                      unsigned int addr, 
                                      unsigned long long value, 
                                      unsigned int byte );
    void dispExInstNum( unsigned int peid );
    void setClockRatio( unsigned int peid, unsigned int ratio );

    unsigned long long getIcacheAccNum( unsigned int peid );
    unsigned long long getIcacheHitNum( unsigned int peid );
    unsigned long long getIcacheMissNum( unsigned int peid );

    void setSwdtEnable( unsigned int peid, bool mode );
    void setSyserrOut( unsigned int peid,  unsigned int code );
    void setSyserrOperandOut( unsigned int peid,  unsigned int code );
    void setTraceEnable( bool mode );
    void clearFastissCache( unsigned int peid );
    void setSyncMode( void );
    void setAsyncMode( void );
    bool isSyncMode( void );
    bool isAsyncMode( void );
    unsigned long long getCurrentPc( unsigned int peid );
    unsigned int getMemAccCntCurrentPc(unsigned int peid, bool isWrite=true );
    void getMemAccDataCurrentPc( unsigned int peid, unsigned int acc_cnt, unsigned long long *data_l, unsigned long long *data_u, unsigned int *data_length, bool isWrite=true );

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
    void writeGrValue( unsigned int peid, unsigned int regid, unsigned int value );
    unsigned long long readWrValue( unsigned int peid, unsigned int regid, bool isUpper ); 
    void writeWrValue( unsigned int peid, unsigned int regid, unsigned long long up_value, unsigned long long lo_value );
    unsigned int readSrValue( unsigned int peid, unsigned int regid, unsigned int selid ); 
    void writeSrValue( unsigned int peid, unsigned int regid, unsigned int selid, unsigned int value );

    void SetSwd0pcadValue( unsigned int peid, unsigned int value );
    void CfgRegAcc( TlmBasicPayload& trans, bool isRead );
    void CrgRegAcc( TlmBasicPayload& trans, bool isRead );
    void PegRegAcc( TlmBasicPayload& trans, bool isRead );

    void waitWhl0freq();

}; // end of NSMVG4CPUV01 definition

#endif /* NSMVG4CPUV01_H */
