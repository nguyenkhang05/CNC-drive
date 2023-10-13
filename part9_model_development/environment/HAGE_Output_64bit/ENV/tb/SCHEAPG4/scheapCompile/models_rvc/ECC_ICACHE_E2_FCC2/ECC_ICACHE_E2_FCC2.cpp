// ---------------------------------------------------------------------
// $Id: ECC_ICACHE_E2_FCC2.cpp,v 1.17 2019/10/23 08:29:11 landang Exp $
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
#include "ECC_ICACHE_E2_FCC2.h"
#include "ECCCNT_IT_PEn_FCC2_Func.h"
#include "ECCCNT_ID_PEn_FCC2_Func.h"
#include "MECCCAP_IT_PEn_FCC2_Func.h"
#include "MECCCAP_ID_PEn_FCC2_Func.h"
#include <iostream>
/// Constructor and Destructor

ECC_ICACHE_E2_FCC2::ECC_ICACHE_E2_FCC2(sc_module_name name, const uint8_t iPE_NUM, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_ICACHE_E2_FCC2_AgentController()
        ,PE_NUM(iPE_NUM)
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,sys_clk("sys_clk")
        ,sys_resetz("sys_resetz")
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
    else {
       // do nothing
    }
    // New instance and port
    eccdisable_it = new sc_out<bool>*[PE_NUM];
    std::string port_name;
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("eccdisable_it", i);
        eccdisable_it[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(eccdisable_it[i] != NULL);
        eccdisable_it[i]->initialize(false); 
    }
    afedisable_it = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("afedisable_it", i);
        afedisable_it[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(afedisable_it[i] != NULL);
        afedisable_it[i]->initialize(false); 
    }
    eccdisable_id = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("eccdisable_id", i);
        eccdisable_id[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(eccdisable_id[i] != NULL);
        eccdisable_id[i]->initialize(false); 
    }
    afedisable_id = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("afedisable_id", i);
        afedisable_id[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(afedisable_id[i] != NULL);
        afedisable_id[i]->initialize(false); 
    }
    errovfout1_IT = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("errovfout1_IT", i);
        errovfout1_IT[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout1_IT[i] != NULL);
        errovfout1_IT[i]->initialize(false); 
    }
    errovfout0_IT = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("errovfout0_IT", i);
        errovfout0_IT[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout0_IT[i] != NULL);
        errovfout0_IT[i]->initialize(false); 
    }
    dederrout_IT = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("dederrout_IT", i);
        dederrout_IT[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(dederrout_IT[i] != NULL);
        dederrout_IT[i]->initialize(false); 
    }
    afeerrout_IT = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("afeerrout_IT", i);
        afeerrout_IT[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(afeerrout_IT[i] != NULL);
        afeerrout_IT[i]->initialize(false); 
    }
    sederrout_IT = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("sederrout_IT", i);
        sederrout_IT[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(sederrout_IT[i] != NULL);
        sederrout_IT[i]->initialize(false); 
    }
    errovfout1_ID = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("errovfout1_ID", i);
        errovfout1_ID[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout1_ID[i] != NULL);
        errovfout1_ID[i]->initialize(false); 
    }
    errovfout0_ID = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("errovfout0_ID", i);
        errovfout0_ID[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout0_ID[i] != NULL);
        errovfout0_ID[i]->initialize(false); 
    }
    dederrout_ID = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("dederrout_ID", i);
        dederrout_ID[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(dederrout_ID[i] != NULL);
        dederrout_ID[i]->initialize(false); 
    }
    afeerrout_ID = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("afeerrout_ID", i);
        afeerrout_ID[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(afeerrout_ID[i] != NULL);
        afeerrout_ID[i]->initialize(false); 
    }
    sederrout_ID = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("sederrout_ID", i);
        sederrout_ID[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(sederrout_ID[i] != NULL);
        sederrout_ID[i]->initialize(false); 
    }

    mpECCCNT_ITFunc = new ECCCNT_IT_PEn_FCC2_Func*[PE_MAX_NUM];
    mpMECCCAP_ITFunc = new MECCCAP_IT_PEn_FCC2_Func*[PE_MAX_NUM];
    mpECCCNT_IDFunc = new ECCCNT_ID_PEn_FCC2_Func*[PE_MAX_NUM];
    mpMECCCAP_IDFunc = new MECCCAP_ID_PEn_FCC2_Func*[PE_MAX_NUM];

    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        mpECCCNT_ITFunc[i] = new ECCCNT_IT_PEn_FCC2_Func((std::string)name, i, this);
        mpMECCCAP_ITFunc[i] = new MECCCAP_IT_PEn_FCC2_Func((std::string)name, i, this);
        mpECCCNT_IDFunc[i] = new ECCCNT_ID_PEn_FCC2_Func((std::string)name, i, this);
        mpMECCCAP_IDFunc[i] = new MECCCAP_ID_PEn_FCC2_Func((std::string)name, i, this);
    }
    
    BusSlaveBase<32, emNumTgt>::setSlaveResetPort32(&presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
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
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk);
    BusSlaveBase<32, emNumTgt>::setTargetSocket32((char*)"ts_ecccnt_it_0", (char*)"ts_ecccnt_it_1", (char*)"ts_ecccnt_it_2"
                                                , (char*)"ts_ecccnt_it_3", (char*)"ts_ecccnt_it_4", (char*)"ts_ecccnt_it_5"
                                                , (char*)"ts_ecccnt_it_6", (char*)"ts_ecccnt_it_7", (char*)"ts_ecccnt_id_0"
                                                , (char*)"ts_ecccnt_id_1", (char*)"ts_ecccnt_id_2", (char*)"ts_ecccnt_id_3"
                                                , (char*)"ts_ecccnt_id_4", (char*)"ts_ecccnt_id_5", (char*)"ts_ecccnt_id_6"
                                                , (char*)"ts_ecccnt_id_7", (char*)"ts_mecccap_it_0", (char*)"ts_mecccap_it_1"
                                                , (char*)"ts_mecccap_it_2", (char*)"ts_mecccap_it_3", (char*)"ts_mecccap_it_4"
                                                , (char*)"ts_mecccap_it_5", (char*)"ts_mecccap_it_6", (char*)"ts_mecccap_it_7"
                                                , (char*)"ts_mecccap_id_0", (char*)"ts_mecccap_id_1", (char*)"ts_mecccap_id_2"
                                                , (char*)"ts_mecccap_id_3", (char*)"ts_mecccap_id_4", (char*)"ts_mecccap_id_5"
                                                , (char*)"ts_mecccap_id_6", (char*)"ts_mecccap_id_7");

    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_ecccnt_it[i] = BusSlaveBase<32, emNumTgt>::tSocket32[i];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setFuncModulePtr(mpECCCNT_ITFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setWriteLatency(wLatency);
    }
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_ecccnt_id[i] = BusSlaveBase<32, emNumTgt>::tSocket32[PE_MAX_NUM + i];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setFuncModulePtr(mpECCCNT_IDFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + i]->setWriteLatency(wLatency);
    }
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_mecccap_it[i] = BusSlaveBase<32, emNumTgt>::tSocket32[PE_MAX_NUM*2 + i];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2 + i]->setFuncModulePtr(mpMECCCAP_ITFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2 + i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2 + i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*2 + i]->setWriteLatency(wLatency);
    }
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_mecccap_id[i] = BusSlaveBase<32, emNumTgt>::tSocket32[PE_MAX_NUM*3 + i];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*3 + i]->setFuncModulePtr(mpMECCCAP_IDFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*3 + i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*3 + i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM*3 + i]->setWriteLatency(wLatency);
    }

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

    mAWIDTH_MSB_IT = DEFAULT_AWIDTH_MSB_IT;
    mAWIDTH_LSB_IT = DEFAULT_AWIDTH_LSB_IT;
    mCAP_DEPTH_IT  = DEFAULT_CAP_DEPTH_IT;
    mERRINT_INIT_IT= DEFAULT_ERRINT_INIT_IT;
    mAWIDTH_MSB_ID = DEFAULT_AWIDTH_MSB_ID;
    mAWIDTH_LSB_ID = DEFAULT_AWIDTH_LSB_ID;
    mCAP_DEPTH_ID  = DEFAULT_CAP_DEPTH_ID;
    mERRINT_INIT_ID= DEFAULT_ERRINT_INIT_ID;
    
    mDumpInterrupt = false;
    
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
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(MonitorkeyopenMethod);
    dont_initialize();
    sensitive << keyopen;

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDisPort_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteDisPort_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteDisPort_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDisPort_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteDisPort_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteDisPort_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT0_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteERROVFOUT0_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT0_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT1_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteERROVFOUT1_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT1_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT0_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateERROVFOUT0_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT0_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT1_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateERROVFOUT1_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT1_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteSEDERROUT_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteSEDERROUT_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteSEDERROUT_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertSEDERROUT_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::AssertSEDERROUT_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("AssertSEDERROUT_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateSEDERROUT_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateSEDERROUT_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateSEDERROUT_ITMethod"), &opt);
    }
    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateDEDERROUT_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateDEDERROUT_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateDEDERROUT_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDEDERROUT_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteDEDERROUT_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteDEDERROUT_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateAFEERROUT_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateAFEERROUT_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateAFEERROUT_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteAFEERROUT_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteAFEERROUT_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteAFEERROUT_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcess1bitError_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::Process1bitError_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("Process1bitError_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessFatalError_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::ProcessFatalError_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("ProcessFatalError_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSetFatalErrorStatus_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::SetFatalErrorStatus_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("SetFatalErrorStatus_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNotifyFatalError_ITEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NotifyFatalError_ITMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NotifyFatalError_ITMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT0_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteERROVFOUT0_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT0_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT1_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteERROVFOUT1_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT1_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT0_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateERROVFOUT0_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT0_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT1_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateERROVFOUT1_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT1_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteSEDERROUT_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteSEDERROUT_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteSEDERROUT_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertSEDERROUT_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::AssertSEDERROUT_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("AssertSEDERROUT_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateSEDERROUT_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateSEDERROUT_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateSEDERROUT_IDMethod"), &opt);
    }
    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateDEDERROUT_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateDEDERROUT_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateDEDERROUT_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDEDERROUT_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteDEDERROUT_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteDEDERROUT_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateAFEERROUT_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NegateAFEERROUT_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NegateAFEERROUT_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteAFEERROUT_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::WriteAFEERROUT_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteAFEERROUT_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcess1bitError_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::Process1bitError_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("Process1bitError_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessFatalError_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::ProcessFatalError_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("ProcessFatalError_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSetFatalErrorStatus_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::SetFatalErrorStatus_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("SetFatalErrorStatus_IDMethod"), &opt);
    }

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNotifyFatalError_IDEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_ICACHE_E2_FCC2::NotifyFatalError_IDMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("NotifyFatalError_IDMethod"), &opt);
    }

    SC_THREAD(Set1bitErrorStatusThread);
    dont_initialize();
    sensitive << mSet1bitErrorStatusEvent;
    sensitive << mResetHardEvent[emsys_resetzId];
    sensitive << mResetCmdEvent[emsys_resetzId];
    sensitive << mClkZeroEvent[emsys_clkId];
    sensitive << mClkZeroEvent[empclkId];
}//}}} 

/// Destructor

ECC_ICACHE_E2_FCC2::~ECC_ICACHE_E2_FCC2()
{//{{{
    for (unsigned int i = 0; i < PE_NUM; i++){
        delete eccdisable_it[i];
        delete afedisable_it[i];
        delete eccdisable_id[i];
        delete afedisable_id[i];
        delete errovfout1_IT[i];
        delete errovfout0_IT[i];
        delete dederrout_IT[i];
        delete afeerrout_IT[i];
        delete sederrout_IT[i];
        delete errovfout1_ID[i];
        delete errovfout0_ID[i];
        delete dederrout_ID[i];
        delete afeerrout_ID[i];
        delete sederrout_ID[i];
    }

    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        delete mpECCCNT_ITFunc[i];
        delete mpMECCCAP_ITFunc[i];
        delete mpECCCNT_IDFunc[i];
        delete mpMECCCAP_IDFunc[i];
    }

    if (mpECCCNT_ITFunc != NULL) delete [] mpECCCNT_ITFunc;
    if (mpMECCCAP_ITFunc != NULL) delete [] mpMECCCAP_ITFunc;
    if (mpECCCNT_IDFunc != NULL) delete [] mpECCCNT_IDFunc;
    if (mpMECCCAP_IDFunc != NULL) delete [] mpMECCCAP_IDFunc;
    
    if (eccdisable_it != NULL) delete [] eccdisable_it;
    if (afedisable_it != NULL) delete [] afedisable_it;
    if (eccdisable_id != NULL) delete [] eccdisable_id;
    if (afedisable_id != NULL) delete [] afedisable_id;
    if (errovfout1_IT != NULL) delete [] errovfout1_IT;
    if (errovfout0_IT != NULL) delete [] errovfout0_IT;
    if (dederrout_IT != NULL) delete [] dederrout_IT;
    if (afeerrout_IT != NULL) delete [] afeerrout_IT;
    if (sederrout_IT != NULL) delete [] sederrout_IT;
    if (errovfout1_ID != NULL) delete [] errovfout1_ID;
    if (errovfout0_ID != NULL) delete [] errovfout0_ID;
    if (dederrout_ID != NULL) delete [] dederrout_ID;
    if (afeerrout_ID != NULL) delete [] afeerrout_ID;
    if (sederrout_ID != NULL) delete [] sederrout_ID;

}//}}}

/// Create ICACHE EDL Map

void ECC_ICACHE_E2_FCC2::CreateEDLMap()
{//{{{
    std::string edl_name;
    // Create EDL map for IT
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W0T", i);
        mEDLmap_IT.insert(std::pair<std::string, unsigned int>(edl_name, 0x0));
    }
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W1T", i);
        mEDLmap_IT.insert(std::pair<std::string, unsigned int>(edl_name, 0x1));
    }
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W2T", i);
        mEDLmap_IT.insert(std::pair<std::string, unsigned int>(edl_name, 0x2));
    }
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W3T", i);
        mEDLmap_IT.insert(std::pair<std::string, unsigned int>(edl_name, 0x3));
    }
    // Create EDL map for ID
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W0D", i);
        mEDLmap_ID.insert(std::pair<std::string, unsigned int>(edl_name, 0x0));
    }
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W1D", i);
        mEDLmap_ID.insert(std::pair<std::string, unsigned int>(edl_name, 0x1));
    }
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W2D", i);
        mEDLmap_ID.insert(std::pair<std::string, unsigned int>(edl_name, 0x2));
    }
    for (unsigned int i = 0; i < PE_NUM; i++){
        edl_name = CombineStringUint("W3D", i);
        mEDLmap_ID.insert(std::pair<std::string, unsigned int>(edl_name, 0x3));
    }
}//}}}

/// Initialize when reset activated

void ECC_ICACHE_E2_FCC2::Initialize(void)
{//{{{
    for (unsigned int i = 0; i < PE_NUM; i++){
        unsigned int iteccctl  = mpECCCNT_ITFunc[i]->GetITECCCTLReg();
        mECCDIS_IT[i] = (bool)(iteccctl & 0x1);
        mAFEDIS_IT[i] = (bool)((iteccctl >> 3) & 0x1);
        unsigned int ideccctl  = mpECCCNT_IDFunc[i]->GetIDECCCTLReg();
        mECCDIS_ID[i] = (bool)(ideccctl & 0x1);
        mAFEDIS_ID[i] = (bool)((ideccctl >> 3) & 0x1);
    }

    for (unsigned int i = 0; i < PE_NUM; i++){
        mERROVFOUT0_IT[i]  = false;
        mERROVFOUT1_IT[i]  = false;
        mSEDERROUT_IT[i]   = false;
        mDEDERROUT_IT[i]   = false;
        mAFEERROUT_IT[i]   = false;
        mERROVFOUT0_ID[i]  = false;
        mERROVFOUT1_ID[i]  = false;
        mSEDERROUT_ID[i]   = false;
        mDEDERROUT_ID[i]   = false;
        mAFEERROUT_ID[i]   = false;
    }
    
    for (unsigned int i = 0; i < PE_NUM; i++){
        mHas2bitError_IT[i]         = false;
        mHasAddrFeedbackError_IT[i] = false;
        mFatalErrorModAddr_IT[i]    = 0;
        mFatalEDLCode_IT[i]         = 0;
        mSize1bitErrorList_IT[i]    = 0;
        mSizeFatalErrorList_IT[i]   = 0;
        while (!m1bitErrorFIFO_IT[i].empty()){
            m1bitErrorFIFO_IT[i].pop();
        }
        mHas2bitError_ID[i]         = false;
        mHasAddrFeedbackError_ID[i] = false;
        mFatalErrorModAddr_ID[i]    = 0;
        mFatalEDLCode_ID[i]         = 0;
        mSize1bitErrorList_ID[i]    = 0;
        mSizeFatalErrorList_ID[i]   = 0;
        while (!m1bitErrorFIFO_ID[i].empty()){
            m1bitErrorFIFO_ID[i].pop();
        }
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Prepare to write prohibition port of ECCCNT IT

void ECC_ICACHE_E2_FCC2::WriteDisPort_IT(unsigned int pe_id, unsigned int value)
{//{{{
    mECCDIS_IT[pe_id] = (bool)(value & 0x1);
    mAFEDIS_IT[pe_id] = (bool)((value >> 3) & 0x1);
    mWriteDisPort_ITEvent[pe_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_ITEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Prepare to write prohibition port of ECCCNT ID

void ECC_ICACHE_E2_FCC2::WriteDisPort_ID(unsigned int pe_id, unsigned int value)
{//{{{
    mECCDIS_ID[pe_id] = (bool)(value & 0x1);
    mAFEDIS_ID[pe_id] = (bool)((value >> 3) & 0x1);
    mWriteDisPort_IDEvent[pe_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_IDEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get CAP DEPTH

unsigned int ECC_ICACHE_E2_FCC2::GetCapDepth(std::string sub_name)
{//{{{
    sc_assert((sub_name == "IT") || (sub_name == "ID"));
    unsigned int ret = 0;

    if (sub_name == "IT"){
        ret = mCAP_DEPTH_IT;
    }else{//ID
        ret = mCAP_DEPTH_ID;
    }
    return ret;
}//}}}

/// Assert errovfout0_IT port

void ECC_ICACHE_E2_FCC2::AssertERROVFOUT0_IT(unsigned int pe_id)
{//{{{
    mERROVFOUT0_IT[pe_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_ITEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_IT port

void ECC_ICACHE_E2_FCC2::AssertERROVFOUT1_IT(unsigned int pe_id)
{//{{{
    mERROVFOUT1_IT[pe_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_ITEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout0_ID port

void ECC_ICACHE_E2_FCC2::AssertERROVFOUT0_ID(unsigned int pe_id)
{//{{{
    mERROVFOUT0_ID[pe_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_IDEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_ID port

void ECC_ICACHE_E2_FCC2::AssertERROVFOUT1_ID(unsigned int pe_id)
{//{{{
    mERROVFOUT1_ID[pe_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_IDEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get keycode value

bool ECC_ICACHE_E2_FCC2::GetKeyOpen()
{//{{{
    return mKeyOpen;
}//}}}
//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock

void ECC_ICACHE_E2_FCC2::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor sys_clk clock

void ECC_ICACHE_E2_FCC2::Monitorsys_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

/// Monitor presetn port

void ECC_ICACHE_E2_FCC2::MonitorpresetnMethod()
{//{{{
    mResetCurVal[empresetnId] = presetn.read();
    if (mResetCurVal[empresetnId] != mResetPreVal[empresetnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[empresetnId] = mResetCurVal[empresetnId];
        // Reset signals are asynchronous
        mResetHardEvent[empresetnId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor presetn port

void ECC_ICACHE_E2_FCC2::Monitorsys_resetzMethod()
{//{{{
    mResetCurVal[emsys_resetzId] = sys_resetz.read();
    if (mResetCurVal[emsys_resetzId] != mResetPreVal[emsys_resetzId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emsys_resetzId] = mResetCurVal[emsys_resetzId];
        // Reset signals are asynchronous
        mResetHardEvent[emsys_resetzId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Process reset function when reset port is active

void ECC_ICACHE_E2_FCC2::HandleResetHardMethod (const unsigned int reset_id)
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

void ECC_ICACHE_E2_FCC2::HandleResetCmdMethod (const unsigned int reset_id)
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

void ECC_ICACHE_E2_FCC2::CancelResetCmdMethod (const unsigned int reset_id)
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

void ECC_ICACHE_E2_FCC2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    // Write ERRINT_INIT to ERRINT reg after reset
    if (reset_name=="sys_resetz" && is_active){
        for (unsigned int i = 0; i < PE_NUM; i++){
            mpMECCCAP_ITFunc[i]->WriteERRINT_INIT(mERRINT_INIT_IT);
            mpMECCCAP_IDFunc[i]->WriteERRINT_INIT(mERRINT_INIT_ID);
        }
    }
    for (unsigned int i = 0; i < PE_NUM; i++) {
        (mpECCCNT_ITFunc[i])->EnableReset(reset_name, is_active);
        (mpMECCCAP_ITFunc[i])->EnableReset(reset_name, is_active);
        (mpECCCNT_IDFunc[i])->EnableReset(reset_name, is_active);
        (mpMECCCAP_IDFunc[i])->EnableReset(reset_name, is_active);
    }
    if(reset_name=="sys_resetz" && is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        for (unsigned int i = 0; i < PE_NUM; i++){
            mWriteDisPort_ITEvent[i].notify();
            mWriteERROVFOUT0_ITEvent[i].notify();
            mWriteERROVFOUT1_ITEvent[i].notify();
            mWriteSEDERROUT_ITEvent[i].notify();
            mWriteDEDERROUT_ITEvent[i].notify();
            mWriteAFEERROUT_ITEvent[i].notify();

            mWriteDisPort_IDEvent[i].notify();
            mWriteERROVFOUT0_IDEvent[i].notify();
            mWriteERROVFOUT1_IDEvent[i].notify();
            mWriteSEDERROUT_IDEvent[i].notify();
            mWriteDEDERROUT_IDEvent[i].notify();
            mWriteAFEERROUT_IDEvent[i].notify();
        }
    }
}//}}}

/// Cancel events

void ECC_ICACHE_E2_FCC2::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < PE_NUM; i++){
        // Cancel event related to IT
        mWriteDisPort_ITEvent[i].cancel();
        mWriteERROVFOUT0_ITEvent[i].cancel();
        mWriteERROVFOUT1_ITEvent[i].cancel();
        mNegateERROVFOUT0_ITEvent[i].cancel();
        mNegateERROVFOUT1_ITEvent[i].cancel();
        mWriteSEDERROUT_ITEvent[i].cancel();
        mAssertSEDERROUT_ITEvent[i].cancel();
        mNegateSEDERROUT_ITEvent[i].cancel();
        mNegateDEDERROUT_ITEvent[i].cancel();
        mNegateAFEERROUT_ITEvent[i].cancel();
        mWriteDEDERROUT_ITEvent[i].cancel();
        mWriteAFEERROUT_ITEvent[i].cancel();
        mProcess1bitError_ITEvent[i].cancel();
        mProcessFatalError_ITEvent[i].cancel();
        mSetFatalErrorStatus_ITEvent[i].cancel();
        mNotifyFatalError_ITEvent[i].cancel();

        // Cancel event related to ID
        mWriteDisPort_IDEvent[i].cancel();
        mWriteERROVFOUT0_IDEvent[i].cancel();
        mWriteERROVFOUT1_IDEvent[i].cancel();
        mNegateERROVFOUT0_IDEvent[i].cancel();
        mNegateERROVFOUT1_IDEvent[i].cancel();
        mWriteSEDERROUT_IDEvent[i].cancel();
        mAssertSEDERROUT_IDEvent[i].cancel();
        mNegateSEDERROUT_IDEvent[i].cancel();
        mNegateDEDERROUT_IDEvent[i].cancel();
        mNegateAFEERROUT_IDEvent[i].cancel();
        mWriteDEDERROUT_IDEvent[i].cancel();
        mWriteAFEERROUT_IDEvent[i].cancel();
        mProcess1bitError_IDEvent[i].cancel();
        mProcessFatalError_IDEvent[i].cancel();
        mSetFatalErrorStatus_IDEvent[i].cancel();
        mNotifyFatalError_IDEvent[i].cancel();
    }
    mSet1bitErrorStatusEvent.cancel();
}//}}}

/// Get reset status

bool ECC_ICACHE_E2_FCC2::GetResetStatus (const std::string reset_name)
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

bool ECC_ICACHE_E2_FCC2::CheckClockPeriod (const std::string clock_name)
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

void ECC_ICACHE_E2_FCC2::MonitorkeyopenMethod()
{//{{{
    mKeyOpen = keyopen.read();
}//}}}

/// Write prohibition ports of IT

void ECC_ICACHE_E2_FCC2::WriteDisPort_ITMethod(unsigned int pe_id)
{//{{{ 
    eccdisable_it[pe_id]->write(mECCDIS_IT[pe_id]);
    afedisable_it[pe_id]->write(mAFEDIS_IT[pe_id]);
}//}}}

/// Write errovfout0_IT port

void ECC_ICACHE_E2_FCC2::WriteERROVFOUT0_ITMethod(unsigned int pe_id)
{//{{{
    errovfout0_IT[pe_id]->write(mERROVFOUT0_IT[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_IT[%d] is changed to %d\n", pe_id, (unsigned int)mERROVFOUT0_IT[pe_id]);
    }
    // Negate if any
    if (mERROVFOUT0_IT[pe_id]){
        mNegateERROVFOUT0_ITEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_IT port

void ECC_ICACHE_E2_FCC2::WriteERROVFOUT1_ITMethod(unsigned int pe_id)
{//{{{
    errovfout1_IT[pe_id]->write(mERROVFOUT1_IT[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_IT[%d] is changed to %d\n", pe_id, (unsigned int)mERROVFOUT1_IT[pe_id]);
    }
    // Negate if any
    if (mERROVFOUT1_IT[pe_id]){
        mNegateERROVFOUT1_ITEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout0_IT port

void ECC_ICACHE_E2_FCC2::NegateERROVFOUT0_ITMethod(unsigned int pe_id)
{//{{{
    mERROVFOUT0_IT[pe_id] = false;
    mWriteERROVFOUT0_ITEvent[pe_id].notify();
}//}}}

/// Negate errovfout1_IT port

void ECC_ICACHE_E2_FCC2::NegateERROVFOUT1_ITMethod(unsigned int pe_id)
{//{{{
    mERROVFOUT1_IT[pe_id] = false;
    mWriteERROVFOUT1_ITEvent[pe_id].notify();
}//}}}

/// Write sederrout_IT port

void ECC_ICACHE_E2_FCC2::WriteSEDERROUT_ITMethod(unsigned int pe_id)
{//{{{
    sederrout_IT[pe_id]->write(mSEDERROUT_IT[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_IT[%d] is changed to %d\n", pe_id, (unsigned int)mSEDERROUT_IT[pe_id]);
    }
    // Negate if any
    if (mSEDERROUT_IT[pe_id]){
        mNegateSEDERROUT_ITEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)

void ECC_ICACHE_E2_FCC2::AssertSEDERROUT_ITMethod(unsigned int pe_id)
{//{{{
    unsigned int sedie = mpMECCCAP_ITFunc[pe_id]->GetERRINT() & 0x1;
    if (sedie == 1){
        mSEDERROUT_IT[pe_id] = true;
        mWriteSEDERROUT_ITEvent[pe_id].notify();
    }
}//}}}

/// Negate sederrout_IT port

void ECC_ICACHE_E2_FCC2::NegateSEDERROUT_ITMethod(unsigned int pe_id)
{//{{{
    mSEDERROUT_IT[pe_id] = false;
    mWriteSEDERROUT_ITEvent[pe_id].notify();
}//}}}

/// Negate dederrout_IT port

void ECC_ICACHE_E2_FCC2::NegateDEDERROUT_ITMethod(unsigned int pe_id)
{//{{{
    mDEDERROUT_IT[pe_id] = false;
    mWriteDEDERROUT_ITEvent[pe_id].notify();
}//}}}

/// Write dederrout_IT port

void ECC_ICACHE_E2_FCC2::WriteDEDERROUT_ITMethod(unsigned int pe_id)
{//{{{
    dederrout_IT[pe_id]->write(mDEDERROUT_IT[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_IT[%d] is changed to %d\n", pe_id, (unsigned int)mDEDERROUT_IT[pe_id]);
    }
    if (mDEDERROUT_IT[pe_id]){
        mNegateDEDERROUT_ITEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write prohibition ports of ID

void ECC_ICACHE_E2_FCC2::WriteDisPort_IDMethod(unsigned int pe_id)
{//{{{ 
    eccdisable_id[pe_id]->write(mECCDIS_ID[pe_id]);
    afedisable_id[pe_id]->write(mAFEDIS_ID[pe_id]);
}//}}}

/// Write errovfout0_ID port

void ECC_ICACHE_E2_FCC2::WriteERROVFOUT0_IDMethod(unsigned int pe_id)
{//{{{
    errovfout0_ID[pe_id]->write(mERROVFOUT0_ID[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_ID[%d] is changed to %d\n", pe_id, (unsigned int)mERROVFOUT0_ID[pe_id]);
    }
    // Negate if any
    if (mERROVFOUT0_ID[pe_id]){
        mNegateERROVFOUT0_IDEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_ID port

void ECC_ICACHE_E2_FCC2::WriteERROVFOUT1_IDMethod(unsigned int pe_id)
{//{{{
    errovfout1_ID[pe_id]->write(mERROVFOUT1_ID[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_ID[%d] is changed to %d\n", pe_id, (unsigned int)mERROVFOUT1_ID[pe_id]);
    }
    // Negate if any
    if (mERROVFOUT1_ID[pe_id]){
        mNegateERROVFOUT1_IDEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout0_ID port

void ECC_ICACHE_E2_FCC2::NegateERROVFOUT0_IDMethod(unsigned int pe_id)
{//{{{
    mERROVFOUT0_ID[pe_id] = false;
    mWriteERROVFOUT0_IDEvent[pe_id].notify();
}//}}}

/// Negate errovfout1_ID port

void ECC_ICACHE_E2_FCC2::NegateERROVFOUT1_IDMethod(unsigned int pe_id)
{//{{{
    mERROVFOUT1_ID[pe_id] = false;
    mWriteERROVFOUT1_IDEvent[pe_id].notify();
}//}}}

/// Write sederrout_ID port

void ECC_ICACHE_E2_FCC2::WriteSEDERROUT_IDMethod(unsigned int pe_id)
{//{{{
    sederrout_ID[pe_id]->write(mSEDERROUT_ID[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_ID[%d] is changed to %d\n", pe_id, (unsigned int)mSEDERROUT_ID[pe_id]);
    }
    // Negate if any
    if (mSEDERROUT_ID[pe_id]){
        mNegateSEDERROUT_IDEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)

void ECC_ICACHE_E2_FCC2::AssertSEDERROUT_IDMethod(unsigned int pe_id)
{//{{{
    unsigned int sedie = mpMECCCAP_IDFunc[pe_id]->GetERRINT() & 0x1;
    if (sedie == 1){
        mSEDERROUT_ID[pe_id] = true;
        mWriteSEDERROUT_IDEvent[pe_id].notify();
    }
}//}}}

/// Negate sederrout_ID port

void ECC_ICACHE_E2_FCC2::NegateSEDERROUT_IDMethod(unsigned int pe_id)
{//{{{
    mSEDERROUT_ID[pe_id] = false;
    mWriteSEDERROUT_IDEvent[pe_id].notify();
}//}}}

/// Negate dederrout_ID port

void ECC_ICACHE_E2_FCC2::NegateDEDERROUT_IDMethod(unsigned int pe_id)
{//{{{
    mDEDERROUT_ID[pe_id] = false;
    mWriteDEDERROUT_IDEvent[pe_id].notify();
}//}}}

/// Write dederrout_ID port

void ECC_ICACHE_E2_FCC2::WriteDEDERROUT_IDMethod(unsigned int pe_id)
{//{{{
    dederrout_ID[pe_id]->write(mDEDERROUT_ID[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_ID[%d] is changed to %d\n", pe_id, (unsigned int)mDEDERROUT_ID[pe_id]);
    }
    if (mDEDERROUT_ID[pe_id]){
        mNegateDEDERROUT_IDEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate afeerrout_ID port

void ECC_ICACHE_E2_FCC2::NegateAFEERROUT_IDMethod(unsigned int pe_id)
{//{{{
    mAFEERROUT_ID[pe_id] = false;
    mWriteAFEERROUT_IDEvent[pe_id].notify();
}//}}}

/// Write afeerrout_ID port

void ECC_ICACHE_E2_FCC2::WriteAFEERROUT_IDMethod(unsigned int pe_id)
{//{{{
    afeerrout_ID[pe_id]->write(mAFEERROUT_ID[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "afeerrout_ID[%d] is changed to %d\n", pe_id, (unsigned int)mAFEERROUT_ID[pe_id]);
    }
    if (mAFEERROUT_ID[pe_id]){
        mNegateAFEERROUT_IDEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate afeerrout_IT port

void ECC_ICACHE_E2_FCC2::NegateAFEERROUT_ITMethod(unsigned int pe_id)
{//{{{
    mAFEERROUT_IT[pe_id] = false;
    mWriteAFEERROUT_ITEvent[pe_id].notify();
}//}}}

/// Write afeerrout_IT port

void ECC_ICACHE_E2_FCC2::WriteAFEERROUT_ITMethod(unsigned int pe_id)
{//{{{
    afeerrout_IT[pe_id]->write(mAFEERROUT_IT[pe_id]);
    if (mDumpInterrupt){
        re_printf("info", "afeerrout_IT[%d] is changed to %d\n", pe_id, (unsigned int)mAFEERROUT_IT[pe_id]);
    }
    if (mAFEERROUT_IT[pe_id]){
        mNegateAFEERROUT_ITEvent[pe_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit for IT

void ECC_ICACHE_E2_FCC2::Process1bitError_ITMethod(unsigned int pe_id)
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_IT; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList_IT[pe_id]; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList_IT[pe_id][i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO_IT[pe_id].size() < emTotalSlavePorts_IT) {// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO_IT[pe_id].push(m1bitErrorList_IT[pe_id][i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_ITFunc[pe_id]->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList_IT[pe_id]; i++){
        unsigned int mod_address = ShiftAddress("IT", m1bitErrorList_IT[pe_id][i].address);
        if (Check1bitAddressNotCaptured_IT(pe_id, mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_ITEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList_IT[pe_id] = 0;
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit for ID

void ECC_ICACHE_E2_FCC2::Process1bitError_IDMethod(unsigned int pe_id)
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_ID; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList_ID[pe_id]; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList_ID[pe_id][i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO_ID[pe_id].size() < emTotalSlavePorts_ID) {// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO_ID[pe_id].push(m1bitErrorList_ID[pe_id][i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_IDFunc[pe_id]->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList_ID[pe_id]; i++){
        unsigned int mod_address = ShiftAddress("ID",m1bitErrorList_ID[pe_id][i].address);
        if (Check1bitAddressNotCaptured_ID(pe_id, mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_IDEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList_ID[pe_id] = 0;
}//}}}

/// Process 1bit error for both MEMCAP 

void ECC_ICACHE_E2_FCC2::Set1bitErrorStatusThread()
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

                    // Process FIFOs of IT
                    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++){
                        if (m1bitErrorFIFO_IT[pe_id].size() != 0){
                            unsigned int mod_address = ShiftAddress("IT", m1bitErrorFIFO_IT[pe_id].front().address);
                            if (mpMECCCAP_ITFunc[pe_id]->CheckHasRoomFor1bit()){
                                // then set status bit if there is room
                                unsigned int edl_code = m1bitErrorFIFO_IT[pe_id].front().edl_code;
                                if (Check1bitAddressNotCaptured_IT(pe_id, mod_address)){
                                    mpMECCCAP_ITFunc[pe_id]->Set1bitStatusAndAddress(edl_code, mod_address);
                                }
                            } else {
                                //ECC 1bit error was detacted when all flag are set.
                                if (Check1bitAddressNotCaptured_IT(pe_id, mod_address)){
                                    mpMECCCAP_ITFunc[pe_id]->SetFlagAddressBufferOverflow();
                            while (!m1bitErrorFIFO_IT[pe_id].empty()){
                                m1bitErrorFIFO_IT[pe_id].pop();
                            }
                                }
                            }
                            //remove FIFO
                            if (!m1bitErrorFIFO_IT[pe_id].empty()){
                                m1bitErrorFIFO_IT[pe_id].pop();
                            }
                        }
                    }

                    // Process FIFOs of ID
                    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++){
                        if (m1bitErrorFIFO_ID[pe_id].size() != 0){
                            unsigned int mod_address = ShiftAddress("ID", m1bitErrorFIFO_ID[pe_id].front().address);
                            if (mpMECCCAP_IDFunc[pe_id]->CheckHasRoomFor1bit()){
                                // then set status bit if there is room
                                unsigned int edl_code = m1bitErrorFIFO_ID[pe_id].front().edl_code;
                                if (Check1bitAddressNotCaptured_ID(pe_id, mod_address)){
                                    mpMECCCAP_IDFunc[pe_id]->Set1bitStatusAndAddress(edl_code, mod_address);
                                }
                            } else {
                                //ECC 1bit error was detacted when all flag are set.
                                if (Check1bitAddressNotCaptured_ID(pe_id, mod_address)){
                                    mpMECCCAP_IDFunc[pe_id]->SetFlagAddressBufferOverflow();
                            while (!m1bitErrorFIFO_ID[pe_id].empty()){
                                m1bitErrorFIFO_ID[pe_id].pop();
                            }
                                }
                            }
                            //remove FIFO
                            if (!m1bitErrorFIFO_ID[pe_id].empty()){
                                m1bitErrorFIFO_ID[pe_id].pop();
                            }
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

bool ECC_ICACHE_E2_FCC2::CheckAnyFIFOHasElement()
{//{{{
    for (unsigned int i = 0; i < PE_NUM; i++){
        if (m1bitErrorFIFO_IT[i].size() != 0){
            return true;
        }
        if (m1bitErrorFIFO_ID[i].size() != 0){
            return true;
        }
    }
    return false;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit for IT

void ECC_ICACHE_E2_FCC2::ProcessFatalError_ITMethod(unsigned int pe_id)
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError_IT[pe_id] = false;
    mHasAddrFeedbackError_IT[pe_id] = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_IT; portindex++){// With fix priority
        if (is_update_status){
            break;
        }
        for (unsigned int i = 0; i < mSizeFatalErrorList_IT[pe_id]; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList_IT[pe_id][i].edl_code == portindex){
                if (CheckNoFatalStatus_IT(pe_id)){
                    mHas2bitError_IT[pe_id] |= (mFatalErrorList_IT[pe_id][i].type == em2bitError);
                    mHasAddrFeedbackError_IT[pe_id] |= (mFatalErrorList_IT[pe_id][i].type == emAddressFeedbackError);
                    mod_address = ShiftAddress("IT", mFatalErrorList_IT[pe_id][i].address);
                    edl_code = mFatalErrorList_IT[pe_id][i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalEDLCode_IT[pe_id] = edl_code;
        mFatalErrorModAddr_IT[pe_id] = mod_address;
        mSetFatalErrorStatus_ITEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList_IT[pe_id]; i++){
        if (mFatalErrorList_IT[pe_id][i].type == em2bitError){
            mDEDERROUT_IT[pe_id] |= true;
        }
        if (mFatalErrorList_IT[pe_id][i].type == emAddressFeedbackError){
            mAFEERROUT_IT[pe_id] |= true;
        }
    }
    if (mDEDERROUT_IT[pe_id] || mAFEERROUT_IT[pe_id]){
        mNotifyFatalError_ITEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList_IT[pe_id] = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit for ID

void ECC_ICACHE_E2_FCC2::ProcessFatalError_IDMethod(unsigned int pe_id)
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError_ID[pe_id] = false;
    mHasAddrFeedbackError_ID[pe_id] = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_ID; portindex++){// With fix priority
        if (is_update_status){
            break;
        }
        for (unsigned int i = 0; i < mSizeFatalErrorList_ID[pe_id]; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList_ID[pe_id][i].edl_code == portindex){
                if (CheckNoFatalStatus_ID(pe_id)){
                    mHas2bitError_ID[pe_id] |= (mFatalErrorList_ID[pe_id][i].type == em2bitError);
                    mHasAddrFeedbackError_ID[pe_id] |= (mFatalErrorList_ID[pe_id][i].type == emAddressFeedbackError);
                    mod_address = ShiftAddress("ID", mFatalErrorList_ID[pe_id][i].address);
                    edl_code = mFatalErrorList_ID[pe_id][i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalEDLCode_ID[pe_id] = edl_code;
        mFatalErrorModAddr_ID[pe_id] = mod_address;
        mSetFatalErrorStatus_IDEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList_ID[pe_id]; i++){
        if (mFatalErrorList_ID[pe_id][i].type == em2bitError){
            mDEDERROUT_ID[pe_id] |= true;
        }
        if (mFatalErrorList_ID[pe_id][i].type == emAddressFeedbackError){
            mAFEERROUT_ID[pe_id] |= true;
        }
    }
    if (mDEDERROUT_ID[pe_id] || mAFEERROUT_ID[pe_id]){
        mNotifyFatalError_IDEvent[pe_id].notify(CalculateCLKPosEdge((double)mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList_ID[pe_id] = 0;
}//}}}

/// Set fatal status bit for IT

void ECC_ICACHE_E2_FCC2::SetFatalErrorStatus_ITMethod(unsigned int pe_id)
{//{{{
    if (mHas2bitError_IT[pe_id]){
        mpMECCCAP_ITFunc[pe_id]->Set2bitErrorStatus();
        mpMECCCAP_ITFunc[pe_id]->SetFatalAddress(mFatalEDLCode_IT[pe_id], mFatalErrorModAddr_IT[pe_id]);
    }
    if (mHasAddrFeedbackError_IT[pe_id]){
        mpMECCCAP_ITFunc[pe_id]->SetAddrFeedbackErrorStatus();
        mpMECCCAP_ITFunc[pe_id]->SetFatalAddress(mFatalEDLCode_IT[pe_id], mFatalErrorModAddr_IT[pe_id]);
    }
}//}}}

/// Set fatal status bit for ID

void ECC_ICACHE_E2_FCC2::SetFatalErrorStatus_IDMethod(unsigned int pe_id)
{//{{{
    if (mHas2bitError_ID[pe_id]){
        mpMECCCAP_IDFunc[pe_id]->Set2bitErrorStatus();
        mpMECCCAP_IDFunc[pe_id]->SetFatalAddress(mFatalEDLCode_ID[pe_id], mFatalErrorModAddr_ID[pe_id]);
    }
    if (mHasAddrFeedbackError_ID[pe_id]){
        mpMECCCAP_IDFunc[pe_id]->SetAddrFeedbackErrorStatus();
        mpMECCCAP_IDFunc[pe_id]->SetFatalAddress(mFatalEDLCode_ID[pe_id], mFatalErrorModAddr_ID[pe_id]);
    }
}//}}}

/// Notify fatal error for IT

void ECC_ICACHE_E2_FCC2::NotifyFatalError_ITMethod(unsigned int pe_id)
{//{{{
    unsigned int dedie = (mpMECCCAP_ITFunc[pe_id]->GetERRINT() >> 1) & 0x1;
    unsigned int afeie = (mpMECCCAP_ITFunc[pe_id]->GetERRINT() >> 3) & 0x1;
    if (mDEDERROUT_IT[pe_id] && (dedie == 1)){
        mWriteDEDERROUT_ITEvent[pe_id].notify();
    }else{
        mDEDERROUT_IT[pe_id] = false;
    }
    if (mAFEERROUT_IT[pe_id] && (afeie == 1)){
        mWriteAFEERROUT_ITEvent[pe_id].notify();
    }else{
        mAFEERROUT_IT[pe_id] = false;
    }
}//}}}

/// Notify fatal error for ID

void ECC_ICACHE_E2_FCC2::NotifyFatalError_IDMethod(unsigned int pe_id)
{//{{{
    unsigned int dedie = (mpMECCCAP_IDFunc[pe_id]->GetERRINT() >> 1) & 0x1;
    unsigned int afeie = (mpMECCCAP_IDFunc[pe_id]->GetERRINT() >> 3) & 0x1;
    if (mDEDERROUT_ID[pe_id] && (dedie == 1)){
        mWriteDEDERROUT_IDEvent[pe_id].notify();
    }else{
        mDEDERROUT_ID[pe_id] = false;
    }
    if (mAFEERROUT_ID[pe_id] && (afeie == 1)){
        mWriteAFEERROUT_IDEvent[pe_id].notify();
    }else{
        mAFEERROUT_ID[pe_id] = false;
    }
}//}}}

/// Check no fatal status of IT

bool ECC_ICACHE_E2_FCC2::CheckNoFatalStatus_IT(unsigned int pe_id)
{//{{{
    return mpMECCCAP_ITFunc[pe_id]->CheckNoFatalStatus();
}//}}}

/// Check no fatal status of ID

bool ECC_ICACHE_E2_FCC2::CheckNoFatalStatus_ID(unsigned int pe_id)
{//{{{
    return mpMECCCAP_IDFunc[pe_id]->CheckNoFatalStatus();
}//}}}

/// Check this address is captured in 1bit address register buffer for IT

bool ECC_ICACHE_E2_FCC2::Check1bitAddressNotCaptured_IT(unsigned int pe_id, unsigned int mod_address)
{//{{{
    for (unsigned int i = 0; i < mCAP_DEPTH_IT; i++){
        unsigned int cap_addr = mpMECCCAP_ITFunc[pe_id]->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_ITFunc[pe_id]->GetIT_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Check this address is captured in 1bit address register buffer for ID

bool ECC_ICACHE_E2_FCC2::Check1bitAddressNotCaptured_ID(unsigned int pe_id, unsigned int mod_address)
{//{{{
    for (unsigned int i = 0; i < mCAP_DEPTH_ID; i++){
        unsigned int cap_addr = mpMECCCAP_IDFunc[pe_id]->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_IDFunc[pe_id]->GetID_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Update 1bit error list for IT

void ECC_ICACHE_E2_FCC2::Update1bitErrorList_IT(unsigned int pe_id)
{//{{{
    bool is_same_port = false;

    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList_IT[pe_id]; i++){
        if (mErrorEDL == m1bitErrorList_IT[pe_id][i].edl_code){
            m1bitErrorList_IT[pe_id][i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList_IT[pe_id][mSize1bitErrorList_IT[pe_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList_IT[pe_id]++;
    }
}//}}}

/// Update 1bit error list for ID

void ECC_ICACHE_E2_FCC2::Update1bitErrorList_ID(unsigned int pe_id)
{//{{{
    bool is_same_port = false;

    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList_ID[pe_id]; i++){
        if (mErrorEDL == m1bitErrorList_ID[pe_id][i].edl_code){
            m1bitErrorList_ID[pe_id][i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList_ID[pe_id][mSize1bitErrorList_ID[pe_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList_ID[pe_id]++;
    }
}//}}}

/// Update fatal error list for IT

void ECC_ICACHE_E2_FCC2::UpdateFatalErrorList_IT(unsigned int pe_id)
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if havingpe_id change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList_IT[pe_id]; i++){
        if (mErrorEDL == mFatalErrorList_IT[pe_id][i].edl_code){
            mFatalErrorList_IT[pe_id][i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList_IT[pe_id][i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList_IT[pe_id][mSizeFatalErrorList_IT[pe_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList_IT[pe_id]++;
    }
}//}}}

/// Update fatal error list for ID

void ECC_ICACHE_E2_FCC2::UpdateFatalErrorList_ID(unsigned int pe_id)
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if havingpe_id change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList_ID[pe_id]; i++){
        if (mErrorEDL == mFatalErrorList_ID[pe_id][i].edl_code){
            mFatalErrorList_ID[pe_id][i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList_ID[pe_id][i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList_ID[pe_id][mSizeFatalErrorList_ID[pe_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList_ID[pe_id]++;
    }
}//}}}

/// Combine string and unsigned int

std::string ECC_ICACHE_E2_FCC2::CombineStringUint(std::string name, unsigned int index)
{//{{{
    std::ostringstream ret_string;
    ret_string.str("");
    ret_string << name << index;
    return ret_string.str();
}//}}}

/// Shift address to corresponding with MSB/LSB

unsigned int ECC_ICACHE_E2_FCC2::ShiftAddress(std::string sub_name, unsigned int address)
{//{{{
    sc_assert((sub_name == "IT") || (sub_name == "ID"));
    unsigned int mod_addr = 0;
    if (sub_name == "IT"){
        mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB_IT + 1)) - 1))>>mAWIDTH_LSB_IT) << (mAWIDTH_LSB_IT - MIN_AWIDTH_LSB);
    }else{ // ID
        mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB_ID + 1)) - 1))>>mAWIDTH_LSB_ID) << (mAWIDTH_LSB_ID - MIN_AWIDTH_LSB);
    }
    return mod_addr;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)

void ECC_ICACHE_E2_FCC2::SetMessageLevel (const std::string msg_lv)
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
            mpECCCNT_ITFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_ITFunc[i]->RegisterHandler(cmd);
            mpECCCNT_IDFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_IDFunc[i]->RegisterHandler(cmd);
        }

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_ITFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_ITFunc[i]->RegisterHandler(cmd);
            mpECCCNT_IDFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_IDFunc[i]->RegisterHandler(cmd);
        }
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_ITFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_ITFunc[i]->RegisterHandler(cmd);
            mpECCCNT_IDFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_IDFunc[i]->RegisterHandler(cmd);
        }

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_ITFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_ITFunc[i]->RegisterHandler(cmd);
            mpECCCNT_IDFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_IDFunc[i]->RegisterHandler(cmd);
        }
    }
}//}}}

/// Enable/disable dumping message when users access registers

void ECC_ICACHE_E2_FCC2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);

    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_ITFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_ITFunc[i]->RegisterHandler(cmd);
        mpECCCNT_IDFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_IDFunc[i]->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping interrupt information

void ECC_ICACHE_E2_FCC2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_ICACHE_E2_FCC2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_ICACHE_E2_FCC2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_ICACHE_E2_FCC2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Set AWIDTH_MSB

void ECC_ICACHE_E2_FCC2::AWIDTH_MSB(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "IT"){
        if ((mAWIDTH_LSB_IT <= value) && (value <= MAX_ADDWIDTH_MSB)){
            mAWIDTH_MSB_IT = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_ICACHE_E2_FCC2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB_IT, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB_IT);
        }
    }else if (sub_name == "ID"){
        if ((mAWIDTH_LSB_ID <= value) && (value <= MAX_ADDWIDTH_MSB)){
            mAWIDTH_MSB_ID = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_ICACHE_E2_FCC2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB_ID, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB_ID);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_ICACHE_E2_FCC2_AWIDTH_MSB parameter. Valid name of sub-block is IT or ID.\n", sub_name.c_str());
    }
}//}}}

/// Set AWIDTH_LSB

void ECC_ICACHE_E2_FCC2::AWIDTH_LSB(const std::string sub_name, const unsigned int value)
{//{{
    if (sub_name == "IT"){
        if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB_IT)){
            mAWIDTH_LSB_IT = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_ICACHE_E2_FCC2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB_IT, DEFAULT_AWIDTH_LSB_IT);
        }
    }else if (sub_name == "ID"){
        if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB_ID)){
            mAWIDTH_LSB_ID = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_ICACHE_E2_FCC2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB_ID, DEFAULT_AWIDTH_LSB_ID);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_ICACHE_E2_FCC2_AWIDTH_LSB parameter. Valid name of sub-block is IT or ID.\n", sub_name.c_str());
    }
}//}}}

/// Set CAP_DEPTH

void ECC_ICACHE_E2_FCC2::CAP_DEPTH(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "IT"){
        if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
            mCAP_DEPTH_IT = value;
        }else{
            re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_ICACHE_E2_FCC2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH_IT);
        }
    }else if (sub_name == "ID"){
        if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
            mCAP_DEPTH_ID = value;
        }else{
            re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_ICACHE_E2_FCC2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH_ID);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_ICACHE_E2_FCC2_CAP_DEPTH parameter. Valid name of sub-block is IT or ID.\n", sub_name.c_str());
    }
}//}}}

/// Set ERRINT_INIT

void ECC_ICACHE_E2_FCC2::ERRINT_INIT(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "IT"){
        if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
            mERRINT_INIT_IT = value;
        }else{
            re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_ICACHE_E2_FCC2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT_IT);
        }
    }else if (sub_name == "ID"){
        if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
            mERRINT_INIT_ID = value;
        }else{
            re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_ICACHE_E2_FCC2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT_ID);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_ICACHE_E2_FCC2_ERRINT_INIT parameter. Valid name of sub-block is IT or ID.\n", sub_name.c_str());
    }
}//}}}

/// Calculate EDL code

std::string ECC_ICACHE_E2_FCC2::CalculateEDLCode()
{//{{{
    std::string edl_name = mStrEDL;
    if (mCurConfigSubName == "IT"){
        std::map<std::string, unsigned int>::iterator it;
        for (it = mEDLmap_IT.begin(); it != mEDLmap_IT.end(); it++){
            if ((it->first) == edl_name){
                mErrorEDL = it->second;
            }
        }
    }else{//"ID"
        std::map<std::string, unsigned int>::iterator it;
        for (it = mEDLmap_ID.begin(); it != mEDLmap_ID.end(); it++){
            if ((it->first) == edl_name){
                mErrorEDL = it->second;
            }
        }
    }
    return edl_name;
}//}}}

/// Issue error to ECC

void ECC_ICACHE_E2_FCC2::IssueError(const unsigned int errortype)
{//{{{
    // Not receive error when reset/clock stop
    if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    // Issue pseudo error
    if ((mIsEDLConfig) && (mIsAddrConfig)){
        if (((errortype == em1bitError) || (errortype == em2bitError) || (errortype == emAddressFeedbackError)) && ((mCurConfigSubName == "IT") || (mCurConfigSubName == "ID"))){
            // Calculate mErrorEDL and update mErrorType
            std::string edl_name = CalculateEDLCode();
            mErrorType = (eErrorType)errortype;
            re_printf("info", "Error input Type: %d, EDL: %s, Address: 0x%08X\n", errortype, edl_name.c_str(), mErrorAddress);
            // Process 1bit error
            if (errortype == em1bitError){
                if (mCurConfigSubName == "IT"){
                    Update1bitErrorList_IT(mPeId);
                    mProcess1bitError_ITEvent[mPeId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcess1bitError_ITEvent[mPeId].notify(CalculateCLKPosEdge((double)mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else{//"ID"
                    Update1bitErrorList_ID(mPeId);
                    mProcess1bitError_IDEvent[mPeId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcess1bitError_IDEvent[mPeId].notify(CalculateCLKPosEdge((double)mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
                }

            // Fatal error
            }else{
                if (mCurConfigSubName == "IT"){
                    UpdateFatalErrorList_IT(mPeId);
                    mProcessFatalError_ITEvent[mPeId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcessFatalError_ITEvent[mPeId].notify(CalculateCLKPosEdge((double)mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else{//"ID"
                    UpdateFatalErrorList_ID(mPeId);
                    mProcessFatalError_IDEvent[mPeId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcessFatalError_IDEvent[mPeId].notify(CalculateCLKPosEdge((double)mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
                }
            }
        }else{
            re_printf("warning", "Invalid error type (%d) of ECC_ICACHE_E2_FCC2_IssueError command.\n", errortype);
        }
    }else{
        re_printf("warning", "The error address and error location are required to configure before issuing error.\n");
    }
}//}}}

/// Config error address

void ECC_ICACHE_E2_FCC2::SetErrorAddr(const unsigned int addr_value)
{//{{{
    mErrorAddress = addr_value;
    mIsAddrConfig = true;
}//}}}

/// Config error detection location (EDL)

void ECC_ICACHE_E2_FCC2::SetEDL (const std::string edl, const unsigned int pe_index)
{//{{{
    std::string edl_name = CombineStringUint(edl, pe_index);
    std::map<std::string, unsigned int>::iterator it;

    // Update EDL if any
    for (it = mEDLmap_IT.begin(); it != mEDLmap_IT.end(); it++){
        if ((it->first) == edl_name){
            mErrorEDL = it->second;
            mStrEDL = edl_name;
            mIsEDLConfig = true;
            mPeId = pe_index;
            mCurConfigSubName = "IT";
        }
    }
    for (it = mEDLmap_ID.begin(); it != mEDLmap_ID.end(); it++){
        if ((it->first) == edl_name){
            mErrorEDL = it->second;
            mStrEDL = edl_name;
            mIsEDLConfig = true;
            mPeId = pe_index;
            mCurConfigSubName = "ID";
        }
    }
    
    // Dump warning message if config wrong EDL
    if (!mIsEDLConfig){
        re_printf("warning", "Invalid EDL (%s, %d) of ECC_ICACHE_E2_FCC2_SetEDL command.\n", edl.c_str(), pe_index);
    }
}//}}}

/// Dump Status info

void ECC_ICACHE_E2_FCC2::DumpStatusInfo()
{//{{{
    // For IT - PEn
    for (unsigned int i = 0; i < PE_NUM; i++){
        std::string type = CombineStringUint("StatInfo IT - PE", i);
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   IT_SERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_ITFunc[i]->GetIT_SERSTR());
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   IT_DERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_ITFunc[i]->GetIT_DERSTR());
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   IT_OVFSTR: 0x%08X\n", type.c_str(), mpMECCCAP_ITFunc[i]->GetIT_OVFSTR());
        for (unsigned int k = 0; k < mCAP_DEPTH_IT; k++){
            unsigned int sedl  = mpMECCCAP_ITFunc[i]->GetSEDLBit(k);
            unsigned int seadr = mpMECCCAP_ITFunc[i]->GetSEADRBit(k) >> (mAWIDTH_LSB_IT - MIN_AWIDTH_LSB);
            printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   IT_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type.c_str(), k, sedl, seadr);
        }
        unsigned int dedl  = mpMECCCAP_ITFunc[i]->GetDEDLBit();
        unsigned int deadr = mpMECCCAP_ITFunc[i]->GetDEADRBit() >> (mAWIDTH_LSB_IT - MIN_AWIDTH_LSB);
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   IT_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type.c_str(), dedl, deadr);
    }

    // For ID - PEn
    for (unsigned int i = 0; i < PE_NUM; i++){
        std::string type = CombineStringUint("StatInfo ID - PE", i);
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   ID_SERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_IDFunc[i]->GetID_SERSTR());
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   ID_DERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_IDFunc[i]->GetID_DERSTR());
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   ID_OVFSTR: 0x%08X\n", type.c_str(), mpMECCCAP_IDFunc[i]->GetID_OVFSTR());
        for (unsigned int k = 0; k < mCAP_DEPTH_ID; k++){
            unsigned int sedl  = mpMECCCAP_IDFunc[i]->GetSEDLBit(k);
            unsigned int seadr = mpMECCCAP_IDFunc[i]->GetSEADRBit(k) >> (mAWIDTH_LSB_ID - MIN_AWIDTH_LSB);
            printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   ID_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type.c_str(), k, sedl, seadr);
        }
        unsigned int dedl  = mpMECCCAP_IDFunc[i]->GetDEDLBit();
        unsigned int deadr = mpMECCCAP_IDFunc[i]->GetDEADRBit() >> (mAWIDTH_LSB_ID - MIN_AWIDTH_LSB);
        printf("PROFILE(%s): ECC_ICACHE_E2_FCC2:   ID_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type.c_str(), dedl, deadr);
    }
}//}}}


/// Command to assert reset

void ECC_ICACHE_E2_FCC2::AssertReset (const std::string reset_name, const double start_time, const double period)
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

void ECC_ICACHE_E2_FCC2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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

void ECC_ICACHE_E2_FCC2::GetCLKFreq (const std::string clock_name)
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

void ECC_ICACHE_E2_FCC2::ForceRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_IT"){
                if (block_index < PE_NUM){
                    mpECCCNT_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_ID"){
                if (block_index < PE_NUM){
                    mpECCCNT_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_IT"){
                if (block_index < PE_NUM){
                    mpMECCCAP_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_ID"){
                if (block_index < PE_NUM){
                    mpMECCCAP_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
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

void ECC_ICACHE_E2_FCC2::ReleaseRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
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
            if (block_name == "ECCCNT_IT"){
                if (block_index < PE_NUM){
                    mpECCCNT_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_ID"){
                if (block_index < PE_NUM){
                    mpECCCNT_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_IT"){
                if (block_index < PE_NUM){
                    mpMECCCAP_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_ID"){
                if (block_index < PE_NUM){
                    mpMECCCAP_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
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

void ECC_ICACHE_E2_FCC2::WriteRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_IT"){
                if (block_index < PE_NUM){
                    mpECCCNT_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_ID"){
                if (block_index < PE_NUM){
                    mpECCCNT_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_IT"){
                if (block_index < PE_NUM){
                    mpMECCCAP_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_ID"){
                if (block_index < PE_NUM){
                    mpMECCCAP_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
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

void ECC_ICACHE_E2_FCC2::ReadRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_IT"){
                if (block_index < PE_NUM){
                    mpECCCNT_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_ID"){
                if (block_index < PE_NUM){
                    mpECCCNT_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_IT"){
                if (block_index < PE_NUM){
                    mpMECCCAP_ITFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_ID"){
                if (block_index < PE_NUM){
                    mpMECCCAP_IDFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
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

void ECC_ICACHE_E2_FCC2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_ITFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_ITFunc[i]->RegisterHandler(cmd);
        mpECCCNT_IDFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_IDFunc[i]->RegisterHandler(cmd);
    }
}//}}}

/// Dump help message of all parameters or commands

void ECC_ICACHE_E2_FCC2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_MessageLevel (\"ECC_ICACHE_E2_FCC2 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_DumpRegisterRW (\"ECC_ICACHE_E2_FCC2 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_DumpInterrupt (\"ECC_ICACHE_E2_FCC2 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_AWIDTH_MSB (\"ECC_ICACHE_E2_FCC2 instance\", \"sub_name\", value)                                     Set address bit width MSB for IT or ID (Default: 11 for IT, ID). Valid range to be set: 7-26");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_AWIDTH_LSB (\"ECC_ICACHE_E2_FCC2 instance\", \"sub_name\", value)                                     Set address bit width LSB for IT or ID (Default: 5 for IT, 3 for ID). Valid range to be set: 2-4");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_CAP_DEPTH (\"ECC_ICACHE_E2_FCC2 instance\", \"sub_name\", value)                                      Set number of address registers for IT or ID (Default: 1 for both IT, ID). Valid range to be set: 1-32");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_ERRINT_INIT (\"ECC_ICACHE_E2_FCC2 instance\", \"sub_name\", value)                                    Set initial value of ERRINT register for IT or ID (Default: 0x0 for IT, ID). Valid range to be set: 0x0-0xFF");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_EnableRegisterMessage (\"ECC_ICACHE_E2_FCC2 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_IssueError (\"ECC_ICACHE_E2_FCC2 instance\", error_type)                                              Issue pseudo error to the ECC_ICACHE_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_SetErrorAddr (\"ECC_ICACHE_E2_FCC2 instance\", addr_value)                                            Set error detection address");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_SetEDL (\"ECC_ICACHE_E2_FCC2 instance\", \"edl\", pe_index)                                           Set error detection location");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_DumpStatusInfo (\"ECC_ICACHE_E2_FCC2 instance\")                                                      Dump information of the error status registers of the ECC_ICACHE_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_AssertReset (\"ECC_ICACHE_E2_FCC2 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_ICACHE_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_SetCLKFreq (\"ECC_ICACHE_E2_FCC2 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_ICACHE_E2_FCC2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_GetCLKFreq (\"ECC_ICACHE_E2_FCC2 instance\", \"clock_name\")                                          Get clock frequency value of ECC_ICACHE_E2_FCC2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_ForceRegister (\"ECC_ICACHE_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_ReleaseRegister (\"ECC_ICACHE_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_WriteRegister (\"ECC_ICACHE_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_ReadRegister (\"ECC_ICACHE_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_ICACHE_E2_FCC2_ListRegister (\"ECC_ICACHE_E2_FCC2 instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_ICACHE_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_ICACHE_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}


void ECC_ICACHE_E2_FCC2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_ICACHE_E2_FCC2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_ICACHE_E2_FCC2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_ICACHE_E2_FCC2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
