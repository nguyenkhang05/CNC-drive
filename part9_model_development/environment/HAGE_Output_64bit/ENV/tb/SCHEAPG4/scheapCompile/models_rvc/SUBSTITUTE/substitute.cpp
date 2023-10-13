// -----------------------------------------------------------------------------
// $Id: substitute.cpp,v 1.1.1.1 2012/10/15 02:47:27 chaunguyen Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------
#include <string>
#include <cstdarg>
#include "re_define.h"
#include "substitute.h"

Csubstitute::Csubstitute(sc_module_name name):
         Csubstitute_regif((std::string)name, 32)
        ,vpcl::tlm_tgt_if<32>(name)
	,pclk("pclk")
	,preset("preset")
	,TPBATSST("TPBATSST")
	,TPBATTOUT("TPBATTOUT")
//       ,TPBATINTPRD ("TPBATINTPRD")
//       ,TPBATINTDTY ("TPBATINTDTY")
//       ,TPBATINTPAT ("TPBATINTPAT")
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
        //,adcsm0_adpe("adcsm0_adpe")
        // ,adcsm1_adpe("adcsm1_adpe")
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
//	 // SCI3 
//	 ,SCKI("SCKI")
//	 ,SCKO_1("SCKO_1")
//	 ,SCKO_2("SCKO_2")
//	 ,PCR("PCR")
	 ,PDR("PDR")
         ,TEI_1("TEI_1")
         ,TEI_2("TEI_2")
         ,ERI_1("ERI_1")
         ,ERI_2("ERI_2")
//	 // RLIN3
	 ,clkc("clkc")
	 ,rstc_n("rstc_n")
	 ,RX_CONTROL("RX_CONTROL")
	 ,RX_DATA("RX_DATA")
	 ,TX_CONTROL("TX_CONTROL")
	 ,TX_DATA("TX_DATA")
	  // SCDS
	,frefseq("frefseq")
	,onbd("onbd")
	,tf_wr("tf_wr")
	,tf_rd("tf_rd")
	,ocdid1("ocdid1")
	,ocdidin1("ocdidin1")
	,serialiden_n("serialiden_n")
	,seclock_faci("seclock_faci")
        // POWER
        // For selb_vcc block
       ,cvm2hdeten("cvm2hdeten")
       ,cvm2ldeten("cvm2ldeten")
       ,cvm2hdetlvl0("cvm2hdetlvl0")
       ,cvm2hdetlvl1("cvm2hdetlvl1")
       ,cvm2ldetlvl0("cvm2ldetlvl0")
       ,cvm2ldetlvl1("cvm2ldetlvl1")
       ,cvm_res_msk("cvm_res_msk")
       ,cvm_resreq_msk("cvm_resreq_msk")
       ,cvm_diag_mask("cvm_diag_mask")
       ,clunc_and_v50("clunc_and_v50")
       ,clunc_or_v50("clunc_or_v50")
       ,pdcvm2isoh_t("pdcvm2isoh_t") 
       ,pdcvm2isol_t("pdcvm2isol_t")
       ,pdcvm2awoh_t("pdcvm2awoh_t")
       ,pdcvm2awol_t("pdcvm2awol_t")
       ,cvm2hdet_v50("cvm2hdet_v50")
       ,cvm2ldet_v50("cvm2ldet_v50")
       ,reset_req_cvm_v50("reset_req_cvm_v50")
       ,dis_ldo_iso_v50("dis_ldo_iso_v50")
       ,cvm2isoldet_flg_v50("cvm2isoldet_flg_v50")
       ,cvm2awoldet_flg_v50("cvm2awoldet_flg_v50")
       ,cvm_reset_v50("cvm_reset_v50")
       ,ei_cvmout_v50("ei_cvmout_v50")
       // for cvm_awo block 
       ,diag_mode("diag_mode")
       ,cvmde_wen("cvmde_wen")
       ,cvmdmask1_wen("cvmdmask1_wen")
       ,cvmdmask2_wen("cvmdmask2_wen")
       ,clk("clk")
       ,clk_roscl("clk_roscl")
       ,moscen("moscen")
       ,pllen("pllen")
       ,cpuclks0("cpuclks0")
       ,cpuclks1("cpuclks1")
       ,cpuclks2("cpuclks2")
      // ,pocdet_n("pocdet_n")
       ,resz12("resz12")
       ,reset_fct("reset_fct")
       ,mstprqz("mstprqz")
       ,stop("stop")
       ,off("off")
       ,cyclic("cyclic")
       ,isolate("isolate")
       ,vccholdtrg("vccholdtrg")
       ,haltst("haltst")
       ,pdcvm2bgr_t("pdcvm2bgr_t")
       ,pdcvm2bufo_t("pdcvm2bufo_t")
       ,errcvm2isoh_t("errcvm2isoh_t")
       ,errcvm2isol_t("errcvm2isol_t")
       ,errcvm2awoh_t("errcvm2awoh_t")
       ,errcvm2awol_t("errcvm2awol_t")
       ,tapkeep("tapkeep")
       ,cvm2tmsk("cvm2tmsk")
       // for hold_cvm block
       ,cvmtrim0_in("cvmtrim0_in")
       ,cvmtrim1_in("cvmtrim1_in")
       ,cvmtrim2_in("cvmtrim2_in")
       ,cvmtrim3_in("cvmtrim3_in")
       ,cvmtrim4_in("cvmtrim4_in")
       ,en0("en0")
       ,en1("en1")
       ,en2("en2")
       ,en3("en3")
       ,en4("en4")
       // for POC
       ,CLK_V00("CLK_V00")
       ,CLR_POF("CLR_POF")
       ,poradet_n_v50("poradet_n_v50")
       ,pocadet_n_v50("pocadet_n_v50")
       ,pocdet_n_v50("pocdet_n_v50")
       ,det0det_v50("det0det_v50")
       ,REQ_POF("REQ_POF")
       ,DET0DET_MSK("DET0DET_MSK")
       ,POCADET_N_MSK("POCADET_N_MSK")
       ,POCDET_N_MSK("POCDET_N_MSK")

{//{{{ 

    SC_THREAD (RLIN3TransmistData);
    dont_initialize ();
    sensitive << mRLIN3TransmitDataEvent;

    preset.initialize(vpcl::emResetNonactive);

    //Initalze handleCommand  
    for(int i=0; i< 128; i++){
			std::ostringstream str_greg;
			str_greg << "greg"  << i;
			greg[i] = new sc_in<sc_uint<32>  >(str_greg.str().c_str());
		}
	
      
	tf_wr.initialize(sc_biguint<128>(-1));
	tf_rd.initialize(sc_biguint<128>(-1));

    AVREFH0.initialize(5.0);
    AVREFH1.initialize(5.0);

    CommandInit(this->name());    
    Csubstitute_regif::set_instance_name(this->name());

    SetLatency_TLM(0, true);
    printf ("[%s] [%s]  In constructor  \n",this->name(), sc_time_stamp().to_string().c_str());
   
  }//}}}  

//destructor of Csubstitute class
Csubstitute::~Csubstitute()
{//{{{
}//}}}


//Function
//tlm_tgt_if function

void Csubstitute::set_sarad_pointer(Csarad *sarad)
{//{{{
    if (sarad != NULL) {
        this->p_sarad = sarad;
    }
}//}}}
void Csubstitute::set_ecc_pointer(Cecc *ecc)
{//{{{
    if (ecc != NULL) {
        this->p_ecc = ecc;
    }
}//}}}
void Csubstitute::tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t)
 { //{{{
    // Get transaction attributes                                                                 
    tlm::tlm_command command;                                                                     
    sc_dt::uint64 addr = 0;                                                                       
    unsigned char *p_data = NULL;                                                                 
    unsigned int size = 0;                                                                        
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);          
    if (!status) {                                                                                
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);                           
        return;                                                                                   
    }                                                                                             
    sc_assert(p_data != NULL);

    printf ("[%s] [%s] tgt_acc : Addr = 0x%llX\n",this->name(),sc_time_stamp().to_string().c_str(),addr);
    
    // Callback method                                                                            
    if (command == tlm::TLM_READ_COMMAND) {                                                       
        status = reg_rd((unsigned int) addr, p_data, size);                                       
    }                                                                                             
    else if (command == tlm::TLM_WRITE_COMMAND) {                                                 
        status = reg_wr((unsigned int) addr, p_data, size);                                       
    }                                                                                             
    // Be necessarily TLM_IGNORE_COMMAND                                                          
    else {                                                                                        
        status = true;                                                                            
    }

    // Set response status attribute                                                              
    trans.set_response_status(status ? tlm::TLM_OK_RESPONSE : tlm::TLM_GENERIC_ERROR_RESPONSE);   
} // }}}

unsigned int Csubstitute::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = this->tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, true);
    sc_assert(data_ptr != NULL);

    // Callback method 
    if (command == tlm::TLM_READ_COMMAND) {
        status = reg_rd_dbg((unsigned int) address, data_ptr, data_length);
    }
    else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr_dbg((unsigned int) address, data_ptr, data_length);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
        data_length = 0;
    }

    // Set response status attribute and return value
    if (status) {
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return data_length;
    }
    else {
        trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        return 0;
    }
}//}}}


//Call_back function
void Csubstitute::cb_END_END(RegCBstr str)
{//{{{    
    if((unsigned int)(*END) == 0x0001){
         printf ("    **************************************************************\n");
         printf ("    *                                                            *\n");

         if ((*JUDGE) == 0x00) {
	     printf ("    *             The simulation has the PASS result.            *\n");
         }
         else if ((*JUDGE) == 0x10) {
	    printf ("    *             The simulation has the FAIL result.            *\n");
         }       

         printf ("    *                    End of the simulation                   *\n");
         printf ("    *                                                            *\n");
         printf ("    **************************************************************\n");
    }   
    
    
  }//}}}

void Csubstitute::cb_CTRL_CTRL0(RegCBstr str)
{//{{{ 
    if ((*CTRL)["CTRL1"] == 0x1) {
         printf ("    **************************************************************\n");
         (*CTRL)["CTRL1"] = 0x0;
         mRLIN3TransmitDataEvent.notify (); 
    }
    else  if ((*CTRL)["CTRL2"] == 0x1) {
         std::string ret ;
         std::vector<std::string> cmd;
         printf("in ctrl2 \n");
         char err_det_set_str[255];
         unsigned int err_det_set = 2;
         sprintf(err_det_set_str, "%d",err_det_set);
         cmd.push_back("EccError");
         cmd.push_back(err_det_set_str);
//         DumpCommandInfo(cmd);
         ret = p_ecc->handleCommand(cmd);
         cout << ret ;        

    }
    else  if ((*CTRL)["CTRL3"] == 0x1) {
         std::string ret ;
         std::vector<std::string> cmd;
        printf ("in ctrl3 \n");
         char err_det_set_str[255];
         unsigned int err_det_set = 1;
         sprintf(err_det_set_str, "%d",err_det_set);
         cmd.push_back("EccError");
         cmd.push_back(err_det_set_str);
//         DumpCommandInfo(cmd);
         ret = p_ecc->handleCommand(cmd);
         cout << ret ;        
    }

    else if ((*CTRL)["CTRL0"] == 0x1) {
         printf ("    **************************************************************\n");
         printf ("    *    The HW reset is asserted at %s                           \n",sc_time_stamp().to_string().c_str());
         printf ("    **************************************************************\n");
         preset.write (vpcl::emResetActive); 
       //  rstc_n.write (vpcl::emResetActive);
    } else if ((*CTRL)["CTRL0"] == 0x0) {
         printf ("    **************************************************************\n");
         printf ("    *    The HW reset is deasserted at %s                         \n",sc_time_stamp().to_string().c_str());
         printf ("    **************************************************************\n");
         preset.write (vpcl::emResetNonactive); 
       //  rstc_n.write (vpcl::emResetNonactive);
    
        }
    
 
}//}}} 

void Csubstitute::cb_JUDGE_JUDGE(RegCBstr str)
{//{{{
    // Pass result
    if ((unsigned int)(*JUDGE) != 0x00000000 && (unsigned int)(*JUDGE) != 0x00000010) { 
         unsigned int interrupt_channel = (unsigned int)(*JUDGE);
         printf ("    *   [%s] [%s]  [INT] PC moves to the vector code at 0x%X      \n",this->name(), sc_time_stamp().to_string().c_str(),interrupt_channel);
    } 
}//}}}

void Csubstitute::SetLatency_TLM(const double pclk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}

void Csubstitute::RLIN3TransmistData()
{//{{{
    while (1) {

        printf ("    [%s] [%s] RLIN3_CONTROL = 0x000A0148\n",this->name(),sc_time_stamp().to_string().c_str());
        printf ("    [%s] [%s] RLIN3_DATA    = 0x0000001F\n",this->name(),sc_time_stamp().to_string().c_str());
                                                                                
        RX_CONTROL.write (0x000A0148);
        RX_DATA   .write (0x0000001F);

        wait (10800,SC_NS);

        printf ("    [%s] [%s] RLIN3_CONTROL = 0x000A0188\n",this->name(),sc_time_stamp().to_string().c_str());
        printf ("    [%s] [%s] RLIN3_DATA    = 0xFFFFFFFF\n",this->name(),sc_time_stamp().to_string().c_str());
                                                                                
        RX_CONTROL.write (0x000A0188);
        RX_DATA   .write (0xFFFFFFFF);

        wait (1200,SC_NS);

        printf ("    [%s] [%s] RLIN3_CONTROL = 0x000A0108\n",this->name(),sc_time_stamp().to_string().c_str());
        printf ("    [%s] [%s] RLIN3_DATA    = 0xFFFFFFFF\n",this->name(),sc_time_stamp().to_string().c_str());
                                                                                
        RX_CONTROL.write (0x000A0108);
        RX_DATA   .write (0xFFFFFFFF);
        printf ("    **************************************************************\n");
                                                                                
        wait ();
    }
}//}}}

// vim600: set foldmethod=marker :
