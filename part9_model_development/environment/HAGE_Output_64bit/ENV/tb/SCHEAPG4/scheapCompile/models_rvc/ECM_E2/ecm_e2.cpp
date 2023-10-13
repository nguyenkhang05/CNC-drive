// ----------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2015 Renesas System Design Co., Ltd.
// Copyright(c) 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#include "ecm_e2.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//===============Cecm_e2 class=======================
//Constructor of Cecm_e2 class
Cecm_e2::Cecm_e2(sc_module_name name, Cecm_e2_wp* wrapper):  
     Cecm_e2_regif((std::string)name,32)
    ,Cgeneral_timer(name)
    ,erroutresz("erroutresz")
    ,sgaterrlbz("sgaterrlbz")
    ,sgattin("sgattin")
    ,dtmtrgin("dtmtrgin")
    ,resstg1z("resstg1z")
    ,resstg1z_pclkin("resstg1z_pclkin")
    ,erroutresz_for_sync("erroutresz_for_sync")
    ,errout_clear_mask_in("errout_clear_mask_in")
    ,sgaterroz("sgaterroz")
    ,sgaterroutz("sgaterroutz")
    ,sgatnmi("sgatnmi")
    ,sgatresz("sgatresz")
    ,dtmtrgout("dtmtrgout")
    ,errout_clear_mask_out("errout_clear_mask_out")
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
    for(unsigned int index = 0; index < emNumPE; index++){
        std::ostringstream str_sgati_pe;
        std::ostringstream str_errout_pe;

        str_sgati_pe<<"sgati_pe"<<(index+1);// start from 1, not from 0
        str_errout_pe<<"errout_pe"<<(index+1);// start from 1, not from 0

        sgati_pe[index] = new sc_out<bool>(str_sgati_pe.str().c_str());
        errout_pe[index] = new sc_out<bool>(str_errout_pe.str().c_str());

        sgati_pe[index]->initialize(0);
        errout_pe[index]->initialize(0);
    }


    Cecm_e2_regif::set_instance_name(this->name());//set hierarchy name
    //initial output port
    sgaterroz.initialize(false);
    sgaterroutz.initialize(false);
    sgatnmi.initialize(false);
    sgatresz.initialize(true);
    dtmtrgout.initialize(false);
    errout_clear_mask_out.initialize(true);
    pseudo_compare_msk_m.initialize(false);
    pseudo_compare_msk_c.initialize(false);
    //Initialize internal variables
    mSyncSignal             = emECMErrorOutputActiveLevel;
    mSGATERROZ              = emECMErrorOutputActiveLevel;
    mSGATERROUTZ            = emECMErrorOutputActiveLevel;
    mSGATRESZ               = emECMInternalResetInactiveLevel;
    mHoldSignal             = true;
    mRetartCounterTime      = 0;
    mCountClock             = 0;
    mPCLKClock              = 0;
    mSGATI                  = emECMInterruptInactiveLevel;
    mSGATNMI                = emECMInterruptInactiveLevel;
    mIsECMReset             = false;
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
        mERROUTPEArray[i] = false;
        mSGATIPEArray[i] = false;
    }
    //construct table
    ErrorInputTableConstruction();
    ProtectedRegisterTableConstruction();

    //Initialize mForcedPortArray
    for(unsigned int i = 0; i < emNumPE; i++){
        mForcedPortArray.push_back(sgati_pe[i]);
        mForcedPortArray.push_back(errout_pe[i]);
    }
    mForcedPortArray.push_back(&sgatnmi);
    mForcedPortArray.push_back(&sgaterroz);
    mForcedPortArray.push_back(&sgatresz);
    mForcedPortArray.push_back(&dtmtrgout);

    //Initialize General Timer
    InitializeGeneralTimer();

    //Initialize registers for supported/reserved error factors (based on TARGET_PRODUCT)
    SC_METHOD(InitializeRegistersMethod);

    SC_METHOD(ECMReceiveErrorInputMethod)
    dont_initialize();
    //sgaterrinN (N = 00 .. 308)
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
    sensitive << erroutresz_for_sync;

    SC_METHOD(resstg1z_pclkinMethod)
    dont_initialize();
    sensitive << resstg1z_pclkin;
    sensitive << erroutresz_for_sync;

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

    sc_core::sc_spawn_options otp0;
    otp0.spawn_method();
    otp0.set_sensitivity(&mWriteDTMTRGOUTPortEvent[0]);
    sc_core::sc_spawn(sc_bind(&Cecm_e2::WriteDTMTRGOUTPortMethod, this, false), sc_core::sc_gen_unique_name("WriteDTMTRGOUTPortMethod"), &otp0);
    dont_initialize();
    sc_core::sc_spawn_options otp1;
    otp1.spawn_method();
    otp1.set_sensitivity(&mWriteDTMTRGOUTPortEvent[1]);
    sc_core::sc_spawn(sc_bind(&Cecm_e2::WriteDTMTRGOUTPortMethod, this, true) , sc_core::sc_gen_unique_name("WriteDTMTRGOUTPortMethod"), &otp1);
    dont_initialize();

    SC_METHOD(WriteErrOutMskClrPortMethod)
    dont_initialize();
    sensitive << mWriteErrOutMskClrPortEvent;

    SC_METHOD(DelayTimerConfigureMethod)
    dont_initialize();
    sensitive << dtmtrgin;

    SC_METHOD(DelayTimerStopMethod)
    dont_initialize();
    sensitive << mDelayTimerStopEvent;

    SC_METHOD(EnableClearOutputErrMethod)
    dont_initialize();
    sensitive << mEnableClearOutputErrEvent;

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

    SC_METHOD(ProcessECMmECTBitMethod)
    dont_initialize();
    sensitive << mProcessECMmECTBitEvent;
}//}}}

//Destructor of Cecm_e2 class
Cecm_e2::~Cecm_e2()
{//{{{
}//}}}

void Cecm_e2::start_of_simulation(void)
{//{{{
    mStartSimulation = true;
}//}}}

unsigned int Cecm_e2::GetRegisterIndex(unsigned int factor_index)
{//{{{
    return (factor_index + 8)/32;
}//}}}

std::string Cecm_e2::GetBitName( const char name[], unsigned int bit_idx, unsigned int reg_index)
{//{{{
    std::ostringstream bit_name;
    if ((reg_index > 0) && (reg_index < 9)){
        bit_name << std::string(name) << std::right << std::setfill('0') << std::setw(2) << bit_idx;
    }else{
        bit_name << std::string(name) << reg_index << std::right << std::setfill('0') << std::setw(2) << bit_idx;
    }
    return bit_name.str().c_str();
}//}}} 

void Cecm_e2::ErrorInputTableConstruction(void)
{//{{{
    unsigned int reg_index = 0;
    unsigned int bit_idx = 0;
    std::string sse;
    std::string mie;
    std::string nmie;
    std::string ire;
    std::string emk;
    std::string clsse;
    std::string pe;
    std::string mite;
    std::string nmite;

    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[0], sgatpe[0], false, 0, &(*ECMmESSTR0)["ECMmSSE00100"], &(*ECMMICFG0)["ECMMIE00100"], &(*ECMNMICFG0)["ECMNMIE00100"], &(*ECMIRCFG0)["ECMIRE00100"], &(*ECMEMK0)["ECMEMK00100"], &(*ECMESSTC0)["ECMCLSSE00100"], &(*ECMPE0)["ECMPE00100"], &(*ECMMIDTMCFG0)["ECMMITE00100"], &(*ECMNMIDTMCFG0)["ECMNMITE00100"]));
    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[1], sgatpe[1], false, 0, &(*ECMmESSTR0)["ECMmSSE00302"], &(*ECMMICFG0)["ECMMIE00302"], &(*ECMNMICFG0)["ECMNMIE00302"], &(*ECMIRCFG0)["ECMIRE00302"], &(*ECMEMK0)["ECMEMK00302"], &(*ECMESSTC0)["ECMCLSSE00302"], &(*ECMPE0)["ECMPE00302"], &(*ECMMIDTMCFG0)["ECMMITE00302"], &(*ECMNMIDTMCFG0)["ECMNMITE00302"]));
    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[2], sgatpe[2], false, 0, &(*ECMmESSTR0)["ECMmSSE00504"], &(*ECMMICFG0)["ECMMIE00504"], &(*ECMNMICFG0)["ECMNMIE00504"], &(*ECMIRCFG0)["ECMIRE00504"], &(*ECMEMK0)["ECMEMK00504"], &(*ECMESSTC0)["ECMCLSSE00504"], &(*ECMPE0)["ECMPE00504"], &(*ECMMIDTMCFG0)["ECMMITE00504"], &(*ECMNMIDTMCFG0)["ECMNMITE00504"]));
    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[3], sgatpe[3], false, 0, &(*ECMmESSTR0)["ECMmSSE00706"], &(*ECMMICFG0)["ECMMIE00706"], &(*ECMNMICFG0)["ECMNMIE00706"], &(*ECMIRCFG0)["ECMIRE00706"], &(*ECMEMK0)["ECMEMK00706"], &(*ECMESSTC0)["ECMCLSSE00706"], &(*ECMPE0)["ECMPE00706"], &(*ECMMIDTMCFG0)["ECMMITE00706"], &(*ECMNMIDTMCFG0)["ECMNMITE00706"]));
    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[4], sgatpe[4], false, 0, &(*ECMmESSTR0)["ECMmSSE00908"], &(*ECMMICFG0)["ECMMIE00908"], &(*ECMNMICFG0)["ECMNMIE00908"], &(*ECMIRCFG0)["ECMIRE00908"], &(*ECMEMK0)["ECMEMK00908"], &(*ECMESSTC0)["ECMCLSSE00908"], &(*ECMPE0)["ECMPE00908"], &(*ECMMIDTMCFG0)["ECMMITE00908"], &(*ECMNMIDTMCFG0)["ECMNMITE00908"]));
    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[5], sgatpe[5], false, 0, &(*ECMmESSTR0)["ECMmSSE01110"], &(*ECMMICFG0)["ECMMIE01110"], &(*ECMNMICFG0)["ECMNMIE01110"], &(*ECMIRCFG0)["ECMIRE01110"], &(*ECMEMK0)["ECMEMK01110"], &(*ECMESSTC0)["ECMCLSSE01110"], &(*ECMPE0)["ECMPE01110"], &(*ECMMIDTMCFG0)["ECMMITE01110"], &(*ECMNMIDTMCFG0)["ECMNMITE01110"]));
    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[6], sgatpe[6], false, 0, &(*ECMmESSTR0)["ECMmSSE01312"], &(*ECMMICFG0)["ECMMIE01312"], &(*ECMNMICFG0)["ECMNMIE01312"], &(*ECMIRCFG0)["ECMIRE01312"], &(*ECMEMK0)["ECMEMK01312"], &(*ECMESSTC0)["ECMCLSSE01312"], &(*ECMPE0)["ECMPE01312"], &(*ECMMIDTMCFG0)["ECMMITE01312"], &(*ECMNMIDTMCFG0)["ECMNMITE01312"]));
    mErrorInputTable.push_back(ECMErrorInputElement(sgaterrin[7], sgatpe[7], false, 0, &(*ECMmESSTR0)["ECMmSSE01514"], &(*ECMMICFG0)["ECMMIE01514"], &(*ECMNMICFG0)["ECMNMIE01514"], &(*ECMIRCFG0)["ECMIRE01514"], &(*ECMEMK0)["ECMEMK01514"], &(*ECMESSTC0)["ECMCLSSE01514"], &(*ECMPE0)["ECMPE01514"], &(*ECMMIDTMCFG0)["ECMMITE01514"], &(*ECMNMIDTMCFG0)["ECMNMITE01514"]));

    for(unsigned int i = 8; i < emNumErrorFactor; i++){
        reg_index = GetRegisterIndex(i);
        bit_idx = (i + 8) % 32;
        sse = GetBitName("ECMmSSE",bit_idx, reg_index);
        mie = GetBitName("ECMMIE",bit_idx, reg_index);
        nmie = GetBitName("ECMNMIE",bit_idx, reg_index);
        ire = GetBitName("ECMIRE",bit_idx, reg_index);
        emk = GetBitName("ECMEMK",bit_idx, reg_index);
        clsse = GetBitName("ECMCLSSE",bit_idx, reg_index);
        pe = GetBitName("ECMPE",bit_idx, reg_index);
        mite = GetBitName("ECMMITE",bit_idx, reg_index);
        nmite = GetBitName("ECMNMITE",bit_idx, reg_index);
        if( i < 24){
            mErrorInputTable.push_back(ECMErrorInputElement( sgaterrin[i]
                                                           , sgatpe[i]
                                                           , false, 0
                                                           , &(*ECMmESSTR0)[sse.c_str()]
                                                           , &(*ECMMICFG0)[mie.c_str()]
                                                           , &(*ECMNMICFG0)[nmie.c_str()] 
                                                           , &(*ECMIRCFG0)[ire.c_str()] 
                                                           , &(*ECMEMK0)[emk.c_str()]
                                                           , &(*ECMESSTC0)[clsse.c_str()]
                                                           , &(*ECMPE0)[pe.c_str()]
                                                           , &(*ECMMIDTMCFG0)[mite.c_str()]
                                                           , &(*ECMNMIDTMCFG0)[nmite.c_str()]));
        }else if (i < 280){
            mErrorInputTable.push_back(ECMErrorInputElement( sgaterrin[i]
                                                           , sgatpe[i]
                                                           , false, 0
                                                           , &(*ECMmESSTR[reg_index])[sse.c_str()]
                                                           , &(*ECMMICFG[reg_index])[mie.c_str()]
                                                           , &(*ECMNMICFG[reg_index])[nmie.c_str()] 
                                                           , &(*ECMIRCFG[reg_index])[ire.c_str()] 
                                                           , &(*ECMEMK[reg_index])[emk.c_str()]
                                                           , &(*ECMESSTC[reg_index])[clsse.c_str()]
                                                           , &(*ECMPE[reg_index])[pe.c_str()]
                                                           , &(*ECMMIDTMCFG[reg_index])[mite.c_str()]
                                                           , &(*ECMNMIDTMCFG[reg_index])[nmite.c_str()]));
        }else{
            mErrorInputTable.push_back(ECMErrorInputElement( sgaterrin[i]
                                                           , sgatpe[i]
                                                           , false, 0
                                                           , &(*ECMmESSTR9)[sse.c_str()]
                                                           , &(*ECMMICFG9)[mie.c_str()]
                                                           , &(*ECMNMICFG9)[nmie.c_str()] 
                                                           , &(*ECMIRCFG9)[ire.c_str()] 
                                                           , &(*ECMEMK9)[emk.c_str()]
                                                           , &(*ECMESSTC9)[clsse.c_str()]
                                                           , &(*ECMPE9)[pe.c_str()]
                                                           , &(*ECMMIDTMCFG9)[mite.c_str()]
                                                           , &(*ECMNMIDTMCFG9)[nmite.c_str()]));
        }
    }
}//}}}

void Cecm_e2::ProtectedRegisterTableConstruction(void)
{//{{{
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET",  0x000   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR",  0x004   ,   emMasterArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET",  0x000   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR",  0x004   ,   emCheckerArea));    
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEPCFG",  0x100   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0", 0x104   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0", 0x105   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0", 0x106   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0", 0x107   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1", 0x108   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1", 0x109   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1", 0x10A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1", 0x10B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2", 0x10C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2", 0x10D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2", 0x10E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2", 0x10F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG3", 0x110   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG3", 0x111   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG3", 0x112   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG3", 0x113   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG4", 0x114   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG4", 0x115   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG4", 0x116   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG4", 0x117   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG5", 0x118   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG5", 0x119   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG5", 0x11A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG5", 0x11B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG6", 0x11C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG6", 0x11D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG6", 0x11E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG6", 0x11F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG7", 0x120   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG7", 0x121   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG7", 0x122   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG7", 0x123   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG8", 0x124   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG8", 0x125   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG8", 0x126   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG8", 0x127   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG9", 0x128   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG9", 0x129   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG9", 0x12A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG9", 0x12B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",    0x12C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",    0x12D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",    0x12E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",    0x12F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",    0x130   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",    0x131   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",    0x132   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",    0x133   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",    0x134   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",    0x135   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",    0x136   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",    0x137   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG3",    0x138   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG3",    0x139   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG3",    0x13A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG3",    0x13B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG4",    0x13C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG4",    0x13D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG4",    0x13E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG4",    0x13F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG5",    0x140   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG5",    0x141   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG5",    0x142   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG5",    0x143   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG6",    0x144   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG6",    0x145   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG6",    0x146   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG6",    0x147   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG7",    0x148   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG7",    0x149   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG7",    0x14A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG7",    0x14B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG8",    0x14C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG8",    0x14D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG8",    0x14E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG8",    0x14F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG9",    0x150   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG9",    0x151   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG9",    0x152   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG9",    0x153   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0", 0x154   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0", 0x155   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0", 0x156   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0", 0x157   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1", 0x158   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1", 0x159   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1", 0x15A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1", 0x15B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2", 0x15C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2", 0x15D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2", 0x15E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2", 0x15F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG3", 0x160   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG3", 0x161   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG3", 0x162   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG3", 0x163   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG4", 0x164   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG4", 0x165   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG4", 0x166   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG4", 0x167   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG5", 0x168   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG5", 0x169   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG5", 0x16A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG5", 0x16B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG6", 0x16C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG6", 0x16D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG6", 0x16E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG6", 0x16F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG7", 0x170   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG7", 0x171   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG7", 0x172   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG7", 0x173   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG8", 0x174   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG8", 0x175   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG8", 0x176   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG8", 0x177   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG9", 0x178   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG9", 0x179   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG9", 0x17A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG9", 0x17B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",   0x17C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",   0x17D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",   0x17E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",   0x17F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",   0x180   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",   0x181   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",   0x182   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",   0x183   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",   0x184   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",   0x185   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",   0x186   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",   0x187   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK3",   0x188   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK3",   0x189   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK3",   0x18A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK3",   0x18B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK4",   0x18C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK4",   0x18D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK4",   0x18E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK4",   0x18F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK5",   0x190   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK5",   0x191   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK5",   0x192   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK5",   0x193   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK6",   0x194   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK6",   0x195   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK6",   0x196   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK6",   0x197   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK7",   0x198   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK7",   0x199   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK7",   0x19A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK7",   0x19B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK8",   0x19C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK8",   0x19D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK8",   0x19E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK8",   0x19F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK9",   0x1A0   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK9",   0x1A1   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK9",   0x1A2   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK9",   0x1A3   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC0", 0x1A4   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC1", 0x1A8   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC2", 0x1AC   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC3", 0x1B0   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC4", 0x1B4   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC5", 0x1B8   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC6", 0x1BC   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC7", 0x1C0   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC8", 0x1C4   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC9", 0x1C8   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE0",    0x1D0   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE1",    0x1D4   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE2",    0x1D8   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE3",    0x1DC   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE4",    0x1E0   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE5",    0x1E4   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE6",    0x1E8   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE7",    0x1EC   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE8",    0x1F0   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE9",    0x1F4   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCTL", 0x1F8   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCTL", 0x1F9   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCTL", 0x1FA   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCTL", 0x1FB   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x200   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x201   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x202   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP", 0x203   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG0",  0x204   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG0",  0x205   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG0",  0x206   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG0",  0x207   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG1",  0x208   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG1",  0x209   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG1",  0x20A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG1",  0x20B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG2",  0x20C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG2",  0x20D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG2",  0x20E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG2",  0x20F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG3",  0x210   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG3",  0x211   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG3",  0x212   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG3",  0x213   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG4",  0x214   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG4",  0x215   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG4",  0x216   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG4",  0x217   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG5",  0x218   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG5",  0x219   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG5",  0x21A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG5",  0x21B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG6",  0x21C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG6",  0x21D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG6",  0x21E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG6",  0x21F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG7",  0x220   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG7",  0x221   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG7",  0x222   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG7",  0x223   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG8",  0x224   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG8",  0x225   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG8",  0x226   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG8",  0x227   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG9",  0x228   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG9",  0x229   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG9",  0x22A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMIDTMCFG9",  0x22B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG0", 0x22C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG0", 0x22D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG0", 0x22E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG0", 0x22F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG1", 0x230   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG1", 0x231   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG1", 0x232   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG1", 0x233   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG2", 0x234   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG2", 0x235   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG2", 0x236   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG2", 0x237   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG3", 0x238   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG3", 0x239   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG3", 0x23A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG3", 0x23B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG4", 0x23C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG4", 0x23D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG4", 0x23E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG4", 0x23F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG5", 0x240   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG5", 0x241   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG5", 0x242   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG5", 0x243   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG6", 0x244   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG6", 0x245   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG6", 0x246   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG6", 0x247   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG7", 0x248   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG7", 0x249   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG7", 0x24A   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG7", 0x24B   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG8", 0x24C   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG8", 0x24D   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG8", 0x24E   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG8", 0x24F   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG9", 0x250   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG9", 0x251   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG9", 0x252   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMIDTMCFG9", 0x253   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEOCCFG", 0x254   ,   emCommonArea)); 
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEOCCFG", 0x256   ,   emCommonArea)); 
}//}}}

void Cecm_e2::ECMEnableReset(const bool is_active)
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
        mEnableClearOutputErrEvent.cancel();
        mCombineSignalEvent.cancel();
        mUpdateDTMSTACNTCLKBitEvent.cancel();
        mWriteDCLSErrorPortEvent.cancel();
        mProcessECMmECTBitEvent.cancel();
        mDelayTimerStopEvent.cancel();

        //Initialize output ports
        mECMInitializeEvent.notify();

        //backup current value of ECMEOCCFG and ECMmESSTR0/1/2/3
        mECMEOCCFG = (unsigned int)(*ECMEOCCFG);    
        mECMmESSTR[0] = (unsigned int)(*ECMmESSTR0);
        for(unsigned int i = 1; i < 9; i++){
            mECMmESSTR[i] = (unsigned int)(*ECMmESSTR[i]);
        }
        mECMmESSTR[9] = (unsigned int)(*ECMmESSTR9);

        Cecm_e2_regif::EnableReset(is_active);
        // added vupham 20150304 for issue trac #152
        (*ECMEOCCFG).EnableReset(false);
        (*ECMmESSTR0).EnableReset(false);
        for(unsigned int i = 1; i < 9; i++){
            (*ECMmESSTR[i]).EnableReset(false);
        }
        (*ECMmESSTR9).EnableReset(false);
        // end added vupham

        (*ECMEOCCFG)   = mECMEOCCFG;
        (*ECMmESSTR0)  = mECMmESSTR[0]; 
        for(unsigned int i = 1; i < 9; i++){
            (*ECMmESSTR[i])  = mECMmESSTR[i]; 
        }
        (*ECMmESSTR9)  = mECMmESSTR[9]; 
    }else{
        InitializeGeneralTimer();
        Cecm_e2_regif::EnableReset(is_active);
        InitializeRegistersMethod();
    }

    //Call the reset function of common/general class
    Cgeneral_timer::EnableReset(is_active);
}//}}}

void Cecm_e2::ECMInitializeMethod(void)
{//{{{
    //Initialize output port
    //sgatpeN (N = 000 .. 308)
    for(unsigned int i=0; i<emNumErrorFactor; i++){
        mPseudoErrorArray[i] = 0;
    }
    mIsLockReg = true;
    for(unsigned int i = 0; i < 8; i++){
        mSGATIPEArray[i] = false;
        mERROUTPEArray[i] = false;
    }
    mWritePseudoErrorOutEvent.notify();
    mWriteSGATIPEPortEvent.notify();
    mWriteDCLSErrorPortEvent.notify();

    //Initialize internal variables
    mSGATI = emECMInterruptInactiveLevel;
    mCombineSGATIPEPortEvent.notify();
    mSGATNMI = emECMInterruptInactiveLevel;
    mWriteSGATNMIPortEvent.notify();
    mSGATRESZ = emECMInternalResetInactiveLevel;
    mWriteSGATRESZPortEvent.notify();
    mWriteDTMTRGOUTPortEvent[0].notify();
}//}}}

bool Cecm_e2::ECMCheckZeroClock(const std::string clk_name)
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

bool Cecm_e2::ECM_common_tgt_rd(const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    if (is_rd_dbg){
        // Set debug mode (member variable of Cecm_e2_regif class)
        return reg_rd_dbg(addr, p_data, size);
    }else{
        return reg_rd(addr, p_data, size);
    }
}//}}}

bool Cecm_e2::ECM_common_tgt_wr(const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    if (is_wr_dbg){
        return reg_wr_dbg( addr,p_data, size);
    }else{
        unsigned int data = 0;
        sc_assert(p_data != NULL);
        memcpy(&data, p_data, size);

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

void Cecm_e2::cmpMatchOutputHandling (bool value)
{//{{{
    //turn on status of delay timer overflow
    (*ECMmESSTR9)["ECMmSSE929"] = 1;
    //Issue terminal error output
    IssueErrorOutput();
    //Issue internal reset
    IssueInternalReset();
}//}}}

void Cecm_e2::ECMReceiveErrorInputMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    bool IsError = false;
    for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input
        if ((mErrorInputTable[error_index].errorin_pre != mErrorInputTable[error_index].errorin->read()) && (mErrorInputTable[error_index].errorin->read() != 0)){
            IsError = true;
        }
    }
    UpdateErrorStatus();    //Update error status
    IssueInterrupt();       //Issue interrupt signal
    IssueErrorOutput();     //Issue error output
    if (IsError) {
        if (mSGATERROZ == emECMErrorOutputActiveLevel) {
            mEnableClearOutputErrEvent.cancel();
            bool errout_clear_invalid_enable = true;
            if ((mWrapper->TARGET_PRODUCT.value == "E2x-FCC2" || mWrapper->TARGET_PRODUCT.value == "E2x-FCC1-2nd") && ((*ECMEOCCFG)["EOCIEN"] == 0) ) {
                errout_clear_invalid_enable = false;
            }
            if (errout_clear_invalid_enable) { // error output clear invalidation function enabled
                mHoldSignal = false;
                mWriteErrOutMskClrPortEvent.notify();
                mRetartCounterTime = GetCurTime();
                mEnableClearOutputErrEvent.notify(mCountClock*((unsigned int)((*ECMEOCCFG)["ECMEOUTCLRT"]) + 2),SC_NS); // Error output clear is invalid for (ECMEOUTCLRT+2) cntclk cycles
            }
            mWriteSGATERROZPortEvent.notify();
            if ((*ECMEPCFG)["ECMSL0"] == 0){ //non-dynamic mode
                mWriteSGATERROUTZPortEvent.notify();
            }
        }
    }
    IssueInternalReset();   //Issue internal reset
}//}}}

void Cecm_e2::UpdateErrorStatus(void)
{//{{{
    for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        // DCLS
        if (error_index <= 7){
            if (((mErrorInputTable[error_index].errorin_pre != mErrorInputTable[error_index].errorin->read())
                    || ((unsigned int)*mErrorInputTable[error_index].esstr == 0))
                && (mErrorInputTable[error_index].errorin->read() != 0)){
                // Count up if next error
                mErrorInputTable[error_index].error_count++;
                if (mErrorInputTable[error_index].error_count < 4){
                    *mErrorInputTable[error_index].esstr = mErrorInputTable[error_index].error_count;
                }
            }
        // Normal
        }else{
            if (mErrorInputTable[error_index].errorin->read() == 1) {
                *mErrorInputTable[error_index].esstr = 1;
            }
        }
        mPseudoErrorArray[error_index] = 0;
        // Update pre error
        if (mErrorInputTable[error_index].errorin_pre != mErrorInputTable[error_index].errorin->read()) {
            mErrorInputTable[error_index].errorin_pre = mErrorInputTable[error_index].errorin->read();
        }
    }
    mWritePseudoErrorOutEvent.notify(mPCLKClock,SC_NS);

    // Check resstg1z_pclkin or erroutresz_for_sync is active
    if ((resstg1z_pclkin.read() == vpcl::emResetActive) || (erroutresz_for_sync.read() == vpcl::emResetActive)) {
        (*ECMmESSTR9)["ECMmSSE929"] = 0;
        for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
            *mErrorInputTable[error_index].esstr = 0;
            if (error_index <= 7){
                mErrorInputTable[error_index].error_count = 0;
            }
        }
    }
    // Check resstg1z is active
    if (resstg1z.read() == vpcl::emResetActive) {
        (*ECMmESSTR9)["ECMmSSE930"] = 0;
    }
}//}}}

void Cecm_e2::IssueInterrupt(void)
{//{{{
    mSGATI = emECMInterruptInactiveLevel;
    mSGATNMI = emECMInterruptInactiveLevel;
    bool dtmtrgout_val = false;
    for (unsigned int i = 0; i <emNumPE; i++){
        mERROUTPEArray[i] = emECMInterruptInactiveLevel;
    }

    for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        bool disabled = false;
        // E2x-FCC2: DCLS interupts not generated when an error input asserts more than 3 times
        if ((mWrapper->TARGET_PRODUCT.value == "E2x-FCC2" || mWrapper->TARGET_PRODUCT.value == "E2x-FCC1-2nd") && (error_index <= 7) && (mErrorInputTable[error_index].error_count > 3)) {
            disabled = true;
        }
        //There is error input
        if (mErrorInputTable[error_index].errorin->read() != 0){
            // Maskable interupt
            if (((unsigned int)*mErrorInputTable[error_index].micfg > 0)
                    && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].micfg) && (!disabled)){
                if (error_index <= 7){// For DCLS mask interrupt port PE1..8
                    mERROUTPEArray[error_index - emOffsetDCLS] = emECMInterruptActiveLevel; // Activate
                }else{  //ti signal is OR from 8 -> 308
                    mSGATI = emECMInterruptActiveLevel;
                }

                if (((unsigned int)*mErrorInputTable[error_index].midtmcfg > 0) 
                        && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].midtmcfg)){
                    dtmtrgout_val = true;
                }
            }
            // Non-maskable interupt (for both DCLS and normal error)
            if (((unsigned int)*mErrorInputTable[error_index].nmicfg > 0)
                    && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].nmicfg) && (!disabled)){
                mSGATNMI = emECMInterruptActiveLevel;
                if ((*mErrorInputTable[error_index].nmidtmcfg > 0)
                        && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].nmidtmcfg)){
                    dtmtrgout_val = true;
                }
            }
        }
    }
    //write maskable interrupt and non-maskable interrupt
    mCombineSGATIPEPortEvent.notify();
    mWriteSGATNMIPortEvent.notify();
    //write value to dtmtrgout
    if (dtmtrgout_val && ECMCheckZeroClock("cntclk") && ECMCheckZeroClock("pclk")) {
        if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
            mWriteDTMTRGOUTPortEvent[1].notify(3*mCountClock, SC_NS); // wait for 3 cntclk cycles to start delay timer
        } else {
            mWriteDTMTRGOUTPortEvent[1].notify();
        }
    }
}//}}}

void Cecm_e2::DelayTimerConfigureMethod(void)
{//{{{
    if(dtmtrgin.read() && (((unsigned int)(*ECMDTMCTL) & 0x3) == 1)){ //if delay timer can start
        StartStopCounter(true);             //Start general timer
    }
}//}}}

void Cecm_e2::DelayTimerStopMethod(void)
{//{{{
    StartStopCounter(false); //Stop general timer
    Cgeneral_timer::setCNT(0x0);          //Count value
}//}}}

void Cecm_e2::IssueErrorOutput(void)
{//{{{
    CalculateErrorOutput();
    mWriteSGATERROZPortEvent.notify();
    //Non-dynamic mode, and there is no error input
    if ((mSGATERROUTZ == emECMErrorOutputActiveLevel) || ((*ECMEPCFG)["ECMSL0"] == 0)){
        mWriteSGATERROUTZPortEvent.notify();
    }
}//}}}

void Cecm_e2::CalculateErrorOutput(void)
{//{{{ 
    bool status_mask = emECMErrorOutputInactiveLevel;
    //There is delay timer overflow and None mask
    if (((*ECMmESSTR9)["ECMmSSE929"] == 1) && (*ECMEMK9)["ECMEMK929"] == 0){
        status_mask = emECMErrorOutputActiveLevel;
    }
    // For all remains errors
    for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input and None mask (for both DCLS and normal error)
        if ((unsigned int)*mErrorInputTable[error_index].esstr > (unsigned int)*mErrorInputTable[error_index].emk){
            status_mask = emECMErrorOutputActiveLevel;
            break;
        }
    }
    mSGATERROZ   = status_mask && mSyncSignal;
    mSGATERROUTZ = status_mask && mSyncSignal;
    mSyncSignal  = mSGATERROZ;
}//}}}

void Cecm_e2::IssueInternalReset(void)
{//{{{
    //There is pseudo-overflow and its internal reset is permited
    if (((*ECMIRCFG9)["ECMIRE929"] == 1) && ((*ECMmESSTR9)["ECMmSSE929"] == 1)){
        mSGATRESZ = emECMInternalResetActiveLevel;
    }
    for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        bool disabled = false;
        // E2x-FCC2: DCLS internal reset not generated when an error input asserts more than 3 times
        if ((mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") && (error_index <= 7) && (mErrorInputTable[error_index].error_count > 3)) {
            disabled = true;
        }
        //There is error input and internal reset is permited, number of error count >= ircfg
        if ((mErrorInputTable[error_index].errorin->read() != 0)
            && (*mErrorInputTable[error_index].ircfg > 0)
            && ((unsigned int)*mErrorInputTable[error_index].esstr >= (unsigned int)*mErrorInputTable[error_index].ircfg) && (!disabled)){
            mSGATRESZ = emECMInternalResetActiveLevel;
            break;
        }
    }
    mWriteSGATRESZPortEvent.notify();
}//}}}

void Cecm_e2::LoopBackSignalProcessingMethod(void)
{//{{{ 
    if (mIsECMReset){
        return;
    }
    if (sgaterrlbz.read()){
        (*ECMmESSTR9)["ECMmSSE931"] = 1;
    }else{
        (*ECMmESSTR9)["ECMmSSE931"] = 0;
    }
}//}}}

void Cecm_e2::resstg1zMethod(void)
{//{{{
    // This method NOT depend on preset_n. No need to check mIsECMReset
    //if resstg1z is active
    if ((resstg1z.read() == vpcl::emResetActive) || (erroutresz_for_sync.read() == vpcl::emResetActive)) {
        (*ECMmESSTR9)["ECMmSSE930"] = 0;//clear error set triger status bit
    }
}//}}}

void Cecm_e2::resstg1z_pclkinMethod(void)
{//{{{
    // This method NOT depend on preset_n. No need to check mIsECMReset
    UpdateErrorStatus();
    IssueErrorOutput();
    IssueInternalReset();
}//}}}

void Cecm_e2::sgattinMethod(void)
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

void Cecm_e2::WriteSGATERROZPortMethod(void)
{//{{{
    sgaterroz.write(mSGATERROZ);
}//}}}

void Cecm_e2::WriteSGATERROUTZPortMethod(void)
{//{{{ 
    sgaterroutz.write(mSGATERROUTZ);
}//}}}

void Cecm_e2::WriteSGATRESZPortMethod(void)
{//{{{
    sgatresz.write(mSGATRESZ);
}//}}}

void Cecm_e2::WriteSGATNMIPortMethod(void)
{//{{{
    if (!ECMCheckZeroClock("pclk")) {
        return;
    }
    sgatnmi.write(mSGATNMI);

    // Negate interrupts after 1 pclk cycle
    if (mSGATNMI == emECMInterruptActiveLevel) {
        mSGATNMI = emECMInterruptInactiveLevel;
        mWriteSGATNMIPortEvent.notify(mPCLKClock, SC_NS);
    }
}//}}}

void Cecm_e2::WriteDTMTRGOUTPortMethod(bool dtmtrgout_val)
{//{{{
    dtmtrgout.write(dtmtrgout_val);

    // Negate dtmtrgout after 1 pclk cycle
    if (dtmtrgout_val) {
        mWriteDTMTRGOUTPortEvent[0].notify(mPCLKClock, SC_NS);
    }
}//}}}

void Cecm_e2::WriteErrOutMskClrPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    errout_clear_mask_out.write(mHoldSignal);
}//}}}

bool Cecm_e2::CheckProtectedRegister(unsigned int addr)
{//{{{
    addr = addr & 0x3FF;
    for(unsigned int index = 0; index < mProtectedRegisterTable.size(); index++){
        if (addr == mProtectedRegisterTable[index].addr){
            return true;
        }
    }
    return false;
}//}}}

std::string Cecm_e2::GetProtectedRegisterName(unsigned int addr)
{//{{{
    addr = addr & 0x3FF;
    for(unsigned int index = 0; index < mProtectedRegisterTable.size(); index++){
        if (addr == mProtectedRegisterTable[index].addr){
            return mProtectedRegisterTable[index].register_name;
        }
    }
    return "";
}//}}}

void Cecm_e2::StartStopCounter(const bool start)
{//{{{
    (*ECMmESSTR9)["ECMmSSE929"] = 0;
    if (start){
        if (Cgeneral_timer::getCR().gtSTR) { // delay timer is counting
            return;
        } else if((unsigned int)((*ECMDTMCMP)["ECMDTMCMP"]) > 0){
            Cgeneral_timer::setClkCountPeriod(mCountClock);
            if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
                Cgeneral_timer::setCOR((unsigned int)((*ECMDTMCMP)["ECMDTMCMP"]) + 2); // compare match occurs at the end of next cntclk cycle
            } else {
                Cgeneral_timer::setCOR((unsigned int)((*ECMDTMCMP)["ECMDTMCMP"]));
            }
            Cgeneral_timer::setSTR(true);
        } else { //compare match
            cmpMatchOutputHandling(true);
        }
    }else{
        Cgeneral_timer::setSTR(false);
    }
}//}}} 

void Cecm_e2::InitializeGeneralTimer(void)
{//{{{
    Cgeneral_timer::setSTR(false);                  //Count permission
    Cgeneral_timer::setCMS(16U);
    Cgeneral_timer::setCMM(emOneShot);              //Operation mode
    Cgeneral_timer::setCounterDirection(emCountUp); //Count mode
    Cgeneral_timer::setCCLR(emInFactor);            //Clear mode
    Cgeneral_timer::setTimeUnit(SC_NS);             //Time unit
    Cgeneral_timer::setCOR(0x0);                    //Match value
    Cgeneral_timer::setCNT(0x0);                    //Count value
    Cgeneral_timer::setCKS(emClkDivide);            //Divide Input clock
}//}}} 

void Cecm_e2::InitializeRegistersMethod(void)
{//{{{
    unsigned char fcc2_support[emNumErrorFactor] = {  1,    1,    0,    0,    1,    1,    0,    0,    1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                                                      1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                                                      1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                                                      1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                                                      1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
                                                      1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                      1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1 
                                                   };
    for (unsigned int error_index = 0; error_index < emNumErrorFactor; error_index++) {
        if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
            *mErrorInputTable[error_index].emk = (fcc2_support[error_index] == 1) ? 0 : 3; // if factor is reserved, EMK bits are initialized all-1
        } else {
            *mErrorInputTable[error_index].emk = 0; // if factor is reserved, EMK bits are initialized all-1
        }
    }
    if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
        (*ECMEMK9)["ECMEMK930"] = 1;
        (*ECMEMK9)["ECMEMK931"] = 1;
    }
}//}}} 

void Cecm_e2::EnableClearOutputErrMethod(void)
{//{{{
    mHoldSignal = true;
    mWriteErrOutMskClrPortEvent.notify();
}//}}}

void Cecm_e2::ECMSetZeroClock(const std::string clk_name)
{//{{{
    //cancel all processes 
    mWriteSGATERROZPortEvent.cancel();
    mWriteSGATERROUTZPortEvent.cancel();
    mWriteSGATRESZPortEvent.cancel();
    mCombineSGATIPEPortEvent.cancel();
    mWriteSGATIPEPortEvent.cancel();
    mWriteSGATNMIPortEvent.cancel();
    mEnableClearOutputErrEvent.cancel();
    mCombineSignalEvent.cancel();

    if (clk_name == "cntclk") {
        mWriteDTMTRGOUTPortEvent[0].cancel();
        mWriteDTMTRGOUTPortEvent[1].cancel();
        StartStopCounter(false); //Stop general timer
    }
}//}}}

void Cecm_e2::ResetForErrpinMethod(void)
{//{{{
    if (erroutresz.read() == vpcl::emResetActive) {
        (*ECMEOCCFG) = 0; 
        mHoldSignal = true;
        mSyncSignal = emECMErrorOutputActiveLevel;
        mSGATERROZ = emECMErrorOutputActiveLevel;
        mSGATERROUTZ = emECMErrorOutputActiveLevel;
        mWriteSGATERROZPortEvent.notify();
        mWriteSGATERROUTZPortEvent.notify();
        mWriteErrOutMskClrPortEvent.notify();
    } else {
        IssueErrorOutput();
    } 
}//}}}

void Cecm_e2::WritePseudoErrorOutMethod()
{//{{{
    for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        mErrorInputTable[error_index].pseudo_error_out->write(mPseudoErrorArray[error_index]);
    }
}//}}}

void Cecm_e2::CombineSGATIPEPortMethod()
{//{{{ 
    // Combine sgati_pe1..8
    for(unsigned int i = 0; i < emNumPE; i++){
        mSGATIPEArray[i] = false;
        for(unsigned int j = 0; j < emNumPE; j++){
            if (i != j){
                mSGATIPEArray[i] |= mERROUTPEArray[j];
            }
        }
        mSGATIPEArray[i] |= mSGATI;
    }
    mWriteSGATIPEPortEvent.notify();
    mWriteDCLSErrorPortEvent.notify();

    // Negate interrupts after 1 pclk cycle
    mNegateInterruptEvent.notify(mPCLKClock, SC_NS);
}//}}}

void Cecm_e2::NegateInterruptMethod()
{//{{{ 
    bool deactivate = false;
    for (unsigned int i = 0; i < emNumPE ; i++) {
        if (mERROUTPEArray[i] == emECMInterruptActiveLevel) {
            mERROUTPEArray[i] = emECMInterruptInactiveLevel;
            deactivate = true;
        }
    }
    if (mSGATI == emECMInterruptActiveLevel) {
        mSGATI = emECMInterruptInactiveLevel;
        deactivate = true;
    }
    if (deactivate) {
        mCombineSGATIPEPortEvent.notify();
    }
}//}}}

void Cecm_e2::WriteSGATIPEPortMethod()
{//{{{ 
    if (!ECMCheckZeroClock("pclk")) {
        return;
    }
    for(unsigned int i = 0; i < emNumPE; i++){
        sgati_pe[i]->write(mSGATIPEArray[i]);
    }
}//}}}

void Cecm_e2::UpdateDTMSTACNTCLKBitMethod()
{//{{{ 
    if (mIsECMReset || (!ECMCheckZeroClock("cntclk"))) {
        return;
    }
    (*ECMDTMCTL)["DTMSTACNTCLK"] = (unsigned int)(*ECMDTMCTL)["DTMSTA"];
}//}}}

void Cecm_e2::WriteDCLSErrorPortMethod()
{//{{{  
    if (!ECMCheckZeroClock("pclk")) {
        return;
    }
    for(unsigned int i= 0; i < emNumPE; i++){
        errout_pe[i]->write(mERROUTPEArray[i]);
    }
}//}}}

void Cecm_e2::ProcessECMmECTBitMethod()
{//{{{  
    // Check all error status are 0 or masked
    bool status_mask = emECMErrorOutputInactiveLevel;
    //There is delay timer overflow and None mask
    if (((*ECMmESSTR9)["ECMmSSE929"] == 1) && ((*ECMEMK9)["ECMEMK929"] == 0)){
        status_mask = emECMErrorOutputActiveLevel;
    }
    for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input and None mask (for both DCLS and normal error)
        if ((unsigned int)*mErrorInputTable[error_index].esstr > (unsigned int)*mErrorInputTable[error_index].emk){
            status_mask = emECMErrorOutputActiveLevel;
            break;
        }
    }
    if (((unsigned int)(*ECMmESSTR9)["ECMmSSE930"] == 0) && (status_mask == emECMErrorOutputInactiveLevel)){
        mSyncSignal = emECMErrorOutputInactiveLevel;
        IssueErrorOutput();
    }
}//}}}

void Cecm_e2::cb_ECMESSTC0_ECMCLSSE00100(RegCBstr str)
{//{{{
    for(unsigned int error_index = 0; error_index < 24; error_index++){// error 0 to 23
        // Clear error status when error input negated, and CLSSE bit > 0 (both DCLS and normal error)
        if ((mErrorInputTable[error_index].errorin->read() == 0) && ((unsigned int)*mErrorInputTable[error_index].esstc > 0)){
            *mErrorInputTable[error_index].esstr = 0;
            if (error_index <= 7){
                mErrorInputTable[error_index].error_count = 0;
            }
        }
    }
    (*ECMESSTC0) = 0x0;
    IssueErrorOutput();
    IssueInternalReset();
}//}}}  

void Cecm_e2::cb_ECMESSTC_ECMCLSSE00(RegCBstr str)
{//{{{ 
    for(unsigned int error_index = 24; error_index < 280; error_index++){// error 24 to 55
        if ((mErrorInputTable[error_index].errorin->read() == 0) && ((unsigned int)*mErrorInputTable[error_index].esstc > 0)){
            *mErrorInputTable[error_index].esstr = 0;
        }
    }
    (*ECMESSTC[str.channel]) = 0x0;
    IssueErrorOutput();
    IssueInternalReset();
}//}}}

void Cecm_e2::cb_ECMESSTC9_ECMCLSSE900(RegCBstr str)
{//{{{ 
    for(unsigned int error_index = 280; error_index < mErrorInputTable.size(); error_index++){//error 88 to 308
        if ((mErrorInputTable[error_index].errorin->read()==0) && ((unsigned int)*mErrorInputTable[error_index].esstc > 0)){
            *mErrorInputTable[error_index].esstr = 0;
        }
    }
    if ((*ECMESSTC9)["ECMCLSSE929"] == 1) {
        //clear delay timer overflow status bit
        (*ECMmESSTR9)["ECMmSSE929"] = 0;
    }
    if ((*ECMESSTC9)["ECMCLSSE930"] == 1) {
        //clear error set trigger status bit
        (*ECMmESSTR9)["ECMmSSE930"] = 0;
    }
    (*ECMESSTC9) = 0x0;
    IssueErrorOutput();
    IssueInternalReset();
}//}}}  

void Cecm_e2::cb_ECMEMK0_ECMEMK00100(RegCBstr str)
{//{{{
    IssueErrorOutput();
}//}}}

void Cecm_e2::cb_ECMEMK_ECMEMK00(RegCBstr str)
{//{{{
    IssueErrorOutput();
}//}}}

void Cecm_e2::cb_ECMEMK9_ECMEMK900(RegCBstr str)
{//{{{ 
    (*ECMEMK9)["ECMEMK930"] = 0;
    (*ECMEMK9)["ECMEMK931"] = 0;
    if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
        (*ECMEMK9)["ECMEMK930"] = 1;
        (*ECMEMK9)["ECMEMK931"] = 1;
    }
    IssueErrorOutput();
}//}}}

void Cecm_e2::cb_ECMmESET_ECMmEST(RegCBstr str)
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
        (*ECMmESSTR9)["ECMmSSE930"] = 1;
    }
}//}}}

void Cecm_e2::cb_ECMmECLR_ECMmECT(RegCBstr str)
{//{{{
    // Ignore if write 0
    if ((str.data & 0x1) == 0){
        return;
    }

    (*ECMmECLR)["ECMmECT"] = 0;
    if (!(ECMCheckZeroClock("pclk") && ECMCheckZeroClock("cntclk"))) {
        return;
    }

    if (errout_clear_mask_in.read()) {
        if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
            mProcessECMmECTBitEvent.notify(3*mCountClock, SC_NS);
        } else {
            mProcessECMmECTBitEvent.notify(3*mCountClock + 5*mPCLKClock, SC_NS);
        }
    }
}//}}}

void Cecm_e2::cb_ECMKCPROT_KCE(RegCBstr str)
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

void Cecm_e2::cb_ECMDTMCTL_DTMSTP(RegCBstr str)
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
            if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
                mDelayTimerStopEvent.notify(3*mCountClock, SC_NS); // wait for 3 cntclk cycles until delay timer is stopped
            } else {
                mDelayTimerStopEvent.notify();
            }
            (*ECMDTMCTL)["DTMSTA"] = 0;
            (*ECMDTMCTL)["DTMSTP"] = 0; // not hold value
        }else{
            if ((str.data & 0x1) != (str.pre_data & 0x1)) {
                //STA bit = 0
                if ((str.data & 0x1) == 0) {
                    if (mWrapper->TARGET_PRODUCT.value == "E2x-FCC2") {
                        mDelayTimerStopEvent.notify(3*mCountClock, SC_NS); // wait for 3 cntclk cycles until delay timer is stopped
                    } else {
                        mDelayTimerStopEvent.notify();
                    }
                }
            }
        }
        mUpdateDTMSTACNTCLKBitEvent.notify(mCountClock, SC_NS);
    }else{
        re_printf("warning", "Should write to ECMDTMCTL register while value of DTMSTACNTCLK bit equals to value of DTMSTA bit.");
        (*ECMDTMCTL) = (unsigned int)str.pre_data;// not update written value
    }
}//}}}

void Cecm_e2::cb_ECMDTMCMP_ECMDTMCMP(RegCBstr str)
{//{{{
    if (Cgeneral_timer::getCR().gtSTR) { // delay timer is counting
        re_printf("warning","Cannot write to ECMDTMCMP register when delay timer is counting.\n");
        (*ECMDTMCMP) = str.pre_data;
    }
}//}}}

void Cecm_e2::cb_ECMDTMR_ECMDTMR(RegCBstr str)
{//{{{ 
    unsigned int value = Cgeneral_timer::getCNT();
    (*ECMDTMR) = value;
}//}}}

void Cecm_e2::cbDBG_ECMDTMR_ECMDTMR(RegCBstr str)
{//{{{ 
    unsigned int value = Cgeneral_timer::getCNT();
    (*ECMDTMR) = value;
}//}}}

void Cecm_e2::cb_ECMPE0_ECMPE00100(RegCBstr str)
{//{{{
    //Issue pseudo error
    for(unsigned int error_index = 0; error_index < 24; error_index++){// 0 to 23
        mPseudoErrorArray[error_index] = (bool)(*(mErrorInputTable[error_index].pe));
    }
    mWritePseudoErrorOutEvent.notify();
    (*ECMPE0) = 0x0; //not hold value
}//}}}

void Cecm_e2::cb_ECMPE_ECMPE00(RegCBstr str)
{//{{{ 
    //Issue pseudo error according channel of register
    unsigned int start_index = str.channel*32 - 8;
    unsigned int end_index = start_index + 32 - 1;
    for(unsigned int error_index = start_index; error_index <= end_index; error_index++){
        mPseudoErrorArray[error_index] = (bool)(*(mErrorInputTable[error_index].pe));
    }
    mWritePseudoErrorOutEvent.notify();
    (*ECMPE[str.channel]) = 0x0; //not hold value
}//}}}

void Cecm_e2::cb_ECMPE9_ECMPE900(RegCBstr str)
{//{{{
    //There is pseudo-delay timer overflow if ECMPE929 bit equals 1
    if (((*ECMPE9)["ECMPE929"] == 1) && ECMCheckZeroClock("cntclk")) {
        cmpMatchOutputHandling(true);
    }
    //Issue pseudo error
    for(unsigned int error_index = 280; error_index < mErrorInputTable.size(); error_index++){// 88 to 308
        mPseudoErrorArray[error_index] = (bool)(*(mErrorInputTable[error_index].pe));
    }
    mWritePseudoErrorOutEvent.notify();
    (*ECMPE9) = 0x0; //not hold value
}//}}}

void Cecm_e2::cb_ECMEOCCFG_ECMEOUTCLRT(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock("cntclk")) {
        return;
    }
    if (erroutresz.read() == vpcl::emResetActive) {
        (*ECMEOCCFG) = 0; 
        return;
    }
    if (((unsigned int)(*ECMEOCCFG) != str.pre_data)&&(mHoldSignal == false)) { // write to this register when error output clear invalidation is active
        mEnableClearOutputErrEvent.cancel();
        bool errout_clear_invalid_enable = true;
        if ((mWrapper->TARGET_PRODUCT.value == "E2x-FCC2" || mWrapper->TARGET_PRODUCT.value == "E2x-FCC1-2nd") && ((*ECMEOCCFG)["EOCIEN"] == 0) ) {
            errout_clear_invalid_enable = false;
        }
        if (errout_clear_invalid_enable) { // error output clear invalidation function enabled
            double current_time = GetCurTime();
            double waited_time = current_time - mRetartCounterTime;
            double new_wait_time = (double)(mCountClock*((unsigned int)((*ECMEOCCFG)["ECMEOUTCLRT"]) + 2)); // (ECMEOUTCLRT+2) cntclk cycles
            if (new_wait_time <= waited_time) {
                mEnableClearOutputErrEvent.notify();  // enable output clear
            } else {
                double additional_wait = new_wait_time - waited_time;
                mEnableClearOutputErrEvent.notify(additional_wait, SC_NS);
            }
        } else { // function disabled
            mEnableClearOutputErrEvent.notify();  // enable output clear
        }
    }
    if (mWrapper->TARGET_PRODUCT.value != "E2x-FCC2" && mWrapper->TARGET_PRODUCT.value != "E2x-FCC1-2nd" ) {
        (*ECMEOCCFG)["EOCIEN"] = 0;  // EOCIEN bit is not supported in E2x-FCC1
    }
}//}}}

void Cecm_e2::cb_ECMPEM_MSKC(RegCBstr str)
{//{{{
    pseudo_compare_msk_m.write((bool)((str.data >> 1) & 0x1));
    pseudo_compare_msk_c.write((bool)(str.data & 0x1));
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit SC_NS)
**********************************/
double Cecm_e2::GetCurTime(void)
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

//===============Cecm_e2_wp class=======================
//Constructor of Cecm_e2_wp class
Cecm_e2_wp::Cecm_e2_wp(sc_module_name name):
         vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 3> (name)
         //sgaterrin (N = 000 .. 308): in this, 0-307 in constructor
         ,master_sgaterrin308_sig("master_sgaterrin308_sig") 
         ,checker_sgaterrin308_sig("checker_sgaterrin308_sig")  
         ,master_sgaterroz_sig("master_sgaterroz_sig")
         ,master_sgatnmi_sig("master_sgatnmi_sig")
         ,master_sgatresz_sig("master_sgatresz_sig")
         ,checker_sgaterroz_sig("checker_sgaterroz_sig")
         ,checker_sgatnmi_sig("checker_sgatnmi_sig")
         ,checker_sgatresz_sig("checker_sgatresz_sig")
         ,errout_clear_mask_sig("errout_clear_mask_sig")
         ,errout_clear_mask_checker("errout_clear_mask_checker")
         ,master_ecmterrin308msk_m("master_ecmterrin308msk_m")
         ,master_ecmterrin308msk_c("master_ecmterrin308msk_c")
         ,checker_ecmterrin308msk_m("checker_ecmterrin308msk_m")
         ,checker_ecmterrin308msk_c("checker_ecmterrin308msk_c")
         ,checker_trgout_sig("checker_trgout_sig")
         ,master_trgout_sig("master_trgout_sig")
         ,dtmtrgout_sig("dtmtrgout_sig")
         ,pclk("pclk")
         ,cntclk("cntclk")
         ,preset_n("preset_n")
         ,cntclk_preset_n("cntclk_preset_n")
         ,erroutresz("erroutresz")
         ,cntclk_erroutresz("cntclk_erroutresz")
         ,erroutresz_for_sync("erroutresz_for_sync")
         ,resstg1z("resstg1z")
         ,pclkin_resstg1z("pclkin_resstg1z")
         ,ecmterrlbz_m("ecmterrlbz_m")
         ,ecmterrlbz_c("ecmterrlbz_c")
         ,ecmttin("ecmttin")
         ,svaccess("svaccess")
         ,ecmterroz("ecmterroz")
         ,ecmterroutz("ecmterroutz")
         ,ecmtnmi("ecmtnmi")
         ,ecmtresz("ecmtresz")
         ,ecmterroutz_m("ecmterroutz_m")
         ,ecmterroutz_c("ecmterroutz_c")
         ,TARGET_PRODUCT("TARGET_PRODUCT","")
{//{{{
    CommandInit(this->name());
    //Name ports/signals
    for(unsigned int index = 0; index < emNumErrorFactor-1; index++){// 307 error in
        std::ostringstream str_sgaterrin_sig;
        std::ostringstream str_ecmterrin;

        str_sgaterrin_sig<<"sgaterrin_sig"<<index;
        str_ecmterrin<<"ecmterrin"<<index;
        ecmterrin[index] = new sc_in<bool>(str_ecmterrin.str().c_str());
        sgaterrin_sig[index] = new sc_signal<bool, SC_UNCHECKED_WRITERS>(str_sgaterrin_sig.str().c_str());

        *sgaterrin_sig[index] = 0;
    }
    for(unsigned int index = 0; index < emNumErrorFactor; index++){// 308
        std::ostringstream str_master_sgatpe_sig;
        std::ostringstream str_checker_sgatpe_sig;

        str_master_sgatpe_sig<<"master_sgatpe_sig"<<index;
        str_checker_sgatpe_sig<<"checker_sgatpe_sig"<<index;
        master_sgatpe_sig[index] = new sc_signal<bool, SC_UNCHECKED_WRITERS>(str_master_sgatpe_sig.str().c_str());
        checker_sgatpe_sig[index] = new sc_signal<bool, SC_UNCHECKED_WRITERS>(str_checker_sgatpe_sig.str().c_str());

        *master_sgatpe_sig[index] = 0;
        *checker_sgatpe_sig[index] = 0;
    }
    for(unsigned int index = 0; index < emNumPE; index++){
        std::ostringstream str_master_dclsint_sig;
        std::ostringstream str_checker_dclsint_sig;
        std::ostringstream str_master_sgati_pe_sig;
        std::ostringstream str_checker_sgati_pe_sig;
        std::ostringstream str_ecmti_pe;
        std::ostringstream str_ecmdclsint;

        str_ecmti_pe<<"ecmti_pe"<<index;
        str_ecmdclsint<<"ecmdclsint"<<index;
        str_master_dclsint_sig<<"master_dclsint_sig"<<index;
        str_checker_dclsint_sig<<"checker_dclsint_sig"<<index;
        str_master_sgati_pe_sig<<"master_sgati_pe_sig"<<index;
        str_checker_sgati_pe_sig<<"checker_sgati_pe_sig"<<index;

        ecmti_pe[index] = new sc_out<bool>(str_ecmti_pe.str().c_str());
        ecmdclsint[index] = new sc_out<bool>(str_ecmdclsint.str().c_str());
        master_dclsint_sig[index] = new sc_signal<bool, SC_UNCHECKED_WRITERS>(str_master_dclsint_sig.str().c_str());
        checker_dclsint_sig[index] = new sc_signal<bool, SC_UNCHECKED_WRITERS>(str_checker_dclsint_sig.str().c_str());
        master_sgati_pe_sig[index] = new sc_signal<bool, SC_UNCHECKED_WRITERS>(str_master_sgati_pe_sig.str().c_str());
        checker_sgati_pe_sig[index] = new sc_signal<bool, SC_UNCHECKED_WRITERS>(str_checker_sgati_pe_sig.str().c_str());

        ecmti_pe[index]->initialize(0);
        ecmdclsint[index]->initialize(0);
        *master_dclsint_sig[index] = 0;
        *checker_dclsint_sig[index] = 0;
        *master_sgati_pe_sig[index] = 0;
        *checker_sgati_pe_sig[index] = 0;
    }


    mMaster = new Cecm_e2 ("mMaster", this);
    sc_assert(mMaster != NULL);
    mChecker = new Cecm_e2 ("mChecker", this);
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
    for(unsigned int i = 0; i < 8; i++){
        mECMTI_PE[i] = false;
        mECMDCLSINT[i] = false;
    }
    mECMTNMI                = false;
    mECMTRESZ               = true;
    mECMTERROZ              = false;
    mFreqPCLK               = 0;
    mFreqCNTCLK             = 0;

    InitializeInternalSignal();
    //Initialize output port
    ecmterroz.initialize(false);
    ecmterroutz.initialize(false);
    ecmtnmi.initialize(false);
    ecmtresz.initialize(true);
    ecmterroutz_m.initialize(false);
    ecmterroutz_c.initialize(false);

    //Initialize mExternalErrorInputTable
    ExternalErrorInputTableConstruction();

    SC_METHOD(ReceiveExternalErrorMethod)
    dont_initialize();
    //sensitive <<  ecmterrinN (N = 000 .. 307) error input
    for(unsigned int i = 0; i < emNumErrorFactor -1 ; i++){
        sensitive <<  (*ecmterrin[i]);
    }
    //sensitive <<  sgatpeN (N = 000 .. 114), not sensitive factor 307 because no pseudo-error
    for(unsigned int i = 0; i < emNumErrorFactor - 3; i++){
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
    sensitive << ecmterroutz_m;
    sensitive << master_sgatnmi_sig;
    sensitive << master_sgatresz_sig;
    sensitive << checker_sgaterroz_sig;
    sensitive << ecmterroutz_c;
    sensitive << checker_sgatnmi_sig;
    sensitive << checker_sgatresz_sig;
    sensitive << checker_trgout_sig;
    sensitive << master_trgout_sig;
    sensitive << mMaster->mCombineSignalEvent;
    sensitive << mChecker->mCombineSignalEvent;
    for(unsigned int i = 0; i < emNumPE; i++){
        sensitive << (*master_sgati_pe_sig[i]);
        sensitive << (*checker_sgati_pe_sig[i]);
    }

    SC_METHOD(WriteOutputPortMethod)
    dont_initialize();
    sensitive << mWriteOutputPortEvent;

    SC_METHOD(ComparatorTestMethod)
    dont_initialize();
    sensitive << (*master_sgatpe_sig[308]);
    sensitive << mComparatorTestEvent;
    sensitive << master_ecmterrin308msk_m;
    sensitive << master_ecmterrin308msk_c;


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

Cecm_e2_wp::~Cecm_e2_wp()
{//{{{
    delete mMaster;
    delete mChecker;
}//}}}

std::string Cecm_e2_wp::GetMasterCheckerName(const bool is_master) const
{//{{{
    if (is_master){
        return (std::string)mMaster->name();
    }else{
        return (std::string)mChecker->name();
    }
}//}}}

void Cecm_e2_wp::ReceiveExternalErrorMethod(void)
{//{{{
    if (mPortReset || mCmdReset){
        return;
    }

    UpdateState(mState, emErrorInputProcessingState);
    //sgatpeN_sig.write(ecmterrinN.read()) (N = 000 .. 114)
    //{{{
    for(unsigned int i = 0; i < emNumErrorFactor - 3; i++){//0-305 connect, pseudo error 306, 307 not connect
        sgaterrin_sig[i]->write(ecmterrin[i]->read() | master_sgatpe_sig[i]->read());
    }
    sgaterrin_sig[306]->write(ecmterrin[306]->read()); // sgatpe307 not connected.
    sgaterrin_sig[307]->write(ecmterrin[307]->read()); // sgatpe307 not connected.
    //}}}
    DumpEnableTransInfo();
    
    UpdateState(mState, emECMProcessingState);
}//}}} 

void Cecm_e2_wp::ResetMethod(void)
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

void Cecm_e2_wp::AssertReset( double delay, double period)
{//{{{
    //If preset_n is negated, ECM_WP waits delay time then notifies handleCommand reset event
    if ((!mPortReset)&&(!mIsCmdResetStatus)){
        mIsCmdResetStatus = true;
        re_printf("info","ECM_WP will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) delay, FRACTION_TO_INT(delay));
        mCmdResetEvent.notify(delay, SC_NS);
        mResetPeriod = period;
    }else{
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Cecm_e2_wp::CmdResetMethod(void)
{//{{{
    mCmdReset = true;
    this->EnableReset(mCmdReset);
    mCancelCmdResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Cecm_e2_wp::CancelCmdResetMethod(void)
{//{{{
    mCmdReset = false;
    mIsCmdResetStatus = false;
    this->EnableReset(mCmdReset);
}//}}}

void Cecm_e2_wp::EnableReset(const bool is_active)
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

void Cecm_e2_wp::PCLKPeriodMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Cecm_e2_wp::CounterPeriodMethod(void)
{//{{{
    SetCLKfreq("cntclk", sc_dt::uint64_to_double(cntclk.read()));
}//}}}

void Cecm_e2_wp::CombineSignalMethod(void)
{//{{{
    UpdateState(mState, emOutputSignalCombinationState);
    // ecmdclsint1..8
    bool ecmdclsint_array[emNumPE];
    for(unsigned int i = 0; i < emNumPE; i++){
        ecmdclsint_array[i] = (master_dclsint_sig[i]->read() | checker_dclsint_sig[i]->read());
    }
    //interrupt (maskable and non-maskable)
    bool ecmti_pe_array[emNumPE];
    for(unsigned int i = 0; i < emNumPE; i++){
        ecmti_pe_array[i] = (master_sgati_pe_sig[i]->read() | checker_sgati_pe_sig[i]->read());
    }
    bool ecmtnmi_value = (master_sgatnmi_sig.read()) | (checker_sgatnmi_sig.read());

    if (DumpInterrupt){
        for(unsigned int i=0; i < 8; i++){
            if (mECMTI_PE[i] != ecmti_pe_array[i]){
                if (ecmti_pe_array[i]){
                    re_printf("info","INT [%s: ECMTI_PE%d] Assert.\n", this->basename(), i+1);
                }else{
                    re_printf("info","INT [%s: ECMTI_PE%d] De-assert.\n", this->basename(), i+1);
                }
            }
        }
        if (mECMTNMI != ecmtnmi_value){
            if (ecmtnmi_value){
                re_printf("info","INT [%s: ECMTNMI] Assert.\n", this->basename());
            }else{
                re_printf("info","INT [%s: ECMTNMI] De-assert.\n", this->basename());
            }
        }
    }
    for(unsigned int i= 0; i < 8; i++){
        mECMDCLSINT[i] = ecmdclsint_array[i];
        mECMTI_PE[i] = ecmti_pe_array[i];
    }
    mECMTNMI = ecmtnmi_value;
    //error output
    mECMTERROZ = (master_sgaterroz_sig.read()) & (checker_sgaterroz_sig.read());
    // DCLS INT
    //safety reset
    mECMTRESZ = (master_sgatresz_sig.read()) & (checker_sgatresz_sig.read());

    //assign value to dtmtrgin of Master and Checker
    dtmtrgout_sig.write((master_trgout_sig.read()) | (checker_trgout_sig.read()));

    if ((master_sgaterroz_sig.read()) == (checker_sgaterroz_sig.read())){
        mSgoterrin = false;
    }else{
        re_printf("warning","The error output signals from Master and Checker are different.\n");
        mSgoterrin = true;
    }
    mWriteOutputPortEvent.notify();
    mComparatorTestEvent.notify();
}//}}}

void Cecm_e2_wp::ComparatorTestMethod(void)
{//{{{
    master_sgaterrin308_sig.write((mSgoterrin || master_sgatpe_sig[308]->read()) && !master_ecmterrin308msk_m.read());
    checker_sgaterrin308_sig.write((mSgoterrin || master_sgatpe_sig[308]->read()) && !master_ecmterrin308msk_c.read()); 
}//}}}

void Cecm_e2_wp::WriteOutputPortMethod(void)
{//{{{ 
    ecmtnmi.write(mECMTNMI);
    ecmterroz.write(mECMTERROZ);
    ecmterroutz.write(ecmterroutz_m.read() & ecmterroutz_c.read());
    ecmtresz.write(mECMTRESZ);
    for(unsigned int i= 0; i < emNumPE; i++){
        ecmti_pe[i]->write(mECMTI_PE[i]);
        ecmdclsint[i]->write(mECMDCLSINT[i]);
    }
}//}}}

void Cecm_e2_wp::ExternalErrorInputTableConstruction(void)
{//{{{
    for(unsigned int i = 0; i < emNumErrorFactor - 1; i++){//from 0 to 307
        std::ostringstream str_ecmterrin;
        str_ecmterrin<<"ecmterrin"<<i;
        mExternalErrorInputTable.push_back(ExternalErrorInputElement(ecmterrin[i],str_ecmterrin.str(),false));
    }
}//}}}

void Cecm_e2_wp::DumpStatInfo(void)
{//{{{
    const char *type = "StatInfo";
    DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
    DumpInfo(type, "  ECMTERROUTZ: %d\n", ecmterroutz.read());
    DumpInfo(type, "  ECMTERROUTZ_M: %d\n", ecmterroutz_m.read());
    DumpInfo(type, "  ECMTERROUTZ_C: %d\n", ecmterroutz_c.read());
    DumpInfo(type, "  ECMTERROZ: %d\n", ecmterroz.read());
    for(unsigned int i = 0; i < emNumPE; i++){
        DumpInfo(type, "  ECMTI_PE%d: %d\n", i, ecmti_pe[i]->read());
    }
    DumpInfo(type, "  ECMTNMI: %d\n", ecmtnmi.read());
    DumpInfo(type, "  ECMTRESZ: %d\n", ecmtresz.read());
    for(unsigned int i = 0; i < emNumPE; i++){
        DumpInfo(type, "  ECMDCLSINT%d: %d\n", i, ecmdclsint[i]->read());
    }
    DumpInfo(type, "EndInfo.\n");
}//}}}

void Cecm_e2_wp::DumpEnableTransInfo()
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

void Cecm_e2_wp::UpdateState(eState prev_state, eState next_state)
{//{{{
    if (prev_state != next_state){
        re_printf("info","ECM_WP state is changed from %s state to %s state.\n", mStateArray[prev_state].c_str(), mStateArray[next_state].c_str());
        mState = next_state;
    }
}//}}}

void Cecm_e2_wp::ConnectPort()
{//{{{
    mMaster->erroutresz(erroutresz);
    mChecker->erroutresz(erroutresz);
    mMaster->sgaterroz(master_sgaterroz_sig);
    mMaster->sgatnmi(master_sgatnmi_sig);
    mMaster->sgatresz(master_sgatresz_sig);
    mChecker->sgaterroz(checker_sgaterroz_sig);
    mChecker->sgatnmi(checker_sgatnmi_sig);
    mChecker->sgatresz(checker_sgatresz_sig);
    mMaster->sgaterroutz(this->ecmterroutz_m);
    mChecker->sgaterroutz(this->ecmterroutz_c);

    mChecker->dtmtrgout(checker_trgout_sig);
    mMaster->dtmtrgout(master_trgout_sig);
    mMaster->dtmtrgin(dtmtrgout_sig);
    mChecker->dtmtrgin(dtmtrgout_sig);
    mMaster->errout_clear_mask_out(errout_clear_mask_sig);
    mMaster->errout_clear_mask_in(errout_clear_mask_sig);
    mChecker->errout_clear_mask_in(errout_clear_mask_sig);
    mChecker->errout_clear_mask_out(errout_clear_mask_checker);
    mMaster->pseudo_compare_msk_m(master_ecmterrin308msk_m);
    mMaster->pseudo_compare_msk_c(master_ecmterrin308msk_c);
    mChecker->pseudo_compare_msk_m(checker_ecmterrin308msk_m);
    mChecker->pseudo_compare_msk_c(checker_ecmterrin308msk_c);

    mMaster->sgattin(ecmttin);
    mChecker->sgattin(ecmttin);
    mMaster->sgaterrlbz(ecmterrlbz_m);
    mChecker->sgaterrlbz(ecmterrlbz_c);
    mMaster->resstg1z(resstg1z);
    mChecker->resstg1z(resstg1z);
    mMaster->resstg1z_pclkin(pclkin_resstg1z);
    mChecker->resstg1z_pclkin(pclkin_resstg1z);
    mMaster->erroutresz_for_sync(erroutresz_for_sync);
    mChecker->erroutresz_for_sync(erroutresz_for_sync);
    // connect mMaster->sgaterrinN(sgaterrinN_sig) (N = 00..308)
    for(unsigned int i = 0; i < emNumErrorFactor - 1; i++){//307
        mMaster->sgaterrin[i]->bind(*sgaterrin_sig[i]);
    }
    mMaster->sgaterrin[308]->bind(master_sgaterrin308_sig);

    // connect mChecker->sgaterrinN(sgaterrinN_sig) (N = 00..308)
    for(unsigned int i = 0; i < emNumErrorFactor - 1; i++){//307
        mChecker->sgaterrin[i]->bind(*sgaterrin_sig[i]);
    }
    mChecker->sgaterrin[308]->bind(checker_sgaterrin308_sig);

    //connect mMaster->sgatpeN(master_sgatpeN_sig) (N = 000.. 308)
    for(unsigned int i = 0; i < emNumErrorFactor; i++){//308
        mMaster->sgatpe[i]->bind(*master_sgatpe_sig[i]);
    }

    //connect mChecker->sgatpeN(checker_sgatpeN_sig) (N = 000.. 308)
    for(unsigned int i = 0; i < emNumErrorFactor; i++){//308
        mChecker->sgatpe[i]->bind(*checker_sgatpe_sig[i]);
    }

    // Connect dclsint, ecmti_pe 
    for(unsigned int i = 0; i < emNumPE; i++){
        mMaster->errout_pe[i]->bind(*master_dclsint_sig[i]);
        mChecker->errout_pe[i]->bind(*checker_dclsint_sig[i]);
        mMaster->sgati_pe[i]->bind(*master_sgati_pe_sig[i]);
        mChecker->sgati_pe[i]->bind(*checker_sgati_pe_sig[i]);
    }
}//}}}

void Cecm_e2_wp::InitializeInternalSignal()
{//{{{
    mSgoterrin               = false;
    master_sgaterrin308_sig = 0;
    checker_sgaterrin308_sig = 0;

    master_sgaterroz_sig    = false;
    master_sgatnmi_sig      = false;
    master_sgatresz_sig     = true;
    checker_sgaterroz_sig   = false;
    checker_sgatnmi_sig     = false;
    checker_sgatresz_sig    = true;
    errout_clear_mask_sig   = true;
    errout_clear_mask_checker = true;
    dtmtrgout_sig = false;
}//}}}

void Cecm_e2_wp::DumpInfo(const char *type, const char *message,...)
{//{{{
    if ((message != NULL) && (type != NULL)){
        printf ("PROFILE(%s): ECM_WP: ", type);
        //Print body
        va_list argptr;
        va_start(argptr, message);
        vprintf(message, argptr);
        va_end(argptr);
    }
}//}}}

void Cecm_e2_wp::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
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
        if(id == emCommonId){
            memset(p_data, 0, size);
            status = mMaster->ECM_common_tgt_rd(false, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
        }else if(id == emMasterId){
            memset(p_data, 0, size);
            status = mMaster->ECM_common_tgt_rd(false, (unsigned int)addr, p_data, size);
            if ((((unsigned int)addr & 0xFF) == 0x00) || (((unsigned int)addr & 0xFF) == 0x04)) {
                for(unsigned int index = 1; index < size; index++) {
                    p_data[index] = 0;
                }
            }
        }else{
            memset(p_data, 0, size);
            status = mChecker->ECM_common_tgt_rd(false, (unsigned int)addr, p_data, size);
            if ((((unsigned int)addr & 0xFF) == 0x00) || (((unsigned int)addr & 0xFF) == 0x04)) {
                for(unsigned int index = 1; index < size; index++) {
                    p_data[index] = 0;
                }
            }
        }
    //Write access    
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        if(id == emCommonId){
            bool temp1 = mMaster->ECM_common_tgt_wr(false, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            bool temp2 = mChecker->ECM_common_tgt_wr(false, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            status = temp1 && temp2;
        }else if(id == emMasterId){
            status = mMaster->ECM_common_tgt_wr(false, (unsigned int)addr, p_data, size);
        }else{
            status = mChecker->ECM_common_tgt_wr(false, (unsigned int)addr, p_data, size);
        }  
    //No write/read access    
    } else {
        status = true;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE); 
}//}}}

unsigned int Cecm_e2_wp::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
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
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        if(id == emCommonId){
            bool temp1 = mMaster->ECM_common_tgt_wr(true, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            bool temp2 = mChecker->ECM_common_tgt_wr(true, ((unsigned int)addr + emOffsetOfCommon), p_data, size);
            status = temp1 && temp2;
        }else if(id == emMasterId){
            status = mMaster->ECM_common_tgt_wr(true, (unsigned int)addr, p_data, size);
        }else{
            status = mChecker->ECM_common_tgt_wr(true, (unsigned int)addr, p_data, size);
        }    
    //No read/write access    
    } else {
        status = true;
        size+= 0;
    }
    
    trans.set_response_status(tlm::TLM_OK_RESPONSE); 
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

double Cecm_e2_wp::GetTimeResolution(void)
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

void Cecm_e2_wp::SetCLKfreq(std::string clk_name, double clk_freq)
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
   
    re_printf("info","The %s is set with a frequency as %lld.%06d\n",clk_name.c_str(), (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));

}//}}}

void Cecm_e2_wp::SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor)
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
