// ---------------------------------------------------------------------
// $Id: APG.cpp,v 1.15 2014/03/03 06:33:04 dungvannguyen Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "APG.h"
#include "ApgDataHandler.h"

///Constructor of APG class
APG::APG(sc_module_name name,
         unsigned int rLatency,
         unsigned int wLatency,
         unsigned int APG_VERSION,
         bool P_LOCK_SUPPORT,
         unsigned int P_FILTER_NO):
         ApgAgentController(),
         BusBridgeModuleBase<32,32>(name),
         BusBridgeSlaveBase<32,1>(),
         BusMasterBase<32,1>(),
         BusSlaveBase<32,2>(),
         PCLK("PCLK"),
         INTAPB("INTAPB")
{//{{{
    INTAPB.initialize(false);
    //Handle config parameters
    mVersion = APG_VERSION;
    mLockSupport = P_LOCK_SUPPORT;
    mFilterNumber = 1;
    if ((P_FILTER_NO > 16) || (P_FILTER_NO == 0)) {
        char message[0xF] = {""};
        sprintf(message,"Invalid APG_P_FILTER_NO : %d", P_FILTER_NO);
        SC_REPORT_WARNING(this->basename(), message);
        SC_REPORT_WARNING(this->basename(),"Number of filter is from 1 to 16.");
    } else {
        mFilterNumber = P_FILTER_NO;
    }
   
    mIsConstruct = true;
    mClkPeriod = 0;
    mTimeUnitValue = 1;
    mTimeUnit = SC_NS;
    GetResolutionTime(mTimeUnitValue, mTimeUnit);
    mClkFreq = 0;
    mClkOrgFreq = 0;
    mClkUnit = "Hz";
    mCmdResetPeriod = 0;
    mIntrValue = false;
    mIsHardResetActive = false;
    mIsCmdResetActive = false;
    mIsAssertResetReceive = false;
    mRstPortValue = true;
    mDumpInterrupt = false;
 
    setMasterResetPort32(&resetPort );
    setMasterFreqPort32(&PCLK );
    setInitiatorSocket32("iss");
    setSlaveResetPort32(&resetPort,&resetPort);
    setSlaveResetPort(&resetPort);
    setSlaveFreqPort32(&PCLK,&PCLK);
    setSlaveFreqPort(&PCLK);
    setTargetSocket32("tsg","tse");
    setTargetSocket("tss");

    tss = BusBridgeSlaveBase<32,1>::tSocket[0]; //Alias for binding
    tsg = BusSlaveBase<32,2>::tSocket32[0]; //Alias for binding
    tse = BusSlaveBase<32,2>::tSocket32[1]; //Alias for binding
    iss = iSocket32[0]; //Alias for binding

    mAPBFSGuard = new APBFSGuard((std::string)name,
                                  this,
                                  mVersion,
                                  mLockSupport,
                                  mFilterNumber);
    sc_assert(mAPBFSGuard != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mAPBFSGuard);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    mAPBERRSLV_Org = (APBERRSLV *)0;
    mAPBERRSLV = new APBERRSLV((std::string)name, this, mVersion);
    sc_assert(mAPBERRSLV != NULL);
    mBusSlaveIf32[1]->setFuncModulePtr(mAPBERRSLV);
    mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[1]->setReadInitLatency(rLatency);
    mBusSlaveIf32[1]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[1]->setTransNmbLmt(0xFFFFFFFF);

    mApgDataHandler = new ApgDataHandler((std::string)name,this,mBusSlaveIf[0], mBusMasterIf32[0], this);
    mpCore = mApgDataHandler;
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mpCore);
    mBusMasterIf32[0]->setBusProtocol(BUS_APB);
    mBusMasterIf32[0]->setTransNmbLmt(1);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore);
    mBusSlaveIf[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf[0]->setTransNmbLmt(0xFFFFFFFF);

    SC_METHOD(HandlePCLKMethod);
    dont_initialize();
    sensitive<<PCLK;

    SC_METHOD(HandleResetPortMethod);
    dont_initialize();
    sensitive<<resetPort;

    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive<<mResetEvent;

    SC_METHOD(CmdResetMethod);
    dont_initialize();
    sensitive<<mCmdResetEvent;

    SC_METHOD(CancelCmdResetMethod);
    dont_initialize();
    sensitive<<mCancelCmdResetEvent;

    SC_METHOD(WriteINTAPBMethod);
    dont_initialize();
    sensitive<<mWriteIntrEvent;

    SC_METHOD(ClearINTAPBMethod);
    dont_initialize();
    sensitive<<mClearIntrEvent;

    SC_METHOD(HandleErrorInfoMethod);
    dont_initialize();
    sensitive<<mHandleErrorInfoEvent;

    SC_METHOD(NotifyTranferCompleteMethod);
    dont_initialize();
    sensitive<<mTransferCompleteEvent;
}//}}}

///Destructor of APG class
APG::~APG(void)
{//{{{
    delete mAPBFSGuard;
    delete mApgDataHandler;
    if (mAPBERRSLV_Org != NULL) {
        mAPBERRSLV = mAPBERRSLV_Org;
    }
    delete mAPBERRSLV;
}//}}}

///Set pointer of APBERRSLV class to APBERRSLV instance
void APG::SetERRSLVPointer(APBERRSLV *ptr)
{//{{{
    sc_assert(ptr!=NULL);
    mAPBERRSLV_Org = mAPBERRSLV;//Store mAPBERRSLV pointer
    mAPBERRSLV = ptr;
}//}}}

///Get pointer address of APBERRSLV instance
APBERRSLV* APG::GetERRSLVPointer(void)
{//{{{
    sc_assert(mAPBERRSLV);
    return mAPBERRSLV;
}//}}}}

///Trigger reset operation via Python IF
void APG::AssertReset(const double delay, const double period)
{//{{{
    if ((!mIsHardResetActive)&&(!mIsAssertResetReceive)) {
        mIsAssertResetReceive = true;
        re_printf("info","APG will reset for %f ns after %f ns.\n", period, delay);
        mCmdResetEvent.notify(delay, SC_NS);
        mCmdResetPeriod = period;
    }
}//}}}

///Set clock frequency via Python IF
void APG::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, clock_name);
    if (temp.size() == 1) {
        if (temp[0] == "PCLK") {
            if (unit == "GHz") {
                mClkOrgFreq = freq;
                mClkFreq = freq * (sc_dt::uint64)(1.0e+9);
                mClkUnit = "GHz";
            } else if (unit == "MHz") {
                mClkOrgFreq = freq;
                mClkFreq = freq * (sc_dt::uint64)(1.0e+6);
                mClkUnit = "MHz";
            } else if (unit == "KHz") {
                mClkOrgFreq = freq;
                mClkFreq = freq * (sc_dt::uint64)(1.0e+3);
                mClkUnit = "KHz";
            } else if (unit == "Hz") {
                mClkOrgFreq = freq;
                mClkFreq = freq;
                mClkUnit = "Hz";
            } else {// Frequency unit is wrong, frequency unit is set as Hz default.
                mClkOrgFreq = freq;
                mClkFreq = freq;
                mClkUnit = "Hz";
                re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit.c_str());
            }
            if (mClkFreq > 0) {
                mClkPeriod = (sc_dt::uint64)((1/(double)mClkFreq)*(double)mTimeUnitValue + 0.5);
            } else {
                mClkPeriod = 0;
                mHandleErrorInfoEvent.cancel();
                mWriteIntrEvent.cancel();
                mClearIntrEvent.cancel();
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", temp[0].c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", temp[0].c_str());
    }
}//}}}

///Get clock frequency via Python IF
sc_dt::uint64 APG::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, clock_name);
    if (temp.size() == 1) {
        if (temp[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mClkOrgFreq, mClkUnit.c_str());
            return mClkOrgFreq;
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", temp[0].c_str());
            return 0;
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", temp[0].c_str());
        return 0;
    }
}//}}}

///Set the information (id, address area) for filters via Python IF
void APG::SetFilterInfo(const std::string is_set,
                        const unsigned int id,
                        const unsigned int start_addr,
                        const long long size,
                        const std::string property)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_set);
    if (temp.size() == 1) {
        if (temp[0] == "setvalue") {
            if ((property == "R") || (property == "W") || (property == "RW")) {
                mAPBFSGuard->SetFilterInfo(true, id, start_addr, size, property);
            } else {
                re_printf("error","Invalid argument: %s\n", property.c_str());
            }
        } else if (temp[0] == "getvalue") {
            mAPBFSGuard->SetFilterInfo(false, id, 0, 0, "R");
        } else {
            re_printf("error","Invalid argument: %s\n", temp[0].c_str());
        }
    } else {
        re_printf("error","Invalid argument: %s\n", is_set.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info) via Python IF
void APG::SetMessageLevel(const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string ret = handleCommand(cmd);
    if (ret != "") {
        SC_REPORT_INFO(this->basename(),ret.c_str());
    }

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAPBFSGuard->RegisterHandler(cmd);
    mAPBERRSLV->RegisterHandler(cmd);

    for(unsigned int index = 0; index < mFilterNumber; index++){
        std::string arg = "";
        char RegName[64];
        cmd.clear();                      
        sprintf(RegName,"PROT%d", (char)index );
        arg = (std::string)RegName;             
        cmd.push_back("reg");               
        cmd.push_back(arg);               
        cmd.push_back("MessageLevel");               
        SeparateString(cmd, msg_lv);
        mAPBFSGuard->RegisterHandler(cmd);
        cmd.clear();                      
        if (mVersion == emCC_CUBE) {
            sprintf(RegName,"SPID%d", (char)index );
            arg = (std::string)RegName;             
            cmd.push_back("reg");               
            cmd.push_back(arg);               
            cmd.push_back("MessageLevel");               
            SeparateString(cmd, msg_lv);
            mAPBFSGuard->RegisterHandler(cmd);
            cmd.clear();
        }
    }
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("CTL");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAPBERRSLV->RegisterHandler(cmd);

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("STAT");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAPBERRSLV->RegisterHandler(cmd);

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("ADDR");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAPBERRSLV->RegisterHandler(cmd);

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("TYPE");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAPBERRSLV->RegisterHandler(cmd);
}//}}}

///Set whether interrupt information is dumpped via Python IF
void APG::DumpInterrupt(const std::string is_enable)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_enable);
    if (temp.size() == 1) {
        if (temp[0] == "true") {
            mDumpInterrupt = true;
        } else if (temp[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("error","Invalid argument: %s\n", is_enable.c_str());
        }
    } else if (temp.size() == 0){
        std::string temp_arg = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","DumpInterrupt     %s\n",temp_arg.c_str());
    } else {
        re_printf("error","Invalid argument: DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

///Dump the information about all registers via Python IF
void APG::DumpStatusInfo(const std::string block_name)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, block_name);
    if (temp.size() == 1) {
        if (temp[0] == "apbfsguard") {
            mAPBFSGuard->DumpStatusInfo();
        } else if (temp[0] == "apberrslv") {
            mAPBERRSLV->DumpStatusInfo();
        } else {
            re_printf("error","Invalid argument: %s\n", temp[0].c_str());
        }
    } else {
        re_printf("error","Invalid argument: %s\n", block_name.c_str());
    }
}//}}}

///Set whether register access information is dumpped via Python IF
void APG::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::string msg = "";
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    msg = mAPBFSGuard->RegisterHandler(cmd);
    mAPBERRSLV->RegisterHandler(cmd);
    if (msg != "") {
        SC_REPORT_INFO(this->basename(),msg.c_str());
    }
}//}}}

///Set whether file name and line number are dumpped via Python IF
void APG::DumpFileNameLineNum(const std::string is_enable)
{//{{{
    std::string msg = "";
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpFileNameLineNum");
    SeparateString(cmd, is_enable);
    msg = mAPBFSGuard->RegisterHandler(cmd);
    mAPBERRSLV->RegisterHandler(cmd);
    if (msg != "") {
        SC_REPORT_INFO(this->basename(),msg.c_str());
    }
}//}}}

///Force a value to register via Python IF
void APG::ForceRegister(const std::string reg_name, const std::string reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    SeparateString(cmd, reg_value);
    if ((reg_name == "CTL") || (reg_name == "STAT") || (reg_name == "ADDR") || (reg_name == "TYPE")) {
        mAPBERRSLV->RegisterHandler(cmd);
    } else {
        mAPBFSGuard->RegisterHandler(cmd);
    }
}//}}}

///Release forced value of registers via Python IF
void APG::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    if ((reg_name == "CTL") || (reg_name == "STAT") || (reg_name == "ADDR") || (reg_name == "TYPE")) {
        mAPBERRSLV->RegisterHandler(cmd);
    } else {
        mAPBFSGuard->RegisterHandler(cmd);
    }
}//}}}

///Read/Write the value from/to registers via Python IF
void APG::AccessRegister(const std::string reg_name, const std::string reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    SeparateString(cmd, reg_value);
    if ((reg_name == "CTL") || (reg_name == "STAT") || (reg_name == "ADDR") || (reg_name == "TYPE")) {
        mAPBERRSLV->RegisterHandler(cmd);
    } else {
        mAPBFSGuard->RegisterHandler(cmd);
    }
}//}}}

///List all registers via Python IF
void APG::ListRegister(const std::string block_name)
{//{{{
    std::vector<std::string> cmd;
    SeparateString(cmd, block_name);
    if (cmd.size() == 1) {
        std::vector<std::string> temp;
        temp.push_back("reg");
        if (cmd[0] == "apbfsguard") {
            mAPBFSGuard->RegisterHandler(temp);
        } else if (cmd[0] == "apberrslv") {
            mAPBERRSLV->RegisterHandler(temp);
        } else {
            re_printf("error","Invalid argument: %s\n", cmd[0].c_str());
        }
    } else {
        re_printf("error","Block name is empty. Block name must be apbfsguard or apberrlsv.\n");
    }
}//}}}

///Set inital value to APBFSGuard registers
void APG::SetRegInitVal(const unsigned int filter_id, const std::string reg_name, const unsigned int value)
{//{{{
    unsigned int reg_value = value;
    bool is_valid = true;
    if (filter_id >= mFilterNumber) {
        re_printf("warning","Filter id %d is invalid. Filter id should be from 0 to %d.\n", filter_id, mFilterNumber-1);
    } else {
        if (reg_name == "P_RESET_VALUES") {
            if ((reg_value >> 31) == 1) {
                SC_REPORT_WARNING(this->basename(),"Reset value of LOCK bit should be set to 0.");
            }
            if (mVersion == emCC_CUBE) {
                if((((reg_value >> 27) & 0xF) > 0x0)||(((reg_value >> 5) & 0xF) > 0x0)){//Check reserve bits
                    SC_REPORT_WARNING(this->basename(),"Reserve bits of PROT register cannot be written to 1.");
                }
            } else {
                if(((reg_value >> 27) & 0xF) > 0x0){//Check reserve bits
                    SC_REPORT_WARNING(this->basename(),"Reserve bits of PROT register cannot be written to 1.");
                }
            }
            if (mVersion == emCC_CUBE) {
                reg_value = reg_value & 0x07FFFE1F;
            } else {
                reg_value = reg_value & 0x07FFFFFF;
            }
        } else if (reg_name == "P_SPID_RESET_VALUES") {
            if (mVersion != emCC_CUBE) {
                is_valid = false;
                re_printf("error","Invalid argument: %s\n", reg_name.c_str());
            }
        } else {
            is_valid = false;
            re_printf("error","Invalid argument: %s\n", reg_name.c_str());
        }
        if (is_valid) {
            mAPBFSGuard->SetRegInitValue(filter_id, reg_name, reg_value);
        }
    }
}//}}}

///Dump parameters and Commands in AHG model
void APG::Help(const std::string type)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, type);
    if (temp.size() == 1) {
        if (temp[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"----Parameters----");
            SC_REPORT_INFO(this->basename(),"APG_MessageLevel (fatal|error|warning|info)            Select debug message level (Default: fatal,error)");
            SC_REPORT_INFO(this->basename(),"APG_DumpInterrupt (true/false)                         Displays interrupt information when interrupt get/set.");
            SC_REPORT_INFO(this->basename(),"APG_DumpFileNameLineNum (true/false)                   Select dump information about file name and line number.");
        } else if (temp[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"----Commands----");
            SC_REPORT_INFO(this->basename(),"APG_DumpStatusInfo (block_name)                        Display the status information of apbfsguard or apberrslv block.");
            SC_REPORT_INFO(this->basename(),"APG_AssertReset (start_time, period)                   Assert and de-assert reset signal to the APG model. ");
            SC_REPORT_INFO(this->basename(),"APG_SetFilterInfo (filter_id, start_addr, size)        Set information to each apbfsguard filter.");
            SC_REPORT_INFO(this->basename(),"APG_SetCLKFreq (clock_name, freq, unit)                Set frequency value to APG. Valid clock_name is PCLK.");
            SC_REPORT_INFO(this->basename(),"APG_GetCLKFreq (clock_name)                            Get frequency value of APG. Valid clock_name is PCLK.");
            SC_REPORT_INFO(this->basename(),"APG_ForceRegister (reg_name, value)                    Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"APG_ReleaseRegister (reg_name)                         Release register from force value.");
            SC_REPORT_INFO(this->basename(),"APG_AccessRegister (reg_name, value)                   Write or read value of register. In case value arg is empty, AccessRegister reads the value of register");
            SC_REPORT_INFO(this->basename(),"APG_ListRegister (block_name)                          Dump name of register of each block based on block_name is apbfsguard or apberrslv.");
            SC_REPORT_INFO(this->basename(),"APG_SetRegInitVal (filter_id, prot_val, spid_val)      Set reset values for specified register.");
        } else {
            re_printf("error","Invalid argument: Help %s. The valid argument is commands or parameters.\n", type.c_str());
        }
    } else {
        re_printf("error","Invalid argument: Help %s. The valid argument is commands or parameters.\n", type.c_str());
    }
}//}}}

///Handle PCLK frequency of PCLK port
void APG::HandlePCLKMethod(void)
{//{{{
    sc_dt::uint64 temp_freq = PCLK.read();
    SetCLKFreq("PCLK", temp_freq, "Hz");
}//}}}

///Handle the value of PRESETZ port
void APG::HandleResetPortMethod(void)
{//{{{
    if (mIsConstruct) {
        mIsConstruct = false;
    } else {
        mRstPortValue = resetPort.read();
        if (mClkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetEvent.notify(PosEdgeCalc(true, current_time), mTimeUnit);
        } else {
            mResetEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Reset operation is active
void APG::ResetMethod(void)
{//{{{
    if (mRstPortValue == emResetActive) {
        mIsHardResetActive = true;
        re_printf("info","Reset signal is asserted.\n");
        this->EnableRst(mIsHardResetActive);
    } else if (mIsHardResetActive) {
        mIsHardResetActive = false;
        re_printf("info","Reset signal is de-asserted.\n");
        this->EnableRst(mIsHardResetActive);
    }
    // Cancel Command AssertReset when rst_n is asserted
    if (mIsAssertResetReceive) {
        mCmdResetEvent.cancel();
        mCancelCmdResetEvent.cancel();
        mIsAssertResetReceive = false;
        mIsCmdResetActive = false;
    }
}//}}}

///Handle reset command
void APG::CmdResetMethod(void)
{//{{{
    mIsCmdResetActive = true;
    re_printf("info","APG model is reset by AssertReset command.\n");
    this->EnableRst(mIsCmdResetActive);
    mCancelCmdResetEvent.notify(mCmdResetPeriod, SC_NS);
}//}}}

///Cancel reset command when reset period is over
void APG::CancelCmdResetMethod(void)
{//{{{
    mIsCmdResetActive = false;
    mIsAssertResetReceive = false;
    re_printf("info","Reset period is over.\n");
    this->EnableRst(mIsCmdResetActive);    
}//}}}

///Write the value to INTAPB port
void APG::WriteINTAPBMethod(void)
{//{{{
    INTAPB.write(mIntrValue);
    if (mDumpInterrupt) {
        if (mIntrValue) {
            re_printf("info","INT [INTAPB] Assert.\n");
        } else {
            if ((!mIsHardResetActive) && (!mIsCmdResetActive)) {
                re_printf("info","INT [INTAPB] De-assert.\n");
            }
        }
    }
}//}}}

///Triger a event to deactivate INTAPB port
void APG::ClearINTAPBMethod(void)
{//{{{
    mIntrValue = false;
    mWriteIntrEvent.notify();
}//}}}

///Handle error information
void APG::HandleErrorInfoMethod(void)
{//{{{
    mAPBERRSLV->HandleErrInfo();
    ControlIntr();
}//}}}

///Call function of ApgDataHandler to inform that transferring data is completed
void APG::NotifyTranferCompleteMethod(void)
{//{{{
    mApgDataHandler->TransferComplete();
}//}}}

///Calculate synchronous time of a event with clock posedge
double APG::PosEdgeCalc(const bool is_reset, const double time_point)
{//{{{
    //calcualte the time point of next pos edge of PCLK
    double period_num = time_point / (double)mClkPeriod;
    unsigned int mod_period_num = (unsigned int)period_num;
    if (is_reset) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = (double)mod_period_num * (double)mClkPeriod - time_point;
    return pos_edge_point;
}//}}}

///Reset APG model
void APG::EnableRst(const bool is_active)
 {//{{{ 
    mAPBFSGuard->Reset(is_active);
    mAPBERRSLV->Reset(is_active);
    if (is_active) {
        mHandleErrorInfoEvent.cancel();
        mWriteIntrEvent.cancel();
        mClearIntrEvent.cancel();
        mIntrValue = false;
        mWriteIntrEvent.notify();
    }
}//}}}

///Separate a string to a vector
void APG::SeparateString(std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; // this operation makes temp to be not empty
    while (temp != "") {
        std::string temp1;
        cmd >> temp1;
        if (temp1 != "") {
            vtr.push_back(temp1);
        }
        temp = temp1;
    }
}//}}}

///Get time resolution
void APG::GetResolutionTime(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
  if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
      resolution_value = 1000000000000000LL;
      resolution_unit = SC_FS;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
      resolution_value = 1000000000000LL;
      resolution_unit = SC_PS;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
      resolution_value = 1000000000;
      resolution_unit = SC_NS;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
      resolution_value = 1000000;
      resolution_unit = SC_US;
  }
  else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
      resolution_value = 1000;
      resolution_unit = SC_MS;
  }
  else {
      resolution_value = 1;
      resolution_unit = SC_SEC;
  }
}//}}}

///Control interrupt port INTAPB
void APG::ControlIntr(void)
{//{{{
    mIntrValue = true;
    mWriteIntrEvent.notify();
    mClearIntrEvent.notify(mClkPeriod, mTimeUnit);
}//}}}

///Wait synchronous time with clock posedge
void APG::WaitPosEdgeClk(void)
{//{{{
    if (CheckPCLKClock()) {
        double current_time = sc_time_stamp().to_double();
        mHandleErrorInfoEvent.notify(PosEdgeCalc(false, current_time), mTimeUnit);
    }
}//}}}

///Check clock period
bool APG::CheckPCLKClock(void)
{//{{{
    if (mClkPeriod > 0) {
        return true;
    } else {
        re_printf("info","PCLK frequency is zero.\n");
        return false;
    }
}//}}}

///Check access ability to peripherals
bool APG::CheckAccessPermission(TlmBasicPayload &trans, const bool debug, bool &is_valid_trans)
{//{{{
    return mAPBFSGuard->CheckAccessibility(trans, debug, is_valid_trans);
}//}}}

///Inform error information to APBERRSLV class
void APG::TransferErrInfo(TlmBasicPayload &trans)
{//{{{
    mAPBERRSLV->InformError(trans);
    WaitPosEdgeClk();
}//}}}

///Get reset status
bool APG::GetResetStatus(void)
{//{{{
    bool status = false;
    if (mIsHardResetActive || mIsCmdResetActive) {
        status = true;
    } else {
        status = false;
    }
    return status;
}//}}}

///Notify event to inform that transferring data is completed
void APG::NotifyTransComp(void)
{//{{{
    mTransferCompleteEvent.notify();
}//}}}
// vim600: set foldmethod=marker :
