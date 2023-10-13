/***********************************************************************
 *
 * NSMVG4SSV01.h
 *
 * Copyright(c) 2016-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 ***********************************************************************/

#ifndef NSMVG4SSV01_H
#define NSMVG4SSV01_H

#include "tlm.h"      // for OSCI TLM2.0
#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0
#include "global.h"
#include "G4_intc_def.h"
#include "slct_sc_signal_ports.h"
#include "DummySlave.h"

class NSMVG4PEV01;
class G4_Intc2Wrapper;
class G4_IPIR;
class G4_OR_IPIR;
class NSMVTPTMV01;
class NSMVMEVV02;
class NSMVBARRV01; 
class ATLTSLAVE32;
class APISS;
class AXISS_E2;
class AXI2VCI;
class VCI2APB;
class VCI2AXI;
class IBG;
class IBG_U2;
class INTC2G;
class INTC2G_U2;
template< unsigned int BUSWIDTH > class OSCI2DCDR;
template< unsigned int BUSWIDTH > class BACKWARD_DECODER;
struct Cerrinfo;

#ifdef IEEE_1666_SYSTEMC
#define SC_SIGNAL(T) sc_signal< T , SC_UNCHECKED_WRITERS>
#else
#define SC_SIGNAL(T) sc_signal< T >
#endif

class NSMVG4SSV01 : public sc_module
{
 protected:
  const char *mConfigFile; // for the name of configulation file
  
 public:
  AXISS_E2                  *H_BUS;                  // H_BUS
  APISS                     *P_BUS;                  // P_BUS
  BACKWARD_DECODER<BUS_WIDTH_VCI> *BW_DECODER;       // VCI BACKWARD_DECODER
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_PFSS;              // APB for PFSS
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG1;               // APB for SG1
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG2_0;             // APB for SG2_0
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG2_1;             // APB for SG2_1
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG3;               // APB for SG3
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG4;               // APB for SG4
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG5;               // APB for SG5
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG6;               // APB for SG6
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG7;               // APB for SG7
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG8;               // APB for SG8
  OSCI2DCDR<BUS_WIDTH_APB>  *GAPB_SG9;               // APB for SG9
  AXI2VCI                   *AXI2VCI_BRIDGE;         // AXI2VCI
  VCI2AXI                   *VCI2AXI_BRIDGE;         // VCI2AXI
  VCI2APB                   *VCI2APB_BRIDGE_PFSS;    // VCI2APB PFSS
  VCI2APB                   *VCI2APB_BRIDGE_SG1;     // VCI2APB SG1
  VCI2APB                   *VCI2APB_BRIDGE_SG2_0;   // VCI2APB SG2_0
  VCI2APB                   *VCI2APB_BRIDGE_SG2_1;   // VCI2APB SG2_1
  VCI2APB                   *VCI2APB_BRIDGE_SG3;     // VCI2APB SG3
  VCI2APB                   *VCI2APB_BRIDGE_SG4;     // VCI2APB SG4
  VCI2APB                   *VCI2APB_BRIDGE_SG5;     // VCI2APB SG5
  VCI2APB                   *VCI2APB_BRIDGE_SG6;     // VCI2APB SG6
  VCI2APB                   *VCI2APB_BRIDGE_SG7;     // VCI2APB SG7
  VCI2APB                   *VCI2APB_BRIDGE_SG8;     // VCI2APB SG8
  VCI2APB                   *VCI2APB_BRIDGE_SG9;     // VCI2APB SG9
  VCI2AXI                   *CL_VCI2AXI_BRIDGE[CL_MAX_NUM];//CLSTR's VCI2AXI
  
  slct_sc_in<sc_dt::uint64> *CL_sys_clk[CL_MAX_NUM];// for CLSTR's clock port
  slct_sc_in<sc_dt::uint64> GAPB_PFSS_clk;          // GAPB PFSS bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG1_clk;           // GAPB SG1 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG2_0_clk;         // GAPB SG2_0 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG2_1_clk;         // GAPB SG2_1 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG3_clk;           // GAPB SG3 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG4_clk;           // GAPB SG4 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG5_clk;           // GAPB SG5 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG6_clk;           // GAPB SG6 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG7_clk;           // GAPB SG7 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG8_clk;           // GAPB SG8 bus clock
  slct_sc_in<sc_dt::uint64> GAPB_SG9_clk;           // GAPB SG9 bus clock
  slct_sc_in<sc_dt::uint64> VCI_clk;                // G/PVCI bus clock
  slct_sc_in<sc_dt::uint64> AXI_clk;                // HBUS bus clock
  slct_sc_in<sc_dt::uint64> From_clk;               // FROM I/F clock
  slct_sc_in<sc_dt::uint64> Cram_clk;               // CRAM I/F clock
  slct_sc_in<sc_dt::uint64> SYS_BUS_clk;            // system interconnect clock
  
  slct_sc_in<bool>          sys_reset;               // reset port
  
  slct_sc_in<bool>           fenmi;                                      // fenmi port
  slct_sc_in<bool>          *PE_feint[PE_MAX_NUM][FE_INTC1_CH_NUM];      // feint port
  slct_sc_in<bool>          *PE_feint_type[PE_MAX_NUM][FE_INTC1_CH_NUM]; // feint_type port
  slct_sc_in<bool>          *PE_eiint[PE_MAX_NUM][EI_INTC1_CH_NUM];      // PE's eiint port
  slct_sc_in<bool>          *PE_eiint_type[PE_MAX_NUM][EI_INTC1_CH_NUM]; // PE's eiint_type port
  slct_sc_in<bool>          *eiint[EI_ALL_CH_NUM - EI_INTC1_CH_NUM];     // eiint port
  slct_sc_in<bool>          *eiint_type[EI_ALL_CH_NUM - EI_INTC1_CH_NUM];// eiint_type port
  
  TlmTargetSocket<BUS_WIDTH_AXI>    tsx;      //target socket for AXI
  TlmTargetSocket<BUS_WIDTH_FLSH>   tsf;      //target socket for FROM
  TlmInitiatorSocket<BUS_WIDTH_AXI> isx;      //initiator socket for AXI
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg1;  //initiator socket for GAPB SG1
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg2_0;//initiator socket for GAPB SG2_0
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg2_1;//initiator socket for GAPB SG2_1
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg3;  //initiator socket for GAPB SG3
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg4;  //initiator socket for GAPB SG4
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg5;  //initiator socket for GAPB SG5
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg6;  //initiator socket for GAPB SG6
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg7;  //initiator socket for GAPB SG7
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg8;  //initiator socket for GAPB SG8
  TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg9;  //initiator socket for GAPB SG9
  
  NSMVG4PEV01*      G4PE;           // NSMVG4PEV01 instance
  G4_Intc2Wrapper** INTC2;          // INTC2 instance
  G4_IPIR*          IPIR;           // IPIR instance
  G4_OR_IPIR**      OR_IPIR;        // OR instance
  NSMVMEVV02*       MEV;            // MEV instance
  NSMVBARRV01*      BARR;           // BARR instance
  NSMVTPTMV01*      TPTM;           // TPTM instance

  IBG*              GUARD_BARR;     // IBG instarnce  
  IBG*              GUARD_IPIR;     // IBG instarnce  
  IBG*              GUARD_MEV;      // IBG instarnce  
  IBG*              GUARD_TPTM;     // IBG instarnce
  IBG_U2*           GUARD_BARR_U2;  // IBG instarnce for U2
  IBG_U2*           GUARD_IPIR_U2;  // IBG instarnce for U2
  IBG_U2*           GUARD_TPTM_U2;  // IBG instarnce for U2

  INTC2G*           GUARD_INTC2[INTC2_MAX_NUM];// INTC2G instance for E2x
  INTC2G_U2*        GUARD_INTC2_U2[INTC2_MAX_NUM];// INTC2G instance for U2

  DummySlave<BUS_WIDTH_VCI32>* GC_DUMMY_SLAVE; //dummy slave

  sc_signal<uint64>*       i2_g_eiint_sig[INTC2_MAX_NUM][PE_MAX_NUM]; // for INTC2
#ifndef __VLAB_MIT__
  SC_SIGNAL(bool)*         ipir_sig[PE_MAX_NUM][INTC_IPIR_CH_NUM]; // for IPIR
#endif
  sc_signal<bool>*         i2_bcast_sig[INTC2_MAX_NUM][INTC_BCAST_NUM]; // for INTC2
  sc_signal<unsigned int>* i1_p_intack_sig[PE_MAX_NUM];                 // for INTC1
  sc_signal<bool>*         ipir2eiint_sig[PE_MAX_NUM][INTC1_CH_NUM]; // for INTC1
  sc_signal<bool>*         swdt_clk_sig[PE_MAX_NUM];        // for connection of SWDT to CPU
  sc_signal<bool>*         sys_cpinit_sig[PE_MAX_NUM];      // for connection of BOOTCTRL to CPU
  sc_signal<bool>*         pe_fli_areaerr_sig[PE_MAX_NUM];  // for connection of ECM to CPU (not use for E2xFCC1)
  sc_signal<bool>*         pe_cram_areaerr_sig[PE_MAX_NUM]; // for connection of ECM to CPU
  sc_signal<bool>*         pe_peg_err_m_sig[PE_MAX_NUM];    // for connection of ECM to CPU
  sc_signal<bool>*         pe_peg_err_s_sig[PE_MAX_NUM];    // for connection of ECM to CPU
  sc_signal<bool>*         exm_fli_areaerr_sig;             // for connection of ECM to CPU (not use for E2xFCC1)
  sc_signal<bool>*         exm_cram_areaerr_sig;            // for connection of ECM to CPU
  sc_signal<bool>*         cram_grderr_sig;                 // for connection of ECM to CPU
  sc_signal<bool>*         pe_fli_grderr_sig[PE_MAX_NUM];   // for connection of ICUP to CPU (not use for E2xFCC1)
  sc_signal<bool>*         exm_fli_grderr_sig;              // for connection of ICUP to CPU (not use for E2xFCC1)
  SC_SIGNAL(bool)*         gvci_areaerr_sig;                // for connection of ECM(74)

  sc_signal<bool>*         ipir_init_ch_sig[INTC_IPIR_CH_NUM]; // for IPIR ipir_init_ch port
  sc_signal<bool>          ipir_ext_err_dummy_sig;             // dummy for IPIR ext_err port
  sc_signal<bool>**        tptm_irq_sig;                    // for TPTM TPTM_IRQ port (for INTERVAL_TIMER)
  sc_signal<bool>***       tptm_inttptmu_sig;               // for TPTM INTTPTMU port (for UP_COUNTER_TIMER)

  sc_signal<bool>*         guard_barr_err_irq_sig;        // for err_irq of IBG
  sc_signal<bool>*         guard_ipir_err_irq_sig;        // for err_irq of IBG
  sc_signal<bool>*         guard_mev_err_irq_sig;         // for err_irq of IBG
  sc_signal<bool>*         guard_tptm_err_irq_sig;        // for err_irq of IBG

  sc_signal<bool>*         guard_intc2_err_irq_sig[INTC2_MAX_NUM];// for err_irq of INTC2G

  sc_trace_file*           mpTf; //trace file

  // config parameters with some default values
  // ----------------------------------------
  
  NSMVG4SSV01(sc_module_name module_name, const char *config_file);
  
  ~NSMVG4SSV01(void);
  
  void start_of_simulation( void );
  
  void read_config_file( const char *filename );

  void SetSimMode( SIM_MODE_T iss_mode );
  
  char mMapFileName[1024];            // bus map file
  
  unsigned int mPeNum;        // PE number
  unsigned int mClNum;        // CLSTR number
  unsigned int mIntc2Num;     // INTC number
  SIM_MODE_T   mIssMode;      // for the simulation mode 
  const char*  mpModName;     // module name
  G4_PFSS_T mPfssType;        // PFSS type
  unsigned char mGuardBarrChNum;// channel number for IBG
  unsigned char mGuardIpirChNum;// channel number for IBG
  unsigned char mGuardMevChNum; // channel number for IBG
  unsigned char mGuardTptmChNum;// channel number for IBG

  void setTraceEnable( sc_trace_file* tf );

}; // end of NSMVG4SSV01 definition

#endif /* NSMVG4SSV01_H */
