// ---------------------------------------------------------------------
// $Id: LPSA100.cpp,v 1.15 2020/05/29 04:34:32 hadinhtran Exp $
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

#include "re_define.h"
#include "LPSA100.h"
#include "LPSA100_Func.h"

/*********************************    
// Function     : LPSA100
// Description  : Constructor of LPSA100 class
// Parameter    : 
//   name            module name
//   rLatency        read latency
//   wLatency        write latency
// Return value : None    
**********************************/
LPSA100::LPSA100(sc_module_name name, const unsigned int rLatency, const unsigned int wLatency)
        :sc_module(name)
        ,LPSA100_AgentController()
        ,BusSlaveBase<32, 1>()
        ,rvc_common_model()
        ,CLK_LSB("CLK_LSB")
        ,CLKA_LPS("CLKA_LPS")
        ,PRESETn("PRESETn")
        ,ule_lps("ule_lps")
        ,APO("APO")
        ,DPO("DPO")
        
{//{{{ 
    /// Initialize variables
    for (int i = 0; i < emNumClk; i++){
        mClkPeriod[i]    = 0;
        mClkOrgFreq[i]   = 0;
        mClkFreq[i]      = 0;
        mClkFreqUnit[i]  = "Hz";
        mStartClkTime[i] = 0;
    }
    
    mEnableRegisterMessage = true;
    mIsPRESETnInit     = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    mDumpInterrupt     = false;
    mfRH               = 0;

    mTimeResolutionValue = 1;
    mTimeResolutionUnit  = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    Initialize();
    
    /// Bind clock, reset signal and socket to Bus IF
    setSlaveResetPort32(&PRESETn);
    setSlaveFreqPort32(&CLK_LSB);
    setTargetSocket32((char*)"ts");

    /// Alias for binding
    ts = BusSlaveBase<32, 1>::tSocket32[0];
    
    /// Instance mpLPSA100Func
    mpLPSA100Func = new LPSA100_Func((std::string)name, this);
    sc_assert(mpLPSA100Func != NULL);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setFuncModulePtr(mpLPSA100Func);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setReadLatency(rLatency);
    BusSlaveBase<32, 1>::mBusSlaveIf32[0]->setWriteLatency(wLatency);

    /// Declare & instantiate array of input ports
    for(int y = 0; y < emNumINTTAUy; y++) {
        for(int x = 0; x < emNumINTTAUx; x++) {
            std::ostringstream inst_name;
            inst_name << "INTTAUJ" << y + emStartIndexINTTAUy << "I" << x;
            INTTAUJyIx[y][x] = new sc_in<bool>(inst_name.str().c_str());
            sc_assert(INTTAUJyIx[y][x] != NULL);
        }
    }
    
    for(int x = 0; x < emNumINT_AD; x++) {
        std::ostringstream inst_name;
        inst_name << "INT_AD" << x;
        INT_AD[x] = new sc_in<bool>(inst_name.str().c_str());
        sc_assert(INT_AD[x] != NULL);
    }
    
    for(int m = 0; m < emNumDPIN; m++) {
        std::ostringstream inst_name;
        inst_name << "DPIN" << m;
        DPIN[m] = new sc_in<bool>(inst_name.str().c_str());
        sc_assert(DPIN[m] != NULL);
    }
    
    /// Declare, instantiate & initialize value of the output ports
    for(int k = 0; k < emNumDPSEL; k++) {
        std::ostringstream inst_name;
        inst_name << "DPSEL" << k;
        DPSEL[k] = new sc_out<bool>(inst_name.str().c_str());
        sc_assert(DPSEL[k] != NULL);
        DPSEL[k]->initialize(false);
    }
    
    APO.initialize(false);
    DPO.initialize(false);
    INTCWEND.initialize(false);
    WUTRG0.initialize(false);
    WUTRG1.initialize(false);
    INTDPE.initialize(false);
    SEQADTRG.initialize(false);
    
    /// SC_METHOD
    SC_METHOD(MonitorCLK_LSBMethod);
    dont_initialize();
    sensitive << CLK_LSB;
    
    SC_METHOD(MonitorCLKA_LPSMethod);
    dont_initialize();
    sensitive << CLKA_LPS;
    
    SC_METHOD(MonitorPRESETnMethod);
    dont_initialize();
    sensitive << PRESETn;
    
    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;
    
    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;
    
    SC_METHOD(WriteAPOMethod);
    dont_initialize();
    sensitive << mWriteAPOEvent;
    
    SC_METHOD(NegateAPOMethod);
    dont_initialize();
    sensitive << mNegateAPOEvent;
    
    SC_METHOD(WriteDPOMethod);
    dont_initialize();
    sensitive << mWriteDPOEvent;
    
    SC_METHOD(NegateDPOMethod);
    dont_initialize();
    sensitive << mNegateDPOEvent;
    
    SC_METHOD(WriteINTCWENDMethod);
    dont_initialize();
    sensitive << mWriteINTCWENDEvent;
    
    SC_METHOD(NegateINTCWENDMethod);
    dont_initialize();
    sensitive << mNegateINTCWENDEvent;
    
    SC_METHOD(WriteWUTRG0Method);
    dont_initialize();
    sensitive << mWriteWUTRG0Event;
    
    SC_METHOD(NegateWUTRG0Method);
    dont_initialize();
    sensitive << mNegateWUTRG0Event;
    
    SC_METHOD(WriteWUTRG1Method);
    dont_initialize();
    sensitive << mWriteWUTRG1Event;
    
    SC_METHOD(NegateWUTRG1Method);
    dont_initialize();
    sensitive << mNegateWUTRG1Event;
    
    SC_METHOD(WriteSEQADTRGMethod);
    dont_initialize();
    sensitive << mWriteSEQADTRGEvent;
    
    SC_METHOD(NegateSEQADTRGMethod);
    dont_initialize();
    sensitive << mNegateSEQADTRGEvent;
    
    SC_METHOD(WriteDPSELMethod);
    dont_initialize();
    sensitive << mWriteDPSELEvent;
    
    SC_METHOD(MonitorStartTriggerMethod);
    dont_initialize();
    for(int y = 0; y < emNumINTTAUy; y++) {
        for(int x = 0; x < emNumINTTAUx; x++) {
            sensitive << (*INTTAUJyIx[y][x]);
        }
    }
    
    SC_METHOD(MonitorULE_LPSMethod);
    dont_initialize();
    sensitive << ule_lps;
    
    SC_METHOD(MonitorINT_ADMethod);
    dont_initialize();
    for(int x = 0; x < emNumINT_AD; x++) {
        sensitive << (*INT_AD[x]);
    }
    
    /// SC_THREAD
    SC_THREAD(ProcessDigitalInputModeThread);
    dont_initialize();
    sensitive << mProcessDigitalInputModeEvent;
    
    SC_THREAD(ProcessAnalogInputModeThread);
    dont_initialize();
    sensitive << mProcessAnalogInputModeEvent;
    
    SC_METHOD(WriteINTDPEMethod);
    dont_initialize();
    sensitive << mWriteINTDPEEvent;

}//}}} 

/*********************************
// Function     : ~LPSA100
// Description  : Destructor of LPSA100 class
// Parameter    : None
// Return value : None
**********************************/
LPSA100::~LPSA100()
{//{{{
    delete mpLPSA100Func;
}//}}}

/*********************************
// Function     : MonitorCLK_LSBMethod
// Description  : Handle CLK_LSB clock
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::MonitorCLK_LSBMethod()
{//{{{
    sc_dt::uint64 freq_value = CLK_LSB.read();
    SetCLKFreq("CLK_LSB", freq_value, "Hz");
}//}}}

/*********************************
// Function     : MonitorCLKA_LPSMethod
// Description  : Handle CLKA_LPS clock
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::MonitorCLKA_LPSMethod()
{//{{{
    sc_dt::uint64 freq_value = CLKA_LPS.read();
    SetCLKFreq("CLKA_LPS", freq_value, "Hz");
    mfRH = mClkFreq[emCLKA_LPSId];
}//}}}

/*********************************
// Function     : MonitorpresetzMethod
// Description  : Handle PRESETn signal
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::MonitorPRESETnMethod()
{//{{{
    bool reset = PRESETn.read();
    if (reset == vpcl::emResetActive){
        mIsPRESETnInit = false;
        mIsResetHardActive = true;
        mResetHardEvent.notify();
        re_printf("info", "The reset port PRESETn is asserted.\n");
        EnableReset(true);
        
    }else{
        if (mIsPRESETnInit){
            mIsPRESETnInit = false;
        }else{
            mIsResetHardActive = false;
            mResetHardEvent.cancel();
            re_printf("info", "The reset port PRESETn is de-asserted.\n");
            EnableReset(false);
        }
    }
    /// Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

/*********************************
// Function     : HandleResetCmdMethod
// Description  : Process reset function when reset command is active
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::HandleResetCmdMethod()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/*********************************
// Function     : CancelResetCmdMethod
// Description  : Cancel reset function when reset command is active
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::CancelResetCmdMethod()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/*********************************
// Function     : WriteAPOMethod
// Description  : Write APO port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteAPOMethod()
{//{{{
    APO.write(mAPO);
    if (mDumpInterrupt){
        re_printf("info", "APO is changed to %d\n", (unsigned int)mAPO);
    }
}//}}}

/*********************************
// Function     : NegateAPOMethod
// Description  : Negate APO port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::NegateAPOMethod()
{//{{{
    mAPO = false;
    mAnalog_IsAssertINT = false;
    mWriteAPOEvent.notify();
}//}}}

/*********************************
// Function     : WriteDPOMethod
// Description  : Write DPO port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteDPOMethod()
{//{{{
    DPO.write(mDPO);
    if (mDumpInterrupt){
        re_printf("info", "DPO is changed to %d\n", (unsigned int)mDPO);
    }
}//}}}

/*********************************
// Function     : NegateDPOMethod
// Description  : Negate DPO port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::NegateDPOMethod()
{//{{{
    mDPO = false;
    mWriteDPOEvent.notify();
    
    /// clear condition for digital port error interrupt
    if (mINTDPE == true){
        mINTDPE = false;
        mWriteINTDPEEvent.notify();
    }
}//}}}

/*********************************
// Function     : WriteINTCWENDMethod
// Description  : Write INTCWEND port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteINTCWENDMethod()
{//{{{
    INTCWEND.write(mINTCWEND);
    if (mDumpInterrupt){    
        re_printf("info", "INTCWEND is changed to %d\n", (unsigned int)mINTCWEND);
    }
    
    /// Negate after one clock period
    if (mINTCWEND){
        mNegateINTCWENDEvent.notify((double)mClkPeriod[emCLKA_LPSId], mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : NegateINTCWENDMethod
// Description  : Negate INTCWEND port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::NegateINTCWENDMethod()
{//{{{
    mINTCWEND = false;
    mWriteINTCWENDEvent.notify();
}//}}}

/*********************************
// Function     : WriteWUTRG0Method
// Description  : Write WUTRG0 port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteWUTRG0Method()
{//{{{
    WUTRG0.write(mWUTRG0);
    if (mDumpInterrupt){
        re_printf("info", "WUTRG0 is changed to %d\n", (unsigned int)mWUTRG0);
    }
    
    /// Negate after one clock period
    if (mWUTRG0){
        mNegateWUTRG0Event.notify((double)mClkPeriod[emCLKA_LPSId], mTimeResolutionUnit);
    }

    /// set condition for digital port error interrupt
    if (mINTDPE == false && mWUTRG0 == true){
        mINTDPE = true;
        mWriteINTDPEEvent.notify();
    }
}//}}}

/*********************************
// Function     : NegateWUTRG0Method
// Description  : Negate WUTRG0 port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::NegateWUTRG0Method()
{//{{{
    mWUTRG0 = false;
    mWriteWUTRG0Event.notify();
}//}}}

/*********************************
// Function     : WriteWUTRG1Method
// Description  : Write WUTRG1 port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteWUTRG1Method()
{//{{{
    WUTRG1.write(mWUTRG1);
    if (mDumpInterrupt){
        re_printf("info", "WUTRG1 is changed to %d\n", (unsigned int)mWUTRG1);
    }
    
    /// Negate after one clock period
    if (mWUTRG1){
        mNegateWUTRG1Event.notify((double)mClkPeriod[emCLKA_LPSId], mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : NegateWUTRG1Method
// Description  : Negate WUTRG1 port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::NegateWUTRG1Method()
{//{{{
    mWUTRG1 = false;
    mWriteWUTRG1Event.notify();
}//}}}

/*********************************
// Function     : WriteINTDPEMethod
// Description  : Write INTDPE port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteINTDPEMethod()
{//{{{
    INTDPE.write(mINTDPE);  
    if (mDumpInterrupt){
        re_printf("info", "INTDPE is changed to %d\n", (unsigned int)mINTDPE);
    }
}//}}}

/*********************************
// Function     : WriteSEQADTRGMethod
// Description  : Write SEQADTRG port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteSEQADTRGMethod()
{//{{{
    bool cur_val = SEQADTRG.read();
    if(cur_val != mSEQADTRG) {
        SEQADTRG.write(mSEQADTRG);
        if (mDumpInterrupt){
            re_printf("info", "SEQADTRG is changed to %d\n", (unsigned int)mSEQADTRG);
        }
    }
    
    /// Negate after one clock period
    if (mSEQADTRG){
        mNegateSEQADTRGEvent.notify((double)mClkPeriod[emCLKA_LPSId], mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : NegateSEQADTRGMethod
// Description  : Negate SEQADTRG port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::NegateSEQADTRGMethod()
{//{{{
    mSEQADTRG = false;
    mWriteSEQADTRGEvent.notify();
}//}}}

/*********************************
// Function     : WriteDPSELMethod
// Description  : Write DPSEL port
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::WriteDPSELMethod()
{//{{{
    for(int k = 0; k < emNumDPSEL; k++) {
        bool DPSEL_value = (bool)((mDPSEL >> k) & 0x1);
        if(DPSEL[k]->read() != DPSEL_value) {
            DPSEL[k]->write(DPSEL_value);
            if (mDumpInterrupt){
                re_printf("info", "DPSEL%d is changed to %d\n", k, (unsigned int)DPSEL_value);
            }
        }
    }
}//}}}

/*********************************
// Function     : MonitorStartTriggerMethod
// Description  : Method monitoring operation when start trigger is asserted
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::MonitorStartTriggerMethod()
{//{{{
    if(!GetResetStatus() && CheckClockPeriod("CLKA_LPS")) {
       for(int y = 0; y < emNumINTTAUy; y++) {
           for(int x = 0; x < emNumINTTAUx; x++) {
               bool cur_val = INTTAUJyIx[y][x]->read();
               /// Start trigger INTTAUJyIx is asserted
               if(mINTTAUJyIx[y][x] != cur_val) {
                   mINTTAUJyIx[y][x] = cur_val;
                   
                   /// Input Mode is enabled
                   if(cur_val && (mDigital_IsEnabled || mAnalog_IsEnabled)) {
                       std::ostringstream inst_name;
                       inst_name << "INTTAUJ" << y + emStartIndexINTTAUy << "I" << x;

                       /// Start trigger is accepted by setting
                       if (mpLPSA100Func->CheckTJIS(inst_name.str())){
                           if(CheckStartLPSOperation()) {
                               /// Analog started
                               if(mAnalog_IsEnabled) {
                                   mProcessAnalogInputModeEvent.notify();
                               } 
                               /// Digital started
                               if(mDigital_IsEnabled) {
                                   mProcessDigitalInputModeEvent.notify();
                               }
                           } else { /// Start trigger is asserted when LPSA100 is running.
                               re_printf("warning", "Cannot issue start trigger during LPSA100 operation, the start trigger is canceled\n");
                           }
                       } else { /// Start trigger is not accepted by setting
                           re_printf("info", "INTTAUJ%dI%d is not accepted start trigger\n", y + emStartIndexINTTAUy, x);
                       }
                   } else {
                       if (cur_val && !mAnalog_IsEnabled) {
                           re_printf("info", "Analog Input Mode is not enabled\n");
                       }
                       if (cur_val && !mDigital_IsEnabled) {
                           re_printf("info", "Digital Input Mode is not enabled\n");
                       }
                   }
               }
           }
       }
    } else {
        re_printf("info", "Operation clock is zero or in reset period\n");
    }
}//}}}

/*********************************
// Function     : MonitorULE_LPSMethod
// Description  : Method monitoring operation when ule_lps port is asserted
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::MonitorULE_LPSMethod()
{//{{{
    unsigned char cur_ule_lps = ule_lps.read();
    /// ule_lps is changed
    if(cur_ule_lps != mUle_lps) {
        mUle_lps = cur_ule_lps;
        /// ule_lps is cleared
        if(cur_ule_lps == 0x0) {
            mAnalog_IsError = false;
            mADErrorClearedEvent.notify();
            re_printf("info", "A/D error input port is cleared\n");
        }
    }
    /// ule_lps is not 0x0 and analog input mode still running
    /// which mean, there is an error occurred
    if(mUle_lps != 0x0 && mAnalog_IsProcessing) {
         mAnalog_IsError = true;
         mContinueAnalogThreadEvent.notify();
    }
}//}}}

/*********************************
// Function     : MonitorINT_ADMethod
// Description  : Method monitoring operation when INT_AD port is asserted
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::MonitorINT_ADMethod() 
{//{{{
    bool cur_INT_AD = false;
    for(unsigned int index = 0; index < emNumINT_AD; index++) {
        cur_INT_AD = INT_AD[index]->read();
        if ((INT_AD[index]->event() == true) && (cur_INT_AD) && (mAPO)){
            mAnalog_IsAssertINT = true;
            std::ostringstream inst_name;
            inst_name << "INT_AD" << index;
            re_printf("info", "Port %s is asserted\n", inst_name.str().c_str());
        }
    }
    /// A/D conversion is finished and there is no error occurred
     if((mAnalog_IsAssertINT) && (!mAnalog_IsError)) {
        mContinueAnalogThreadEvent.notify();
    }
}//}}}

/*********************************
// Function     : ProcessDigitalInputModeThread
// Description  : Thread monitoring operation when Digital Input Mode is started
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::ProcessDigitalInputModeThread()
{//{{{
    while(true) {
        /// There is no process when operation clock is 0 or in reset period or operation is stopped
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS") || CheckStopLPSOperation()) {
            wait();
            continue;
        }
        
        CalculateTiming();
        
        mDigital_IsProcessing = true;
        
        /// Wait a stabilization time 
        wait(mDigital_WaitTime, mDigital_WaitTimeUnit, mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
            continue;
        }
        
        re_printf("info", "Digital Wait time is completed\n");
        
        /// Output high level DPO port
        mDPO = true;
        mWriteDPOEvent.notify();    
        
        /// Wait a time signal source stabilization
        wait(mDigital_SignalSourceStabilizationTime, mTimeResolutionUnit, mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
            continue;
        }
        re_printf("info", "Signal Source Stabilization Time is completed\n");
        
        /// Recieve digital input value
        while(true) {
            wait(mDigital_IncreDPSELTime, mTimeResolutionUnit, mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
            if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
                mDigital_IsBreak = true;    
                break;
            }
        
            /// Store input value to register
            StoreDPIN();
            mpLPSA100Func->SetDPDIMR(mDPIN, mDPSEL);
            re_printf("info", "%s data input: 0x%08X\n", mTimesString[mDPSEL].c_str(), mDPIN);
            mDPSEL++;
            /// Number of times ports are read is enough
            if(mDPSEL > mNUMDP) {
                break;
            }
            mWriteDPSELEvent.notify();
        }
        if(mDigital_IsBreak) {
            mDigital_IsBreak = false;
            ResetDigitalInputMode();
            continue;
        }
        
        /// Generate port polling end interrupt
        mINTCWEND = true;
        mWriteINTCWENDEvent.notify();
        
        /// Reset DPSEL port after 4/mfRH/20
        wait(mDigital_ResetDPSELTime, mTimeResolutionUnit, mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
            continue;
        }
        
        mDPSEL = 0;
        mWriteDPSELEvent.notify();
        
        /// Input value matched with expected value
        if (CheckDigitalInputPort()) {
            /// Reset DPO output port
            mNegateDPOEvent.notify();
            
            /// Set event flag
            mpLPSA100Func->SetDINEVF(false);
            re_printf("info", "Comparison is matched\n");
            
        } else { /// Input value not matched with expected value
            /// Generate wake-up factor output signal for digital input
            mWUTRG0 = true;
            mWriteWUTRG0Event.notify();    
        
            /// Set event flag
            mpLPSA100Func->SetDINEVF(true);
            re_printf("info", "Comparison is not matched\n");
            
            /// LPS into waiting state until event flag DINEVF is cleared.
            wait(mEventFlagClearedEvent | mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
            if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
                continue;
            }
            
            /// Reset DPO output port
            mNegateDPOEvent.notify();
        }
        
        ResetDigitalInputMode();
        if(CheckStopLPSOperation()) {
            re_printf("info", "Digital Input Mode Operation is stopped\n");
            continue;
        }
        wait();
    }
}//}}}

/*********************************
// Function     : ProcessAnalogInputModeThread
// Description  : Thread monitoring operation when Analog Input Mode is started
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::ProcessAnalogInputModeThread()
{//{{{
    while(true) {
        /// There is no process when operation clock is 0 or in reset period or operation is stopped
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS") || CheckStopLPSOperation()) {
            wait();
            continue;
        }
        
        CalculateTiming();
        
        mAnalog_IsProcessing = true;
        
        /// Wait a stabilization time 
        wait(mAnalog_WaitTime, mAnalog_WaitTimeUnit, mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
            continue;
        }
        
        re_printf("info", "Analog Wait time is completed\n");
        
        /// Output high level APO port
        mAPO = true;
        mWriteAPOEvent.notify();
        
        /// Wait a stabilization time 
        wait(mAnalog_StabilizationTime, mTimeResolutionUnit, mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
            continue;
        }
        
        re_printf("info", "Stabilization time is completed\n");
        
        /// Output trigger to A/D converter
        mSEQADTRG = true;
        mWriteSEQADTRGEvent.notify();
        
        /// Waiting for A/D conversion error or all ended
        wait(mContinueAnalogThreadEvent | mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
        if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
            continue;
        }
        /// There is an error occurred
        if(mAnalog_IsError) {
            re_printf("info", "There is an error occurred in A/D conversion\n");
            
            /// Generate wake-up factor output signal for analog input
            mWUTRG1 = true;
            mWriteWUTRG1Event.notify();
            
            /// LPS into waiting state until error flag ule_lps is cleared.
            wait(mADErrorClearedEvent | mClkZeroEvent | mResetCmdEvent | mResetHardEvent);
            if(GetResetStatus() || !CheckClockPeriod("CLKA_LPS")) {
                continue;
            }
        } else {  /// There is no error and A/D conversion is done
            re_printf("info", "There is no error. A/D conversion is done\n");
        }
        /// Reset APO output port and negate SEQADTRG if needed
        mNegateAPOEvent.notify();
        mNegateSEQADTRGEvent.notify();
        
        ResetAnalogInputMode();
        if(CheckStopLPSOperation()) {
            re_printf("info", "Analog Input Mode Operation is stopped\n");
            continue;
        }
        wait();
    }
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initialize when reset activated
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::Initialize(void)
{//{{{    
    /// Initialize variables    
    mDPO       = false;
    mDPSEL     = 0;
    mINTCWEND  = false;
    mWUTRG0    = false;
    mDPIN      = 0;
    mNUMDP     = 0;
    mAPO       = false;
    mSEQADTRG  = false;
    mWUTRG1    = false;
    mUle_lps   = 0;
    
    mINTDPE    = false;
    for(int y = 0; y < emNumINTTAUy; y++) {
        for(int x = 0; x < emNumINTTAUx; x++) {
            mINTTAUJyIx[y][x] = false;
        }
    }
    
    mDigital_IsProcessing                  = false;
    mDigital_IsEnabled                     = false;
    mDigital_IsStopped                     = false;
    mDigital_SignalSourceStabilizationTime = 0;
    mDigital_IncreDPSELTime                = 0;
    mDigital_ResetDPSELTime                = 0;
    mDigital_WaitTime                      = 0;
    mDigital_WaitTimeUnit                  = SC_NS;
    mDigital_IsBreak                       = false;
    
    mAnalog_IsEnabled         = false;
    mAnalog_IsProcessing      = false;
    mAnalog_IsStopped         = false;
    mAnalog_IsError           = false;
    mAnalog_StabilizationTime = 0;
    mAnalog_WaitTime          = 0;
    mAnalog_WaitTimeUnit      = SC_NS;
    
    mAnalog_IsAssertINT = false;

}//}}}

/*********************************
// Function     : EnableReset
// Description  : Execute reset operation
// Parameter    :
//    is_active   true: reset; false: not reset
// Return value : None
**********************************/
void LPSA100::EnableReset (const bool is_active)
{//{{{
    mpLPSA100Func->EnableReset(is_active);
    if(is_active) {
        /// Cancel events
        CancelEvents();
        /// Initialize variables
        Initialize();
        /// Initialize output ports
        mWriteAPOEvent.notify();
        mWriteDPOEvent.notify();
        mWriteINTCWENDEvent.notify();
        mWriteWUTRG0Event.notify();
        mWriteWUTRG1Event.notify();
        mWriteINTDPEEvent.notify();
        mWriteSEQADTRGEvent.notify();
        mWriteDPSELEvent.notify();
    }
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel operation events
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::CancelEvents()
{//{{{
    mWriteAPOEvent.cancel();
    mNegateAPOEvent.cancel();
    mWriteDPOEvent.cancel();
    mNegateDPOEvent.cancel();
    mWriteINTCWENDEvent.cancel();
    mNegateINTCWENDEvent.cancel();
    mWriteWUTRG0Event.cancel();
    mNegateWUTRG0Event.cancel();
    mWriteWUTRG1Event.cancel();
    mNegateWUTRG1Event.cancel();
    mWriteINTDPEEvent.cancel();
    mWriteSEQADTRGEvent.cancel();
    mNegateSEQADTRGEvent.cancel();
    mWriteDPSELEvent.cancel();
    mProcessDigitalInputModeEvent.cancel();
    mProcessAnalogInputModeEvent.cancel();
    mContinueAnalogThreadEvent.cancel();
    mEventFlagClearedEvent.cancel();
    mADErrorClearedEvent.cancel();
}//}}}

/*********************************
// Function     : CalculateTiming
// Description  : Calculate timing agrument in LPS
// Parameter    : None
// Return value : 
//   true         Operation clock is not zero or not in reset period
//   false        Operation clock is zero or in reset period
**********************************/
bool LPSA100::CalculateTiming()
{//{{{
    sc_dt::uint64 stabilizationTime = (sc_dt::uint64)(((1.0 / mfRH / 20.0) * 16 * mpLPSA100Func->GetCNT0()) * mTimeResolutionValue);
    mDigital_SignalSourceStabilizationTime = stabilizationTime + (sc_dt::uint64)((1.0 / mfRH / 20.0) * mTimeResolutionValue);
    
    mDigital_IncreDPSELTime = (sc_dt::uint64)((2.0 / mfRH/ 20.0) * mTimeResolutionValue);
    mDigital_ResetDPSELTime = (sc_dt::uint64)((4.0 / mfRH/ 20.0) * mTimeResolutionValue);
    
    mNUMDP = mpLPSA100Func->GetNUMDP();
    
    mAnalog_StabilizationTime = (sc_dt::uint64)((1.0 / mfRH/ 20.0) * 16 * mpLPSA100Func->GetCNT1() * mTimeResolutionValue);
    
    return true;
}//}}}

/*********************************
// Function     : StoreDPIN
// Description  : Store input ports DPINm to mDPIN
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::StoreDPIN() 
{//{{{
    mDPIN = 0;
    for(int m = 0; m < emNumDPIN; m++) {
        mDPIN = mDPIN | ((*DPIN[m])->read() << m);
    }
}//}}}

/*********************************
// Function     : CheckDigitalInputPort
// Description  : Check digital input value with expected value
// Parameter    : None
// Return value : 
//       true     matched
//       false    not matched
**********************************/
bool LPSA100::CheckDigitalInputPort()
{//{{{
    for(unsigned int i = 0; i <= mNUMDP; i++) {
        unsigned int mask = mpLPSA100Func->GetDnEN(i);
        unsigned int input = mpLPSA100Func->GetDnM(i);
        unsigned int expected = mpLPSA100Func->GetDn(i);
        
        if ((bool)((mask & input) ^ (mask & expected))) {
            return false;
        }    
    }
    return true;
}//}}}

/*********************************
// Function     : CheckStartLPSOperation
// Description  : Check LPS can be started or not
// Parameter    : None
// Return value : 
//       true     start
//       false    not start
**********************************/
bool LPSA100::CheckStartLPSOperation()
{//{{{
    /// SOF = 1, LPS is in progress, start trigger is ignored
    if(mpLPSA100Func->GetOperationStatus()) {
        return false;
    }
    
    /// SOF = 0, start trigger is accepted
    mpLPSA100Func->SetOperationStatus(true);
    return true;
}//}}}

/*********************************
// Function     : CheckStopLPSOperation
// Description  : Check LPS can be stopped or not
// Parameter    : None
// Return value : None
**********************************/
bool LPSA100::CheckStopLPSOperation()
{//{{{
    /// Both mode is done
    if(mDigital_IsStopped && mAnalog_IsStopped) {
        mpLPSA100Func->SetOperationStatus(false);
        return true;
    }
    
    /// Digital mode is done. 
    if(mDigital_IsStopped) {
        /// Analog mode is running/ enabled
        if(mAnalog_IsProcessing || mAnalog_IsEnabled) {
            return false;
        } else { /// Digital mode is done. Analog mode is not running
            mpLPSA100Func->SetOperationStatus(false);
            return true;
        }
    } 
    
    /// Analog mode is done.
    if(mAnalog_IsStopped) {
        /// Digital mode is running/ enabled
        if(mDigital_IsProcessing || mDigital_IsEnabled) {
            return false;
        } else { /// Digital mode is done. Analog mode is not running
            mpLPSA100Func->SetOperationStatus(false);
            return true;
        }
    } 
    return false;
}//}}}

/*********************************
// Function     : ResetDigitalInputMode
// Description  : Reset some variables when a digital operation done
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::ResetDigitalInputMode()
{//{{{
    mDigital_IsProcessing = false;
    mDigital_IsBreak = false;
    if(!mAnalog_IsProcessing) {
        mpLPSA100Func->SetOperationStatus(false);
        for(int y = 0; y < emNumINTTAUy; y++) {
            for(int x = 0; x < emNumINTTAUx; x++) {
                mINTTAUJyIx[y][x] = false;
            }
        }
    }
}//}}}

/*********************************
// Function     : ResetAnalogInputMode
// Description  : Reset some variables when an analog operation done
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::ResetAnalogInputMode()
{//{{{
    mAnalog_IsProcessing = false;
    mAnalog_IsError      = false;
    if(!mDigital_IsProcessing) {
        mpLPSA100Func->SetOperationStatus(false);
        for(int y = 0; y < emNumINTTAUy; y++) {
            for(int x = 0; x < emNumINTTAUx; x++) {
                mINTTAUJyIx[y][x] = false;
            }
        }
    }
}//}}}

/*********************************
// Function     : CheckClockPeriod
// Description  : Check clock period value
// Parameter    : 
//   clock_name   name of clock (CLKA_LPS/ CLK_LSB)
// Return value : None
**********************************/
bool LPSA100::CheckClockPeriod (const std::string clock_name)
{//{{{
    if (clock_name == "CLK_LSB"){
        return (bool) (mClkPeriod[emCLK_LSBId] > 0);
    }else{
        return (bool) (mClkPeriod[emCLKA_LPSId] > 0);
    }
}//}}} 

/*********************************
// Function     : GetResetStatus
// Description  : Get reset status
// Parameter    : None
// Return value : None
**********************************/
bool LPSA100::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/*********************************
// Function     : NotifyEventFlagCleared
// Description  : Notify event flag is cleared event
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::NotifyEventFlagCleared()
{//{{{
    mEventFlagClearedEvent.notify();
}//}}}

/*********************************
// Function     : EnableDigitalInputMode
// Description  : Digital input mode is enabled
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::EnableDigitalInputMode()
{//{{{
    re_printf("info", "Digital Input Mode is enabled\n");
    mDigital_IsEnabled = true;
    mDigital_IsStopped = false;
    if(!mAnalog_IsProcessing) {
        mAnalog_IsStopped = false;
    }
}//}}}

/*********************************
// Function     : StopDigitalInputMode
// Description  : Digital input mode is stopped
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::StopDigitalInputMode()
{//{{{
    mDigital_IsEnabled = false;
    mDigital_IsStopped = true;
}//}}}

/*********************************
// Function     : EnableAnalogInputMode
// Description  : Analog input mode is enabled
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::EnableAnalogInputMode()
{//{{{
    re_printf("info", "Analog Input Mode is enabled\n");
    mAnalog_IsEnabled = true;
    mAnalog_IsStopped = false;
    if(!mDigital_IsProcessing) {
        mDigital_IsStopped = false;
    }
}//}}}

/*********************************
// Function     : StopAnalogInputMode
// Description  : Analog input mode is stopped
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::StopAnalogInputMode()
{//{{{
    mAnalog_IsEnabled = false;
    mAnalog_IsStopped = true;
}//}}}

/*********************************
// Function     : SetMessageLevel
// Description  : Set message level (fatal, error, warning, info)
// Parameter    : 
//     msg_lv     input argument for SetMessageLevel command
// Return value : None
**********************************/
void LPSA100::SetMessageLevel (const std::string msg_lv)
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
        /// Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mpLPSA100Func->RegisterHandler(cmd);

        ///Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        mpLPSA100Func->RegisterHandler(cmd);
    }else{
        /// Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mpLPSA100Func->RegisterHandler(cmd);

        /// Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        mpLPSA100Func->RegisterHandler(cmd);
    }
}//}}}

/*********************************
// Function     : DumpRegisterRW
// Description  : Enable/ Disable dumping message when users access registersn
// Parameter    : 
//     is_enable  true: dump info; false: not dump info
// Return value : None
**********************************/
void LPSA100::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpLPSA100Func->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : EnableRegisterMessage
// Description  : Enable/disable dumping info/warning/error message of register IF
// Parameter    : 
//     is_enable  true: dump info; false: not dump info
// Return value : None
**********************************/
void LPSA100::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: LPSA100_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mEnableRegisterMessage) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","LPSA100_EnableRegisterMessage %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: LPSA100_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/*********************************
// Function     : SetAnalogWaitTime
// Description  : Set value for Waiting Time in Analog mode
// Parameter    : 
//     time_value   stabilization time value
// Return value : None
**********************************/
void LPSA100::SetAnalogWaitTime(const unsigned int time_value, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, unit);
    if (word_vector.size() == 1) {
        if ((word_vector[0] == "s") || (word_vector[0] == "ms") || (word_vector[0] == "us") || (word_vector[0] == "ns") || (word_vector[0] == "ps")) {

            mAnalog_WaitTime = time_value;
            if (unit == "s") {
                mAnalog_WaitTimeUnit = SC_SEC;
            } else if (unit == "ms") {
                mAnalog_WaitTimeUnit = SC_MS;
            } else if (unit == "us") {
                mAnalog_WaitTimeUnit = SC_US;
            } else if (unit == "ns") {
                mAnalog_WaitTimeUnit = SC_NS;
            } else {
                mAnalog_WaitTimeUnit = SC_PS;
            }
        } else {
            re_printf("warning","Time unit (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Time unit (%s) is invalid\n", unit.c_str());
    }
}//}}}

/*********************************
// Function     : SetDigitalWaitTime
// Description  : Set value for Waiting Time in Digital mode
// Parameter    : 
//     time_value   stabilization time value
// Return value : None
**********************************/
void LPSA100::SetDigitalWaitTime(unsigned int time_value, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, unit);
    if (word_vector.size() == 1) {
        if ((word_vector[0] == "s") || (word_vector[0] == "ms") || (word_vector[0] == "us") || (word_vector[0] == "ns") || (word_vector[0] == "ps")) {
            mDigital_WaitTime = time_value;
            if (unit == "s") {
                mDigital_WaitTimeUnit = SC_SEC;
            } else if (unit == "ms") {
                mDigital_WaitTimeUnit = SC_MS;
            } else if (unit == "us") {
                mDigital_WaitTimeUnit = SC_US;
            } else if (unit == "ns") {
                mDigital_WaitTimeUnit = SC_NS;
            } else {
                mDigital_WaitTimeUnit = SC_PS;
            } 
        } else {
            re_printf("warning","Time unit (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Time unit (%s) is invalid\n", unit.c_str());
    }
}//}}}

/*********************************
// Function     : DumpInterrupt
// Description  : Enable/ Disable dumping interrupt information
// Parameter    : 
//     is_enable  true: dump info; false: not dump info
// Return value : None
**********************************/
void LPSA100::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: LPSA100_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "";
        if (mDumpInterrupt) {
            temp = "true";
        } else {
            temp = "false";
        }
        re_printf("info","LPSA100_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: LPSA100_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/*********************************
// Function     : AssertReset
// Description  : Assert reset by software
// Parameter    : 
//    reset_name   name of reset (PRESETn)
//    start_time   after <start_time> model will be rese
//    period       period of reset
// Return value : None
**********************************/
void LPSA100::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn.\n",reset_name.c_str());
    }
}//}}}

/*********************************
// Function     : SetCLKFreq
// Description  : Set clock value and clock unit
// Parameter    : 
//    clock_name   name of clock (CLKA_LPS/ CLK_LSB)
//    freq         new value frequency 
//    unit         new unit of frequency
// Return value : None
**********************************/
void LPSA100::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    unsigned int tmpCLKID;
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLKA_LPS" || word_vector[0] == "CLK_LSB") {
            if (word_vector[0] == "CLKA_LPS") {
                tmpCLKID = emCLKA_LPSId;
            } else {
                tmpCLKID = emCLK_LSBId;
            }
            mClkOrgFreq[tmpCLKID] = freq;
            ConvertClockFreq (mClkFreq[tmpCLKID], mClkFreqUnit[tmpCLKID], freq, unit);
            if (mClkFreq[tmpCLKID] > 0) {
                mClkPeriod[tmpCLKID] = (sc_dt::uint64)(((1/(double)mClkFreq[tmpCLKID])*(double)mTimeResolutionValue) + 0.5);
                mStartClkTime[tmpCLKID] = sc_time_stamp().to_double();
            } else {
                mClkPeriod[tmpCLKID] = 0;
                re_printf("warning","Clock frequency (%s) is zero\n", word_vector[0].c_str());
                CancelEvents();
                mClkZeroEvent.notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/*********************************
// Function     : GetCLKFreq
// Description  : Get clock value
// Parameter    : 
//    clock_name   name of clock (CLKA_LPS/ CLK_LSB)
// Return value : None
**********************************/
void LPSA100::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLKA_LPS") {
            re_printf("info","%s frequency is %0.0f %s\n", clock_name.c_str(), (double)mClkOrgFreq[emCLKA_LPSId], mClkFreqUnit[emCLKA_LPSId].c_str());
        } else if (word_vector[0] == "CLK_LSB") {
            re_printf("info","%s frequency is %0.0f %s\n", clock_name.c_str(), (double)mClkOrgFreq[emCLK_LSBId], mClkFreqUnit[emCLK_LSBId].c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/*********************************
// Function     : ForceRegister
// Description  : Force value to register
// Parameter    : 
//    reg_name    name of register
//    reg_value   value need to force to register
// Return value : None
**********************************/
void LPSA100::ForceRegister (const std::string reg_name, const unsigned int reg_value)
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
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpLPSA100Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : ReleaseRegister
// Description  : Release forced value after forcing registers
// Parameter    : 
//    reg_name    name of register
// Return value : None
**********************************/
void LPSA100::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpLPSA100Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : WriteRegister
// Description  : Write value to registers by software
// Parameter    : 
//    reg_name    name of register
//    reg_value   value need to write to register
// Return value : None
**********************************/
void LPSA100::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mpLPSA100Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : ReadRegister
// Description  : Read value of register by software
// Parameter    : 
//    reg_name    name of register
// Return value : None
**********************************/
void LPSA100::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mpLPSA100Func->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/*********************************
// Function     : ListRegister
// Description  : List registers name of whole model 
// Parameter    : None
// Return value : None
**********************************/
void LPSA100::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpLPSA100Func->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : Help
// Description  : Dump help message of all parameters or commands
// Parameter    : 
//     type       parameters/ commands
// Return value : None
**********************************/
void LPSA100::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"LPSA100_MessageLevel (\"LPSA100 instance\", \"fatal|error|warning|info\")           Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"LPSA100_DumpRegisterRW (\"LPSA100 instance\", \"true/false\")                       Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"LPSA100_DumpInterrupt (\"LPSA100 instance\", \"true/false\")                        Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"LPSA100_SetAnalogWaitTime (\"LPSA100 instance\", time_value, \"unit\")              Set value for Wait Time in Analog mode. (Default: 0 NS)");
            SC_REPORT_INFO(this->basename(),"LPSA100_SetDigitalWaitTime  (\"LPSA100 instance\", time_value, \"unit\")            Set value for Wait Time in Digital mode. (Default: 0 NS)");
            SC_REPORT_INFO(this->basename(),"LPSA100_EnableRegisterMessage (\"LPSA100 instance\", \"true/false\")                Enable/disable dumping info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"LPSA100_AssertReset (\"LPSA100 instance\", \"reset_name\", start_time, period)      Assert and de-assert reset signal to the LPSA100 model");
            SC_REPORT_INFO(this->basename(),"LPSA100_SetCLKFreq (\"LPSA100 instance\", \"clock_name\", freq, \"unit\")           Set clock frequency value to LPSA100 model. Valid clock_name is pclk");
            SC_REPORT_INFO(this->basename(),"LPSA100_GetCLKFreq (\"LPSA100 instance\", \"clock_name\")                           Get clock frequency value of LPSA100 model. Valid clock_name is pclk");
            SC_REPORT_INFO(this->basename(),"LPSA100_ForceRegister (\"LPSA100 instance\", \"reg_name\", reg_value)               Force register with setting value");
            SC_REPORT_INFO(this->basename(),"LPSA100_ReleaseRegister (\"LPSA100 instance\", \"reg_name\")                        Release register from force value");
            SC_REPORT_INFO(this->basename(),"LPSA100_WriteRegister (\"LPSA100 instance\", \"reg_name\", reg_value)               Write a value to a register");
            SC_REPORT_INFO(this->basename(),"LPSA100_ReadRegister (\"LPSA100 instance\", \"reg_name\")                           Read a value from a register");
            SC_REPORT_INFO(this->basename(),"LPSA100_ListRegister (\"LPSA100 instance\")                                         Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of LPSA100_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of LPSA100_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}
// vim600: set foldmethod=marker :
