// ---------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2017 Renesas Electronics Corporation
// Copyright(c) 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "DNFP020_E2.h"
#include "DNFP020_E2_Func.h"
#include "DNFP020_E2_Sub.h"
#include "DNFP020_E2_Sub_Func.h"
#include <iostream>

///Constructor of DNFP020_E2 class

DNFP020_E2::DNFP020_E2 (sc_module_name name,
			uint8_t mChNum,
            unsigned int rLatency,
            unsigned int wLatency):
            sc_module(name),
            BusSlaveBase<32,1>(),
            PCLK("PCLK"),
            PRESETn("PRESETn")
{//{{{
	 //Initialize variables
    mPCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz";

    mResetCurVal = true;
    mResetPreVal = true;
    mIsResetHardActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive = false;
    mResetCmdPeriod =  0;

    mTimeCLKChangePoint = 0;
	if ((mChNum >= 1) && (mChNum <= 16)){
		mChanNum = mChNum;
	} else {
		if (mChNum < 1){
			re_printf("warning","Channel id (%d) is not in range [1 - 16]. Set to 1.", mChNum);
			mChanNum = 1;
		}
		if (mChNum > 16){
			re_printf("warning","Channel id (%d) is not in range [1 - 16]. Set to 16.", mChNum);
			mChanNum = 16;
		}
	}

    // Initialize internal variables
    mDNFP020_E2_Sub = new DNFP020_E2_Sub*[mChanNum];
    DNFEDGEIN = new sc_in<bool>*[mChanNum];


    // Initialize internal signal

    // Initialize Output port
    //Bind clock, reset signal and socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mDNFP020_E2_Func = new DNFP020_E2_Func((std::string)name, this);
    sc_assert(mDNFP020_E2_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mDNFP020_E2_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    std::ostringstream port_name;
    for (unsigned int index = 0; index < mChanNum; index++) {
        port_name.str("");
        port_name << "DNFEDGEIN" << index;
        DNFEDGEIN[index] = new sc_in<bool>(port_name.str().c_str());
        sc_assert(DNFEDGEIN[index] != NULL);
    }

    std::ostringstream ins_name;
    for (unsigned int i = 0; i < mChanNum; i++) {
        ins_name.str("");
        ins_name << "DNFP020_E2_Sub" << i;
        mDNFP020_E2_Sub[i]  = new DNFP020_E2_Sub(ins_name.str().c_str() , this, i );
        sc_assert(mDNFP020_E2_Sub[i] != NULL);
    }
    /// Port connection
    for (unsigned int i = 0; i < mChanNum; i++) {
        ((mDNFP020_E2_Sub[i]->DNFEDGEIN))(*(DNFEDGEIN[i]));
    }

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive << PRESETn;


    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;
}//}}}

///Destructor of DNFP020_E2 class
DNFP020_E2::~DNFP020_E2 (void)
{//{{{
    delete mDNFP020_E2_Func;

    for (unsigned int i = 0; i < mChanNum; i++) {
        delete mDNFP020_E2_Sub[i];
    }
    delete [] mDNFP020_E2_Sub;

	for (unsigned int i = 0; i < mChanNum; i++) {
		delete DNFEDGEIN[i];
	}
	delete [] DNFEDGEIN;
}//}}}

///Assert reset by software
void DNFP020_E2::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be PRESETn\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void DNFP020_E2::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mTimeCLKChangePoint = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent.notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value
void DNFP020_E2::GetCLKFreq (const std::string clock_name)
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


///Force value to register
void DNFP020_E2::ForceRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void DNFP020_E2::ReleaseRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandler(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void DNFP020_E2::WriteRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name, const unsigned int reg_value)
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
            RegisterHandler(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void DNFP020_E2::ReadRegister (const bool is_com_reg, const unsigned int ch_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandler(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Wrapper for accessing registers
void DNFP020_E2::RegisterHandler (const bool is_com_reg, const unsigned int ch_id, const std::vector<std::string> &args)
{//{{{
    if (ch_id < mChanNum) {
        if(is_com_reg) {
            mDNFP020_E2_Func->RegisterHandler(args);
        }
        else {
            if (args.size() == 1) {//Dump registers name
                for (unsigned int index = 0; index < mChanNum; index++) {//Dump Reg of all sub
                    mDNFP020_E2_Sub[index]->mDNFP020_E2_Sub_Func->RegisterHandler(args);
                }
            } else {
                if (args[1] == "*") {//Set message level for each register
                    for (unsigned int index = 0; index < mChanNum; index++) {//Set Message level to all sub
                        mDNFP020_E2_Sub[index]->mDNFP020_E2_Sub_Func->RegisterHandler(args);
                    }
                } else if  (args[1] == "DumpRegisterRW") {
                    for (unsigned int index = 0; index < mChanNum; index++) {//Set Message level to all sub
                        mDNFP020_E2_Sub[index]->mDNFP020_E2_Sub_Func->RegisterHandler(args);
                    }
                } else {
                    mDNFP020_E2_Sub[ch_id]->mDNFP020_E2_Sub_Func->RegisterHandler(args);
                }
            }
        }
    } else {
        re_printf("warning","chanel id (%d) is invalid\n", ch_id);
    }
}//}}}

///List all registers name
void DNFP020_E2::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    RegisterHandler(true,0,cmd);
    RegisterHandler(false,0,cmd);
}//}}}

///Dump help message of all parameters or commands
void DNFP020_E2::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_MessageLevel (fatal|error|warning|info)                       Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_DumpRegisterRW (true/false)                                   Enable/disable dumping access register (Default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_AssertReset (reset_name, start_time, period)                  Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_SetCLKFreq (clock_name, freq, unit)                           Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_GetCLKFreq (clock_name)                                       Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_ForceRegister (is_com_reg, ch_id, reg_name, reg_value)        Force DNFP020_E2’s register with setting value.");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_ReleaseRegister (is_com_reg, ch_id, reg_name)                 Release DNFP020_E2’s register from force value.");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_WriteRegister (is_com_reg, ch_id, reg_name, reg_value)        Write a value to a DNFP020_E2’s register.");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_ReadRegister (is_com_reg, ch_id, reg_name)                    Read a value from a DNFP020_E2’s register.");
            SC_REPORT_INFO(this->basename(),"DNFP020_E2_ListRegister ()                                               Dump name of model's registers.");
        } else {
            re_printf("warning","The name (%s) of DNFP020_E2_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of DNFP020_E2_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void DNFP020_E2::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    for (unsigned int index = 0; index < mChanNum; index++) {//Dump Reg of all sub
        std::string msg_ret = mDNFP020_E2_Sub[index]->handleCommand(cmd);
        if (msg_ret != "") {
            SC_REPORT_INFO(mDNFP020_E2_Sub[index]->basename(), msg_ret.c_str());
        }
    }

    //Set message level for Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    RegisterHandler(true,0,cmd);
    RegisterHandler(false,0,cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    RegisterHandler(true,0,cmd);
    RegisterHandler(false,0,cmd);
}//}}}

///Enable/disable dumping message when users access registers
void DNFP020_E2::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    RegisterHandler(true,0,cmd);
    RegisterHandler(false,0,cmd);
}//}}}


///Check frequency value and frequency unit
void DNFP020_E2::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                             sc_dt::uint64 freq_in, std::string unit_in)
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
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

///Separate some words from a string to store a vector
void DNFP020_E2::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

///Get time resolution
void DNFP020_E2::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Check reset status
bool DNFP020_E2::GetResetStatus (const std::string reset_name)
{//{{{
    bool reset_status = false;
    if(reset_name=="PRESETn") {
        if (mIsResetHardActive || mIsResetCmdActive) {
            reset_status = true;
        }
    }
    return reset_status;
}//}}}

///Check clock period value
bool DNFP020_E2::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "PCLK") ? mPCLKPeriod : 0;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Handle PCLK signal
void DNFP020_E2::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}


///Handle PRESETn signal
void DNFP020_E2::HandlePRESETnSignalMethod (void)
{//{{{
    mResetCurVal = PRESETn.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void DNFP020_E2::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port PRESETn is asserted.\n");
        this->EnableReset("PRESETn", mIsResetHardActive);
    } else {
        if (mIsResetHardActive) {
            mIsResetHardActive = false;
            re_printf("info","The reset port PRESETn is de-asserted.\n");
            this->EnableReset("PRESETn", mIsResetHardActive);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///Process reset function when reset command is active
void DNFP020_E2::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "PRESETn";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void DNFP020_E2::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "PRESETn";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
}//}}}


///Execute reset operation
void DNFP020_E2::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if(is_active) {
        // Cancel events
        CancelEvents();
    }
    mDNFP020_E2_Func->Reset(is_active);
    for (unsigned int i = 0; i < mChanNum; i++) {
        mDNFP020_E2_Sub[i]->EnableReset (reset_name, is_active);
    }
}//}}}

///Calculate synchronous time to next clock edge
double DNFP020_E2::CalculateNextClockEdge (const double clock_period, const bool is_pos, const double current_time, const double start_time)
{//{{{
    double period_num = 0;
    if (clock_period == 0){
        return 0;
    }
    period_num = (current_time - start_time) / clock_period;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (is_pos) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        if (((double)mod_period_num + 0.5) < period_num) {
            mod_period_num++;
        }
    }
    double mod_period_num_db = 0;
    if (is_pos) {
        mod_period_num_db = (double)mod_period_num;
    } else {
        mod_period_num_db = (double)mod_period_num + 0.5;
    }
    return ((mod_period_num_db * (double)clock_period) - (current_time - start_time));
}//}}}

// Cancel operation events
void DNFP020_E2::CancelEvents (void)
{//{{{
    for (unsigned int index = 0; index < mChanNum; index++) {
        mDNFP020_E2_Sub[index]->CancelEvents();
    }
}//}}}

///Virtual function of ReadSub class
bool DNFP020_E2::GetSubNFEN (void)
{//{{{
    bool status = false;
    for (unsigned int index = 0; index < mChanNum; index++) {
        if(mDNFP020_E2_Sub[index]->mDNFP020_E2_Sub_Func->GetNFEN()) {
            status = true;
            break;
        }
    }
    return status;
}//}}}

void DNFP020_E2::ReadSub (unsigned int ch_id, unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    unsigned int offset_size = ((unsigned int)trans.get_address() & 0x3);
    if(ch_id >= mChanNum) {
        re_printf("error", "Invalid access address 0x%08X\n", (unsigned int)trans.get_address());
    }
    else if((offset_size+(unsigned int)trans.get_data_length())>4 && debug) {
        re_printf("error", "Reading access size at address 0x%08X is wrong: %d byte(s).\n", (unsigned int)trans.get_address(),(unsigned int)trans.get_data_length());
    }
    else {
        mDNFP020_E2_Sub[ch_id]->mDNFP020_E2_Sub_Func->read(offsetAddress, trans, t, debug);
    }
}//}}}

///Virtual function of WriteSub class
void DNFP020_E2::WriteSub (unsigned int ch_id, unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int offset_size = ((unsigned int)trans.get_address() & 0x3);
    if(ch_id >= mChanNum) {
        re_printf("error", "Invalid access address 0x%08X\n", (unsigned int)trans.get_address());
    }
    else if((offset_size+(unsigned int)trans.get_data_length())>4 && debug) {
        re_printf("error", "Writing access size at address 0x%08X is wrong: %d byte(s).\n", (unsigned int)trans.get_address(),(unsigned int)trans.get_data_length());
    }
    else {
        mDNFP020_E2_Sub[ch_id]->mDNFP020_E2_Sub_Func->write(offsetAddress, trans, t, debug);
    }
}//}}}

unsigned int DNFP020_E2::GetSLST(void)
{//{{{
    return mDNFP020_E2_Func->GetSLST();
}//}}}

unsigned int DNFP020_E2::GetPRS(void)
{//{{{
    return mDNFP020_E2_Func->GetPRS();
}//}}}

// vim600: set foldmethod=marker :
