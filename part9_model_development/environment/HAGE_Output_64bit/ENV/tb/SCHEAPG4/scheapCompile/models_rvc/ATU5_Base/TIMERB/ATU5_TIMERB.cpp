// ---------------------------------------------------------------------
// $Id: ATU5_TIMERB.cpp 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#include <string>
#include "ATU5_Base.h"
#include "ATU5_TIMERB.h"
#include "ATU5_TIMERB_Func.h"

/*********************************
// Function     : ATU5_TIMERB
// Description  : Constructor of ATU5_TIMERB class
// Parameter    :
//      name            Module name
//      ATU5_Ptr        ATU5_Base pointer
// Return value : None
**********************************/
ATU5_TIMERB::ATU5_TIMERB(sc_module_name name, ATU5_Base *ATU5_Ptr):
                   sc_module(name),
                   clkbus_0("clkbus_0"),
                   clkbus_1("clkbus_1"),
                   clkbus_2("clkbus_2"),
                   clkbus_3("clkbus_3"),
                   ev_in_1_0("ev_in_1_0"),
                   ev_in_1_1("ev_in_1_1"),
                   ev_in_1_2("ev_in_1_2"),
                   ev_in_1B("ev_in_1B"),
                   ev_in_1C("ev_in_1C"),
                   ev_in_1D("ev_in_1D"),
                   ev_in_1E("ev_in_1E"),
                   ev_in_1F("ev_in_1F"),
                   ev_in_1G("ev_in_1G"),
                   ev_in_1H("ev_in_1H"),
                   ev_in_1I("ev_in_1I"),
                   ICIB("ICIB"),
                   CMIB0("CMIB0"),
                   CMIB1("CMIB1"),
                   CMIB6("CMIB6"),
                   CMIB10("CMIB10"),
                   CMIB11("CMIB11"),
                   CMIB12("CMIB12"),
                   CMIB6M("CMIB6M"),
                   CMIB6E("CMIB6E"),
                   AGCKM("AGCKM"),
                   AGCK1("AGCK1"),
                   AGCKTIM("AGCKTIM"),
                   TCNTB6CM("TCNTB6CM"),
                   TCNTDCLR("TCNTDCLR")
{//{{{
    assert(ATU5_Ptr != NULL);
    mATU5 = ATU5_Ptr;

    std::ostringstream blocknum;
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        blocknum.str("");
        blocknum << "mTCNTB0Cnt" << block_num;
        mTCNTB0Cnt[block_num] = new ATU5_Timer(blocknum.str().c_str(), mATU5);
        sc_assert(mTCNTB0Cnt[block_num] != NULL);
        mTCNTB0Cnt[block_num]->SetupGeneralTimer(false, true);
        mTCNTB0Cnt[block_num]->SetCounterMode(true);//Free-running mode
        mTCNTB0Cnt[block_num]->SetCompareMatchValue(0xFFFFFFFF);
    }

    mATU5_TIB_Func = new ATU5_TIMERB_Func((std::string)name, this);
    sc_assert(mATU5_TIB_Func != NULL);
    //Initial value of the ports----------------
    ICIB.initialize(false);
    CMIB0.initialize(false);
    CMIB1.initialize(false);
    CMIB6.initialize(false);
    CMIB10.initialize(false);
    CMIB11.initialize(false);
    CMIB12.initialize(false);
    CMIB6M.initialize(false);
    CMIB6E.initialize(false);
    AGCKM.initialize(0);
    AGCK1.initialize(0);
    AGCKTIM.initialize(false);
    TCNTB6CM.initialize(false);
    TCNTDCLR.initialize(false);
    //Initial variables-------------------------
    mIsPreCLKBUSzero = true;
    Initialize();
    for (unsigned int i = 0; i < emClknum; i++) {
        mTimeCLKChangePoint[i] = 0;
    }
    //------------------------------------------
    SC_METHOD(HandleCntClkInputMethod);
    dont_initialize();
    sensitive << clkbus_0;
    sensitive << clkbus_1;
    sensitive << clkbus_2;
    sensitive << clkbus_3;

    SC_METHOD(HandleEventInputMethod);
    dont_initialize();
    sensitive << ev_in_1_0;
    sensitive << ev_in_1_1;
    sensitive << ev_in_1_2;
    sensitive << ev_in_1B;
    sensitive << ev_in_1C;
    sensitive << ev_in_1D;
    sensitive << ev_in_1E;
    sensitive << ev_in_1F;
    sensitive << ev_in_1G;
    sensitive << ev_in_1H;
    sensitive << ev_in_1I;

    SC_METHOD(HandleAGCKMethod);
    dont_initialize();
    sensitive << mAGCKEvent;

    for (unsigned int event_id = 0; event_id < 8; event_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mTCNTB1CapEvent[event_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERB::HandleTCNTB1CapMethod, this, event_id),
                          sc_core::sc_gen_unique_name("HandleTCNTB1CapMethod"), &opt);
    }

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIntrActiveEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERB::AssertIntrMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("AssertIntrMethod"), &opt);
    }

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIntrInActiveEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERB::DeAssertIntrMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("DeAssertIntrMethod"), &opt);
    }
    
    SC_METHOD(HandleAssertAGCKTIMMethod);
    dont_initialize();
    sensitive << mAssertAGCKTIMEvent;

    SC_METHOD(HandleDeassertAGCKTIMMethod);
    dont_initialize();
    sensitive << mDeassertAGCKTIMEvent;

    SC_METHOD(HandleAGCKReloadB2Method);
    dont_initialize();
    sensitive << mAGCKReloadB2Event;

    SC_METHOD(HandleTCNTB3LoadMethod);
    dont_initialize();
    sensitive << mTCNTB3LoadEvent;

    SC_METHOD(HandleTCNTB6LoadMethod);
    dont_initialize();
    sensitive << mTCNTB6LoadEvent;

    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTCNTB0Cnt[index]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERB::HandleTCNTB0CorMatchMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTCNTB0CorMatchMethod"), &opt);
    }

    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTCNTB1COREvent[index]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERB::HandleTCNTB1CorMatchMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTCNTB1CorMatchMethod"), &opt);
    }


    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTCNTB0Cnt[index]->mOverflowEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERB::HandleTCNTB0OVFMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTCNTB0OVFMethod"), &opt);
    }


    SC_METHOD(HandleCLKLSBUpdatedMethod);
    dont_initialize();
    sensitive << mATU5->mCLKLSBUpdatedEvent;

    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartTCNTB0Event[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERB::StartTCNTB0Method, this, index),
                          sc_core::sc_gen_unique_name("StartTCNTB0Method"), &opt);
    }

    SC_THREAD(TCNTB0CntThread);
    dont_initialize();
    sensitive << mTCNTB0CntEvent[0];

    SC_THREAD(TCNTB0S1CntThread);
    dont_initialize();
    sensitive << mTCNTB0CntEvent[1];

    SC_THREAD(TCNTB0S2CntThread);
    dont_initialize();
    sensitive << mTCNTB0CntEvent[2];
    
    SC_METHOD(WriteTCNTDCLRMethod);
    dont_initialize();
    sensitive << mWriteTCNTDCLREvent;
    
    SC_METHOD(ClearAGCKMMethod);
    dont_initialize();
    sensitive << mClearAGCKMEvent;

    SC_METHOD(ClearAGCK1Method);
    dont_initialize();
    sensitive << mClearAGCK1Event;

    SC_THREAD(B2Thread);
    dont_initialize();
    sensitive << mB2Event;

    SC_METHOD(B2RepeatMethod);
    dont_initialize();
    sensitive << mB2RepeatEvent;

    SC_THREAD(B5Thread);
    dont_initialize();
    sensitive << mB5Event;

    SC_METHOD(B5RepeatMethod);
    dont_initialize();
    sensitive << mB5RepeatEvent;
    //------------------------------------------
}//}}}

/*********************************
// Function     : ~ATU5_TIMERB
// Description  : Destructor of ATU5_TIMERB class
// Parameter    : None
// Return value : None
**********************************/
ATU5_TIMERB::~ATU5_TIMERB(void)
{//{{{
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        delete mTCNTB0Cnt[block_num];
    }
    delete mATU5_TIB_Func;
}//}}}

/*********************************
// Function     : GetCounterValue
// Description  : Get current counter value of TCNTB0
// Parameter    :
//      cnt_name            Counter name (TCNTB0, TCNTB1)
//      block_num           Block number 0 -> 2
// Return value : Current counter value
**********************************/
unsigned int ATU5_TIMERB::GetCounterValue (const std::string cnt_name, const unsigned int block_num)
{//{{{
    unsigned int value = 0;
    if (cnt_name == "TCNTB0") {
        value = mTCNTB0Cnt[block_num]->GetCounterValue();
        if (mIsTCNTB0Updated[block_num] || !mIsTCNTB0Running[block_num]) {
            value = mATU5_TIB_Func->GetTCNTB0(block_num);
        }
    } else if (cnt_name == "TCNTB1") {
        value = mATU5_TIB_Func->GetTCNTB1(block_num);
    }
    return value;
}//}}}

/*********************************
// Function     : IsTIBEn
// Description  : Check Timer B is enable or disable
// Parameter    : None
// Return value : Current status of Timer B
**********************************/
bool ATU5_TIMERB::IsTIBEn (void)
{//{{{
    return mIsTIBEn;
 }//}}}

/*********************************
// Function     : IsTCNTB0Running
// Description  : Check TCNTB0 is enable or disable
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : Current status of TCNTB0
**********************************/
bool ATU5_TIMERB::IsTCNTB0Running(const unsigned int block_num)
{ //{{{
    return mIsTCNTB0Running[block_num];
}//}}}

/*********************************
// Function     : IsTCNTB11COR
// Description  : Check TCNTB1 and OCRB11 is match or not
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : Current status of TCNTB0
**********************************/
bool ATU5_TIMERB::IsTCNTB11COR(const unsigned int block_num)
{//{{{
    return mIsTCNTB11CorMatch[block_num];
}//}}}

/*********************************
// Function     : EnableTIB
// Description  : Enable/Disable Timer B
// Parameter    :
//      is_start            Start/Stop trigger
// Return value : None
**********************************/
void ATU5_TIMERB::EnableTIB (const bool is_start)
{//{{{
    if (is_start) {
        if (!mIsTIBEn) { //Dis -> En
            mIsTIBEnDontCareClock = true;
            SetCLKBusPeriod(mATU5_TIB_Func->GetCLKBusSel());
            mTimeCLKChangePoint[0] = mATU5->mClkBusStartTime[mATU5_TIB_Func->GetCLKBusSel()];

            // B0 & B2 counter processing
            if (mCLKBUSPeriod == 0) {
                mIsTCNTB0Running[0] = false;
                mIsTCNTB0Running[1] = false;
                mIsTCNTB0Running[2] = false;

                // timer B started without prescaler
                mIsStartWithoutPrescaler = true;
            } else {
                mIsTIBEn = true;
                re_printf("info","Timer B is enabled\n");
                mIsStartWithoutPrescaler = false;
                for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
                    mTCNTB0Cnt[block_num]->SetCounterPeriod((double)mCLKBUSPeriod);
                    mStartTCNTB0Event[block_num].notify(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5->mTimeResolutionUnit);
                }

                // Start TCNTB2 down counter at the next positive edge of bus clock selected
                mB2Event.cancel();
                mB2Event.notify(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);//Start TCNTB2
            }

            // TCNTB5 processing
            if (((mATU5_TIB_Func->GetTIORBVal() >> 6) & 0x1) == 0x0) {
                // TCNTB5 is enabled
                mCurB5 = mATU5_TIB_Func->GetTCNTB5();
                mB5Event.notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
                if ( mCurB5 == mATU5_TIB_Func->GetTCCLRB() ) {
                    mTCNTDCLRVal = true;
                }
                else{
                    mTCNTDCLRVal = false;
                }
                mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
            }
        }
    } else {
        mIsStartWithoutPrescaler = false;
        if (mIsTIBEn) {
            mIsTIBEn = false;
            mIsTIBEnDontCareClock = false;
            re_printf("info","Timer B is disabled\n");
            for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
                mIsTCNTB0Running[block_num] = false;
                mStartTCNTB0Event[block_num].notify(SC_ZERO_TIME);
            }

            // Stop events for TCNTB2, TCNTB5
            mDisableTIBEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : ResumingB5
// Description  : Resuming TCNTB5 when CTCNTB5 is written 0
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::ResumingB5 (void)
{//{{{
    mB5Event.notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
}//}}}

/*********************************
// Function     : RegisterHandlerTIB
// Description  : Handle accessing registers
// Parameter    :
//      cmd                 Register command
// Return value : None
**********************************/
void ATU5_TIMERB::RegisterHandlerTIB (const std::vector<std::string> cmd)
{//{{{
    mATU5_TIB_Func->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : NotifyUpdatedCntVal
// Description  : Update counter value when users write into counter register
// Parameter    :
//      cnt_name            Counter name (TCNTB0, TCNTB1, TCNTB2)
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5_TIMERB::NotifyUpdatedCntVal (const std::string cnt_name, const unsigned int block_num)
{//{{{
    if (cnt_name == "TCNTB0") {
        mIsTCNTB0Updated[block_num] = true;
        mTCNTB0CntEvent[block_num].notify(SC_ZERO_TIME);
    } else if (cnt_name == "TCNTB2") {
        mCurB2 = mATU5_TIB_Func->GetTCNTB2();
    } else if (cnt_name == "TCNTB5") {
        mCurB5 = mATU5_TIB_Func->GetTCNTB5();
        if ( (mCurB5 == mATU5_TIB_Func->GetTCCLRB()) && mIsTIBEnDontCareClock ) {
            mTCNTDCLRVal = true;
        }
        else{
            mTCNTDCLRVal = false;
        }
        mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
    } else if (cnt_name == "TCNTB4") {
        // do nothing
    }
}//}}}

/*********************************
// Function     : NotifyUpdatedPIMVal
// Description  : Update PIM when PIM1 is selected
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::NotifyUpdatedPIMVal (void)
{//{{{
    mCurPIM = mATU5_TIB_Func->GetPIM();
}//}}}

/*********************************
// Function     : NotifyTCNTB6COR
// Description  : Notify compare match occurred between TCNTB6 and OCRB6 when TCNTB6 is updated
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::NotifyTCNTB6COR (void)
{//{{{
    mIsTCNTB6CORMatch = true;
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        mIsTCNTB1ClrReq[1][block_num] = true;
    }
    mIsTCNTB3ClrReq = 2;
    mTCNTB3LoadEvent.notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
}//}}}

/*********************************
// Function     : NotifyCMFB6M
// Description  : Assert CMIB6M
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::NotifyCMFB6M (void)
{//{{{
    mIntrActiveEvent[emCMIB6M].notify(SC_ZERO_TIME);
    unsigned int ticrb_val = mATU5_TIB_Func->GetTICRBVal();
    if ((((ticrb_val>>2) & 0x3) == 2) ||
           ((((ticrb_val>>2) & 0x3) == 1) && (mATU5_TIB_Func->GetTimerStatus(emCMIB6)))) {
        mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : NotifyTCNTB3_8
// Description  : Notify TCNTB3 and OCRB8 is match
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::NotifyTCNTB3_8 (void)
{//{{{
    mIsTCNTB3ClrReq = 3;
    mTCNTB3LoadEvent.notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
}//}}}

/*********************************
// Function     : SetTCNTB0CORVal
// Description  : Set TCNTB0/TCNTB0S1/TCNTB0S2 compare match value
// Parameter    :
//      block_num           Block number 0 -> 2
//      cor_val             Compare match value
// Return value : None
**********************************/
void ATU5_TIMERB::SetTCNTB0CORVal (const unsigned int block_num, const unsigned int cor_val)
{//{{{
    mTCNTB0Cnt[block_num]->SetCompareMatchValue(cor_val);
    mTCNTB0CntEvent[block_num].notify(SC_ZERO_TIME);
}//}}}

/*********************************
// Function     : EnableSeq
// Description  : Enable/disable Sequencer
// Parameter    :
//      is_start            Start/Stop trigger
// Return value : None
**********************************/
void ATU5_TIMERB::EnableSeq(const bool is_start)
{//{{{
    if (is_start) {
        if (!mIsSeqEn) { //Dis -> En
            if (!mATU5->CheckClockPeriod("CLK_LSB")) {
                re_printf("warning","Sequencer can not be enabled due to zero clock\n");
            } else {
                mIsSeqEn = true;
            }
        }
    } else {
        mIsSeqEn = false;
    }
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initializes internal variables & output ports
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::Initialize (void)
{//{{{
    mCLKBUSPeriod = 0;
    mAGCK1Period = 0;
    mAGCKMPeriod = 0;
    for (unsigned int i = 0; i < emCLKBUSnum; i++) {
        mCLKBUS_nPeriod[i] = 0;
    }
    mIsTIBEnDontCareClock = false;
    mIsTIBEn = false;
    mIsSeqEn = false;
    for (unsigned int i = 0; i < emEventNum; i++) {
        mEventInVal[i] = false;
    }
    mAGCKVal = false;
    mIsAGCKTrig = false;
    mIsTCNTB6CORMatch = false;
    mIsCMIB6Req[0] = false;
    mIsCMIB6Req[1] = false;
    for (unsigned int i = 0; i < emBlocknum; i++) {
        mIsTCNTB0Running[i] = false;
        mIsTCNTB0CorMatch[i] = false;
        mIsTCNTB0OVF[i] = false;
        mIsTCNTB0Updated[i] = false;
        mIsTCNTB1CorMatch[i] = false;
        mIsTCNTB11CorMatch[i] = false;
        mIsTCNTB1ClrReq[0][i] = false;
        mIsTCNTB1ClrReq[1][i] = false;
        mIsAGCKReloadB2 = false;
        mTimeTick[i] = 0;
        mCurTCNTB0[i] = 1;
        mCurICRB1[i] = 0;
        mIsCLKBusUpdated[i] = false;
    }
    mTimeTick3 = 0;
    mCurTCNTB1 = 0;
    mCurPIM = 0;
    mIsTCNTB3ClrReq = 0;
    mIsWrapperCall = false;
    mIsPSEnable = false;
    mIsAGCK1Update = false;
    mIsTCNTB5AutoCntTrg = false;
    mTCNTDCLRVal = false;
    mIsStartWithoutPrescaler = false;
    mCurB2 = 0;
    mCurB5 = 1;
    mB2ReloadingRLDB = false; // added by chuongle 11/6/2018
    mB2LoadingICRB0 = false;  // added by chuongle 11/7/2018
    mB5NotReachTCCLRB = false;
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel all operation events
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::CancelEvents (void)
{//{{{
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        mTCNTB0CntEvent[block_num].cancel();
        mStartTCNTB0Event[block_num].cancel();
        mTCNTB1COREvent[block_num].cancel();
    }
    mTCNTB6LoadEvent.cancel();
    mTCNTB5LoadEvent.cancel();
    mTCNTB3LoadEvent.cancel();
    mAGCKReloadB2Event.cancel();
    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        mIntrInActiveEvent[interrupt_id].cancel();
        mIntrActiveEvent[interrupt_id].cancel();
    }
    for (unsigned int i = 0; i <8; i++) {
        mTCNTB1CapEvent[i].cancel();
    }
    mAGCKEvent.cancel();
    mAssertAGCKTIMEvent.cancel();
    mDeassertAGCKTIMEvent.cancel();
    mB2Event.cancel();
    mB2RepeatEvent.cancel();
    mB5Event.cancel();
    mB5RepeatEvent.cancel();
    mDisableTIBEvent.cancel();
}//}}}

/*********************************
// Function     : ResetOutputPort
// Description  : Reset output port when reset is active
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::ResetOutputPort (void)
{//{{{
    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        ControlInterruptPorts(interrupt_id, false);
    }
    AGCKM.write(0);
    AGCK1.write(0);
    AGCKTIM.write(0);
    TCNTB6CM.write(false);
    TCNTDCLR.write(false);
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Process reset function
// Parameter    :
//      is_active           Reset is active
// Return value : None
**********************************/
void ATU5_TIMERB::EnableReset (const bool is_active)
{//{{{
    mATU5_TIB_Func->EnableReset(is_active);
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        mTCNTB0Cnt[block_num]->EnableReset(is_active);
    }
    if (is_active) {
        mRstStateEvent.notify(SC_ZERO_TIME);
        CancelEvents();
        ResetOutputPort();
        Initialize();
    }
}//}}}

/*********************************
// Function     : HandleCntClkInputMethod
// Description  : Handle the change of clkbus_n clock (n:0->3)
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::HandleCntClkInputMethod (void)
{//{{{
    mCLKBUS_nPeriod[0] = clkbus_0.read();
    mCLKBUS_nPeriod[1] = clkbus_1.read();
    mCLKBUS_nPeriod[2] = clkbus_2.read();
    mCLKBUS_nPeriod[3] = clkbus_3.read();
    if (mCLKBUSPeriod != mCLKBUS_nPeriod[mATU5_TIB_Func->GetCLKBusSel()]){ // bus clock is updated
        if ((mCLKBUSPeriod!=0)&&(mIsTCNTB0Running[0] || mIsTCNTB0Running[1] || mIsTCNTB0Running[2])) {
            for (unsigned int i = 0; i < emBlocknum; i++) {
                mIsCLKBusUpdated[i] = true;
            }
            re_printf("warning","TCNTB0 clock is changed while edge-interval measuring counter is running\n");
        }
        if (mCLKBUSPeriod == 0) {
            mIsPreCLKBUSzero = true;
            if (mIsTIBEn) {
                mIsPSEnable = true;
            }
        } else {
            mIsPreCLKBUSzero = false;
        }
        SetCLKBusPeriod(mATU5_TIB_Func->GetCLKBusSel());
        mTimeCLKChangePoint[0] = mATU5->mClkBusStartTime[mATU5_TIB_Func->GetCLKBusSel()];
        std::string time_u = GetTimeUnitStr(mATU5->mTimeResolutionUnit);
        re_printf("info","Bus clock period (TCNTB0/TCNTB2 clock) is changed to %llu (%s)\n", mCLKBUSPeriod, time_u.c_str());
        if (!mIsWrapperCall) {
            mTCNTB0CntEvent[0].notify(SC_ZERO_TIME);
            mTCNTB0CntEvent[1].notify(SC_ZERO_TIME);
            mTCNTB0CntEvent[2].notify(SC_ZERO_TIME);
        } else {
            mIsWrapperCall = false;
        }

        // Notify counter TCNTB2
        mB2Event.notify(SC_ZERO_TIME);
    }

    // Start timer B again if it was enable beforehand without prescaler
    if (mIsStartWithoutPrescaler && (mCLKBUSPeriod > 0)) {
        EnableTIB(true);
    }
}//}}}

void ATU5_TIMERB::B2Thread (void)
{///{{{
    for(;;) {
        if(IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
            wait();
            continue;
        }
        try {
            while (true) {
                wait(SC_ZERO_TIME); // wait for B2 loading ICRB0 if any added by chuongle 11/7/2018
                if (mCurB2 <= mCurPIM || mB2ReloadingRLDB) { // reloading B2 += RLDB || loading B2 = ICRB0
                    
                    // Output a pulse of AGCK1
                    mAGCK1Period = mATU5->mCLK_LSBPeriod;
                    AGCK1.write(mAGCK1Period);
                    std::string time_u = GetTimeUnitStr(mATU5->mTimeResolutionUnit);
                    re_printf("info","Frequency multiplied clock (AGCK1) period is changed to %llu (%s)\n", mAGCK1Period, time_u.c_str());
                    mClearAGCK1Event.notify((double)mATU5->mCLK_LSBPeriod * 0.99, mATU5->mTimeResolutionUnit);

                    // Trigger B6, B4, B6M
                    mTCNTB6LoadEvent.cancel();
                    mTCNTB6LoadEvent.notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit); // figure 30.26, figure 30.30

                    if (mB2LoadingICRB0 == false) { // reloading TCNTB2 += RLDB
                        mCurB2 += mATU5_TIB_Func->GetRLDB();
                    }

                    mB2ReloadingRLDB = false;

                } else { // down counting B2 -= PIMR
                    mCurB2 -= mATU5_TIB_Func->GetPIM(); 

                    // If TCNTB2 <= PIMR, TCNTB2 loading signal is raised
                    if (mCurB2 <= mCurPIM) {
                        mB2ReloadingRLDB = true;
                    }

                }

                // Always clean mB2LoadingICRB0 at the edge of TCNTB2 counter
                if (mB2LoadingICRB0) {
                    mB2LoadingICRB0 = false;
                }

                // Waiting for the next clock edge
                wait((double)mCLKBUSPeriod, mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent | mDisableTIBEvent);
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
                    throw vpcl::emRstException;
                }
                mB2RepeatEvent.notify(SC_ZERO_TIME);
                wait();
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}///}}}

void ATU5_TIMERB::B2RepeatMethod (void)
{///{{{
    mB2Event.notify(SC_ZERO_TIME);
}///}}}

/*********************************
// Function     : HandleEventInputMethod
// Description  : Handle the change of ev_in_1*
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::HandleEventInputMethod (void)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mATU5_TIB_Func->IsEventInputEnable() && mIsTIBEnDontCareClock) {
        mEventInVal[emEVO1_0] = (bool)ev_in_1_0.read();
        mEventInVal[emEVO1_1] = (bool)ev_in_1_1.read();
        mEventInVal[emEVO1_2] = (bool)ev_in_1_2.read();
        if (((GetEIMBlkNum() == 0x2) && (mAGCKVal != mEventInVal[emEVO1_2])) ||
            ((GetEIMBlkNum() == 0x1) && (mAGCKVal != mEventInVal[emEVO1_1])) ||
            ((GetEIMBlkNum() == 0x0) && (mAGCKVal != mEventInVal[emEVO1_0]))) {
            if (GetEIMBlkNum() == 0x2) {
                mAGCKVal = mEventInVal[emEVO1_2];
            } else if (GetEIMBlkNum() == 0x1) {
                mAGCKVal = mEventInVal[emEVO1_1];
            } else {
                mAGCKVal = mEventInVal[emEVO1_0];
            }
            if (mAGCKVal) {
                mIsAGCKTrig = true;
                mCurTCNTB0[GetEIMBlkNum()] = GetCounterValue("TCNTB0",GetEIMBlkNum());
                mAGCKEvent.notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","[Channel %d] The external event input (AGCK) is changed to %d\n", GetEIMBlkNum(), mAGCKVal);
        }
        mCurTCNTB1 = GetCounterValue("TCNTB1",GetEIMBlkNum());
        if (mEventInVal[emEVO1B] != (bool)ev_in_1B.read()) {
            mEventInVal[emEVO1B] = (bool)ev_in_1B.read();
            if (mEventInVal[emEVO1B]) {
                mTCNTB1CapEvent[0].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1B) is changed to %d\n", mEventInVal[emEVO1B]);
        }
        if (mEventInVal[emEVO1C] != (bool)ev_in_1C.read()) {
            mEventInVal[emEVO1C] = (bool)ev_in_1C.read();
            if (mEventInVal[emEVO1C]) {
                mTCNTB1CapEvent[1].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1C) is changed to %d\n", mEventInVal[emEVO1C]);
        }
        if (mEventInVal[emEVO1D] != (bool)ev_in_1D.read()) {
            mEventInVal[emEVO1D] = (bool)ev_in_1D.read();
            if (mEventInVal[emEVO1D]) {
                mTCNTB1CapEvent[2].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1D) is changed to %d\n", mEventInVal[emEVO1D]);
        }
        if (mEventInVal[emEVO1E] != (bool)ev_in_1E.read()) {
            mEventInVal[emEVO1E] = (bool)ev_in_1E.read();
            if (mEventInVal[emEVO1E]) {
                mTCNTB1CapEvent[3].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1E) is changed to %d\n", mEventInVal[emEVO1E]);
        }
        if (mEventInVal[emEVO1F] != (bool)ev_in_1F.read()) {
            mEventInVal[emEVO1F] = (bool)ev_in_1F.read();
            if (mEventInVal[emEVO1F]) {
                mTCNTB1CapEvent[4].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1F) is changed to %d\n", mEventInVal[emEVO1F]);
        }
        if (mEventInVal[emEVO1G] != (bool)ev_in_1G.read()) {
            mEventInVal[emEVO1G] = (bool)ev_in_1G.read();
            if (mEventInVal[emEVO1G]) {
                mTCNTB1CapEvent[5].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1G) is changed to %d\n", mEventInVal[emEVO1G]);
        }
        if (mEventInVal[emEVO1H] != (bool)ev_in_1H.read()) {
            mEventInVal[emEVO1H] = (bool)ev_in_1H.read();
            if (mEventInVal[emEVO1H]) {
                mTCNTB1CapEvent[6].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1H) is changed to %d\n", mEventInVal[emEVO1H]);
        }
        if (mEventInVal[emEVO1I] != (bool)ev_in_1I.read()) {
            mEventInVal[emEVO1I] = (bool)ev_in_1I.read();
            if (mEventInVal[emEVO1I]) {
                mTCNTB1CapEvent[7].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1I) is changed to %d\n", mEventInVal[emEVO1I]);
        }
    }
}//}}}

/*********************************
// Function     : HandleAGCKMethod
// Description  : Handle the change of AGCK (external event selection) - Figure 30.15
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::HandleAGCKMethod (void)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mATU5_TIB_Func->SetICRB0(GetEIMBlkNum(),true, mCurTCNTB0[GetEIMBlkNum()]);
        mIntrActiveEvent[emICIB0].notify(SC_ZERO_TIME);
        NotifyUpdatedCntVal("TCNTB0", GetEIMBlkNum());
        mATU5_TIB_Func->SetICRB1(GetEIMBlkNum(),false, true, mCurTCNTB0[GetEIMBlkNum()]);
        TCNTB1CntUp(GetEIMBlkNum());

        // B3, B4 processing
        mTimeTick3 = GetCurTime();
        mTCNTB3LoadEvent.notify(SC_ZERO_TIME);

        // Reloading TCNTB2 @ TIA change
        mIsAGCKReloadB2 = true; // inform that TIA is triggered
        mAGCKReloadB2Event.cancel();
        mAGCKReloadB2Event.notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit); //Update TCNTB2 = ICRB0

        // B6, B4, B6M processing
        // TCNTB6: This counter is cleared to 00000H when the external input event selection (AGCK) is detected.
        // TCNTB6M is initialized to 0000 0000H at the first clock (PCLK) timing after occurrence of an external event selection input.
        mATU5_TIB_Func->SetTCNTB6(true);
        //CMIB6x interrupt
        unsigned int ticrb_val = mATU5_TIB_Func->GetTICRBVal();
        if (mIsCMIB6Req[0]) {
            mIsCMIB6Req[0] = false;
            if (((ticrb_val & 0x3) == 1)&&(mATU5_TIB_Func->GetTimerStatus(emCMIB6))) { // IREGB6: 01
                mIntrActiveEvent[emCMIB6].notify(SC_ZERO_TIME);
                if ((((ticrb_val>>2) & 0x3) == 2) ||
                    ((((ticrb_val>>2) & 0x3) == 1) && (mATU5_TIB_Func->GetTimerStatus(emCMIB6M) || mATU5_TIB_Func->mIsCMFB6M))) { // IREGB6E: 01
                    mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
                }
            }
        } else if (mIsCMIB6Req[1]) {
            mIsCMIB6Req[1] = false;
            if (((ticrb_val & 0x3) == 2)&&(mATU5_TIB_Func->GetTimerStatus(emCMIB6))) { // IREGB6: 10
                mIntrActiveEvent[emCMIB6].notify(SC_ZERO_TIME);
                if ((((ticrb_val>>2) & 0x3) == 2) ||
                    ((((ticrb_val>>2) & 0x3) == 1) && (mATU5_TIB_Func->GetTimerStatus(emCMIB6M) || mATU5_TIB_Func->mIsCMFB6M))) { // IREGB6E: 01
                    mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
                }
            }
        }

        // External event AGCK is triggered while TCNTB3 = 0
        if (mATU5_TIB_Func->GetTCNTB3() == 0) {
            if (mCurB5 == mATU5_TIB_Func->GetTCCLRB()) {
                // TCNTB5 is cleared & TCCLFB bit = 0
                mATU5_TIB_Func->SetTCCLFB(0, true);
                mCurB5 = mATU5_TIB_Func->GetTCNTB5();

                // When TCNTB5 >= TCCLRB clear timer D
                mTCNTDCLRVal = true;
                mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);

                if (((mATU5_TIB_Func->GetTIORBVal()>>6) & 0x1) == 0x0) { // CTCNTB5 = 0: TCNTB5 count operation is enabled.
                    
                    // Generate 1 pulse of AGCKM
                    mAGCKMPeriod = mATU5->mCLK_LSBPeriod;
                    AGCKM.write(mAGCKMPeriod);
                    std::string time_u = GetTimeUnitStr(mATU5->mTimeResolutionUnit);
                    re_printf("info","Frequency multiplied and corrected clock (AGCKM) period is changed to %llu (%s)\n", mAGCKMPeriod, time_u.c_str());
                    mClearAGCKMEvent.notify((double)mATU5->mCLK_LSBPeriod * 0.99, mATU5->mTimeResolutionUnit);

                    // Output AGCK Timing signal
                    if (mCurB5 == 0) {
                        mAssertAGCKTIMEvent.notify(SC_ZERO_TIME); // Figure 30.34
                    }
                }
            } else {
                // TCNTB5 is not clear & TCCLFB bit = 1
                // However, when TCNTB5 has not reached TCCLRB, TCNTB5 is incremented until it reaches TCCLRB. After that, it is cleared.
                mATU5_TIB_Func->SetTCCLFB(1, false);
                mB5NotReachTCCLRB = true; // figure 30.32, 30.35
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleTCNTB1CapMethod
// Description  : Handle ev_in_1* is assert and capture event counter TCNTB1
// Parameter    :
//      event_id            Block number 0 -> 7
// Return value : None
**********************************/
void ATU5_TIMERB::HandleTCNTB1CapMethod (const unsigned int event_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mATU5_TIB_Func->SetICRB3(event_id + 30, mCurTCNTB1);
    }
}//}}}

/*********************************
// Function     : AssertIntrMethod
// Description  : Set interrupt port to 1
// Parameter    :
//      interrupt_id        Interrupt ID
// Return value : None
**********************************/
void ATU5_TIMERB::AssertIntrMethod (const unsigned int interrupt_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        if (interrupt_id != emCMIB6) {
            // Update interrupt flags
            mATU5_TIB_Func->UpdateTimerStatus(interrupt_id);
        }
        
        // Fire the interrupt
        ControlInterruptPorts(interrupt_id, true);
        mIntrInActiveEvent[interrupt_id].notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : DeAssertIntrMethod
// Description  : Clear interrupt port to 0
// Parameter    :
//      interrupt_id        Interrupt ID
// Return value : None
**********************************/
void ATU5_TIMERB::DeAssertIntrMethod (const unsigned int interrupt_id)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB")) {
        ControlInterruptPorts(interrupt_id, false);
    }
}//}}}

void ATU5_TIMERB::HandleAssertAGCKTIMMethod (void)
{//{{{
    AGCKTIM.write(true);
    mDeassertAGCKTIMEvent.notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
    re_printf("info", "AGCKTIM is assert");
}//}}}

void ATU5_TIMERB::HandleDeassertAGCKTIMMethod (void)
{//{{{
    AGCKTIM.write(false);
    re_printf("info", "AGCKTIM is de-assert");
}//}}}

/*********************************
// Function     : HandleAGCKReloadB2Method
// Description  : Handle TCNTB2 load value
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::HandleAGCKReloadB2Method (void)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        TCNTB2LoadInit();
    }
}//}}}


/*********************************
// Function     : HandleTCNTB3LoadMethod
// Description  : Handle TCNTB3 clearing
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::HandleTCNTB3LoadMethod (void)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        if ((mIsTCNTB3ClrReq > 0) && (mIsTCNTB3ClrReq <= 3) && (mTimeTick3 != GetCurTime())) {
            mATU5_TIB_Func->SetTCNTB3(true, (mIsTCNTB3ClrReq - 1));
            mIsTCNTB3ClrReq = 0;
            
        } else if (mTimeTick3 == GetCurTime()) {
            mATU5_TIB_Func->SetTCNTB3(false, 0);

            if (mIsTCNTB3ClrReq == 1) {
                mTCNTB3LoadEvent.notify((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit);
            }
        }
    }
}//}}}

/*********************************
// Function     : B5Thread
// Description  : B5 counter
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::B5Thread (void)
{//{{{
    for(;;) {
        if(IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || !mIsTIBEnDontCareClock) {
            wait();
            continue;
        }
        try {
            while (true) {
                bool ctcntb5 = mATU5_TIB_Func->GetTIORBVal() >> 6 & 0x1;

                // When TCNBT5 = 0xF_FFFF then TCNTB5 is always >= TCCLRB whose
                // maximum value is 0xF_FFFF. TCNTB5 is stopped then. Therefore,
                // TCNTB5 overflow handling is not required

                wait(SC_ZERO_TIME); // avoid racing by mB5NotReachTCCLRB
                if ((mCurB5 < mATU5_TIB_Func->GetB5UpperLimit() || mB5NotReachTCCLRB) && !ctcntb5) { // CTCNTB5 = 0: TCNTB5 count operation is enabled.
                    // When B5 < B4 & B5 < TCCLRB, B5++ and generate a pulse of AGCK1
                    // TCNTB4 is unconditionally cleared to 0000 0000H when a pulse of the external-event selection signal (AGCK) is input
                    // while TCNTB3 = 0000 0000H. TCNTB5 is unconditionally set to 0000 1000H at CU4SEL = 0, or cleared to 0000 0000H
                    // at CU4SEL = 1 when a pulse of the external-event selection signal (AGCK) is input while TCNTB3 = 0000 0000H.
                    // However, when TCNTB5 has not reached TCCLRB, TCNTB5 is incremented until it reaches TCCLRB. After that, it is cleared.
                    if (mCurB5 == mATU5_TIB_Func->GetTCCLRB()) {
                        mATU5_TIB_Func->ClearTCNTB5();
                        mCurB5 = mATU5_TIB_Func->GetTCNTB5();
                        mB5NotReachTCCLRB = false;
                    } else {
                        mCurB5 ++;

                        // Output AGCK Timing signal
                        if (mCurB5 == mATU5_TIB_Func->GetTCNTB3() - mATU5_TIB_Func->GetPIM1()) {
                            mAssertAGCKTIMEvent.notify(SC_ZERO_TIME); // Figure 30.34
                        }

                        // Output a single pulse of AGCKM
                        mAGCKMPeriod = mATU5->mCLK_LSBPeriod;
                        AGCKM.write(mAGCKMPeriod);
                        std::string time_u = GetTimeUnitStr(mATU5->mTimeResolutionUnit);
                        re_printf("info","Frequency multiplied and corrected clock (AGCKM) period is changed to %llu (%s)\n", mAGCKMPeriod, time_u.c_str());
                        mClearAGCKMEvent.notify((double)mATU5->mCLK_LSBPeriod * 0.99, mATU5->mTimeResolutionUnit);
                    }

                } else { // CTCNTB5 = 1: TCNTB5 count operation is disabled.
                    // When B5 >= B4 or B5 >= TCCLRB or TIORB.CTCNTB5 = 1, B5 stops
                }

                // If B5 = TCCLRB, timer D is cleared regardless of the relation between its value and B4
                if ( (mCurB5 == mATU5_TIB_Func->GetTCCLRB()) && mIsTIBEnDontCareClock ) {
                    mTCNTDCLRVal = true;
                }
                else{
                    mTCNTDCLRVal = false;
                }
                mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);

                // Waiting for the next clock edge
                wait((double)mATU5->mCLK_LSBPeriod, mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mDisableTIBEvent);
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || !mIsTIBEnDontCareClock) {
                    throw vpcl::emRstException;
                }
                mB5RepeatEvent.notify(SC_ZERO_TIME);
                wait();
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || !mIsTIBEnDontCareClock) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

void ATU5_TIMERB::B5RepeatMethod (void)
{///{{{
    mB5Event.notify(SC_ZERO_TIME);
}///}}}

/*********************************
// Function     : HandleTCNTB6LoadMethod
// Description  : Handle TCNTB6 load value
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::HandleTCNTB6LoadMethod (void)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEn) {
            
        // Processing interrupts relating to B6, B6M
        bool iob6_val = (bool)(0x1&mATU5_TIB_Func->GetTIORBVal()); // 1: Compare match between TCNTB6 and OCRB6 is enabled.
        if (mIsTCNTB6CORMatch &&  iob6_val) {
            if (!mATU5_TIB_Func->GetTimerStatus(emCMIB6)) { // refer #74701: when CMFB6 is set again at CMFB6 = 1, interrupt does not occur.
                mATU5_TIB_Func->UpdateTimerStatus(emCMIB6);
                mIsCMIB6Req[0] = true;
                mIsCMIB6Req[1] = true;
                unsigned int ticrb_val = mATU5_TIB_Func->GetTICRBVal();
                if ((ticrb_val & 0x3) == 0) { // IREGB6: 00: An interrupt request is output when CMFB6 is enabled
                    mIntrActiveEvent[emCMIB6].notify(SC_ZERO_TIME);
                    if (((ticrb_val>>2) & 0x3) == 2) { // IREGB6E: 10: An interrupt request is output when CMFB6 or CMFB6M is enabled (OR condition)
                        mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
                    }
                }
            }
        }
        mIsTCNTB6CORMatch = false;
        
        // Processing B6, B6M registers' values
        mATU5_TIB_Func->SetTCNTB6(false); // trigger B6, B6M @ AGCK1

        // Processing B4 register value
        mATU5_TIB_Func->SetTCNTB4();
    }
}//}}}

/*********************************
// Function     : HandleTCNTB0CorMatchMethod
// Description  : Handle compare match event of TCNTB0/TCNTB0S1/TCNTB0S2 with OCRB0/OCRB0S1/OCRB0S2
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5_TIMERB::HandleTCNTB0CorMatchMethod (const unsigned int block_num)
{//{{{
    if ((mATU5->CheckClockPeriod("CLK_LSB") && mCLKBUSPeriod != 0) && mIsTIBEn) {
        mTimeTick[block_num] = GetCurTime();
        mIsTCNTB0CorMatch[block_num] = true;
        mTCNTB0CntEvent[block_num].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : HandleTCNTB1CorMatchMethod
// Description  : Handle compare match event of TCNTB1/TCNTB1S1/TCNTB1S2 with OCRB1/OCRB1S1/OCRB1S2
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5_TIMERB::HandleTCNTB1CorMatchMethod (const unsigned int block_num)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mATU5_TIB_Func->SetICRB2(block_num, mCurICRB1[block_num]);
        mATU5_TIB_Func->SetICRB1(block_num, true, mIsAGCKTrig, 0);
        mIntrActiveEvent[emCMIB1].notify(SC_ZERO_TIME);
        mIsTCNTB1CorMatch[block_num] = false;
    }
}//}}}


/*********************************
// Function     : HandleCLKLSBUpdatedMethod
// Description  : Handle CLK_LSB clock is updated
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::HandleCLKLSBUpdatedMethod (void)
{//{{{
    if (mATU5->CheckClockPeriod("CLK_LSB") && mIsTIBEn) {

        // Start B5 counter
        mB5Event.notify(SC_ZERO_TIME);

    }
}//}}}

/*********************************
// Function     : HandleTCNTB0OVFMethod
// Description  : Handle overflow event of TCNTB0/TCNTB0S1/TCNTB0S2
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5_TIMERB::HandleTCNTB0OVFMethod (const unsigned int block_num)
{//{{{
    if ((mATU5->CheckClockPeriod("CLK_LSB") && mCLKBUSPeriod != 0) && mIsTIBEn) {
        mTimeTick[block_num] = GetCurTime();
        mIsTCNTB0OVF[block_num] = true;
        mTCNTB0CntEvent[block_num].notify(SC_ZERO_TIME);
    }
}//}}}


/*********************************
// Function     : StartTCNTB0Method
// Description  : Handle starting TCNTB0/TCNTB0S1/TCNTB0S2 counter
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5_TIMERB::StartTCNTB0Method (const unsigned int block_num)
{//{{{
    unsigned int cur_cnt = GetCounterValue("TCNTB0", block_num);
    mIsTCNTB0Running[block_num] = mIsTIBEn;
    if (mIsTCNTB0Running[block_num] && !mATU5->CheckClockPeriod("CLK_LSB") && (mCLKBUSPeriod == 0)) {
        mIsTCNTB0Running[block_num] = false;
    }
    mIsTCNTB0CorMatch[block_num] = false;
    mIsTCNTB0OVF[block_num] = false;
    mTCNTB0Cnt[block_num]->StartStop(mIsTCNTB0Running[block_num]);
    if (mIsTCNTB0Running[block_num]) {
        cur_cnt = mATU5_TIB_Func->GetTCNTB0(block_num)+1;
        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cur_cnt);
        mTimeTick[block_num] = GetCurTime();
    }
    mATU5_TIB_Func->SetTCNTB0(block_num, cur_cnt);
}//}}}


/*********************************
// Function     : TCNTB0CntThread
// Description  : Handle TCNTB0 counter process
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::TCNTB0CntThread (void)
{//{{{
    unsigned int block_num = 0;
    for(;;) {
        if(!mIsTCNTB0Running[block_num] || IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
            mTCNTB0Cnt[block_num]->StartStop(false);
            if (mIsTCNTB0Updated[block_num]) {
                mIsTCNTB0Updated[block_num] = false;
            }
            wait();
            continue;
        }
        try {
            while (true) {
                unsigned int cnt_val = mTCNTB0Cnt[block_num]->GetCounterValue();
                if ((mTimeTick[block_num] == GetCurTime())&&(mIsTCNTB0CorMatch[block_num] || mIsTCNTB0OVF[block_num])) {
                    cnt_val = mTCNTB0Cnt[block_num]->GetCompareMatchValue();
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsTCNTB0OVF[block_num]) {
                        mATU5_TIB_Func->SetTCNTB0(block_num, 0xFFFFFFFF);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(0xFFFFFFFF);
                    } else {
                        mATU5_TIB_Func->SetTCNTB0(block_num, cnt_val);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val);
                    }
                    wait((double)mCLKBUSPeriod, mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod((double)mCLKBUSPeriod);
                        wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                            throw vpcl::emRstException;
                        }
                        mIsCLKBusUpdated[block_num] = false;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(1 + mATU5_TIB_Func->GetTCNTB0(block_num));
                    } else {
                        if (mIsTCNTB0OVF[block_num]) {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(0);
                        } else {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                        }
                    }
                    if (mIsTCNTB0CorMatch[block_num] && (block_num==GetEIMBlkNum())) {
                        mIntrActiveEvent[emCMIB0].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
                    }
                    mIsTCNTB0CorMatch[block_num] = false;
                    mIsTCNTB0OVF[block_num] = false;
                } else {
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod((double)mCLKBUSPeriod);
                    }
                    wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        // Fix bug Redmine #76985
                        // TCNTB0 is cleared to 0000 0001H on input capture by external input event 1_0.
                        if (mIsAGCKTrig) {
                            mIsAGCKTrig = false;
                            cnt_val = 0;
                        } else {
                          cnt_val = mATU5_TIB_Func->GetTCNTB0(block_num);
                        }
                    }
                    if ((!mIsCLKBusUpdated[block_num]) || (mIsPreCLKBUSzero)) {
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                    }
                    mIsCLKBusUpdated[block_num] = false;
                }
                wait();
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
            mIsCLKBusUpdated[block_num] = false;
            mIsTCNTB0CorMatch[block_num] = false;
            mIsTCNTB0OVF[block_num] = false;
            mIsTCNTB0Updated[block_num] = false;
            mTCNTB0Cnt[block_num]->StartStop(false);
        }
    }
}//}}}

/*********************************
// Function     : TCNTB0S1CntThread
// Description  : Handle TCNTB0S1 counter process
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::TCNTB0S1CntThread (void)
{//{{{
    unsigned int block_num = 1;
    for(;;) {
        if(!mIsTCNTB0Running[block_num] || IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
            mTCNTB0Cnt[block_num]->StartStop(false);
            if (mIsTCNTB0Updated[block_num]) {
                mIsTCNTB0Updated[block_num] = false;
            }
            wait();
            continue;
        }
        try {
            while (true) {
                unsigned int cnt_val = mTCNTB0Cnt[block_num]->GetCounterValue();
                if ((mTimeTick[block_num] == GetCurTime())&&(mIsTCNTB0CorMatch[block_num] || mIsTCNTB0OVF[block_num])) {
                    cnt_val = mTCNTB0Cnt[block_num]->GetCompareMatchValue();
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsTCNTB0OVF[block_num]) {
                        mATU5_TIB_Func->SetTCNTB0(block_num, 0xFFFFFFFF);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(0xFFFFFFFF);
                    } else {
                        mATU5_TIB_Func->SetTCNTB0(block_num, cnt_val);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val);
                    }
                    wait((double)mCLKBUSPeriod, mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod((double)mCLKBUSPeriod);
                        wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                            throw vpcl::emRstException;
                        }
                        mIsCLKBusUpdated[block_num] = false;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(1 + mATU5_TIB_Func->GetTCNTB0(block_num));
                    } else {
                        if (mIsTCNTB0OVF[block_num]) {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(0);
                        } else {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                        }
                    }
                    if (mIsTCNTB0CorMatch[block_num] && (block_num==GetEIMBlkNum())) {
                        mIntrActiveEvent[emCMIB0].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
                    }
                    mIsTCNTB0CorMatch[block_num] = false;
                    mIsTCNTB0OVF[block_num] = false;
                } else {
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod((double)mCLKBUSPeriod);
                    }
                    wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5->mTimeResolutionUnit);
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        // Fix bug Redmine #76985
                        // TCNTB0S1 is cleared to 0000 0001H on input capture by external input event 1_1.
                        if (mIsAGCKTrig) {
                            mIsAGCKTrig = false;
                            cnt_val = 0;
                        } else {
                          cnt_val = mATU5_TIB_Func->GetTCNTB0(block_num);
                        }
                    }
                    if ((!mIsCLKBusUpdated[block_num]) || (mIsPreCLKBUSzero)) {
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                    }
                    mIsCLKBusUpdated[block_num] = false;
                }
                wait();
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
            mIsCLKBusUpdated[block_num] = false;
            mIsTCNTB0CorMatch[block_num] = false;
            mIsTCNTB0OVF[block_num] = false;
            mIsTCNTB0Updated[block_num] = false;
            mTCNTB0Cnt[block_num]->StartStop(false);
        }
    }
}//}}}

/*********************************
// Function     : TCNTB0S2CntThread
// Description  : Handle TCNTB0S2 counter process
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::TCNTB0S2CntThread (void)
{//{{{
    unsigned int block_num = 2;
    for(;;) {
        if(!mIsTCNTB0Running[block_num] || IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
            mTCNTB0Cnt[block_num]->StartStop(false);
            if (mIsTCNTB0Updated[block_num]) {
                mIsTCNTB0Updated[block_num] = false;
            }
            wait();
            continue;
        }
        try {
            while (true) {
                unsigned int cnt_val = mTCNTB0Cnt[block_num]->GetCounterValue();
                if ((mTimeTick[block_num] == GetCurTime())&&(mIsTCNTB0CorMatch[block_num] || mIsTCNTB0OVF[block_num])) {
                    cnt_val = mTCNTB0Cnt[block_num]->GetCompareMatchValue();
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsTCNTB0OVF[block_num]) {
                        mATU5_TIB_Func->SetTCNTB0(block_num, 0xFFFFFFFF);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(0xFFFFFFFF);
                    } else {
                        mATU5_TIB_Func->SetTCNTB0(block_num, cnt_val);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val);
                    }
                    wait((double)mCLKBUSPeriod, mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod((double)mCLKBUSPeriod);
                        wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                            throw vpcl::emRstException;
                        }
                        mIsCLKBusUpdated[block_num] = false;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(1 + mATU5_TIB_Func->GetTCNTB0(block_num));
                    } else {
                        if (mIsTCNTB0OVF[block_num]) {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(0);
                        } else {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                        }
                    }
                    if (mIsTCNTB0CorMatch[block_num] && (block_num==GetEIMBlkNum())) {
                        mIntrActiveEvent[emCMIB0].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
                    }
                    mIsTCNTB0CorMatch[block_num] = false;
                    mIsTCNTB0OVF[block_num] = false;
                } else {
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod((double)mCLKBUSPeriod);
                    }
                    wait(CalculateClockEdge((double)mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5->mTimeResolutionUnit, mRstStateEvent | mATU5->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        // Fix bug Redmine #76985
                        // TCNTB0S2 is cleared to 0000 0001H on input capture by external input event 1_2.
                        if (mIsAGCKTrig) {
                            mIsAGCKTrig = false;
                            cnt_val = 0;
                        } else {
                          cnt_val = mATU5_TIB_Func->GetTCNTB0(block_num);
                        }
                    }
                    if ((!mIsCLKBusUpdated[block_num]) || (mIsPreCLKBUSzero)) {
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                    }
                    mIsCLKBusUpdated[block_num] = false;
                }
                wait();
                if (IsModelResetActive() || !mATU5->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
            mIsCLKBusUpdated[block_num] = false;
            mIsTCNTB0CorMatch[block_num] = false;
            mIsTCNTB0OVF[block_num] = false;
            mIsTCNTB0Updated[block_num] = false;
            mTCNTB0Cnt[block_num]->StartStop(false);
        }
    }
}//}}}


/*********************************
// Function     : DumpInterruptMsg
// Description  : Dump interrupt message
// Parameter    :
//      intr_name           Interrupt name
//      value               true: active level; false: de-active level
// Return value : None
**********************************/
void ATU5_TIMERB::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mATU5->mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
   }
}//}}}

/*********************************
// Function     : ControlInterruptPorts
// Description  : Control interrupt ports
// Parameter    :
//      interrupt_id        Interrupt ID
//      set_val             Setting value
// Return value : None
**********************************/
void ATU5_TIMERB::ControlInterruptPorts (const unsigned int interrupt_id, const bool set_val)
{//{{{
    std::string intr_name;
    bool pre_value;
    bool written = false;
    switch (interrupt_id) {
        case emCMIB0:
            intr_name = "CMIB0";
            pre_value = CMIB0.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB0IE")&& set_val)) {
                CMIB0.write(set_val);
                written = true;
            }
            break;
        case emICIB0:
            intr_name = "ICIB";
            pre_value = ICIB.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("ICFB0IE")&& set_val)) {
                ICIB.write(set_val);
                written = true;
            }
            break;
        case emCMIB1:
            intr_name = "CMIB1";
            pre_value = CMIB1.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB1IE")&& set_val)) {
                CMIB1.write(set_val);
                written = true;
            }
            break;
        case emCMIB6:
            intr_name = "CMIB6";
            pre_value = CMIB6.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB6IE")&& set_val)) {
                CMIB6.write(set_val);
                written = true;
            }
            break;
        case emCMIB10:
            intr_name = "CMIB10";
            pre_value = CMIB10.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB10IE")&& set_val)) {
                CMIB10.write(set_val);
                written = true;
            }
            break;
        case emCMIB11:
            intr_name = "CMIB11";
            pre_value = CMIB11.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB11IE")&& set_val)) {
                CMIB11.write(set_val);
                written = true;
            }
            break;
        case emCMIB12:
            intr_name = "CMIB12";
            pre_value = CMIB12.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB12IE")&& set_val)) {
                CMIB12.write(set_val);
                written = true;
            }
            break;
        case emCMIB6M:
            intr_name = "CMIB6M";
            pre_value = CMIB6M.read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB6MIE")&& set_val)) {
                CMIB6M.write(set_val);
                written = true;
            }
            break;
        default://emCMIB6E
            intr_name = "CMIB6E";
            pre_value = CMIB6E->read();
            if (!set_val || (mATU5_TIB_Func->IsInterruptEnable("CMFB6EIE") && set_val)) {
                CMIB6E->write(set_val);
                written = true;
            }
            break;
    }
    if ((pre_value != set_val) && written) {
        DumpInterruptMsg(intr_name,set_val);
    }
}//}}}

/*********************************
// Function     : SetCLKBusPeriod
// Description  : Set TCNTB0/TCNTB2 clock period
// Parameter    :
//      clkbus_sel          Clock bus selection
// Return value : None
**********************************/
void ATU5_TIMERB::SetCLKBusPeriod (const unsigned int clkbus_sel)
{//{{{
    mCLKBUSPeriod = mCLKBUS_nPeriod[clkbus_sel];
    if (mCLKBUSPeriod == 0){
        mClkZeroEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : TCNTB1CntUp
// Description  : Count up TCNTB1/TCNTB1S1/TCNTB1S2
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5_TIMERB::TCNTB1CntUp (const unsigned int block_num)
{//{{{
    if (mIsTCNTB1ClrReq[0][block_num] == true) {
        mIsTCNTB1ClrReq[0][block_num] = false;
        mATU5_TIB_Func->SetTCNTB1(block_num, true, false); // clear TCNTB1 as compare match TCNTB1 & OCRB10
    } else if (mIsTCNTB1ClrReq[1][block_num] == true) {
        mIsTCNTB1ClrReq[1][block_num] = false;
        mATU5_TIB_Func->SetTCNTB1(block_num, true, true); // clear TCNTB1 as compare match TCNTB6 & OCRB6 
    } else {
        mATU5_TIB_Func->SetTCNTB1(block_num, false, false); // TCNTB1 normal count up
    }
    mIsTCNTB11CorMatch[block_num] = false;
    unsigned int cur_tcntb1 = mATU5_TIB_Func->GetTCNTB1(block_num);
    unsigned int cur_ocrb1 = mATU5_TIB_Func->GetOCRB1(block_num);
    unsigned int cur_ocrb10 = mATU5_TIB_Func->GetOCRB10(block_num);
    unsigned int cur_ocrb11 = mATU5_TIB_Func->GetOCRB11(block_num);
    unsigned int cur_ocrb12 = mATU5_TIB_Func->GetOCRB12(block_num);
    if (mIsSeqEn) {
        unsigned int state;
        unsigned int cur_ocrb20_43;
        for (state = 1; state <= emSeqStatenum; state++) {
            if (mATU5_TIB_Func->GetTSEQNENB(state)) {
                cur_ocrb20_43 = mATU5_TIB_Func->GetOCRB20_43(state);
                if (cur_tcntb1 == cur_ocrb20_43){
                    mATU5_TIB_Func->SetTSEQRB(state);
                    break;
                }
            }
        }
        state = mATU5_TIB_Func->GetTSEQRB();
        mATU5_TIB_Func->SetTEPCVALRB0(state);
    } else {
        mATU5_TIB_Func->SetTSEQRB(0); //Clear to 0
    }
    if (cur_tcntb1 == cur_ocrb1) {
        mIsTCNTB1CorMatch[block_num] = true;
        mCurICRB1[block_num] = mATU5_TIB_Func->GetICRB1(block_num);
        mTCNTB1COREvent[block_num].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
    }
    if (cur_tcntb1 == cur_ocrb10) {
        mIsTCNTB1ClrReq[0][block_num] = true;
        mIntrActiveEvent[emCMIB10].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
        mIsTCNTB3ClrReq = 1;
    }
    if (cur_tcntb1 == cur_ocrb11) {
        mIsTCNTB11CorMatch[block_num] = true;
        mIntrActiveEvent[emCMIB11].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
    } else {
        mIsTCNTB11CorMatch[block_num] = false;
    }
    if (cur_tcntb1 == cur_ocrb12) {
        mIntrActiveEvent[emCMIB12].notify(CalculateClockEdge((double)mATU5->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5->mTimeCLKChangePoint[0]), mATU5->mTimeResolutionUnit);
        mATU5_TIB_Func->UpdateRBURB();
    }
}//}}}

/*********************************
// Function     : TCNTB2LoadInit
// Description  : Update load value for TCNTB2 when AGCK is triggered
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERB::TCNTB2LoadInit (void)
{//{{{
    if (mATU5_TIB_Func->GetLDEN()) { // if LDEN = 1, TCNTB2 is not reloaded @ AGCK
        mIsAGCKReloadB2 = false;
    }

    if (mIsAGCKReloadB2) {


        
        // If LDEN = 0, and TIA is triggered, B2 is loaded from ICRB0 or LDB
        mCurB2 = mATU5_TIB_Func->GetLDVal();
        mB2LoadingICRB0 = true;

        // At the same time, RLDB = ICRB0 - PIM
        mATU5_TIB_Func->SetRLDB(mATU5_TIB_Func->GetLDVal() - mATU5_TIB_Func->GetPIM());
        
        mIsAGCKReloadB2 = false;
    }
}//}}}

/*********************************
// Function     : IsModelResetActive
// Description  : Check active level of presetz
// Parameter    : None
// Return value : Current active level of presetz
**********************************/
bool ATU5_TIMERB::IsModelResetActive(void)
{//{{{
    bool ret = false;
    if (mATU5->mIsResetHardActive[1] || mATU5->mIsResetCmdReceive[1]) {
        ret = true;
    }
    return ret;
}//}}}

/*********************************
// Function     : GetEIMBlkNum
// Description  : Get Edge Interval Measuring Block number
// Parameter    : None
// Return value : Edge Interval Measuring Block number
**********************************/
unsigned int ATU5_TIMERB::GetEIMBlkNum(void)
{//{{{
    if (mATU5_TIB_Func->GetChannelSel() == 0x1) {
        return 1;
    } else if (mATU5_TIB_Func->GetChannelSel() == 0x2) {
        return 2;
    } else {
        return 0;
    }
}//}}}

/*********************************
// Function     : GetB2Val
// Description  : Get timer TCNTB2 current value
// Parameter    : None
// Return value : Current TCNTB2 value
**********************************/
unsigned int ATU5_TIMERB::GetB2Val(void)
{//{{{
    return mCurB2;
}//}}}

/*********************************
// Function     : GetB5Val
// Description  : Get timer TCNTB5 current value
// Parameter    : None
// Return value : Current TCNTB5 value
**********************************/
unsigned int ATU5_TIMERB::GetB5Val(void)
{//{{{
    return mCurB5;
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time
**********************************/
double ATU5_TIMERB::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

/*********************************
// Function     : CalculateClockEdge
// Description  : Calculate synchronous time
// Parameter    :
//      clock_period        Clock period (unit depend on mATU5->mTimeResolutionValue)
//      is_pos              true: raising edge; false: falling edge
//      add_period          true: 0 < syn_time <= 1; false: 0 <= syn_time < 1
//      time_point          Time point
//      time_change_point   Time point clock is updated
// Return value : The duration from time_point to next valid clock edge
**********************************/
double ATU5_TIMERB::CalculateClockEdge (double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
{//{{{
    double period_num = 0;

    // Returns 0 if input clock is 0
    if (clock_period == 0) {
        return 0;
    }

    period_num = (time_point - time_change_point) / clock_period;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (!add_period) {
        if (is_pos) {
            if ((double)mod_period_num < period_num) {
                mod_period_num++;
            }
        } else {
            if (((double)mod_period_num + 0.5) < period_num) {
                mod_period_num++;
            }
        }
    } else {
        mod_period_num++;
    }
    double edge_point = 0, mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    edge_point = mod_period_num_db * clock_period - (time_point - time_change_point);
    return edge_point;
}//}}}

std::string ATU5_TIMERB::GetTimeUnitStr (sc_time_unit time_u)
{//{{{
    std::string ret = "";
    if (time_u == SC_SEC) {
        ret = " s";
    } else if (time_u == SC_MS) {
        ret = " ms";
    } else if (time_u == SC_US) {
        ret = " us";
    } else if (time_u == SC_NS) {
        ret = " ns";
    } else if (time_u == SC_PS) {
        ret = " ps";
    } else if (time_u == SC_FS) {
        ret = " fs";
    }
    return ret;
}//}}}

void ATU5_TIMERB::NotifyUpdatedTCCLRBVal (const unsigned int value)
{//{{{
    if ((mCurB5 == mATU5_TIB_Func->GetTCCLRB()) && mIsTIBEnDontCareClock) {
        mTCNTDCLRVal =  true;
    }
    else {
        mTCNTDCLRVal =  false;
    }
    mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
}//}}}

void ATU5_TIMERB::WriteTCNTDCLRMethod (void)
{//{{{
    TCNTDCLR.write(mTCNTDCLRVal);
}//}}}

void ATU5_TIMERB::ClearAGCKMMethod (void)
{//{{{
    mAGCKMPeriod = 0;
    AGCKM.write(mAGCKMPeriod);
}//}}}

void ATU5_TIMERB::ClearAGCK1Method (void)
{//{{{
    mAGCK1Period = 0;
    AGCK1.write(mAGCK1Period);
}//}}}

// vim600: set foldmethod=marker :
