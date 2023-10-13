// ---------------------------------------------------------------------
// $Id: AXISTB.cpp,v 1.3 2014/05/05 01:23:39 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "AXISTB.h"
#include "AXISTB_DataHandler.h"
#include "re_define.h"

///Constructor of AXISTB class
AXISTB::AXISTB (sc_module_name name,
                unsigned int rLatency,
                unsigned int wLatency):
                sc_module(name),
                AXISTB_AgentController(),
                BusBridgeSlaveBase<64,2>(),
                BusMasterBase<64,2>(),
                BusSlaveBase<32,1>(),
                PCLK("PCLK"),
                HCLK("HCLK"),
                PRESETn("PRESETn"),
                HRESETn("HRESETn"),
                mresetPortSig("mresetPortSig")
{//{{{
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&PRESETn);
    setTargetSocket32("tsp");

    setMasterResetPort64(&HRESETn, &HRESETn);
    setMasterFreqPort64(&HCLK, &HCLK);
    setInitiatorSocket64("isg", "iss");

    setSlaveResetPort(&HRESETn, &HRESETn);
    setSlaveFreqPort(&HCLK, &HCLK);
    setTargetSocket("tsg", "tss");

    tsp = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    tsg = BusBridgeSlaveBase<64,2>::tSocket[0]; //Alias for binding
    tss = BusBridgeSlaveBase<64,2>::tSocket[1]; //Alias for binding
    isg = iSocket64[0]; //Alias for binding
    iss = iSocket64[1]; //Alias for binding

    mBusBridgeModule[0] = new BusBridgeModuleBase<64,64> ((std::string(name) + "_GRI").c_str());
    (mBusBridgeModule[0]->resetPort).bind(mresetPortSig);

    mBusBridgeModule[1] = new BusBridgeModuleBase<64,64> ((std::string(name) + "_SIC").c_str());
    (mBusBridgeModule[1]->resetPort).bind(mresetPortSig);

    mresetPortSig.write(true);//Inactivate resetPort of BusBridgeModuleBase's instances

    mAXISTBFunc = new AXISTBFunc((std::string)name, this);
    sc_assert(mAXISTBFunc != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mAXISTBFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    mAXISTBDataHandlerGRI = new AXISTB_DataHandler((std::string)name, "GRI", this->mBusBridgeModule[0], mBusSlaveIf[0], mBusMasterIf64[0], this);
    mBusBridgeModule[0]->mpCore = mAXISTBDataHandlerGRI;
    mBusMasterIf64[0]->setFuncModulePtr((BusMasterFuncIf *)(mBusBridgeModule[0]->mpCore));
    mBusMasterIf64[0]->setBusProtocol(BUS_AXI);
    mBusMasterIf64[0]->setTransNmbLmt(0xFFFFFFFF);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)(mBusBridgeModule[0]->mpCore));
    mBusSlaveIf[0]->setBusProtocol(BUS_AXI);
    mBusSlaveIf[0]->setTransNmbLmt(0xFFFFFFFF);
    
    mAXISTBDataHandlerSIC = new AXISTB_DataHandler((std::string)name, "SIC", this->mBusBridgeModule[1], mBusSlaveIf[1], mBusMasterIf64[1], this);
    mBusBridgeModule[1]->mpCore = mAXISTBDataHandlerSIC;
    mBusMasterIf64[1]->setFuncModulePtr((BusMasterFuncIf *)(mBusBridgeModule[1]->mpCore));
    mBusMasterIf64[1]->setBusProtocol(BUS_AXI);
    mBusMasterIf64[1]->setTransNmbLmt(0xFFFFFFFF);
    mBusSlaveIf[1]->setFuncModulePtr( (BusSlaveFuncIf *)(mBusBridgeModule[1]->mpCore));
    mBusSlaveIf[1]->setBusProtocol(BUS_AXI);
    mBusSlaveIf[1]->setTransNmbLmt(0xFFFFFFFF);

    //Initialize variables
    mIsStartSim = false;
    mPCLKPeriod = 0;
    mHCLKPeriod = 0;
    mPCLKFreq = 0;
    mHCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mHCLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz"; 
    mHCLKFreqUnit = "Hz"; 
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mTransLTConter[0] = 0;
    mTransLTConter[1] = 0;

    mPRESETnSignalCurrentValue = emRstInActive;
    mPRESETnSignalPreviousValue = emRstInActive;
    mIsPRESETnHardActive = false;
    mIsPRESETnCmdReceive = false;
    mIsPRESETnCmdActive = false;
    mPRESETnCmdPeriod =  0;

    mHRESETnSignalCurrentValue = emRstInActive;
    mHRESETnSignalPreviousValue = emRstInActive;
    mIsHRESETnHardActive = false;
    mIsHRESETnCmdReceive = false;
    mIsHRESETnCmdActive = false;
    mHRESETnCmdPeriod =  0;

    SetBufferSize("Default", emDefaultBufferSize);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive<<PCLK;
 
    SC_METHOD(HandleHCLKSignalMethod);
    dont_initialize();
    sensitive<<HCLK;
 
    SC_METHOD(HandlePRESETnSignalMethod);
    sensitive<<PRESETn;
    
    SC_METHOD(HandlePRESETnHardMethod);
    dont_initialize();
    sensitive<<mPRESETnHardEvent;

    SC_METHOD(HandlePRESETnCmdMethod);
    dont_initialize();
    sensitive<<mPRESETnCmdEvent;
 
    SC_METHOD(CancelPRESETnCmdMethod);
    dont_initialize();
    sensitive<<mPRESETnCmdCancelEvent;

    SC_METHOD(HandleHRESETnSignalMethod);
    sensitive<<HRESETn;
    
    SC_METHOD(HandleHRESETnHardMethod);
    dont_initialize();
    sensitive<<mHRESETnHardEvent;

    SC_METHOD(HandleHRESETnCmdMethod);
    dont_initialize();
    sensitive<<mHRESETnCmdEvent;
 
    SC_METHOD(CancelHRESETnCmdMethod);
    dont_initialize();
    sensitive<<mHRESETnCmdCancelEvent;

    SC_THREAD(SendDataFromBufGRIThread);
    dont_initialize();
    sensitive<<mGetDataFromBufGRIEvent;
    sensitive<<mHRESETnEvent;

    SC_THREAD(SendDataFromBufSICThread);
    dont_initialize();
    sensitive<<mGetDataFromBufSICEvent;
    sensitive<<mHRESETnEvent;

    SC_METHOD(NotifyGRITransferringCompleteMethod);
    dont_initialize();
    sensitive<<mNotifyGRITransCompEvent;

    SC_METHOD(NotifySICTransferringCompleteMethod);
    dont_initialize();
    sensitive<<mNotifySICTransCompEvent;

    SC_METHOD(NotifyGRITransferringCancelMethod);
    dont_initialize();
    sensitive<<mNotifyGRITransCancelEvent;

    SC_METHOD(NotifySICTransferringCancelMethod);
    dont_initialize();
    sensitive<<mNotifySICTransCancelEvent;
}//}}}

///Destructor of AXISTB class
AXISTB::~AXISTB (void)
{//{{{
    delete mAXISTBFunc;
    delete mAXISTBDataHandlerGRI;
    delete mAXISTBDataHandlerSIC;
    delete[] mGRICompLTEvent;
    delete[] mSICCompLTEvent;
}//}}}

void AXISTB::start_of_simulation(void)
{//{{{
    mIsStartSim = true;
}//}}}

///Set TLM transaction type
void AXISTB::setTlmTransType(SIM_MODE_T simMode)
{//{{{
    mBusBridgeModule[0]->setTlmTransType(simMode);
    mBusBridgeModule[1]->setTlmTransType(simMode);
}//}}}

///Dump information of registers
void AXISTB::DumpStatusInfo (void)
{//{{{
    mAXISTBFunc->DumpStatusInfo();
}//}}}

///Assert reset by software for PRESETn and reset
void AXISTB::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "PRESETn") {
        if ((!mIsPRESETnHardActive)&&(!mIsPRESETnCmdReceive)) {
            mIsPRESETnCmdReceive = true;
            re_printf("info","The model will be reset (PRESETn) for %f ns after %f ns.\n", period, start_time);
            mPRESETnCmdEvent.notify(start_time, SC_NS);
            mPRESETnCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of PRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "HRESETn") {
        if ((!mIsHRESETnHardActive)&&(!mIsHRESETnCmdReceive)) {
            mIsHRESETnCmdReceive = true;
            re_printf("info","The model will be reset (HRESETn) for %f ns after %f ns.\n", period, start_time);
            mHRESETnCmdEvent.notify(start_time, SC_NS);
            mHRESETnCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of HRESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be PRESETn or HRESETn.\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit for PCLK and clk
void AXISTB::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
            }
        } else if (word_vector[0] == "HCLK") {
            mHCLKOrgFreq = freq;
            ConvertClockFreq (mHCLKFreq, mHCLKFreqUnit, freq, unit);
            if (mHCLKFreq > 0) {
                mHCLKPeriod = (sc_dt::uint64)(((1/(double)mHCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mHCLKPeriod = 0;
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock value of PCLK and clk
void AXISTB::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "HCLK") {
            re_printf("info","HCLK frequency is %0.0f %s.\n", (double)mHCLKOrgFreq, mHCLKFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Force value to register
void AXISTB::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mAXISTBFunc->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void AXISTB::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mAXISTBFunc->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void AXISTB::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            mAXISTBFunc->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void AXISTB::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            std::vector<std::string> cmd;
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mAXISTBFunc->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///List all reigsters name
void AXISTB::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mAXISTBFunc->RegisterHandler(cmd);
}//}}}

///Dump help message of all parameters or commands
void AXISTB::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"AXI_Standby_MessageLevel (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_DumpRegisterRW (true/false)                   Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_GRIBufferNum (size)                           Set buffer size for buffer GRI.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_SICBufferNum (size)                           Set buffer size for buffer SIC.");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"AXI_Standby_DumpStatusInfo                                Dump the status information.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_AssertReset (reset_name, start_time, period)  Assert/De-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_SetCLKFreq (clock_name, freq, unit)           Set clock frequency to PCLK, HCLK clock.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_GetCLKFreq (clock_name)                       Get clock frequency of PCLK, HCLK clock.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_ForceRegister (reg_name, reg_value)           Force a value to a register.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_ReleaseRegister (reg_name)                    Release forced value of a register.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_WriteRegister (reg_name, reg_value)           Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_ReadRegister (reg_name)                       Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"AXI_Standby_ListRegister                                  List all registers of the model.");
        } else {
            re_printf("error","The name (%s) of AXI_Standby_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of AXI_Standby_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void AXISTB::SetMessageLevel (const std::string msg_lv)
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
    mAXISTBFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAXISTBFunc->RegisterHandler(cmd);
}//}}}

///Enable/disable dumping message when users access registers
void AXISTB::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mAXISTBFunc->RegisterHandler(cmd);
}//}}}

///Set buffer size
void AXISTB::SetBufferSize(const std::string name, const int size)
{//{{{
    if (mIsStartSim) {
        re_printf("warning","Cannot set buffer size during simulation.\n");
    } else {
        if ((name == "GRI") || (name == "SIC") || (name == "Default")) {
            unsigned int buf_size = emDefaultBufferSize;
            if (size < 1) {
                re_printf("warning","Value of buffer size is invalid. It should be greater than 0.\n");
            } else {
                buf_size = (unsigned int)size;
            }
            if (name == "Default") {
                mBufferSize[0] = emDefaultBufferSize;
                mGRICompLTEvent = new sc_event [emDefaultBufferSize];
                mBufferSize[1] = emDefaultBufferSize;
                mSICCompLTEvent = new sc_event [emDefaultBufferSize];
            } else if (name == "GRI") {
                mBufferSize[0] = buf_size;
                delete[] mGRICompLTEvent;
                mGRICompLTEvent = new sc_event [buf_size];
            } else {//SIC
                mBufferSize[1] = buf_size;
                delete[] mSICCompLTEvent;
                mSICCompLTEvent = new sc_event [buf_size];
            }
            if (name != "Default") {
                re_printf("info","Size of %s buffer is set to %u.\n", name.c_str(), buf_size);
            }
        }
    }
}//}}}

///Handle PCLK signal
void AXISTB::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle HCLK signal
void AXISTB::HandleHCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = HCLK.read();
    SetCLKFreq("HCLK", freq_value, "Hz");
}//}}}

///Handle PRESETn signal
void AXISTB::HandlePRESETnSignalMethod (void)
{//{{{
    mPRESETnSignalCurrentValue = PRESETn.read();
    if (mPRESETnSignalCurrentValue != mPRESETnSignalPreviousValue) {//mPRESETnSignalPreviousValue is "true" at initial state
        mPRESETnSignalPreviousValue = mPRESETnSignalCurrentValue;
        if (mPCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mPRESETnHardEvent.notify(CalculateCLKPosEdge("PCLK", current_time), mTimeResolutionUnit);
        } else {
            mPRESETnHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle HRESETn signal
void AXISTB::HandleHRESETnSignalMethod (void)
{//{{{
    mHRESETnSignalCurrentValue = HRESETn.read();
    mresetPortSig.write(mHRESETnSignalCurrentValue);
    if (mHRESETnSignalCurrentValue != mHRESETnSignalPreviousValue) {//mHRESETnSignalPreviousValue is "false" at initial state
        mHRESETnSignalPreviousValue = mHRESETnSignalCurrentValue;
        if (mHCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mHRESETnHardEvent.notify(CalculateCLKPosEdge("HCLK", current_time), mTimeResolutionUnit);
        } else {
            mHRESETnHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when PRESETn is active
void AXISTB::HandlePRESETnHardMethod (void)
{//{{{
    if (mPRESETnSignalCurrentValue == emRstActive) {
        mIsPRESETnHardActive = true;
        re_printf("info","The reset signal PRESETn is asserted.\n");
        this->EnableReset("PRESETn", mIsPRESETnHardActive);
    } else if (mIsPRESETnHardActive) {
        mIsPRESETnHardActive = false;
        this->EnableReset("PRESETn", mIsPRESETnHardActive);
        re_printf("info","The reset signal PRESETn is de-asserted.\n");
    }
    //Cancel AssertReset command when PRESETn is active
    if (mIsPRESETnCmdReceive) {
        mPRESETnCmdEvent.cancel();
        mPRESETnCmdCancelEvent.cancel();
        mIsPRESETnCmdReceive = false;
        mIsPRESETnCmdActive = false;
    }
}//}}}

///Process reset function when HRESETn is active
void AXISTB::HandleHRESETnHardMethod (void)
{//{{{
    if (mHRESETnSignalCurrentValue == emRstActive) {
        mIsHRESETnHardActive = true;
        re_printf("info","The reset signal HRESETn is asserted.\n");
        this->EnableReset("HRESETn", mIsHRESETnHardActive);
    } else if (mIsHRESETnHardActive) {
        mIsHRESETnHardActive = false;
        this->EnableReset("HRESETn", mIsHRESETnHardActive);
        re_printf("info","The reset signal HRESETn is de-asserted.\n");
    }
    //Cancel AssertReset command when HRESETn is active
    if (mIsHRESETnCmdReceive) {
        mHRESETnCmdEvent.cancel();
        mHRESETnCmdCancelEvent.cancel();
        mIsHRESETnCmdReceive = false;
        mIsHRESETnCmdActive = false;
    }
}//}}}

///Process reset function when PRESETn command is active
void AXISTB::HandlePRESETnCmdMethod (void)
{//{{{
    mIsPRESETnCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of PRESETn.\n");
    this->EnableReset("PRESETn", mIsPRESETnCmdActive);
    mPRESETnCmdCancelEvent.notify(mPRESETnCmdPeriod, SC_NS);
}//}}}

///Process reset function when HRESETn command is active
void AXISTB::HandleHRESETnCmdMethod (void)
{//{{{
    mIsHRESETnCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of HRESETn.\n");
    this->EnableReset("HRESETn", mIsHRESETnCmdActive);
    mHRESETnCmdCancelEvent.notify(mHRESETnCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when PRESETn command is active
void AXISTB::CancelPRESETnCmdMethod (void)
{//{{{
    mIsPRESETnCmdActive = false;
    mIsPRESETnCmdReceive = false;
    re_printf("info","Reset period of PRESETn is over.\n");
    this->EnableReset("PRESETn", mIsPRESETnCmdActive); 
}//}}}

///Cancel reset function when HRESETn command is active
void AXISTB::CancelHRESETnCmdMethod (void)
{//{{{
    mIsHRESETnCmdActive = false;
    mIsHRESETnCmdReceive = false;
    re_printf("info","Reset period of reset is over.\n");
    this->EnableReset("HRESETn", mIsHRESETnCmdActive); 
}//}}}

///Process reset function
void AXISTB::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mAXISTBFunc->Reset(reset_name, is_active);
    if (is_active) {
        if (reset_name == "PRESETn") {
            if (!mBufferGRI.empty()) {
                ContinueTransferring("GRI");
            }
            if (!mBufferSIC.empty()) {
                ContinueTransferring("SIC");
            }
        } else {
            mHRESETnEvent.notify();
            mGetDataFromBufGRIEvent.cancel();
            mGetDataFromBufSICEvent.cancel();
            mNotifyGRITransCompEvent.cancel();
            mNotifySICTransCompEvent.cancel();
            mGRIContinueTransferringEvent.cancel();
            mSICContinueTransferringEvent.cancel();
            mGRITransCompEvent.cancel();
            mSICTransCompEvent.cancel();
            mTransLTConter[0] = 0;
            mTransLTConter[1] = 0;

            BufferData temp;
            while (!mBufferGRI.empty()) {
                temp = mBufferGRI.front();
                mBufferGRI.pop();
                if (mIsHRESETnCmdActive) {
                    mAXISTBDataHandlerGRI->mTrans = temp.trans;
                    mAXISTBDataHandlerGRI->mTransTime = temp.transtime;
                    mAXISTBDataHandlerGRI->CancelTransferring();
                    NotifyTransCancel("GRI");
                }
            }
            while (!mBufferSIC.empty()) {
                temp = mBufferSIC.front();
                mBufferSIC.pop();
                if (mIsHRESETnCmdActive) {
                    mAXISTBDataHandlerSIC->mTrans = temp.trans;
                    mAXISTBDataHandlerSIC->mTransTime = temp.transtime;
                    mAXISTBDataHandlerSIC->CancelTransferring();
                    NotifyTransCancel("SIC");
                }
            }
        }
    }
}//}}}

///Check frequency value and frequency unit of PCLK, HCLK clock
void AXISTB::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
void AXISTB::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void AXISTB::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Calculate synchronous time with PCLK
double AXISTB::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "PCLK") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//clock_name is HCLK
        period_num = time_point / (double)mHCLKPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//clock_name is HCLK
        pos_edge_point = (double)mod_period_num * (double)mHCLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Check period value of PCLK, HCLK clock
bool AXISTB::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "PCLK")?mPCLKPeriod:mHCLKPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

///Check reset status
bool AXISTB::GetHRESETZResetStatus (void)
{//{{{
    bool reset_status = false;
    if (mIsHRESETnHardActive || mIsHRESETnCmdActive) {
        reset_status = true;
    }
    return reset_status;
}//}}}

///Check access permission
bool AXISTB::PushDataToBuffer (const std::string name, const bool is_write, TlmBasicPayload& trans, const BusTime_t trans_time)
{//{{{
    BufferData temp;
    temp.is_write = is_write;
    temp.trans = &trans;
    temp.transtime = trans_time;
    bool ret = true;
    if (name == "GRI") {
        if (mBufferGRI.size() < mBufferSize[0]) {
            mBufferGRI.push(temp);
            if (mBufferGRI.size() == mBufferSize[0]) {
                re_printf("info","Buffer of GRI is full.\n");
            }
        } else {
            ret = false;
            re_printf("warning","Buffer of GRI is overflowed.\n");
            mAXISTBDataHandlerGRI->mTrans = temp.trans;
            mAXISTBDataHandlerGRI->mTransTime = temp.transtime;
            NotifyTransCancel("GRI");
        }
    } else {
        if (mBufferSIC.size() < mBufferSize[1]) {
            mBufferSIC.push(temp);
            if (mBufferSIC.size() == mBufferSize[1]) {
                re_printf("info","Buffer of SIC is full.\n");
            }
        } else {
            ret = false;
            re_printf("warning","Buffer of SIC is overflowed.\n");
            mAXISTBDataHandlerSIC->mTrans = temp.trans;
            mAXISTBDataHandlerSIC->mTransTime = temp.transtime;
            NotifyTransCancel("SIC");
        }
    }
    return ret;
}//}}}

///Continue to transfer after suspending the transaction
void AXISTB::ContinueTransferring(const std::string name)
{//{{{
    if (name == "GRI") {
        if (!mBufferGRI.empty()) {
            // notify an event to pop data from buffer and transferring
            mGRIContinueTransferringEvent.notify();
            mGetDataFromBufGRIEvent.notify();
        }
    } else {
        if (!mBufferSIC.empty()) {
            // notify an event to pop data from buffer and transferring
            mSICContinueTransferringEvent.notify();
            mGetDataFromBufSICEvent.notify();
        }
    }
}//}}}

///Notify to complete transfer in GRI
void AXISTB::NotifyGRITransferringCompleteMethod(void)
{//{{{
    mGRITransCompEvent.notify();
}//}}}

///Notify to complete transfer in SIC
void AXISTB::NotifySICTransferringCompleteMethod(void)
{//{{{
    mSICTransCompEvent.notify();
}//}}}

///Notify to cancel transfer in GRI
void AXISTB::NotifyGRITransferringCancelMethod(void)
{//{{{
    mAXISTBDataHandlerGRI->CancelTransferring();
}//}}}

///Notify to cancel transfer in SIC
void AXISTB::NotifySICTransferringCancelMethod(void)
{//{{{
    mAXISTBDataHandlerSIC->CancelTransferring();
}//}}}

///Transfer transaction in GRI
void AXISTB::SendDataFromBufGRIThread(void)
{//{{{
    for(;;){
        if (GetHRESETZResetStatus()) {
            wait();
            continue;
        }
        try {
            while (true) {
                while (!mBufferGRI.empty()) {
                    SendDataToDataHandler("GRI");
                    wait(mGRITransCompEvent | mGRIContinueTransferringEvent |mHRESETnEvent);
                    if (GetHRESETZResetStatus()) {
                        throw vpcl::emRstException;
                    }                
                }
                wait();
                if (GetHRESETZResetStatus()) {
                    throw vpcl::emRstException;
                }                
            }
        }
        catch (vpcl::eResetException){
        }    
        wait();
    }    
}//}}}

///Transfer transaction in SIC
void AXISTB::SendDataFromBufSICThread(void)
{//{{{
    for(;;){
        if (GetHRESETZResetStatus()) {
            wait();
            continue;
        }
        try {
            while (true) {
                while (!mBufferSIC.empty()) {
                    SendDataToDataHandler("SIC");
                    wait(mSICTransCompEvent | mSICContinueTransferringEvent | mHRESETnEvent);
                    if (GetHRESETZResetStatus()) {
                        throw vpcl::emRstException;
                    }                
                }
                wait();
                if (GetHRESETZResetStatus()) {
                    throw vpcl::emRstException;
                }                
            }
        }
        catch (vpcl::eResetException){
        }    
        wait();
    }    
}//}}}

///Transfer transaction for both GRI and SIC
void AXISTB::SendDataToDataHandler(const std::string name)
{//{{{
    if (mAXISTBFunc->CheckHaltStatus(name)) {
        BufferData temp;
        if (name == "GRI") {
            temp = mBufferGRI.front();
            mBufferGRI.pop();
        } else {
            temp = mBufferSIC.front();
            mBufferSIC.pop();
        }
        if (name == "GRI") {
            mAXISTBDataHandlerGRI->TransferData(temp.is_write, temp.trans, &(temp.transtime), false);
        } else {
            mAXISTBDataHandlerSIC->TransferData(temp.is_write, temp.trans, &(temp.transtime), false);
        }
    } else {
        std::string ini_name = "iss";
        if (name == "GRI") {
            ini_name = "isg";
        }
        re_printf("warning","The transfer via %s socket has been suspended already.\n", ini_name.c_str());
    }
}//}}}

///Notify to complete transaction for both GRI and SIC
void AXISTB::NotifyTransComp(const std::string name)
{//{{{
    if (name == "GRI") {
        mNotifyGRITransCompEvent.notify();
        if (!mEventQueueGRI.empty()) {
            (mEventQueueGRI.front())->notify();
        }
    } else {
        mNotifySICTransCompEvent.notify();
        if (!mEventQueueSIC.empty()) {
            (mEventQueueSIC.front())->notify();
        }
    }
}//}}}

///Notify to cancel transaction for both GRI and SIC
void AXISTB::NotifyTransCancel(const std::string name)
{//{{{
    if (name == "GRI") {
        mNotifyGRITransCancelEvent.notify();
    } else {
        mNotifySICTransCancelEvent.notify();
    }
}//}}}

void AXISTB::TransferLT(const std::string name, const bool is_write, TlmBasicPayload &trans, const BusTime_t trans_time)
{//{{{
    bool trans_allow = PushDataToBuffer(name, is_write, trans, trans_time);
    if (trans_allow) {
        ContinueTransferring(name);
        if (name == "GRI") {
            mEventQueueGRI.push(&mGRICompLTEvent[mTransLTConter[0]]);
            if (mTransLTConter[0] < (mBufferSize[0] - 1)) {
                mTransLTConter[0]++;
            } else {
                mTransLTConter[0] = 0;
            }
            wait(*(mEventQueueGRI.front()) | mHRESETnEvent);
            mEventQueueGRI.pop();
        } else {
            mEventQueueSIC.push(&mSICCompLTEvent[mTransLTConter[1]]);
            if (mTransLTConter[1] < (mBufferSize[1] - 1)) {
                mTransLTConter[1]++;
            } else {
                mTransLTConter[1] = 0;
            }
            wait(*(mEventQueueSIC.front()) | mHRESETnEvent);
            mEventQueueSIC.pop();
        }
    }
}//}}}
// vim600: set foldmethod=marker :
