// ---------------------------------------------------------------------
// $Id: ECC_CFLASH_E2_FCC2.cpp,v 1.18 2019/10/23 08:28:56 landang Exp $
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
#include "ECC_CFLASH_E2_FCC2.h"
#include "ECCCNT_CFP_PEn_FCC2_Func.h"
#include "ECCCNT_CFS_FCC2_Func.h"
#include "ECCCNT_CFC_FCC2_Func.h"
#include "MECCCAP_CFL_FCC2_Func.h"
#include "Common.h"
#include <iostream>
/// Constructor and Destructor

ECC_CFLASH_E2_FCC2::ECC_CFLASH_E2_FCC2(sc_module_name name, const uint8_t iPE_NUM, const uint8_t iCFC_NUM, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_CFLASH_E2_FCC2_AgentController()
        ,rvc_common_model()
        ,PE_NUM(iPE_NUM)
        ,CFC_NUM(iCFC_NUM)
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,sys_clk("sys_clk")
        ,sys_resetz("sys_resetz")
        // ECCCNT CFS
        ,eccdisable_cfs("eccdisable_cfs")
        ,secdisable_cfs("secdisable_cfs")
        // MECCCAP CFL
        ,errovfout1_CF("errovfout1_CF")
        ,errovfout0_CF("errovfout0_CF")
        ,apeerrout_CF("apeerrout_CF")
        ,dederrout_CF("dederrout_CF")
        ,sederrout_CF("sederrout_CF")
        // Key open
        ,keyopen("keyopen")
{//{{{
    //Check valid of PE_NUM and CFC_NUM
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

    if (CFC_NUM < 1) {
       re_printf("warning","Invalid CFC_NUM setting, CFC_NUM is set to default min.\n");
       CFC_NUM = 1;    
    }
    else if (CFC_NUM > 4 ) {
       re_printf("warning","Invalid CFC_NUM setting, CFC_NUM is set to default max.\n");
       CFC_NUM = 4;    
    }
    else {
       // do nothing
    }

    // New instance and port
    std::string port_name;
    //CFP module
    eccdisable_cfp = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("eccdisable_cfp", i);
        eccdisable_cfp[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(eccdisable_cfp[i] != NULL);
        eccdisable_cfp[i]->initialize(false); 
    }
    secdisable_cfp = new sc_out<bool>*[PE_NUM];
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("secdisable_cfp", i);
        secdisable_cfp[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(secdisable_cfp[i] != NULL);
        secdisable_cfp[i]->initialize(false); 
    }
    //CFC module
    eccdisable_cfc = new sc_out<bool>*[CFC_NUM];
    for (unsigned int i = 0; i < CFC_NUM; i++){
        port_name = CombineStringUint("eccdisable_cfc", i);
        eccdisable_cfc[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(eccdisable_cfc[i] != NULL);
        eccdisable_cfc[i]->initialize(false);
    }
    secdisable_cfc = new sc_out<bool>*[CFC_NUM];
    for (unsigned int i = 0; i < CFC_NUM; i++){
        port_name = CombineStringUint("secdisable_cfc", i);
        secdisable_cfc[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(secdisable_cfc[i] != NULL);
        secdisable_cfc[i]->initialize(false);
    }
    apedisable_cfc = new sc_out<bool>*[CFC_NUM];
    for (unsigned int i = 0; i < CFC_NUM; i++){
        port_name = CombineStringUint("apedisable_cfc", i);
        apedisable_cfc[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(apedisable_cfc[i] != NULL);
        apedisable_cfc[i]->initialize(false);
    }
    blankerrormaskenable_cfc = new sc_out<bool>*[CFC_NUM];
    for (unsigned int i = 0; i < CFC_NUM; i++){
        port_name = CombineStringUint("blankerrormaskenable_cfc", i);
        blankerrormaskenable_cfc[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(blankerrormaskenable_cfc[i] != NULL);
        blankerrormaskenable_cfc[i]->initialize(false);
    }
    mpECCCNT_CFPFunc = new ECCCNT_CFP_PEn_FCC2_Func*[PE_MAX_NUM];
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        mpECCCNT_CFPFunc[i] = new ECCCNT_CFP_PEn_FCC2_Func((std::string)name, i, this);
    }
    mpECCCNT_CFCFunc = new ECCCNT_CFC_FCC2_Func*[CL_MAX_NUM];
    for (unsigned int i = 0; i < CL_MAX_NUM; i++){
        mpECCCNT_CFCFunc[i] = new ECCCNT_CFC_FCC2_Func((std::string)name, i, this);
    }
    mpECCCNT_CFSFunc = new ECCCNT_CFS_FCC2_Func((std::string)name, this);
    mpMECCCAP_CFLFunc = new MECCCAP_CFL_FCC2_Func((std::string)name, this);
    
    // Configure Slave socket
 
    BusSlaveBase<32, emNumTgt>::setSlaveResetPort32(&presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn);
    BusSlaveBase<32, emNumTgt>::setSlaveFreqPort32(&pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk);
    BusSlaveBase<32, emNumTgt>::setTargetSocket32((char*)"ts_ecccnt_cfp_0", (char*)"ts_ecccnt_cfp_1", (char*)"ts_ecccnt_cfp_2"
                                                 ,(char*)"ts_ecccnt_cfp_3", (char*)"ts_ecccnt_cfp_4", (char*)"ts_ecccnt_cfp_5"
                                                 ,(char*)"ts_ecccnt_cfp_6", (char*)"ts_ecccnt_cfp_7", (char*)"ts_ecccnt_cfs"
                                                 ,(char*)"ts_ecccnt_cfc_0", (char*)"ts_ecccnt_cfc_1", (char*)"ts_ecccnt_cfc_2"
                                                 ,(char*)"ts_ecccnt_cfc_3", (char*)"ts_mecccap_cf");

    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_ecccnt_cfp[i] = BusSlaveBase<32, emNumTgt>::tSocket32[i];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setFuncModulePtr(mpECCCNT_CFPFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setWriteLatency(wLatency);
    }

    ts_ecccnt_cfs = BusSlaveBase<32, emNumTgt>::tSocket32[emECCCNT_CFSTgtId];
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFSTgtId]->setFuncModulePtr(mpECCCNT_CFSFunc);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFSTgtId]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFSTgtId]->setReadLatency(rLatency);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFSTgtId]->setWriteLatency(wLatency);

    for (unsigned int i = 0; i < CL_MAX_NUM; i++){
        ts_ecccnt_cfc[i] = BusSlaveBase<32, emNumTgt>::tSocket32[emECCCNT_CFCTgtId + i ];
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFCTgtId + i]->setFuncModulePtr(mpECCCNT_CFCFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFCTgtId + i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFCTgtId + i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emECCCNT_CFCTgtId + i]->setWriteLatency(wLatency);
    }

    ts_mecccap_cf = BusSlaveBase<32, emNumTgt>::tSocket32[emMECCCAP_CFTgtId];
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emMECCCAP_CFTgtId]->setFuncModulePtr(mpMECCCAP_CFLFunc);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emMECCCAP_CFTgtId]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emMECCCAP_CFTgtId]->setReadLatency(rLatency);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[emMECCCAP_CFTgtId]->setWriteLatency(wLatency);

    // Initialize remain output ports
    eccdisable_cfs.initialize(false);
    secdisable_cfs.initialize(false);
    errovfout1_CF.initialize(false);
    errovfout0_CF.initialize(false);
    apeerrout_CF.initialize(false);
    dederrout_CF.initialize(false);
    sederrout_CF.initialize(false);

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

    mAWIDTH_MSB = DEFAULT_AWIDTH_MSB;
    mAWIDTH_LSB = DEFAULT_AWIDTH_LSB;
    mCAP_DEPTH  = DEFAULT_CAP_DEPTH;
    mERRINT_INIT= DEFAULT_ERRINT_INIT;

    mDumpInterrupt = false;
    mIsEDLConfig    = false;
    mIsAddrConfig   = false;
    mErrorEDL       = 0;
    mErrorAddress   = 0;
    mStrEDL         = "";
    mKeyOpen        = false;

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
        sc_core::sc_spawn(sc_bind(&ECC_CFLASH_E2_FCC2::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CFLASH_E2_FCC2::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CFLASH_E2_FCC2::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(MonitorkeyopenMethod);
    dont_initialize();
    sensitive << keyopen;

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDisPort_CFPEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CFLASH_E2_FCC2::WriteDisPort_CFPMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteDisPort_CFPMethod"), &opt);
    }

    SC_METHOD(WriteDisPort_CFSMethod);
    dont_initialize();
    sensitive << mWriteDisPort_CFSEvent;

    for (unsigned int cfc_id = 0; cfc_id < CFC_NUM; cfc_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDisPort_CFCEvent[cfc_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CFLASH_E2_FCC2::WriteDisPort_CFCMethod, this, cfc_id),
                          sc_core::sc_gen_unique_name("WriteDisPort_CFCMethod"), &opt);
    }

    SC_METHOD(WriteERROVFOUT0_CFMethod);
    dont_initialize();
    sensitive << mWriteERROVFOUT0_CFEvent;

    SC_METHOD(WriteERROVFOUT1_CFMethod);
    dont_initialize();
    sensitive << mWriteERROVFOUT1_CFEvent;

    SC_METHOD(WriteSEDERROUT_CFMethod);
    dont_initialize();
    sensitive << mWriteSEDERROUT_CFEvent;

    SC_METHOD(NegateERROVFOUT0_CFMethod);
    dont_initialize();
    sensitive << mNegateERROVFOUT0_CFEvent;

    SC_METHOD(NegateERROVFOUT1_CFMethod);
    dont_initialize();
    sensitive << mNegateERROVFOUT1_CFEvent;

    SC_METHOD(AssertSEDERROUT_CFMethod);
    dont_initialize();
    sensitive << mAssertSEDERROUT_CFEvent;

    SC_METHOD(NegateSEDRROUT_CFMethod);
    dont_initialize();
    sensitive << mNegateSEDERROUT_CFEvent;

    SC_METHOD(Process1bitErrorMethod);
    dont_initialize();
    sensitive << mProcess1bitErrorEvent;

    SC_METHOD(ProcessFatalErrorMethod);
    dont_initialize();
    sensitive << mProcessFatalErrorEvent;

    SC_METHOD(SetFatalErrorStatusMethod);
    dont_initialize();
    sensitive << mSetFatalErrorStatusEvent;

    SC_METHOD(NotifyFatalErrorMethod);
    dont_initialize();
    sensitive << mNotifyFatalErrorEvent;

    SC_METHOD(NegateDEDERROUT_CFMethod);
    dont_initialize();
    sensitive << mNegateDEDERROUT_CFEvent;

    SC_METHOD(NegateAPEERROUT_CFMethod);
    dont_initialize();
    sensitive << mNegateAPEERROUT_CFEvent;

    SC_METHOD(WriteDEDERROUT_CFMethod);
    dont_initialize();
    sensitive << mWriteDEDERROUT_CFEvent;

    SC_METHOD(WriteAPEERROUT_CFMethod);
    dont_initialize();
    sensitive << mWriteAPEERROUT_CFEvent;

    SC_THREAD(Set1bitErrorStatusThread);
    dont_initialize();
    sensitive << mSet1bitErrorStatusEvent;
    sensitive << mResetHardEvent[emsys_resetzId];
    sensitive << mResetCmdEvent[emsys_resetzId];
    sensitive << mClkZeroEvent[emsys_clkId];
    sensitive << mClkZeroEvent[empclkId];
}//}}} 

/// Destructor

ECC_CFLASH_E2_FCC2::~ECC_CFLASH_E2_FCC2()
{//{{{
    for (unsigned int i = 0; i < PE_NUM; i++){
        delete eccdisable_cfp[i];
        delete secdisable_cfp[i];
    }
    for (unsigned int i = 0; i < CFC_NUM; i++){
        delete eccdisable_cfc[i];
        delete secdisable_cfc[i];
        delete apedisable_cfc[i];
        delete blankerrormaskenable_cfc[i];
    }
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        delete mpECCCNT_CFPFunc[i];
    }
    for (unsigned int i = 0; i < CL_MAX_NUM; i++){
        delete mpECCCNT_CFCFunc[i];
    }

    if (mpECCCNT_CFPFunc != NULL) delete [] mpECCCNT_CFPFunc;
    if (mpECCCNT_CFCFunc != NULL) delete [] mpECCCNT_CFCFunc;
    delete mpECCCNT_CFSFunc;
    delete mpMECCCAP_CFLFunc;
    if (eccdisable_cfp != NULL) delete [] eccdisable_cfp;
    if (secdisable_cfp != NULL) delete [] secdisable_cfp;
    if (eccdisable_cfc != NULL) delete [] eccdisable_cfc;
    if (secdisable_cfc != NULL) delete [] secdisable_cfc;
    if (apedisable_cfc != NULL) delete [] apedisable_cfc;
    if (blankerrormaskenable_cfc != NULL) delete [] blankerrormaskenable_cfc;

}//}}}

/// Create CFLASH EDL Map

void ECC_CFLASH_E2_FCC2::CreateEDLMap()
{//{{{
    unsigned int order = 0;
    std::string edl_name;
    for (unsigned int i = 0; i < PE_NUM; i++){
        // PEI, pe_index
        edl_name = CombineStringUint("PEI", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, order++));
        // PED, pe_index
        edl_name = CombineStringUint("PED", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, order++));
        // S2F, do not care pe_index, make for all PE ->easy compare later
        edl_name = CombineStringUint("S2F", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0x10));
    } 
}//}}}

/// Initialize when reset activated

void ECC_CFLASH_E2_FCC2::Initialize(void)
{//{{{
    unsigned int cfseccctl  = mpECCCNT_CFSFunc->GetCFSECCCTLReg();
    for (unsigned int i = 0; i < PE_NUM; i++){
        unsigned int cfpeccctl  = mpECCCNT_CFPFunc[i]->GetCFPECCCTLReg();
        mECCDIS_CFP[i] = (bool)(cfpeccctl & 0x1);// get ECCDIS_CFP
        mSECDIS_CFP[i] = (bool)((cfpeccctl >> 1) & 0x1);// get SECDIS_CFP
    }

    for (unsigned int i = 0; i < CFC_NUM; i++){
        unsigned int cfceccctl  = mpECCCNT_CFCFunc[i]->GetCFCECCCTLReg();
        mECCDIS_CFC[i] = (bool)(cfceccctl & 0x1);// get ECCDIS_CFc
        mSECDIS_CFC[i] = (bool)((cfceccctl >> 1) & 0x1);// get SECDIS_CFC
        mAPEDIS_CFC[i] = (bool)((cfceccctl >> 2) & 0x1);// get APEDIS_CFC
        mBLANKMASKENABLE_CFC[i] = (bool)((cfceccctl >> 24) & 0x1);// get BLANKMASKENABLE_CFC
    }
    mECCDIS_CFS             = (bool)(cfseccctl & 0x1);// get ECCDIS_CFS
    mSECDIS_CFS             = (bool)((cfseccctl >> 1) & 0x1);// get SECDIS_CFS
    mERROVFOUT0_CF          = false;
    mERROVFOUT1_CF          = false;
    mSEDERROUT_CF           = false;
    mDEDERROUT_CF           = false;
    mAPEERROUT_CF           = false;
    mHas2bitError           = false;
    mHasAddrParityError     = false;
    mFatalErrorModAddr      = 0;
    mFatalEDLCode           = 0;
    mIsEDLConfig            = false;
    mIsAddrConfig           = false;
    mErrorType              = emNoneError;
    mErrorEDL               = 0;
    mErrorAddress           = 0;
    mSize1bitErrorList = 0;
    mSizeFatalErrorList= 0;
    while (!m1bitErrorFIFO.empty()){
        m1bitErrorFIFO.pop();
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Prepare to write prohibition port of CFP

void ECC_CFLASH_E2_FCC2::WriteDisPort_CFP(unsigned int pe_id, unsigned int value)
{//{{{
    mECCDIS_CFP[pe_id] = (bool)(value & 0x1);
    mSECDIS_CFP[pe_id] = (bool)((value >> 1) & 0x1);
    mWriteDisPort_CFPEvent[pe_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_CFPEvent[pe_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Prepare to write prohibition port of CFS

void ECC_CFLASH_E2_FCC2::WriteDisPort_CFS(unsigned int value)
{//{{{
    mECCDIS_CFS = (bool)(value & 0x1);
    mSECDIS_CFS = (bool)((value >> 1) & 0x1);
    mWriteDisPort_CFSEvent.cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_CFSEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Prepare to write prohibition port of CFC

void ECC_CFLASH_E2_FCC2::WriteDisPort_CFC(unsigned int cfc_id, unsigned int value)
{//{{{
    mECCDIS_CFC[cfc_id] = (bool)(value & 0x1);
    mSECDIS_CFC[cfc_id] = (bool)((value >> 1) & 0x1);
    mAPEDIS_CFC[cfc_id] = (bool)((value >> 2) & 0x1);
    mBLANKMASKENABLE_CFC[cfc_id] = (bool)((value >> 24) & 0x1);
    mWriteDisPort_CFCEvent[cfc_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_CFCEvent[cfc_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}} 

/// Get CAP DEPTH

unsigned int ECC_CFLASH_E2_FCC2::GetCapDepth()
{//{{{
    return mCAP_DEPTH;
}//}}}

/// Assert errovfout0_CF port

void ECC_CFLASH_E2_FCC2::AssertERROVFOUT0_CF()
{//{{{
    mERROVFOUT0_CF = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_CFEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_CF port

void ECC_CFLASH_E2_FCC2::AssertERROVFOUT1_CF()
{//{{{
    mERROVFOUT1_CF = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_CFEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get keycode value

bool ECC_CFLASH_E2_FCC2::GetKeyOpen()
{//{{{
    return mKeyOpen;
}//}}}

/// Get AWIDTH MSB value

unsigned int ECC_CFLASH_E2_FCC2::GetAWIDTH_MSB()
{//{{{
    return mAWIDTH_MSB;
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock

void ECC_CFLASH_E2_FCC2::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor sys_clk clock

void ECC_CFLASH_E2_FCC2::Monitorsys_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

/// Monitor presetn port

void ECC_CFLASH_E2_FCC2::MonitorpresetnMethod()
{//{{{
    mResetCurVal[empresetnId] = presetn.read();
    if (mResetCurVal[empresetnId] != mResetPreVal[empresetnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[empresetnId] = mResetCurVal[empresetnId];
        // Reset signals are asynchronous
        mResetHardEvent[empresetnId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor presetn port

void ECC_CFLASH_E2_FCC2::Monitorsys_resetzMethod()
{//{{{
    mResetCurVal[emsys_resetzId] = sys_resetz.read();
    if (mResetCurVal[emsys_resetzId] != mResetPreVal[emsys_resetzId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emsys_resetzId] = mResetCurVal[emsys_resetzId];
        // Reset signals are asynchronous
        mResetHardEvent[emsys_resetzId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Process reset function when reset port is active

void ECC_CFLASH_E2_FCC2::HandleResetHardMethod (const unsigned int reset_id)
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

void ECC_CFLASH_E2_FCC2::HandleResetCmdMethod (const unsigned int reset_id)
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

void ECC_CFLASH_E2_FCC2::CancelResetCmdMethod (const unsigned int reset_id)
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

void ECC_CFLASH_E2_FCC2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    // Write ERRINT_INIT to ERRINT reg after reset
    if (reset_name=="sys_resetz" && is_active){
        mpMECCCAP_CFLFunc->WriteERRINT_INIT(mERRINT_INIT);
    }
    for (unsigned int i = 0; i < PE_NUM; i++) {
        (mpECCCNT_CFPFunc[i])->EnableReset(reset_name, is_active);
    }
    mpECCCNT_CFSFunc->EnableReset(reset_name, is_active);
    for (unsigned int i = 0; i < CFC_NUM; i++) {
        (mpECCCNT_CFCFunc[i])->EnableReset(reset_name, is_active);
    }
    mpMECCCAP_CFLFunc->EnableReset(reset_name, is_active);
    if(reset_name=="sys_resetz" && is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        for (unsigned int i = 0; i < PE_NUM; i++){
            mWriteDisPort_CFPEvent[i].notify();
        }
        for (unsigned int i = 0; i < CFC_NUM; i++){
            mWriteDisPort_CFCEvent[i].notify();
        }

        mWriteDisPort_CFSEvent.notify();
        mWriteERROVFOUT0_CFEvent.notify();
        mWriteERROVFOUT1_CFEvent.notify();
        mWriteSEDERROUT_CFEvent.notify();
        mWriteDEDERROUT_CFEvent.notify();
        mWriteAPEERROUT_CFEvent.notify();
    }
}//}}}

/// Cancel events

void ECC_CFLASH_E2_FCC2::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < PE_NUM; i++){
        mWriteDisPort_CFPEvent[i].cancel();
    }
    mWriteDisPort_CFSEvent.cancel();
    for (unsigned int i = 0; i < CFC_NUM; i++){
        mWriteDisPort_CFCEvent[i].cancel();
    }
    mWriteERROVFOUT0_CFEvent.cancel();
    mWriteERROVFOUT1_CFEvent.cancel();
    mWriteSEDERROUT_CFEvent.cancel();
    mNegateERROVFOUT0_CFEvent.cancel();
    mNegateERROVFOUT1_CFEvent.cancel();
    mAssertSEDERROUT_CFEvent.cancel();
    mNegateSEDERROUT_CFEvent.cancel();
    mProcess1bitErrorEvent.cancel();
    mProcessFatalErrorEvent.cancel();
    mSetFatalErrorStatusEvent.cancel();
    mNotifyFatalErrorEvent.cancel();
    mNegateDEDERROUT_CFEvent.cancel();
    mNegateAPEERROUT_CFEvent.cancel();
    mWriteDEDERROUT_CFEvent.cancel();
    mWriteAPEERROUT_CFEvent.cancel();
    mSet1bitErrorStatusEvent.cancel();
}//}}}

/// Get reset status

bool ECC_CFLASH_E2_FCC2::GetResetStatus (const std::string reset_name)
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

bool ECC_CFLASH_E2_FCC2::CheckClockPeriod (const std::string clock_name)
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

void ECC_CFLASH_E2_FCC2::MonitorkeyopenMethod()
{//{{{
    mKeyOpen = keyopen.read();
}//}}}

/// Write prohibition port of CFP

void ECC_CFLASH_E2_FCC2::WriteDisPort_CFPMethod(unsigned int pe_id)
{//{{{ 
    eccdisable_cfp[pe_id]->write(mECCDIS_CFP[pe_id]);
    secdisable_cfp[pe_id]->write(mSECDIS_CFP[pe_id]);
}//}}}

/// Write prohibition port of CFS

void ECC_CFLASH_E2_FCC2::WriteDisPort_CFSMethod()
{//{{{
    eccdisable_cfs.write(mECCDIS_CFS);
    secdisable_cfs.write(mSECDIS_CFS);
}//}}}

/// Write disable port CFCECCCTL

void ECC_CFLASH_E2_FCC2::WriteDisPort_CFCMethod(unsigned int cfc_id)
{//{{{
    blankerrormaskenable_cfc[cfc_id]->write(mBLANKMASKENABLE_CFC[cfc_id]);
    apedisable_cfc[cfc_id]->write(mAPEDIS_CFC[cfc_id]);
    eccdisable_cfc[cfc_id]->write(mECCDIS_CFC[cfc_id]);
    secdisable_cfc[cfc_id]->write(mSECDIS_CFC[cfc_id]);

}//}}}

/// Write errovfout0_CF port

void ECC_CFLASH_E2_FCC2::WriteERROVFOUT0_CFMethod()
{//{{{
    errovfout0_CF.write(mERROVFOUT0_CF);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_CF is changed to %d\n", (unsigned int)mERROVFOUT0_CF);
    }
    // Negate if any
    if (mERROVFOUT0_CF){
        mNegateERROVFOUT0_CFEvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_CF port

void ECC_CFLASH_E2_FCC2::WriteERROVFOUT1_CFMethod()
{//{{{
    errovfout1_CF.write(mERROVFOUT1_CF);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_CF is changed to %d\n", (unsigned int)mERROVFOUT1_CF);
    }
    // Negate if any
    if (mERROVFOUT1_CF){
        mNegateERROVFOUT1_CFEvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write sederrout_CF port

void ECC_CFLASH_E2_FCC2::WriteSEDERROUT_CFMethod()
{//{{{
    sederrout_CF.write(mSEDERROUT_CF);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_CF is changed to %d\n", (unsigned int)mSEDERROUT_CF);
    }
    // Negate if any
    if (mSEDERROUT_CF){
        mNegateSEDERROUT_CFEvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)

void ECC_CFLASH_E2_FCC2::AssertSEDERROUT_CFMethod()
{//{{{
    unsigned int sedie = (mpMECCCAP_CFLFunc->GetERRINT() & 0x1);// get sedie
    if (sedie == 1){
        mSEDERROUT_CF = true;
        mWriteSEDERROUT_CFEvent.notify();
    }
}//}}}

/// Negate sederrout_CF port

void ECC_CFLASH_E2_FCC2::NegateSEDRROUT_CFMethod()
{//{{{
    mSEDERROUT_CF = false;
    mWriteSEDERROUT_CFEvent.notify();
}//}}}

/// Negate errovfout0_CF port

void ECC_CFLASH_E2_FCC2::NegateERROVFOUT0_CFMethod()
{//{{{
    mERROVFOUT0_CF = false;
    mWriteERROVFOUT0_CFEvent.notify();
}//}}}

/// Negate errovfout1_CF port

void ECC_CFLASH_E2_FCC2::NegateERROVFOUT1_CFMethod()
{//{{{
    mERROVFOUT1_CF = false;
    mWriteERROVFOUT1_CFEvent.notify();
}//}}}

/// Process 1bit error 

void ECC_CFLASH_E2_FCC2::Set1bitErrorStatusThread()
{//{{{
    while (true){
        if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
            wait();
            continue;
        }
        try{
            while (true){
                while (m1bitErrorFIFO.size() != 0){
                    // Wait 1 cycle
                    wait((double)mClkPeriod[emsys_clkId], mTimeResolutionUnit,
                        mResetHardEvent[emsys_resetzId] | mResetCmdEvent[emsys_resetzId] | mClkZeroEvent[emsys_clkId] | mClkZeroEvent[empclkId]);
                    if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
                        wait();
                        continue;
                    }
                    unsigned int mod_address = ShiftAddress(m1bitErrorFIFO.front().address);
                    if (mpMECCCAP_CFLFunc->CheckHasRoomFor1bit()){
                        // then set status bit if there is room
                        unsigned int edl_code = m1bitErrorFIFO.front().edl_code;
                        if (Check1bitAddressNotCaptured(mod_address)){
                            mpMECCCAP_CFLFunc->Set1bitStatusAndAddress(edl_code, mod_address);
                        }
                    } else {
                        //ECC 1bit error was detacted when all flag are set.
                        if (Check1bitAddressNotCaptured(mod_address)){
                            mpMECCCAP_CFLFunc->SetFlagAddressBufferOverflow();
                            while (!m1bitErrorFIFO.empty()){
                                m1bitErrorFIFO.pop();
                            }
                        }
                    }
                    //remove FIFO
                    if (!m1bitErrorFIFO.empty()){
                        m1bitErrorFIFO.pop();
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

/// Process 1bit error FIFO: update FIFO, assert error notification, update status bit

void ECC_CFLASH_E2_FCC2::Process1bitErrorMethod()
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList[i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO.size() < (PE_NUM*2 +1)) {//emTotalSlavePorts){// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO.push(m1bitErrorList[i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_CFLFunc->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList; i++){
        unsigned int mod_address = ShiftAddress(m1bitErrorList[i].address);
        if ((!CheckMaskAddress(m1bitErrorList[i].address)) && Check1bitAddressNotCaptured(mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_CFEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit

void ECC_CFLASH_E2_FCC2::ProcessFatalErrorMethod()
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError = false;
    mHasAddrParityError = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX; portindex++){// With fix priority
        if (is_update_status){
            break;
        }
        for (unsigned int i = 0; i < mSizeFatalErrorList; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList[i].edl_code == portindex){
                if (CheckNoFatalStatus()){
                    mHas2bitError |= (mFatalErrorList[i].type == em2bitError);
                    mHasAddrParityError |= (mFatalErrorList[i].type == emAddressParityError);
                    mod_address = ShiftAddress(mFatalErrorList[i].address);
                    edl_code = mFatalErrorList[i].edl_code;
                    is_update_status = true;
                }
            }
        }
    }
    if (is_update_status){
        mFatalErrorModAddr = mod_address;
        mFatalEDLCode = edl_code;
        mSetFatalErrorStatusEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSizeFatalErrorList; i++){
        if (!CheckMaskAddress(mFatalErrorList[i].address)){
            if (mFatalErrorList[i].type == em2bitError){
                mDEDERROUT_CF |= true;
            }
            if (mFatalErrorList[i].type == emAddressParityError){
                mAPEERROUT_CF |= true;
            }
        }
    }
    if (mDEDERROUT_CF || mAPEERROUT_CF){
        mNotifyFatalErrorEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList = 0;
}//}}}

/// Set fatal status bit

void ECC_CFLASH_E2_FCC2::SetFatalErrorStatusMethod()
{//{{{
    if (mHas2bitError){
        mpMECCCAP_CFLFunc->Set2bitErrorStatus();
        mpMECCCAP_CFLFunc->SetFatalAddress(mFatalEDLCode, mFatalErrorModAddr);
    }
    if (mHasAddrParityError){
        mpMECCCAP_CFLFunc->SetAddrParityErrorStatus();
        mpMECCCAP_CFLFunc->SetFatalAddress(mFatalEDLCode, mFatalErrorModAddr);
    }
}//}}}

/// Notify fatal error

void ECC_CFLASH_E2_FCC2::NotifyFatalErrorMethod()
{//{{{
    unsigned int errint = mpMECCCAP_CFLFunc->GetERRINT();
    unsigned int dedie = (errint  >> 1) & 0x1;
    unsigned int apeie = (errint >> 2) & 0x1;
    if (mDEDERROUT_CF && (dedie == 1)){
        mWriteDEDERROUT_CFEvent.notify();
    }else{
        mDEDERROUT_CF = false;
    }
    if (mAPEERROUT_CF && (apeie == 1)){
        mWriteAPEERROUT_CFEvent.notify();
    }else{
        mAPEERROUT_CF = false;
    }
}//}}}

/// Negate dederrout_CF port

void ECC_CFLASH_E2_FCC2::NegateDEDERROUT_CFMethod()
{//{{{
    mDEDERROUT_CF = false;
    mWriteDEDERROUT_CFEvent.notify();
}//}}}

/// Negate apeerrout_CF port

void ECC_CFLASH_E2_FCC2::NegateAPEERROUT_CFMethod()
{//{{{
    mAPEERROUT_CF = false;
    mWriteAPEERROUT_CFEvent.notify();
}//}}}

/// Write dederrout_CF port

void ECC_CFLASH_E2_FCC2::WriteDEDERROUT_CFMethod()
{//{{{
    dederrout_CF.write(mDEDERROUT_CF);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_CF is changed to %d\n", (unsigned int)mDEDERROUT_CF);
    }
    if (mDEDERROUT_CF){
        mNegateDEDERROUT_CFEvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write apeerrout_CF port

void ECC_CFLASH_E2_FCC2::WriteAPEERROUT_CFMethod()
{//{{{
    apeerrout_CF.write(mAPEERROUT_CF);
    if (mDumpInterrupt){
        re_printf("info", "apeerrout_CF is changed to %d\n", (unsigned int)mAPEERROUT_CF);
    }
    if (mAPEERROUT_CF){
        mNegateAPEERROUT_CFEvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Check no fatal status

bool ECC_CFLASH_E2_FCC2::CheckNoFatalStatus()
{//{{{
    return mpMECCCAP_CFLFunc->CheckNoFatalStatus();
}//}}}

/// Check an address is masked or not

bool ECC_CFLASH_E2_FCC2::CheckMaskAddress( const unsigned int address)
{//{{{
    for (unsigned int i = 0; i < emNumMaskedArea; i++){
        unsigned int masked_enable = mpMECCCAP_CFLFunc->GetERM_EN(i);
        if (masked_enable != 0){
            unsigned int ermsa = mpMECCCAP_CFLFunc->GetERMSA(i);
            unsigned int ermea = mpMECCCAP_CFLFunc->GetERMEA(i);
            if (ermea >= ermsa){// Ignore if end address < start address
                unsigned int start_mask_address = ermsa << 14;
                unsigned int end_mask_address = (ermea << 14) | 0x3FFF;
                if ((start_mask_address <= address) && (address <= end_mask_address)){
                    return true;
                }
            }
        }
    }
    return false;
}//}}}

/// Check this address is captured in 1bit address register buffer

bool ECC_CFLASH_E2_FCC2::Check1bitAddressNotCaptured(unsigned int mod_address)
{//{{{ 
    for (unsigned int i = 0; i < mCAP_DEPTH; i++){
        unsigned int cap_addr = mpMECCCAP_CFLFunc->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_CFLFunc->GetCF_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Update 1bit error list

void ECC_CFLASH_E2_FCC2::Update1bitErrorList()
{//{{{
    bool is_same_port = false;
    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSize1bitErrorList; i++){
        if (mErrorEDL == m1bitErrorList[i].edl_code){
            m1bitErrorList[i].address = mErrorAddress;
            is_same_port = true;
            break;
        }
    }
    // Add to list if not same port
    if (!is_same_port){
        m1bitErrorList[mSize1bitErrorList] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSize1bitErrorList++;
    }
}//}}}

/// Update fatal error list

void ECC_CFLASH_E2_FCC2::UpdateFatalErrorList()
{//{{{
    bool is_same_port_and_type = false;
    // Update new address if having change in slave port
    for (unsigned int i = 0; i < mSizeFatalErrorList; i++){
        if (mErrorEDL == mFatalErrorList[i].edl_code){
            mFatalErrorList[i].address = mErrorAddress;
            if (mErrorType == mFatalErrorList[i].type){
                is_same_port_and_type = true;
            }
        }
    }
    // Add to list if not same port
    if (!is_same_port_and_type){
        mFatalErrorList[mSizeFatalErrorList] = ErrorInfo(mErrorAddress, mErrorType, mErrorEDL);
        mSizeFatalErrorList++;
    }
}//}}}

/// Combine string and unsigned int

std::string ECC_CFLASH_E2_FCC2::CombineStringUint(std::string name, unsigned int index)
{//{{{
    std::ostringstream ret_string;
    ret_string.str("");
    ret_string << name << index;
    return ret_string.str();
}//}}}

/// Shift address to corresponding with MSB/LSB

unsigned int ECC_CFLASH_E2_FCC2::ShiftAddress(unsigned int address)
{//{{{
    unsigned int mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB + 1)) - 1))>>mAWIDTH_LSB);
    return mod_addr;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)

void ECC_CFLASH_E2_FCC2::SetMessageLevel (const std::string msg_lv)
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
            mpECCCNT_CFPFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CFSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < CFC_NUM; i++){
            mpECCCNT_CFCFunc[i]->RegisterHandler(cmd);
            }
        mpMECCCAP_CFLFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_CFPFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CFSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < CFC_NUM; i++){
            mpECCCNT_CFCFunc[i]->RegisterHandler(cmd);
        }
        mpMECCCAP_CFLFunc->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_CFPFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CFSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < CFC_NUM; i++){
            mpECCCNT_CFCFunc[i]->RegisterHandler(cmd);
        }
        mpMECCCAP_CFLFunc->RegisterHandler(cmd);
        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_CFPFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CFSFunc->RegisterHandler(cmd);
        for (unsigned int i = 0; i < CFC_NUM; i++){
            mpECCCNT_CFCFunc[i]->RegisterHandler(cmd);
        }
        mpMECCCAP_CFLFunc->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers

void ECC_CFLASH_E2_FCC2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_CFPFunc[i]->RegisterHandler(cmd);
    }
    mpECCCNT_CFSFunc->RegisterHandler(cmd);
    for (unsigned int i = 0; i < CFC_NUM; i++){
        mpECCCNT_CFCFunc[i]->RegisterHandler(cmd);
    }
    mpMECCCAP_CFLFunc->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information

void ECC_CFLASH_E2_FCC2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_CFLASH_E2_FCC2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_CFLASH_E2_FCC2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_CFLASH_E2_FCC2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Set AWIDTH_MSB

void ECC_CFLASH_E2_FCC2::AWIDTH_MSB(const unsigned int value)
{//{{{
    if ((mAWIDTH_LSB < value) && (value <= MAX_ADDWIDTH_MSB)){
        mAWIDTH_MSB = value;
    }else{
        re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_CFLASH_E2_FCC2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB);
    }
}//}}}

/// Set AWIDTH_LSB

void ECC_CFLASH_E2_FCC2::AWIDTH_LSB(const unsigned int value)
{//{{{
    if ((MIN_ADDWIDTH_LSB <= (int)value) && (value < mAWIDTH_MSB)){
        mAWIDTH_LSB = value;
    }else{
        re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_CFLASH_E2_FCC2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB, DEFAULT_AWIDTH_LSB);
    }
}//}}}

/// Set CAP_DEPTH

void ECC_CFLASH_E2_FCC2::CAP_DEPTH(const unsigned int value)
{//{{{
    if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
        mCAP_DEPTH = value;
    }else{
        re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_CFLASH_E2_FCC2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH);
    }
}//}}}

/// Set ERRINT_INIT

void ECC_CFLASH_E2_FCC2::ERRINT_INIT(const unsigned int value)
{//{{{
    if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
        mERRINT_INIT = value;
    }else{
        re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_CFLASH_E2_FCC2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT);
    }
}//}}}

/// Issue error to ECC

void ECC_CFLASH_E2_FCC2::IssueError(const unsigned int errortype)
{//{{{
    // Not receive error when reset/clock stop
    if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    if ((errortype == em1bitError) || (errortype == em2bitError) || (errortype == emAddressParityError)){
        if ((mIsEDLConfig) && (mIsAddrConfig)){
            mErrorType = (eErrorType)errortype;
            re_printf("info", "Error input Type: %d, EDL: %s, Address: 0x%08X\n", errortype, mStrEDL.c_str(), mErrorAddress);
            // 1bit error
            if (errortype == em1bitError){
                Update1bitErrorList();
                mProcess1bitErrorEvent.cancel();
                double current_time = sc_time_stamp().to_double();
                mProcess1bitErrorEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
            // Fatal error
            }else{
                UpdateFatalErrorList();
                mProcessFatalErrorEvent.cancel();
                double current_time = sc_time_stamp().to_double();
                mProcessFatalErrorEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[emsys_clkId], true, current_time, mStartClkTime[emsys_clkId]), mTimeResolutionUnit);
            }
        }else{
            re_printf("warning", "The error address and error location are required to configure before issuing error.\n");
        }
    }else{
        re_printf("warning", "Invalid error type (%d) of ECC_CFLASH_E2_FCC2_IssueError command.\n", errortype);
    }
}//}}}

/// Config error address

void ECC_CFLASH_E2_FCC2::SetErrorAddr(const unsigned int addr_value)
{//{{{
    mErrorAddress = addr_value;
    mIsAddrConfig = true;
}//}}}

/// Config error detection location (EDL)

void ECC_CFLASH_E2_FCC2::SetEDL (const std::string edl, const unsigned int pe_index)
{//{{{
    std::string edl_name = CombineStringUint(edl, pe_index);
    std::map<std::string, unsigned int>::iterator it;

    // Update EDL if any
    for (it = mEDLmap.begin(); it != mEDLmap.end(); it++){
        if ((it->first) == edl_name){
            mErrorEDL = it->second;
            mStrEDL = edl_name;
            mIsEDLConfig = true;
        }
    }
    
    // Dump warning message if config wrong EDL
    if (!mIsEDLConfig){
        re_printf("warning", "Invalid EDL (%s, %d) of ECC_CFLASH_E2_FCC2_SetEDL command.\n", edl.c_str(), pe_index);
    }
}//}}}

/// Dump Status info

void ECC_CFLASH_E2_FCC2::DumpStatusInfo()
{//{{{
    std::string type = "StatInfo";
    printf("PROFILE(%s): ECC_CFLASH_E2_FCC2: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): ECC_CFLASH_E2_FCC2:   CF_SERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_CFLFunc->GetCF_SERSTR());
    printf("PROFILE(%s): ECC_CFLASH_E2_FCC2:   CF_DERSTR: 0x%08X\n", type.c_str(), mpMECCCAP_CFLFunc->GetCF_DERSTR());
    printf("PROFILE(%s): ECC_CFLASH_E2_FCC2:   CF_OVFSTR: 0x%08X\n", type.c_str(), mpMECCCAP_CFLFunc->GetCF_OVFSTR());
    for (unsigned int i = 0; i < mCAP_DEPTH; i++){
        unsigned int sedl = mpMECCCAP_CFLFunc->GetSEDLBit(i);
        unsigned int seadr = mpMECCCAP_CFLFunc->GetSEADRBit(i);
        printf("PROFILE(%s): ECC_CFLASH_E2_FCC2:   CF_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", type.c_str(), i, sedl, seadr);
    }
    unsigned int dedl = mpMECCCAP_CFLFunc->GetDEDLBit();
    unsigned int deadr = mpMECCCAP_CFLFunc->GetDEADRBit();
    printf("PROFILE(%s): ECC_CFLASH_E2_FCC2:   CF_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", type.c_str(), dedl, deadr);
}//}}}


/// Command to assert reset

void ECC_CFLASH_E2_FCC2::AssertReset (const std::string reset_name, const double start_time, const double period)
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

void ECC_CFLASH_E2_FCC2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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

void ECC_CFLASH_E2_FCC2::GetCLKFreq (const std::string clock_name)
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

void ECC_CFLASH_E2_FCC2::ForceRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_CFP"){
                if (block_index < PE_NUM){
                    mpECCCNT_CFPFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CFS"){
                mpECCCNT_CFSFunc->RegisterHandler(cmd);
            }else if (block_name == "ECCCNT_CFC"){
                if (block_index < CFC_NUM){
                    mpECCCNT_CFCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_CFL"){
                mpMECCCAP_CFLFunc->RegisterHandler(cmd);
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

void ECC_CFLASH_E2_FCC2::ReleaseRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
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
            if (block_name == "ECCCNT_CFP"){
                if (block_index < PE_NUM){
                    mpECCCNT_CFPFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CFS"){
                mpECCCNT_CFSFunc->RegisterHandler(cmd);
            }else if (block_name == "ECCCNT_CFC"){
                if (block_index < CFC_NUM){
                    mpECCCNT_CFCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_CFL"){
                mpMECCCAP_CFLFunc->RegisterHandler(cmd);
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

void ECC_CFLASH_E2_FCC2::WriteRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_CFP"){
                if (block_index < PE_NUM){
                    mpECCCNT_CFPFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CFS"){
                mpECCCNT_CFSFunc->RegisterHandler(cmd);
            }else if (block_name == "ECCCNT_CFC"){
                if (block_index < CFC_NUM){
                    mpECCCNT_CFCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_CFL"){
                mpMECCCAP_CFLFunc->RegisterHandler(cmd);
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

void ECC_CFLASH_E2_FCC2::ReadRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_CFP"){
                if (block_index < PE_NUM){
                    mpECCCNT_CFPFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CFS"){
                mpECCCNT_CFSFunc->RegisterHandler(cmd);
            }else if (block_name == "ECCCNT_CFC"){
                if (block_index < CFC_NUM){
                    mpECCCNT_CFCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "MECCCAP_CFL"){
                mpMECCCAP_CFLFunc->RegisterHandler(cmd);
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

void ECC_CFLASH_E2_FCC2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_CFPFunc[i]->RegisterHandler(cmd);
    }
    mpECCCNT_CFSFunc->RegisterHandler(cmd);
    for (unsigned int i = 0; i < CFC_NUM; i++){
        mpECCCNT_CFCFunc[i]->RegisterHandler(cmd);
    }
    mpMECCCAP_CFLFunc->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands

void ECC_CFLASH_E2_FCC2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_MessageLevel (\"ECC_CFLASH_E2_FCC2 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_DumpRegisterRW (\"ECC_CFLASH_E2_FCC2 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_DumpInterrupt (\"ECC_CFLASH_E2_FCC2 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_AWIDTH_MSB (\"ECC_CFLASH_E2_FCC2 instance\", value)                                                   Set address bit width MSB (Default: 25). Valid range to be set: 7-26");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_AWIDTH_LSB (\"ECC_CFLASH_E2_FCC2 instance\", value)                                                   Set address bit width LSB (Default: 2). Valid range to be set: 2-4");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_CAP_DEPTH (\"ECC_CFLASH_E2_FCC2 instance\", value)                                                    Set number of address registers (Default: 4). Valid range to be set: 1-32");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_ERRINT_INIT (\"ECC_CFLASH_E2_FCC2 instance\", value)                                                  Set initial value of ERRINT register (Default: 0x87). Valid range to be set: 0x0-0xFF");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_EnableRegisterMessage (\"ECC_CFLASH_E2_FCC2 instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_IssueError (\"ECC_CFLASH_E2_FCC2 instance\", error_type)                                              Issue pseudo error to the ECC_CFLASH_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_SetErrorAddr (\"ECC_CFLASH_E2_FCC2 instance\", addr_value)                                            Set error detection address");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_SetEDL (\"ECC_CFLASH_E2_FCC2 instance\", \"edl\", pe_index)                                           Set error detection location");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_DumpStatusInfo (\"ECC_CFLASH_E2_FCC2 instance\")                                                      Dump information of the error status registers of the ECC_CFLASH_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_AssertReset (\"ECC_CFLASH_E2_FCC2 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_CFLASH_E2_FCC2 model");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_SetCLKFreq (\"ECC_CFLASH_E2_FCC2 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_CFLASH_E2_FCC2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_GetCLKFreq (\"ECC_CFLASH_E2_FCC2 instance\", \"clock_name\")                                          Get clock frequency value of ECC_CFLASH_E2_FCC2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_ForceRegister (\"ECC_CFLASH_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_ReleaseRegister (\"ECC_CFLASH_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_WriteRegister (\"ECC_CFLASH_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_ReadRegister (\"ECC_CFLASH_E2_FCC2 instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_CFLASH_E2_FCC2_ListRegister (\"ECC_CFLASH_E2_FCC2 instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_CFLASH_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_CFLASH_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void ECC_CFLASH_E2_FCC2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_CFLASH_E2_FCC2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_CFLASH_E2_FCC2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_CFLASH_E2_FCC2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
