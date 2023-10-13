// ---------------------------------------------------------------------
// $Id: ATU5_TIMERC_SUB.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERC_SUB_H__
#define __ATU5_TIMERC_SUB_H__

#include "ATU5_TIMERC_SUB_AgentController.h"
#include "ATU5_TIMERC_AgentController.h"
#include "re_define.h"
#include "ATU5_TIMERA.h"

class ATU5_TIMERC_SUB_Func;
class ATU5_TIMERC_SUB: public sc_module,
              public ATU5_TIMERC_SUB_AgentController
{
friend class ATU5_TIMERC;
friend class ATU5_NoiseCanceler;
std::string mModuleName;
private:
    enum eResetActiveLevel {
        emResetActive               = 0,
        emResetSignalNum            = 1
    }; //Define active level of reset signal
public:
    ATU5_TIMERC_SUB_Func       *mTIC_SUBFunc;//ATU5_TIMERC_SUB_Func class instance
    //Declare input signals
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> nc_clk;
    sc_in<sc_dt::uint64> clock_bus0;
    sc_in<sc_dt::uint64> clock_bus1;
    sc_in<sc_dt::uint64> clock_bus2;
    sc_in<sc_dt::uint64> clock_bus3;
    sc_in<sc_dt::uint64> clock_bus4;
    sc_in<sc_dt::uint64> clock_bus5;
    sc_inout<bool> TIOC0;
    sc_inout<bool> TIOC1;
    sc_inout<bool> TIOC2;
    sc_inout<bool> TIOC3;
    //add for updating
    sc_out<bool> TOCE0;
    sc_out<bool> TOCE1;
    sc_out<bool> TOCE2;
    sc_out<bool> TOCE3;

    sc_out<bool> TOC0;
    sc_out<bool> TOC1;
    sc_out<bool> TOC2;
    sc_out<bool> TOC3;

    ///////////////////////
    sc_in<bool> i_event_1_0;
    sc_in<bool> i_event_2A;
    sc_in<bool> i_event_2B;

    //Declare output signals
    sc_out<bool> INT_OVI;
    sc_out<bool> *INT_GRC[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_out<bool> *INT_OCRC[ATU5_TIMERC_SUB_AgentController::emChannel];
    //Declare target socket

    SC_HAS_PROCESS (ATU5_TIMERC_SUB);

    ATU5_TIMERC_SUB (sc_module_name name, 
                    ATU5_TIMERC *ATU5_TIMERC_Ptr,
                    unsigned int Sub);
    ~ATU5_TIMERC_SUB (void);
    ATU5_Timer      *mFreeRunCnt;
    ATU5_Timer      *mRunCntGRC[ATU5_TIMERC_SUB_AgentController::emChannel];
    ATU5_Timer      *mRunCntOCRC[ATU5_TIMERC_SUB_AgentController::emChannel];
    ATU5_NoiseCanceler      *mNoiseCanceler[ATU5_TIMERC_SUB_AgentController::emChannel];

    //Declare public functions
    void EnableReset(const bool is_active);
    bool IsNoiseCancelDuration(void);
    void EnableNoiseCancel(const unsigned int channel, const bool is_enable);
    void SetNoiseEachCancelMode(const unsigned int channel, unsigned int mode); // add 0820
private:
    sc_dt::uint64       mCLKBUSPeriod;
    sc_dt::uint64       mPre_CntClkPeriod;
    sc_dt::uint64       mPCLKPeriod;
    sc_dt::uint64       mCLKBUS_nPeriod[6];
    bool        mCurTIC[4];
    bool        mCurTICevent[4];

    double              mTimeCLKChangePoint;
    double              mTimeCLKStart[6]; // Modified by for ver 0.080
    sc_dt::uint64       mCurCLKCntPeriod[6]; // Modified ver 0.080 bug#55267
    sc_dt::uint64       mCLKNOISE_nPeriod[2];      //Frequency value of nc_clk_n (n: 0 -> 1)
    bool                mIsOvf;
    //varibales
    bool mTIOC[ATU5_TIMERC_SUB_AgentController::emChannel];
    bool mIsCntGRCWorking[ATU5_TIMERC_SUB_AgentController::emChannel];
    bool mIsOVFPWM;
    bool mIsTimerRunning;
    bool mTIOC1;
    bool mTIOC2;
    bool mTIOC3;
    bool mTIOC0;
    bool mIA1;
    bool mIA2A;
    bool mIA2B;
    bool mIsGRCWaitOvf[ATU5_TIMERC_SUB_AgentController::emChannel]; //use for 1shot
    bool mIsUpdateGRC[ATU5_TIMERC_SUB_AgentController::emChannel]; // use for 1 shot
    bool mIsWaitOCRCMatching[ATU5_TIMERC_SUB_AgentController::emChannel]; // use for 1 shot
    bool mIsGRCWaitStart[ATU5_TIMERC_SUB_AgentController::emChannel]; // use for 1 shot
    bool mIsOCRCWaitStart[ATU5_TIMERC_SUB_AgentController::emChannel]; // use for 1 shot
    bool mIsWaitGRCMatching[ATU5_TIMERC_SUB_AgentController::emChannel]; // use for 1 shot
    bool mTOCE[ATU5_TIMERC_SUB_AgentController::emChannel]; //update for TOCE port
    bool mCurTOCE[ATU5_TIMERC_SUB_AgentController::emChannel]; //update for TOCE port
    unsigned int mLimitValue;
    unsigned int mSub_id;
    unsigned int mPre_OCRC[ATU5_TIMERC_SUB_AgentController::emChannel];
    double mGRCMatchTime[ATU5_TIMERC_SUB_AgentController::emChannel];
    double mOCRCMatchTime[ATU5_TIMERC_SUB_AgentController::emChannel];
    bool mIsGRC1stMatch[ATU5_TIMERC_SUB_AgentController::emChannel];
    bool mIsOCRC1stMatch[ATU5_TIMERC_SUB_AgentController::emChannel];
	



    //event
    sc_event mNoiseCntEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mCaptureOCRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mCaptureGRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mWriteTIOCOutputEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mNoiseCancellationEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mRisingInterruptOCRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mRisingInterruptGRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mFallingInterruptOCRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mFallingInterruptGRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mHandleFlagAndInterruptGRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mHandleFlagAndInterruptOCRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mCompareZeroMatchEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mClkZeroEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mHandleCmpOCRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mHandleCmpGRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mStartCntOCRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mStartCntGRCEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mOneShotProcessEvent[ATU5_TIMERC_SUB_AgentController::emChannel];
    sc_event mWriteTOCEPortEvent[ATU5_TIMERC_SUB_AgentController::emChannel]; //update for TOCE port
    sc_event mHandleInputChannel0Event;
    sc_event mHandleInputChannel1Event;
    sc_event mHandleInputChannel2Event;
    sc_event mHandleInputChannel3Event;
    sc_event mRisingInterruptOVFEvent;
    sc_event mFallingInterruptOVFEvent;
    sc_event mWritetoTIOC3Event;
    sc_event mWritetoTIOC2Event;
    sc_event mWritetoTIOC1Event;
    sc_event mStartTimerEvent;
    sc_event mHandlePWMEvent;
    sc_event mRstStateEvent;
    sc_event mInterruptWithPCLKEvent;
    sc_event mRestartTimerEvent;
    sc_event mClearTimeEvent;
    sc_event mHandleCmpOvfEvent;
    sc_event mRestartCUCRCEvent;
    sc_event mRestartOneShotEvent;
    //Method
    void RestartTimerMethod(void);
    void OutputNoiseMethod(const unsigned int channel);
    void SetNCNTCMethod(const unsigned int channel);
    void UpdateNoiseCounterValue(const unsigned int channel, const unsigned int value);
    void RisingTIOCOutputMethod(const unsigned int channel);
    void FallingTIOCOutputMethod(const unsigned int channel);
    void RisingInterruptGRCMethod(const unsigned int channel);
    void RisingInterruptOCRCMethod(const unsigned int channel);
    void FallingInterruptGRCMethod(const unsigned int channel);
    void FallingInterruptOCRCMethod(const unsigned int channel);
    void WriteTIOCOutputMethod(const unsigned int channel);
    void WriteTOCEPortMethod(const unsigned int channel);
    void RisingInterruptOVFMethod(void);
    void FallingInterruptOVFMethod(void);
    void HandleInputChannel0Method(void);
    void HandleInputChannel1Method(void);
    void HandleInputChannel2Method(void);
    void HandleInputChannel3Method(void);
    void HandleTIOC0Method(void);
    void HandleTIOC1Method(void);
    void HandleTIOC2Method(void);
    void HandleTIOC3Method(void);
    void HandleCmpOvfMethod(void);
    void InterruptWithPCLKMethod(void);
    void ClearTimeMethod(void);
    void RestartCUCRCMethod(void);
    void HandleCmpOCRCMethod(const unsigned int channel);
    void HandleCmpGRCMethod(const unsigned int channel);
    void HandleFlagAndInterruptGRCMethod(const unsigned int channel);
    void HandleFlagAndInterruptOCRCMethod(const unsigned int channel);
    void HandleCntClkInputMethod(void);
    void CaptureOCRCMethod(const unsigned int channel);
    void CaptureGRCMethod(const unsigned int channel);
    void HandlePWMMethod(void);
    void StartCntOCRCMethod(const unsigned int channel);
    void StartCntGRCMethod(const unsigned int channel);
    void UpdateGRCOneShot(const unsigned int channel);
    void HandleOvfOCRCMethod(const unsigned int channel);
    void HandleOvfGRCMethod(const unsigned int channel);

    void SetCLKNoisePeriod(const unsigned int chan_num);
    void SetNoiseCORVal (const unsigned int chan_num, const unsigned int cor_val);
    void RunningCounter(const bool is_run);
    void StartStopTimer(const bool is_start);
    void StartTimerMethod(void);
    void SetCmpOCRCValue(const unsigned int channel, const unsigned int value);
    void SetCmpGRCValue(const unsigned int channel, const unsigned int value);
    void SetNoiseCancelMode(const unsigned int mode);
//    void ModeOperation (unsigned int event, unsigned int channel);
    void SetCounterPeriod (const double period);
//    void HandleOneshot(const unsigned int channel, const unsigned int value);
    void Initialize(void);
    void CheckCmpGRCOCRC(void);
    void CancelEvent(void);
    double CalculateClockEdge(const std::string clock_name, const bool is_pos, const bool add_period, const double time_point, const double time_change_point);
    double GetCurTime(void);
    bool IsModelResetActive(void);
    void HandlePriorityMethod(const unsigned int channel); // added by for ver 0.080 181
    //Declare virtual functions of ATU5_TIMERC_SUB_AgentController class
    void SetCounterValue(const unsigned int value);
    void SetOCRCValue(const unsigned int channel, const unsigned int value);
    void SetGRCValue(const unsigned int channel, const unsigned int value);
    void SetUpperLimit(const unsigned int value);
    void StartOneShotMethod(const unsigned int channel, const unsigned int value);
    void HandleForcedCmpMatchMethod(const unsigned int channel);
    void UpdateInRunning(const std::string name);
    unsigned int GetCounterValue(void);
    unsigned int GetNoiseCounterValue(const unsigned int channel);
    void UpdatingCntClock(void);
    void OffCntUpperLimit(void);
    void OffPWM(void);
    void SetModeDuringRunning(void);
    void RangeCmpOCRC(const double range_value, const unsigned int channel);
    void RangeCmpGRC(const double range_value, const unsigned int channel);
    void SetPriority(const unsigned int channel, const bool value); // ver 0.080

    //add for updating
    void ControlTOCE(const unsigned int channel, const bool value);
	
};
#endif
