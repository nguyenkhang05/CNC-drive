/*************************************************************************
 *
 *  pltfrmGND.cpp
 *
 * Copyright(c) 2016-2018 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 *
 *************************************************************************/

//************* User include header *************//
#include "NSMVG4SSV01.h"
#include "NSMVG4CPUV01.h"
#include "CLUSTERSS.h"
#include "VCI2APB.h"
#include "systemc.h"
#include "OSCI2.h"     // class for User Modeling environment based on TLM2.0
#include "PFRH850.hpp"
#include "ATLTSLAVE32.h"
#include "ATLTSLAVE64.h"
#include "DummyMaster.h"
#include "IBG.h"
#include "IBG_U2.h"
#include "slct_sc_signal_ports.h"
#include "NSMVCRGV01.h"
#include "NSMVPEGV01.h"
#include "G4_intc_def.h"
#include "INTC2G.h"
#include "INTC2G_U2.h"

using namespace sc_dt;

extern sc_signal<uint64, sc_core::SC_UNCHECKED_WRITERS> *glb_pe_clk_sig; // temporary implementation before CLK-GEN being made

namespace PltfrmGnd
{
  // for fenmi port
  sc_signal<bool>  *fenmi_sig;
  // for PE's feint port
  sc_signal<bool>  *PE_feint_sig[PE_MAX_NUM][FE_INTC1_CH_NUM];
  // for PE's feint port
  sc_signal<bool>  *PE_feint_type_sig[PE_MAX_NUM][FE_INTC1_CH_NUM];
  // for PE's eiint port
  sc_signal<bool>  *PE_eiint_sig[PE_MAX_NUM][INTC1_CH_NUM];
  // for PE's eiint port
  sc_signal<bool>  *PE_eiint_type_sig[PE_MAX_NUM][INTC1_CH_NUM];
  // for eiint port
  sc_signal<bool>  *eiint_sig[EI_ALL_CH_NUM - EI_INTC1_CH_NUM];
  // for eiint port
  sc_signal<bool>  *eiint_type_sig[EI_ALL_CH_NUM - EI_INTC1_CH_NUM];

  //temporal implementation before resetIP being made
  sc_signal<bool>  *reset_sig;

  // global APB slave SG1
  ATLTSLAVE32                 *GAPB_SLAVE_SG1;
  // global APB slave SG2_0
  ATLTSLAVE32                 *GAPB_SLAVE_SG2_0;
  // global APB slave SG2_1
  ATLTSLAVE32                 *GAPB_SLAVE_SG2_1;
  // global APB slave SG3
  ATLTSLAVE32                 *GAPB_SLAVE_SG3;
  // global APB slave SG4
  ATLTSLAVE32                 *GAPB_SLAVE_SG4;
  // global APB slave SG5
  ATLTSLAVE32                 *GAPB_SLAVE_SG5;
  // global APB slave SG6
  ATLTSLAVE32                 *GAPB_SLAVE_SG6;
  // global APB slave SG7
  ATLTSLAVE32                 *GAPB_SLAVE_SG7;
  // global APB slave SG8
  ATLTSLAVE32                 *GAPB_SLAVE_SG8;
  // global APB slave SG9
  ATLTSLAVE32                 *GAPB_SLAVE_SG9;
  // AXI slave
  ATLTSLAVE64                 *AXI_SLAVE;
  // AXI dummy master
  DummyMaster<BUS_WIDTH_AXI>  *AXI_DUMMY_MASTER;
  // Flash ROM dummy master
  DummyMaster<BUS_WIDTH_FLSH> *FLASH_DUMMY_MASTER;
  // dummy master for APG_SG0.tsg
  DummyMaster<BUS_WIDTH_APB>  *APB_DUMMY_MASTER_0;
  // dummy master for APG_SG0.tse
  DummyMaster<BUS_WIDTH_APB>  *APB_DUMMY_MASTER_1;
  // dummy master for GUARD_BARR.ts
  DummyMaster<BUS_WIDTH_APB>  *GUARD_BARR_DUMMY_MASTER;
  // dummy master for GUARD_IPIR.ts
  DummyMaster<BUS_WIDTH_APB>  *GUARD_IPIR_DUMMY_MASTER;
  // dummy master for GUARD_MEV.ts
  DummyMaster<BUS_WIDTH_APB>  *GUARD_MEV_DUMMY_MASTER;
  // dummy master for GUARD_TPTM.ts
  DummyMaster<BUS_WIDTH_APB>  *GUARD_TPTM_DUMMY_MASTER;
  // dummy master for GUARD_INTC2.ts
  DummyMaster<BUS_WIDTH_APB>  *GUARD_INTC2_DUMMY_MASTER[INTC2_MAX_NUM];

  // for CRG test
  NSMVCRGV01 *GUARD_CRAMCLm[CL_MAX_NUM];
  NSMVCRGV01 *CRGCAP_PEnCLm[PE_MAX_NUM];
  NSMVCRGV01 *CRGCAP_CRAMHCLm[CL_MAX_NUM];
  NSMVCRGV01 *CRGCAP_CRAMLCLm[CL_MAX_NUM];
  NSMVCRGV01 *CRGCAP_SX2MBHCLm[CL_MAX_NUM];
  NSMVCRGV01 *CRGCAP_SX2MBLCLm[CL_MAX_NUM];
  NSMVCRGV01 *GUARD_CRAMCRGm1[CL_MAX_NUM];
  NSMVCRGV01 *GUARD_CRSECCLm[CL_MAX_NUM];
  // for PEG test
  NSMVPEGV01 *GUARD_PEnCLm[PE_MAX_NUM];
  NSMVPEGV01 *PEGCAP_M_PEnCLm[PE_MAX_NUM];
  NSMVPEGV01 *PEGCAP_S_PEnCLm[PE_MAX_NUM];
}

using namespace PltfrmGnd;

//************* pltfrm *************//

/// <summary>platform connection function for unused ports</summary>
/// <param name="G4SS">G4SS pointer</param>
/// <param name="pfRh850">PFRH850 pointer</param>
/// <param name="config_file">configration file name</param>
void pltfrmGND(NSMVG4SSV01 *G4SS, PFRH850 *pfRh850, const char *config_file=NULL)
{
#if defined(PLTFRM_TST)
  cout << "pltfrmGND() was called." << endl;
#endif //PLTFRM_TST

  char sname[64];

  //----- G4SS initiator socket connection
  GAPB_SLAVE_SG1  = new ATLTSLAVE32("GAPB_SLAVE_SG1",0,0);
  GAPB_SLAVE_SG2_0= new ATLTSLAVE32("GAPB_SLAVE_SG2_0",0,0);
  GAPB_SLAVE_SG2_1= new ATLTSLAVE32("GAPB_SLAVE_SG2_1",0,0);
  GAPB_SLAVE_SG3  = new ATLTSLAVE32("GAPB_SLAVE_SG3",0,0);
  GAPB_SLAVE_SG4  = new ATLTSLAVE32("GAPB_SLAVE_SG4",0,0);
  GAPB_SLAVE_SG5  = new ATLTSLAVE32("GAPB_SLAVE_SG5",0,0);
  GAPB_SLAVE_SG6  = new ATLTSLAVE32("GAPB_SLAVE_SG6",0,0);
  GAPB_SLAVE_SG7  = new ATLTSLAVE32("GAPB_SLAVE_SG7",0,0);
  GAPB_SLAVE_SG8  = new ATLTSLAVE32("GAPB_SLAVE_SG8",0,0);
  GAPB_SLAVE_SG9  = new ATLTSLAVE32("GAPB_SLAVE_SG9",0,0);
  AXI_SLAVE       = new ATLTSLAVE64("AXI_SLAVE",0,0);
  G4SS->isg_sg1(*(GAPB_SLAVE_SG1->ts));
  G4SS->isg_sg2_0(*(GAPB_SLAVE_SG2_0->ts));
  G4SS->isg_sg2_1(*(GAPB_SLAVE_SG2_1->ts));
  G4SS->isg_sg3(*(GAPB_SLAVE_SG3->ts));
  G4SS->isg_sg4(*(GAPB_SLAVE_SG4->ts));
  G4SS->isg_sg5(*(GAPB_SLAVE_SG5->ts));
  G4SS->isg_sg6(*(GAPB_SLAVE_SG6->ts));
  G4SS->isg_sg7(*(GAPB_SLAVE_SG7->ts));
  G4SS->isg_sg8(*(GAPB_SLAVE_SG8->ts));
  G4SS->isg_sg9(*(GAPB_SLAVE_SG9->ts));
  G4SS->isx(*(AXI_SLAVE->ts));

  //----- G4SS target socket connection
  AXI_DUMMY_MASTER = new DummyMaster<BUS_WIDTH_AXI>("AXI_DUMMY_MASTER");
  FLASH_DUMMY_MASTER=new DummyMaster<BUS_WIDTH_FLSH>("FLASH_DUMMY_MASTER");
  AXI_DUMMY_MASTER->is(G4SS->tsx);
  FLASH_DUMMY_MASTER->is(G4SS->tsf);

  //----- dummy master connection for IBG at no peripheral
  if( ( !pfRh850 ) && ( 1 < G4SS->mPeNum ) ) {
    GUARD_BARR_DUMMY_MASTER = new DummyMaster<BUS_WIDTH_APB>("GUARD_BARR_DUMMY_MASTER");
    GUARD_IPIR_DUMMY_MASTER = new DummyMaster<BUS_WIDTH_APB>("GUARD_IPIR_DUMMY_MASTER");
    if( U2A_PFSS == G4SS->mPfssType ){
      GUARD_BARR_DUMMY_MASTER->is(*(G4SS->GUARD_BARR_U2->ts));
      GUARD_IPIR_DUMMY_MASTER->is(*(G4SS->GUARD_IPIR_U2->ts));
      GUARD_MEV_DUMMY_MASTER = 0;
    }
    else{
      GUARD_MEV_DUMMY_MASTER = new DummyMaster<BUS_WIDTH_APB>("GUARD_MEV_DUMMY_MASTER");
      GUARD_BARR_DUMMY_MASTER->is(*(G4SS->GUARD_BARR->ts));
      GUARD_IPIR_DUMMY_MASTER->is(*(G4SS->GUARD_IPIR->ts));
      GUARD_MEV_DUMMY_MASTER->is(*(G4SS->GUARD_MEV->ts));
    }
  }
  else{
    GUARD_BARR_DUMMY_MASTER = 0;
    GUARD_IPIR_DUMMY_MASTER = 0;
    GUARD_MEV_DUMMY_MASTER = 0;
  }
  //----- dummy master connection for IBG at no peripheral(TPTM)
  if( !pfRh850 ) {
    GUARD_TPTM_DUMMY_MASTER = new DummyMaster<BUS_WIDTH_APB>("GUARD_TPTM_DUMMY_MASTER");
    if( U2A_PFSS == G4SS->mPfssType ){
      GUARD_TPTM_DUMMY_MASTER->is(*(G4SS->GUARD_TPTM_U2->ts));
    }
    else{
      GUARD_TPTM_DUMMY_MASTER->is(*(G4SS->GUARD_TPTM->ts));
    }
  }
  else {
    GUARD_TPTM_DUMMY_MASTER = 0;
  }

  //----- dummy master connection for INTC2G
  if( !pfRh850 ) {
    char tname[64];
    for( int i=0; i<INTC2_MAX_NUM; i++){
      if( U2A_PFSS == G4SS->mPfssType ){
	if( G4SS->GUARD_INTC2_U2[i] ){
	  sprintf( tname, "GUARD_INTC2_DUMMY_MASTER_%d", i );
	  GUARD_INTC2_DUMMY_MASTER[i] = new DummyMaster<BUS_WIDTH_APB>(tname);
	  GUARD_INTC2_DUMMY_MASTER[i]->is(*(G4SS->GUARD_INTC2_U2[i]->tsp));
	}
	else{
	  GUARD_INTC2_DUMMY_MASTER[i] = 0;
	}
      }
      else{
	if( G4SS->GUARD_INTC2[i] ){
	  sprintf( tname, "GUARD_INTC2_DUMMY_MASTER_%d", i );
	  GUARD_INTC2_DUMMY_MASTER[i] = new DummyMaster<BUS_WIDTH_APB>(tname);
	  GUARD_INTC2_DUMMY_MASTER[i]->is(*(G4SS->GUARD_INTC2[i]->ts));
	}
	else{
	  GUARD_INTC2_DUMMY_MASTER[i] = 0;
	}
      }
    }
  }


  //----- reset connection
  reset_sig = new sc_signal<bool>( "reset_sig" );
  *reset_sig = true;
  if( !pfRh850 ) {
    G4SS->sys_reset(*reset_sig);
    if( 1 < G4SS->mPeNum ) {
      if( U2A_PFSS == G4SS->mPfssType ){
	G4SS->GUARD_BARR_U2->PRESETn( *reset_sig );
	G4SS->GUARD_IPIR_U2->PRESETn( *reset_sig );
      }
      else{
	G4SS->GUARD_BARR->PRESETn( *reset_sig );
	G4SS->GUARD_IPIR->PRESETn( *reset_sig );
	G4SS->GUARD_MEV->PRESETn( *reset_sig );
      }
    }
    if( U2A_PFSS == G4SS->mPfssType ){
      G4SS->GUARD_TPTM_U2->PRESETn( *reset_sig );
    }
    else{
      G4SS->GUARD_TPTM->PRESETn( *reset_sig );
    }
  }  // added by request from ASTC
  GAPB_SLAVE_SG1->resetPort(*reset_sig);
  GAPB_SLAVE_SG2_0->resetPort(*reset_sig);
  GAPB_SLAVE_SG2_1->resetPort(*reset_sig);
  GAPB_SLAVE_SG3->resetPort(*reset_sig);
  GAPB_SLAVE_SG4->resetPort(*reset_sig);
  GAPB_SLAVE_SG5->resetPort(*reset_sig);
  GAPB_SLAVE_SG6->resetPort(*reset_sig);
  GAPB_SLAVE_SG7->resetPort(*reset_sig);
  GAPB_SLAVE_SG8->resetPort(*reset_sig);
  GAPB_SLAVE_SG9->resetPort(*reset_sig);
  AXI_SLAVE->resetPort(*reset_sig);

  //----- clock port connection
  for(int i=0; i<CL_MAX_NUM; i++ ){
    (*((G4SS->CL_sys_clk)[i]))(*glb_pe_clk_sig);
  }
  G4SS->GAPB_PFSS_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG1_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG2_0_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG2_1_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG3_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG4_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG5_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG6_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG7_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG8_clk(*glb_pe_clk_sig);
  G4SS->GAPB_SG9_clk(*glb_pe_clk_sig);
  G4SS->VCI_clk(*glb_pe_clk_sig);
  G4SS->AXI_clk(*glb_pe_clk_sig);
  G4SS->From_clk(*glb_pe_clk_sig);
  G4SS->Cram_clk(*glb_pe_clk_sig);
  G4SS->SYS_BUS_clk(*glb_pe_clk_sig);
  GAPB_SLAVE_SG1->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG2_0->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG2_1->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG3->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG4->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG5->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG6->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG7->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG8->freqPort(*glb_pe_clk_sig);
  GAPB_SLAVE_SG9->freqPort(*glb_pe_clk_sig);
  AXI_SLAVE->freqPort(*glb_pe_clk_sig);
#if 0 //The connection is being confirmed
  if( !pfRh850 ){
    if( 1 < G4SS->mPeNum ) {
      G4SS->GUARD_BARR->PCLK( *glb_pe_clk_sig );
      G4SS->GUARD_IPIR->PCLK( *glb_pe_clk_sig );
      G4SS->GUARD_MEV->PCLK( *glb_pe_clk_sig );
    }
    G4SS->GUARD_TPTM->PCLK( *glb_pe_clk_sig );
  }
#endif//The connection is being confirmed
  if( !pfRh850 ){
    if( U2A_PFSS == G4SS->mPfssType ){
      if( 1 < G4SS->mPeNum ) {
	G4SS->GUARD_BARR_U2->PCLK( *glb_pe_clk_sig );
	G4SS->GUARD_IPIR_U2->PCLK( *glb_pe_clk_sig );
      }
      G4SS->GUARD_TPTM_U2->PCLK( *glb_pe_clk_sig );
    }
  }

  //----- G4SS fenmi connection
  sprintf(sname, "fenmi_sig");
  fenmi_sig = new sc_signal<bool>(sname); 
  *fenmi_sig = false;
  G4SS->fenmi(*fenmi_sig, false);

  //----- G4SS PE's feint and feint_type connection
  for(int i=0; i<PE_MAX_NUM; i++){
    for ( int j = 0; j < FE_INTC1_CH_NUM; j++ ) {
      sprintf(sname, "PE%d_feint%d_sig", i, j );
      PE_feint_sig[i][j] = new sc_signal<bool>(sname);
      *(PE_feint_sig[i][j]) = false;
      (*((G4SS->PE_feint)[i][j]))(*(PE_feint_sig[i][j]), false);
      
      sprintf(sname, "PE%d_feint_type%d_sig", i, j );
      PE_feint_type_sig[i][j] = new sc_signal<bool>(sname); 
      *(PE_feint_type_sig[i][j]) = false;
      (*((G4SS->PE_feint_type)[i][j]))(*(PE_feint_type_sig[i][j]), false);
    }
  }

  //----- G4SS PE's eiint and eiint_type connection
  for(int i=0; i<PE_MAX_NUM; i++){
    for ( int j = 0; j < INTC1_CH_NUM; j++ ) {
      sprintf(sname, "PE%d_eiint%d_sig", i, j);
      PE_eiint_sig[i][j] = new sc_signal<bool>(sname);
      *(PE_eiint_sig[i][j]) = false;
      (*((G4SS->PE_eiint)[i][j]))(*(PE_eiint_sig[i][j]), false);

      sprintf(sname, "PE%d_eiint_type%d_sig", i, j);
      PE_eiint_type_sig[i][j] = new sc_signal<bool>(sname); 
      *(PE_eiint_type_sig[i][j]) = false;
      (*((G4SS->PE_eiint_type)[i][j]))(*(PE_eiint_type_sig[i][j]), false);
    }
  }

  //----- G4SS eiint and eiint_type connection
  for (int i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    sprintf( sname, "eiint%d_sig", i+INTC1_CH_NUM );
    eiint_sig[i] = new sc_signal<bool>( sname );
    *(eiint_sig[i]) = false;
    (*((G4SS->eiint)[i]))(*(eiint_sig[i]), false);

    sprintf( sname, "eiint_type%d_sig", i+INTC1_CH_NUM );
    eiint_type_sig[i] = new sc_signal<bool>( sname );
    *(eiint_type_sig[i]) = false;
    (*((G4SS->eiint_type)[i]))(*(eiint_type_sig[i]), false);
  }

  //----- for CRG and PEG instantiation and connection (for unit test)
  if( !pfRh850 ) {
    // for CRG test
    char mod_name[64];
    for (int i=0; i<CL_MAX_NUM; i++){
      if(i<G4SS->mClNum){
	sprintf(mod_name, "GUARD_CRAMCL%d", i);
	GUARD_CRAMCLm[i] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(GUARD_CRAMCLm[i]->ts));
	GUARD_CRAMCLm[i]->clk(*glb_pe_clk_sig);
	GUARD_CRAMCLm[i]->resetz(*reset_sig);
      
	sprintf(mod_name, "GUARD_CRAMCRG%d1", i); // for U2A
	GUARD_CRAMCRGm1[i] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(GUARD_CRAMCRGm1[i]->ts));
	GUARD_CRAMCRGm1[i]->clk(*glb_pe_clk_sig);
	GUARD_CRAMCRGm1[i]->resetz(*reset_sig);

	sprintf(mod_name, "GUARD_CRSECCL%d", i); // for U2A
	GUARD_CRSECCLm[i] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(GUARD_CRSECCLm[i]->ts));
	GUARD_CRSECCLm[i]->clk(*glb_pe_clk_sig);
	GUARD_CRSECCLm[i]->resetz(*reset_sig);

	sprintf(mod_name, "CRGCAP_PE%dCL%d", i*2, i);
	CRGCAP_PEnCLm[i*2] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(CRGCAP_PEnCLm[i*2]->ts));
	CRGCAP_PEnCLm[i*2]->clk(*glb_pe_clk_sig);
	CRGCAP_PEnCLm[i*2]->resetz(*reset_sig);
	if(G4SS->mPeNum > 1){
	  sprintf(mod_name, "CRGCAP_PE%dCL%d", i*2+1, i);
	  CRGCAP_PEnCLm[i*2+1] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	  G4SS->GAPB_PFSS->is(*(CRGCAP_PEnCLm[i*2+1]->ts));
	  CRGCAP_PEnCLm[i*2+1]->clk(*glb_pe_clk_sig);
	  CRGCAP_PEnCLm[i*2+1]->resetz(*reset_sig);
	}

	sprintf(mod_name, "CRGCAP_CRAMHCL%d", i);
	CRGCAP_CRAMHCLm[i] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(CRGCAP_CRAMHCLm[i]->ts));
	CRGCAP_CRAMHCLm[i]->clk(*glb_pe_clk_sig);
	CRGCAP_CRAMHCLm[i]->resetz(*reset_sig);
	
	sprintf(mod_name, "CRGCAP_CRAMLCL%d", i);
	CRGCAP_CRAMLCLm[i] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(CRGCAP_CRAMLCLm[i]->ts));
	CRGCAP_CRAMLCLm[i]->clk(*glb_pe_clk_sig);
	CRGCAP_CRAMLCLm[i]->resetz(*reset_sig);

	sprintf(mod_name, "CRGCAP_SX2MBHCL%d", i);
	CRGCAP_SX2MBHCLm[i] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(CRGCAP_SX2MBHCLm[i]->ts));
	CRGCAP_SX2MBHCLm[i]->clk(*glb_pe_clk_sig);
	CRGCAP_SX2MBHCLm[i]->resetz(*reset_sig);

	sprintf(mod_name, "CRGCAP_SX2MBLCL%d", i);
	CRGCAP_SX2MBLCLm[i] = new NSMVCRGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(CRGCAP_SX2MBLCLm[i]->ts));
	CRGCAP_SX2MBLCLm[i]->clk(*glb_pe_clk_sig);
	CRGCAP_SX2MBLCLm[i]->resetz(*reset_sig);
      }else{
	GUARD_CRAMCLm[i]       = 0;
	CRGCAP_PEnCLm[i*2]     = 0;
	CRGCAP_PEnCLm[(i*2)+1] = 0;
	CRGCAP_CRAMHCLm[i]     = 0;
	CRGCAP_CRAMLCLm[i]     = 0;
	CRGCAP_SX2MBHCLm[i]    = 0;
	CRGCAP_SX2MBLCLm[i]    = 0;
      }
    }
    // for PEG test
    for (int i=0; i<PE_MAX_NUM; i++){
      if(i < G4SS->mPeNum){
	sprintf(mod_name, "GUARD_PE%dCL%d", i, i/2);
	GUARD_PEnCLm[i] = new NSMVPEGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(GUARD_PEnCLm[i]->ts));
	GUARD_PEnCLm[i]->clk(*glb_pe_clk_sig);
	GUARD_PEnCLm[i]->resetz(*reset_sig);
	(*(GUARD_PEnCLm[i]->is))(*(G4SS->G4PE->getG4cpuPtr()->mpCluster[i/2]->PE_ts_peg_p[i%2]));

	sprintf(mod_name, "PEGCAP_M_PE%dCL%d", i, i/2);
	PEGCAP_M_PEnCLm[i] = new NSMVPEGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(PEGCAP_M_PEnCLm[i]->ts));
	PEGCAP_M_PEnCLm[i]->clk(*glb_pe_clk_sig);
	PEGCAP_M_PEnCLm[i]->resetz(*reset_sig);
	(*(PEGCAP_M_PEnCLm[i]->is))(*(G4SS->G4PE->getG4cpuPtr()->mpCluster[i/2]->PE_ts_peg_m[i%2]));

	sprintf(mod_name, "PEGCAP_S_PE%dCL%d", i, i/2);
	PEGCAP_S_PEnCLm[i] = new NSMVPEGV01(mod_name, G4SS->G4PE->getG4cpuPtr());
	G4SS->GAPB_PFSS->is(*(PEGCAP_S_PEnCLm[i]->ts));
	PEGCAP_S_PEnCLm[i]->clk(*glb_pe_clk_sig);
	PEGCAP_S_PEnCLm[i]->resetz(*reset_sig);
	(*(PEGCAP_S_PEnCLm[i]->is))(*(G4SS->G4PE->getG4cpuPtr()->mpCluster[i/2]->PE_ts_peg_s[i%2]));
      }else{
	GUARD_PEnCLm[i]    = 0;
	PEGCAP_M_PEnCLm[i] = 0;
	PEGCAP_S_PEnCLm[i] = 0;
      }
    }
  }
}

/// <summary>delete function</summary>
void pltfrmGNDDelete()
{

#if defined(PLTFRM_TST)
  cout << "pltfrmGNDDelete() was called." << endl;
#endif //PLTFRM_TST

  delete fenmi_sig;

  for(int i=0; i<PE_MAX_NUM; i++){
    for ( int j = 0; j < FE_INTC1_CH_NUM; j++ ) {
      delete PE_feint_sig[i][j];
      delete PE_feint_type_sig[i][j];
    }
    for ( int j = 0; j < INTC1_CH_NUM; j++ ) {
      delete PE_eiint_sig[i][j];
      delete PE_eiint_type_sig[i][j];
    }
  }

  for (int i = 0; i < INTC2_CH_MAX_NUM; i++ ) {
    delete eiint_sig[i];
    delete eiint_type_sig[i];
  }

  delete reset_sig;
  
  delete GAPB_SLAVE_SG1;
  delete GAPB_SLAVE_SG2_0;
  delete GAPB_SLAVE_SG2_1;
  delete GAPB_SLAVE_SG3;
  delete GAPB_SLAVE_SG4;
  delete GAPB_SLAVE_SG5;
  delete GAPB_SLAVE_SG6;
  delete GAPB_SLAVE_SG7;
  delete GAPB_SLAVE_SG8;
  delete GAPB_SLAVE_SG9;
  delete AXI_SLAVE;
  delete AXI_DUMMY_MASTER;
  delete FLASH_DUMMY_MASTER;
  delete APB_DUMMY_MASTER_0;
  delete APB_DUMMY_MASTER_1;
  delete APB_DUMMY_MASTER_1;
  if( GUARD_BARR_DUMMY_MASTER ){
    delete GUARD_BARR_DUMMY_MASTER;
  }
  if( GUARD_IPIR_DUMMY_MASTER ){
    delete GUARD_IPIR_DUMMY_MASTER;
  }
  if( GUARD_MEV_DUMMY_MASTER ){
    delete GUARD_MEV_DUMMY_MASTER;
  }
  if( GUARD_TPTM_DUMMY_MASTER ){
    delete GUARD_TPTM_DUMMY_MASTER;
  }

  for (int i=0; i<INTC2_MAX_NUM; i++){
    if( GUARD_INTC2_DUMMY_MASTER[i] ){
      delete GUARD_INTC2_DUMMY_MASTER[i];
    }
  }

  for (int i=0; i<CL_MAX_NUM; i++){
    if(GUARD_CRAMCLm[i] != NULL)    delete GUARD_CRAMCLm[i];
    if(CRGCAP_CRAMHCLm[i] != NULL)  delete CRGCAP_CRAMHCLm[i];
    if(CRGCAP_CRAMLCLm[i] != NULL)  delete CRGCAP_CRAMLCLm[i];
    if(CRGCAP_SX2MBHCLm[i] != NULL) delete CRGCAP_SX2MBHCLm[i];
    if(CRGCAP_SX2MBLCLm[i] != NULL) delete CRGCAP_SX2MBLCLm[i];
  }
  for (int i=0; i<PE_MAX_NUM; i++){
    if(CRGCAP_PEnCLm[i] != NULL)   delete CRGCAP_PEnCLm[i];
    if(GUARD_PEnCLm[i] != NULL)    delete GUARD_PEnCLm[i];
    if(PEGCAP_M_PEnCLm[i] != NULL) delete PEGCAP_M_PEnCLm[i];
    if(PEGCAP_S_PEnCLm[i] != NULL) delete PEGCAP_S_PEnCLm[i];
  }
}

