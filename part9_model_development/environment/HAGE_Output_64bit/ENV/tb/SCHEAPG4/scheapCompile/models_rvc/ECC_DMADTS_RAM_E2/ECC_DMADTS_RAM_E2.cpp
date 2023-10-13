// ---------------------------------------------------------------------
// $Id: ECC_DMADTS_RAM_E2.cpp,v 1.14 2019/10/23 08:29:05 landang Exp $
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
#include "Common.h"
#include "ECC_DMADTS_RAM_E2.h"
#include "ECCCNT_DTS_Func.h"
#include "ECCCNT_DMDEn_Func.h"
#include "ECCCNT_DMDAn_Func.h"
#include "MECCCAP_DTS_Func.h"
#include "MECCCAP_DMDEn_Func.h"
#include "MECCCAP_DMDAn_Func.h"

/// Constructor and Destructor

ECC_DMADTS_RAM_E2::ECC_DMADTS_RAM_E2(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_DMADTS_RAM_E2_AgentController()
        ,rvc_common_model()
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,sys_clk("sys_clk")
        ,sys_resetz("sys_resetz")
        // ECCCNT DTS
        ,eccdisable_dr("eccdisable_dr")
        ,secdisable_dr("secdisable_dr")
        ,afedisable_dr("afedisable_dr")
        // MECCCAP DTS
        ,errovfout1_DR("errovfout1_DR")
        ,errovfout0_DR("errovfout0_DR")
        ,afeerrout_DR("afeerrout_DR")
        ,dederrout_DR("dederrout_DR")
        ,sederrout_DR("sederrout_DR")
        // Key open
        ,keyopen("keyopen")
{//{{{ 
    // New instance and port
    std::string port_name;
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("eccdisable_de", i);
        eccdisable_de[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(eccdisable_de[i] != NULL);
        eccdisable_de[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("secdisable_de", i);
        secdisable_de[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(secdisable_de[i] != NULL);
        secdisable_de[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("eccdisable_da", i);
        eccdisable_da[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(eccdisable_da[i] != NULL);
        eccdisable_da[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("secdisable_da", i);
        secdisable_da[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(secdisable_da[i] != NULL);
        secdisable_da[i]->initialize(false); 
    }

    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("errovfout1_DE", i);
        errovfout1_DE[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout1_DE[i] != NULL);
        errovfout1_DE[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("errovfout0_DE", i);
        errovfout0_DE[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout0_DE[i] != NULL);
        errovfout0_DE[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("dederrout_DE", i);
        dederrout_DE[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(dederrout_DE[i] != NULL);
        dederrout_DE[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("sederrout_DE", i);
        sederrout_DE[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(sederrout_DE[i] != NULL);
        sederrout_DE[i]->initialize(false); 
    }

    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("errovfout1_DA", i);
        errovfout1_DA[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout1_DA[i] != NULL);
        errovfout1_DA[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("errovfout0_DA", i);
        errovfout0_DA[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(errovfout0_DA[i] != NULL);
        errovfout0_DA[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("dederrout_DA", i);
        dederrout_DA[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(dederrout_DA[i] != NULL);
        dederrout_DA[i]->initialize(false); 
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        port_name = CombineStringUint("sederrout_DA", i);
        sederrout_DA[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(sederrout_DA[i] != NULL);
        sederrout_DA[i]->initialize(false); 
    }

    mpECCCNT_DTSFunc = new ECCCNT_DTS_Func((std::string)name, this);
    mpMECCCAP_DTSFunc = new MECCCAP_DTS_Func((std::string)name, this);
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        mpECCCNT_DMDEFunc[i] = new ECCCNT_DMDEn_Func((std::string)name, i, this);
        mpMECCCAP_DMDEFunc[i] = new MECCCAP_DMDEn_Func((std::string)name, i, this);
        mpECCCNT_DMDAFunc[i] = new ECCCNT_DMDAn_Func((std::string)name, i, this);
        mpMECCCAP_DMDAFunc[i] = new MECCCAP_DMDAn_Func((std::string)name, i, this);
    }
    
    // Configure Slave socket
    BusSlaveBase<32, emNumTgt>::setSlaveResetPort32(&presetn, &presetn, &presetn
                                                      , &presetn, &presetn, &presetn
                                                      , &presetn, &presetn, &presetn
                                                      , &presetn);
    BusSlaveBase<32, emNumTgt>::setSlaveFreqPort32(&pclk, &pclk, &pclk
                                                     , &pclk, &pclk, &pclk
                                                     , &pclk, &pclk, &pclk
                                                     , &pclk);
    BusSlaveBase<32, emNumTgt>::setTargetSocket32((char*)"ts_ecccnt_dr", (char*)"ts_ecccnt_de_0", (char*)"ts_ecccnt_de_1"
                                                    , (char*)"ts_ecccnt_da_0", (char*)"ts_ecccnt_da_1", (char*)"ts_mecccap_dr"
                                                    , (char*)"ts_mecccap_de_0", (char*)"ts_mecccap_de_1", (char*)"ts_mecccap_da_0"
                                                    , (char*)"ts_mecccap_da_1");


    ts_ecccnt_dr = BusSlaveBase<32, emNumTgt>::tSocket32[0];
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[0]->setFuncModulePtr(mpECCCNT_DTSFunc);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[0]->setWriteLatency(wLatency);
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        ts_ecccnt_de[i] = BusSlaveBase<32, emNumTgt>::tSocket32[i+1];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i + 1]->setFuncModulePtr(mpECCCNT_DMDEFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i + 1]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i + 1]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i + 1]->setWriteLatency(wLatency);
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        ts_ecccnt_da[i] = BusSlaveBase<32, emNumTgt>::tSocket32[nSubDMADTS + i + 1];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS + i + 1]->setFuncModulePtr(mpECCCNT_DMDAFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS + i + 1]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS + i + 1]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS + i + 1]->setWriteLatency(wLatency);
    }

    ts_mecccap_dr = BusSlaveBase<32, emNumTgt>::tSocket32[nSubDMADTS*2 + 1];
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + 1]->setFuncModulePtr(mpMECCCAP_DTSFunc);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + 1]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + 1]->setReadLatency(rLatency);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + 1]->setWriteLatency(wLatency);
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        ts_mecccap_de[i] = BusSlaveBase<32, emNumTgt>::tSocket32[nSubDMADTS*2 + i + 2];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + i + 2]->setFuncModulePtr(mpMECCCAP_DMDEFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + i + 2]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + i + 2]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*2 + i + 2]->setWriteLatency(wLatency);
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        ts_mecccap_da[i] = BusSlaveBase<32, emNumTgt>::tSocket32[nSubDMADTS*3 + i + 2];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*3 + i + 2]->setFuncModulePtr(mpMECCCAP_DMDAFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*3 + i + 2]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*3 + i + 2]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[nSubDMADTS*3 + i + 2]->setWriteLatency(wLatency);
    }


    // Initialize remain output ports
    eccdisable_dr.initialize(false);
    secdisable_dr.initialize(false);
    afedisable_dr.initialize(false);
    errovfout1_DR.initialize(false);
    errovfout0_DR.initialize(false);
    afeerrout_DR.initialize(false);
    dederrout_DR.initialize(false);
    sederrout_DR.initialize(false);

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

    mAWIDTH_MSB_DR = DEFAULT_AWIDTH_MSB_DR;
    mAWIDTH_LSB_DR = DEFAULT_AWIDTH_LSB_DR;
    mCAP_DEPTH_DR  = DEFAULT_CAP_DEPTH_DR;
    mERRINT_INIT_DR= DEFAULT_ERRINT_INIT_DR;

    mAWIDTH_MSB_DE = DEFAULT_AWIDTH_MSB_DE;
    mAWIDTH_LSB_DE = DEFAULT_AWIDTH_LSB_DE;
    mCAP_DEPTH_DE  = DEFAULT_CAP_DEPTH_DE;
    mERRINT_INIT_DE= DEFAULT_ERRINT_INIT_DE;
    
    mAWIDTH_MSB_DA = DEFAULT_AWIDTH_MSB_DA;
    mAWIDTH_LSB_DA = DEFAULT_AWIDTH_LSB_DA;
    mCAP_DEPTH_DA  = DEFAULT_CAP_DEPTH_DA;
    mERRINT_INIT_DA= DEFAULT_ERRINT_INIT_DA;

    mDumpInterrupt = false;
    
    mIsEDLConfig        = false;
    mIsAddrConfig       = false;
    mErrorType          = emNoneError;
    mErrorEDL           = 0;
    mErrorAddress       = 0;
    mCurConfigSubName   = "";
    mSubId               = 0;
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
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(MonitorkeyopenMethod);
    dont_initialize();
    sensitive << keyopen;

    SC_METHOD(WriteDisPort_DRMethod)
    dont_initialize();
    sensitive << mWriteDisPort_DREvent;

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDisPort_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteDisPort_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteDisPort_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDisPort_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteDisPort_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteDisPort_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT0_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteERROVFOUT0_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT0_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT1_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteERROVFOUT1_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT1_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT0_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateERROVFOUT0_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT0_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT1_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateERROVFOUT1_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT1_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteSEDERROUT_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteSEDERROUT_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteSEDERROUT_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertSEDERROUT_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::AssertSEDERROUT_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("AssertSEDERROUT_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateSEDERROUT_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateSEDERROUT_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateSEDERROUT_DEMethod"), &opt);
    }
    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateDEDERROUT_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateDEDERROUT_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateDEDERROUT_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDEDERROUT_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteDEDERROUT_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteDEDERROUT_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcess1bitError_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::Process1bitError_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("Process1bitError_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessFatalError_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::ProcessFatalError_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("ProcessFatalError_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSetFatalErrorStatus_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::SetFatalErrorStatus_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("SetFatalErrorStatus_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNotifyFatalError_DEEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NotifyFatalError_DEMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NotifyFatalError_DEMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT0_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteERROVFOUT0_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT0_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteERROVFOUT1_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteERROVFOUT1_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteERROVFOUT1_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT0_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateERROVFOUT0_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT0_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateERROVFOUT1_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateERROVFOUT1_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateERROVFOUT1_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteSEDERROUT_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteSEDERROUT_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteSEDERROUT_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertSEDERROUT_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::AssertSEDERROUT_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("AssertSEDERROUT_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateSEDERROUT_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateSEDERROUT_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateSEDERROUT_DAMethod"), &opt);
    }
    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNegateDEDERROUT_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NegateDEDERROUT_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NegateDEDERROUT_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDEDERROUT_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::WriteDEDERROUT_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("WriteDEDERROUT_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcess1bitError_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::Process1bitError_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("Process1bitError_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessFatalError_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::ProcessFatalError_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("ProcessFatalError_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSetFatalErrorStatus_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::SetFatalErrorStatus_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("SetFatalErrorStatus_DAMethod"), &opt);
    }

    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNotifyFatalError_DAEvent[sub_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_DMADTS_RAM_E2::NotifyFatalError_DAMethod, this, sub_id),
                          sc_core::sc_gen_unique_name("NotifyFatalError_DAMethod"), &opt);
    }

    SC_METHOD(WriteERROVFOUT0_DRMethod);
    dont_initialize();
    sensitive << mWriteERROVFOUT0_DREvent;

    SC_METHOD(WriteERROVFOUT1_DRMethod);
    dont_initialize();
    sensitive << mWriteERROVFOUT1_DREvent;

    SC_METHOD(NegateERROVFOUT0_DRMethod);
    dont_initialize();
    sensitive << mNegateERROVFOUT0_DREvent;

    SC_METHOD(NegateERROVFOUT1_DRMethod);
    dont_initialize();
    sensitive << mNegateERROVFOUT1_DREvent;

    SC_METHOD(WriteSEDERROUT_DRMethod);
    dont_initialize();
    sensitive << mWriteSEDERROUT_DREvent;

    SC_METHOD(AssertSEDERROUT_DRMethod);
    dont_initialize();
    sensitive << mAssertSEDERROUT_DREvent;

    SC_METHOD(NegateSEDERROUT_DRMethod);
    dont_initialize();
    sensitive << mNegateSEDERROUT_DREvent;

    SC_METHOD(Process1bitError_DRMethod);
    dont_initialize();
    sensitive << mProcess1bitError_DREvent;

    SC_METHOD(ProcessFatalError_DRMethod);
    dont_initialize();
    sensitive << mProcessFatalError_DREvent;

    SC_METHOD(SetFatalErrorStatus_DRMethod);
    dont_initialize();
    sensitive << mSetFatalErrorStatus_DREvent;

    SC_METHOD(NotifyFatalError_DRMethod);
    dont_initialize();
    sensitive << mNotifyFatalError_DREvent;

    SC_METHOD(NegateDEDERROUT_DRMethod);
    dont_initialize();
    sensitive << mNegateDEDERROUT_DREvent;

    SC_METHOD(NegateAFEERROUT_DRMethod);
    dont_initialize();
    sensitive << mNegateAFEERROUT_DREvent;

    SC_METHOD(WriteDEDERROUT_DRMethod);
    dont_initialize();
    sensitive << mWriteDEDERROUT_DREvent;

    SC_METHOD(WriteAFEERROUT_DRMethod);
    dont_initialize();
    sensitive << mWriteAFEERROUT_DREvent;

    SC_THREAD(Set1bitErrorStatusThread);
    dont_initialize();
    sensitive << mSet1bitErrorStatusEvent;
    sensitive << mResetHardEvent[emsys_resetzId];
    sensitive << mResetCmdEvent[emsys_resetzId];
    sensitive << mClkZeroEvent[emsys_clkId];
    sensitive << mClkZeroEvent[empclkId];
}//}}} 

/// Destructor

ECC_DMADTS_RAM_E2::~ECC_DMADTS_RAM_E2()
{//{{{
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        delete mpECCCNT_DMDEFunc[i];
        delete mpMECCCAP_DMDEFunc[i];
        delete mpECCCNT_DMDAFunc[i];
        delete mpMECCCAP_DMDAFunc[i];
    }
    delete mpECCCNT_DTSFunc;
    delete mpMECCCAP_DTSFunc;

    for (unsigned int i = 0; i < nSubDMADTS; i++){
        if(eccdisable_de[i] != NULL){
            delete eccdisable_de[i];
        }
        if(secdisable_de[i] != NULL){
            delete secdisable_de[i];
        }
        if(eccdisable_da[i] != NULL){
            delete eccdisable_da[i];
        }
        if(secdisable_da[i] != NULL){
            delete secdisable_da[i];
        }
        if(errovfout1_DE[i] != NULL){
            delete errovfout1_DE[i];
        }
        if(errovfout0_DE[i] != NULL){
            delete errovfout0_DE[i];
        }
        if(dederrout_DE[i] != NULL){
            delete dederrout_DE[i];
        }
        if(sederrout_DE[i] != NULL){
            delete sederrout_DE[i];
        }
        if(errovfout1_DA[i] != NULL){
            delete errovfout1_DA[i];
        }
        if(errovfout0_DA[i] != NULL){
            delete errovfout0_DA[i];
        }
        if(dederrout_DA[i] != NULL){
            delete dederrout_DA[i];
        }
        if(sederrout_DA[i] != NULL){
            delete sederrout_DA[i];
        }
    }
}//}}}

/// Create DMADTS_RAM EDL Map

void ECC_DMADTS_RAM_E2::CreateEDLMap()
{//{{{
    std::string edl_name;
    // Create EDL map for DR
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        edl_name = CombineStringUint("DR", i);
        mEDLmap_DR.insert(std::pair<std::string, unsigned int>(edl_name, 0x0));
    }
    // Create EDL map for DE
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        edl_name = CombineStringUint("DE", i);
        mEDLmap_DE.insert(std::pair<std::string, unsigned int>(edl_name, 0x0));
    }
    // Create EDL map for DA
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        edl_name = CombineStringUint("DA", i);
        mEDLmap_DA.insert(std::pair<std::string, unsigned int>(edl_name, 0x0));
    }
}//}}}

/// Initialize when reset activated

void ECC_DMADTS_RAM_E2::Initialize(void)
{//{{{
    unsigned int dreccctl  = mpECCCNT_DTSFunc->GetDRECCCTLReg();
    mECCDIS_DR = (bool)(dreccctl & 0x1);
    mSECDIS_DR = (bool)((dreccctl >> 1) & 0x1);
    mAFEDIS_DR = (bool)((dreccctl >> 3) & 0x1);
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        unsigned int deeccctl  = mpECCCNT_DMDEFunc[i]->GetDEECCCTLReg();
        mECCDIS_DE[i] = (bool)(deeccctl & 0x1);
        mSECDIS_DE[i] = (bool)((deeccctl >> 1) & 0x1);
        unsigned int daeccctl  = mpECCCNT_DMDAFunc[i]->GetDAECCCTLReg();
        mECCDIS_DA[i] = (bool)(daeccctl & 0x1);
        mSECDIS_DA[i] = (bool)((daeccctl >> 1) & 0x1);
    }

    mERROVFOUT0_DR      = false;
    mERROVFOUT1_DR      = false;
    mSEDERROUT_DR       = false;
    mDEDERROUT_DR       = false;
    mAFEERROUT_DR       = false;
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        mERROVFOUT0_DE[i]  = false;
        mERROVFOUT1_DE[i]  = false;
        mSEDERROUT_DE[i]   = false;
        mDEDERROUT_DE[i]   = false;
        mERROVFOUT0_DA[i]  = false;
        mERROVFOUT1_DA[i]  = false;
        mSEDERROUT_DA[i]   = false;
        mDEDERROUT_DA[i]   = false;
    }
    
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        mHas2bitError_DE[i]         = false;
        mFatalErrorModAddr_DE[i]    = 0;
        mFatalEDLCode_DE[i]         = 0;
        mSize1bitErrorList_DE[i]    = 0;
        mSizeFatalErrorList_DE[i]   = 0;
        while (!m1bitErrorFIFO_DE[i].empty()){
            m1bitErrorFIFO_DE[i].pop();
        }
        mHas2bitError_DA[i]         = false;
        mFatalErrorModAddr_DA[i]    = 0;
        mFatalEDLCode_DA[i]         = 0;
        mSize1bitErrorList_DA[i]    = 0;
        mSizeFatalErrorList_DA[i]   = 0;
        while (!m1bitErrorFIFO_DA[i].empty()){
            m1bitErrorFIFO_DA[i].pop();
        }
    }
    mHas2bitError_DR                = false;
    mHasAddrFeedbackError_DR        = false;
    mFatalErrorModAddr_DR           = 0;
    mFatalEDLCode_DR                = 0;
    mSize1bitErrorList_DR           = 0;
    mSizeFatalErrorList_DR          = 0;
    while (!m1bitErrorFIFO_DR.empty()){
        m1bitErrorFIFO_DR.pop();
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Prepare to write prohibition port of ECCCNT DTS

void ECC_DMADTS_RAM_E2::WriteDisPort_DR(unsigned int value)
{//{{{
    mECCDIS_DR = (bool)(value & 0x1);
    mSECDIS_DR = (bool)((value >> 1) & 0x1);
    mAFEDIS_DR = (bool)((value >> 3) & 0x1);
    mWriteDisPort_DREvent.cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Prepare to write prohibition port of ECCCNT DMDE

void ECC_DMADTS_RAM_E2::WriteDisPort_DE(unsigned int sub_id, unsigned int value)
{//{{{
    mECCDIS_DE[sub_id] = (bool)(value & 0x1);
    mSECDIS_DE[sub_id] = (bool)((value >> 1) & 0x1);
    mWriteDisPort_DEEvent[sub_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_DEEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Prepare to write prohibition port of ECCCNT DMDA

void ECC_DMADTS_RAM_E2::WriteDisPort_DA(unsigned int sub_id, unsigned int value)
{//{{{
    mECCDIS_DA[sub_id] = (bool)(value & 0x1);
    mSECDIS_DA[sub_id] = (bool)((value >> 1) & 0x1);
    mWriteDisPort_DAEvent[sub_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_DAEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get CAP DEPTH

unsigned int ECC_DMADTS_RAM_E2::GetCapDepth(std::string sub_name)
{//{{{
    sc_assert((sub_name == "DR") || (sub_name == "DE") || (sub_name == "DA"));
    unsigned int ret = 0;

    if (sub_name == "DR"){
        ret = mCAP_DEPTH_DR;
    }else if (sub_name == "DE"){
        ret = mCAP_DEPTH_DE;
    }else{//DA
        ret = mCAP_DEPTH_DA;
    }
    return ret;
}//}}}

/// Assert errovfout0_DE port

void ECC_DMADTS_RAM_E2::AssertERROVFOUT0_DE(unsigned int sub_id)
{//{{{
    mERROVFOUT0_DE[sub_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_DEEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_DE port

void ECC_DMADTS_RAM_E2::AssertERROVFOUT1_DE(unsigned int sub_id)
{//{{{
    mERROVFOUT1_DE[sub_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_DEEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout0_DA port

void ECC_DMADTS_RAM_E2::AssertERROVFOUT0_DA(unsigned int sub_id)
{//{{{
    mERROVFOUT0_DA[sub_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_DAEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_DA port

void ECC_DMADTS_RAM_E2::AssertERROVFOUT1_DA(unsigned int sub_id)
{//{{{
    mERROVFOUT1_DA[sub_id] = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_DAEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout0_DR port

void ECC_DMADTS_RAM_E2::AssertERROVFOUT0_DR()
{//{{{
    mERROVFOUT0_DR = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_DR port

void ECC_DMADTS_RAM_E2::AssertERROVFOUT1_DR()
{//{{{
    mERROVFOUT1_DR = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get keycode value

bool ECC_DMADTS_RAM_E2::GetKeyOpen()
{//{{{
    return mKeyOpen;
}//}}}
//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock

void ECC_DMADTS_RAM_E2::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor sys_clk clock

void ECC_DMADTS_RAM_E2::Monitorsys_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

/// Monitor presetn port

void ECC_DMADTS_RAM_E2::MonitorpresetnMethod()
{//{{{
    mResetCurVal[empresetnId] = presetn.read();
    if (mResetCurVal[empresetnId] != mResetPreVal[empresetnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[empresetnId] = mResetCurVal[empresetnId];
        // Reset signals are asynchronous
        mResetHardEvent[empresetnId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor presetn port

void ECC_DMADTS_RAM_E2::Monitorsys_resetzMethod()
{//{{{
    mResetCurVal[emsys_resetzId] = sys_resetz.read();
    if (mResetCurVal[emsys_resetzId] != mResetPreVal[emsys_resetzId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emsys_resetzId] = mResetCurVal[emsys_resetzId];
        // Reset signals are asynchronous
        mResetHardEvent[emsys_resetzId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Process reset function when reset port is active

void ECC_DMADTS_RAM_E2::HandleResetHardMethod (const unsigned int reset_id)
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

void ECC_DMADTS_RAM_E2::HandleResetCmdMethod (const unsigned int reset_id)
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

void ECC_DMADTS_RAM_E2::CancelResetCmdMethod (const unsigned int reset_id)
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

void ECC_DMADTS_RAM_E2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    // Write ERRINT_INIT to ERRINT reg after reset
    if (reset_name=="sys_resetz" && is_active){
        for (unsigned int i = 0; i < nSubDMADTS; i++){
            mpMECCCAP_DMDEFunc[i]->WriteERRINT_INIT(mERRINT_INIT_DE);
            mpMECCCAP_DMDAFunc[i]->WriteERRINT_INIT(mERRINT_INIT_DA);
        }
        mpMECCCAP_DTSFunc->WriteERRINT_INIT(mERRINT_INIT_DR);
    }
    for (unsigned int i = 0; i < nSubDMADTS; i++) {
        (mpECCCNT_DMDEFunc[i])->EnableReset(reset_name, is_active);
        (mpMECCCAP_DMDEFunc[i])->EnableReset(reset_name, is_active);
        (mpECCCNT_DMDAFunc[i])->EnableReset(reset_name, is_active);
        (mpMECCCAP_DMDAFunc[i])->EnableReset(reset_name, is_active);
    }
    mpECCCNT_DTSFunc->EnableReset(reset_name, is_active);
    mpMECCCAP_DTSFunc->EnableReset(reset_name, is_active);
    if(reset_name=="sys_resetz" && is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        for (unsigned int i = 0; i < nSubDMADTS; i++){
            mWriteDisPort_DEEvent[i].notify();
            mWriteERROVFOUT0_DEEvent[i].notify();
            mWriteERROVFOUT1_DEEvent[i].notify();
            mWriteSEDERROUT_DEEvent[i].notify();
            mWriteDEDERROUT_DEEvent[i].notify();

            mWriteDisPort_DAEvent[i].notify();
            mWriteERROVFOUT0_DAEvent[i].notify();
            mWriteERROVFOUT1_DAEvent[i].notify();
            mWriteSEDERROUT_DAEvent[i].notify();
            mWriteDEDERROUT_DAEvent[i].notify();
        }
        mWriteDisPort_DREvent.notify();
        mWriteERROVFOUT0_DREvent.notify();
        mWriteERROVFOUT1_DREvent.notify();
        mWriteSEDERROUT_DREvent.notify();
        mWriteDEDERROUT_DREvent.notify();
        mWriteAFEERROUT_DREvent.notify();
    }
}//}}}

/// Cancel events

void ECC_DMADTS_RAM_E2::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        // Cancel event related to DE
        mWriteDisPort_DEEvent[i].cancel();
        mWriteERROVFOUT0_DEEvent[i].cancel();
        mWriteERROVFOUT1_DEEvent[i].cancel();
        mNegateERROVFOUT0_DEEvent[i].cancel();
        mNegateERROVFOUT1_DEEvent[i].cancel();
        mWriteSEDERROUT_DEEvent[i].cancel();
        mAssertSEDERROUT_DEEvent[i].cancel();
        mNegateSEDERROUT_DEEvent[i].cancel();
        mNegateDEDERROUT_DEEvent[i].cancel();
        mWriteDEDERROUT_DEEvent[i].cancel();
        mProcess1bitError_DEEvent[i].cancel();
        mProcessFatalError_DEEvent[i].cancel();
        mSetFatalErrorStatus_DEEvent[i].cancel();
        mNotifyFatalError_DEEvent[i].cancel();

        // Cancel event related to DA
        mWriteDisPort_DAEvent[i].cancel();
        mWriteERROVFOUT0_DAEvent[i].cancel();
        mWriteERROVFOUT1_DAEvent[i].cancel();
        mNegateERROVFOUT0_DAEvent[i].cancel();
        mNegateERROVFOUT1_DAEvent[i].cancel();
        mWriteSEDERROUT_DAEvent[i].cancel();
        mAssertSEDERROUT_DAEvent[i].cancel();
        mNegateSEDERROUT_DAEvent[i].cancel();
        mNegateDEDERROUT_DAEvent[i].cancel();
        mWriteDEDERROUT_DAEvent[i].cancel();
        mProcess1bitError_DAEvent[i].cancel();
        mProcessFatalError_DAEvent[i].cancel();
        mSetFatalErrorStatus_DAEvent[i].cancel();
        mNotifyFatalError_DAEvent[i].cancel();
    }
    // Cancel event related to DR
    mWriteDisPort_DREvent.cancel();
    mWriteERROVFOUT0_DREvent.cancel();
    mWriteERROVFOUT1_DREvent.cancel();
    mNegateERROVFOUT0_DREvent.cancel();
    mNegateERROVFOUT1_DREvent.cancel();
    mWriteSEDERROUT_DREvent.cancel();
    mAssertSEDERROUT_DREvent.cancel();
    mNegateSEDERROUT_DREvent.cancel();
    mNegateDEDERROUT_DREvent.cancel();
    mNegateAFEERROUT_DREvent.cancel();
    mWriteDEDERROUT_DREvent.cancel();
    mWriteAFEERROUT_DREvent.cancel();
    mProcess1bitError_DREvent.cancel();
    mProcessFatalError_DREvent.cancel();
    mSetFatalErrorStatus_DREvent.cancel();
    mNotifyFatalError_DREvent.cancel();

    mSet1bitErrorStatusEvent.cancel();
}//}}}

/// Get reset status

bool ECC_DMADTS_RAM_E2::GetResetStatus (const std::string reset_name)
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

bool ECC_DMADTS_RAM_E2::CheckClockPeriod (const std::string clock_name)
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

void ECC_DMADTS_RAM_E2::MonitorkeyopenMethod()
{//{{{
    mKeyOpen = keyopen.read();
}//}}}

/// Write prohibition ports of DMDE

void ECC_DMADTS_RAM_E2::WriteDisPort_DRMethod()
{//{{{ 
    eccdisable_dr.write(mECCDIS_DR);
    secdisable_dr.write(mSECDIS_DR);
    afedisable_dr.write(mAFEDIS_DR);
}//}}}

/// Write prohibition ports of DMDE

void ECC_DMADTS_RAM_E2::WriteDisPort_DEMethod(unsigned int sub_id)
{//{{{ 
    eccdisable_de[sub_id]->write(mECCDIS_DE[sub_id]);
    secdisable_de[sub_id]->write(mSECDIS_DE[sub_id]);
}//}}}

/// Write errovfout0_DE port

void ECC_DMADTS_RAM_E2::WriteERROVFOUT0_DEMethod(unsigned int sub_id)
{//{{{
    errovfout0_DE[sub_id]->write(mERROVFOUT0_DE[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_DE[%d] is changed to %d\n", sub_id, (unsigned int)mERROVFOUT0_DE[sub_id]);
    }
    // Negate if any
    if (mERROVFOUT0_DE[sub_id]){
        mNegateERROVFOUT0_DEEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_DE port

void ECC_DMADTS_RAM_E2::WriteERROVFOUT1_DEMethod(unsigned int sub_id)
{//{{{
    errovfout1_DE[sub_id]->write(mERROVFOUT1_DE[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_DE[%d] is changed to %d\n", sub_id, (unsigned int)mERROVFOUT1_DE[sub_id]);
    }
    // Negate if any
    if (mERROVFOUT1_DE[sub_id]){
        mNegateERROVFOUT1_DEEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout0_DE port

void ECC_DMADTS_RAM_E2::NegateERROVFOUT0_DEMethod(unsigned int sub_id)
{//{{{
    mERROVFOUT0_DE[sub_id] = false;
    mWriteERROVFOUT0_DEEvent[sub_id].notify();
}//}}}

/// Negate errovfout1_DE port

void ECC_DMADTS_RAM_E2::NegateERROVFOUT1_DEMethod(unsigned int sub_id)
{//{{{
    mERROVFOUT1_DE[sub_id] = false;
    mWriteERROVFOUT1_DEEvent[sub_id].notify();
}//}}}

/// Write sederrout_DE port

void ECC_DMADTS_RAM_E2::WriteSEDERROUT_DEMethod(unsigned int sub_id)
{//{{{
    sederrout_DE[sub_id]->write(mSEDERROUT_DE[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_DE[%d] is changed to %d\n", sub_id, (unsigned int)mSEDERROUT_DE[sub_id]);
    }
    // Negate if any
    if (mSEDERROUT_DE[sub_id]){
        mNegateSEDERROUT_DEEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)

void ECC_DMADTS_RAM_E2::AssertSEDERROUT_DEMethod(unsigned int sub_id)
{//{{{
    unsigned int sedie = mpMECCCAP_DMDEFunc[sub_id]->GetERRINT() & 0x1;
    if (sedie == 1){
        mSEDERROUT_DE[sub_id] = true;
        mWriteSEDERROUT_DEEvent[sub_id].notify();
    }
}//}}}

/// Negate sederrout_DE port

void ECC_DMADTS_RAM_E2::NegateSEDERROUT_DEMethod(unsigned int sub_id)
{//{{{
    mSEDERROUT_DE[sub_id] = false;
    mWriteSEDERROUT_DEEvent[sub_id].notify();
}//}}}

/// Negate dederrout_DE port

void ECC_DMADTS_RAM_E2::NegateDEDERROUT_DEMethod(unsigned int sub_id)
{//{{{
    mDEDERROUT_DE[sub_id] = false;
    mWriteDEDERROUT_DEEvent[sub_id].notify();
}//}}}

/// Write dederrout_DE port

void ECC_DMADTS_RAM_E2::WriteDEDERROUT_DEMethod(unsigned int sub_id)
{//{{{
    dederrout_DE[sub_id]->write(mDEDERROUT_DE[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_DE[%d] is changed to %d\n", sub_id, (unsigned int)mDEDERROUT_DE[sub_id]);
    }
    if (mDEDERROUT_DE[sub_id]){
        mNegateDEDERROUT_DEEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write prohibition ports of DMDA

void ECC_DMADTS_RAM_E2::WriteDisPort_DAMethod(unsigned int sub_id)
{//{{{ 
    eccdisable_da[sub_id]->write(mECCDIS_DA[sub_id]);
    secdisable_da[sub_id]->write(mSECDIS_DA[sub_id]);
}//}}}

/// Write errovfout0_DA port

void ECC_DMADTS_RAM_E2::WriteERROVFOUT0_DAMethod(unsigned int sub_id)
{//{{{
    errovfout0_DA[sub_id]->write(mERROVFOUT0_DA[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_DA[%d] is changed to %d\n", sub_id, (unsigned int)mERROVFOUT0_DA[sub_id]);
    }
    // Negate if any
    if (mERROVFOUT0_DA[sub_id]){
        mNegateERROVFOUT0_DAEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_DA port

void ECC_DMADTS_RAM_E2::WriteERROVFOUT1_DAMethod(unsigned int sub_id)
{//{{{
    errovfout1_DA[sub_id]->write(mERROVFOUT1_DA[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_DA[%d] is changed to %d\n", sub_id, (unsigned int)mERROVFOUT1_DA[sub_id]);
    }
    // Negate if any
    if (mERROVFOUT1_DA[sub_id]){
        mNegateERROVFOUT1_DAEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout0_DA port

void ECC_DMADTS_RAM_E2::NegateERROVFOUT0_DAMethod(unsigned int sub_id)
{//{{{
    mERROVFOUT0_DA[sub_id] = false;
    mWriteERROVFOUT0_DAEvent[sub_id].notify();
}//}}}

/// Negate errovfout1_DA port

void ECC_DMADTS_RAM_E2::NegateERROVFOUT1_DAMethod(unsigned int sub_id)
{//{{{
    mERROVFOUT1_DA[sub_id] = false;
    mWriteERROVFOUT1_DAEvent[sub_id].notify();
}//}}}

/// Write sederrout_DA port

void ECC_DMADTS_RAM_E2::WriteSEDERROUT_DAMethod(unsigned int sub_id)
{//{{{
    sederrout_DA[sub_id]->write(mSEDERROUT_DA[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_DA[%d] is changed to %d\n", sub_id, (unsigned int)mSEDERROUT_DA[sub_id]);
    }
    // Negate if any
    if (mSEDERROUT_DA[sub_id]){
        mNegateSEDERROUT_DAEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)

void ECC_DMADTS_RAM_E2::AssertSEDERROUT_DAMethod(unsigned int sub_id)
{//{{{
    unsigned int sedie = mpMECCCAP_DMDAFunc[sub_id]->GetERRINT() & 0x1;
    if (sedie == 1){
        mSEDERROUT_DA[sub_id] = true;
        mWriteSEDERROUT_DAEvent[sub_id].notify();
    }
}//}}}

/// Negate sederrout_DA port

void ECC_DMADTS_RAM_E2::NegateSEDERROUT_DAMethod(unsigned int sub_id)
{//{{{
    mSEDERROUT_DA[sub_id] = false;
    mWriteSEDERROUT_DAEvent[sub_id].notify();
}//}}}

/// Negate dederrout_DA port

void ECC_DMADTS_RAM_E2::NegateDEDERROUT_DAMethod(unsigned int sub_id)
{//{{{
    mDEDERROUT_DA[sub_id] = false;
    mWriteDEDERROUT_DAEvent[sub_id].notify();
}//}}}

/// Write dederrout_DA port

void ECC_DMADTS_RAM_E2::WriteDEDERROUT_DAMethod(unsigned int sub_id)
{//{{{
    dederrout_DA[sub_id]->write(mDEDERROUT_DA[sub_id]);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_DA[%d] is changed to %d\n", sub_id, (unsigned int)mDEDERROUT_DA[sub_id]);
    }
    if (mDEDERROUT_DA[sub_id]){
        mNegateDEDERROUT_DAEvent[sub_id].notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit for DE

void ECC_DMADTS_RAM_E2::Process1bitError_DEMethod(unsigned int sub_id)
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_DE; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList_DE[sub_id]; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList_DE[sub_id][i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO_DE[sub_id].size() < emTotalSlavePorts_DE) {// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO_DE[sub_id].push(m1bitErrorList_DE[sub_id][i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_DMDEFunc[sub_id]->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList_DE[sub_id]; i++){
        unsigned int mod_address = ShiftAddress("DE", m1bitErrorList_DE[sub_id][i].address);
        if (Check1bitAddressNotCaptured_DE(sub_id, mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_DEEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList_DE[sub_id] = 0;
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit for DA

void ECC_DMADTS_RAM_E2::Process1bitError_DAMethod(unsigned int sub_id)
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_DA; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList_DA[sub_id]; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList_DA[sub_id][i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO_DA[sub_id].size() < emTotalSlavePorts_DA) {// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO_DA[sub_id].push(m1bitErrorList_DA[sub_id][i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_DMDAFunc[sub_id]->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList_DA[sub_id]; i++){
        unsigned int mod_address = ShiftAddress("DA",m1bitErrorList_DA[sub_id][i].address);
        if (Check1bitAddressNotCaptured_DA(sub_id, mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_DAEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList_DA[sub_id] = 0;
}//}}}

/// Write errovfout0_DR port

void ECC_DMADTS_RAM_E2::WriteERROVFOUT0_DRMethod()
{//{{{
    errovfout0_DR.write(mERROVFOUT0_DR);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_DR is changed to %d\n", (unsigned int)mERROVFOUT0_DR);
    }
    // Negate if any
    if (mERROVFOUT0_DR){
        mNegateERROVFOUT0_DREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_DR port

void ECC_DMADTS_RAM_E2::WriteERROVFOUT1_DRMethod()
{//{{{
    errovfout1_DR.write(mERROVFOUT1_DR);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_DR is changed to %d\n", (unsigned int)mERROVFOUT1_DR);
    }
    // Negate if any
    if (mERROVFOUT1_DR){
        mNegateERROVFOUT1_DREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Negate errovfout0_DR port

void ECC_DMADTS_RAM_E2::NegateERROVFOUT0_DRMethod()
{//{{{
    mERROVFOUT0_DR = false;
    mWriteERROVFOUT0_DREvent.notify();
}//}}}

/// Negate errovfout1_DR port

void ECC_DMADTS_RAM_E2::NegateERROVFOUT1_DRMethod()
{//{{{
    mERROVFOUT1_DR = false;
    mWriteERROVFOUT1_DREvent.notify();
}//}}}

/// Write sederrout_DR port

void ECC_DMADTS_RAM_E2::WriteSEDERROUT_DRMethod()
{//{{{
    sederrout_DR.write(mSEDERROUT_DR);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_DR is changed to %d\n", (unsigned int)mSEDERROUT_DR);
    }
    // Negate if any
    if (mSEDERROUT_DR){
        mNegateSEDERROUT_DREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)

void ECC_DMADTS_RAM_E2::AssertSEDERROUT_DRMethod()
{//{{{
    unsigned int sedie = mpMECCCAP_DTSFunc->GetERRINT() & 0x1;
    if (sedie == 1){
        mSEDERROUT_DR = true;
        mWriteSEDERROUT_DREvent.notify();
    }
}//}}}

/// Negate sederrout_DR port

void ECC_DMADTS_RAM_E2::NegateSEDERROUT_DRMethod()
{//{{{
    mSEDERROUT_DR = false;
    mWriteSEDERROUT_DREvent.notify();
}//}}}

/// Negate dederrout_DR port

void ECC_DMADTS_RAM_E2::NegateDEDERROUT_DRMethod()
{//{{{
    mDEDERROUT_DR = false;
    mWriteDEDERROUT_DREvent.notify();
}//}}}

/// Negate afeerrout_DR port

void ECC_DMADTS_RAM_E2::NegateAFEERROUT_DRMethod()
{//{{{
    mAFEERROUT_DR = false;
    mWriteAFEERROUT_DREvent.notify();
}//}}}

/// Write dederrout_DR port

void ECC_DMADTS_RAM_E2::WriteDEDERROUT_DRMethod()
{//{{{
    dederrout_DR.write(mDEDERROUT_DR);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_DR is changed to %d\n", (unsigned int)mDEDERROUT_DR);
    }
    if (mDEDERROUT_DR){
        mNegateDEDERROUT_DREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write afeerrout_DR port

void ECC_DMADTS_RAM_E2::WriteAFEERROUT_DRMethod()
{//{{{
    afeerrout_DR.write(mAFEERROUT_DR);
    if (mDumpInterrupt){
        re_printf("info", "afeerrout_DR is changed to %d\n", (unsigned int)mAFEERROUT_DR);
    }
    if (mAFEERROUT_DR){
        mNegateAFEERROUT_DREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Process 1bit error for both MEMCAP 

void ECC_DMADTS_RAM_E2::Set1bitErrorStatusThread()
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

                    // Process FIFOs of DE
                    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++){
                        if (m1bitErrorFIFO_DE[sub_id].size() != 0){
                            unsigned int mod_address = ShiftAddress("DE", m1bitErrorFIFO_DE[sub_id].front().address);
                            if (mpMECCCAP_DMDEFunc[sub_id]->CheckHasRoomFor1bit()){
                                // then set status bit if there is room
                                unsigned int edl_code = m1bitErrorFIFO_DE[sub_id].front().edl_code;
                                if (Check1bitAddressNotCaptured_DE(sub_id, mod_address)){
                                    mpMECCCAP_DMDEFunc[sub_id]->Set1bitStatusAndAddress(edl_code, mod_address);
                                }
                            } else {
                                //ECC 1bit error was detacted when all flag are set.
                                if (Check1bitAddressNotCaptured_DE(sub_id, mod_address)){
                                    mpMECCCAP_DMDEFunc[sub_id]->SetFlagAddressBufferOverflow();
                                    while (!m1bitErrorFIFO_DE[sub_id].empty()){
                                        m1bitErrorFIFO_DE[sub_id].pop();
                                    }
                                }
                            }
                            //remove FIFO
                            if (!m1bitErrorFIFO_DE[sub_id].empty()){
                                m1bitErrorFIFO_DE[sub_id].pop();
                            }
                        }
                    }

                    // Process FIFOs of DA
                    for (unsigned int sub_id = 0; sub_id < nSubDMADTS; sub_id++){
                        if (m1bitErrorFIFO_DA[sub_id].size() != 0){
                            unsigned int mod_address = ShiftAddress("DA", m1bitErrorFIFO_DA[sub_id].front().address);
                            if (mpMECCCAP_DMDAFunc[sub_id]->CheckHasRoomFor1bit()){
                                // then set status bit if there is room
                                unsigned int edl_code = m1bitErrorFIFO_DA[sub_id].front().edl_code;
                                if (Check1bitAddressNotCaptured_DA(sub_id, mod_address)){
                                    mpMECCCAP_DMDAFunc[sub_id]->Set1bitStatusAndAddress(edl_code, mod_address);
                                }
                            } else {
                                //ECC 1bit error was detacted when all flag are set.
                                if (Check1bitAddressNotCaptured_DA(sub_id, mod_address)){
                                    mpMECCCAP_DMDAFunc[sub_id]->SetFlagAddressBufferOverflow();
                                    while (!m1bitErrorFIFO_DA[sub_id].empty()){
                                        m1bitErrorFIFO_DA[sub_id].pop();
                                    }
                                }
                            }
                            //remove FIFO
                            if (!m1bitErrorFIFO_DA[sub_id].empty()){
                                m1bitErrorFIFO_DA[sub_id].pop();
                            }
                        }
                    }

                    // Process FIFO of DR
                    if (m1bitErrorFIFO_DR.size() != 0){
                        unsigned int mod_address = ShiftAddress("DR", m1bitErrorFIFO_DR.front().address);
                        if (mpMECCCAP_DTSFunc->CheckHasRoomFor1bit()){
                            // then set status bit if there is room
                            unsigned int edl_code = m1bitErrorFIFO_DR.front().edl_code;
                            if (Check1bitAddressNotCaptured_DR(mod_address)){
                                mpMECCCAP_DTSFunc->Set1bitStatusAndAddress(edl_code, mod_address);
                            }
                        } else {
                            //ECC 1bit error was detacted when all flag are set.
                            if (Check1bitAddressNotCaptured_DR(mod_address)){
                                mpMECCCAP_DTSFunc->SetFlagAddressBufferOverflow();
                                while (!m1bitErrorFIFO_DR.empty()){
                                    m1bitErrorFIFO_DR.pop();
                                }
                            }
                        }
                        //remove FIFO
                        if (!m1bitErrorFIFO_DR.empty()){
                            m1bitErrorFIFO_DR.pop();
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

bool ECC_DMADTS_RAM_E2::CheckAnyFIFOHasElement()
{//{{{
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        if (m1bitErrorFIFO_DE[i].size() != 0){
            return true;
        }
        if (m1bitErrorFIFO_DA[i].size() != 0){
            return true;
        }
    }
    if (m1bitErrorFIFO_DR.size() != 0){
        return true;
    }
    return false;
}//}}}

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit for DR

void ECC_DMADTS_RAM_E2::Process1bitError_DRMethod()
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_DR; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList_DR; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList_DR[i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO_DR.size() < emTotalSlavePorts_DR) {// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO_DR.push(m1bitErrorList_DR[i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_DTSFunc->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList_DR; i++){
        unsigned int mod_address = ShiftAddress("DR", m1bitErrorList_DR[i].address);
        if (Check1bitAddressNotCaptured_DR(mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList_DR = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit for DE

void ECC_DMADTS_RAM_E2::ProcessFatalError_DEMethod(unsigned int sub_id)
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError_DE[sub_id] = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_DE; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSizeFatalErrorList_DE[sub_id]; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList_DE[sub_id][i].edl_code == portindex){
                if (CheckNoFatalStatus_DE(sub_id)){
                    mHas2bitError_DE[sub_id] |= (mFatalErrorList_DE[sub_id][i].type == em2bitError);
                    mod_address = ShiftAddress("DE", mFatalErrorList_DE[sub_id][i].address);
                    edl_code = mFatalErrorList_DE[sub_id][i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalEDLCode_DE[sub_id] = edl_code;
        mFatalErrorModAddr_DE[sub_id] = mod_address;
        mSetFatalErrorStatus_DEEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList_DE[sub_id]; i++){
        if (mFatalErrorList_DE[sub_id][i].type == em2bitError){
            mDEDERROUT_DE[sub_id] |= true;
        }
    }
    if (mDEDERROUT_DE[sub_id]){
        mNotifyFatalError_DEEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList_DE[sub_id] = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit for DA

void ECC_DMADTS_RAM_E2::ProcessFatalError_DAMethod(unsigned int sub_id)
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError_DA[sub_id] = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_DA; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSizeFatalErrorList_DA[sub_id]; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList_DA[sub_id][i].edl_code == portindex){
                if (CheckNoFatalStatus_DA(sub_id)){
                    mHas2bitError_DA[sub_id] |= (mFatalErrorList_DA[sub_id][i].type == em2bitError);
                    mod_address = ShiftAddress("DA", mFatalErrorList_DA[sub_id][i].address);
                    edl_code = mFatalErrorList_DA[sub_id][i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalEDLCode_DA[sub_id] = edl_code;
        mFatalErrorModAddr_DA[sub_id] = mod_address;
        mSetFatalErrorStatus_DAEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList_DA[sub_id]; i++){
        if (mFatalErrorList_DA[sub_id][i].type == em2bitError){
            mDEDERROUT_DA[sub_id] |= true;
        }
    }
    if (mDEDERROUT_DA[sub_id]){
        mNotifyFatalError_DAEvent[sub_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList_DA[sub_id] = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit for DR

void ECC_DMADTS_RAM_E2::ProcessFatalError_DRMethod()
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError_DR = false;
    mHasAddrFeedbackError_DR = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX_DR; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSizeFatalErrorList_DR; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList_DR[i].edl_code == portindex){
                if (CheckNoFatalStatus_DR()){
                    mHas2bitError_DR |= (mFatalErrorList_DR[i].type == em2bitError);
                    mHasAddrFeedbackError_DR |= (mFatalErrorList_DR[i].type == emAddressFeedbackError);
                    mod_address = ShiftAddress("DR", mFatalErrorList_DR[i].address);
                    edl_code = mFatalErrorList_DR[i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalEDLCode_DR = edl_code;
        mFatalErrorModAddr_DR = mod_address;
        mSetFatalErrorStatus_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList_DR; i++){
        if (mFatalErrorList_DR[i].type == em2bitError){
            mDEDERROUT_DR |= true;
        }
        if (mFatalErrorList_DR[i].type == emAddressFeedbackError){
            mAFEERROUT_DR |= true;
        }
    }
    if (mDEDERROUT_DR || mAFEERROUT_DR){
        mNotifyFatalError_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList_DR = 0;
}//}}}

/// Set fatal status bit for DE

void ECC_DMADTS_RAM_E2::SetFatalErrorStatus_DEMethod(unsigned int sub_id)
{//{{{
    if (mHas2bitError_DE[sub_id]){
        mpMECCCAP_DMDEFunc[sub_id]->Set2bitErrorStatus();
        mpMECCCAP_DMDEFunc[sub_id]->SetFatalAddress(mFatalEDLCode_DE[sub_id], mFatalErrorModAddr_DE[sub_id]);
    }
}//}}}

/// Set fatal status bit for DA

void ECC_DMADTS_RAM_E2::SetFatalErrorStatus_DAMethod(unsigned int sub_id)
{//{{{
    if (mHas2bitError_DA[sub_id]){
        mpMECCCAP_DMDAFunc[sub_id]->Set2bitErrorStatus();
        mpMECCCAP_DMDAFunc[sub_id]->SetFatalAddress(mFatalEDLCode_DA[sub_id], mFatalErrorModAddr_DA[sub_id]);
    }
}//}}}

/// Set fatal status bit for DR

void ECC_DMADTS_RAM_E2::SetFatalErrorStatus_DRMethod()
{//{{{
    if (mHas2bitError_DR){
        mpMECCCAP_DTSFunc->Set2bitErrorStatus();
        mpMECCCAP_DTSFunc->SetFatalAddress(mFatalEDLCode_DR, mFatalErrorModAddr_DR);
    }
    if (mHasAddrFeedbackError_DR){
        mpMECCCAP_DTSFunc->SetAddrFeedbackErrorStatus();
        mpMECCCAP_DTSFunc->SetFatalAddress(mFatalEDLCode_DR, mFatalErrorModAddr_DR);
    }
}//}}}

/// Notify fatal error for DE

void ECC_DMADTS_RAM_E2::NotifyFatalError_DEMethod(unsigned int sub_id)
{//{{{
    unsigned int dedie = (mpMECCCAP_DMDEFunc[sub_id]->GetERRINT() >> 1) & 0x1;
    if (mDEDERROUT_DE[sub_id] && (dedie == 1)){
        mWriteDEDERROUT_DEEvent[sub_id].notify();
    }else{
        mDEDERROUT_DE[sub_id] = false;
    }
}//}}}

/// Notify fatal error for DA

void ECC_DMADTS_RAM_E2::NotifyFatalError_DAMethod(unsigned int sub_id)
{//{{{
    unsigned int dedie = (mpMECCCAP_DMDAFunc[sub_id]->GetERRINT() >> 1) & 0x1;
    if (mDEDERROUT_DA[sub_id] && (dedie == 1)){
        mWriteDEDERROUT_DAEvent[sub_id].notify();
    }else{
        mDEDERROUT_DA[sub_id] = false;
    }
}//}}}

/// Notify fatal error for DR

void ECC_DMADTS_RAM_E2::NotifyFatalError_DRMethod()
{//{{{
    unsigned int dedie = (mpMECCCAP_DTSFunc->GetERRINT() >> 1) & 0x1;
    unsigned int afeie = (mpMECCCAP_DTSFunc->GetERRINT() >> 3) & 0x1;
    if (mDEDERROUT_DR && (dedie == 1)){
        mWriteDEDERROUT_DREvent.notify();
    }else{
        mDEDERROUT_DR = false;
    }
    if (mAFEERROUT_DR && (afeie == 1)){
        mWriteAFEERROUT_DREvent.notify();
    }else{
        mAFEERROUT_DR = false;
    }
}//}}}

/// Check no fatal status of DE

bool ECC_DMADTS_RAM_E2::CheckNoFatalStatus_DE(unsigned int sub_id)
{//{{{
    return mpMECCCAP_DMDEFunc[sub_id]->CheckNoFatalStatus();
}//}}}

/// Check no fatal status of DA

bool ECC_DMADTS_RAM_E2::CheckNoFatalStatus_DA(unsigned int sub_id)
{//{{{
    return mpMECCCAP_DMDAFunc[sub_id]->CheckNoFatalStatus();
}//}}}

/// Check no fatal status of DR

bool ECC_DMADTS_RAM_E2::CheckNoFatalStatus_DR()
{//{{{
    return mpMECCCAP_DTSFunc->CheckNoFatalStatus();
}//}}}

/// Check this address is captured in 1bit address register buffer for DE

bool ECC_DMADTS_RAM_E2::Check1bitAddressNotCaptured_DE(unsigned int sub_id, unsigned int mod_address)
{//{{{
    for (unsigned int i = 0; i < mCAP_DEPTH_DE; i++){
        unsigned int cap_addr = mpMECCCAP_DMDEFunc[sub_id]->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_DMDEFunc[sub_id]->GetDE_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Check this address is captured in 1bit address register buffer for DA

bool ECC_DMADTS_RAM_E2::Check1bitAddressNotCaptured_DA(unsigned int sub_id, unsigned int mod_address)
{//{{{
    for (unsigned int i = 0; i < mCAP_DEPTH_DA; i++){
        unsigned int cap_addr = mpMECCCAP_DMDAFunc[sub_id]->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_DMDAFunc[sub_id]->GetDA_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Check this address is captured in 1bit address register buffer for DR

bool ECC_DMADTS_RAM_E2::Check1bitAddressNotCaptured_DR(unsigned int mod_address)
{//{{{
    for (unsigned int i = 0; i < mCAP_DEPTH_DR; i++){
        unsigned int cap_addr = mpMECCCAP_DTSFunc->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_DTSFunc->GetDR_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Update 1bit error list for DE

void ECC_DMADTS_RAM_E2::Update1bitErrorList_DE(unsigned int sub_id)
{//{{{
    bool is_same_port = false;

    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList_DE[sub_id]; i++){
        if (mErrorEDL == m1bitErrorList_DE[sub_id][i].edl_code){
            m1bitErrorList_DE[sub_id][i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList_DE[sub_id][mSize1bitErrorList_DE[sub_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList_DE[sub_id]++;
    }
}//}}}

/// Update 1bit error list for DA

void ECC_DMADTS_RAM_E2::Update1bitErrorList_DA(unsigned int sub_id)
{//{{{
    bool is_same_port = false;

    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList_DA[sub_id]; i++){
        if (mErrorEDL == m1bitErrorList_DA[sub_id][i].edl_code){
            m1bitErrorList_DA[sub_id][i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList_DA[sub_id][mSize1bitErrorList_DA[sub_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList_DA[sub_id]++;
    }
}//}}}

/// Update 1bit error list for DR

void ECC_DMADTS_RAM_E2::Update1bitErrorList_DR()
{//{{{
    bool is_same_port = false;

    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList_DR; i++){
        if (mErrorEDL == m1bitErrorList_DR[i].edl_code){
            m1bitErrorList_DR[i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList_DR[mSize1bitErrorList_DR] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList_DR++;
    }
}//}}}

/// Update fatal error list for DE

void ECC_DMADTS_RAM_E2::UpdateFatalErrorList_DE(unsigned int sub_id)
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if havingpe_id change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList_DE[sub_id]; i++){
        if (mErrorEDL == mFatalErrorList_DE[sub_id][i].edl_code){
            mFatalErrorList_DE[sub_id][i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList_DE[sub_id][i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList_DE[sub_id][mSizeFatalErrorList_DE[sub_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList_DE[sub_id]++;
    }
}//}}}

/// Update fatal error list for DA

void ECC_DMADTS_RAM_E2::UpdateFatalErrorList_DA(unsigned int sub_id)
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if havingpe_id change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList_DA[sub_id]; i++){
        if (mErrorEDL == mFatalErrorList_DA[sub_id][i].edl_code){
            mFatalErrorList_DA[sub_id][i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList_DA[sub_id][i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList_DA[sub_id][mSizeFatalErrorList_DA[sub_id]] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList_DA[sub_id]++;
    }
}//}}}

/// Update fatal error list for DR

void ECC_DMADTS_RAM_E2::UpdateFatalErrorList_DR()
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if havingpe_id change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList_DR; i++){
        if (mErrorEDL == mFatalErrorList_DR[i].edl_code){
            mFatalErrorList_DR[i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList_DR[i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList_DR[mSizeFatalErrorList_DR] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList_DR++;
    }
}//}}}

/// Combine string and unsigned int

std::string ECC_DMADTS_RAM_E2::CombineStringUint(std::string name, unsigned int index)
{//{{{
    std::ostringstream ret_string;
    ret_string.str("");
    ret_string << name << index;
    return ret_string.str();
}//}}}

/// Shift address to corresponding with MSB/LSB

unsigned int ECC_DMADTS_RAM_E2::ShiftAddress(std::string sub_name, unsigned int address)
{//{{{
    sc_assert((sub_name == "DR") || (sub_name == "DE") || (sub_name == "DA"));
    unsigned int mod_addr = 0;
    if (sub_name == "DR"){
        mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB_DR + 1)) - 1))>>mAWIDTH_LSB_DR) << (mAWIDTH_LSB_DR - MIN_AWIDTH_LSB);
    }else if (sub_name == "DE"){
        mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB_DE + 1)) - 1))>>mAWIDTH_LSB_DE) << (mAWIDTH_LSB_DE - MIN_AWIDTH_LSB);
    }else{ // DA
        mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB_DA + 1)) - 1))>>mAWIDTH_LSB_DA) << (mAWIDTH_LSB_DA - MIN_AWIDTH_LSB);
    }
    return mod_addr;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)

void ECC_DMADTS_RAM_E2::SetMessageLevel (const std::string msg_lv)
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

        mpECCCNT_DTSFunc->RegisterHandler(cmd);
        mpMECCCAP_DTSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < nSubDMADTS; i++){
            mpECCCNT_DMDEFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDEFunc[i]->RegisterHandler(cmd);
            mpECCCNT_DMDAFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDAFunc[i]->RegisterHandler(cmd);
        }

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpECCCNT_DTSFunc->RegisterHandler(cmd);
        mpMECCCAP_DTSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < nSubDMADTS; i++){
            mpECCCNT_DMDEFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDEFunc[i]->RegisterHandler(cmd);
            mpECCCNT_DMDAFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDAFunc[i]->RegisterHandler(cmd);
        }
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECCCNT_DTSFunc->RegisterHandler(cmd);
        mpMECCCAP_DTSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < nSubDMADTS; i++){
            mpECCCNT_DMDEFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDEFunc[i]->RegisterHandler(cmd);
            mpECCCNT_DMDAFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDAFunc[i]->RegisterHandler(cmd);
        }

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpECCCNT_DTSFunc->RegisterHandler(cmd);
        mpMECCCAP_DTSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < nSubDMADTS; i++){
            mpECCCNT_DMDEFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDEFunc[i]->RegisterHandler(cmd);
            mpECCCNT_DMDAFunc[i]->RegisterHandler(cmd);
            mpMECCCAP_DMDAFunc[i]->RegisterHandler(cmd);
        }
    }
}//}}}

/// Enable/disable dumping message when users access registers

void ECC_DMADTS_RAM_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);

    mpECCCNT_DTSFunc->RegisterHandler(cmd);
    mpMECCCAP_DTSFunc->RegisterHandler(cmd);
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        mpECCCNT_DMDEFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_DMDEFunc[i]->RegisterHandler(cmd);
        mpECCCNT_DMDAFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_DMDAFunc[i]->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping interrupt information

void ECC_DMADTS_RAM_E2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_DMADTS_RAM_E2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_DMADTS_RAM_E2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_DMADTS_RAM_E2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Set AWIDTH_MSB

void ECC_DMADTS_RAM_E2::AWIDTH_MSB(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "DE"){
        if ((mAWIDTH_LSB_DE <= value) && (value <= MAX_ADDWIDTH_MSB)){
            mAWIDTH_MSB_DE = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_DMADTS_RAM_E2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB_DE, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB_DE);
        }
    }else if (sub_name == "DA"){
        if ((mAWIDTH_LSB_DA <= value) && (value <= MAX_ADDWIDTH_MSB)){
            mAWIDTH_MSB_DA = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_DMADTS_RAM_E2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB_DA, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB_DA);
        }
    }else if (sub_name == "DR"){
        if ((mAWIDTH_LSB_DR <= value) && (value <= MAX_ADDWIDTH_MSB)){
            mAWIDTH_MSB_DR = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_DMADTS_RAM_E2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB_DR, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB_DR);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_DMADTS_RAM_E2_AWIDTH_MSB parameter. Valid name of sub-block is DR or DE or DA.\n", sub_name.c_str());
    }
}//}}}

/// Set AWIDTH_LSB

void ECC_DMADTS_RAM_E2::AWIDTH_LSB(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "DE"){
        if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB_DE)){
            mAWIDTH_LSB_DE = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_DMADTS_RAM_E2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB_DE, DEFAULT_AWIDTH_LSB_DE);
        }
    }else if (sub_name == "DA"){
        if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB_DA)){
            mAWIDTH_LSB_DA = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_DMADTS_RAM_E2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB_DA, DEFAULT_AWIDTH_LSB_DA);
        }
    }else if (sub_name == "DR"){
        if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB_DR)){
            mAWIDTH_LSB_DR = value;
        }else{
            re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_DMADTS_RAM_E2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB_DR, DEFAULT_AWIDTH_LSB_DR);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_DMADTS_RAM_E2_AWIDTH_LSB parameter. Valid name of sub-block is DR or DE or DA.\n", sub_name.c_str());
    }
}//}}}

/// Set CAP_DEPTH

void ECC_DMADTS_RAM_E2::CAP_DEPTH(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "DE"){
        if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
            mCAP_DEPTH_DE = value;
        }else{
            re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_DMADTS_RAM_E2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH_DE);
        }
    }else if (sub_name == "DA"){
        if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
            mCAP_DEPTH_DA = value;
        }else{
            re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_DMADTS_RAM_E2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH_DA);
        }
    }else if (sub_name == "DR"){
        if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
            mCAP_DEPTH_DR = value;
        }else{
            re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_DMADTS_RAM_E2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH_DR);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_DMADTS_RAM_E2_CAP_DEPTH parameter. Valid name of sub-block is DR or DE or DA.\n", sub_name.c_str());
    }
}//}}}

/// Set ERRINT_INIT

void ECC_DMADTS_RAM_E2::ERRINT_INIT(const std::string sub_name, const unsigned int value)
{//{{{
    if (sub_name == "DE"){
        if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
            mERRINT_INIT_DE = value;
        }else{
            re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_DMADTS_RAM_E2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT_DE);
        }
    }else if (sub_name == "DA"){
        if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
            mERRINT_INIT_DA = value;
        }else{
            re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_DMADTS_RAM_E2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT_DA);
        }
    }else if (sub_name == "DR"){
        if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
            mERRINT_INIT_DR = value;
        }else{
            re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_DMADTS_RAM_E2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT_DR);
        }
    }else{
        re_printf("warning", "Invalid name of sub-block (%s) of the ECC_DMADTS_RAM_E2_ERRINT_INIT parameter. Valid name of sub-block is DR or DE or DA.\n", sub_name.c_str());
    }
}//}}}

/// Calculate EDL code

std::string ECC_DMADTS_RAM_E2::CalculateEDLCode()
{//{{{
    std::string edl_name = "";
    if (mCurConfigSubName == "DE"){
        edl_name = CombineStringUint(mStrEDL, mSubId);
        std::map<std::string, unsigned int>::iterator it;
        for (it = mEDLmap_DE.begin(); it != mEDLmap_DE.end(); it++){
            if ((it->first) == edl_name){
                mErrorEDL = it->second;
            }
        }
    }else if (mCurConfigSubName == "DA"){
        edl_name = CombineStringUint(mStrEDL, mSubId);
        std::map<std::string, unsigned int>::iterator it;
        for (it = mEDLmap_DA.begin(); it != mEDLmap_DA.end(); it++){
            if ((it->first) == edl_name){
                mErrorEDL = it->second;
            }
        }
    }else{// DR
        edl_name = CombineStringUint(mStrEDL, mSubId);
        std::map<std::string, unsigned int>::iterator it;
        for (it = mEDLmap_DR.begin(); it != mEDLmap_DR.end(); it++){
            if ((it->first) == edl_name){
                mErrorEDL = it->second;
            }
        }
    }
    return edl_name;
}//}}}

/// Issue error to ECC

void ECC_DMADTS_RAM_E2::IssueError(const unsigned int errortype)
{//{{{
    // Not receive error when reset/clock stop
    if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    // Determine set error to DR or DE or DA, use this line in case extend number input to DR,DE,DA
    mCurConfigSubName = mStrEDL;

    // Issue pseudo error
    if ((mIsEDLConfig) && (mIsAddrConfig)){
        if ((((errortype == em1bitError) || (errortype == em2bitError) || (errortype == emAddressFeedbackError)) && (mCurConfigSubName == "DR"))
            || (((errortype == em1bitError) || (errortype == em2bitError)) && ((mCurConfigSubName == "DE") || (mCurConfigSubName == "DA")))){
            // Calculate mErrorEDL and update mErrorType
            std::string edl_name = CalculateEDLCode();
            mErrorType = (eErrorType)errortype;
            re_printf("info", "Error input Type: %d, EDL: %s, Address: 0x%08X\n", errortype, edl_name.c_str(), mErrorAddress);
            // Process 1bit error
            if (errortype == em1bitError){
                if (mCurConfigSubName == "DE"){
                    Update1bitErrorList_DE(mSubId);
                    mProcess1bitError_DEEvent[mSubId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcess1bitError_DEEvent[mSubId].notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else if (mCurConfigSubName == "DA"){
                    Update1bitErrorList_DA(mSubId);
                    mProcess1bitError_DAEvent[mSubId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcess1bitError_DAEvent[mSubId].notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else{// DR
                    Update1bitErrorList_DR();
                    mProcess1bitError_DREvent.cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcess1bitError_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
                }

            // Fatal error
            }else{
                if (mCurConfigSubName == "DE"){
                    UpdateFatalErrorList_DE(mSubId);
                    mProcessFatalError_DEEvent[mSubId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcessFatalError_DEEvent[mSubId].notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else if (mCurConfigSubName == "DA"){
                    UpdateFatalErrorList_DA(mSubId);
                    mProcessFatalError_DAEvent[mSubId].cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcessFatalError_DAEvent[mSubId].notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);

                }else{// DR
                    UpdateFatalErrorList_DR();
                    mProcessFatalError_DREvent.cancel();
                    double current_time = sc_time_stamp().to_double();
                    mProcessFatalError_DREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
                }
            }
        }else{
            re_printf("warning", "Invalid error type (%d) of ECC_DMADTS_RAM_E2_IssueError command.\n", errortype);
        }
    }else{
        re_printf("warning", "The error address and error location are required to configure before issuing error.\n");
    }
}//}}}

/// Config error address

void ECC_DMADTS_RAM_E2::SetErrorAddr(const unsigned int addr_value)
{//{{{
    mErrorAddress = addr_value;
    mIsAddrConfig = true;
}//}}}

/// Config error detection location (EDL)

void ECC_DMADTS_RAM_E2::SetEDL (const std::string edl, const unsigned int pe_index)
{//{{{
    // Dump warning message if config wrong EDL
    if (((edl != "DR") && (edl != "DE") && (edl != "DA")) || (pe_index > 7)){
        re_printf("warning", "Invalid EDL (%s, %d) of ECC_DMADTS_RAM_E2_SetEDL command.\n", edl.c_str(), pe_index);
    // Store info to use later (in IssueError function)
    }else{
        mStrEDL = edl;
        mSubId = pe_index;
        mIsEDLConfig = true;
    }
}//}}}

/// Dump Status info

void ECC_DMADTS_RAM_E2::DumpStatusInfo()
{//{{{
    // For DE - PEn
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        std::string type = CombineStringUint("StatInfo DE-PE", i);
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DE_SERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_DMDEFunc[i]->GetDE_SERSTR());
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DE_DERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_DMDEFunc[i]->GetDE_DERSTR());
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DE_OVFSTR: 0x%08X\n", type.c_str(), mpMECCCAP_DMDEFunc[i]->GetDE_OVFSTR());
        for (unsigned int k = 0; k < mCAP_DEPTH_DE; k++){
            unsigned int sedl  = mpMECCCAP_DMDEFunc[i]->GetSEDLBit(k);
            unsigned int seadr = mpMECCCAP_DMDEFunc[i]->GetSEADRBit(k) >> (mAWIDTH_LSB_DE - MIN_AWIDTH_LSB);
            printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DE_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type.c_str(), k, sedl, seadr);
        }
        unsigned int dedl  = mpMECCCAP_DMDEFunc[i]->GetDEDLBit();
        unsigned int deadr = mpMECCCAP_DMDEFunc[i]->GetDEADRBit() >> (mAWIDTH_LSB_DE - MIN_AWIDTH_LSB);
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DE_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type.c_str(), dedl, deadr);
    }

    // For DA - PEn
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        std::string type = CombineStringUint("StatInfo DA-PE", i);
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DA_SERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_DMDAFunc[i]->GetDA_SERSTR());
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DA_DERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_DMDAFunc[i]->GetDA_DERSTR());
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DA_OVFSTR: 0x%08X\n", type.c_str(), mpMECCCAP_DMDAFunc[i]->GetDA_OVFSTR());
        for (unsigned int k = 0; k < mCAP_DEPTH_DA; k++){
            unsigned int sedl  = mpMECCCAP_DMDAFunc[i]->GetSEDLBit(k);
            unsigned int seadr = mpMECCCAP_DMDAFunc[i]->GetSEADRBit(k) >> (mAWIDTH_LSB_DA - MIN_AWIDTH_LSB);
            printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DA_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type.c_str(), k, sedl, seadr);
        }
        unsigned int dedl  = mpMECCCAP_DMDAFunc[i]->GetDEDLBit();
        unsigned int deadr = mpMECCCAP_DMDAFunc[i]->GetDEADRBit() >> (mAWIDTH_LSB_DA - MIN_AWIDTH_LSB);
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DA_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type.c_str(), dedl, deadr);
    }

    // For DR
    std::string type1 = "StatInfo DR";
    printf("PROFILE(%s): ECC_DMADTS_RAM_E2: Info [%20s] (%10s):\n", type1.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DR_SERSTR: 0x%08X\n", type1.c_str(), mpMECCCAP_DTSFunc->GetDR_SERSTR());
    printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DR_DERSTR: 0x%08X\n", type1.c_str(), mpMECCCAP_DTSFunc->GetDR_DERSTR());
    printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DR_OVFSTR: 0x%08X\n", type1.c_str(), mpMECCCAP_DTSFunc->GetDR_OVFSTR());
    for (unsigned int i = 0; i < mCAP_DEPTH_DR; i++){
        unsigned int sedl  = mpMECCCAP_DTSFunc->GetSEDLBit(i);
        unsigned int seadr = mpMECCCAP_DTSFunc->GetSEADRBit(i) >> (mAWIDTH_LSB_DR - MIN_AWIDTH_LSB);
        printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DR_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type1.c_str(), i, sedl, seadr);
    }
    unsigned int dedl1  = mpMECCCAP_DTSFunc->GetDEDLBit();
    unsigned int deadr1 = mpMECCCAP_DTSFunc->GetDEADRBit() >> (mAWIDTH_LSB_DR - MIN_AWIDTH_LSB);
    printf("PROFILE(%s): ECC_DMADTS_RAM_E2:   DR_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type1.c_str(), dedl1, deadr1);
}//}}}


/// Command to assert reset

void ECC_DMADTS_RAM_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
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

void ECC_DMADTS_RAM_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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

void ECC_DMADTS_RAM_E2::GetCLKFreq (const std::string clock_name)
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

void ECC_DMADTS_RAM_E2::ForceRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_DMDE"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DMDA"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DTS"){
                mpECCCNT_DTSFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_DMDE"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DMDA"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DTS"){
                mpMECCCAP_DTSFunc->RegisterHandler(cmd);
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

void ECC_DMADTS_RAM_E2::ReleaseRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
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
            if (block_name == "ECCCNT_DMDE"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DMDA"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DTS"){
                mpECCCNT_DTSFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_DMDE"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DMDA"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DTS"){
                mpMECCCAP_DTSFunc->RegisterHandler(cmd);
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

void ECC_DMADTS_RAM_E2::WriteRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_DMDE"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DMDA"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DTS"){
                mpECCCNT_DTSFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_DMDE"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DMDA"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DTS"){
                mpMECCCAP_DTSFunc->RegisterHandler(cmd);
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

void ECC_DMADTS_RAM_E2::ReadRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_DMDE"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DMDA"){
                if (block_index < nSubDMADTS){
                    mpECCCNT_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_DTS"){
                mpECCCNT_DTSFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_DMDE"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDEFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DMDA"){
                if (block_index < nSubDMADTS){
                    mpMECCCAP_DMDAFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_DTS"){
                mpMECCCAP_DTSFunc->RegisterHandler(cmd);
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

void ECC_DMADTS_RAM_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpECCCNT_DTSFunc->RegisterHandler(cmd);
    mpMECCCAP_DTSFunc->RegisterHandler(cmd);
    for (unsigned int i = 0; i < nSubDMADTS; i++){
        mpECCCNT_DMDEFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_DMDEFunc[i]->RegisterHandler(cmd);
        mpECCCNT_DMDAFunc[i]->RegisterHandler(cmd);
        mpMECCCAP_DMDAFunc[i]->RegisterHandler(cmd);
    }
}//}}}

/// Dump help message of all parameters or commands

void ECC_DMADTS_RAM_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_MessageLevel (\"ECC_DMADTS_RAM_E2 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_DumpRegisterRW (\"ECC_DMADTS_RAM_E2 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_DumpInterrupt (\"ECC_DMADTS_RAM_E2 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_AWIDTH_MSB (\"ECC_DMADTS_RAM_E2 instance\", \"sub_name\", value)                                     Set address bit width MSB for DR or DE or DA (Default: 11 for DR, DE; 10 for DA). Valid range to be set: 7-26");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_AWIDTH_LSB (\"ECC_DMADTS_RAM_E2 instance\", \"sub_name\", value)                                     Set address bit width LSB for DR or DE or DA (Default: 2 for DR, DE; 3 for DA). Valid range to be set: 2-4");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_CAP_DEPTH (\"ECC_DMADTS_RAM_E2 instance\", \"sub_name\", value)                                      Set number of address registers for DR or DE or DA (Default: 1 for both DR,DE,and DA). Valid range to be set: 1-32");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_ERRINT_INIT (\"ECC_DMADTS_RAM_E2 instance\", \"sub_name\", value)                                    Set initial value of ERRINT register for DR or DE or DA (Default: 0x8B for DR, 0x83 for DE, DA). Valid range to be set: 0x0-0xFF");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_EnableRegisterMessage (\"ECC_DMADTS_RAM_E2 instance\", \"true/false\")             Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_IssueError (\"ECC_DMADTS_RAM_E2 instance\", error_type)                                              Issue pseudo error to the ECC_DMADTS_RAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_SetErrorAddr (\"ECC_DMADTS_RAM_E2 instance\", addr_value)                                            Set error detection address");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_SetEDL (\"ECC_DMADTS_RAM_E2 instance\", \"edl\", pe_index)                                           Set error detection location");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_DumpStatusInfo (\"ECC_DMADTS_RAM_E2 instance\")                                                      Dump information of the error status registers of the ECC_DMADTS_RAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_AssertReset (\"ECC_DMADTS_RAM_E2 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_DMADTS_RAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_SetCLKFreq (\"ECC_DMADTS_RAM_E2 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_DMADTS_RAM_E2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_GetCLKFreq (\"ECC_DMADTS_RAM_E2 instance\", \"clock_name\")                                          Get clock frequency value of ECC_DMADTS_RAM_E2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_ForceRegister (\"ECC_DMADTS_RAM_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_ReleaseRegister (\"ECC_DMADTS_RAM_E2 instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_WriteRegister (\"ECC_DMADTS_RAM_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_ReadRegister (\"ECC_DMADTS_RAM_E2 instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_DMADTS_RAM_E2_ListRegister (\"ECC_DMADTS_RAM_E2 instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_DMADTS_RAM_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_DMADTS_RAM_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void ECC_DMADTS_RAM_E2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_DMADTS_RAM_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_DMADTS_RAM_E2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_DMADTS_RAM_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
