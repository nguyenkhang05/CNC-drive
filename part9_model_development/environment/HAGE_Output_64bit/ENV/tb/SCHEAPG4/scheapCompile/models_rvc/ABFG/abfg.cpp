// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 - 2017 Renesas Electronics Corporation
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <string>
#include "abfg.h"
#include "abfg_core.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

/*********************************
// Function     : Cabfg
// Description  : Constructor of Cabfg class
// Parameter    :
//      name            Module name
// Return value : None
**********************************/
Cabfg::Cabfg(sc_module_name name): sc_module(name)
                   ,vpcl::tlm_tgt_if<32> (name)
                   , CLK_LSB("CLK_LSB")
                   , PRESETn("PRESETn")
                   , vculmo_0("vculmo_0")
                   , vcllmo_0("vcllmo_0")
                   , vculmo_1("vculmo_1")
                   , vcllmo_1("vcllmo_1")
                   , vculmo_2("vculmo_2")
                   , vcllmo_2("vcllmo_2")
                   , vculmo_3("vculmo_3")
                   , vcllmo_3("vcllmo_3")
                   , CADUE00("CADUE00")
                   , CADLE00("CADLE00")
                   , DSADUE00("DSADUE00")
                   , DSADLE00("DSADLE00")
                   , DSADUE10("DSADUE10")
                   , DSADLE10("DSADLE10")
                   , DSADUE20("DSADUE20")
                   , DSADLE20("DSADLE20")
                   , DSADUE12("DSADUE12")
                   , DSADLE12("DSADLE12")
                   , DSADUE13("DSADUE13")
                   , DSADLE13("DSADLE13")
                   , DSADUE11("DSADUE11")
                   , DSADLE11("DSADLE11")
                   , DSADUE14("DSADUE14")
                   , DSADLE14("DSADLE14")
                   , DSADUE15("DSADUE15")
                   , DSADLE15("DSADLE15")
                   , DSADUE21("DSADUE21")
                   , DSADLE21("DSADLE21")
                   , DSADUE22("DSADUE22")
                   , DSADLE22("DSADLE22")
{//{{{
    std::ostringstream port_name;
    for (unsigned int i = 0; i < emADCHVCNum; i++) {
        port_name.str(""); port_name << "vcend_0" << i;
        vcend_0[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(vcend_0[i] != NULL);
        port_name.str(""); port_name << "vcend_1" << i;
        vcend_1[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(vcend_1[i] != NULL);
        port_name.str(""); port_name << "vcend_2" << i;
        vcend_2[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(vcend_2[i] != NULL);
        port_name.str(""); port_name << "vcend_3" << i;
        vcend_3[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(vcend_3[i] != NULL);
    }
    for (unsigned int i = 0; i < emCADCVCNum; i++) {
        port_name.str(""); port_name << "CADCVCI00" << i;
        CADCVCI00[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(CADCVCI00[i] != NULL);
    }
    for (unsigned int i = 0; i < emDSADCVCNum; i++) {
        port_name.str(""); port_name << "DSADVCI00" << i;
        DSADVCI00[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI00[i] != NULL);
        port_name.str(""); port_name << "DSADVCI10" << i;
        DSADVCI10[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI10[i] != NULL);
        port_name.str(""); port_name << "DSADVCI20" << i;
        DSADVCI20[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI20[i] != NULL);
        port_name.str(""); port_name << "DSADVCI12" << i;
        DSADVCI12[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI10[i] != NULL);
        port_name.str(""); port_name << "DSADVCI13" << i;
        DSADVCI13[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI13[i] != NULL);
        port_name.str(""); port_name << "DSADVCI11" << i;
        DSADVCI11[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI11[i] != NULL);
        port_name.str(""); port_name << "DSADVCI14" << i;
        DSADVCI14[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI14[i] != NULL);
        port_name.str(""); port_name << "DSADVCI15" << i;
        DSADVCI15[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI15[i] != NULL);
        port_name.str(""); port_name << "DSADVCI21" << i;
        DSADVCI21[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI21[i] != NULL);
        port_name.str(""); port_name << "DSADVCI22" << i;
        DSADVCI22[i] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DSADVCI22[i] != NULL);
    }
    for (unsigned int i = 0; i < emInputChannelNum; i++) {
        port_name.str(""); port_name << "bflg_flag" << i;
        bflg_flag[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(bflg_flag[i] != NULL);
        bflg_flag[i]->initialize(false);
        port_name.str(""); port_name << "bflg_bfpulse" << i;
        bflg_bfpulse[i] = new sc_out<bool>(port_name.str().c_str());
        sc_assert(bflg_bfpulse[i] != NULL);
        bflg_bfpulse[i]->initialize(false);
    }
    mABFGcore = new Cabfg_core((std::string)name, this);
    sc_assert(mABFGcore != NULL);
    //Initial variables-------------------------
    mCLK_LSBPeriod = 0;
    mCLK_LSBFreq = 0;
    mCLK_LSBOrgFreq = 0;
    mCLK_LSBFreqUnit = "Hz";
    mTimeCLKChangePoint = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
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
    SC_METHOD(HandleCLK_LSBSignalMethod);
    dont_initialize();
    sensitive << CLK_LSB;

    SC_METHOD(HandlePRESETnSignalMethod);//initialized at beginning
    sensitive << PRESETn;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt0;
        opt0.spawn_method();
        opt0.set_sensitivity(&mResetHardEvent[reset_id]);
        opt0.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleResetHardMethod, this, reset_id), sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt0);
        sc_core::sc_spawn_options opt1;
        opt1.spawn_method();
        opt1.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt1.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleResetCmdMethod, this, reset_id), sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt1);
        sc_core::sc_spawn_options opt2;
        opt2.spawn_method();
        opt2.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt2.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::CancelResetCmdMethod, this, reset_id), sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt2);
    }

    for (unsigned int chn_id = 0; chn_id < emADCHVCNum; chn_id++) {
        sc_core::sc_spawn_options opt0;
        opt0.spawn_method();
        opt0.set_sensitivity(vcend_0[chn_id]);
        opt0.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleADCH0EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleADCH0EndInputMethod"), &opt0);//0->39
        sc_core::sc_spawn_options opt1;
        opt1.spawn_method();
        opt1.set_sensitivity(vcend_1[chn_id]);
        opt1.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleADCH1EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleADCH1EndInputMethod"), &opt1);//80->119
        sc_core::sc_spawn_options opt2;
        opt2.spawn_method();
        opt2.set_sensitivity(vcend_2[chn_id]);
        opt2.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleADCH2EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleADCH2EndInputMethod"), &opt2);//40 -> 79
        sc_core::sc_spawn_options opt3;
        opt3.spawn_method();
        opt3.set_sensitivity(vcend_3[chn_id]);
        opt3.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleADCH3EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleADCH3EndInputMethod"), &opt3);//120->159
    }
    for (unsigned int chn_id = 0; chn_id < emDSADCVCNum; chn_id++) {
        sc_core::sc_spawn_options opt0;
        opt0.spawn_method();
        opt0.set_sensitivity(DSADVCI00[chn_id]);
        opt0.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC00EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC00EndInputMethod"), &opt0);//160->167
        sc_core::sc_spawn_options opt1;
        opt1.spawn_method();
        opt1.set_sensitivity(DSADVCI14[chn_id]);
        opt1.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC14EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC14EndInputMethod"), &opt1);//190->197
    }
    for (unsigned int chn_id = 0; chn_id < (emDSADCVCNum4); chn_id++) {
        sc_core::sc_spawn_options opt0;
        opt0.spawn_method();
        opt0.set_sensitivity(DSADVCI10[chn_id]);
        opt0.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC10EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC10EndInputMethod"), &opt0);//168->171
        sc_core::sc_spawn_options opt1;
        opt1.spawn_method();
        opt1.set_sensitivity(DSADVCI11[chn_id]);
        opt1.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC11EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC11EndInputMethod"), &opt1);//182->185
        sc_core::sc_spawn_options opt2;
        opt2.spawn_method();
        opt2.set_sensitivity(DSADVCI12[chn_id]);
        opt2.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC12EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC12EndInputMethod"), &opt2);//174->177
        sc_core::sc_spawn_options opt3;
        opt3.spawn_method();
        opt3.set_sensitivity(DSADVCI13[chn_id]);
        opt3.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC13EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC13EndInputMethod"), &opt3);//178->181
        sc_core::sc_spawn_options opt4;
        opt4.spawn_method();
        opt4.set_sensitivity(DSADVCI15[chn_id]);
        opt4.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC15EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC15EndInputMethod"), &opt4);//186->189
    }
    for (unsigned int chn_id = 0; chn_id < (emDSADCVCNum2); chn_id++) {
        sc_core::sc_spawn_options opt0;
        opt0.spawn_method();
        opt0.set_sensitivity(DSADVCI20[chn_id]);
        opt0.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC20EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC20EndInputMethod"), &opt0);//172->173
        sc_core::sc_spawn_options opt1;
        opt1.spawn_method();
        opt1.set_sensitivity(DSADVCI21[chn_id]);
        opt1.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC21EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC21EndInputMethod"), &opt1);//208->209
        sc_core::sc_spawn_options opt2;
        opt2.spawn_method();
        opt2.set_sensitivity(DSADVCI22[chn_id]);
        opt2.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleDSADC22EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleDSADC22EndInputMethod"), &opt2);//206->207
    }
    for (unsigned int chn_id = 0; chn_id < emCADCVCNum; chn_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(CADCVCI00[chn_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::HandleCADC00EndInputMethod, this, chn_id), sc_core::sc_gen_unique_name("HandleCADC00EndInputMethod"), &opt);//198->205
    }

    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        sc_core::sc_spawn_options opt0;
        opt0.spawn_method();
        opt0.set_sensitivity(&mNotifyFilterControlEvent[chn_sel]);
        opt0.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::NotifyFilterControlMethod, this, chn_sel), sc_core::sc_gen_unique_name("NotifyFilterControlMethod"), &opt0);
        sc_core::sc_spawn_options opt1;
        opt1.spawn_method();
        opt1.set_sensitivity(&mNotifyBFPulseDoneEvent[chn_sel]);
        opt1.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::NotifyBFPulseDoneMethod, this, chn_sel), sc_core::sc_gen_unique_name("NotifyBFPulseDoneMethod"), &opt1);
        sc_core::sc_spawn_options opt2;
        opt2.spawn_method();
        opt2.set_sensitivity(&mWriteBFEvent[chn_sel]);
        opt2.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::WriteBFMethod, this, chn_sel), sc_core::sc_gen_unique_name("WriteBFMethod"), &opt2);
        sc_core::sc_spawn_options opt3;
        opt3.spawn_method();
        opt3.set_sensitivity(&mWriteBFPulseEvent[chn_sel]);
        opt3.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Cabfg::WriteBFPulseMethod, this, chn_sel), sc_core::sc_gen_unique_name("WriteBFPulseMethod"), &opt3);
    }

    SetLatency_TLM(true);
}//}}}

/*********************************
// Function     : ~Cabfg
// Description  : Destructor of Cabfg class
// Parameter    : None
// Return value : None
**********************************/
Cabfg::~Cabfg(void)
{//{{{
    for (unsigned int i = 0; i < emADCHVCNum; i++) {
        delete vcend_0[i];
        delete vcend_1[i];
        delete vcend_2[i];
        delete vcend_3[i];
    }
    for (unsigned int i = 0; i < emCADCVCNum; i++) {
        delete CADCVCI00[i];
    }
    for (unsigned int i = 0; i < emDSADCVCNum; i++) {
        delete DSADVCI00[i];
        delete DSADVCI10[i];
        delete DSADVCI20[i];
        delete DSADVCI12[i];
        delete DSADVCI13[i];
        delete DSADVCI11[i];
        delete DSADVCI14[i];
        delete DSADVCI15[i];
        delete DSADVCI21[i];
        delete DSADVCI22[i];
    }
    for (unsigned int i = 0; i < emInputChannelNum; i++) {
        delete bflg_flag[i];
        delete bflg_bfpulse[i];
    }
    delete mABFGcore;
}//}}}

/*********************************
// Function     : AssertReset
// Description  : Reset the model
//                This method is called by Command IF
// Parameter    :
//      start_time          Start time
//      period              Reset period
// Return value : None
**********************************/
void Cabfg::AssertReset(const double start_time, const double period)
{//{{{
    if ((!mIsResetHardActive[emPRESETn])&&(!mIsResetCmdReceive[emPRESETn])) {
        mIsResetCmdReceive[emPRESETn] = true;
        re_printf("info","The model will be reset (PRESETn) for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) start_time, FRACTION_TO_INT(start_time));
        mResetCmdEvent[emPRESETn].notify(start_time, SC_NS);
        mResetCmdPeriod[emPRESETn] = period;
    } else {
        re_printf("warning","The AssertReset command of PRESETn is called in the reset operation (PRESETn) of the model. So this calling is ignored.\n");
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
void Cabfg::SetCLKfreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
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
void Cabfg::GetCLKfreq(const std::string clock_name)
{//{{{
    if (clock_name == "CLK_LSB") {
        re_printf("info","CLK_LSB frequency is %0.0f %s.\n", (double)mCLK_LSBOrgFreq, mCLK_LSBFreqUnit.c_str());
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

/*********************************
// Function     : RegisterHandleCommand
// Description  : Set parameters or commands related to register I/F to ABFG_Core
//                This method is called by Command IF
// Parameter    :
//      args            Argument of parameter/command
// Return value : Register dumping message
**********************************/
std::string Cabfg::RegisterHandleCommand(const std::vector<std::string>& args)
{//{{{
    return mABFGcore->RegisterHandler(args);
}//}}}

/*********************************
// Function     : NotifyBFOut
// Description  : Notify Boundary Flag is updated
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector)
//      val                 bf value
//      isBFPOut            bfp flag (true: bfp outputted; false: bfp is not outputted)
// Return value : None
**********************************/
void Cabfg::NotifyBFOut(const unsigned int chn_sel, const bool val, const bool isBFPOut)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            double synclk = CalculateClockEdge((double)mCLK_LSBPeriod, true, false, GetCurTime(), mTimeCLKChangePoint);
            if (val != (bool)(bflg_flag[chn_sel]->read())) {
                mBFOut[chn_sel] = val;
                mWriteBFEvent[chn_sel].notify(synclk, mTimeResolutionUnit);
                if (isBFPOut) {
                    mWriteBFPulseEvent[chn_sel].cancel();
                    mBFPOut[chn_sel] = true;
                    mWriteBFPulseEvent[chn_sel].notify(synclk, mTimeResolutionUnit);
                    mNotifyBFPulseDoneEvent[chn_sel].notify(synclk+mCLK_LSBPeriod, mTimeResolutionUnit);
                }
            }
        }
    }
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
unsigned int Cabfg::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    // This function is not used because there are 1 TLM target socket
    // It is implemented to fix 1Team issue
    return tgt_acc_dbg(trans);
}//}}}
unsigned int Cabfg::tgt_acc_dbg( tlm::tlm_generic_payload &trans)
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
        status = mABFGcore->read(true, (unsigned int)addr, p_data, size);
    //Write access    
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = mABFGcore->write(true, (unsigned int)addr, p_data, size);
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
void Cabfg::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    // This function is not used because there are 1 TLM target socket
    // It is implemented to fix 1Team issue
    tgt_acc(trans, t);
}//}}}
void Cabfg::tgt_acc( tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr    = 0;
    unsigned char *p_data  = NULL;
    unsigned int size = 0; 
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if (!status) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    //Read access
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        status = mABFGcore->read(false, (unsigned int)addr, p_data, size);
    //Write access 
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = mABFGcore->write(false, (unsigned int)addr, p_data, size);
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
void Cabfg::Initialize(void)
{//{{{
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        mEndPulse[chn_sel] = false;
        mLowerErr[chn_sel] = false;
        mUpperErr[chn_sel] = false;
        mBFOut[chn_sel] = false;
        mBFPOut[chn_sel] = false;
    }
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel all operation events
// Parameter    : None
// Return value : None
**********************************/
void Cabfg::CancelEvents(void)
{//{{{
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        mNotifyFilterControlEvent[chn_sel].cancel();
        mNotifyBFPulseDoneEvent[chn_sel].cancel();
        mWriteBFEvent[chn_sel].cancel();
        mWriteBFPulseEvent[chn_sel].cancel();
    }
}//}}}

/*********************************
// Function     : ResetOutputPort
// Description  : Reset output ports when reset is active
// Parameter    : None
// Return value : None
**********************************/
void Cabfg::ResetOutputPort(void)
{//{{{
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        NotifyBFOut(chn_sel, false, false);
        mBFPOut[chn_sel] = false;
        mWriteBFPulseEvent[chn_sel].notify(SC_ZERO_TIME);
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
void Cabfg::EnableReset(const bool is_active, const unsigned int reset_id)
{//{{{
    mABFGcore->EnableReset(is_active, reset_id);
    if (is_active) {
        CancelEvents();
        ResetOutputPort();
        this->Initialize();
    }
}//}}}

/*********************************
// Function     : EndPulseControl
// Description  : Handle the change in end pulse input port
// Parameter    :
//      chan_num            Channel selector (eInputChannelSelector)
// Return value : None
**********************************/
void Cabfg::EndPulseControl(const unsigned int chan_num, const bool end_pulse, const bool upper_error, const bool lower_error)
{//{{{
    unsigned int same_setting = 0;
    double endpulse_period = CalculateClockEdge((double)mCLK_LSBPeriod, true, true, GetCurTime(), mTimeCLKChangePoint);
    for (unsigned int chn_sel = 0; chn_sel < emInputChannelNum; chn_sel++) {
        if (chan_num == (mABFGcore->GetChannelSelect(chn_sel))) {
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
}//}}}

/*********************************
// Function     : HandleCLK_LSBSignalMethod
// Description  : Handle the change of CLK_LSB signal
// Parameter    : None
// Return value : None
**********************************/
void Cabfg::HandleCLK_LSBSignalMethod(void)
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
void Cabfg::HandlePRESETnSignalMethod(void)
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
void Cabfg::HandleResetHardMethod(const unsigned int reset_id)
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
void Cabfg::HandleResetCmdMethod(const unsigned int reset_id)
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
void Cabfg::CancelResetCmdMethod(const unsigned int reset_id)
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
// Function     : Handle<n>InputMethod
// Description  : Handle the change in end input port
//                <n>: ADCH0End, ADCH1End, ADCH2End, ADCH3End,
//                     DSADC00End, DSADC10End, DSADC11End, DSADC12End, DSADC13End
//                     DSADC14End, DSADC15End, DSADC20End, DSADC21End, DSADC22End
//                     CADC00End
// Parameter    :
//      id                  Port index
// Return value : None
**********************************/
void Cabfg::HandleADCH0EndInputMethod(const unsigned int id)//0->39
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id, vcend_0[id]->read(), vculmo_0.read(), vcllmo_0.read());
        }
    }
}//}}}
void Cabfg::HandleADCH1EndInputMethod(const unsigned int id)//80->119
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput80, vcend_1[id]->read(), vculmo_1.read(), vcllmo_1.read());
        }
    }
}//}}}
void Cabfg::HandleADCH2EndInputMethod(const unsigned int id)//40 -> 79
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput40, vcend_2[id]->read(), vculmo_2.read(), vcllmo_2.read());
        }
    }
}//}}}
void Cabfg::HandleADCH3EndInputMethod(const unsigned int id)//120->159
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput120, vcend_3[id]->read(), vculmo_3.read(), vcllmo_3.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC00EndInputMethod(const unsigned int id)//160->167
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput160, DSADVCI00[id]->read(), DSADUE00.read(), DSADLE00.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC10EndInputMethod(const unsigned int id)//168->171
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput168, DSADVCI10[id]->read(), DSADUE10.read(), DSADLE10.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC11EndInputMethod(const unsigned int id)//182->185
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput182, DSADVCI11[id]->read(), DSADUE11.read(), DSADLE11.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC12EndInputMethod(const unsigned int id)//174->177
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput174, DSADVCI12[id]->read(), DSADUE12.read(), DSADLE12.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC13EndInputMethod(const unsigned int id)//178->181
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput178, DSADVCI13[id]->read(), DSADUE13.read(), DSADLE13.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC14EndInputMethod(const unsigned int id)//190->197
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput190, DSADVCI14[id]->read(), DSADUE14.read(), DSADLE14.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC15EndInputMethod(const unsigned int id)//186->189
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput186, DSADVCI15[id]->read(), DSADUE15.read(), DSADLE15.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC20EndInputMethod(const unsigned int id)//172->173
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput172, DSADVCI20[id]->read(), DSADUE20.read(), DSADLE20.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC21EndInputMethod(const unsigned int id)//208->209
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput208, DSADVCI21[id]->read(), DSADUE21.read(), DSADLE21.read());
        }
    }
}//}}}
void Cabfg::HandleDSADC22EndInputMethod(const unsigned int id)//206->207
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput206, DSADVCI22[id]->read(), DSADUE22.read(), DSADLE22.read());
        }
    }
}//}}}
void Cabfg::HandleCADC00EndInputMethod(const unsigned int id)//198->205
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            EndPulseControl(id+emMinEndInput198, CADCVCI00[id]->read(), CADUE00.read(), CADLE00.read());
        }
    }
}//}}}

/*********************************
// Function     : NotifyFilterControlMethod
// Description  : Notify to Filter Control after receive AD end pulse
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector)
// Return value : None
**********************************/
void Cabfg::NotifyFilterControlMethod(const unsigned int chn_sel)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            mABFGcore->NotifyFilterControl(chn_sel, mUpperErr[chn_sel], mLowerErr[chn_sel]);
        }
    }
}//}}}

/*********************************
// Function     : NotifyBFPulseDoneMethod
// Description  : Notify to BF pulse output is done
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector)
// Return value : None
**********************************/
void Cabfg::NotifyBFPulseDoneMethod(const unsigned int chn_sel)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            mBFPOut[chn_sel] = false;
            mWriteBFPulseEvent[chn_sel].notify(SC_ZERO_TIME);
        }
    }
}//}}}

/*********************************
// Function     : WriteBFMethod
// Description  : Write BF output
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector)
// Return value : None
**********************************/
void Cabfg::WriteBFMethod(const unsigned int chn_sel)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            bflg_flag[chn_sel]->write(mBFOut[chn_sel]);
        }
    }
}//}}}

/*********************************
// Function     : WriteBFPulseMethod
// Description  : Write BF pulse output
// Parameter    :
//      chn_sel             Channel selector (eInputChannelSelector)
// Return value : None
**********************************/
void Cabfg::WriteBFPulseMethod(const unsigned int chn_sel)
{//{{{
    if ((!mIsResetCmdActive[emPRESETn])&&(!mIsResetHardActive[emPRESETn])) {
        if (CheckClockPeriod("CLK_LSB", mCLK_LSBPeriod)) {
            if (DumpInterrupt) {
                if (mBFPOut[chn_sel]){
                    re_printf("info","INT [bflg_bfpulse_%d] Assert.\n", chn_sel);
                } else {
                    re_printf("info","INT [bflg_bfpulse_%d] De-assert.\n", chn_sel);
                }
            }
            bflg_bfpulse[chn_sel]->write(mBFPOut[chn_sel]);
        }
    }
}//}}}

/*********************************
// Function     : CheckClockPeriod
// Description  : Check clock period
// Parameter    :
//      clock_name          Clock name
//      clock_period        Clock period
// Return value : true: clock period > 0; false: clock period = 0
**********************************/
bool Cabfg::CheckClockPeriod(const std::string clock_name, const sc_dt::uint64 clock_period)
{//{{{
    if (clock_period > 0) {
        return true;
    } else {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
        return false;
    }
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit depend on mTimeResolutionValue)
**********************************/
double Cabfg::GetCurTime(void)
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
double Cabfg::CalculateClockEdge(const double clock_period, const bool is_pos, const bool add_period, const double time_point, const double time_change_point)
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
void Cabfg::ConvertClockFreq(sc_dt::uint64 &freq_out, std::string &unit_out, const sc_dt::uint64 freq_in, const std::string unit_in)
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
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

/*********************************
// Function     : GetTimeResolution
// Description  : Get simulation time resolution
//      resolution_value    Resolution value
//      resolution_unit     Resolution unit
// Return value : None
**********************************/
void Cabfg::GetTimeResolution(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

void Cabfg::SetLatency_TLM(const bool is_constructor)
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

