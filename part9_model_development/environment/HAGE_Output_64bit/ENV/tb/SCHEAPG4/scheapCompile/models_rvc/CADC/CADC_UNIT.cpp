// ---------------------------------------------------------------------
// $Id: CADC_UNIT.cpp,v 1.5 2018/11/20 08:01:49 chanle Exp $
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
#include "CADC_UNIT.h"
#include "CADC_UNIT_Func.h"
#include "CADC.h"
#include "CADC_Func.h"

///Constructor of CADC_UNIT class
CADC_UNIT::CADC_UNIT (sc_module_name name,
                        CADC *CADC_Ptr,
                        unsigned int ad_id,
                        unsigned int ch_num):
    sc_module(name),
    CADC_UNIT_AgentController(),
    // Input ports
    ADSVREFH("ADSVREFH"),
    ADSVREFL("ADSVREFL"),
    CADTRG("CADTRG"),
    CADRGT("CADRGT"),
    // Output ports
    CADEND("CADEND"),
    CADI("CADI"),
    CADE("CADE"),
    CADPE("CADPE"),
    CADDFREQ("CADDFREQ"),
    CADDFTAG("CADDFTAG"),
    CADDFDATA("CADDFDATA"),
    CADUE("CADUE"),
    CADLE("CADLE")
{//{{{
    assert(CADC_Ptr != NULL);
    mCADC = CADC_Ptr;
    mModuleName = (std::string)name;
    mAdId = ad_id;
    mChannelNum = ch_num;
    mCADC_UNIT_Func = new CADC_UNIT_Func((std::string)name, this);
    sc_assert(mCADC_UNIT_Func != NULL);

    std::ostringstream port_name;
    for (unsigned int index = 0; index < 8; index++) {
        port_name.str("");
        port_name << "AN" << index;
        AN[index] = new sc_in<double>(port_name.str().c_str());
        sc_assert(AN[index] != NULL);
    }
    for (unsigned int index = 0; index < CADC_AgentController::emVCNum; index++) {
        port_name.str("");
        port_name << "CADCVCI" << index;
        CADCVCI[index] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(CADCVCI[index] != NULL);
    }
    
    // Initialize variables & output ports
    //Initialize output ports
    CADEND.initialize(0);
    CADI.initialize(0);
    CADE.initialize(0);
    CADPE.initialize(0);
    CADDFREQ.initialize(0);
    CADDFTAG.initialize(0);
    CADDFDATA.initialize(0);
    CADUE.initialize(0);
    CADLE.initialize(0);
    for (unsigned int i = 0; i < CADC_AgentController::emVCNum; i++) {
        CADCVCI[i]->initialize(0);
    }
    Initialize();
    
    // Declare method/thread
    SC_METHOD(WriteCADENDMethod);
    dont_initialize();
    sensitive << mWriteCADENDEvent;
    
    SC_METHOD(WriteCADDFTAGMethod);
    dont_initialize();
    sensitive << mWriteCADDFTAGEvent;
    
    SC_METHOD(WriteCADDFDATAMethod);
    dont_initialize();
    sensitive << mWriteCADDFDATAEvent;
    
    SC_METHOD(AssertCADIMethod);
    dont_initialize();
    sensitive << mAssertCADIEvent;
    
    SC_METHOD(DeassertCADIMethod);
    dont_initialize();
    sensitive << mDeassertCADIEvent;
    
    SC_METHOD(AssertCADEMethod);
    dont_initialize();
    sensitive << mAssertCADEEvent;
    
    SC_METHOD(DeassertCADEMethod);
    dont_initialize();
    sensitive << mDeassertCADEEvent;
    
    SC_METHOD(AssertCADPEMethod);
    dont_initialize();
    sensitive << mAssertCADPEEvent;
    
    SC_METHOD(DeassertCADPEMethod);
    dont_initialize();
    sensitive << mDeassertCADPEEvent;
    
    SC_METHOD(AssertCADDFREQMethod);
    dont_initialize();
    sensitive << mAssertCADDFREQEvent;
    
    SC_METHOD(DeassertCADDFREQMethod);
    dont_initialize();
    sensitive << mDeassertCADDFREQEvent;
    
    SC_METHOD(AssertCADUEMethod);
    dont_initialize();
    sensitive << mAssertCADUEEvent;
    
    SC_METHOD(DeassertCADUEMethod);
    dont_initialize();
    sensitive << mDeassertCADUEEvent;
    
    SC_METHOD(AssertCADLEMethod);
    dont_initialize();
    sensitive << mAssertCADLEEvent;
    
    SC_METHOD(DeassertCADLEMethod);
    dont_initialize();
    sensitive << mDeassertCADLEEvent;
    
    SC_METHOD(CheckFinishMethod);
    dont_initialize();
    sensitive << mUpdateConversionFinishEvent;

    for (unsigned int ch_id = 0; ch_id < CADC_AgentController::emVCNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertCADVCIEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&CADC_UNIT::AssertCADVCIMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertCADVCIMethod"), &opt);
    }
    
    for (unsigned int ch_id = 0; ch_id < CADC_AgentController::emVCNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeassertCADCVCIEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&CADC_UNIT::DeassertCADVCIMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeassertCADVCIMethod"), &opt);
    }
    
    SC_METHOD(HandleCADTRGInputMethod);
    dont_initialize();
    sensitive << CADTRG.neg();
    
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
    sensitive << mACtartEvent;
    
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

    SC_METHOD(WriteCADIMethod);
    dont_initialize();
    sensitive << mWriteCADIEvent;

    SC_METHOD(WriteCADEMethod);
    dont_initialize();
    sensitive << mWriteCADEEvent;

    SC_METHOD(WriteCADPEMethod);
    dont_initialize();
    sensitive << mWriteCADPEEvent;

    SC_METHOD(WriteCADDFREQMethod);
    dont_initialize();
    sensitive << mWriteCADDFREQEvent;

    SC_METHOD(WriteCADUEMethod);
    dont_initialize();
    sensitive << mWriteCADUEEvent;

    SC_METHOD(WriteCADLEMethod);
    dont_initialize();
    sensitive << mWriteCADLEEvent;
   
    for (unsigned int ch_id = 0; ch_id < CADC_AgentController::emVCNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteCADVCIEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&CADC_UNIT::WriteCADVCIMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteCADVCIMethod"), &opt);
    }

    for (unsigned int ch_id = 0; ch_id < CADC_AgentController::emVCNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mUpdateDIREvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&CADC_UNIT::UpdateDIRMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("UpdateDIRMethod"), &opt);
    }
}//}}}

///Destructor of CADC_UNIT class
CADC_UNIT::~CADC_UNIT (void)
{//{{{
    delete mCADC_UNIT_Func;
}//}}}

/// Cancel operation events
void CADC_UNIT::CancelEvents (void)
{//{{{
    mWriteCADENDEvent.cancel();
    mWriteCADDFTAGEvent.cancel();
    mWriteCADDFDATAEvent.cancel();
    mAssertCADIEvent.cancel();
    mDeassertCADIEvent.cancel();
    mAssertCADEEvent.cancel();
    mDeassertCADEEvent.cancel();
    mAssertCADPEEvent.cancel();
    mDeassertCADPEEvent.cancel();
    mAssertCADDFREQEvent.cancel();
    mDeassertCADDFREQEvent.cancel();
    mAssertCADUEEvent.cancel();
    mDeassertCADUEEvent.cancel();
    mAssertCADLEEvent.cancel();
    mDeassertCADLEEvent.cancel();
    for (unsigned int i = 0; i < CADC_AgentController::emVCNum; i++) {
        mAssertCADVCIEvent[i].cancel();
    }
    for (unsigned int i = 0; i < CADC_AgentController::emVCNum; i++) {
        mDeassertCADCVCIEvent[i].cancel();
    }
    mStartTriggerEvent.cancel();
    mEndTriggerEvent.cancel();
    mCalibStartEvent.cancel();
    mCalibStopEvent.cancel();
    mCalibFinishEvent.cancel();
    mACtartEvent.cancel();
    mACtopEvent.cancel();
    mConvertFinishEvent.cancel();
    mConvertStopEvent.cancel();
    mAssertADACTEvent.cancel();
    mDeassertADACTEvent.cancel();
}//}}}

/// Initialize variables & output ports
void CADC_UNIT::Initialize (void)
{//{{{
    // Initialize variables
    mCADEND = false;
    mCADI = false;
    mCADE = false;
    mCADPE = false;
    mCADDFREQ = false;
    mCADDFTAG = 0;
    mCADDFDATA = 0;
    mCADUE = false;
    mCADLE = false;
    for (unsigned int i = 0; i < CADC_AgentController::emVCNum; i++) {
        mCADCVCI[i] = false;
    }
    mIsACtopEvent = false;
    mIsTriggerProcessing = false;
    mIsContinueConvert = false;
    mIsEndTrigger = false;
    
    mCADC_UNIT_Func->Initialize();
    // Write output ports inactive
    mWriteCADENDEvent.notify();
    mWriteCADIEvent.notify();
    mWriteCADEEvent.notify();
    mWriteCADEEvent.notify();
    mWriteCADDFREQEvent.notify();
    mWriteCADDFTAGEvent.notify();
    mWriteCADDFDATAEvent.notify();
    mDeassertCADUEEvent.notify();
    mDeassertCADLEEvent.notify();
    for (unsigned int i = 0; i < CADC_AgentController::emVCNum; i++) {
        mDeassertCADCVCIEvent[i].notify();
    }
    
}//}}}

/// Get analog input values
double CADC_UNIT::GetAnalogValue (const bool is_refh, const unsigned int an_no)
{//{{{
    double ret = 0;
    if (is_refh) {
        ret = ADSVREFH.read();
    } else {
        if (mCADC->mCADC_Func->IsPinDiagEnable()) {
            if (mCADC_UNIT_Func->GetPinDiagBitValue(an_no) == 1) {
                ret = mCADC->mADSVCC;
            } else {
                ret = mCADC->mADSVSS;
            }
        } else {
            ret = AN[an_no]->read();
        }
    }
    return ret;
}//}}}

/// Notify event in CADC_UNIT
void CADC_UNIT::NotifyEvent (const unsigned int event, const unsigned int vc_no)
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
        case  emAssertCADPE :
            mAssertCADPEEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time) + mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
            break;
        case  emAssertCADCVCI :
            mAssertCADVCIEvent[vc_no].notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time), mCADC->mTimeResolutionUnit);
            break;
        case emAssertCADUE :
            mAssertCADUEEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time), mCADC->mTimeResolutionUnit);
            break;
        case emUpdateDIR :
            mUpdateDIREvent[vc_no].notify(SC_ZERO_TIME);
            break;
        case  emCheckFinish :
            mUpdateConversionFinishEvent.notify(SC_ZERO_TIME);
            break;
        default:    // emAssertCADLE
            mAssertCADLEEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time), mCADC->mTimeResolutionUnit);
            break;
    }
}//}}}

/// Write to CADEND output port
void CADC_UNIT::WriteCADENDMethod (void)
{//{{{
    CADEND.write(mCADEND);
}//}}}

/// Write to CADDFTAG output port
void CADC_UNIT::WriteCADDFTAGMethod (void)
{//{{{
    CADDFTAG.write((sc_uint<4>)mCADDFTAG);
}//}}}

/// Write to CADDFDATA output port
void CADC_UNIT::WriteCADDFDATAMethod (void)
{//{{{
    CADDFDATA.write((sc_uint<16>)mCADDFDATA);
}//}}}
/// Write to CADI output port
void CADC_UNIT::WriteCADIMethod (void)
{//{{{
    CADI.write(mCADI);
}//}}}
/// Assert CADI interrupt port
void CADC_UNIT::AssertCADIMethod (void)
{//{{{
    mCADI = true;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADI] Assert.\n");
    }
    mWriteCADIEvent.notify(SC_ZERO_TIME);
    //CADI.write(mCADI);
    mDeassertCADIEvent.notify(mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
}//}}}

/// De-assert CADI interrupt port
void CADC_UNIT::DeassertCADIMethod (void)
{//{{{
    mCADI = false;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADI] De-assert.\n");
    }
    mWriteCADIEvent.notify(SC_ZERO_TIME);
    //CADI.write(mCADI);
}//}}}
void CADC_UNIT::WriteCADEMethod (void)
{//{{{
    CADE.write(mCADE);
}//}}}
/// Assert CADE interrupt port
void CADC_UNIT::AssertCADEMethod (void)
{//{{{
    mCADE = true;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADE] Assert.\n");
    }
    mWriteCADEEvent.notify(SC_ZERO_TIME);
    //CADE.write(mCADE);
    mDeassertCADEEvent.notify(mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
}//}}}

/// De-assert CADE interrupt port
void CADC_UNIT::DeassertCADEMethod (void)
{//{{{
    mCADE = false;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADE] De-assert.\n");
    }
    mWriteCADEEvent.notify(SC_ZERO_TIME);
    //CADE.write(mCADE);
}//}}}
void CADC_UNIT::WriteCADPEMethod (void)
{//{{{
    CADPE.write(mCADPE);
}//}}}
/// Assert CADPE interrupt port
void CADC_UNIT::AssertCADPEMethod (void)
{//{{{
    mCADPE = true;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADPE] Assert.\n");
    }
    mWriteCADPEEvent.notify(SC_ZERO_TIME);
    //CADPE.write(mCADPE);
    mDeassertCADPEEvent.notify(mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
}//}}}

/// De-assert CADPE interrupt port
void CADC_UNIT::DeassertCADPEMethod (void)
{//{{{
    mCADPE = false;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADPE] De-assert.\n");
    }
    mWriteCADPEEvent.notify(SC_ZERO_TIME);
    //CADPE.write(mCADPE);
}//}}}
void CADC_UNIT::WriteCADDFREQMethod (void)
{//{{{
    CADDFREQ.write(mCADDFREQ);
}//}}}
/// Assert CADDFREQ interrupt port
void CADC_UNIT::AssertCADDFREQMethod (void)
{//{{{CADDFTAG
    mCADDFREQ = true;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADDFREQ] Assert.\n");
    }
    mWriteCADDFREQEvent.notify(SC_ZERO_TIME);
    mDeassertCADDFREQEvent.notify(mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
}//}}}

/// De-assert CADDFREQ interrupt port
void CADC_UNIT::DeassertCADDFREQMethod (void)
{//{{{
    mCADDFREQ = false;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADDFREQ] De-assert.\n");
    }
    mWriteCADDFREQEvent.notify(SC_ZERO_TIME);
}//}}}

void CADC_UNIT::WriteCADUEMethod (void)
{//{{{
    CADUE.write(mCADUE);
}//}}}
/// Assert CADUE interrupt port
void CADC_UNIT::AssertCADUEMethod (void)
{//{{{
    mCADUE = true;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADUE] Assert.\n");
    }
    mWriteCADUEEvent.notify(SC_ZERO_TIME);
    mDeassertCADUEEvent.notify(mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
}//}}}

/// De-assert CADUE interrupt port
void CADC_UNIT::DeassertCADUEMethod (void)
{//{{{
    mCADUE = false;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADUE] De-assert.\n");
    }
    mWriteCADUEEvent.notify(SC_ZERO_TIME);
}//}}}
void CADC_UNIT::WriteCADLEMethod (void)
{//{{{
    CADLE.write(mCADLE);
}//}}}
/// Assert CADLE interrupt port
void CADC_UNIT::AssertCADLEMethod (void)
{//{{{
    mCADLE = true;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADLE] Assert.\n");
    }
    mWriteCADLEEvent.notify(SC_ZERO_TIME);
    mDeassertCADLEEvent.notify(mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
}//}}}

/// De-assert CADLE interrupt port
void CADC_UNIT::DeassertCADLEMethod (void)
{//{{{
    mCADLE = false;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADLE] De-assert.\n");
    }
    mWriteCADLEEvent.notify(SC_ZERO_TIME);
}//}}}
void CADC_UNIT::WriteCADVCIMethod (const unsigned int ch_id)
{//{{{
    CADCVCI[ch_id]->write(mCADCVCI[ch_id]);
}//}}}
/// Assert CADVCIj interrupt port
void CADC_UNIT::AssertCADVCIMethod (const unsigned int ch_id)
{//{{{
    mCADCVCI[ch_id] = true;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADCVCI%d] Assert.\n", ch_id);
    }
    mWriteCADVCIEvent[ch_id].notify(SC_ZERO_TIME);
    mDeassertCADCVCIEvent[ch_id].notify(mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
}//}}}

/// De-assert CADVCIj interrupt port
void CADC_UNIT::DeassertCADVCIMethod (const unsigned int ch_id)
{//{{{
    mCADCVCI[ch_id] = false;
    if (mCADC->mDumpInterrupt) {
        re_printf("info","INT [CADCVCI%d] De-assert.\n", ch_id);
    }
    mWriteCADVCIEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

/// Handle External trigger
void CADC_UNIT::HandleCADTRGInputMethod (void)
{//{{{
    if (!mCADC->GetResetStatus("resetad_n") && mCADC->CheckClockPeriod("CLK_HSB")) {
        switch(mCADC_UNIT_Func->TriggerType()) {
            case 3 :
            case 2 :
                // End trigger
                mEndTriggerEvent.notify(SC_ZERO_TIME);
                break;
            case 1 :
                // Start trigger
                mStartTriggerEvent.notify(SC_ZERO_TIME);
                break;
            default:
                break;
        }
    }
}//}}}

/// Process Start trigger
void CADC_UNIT::StartTriggerMethod (void)
{//{{{
    if (!mCADC->GetResetStatus("resetad_n") && mCADC->CheckClockPeriod("CLK_HSB")) {
        if (!mIsTriggerProcessing) {
            if (mCADC_UNIT_Func->GetStatusFlag(true)) {
                // ADACT = 1
                if (mCADC_UNIT_Func->GetVCEndPointer() > 0) {
                    if (mCADC_UNIT_Func->CheckSetting(true)) {
                        mIsContinueConvert = true;
                        mIsACtopEvent = true;
                        mACtopEvent.notify(SC_ZERO_TIME);
                    }
                }
            } else {
                if (mCADC_UNIT_Func->CheckSetting(false)) {
                    if (mCADC_UNIT_Func->GetStatusFlag(false)) {
                        // CLBACT = 1
                        mCalibStopEvent.notify(SC_ZERO_TIME);
                    }
                    mACtartEvent.notify(SC_ZERO_TIME);
                }
            }
        }
    }
}//}}}

/// Process End trigger
void CADC_UNIT::EndTriggerMethod (void)
{//{{{
    if (!mCADC->GetResetStatus("resetad_n") && mCADC->CheckClockPeriod("CLK_HSB")) {
        if (!mIsTriggerProcessing) {
            if (mCADC_UNIT_Func->GetStatusFlag(true)) {
                // ADACT = 1
                mIsEndTrigger = true;
                if (mIsContinueConvert) {
                    mACtartEvent.cancel();
                    mDeassertADACTEvent.notify(SC_ZERO_TIME);
                    mConvertStopEvent.notify(SC_ZERO_TIME);
                } else {
                    mIsACtopEvent = true;
                    mACtopEvent.notify(SC_ZERO_TIME);
                }
            }
        }
    }
}//}}}

/// Start Calibration processing
void CADC_UNIT::CalibStartMethod (void)
{//{{{
    if (!mCADC->GetResetStatus("resetad_n") && mCADC->CheckClockPeriod("CLK_HSB")) {
        if (!mCADC_UNIT_Func->GetStatusFlag(true) && !mCADC_UNIT_Func->GetStatusFlag(false) && !mIsTriggerProcessing) {
            // ADACT = 0 & CLBACT = 0
            // Set CLBACT to 1
            mCADC_UNIT_Func->SetStatusFlag(false, true);
            // Wait for Calibration finish
            mCalibFinishEvent.notify((mCADC->mtCLB * mCADC->mCLK_LSBPeriod), mCADC->mTimeResolutionUnit);
        }
    }
}//}}}

/// Stop Calibration processing
void CADC_UNIT::CalibStopMethod (void)
{//{{{
    if (!mCADC->GetResetStatus("resetad_n") && mCADC->CheckClockPeriod("CLK_HSB")) {
        // Cancel Calibration process
        mCalibFinishEvent.cancel();
        // Clear CLBACT to 0
        mCADC_UNIT_Func->SetStatusFlag(false, false);
    }
}//}}}

/// Calibration processing is finished
void CADC_UNIT::CalibFinishMethod (void)
{//{{{
    if (!mCADC->GetResetStatus("resetad_n") && mCADC->CheckClockPeriod("CLK_HSB")) {
        // Clear CLBACT to 0
        mCADC_UNIT_Func->SetStatusFlag(false, false);
        mCADC_UNIT_Func->UpdateCalibResult();
    }
}//}}}

/// A/D conversion processing
void CADC_UNIT::ADProcessingThread (void)
{//{{{
    for(;;) {
        if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
        (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {
            wait();
            continue;
        }
        try {
            while (true) {
                mIsACtopEvent = false;
                bool is_start = false;
                double current_time = 0;
                if (!mIsContinueConvert) {
                    mIsTriggerProcessing = true;
                    // Wait ADACT (internal) sync time
                    current_time = sc_time_stamp().to_double();
                    wait((mCADC->CalculateClockEdge("CLK_HSB", true, current_time) + 3*mCADC->mCLK_HSBPeriod), mCADC->mTimeResolutionUnit,
                        mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] |
                        mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0]);
                    if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
                    (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {
                        throw vpcl::emRstException;
                    }
                    // Assert CADEND
                    mCADEND = true;
                    mWriteCADENDEvent.notify(SC_ZERO_TIME);
                    // Assert CADnACR.ADACT
                    current_time = sc_time_stamp().to_double();
                    mAssertADACTEvent.notify((mCADC->CalculateClockEdge("CLK_LSB", true, current_time) + 2*mCADC->mCLK_LSBPeriod), mCADC->mTimeResolutionUnit);
                    // Wait sync time 
                    if (mCADC->mCLK_HSBPeriod <= (mCADC->CalculateClockEdge("CYCLK16M", false, current_time))){
                        wait((mCADC->CalculateClockEdge("CYCLK16M", false, current_time) - mCADC->mCLK_HSBPeriod), mCADC->mTimeResolutionUnit,
                            mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] |
                            mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0] | mACtopEvent);
                    }else{
                        wait((mCADC->CalculateClockEdge("CYCLK16M", false, current_time) - mCADC->mCLK_HSBPeriod) + mCADC->mM16Period, mCADC->mTimeResolutionUnit, mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] | mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0] | mACtopEvent);
                    }
                    if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
                    (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {

                        throw vpcl::emRstException;
                    }
                }
                mIsContinueConvert = false;
                
                if (!mIsACtopEvent) {
                    // Enter Internal stability period
                    re_printf("info","%s enters Internal stability period.\n", mModuleName.c_str());
                    // Wait internal stabilization time
                    wait((mCADC->mtW * mCADC->mCLK_LSBPeriod), mCADC->mTimeResolutionUnit,
                        mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] |
                        mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0] | mACtopEvent);
                    if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
                    (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {
                        throw vpcl::emRstException;
                    }
                    
                    if (!mIsACtopEvent) {
                        // Enter Converting state
                        re_printf("info","%s starts AD conversion.\n", mModuleName.c_str());
                        bool is_first_convert = true;
                        is_start = true;
                        while (true) {
                            // Wait Conversion time
                            if (is_first_convert) {
                                wait(CaltDF(), mCADC->mTimeResolutionUnit,
                                    mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] |
                                    mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0] | mACtopEvent);
                            } else {
                                wait(CaltS() , mCADC->mTimeResolutionUnit,
                                    mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] |
                                    mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0] | mACtopEvent);
                            }
                            if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
                            (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {
                                throw vpcl::emRstException;
                            }
                            if (mIsACtopEvent) {
                                break;
                            }
                            is_first_convert = false;
                            mConvertFinishEvent.notify(SC_ZERO_TIME);
                        }
                    }
                }
                
                /// AD processing is stopped
                mIsTriggerProcessing = true;
                // Wait ADACT (internal) sync time
                current_time = sc_time_stamp().to_double();

                wait((mCADC->CalculateClockEdge("CLK_HSB", true, current_time) + 3*mCADC->mCLK_HSBPeriod), mCADC->mTimeResolutionUnit,
                      mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] |
                      mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0]);
                if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
                (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {
                    throw vpcl::emRstException;
                }
                mIsTriggerProcessing = false;
                if (mIsContinueConvert) {
                    mACtartEvent.notify(mCADC->mtR * mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
                } else {
                    // Deassert CADEND
                    mCADEND = false;
                    mWriteCADENDEvent.notify(SC_ZERO_TIME);
                    // Deassert CADnACR.ADACT
                    current_time = sc_time_stamp().to_double();
                    mDeassertADACTEvent.notify((mCADC->CalculateClockEdge("CLK_LSB", true, current_time) + 2*mCADC->mCLK_LSBPeriod), mCADC->mTimeResolutionUnit);

                    // Wait for CADC stop 
                    if (mCADC->mCLK_HSBPeriod <= (mCADC->CalculateClockEdge("CYCLK16M", false, current_time))){
                        wait((mCADC->CalculateClockEdge("CYCLK16M", false, current_time) - mCADC->mCLK_HSBPeriod), mCADC->mTimeResolutionUnit,
                            mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] |
                            mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0]);
                    }else{
                        wait((mCADC->CalculateClockEdge("CYCLK16M", false, current_time) - mCADC->mCLK_HSBPeriod) + mCADC->mM16Period, mCADC->mTimeResolutionUnit, mCADC->mResetHardEvent[1] | mCADC->mResetCmdEvent[1] | mCADC->mResetHardEvent[0] | mCADC->mResetCmdEvent[0] | mCADC->mClkZeroEvent[1] | mCADC->mClkZeroEvent[0]);
                    }
                    if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
                    (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {
                        throw vpcl::emRstException;
                    }
                }
                if (is_start) {
                    re_printf("info","%s stops AD conversion.\n", mModuleName.c_str());
                }
                mConvertStopEvent.notify(SC_ZERO_TIME);
                wait();
                if (mCADC->GetResetStatus("resetad_n") || mCADC->GetResetStatus("PRESETn") ||
                (!mCADC->CheckClockPeriod("CLK_HSB")) || (!mCADC->CheckClockPeriod("CLK_LSB"))) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }
}//}}}

/// When each A/D conversion finished
void CADC_UNIT::ConvertFinishMethod (void)
{//{{{
    // Update DIR, UDIR, ER registers
    mCADC_UNIT_Func->UpdateConversionResult();
}//}}}

/// call when conversion Result updated
void CADC_UNIT::CheckFinishMethod (void)
{//{{{
    // Assert CADI, CADE interrupts
    bool dsadrgt = CADRGT.read();
    double current_time = sc_time_stamp().to_double();
    if (mCADC_UNIT_Func->IsADInterruptEnable(dsadrgt)) {
        mAssertCADIEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time) + mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
    }
    if (mCADC_UNIT_Func->IsErrorInterrupt()) {
        mAssertCADEEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time) + mCADC->mCLK_LSBPeriod, mCADC->mTimeResolutionUnit);
    }
    
    // DFE control
    unsigned int dfreq = 0;
    if (mCADC_UNIT_Func->IsDFEEntry(mCADDFTAG, mCADDFDATA)) {
        mAssertCADDFREQEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time), mCADC->mTimeResolutionUnit);
        mWriteCADDFTAGEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time), mCADC->mTimeResolutionUnit);
        mWriteCADDFDATAEvent.notify(mCADC->CalculateClockEdge("CLK_LSB", true, current_time), mCADC->mTimeResolutionUnit);
    }
}//}}}

/// AD conversion stop processing
void CADC_UNIT::ConvertStopMethod (void)
{//{{{
    // Update VCPTR
    mCADC_UNIT_Func->UpdateVCPointer(mIsEndTrigger);
    mIsEndTrigger = false;
}//}}}

/// Assert CADnACR.ADACT bit
void CADC_UNIT::AssertADACTMethod (void)
{//{{{
    mCADC_UNIT_Func->SetStatusFlag(true, true);
    mIsTriggerProcessing = false;
}//}}}

/// De-assert CADnACR.ADACT bit
void CADC_UNIT::DeassertADACTMethod (void)
{//{{{
    mCADC_UNIT_Func->SetStatusFlag(true, false);
    mIsTriggerProcessing = false;
}//}}}

/// Calculate tS value
double CADC_UNIT::CaltS (void)
{//{{{
    double tS;
    if (mCADC_UNIT_Func->GetAver() == 0){
        tS = (mCADC->mtSNotAver * mCADC->mCLK_LSBPeriod);
    }else{
        tS = (mCADC->mtSAver * mCADC->mCLK_LSBPeriod);
    }    
    return tS;
}//}}} 

/// Calculate tDF value
double CADC_UNIT::CaltDF (void)
{//{{{
    double tDF;
    if (mCADC_UNIT_Func->GetAver() == 0){
        tDF = (mCADC->mtDFNotAver * mCADC->mCLK_LSBPeriod);
    }else{
        tDF = (mCADC->mtDFAver * mCADC->mCLK_LSBPeriod);
    }    
    return tDF;
}//}}}

/// Call method update DIR register
void CADC_UNIT::UpdateDIRMethod(unsigned int ch_id)
{//{{{
    // call update method of register here
    mCADC_UNIT_Func->UpdateDIR(ch_id);
}//}}}

// vim600: set foldmethod=marker :
