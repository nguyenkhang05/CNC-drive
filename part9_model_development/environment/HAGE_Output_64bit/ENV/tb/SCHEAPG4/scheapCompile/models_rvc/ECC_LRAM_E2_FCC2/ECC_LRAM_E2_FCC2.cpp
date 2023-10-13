// ---------------------------------------------------------------------
// $Id: ECC_LRAM_E2_FCC2.cpp,v 1.17 2019/10/23 08:29:18 landang Exp $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "ECC_def.h"
#include "ECC_LRAM_E2_FCC2.h"
#include "ECCCNT_LR0_PEn_FCC2_Func.h"
#include "MECCCAP_LR0_PEn_FCC2_Func.h"
#include "MECCCAP_LR1_FCC2_Func.h"
#include <iostream>
/// Constructor and Destructor
ECC_LRAM_E2_FCC2::ECC_LRAM_E2_FCC2(sc_module_name name, const uint8_t iPE_NUM, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_LRAM_E2_FCC2_AgentController()
        ,PE_NUM(iPE_NUM)
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,sys_clk("sys_clk")
        ,sys_resetz("sys_resetz")
        // MECCCAP LRA (LR1)
        ,errovfout1_LR1("errovfout1_LR1")
        ,errovfout0_LR1("errovfout0_LR1")
        ,dederrout_LR1("dederrout_LR1")
        ,sederrout_LR1("sederrout_LR1")
        // Key open
        ,keyopen("keyopen")
{//{{{ 
    //Check valid of PE_NUM
    if (PE_NUM < 1) {
       re_printf("warning","Invalid PE_NUM setting, PE_NUM is set to default min.\n");
       PE_NUM = 1;  
    }
    else if (PE_NUM > 8 ) {
       re_printf("warning","Invalid PE_NUM setting, PE_NUM is set to default max.\n");
       PE_NUM = 8;  
    }
    // New instance and port
    eccdisable = new sc_out<bool>*[PE_NUM];
    std::string port_name;
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("eccdisable", i);
        eccdisable[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(eccdisable[i] != NULL);
        eccdisable[i]->initialize(false); 
    }
    secdisable = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("secdisable", i);
        secdisable[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(secdisable[i] != NULL);
        secdisable[i]->initialize(false); 
    }
    afedisable = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("afedisable", i);
        afedisable[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(afedisable[i] != NULL);
        afedisable[i]->initialize(false); 
    }
    errovfout1_LR0 = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("errovfout1_LR0", i);
        errovfout1_LR0[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout1_LR0[i] != NULL);
        errovfout1_LR0[i]->initialize(false); 
    }
    errovfout0_LR0 = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("errovfout0_LR0", i);
        errovfout0_LR0[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout0_LR0[i] != NULL);
        errovfout0_LR0[i]->initialize(false); 
    }
    afeerrout_LR0 = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("afeerrout_LR0", i);
        afeerrout_LR0[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(afeerrout_LR0[i] != NULL);
        afeerrout_LR0[i]->initialize(false); 
    }
    dederrout_LR0 = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("dederrout_LR0", i);
        dederrout_LR0[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(dederrout_LR0[i] != NULL);
        dederrout_LR0[i]->initialize(false); 
    }
    sederrout_LR0 = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("sederrout_LR0", i);
        sederrout_LR0[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(sederrout_LR0[i] != NULL);
        sederrout_LR0[i]->initialize(false); 
    }

    // New instance and port
    mpECCCNT_LR0Func = new ECCCNT_LR0_PEn_FCC2_Func*[PE_MAX_NUM];
    mpMECCCAP_LR0Func = new MECCCAP_LR0_PEn_FCC2_Func*[PE_MAX_NUM];
  
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        mpECCCNT_LR0Func[i] = new ECCCNT_LR0_PEn_FCC2_Func((std::string)name, i, this);
        mpMECCCAP_LR0Func[i] = new MECCCAP_LR0_PEn_FCC2_Func((std::string)name, i, this);
    }
    mpMECCCAP_LR1Func = new MECCCAP_LR1_FCC2_Func((std::string)name, this);
    
    BusSlaveBase<32, emNumTgt>::setSlaveResetPort32(&presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn);
    BusSlaveBase<32, emNumTgt>::setSlaveFreqPort32(&pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk);
    BusSlaveBase<32, emNumTgt>::setTargetSocket32((char*)"ts_ecccnt_lr_0", (char*)"ts_ecccnt_lr_1", (char*)"ts_ecccnt_lr_2"
                                                , (char*)"ts_ecccnt_lr_3", (char*)"ts_ecccnt_lr_4", (char*)"ts_ecccnt_lr_5"
                                                , (char*)"ts_ecccnt_lr_6", (char*)"ts_ecccnt_lr_7", (char*)"ts_mecccap_lr0_0"
                                                , (char*)"ts_mecccap_lr0_1", (char*)"ts_mecccap_lr0_2", (char*)"ts_mecccap_lr0_3"
                                                , (char*)"ts_mecccap_lr0_4", (char*)"ts_mecccap_lr0_5", (char*)"ts_mecccap_lr0_6"
                                                , (char*)"ts_mecccap_lr0_7", (char*)"ts_mecccap_lr1");

    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_ecccnt_lr[i] = BusSlaveBase<32, emNumTgt>::tSocket32[i];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setFuncModulePtr(mpECCCNT_LR0Func[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setWriteLatency(wLatency);
    }
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_mecccap_lr0[i] = BusSlaveBase<32, emNumTgt>::tSocket32[PE_MAX_NUM + i];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setFuncModulePtr(mpMECCCAP_LR0Func[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setWriteLatency(wLatency);
    }

    ts_mecccap_lr1 = BusSlaveBase<32, emNumTgt>::tSocket32[PE_MAX_NUM*2];
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2]->setFuncModulePtr(mpMECCCAP_LR1Func);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2]->setReadLatency(rLatency);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2]->setWriteLatency(wLatency);

    // Initialize remain output ports
    errovfout1_LR1.initialize(false);
    errovfout0_LR1.initialize(false);
    dederrout_LR1.initialize(false);
    sederrout_LR1.initialize(false);

    // Initialize variables
    mEnableRegisterMessage = true;
    for (int i = 0; i < emNumClk; i++){
        mClkPeriod[i]       = 0;
        mClkOrgFreq[i]      = 0;
        mClkFreq[i]         = 0;
        mClkFreqUnit[i]     = "Hz";
        mStartClkTime[i]    = 0;
    }
    for (int i = 0; i < emNumReset; i++){
        mResetCurVal[i]         = true;
        mResetPreVal[i]         = true;
        mIsResetHardActive[i]   = false;
        mIsResetCmdReceive[i]   = false;
        mIsResetCmdActive[i]    = false;
        mResetCmdPeriod[i]      = 0;
    }
    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mAWIDTH_MSB_LR0 = DEFAULT_AWIDTH_MSB_LR0;
    mAWIDTH_LSB_LR0 = DEFAULT_AWIDTH_LSB_LR0;
    mCAP_DEPTH_LR0  = DEFAULT_CAP_DEPTH_LR0;
    mERRINT_INIT_LR0= DEFAULT_ERRINT_INIT_LR0;

    mAWIDTH_MSB_LR1 = DEFAULT_AWIDTH_MSB_LR1;
    mAWIDTH_LSB_LR1 = DEFAULT_AWIDTH_LSB_LR1;
    mCAP_DEPTH_LR1  = DEFAULT_CAP_DEPTH_LR1;
    mERRINT_INIT_LR1= DEFAULT_ERRINT_INIT_LR1;

    mDumpInterrupt  = false;
    
    mIsEDLConfig        = false;
    mIsAddrConfig       = false;
    mErrorType          = emNoneError;
    mErrorEDL           = 0;
    mErrorAddress       = 0;
    mCurConfigSubName   = "";
    mPeId               = 0;
    mStrEDL             = "";
    mKeyOpen            = false;

    CreateEDLMap();

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorpclkMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(Monitorsys_clkMethod);
    dont_initialize();
    sensitive << sys_clk;

    SC_METHOD(MonitorpresetnMethod);
    sensitive << presetn;

    SC_METHOD(Monitorsys_resetzMethod);
    sensitive << sys_resetz;

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(MonitorkeyopenMethod);
    dont_initialize();
    sensitive << keyopen;

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteProhibitionEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::WriteProhibitionPortMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteProhibitionPortMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT0_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::WriteERROVFOUT0_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT0_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT1_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::WriteERROVFOUT1_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT1_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT0_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::NegateERROVFOUT0_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT0_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT1_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::NegateERROVFOUT1_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT1_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteSEDERROUT_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::WriteSEDERROUT_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteSEDERROUT_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertSEDERROUT_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::AssertSEDERROUT_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("AssertSEDERROUT_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateSEDERROUT_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::NegateSEDERROUT_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateSEDERROUT_LR0Method"), &opt);
    }
    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateDEDERROUT_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::NegateDEDERROUT_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateDEDERROUT_LR0Method"), &opt);
    }
    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateAFEERROUT_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::NegateAFEERROUT_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateAFEERROUT_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDEDERROUT_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::WriteDEDERROUT_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteDEDERROUT_LR0Method"), &opt);
    }
    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteAFEERROUT_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::WriteAFEERROUT_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteAFEERROUT_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcess1bitError_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::Process1bitError_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("Process1bitError_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessFatalError_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::ProcessFatalError_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("ProcessFatalError_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSetFatalErrorStatus_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::SetFatalErrorStatus_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("SetFatalErrorStatus_LR0Method"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNotifyFatalError_LR0Event[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_LRAM_E2_FCC2::NotifyFatalError_LR0Method, this, pe_id),
                          sc_core::sc_gen_unique_name("NotifyFatalError_LR0Method"), &opt);
    }

    SC_METHOD(WriteERROVFOUT0_LR1Method);
    dont_initialize();
    sensitive << mWriteERROVFOUT0_LR1Event;

    SC_METHOD(WriteERROVFOUT1_LR1Method);
    dont_initialize();
    sensitive << mWriteERROVFOUT1_LR1Event;

    SC_METHOD(NegateERROVFOUT0_LR1Method);
    dont_initialize();
    sensitive << mNegateERROVFOUT0_LR1Event;

    SC_METHOD(NegateERROVFOUT1_LR1Method);
    dont_initialize();
    sensitive << mNegateERROVFOUT1_LR1Event;

    SC_METHOD(WriteSEDERROUT_LR1Method);
    dont_initialize();
    sensitive << mWriteSEDERROUT_LR1Event;

    SC_METHOD(AssertSEDERROUT_LR1Method);
    dont_initialize();
    sensitive << mAssertSEDERROUT_LR1Event;

    SC_METHOD(NegateSEDERROUT_LR1Method);
    dont_initialize();
    sensitive << mNegateSEDERROUT_LR1Event;

    SC_METHOD(Process1bitError_LR1Method);
    dont_initialize();
    sensitive << mProcess1bitError_LR1Event;

    SC_METHOD(ProcessFatalError_LR1Method);
    dont_initialize();
    sensitive << mProcessFatalError_LR1Event;

    SC_METHOD(SetFatalErrorStatus_LR1Method);
    dont_initialize();
    sensitive << mSetFatalErrorStatus_LR1Event;

    SC_METHOD(NotifyFatalError_LR1Method);
    dont_initialize();
    sensitive << mNotifyFatalError_LR1Event;

    SC_METHOD(NegateDEDERROUT_LR1Method);
    dont_initialize();
    sensitive << mNegateDEDERROUT_LR1Event;

    SC_METHOD(WriteDEDERROUT_LR1Method);
    dont_initialize();
    sensitive << mWriteDEDERROUT_LR1Event;

    SC_THREAD(Set1bitErrorStatusThread);
    dont_initialize();
    sensitive << mSet1bitErrorStatusEvent;
    sensitive << mResetHardEvent[emsys_resetzId];
    sensitive << mResetCmdEvent[emsys_resetzId];
    sensitive << mClkZeroEvent[emsys_clkId];
    sensitive << mClkZeroEvent[empclkId];
}//}}} 

/// Destructor
ECC_LRAM_E2_FCC2::~ECC_LRAM_E2_FCC2()
{//{{{


    for (unsigned int i = 0; i < PE_NUM; i++){
        delete eccdisable[i];
        delete secdisable[i];
        delete afedisable[i];
        delete errovfout1_LR0[i];
        delete errovfout0_LR0[i];
        delete afeerrout_LR0[i];
        delete dederrout_LR0[i];
        delete sederrout_LR0[i];
    }
  
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        delete mpECCCNT_LR0Func[i];
        delete mpMECCCAP_LR0Func[i];
    }
    delete mpMECCCAP_LR1Func;
    if(mpECCCNT_LR0Func != NULL) delete [] mpECCCNT_LR0Func;
    if(mpMECCCAP_LR0Func != NULL) delete [] mpMECCCAP_LR0Func;

    if(eccdisable != NULL){
        delete [] eccdisable; 
    }
    if(secdisable != NULL){
        delete [] secdisable; 
    }
    if(afedisable != NULL){
        delete [] afedisable; 
    }
    if(errovfout1_LR0 != NULL){
        delete [] errovfout1_LR0; 
    }
    if(errovfout0_LR0 != NULL){
        delete [] errovfout0_LR0; 
    }
    if(afeerrout_LR0 != NULL){
        delete [] afeerrout_LR0; 
    }
    if(dederrout_LR0 != NULL){
        delete [] dederrout_LR0; 
    }
    if(sederrout_LR0 != NULL){
        delete [] sederrout_LR0; 
    }
}//}}}

/// Create LRAM EDL Map
void ECC_LRAM_E2_FCC2::CreateEDLMap()
{//{{{
    // Create EDL map for LR0
    std::string edl_name;
    for (unsigned int i = 0; i < PE_NUM; i++){
        // LSU, pe_index
        edl_name = CombineStringUint("LSU_PE", i);
        mEDLmap_LR0.insert(std::pair<std::string, unsigned int>(edl_name, 0x0));
        // Bank, pe_index
        for (unsigned int bankid = 0; bankid < 4; bankid++){
            edl_name = CombineStringUint(CombineStringUint("Bank", bankid)+ "_PE", i);
            mEDLmap_LR0.insert(std::pair<std::string, unsigned int>(edl_name, bankid + 1));
        }
    } 
    // Create EDL map for LR1
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        edl_name = CombineStringUint("PE", i);
        mEDLmap_LR1.insert(std::pair<std::string, unsigned int>(edl_name, i));
    }
}//}}}

/// Initialize when reset activated
void ECC_LRAM_E2_FCC2::Initialize(void)
{//{{{
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        unsigned int lreccctl  = mpECCCNT_LR0Func[i]->GetLRECCCTLReg();
        mECCDIS[i] = (bool)(lreccctl & 0x1);// get ECCDIS_CFP
        mSECDIS[i] = (bool)((lreccctl >> 1) & 0x1);// get SECDIS
        mAFEDIS[i] = (bool)((lreccctl >> 3) & 0x1);// get AFEDIS
    }
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        mERROVFOUT0_LR0[i]  = false;
        mERROVFOUT1_LR0[i]  = false;
        mSEDERROUT_LR0[i]   = false;
        mDEDERROUT_LR0[i]   = false;
        mAFEERROUT_LR0[i]   = false;
    }
    mERROVFOUT0_LR1      = false;
    mERROVFOUT1_LR1      = false;
    mSEDERROUT_LR1       = false;
    mDEDERROUT_LR1       = false;
    
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        mHas2bitError_LR0[i]            = false;
        mHasAddrFeedbackError_LR0[i]    = false;
        mFatalErrorModAddr_LR0[i]       = 0;
        mFatalEDLCode_LR0[i]            = 0;
        mSize1bitErrorList_LR0[i]       = 0;
        mSizeFatalErrorList_LR0[i]      = 0;
        while (!m1bitErrorFIFO_LR0[i].empty()){
            m1bitErrorFIFO_LR0[i].pop();
        }
    }
    mHas2bitError_LR1               = false;
    mFatalErrorModAddr_LR1          = 0;
    mFatalEDLCode_LR1               = 0;
    mSize1bitErrorList_LR1          = 0;
    mSizeFatalErrorList_LR1         = 0;
    while (!m1bitErrorFIFO_LR1.empty()){
        m1bitErrorFIFO_LR1.pop();
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Prepare to write ECCDIS_CFP
void ECC_LRAM_E2_FCC2::WriteProhibitionSignal(unsigned int pe_id, unsigned int value)
{//{{{
    mECCDIS[pe_id] = (bool)(value & 0x1);
    mSECDIS[pe_id] = (bool)((value >> 1) & 0x1);
    mAFEDIS[pe_id] = (bool)((value >> 3) & 0x1);
    mWriteProhibitionEvent[pe_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteProhibitionEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get CAP DEPTH
unsigned int ECC_LRAM_E2_FCC2::GetCapDepth(std::string sub_name)
{//{{{
    sc_assert((sub_name == "LR0") || (sub_name == "LR1"));
    unsigned int ret = 0;

    if (sub_name == "LR0"){
        ret = mCAP_DEPTH_LR0;
    }else{// LR1
        ret = mCAP_DEPTH_LR1;
    }
    return ret;
}//}}}

/// Assert errovfout0_LR0 port
void ECC_LRAM_E2_FCC2::AssertERROVFOUT0_LR0(unsigned int pe_id)
{//{{{
    mERROVFOUT0_LR0[pe_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_LR0Event[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_LR0 port
void ECC_LRAM_E2_FCC2::AssertERROVFOUT1_LR0(unsigned int pe_id)
{//{{{
    mERROVFOUT1_LR0[pe_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_LR0Event[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout0_LR1 port
void ECC_LRAM_E2_FCC2::AssertERROVFOUT0_LR1()
{//{{{
    mERROVFOUT0_LR1 = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_LR1Event.notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_LR1 port
void ECC_LRAM_E2_FCC2::AssertERROVFOUT1_LR1()
{//{{{
    mERROVFOUT1_LR1 = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_LR1Event.notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get keycode value
bool ECC_LRAM_E2_FCC2::GetKeyOpen()
{//{{{
    return mKeyOpen;
}//}}}

/// Get AWIDTH_MSB
unsigned int ECC_LRAM_E2_FCC2::GetAWIDTH_MSB(std::string sub_name)
{//{{{
    sc_assert((sub_name == "LR0") || (sub_name == "LR1"));
    unsigned int ret = 0;

    if (sub_name == "LR0"){
        ret = mAWIDTH_MSB_LR0;
    }else{// LR1
        ret = mAWIDTH_MSB_LR1;
    }
    return ret;
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock
void ECC_LRAM_E2_FCC2::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor sys_clk clock
void ECC_LRAM_E2_FCC2::Monitorsys_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

/// Monitor presetn port
void ECC_LRAM_E2_FCC2::MonitorpresetnMethod()
{//{{{
    mResetCurVal[empresetnId] = presetn.read();
    if (mResetCurVal[empresetnId] != mResetPreVal[empresetnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[empresetnId] = mResetCurVal[empresetnId];
        // Reset signals are asynchronous
        mResetHardEvent[empresetnId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor presetn port
void ECC_LRAM_E2_FCC2::Monitorsys_resetzMethod()
{//{{{
    mResetCurVal[emsys_resetzId] = sys_resetz.read();
    if (mResetCurVal[emsys_resetzId] != mResetPreVal[emsys_resetzId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emsys_resetzId] = mResetCurVal[emsys_resetzId];
        // Reset signals are asynchronous
        mResetHardEvent[emsys_resetzId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Process reset function when reset port is active
void ECC_LRAM_E2_FCC2::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == empresetnId) {
            re_printf("info","The reset port presetn is asserted.\n");
            this->EnableReset("presetn", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port sys_resetz is asserted.\n");
            this->EnableReset("sys_resetz", mIsResetHardActive[reset_id]);
        }
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == empresetnId) {
                re_printf("info","The reset port presetn is de-asserted.\n");
                this->EnableReset("presetn", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port sys_resetz is de-asserted.\n");
                this->EnableReset("sys_resetz", mIsResetHardActive[reset_id]);
            }
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

/// Process reset function when reset command is active
void ECC_LRAM_E2_FCC2::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetn";
    if (reset_id == empresetnId) {
        reset_name = "presetn";
    } else { //reset_id == emsys_resetzId
        reset_name = "sys_resetz";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
void ECC_LRAM_E2_FCC2::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetn";
    if (reset_id == empresetnId) {
        reset_name = "presetn";
    } else { //reset_id == emsys_resetzId
        reset_name = "sys_resetz";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

/// Reset model and its sub-instance if reset is actived
void ECC_LRAM_E2_FCC2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    // Write ERRINT_INIT to ERRINT reg after reset
    if (reset_name=="sys_resetz" && is_active){
        for (unsigned int i = 0; i < PE_MAX_NUM; i++){
            mpMECCCAP_LR0Func[i]->WriteERRINT_INIT(mERRINT_INIT_LR0);
        }
        mpMECCCAP_LR1Func->WriteERRINT_INIT(mERRINT_INIT_LR1);
    }
    for (unsigned int i = 0; i < PE_MAX_NUM; i++) {
        (mpECCCNT_LR0Func[i])->EnableReset(reset_name, is_active);
        (mpMECCCAP_LR0Func[i])->EnableReset(reset_name, is_active);
    }
    mpMECCCAP_LR1Func->EnableReset(reset_name, is_active);
    if(reset_name=="sys_resetz" && is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        for (unsigned int i = 0; i < PE_MAX_NUM; i++){
            mWriteProhibitionEvent[i].notify();
            mWriteERROVFOUT0_LR0Event[i].notify();
            mWriteERROVFOUT1_LR0Event[i].notify();
            mWriteSEDERROUT_LR0Event[i].notify();
            mWriteDEDERROUT_LR0Event[i].notify();
            mWriteAFEERROUT_LR0Event[i].notify();
        }
        mWriteERROVFOUT0_LR1Event.notify();
        mWriteERROVFOUT1_LR1Event.notify();
        mWriteSEDERROUT_LR1Event.notify();
        mWriteDEDERROUT_LR1Event.notify();
    }
}//}}}

/// Cancel events
void ECC_LRAM_E2_FCC2::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        mWriteProhibitionEvent[i].cancel();
        mWriteERROVFOUT0_LR0Event[i].cancel();
        mWriteERROVFOUT1_LR0Event[i].cancel();
        mNegateERROVFOUT0_LR0Event[i].cancel();
        mNegateERROVFOUT1_LR0Event[i].cancel();
        mWriteSEDERROUT_LR0Event[i].cancel();
        mAssertSEDERROUT_LR0Event[i].cancel();
        mNegateSEDERROUT_LR0Event[i].cancel();
        mNegateDEDERROUT_LR0Event[i].cancel();
        mWriteDEDERROUT_LR0Event[i].cancel();
        mProcess1bitError_LR0Event[i].cancel();
        mProcessFatalError_LR0Event[i].cancel();
        mSetFatalErrorStatus_LR0Event[i].cancel();
        mNotifyFatalError_LR0Event[i].cancel();
        mNegateAFEERROUT_LR0Event[i].cancel();
        mWriteAFEERROUT_LR0Event[i].cancel();
    }
    mWriteERROVFOUT0_LR1Event.cancel();
    mWriteERROVFOUT1_LR1Event.cancel();
    mNegateERROVFOUT0_LR1Event.cancel();
    mNegateERROVFOUT1_LR1Event.cancel();
    mWriteSEDERROUT_LR1Event.cancel();
    mAssertSEDERROUT_LR1Event.cancel();
    mNegateSEDERROUT_LR1Event.cancel();
    mNegateDEDERROUT_LR1Event.cancel();
    mWriteDEDERROUT_LR1Event.cancel();
    mProcess1bitError_LR1Event.cancel();
    mProcessFatalError_LR1Event.cancel();
    mSetFatalErrorStatus_LR1Event.cancel();
    mNotifyFatalError_LR1Event.cancel();
    mSet1bitErrorStatusEvent.cancel();
}//}}}

/// Get reset status
bool ECC_LRAM_E2_FCC2::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if(reset_name=="presetn") {
        if (mIsResetHardActive[empresetnId] || mIsResetCmdActive[empresetnId]) {
            reset_status = true;
        }
    }else{
        if (mIsResetHardActive[emsys_resetzId] || mIsResetCmdActive[emsys_resetzId]) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

/// Check clock period 
bool ECC_LRAM_E2_FCC2::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == "pclk") {
        clock_period = mClkPeriod[empclkId];
    } else {
        clock_period = mClkPeriod[emsys_clkId];
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Monitor keyopen port
void ECC_LRAM_E2_FCC2::MonitorkeyopenMethod()
{//{{{
    mKeyOpen = keyopen.read();
}//}}}

/// Write eccdisable_cfp port
void ECC_LRAM_E2_FCC2::WriteProhibitionPortMethod(unsigned int pe_id)
{//{{{
    eccdisable[pe_id]->write(mECCDIS[pe_id]);
    secdisable[pe_id]->write(mSECDIS[pe_id]);
    afedisable[pe_id]->write(mAFEDIS[pe_id]);
}//}}}

/// Write errovfout0_LR0 port
void ECC_LRAM_E2_FCC2::WriteERROVFOUT0_LR0Method(unsigned int pe_id)
{//{{{
    errovfout0_LR0[pe_id]->write(mERROVFOUT0_LR0[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_LR0[%d] is changed to %d\n", pe_id, (unsigned int)mERROVFOUT0_LR0[pe_id]);
    }
    // Negate if any
    if (mERROVFOUT0_LR0[pe_id]){
        mNegateERROVFOUT0_LR0Event[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_LR0 port
void ECC_LRAM_E2_FCC2::WriteERROVFOUT1_LR0Method(unsigned int pe_id)
{//{{{
    errovfout1_LR0[pe_id]->write(mERROVFOUT1_LR0[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_LR0[%d] is changed to %d\n", pe_id, (unsigned int)mERROVFOUT1_LR0[pe_id]);
    }
    // Negate if any
    if (mERROVFOUT1_LR0[pe_id]){
        mNegateERROVFOUT1_LR0Event[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout0_LR0 port
void ECC_LRAM_E2_FCC2::NegateERROVFOUT0_LR0Method(unsigned int pe_id)
{//{{{
    mERROVFOUT0_LR0[pe_id] = false;
    mWriteERROVFOUT0_LR0Event[pe_id].notify();
}//}}}

/// Negate errovfout1_LR0 port
void ECC_LRAM_E2_FCC2::NegateERROVFOUT1_LR0Method(unsigned int pe_id)
{//{{{
    mERROVFOUT1_LR0[pe_id] = false;
    mWriteERROVFOUT1_LR0Event[pe_id].notify();
}//}}}

/// Write sederrout_LR0 port
void ECC_LRAM_E2_FCC2::WriteSEDERROUT_LR0Method(unsigned int pe_id)
{//{{{
    sederrout_LR0[pe_id]->write(mSEDERROUT_LR0[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_LR0[%d] is changed to %d\n", pe_id, (unsigned int)mSEDERROUT_LR0[pe_id]);
    }
    // Negate if any
    if (mSEDERROUT_LR0[pe_id]){
        mNegateSEDERROUT_LR0Event[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)
void ECC_LRAM_E2_FCC2::AssertSEDERROUT_LR0Method(unsigned int pe_id)
{//{{{
    unsigned int sedie = mpMECCCAP_LR0Func[pe_id]->GetERRINT() & 0x1;
    if(sedie == 1){
        mSEDERROUT_LR0[pe_id] = true;
        mWriteSEDERROUT_LR0Event[pe_id].notify();
    }
}//}}}

/// Negate sederrout_LR0 port
void ECC_LRAM_E2_FCC2::NegateSEDERROUT_LR0Method(unsigned int pe_id)
{//{{{
    mSEDERROUT_LR0[pe_id] = false;
    mWriteSEDERROUT_LR0Event[pe_id].notify();
}//}}}

/// Negate dederrout_LR0 port
void ECC_LRAM_E2_FCC2::NegateDEDERROUT_LR0Method(unsigned int pe_id)
{//{{{
    mDEDERROUT_LR0[pe_id] = false;
    mWriteDEDERROUT_LR0Event[pe_id].notify();
}//}}}

/// Negate afeerrout_LR0 port

void ECC_LRAM_E2_FCC2::NegateAFEERROUT_LR0Method(unsigned int pe_id)
{//{{{
    mAFEERROUT_LR0[pe_id] = false;
    mWriteAFEERROUT_LR0Event[pe_id].notify();
}//}}}

/// Write dederrout_LR0 port
void ECC_LRAM_E2_FCC2::WriteDEDERROUT_LR0Method(unsigned int pe_id)
{//{{{
    dederrout_LR0[pe_id]->write(mDEDERROUT_LR0[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_LR0[%d] is changed to %d\n", pe_id, (unsigned int)mDEDERROUT_LR0[pe_id]);
    }
    if (mDEDERROUT_LR0[pe_id]){
        mNegateDEDERROUT_LR0Event[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write afeerrout_LR0 port
void ECC_LRAM_E2_FCC2::WriteAFEERROUT_LR0Method(unsigned int pe_id)
{//{{{
    afeerrout_LR0[pe_id]->write(mAFEERROUT_LR0[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "afeerrout_LR0[%d] is changed to %d\n", pe_id, (unsigned int)mAFEERROUT_LR0[pe_id]);
    }
    if (mAFEERROUT_LR0[pe_id]){
        mNegateAFEERROUT_LR0Event[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit for LR0
void ECC_LRAM_E2_FCC2::Process1bitError_LR0Method(unsigned int pe_id)
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_LR0; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList_LR0[pe_id]; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList_LR0[pe_id][i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO_LR0[pe_id].size() < emTotalSlavePorts_LR0) {// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO_LR0[pe_id].push(m1bitErrorList_LR0[pe_id][i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_LR0Func[pe_id]->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList_LR0[pe_id]; i++){
        unsigned int mod_address = ShiftAddress("LR0", m1bitErrorList_LR0[pe_id][i].address);
        if ((!CheckMaskAddress_LR0(pe_id, m1bitErrorList_LR0[pe_id][i].address)) && Check1bitAddressNotCaptured_LR0(pe_id, mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_LR0Event[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList_LR0[pe_id] = 0;
}//}}}

/// Write errovfout0_LR1 port
void ECC_LRAM_E2_FCC2::WriteERROVFOUT0_LR1Method()
{//{{{
    errovfout0_LR1.write(mERROVFOUT0_LR1);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_LR1 is changed to %d\n", (unsigned int)mERROVFOUT0_LR1);
    }
    // Negate if any
    if (mERROVFOUT0_LR1){
        mNegateERROVFOUT0_LR1Event.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_LR1 port
void ECC_LRAM_E2_FCC2::WriteERROVFOUT1_LR1Method()
{//{{{
    errovfout1_LR1.write(mERROVFOUT1_LR1);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_LR1 is changed to %d\n", (unsigned int)mERROVFOUT1_LR1);
    }
    // Negate if any
    if (mERROVFOUT1_LR1){
        mNegateERROVFOUT1_LR1Event.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout0_LR1 port
void ECC_LRAM_E2_FCC2::NegateERROVFOUT0_LR1Method()
{//{{{
    mERROVFOUT0_LR1 = false;
    mWriteERROVFOUT0_LR1Event.notify();
}//}}}

/// Negate errovfout1_LR1 port
void ECC_LRAM_E2_FCC2::NegateERROVFOUT1_LR1Method()
{//{{{
    mERROVFOUT1_LR1 = false;
    mWriteERROVFOUT1_LR1Event.notify();
}//}}}

/// Write sederrout_LR1 port
void ECC_LRAM_E2_FCC2::WriteSEDERROUT_LR1Method()
{//{{{
    sederrout_LR1.write(mSEDERROUT_LR1);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_LR1 is changed to %d\n", (unsigned int)mSEDERROUT_LR1);
    }
    // Negate if any
    if (mSEDERROUT_LR1){
        mNegateSEDERROUT_LR1Event.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)
void ECC_LRAM_E2_FCC2::AssertSEDERROUT_LR1Method()
{//{{{
    unsigned int sedie = mpMECCCAP_LR1Func->GetERRINT() & 0x1;
    if(sedie == 1){
        mSEDERROUT_LR1 = true;
        mWriteSEDERROUT_LR1Event.notify();
    }
}//}}}

/// Negate sederrout_LR1 port
void ECC_LRAM_E2_FCC2::NegateSEDERROUT_LR1Method()
{//{{{
    mSEDERROUT_LR1 = false;
    mWriteSEDERROUT_LR1Event.notify();
}//}}}

/// Negate dederrout_LR1 port
void ECC_LRAM_E2_FCC2::NegateDEDERROUT_LR1Method()
{//{{{
    mDEDERROUT_LR1 = false;
    mWriteDEDERROUT_LR1Event.notify();
}//}}}

/// Write dederrout_LR1 port
void ECC_LRAM_E2_FCC2::WriteDEDERROUT_LR1Method()
{//{{{
    dederrout_LR1.write(mDEDERROUT_LR1);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_LR1 is changed to %d\n", (unsigned int)mDEDERROUT_LR1);
    }
    if (mDEDERROUT_LR1){
        mNegateDEDERROUT_LR1Event.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Process 1bit error for both MEMCAP 
void ECC_LRAM_E2_FCC2::Set1bitErrorStatusThread()
{//{{{
    while (true){
        if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
            wait();
            continue;
        }
        try{
            while (true){
                while (CheckAnyFIFOHasElement()){// there is any element in any FIFO
                    // Wait 1 cycle (before process all FIFO parallel)
                    wait((double)mClkPeriod[emsys_clkId], mTimeResolutionUnit,
                            mResetHardEvent[emsys_resetzId] | mResetCmdEvent[emsys_resetzId] | mClkZeroEvent[emsys_clkId] | mClkZeroEvent[empclkId]);
                    if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
                        wait();
                        continue;
                    }

                    // Process FIFOs of LR0
                    for (unsigned int pe_id = 0; pe_id < PE_MAX_NUM; pe_id++){
                        if (m1bitErrorFIFO_LR0[pe_id].size() != 0){
                            unsigned int mod_address = ShiftAddress("LR0", m1bitErrorFIFO_LR0[pe_id].front().address);
                            if (mpMECCCAP_LR0Func[pe_id]->CheckHasRoomFor1bit()){
                                // then set status bit if there is room
                                unsigned int edl_code = m1bitErrorFIFO_LR0[pe_id].front().edl_code;
                                if (Check1bitAddressNotCaptured_LR0(pe_id, mod_address)){
                                    mpMECCCAP_LR0Func[pe_id]->Set1bitStatusAndAddress(edl_code, mod_address);
                                }
                            } else {
                                //ECC 1bit error was detacted when all flag are set.
                                if (Check1bitAddressNotCaptured_LR0(pe_id, mod_address)){
                                    mpMECCCAP_LR0Func[pe_id]->SetFlagAddressBufferOverflow();
                            while (!m1bitErrorFIFO_LR0[pe_id].empty()){
                                m1bitErrorFIFO_LR0[pe_id].pop();
                            }
                                }
                            }
                            //remove FIFO
                            if (!m1bitErrorFIFO_LR0[pe_id].empty()){
                                m1bitErrorFIFO_LR0[pe_id].pop();
                            }
                        }
                    }

                    // Process FIFO of LR1
                    if (m1bitErrorFIFO_LR1.size() != 0){
                        unsigned int mod_address = ShiftAddress("LR1", m1bitErrorFIFO_LR1.front().address);
                        if (mpMECCCAP_LR1Func->CheckHasRoomFor1bit()){
                            // then set status bit if there is room
                            unsigned int edl_code = m1bitErrorFIFO_LR1.front().edl_code;
                            if (Check1bitAddressNotCaptured_LR1(mod_address)){
                                mpMECCCAP_LR1Func->Set1bitStatusAndAddress(edl_code, mod_address);
                            }
                        } else {
                            //ECC 1bit error was detacted when all flag are set.
                            if (Check1bitAddressNotCaptured_LR1(mod_address)){
                                mpMECCCAP_LR1Func->SetFlagAddressBufferOverflow();
                            while (!m1bitErrorFIFO_LR1.empty()){
                                m1bitErrorFIFO_LR1.pop();
                            }
                            }
                        }
                        //remove FIFO
                        if (!m1bitErrorFIFO_LR1.empty()){
                            m1bitErrorFIFO_LR1.pop();
                        }
                    }
                }
                wait();
                if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException){
        }
    }
}//}}}

/// Check there is any element in any FIFO
bool ECC_LRAM_E2_FCC2::CheckAnyFIFOHasElement()
{//{{{
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        if (m1bitErrorFIFO_LR0[i].size() != 0){
            return true;
        }
    }
    if (m1bitErrorFIFO_LR1.size() != 0){
        return true;
    }
    return false;
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit for LR1
void ECC_LRAM_E2_FCC2::Process1bitError_LR1Method()
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_LR1; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList_LR1; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList_LR1[i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO_LR1.size() < PE_NUM) {//emTotalSlavePorts_LR1){// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO_LR1.push(m1bitErrorList_LR1[i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_LR1Func->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList_LR1; i++){
        unsigned int mod_address = ShiftAddress("LR1", m1bitErrorList_LR1[i].address);
        if ((!CheckMaskAddress_LR1(m1bitErrorList_LR1[i].address)) && Check1bitAddressNotCaptured_LR1(mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_LR1Event.notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList_LR1 = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit for LR0
void ECC_LRAM_E2_FCC2::ProcessFatalError_LR0Method(unsigned int pe_id)
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError_LR0[pe_id] = false;
    mHasAddrFeedbackError_LR0[pe_id] = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_LR0; portindex++){// With fix priority
        if (is_update_status){
            break;
        }
        for (unsigned int i = 0; i < mSizeFatalErrorList_LR0[pe_id]; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList_LR0[pe_id][i].edl_code == portindex){
                if (CheckNoFatalStatus_LR0(pe_id)){
                    mHas2bitError_LR0[pe_id] |= (mFatalErrorList_LR0[pe_id][i].type == em2bitError);
                    mHasAddrFeedbackError_LR0[pe_id] |= (mFatalErrorList_LR0[pe_id][i].type == emAddressFeedbackError);
                    mod_address = ShiftAddress("LR0", mFatalErrorList_LR0[pe_id][i].address);
                    edl_code = mFatalErrorList_LR0[pe_id][i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalErrorModAddr_LR0[pe_id] = mod_address;
        mFatalEDLCode_LR0[pe_id] = edl_code;
        mSetFatalErrorStatus_LR0Event[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList_LR0[pe_id]; i++){
        if (!CheckMaskAddress_LR0(pe_id, mFatalErrorList_LR0[pe_id][i].address)){
            if (mFatalErrorList_LR0[pe_id][i].type == em2bitError){
                mDEDERROUT_LR0[pe_id] |= true;
            }
            if (mFatalErrorList_LR0[pe_id][i].type == emAddressFeedbackError){
                mAFEERROUT_LR0[pe_id] |= true;
            }
        }
    }
    if (mDEDERROUT_LR0[pe_id] || mAFEERROUT_LR0[pe_id]){
            mNotifyFatalError_LR0Event[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }
    // Clear mFatalErrorList
    mSizeFatalErrorList_LR0[pe_id] = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit for LR1
void ECC_LRAM_E2_FCC2::ProcessFatalError_LR1Method()
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError_LR1 = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_LR1; portindex++){// With fix priority
        if (is_update_status){
            break;
        }
        for (unsigned int i = 0; i < mSizeFatalErrorList_LR1; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList_LR1[i].edl_code == portindex){
                if (CheckNoFatalStatus_LR1()){
                    mHas2bitError_LR1 |= (mFatalErrorList_LR1[i].type == em2bitError);
                    mod_address = ShiftAddress("LR1", mFatalErrorList_LR1[i].address);
                    edl_code = mFatalErrorList_LR1[i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalErrorModAddr_LR1 = mod_address;
        mFatalEDLCode_LR1 = edl_code;
        mSetFatalErrorStatus_LR1Event.notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList_LR1; i++){
        if (!CheckMaskAddress_LR1(mFatalErrorList_LR1[i].address)){
            if (mFatalErrorList_LR1[i].type == em2bitError){
                mDEDERROUT_LR1 |= true;
            }
        }
    }
    if (mDEDERROUT_LR1){
            mNotifyFatalError_LR1Event.notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList_LR1 = 0;
}//}}}

/// Set fatal status bit for LR0
void ECC_LRAM_E2_FCC2::SetFatalErrorStatus_LR0Method(unsigned int pe_id)
{//{{{
    if (mHas2bitError_LR0[pe_id]){
        mpMECCCAP_LR0Func[pe_id]->Set2bitErrorStatus();
        mpMECCCAP_LR0Func[pe_id]->SetFatalAddress(mFatalEDLCode_LR0[pe_id], mFatalErrorModAddr_LR0[pe_id]);
    }
    if (mHasAddrFeedbackError_LR0[pe_id]){
        mpMECCCAP_LR0Func[pe_id]->SetAddrFeedbackErrorStatus();
        mpMECCCAP_LR0Func[pe_id]->SetFatalAddress(mFatalEDLCode_LR0[pe_id], mFatalErrorModAddr_LR0[pe_id]);
    }
}//}}}

/// Set fatal status bit for LR1

void ECC_LRAM_E2_FCC2::SetFatalErrorStatus_LR1Method()
{//{{{
    if (mHas2bitError_LR1){
        mpMECCCAP_LR1Func->Set2bitErrorStatus();
        mpMECCCAP_LR1Func->SetFatalAddress(mFatalEDLCode_LR1, mFatalErrorModAddr_LR1);
    }
}//}}}

/// Notify fatal error for LR0
void ECC_LRAM_E2_FCC2::NotifyFatalError_LR0Method(unsigned int pe_id)
{//{{{
    unsigned int errint = mpMECCCAP_LR0Func[pe_id]->GetERRINT();
    unsigned int dedie = (errint >> 1) & 0x1;
    unsigned int afeie = (errint >> 3) & 0x1;
    if (mDEDERROUT_LR0[pe_id] && (dedie == 1)){
        mWriteDEDERROUT_LR0Event[pe_id].notify();
    }else{
        mDEDERROUT_LR0[pe_id] = false;
    }
    if (mAFEERROUT_LR0[pe_id] && (afeie == 1)){
        mWriteAFEERROUT_LR0Event[pe_id].notify();
    }else{
        mAFEERROUT_LR0[pe_id] = false;
    }
}//}}}

/// Notify fatal error for LR1
void ECC_LRAM_E2_FCC2::NotifyFatalError_LR1Method()
{//{{{
    unsigned int errint = mpMECCCAP_LR1Func->GetERRINT();
    unsigned int dedie = (errint >> 1) & 0x1;
    if (mDEDERROUT_LR1 && (dedie == 1)){
        mWriteDEDERROUT_LR1Event.notify();
    }else{
        mDEDERROUT_LR1 = false;
    }
}//}}}

/// Check no fatal status of LR0
bool ECC_LRAM_E2_FCC2::CheckNoFatalStatus_LR0(unsigned int pe_id)
{//{{{
    return mpMECCCAP_LR0Func[pe_id]->CheckNoFatalStatus();
}//}}}

/// Check no fatal status of LR1
bool ECC_LRAM_E2_FCC2::CheckNoFatalStatus_LR1()
{//{{{
    return mpMECCCAP_LR1Func->CheckNoFatalStatus();
}//}}}

/// Check an address is masked or not for LR0
bool ECC_LRAM_E2_FCC2::CheckMaskAddress_LR0(const unsigned int pe_id, const unsigned int address)
{//{{{
    for (unsigned int i = 0; i < emNumMaskedArea; i++){
        unsigned int masked_enable = mpMECCCAP_LR0Func[pe_id]->GetERM_EN(i);
        if (masked_enable != 0){
            unsigned int ermsa = mpMECCCAP_LR0Func[pe_id]->GetERMSA(i);
            unsigned int ermea = mpMECCCAP_LR0Func[pe_id]->GetERMEA(i);
            if (ermea >= ermsa){// Ignore if end address < start address
                unsigned int start_mask_address = ermsa << 12;
                unsigned int end_mask_address = (ermea << 12) | 0xFFF;
                if ((start_mask_address <= address) && (address <= end_mask_address)){
                    return true;
                }
            }
        }
    }
    return false;
}//}}}

/// Check an address is masked or not for LR1
bool ECC_LRAM_E2_FCC2::CheckMaskAddress_LR1(const unsigned int address)
{//{{{
    for (unsigned int i = 0; i < emNumMaskedArea; i++){
        unsigned int masked_enable = mpMECCCAP_LR1Func->GetERM_EN(i);
        if (masked_enable != 0){
            unsigned int ermsa = mpMECCCAP_LR1Func->GetERMSA(i);
            unsigned int ermea = mpMECCCAP_LR1Func->GetERMEA(i);
            if (ermea >= ermsa){// Ignore if end address < start address
                unsigned int start_mask_address = ermsa << 12;
                unsigned int end_mask_address = (ermea << 12) | 0xFFF;
                if ((start_mask_address <= address) && (address <= end_mask_address)){
                    return true;
                }
            }
        }
    }
    return false;
}//}}}

/// Check this address is captured in 1bit address register buffer for LR0
bool ECC_LRAM_E2_FCC2::Check1bitAddressNotCaptured_LR0(unsigned int pe_id, unsigned int mod_address)
{//{{{
    for (unsigned int i = 0; i < mCAP_DEPTH_LR0; i++){
        unsigned int cap_addr = mpMECCCAP_LR0Func[pe_id]->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_LR0Func[pe_id]->GetLR0_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Check this address is captured in 1bit address register buffer for LR1
bool ECC_LRAM_E2_FCC2::Check1bitAddressNotCaptured_LR1(unsigned int mod_address)
{//{{{
    for (unsigned int i = 0; i < mCAP_DEPTH_LR1; i++){
        unsigned int cap_addr = mpMECCCAP_LR1Func->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_LR1Func->GetLR1_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Update 1bit error list for LR0
void ECC_LRAM_E2_FCC2::Update1bitErrorList_LR0(unsigned int pe_id)
{//{{{
    bool is_same_port = false;

    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList_LR0[pe_id]; i++){
        if (mErrorEDL == m1bitErrorList_LR0[pe_id][i].edl_code){
            m1bitErrorList_LR0[pe_id][i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList_LR0[pe_id][mSize1bitErrorList_LR0[pe_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList_LR0[pe_id]++;
    }
}//}}}

/// Update 1bit error list for LR1
void ECC_LRAM_E2_FCC2::Update1bitErrorList_LR1()
{//{{{
    bool is_same_port = false;

    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList_LR1; i++){
        if (mErrorEDL == m1bitErrorList_LR1[i].edl_code){
            m1bitErrorList_LR1[i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList_LR1[mSize1bitErrorList_LR1] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList_LR1++;
    }
}//}}}

/// Update fatal error list for LR0
void ECC_LRAM_E2_FCC2::UpdateFatalErrorList_LR0(unsigned int pe_id)
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if havingpe_id change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList_LR0[pe_id]; i++){
        if (mErrorEDL == mFatalErrorList_LR0[pe_id][i].edl_code){
            mFatalErrorList_LR0[pe_id][i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList_LR0[pe_id][i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList_LR0[pe_id][mSizeFatalErrorList_LR0[pe_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList_LR0[pe_id]++;
    }
}//}}}

/// Update fatal error list for LR1
void ECC_LRAM_E2_FCC2::UpdateFatalErrorList_LR1()
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if havingpe_id change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList_LR1; i++){
        if (mErrorEDL == mFatalErrorList_LR1[i].edl_code){
            mFatalErrorList_LR1[i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList_LR1[i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList_LR1[mSizeFatalErrorList_LR1] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList_LR1++;
    }
}//}}}

/// Combine string and unsigned int
std::string ECC_LRAM_E2_FCC2::CombineStringUint(std::string name, unsigned int index)
{//{{{
    std::ostringstream ret_string;
    ret_string.str("");
    ret_string << name << index;
    return ret_string.str();
}//}}}

/// Shift address to corresponding with MSB/LSB
unsigned int ECC_LRAM_E2_FCC2::ShiftAddress(std::string sub_name, unsigned int address)
{//{{{
    unsigned int mod_addr = 0;
    if (sub_name == "LR0"){
        mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB_LR0 + 1)) - 1))>>mAWIDTH_LSB_LR0) << (mAWIDTH_LSB_LR0 - MIN_AWIDTH_LSB);
    }else{ // LR1
        mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB_LR1 + 1)) - 1))>>mAWIDTH_LSB_LR1) << (mAWIDTH_LSB_LR1 - MIN_AWIDTH_LSB);
    }
    return mod_addr;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void ECC_LRAM_E2_FCC2::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    //Set message level for Register IF class
    if (true == mEnableRegisterMessage) {
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_LR0Func[i]->RegisterHandler(cmd);
            mpMECCCAP_LR0Func[i]->RegisterHandler(cmd);
        }
        mpMECCCAP_LR1Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_LR0Func[i]->RegisterHandler(cmd);
            mpMECCCAP_LR0Func[i]->RegisterHandler(cmd);
        }
        mpMECCCAP_LR1Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_LR0Func[i]->RegisterHandler(cmd);
            mpMECCCAP_LR0Func[i]->RegisterHandler(cmd);
        }
        mpMECCCAP_LR1Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_LR0Func[i]->RegisterHandler(cmd);
            mpMECCCAP_LR0Func[i]->RegisterHandler(cmd);
        }
        mpMECCCAP_LR1Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void ECC_LRAM_E2_FCC2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_LR0Func[i]->RegisterHandler(cmd);
        mpMECCCAP_LR0Func[i]->RegisterHandler(cmd);
    }
    mpMECCCAP_LR1Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void ECC_LRAM_E2_FCC2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_LRAM_E2_FCC2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_LRAM_E2_FCC2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_LRAM_E2_FCC2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Set AWIDTH_MSB

void ECC_LRAM_E2_FCC2::AWIDTH_MSB(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "LR0"){
        if ((mAWIDTH_LSB_LR0 <= value) && (value <= MAX_ADDWIDTH_MSB)){
            mAWIDTH_MSB_LR0 = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_LRAM_E2_FCC2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB_LR0, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB_LR0);
        }
    }else if (sub_name == "LR1"){
        if ((mAWIDTH_LSB_LR1 <= value) && (value <= MAX_ADDWIDTH_MSB)){
            mAWIDTH_MSB_LR1 = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_LRAM_E2_FCC2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB_LR1, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB_LR1);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_LRAM_E2_FCC2_AWIDTH_MSB parameter. Valid name of sub-block is LR0 or LR1.\n", sub_name.c_str());
    }
}//}}}

/// Set AWIDTH_LSB
void ECC_LRAM_E2_FCC2::AWIDTH_LSB(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "LR0"){
        if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB_LR0)){
            mAWIDTH_LSB_LR0 = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_LRAM_E2_FCC2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB_LR0, DEFAULT_AWIDTH_LSB_LR0);
        }
    }else if (sub_name == "LR1"){
        if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB_LR1)){
            mAWIDTH_LSB_LR1 = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_LRAM_E2_FCC2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB_LR1, DEFAULT_AWIDTH_LSB_LR1);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_LRAM_E2_FCC2_AWIDTH_LSB parameter. Valid name of sub-block is LR0 or LR1.\n", sub_name.c_str());
    }
}//}}}

/// Set CAP_DEPTH
void ECC_LRAM_E2_FCC2::CAP_DEPTH(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "LR0"){
        if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
            mCAP_DEPTH_LR0 = value;
        }else{
            re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_LRAM_E2_FCC2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH_LR0);
        }
    }else if (sub_name == "LR1"){
        if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
            mCAP_DEPTH_LR1 = value;
        }else{
            re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_LRAM_E2_FCC2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH_LR1);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_LRAM_E2_FCC2_CAP_DEPTH parameter. Valid name of sub-block is LR0 or LR1.\n", sub_name.c_str());
    }
}//}}}

/// Set ERRINT_INIT
void ECC_LRAM_E2_FCC2::ERRINT_INIT(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "LR0"){
        if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
            mERRINT_INIT_LR0 = value;
        }else{
            re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_LRAM_E2_FCC2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT_LR0);
        }
    }else if (sub_name == "LR1"){
        if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
            mERRINT_INIT_LR1 = value;
        }else{
            re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_LRAM_E2_FCC2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT_LR1);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_LRAM_E2_FCC2_ERRINT_INIT parameter. Valid name of sub-block is LR0 or LR1.\n", sub_name.c_str());
    }
}//}}}

/// Calculate EDL code
std::string ECC_LRAM_E2_FCC2::CalculateEDLCode()
{//{{{
    std::string edl_name = "";
    if (mCurConfigSubName == "LR0"){
        if (mStrEDL == "Bank"){
            unsigned int bankid = (mErrorAddress >> 2) & 0x3;
            edl_name = CombineStringUint(CombineStringUint(mStrEDL, bankid) + "_PE", mPeId);
        }else{// LSU
            edl_name = CombineStringUint(mStrEDL + "_PE", mPeId);
        }
        std::map<std::string, unsigned int>::iterator it;
        for (it = mEDLmap_LR0.begin(); it != mEDLmap_LR0.end(); it++){
            if ((it->first) == edl_name){
                mErrorEDL = it->second;
            }
        }
    }else{// LR1
        edl_name = CombineStringUint(mStrEDL, mPeId);
        std::map<std::string, unsigned int>::iterator it;
        for (it = mEDLmap_LR1.begin(); it != mEDLmap_LR1.end(); it++){
            if ((it->first) == edl_name){
                mErrorEDL = it->second;
            }
        }
    }
    return edl_name;
}//}}}

/// Issue error to ECC
void ECC_LRAM_E2_FCC2::IssueError(const unsigned int errortype)
{//{{{
    // Not receive error when reset/clock stop
    if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    // Determine set error to LR0 or LR1
    if (mIsEDLConfig){
        if (mStrEDL == "PE"){
            mCurConfigSubName = "LR1";
        }else{// LSU, Bank
            mCurConfigSubName = "LR0";
        }
    }

    // Issue pseudo error
    if ((mIsEDLConfig) && (mIsAddrConfig)){
        if ((((errortype == em1bitError) || (errortype == em2bitError) || (errortype == emAddressFeedbackError)) && (mCurConfigSubName == "LR0"))
                    || (((errortype == em1bitError) || (errortype == em2bitError)) && (mCurConfigSubName == "LR1"))){
            // Calculate mErrorEDL and update mErrorType
            std::string edl_name = CalculateEDLCode();
            mErrorType = (eErrorType)errortype;
            // Process 1bit error
            re_printf("info", "Error input Type: %d, EDL: %s, Address: 0x%08X\n", (unsigned int)errortype, edl_name.c_str(), mErrorAddress);
            if (errortype == em1bitError){
                if (mCurConfigSubName == "LR0"){
                    Update1bitErrorList_LR0(mPeId);
                    mProcess1bitError_LR0Event[mPeId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcess1bitError_LR0Event[mPeId].notify(CalculateCLKPosEdge(mStartClkTime[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else{// LR1
                    Update1bitErrorList_LR1();
                    mProcess1bitError_LR1Event.cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcess1bitError_LR1Event.notify(CalculateCLKPosEdge(mStartClkTime[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
                }

            // Fatal error
            }else{
                if (mCurConfigSubName == "LR0"){
                    UpdateFatalErrorList_LR0(mPeId);
                    mProcessFatalError_LR0Event[mPeId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcessFatalError_LR0Event[mPeId].notify(CalculateCLKPosEdge(mStartClkTime[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else{// LR1
                    UpdateFatalErrorList_LR1();
                    mProcessFatalError_LR1Event.cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcessFatalError_LR1Event.notify(CalculateCLKPosEdge(mStartClkTime[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
                }
            }
        }else{
            re_printf("warning", "Invalid error type (%d) of ECC_LRAM_E2_FCC2_IssueError command.\n", errortype);
        }
    }else{
        re_printf("warning", "The error address and error location are required to configure before issuing error.\n");
    }
}//}}}

/// Config error address
void ECC_LRAM_E2_FCC2::SetErrorAddr(const unsigned int addr_value)
{//{{{
    mErrorAddress = addr_value;
    mIsAddrConfig = true;
}//}}}

/// Config error detection location (EDL)
void ECC_LRAM_E2_FCC2::SetEDL (const std::string edl, const unsigned int pe_index)
{//{{{
    // Dump warning message if config wrong EDL
    if (((edl != "LSU") && (edl != "Bank") && (edl != "PE")) || (pe_index > 7)){
        re_printf("warning", "Invalid EDL (%s, %d) of ECC_LRAM_E2_FCC2_SetEDL command.\n", edl.c_str(), pe_index);
    // Store info to use later (in IssueError function)
    }else{
        mStrEDL = edl;
        mPeId = pe_index;
        mIsEDLConfig = true;
    }
}//}}}

/// Dump Status info
void ECC_LRAM_E2_FCC2::DumpStatusInfo()
{//{{{
    // For LR0 - PEn
    for (unsigned int i = 0; i < PE_NUM; i++){
        std::string type = CombineStringUint("StatInfo LR0 - PE", i);
        printf("PROFILE(%s): ECC_LRAM_E2_FCC2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
        printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR0_SERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_LR0Func[i]->GetLR0_SERSTR());
        printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR0_DERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_LR0Func[i]->GetLR0_DERSTR());
        printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR0_OVFSTR: 0x%08X\n", type.c_str(), mpMECCCAP_LR0Func[i]->GetLR0_OVFSTR());
        for (unsigned int k = 0; k < mCAP_DEPTH_LR0; k++){
            unsigned int sedl  = mpMECCCAP_LR0Func[i]->GetSEDLBit(k);
            unsigned int seadr = mpMECCCAP_LR0Func[i]->GetSEADRBit(k) >> (mAWIDTH_LSB_LR0 - MIN_AWIDTH_LSB);
            printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR0_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type.c_str(), k, sedl, seadr);
        }
        unsigned int dedl  = mpMECCCAP_LR0Func[i]->GetDEDLBit();
        unsigned int deadr = mpMECCCAP_LR0Func[i]->GetDEADRBit() >> (mAWIDTH_LSB_LR0 - MIN_AWIDTH_LSB);
        printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR0_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type.c_str(), dedl, deadr);
    }

    // For LR1
    std::string type1 = "StatInfo LR1";
    printf("PROFILE(%s): ECC_LRAM_E2_FCC2: Info [%20s] (%10s):\n", type1.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR1_SERSTR: 0x%08X\n", type1.c_str(), mpMECCCAP_LR1Func->GetLR1_SERSTR());
    printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR1_DERSTR: 0x%08X\n", type1.c_str(), mpMECCCAP_LR1Func->GetLR1_DERSTR());
    printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR1_OVFSTR: 0x%08X\n", type1.c_str(), mpMECCCAP_LR1Func->GetLR1_OVFSTR());
    for (unsigned int i = 0; i < mCAP_DEPTH_LR1; i++){
        unsigned int sedl  = mpMECCCAP_LR1Func->GetSEDLBit(i);
        unsigned int seadr = mpMECCCAP_LR1Func->GetSEADRBit(i) >> (mAWIDTH_LSB_LR1 - MIN_AWIDTH_LSB);
        printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR1_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type1.c_str(), i, sedl, seadr);
    }
    unsigned int dedl1  = mpMECCCAP_LR1Func->GetDEDLBit();
    unsigned int deadr1 = mpMECCCAP_LR1Func->GetDEADRBit() >> (mAWIDTH_LSB_LR1 - MIN_AWIDTH_LSB);
    printf("PROFILE(%s): ECC_LRAM_E2_FCC2:   LR1_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type1.c_str(), dedl1, deadr1);
}//}}}


/// Command to assert reset
void ECC_LRAM_E2_FCC2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetn") {
        if ((!mIsResetHardActive[empresetnId])&&(!mIsResetCmdReceive[empresetnId])) {
            mIsResetCmdReceive[empresetnId] = true;
            re_printf("info","The model will be reset (presetn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[empresetnId].notify(start_time, SC_NS);
            mResetCmdPeriod[empresetnId] = period;
        } else {
            re_printf("warning","The software reset of presetn is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "sys_resetz") {
        if ((!mIsResetHardActive[emsys_resetzId])&&(!mIsResetCmdReceive[emsys_resetzId])) {
            mIsResetCmdReceive[emsys_resetzId] = true;
            re_printf("info","The model will be reset (sys_resetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emsys_resetzId].notify(start_time, SC_NS);
            mResetCmdPeriod[emsys_resetzId] = period;
        } else {
            re_printf("warning","The software reset of sys_resetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be presetn or sys_resetz\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void ECC_LRAM_E2_FCC2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            mClkOrgFreq[empclkId] = freq;
            ConvertClockFreq (mClkFreq[empclkId], mClkFreqUnit[empclkId], freq, unit);
            if (mClkFreq[empclkId] > 0) {
                mClkPeriod[empclkId] = (sc_dt::uint64)(((1/(double)mClkFreq[empclkId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[empclkId] = sc_time_stamp().to_double();
            } else {
                mClkPeriod[empclkId] = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[empclkId].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "sys_clk") {
            mClkOrgFreq[emsys_clkId] = freq;
            ConvertClockFreq (mClkFreq[emsys_clkId], mClkFreqUnit[emsys_clkId], freq, unit);
            if (mClkFreq[emsys_clkId] > 0) {
                mClkPeriod[emsys_clkId] = (sc_dt::uint64)(((1/(double)mClkFreq[emsys_clkId])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[emsys_clkId] = sc_time_stamp().to_double();
            } else {
                mClkPeriod[emsys_clkId] = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent[emsys_clkId].notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void ECC_LRAM_E2_FCC2::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s\n", (double)mClkOrgFreq[empclkId], mClkFreqUnit[empclkId].c_str());
        } else if (word_vector[0] == "sys_clk") {
            re_printf("info","sys_clk frequency is %0.0f %s\n", (double)mClkOrgFreq[emsys_clkId], mClkFreqUnit[emsys_clkId].c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void ECC_LRAM_E2_FCC2::ForceRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
{//{{{ 
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_LR0"){
                if (block_index < PE_MAX_NUM){
                    mpECCCNT_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR0"){
                if (block_index < PE_MAX_NUM){
                    mpMECCCAP_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR1"){
                mpMECCCAP_LR1Func->RegisterHandler(cmd);
            }else{
                re_printf("warning","Block name (%s) is invalid.\n", block_name.c_str());
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void ECC_LRAM_E2_FCC2::ReleaseRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_LR0"){
                if (block_index < PE_NUM){
                    mpECCCNT_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR0"){
                if (block_index < PE_NUM){
                    mpMECCCAP_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR1"){
                mpMECCCAP_LR1Func->RegisterHandler(cmd);
            }else{
                re_printf("warning","Block name (%s) is invalid.\n", block_name.c_str());
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void ECC_LRAM_E2_FCC2::WriteRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_LR0"){
                if (block_index < PE_NUM){
                    mpECCCNT_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR0"){
                if (block_index < PE_NUM){
                    mpMECCCAP_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR1"){
                mpMECCCAP_LR1Func->RegisterHandler(cmd);
            }else{
                re_printf("warning","Block name (%s) is invalid.\n", block_name.c_str());
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void ECC_LRAM_E2_FCC2::ReadRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_LR0"){
                if (block_index < PE_NUM){
                    mpECCCNT_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR0"){
                if (block_index < PE_NUM){
                    mpMECCCAP_LR0Func[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_LR1"){
                mpMECCCAP_LR1Func->RegisterHandler(cmd);
            }else{
                re_printf("warning","Block name (%s) is invalid.\n", block_name.c_str());
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void ECC_LRAM_E2_FCC2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_LR0Func[i]->RegisterHandler(cmd);
        mpMECCCAP_LR0Func[i]->RegisterHandler(cmd);
    }
    mpMECCCAP_LR1Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void ECC_LRAM_E2_FCC2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_MessageLevel (\"ECC_LRAM_E2_FCC2 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_DumpRegisterRW (\"ECC_LRAM_E2_FCC2 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_DumpInterrupt (\"ECC_LRAM_E2_FCC2 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_AWIDTH_MSB (\"ECC_LRAM_E2_FCC2 instance\", \"sub_name\", value)                                     Set address bit width MSB for LR0 or LR1 (Default: 14 for LR0, 21 for LR1). Valid range to be set: 7-26");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_AWIDTH_LSB (\"ECC_LRAM_E2_FCC2 instance\", \"sub_name\", value)                                     Set address bit width LSB for LR0 or LR1 (Default: 2 for both LR0 and LR1). Valid range to be set: 2-4");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_CAP_DEPTH (\"ECC_LRAM_E2_FCC2 instance\", \"sub_name\", value)                                      Set number of address registers for LR0 or LR1 (Default: 8 for both LR0 or LR1). Valid range to be set: 1-32");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_ERRINT_INIT (\"ECC_LRAM_E2_FCC2 instance\", \"sub_name\", value)                                    Set initial value of ERRINT register for LR0 or LR1 (Default: 0x8F for LR0, 0x87 for LR1). Valid range to be set: 0x0-0xFF");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_EnableRegisterMessage (\"ECC_LRAM_E2_FCC2 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_IssueError (\"ECC_LRAM_E2_FCC2 instance\", error_type)                                              Issue pseudo error to the ECC_LRAM_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_SetErrorAddr (\"ECC_LRAM_E2_FCC2 instance\", addr_value)                                            Set error detection address");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_SetEDL (\"ECC_LRAM_E2_FCC2 instance\", \"edl\", pe_index)                                           Set error detection location");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_DumpStatusInfo (\"ECC_LRAM_E2_FCC2 instance\")                                                      Dump information of the error status registers of the ECC_LRAM_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_AssertReset (\"ECC_LRAM_E2_FCC2 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_LRAM_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_SetCLKFreq (\"ECC_LRAM_E2_FCC2 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_LRAM_E2_FCC2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_GetCLKFreq (\"ECC_LRAM_E2_FCC2 instance\", \"clock_name\")                                          Get clock frequency value of ECC_LRAM_E2_FCC2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_ForceRegister (\"ECC_LRAM_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_ReleaseRegister (\"ECC_LRAM_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_WriteRegister (\"ECC_LRAM_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_ReadRegister (\"ECC_LRAM_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_LRAM_E2_FCC2_ListRegister (\"ECC_LRAM_E2_FCC2 instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_LRAM_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_LRAM_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void ECC_LRAM_E2_FCC2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_LRAM_E2_FCC2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_LRAM_E2_FCC2_EnableRegisterMessage %s \n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_LRAM_E2_FCC2_EnableRegisterMessage %s.\n ", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
