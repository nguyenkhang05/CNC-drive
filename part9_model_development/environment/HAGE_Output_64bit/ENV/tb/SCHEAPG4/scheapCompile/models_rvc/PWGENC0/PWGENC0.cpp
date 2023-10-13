// ---------------------------------------------------------------------
// $Id: PWGENC0.cpp,v 1.37 2020/01/15 13:23:55 landang Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "PWGENC0.h"
#include "PWGENC0_Func.h"

/// Constructor and Destructor
PWGENC0::PWGENC0(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,PWGENC0_AgentController()
        ,rvc_common_model()
        ,PCLK("PCLK")
        ,PCLKRW("PCLKRW")
        ,PRESETZ("PRESETZ")
        ,PWG_STE("PWG_STE")
        ,PWG_PERIOD("PWG_PERIOD")
        ,PWG_INTR("PWG_INTR")
        ,PWG_INTF("PWG_INTF")
        ,PWG_TRGOUT("PWG_TRGOUT")
        ,PWG_TOUT("PWG_TOUT")
{//{{{ 
    // Initialize ports
    PWG_INTR.initialize(false);
    PWG_INTF.initialize(false);
    PWG_TRGOUT.initialize(false);
    PWG_TOUT.initialize(false);
    // New instance and port
    mpPWGENC0Func = new PWGENC0_Func((std::string)name, this);
    for (unsigned int i = 0; i < emNumClkOut; i++){
        std::ostringstream port_name;
        port_name.str("");
        port_name << "PWG_CKI" << i;
        PWG_CKI[i] = new sc_in<sc_dt::uint64>(port_name.str().c_str());
        sc_assert(PWG_CKI[i] != NULL);
    }
    
    // Configure Slave socket
    BusSlaveBase<32, 1>::setSlaveResetPort32(&PRESETZ);
    BusSlaveBase<32, 1>::setSlaveFreqPort32(&PCLKRW);
    BusSlaveBase<32, 1>::setTargetSocket32("ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0];
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setFuncModulePtr(mpPWGENC0Func);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setWriteLatency(wLatency);

    // Initialize variables
    mIsPRESETZInit = true;
    mPCLKPeriod = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreq = 0;
    mPCLKFreqUnit = "Hz";
    mPCLKRWPeriod = 0;
    mPCLKRWOrgFreq = 0;
    mPCLKRWFreq = 0;
    mPCLKRWFreqUnit = "Hz";
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod = 0;

    for (unsigned int i = 0;i < emNumClkOut; i++){
        mPWG_CKI[i] = 0;
    }
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mStartPCLKTime = 0;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mDumpInterrupt = false;
    mEnableRegisterMessage = true;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorPCLKMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(MonitorPCLKRWMethod);
    dont_initialize();
    sensitive << PCLKRW;

    SC_METHOD(MonitorPRESETZMethod);
    sensitive << PRESETZ;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(CmpMatchCRBRMethod);
    dont_initialize();
    sensitive << mCmpMatchCRBREvent;

    SC_METHOD(CmpMatchCSBRMethod);
    dont_initialize();
    sensitive << mCmpMatchCSBREvent;

    SC_METHOD(CmpMatchCTBRMethod);
    dont_initialize();
    sensitive << mCmpMatchCTBREvent;

    SC_METHOD(CmpMatchPERIODMethod);
    dont_initialize();
    sensitive << mCmpMatchPERIODEvent;

    SC_METHOD(LoadTCBRMethod);
    dont_initialize();
    sensitive << mLoadTCBREvent;

    SC_METHOD(MonitorPWG_STEMethod);
    dont_initialize();
    sensitive << PWG_STE;

    SC_METHOD(MonitorPWG_CKIMethod);
    dont_initialize();
    for (unsigned int i = 0; i < emNumClkOut; i++){
        sensitive << *PWG_CKI[i];
    }
    sensitive << mCheckPWG_CKIEvent; // in case CKI is fix 1

    SC_METHOD(MonitorPWG_PERIODMethod);
    dont_initialize();
    sensitive << PWG_PERIOD;
    
    SC_METHOD(UpdateRSFBitMethod);
    dont_initialize();
    sensitive << mUpdateRSFBitEvent;
    
    SC_METHOD(UpdateRSFTBitMethod);
    dont_initialize();
    sensitive << mUpdateRSFTBitEvent;
    
    SC_METHOD(UpdateFOFBitMethod);
    dont_initialize();
    sensitive << mUpdateFOFBitEvent;

    SC_METHOD(WritePWG_TOUTMethod);
    dont_initialize();
    sensitive << mWritePWG_TOUTEvent;

    SC_METHOD(WritePWG_TRGOUTMethod);
    dont_initialize();
    sensitive << mWritePWG_TRGOUTEvent;

    SC_METHOD(WritePWG_INTRMethod);
    dont_initialize();
    sensitive << mWritePWG_INTREvent;

    SC_METHOD(WritePWG_INTFMethod);
    dont_initialize();
    sensitive << mWritePWG_INTFEvent;
}//}}} 

/// Destructor
PWGENC0::~PWGENC0()
{//{{{
    delete mpPWGENC0Func;
}//}}}

/// Initialize when reset activated
void PWGENC0::Initialize(void)
{//{{{
    mIsResetRSFBit = 0;
    mIsResetRSFTBit = 0;
    mIsResetFOFBit = 0;
    mIsSetRSFBit = 0;
    mIsSetRSFTBit = 0;
    mIsSetFOFBit = 0;
    mIsClearRSFBit = 0;
    mIsClearRSFTBit = 0;
    mIsClearFOFBit = 0;
    mRealTOUT = 0;
    mPWG_TOUT = 0;
    mPWG_TRGOUT = 0;
    mPWG_INTR = 0;
    mPWG_INTF = 0;
    mEnableCounting = 0;
    mIsSuspendCounting  = 0;
    mResumeCounterTime = 0;
    mCounter = 0;
    mPERIOD = 0;
    mPWG_STE = 0;
    mReloadCounterTime = 0;
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Get reset status
bool PWGENC0::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock is valid or not
bool PWGENC0::CheckClockPeriod(std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == PCLK.basename()) {
        clock_period = mPCLKPeriod;
    }else{// (clock_name == PCLKRW.basename()) 
        clock_period = mPCLKRWPeriod;
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

/// Notify event to clear TOUT port
void PWGENC0::ClearTOUTPort()
{//{{{
    mRealTOUT = 0;
    mPWG_TOUT = 0;
    double current_time = sc_time_stamp().to_double();
    mWritePWG_TOUTEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
}//}}}

/// Force PWG_TOUT to a specific value (FOS bit)
void PWGENC0::ForceTOUT(bool fos)
{//{{{
    mPWG_TOUT = fos;
    double current_time = sc_time_stamp().to_double();
    mWritePWG_TOUTEvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
}//}}}

/// Update PWG0TCBR after 1 PCLK
void PWGENC0::LoadTCBREventAfter1PCLK()
{//{{{
    double current_time = sc_time_stamp().to_double();
    mLoadTCBREvent.notify(CalculateCLKPosEdge(mPCLKPeriod, false, current_time, mStartPCLKTime) + mPCLKPeriod, mTimeResolutionUnit);
}//}}}

/// Request set RSF bit as priority
void PWGENC0::RequestSetRSFBit()
{//{{{
    mIsSetRSFBit = true;
    mUpdateRSFBitEvent.notify(SC_ZERO_TIME);
}//}}}

/// Request set RSFT bit as priority
void PWGENC0::RequestSetRSFTBit()
{//{{{
    mIsSetRSFTBit = true;
    mUpdateRSFTBitEvent.notify(SC_ZERO_TIME);
}//}}}

/// Request set FOF bit as priority
void PWGENC0::RequestSetFOFBit()
{//{{{
    mIsSetFOFBit = true;
    mUpdateFOFBitEvent.notify(SC_ZERO_TIME);
}//}}}

/// Request clear RSF bit as priority
void PWGENC0::RequestClearRSFBit()
{//{{{
    mIsClearRSFBit = true;
    mUpdateRSFBitEvent.notify(SC_ZERO_TIME);
}//}}}

/// Request clear RSFT bit as priority
void PWGENC0::RequestClearRSFTBit()
{//{{{
    mIsClearRSFTBit = true;
    mUpdateRSFTBitEvent.notify(SC_ZERO_TIME);
}//}}}

/// Request clear FOF bit as priority
void PWGENC0::RequestClearFOFBit()
{//{{{
    mIsClearFOFBit = true;
    mUpdateFOFBitEvent.notify(SC_ZERO_TIME);
}//}}}

/// Enable/Disable counter
void PWGENC0::EnableCounter(bool is_enable)
{//{{{
    if (GetResetStatus() || (!CheckClockPeriod("PCLK"))){
        return;
    }
    if (is_enable){
        if (mPERIOD == 0){
            re_printf("warning", "PWG_PERIOD is required to larger than 0.\n");
            return;
        }
        mpPWGENC0Func->UpdateTEBit(1);
        if (!mEnableCounting){
            mpPWGENC0Func->LoadCounterValue();
            mpPWGENC0Func->LoadTCBR();
            mEnableCounting = 1;
            mIsResetRSFBit = true; // request reset RSF as priority
            mUpdateRSFBitEvent.notify(SC_ZERO_TIME); 
            mIsResetRSFTBit = true; // request reset RSFT as priority
            mUpdateRSFTBitEvent.notify(SC_ZERO_TIME);
            mCounter = 0;
            // Operating when selected CKI > 0, and mEnableCounting = 1
            unsigned int cks = (mpPWGENC0Func->GetPWG0CTLReg()) & 0x3;
            if (mPWG_CKI[cks] > 0){
                CountToPERIOD(0);
            }
        }
    }else{
        mpPWGENC0Func->UpdateTEBit(0);
        ClearTOUTPort();
        mpPWGENC0Func->SetPWG0STReg(0);
    }
    mIsResetFOFBit = true; // request reset FOF as priority
    mUpdateFOFBitEvent.notify(SC_ZERO_TIME);
}//}}}

/// Get value of PWG_PERIOD port
unsigned int PWGENC0::GetPERIOD()
{//{{{
    return mPERIOD;
}//}}}

/// Update current counter from written value to CNT
void PWGENC0::UpdateCurrentCounter(unsigned int value)
{//{{{
    mCounter = value;
}//}}}

/// Get current counter and set to CNT register
unsigned int PWGENC0::GetCurrentCounter() 
{//{{{
    unsigned int cks = (mpPWGENC0Func->GetPWG0CTLReg()) & 0x3;
    unsigned int ret = 0;
    if ((mEnableCounting) && (mPWG_CKI[cks] > 0)){// counter is counting
        if (mResumeCounterTime > 0){// counting after resume
            double period = sc_time_stamp().to_double() - mResumeCounterTime;
            ret = (unsigned int)((sc_dt::uint64)period/mPWG_CKI[cks]) + mCounter;
        }else{// normal case
            double period = sc_time_stamp().to_double() - mReloadCounterTime;
            ret = (unsigned int)((sc_dt::uint64)period/mPWG_CKI[cks]);
        }
    }else{
        ret = mCounter;
    }
    return ret%(mPERIOD + 1);
}//}}}

/// Get value of PWG_STE port
bool PWGENC0::GetPWG_STE()
{//{{{
    return mPWG_STE;
}//}}}

/// Check counter is running or not
bool PWGENC0::CheckCounterRunning()
{//{{{
    return mEnableCounting;
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor PCLK clock
void PWGENC0::MonitorPCLKMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

/// Monitor PCLKRW clock
void PWGENC0::MonitorPCLKRWMethod()
{//{{{
    sc_dt::uint64 freq_value = PCLKRW.read();
    SetCLKFreq("PCLKRW", freq_value, "Hz");
}//}}}

/// Monitor PRESETZ port
void PWGENC0::MonitorPRESETZMethod()
{//{{{
    bool reset = PRESETZ.read();
    if (reset == vpcl::emResetActive){
        mIsPRESETZInit = false;
        mIsResetHardActive = true;
        re_printf("info", "The reset port PRESETZ is asserted.\n");
        EnableReset(true);
    }else{
        if (mIsPRESETZInit){
            mIsPRESETZInit = false;
        }else{
            mIsResetHardActive = false;
            re_printf("info", "The reset port PRESETZ is de-asserted.\n");
            EnableReset(false);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

/// Process reset function when reset command is active
void PWGENC0::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETZ.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void PWGENC0::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETZ is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Reset model and its sub-instance if reset is actived
void PWGENC0::EnableReset (const bool is_active)
{//{{{
    mpPWGENC0Func->EnableReset(is_active);
    if(is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        mWritePWG_TOUTEvent.notify();
        mWritePWG_TRGOUTEvent.notify();
        mWritePWG_INTREvent.notify();
        mWritePWG_INTFEvent.notify();
    }
}//}}}

/// Cancel events
void PWGENC0::CancelEvents()
{//{{{
    mLoadTCBREvent.cancel();
    mCheckPWG_CKIEvent.cancel();
    mUpdateRSFBitEvent.cancel();
    mUpdateRSFTBitEvent.cancel();
    mUpdateFOFBitEvent.cancel();
    mWritePWG_TOUTEvent.cancel();
    mWritePWG_TRGOUTEvent.cancel();
    mWritePWG_INTREvent.cancel();
    mWritePWG_INTFEvent.cancel();
    mCmpMatchCRBREvent.cancel();
    mCmpMatchCSBREvent.cancel();
    mCmpMatchCTBREvent.cancel();
    mCmpMatchPERIODEvent.cancel();
}//}}}

//========================================================
//============ Operation               ===================
//========================================================
/// Call PWGENC0_Func to update PWG0TCBR
void PWGENC0::LoadTCBRMethod()
{//{{{
    mpPWGENC0Func->LoadTCBR();
}//}}}

/// Monitor PWG_STE port
void PWGENC0::MonitorPWG_STEMethod()
{//{{{
    if (GetResetStatus() || (!CheckClockPeriod("PCLK"))){
        return;
    }

    mPWG_STE = PWG_STE.read();
    EnableCounter(mPWG_STE);
}//}}}

/// Monitor PWG_CKI ports and event to count up
void PWGENC0::MonitorPWG_CKIMethod()
{//{{{
    if (GetResetStatus() || (!CheckClockPeriod("PCLK"))){
        return;
    }

    // backup previous clock
    sc_dt::uint64 pre_cki[4];
    for (unsigned int i = 0; i < emNumClkOut; i++){
        pre_cki[i] = mPWG_CKI[i];
    }
    // Get new clock
    for (unsigned int i = 0; i < emNumClkOut; i++){
        mPWG_CKI[i] = PWG_CKI[i]->read();
    }
    // Operating when selected CKI > 0, and mEnableCounting = 1
    unsigned int cks = (mpPWGENC0Func->GetPWG0CTLReg()) & 0x3;
    if (mEnableCounting){
        if (mPWG_CKI[cks] > 0){
            if (mIsSuspendCounting){// Check counter is in suspend state due to clock = 0
                if (mResumeCounterTime == 0){// only process resume for first supplied clock
                    CountToPERIOD(mCounter);// resume counting from mCounter when clock is supplied again
                }
            }else{
                if (mReloadCounterTime == 0){
                    CountToPERIOD(0);
                }
            }
        }else{
            mCmpMatchCRBREvent.cancel();// cancel counting
            mCmpMatchCSBREvent.cancel();
            mCmpMatchCTBREvent.cancel();
            mCmpMatchPERIODEvent.cancel();
            mPWG_CKI[cks] = pre_cki[cks];// calculate current counter with previous clock
            mCounter = GetCurrentCounter();
            mPWG_CKI[cks] = PWG_CKI[cks]->read();// update current clock again
            mpPWGENC0Func->SetPWG0CNTReg(mCounter);
            mIsSuspendCounting = true; // inform that counter is in suspend state
            mResumeCounterTime = 0; // clear time resume when clock = 0, for case 2 resume times in 1 counting
        }
    }
}//}}}

/// Count from current counter to PWG_PERIOD and process compare match CRBR, CSBR, CTBR
void PWGENC0::CountToPERIOD(unsigned int current_counter)
{//{{{
    unsigned int csbr = mpPWGENC0Func->GetPWG0CSBRReg();
    unsigned int crbr = mpPWGENC0Func->GetPWG0CRBRReg();
    unsigned int ctbr = mpPWGENC0Func->GetPWG0CTBRReg();
    unsigned int cks = (mpPWGENC0Func->GetPWG0CTLReg()) & 0x3;

    // Notify event to process match CRBR, CSBR, CTBR, PWG_PERIOD
    if (current_counter > 0){
        mResumeCounterTime = sc_time_stamp().to_double();// variable is used to calculate counter when read CNT after resume
        if (crbr > mPERIOD){
            mCmpMatchCRBREvent.notify((double)((mPERIOD - current_counter) * mPWG_CKI[cks]), mTimeResolutionUnit);
        }else{
            if (crbr > current_counter){
                mCmpMatchCRBREvent.notify((double)((crbr - current_counter) * mPWG_CKI[cks]), mTimeResolutionUnit);
            }
        }
        if (csbr > current_counter){
            mCmpMatchCSBREvent.notify((double)((csbr-current_counter) * mPWG_CKI[cks]), mTimeResolutionUnit);
        }
        if (ctbr > current_counter){
            mCmpMatchCTBREvent.notify((double)((ctbr-current_counter) * mPWG_CKI[cks]), mTimeResolutionUnit);
        }
        mCmpMatchPERIODEvent.notify((double)((mPERIOD + 1 - current_counter) * mPWG_CKI[cks]), mTimeResolutionUnit);
        
    }else{
        mReloadCounterTime = sc_time_stamp().to_double();// variable is used to calculate counter value when read CNT
        if (crbr > mPERIOD){
            mCmpMatchCRBREvent.notify((double)((mPERIOD) * mPWG_CKI[cks]), mTimeResolutionUnit);
        }else{
            mCmpMatchCRBREvent.notify((double)((crbr) * mPWG_CKI[cks]), mTimeResolutionUnit);
        }
        mCmpMatchCSBREvent.notify((double)((csbr) * mPWG_CKI[cks]), mTimeResolutionUnit);
        mCmpMatchCTBREvent.notify((double)((ctbr) * mPWG_CKI[cks]), mTimeResolutionUnit);
        mCmpMatchPERIODEvent.notify((double)((mPERIOD + 1) * mPWG_CKI[cks]), mTimeResolutionUnit);
    }

}//}}}

/// Control PWG_TOUT, and PWG_INTF, reload counter, when CRBR is matched with counter
void PWGENC0::CmpMatchCRBRMethod()
{//{{{
    unsigned int csbr = mpPWGENC0Func->GetPWG0CSBRReg();
    unsigned int crbr = mpPWGENC0Func->GetPWG0CRBRReg();
    unsigned int st = mpPWGENC0Func->GetPWG0STReg();
    unsigned int ctl = mpPWGENC0Func->GetPWG0CTLReg();
    unsigned int rsf = st & 0x1;
    unsigned int rsft = (st >> 1) & 0x1;
    unsigned int fof = (st >> 8) & 0x1;
    unsigned int tcut = (ctl >> 4) & 0x3;
    unsigned int tfie = (ctl >> 15) & 0x1;
    // Control interrupt
    if (tfie == 1){// Issue interrupt if any
        if ((crbr == csbr) || (crbr == 0) || (crbr > mPERIOD) || (mRealTOUT == 1)){// not need check change from 1 to 0 in case crbr = 0, or > PERIOD, if not, need to check TOUT change from 1 to 0
            mPWG_INTF = 1;
            mWritePWG_INTFEvent.notify();
        }
    }

    // Reload compare counter
    if ((mRealTOUT == 1) && (rsf == 1)){
        mpPWGENC0Func->LoadCounterValue();// Load CTBR, CRBR, CSBR
        RequestClearRSFBit(); // Clear RSF as priority reset > set > clear
    }
    if ( (mRealTOUT == 1) && (tcut == 2) && (rsft == 1) ){// Reload TCBR
        mpPWGENC0Func->LoadTCBR();
        RequestClearRSFTBit(); // Clear RSFT as priority reset > set > clear
    }

    // Update PWG_TOUT only when CRBR < PWG_PERIOD
    if (crbr < mPERIOD){
        if (fof == 0){ // Write PWG_TOUT if not in force
            mPWG_TOUT = 0;
            mWritePWG_TOUTEvent.notify();
        }
        mRealTOUT = 0;
        if (mpPWGENC0Func->GetTEBit() == 0){// Disable counting if TE = 1 (after finish falling edge TOUT)
            mCounter = GetCurrentCounter();
            mpPWGENC0Func->SetPWG0CNTReg(mCounter); // update CNT before stop
            mEnableCounting = false;
            mIsSuspendCounting = false;
            mResumeCounterTime = 0;
            mReloadCounterTime = 0;
        }
    }
}//}}}

/// Control PWG_TOUT, and PWG_INTF, reload counter, when CRBR is matched with counter
void PWGENC0::CmpMatchCSBRMethod()
{//{{{
    unsigned int csbr = mpPWGENC0Func->GetPWG0CSBRReg();
    unsigned int crbr = mpPWGENC0Func->GetPWG0CRBRReg();
    unsigned int st = mpPWGENC0Func->GetPWG0STReg();
    unsigned int ctl = mpPWGENC0Func->GetPWG0CTLReg();
    unsigned int rsft = (st >> 1) & 0x1;
    unsigned int tcut = (ctl >> 4) & 0x3;
    unsigned int trie = (ctl >> 14) & 0x1;
    // Control interrupt
    if (trie == 1){// Issue interrupt if any
        if ((crbr == csbr) || (csbr == 0) || (crbr > mPERIOD) || (mRealTOUT == 0)){// not need check change from 1 to 0 in case csbr = 0,if not, need to check TOUT change from 0 to 1
            mPWG_INTR = 1;
            mWritePWG_INTREvent.notify();
        }
    }

    // Reload compare counter
    if (crbr != csbr){
        if ( (mRealTOUT == 0) && (tcut == 1) && (rsft == 1) ){// Reload TCBR
            mpPWGENC0Func->LoadTCBR();
            RequestClearRSFTBit(); // Clear RSFT as priority reset > set > clear
        }
        RequestClearFOFBit(); // Clear FOF as priority reset > set > clear
    }

    // Update PWG_TOUT only when CRBR < PWG_PERIOD
    if (crbr != csbr){
        mRealTOUT = 1;
        mPWG_TOUT = 1;
        mWritePWG_TOUTEvent.notify();
    }
}//}}}

/// Control PWG_TRGOUT port when CTBR is matched with counter
void PWGENC0::CmpMatchCTBRMethod()
{//{{{
    unsigned int ocl = ((mpPWGENC0Func->GetPWG0CTLReg()) >> 7) & 0x1;
    unsigned int tobe = mpPWGENC0Func->GetPWG0TCBRReg();
    // ocl = 0 && TOUT = 1, or ocl = 1 (regardless TOUT), write PWG_TRGOUT if TOBE is enable
    if ( ((ocl == 1) || (mRealTOUT == 1)) && (tobe == 1) ){
        mPWG_TRGOUT = 1;
        mWritePWG_TRGOUTEvent.notify();
    }
}//}}}

/// Repeat counting when match PWG_PERIOD
void PWGENC0::CmpMatchPERIODMethod()
{//{{{
    // Continue count up to PWG_PERIOD if mEnableCounting, and PWG_CKI[cks] > 0
    unsigned int cks = (mpPWGENC0Func->GetPWG0CTLReg()) & 0x3;
    if ((mEnableCounting) && (mPWG_CKI[cks] > 0)){
        mIsSuspendCounting = false;// clear variable related to resume when matching PWG_PERIOD
        mResumeCounterTime = 0;
        CountToPERIOD(0);
    }
}//}}}

/// Monitor PWG_PERIOD port
void PWGENC0::MonitorPWG_PERIODMethod()
{//{{{
    if (mEnableCounting){
        re_printf("warning", "Ignore change of PWG_PERIOD while counting.\n");
    }else{
        mPERIOD = (unsigned int)PWG_PERIOD.read();
    }
}//}}}

/// Update RSF bit as priority reset > set > clear
void PWGENC0::UpdateRSFBitMethod()
{//{{{
    if (mIsResetRSFBit){
        mpPWGENC0Func->UpdateRSFBit(0);
    }else if (mIsSetRSFBit){
        mpPWGENC0Func->UpdateRSFBit(1);
    }else{ // mIsClearRSFBit
        mpPWGENC0Func->UpdateRSFBit(0);
    }
    mIsResetRSFBit = false;
    mIsSetRSFBit = false;
    mIsClearRSFBit = false;
}//}}}

/// Update RSFT bit as priority reset > set > clear
void PWGENC0::UpdateRSFTBitMethod()
{//{{{
    if (mIsResetRSFTBit){
        mpPWGENC0Func->UpdateRSFTBit(0);
    }else if (mIsSetRSFTBit){
        mpPWGENC0Func->UpdateRSFTBit(1);
    }else{ // mIsClearRSFTBit
        mpPWGENC0Func->UpdateRSFTBit(0);
    }
    mIsResetRSFTBit = false;
    mIsSetRSFTBit = false;
    mIsClearRSFTBit = false;
}//}}}

/// Update FOF bit as priority reset > set > clear
void PWGENC0::UpdateFOFBitMethod()
{//{{{
    if (mIsResetFOFBit){
        mpPWGENC0Func->UpdateFOFBit(0);
    }else if (mIsSetFOFBit){
        mpPWGENC0Func->UpdateFOFBit(1);
    }else{ // mIsClearFOFBit
        mpPWGENC0Func->UpdateFOFBit(0);
    }
    mIsResetFOFBit = false;
    mIsSetFOFBit = false;
    mIsClearFOFBit = false;
}//}}}

/// Write PWG_TOUT port
void PWGENC0::WritePWG_TOUTMethod()
{//{{{
    PWG_TOUT.write(mPWG_TOUT);
}//}}}

/// Write PWG_TRGOUT port
void PWGENC0::WritePWG_TRGOUTMethod()
{//{{{
    PWG_TRGOUT.write(mPWG_TRGOUT);
    if (mPWG_TRGOUT){
        mPWG_TRGOUT = false;
        mWritePWG_TRGOUTEvent.notify(mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/// Write interrupt PWG_INTR port
void PWGENC0::WritePWG_INTRMethod()
{//{{{
    PWG_INTR.write(mPWG_INTR);
    if (mPWG_INTR){
        if (mDumpInterrupt){
            re_printf("info", "PWG_INTR is changed to %d.\n", (unsigned int)mPWG_INTR);
        }
        mPWG_INTR = false;
        mWritePWG_INTREvent.notify(mPCLKPeriod, mTimeResolutionUnit);
    }else{
        if (mDumpInterrupt){
            re_printf("info", "PWG_INTR is changed to %d.\n", (unsigned int)mPWG_INTR);
        }
    }
}//}}}

/// Write interrupt PWG_INTF port
void PWGENC0::WritePWG_INTFMethod()
{//{{{
    PWG_INTF.write(mPWG_INTF);
    if (mPWG_INTF){
        if (mDumpInterrupt){
            re_printf("info", "PWG_INTF is changed to %d.\n", (unsigned int)mPWG_INTF);
        }
        mPWG_INTF = false;
        mWritePWG_INTFEvent.notify(mPCLKPeriod, mTimeResolutionUnit);
    }else{
        if (mDumpInterrupt){
            re_printf("info", "PWG_INTF is changed to %d.\n", (unsigned int)mPWG_INTF);
        }
    }
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void PWGENC0::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (mEnableRegisterMessage){
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpPWGENC0Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpPWGENC0Func->RegisterHandler(cmd);
    }else{
        //Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpPWGENC0Func->RegisterHandler(cmd);

        //Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mpPWGENC0Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping message when users access registers
void PWGENC0::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpPWGENC0Func->RegisterHandler(cmd);
}//}}}

/// Enable/disable dumping interrupt message when it is asserted
void PWGENC0::DumpInterrupt (const std::string is_enable) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: PWGENC0_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","PWGENC0_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: PWGENC0_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Enable/disable dumping info/warning/error message of register IF
void PWGENC0::EnableRegisterMessage (const std::string is_enable) 
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: PWGENC0_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","PWGENC0_EnableRegisterMessage %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: PWGENC0_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// Command to assert reset
void PWGENC0::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETZ") {
        if ((!mIsResetHardActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETZ) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETZ is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETZ.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void PWGENC0::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
                // Note: not need notify zero clock because there is no thread
            }
        }else if (word_vector[0] == "PCLKRW") {
            mPCLKRWOrgFreq = freq;
            ConvertClockFreq (mPCLKRWFreq, mPCLKRWFreqUnit, freq, unit);
            if (mPCLKRWFreq > 0) {
                mPCLKRWPeriod = (sc_dt::uint64)(((1/(double)mPCLKRWFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKRWPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void PWGENC0::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        }else if (word_vector[0] == "PCLKRW") {
            re_printf("info","PCLKRW frequency is %0.0f %s\n", (double)mPCLKRWOrgFreq, mPCLKRWFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void PWGENC0::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpPWGENC0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void PWGENC0::ReleaseRegister (const std::string reg_name)
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
            mpPWGENC0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void PWGENC0::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mpPWGENC0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void PWGENC0::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpPWGENC0Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void PWGENC0::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpPWGENC0Func->RegisterHandler(cmd);
}//}}}

/// Dump help message of all parameters or commands
void PWGENC0::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"PWGENC0_MessageLevel (\"PWGENC0 instance\", \"fatal|error|warning|info\")  Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"PWGENC0_DumpRegisterRW (\"PWGENC0 instance\", \"true/false\")              Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"PWGENC0_DumpInterrupt (true/false)                                         Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(),"PWGENC0_EnableRegisterMessage (true/false)                                 Enable/disable dumping info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"PWGENC0_AssertReset (\"PWGENC0 instance\", \"reset_name\", start_time, period) Assert and de-assert reset signal to the PWGENC0 model");
            SC_REPORT_INFO(this->basename(),"PWGENC0_SetCLKFreq (\"PWGENC0 instance\", \"clock_name\", freq, \"unit\")      Set clock frequency value to PWGENC0 model. Valid clock_name is PCLK or PCLKRW");
            SC_REPORT_INFO(this->basename(),"PWGENC0_GetCLKFreq (\"PWGENC0 instance\", \"clock_name\")                      Get clock frequency value of PWGENC0 model. Valid clock_name is PCLK or PCLKRW");
            SC_REPORT_INFO(this->basename(),"PWGENC0_ForceRegister (\"PWGENC0 instance\", \"reg_name\", reg_value)          Force register with setting value");
            SC_REPORT_INFO(this->basename(),"PWGENC0_ReleaseRegister (\"PWGENC0 instance\", \"reg_name\")                   Release register from force value");
            SC_REPORT_INFO(this->basename(),"PWGENC0_WriteRegister (\"PWGENC0 instance\", \"reg_name\", reg_value)          Write a value to a register");
            SC_REPORT_INFO(this->basename(),"PWGENC0_ReadRegister (\"PWGENC0 instance\", \"reg_name\")                      Read a value from a register");
            SC_REPORT_INFO(this->basename(),"PWGENC0_ListRegister (\"PWGENC0 instance\")                                    Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of PWGENC0_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of PWGENC0_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
