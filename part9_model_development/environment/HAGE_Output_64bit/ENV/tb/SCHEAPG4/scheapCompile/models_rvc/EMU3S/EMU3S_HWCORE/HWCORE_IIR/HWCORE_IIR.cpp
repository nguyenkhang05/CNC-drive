// ---------------------------------------------------------------------
// $Id: HWCORE_IIR.cpp,v 1.5 2020/11/12 16:43:32 minhquocha Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of HWCORE_IIR.cpp
// Ref: {HWCORE_IIR_UD_Copyright_001}
// Description: About the include header files of HWCORE_IIR.cpp
// Ref: {HWCORE_IIR_UD_ATT1_002}
#include "re_define.h"
#include "HWCORE_IIR.h"
#include "HWCORE_IIR_Func.h"
///*********************************
/// Function     : HWCORE_IIR
/// Description  : Constructor of HWCORE_IIR class
// Ref: {HWCORE_IIR_UD_Summary_001, HWCORE_IIR_UD_ATT1_004, HWCORE_IIR_UD_ATT1_011, HWCORE_IIR_UD_ReferenceDoc_001}
///*********************************
HWCORE_IIR::HWCORE_IIR(sc_module_name name)
    :sc_module(name)
    , HWCORE_IIR_AgentController()
    , CRWInterface()
    , rvc_common_model()
    //Clock/reset
    , PCLK("PCLK")
    , PRESETn("PRESETn")
    //Input ports
    , rst_n("rst_n")
    , em3ictrinmd_iirdq("em3ictrinmd_iirdq")
    , rdata_em2iidfixobuf("rdata_em2iidfixobuf")
    , rdata_em2iiqfixobuf("rdata_em2iiqfixobuf")
    , em2iadi0("em2iadi0")
    , em2iadi1("em2iadi1")
    , em2iadi2("em2iadi2")
    , adtrg_end_trig("adtrg_end_trig")
    , input_end_sp_reg("input_end_sp_reg")
    //Output ports
    , iir_end0("iir_end0")
    , iir_end1("iir_end1")
    , iir_end2("iir_end2")
    , iir_output0("iir_output0")
    , iir_output1("iir_output1")
    , iir_output2("iir_output2")
    , iir_ovfl_out("iir_ovfl_out")
    , iir_ovfl_func("iir_ovfl_func")
{//{{{

    
    mpHWCORE_IIRFunc = new HWCORE_IIR_Func((std::string)name, this);
    sc_assert(mpHWCORE_IIRFunc != NULL);

    // Initialize variables
    mPCLKPeriod =           0;
    mPCLKOrgFreq =          0;
    mPCLKFreq =             0;
    mPCLKFreqUnit =         "Hz";
    
    // Initialize reset flags
    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }
    
    // Initialize time variables
    mTimeResolutionValue =      1;
    mTimeResolutionUnit =   SC_NS;
    
    // Initialize Python parameters
    mEnableRegisterMessage = true;
    mDumpInterrupt =        false;
    mStartPCLKTime =        false;
    mEnableStatInfo =       false;
    mIIR_cyc =               0x01;
    
    // Initialize Trigger ports
    mADTRGCurVal =          false;
    mADTRGPreVal =          false;
    mInputTRGCurVal =       false;
    mInputTRGPreVal =       false;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    // Initialize output port
    iir_end0.initialize(false);
    iir_end1.initialize(false);
    iir_end2.initialize(false);
    iir_output0.initialize(0x00000000);
    iir_output1.initialize(0x00000000);
    iir_output2.initialize(0x00000000);
    iir_ovfl_out.initialize(false);
    iir_ovfl_func.initialize(0x00);
    
    // Initialize internal variables
    InitializeBuffer();
    InitializeOperation();

    // SC_THREAD/METHOD
    //Clock/reset
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    //PRESETn
    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;

    //rst_n
    SC_METHOD(Handlerst_nSignalMethod);
    sensitive << rst_n;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_IIR::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_IIR::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HWCORE_IIR::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    //Operation
    SC_METHOD(ADCCompletionTriggerMethod);
    dont_initialize();
    sensitive << mADCCompletionEvent;

    SC_METHOD(InputIPCompletionTriggerMethod);
    dont_initialize();
    sensitive << mInputIPCompletionEvent;

    SC_METHOD(IIRJudgeStartupMethod);
    dont_initialize();
    sensitive << mIIRJudgeStartupEvent;

    SC_METHOD(IIRStartChannel0Method);
    dont_initialize();
    sensitive << mIIRCh0StartEvent;

    SC_METHOD(IIRStartChannel1Method);
    dont_initialize();
    sensitive << mIIRCh1StartEvent;

    SC_METHOD(IIRStartChannel2Method);
    dont_initialize();
    sensitive << mIIRCh2StartEvent;

    //For input part
    SC_METHOD(Handle_adtrg_end_trig_Method);
    dont_initialize();
    sensitive << adtrg_end_trig;

    SC_METHOD(Handle_input_end_sp_reg_Method);
    dont_initialize();
    sensitive << input_end_sp_reg;

    SC_METHOD(Handle_em3ictrinmd_iirdq_Method);
    dont_initialize();
    sensitive << em3ictrinmd_iirdq;

    SC_METHOD(Handle_rdata_em2iidfixobuf_Method);
    dont_initialize();
    sensitive << rdata_em2iidfixobuf;

    SC_METHOD(Handle_rdata_em2iiqfixobuf_Method);
    dont_initialize();
    sensitive << rdata_em2iiqfixobuf;

    SC_METHOD(Handle_em2iadi0_Method);
    dont_initialize();
    sensitive << em2iadi0;

    SC_METHOD(Handle_em2iadi1_Method);
    dont_initialize();
    sensitive << em2iadi1;

    SC_METHOD(Handle_em2iadi2_Method);
    dont_initialize();
    sensitive << em2iadi2;

    //For interrupt part
    SC_METHOD(WriteIIROverflowOutPortMethod);
    dont_initialize();
    sensitive << mWriteIIROverflowOutPortEvent;

    SC_METHOD(AssertIIROverflowOutPortMethod);
    dont_initialize();
    sensitive << mAssertIIROverflowOutPortEvent;

    SC_METHOD(DeassertIIROverflowOutPortMethod);
    dont_initialize();
    sensitive << mDeassertIIROverflowOutPortEvent;

    //For output data
    SC_METHOD(WriteIIREnd0PortMethod);
    dont_initialize();
    sensitive << mWriteIIREnd0PortEvent;

    SC_METHOD(AssertIIREnd0Method);
    dont_initialize();
    sensitive << mAssertIIREnd0PortEvent;

    SC_METHOD(DeassertIIREnd0Method);
    dont_initialize();
    sensitive << mDeassertIIREnd0PortEvent;
    
    SC_METHOD(WriteIIREnd1PortMethod);
    dont_initialize();
    sensitive << mWriteIIREnd1PortEvent;
    
    SC_METHOD(AssertIIREnd1Method);
    dont_initialize();
    sensitive << mAssertIIREnd1PortEvent;
    
    SC_METHOD(DeassertIIREnd1Method);
    dont_initialize();
    sensitive << mDeassertIIREnd1PortEvent;

    SC_METHOD(WriteIIREnd2PortMethod);
    dont_initialize();
    sensitive << mWriteIIREnd2PortEvent;

    SC_METHOD(AssertIIREnd2Method);
    dont_initialize();
    sensitive << mAssertIIREnd2PortEvent;

    SC_METHOD(DeassertIIREnd2Method);
    dont_initialize();
    sensitive << mDeassertIIREnd2PortEvent;

    SC_METHOD(ResetPortsMethod);
    dont_initialize();
    sensitive << mResetPortsEvent;

    SC_METHOD(WriteIIROutputPortsMethod);
    dont_initialize();
    sensitive << mWriteIIROutputPortsEvent;

    SC_METHOD(WriteIIROvflFuncPortMethod);
    dont_initialize();
    sensitive << mWriteIIROvflFuncPortsEvent;

}//}}}

///*********************************
/// Function     : ~HWCORE_IIR
/// Description  : Destructor
///*********************************
HWCORE_IIR::~HWCORE_IIR()
{//{{{
    delete mpHWCORE_IIRFunc;
}//}}}

// Define read APIs
// Ref: {HWCORE_IIR_UD_ATT1_006, HWCORE_IIR_UD_Flow_004}
void HWCORE_IIR::read(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL) 
    {
        return;
    }
    mpHWCORE_IIRFunc->read(offsetAddress, data_ptr, size, debug);
}//}}}

// Define write APIs
// Ref: {HWCORE_IIR_UD_ATT1_006, HWCORE_IIR_UD_Flow_005, HWCORE_IIR_UD_Direction_051}
void HWCORE_IIR::write(unsigned int offsetAddress, unsigned char* data_ptr, unsigned int size, bool debug)
{//{{{
    if (data_ptr == NULL)
    {
        return;
    }

    if (((offsetAddress & 0x07FF) == 0x05C0) && ((*data_ptr) == 0x08))
    {
        re_printf("error", "Setting value %08x to IIRCTR0.TRGSEL bit field is prohibited.\n", *data_ptr, offsetAddress);
    }
    else if (((offsetAddress & 0x07FF) == 0x05C1) && ((*data_ptr) == 0x09))
    {
        re_printf("error", "Setting value %08x to IIRCTR1.TRGSEL bit field is prohibited.\n", *data_ptr, offsetAddress);
    }
    else if (((offsetAddress & 0x07FF) == 0x05C2) && ((*data_ptr) == 0x0A))
    {
        re_printf("error", "Setting value %08x to IIRCTR2.TRGSEL bit field is prohibited.\n", *data_ptr, offsetAddress);
    }
    else if (((offsetAddress & 0x07FF) == 0x05C8) && (((*data_ptr) & 0x03) == 0x03))
    {
        re_printf("error", "Setting value %08x to IIRSFT.TRGSEL bit field is prohibited.\n", *data_ptr, offsetAddress);
    }
    else
    {
        mpHWCORE_IIRFunc->write(offsetAddress, data_ptr, size, debug);
    }
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
///*********************************
/// Function     : SetMessageLevel 
/// Description  : Set message level (fatal, error, warning, info)
// Ref: {HWCORE_IIR_UD_Direction_001, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018}
///*********************************
void HWCORE_IIR::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_IIRFunc->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mpHWCORE_IIRFunc->RegisterHandler(cmd);
    } else {
    //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpHWCORE_IIRFunc->RegisterHandler(cmd);
    
        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
    
        mpHWCORE_IIRFunc->RegisterHandler(cmd);
    }
}//}}}

///*********************************
/// Function     : DumpRegisterRW 
/// Description  : Enable/disable dumping message when users access registers
// Ref: {HWCORE_IIR_UD_Direction_002, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018}
///*********************************
void HWCORE_IIR::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mpHWCORE_IIRFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : EnableRegisterMessage 
/// Description  : Enable/disable EnableRegisterMessage
// Ref: {HWCORE_IIR_UD_Direction_003, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_044, HWCORE_IIR_UD_Direction_033}
///*********************************
void HWCORE_IIR::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_IIR_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
        if (mEnableRegisterMessage) {
            re_printf("info","HWCORE_IIR_EnableRegisterMessage %s\n", "true");
        } else {
            re_printf("info","HWCORE_IIR_EnableRegisterMessage %s\n", "false");
        }
    } else {
        re_printf("warning","Invalid argument: HWCORE_IIR_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpInterrupt 
/// Description  : Enable/disable dumping interrupt information
// Ref: {HWCORE_IIR_UD_Direction_004, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_049, HWCORE_IIR_UD_Direction_033, HWCORE_IIR_UD_Flow_018}
///*********************************
void HWCORE_IIR::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HWCORE_IIR_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt){
            temp = "true";
        }else{
            temp = "false";
        }
        re_printf("info","HWCORE_IIR_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HWCORE_IIR_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : EnableStatisticalInfo
/// Description  : Enable/disable dumping statistical information
///*********************************
// Description: EnableStatInfo
/// Ref: {HWCORE_IIR_UD_Direction_052, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018}
void HWCORE_IIR::EnableStatInfo(const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mEnableStatInfo = true;
        }
        else if (temp[0] == "false") {
            mEnableStatInfo = false;
        }
        else {
            re_printf("warning", "Invalid argument: HWCORE_IIR_EnableStatInfo %s\n", is_enable.c_str());
        }
    }
    else if (temp.size() == 0) {
        std::string temp_arg = "";
        if (mEnableStatInfo == true) {
            temp_arg = "true";
        }
        else {
            temp_arg = "false";
        }
        re_printf("info", "HWCORE_IIR_EnableStatInfo %s\n", temp_arg.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: HWCORE_IIR_EnableStatInfo %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : DumpStatusInfo
/// Description  : Dump register's value
// Ref: {HWCORE_IIR_UD_Direction_006, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018}
///*********************************
void HWCORE_IIR::DumpStatInfo(void)
{//{{{
    re_printf("info", "*** IIR Filter channel 0 coefficient buffers ***\n");
    re_printf("info", "IIRICOEF_ch0_buf = 0x%08X", mCh0_IIRICOEF);
    re_printf("info", "IIRCOEF0_ch0_buf = 0x%08X", mCh0_IIRCOEF0);
    re_printf("info", "IIRCOEF1_ch0_buf = 0x%08X", mCh0_IIRCOEF1);
    re_printf("info", "IIRCOEF2_ch0_buf = 0x%08X", mCh0_IIRCOEF2);
    re_printf("info", "IIRCOEF3_ch0_buf = 0x%08X", mCh0_IIRCOEF3);
    re_printf("info", "IIRCOEF4_ch0_buf = 0x%08X", mCh0_IIRCOEF4);
    re_printf("info", "IIROCOEF_ch0_buf = 0x%08X", mCh0_IIROCOEF);

    re_printf("info", "*** IIR Filter channel 1 coefficient buffers ***\n");
    re_printf("info", "IIRICOEF_ch1_buf = 0x%08X", mCh1_IIRICOEF);
    re_printf("info", "IIRCOEF0_ch1_buf = 0x%08X", mCh1_IIRCOEF0);
    re_printf("info", "IIRCOEF1_ch1_buf = 0x%08X", mCh1_IIRCOEF1);
    re_printf("info", "IIRCOEF2_ch1_buf = 0x%08X", mCh1_IIRCOEF2);
    re_printf("info", "IIRCOEF3_ch1_buf = 0x%08X", mCh1_IIRCOEF3);
    re_printf("info", "IIRCOEF4_ch1_buf = 0x%08X", mCh1_IIRCOEF4);
    re_printf("info", "IIROCOEF_ch1_buf = 0x%08X", mCh1_IIROCOEF);

    re_printf("info", "*** IIR Filter channel 2 coefficient buffers ***\n");
    re_printf("info", "IIRICOEF_ch2_buf = 0x%08X", mCh2_IIRICOEF);
    re_printf("info", "IIRCOEF0_ch2_buf = 0x%08X", mCh2_IIRCOEF0);
    re_printf("info", "IIRCOEF1_ch2_buf = 0x%08X", mCh2_IIRCOEF1);
    re_printf("info", "IIRCOEF2_ch2_buf = 0x%08X", mCh2_IIRCOEF2);
    re_printf("info", "IIRCOEF3_ch2_buf = 0x%08X", mCh2_IIRCOEF3);
    re_printf("info", "IIRCOEF4_ch2_buf = 0x%08X", mCh2_IIRCOEF4);
    re_printf("info", "IIROCOEF_ch2_buf = 0x%08X", mCh2_IIROCOEF);
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Command to assert reset
// Ref: {HWCORE_IIR_UD_Direction_006, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_034, HWCORE_IIR_UD_Direction_035, HWCORE_IIR_UD_Direction_040}
///*********************************
void HWCORE_IIR::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive[emPRESETnId])&&(!mIsResetCmdReceive[emPRESETnId])) {
            mIsResetCmdReceive[emPRESETnId] = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emPRESETnId].notify(start_time, SC_NS);
            mResetCmdPeriod[emPRESETnId] = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else if (reset_name == "rst_n") {
        if ((!mIsResetHardActive[emrst_nId])&&(!mIsResetCmdReceive[emrst_nId])) {
            mIsResetCmdReceive[emrst_nId] = true;
            re_printf("info","The model will be reset (rst_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emrst_nId].notify(start_time, SC_NS);
            mResetCmdPeriod[emrst_nId] = period;
        } else {
            re_printf("warning","The software reset of rst_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else{
        re_printf("warning","The reset name (%s) is wrong. It should be rst_n",reset_name.c_str());
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value
// Ref: {HWCORE_IIR_UD_Direction_007, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_032}
///*********************************
void HWCORE_IIR::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : GetCLKFreq
/// Description  : Get clock value
// Ref: {HWCORE_IIR_UD_Direction_008, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_043, HWCORE_IIR_UD_Direction_032}
///*********************************
void HWCORE_IIR::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ForceRegister
/// Description  : Force value to register
// Ref: {HWCORE_IIR_UD_Direction_009, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_029}
///*********************************
void HWCORE_IIR::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mpHWCORE_IIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReleaseRegister
/// Description  : Release forced value after forcing registers
// Ref: {HWCORE_IIR_UD_Direction_010, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_029}
///*********************************
void HWCORE_IIR::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mpHWCORE_IIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : WriteRegister
/// Description  : Write value to registers by software
// Ref: {HWCORE_IIR_UD_Direction_011, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_029}
///*********************************
void HWCORE_IIR::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            mpHWCORE_IIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
// Ref: {HWCORE_IIR_UD_Direction_012, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_029}
///*********************************
void HWCORE_IIR::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            mpHWCORE_IIRFunc->RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ListRegister 
/// Description  : List all registers name
// Ref: {HWCORE_IIR_UD_Direction_013, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018}
///*********************************
void HWCORE_IIR::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mpHWCORE_IIRFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
// Ref: {HWCORE_IIR_UD_Direction_014, HWCORE_IIR_UD_Direction_016, HWCORE_IIR_UD_Direction_017, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_028, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018}
///*********************************
void HWCORE_IIR::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_MessageLevel (\"HWCORE_IIR instance\", \"fatal|error|warning|info\")                          Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_DumpRegisterRW (\"HWCORE_IIR instance\", \"true/false\")                                      Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_DumpInterrupt (\"HWCORE_IIR instance\", \"true/false\")                                       Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_EnableRegisterMessage (\"HWCORE_IIR instance\", \"true/false\")                               Enable/disable info/warning/error message of register IF (Default: true)");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_EnableStatInfo (\"HWCORE_IIR instance\", \"true/false\")                                      Enable/disable dumping the internal buffer registers' values of the HWCORE_IIR model (Default: false)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_DumpStatInfo (\"HWCORE_IIR instance\")                                                        Dump the values of the internal buffer registers of the HWCORE_IIR model");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_AssertReset (\"HWCORE_IIR instance\", \"reset_name\", start_time, period)                     Assert and de-assert reset signal to the HWCORE_IIR model");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_SetCLKFreq (\"HWCORE_IIR instance\", \"clock_name\", freq, \"unit\")                          Set clock frequency value to HWCORE_IIR model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_GetCLKFreq (\"HWCORE_IIR instance\", \"clock_name\")                                          Get clock frequency value of HWCORE_IIR model. Valid clock_name is PCLK");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_ForceRegister (\"HWCORE_IIR instance\",\"reg_name\", reg_value)                               Force register with setting value");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_ReleaseRegister (\"HWCORE_IIR instance\",\"reg_name\")                                        Release register from force value");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_WriteRegister (\"HWCORE_IIR instance\",\"reg_name\", reg_value)                               Write a value to a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_ReadRegister (\"HWCORE_IIR instance\",\"reg_name\")                                           Read a value from a register");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_ListRegister (\"HWCORE_IIR instance\")                                                        Dump name of all registers");
            SC_REPORT_INFO(this->basename(),"HWCORE_IIR_ChgProcessingTime (\"HWCORE_IIR instance\", \"cycle\")                                        Change model processing time");
        } else {
            re_printf("warning","The name (%s) of HWCORE_IIR_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HWCORE_IIR_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///*********************************
/// Function     : ChgProcessingTime
/// Description  : Change the process time
// Ref: {HWCORE_IIR_UD_Direction_015, HWCORE_IIR_UD_Flow_003, HWCORE_IIR_UD_Flow_018, HWCORE_IIR_UD_Direction_018, HWCORE_IIR_UD_Direction_050}
///*********************************
void HWCORE_IIR::ChgProcessingTime(const unsigned int cycle)
{//{{{
    mIIR_cyc = cycle;
    re_printf("info", "Change the process time for IIR Filter to %d.\n", cycle);
}//}}}

//========================================================
//============ Method/Threads          ===================
//========================================================
///*********************************
/// Function     : HandlePCLKSignalMethod 
/// Description  : Handle PCLK signal
// Ref: {HWCORE_IIR_UD_Flow_002}
///*********************************
void HWCORE_IIR::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandlePRESETnSignalMethod 
/// Description  : Handle PRESETn signal
///*********************************
void HWCORE_IIR::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal[emPRESETnId] = PRESETn.read();
    if (mResetCurVal[emPRESETnId] != mResetPreVal[emPRESETnId] ) {       //mResetPreVal is "true" at initial state
        mResetPreVal[emPRESETnId]  = mResetCurVal[emPRESETnId] ;
        mResetHardEvent[emPRESETnId].notify(SC_ZERO_TIME);
    }

}//}}}

///*********************************
/// Function     : Handlerst_nSignalMethod 
/// Description  : Handle rst_n signal
///*********************************
void HWCORE_IIR::Handlerst_nSignalMethod (void)
{//{{{
    mResetCurVal[emrst_nId] = rst_n.read();
    if (mResetCurVal[emrst_nId] != mResetPreVal[emrst_nId]) {       //mResetPreVal is "false" at initial state
        mResetPreVal[emrst_nId] = mResetCurVal[emrst_nId];
        mResetHardEvent[emrst_nId].notify(SC_ZERO_TIME);
    }
}//}}}

///*********************************
/// Function     : Handle_adtrg_end_trig_Method 
/// Description  : Handle adtrg_end_trig port
///*********************************
void HWCORE_IIR::Handle_adtrg_end_trig_Method(void)
{//{{{
    mADTRGCurVal = adtrg_end_trig.read();
    if (mADTRGCurVal == mADTRGPreVal) {       //mADTRGPreVal is "false" at initial state
        return;
    }
    mADTRGPreVal = mADTRGCurVal;
    if (mADTRGCurVal == 1) {
        mADCCompletionEvent.notify(SC_ZERO_TIME);
    }

}//}}}

///*********************************
/// Function     : Handle_input_end_sp_reg_Method 
/// Description  : Handle input_end_sp_reg port
///*********************************
void HWCORE_IIR::Handle_input_end_sp_reg_Method(void)
{//{{{
    mInputTRGCurVal = input_end_sp_reg.read();
    if (mInputTRGCurVal == mInputTRGPreVal) {       //mADTRGPreVal is "false" at initial state
        return;
    }
    mInputTRGPreVal = mInputTRGCurVal;
    if (mInputTRGCurVal == 1) {
        mInputIPCompletionEvent.notify(SC_ZERO_TIME);
    }

}//}}}

///*********************************
/// Function     : Handle_em3ictrinmd_iirdq_Method 
/// Description  : Handle em3ictrinmd_iirdq port
///*********************************
void HWCORE_IIR::Handle_em3ictrinmd_iirdq_Method(void)
{//{{{
    mIIRDQ = em3ictrinmd_iirdq.read();
}//}}}

///*********************************
/// Function     : Handle_rdata_em2iidfixobuf_Method 
/// Description  : Handle rdata_em2iidfixobuf port
///*********************************
void HWCORE_IIR::Handle_rdata_em2iidfixobuf_Method(void)
{//{{{
    mIDFIXOBUF = rdata_em2iidfixobuf.read();
}//}}}

///*********************************
/// Function     : Handle_rdata_em2iiqfixobuf_Method
/// Description  : Handle rdata_em2iiqfixobuf port
///*********************************
void HWCORE_IIR::Handle_rdata_em2iiqfixobuf_Method(void)
{//{{{
    mIQFIXOBUF = rdata_em2iiqfixobuf.read();
}//}}}

///*********************************
/// Function     : Handle_em2iadi0_Method
/// Description  : Handle em2iadi0 port
///*********************************
void HWCORE_IIR::Handle_em2iadi0_Method(void)
{//{{{
    mAD0 = (unsigned short)(em2iadi0.read() & 0x0FFF);
}//}}}

///*********************************
/// Function     : Handle_em2iadi1_Method
/// Description  : Handle em2iadi1 port
///*********************************
void HWCORE_IIR::Handle_em2iadi1_Method(void)
{//{{{
    mAD1 = (unsigned short)(em2iadi1.read() & 0x0FFF);
}//}}}

///*********************************
/// Function     : Handle_em2iadi2_Method 
/// Description  : Handle em2iadi2 port
///*********************************
void HWCORE_IIR::Handle_em2iadi2_Method(void)
{//{{{
    mAD2 = (unsigned short)(em2iadi2.read() & 0x0FFF);
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
///*********************************
/// Function     : HandleResetHardMethod
/// Description  : Process reset function when reset port is active
// Ref: {HWCORE_IIR_UD_Flow_001, HWCORE_IIR_UD_Direction_041, HWCORE_IIR_UD_Direction_042, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        } else {
            re_printf("info","The reset port rst_n is asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
    } else {
        mIsResetHardActive[reset_id] = false;
        if (reset_id == emPRESETnId) {
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        } else {
            re_printf("info","The reset port rst_n is de-asserted.\n");
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
        }
    }

    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id] && mIsResetHardActive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }

}//}}}

///*********************************
/// Function     : HandleResetCmdMethod 
/// Description  : Process reset function when reset command is active
// Ref: {HWCORE_IIR_UD_Direction_039, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else { //reset_id == emrst_nId
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///Cancel reset function when reset command is active
///*********************************
/// Function     : CancelResetCmdMethod 
/// Description  : Cancel ResetCmd Method
// Ref: {HWCORE_IIR_UD_Direction_038, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "PRESETn";
    if (reset_id == emPRESETnId) {
        reset_name = "PRESETn";
    } else {
        reset_name = "rst_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Execute reset operation
// Ref: {HWCORE_IIR_UD_Flow_017}
///*********************************
void HWCORE_IIR::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    mpHWCORE_IIRFunc->EnableReset (is_active);
    if (is_active) {
        InitializeBuffer();
        if (reset_id == emrst_nId){
            InitializeOperation();
            CancelEvents();
            mResetPortsEvent.notify();
        }
    }

}//}}}

///*********************************
/// Function     : InitializeBuffer
/// Description  : Initialize Buffer variables
///*********************************
void HWCORE_IIR::InitializeBuffer(void)
{//{{{
    mCh0_IIRICOEF = 0x00000000;
    mCh0_IIRCOEF0 = 0x00000000;
    mCh0_IIRCOEF1 = 0x00000000;
    mCh0_IIRCOEF2 = 0x00000000;
    mCh0_IIRCOEF3 = 0x00000000;
    mCh0_IIRCOEF4 = 0x00000000;
    mCh0_IIROCOEF = 0x00000000;

    mCh1_IIRICOEF = 0x00000000;
    mCh1_IIRCOEF0 = 0x00000000;
    mCh1_IIRCOEF1 = 0x00000000;
    mCh1_IIRCOEF2 = 0x00000000;
    mCh1_IIRCOEF3 = 0x00000000;
    mCh1_IIRCOEF4 = 0x00000000;
    mCh1_IIROCOEF = 0x00000000;

    mCh2_IIRICOEF = 0x00000000;
    mCh2_IIRCOEF0 = 0x00000000;
    mCh2_IIRCOEF1 = 0x00000000;
    mCh2_IIRCOEF2 = 0x00000000;
    mCh2_IIRCOEF3 = 0x00000000;
    mCh2_IIRCOEF4 = 0x00000000;
    mCh2_IIROCOEF = 0x00000000;
}//}}}

///*********************************
/// Function     : InitializeOperation
/// Description  : Initialize Operation variables
///*********************************
void HWCORE_IIR::InitializeOperation(void)
{//{{{
    mIIR_reserve[emIIR_CH0] = emIIR_NO_RESERVE;
    mIIR_reserve[emIIR_CH1] = emIIR_NO_RESERVE;
    mIIR_reserve[emIIR_CH2] = emIIR_NO_RESERVE;

    mIIR_state[emIIR_CH0] = emIIR_INACTIVE;
    mIIR_state[emIIR_CH1] = emIIR_INACTIVE;
    mIIR_state[emIIR_CH2] = emIIR_INACTIVE;

    mIIRDQ = 0x0;
    mIDFIXOBUF = 0x00000000;
    mIQFIXOBUF = 0x00000000;
    mAD0 = 0x0000;
    mAD1 = 0x0000;
    mAD2 = 0x0000;

    mIIREnd0_NextVal = false;
    mIIREnd1_NextVal = false;
    mIIREnd2_NextVal = false;
    mIIROvflFunc_NextVal = 0x00;
    mIIROvflOut_NextVal = false;
}//}}}

///*********************************
/// Function     : Reset_Ports
/// Description  : Initialize output ports
///*********************************
void HWCORE_IIR::ResetPortsMethod(void)
{//{{{
    mWriteIIROutputPortsEvent.notify();
    mDeassertIIROverflowOutPortEvent.notify();
    mDeassertIIREnd0PortEvent.notify();
    mDeassertIIREnd1PortEvent.notify();
    mDeassertIIREnd2PortEvent.notify();
    mWriteIIROvflFuncPortsEvent.notify();
}//}}}

///*********************************
/// Function     : CancelEvents
/// Description  : Cancel all events
///*********************************
void HWCORE_IIR::CancelEvents()
{//{{{
    mADCCompletionEvent.cancel();
    mInputIPCompletionEvent.cancel();
    mIIRJudgeStartupEvent.cancel();
    mIIRCh0StartEvent.cancel();
    mIIRCh1StartEvent.cancel();
    mIIRCh2StartEvent.cancel();
    mWriteIIROverflowOutPortEvent.cancel();
    mAssertIIROverflowOutPortEvent.cancel();
    mDeassertIIROverflowOutPortEvent.cancel();
    mWriteIIREnd0PortEvent.cancel();
    mAssertIIREnd0PortEvent.cancel();
    mDeassertIIREnd0PortEvent.cancel();
    mWriteIIREnd1PortEvent.cancel();
    mAssertIIREnd1PortEvent.cancel();
    mDeassertIIREnd1PortEvent.cancel();
    mWriteIIREnd2PortEvent.cancel();
    mAssertIIREnd2PortEvent.cancel();
    mDeassertIIREnd2PortEvent.cancel();
    mWriteIIROutputPortsEvent.cancel();
    mWriteIIROvflFuncPortsEvent.cancel();
}//}}}

///*********************************
/// Function     : DumpInterruptMsg
/// Description  : Dump Interrupt Message
// Ref: {HWCORE_IIR_UD_Direction_048, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
    if (mDumpInterrupt) {
        if (value) {
            re_printf("info","[%s] is changed to %s.\n",intr_name.c_str(),"1");
        } else {
            re_printf("info","[%s] is changed to %s .\n",intr_name.c_str(),"0");
        }
    }

}//}}}

///*********************************
/// Function     : WriteIIROutputPort 
/// Description  : Write iir_output ports
// Ref: {HWCORE_IIR_UD_Flow_016, HWCORE_IIR_UD_Direction_047, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::WriteIIROutputPortsMethod(void)
{//{{{
    signed int mIIROutput0_CurVal = iir_output0.read();
    signed int mIIROutput1_CurVal = iir_output1.read();
    signed int mIIROutput2_CurVal = iir_output2.read();

    if(mIIROutput0_CurVal != mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT0"))
    {
        iir_output0.write(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT0"));
        re_printf("info","Port iir_output0 is set to (%08x).\n", mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT0"));
    }
    if(mIIROutput1_CurVal != mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT1"))
    {
        iir_output1.write(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT1"));
        re_printf("info","Port iir_output1 is set to (%08x).\n", mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT1"));
    }
    if(mIIROutput2_CurVal != mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT2"))
    {
        iir_output2.write(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT2"));
        re_printf("info","Port iir_output2 is set to (%08x).\n", mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT2"));
    }
}//}}}

///*********************************
/// Function     : WriteIIROvflFuncPortMethod 
/// Description  : Write iir_ovfl_func Method
// Ref: {HWCORE_IIR_UD_Flow_016}
///*********************************
void HWCORE_IIR::WriteIIROvflFuncPortMethod(void)
{//{{{
    unsigned char mIIROvflFunc_CurVal = iir_ovfl_func.read();

    if (mIIROvflFunc_CurVal != mIIROvflFunc_NextVal) {
        iir_ovfl_func.write(mIIROvflFunc_NextVal);
        re_printf("info","[iir_ovfl_func] is changed to 0x0%d.\n", mIIROvflFunc_NextVal);
    }
}//}}}

///*********************************
/// Function     : WriteIIREnd0PortMethod 
/// Description  : Write iir_end0 Method
// Ref: {HWCORE_IIR_UD_Flow_016}
///*********************************
void HWCORE_IIR::WriteIIREnd0PortMethod(void)
{
    bool current_value = iir_end0.read();

    if (current_value != mIIREnd0_NextVal) {
        iir_end0.write(mIIREnd0_NextVal);
        DumpInterruptMsg("iir_end0", mIIREnd0_NextVal);
    }
}

///*********************************
/// Function     : AssertIIREnd0Method 
/// Description  : Assert iir_end0 Method 
///*********************************
void HWCORE_IIR::AssertIIREnd0Method(void)
{
    mIIREnd0_NextVal = true;
    mWriteIIREnd0PortEvent.notify();
    // Deassert after 1 cycle
    double current_time = sc_time_stamp().to_double();
    mDeassertIIREnd0PortEvent.notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}

///*********************************
/// Function     : DeassertIIREnd0Method 
/// Description  : DeAssert iir_end0 Method 
///*********************************
void HWCORE_IIR::DeassertIIREnd0Method(void)
{
    mIIREnd0_NextVal = false;
    mWriteIIREnd0PortEvent.notify();
}

///*********************************
/// Function     : WriteIIREnd1PortMethod 
/// Description  : Write iir_end1 Method
// Ref: {HWCORE_IIR_UD_Flow_016}
///*********************************
void HWCORE_IIR::WriteIIREnd1PortMethod(void)
{
    bool current_value = iir_end1.read();

    if (current_value != mIIREnd1_NextVal) {
        iir_end1.write(mIIREnd1_NextVal);
        DumpInterruptMsg("iir_end1", mIIREnd1_NextVal);
    }
}

///*********************************
/// Function     : AssertIIREnd1Method 
/// Description  : Assert iir_end1 Method 
///*********************************
void HWCORE_IIR::AssertIIREnd1Method(void)
{
    mIIREnd1_NextVal = true;
    mWriteIIREnd1PortEvent.notify();
    // Deassert after 1 cycle
    double current_time = sc_time_stamp().to_double();
    mDeassertIIREnd1PortEvent.notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}

///*********************************
/// Function     : DeassertIIREnd1Method 
/// Description  : DeAssert iir_end1 Method 
///*********************************
void HWCORE_IIR::DeassertIIREnd1Method(void)
{
    mIIREnd1_NextVal = false;
    mWriteIIREnd1PortEvent.notify();
}

///*********************************
/// Function     : WriteIIREnd2PortMethod 
/// Description  : Write iir_end2 Method
// Ref: {HWCORE_IIR_UD_Flow_016}
///*********************************
void HWCORE_IIR::WriteIIREnd2PortMethod(void)
{
    bool current_value = iir_end2.read();

    if (current_value != mIIREnd2_NextVal) {
        iir_end2.write(mIIREnd2_NextVal);
        DumpInterruptMsg("iir_end2", mIIREnd2_NextVal);
    }
}

///*********************************
/// Function     : AssertIIREnd2Method 
/// Description  : Assert iir_end2 Method 
///*********************************
void HWCORE_IIR::AssertIIREnd2Method(void)
{
    mIIREnd2_NextVal = true;
    mWriteIIREnd2PortEvent.notify();
    // Deassert after 1 cycle
    double current_time = sc_time_stamp().to_double();
    mDeassertIIREnd2PortEvent.notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}

///*********************************
/// Function     : DeassertIIREnd2Method 
/// Description  : DeAssert iir_end2 Method 
///*********************************
void HWCORE_IIR::DeassertIIREnd2Method(void)
{
    mIIREnd2_NextVal = false;
    mWriteIIREnd2PortEvent.notify();
}

///*********************************
/// Function     : WriteIIROverflowInterruptPortMethod 
/// Description  : Write iir_ovfl_out Method
// Ref: {HWCORE_IIR_UD_Flow_016}
///*********************************
void HWCORE_IIR::WriteIIROverflowOutPortMethod(void)
{
    bool current_value = iir_ovfl_out.read();

    if (current_value != mIIROvflOut_NextVal) {
        iir_ovfl_out.write(mIIROvflOut_NextVal);
        DumpInterruptMsg("iir_ovfl_out", mIIROvflOut_NextVal);
    }
}

///*********************************
/// Function     : AssertIIROverflowInterruptPortMethod 
/// Description  : Assert iir_ovfl_out Method 
///*********************************
void HWCORE_IIR::AssertIIROverflowOutPortMethod(void)
{
    mIIROvflOut_NextVal = true;
    mWriteIIROverflowOutPortEvent.notify();
    // Deassert after 1 cycle
    double current_time = sc_time_stamp().to_double();
    mDeassertIIROverflowOutPortEvent.notify(CalculateCLKPosEdge(
        (double)mPCLKPeriod, false, current_time, mStartPCLKTime) + (double)mPCLKPeriod, mTimeResolutionUnit);
}

///*********************************
/// Function     : DeassertIIROverflowOutPortMethod 
/// Description  : DeAssert iir_ovfl_out Method 
///*********************************
void HWCORE_IIR::DeassertIIROverflowOutPortMethod(void)
{//{{{
    mIIROvflOut_NextVal = false;
    mWriteIIROverflowOutPortEvent.notify();
}//}}}

///*********************************
/// Function     : ADCCompletionTriggerMethod
/// Description  : Operations when model receives notify of ADC Completion.
// Ref: {HWCORE_IIR_UD_Flow_010}
///*********************************
void HWCORE_IIR::ADCCompletionTriggerMethod(void)
{//{{{
    if (GetResetStatus("rst_n") == true || CheckClockPeriod() == false) {
        return;
    }

    if (mIIRDQ == emAD)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR2", "TRGSEL") == 0x04)        /* confirm whether iir2 trigger is AD0 finish */
        {
            mIIR_reserve[emIIR_CH2] = emIIR_RESERVE;
        }

        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR1", "TRGSEL") == 0x04)        /* confirm whether iir1 trigger is AD0 finish */
        {
            mIIR_reserve[emIIR_CH1] = emIIR_RESERVE;
        }

        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR0", "TRGSEL") == 0x04)        /* confirm whether iir0 trigger is AD0 finish */
        {
            mIIR_reserve[emIIR_CH0] = emIIR_RESERVE;
        }
        mIIRJudgeStartupEvent.notify();
    }
}//}}}

///*********************************
/// Function     : InputIPCompletionTriggerMethod
/// Description  : Operations when model receives notify of Input IP.
// Ref: {HWCORE_IIR_UD_Flow_011}
///*********************************
void HWCORE_IIR::InputIPCompletionTriggerMethod(void)
{//{{{
    if (GetResetStatus("rst_n") == true || CheckClockPeriod() == false) {
        return;
    }

    if (mIIRDQ == emDQ)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR0", "TRGSEL") == 0x04)        /* confirm whether iir0 trigger is AD0 finish */
        {
            mIIR_reserve[emIIR_CH0] = emIIR_RESERVE;
        }
        mIIRJudgeStartupEvent.notify();
    }
}//}}}

///*********************************
/// Function     : IIRJudgeStartupMethod
/// Description  : Select IIR channel to start operation.
// Ref: {HWCORE_IIR_UD_Flow_012, HWCORE_IIR_UD_Direction_045, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::IIRJudgeStartupMethod(void)
{//{{{
    if ((mIIR_state[emIIR_CH0] == emIIR_INACTIVE)
        && (mIIR_state[emIIR_CH1] == emIIR_INACTIVE)
        && (mIIR_state[emIIR_CH2] == emIIR_INACTIVE))            /* all IIR filter is inactive */
    {
        /* iir priority order is 2,1,0 */
        if (mIIR_reserve[emIIR_CH2] == emIIR_RESERVE)
        {
            mIIR_state[emIIR_CH2] = emIIR_ACTIVE;
            mIIR_reserve[emIIR_CH2] = emIIR_NO_RESERVE;
            mIIRCh2StartEvent.notify((double)(mIIR_cyc * mPCLKPeriod), mTimeResolutionUnit);
            re_printf("info","IIR Filter channel 2 start operation");
        }
        else if (mIIR_reserve[emIIR_CH1] == emIIR_RESERVE)
        {
            mIIR_state[emIIR_CH1] = emIIR_ACTIVE;
            mIIR_reserve[emIIR_CH1] = emIIR_NO_RESERVE;
            mIIRCh1StartEvent.notify((double)(mIIR_cyc * mPCLKPeriod), mTimeResolutionUnit);
            re_printf("info","IIR Filter channel 1 start operation");
        }
        else if (mIIR_reserve[emIIR_CH0] == emIIR_RESERVE)
        {
            mIIR_state[emIIR_CH0] = emIIR_ACTIVE;
            mIIR_reserve[emIIR_CH0] = emIIR_NO_RESERVE;
            mIIRCh0StartEvent.notify((double)(mIIR_cyc * mPCLKPeriod), mTimeResolutionUnit);
            re_printf("info","IIR Filter channel 0 start operation");
        }
    }
}//}}}

///*********************************
/// Function     : IIRStartChannel0Method
/// Description  : Operations of IIR Filter channel 0.
// Ref: {HWCORE_IIR_UD_Flow_013}
///*********************************
void HWCORE_IIR::IIRStartChannel0Method(void)
{//{{{
    IIROperation(emIIR_CH0);                                /* IIR process */

    mIIR_state[emIIR_CH0] = emIIR_INACTIVE;                    /* IIR0 change to INACTIVE */

    if (mIIRDQ == emAD)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR1", "TRGSEL") == 0x08)        /* confirm whether iir1 trigger is IIR0 finish */
        {
            mIIR_reserve[emIIR_CH1] = emIIR_RESERVE;
        }

        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR2", "TRGSEL") == 0x08)        /* confirm whether iir2 trigger is IIR0 finish */
        {
            mIIR_reserve[emIIR_CH2] = emIIR_RESERVE;
        }
    }
    else
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR1", "TRGSEL") == 0x04)        /* confirm whether iir1 trigger is IIR0 finish */
        {
            mIIR_reserve[emIIR_CH1] = emIIR_RESERVE;
        }
    }
    mIIRJudgeStartupEvent.notify();
}//}}}

///*********************************
/// Function     : IIRStartChannel1Method
/// Description  : Operations of IIR Filter channel 1.
// Ref: {HWCORE_IIR_UD_Flow_014}
///*********************************
void HWCORE_IIR::IIRStartChannel1Method(void)
{//{{{
    IIROperation(emIIR_CH1);                                /* IIR process */

    mIIR_state[emIIR_CH1] = emIIR_INACTIVE;                    /* IIR1 change to INACTIVE */

    if (mIIRDQ == emAD)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR0", "TRGSEL") == 0x09)        /* confirm whether iir0 trigger is IIR1 finish */
        {
            mIIR_reserve[emIIR_CH0] = emIIR_RESERVE;
        }

        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR2", "TRGSEL") == 0x09)        /* confirm whether iir2 trigger is IIR1 finish */
        {
            mIIR_reserve[emIIR_CH2] = emIIR_RESERVE;
        }
    }

    mIIRJudgeStartupEvent.notify();
}//}}}

///*********************************
/// Function     : IIRStartChannel2Method
/// Description  : Operations of IIR Filter channel 2.
// Ref: {HWCORE_IIR_UD_Flow_015}
///*********************************
void HWCORE_IIR::IIRStartChannel2Method(void)
{//{{{
    IIROperation(emIIR_CH2);                                /* IIR process */

    mIIR_state[emIIR_CH2] = emIIR_INACTIVE;                    /* IIR2 change to INACTIVE */

    if (mIIRDQ == emAD)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR0", "TRGSEL") == 0x0A)        /* confirm whether iir0 trigger is IIR2 finish */
        {
            mIIR_reserve[emIIR_CH0] = emIIR_RESERVE;
        }

        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR1", "TRGSEL") == 0x0A)        /* confirm whether iir1 trigger is IIR2 finish */
        {
            mIIR_reserve[emIIR_CH1] = emIIR_RESERVE;
        }
    }

    mIIRJudgeStartupEvent.notify();
}//}}}

///*********************************
/// Function     : IIRInputSelection
/// Description  : Select input data for IIR Filter calculation.
// Ref: {HWCORE_IIR_UD_Flow_016}
///*********************************
signed int HWCORE_IIR::IIRInputSelection(unsigned char iir_ch)
{//{{{
    signed int input_value = 0;

    switch(iir_ch)
    {
    case emIIR_CH0:                /* IIR0 set variable */
        /* get input value */
        if(mIIRDQ == emAD)
        {
            switch(mpHWCORE_IIRFunc->GetRegVal("IIRCTR0","DATSEL"))
            {
            case 0x0:        /* IIR0 soft input */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRSFTDAT0") & 0xFFFFFFFF);
                break;
            case 0x2:        /* 0 data soft input */
                input_value = 0x00000000;
                break;
            case 0x4:        /* AD0 */
                input_value = mAD0;
                break;
            case 0x5:        /* AD1 */
                input_value = mAD1;
                break;
            case 0x6:        /* AD2 */
                input_value = mAD2;
                break;
            case 0x8:        /* IIR0 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT0") & 0xFFFFFFFF);
                break;
            case 0x9:        /* IIR1 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT1") & 0xFFFFFFFF);
                break;
            case 0xA:        /* IIR2 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT2") & 0xFFFFFFFF);
                break;
            default:        /* no operation */
                break;
            }
        }
        else
        {
            switch(mpHWCORE_IIRFunc->GetRegVal("IIRCTR0","DATSEL"))
            {
            case 0x0:        /* IIR0 soft input */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRSFTDAT0") & 0xFFFFFFFF);
                break;
            case 0x2:        /* 0 data soft input */
                input_value = 0x00000000;
                break;
            case 0x4:        /* IDFIXOBUF */
                input_value = (signed int)(mIDFIXOBUF);
                break;
            default:        /* no operation*/
                break;
            }
        }
        /* get input value */
        break;

    case emIIR_CH1:                /* IIR1 set variable */
        /* get input value */
        if(mIIRDQ == emAD)
        {
            switch(mpHWCORE_IIRFunc->GetRegVal("IIRCTR1","DATSEL"))
            {
            case 0x0:        /* IIR1 soft input */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRSFTDAT1") & 0xFFFFFFFF);
                break;
            case 0x2:        /* 0 data soft input */
                input_value = 0x00000000;
                break;
            case 0x4:        /* AD0 */
                input_value = (signed int)(mAD0);
                break;
            case 0x5:        /* AD1 */
                input_value = (signed int)(mAD1);
                break;
            case 0x6:        /* AD2 */
                input_value = (signed int)(mAD2);
                break;
            case 0x8:        /* IIR0 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT0") & 0xFFFFFFFF);
                break;
            case 0x9:        /* IIR1 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT1") & 0xFFFFFFFF);
                break;
            case 0xA:        /* IIR2 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT2") & 0xFFFFFFFF);
                break;
            default:        /* no operation*/
                break;
            }
        }
        else
        {
            switch(mpHWCORE_IIRFunc->GetRegVal("IIRCTR1","DATSEL"))
            {
            case 0x0:        /* IIR1 soft input */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRSFTDAT1") & 0xFFFFFFFF);
                break;
            case 0x2:        /* 0 data soft input */
                input_value = 0x00000000;
                break;
            case 0x4:        /* IQFIXOBUF */
                input_value = (signed int)(mIQFIXOBUF);
                break;
            default:        /* no operation*/
                break;
            }
        }
        /* get input value */
        break;

    case emIIR_CH2:                /* IIR2 set variable */
        /* get input value */
        if(mIIRDQ == emAD)
        {
            switch(mpHWCORE_IIRFunc->GetRegVal("IIRCTR2","DATSEL"))
            {
            case 0x0:        /* IIR2 soft input */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRSFTDAT2") & 0xFFFFFFFF);
                break;
            case 0x2:        /* 0 data soft input */
                input_value = 0x00000000;
                break;
            case 0x4:        /* AD0 */
                input_value = (signed int)(mAD0);
                break;
            case 0x5:        /* AD1 */
                input_value = (signed int)(mAD1);
                break;
            case 0x6:        /* AD2 */
                input_value = (signed int)(mAD2);
                break;
            case 0x8:        /* IIR0 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT0") & 0xFFFFFFFF);
                break;
            case 0x9:        /* IIR1 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT1") & 0xFFFFFFFF);
                break;
            case 0xA:        /* IIR2 output */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIROUTDAT2") & 0xFFFFFFFF);
                break;
            default:        /* no operation*/
                break;
            }
        }
        else
        {
            switch(mpHWCORE_IIRFunc->GetRegVal("IIRCTR2","DATSEL"))
            {
            case 0x0:        /* IIR2 soft input */
                input_value = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRSFTDAT2") & 0xFFFFFFFF);
                break;
            case 0x2:        /* 0 data soft input */
                input_value = 0x00000000;
                break;
            default:        /* no operation*/
                break;
            }
        }

    default:
        break;
    }

    return input_value;
}//}}}

///*********************************
/// Function     : IIROperation
/// Description  : Operations of IIR Filter calculation.
// Ref: {HWCORE_IIR_UD_Flow_016, HWCORE_IIR_UD_Direction_046, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::IIROperation(unsigned char iir_ch)
{//{{{
    signed long long coeff_0        = 0;
    signed long long coeff_1        = 0;
    signed long long coeff_2        = 0;
    signed long long coeff_3        = 0;
    signed long long coeff_4        = 0;
    signed long long icoef          = 0;
    signed long long ocoef          = 0;
    signed int       input_value    = 0;
    signed int       iirzn1dat      = 0;
    signed int       iirzn2dat      = 0;
    signed long long result         = 0;
    signed long long now            = 0;
    signed long long d1             = 0;
    signed long long e2             = 0;
    signed long long f2             = 0;
    signed long long g              = 0;
    signed long long iir_mul_temp_0 = 0;
    signed long long iir_mul_temp_1 = 0;
    signed long long iir_mul_temp_2 = 0;
    signed long long iir_mul_temp_3 = 0;
    signed long long iir_mul_temp_4 = 0;

    // Select input data
    input_value = IIRInputSelection(iir_ch);

    switch(iir_ch)
    {
    case emIIR_CH0:                /* IIR0 set variable */
        /* get coefficient */
        icoef   = mCh0_IIRICOEF;
        coeff_0 = mCh0_IIRCOEF0;
        coeff_1 = mCh0_IIRCOEF1;
        coeff_2 = mCh0_IIRCOEF2;
        coeff_3 = mCh0_IIRCOEF3;
        coeff_4 = mCh0_IIRCOEF4;
        ocoef   = mCh0_IIROCOEF;
        /* get coefficient */

        /* get pre value and pre-pre value */
        iirzn1dat = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRZN1DAT0") & 0xFFFFFFFF);
        iirzn2dat = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRZN2DAT0") & 0xFFFFFFFF);
        /* get pre value and pre-pre value */
        break;

    case emIIR_CH1:                /* IIR1 set variable */
        /* get coefficient */
        icoef   = mCh1_IIRICOEF;
        coeff_0 = mCh1_IIRCOEF0;
        coeff_1 = mCh1_IIRCOEF1;
        coeff_2 = mCh1_IIRCOEF2;
        coeff_3 = mCh1_IIRCOEF3;
        coeff_4 = mCh1_IIRCOEF4;
        ocoef   = mCh1_IIROCOEF;
        /* get coefficient */

        /* get pre value and pre-pre value */
        iirzn1dat = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRZN1DAT1") & 0xFFFFFFFF);
        iirzn2dat = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRZN2DAT1") & 0xFFFFFFFF);
        /* get pre value and pre-pre value */
        break;

    case emIIR_CH2:                /* IIR2 set variable */
        /* get coefficient */
        icoef   = mCh2_IIRICOEF;
        coeff_0 = mCh2_IIRCOEF0;
        coeff_1 = mCh2_IIRCOEF1;
        coeff_2 = mCh2_IIRCOEF2;
        coeff_3 = mCh2_IIRCOEF3;
        coeff_4 = mCh2_IIRCOEF4;
        ocoef   = mCh2_IIROCOEF;
        /* get coefficient */

        /* get pre value and pre-pre value */
        iirzn1dat = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRZN1DAT2") & 0xFFFFFFFF);
        iirzn2dat = (signed int)(mpHWCORE_IIRFunc->GetRegVal("IIRZN2DAT2") & 0xFFFFFFFF);
        /* get pre value and pre-pre value */
        break;

    default:
        break;
    }

    /* IIR process */
    // Input scaling
    now            = (input_value * icoef) >> 15;               Chk_overflow_s(now, iir_ch);

    // Two-stage IIR filtering
    iir_mul_temp_4 = (iirzn2dat * coeff_4) >> 14;               Chk_overflow_s(iir_mul_temp_4, iir_ch);
    iir_mul_temp_3 = (iirzn1dat * coeff_3) >> 14;               Chk_overflow_s(iir_mul_temp_3, iir_ch);
    d1             = iir_mul_temp_3 + iir_mul_temp_4;           Chk_overflow_s(d1, iir_ch);

    e2             = d1 + now;                                  Chk_overflow_s(e2, iir_ch);

    iir_mul_temp_2 = (iirzn2dat * coeff_2) >> 14;               Chk_overflow_s(iir_mul_temp_2, iir_ch);
    iir_mul_temp_1 = (iirzn1dat * coeff_1) >> 14;               Chk_overflow_s(iir_mul_temp_1, iir_ch);
    f2             = iir_mul_temp_1 + iir_mul_temp_2;           Chk_overflow_s(f2, iir_ch);

    iir_mul_temp_0 = (e2 * coeff_0) >> 14;                      Chk_overflow_s(iir_mul_temp_0, iir_ch);

    g              = f2 + iir_mul_temp_0;                       Chk_overflow_s(g, iir_ch);

    // Output scaling
    result         = ((g * ocoef) >> 16);                       Chk_overflow_s(result, iir_ch);

    double current_time = sc_time_stamp().to_double();
    switch(iir_ch)
    {
    case emIIR_CH0:
        /* set output */
        mpHWCORE_IIRFunc->SetRegVal("IIROUTDAT0", result & 0xFFFFFFFF);

        /* set pre value(Z-1) */
        mpHWCORE_IIRFunc->SetRegVal("IIRZN2DAT0", mpHWCORE_IIRFunc->GetRegVal("IIRZN1DAT0"));

        mpHWCORE_IIRFunc->SetRegVal("IIRZN1DAT0", (e2 & 0xFFFFFFFF));
        /* set pre value(Z-1) */

        /*Notify IIR channel 0 completion */
        mAssertIIREnd0PortEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);

        break;

    case emIIR_CH1:
        /* set output */
        mpHWCORE_IIRFunc->SetRegVal("IIROUTDAT1", result & 0xFFFFFFFF);
        
        /* set pre value(Z-1) */
        mpHWCORE_IIRFunc->SetRegVal("IIRZN2DAT1", mpHWCORE_IIRFunc->GetRegVal("IIRZN1DAT1"));
        
        mpHWCORE_IIRFunc->SetRegVal("IIRZN1DAT1", (e2 & 0xFFFFFFFF));
        /* set pre value(Z-1) */

        /*Notify IIR channel 1 completion */
        mAssertIIREnd1PortEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);

       break;

    case emIIR_CH2:
        /* set output */
        mpHWCORE_IIRFunc->SetRegVal("IIROUTDAT2", result & 0xFFFFFFFF);
        
        /* set pre value(Z-1) */
        mpHWCORE_IIRFunc->SetRegVal("IIRZN2DAT2", mpHWCORE_IIRFunc->GetRegVal("IIRZN1DAT2"));
        
        mpHWCORE_IIRFunc->SetRegVal("IIRZN1DAT2", (e2 & 0xFFFFFFFF));
        /* set pre value(Z-1) */

        /*Notify IIR channel 2 completion */
        mAssertIIREnd2PortEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);

        break;
    default:
        break;
    }

    /*Output result to iir_output port */
    mWriteIIROutputPortsEvent.notify();
    re_printf("info","IIR Filter channel %d end operation", iir_ch);
}//}}}

///*********************************
/// Function     : Chk_overflow_s
/// Description  : Check for arithmetic operation overflow.
// Ref: {HWCORE_IIR_UD_Direction_048, HWCORE_IIR_UD_Direction_018}
///*********************************
void HWCORE_IIR::Chk_overflow_s(signed long long input, unsigned char channel)
{//{{{
    signed long long check_val1 = 0x000000007fffffffLL;
    signed long long check_val2 = 0xffffffff80000000LL;

    if( ( input > check_val1 ) || (input < check_val2) ) /* Overflow or underflow upon the occurrence */
    {
        double current_time = sc_time_stamp().to_double();
        mAssertIIROverflowOutPortEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mStartPCLKTime), mTimeResolutionUnit);
        mIIROvflFunc_NextVal = channel;
        mWriteIIROvflFuncPortsEvent.notify();
    }
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
///*********************************
/// Function     : CheckClockPeriod 
/// Description  : Check Clock Period
// Ref: {HWCORE_IIR_UD_Direction_037}
///*********************************
bool HWCORE_IIR::CheckClockPeriod (void)
{//{{{
    bool clock_status = false;
    if (mPCLKFreq > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","PCLK frequency is zero\n");
    }
    return clock_status;
}//}}}

///*********************************
/// Function     : GetResetStatus
/// Description  : Get Reset Status
///*********************************
bool HWCORE_IIR::GetResetStatus(const std::string reset_name)
{//{{{
   bool reset_status = false;
    if (reset_name == "rst_n"){
        if (mIsResetHardActive[emrst_nId] || mIsResetCmdActive[emrst_nId]) {
            reset_status = true;
        }
    }

    return reset_status;
}//}}}

///*********************************
/// Function     : callback_IIRSFT_TRG
/// Description  : callback operation of IIRSFT.TRG
// Ref: {HWCORE_IIR_UD_Flow_007}
///*********************************
void HWCORE_IIR::callback_IIRSFT_TRG(void)
{//{{{
    if (mpHWCORE_IIRFunc->GetRegVal("IIRSFT", "TRGSEL") == 0x0)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR0", "TRGSEL") == 0x00)        // confirm whether iir0 trigger is software
        {
            mIIR_reserve[emIIR_CH0] = emIIR_RESERVE;
        }
    }

    if (mpHWCORE_IIRFunc->GetRegVal("IIRSFT", "TRGSEL") == 0x1)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR1", "TRGSEL") == 0x00)        // confirm whether iir1 trigger is software
        {
            mIIR_reserve[emIIR_CH1] = emIIR_RESERVE;
        }
    }

    if (mpHWCORE_IIRFunc->GetRegVal("IIRSFT", "TRGSEL") == 0x2)
    {
        if (mpHWCORE_IIRFunc->GetRegVal("IIRCTR2", "TRGSEL") == 0x00)        // confirm whether iir2 trigger is software
        {
            mIIR_reserve[emIIR_CH2] = emIIR_RESERVE;
        }
    }

    mIIRJudgeStartupEvent.notify();
    mpHWCORE_IIRFunc->SetRegVal("IIRSFT", "TRG", 0x0);
}//}}}

///*********************************
/// Function     : callback_IIRRLD 
/// Description  : callback operation of IIRRLD
// Ref: {HWCORE_IIR_UD_Flow_008}
///*********************************
void HWCORE_IIR::callback_IIRRLD(void)
{
    if (mpHWCORE_IIRFunc->GetRegVal("IIRRLD", "RLD0") == 0x1)        // IIR0
    {
        // set coefficient
        mCh0_IIRICOEF = mpHWCORE_IIRFunc->GetRegVal("IIRICOEF");
        mCh0_IIRCOEF0 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF0");
        mCh0_IIRCOEF1 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF1");
        mCh0_IIRCOEF2 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF2");
        mCh0_IIRCOEF3 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF3");
        mCh0_IIRCOEF4 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF4");
        mCh0_IIROCOEF = mpHWCORE_IIRFunc->GetRegVal("IIROCOEF");
    }

    if (mpHWCORE_IIRFunc->GetRegVal("IIRRLD", "RLD1") == 0x1)        // IIR1
    {
        // set coefficient
        mCh1_IIRICOEF = mpHWCORE_IIRFunc->GetRegVal("IIRICOEF");
        mCh1_IIRCOEF0 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF0");
        mCh1_IIRCOEF1 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF1");
        mCh1_IIRCOEF2 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF2");
        mCh1_IIRCOEF3 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF3");
        mCh1_IIRCOEF4 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF4");
        mCh1_IIROCOEF = mpHWCORE_IIRFunc->GetRegVal("IIROCOEF");
    }

    if (mpHWCORE_IIRFunc->GetRegVal("IIRRLD", "RLD2") == 0x1)        // IIR2
    {
        // set coefficient
        mCh2_IIRICOEF = mpHWCORE_IIRFunc->GetRegVal("IIRICOEF");
        mCh2_IIRCOEF0 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF0");
        mCh2_IIRCOEF1 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF1");
        mCh2_IIRCOEF2 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF2");
        mCh2_IIRCOEF3 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF3");
        mCh2_IIRCOEF4 = mpHWCORE_IIRFunc->GetRegVal("IIRCOEF4");
        mCh2_IIROCOEF = mpHWCORE_IIRFunc->GetRegVal("IIROCOEF");
    }
    if (mEnableStatInfo == true) {
        re_printf("info", "Information of buffer: \n");
        DumpStatInfo();
    }

    mpHWCORE_IIRFunc->SetRegVal("IIRRLD", "RLD2", 0x0);
    mpHWCORE_IIRFunc->SetRegVal("IIRRLD", "RLD1", 0x0);
    mpHWCORE_IIRFunc->SetRegVal("IIRRLD", "RLD0", 0x0);
}

///*********************************
/// Function     : callback_IIRRSL
/// Description  : callback operation of IIRRSL
// Ref: {HWCORE_IIR_UD_Flow_009}
///*********************************
void HWCORE_IIR::callback_IIRRSL(void)
{
    if (mpHWCORE_IIRFunc->GetRegVal("IIRRSL", "RSL") == 0x1)        // IIR0
    {
        // set coefficient
        mpHWCORE_IIRFunc->SetRegVal("IIRICOEF", mCh0_IIRICOEF);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF0", mCh0_IIRCOEF0);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF1", mCh0_IIRCOEF1);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF2", mCh0_IIRCOEF2);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF3", mCh0_IIRCOEF3);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF4", mCh0_IIRCOEF4);
        mpHWCORE_IIRFunc->SetRegVal("IIROCOEF", mCh0_IIROCOEF);
    }

    if (mpHWCORE_IIRFunc->GetRegVal("IIRRSL", "RSL") == 0x2)        // IIR1
    {
        // set coefficient
        mpHWCORE_IIRFunc->SetRegVal("IIRICOEF", mCh1_IIRICOEF);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF0", mCh1_IIRCOEF0);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF1", mCh1_IIRCOEF1);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF2", mCh1_IIRCOEF2);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF3", mCh1_IIRCOEF3);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF4", mCh1_IIRCOEF4);
        mpHWCORE_IIRFunc->SetRegVal("IIROCOEF", mCh1_IIROCOEF);
    }

    if (mpHWCORE_IIRFunc->GetRegVal("IIRRSL", "RSL") == 0x3)        // IIR2
    {
        // set coefficient
        mpHWCORE_IIRFunc->SetRegVal("IIRICOEF", mCh2_IIRICOEF);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF0", mCh2_IIRCOEF0);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF1", mCh2_IIRCOEF1);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF2", mCh2_IIRCOEF2);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF3", mCh2_IIRCOEF3);
        mpHWCORE_IIRFunc->SetRegVal("IIRCOEF4", mCh2_IIRCOEF4);
        mpHWCORE_IIRFunc->SetRegVal("IIROCOEF", mCh2_IIROCOEF);
    }
}
