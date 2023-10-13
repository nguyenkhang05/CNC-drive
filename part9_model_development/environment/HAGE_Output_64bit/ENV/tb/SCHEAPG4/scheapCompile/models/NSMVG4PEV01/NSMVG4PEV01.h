/**********************************************************************
 *
 * NSMVG4PEV01.h
 *
 * Copyright(c) 2016-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 **********************************************************************/


#ifndef NSMVG4PEV01_H
#define NSMVG4PEV01_H

#include "tlm.h"      // for OSCI TLM2.0
#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0
#include "OSCI2DCDR.h"
#include "global.h"
#include "G4_intc_def.h"
#include "G4_intc_typ.h"
#include "DummySlave.h"

class NSMVG4CPUV01;
class G4_Intc1Wrapper;
class NSMVREGV01;
class ATLTSLAVE32;
template< unsigned int BUSWIDTH > class BACKWARD_DECODER;
template< unsigned int BUSWIDTH > class OSCI2DCDR_WITH_ERRSIG;

extern sc_time_unit glb_resolution_unit;

#define NSMVG4PEV01_INTC1_SELF_ADDRESS (ADDRESS_TYPE)0xFFFC0000
#define NSMVG4PEV01_INTC1_OFFS_ADDRESS (ADDRESS_TYPE)0x00004000

#ifdef IEEE_1666_SYSTEMC
#define SC_SIGNAL(T) sc_signal< T , SC_UNCHECKED_WRITERS>
#else
#define SC_SIGNAL(T) sc_signal< T >
#endif

class NSMVG4PEV01 : public sc_module
{
    
  public:
    // clock port
    sc_in<sc_dt::uint64> *CL_sys_clk[CL_MAX_NUM]; // for PE's clock port
    sc_in<sc_dt::uint64> VCI_clk;     // for VCI bus's clock port
    sc_in<sc_dt::uint64> SYS_BUS_clk; // for system interconnect's clock port
    sc_in<sc_dt::uint64> From_clk;    // for flash rom's clock port
    sc_in<sc_dt::uint64> Cram_clk;    // for Cluster ram's clock port

    // reset port
    sc_in<bool>          sys_reset; // for reset port

    // bus socket : TlmInitiatorSocket (tlm::tlm_initiator_socket<>)
    TlmInitiatorSocket<BUS_WIDTH_VCI>   ish;// for H_BUS via system interconnect
    TlmInitiatorSocket<BUS_WIDTH_VCI>*  CL_ish[CL_MAX_NUM]; // for H_BUS
    TlmInitiatorSocket<BUS_WIDTH_VCI>*  CL_isgp[CL_MAX_NUM];// for P_BUS
    TlmInitiatorSocket<BUS_WIDTH_VCI>   isgp; // for P_BUS
    TlmInitiatorSocket<BUS_WIDTH_VCI32> isgc; // for GVCI

    // bus socket : TlmTargetSocket (tlm::tlm_initiator_socket<>)
    TlmTargetSocket<BUS_WIDTH_FLSH> tsf;  // for flash I/F
    TlmTargetSocket<BUS_WIDTH_VCI>  tss;  // for system interconnect
    TlmTargetSocket<BUS_WIDTH_CRAM> tscr; // for Cluster ram I/F

    // interrupt port
    // for INTC1
    sc_in<bool>*          PE_eiint[PE_MAX_NUM][INTC1_CH_NUM];      // for PE's eiint port
    sc_in<bool>*          PE_eiint_type[PE_MAX_NUM][INTC1_CH_NUM]; // for PE's eiint_type port
    sc_in<bool>*          PE_feint[PE_MAX_NUM][FE_INTC1_CH_NUM];   // for feint port
    sc_in<bool>*          PE_feint_type[PE_MAX_NUM][FE_INTC1_CH_NUM];// for feint_type port
    sc_in<bool>           fenmi; // for fenmi port
    // for INTC2
    sc_in<uint64>*        eiint[INTC2_MAX_NUM][PE_MAX_NUM]; // for eiint port
    sc_out<unsigned int>* intack[PE_MAX_NUM];                     // for intack port
    void setTraceEnable( sc_trace_file* tf ); // for debug

    G4_Intc1Wrapper*  INTC1[PE_MAX_NUM];  // for the pointer of G4_Intc1Wrapper's instance

    BACKWARD_DECODER<BUS_WIDTH_VCI32>   *GVCI_BW_DECODER;// GVCI BACKWARD_DECODER
    OSCI2DCDR_WITH_ERRSIG<BUS_WIDTH_VCI32> *GVCI_DECODER;// GVCI decoder

    SC_SIGNAL(unsigned int)* cpugminfo_sig[PE_MAX_NUM];// signal for CPU guest mode information

  private: 
    const char *mConfigFile; // for the name of configulation file
    NSMVG4CPUV01* G4CPU;              // for the pointer of NSMVG4CPUV01's instance
    unsigned int   mPeNum;              // for the number of PE
    unsigned int   mClNum;              // for the number of cluster

    char           mMapFileName[1024];  // map file name

    DummySlave<BUS_WIDTH_VCI32>* GC_DUMMY_SLAVE[CL_MAX_NUM]; //dummy slave
    DummySlave<BUS_WIDTH_VCI>*   GP_DUMMY_SLAVE[CL_MAX_NUM]; //dummy slave
    DummySlave<BUS_WIDTH_VCI>*   HB_DUMMY_SLAVE[CL_MAX_NUM]; //dummy slav
    DummySlave<BUS_WIDTH_VCI32>* I1_DUMMY_SLAVE[PE_MAX_NUM]; //dummy slave

    // sc_singals
    // for INTC1-CPU connection
    sc_signal<uint64>*       eiint_sig[PE_MAX_NUM];   // signal for eiint
    sc_signal<unsigned int>* intack_sig[PE_MAX_NUM];  // signal for intack
    sc_signal<bool>*         fenmirq_sig[PE_MAX_NUM]; // signal for fenmirq
    sc_signal<bool>*         fenmiack_sig[PE_MAX_NUM];// signal for fenmiack
    sc_signal<unsigned int>* feintrq_sig[PE_MAX_NUM]; // signal for feintrq
    sc_signal<unsigned int>* feintack_sig[PE_MAX_NUM];// signal for feintack

  public:

    NSMVG4PEV01( sc_module_name module_name, const char *config_file );
    ~NSMVG4PEV01();

    void read_config_file( const char *filename );

    NSMVG4CPUV01* getG4cpuPtr( void );

}; // end of NSMVG4PEV01 definition

#endif /* NSMVG4PEV01_H */
