// ---------------------------------------------------------------------
// $Id$
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
#include "ATU5_TIMERD.h"
#include "ATU5_TIMERD_Func.h"
#include "ATU5_Timer.h"
#include "ATU5_TIMERD_SUB.h"
#include "ATU5_TIMERD_SUB_Func.h"
#include "ATU5_Base.h"
///*********************************
/// Function     : ATU5_TIMERD_SUB
/// Description  : Constructor of ATU5_TIMERD_SUB class
///*********************************
ATU5_TIMERD_SUB::ATU5_TIMERD_SUB (sc_module_name name,
            ATU5_TIMERD *ATU5_TIMERD_Ptr,
            unsigned int sub):
                sc_module(name),
                ATU5_TIMERD_SUB_AgentController(),
                //input ports
                TAEO2A("TAEO2A"),
                TAEO2B("TAEO2B"),
                PHU0DFE("PHU0DFE"),
                PHU1DFE("PHU1DFE"),
                CLRB("CLRB"),
                INT_OVF1("INT_OVF1"),
                INT_OVF2("INT_OVF2")
                //output ports


{//{{{
   assert(ATU5_TIMERD_Ptr != NULL);
   mTIMERD = ATU5_TIMERD_Ptr;
   subnum = sub;
    std::ostringstream port_name;
    for (unsigned int index = 0; index < ATU5_TIMERD_SUB_AgentController::emChannel; index++) {
        port_name.str("");
        port_name << "TODA" << index;
        TODA[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TODA[index] != NULL);

        port_name.str("");
        port_name << "TODB" << index;
        TODB[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TODB[index] != NULL);
        
        port_name.str("");
        port_name << "TODMIG" << index;
        TODMIG[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(TODMIG[index] != NULL);

        port_name.str("");
        port_name << "INT_CMPA" << index;
        INT_CMPA[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_CMPA[index] != NULL);

        port_name.str("");
        port_name << "INT_CMPB" << index;
        INT_CMPB[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_CMPB[index] != NULL);

        port_name.str("");
        port_name << "INT_FEIED" << index;
        INT_FEIED[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_FEIED[index] != NULL);

        port_name.str("");
        port_name << "INT_REIED" << index;
        INT_REIED[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_REIED[index] != NULL);

        port_name.str("");
        port_name << "INT_UDF" << index;
        INT_UDF[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(INT_UDF[index] != NULL);
    }


    mATU5_TIMERD_SUBFunc = new ATU5_TIMERD_SUB_Func((std::string)name, this);
    sc_assert(mATU5_TIMERD_SUBFunc != NULL);

    //TCNTD1
    std::ostringstream namecnt1;
    namecnt1.str("");namecnt1 << "mTimer_TCNT1D_CUCR1D";
    mTimer_TCNT1D_CUCR1D = new ATU5_Timer(namecnt1.str().c_str(), mTIMERD->mATU5);
    sc_assert(mTimer_TCNT1D_CUCR1D != NULL);

    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mTimer_TCNT1D_OCR1D" << index;
        mTimer_TCNT1D_OCR1D[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_TCNT1D_OCR1D[index] != NULL);

        name.str("");name << "mTimer_TCNT1D_OCR2D" << index;
        mTimer_TCNT1D_OCR2D[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_TCNT1D_OCR2D[index] != NULL);
    }

    //TCNTD2
    std::ostringstream namecnt2;
    namecnt2.str("");namecnt2 << "mTimer_TCNT2D_CUCR2D";
    mTimer_TCNT2D_CUCR2D = new ATU5_Timer(namecnt2.str().c_str(), mTIMERD->mATU5);
    sc_assert(mTimer_TCNT2D_CUCR2D != NULL);

    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mTimer_TCNT2D_OCR1D" << index;
        mTimer_TCNT2D_OCR1D[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_TCNT2D_OCR1D[index] != NULL);

        name.str("");name << "mTimer_TCNT2D_OCR2D" << index;
        mTimer_TCNT2D_OCR2D[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_TCNT2D_OCR2D[index] != NULL);
    }

    //DCNT
    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mTimer_DCNTD" << index;
        mTimer_DCNTD[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_DCNTD[index] != NULL);
    }
    
    //Measurement counter
    for (unsigned int index = 0; index <= 3; index++) {
        std::ostringstream name;
        name.str("");name << "mTimer_OFMICNTD" << index;
        mTimer_OFMICNTD[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_OFMICNTD[index] != NULL);

        name.str("");name << "mTimer_ONMICNTD" << index;
        mTimer_ONMICNTD[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_ONMICNTD[index] != NULL);
        
        name.str("");name << "mTimer_OTONMICNTD" << index;
        mTimer_OTONMICNTD[index] = new ATU5_Timer(name.str().c_str(), mTIMERD->mATU5);
        sc_assert(mTimer_OTONMICNTD[index] != NULL);
    }

    //Initialize ports
    for(unsigned int ch_id = 0; ch_id<ATU5_TIMERD_SUB_AgentController::emChannel; ch_id++) {
            TODB[ch_id]->initialize(false);
            TODA[ch_id]->initialize(false);
            TODMIG[ch_id]->initialize(false);
            INT_UDF[ch_id]->initialize(false);
            INT_CMPA[ch_id]->initialize(false);
            INT_CMPB[ch_id]->initialize(false);
            INT_FEIED[ch_id]->initialize(false);
            INT_REIED[ch_id]->initialize(false);
    }
    INT_OVF1.initialize(false);
    INT_OVF2.initialize(false);
    //Initialize variables - for clock, time resolution, reset
    Initialize();

    // Declare method/thread
     SC_METHOD(HandleInputCaptureMethod);
     dont_initialize();
     sensitive << TAEO2A
               << TAEO2B
               << PHU0DFE
               << PHU1DFE
               << mCaptureClockEvent;
     SC_METHOD(HandleInputClearCounterMethod);
     dont_initialize();
     sensitive << CLRB;

     SC_METHOD(StartTimer1Method);
     dont_initialize();
     sensitive << mStartTimerEvent << mStartTimer1Event;

     SC_METHOD(StartTimer2Method);
     dont_initialize();
     sensitive << mStartTimerEvent << mStartTimer2Event;
    // Overflow and Underflow method
     SC_METHOD(KeepCUCR1DValueOvfMethod);
     dont_initialize();
     sensitive << mTimer_TCNT1D_CUCR1D->mOverflowEvent;

     SC_METHOD(PreHandleCmpMatchCUCR1DMethod);
     dont_initialize();
     sensitive << mTimer_TCNT1D_CUCR1D->mCompareMatchEvent;

     SC_METHOD(Pre2HandleCmpMatchCUCR1DMethod);
     dont_initialize();
     sensitive << mCheckCompareMatchCUCR1DEvent;

     SC_METHOD(KeepCUCR2DValueOvfMethod);
     dont_initialize();
     sensitive << mTimer_TCNT2D_CUCR2D->mOverflowEvent;

     SC_METHOD(PreHandleCmpMatchCUCR2DMethod);
     dont_initialize();
     sensitive << mTimer_TCNT2D_CUCR2D->mCompareMatchEvent;

     SC_METHOD(Pre2HandleCmpMatchCUCR2DMethod);
     dont_initialize();
     sensitive << mCheckCompareMatchCUCR2DEvent;

     SC_METHOD(HandleCmpMatchCUCR1DMethod);
     dont_initialize();
     sensitive << mCompareMatchCUCR1DEvent;

     SC_METHOD(HandleCmpMatchCUCR2DMethod);
     dont_initialize();
     sensitive << mCompareMatchCUCR2DEvent;

     SC_METHOD(HandleInterruptOVF1Method);
     dont_initialize();
     sensitive << mInterruptOutputOVF1Event;

     SC_METHOD(HandleInterruptOVF2Method);
     dont_initialize();
     sensitive << mInterruptOutputOVF2Event;

     SC_METHOD(ClearInterruptOVF1Method);
     dont_initialize();
     sensitive << mClearInterrupt1Event;

     SC_METHOD(ClearInterruptOVF2Method);
     dont_initialize();
     sensitive << mClearInterrupt2Event;

     SC_METHOD(InputCaptureWriteMethod);
     dont_initialize();
     sensitive << mInputCaptureEvent;
   // DCNT and underflow methods

   // Compare match A counters
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTimer_TCNT1D_OCR1D[signal_id]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::PreHandleCmpMatchCNT1OCR1DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PreHandleCmpMatchCNT1OCR1DMethod"), &opt);
    }
        for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCmpMatchCNT1OCR1DEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleCmpMatchCNT1OCR1DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpMatchCNT1OCR1DMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTimer_TCNT2D_OCR1D[signal_id]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::PreHandleCmpMatchCNT2OCR1DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PreHandleCmpMatchCNT2OCR1DMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCmpMatchCNT2OCR1DEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleCmpMatchCNT2OCR1DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpMatchCNT2OCR1DMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mInterruptOutputCMPAEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleInterruptCMPAMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleInterruptCMPAMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mClearInterruptOutputCMPAEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::ClearInterruptCMPAMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("ClearInterruptCMPAMethod"), &opt);
    }

    // Compare match B counters
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTimer_TCNT1D_OCR2D[signal_id]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::PreHandleCmpMatchCNT1OCR2DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PreHandleCmpMatchCNT1OCR2DMethod"), &opt);
    }
     for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCmpMatchCNT1OCR2DEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleCmpMatchCNT1OCR2DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpMatchCNT1OCR2DMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTimer_TCNT2D_OCR2D[signal_id]->mCompareMatchEvent));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::PreHandleCmpMatchCNT2OCR2DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("PreHandleCmpMatchCNT2OCR2DMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCmpMatchCNT2OCR2DEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleCmpMatchCNT2OCR2DMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleCmpMatchCNT2OCR2DMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mInterruptOutputCMPBEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleInterruptCMPBMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleInterruptCMPBMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mClearInterruptOutputCMPBEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::ClearInterruptCMPBMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("ClearInterruptCMPBMethod"), &opt);
    }

    // Change value during operation
    for (unsigned int signal_id = 0; signal_id < 2; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mWriteCompareMatchValue[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::WriteCompareMatchValueMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WriteCompareMatchValueMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 2; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mWriteCounterValue[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::WriteCounterValueMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WriteCounterValueMethod"), &opt);
    }

    // DCNT counter method

    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mInterruptOutputFEIEDEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleInterruptFEIEDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleInterruptFEIEDMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mInterruptOutputREIEDEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleInterruptREIEDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleInterruptREIEDMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mClearInterruptOutputFEIEDEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::ClearInterruptFEIEDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("ClearInterruptFEIEDMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mClearInterruptOutputREIEDEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::ClearInterruptREIEDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("ClearInterruptREIEDMethod"), &opt);
    }
    //Measurement counter
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mStartOFMICNTDEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StartTimerOFMICNTDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StartTimerOFMICNTDMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++)
        {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mStartONMICNTDEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StartTimerONMICNTDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StartTimerONMICNTDMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mStartOTOMICNTDEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StartTimerOTOMICNTDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StartTimerOTOMICNTDMethod"), &opt);
    }
    
        for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTimer_OFMICNTD[signal_id]) -> mUnderflowEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StopTimerOFMICNTDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StopTimerOFMICNTDMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++)
        {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTimer_ONMICNTD[signal_id]) -> mUnderflowEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StopTimerONMICNTDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StopTimerONMICNTDMethod"), &opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mTimer_OTONMICNTD[signal_id]) -> mUnderflowEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StopTimerOTOMICNTDMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("StopTimerOTOMICNTDMethod"), &opt);
    }
    
    // TODA port
    for (unsigned int signal_id = 0; signal_id < 4; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&(mChangeOutputPort_TODAEvent[signal_id]));
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::HandleOutputPortTODAMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("HandleOutputPortTODAMethod"), &opt);
    }

      for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCompareMatchAEventDly[ch_id]);
        opt.set_sensitivity(&mCompareMatchBEventDly[ch_id]);
        opt.set_sensitivity(&mStartDCNTTriggerEvent[ch_id]);
        opt.set_sensitivity(&mTimer_DCNTD[ch_id]->mUnderflowEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::TriggerDCNTMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("TriggerDCNTMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCompareMatchAEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::AssertmIsCMAMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertmIsCMAMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mUpdateDCNTDEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::UpdateDCNTDMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("UpdateDCNTDMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mCompareMatchBEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::AssertmIsCMBMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertmIsCMBMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mTimer_DCNTD[ch_id]->mUnderflowEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::AssertmIsUNFMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertmIsUNFMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartDcntEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StartDcntMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("StartDcntMethod"),&opt);
    }
    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStartDCNTDEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StartDCNTDMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("StartDCNTDMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mStopDcntEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::StopDcntMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("StopDcntMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mClearDSR1D_DSFDEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::ClearDSR1D_DSFDMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("ClearDSR1D_DSFDMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertTODB_normalEvent[ch_id]);
        opt.set_sensitivity(&mDeassertTODB_normalEvent[ch_id]);
        opt.set_sensitivity(&mInvertedRegIsWriteEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::WriteTODBMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteTODBMethod"),&opt);
    }
    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mTODxChange_Event[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::ProcessMIGMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("ProcessMIGMethod"),&opt);
    }
    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mTriggerTODMIG_Event[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::WriteTODMIGMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteTODMIGMethod"),&opt);
    }
    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertTODB_normalEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::AssertTODB_normalMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertTODB_normalMethod"),&opt);
    }
    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeassertTODB_normalEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::DeassertTODB_normalMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeassertTODB_normalMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertINT_UDFEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::AssertINT_UDFMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertINT_UDFMethod"),&opt);
    }

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeassertINT_UDFEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::DeassertINT_UDFMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeassertINT_UDFMethod"),&opt);
    }
    for (unsigned int signal_id = 0; signal_id < 4; signal_id++) {      //add by ducla
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mProcessSettingEvent[signal_id]);
        opt.set_sensitivity(&mProcessClearingEvent[signal_id]);
        opt.set_sensitivity(&mProcessWriteTCNT1DEvent);
        opt.set_sensitivity(&mProcessWriteTCNT2DEvent); 
        opt.set_sensitivity(&mProcessClearTCNT1DEvent);
        opt.set_sensitivity(&mProcessClearTCNT2DEvent);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ATU5_TIMERD_SUB::ProcessPriorityMethod, this, signal_id),
        sc_core::sc_gen_unique_name("ProcessPriorityMethod"), &opt);
    }

 }//}}}

///*********************************
/// Function     : ~ATU5_TIMERD_SUB
/// Description  : Destructor of ATU5_TIMERD_SUB class
///*********************************
ATU5_TIMERD_SUB::~ATU5_TIMERD_SUB (void)
{//{{{
    delete mATU5_TIMERD_SUBFunc;
    delete mTimer_TCNT1D_CUCR1D;
    delete mTimer_TCNT2D_CUCR2D;
    for (unsigned int index = 0; index <= 3; index++) {
        delete mTimer_TCNT1D_OCR1D[index];
        delete mTimer_TCNT1D_OCR2D[index];
        delete mTimer_TCNT2D_OCR1D[index];
        delete mTimer_TCNT2D_OCR2D[index];
        delete mTimer_DCNTD[index];
        delete mTimer_OFMICNTD[index];
        delete mTimer_ONMICNTD[index];
        delete mTimer_OTONMICNTD[index];
    }

}//}}}
/***********************************
Function        : CalculateClockEdge
Description     : Calculate the Clock
***********************************/
double ATU5_TIMERD_SUB::CalculateClockEdge (const std::string clock_name, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
{//{{{
    double period_num = 0;

    // Returns 0 if input clock is 0
    bool zero_clock = false;
    if (clock_name == mCNT1CLKName) {
       // zero_clock = (mCNT1CLKPeriod == 0) ? true : false;
        if(mCNT1CLKPeriod == 0){
            zero_clock=true;
        }else{
            zero_clock=false;
        }
    } else if (clock_name == mCNT2CLKName) {
        //zero_clock = (mCNT2CLKPeriod == 0) ? true : false;
        if(mCNT2CLKPeriod == 0){
            zero_clock=true;
        }else{
            zero_clock=false;
        }        
    } else if (clock_name == mDCNTCLKName) {
        //zero_clock = (mDCNTCLKPeriod == 0) ? true : false;
        if(mDCNTCLKPeriod == 0){
            zero_clock=true;
        }else{
            zero_clock=false;
        } 
    } else if (clock_name == mPCLKName) {
        //zero_clock = (mTIMERD->mPCLKPeriod == 0) ? true : false;
        if(mTIMERD->mPCLKPeriod == 0){
            zero_clock=true;
        }else{
            zero_clock=false;
        }         
    }
    if (zero_clock) {
        return 0;
    }

    if (clock_name == mCNT1CLKName) {
        period_num = (time_point - time_change_point) / (double)mCNT1CLKPeriod;
    } else if (clock_name == mCNT2CLKName) {
        period_num = (time_point - time_change_point) / (double)mCNT2CLKPeriod;
    } else if (clock_name == mDCNTCLKName) {
        period_num = (time_point - time_change_point) / (double)mDCNTCLKPeriod;
    } else if (clock_name == mPCLKName) {
        period_num = (time_point - time_change_point) / (double)mTIMERD->mPCLKPeriod;
    }
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num; // revise by ducla 11/07/2018
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
    if (clock_name == mCNT1CLKName) {
        edge_point = (mod_period_num_db * (double)mCNT1CLKPeriod) - (time_point - time_change_point);
    } else if (clock_name == mCNT2CLKName) {
        edge_point = mod_period_num_db * (double)mCNT2CLKPeriod - (time_point - time_change_point);
    } else if (clock_name == mDCNTCLKName) {
        edge_point = mod_period_num_db * (double)mDCNTCLKPeriod - (time_point - time_change_point);
    } else if (clock_name == mPCLKName) {
        edge_point = mod_period_num_db * (double)mTIMERD->mPCLKPeriod - (time_point - time_change_point);
    }

    return edge_point;
}//}}}

/***********************************
Function        : RunningCounter
Description     : TSTRD set counter start
***********************************/
void ATU5_TIMERD_SUB::RunningCounter(const bool is_run)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (is_run) {
            mCounterEnable = true;
            StartStopTimer(true);
        } else {
            mCounterEnable = false;
            StartStopTimer(false);
        }
    }
}//}}}
/***********************************
Function        : Get counter value
Description     : Get counter value
***********************************/
unsigned int ATU5_TIMERD_SUB::GetCounterValue(unsigned int counter, unsigned int channel)
{//{{{
        unsigned int val = 0;
        if (counter == ATU5_TIMERD_SUB_AgentController::emCounter1) {
            val = mTimer_TCNT1D_CUCR1D->GetCounterValue();
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emCounter2) {
            val = mTimer_TCNT2D_CUCR2D->GetCounterValue();
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emDownCounter) {
            val = mTimer_DCNTD[channel]->GetCounterValue();
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emOFMICNTDCounter) {
            val = mTimer_OFMICNTD[channel]->GetCounterValue();
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emONMICNTDCounter) {
            val = mTimer_ONMICNTD[channel]->GetCounterValue();
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emOTOMICNTDCounter) {
            val = mTimer_OTONMICNTD[channel]->GetCounterValue();
        }
        return val;
}//}}}
/***********************************
Function        : WriteCounterValueMethod
Description     :
***********************************/
void ATU5_TIMERD_SUB::WriteCounterValueMethod(const unsigned int counter)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        if (mIsTimer1Running && (counter == ATU5_TIMERD_SUB_AgentController::emCounter1)) {
        StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1,false);
        mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
        } else if(mIsTimer2Running && (counter == ATU5_TIMERD_SUB_AgentController::emCounter2)){
        StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2,false);
        mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false , cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
}//}}}
/***********************************
Function        : WriteCompareMatchValueMethod
Description     :
***********************************/
void ATU5_TIMERD_SUB::WriteCompareMatchValueMethod(const unsigned int counter)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        if (mIsTimer1Running && (counter == ATU5_TIMERD_SUB_AgentController::emCounter1)) {
        StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1,true);
        mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
        } else if(mIsTimer2Running && (counter == ATU5_TIMERD_SUB_AgentController::emCounter2)){
        StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2,true);
        mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
}//}}}

/***********************************
Function        : StartStopTimer
Description     : Call to start or stop counter
***********************************/
void ATU5_TIMERD_SUB::StartStopTimer (const bool is_start)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        IsTimerRunning = false;
        IsTimerRunning = is_start;
        if (IsTimerRunning) {
            //Start Counter
            if(mCNT1CLKPeriod > 0) {
                mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
            if(mCNT2CLKPeriod > 0) {
                mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
        } else {
            //Stop Counter
            StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1,true);
            StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2,true);
        }
    }
}//}}}
/***********************************
Function        : StartTimer1Method
Description     : Call to start or stop counter
***********************************/
void ATU5_TIMERD_SUB::StartTimer1Method (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        StartTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1);
   }
}//}}}

/***********************************
Function        : StartTimer2Method
Description     : Call to start or stop counter
***********************************/
void ATU5_TIMERD_SUB::StartTimer2Method (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        StartTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2);
   }
}//}}}

// measurement counter
void ATU5_TIMERD_SUB::StartTimerOFMICNTDMethod (const unsigned int channel)
{//{{{
    #if 0
        re_printf("info", "StartTimerOFMICNTDMethod\n");
    #endif
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        unsigned int SetCounterValue = 0;
            if (!mIsTimerOFMICNTDRunning[channel]) {
                // set counter status bit
                mIsTimerOFMICNTDRunning[channel] = true ;
                SetCounterValue = mATU5_TIMERD_SUBFunc->GetOFMINDRegValue(channel);
                mATU5_TIMERD_SUBFunc->SetOFMICNTDRegValue(channel,SetCounterValue);

                // set count period
                mTimer_OFMICNTD[channel]->SetCounterMode(true);
                mTimer_OFMICNTD[channel]->SetCounterPeriod((double)mTIMERD->mPCLKPeriod);
                mTimer_OFMICNTD[channel]->SetupGeneralTimer(false, false); //count down
                mTimer_OFMICNTD[channel]->SetCounterInitialValue(SetCounterValue);
                mTimer_OFMICNTD[channel]->StartStop(mIsTimerOFMICNTDRunning[channel]);
            } 

        }
}//}}}
void ATU5_TIMERD_SUB::StartTimerONMICNTDMethod (const unsigned int channel)
{//{{{
    #if 0
        re_printf("info", "StartTimerONMICNTDMethod\n");
    #endif
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        unsigned int SetCounterValue = 0;
            if (!mIsTimerONMICNTDRunning[channel]) {
                mIsTimerONMICNTDRunning[channel] = true ;
                SetCounterValue = mATU5_TIMERD_SUBFunc->GetONMINDRegValue(channel);
                mATU5_TIMERD_SUBFunc->SetONMICNTDRegValue(channel,SetCounterValue);

                // set count period
                mTimer_ONMICNTD[channel]->SetCounterMode(true);
                mTimer_ONMICNTD[channel]->SetCounterPeriod((double)mTIMERD->mPCLKPeriod);
                mTimer_ONMICNTD[channel]->SetupGeneralTimer(false, false); //count down
                mTimer_ONMICNTD[channel]->SetCounterInitialValue(SetCounterValue);
                mTimer_ONMICNTD[channel]->StartStop(mIsTimerONMICNTDRunning[channel]);
            } 

        }
}//}}}
void ATU5_TIMERD_SUB::StartTimerOTOMICNTDMethod (const unsigned int channel)
{//{{{
    #if 0
        re_printf("info", "StartTimerOTOMICNTDMethod\n");
    #endif
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        unsigned int SetCounterValue = 0;
            if (!mIsTimerOTONMICNTDRunning[channel]) {
                // set counter status bit
                mIsTimerOTONMICNTDRunning[channel] = true ;
                SetCounterValue = mATU5_TIMERD_SUBFunc->GetOTOMINDRegValue(channel);
                mATU5_TIMERD_SUBFunc->SetOTONMICNTDRegValue(channel,SetCounterValue);

                // set count period
                mTimer_OTONMICNTD[channel]->SetCounterMode(true);
                mTimer_OTONMICNTD[channel]->SetCounterPeriod((double)mTIMERD->mPCLKPeriod);
                mTimer_OTONMICNTD[channel]->SetupGeneralTimer(false, false); //count down
                mTimer_OTONMICNTD[channel]->SetCounterInitialValue(SetCounterValue);
                mTimer_OTONMICNTD[channel]->StartStop(mIsTimerOTONMICNTDRunning[channel]);
            } 

        }
}//}}}
void ATU5_TIMERD_SUB::StopTimerOFMICNTDMethod (const unsigned int channel)
{//{{{
    #if 0
        re_printf("info", "StopTimerOFMICNTDMethod\n");
    #endif
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        unsigned int SetCounterValue = 0;
            if (mIsTimerOFMICNTDRunning[channel]) {
                // set counter status bit
                mIsTimerOFMICNTDRunning[channel] = false ;
                mATU5_TIMERD_SUBFunc->SetOFMICNTDRegValue(channel,0x0);
                mTimer_OFMICNTD[channel]->StartStop(mIsTimerOFMICNTDRunning[channel]);
                mOFMIZeroEventFunc(channel);
            } 
        }
}//}}}
void ATU5_TIMERD_SUB::StopTimerONMICNTDMethod (const unsigned int channel)
{//{{{
    #if 0
        re_printf("info", "StopTimerONMICNTDMethod\n");
    #endif
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        unsigned int SetCounterValue = 0;
            if (mIsTimerONMICNTDRunning[channel]) {
                // set counter status bit
                mIsTimerONMICNTDRunning[channel] = false ;
                mATU5_TIMERD_SUBFunc->SetONMICNTDRegValue(channel,0x0);
                mTimer_ONMICNTD[channel]->StartStop(mIsTimerONMICNTDRunning[channel]);
                mONMIZeroEventFunc(channel);
            } 
        }
}//}}}
void ATU5_TIMERD_SUB::StopTimerOTOMICNTDMethod (const unsigned int channel)
{//{{{
    #if 0
        re_printf("info", "StopTimerOTOMICNTDMethod\n");
    #endif
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        unsigned int SetCounterValue = 0;
            if (mIsTimerOTONMICNTDRunning[channel]) {
                // set counter status bit
                mIsTimerOTONMICNTDRunning[channel] = false ;
                mATU5_TIMERD_SUBFunc->SetOTONMICNTDRegValue(channel,0x0);
                mTimer_OTONMICNTD[channel]->StartStop(mIsTimerOTONMICNTDRunning[channel]);
                mOTONMIZeroEventFunc(channel);
            } 
        }
}//}}}
/*********************************
// Function     : StartTimerFunction
// Description  : Start timer
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERD_SUB::StartTimerFunction (unsigned int counter)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        unsigned int SetCounterValue = 0;
        if (counter == ATU5_TIMERD_SUB_AgentController::emCounter1) {
            if (!mIsTimer1Running) {
                mIsTimer1Running = true ;
            if(mOverflowTCNT1D || mCLRBTCNT1D){
               if(mOverflowTCNT1D){
                    mOverflowTCNT1D = false;
                }
                if(mInitialStage && mCLRBTCNT1D && (mCKSEL1DVal == 5)){ // figure 30.31, 30.32, 30.35: when using clock bus 5 (AGCKM), at initial stage, TCNT2Dx is cleared to 1 on its 1st count clock
                    mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(0x1);
                    SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT1DRegValue();
                    mInitialStage = false;
                } else {
                    mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(0x0);
                    SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT1DRegValue();
                }
                if(mCLRBTCNT1D){
                    mCLRBTCNT1D = false;
                }
            } else if(mCLRCounter1) {                
                if (mCLRSEL1DVal == true) {
                    mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(0x0);
                } else {
                    mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(0x1);
                }
                SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT1DRegValue();
                
            } else {
                SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT1DRegValue() + 1;
                }
            mCLRCounter1 = false;
            mOverflowTCNT1D = false;
                // set count period
                mTimer_TCNT1D_CUCR1D->SetCounterMode(true);
                mTimer_TCNT1D_CUCR1D->SetCounterPeriod((double)mCNT1CLKPeriod);
                mTimer_TCNT1D_CUCR1D->SetupGeneralTimer(false, true);
                mTimer_TCNT1D_CUCR1D->SetCounterInitialValue(SetCounterValue);
                mTimer_TCNT1D_CUCR1D->SetCompareMatchValue(mCUCR1DValue);
                for (unsigned int channel = 0; channel < 4; channel ++) {
                    mTimer_TCNT1D_OCR1D[channel]->SetCounterMode(true);
                    mTimer_TCNT1D_OCR1D[channel]->SetCounterPeriod((double)mCNT1CLKPeriod);
                    mTimer_TCNT1D_OCR1D[channel]->SetupGeneralTimer(false, true);
                    mTimer_TCNT1D_OCR1D[channel]->SetCounterInitialValue(SetCounterValue);
                    mTimer_TCNT1D_OCR1D[channel]->SetCompareMatchValue(mOCR1DValue[channel]);

                    mTimer_TCNT1D_OCR2D[channel]->SetCounterMode(true);
                    mTimer_TCNT1D_OCR2D[channel]->SetCounterPeriod((double)mCNT1CLKPeriod);
                    mTimer_TCNT1D_OCR2D[channel]->SetupGeneralTimer(false, true);
                    mTimer_TCNT1D_OCR2D[channel]->SetCounterInitialValue(SetCounterValue);
                    mTimer_TCNT1D_OCR2D[channel]->SetCompareMatchValue(mOCR2DValue[channel]);
                }
                // start counter
                mTimer_TCNT1D_CUCR1D->StartStop(mIsTimer1Running);
                if(SetCounterValue == mCUCR1DValue) {
                    mCheckCompareMatchCUCR1DEvent.notify(CalculateClockEdge(mPCLKName, false, false, cur_timer,mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emPCLKClockIndex]),mTIMERD->mATU5->mTimeResolutionUnit);
                }
                for (unsigned int channel = 0; channel < 4; channel ++) {
                    if (mCMPSEL1DVal[channel]==false){ //0: A-> TCNT1; 1:A->TCNT2
                        mTimer_TCNT1D_OCR1D[channel]->StartStop(mIsTimer1Running);
                        if(SetCounterValue == mOCR1DValue[channel]) {
                            mCmpMatchCNT1OCR1DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
                        }
                    }
                    if (mCMPSEL2DVal[channel]==true){ //0: B-> TCNT2; 1:A->TCNT1
                        mTimer_TCNT1D_OCR2D[channel]->StartStop(mIsTimer1Running);
                        if(SetCounterValue == mOCR2DValue[channel]) {
                            mCmpMatchCNT1OCR2DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
                        }
                    }
                }
            } 

        } else if(counter == ATU5_TIMERD_SUB_AgentController::emCounter2) {
            if (!mIsTimer2Running) {
                // set counter status bit
                mIsTimer2Running = true ;
                //
            if (mOverflowTCNT2D || mCLRBTCNT2D) {
                if(mOverflowTCNT2D){
                mOverflowTCNT2D = false;
                }
                if(mInitialStage && mCLRBTCNT2D && (mCKSEL2DVal == 5)){ // figure 30.31, 30.32, 30.35: when using clock bus 5 (AGCKM), at initial stage, TCNT2Dx is cleared to 1 on its 1st count clock
                    mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(0x1);
                    SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT2DRegValue();
                    mInitialStage = false;
                } else {
                    mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(0x0);
                    SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT2DRegValue();
                }
                if(mCLRBTCNT2D){
                    mCLRBTCNT2D = false;
                }
            } else if(mCLRCounter2) {
                   if (mCLRSEL2DVal == true) {
                        mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(0x0);
                   } else {
                       mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(0x1);
                     }
                   SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT2DRegValue();
           
            } else {
                SetCounterValue = mATU5_TIMERD_SUBFunc->GetTCNT2DRegValue() + 1;
            }
            mCLRCounter2 = false;
            mOverflowTCNT2D = false;
                // set count period
                mTimer_TCNT2D_CUCR2D->SetCounterMode(true);
                mTimer_TCNT2D_CUCR2D->SetupGeneralTimer(false, true);
                mTimer_TCNT2D_CUCR2D->SetCounterPeriod((double)mCNT2CLKPeriod);
                mTimer_TCNT2D_CUCR2D->SetCounterInitialValue(SetCounterValue);
                mTimer_TCNT2D_CUCR2D->SetCompareMatchValue(mCUCR2DValue);
                for (unsigned int channel = 0; channel < 4; channel ++) {
                    mTimer_TCNT2D_OCR1D[channel]->SetCounterMode(true);
                    mTimer_TCNT2D_OCR1D[channel]->SetupGeneralTimer(false, true);
                    mTimer_TCNT2D_OCR1D[channel]->SetCounterPeriod((double)mCNT2CLKPeriod);
                    mTimer_TCNT2D_OCR1D[channel]->SetCounterInitialValue(SetCounterValue);
                    mTimer_TCNT2D_OCR1D[channel]->SetCompareMatchValue(mOCR1DValue[channel]);

                    mTimer_TCNT2D_OCR2D[channel]->SetCounterMode(true);
                    mTimer_TCNT2D_OCR2D[channel]->SetupGeneralTimer(false, true);
                    mTimer_TCNT2D_OCR2D[channel]->SetCounterPeriod((double)mCNT2CLKPeriod);
                    mTimer_TCNT2D_OCR2D[channel]->SetCounterInitialValue(SetCounterValue);
                    mTimer_TCNT2D_OCR2D[channel]->SetCompareMatchValue(mOCR2DValue[channel]);

                }
                // start counter
                mTimer_TCNT2D_CUCR2D->StartStop(mIsTimer2Running);
                if(SetCounterValue == mCUCR2DValue) {
                   mCheckCompareMatchCUCR2DEvent.notify(CalculateClockEdge(mPCLKName, false, false, cur_timer,mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emPCLKClockIndex]),mTIMERD->mATU5->mTimeResolutionUnit);
                }
                for (unsigned int channel = 0; channel < 4; channel ++) {
                    if (mCMPSEL1DVal[channel]==true){ //0: A-> TCNT1; 1:A->TCNT2
                        mTimer_TCNT2D_OCR1D[channel]->StartStop(mIsTimer2Running);
                        if(SetCounterValue == mOCR1DValue[channel]) {
                            mCmpMatchCNT2OCR1DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
                        }
                    }
                    if (mCMPSEL2DVal[channel]==false){ //0: B-> TCNT2; 1:A->TCNT1
                        mTimer_TCNT2D_OCR2D[channel]->StartStop(mIsTimer2Running);
                        if(SetCounterValue == mOCR2DValue[channel]) {
                            mCmpMatchCNT2OCR2DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
                        }
                    }
                }
            } 
        }
    }
}//}}}

/*********************************
// Function     : StopTimerFunction
// Description  : Stop timer
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERD_SUB::StopTimerFunction (unsigned int counter, bool reg_update)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (counter == ATU5_TIMERD_SUB_AgentController::emCounter1) {
            if (mIsTimer1Running) {
                // set counter status bit
                mIsTimer1Running = false ;
                // get counter value
                if(reg_update) {
                     mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(mTimer_TCNT1D_CUCR1D->GetCounterValue());
                }
                // stop counter
                mTimer_TCNT1D_CUCR1D->StartStop(mIsTimer1Running);
                for (unsigned int channel = 0; channel < 4; channel ++) {
                    mTimer_TCNT1D_OCR1D[channel]->StartStop(mIsTimer1Running);
                    mTimer_TCNT1D_OCR2D[channel]->StartStop(mIsTimer1Running);
                }
            } 
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emCounter2) {
            if (mIsTimer2Running) {
                // set counter status bit
                mIsTimer2Running = false ;
                // get counter value
                if(reg_update) {
                    mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(mTimer_TCNT2D_CUCR2D->GetCounterValue());
                }
                // stop counter
                mTimer_TCNT2D_CUCR2D->StartStop(mIsTimer2Running);
                for (unsigned int channel = 0; channel < 4; channel ++) {
                    mTimer_TCNT2D_OCR1D[channel]->StartStop(mIsTimer2Running);
                    mTimer_TCNT2D_OCR2D[channel]->StartStop(mIsTimer2Running);
                }
            } 
        }
   }
}//}}}

// Adjust timing of event
void ATU5_TIMERD_SUB::KeepCUCR1DValueOvfMethod(void)
{//{{{
    if ((mCUCR1DOvfTime == sc_time_stamp().to_double()) && (mIsCUCR1DOvf)) {
        return;
    } else {
        mCUCR1DOvfTime = sc_time_stamp().to_double();
        mIsCUCR1DOvf = true;
        if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
        (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
            mOverflowTCNT1D = true;
            mIsTimer1Running = false;
            mTimer_TCNT1D_CUCR1D->StartStop(mIsTimer1Running);
            for (unsigned int channel = 0; channel < 4; channel ++) {
                mTimer_TCNT1D_OCR1D[channel]->StartStop(mIsTimer1Running);
                mTimer_TCNT1D_OCR2D[channel]->StartStop(mIsTimer1Running);
            }
            mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(0xFFFFFFFF);
            mCompareMatchCUCR1DEvent.notify((double)mCNT1CLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
}//}}}
void ATU5_TIMERD_SUB::KeepCUCR2DValueOvfMethod(void)
{//{{{
    if ((mCUCR2DOvfTime == sc_time_stamp().to_double()) && (mIsCUCR2DOvf)) {
        return;
    } else {
        mCUCR2DOvfTime = sc_time_stamp().to_double();
        mIsCUCR2DOvf = true;
        if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
        (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
            mOverflowTCNT2D = true;
            mIsTimer2Running = false;
            mTimer_TCNT2D_CUCR2D->StartStop(mIsTimer2Running);
            for (unsigned int channel = 0; channel < 4; channel ++) {
                mTimer_TCNT2D_OCR1D[channel]->StartStop(mIsTimer2Running);
                mTimer_TCNT2D_OCR2D[channel]->StartStop(mIsTimer2Running);
            }
            mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(0xFFFFFFFF);
            mCompareMatchCUCR2DEvent.notify((double)mCNT2CLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
}//}}}
void ATU5_TIMERD_SUB::PreHandleCmpMatchCUCR1DMethod(void)
{//{{{
    if (!mTimer_TCNT1D_CUCR1D->GetBitOVF() || (mCUCR1DValue == 0xFFFFFFFF)) {
        mTimer_TCNT1D_CUCR1D->ClearBitCMF();
        if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
        (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
            double cur_timer = sc_time_stamp().to_double();
            mCheckCompareMatchCUCR1DEvent.notify(CalculateClockEdge(mPCLKName, false, false, cur_timer,mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emPCLKClockIndex]),mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
    mTimer_TCNT1D_CUCR1D->ClearBitOVF();
}//}}}
void ATU5_TIMERD_SUB::Pre2HandleCmpMatchCUCR1DMethod(void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mOverflowTCNT1D){
            mOverflowTCNT1D = false;
            mCompareMatchCUCR1DEvent.cancel();
        }
        mCUCR1DMatch = true;
        double cur_timer = sc_time_stamp().to_double();
        mCompareMatchCUCR1DEvent.notify(CalculateClockEdge(mPCLKName, true, false, cur_timer,mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emPCLKClockIndex]),mTIMERD->mATU5->mTimeResolutionUnit);
    }
}//}}}
void ATU5_TIMERD_SUB::PreHandleCmpMatchCUCR2DMethod(void)
{//{{{
    if (!mTimer_TCNT2D_CUCR2D->GetBitOVF() || (mCUCR2DValue == 0xFFFFFFFF)) {
        mTimer_TCNT2D_CUCR2D->ClearBitCMF();
        if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
        (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
        mCheckCompareMatchCUCR2DEvent.notify(CalculateClockEdge(mPCLKName, false, false, cur_timer,mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emPCLKClockIndex]),mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
    mTimer_TCNT2D_CUCR2D->ClearBitOVF();
}//}}}
void ATU5_TIMERD_SUB::Pre2HandleCmpMatchCUCR2DMethod(void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mOverflowTCNT2D){
            mOverflowTCNT2D = false;
            mCompareMatchCUCR2DEvent.cancel();
        }
        mCUCR2DMatch = true;
        double cur_timer = sc_time_stamp().to_double();
        mCompareMatchCUCR2DEvent.notify(CalculateClockEdge(mPCLKName, true, false, cur_timer,mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emPCLKClockIndex]),mTIMERD->mATU5->mTimeResolutionUnit);
    }
}//}}}

void ATU5_TIMERD_SUB::PreHandleCmpMatchCNT1OCR1DMethod (const unsigned int channel)
{//{{{
     mTimer_TCNT1D_OCR1D[channel]->ClearBitCMF();
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCmpMatchCNT1OCR1DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
    }
}//}}}
void ATU5_TIMERD_SUB::PreHandleCmpMatchCNT2OCR1DMethod (const unsigned int channel)
{//{{{
    mTimer_TCNT2D_OCR1D[channel]->ClearBitCMF();
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCmpMatchCNT2OCR1DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
    }
}//}}}
void ATU5_TIMERD_SUB::PreHandleCmpMatchCNT1OCR2DMethod (const unsigned int channel)
{//{{{
    mTimer_TCNT1D_OCR2D[channel]->ClearBitCMF();
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCmpMatchCNT1OCR2DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
    }
}//}}}
void ATU5_TIMERD_SUB::PreHandleCmpMatchCNT2OCR2DMethod (const unsigned int channel)
{//{{{
     mTimer_TCNT2D_OCR2D[channel]->ClearBitCMF();
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCmpMatchCNT2OCR2DEvent[channel].notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
    }
}//}}}

// Main method
void ATU5_TIMERD_SUB::HandleCmpMatchCUCR1DMethod (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
//        if(mOverflowTCNT1D || mCUCR1DMatch){ ducla revised 24/10/2019
            mCUCR1DMatch = false;
            mATU5_TIMERD_SUBFunc->TurnOVF1Flag();
            if(mInterruptOVF1Enable){
                mInterruptOutputOVF1Event.notify(SC_ZERO_TIME);
            }
//        }
       if(mOverflowTCNT1D){
            mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
       } else if(mIsTimer1Running) {
            mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(mTimer_TCNT1D_CUCR1D->GetCounterValue());
            if (mCLR1DVal) {
                StopTimerFunction (ATU5_TIMERD_SUB_AgentController::emCounter1,true);
                mCLRCounter1 = true;
                if ((mCLRSEL1DVal == false) && (mCNT1CLKPeriod > (mTIMERD->mPCLKPeriod + 1))) {
                    mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(0x0);
                }
                //start timer again
                mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
        } else {
            mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
}//}}}
void ATU5_TIMERD_SUB::HandleCmpMatchCUCR2DMethod (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double cur_timer = sc_time_stamp().to_double();
//        if(mOverflowTCNT2D || mCUCR2DMatch){ ducla revised 24/10/2019
            mATU5_TIMERD_SUBFunc->TurnOVF2Flag();
            if(mInterruptOVF2Enable){
                mInterruptOutputOVF2Event.notify(SC_ZERO_TIME);
            }
//        }
        if(mOverflowTCNT2D){
                mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
        } else if(mIsTimer2Running) {
            mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(mTimer_TCNT2D_CUCR2D->GetCounterValue());
            if (mCLR2DVal) {
                // clear counter
                StopTimerFunction (ATU5_TIMERD_SUB_AgentController::emCounter2,true);         
                mCLRCounter2 = true;
                if ((mCLRSEL2DVal == false) && (mCNT2CLKPeriod > (mTIMERD->mPCLKPeriod + 1))) {
                   mATU5_TIMERD_SUBFunc->SetTCNT2DRegVal(0x0);
                }
                //start timer again
                mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
        } else {
            mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
        }
    }
}//}}}


void ATU5_TIMERD_SUB::HandleCmpMatchCNT1OCR1DMethod (const unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (mCompareMatchCNT1Enable[channel]) {
        mCompareMatchCNT1OCR1[channel] = true;
        HandleCmpMatchCMPA(channel);
        }
    }
}//}}}
void ATU5_TIMERD_SUB::HandleCmpMatchCNT2OCR1DMethod (const unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (mCompareMatchCNT2Enable[channel]) {
        mCompareMatchCNT2OCR1[channel] = true;
        HandleCmpMatchCMPA(channel);
        }
    }
}//}}}
void ATU5_TIMERD_SUB::HandleCmpMatchCNT1OCR2DMethod (const unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (mCompareMatchCNT1Enable[channel]) {
            mCompareMatchCNT1OCR2[channel] = true;
            HandleCmpMatchCMPB(channel);
        }
    }
}//}}}
void ATU5_TIMERD_SUB::HandleCmpMatchCNT2OCR2DMethod (const unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (mCompareMatchCNT2Enable[channel]) {
        mCompareMatchCNT2OCR2[channel] = true;
        HandleCmpMatchCMPB(channel);
        }
    }
}//}}}
void ATU5_TIMERD_SUB::HandleCmpMatchCMPB (unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCompareMatchBEvent[channel].notify(); //down counter
        mATU5_TIMERD_SUBFunc->TurnCMFBFlag(channel);
        if (mCMPSEL1DVal[channel] == 0) {//ducla revise following row 258 diff
            mATU5_TIMERD_SUBFunc->SetICR1DVal(channel, mTimer_TCNT1D_CUCR1D->GetCounterValue());
        }
        else {
            mATU5_TIMERD_SUBFunc->SetICR1DVal(channel, mTimer_TCNT2D_CUCR2D->GetCounterValue());
        }

        mChangeOutputPort_TODAEvent[channel].notify();
        if(mInterruptCMPBEnable[channel]){
            mInterruptOutputCMPBEvent[channel].notify(SC_ZERO_TIME);
        }
   }
}//}}}
void ATU5_TIMERD_SUB::HandleCmpMatchCMPA (unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCompareMatchAEvent[channel].notify();//down counter
        mATU5_TIMERD_SUBFunc->TurnCMFAFlag(channel);
        if (mCMPSEL2DVal[channel] == 0) {//ducla revise following row 497 diff
            mATU5_TIMERD_SUBFunc->SetICR2DVal(channel, mTimer_TCNT2D_CUCR2D->GetCounterValue());
        }
        else {
            mATU5_TIMERD_SUBFunc->SetICR2DVal(channel, mTimer_TCNT1D_CUCR1D->GetCounterValue());
        }
        mChangeOutputPort_TODAEvent[channel].notify(SC_ZERO_TIME);
        if(mInterruptCMPAEnable[channel]){
            mInterruptOutputCMPAEvent[channel].notify(SC_ZERO_TIME);
        }
   }
}//}}}
void ATU5_TIMERD_SUB::HandleOutputPortTODAMethod (const unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (mOSSDValue[channel] == 1) {
        if(mCompareMatchCNT2OCR1[channel] || mCompareMatchCNT1OCR1[channel]) {
            //change TOD port value
            ChangeOutputPortTODA(channel,mIOADValue[channel]);
            mCompareMatchCNT2OCR1[channel] = false;
            mCompareMatchCNT1OCR1[channel] = false;
        }
        } else if (mOSSDValue[channel] == 2) {
        if(mCompareMatchCNT2OCR2[channel] || mCompareMatchCNT1OCR2[channel]) {
            //change TOD port value
            ChangeOutputPortTODA(channel,mIOBDValue[channel]);
            mCompareMatchCNT2OCR2[channel] = false;
            mCompareMatchCNT1OCR2[channel] = false;
        }
        } else if (mOSSDValue[channel] == 3) {
        if ((mCompareMatchCNT2OCR1[channel] || mCompareMatchCNT1OCR1[channel])
            && (mCompareMatchCNT2OCR2[channel] || mCompareMatchCNT1OCR2[channel])) {
            // change TOD port, CMPA/B both rise
            ChangeOutputPortTODA(channel,mIOADValue[channel]);
            mCompareMatchCNT2OCR1[channel] = false;
            mCompareMatchCNT1OCR1[channel] = false;
            mCompareMatchCNT2OCR2[channel] = false;
            mCompareMatchCNT1OCR2[channel] = false;
        } else if (mCompareMatchCNT2OCR1[channel] || mCompareMatchCNT1OCR1[channel]) {
            ChangeOutputPortTODA(channel,mIOADValue[channel]);//CMPA
            mCompareMatchCNT2OCR1[channel] = false;
            mCompareMatchCNT1OCR1[channel] = false;
        } else if (mCompareMatchCNT2OCR2[channel] || mCompareMatchCNT1OCR2[channel]) {
            ChangeOutputPortTODA(channel,mIOBDValue[channel]);//CMPB
            mCompareMatchCNT2OCR2[channel] = false;
            mCompareMatchCNT1OCR2[channel] = false;
        }
        }
    }
}//}}}
void ATU5_TIMERD_SUB::ChangeOutputPortTODA (const unsigned int channel, unsigned int port_val)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        bool port_temp = TODA[channel] -> read();
        bool write_val = false;
        bool invert_en = false;
        
        bool is_write = false;
        bool write_value;
        
        if (mGlobalInvertASelect || mChannelAInvert[channel])  {
            invert_en = true;
        } else {
            invert_en = false;
        }
        if(mOSELADValue[channel]){//output from register value
            TODA[channel] -> write(mODADValue[channel]);
            is_write = true;
            write_value = mODADValue[channel];
        } else {// normal output
            switch(port_val) {
                case 0:
                    break;
                case 1:
                    if(invert_en) { // inverted output
                        TODA[channel] -> write(true);
                        is_write = true;
                        write_value = true;
                    } else { // normal output
                        TODA[channel] -> write(false);
                        is_write = true;
                        write_value = false;
                    }
                    break;
                case 2:
                    if(invert_en) { // inverted output
                        TODA[channel] -> write(false);
                        is_write = true;
                        write_value = false;
                    } else { // normal output
                        TODA[channel] -> write(true);
                        is_write = true;
                        write_value = true;
                    }
                    break;
                case 3:
                    TODA[channel] -> write(!port_temp);
                    is_write = true;
                    write_value = !port_temp;
                    break;
                default:
                    break;
            }
        }
        if(is_write && (!mATU5_TIMERD_SUBFunc->Get_MIGSELD_MIGSEL(channel))) {
            mTODxValue[channel] = write_value;
            mTODxChange_Event[channel].notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : Input Capture Method
// Description  : Input capture
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERD_SUB::HandleInputCaptureMethod (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mTIDSELDVal =  mATU5_TIMERD_SUBFunc->GetTIDSELDVal();
        bool input_val = false;
        if(!mOBREDVal) { //OBRED:0 Enable; 1:disable
            switch(mTIDSELDVal){
                case 0:
                input_val = (bool)TAEO2A.read();
                break;
                case 1:
                input_val = (bool)TAEO2B.read();
                break;
                case 2:
                input_val = (bool)PHU0DFE.read();
                break;
                case 3:
                input_val = (bool)PHU1DFE.read();
                break;
                default:
                input_val = false;
                break;
            }
        }
        if(input_val){ //capture                 
            mATU5_TIMERD_SUBFunc->SetTCNT1DRegVal(mTimer_TCNT1D_CUCR1D->GetCounterValue());
            mCaptureValue = mTimer_TCNT1D_CUCR1D->GetCounterValue();
	        mInputCaptureEvent.notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
	    if(mTIDSELDVal==0||mTIDSELDVal==1){//ducla revise following diff line 406
		    mCaptureClockEvent.notify((double)mTIMERD->mPCLKPeriod, mTIMERD->mATU5->mTimeResolutionUnit);
	     }
        }
    }
}//}}}
/*********************************
// Function     : InputCaptureWriteMethod
// Description  : Input capture
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERD_SUB::InputCaptureWriteMethod (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mATU5_TIMERD_SUBFunc->SetOSBRDRegVal(mCaptureValue);
    }
}//}}}

/*********************************
// Function     : Range compare function1
// Description  : Range compare function1
// Parameter    : None
// Return value : None
**********************************/
void  ATU5_TIMERD_SUB::CheckRangeCompareFunction1(unsigned int counter, unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (counter == ATU5_TIMERD_SUB_AgentController::emCounter1) {
        	unsigned int UpperLimmitVal1 = 0; //ducla revise
        	if(mCLR1DVal){
        		UpperLimmitVal1=mCUCR1DValue;
        	}else{
        		UpperLimmitVal1=0xFFFFFFFF;
        	}
            if ((mOCR1DValue[channel] + (double)pow((double)2,(double)mRC1DVal) - 1) > UpperLimmitVal1){
                if ((mTimer_TCNT1D_CUCR1D->GetCounterValue()) <= ((mOCR1DValue[channel] + (double)pow((double)2,(double)mRC1DVal) - 1) - UpperLimmitVal1)
                    ||((mTimer_TCNT1D_CUCR1D->GetCounterValue()>= mOCR1DValue[channel])&&(mTimer_TCNT1D_CUCR1D->GetCounterValue()<=UpperLimmitVal1))) {
                // compare match A //ducla revise
                    mCompareMatchCNT1OCR1[channel] = true;
                    HandleCmpMatchCMPA(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR1DVal(channel,mTimer_TCNT1D_CUCR1D->GetCounterValue());

                }
            } else {
            if (((mTimer_TCNT1D_CUCR1D->GetCounterValue()) <= (mOCR1DValue[channel] + (double)pow((double)2,(double)mRC1DVal) - 1)) &&
                (mTimer_TCNT1D_CUCR1D->GetCounterValue() >= mOCR1DValue[channel])) {
                // compare match A
                    mCompareMatchCNT1OCR1[channel] = true;
                    HandleCmpMatchCMPA(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR1DVal(channel,mTimer_TCNT1D_CUCR1D->GetCounterValue());
                }
            }
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emCounter2) {
        	unsigned int UpperLimmitVal2 = 0;
        	if(mCLR2DVal){
        	      UpperLimmitVal2=mCUCR2DValue;
        	 } else {
        	      UpperLimmitVal2=0xFFFFFFFF;
        	 }
            if ((mOCR1DValue[channel] + (double)pow((double)2,(double)mRC2DVal) - 1 ) > UpperLimmitVal2) {
                if ((mTimer_TCNT2D_CUCR2D->GetCounterValue()) <= ((mOCR1DValue[channel] + (double)pow((double)2,(double)mRC2DVal) - 1) - UpperLimmitVal2)
                    ||((mTimer_TCNT2D_CUCR2D->GetCounterValue()>= mOCR1DValue[channel])&&(mTimer_TCNT2D_CUCR2D->GetCounterValue()<=UpperLimmitVal2))) {
                // compare match B
                    mCompareMatchCNT2OCR1[channel] = true;
                    HandleCmpMatchCMPA(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR1DVal(channel,mTimer_TCNT2D_CUCR2D->GetCounterValue());
                }
            } else {
            if (((mTimer_TCNT2D_CUCR2D->GetCounterValue()) <= (mOCR1DValue[channel] + (double)pow((double)2,(double)mRC2DVal) - 1)) &&
                (mTimer_TCNT2D_CUCR2D->GetCounterValue() >= mOCR1DValue[channel])) {
                // compare match B
                    mCompareMatchCNT2OCR1[channel] = true;
                    HandleCmpMatchCMPA(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR1DVal(channel,mTimer_TCNT2D_CUCR2D->GetCounterValue());
                }
            }
        }
    }
 }//}}}

/*********************************
// Function     : Range compare function2
// Description  : Range compare function2
// Parameter    : None
// Return value : None
**********************************/
void  ATU5_TIMERD_SUB::CheckRangeCompareFunction2(unsigned int counter, unsigned int channel)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if (counter == ATU5_TIMERD_SUB_AgentController::emCounter1) {
        	unsigned int UpperLimmitVal1 = 0;
        	if(mCLR1DVal){
        		UpperLimmitVal1=mCUCR1DValue;
        	}else{
        		UpperLimmitVal1=0xFFFFFFFF;
        	}
            if ((mOCR2DValue[channel] + (double)pow((double)2,(double)mRC1DVal) - 1) > UpperLimmitVal1) {
                if ((mTimer_TCNT1D_CUCR1D->GetCounterValue()) <= ((mOCR2DValue[channel] + (double)pow((double)2,(double)mRC1DVal) - 1) - UpperLimmitVal1)
                    ||((mTimer_TCNT1D_CUCR1D->GetCounterValue()>= mOCR2DValue[channel])&&(mTimer_TCNT1D_CUCR1D->GetCounterValue()<=UpperLimmitVal1))) {
                // compare match A
                    mCompareMatchCNT1OCR2[channel] = true;
                    HandleCmpMatchCMPB(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR2DVal(channel,mTimer_TCNT1D_CUCR1D->GetCounterValue());

                }
            } else {
            if (((mTimer_TCNT1D_CUCR1D->GetCounterValue()) <= (mOCR2DValue[channel] + (double)pow((double)2,(double)mRC1DVal) - 1)) &&
                (mTimer_TCNT1D_CUCR1D->GetCounterValue() >= mOCR2DValue[channel])) {
                // compare match A
                    mCompareMatchCNT1OCR2[channel] = true;
                    HandleCmpMatchCMPB(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR2DVal(channel,mTimer_TCNT1D_CUCR1D->GetCounterValue());
                }
            }
        } else if (counter == ATU5_TIMERD_SUB_AgentController::emCounter2) {
           	unsigned int UpperLimmitVal2 = 0;
            if(mCLR2DVal){
            	   UpperLimmitVal2=mCUCR2DValue;
            } else {
            	   UpperLimmitVal2=0xFFFFFFFF;
            }
            if ((mOCR2DValue[channel] + (double)pow((double)2,(double)mRC2DVal) - 1 ) > UpperLimmitVal2) {
                if ((mTimer_TCNT2D_CUCR2D->GetCounterValue()) <= ((mOCR2DValue[channel] + (double)pow((double)2,(double)mRC2DVal) - 1) - UpperLimmitVal2)
                    ||((mTimer_TCNT2D_CUCR2D->GetCounterValue()>= mOCR2DValue[channel])&&(mTimer_TCNT2D_CUCR2D->GetCounterValue()<=UpperLimmitVal2))) {
                // compare match B
                    mCompareMatchCNT2OCR2[channel] = true;
                    HandleCmpMatchCMPB(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR2DVal(channel,mTimer_TCNT2D_CUCR2D->GetCounterValue());
                }
            } else {
            if (((mTimer_TCNT2D_CUCR2D->GetCounterValue()) <= (mOCR2DValue[channel] + (double)pow((double)2,(double)mRC2DVal) - 1)) &&
                (mTimer_TCNT2D_CUCR2D->GetCounterValue() >= mOCR2DValue[channel])) {
                // compare match B
                    mCompareMatchCNT2OCR2[channel] = true;
                    HandleCmpMatchCMPB(channel);
                    mATU5_TIMERD_SUBFunc->SetOCR2DVal(channel,mTimer_TCNT2D_CUCR2D->GetCounterValue());
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : HandleInputClearCounterMethod
// Description  : Input clear request
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERD_SUB::HandleInputClearCounterMethod (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        bool input_val = (bool)CLRB.read();
        double cur_timer = sc_time_stamp().to_double();
        if(input_val && mC1CEDVal){
            if (mIsClearTCNT1D) {
                mCLRBTCNT1D = true;
                mIsClearTCNT1D=false;
                
                if (mIsTimer1Running) {
                    StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1, false);
                    mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer, mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]), mTIMERD->mATU5->mTimeResolutionUnit);
                }
            }else{
                mClearTCNT1D = true;
                mProcessClearTCNT1DEvent.notify(SC_ZERO_TIME);
            }
        }

        if(input_val && mC2CEDVal){
            if (mIsClearTCNT2D) {
                mCLRBTCNT2D = true;
                mIsClearTCNT2D=false;
                if (mIsTimer2Running) {
                    StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2, false);
                    mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false, cur_timer, mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]), mTIMERD->mATU5->mTimeResolutionUnit);	
                }
            }else{
                mClearTCNT2D = true;
                mProcessClearTCNT2DEvent.notify(SC_ZERO_TIME);
            }
        }
    }
}//}}}

/*********************************
// Function     : ClockHandleFunction
// Description  : Input clear request
// Parameter    : None
// Return value : None
**********************************/
void ATU5_TIMERD_SUB::ClockHandleFunction (void)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif"))){
        sc_dt::uint64 PreCNT1CLKPeriod = 0;
        sc_dt::uint64 PreCNT2CLKPeriod = 0;
        sc_dt::uint64 PreDCNTCLKPeriod = 0;
        double cur_timer = sc_time_stamp().to_double();
        switch(mCKSEL1DVal){
         case 0:
           mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS0ClockIndex];
           PreCNT1CLKPeriod = mTIMERD->mCLKBUS0Period;
           break;
         case 1:
           mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS1ClockIndex];
           PreCNT1CLKPeriod = mTIMERD->mCLKBUS1Period;
           break;
         case 2:
           mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS2ClockIndex];
           PreCNT1CLKPeriod = mTIMERD->mCLKBUS2Period;
           break;
         case 3:
           mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS3ClockIndex];
           PreCNT1CLKPeriod = mTIMERD->mCLKBUS3Period;
           break;
         case 4:
           mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS4ClockIndex];
           PreCNT1CLKPeriod = mTIMERD->mCLKBUS4Period;
           break;
         case 5:
           mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS5ClockIndex];
           PreCNT1CLKPeriod = mTIMERD->mCLKBUS5Period;
           break;
         default:
           mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS0ClockIndex];
           PreCNT1CLKPeriod = mTIMERD->mCLKBUS0Period;
           break;
        }
        switch(mCKSEL2DVal){
          case 0:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS0ClockIndex];
            PreCNT2CLKPeriod = mTIMERD->mCLKBUS0Period;
            break;
          case 1:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS1ClockIndex];
            PreCNT2CLKPeriod = mTIMERD->mCLKBUS1Period;
            break;
          case 2:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS2ClockIndex];
            PreCNT2CLKPeriod = mTIMERD->mCLKBUS2Period;
            break;
          case 3:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS3ClockIndex];
            PreCNT2CLKPeriod = mTIMERD->mCLKBUS3Period;
            break;
          case 4:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS4ClockIndex];
            PreCNT2CLKPeriod = mTIMERD->mCLKBUS4Period;
            break;
          case 5:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS5ClockIndex];
            PreCNT2CLKPeriod = mTIMERD->mCLKBUS5Period;
            break;
          default:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS0ClockIndex];
            PreCNT2CLKPeriod = mTIMERD->mCLKBUS0Period;
            break;
        }
        switch(mDCSELDVal){
          case 0:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS0ClockIndex];
            PreDCNTCLKPeriod = mTIMERD->mCLKBUS0Period;
            break;
          case 1:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS1ClockIndex];
            PreDCNTCLKPeriod = mTIMERD->mCLKBUS1Period;
            break;
          case 2:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS2ClockIndex];
            PreDCNTCLKPeriod = mTIMERD->mCLKBUS2Period;
            break;
          case 3:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS3ClockIndex];
            PreDCNTCLKPeriod = mTIMERD->mCLKBUS3Period;
            break;
          case 4:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS4ClockIndex];
            PreDCNTCLKPeriod = mTIMERD->mCLKBUS4Period;
            break;
          case 5:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS5ClockIndex];
            PreDCNTCLKPeriod = mTIMERD->mCLKBUS5Period;
            break;
          default:
            mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter] = mTIMERD->mInputClockStartPoint[ATU5_TIMERD_AgentController::emCLKBUS0ClockIndex];
            PreDCNTCLKPeriod = mTIMERD->mCLKBUS0Period;
            break;
        }
        if(PreCNT1CLKPeriod==0){ //zero clock condition
            mCNT1CLKPeriod = PreCNT1CLKPeriod;
            //stop counter
            if (mIsTimer1Running) {
                StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1,true);
            }
            if (mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter1]){
                mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter1] = false;
            }
        } else if (mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter1]){//count clock selection change
            mCNT1CLKPeriod = PreCNT1CLKPeriod;
            mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter1] = false;
            if (mIsTimer1Running) {
                StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1,true);
                mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
            } else if (mCounterEnable) {
                mStartTimer1Event.cancel();
                mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
        } else if (PreCNT1CLKPeriod != mCNT1CLKPeriod){
            mCNT1CLKPeriod = PreCNT1CLKPeriod;
            if (mIsTimer1Running) {
                StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter1,true);
                mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
            } else if (mCounterEnable) {
                mStartTimer1Event.cancel();
                mStartTimer1Event.notify(CalculateClockEdge(mCNT1CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter1]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
        }
       //counter 2
       if(PreCNT2CLKPeriod==0){ //zero clock condition
            mCNT2CLKPeriod = PreCNT2CLKPeriod;
            //stop counter
            if (mIsTimer2Running) {
                StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2,true);
            }
            if (mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter2]){
                mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter2] = false;
            }
        } else if (mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter2]){//count clock selection change
            mCNT2CLKPeriod = PreCNT2CLKPeriod;
            mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emCounter2] = false;
            if (mIsTimer2Running) {
                StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2,true);
                mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
            } else if (mCounterEnable) {
                mStartTimer2Event.cancel();
                mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
        } else if (PreCNT2CLKPeriod != mCNT2CLKPeriod){
            mCNT2CLKPeriod = PreCNT2CLKPeriod;
            if (mIsTimer2Running) {
                StopTimerFunction(ATU5_TIMERD_SUB_AgentController::emCounter2,true);
                mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
            } else if (mCounterEnable) {
                mStartTimer2Event.cancel();
                mStartTimer2Event.notify(CalculateClockEdge(mCNT2CLKName, true, false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emCounter2]),mTIMERD->mATU5->mTimeResolutionUnit);
            }
        }
        //Down Counter
       if(PreDCNTCLKPeriod==0){ //zero clock condition
            mDCNTCLKPeriod = PreDCNTCLKPeriod;
            //stop counter
            for(unsigned int ch_id=0;ch_id <=3; ch_id++) {
                if(mIsDcntRunning[ch_id]){
                    mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter] = true;
                    mIsDcntWasRunning[ch_id] = true;
                    StopDcntMethod(ch_id);
                }
            }
            if (mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter]){
                mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter] = false;
            }
        } else if (mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter]){//count clock selection change
            mDCNTCLKPeriod = PreDCNTCLKPeriod;
            for(unsigned int ch_id=0;ch_id <=3; ch_id++) {
                if(mIsDcntRunning[ch_id]){
                    mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter] = true;
                    StopDcntMethod(ch_id);
                    mStartDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                }
            }
            mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter] = false;
        } else if (PreDCNTCLKPeriod != mDCNTCLKPeriod){
            mDCNTCLKPeriod = PreDCNTCLKPeriod;
            for(unsigned int ch_id=0;ch_id <=3; ch_id++) {
                if(mIsDcntRunning[ch_id]){
                    mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter] = true;
                    StopDcntMethod(ch_id);
                    mStartDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                }
                else if(mIsDcntWasRunning[ch_id]) {
                    mStartDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                    mIsDcntWasRunning[ch_id] = false;
                }
            }
            if (mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter]){
                mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter] = false;
            }
        }
    }
}//}}}
/***********************************
Function        : EnableReset
Description     : Reset is called
***********************************/
void ATU5_TIMERD_SUB::EnableReset(const bool is_active)
{//{{{
    mATU5_TIMERD_SUBFunc->EnableReset(is_active);
    mTimer_TCNT1D_CUCR1D->EnableReset(is_active);
    mTimer_TCNT2D_CUCR2D->EnableReset(is_active);
    if(is_active){
        Initialize();
        InitializePort();
        CancelAllEvents();
    }
    for (unsigned int channel = 0; channel < 4; channel++) {
        mTimer_TCNT1D_OCR1D[channel]->EnableReset(is_active);
        mTimer_TCNT1D_OCR2D[channel]->EnableReset(is_active);
        mTimer_TCNT2D_OCR1D[channel]->EnableReset(is_active);
        mTimer_TCNT2D_OCR2D[channel]->EnableReset(is_active);
        mTimer_DCNTD[channel]->EnableReset(is_active);
        mTimer_OFMICNTD[channel]->EnableReset(is_active);;
        mTimer_ONMICNTD[channel]->EnableReset(is_active);;
        mTimer_OTONMICNTD[channel]->EnableReset(is_active);;
    }
}//}}}

//////////////////////////////////////////////////////////////////////
///*********************************
/// Function     : WriteTODBMethod
/// Description  : WriteTODBMethod
///*********************************
void ATU5_TIMERD_SUB::WriteTODBMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mATU5_TIMERD_SUBFunc->Get_OSELRD_OSELBD(ch_id)) {
            TODB[ch_id]->write(mATU5_TIMERD_SUBFunc->Get_ODRD_ODBD(ch_id));
        }
        else {
            if(mATU5_TIMERD_SUBFunc->Get_TOCRD_TONEBD(ch_id) || mATU5_TIMERD_SUBFunc->Get_TOCCRD_TONEBD(ch_id)) {
                    TODB[ch_id]->write(!mTODB_normal[ch_id]);
                    if(mATU5_TIMERD_SUBFunc->Get_MIGSELD_MIGSEL(ch_id)) {//revise by ducla 06/07/2018
                        mTODxValue[ch_id] = !mTODB_normal[ch_id];
                        mTODxChange_Event[ch_id].notify(SC_ZERO_TIME);
                    }
            }
            else {
                    TODB[ch_id]->write(mTODB_normal[ch_id]);
                    if(mATU5_TIMERD_SUBFunc->Get_MIGSELD_MIGSEL(ch_id)) {//revise by ducla 06/07/2018
                        mTODxValue[ch_id] = mTODB_normal[ch_id];
                        mTODxChange_Event[ch_id].notify(SC_ZERO_TIME);
                    }
            }
        }
    }
}//}}}

///*********************************
/// Function     : ProcessMIGMethod
/// Description  : ProcessMIGMethod
///*********************************
void ATU5_TIMERD_SUB::ProcessMIGMethod (const unsigned int ch_id)
{//{{{
    if (!mATU5_TIMERD_SUBFunc->Get_MIGCRD_MIGEND(ch_id)) { // not enable MIN GUARD
        mTODMIGValue[ch_id] = mTODxValue[ch_id];
        mTriggerTODMIG_Event[ch_id].notify(SC_ZERO_TIME);
    } else { // enable MIN GUARD
        double current_time = sc_time_stamp().to_double();
        if (mTODxValue[ch_id]) { // Assert
            if (mIsOFMICNTZero(ch_id) && mIsOTONMICNTZero(ch_id)) { // match requirement
                mTODMIGValue[ch_id] = true;
                mATU5_TIMERD_SUBFunc->Set_TSR2D_REFD(ch_id, 1);  //ducla add
                mTriggerTODMIG_Event[ch_id].notify(SC_ZERO_TIME);
            } else {
                mIsRaisingRequest[ch_id] =  true;
                mIsFailingRequest[ch_id] =  false; // cancel failing
            }
        } else {  // Deassert
            if (mIsONMICNTZero(ch_id)) { // match requirement
                mTODMIGValue[ch_id] = false;
                mATU5_TIMERD_SUBFunc->Set_TSR2D_FEFD(ch_id, 1); //ducla add
                mTriggerTODMIG_Event[ch_id].notify(SC_ZERO_TIME);
            } else {
                mIsFailingRequest[ch_id] = true;
                mIsRaisingRequest[ch_id] = false; // cancel raising
            }
        }
    }
}//}}}

///*********************************
/// Function     : WriteTODMIGMethod
/// Description  : WriteTODMIGMethod
///*********************************
void ATU5_TIMERD_SUB::WriteTODMIGMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mATU5_TIMERD_SUBFunc->Get_OSELRD_OSELBD(ch_id)) {
            if(!(mATU5_TIMERD_SUBFunc->Get_MIGSELD_MIGSEL(ch_id))) {// ducla change mATU5_TIMERD_SUBFunc->Get_MIGSELD_MIGSEL(ch_id) ->!(mATU5_TIMERD_SUBFunc->Get_MIGSELD_MIGSEL(ch_id))
                TODMIG[ch_id]->write(mOSELADValue[ch_id]);
            } else {
                TODMIG[ch_id]->write(mATU5_TIMERD_SUBFunc->Get_ODRD_ODBD(ch_id));
            }
        } else {
            TODMIG[ch_id]->write(mTODMIGValue[ch_id]);
            if (mATU5_TIMERD_SUBFunc->Get_MIGCRD_MIGEND(ch_id)) { // enable MIN GUARD
                if (mTODMIGValue[ch_id]) { // Assert
                    //start counters MIGs
                    mStartONMICNTDEvent[ch_id].notify(SC_ZERO_TIME);
                    mStartOTOMICNTDEvent[ch_id].notify(SC_ZERO_TIME);
                    //capture
                    mATU5_TIMERD_SUBFunc->Set_CAPD("ONCAP1D", ch_id);
                    mATU5_TIMERD_SUBFunc->Set_CAPD("ONCAP2D", ch_id);
                    //interrupt
                    if(mInterruptREIEDEnable[ch_id]){
                        mInterruptOutputREIEDEvent[ch_id].notify(SC_ZERO_TIME);
                    }
                } else { // Deassert
                    //start counters MIGs
                    mStartOFMICNTDEvent[ch_id].notify(SC_ZERO_TIME);
                    //capture
                    mATU5_TIMERD_SUBFunc->Set_CAPD("OFCAP1D", ch_id);
                    mATU5_TIMERD_SUBFunc->Set_CAPD("OFCAP2D", ch_id);
                    //interrupt
                    if(mInterruptFEIEDEnable[ch_id]){
                        mInterruptOutputFEIEDEvent[ch_id].notify(SC_ZERO_TIME);
                    }
                }
            }
        }
    }
}//}}}

///*********************************
/// Function     : DeassertTODB_normalMethod
/// Description  : DeassertTODB_normal
///*********************************
void ATU5_TIMERD_SUB::DeassertTODB_normalMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(!mATU5_TIMERD_SUBFunc->Get_MIGCRD_MIGEND(ch_id)) { //normal
            if(mInterruptFEIEDEnable[ch_id]){
                mInterruptOutputFEIEDEvent[ch_id].notify(SC_ZERO_TIME);
            }
        }
        mTODB_normal[ch_id] = false;

    }
}//}}}

///*********************************
/// Function     : AssertTODB_normalMethod
/// Description  : AssertTODB_normal
///*********************************
void ATU5_TIMERD_SUB::AssertTODB_normalMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(!mATU5_TIMERD_SUBFunc->Get_MIGCRD_MIGEND(ch_id)) { //normal
            if(mInterruptREIEDEnable[ch_id]){
                mInterruptOutputREIEDEvent[ch_id].notify(SC_ZERO_TIME);
            }
        }
        mTODB_normal[ch_id] = true;

    }
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables
///*********************************
void ATU5_TIMERD_SUB::Initialize (void)
{//{{{
    // Initialize variables
   mCUCR1DMatch = false;
   mCUCR2DMatch = false;
   mCUCR1DValue = 0xFFFFFFFF;
   mCUCR2DValue = 0xFFFFFFFF;
   mCLRCounter1 = false;
   mCLRCounter2 = false;
   mOverflowTCNT1D = false;
   mOverflowTCNT2D = false;
   mCLRBTCNT1D = false;
   mCLRBTCNT2D = false;
   mInitialStage = true;
   IsTimerRunning = false;
   mIsTimer1Running= false;
   mIsTimer2Running= false;
   mCLR1DVal = false ; //clear enable by CUCR1D (CLR1D bit)
   mCLR2DVal = false ; //clear enable by CUCR1D (CLR2D bit)
   mOBREDVal = false; //OBRED enable input capture TCNT1D to OSBRD
   mTIDSELDVal = 0; // select input trigger signal
   mC2CEDVal = false; //enable CLRB clear counter 2
   mC1CEDVal = false; //enable CLRB clear counter 1
   mCKSEL1DVal = 0; //CKSEL1D, select clock lines
   mCKSEL2DVal = 0; //CKSEL2D, select clock lines
   mDCSELDVal = 0; //DCSELD, select clock lines for down counter
   mCLRSEL1DVal = false; //clear sync with PCLK/CLKBUS (CLRSEL1D)
   mCLRSEL2DVal = false; //clear sync with PCLK/CLKBUS (CLRSEL2D)
   mInterruptOVF1Enable = false; //OV1ED
   mInterruptOVF2Enable = false; //OV2ED
   mRC1DVal = 0;
   mRC2DVal = 0;
   mCaptureValue = 0;
   mGlobalInvertASelect = false;//TOCRD_TONEAD
   mSetDSFD = false;//ducla add
   mClearTCNT1D = false;
   mClearTCNT2D = false;
   mIsClearTCNT1D = false;//ducla add
   mIsClearTCNT2D = false;//ducla add
   mSetTCNT1D = false;//ducla add
   mSetTCNT2D = false;//ducla add

   mCounterEnable = false;
   mCUCR1DOvfTime = 0;
   mCUCR2DOvfTime = 0;
   mIsCUCR1DOvf = false;
   mIsCUCR2DOvf = false;
  
   for (unsigned int index = 0; index <= 3; index++) {
       mIsTimerOFMICNTDRunning[index] = false;
       mIsTimerONMICNTDRunning[index] = false;
       mIsTimerOTONMICNTDRunning[index] = false;
       mIsDCNTUpdated[index] = false;
       mOCR1DValue[index] = 0xFFFFFFFF;
       mOCR2DValue[index] = 0xFFFFFFFF;

       mCMPARequest[index] = false;
       mCMPBRequest[index] = false;

       mCMPSEL1DVal[index] = false;
       mCMPSEL2DVal[index] = false;

       mInterruptCMPAEnable[index] = false;
       mInterruptCMPBEnable[index] = false;

       mCompareMatchCNT1Enable[index] = false;//CMPE1D
       mCompareMatchCNT2Enable[index] = false;//CMPE2D

       mOSELADValue[index] = false;
       mODADValue[index] = false;

       mOSSDValue[index] = 0;
       mIOADValue[index] = 0;
       mIOBDValue[index] = 0;

       mChannelAInvert[index] = false; //TOCCRD_TONEADx

       mCompareMatchCNT1OCR1[index]=false;
       mCompareMatchCNT2OCR1[index]=false;
       mCompareMatchCNT1OCR2[index]=false;
       mCompareMatchCNT2OCR2[index]=false;

       mIsDcntinUdfPeriod[index] = false;
       if (index < 3) {
           mChangeCountClock[index] = false;
           mCountClockStartPoint[index] = 0;
       }
       
       mInterruptUNDEDEnable[index] = false;
       mInterruptFEIEDEnable[index] = false;
       mInterruptREIEDEnable[index] = false;

       //add by ducla
       mIsSetDSFD[index] = false;
       mIsClearDSFD[index] = false;
    }
    mDCNTCLKPeriod = 0;
    mCNT1CLKPeriod = 0;
    mCNT2CLKPeriod = 0;
    mDCNTCLKName = "DTCNTCLK";
    mCNT1CLKName = "TCNT1CLK";
    mCNT2CLKName = "TCNT2CLK";
    mPCLKName = "PCLK";

    for(unsigned int ch_id=0;ch_id<ATU5_TIMERD_SUB_AgentController::emChannel;ch_id++) {
        mIsCMA[ch_id] = false;
        mIsCMB[ch_id] = false;
        mIsUNF[ch_id] = false;
        mIsDcntRunning[ch_id] = false;
        mIsDcntWasRunning[ch_id] = false;

        //TODxyB variables
        mTODB_normal[ch_id] = false;
        mTODB_type2[ch_id] = false;

        mIsRaisingRequest[ch_id] = false;
        mIsFailingRequest[ch_id] = false;
        mTODxValue[ch_id] = false;
        mTODMIGValue[ch_id] = false;
    }
    mStartDcntWithZeroClk = false; // chuongle add 11/22/2018
    mTimeToNotifyStartDcntEvent = 0; // chuongle add 11/22/2018
}//}}}

void ATU5_TIMERD_SUB::InitializePort (void)
{//{{{
    INT_OVF1.write(false);
    INT_OVF2.write(false);
    for(unsigned int ch_id = 0; ch_id<ATU5_TIMERD_SUB_AgentController::emChannel; ch_id++) {
        //Initialize ports
        TODB[ch_id]->write(false);
        TODA[ch_id]->write(false);
        TODMIG[ch_id]->write(false);
        INT_UDF[ch_id]->write(false);
        INT_CMPA[ch_id]->write(false);
        INT_CMPB[ch_id]->write(false);
        INT_FEIED[ch_id]->write(false);
        INT_REIED[ch_id]->write(false);
    }
}//}}}
void ATU5_TIMERD_SUB::CancelAllEvents (void)
{//{{{
    mStartTimerEvent.cancel();
    mStartTimer1Event.cancel();
    mStartTimer2Event.cancel();
    mCompareMatchCUCR1DEvent.cancel();
    mCompareMatchCUCR2DEvent.cancel();
    mInterruptOutputOVF1Event.cancel();
    mInterruptOutputOVF2Event.cancel();
    mClearInterrupt1Event.cancel();
    mClearInterrupt2Event.cancel();
    mInputCaptureEvent.cancel();
    mCheckCompareMatchCUCR1DEvent.cancel();
    mCheckCompareMatchCUCR2DEvent.cancel();

    mProcessWriteTCNT1DEvent.cancel();
    mProcessWriteTCNT2DEvent.cancel();
    mProcessClearTCNT1DEvent.cancel();
    mProcessClearTCNT2DEvent.cancel();
    mCaptureClockEvent.cancel();

    for (unsigned int index = 0; index < ATU5_TIMERD_SUB_AgentController::emChannel; index++) {
        mTODxChange_Event[index].cancel();
        mTriggerTODMIG_Event[index].cancel();
        mStartOFMICNTDEvent[index].cancel();
        mStartONMICNTDEvent[index].cancel();
        mStartOTOMICNTDEvent[index].cancel();
        mUpdateDCNTDEvent[index].cancel();
        mCmpMatchCNT1OCR1DEvent[index].cancel();
        mCmpMatchCNT2OCR1DEvent[index].cancel();
        mCmpMatchCNT1OCR2DEvent[index].cancel();
        mCmpMatchCNT2OCR2DEvent[index].cancel();
        mInterruptOutputCMPAEvent[index].cancel();
        mInterruptOutputCMPBEvent[index].cancel();
        mInterruptOutputFEIEDEvent[index].cancel();
        mInterruptOutputREIEDEvent[index].cancel();
        mClearInterruptOutputCMPAEvent[index].cancel();
        mClearInterruptOutputCMPBEvent[index].cancel();
        mClearInterruptOutputFEIEDEvent[index].cancel();
        mClearInterruptOutputREIEDEvent[index].cancel();
        mChangeOutputPort_TODAEvent[index].cancel();
        mAssertTODB_normalEvent[index].cancel();
        mStartDCNTDEvent[index].cancel();
        mDeassertTODB_normalEvent[index].cancel();
	    mInvertedRegIsWriteEvent[index].cancel();
        mCompareMatchAEvent[index].cancel();
 	    mCompareMatchBEvent[index].cancel();
 	    mStartDCNTTriggerEvent[index].cancel();
	    mStartDcntEvent[index].cancel();
	    mStopDcntEvent[index].cancel();
	    mClearDSR1D_DSFDEvent[index].cancel();
        mAssertINT_UDFEvent[index].cancel();
        mDeassertINT_UDFEvent[index].cancel();
		mCompareMatchAEventDly[index].cancel();
        mCompareMatchBEventDly[index].cancel();
        mProcessClearingEvent[index].cancel();//ducla add
        mProcessSettingEvent[index].cancel();//ducla add
    }
     for (unsigned int index = 0; index < ATU5_TIMERD_SUB_AgentController::emDownCounter; index++) {
       mWriteCompareMatchValue[index].cancel();
       mWriteCounterValue[index].cancel();
     }
}//}}}
///*********************************
/// Function     : StartDCNTDReg
/// Description  : Start DCNTD
///*********************************
void ATU5_TIMERD_SUB::StartDCNTDReg (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        double current_time = sc_time_stamp().to_double();
        mStartDCNTTriggerEvent[ch_id].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : ClearDSR1D_DSFDMethod
/// Description  : Start DCNTD
///*********************************
void ATU5_TIMERD_SUB::ClearDSR1D_DSFDMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,0);
        mIsClearDSFD[ch_id] = true;
        mProcessClearingEvent[ch_id].notify(SC_ZERO_TIME);
    }
}//}}}


///*********************************
/// Function     : UpdateDCNTDMethod
/// Description  : Update DCNTD
///*********************************
void ATU5_TIMERD_SUB::UpdateDCNTDMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mIsDcntinUdfPeriod[ch_id]){
            mStopDcntEvent[ch_id].cancel();
        }
        mIsDCNTUpdated[ch_id] = true;
        mTimer_DCNTD[ch_id]->StartStop(false);
        unsigned int counter_value = mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id) - 1;
        unsigned int cm_value = 0xFFFFFFFF;
        double       period = (double)mDCNTCLKPeriod;
        //setup timer
        mTimer_DCNTD[ch_id]->SetupGeneralTimer(false, false);             //set count down mode
        mTimer_DCNTD[ch_id]->SetCounterMode(true);                              //Free-running mode
        mTimer_DCNTD[ch_id]->SetTimerSize(32);
        mTimer_DCNTD[ch_id]->SetCounterInitialValue(counter_value);        //set counter register
        mTimer_DCNTD[ch_id]->SetCompareMatchValue(cm_value);                    // set compare match register
        mTimer_DCNTD[ch_id]->SetCounterPeriod(period);                          //set clock count period
        //start timer
        double cur_timer = sc_time_stamp().to_double();
        mStartDCNTDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,false, cur_timer,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
        /////
    }
}//}}}

///*********************************
/// Function     : StartDCNTDMethod
/// Description  : Start DCNTD
///*********************************
void ATU5_TIMERD_SUB::StartDCNTDMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(mIsDcntinUdfPeriod[ch_id]){
            mIsDcntinUdfPeriod[ch_id] = false;
            mIsDcntRunning[ch_id] = true;
        }
        mIsDCNTUpdated[ch_id] = false;
        mTimer_DCNTD[ch_id]->StartStop(true);
    }
}//}}}

///*********************************
/// Function     : StartDcntMethod
/// Description  : Start DCNTD
///*********************************
void ATU5_TIMERD_SUB::StartDcntMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsDcntRunning[ch_id] = true;
        unsigned int counter_value;
        if (mStartDcntWithZeroClk) {
            mStartDcntWithZeroClk = false;
            counter_value = mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id); // when clock is zero, down counter does not counts down
        } else {
            counter_value = mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id) - 1;
        }

        unsigned int cm_value = 0xFFFFFFFF;
        double       period = (double)mDCNTCLKPeriod;

        //setup timer
        mTimer_DCNTD[ch_id]->SetupGeneralTimer(false, false);             //set count down mode
        mTimer_DCNTD[ch_id]->SetCounterMode(true);                              //Free-running mode
        mTimer_DCNTD[ch_id]->SetTimerSize(32);
        mTimer_DCNTD[ch_id]->SetCounterInitialValue(counter_value);        //set counter register
        mTimer_DCNTD[ch_id]->SetCompareMatchValue(cm_value);                    // set compare match register
        mTimer_DCNTD[ch_id]->SetCounterPeriod(period);                          //set clock count period

        // If period is zero, then stop the down counter & wait for the next non-zero period
        if (period == 0) {
            ClockHandleFunction(); // chuongle added 10/30/2018
        } else {
            //start timer
            mTimer_DCNTD[ch_id]->StartStop(true);
        }

        ////TODxyB driver
        mAssertTODB_normalEvent[ch_id].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : StopDcntMethod
/// Description  : Stop DCNTD
///*********************************
void ATU5_TIMERD_SUB::StopDcntMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsDcntRunning[ch_id] = false;
        //Stop timer
        mTimer_DCNTD[ch_id]->StartStop(false);
        /////
        if(mChangeCountClock[ATU5_TIMERD_SUB_AgentController::emDownCounter]){
            mATU5_TIMERD_SUBFunc->Set_DCNTD(ch_id,GetCounterValue(ATU5_TIMERD_SUB_AgentController::emDownCounter,ch_id));
        }
        else {
            mATU5_TIMERD_SUBFunc->Set_DCNTD(ch_id,0);
        }

        ///TODxyB driver

        // When the down counter is stopped because of zero clock,
        // then TODB interrupt is not de-asserted
        if (!mIsDcntWasRunning[ch_id]) {
            mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,0);
            mDeassertTODB_normalEvent[ch_id].notify(SC_ZERO_TIME);
        }

        if(mIsDcntinUdfPeriod[ch_id]) {
            mATU5_TIMERD_SUBFunc->Set_TSRD_UDFD(ch_id,1);
            if(mInterruptUNDEDEnable[ch_id]){
                mAssertINT_UDFEvent[ch_id]. notify(SC_ZERO_TIME);
            }
            mIsDcntinUdfPeriod[ch_id] = false;
        }
    }
}//}}}

///*********************************
/// Function     : TriggerDCNTMethod
/// Description  : Trigger Start or Stop DCNTD
///*********************************
void ATU5_TIMERD_SUB::TriggerDCNTMethod (const unsigned int ch_id)
{//{{{
    mTimer_DCNTD[ch_id]->ClearBitUNF();
    double current_time = sc_time_stamp().to_double();
    if (IsTimerRunning && !(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        if(!mIsDcntRunning[ch_id]) {

            //-----------------------------------------------------------------------------
            // Clock bus 5, i.e. AGCKM after fixing #58712 is often zero, so pulse width
            // of TODB is only (DCNTDxy - 1). Meanwhile, expected width of TODB pulse is
            // DCNTDxy, i.e. there are DCNTDxy down counter clock pulses within TODB pulse
            // Countermeasure: when mDCNTCLKPeriod is zero:
            // - Turns ON mStartDcntWithZeroClk to inform a non-count-down for the 1st
            //   count clock to lengthen the TODB by 1 count
            // - Set 0 to mTimeToNotifyStartDcntEvent to prevent USK W544 warning
            //-----------------------------------------------------------------------------
            if (mDCNTCLKPeriod == 0) {
                mStartDcntWithZeroClk = true;
                mTimeToNotifyStartDcntEvent = 0;
            } else {
                mTimeToNotifyStartDcntEvent = CalculateClockEdge(mDCNTCLKName, true,true, current_time, mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]);
            }

            if(mATU5_TIMERD_SUBFunc->GetDcntStart(ch_id)){
                mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,1);
                if(mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id)!=0) {
                    mStartDcntEvent[ch_id].notify(mTimeToNotifyStartDcntEvent, mTIMERD->mATU5->mTimeResolutionUnit);
                }else{
                    mClearDSR1D_DSFDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                }
                mIsCMA[ch_id] = false;
                mIsCMB[ch_id] = false;
            }
            else{
                switch(mATU5_TIMERD_SUBFunc->GetDcntTrigger(ch_id)) {
                    case 2:
                        if(mIsCMA[ch_id]) {
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,1);
                            mIsCMA[ch_id] = false;
                            if(mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id)!=0) {
                                mStartDcntEvent[ch_id].notify(mTimeToNotifyStartDcntEvent, mTIMERD->mATU5->mTimeResolutionUnit);
                                mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,1);
                            }
                            else {
                                mClearDSR1D_DSFDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            }
                        }
                        break;
                    case 3:
                        if(mIsCMA[ch_id]) {
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,1);
                            mIsCMA[ch_id] = false;
                            if(mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id)!=0) {
                                mStartDcntEvent[ch_id].notify(mTimeToNotifyStartDcntEvent, mTIMERD->mATU5->mTimeResolutionUnit);
                                mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,1);
                            }
                            else {
                                mClearDSR1D_DSFDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            }
                        }
                        break;
                    case 4:
                        if(mIsCMB[ch_id]) {
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,1);
                            mIsCMB[ch_id] = false;
                            if(mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id)!=0) {
                                mStartDcntEvent[ch_id].notify(mTimeToNotifyStartDcntEvent, mTIMERD->mATU5->mTimeResolutionUnit);
                                mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,2);
                            }
                            else {
                                mClearDSR1D_DSFDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            }
                        }
                        break;
                    case 6:
                        if(mIsCMA[ch_id] && mIsCMB[ch_id]) {
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,1);
                            mIsCMA[ch_id] = false;
                            mIsCMB[ch_id] = false;
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DWFD(ch_id,0);
                            if(mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id)!=0) {
                                mStartDcntEvent[ch_id].notify(mTimeToNotifyStartDcntEvent, mTIMERD->mATU5->mTimeResolutionUnit);
                                mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,3);
                            }
                            else {
                                mClearDSR1D_DSFDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            }
                        }
                        else if (mIsCMA[ch_id] || mIsCMB[ch_id]) {
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DWFD(ch_id,1);
                        }
                        break;
                    case 10:
                        if(mIsCMA[ch_id] || mIsCMB[ch_id]) {
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,1);
                            if(mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id)!=0) {
                                mStartDcntEvent[ch_id].notify(mTimeToNotifyStartDcntEvent, mTIMERD->mATU5->mTimeResolutionUnit);
                                mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,2);
                                if(mIsCMA[ch_id]) {
                                    mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,1);
                                }
                            }
                            else {
                                mClearDSR1D_DSFDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            }
                            mIsCMA[ch_id] = false;
                            mIsCMB[ch_id] = false;
                        }
                        break;
                    case 11:
                        if(mIsCMA[ch_id] || mIsCMB[ch_id]) {
                            mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(ch_id,1);
                            if(mATU5_TIMERD_SUBFunc->Get_DCNTD(ch_id)!=0) {
                                mStartDcntEvent[ch_id].notify(mTimeToNotifyStartDcntEvent, mTIMERD->mATU5->mTimeResolutionUnit);
                                mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,2);
                                if(mIsCMA[ch_id]) {
                                    mATU5_TIMERD_SUBFunc->Set_DSR2D_DWRFD(ch_id,1);
                                }
                            }
                            else {
                                mClearDSR1D_DSFDEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            }
                            mIsCMA[ch_id] = false;
                            mIsCMB[ch_id] = false;
                        }
                        break;
				
                    default:
                        re_printf("warning","TRGSELD[%d]: un-supported setting to start DCNTD.\n",ch_id);
                        break;
                }
            }
        }
        else {
            if(mIsUNF[ch_id]) {
                mIsDcntinUdfPeriod[ch_id] = true;
                mIsDcntRunning[ch_id] = false;
                //Stop timer
                mTimer_DCNTD[ch_id]->StartStop(false);
                mATU5_TIMERD_SUBFunc->Set_DCNTD(ch_id,0);
                mStopDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                mIsUNF[ch_id] = false;
            }
            else {
                switch(mATU5_TIMERD_SUBFunc->GetDcntTrigger(ch_id)) {
                    case 1:
                        if(mIsCMB[ch_id]) {
                            mStopDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            mIsCMB[ch_id] = false;
                        }
                        break;
                    case 3:
                        if(mIsCMB[ch_id]) {
                            mStopDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            mIsCMB[ch_id] = false;
                        }
                        break;
                    case 9:
                        if(mIsCMB[ch_id] || mIsCMA[ch_id]) {
                            mStopDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            mIsCMB[ch_id] = false;
                            mIsCMA[ch_id] = false;
                        }
                        break;
                    case 11:
                        if(mIsCMB[ch_id] || mIsCMA[ch_id]) {
                            mStopDcntEvent[ch_id].notify(CalculateClockEdge(mDCNTCLKName, true,true, current_time,mCountClockStartPoint[ATU5_TIMERD_SUB_AgentController::emDownCounter]),mTIMERD->mATU5->mTimeResolutionUnit);
                            mIsCMB[ch_id] = false;
                            mIsCMA[ch_id] = false;
                        }
                        break;
                    default:
                        re_printf("warning","TRGSELD[%d]: un-supported setting to start DCNTD.\n",ch_id);
                        break;
                }
            }

        }

    }
}//}}}

///*********************************
/// Function     : AssertmIsCMAMethod
/// Description  : Assert mIsCMA variable
///*********************************
void ATU5_TIMERD_SUB::AssertmIsUNFMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsUNF[ch_id] = true;
    }
}//}}}

///*********************************
/// Function     : AssertmIsCMAMethod
/// Description  : Assert mIsCMA variable
///*********************************
void ATU5_TIMERD_SUB::AssertmIsCMAMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mIsCMA[ch_id] = true;
        mCompareMatchAEventDly[ch_id].notify((double)mTIMERD->mPCLKPeriod/4, mTIMERD->mATU5->mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : AssertmIsCMBMethod
/// Description  : Assert mIsCMB variable
///*********************************
void ATU5_TIMERD_SUB::AssertmIsCMBMethod (const unsigned int ch_id)
{//{{{
    if (!(mTIMERD->mATU5->GetResetStatus("presetz") || mTIMERD->mATU5->GetResetStatus("presetz_bif") ||
    (!mTIMERD->mATU5->CheckClockPeriod("CLK_HSB")) || (!mTIMERD->mATU5->CheckClockPeriod("CLK_LSB")))) {
        mCompareMatchBEventDly[ch_id].notify((double)mTIMERD->mPCLKPeriod/4, mTIMERD->mATU5->mTimeResolutionUnit);
        mIsCMB[ch_id] = true;
    }
}//}}}

void ATU5_TIMERD_SUB::HandleInterruptOVF1Method(void)
{//{{{
    INT_OVF1.write(true);
    mClearInterrupt1Event.notify((double)mTIMERD->mPCLKPeriod,mTIMERD->mATU5->mTimeResolutionUnit);
}//}}}

void ATU5_TIMERD_SUB::HandleInterruptOVF2Method(void)
{//{{{
    INT_OVF2.write(true);
    mClearInterrupt2Event.notify((double)mTIMERD->mPCLKPeriod,mTIMERD->mATU5->mTimeResolutionUnit);
}//}}}

void ATU5_TIMERD_SUB::ClearInterruptOVF1Method(void)
{//{{{
    INT_OVF1.write(false);
}//}}}

void ATU5_TIMERD_SUB::ClearInterruptOVF2Method(void)
{//{{{
    INT_OVF2.write(false);
}//}}}

void ATU5_TIMERD_SUB::HandleInterruptCMPAMethod(const unsigned int channel)
{//{{{
  INT_CMPA[channel] -> write(true);
  mClearInterruptOutputCMPAEvent[channel].notify((double)mTIMERD->mPCLKPeriod,mTIMERD->mATU5->mTimeResolutionUnit);
}//}}}

void ATU5_TIMERD_SUB::HandleInterruptCMPBMethod(const unsigned int channel)
{//{{{
  INT_CMPB[channel] -> write(true);
  mClearInterruptOutputCMPBEvent[channel].notify((double)mTIMERD->mPCLKPeriod,mTIMERD->mATU5->mTimeResolutionUnit);
}//}}}

void ATU5_TIMERD_SUB::ClearInterruptCMPAMethod(const unsigned int channel)
{//{{{
  INT_CMPA[channel] -> write(false);
}//}}}

void ATU5_TIMERD_SUB::ClearInterruptCMPBMethod(const unsigned int channel)
{//{{{
  INT_CMPB[channel] -> write(false);
}//}}}

void ATU5_TIMERD_SUB::HandleInterruptFEIEDMethod(const unsigned int channel)
{//{{{
    INT_FEIED[channel] -> write(true);
    mClearInterruptOutputFEIEDEvent[channel].notify((double)mTIMERD->mPCLKPeriod,mTIMERD->mATU5->mTimeResolutionUnit);
}//}}}

void ATU5_TIMERD_SUB::HandleInterruptREIEDMethod(const unsigned int channel)
{//{{{
    INT_REIED[channel] -> write(true);
    mClearInterruptOutputREIEDEvent[channel].notify((double)mTIMERD->mPCLKPeriod,mTIMERD->mATU5->mTimeResolutionUnit);
}//}}}

void ATU5_TIMERD_SUB::ClearInterruptFEIEDMethod(const unsigned int channel)
{//{{{
    INT_FEIED[channel] -> write(false);
}//}}}

void ATU5_TIMERD_SUB::ClearInterruptREIEDMethod(const unsigned int channel)
{//{{{
    INT_REIED[channel] -> write(false);
}//}}}

void ATU5_TIMERD_SUB::AssertINT_UDFMethod(const unsigned int channel)
{//{{{
    INT_UDF[channel] -> write(true);
    mDeassertINT_UDFEvent[channel].notify((double)mTIMERD->mPCLKPeriod,mTIMERD->mATU5->mTimeResolutionUnit);
}//}}}

void ATU5_TIMERD_SUB::DeassertINT_UDFMethod(const unsigned int channel)
{//{{{
    INT_UDF[channel] -> write(false);
}//}}}

unsigned int ATU5_TIMERD_SUB::GetTCNTDRegisterVal (unsigned int counter)
{//{{{
   unsigned int val;
   if (counter == ATU5_TIMERD_SUB_AgentController::emCounter1) {
      val = mATU5_TIMERD_SUBFunc->GetTCNT1DRegValue();
   } else {
      val = mATU5_TIMERD_SUBFunc->GetTCNT2DRegValue();
   }
   return val;
}//}}}

void ATU5_TIMERD_SUB::mOFMIZeroEventFunc(unsigned int chnnl)
{//{{{
    #if 0
        re_printf("info", "mOFMIZeroEventFunc\n");
    #endif
    if (mIsRaisingRequest[chnnl]) {
        if (mIsOFMICNTZero(chnnl) && mIsOTONMICNTZero(chnnl)) {
            mTODMIGValue[chnnl] = true;
            mTriggerTODMIG_Event[chnnl].notify(SC_ZERO_TIME);
            mIsRaisingRequest[chnnl] = false;
        }
    }
}//}}}

void ATU5_TIMERD_SUB::mONMIZeroEventFunc(unsigned int chnnl)
{//{{{
    #if 0
        re_printf("info", "mONMIZeroEventFunc\n");
    #endif
    if (mIsFailingRequest[chnnl]) {
        if (mIsONMICNTZero(chnnl)) {
            mTODMIGValue[chnnl] = false;
            mTriggerTODMIG_Event[chnnl].notify(SC_ZERO_TIME);
            mIsFailingRequest[chnnl] = false;
        }
    }
}//}}}

void ATU5_TIMERD_SUB::mOTONMIZeroEventFunc(unsigned int chnnl)
{//{{{
    #if 0
        re_printf("info", "mOTONMIZeroEventFunc\n");
    #endif
    if (mIsRaisingRequest[chnnl]) {
        if (mIsOFMICNTZero(chnnl) && mIsOTONMICNTZero(chnnl)) {
            mTODMIGValue[chnnl] = true;
            mTriggerTODMIG_Event[chnnl].notify(SC_ZERO_TIME);
            mIsRaisingRequest[chnnl] = false;
        }
    }
}//}}}

bool ATU5_TIMERD_SUB::mIsOFMICNTZero(unsigned int chnnl)
{//{{{
    bool return_val = false;
    unsigned int counter_val = 0x1;
    if (mIsTimerOFMICNTDRunning[chnnl] == true) {
        counter_val = mTimer_OFMICNTD[chnnl]->GetCounterValue();
    } else {
        counter_val = mATU5_TIMERD_SUBFunc->GetOFMICNTDRegValue(chnnl);
    }
    if (counter_val == 0) {
       return_val = true;
    }
    return return_val;
}//}}}

bool ATU5_TIMERD_SUB::mIsONMICNTZero(unsigned int chnnl)
{//{{{
    bool return_val = false;
    unsigned int counter_val = 0x1;
    if (mIsTimerONMICNTDRunning[chnnl] == true) {
        counter_val = mTimer_ONMICNTD[chnnl]->GetCounterValue();
    } else {
        counter_val = mATU5_TIMERD_SUBFunc->GetONMICNTDRegValue(chnnl);
    }
    if (counter_val == 0) {
       return_val = true;
    }
    return return_val;
}//}}}

bool ATU5_TIMERD_SUB::mIsOTONMICNTZero(unsigned int chnnl)
{// {{{
    bool return_val = false;
    unsigned int counter_val = 0x1;
    if (mIsTimerOTONMICNTDRunning[chnnl] == true) {
        counter_val = mTimer_OTONMICNTD[chnnl]->GetCounterValue();
    } else {
        counter_val = mATU5_TIMERD_SUBFunc->GetOTONMICNTDRegValue(chnnl);
    }
    if (counter_val == 0) {
       return_val = true;
    }
    return return_val;
}//}}}
void ATU5_TIMERD_SUB::ProcessPriorityMethod(const unsigned int channel) // ducla
{//{{{
    if (!mIsClearDSFD[channel] &&mIsSetDSFD[channel])
    {
        mSetDSFD = true;
        mATU5_TIMERD_SUBFunc->Set_DSR1D_DSFD(channel, 1);
    }
    if(!mSetTCNT1D && mClearTCNT1D)
    {
        mIsClearTCNT1D = true;
        HandleInputClearCounterMethod();
    }
    if (!mSetTCNT2D && mClearTCNT2D)
    {
        mIsClearTCNT2D = true;
        HandleInputClearCounterMethod();	
    }
    mClearTCNT1D=false;
    mClearTCNT2D=false;
    mSetTCNT1D = false;
    mSetTCNT2D = false;
    mIsClearDSFD[channel] = false;
    mIsSetDSFD[channel] = false;
}//}}}
void ATU5_TIMERD_SUB::WriteCompareMatchValue(unsigned int counter){
    if(counter == ATU5_TIMERD_SUB_AgentController::emCounter1){
	 mWriteCompareMatchValue[ATU5_TIMERD_SUB_AgentController::emCounter1].notify(SC_ZERO_TIME);
    } 
    else if(counter == ATU5_TIMERD_SUB_AgentController::emCounter2){
	 mWriteCompareMatchValue[ATU5_TIMERD_SUB_AgentController::emCounter2].notify(SC_ZERO_TIME);
    }  
}
void ATU5_TIMERD_SUB::UpdateDCNTD(unsigned int channel){
    if(channel == 0){
      	mUpdateDCNTDEvent[0].notify(SC_ZERO_TIME);
    }
    if(channel == 1){
       	mUpdateDCNTDEvent[1].notify(SC_ZERO_TIME);
    }
    if(channel == 2){
    	mUpdateDCNTDEvent[2].notify(SC_ZERO_TIME);
    }
    if(channel == 3){
    	mUpdateDCNTDEvent[3].notify(SC_ZERO_TIME);
    }
}
void ATU5_TIMERD_SUB::WriteTCNTD1(void){
	mProcessWriteTCNT1DEvent.notify(SC_ZERO_TIME);
}

void ATU5_TIMERD_SUB::WriteTCNTD2(void){   
	mProcessWriteTCNT2DEvent.notify(SC_ZERO_TIME);
}

void ATU5_TIMERD_SUB::ProcessSetting(unsigned int channel){
	mProcessSettingEvent[channel].notify(SC_ZERO_TIME);
}
void ATU5_TIMERD_SUB::WriteCounterValue(unsigned int channel)
{
	if(channel==ATU5_TIMERD_SUB_AgentController::emCounter1){
		mWriteCounterValue[ATU5_TIMERD_SUB_AgentController::emCounter1].notify(SC_ZERO_TIME);
	}
	if(channel==ATU5_TIMERD_SUB_AgentController::emCounter2){
		mWriteCounterValue[ATU5_TIMERD_SUB_AgentController::emCounter2].notify(SC_ZERO_TIME);
	}
}
// vim600: set foldmethod=marker :
