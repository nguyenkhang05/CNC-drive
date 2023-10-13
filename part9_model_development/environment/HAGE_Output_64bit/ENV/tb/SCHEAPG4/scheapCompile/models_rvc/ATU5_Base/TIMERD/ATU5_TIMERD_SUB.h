// ---------------------------------------------------------------------
// $Id: ATU5_TIMERD_SUB.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5_TIMERD_SUB_H__
#define __ATU5_TIMERD_SUB_H__

#include "ATU5_TIMERD_SUB_AgentController.h"
#include "ATU5_TIMERD.h"
#include "ATU5_Base.h"
#include "ATU5_Timer.h"
class ATU5_TIMERD_SUB_Func;
class ATU5_TIMERD_SUB: public sc_module,
              public ATU5_TIMERD_SUB_AgentController
             // public BusSlaveBase<32,1>
{
friend class ATU5_Base;

private:
    enum eResetActiveLevel {
        emResetActive               = 0,
        emResetSignalNum            = 1
    };//Define active level of reset signal
public:
    //Declare input signals
    sc_in<bool> TAEO2A;
    sc_in<bool> TAEO2B;
    sc_in<bool> PHU0DFE;
    sc_in<bool> PHU1DFE;
    sc_in<bool> CLRB;
    //Declare output signals
    sc_out<bool> *TODA[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_out<bool> *TODB[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_out<bool> *TODMIG[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_out<bool> *INT_UDF[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_out<bool> INT_OVF1;
    sc_out<bool> INT_OVF2;
    sc_out<bool> *INT_CMPA[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_out<bool> *INT_CMPB[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_out<bool> *INT_FEIED[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_out<bool> *INT_REIED[ATU5_TIMERD_SUB_AgentController::emChannel];
    //Declare target socket

    SC_HAS_PROCESS (ATU5_TIMERD_SUB);

    ATU5_TIMERD_SUB (sc_module_name name,
            ATU5_TIMERD *ATU5_TIMERD_Ptr,
            unsigned int sub);
    ~ATU5_TIMERD_SUB (void);
   ATU5_Timer *mTimer_TCNT1D_CUCR1D;
   ATU5_Timer *mTimer_TCNT2D_CUCR2D;
   ATU5_Timer *mTimer_TCNT1D_OCR1D[ATU5_TIMERD_SUB_AgentController::emChannel];
   ATU5_Timer *mTimer_TCNT1D_OCR2D[ATU5_TIMERD_SUB_AgentController::emChannel];
   ATU5_Timer *mTimer_TCNT2D_OCR1D[ATU5_TIMERD_SUB_AgentController::emChannel];
   ATU5_Timer *mTimer_TCNT2D_OCR2D[ATU5_TIMERD_SUB_AgentController::emChannel];
   ATU5_Timer *mTimer_DCNTD[ATU5_TIMERD_SUB_AgentController::emChannel];

   //ATU5_Timer *mTimer_ONMAXD[ATU5_TIMERD_SUB_AgentController::emChannel];
   ATU5_Timer *mTimer_OFMICNTD  [ATU5_TIMERD_SUB_AgentController::emChannel];
   ATU5_Timer *mTimer_ONMICNTD  [ATU5_TIMERD_SUB_AgentController::emChannel];
   ATU5_Timer *mTimer_OTONMICNTD[ATU5_TIMERD_SUB_AgentController::emChannel];

    //Declare public functions
    void RunningCounter (bool is_start);
    double CalculateClockEdge (const std::string clock_name, const bool is_pos, const bool add_period, const double time_point,const double time_change_point);
    void StartStopTimer (const bool is_start);
    void StartTimerFunction (unsigned int counter);
    void StopTimerFunction (unsigned int counter, bool reg_update);
    void HandleCmpMatchCMPB (unsigned int channel);
    void HandleCmpMatchCMPA (unsigned int channel);
    void ChangeOutputPortTODA (const unsigned int channel, unsigned int port_val);
    void ClockHandleFunction (void);
    void Initialize (void);
    void EnableReset(const bool is_active);
    unsigned int GetCounterValue(unsigned int counter, unsigned int channel);
    unsigned int GetTCNTDRegisterVal (unsigned int counter);
    unsigned int mCaptureValue;
    sc_dt::uint64   mCNT1CLKPeriod;
    sc_dt::uint64   mCNT2CLKPeriod;
    sc_dt::uint64	mDCNTCLKPeriod;
 	std::string     mDCNTCLKName;
    std::string     mCNT1CLKName;
    std::string     mCNT2CLKName;
    std::string     mPCLKName;
    bool     mCUCR1DMatch;
    bool     mCUCR2DMatch;
    bool     mCLRBTCNT1D;
    bool     mCLRBTCNT2D;
    bool     mInitialStage;
    double   mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emAllCounter];
    bool     mCompareMatchCNT1OCR1[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool     mCompareMatchCNT2OCR1[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool     mCompareMatchCNT1OCR2[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool     mCompareMatchCNT2OCR2[ATU5_TIMERD_SUB_AgentController::emChannel];

    bool        mCMPARequest[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool        mCMPBRequest[ATU5_TIMERD_SUB_AgentController::emChannel];

	bool 		mIsCMA[ATU5_TIMERD_SUB_AgentController::emChannel];
	bool 		mIsCMB[ATU5_TIMERD_SUB_AgentController::emChannel];
	bool 		mIsUNF[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool		mTODB_normal[ATU5_TIMERD_SUB_AgentController::emChannel];
	bool		mTODB_type2[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool        mCounterEnable;
    bool mStartDcntWithZeroClk;
    double mTimeToNotifyStartDcntEvent;
    ATU5_TIMERD_SUB_Func       *mATU5_TIMERD_SUBFunc;                          //ATU5_TIMERD_SUB_Func class instance
private:
    void   CheckRangeCompareFunction1(unsigned int counter, unsigned int channel);
    void   CheckRangeCompareFunction2(unsigned int counter, unsigned int channel);
    void InitializePort (void);
    void CancelAllEvents (void);

    void   HandleInputCaptureMethod(void);
    void   HandleInputClearCounterMethod(void);
    void   StartTimer1Method(void);
    void   StartTimer2Method(void);
    void   KeepCUCR1DValueOvfMethod(void);
    void   KeepCUCR2DValueOvfMethod(void);
    void   PreHandleCmpMatchCUCR1DMethod(void);
    void   PreHandleCmpMatchCUCR2DMethod(void);
    void   Pre2HandleCmpMatchCUCR1DMethod(void);
    void   Pre2HandleCmpMatchCUCR2DMethod(void);
    void   HandleCmpMatchCUCR1DMethod(void);
    void   HandleCmpMatchCUCR2DMethod(void);
    void   HandleInterruptOVF1Method(void);
    void   HandleInterruptOVF2Method(void);
    void   ClearInterruptOVF1Method(void);
    void   ClearInterruptOVF2Method(void);
    void   PreHandleCmpMatchCNT1OCR1DMethod(const unsigned int channel);
    void   HandleCmpMatchCNT1OCR1DMethod(const unsigned int channel);
	void   PreHandleCmpMatchCNT2OCR1DMethod(const unsigned int channel);
    void   HandleCmpMatchCNT2OCR1DMethod(const unsigned int channel);
    void   PreHandleCmpMatchCNT1OCR2DMethod(const unsigned int channel);
    void   HandleCmpMatchCNT1OCR2DMethod(const unsigned int channel);
    void   PreHandleCmpMatchCNT2OCR2DMethod(const unsigned int channel);
    void   HandleCmpMatchCNT2OCR2DMethod(const unsigned int channel);
    void   HandleInterruptCMPAMethod(const unsigned int channel);
    void   HandleInterruptCMPBMethod(const unsigned int channel);
    void   HandleInterruptFEIEDMethod(const unsigned int channel);
    void   HandleInterruptREIEDMethod(const unsigned int channel);
    void   ClearInterruptCMPAMethod(const unsigned int channel);
    void   ClearInterruptCMPBMethod(const unsigned int channel);
    void   ClearInterruptFEIEDMethod(const unsigned int channel);
    void   ClearInterruptREIEDMethod(const unsigned int channel);
    void   HandleOutputPortTODAMethod(const unsigned int channel);
    void   TriggerDCNTMethod(const unsigned int channel);
    void   AssertmIsCMAMethod(const unsigned int channel);
    void   AssertmIsCMBMethod(const unsigned int channel);
    void   AssertmIsUNFMethod(const unsigned int channel);
    void   StartDcntMethod(const unsigned int channel);
    void   ClearDSR1D_DSFDMethod(const unsigned int channel);
    void   WriteTODBMethod(const unsigned int channel);
    void   ProcessMIGMethod(const unsigned int channel);
    void   WriteTODMIGMethod(const unsigned int channel);
    void   StopDcntMethod (const unsigned int ch_id);
    void   AssertTODB_normalMethod(const unsigned int channel);
    void   DeassertTODB_normalMethod(const unsigned int channel);
    void   StartDCNTDReg (const unsigned int channel);
    void   StartDCNTDMethod (const unsigned int channel);
    void   UpdateDCNTDMethod (const unsigned int channel);
    void   WriteCompareMatchValue(unsigned int counter);//ducla add
    void   UpdateDCNTD(unsigned int channel);//ducla add
    void   WriteTCNTD1(void);//ducla add
    void   WriteTCNTD2(void);//ducla add
    void   ProcessSetting(unsigned int channel);//ducla add
    void   WriteCounterValue(unsigned int counter);//ducla add
    
    void   AssertINT_UDFMethod(const unsigned int channel);
    void   DeassertINT_UDFMethod(const unsigned int channel);

    void   WriteCompareMatchValueMethod(const unsigned int counter);
    void   WriteCounterValueMethod(const unsigned int counter);

    void   InputCaptureWriteMethod(void);
    
    void StartTimerOFMICNTDMethod (const unsigned int channel);
    void StartTimerONMICNTDMethod (const unsigned int channel);
    void StartTimerOTOMICNTDMethod (const unsigned int channel);
    void StopTimerOFMICNTDMethod (const unsigned int channel);
    void StopTimerONMICNTDMethod (const unsigned int channel);
    void StopTimerOTOMICNTDMethod (const unsigned int channel);
    void ProcessPriorityMethod(const unsigned int channel);//add by ducla
    sc_event    mStartTimerEvent;
    sc_event    mStartTimer1Event;
    sc_event    mStartTimer2Event;
    sc_event    mCompareMatchCUCR1DEvent;
    sc_event    mCompareMatchCUCR2DEvent;
    sc_event    mInterruptOutputOVF1Event;
    sc_event    mInterruptOutputOVF2Event;
    sc_event    mClearInterrupt1Event;
    sc_event    mClearInterrupt2Event;
    sc_event    mCheckCompareMatchCUCR1DEvent;
    sc_event    mCheckCompareMatchCUCR2DEvent;
    sc_event    mCmpMatchCNT1OCR1DEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mCmpMatchCNT2OCR1DEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mCmpMatchCNT1OCR2DEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mCmpMatchCNT2OCR2DEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mInterruptOutputCMPAEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mInterruptOutputCMPBEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mInterruptOutputFEIEDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mInterruptOutputREIEDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mClearInterruptOutputCMPAEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mClearInterruptOutputCMPBEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mClearInterruptOutputFEIEDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mClearInterruptOutputREIEDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mChangeOutputPort_TODAEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mInputCaptureEvent;
    bool IsTimerRunning;
    sc_event    mStartDCNTDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];

    sc_event    mAssertTODB_normalEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mDeassertTODB_normalEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    
    bool        mTODxValue[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mTODxChange_Event[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool        mTODMIGValue[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mTriggerTODMIG_Event[ATU5_TIMERD_SUB_AgentController::emChannel];

	sc_event    mInvertedRegIsWriteEvent[ATU5_TIMERD_SUB_AgentController::emChannel];

    sc_event 	mCompareMatchAEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
 	sc_event 	mCompareMatchBEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mCompareMatchAEventDly[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mCompareMatchBEventDly[ATU5_TIMERD_SUB_AgentController::emChannel];
 	sc_event 	mStartDCNTTriggerEvent[ATU5_TIMERD_SUB_AgentController::emChannel];

	sc_event	mStartDcntEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
	sc_event	mStopDcntEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
	sc_event	mClearDSR1D_DSFDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mAssertINT_UDFEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mDeassertINT_UDFEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mStartOFMICNTDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mStartONMICNTDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    sc_event    mStartOTOMICNTDEvent[ATU5_TIMERD_SUB_AgentController::emChannel];
    
    bool        mIsOFMICNTZero(unsigned int chnnl);
    bool        mIsONMICNTZero(unsigned int chnnl);
    bool        mIsOTONMICNTZero(unsigned int chnnl);

    bool        mIsRaisingRequest[ATU5_TIMERD_SUB_AgentController::emChannel];
    bool        mIsFailingRequest[ATU5_TIMERD_SUB_AgentController::emChannel];
    double      mCUCR1DOvfTime;
    double      mCUCR2DOvfTime;
    bool        mIsCUCR1DOvf;
    bool        mIsCUCR2DOvf;

public:
    void        mOFMIZeroEventFunc(unsigned int chnnl);
    void        mONMIZeroEventFunc(unsigned int chnnl);
    void        mOTONMIZeroEventFunc(unsigned int chnnl);
};
#endif
