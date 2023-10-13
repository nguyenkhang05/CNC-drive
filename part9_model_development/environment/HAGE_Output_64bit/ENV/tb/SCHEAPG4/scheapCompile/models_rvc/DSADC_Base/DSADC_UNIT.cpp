// ---------------------------------------------------------------------
// $Id: DSADC_UNIT.cpp 1398 2019-11-26 06:16:27Z chuonghoangle $
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
#include "DSADC_UNIT.h"
#include "DSADC_UNIT_Func.h"

/// Constructor of DSADC_UNIT class
DSADC_UNIT::DSADC_UNIT (sc_module_name name,
        DSADC_Base *DSADC_Ptr,
        unsigned int ad_id,
        unsigned int ch_num,
        unsigned int rLatency,
        unsigned int wLatency):
    sc_module(name),
    DSADC_UNIT_AgentController(),
    /// Input ports
    ADSVREFH("ADSVREFH"),
    ADSVREFL("ADSVREFL"),
    DSADTRG("DSADTRG"),
    DSADRGT("DSADRGT"),
    /// Output ports
    DSADEND("DSADEND"),
    DSADI("DSADI"),
    DSADE("DSADE"),
    DSADPE("DSADPE"),
    DSADDFREQ0("DSADDFREQ0"),
    DSADDFREQ1("DSADDFREQ1"),
    DSADDFTAG("DSADDFTAG"),
    DSADDFDATA("DSADDFDATA"),
    DSADUE("DSADUE"),
    DSADLE("DSADLE"),
    IsRGTpos(false)

{//{{{

    assert(DSADC_Ptr != NULL);
    mDSADC = DSADC_Ptr;
    /// socket to IF bus
    setSlaveFreqPort32(&mDSADC->CLK_LSB);
    setSlaveResetPort32(&mDSADC->PRESETn);
    setTargetSocket32((char*)"tsp");
    tsp = tSocket32[0]; /// Alias for binding
    mDSADC_UNIT_Func = new DSADC_UNIT_Func((std::string)name, this, ch_num);
    sc_assert(mDSADC_UNIT_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mDSADC_UNIT_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);
    mModuleName = (std::string)name;
    mAdId = ad_id;
    sc_assert(ch_num <= 8U);
    mChannelNum = ch_num;
    std::ostringstream port_name;
    for (unsigned int index = 0; index < mChannelNum; index++) {
        port_name.str("");
        port_name << "AN" << index;
        AN[index] = new sc_in<double>(port_name.str().c_str());
        sc_assert(AN[index] != NULL);
    }
    for (unsigned int index = 0; index < DSADC_Base::emVCNum; index++) {
        port_name.str("");
        port_name << "DSADVCI" << index;
        DSADVCI[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(DSADVCI[index] != NULL);
    }
    
    /// Declare method/thread
    SC_METHOD(WriteDSADENDMethod);
    dont_initialize();
    sensitive << mWriteDSADENDEvent;
    
    SC_METHOD(WriteDSADDFTAGMethod);
    dont_initialize();
    sensitive << mWriteDSADDFTAGEvent;
    
    SC_METHOD(WriteDSADDFDATAMethod);
    dont_initialize();
    sensitive << mWriteDSADDFDATAEvent;
    
    SC_METHOD(AssertDSADIMethod);
    dont_initialize();
    sensitive << mAssertDSADIEvent;
    
    SC_METHOD(DeassertDSADIMethod);
    dont_initialize();
    sensitive << mDeassertDSADIEvent;
    
    SC_METHOD(AssertDSADEMethod);
    dont_initialize();
    sensitive << mAssertDSADEEvent;
    
    SC_METHOD(DeassertDSADEMethod);
    dont_initialize();
    sensitive << mDeassertDSADEEvent;
    
    SC_METHOD(AssertDSADPEMethod);
    dont_initialize();
    sensitive << mAssertDSADPEEvent;
    
    SC_METHOD(DeassertDSADPEMethod);
    dont_initialize();
    sensitive << mDeassertDSADPEEvent;
    
    SC_METHOD(AssertDSADDFREQ0Method);
    dont_initialize();
    sensitive << mAssertDSADDFREQ0Event;
    
    SC_METHOD(DeassertDSADDFREQ0Method);
    dont_initialize();
    sensitive << mDeassertDSADDFREQ0Event;
    
    SC_METHOD(AssertDSADDFREQ1Method);
    dont_initialize();
    sensitive << mAssertDSADDFREQ1Event;
    
    SC_METHOD(DeassertDSADDFREQ1Method);
    dont_initialize();
    sensitive << mDeassertDSADDFREQ1Event;
    
    SC_METHOD(AssertDSADUEMethod);
    dont_initialize();
    sensitive << mAssertDSADUEEvent;
    
    SC_METHOD(DeassertDSADUEMethod);
    dont_initialize();
    sensitive << mDeassertDSADUEEvent;
    
    SC_METHOD(AssertDSADLEMethod);
    dont_initialize();
    sensitive << mAssertDSADLEEvent;
    
    SC_METHOD(DeassertDSADLEMethod);
    dont_initialize();
    sensitive << mDeassertDSADLEEvent;
    
    SC_METHOD(WriteOutDSADIportMethod);
    dont_initialize();
    sensitive << mWriteOutDSADIevent;

    SC_METHOD(WriteOutDSADEportMethod);
    dont_initialize();
    sensitive << mWriteOutDSADEevent;

    SC_METHOD(WriteOutDSADPEportMethod);
    dont_initialize();
    sensitive << mWriteOutDSADPEevent;

    SC_METHOD(WriteOutFREQ0portMethod);
    dont_initialize();
    sensitive << mWriteOutDFREQ0event;

    SC_METHOD(WriteOutFREQ1portMethod);
    dont_initialize();
    sensitive << mWriteOutDFREQ1event;

    SC_METHOD(WriteOutDSADUEportMethod);
    dont_initialize();
    sensitive << mWriteOutDSADUEevent;

    SC_METHOD(WriteOutDSADLEportMethod);
    dont_initialize();
    sensitive << mWriteOutDSADLEevent;

    SC_METHOD(HandleDSADTRGInputMethod);
    dont_initialize();
    sensitive << mTriggerEvent;

    for (unsigned int ch_id = 0; ch_id < DSADC_Base::emVCNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertDSADVCIEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DSADC_UNIT::AssertDSADVCIMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertDSADVCIMethod"), &opt);
    }
    
    for (unsigned int ch_id = 0; ch_id < DSADC_Base::emVCNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeassertDSADVCIEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&DSADC_UNIT::DeassertDSADVCIMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeassertDSADVCIMethod"), &opt);
    }
    
    for (unsigned int ch_id = 0; ch_id < DSADC_Base::emVCNum; ch_id++) {
         sc_core::sc_spawn_options opt;
         opt.spawn_method();
         opt.set_sensitivity(&mWriteOutDSADVCIjEvent[ch_id]);
         opt.dont_initialize();
         sc_core::sc_spawn(sc_bind(&DSADC_UNIT::WriteOutVCIjPortMethod, this, ch_id),
                           sc_core::sc_gen_unique_name("WriteOutVCIjPort"), &opt);
     }

    SC_METHOD(CheckPeriodTriggerMethod);
    dont_initialize();
    sensitive << DSADTRG.neg();
    
    SC_METHOD(HandleDSADRGTInputMethod);
    dont_initialize();
    sensitive << DSADRGT.pos();
    sensitive << DSADRGT.neg();
    
    SC_METHOD(StartTriggerMethod);
    dont_initialize();
    sensitive << mStartTriggerEvent;
    
    SC_METHOD(EndTriggerMethod);
    dont_initialize();
    sensitive << mEndTriggerEvent;
    
    SC_METHOD(CalibStartMethod);
    dont_initialize();
    sensitive << mCalibStartEvent;
    
    SC_METHOD(CalibStopMethod);
    dont_initialize();
    sensitive << mCalibStopEvent;
    
    SC_METHOD(CalibFinishMethod);
    dont_initialize();
    sensitive << mCalibFinishEvent;
    
    SC_THREAD(ADProcessingThread);
    dont_initialize();
    sensitive << mADStartEvent;
    
    SC_METHOD(ConvertFinishMethod);
    dont_initialize();
    sensitive << mConvertFinishEvent;
    
    SC_METHOD(ConvertStopMethod);
    dont_initialize();
    sensitive << mConvertStopEvent;
    
    SC_METHOD(AssertADACTMethod);
    dont_initialize();
    sensitive << mAssertADACTEvent;
    
    SC_METHOD(DeassertADACTMethod);
    dont_initialize();
    sensitive << mDeassertADACTEvent;
    /// Initialize variables & output ports
    Initialize();
    /// Initialize output ports
    DSADEND.initialize(false);
    DSADI.initialize(false);
    DSADE.initialize(false);
    DSADPE.initialize(false);
    DSADDFREQ0.initialize(false);
    DSADDFREQ1.initialize(false);
    DSADDFTAG.initialize((sc_uint<4>)0);
    DSADDFDATA.initialize((sc_uint<16>)0);
    DSADUE.initialize(false);
    DSADLE.initialize(false);
    for (unsigned int i = 0; i < DSADC_Base::emVCNum; i++) {
        DSADVCI[i]->initialize(false);
    }
}//}}}

///Destructor of DSADC_UNIT class
DSADC_UNIT::~DSADC_UNIT (void)
{//{{{    
    delete mDSADC_UNIT_Func;
    for (unsigned int i = 0; i < DSADC_Base::emVCNum; i++) {
        delete DSADVCI[i];
    }
    for (unsigned int j = 0; j < mChannelNum; j++) {
        delete AN[j];
    }
}//}}}

/// Check Odd/Even DSADC_E2XFCC1_2ND
bool DSADC_UNIT::IsOddDsadc (void)
{//{{{
    bool ret = false;
    if ((mAdId % 2)>0) {
        ret = true;
    }
    return ret;
}//}}}

/// Cancel operation events
void DSADC_UNIT::CancelEvents (void)
{//{{{

    mWriteDSADENDEvent.cancel();
    mWriteOutDSADIevent.cancel();
    mWriteOutDSADEevent.cancel();
    mWriteOutDSADPEevent.cancel();
    mWriteOutDFREQ0event.cancel();
    mWriteOutDFREQ1event.cancel();
    mWriteDSADDFTAGEvent.cancel();
    mWriteDSADDFDATAEvent.cancel();
    mWriteOutDSADUEevent.cancel();
    mWriteOutDSADLEevent.cancel();
    for (unsigned int i = 0; i < DSADC_Base::emVCNum; i++) {
        mWriteOutDSADVCIjEvent[i].cancel();
        mAssertDSADVCIEvent[i].cancel();
        mDeassertDSADVCIEvent[i].cancel();
    }

    mAssertDSADIEvent.cancel();
    mDeassertDSADIEvent.cancel();
    mAssertDSADEEvent.cancel();
    mDeassertDSADEEvent.cancel();
    mAssertDSADPEEvent.cancel();
    mDeassertDSADPEEvent.cancel();
    mAssertDSADDFREQ0Event.cancel();
    mDeassertDSADDFREQ0Event.cancel();
    mAssertDSADDFREQ1Event.cancel();
    mDeassertDSADDFREQ1Event.cancel();
    mAssertDSADUEEvent.cancel();
    mDeassertDSADUEEvent.cancel();
    mAssertDSADLEEvent.cancel();
    mDeassertDSADLEEvent.cancel();
    mStartTriggerEvent.cancel();
    mEndTriggerEvent.cancel();
    mCalibStartEvent.cancel();
    mCalibStopEvent.cancel();
    mCalibFinishEvent.cancel();
    mADStartEvent.cancel();
    mADStopEvent.cancel();
    mConvertFinishEvent.cancel();
    mConvertStopEvent.cancel();
    mAssertADACTEvent.cancel();
    mDeassertADACTEvent.cancel();
    mTriggerEvent.cancel();
}//}}}

/// Initialize variables & output ports
void DSADC_UNIT::Initialize (void)
{//{{{
    /// Initialize variables
    mDSADEND = false;
    mDSADI = false;
    mDSADE = false;
    mDSADPE = false;
    mDSADDFREQ0 = false;
    mDSADDFREQ1 = false;
    mDSADDFTAG = 0;
    mDSADDFDATA = 0;
    mDSADUE = false;
    mDSADLE = false;
    mCalibTimeStop = 0;
    for (unsigned int i = 0; i < DSADC_Base::emVCNum; i++) {
        mDSADVCI[i] = false;
    }
    mIsADStopEvent = false;
    mIsTriggerProcessing = false;
    mIsContinueConvert = false;
    mADConvFinishTime = 0;
    mIsEndTrigger = false;   
    mDSADC_UNIT_Func->Initialize();    
}//}}}

/// Get analog input values
double DSADC_UNIT::GetAnalogValue (const bool is_refh, const unsigned int an_no)
{//{{{
    double ret = 0;
    if (is_refh) {
        ret = ADSVREFH.read();
    } else {
        if (mDSADC->GetIsPinDiagEnable()) {                                 /// GetIsPinDiagEnable
            if (mDSADC_UNIT_Func->GetPinDiagBitValue(an_no) == 1) {
                ret = mDSADC->mADSVCC;
            } else {
                ret = mDSADC->mADSVSS;
            }
        } else {
            ret = AN[an_no]->read();
        }
    }
    return ret;
}//}}}

/// Notify event in DSADC_UNIT
void DSADC_UNIT::NotifyEvent (const unsigned int event, const unsigned int vc_no)
{//{{{
    double current_time = sc_time_stamp().to_double();
    switch (event) {
        case emStartTrigger :
            mStartTriggerEvent.notify(SC_ZERO_TIME);
            break;
        case emEndTrigger :
            mEndTriggerEvent.notify(SC_ZERO_TIME);
            break;
        case emCalibStart :
            mCalibStartEvent.notify(SC_ZERO_TIME);
            break;
        case emCalibStop :
            mCalibStopEvent.notify(SC_ZERO_TIME);
            break;
        case emAssertDSADPE :
            mAssertDSADPEEvent.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time) + mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
            break;
        case emAssertDSADVCI :
            mAssertDSADVCIEvent[vc_no].notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time), mDSADC->mTimeResolutionUnit);
            break;
        case emAssertDSADUE :
            mAssertDSADUEEvent.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time), mDSADC->mTimeResolutionUnit);
            break;
        default:    /// emAssertDSADLE
            mAssertDSADLEEvent.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time), mDSADC->mTimeResolutionUnit);
            break;
    }
}//}}}

/// Write to DSADEND output port
void DSADC_UNIT::WriteDSADENDMethod (void)
{//{{{
    DSADEND.write(mDSADEND);
}//}}}

/// Write to DSADDFTAG output port
void DSADC_UNIT::WriteDSADDFTAGMethod (void)
{//{{{
    DSADDFTAG.write((sc_uint<4>)mDSADDFTAG);
}//}}}

/// Write to DSADDFDATA output port
void DSADC_UNIT::WriteDSADDFDATAMethod (void)
{//{{{
    DSADDFDATA.write((sc_uint<16>)mDSADDFDATA);
}//}}}

/// Assert DSADI interrupt port
void DSADC_UNIT::AssertDSADIMethod (void)
{//{{{
    mDSADI = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADI] Assert.\n");
    }
    mWriteOutDSADIevent.notify(SC_ZERO_TIME);
    mDeassertDSADIEvent.notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
}//}}}

/// De-assert DSADI interrupt port
void DSADC_UNIT::DeassertDSADIMethod(void)
{//{{{
    mDSADI = false;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADI] De-assert.\n");
    }
    mWriteOutDSADIevent.notify(SC_ZERO_TIME);
}//}}}

/// Write to DSADI port
void DSADC_UNIT::WriteOutDSADIportMethod(void)
{//{{{
    DSADI.write(mDSADI);
}//}}}

/// Assert DSADE interrupt port
void DSADC_UNIT::AssertDSADEMethod (void)
{//{{{
    mDSADE = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADE] Assert.\n");
    }
    mWriteOutDSADEevent.notify(SC_ZERO_TIME);
    mDeassertDSADEEvent.notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
 }//}}}

 /// De-assert DSADE interrupt port
 void DSADC_UNIT::DeassertDSADEMethod (void)
 {//{{{
     mDSADE = false;
     if (mDSADC->mDumpInterrupt) {
         re_printf("info","INT [DSADE] De-assert.\n");
     }
     mWriteOutDSADEevent.notify(SC_ZERO_TIME);
 }//}}}

 ///Write to port DSADE
 void DSADC_UNIT::WriteOutDSADEportMethod(void)
 {//{{{
     DSADE.write(mDSADE);
 }//}}}

/// Assert DSADPE interrupt port
void DSADC_UNIT::AssertDSADPEMethod (void)
{//{{{
    mDSADPE = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADPE] Assert.\n");
    }
    mWriteOutDSADPEevent.notify(SC_ZERO_TIME);
    mDeassertDSADPEEvent.notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
}//}}}

/// De-assert DSADPE interrupt port
void DSADC_UNIT::DeassertDSADPEMethod (void)
{//{{{
    mDSADPE = false;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADPE] De-assert.\n");
    }
    mWriteOutDSADPEevent.notify(SC_ZERO_TIME);
}//}}}

///Write to DSADPE port
void DSADC_UNIT::WriteOutDSADPEportMethod(void)
{//{{{
    DSADPE.write(mDSADPE);
}//}}}

/// Assert DSADDFREQ0 interrupt port
void DSADC_UNIT::AssertDSADDFREQ0Method (void)
{//{{{
    mDSADDFREQ0 = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADDFREQ0] Assert.\n");
    }
    mWriteOutDFREQ0event.notify(SC_ZERO_TIME);
    mDeassertDSADDFREQ0Event.notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
}//}}}

/// De-assert DSADDFREQ0 interrupt port
void DSADC_UNIT::DeassertDSADDFREQ0Method (void)
{//{{{
    mDSADDFREQ0 = false;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADDFREQ0] De-assert.\n");
    }
    mWriteOutDFREQ0event.notify(SC_ZERO_TIME);
}//}}}

/// Write to DSADDFREQ0 port
void DSADC_UNIT::WriteOutFREQ0portMethod(void)
{//{{{
    DSADDFREQ0.write(mDSADDFREQ0);
}//}}}

/// Assert DSADDFREQ1 interrupt port
void DSADC_UNIT::AssertDSADDFREQ1Method (void)
{//{{{
    mDSADDFREQ1 = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADDFREQ1] Assert.\n");
    }
    mWriteOutDFREQ1event.notify(SC_ZERO_TIME);
    mDeassertDSADDFREQ1Event.notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
}//}}}

/// De-assert DSADDFREQ1 interrupt port
void DSADC_UNIT::DeassertDSADDFREQ1Method (void)
{//{{{
    mDSADDFREQ1 = false;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADDFREQ1] De-assert.\n");
    }
    mWriteOutDFREQ1event.notify(SC_ZERO_TIME);
}//}}}

/// Write to DSADDFREQ1 port
void DSADC_UNIT::WriteOutFREQ1portMethod(void)
{//{{{
    DSADDFREQ1.write(mDSADDFREQ1);
}//}}}

/// Assert DSADUE interrupt port
void DSADC_UNIT::AssertDSADUEMethod (void)
{//{{{
    mDSADUE = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADUE] Assert.\n");
    }
    mWriteOutDSADUEevent.notify(SC_ZERO_TIME);
    mDeassertDSADUEEvent.notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
}//}}}

/// De-assert DSADUE interrupt port
void DSADC_UNIT::DeassertDSADUEMethod (void)
{//{{{
    mDSADUE = false;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADUE] De-assert.\n");
    }
    mWriteOutDSADUEevent.notify(SC_ZERO_TIME);
}//}}}

/// Write DSADUE  port
void DSADC_UNIT::WriteOutDSADUEportMethod(void)
{//{{{
    DSADUE.write(mDSADUE);
}//}}}


/// Assert DSADLE interrupt port
void DSADC_UNIT::AssertDSADLEMethod (void)
{//{{{
    mDSADLE = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADLE] Assert.\n");
    }
    mWriteOutDSADLEevent.notify(SC_ZERO_TIME);
    mDeassertDSADLEEvent.notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
}//}}}

/// De-assert DSADLE interrupt port
void DSADC_UNIT::DeassertDSADLEMethod (void)
{//{{{
    mDSADLE = false;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADLE] De-assert.\n");
    }
    mWriteOutDSADLEevent.notify(SC_ZERO_TIME);
}//}}}

/// Write DSADLE  port
void DSADC_UNIT::WriteOutDSADLEportMethod(void)
{//{{{
    DSADLE.write(mDSADLE);
}//}}}


/// Assert DSADVCIj interrupt port
void DSADC_UNIT::AssertDSADVCIMethod (const unsigned int ch_id)
{//{{{
    mDSADVCI[ch_id] = true;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADVCI%d] Assert.\n", ch_id);
    }
    mWriteOutDSADVCIjEvent[ch_id].notify(SC_ZERO_TIME);
    mDeassertDSADVCIEvent[ch_id].notify((double)mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
}//}}}

/// De-assert DSADVCIj interrupt port
void DSADC_UNIT::DeassertDSADVCIMethod (const unsigned int ch_id)
{//{{{
    mDSADVCI[ch_id] = false;
    if (mDSADC->mDumpInterrupt) {
        re_printf("info","INT [DSADVCI%d] De-assert.\n", ch_id);
    }
    mWriteOutDSADVCIjEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

/// Write to DSADVCIj port
void DSADC_UNIT::WriteOutVCIjPortMethod(const unsigned int ch_id)
{//{{{
    DSADVCI[ch_id]->write(mDSADVCI[ch_id]);
}//}}}

/// Handle External trigger
void DSADC_UNIT::HandleDSADTRGInputMethod (void)
{//{{{
    bool tempVal = DSADTRG.read();
    if (!mDSADC->GetResetStatus("resetad_n") && mDSADC->CheckClockPeriod("CLK_HSB")) {
        if (tempVal == 0)
        {
        switch(mDSADC_UNIT_Func->TriggerType()) {
            case 3 :
            case 2 :
                /// End trigger
                mEndTriggerEvent.notify(SC_ZERO_TIME);
                break;
            case 1 :
                /// Start trigger
                mStartTriggerEvent.notify(SC_ZERO_TIME);
                break;
            default:
                break;
        }
        } else { re_printf("warning","Trigger input is less than 1 ADCLK.\n");
        }

        }
}//}}}

/// Wait to 1 CLK_HSB to check TRG input after failing edged is detected
void DSADC_UNIT::CheckPeriodTriggerMethod(void)
{//{{{
    mTriggerEvent.notify((double)mDSADC->mCLK_HSBPeriod,mDSADC->mTimeResolutionUnit);
}//}}}

/// Handle DSADRGT input
void DSADC_UNIT::HandleDSADRGTInputMethod (void)
{//{{{
    if (!mDSADC->GetResetStatus("resetad_n") && mDSADC->CheckClockPeriod("CLK_HSB")) {
        if (IsRGTpos == false){
            IsRGTpos = true;
            mTimeRGTpos = sc_time_stamp().to_double();
            if (mADConvFinishTime > 0) {
                double current_time = sc_time_stamp().to_double();
                unsigned int tx = (unsigned int)((current_time - mADConvFinishTime) / mDSADC->mFosPeriod);
                mDSADC_UNIT_Func->UpdateTimeStamp(true, tx);
            } else {
                re_printf("warning","DSADRGT is active while converting \n");
            }
        } else {
            IsRGTpos = false;
            mTimeRGTneg = sc_time_stamp().to_double();
            if ((mTimeRGTneg - mTimeRGTpos - 10 * mDSADC->mCLK_LSBPeriod) < 0){
                re_printf("warning","DSADRGT's rising time is less than 10 PCLK.\n");
            }
        }
        }
}//}}}

/// Process Start trigger
void DSADC_UNIT::StartTriggerMethod (void)
{//{{{
    if (!mDSADC->GetResetStatus("resetad_n") && mDSADC->CheckClockPeriod("CLK_HSB")) {
        if (!mIsTriggerProcessing) {
            if (mDSADC_UNIT_Func->GetStatusFlag(mDSADC_UNIT_Func->emADCT)) {
                /// ADACT = 1
                    if (mDSADC_UNIT_Func->CheckSetting(true)) {
                        mIsContinueConvert = true;
                        mIsADStopEvent = true;
                        mADStopEvent.notify(SC_ZERO_TIME);
                    }
            } else {
                if (mDSADC_UNIT_Func->CheckSetting(false)) {
                    if (mDSADC_UNIT_Func->GetStatusFlag(mDSADC_UNIT_Func->emdCLBACT)) {
                        /// CLBACT = 1
                        mCalibStopEvent.notify(SC_ZERO_TIME);
                    }
                    mADStartEvent.notify(SC_ZERO_TIME);
                }
            }
        }
    }
}//}}}

/// Process End trigger
void DSADC_UNIT::EndTriggerMethod (void)
{//{{{
    if (!mDSADC->GetResetStatus("resetad_n") && mDSADC->CheckClockPeriod("CLK_HSB")) {
        if (!mIsTriggerProcessing) {
            if (mDSADC_UNIT_Func->GetStatusFlag(mDSADC_UNIT_Func->emADCT)) {
                /// ADACT = 1
                mIsEndTrigger = true;
                if (mIsContinueConvert) {
                    mADStartEvent.cancel();
                    mDeassertADACTEvent.notify(SC_ZERO_TIME);
                    mConvertStopEvent.notify(SC_ZERO_TIME);
                } else {
                    mIsADStopEvent = true;
                    mADStopEvent.notify(SC_ZERO_TIME);
                }
            }
        }
    }
}//}}}

/// Start Calibration processing
void DSADC_UNIT::CalibStartMethod (void)
{//{{{
    if (!mDSADC->GetResetStatus("resetad_n") && mDSADC->CheckClockPeriod("CLK_HSB")) {
        if (!mDSADC_UNIT_Func->GetStatusFlag(mDSADC_UNIT_Func->emADCT) && !mDSADC_UNIT_Func->GetStatusFlag(mDSADC_UNIT_Func->emdCLBACT) && !mIsTriggerProcessing) {
            /// ADACT = 0 & CLBACT = 0
            /// Set CLBACT to 1
            mDSADC_UNIT_Func->SetStatusFlag(false, true);
            /// Wait for Calibration finish
            mCalibFinishEvent.notify((double)(mDSADC->mtCLB * mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit);
        }
    }
}//}}}

/// Stop Calibration processing
void DSADC_UNIT::CalibStopMethod (void)
{//{{{
    if (!mDSADC->GetResetStatus("resetad_n") && mDSADC->CheckClockPeriod("CLK_HSB")) {
        /// Cancel Calibration process
        mCalibFinishEvent.cancel();
        /// Clear CLBACT to 0
        mDSADC_UNIT_Func->SetStatusFlag(false, false);
        mCalibTimeStop = sc_time_stamp().to_double();
    }
}//}}}

/// Calibration processing is finished
void DSADC_UNIT::CalibFinishMethod (void)
{//{{{
    if (!mDSADC->GetResetStatus("resetad_n") && mDSADC->CheckClockPeriod("CLK_HSB")) {
        /// Clear CLBACT to 0
        mDSADC_UNIT_Func->SetStatusFlag(false, false);
        mDSADC_UNIT_Func->UpdateCalibResult();
    }
}//}}}

//Get time point calibration stop
double DSADC_UNIT::GetCalibTimeStop(void)
{//{{{
    return mCalibTimeStop;
}//}}}

/// Clear time point calibaration stop
void DSADC_UNIT::ClearCalibTimeStop(void)
{//{{{
    mCalibTimeStop = 0;
}//}}}

/// A/D conversion processing
void DSADC_UNIT::ADProcessingThread (void)
{//{{{
    for(;;) {
        if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||
        (!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
            wait();
            continue;
        }
        try {
            while (true) {
                mIsADStopEvent = false;
                mADConvFinishTime = 0;
                bool is_start = false;
                double current_time = 0;
                if (!mIsContinueConvert) {
                    mIsTriggerProcessing = true;
                    /// Wait ADACT (internal) sync time
                    current_time = sc_time_stamp().to_double();
                    wait((mDSADC->CalculateClockEdge("CLK_HSB", true, current_time) + 3*mDSADC->mCLK_HSBPeriod), mDSADC->mTimeResolutionUnit,
                        mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                        mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0]);
                    if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||
                    (!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
                        throw vpcl::emRstException;
                    }
                    /// Assert DSADEND
                    mDSADEND = true;
                    mWriteDSADENDEvent.notify(SC_ZERO_TIME);
                    /// Assert DSADnADSR.ADACT
                    current_time = sc_time_stamp().to_double();
                    mAssertADACTEvent.notify((mDSADC->CalculateClockEdge("CLK_LSB", true, current_time) + 2*mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit);
                    /// Wait Fos sync time
                    if (IsOddDsadc()) {
                        wait(mDSADC->CalculateClockEdge("Fos", false, current_time), mDSADC->mTimeResolutionUnit,
                            mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                            mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0] | mADStopEvent);
                    } else {
                        /// Even DSADC_E2XFCC1_2NDs
                        wait((mDSADC->CalculateClockEdge("Fos", true, current_time) + mDSADC->mFosPeriod), mDSADC->mTimeResolutionUnit,
                            mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                            mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0] | mADStopEvent);
                    }
                    if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||(!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
                        throw vpcl::emRstException;
                    }
                }
                mIsContinueConvert = false;
                
                if (!mIsADStopEvent) {
                    /// Enter Internal stability period
                    re_printf("info","%s enters Internal stability period.\n", mModuleName.c_str());
                    /// Wait internal stabilization time
                    if (IsOddDsadc()) {
                        wait((double)(mDSADC->mtWOdd * mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit,
                            mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                            mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0] | mADStopEvent);
                    } else {
                        wait((double)(mDSADC->mtWEven * mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit,
                            mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                            mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0] | mADStopEvent);
                    }
                    if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||
                    (!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
                        throw vpcl::emRstException;
                    }
                    
                    if (!mIsADStopEvent) {
                        /// Enter Converting state
                        re_printf("info","%s starts AD conversion.\n", mModuleName.c_str());
                        bool is_first_convert = true;
                        is_start = true;
                        while (true) {
                            /// Wait Conversion time
                            if (is_first_convert) {
                                wait((double)(mDSADC->mtDF * mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit,
                                    mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                                    mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0] | mADStopEvent);
                            } else {
                                wait((double)(mDSADC->mtS * mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit,
                                    mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                                    mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0] | mADStopEvent);
                            }
                            if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||
                            (!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
                                throw vpcl::emRstException;
                            }
                            if (mIsADStopEvent) {
                                break;
                            }
                            is_first_convert = false;
                            mConvertFinishEvent.notify(SC_ZERO_TIME);
                        }
                    }
                }
                
                /// AD processing is stopped
                mIsTriggerProcessing = true;
                /// Wait ADACT (internal) sync time
                current_time = sc_time_stamp().to_double();
                wait((mDSADC->CalculateClockEdge("CLK_HSB", true, current_time) + 3*mDSADC->mCLK_HSBPeriod), mDSADC->mTimeResolutionUnit,
                      mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                      mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0]);
                if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||(!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
                    throw vpcl::emRstException;
                }
                mIsTriggerProcessing = false;
                if (mIsContinueConvert) {
                    mADStartEvent.notify((double)(mDSADC->mtR * mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit);
                } else {
                    /// Deassert DSADEND
                    mDSADEND = false;
                    mWriteDSADENDEvent.notify(SC_ZERO_TIME);
                    /// Deassert DSADnADSR.ADACT
                    current_time = sc_time_stamp().to_double();
                    mDeassertADACTEvent.notify((mDSADC->CalculateClockEdge("CLK_LSB", true, current_time) + 2*mDSADC->mCLK_LSBPeriod), mDSADC->mTimeResolutionUnit);
                    /// Wait for DSADC_E2XFCC1_2ND stop
                    wait((double)mDSADC->mCLK_HSBPeriod, mDSADC->mTimeResolutionUnit,
                         mDSADC->mResetHardEvent[1] | mDSADC->mResetCmdEvent[1] | mDSADC->mResetHardEvent[0] | mDSADC->mResetCmdEvent[0] |
                                 mDSADC->mClkZeroEvent[1] | mDSADC->mClkZeroEvent[0]);
                    if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||(!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
                        throw vpcl::emRstException;
                    }
                }
                if (is_start) {
                    re_printf("info","%s stops AD conversion.\n", mModuleName.c_str());
                }
                
                mConvertStopEvent.notify(SC_ZERO_TIME);
                wait();
                if (mDSADC->GetResetStatus("resetad_n") || mDSADC->GetResetStatus("PRESETn") ||(!mDSADC->CheckClockPeriod("CLK_HSB")) || (!mDSADC->CheckClockPeriod("CLK_LSB"))) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

/// When each A/D conversion finished
void DSADC_UNIT::ConvertFinishMethod (void)
{//{{{
    /// Update convert finish time
    mADConvFinishTime = sc_time_stamp().to_double();
    /// Update DIR, UDIR, ER registers
    mDSADC_UNIT_Func->UpdateConversionResult();
    
    /// Assert DSADI, DSADE interrupts
    bool dsadrgt = DSADRGT.read();
    double current_time = sc_time_stamp().to_double();
    if (mDSADC_UNIT_Func->IsADInterruptEnable(dsadrgt)) {
        mAssertDSADIEvent.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time) + mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
    }
    if (mDSADC_UNIT_Func->IsErrorInterrupt()) {
        mAssertDSADEEvent.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time) + mDSADC->mCLK_LSBPeriod, mDSADC->mTimeResolutionUnit);
    }
    
    /// DFE control
    unsigned int dfreq = 0;
    if (mDSADC_UNIT_Func->IsDFEEntry(dfreq, mDSADDFTAG, mDSADDFDATA)) {
        if (dfreq == 0) {
            mAssertDSADDFREQ0Event.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time), mDSADC->mTimeResolutionUnit);
        } else {
            mAssertDSADDFREQ1Event.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time), mDSADC->mTimeResolutionUnit);
        }
        mWriteDSADDFTAGEvent.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time), mDSADC->mTimeResolutionUnit);
        mWriteDSADDFDATAEvent.notify(mDSADC->CalculateClockEdge("CLK_LSB", true, current_time), mDSADC->mTimeResolutionUnit);
    }
}//}}}

/// AD conversion stop processing
void DSADC_UNIT::ConvertStopMethod (void)
{//{{{
    /// Update VCPTR
    mDSADC_UNIT_Func->UpdateVCPointer(mIsEndTrigger);
    /// Reset convert finish time
    mADConvFinishTime = 0;
    /// Reset Time Stamp register
    mDSADC_UNIT_Func->UpdateTimeStamp(false, 0);
    mIsEndTrigger = false;
}//}}}

/// Assert DSADnADSR.ADACT bit
void DSADC_UNIT::AssertADACTMethod (void)
{//{{{
    mDSADC_UNIT_Func->SetStatusFlag(true, true);
    mIsTriggerProcessing = false;
}//}}}

/// De-assert DSADnADSR.ADACT bit
void DSADC_UNIT::DeassertADACTMethod (void)
{//{{{
    mDSADC_UNIT_Func->SetStatusFlag(true, false);
    mIsTriggerProcessing = false;
}//}}}

// vim600: set foldmethod=marker :
