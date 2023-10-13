// ----------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2017-2020 Renesas System Design Co., Ltd.
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#include "ecm_u2a.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//===============Cecm_u2a class=======================
//Constructor of Cecm_u2a class
Cecm_u2a::Cecm_u2a(sc_module_name name, Cecm_u2a_wp* wrapper):  
     Cecm_u2a_regif((std::string)name,32)
    ,Cgeneral_timer(name)
    ,erroutresz("erroutresz")
    ,sgaterrlbz("sgaterrlbz")
    ,sgattin("sgattin")
    ,dtmtrgin("dtmtrgin")
    ,resstg1z("resstg1z")
    ,resstg1z_pclkin("resstg1z_pclkin")
    ,errin_mask_reset("errin_mask_reset")
    ,sgaterroz("sgaterroz")
    ,sgaterroutz("sgaterroutz")
    ,sgatresz("sgatresz")
    ,dtmtrgout("dtmtrgout")
    ,pseudo_compare_msk_m("pseudo_compare_msk_m")
    ,pseudo_compare_msk_c("pseudo_compare_msk_c")
{//{{{
    CommandInit(this->name());
    mWrapper = wrapper;
    // Name for ports/signals
    for(unsigned int index = 0; index < emNumErrorFactor; index++){
        std::ostringstream str_sgaterrin;
        std::ostringstream str_sgatpe;

        str_sgaterrin<<"sgaterrin"<<index;
        str_sgatpe<<"sgatpe"<<index;

        sgaterrin[index] = new sc_in<bool>(str_sgaterrin.str().c_str());
        sgatpe[index] = new sc_out<bool>(str_sgatpe.str().c_str());

        sgatpe[index]->initialize(0);
    }
    for (unsigned i = 0; i <= emNumPE; i++) {
        std::ostringstream str_errout_clear_mask_out;
        std::ostringstream str_errout_clear_mask_in;
        str_errout_clear_mask_out << "errout_clear_mask_out" << i;
        str_errout_clear_mask_in << "errout_clear_mask_in" << i;
        errout_clear_mask_out[i] = new sc_out<bool> (str_errout_clear_mask_out.str().c_str());
        errout_clear_mask_in[i] = new sc_in<bool> (str_errout_clear_mask_in.str().c_str());
        errout_clear_mask_out[i]->initialize(true);
    }
    for(unsigned int index = 0; index < emNumPE; index++){
        std::ostringstream str_sgati_pe;
        std::ostringstream str_errout_pe;
        std::ostringstream str_sgatnmi;
        std::ostringstream str_sgaterroz_n;

        str_sgati_pe<<"sgati_pe"<<(index+1);// start from 1, not from 0
        str_errout_pe<<"errout_pe"<<(index+1);// start from 1, not from 0
        str_sgatnmi<<"sgatnmi"<<(index+1);// start from 1, not from 0
        str_sgaterroz_n << "sgaterroz_n" << index;

        sgati_pe[index]     = new sc_out<bool>(str_sgati_pe.str().c_str());
        errout_pe[index]    = new sc_out<bool>(str_errout_pe.str().c_str());
        sgatnmi[index]      = new sc_out<bool>(str_sgatnmi.str().c_str());
        sgaterroz_n[index]  = new sc_out<bool>(str_sgaterroz_n.str().c_str());

        sgati_pe[index]->initialize(0);
        errout_pe[index]->initialize(0);
        sgatnmi[index]->initialize(0);
        sgaterroz_n[index]->initialize(0);
    }


    Cecm_u2a_regif::set_instance_name(this->name());//set hierarchy name
    //initial output port
    sgaterroz.initialize(false);
    sgaterroutz.initialize(false);
    sgatresz.initialize(true);
    dtmtrgout.initialize(false);
    pseudo_compare_msk_m.initialize(false);
    pseudo_compare_msk_c.initialize(false);
    //Initialize internal variables
    mSyncSignal             = emECMErrorOutputActiveLevel;
    mSGATERROZ              = emECMErrorOutputActiveLevel;
    mSGATERROUTZ            = emECMErrorOutputActiveLevel;
    mSGATRESZ               = emECMInternalResetInactiveLevel;
    mCountClock             = 0;
    mPCLKClock              = 0;
    mIsECMReset             = false;
    mIsECMerroutreszReset   = false;
    mIsECMresstg1zReset     = false;
    mIsECMpclkin_resstg1zReset = false;
    mECMEOCCFG              = 0;
    for(unsigned int i = 0; i < emNumStatusReg; i++){
        mECMmESSTR[i]       = 0;
    }
    mStartSimulation        = false;
    for(unsigned int i=0; i<emNumErrorFactor; i++){
        mPseudoErrorArray[i] = 0;
    }
    mIsLockReg              = true;
    for(unsigned int i = 0; i < emNumPE; i++){
        mERROUTPEArray[i]          = false;
        mSGATIPEArray[i]           = false;
        mSGATNMI[i]                = emECMInterruptInactiveLevel;
        mSGATI[i]                  = emECMInterruptInactiveLevel;
        mSyncSignalPe[i]           = emECMErrorOutputActiveLevel;
        mSGATERROZn[i]             = emECMErrorOutputActiveLevel;
        mECMEOCCFGn[i]             = 0;
    }
    for(unsigned int i = 0; i <= emNumPE; i++){
        mHoldSignal[i]             = true;
        mRestartCounterTime[i]     = 0;
    }
    //construct table
    ErrorInputTableConstruction();
    ProtectedRegisterTableConstruction();
    //Define special errors
    unsigned int TempListSpecialError[emTotalSpecialError] = { 33, 34,                                             // Factors that None Interrupt
                                                               33, 34, 48, 49, 50, 51, 52, 53, 229, 261, 293, 325, // Factors that None Delay Timer
                                                               255, 287, 319, 351,                                 // Factors that None Error Output
                                                               11, 33, 34, 255, 287, 319, 351,                     // Factors that None Error Trigger 
                                                               33, 34,                                             // Factors that None Internal Reset
                                                               33, 34                                              // Factors that Checked after reset (except application reset) 
                                                             };
    for (unsigned int i = 0; i < emTotalSpecialError; i++) {
        mListSpecialError[i] = TempListSpecialError[i];
    }

    //Initialize mForcedPortArray
    for(unsigned int i = 0; i < emNumPE; i++){
        mForcedPortArray.push_back(sgati_pe[i]);
        mForcedPortArray.push_back(errout_pe[i]);
        mForcedPortArray.push_back(sgatnmi[i]);
        mForcedPortArray.push_back(sgaterroz_n[i]);
    }
    mForcedPortArray.push_back(&sgaterroz);
    mForcedPortArray.push_back(&sgatresz);
    mForcedPortArray.push_back(&dtmtrgout);

    //Initialize General Timer
    InitializeGeneralTimer();

    //Initialize registers for supported/reserved error factors (based on TARGET_PRODUCT)
    SC_METHOD(InitializeRegistersMethod);

    SC_METHOD(ECMReceiveErrorInputMethod)
    dont_initialize();
    //sgaterrinN (N = 00 .. 343)
    for(unsigned int index = 0; index < emNumErrorFactor; index++){
        sensitive << (*sgaterrin[index]);
    }

    SC_METHOD(LoopBackSignalProcessingMethod)
    dont_initialize();
    sensitive << sgaterrlbz;

    SC_METHOD(ECMInitializeMethod)
    dont_initialize();
    sensitive << mECMInitializeEvent;

    SC_METHOD(resstg1zMethod)
    dont_initialize();
    sensitive << resstg1z;

    SC_METHOD(resstg1z_pclkinMethod)
    dont_initialize();
    sensitive << resstg1z_pclkin;

    SC_METHOD(sgattinMethod)
    dont_initialize();
    sensitive << sgattin;

    SC_METHOD(WriteSGATERROZPortMethod)
    dont_initialize();
    sensitive << mWriteSGATERROZPortEvent;

    SC_METHOD(WriteSGATERROUTZPortMethod)
    dont_initialize();
    sensitive << mWriteSGATERROUTZPortEvent;

    SC_METHOD(WriteSGATRESZPortMethod)
    dont_initialize();
    sensitive << mWriteSGATRESZPortEvent;

    SC_METHOD(WriteSGATNMIPortMethod)
    dont_initialize();
    sensitive << mWriteSGATNMIPortEvent;

    SC_METHOD(WriteCompareMaskMethod)
    dont_initialize();
    sensitive << mWriteCompareMaskEvent;

    sc_core::sc_spawn_options otp0;
    otp0.spawn_method();
    otp0.set_sensitivity(&mWriteDTMTRGOUTPortEvent[0]);
    sc_core::sc_spawn(sc_bind(&Cecm_u2a::WriteDTMTRGOUTPortMethod, this, false), sc_core::sc_gen_unique_name("WriteDTMTRGOUTPortMethod"), &otp0);
    dont_initialize();

    sc_core::sc_spawn_options otp1;
    otp1.spawn_method();
    otp1.set_sensitivity(&mWriteDTMTRGOUTPortEvent[1]);
    sc_core::sc_spawn(sc_bind(&Cecm_u2a::WriteDTMTRGOUTPortMethod, this, true) , sc_core::sc_gen_unique_name("WriteDTMTRGOUTPortMethod"), &otp1);
    dont_initialize();

    SC_METHOD(DelayTimerConfigureMethod)
    dont_initialize();
    sensitive << dtmtrgin;

    SC_METHOD(DelayTimerStopMethod)
    dont_initialize();
    sensitive << mDelayTimerStopEvent;

    for (unsigned i = 0; i <= emNumPE; i++) {
        sc_core::sc_spawn_options otp;
        otp.spawn_method();
        otp.set_sensitivity(&mEnableClearOutputErrEvent[i]);
        sc_core::sc_spawn(sc_bind(&Cecm_u2a::EnableClearOutputErrMethod, this, i) , sc_core::sc_gen_unique_name("EnableClearOutputErrMethod"), &otp);
        dont_initialize();

        sc_core::sc_spawn_options otp_port;
        otp_port.spawn_method();
        otp_port.set_sensitivity(&mWriteErrOutMskClrPortEvent[i]);
        sc_core::sc_spawn(sc_bind(&Cecm_u2a::WriteErrOutMskClrPortMethod, this, i) , sc_core::sc_gen_unique_name("WriteErrOutMskClrPortMethod"), &otp_port);
        dont_initialize();

        sc_core::sc_spawn_options clr_oz;
        clr_oz.spawn_method();
        clr_oz.set_sensitivity(&mProcessECMmECTBitEvent[i]);
        sc_core::sc_spawn(sc_bind(&Cecm_u2a::ProcessECMmECTBitMethod, this, i) , sc_core::sc_gen_unique_name("ProcessECMmECTBitMethod"), &clr_oz);
        dont_initialize();
    }

    SC_METHOD(ResetForErrpinMethod)
    dont_initialize();
    sensitive << erroutresz;

    SC_METHOD(WritePseudoErrorOutMethod)
    dont_initialize();
    sensitive << mWritePseudoErrorOutEvent;

    SC_METHOD(CombineSGATIPEPortMethod)
    dont_initialize();
    sensitive << mCombineSGATIPEPortEvent;
    for(unsigned int index = 0; index < emNumPE; index++){
        sensitive << (*errout_pe[index]);
    }

    SC_METHOD(WriteSGATIPEPortMethod)
    dont_initialize();
    sensitive << mWriteSGATIPEPortEvent;

    SC_METHOD(WriteDCLSErrorPortMethod)
    dont_initialize();
    sensitive << mWriteDCLSErrorPortEvent;

    SC_METHOD(NegateInterruptMethod)
    dont_initialize();
    sensitive << mNegateInterruptEvent;

    SC_METHOD(UpdateDTMSTACNTCLKBitMethod)
    dont_initialize();
    sensitive << mUpdateDTMSTACNTCLKBitEvent;

}//}}}

//Destructor of Cecm_u2a class
Cecm_u2a::~Cecm_u2a()
{//{{{
}//}}}

void Cecm_u2a::start_of_simulation(void)
{//{{{
    mStartSimulation = true;
}//}}}

unsigned int Cecm_u2a::GetRegisterIndex(unsigned int factor_index)
{//{{{
    return factor_index/32;
}//}}}

std::string Cecm_u2a::GetBitName( const char name[], unsigned int bit_idx, unsigned int reg_index)
{//{{{
    std::ostringstream bit_name;
    bit_name << std::string(name) << std::right << std::setfill('0') << std::setw(2) << bit_idx;
    return bit_name.str().c_str();
}//}}} 

void Cecm_u2a::ErrorInputTableConstruction(void)
{//{{{
    unsigned int reg_index = 0;
    unsigned int bit_idx = 0;
    std::string sse;
    std::string mis;
    std::string nmie0;
    std::string nmie1;
    std::string nmie2;
    std::string nmie3;
    std::string ire;
    std::string emk;
    std::string etmk0;
    std::string etmk1;
    std::string etmk2;
    std::string etmk3;
    std::string clsse;
    std::string pe;
    std::string cfg;

    for(unsigned int i = 0; i < emNumErrorFactor; i++){
        reg_index = GetRegisterIndex(i);
        bit_idx = i % 32;
        sse = GetBitName("ECMmSSE",bit_idx, reg_index);
        mis = GetBitName("ECMIS",bit_idx, reg_index);
        nmie0 = GetBitName("ECMIE",bit_idx, reg_index);
        nmie1 = GetBitName("ECMIE",bit_idx, reg_index);
        nmie2 = GetBitName("ECMIE",bit_idx, reg_index);
        nmie3 = GetBitName("ECMIE",bit_idx, reg_index);
        ire = GetBitName("ECMIRE",bit_idx, reg_index);
        emk = GetBitName("ECMEMK",bit_idx, reg_index);
        etmk0 = GetBitName("ECMETMK",bit_idx, reg_index);
        etmk1 = GetBitName("ECMETMK",bit_idx, reg_index);
        etmk2 = GetBitName("ECMETMK",bit_idx, reg_index);
        etmk3 = GetBitName("ECMETMK",bit_idx, reg_index);
        clsse = GetBitName("ECMCLSSE",bit_idx, reg_index);
        pe = GetBitName("ECMPE",bit_idx, reg_index);
        cfg = GetBitName("ECMDTME", bit_idx, reg_index);
        mErrorInputTable.push_back(ECMErrorInputElement( sgaterrin[i]
                                                           , sgatpe[i]
                                                           , false
                                                           , &(*ECMmESSTR[reg_index])[sse.c_str()]
                                                           , &(*ECMISCFG[reg_index])[mis.c_str()]
                                                           , &(*ECMINCFG0[reg_index])[nmie0.c_str()] 
                                                           , &(*ECMINCFG1[reg_index])[nmie1.c_str()] 
                                                           , &(*ECMINCFG2[reg_index])[nmie2.c_str()] 
                                                           , &(*ECMINCFG3[reg_index])[nmie3.c_str()] 
                                                           , &(*ECMIRCFG[reg_index])[ire.c_str()] 
                                                           , &(*ECMEMK[reg_index])[emk.c_str()]
                                                           , &(*ECMETMK0[reg_index])[etmk0.c_str()]
                                                           , &(*ECMETMK1[reg_index])[etmk1.c_str()]
                                                           , &(*ECMETMK2[reg_index])[etmk2.c_str()]
                                                           , &(*ECMETMK3[reg_index])[etmk3.c_str()]
                                                           , &(*ECMESSTC[reg_index])[clsse.c_str()]
                                                           , &(*ECMPE[reg_index])[pe.c_str()]
                                                           , &(*ECMDTMCFG[reg_index])[cfg.c_str()]
                                                           , &(*ECMDTMCFG0[reg_index])[cfg.c_str()]
                                                           , &(*ECMDTMCFG1[reg_index])[cfg.c_str()]
                                                           , &(*ECMDTMCFG2[reg_index])[cfg.c_str()]
                                                           , &(*ECMDTMCFG3[reg_index])[cfg.c_str()]
                                                            ) );
    }
}//}}}

void Cecm_u2a::ProtectedRegisterTableConstruction(void)
{//{{{
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET",  0x000   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET0", 0x008   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET1", 0x010   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET2", 0x018   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET3", 0x020   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR",  0x004   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR0", 0x00C   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR1", 0x014   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR2", 0x01C   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR3", 0x024   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET",  0x000   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET0", 0x008   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET1", 0x010   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET2", 0x018   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET3", 0x020   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR",  0x004   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR0", 0x00C   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR1", 0x014   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR2", 0x01C   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR3", 0x024   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEPCFG",  0x100   ,   emCommonArea)); 
    std::map<std::string, unsigned int > prot_reg_name;
    prot_reg_name["ECMISCFG"]  =   0x104;
    prot_reg_name["ECMINCFG0"] =   0x140;
    prot_reg_name["ECMINCFG1"] =   0x17C;
    prot_reg_name["ECMINCFG2"] =   0x1B8;
    prot_reg_name["ECMINCFG3"] =   0x1F4;
    prot_reg_name["ECMIRCFG"]  =   0x320;
    prot_reg_name["ECMEMK"]   =    0x35C;
    prot_reg_name["ECMETMK0"] =    0x398;
    prot_reg_name["ECMETMK1"] =    0x3D4;
    prot_reg_name["ECMETMK2"] =    0x410;
    prot_reg_name["ECMETMK3"] =    0x44C;
    prot_reg_name["ECMESSTC"] =    0x578;
    prot_reg_name["ECMPE"]    =    0x5B8;
    prot_reg_name["ECMDTMCFG"] =    0x600;
    prot_reg_name["ECMDTMCFG0"] =    0x63C;
    prot_reg_name["ECMDTMCFG1"] =    0x678;
    prot_reg_name["ECMDTMCFG2"] =    0x6B4;
    prot_reg_name["ECMDTMCFG3"] =    0x6F0;
    std::map<std::string, unsigned int>::iterator p;
    for (p = prot_reg_name.begin(); p != prot_reg_name.end(); ++p) {
        for (unsigned i = 0; i < emNumStatusReg ; i++) {
            std::ostringstream str_reg_name;
            str_reg_name << p->first << i;
            mProtectedRegisterTable.push_back(ProtectedRegisterElement(str_reg_name.str().c_str(),  p->second + (i*4)  ,   emCommonArea)); 
            mProtectedRegisterTable.push_back(ProtectedRegisterElement(str_reg_name.str().c_str(),  p->second + (i*4) +1 ,   emCommonArea)); 
            mProtectedRegisterTable.push_back(ProtectedRegisterElement(str_reg_name.str().c_str(),  p->second + (i*4) +2 ,   emCommonArea)); 
            mProtectedRegisterTable.push_back(ProtectedRegisterElement(str_reg_name.str().c_str(),  p->second + (i*4) +3 ,   emCommonArea)); 
        }
    }
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCTL", 0x5F4   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x5FC   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x5FD   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x5FE   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x5FF   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG", 0x600   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG", 0x601   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG", 0x602   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG", 0x603   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEOCCFG",  0x81C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEOCCFG",  0x81D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEOCCFG",  0x81E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEOCCFG",  0x81F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG0",  0x820   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG0",  0x821   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG0",  0x822   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG0",  0x823   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG1",  0x824   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG1",  0x825   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG1",  0x826   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG1",  0x827   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG2",  0x828   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG2",  0x829   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG2",  0x82A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG2",  0x82B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG3",  0x82C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG3",  0x82D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG3",  0x82E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMETCCFG3",  0x82F   ,   emCommonArea)); 
}//}}}

void Cecm_u2a::ECMEnableReset(const bool is_active)
{//{{{
    mIsECMReset = is_active;
    if (is_active){
        //Cancel all event
        mWriteSGATERROZPortEvent.cancel();
        mWriteSGATERROUTZPortEvent.cancel();
        mWriteSGATRESZPortEvent.cancel();
        mWriteSGATNMIPortEvent.cancel();
        mWriteDTMTRGOUTPortEvent[0].cancel();
        mWriteDTMTRGOUTPortEvent[1].cancel();
        for (unsigned i = 0; i <= emNumPE; i++ ){
//            mEnableClearOutputErrEvent[i].cancel(); // removed by TrungPham 2020.03.16: error output clear invalidation not cancelled on preset_n
            mProcessECMmECTBitEvent[i].cancel();
        }
        mCombineSignalEvent.cancel();
        mUpdateDTMSTACNTCLKBitEvent.cancel();
        mWriteDCLSErrorPortEvent.cancel();
        mDelayTimerStopEvent.cancel();

        //Initialize output ports
        mECMInitializeEvent.notify();

        //backup current value of ECMEOCCFG and ECMmESSTR0/1/2/3
        if (erroutresz.read() != vpcl::emResetActive ) {
            mECMEOCCFG = (unsigned int)(*ECMEOCCFG);    
            for (unsigned int i = 0; i < emNumPE; i++) {
                mECMEOCCFGn[i] = (unsigned int)(*ECMETCCFG[i]);
            }
        }
        for(unsigned int i = 0; i < emNumStatusReg; i++) {
            mECMmESSTR[i] = (unsigned int)(*ECMmESSTR[i]);
        }

        Cecm_u2a_regif::EnableReset(is_active);
        if (erroutresz.read() != vpcl::emResetActive ) {
            (*ECMEOCCFG).EnableReset(false);
            for (unsigned int i = 0; i < emNumPE; i++) {
                (*ECMETCCFG[i]).EnableReset(false);
            }
        }
        for(unsigned int i = 0; i < emNumStatusReg; i++){
            (*ECMmESSTR[i]).EnableReset(false);
        }

        if (erroutresz.read() != vpcl::emResetActive ) {
            (*ECMEOCCFG)   = mECMEOCCFG;
            for (unsigned int i = 0; i < emNumPE; i++) {
                (*ECMETCCFG[i]) = mECMEOCCFGn[i];
            }
        }
        for(unsigned int i = 1; i < emNumStatusReg; i++){
            if ( resstg1z_pclkin.read() != vpcl::emResetActive ) {
                (*ECMmESSTR[i])  = mECMmESSTR[i]; 
            }
        }
        if (resstg1z.read() == vpcl::emResetActive) {
            mECMmESSTR[0] = mECMmESSTR[0] & 0xFFFFF801; 
        }
        if ( resstg1z_pclkin.read() == vpcl::emResetActive ) {
            mECMmESSTR[0] = mECMmESSTR[0] & 0x7FF; 
        }
        (*ECMmESSTR[0])  = mECMmESSTR[0]; 
    }else{
        InitializeGeneralTimer();
        Cecm_u2a_regif::EnableReset(is_active);
        InitializeRegistersMethod();
    }

    //Call the reset function of common/general class
    Cgeneral_timer::EnableReset(is_active);
}//}}}

void Cecm_u2a::ECMInitializeMethod(void)
{//{{{
    //Initialize output port
    //sgatpeN (N = 000 .. 343)
    for(unsigned int i = 0; i < emNumErrorFactor; i++){
        mPseudoErrorArray[i] = 0;
    }
    mIsLockReg = true;
    for(unsigned int i = 0; i < emNumPE; i++){
        mSGATIPEArray[i] = false;
        mERROUTPEArray[i] = false;
        mSGATNMI[i] = emECMInterruptInactiveLevel;
        mSGATI[i] = emECMInterruptInactiveLevel;
    }
    mWritePseudoErrorOutEvent.notify();
    mWriteSGATIPEPortEvent.notify();
    mWriteDCLSErrorPortEvent.notify();

    //Initialize internal variables
    mCombineSGATIPEPortEvent.notify();
    mWriteSGATNMIPortEvent.notify();
    mSGATRESZ = emECMInternalResetInactiveLevel;
    mWriteSGATRESZPortEvent.notify();
    mWriteDTMTRGOUTPortEvent[0].notify();
    mWriteDTMTRGOUTPortEvent[1].notify();
    mWriteCompareMaskEvent.notify();
}//}}}

bool Cecm_u2a::ECMCheckZeroClock(const std::string clk_name)
{//{{{
    bool ret = true;
    if ((clk_name == "pclk") && (mPCLKClock == 0)) {
        ret = false;        
    }
    if ((clk_name == "cntclk") && (mCountClock == 0)) {
        ret = false;        
    }
    return ret;
}//}}}

bool Cecm_u2a::ECM_common_tgt_rd(const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    if (is_rd_dbg){
        // Set debug mode (member variable of Cecm_u2a_regif class)
        return reg_rd_dbg(addr, p_data, size);
    }else{
        return reg_rd(addr, p_data, size);
    }
}//}}}

bool Cecm_u2a::ECM_common_tgt_wr(const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    if (is_wr_dbg){
        return reg_wr_dbg( addr,p_data, size);
    }else{
        sc_assert(p_data != NULL);

        //if write to a protected register or command register
        if(CheckProtectedRegister(addr)){
            if (mIsLockReg){
                re_printf("warning", "Cannot write to protection register %s because of lock register.", GetProtectedRegisterName(addr).c_str());
                return true;
            }else{
                return reg_wr(addr, p_data, size);
            }
            //if write to a normal register
        }else{
            return reg_wr(addr, p_data, size);
        }
    }
}//}}} 

void Cecm_u2a::cmpMatchOutputHandling (bool value)
{//{{{
    //turn on status of delay timer overflow
    mIsDTMActive = false;
    for (unsigned int error_index = (emCompareErrorId); error_index < mErrorInputTable.size(); error_index++){
        if (((unsigned int)*mErrorInputTable[error_index].dtmcfg > 0) 
                && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].dtmcfg)){
            (*ECMmESSTR[0])["ECMmSSE11"] = 1; 
            break;
        }
    }
    for (unsigned i = 0; i < emNumPE; i++) {
        for (unsigned int error_index = (emCompareErrorId+1); error_index < mErrorInputTable.size(); error_index++){
            if (((unsigned int)*mErrorInputTable[error_index].dtmcfgn[i] > 0) 
                    && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].dtmcfgn[i])){
                (*ECMmESSTR[0]) = (unsigned int) (*ECMmESSTR[0]) | (1 << (12+i));
                break;
            }
        }
    }
    //Issue terminal error output
    IssueErrorOutput();
    //Issue internal reset
    IssueInternalReset();
}//}}}

bool Cecm_u2a::isDCLSError (unsigned int error_idx)
{//{{{
    if (error_idx >= emPEStartErrorId) {
        return ( ((error_idx - emPEStartErrorId) % emPETotalError) == 0);
    } else {
        return 0;
    }
}//}}}

unsigned int Cecm_u2a::mapDCLS_PEOUT (unsigned int error_idx)
{//{{{
   return ((error_idx - emPEStartErrorId)/emPETotalError);
}//}}}

void Cecm_u2a::ECMReceiveErrorInputMethod(void)
{//{{{
    if (mIsECMReset && errin_mask_reset) {
        return;
    }
    bool IsError[emNumPE + 1];
    for (unsigned i = 0; i <= emNumPE; i++ ) {
        IsError[i] = false;
    }
    for(unsigned int error_index = emPseudoErrorStartID; error_index < mErrorInputTable.size(); error_index++){
        //There is error input
        if ((mIsECMReset || mIsECMerroutreszReset || mIsECMresstg1zReset ||  mIsECMpclkin_resstg1zReset) && (!CheckSpecialError(error_index, emStartIDCheckAfterReset, emTotalSpecialError))) {
            continue;
        }
        if ((mErrorInputTable[error_index].errorin_pre != mErrorInputTable[error_index].errorin->read()) && (mErrorInputTable[error_index].errorin->read() != 0)){
            for (unsigned i = 0; i <= emNumPE; i++ ) {
                IsError[i] = true;
//            if (error_index >= emPEStartErrorId) {
//                IsError[((error_index - emPEStartErrorId)/emPETotalError) + 1] = true;
            }
        }
    }
    UpdateErrorStatus();    //Update error status
    if (IsError[0]) 
        IssueInterrupt();       //Issue interrupt signal
    IssueErrorOutput();     //Issue error output
    for (unsigned i = 1; i <= emNumPE; i++) {
        if (IsError[i] && (mSGATERROZn[i-1] == emECMErrorOutputActiveLevel) ) {
            if ((*ECMETCCFG[i-1])["EOCIEN"] == 1) { // error output clear invalidation function enabled
                mHoldSignal[i] = false;
                mWriteErrOutMskClrPortEvent[i].notify();
                mRestartCounterTime[i] = GetCurTime();
                mEnableClearOutputErrEvent[i].notify(mCountClock*((unsigned int)((*ECMETCCFG[i-1])["ECMEOUTCLRT"]) + 2),SC_NS); // Error output clear is invalid for (ECMEOUTCLRT+2) cntclk cycles
            }
        }
    }
    if (IsError[0]) {
        if (mSGATERROZ == emECMErrorOutputActiveLevel) {
            mEnableClearOutputErrEvent[0].cancel();
            if ((*ECMEOCCFG)["EOCIEN"] == 1) { // error output clear invalidation function enabled
                mHoldSignal[0] = false;
                mWriteErrOutMskClrPortEvent[0].notify();
                mRestartCounterTime[0] = GetCurTime();
                mEnableClearOutputErrEvent[0].notify(mCountClock*((unsigned int)((*ECMEOCCFG)["ECMEOUTCLRT"]) + 2),SC_NS); // Error output clear is invalid for (ECMEOUTCLRT+2) cntclk cycles
            }
            mWriteSGATERROZPortEvent.notify();
            if ((*ECMEPCFG)["ECMSL0"] == 0){ //non-dynamic mode
                mWriteSGATERROUTZPortEvent.notify();
            }
        }
    }
    IssueInternalReset();   //Issue internal reset
}//}}}

void Cecm_u2a::UpdateErrorStatus(void)
{//{{{
    if (resstg1z_pclkin.read() == vpcl::emResetActive) {
        for(unsigned int i = 1; i < emNumStatusReg; i++) {
            (*ECMmESSTR[i]) = 0x0;
        }
    }
    for(unsigned int error_index = 2; error_index < mErrorInputTable.size(); error_index++){
        if ((mIsECMReset || mIsECMerroutreszReset || mIsECMresstg1zReset ||  mIsECMpclkin_resstg1zReset) && (!CheckSpecialError(error_index, emStartIDCheckAfterReset, emTotalSpecialError))) {
            continue;
        }
        // DCLS
        if (mErrorInputTable[error_index].errorin->read() == 1) {
            (*mErrorInputTable[error_index].esstr) = 1;
        }
        mPseudoErrorArray[error_index] = 0;
        // Update pre error
        if (mErrorInputTable[error_index].errorin_pre != mErrorInputTable[error_index].errorin->read()) {
            mErrorInputTable[error_index].errorin_pre = mErrorInputTable[error_index].errorin->read();
        }
        // Removed by TrungPham (11/20/2019)
//        if (resstg1z_pclkin.read() == vpcl::emResetActive) {
//            *mErrorInputTable[error_index].esstr = mErrorInputTable[error_index].errorin->read();
//        }
    }
    mWritePseudoErrorOutEvent.notify(mPCLKClock,SC_NS);

    // Check resstg1z_pclkin 
    if (resstg1z_pclkin.read() == vpcl::emResetActive) {
        (*ECMmESSTR[0]) = (unsigned int) (*ECMmESSTR[0]) & 0x0007FF; // reset delay timer overflow
    }
    // Check resstg1z is active
    resstg1zMethod();
}//}}}

void Cecm_u2a::IssueInterrupt(void)
{//{{{
//    mSGATNMI = emECMInterruptInactiveLevel;
    bool dtmtrgout_val = false;
    for (unsigned int i = 0; i < emNumPE; i++){
        mSGATI[i] = emECMInterruptInactiveLevel;
        mERROUTPEArray[i] = emECMInterruptInactiveLevel;
        mSGATNMI[i] = emECMInterruptInactiveLevel;
    }

    for(unsigned int error_index = emCompareErrorId; error_index < mErrorInputTable.size(); error_index++){
        if ((mIsECMReset || mIsECMerroutreszReset || mIsECMresstg1zReset ||  mIsECMpclkin_resstg1zReset) && (!CheckSpecialError(error_index, emStartIDCheckAfterReset, emTotalSpecialError))) {
            continue;
        }
        //There is error input
        if (mErrorInputTable[error_index].errorin->read() != 0){
            for (unsigned i = 0; i < emNumPE; i++) {
                // Check Factors that None Interrupt
                if (!CheckSpecialError(error_index, emStartIDNoneInterrupt, emStartIDNoneDelayTimer)) {
                    if ((unsigned int)(*mErrorInputTable[error_index].incfg[i]) == 0)
                        continue;
                    if ((unsigned int)(*mErrorInputTable[error_index].misfg) == 0) {
                        // EI level interupt (for both DCLS and normal error)
                        if (isDCLSError(error_index)){// For DCLS mask interrupt port
                            mERROUTPEArray[mapDCLS_PEOUT(error_index)] = emECMInterruptActiveLevel; // Activate
                        } else {
                            mSGATI[i] = emECMInterruptActiveLevel;
                        }
                    } else {
                        // FE level interrupt 
                        mSGATNMI[i] = emECMInterruptActiveLevel;
                    }
                }
                // Check Factors that None Delay Timer 
                if (!CheckSpecialError(error_index, emStartIDNoneDelayTimer, emStartIDNoneErrorOutput)) {
                    if (((unsigned int)(*mErrorInputTable[error_index].dtmcfg) > 0) 
                            && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].dtmcfg)){
                        dtmtrgout_val = true;
                    }
                    if (((unsigned int)(*mErrorInputTable[error_index].dtmcfgn[i]) > 0) 
                            && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].dtmcfgn[i])){
                        dtmtrgout_val = true;
                    }
                }
            }
        }
    }
    //write maskable interrupt and non-maskable interrupt
    mCombineSGATIPEPortEvent.notify();
    mWriteSGATNMIPortEvent.notify();
    //write value to dtmtrgout
    if (dtmtrgout_val && !mIsDTMActive && ECMCheckZeroClock("cntclk") && ECMCheckZeroClock("pclk")) {
        mIsDTMActive = true;
        mWriteDTMTRGOUTPortEvent[1].notify(3*mCountClock, SC_NS); // wait for 3 cntclk cycles to start delay timer
    }
}//}}}

void Cecm_u2a::DelayTimerConfigureMethod(void)
{//{{{
    if(dtmtrgin.read() && (((unsigned int)(*ECMDTMCTL) & 0x3) == 1)){ //if delay timer can start
        StartStopCounter(true);             //Start general timer
    }
}//}}}

void Cecm_u2a::DelayTimerStopMethod(void)
{//{{{
    mIsDTMActive = false;
    StartStopCounter(false); //Stop general timer
    Cgeneral_timer::setCNT(0x0);          //Count value
}//}}}

void Cecm_u2a::IssueErrorOutput(unsigned int src)
{//{{{
    CalculateErrorOutput(src);
    mWriteSGATERROZPortEvent.notify();
    //Non-dynamic mode, and there is error input
    if ((mSGATERROUTZ == emECMErrorOutputActiveLevel) || ((*ECMEPCFG)["ECMSL0"] == 0)){
        mWriteSGATERROUTZPortEvent.notify();
    }
}//}}}

void Cecm_u2a::CalculateErrorOutput(unsigned int src)
{//{{{
    bool status_mask = emECMErrorOutputInactiveLevel;
    bool status_mask_pe[emNumPE];
    bool error_in_reset = false;
    for (unsigned int i = 0; i < emNumPE; i++) {
        status_mask_pe[i] = emECMErrorOutputInactiveLevel;
    }
    // For all remains errors
    for(unsigned int error_index = 2; error_index < mErrorInputTable.size(); error_index++){
        if ((mIsECMReset || mIsECMerroutreszReset || mIsECMresstg1zReset ||  mIsECMpclkin_resstg1zReset) && (!CheckSpecialError(error_index, emStartIDCheckAfterReset, emTotalSpecialError))) {
            continue;
        }
        // There is error input and None mask (for both DCLS and normal error)
        // Check Factors that None Error Output 
        if (((unsigned int)*mErrorInputTable[error_index].esstr > (unsigned int)*mErrorInputTable[error_index].emk)
                && (!CheckSpecialError(error_index, emStartIDNoneErrorOutput, emStartIDNoneErrorTrigger))){
            status_mask = emECMErrorOutputActiveLevel;
            error_in_reset = true;
        }
        // Check Factors that None Error Trigger 
        for (unsigned i = 0; i < emNumPE; i++) {
            if (((unsigned int)*mErrorInputTable[error_index].esstr > (unsigned int)*mErrorInputTable[error_index].etmk[i])
                    && (!CheckSpecialError(error_index, emStartIDNoneErrorTrigger, emStartIDNoneInternalReset))){
                status_mask_pe[i] = emECMErrorOutputActiveLevel;
                error_in_reset = true;
            }
        }
    }
    if ((!(mIsECMReset || mIsECMerroutreszReset || mIsECMresstg1zReset ||  mIsECMpclkin_resstg1zReset)) || (error_in_reset)) {
        //There is delay timer overflow and None mask
        if (((*ECMmESSTR[0])["ECMmSSE11"] == 1) && (*ECMEMK[0])["ECMEMK11"] == 0){
            status_mask = emECMErrorOutputActiveLevel;
        }
        if (((*ECMmESSTR[0])["ECMmSSE12"] == 1) && (*ECMETMK0[0])["ECMETMK12"] == 0){
            status_mask_pe[0] = emECMErrorOutputActiveLevel;
        }
        if (((*ECMmESSTR[0])["ECMmSSE13"] == 1) && (*ECMETMK1[0])["ECMETMK13"] == 0){
            status_mask_pe[1] = emECMErrorOutputActiveLevel;
        }
        if (((*ECMmESSTR[0])["ECMmSSE14"] == 1) && (*ECMETMK2[0])["ECMETMK14"] == 0){
            status_mask_pe[2] = emECMErrorOutputActiveLevel;
        }
        if (((*ECMmESSTR[0])["ECMmSSE15"] == 1) && (*ECMETMK3[0])["ECMETMK15"] == 0){
            status_mask_pe[3] = emECMErrorOutputActiveLevel;
        }
    }
    if (src == 0 || src == emAllUnit) { // All source
        mSGATERROZ   = status_mask && mSyncSignal;
        mSGATERROUTZ = status_mask && mSyncSignal;
        mSyncSignal  = mSGATERROZ;
    }
    for (unsigned int i = 1; i <= emNumPE; i++) {
        if (src == i || src == emAllUnit) { // PE0
            mSGATERROZn[i-1]  = status_mask_pe[i-1] && mSyncSignalPe[i-1];
            mSyncSignalPe[i-1] = mSGATERROZn[i-1];
        }
    }
}//}}}

void Cecm_u2a::IssueInternalReset(void)
{//{{{
    for(unsigned int error_index = emPseudoErrorStartID; error_index < mErrorInputTable.size(); error_index++){
        if ((mIsECMReset || mIsECMerroutreszReset || mIsECMresstg1zReset ||  mIsECMpclkin_resstg1zReset) && (!CheckSpecialError(error_index, emStartIDCheckAfterReset, emTotalSpecialError))) {
            continue;
        }
        //There is error input and internal reset is permited, number of error count >= ircfg
        if (((error_index < emCompareErrorId || mErrorInputTable[error_index].errorin->read() != 0)
            && (*mErrorInputTable[error_index].ircfg > 0)
            && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].ircfg))
            && (!CheckSpecialError(error_index, emStartIDNoneInternalReset, emStartIDCheckAfterReset))){
            mSGATRESZ = emECMInternalResetActiveLevel;
            break;
        }
    }
    mWriteSGATRESZPortEvent.notify();
}//}}}

void Cecm_u2a::LoopBackSignalProcessingMethod(void)
{//{{{ 
    if (sgaterrlbz.read()){
        (*ECMmESSTR[0])["ECMmSSE00"] = 1;
    } else {
        (*ECMmESSTR[0])["ECMmSSE00"] = 0;
    }
}//}}}

void Cecm_u2a::resstg1zMethod(void)
{//{{{
    // This method NOT depend on preset_n. No need to check mIsECMReset
    //if resstg1z is active
    if ( resstg1z.read() == vpcl::emResetActive ) {
        mIsECMresstg1zReset = true;
        (*ECMmESSTR[0])  = (unsigned int) (*ECMmESSTR[0]) & 0xFFFFF801; 
    } else {
        mIsECMresstg1zReset = false;
    }
}//}}}

void Cecm_u2a::resstg1z_pclkinMethod(void)
{//{{{
    // This method NOT depend on preset_n. No need to check mIsECMReset
    if (resstg1z_pclkin.read() == vpcl::emResetActive) {
        mIsECMpclkin_resstg1zReset = true;
        UpdateErrorStatus();
        IssueErrorOutput(); 
        IssueInternalReset();
    } else {
        mIsECMpclkin_resstg1zReset = false;
    }
}//}}}

void Cecm_u2a::sgattinMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    CalculateErrorOutput();
    //Dynamic, and there is no error input, toggle by timer input
    if (((*ECMEPCFG)["ECMSL0"] == 1) && (mSGATERROUTZ == emECMErrorOutputInactiveLevel)){
        if (sgattin.read()){
            mSGATERROUTZ = emECMErrorOutputActiveLevel;
        }else{
            mSGATERROUTZ = emECMErrorOutputInactiveLevel;
        }
        mWriteSGATERROUTZPortEvent.notify();
    }
}//}}}

void Cecm_u2a::WriteSGATERROZPortMethod(void)
{//{{{
    sgaterroz.write(mSGATERROZ);
    for (unsigned int i = 0; i < emNumPE; i++) {
        sgaterroz_n[i]->write(mSGATERROZn[i]);
    }
}//}}}

void Cecm_u2a::WriteSGATERROUTZPortMethod(void)
{//{{{ 
    sgaterroutz.write(mSGATERROUTZ);
}//}}}

void Cecm_u2a::WriteSGATRESZPortMethod(void)
{//{{{
    sgatresz.write(mSGATRESZ);
}//}}}

void Cecm_u2a::WriteSGATNMIPortMethod(void)
{//{{{
    if (!ECMCheckZeroClock("pclk")) {
        return;
    }
    for(unsigned int i = 0; i < emNumPE; i++){
        sgatnmi[i]->write(mSGATNMI[i]);
        // Negate interrupts after 1 pclk cycle
        if (mSGATNMI[i] == emECMInterruptActiveLevel) {
            mSGATNMI[i] = emECMInterruptInactiveLevel;
            mWriteSGATNMIPortEvent.notify(mPCLKClock, SC_NS);
        }
    }
}//}}}

void Cecm_u2a::WriteDTMTRGOUTPortMethod(bool dtmtrgout_val)
{//{{{
    dtmtrgout.write(dtmtrgout_val);

    // Negate dtmtrgout after 1 pclk cycle
    if (dtmtrgout_val) {
        mWriteDTMTRGOUTPortEvent[0].notify(mPCLKClock, SC_NS);
    }
}//}}}

void Cecm_u2a::WriteErrOutMskClrPortMethod(unsigned i)
{//{{{
//    if (mIsECMReset){ // removed by DucD 2020.03.04 to fix #117731
//        return;
//    }
    errout_clear_mask_out[i]->write(mHoldSignal[i]);
}//}}}

void Cecm_u2a::WriteCompareMaskMethod(void)
{//{{{
    pseudo_compare_msk_m.write((bool)((unsigned int)(*ECMPEM)["MSKM"]));
    pseudo_compare_msk_c.write((bool)((unsigned int)(*ECMPEM)["MSKC"]));
}//}}}

bool Cecm_u2a::CheckProtectedRegister(unsigned int addr)
{//{{{
    addr = addr & 0xFFF;
    for(unsigned int index = 0; index < mProtectedRegisterTable.size(); index++){
        if (addr == mProtectedRegisterTable[index].addr){
            return true;
        }
    }
    return false;
}//}}}

std::string Cecm_u2a::GetProtectedRegisterName(unsigned int addr)
{//{{{
    addr = addr & 0xFFF;
    std::string ret = "";
    for(unsigned int index = 0; index < mProtectedRegisterTable.size(); index++){
        if (addr == mProtectedRegisterTable[index].addr){
            ret = mProtectedRegisterTable[index].register_name;
            break;
        }
    }
    return ret;
}//}}}

void Cecm_u2a::StartStopCounter(const bool start)
{//{{{
    if (start) {
        if((unsigned int)((*ECMDTMCMP)["ECMDTMCMP"]) > 0){
            Cgeneral_timer::setClkCountPeriod(mCountClock);
            Cgeneral_timer::setCOR((unsigned int)((*ECMDTMCMP)["ECMDTMCMP"]) + 2); // compare match occurs at the end of next cntclk cycle
            Cgeneral_timer::setSTR(true);
        } else { //compare match
            cmpMatchOutputHandling(true);
        }
    }else{
        Cgeneral_timer::setSTR(false);
    }
}//}}}

void Cecm_u2a::InitializeGeneralTimer(void)
{//{{{
    Cgeneral_timer::setSTR(false);                  //Count permission
    Cgeneral_timer::setCMS(25U);
    Cgeneral_timer::setCMM(emOneShot);              //Operation mode
    Cgeneral_timer::setCounterDirection(emCountUp); //Count mode
    Cgeneral_timer::setCCLR(emInFactor);            //Clear mode
    Cgeneral_timer::setTimeUnit(SC_NS);             //Time unit
    Cgeneral_timer::setCOR(0x0);                    //Match value
    Cgeneral_timer::setCNT(0x0);                    //Count value
    Cgeneral_timer::setCKS(emClkDivide);            //Divide Input clock
    mIsDTMActive = false;
}//}}} 

void Cecm_u2a::InitializeRegistersMethod(void)
{//{{{
    (*ECMIRCFG[7])  = 0x10;
}//}}} 

void Cecm_u2a::EnableClearOutputErrMethod(unsigned i)
{//{{{ 
    mHoldSignal[i] = true;
    mWriteErrOutMskClrPortEvent[i].notify();
}//}}}

void Cecm_u2a::ECMSetZeroClock(const std::string clk_name)
{//{{{
    //cancel all processes 
    mWriteSGATERROZPortEvent.cancel();
    mWriteSGATERROUTZPortEvent.cancel();
    mWriteSGATRESZPortEvent.cancel();
    mCombineSGATIPEPortEvent.cancel();
    mWriteSGATIPEPortEvent.cancel();
    mWriteSGATNMIPortEvent.cancel();
    for (unsigned i = 0; i <= emNumPE; i++ ){
        mEnableClearOutputErrEvent[i].cancel();
    }
    mCombineSignalEvent.cancel();

    if (clk_name == "cntclk") {
        mWriteDTMTRGOUTPortEvent[0].cancel();
        mWriteDTMTRGOUTPortEvent[1].cancel();
        StartStopCounter(false); //Stop general timer
    }
}//}}}

void Cecm_u2a::ResetForErrpinMethod(void)
{//{{{
    if (erroutresz.read() == vpcl::emResetActive) {
        mIsECMerroutreszReset = true;
        (*ECMEOCCFG) = 0; 
        for (unsigned int i = 0; i < emNumPE; i++) {
            (*ECMETCCFG[i]) = 0;
            mSGATERROZn[i] = emECMErrorOutputActiveLevel;
            mSyncSignalPe[i] = emECMErrorOutputActiveLevel;
        }
        for (unsigned int i = 0; i <= emNumPE; i++) {
            mHoldSignal[i] = true;
            mWriteErrOutMskClrPortEvent[i].notify();
        }
        mSyncSignal = emECMErrorOutputActiveLevel;
        mSGATERROZ = emECMErrorOutputActiveLevel;
        mSGATERROUTZ = emECMErrorOutputActiveLevel;
        mWriteSGATERROZPortEvent.notify();
        mWriteSGATERROUTZPortEvent.notify();
    } else {
        mIsECMerroutreszReset = false;
        IssueErrorOutput();
    } 
}//}}}

void Cecm_u2a::WritePseudoErrorOutMethod()
{//{{{
    for(unsigned int error_index = emPseudoErrorStartID; error_index < mErrorInputTable.size(); error_index++){
        mErrorInputTable[error_index].pseudo_error_out->write(mPseudoErrorArray[error_index]);
    }
}//}}}

void Cecm_u2a::CombineSGATIPEPortMethod()
{//{{{ 
    // Combine sgati_pe0..3
    for(unsigned int i = 0; i < emNumPE; i++){
        mSGATIPEArray[i] = false;
        for(unsigned int j = 0; j < emNumPE; j++){
            if (i != j){
                mSGATIPEArray[i] |= mERROUTPEArray[j];
            }
        }
        mSGATIPEArray[i] |= mSGATI[i];
    }
    mWriteSGATIPEPortEvent.notify();
    mWriteDCLSErrorPortEvent.notify();

    // Negate interrupts after 1 pclk cycle
    mNegateInterruptEvent.notify(mPCLKClock, SC_NS);
}//}}}

void Cecm_u2a::NegateInterruptMethod()
{//{{{ 
    bool deactivate = false;
    for (unsigned int i = 0; i < emNumPE ; i++) {
        if (mERROUTPEArray[i] == emECMInterruptActiveLevel) {
            mERROUTPEArray[i] = emECMInterruptInactiveLevel;
            deactivate = true;
        }
        if (mSGATI[i] == emECMInterruptActiveLevel) {
            mSGATI[i] = emECMInterruptInactiveLevel;
            deactivate = true;
        }
    }
    if (deactivate) {
        mCombineSGATIPEPortEvent.notify();
    }
}//}}}

void Cecm_u2a::WriteSGATIPEPortMethod()
{//{{{ 
    if (!ECMCheckZeroClock("pclk")) {
        return;
    }
    for(unsigned int i = 0; i < emNumPE; i++){
        sgati_pe[i]->write(mSGATIPEArray[i]);
    }
}//}}}

void Cecm_u2a::UpdateDTMSTACNTCLKBitMethod()
{//{{{ 
    if (mIsECMReset || (!ECMCheckZeroClock("cntclk"))) {
        return;
    }
    (*ECMDTMCTL)["DTMSTACNTCLK"] = (unsigned int)(*ECMDTMCTL)["DTMSTA"];
}//}}}

void Cecm_u2a::WriteDCLSErrorPortMethod()
{//{{{  
    if (!ECMCheckZeroClock("pclk")) {
        return;
    }
    for(unsigned int i= 0; i < emNumPE; i++){
        errout_pe[i]->write(mERROUTPEArray[i]);
    }
}//}}}

void Cecm_u2a::ProcessECMmECTBitMethod(unsigned index)
{//{{{  
    // Check all error status are 0 or masked
    bool status_mask = emECMErrorOutputInactiveLevel;
    bool issue_err_flag = false;
    //There is delay timer overflow and None mask
    if (index == 0 && ((*ECMmESSTR[0])["ECMmSSE11"] == 1) && (*ECMEMK[0])["ECMEMK11"] == 0) {
        status_mask = emECMErrorOutputActiveLevel;
    }
    if (index == 1 && ((*ECMmESSTR[0])["ECMmSSE12"] == 1) && (*ECMETMK0[0])["ECMETMK12"] == 0) {
        status_mask = emECMErrorOutputActiveLevel;
    }
    if (index == 2 && ((*ECMmESSTR[0])["ECMmSSE13"] == 1) && (*ECMETMK1[0])["ECMETMK13"] == 0) {
        status_mask = emECMErrorOutputActiveLevel;
    }
    if (index == 3 && ((*ECMmESSTR[0])["ECMmSSE14"] == 1) && (*ECMETMK2[0])["ECMETMK14"] == 0) {
        status_mask = emECMErrorOutputActiveLevel;
    }
    if (index == 4 && ((*ECMmESSTR[0])["ECMmSSE15"] == 1) && (*ECMETMK3[0])["ECMETMK15"] == 0) {
        status_mask = emECMErrorOutputActiveLevel;
    }
    for(unsigned int error_index = 2; error_index < mErrorInputTable.size(); error_index++){
        //There is error input and None mask (for both DCLS and normal error)
        if (index == 0 && (unsigned int)*mErrorInputTable[error_index].esstr > (unsigned int)*mErrorInputTable[error_index].emk){
            status_mask = emECMErrorOutputActiveLevel;
        }
        if (index > 0 && (unsigned int)*mErrorInputTable[error_index].esstr > (unsigned int)*mErrorInputTable[error_index].etmk[index - 1]){
            status_mask = emECMErrorOutputActiveLevel;
        }
    }
    if (index == 0 && ((unsigned int)(*ECMmESSTR[0])["ECMmSSE02"] == 0) && (status_mask == emECMErrorOutputInactiveLevel)){
        mSyncSignal = emECMErrorOutputInactiveLevel;
        issue_err_flag = true;
    }
    if (index == 1 && ((unsigned int)(*ECMmESSTR[0])["ECMmSSE03"] == 0) && (status_mask == emECMErrorOutputInactiveLevel)){
        mSyncSignalPe[0] = emECMErrorOutputInactiveLevel;
        issue_err_flag = true;
    }
    if (index == 2 && ((unsigned int)(*ECMmESSTR[0])["ECMmSSE04"] == 0) && (status_mask == emECMErrorOutputInactiveLevel)){
        mSyncSignalPe[1] = emECMErrorOutputInactiveLevel;
        issue_err_flag = true;
    }
    if (index == 3 && ((unsigned int)(*ECMmESSTR[0])["ECMmSSE05"] == 0) && (status_mask == emECMErrorOutputInactiveLevel)){
        mSyncSignalPe[2] = emECMErrorOutputInactiveLevel;
        issue_err_flag = true;
    }
    if (index == 4 && ((unsigned int)(*ECMmESSTR[0])["ECMmSSE06"] == 0) && (status_mask == emECMErrorOutputInactiveLevel)){
        mSyncSignalPe[3] = emECMErrorOutputInactiveLevel;
        issue_err_flag = true;
    }
    if (issue_err_flag) 
        IssueErrorOutput(index);
}//}}}

void Cecm_u2a::cb_ECMESSTC_ECMCLSSE00(RegCBstr str)
{//{{{ 
    for(unsigned int error_index = 2; error_index < emNumErrorFactor; error_index++){
        if ((mErrorInputTable[error_index].errorin->read() == 0) && ((unsigned int)*mErrorInputTable[error_index].esstc > 0)){
            *mErrorInputTable[error_index].esstr = 0;
        }
    }
    (*ECMESSTC[str.channel]) = 0x0;
    IssueErrorOutput();
    IssueInternalReset();
}//}}}

void Cecm_u2a::cb_ECMISCFG_ECMIS00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMISCFG[0]) = str.data & 0xFFF00000; // Disable 0 -> 19
    }
}//}}}

void Cecm_u2a::cb_ECMINCFG0_ECMIE00(RegCBstr str)
{//{{{ 
    if (str.channel == 0) {
        (*ECMINCFG0[0]) = str.data & 0xFFF00000; // Disable 0 -> 19
    }
}//}}}

void Cecm_u2a::cb_ECMINCFG1_ECMIE00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMINCFG1[0]) = str.data & 0xFFF00000; // Disable 0 -> 19
    }
}//}}}

void Cecm_u2a::cb_ECMINCFG2_ECMIE00(RegCBstr str)
{//{{{ 
    if (str.channel == 0) {
        (*ECMINCFG2[0]) = str.data & 0xFFF00000; // Disable 0 -> 19
    }
}//}}}

void Cecm_u2a::cb_ECMINCFG3_ECMIE00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMINCFG3[0]) = str.data & 0xFFF00000; // Disable 0 -> 19
    }
}//}}}

void Cecm_u2a::cb_ECMIRCFG_ECMIRE00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMIRCFG[0]) = str.data & 0xFFFFF800; // Disable 0 -> 10
    }
}//}}}

void Cecm_u2a::cb_ECMEMK_ECMEMK00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMEMK[0]) = str.data & 0xFFF00800;
    }
    IssueErrorOutput(0);
}//}}}

void Cecm_u2a::cb_ECMETMK0_ECMETMK00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMETMK0[0]) = str.data | 0x000FEFFF;
    }
    IssueErrorOutput(1);
}//}}}

void Cecm_u2a::cb_ECMETMK1_ECMETMK00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMETMK1[0]) = str.data | 0x000FDFFF;
    }
    IssueErrorOutput(2);
}//}}} 

void Cecm_u2a::cb_ECMETMK2_ECMETMK00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMETMK2[0]) = str.data | 0x000FBFFF;
    }
    IssueErrorOutput(3);
}//}}} 

void Cecm_u2a::cb_ECMETMK3_ECMETMK00(RegCBstr str)
{//{{{ 
    if (str.channel == 0) {
        (*ECMETMK3[0]) = str.data | 0x000F7FFF;
    }
    IssueErrorOutput(4);
}//}}}


void Cecm_u2a::cb_ECMmESET_ECMmEST(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmESET)["ECMmEST"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }
    //if resstg1z is still ACTIVE
    if (resstg1z.read() == vpcl::emResetActive) {
        re_printf("warning","Cannot write 1 to ECMmESET.ECMmEST while resstg1z is still active.\n");
    //if resstg1z is INACTIVE
    }else{
        //Assert error output signals
        mSyncSignal = emECMErrorOutputActiveLevel;
        mSGATERROZ = emECMErrorOutputActiveLevel;
        mWriteSGATERROZPortEvent.notify();
        mSGATERROUTZ = emECMErrorOutputActiveLevel;
        mWriteSGATERROUTZPortEvent.notify();
        //turn on status of error set trigger
        (*ECMmESSTR[0])["ECMmSSE02"] = 1;
    }
}//}}} 

void Cecm_u2a::cb_ECMmESET0_ECMmEST(RegCBstr str)
{//{{{
     // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmESET0)["ECMmEST"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }
    //if resstg1z is still ACTIVE
    if (resstg1z.read() == vpcl::emResetActive) {
        re_printf("warning","Cannot write 1 to ECMmESET.ECMmEST while resstg1z is still active.\n");
    //if resstg1z is INACTIVE
    }else{
        //Assert error output signals
        mSyncSignalPe[0] = emECMErrorOutputActiveLevel;
        mSGATERROZn[0] = emECMErrorOutputActiveLevel;
        mWriteSGATERROZPortEvent.notify();
//        mSGATERROUTZ = emECMErrorOutputActiveLevel;
//        mWriteSGATERROUTZPortEvent.notify();
        //turn on status of error set trigger
        (*ECMmESSTR[0])["ECMmSSE03"] = 1;
    }
}//}}}

void Cecm_u2a::cb_ECMmESET1_ECMmEST(RegCBstr str)
{//{{{
      // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmESET1)["ECMmEST"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }
    //if resstg1z is still ACTIVE
    if (resstg1z.read() == vpcl::emResetActive) {
        re_printf("warning","Cannot write 1 to ECMmESET.ECMmEST while resstg1z is still active.\n");
    //if resstg1z is INACTIVE
    }else{
        //Assert error output signals
        mSyncSignalPe[1] = emECMErrorOutputActiveLevel;
        mSGATERROZn[1] = emECMErrorOutputActiveLevel;
        mWriteSGATERROZPortEvent.notify();
//        mSGATERROUTZ = emECMErrorOutputActiveLevel;
//        mWriteSGATERROUTZPortEvent.notify();
        //turn on status of error set trigger
        (*ECMmESSTR[0])["ECMmSSE04"] = 1;
    }
    
}//}}}

void Cecm_u2a::cb_ECMmESET2_ECMmEST(RegCBstr str)
{//{{{
      // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmESET2)["ECMmEST"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }
    //if resstg1z is still ACTIVE
    if (resstg1z.read() == vpcl::emResetActive) {
        re_printf("warning","Cannot write 1 to ECMmESET.ECMmEST while resstg1z is still active.\n");
    //if resstg1z is INACTIVE
    }else{
        //Assert error output signals
        mSyncSignalPe[2] = emECMErrorOutputActiveLevel;
        mSGATERROZn[2] = emECMErrorOutputActiveLevel;
        mWriteSGATERROZPortEvent.notify();
//        mSGATERROUTZ = emECMErrorOutputActiveLevel;
//        mWriteSGATERROUTZPortEvent.notify();
        //turn on status of error set trigger
        (*ECMmESSTR[0])["ECMmSSE05"] = 1;
    }
    
}//}}}

void Cecm_u2a::cb_ECMmESET3_ECMmEST(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmESET3)["ECMmEST"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }
    //if resstg1z is still ACTIVE
    if (resstg1z.read() == vpcl::emResetActive) {
        re_printf("warning","Cannot write 1 to ECMmESET.ECMmEST while resstg1z is still active.\n");
    //if resstg1z is INACTIVE
    }else{
        //Assert error output signals
        mSyncSignalPe[3] = emECMErrorOutputActiveLevel;
        mSGATERROZn[3] = emECMErrorOutputActiveLevel;
        mWriteSGATERROZPortEvent.notify();
//        mSGATERROUTZ = emECMErrorOutputActiveLevel;
//        mWriteSGATERROUTZPortEvent.notify();
        //turn on status of error set trigger
        (*ECMmESSTR[0])["ECMmSSE06"] = 1;
    }
}//}}}

void Cecm_u2a::cb_ECMmECLR_ECMmECT(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmECLR)["ECMmECT"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }

    if (errout_clear_mask_in[0]->read()) {
        mProcessECMmECTBitEvent[0].notify(3*mCountClock + 5*mPCLKClock, SC_NS);
    }
}//}}}

void Cecm_u2a::cb_ECMmECLR0_ECMmECT(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmECLR0)["ECMmECT"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }

    if (errout_clear_mask_in[1]->read()) {
        mProcessECMmECTBitEvent[1].notify(3*mCountClock + 5*mPCLKClock, SC_NS);
    }
    
}//}}}

void Cecm_u2a::cb_ECMmECLR1_ECMmECT(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmECLR1)["ECMmECT"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }

    if (errout_clear_mask_in[2]->read()) {
        mProcessECMmECTBitEvent[2].notify(3*mCountClock + 5*mPCLKClock, SC_NS);
    }
    
}//}}}

void Cecm_u2a::cb_ECMmECLR2_ECMmECT(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmECLR2)["ECMmECT"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }

    if (errout_clear_mask_in[3]->read()) {
        mProcessECMmECTBitEvent[3].notify(3*mCountClock + 5*mPCLKClock, SC_NS);
    }
    
}//}}}

void Cecm_u2a::cb_ECMmECLR3_ECMmECT(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmECLR3)["ECMmECT"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }

    if (errout_clear_mask_in[4]->read()) {
        mProcessECMmECTBitEvent[4].notify(3*mCountClock + 5*mPCLKClock, SC_NS);
    }
    
}//}}}

void Cecm_u2a::cb_ECMKCPROT_KCE(RegCBstr str)
{//{{{ 
    // Unlock all protection registers
    if (str.data == 0xA5A5A501){
        mIsLockReg = false;
    // Lock all protection register
    }else if (str.data == 0xA5A5A500){
        mIsLockReg = true;
    } else {
        (*ECMKCPROT)["KCE"] = str.pre_data & 0x1;
    }
    (*ECMKCPROT)["KCPROT"] = 0;
}//}}}

void Cecm_u2a::cb_ECMDTMCTL_DTMSTP(RegCBstr str)
{//{{{
    bool cur_dtmsta = (bool)(str.pre_data & 0x1);
    bool cur_dtmstacntclk = (bool)((str.pre_data >> 4) & 0x1);

    if (cur_dtmstacntclk == cur_dtmsta){
        if (!ECMCheckZeroClock("cntclk")) {
            (*ECMDTMCTL)["DTMSTA"] = cur_dtmsta;
            (*ECMDTMCTL)["DTMSTP"] = 0; // not hold value
            return;
        }
        //STP bit = 1
        if ((str.data & 0x2) == 0x2){
            mDelayTimerStopEvent.notify(3*mCountClock, SC_NS); // wait for 3 cntclk cycles until delay timer is stopped
            (*ECMDTMCTL)["DTMSTA"] = 0;
            (*ECMDTMCTL)["DTMSTP"] = 0; // not hold value
        }else{
            if ((str.data & 0x1) != (str.pre_data & 0x1)) {
                //STA bit = 0
                if ((str.data & 0x1) == 0) {
                    mDelayTimerStopEvent.notify(3*mCountClock, SC_NS); // wait for 3 cntclk cycles until delay timer is stopped
                }
            }
        }
        mUpdateDTMSTACNTCLKBitEvent.notify(mCountClock, SC_NS);
    }else{
        re_printf("warning", "Should write to ECMDTMCTL register while value of DTMSTACNTCLK bit equals to value of DTMSTA bit.");
        (*ECMDTMCTL) = (unsigned int)str.pre_data;// not update written value
    }
}//}}}

void Cecm_u2a::cb_ECMDTMR_ECMDTMR(RegCBstr str)
{//{{{ 
    (*ECMDTMR) = (unsigned int) Cgeneral_timer::getCNT();
}//}}}

void Cecm_u2a::cbDBG_ECMDTMR_ECMDTMR(RegCBstr str)
{//{{{ 
    (*ECMDTMR) = (unsigned int) Cgeneral_timer::getCNT();
}//}}}

void Cecm_u2a::cb_ECMDTMCMP_ECMDTMCMP(RegCBstr str)
{//{{{ 
    if (Cgeneral_timer::getCR().gtSTR) { // delay timer is counting
        re_printf("warning","Cannot write to ECMDTMCMP register when delay timer is counting.\n");
        (*ECMDTMCMP) = str.pre_data;
    }
}//}}}

void Cecm_u2a::cb_ECMDTMCFG_ECMDTME00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMDTMCFG[0]) = str.data & 0xFFF00000;
    }
}//}}}

void Cecm_u2a::cb_ECMDTMCFG0_ECMDTME00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMDTMCFG0[0]) = str.data & 0xFFF00000;
    }
}//}}}

void Cecm_u2a::cb_ECMDTMCFG1_ECMDTME00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMDTMCFG1[0]) = str.data & 0xFFF00000;
    }
}//}}}

void Cecm_u2a::cb_ECMDTMCFG2_ECMDTME00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMDTMCFG2[0]) = str.data & 0xFFF00000;
    }
}//}}}

void Cecm_u2a::cb_ECMDTMCFG3_ECMDTME00(RegCBstr str)
{//{{{
    if (str.channel == 0) {
        (*ECMDTMCFG3[0]) = str.data & 0xFFF00000;
    }
}//}}}

void Cecm_u2a::cb_ECMPE_ECMPE00(RegCBstr str)
{//{{{ 
    //Issue pseudo error according channel of register
    unsigned int start_index = str.channel*32;
    unsigned int end_index = start_index + 32;
    if (str.channel == 0) {
        start_index = emPseudoErrorStartID;
    }
    for(unsigned int error_index = start_index; error_index < end_index; error_index++){
        mPseudoErrorArray[error_index] = (bool)(*(mErrorInputTable[error_index].pe));
    }
    mWritePseudoErrorOutEvent.notify();
    (*ECMPE[str.channel]) = 0x0; //not hold value
}//}}}

void Cecm_u2a::cb_ECMEOCCFG_ECMEOUTCLRT(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock("cntclk")) {
        return;
    }
    if (erroutresz.read() == vpcl::emResetActive) {
        (*ECMEOCCFG) = 0; 
        return;
    }
    if (((unsigned int)(*ECMEOCCFG) != str.pre_data)&&(mHoldSignal[0] == false)) { // write to this register when error output clear invalidation is active
        mEnableClearOutputErrEvent[0].cancel();
        bool errout_clear_invalid_enable = (*ECMEOCCFG)["EOCIEN"];
        if (errout_clear_invalid_enable) { // error output clear invalidation function enabled
            double current_time = GetCurTime();
            double waited_time = current_time - mRestartCounterTime[0];
            double new_wait_time = (double)(mCountClock*((unsigned int)((*ECMEOCCFG)["ECMEOUTCLRT"]) + 2)); // (ECMEOUTCLRT+2) cntclk cycles
            if (new_wait_time <= waited_time) {
                mEnableClearOutputErrEvent[0].notify();  // enable output clear
            } else {
                double additional_wait = new_wait_time - waited_time;
                mEnableClearOutputErrEvent[0].notify(additional_wait, SC_NS);
            }
        } else { // function disabled
            mEnableClearOutputErrEvent[0].notify();  // enable output clear
        }
    }
}//}}}

void Cecm_u2a::cb_ECMETCCFG_ECMEOUTCLRT(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock("cntclk")) {
        return;
    }
    if (erroutresz.read() == vpcl::emResetActive) {
        (*ECMETCCFG[str.channel]) = 0; 
        return;
    }
    if (((unsigned int)(*ECMETCCFG[str.channel]) != str.pre_data)&&(mHoldSignal[str.channel+1] == false)) { // write to this register when error output clear invalidation is active
        mEnableClearOutputErrEvent[str.channel+1].cancel();
        bool errout_clear_invalid_enable = (*ECMETCCFG[str.channel])["EOCIEN"] ;
        if (errout_clear_invalid_enable) { // error output clear invalidation function enabled
            double current_time = GetCurTime();
            double waited_time = current_time - mRestartCounterTime[str.channel + 1];
            double new_wait_time = (double)(mCountClock*((unsigned int)((*ECMETCCFG[str.channel])["ECMEOUTCLRT"]) + 2)); // (ECMEOUTCLRT+2) cntclk cycles
            if (new_wait_time <= waited_time) {
                mEnableClearOutputErrEvent[str.channel+1].notify();  // enable output clear
            } else {
                double additional_wait = new_wait_time - waited_time;
                mEnableClearOutputErrEvent[str.channel+1].notify(additional_wait, SC_NS);
            }
        } else { // function disabled
            mEnableClearOutputErrEvent[str.channel+1].notify();  // enable output clear
        }
    }
}//}}}

void Cecm_u2a::cb_ECMPEM_MSKC(RegCBstr str)
{//{{{
    mWriteCompareMaskEvent.notify();
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit SC_NS)
**********************************/
double Cecm_u2a::GetCurTime(void)
{//{{{
    double cur_time = sc_time_stamp().to_double();
    double ret = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {//SC_FS
        ret = cur_time/1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {//SC_PS
        ret = cur_time/1000;
    }  else if (sc_get_time_resolution() < sc_time(1, SC_US)) {//SC_NS
        ret = cur_time;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {//SC_US
        ret = cur_time * 1000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {//SC_MS
        ret = cur_time * 1000000;
    } else {//SC_SEC
        ret = cur_time * 1000000000;
    }
    return ret;
}//}}}

bool Cecm_u2a::CheckSpecialError(unsigned int error_index, eListSpecialError startID, eListSpecialError endID)
{//{{{
    for (unsigned int i = (unsigned int)startID; i < (unsigned int)endID ; i++) {
        if (error_index == mListSpecialError[i]) {
            return true;
        }
    }
    return false;
}//}}}

//===============Cecm_u2a_wp class=======================
//Constructor of Cecm_u2a_wp class
Cecm_u2a_wp::Cecm_u2a_wp(sc_module_name name):
         vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 3> (name)
         //sgaterrin (N = 000 .. 343): in this, 0-307 in constructor
         ,master_sgaterrin020_sig("master_sgaterrin020_sig") 
         ,checker_sgaterrin020_sig("checker_sgaterrin020_sig")  
         ,master_sgaterroz_sig("master_sgaterroz_sig")
         ,master_sgatresz_sig("master_sgatresz_sig")
         ,checker_sgaterroz_sig("checker_sgaterroz_sig")
         ,checker_sgatresz_sig("checker_sgatresz_sig")
         ,master_ecmterrin020msk_m("master_ecmterrin020msk_m")
         ,master_ecmterrin020msk_c("master_ecmterrin020msk_c")
         ,checker_ecmterrin020msk_m("checker_ecmterrin020msk_m")
         ,checker_ecmterrin020msk_c("checker_ecmterrin020msk_c")
         ,checker_trgout_sig("checker_trgout_sig")
         ,master_trgout_sig("master_trgout_sig")
         ,dtmtrgout_sig("dtmtrgout_sig")
         ,pclk("pclk")
         ,cntclk("cntclk")
         ,preset_n("preset_n")
         ,cntclk_preset_n("cntclk_preset_n")
         ,erroutresz("erroutresz")
         ,cntclk_erroutresz("cntclk_erroutresz")
         ,resstg1z("resstg1z")
         ,pclkin_resstg1z("pclkin_resstg1z")
         ,ecmterrlbz_m("ecmterrlbz_m")
         ,ecmterrlbz_c("ecmterrlbz_c")
         ,ecmttin("ecmttin")
         ,errin_mask_reset("errin_mask_reset")
         ,ecmterroz("ecmterroz")
         ,ecmterroutz("ecmterroutz")
         ,ecmtresz("ecmtresz")
         ,ecmterroutz_m("ecmterroutz_m")
         ,ecmterroutz_c("ecmterroutz_c")
         ,TARGET_PRODUCT("TARGET_PRODUCT","")
{//{{{
    CommandInit(this->name());
    //Name ports/signals
    for(unsigned int index = 0; index < emNumErrInPort; index++){
        std::ostringstream str_ecmterrin;

        str_ecmterrin<<"ecmterrin"<<index;
        ecmterrin[index] = new sc_in<bool>(str_ecmterrin.str().c_str());
    }
    for(unsigned int index = 0; index < emNumErrorFactor; index++){
        std::ostringstream str_sgaterrin_sig;
        str_sgaterrin_sig<<"sgaterrin_sig"<<index;
        sgaterrin_sig[index] = new sc_signal<bool>(str_sgaterrin_sig.str().c_str());

        *sgaterrin_sig[index] = 0;
        std::ostringstream str_master_sgatpe_sig;
        std::ostringstream str_checker_sgatpe_sig;

        str_master_sgatpe_sig<<"master_sgatpe_sig"<<index;
        str_checker_sgatpe_sig<<"checker_sgatpe_sig"<<index;
        master_sgatpe_sig[index]  = new sc_signal<bool>(str_master_sgatpe_sig.str().c_str());
        checker_sgatpe_sig[index] = new sc_signal<bool>(str_checker_sgatpe_sig.str().c_str());

        *master_sgatpe_sig[index] = 0;
        *checker_sgatpe_sig[index] = 0;
    }

    for(unsigned int index = 0; index <= emNumPE; index++) {
        std::ostringstream str_errout_clear_mask_sig;
        std::ostringstream str_errout_clear_mask_checker;
        str_errout_clear_mask_sig       << "errout_clear_mask_sig" << index;
        str_errout_clear_mask_checker   << "errout_clear_mask_checker" << index;
        errout_clear_mask_sig[index]     = new sc_signal<bool>(str_errout_clear_mask_sig.str().c_str());
        errout_clear_mask_checker[index] = new sc_signal<bool>(str_errout_clear_mask_checker.str().c_str());
    }

    for(unsigned int index = 0; index < emNumPE; index++){
        std::ostringstream str_master_dclsint_sig;
        std::ostringstream str_checker_dclsint_sig;
        std::ostringstream str_master_sgati_pe_sig;
        std::ostringstream str_checker_sgati_pe_sig;
        std::ostringstream str_ecmti_pe;
        std::ostringstream str_ecmtnmi;
        std::ostringstream str_ecmdclsint;
        std::ostringstream str_master_sgatnmi_sig;
        std::ostringstream str_checker_sgatnmi_sig;
        std::ostringstream str_ozN;
        std::ostringstream str_master_sgaterroz_sig ;
        std::ostringstream str_checker_sgaterroz_sig ;

        std::ostringstream str_master_sgaterroz_sig_n;
        std::ostringstream str_checker_sgaterroz_sig_n;
        str_ecmti_pe<<"ecmti_pe"<<index;
        str_ecmtnmi<<"ecmtnmi"<<index;
        str_ecmdclsint<<"ecmdclsint"<<index;
        str_master_dclsint_sig<<"master_dclsint_sig"<<index;
        str_checker_dclsint_sig<<"checker_dclsint_sig"<<index;
        str_master_sgati_pe_sig<<"master_sgati_pe_sig"<<index;
        str_checker_sgati_pe_sig<<"checker_sgati_pe_sig"<<index;
        str_master_sgatnmi_sig<<"master_sgatnmi_sig"<<index;
        str_checker_sgatnmi_sig<<"checker_sgatnmi_sig"<<index;
        str_ozN << "ecmterroz"<<index;
        str_master_sgaterroz_sig << "master_sgaterroz_sig_n" << index;
        str_checker_sgaterroz_sig << "checker_sgaterroz_sig_n" << index;

        ecmti_pe[index] = new sc_out<bool>(str_ecmti_pe.str().c_str());
        ecmtnmi[index] = new sc_out<bool>(str_ecmtnmi.str().c_str());
        ecmdclsint[index] = new sc_out<bool>(str_ecmdclsint.str().c_str());
        master_dclsint_sig[index] = new sc_signal<bool>(str_master_dclsint_sig.str().c_str());
        checker_dclsint_sig[index] = new sc_signal<bool>(str_checker_dclsint_sig.str().c_str());
        master_sgati_pe_sig[index] = new sc_signal<bool>(str_master_sgati_pe_sig.str().c_str());
        checker_sgati_pe_sig[index] = new sc_signal<bool>(str_checker_sgati_pe_sig.str().c_str());
        master_sgatnmi_sig[index] = new sc_signal<bool>(str_master_sgatnmi_sig.str().c_str());
        checker_sgatnmi_sig[index]= new sc_signal<bool>(str_checker_sgatnmi_sig.str().c_str());
        ecmterroz_pe[index]         = new sc_out<bool>(str_ozN.str().c_str());
        master_sgaterroz_sig_n[index] = new sc_signal<bool>(str_master_sgaterroz_sig.str().c_str());
        checker_sgaterroz_sig_n[index]= new sc_signal<bool>(str_checker_sgaterroz_sig.str().c_str());

        ecmti_pe[index]->initialize(0);
        ecmtnmi[index]->initialize(0);
        ecmdclsint[index]->initialize(0);
        ecmterroz_pe[index]->initialize(0);
        *master_dclsint_sig[index] = 0;
        *checker_dclsint_sig[index] = 0;
        *master_sgati_pe_sig[index] = 0;
        *checker_sgati_pe_sig[index] = 0;
        *master_sgatnmi_sig[index] = 0;
        *checker_sgatnmi_sig[index] = 0;
        *master_sgaterroz_sig_n[index] = 0;
        *checker_sgaterroz_sig_n[index] = 0;
    }


    mMaster = new Cecm_u2a ("mMaster", this);
    sc_assert(mMaster != NULL);
    mChecker = new Cecm_u2a ("mChecker", this);
    sc_assert(mChecker != NULL);

    //connect port
    ConnectPort();

    //Initialize internal variables and output port
    mStateArray[emIdleState]                    = "IDLE";
    mStateArray[emErrorInputProcessingState]    = "ERROR INPUT PROCESSING";
    mStateArray[emECMProcessingState]           = "ECM PROCESSING";
    mStateArray[emOutputSignalCombinationState] = "OUTPUT SIGNAL COMBINATION";
    mStateArray[emResettingState]               = "RESETTING";
     
    mIsInitialize           = true;
    mCmdReset               = false;
    mIsCmdResetStatus       = false;
    mPortReset              = false;
    mResetPeriod            = 0;
    mState                  = emIdleState;
    for(unsigned int i = 0; i < emNumPE; i++){
        mECMTI_PE[i]    = false;
        mECMDCLSINT[i]  = false;
        mECMTNMI[i]     = false;
        mECMTERROZn[i]  = false;
    }
    mECMTRESZ               = true;
    mECMTERROZ              = false;
    mFreqPCLK               = 0;
    mFreqCNTCLK             = 0;

    InitializeInternalSignal();
    //Initialize output port
    ecmterroz.initialize(false);
    ecmterroutz.initialize(false);
    ecmtresz.initialize(true);
    ecmterroutz_m.initialize(false);
    ecmterroutz_c.initialize(false);

    //Initialize mExternalErrorInputTable
    ExternalErrorInputTableConstruction();

    SC_METHOD(ReceiveExternalErrorMethod)
    dont_initialize();
    //sensitive <<  ecmterrinN (N = 000 .. 307) error input
    for(unsigned int i = 0; i < emNumErrInPort; i++){
        sensitive <<  (*ecmterrin[i]);
    }
    //sensitive <<  sgatpeN (N = 011 .. 343), not sensitive factor 0 -> 10 because no pseudo-error
    for(unsigned int i = emPseudoErrorStartID; i < emNumErrorFactor; i++){
        sensitive <<  (*master_sgatpe_sig[i]);
    }

    sensitive << mReceiveExternalErrorEvent;

    SC_METHOD(ResetMethod)
    dont_initialize();
    sensitive << preset_n;

    SC_METHOD(CmdResetMethod)
    dont_initialize();
    sensitive << mCmdResetEvent;

    SC_METHOD(CancelCmdResetMethod)
    dont_initialize();
    sensitive << mCancelCmdResetEvent;

    SC_METHOD(CounterPeriodMethod)
    dont_initialize();
    sensitive << cntclk;

    SC_METHOD(PCLKPeriodMethod)
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(CombineSignalMethod)
    dont_initialize();
    sensitive << master_sgaterroz_sig;
    for(unsigned int i = 0; i < emNumPE; i++){
        sensitive << (*master_sgaterroz_sig_n[i]);
        sensitive << (*checker_sgaterroz_sig_n[i]);
    }
    sensitive << ecmterroutz_m;
    sensitive << master_sgatresz_sig;
    sensitive << checker_sgaterroz_sig;
    sensitive << ecmterroutz_c;
    sensitive << checker_sgatresz_sig;
    sensitive << checker_trgout_sig;
    sensitive << master_trgout_sig;
    sensitive << mMaster->mCombineSignalEvent;
    sensitive << mChecker->mCombineSignalEvent;
    for(unsigned int i = 0; i < emNumPE; i++){
        sensitive << (*master_sgati_pe_sig[i]);
        sensitive << (*master_sgatnmi_sig[i]);
        sensitive << (*master_dclsint_sig[i]);
        sensitive << (*checker_sgati_pe_sig[i]);
        sensitive << (*checker_sgatnmi_sig[i]);
        sensitive << (*checker_dclsint_sig[i]);
    }

    SC_METHOD(WriteOutputPortMethod)
    dont_initialize();
    sensitive << mWriteOutputPortEvent;
    
    SC_METHOD(ComparatorTestMethod)
    dont_initialize();
    sensitive << (*master_sgatpe_sig[emCompareErrorId]);
    sensitive << mComparatorTestEvent;
    sensitive << master_ecmterrin020msk_m;
    sensitive << master_ecmterrin020msk_c;


    #ifdef USE_CWR_SYSTEMC 
    //handle command
    SCML_COMMAND_PROCESSOR(handleCommand);
    SCML_ADD_COMMAND("tgt", 1, 8, "tgt <cmd> <arg0> <arg1> <arg2> ...", "prefix of tlm target if command");
    SCML_ADD_COMMAND("command", 1, 3, "command <param> <arg> <1/->", "prefix of setting module parameters");
    SCML_ADD_COMMAND("reg", 1, 8, "reg <cmd> <arg0> <arg1> <arg2> ...", "prefix of register command");
    #endif
    
    for(unsigned int id = 0; id < emMaxId; id++){
        SetLatency_TLM(id, 0, true);
    }

}//}}}

Cecm_u2a_wp::~Cecm_u2a_wp()
{//{{{
    delete mMaster;
    delete mChecker;
}//}}}

std::string Cecm_u2a_wp::GetMasterCheckerName(const bool is_master) const
{//{{{
    if (is_master){
        return (std::string)mMaster->name();
    }else{
        return (std::string)mChecker->name();
    }
}//}}}

void Cecm_u2a_wp::ReceiveExternalErrorMethod(void)
{//{{{
    if ((mPortReset || mCmdReset) && errin_mask_reset){
        return;
    }

    UpdateState(mState, emErrorInputProcessingState);
    for(unsigned int i = 0; i < emNumErrorFactor; i++){
        if (i < emPseudoErrorStartID) { // error num 0->10
            sgaterrin_sig[i]->write(0);
        } else if (i > emCompareErrorId) { // error num 21->351
            if ((emSoftwareAlarm <= i) && (i <= (emSoftwareAlarm + 3))) { // error num 192->195
                sgaterrin_sig[i]->write(master_sgatpe_sig[i]->read()); // only pseudo connected
            } else {
                sgaterrin_sig[i]->write(ecmterrin[i-21]->read() | master_sgatpe_sig[i]->read());
            }
        } else if (i < emCompareErrorId) { // error num 11
            sgaterrin_sig[i]->write(master_sgatpe_sig[i]->read()); // only pseudo connected
        }
    }
    DumpEnableTransInfo();
    
    UpdateState(mState, emECMProcessingState);
}//}}} 

void Cecm_u2a_wp::ResetMethod(void)
{//{{{
    bool is_cancel_assertreset = false;
    if(preset_n.read() == vpcl::emResetActive){
        mPortReset = true;
    }else{
        mPortReset = false;
    }

    if (mPortReset){
        mIsInitialize = false;
        re_printf("info","Reset signal is asserted.\n");
        this->EnableReset(mPortReset);
        is_cancel_assertreset = true;
    }else{
        if(mIsInitialize){
            mIsInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info","Reset signal is negated.\n");
            this->EnableReset(mPortReset);
            is_cancel_assertreset = true;
        }
    }

    //Cancel handleCommand reset when preset_n is asserted
    if (is_cancel_assertreset){
        mCmdResetEvent.cancel();
        mCancelCmdResetEvent.cancel();
        mIsCmdResetStatus = false;
        mCmdReset = false;
    }
}//}}}

void Cecm_u2a_wp::AssertReset( double delay, double period)
{//{{{
    //If preset_n is negated, ECM_WP waits delay time then notifies handleCommand reset event
    if ((!mPortReset)&&(!mIsCmdResetStatus)){
        mIsCmdResetStatus = true;
        re_printf("info","ECM_WP will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long)period, FRACTION_TO_INT(period), (unsigned long long) delay, FRACTION_TO_INT(delay));
        mCmdResetEvent.notify(delay, SC_NS);
        mResetPeriod = period;
    }else{
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Cecm_u2a_wp::CmdResetMethod(void)
{//{{{
    mCmdReset = true;
    this->EnableReset(mCmdReset);
    mCancelCmdResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Cecm_u2a_wp::CancelCmdResetMethod(void)
{//{{{
    mCmdReset = false;
    mIsCmdResetStatus = false;
    this->EnableReset(mCmdReset);
}//}}}

void Cecm_u2a_wp::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        //Change status of ECM_WP to resetting
        UpdateState(mState, emResettingState);

        //cancel events
        mWriteOutputPortEvent.cancel();
        mReceiveExternalErrorEvent.cancel();
    }else{
        //Change status of ECM_WP from resetting to idle
        UpdateState(mState, emIdleState);
        //Receive the input signal after reset
        mReceiveExternalErrorEvent.notify();
    }
    //Call the reset function of common/general class
    mMaster->ECMEnableReset(is_active);
    mChecker->ECMEnableReset(is_active);
    if(mCmdReset == false){  // if reset by AssertReset command, tgt if isn't initialize.
        this->tgt_enable_reset(is_active);
    }
}//}}}

void Cecm_u2a_wp::PCLKPeriodMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Cecm_u2a_wp::CounterPeriodMethod(void)
{//{{{
    SetCLKfreq("cntclk", sc_dt::uint64_to_double(cntclk.read()));
}//}}}

void Cecm_u2a_wp::CombineSignalMethod(void)
{//{{{
    UpdateState(mState, emOutputSignalCombinationState);
    // ecmdclsint1..8
    bool ecmdclsint_array[emNumPE];
    //interrupt (maskable and non-maskable)
    bool ecmti_pe_array[emNumPE];
    bool ecmtnmi_value[emNumPE];
    bool ecmdclsint_value[emNumPE];

    for(unsigned int i = 0; i < emNumPE; i++){
        ecmdclsint_array[i] = (master_dclsint_sig[i]->read() | checker_dclsint_sig[i]->read());
        ecmti_pe_array[i] = (master_sgati_pe_sig[i]->read() | checker_sgati_pe_sig[i]->read());
        ecmtnmi_value[i] = (master_sgatnmi_sig[i]->read()) | (checker_sgatnmi_sig[i]->read());
        ecmdclsint_value[i] = (master_dclsint_sig[i]->read()) | (checker_dclsint_sig[i]->read());
    }
    if (DumpInterrupt){
        for(unsigned int i=0; i < emNumPE; i++){
            if (mECMTI_PE[i] != ecmti_pe_array[i]){
                if (ecmti_pe_array[i]){
                    re_printf("info","INT [%s: ECMTI_PE%d] Assert.\n", this->basename(), i+1);
                }else{
                    re_printf("info","INT [%s: ECMTI_PE%d] De-assert.\n", this->basename(), i+1);
                }
            }
            if (mECMTNMI[i] != ecmtnmi_value[i]){
                if (ecmtnmi_value[i]){
                    re_printf("info","INT [%s: ECMTNMI%d] Assert.\n", this->basename(), i+1);
                }else{
                    re_printf("info","INT [%s: ECMTNMI%d] De-assert.\n", this->basename(), i+1);
                }
            }
            if (mECMDCLSINT[i] != ecmdclsint_value[i]){
                if (ecmdclsint_value[i]){
                    re_printf("info","INT [%s: ECMDCLSINT%d] Assert.\n", this->basename(), i+1);
                }else{
                    re_printf("info","INT [%s: ECMDCLSINT%d] De-assert.\n", this->basename(), i+1);
                }
            }
        }
    }
    for(unsigned int i= 0; i < emNumPE; i++){
        mECMDCLSINT[i] = ecmdclsint_array[i];
        mECMTI_PE[i] = ecmti_pe_array[i];
        mECMTNMI[i] = ecmtnmi_value[i];
        mECMTERROZn[i] = (master_sgaterroz_sig_n[i]->read()) & (checker_sgaterroz_sig_n[i]->read());
    }
    //error output
    mECMTERROZ = (master_sgaterroz_sig.read()) & (checker_sgaterroz_sig.read());
    // DCLS INT
    //safety reset
    mECMTRESZ = (master_sgatresz_sig.read()) & (checker_sgatresz_sig.read());

    //assign value to dtmtrgin of Master and Checker
    dtmtrgout_sig.write((master_trgout_sig.read()) | (checker_trgout_sig.read()));

    mSgoterrin = false;
    if ((master_sgaterroz_sig.read()) != (checker_sgaterroz_sig.read())){
        mSgoterrin = true;
        re_printf("warning","The error output signals from Master and Checker are different.\n");
    }
    for (unsigned int i = 0; i < emNumPE; i++) {
        if ((master_sgaterroz_sig_n[i]->read()) != (checker_sgaterroz_sig_n[i]->read())){
            mSgoterrin = true;
            re_printf("warning","The error output signals from Master and Checker of PE%d are different.\n", i);
        }
    }
    mWriteOutputPortEvent.notify();
    mComparatorTestEvent.notify();
}//}}}

void Cecm_u2a_wp::ComparatorTestMethod(void)
{//{{{
    master_sgaterrin020_sig.write ((mSgoterrin || master_sgatpe_sig[emCompareErrorId]->read()) && !master_ecmterrin020msk_m.read());
    checker_sgaterrin020_sig.write((mSgoterrin || master_sgatpe_sig[emCompareErrorId]->read()) && !master_ecmterrin020msk_c.read()); 
}//}}}

void Cecm_u2a_wp::WriteOutputPortMethod(void)
{//{{{ 
    ecmterroz.write(mECMTERROZ);
    ecmterroutz.write(ecmterroutz_m.read() & ecmterroutz_c.read());
    ecmtresz.write(mECMTRESZ);
    for(unsigned int i= 0; i < emNumPE; i++){
        ecmti_pe[i]->write(mECMTI_PE[i]);
        ecmdclsint[i]->write(mECMDCLSINT[i]);
        ecmtnmi[i]->write(mECMTNMI[i]);
        ecmterroz_pe[i]->write(mECMTERROZn[i]);
    }
}//}}}

void Cecm_u2a_wp::ExternalErrorInputTableConstruction(void)
{//{{{
    for(unsigned int i = 0; i < emNumErrInPort; i++){
        std::ostringstream str_ecmterrin;
        str_ecmterrin<<"ecmterrin"<<i;
        mExternalErrorInputTable.push_back(ExternalErrorInputElement(ecmterrin[i],str_ecmterrin.str(),false));
    }
}//}}}

void Cecm_u2a_wp::DumpStatInfo(void)
{//{{{
    const char *type = "StatInfo";
    DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
    DumpInfo(type, "  ECMTERROUTZ: %d\n", ecmterroutz.read());
    DumpInfo(type, "  ECMTERROUTZ_M: %d\n", ecmterroutz_m.read());
    DumpInfo(type, "  ECMTERROUTZ_C: %d\n", ecmterroutz_c.read());
    DumpInfo(type, "  ECMTERROZ: %d\n", ecmterroz.read());
    for(unsigned int i = 0; i < emNumPE; i++){
        DumpInfo(type, "  ECMTI_PE%d: %d\n", i, ecmti_pe[i]->read());
        DumpInfo(type, "  ECMTNMI%d: %d\n", i, ecmtnmi[i]->read());
        DumpInfo(type, "  ECMTERROR_PE%d: %d\n", i, ecmterroz_pe[i]->read());
    }
    DumpInfo(type, "  ECMTRESZ: %d\n", ecmtresz.read());
    for(unsigned int i = 0; i < emNumPE; i++){
        DumpInfo(type, "  ECMDCLSINT%d: %d\n", i, ecmdclsint[i]->read());
    }
    DumpInfo(type, "EndInfo.\n");
}//}}}

void Cecm_u2a_wp::DumpEnableTransInfo()
{//{{{
    if (EnableTransInfo){
        const char *type = "TransInfo";
        DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
        bool error_value = false;
        //check whether external error input changes or not
        for(unsigned int error_index = 0; error_index < mExternalErrorInputTable.size(); error_index++){
            error_value = mExternalErrorInputTable[error_index].external_error->read();
            if (error_value != mExternalErrorInputTable[error_index].pre_value){
                DumpInfo(type, "  [%-22s] [%d -> %d]\n", mExternalErrorInputTable[error_index].external_error->basename(),
                mExternalErrorInputTable[error_index].pre_value, error_value);

                //update pre_value for pheripheral error inputs
                mExternalErrorInputTable[error_index].pre_value = error_value;
            }
        }
        DumpInfo(type, "EndInfo.\n");
    }
}//}}}

void Cecm_u2a_wp::UpdateState(eState prev_state, eState next_state)
{//{{{
    if (prev_state != next_state){
        re_printf("info","ECM_WP state is changed from %s state to %s state.\n", mStateArray[prev_state].c_str(), mStateArray[next_state].c_str());
        mState = next_state;
    }
}//}}}

void Cecm_u2a_wp::ConnectPort()
{//{{{
    mMaster->erroutresz(erroutresz);
    mChecker->erroutresz(erroutresz);
    mMaster->sgaterroz(master_sgaterroz_sig);
    mMaster->sgatresz(master_sgatresz_sig);
    mChecker->sgaterroz(checker_sgaterroz_sig);
    mChecker->sgatresz(checker_sgatresz_sig);
    mMaster->sgaterroutz(this->ecmterroutz_m);
    mChecker->sgaterroutz(this->ecmterroutz_c);

    mChecker->dtmtrgout(checker_trgout_sig);
    mMaster->dtmtrgout(master_trgout_sig);
    mMaster->dtmtrgin(dtmtrgout_sig);
    mChecker->dtmtrgin(dtmtrgout_sig);
    mMaster->pseudo_compare_msk_m(master_ecmterrin020msk_m);
    mMaster->pseudo_compare_msk_c(master_ecmterrin020msk_c);
    mChecker->pseudo_compare_msk_m(checker_ecmterrin020msk_m);
    mChecker->pseudo_compare_msk_c(checker_ecmterrin020msk_c);

    mMaster->sgattin(ecmttin);
    mChecker->sgattin(ecmttin);
    mMaster->sgaterrlbz(ecmterrlbz_m);
    mChecker->sgaterrlbz(ecmterrlbz_c);
    mMaster->resstg1z(resstg1z);
    mChecker->resstg1z(resstg1z);
    mMaster->resstg1z_pclkin(pclkin_resstg1z);
    mChecker->resstg1z_pclkin(pclkin_resstg1z);
    // connect mMaster->sgaterrinN(sgaterrinN_sig) (N = 00..343)
    for(unsigned int i = 0; i < emNumErrorFactor; i++){
        if (i != emCompareErrorId) 
            mMaster->sgaterrin[i]->bind(*sgaterrin_sig[i]);
        else 
            mMaster->sgaterrin[i]->bind(master_sgaterrin020_sig);
    }

    // connect mChecker->sgaterrinN(sgaterrinN_sig) (N = 00..343)
    for(unsigned int i = 0; i < emNumErrorFactor; i++){
        if (i != emCompareErrorId)
            mChecker->sgaterrin[i]->bind(*sgaterrin_sig[i]);
        else 
            mChecker->sgaterrin[i]->bind(checker_sgaterrin020_sig);
    }
    mMaster->errin_mask_reset(errin_mask_reset);
    mChecker->errin_mask_reset(errin_mask_reset);

    //connect mMaster->sgatpeN(master_sgatpeN_sig) (N = 000.. 343)
    for(unsigned int i = 0; i < emNumErrorFactor; i++){
        mMaster->sgatpe[i]->bind(*master_sgatpe_sig[i]);
    }

    //connect mChecker->sgatpeN(checker_sgatpeN_sig) (N = 000.. 343)
    for(unsigned int i = 0; i < emNumErrorFactor; i++){
        mChecker->sgatpe[i]->bind(*checker_sgatpe_sig[i]);
    }

    // Connect dclsint, ecmti_pe 
    for(unsigned int i = 0; i < emNumPE; i++){
        mMaster->errout_pe[i]->bind(*master_dclsint_sig[i]);
        mChecker->errout_pe[i]->bind(*checker_dclsint_sig[i]);
        mMaster->sgati_pe[i]->bind(*master_sgati_pe_sig[i]);
        mChecker->sgati_pe[i]->bind(*checker_sgati_pe_sig[i]);
        mMaster->sgatnmi[i]->bind(*master_sgatnmi_sig[i]);
        mChecker->sgatnmi[i]->bind(*checker_sgatnmi_sig[i]);
        mMaster->sgaterroz_n[i]->bind(*master_sgaterroz_sig_n[i]);
        mChecker->sgaterroz_n[i]->bind(*checker_sgaterroz_sig_n[i]);
    }
    for(unsigned int i = 0; i <= emNumPE; i++){
        mMaster->errout_clear_mask_out[i]->bind(*errout_clear_mask_sig[i]);
        mMaster->errout_clear_mask_in[i]->bind(*errout_clear_mask_sig[i]);
        mChecker->errout_clear_mask_in[i]->bind(*errout_clear_mask_sig[i]);
        mChecker->errout_clear_mask_out[i]->bind(*errout_clear_mask_checker[i]);
    }
}//}}}

void Cecm_u2a_wp::InitializeInternalSignal()
{//{{{
    mSgoterrin               = false;
    master_sgaterrin020_sig  = 0;
    checker_sgaterrin020_sig = 0;

    master_sgaterroz_sig    = false;
    for (unsigned i = 0; i < emNumPE; i++) {
        *master_sgaterroz_sig_n[i]    = false;
        *checker_sgaterroz_sig_n[i]   = false;
    }
//    master_sgatnmi_sig      = false;
    master_sgatresz_sig     = true;
    checker_sgaterroz_sig   = false;
//    checker_sgatnmi_sig     = false;
    checker_sgatresz_sig    = true;
    for (unsigned i = 0; i <= emNumPE; i++) {
        *errout_clear_mask_sig[i]   = true;
        *errout_clear_mask_checker[i] = true;
    }
    dtmtrgout_sig = false;
}//}}}

void Cecm_u2a_wp::DumpInfo(const char *type, const char *message,...)
{//{{{
    sc_assert((message != NULL) && (type != NULL));
    printf ("PROFILE(%s): ECM_WP: ", type);
    //Print body
    va_list argptr;
    va_start(argptr, message);
    vprintf(message, argptr);
    va_end(argptr);
}//}}}

void Cecm_u2a_wp::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr    = 0;
    unsigned char *p_data  = NULL;
    unsigned int size = 0; 
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if(!status){
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    //Read access
    if (command == tlm::TLM_READ_COMMAND) {
        if (id == emCommonId) {
            memset(p_data, 0, size);
            status = mMaster->ECM_common_tgt_rd(false, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
        } else if (id == emMasterId) {
            memset(p_data, 0, size);
            status = mMaster->ECM_common_tgt_rd(false, (unsigned int)addr, p_data, size);
            if ((((unsigned int)addr & 0xFF) == 0x00) || (((unsigned int)addr & 0xFF) == 0x04)) {
                for(unsigned int index = 1; index < size; index++) {
                    p_data[index] = 0;
                }
            }
        } else {
            memset(p_data, 0, size);
            status = mChecker->ECM_common_tgt_rd(false, (unsigned int)addr, p_data, size);
            if ((((unsigned int)addr & 0xFF) == 0x00) || (((unsigned int)addr & 0xFF) == 0x04)) {
                for(unsigned int index = 1; index < size; index++) {
                    p_data[index] = 0;
                }
            }
        }
    //Write access    
    } else {
        if(id == emCommonId){
            bool temp1 = mMaster->ECM_common_tgt_wr(false, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            bool temp2 = mChecker->ECM_common_tgt_wr(false, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            status = temp1 && temp2;
        } else if (id == emMasterId){
            status = mMaster->ECM_common_tgt_wr(false, (unsigned int)addr, p_data, size);
        } else {
            status = mChecker->ECM_common_tgt_wr(false, (unsigned int)addr, p_data, size);
        }  
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE); 
}//}}}

unsigned int Cecm_u2a_wp::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    //Getinformation
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    //Read access
    if (command == tlm::TLM_READ_COMMAND) {
        if(id == emCommonId){
            memset(p_data, 0, size);
            status = mMaster->ECM_common_tgt_rd(true, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
        }else if(id == emMasterId){
            memset(p_data, 0, size);
            status = mMaster->ECM_common_tgt_rd(true, (unsigned int)addr, p_data, size);
            if ((((unsigned int)addr & 0xFF) == 0x00) || (((unsigned int)addr & 0xFF) == 0x04)) {
                for(unsigned int index = 1; index < size; index++) {
                    p_data[index] = 0;
                }
            }
        }else{
            memset(p_data, 0, size);
            status = mChecker->ECM_common_tgt_rd(true, (unsigned int)addr, p_data, size);
            if ((((unsigned int)addr & 0xFF) == 0x00) || (((unsigned int)addr & 0xFF) == 0x04)) {
                for(unsigned int index = 1; index < size; index++) {
                    p_data[index] = 0;
                }
            }
        }
    //Write access    
    } else {
        if (id == emCommonId) {
            bool temp1 = mMaster->ECM_common_tgt_wr(true, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            bool temp2 = mChecker->ECM_common_tgt_wr(true, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            status = temp1 && temp2;
        } else if (id == emMasterId) {
            status = mMaster->ECM_common_tgt_wr(true, (unsigned int)addr, p_data, size);
        } else {
            status = mChecker->ECM_common_tgt_wr(true, (unsigned int)addr, p_data, size);
        }    
    }
    
    trans.set_response_status(tlm::TLM_OK_RESPONSE); 
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

double Cecm_u2a_wp::GetTimeResolution(void)
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)){
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

void Cecm_u2a_wp::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{  
    if ((clk_name != "pclk")&&(clk_name != "cntclk")) {
        re_printf("error","Clock name is invalid.\n");
        return;
    }
    
    double clk_period = 0 ;
    if(clk_freq <= 0){
        //cancel all processes of ecm_wp
        mWriteOutputPortEvent.cancel();
        mReceiveExternalErrorEvent.cancel();

        //cancel processes of ecm
        mMaster ->ECMSetZeroClock(clk_name);
        mChecker->ECMSetZeroClock(clk_name); 
    } else {
        clk_period = (double)(emNanoSecond / clk_freq);
        double time_unit = (double)(emNanoSecond / GetTimeResolution());
        if(time_unit > clk_period){
            re_printf("warning","The %s period is less than 1 unit time of system.\n", clk_name.c_str());
            return;
        }
    }
    
    if (clk_name == "pclk") {
        mFreqPCLK  = clk_freq;
        mMaster ->mPCLKClock = clk_period;
        mChecker->mPCLKClock = clk_period;
        for(unsigned int id = 0; id < emMaxId; id++){
            SetLatency_TLM(id, clk_period, false);
        }
    } else { //cntclk clock
        mFreqCNTCLK = clk_freq;
        mMaster ->mCountClock = clk_period;
        mChecker->mCountClock = clk_period;
    }
   
    re_printf("info","The %s is set with a frequency as %lld.%06d Hz (%lld.%06d ns)\n",clk_name.c_str(), (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq), (unsigned long long) clk_period, FRACTION_TO_INT(clk_period) );

}//}}}

void Cecm_u2a_wp::SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 3>::tgt_get_param(id);
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 3>::tgt_set_param(id, tgt_param);
}//}}}

// vim600: set foldmethod=marker :
