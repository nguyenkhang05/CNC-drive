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

#ifndef PECORESS_H
#define PECORESS_H

#include "rh850g4/core.h"       // for rh850g4::core class
#include "rh850g4/memory_wrapper.h" // for rh850g4::memory_wrapper class
#include "tlm.h"      // for OSCI TLM2.0
#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0
#include "global.h"
#include "g4cpu_typ.h"
#include "G4_PEG_ERR_INFO.h"

template< unsigned int BUSWIDTH > class G4_PAYLOAD_ADDER;
template< unsigned int BUSWIDTH > class OSCI2DCDR;
template< unsigned int IS_BUSWIDTH, unsigned int TS_BUSWIDTH > class I_BRIDGE;
class G4_PEG_Wrapper;

extern sc_time_unit glb_resolution_unit;

class PECORESS : public sc_module
{
  protected:
    
  public:
    G4_PAYLOAD_ADDER<BUS_WIDTH_VCI32> *mpPayloadAdder;        // pointer of G4_PAYLOAD_ADDER's instance
    OSCI2DCDR<BUS_WIDTH_VCI32>     *mpIsRouter;               // pointer of OSCI2DCDR's instance
    I_BRIDGE<BUS_WIDTH_VCI,  BUS_WIDTH_VCI32> *mpIsgpBridge;  // pointer of I_BRIDGE's instance(for P-Bus area)
    I_BRIDGE<BUS_WIDTH_VCI,  BUS_WIDTH_VCI32> *mpIsgcBridge;  // pointer of I_BRIDGE's instance(for I-Bus area)
    I_BRIDGE<BUS_WIDTH_VCI,  BUS_WIDTH_VCI32> *mpIshBridge;   // pointer of I_BRIDGE's instance(for H-Bus area)
    I_BRIDGE<BUS_WIDTH_VCI,  BUS_WIDTH_VCI32> *mpIsiBridge;   // pointer of I_BRIDGE's instance(for INTC1)
    I_BRIDGE<BUS_WIDTH_VCI32,  BUS_WIDTH_VCI32> *mpIsBridge;  // pointer of I_BRIDGE's instance(for Other PE resource or CRAM)
    I_BRIDGE<BUS_WIDTH_VCI,  BUS_WIDTH_VCI32> *mpTspegBridge;  // pointer of I_BRIDGE's instance(for PEG tsv)
    I_BRIDGE<BUS_WIDTH_VCI32,  BUS_WIDTH_VCI> *mpIspegBridge;  // pointer of I_BRIDGE's instance(for PEG is)
    G4_PEG_Wrapper *mpG4PegWrapper; // pointer of G4_PEG_Wrapper's instance

    // clock port
    sc_in<sc_dt::uint64> sys_clk; // for clock port

    // reset port
    sc_in<bool>          sys_reset; // for reset port

    // bus socket : TlmInitiatorSocket (tlm::tlm_initiator_socket<>)
    TlmInitiatorSocket<BUS_WIDTH_VCI32> is;  // for local RAM of other cluster's PE
    TlmInitiatorSocket<BUS_WIDTH_VCI> ish;   // for H-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI> isgp;  // for P-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI> isgc;  // for I-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI> isi;   // for INTC1
    TlmTargetSocket<BUS_WIDTH_VCI32>  ts;    // for other Master's access
    TlmTargetSocket<BUS_WIDTH_VCI32>  ts_peg_v;  // for other Master's access to LRAM and INTC1
    TlmTargetSocket<BUS_WIDTH_APB>    ts_peg_p;  // for PEG register
    TlmTargetSocket<BUS_WIDTH_APB>    ts_peg_m;  // for PEGCAP_M register
    TlmTargetSocket<BUS_WIDTH_APB>    ts_peg_s;  // for PEGCAP_S register

    // interrupt port for INTC1
    sc_in<bool>          FenmiReq; // for fenmi request port
    sc_in<unsigned int>  FeintReq; // for feint request port
    sc_in<sc_dt::uint64> EiintReq; // for eiint request port
    sc_out<bool>         FenmiAck; // for fenmi ack port
    sc_out<unsigned int> FeintAck; // for feint ack port
    sc_out<unsigned int> EiintAck; // for eiint ack port
    sc_out<unsigned int> CpuGmInfo; // for CPU mode info notification port

    // interrupt port for Guard error
    sc_in<G4_PEG_Cerrinfo>  PegInfoIn;   // for input the error information to Master CPU
    sc_out<G4_PEG_Cerrinfo> PegInfoOut;  // for output the error information to Master CPU
    sc_out<bool>            PegErrM;     // for output the PEGCAP_M's error to ECM
    sc_out<bool>            PegErrS;     // for output the PEGCAP_M's error to ECM

  private:    
    rh850g4::core   *mpFastIss_ASTC; // pointer of fast ISS instance
    rh850g4::memory_wrapper *mpFastIssMemWrapper_ASTC; // pointer of fast ISS memory wrapper
    unsigned int    mPeId;        // for PE ID
    unsigned int    mPeType;        // for PE type
    SIM_MODE_T     mIssMode;      // for the simulation mode 
    FASTISS_MODE_T mFastIssMode;  // for the simulation mode 
    const char    *mConfigFile;   // for the name of configulation file
    char mFastissRouterMapFname[1024];   // map file name for FastISS

    sc_signal<bool>*         SyserrAckNm_sig;   // signal of Syserr Ack from fastISS
    sc_signal<bool>*         SyserrReqNm_sig;   // signal of Syserr Requset to fastISS
    sc_signal<sc_uint<8> >*  SyserrCodeNm_sig;  // signal of Syserr Code to fastISS
    sc_signal<bool>*         SyserrModeNm_sig;   // signal of Syserr Mode to fastISS

    sc_event                 syserr_init_event; // for SYSERR info init event

  public:
    SC_HAS_PROCESS(PECORESS);
    PECORESS( sc_module_name module_name, const char *config_file );
    ~PECORESS( );
    rh850g4::core* GetFastIssPtr( void );
    rh850g4::memory_wrapper* GetFastIssMemWrapperPtr( void );
    void SetPeId( unsigned int peid );
    void setSyserrOut( unsigned int code );
    void setSyserrOperandOut( unsigned int code );
    void SyserrAckProcess( void );
    void Reset( void );
    void SyserrInitProcess( void );
    void SetPegInfo ( void );

  private:
    void read_config_file( const char *filename );
    void read_config_file_PeType( const char *filename );
    void SetSimMode( SIM_MODE_T iss_mode );
    void SetFastIssMode( FASTISS_MODE_T fastiss_mode );
    void SetInitVal( void );
}; // end of PECORESS definition

#endif /* PECORESS_H */
