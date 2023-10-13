// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERA.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERA_H__
#define __ATU5010_TIMERA_H__

#include "ATU5010_AgentController.h"
#include "ATU5010_TIMERA_Base.h"
#include "re_define.h"
#include <vector>

class ATU5010_Timer;
class ATU5010_NoiseCanceler;
class ATU5010_TIMERA_Func;
class ATU5010_TIMERA: public sc_module,
                   public ATU5010_TIMERA_Base
{
friend class ATU5010;
public:
    //Port declaration
    sc_in<uint64>                   clkbus_0;
    sc_in<uint64>                   clkbus_1;
    sc_in<uint64>                   clkbus_2;
    sc_in<uint64>                   clkbus_3;
    sc_in<uint64>                   clkbus_4;
    sc_in<uint64>                   clkbus_5;
    sc_in<uint64>                   nc_clk_1;
    sc_in<uint64>                   nc_clk_2;
    sc_in<bool>                     *TIA[ATU5010_AgentController::emTIAnum];
    sc_in<bool>                     TCNTB6CM;
    sc_out<bool>                    ev_out_1_0;
    sc_out<bool>                    ev_out_1_1;
    sc_out<bool>                    ev_out_1_2;
    sc_out<bool>                    ev_out_2A;
    sc_out<bool>                    ev_out_2B;
    sc_out<bool>                    ev_out_1B;
    sc_out<bool>                    ev_out_1C;
    sc_out<bool>                    ev_out_1D;
    sc_out<bool>                    ev_out_1E;
    sc_out<bool>                    ev_out_1F;
    sc_out<bool>                    ev_out_1G;
    sc_out<bool>                    ev_out_1H;
    sc_out<bool>                    ev_out_1I;
    sc_out<bool>                    *ICIA[ATU5010_AgentController::emTIAnum];
    sc_out<bool>                    OVIA;
    sc_out<bool>                    *TILOA[ATU5010_AgentController::emTIAnum];

    enum eInterruptId {emOVIA=0, emICIA0, emICIA1, emICIA2, emICIA3, emICIA4, emICIA5, emICIA6, emICIA7, emInterruptNum};
    enum eEventId {emEVO1_0=0, emEVO1_1, emEVO1_2, emEVO2A, emEVO2B, emEVO1B, emEVO1C, emEVO1D, emEVO1E, emEVO1F, emEVO1G, emEVO1H, emEVO1I, emEventNum};

    ATU5010_TIMERA_Func                *mATU5010_TIA_Func;
private:
    ATU5010_Timer                      *mFreeRunCnt;
    ATU5010_NoiseCanceler              *mNoiseCanceler[ATU5010_AgentController::emTIAnum];
    //Declare variables
    //Free-running counter
    sc_dt::uint64                   mCLKBUSPeriod;                                          //Period value of counter clock (TCNTA clock)
    sc_dt::uint64                   mCLKBUS_nPeriod[emCLKBUSnum];                           //Period value of clkbus_n (n: 0 -> 5)
    bool                            mIsTimerRunning;                                        //Free-running counter status
    bool                            mIsTimerCorMatch;                                       //Free-running counter compare match
    bool                            mIsCLKBusUpdated;                                       //clkbus_* is updated
    bool                            mIsTCNTAUpdated;                                        //TCNTA is updated by users
    double                          mTimeTick;                                              //Get tick clock time at start time and compare match time
    double                          mTimeCLKChangePoint;                                    //The time clock is changed (mCLKBUSPeriod)
    //Noise cancellation
    sc_dt::uint64                   mCLKNOISE_nPeriod[emCLKNSnum];                          //Period value of nc_clk_n (n: 0 -> 1)
    bool                            mCurTIA[ATU5010_AgentController::emTIAnum];                //TIA after noise cancellation (if enable)/or original TIA
    unsigned int                    mTCNTD0_val;
    bool                            mIsPreCLKBUSzero;
    bool                            mIsOVFInt;

    sc_event                        mTIAtoEVOEvent[ATU5010_AgentController::emTIAnum];         //mCurTIA is changed
    sc_event                        mAssertEvent[emEventNum];                               //Call update of event output
    sc_event                        mDeAssertEvent[emEventNum];                             //Call update of event output
    sc_event                        mIntrActiveEvent[emInterruptNum];                       //Call update of interrupt port
    sc_event                        mIntrInActiveEvent[emInterruptNum];                     //Call update of interrupt port
    sc_event                        mStartTimerEvent;
    sc_event                        mFreeRunCntEvent;
    sc_event                        mClkZeroEvent;                                          //Notify bus clock is zero
    sc_event                        mRstStateEvent;                                         //Notify reset state
    sc_event                        mCaptureTCNTDEvent[emInterruptNum];
    sc_event                        mOverflowEvent;

public:
    SC_HAS_PROCESS(ATU5010_TIMERA);
    ATU5010_TIMERA(sc_module_name name, ATU5010 *ATU5010_Ptr);
    ~ATU5010_TIMERA (void);

    void EnableTIA (const bool is_start);                                               //Enable/Disable Timer A
    void RegisterHandlerTIA (const std::vector<std::string> cmd);                       //Handle accessing registers
private:
    void Initialize (void);                                                             //Initializes internal variables & output ports
    void CancelEvents (void);                                                           //Cancel all operation events
    void EnableReset (const bool is_active);                                            //Enable/Disable reset state
    void ResetOutputPort (void);                                                        //Reset output port when reset is active

    void HandleCntClkInputMethod (void);                                                //Handle the change of clkbus_n clock (n:0->5) and nc_clk_n clock (n:1 or 2)
    void HandleTIAInputMethod (const unsigned int chan_num);                            //Handle the change in TIAx input port
    void HandleTIAtoEVOMethod (const unsigned int chan_num);                            //Handle the change in TIAx input port (after noise cancellation-if enabled) syn with CLK_LSB
    void OutputNoiseMethod (const unsigned int chan_num);                               //Handle the noise cancellation output
    void SetNCNTAMethod (const unsigned int chan_num);                                  //Handle the noise cancellation output
    void AssertEventMethod (const unsigned int event_id);                               //Set event outputs to 1
    void DeAssertEventMethod (const unsigned int event_id);                             //Clear event outputs to 0
    void AssertIntrMethod (const unsigned int interrupt_id);                            //Set interrupt port to 1
    void DeAssertIntrMethod (const unsigned int interrupt_id);                          //Set interrupt port to 0
    void StartTimerMethod (void);                                                       //Start counter of TCNTA
    void HandleTimerCompareMatchMethod (void);                                          //Handle compare match event of TCNTA
    void FreeRunCntThread (void);                                                       //Handle the free-running counter of TCNTA
    void CaptureTCNTDMethod (const unsigned int interrupt_id);

    void ControlEvents (const unsigned int event_id, const bool set_val);               //Control event output
    void DumpInterruptMsg (const std::string intr_name, const bool value);              //Dump interrupt message
    void ControlInterruptPorts (const unsigned int interrupt_id, const bool set_val);   //Control interrupt ports
    void StartStopTimer (const bool is_start);                                          //Start/stop free-running counter A
    void SetCLKBusPeriod (unsigned int clkbus_sel);                                     //Set TCNTA clock period
    void SetCLKNoisePeriod (const unsigned int chan_num);                               //Set NCNTA clock period
    void NotifyTIAChange (const unsigned int chan_num);                                 //Notify the change of TIA after noise cancellation(if any)

    bool IsModelResetActive (void);                                                     //Check active level of presetz

    double GetCurTime (void);                                                           //Get current simulation time
    double CalculateClockEdge (double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point);

    //Virtual functions of ATU5010_TIMERA_Base class
    bool IsTimerRunning (void);                                                         //Check Timer A is enable or disable
    bool IsNoiseCancelDuration (const unsigned int chan_num);                           //Check Noise cancellation is in process or not
    unsigned int GetCounterValue (void);                                                //Get current counter value of TCNTA
    unsigned int GetNoiseCounterValue (const unsigned int chan_num);                    //Get current counter value of NCNTAx
    unsigned int GetTILRAVal (void);                                                    //Get current TILRA value
    void UpdateCounterValue (void);                                                     //Update counter value of TCNTA
    void UpdateNoiseCounterValue (const unsigned int chan_num);                         //Update counter value of NCNTA
    void SetNoiseCancelMode (const unsigned int chan_num);                              //Set Noise Cancellation mode
    void SetNoiseCORVal (const unsigned int chan_num, const unsigned int cor_val);      //Set Noise Cancellation compare match value
    void EnableNoiseCancel (const unsigned int chan_num, const bool is_enable);
};

class ATU5010_NoiseCanceler: public sc_module
{
public:
    sc_event                        mResultOutEvent;
    sc_event                        mUpdateCntEvent;

    ATU5010                            *mATU5010;
private:
    sc_dt::uint64                   mCLKNOISEPeriod;                                    //Period value of noise cancellation clock
    unsigned int                    mNoiseCancelMode;
    unsigned int                    mUpdatedCntVal;
    bool                            mIsNoiseCancelDuration;                             //Noise cancellation counter status
    bool                            mSyncTIA;                                           //Input TIA in noise cancellation process
    bool                            mCurTIA;                                            //TIA after noise cancellation (if enable)/or original TIA
    bool                            mCurOutval;
    bool                            mIsNoiseTimerCorMatch;                              //Noise cancellation counter compare match
    bool                            mIsCLKNoiseUpdated;                                 //nc_clk_* is updated
    bool                            mIsNCNTAUpdated;                                    //NCNTA is updated
    bool                            mIsNoiseCntUp;                                      //Noise cancellation count mode (true: up; false: down) in Level-accumulated cancellation mode
    double                          mNoiseTimeTick;                                     //Get tick clock time at start time and compare match time
    double                          mTimeCLKChangePoint;                                //The time clock is changed (mCLKNOISEPeriod)
    bool                            mIsEnable;
    unsigned int                    mNoiseCORVal;
    bool                            mIsInputProcessing;
    bool                            mIsFallEdgeDetect;
    bool                            mTIAVal;
    std::vector<double>             mTIASyncTimeVec;
    bool                            mIsPreCLKNOISEzero;

    sc_event                        mClkZeroEvent;                                      //Notify clock is zero (0: clock bus; 1 -> 8: noise cancellation clock)
    sc_event                        mRstStateEvent;                                     //Notify reset state
    sc_event                        mNoiseEnEvent;
    sc_event                        mNoiseCancellationEvent;
    sc_event                        mStartNoiseTimerEvent;
    sc_event                        mNoiseCntEvent;
    sc_event                        mTIASyncEvent;

    ATU5010_Timer                      *mNoiseCancelCnt;
public:
    SC_HAS_PROCESS(ATU5010_NoiseCanceler);
    ATU5010_NoiseCanceler(sc_module_name name, ATU5010 *ATU5010_Ptr);
    ~ATU5010_NoiseCanceler (void);

    bool IsZeroNoiseCLk (void);
    bool GetCurOutval (void);                                                           //Sensitive with mResultOutEvent
    bool IsNoiseCancelDuration (void);                                                  //Check Noise cancellation is in process or not
    unsigned int GetUpdatedCntValue (void);                                             //Get updated counter value of NCNTAx, sensitive with mUpdateCntEvent
    unsigned int GetNoiseCounterValue (const unsigned int reg_val);                     //Get current counter value of NCNTAx

    void SetNoiseCancelMode (const unsigned int nc_mode);
    void SetNoiseCORVal (const unsigned int cor_val);
    void SetNoiseCounterValue (const unsigned int nc_cnt);
    void SetCLKNoisePeriod (const sc_dt::uint64 nc_clk, const double time_change_point);
    void NotifyNoiseCancellation (const bool nc_en, const bool input_trig, const bool input_val);
    void EnableReset (const bool is_active);                                            //Enable/Disable reset state
    void Enable (const bool is_enable);
private:
    void Initialize (void);                                                             //Initializes internal variables & output ports
    void CancelEvents (void);                                                           //Cancel all operation events

    void HandleNoiseCompareMatchMethod (void);                                          //Handle compare match event of NCNTA
    void NoiseEnMethod (void);                                                          //Handle the noise cancellation
    void NoiseCancellationMethod (void);                                                //Handle the noise cancellation
    void StartNoiseTimerMethod (void);                                                  //Start noise cancellation counter
    void NoiseCancelCntThread (void);                                                   //Handle the noise cancellation counter of NCNTA
    void TIASyncMethod (void);

    void NotifyTIAChange (void);                                                        //Notify the change of TIA after noise cancellation(if any)

    bool IsModelResetActive (void);                                                     //Check active level of presetz
    double GetCurTime (void);                                                           //Get current simulation time
    double CalculateClockEdge (double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point);
};
#endif
