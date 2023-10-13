// ---------------------------------------------------------------------
// $Id: ECC_CRAM_E2.cpp,v 1.5 2019/10/23 08:28:58 landang Exp $
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
#include "ECC_CRAM_E2.h"
#include "ECCCNT_CRCn_Func.h"
#include "ECCCNT_CRA_Func.h"
#include "MECCCAP_CR_Func.h"

/// Constructor and Destructor
ECC_CRAM_E2::ECC_CRAM_E2(sc_module_name name, const uint8_t iPE_NUM, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,ECC_CRAM_E2_AgentController()
        ,rvc_common_model()
        ,PE_NUM(iPE_NUM)
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        ,sys_clk("sys_clk")
        ,sys_resetz("sys_resetz")
        // ECCCNT CRA
        ,eccdisable("eccdisable")
        ,secdisable("secdisable")
        // MECCCAP CR
        ,errovfout1_CR("errovfout1_CR")
        ,errovfout0_CR("errovfout0_CR")
        ,afeerrout_CR("afeerrout_CR")
        ,dederrout_CR("dederrout_CR")
        ,sederrout_CR("sederrout_CR")
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
    afedisable = new sc_out<bool>*[PE_NUM];
    std::string port_name;
    for (unsigned int i = 0; i < PE_NUM; i++){
        port_name = CombineStringUint("afedisable", i);
        afedisable[i] = new sc_out<bool>(port_name.c_str());
        sc_assert(afedisable[i] != NULL);
        afedisable[i]->initialize(false); 
    }
    mpECCCNT_CRCFunc = new ECCCNT_CRCn_Func*[PE_MAX_NUM];
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        mpECCCNT_CRCFunc[i] = new ECCCNT_CRCn_Func((std::string)name, i, this);
    }
    mpECCCNT_CRAFunc = new ECCCNT_CRA_Func((std::string)name, this);
    mpMECCCAP_CRFunc = new MECCCAP_CR_Func((std::string)name, this);
    
    // Configure Slave socket

    BusSlaveBase<32, emNumTgt>::setSlaveResetPort32(&presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn, &presetn, &presetn
                                                  , &presetn);
    BusSlaveBase<32, emNumTgt>::setSlaveFreqPort32(&pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk, &pclk, &pclk
                                                 , &pclk);
    BusSlaveBase<32, emNumTgt>::setTargetSocket32((char*)"ts_ecccnt_crc_0", (char*)"ts_ecccnt_crc_1", (char*)"ts_ecccnt_crc_2"
                                                 ,(char*)"ts_ecccnt_crc_3", (char*)"ts_ecccnt_crc_4", (char*)"ts_ecccnt_crc_5"
                                                 ,(char*)"ts_ecccnt_crc_6", (char*)"ts_ecccnt_crc_7", (char*)"ts_ecccnt_cra"
                                                 ,(char*)"ts_mecccap_cr");

    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        ts_ecccnt_crc[i] = BusSlaveBase<32, emNumTgt>::tSocket32[i];

        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setFuncModulePtr(mpECCCNT_CRCFunc[i]);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setBusProtocol(BUS_APB);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setReadLatency(rLatency);
        BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[i]->setWriteLatency(wLatency);
    }

    ts_ecccnt_cra = BusSlaveBase<32, emNumTgt>::tSocket32[PE_MAX_NUM];
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM]->setFuncModulePtr(mpECCCNT_CRAFunc);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM]->setReadLatency(rLatency);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM]->setWriteLatency(wLatency);

    ts_mecccap_cr = BusSlaveBase<32, emNumTgt>::tSocket32[PE_MAX_NUM + 1];
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + 1]->setFuncModulePtr(mpMECCCAP_CRFunc);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + 1]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + 1]->setReadLatency(rLatency);
    BusSlaveBase<32, emNumTgt>::mBusSlaveIf32[PE_MAX_NUM + 1]->setWriteLatency(wLatency);

    // Initialize remain output ports
    eccdisable.initialize(false);
    secdisable.initialize(false);
    errovfout1_CR.initialize(false);
    errovfout0_CR.initialize(false);
    afeerrout_CR.initialize(false);
    dederrout_CR.initialize(false);
    sederrout_CR.initialize(false);

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
    mErrorType      = emNoneError;
    mErrorEDL       = 0;
    mErrorAddress   = 0;
    mStrEDL         = "";
    mPeId           = 0;
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
        sc_core::sc_spawn(sc_bind(&ECC_CRAM_E2::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CRAM_E2::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emNumReset; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CRAM_E2::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    SC_METHOD(MonitorkeyopenMethod);
    dont_initialize();
    sensitive << keyopen;

    for (unsigned int pe_id = 0; pe_id < PE_NUM; pe_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteDisPort_CRCEvent[pe_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CRAM_E2::WriteDisPort_CRCMethod, this, pe_id),
                          sc_core::sc_gen_unique_name("WriteDisPort_CRCMethod"), &opt);
    }

    SC_METHOD(WriteDisPort_CRAMethod);
    dont_initialize();
    sensitive << mWriteDisPort_CRAEvent;

    SC_METHOD(WriteERROVFOUT0_CRMethod);
    dont_initialize();
    sensitive << mWriteERROVFOUT0_CREvent;

    SC_METHOD(WriteERROVFOUT1_CRMethod);
    dont_initialize();
    sensitive << mWriteERROVFOUT1_CREvent;

    SC_METHOD(WriteSEDERROUT_CRMethod);
    dont_initialize();
    sensitive << mWriteSEDERROUT_CREvent;

    SC_METHOD(NegateERROVFOUT0_CRMethod);
    dont_initialize();
    sensitive << mNegateERROVFOUT0_CREvent;

    SC_METHOD(NegateERROVFOUT1_CRMethod);
    dont_initialize();
    sensitive << mNegateERROVFOUT1_CREvent;

    SC_METHOD(AssertSEDERROUT_CRMethod);
    dont_initialize();
    sensitive << mAssertSEDERROUT_CREvent;

    SC_METHOD(NegateSEDRROUT_CRMethod);
    dont_initialize();
    sensitive << mNegateSEDERROUT_CREvent;

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

    SC_METHOD(NegateDEDERROUT_CRMethod);
    dont_initialize();
    sensitive << mNegateDEDERROUT_CREvent;

    SC_METHOD(NegateAFEERROUT_CRMethod);
    dont_initialize();
    sensitive << mNegateAFEERROUT_CREvent;

    SC_METHOD(WriteDEDERROUT_CRMethod);
    dont_initialize();
    sensitive << mWriteDEDERROUT_CREvent;

    SC_METHOD(WriteAFEERROUT_CRMethod);
    dont_initialize();
    sensitive << mWriteAFEERROUT_CREvent;

    SC_THREAD(Set1bitErrorStatusThread);
    dont_initialize();
    sensitive << mSet1bitErrorStatusEvent;
    sensitive << mResetHardEvent[emsys_resetzId];
    sensitive << mResetCmdEvent[emsys_resetzId];
    sensitive << mClkZeroEvent[emsys_clkId];
    sensitive << mClkZeroEvent[empclkId];
}//}}} 

/// Destructor
ECC_CRAM_E2::~ECC_CRAM_E2()
{//{{{
    for (unsigned int i = 0; i < PE_MAX_NUM; i++){
        delete mpECCCNT_CRCFunc[i];
    }
    delete mpECCCNT_CRAFunc;
    delete mpMECCCAP_CRFunc;

    for (unsigned int i = 0; i < PE_NUM; i++){
        if (afedisable[i] != NULL){
            delete afedisable[i];
        }
    }
    if (afedisable != NULL) delete [] afedisable;
    if (mpECCCNT_CRCFunc != NULL) delete [] mpECCCNT_CRCFunc;
}//}}}

/// Create CRAM EDL Map
void ECC_CRAM_E2::CreateEDLMap()
{//{{{
    std::string edl_name;
    for (unsigned int i = 0; i < PE_NUM; i++){
        // PE, pe_index
        edl_name = CombineStringUint("PE", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, i));

        // S2M0L, pe_index
        edl_name = CombineStringUint("S2M0L", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0x8));
        // S2M0U, pe_index
        edl_name = CombineStringUint("S2M0U", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0x9));
        // S2M1L, pe_index
        edl_name = CombineStringUint("S2M1L", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0xA));
        // S2M1U, pe_index
        edl_name = CombineStringUint("S2M1U", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0xB));

        // Cram0L, pe_index
        edl_name = CombineStringUint("Cram0L", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0x10));
        // Cram0U, pe_index
        edl_name = CombineStringUint("Cram0U", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0x11));
        // Cram1L, pe_index
        edl_name = CombineStringUint("Cram1L", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0x12));
        // Cram1U, pe_index
        edl_name = CombineStringUint("Cram1U", i);
        mEDLmap.insert(std::pair<std::string, unsigned int>(edl_name, 0x13));
    } 
}//}}}

/// Initialize when reset activated
void ECC_CRAM_E2::Initialize(void)
{//{{{
    unsigned int craeccctl  = mpECCCNT_CRAFunc->GetCRAECCCTLReg();
    for (unsigned int i = 0; i < PE_NUM; i++){
        unsigned int crceccctl  = mpECCCNT_CRCFunc[i]->GetCRCAFCTLReg();
        mAFEDIS[i] = (bool)((crceccctl >> 3) & 0x1);// get AFEDIS_CRC
    }
    mECCDIS                     = (bool)(craeccctl & 0x1);// get ECCDIS_CRA
    mSECDIS                     = (bool)((craeccctl >> 1) & 0x1);// get SECDIS_CRA
    mERROVFOUT0_CR              = false;
    mERROVFOUT1_CR              = false;
    mSEDERROUT_CR               = false;
    mDEDERROUT_CR               = false;
    mAFEERROUT_CR               = false;
    mHas2bitError               = false;
    mHasAddrFeedbackError       = false;
    mFatalErrorModAddr          = 0;
    mFatalEDLCode               = 0;
    mIsEDLConfig                = false;
    mIsAddrConfig               = false;
    mErrorType                  = emNoneError;
    mErrorEDL                   = 0;
    mErrorAddress               = 0;
    mSize1bitErrorList          = 0;
    mSizeFatalErrorList         = 0;
    while (!m1bitErrorFIFO.empty()){
        m1bitErrorFIFO.pop();
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Prepare to write prohibition port of CFP
void ECC_CRAM_E2::WriteDisPort_CRC(unsigned int pe_id, unsigned int value)
{//{{{
    mAFEDIS[pe_id] = (bool)((value >> 3) & 0x1);
    mWriteDisPort_CRCEvent[pe_id].cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_CRCEvent[pe_id].notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Prepare to write prohibition port of CRA
void ECC_CRAM_E2::WriteDisPort_CRA(unsigned int value)
{//{{{
    mECCDIS = (bool)(value & 0x1);
    mSECDIS = (bool)((value >> 1) & 0x1);
    mWriteDisPort_CRAEvent.cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisPort_CRAEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get CAP DEPTH
unsigned int ECC_CRAM_E2::GetCapDepth()
{//{{{
    return mCAP_DEPTH;
}//}}}

/// Assert errovfout0_CR port
void ECC_CRAM_E2::AssertERROVFOUT0_CR()
{//{{{
    mERROVFOUT0_CR = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT0_CREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Assert errovfout1_CR port
void ECC_CRAM_E2::AssertERROVFOUT1_CR()
{//{{{
    mERROVFOUT1_CR = true;
    double current_time = sc_time_stamp().to_double();
    mWriteERROVFOUT1_CREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], true, current_time, mStartClkTime[empclkId]), mTimeResolutionUnit);
}//}}}

/// Get keycode value
bool ECC_CRAM_E2::GetKeyOpen()
{//{{{
    return mKeyOpen;
}//}}}

/// Get AWIDTH MSB value
unsigned int ECC_CRAM_E2::GetAWIDTH_MSB()
{//{{{
    return mAWIDTH_MSB;
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock
void ECC_CRAM_E2::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor sys_clk clock
void ECC_CRAM_E2::Monitorsys_clkMethod()
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

/// Monitor presetn port
void ECC_CRAM_E2::MonitorpresetnMethod()
{//{{{
    mResetCurVal[empresetnId] = presetn.read();
    if (mResetCurVal[empresetnId] != mResetPreVal[empresetnId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[empresetnId] = mResetCurVal[empresetnId];
        // Reset signals are asynchronous
        mResetHardEvent[empresetnId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Monitor presetn port
void ECC_CRAM_E2::Monitorsys_resetzMethod()
{//{{{
    mResetCurVal[emsys_resetzId] = sys_resetz.read();
    if (mResetCurVal[emsys_resetzId] != mResetPreVal[emsys_resetzId]) { //mResetPreVal is "true" at initial state
        mResetPreVal[emsys_resetzId] = mResetCurVal[emsys_resetzId];
        // Reset signals are asynchronous
        mResetHardEvent[emsys_resetzId].notify(SC_ZERO_TIME);
    }
}//}}}

/// Process reset function when reset port is active
void ECC_CRAM_E2::HandleResetHardMethod (const unsigned int reset_id)
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
void ECC_CRAM_E2::HandleResetCmdMethod (const unsigned int reset_id)
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
void ECC_CRAM_E2::CancelResetCmdMethod (const unsigned int reset_id)
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
void ECC_CRAM_E2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    // Write ERRINT_INIT to ERRINT reg after reset
    if (reset_name=="sys_resetz" && is_active){
        mpMECCCAP_CRFunc->WriteERRINT_INIT(mERRINT_INIT);
    }
    for (unsigned int i = 0; i < PE_NUM; i++) {
        (mpECCCNT_CRCFunc[i])->EnableReset(reset_name, is_active);
    }
    mpECCCNT_CRAFunc->EnableReset(reset_name, is_active);
    mpMECCCAP_CRFunc->EnableReset(reset_name, is_active);

    if(reset_name=="sys_resetz" && is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        for (unsigned int i = 0; i < PE_NUM; i++){
            mWriteDisPort_CRCEvent[i].notify();
        }
        mWriteDisPort_CRAEvent.notify();
        mWriteERROVFOUT0_CREvent.notify();
        mWriteERROVFOUT1_CREvent.notify();
        mWriteSEDERROUT_CREvent.notify();
        mWriteDEDERROUT_CREvent.notify();
        mWriteAFEERROUT_CREvent.notify();
    }
}//}}}

/// Cancel events

void ECC_CRAM_E2::CancelEvents()
{//{{{
    for (unsigned int i = 0; i < PE_NUM; i++){
        mWriteDisPort_CRCEvent[i].cancel();
    }
    mWriteDisPort_CRAEvent.cancel();
    mWriteERROVFOUT0_CREvent.cancel();
    mWriteERROVFOUT1_CREvent.cancel();
    mWriteSEDERROUT_CREvent.cancel();
    mNegateERROVFOUT0_CREvent.cancel();
    mNegateERROVFOUT1_CREvent.cancel();
    mAssertSEDERROUT_CREvent.cancel();
    mNegateSEDERROUT_CREvent.cancel();
    mProcess1bitErrorEvent.cancel();
    mProcessFatalErrorEvent.cancel();
    mSetFatalErrorStatusEvent.cancel();
    mNotifyFatalErrorEvent.cancel();
    mNegateDEDERROUT_CREvent.cancel();
    mNegateAFEERROUT_CREvent.cancel();
    mWriteDEDERROUT_CREvent.cancel();
    mWriteAFEERROUT_CREvent.cancel();
    mSet1bitErrorStatusEvent.cancel();
}//}}}

/// Get reset status
bool ECC_CRAM_E2::GetResetStatus (const std::string reset_name)
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
bool ECC_CRAM_E2::CheckClockPeriod (const std::string clock_name)
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
void ECC_CRAM_E2::MonitorkeyopenMethod()
{//{{{
    mKeyOpen = keyopen.read();
}//}}}

/// Write prohibition port of CFP
void ECC_CRAM_E2::WriteDisPort_CRCMethod(unsigned int pe_id)
{//{{{ 
    afedisable[pe_id]->write(mAFEDIS[pe_id]);
}//}}}

/// Write prohibition port of CRA
void ECC_CRAM_E2::WriteDisPort_CRAMethod()
{//{{{
    eccdisable.write(mECCDIS);
    secdisable.write(mSECDIS);
}//}}}

/// Write errovfout0_CR port
void ECC_CRAM_E2::WriteERROVFOUT0_CRMethod()
{//{{{
    errovfout0_CR.write(mERROVFOUT0_CR);
    if (mDumpInterrupt){
        re_printf("info", "errovfout0_CR is changed to %d\n", (unsigned int)mERROVFOUT0_CR);
    }
    // Negate if any
    if (mERROVFOUT0_CR){
        mNegateERROVFOUT0_CREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write errovfout1_CR port
void ECC_CRAM_E2::WriteERROVFOUT1_CRMethod()
{//{{{
    errovfout1_CR.write(mERROVFOUT1_CR);
    if (mDumpInterrupt){
        re_printf("info", "errovfout1_CR is changed to %d\n", (unsigned int)mERROVFOUT1_CR);
    }
    // Negate if any
    if (mERROVFOUT1_CR){
        mNegateERROVFOUT1_CREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write sederrout_CR port
void ECC_CRAM_E2::WriteSEDERROUT_CRMethod()
{//{{{
    sederrout_CR.write(mSEDERROUT_CR);
    if (mDumpInterrupt){
        re_printf("info", "sederrout_CR is changed to %d\n", (unsigned int)mSEDERROUT_CR);
    }
    // Negate if any
    if (mSEDERROUT_CR){
        mNegateSEDERROUT_CREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Assert 1bit error notification (to output port)
void ECC_CRAM_E2::AssertSEDERROUT_CRMethod()
{//{{{
    unsigned int sedie = (mpMECCCAP_CRFunc->GetERRINT() & 0x1);// get sedie
    if (sedie == 1){
        mSEDERROUT_CR = true;
        mWriteSEDERROUT_CREvent.notify();
    }
}//}}}

/// Negate sederrout_CR port
void ECC_CRAM_E2::NegateSEDRROUT_CRMethod()
{//{{{
    mSEDERROUT_CR = false;
    mWriteSEDERROUT_CREvent.notify();
}//}}}

/// Negate errovfout0_CR port
void ECC_CRAM_E2::NegateERROVFOUT0_CRMethod()
{//{{{
    mERROVFOUT0_CR = false;
    mWriteERROVFOUT0_CREvent.notify();
}//}}}

/// Negate errovfout1_CR port
void ECC_CRAM_E2::NegateERROVFOUT1_CRMethod()
{//{{{
    mERROVFOUT1_CR = false;
    mWriteERROVFOUT1_CREvent.notify();
}//}}}

/// Process 1bit error 
void ECC_CRAM_E2::Set1bitErrorStatusThread()
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
                    if (mpMECCCAP_CRFunc->CheckHasRoomFor1bit()){
                        // then set status bit if there is room
                        unsigned int edl_code = m1bitErrorFIFO.front().edl_code;
                        if (Check1bitAddressNotCaptured(mod_address)){
                            mpMECCCAP_CRFunc->Set1bitStatusAndAddress(edl_code, mod_address);
                        }
                    } else {
                        //ECC 1bit error was detacted when all flag are set.
                        if (Check1bitAddressNotCaptured(mod_address)){
                            mpMECCCAP_CRFunc->SetFlagAddressBufferOverflow();
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
void ECC_CRAM_E2::Process1bitErrorMethod()
{//{{{
    // Update 1bit error FIFO
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX; portindex++){// With fix priority
        for (unsigned int i = 0; i < mSize1bitErrorList; i++){// Scan list to find high priority
            // Put to FIFO from the high priority (lowest port index)
            if (m1bitErrorList[i].edl_code == portindex){
                // Check FIFO is full or not, if not, push to FIFO
                if (m1bitErrorFIFO.size() < (PE_NUM+8)) {//emTotalSlavePorts){// Fixed FIFO size = emTotalSlavePorts
                    m1bitErrorFIFO.push(m1bitErrorList[i]);
                    break;// break due to each port just has only one 1bit error in 1cycle
                // If FIFO full, raise FIFO overflow
                }else{
                    mpMECCCAP_CRFunc->SetFIFOOverflow();
                }
            }
        }
    }

    // Assert error notification if address not masked
    for (unsigned int i = 0; i < mSize1bitErrorList; i++){
        unsigned int mod_address = ShiftAddress(m1bitErrorList[i].address);
        if ((!CheckMaskAddress(m1bitErrorList[i].address)) && Check1bitAddressNotCaptured(mod_address)){
            double current_time = sc_time_stamp().to_double();
            mAssertSEDERROUT_CREvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
            break;// just 1 address not masked is enough to raise error notification
        }
    }

    // Update status bit
    mSet1bitErrorStatusEvent.notify();

    // Clear m1bitErrorList
    mSize1bitErrorList = 0;
}//}}}

/// Process Fatal error FIFO: update FIFO, assert error notification, update status bit
void ECC_CRAM_E2::ProcessFatalErrorMethod()
{//{{{
    bool is_update_status = false;
    double current_time = sc_time_stamp().to_double();
    unsigned int mod_address = 0;
    unsigned int edl_code = 0;

    // Update status bit
    mHas2bitError = false;
    mHasAddrFeedbackError = false;
    for (unsigned int portindex = 0; portindex <= HIGHEST_PORT_INDEX; portindex++){// With fix priority
        if (is_update_status){
            break;
        }
        for (unsigned int i = 0; i < mSizeFatalErrorList; i++){// Scan list to find high priority
            // get highest priority to update status flag
            if (mFatalErrorList[i].edl_code == portindex){
                if (CheckNoFatalStatus()){
                    mHas2bitError |= (mFatalErrorList[i].type == em2bitError);
                    mHasAddrFeedbackError |= (mFatalErrorList[i].type == emAddressFeedbackError);
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
                mDEDERROUT_CR |= true;
            }
            if (mFatalErrorList[i].type == emAddressFeedbackError){
                mAFEERROUT_CR |= true;
            }
        }
    }
    if (mDEDERROUT_CR || mAFEERROUT_CR){
        mNotifyFatalErrorEvent.notify(CalculateCLKPosEdge((double) mClkPeriod[empclkId], false, current_time, mStartClkTime[empclkId]) + mClkPeriod[empclkId], mTimeResolutionUnit);
    }

    // Clear mFatalErrorList
    mSizeFatalErrorList = 0;
}//}}}

/// Set fatal status bit
void ECC_CRAM_E2::SetFatalErrorStatusMethod()
{//{{{
    if (mHas2bitError){
        mpMECCCAP_CRFunc->Set2bitErrorStatus();
        mpMECCCAP_CRFunc->SetFatalAddress(mFatalEDLCode, mFatalErrorModAddr);
    }
    if (mHasAddrFeedbackError){
        mpMECCCAP_CRFunc->SetAddrFeedbackErrorStatus();
        mpMECCCAP_CRFunc->SetFatalAddress(mFatalEDLCode, mFatalErrorModAddr);
    }
}//}}}

/// Notify fatal error
void ECC_CRAM_E2::NotifyFatalErrorMethod()
{//{{{
    unsigned int errint = mpMECCCAP_CRFunc->GetERRINT();
    unsigned int dedie = (errint  >> 1) & 0x1;
    unsigned int afeie = (errint >> 3) & 0x1;
    if (mDEDERROUT_CR && (dedie == 1)){
        mWriteDEDERROUT_CREvent.notify();
    }else{
        mDEDERROUT_CR = false;
    }
    if (mAFEERROUT_CR && (afeie == 1)){
        mWriteAFEERROUT_CREvent.notify();
    }else{
        mAFEERROUT_CR = false;
    }
}//}}}

/// Negate dederrout_CR port
void ECC_CRAM_E2::NegateDEDERROUT_CRMethod()
{//{{{
    mDEDERROUT_CR = false;
    mWriteDEDERROUT_CREvent.notify();
}//}}}

/// Negate afeerrout_CR port
void ECC_CRAM_E2::NegateAFEERROUT_CRMethod()
{//{{{
    mAFEERROUT_CR = false;
    mWriteAFEERROUT_CREvent.notify();
}//}}}

/// Write dederrout_CR port
void ECC_CRAM_E2::WriteDEDERROUT_CRMethod()
{//{{{
    dederrout_CR.write(mDEDERROUT_CR);
    if (mDumpInterrupt){
        re_printf("info", "dederrout_CR is changed to %d\n", (unsigned int)mDEDERROUT_CR);
    }
    if (mDEDERROUT_CR){
        mNegateDEDERROUT_CREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Write afeerrout_CR port
void ECC_CRAM_E2::WriteAFEERROUT_CRMethod()
{//{{{
    afeerrout_CR.write(mAFEERROUT_CR);
    if (mDumpInterrupt){
        re_printf("info", "afeerrout_CR is changed to %d\n", (unsigned int)mAFEERROUT_CR);
    }
    if (mAFEERROUT_CR){
        mNegateAFEERROUT_CREvent.notify((double)mClkPeriod[empclkId], mTimeResolutionUnit);
    }
}//}}}

/// Check no fatal status
bool ECC_CRAM_E2::CheckNoFatalStatus()
{//{{{
    return mpMECCCAP_CRFunc->CheckNoFatalStatus();
}//}}}

/// Check an address is masked or not
bool ECC_CRAM_E2::CheckMaskAddress( const unsigned int address)
{//{{{
    for (unsigned int i = 0; i < emNumMaskedArea; i++){
        unsigned int masked_enable = mpMECCCAP_CRFunc->GetERM_EN(i);
        if (masked_enable != 0){
            unsigned int ermsa = mpMECCCAP_CRFunc->GetERMSA(i);
            unsigned int ermea = mpMECCCAP_CRFunc->GetERMEA(i);
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

/// Check this address is captured in 1bit address register buffer
bool ECC_CRAM_E2::Check1bitAddressNotCaptured(unsigned int mod_address)
{//{{{ 
    for (unsigned int i = 0; i < mCAP_DEPTH; i++){
        unsigned int cap_addr = mpMECCCAP_CRFunc->GetSEADRBit(i);
        unsigned int serstr = mpMECCCAP_CRFunc->GetCR_SERSTR();
        if ((cap_addr == mod_address) && (((serstr >> i) & 0x1) == 1)){
            return false;
        }
    }
    return true;
}//}}}

/// Update 1bit error list
void ECC_CRAM_E2::Update1bitErrorList()
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
void ECC_CRAM_E2::UpdateFatalErrorList()
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
std::string ECC_CRAM_E2::CombineStringUint(std::string name, unsigned int index)
{//{{{
    std::ostringstream ret_string;
    ret_string.str("");
    ret_string << name << index;
    return ret_string.str();
}//}}}

/// Shift address to corresponding with MSB/LSB
unsigned int ECC_CRAM_E2::ShiftAddress(unsigned int address)
{//{{{
    unsigned int mod_addr = ((address & ((unsigned int)(1 << (mAWIDTH_MSB + 1)) - 1))>>mAWIDTH_LSB) << (mAWIDTH_LSB - MIN_AWIDTH_LSB);
    return mod_addr;
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void ECC_CRAM_E2::SetMessageLevel (const std::string msg_lv)
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
            mpECCCNT_CRCFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CRAFunc->RegisterHandler(cmd);
        mpMECCCAP_CRFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_CRCFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CRAFunc->RegisterHandler(cmd);
        mpMECCCAP_CRFunc->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_CRCFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CRAFunc->RegisterHandler(cmd);
        mpMECCCAP_CRFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        for (unsigned int i = 0; i < PE_NUM; i++){
            mpECCCNT_CRCFunc[i]->RegisterHandler(cmd);
        }
        mpECCCNT_CRAFunc->RegisterHandler(cmd);
        mpMECCCAP_CRFunc->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void ECC_CRAM_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_CRCFunc[i]->RegisterHandler(cmd);
    }
    mpECCCNT_CRAFunc->RegisterHandler(cmd);
    mpMECCCAP_CRFunc->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt information
void ECC_CRAM_E2::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_CRAM_E2_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_CRAM_E2_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_CRAM_E2_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Set AWIDTH_MSB
void ECC_CRAM_E2::AWIDTH_MSB(const unsigned int value)
{//{{{
    if ((mAWIDTH_LSB <= value) && (value <= MAX_ADDWIDTH_MSB)){
        mAWIDTH_MSB = value;
    }else{
        re_printf("warning", "Invalid AWIDTH_MSB (%d) of the ECC_CRAM_E2_AWIDTH_MSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, mAWIDTH_LSB, MAX_ADDWIDTH_MSB, DEFAULT_AWIDTH_MSB);
    }
}//}}}

/// Set AWIDTH_LSB
void ECC_CRAM_E2::AWIDTH_LSB(const unsigned int value)
{//{{{
    if ((MIN_ADDWIDTH_LSB <= (int)value) && (value <= mAWIDTH_MSB)){
        mAWIDTH_LSB = value;
    }else{
        re_printf("warning", "Invalid AWIDTH_LSB (%d) of the ECC_CRAM_E2_AWIDTH_LSB parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ADDWIDTH_LSB, mAWIDTH_MSB, DEFAULT_AWIDTH_LSB);
    }
}//}}}

/// Set CAP_DEPTH
void ECC_CRAM_E2::CAP_DEPTH(const unsigned int value)
{//{{{
    if ((MIN_CAP_DEPTH <= value) && (value <= MAX_CAP_DEPTH)){
        mCAP_DEPTH = value;
    }else{
        re_printf("warning", "Invalid CAP_DEPTH (%d) of the ECC_CRAM_E2_CAP_DEPTH parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_CAP_DEPTH, MAX_CAP_DEPTH, DEFAULT_CAP_DEPTH);
    }
}//}}}

/// Set ERRINT_INIT
void ECC_CRAM_E2::ERRINT_INIT(const unsigned int value)
{//{{{
    if ((MIN_ERRINT_INIT <= (int)value) && (value <= MAX_ERRINT_INIT)){
        mERRINT_INIT = value;
    }else{
        re_printf("warning", "Invalid ERRINT_INIT (%d) of the ECC_CRAM_E2_ERRINT_INIT parameter. Valid range is %d-%d. Value %d is set as default.\n", value, MIN_ERRINT_INIT, MAX_ERRINT_INIT, DEFAULT_ERRINT_INIT);
    }
}//}}}

/// Issue error to ECC
void ECC_CRAM_E2::IssueError(const unsigned int errortype)
{//{{{
    // Not receive error when reset/clock stop
    if (GetResetStatus("sys_resetz") || (!CheckClockPeriod("sys_clk")) || (!CheckClockPeriod("pclk"))){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    if ((errortype == em1bitError) || (errortype == em2bitError) || (errortype == emAddressFeedbackError)){
        if ((mIsEDLConfig) && (mIsAddrConfig)){
            mErrorType = (eErrorType)errortype;
            std::string edl_name = CalculateEDLCode();
            re_printf("info", "Error input Type: %d, EDL: %s, Address: 0x%08X\n", errortype, edl_name.c_str(), mErrorAddress);
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
        re_printf("warning", "Invalid error type (%d) of ECC_CRAM_E2_IssueError command.\n", errortype);
    }
}//}}}

/// Config error address
void ECC_CRAM_E2::SetErrorAddr(const unsigned int addr_value)
{//{{{
    mErrorAddress = addr_value;
    mIsAddrConfig = true;
}//}}}

/// Calculate EDL code
std::string ECC_CRAM_E2::CalculateEDLCode()
{//{{{
    std::string edl_name = "";
    edl_name = CombineStringUint(mStrEDL, mPeId);
    std::map<std::string, unsigned int>::iterator it;
    for (it = mEDLmap.begin(); it != mEDLmap.end(); it++){
        if ((it->first) == edl_name){
            mErrorEDL = it->second;
        }
    }
    return edl_name;
}//}}}

/// Config error detection location (EDL)
void ECC_CRAM_E2::SetEDL (const std::string edl, const unsigned int pe_index)
{//{{{
    std::string edl_name = CombineStringUint(edl, pe_index);
    std::map<std::string, unsigned int>::iterator it;

    // Update EDL if any
    for (it = mEDLmap.begin(); it != mEDLmap.end(); it++){
        if ((it->first) == edl_name){
            mStrEDL = edl;
            mPeId = pe_index;
            mIsEDLConfig = true;
        }
    }
    
    // Dump warning message if config wrong EDL
    if (!mIsEDLConfig){
        re_printf("warning", "Invalid EDL (%s, %d) of ECC_CRAM_E2_SetEDL command.\n", edl.c_str(), pe_index);
    }
}//}}}

/// Dump Status info
void ECC_CRAM_E2::DumpStatusInfo()
{//{{{
    const char *type = "StatInfo";
    DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
    DumpInfo(type, "  CR_SERSTR: 0x%08X\n", mpMECCCAP_CRFunc->GetCR_SERSTR());
    DumpInfo(type, "  CR_DERSTR: 0x%08X\n", mpMECCCAP_CRFunc->GetCR_DERSTR());
    DumpInfo(type, "  CR_OVFSTR: 0x%08X\n", mpMECCCAP_CRFunc->GetCR_OVFSTR());
    for (unsigned int i = 0; i < mCAP_DEPTH; i++){
        unsigned int sedl = mpMECCCAP_CRFunc->GetSEDLBit(i);
        unsigned int seadr = mpMECCCAP_CRFunc->GetSEADRBit(i) >> (mAWIDTH_LSB - MIN_AWIDTH_LSB);
        DumpInfo(type, "  CR_%2dSEADR: SEDL = 0x%02X, SEADR = 0x%08X\n", i, sedl, seadr);
    }
    unsigned int dedl = mpMECCCAP_CRFunc->GetDEDLBit();
    unsigned int deadr = mpMECCCAP_CRFunc->GetDEADRBit() >> (mAWIDTH_LSB - MIN_AWIDTH_LSB);
    DumpInfo(type, "  CR_00DEADR: DEDL = 0x%02X, DEADR = 0x%08X\n", dedl, deadr);
}//}}}

/// Common dump info message
void ECC_CRAM_E2::DumpInfo(const char *type, const char *message, ...)
{//{{{
    if ((message != NULL) && (type != NULL)){
        printf("PROFILE(%s): ECC_CRAM_E2: ", type);
        // Print body
        va_list argptr;
        va_start(argptr, message);
        vprintf(message, argptr);
        va_end(argptr);
    }
}//}}}

/// Command to assert reset
void ECC_CRAM_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
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
void ECC_CRAM_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
void ECC_CRAM_E2::GetCLKFreq (const std::string clock_name)
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
void ECC_CRAM_E2::ForceRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_CRC"){
                if (block_index < PE_NUM){
                    mpECCCNT_CRCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CRA"){
                mpECCCNT_CRAFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_CR"){
                mpMECCCAP_CRFunc->RegisterHandler(cmd);
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
void ECC_CRAM_E2::ReleaseRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
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
            if (block_name == "ECCCNT_CRC"){
                if (block_index < PE_NUM){
                    mpECCCNT_CRCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CRA"){
                mpECCCNT_CRAFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_CR"){
                mpMECCCAP_CRFunc->RegisterHandler(cmd);
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
void ECC_CRAM_E2::WriteRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name, const unsigned int reg_value)
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
            if (block_name == "ECCCNT_CRC"){
                if (block_index < PE_NUM){
                    mpECCCNT_CRCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CRA"){
                mpECCCNT_CRAFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_CR"){
                mpMECCCAP_CRFunc->RegisterHandler(cmd);
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
void ECC_CRAM_E2::ReadRegister (const std::string block_name, const unsigned int block_index, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if (block_name == "ECCCNT_CRC"){
                if (block_index < PE_NUM){
                    mpECCCNT_CRCFunc[block_index]->RegisterHandler(cmd);
                }else{
                    re_printf("warning","Block index (%d) is invalid.\n", block_index);
                }
            }else if (block_name == "ECCCNT_CRA"){
                mpECCCNT_CRAFunc->RegisterHandler(cmd);
            }else if (block_name == "MECCCAP_CR"){
                mpMECCCAP_CRFunc->RegisterHandler(cmd);
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
void ECC_CRAM_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    for (unsigned int i = 0; i < PE_NUM; i++){
        mpECCCNT_CRCFunc[i]->RegisterHandler(cmd);
    }
    mpECCCNT_CRAFunc->RegisterHandler(cmd);
    mpMECCCAP_CRFunc->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands

void ECC_CRAM_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_MessageLevel (\"ECC_CRAM_E2 instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_DumpRegisterRW (\"ECC_CRAM_E2 instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_DumpInterrupt (\"ECC_CRAM_E2 instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_AWIDTH_MSB (\"ECC_CRAM_E2 instance\", value)                                                   Set address bit width MSB (Default: 19). Valid range to be set: 7-26");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_AWIDTH_LSB (\"ECC_CRAM_E2 instance\", value)                                                   Set address bit width LSB (Default: 2). Valid range to be set: 2-4");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_CAP_DEPTH (\"ECC_CRAM_E2 instance\", value)                                                    Set number of address registers (Default: 8). Valid range to be set: 1-32");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_ERRINT_INIT (\"ECC_CRAM_E2 instance\", value)                                                  Set initial value of ERRINT register (Default: 0x8F). Valid range to be set: 0x0-0xFF");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_EnableRegisterMessage (\"ECC_CRAM_E2 instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_IssueError (\"ECC_CRAM_E2 instance\", error_type)                                              Issue pseudo error to the ECC_CRAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_SetErrorAddr (\"ECC_CRAM_E2 instance\", addr_value)                                            Set error detection address");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_SetEDL (\"ECC_CRAM_E2 instance\", \"edl\", pe_index)                                           Set error detection location");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_DumpStatusInfo (\"ECC_CRAM_E2 instance\")                                                      Dump information of the error status registers of the ECC_CRAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_AssertReset (\"ECC_CRAM_E2 instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the ECC_CRAM_E2 model");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_SetCLKFreq (\"ECC_CRAM_E2 instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to ECC_CRAM_E2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_GetCLKFreq (\"ECC_CRAM_E2 instance\", \"clock_name\")                                          Get clock frequency value of ECC_CRAM_E2 model. Valid clock_name is pclk or sys_clk");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_ForceRegister (\"ECC_CRAM_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_ReleaseRegister (\"ECC_CRAM_E2 instance\", \"block_name\", block_index, \"reg_name\")          Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_WriteRegister (\"ECC_CRAM_E2 instance\", \"block_name\", block_index, \"reg_name\", reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_ReadRegister (\"ECC_CRAM_E2 instance\", \"block_name\", block_index, \"reg_name\")             Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_CRAM_E2_ListRegister (\"ECC_CRAM_E2 instance\")                                                        Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_CRAM_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_CRAM_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}
void ECC_CRAM_E2::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: ECC_CRAM_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        std::string temp = "false";
        if(mEnableRegisterMessage){
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","ECC_CRAM_E2_EnableRegisterMessage %s\n", temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_CRAM_E2_EnableRegisterMessage %s.\n", is_enable.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
