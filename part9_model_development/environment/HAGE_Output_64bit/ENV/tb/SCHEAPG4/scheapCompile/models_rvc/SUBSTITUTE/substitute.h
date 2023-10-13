
#ifndef __SUBSTITUTE_H__
#define __SUBSTITUTE_H__
#include "systemc.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include "iterator"
#include "substitute_regif.h"
#include "ecc.h"
#include "adc.h"
#include "sarad.h"
/// SUBSTITUTE model class
class Csubstitute: public sc_module
             ,public Csubstitute_regif
             ,public vpcl::tlm_tgt_if<32>
{

#include "substitute_cmdif.h"

public:    
      // common port
   sc_out<sc_dt::uint64> pclk;	
   sc_out<bool> preset;	
   // TPBA 
   sc_out<bool> TPBATSST;
   sc_in<bool>  TPBATTOUT;

  // sc_in<bool> TPBATINTPRD;
  // sc_in<bool> TPBATINTDTY;
  // sc_in<bool> TPBATINTPAT;

    // OTS model
   sc_out<sc_dt::uint64>clkot;
   sc_in <bool> ote;
     
   // SARAD model
    sc_out<sc_dt::uint64> ADCLK;    
    sc_out<double> AVREFH0;
    sc_out<double> AVREFH1;

    sc_out<bool> AD0SG0TRG;
    sc_out<bool> AD0SG1TRG;
    sc_out<bool> AD0SG2TRG;
    sc_out<bool> AD0SG3TRG;
    sc_out<bool> AD0SG4TRG;
    sc_out<bool> AD1SG0TRG;
    sc_out<bool> AD1SG1TRG;
    sc_out<bool> AD1SG2TRG;
    sc_out<bool> AD1SG3TRG;
    sc_out<bool> AD1SG4TRG;

    sc_out< double > AN000; 
    sc_out< double > AN001;
    sc_out< double > AN002;
    sc_out< double > AN003;
    sc_out< double > AN010;
    sc_out< double > AN011;
    sc_out< double > AN012;
    sc_out< double > AN013;
    sc_out< double > AN020;
    sc_out< double > AN021;
    sc_out< double > AN022;
    sc_out< double > AN023;
    sc_out< double > AN030;
    sc_out< double > AN031;
    sc_out< double > AN032;
    sc_out< double > AN033;
    sc_out< double > AN040;
    sc_out< double > AN041;
    sc_out< double > AN042;
    sc_out< double > AN043;
    sc_out< double > AN050;
    sc_out< double > AN051;
    sc_out< double > AN052;
    sc_out< double > AN053;
    sc_out< double > AN060;
    sc_out< double > AN061;
    sc_out< double > AN062;
    sc_out< double > AN063;

    sc_out< double > AN100; 
    sc_out< double > AN101;
    sc_out< double > AN102;
    sc_out< double > AN103;
    sc_out< double > AN110;
    sc_out< double > AN111;
    sc_out< double > AN112;
    sc_out< double > AN113;
    sc_out< double > AN120;
    sc_out< double > AN121;
    sc_out< double > AN122;
    sc_out< double > AN123;
    sc_out< double > AN130;
    sc_out< double > AN131;
    sc_out< double > AN132;
    sc_out< double > AN133;
    sc_out< double > AN140;
    sc_out< double > AN141;
    sc_out< double > AN142;
    sc_out< double > AN143;
    sc_out< double > AN150;
    sc_out< double > AN151;
    sc_out< double > AN152;
    sc_out< double > AN153;
    sc_out< double > AN160;
    sc_out< double > AN161;
    sc_out< double > AN162;
    sc_out< double > AN163;
    sc_out< double > AN170;
    sc_out< double > AN171;
    sc_out< double > AN172;
    sc_out< double > AN173;
    
    sc_in<bool> DFREQ0;
    sc_in<bool> DFREQ1;
    sc_in< sc_uint<4> > DFTAG0;
    sc_in< sc_uint<4> > DFTAG1;
    sc_in< sc_uint<16> > DFDATA0;
    sc_in< sc_uint<16> > DFDATA1;
    
    sc_in<bool> ADEND00; 
    sc_in<bool> ADEND01;
    sc_in<bool> ADEND02;
    sc_in<bool> ADEND03;
    sc_in<bool> ADEND04;
    sc_in<bool> ADEND10;
    sc_in<bool> ADEND11;
    sc_in<bool> ADEND12;
    sc_in<bool> ADEND13;
    sc_in<bool> ADEND14;
    // ECM
    sc_out<bool> wdta0_WDTATRES;
    sc_out<bool> wdta1_WDTATRES;
    sc_out<bool> PE1_Lram_dederrout;
    sc_out<bool> PE1_Lram_sederrout;
    sc_out<bool> PE3_Lram_dederrout;
    sc_out<bool> PE3_Lram_sederrout;
    sc_out<bool> PE1_GRAM_dederrout;
    sc_out<bool> PE1_GRAM_sederrout;
    sc_out<bool> PE3_GRAM_dederrout;
    sc_out<bool> PE3_GRAM_sederrout;
    sc_out<bool> GRAMC_GRAM_dederrout;
    sc_out<bool> GRAMC_GRAM_sederrout;
    sc_out<bool> apa0_ec7tie2;
    sc_out<bool> apa0_ec7tie1;
    sc_out<bool> apa1_ec7tie2;
    sc_out<bool> apa1_ec7tie1;
    sc_out<bool> apa2_ec7tie2;
    sc_out<bool> apa2_ec7tie1;
    sc_out<bool> dfe1_ec7tie2;
    sc_out<bool> dfe1_ec7tie1;
    sc_out<bool> dfe20_ec6tie2;
    sc_out<bool> dfe20_ec6tie1;
    sc_out<bool> dfe21_ec6tie2;
    sc_out<bool> dfe21_ec6tie1;
    sc_out<bool> can_ec7tie2;
    sc_out<bool> can_ec7tie1;
    sc_out<bool> csih0_ec7tie2;
    sc_out<bool> csih0_ec7tie1;
    sc_out<bool> csih1_ec7tie2;
    sc_out<bool> csih1_ec7tie1;
    sc_out<bool> csih2_ec7tie2;
    sc_out<bool> csih2_ec7tie1;
    sc_out<bool> csih3_ec7tie2;
    sc_out<bool> csih3_ec7tie1;
    sc_out<bool> fxr_ec7tie2;
    sc_out<bool> fxr_ec7tie1;
    sc_out<bool> DTSRAMDED;
    sc_out<bool> DTSRAMSEC;
    sc_out<bool> PE1_fr_dederrout;
    sc_out<bool> PE1_fr_sederrout;
    sc_out<bool> PE3_fr_dederrout;
    sc_out<bool> PE3_fr_sederrout;
    sc_out<bool> AXI2CFB_fr_dederrout;
    sc_out<bool> AXI2CFB_fr_sederrout;
    sc_out<bool> PE1_ICDRAM_dederrout;
    sc_out<bool> PE1_ICDRAM_sederrout;
    sc_out<bool> PE3_ICDRAM_dederrout;
    sc_out<bool> PE3_ICDRAM_sederrout;
    sc_out<bool> PE1_ICTRAM_dederrout;
    sc_out<bool> PE1_ICTRAM_sederrout;
    sc_out<bool> PE3_ICTRAM_dederrout;
    sc_out<bool> PE3_ICTRAM_sederrout;
    sc_out<bool> DCIB_dederrout;
    sc_out<bool> DCIB_sederrout;
    sc_out<bool> CLMA2TRESZ;
    sc_out<bool> CLMA3TRESZ;
    sc_out<bool> CLMA4TRESZ;
    sc_out<bool> CLMA0TRESZ;
    sc_out<bool> CLMA1TRESZ;
    // sc_out<bool>             otabe;
    sc_out<bool> dsadcsm0_dsadpe;
    sc_out<bool> dsadcsm1_dsadpe;
    sc_out<bool> dsadcsm2_dsadpe;
    sc_out<bool> dsadcsm3_dsadpe;
    sc_out<bool> dsadcsm4_dsadpe;
    sc_out<bool> dsadcsm5_dsadpe;
    //sc_out<bool>             adcsm0_adpe;
   // sc_out<bool>             adcsm1_adpe;
    sc_out<bool> intflerr;
    sc_out<bool> intfrterr;
    
    sc_out<bool> ecmterrlbz_m;
    sc_out<bool> ecmterrlbz_c;
    sc_out<bool> ecmttin;
    sc_out<bool> extresz;
    sc_out<bool> svaccess;
    sc_in<bool> ecmterroz;
    sc_in<bool> ecmterroutz_m;
    sc_in<bool> ecmterroutz_c;
    sc_in<bool> ecmtresz;
 //   
 //   // SCI3
//   sc_in<sc_dt::uint64>   SCKO_1;
//   sc_in<sc_dt::uint64>   SCKO_2;
//
   sc_in<bool>            TEI_1;
   sc_in<bool>            ERI_1;
   sc_in<bool>            TEI_2;
   sc_in<bool>            ERI_2;
 //
//   sc_out<sc_dt::uint64>  SCKI;
//   sc_out<bool> 	   PCR;
   sc_out<bool> 	   PDR;

 //   // RLIN3   
 //   
    sc_out<sc_dt::uint64> clkc;
    sc_out<bool>          rstc_n;
    sc_out<sc_uint<32> >  RX_CONTROL;
    sc_out<sc_uint<32> >  RX_DATA;    
    sc_in<sc_uint<32> >  TX_CONTROL;
    sc_in<sc_uint<32> >  TX_DATA;
    
    // SCDS
    sc_out<bool>    frefseq;
    sc_out<bool>    onbd;
    sc_out<sc_biguint<128> > tf_wr;
    sc_out<sc_biguint<128> > tf_rd;
    sc_out<sc_biguint<128> > ocdid1;
    sc_out<sc_biguint<128> > ocdidin1;
    sc_out<bool>    serialiden_n;
    sc_in<bool>     seclock_faci;

   sc_in<sc_uint<32> > *greg[128];

   // POWER
  // For selb_vcc block  
  sc_out<bool> cvm2hdeten;
  sc_out<bool> cvm2ldeten;
  sc_out<bool> cvm2hdetlvl0;
  sc_out<bool> cvm2hdetlvl1;
  sc_out<bool> cvm2ldetlvl0;
  sc_out<bool> cvm2ldetlvl1;
  sc_out<bool> cvm_res_msk;
  sc_out<bool> cvm_resreq_msk;
  sc_out<bool> cvm_diag_mask;
  sc_out<bool> clunc_and_v50;
  sc_out<bool> clunc_or_v50;
  sc_out<bool> pdcvm2isoh_t;
  sc_out<bool> pdcvm2isol_t;
  sc_out<bool> pdcvm2awoh_t;
  sc_out<bool> pdcvm2awol_t;
  sc_in<bool> cvm2hdet_v50;
  sc_in<bool> cvm2ldet_v50;
  sc_in<bool> reset_req_cvm_v50;
  sc_in<bool> dis_ldo_iso_v50;
  sc_in<bool> cvm2isoldet_flg_v50;
  sc_in<bool> cvm2awoldet_flg_v50;
  sc_in<bool> cvm_reset_v50;
  sc_in<bool> ei_cvmout_v50;
  // For cvm_awo block 
  sc_out<bool> diag_mode;
  sc_out<bool> cvmde_wen;   
  sc_out<bool> cvmdmask1_wen;
  sc_out<bool> cvmdmask2_wen;
  sc_out<sc_dt::uint64> clk;
  sc_out<sc_dt::uint64> clk_roscl;
  sc_out<bool> moscen;
  sc_out<bool> pllen;
  sc_out<bool> cpuclks0;
  sc_out<bool> cpuclks1;
  sc_out<bool> cpuclks2;
 // sc_out<bool> pocdet_n;
  sc_out<bool> resz12;
  sc_out<bool> reset_fct;
  sc_out<bool> mstprqz;
  sc_out<bool> stop;
  sc_out<bool> off;
  sc_out<bool> cyclic;
  sc_out<bool> isolate;
  sc_out<bool> vccholdtrg;
  sc_out<bool> haltst;
  sc_out<bool> pdcvm2bgr_t;
  sc_out<bool> pdcvm2bufo_t;
  sc_out<bool> errcvm2isoh_t;
  sc_out<bool> errcvm2isol_t;
  sc_out<bool> errcvm2awoh_t;
  sc_out<bool> errcvm2awol_t;
  sc_out<bool> tapkeep;
  sc_out<bool> cvm2tmsk;

  // For hold_cvm block 
  sc_out<bool> cvmtrim0_in;
  sc_out<bool> cvmtrim1_in;
  sc_out<bool> cvmtrim2_in;
  sc_out<bool> cvmtrim3_in;
  sc_out<bool> cvmtrim4_in;
  sc_out<bool> en0;
  sc_out<bool> en1;
  sc_out<bool> en2;
  sc_out<bool> en3;
  sc_out<bool> en4;
  // For POC
  sc_out<sc_dt::uint64> CLK_V00;
  sc_out<bool> CLR_POF;
  sc_in<bool> poradet_n_v50;
  sc_in<bool> pocadet_n_v50;
  sc_in<bool> pocdet_n_v50;
  sc_in<bool> det0det_v50;
  sc_in<bool> REQ_POF;
  sc_in<bool> DET0DET_MSK;
  sc_in<bool> POCADET_N_MSK;
  sc_in<bool> POCDET_N_MSK;

 SC_HAS_PROCESS(Csubstitute);
 Csubstitute(sc_module_name name);  
 // ECC 
    void set_ecc_pointer(Cecc *ecc);
    void set_sarad_pointer(Csarad *sarad);
 ~Csubstitute();
   //Port declaration    
private:
    //Enum
    
    //Variable 
    unsigned int mState;
    Cecc * p_ecc;
    Csarad *p_sarad;
    //Event
    sc_event mRLIN3TransmitDataEvent;

    //Connect port

    //tlm function
    void tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int  tgt_acc_dbg(tlm::tlm_generic_payload &trans);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
 
    void RLIN3TransmistData ();
    //call_back function
    void cb_CTRL_CTRL0(RegCBstr str);
    void cb_JUDGE_JUDGE(RegCBstr str);
    void cb_END_END(RegCBstr str);

    //Normal function

    //handle_command function

    //method

};

#endif //__SUBSTITUTE_H__
