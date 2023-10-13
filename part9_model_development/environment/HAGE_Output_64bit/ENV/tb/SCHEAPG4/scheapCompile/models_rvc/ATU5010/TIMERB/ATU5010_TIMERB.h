// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERB.h,v 1.1 2017/07/11 10:15:45 binhnguyen Exp $
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
#ifndef __ATU5010_TIMERB_H__
#define __ATU5010_TIMERB_H__

#include "ATU5010_AgentController.h"
#include "re_define.h"
#include "ATU5010_Timer.h"

class ATU5010;
class ATU5010_TIMERB_Func;
class ATU5010_TIMERB: public sc_module
{
friend class ATU5010;
friend class ATU5010_TIMERB_Func;
#include "ATU5010_TIMERB_cmdif.h"
public:
    //Port declaration
    sc_in<uint64>                   clkbus_0;
    sc_in<uint64>                   clkbus_1;
    sc_in<uint64>                   clkbus_2;
    sc_in<uint64>                   clkbus_3;
    sc_in<bool>                     ev_in_1_0;
    sc_in<bool>                     ev_in_1_1;
    sc_in<bool>                     ev_in_1_2;
    sc_in<bool>                     ev_in_1B;
    sc_in<bool>                     ev_in_1C;
    sc_in<bool>                     ev_in_1D;
    sc_in<bool>                     ev_in_1E;
    sc_in<bool>                     ev_in_1F;
    sc_in<bool>                     ev_in_1G;
    sc_in<bool>                     ev_in_1H;
    sc_in<bool>                     ev_in_1I;
    sc_out<bool>                    ICIB;
    sc_out<bool>                    CMIB0;
    sc_out<bool>                    CMIB1;
    sc_out<bool>                    CMIB6;
    sc_out<bool>                    CMIB10;
    sc_out<bool>                    CMIB11;
    sc_out<bool>                    CMIB12;
    sc_out<bool>                    CMIB6M;
    sc_out<bool>                    CMIB6E;
    sc_out<sc_dt::uint64>           AGCKM;
    sc_out<sc_dt::uint64>           AGCK1;
    sc_out<bool>                    AGCKTIM;
    sc_out<bool>                    TCNTB6CM;                                               //B -> A (Maybe this port is removed)
    sc_out<bool>                    TCNTDCLR;

    enum eCLKID {    // Define clock index
        emCLKBUSnum = 4,
        emBlocknum = 3,
        emSeqStatenum = 24,
        emClknum = 2
    };
    enum eInterruptId {emCMIB0=0, emICIB0, emCMIB1, emCMIB6, emCMIB10, emCMIB11, emCMIB12, emCMIB6M, emCMIB6E, emInterruptNum};
    enum eEventId {emEVO1_0=0, emEVO1_1, emEVO1_2, emEVO1B, emEVO1C, emEVO1D, emEVO1E, emEVO1F, emEVO1G, emEVO1H, emEVO1I, emEventNum};

    ATU5010                            *mATU5010;
    ATU5010_TIMERB_Func                *mATU5010_TIB_Func;
private:
    ATU5010_Timer                      *mTCNTB0Cnt[emBlocknum];
    ATU5010_Timer                      *mTCNTB5Cnt;
    //Declare variables
    sc_dt::uint64                   mCLKBUSPeriod;                                          //Period value of counter clock (TCNTB0/TCNTB2 clock)
    sc_dt::uint64                   mAGCK1Period;                                           //Period value of AGCK1 clock
    sc_dt::uint64                   mAGCKMPeriod;                                           //Period value of AGCKM clock
    sc_dt::uint64                   mCLKBUS_nPeriod[emCLKBUSnum];                           //Period value of clkbus_n (n: 0 -> 3)
    bool                            mIsTIBEnDontCareClock;                                  //Timer B enable
    bool                            mIsTIBEn;                                               //Timer B enable
    bool                            mIsSeqEn;                                               //Sequencer status
    bool                            mEventInVal[emEventNum];                                //ev_in_1* value
    bool                            mAGCKVal;                                               //AGCK value (selected from ev_in_1_*)
    bool                            mIsAGCKTrig;                                            //AGCK event assert
    bool                            mIsCLKBusUpdated[emBlocknum];                           //clkbus_* value is updated by users
    bool                            mIsTCNTB2Updated;                                       //TCNTB2 is updated by users
    bool                            mIsPIMUpdated;                                          //PIM is updated by users
    bool                            mIsTCNTB0Running[emBlocknum];                           //TCNTB0/TCNTB0S1/TCNTB0S2 counter status
    bool                            mIsTCNTB0CorMatch[emBlocknum];                          //TCNTB0/TCNTB0S1/TCNTB0S2 compare match
    bool                            mIsTCNTB0OVF[emBlocknum];                               //TCNTB0/TCNTB0S1/TCNTB0S2 overflow
    bool                            mIsTCNTB0Updated[emBlocknum];                           //TCNTB0/TCNTB0S1/TCNTB0S2 is updated by users
    bool                            mIsTCNTB1CorMatch[emBlocknum];                          //Compare match between TCNTB1 and OCRB1 occurs
    bool                            mIsTCNTB11CorMatch[emBlocknum];                         //Compare match between TCNTB1 and OCRB11 occurs
    bool                            mIsTCNTB1ClrReq[2][emBlocknum];                         //TCNTB1 clear request (according TCRB.CLRB1 and TCRB.CLRB1SEL setting): mIsTCNTB1ClrReq[0][] ~ compare match between TCNTB1 and OCRB10
    bool                            mIsTCNTB5AutoCnt;                                       //TCNTB5 auto counting
    bool                            mIsTCNTB5CorMatch;                                      //TCNTB5 compare match
    bool                            mIsTCNTB5OVF;                                           //TCNTB5 overflow
    bool                            mIsTCNTB5Updated;                                       //TCNTB5 is updated
    bool                            mIsCLKLSBUpdated;                                       //CLK_LSB is updated
    bool                            mIsTCNTB6CORMatch;                                      //TCNTB6 and OCRB6 compare match
    bool                            mIsCMIB6Req[2];                                         //CMIB6 interrupt request (0: checked at 1st AGCK; 1: checked at 2nd AGCK)
    bool                            mIsTCNTB2LoadState[3];                                  //TCNTB2 load state (0: TCNTB2 -= PIM; 1: TCNTB2 += RLDB; 2: TCNTB2 = ICRB0 and RLDB = ICRB0 - PIM
    double                          mTimeTick[emBlocknum];                                  //Get tick clock time at start time and compare match time of TCNTB0
    double                          mTimeTick2;                                             //Get tick clock time TCNTB2 is load
    double                          mTimeTick3;                                             //Get tick clock time TCNTB3 is load
    double                          mTimeTick5;                                             //Get tick clock time TCNTB5 is load
    double                          mTimeCLKChangePoint[emClknum];                          //The time clock is changed (0: mCLKBUSPeriod; 1: mAGCK1Period;)
    unsigned int                    mCurTCNTB0[emBlocknum];                                 //Current TCNTB0/TCNTB0S1/TCNTB0S2 value at event trigger time
    unsigned int                    mCurICRB1[emBlocknum];                                  //Current ICRB1/ICRB1S1/ICRB1S2 value at the time TCNTB1 and OCRB1 occurs
    unsigned int                    mCurTCNTB1;                                             //Current TCNTB1 value at the time ev_in_1* is assert
    unsigned int                    mCurTCNTB2;                                             //Current TCNTB2 value at the loaded time
    unsigned int                    mCurPIM;                                                //Current PIM value at the loaded time
    unsigned int                    mIsTCNTB3ClrReq;                                        //0: None; 1: Compare match TCNTB1 and OCRB10 occurred; 2: Compare match TCNTB6 and OCRB6 occurred; 3: Compare match TCNTB3 and OCRB8 occurred
    bool                            mIsWrapperCall;
    bool                            mIsPreCLKBUSzero;
    bool                            mIsPSEnable;
    bool                            mIsAGCK1Update;
    bool                            mIsTCNTB5AutoCntTrg;
    bool                            mTCNTDCLRVal;

    sc_event                        mAGCKEvent;                                             //Notify AGCK is assert
    sc_event                        mTCNTB1CapEvent[8];                                     //Notify ev_in_1* is assert and capture event counter TCNTB1
    sc_event                        mIntrActiveEvent[emInterruptNum];                       //Call update of interrupt port
    sc_event                        mIntrInActiveEvent[emInterruptNum];                     //Call update of interrupt port
    sc_event                        mTCNTB2LoadEvent;                                       //Notify TCNTB2 is updated
    sc_event                        mTCNTB2CntUpEvent;                                      //Notify TCNTB2 is counted up
    sc_event                        mTCNTB3LoadEvent;                                       //Notify TCNTB3 is cleared
    sc_event                        mTCNTB5LoadEvent;                                       //Notify TCNTB6 is updated
    sc_event                        mTCNTB6LoadEvent;                                       //Notify TCNTB6 is updated
    sc_event                        mTCNTB1COREvent[emBlocknum];                            //Notify TCNTB1/TCNTB1S1/TCNTB1S2 with OCRB1/OCRB1S1/OCRB1S2 compare match is occurred
    sc_event                        mStartTCNTB0Event[emBlocknum];                          //Notify TCNTB0/TCNTB0S1/TCNTB0S2 is started
    sc_event                        mStartTCNTB5Event;                                      //Notify TCNTB5 auto count up start
    sc_event                        mTCNTB0CntEvent[emBlocknum];                            //Notify TCNTB0/TCNTB0S1/TCNTB0S2 with OCRB0/OCRB0S1/OCRB0S2 compare match is occurred or TCNTB0 is updated (count value, count clock) by users
    sc_event                        mTCNTB5CntEvent;                                        //Notify TCNTB5 is updated (count value, count clock) by users
    sc_event                        mClkZeroEvent;                                          //Notify bus clock is zero
    sc_event                        mRstStateEvent;                                         //Notify reset state
    sc_event                        mWriteTCNTDCLREvent;
    sc_event                        mClearAGCKMEvent;
    sc_event                        mAssertAGCKTIMEvent;
    sc_event                        mDeassertAGCKTIMEvent;

public:
    SC_HAS_PROCESS(ATU5010_TIMERB);
    ATU5010_TIMERB(sc_module_name name, ATU5010 *ATU5010_Ptr);
    ~ATU5010_TIMERB (void);

    unsigned int GetCounterValue (const std::string cnt_name, const unsigned int block_num);
    bool IsTIBEn (void);                                                                    //Check Timer B is enable or disable
    bool IsTCNTB0Running (const unsigned int block_num);                                    //Check TCNTB0 is enable or disable
    bool IsTCNTB11COR (const unsigned int block_num);                                       //Check TCNTB1 and OCRB11 is match or not
    void EnableTIB (const bool is_start);                                                   //Enable/Disable Timer B
    void RegisterHandlerTIB (const std::vector<std::string> cmd);                           //Handle accessing registers
    void NotifyUpdatedCntVal (const std::string cnt_name, const unsigned int block_num);    //Update counter value when users write into counter register
    void NotifyUpdatedPIMVal (void);                                                        //Update PIM when PIM1 is selected
    void NotifyTCNTB6COR (void);                                                            //Notify compare match occurred between TCNTB6 and OCRB6 when TCNTB6 is updated
    void NotifyCMFB6M (void);                                                               //Assert CMIB6M
    void NotifyTCNTB3_8 (void);                                                             //Notify TCNTB3 and OCRB8 is match
    void SetTCNTB0CORVal (const unsigned int block_num, const unsigned int cor_val);        //Set TCNTB0/TCNTB0S1/TCNTB0S2 compare match value
    void EnableSeq(const bool is_start);                                                    //Enable/disable Sequencer
    void EnableTCNTB5AutoCounting(const bool is_start);                                     //Enable/disable TCNTB5 auto counting
    void NotifyUpdatedTCCLRBVal (const unsigned int value);
private:
    void Initialize (void);                                                                 //Initializes internal variables & output ports
    void CancelEvents (void);                                                               //Cancel all operation events
    void ResetOutputPort (void);                                                            //Reset output port when reset is active
    void EnableReset (const bool is_active);                                                //Enable/Disable reset state

    void HandleCntClkInputMethod (void);                                                    //Handle the change of clkbus_n clock (n:0->3)
    void HandleEventInputMethod (void);                                                     //Handle the change of ev_in_1*
    void HandleAGCKMethod (void);                                                           //Handle the change of AGCK
    void HandleTCNTB1CapMethod (const unsigned int event_id);                               //Handle ev_in_1* is assert and capture event counter TCNTB1
    void AssertIntrMethod (const unsigned int interrupt_id);                                //Set interrupt port to 1
    void DeAssertIntrMethod (const unsigned int interrupt_id);                              //Set interrupt port to 0
    void HandleAssertAGCKTIMMethod (void);
    void HandleDeassertAGCKTIMMethod (void);
    void HandleTCNTB2LoadMethod (void);                                                     //Handle TCNTB2 load value
    void HandleTCNTB3LoadMethod (void);                                                     //Handle TCNTB3 load value
    void HandleTCNTB5LoadMethod (void);                                                     //Handle TCNTB5 load value
    void HandleTCNTB6LoadMethod (void);                                                     //Handle TCNTB6 load value
    void HandleTCNTB0CorMatchMethod (const unsigned int block_num);                         //Handle compare match event of TCNTB0/TCNTB0S1/TCNTB0S2 with OCRB0/OCRB0S1/OCRB0S2
    void HandleTCNTB1CorMatchMethod (const unsigned int block_num);                         //Handle compare match event of TCNTB1/TCNTB1S1/TCNTB1S2 with OCRB1/OCRB1S1/OCRB1S2
    void HandleTCNTB5CorMatchMethod (void);                                                 //Handle compare match event of TCNTB5 with TCCLRB
    void HandleTCNTB0OVFMethod (const unsigned int block_num);                              //Handle overflow event of TCNTB0/TCNTB0S1/TCNTB0S2
    void HandleTCNTB5OVFMethod (void);                                                      //Handle overflow event of TCNTB5
    void HandleCLKLSBUpdatedMethod (void);                                                  //Handle CLK_LSB clock is updated
    void StartTCNTB0Method (const unsigned int block_num);                                  //Handle starting TCNTB0/TCNTB0S1/TCNTB0S2 counter
    void StartTCNTB5Method (void);                                                          //Handle starting TCNTB5 auto count up start
    void TCNTB0CntThread (void);                                                            //Handle TCNTB0 counter process
    void TCNTB0S1CntThread (void);                                                          //Handle TCNTB0S1 counter process
    void TCNTB0S2CntThread (void);                                                          //Handle TCNTB0S2 counter process
    void TCNTB5CntThread (void);                                                            //Handle TCNTB5 counter process
    void TCNTB2CntUpMethod (void);                                                          //Handle TCNTB2 CountUp process
    void WriteTCNTDCLRMethod (void);
    void ClearAGCKMMethod (void);

    void DumpInterruptMsg (const std::string intr_name, const bool value);                  //Dump interrupt message
    void ControlInterruptPorts (const unsigned int interrupt_id, const bool set_val);       //Control interrupt ports
    void SetCLKBusPeriod (unsigned int clkbus_sel);                                         //Set TCNTB0/TCNTB2 clock period
    void SetCLKAGCK1Period (void);                                                          //Set AGCK1 clock period
    void TCNTB1CntUp (const unsigned int block_num);                                        //Count up TCNTB1/TCNTB1S1/TCNTB1S2
    void TCNTB2LoadInit (void);                                                             //Update load value for TCNTB2

    bool IsModelResetActive (void);                                                         //Check active level of presetz
    unsigned int GetEIMBlkNum (void);                                                       //Get Edge Interval Measuring Block number

    double GetCurTime(void);                                                                //Get current simulation time
    double CalculateClockEdge (double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point);
    std::string GetTimeUnitStr (sc_time_unit time_u);
    void CheckTCNTB5Update (void);
};
#endif
