/*
* Copyright(c) 2012 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*/

#ifndef DUMMY_RVC_H
#define DUMMY_RVC_H

#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0
#include "global.h"
#include "tlm.h"

template<unsigned int BUSWIDTH>
class SubstituteModel : public sc_module, public virtual TlmBwTransportIf
{
  public:
    // bus socket : TlmInitiatorSocket
    // TlmInitiatorSocket<BUSWIDTH> is;
    // common port
   sc_out<sc_dt::uint64> pclk;	
   sc_out<bool> preset;	
   // TPBA 
   sc_out<bool> TPBATSST;
   sc_in<bool>  TPBATTOUT;
    // OTS model
   sc_out<sc_dt::uint64>clkot;
     
   // SARAD model
    sc_out<sc_dt::uint64> ADCLK;    
    sc_in< sc_uint<32> > AVREFH0;
    sc_in< sc_uint<32> > AVREFH1;

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
    sc_out<bool>             wdta0_WDTATRES;
    sc_out<bool>             wdta1_WDTATRES;
    sc_out<bool>             PE1_Lram_dederrout;
    sc_out<bool>             PE1_Lram_sederrout;
    sc_out<bool>             PE3_Lram_dederrout;
    sc_out<bool>             PE3_Lram_sederrout;
    sc_out<bool>             PE1_GRAM_dederrout;
    sc_out<bool>             PE1_GRAM_sederrout;
    sc_out<bool>             PE3_GRAM_dederrout;
    sc_out<bool>             PE3_GRAM_sederrout;
    sc_out<bool>             GRAMC_GRAM_dederrout;
    sc_out<bool>             GRAMC_GRAM_sederrout;
    sc_out<bool>             apa0_ec7tie2;
    sc_out<bool>             apa0_ec7tie1;
    sc_out<bool>             apa1_ec7tie2;
    sc_out<bool>             apa1_ec7tie1;
    sc_out<bool>             apa2_ec7tie2;
    sc_out<bool>             apa2_ec7tie1;
    sc_out<bool>             dfe1_ec7tie2;
    sc_out<bool>             dfe1_ec7tie1;
    sc_out<bool>             dfe20_ec6tie2;
    sc_out<bool>             dfe20_ec6tie1;
    sc_out<bool>             dfe21_ec6tie2;
    sc_out<bool>             dfe21_ec6tie1;
    sc_out<bool>             can_ec7tie2;
    sc_out<bool>             can_ec7tie1;
    sc_out<bool>             csih0_ec7tie2;
    sc_out<bool>             csih0_ec7tie1;
    sc_out<bool>             csih1_ec7tie2;
    sc_out<bool>             csih1_ec7tie1;
    sc_out<bool>             csih2_ec7tie2;
    sc_out<bool>             csih2_ec7tie1;
    sc_out<bool>             csih3_ec7tie2;
    sc_out<bool>             csih3_ec7tie1;
    sc_out<bool>             fxr_ec7tie2;
    sc_out<bool>             fxr_ec7tie1;
    sc_out<bool>             DTSRAMDED;
    sc_out<bool>             DTSRAMSEC;
    sc_out<bool>             PE1_fr_dederrout;
    sc_out<bool>             PE1_fr_sederrout;
    sc_out<bool>             PE3_fr_dederrout;
    sc_out<bool>             PE3_fr_sederrout;
    sc_out<bool>             AXI2CFB_fr_dederrout;
    sc_out<bool>             AXI2CFB_fr_sederrout;
    sc_out<bool>             PE1_ICDRAM_dederrout;
    sc_out<bool>             PE1_ICDRAM_sederrout;
    sc_out<bool>             PE3_ICDRAM_dederrout;
    sc_out<bool>             PE3_ICDRAM_sederrout;
    sc_out<bool>             PE1_ICTRAM_dederrout;
    sc_out<bool>             PE1_ICTRAM_sederrout;
    sc_out<bool>             PE3_ICTRAM_dederrout;
    sc_out<bool>             PE3_ICTRAM_sederrout;
    sc_out<bool>             DCIB_dederrout;
    sc_out<bool>             DCIB_sederrout;
    sc_out<bool>             CLMA2TRESZ;
    sc_out<bool>             CLMA3TRESZ;
    sc_out<bool>             CLMA4TRESZ;
    sc_out<bool>             CLMA0TRESZ;
    sc_out<bool>             CLMA1TRESZ;
    // sc_out<bool>             otabe;
    sc_out<bool>             dsadcsm0_dsadpe;
    sc_out<bool>             dsadcsm1_dsadpe;
    sc_out<bool>             dsadcsm2_dsadpe;
    sc_out<bool>             dsadcsm3_dsadpe;
    sc_out<bool>             dsadcsm4_dsadpe;
    sc_out<bool>             dsadcsm5_dsadpe;
    sc_out<bool>             adcsm0_adpe;
    sc_out<bool>             adcsm1_adpe;
    sc_out<bool>             intflerr;
    sc_out<bool>             intfrterr;
    
    sc_out<bool>             ecmterrlbz_m;
    sc_out<bool>             ecmterrlbz_c;
    sc_out<bool>             ecmttin;
    sc_out<bool>             extresz;
    sc_out<bool>              svaccess;
    sc_in<bool>            ecmterroz;
    sc_in<bool>            ecmterroutz_m;
    sc_in<bool>            ecmterroutz_c;
    sc_in<bool>            ecmtresz;
    // SCI3
    sc_out<sc_dt::uint64>  SCKI;
    sc_in<sc_dt::uint64>  SCKO_1;
    sc_in<sc_dt::uint64>  SCKO_2;
    sc_out<bool> 	   PCR;
    sc_out<bool> 	   PDR;
    // RLIN3   
    
    sc_out<sc_dt::uint64> clkc;
    sc_out<bool>          rstc_n;
    sc_out<sc_uint<32> >  RX_CONTROL;
    sc_out<sc_uint<32> >  RX_DATA;    
    sc_in<sc_uint<32> >  TX_CONTROL;
    sc_in<sc_uint<32> >  TX_DATA;
    
    // SCDS
    // sc_out<bool>    frefseq;
    // sc_out<bool>    onbd;
    // sc_out<sc_biguint<128> > tf_wr;
    // sc_out<sc_biguint<128> > tf_rd;
    // sc_out<sc_biguint<128> > ocdid1;
    // sc_out<sc_biguint<128> > ocdidin1;
    // sc_out<bool>    serialiden_n;
    // sc_in<bool>     seclock_faci;

   // sc_in<sc_uint<32> > *greg[128];
   
    
   

    
    // constructor
    SubstituteModel( sc_module_name module_name="Substitute Model" ):
	pclk("pclk")
	,preset("preset")
	,TPBATSST("TPBATSST")
	,TPBATTOUT("TPBATTOUT")
	,clkot("clkot")	
	,ADCLK("ADCLK")        
        ,AVREFH0("AVREFH0")
        ,AVREFH1("AVREFH1")
        //trigger input port
        ,AD0SG0TRG("AD0SG0TRG")  
        ,AD0SG1TRG("AD0SG1TRG")  
        ,AD0SG2TRG("AD0SG2TRG")  
        ,AD0SG3TRG("AD0SG3TRG")  
        ,AD0SG4TRG("AD0SG4TRG")  
        ,AD1SG0TRG("AD1SG0TRG")  
        ,AD1SG1TRG("AD1SG1TRG")  
        ,AD1SG2TRG("AD1SG2TRG")  
        ,AD1SG3TRG("AD1SG3TRG")  
        ,AD1SG4TRG("AD1SG4TRG")
        //Analog terminal input port
        ,AN000("AN000")
        ,AN001("AN001")
        ,AN002("AN002")
        ,AN003("AN003")
        ,AN010("AN010")
        ,AN011("AN011")
        ,AN012("AN012")
        ,AN013("AN013")
        ,AN020("AN020")
        ,AN021("AN021")
        ,AN022("AN022")
        ,AN023("AN023")
        ,AN030("AN030")
        ,AN031("AN031")
        ,AN032("AN032")
        ,AN033("AN033")
        ,AN040("AN040")
        ,AN041("AN041")
        ,AN042("AN042")
        ,AN043("AN043")
        ,AN050("AN050")
        ,AN051("AN051")
        ,AN052("AN052")
        ,AN053("AN053")
        ,AN060("AN060")
        ,AN061("AN061")
        ,AN062("AN062")
        ,AN063("AN063")

        ,AN100("AN100")
        ,AN101("AN101")
        ,AN102("AN102")
        ,AN103("AN103")
        ,AN110("AN110")
        ,AN111("AN111")
        ,AN112("AN112")
        ,AN113("AN113")
        ,AN120("AN120")
        ,AN121("AN121")
        ,AN122("AN122")
        ,AN123("AN123")
        ,AN130("AN130")
        ,AN131("AN131")
        ,AN132("AN132")
        ,AN133("AN133")
        ,AN140("AN140")
        ,AN141("AN141")
        ,AN142("AN142")
        ,AN143("AN143")
        ,AN150("AN150")
        ,AN151("AN151")
        ,AN152("AN152")
        ,AN153("AN153")
        ,AN160("AN160")
        ,AN161("AN161")
        ,AN162("AN162")
        ,AN163("AN163")
        ,AN170("AN170")
        ,AN171("AN171")
        ,AN172("AN172")
        ,AN173("AN173")
        //Data output port
        ,DFREQ0("DFREQ0")
        ,DFREQ1("DFREQ1")
        ,DFTAG0("DFTAG0")
        ,DFTAG1("DFTAG1")
        ,DFDATA0("DFDATA0")
        ,DFDATA1("DFDATA1")
        ,ADEND00("ADEND00")  
        ,ADEND01("ADEND01")
        ,ADEND02("ADEND02")
        ,ADEND03("ADEND03")
        ,ADEND04("ADEND04")
        ,ADEND10("ADEND10")
        ,ADEND11("ADEND11")
        ,ADEND12("ADEND12")
        ,ADEND13("ADEND13")
        ,ADEND14("ADEND14")
	// ecm
	,wdta0_WDTATRES("wdta0_WDTATRES")
        ,wdta1_WDTATRES("wdta1_WDTATRES")
        ,PE1_Lram_dederrout("PE1_Lram_dederrout")
        ,PE1_Lram_sederrout("PE1_Lram_sederrout")
        ,PE3_Lram_dederrout("PE3_Lram_dederrout")
        ,PE3_Lram_sederrout("PE3_Lram_sederrout")
         ,PE1_GRAM_dederrout("PE1_GRAM_dederrout")
         ,PE1_GRAM_sederrout("PE1_GRAM_sederrout")
         ,PE3_GRAM_dederrout("PE3_GRAM_dederrout")
         ,PE3_GRAM_sederrout("PE3_GRAM_sederrout")
         ,GRAMC_GRAM_dederrout("GRAMC_GRAM_dederrout")
         ,GRAMC_GRAM_sederrout("GRAMC_GRAM_sederrout")
         ,apa0_ec7tie2("apa0_ec7tie2")
         ,apa0_ec7tie1("apa0_ec7tie1")
         ,apa1_ec7tie2("apa1_ec7tie2")
         ,apa1_ec7tie1("apa1_ec7tie1")
         ,apa2_ec7tie2("apa2_ec7tie2")
         ,apa2_ec7tie1("apa2_ec7tie1")
         ,dfe1_ec7tie2("dfe1_ec7tie2")
         ,dfe1_ec7tie1("dfe1_ec7tie1")
         ,dfe20_ec6tie2("dfe20_ec6tie2")
         ,dfe20_ec6tie1("dfe20_ec6tie1")
         ,dfe21_ec6tie2("dfe21_ec6tie2")
         ,dfe21_ec6tie1("dfe21_ec6tie1")
         ,can_ec7tie2("can_ec7tie2")
         ,can_ec7tie1("can_ec7tie1")
         ,csih0_ec7tie2("csih0_ec7tie2")
         ,csih0_ec7tie1("csih0_ec7tie1")
         ,csih1_ec7tie2("csih1_ec7tie2")
         ,csih1_ec7tie1("csih1_ec7tie1")
         ,csih2_ec7tie2("csih2_ec7tie2")
         ,csih2_ec7tie1("csih2_ec7tie1")
         ,csih3_ec7tie2("csih3_ec7tie2")
         ,csih3_ec7tie1("csih3_ec7tie1")
         ,fxr_ec7tie2("fxr_ec7tie2")
         ,fxr_ec7tie1("fxr_ec7tie1")
         ,DTSRAMDED("DTSRAMDED")
         ,DTSRAMSEC("DTSRAMSEC")
         ,PE1_fr_dederrout("PE1_fr_dederrout")
         ,PE1_fr_sederrout("PE1_fr_sederrout")
         ,PE3_fr_dederrout("PE3_fr_dederrout")
         ,PE3_fr_sederrout("PE3_fr_sederrout")
         ,AXI2CFB_fr_dederrout("AXI2CFB_fr_dederrout")
         ,AXI2CFB_fr_sederrout("AXI2CFB_fr_sederrout")
         ,PE1_ICDRAM_dederrout("PE1_ICDRAM_dederrout")
         ,PE1_ICDRAM_sederrout("PE1_ICDRAM_sederrout")
         ,PE3_ICDRAM_dederrout("PE3_ICDRAM_dederrout")
         ,PE3_ICDRAM_sederrout("PE3_ICDRAM_sederrout")
         ,PE1_ICTRAM_dederrout("PE1_ICTRAM_dederrout")
         ,PE1_ICTRAM_sederrout("PE1_ICTRAM_sederrout")
         ,PE3_ICTRAM_dederrout("PE3_ICTRAM_dederrout")
         ,PE3_ICTRAM_sederrout("PE3_ICTRAM_sederrout")
         ,DCIB_dederrout("DCIB_dederrout")
         ,DCIB_sederrout("DCIB_sederrout")
         ,CLMA2TRESZ("CLMA2TRESZ")
         ,CLMA3TRESZ("CLMA3TRESZ")
         ,CLMA4TRESZ("CLMA4TRESZ")
         ,CLMA0TRESZ("CLMA0TRESZ")
         ,CLMA1TRESZ("CLMA1TRESZ")
         
         ,dsadcsm0_dsadpe("dsadcsm0_dsadpe")
         ,dsadcsm1_dsadpe("dsadcsm1_dsadpe")
         ,dsadcsm2_dsadpe("dsadcsm2_dsadpe")
         ,dsadcsm3_dsadpe("dsadcsm3_dsadpe")
         ,dsadcsm4_dsadpe("dsadcsm4_dsadpe")
         ,dsadcsm5_dsadpe("dsadcsm5_dsadpe")
         ,adcsm0_adpe("adcsm0_adpe")
         ,adcsm1_adpe("adcsm1_adpe")
         ,intflerr("intflerr")
         ,intfrterr("intfrterr")
         ,ecmterrlbz_m("ecmterrlbz_m")
         ,ecmterrlbz_c("ecmterrlbz_c")
         ,ecmttin("ecmttin")
         ,extresz("extresz")
         ,svaccess("svaccess")
         ,ecmterroz("ecmterroz")
         ,ecmterroutz_m("ecmterroutz_m")
         ,ecmterroutz_c("ecmterroutz_c")
         ,ecmtresz("ecmtresz")
	 ,SCKI("SCKI")
	 ,SCKO_1("SCKO_1")
	 ,SCKO_2("SCKO_2")
	 ,PCR("PCR")
	 ,PDR("PDR")
	 // RLIN3
	 ,clkc("clkc")
	 ,rstc_n("rstc_n")
	 ,RX_CONTROL("RX_CONTROL")
	 ,RX_DATA("RX_DATA")
	 ,TX_CONTROL("TX_CONTROL")
	 ,TX_DATA("TX_DATA")
	 // SCDS
	 // ,frefseq("frefseq")
	// ,onbd("onbd")
	// ,tf_wr("tf_wr")
	// ,tf_rd("tf_rd")
	// ,ocdid1("ocdid1")
	// ,ocdidin1("ocdidin1")
	// ,serialiden_n("serialiden_n")
	// ,seclock_faci("seclock_faci")
	

	{ //is(*this); 
		// for(int i=0; i< 128; i++){
			// std::ostringstream str_greg;
			// str_greg << "greg"  << i;
			// greg[i] = new sc_in<double >(str_greg.str().c_str());
		// }
		
	}

    // destructor
    ~SubstituteModel()
    {}

    void invalidate_direct_mem_ptr( sc_dt::uint64 startRange,
				    sc_dt::uint64 endRange )
    {}

    TlmSyncEnum nb_transport_bw(TlmTransactionType& trans, 
				TlmPhase&           phase, 
				sc_core::sc_time&   t)
    { return tlm::TLM_UPDATED; }

};


#endif /* DUMMY_RVC_H */
