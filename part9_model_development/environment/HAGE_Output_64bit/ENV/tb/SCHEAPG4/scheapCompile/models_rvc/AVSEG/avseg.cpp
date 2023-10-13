// ---------------------------------------------------------------------
// $Id: $
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
#include <string>
#include "avseg.h"
#include "avseg_core.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

#define re_sc_assert(expr, msg)     ((void) ((expr) ? 0 : (SC_REPORT_FATAL( sc_core::SC_ID_ASSERTION_FAILED_ , msg), 0)))

/*********************************
// Function     : Cavseg
// Description  : Constructor of Cavseg class
// Parameter    :
//      name            Module name
// Return value : None
**********************************/
Cavseg::Cavseg(sc_module_name name):
                   sc_module(name),
                   vpcl::tlm_tgt_if<32> (name),
                   CLK_LSB("CLK_LSB"),
                   PRESETn("PRESETn"),
                   vculmo("vculmo"),
                   vcllmo("vcllmo"),
                   evccshdet("evccshdet"),
                   evccsldet("evccsldet"),
                   vccshdet("vccshdet"),
                   vccsldet("vccsldet"),
                   awovddshdet("awovddshdet"),
                   awovddsldet("awovddsldet"),
                   isovddshdet("isovddshdet"),
                   isovddsldet("isovddsldet"),
                   // Attributes
                   TARGET_PRODUCT("TARGET_PRODUCT","AVSEG_U2A"),
                   VCRnum0("VCRnum0", "")
{//{{{
    std::ostringstream port_name;
    for (unsigned int i = 0; i < emADCHVCNum; i++) {
        port_name.str(""); port_name << "vcend" << i;
        vcend[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(vcend[i] != NULL);
    }

    mAVSEGcore = new Cavseg_core((std::string)name, this);
    sc_assert(mAVSEGcore != NULL);

    evccshdet.initialize(false);
    evccsldet.initialize(false);
    vccshdet.initialize(false);
    vccsldet.initialize(false);
    awovddshdet.initialize(false);
    awovddsldet.initialize(false);
    isovddshdet.initialize(false);
    isovddsldet.initialize(false);

    //Initial variables-------------------------
    mADC0_vir_num = 64;
    mCLK_LSBPeriod = 0;
    mCLK_LSBFreq = 0;
    mCLK_LSBOrgFreq = 0;
    mCLK_LSBFreqUnit = "Hz";
    mTimeCLKChangePoint = 0;

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    TargetConfig TempTargetConfigTable[emTargetConfigNum] =  { {"AVSEG_U2A"      ,64}
                                                              ,{"AVSEG_E2x"      ,40} 
                                                             };

    // copy TempTargetConfig
    for (unsigned int i=0;i<emTargetConfigNum;i++) {
        mTargetConfigTable[i] = TempTargetConfigTable[i];
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }

    this->Initialize();
    //------------------------------------------
    SC_METHOD(UpdateTargetProductMethod);

    SC_METHOD(HandleCLK_LSBSignalMethod);
    dont_initialize();
    sensitive << CLK_LSB;

    SC_METHOD(HandlePRESETnSignalMethod);//initialized at beginning
    sensitive << PRESETn;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cavseg::HandleResetHardMethod, this, reset_id), sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cavseg::HandleResetCmdMethod, this, reset_id), sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cavseg::CancelResetCmdMethod, this, reset_id), sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
    for (unsigned int chn_id = 0; chn_id < emADCHVCNum; chn_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(vcend[chn_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cavseg::HandleADEndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleADEndInputMethod"), &opt);
    }
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNotifyFilterControlEvent[chn_sel]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cavseg::NotifyFilterControlMethod, this, chn_sel), sc_core::sc_gen_unique_name("NotifyFilterControlMethod"), &opt);
    }
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mNotifyErrPulseDoneEvent[chn_sel]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cavseg::NotifyErrPulseDoneMethod, this, chn_sel), sc_core::sc_gen_unique_name("NotifyErrPulseDoneMethod"), &opt);
    }
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteErrPulseEvent[chn_sel]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cavseg::WriteErrPulseMethod, this, chn_sel), sc_core::sc_gen_unique_name("WriteErrPulseMethod"), &opt);
    }
    
    SetLatency_TLM(true);
}//}}}

/*********************************
// Function     : ~Cavseg
// Description  : Destructor of Cavseg class
// Parameter    : None
// Return value : None
**********************************/
Cavseg::~Cavseg(void)
{//{{{
    for (unsigned int i = 0; i < emADCHVCNum; i++) {
        delete vcend[i];
    }
    delete mAVSEGcore;
}//}}}

/*********************************
// Function     : AssertReset
// Description  : Reset the model
//                This method is called by Command IF
// Parameter    :
//      reset_name          Reset name
//      start_time          Start time
//      period              Reset period
// Return value : None
**********************************/
void Cavseg::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETn])&&(!mIsResetCmdReceive[emPRESETn])) {
            mIsResetCmdReceive[emPRESETn] = true;
            re_printf("info","The model will be reset (PRESETn) for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) start_time, FRACTION_TO_INT(start_time));
            mResetCmdEvent[emPRESETn].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETn] = period;
        } else {
            re_printf("warning","The AssertReset command of PRESETn is called in the reset operation (PRESETn) of the model. So this calling is ignored.\n");
        }
    } else {
        re_printf("warning", "The name of reset signal is wrong. It should be PRESETn.\n");
    }
}//}}}



/*********************************
// Function     : SetCLKFreq
// Description  : Set clock frequency to the model
//                This method is called by Command IF
// Parameter    :
//      clock_name          Clock name
//      freq                Frequency setting
//      unit                Frequency unit setting
// Return value : None
**********************************/
void Cavseg::SetCLKfreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    if (clock_name == "CLK_LSB") {
        ConvertClockFreq (mCLK_LSBFreq, mCLK_LSBFreqUnit, freq, unit);
        mCLK_LSBOrgFreq = freq;
        if (mCLK_LSBFreq > 0) {
            mCLK_LSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_LSBFreq)*(double)mTimeResolutionValue) + 0.5);
        } else {
            mCLK_LSBPeriod = 0;
            CancelEvents();
        }
        mTimeCLKChangePoint = GetCurTime();
        SetLatency_TLM(false);
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

/*********************************
// Function     : GetCLKFreq
// Description  : Dump information for clock frequency from the model
//                This method is called by Command IF
// Parameter    :
//      clock_name          Clock name
// Return value : None
**********************************/
void Cavseg::GetCLKfreq (const std::string clock_name)
{//{{{
    if (clock_name == "CLK_LSB") {
        re_printf("info","CLK_LSB frequency is %0.0f %s.\n", (double)mCLK_LSBOrgFreq, mCLK_LSBFreqUnit.c_str());
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

/*********************************
// Function     : RegisterHandleCommand
// Description  : Set parameters or commands related to register I/F to AVSEG_Core
//                This method is called by Command IF
// Parameter    :
//      args            Argument of parameter/command
// Return value : Register dumping message
**********************************/
std::string Cavseg::RegisterHandleCommand(const std::vector<std::string>& args)
{//{{{
    return mAVSEGcore->RegisterHandler(args);
}//}}}

/*********************************
// Function     : tgt_acc_dbg
// Description  : Access memory in debug mode via TLM target socket
//                This is virtual function of tlm_tgt_if
// Parameter    :
//      id                  TLM target socket ID
//      trans               Transaction
// Return value : None
**********************************/
unsigned int Cavseg::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    //Read access
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        status = mAVSEGcore->read(true, (unsigned int)addr, p_data, size);
    //Write access    
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = mAVSEGcore->write(true, (unsigned int)addr, p_data, size);
    //No read/write access    
    } else {
        status = true;
        size = 0;
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE); 
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

/*********************************
// Function     : tgt_acc
// Description  : Access memory in normal mode via TLM target socket
//                This is virtual function of tlm_tgt_if
// Parameter    :
//      id                  TLM target socket ID
//      trans               Transaction
// Return value : None
**********************************/
void Cavseg::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr    = 0;
    unsigned char *p_data  = NULL;
    unsigned int size = 0; 
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if(!status){
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    //Read access
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        status = mAVSEGcore->read(false, (unsigned int)addr, p_data, size);
    //Write access 
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = mAVSEGcore->write(false, (unsigned int)addr, p_data, size);
    //No write/read access    
    } else {
        status = true;
    }
    trans.set_response_status(tlm::TLM_OK_RESPONSE); 
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initializes internal variables & output ports
// Parameter    : None
// Return value : None
**********************************/
void Cavseg::Initialize (void)
{//{{{
    for (unsigned int i_chan = 0; i_chan < emInputChannelNum; i_chan++) {
        mEndPulse[i_chan] = false;
        mUpperErr[i_chan] = false;
        mLowerErr[i_chan] = false;
        mUpperErrOut[i_chan] = false;
        mLowerErrOut[i_chan] = false;
    }
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel all operation events
// Parameter    : None
// Return value : None
**********************************/
void Cavseg::CancelEvents (void)
{//{{{
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        mNotifyFilterControlEvent[chn_sel].cancel();
        mNotifyErrPulseDoneEvent[chn_sel].cancel();
        mWriteErrPulseEvent[chn_sel].cancel();
    }
}//}}}

/*********************************
// Function     : ResetOutputPort
// Description  : Reset output ports when reset is active
// Parameter    : None
// Return value : None
**********************************/
void Cavseg::ResetOutputPort (void)
{//{{{
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        mUpperErrOut[chn_sel] = false;
        mLowerErrOut[chn_sel] = false;
        mWriteErrPulseEvent[chn_sel].notify(SC_ZERO_TIME);
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Process reset function
// Parameter    :
//      is_active           Reset is active
//      reset_id            Reset ID
// Return value : None
**********************************/
void Cavseg::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    mAVSEGcore->EnableReset(is_active, reset_id);
    if (is_active) {
        CancelEvents();
        ResetOutputPort();
        this->Initialize();
    }
}//}}}

/*********************************
// Function     : NotifyErrPulseOut
// Description  : Notify Upper/Lower error pulse output
// Parameter    :
//      chn_sel             Input channel selector
//      is_upper            true: upper error pulse output
//      is_lower            true: lower error pulse output
// Return value : None
**********************************/
void Cavseg::NotifyErrPulseOut(const unsigned int chn_sel, const unsigned int ex_bound)
{//{{{
    if (ex_bound == 1) {
        mUpperErrOut[chn_sel] = true;
        mLowerErrOut[chn_sel] = false;
    } else {// if (ex_bound == 2)
        mLowerErrOut[chn_sel] = true;
        mUpperErrOut[chn_sel] = false;
    }
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB")) {
            double endpulse_period = CalculateClockEdge((double)mCLK_LSBPeriod, true, true, GetCurTime(), mTimeCLKChangePoint);
            mWriteErrPulseEvent[chn_sel].notify(SC_ZERO_TIME);
            mNotifyErrPulseDoneEvent[chn_sel].notify(endpulse_period, mTimeResolutionUnit);
            mAVSEGcore->UpdateErrorRegister(chn_sel, mUpperErrOut[chn_sel], mLowerErrOut[chn_sel]);
        }
    }
}//}}}

/*********************************
// Function     : HandleCLK_LSBSignalMethod
// Description  : Handle the change of CLK_LSB signal
// Parameter    : None
// Return value : None
**********************************/
void Cavseg::HandleCLK_LSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_LSB.read();
    SetCLKfreq("CLK_LSB", freq_value, "Hz");
}//}}}

/*********************************
// Function     : HandlePRESETnSignalMethod
// Description  : Handle the change of PRESETn signal
// Parameter    : None
// Return value : None
**********************************/
void Cavseg::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[emPRESETn] = PRESETn.read();
    if (mResetCurVal[emPRESETn] != mResetPreVal[emPRESETn]) {
        mResetPreVal[emPRESETn] = mResetCurVal[emPRESETn];
        if (mCLK_LSBPeriod > 0) {
            mResetHardEvent[emPRESETn].notify(CalculateClockEdge((double)mCLK_LSBPeriod, true, false, GetCurTime(), mTimeCLKChangePoint), mTimeResolutionUnit);
        } else {
            mResetHardEvent[emPRESETn].notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : HandleResetHardMethod
// Description  : Process reset function when reset port is changed
// Parameter    :
//      reset_id            Reset ID
// Return value : None
**********************************/
void Cavseg::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    bool is_cancel_assertreset = false;
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETn) {
        reset_name = "PRESETn";
    } //else other reset_id if any
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        re_printf("info","The reset signal of %s is asserted.\n", reset_name.c_str());
        this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        is_cancel_assertreset = true;
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
            re_printf("info","The reset signal of %s is negated.\n", reset_name.c_str());
            is_cancel_assertreset = true;
        }
    }
    //Cancel AssertReset command when PRESETn is active
    if (mIsResetCmdReceive[reset_id] && is_cancel_assertreset) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

/*********************************
// Function     : HandleResetCmdMethod
// Description  : Process reset function when reset command is called
// Parameter    :
//      reset_id            Reset ID
// Return value : None
**********************************/
void Cavseg::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETn) {
        reset_name = "PRESETn";
    } //else other reset_id if any
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

/*********************************
// Function     : CancelResetCmdMethod
// Description  : Cancel reset function when reset command is processed completely
// Parameter    :
//      reset_id            Reset ID
// Return value : None
**********************************/
void Cavseg::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == 0) {
        reset_name = "PRESETn";
    } //else other reset_id if any
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id); 
}//}}}

/*********************************
// Function     : HandleADEndInputMethod
// Description  : Handle the change in vcend[chan_num] input port
// Parameter    :
//      chan_num            ADCH virtual channel number
// Return value : None
**********************************/
void Cavseg::HandleADEndInputMethod (const unsigned int chan_num)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (chan_num >= mADC0_vir_num) {
            re_printf("warning","Product %s: The selected channel %d is exceeded the product supported %d channels. Channel is invalid\n", TARGET_PRODUCT.value.c_str(), chan_num, mADC0_vir_num);
        } else {
            if (CheckClockPeriod("CLK_LSB")) {
                unsigned int same_setting = 0;
                bool end_pulse = vcend[chan_num]->read();
                bool upper_error = vculmo.read();
                bool lower_error = vcllmo.read();
                double endpulse_period = CalculateClockEdge((double)mCLK_LSBPeriod, true, true, GetCurTime(), mTimeCLKChangePoint);
                for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
                    if (chan_num == (mAVSEGcore->GetChannelSelect(chn_sel))) {
                        mEndPulse[chn_sel] = end_pulse;
                        mUpperErr[chn_sel] = upper_error;
                        mLowerErr[chn_sel] = lower_error;
                        if (end_pulse) {
                            if (upper_error && lower_error) {
                                re_printf("warning","The input of both upper and lower pulses into the same module at the same time is prohibited.\n");
                            }
                            mNotifyFilterControlEvent[chn_sel].notify(endpulse_period, mTimeResolutionUnit);
                        }
                        same_setting += 1;
                    }
                }
                if (same_setting > 1) {
                    re_printf("info", "There are %d same input channel selectors (CHS bit = 0x%X).\n", same_setting, chan_num);
                }
            }
        }
    }
}//}}}

/*********************************
// Function     : NotifyFilterControlMethod
// Description  : Notify to Filter Control after receive AD end pulse
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector: emVCC,emEVCC,emVDD)
// Return value : None
**********************************/
void Cavseg::NotifyFilterControlMethod (const unsigned int chn_sel)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB")) {
            mAVSEGcore->NotifyFilterControl(chn_sel, mUpperErr[chn_sel], mLowerErr[chn_sel]);
        }
    }
}//}}}

/*********************************
// Function     : NotifyErrPulseDoneMethod
// Description  : Notify to error pulse output is done
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector: emVCC,emEVCC,emVDD)
// Return value : None
**********************************/
void Cavseg::NotifyErrPulseDoneMethod (const unsigned int chn_sel)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB")) {
            mUpperErrOut[chn_sel] = false;
            mLowerErrOut[chn_sel] = false;
            mWriteErrPulseEvent[chn_sel].notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : WriteErrPulseMethod
// Description  : Write error pulse output
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector: emVCC,emEVCC,emVDD)
// Return value : None
**********************************/
void Cavseg::WriteErrPulseMethod (const unsigned int chn_sel)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB")) {
            if (chn_sel == emVCC) {
                vccshdet.write(mUpperErrOut[emVCC]);
                vccsldet.write(mLowerErrOut[emVCC]);
            } else if (chn_sel == emEVCC) {
                evccshdet.write(mUpperErrOut[emEVCC]);
                evccsldet.write(mLowerErrOut[emEVCC]);
            } else if (chn_sel == emAWOVDD) {
                awovddshdet.write(mUpperErrOut[emAWOVDD]);
                awovddsldet.write(mLowerErrOut[emAWOVDD]);
            } else {
                isovddshdet.write(mUpperErrOut[emISOVDD]);
                isovddsldet.write(mLowerErrOut[emISOVDD]);
            }
        }
    }
}//}}}

/*********************************
// Function     : CheckClockPeriod
// Description  : Check clock period
// Parameter    :
//      clock_name          Clock name
// Return value : true: clock period > 0; false: clock period = 0
**********************************/
bool Cavseg::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool status = false;
    if (clock_name == "CLK_LSB") {
        if (mCLK_LSBPeriod > 0) {
            status = true;
        }
    } //else other clock if any
    if (!status) {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return status;
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit depend on mTimeResolutionValue)
**********************************/
double Cavseg::GetCurTime(void)
{//{{{
    return sc_time_stamp().to_double();
}//}}}

/*********************************
// Function     : CalculateClockEdge
// Description  : Calculate clock synchronous time
// Parameter    :
//      clock_period        Clock period (unit depend on mTimeResolutionValue)
//      is_pos              true: raising edge; false: falling edge
//      add_period          true: 0 < syn_time <= 1; false: 0 <= syn_time < 1
//      time_point          Time point
//      time_change_point   Time point clock is updated
// Return value : The duration from time_point to next valid clock edge
**********************************/
double Cavseg::CalculateClockEdge (const double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
{//{{{
    double period_num = 0;
    period_num = (time_point - time_change_point) / clock_period;
    unsigned int mod_period_num = (unsigned int)period_num;
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

/*********************************
// Function     : ConvertClockFreq
// Description  : Convert frequency value and frequency unit
// Parameter    :
//      freq_out            Frequency value (output) (unit: Hz)
//      unit_out            Frequency unit (output)
//      freq_in             Frequency value (input) (unit: unit_in)
//      unit_in             Frequency unit (input)
// Return value : None
**********************************/
void Cavseg::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, const sc_dt::uint64 freq_in, const std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {//Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong; frequency unit (Hz) is set as default.\n", unit_in.c_str());
    }
}//}}}


/*********************************
// Function     : GetTimeResolution
// Description  : Get simulation time resolution
//      resolution_value    Resolution value
//      resolution_unit     Resolution unit
// Return value : None
**********************************/
void Cavseg::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}


void Cavseg::SetLatency_TLM(const bool is_constructor)
{//{{{
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock((double)mCLK_LSBPeriod,(sc_time_unit)mTimeResolutionUnit);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}

/*********************************
// Function     : UpdateTargetProduct
// Description  : Update target product
//      mTargetProduct    TARGET_PRODUCT
// Return value : None
**********************************/
void Cavseg::UpdateTargetProductMethod(void)
{//{{{
    unsigned int found_index = 0;
//    unsigned int mADC0_vir_num = emADCHVCNum;

    re_printf("info","TARGET_PRODUCT is %s .\n", TARGET_PRODUCT.value.c_str());

    for(found_index = 0; found_index < emTargetConfigNum; found_index++) {
        if (TARGET_PRODUCT.value == mTargetConfigTable[found_index].target_product_name) {
            break;
        }
    }

    if (found_index < emTargetConfigNum) { // target product name found
        mADC0_vir_num = mTargetConfigTable[found_index].virtual_ch_num0;
    } else { // for custom configuration
        // already set in initial variables
    }

    // Use defined value if it configured
    if (VCRnum0.value != "") {
        mADC0_vir_num = (unsigned int) strtoul(VCRnum0.value.c_str(),NULL,0);
    }
    if (TARGET_PRODUCT.value == "AVSEG_E2x") {
        re_sc_assert(mADC0_vir_num <= emVCRnum_Max_E2 , "The number of virtual channels of ADC0 is greater than 40");
    } else {
        re_sc_assert(mADC0_vir_num <= emVCRnum_Max_U2 , "The number of virtual channels of ADC0 is greater than 64");
    }
    this->mAVSEGcore->mTargetProduct = TARGET_PRODUCT.value;
    this->mAVSEGcore->mVCENDNum = mADC0_vir_num;

}//}}}
// vim600: set foldmethod=marker : 
