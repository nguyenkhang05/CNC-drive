// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERF_Sub.h,v 1.3 2017/06/28 10:15:45 HuyDT1 Exp $
//
// Copyright(c) 2016-2017 Renesas Electronics Corporation
// Copyright(c) 2016-2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_TIMERF_SUB_H__
#define __ATU5010_TIMERF_SUB_H__

#include "ATU5010_TIMERF_Sub_AgentController.h"
#include "ATU5010_TIMERF_AgentController.h"
#include "ATU5010.h"


class ATU5010_TIMERF_Sub_Func;
class ATU5010_Timer;
class ATU5010_NoiseCanceler;

class ATU5010_TIMERF_Sub: public sc_module,
            public ATU5010_TIMERF_Sub_AgentController
{
friend class ATU5010_TIMERF;
friend class ATU5010_TIMERF_Func;
friend class Cgeneral_timer;

private:
    ATU5010_TIMERF_Sub_Func *mATU5010_TIMERF_Sub_Func;      // ATU5010_TIMERF_Sub_Func class instance

    /// Declare variables
    std::string mModuleName;

    sc_dt::uint64 mCntCLKPeriod;                 //Period value of clock
    sc_dt::uint64 mCntCLKFreq;                   //Frequency value of clock
    sc_dt::uint64 mCntCLKOrgFreq;                //Original frequency value of clock
    std::string   mCntCLKFreqUnit;               //Frequency unit of clock
    std::string   mClkName;                      //name of clock bus
    double mTimeCLKChangePoint;
    double mTimeCLK_NOISEChangePoint;

    sc_event mCntCLKZeroEvent;
    sc_event mClearECNTBF_isIncreasedEvent;
    sc_event mClearECNTAF_isIncreasedEvent;

    sc_event mCntCLKUpdatedEvent;
    sc_event mECNTAFUpdatedEvent;
    sc_event mECNTCFUpdatedEvent;
    sc_event mGRAFUpdatedEvent;
    sc_event mGRBFUpdatedEvent;
    sc_event mGRDFUpdatedEvent;

    SC_SIGNAL(bool) signalA;
    SC_SIGNAL(bool) signalA_sync;
    SC_SIGNAL(bool) signalB;

    /// Declare events
    sc_event    mAssertSignalA_syncEvent;
    sc_event    mDeassertSignalA_syncEvent;

    sc_event mECNTAFCMEvent;
    sc_event mECNTCFCMEvent;
    //Timer Variable
    bool    mECNTCFisCM;
    bool    mIsCMA;                         //Compare Match A occurs
    bool    mIsCMB;                         //Compare Match B occurs
    bool    mECNTBF_isIncreased;            //increased ECNTBF
    bool    mECNTAF_isIncreased;            //increased ECNTAF
    double  mCmpATime;
    double  mCmpCTime;
    bool    mIsCmpA;
    bool    mIsCmpC;
    //Timer event
    sc_event ClearECNTBFEvent;              //Clear ECNTBF event
    sc_event mECNTAFOverFlowEvent;
    sc_event mECNTCFOverFlowEvent;

    sc_event mStartTimerAEvent;                      //Start timer A event
    sc_event mStopTimerAEvent;                        //Stop timer A event
    sc_event mStopTimerCEvent;                        //Stop timer C event
    sc_event mStartTimerCEvent;                       //Start timer C event
    sc_event mIncreaseECNTBFEvent;                   //Increase ECNTBF event
    sc_event mDecreaseECNTBFEvent;                   //Decrease ECNTBF event
    sc_event mCMECNTBFEvent;                         //Compare match ECNTBF = GRBF event
    //Overflow event
    sc_event mOVFBEvent;                             //event of overflow ECNTBF

    //Mode Events
    sc_event mEndEdgeCntInGivenTimeEvent;      //EdgeCntInGivenTime end processing
    sc_event mEndEdgeIntCntEvent;              //EdgeIntCnt end processing
    sc_event mEndTimeDurInputLevelEvent;       //TimeDurInputLevel end processing
    sc_event mEndPwmWaveTimeEvent;             //PwmWaveTime end processing
    sc_event mEndRotSpeedPulseEvent;           //RotSpeedPulse end processing
    sc_event mEndUpDownCntEvent;               //UpDownCnt end processing
    sc_event mEndUpDownCntx4Event;             //UpDownCntx4 end processing

    sc_event mEdgeCntInGivenTimeEvent;      //EdgeCntInGivenTime processing
    sc_event mEdgeIntCntEvent;              //EdgeIntCnt processing
    sc_event mTimeDurInputLevelEvent;       //TimeDurInputLevel processing
    sc_event mPwmWaveTimeEvent;             //PwmWaveTime processing
    sc_event mRotSpeedPulseEvent;           //RotSpeedPulse processing
    sc_event mUpDownCntEvent;               //UpDownCnt processing
    sc_event mUpDownCntx4Event;             //UpDownCntx4 processing

    sc_event mAssertOVFFIEvent;             //Call AssertOVFFIMethod
    sc_event mDeassertIOVFIEvent;           //Call DeassertIOVFIMethod
    sc_event mAssertICFFIEvent;             //Call AssertICFFIMethod
    sc_event mDeassertICFFIEvent;           //Call DeassertICFFIMethod

public:
    /// Declare input signals
    sc_in<bool>     tifa;
    sc_in<bool>     tifb;
    sc_in<bool>     *tia[8];

    /// Declare output signals
    sc_out<bool>    ovffi;
    sc_out<bool>    icffi;
    sc_out<bool>    *poe_sub[2];

    SC_HAS_PROCESS (ATU5010_TIMERF_Sub);
    ATU5010_TIMERF_Sub (sc_module_name name,
                ATU5010_TIMERF *ATU5010_TIMERF_Ptr,
                unsigned int ad_id);
    ~ATU5010_TIMERF_Sub (void);

    void EnableReset(const bool is_active);
    ///Noise canceller
    ATU5010_NoiseCanceler      *mNoiseCancelerA;
    ATU5010_NoiseCanceler      *mNoiseCancelerB;
    sc_dt::uint64                   mCLKNOISEPeriod;           //Period value of noise cancellation clock

    unsigned int GetNoiseCounterValueA (void);
    bool IsNoiseCancelDurationA(void);
    void StartNoiseCancellerA (bool inputval);
    void OutputNoiseMethodA (void);
    void SetNCNTFAMethod (void);

    unsigned int GetNoiseCounterValueB (void);
    bool IsNoiseCancelDurationB(void);
    void StartNoiseCancellerB (bool inputval);
    void OutputNoiseMethodB (void);
    void SetNCNTFBMethod (void);
    void SetCLKNoisePeriod(void);

    void SetNoiseCORValB (void);
    void SetNoiseCounterValueB (void);

    void SetNoiseCORValA (void);
    void SetNoiseCounterValueA (void);

private:
    void NotifyEvent (const unsigned int event);
    void HandleOVFbyGTimerAMethod(void);
    void HandleOVFbyGTimerCMethod(void);
    void HandleCMTimerAMethod(void);
    void HandleCMTimerCMethod(void);

    void ClearECNTBF_isIncreasedMethod(void);
    void ClearECNTAF_isIncreasedMethod(void);

    void HandlePOE0Method(void);
    void HandlePOE1Method(void);
    /// Declare methods
    void SlectInputAMethod (void);
    void HandleTIFBMethod(void);
    void SelectClockInput (void);

    //Timer methods
    void ClearECNTBFMethod(void);

    void CompareMatchAMethod(void);
    void CompareMatchBMethod(void);
    void CntEdgeBMethod(void);
    void StartTimerCMethod(void);

    void StartTimerAMethod(void);
    void StopTimerAMethod(void);
    void StopTimerCMethod(void);
    void CntEdgeAMethod(void);
    void IncreaseECNTBFMethod(void);
    void DecreaseECNTBFMethod(void);
    void LevelAMethod(void);
    void SignalAisChangeMethod(void);
    void AssertSignalA_syncMethod(void);
    void DeassertSignalA_syncMethod(void);

    //Overflow methods
    void OVFAMethod(void);
    void OVFBMethod(void);
    void OVFCMethod(void);

    void StartOperationMethod (void);
    void EndOperationMethod (void);

    void EdgeCntInGivenTimeMethod (void);
    void EdgeIntCntMethod (void);
    void TimeDurInputLevelMethod (void);
    void PwmWaveTimeMethod (void);
    void RotSpeedPulseMethod (void);
    void UpDownCntMethod (void);
    void UpDownCntx4Method (void);

    void EndEdgeCntInGivenTimeMethod (void);
    void EndEdgeIntCntMethod (void);
    void EndTimeDurInputLevelMethod (void);
    void EndPwmWaveTimeMethod (void);
    void EndRotSpeedPulseMethod (void);
    void EndUpDownCntMethod (void);
    void EndUpDownCntx4Method (void);

    void HandleTILOAMethod (void);
    void HandleTIFAInputMethod (void);
    void HandleTIFBInputMethod (void);

    void AssertOVFFIMethod (void);
    void DeassertIOVFIMethod (void);
    void AssertICFFIMethod (void);
    void DeassertICFFIMethod (void);

    void StartTriggerMethod (void);
    void EndTriggerMethod (void);

    void CancelEvents (void);
    void Initialize (void);
    void SetCntPeriod(const double counter_period);
    void SetupCounter(const std::string counter_name, unsigned int counter_value,unsigned int cm_value, double period);
};
#endif
