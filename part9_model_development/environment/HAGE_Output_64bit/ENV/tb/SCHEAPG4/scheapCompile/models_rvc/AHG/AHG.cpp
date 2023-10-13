// ---------------------------------------------------------------------
// $Id: AHG.cpp,v 1.7 2014/03/03 06:28:41 dungvannguyen Exp $
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

#include "AHG.h"
#include "AhgDataHandler.h"

///Constructor of AHG class
AHG::AHG(sc_module_name name,
         unsigned int rLatency,
         unsigned int wLatency,
         unsigned int AHG_VERSION,
         bool P_LOCK_SUPPORT,
         unsigned int P_FILTER_NO):
         AhgAgentController(),
         BusBridgeModuleBase<32,32>(name),
         BusBridgeSlaveBase<32,1>(),
         BusMasterBase<32,1>(),
         BusSlaveBase<32,2>(),
         PCLK("PCLK"),
         HCLK("HCLK"),
         HRESETZ("HRESETZ"),
         INTAHB("INTAHB")
{//{{{
    INTAHB.initialize(false);
    //Handle config parameters
    mVersion = AHG_VERSION;
    mLockSupport = P_LOCK_SUPPORT;
    mFilterNumber = 1;
    if ((P_FILTER_NO > 16) || (P_FILTER_NO == 0)) {
        char message[0xF] = {""};
        sprintf(message,"Invalid AHG_P_FILTER_NO : %d", P_FILTER_NO);
        SC_REPORT_WARNING(this->basename(),message);
        SC_REPORT_WARNING(this->basename(),"Number of filter is from 1 to 16.");
    } else {
        mFilterNumber = P_FILTER_NO;
    }

    mIsConstructPRESETZ = true;
    mIsConstructHRESETZ = true;
    mPCLKPeriod = 0;
    mHCLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mHCLKFreq = 0;
    mHCLKOrgFreq = 0;
    mTimeUnitValue = 1;
    mTimeUnit = SC_NS;
    GetResolutionTime(mTimeUnitValue, mTimeUnit);
    mPCLKFreqUnit = "Hz";
    mHCLKFreqUnit = "Hz";
    mIntrValue = false;
    mPRESETZValue = true;
    mHRESETZValue = true;
    mIsHRESETZHardActive = false;
    mIsPRESETZHardActive = false;
    mIsHRESETZCmdActive = false;
    mIsPRESETZCmdActive = false;
    mIsHRESETZCmdReceive = false;
    mIsPRESETZCmdReceive = false;
    mCmdHRESETZResetPeriod = 0;
    mCmdPRESETZResetPeriod = 0;
    mDumpInterrupt = false;
    mErrorAccessIgnore = false;
    mStatusClearIgnore = false;
    mClearOnlyOVF = true;
 
    setMasterResetPort32(&HRESETZ);
    setMasterFreqPort32(&HCLK );
    setInitiatorSocket32("iss");
    setSlaveResetPort32(&resetPort,&resetPort);
    setSlaveResetPort(&HRESETZ);
    setSlaveFreqPort32(&PCLK,&PCLK);
    setSlaveFreqPort(&HCLK);
    setTargetSocket32("tsg","tse");
    setTargetSocket("tss");

    tss = BusBridgeSlaveBase<32,1>::tSocket[0]; //Alias for binding
    tsg = BusSlaveBase<32,2>::tSocket32[0]; //Alias for binding
    tse = BusSlaveBase<32,2>::tSocket32[1]; //Alias for binding
    iss = iSocket32[0]; //Alias for binding

    mAHBFSGuard = new AHBFSGuard((std::string)name,
                                  this,
                                  mVersion,
                                  mLockSupport,
                                  mFilterNumber);
    sc_assert(mAHBFSGuard != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mAHBFSGuard);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    mAHBERRSLV_Org = (AHBERRSLV *)0;
    mAHBERRSLV = new AHBERRSLV((std::string)name, this, mVersion);
    sc_assert(mAHBERRSLV != NULL);
    mBusSlaveIf32[1]->setFuncModulePtr(mAHBERRSLV);
    mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[1]->setReadInitLatency(rLatency);
    mBusSlaveIf32[1]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[1]->setTransNmbLmt(0xFFFFFFFF);

    mAhgDataHandler = new AhgDataHandler((std::string)name, this, mBusSlaveIf[0], mBusMasterIf32[0], this);
    mpCore = mAhgDataHandler;
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mpCore);
    mBusMasterIf32[0]->setBusProtocol(BUS_AHB);
    mBusMasterIf32[0]->setTransNmbLmt(1);
    mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *)mpCore);
    mBusSlaveIf[0]->setBusProtocol(BUS_AHB);
    mBusSlaveIf[0]->setTransNmbLmt(0xFFFFFFFF);

    SC_METHOD(HandlePCLKMethod);
    dont_initialize();
    sensitive<<PCLK;

    SC_METHOD(HandleHCLKMethod);
    dont_initialize();
    sensitive<<HCLK;

    SC_METHOD(ReceivePRESETZHardMethod);
    dont_initialize();
    sensitive<<resetPort;//PRESETZ is modelled by resetPort in base class

    SC_METHOD(ReceiveHRESETZHardMethod);
    dont_initialize();
    sensitive<<HRESETZ;

    SC_METHOD(HandlePRESETZHardMethod);
    dont_initialize();
    sensitive<<mPRESETZHardEvent;

    SC_METHOD(HandleHRESETZHardMethod);
    dont_initialize();
    sensitive<<mHRESETZHardEvent;

    SC_METHOD(HandlePRESETZCmdMethod);
    dont_initialize();
    sensitive<<mPRESETZCmdEvent;

    SC_METHOD(HandleHRESETZCmdMethod);
    dont_initialize();
    sensitive<<mHRESETZCmdEvent;

    SC_METHOD(CancelPRESETZCmdMethod);
    dont_initialize();
    sensitive<<mCancelPRESETZCmdEvent;

    SC_METHOD(CancelHRESETZCmdMethod);
    dont_initialize();
    sensitive<<mCancelHRESETZCmdEvent;

    SC_METHOD(WriteINTAHBMethod);
    dont_initialize();
    sensitive<<mWriteIntrEvent;

    SC_METHOD(ClearINTAHBMethod);
    dont_initialize();
    sensitive<<mClearIntrEvent;

    SC_METHOD(HandleErrorInfoMethod);
    dont_initialize();
    sensitive<<mHandleErrorInfoEvent;

    SC_METHOD(ErrorAccessIgnoreMethod);
    dont_initialize();
    sensitive<<mErrorAccessIgnoreEvent;

    SC_METHOD(StatusClearMethod);
    dont_initialize();
    sensitive<<mStatusClearEvent;

    SC_METHOD(NotifyTranferCompleteMethod);
    dont_initialize();
    sensitive<<mTransferCompleteEvent;
}//}}}

///Destructor of AHG class
AHG::~AHG(void)
{//{{{
    delete mAHBFSGuard;
    delete mAhgDataHandler; 
    if (mAHBERRSLV_Org != NULL) {
        mAHBERRSLV = mAHBERRSLV_Org;
    }
    delete mAHBERRSLV;
}//}}}

///Set pointer of AHBERRSLV class to AHBERRSLV instance
void AHG::SetERRSLVPointer(AHBERRSLV *ptr)
{//{{{
    sc_assert(ptr!=NULL);
    mAHBERRSLV_Org = mAHBERRSLV;//Store mAHBERRSLV pointer
    mAHBERRSLV = ptr;
}//}}}

///Get pointer address of AHBERRSLV instance
AHBERRSLV* AHG::GetERRSLVPointer(void)
{//{{{
    sc_assert(mAHBERRSLV);
    return mAHBERRSLV;
}//}}}}

///Trigger reset operation via Python IF
void AHG::AssertReset(const std::string reset_name, const double delay, const double period)
{//{{{
    if(reset_name == "HRESETZ") {
        if ((!mIsHRESETZHardActive)&&(!mIsHRESETZCmdReceive)) {
            mIsHRESETZCmdReceive = true;
            re_printf("info","AHG operation will reset for %f ns after %f ns.\n", period, delay);
            mHRESETZCmdEvent.notify(delay, SC_NS);
            mCmdHRESETZResetPeriod = period;
        }
    } else if (reset_name == "PRESETZ") {
        if ((!mIsPRESETZHardActive)&&(!mIsPRESETZCmdReceive)) {
            mIsPRESETZCmdReceive = true;
            re_printf("info","AHG registers will reset for %f ns after %f ns.\n", period, delay);
            mPRESETZCmdEvent.notify(delay, SC_NS);
            mCmdPRESETZResetPeriod = period;
        }
    } else {
        re_printf("error","Reset port name (%s) is wrong", reset_name.c_str());
    }
}//}}}

///Set clock frequency via Python IF
void AHG::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, clock_name);
    if (temp.size() == 1) {
        if (temp[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            CheckFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeUnitValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
                mErrorAccessIgnoreEvent.cancel();
                mStatusClearEvent.cancel();
                mHandleErrorInfoEvent.cancel();
                mWriteIntrEvent.cancel();
                mClearIntrEvent.cancel();
            }
        } else if (temp[0] == "HCLK") {
            mHCLKOrgFreq = freq;
            CheckFreq(mHCLKFreq, mHCLKFreqUnit, freq, unit);
            if (mHCLKFreq > 0) {
                mHCLKPeriod = (sc_dt::uint64)(((1/(double)mHCLKFreq)*(double)mTimeUnitValue) + 0.5);
            } else {
                mHCLKPeriod = 0;
                mErrorAccessIgnoreEvent.cancel();
                mStatusClearEvent.cancel();
                mHandleErrorInfoEvent.cancel();
                mWriteIntrEvent.cancel();
                mClearIntrEvent.cancel();
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock frequency via Python IF
sc_dt::uint64 AHG::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, clock_name);
    if (temp.size() == 1) {
        if (temp[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
            return mPCLKOrgFreq;
        } else if (temp[0] == "HCLK") {
            re_printf("info","HCLK frequency is %0.0f %s.\n", (double)mHCLKOrgFreq, mHCLKFreqUnit.c_str());
            return mHCLKOrgFreq;
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
            return 0;
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
        return 0;
    }
}//}}}

///Set the information (id, address area) for filters via Python IF
void AHG::SetFilterInfo(const std::string is_set,
                        const unsigned int id,
                        const unsigned int start_addr,
                        const long long size,
                        const std::string property)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, is_set);
    if (temp.size() == 1) {
        if ((property == "R") || (property == "W") || (property == "RW")) {
            if (temp[0] == "setvalue") {
                mAHBFSGuard->SetFilterInfo(true, id, start_addr, size, property);
            } else if (temp[0] == "getvalue") {
                mAHBFSGuard->SetFilterInfo(false, id, 0, 0, property);
            } else {
                re_printf("error","Invalid argument: %s\n", temp[0].c_str());
            }
        }
    } else {
        re_printf("error","Invalid argument: %s\n", is_set.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info) via Python IF
void AHG::SetMessageLevel(const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    std::string ret = handleCommand(cmd);
    if (ret != "") {
        SC_REPORT_WARNING(this->basename(),ret.c_str());
    }

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAHBFSGuard->RegisterHandler(cmd);
    mAHBERRSLV->RegisterHandler(cmd);

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
        mAHBFSGuard->RegisterHandler(cmd);
        cmd.clear();                      
        if (mVersion == emCC_CUBE) {
            sprintf(RegName,"SPID%d", (char)index );
            arg = (std::string)RegName;             
            cmd.push_back("reg");               
            cmd.push_back(arg);               
            cmd.push_back("MessageLevel");               
            SeparateString(cmd, msg_lv);
            mAHBFSGuard->RegisterHandler(cmd);
            cmd.clear();
        }
    }
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("CTL");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAHBERRSLV->RegisterHandler(cmd);

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("STAT");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAHBERRSLV->RegisterHandler(cmd);

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("ADDR");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAHBERRSLV->RegisterHandler(cmd);

    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("TYPE");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mAHBERRSLV->RegisterHandler(cmd);
}//}}}

///Set whether interrupt information is dumpped via Python IF
void AHG::DumpInterrupt(const std::string is_enable)
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
void AHG::DumpStatusInfo(const std::string block_name)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, block_name);
    if (temp.size() == 1) {
        if (temp[0] == "ahbfsguard") {
            mAHBFSGuard->DumpStatusInfo();
        } else if (temp[0] == "ahberrslv") {
            mAHBERRSLV->DumpStatusInfo();
        } else {
            re_printf("error","Invalid argument: %s\n", temp[0].c_str());
        }
    } else {
        re_printf("error","Invalid argument: %s\n", block_name.c_str());
    }
}//}}}

///Set whether register access information is dumpped via Python IF
void AHG::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::string msg = "";
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    msg = mAHBFSGuard->RegisterHandler(cmd);
    mAHBERRSLV->RegisterHandler(cmd);
    if (msg != "") {
        SC_REPORT_WARNING(this->basename(), msg.c_str());
    }
}//}}}

///Set whether file name and line number are dumpped via Python IF
void AHG::DumpFileNameLineNum(const std::string is_enable)
{//{{{
    std::string msg = "";
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpFileNameLineNum");
    SeparateString(cmd, is_enable);
    msg = mAHBFSGuard->RegisterHandler(cmd);
    mAHBERRSLV->RegisterHandler(cmd);
    if (msg != "") {
        SC_REPORT_WARNING(this->basename(), msg.c_str());
    }
}//}}}

///Force a value to register via Python IF
void AHG::ForceRegister(const std::string reg_name, const std::string reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    SeparateString(cmd, reg_value);
    if ((reg_name == "CTL") || (reg_name == "STAT") || (reg_name == "ADDR") || (reg_name == "TYPE")) {
        mAHBERRSLV->RegisterHandler(cmd);
    } else {
        mAHBFSGuard->RegisterHandler(cmd);
    }
}//}}}

///Release forced value of registers via Python IF
void AHG::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    if ((reg_name == "CTL") || (reg_name == "STAT") || (reg_name == "ADDR") || (reg_name == "TYPE")) {
        mAHBERRSLV->RegisterHandler(cmd);
    } else {
        mAHBFSGuard->RegisterHandler(cmd);
    }
}//}}}

///Read/Write the value from/to registers via Python IF
void AHG::AccessRegister(const std::string reg_name, const std::string reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    SeparateString(cmd, reg_value);
    if ((reg_name == "CTL") || (reg_name == "STAT") || (reg_name == "ADDR") || (reg_name == "TYPE")) {
        mAHBERRSLV->RegisterHandler(cmd);
    } else {
        mAHBFSGuard->RegisterHandler(cmd);
    }
}//}}}

///List all registers via Python IF
void AHG::ListRegister(const std::string block_name)
{//{{{
    std::vector<std::string> cmd;
    SeparateString(cmd, block_name);
    if (cmd.size() == 1) {
        std::vector<std::string> temp;
        temp.push_back("reg");
        if (cmd[0] == "ahbfsguard") {
            mAHBFSGuard->RegisterHandler(temp);
        } else if (cmd[0] == "ahberrslv") {
            mAHBERRSLV->RegisterHandler(temp);
        } else {
            re_printf("error","Invalid argument: %s\n", cmd[0].c_str());
        }
    } else {
        re_printf("error","Block name is empty. Block name must be apbfsguard or apberrlsv.\n");
    }
}//}}}

///Set initial value of PROT and SPID register from Python IF
void AHG::SetRegInitVal(const unsigned int filter_id, const std::string reg_name, const unsigned int value)
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
            mAHBFSGuard->SetRegInitValue(filter_id, reg_name, reg_value);
        }
    }
}//}}}

///Dump parameters and Commands in AHG model
void AHG::Help(const std::string type)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, type);
    if (temp.size() == 1) {
        if (temp[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"----Parameters----");
            SC_REPORT_INFO(this->basename(),"AHG_MessageLevel (fatal|error|warning|info)            Select debug message level (Default: fatal,error)");
            SC_REPORT_INFO(this->basename(),"AHG_DumpInterrupt (true/false)                         Displays interrupt information when interrupt get/set.");
            SC_REPORT_INFO(this->basename(),"AHG_DumpFileNameLineNum (true/false)                   Select dump information about file name and line number.");
        } else if (temp[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"----Commands----");
            SC_REPORT_INFO(this->basename(),"AHG_DumpStatusInfo (block_name)                        Display the status information of apbfsguard or apberrslv block.");
            SC_REPORT_INFO(this->basename(),"AHG_AssertReset (start_time, period)                   Assert and de-assert reset signal to the AHG model.");
            SC_REPORT_INFO(this->basename(),"AHG_SetFilterInfo (filter_id, start_addr, size)        Set information to each apbfsguard filter.");
            SC_REPORT_INFO(this->basename(),"AHG_SetCLKFreq (clock_name, freq, unit)                Set frequency value to AHG. Valid clock_name is PCLK or HCLK.");
            SC_REPORT_INFO(this->basename(),"AHG_GetCLKFreq (clock_name)                            Get frequency value of AHG. Valid clock_name is PCLK or HCLK.");
            SC_REPORT_INFO(this->basename(),"AHG_ForceRegister (reg_name, value)                    Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"AHG_ReleaseRegister (reg_name)                         Release register from force value.");
            SC_REPORT_INFO(this->basename(),"AHG_AccessRegister (reg_name, value)                   Write or read value of register. In case value arg is empty, AccessRegister reads the value of register.");
            SC_REPORT_INFO(this->basename(),"AHG_ListRegister (block_name)                          Dump name of register of each block based on block_name is apbfsguard or apberrslv.");
            SC_REPORT_INFO(this->basename(),"AHG_SetRegInitVal (filter_id, prot_val, spid_val)      Set reset values for specified register.");
        } else {
            re_printf("error","Invalid argument: Help %s. The valid argument is commands or parameters.\n", type.c_str());
        }
    } else {
        re_printf("error","Invalid argument: Help %s. The valid argument is commands or parameters.\n", type.c_str());
    }
}//}}}

///Handle PCLK frequency of PCLK port
void AHG::HandlePCLKMethod(void)
{//{{{
    sc_dt::uint64 temp_freq = PCLK.read();
    SetCLKFreq("PCLK", temp_freq, "Hz");
}//}}}

///Handle HCLK frequency of HCLK port
void AHG::HandleHCLKMethod(void)
{//{{{
    sc_dt::uint64 temp_freq = HCLK.read();
    SetCLKFreq("HCLK", temp_freq, "Hz");
}//}}}

///Receive the value of PRESETZ port
void AHG::ReceivePRESETZHardMethod(void)
{//{{{
    if (mIsConstructPRESETZ) {
        mIsConstructPRESETZ = false;
    } else {
        mPRESETZValue = resetPort.read();//PRESETZ is modelled by resetPort in base class
        if (mPCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mPRESETZHardEvent.notify(PosEdgeCalc("PCLK", false, current_time), mTimeUnit);
        } else {
            mPRESETZHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Receive the value of HRESETZ port
void AHG::ReceiveHRESETZHardMethod(void)
{//{{{
    if (mIsConstructHRESETZ) {
        mIsConstructHRESETZ = false;
    } else {
        mHRESETZValue = HRESETZ.read();
        if (mHCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mHRESETZHardEvent.notify(PosEdgeCalc("HCLK", false, current_time), mTimeUnit);
        } else {
            mHRESETZHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Hanlde the value of PRESETZ port
void AHG::HandlePRESETZHardMethod(void)
{//{{{
    if (mPRESETZValue == emResetActive) {
        mIsPRESETZHardActive = true;
        re_printf("info","PRESETZ reset signal is asserted.\n");
        this->EnablePRESETZ(mIsPRESETZHardActive);
    } else if (mIsPRESETZHardActive) {
        mIsPRESETZHardActive = false;
        re_printf("info","PRESETZ reset signal is de-asserted.\n");
        this->EnablePRESETZ(mIsPRESETZHardActive);
    }
    // Cancel Command AssertReset when PRESETZ is active
    if (mIsPRESETZCmdReceive) {
        mPRESETZCmdEvent.cancel();
        mCancelPRESETZCmdEvent.cancel();
        mIsPRESETZCmdReceive = false;
        mIsPRESETZCmdActive = false;
    }
}//}}}

///Hanlde the value of HRESETZ port
void AHG::HandleHRESETZHardMethod(void)
{//{{{
    if (mHRESETZValue == emResetActive) {
        mIsHRESETZHardActive = true;
        re_printf("info","HRESETZ reset signal is asserted.\n");
        this->EnableHRESETZ();
    } else if (mIsHRESETZHardActive) {
        mIsHRESETZHardActive = false;
        re_printf("info","HRESETZ reset signal is de-asserted.\n");
    }
    // Cancel Command AssertReset when HRESETZ is active
    if (mIsHRESETZCmdReceive) {
        mHRESETZCmdEvent.cancel();
        mCancelHRESETZCmdEvent.cancel();
        mIsHRESETZCmdReceive = false;
        mIsHRESETZCmdActive = false;
    }
}//}}}

///Hanlde command of PRESETZ reset
void AHG::HandlePRESETZCmdMethod(void)
{//{{{
    mIsPRESETZCmdActive = true;
    re_printf("info","AHG model operation is reset by AssertReset command.\n");
    this->EnablePRESETZ(mIsPRESETZCmdActive);
    mCancelPRESETZCmdEvent.notify(mCmdPRESETZResetPeriod, SC_NS);
}//}}}

///Hanlde command of HRESETZ reset
void AHG::HandleHRESETZCmdMethod(void)
{//{{{
    mIsHRESETZCmdActive = true;
    re_printf("info","AHG model operation is reset by AssertReset command.\n");
    this->EnableHRESETZ();
    mCancelHRESETZCmdEvent.notify(mCmdHRESETZResetPeriod, SC_NS);
}//}}}

///Cancel command of PRESETZ reset when reset period is over
void AHG::CancelPRESETZCmdMethod(void)
{//{{{
    mIsPRESETZCmdActive = false;
    mIsPRESETZCmdReceive = false;
    this->EnablePRESETZ(mIsPRESETZCmdActive);
    re_printf("info","PRESETZ reset period is over.\n");
}//}}}

///Cancel command of HRESETZ reset when reset period is over
void AHG::CancelHRESETZCmdMethod(void)
{//{{{
    mIsHRESETZCmdActive = false;
    mIsHRESETZCmdReceive = false;
    re_printf("info","HRESETZ reset period is over.\n");
}//}}}

///Write the value to INTAHB port
void AHG::WriteINTAHBMethod(void)
{//{{{
    INTAHB.write(mIntrValue);
    if (mDumpInterrupt) {
        if (mIntrValue) {
            re_printf("info","INT [INTAHB] Assert.\n");
        } else {
            if ((mHRESETZValue) && (!mIsHRESETZCmdActive)) {
                re_printf("info","INT [INTAHB] De-assert.\n");
            }
        }
    }
}//}}}

///trigger a event to deactivate INTAHB port
void AHG::ClearINTAHBMethod(void)
{//{{{
    mIntrValue = false;
    mWriteIntrEvent.notify();
}//}}}

///Inform error information to AHBERRSLV class
void AHG::HandleErrorInfoMethod(void)
{//{{{
    mAHBERRSLV->HandleErrInfo();
    ControlIntr();
}//}}}

///Ignore error access
void AHG::ErrorAccessIgnoreMethod(void)
{//{{{
    mErrorAccessIgnore = false;
}//}}}

///Clear error status
void AHG::StatusClearMethod(void)
{//{{{
    mStatusClearIgnore = false;
    re_printf("info","Error flags (OVF, ERR) are cleared\n");
    mAHBERRSLV->ClearErrorFlags(mClearOnlyOVF); 
}//}}}

///Call function of AhgDataHandler to inform that transferring data is completed
void AHG::NotifyTranferCompleteMethod(void)
{//{{{
    mAhgDataHandler->TransferComplete();
}//}}}

///Check clock frequency
void AHG::CheckFreq(sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
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

///Calculate the time to synchronize a event with clock posedge
double AHG::PosEdgeCalc(const std::string clock_name, const bool add_period, const double time_point)
{//{{{
    //calcualte the time point of next pos edge of PCLK
    double period_num = 0;
    if (clock_name == "PCLK") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//HCLK
        period_num = time_point / (double)mHCLKPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "PCLK") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//HCLK
        pos_edge_point = (double)mod_period_num * (double)mHCLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Reset the AHG model in HRESETZ domain
void AHG::EnableHRESETZ(void)
{//{{{ 
    mAHBERRSLV->Reset(true, false);//Reset only internal variables
    mErrorAccessIgnoreEvent.cancel();
    mStatusClearEvent.cancel();
    mHandleErrorInfoEvent.cancel();
    mWriteIntrEvent.cancel();
    mClearIntrEvent.cancel();
    mIntrValue = false;
    mErrorAccessIgnore = false;
    mStatusClearIgnore = false;
    mClearOnlyOVF = true;
    mWriteIntrEvent.notify();
}//}}}

///Reset the AHG model in PRESETZ domain
void AHG::EnablePRESETZ(bool is_active)
{//{{{ 
    mAHBFSGuard->Reset(is_active);
    mAHBERRSLV->Reset(is_active, true);//Reset only register
}//}}}

///Separate a string to a vector
void AHG::SeparateString(std::vector<std::string> &vtr, const std::string msg)
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
void AHG::GetResolutionTime(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Control interrupt port INTAHB
void AHG::ControlIntr(void)
{//{{{
    double current_time = sc_time_stamp().to_double(); //get current time
    //synchronization with HCLK before adding 2*HCLK
    double time_to_hclk_first = PosEdgeCalc("HCLK", false, current_time) + 2*(double)mHCLKPeriod;
    //adding synchronization with PCLK before adding 3*HCLK
    double time_to_pclk = PosEdgeCalc("PCLK", true, current_time + time_to_hclk_first) + 3*(double)mPCLKPeriod;
    //synchronization with HCLK
    double time_to_hclk_second = PosEdgeCalc("HCLK", false, current_time + time_to_hclk_first + time_to_pclk);
    mIntrValue = true;
    //Assert INTAHB after 2*HCLK + Max(PCLK) + 3*PCLK
    mWriteIntrEvent.notify(time_to_hclk_first + time_to_pclk, mTimeUnit);
    //Clear INTAHB after 1*PCLK
    mClearIntrEvent.notify(time_to_hclk_first + time_to_pclk + (double)mPCLKPeriod, mTimeUnit);
    //Release restricted time space 2*HCLK + Max(PCLK) + 3*PCLK + Max (HCLK)
    mErrorAccessIgnoreEvent.notify(time_to_hclk_first + time_to_pclk + time_to_hclk_second, mTimeUnit);
}//}}}

///Synchronize the event with clock posedge
void AHG::WaitPosEdgeClk(void)
{//{{{
    if (CheckClock("PCLK")&&CheckClock("HCLK")) {
        double current_time = sc_time_stamp().to_double();
        if (!mErrorAccessIgnore) {
            mErrorAccessIgnore = true;
            mHandleErrorInfoEvent.notify(PosEdgeCalc("HCLK", false, current_time), mTimeUnit);
        }
    }
}//}}}

///Check clock period
bool AHG::CheckClock(const std::string clock_name)
{//{{{
    if (clock_name == "PCLK") {
        if (mPCLKPeriod > 0) {
            return true;
        } else {
            re_printf("info","%s frequency is zero.\n",clock_name.c_str());
            return false;
        }
    } else {//HCLK
        if (mHCLKPeriod > 0) {
            return true;
        } else {
            re_printf("info","%s frequency is zero.\n",clock_name.c_str());
            return false;
        }
    }
}//}}}

///Check access permission tp peripherals
bool AHG::CheckAccessPermission(TlmBasicPayload &trans, const bool debug, bool &is_valid_trans)
{//{{{
    return mAHBFSGuard->CheckAccessibility(trans, debug, is_valid_trans);
}//}}}

///Transfer error information to AHBERRSLV class
void AHG::TransferErrInfo(TlmBasicPayload &trans)
{//{{{
    mAHBERRSLV->InformError(trans);
    WaitPosEdgeClk();
}//}}}

///Wait a synchronous time to clear error status
void AHG::WaitTimeToClearErrorFlags(bool ovf_flag)
{//{{{
    if (CheckClock("PCLK")&&CheckClock("HCLK")) {
        double current_time = sc_time_stamp().to_double();
        //synchronization with PCLK before adding 3*PCLK and 2*HCLK
        double time_to_pclk_first = PosEdgeCalc("PCLK", false, current_time) + 3*(double)mPCLKPeriod + 2*(double)mHCLKPeriod;
        //adding synchronization with HCLK
        double time_to_hclk =  PosEdgeCalc("HCLK", true, current_time + time_to_pclk_first);
        //adding synchronization with PCLK
        double time_to_pclk_second = PosEdgeCalc("PCLK", true, current_time + time_to_pclk_first + time_to_hclk);
        if(!mStatusClearIgnore){
            mStatusClearIgnore = true;
            if (ovf_flag) {
                mClearOnlyOVF = true;
            } else {
                mClearOnlyOVF = false;
            }
            mStatusClearEvent.notify(time_to_pclk_first + time_to_hclk + time_to_pclk_second, mTimeUnit);
        }
    }
}//}}}

///Get reset status
bool AHG::GetResetStatus(void)
{//{{{
    bool status = false;
    if (mIsHRESETZHardActive || mIsHRESETZCmdActive) {
        status = true;
    } else {
        status = false;
    }
    return status;
}//}}}

///Notify event to inform that transferring data is completed
void AHG::NotifyTransComp(void)
{//{{{
    mTransferCompleteEvent.notify();
}//}}}

// vim600: set foldmethod=marker :
