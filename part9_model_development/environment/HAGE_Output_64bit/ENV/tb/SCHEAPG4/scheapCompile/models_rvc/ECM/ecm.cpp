// ----------------------------------------------------------------------
// $Id$
// 
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ----------------------------------------------------------------------

#include "ecm.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//===============Cecm class=======================
//Constructor of Cecm class
Cecm::Cecm(sc_module_name name):  
     Cecm_regif((std::string)name,32)
    ,Cgeneral_timer(name)
    ,preset_n("preset_n")
    ,pclk("pclk")
    ,erroutresz("erroutresz")
    ,cntclk("cntclk")
    //sgaterrinN (N = 00 .. 92)
    //{{{
    ,sgaterrin00("sgaterrin00")
    ,sgaterrin01("sgaterrin01")
    ,sgaterrin02("sgaterrin02")
    ,sgaterrin03("sgaterrin03")
    ,sgaterrin04("sgaterrin04")
    ,sgaterrin05("sgaterrin05")
    ,sgaterrin06("sgaterrin06")
    ,sgaterrin07("sgaterrin07")
    ,sgaterrin08("sgaterrin08")
    ,sgaterrin09("sgaterrin09")
    ,sgaterrin10("sgaterrin10")
    ,sgaterrin11("sgaterrin11")
    ,sgaterrin12("sgaterrin12")
    ,sgaterrin13("sgaterrin13")
    ,sgaterrin14("sgaterrin14")
    ,sgaterrin15("sgaterrin15")
    ,sgaterrin16("sgaterrin16")
    ,sgaterrin17("sgaterrin17")
    ,sgaterrin18("sgaterrin18")
    ,sgaterrin19("sgaterrin19")
    ,sgaterrin20("sgaterrin20")
    ,sgaterrin21("sgaterrin21")
    ,sgaterrin22("sgaterrin22")
    ,sgaterrin23("sgaterrin23")
    ,sgaterrin24("sgaterrin24")
    ,sgaterrin25("sgaterrin25")
    ,sgaterrin26("sgaterrin26")
    ,sgaterrin27("sgaterrin27")
    ,sgaterrin28("sgaterrin28")
    ,sgaterrin29("sgaterrin29")
    ,sgaterrin30("sgaterrin30")
    ,sgaterrin31("sgaterrin31")
    ,sgaterrin32("sgaterrin32")
    ,sgaterrin33("sgaterrin33")
    ,sgaterrin34("sgaterrin34")
    ,sgaterrin35("sgaterrin35")
    ,sgaterrin36("sgaterrin36")
    ,sgaterrin37("sgaterrin37")
    ,sgaterrin38("sgaterrin38")
    ,sgaterrin39("sgaterrin39")
    ,sgaterrin40("sgaterrin40")
    ,sgaterrin41("sgaterrin41")
    ,sgaterrin42("sgaterrin42")
    ,sgaterrin43("sgaterrin43")
    ,sgaterrin44("sgaterrin44")
    ,sgaterrin45("sgaterrin45")
    ,sgaterrin46("sgaterrin46")
    ,sgaterrin47("sgaterrin47")
    ,sgaterrin48("sgaterrin48")
    ,sgaterrin49("sgaterrin49")
    ,sgaterrin50("sgaterrin50")
    ,sgaterrin51("sgaterrin51")
    ,sgaterrin52("sgaterrin52")
    ,sgaterrin53("sgaterrin53")
    ,sgaterrin54("sgaterrin54")
    ,sgaterrin55("sgaterrin55")
    ,sgaterrin56("sgaterrin56")
    ,sgaterrin57("sgaterrin57")
    ,sgaterrin58("sgaterrin58")
    ,sgaterrin59("sgaterrin59")
    ,sgaterrin60("sgaterrin60")
    ,sgaterrin61("sgaterrin61")
    ,sgaterrin62("sgaterrin62")
    ,sgaterrin63("sgaterrin63")
    ,sgaterrin64("sgaterrin64")
    ,sgaterrin65("sgaterrin65")
    ,sgaterrin66("sgaterrin66")
    ,sgaterrin67("sgaterrin67")
    ,sgaterrin68("sgaterrin68")
    ,sgaterrin69("sgaterrin69")
    ,sgaterrin70("sgaterrin70")
    ,sgaterrin71("sgaterrin71")
    ,sgaterrin72("sgaterrin72")
    ,sgaterrin73("sgaterrin73")
    ,sgaterrin74("sgaterrin74")
    ,sgaterrin75("sgaterrin75")
    ,sgaterrin76("sgaterrin76")
    ,sgaterrin77("sgaterrin77")
    ,sgaterrin78("sgaterrin78")
    ,sgaterrin79("sgaterrin79")
    ,sgaterrin80("sgaterrin80")
    ,sgaterrin81("sgaterrin81")
    ,sgaterrin82("sgaterrin82")
    ,sgaterrin83("sgaterrin83")
    ,sgaterrin84("sgaterrin84")
    ,sgaterrin85("sgaterrin85")
    ,sgaterrin86("sgaterrin86")
    ,sgaterrin87("sgaterrin87")
    ,sgaterrin88("sgaterrin88")
    ,sgaterrin89("sgaterrin89")
    ,sgaterrin90("sgaterrin90")
    ,sgaterrin91("sgaterrin91")
    ,sgaterrin92("sgaterrin92")
    //}}}
    ,sgatpsin("sgatpsin")
    ,sgaterrlbz("sgaterrlbz")
    ,sgattin("sgattin")
    ,dtmtrgin("dtmtrgin")
    ,resstg1z("resstg1z")
    ,resstg1z_pclkin("resstg1z_pclkin")
    ,svaccess("svaccess")
    ,errout_clear_mask_in("errout_clear_mask_in")
    //sgatpeN (N = 000 .. 092)
    //{{{
    ,sgatpe000("sgatpe000") 
    ,sgatpe001("sgatpe001")
    ,sgatpe002("sgatpe002")
    ,sgatpe003("sgatpe003")
    ,sgatpe004("sgatpe004")
    ,sgatpe005("sgatpe005")
    ,sgatpe006("sgatpe006")
    ,sgatpe007("sgatpe007")
    ,sgatpe008("sgatpe008")
    ,sgatpe009("sgatpe009")
    ,sgatpe010("sgatpe010")
    ,sgatpe011("sgatpe011")
    ,sgatpe012("sgatpe012")
    ,sgatpe013("sgatpe013")
    ,sgatpe014("sgatpe014")
    ,sgatpe015("sgatpe015")
    ,sgatpe016("sgatpe016")
    ,sgatpe017("sgatpe017")
    ,sgatpe018("sgatpe018")
    ,sgatpe019("sgatpe019")
    ,sgatpe020("sgatpe020")
    ,sgatpe021("sgatpe021")
    ,sgatpe022("sgatpe022")
    ,sgatpe023("sgatpe023")
    ,sgatpe024("sgatpe024")
    ,sgatpe025("sgatpe025")
    ,sgatpe026("sgatpe026")
    ,sgatpe027("sgatpe027")
    ,sgatpe028("sgatpe028")
    ,sgatpe029("sgatpe029")
    ,sgatpe030("sgatpe030")
    ,sgatpe031("sgatpe031")
    ,sgatpe032("sgatpe032")
    ,sgatpe033("sgatpe033")
    ,sgatpe034("sgatpe034")
    ,sgatpe035("sgatpe035")
    ,sgatpe036("sgatpe036")
    ,sgatpe037("sgatpe037")
    ,sgatpe038("sgatpe038")
    ,sgatpe039("sgatpe039")
    ,sgatpe040("sgatpe040")
    ,sgatpe041("sgatpe041")
    ,sgatpe042("sgatpe042")
    ,sgatpe043("sgatpe043")
    ,sgatpe044("sgatpe044")
    ,sgatpe045("sgatpe045")
    ,sgatpe046("sgatpe046")
    ,sgatpe047("sgatpe047")
    ,sgatpe048("sgatpe048")
    ,sgatpe049("sgatpe049")
    ,sgatpe050("sgatpe050")
    ,sgatpe051("sgatpe051")
    ,sgatpe052("sgatpe052")
    ,sgatpe053("sgatpe053")
    ,sgatpe054("sgatpe054")
    ,sgatpe055("sgatpe055")
    ,sgatpe056("sgatpe056")
    ,sgatpe057("sgatpe057")
    ,sgatpe058("sgatpe058")
    ,sgatpe059("sgatpe059")
    ,sgatpe060("sgatpe060")
    ,sgatpe061("sgatpe061")
    ,sgatpe062("sgatpe062")
    ,sgatpe063("sgatpe063")
    ,sgatpe064("sgatpe064")
    ,sgatpe065("sgatpe065")
    ,sgatpe066("sgatpe066")
    ,sgatpe067("sgatpe067")
    ,sgatpe068("sgatpe068")
    ,sgatpe069("sgatpe069")
    ,sgatpe070("sgatpe070")
    ,sgatpe071("sgatpe071")
    ,sgatpe072("sgatpe072")
    ,sgatpe073("sgatpe073")
    ,sgatpe074("sgatpe074")
    ,sgatpe075("sgatpe075")
    ,sgatpe076("sgatpe076")
    ,sgatpe077("sgatpe077")
    ,sgatpe078("sgatpe078")
    ,sgatpe079("sgatpe079")
    ,sgatpe080("sgatpe080")
    ,sgatpe081("sgatpe081")
    ,sgatpe082("sgatpe082")
    ,sgatpe083("sgatpe083")
    ,sgatpe084("sgatpe084")
    ,sgatpe085("sgatpe085")
    ,sgatpe086("sgatpe086")
    ,sgatpe087("sgatpe087")
    ,sgatpe088("sgatpe088")
    ,sgatpe089("sgatpe089")
    ,sgatpe090("sgatpe090")
    ,sgatpe091("sgatpe091")
    ,sgatpe092("sgatpe092")
    //}}}
    ,sgatpsout("sgatpsout")
    ,sgaterroz("sgaterroz")
    ,sgaterroutz("sgaterroutz")
    ,sgati("sgati")
    ,sgatnmi("sgatnmi")
    ,sgatresz("sgatresz")
    ,dtmtrgout("dtmtrgout")
    ,errout_clear_mask_out("errout_clear_mask_out")
{//{{{
    CommandInit(this->name());

    Cecm_regif::set_instance_name(this->name());//set hierarchy name
    //initial output port
    //sgatpeN(N = 000 .. 092)
    //{{{
    sgatpe000.initialize(false);
    sgatpe001.initialize(false);
    sgatpe002.initialize(false);
    sgatpe003.initialize(false);
    sgatpe004.initialize(false);
    sgatpe005.initialize(false);
    sgatpe006.initialize(false);
    sgatpe007.initialize(false);
    sgatpe008.initialize(false);
    sgatpe009.initialize(false);
    sgatpe010.initialize(false);
    sgatpe011.initialize(false);
    sgatpe012.initialize(false);
    sgatpe013.initialize(false);
    sgatpe014.initialize(false);
    sgatpe015.initialize(false);
    sgatpe016.initialize(false);
    sgatpe017.initialize(false);
    sgatpe018.initialize(false);
    sgatpe019.initialize(false);
    sgatpe020.initialize(false);
    sgatpe021.initialize(false);
    sgatpe022.initialize(false);
    sgatpe023.initialize(false);
    sgatpe024.initialize(false);
    sgatpe025.initialize(false);
    sgatpe026.initialize(false);
    sgatpe027.initialize(false);
    sgatpe028.initialize(false);
    sgatpe029.initialize(false);
    sgatpe030.initialize(false);
    sgatpe031.initialize(false);
    sgatpe032.initialize(false);
    sgatpe033.initialize(false);
    sgatpe034.initialize(false);
    sgatpe035.initialize(false);
    sgatpe036.initialize(false);
    sgatpe037.initialize(false);
    sgatpe038.initialize(false);
    sgatpe039.initialize(false);
    sgatpe040.initialize(false);
    sgatpe041.initialize(false);
    sgatpe042.initialize(false);
    sgatpe043.initialize(false);
    sgatpe044.initialize(false);
    sgatpe045.initialize(false);
    sgatpe046.initialize(false);
    sgatpe047.initialize(false);
    sgatpe048.initialize(false);
    sgatpe049.initialize(false);
    sgatpe050.initialize(false);
    sgatpe051.initialize(false);
    sgatpe052.initialize(false);
    sgatpe053.initialize(false);
    sgatpe054.initialize(false);
    sgatpe055.initialize(false);
    sgatpe056.initialize(false);
    sgatpe057.initialize(false);
    sgatpe058.initialize(false);
    sgatpe059.initialize(false);
    sgatpe060.initialize(false);
    sgatpe061.initialize(false);
    sgatpe062.initialize(false);
    sgatpe063.initialize(false);
    sgatpe064.initialize(false);
    sgatpe065.initialize(false);
    sgatpe066.initialize(false);
    sgatpe067.initialize(false);
    sgatpe068.initialize(false);
    sgatpe069.initialize(false);
    sgatpe070.initialize(false);
    sgatpe071.initialize(false);
    sgatpe072.initialize(false);
    sgatpe073.initialize(false);
    sgatpe074.initialize(false);
    sgatpe075.initialize(false);
    sgatpe076.initialize(false);
    sgatpe077.initialize(false);
    sgatpe078.initialize(false);
    sgatpe079.initialize(false);
    sgatpe080.initialize(false);
    sgatpe081.initialize(false);
    sgatpe082.initialize(false);
    sgatpe083.initialize(false);
    sgatpe084.initialize(false);
    sgatpe085.initialize(false);
    sgatpe086.initialize(false);
    sgatpe087.initialize(false);
    sgatpe088.initialize(false);
    sgatpe089.initialize(false);
    sgatpe090.initialize(false);
    sgatpe091.initialize(false);
    sgatpe092.initialize(false);
    //}}}
    sgatpsout.initialize(false);
    sgaterroz.initialize(false);
    sgaterroutz.initialize(false);
    sgati.initialize(false);
    sgatnmi.initialize(false);
    sgatresz.initialize(true);
    dtmtrgout.initialize(false);
    errout_clear_mask_out.initialize(true);
    //Initialize internal variables
    mNorFinishStep          = 0;
    mNorCurRegister         = "";
    mNorProtectedRegister   = "";
    mNorCurRegArea          = emNoneArea;
    mBreFinishStep          = 0;
    mBreCurRegister         = "";
    mBreProtectedRegister   = "";
    mBreCurRegArea          = emNoneArea;
    mRequestValue           = 0;
    mSyncSignal             = emECMErrorOutputActiveLevel;
    mSGATERROZ              = emECMErrorOutputActiveLevel;
    mSGATERROUTZ            = emECMErrorOutputActiveLevel;
    mSGATRESZ               = emECMInternalResetInactiveLevel;
    mSGATPSOUT              = false;
    mDTMTRGOUT              = false;
    mHoldSignal             = true;
    mRetartCounterTime      = 0;
    mCountClock             = 0;
    mPCLKClock              = 0;
    mSGATI                  = emECMInterruptInactiveLevel;
    mSGATNMI                = emECMInterruptInactiveLevel;
    mIsECMReset             = false;
    mECMEOUTCLRT            = 0;
    mECMmESSTR0             = 0;
    mECMmESSTR1             = 0;
    mECMmESSTR2             = 0;
    for(unsigned int i=0; i<emErrorFactor; i++){
        mPseudoErrorArray[i] = false;
    }
    //construct table
    ErrorInputTableConstruction();
    ProtectedRegisterTableConstruction();

    //Initialize mForcedPortArray
    mForcedPortArray.push_back(&sgati);
    mForcedPortArray.push_back(&sgatnmi);
    mForcedPortArray.push_back(&sgaterroz);
    mForcedPortArray.push_back(&sgatresz);
    mForcedPortArray.push_back(&dtmtrgout);

    //Initialize General Timer
    InitializeGeneralTimer();

    SC_METHOD(ECMReceiveErrorInputMethod)
    dont_initialize();
    //sgaterrinN (N = 00 .. 92)
    //{{{
    sensitive << sgaterrin00;
    sensitive << sgaterrin01;
    sensitive << sgaterrin02;
    sensitive << sgaterrin03;
    sensitive << sgaterrin04;
    sensitive << sgaterrin05;
    sensitive << sgaterrin06;
    sensitive << sgaterrin07;
    sensitive << sgaterrin08;
    sensitive << sgaterrin09;
    sensitive << sgaterrin10;
    sensitive << sgaterrin11;
    sensitive << sgaterrin12;
    sensitive << sgaterrin13;
    sensitive << sgaterrin14;
    sensitive << sgaterrin15;
    sensitive << sgaterrin16;
    sensitive << sgaterrin17;
    sensitive << sgaterrin18;
    sensitive << sgaterrin19;
    sensitive << sgaterrin20;
    sensitive << sgaterrin21;
    sensitive << sgaterrin22;
    sensitive << sgaterrin23;
    sensitive << sgaterrin24;
    sensitive << sgaterrin25;
    sensitive << sgaterrin26;
    sensitive << sgaterrin27;
    sensitive << sgaterrin28;
    sensitive << sgaterrin29;
    sensitive << sgaterrin30;
    sensitive << sgaterrin31;
    sensitive << sgaterrin32;
    sensitive << sgaterrin33;
    sensitive << sgaterrin34;
    sensitive << sgaterrin35;
    sensitive << sgaterrin36;
    sensitive << sgaterrin37;
    sensitive << sgaterrin38;
    sensitive << sgaterrin39;
    sensitive << sgaterrin40;
    sensitive << sgaterrin41;
    sensitive << sgaterrin42;
    sensitive << sgaterrin43;
    sensitive << sgaterrin44;
    sensitive << sgaterrin45;
    sensitive << sgaterrin46;
    sensitive << sgaterrin47;
    sensitive << sgaterrin48;
    sensitive << sgaterrin49;
    sensitive << sgaterrin50;
    sensitive << sgaterrin51;
    sensitive << sgaterrin52;
    sensitive << sgaterrin53;
    sensitive << sgaterrin54;
    sensitive << sgaterrin55;
    sensitive << sgaterrin56;
    sensitive << sgaterrin57;
    sensitive << sgaterrin58;
    sensitive << sgaterrin59;
    sensitive << sgaterrin60;
    sensitive << sgaterrin61;
    sensitive << sgaterrin62;
    sensitive << sgaterrin63;
    sensitive << sgaterrin64;
    sensitive << sgaterrin65;
    sensitive << sgaterrin66;
    sensitive << sgaterrin67;
    sensitive << sgaterrin68;
    sensitive << sgaterrin69;
    sensitive << sgaterrin70;
    sensitive << sgaterrin71;
    sensitive << sgaterrin72;
    sensitive << sgaterrin73;
    sensitive << sgaterrin74;
    sensitive << sgaterrin75;
    sensitive << sgaterrin76;
    sensitive << sgaterrin77;
    sensitive << sgaterrin78;
    sensitive << sgaterrin79;
    sensitive << sgaterrin80;
    sensitive << sgaterrin81;
    sensitive << sgaterrin82;
    sensitive << sgaterrin83;
    sensitive << sgaterrin84;
    sensitive << sgaterrin85;
    sensitive << sgaterrin86;
    sensitive << sgaterrin87;
    sensitive << sgaterrin88;
    sensitive << sgaterrin89;
    sensitive << sgaterrin90;
    sensitive << sgaterrin91;
    sensitive << sgaterrin92;
    //}}}

    SC_METHOD(LoopBackSignalProcessingMethod)
    dont_initialize();
    sensitive << sgaterrlbz;

    SC_METHOD(ClearBreakInfoMethod)
    dont_initialize();
    sensitive << svaccess;

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

    SC_METHOD(sgatpsinMethod)
    dont_initialize();
    sensitive << sgatpsin;

    SC_METHOD(WriteSGATERROZPortMethod)
    dont_initialize();
    sensitive << mWriteSGATERROZPortEvent;

    SC_METHOD(WriteSGATERROUTZPortMethod)
    dont_initialize();
    sensitive << mWriteSGATERROUTZPortEvent;

    SC_METHOD(WriteSGATRESZPortMethod)
    dont_initialize();
    sensitive << mWriteSGATRESZPortEvent;

    SC_METHOD(WriteSGATIPortMethod)
    dont_initialize();
    sensitive << mWriteSGATIPortEvent;

    SC_METHOD(WriteSGATNMIPortMethod)
    dont_initialize();
    sensitive << mWriteSGATNMIPortEvent;

    SC_METHOD(WriteSGATPSOUTPortMethod)
    dont_initialize();
    sensitive << mWriteSGATPSOUTPortEvent;

    SC_METHOD(WriteDTMTRGOUTPortMethod)
    dont_initialize();
    sensitive << mWriteDTMTRGOUTPortEvent;

    SC_METHOD(WriteErrOutMskClrPortMethod)
    dont_initialize();
    sensitive << mWriteErrOutMskClrPortEvent;

    SC_METHOD(DelayTimerConfigureMethod)
    dont_initialize();
    sensitive << dtmtrgin;

    SC_METHOD(ClearErrorStatusMethod)
    dont_initialize();
    sensitive << mClearErrorStatusEvent;

    SC_METHOD(EnableClearOutputErrMethod)
    dont_initialize();
    sensitive << mEnableClearOutputErrEvent;

    SC_METHOD(ResetForErrpinMethod)
    dont_initialize();
    sensitive << erroutresz;

    SC_METHOD(WritePseudoErrorOutMethod)
    dont_initialize();
    sensitive << mWritePseudoErrorOutEvent;

}//}}}

//Destructor of Cecm class
Cecm::~Cecm()
{//{{{
}//}}}

void Cecm::ErrorInputTableConstruction(void)
{//{{{
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin00,  &sgatpe000, false,  &(*ECMmESSTR0)["ECMmSSE000"], &(*ECMMICFG0)["ECMMIE000"], &(*ECMNMICFG0)["ECMNMIE000"], &(*ECMIRCFG0)["ECMIRE000"], &(*ECMEMK0)["ECMEMK000"], &(*ECMESSTC0)["ECMCLSSE000"], &(*ECMPE0)["ECMPE000"], &(*ECMDTMCFG0)["ECMDTMCFG000"], &(*ECMDTMCFG3)["ECMDTMCFG300"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin01,  &sgatpe001, false,  &(*ECMmESSTR0)["ECMmSSE001"], &(*ECMMICFG0)["ECMMIE001"], &(*ECMNMICFG0)["ECMNMIE001"], &(*ECMIRCFG0)["ECMIRE001"], &(*ECMEMK0)["ECMEMK001"], &(*ECMESSTC0)["ECMCLSSE001"], &(*ECMPE0)["ECMPE001"], &(*ECMDTMCFG0)["ECMDTMCFG001"], &(*ECMDTMCFG3)["ECMDTMCFG301"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin02,  &sgatpe002, false,  &(*ECMmESSTR0)["ECMmSSE002"], &(*ECMMICFG0)["ECMMIE002"], &(*ECMNMICFG0)["ECMNMIE002"], &(*ECMIRCFG0)["ECMIRE002"], &(*ECMEMK0)["ECMEMK002"], &(*ECMESSTC0)["ECMCLSSE002"], &(*ECMPE0)["ECMPE002"], &(*ECMDTMCFG0)["ECMDTMCFG002"], &(*ECMDTMCFG3)["ECMDTMCFG302"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin03,  &sgatpe003, false,  &(*ECMmESSTR0)["ECMmSSE003"], &(*ECMMICFG0)["ECMMIE003"], &(*ECMNMICFG0)["ECMNMIE003"], &(*ECMIRCFG0)["ECMIRE003"], &(*ECMEMK0)["ECMEMK003"], &(*ECMESSTC0)["ECMCLSSE003"], &(*ECMPE0)["ECMPE003"], &(*ECMDTMCFG0)["ECMDTMCFG003"], &(*ECMDTMCFG3)["ECMDTMCFG303"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin04,  &sgatpe004, false,  &(*ECMmESSTR0)["ECMmSSE004"], &(*ECMMICFG0)["ECMMIE004"], &(*ECMNMICFG0)["ECMNMIE004"], &(*ECMIRCFG0)["ECMIRE004"], &(*ECMEMK0)["ECMEMK004"], &(*ECMESSTC0)["ECMCLSSE004"], &(*ECMPE0)["ECMPE004"], &(*ECMDTMCFG0)["ECMDTMCFG004"], &(*ECMDTMCFG3)["ECMDTMCFG304"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin05,  &sgatpe005, false,  &(*ECMmESSTR0)["ECMmSSE005"], &(*ECMMICFG0)["ECMMIE005"], &(*ECMNMICFG0)["ECMNMIE005"], &(*ECMIRCFG0)["ECMIRE005"], &(*ECMEMK0)["ECMEMK005"], &(*ECMESSTC0)["ECMCLSSE005"], &(*ECMPE0)["ECMPE005"], &(*ECMDTMCFG0)["ECMDTMCFG005"], &(*ECMDTMCFG3)["ECMDTMCFG305"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin06,  &sgatpe006, false,  &(*ECMmESSTR0)["ECMmSSE006"], &(*ECMMICFG0)["ECMMIE006"], &(*ECMNMICFG0)["ECMNMIE006"], &(*ECMIRCFG0)["ECMIRE006"], &(*ECMEMK0)["ECMEMK006"], &(*ECMESSTC0)["ECMCLSSE006"], &(*ECMPE0)["ECMPE006"], &(*ECMDTMCFG0)["ECMDTMCFG006"], &(*ECMDTMCFG3)["ECMDTMCFG306"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin07,  &sgatpe007, false,  &(*ECMmESSTR0)["ECMmSSE007"], &(*ECMMICFG0)["ECMMIE007"], &(*ECMNMICFG0)["ECMNMIE007"], &(*ECMIRCFG0)["ECMIRE007"], &(*ECMEMK0)["ECMEMK007"], &(*ECMESSTC0)["ECMCLSSE007"], &(*ECMPE0)["ECMPE007"], &(*ECMDTMCFG0)["ECMDTMCFG007"], &(*ECMDTMCFG3)["ECMDTMCFG307"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin08,  &sgatpe008, false,  &(*ECMmESSTR0)["ECMmSSE008"], &(*ECMMICFG0)["ECMMIE008"], &(*ECMNMICFG0)["ECMNMIE008"], &(*ECMIRCFG0)["ECMIRE008"], &(*ECMEMK0)["ECMEMK008"], &(*ECMESSTC0)["ECMCLSSE008"], &(*ECMPE0)["ECMPE008"], &(*ECMDTMCFG0)["ECMDTMCFG008"], &(*ECMDTMCFG3)["ECMDTMCFG308"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin09,  &sgatpe009, false,  &(*ECMmESSTR0)["ECMmSSE009"], &(*ECMMICFG0)["ECMMIE009"], &(*ECMNMICFG0)["ECMNMIE009"], &(*ECMIRCFG0)["ECMIRE009"], &(*ECMEMK0)["ECMEMK009"], &(*ECMESSTC0)["ECMCLSSE009"], &(*ECMPE0)["ECMPE009"], &(*ECMDTMCFG0)["ECMDTMCFG009"], &(*ECMDTMCFG3)["ECMDTMCFG309"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin10,  &sgatpe010, false,  &(*ECMmESSTR0)["ECMmSSE010"], &(*ECMMICFG0)["ECMMIE010"], &(*ECMNMICFG0)["ECMNMIE010"], &(*ECMIRCFG0)["ECMIRE010"], &(*ECMEMK0)["ECMEMK010"], &(*ECMESSTC0)["ECMCLSSE010"], &(*ECMPE0)["ECMPE010"], &(*ECMDTMCFG0)["ECMDTMCFG010"], &(*ECMDTMCFG3)["ECMDTMCFG310"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin11,  &sgatpe011, false,  &(*ECMmESSTR0)["ECMmSSE011"], &(*ECMMICFG0)["ECMMIE011"], &(*ECMNMICFG0)["ECMNMIE011"], &(*ECMIRCFG0)["ECMIRE011"], &(*ECMEMK0)["ECMEMK011"], &(*ECMESSTC0)["ECMCLSSE011"], &(*ECMPE0)["ECMPE011"], &(*ECMDTMCFG0)["ECMDTMCFG011"], &(*ECMDTMCFG3)["ECMDTMCFG311"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin12,  &sgatpe012, false,  &(*ECMmESSTR0)["ECMmSSE012"], &(*ECMMICFG0)["ECMMIE012"], &(*ECMNMICFG0)["ECMNMIE012"], &(*ECMIRCFG0)["ECMIRE012"], &(*ECMEMK0)["ECMEMK012"], &(*ECMESSTC0)["ECMCLSSE012"], &(*ECMPE0)["ECMPE012"], &(*ECMDTMCFG0)["ECMDTMCFG012"], &(*ECMDTMCFG3)["ECMDTMCFG312"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin13,  &sgatpe013, false,  &(*ECMmESSTR0)["ECMmSSE013"], &(*ECMMICFG0)["ECMMIE013"], &(*ECMNMICFG0)["ECMNMIE013"], &(*ECMIRCFG0)["ECMIRE013"], &(*ECMEMK0)["ECMEMK013"], &(*ECMESSTC0)["ECMCLSSE013"], &(*ECMPE0)["ECMPE013"], &(*ECMDTMCFG0)["ECMDTMCFG013"], &(*ECMDTMCFG3)["ECMDTMCFG313"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin14,  &sgatpe014, false,  &(*ECMmESSTR0)["ECMmSSE014"], &(*ECMMICFG0)["ECMMIE014"], &(*ECMNMICFG0)["ECMNMIE014"], &(*ECMIRCFG0)["ECMIRE014"], &(*ECMEMK0)["ECMEMK014"], &(*ECMESSTC0)["ECMCLSSE014"], &(*ECMPE0)["ECMPE014"], &(*ECMDTMCFG0)["ECMDTMCFG014"], &(*ECMDTMCFG3)["ECMDTMCFG314"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin15,  &sgatpe015, false,  &(*ECMmESSTR0)["ECMmSSE015"], &(*ECMMICFG0)["ECMMIE015"], &(*ECMNMICFG0)["ECMNMIE015"], &(*ECMIRCFG0)["ECMIRE015"], &(*ECMEMK0)["ECMEMK015"], &(*ECMESSTC0)["ECMCLSSE015"], &(*ECMPE0)["ECMPE015"], &(*ECMDTMCFG0)["ECMDTMCFG015"], &(*ECMDTMCFG3)["ECMDTMCFG315"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin16,  &sgatpe016, false,  &(*ECMmESSTR0)["ECMmSSE016"], &(*ECMMICFG0)["ECMMIE016"], &(*ECMNMICFG0)["ECMNMIE016"], &(*ECMIRCFG0)["ECMIRE016"], &(*ECMEMK0)["ECMEMK016"], &(*ECMESSTC0)["ECMCLSSE016"], &(*ECMPE0)["ECMPE016"], &(*ECMDTMCFG0)["ECMDTMCFG016"], &(*ECMDTMCFG3)["ECMDTMCFG316"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin17,  &sgatpe017, false,  &(*ECMmESSTR0)["ECMmSSE017"], &(*ECMMICFG0)["ECMMIE017"], &(*ECMNMICFG0)["ECMNMIE017"], &(*ECMIRCFG0)["ECMIRE017"], &(*ECMEMK0)["ECMEMK017"], &(*ECMESSTC0)["ECMCLSSE017"], &(*ECMPE0)["ECMPE017"], &(*ECMDTMCFG0)["ECMDTMCFG017"], &(*ECMDTMCFG3)["ECMDTMCFG317"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin18,  &sgatpe018, false,  &(*ECMmESSTR0)["ECMmSSE018"], &(*ECMMICFG0)["ECMMIE018"], &(*ECMNMICFG0)["ECMNMIE018"], &(*ECMIRCFG0)["ECMIRE018"], &(*ECMEMK0)["ECMEMK018"], &(*ECMESSTC0)["ECMCLSSE018"], &(*ECMPE0)["ECMPE018"], &(*ECMDTMCFG0)["ECMDTMCFG018"], &(*ECMDTMCFG3)["ECMDTMCFG318"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin19,  &sgatpe019, false,  &(*ECMmESSTR0)["ECMmSSE019"], &(*ECMMICFG0)["ECMMIE019"], &(*ECMNMICFG0)["ECMNMIE019"], &(*ECMIRCFG0)["ECMIRE019"], &(*ECMEMK0)["ECMEMK019"], &(*ECMESSTC0)["ECMCLSSE019"], &(*ECMPE0)["ECMPE019"], &(*ECMDTMCFG0)["ECMDTMCFG019"], &(*ECMDTMCFG3)["ECMDTMCFG319"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin20,  &sgatpe020, false,  &(*ECMmESSTR0)["ECMmSSE020"], &(*ECMMICFG0)["ECMMIE020"], &(*ECMNMICFG0)["ECMNMIE020"], &(*ECMIRCFG0)["ECMIRE020"], &(*ECMEMK0)["ECMEMK020"], &(*ECMESSTC0)["ECMCLSSE020"], &(*ECMPE0)["ECMPE020"], &(*ECMDTMCFG0)["ECMDTMCFG020"], &(*ECMDTMCFG3)["ECMDTMCFG320"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin21,  &sgatpe021, false,  &(*ECMmESSTR0)["ECMmSSE021"], &(*ECMMICFG0)["ECMMIE021"], &(*ECMNMICFG0)["ECMNMIE021"], &(*ECMIRCFG0)["ECMIRE021"], &(*ECMEMK0)["ECMEMK021"], &(*ECMESSTC0)["ECMCLSSE021"], &(*ECMPE0)["ECMPE021"], &(*ECMDTMCFG0)["ECMDTMCFG021"], &(*ECMDTMCFG3)["ECMDTMCFG321"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin22,  &sgatpe022, false,  &(*ECMmESSTR0)["ECMmSSE022"], &(*ECMMICFG0)["ECMMIE022"], &(*ECMNMICFG0)["ECMNMIE022"], &(*ECMIRCFG0)["ECMIRE022"], &(*ECMEMK0)["ECMEMK022"], &(*ECMESSTC0)["ECMCLSSE022"], &(*ECMPE0)["ECMPE022"], &(*ECMDTMCFG0)["ECMDTMCFG022"], &(*ECMDTMCFG3)["ECMDTMCFG322"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin23,  &sgatpe023, false,  &(*ECMmESSTR0)["ECMmSSE023"], &(*ECMMICFG0)["ECMMIE023"], &(*ECMNMICFG0)["ECMNMIE023"], &(*ECMIRCFG0)["ECMIRE023"], &(*ECMEMK0)["ECMEMK023"], &(*ECMESSTC0)["ECMCLSSE023"], &(*ECMPE0)["ECMPE023"], &(*ECMDTMCFG0)["ECMDTMCFG023"], &(*ECMDTMCFG3)["ECMDTMCFG323"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin24,  &sgatpe024, false,  &(*ECMmESSTR0)["ECMmSSE024"], &(*ECMMICFG0)["ECMMIE024"], &(*ECMNMICFG0)["ECMNMIE024"], &(*ECMIRCFG0)["ECMIRE024"], &(*ECMEMK0)["ECMEMK024"], &(*ECMESSTC0)["ECMCLSSE024"], &(*ECMPE0)["ECMPE024"], &(*ECMDTMCFG0)["ECMDTMCFG024"], &(*ECMDTMCFG3)["ECMDTMCFG324"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin25,  &sgatpe025, false,  &(*ECMmESSTR0)["ECMmSSE025"], &(*ECMMICFG0)["ECMMIE025"], &(*ECMNMICFG0)["ECMNMIE025"], &(*ECMIRCFG0)["ECMIRE025"], &(*ECMEMK0)["ECMEMK025"], &(*ECMESSTC0)["ECMCLSSE025"], &(*ECMPE0)["ECMPE025"], &(*ECMDTMCFG0)["ECMDTMCFG025"], &(*ECMDTMCFG3)["ECMDTMCFG325"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin26,  &sgatpe026, false,  &(*ECMmESSTR0)["ECMmSSE026"], &(*ECMMICFG0)["ECMMIE026"], &(*ECMNMICFG0)["ECMNMIE026"], &(*ECMIRCFG0)["ECMIRE026"], &(*ECMEMK0)["ECMEMK026"], &(*ECMESSTC0)["ECMCLSSE026"], &(*ECMPE0)["ECMPE026"], &(*ECMDTMCFG0)["ECMDTMCFG026"], &(*ECMDTMCFG3)["ECMDTMCFG326"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin27,  &sgatpe027, false,  &(*ECMmESSTR0)["ECMmSSE027"], &(*ECMMICFG0)["ECMMIE027"], &(*ECMNMICFG0)["ECMNMIE027"], &(*ECMIRCFG0)["ECMIRE027"], &(*ECMEMK0)["ECMEMK027"], &(*ECMESSTC0)["ECMCLSSE027"], &(*ECMPE0)["ECMPE027"], &(*ECMDTMCFG0)["ECMDTMCFG027"], &(*ECMDTMCFG3)["ECMDTMCFG327"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin28,  &sgatpe028, false,  &(*ECMmESSTR0)["ECMmSSE028"], &(*ECMMICFG0)["ECMMIE028"], &(*ECMNMICFG0)["ECMNMIE028"], &(*ECMIRCFG0)["ECMIRE028"], &(*ECMEMK0)["ECMEMK028"], &(*ECMESSTC0)["ECMCLSSE028"], &(*ECMPE0)["ECMPE028"], &(*ECMDTMCFG0)["ECMDTMCFG028"], &(*ECMDTMCFG3)["ECMDTMCFG328"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin29,  &sgatpe029, false,  &(*ECMmESSTR0)["ECMmSSE029"], &(*ECMMICFG0)["ECMMIE029"], &(*ECMNMICFG0)["ECMNMIE029"], &(*ECMIRCFG0)["ECMIRE029"], &(*ECMEMK0)["ECMEMK029"], &(*ECMESSTC0)["ECMCLSSE029"], &(*ECMPE0)["ECMPE029"], &(*ECMDTMCFG0)["ECMDTMCFG029"], &(*ECMDTMCFG3)["ECMDTMCFG329"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin30,  &sgatpe030, false,  &(*ECMmESSTR0)["ECMmSSE030"], &(*ECMMICFG0)["ECMMIE030"], &(*ECMNMICFG0)["ECMNMIE030"], &(*ECMIRCFG0)["ECMIRE030"], &(*ECMEMK0)["ECMEMK030"], &(*ECMESSTC0)["ECMCLSSE030"], &(*ECMPE0)["ECMPE030"], &(*ECMDTMCFG0)["ECMDTMCFG030"], &(*ECMDTMCFG3)["ECMDTMCFG330"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin31,  &sgatpe031, false,  &(*ECMmESSTR0)["ECMmSSE031"], &(*ECMMICFG0)["ECMMIE031"], &(*ECMNMICFG0)["ECMNMIE031"], &(*ECMIRCFG0)["ECMIRE031"], &(*ECMEMK0)["ECMEMK031"], &(*ECMESSTC0)["ECMCLSSE031"], &(*ECMPE0)["ECMPE031"], &(*ECMDTMCFG0)["ECMDTMCFG031"], &(*ECMDTMCFG3)["ECMDTMCFG331"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin32,  &sgatpe032, false,  &(*ECMmESSTR1)["ECMmSSE100"], &(*ECMMICFG1)["ECMMIE100"], &(*ECMNMICFG1)["ECMNMIE100"], &(*ECMIRCFG1)["ECMIRE100"], &(*ECMEMK1)["ECMEMK100"], &(*ECMESSTC1)["ECMCLSSE100"], &(*ECMPE1)["ECMPE100"], &(*ECMDTMCFG1)["ECMDTMCFG100"], &(*ECMDTMCFG4)["ECMDTMCFG400"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin33,  &sgatpe033, false,  &(*ECMmESSTR1)["ECMmSSE101"], &(*ECMMICFG1)["ECMMIE101"], &(*ECMNMICFG1)["ECMNMIE101"], &(*ECMIRCFG1)["ECMIRE101"], &(*ECMEMK1)["ECMEMK101"], &(*ECMESSTC1)["ECMCLSSE101"], &(*ECMPE1)["ECMPE101"], &(*ECMDTMCFG1)["ECMDTMCFG101"], &(*ECMDTMCFG4)["ECMDTMCFG401"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin34,  &sgatpe034, false,  &(*ECMmESSTR1)["ECMmSSE102"], &(*ECMMICFG1)["ECMMIE102"], &(*ECMNMICFG1)["ECMNMIE102"], &(*ECMIRCFG1)["ECMIRE102"], &(*ECMEMK1)["ECMEMK102"], &(*ECMESSTC1)["ECMCLSSE102"], &(*ECMPE1)["ECMPE102"], &(*ECMDTMCFG1)["ECMDTMCFG102"], &(*ECMDTMCFG4)["ECMDTMCFG402"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin35,  &sgatpe035, false,  &(*ECMmESSTR1)["ECMmSSE103"], &(*ECMMICFG1)["ECMMIE103"], &(*ECMNMICFG1)["ECMNMIE103"], &(*ECMIRCFG1)["ECMIRE103"], &(*ECMEMK1)["ECMEMK103"], &(*ECMESSTC1)["ECMCLSSE103"], &(*ECMPE1)["ECMPE103"], &(*ECMDTMCFG1)["ECMDTMCFG103"], &(*ECMDTMCFG4)["ECMDTMCFG403"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin36,  &sgatpe036, false,  &(*ECMmESSTR1)["ECMmSSE104"], &(*ECMMICFG1)["ECMMIE104"], &(*ECMNMICFG1)["ECMNMIE104"], &(*ECMIRCFG1)["ECMIRE104"], &(*ECMEMK1)["ECMEMK104"], &(*ECMESSTC1)["ECMCLSSE104"], &(*ECMPE1)["ECMPE104"], &(*ECMDTMCFG1)["ECMDTMCFG104"], &(*ECMDTMCFG4)["ECMDTMCFG404"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin37,  &sgatpe037, false,  &(*ECMmESSTR1)["ECMmSSE105"], &(*ECMMICFG1)["ECMMIE105"], &(*ECMNMICFG1)["ECMNMIE105"], &(*ECMIRCFG1)["ECMIRE105"], &(*ECMEMK1)["ECMEMK105"], &(*ECMESSTC1)["ECMCLSSE105"], &(*ECMPE1)["ECMPE105"], &(*ECMDTMCFG1)["ECMDTMCFG105"], &(*ECMDTMCFG4)["ECMDTMCFG405"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin38,  &sgatpe038, false,  &(*ECMmESSTR1)["ECMmSSE106"], &(*ECMMICFG1)["ECMMIE106"], &(*ECMNMICFG1)["ECMNMIE106"], &(*ECMIRCFG1)["ECMIRE106"], &(*ECMEMK1)["ECMEMK106"], &(*ECMESSTC1)["ECMCLSSE106"], &(*ECMPE1)["ECMPE106"], &(*ECMDTMCFG1)["ECMDTMCFG106"], &(*ECMDTMCFG4)["ECMDTMCFG406"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin39,  &sgatpe039, false,  &(*ECMmESSTR1)["ECMmSSE107"], &(*ECMMICFG1)["ECMMIE107"], &(*ECMNMICFG1)["ECMNMIE107"], &(*ECMIRCFG1)["ECMIRE107"], &(*ECMEMK1)["ECMEMK107"], &(*ECMESSTC1)["ECMCLSSE107"], &(*ECMPE1)["ECMPE107"], &(*ECMDTMCFG1)["ECMDTMCFG107"], &(*ECMDTMCFG4)["ECMDTMCFG407"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin40,  &sgatpe040, false,  &(*ECMmESSTR1)["ECMmSSE108"], &(*ECMMICFG1)["ECMMIE108"], &(*ECMNMICFG1)["ECMNMIE108"], &(*ECMIRCFG1)["ECMIRE108"], &(*ECMEMK1)["ECMEMK108"], &(*ECMESSTC1)["ECMCLSSE108"], &(*ECMPE1)["ECMPE108"], &(*ECMDTMCFG1)["ECMDTMCFG108"], &(*ECMDTMCFG4)["ECMDTMCFG408"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin41,  &sgatpe041, false,  &(*ECMmESSTR1)["ECMmSSE109"], &(*ECMMICFG1)["ECMMIE109"], &(*ECMNMICFG1)["ECMNMIE109"], &(*ECMIRCFG1)["ECMIRE109"], &(*ECMEMK1)["ECMEMK109"], &(*ECMESSTC1)["ECMCLSSE109"], &(*ECMPE1)["ECMPE109"], &(*ECMDTMCFG1)["ECMDTMCFG109"], &(*ECMDTMCFG4)["ECMDTMCFG409"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin42,  &sgatpe042, false,  &(*ECMmESSTR1)["ECMmSSE110"], &(*ECMMICFG1)["ECMMIE110"], &(*ECMNMICFG1)["ECMNMIE110"], &(*ECMIRCFG1)["ECMIRE110"], &(*ECMEMK1)["ECMEMK110"], &(*ECMESSTC1)["ECMCLSSE110"], &(*ECMPE1)["ECMPE110"], &(*ECMDTMCFG1)["ECMDTMCFG110"], &(*ECMDTMCFG4)["ECMDTMCFG410"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin43,  &sgatpe043, false,  &(*ECMmESSTR1)["ECMmSSE111"], &(*ECMMICFG1)["ECMMIE111"], &(*ECMNMICFG1)["ECMNMIE111"], &(*ECMIRCFG1)["ECMIRE111"], &(*ECMEMK1)["ECMEMK111"], &(*ECMESSTC1)["ECMCLSSE111"], &(*ECMPE1)["ECMPE111"], &(*ECMDTMCFG1)["ECMDTMCFG111"], &(*ECMDTMCFG4)["ECMDTMCFG411"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin44,  &sgatpe044, false,  &(*ECMmESSTR1)["ECMmSSE112"], &(*ECMMICFG1)["ECMMIE112"], &(*ECMNMICFG1)["ECMNMIE112"], &(*ECMIRCFG1)["ECMIRE112"], &(*ECMEMK1)["ECMEMK112"], &(*ECMESSTC1)["ECMCLSSE112"], &(*ECMPE1)["ECMPE112"], &(*ECMDTMCFG1)["ECMDTMCFG112"], &(*ECMDTMCFG4)["ECMDTMCFG412"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin45,  &sgatpe045, false,  &(*ECMmESSTR1)["ECMmSSE113"], &(*ECMMICFG1)["ECMMIE113"], &(*ECMNMICFG1)["ECMNMIE113"], &(*ECMIRCFG1)["ECMIRE113"], &(*ECMEMK1)["ECMEMK113"], &(*ECMESSTC1)["ECMCLSSE113"], &(*ECMPE1)["ECMPE113"], &(*ECMDTMCFG1)["ECMDTMCFG113"], &(*ECMDTMCFG4)["ECMDTMCFG413"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin46,  &sgatpe046, false,  &(*ECMmESSTR1)["ECMmSSE114"], &(*ECMMICFG1)["ECMMIE114"], &(*ECMNMICFG1)["ECMNMIE114"], &(*ECMIRCFG1)["ECMIRE114"], &(*ECMEMK1)["ECMEMK114"], &(*ECMESSTC1)["ECMCLSSE114"], &(*ECMPE1)["ECMPE114"], &(*ECMDTMCFG1)["ECMDTMCFG114"], &(*ECMDTMCFG4)["ECMDTMCFG414"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin47,  &sgatpe047, false,  &(*ECMmESSTR1)["ECMmSSE115"], &(*ECMMICFG1)["ECMMIE115"], &(*ECMNMICFG1)["ECMNMIE115"], &(*ECMIRCFG1)["ECMIRE115"], &(*ECMEMK1)["ECMEMK115"], &(*ECMESSTC1)["ECMCLSSE115"], &(*ECMPE1)["ECMPE115"], &(*ECMDTMCFG1)["ECMDTMCFG115"], &(*ECMDTMCFG4)["ECMDTMCFG415"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin48,  &sgatpe048, false,  &(*ECMmESSTR1)["ECMmSSE116"], &(*ECMMICFG1)["ECMMIE116"], &(*ECMNMICFG1)["ECMNMIE116"], &(*ECMIRCFG1)["ECMIRE116"], &(*ECMEMK1)["ECMEMK116"], &(*ECMESSTC1)["ECMCLSSE116"], &(*ECMPE1)["ECMPE116"], &(*ECMDTMCFG1)["ECMDTMCFG116"], &(*ECMDTMCFG4)["ECMDTMCFG416"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin49,  &sgatpe049, false,  &(*ECMmESSTR1)["ECMmSSE117"], &(*ECMMICFG1)["ECMMIE117"], &(*ECMNMICFG1)["ECMNMIE117"], &(*ECMIRCFG1)["ECMIRE117"], &(*ECMEMK1)["ECMEMK117"], &(*ECMESSTC1)["ECMCLSSE117"], &(*ECMPE1)["ECMPE117"], &(*ECMDTMCFG1)["ECMDTMCFG117"], &(*ECMDTMCFG4)["ECMDTMCFG417"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin50,  &sgatpe050, false,  &(*ECMmESSTR1)["ECMmSSE118"], &(*ECMMICFG1)["ECMMIE118"], &(*ECMNMICFG1)["ECMNMIE118"], &(*ECMIRCFG1)["ECMIRE118"], &(*ECMEMK1)["ECMEMK118"], &(*ECMESSTC1)["ECMCLSSE118"], &(*ECMPE1)["ECMPE118"], &(*ECMDTMCFG1)["ECMDTMCFG118"], &(*ECMDTMCFG4)["ECMDTMCFG418"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin51,  &sgatpe051, false,  &(*ECMmESSTR1)["ECMmSSE119"], &(*ECMMICFG1)["ECMMIE119"], &(*ECMNMICFG1)["ECMNMIE119"], &(*ECMIRCFG1)["ECMIRE119"], &(*ECMEMK1)["ECMEMK119"], &(*ECMESSTC1)["ECMCLSSE119"], &(*ECMPE1)["ECMPE119"], &(*ECMDTMCFG1)["ECMDTMCFG119"], &(*ECMDTMCFG4)["ECMDTMCFG419"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin52,  &sgatpe052, false,  &(*ECMmESSTR1)["ECMmSSE120"], &(*ECMMICFG1)["ECMMIE120"], &(*ECMNMICFG1)["ECMNMIE120"], &(*ECMIRCFG1)["ECMIRE120"], &(*ECMEMK1)["ECMEMK120"], &(*ECMESSTC1)["ECMCLSSE120"], &(*ECMPE1)["ECMPE120"], &(*ECMDTMCFG1)["ECMDTMCFG120"], &(*ECMDTMCFG4)["ECMDTMCFG420"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin53,  &sgatpe053, false,  &(*ECMmESSTR1)["ECMmSSE121"], &(*ECMMICFG1)["ECMMIE121"], &(*ECMNMICFG1)["ECMNMIE121"], &(*ECMIRCFG1)["ECMIRE121"], &(*ECMEMK1)["ECMEMK121"], &(*ECMESSTC1)["ECMCLSSE121"], &(*ECMPE1)["ECMPE121"], &(*ECMDTMCFG1)["ECMDTMCFG121"], &(*ECMDTMCFG4)["ECMDTMCFG421"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin54,  &sgatpe054, false,  &(*ECMmESSTR1)["ECMmSSE122"], &(*ECMMICFG1)["ECMMIE122"], &(*ECMNMICFG1)["ECMNMIE122"], &(*ECMIRCFG1)["ECMIRE122"], &(*ECMEMK1)["ECMEMK122"], &(*ECMESSTC1)["ECMCLSSE122"], &(*ECMPE1)["ECMPE122"], &(*ECMDTMCFG1)["ECMDTMCFG122"], &(*ECMDTMCFG4)["ECMDTMCFG422"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin55,  &sgatpe055, false,  &(*ECMmESSTR1)["ECMmSSE123"], &(*ECMMICFG1)["ECMMIE123"], &(*ECMNMICFG1)["ECMNMIE123"], &(*ECMIRCFG1)["ECMIRE123"], &(*ECMEMK1)["ECMEMK123"], &(*ECMESSTC1)["ECMCLSSE123"], &(*ECMPE1)["ECMPE123"], &(*ECMDTMCFG1)["ECMDTMCFG123"], &(*ECMDTMCFG4)["ECMDTMCFG423"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin56,  &sgatpe056, false,  &(*ECMmESSTR1)["ECMmSSE124"], &(*ECMMICFG1)["ECMMIE124"], &(*ECMNMICFG1)["ECMNMIE124"], &(*ECMIRCFG1)["ECMIRE124"], &(*ECMEMK1)["ECMEMK124"], &(*ECMESSTC1)["ECMCLSSE124"], &(*ECMPE1)["ECMPE124"], &(*ECMDTMCFG1)["ECMDTMCFG124"], &(*ECMDTMCFG4)["ECMDTMCFG424"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin57,  &sgatpe057, false,  &(*ECMmESSTR1)["ECMmSSE125"], &(*ECMMICFG1)["ECMMIE125"], &(*ECMNMICFG1)["ECMNMIE125"], &(*ECMIRCFG1)["ECMIRE125"], &(*ECMEMK1)["ECMEMK125"], &(*ECMESSTC1)["ECMCLSSE125"], &(*ECMPE1)["ECMPE125"], &(*ECMDTMCFG1)["ECMDTMCFG125"], &(*ECMDTMCFG4)["ECMDTMCFG425"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin58,  &sgatpe058, false,  &(*ECMmESSTR1)["ECMmSSE126"], &(*ECMMICFG1)["ECMMIE126"], &(*ECMNMICFG1)["ECMNMIE126"], &(*ECMIRCFG1)["ECMIRE126"], &(*ECMEMK1)["ECMEMK126"], &(*ECMESSTC1)["ECMCLSSE126"], &(*ECMPE1)["ECMPE126"], &(*ECMDTMCFG1)["ECMDTMCFG126"], &(*ECMDTMCFG4)["ECMDTMCFG426"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin59,  &sgatpe059, false,  &(*ECMmESSTR1)["ECMmSSE127"], &(*ECMMICFG1)["ECMMIE127"], &(*ECMNMICFG1)["ECMNMIE127"], &(*ECMIRCFG1)["ECMIRE127"], &(*ECMEMK1)["ECMEMK127"], &(*ECMESSTC1)["ECMCLSSE127"], &(*ECMPE1)["ECMPE127"], &(*ECMDTMCFG1)["ECMDTMCFG127"], &(*ECMDTMCFG4)["ECMDTMCFG427"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin60,  &sgatpe060, false,  &(*ECMmESSTR1)["ECMmSSE128"], &(*ECMMICFG1)["ECMMIE128"], &(*ECMNMICFG1)["ECMNMIE128"], &(*ECMIRCFG1)["ECMIRE128"], &(*ECMEMK1)["ECMEMK128"], &(*ECMESSTC1)["ECMCLSSE128"], &(*ECMPE1)["ECMPE128"], &(*ECMDTMCFG1)["ECMDTMCFG128"], &(*ECMDTMCFG4)["ECMDTMCFG428"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin61,  &sgatpe061, false,  &(*ECMmESSTR1)["ECMmSSE129"], &(*ECMMICFG1)["ECMMIE129"], &(*ECMNMICFG1)["ECMNMIE129"], &(*ECMIRCFG1)["ECMIRE129"], &(*ECMEMK1)["ECMEMK129"], &(*ECMESSTC1)["ECMCLSSE129"], &(*ECMPE1)["ECMPE129"], &(*ECMDTMCFG1)["ECMDTMCFG129"], &(*ECMDTMCFG4)["ECMDTMCFG429"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin62,  &sgatpe062, false,  &(*ECMmESSTR1)["ECMmSSE130"], &(*ECMMICFG1)["ECMMIE130"], &(*ECMNMICFG1)["ECMNMIE130"], &(*ECMIRCFG1)["ECMIRE130"], &(*ECMEMK1)["ECMEMK130"], &(*ECMESSTC1)["ECMCLSSE130"], &(*ECMPE1)["ECMPE130"], &(*ECMDTMCFG1)["ECMDTMCFG130"], &(*ECMDTMCFG4)["ECMDTMCFG430"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin63,  &sgatpe063, false,  &(*ECMmESSTR1)["ECMmSSE131"], &(*ECMMICFG1)["ECMMIE131"], &(*ECMNMICFG1)["ECMNMIE131"], &(*ECMIRCFG1)["ECMIRE131"], &(*ECMEMK1)["ECMEMK131"], &(*ECMESSTC1)["ECMCLSSE131"], &(*ECMPE1)["ECMPE131"], &(*ECMDTMCFG1)["ECMDTMCFG131"], &(*ECMDTMCFG4)["ECMDTMCFG431"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin64,  &sgatpe064, false,  &(*ECMmESSTR2)["ECMmSSE200"], &(*ECMMICFG2)["ECMMIE200"], &(*ECMNMICFG2)["ECMNMIE200"], &(*ECMIRCFG2)["ECMIRE200"], &(*ECMEMK2)["ECMEMK200"], &(*ECMESSTC2)["ECMCLSSE200"], &(*ECMPE2)["ECMPE200"], &(*ECMDTMCFG2)["ECMDTMCFG200"], &(*ECMDTMCFG5)["ECMDTMCFG500"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin65,  &sgatpe065, false,  &(*ECMmESSTR2)["ECMmSSE201"], &(*ECMMICFG2)["ECMMIE201"], &(*ECMNMICFG2)["ECMNMIE201"], &(*ECMIRCFG2)["ECMIRE201"], &(*ECMEMK2)["ECMEMK201"], &(*ECMESSTC2)["ECMCLSSE201"], &(*ECMPE2)["ECMPE201"], &(*ECMDTMCFG2)["ECMDTMCFG201"], &(*ECMDTMCFG5)["ECMDTMCFG501"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin66,  &sgatpe066, false,  &(*ECMmESSTR2)["ECMmSSE202"], &(*ECMMICFG2)["ECMMIE202"], &(*ECMNMICFG2)["ECMNMIE202"], &(*ECMIRCFG2)["ECMIRE202"], &(*ECMEMK2)["ECMEMK202"], &(*ECMESSTC2)["ECMCLSSE202"], &(*ECMPE2)["ECMPE202"], &(*ECMDTMCFG2)["ECMDTMCFG202"], &(*ECMDTMCFG5)["ECMDTMCFG502"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin67,  &sgatpe067, false,  &(*ECMmESSTR2)["ECMmSSE203"], &(*ECMMICFG2)["ECMMIE203"], &(*ECMNMICFG2)["ECMNMIE203"], &(*ECMIRCFG2)["ECMIRE203"], &(*ECMEMK2)["ECMEMK203"], &(*ECMESSTC2)["ECMCLSSE203"], &(*ECMPE2)["ECMPE203"], &(*ECMDTMCFG2)["ECMDTMCFG203"], &(*ECMDTMCFG5)["ECMDTMCFG503"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin68,  &sgatpe068, false,  &(*ECMmESSTR2)["ECMmSSE204"], &(*ECMMICFG2)["ECMMIE204"], &(*ECMNMICFG2)["ECMNMIE204"], &(*ECMIRCFG2)["ECMIRE204"], &(*ECMEMK2)["ECMEMK204"], &(*ECMESSTC2)["ECMCLSSE204"], &(*ECMPE2)["ECMPE204"], &(*ECMDTMCFG2)["ECMDTMCFG204"], &(*ECMDTMCFG5)["ECMDTMCFG504"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin69,  &sgatpe069, false,  &(*ECMmESSTR2)["ECMmSSE205"], &(*ECMMICFG2)["ECMMIE205"], &(*ECMNMICFG2)["ECMNMIE205"], &(*ECMIRCFG2)["ECMIRE205"], &(*ECMEMK2)["ECMEMK205"], &(*ECMESSTC2)["ECMCLSSE205"], &(*ECMPE2)["ECMPE205"], &(*ECMDTMCFG2)["ECMDTMCFG205"], &(*ECMDTMCFG5)["ECMDTMCFG505"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin70,  &sgatpe070, false,  &(*ECMmESSTR2)["ECMmSSE206"], &(*ECMMICFG2)["ECMMIE206"], &(*ECMNMICFG2)["ECMNMIE206"], &(*ECMIRCFG2)["ECMIRE206"], &(*ECMEMK2)["ECMEMK206"], &(*ECMESSTC2)["ECMCLSSE206"], &(*ECMPE2)["ECMPE206"], &(*ECMDTMCFG2)["ECMDTMCFG206"], &(*ECMDTMCFG5)["ECMDTMCFG506"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin71,  &sgatpe071, false,  &(*ECMmESSTR2)["ECMmSSE207"], &(*ECMMICFG2)["ECMMIE207"], &(*ECMNMICFG2)["ECMNMIE207"], &(*ECMIRCFG2)["ECMIRE207"], &(*ECMEMK2)["ECMEMK207"], &(*ECMESSTC2)["ECMCLSSE207"], &(*ECMPE2)["ECMPE207"], &(*ECMDTMCFG2)["ECMDTMCFG207"], &(*ECMDTMCFG5)["ECMDTMCFG507"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin72,  &sgatpe072, false,  &(*ECMmESSTR2)["ECMmSSE208"], &(*ECMMICFG2)["ECMMIE208"], &(*ECMNMICFG2)["ECMNMIE208"], &(*ECMIRCFG2)["ECMIRE208"], &(*ECMEMK2)["ECMEMK208"], &(*ECMESSTC2)["ECMCLSSE208"], &(*ECMPE2)["ECMPE208"], &(*ECMDTMCFG2)["ECMDTMCFG208"], &(*ECMDTMCFG5)["ECMDTMCFG508"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin73,  &sgatpe073, false,  &(*ECMmESSTR2)["ECMmSSE209"], &(*ECMMICFG2)["ECMMIE209"], &(*ECMNMICFG2)["ECMNMIE209"], &(*ECMIRCFG2)["ECMIRE209"], &(*ECMEMK2)["ECMEMK209"], &(*ECMESSTC2)["ECMCLSSE209"], &(*ECMPE2)["ECMPE209"], &(*ECMDTMCFG2)["ECMDTMCFG209"], &(*ECMDTMCFG5)["ECMDTMCFG509"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin74,  &sgatpe074, false,  &(*ECMmESSTR2)["ECMmSSE210"], &(*ECMMICFG2)["ECMMIE210"], &(*ECMNMICFG2)["ECMNMIE210"], &(*ECMIRCFG2)["ECMIRE210"], &(*ECMEMK2)["ECMEMK210"], &(*ECMESSTC2)["ECMCLSSE210"], &(*ECMPE2)["ECMPE210"], &(*ECMDTMCFG2)["ECMDTMCFG210"], &(*ECMDTMCFG5)["ECMDTMCFG510"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin75,  &sgatpe075, false,  &(*ECMmESSTR2)["ECMmSSE211"], &(*ECMMICFG2)["ECMMIE211"], &(*ECMNMICFG2)["ECMNMIE211"], &(*ECMIRCFG2)["ECMIRE211"], &(*ECMEMK2)["ECMEMK211"], &(*ECMESSTC2)["ECMCLSSE211"], &(*ECMPE2)["ECMPE211"], &(*ECMDTMCFG2)["ECMDTMCFG211"], &(*ECMDTMCFG5)["ECMDTMCFG511"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin76,  &sgatpe076, false,  &(*ECMmESSTR2)["ECMmSSE212"], &(*ECMMICFG2)["ECMMIE212"], &(*ECMNMICFG2)["ECMNMIE212"], &(*ECMIRCFG2)["ECMIRE212"], &(*ECMEMK2)["ECMEMK212"], &(*ECMESSTC2)["ECMCLSSE212"], &(*ECMPE2)["ECMPE212"], &(*ECMDTMCFG2)["ECMDTMCFG212"], &(*ECMDTMCFG5)["ECMDTMCFG512"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin77,  &sgatpe077, false,  &(*ECMmESSTR2)["ECMmSSE213"], &(*ECMMICFG2)["ECMMIE213"], &(*ECMNMICFG2)["ECMNMIE213"], &(*ECMIRCFG2)["ECMIRE213"], &(*ECMEMK2)["ECMEMK213"], &(*ECMESSTC2)["ECMCLSSE213"], &(*ECMPE2)["ECMPE213"], &(*ECMDTMCFG2)["ECMDTMCFG213"], &(*ECMDTMCFG5)["ECMDTMCFG513"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin78,  &sgatpe078, false,  &(*ECMmESSTR2)["ECMmSSE214"], &(*ECMMICFG2)["ECMMIE214"], &(*ECMNMICFG2)["ECMNMIE214"], &(*ECMIRCFG2)["ECMIRE214"], &(*ECMEMK2)["ECMEMK214"], &(*ECMESSTC2)["ECMCLSSE214"], &(*ECMPE2)["ECMPE214"], &(*ECMDTMCFG2)["ECMDTMCFG214"], &(*ECMDTMCFG5)["ECMDTMCFG514"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin79,  &sgatpe079, false,  &(*ECMmESSTR2)["ECMmSSE215"], &(*ECMMICFG2)["ECMMIE215"], &(*ECMNMICFG2)["ECMNMIE215"], &(*ECMIRCFG2)["ECMIRE215"], &(*ECMEMK2)["ECMEMK215"], &(*ECMESSTC2)["ECMCLSSE215"], &(*ECMPE2)["ECMPE215"], &(*ECMDTMCFG2)["ECMDTMCFG215"], &(*ECMDTMCFG5)["ECMDTMCFG515"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin80,  &sgatpe080, false,  &(*ECMmESSTR2)["ECMmSSE216"], &(*ECMMICFG2)["ECMMIE216"], &(*ECMNMICFG2)["ECMNMIE216"], &(*ECMIRCFG2)["ECMIRE216"], &(*ECMEMK2)["ECMEMK216"], &(*ECMESSTC2)["ECMCLSSE216"], &(*ECMPE2)["ECMPE216"], &(*ECMDTMCFG2)["ECMDTMCFG216"], &(*ECMDTMCFG5)["ECMDTMCFG516"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin81,  &sgatpe081, false,  &(*ECMmESSTR2)["ECMmSSE217"], &(*ECMMICFG2)["ECMMIE217"], &(*ECMNMICFG2)["ECMNMIE217"], &(*ECMIRCFG2)["ECMIRE217"], &(*ECMEMK2)["ECMEMK217"], &(*ECMESSTC2)["ECMCLSSE217"], &(*ECMPE2)["ECMPE217"], &(*ECMDTMCFG2)["ECMDTMCFG217"], &(*ECMDTMCFG5)["ECMDTMCFG517"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin82,  &sgatpe082, false,  &(*ECMmESSTR2)["ECMmSSE218"], &(*ECMMICFG2)["ECMMIE218"], &(*ECMNMICFG2)["ECMNMIE218"], &(*ECMIRCFG2)["ECMIRE218"], &(*ECMEMK2)["ECMEMK218"], &(*ECMESSTC2)["ECMCLSSE218"], &(*ECMPE2)["ECMPE218"], &(*ECMDTMCFG2)["ECMDTMCFG218"], &(*ECMDTMCFG5)["ECMDTMCFG518"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin83,  &sgatpe083, false,  &(*ECMmESSTR2)["ECMmSSE219"], &(*ECMMICFG2)["ECMMIE219"], &(*ECMNMICFG2)["ECMNMIE219"], &(*ECMIRCFG2)["ECMIRE219"], &(*ECMEMK2)["ECMEMK219"], &(*ECMESSTC2)["ECMCLSSE219"], &(*ECMPE2)["ECMPE219"], &(*ECMDTMCFG2)["ECMDTMCFG219"], &(*ECMDTMCFG5)["ECMDTMCFG519"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin84,  &sgatpe084, false,  &(*ECMmESSTR2)["ECMmSSE220"], &(*ECMMICFG2)["ECMMIE220"], &(*ECMNMICFG2)["ECMNMIE220"], &(*ECMIRCFG2)["ECMIRE220"], &(*ECMEMK2)["ECMEMK220"], &(*ECMESSTC2)["ECMCLSSE220"], &(*ECMPE2)["ECMPE220"], &(*ECMDTMCFG2)["ECMDTMCFG220"], &(*ECMDTMCFG5)["ECMDTMCFG520"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin85,  &sgatpe085, false,  &(*ECMmESSTR2)["ECMmSSE221"], &(*ECMMICFG2)["ECMMIE221"], &(*ECMNMICFG2)["ECMNMIE221"], &(*ECMIRCFG2)["ECMIRE221"], &(*ECMEMK2)["ECMEMK221"], &(*ECMESSTC2)["ECMCLSSE221"], &(*ECMPE2)["ECMPE221"], &(*ECMDTMCFG2)["ECMDTMCFG221"], &(*ECMDTMCFG5)["ECMDTMCFG521"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin86,  &sgatpe086, false,  &(*ECMmESSTR2)["ECMmSSE222"], &(*ECMMICFG2)["ECMMIE222"], &(*ECMNMICFG2)["ECMNMIE222"], &(*ECMIRCFG2)["ECMIRE222"], &(*ECMEMK2)["ECMEMK222"], &(*ECMESSTC2)["ECMCLSSE222"], &(*ECMPE2)["ECMPE222"], &(*ECMDTMCFG2)["ECMDTMCFG222"], &(*ECMDTMCFG5)["ECMDTMCFG522"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin87,  &sgatpe087, false,  &(*ECMmESSTR2)["ECMmSSE223"], &(*ECMMICFG2)["ECMMIE223"], &(*ECMNMICFG2)["ECMNMIE223"], &(*ECMIRCFG2)["ECMIRE223"], &(*ECMEMK2)["ECMEMK223"], &(*ECMESSTC2)["ECMCLSSE223"], &(*ECMPE2)["ECMPE223"], &(*ECMDTMCFG2)["ECMDTMCFG223"], &(*ECMDTMCFG5)["ECMDTMCFG523"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin88,  &sgatpe088, false,  &(*ECMmESSTR2)["ECMmSSE224"], &(*ECMMICFG2)["ECMMIE224"], &(*ECMNMICFG2)["ECMNMIE224"], &(*ECMIRCFG2)["ECMIRE224"], &(*ECMEMK2)["ECMEMK224"], &(*ECMESSTC2)["ECMCLSSE224"], &(*ECMPE2)["ECMPE224"], &(*ECMDTMCFG2)["ECMDTMCFG224"], &(*ECMDTMCFG5)["ECMDTMCFG524"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin89,  &sgatpe089, false,  &(*ECMmESSTR2)["ECMmSSE225"], &(*ECMMICFG2)["ECMMIE225"], &(*ECMNMICFG2)["ECMNMIE225"], &(*ECMIRCFG2)["ECMIRE225"], &(*ECMEMK2)["ECMEMK225"], &(*ECMESSTC2)["ECMCLSSE225"], &(*ECMPE2)["ECMPE225"], &(*ECMDTMCFG2)["ECMDTMCFG225"], &(*ECMDTMCFG5)["ECMDTMCFG525"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin90,  &sgatpe090, false,  &(*ECMmESSTR2)["ECMmSSE226"], &(*ECMMICFG2)["ECMMIE226"], &(*ECMNMICFG2)["ECMNMIE226"], &(*ECMIRCFG2)["ECMIRE226"], &(*ECMEMK2)["ECMEMK226"], &(*ECMESSTC2)["ECMCLSSE226"], &(*ECMPE2)["ECMPE226"], &(*ECMDTMCFG2)["ECMDTMCFG226"], &(*ECMDTMCFG5)["ECMDTMCFG526"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin91,  &sgatpe091, false,  &(*ECMmESSTR2)["ECMmSSE227"], &(*ECMMICFG2)["ECMMIE227"], &(*ECMNMICFG2)["ECMNMIE227"], &(*ECMIRCFG2)["ECMIRE227"], &(*ECMEMK2)["ECMEMK227"], &(*ECMESSTC2)["ECMCLSSE227"], &(*ECMPE2)["ECMPE227"], &(*ECMDTMCFG2)["ECMDTMCFG227"], &(*ECMDTMCFG5)["ECMDTMCFG527"]));
    mErrorInputTable.push_back(ECMErrorInputElement(&sgaterrin92,  &sgatpe092, false,  &(*ECMmESSTR2)["ECMmSSE228"], &(*ECMMICFG2)["ECMMIE228"], &(*ECMNMICFG2)["ECMNMIE228"], &(*ECMIRCFG2)["ECMIRE228"], &(*ECMEMK2)["ECMEMK228"], &(*ECMESSTC2)["ECMCLSSE228"], &(*ECMPE2)["ECMPE228"], &(*ECMDTMCFG2)["ECMDTMCFG228"], &(*ECMDTMCFG5)["ECMDTMCFG528"]));
}//}}}

void Cecm::ProtectedRegisterTableConstruction(void)
{//{{{
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET",    0x000 , emMasterArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR",    0x004 , emMasterArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmPCMD0",   0x014 , emMasterArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmESET",    0x000 , emCheckerArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmECLR",    0x004 , emCheckerArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMmPCMD0",   0x014 , emCheckerArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEPCFG",    0x100 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0",   0x104 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0",   0x105 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0",   0x106 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG0",   0x107 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1",   0x108 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1",   0x109 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1",   0x10A , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG1",   0x10B , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2",   0x10C , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2",   0x10D , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2",   0x10E , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMMICFG2",   0x10F , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",  0x110 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",  0x111 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",  0x112 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG0",  0x113 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",  0x114 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",  0x115 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",  0x116 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG1",  0x117 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",  0x118 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",  0x119 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",  0x11A , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMNMICFG2",  0x11B , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0",   0x11C , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0",   0x11D , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0",   0x11E , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG0",   0x11F , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1",   0x120 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1",   0x121 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1",   0x122 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG1",   0x123 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2",   0x124 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2",   0x125 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2",   0x126 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMIRCFG2",   0x127 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",     0x128 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",     0x129 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",     0x12A , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK0",     0x12B , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",     0x12C , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",     0x12D , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",     0x12E , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK1",     0x12F , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",     0x130 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",     0x131 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",     0x132 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEMK2",     0x133 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC0",   0x134 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC1",   0x138 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMESSTC2",   0x13C , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPCMD1",    0x140 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE0",      0x148 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE1",      0x14C , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMPE2",      0x150 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCTL",   0x154 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCMP",   0x15C , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG0",  0x160 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG0",  0x161 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG0",  0x162 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG0",  0x163 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG1",  0x164 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG1",  0x165 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG1",  0x166 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG1",  0x167 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG2",  0x168 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG2",  0x169 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG2",  0x16A , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG2",  0x16B , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG3",  0x16C , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG3",  0x16D , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG3",  0x16E , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG3",  0x16F , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG4",  0x170 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG4",  0x171 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG4",  0x172 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG4",  0x173 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG5",  0x174 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG5",  0x175 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG5",  0x176 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMDTMCFG5",  0x177 , emCommonArea));
    mProtectedRegisterTable.push_back(ProtectedRegisterElement("ECMEOUTCLRT", 0x178 , emCommonArea));
}//}}}

void Cecm::ECMEnableReset(const bool is_active)
{//{{{
    mIsECMReset = is_active;
    if (is_active){
        //Cancel all event
        mWriteSGATERROZPortEvent.cancel();
        mWriteSGATERROUTZPortEvent.cancel();
        mWriteSGATRESZPortEvent.cancel();
        mWriteSGATIPortEvent.cancel();
        mWriteSGATNMIPortEvent.cancel();
        mWriteSGATPSOUTPortEvent.cancel();
        mWriteDTMTRGOUTPortEvent.cancel();
        mEnableClearOutputErrEvent.cancel();
        mCombineSignalEvent.cancel();

        //Initialize output ports
        mECMInitializeEvent.notify();

        //backup current value of ECMEOUTCLRT and ECMmESSTR0/1/2
        mECMEOUTCLRT = (unsigned int)(*ECMEOUTCLRT);    
        mECMmESSTR0  = (unsigned int)(*ECMmESSTR0);
        mECMmESSTR1  = (unsigned int)(*ECMmESSTR1);
        mECMmESSTR2  = (unsigned int)(*ECMmESSTR2);
       
        Cecm_regif::EnableReset(is_active);
        // added vupham 20150304 for issue trac #152
        (*ECMEOUTCLRT).EnableReset(false);
        (*ECMmESSTR0).EnableReset(false);
        (*ECMmESSTR1).EnableReset(false);
        (*ECMmESSTR2).EnableReset(false);
        // end added vupham
        
        (*ECMEOUTCLRT) = mECMEOUTCLRT;
        (*ECMmESSTR0)  = mECMmESSTR0; 
        (*ECMmESSTR1)  = mECMmESSTR1; 
        (*ECMmESSTR2)  = mECMmESSTR2; 
    }else{
        InitializeGeneralTimer();
        Cecm_regif::EnableReset(is_active);
    }

    //Call the reset function of common/general class
    Cgeneral_timer::EnableReset(is_active);
}//}}}

void Cecm::ECMInitializeMethod(void)
{//{{{
    //Initialize output port
    //sgatpeN (N = 000 .. 092)
    for(unsigned int i=0; i<emErrorFactor; i++){
        mPseudoErrorArray[i] = false;
    }
    mWritePseudoErrorOutEvent.notify();

    mSGATPSOUT = false;
    mWriteSGATPSOUTPortEvent.notify();
    mSGATI = emECMInterruptInactiveLevel;
    mWriteSGATIPortEvent.notify();
    mSGATNMI = emECMInterruptInactiveLevel;
    mWriteSGATNMIPortEvent.notify();
    mSGATRESZ = emECMInternalResetInactiveLevel;
    mWriteSGATRESZPortEvent.notify();
    mDTMTRGOUT = false;
    mWriteDTMTRGOUTPortEvent.notify();
    //Initialize internal variables
    mNorFinishStep          = 0;
    mNorCurRegister         = "";
    mNorProtectedRegister   = "";
    mNorCurRegArea          = emNoneArea;
    mBreFinishStep          = 0;
    mBreCurRegister         = "";
    mBreProtectedRegister   = "";
    mBreCurRegArea          =emNoneArea;
    mRequestValue           = 0;
}//}}}

void Cecm::ClearBreakInfoMethod(void)
{//{{{
    if (mIsECMReset || !ECMCheckZeroClock(true)){
        return;
    }
    if (svaccess.read()){
        mBreFinishStep = 0;
        mBreCurRegArea = emNoneArea;
        mBreCurRegister = "";
        mBreProtectedRegister = "";
    }
}//}}}

bool Cecm::ECMCheckZeroClock(const bool dump_warning)
{//{{{
    bool ret = true;
    if (mPCLKClock == 0) {
        if (dump_warning) {
            re_printf("warning","The pclk period is equal 0.\n");
        }
        ret = false;        
    }
    if (mCountClock == 0) {
        if (dump_warning) {
            re_printf("warning","The cntclk period is equal 0.\n");
        }
        ret = false;        
    }
    return ret;
}//}}}

bool Cecm::ECM_common_tgt_rd(const bool is_rd_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    if (is_rd_dbg){
        // Set debug mode (member variable of Cecm_regif class)
        return reg_rd_dbg(addr, p_data, size);
    }else{
        return reg_rd(addr, p_data, size);
    }
}//}}}

bool Cecm::ECM_common_tgt_wr(const bool is_wr_dbg, unsigned int addr, unsigned char *p_data, unsigned int size)
{//{{{
    if (is_wr_dbg){
        return reg_wr_dbg( addr,p_data, size);
    }else{
        unsigned int data = 0;
        sc_assert(p_data != NULL);
        memcpy(&data, p_data, size);

        //if write to a protected register or command register
        if(CheckProtectedRegister(addr)){
            if(WriteProtectedRegister(addr, data, size)){//check can write value
                return reg_wr(addr, p_data, size);
            }else{
                return true;
            }
        //if write to a normal register
        }else{
            return reg_wr(addr, p_data, size);
        }
    }
}//}}} 

void Cecm::cmpMatchOutputHandling (bool value)
{//{{{
    //turn on status of delay timer overflow
    (*ECMmESSTR2)["ECMmSSE229"] = 1;
    //Issue terminal error output
    IssueErrorOutput();
    //Issue internal reset
    IssueInternalReset();
}//}}}

void Cecm::ECMReceiveErrorInputMethod(void)
{//{{{
    if (!ECMCheckZeroClock(true)){
        return;
    }
    bool IsError = false;
    for (unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input
        if ((mErrorInputTable[error_index].errorin_pre == false) && (mErrorInputTable[error_index].errorin->read() == true)){
            IsError = true;          
        }
        if (mErrorInputTable[error_index].errorin_pre != mErrorInputTable[error_index].errorin->read()) {
            mErrorInputTable[error_index].errorin_pre = mErrorInputTable[error_index].errorin->read();
        }
    }
    IssueInterrupt();       //Issue interrupt signal
    UpdateErrorStatus();    //Update error status
    IssueErrorOutput();     //Issue error output
    if (IsError) {
        if (mSGATERROZ == emECMErrorOutputActiveLevel) {
            mEnableClearOutputErrEvent.cancel();
            mHoldSignal = false;
            mWriteErrOutMskClrPortEvent.notify();
            mRetartCounterTime = GetCurTime();
            mEnableClearOutputErrEvent.notify(mCountClock*((unsigned int)(*ECMEOUTCLRT)),SC_NS);
            mWriteSGATERROZPortEvent.notify();
            if ((unsigned int)(*ECMEPCFG)["ECMSL0"] == 0){ //non-dynamic mode
                mWriteSGATERROUTZPortEvent.notify();
            }
        }
    }
    IssueInternalReset();   //Issue internal reset
}//}}}

void Cecm::UpdateErrorStatus(void)
{//{{{
    // Check resstg1z_pclkin is active
    if (resstg1z_pclkin.read() == false){
        (*ECMmESSTR2)["ECMmSSE229"] = 0;
        for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
            (*mErrorInputTable[error_index].esstr) = 0;
        }
    }else{
        for(unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
            if (mErrorInputTable[error_index].errorin->read() == true) {
                (*mErrorInputTable[error_index].esstr) = 1;
                mPseudoErrorArray[error_index] = false; // negate pseudo error output
            }
        }
        mWritePseudoErrorOutEvent.notify(mCountClock,SC_NS);
    }
    // Check resstg1z is active
    if (resstg1z.read() == false){
        (*ECMmESSTR2)["ECMmSSE230"] = 0;
    }
}//}}}

void Cecm::IssueInterrupt(void)
{//{{{ 
    mSGATI = emECMInterruptInactiveLevel;
    mSGATNMI = emECMInterruptInactiveLevel;
    mDTMTRGOUT = false;
    for (unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input
        if (mErrorInputTable[error_index].errorin->read() == true){
            //Maskable interrupt
            if ((unsigned int)(*mErrorInputTable[error_index].micfg) == 1){
                mSGATI = emECMInterruptActiveLevel;
                if (((unsigned int)(*mErrorInputTable[error_index].midtmcfg) == 1)){
                    mDTMTRGOUT = true;
                }
            }
            //Non-maskable interrupt
            if ((unsigned int)(*mErrorInputTable[error_index].nmicfg) == 1){
                mSGATNMI = emECMInterruptActiveLevel;
                if (((unsigned int)(*mErrorInputTable[error_index].nmidtmcfg) == 1)){
                    mDTMTRGOUT = true;
                }
            }
        }
    }
    //write value to dtmtrgout
    mWriteDTMTRGOUTPortEvent.notify();
    //write maskable interrupt and non-maskable interrupt
    mWriteSGATIPortEvent.notify();
    mWriteSGATNMIPortEvent.notify();
}//}}}

void Cecm::DelayTimerConfigureMethod(void)
{//{{{
    if(dtmtrgin.read() && (((unsigned int)(*ECMDTMCTL) & 0x3) == 1)){ //if delay timer can start
        StartStopCounter(true, mCountClock);             //Start general timer
    }
}//}}}

void Cecm::IssueErrorOutput(void)
{//{{{
    CalculateErrorOutput();
    mWriteSGATERROZPortEvent.notify();
    //Non-dynamic mode, and there is no error input
    if ((mSGATERROUTZ == emECMErrorOutputActiveLevel) || ((unsigned int)(*ECMEPCFG)["ECMSL0"] == 0)){
        mWriteSGATERROUTZPortEvent.notify();
    }
}//}}}

void Cecm::CalculateErrorOutput(void)
{//{{{ 
    bool status_mask = emECMErrorOutputInactiveLevel;
    //There is delay timer overflow and None mask
    if (((unsigned int)(*ECMmESSTR2)["ECMmSSE229"] == 1) && ((unsigned int)(*ECMEMK2)["ECMEMK229"] == 0)){
        status_mask = emECMErrorOutputActiveLevel;
    }
    for (unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input and None mask
        if (((unsigned int)(*mErrorInputTable[error_index].esstr) == 1) && ((unsigned int)(*mErrorInputTable[error_index].emk) == 0)){
            status_mask = emECMErrorOutputActiveLevel;
            break;
        }
    }

    mSGATERROZ   = status_mask && mSyncSignal;
    mSGATERROUTZ = status_mask && mSyncSignal;
    mSyncSignal  = mSGATERROZ;
}//}}}

void Cecm::IssueInternalReset(void)
{//{{{
    mSGATRESZ = emECMInternalResetInactiveLevel;
    //There is pseudo-overflow and its internal reset is permited
    if (((unsigned int)(*ECMIRCFG2)["ECMIRE229"] == 1) && ((unsigned int)(*ECMmESSTR2)["ECMmSSE229"] == 1)){
        mSGATRESZ = emECMInternalResetActiveLevel;
    }
    for (unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input and internal reset is permited
        if ((mErrorInputTable[error_index].errorin->read() == true) && ((unsigned int)(*mErrorInputTable[error_index].ircfg) == 1)){
            mSGATRESZ = emECMInternalResetActiveLevel;
            break;
        }
    }
    mWriteSGATRESZPortEvent.notify();
}//}}}

void Cecm::LoopBackSignalProcessingMethod(void)
{//{{{
    if (mIsECMReset || !ECMCheckZeroClock(true)){
        return;
    }
    //if sgaterrlbz is asserted
    if (sgaterrlbz.read() == false){
        (*ECMmESSTR2)["ECMmSSE231"] = 0;
    //if sgaterrlbz is de-asserted
    }else{
        (*ECMmESSTR2)["ECMmSSE231"] = 1;
    }
}//}}}

void Cecm::resstg1zMethod(void)
{//{{{
    // This method NOT depend on preset_n. No need to check mIsECMReset
    //if resstg1z is active
    if(resstg1z.read() == false){
        (*ECMmESSTR2)["ECMmSSE230"] = 0;//clear error set triger status bit
    }
}//}}}

void Cecm::resstg1z_pclkinMethod(void)
{//{{{
    // This method NOT depend on preset_n. No need to check mIsECMReset
    //if resstg1z_pclkin is active
    if(resstg1z_pclkin.read() == false){
        mClearErrorStatusEvent.notify();
    }else{
        UpdateErrorStatus();
    }
}//}}}

void Cecm::sgattinMethod(void)
{//{{{
    if (mIsECMReset || !ECMCheckZeroClock(true)){
        return;
    }
    CalculateErrorOutput();
    //Dynamic, and there is no error input, toggle by timer input
    if (((unsigned int)(*ECMEPCFG)["ECMSL0"] == 1) && (mSGATERROUTZ == emECMErrorOutputInactiveLevel)){
        if (sgattin.read()){
            mSGATERROUTZ = emECMErrorOutputActiveLevel;
        }else{
            mSGATERROUTZ = emECMErrorOutputInactiveLevel;
        }
        mWriteSGATERROUTZPortEvent.notify();
    }
}//}}}

void Cecm::sgatpsinMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    if (sgatpsin.read()){   //active
        (*ECMPS)["ECMPRERR"] = 1;
    }
}//}}}

void Cecm::WriteSGATERROZPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    sgaterroz.write(mSGATERROZ);
}//}}}

void Cecm::WriteSGATERROUTZPortMethod(void)
{//{{{ 
    if (mIsECMReset){
        return;
    }
    sgaterroutz.write(mSGATERROUTZ);
}//}}}

void Cecm::WriteSGATRESZPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    sgatresz.write(mSGATRESZ);
}//}}}

void Cecm::WriteSGATIPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    sgati.write(mSGATI);
}//}}}

void Cecm::WriteSGATNMIPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    sgatnmi.write(mSGATNMI);
}//}}}

void Cecm::WriteSGATPSOUTPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    sgatpsout.write(mSGATPSOUT);
}//}}}

void Cecm::WriteDTMTRGOUTPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    dtmtrgout.write(mDTMTRGOUT);
}//}}}

void Cecm::WriteErrOutMskClrPortMethod(void)
{//{{{
    if (mIsECMReset){
        return;
    }
    errout_clear_mask_out.write(mHoldSignal);
}//}}}

bool Cecm::CheckProtectedRegister(unsigned int addr)
{//{{{
    addr = addr & 0x1FF;
    for (unsigned int index = 0; index < mProtectedRegisterTable.size(); index++){
        //COMMAND REGISTER or PROTECTED REGISTER
        if (addr == mProtectedRegisterTable[index].addr){
            return true;
        }
    }
    return false;
}//}}}

bool Cecm::WriteProtectedRegister(unsigned int addr, unsigned int data, unsigned int size)
{//{{{
    if (svaccess.read()){//protection sequence with BREAK
        return CheckBreakProtectionSequence(addr, data, size);
    }else{//NORMAL protection sequence
        return CheckNormalProtectionSequence(addr, data, size);
    }
}//}}}

bool Cecm::CheckBreakProtectionSequence(unsigned int addr, unsigned int data, unsigned int size)
{//{{{
    bool ret = false;
    addr = addr & 0x1FF;
    for (unsigned int index = 0; index < mProtectedRegisterTable.size(); index++){
        if (addr == mProtectedRegisterTable[index].addr){
            mBreCurRegister = mProtectedRegisterTable[index].register_name;
            //COMMAND REGISTER
            //Step 1
            if ((mProtectedRegisterTable[index].register_name == "ECMmPCMD0") || (mProtectedRegisterTable[index].register_name == "ECMPCMD1")){
                if ((mBreFinishStep == 0) || (mBreFinishStep == 1)){
                    mBreCurRegArea = mProtectedRegisterTable[index].register_area;
                    mBreFinishStep = 1;
                    ret = false;//The value of command register is not kept in break state of protection sequence
                    break;
                }else{
                    ret = ProtectionErrorProcess(true, data);
                    break;
                }
            //PROTECTED REGISTER
            }else{
                if (mBreFinishStep == 0){
                    mBreProtectedRegister = mProtectedRegisterTable[index].register_name;
                    ret = ProtectionErrorProcess(true, data);
                    break;
                //Step 2
                }else if (mBreFinishStep == 1){
                    if (mBreCurRegArea == mProtectedRegisterTable[index].register_area){
                        mBreFinishStep = 2;
                        mBreProtectedRegister = mProtectedRegisterTable[index].register_name;
                        ret = false;//This register does not store request value
                        break;
                    }else{
                        ret = ProtectionErrorProcess(true, data);
                        break;
                    }
                //Step 3
                }else if (mBreFinishStep == 2){
                    if (mBreCurRegArea == mProtectedRegisterTable[index].register_area){
                        if (mBreProtectedRegister == mProtectedRegisterTable[index].register_name){
                            mBreFinishStep = 3;
                            ret = false;//This register does not store request value
                            break;
                        }else{
                            ret = ProtectionErrorProcess(true, data);
                            break;
                        }
                    }else{
                        ret = ProtectionErrorProcess(true, data);
                        break;
                    }
                //Step 4
                }else{
                    if (mBreCurRegArea == mProtectedRegisterTable[index].register_area){
                        if (mBreProtectedRegister == mProtectedRegisterTable[index].register_name){
                            mBreFinishStep = 0;
                            mBreProtectedRegister = "";//release protected register
                            mBreCurRegArea = emNoneArea;
                            ret = true;//This register stores request value
                            break;
                        }else{
                            ret = ProtectionErrorProcess(true, data);
                            break;
                        }
                    }else{
                        ret = ProtectionErrorProcess(true, data);
                        break;
                    }
                }
            }
        }
    }
    return ret;
}//}}}

bool Cecm::CheckNormalProtectionSequence(unsigned int addr, unsigned int data, unsigned int size)
{//{{{  
    bool ret = false;
    addr = addr & 0x1FF;
    for (unsigned int index = 0; index < mProtectedRegisterTable.size(); index++){
        //COMMAND REGISTER or PROTECTED REGISTER
        if (addr == mProtectedRegisterTable[index].addr){
            mNorCurRegister = mProtectedRegisterTable[index].register_name;
            //COMMAND REGISTER
            //Step 1
            if ((mProtectedRegisterTable[index].register_name == "ECMmPCMD0") || (mProtectedRegisterTable[index].register_name == "ECMPCMD1")){
                if ((mNorFinishStep == 0) || (mNorFinishStep == 1)){
                    if (data == emFixCmdRegValue){
                        mNorCurRegArea = mProtectedRegisterTable[index].register_area;
                        mNorFinishStep = 1;
                    }
                    if (mProtectedRegisterTable[index].register_name == "ECMPCMD1") {
                        ret = false;
                    } else {
                        ret = true;
                    }
                    break;
                }else{
                    ret = ProtectionErrorProcess(false, data);
                    break;
                }
            //PROTECTED REGISTER
            }else{
                if (mNorFinishStep == 0){
                    mNorProtectedRegister = mProtectedRegisterTable[index].register_name;
                    ret = ProtectionErrorProcess(false, data);
                    break;
                //Step 2
                }else if (mNorFinishStep == 1){
                    mNorProtectedRegister = mProtectedRegisterTable[index].register_name;
                    if (mNorCurRegArea == mProtectedRegisterTable[index].register_area){
                        mRequestValue = data;
                        mNorFinishStep = 2;
                        ret = false;//This register does not store request value
                        break;
                    }else{
                        ret = ProtectionErrorProcess(false, data);
                        break;
                    }
                //Step 3
                }else if (mNorFinishStep == 2){
                    if (mNorCurRegArea == mProtectedRegisterTable[index].register_area){
                        if (mNorProtectedRegister == mProtectedRegisterTable[index].register_name){
                            unsigned int mask = (unsigned int)((0x1 << (size*8)) - 1);
                            if (size == 4){
                                mask = 0xFFFFFFFF;
                            }
                            if ((data & mask) == (~(mRequestValue) & mask)){
                                mNorFinishStep = 3;
                                ret = false;//This register does not store request value
                                break;
                            }else{
                                ret = ProtectionErrorProcess(false, data);
                                break;
                            }
                        }else{
                            ret = ProtectionErrorProcess(false, data);
                            break;
                        }
                    }else{
                        ret = ProtectionErrorProcess(false, data);
                        break;
                    }
                //Step 4
                }else{
                    if (mNorCurRegArea == mProtectedRegisterTable[index].register_area){
                        if (mNorProtectedRegister == mProtectedRegisterTable[index].register_name){
                            if (data == mRequestValue){
                                mNorFinishStep = 0;
                                mNorProtectedRegister = "";//release protected register
                                mNorCurRegArea = emNoneArea;
                                (*ECMPS)["ECMPRERR"] = 0;
                                mSGATPSOUT = false;
                                mWriteSGATPSOUTPortEvent.notify();
                                ret = true; //This register stores request value
                                break;
                            }else{
                                ret = ProtectionErrorProcess(false, data);
                                break;
                            }
                        }else{
                            ret = ProtectionErrorProcess(false, data);
                            break;
                        }
                    }else{
                        ret = ProtectionErrorProcess(false, data);
                        break;
                    }
                }
            }
        }
    }
    return ret;
}//}}}

bool Cecm::ProtectionErrorProcess(const bool is_break, unsigned int data)
{//{{{
    if (is_break){
        re_printf("warning","In break mode, write-protection sequence for %s fails at step %d when writing 0x%X to %s.\n",
        mBreProtectedRegister.c_str(), mBreFinishStep + 1, data, mBreCurRegister.c_str());

        mBreFinishStep = 0;
        mBreCurRegArea = emNoneArea;
        mBreCurRegister = "";
        mBreProtectedRegister = "";
    }else{
        re_printf("warning","In normal mode, write-protection sequence for %s fails at step %d when writing 0x%X to %s.\n",
        mNorProtectedRegister.c_str(), mNorFinishStep + 1, data, mNorCurRegister.c_str());

        mSGATPSOUT = true;
        mNorFinishStep = 0;
        mNorCurRegArea = emNoneArea;
        mNorCurRegister = "";

        (*ECMPS)["ECMPRERR"] = 1;
        mNorProtectedRegister = "";
        mWriteSGATPSOUTPortEvent.notify();
    }
    return false;
}//}}}  

void Cecm::StartStopCounter(const bool start, const double clk_period)
{//{{{
    (*ECMmESSTR2)["ECMmSSE229"] = 0;
    if (start){
        if (Cgeneral_timer::getCR().gtSTR) { // delay timer is counting
            return;
        } else if((unsigned int)(*ECMDTMCMP) > 0){
            Cgeneral_timer::setClkCountPeriod(clk_period);
            Cgeneral_timer::setCOR((unsigned int)(*ECMDTMCMP)); //Match value
            Cgeneral_timer::setSTR(true);
        } else { //compare match
            cmpMatchOutputHandling(true);
        }
    }else{
        Cgeneral_timer::setSTR(false);
    }
}//}}} 

void Cecm::InitializeGeneralTimer(void)
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

void Cecm::EnableClearOutputErrMethod(void)
{//{{{
    mHoldSignal = true;
    mWriteErrOutMskClrPortEvent.notify();
}//}}}

void Cecm::ECMSetZeroClock(void)
{//{{{
    //cancel all processes 
    mWriteSGATERROZPortEvent.cancel();
    mWriteSGATERROUTZPortEvent.cancel();
    mWriteSGATRESZPortEvent.cancel();
    mWriteSGATIPortEvent.cancel();
    mWriteSGATNMIPortEvent.cancel();
    mWriteSGATPSOUTPortEvent.cancel();
    mWriteDTMTRGOUTPortEvent.cancel();
    mEnableClearOutputErrEvent.cancel();
    mCombineSignalEvent.cancel();

    StartStopCounter(false, mCountClock); //Stop general timer
}//}}}

void Cecm::ResetForErrpinMethod(void)
{//{{{
    if (erroutresz.read() == false) {
        (*ECMEOUTCLRT) = 0; 
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

void Cecm::cb_ECMESSTC0_ECMCLSSE000(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        (*ECMESSTC0) = 0x0;
        return;
    }
    for(unsigned int error_index = 0; error_index < 32; error_index++){
        if (mErrorInputTable[error_index].errorin->read() == true) {
            (*mErrorInputTable[error_index].esstr) = 1;
        } else {
            if ((unsigned int)(*mErrorInputTable[error_index].esstc) == 1){
                (*mErrorInputTable[error_index].esstr) = 0;
            }
        }
    }
    (*ECMESSTC0) = 0x0;
    IssueErrorOutput();
    IssueInternalReset();
}//}}}

void Cecm::cb_ECMESSTC1_ECMCLSSE100(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        (*ECMESSTC1) = 0x0;
        return;
    }
    for(unsigned int error_index = 32; error_index < 64; error_index++){
        if (mErrorInputTable[error_index].errorin->read() == true) {
            *mErrorInputTable[error_index].esstr = 1;
        } else {
            if ((unsigned int)(*mErrorInputTable[error_index].esstc) == 1){
                *mErrorInputTable[error_index].esstr = 0;
            }
        }
    }
    (*ECMESSTC1) = 0x0;
    IssueErrorOutput();
    IssueInternalReset();
}//}}}

void Cecm::cb_ECMESSTC2_ECMCLSSE200(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        (*ECMESSTC2) = 0x0;
        return;
    }
    for(unsigned int error_index = 64; error_index < mErrorInputTable.size(); error_index++){
        if (mErrorInputTable[error_index].errorin->read() == true) {
            *mErrorInputTable[error_index].esstr = 1;
        } else {
            if ((unsigned int)(*mErrorInputTable[error_index].esstc) == 1){
                *mErrorInputTable[error_index].esstr = 0;
            }
        }
    }
    if ((*ECMESSTC2)["ECMCLSSE229"] == 1) {
        //clear delay timer overflow status bit
        (*ECMmESSTR2)["ECMmSSE229"] = 0;
    }
    if ((*ECMESSTC2)["ECMCLSSE230"] == 1) {
        //clear error set trigger status bit
        (*ECMmESSTR2)["ECMmSSE230"] = 0;
    }
    (*ECMESSTC2) = 0x0;
    IssueErrorOutput();
    IssueInternalReset();
}//}}}

void Cecm::cb_ECMMICFG0_ECMMIE000(RegCBstr str)
{//{{{ 
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMMICFG1_ECMMIE100(RegCBstr str)
{//{{{ 
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}} 

void Cecm::cb_ECMMICFG2_ECMMIE200(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMNMICFG0_ECMNMIE000(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMNMICFG1_ECMNMIE100(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMNMICFG2_ECMNMIE200(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMIRCFG0_ECMIRE000(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInternalReset();
}//}}}

void Cecm::cb_ECMIRCFG1_ECMIRE100(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInternalReset();
}//}}}

void Cecm::cb_ECMIRCFG2_ECMIRE200(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInternalReset();
}//}}}

void Cecm::cb_ECMEMK0_ECMEMK000(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueErrorOutput();
}//}}}

void Cecm::cb_ECMEMK1_ECMEMK100(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueErrorOutput();
}//}}}

void Cecm::cb_ECMEMK2_ECMEMK200(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueErrorOutput();
}//}}}

void Cecm::cb_ECMDTMCFG0_ECMDTMCFG000(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMDTMCFG1_ECMDTMCFG100(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMDTMCFG2_ECMDTMCFG200(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMDTMCFG3_ECMDTMCFG300(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMDTMCFG4_ECMDTMCFG400(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMDTMCFG5_ECMDTMCFG500(RegCBstr str)
{//{{{ 
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    IssueInterrupt();
}//}}}

void Cecm::cb_ECMmESET_ECMmEST(RegCBstr str)
{//{{{
    (*ECMmESET)["ECMmEST"] = 0;
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    //if resstg1z is still ACTIVE
    if (resstg1z.read() == false){
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
        (*ECMmESSTR2)["ECMmSSE230"] = 1;
    }
}//}}}

void Cecm::cb_ECMmECLR_ECMmECT(RegCBstr str)
{//{{{
    (*ECMmECLR)["ECMmECT"] = 0;
    if (!ECMCheckZeroClock(false)) {
        return;
    }

    // Check all error status are 0 or masked
    bool status_mask = emECMErrorOutputInactiveLevel;
    //There is delay timer overflow and None mask
    if (((unsigned int)(*ECMmESSTR2)["ECMmSSE229"] == 1) && ((unsigned int)(*ECMEMK2)["ECMEMK229"] == 0)){
        status_mask = emECMErrorOutputActiveLevel;
    }
    for (unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        //There is error input and None mask
        if (((unsigned int)(*mErrorInputTable[error_index].esstr) == 1) && ((unsigned int)(*mErrorInputTable[error_index].emk) == 0)){
            status_mask = emECMErrorOutputActiveLevel;
            break;
        }
    }

    if (((unsigned int)(*ECMmESSTR2)["ECMmSSE230"] == 0) && (errout_clear_mask_in.read()) && (status_mask == emECMErrorOutputInactiveLevel)){
        mSyncSignal = emECMErrorOutputInactiveLevel;
        IssueErrorOutput();
    }
}//}}}

void Cecm::ClearErrorStatusMethod(void)
{//{{{
    UpdateErrorStatus();
    IssueErrorOutput();
    IssueInternalReset();
}//}}}

void Cecm::cb_ECMDTMCTL_ECMSTP(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        (*ECMDTMCTL)["ECMSTA"] = 0;
        (*ECMDTMCTL)["ECMSTP"] = 0; // not hold value
        return;
    }
    //STP bit = 1
    if ((str.data & 0x2) == 0x2){
        StartStopCounter(false, mCountClock); //Stop general timer
        Cgeneral_timer::setCNT(0x0);             //Count value
        (*ECMDTMCTL)["ECMSTA"] = 0;
        (*ECMDTMCTL)["ECMSTP"] = 0; // not hold value
    //STP bit = 0
    }else{
        //if STA bit changes
        if ((str.data & 0x1) != (str.pre_data & 0x1)){
            //STA bit = 1
            if ((str.data & 0x1) == 1){
                if (dtmtrgin.read()){//consider dtmtrgin to start delay timer
                    StartStopCounter(true, mCountClock); //Start general timer
                }
            //STA bit = 0
            }else{
                StartStopCounter(false, mCountClock); //Stop general timer
            }
        }
    }
}//}}}

void Cecm::cb_ECMDTMCMP_ECMDTMCMP(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    if ((unsigned int)(*ECMDTMCTL)["ECMSTA"] == 1){
        re_printf("warning","Cannot write to ECMDTMCMP register when delay timer is counting.\n");
        (*ECMDTMCMP) = str.pre_data;
    }
}//}}}

void Cecm::cb_ECMDTMR_ECMDTMR(RegCBstr str)
{//{{{ 
    unsigned int value = Cgeneral_timer::getCNT();
    (*ECMDTMR) = value;
}//}}}

void Cecm::WritePseudoErrorOutMethod()
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    for (unsigned int error_index = 0; error_index < mErrorInputTable.size(); error_index++){
        mErrorInputTable[error_index].pseudo_error_out->write(mPseudoErrorArray[error_index]);
    }
}//}}}

void Cecm::cb_ECMPE0_ECMPE000(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        (*ECMPE0) = 0x0; //not hold value
        return;
    }
    //Issue pseudo error
    for (unsigned int error_index = 0; error_index < 32; error_index++){
        mPseudoErrorArray[error_index] = (bool)(*(mErrorInputTable[error_index].pe));
    }
    mWritePseudoErrorOutEvent.notify();
    (*ECMPE0) = 0x0; //not hold value
}//}}}

void Cecm::cb_ECMPE1_ECMPE100(RegCBstr str)
{//{{{ 
    if (!ECMCheckZeroClock(false)) {
        (*ECMPE1) = 0x0; //not hold value
        return;
    }
    //Issue pseudo error
    for (unsigned int error_index = 32; error_index < 64; error_index++){
        mPseudoErrorArray[error_index] = (bool)(*(mErrorInputTable[error_index].pe));
    }
    mWritePseudoErrorOutEvent.notify();
    (*ECMPE1) = 0x0; //not hold value
}//}}}

void Cecm::cb_ECMPE2_ECMPE200(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        (*ECMPE2) = 0x0; //not hold value
        return;
    }
    //There is pseudo-delay timer overflow if ECMPE229 bit equals 1
    if ((*ECMPE2)["ECMPE229"] == 1){
        cmpMatchOutputHandling(true);
    }
    //Issue pseudo error
    for (unsigned int error_index = 64; error_index < mErrorInputTable.size(); error_index++){
        mPseudoErrorArray[error_index] = (bool)(*(mErrorInputTable[error_index].pe));
    }
    mWritePseudoErrorOutEvent.notify();
    (*ECMPE2) = 0x0; //not hold value
}//}}}

void Cecm::cb_ECMEOUTCLRT_ECMEOUTCLRT(RegCBstr str)
{//{{{
    if (!ECMCheckZeroClock(false)) {
        return;
    }
    if (erroutresz.read() == false) {
        (*ECMEOUTCLRT) = 0; 
        return;
    }
    if (((unsigned int)(*ECMEOUTCLRT) != str.pre_data)&&(mHoldSignal == false)) {
        mEnableClearOutputErrEvent.cancel();
        double current_time = GetCurTime();
        double waited_time = current_time - mRetartCounterTime;
        double new_wait_time = (double)(mCountClock*((unsigned int)(*ECMEOUTCLRT)));
        if (new_wait_time <= waited_time) {
            mHoldSignal = true;
            mWriteErrOutMskClrPortEvent.notify();
        } else {
            double additional_wait = new_wait_time - waited_time;
            mEnableClearOutputErrEvent.notify(additional_wait, SC_NS);
        }
    }
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit SC_NS)
**********************************/
double Cecm::GetCurTime(void)
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

//===============Cecm_wp class=======================
//Constructor of Cecm_wp class
Cecm_wp::Cecm_wp(sc_module_name name):
         vpcl::tlm_tgt_if<32, tlm::tlm_base_protocol_types, 3> (name)
         //sgaterrinN_sig (N = 00 .. 92)
         //{{{
         ,sgaterrin00_sig("sgaterrin00_sig") 
         ,sgaterrin01_sig("sgaterrin01_sig")
         ,sgaterrin02_sig("sgaterrin02_sig")
         ,sgaterrin03_sig("sgaterrin03_sig")
         ,sgaterrin04_sig("sgaterrin04_sig")
         ,sgaterrin05_sig("sgaterrin05_sig")
         ,sgaterrin06_sig("sgaterrin06_sig")
         ,sgaterrin07_sig("sgaterrin07_sig")
         ,sgaterrin08_sig("sgaterrin08_sig")
         ,sgaterrin09_sig("sgaterrin09_sig")
         ,sgaterrin10_sig("sgaterrin10_sig")
         ,sgaterrin11_sig("sgaterrin11_sig")
         ,sgaterrin12_sig("sgaterrin12_sig")
         ,sgaterrin13_sig("sgaterrin13_sig")
         ,sgaterrin14_sig("sgaterrin14_sig")
         ,sgaterrin15_sig("sgaterrin15_sig")
         ,sgaterrin16_sig("sgaterrin16_sig")
         ,sgaterrin17_sig("sgaterrin17_sig")
         ,sgaterrin18_sig("sgaterrin18_sig")
         ,sgaterrin19_sig("sgaterrin19_sig")
         ,sgaterrin20_sig("sgaterrin20_sig")
         ,sgaterrin21_sig("sgaterrin21_sig")
         ,sgaterrin22_sig("sgaterrin22_sig")
         ,sgaterrin23_sig("sgaterrin23_sig")
         ,sgaterrin24_sig("sgaterrin24_sig")
         ,sgaterrin25_sig("sgaterrin25_sig")
         ,sgaterrin26_sig("sgaterrin26_sig")
         ,sgaterrin27_sig("sgaterrin27_sig")
         ,sgaterrin28_sig("sgaterrin28_sig")
         ,sgaterrin29_sig("sgaterrin29_sig")
         ,sgaterrin30_sig("sgaterrin30_sig")
         ,sgaterrin31_sig("sgaterrin31_sig")
         ,sgaterrin32_sig("sgaterrin32_sig")
         ,sgaterrin33_sig("sgaterrin33_sig")
         ,sgaterrin34_sig("sgaterrin34_sig")
         ,sgaterrin35_sig("sgaterrin35_sig")
         ,sgaterrin36_sig("sgaterrin36_sig")
         ,sgaterrin37_sig("sgaterrin37_sig")
         ,sgaterrin38_sig("sgaterrin38_sig")
         ,sgaterrin39_sig("sgaterrin39_sig")
         ,sgaterrin40_sig("sgaterrin40_sig")
         ,sgaterrin41_sig("sgaterrin41_sig")
         ,sgaterrin42_sig("sgaterrin42_sig")
         ,sgaterrin43_sig("sgaterrin43_sig")
         ,sgaterrin44_sig("sgaterrin44_sig")
         ,sgaterrin45_sig("sgaterrin45_sig")
         ,sgaterrin46_sig("sgaterrin46_sig")
         ,sgaterrin47_sig("sgaterrin47_sig")
         ,sgaterrin48_sig("sgaterrin48_sig")
         ,sgaterrin49_sig("sgaterrin49_sig")
         ,sgaterrin50_sig("sgaterrin50_sig")
         ,sgaterrin51_sig("sgaterrin51_sig")
         ,sgaterrin52_sig("sgaterrin52_sig")
         ,sgaterrin53_sig("sgaterrin53_sig")
         ,sgaterrin54_sig("sgaterrin54_sig")
         ,sgaterrin55_sig("sgaterrin55_sig")
         ,sgaterrin56_sig("sgaterrin56_sig")
         ,sgaterrin57_sig("sgaterrin57_sig")
         ,sgaterrin58_sig("sgaterrin58_sig")
         ,sgaterrin59_sig("sgaterrin59_sig")
         ,sgaterrin60_sig("sgaterrin60_sig")
         ,sgaterrin61_sig("sgaterrin61_sig")
         ,sgaterrin62_sig("sgaterrin62_sig")
         ,sgaterrin63_sig("sgaterrin63_sig")
         ,sgaterrin64_sig("sgaterrin64_sig")
         ,sgaterrin65_sig("sgaterrin65_sig")
         ,sgaterrin66_sig("sgaterrin66_sig")
         ,sgaterrin67_sig("sgaterrin67_sig")
         ,sgaterrin68_sig("sgaterrin68_sig")
         ,sgaterrin69_sig("sgaterrin69_sig")
         ,sgaterrin70_sig("sgaterrin70_sig")
         ,sgaterrin71_sig("sgaterrin71_sig")
         ,sgaterrin72_sig("sgaterrin72_sig")
         ,sgaterrin73_sig("sgaterrin73_sig")
         ,sgaterrin74_sig("sgaterrin74_sig")
         ,sgaterrin75_sig("sgaterrin75_sig")
         ,sgaterrin76_sig("sgaterrin76_sig")
         ,sgaterrin77_sig("sgaterrin77_sig")
         ,sgaterrin78_sig("sgaterrin78_sig")
         ,sgaterrin79_sig("sgaterrin79_sig")
         ,sgaterrin80_sig("sgaterrin80_sig")
         ,sgaterrin81_sig("sgaterrin81_sig")
         ,sgaterrin82_sig("sgaterrin82_sig")
         ,sgaterrin83_sig("sgaterrin83_sig")
         ,sgaterrin84_sig("sgaterrin84_sig")
         ,sgaterrin85_sig("sgaterrin85_sig")
         ,sgaterrin86_sig("sgaterrin86_sig")
         ,sgaterrin87_sig("sgaterrin87_sig")
         ,sgaterrin88_sig("sgaterrin88_sig")
         ,sgaterrin89_sig("sgaterrin89_sig")
         ,sgaterrin90_sig("sgaterrin90_sig")
         ,sgaterrin91_sig("sgaterrin91_sig")
         ,master_sgaterrin92_sig("master_sgaterrin92_sig") 
         ,checker_sgaterrin92_sig("checker_sgaterrin92_sig")  
         //}}}
         ,master_sgaterroz_sig("master_sgaterroz_sig")
         ,master_sgaterroutz_sig("master_sgaterroutz_sig")
         ,master_sgati_sig("master_sgati_sig")
         ,master_sgatnmi_sig("master_sgatnmi_sig")
         ,master_sgatresz_sig("master_sgatresz_sig")
         ,checker_sgaterroz_sig("checker_sgaterroz_sig")
         ,checker_sgaterroutz_sig("checker_sgaterroutz_sig")
         ,checker_sgati_sig("checker_sgati_sig")
         ,checker_sgatnmi_sig("checker_sgatnmi_sig")
         ,checker_sgatresz_sig("checker_sgatresz_sig")
         ,errout_clear_mask_sig("errout_clear_mask_sig")
         ,errout_clear_mask_checker("errout_clear_mask_checker")
         
         //master_sgatpeN_sig (N = 000 .. 092)
         //{{{
         ,master_sgatpe000_sig("master_sgatpe000_sig") 
         ,master_sgatpe001_sig("master_sgatpe001_sig")
         ,master_sgatpe002_sig("master_sgatpe002_sig")
         ,master_sgatpe003_sig("master_sgatpe003_sig")
         ,master_sgatpe004_sig("master_sgatpe004_sig")
         ,master_sgatpe005_sig("master_sgatpe005_sig")
         ,master_sgatpe006_sig("master_sgatpe006_sig")
         ,master_sgatpe007_sig("master_sgatpe007_sig")
         ,master_sgatpe008_sig("master_sgatpe008_sig")
         ,master_sgatpe009_sig("master_sgatpe009_sig")
         ,master_sgatpe010_sig("master_sgatpe010_sig")
         ,master_sgatpe011_sig("master_sgatpe011_sig")
         ,master_sgatpe012_sig("master_sgatpe012_sig")
         ,master_sgatpe013_sig("master_sgatpe013_sig")
         ,master_sgatpe014_sig("master_sgatpe014_sig")
         ,master_sgatpe015_sig("master_sgatpe015_sig")
         ,master_sgatpe016_sig("master_sgatpe016_sig")
         ,master_sgatpe017_sig("master_sgatpe017_sig")
         ,master_sgatpe018_sig("master_sgatpe018_sig")
         ,master_sgatpe019_sig("master_sgatpe019_sig")
         ,master_sgatpe020_sig("master_sgatpe020_sig")
         ,master_sgatpe021_sig("master_sgatpe021_sig")
         ,master_sgatpe022_sig("master_sgatpe022_sig")
         ,master_sgatpe023_sig("master_sgatpe023_sig")
         ,master_sgatpe024_sig("master_sgatpe024_sig")
         ,master_sgatpe025_sig("master_sgatpe025_sig")
         ,master_sgatpe026_sig("master_sgatpe026_sig")
         ,master_sgatpe027_sig("master_sgatpe027_sig")
         ,master_sgatpe028_sig("master_sgatpe028_sig")
         ,master_sgatpe029_sig("master_sgatpe029_sig")
         ,master_sgatpe030_sig("master_sgatpe030_sig")
         ,master_sgatpe031_sig("master_sgatpe031_sig")
         ,master_sgatpe032_sig("master_sgatpe032_sig")
         ,master_sgatpe033_sig("master_sgatpe033_sig")
         ,master_sgatpe034_sig("master_sgatpe034_sig")
         ,master_sgatpe035_sig("master_sgatpe035_sig")
         ,master_sgatpe036_sig("master_sgatpe036_sig")
         ,master_sgatpe037_sig("master_sgatpe037_sig")
         ,master_sgatpe038_sig("master_sgatpe038_sig")
         ,master_sgatpe039_sig("master_sgatpe039_sig")
         ,master_sgatpe040_sig("master_sgatpe040_sig")
         ,master_sgatpe041_sig("master_sgatpe041_sig")
         ,master_sgatpe042_sig("master_sgatpe042_sig")
         ,master_sgatpe043_sig("master_sgatpe043_sig")
         ,master_sgatpe044_sig("master_sgatpe044_sig")
         ,master_sgatpe045_sig("master_sgatpe045_sig")
         ,master_sgatpe046_sig("master_sgatpe046_sig")
         ,master_sgatpe047_sig("master_sgatpe047_sig")
         ,master_sgatpe048_sig("master_sgatpe048_sig")
         ,master_sgatpe049_sig("master_sgatpe049_sig")
         ,master_sgatpe050_sig("master_sgatpe050_sig")
         ,master_sgatpe051_sig("master_sgatpe051_sig")
         ,master_sgatpe052_sig("master_sgatpe052_sig")
         ,master_sgatpe053_sig("master_sgatpe053_sig")
         ,master_sgatpe054_sig("master_sgatpe054_sig")
         ,master_sgatpe055_sig("master_sgatpe055_sig")
         ,master_sgatpe056_sig("master_sgatpe056_sig")
         ,master_sgatpe057_sig("master_sgatpe057_sig")
         ,master_sgatpe058_sig("master_sgatpe058_sig")
         ,master_sgatpe059_sig("master_sgatpe059_sig")
         ,master_sgatpe060_sig("master_sgatpe060_sig")
         ,master_sgatpe061_sig("master_sgatpe061_sig")
         ,master_sgatpe062_sig("master_sgatpe062_sig")
         ,master_sgatpe063_sig("master_sgatpe063_sig")
         ,master_sgatpe064_sig("master_sgatpe064_sig")
         ,master_sgatpe065_sig("master_sgatpe065_sig")
         ,master_sgatpe066_sig("master_sgatpe066_sig")
         ,master_sgatpe067_sig("master_sgatpe067_sig")
         ,master_sgatpe068_sig("master_sgatpe068_sig")
         ,master_sgatpe069_sig("master_sgatpe069_sig")
         ,master_sgatpe070_sig("master_sgatpe070_sig")
         ,master_sgatpe071_sig("master_sgatpe071_sig")
         ,master_sgatpe072_sig("master_sgatpe072_sig")
         ,master_sgatpe073_sig("master_sgatpe073_sig")
         ,master_sgatpe074_sig("master_sgatpe074_sig")
         ,master_sgatpe075_sig("master_sgatpe075_sig")
         ,master_sgatpe076_sig("master_sgatpe076_sig")
         ,master_sgatpe077_sig("master_sgatpe077_sig")
         ,master_sgatpe078_sig("master_sgatpe078_sig")
         ,master_sgatpe079_sig("master_sgatpe079_sig")
         ,master_sgatpe080_sig("master_sgatpe080_sig")
         ,master_sgatpe081_sig("master_sgatpe081_sig")
         ,master_sgatpe082_sig("master_sgatpe082_sig")
         ,master_sgatpe083_sig("master_sgatpe083_sig")
         ,master_sgatpe084_sig("master_sgatpe084_sig")
         ,master_sgatpe085_sig("master_sgatpe085_sig")
         ,master_sgatpe086_sig("master_sgatpe086_sig")
         ,master_sgatpe087_sig("master_sgatpe087_sig")
         ,master_sgatpe088_sig("master_sgatpe088_sig")
         ,master_sgatpe089_sig("master_sgatpe089_sig")
         ,master_sgatpe090_sig("master_sgatpe090_sig")
         ,master_sgatpe091_sig("master_sgatpe091_sig")
         ,master_sgatpe092_sig("master_sgatpe092_sig")
         //}}}
         //checker_sgatpeN_sig (N = 000 .. 092)
         //{{{
         ,checker_sgatpe000_sig("checker_sgatpe000_sig") 
         ,checker_sgatpe001_sig("checker_sgatpe001_sig")
         ,checker_sgatpe002_sig("checker_sgatpe002_sig")
         ,checker_sgatpe003_sig("checker_sgatpe003_sig")
         ,checker_sgatpe004_sig("checker_sgatpe004_sig")
         ,checker_sgatpe005_sig("checker_sgatpe005_sig")
         ,checker_sgatpe006_sig("checker_sgatpe006_sig")
         ,checker_sgatpe007_sig("checker_sgatpe007_sig")
         ,checker_sgatpe008_sig("checker_sgatpe008_sig")
         ,checker_sgatpe009_sig("checker_sgatpe009_sig")
         ,checker_sgatpe010_sig("checker_sgatpe010_sig")
         ,checker_sgatpe011_sig("checker_sgatpe011_sig")
         ,checker_sgatpe012_sig("checker_sgatpe012_sig")
         ,checker_sgatpe013_sig("checker_sgatpe013_sig")
         ,checker_sgatpe014_sig("checker_sgatpe014_sig")
         ,checker_sgatpe015_sig("checker_sgatpe015_sig")
         ,checker_sgatpe016_sig("checker_sgatpe016_sig")
         ,checker_sgatpe017_sig("checker_sgatpe017_sig")
         ,checker_sgatpe018_sig("checker_sgatpe018_sig")
         ,checker_sgatpe019_sig("checker_sgatpe019_sig")
         ,checker_sgatpe020_sig("checker_sgatpe020_sig")
         ,checker_sgatpe021_sig("checker_sgatpe021_sig")
         ,checker_sgatpe022_sig("checker_sgatpe022_sig")
         ,checker_sgatpe023_sig("checker_sgatpe023_sig")
         ,checker_sgatpe024_sig("checker_sgatpe024_sig")
         ,checker_sgatpe025_sig("checker_sgatpe025_sig")
         ,checker_sgatpe026_sig("checker_sgatpe026_sig")
         ,checker_sgatpe027_sig("checker_sgatpe027_sig")
         ,checker_sgatpe028_sig("checker_sgatpe028_sig")
         ,checker_sgatpe029_sig("checker_sgatpe029_sig")
         ,checker_sgatpe030_sig("checker_sgatpe030_sig")
         ,checker_sgatpe031_sig("checker_sgatpe031_sig")
         ,checker_sgatpe032_sig("checker_sgatpe032_sig")
         ,checker_sgatpe033_sig("checker_sgatpe033_sig")
         ,checker_sgatpe034_sig("checker_sgatpe034_sig")
         ,checker_sgatpe035_sig("checker_sgatpe035_sig")
         ,checker_sgatpe036_sig("checker_sgatpe036_sig")
         ,checker_sgatpe037_sig("checker_sgatpe037_sig")
         ,checker_sgatpe038_sig("checker_sgatpe038_sig")
         ,checker_sgatpe039_sig("checker_sgatpe039_sig")
         ,checker_sgatpe040_sig("checker_sgatpe040_sig")
         ,checker_sgatpe041_sig("checker_sgatpe041_sig")
         ,checker_sgatpe042_sig("checker_sgatpe042_sig")
         ,checker_sgatpe043_sig("checker_sgatpe043_sig")
         ,checker_sgatpe044_sig("checker_sgatpe044_sig")
         ,checker_sgatpe045_sig("checker_sgatpe045_sig")
         ,checker_sgatpe046_sig("checker_sgatpe046_sig")
         ,checker_sgatpe047_sig("checker_sgatpe047_sig")
         ,checker_sgatpe048_sig("checker_sgatpe048_sig")
         ,checker_sgatpe049_sig("checker_sgatpe049_sig")
         ,checker_sgatpe050_sig("checker_sgatpe050_sig")
         ,checker_sgatpe051_sig("checker_sgatpe051_sig")
         ,checker_sgatpe052_sig("checker_sgatpe052_sig")
         ,checker_sgatpe053_sig("checker_sgatpe053_sig")
         ,checker_sgatpe054_sig("checker_sgatpe054_sig")
         ,checker_sgatpe055_sig("checker_sgatpe055_sig")
         ,checker_sgatpe056_sig("checker_sgatpe056_sig")
         ,checker_sgatpe057_sig("checker_sgatpe057_sig")
         ,checker_sgatpe058_sig("checker_sgatpe058_sig")
         ,checker_sgatpe059_sig("checker_sgatpe059_sig")
         ,checker_sgatpe060_sig("checker_sgatpe060_sig")
         ,checker_sgatpe061_sig("checker_sgatpe061_sig")
         ,checker_sgatpe062_sig("checker_sgatpe062_sig")
         ,checker_sgatpe063_sig("checker_sgatpe063_sig")
         ,checker_sgatpe064_sig("checker_sgatpe064_sig")
         ,checker_sgatpe065_sig("checker_sgatpe065_sig")
         ,checker_sgatpe066_sig("checker_sgatpe066_sig")
         ,checker_sgatpe067_sig("checker_sgatpe067_sig")
         ,checker_sgatpe068_sig("checker_sgatpe068_sig")
         ,checker_sgatpe069_sig("checker_sgatpe069_sig")
         ,checker_sgatpe070_sig("checker_sgatpe070_sig")
         ,checker_sgatpe071_sig("checker_sgatpe071_sig")
         ,checker_sgatpe072_sig("checker_sgatpe072_sig")
         ,checker_sgatpe073_sig("checker_sgatpe073_sig")
         ,checker_sgatpe074_sig("checker_sgatpe074_sig")
         ,checker_sgatpe075_sig("checker_sgatpe075_sig")
         ,checker_sgatpe076_sig("checker_sgatpe076_sig")
         ,checker_sgatpe077_sig("checker_sgatpe077_sig")
         ,checker_sgatpe078_sig("checker_sgatpe078_sig")
         ,checker_sgatpe079_sig("checker_sgatpe079_sig")
         ,checker_sgatpe080_sig("checker_sgatpe080_sig")
         ,checker_sgatpe081_sig("checker_sgatpe081_sig")
         ,checker_sgatpe082_sig("checker_sgatpe082_sig")
         ,checker_sgatpe083_sig("checker_sgatpe083_sig")
         ,checker_sgatpe084_sig("checker_sgatpe084_sig")
         ,checker_sgatpe085_sig("checker_sgatpe085_sig")
         ,checker_sgatpe086_sig("checker_sgatpe086_sig")
         ,checker_sgatpe087_sig("checker_sgatpe087_sig")
         ,checker_sgatpe088_sig("checker_sgatpe088_sig")
         ,checker_sgatpe089_sig("checker_sgatpe089_sig")
         ,checker_sgatpe090_sig("checker_sgatpe090_sig")
         ,checker_sgatpe091_sig("checker_sgatpe091_sig")
         ,checker_sgatpe092_sig("checker_sgatpe092_sig")
         //}}}
         ,checker_psout_sig("checker_psout_sig")
         ,master_psout_sig("master_psout_sig")
         ,checker_trgout_sig("checker_trgout_sig")
         ,master_trgout_sig("master_trgout_sig")
         ,dtmtrgout_sig("dtmtrgout_sig")
         ,preset_n("preset_n")
         ,cntclk_preset_n("cntclk_preset_n")
         ,pclk("pclk")
         ,erroutresz("erroutresz")
         ,cntclk_erroutresz("cntclk_erroutresz")
         ,cntclk("cntclk")
         //ecmterrinN (N = 00 .. 92)
         //{{{ 
         ,ecmterrin0("ecmterrin0")
         ,ecmterrin1("ecmterrin1")
         ,ecmterrin2("ecmterrin2")
         ,ecmterrin3("ecmterrin3")
         ,ecmterrin4("ecmterrin4")
         ,ecmterrin5("ecmterrin5")
         ,ecmterrin6("ecmterrin6")
         ,ecmterrin7("ecmterrin7")
         ,ecmterrin8("ecmterrin8")
         ,ecmterrin9("ecmterrin9")
         ,ecmterrin10("ecmterrin10")
         ,ecmterrin11("ecmterrin11")
         ,ecmterrin12("ecmterrin12")
         ,ecmterrin13("ecmterrin13")
         ,ecmterrin14("ecmterrin14")
         ,ecmterrin15("ecmterrin15")
         ,ecmterrin16("ecmterrin16")
         ,ecmterrin17("ecmterrin17")
         ,ecmterrin18("ecmterrin18")
         ,ecmterrin19("ecmterrin19")
         ,ecmterrin20("ecmterrin20")
         ,ecmterrin21("ecmterrin21")
         ,ecmterrin22("ecmterrin22")
         ,ecmterrin23("ecmterrin23")
         ,ecmterrin24("ecmterrin24")
         ,ecmterrin25("ecmterrin25")
         ,ecmterrin26("ecmterrin26")
         ,ecmterrin27("ecmterrin27")
         ,ecmterrin28("ecmterrin28")
         ,ecmterrin29("ecmterrin29")
         ,ecmterrin30("ecmterrin30")
         ,ecmterrin31("ecmterrin31")
         ,ecmterrin32("ecmterrin32")
         ,ecmterrin33("ecmterrin33")
         ,ecmterrin34("ecmterrin34")
         ,ecmterrin35("ecmterrin35")
         ,ecmterrin36("ecmterrin36")
         ,ecmterrin37("ecmterrin37")
         ,ecmterrin38("ecmterrin38")
         ,ecmterrin39("ecmterrin39")
         ,ecmterrin40("ecmterrin40")
         ,ecmterrin41("ecmterrin41")
         ,ecmterrin42("ecmterrin42")
         ,ecmterrin43("ecmterrin43")
         ,ecmterrin44("ecmterrin44")
         ,ecmterrin45("ecmterrin45")
         ,ecmterrin46("ecmterrin46")
         ,ecmterrin47("ecmterrin47")
         ,ecmterrin48("ecmterrin48")
         ,ecmterrin49("ecmterrin49")
         ,ecmterrin50("ecmterrin50")
         ,ecmterrin51("ecmterrin51")
         ,ecmterrin52("ecmterrin52")
         ,ecmterrin53("ecmterrin53")
         ,ecmterrin54("ecmterrin54")
         ,ecmterrin55("ecmterrin55")
         ,ecmterrin56("ecmterrin56")
         ,ecmterrin57("ecmterrin57")
         ,ecmterrin58("ecmterrin58")
         ,ecmterrin59("ecmterrin59")
         ,ecmterrin60("ecmterrin60")
         ,ecmterrin61("ecmterrin61")
         ,ecmterrin62("ecmterrin62")
         ,ecmterrin63("ecmterrin63")
         ,ecmterrin64("ecmterrin64")
         ,ecmterrin65("ecmterrin65")
         ,ecmterrin66("ecmterrin66")
         ,ecmterrin67("ecmterrin67")
         ,ecmterrin68("ecmterrin68")
         ,ecmterrin69("ecmterrin69")
         ,ecmterrin70("ecmterrin70")
         ,ecmterrin71("ecmterrin71")
         ,ecmterrin72("ecmterrin72")
         ,ecmterrin73("ecmterrin73")
         ,ecmterrin74("ecmterrin74")
         ,ecmterrin75("ecmterrin75")
         ,ecmterrin76("ecmterrin76")
         ,ecmterrin77("ecmterrin77")
         ,ecmterrin78("ecmterrin78")
         ,ecmterrin79("ecmterrin79")
         ,ecmterrin80("ecmterrin80")
         ,ecmterrin81("ecmterrin81")
         ,ecmterrin82("ecmterrin82")
         ,ecmterrin83("ecmterrin83")
         ,ecmterrin84("ecmterrin84")
         ,ecmterrin85("ecmterrin85")
         ,ecmterrin86("ecmterrin86")
         ,ecmterrin87("ecmterrin87")
         ,ecmterrin88("ecmterrin88")
         ,ecmterrin89("ecmterrin89")
         ,ecmterrin90("ecmterrin90")
         ,ecmterrin91("ecmterrin91")
         //}}}
         ,ecmterrlbz("ecmterrlbz")
         ,ecmttin("ecmttin")
         ,resstg1z("resstg1z")
         ,pclkin_resstg1z("pclkin_resstg1z")
         ,svaccess("svaccess")
         ,ecmterrin92msk_m("ecmterrin92msk_m")
         ,ecmterrin92msk_c("ecmterrin92msk_c")
         ,ecmterroz("ecmterroz")
         ,ecmterroutz("ecmterroutz")
         ,ecmti("ecmti")
         ,ecmtnmi("ecmtnmi")
         ,ecmtresz("ecmtresz")
{//{{{
    CommandInit(this->name());

    mMaster = new Cecm ("mMaster");
    sc_assert(mMaster != NULL);
    mChecker = new Cecm ("mChecker");
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
    mECMTI                  = false;
    mECMTNMI                = false;
    mECMTRESZ               = true;
    mECMTERROZ              = false;
    mECMTERROUTZ_M          = false;
    mECMTERROUTZ_C          = false;
    mFreqPCLK               = 0;
    mFreqCNTCLK             = 0;

    InitializeInternalSignal();
    //Initialize output port
    ecmterroz.initialize(false);
    ecmterroutz.initialize(false);
    ecmti.initialize(false);
    ecmtnmi.initialize(false);
    ecmtresz.initialize(true);

    //Initialize mExternalErrorInputTable
    ExternalErrorInputTableConstruction();

    SC_METHOD(ReceiveExternalErrorMethod)
    dont_initialize();
    //sensitive <<  ecmterrinN (N = 000 .. 092)
    //{{{
    sensitive <<  ecmterrin0;
    sensitive <<  ecmterrin1;
    sensitive <<  ecmterrin2;
    sensitive <<  ecmterrin3;
    sensitive <<  ecmterrin4;
    sensitive <<  ecmterrin5;
    sensitive <<  ecmterrin6;
    sensitive <<  ecmterrin7;
    sensitive <<  ecmterrin8;
    sensitive <<  ecmterrin9;
    sensitive <<  ecmterrin10;
    sensitive <<  ecmterrin11;
    sensitive <<  ecmterrin12;
    sensitive <<  ecmterrin13;
    sensitive <<  ecmterrin14;
    sensitive <<  ecmterrin15;
    sensitive <<  ecmterrin16;
    sensitive <<  ecmterrin17;
    sensitive <<  ecmterrin18;
    sensitive <<  ecmterrin19;
    sensitive <<  ecmterrin20;
    sensitive <<  ecmterrin21;
    sensitive <<  ecmterrin22;
    sensitive <<  ecmterrin23;
    sensitive <<  ecmterrin24;
    sensitive <<  ecmterrin25;
    sensitive <<  ecmterrin26;
    sensitive <<  ecmterrin27;
    sensitive <<  ecmterrin28;
    sensitive <<  ecmterrin29;
    sensitive <<  ecmterrin30;
    sensitive <<  ecmterrin31;
    sensitive <<  ecmterrin32;
    sensitive <<  ecmterrin33;
    sensitive <<  ecmterrin34;
    sensitive <<  ecmterrin35;
    sensitive <<  ecmterrin36;
    sensitive <<  ecmterrin37;
    sensitive <<  ecmterrin38;
    sensitive <<  ecmterrin39;
    sensitive <<  ecmterrin40;
    sensitive <<  ecmterrin41;
    sensitive <<  ecmterrin42;
    sensitive <<  ecmterrin43;
    sensitive <<  ecmterrin44;
    sensitive <<  ecmterrin45;
    sensitive <<  ecmterrin46;
    sensitive <<  ecmterrin47;
    sensitive <<  ecmterrin48;
    sensitive <<  ecmterrin49;
    sensitive <<  ecmterrin50;
    sensitive <<  ecmterrin51;
    sensitive <<  ecmterrin52;
    sensitive <<  ecmterrin53;
    sensitive <<  ecmterrin54;
    sensitive <<  ecmterrin55;
    sensitive <<  ecmterrin56;
    sensitive <<  ecmterrin57;
    sensitive <<  ecmterrin58;
    sensitive <<  ecmterrin59;
    sensitive <<  ecmterrin60;
    sensitive <<  ecmterrin61;
    sensitive <<  ecmterrin62;
    sensitive <<  ecmterrin63;
    sensitive <<  ecmterrin64;
    sensitive <<  ecmterrin65;
    sensitive <<  ecmterrin66;
    sensitive <<  ecmterrin67;
    sensitive <<  ecmterrin68;
    sensitive <<  ecmterrin69;
    sensitive <<  ecmterrin70;
    sensitive <<  ecmterrin71;
    sensitive <<  ecmterrin72;
    sensitive <<  ecmterrin73;
    sensitive <<  ecmterrin74;
    sensitive <<  ecmterrin75;
    sensitive <<  ecmterrin76;
    sensitive <<  ecmterrin77;
    sensitive <<  ecmterrin78;
    sensitive <<  ecmterrin79;
    sensitive <<  ecmterrin80;
    sensitive <<  ecmterrin81;
    sensitive <<  ecmterrin82;
    sensitive <<  ecmterrin83;
    sensitive <<  ecmterrin84;
    sensitive <<  ecmterrin85;
    sensitive <<  ecmterrin86;
    sensitive <<  ecmterrin87;
    sensitive <<  ecmterrin88;
    sensitive <<  ecmterrin89;
    sensitive <<  ecmterrin90;
    sensitive <<  ecmterrin91;
    //}}}
    
    //sensitive <<  sgatpeN (N = 000 .. 092)
    //{{{
    sensitive <<  master_sgatpe000_sig;
    sensitive <<  master_sgatpe001_sig;
    sensitive <<  master_sgatpe002_sig;
    sensitive <<  master_sgatpe003_sig;
    sensitive <<  master_sgatpe004_sig;
    sensitive <<  master_sgatpe005_sig;
    sensitive <<  master_sgatpe006_sig;
    sensitive <<  master_sgatpe007_sig;
    sensitive <<  master_sgatpe008_sig;
    sensitive <<  master_sgatpe009_sig;
    sensitive <<  master_sgatpe010_sig;
    sensitive <<  master_sgatpe011_sig;
    sensitive <<  master_sgatpe012_sig;
    sensitive <<  master_sgatpe013_sig;
    sensitive <<  master_sgatpe014_sig;
    sensitive <<  master_sgatpe015_sig;
    sensitive <<  master_sgatpe016_sig;
    sensitive <<  master_sgatpe017_sig;
    sensitive <<  master_sgatpe018_sig;
    sensitive <<  master_sgatpe019_sig;
    sensitive <<  master_sgatpe020_sig;
    sensitive <<  master_sgatpe021_sig;
    sensitive <<  master_sgatpe022_sig;
    sensitive <<  master_sgatpe023_sig;
    sensitive <<  master_sgatpe024_sig;
    sensitive <<  master_sgatpe025_sig;
    sensitive <<  master_sgatpe026_sig;
    sensitive <<  master_sgatpe027_sig;
    sensitive <<  master_sgatpe028_sig;
    sensitive <<  master_sgatpe029_sig;
    sensitive <<  master_sgatpe030_sig;
    sensitive <<  master_sgatpe031_sig;
    sensitive <<  master_sgatpe032_sig;
    sensitive <<  master_sgatpe033_sig;
    sensitive <<  master_sgatpe034_sig;
    sensitive <<  master_sgatpe035_sig;
    sensitive <<  master_sgatpe036_sig;
    sensitive <<  master_sgatpe037_sig;
    sensitive <<  master_sgatpe038_sig;
    sensitive <<  master_sgatpe039_sig;
    sensitive <<  master_sgatpe040_sig;
    sensitive <<  master_sgatpe041_sig;
    sensitive <<  master_sgatpe042_sig;
    sensitive <<  master_sgatpe043_sig;
    sensitive <<  master_sgatpe044_sig;
    sensitive <<  master_sgatpe045_sig;
    sensitive <<  master_sgatpe046_sig;
    sensitive <<  master_sgatpe047_sig;
    sensitive <<  master_sgatpe048_sig;
    sensitive <<  master_sgatpe049_sig;
    sensitive <<  master_sgatpe050_sig;
    sensitive <<  master_sgatpe051_sig;
    sensitive <<  master_sgatpe052_sig;
    sensitive <<  master_sgatpe053_sig;
    sensitive <<  master_sgatpe054_sig;
    sensitive <<  master_sgatpe055_sig;
    sensitive <<  master_sgatpe056_sig;
    sensitive <<  master_sgatpe057_sig;
    sensitive <<  master_sgatpe058_sig;
    sensitive <<  master_sgatpe059_sig;
    sensitive <<  master_sgatpe060_sig;
    sensitive <<  master_sgatpe061_sig;
    sensitive <<  master_sgatpe062_sig;
    sensitive <<  master_sgatpe063_sig;
    sensitive <<  master_sgatpe064_sig;
    sensitive <<  master_sgatpe065_sig;
    sensitive <<  master_sgatpe066_sig;
    sensitive <<  master_sgatpe067_sig;
    sensitive <<  master_sgatpe068_sig;
    sensitive <<  master_sgatpe069_sig;
    sensitive <<  master_sgatpe070_sig;
    sensitive <<  master_sgatpe071_sig;
    sensitive <<  master_sgatpe072_sig;
    sensitive <<  master_sgatpe073_sig;
    sensitive <<  master_sgatpe074_sig;
    sensitive <<  master_sgatpe075_sig;
    sensitive <<  master_sgatpe076_sig;
    sensitive <<  master_sgatpe077_sig;
    sensitive <<  master_sgatpe078_sig;
    sensitive <<  master_sgatpe079_sig;
    sensitive <<  master_sgatpe080_sig;
    sensitive <<  master_sgatpe081_sig;
    sensitive <<  master_sgatpe082_sig;
    sensitive <<  master_sgatpe083_sig;
    sensitive <<  master_sgatpe084_sig;
    sensitive <<  master_sgatpe085_sig;
    sensitive <<  master_sgatpe086_sig;
    sensitive <<  master_sgatpe087_sig;
    sensitive <<  master_sgatpe088_sig;
    sensitive <<  master_sgatpe089_sig;
    sensitive <<  master_sgatpe090_sig;
    //}}}

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
    sensitive << mReceivePCLKEvent;

    SC_METHOD(svaccessMethod)
    dont_initialize();
    sensitive << svaccess;
    sensitive << mReceiveSVACCESSEvent;

    SC_METHOD(InitializeMethod)
    dont_initialize();
    sensitive << mInitializeEvent;

    SC_METHOD(CombineSignalMethod)
    dont_initialize();
    sensitive << master_sgaterroz_sig;
    sensitive << master_sgaterroutz_sig;
    sensitive << master_sgati_sig;
    sensitive << master_sgatnmi_sig;
    sensitive << master_sgatresz_sig;
    sensitive << checker_sgaterroz_sig;
    sensitive << checker_sgaterroutz_sig;
    sensitive << checker_sgati_sig;
    sensitive << checker_sgatnmi_sig;
    sensitive << checker_sgatresz_sig;
    sensitive << checker_trgout_sig;
    sensitive << master_trgout_sig;
    sensitive << mMaster->mCombineSignalEvent;
    sensitive << mChecker->mCombineSignalEvent;

    SC_METHOD(WriteOutputPortMethod)
    dont_initialize();
    sensitive << mWriteOutputPortEvent;

    SC_METHOD(ComparatorTestMethod)
    dont_initialize();
    sensitive << master_sgatpe092_sig;
    sensitive << mComparatorTestEvent;

    SC_METHOD(ECMterrin92MaskMethod)
    dont_initialize();
    sensitive << ecmterrin92msk_m;
    sensitive << ecmterrin92msk_c;


    #ifdef CWR_SYSTEMC
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

Cecm_wp::~Cecm_wp()
{//{{{
    delete mMaster;
    delete mChecker;
}//}}}

std::string Cecm_wp::GetMasterCheckerName(const bool is_master) const
{//{{{
    if (is_master){
        return (std::string)mMaster->name();
    }else{
        return (std::string)mChecker->name();
    }
}//}}}

void Cecm_wp::ReceiveExternalErrorMethod(void)
{//{{{
    if (mFreqPCLK == 0) {
        re_printf("warning","The pclk period is equal 0.\n");
    }

    if (mFreqCNTCLK == 0) {
        re_printf("warning","The cntclk period is equal 0.\n");
    }

    if ((mFreqPCLK != 0) && (mFreqCNTCLK != 0) && (!mPortReset) && (!mCmdReset)){
        UpdateState(mState, emErrorInputProcessingState);
    }
    //sgatpeN_sig.write(ecmterrinN.read()) (N = 000 .. 090)
    //{{{
    sgaterrin00_sig.write(ecmterrin0.read() || master_sgatpe000_sig.read());
    sgaterrin01_sig.write(ecmterrin1.read() || master_sgatpe001_sig.read());
    sgaterrin02_sig.write(ecmterrin2.read() || master_sgatpe002_sig.read());
    sgaterrin03_sig.write(ecmterrin3.read() || master_sgatpe003_sig.read());
    sgaterrin04_sig.write(ecmterrin4.read() || master_sgatpe004_sig.read());
    sgaterrin05_sig.write(ecmterrin5.read() || master_sgatpe005_sig.read());
    sgaterrin06_sig.write(ecmterrin6.read() || master_sgatpe006_sig.read());
    sgaterrin07_sig.write(ecmterrin7.read() || master_sgatpe007_sig.read());
    sgaterrin08_sig.write(ecmterrin8.read() || master_sgatpe008_sig.read());
    sgaterrin09_sig.write(ecmterrin9.read() || master_sgatpe009_sig.read());
    sgaterrin10_sig.write(ecmterrin10.read() || master_sgatpe010_sig.read());
    sgaterrin11_sig.write(ecmterrin11.read() || master_sgatpe011_sig.read());
    sgaterrin12_sig.write(ecmterrin12.read() || master_sgatpe012_sig.read());
    sgaterrin13_sig.write(ecmterrin13.read() || master_sgatpe013_sig.read());
    sgaterrin14_sig.write(ecmterrin14.read() || master_sgatpe014_sig.read());
    sgaterrin15_sig.write(ecmterrin15.read() || master_sgatpe015_sig.read());
    sgaterrin16_sig.write(ecmterrin16.read() || master_sgatpe016_sig.read());
    sgaterrin17_sig.write(ecmterrin17.read() || master_sgatpe017_sig.read());
    sgaterrin18_sig.write(ecmterrin18.read() || master_sgatpe018_sig.read());
    sgaterrin19_sig.write(ecmterrin19.read() || master_sgatpe019_sig.read());
    sgaterrin20_sig.write(ecmterrin20.read() || master_sgatpe020_sig.read());
    sgaterrin21_sig.write(ecmterrin21.read() || master_sgatpe021_sig.read());
    sgaterrin22_sig.write(ecmterrin22.read() || master_sgatpe022_sig.read());
    sgaterrin23_sig.write(ecmterrin23.read() || master_sgatpe023_sig.read());
    sgaterrin24_sig.write(ecmterrin24.read() || master_sgatpe024_sig.read());
    sgaterrin25_sig.write(ecmterrin25.read() || master_sgatpe025_sig.read());
    sgaterrin26_sig.write(ecmterrin26.read() || master_sgatpe026_sig.read());
    sgaterrin27_sig.write(ecmterrin27.read() || master_sgatpe027_sig.read());
    sgaterrin28_sig.write(ecmterrin28.read() || master_sgatpe028_sig.read());
    sgaterrin29_sig.write(ecmterrin29.read() || master_sgatpe029_sig.read());
    sgaterrin30_sig.write(ecmterrin30.read() || master_sgatpe030_sig.read());
    sgaterrin31_sig.write(ecmterrin31.read() || master_sgatpe031_sig.read());
    sgaterrin32_sig.write(ecmterrin32.read() || master_sgatpe032_sig.read());
    sgaterrin33_sig.write(ecmterrin33.read() || master_sgatpe033_sig.read());
    sgaterrin34_sig.write(ecmterrin34.read() || master_sgatpe034_sig.read());
    sgaterrin35_sig.write(ecmterrin35.read() || master_sgatpe035_sig.read());
    sgaterrin36_sig.write(ecmterrin36.read() || master_sgatpe036_sig.read());
    sgaterrin37_sig.write(ecmterrin37.read() || master_sgatpe037_sig.read());
    sgaterrin38_sig.write(ecmterrin38.read() || master_sgatpe038_sig.read());
    sgaterrin39_sig.write(ecmterrin39.read() || master_sgatpe039_sig.read());
    sgaterrin40_sig.write(ecmterrin40.read() || master_sgatpe040_sig.read());
    sgaterrin41_sig.write(ecmterrin41.read() || master_sgatpe041_sig.read());
    sgaterrin42_sig.write(ecmterrin42.read() || master_sgatpe042_sig.read());
    sgaterrin43_sig.write(ecmterrin43.read() || master_sgatpe043_sig.read());
    sgaterrin44_sig.write(ecmterrin44.read() || master_sgatpe044_sig.read());
    sgaterrin45_sig.write(ecmterrin45.read() || master_sgatpe045_sig.read());
    sgaterrin46_sig.write(ecmterrin46.read() || master_sgatpe046_sig.read());
    sgaterrin47_sig.write(ecmterrin47.read() || master_sgatpe047_sig.read());
    sgaterrin48_sig.write(ecmterrin48.read() || master_sgatpe048_sig.read());
    sgaterrin49_sig.write(ecmterrin49.read() || master_sgatpe049_sig.read());
    sgaterrin50_sig.write(ecmterrin50.read() || master_sgatpe050_sig.read());
    sgaterrin51_sig.write(ecmterrin51.read() || master_sgatpe051_sig.read());
    sgaterrin52_sig.write(ecmterrin52.read() || master_sgatpe052_sig.read());
    sgaterrin53_sig.write(ecmterrin53.read() || master_sgatpe053_sig.read());
    sgaterrin54_sig.write(ecmterrin54.read() || master_sgatpe054_sig.read());
    sgaterrin55_sig.write(ecmterrin55.read() || master_sgatpe055_sig.read());
    sgaterrin56_sig.write(ecmterrin56.read() || master_sgatpe056_sig.read());
    sgaterrin57_sig.write(ecmterrin57.read() || master_sgatpe057_sig.read());
    sgaterrin58_sig.write(ecmterrin58.read() || master_sgatpe058_sig.read());
    sgaterrin59_sig.write(ecmterrin59.read() || master_sgatpe059_sig.read());
    sgaterrin60_sig.write(ecmterrin60.read() || master_sgatpe060_sig.read());
    sgaterrin61_sig.write(ecmterrin61.read() || master_sgatpe061_sig.read());
    sgaterrin62_sig.write(ecmterrin62.read() || master_sgatpe062_sig.read());
    sgaterrin63_sig.write(ecmterrin63.read() || master_sgatpe063_sig.read());
    sgaterrin64_sig.write(ecmterrin64.read() || master_sgatpe064_sig.read());
    sgaterrin65_sig.write(ecmterrin65.read() || master_sgatpe065_sig.read());
    sgaterrin66_sig.write(ecmterrin66.read() || master_sgatpe066_sig.read());
    sgaterrin67_sig.write(ecmterrin67.read() || master_sgatpe067_sig.read());
    sgaterrin68_sig.write(ecmterrin68.read() || master_sgatpe068_sig.read());
    sgaterrin69_sig.write(ecmterrin69.read() || master_sgatpe069_sig.read());
    sgaterrin70_sig.write(ecmterrin70.read() || master_sgatpe070_sig.read());
    sgaterrin71_sig.write(ecmterrin71.read() || master_sgatpe071_sig.read());
    sgaterrin72_sig.write(ecmterrin72.read() || master_sgatpe072_sig.read());
    sgaterrin73_sig.write(ecmterrin73.read() || master_sgatpe073_sig.read());
    sgaterrin74_sig.write(ecmterrin74.read() || master_sgatpe074_sig.read());
    sgaterrin75_sig.write(ecmterrin75.read() || master_sgatpe075_sig.read());
    sgaterrin76_sig.write(ecmterrin76.read() || master_sgatpe076_sig.read());
    sgaterrin77_sig.write(ecmterrin77.read() || master_sgatpe077_sig.read());
    sgaterrin78_sig.write(ecmterrin78.read() || master_sgatpe078_sig.read());
    sgaterrin79_sig.write(ecmterrin79.read() || master_sgatpe079_sig.read());
    sgaterrin80_sig.write(ecmterrin80.read() || master_sgatpe080_sig.read());
    sgaterrin81_sig.write(ecmterrin81.read() || master_sgatpe081_sig.read());
    sgaterrin82_sig.write(ecmterrin82.read() || master_sgatpe082_sig.read());
    sgaterrin83_sig.write(ecmterrin83.read() || master_sgatpe083_sig.read());
    sgaterrin84_sig.write(ecmterrin84.read() || master_sgatpe084_sig.read());
    sgaterrin85_sig.write(ecmterrin85.read() || master_sgatpe085_sig.read());
    sgaterrin86_sig.write(ecmterrin86.read() || master_sgatpe086_sig.read());
    sgaterrin87_sig.write(ecmterrin87.read() || master_sgatpe087_sig.read());
    sgaterrin88_sig.write(ecmterrin88.read() || master_sgatpe088_sig.read());
    sgaterrin89_sig.write(ecmterrin89.read() || master_sgatpe089_sig.read());
    sgaterrin90_sig.write(ecmterrin90.read() || master_sgatpe090_sig.read());
    sgaterrin91_sig.write(ecmterrin91.read());
    //}}}
    DumpEnableTransInfo();
    
    if ((mFreqPCLK != 0) && (mFreqCNTCLK != 0) && (!mPortReset) && (!mCmdReset)){
        UpdateState(mState, emECMProcessingState);
    }
}//}}} 

void Cecm_wp::ResetMethod(void)
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

void Cecm_wp::AssertReset( double delay, double period)
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

void Cecm_wp::CmdResetMethod(void)
{//{{{
    mCmdReset = true;
    this->EnableReset(mCmdReset);
    mCancelCmdResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Cecm_wp::CancelCmdResetMethod(void)
{//{{{
    mCmdReset = false;
    mIsCmdResetStatus = false;
    this->EnableReset(mCmdReset);
}//}}}

void Cecm_wp::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        //Change status of ECM_WP to resetting
        UpdateState(mState, emResettingState);

        //cancel events
        mWriteOutputPortEvent.cancel();
        mReceiveExternalErrorEvent.cancel();
        mReceivePCLKEvent.cancel();
        mReceiveSVACCESSEvent.cancel();

        //Initialize output ports
        mInitializeEvent.notify();
    }else{
        //Change status of ECM_WP from resetting to idle
        UpdateState(mState, emIdleState);
        //Receive the input signal after reset
        mReceiveExternalErrorEvent.notify();
        mReceivePCLKEvent.notify();
        mReceiveSVACCESSEvent.notify();
    }
    //Call the reset function of common/general class
    mMaster->ECMEnableReset(is_active);
    mChecker->ECMEnableReset(is_active);
    if(mCmdReset == false){  // if reset by AssertReset commad, tgt if isn't initialized.
        this->tgt_enable_reset(is_active);
    }
}//}}}

void Cecm_wp::PCLKPeriodMethod(void)
{//{{{
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Cecm_wp::CounterPeriodMethod(void)
{//{{{
    SetCLKfreq("cntclk", sc_dt::uint64_to_double(cntclk.read()));
}//}}}

void Cecm_wp::svaccessMethod(void)
{//{{{
    if (mPortReset || mCmdReset){
        return;
    }
    if (mFreqPCLK == 0) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }

    if (mFreqCNTCLK == 0) {
        re_printf("warning","The cntclk period is equal 0.\n");
        return;
    }

    if (svaccess.read()){
        re_printf("info","Break signal is asserted.\n");
    }else{
        re_printf("info","Break signal is de-asserted.\n");
    }
}//}}}

void Cecm_wp::CombineSignalMethod(void)
{//{{{
    if (mPortReset || mCmdReset){
        return;
    }
    UpdateState(mState, emOutputSignalCombinationState);
    //interrupt (maskable and non-maskable)
    bool ecmti_value = (master_sgati_sig.read()) | (checker_sgati_sig.read());
    bool ecmtnmi_value = (master_sgatnmi_sig.read()) | (checker_sgatnmi_sig.read());

    if (DumpInterrupt){
        if (mECMTI != ecmti_value){
            if (ecmti_value){
                re_printf("info","INT [%s: ECMTI] Assert.\n", this->basename());
            }else{
                re_printf("info","INT [%s: ECMTI] De-assert.\n", this->basename());
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
    mECMTI = ecmti_value;
    mECMTNMI = ecmtnmi_value;
    //error output
    mECMTERROZ = (master_sgaterroz_sig.read()) & (checker_sgaterroz_sig.read());
    mECMTERROUTZ_M = master_sgaterroutz_sig.read();
    mECMTERROUTZ_C = checker_sgaterroutz_sig.read();
    //safety reset
    mECMTRESZ = (master_sgatresz_sig.read()) & (checker_sgatresz_sig.read());

    //assign value to dtmtrgin of Master and Checker
    dtmtrgout_sig.write((master_trgout_sig.read()) | (checker_trgout_sig.read()));

    //update sgoterrin
    if ((master_sgaterroz_sig.read()) == (checker_sgaterroz_sig.read())){
        mSgoterrin = false;
    }else{
        re_printf("warning","The error output signals from Master and Checker are different.\n");
        mSgoterrin = true;
    }
    mWriteOutputPortEvent.notify();
    mComparatorTestEvent.notify();
}//}}}

void Cecm_wp::ComparatorTestMethod(void)
{//{{{
    master_sgaterrin92_sig.write((mSgoterrin || master_sgatpe092_sig.read()) && !ecmterrin92msk_m.read());
    checker_sgaterrin92_sig.write((mSgoterrin || master_sgatpe092_sig.read()) && !ecmterrin92msk_c.read()); 
}//}}}

void Cecm_wp::ECMterrin92MaskMethod(void)
{//{{{
    if (mPortReset || mCmdReset){
        return;
    }
    if (mFreqPCLK == 0) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }

    if (mFreqCNTCLK == 0) {
        re_printf("warning","The cntclk period is equal 0.\n");
        return;
    }

    mComparatorTestEvent.notify();
}//}}}

void Cecm_wp::WriteOutputPortMethod(void)
{//{{{ 
    ecmti.write(mECMTI);
    ecmtnmi.write(mECMTNMI);
    ecmterroz.write(mECMTERROZ);
    ecmterroutz.write(mECMTERROUTZ_M&mECMTERROUTZ_C);
    ecmtresz.write(mECMTRESZ);
}//}}}

void Cecm_wp::ExternalErrorInputTableConstruction(void)
{//{{{
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin0,"ecmterrin0",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin1,"ecmterrin1",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin2,"ecmterrin2",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin3,"ecmterrin3",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin4,"ecmterrin4",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin5,"ecmterrin5",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin6,"ecmterrin6",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin7,"ecmterrin7",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin8,"ecmterrin8",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin9,"ecmterrin9",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin10,"ecmterrin10",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin11,"ecmterrin11",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin12,"ecmterrin12",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin13,"ecmterrin13",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin14,"ecmterrin14",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin15,"ecmterrin15",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin16,"ecmterrin16",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin17,"ecmterrin17",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin18,"ecmterrin18",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin19,"ecmterrin19",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin20,"ecmterrin20",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin21,"ecmterrin21",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin22,"ecmterrin22",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin23,"ecmterrin23",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin24,"ecmterrin24",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin25,"ecmterrin25",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin26,"ecmterrin26",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin27,"ecmterrin27",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin28,"ecmterrin28",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin29,"ecmterrin29",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin30,"ecmterrin30",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin31,"ecmterrin31",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin32,"ecmterrin32",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin33,"ecmterrin33",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin34,"ecmterrin34",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin35,"ecmterrin35",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin36,"ecmterrin36",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin37,"ecmterrin37",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin38,"ecmterrin38",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin39,"ecmterrin39",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin40,"ecmterrin40",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin41,"ecmterrin41",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin42,"ecmterrin42",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin43,"ecmterrin43",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin44,"ecmterrin44",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin45,"ecmterrin45",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin46,"ecmterrin46",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin47,"ecmterrin47",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin48,"ecmterrin48",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin49,"ecmterrin49",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin50,"ecmterrin50",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin51,"ecmterrin51",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin52,"ecmterrin52",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin53,"ecmterrin53",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin54,"ecmterrin54",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin55,"ecmterrin55",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin56,"ecmterrin56",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin57,"ecmterrin57",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin58,"ecmterrin58",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin59,"ecmterrin59",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin60,"ecmterrin60",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin61,"ecmterrin61",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin62,"ecmterrin62",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin63,"ecmterrin63",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin64,"ecmterrin64",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin65,"ecmterrin65",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin66,"ecmterrin66",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin67,"ecmterrin67",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin68,"ecmterrin68",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin69,"ecmterrin69",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin70,"ecmterrin70",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin71,"ecmterrin71",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin72,"ecmterrin72",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin73,"ecmterrin73",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin74,"ecmterrin74",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin75,"ecmterrin75",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin76,"ecmterrin76",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin77,"ecmterrin77",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin78,"ecmterrin78",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin79,"ecmterrin79",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin80,"ecmterrin80",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin81,"ecmterrin81",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin82,"ecmterrin82",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin83,"ecmterrin83",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin84,"ecmterrin84",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin85,"ecmterrin85",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin86,"ecmterrin86",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin87,"ecmterrin87",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin88,"ecmterrin88",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin89,"ecmterrin89",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin90,"ecmterrin90",false));
    mExternalErrorInputTable.push_back(ExternalErrorInputElement(&ecmterrin91,"ecmterrin91",false));
}//}}}

void Cecm_wp::DumpStatInfo(void)
{//{{{
    const char *type = "StatInfo";
    DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
    DumpInfo(type, "  ECMTERROUTZ: %d\n", ecmterroutz.read());
    DumpInfo(type, "  ECMTERROZ: %d\n", ecmterroz.read());
    DumpInfo(type, "  ECMTI: %d\n", ecmti.read());
    DumpInfo(type, "  ECMTNMI: %d\n", ecmtnmi.read());
    DumpInfo(type, "  ECMTRESZ: %d\n", ecmtresz.read());
    DumpInfo(type, "EndInfo.\n");
}//}}}

void Cecm_wp::DumpEnableTransInfo()
{//{{{
    if (EnableTransInfo){
        const char *type = "TransInfo";
        DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
        bool error_value = false;
        //check whether external error input changes or not
        for (unsigned int error_index = 0; error_index < mExternalErrorInputTable.size(); error_index++){
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

void Cecm_wp::UpdateState(eState prev_state, eState next_state)
{//{{{
    if (prev_state != next_state){
        re_printf("info","ECM_WP state is changed from %s state to %s state.\n", mStateArray[prev_state].c_str(), mStateArray[next_state].c_str());
        mState = next_state;
    }
}//}}}

void Cecm_wp::InitializeMethod()
{//{{{
    //Initialize data members
    mECMTI              = false;
    mECMTNMI            = false;
    mECMTRESZ           = true;
    mWriteOutputPortEvent.notify();
}//}}}

void Cecm_wp::ConnectPort()
{//{{{
    mMaster->pclk(pclk);
    mChecker->pclk(pclk);
    mMaster->cntclk(cntclk);
    mChecker->cntclk(cntclk);
    mMaster->preset_n(preset_n);
    mChecker->preset_n(preset_n);
    mMaster->erroutresz(erroutresz);
    mChecker->erroutresz(erroutresz);
    mMaster->sgaterroz(master_sgaterroz_sig);
    mMaster->sgaterroutz(master_sgaterroutz_sig);
    mMaster->sgati(master_sgati_sig);
    mMaster->sgatnmi(master_sgatnmi_sig);
    mMaster->sgatresz(master_sgatresz_sig);
    mChecker->sgaterroz(checker_sgaterroz_sig);
    mChecker->sgaterroutz(checker_sgaterroutz_sig);
    mChecker->sgati(checker_sgati_sig);
    mChecker->sgatnmi(checker_sgatnmi_sig);
    mChecker->sgatresz(checker_sgatresz_sig);

    mMaster->sgatpsin(checker_psout_sig);
    mChecker->sgatpsout(checker_psout_sig);
    mChecker->sgatpsin(master_psout_sig);
    mMaster->sgatpsout(master_psout_sig);
    mChecker->dtmtrgout(checker_trgout_sig);
    mMaster->dtmtrgout(master_trgout_sig);
    mMaster->dtmtrgin(dtmtrgout_sig);
    mChecker->dtmtrgin(dtmtrgout_sig);
    mMaster->errout_clear_mask_out(errout_clear_mask_sig);
    mMaster->errout_clear_mask_in(errout_clear_mask_sig);
    mChecker->errout_clear_mask_in(errout_clear_mask_sig);
    mChecker->errout_clear_mask_out(errout_clear_mask_checker);

    mMaster->svaccess(svaccess);
    mChecker->svaccess(svaccess);
    mMaster->sgattin(ecmttin);
    mChecker->sgattin(ecmttin);
    mMaster->sgaterrlbz(ecmterrlbz);
    mChecker->sgaterrlbz(ecmterrlbz);
    mMaster->resstg1z(resstg1z);
    mChecker->resstg1z(resstg1z);
    mMaster->resstg1z_pclkin(pclkin_resstg1z);
    mChecker->resstg1z_pclkin(pclkin_resstg1z);
    // connect mMaster->sgaterrinN(sgaterrinN_sig) (N = 00..91)
    //{{{
    mMaster->sgaterrin00(sgaterrin00_sig);
    mMaster->sgaterrin01(sgaterrin01_sig);
    mMaster->sgaterrin02(sgaterrin02_sig);
    mMaster->sgaterrin03(sgaterrin03_sig);
    mMaster->sgaterrin04(sgaterrin04_sig);
    mMaster->sgaterrin05(sgaterrin05_sig);
    mMaster->sgaterrin06(sgaterrin06_sig);
    mMaster->sgaterrin07(sgaterrin07_sig);
    mMaster->sgaterrin08(sgaterrin08_sig);
    mMaster->sgaterrin09(sgaterrin09_sig);
    mMaster->sgaterrin10(sgaterrin10_sig);
    mMaster->sgaterrin11(sgaterrin11_sig);
    mMaster->sgaterrin12(sgaterrin12_sig);
    mMaster->sgaterrin13(sgaterrin13_sig);
    mMaster->sgaterrin14(sgaterrin14_sig);
    mMaster->sgaterrin15(sgaterrin15_sig);
    mMaster->sgaterrin16(sgaterrin16_sig);
    mMaster->sgaterrin17(sgaterrin17_sig);
    mMaster->sgaterrin18(sgaterrin18_sig);
    mMaster->sgaterrin19(sgaterrin19_sig);
    mMaster->sgaterrin20(sgaterrin20_sig);
    mMaster->sgaterrin21(sgaterrin21_sig);
    mMaster->sgaterrin22(sgaterrin22_sig);
    mMaster->sgaterrin23(sgaterrin23_sig);
    mMaster->sgaterrin24(sgaterrin24_sig);
    mMaster->sgaterrin25(sgaterrin25_sig);
    mMaster->sgaterrin26(sgaterrin26_sig);
    mMaster->sgaterrin27(sgaterrin27_sig);
    mMaster->sgaterrin28(sgaterrin28_sig);
    mMaster->sgaterrin29(sgaterrin29_sig);
    mMaster->sgaterrin30(sgaterrin30_sig);
    mMaster->sgaterrin31(sgaterrin31_sig);
    mMaster->sgaterrin32(sgaterrin32_sig);
    mMaster->sgaterrin33(sgaterrin33_sig);
    mMaster->sgaterrin34(sgaterrin34_sig);
    mMaster->sgaterrin35(sgaterrin35_sig);
    mMaster->sgaterrin36(sgaterrin36_sig);
    mMaster->sgaterrin37(sgaterrin37_sig);
    mMaster->sgaterrin38(sgaterrin38_sig);
    mMaster->sgaterrin39(sgaterrin39_sig);
    mMaster->sgaterrin40(sgaterrin40_sig);
    mMaster->sgaterrin41(sgaterrin41_sig);
    mMaster->sgaterrin42(sgaterrin42_sig);
    mMaster->sgaterrin43(sgaterrin43_sig);
    mMaster->sgaterrin44(sgaterrin44_sig);
    mMaster->sgaterrin45(sgaterrin45_sig);
    mMaster->sgaterrin46(sgaterrin46_sig);
    mMaster->sgaterrin47(sgaterrin47_sig);
    mMaster->sgaterrin48(sgaterrin48_sig);
    mMaster->sgaterrin49(sgaterrin49_sig);
    mMaster->sgaterrin50(sgaterrin50_sig);
    mMaster->sgaterrin51(sgaterrin51_sig);
    mMaster->sgaterrin52(sgaterrin52_sig);
    mMaster->sgaterrin53(sgaterrin53_sig);
    mMaster->sgaterrin54(sgaterrin54_sig);
    mMaster->sgaterrin55(sgaterrin55_sig);
    mMaster->sgaterrin56(sgaterrin56_sig);
    mMaster->sgaterrin57(sgaterrin57_sig);
    mMaster->sgaterrin58(sgaterrin58_sig);
    mMaster->sgaterrin59(sgaterrin59_sig);
    mMaster->sgaterrin60(sgaterrin60_sig);
    mMaster->sgaterrin61(sgaterrin61_sig);
    mMaster->sgaterrin62(sgaterrin62_sig);
    mMaster->sgaterrin63(sgaterrin63_sig);
    mMaster->sgaterrin64(sgaterrin64_sig);
    mMaster->sgaterrin65(sgaterrin65_sig);
    mMaster->sgaterrin66(sgaterrin66_sig);
    mMaster->sgaterrin67(sgaterrin67_sig);
    mMaster->sgaterrin68(sgaterrin68_sig);
    mMaster->sgaterrin69(sgaterrin69_sig);
    mMaster->sgaterrin70(sgaterrin70_sig);
    mMaster->sgaterrin71(sgaterrin71_sig);
    mMaster->sgaterrin72(sgaterrin72_sig);
    mMaster->sgaterrin73(sgaterrin73_sig);
    mMaster->sgaterrin74(sgaterrin74_sig);
    mMaster->sgaterrin75(sgaterrin75_sig);
    mMaster->sgaterrin76(sgaterrin76_sig);
    mMaster->sgaterrin77(sgaterrin77_sig);
    mMaster->sgaterrin78(sgaterrin78_sig);
    mMaster->sgaterrin79(sgaterrin79_sig);
    mMaster->sgaterrin80(sgaterrin80_sig);
    mMaster->sgaterrin81(sgaterrin81_sig);
    mMaster->sgaterrin82(sgaterrin82_sig);
    mMaster->sgaterrin83(sgaterrin83_sig);
    mMaster->sgaterrin84(sgaterrin84_sig);
    mMaster->sgaterrin85(sgaterrin85_sig);
    mMaster->sgaterrin86(sgaterrin86_sig);
    mMaster->sgaterrin87(sgaterrin87_sig);
    mMaster->sgaterrin88(sgaterrin88_sig);
    mMaster->sgaterrin89(sgaterrin89_sig);
    mMaster->sgaterrin90(sgaterrin90_sig);
    mMaster->sgaterrin91(sgaterrin91_sig);
    mMaster->sgaterrin92(master_sgaterrin92_sig);
    //}}}

    // connect mChecker->sgaterrinN(sgaterrinN_sig) (N = 00..91)
    //{{{
    mChecker->sgaterrin00(sgaterrin00_sig);
    mChecker->sgaterrin01(sgaterrin01_sig);
    mChecker->sgaterrin02(sgaterrin02_sig);
    mChecker->sgaterrin03(sgaterrin03_sig);
    mChecker->sgaterrin04(sgaterrin04_sig);
    mChecker->sgaterrin05(sgaterrin05_sig);
    mChecker->sgaterrin06(sgaterrin06_sig);
    mChecker->sgaterrin07(sgaterrin07_sig);
    mChecker->sgaterrin08(sgaterrin08_sig);
    mChecker->sgaterrin09(sgaterrin09_sig);
    mChecker->sgaterrin10(sgaterrin10_sig);
    mChecker->sgaterrin11(sgaterrin11_sig);
    mChecker->sgaterrin12(sgaterrin12_sig);
    mChecker->sgaterrin13(sgaterrin13_sig);
    mChecker->sgaterrin14(sgaterrin14_sig);
    mChecker->sgaterrin15(sgaterrin15_sig);
    mChecker->sgaterrin16(sgaterrin16_sig);
    mChecker->sgaterrin17(sgaterrin17_sig);
    mChecker->sgaterrin18(sgaterrin18_sig);
    mChecker->sgaterrin19(sgaterrin19_sig);
    mChecker->sgaterrin20(sgaterrin20_sig);
    mChecker->sgaterrin21(sgaterrin21_sig);
    mChecker->sgaterrin22(sgaterrin22_sig);
    mChecker->sgaterrin23(sgaterrin23_sig);
    mChecker->sgaterrin24(sgaterrin24_sig);
    mChecker->sgaterrin25(sgaterrin25_sig);
    mChecker->sgaterrin26(sgaterrin26_sig);
    mChecker->sgaterrin27(sgaterrin27_sig);
    mChecker->sgaterrin28(sgaterrin28_sig);
    mChecker->sgaterrin29(sgaterrin29_sig);
    mChecker->sgaterrin30(sgaterrin30_sig);
    mChecker->sgaterrin31(sgaterrin31_sig);
    mChecker->sgaterrin32(sgaterrin32_sig);
    mChecker->sgaterrin33(sgaterrin33_sig);
    mChecker->sgaterrin34(sgaterrin34_sig);
    mChecker->sgaterrin35(sgaterrin35_sig);
    mChecker->sgaterrin36(sgaterrin36_sig);
    mChecker->sgaterrin37(sgaterrin37_sig);
    mChecker->sgaterrin38(sgaterrin38_sig);
    mChecker->sgaterrin39(sgaterrin39_sig);
    mChecker->sgaterrin40(sgaterrin40_sig);
    mChecker->sgaterrin41(sgaterrin41_sig);
    mChecker->sgaterrin42(sgaterrin42_sig);
    mChecker->sgaterrin43(sgaterrin43_sig);
    mChecker->sgaterrin44(sgaterrin44_sig);
    mChecker->sgaterrin45(sgaterrin45_sig);
    mChecker->sgaterrin46(sgaterrin46_sig);
    mChecker->sgaterrin47(sgaterrin47_sig);
    mChecker->sgaterrin48(sgaterrin48_sig);
    mChecker->sgaterrin49(sgaterrin49_sig);
    mChecker->sgaterrin50(sgaterrin50_sig);
    mChecker->sgaterrin51(sgaterrin51_sig);
    mChecker->sgaterrin52(sgaterrin52_sig);
    mChecker->sgaterrin53(sgaterrin53_sig);
    mChecker->sgaterrin54(sgaterrin54_sig);
    mChecker->sgaterrin55(sgaterrin55_sig);
    mChecker->sgaterrin56(sgaterrin56_sig);
    mChecker->sgaterrin57(sgaterrin57_sig);
    mChecker->sgaterrin58(sgaterrin58_sig);
    mChecker->sgaterrin59(sgaterrin59_sig);
    mChecker->sgaterrin60(sgaterrin60_sig);
    mChecker->sgaterrin61(sgaterrin61_sig);
    mChecker->sgaterrin62(sgaterrin62_sig);
    mChecker->sgaterrin63(sgaterrin63_sig);
    mChecker->sgaterrin64(sgaterrin64_sig);
    mChecker->sgaterrin65(sgaterrin65_sig);
    mChecker->sgaterrin66(sgaterrin66_sig);
    mChecker->sgaterrin67(sgaterrin67_sig);
    mChecker->sgaterrin68(sgaterrin68_sig);
    mChecker->sgaterrin69(sgaterrin69_sig);
    mChecker->sgaterrin70(sgaterrin70_sig);
    mChecker->sgaterrin71(sgaterrin71_sig);
    mChecker->sgaterrin72(sgaterrin72_sig);
    mChecker->sgaterrin73(sgaterrin73_sig);
    mChecker->sgaterrin74(sgaterrin74_sig);
    mChecker->sgaterrin75(sgaterrin75_sig);
    mChecker->sgaterrin76(sgaterrin76_sig);
    mChecker->sgaterrin77(sgaterrin77_sig);
    mChecker->sgaterrin78(sgaterrin78_sig);
    mChecker->sgaterrin79(sgaterrin79_sig);
    mChecker->sgaterrin80(sgaterrin80_sig);
    mChecker->sgaterrin81(sgaterrin81_sig);
    mChecker->sgaterrin82(sgaterrin82_sig);
    mChecker->sgaterrin83(sgaterrin83_sig);
    mChecker->sgaterrin84(sgaterrin84_sig);
    mChecker->sgaterrin85(sgaterrin85_sig);
    mChecker->sgaterrin86(sgaterrin86_sig);
    mChecker->sgaterrin87(sgaterrin87_sig);
    mChecker->sgaterrin88(sgaterrin88_sig);
    mChecker->sgaterrin89(sgaterrin89_sig);
    mChecker->sgaterrin90(sgaterrin90_sig);
    mChecker->sgaterrin91(sgaterrin91_sig);
    mChecker->sgaterrin92(checker_sgaterrin92_sig);
    //}}}

    //connect mMaster->sgatpeN(master_sgatpeN_sig) (N = 000.. 090)
    //{{{
    mMaster->sgatpe000(master_sgatpe000_sig);
    mMaster->sgatpe001(master_sgatpe001_sig);
    mMaster->sgatpe002(master_sgatpe002_sig);
    mMaster->sgatpe003(master_sgatpe003_sig);
    mMaster->sgatpe004(master_sgatpe004_sig);
    mMaster->sgatpe005(master_sgatpe005_sig);
    mMaster->sgatpe006(master_sgatpe006_sig);
    mMaster->sgatpe007(master_sgatpe007_sig);
    mMaster->sgatpe008(master_sgatpe008_sig);
    mMaster->sgatpe009(master_sgatpe009_sig);
    mMaster->sgatpe010(master_sgatpe010_sig);
    mMaster->sgatpe011(master_sgatpe011_sig);
    mMaster->sgatpe012(master_sgatpe012_sig);
    mMaster->sgatpe013(master_sgatpe013_sig);
    mMaster->sgatpe014(master_sgatpe014_sig);
    mMaster->sgatpe015(master_sgatpe015_sig);
    mMaster->sgatpe016(master_sgatpe016_sig);
    mMaster->sgatpe017(master_sgatpe017_sig);
    mMaster->sgatpe018(master_sgatpe018_sig);
    mMaster->sgatpe019(master_sgatpe019_sig);
    mMaster->sgatpe020(master_sgatpe020_sig);
    mMaster->sgatpe021(master_sgatpe021_sig);
    mMaster->sgatpe022(master_sgatpe022_sig);
    mMaster->sgatpe023(master_sgatpe023_sig);
    mMaster->sgatpe024(master_sgatpe024_sig);
    mMaster->sgatpe025(master_sgatpe025_sig);
    mMaster->sgatpe026(master_sgatpe026_sig);
    mMaster->sgatpe027(master_sgatpe027_sig);
    mMaster->sgatpe028(master_sgatpe028_sig);
    mMaster->sgatpe029(master_sgatpe029_sig);
    mMaster->sgatpe030(master_sgatpe030_sig);
    mMaster->sgatpe031(master_sgatpe031_sig);
    mMaster->sgatpe032(master_sgatpe032_sig);
    mMaster->sgatpe033(master_sgatpe033_sig);
    mMaster->sgatpe034(master_sgatpe034_sig);
    mMaster->sgatpe035(master_sgatpe035_sig);
    mMaster->sgatpe036(master_sgatpe036_sig);
    mMaster->sgatpe037(master_sgatpe037_sig);
    mMaster->sgatpe038(master_sgatpe038_sig);
    mMaster->sgatpe039(master_sgatpe039_sig);
    mMaster->sgatpe040(master_sgatpe040_sig);
    mMaster->sgatpe041(master_sgatpe041_sig);
    mMaster->sgatpe042(master_sgatpe042_sig);
    mMaster->sgatpe043(master_sgatpe043_sig);
    mMaster->sgatpe044(master_sgatpe044_sig);
    mMaster->sgatpe045(master_sgatpe045_sig);
    mMaster->sgatpe046(master_sgatpe046_sig);
    mMaster->sgatpe047(master_sgatpe047_sig);
    mMaster->sgatpe048(master_sgatpe048_sig);
    mMaster->sgatpe049(master_sgatpe049_sig);
    mMaster->sgatpe050(master_sgatpe050_sig);
    mMaster->sgatpe051(master_sgatpe051_sig);
    mMaster->sgatpe052(master_sgatpe052_sig);
    mMaster->sgatpe053(master_sgatpe053_sig);
    mMaster->sgatpe054(master_sgatpe054_sig);
    mMaster->sgatpe055(master_sgatpe055_sig);
    mMaster->sgatpe056(master_sgatpe056_sig);
    mMaster->sgatpe057(master_sgatpe057_sig);
    mMaster->sgatpe058(master_sgatpe058_sig);
    mMaster->sgatpe059(master_sgatpe059_sig);
    mMaster->sgatpe060(master_sgatpe060_sig);
    mMaster->sgatpe061(master_sgatpe061_sig);
    mMaster->sgatpe062(master_sgatpe062_sig);
    mMaster->sgatpe063(master_sgatpe063_sig);
    mMaster->sgatpe064(master_sgatpe064_sig);
    mMaster->sgatpe065(master_sgatpe065_sig);
    mMaster->sgatpe066(master_sgatpe066_sig);
    mMaster->sgatpe067(master_sgatpe067_sig);
    mMaster->sgatpe068(master_sgatpe068_sig);
    mMaster->sgatpe069(master_sgatpe069_sig);
    mMaster->sgatpe070(master_sgatpe070_sig);
    mMaster->sgatpe071(master_sgatpe071_sig);
    mMaster->sgatpe072(master_sgatpe072_sig);
    mMaster->sgatpe073(master_sgatpe073_sig);
    mMaster->sgatpe074(master_sgatpe074_sig);
    mMaster->sgatpe075(master_sgatpe075_sig);
    mMaster->sgatpe076(master_sgatpe076_sig);
    mMaster->sgatpe077(master_sgatpe077_sig);
    mMaster->sgatpe078(master_sgatpe078_sig);
    mMaster->sgatpe079(master_sgatpe079_sig);
    mMaster->sgatpe080(master_sgatpe080_sig);
    mMaster->sgatpe081(master_sgatpe081_sig);
    mMaster->sgatpe082(master_sgatpe082_sig);
    mMaster->sgatpe083(master_sgatpe083_sig);
    mMaster->sgatpe084(master_sgatpe084_sig);
    mMaster->sgatpe085(master_sgatpe085_sig);
    mMaster->sgatpe086(master_sgatpe086_sig);
    mMaster->sgatpe087(master_sgatpe087_sig);
    mMaster->sgatpe088(master_sgatpe088_sig);
    mMaster->sgatpe089(master_sgatpe089_sig);
    mMaster->sgatpe090(master_sgatpe090_sig);
    mMaster->sgatpe091(master_sgatpe091_sig);
    mMaster->sgatpe092(master_sgatpe092_sig);
    //}}}
    //connect mChecker->sgatpeN(checker_sgatpeN_sig) (N = 000.. 090)
    //{{{
    mChecker->sgatpe000(checker_sgatpe000_sig);
    mChecker->sgatpe001(checker_sgatpe001_sig);
    mChecker->sgatpe002(checker_sgatpe002_sig);
    mChecker->sgatpe003(checker_sgatpe003_sig);
    mChecker->sgatpe004(checker_sgatpe004_sig);
    mChecker->sgatpe005(checker_sgatpe005_sig);
    mChecker->sgatpe006(checker_sgatpe006_sig);
    mChecker->sgatpe007(checker_sgatpe007_sig);
    mChecker->sgatpe008(checker_sgatpe008_sig);
    mChecker->sgatpe009(checker_sgatpe009_sig);
    mChecker->sgatpe010(checker_sgatpe010_sig);
    mChecker->sgatpe011(checker_sgatpe011_sig);
    mChecker->sgatpe012(checker_sgatpe012_sig);
    mChecker->sgatpe013(checker_sgatpe013_sig);
    mChecker->sgatpe014(checker_sgatpe014_sig);
    mChecker->sgatpe015(checker_sgatpe015_sig);
    mChecker->sgatpe016(checker_sgatpe016_sig);
    mChecker->sgatpe017(checker_sgatpe017_sig);
    mChecker->sgatpe018(checker_sgatpe018_sig);
    mChecker->sgatpe019(checker_sgatpe019_sig);
    mChecker->sgatpe020(checker_sgatpe020_sig);
    mChecker->sgatpe021(checker_sgatpe021_sig);
    mChecker->sgatpe022(checker_sgatpe022_sig);
    mChecker->sgatpe023(checker_sgatpe023_sig);
    mChecker->sgatpe024(checker_sgatpe024_sig);
    mChecker->sgatpe025(checker_sgatpe025_sig);
    mChecker->sgatpe026(checker_sgatpe026_sig);
    mChecker->sgatpe027(checker_sgatpe027_sig);
    mChecker->sgatpe028(checker_sgatpe028_sig);
    mChecker->sgatpe029(checker_sgatpe029_sig);
    mChecker->sgatpe030(checker_sgatpe030_sig);
    mChecker->sgatpe031(checker_sgatpe031_sig);
    mChecker->sgatpe032(checker_sgatpe032_sig);
    mChecker->sgatpe033(checker_sgatpe033_sig);
    mChecker->sgatpe034(checker_sgatpe034_sig);
    mChecker->sgatpe035(checker_sgatpe035_sig);
    mChecker->sgatpe036(checker_sgatpe036_sig);
    mChecker->sgatpe037(checker_sgatpe037_sig);
    mChecker->sgatpe038(checker_sgatpe038_sig);
    mChecker->sgatpe039(checker_sgatpe039_sig);
    mChecker->sgatpe040(checker_sgatpe040_sig);
    mChecker->sgatpe041(checker_sgatpe041_sig);
    mChecker->sgatpe042(checker_sgatpe042_sig);
    mChecker->sgatpe043(checker_sgatpe043_sig);
    mChecker->sgatpe044(checker_sgatpe044_sig);
    mChecker->sgatpe045(checker_sgatpe045_sig);
    mChecker->sgatpe046(checker_sgatpe046_sig);
    mChecker->sgatpe047(checker_sgatpe047_sig);
    mChecker->sgatpe048(checker_sgatpe048_sig);
    mChecker->sgatpe049(checker_sgatpe049_sig);
    mChecker->sgatpe050(checker_sgatpe050_sig);
    mChecker->sgatpe051(checker_sgatpe051_sig);
    mChecker->sgatpe052(checker_sgatpe052_sig);
    mChecker->sgatpe053(checker_sgatpe053_sig);
    mChecker->sgatpe054(checker_sgatpe054_sig);
    mChecker->sgatpe055(checker_sgatpe055_sig);
    mChecker->sgatpe056(checker_sgatpe056_sig);
    mChecker->sgatpe057(checker_sgatpe057_sig);
    mChecker->sgatpe058(checker_sgatpe058_sig);
    mChecker->sgatpe059(checker_sgatpe059_sig);
    mChecker->sgatpe060(checker_sgatpe060_sig);
    mChecker->sgatpe061(checker_sgatpe061_sig);
    mChecker->sgatpe062(checker_sgatpe062_sig);
    mChecker->sgatpe063(checker_sgatpe063_sig);
    mChecker->sgatpe064(checker_sgatpe064_sig);
    mChecker->sgatpe065(checker_sgatpe065_sig);
    mChecker->sgatpe066(checker_sgatpe066_sig);
    mChecker->sgatpe067(checker_sgatpe067_sig);
    mChecker->sgatpe068(checker_sgatpe068_sig);
    mChecker->sgatpe069(checker_sgatpe069_sig);
    mChecker->sgatpe070(checker_sgatpe070_sig);
    mChecker->sgatpe071(checker_sgatpe071_sig);
    mChecker->sgatpe072(checker_sgatpe072_sig);
    mChecker->sgatpe073(checker_sgatpe073_sig);
    mChecker->sgatpe074(checker_sgatpe074_sig);
    mChecker->sgatpe075(checker_sgatpe075_sig);
    mChecker->sgatpe076(checker_sgatpe076_sig);
    mChecker->sgatpe077(checker_sgatpe077_sig);
    mChecker->sgatpe078(checker_sgatpe078_sig);
    mChecker->sgatpe079(checker_sgatpe079_sig);
    mChecker->sgatpe080(checker_sgatpe080_sig);
    mChecker->sgatpe081(checker_sgatpe081_sig);
    mChecker->sgatpe082(checker_sgatpe082_sig);
    mChecker->sgatpe083(checker_sgatpe083_sig);
    mChecker->sgatpe084(checker_sgatpe084_sig);
    mChecker->sgatpe085(checker_sgatpe085_sig);
    mChecker->sgatpe086(checker_sgatpe086_sig);
    mChecker->sgatpe087(checker_sgatpe087_sig);
    mChecker->sgatpe088(checker_sgatpe088_sig);
    mChecker->sgatpe089(checker_sgatpe089_sig);
    mChecker->sgatpe090(checker_sgatpe090_sig);
    mChecker->sgatpe091(checker_sgatpe091_sig);
    mChecker->sgatpe092(checker_sgatpe092_sig);
    //}}}
}//}}}

void Cecm_wp::InitializeInternalSignal()
{//{{{
    mSgoterrin               = false;
    //Set sgaterrinN_sig  = false (N = 00 .. 92)
    //{{{
    sgaterrin00_sig = false;
    sgaterrin01_sig = false;
    sgaterrin02_sig = false;
    sgaterrin03_sig = false;
    sgaterrin04_sig = false;
    sgaterrin05_sig = false;
    sgaterrin06_sig = false;
    sgaterrin07_sig = false;
    sgaterrin08_sig = false;
    sgaterrin09_sig = false;
    sgaterrin10_sig = false;
    sgaterrin11_sig = false;
    sgaterrin12_sig = false;
    sgaterrin13_sig = false;
    sgaterrin14_sig = false;
    sgaterrin15_sig = false;
    sgaterrin16_sig = false;
    sgaterrin17_sig = false;
    sgaterrin18_sig = false;
    sgaterrin19_sig = false;
    sgaterrin20_sig = false;
    sgaterrin21_sig = false;
    sgaterrin22_sig = false;
    sgaterrin23_sig = false;
    sgaterrin24_sig = false;
    sgaterrin25_sig = false;
    sgaterrin26_sig = false;
    sgaterrin27_sig = false;
    sgaterrin28_sig = false;
    sgaterrin29_sig = false;
    sgaterrin30_sig = false;
    sgaterrin31_sig = false;
    sgaterrin32_sig = false;
    sgaterrin33_sig = false;
    sgaterrin34_sig = false;
    sgaterrin35_sig = false;
    sgaterrin36_sig = false;
    sgaterrin37_sig = false;
    sgaterrin38_sig = false;
    sgaterrin39_sig = false;
    sgaterrin40_sig = false;
    sgaterrin41_sig = false;
    sgaterrin42_sig = false;
    sgaterrin43_sig = false;
    sgaterrin44_sig = false;
    sgaterrin45_sig = false;
    sgaterrin46_sig = false;
    sgaterrin47_sig = false;
    sgaterrin48_sig = false;
    sgaterrin49_sig = false;
    sgaterrin50_sig = false;
    sgaterrin51_sig = false;
    sgaterrin52_sig = false;
    sgaterrin53_sig = false;
    sgaterrin54_sig = false;
    sgaterrin55_sig = false;
    sgaterrin56_sig = false;
    sgaterrin57_sig = false;
    sgaterrin58_sig = false;
    sgaterrin59_sig = false;
    sgaterrin60_sig = false;
    sgaterrin61_sig = false;
    sgaterrin62_sig = false;
    sgaterrin63_sig = false;
    sgaterrin64_sig = false;
    sgaterrin65_sig = false;
    sgaterrin66_sig = false;
    sgaterrin67_sig = false;
    sgaterrin68_sig = false;
    sgaterrin69_sig = false;
    sgaterrin70_sig = false;
    sgaterrin71_sig = false;
    sgaterrin72_sig = false;
    sgaterrin73_sig = false;
    sgaterrin74_sig = false;
    sgaterrin75_sig = false;
    sgaterrin76_sig = false;
    sgaterrin77_sig = false;
    sgaterrin78_sig = false;
    sgaterrin79_sig = false;
    sgaterrin80_sig = false;
    sgaterrin81_sig = false;
    sgaterrin82_sig = false;
    sgaterrin83_sig = false;
    sgaterrin84_sig = false;
    sgaterrin85_sig = false;
    sgaterrin86_sig = false;
    sgaterrin87_sig = false;
    sgaterrin88_sig = false;
    sgaterrin89_sig = false;
    sgaterrin90_sig = false;
    sgaterrin91_sig = false;
    master_sgaterrin92_sig = false;
    checker_sgaterrin92_sig = false;
    //}}}
    master_sgaterroz_sig    = false;
    master_sgaterroutz_sig  = false;
    master_sgati_sig        = false;
    master_sgatnmi_sig      = false;
    master_sgatresz_sig     = true;
    checker_sgaterroz_sig   = false;
    checker_sgaterroutz_sig = false;
    checker_sgati_sig       = false;
    checker_sgatnmi_sig     = false;
    checker_sgatresz_sig    = true;
    errout_clear_mask_sig   = true;
    errout_clear_mask_checker = true;
    //Set master_sgatpeN_sig  = false(N = 000 .. 092)
    //{{{
    master_sgatpe000_sig = false;
    master_sgatpe001_sig = false;
    master_sgatpe002_sig = false;
    master_sgatpe003_sig = false;
    master_sgatpe004_sig = false;
    master_sgatpe005_sig = false;
    master_sgatpe006_sig = false;
    master_sgatpe007_sig = false;
    master_sgatpe008_sig = false;
    master_sgatpe009_sig = false;
    master_sgatpe010_sig = false;
    master_sgatpe011_sig = false;
    master_sgatpe012_sig = false;
    master_sgatpe013_sig = false;
    master_sgatpe014_sig = false;
    master_sgatpe015_sig = false;
    master_sgatpe016_sig = false;
    master_sgatpe017_sig = false;
    master_sgatpe018_sig = false;
    master_sgatpe019_sig = false;
    master_sgatpe020_sig = false;
    master_sgatpe021_sig = false;
    master_sgatpe022_sig = false;
    master_sgatpe023_sig = false;
    master_sgatpe024_sig = false;
    master_sgatpe025_sig = false;
    master_sgatpe026_sig = false;
    master_sgatpe027_sig = false;
    master_sgatpe028_sig = false;
    master_sgatpe029_sig = false;
    master_sgatpe030_sig = false;
    master_sgatpe031_sig = false;
    master_sgatpe032_sig = false;
    master_sgatpe033_sig = false;
    master_sgatpe034_sig = false;
    master_sgatpe035_sig = false;
    master_sgatpe036_sig = false;
    master_sgatpe037_sig = false;
    master_sgatpe038_sig = false;
    master_sgatpe039_sig = false;
    master_sgatpe040_sig = false;
    master_sgatpe041_sig = false;
    master_sgatpe042_sig = false;
    master_sgatpe043_sig = false;
    master_sgatpe044_sig = false;
    master_sgatpe045_sig = false;
    master_sgatpe046_sig = false;
    master_sgatpe047_sig = false;
    master_sgatpe048_sig = false;
    master_sgatpe049_sig = false;
    master_sgatpe050_sig = false;
    master_sgatpe051_sig = false;
    master_sgatpe052_sig = false;
    master_sgatpe053_sig = false;
    master_sgatpe054_sig = false;
    master_sgatpe055_sig = false;
    master_sgatpe056_sig = false;
    master_sgatpe057_sig = false;
    master_sgatpe058_sig = false;
    master_sgatpe059_sig = false;
    master_sgatpe060_sig = false;
    master_sgatpe061_sig = false;
    master_sgatpe062_sig = false;
    master_sgatpe063_sig = false;
    master_sgatpe064_sig = false;
    master_sgatpe065_sig = false;
    master_sgatpe066_sig = false;
    master_sgatpe067_sig = false;
    master_sgatpe068_sig = false;
    master_sgatpe069_sig = false;
    master_sgatpe070_sig = false;
    master_sgatpe071_sig = false;
    master_sgatpe072_sig = false;
    master_sgatpe073_sig = false;
    master_sgatpe074_sig = false;
    master_sgatpe075_sig = false;
    master_sgatpe076_sig = false;
    master_sgatpe077_sig = false;
    master_sgatpe078_sig = false;
    master_sgatpe079_sig = false;
    master_sgatpe080_sig = false;
    master_sgatpe081_sig = false;
    master_sgatpe082_sig = false;
    master_sgatpe083_sig = false;
    master_sgatpe084_sig = false;
    master_sgatpe085_sig = false;
    master_sgatpe086_sig = false;
    master_sgatpe087_sig = false;
    master_sgatpe088_sig = false;
    master_sgatpe089_sig = false;
    master_sgatpe090_sig = false;
    master_sgatpe091_sig = false;
    master_sgatpe092_sig = false;
    //}}}
    //Set checker_sgatpeN_sig  = false(N = 000 .. 092)
    //{{{
    checker_sgatpe000_sig = false;
    checker_sgatpe001_sig = false;
    checker_sgatpe002_sig = false;
    checker_sgatpe003_sig = false;
    checker_sgatpe004_sig = false;
    checker_sgatpe005_sig = false;
    checker_sgatpe006_sig = false;
    checker_sgatpe007_sig = false;
    checker_sgatpe008_sig = false;
    checker_sgatpe009_sig = false;
    checker_sgatpe010_sig = false;
    checker_sgatpe011_sig = false;
    checker_sgatpe012_sig = false;
    checker_sgatpe013_sig = false;
    checker_sgatpe014_sig = false;
    checker_sgatpe015_sig = false;
    checker_sgatpe016_sig = false;
    checker_sgatpe017_sig = false;
    checker_sgatpe018_sig = false;
    checker_sgatpe019_sig = false;
    checker_sgatpe020_sig = false;
    checker_sgatpe021_sig = false;
    checker_sgatpe022_sig = false;
    checker_sgatpe023_sig = false;
    checker_sgatpe024_sig = false;
    checker_sgatpe025_sig = false;
    checker_sgatpe026_sig = false;
    checker_sgatpe027_sig = false;
    checker_sgatpe028_sig = false;
    checker_sgatpe029_sig = false;
    checker_sgatpe030_sig = false;
    checker_sgatpe031_sig = false;
    checker_sgatpe032_sig = false;
    checker_sgatpe033_sig = false;
    checker_sgatpe034_sig = false;
    checker_sgatpe035_sig = false;
    checker_sgatpe036_sig = false;
    checker_sgatpe037_sig = false;
    checker_sgatpe038_sig = false;
    checker_sgatpe039_sig = false;
    checker_sgatpe040_sig = false;
    checker_sgatpe041_sig = false;
    checker_sgatpe042_sig = false;
    checker_sgatpe043_sig = false;
    checker_sgatpe044_sig = false;
    checker_sgatpe045_sig = false;
    checker_sgatpe046_sig = false;
    checker_sgatpe047_sig = false;
    checker_sgatpe048_sig = false;
    checker_sgatpe049_sig = false;
    checker_sgatpe050_sig = false;
    checker_sgatpe051_sig = false;
    checker_sgatpe052_sig = false;
    checker_sgatpe053_sig = false;
    checker_sgatpe054_sig = false;
    checker_sgatpe055_sig = false;
    checker_sgatpe056_sig = false;
    checker_sgatpe057_sig = false;
    checker_sgatpe058_sig = false;
    checker_sgatpe059_sig = false;
    checker_sgatpe060_sig = false;
    checker_sgatpe061_sig = false;
    checker_sgatpe062_sig = false;
    checker_sgatpe063_sig = false;
    checker_sgatpe064_sig = false;
    checker_sgatpe065_sig = false;
    checker_sgatpe066_sig = false;
    checker_sgatpe067_sig = false;
    checker_sgatpe068_sig = false;
    checker_sgatpe069_sig = false;
    checker_sgatpe070_sig = false;
    checker_sgatpe071_sig = false;
    checker_sgatpe072_sig = false;
    checker_sgatpe073_sig = false;
    checker_sgatpe074_sig = false;
    checker_sgatpe075_sig = false;
    checker_sgatpe076_sig = false;
    checker_sgatpe077_sig = false;
    checker_sgatpe078_sig = false;
    checker_sgatpe079_sig = false;
    checker_sgatpe080_sig = false;
    checker_sgatpe081_sig = false;
    checker_sgatpe082_sig = false;
    checker_sgatpe083_sig = false;
    checker_sgatpe084_sig = false;
    checker_sgatpe085_sig = false;
    checker_sgatpe086_sig = false;
    checker_sgatpe087_sig = false;
    checker_sgatpe088_sig = false;
    checker_sgatpe089_sig = false;
    checker_sgatpe090_sig = false;
    checker_sgatpe091_sig = false;
    checker_sgatpe092_sig = false;
    //}}}
    checker_psout_sig       = false;
    master_psout_sig        = false;
    dtmtrgout_sig = false;
}//}}}

void Cecm_wp::DumpInfo(const char *type, const char *message,...)
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

void Cecm_wp::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
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
            status = mMaster->ECM_common_tgt_rd(false, ((unsigned int)addr | emSizeofCommon), p_data, size);
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
            bool temp1 = mMaster->ECM_common_tgt_wr(false, ((unsigned int)addr | emSizeofCommon), p_data, size);
            bool temp2 = mChecker->ECM_common_tgt_wr(false, ((unsigned int)addr | emSizeofCommon), p_data, size);
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

unsigned int Cecm_wp::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
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
            status = mMaster->ECM_common_tgt_rd(true, ((unsigned int)addr | emSizeofCommon), p_data, size);
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
            bool temp1 = mMaster->ECM_common_tgt_wr(true, ((unsigned int)addr | emSizeofCommon), p_data, size);
            bool temp2 = mChecker->ECM_common_tgt_wr(true, ((unsigned int)addr | emSizeofCommon), p_data, size);
            status = temp1 && temp2;
        }else if(id == emMasterId){
            status = mMaster->ECM_common_tgt_wr(true, (unsigned int)addr, p_data, size);
        }else{
            status = mChecker->ECM_common_tgt_wr(true, (unsigned int)addr, p_data, size);
        }    
    //No read/write access    
    } else {
        status = true;
        size = 0;
    }
    
    trans.set_response_status(tlm::TLM_OK_RESPONSE); 
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

double Cecm_wp::GetTimeResolution(void)
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

void Cecm_wp::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{  
    if ((clk_name != "pclk")&&(clk_name != "cntclk")) {
        re_printf("error","Clock name is invalid.\n");
        return;
    }
    
    double clk_period = 0 ;
    if(clk_freq == 0){
        //cancel all processes of ecm_wp
        mWriteOutputPortEvent.cancel();
        mReceiveExternalErrorEvent.cancel();
        mReceivePCLKEvent.cancel();
        mReceiveSVACCESSEvent.cancel();

        //cancel processes of ecm
        mMaster ->ECMSetZeroClock();
        mChecker->ECMSetZeroClock(); 
    } else {
        clk_period  = (double)(emNanoSecond / clk_freq);
        double time_unit    = (double)(emNanoSecond / GetTimeResolution());
        if(time_unit > clk_period){
            re_printf("warning","The %s period is less than 1 unit time of system.\n", clk_name.c_str());
            return;
        }
    }
    
    if (clk_name == "pclk") {
        mFreqPCLK  = clk_freq;
        mMaster ->mPCLKClock = clk_freq;
        mChecker->mPCLKClock = clk_freq;
        for(unsigned int id = 0; id < emMaxId; id++){
            SetLatency_TLM(id, clk_period, false);
        }
    } else { //cntclk clock
        mFreqCNTCLK = clk_freq;
        mMaster ->mCountClock = clk_period;
        mChecker->mCountClock = clk_period;
    }
   
    re_printf("info","The %s is set with a frequency as %lld.%06d\n",clk_name.c_str(), (unsigned long long)clk_freq, FRACTION_TO_INT(clk_freq));

}//}}}

void Cecm_wp::SetLatency_TLM(const unsigned int id, const double pclk_period, const bool is_constructor)
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
