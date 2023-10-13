/*
* Copyright(c) 2016-2017 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

/* $Id:$ */

#ifndef CLUSTERSS_H
#define CLUSTERSS_H

#include "tlm.h"        // for OSCI TLM2.0
#include "OSCI2.h"      // class for User Modeling environment based on TLM2.0
#include "global.h"
#include "G4_PEG_ERR_INFO.h"

#define CL_PE_NUM 2

template< unsigned int BUSWIDTH > class DummySlave;
template< unsigned int BUSWIDTH > class OSCI2DCDR;
template< unsigned int BUSWIDTH > class I_CONNECTOR;
template< unsigned int BUSWIDTH, unsigned int LT_BUSWIDTH > class T_SELECTOR;
template< unsigned int IS_BUSWIDTH, unsigned int TS_BUSWIDTH > class I_BRIDGE;

class VCI2VCI32;
class PECORESS;

extern sc_time_unit glb_resolution_unit;

class CLUSTERSS : public sc_module
{
  protected:
    
  public:
    PECORESS                   *mpPECORESS[CL_PE_NUM];   // pointer of PECORESS's instance
    OSCI2DCDR<BUS_WIDTH_VCI32> *mpExRouter;      // pointer of OSCI2DCDR's instance(Cluster's router)
    I_CONNECTOR<BUS_WIDTH_VCI> *mpIsgpConnector; // pointer of I_CONNECTOR's instance(for P-Bus area)
    I_CONNECTOR<BUS_WIDTH_VCI> *mpIsgcConnector; // pointer of I_CONNECTOR's instance(for I-Bus area)
    I_CONNECTOR<BUS_WIDTH_VCI> *mpIshConnector;  // pointer of I_CONNECTOR's instance(for H-Bus area)
    I_CONNECTOR<BUS_WIDTH_VCI> *mpIsiConnector[CL_PE_NUM];       // pointer of I_CONNECTOR's instance(for INTC1)
    I_BRIDGE<BUS_WIDTH_VCI32,  BUS_WIDTH_VCI32> *mpIsBridge;     // pointer of I_BRIDGE's instance(for Other PE resource or GRAM)
    VCI2VCI32                  *mpIsgcVci2Vci32Bridge;           // pointer of VCI2VCI32's instance(for I-Bus area)
    VCI2VCI32                  *mpIsiVci2Vci32Bridge[CL_PE_NUM]; // pointer of VCI2VCI32's instance(for INTC1)
    DummySlave<BUS_WIDTH_APB> *mpSlvDummy_pegp[CL_PE_NUM]; // for Dummy memory for PEG register
    DummySlave<BUS_WIDTH_APB> *mpSlvDummy_pegm[CL_PE_NUM]; // for Dummy memory for PEGCAP_M register
    DummySlave<BUS_WIDTH_APB> *mpSlvDummy_pegs[CL_PE_NUM]; // for Dummy memory for PEGCAP_S register
    
    // clock port
    sc_in<sc_dt::uint64> sys_clk; // for cluster's clock port

    // reset port
    sc_in<bool>          sys_reset; // for reset port

    // bus socket : TlmInitiatorSocket (tlm::tlm_initiator_socket<>)
    TlmInitiatorSocket<BUS_WIDTH_VCI32> *is;  // for local RAM of other cluster's PE(for FastISS)
    TlmInitiatorSocket<BUS_WIDTH_VCI>   ish;  // for H-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI>   isgp; // for P-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI32> isgc; // for I-Bus area
    TlmInitiatorSocket<BUS_WIDTH_VCI32> *PE_isi[CL_PE_NUM]; // for INTC1
    TlmTargetSocket<BUS_WIDTH_VCI32>    *ts;  // for other Master's access(for FastISS)
    TlmTargetSocket<BUS_WIDTH_APB>      *PE_ts_peg_p[CL_PE_NUM];  // for PEG register
    TlmTargetSocket<BUS_WIDTH_APB>      *PE_ts_peg_m[CL_PE_NUM];  // for PEGCAP_M register
    TlmTargetSocket<BUS_WIDTH_APB>      *PE_ts_peg_s[CL_PE_NUM];  // for PEGCAP_S register

    // interrupt port for INTC1
    sc_in<bool>          *PE_FenmiReq[CL_PE_NUM]; // for fenmi request port
    sc_in<unsigned int>  *PE_FeintReq[CL_PE_NUM]; // for feint request port
    sc_in<sc_dt::uint64> *PE_EiintReq[CL_PE_NUM]; // for eiint request port
    sc_out<bool>         *PE_FenmiAck[CL_PE_NUM]; // for fenmi ack port
    sc_out<unsigned int> *PE_FeintAck[CL_PE_NUM]; // for feint ack port
    sc_out<unsigned int> *PE_EiintAck[CL_PE_NUM]; // for eiint ack port
    sc_out<unsigned int> *PE_CpuGmInfo[CL_PE_NUM]; // for CPU mode info notification port

    // interrupt port for Guard error
    sc_in<G4_PEG_Cerrinfo>  *PE_PegInfoIn[CL_PE_NUM];  // for input the error information to Master CPU
    sc_out<G4_PEG_Cerrinfo> *PE_PegInfoOut[CL_PE_NUM]; // for output the error information to Master CPU
    sc_out<bool>            *PE_PegErrM[CL_PE_NUM]; // for output the PEGCAP_M's error to ECM
    sc_out<bool>            *PE_PegErrS[CL_PE_NUM]; // for output the PEGCAP_M's error to ECM

  private:
    SIM_MODE_T     mIssMode;      // for the simulation mode 
    FASTISS_MODE_T mFastIssMode;  // for the simulation mode 
    const char    *mConfigFile;   // for the name of configulation file
    char mFastissRouterMapFname[1024];   // map file name for FastISS
    unsigned int mPeNum;

  public:
    CLUSTERSS( sc_module_name module_name, const char *config_file, unsigned int pe_num );
    ~CLUSTERSS( );
    void SetPeId( unsigned int pe0_peid, unsigned int pe1_peid );
    void setSyserrOut( unsigned int peid, unsigned int code );
    void setSyserrOperandOut( unsigned int peid,  unsigned int code );

  private:
    void read_config_file( const char *filename );
    void SetSimMode( SIM_MODE_T iss_mode );
    void SetFastIssMode( FASTISS_MODE_T fastiss_mode );
}; // end of CLUSTERSS definition

#endif /* CLUSTERSS_H */
