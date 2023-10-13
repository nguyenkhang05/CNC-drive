// ---------------------------------------------------------------------
// $Id: RHSB_XBAR.cpp 1339 2019-11-14 09:01:36Z chuonghoangle $
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
#include "RHSB_XBAR.h"
#include "RHSB_XBAR_Func.h"
#include "re_define.h"
///*********************************
/// Function     : RHSB_XBAR
/// Description  : Constructor of RHSB_XBAR class
///*********************************
RHSB_XBAR::RHSB_XBAR (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            std::string user):
                sc_module(name),
                RHSB_XBAR_AgentController(),
                BusSlaveBase<32,1>(),
                rvc_common_model(),
                CLK_LSB("CLK_LSB"),
                PRESETn("PRESETn"),
                ATU_C("ATU_C"),
                ATU_D("ATU_D"),
                ATU_E("ATU_E"),
                GTM_TOM("GTM_TOM"),
                GTM_ATOM("GTM_ATOM"),
                GPIO("GPIO"),
                tmr_out("tmr_out")
{//{{{
    //Initialize output signals
    tmr_out.initialize(0x0);
    //Initialize variables - for clock, time resolution, reset
    mCLK_LSBPeriod = 0;
    mCLK_LSBFreq = 0;
    mCLK_LSBOrgFreq = 0;
    mCLK_LSBFreqUnit = "Hz";
    mStartClkTime = 0;
    //Variables
    mtmr_out = 0x0;
    mATU_C = 0x0;
    mATU_D = 0x0;
    mATU_E = 0x0;
    mGTM_ATOM = 0x0;
    mGTM_TOM = 0x0;
    mGPIO = 0x0;
    mtmrH = 0x0;
    mtmrL = 0x0;
    //Initialize RHSB_XBAR_USER parameter
    if (user == "GTM" || user == "ATU") {
        mUser = user;
    } else {
        re_printf("warning", "The argument (%s) of RHSB_XBAR_USER parameter is invalid. GTM user is set as default.\n",user.c_str());
        mUser = "GTM";
    }

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mResetCurVal = true;
    mResetPreVal = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod =  0;
    mEnableRegisterMessage = true;

    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&CLK_LSB);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32((char*)"ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mRHSB_XBARFunc = new RHSB_XBAR_Func((std::string)name, this, mUser);
    sc_assert(mRHSB_XBARFunc != NULL);

    mBusSlaveIf32[0]->setFuncModulePtr(mRHSB_XBARFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandleCLK_LSBSignalMethod);
    dont_initialize();
    sensitive << CLK_LSB;

    SC_METHOD(HandlePRESETNSignalMethod);
    sensitive << PRESETn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(HandleOutputSignalMethod);
    dont_initialize();
    sensitive << mHandleOutputSignalEvent;

    SC_METHOD(HandleInputSignalMethod);
    dont_initialize();
    sensitive << ATU_C;
    sensitive << ATU_D;
    sensitive << ATU_E;
    sensitive << GTM_TOM;
    sensitive << GTM_ATOM;
    sensitive << GPIO;
    sensitive << mAfterResetEvent;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;
}//}}}

///*********************************
/// Function     : ~RHSB_XBAR
/// Description  : Destructor of RHSB_XBAR class
///*********************************
RHSB_XBAR::~RHSB_XBAR (void)
{//{{{ 
    delete mRHSB_XBARFunc;
}//}}}

///*********************************
/// Function     : SetMessageLevel
/// Description  : Set message level (fatal/error/warning/info)
///                Called by PythonIF
///*********************************
void RHSB_XBAR::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mRHSB_XBARFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mRHSB_XBARFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : DumpRegisterRW
/// Description  : Enable/disable dumping message when users access registers
///                Called by PythonIF
///*********************************
void RHSB_XBAR::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mRHSB_XBARFunc->RegisterHandler(cmd);
}//}}}

void RHSB_XBAR::USER (const std::string user)
{//{{{
    if(user == "GTM" || user == "ATU") {
        mUser = user;
    }
    else {
        mUser = "GTM";
    }
}//}}}


void RHSB_XBAR::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }
        else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        }
        else {
            re_printf("warning", "Invalid argument: RHSB_XBAR_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }
    else if (word_vector.size() == 0) {
        std::string temp = "false";
        if (mEnableRegisterMessage) {
            temp = "true";
        }
        else {
            temp = "false";
        }
        re_printf("info", "RHSB_XBAR_EnableRegisterMessage %s\n", temp.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: RHSB_XBAR_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Assert reset by software for PRESETn
///                Called by PythonIF
///*********************************
void RHSB_XBAR::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is ignored because it is called in the reset operation of the model.\n");
        }
    } else {
        re_printf("warning", "The name of reset signal is wrong. It should be PRESETn.\n");
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value and clock unit for CLK_LSB
///                Called by PythonIF
///*********************************
void RHSB_XBAR::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            ConvertClockFreq (mCLK_LSBFreq, mCLK_LSBFreqUnit, freq, unit);
            mStartClkTime = sc_time_stamp().to_double();
            mCLK_LSBOrgFreq = freq;
            if (mCLK_LSBFreq > 0) {
                mCLK_LSBPeriod = (sc_dt::uint64)(((1/(double)mCLK_LSBFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mCLK_LSBPeriod = 0;
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : GetCLKFreq
/// Description  : Get clock value of CLK_LSB
///                Called by PythonIF
///*********************************
void RHSB_XBAR::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CLK_LSB") {
            re_printf("info","CLK_LSB frequency is %0.0f %s.\n", (double)mCLK_LSBOrgFreq, mCLK_LSBFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    }
}//}}}

///*********************************
/// Function     : ForceRegister
/// Description  : Force value to register
///                Called by PythonIF
///*********************************
void RHSB_XBAR::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mRHSB_XBARFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : ReleaseRegister
/// Description  : Release forced value after forcing registers
///                Called by PythonIF
///*********************************
void RHSB_XBAR::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    mRHSB_XBARFunc->RegisterHandler(cmd);
}//}}} 

///*********************************
/// Function     : WriteRegister
/// Description  : Write value to registers by software
///                Called by PythonIF
///*********************************
void RHSB_XBAR::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{ 
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mRHSB_XBARFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
///                Called by PythonIF
///*********************************
void RHSB_XBAR::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    mRHSB_XBARFunc->RegisterHandler(cmd);
}//}}} 

///*********************************
/// Function     : ListRegister
/// Description  : List all register name
///                Called by PythonIF
///*********************************
void RHSB_XBAR::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mRHSB_XBARFunc->RegisterHandler(cmd);
}//}}} 

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
///                Called by PythonIF
///*********************************
void RHSB_XBAR::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"--- parameters ---");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_MessageLevel   (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_DumpRegisterRW (true/false)                     Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_USER           (GTM/ATU)                        Set user for the model (default: GTM).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"--- commands ---");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_AssertReset    (reset_name, start_time, period) Assert and de-assert reset signal (PRESETn).");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_SetCLKFreq     (clock_name, freq, unit)         Set clock frequency to CLK_LSB clock.");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_GetCLKFreq     (clock_name)                     Get clock frequency of CLK_LSB clock.");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_ForceRegister  (reg_name, value)                Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_ReleaseRegister(reg_name)                       Release register from force value.");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_WriteRegister  (reg_name, value)                Write a value to register.");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_ReadRegister   (reg_name)                       Read value from register.");
            SC_REPORT_INFO(this->basename(),"RHSB_XBAR_ListRegister                                    Dump name of RHSB_XBAR's registers.");
        } else {
            re_printf("warning","The name (%s) of RHSB_XBAR_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of RHSB_XBAR_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}} 


///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void RHSB_XBAR::EnableReset (const bool is_active)
{//{{{
    mRHSB_XBARFunc->EnableReset(is_active);
    if (is_active) {
        mtmr_out = 0;
        mtmrH    = 0;
        mtmrL    = 0;
        mHandleOutputSignalEvent.notify();
    } else {
        mAfterResetEvent.notify();
    }
}//}}}

///*********************************
/// Function     : HandleCLK_LSBSignalMethod
/// Description  : Handle CLK_LSB signal
///*********************************
void RHSB_XBAR::HandleCLK_LSBSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CLK_LSB.read();
    SetCLKFreq("CLK_LSB", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod
/// Description  : Process reset function when PRESETn command is active
///*********************************
void RHSB_XBAR::HandleResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///*********************************
/// Function     : CancelResetCmdMethod
/// Description  : Cancel reset function when PRESETn command is active
///*********************************
void RHSB_XBAR::CancelResetCmdMethod (void)
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of PRESETn is over.\n");
    this->EnableReset(mIsResetCmdActive); 
}//}}}

///*********************************
/// Function     : HandlePRESETNSignalMethod
/// Description  : Handle PRESETn signal
///*********************************
void RHSB_XBAR::HandlePRESETNSignalMethod (void)
{//{{{
    mResetCurVal = PRESETn.read();
    if (mResetCurVal != mResetPreVal) {//mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        mResetHardEvent.notify();
    }
}//}}}

///*********************************
/// Function     : HandleResetHardMethod
/// Description  : Process reset function when PRESETn is active
///*********************************
void RHSB_XBAR::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset signal PRESETn is asserted.\n");
        this->EnableReset(mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        this->EnableReset(mIsResetHardActive);
        re_printf("info","The reset signal PRESETn is de-asserted.\n");
    }
    //Cancel AssertReset command when PRESETn is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///*********************************
/// Function     : CheckClockPeriod
/// Description  : Check clock period of CLK_LSB
///                Virtual function of RHSB_XBAR_AgentController class
///*********************************
bool RHSB_XBAR::CheckClockPeriod (void)
{//{{{ 
    bool status = false;
    if (mCLK_LSBPeriod > 0) {
        status = true;
    }
    if (!status) {
        re_printf("info","CLK_LSB frequency is zero.\n");
    }
    return status;
}//}}}

bool RHSB_XBAR::GetResetStatus(void)
{//{{{ 
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

///*********************************
///Function      : HandleATU_CSignalMethod
///Description   : Handle the ATU_C signal
///*********************************
void RHSB_XBAR::HandleInputSignalMethod (void)
{//{{{
    if (!GetResetStatus() && CheckClockPeriod()) {
        if (mUser == "ATU") {
            mATU_C = ATU_C.read();
            mATU_D = ATU_D.read();
            mATU_E = ATU_E.read();
            mGPIO = GPIO.read();
            mRHSB_XBARFunc->CheckSettingReg();
        } else {
            mGTM_TOM = GTM_TOM.read();
            mGTM_ATOM = GTM_ATOM.read();
            mGPIO = GPIO.read();
            mRHSB_XBARFunc->CheckSettingReg();
       }
    }
}//}}}

void RHSB_XBAR::HandleOutputSignalMethod (void)
{//{{{
    if (CheckClockPeriod()) {
        mtmr_out = (((mtmrH & 0xFFFFFFFF)<<32)&0xFFFFFFFF00000000ULL) | (mtmrL&0xFFFFFFFF);    
        tmr_out.write(mtmr_out);
        re_printf("info","The output value is 0x%llX",(sc_dt::uint64) mtmr_out);
    }
}//}}}
///*********************************
///Function      : SelectforOutput
///Description   : Select the input signal to be the output signal tmr_out
///*********************************
void RHSB_XBAR::SelectforOutput (const unsigned int pos_out, const unsigned int pos_in, const std::string from_input)
{//{{{
    sc_dt::uint64 temp_input = 0;
    sc_dt::uint64 temp_pos_in = (sc_dt::uint64)pos_in;
    sc_dt::uint64 temp_pos_out =(sc_dt::uint64)pos_out;
    if (from_input == "c") {
        //tmr_out [pos_out] = ATU_C [pos_in]
        temp_input = (mATU_C >> pos_in) & 1;
        if (temp_pos_out < 32) {
            UpdateValue(false, temp_pos_out, temp_input);
        } else {
            UpdateValue(true, temp_pos_out, temp_input);
        }
    } else if (from_input == "d") {
        //tmr_out [pos_out] = ATU_D [pos_in]
        temp_input = (mATU_D >> pos_in) & 1;
        if (temp_pos_out < 32) {
            UpdateValue(false, temp_pos_out, temp_input);
        } else {
            UpdateValue(true, temp_pos_out, temp_input);
        }
    } else if (from_input == "e") {
        //tmr_out [pos_out] = ATU_E [pos_in]
        temp_input = (mATU_E >> pos_in) & 1;
        if (temp_pos_out < 32) {
            UpdateValue(false, temp_pos_out, temp_input);
        } else {
            UpdateValue(true, temp_pos_out, temp_input);
        }
    } else if (from_input == "TOM") {
        //tmr_out [temp_pos_out] = GTM_TOM [temp_pos_in]
        temp_input = ((mGTM_TOM >> temp_pos_in) & 1);
        if (temp_pos_out < 32) {
            UpdateValue(false, temp_pos_out, temp_input);
        } else {
            UpdateValue(true, temp_pos_out, temp_input);
        }
    } else if (from_input == "ATOM") {
        //tmr_out [temp_pos_out] = GTM_ATOM [temp_pos_in]
        temp_input =  (sc_dt::uint64) ((mGTM_ATOM >> temp_pos_in) & 1);
        if (temp_pos_out < 32) {
            UpdateValue(false, temp_pos_out, temp_input);
        } else {
            UpdateValue(true, temp_pos_out, temp_input);
        }
    } else if (from_input == "GPIO") {
        //tmr_out [temp_pos_out] = GTM_TOM [temp_pos_in]
        temp_input =  ((mGPIO >> temp_pos_in) & 1);
        if (temp_pos_out < 32) {
            UpdateValue(false, temp_pos_out, temp_input);
        } else {
            UpdateValue(true, temp_pos_out, temp_input);
        }
    } else { //(from_input == "Low")
        //tmr_out [temp_pos_out] = Low level
        if (temp_pos_out < 32) {
            mtmrL &= (~(1 << temp_pos_out));
        } else {
            mtmrH &= (~(1 << (temp_pos_out-32)));
        }
    }
}//}}}

///*********************************
///Function      : UpdateValue
///Description   : Update value of mtmrL and mtmrH
///*********************************
void RHSB_XBAR::UpdateValue(const bool position, const sc_dt::uint64 temp_pos_out, const sc_dt::uint64 temp_input)
{//{{{
    if (!position) {
        if (temp_input == 1) {
            mtmrL |= (1ULL << temp_pos_out);
        } else {
            mtmrL &= (~(1ULL << temp_pos_out));
        }
    } else {
        if (temp_input == 1) {
            mtmrH |= (1ULL << (temp_pos_out - 32));
        } else {
            mtmrH &= (~(1ULL << (temp_pos_out-32)));
        }
    }
}//}}}

///*********************************
///Function      : WriteOutput
///Description   : WriteOutput
///*********************************
void RHSB_XBAR::WriteOutput(void)
{//{{{
    double cur_time = sc_time_stamp().to_double();
    mHandleOutputSignalEvent.notify(CalculateCLKPosEdge((double)mCLK_LSBPeriod, false, cur_time, mStartClkTime), mTimeResolutionUnit); 
}//}}}
// vim600: set foldmethod=marker :
