// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERB.cpp,v 1.4 2018/03/13 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <stdint.h>
#include <string>
#include "ATU5010.h"
#include "ATU5010_TIMERB.h"
#include "ATU5010_TIMERB_Func.h"

/*********************************
// Function     : ATU5010_TIMERB
// Description  : Constructor of ATU5010_TIMERB class
// Parameter    :
//      name            Module name
//      ATU5010_Ptr        ATU5010 pointer
// Return value : None
**********************************/
ATU5010_TIMERB::ATU5010_TIMERB(sc_module_name name, ATU5010 *ATU5010_Ptr):
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
    assert(ATU5010_Ptr != NULL);
    mATU5010 = ATU5010_Ptr;

    std::ostringstream blocknum;
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        blocknum.str("");
        blocknum << "mTCNTB0Cnt" << block_num;
        mTCNTB0Cnt[block_num] = new ATU5010_Timer(blocknum.str().c_str(), mATU5010);
        sc_assert(mTCNTB0Cnt[block_num] != NULL);
        mTCNTB0Cnt[block_num]->SetupGeneralTimer(false, true);
        mTCNTB0Cnt[block_num]->SetCounterMode(true);//Free-running mode
        mTCNTB0Cnt[block_num]->SetCompareMatchValue(0xFFFFFFFF);
    }
    mTCNTB5Cnt = new ATU5010_Timer("mTCNTB5Cnt", mATU5010);
    sc_assert(mTCNTB5Cnt != NULL);
    mTCNTB5Cnt->SetupGeneralTimer(false, true);
    mTCNTB5Cnt->SetCounterMode(true);
    mTCNTB5Cnt->SetCompareMatchValue(0xFFFFF);
    mTCNTB5Cnt->SetTimerSize(20);

    mATU5010_TIB_Func = new ATU5010_TIMERB_Func((std::string)name, this);
    sc_assert(mATU5010_TIB_Func != NULL);
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
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERB::HandleTCNTB1CapMethod, this, event_id),
                          sc_core::sc_gen_unique_name("HandleTCNTB1CapMethod"), &opt);
    }

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIntrActiveEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERB::AssertIntrMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("AssertIntrMethod"), &opt);
    }

    for (unsigned int interrupt_id = 0; interrupt_id < emInterruptNum; interrupt_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mIntrInActiveEvent[interrupt_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERB::DeAssertIntrMethod, this, interrupt_id),
                          sc_core::sc_gen_unique_name("DeAssertIntrMethod"), &opt);
    }
    
    SC_METHOD(HandleAssertAGCKTIMMethod);
    dont_initialize();
    sensitive << mAssertAGCKTIMEvent;

    SC_METHOD(HandleDeassertAGCKTIMMethod);
    dont_initialize();
    sensitive << mDeassertAGCKTIMEvent;

    SC_METHOD(HandleTCNTB2LoadMethod);
    dont_initialize();
    sensitive << mTCNTB2LoadEvent;

    SC_METHOD(HandleTCNTB3LoadMethod);
    dont_initialize();
    sensitive << mTCNTB3LoadEvent;

    SC_METHOD(HandleTCNTB5LoadMethod);
    dont_initialize();
    sensitive << mTCNTB5LoadEvent;

    SC_METHOD(HandleTCNTB6LoadMethod);
    dont_initialize();
    sensitive << mTCNTB6LoadEvent;

    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTCNTB0Cnt[index]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERB::HandleTCNTB0CorMatchMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTCNTB0CorMatchMethod"), &opt);
    }

    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTCNTB1COREvent[index]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERB::HandleTCNTB1CorMatchMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTCNTB1CorMatchMethod"), &opt);
    }

    SC_METHOD(HandleTCNTB5CorMatchMethod);
    dont_initialize();
    sensitive << mTCNTB5Cnt->mCompareMatchEvent;

    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTCNTB0Cnt[index]->mOverflowEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERB::HandleTCNTB0OVFMethod, this, index),
                          sc_core::sc_gen_unique_name("HandleTCNTB0OVFMethod"), &opt);
    }

    SC_METHOD(HandleTCNTB5OVFMethod);
    dont_initialize();
    sensitive << mTCNTB5Cnt->mOverflowEvent;

    SC_METHOD(HandleCLKLSBUpdatedMethod);
    dont_initialize();
    sensitive << mATU5010->mCLKLSBUpdatedEvent;

    for (unsigned int index = 0; index < emBlocknum; index++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartTCNTB0Event[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5010_TIMERB::StartTCNTB0Method, this, index),
                          sc_core::sc_gen_unique_name("StartTCNTB0Method"), &opt);
    }

    SC_METHOD(StartTCNTB5Method);
    dont_initialize();
    sensitive << mStartTCNTB5Event;

    SC_THREAD(TCNTB0CntThread);
    dont_initialize();
    sensitive << mTCNTB0CntEvent[0];

    SC_THREAD(TCNTB0S1CntThread);
    dont_initialize();
    sensitive << mTCNTB0CntEvent[1];

    SC_THREAD(TCNTB0S2CntThread);
    dont_initialize();
    sensitive << mTCNTB0CntEvent[2];

    SC_THREAD(TCNTB5CntThread);
    dont_initialize();
    sensitive << mTCNTB5CntEvent;
    
    SC_METHOD(TCNTB2CntUpMethod);
    dont_initialize();
    sensitive << mTCNTB2CntUpEvent;
    
    SC_METHOD(WriteTCNTDCLRMethod);
    dont_initialize();
    sensitive << mWriteTCNTDCLREvent;
    
    SC_METHOD(ClearAGCKMMethod);
    dont_initialize();
    sensitive << mClearAGCKMEvent;
    //------------------------------------------
}//}}}

/*********************************
// Function     : ~ATU5010_TIMERB
// Description  : Destructor of ATU5010_TIMERB class
// Parameter    : None
// Return value : None
**********************************/
ATU5010_TIMERB::~ATU5010_TIMERB(void)
{//{{{
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        delete mTCNTB0Cnt[block_num];
    }
    delete mTCNTB5Cnt;
    delete mATU5010_TIB_Func;
}//}}}

/*********************************
// Function     : GetCounterValue
// Description  : Get current counter value of TCNTB0
// Parameter    :
//      cnt_name            Counter name (TCNTB0, TCNTB1, TCNTB2)
//      block_num           Block number 0 -> 2
// Return value : Current counter value
**********************************/
unsigned int ATU5010_TIMERB::GetCounterValue (const std::string cnt_name, const unsigned int block_num)
{//{{{
    unsigned int value = 0;
    if (cnt_name == "TCNTB0") {
        value = mTCNTB0Cnt[block_num]->GetCounterValue();
        if (mIsTCNTB0Updated[block_num] || !mIsTCNTB0Running[block_num]) {
            value = mATU5010_TIB_Func->GetTCNTB0(block_num);
        }
    } else if (cnt_name == "TCNTB2") {
        if(mIsTCNTB2Updated || !mIsTIBEn || (mTimeTick2 == 0) || (mCLKBUSPeriod == 0)) {
            value = mATU5010_TIB_Func->GetTCNTB2();
        } else {
            double TCNTB2TickTime = mTimeTick2 + CalculateClockEdge(mCLKBUSPeriod, true, false, mTimeTick2, mTimeCLKChangePoint[0]);
            double cnt_duration = GetCurTime() - TCNTB2TickTime;
            double div_cycle = cnt_duration/(double)mCLKBUSPeriod;
            unsigned int cycle = (unsigned int) div_cycle;
            if ((cycle < div_cycle) || ((cycle == div_cycle) && (!mIsTCNTB2LoadState[1]))) {
                cycle ++;
            }
            if (cnt_duration < 0) {
                value = mCurTCNTB2;
            } else {
                if (TCNTB2TickTime == mTimeTick2) { //Load time = count time
                    value = mCurTCNTB2 - (mCurPIM*(cycle-1));
                } else {
                    value = mCurTCNTB2 - (mCurPIM*cycle);
                }
            }
        }
    } else if (cnt_name == "TCNTB5") {
        value = mTCNTB5Cnt->GetCounterValue();
        if (mIsTCNTB5Updated || !mIsTCNTB5AutoCnt || mIsTCNTB5OVF) {
            value = mATU5010_TIB_Func->GetTCNTB5();
        }
    } else if (cnt_name == "TCNTB1") {
        value = mATU5010_TIB_Func->GetTCNTB1(block_num);
    }
    return value;
}//}}}

/*********************************
// Function     : IsTIBEn
// Description  : Check Timer B is enable or disable
// Parameter    : None
// Return value : Current status of Timer B
**********************************/
bool ATU5010_TIMERB::IsTIBEn (void)
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
bool ATU5010_TIMERB::IsTCNTB0Running(const unsigned int block_num)
{//{{{
    return mIsTCNTB0Running[block_num];
}//}}}

/*********************************
// Function     : IsTCNTB11COR
// Description  : Check TCNTB1 and OCRB11 is match or not
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : Current status of TCNTB0
**********************************/
bool ATU5010_TIMERB::IsTCNTB11COR(const unsigned int block_num)
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
void ATU5010_TIMERB::EnableTIB (const bool is_start)
{//{{{
    if (is_start) {
        if (!mIsTIBEn) { //Dis -> En
            mIsTIBEnDontCareClock = true;
            SetCLKBusPeriod(mATU5010_TIB_Func->GetCLKBusSel());
            mTimeCLKChangePoint[0] = mATU5010->mClkBusStartTime[mATU5010_TIB_Func->GetCLKBusSel()];
            if (mCLKBUSPeriod == 0) {
                mIsTCNTB0Running[0] = false;
                mIsTCNTB0Running[1] = false;
                mIsTCNTB0Running[2] = false;
            } else {
                mIsTIBEn = true;
                re_printf("info","Timer B is enabled\n");
                mTimeTick2 = GetCurTime();
                for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
                    mTCNTB0Cnt[block_num]->SetCounterPeriod(mCLKBUSPeriod);
                    mStartTCNTB0Event[block_num].notify(CalculateClockEdge(mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit);
                }
                mTCNTB2LoadEvent.cancel();
                mTCNTB2LoadEvent.notify(SC_ZERO_TIME);//Start TCNTB2
            }
            mTCNTDCLRVal = mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
            mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
            if (((mATU5010_TIB_Func->GetTIORBVal() >> 6) & 0x1) == 0x0) {
                // TCNTB5 is enabled
                CheckTCNTB5Update();
            }
        }
    } else {
        if (mIsTIBEn) {
            mIsTIBEn = false;
            mIsTIBEnDontCareClock = false;
            re_printf("info","Timer B is disabled\n");
            for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
                mIsTCNTB0Running[block_num] = false;
                mStartTCNTB0Event[block_num].notify(SC_ZERO_TIME);
            }
            mTimeTick2 = 0;
            mCurTCNTB2 = GetCounterValue("TCNTB2", 0);
            mATU5010_TIB_Func->SetTCNTB2(mCurTCNTB2);
            AGCK1.write(0);
        }
        if (mIsTCNTB5AutoCnt) {
            EnableTCNTB5AutoCounting(false);
        }
    }
}//}}}

/*********************************
// Function     : RegisterHandlerTIB
// Description  : Handle accessing registers
// Parameter    :
//      cmd                 Register command
// Return value : None
**********************************/
void ATU5010_TIMERB::RegisterHandlerTIB (const std::vector<std::string> cmd)
{//{{{
    mATU5010_TIB_Func->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : NotifyUpdatedCntVal
// Description  : Update counter value when users write into counter register
// Parameter    :
//      cnt_name            Counter name (TCNTB0, TCNTB1, TCNTB2)
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5010_TIMERB::NotifyUpdatedCntVal (const std::string cnt_name, const unsigned int block_num)
{//{{{
    if (cnt_name == "TCNTB0") {
        mIsTCNTB0Updated[block_num] = true;
        mTCNTB0CntEvent[block_num].notify(SC_ZERO_TIME);
    } else if (cnt_name == "TCNTB2") {
        mIsTCNTB2Updated = true;
        mTCNTB2LoadEvent.cancel();
        mTCNTB2LoadEvent.notify(SC_ZERO_TIME);//Update TCNTB2 value
    } else if (cnt_name == "TCNTB5") {
        if (mIsTCNTB5AutoCnt) {
            mIsTCNTB5Updated = true;
            mTCNTB5CntEvent.notify(SC_ZERO_TIME);
        } else {
            CheckTCNTB5Update();
        }
        mTCNTDCLRVal = (mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB()) && mIsTIBEnDontCareClock ? true : false;
        mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
    } else if (cnt_name == "TCNTB4") {
        CheckTCNTB5Update();
    }
}//}}}

/*********************************
// Function     : NotifyUpdatedPIMVal
// Description  : Update PIM when PIM1 is selected
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::NotifyUpdatedPIMVal (void)
{//{{{
    mIsPIMUpdated = true;
    mTCNTB2LoadEvent.cancel();
    mTCNTB2LoadEvent.notify(SC_ZERO_TIME);//Update PIM value
}//}}}

/*********************************
// Function     : NotifyTCNTB6COR
// Description  : Notify compare match occurred between TCNTB6 and OCRB6 when TCNTB6 is updated
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::NotifyTCNTB6COR (void)
{//{{{
    mIsTCNTB6CORMatch = true;
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        mIsTCNTB1ClrReq[1][block_num] = true;
    }
    mIsTCNTB3ClrReq = 2;
    mTCNTB3LoadEvent.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
}//}}}

/*********************************
// Function     : NotifyCMFB6M
// Description  : Assert CMIB6M
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::NotifyCMFB6M (void)
{//{{{
    mIntrActiveEvent[emCMIB6M].notify(SC_ZERO_TIME);
    unsigned int ticrb_val = mATU5010_TIB_Func->GetTICRBVal();
    if ((((ticrb_val>>2) & 0x3) == 2) ||
           ((((ticrb_val>>2) & 0x3) == 1) && (mATU5010_TIB_Func->GetTimerStatus(emCMIB6)))) {
        mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : NotifyTCNTB3_8
// Description  : Notify TCNTB3 and OCRB8 is match
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::NotifyTCNTB3_8 (void)
{//{{{
    mIsTCNTB3ClrReq = 3;
    mTCNTB3LoadEvent.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
}//}}}

/*********************************
// Function     : SetTCNTB0CORVal
// Description  : Set TCNTB0/TCNTB0S1/TCNTB0S2 compare match value
// Parameter    :
//      block_num           Block number 0 -> 2
//      cor_val             Compare match value
// Return value : None
**********************************/
void ATU5010_TIMERB::SetTCNTB0CORVal (const unsigned int block_num, const unsigned int cor_val)
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
void ATU5010_TIMERB::EnableSeq(const bool is_start)
{//{{{
    if (is_start) {
        if (!mIsSeqEn) { //Dis -> En
            if (!mATU5010->CheckClockPeriod("CLK_LSB")) {
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
// Function     : EnableTCNTB5AutoCounting
// Description  : Enable/disable TCNTB5 auto counting
// Parameter    :
//      is_start            Start/Stop trigger
// Return value : None
**********************************/
void ATU5010_TIMERB::EnableTCNTB5AutoCounting(const bool is_start)
{//{{{
    if (is_start) {
        if (!mIsTCNTB5AutoCnt) { //Dis -> En
            if (!mATU5010->CheckClockPeriod("CLK_LSB")) {
                re_printf("warning","TCNTB5 auto count-up can not processed due to zero clock\n");
                mATU5010_TIB_Func->SetTCCLFB(0, false);
            } else {
                mIsTCNTB5AutoCnt = true;
                mIsTCNTB5AutoCntTrg = true;
                mTCNTB5Cnt->SetCompareMatchValue(mATU5010_TIB_Func->GetTCCLRB());
                unsigned int cur_cnt = mATU5010_TIB_Func->GetTCNTB5();
                mTCNTB5Cnt->SetCounterInitialValue(cur_cnt);
                mStartTCNTB5Event.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
        }
    } else {
        if (mIsTCNTB5AutoCnt) {
            mIsTCNTB5AutoCnt = false;
            mTCNTB5Cnt->StartStop(false);
            mATU5010_TIB_Func->SetTCNTB5(mTCNTB5Cnt->GetCounterValue());
            mStartTCNTB5Event.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            mTCNTDCLRVal = mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
            mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
            mATU5010_TIB_Func->SetTCCLFB(0, false);
        }
    }
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initializes internal variables & output ports
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::Initialize (void)
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
    mIsTCNTB2Updated = false;
    mIsPIMUpdated = false;
    mIsTCNTB5AutoCnt = false;
    mIsTCNTB5CorMatch = false;
    mIsTCNTB5OVF = false;
    mIsTCNTB5Updated = false;
    mIsCLKLSBUpdated = false;
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
        mIsTCNTB2LoadState[i] = false;
        mTimeTick[i] = 0;
        mCurTCNTB0[i] = 1;
        mCurICRB1[i] = 0;
        mIsCLKBusUpdated[i] = false;
    }
    mTimeTick2 = 0;
    mTimeTick3 = 0;
    mTimeTick5 = 0;
    mCurTCNTB1 = 0;
    mCurTCNTB2 = 0;
    mCurPIM = 0;
    mIsTCNTB3ClrReq = 0;
    mIsWrapperCall = false;
    mIsPSEnable = false;
    mIsAGCK1Update = false;
    mIsTCNTB5AutoCntTrg = false;
    mTCNTDCLRVal = false;
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel all operation events
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::CancelEvents (void)
{//{{{
    mTCNTB5CntEvent.cancel();
    for (unsigned int block_num = 0; block_num < emBlocknum; block_num++) {
        mTCNTB0CntEvent[block_num].cancel();
        mStartTCNTB0Event[block_num].cancel();
        mTCNTB1COREvent[block_num].cancel();
    }
    mStartTCNTB5Event.cancel();
    mTCNTB6LoadEvent.cancel();
    mTCNTB5LoadEvent.cancel();
    mTCNTB3LoadEvent.cancel();
    mTCNTB2LoadEvent.cancel();
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
}//}}}

/*********************************
// Function     : ResetOutputPort
// Description  : Reset output port when reset is active
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::ResetOutputPort (void)
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
void ATU5010_TIMERB::EnableReset (const bool is_active)
{//{{{
    mATU5010_TIB_Func->EnableReset(is_active);
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
void ATU5010_TIMERB::HandleCntClkInputMethod (void)
{//{{{
    mCLKBUS_nPeriod[0] = clkbus_0.read();
    mCLKBUS_nPeriod[1] = clkbus_1.read();
    mCLKBUS_nPeriod[2] = clkbus_2.read();
    mCLKBUS_nPeriod[3] = clkbus_3.read();
    if (mCLKBUSPeriod != mCLKBUS_nPeriod[mATU5010_TIB_Func->GetCLKBusSel()]){
        if ((mCLKBUSPeriod!=0)&&(mIsTCNTB0Running[0] || mIsTCNTB0Running[1] || mIsTCNTB0Running[2])) {
            for (unsigned int i = 0; i < emBlocknum; i++) {
                mIsCLKBusUpdated[i] = true;
            }
            re_printf("warning","TCNTB0 clock is changed while edge-interval measuring counter is running\n");
            mCurTCNTB2 = GetCounterValue("TCNTB2", 0);
            mTimeTick2 = GetCurTime();
            AGCK1.write(0);
        }
        if (mCLKBUSPeriod == 0) {
            mIsPreCLKBUSzero = true;
            if (mIsTIBEn) {
                mIsPSEnable = true;
            }
        } else {
            mIsPreCLKBUSzero = false;
        }
        SetCLKBusPeriod(mATU5010_TIB_Func->GetCLKBusSel());
        mTimeCLKChangePoint[0] = mATU5010->mClkBusStartTime[mATU5010_TIB_Func->GetCLKBusSel()];
        std::string time_u = GetTimeUnitStr(mATU5010->mTimeResolutionUnit);
        re_printf("info","Bus clock period (TCNTB0/TCNTB2 clock) is changed to %llu (%s)\n", mCLKBUSPeriod, time_u.c_str());
        if (!mIsWrapperCall) {
            mTCNTB0CntEvent[0].notify(SC_ZERO_TIME);
            mTCNTB0CntEvent[1].notify(SC_ZERO_TIME);
            mTCNTB0CntEvent[2].notify(SC_ZERO_TIME);
        } else {
            mIsWrapperCall = false;
        }
        mTCNTB2LoadEvent.cancel();
        mTCNTB2LoadEvent.notify(SC_ZERO_TIME); //Update bus clock
    }
}//}}}

/*********************************
// Function     : HandleEventInputMethod
// Description  : Handle the change of ev_in_1*
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleEventInputMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mATU5010_TIB_Func->IsEventInputEnable() && mIsTIBEnDontCareClock) {
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
                mAGCKEvent.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","[Channel %d] The external event input (AGCK) is changed to %d\n", GetEIMBlkNum(), mAGCKVal);
        }
        mCurTCNTB1 = GetCounterValue("TCNTB1",GetEIMBlkNum());
        if (mEventInVal[emEVO1B] != (bool)ev_in_1B.read()) {
            mEventInVal[emEVO1B] = (bool)ev_in_1B.read();
            if (mEventInVal[emEVO1B]) {
                mTCNTB1CapEvent[0].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1B) is changed to %d\n", mEventInVal[emEVO1B]);
        }
        if (mEventInVal[emEVO1C] != (bool)ev_in_1C.read()) {
            mEventInVal[emEVO1C] = (bool)ev_in_1C.read();
            if (mEventInVal[emEVO1C]) {
                mTCNTB1CapEvent[1].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1C) is changed to %d\n", mEventInVal[emEVO1C]);
        }
        if (mEventInVal[emEVO1D] != (bool)ev_in_1D.read()) {
            mEventInVal[emEVO1D] = (bool)ev_in_1D.read();
            if (mEventInVal[emEVO1D]) {
                mTCNTB1CapEvent[2].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1D) is changed to %d\n", mEventInVal[emEVO1D]);
        }
        if (mEventInVal[emEVO1E] != (bool)ev_in_1E.read()) {
            mEventInVal[emEVO1E] = (bool)ev_in_1E.read();
            if (mEventInVal[emEVO1E]) {
                mTCNTB1CapEvent[3].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1E) is changed to %d\n", mEventInVal[emEVO1E]);
        }
        if (mEventInVal[emEVO1F] != (bool)ev_in_1F.read()) {
            mEventInVal[emEVO1F] = (bool)ev_in_1F.read();
            if (mEventInVal[emEVO1F]) {
                mTCNTB1CapEvent[4].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1F) is changed to %d\n", mEventInVal[emEVO1F]);
        }
        if (mEventInVal[emEVO1G] != (bool)ev_in_1G.read()) {
            mEventInVal[emEVO1G] = (bool)ev_in_1G.read();
            if (mEventInVal[emEVO1G]) {
                mTCNTB1CapEvent[5].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1G) is changed to %d\n", mEventInVal[emEVO1G]);
        }
        if (mEventInVal[emEVO1H] != (bool)ev_in_1H.read()) {
            mEventInVal[emEVO1H] = (bool)ev_in_1H.read();
            if (mEventInVal[emEVO1H]) {
                mTCNTB1CapEvent[6].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1H) is changed to %d\n", mEventInVal[emEVO1H]);
        }
        if (mEventInVal[emEVO1I] != (bool)ev_in_1I.read()) {
            mEventInVal[emEVO1I] = (bool)ev_in_1I.read();
            if (mEventInVal[emEVO1I]) {
                mTCNTB1CapEvent[7].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            }
            re_printf("info","The external event input (ev_in_1I) is changed to %d\n", mEventInVal[emEVO1I]);
        }
    }
}//}}}

/*********************************
// Function     : HandleAGCKMethod
// Description  : Handle the change of AGCK
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleAGCKMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mIsAGCKTrig = false;
        mATU5010_TIB_Func->SetICRB0(GetEIMBlkNum(),true, mCurTCNTB0[GetEIMBlkNum()]);
        mIntrActiveEvent[emICIB0].notify(SC_ZERO_TIME);
        mATU5010_TIB_Func->SetTCNTB0(GetEIMBlkNum(), 0x1);
        NotifyUpdatedCntVal("TCNTB0", GetEIMBlkNum());
        mATU5010_TIB_Func->SetICRB1(GetEIMBlkNum(),false, true, mCurTCNTB0[GetEIMBlkNum()]);
        TCNTB1CntUp(GetEIMBlkNum());
        mTimeTick3 = GetCurTime();
        mTCNTB3LoadEvent.notify(SC_ZERO_TIME);
        mIsTCNTB2LoadState[2] = true;
        if (!mIsTCNTB2LoadState[1]) {
            mTCNTB2LoadEvent.notify(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit); //Update TCNTB2 = ICRB0
        }
        mATU5010_TIB_Func->SetTCNTB6(true);
        //CMIB6x interrupt
        unsigned int ticrb_val = mATU5010_TIB_Func->GetTICRBVal();
        if (mIsCMIB6Req[0]) {
            mIsCMIB6Req[0] = false;
            if (((ticrb_val & 0x3) == 1)&&(mATU5010_TIB_Func->GetTimerStatus(emCMIB6))) {
                mIntrActiveEvent[emCMIB6].notify(SC_ZERO_TIME);
                if ((((ticrb_val>>2) & 0x3) == 2) ||
                    ((((ticrb_val>>2) & 0x3) == 1) && (mATU5010_TIB_Func->GetTimerStatus(emCMIB6M) || mATU5010_TIB_Func->mIsCMFB6M))) {
                    mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
                }
            }
        } else if (mIsCMIB6Req[1]) {
            mIsCMIB6Req[1] = false;
            if (((ticrb_val & 0x3) == 2)&&(mATU5010_TIB_Func->GetTimerStatus(emCMIB6))) {
                mIntrActiveEvent[emCMIB6].notify(SC_ZERO_TIME);
                if ((((ticrb_val>>2) & 0x3) == 2) ||
                    ((((ticrb_val>>2) & 0x3) == 1) && (mATU5010_TIB_Func->GetTimerStatus(emCMIB6M) || mATU5010_TIB_Func->mIsCMFB6M))) {
                    mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
                }
            }
        }
        if (mATU5010_TIB_Func->GetTCNTB3() == 0) {
            if (mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB()) {
                mATU5010_TIB_Func->SetTCCLFB(0, true);
                mTCNTDCLRVal = false;
                mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
                if (((mATU5010_TIB_Func->GetTIORBVal()>>6) & 0x1) == 0x0) {
                    mAGCKMPeriod = (sc_dt::uint64)CalculateClockEdge(mAGCK1Period, true, false, GetCurTime(), mTimeCLKChangePoint[1]) + mAGCK1Period + 2*mATU5010->mCLK_LSBPeriod;
                    AGCKM.write(mAGCKMPeriod);
                    mAssertAGCKTIMEvent.notify(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit);
                }
            } else {
                mATU5010_TIB_Func->SetTCCLFB(1, false);
                if (((mATU5010_TIB_Func->GetTIORBVal()>>6) & 0x1) == 0x0) {
                    mIsTCNTB5AutoCnt = true;
                    mIsTCNTB5AutoCntTrg = true;
                    mTCNTB5Cnt->SetCompareMatchValue(mATU5010_TIB_Func->GetTCCLRB());
                    mStartTCNTB5Event.notify(SC_ZERO_TIME);
                }
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
void ATU5010_TIMERB::HandleTCNTB1CapMethod (const unsigned int event_id)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mATU5010_TIB_Func->SetICRB3(event_id + 30, mCurTCNTB1);
    }
}//}}}

/*********************************
// Function     : AssertIntrMethod
// Description  : Set interrupt port to 1
// Parameter    :
//      interrupt_id        Interrupt ID
// Return value : None
**********************************/
void ATU5010_TIMERB::AssertIntrMethod (const unsigned int interrupt_id)
{//{{{
    bool isINTActive;
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        if (interrupt_id != emCMIB6) {
            isINTActive = !(mATU5010_TIB_Func->GetTimerStatus(interrupt_id));
            mATU5010_TIB_Func->UpdateTimerStatus(interrupt_id);
        } else {
            isINTActive = true;
        }
        if (isINTActive) {
            ControlInterruptPorts(interrupt_id, true);
            mIntrInActiveEvent[interrupt_id].notify(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit);
        }
    }
}//}}}

/*********************************
// Function     : DeAssertIntrMethod
// Description  : Clear interrupt port to 0
// Parameter    :
//      interrupt_id        Interrupt ID
// Return value : None
**********************************/
void ATU5010_TIMERB::DeAssertIntrMethod (const unsigned int interrupt_id)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB")) {
        ControlInterruptPorts(interrupt_id, false);
    }
}//}}}

void ATU5010_TIMERB::HandleAssertAGCKTIMMethod (void)
{//{{{
    if ((mATU5010_TIB_Func->GetTCNTB5() == (mATU5010_TIB_Func->GetTCNTB3() - mATU5010_TIB_Func->GetPIM1())) && !mIsTCNTB5AutoCnt) {
        AGCKTIM.write(true);
        mDeassertAGCKTIMEvent.notify(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit);
        re_printf("info", "AGCKTIM is assert");
    }
}//}}}

void ATU5010_TIMERB::HandleDeassertAGCKTIMMethod (void)
{//{{{
    AGCKTIM.write(false);
    re_printf("info", "AGCKTIM is de-assert");
}//}}}

/*********************************
// Function     : HandleTCNTB2LoadMethod
// Description  : Handle TCNTB2 load value
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB2LoadMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        TCNTB2LoadInit();
    } else {
        if (mIsPIMUpdated) {
            mCurPIM = mATU5010_TIB_Func->GetPIM();
            mIsPIMUpdated = false;
        }
        if (mIsTCNTB2Updated) {
            mCurTCNTB2 = mATU5010_TIB_Func->GetTCNTB2();
            mTimeTick2 = GetCurTime();
            mIsTCNTB2Updated = false;
        }
        if (mIsTCNTB2LoadState[1]) {
            mIsTCNTB2LoadState[1] = false;
        }
    }
}//}}}

/*********************************
// Function     : HandleTCNTB3LoadMethod
// Description  : Handle TCNTB3 clearing
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB3LoadMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        if ((mIsTCNTB3ClrReq > 0) && (mIsTCNTB3ClrReq <= 3) && (mTimeTick3 != GetCurTime())) {
            mATU5010_TIB_Func->SetTCNTB3(true, (mIsTCNTB3ClrReq - 1));
            mIsTCNTB3ClrReq = 0;
        } else if (mTimeTick3 == GetCurTime()) {
            mATU5010_TIB_Func->SetTCNTB3(false, 0);
            if (mATU5010_TIB_Func->GetTCNTB3() != 0) {
                CheckTCNTB5Update();
            }
            if (mIsTCNTB3ClrReq == 1) {
                mTCNTB3LoadEvent.notify(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit);
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleTCNTB5LoadMethod
// Description  : Handle TCNTB5 load value
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB5LoadMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        unsigned int tcntb4_val = mATU5010_TIB_Func->GetTCNTB4();
        unsigned int tcntb5_val = mATU5010_TIB_Func->GetTCNTB5();
        if ((!mIsTCNTB5AutoCnt) && (((mATU5010_TIB_Func->GetTIORBVal()>>6) & 0x1) == 0x0)) {
            if ((tcntb5_val < tcntb4_val) && (tcntb5_val != mATU5010_TIB_Func->GetTCCLRB()) && (CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]) == 0)) {
                mATU5010_TIB_Func->SetTCNTB5(tcntb5_val+1);
                mTCNTDCLRVal = mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
                mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
                if ((tcntb5_val+1) == tcntb4_val) {
                    mAGCKMPeriod = AGCK1.read();
                    AGCKM.write(mAGCKMPeriod);
                    mAssertAGCKTIMEvent.notify(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit);
                } else {
                    CheckTCNTB5Update();
                    if (mIsTCNTB5AutoCnt) {
                        mAGCKMPeriod = mATU5010->mCLK_LSBPeriod;
                        AGCKM.write(mAGCKMPeriod);
                    }
                }
            } else if ((tcntb5_val > tcntb4_val) || (tcntb5_val == mATU5010_TIB_Func->GetTCCLRB())) {
                mClearAGCKMEvent.notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleTCNTB6LoadMethod
// Description  : Handle TCNTB6 load value
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB6LoadMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEn) {
        bool iob6_val = (bool)(0x1&mATU5010_TIB_Func->GetTIORBVal());
        if (mIsTCNTB6CORMatch &&  iob6_val) {
            mATU5010_TIB_Func->UpdateTimerStatus(emCMIB6);
            mIsCMIB6Req[0] = true;
            mIsCMIB6Req[1] = true;
            unsigned int ticrb_val = mATU5010_TIB_Func->GetTICRBVal();
            if ((ticrb_val & 0x3) == 0) {
                mIntrActiveEvent[emCMIB6].notify(SC_ZERO_TIME);
                if (((ticrb_val>>2) & 0x3) == 2) {
                    mIntrActiveEvent[emCMIB6E].notify(SC_ZERO_TIME);
                }
            }
        }
        mIsTCNTB6CORMatch = false;
        mATU5010_TIB_Func->SetTCNTB6(false);
        if (mATU5010_TIB_Func->SetTCNTB4()) {
            mTCNTB5LoadEvent.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
        } else {
            mClearAGCKMEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : HandleTCNTB0CorMatchMethod
// Description  : Handle compare match event of TCNTB0/TCNTB0S1/TCNTB0S2 with OCRB0/OCRB0S1/OCRB0S2
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB0CorMatchMethod (const unsigned int block_num)
{//{{{
    if ((mATU5010->CheckClockPeriod("CLK_LSB") && mCLKBUSPeriod != 0) && mIsTIBEn) {
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
void ATU5010_TIMERB::HandleTCNTB1CorMatchMethod (const unsigned int block_num)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mATU5010_TIB_Func->SetICRB2(block_num, mCurICRB1[block_num]);
        mATU5010_TIB_Func->SetICRB1(block_num, true, mIsAGCKTrig, 0);
        mIntrActiveEvent[emCMIB1].notify(SC_ZERO_TIME);
        mIsTCNTB1CorMatch[block_num] = false;
    }
}//}}}

/*********************************
// Function     : HandleTCNTB5CorMatchMethod
// Description  : Handle compare match event of TCNTB5 with TCCLRB
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB5CorMatchMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mTimeTick5 = GetCurTime();
        mIsTCNTB5CorMatch = true;
        mTCNTB5CntEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : HandleCLKLSBUpdatedMethod
// Description  : Handle CLK_LSB clock is updated
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleCLKLSBUpdatedMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEn) {
        mIsCLKLSBUpdated = true;
        mTCNTB5CntEvent.notify(SC_ZERO_TIME);
        if (mIsTCNTB5AutoCnt) {
            mAGCKMPeriod = mATU5010->mCLK_LSBPeriod;
            AGCKM.write(mAGCKMPeriod);
            std::string time_u = GetTimeUnitStr(mATU5010->mTimeResolutionUnit);
            re_printf("info","Frequency multiplied and corrected clock (AGCKM) period is changed to %llu (%s)\n", mAGCKMPeriod, time_u.c_str());
        }
    }
}//}}}

/*********************************
// Function     : HandleTCNTB0OVFMethod
// Description  : Handle overflow event of TCNTB0/TCNTB0S1/TCNTB0S2
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB0OVFMethod (const unsigned int block_num)
{//{{{
    if ((mATU5010->CheckClockPeriod("CLK_LSB") && mCLKBUSPeriod != 0) && mIsTIBEn) {
        mTimeTick[block_num] = GetCurTime();
        mIsTCNTB0OVF[block_num] = true;
        mTCNTB0CntEvent[block_num].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : HandleTCNTB5OVFMethod
// Description  : Handle overflow event of TCNTB5
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::HandleTCNTB5OVFMethod (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && mIsTIBEnDontCareClock) {
        mTimeTick5 = GetCurTime();
        mIsTCNTB5OVF = true;
        mTCNTB5CntEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : StartTCNTB0Method
// Description  : Handle starting TCNTB0/TCNTB0S1/TCNTB0S2 counter
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5010_TIMERB::StartTCNTB0Method (const unsigned int block_num)
{//{{{
    unsigned int cur_cnt = GetCounterValue("TCNTB0", block_num);
    mIsTCNTB0Running[block_num] = mIsTIBEn;
    if (mIsTCNTB0Running[block_num] && !mATU5010->CheckClockPeriod("CLK_LSB") && (mCLKBUSPeriod == 0)) {
        mIsTCNTB0Running[block_num] = false;
    }
    mIsTCNTB0CorMatch[block_num] = false;
    mIsTCNTB0OVF[block_num] = false;
    mTCNTB0Cnt[block_num]->StartStop(mIsTCNTB0Running[block_num]);
    if (mIsTCNTB0Running[block_num]) {
        cur_cnt = mATU5010_TIB_Func->GetTCNTB0(block_num)+1;
        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cur_cnt);
        mTimeTick[block_num] = GetCurTime();
    }
    mATU5010_TIB_Func->SetTCNTB0(block_num, cur_cnt);
}//}}}

/*********************************
// Function     : StartTCNTB5Method
// Description  : Handle starting TCNTB5 auto count up start
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::StartTCNTB5Method (void)
{//{{{
    unsigned int cur_cnt = GetCounterValue("TCNTB5", 0);
    if (mIsTCNTB5AutoCnt && !mATU5010->CheckClockPeriod("CLK_LSB")) {
        mIsTCNTB5AutoCnt = false;
    }
    mIsTCNTB5CorMatch = false;
    mIsTCNTB5OVF = false;
    mTCNTB5Cnt->SetCounterPeriod(mATU5010->mCLK_LSBPeriod);
    mTCNTB5Cnt->StartStop(mIsTCNTB5AutoCnt);
    if (mIsTCNTB5AutoCnt) {
        cur_cnt = mATU5010_TIB_Func->GetTCNTB5()+1;
        mTCNTB5Cnt->SetCounterInitialValue(cur_cnt);
        mTimeTick5 = GetCurTime();
        mAGCKMPeriod = mATU5010->mCLK_LSBPeriod;
        AGCKM.write(mAGCKMPeriod);
        mATU5010_TIB_Func->SetTCNTB5(cur_cnt);
        mTCNTDCLRVal = cur_cnt == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
        mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
    } else {
        mClearAGCKMEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : TCNTB0CntThread
// Description  : Handle TCNTB0 counter process
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::TCNTB0CntThread (void)
{//{{{
    unsigned int block_num = 0;
    for(;;) {
        if(!mIsTCNTB0Running[block_num] || IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
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
                        mATU5010_TIB_Func->SetTCNTB0(block_num, 0xFFFFFFFF);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(0xFFFFFFFF);
                    } else {
                        mATU5010_TIB_Func->SetTCNTB0(block_num, cnt_val);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val);
                    }
                    wait(mCLKBUSPeriod, mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod(mCLKBUSPeriod);
                        wait(CalculateClockEdge(mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                            throw vpcl::emRstException;
                        }
                        mIsCLKBusUpdated[block_num] = false;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(1 + mATU5010_TIB_Func->GetTCNTB0(block_num));
                    } else {
                        if (mIsTCNTB0OVF[block_num]) {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(0);
                        } else {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                        }
                    }
                    if (mIsTCNTB0CorMatch[block_num] && (block_num==GetEIMBlkNum())) {
                        mIntrActiveEvent[emCMIB0].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
                    }
                    mIsTCNTB0CorMatch[block_num] = false;
                    mIsTCNTB0OVF[block_num] = false;
                } else {
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod(mCLKBUSPeriod);
                    }
                    wait(CalculateClockEdge(mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        cnt_val = mATU5010_TIB_Func->GetTCNTB0(block_num);
                    }
                    if ((!mIsCLKBusUpdated[block_num]) || (mIsPreCLKBUSzero)) {
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                    }
                    mIsCLKBusUpdated[block_num] = false;
                }
                wait();
                if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
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
void ATU5010_TIMERB::TCNTB0S1CntThread (void)
{//{{{
    unsigned int block_num = 1;
    for(;;) {
        if(!mIsTCNTB0Running[block_num] || IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
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
                        mATU5010_TIB_Func->SetTCNTB0(block_num, 0xFFFFFFFF);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(0xFFFFFFFF);
                    } else {
                        mATU5010_TIB_Func->SetTCNTB0(block_num, cnt_val);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val);
                    }
                    wait(mCLKBUSPeriod, mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod(mCLKBUSPeriod);
                        wait(CalculateClockEdge(mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                            throw vpcl::emRstException;
                        }
                        mIsCLKBusUpdated[block_num] = false;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(1 + mATU5010_TIB_Func->GetTCNTB0(block_num));
                    } else {
                        if (mIsTCNTB0OVF[block_num]) {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(0);
                        } else {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                        }
                    }
                    if (mIsTCNTB0CorMatch[block_num] && (block_num==GetEIMBlkNum())) {
                        mIntrActiveEvent[emCMIB0].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
                    }
                    mIsTCNTB0CorMatch[block_num] = false;
                    mIsTCNTB0OVF[block_num] = false;
                } else {
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod(mCLKBUSPeriod);
                    }
                    wait(CalculateClockEdge(mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        cnt_val = mATU5010_TIB_Func->GetTCNTB0(block_num);
                    }
                    if ((!mIsCLKBusUpdated[block_num]) || (mIsPreCLKBUSzero)) {
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                    }
                    mIsCLKBusUpdated[block_num] = false;
                }
                wait();
                if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
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
void ATU5010_TIMERB::TCNTB0S2CntThread (void)
{//{{{
    unsigned int block_num = 2;
    for(;;) {
        if(!mIsTCNTB0Running[block_num] || IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0) || !mIsTIBEn) {
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
                        mATU5010_TIB_Func->SetTCNTB0(block_num, 0xFFFFFFFF);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(0xFFFFFFFF);
                    } else {
                        mATU5010_TIB_Func->SetTCNTB0(block_num, cnt_val);
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val);
                    }
                    wait(mCLKBUSPeriod, mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod(mCLKBUSPeriod);
                        wait(CalculateClockEdge(mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                        if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                            throw vpcl::emRstException;
                        }
                        mIsCLKBusUpdated[block_num] = false;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(1 + mATU5010_TIB_Func->GetTCNTB0(block_num));
                    } else {
                        if (mIsTCNTB0OVF[block_num]) {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(0);
                        } else {
                            mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                        }
                    }
                    if (mIsTCNTB0CorMatch[block_num] && (block_num==GetEIMBlkNum())) {
                        mIntrActiveEvent[emCMIB0].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
                    }
                    mIsTCNTB0CorMatch[block_num] = false;
                    mIsTCNTB0OVF[block_num] = false;
                } else {
                    mTCNTB0Cnt[block_num]->StartStop(false);
                    if (mIsCLKBusUpdated[block_num]) {
                        mTCNTB0Cnt[block_num]->SetCounterPeriod(mCLKBUSPeriod);
                    }
                    wait(CalculateClockEdge(mCLKBUSPeriod, true, false, GetCurTime(), mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0] | mClkZeroEvent);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
                        throw vpcl::emRstException;
                    }
                    mTCNTB0Cnt[block_num]->StartStop(true);
                    if (mIsTCNTB0Updated[block_num]) {
                        mIsTCNTB0Updated[block_num] = false;
                        cnt_val = mATU5010_TIB_Func->GetTCNTB0(block_num);
                    }
                    if ((!mIsCLKBusUpdated[block_num]) || (mIsPreCLKBUSzero)) {
                        mTCNTB0Cnt[block_num]->SetCounterInitialValue(cnt_val+1);
                    }
                    mIsCLKBusUpdated[block_num] = false;
                }
                wait();
                if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || (mCLKBUSPeriod == 0)) {
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
// Function     : TCNTB5CntThread
// Description  : Handle TCNTB5 counter process
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::TCNTB5CntThread (void)
{//{{{
    for(;;) {
        if(!mIsTCNTB5AutoCnt || IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB") || !mIsTIBEnDontCareClock) {
            mTCNTB5Cnt->StartStop(false);
            unsigned int cnt_val = mTCNTB5Cnt->GetCounterValue();
            mATU5010_TIB_Func->SetTCNTB5(cnt_val);
            mTCNTDCLRVal = (cnt_val == mATU5010_TIB_Func->GetTCCLRB()) && mIsTIBEnDontCareClock ? true : false;
            mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
            wait();
            continue;
        }
        try {
            while (true) {
                unsigned int cnt_val = mTCNTB5Cnt->GetCounterValue();
                if ((mTimeTick5 == GetCurTime())&&(mIsTCNTB5CorMatch || mIsTCNTB5OVF)) {
                    cnt_val = mTCNTB5Cnt->GetCompareMatchValue();
                    mTCNTB5Cnt->StartStop(false);
                    if (mIsTCNTB5OVF) {
                        mATU5010_TIB_Func->SetTCNTB5(0xFFFFF);
                    } else {
                        mATU5010_TIB_Func->SetTCNTB5(cnt_val);
                    }
                    mTCNTDCLRVal = mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
                    mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
                    wait(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0]);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB")) {
                        throw vpcl::emRstException;
                    }
                    if (mIsCLKLSBUpdated) {
                        mTCNTB5Cnt->SetCounterPeriod(mATU5010->mCLK_LSBPeriod);
                        wait(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0]);
                        if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB")) {
                            throw vpcl::emRstException;
                        }
                        mIsCLKLSBUpdated = false;
                    }
                    if (mIsTCNTB5OVF) {
                        mTCNTB5Cnt->StartStop(true);
                        if (mIsTCNTB5Updated) {
                            mIsTCNTB5Updated = false;
                            mTCNTB5Cnt->SetCounterInitialValue(1 + mATU5010_TIB_Func->GetTCNTB5());
                            mATU5010_TIB_Func->SetTCNTB5(1 + mATU5010_TIB_Func->GetTCNTB5());
                        } else {
                            mTCNTB5Cnt->SetCounterInitialValue(1);
                            mATU5010_TIB_Func->SetTCNTB5(1);
                        }
                        mTCNTDCLRVal = mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
                        mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
                    } else {
                        mIsTCNTB5AutoCnt = false;
                        if (mIsTCNTB5AutoCntTrg) {
                            mATU5010_TIB_Func->SetTCCLFB(0, true);
                            mIsTCNTB5AutoCntTrg = false;
                            mTCNTDCLRVal = mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
                            mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
                            CheckTCNTB5Update();
                        }
                        if (!mIsTCNTB5AutoCnt) {
                            mAGCKMPeriod = 0;
                            AGCKM.write(mAGCKMPeriod);
                        }
                    }
                    mIsTCNTB5CorMatch = false;
                    mIsTCNTB5OVF = false;
                } else {
                    mTCNTB5Cnt->StartStop(false);
                    if (mIsCLKLSBUpdated) {
                        mIsCLKLSBUpdated = false;
                        mTCNTB5Cnt->SetCounterPeriod(mATU5010->mCLK_LSBPeriod);
                    }
                    wait(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]),mATU5010->mTimeResolutionUnit, mRstStateEvent | mATU5010->mClkZeroEvent[0]);
                    if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB")) {
                        throw vpcl::emRstException;
                    }
                    mTCNTB5Cnt->StartStop(true);
                    if (mIsTCNTB5Updated) {
                        mIsTCNTB5Updated = false;
                        cnt_val = mATU5010_TIB_Func->GetTCNTB5();
                    }
                    mTCNTB5Cnt->SetCounterInitialValue(cnt_val+1);
                    mATU5010_TIB_Func->SetTCNTB5(cnt_val+1);
                    mTCNTDCLRVal = mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB() ? true : false;
                    mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
                }
                wait();
                if (IsModelResetActive() || !mATU5010->CheckClockPeriod("CLK_LSB")) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
            mIsCLKLSBUpdated = false;
            mIsTCNTB5CorMatch = false;
            mIsTCNTB5OVF = false;
            mIsTCNTB5Updated = false;
            mTCNTB5Cnt->StartStop(false);
            unsigned int cnt_val = mTCNTB5Cnt->GetCounterValue();
            mATU5010_TIB_Func->SetTCNTB5(cnt_val);
            mTCNTDCLRVal = (cnt_val == mATU5010_TIB_Func->GetTCCLRB()) && mIsTIBEnDontCareClock ? true : false;
            mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
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
void ATU5010_TIMERB::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mATU5010->mDumpInterrupt) {
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
void ATU5010_TIMERB::ControlInterruptPorts (const unsigned int interrupt_id, const bool set_val)
{//{{{
    std::string intr_name;
    bool pre_value;
    bool written = false;
    switch (interrupt_id) {
        case emCMIB0:
            intr_name = "CMIB0";
            pre_value = CMIB0.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB0IE")&& set_val)) {
                CMIB0.write(set_val);
                written = true;
            }
            break;
        case emICIB0:
            intr_name = "ICIB";
            pre_value = ICIB.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("ICFB0IE")&& set_val)) {
                ICIB.write(set_val);
                written = true;
            }
            break;
        case emCMIB1:
            intr_name = "CMIB1";
            pre_value = CMIB1.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB1IE")&& set_val)) {
                CMIB1.write(set_val);
                written = true;
            }
            break;
        case emCMIB6:
            intr_name = "CMIB6";
            pre_value = CMIB6.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB6IE")&& set_val)) {
                CMIB6.write(set_val);
                written = true;
            }
            break;
        case emCMIB10:
            intr_name = "CMIB10";
            pre_value = CMIB10.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB10IE")&& set_val)) {
                CMIB10.write(set_val);
                written = true;
            }
            break;
        case emCMIB11:
            intr_name = "CMIB11";
            pre_value = CMIB11.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB11IE")&& set_val)) {
                CMIB11.write(set_val);
                written = true;
            }
            break;
        case emCMIB12:
            intr_name = "CMIB12";
            pre_value = CMIB12.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB12IE")&& set_val)) {
                CMIB12.write(set_val);
                written = true;
            }
            break;
        case emCMIB6M:
            intr_name = "CMIB6M";
            pre_value = CMIB6M.read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB6MIE")&& set_val)) {
                CMIB6M.write(set_val);
                written = true;
            }
            break;
        default://emCMIB6E
            intr_name = "CMIB6E";
            pre_value = CMIB6E->read();
            if (!set_val || (mATU5010_TIB_Func->IsInterruptEnable("CMFB6EIE") && set_val)) {
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
void ATU5010_TIMERB::SetCLKBusPeriod (const unsigned int clkbus_sel)
{//{{{
    mCLKBUSPeriod = mCLKBUS_nPeriod[clkbus_sel];
    if (mCLKBUSPeriod == 0){
        mClkZeroEvent.notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : SetCLKAGCK1Period
// Description  : Set AGCK1 clock period
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::SetCLKAGCK1Period (void)
{//{{{
    if (mIsTIBEn) {
        unsigned int cycle;
        if (mIsTCNTB2LoadState[1]) {
            if (mIsTCNTB2LoadState[2]) {
                mIsAGCK1Update = false;
                mTCNTB2LoadEvent.notify(mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit); //Update TCNTB2 = ICRB0
            }
            mIsTCNTB2LoadState[1] = false;
        } else if ((mIsTCNTB2Updated) || (mIsPIMUpdated) || (mIsPSEnable) || ((mIsTCNTB2LoadState[2]) && !mATU5010_TIB_Func->IsLDEN())) {
            mIsAGCK1Update = false;
            AGCK1.write(0);
            mIsTCNTB2Updated = false;
            mIsPIMUpdated = false;
            mIsPSEnable = false;
            mIsTCNTB2LoadState[2] = false;
        }
        if (mCurTCNTB2 <= mCurPIM) {
            cycle = 1;
            mAGCK1Period = mCLKBUSPeriod * cycle;
        } else {
            double div_cycle = (double)mCurTCNTB2 / (double)mCurPIM;
            cycle = (unsigned int) div_cycle;
            if (cycle < div_cycle) {
                cycle ++;
            }
            mAGCK1Period = mCLKBUSPeriod * cycle;
        }
        if (mAGCK1Period != AGCK1.read()){
            if (mIsAGCK1Update) {
                std::string time_u = GetTimeUnitStr(mATU5010->mTimeResolutionUnit);
                re_printf("info","Frequency multiplied clock (AGCK1) period is changed to %llu (%s)\n", mAGCK1Period, time_u.c_str());
                AGCK1.write(mAGCK1Period);
            }
        }
        if (mAGCK1Period != 0) {
            mTimeCLKChangePoint[1] = GetCurTime() + CalculateClockEdge(mCLKBUSPeriod, true, true, GetCurTime(), mTimeCLKChangePoint[0]) - (double)mCLKBUSPeriod;
            double agck1_ticktime = CalculateClockEdge(mAGCK1Period, true, true, GetCurTime(), mTimeCLKChangePoint[1]);
            double agck1_curticktime = CalculateClockEdge(mAGCK1Period, true, false, GetCurTime(), mTimeCLKChangePoint[1]);
            mTCNTB2CntUpEvent.cancel();
            mTCNTB2CntUpEvent.notify(agck1_ticktime,mATU5010->mTimeResolutionUnit); //Update TCNTB += RLDB
            mTCNTB6LoadEvent.cancel();
            if (mIsAGCK1Update) {
                mTCNTB6LoadEvent.notify(agck1_curticktime+mATU5010->mCLK_LSBPeriod, mATU5010->mTimeResolutionUnit);
            }
        }
        mIsAGCK1Update = false;
    }
}//}}}


/*********************************
// Function     : TCNTB2CntUpMethod
// Description  : Count up TCNTB2 method
// Parameter    :
// Return value : None
**********************************/
void ATU5010_TIMERB::TCNTB2CntUpMethod (void)
{//{{{
    mIsTCNTB2LoadState[1] = true;
    if (mIsTCNTB2LoadState[2]) {
        mTCNTB2LoadEvent.cancel();
    }
    mTCNTB2LoadEvent.notify(SC_ZERO_TIME); //Update TCNTB += RLDB
}//}}}

/*********************************
// Function     : TCNTB1CntUp
// Description  : Count up TCNTB1/TCNTB1S1/TCNTB1S2
// Parameter    :
//      block_num           Block number 0 -> 2
// Return value : None
**********************************/
void ATU5010_TIMERB::TCNTB1CntUp (const unsigned int block_num)
{//{{{
    if (mIsTCNTB1ClrReq[0][block_num] == true) {
        mIsTCNTB1ClrReq[0][block_num] = false;
        mATU5010_TIB_Func->SetTCNTB1(block_num, true, false);
    } else if (mIsTCNTB1ClrReq[1][block_num] == true) {
        mIsTCNTB1ClrReq[1][block_num] = false;
        mATU5010_TIB_Func->SetTCNTB1(block_num, true, true);
    } else {
        mATU5010_TIB_Func->SetTCNTB1(block_num, false, false);
    }
    mIsTCNTB11CorMatch[block_num] = false;
    unsigned int cur_tcntb1 = mATU5010_TIB_Func->GetTCNTB1(block_num);
    unsigned int cur_ocrb1 = mATU5010_TIB_Func->GetOCRB1(block_num);
    unsigned int cur_ocrb10 = mATU5010_TIB_Func->GetOCRB10(block_num);
    unsigned int cur_ocrb11 = mATU5010_TIB_Func->GetOCRB11(block_num);
    unsigned int cur_ocrb12 = mATU5010_TIB_Func->GetOCRB12(block_num);
    if (mIsSeqEn) {
        unsigned int state;
        unsigned int cur_ocrb20_43;
        for (state = 1; state <= emSeqStatenum; state++) {
            if (mATU5010_TIB_Func->GetTSEQNENB(state)) {
                cur_ocrb20_43 = mATU5010_TIB_Func->GetOCRB20_43(state);
                if (cur_tcntb1 == cur_ocrb20_43){
                    mATU5010_TIB_Func->SetTSEQRB(state);
                    break;
                }
            }
        }
        state = mATU5010_TIB_Func->GetTSEQRB();
        mATU5010_TIB_Func->SetTEPCVALRB0(state);
    } else {
        mATU5010_TIB_Func->SetTSEQRB(0); //Clear to 0
    }
    if (cur_tcntb1 == cur_ocrb1) {
        mIsTCNTB1CorMatch[block_num] = true;
        mCurICRB1[block_num] = mATU5010_TIB_Func->GetICRB1(block_num);
        mTCNTB1COREvent[block_num].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
    }
    if (cur_tcntb1 == cur_ocrb10) {
        mIsTCNTB1ClrReq[0][block_num] = true;
        mIntrActiveEvent[emCMIB10].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
        mIsTCNTB3ClrReq = 1;
    }
    mIsPIMUpdated = true;
    if (cur_tcntb1 == cur_ocrb11) {
        mIsTCNTB11CorMatch[block_num] = true;
        mIntrActiveEvent[emCMIB11].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
    } else {
        mIsTCNTB11CorMatch[block_num] = false;
    }
    if (cur_tcntb1 == cur_ocrb12) {
        mIntrActiveEvent[emCMIB12].notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, false, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
        mATU5010_TIB_Func->UpdateRBURB();
    }
}//}}}

/*********************************
// Function     : TCNTB2LoadInit
// Description  : Update load value for TCNTB2 when TCNTB2 updated/Bus clock updated/PIM updated
// Parameter    : None
// Return value : None
**********************************/
void ATU5010_TIMERB::TCNTB2LoadInit (void)
{//{{{
    bool update = false;
    if ((!mIsTCNTB2LoadState[1]) && mIsPIMUpdated) {
        mCurTCNTB2 = GetCounterValue("TCNTB2", 0);
        if (mCurPIM != mATU5010_TIB_Func->GetPIM()) {
            mCurPIM = mATU5010_TIB_Func->GetPIM();
            update = true;
        } else {
            mIsPIMUpdated = false;
            mTimeTick2 = GetCurTime();
        }
        if (!mATU5010_TIB_Func->IsLDEN()) {
            mIsTCNTB2LoadState[2] = false;
        }
    }
    if (mIsTCNTB2Updated) {
        mCurTCNTB2 = mATU5010_TIB_Func->GetTCNTB2();
        mIsTCNTB2LoadState[1] = false;
        mIsTCNTB2LoadState[2] = false;
        update = true;
    }
    if (mIsTCNTB2LoadState[1]) {
        mCurTCNTB2 = GetCounterValue("TCNTB2", 0);
        mCurTCNTB2 = mCurTCNTB2 + mATU5010_TIB_Func->GetRLDB();
        update = true;
    } else if (mIsTCNTB2LoadState[2]) {
        if (mATU5010_TIB_Func->IsLDEN()) {
            mCurTCNTB2 = mATU5010_TIB_Func->GetLDVal();
            mATU5010_TIB_Func->SetTCNTB2(mCurTCNTB2);
            mATU5010_TIB_Func->SetRLDB(mATU5010_TIB_Func->GetLDVal() - mATU5010_TIB_Func->GetPIM());
            mIsTCNTB2LoadState[1] = false;
            mIsTCNTB2LoadState[2] = false;
        } else {
            mCurTCNTB2 = GetCounterValue("TCNTB2", 0);
        }
        update = true;
    }
    if (mCLKBUSPeriod == 0) {
        mCurTCNTB2 = mATU5010_TIB_Func->GetTCNTB2();
    }
    if (mIsPSEnable) {
        mCurTCNTB2 -= mCurPIM;
        update = true;
    }
    if (update) {
        mTimeTick2 = GetCurTime();
        mIsAGCK1Update = true;
    }
    SetCLKAGCK1Period();
}//}}}

/*********************************
// Function     : IsModelResetActive
// Description  : Check active level of presetz
// Parameter    : None
// Return value : Current active level of presetz
**********************************/
bool ATU5010_TIMERB::IsModelResetActive(void)
{//{{{
    bool ret = false;
    if (mATU5010->mIsResetHardActive[1] || mATU5010->mIsResetCmdReceive[1]) {
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
unsigned int ATU5010_TIMERB::GetEIMBlkNum(void)
{//{{{
    if (mATU5010_TIB_Func->GetChannelSel() == 0x1) {
        return 1;
    } else if (mATU5010_TIB_Func->GetChannelSel() == 0x2) {
        return 2;
    } else {
        return 0;
    }
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time
**********************************/
double ATU5010_TIMERB::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

/*********************************
// Function     : CalculateClockEdge
// Description  : Calculate synchronous time
// Parameter    :
//      clock_period        Clock period (unit depend on mATU5010->mTimeResolutionValue)
//      is_pos              true: raising edge; false: falling edge
//      add_period          true: 0 < syn_time <= 1; false: 0 <= syn_time < 1
//      time_point          Time point
//      time_change_point   Time point clock is updated
// Return value : The duration from time_point to next valid clock edge
**********************************/
double ATU5010_TIMERB::CalculateClockEdge (double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
{//{{{
    if (clock_period == 0) {
    	return 0;
    } else {
    	double period_num = 0;
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
    }
}//}}}

std::string ATU5010_TIMERB::GetTimeUnitStr (sc_time_unit time_u)
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

void ATU5010_TIMERB::NotifyUpdatedTCCLRBVal (const unsigned int value)
{//{{{
    if (mIsTCNTB5AutoCnt) {
        mTCNTB5Cnt->SetCompareMatchValue(value);
        mTCNTB5CntEvent.notify(SC_ZERO_TIME);
    } else {
        CheckTCNTB5Update();
    }
    mTCNTDCLRVal = (mATU5010_TIB_Func->GetTCNTB5() == mATU5010_TIB_Func->GetTCCLRB()) && mIsTIBEnDontCareClock ? true : false;
    mWriteTCNTDCLREvent.notify(SC_ZERO_TIME);
}//}}}

void ATU5010_TIMERB::CheckTCNTB5Update (void)
{//{{{
    if (mATU5010->CheckClockPeriod("CLK_LSB") && (mIsTIBEnDontCareClock) && (((mATU5010_TIB_Func->GetTIORBVal() >> 6) & 0x1) == 0x0)) {
        unsigned int tcntb4 = mATU5010_TIB_Func->GetTCNTB4();
        unsigned int tcntb5 = mATU5010_TIB_Func->GetTCNTB5();
        unsigned int tcclrb = mATU5010_TIB_Func->GetTCCLRB();
        unsigned int cor_val = tcntb4 < tcclrb ? tcntb4 : tcclrb;
        if ((tcntb5 + 1) < cor_val) {
            mIsTCNTB5AutoCnt = true;
            mIsTCNTB5AutoCntTrg = false;
            mTCNTB5Cnt->SetCompareMatchValue(cor_val);
            mTCNTB5Cnt->SetCounterInitialValue(tcntb5);
            mStartTCNTB5Event.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
        } else if ((tcntb5 + 1) == cor_val) {
            mTCNTB5LoadEvent.notify(CalculateClockEdge(mATU5010->mCLK_LSBPeriod, true, true, GetCurTime(), mATU5010->mTimeCLKChangePoint[0]), mATU5010->mTimeResolutionUnit);
            mIsTCNTB5AutoCnt = false;
        }
    }
}//}}}

void ATU5010_TIMERB::WriteTCNTDCLRMethod (void)
{//{{{
    TCNTDCLR.write(mTCNTDCLRVal);
}//}}}

void ATU5010_TIMERB::ClearAGCKMMethod (void)
{//{{{
    AGCKM.write(0);
}//}}}

// vim600: set foldmethod=marker :
