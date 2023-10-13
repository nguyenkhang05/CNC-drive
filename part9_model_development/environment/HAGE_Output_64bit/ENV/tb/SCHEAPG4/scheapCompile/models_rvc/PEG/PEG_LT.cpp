// ---------------------------------------------------------------------
// $Id: PEG_LT.cpp,v 1.7 2015/06/12 08:19:58 dungvannguyen Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "PEG_LT.h"
#include "PEG_DataHandler.h"
#include "PEGUARD.h" 

///Constructor of PEG class
PEG::PEG(sc_module_name name,
         unsigned int rLatency,
         unsigned int wLatency,
         unsigned int PEG_ID,
         std::string PEG_VERSION):
         PEG_AgentController(),
         BusBridgeModuleBase<32,32>(name),
         BusBridgeSlaveBase<32,1>(),
         BusMasterBase<32,1>(),
         BusSlaveBase<32,1>(),
         pclk("pclk"),
         lvcclk("lvcclk"),
         presetz("presetz"),
         lvcreset("lvcreset"),
         PEG_ERR("PEG_ERR"),
         VciErrInfo("VciErrInfo"),
         mInternalRstSig("mInternalRstSig")
{//{{{
    //Handle config parameters
    mVersion = PEG_VERSION;

    //Initialize variables
    mIsStartSimulation = false;
    mIsAreaNumberSet = false;
    mNumberOfArea = 0;
    mAreaCounter = 0;
    mPclkPeriod = 0;
    mLvcclkPeriod = 0;
    mPclkFreq = 0;
    mLvcclkFreq = 0;
    mPclkOrgFreq = 0;
    mLvcclkOrgFreq = 0;
    mPclkFreqUnit = "Hz";
    mLvcclkFreqUnit = "Hz";
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mPresetzSignalCurrentValue = emPresetzInactive;
    mPresetzSignalPreviousValue = emPresetzInactive;
    mIsPresetzHardActive = false;
    mIsPresetzCmdReceive = false;
    mIsPresetzCmdActive = false;
    mPresetzCmdPeriod =  0;

    mLvcresetSignalCurrentValue = emLvcresetInactive;
    mLvcresetSignalPreviousValue = emLvcresetInactive;
    mIsLvcresetHardActive = false;
    mIsLvcresetCmdReceive = false;
    mIsLvcresetCmdActive = false;
    mLvcresetCmdPeriod =  0;

    mPEG_ERRValue.Initialize();
    mVciErrInfoValue.Initialize();
    PEG_ERR.initialize(mPEG_ERRValue);
    VciErrInfo.initialize(mVciErrInfoValue);

    setMasterResetPort32(&lvcreset);
    setMasterFreqPort32(&lvcclk);
    setInitiatorSocket32("is");

    setSlaveFreqPort32(&pclk);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("tsp");

    setSlaveResetPort(&lvcreset);
    setSlaveFreqPort(&lvcclk);
    setTargetSocket("tsv");

    tsp = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding
    tsv = BusBridgeSlaveBase<32,1>::tSocket[0]; //Alias for binding
    is  = iSocket32[0]; //Alias for binding

    resetPort.bind(mInternalRstSig);
    mInternalRstSig.write(true);

    mPEGUARD = new PEGUARD((std::string)name, this, PEG_ID, mVersion);
    sc_assert(mPEGUARD != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mPEGUARD);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0xFFFFFFFF);

    mpCore = new PEG_DataHandler((std::string)name, this, mBusSlaveIf[0], mBusMasterIf32[0], this);
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mpCore);
    mBusMasterIf32[0]->setBusProtocol(BUS_VCI);
    mBusMasterIf32[0]->setTransNmbLmt(1);
    mBusSlaveIf[0]->setFuncModulePtr( (BusSlaveFuncIf *)mpCore);
    mBusSlaveIf[0]->setBusProtocol(BUS_VCI);
    mBusSlaveIf[0]->setTransNmbLmt(0xFFFFFFFF);

    SC_METHOD(HandlePclkSignalMethod);
    dont_initialize();
    sensitive<<pclk;
 
    SC_METHOD(HandleLvcclkSignalMethod);
    dont_initialize();
    sensitive<<lvcclk;
 
    SC_METHOD(HandlePresetzSignalMethod);
    sensitive<<presetz;
    
    SC_METHOD(HandlePresetzHardMethod);
    dont_initialize();
    sensitive<<mPresetzHardEvent;

    SC_METHOD(HandlePresetzCmdMethod);
    dont_initialize();
    sensitive<<mPresetzCmdEvent;
 
    SC_METHOD(CancelPresetzCmdMethod);
    dont_initialize();
    sensitive<<mPresetzCmdCancelEvent;

    SC_METHOD(HandleLvcresetSignalMethod);
    sensitive<<lvcreset;
    
    SC_METHOD(HandleLvcresetHardMethod);
    dont_initialize();
    sensitive<<mLvcresetHardEvent;

    SC_METHOD(HandleLvcresetCmdMethod);
    dont_initialize();
    sensitive<<mLvcresetCmdEvent;
 
    SC_METHOD(CancelLvcresetCmdMethod);
    dont_initialize();
    sensitive<<mLvcresetCmdCancelEvent;

    SC_METHOD(UpdatePEG_ERRMethod);
    dont_initialize();
    sensitive<<mUpdatePEG_ERREvent;

    SC_METHOD(WritePEG_ERRMethod);
    dont_initialize();
    sensitive<<mWritePEG_ERREvent;

    SC_METHOD(ClearPEG_ERRMethod);
    dont_initialize();
    sensitive<<mClearPEG_ERREvent;

    SC_METHOD(UpdateVciErrInfoMethod);
    dont_initialize();
    sensitive<<mUpdateVciErrInfoEvent;

    SC_METHOD(WriteVciErrInfoMethod);
    dont_initialize();
    sensitive<<mWriteVciErrInfoEvent;

    SC_METHOD(ClearVciErrInfoMethod);
    dont_initialize();
    sensitive<<mClearVciErrInfoEvent;
}//}}}

///Destructor of PEG class
PEG::~PEG(void)
{//{{{
    delete mPEGUARD;
    delete mpCore;
}//}}}

///This is virtual function of SystemC kernel. It is called by SystemC kernel at simulation start
void PEG::start_of_simulation(void)
{//{{{
   mIsStartSimulation = true;
}//}}}

//Set address of guard areas
void PEG::SetAreaAddress(const std::string area_name, const unsigned int start_addr, const unsigned int size, const std::string pe_name)
{//{{{
    if (!mIsStartSimulation) {
        if ((area_name == "DEBUG") || (area_name == "APB") || (area_name == "TCM") || (area_name == "ROM")) {
            if (mVersion != "G3MH") {
                if ((pe_name == "PE1") || (pe_name == "PE2") || (pe_name == "BOTH")) {
                    mPEGUARD->SetAreaAddr(area_name, start_addr, size, pe_name);
                } else {
                    re_printf("error","PEID is invalid.\n");
                }
            } else {
                if ((pe_name == "PE1") || (pe_name == "PE2") || (pe_name == "PE3") || (pe_name == "BOTH")) {
                    mPEGUARD->SetAreaAddr(area_name, start_addr, size, pe_name);
                } else {
                    re_printf("error","PEID is invalid.\n");
                }
            }
        } else {
            re_printf("error","Area name is invalid. It should be DEBUG or APB or TCM or ROM.\n");
        }
    } else {
        re_printf("warning","Cannot configure address area after simulation has been started.\n");
    }
}//}}}

//Set guard mode
void PEG::SetAccessMode(const std::string mode, const std::string is_enable)
{//{{{
    if ((is_enable == "true") || (is_enable == "false")) {
        if (is_enable == "true") {
            mPEGUARD->SetAccessMode(mode, true);
        } else {
            mPEGUARD->SetAccessMode(mode, false);
        }
    } else {
        re_printf("error","Input argument is invalid. It should be true or false.\n");
    }
}//}}}

//Set intial value of register in area 0
void PEG::SetG0InitialVal(const std::string bit_name, const unsigned int value)
{//{{{
    mPEGUARD->SetG0InitialVal(bit_name, value, mIsStartSimulation);
}//}}}

///Assert reset by software for Presetz and Lvcreset
void PEG::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz") {
        if ((!mIsPresetzHardActive)&&(!mIsPresetzCmdReceive)) {
            mIsPresetzCmdReceive = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mPresetzCmdEvent.notify(start_time, SC_NS);
            mPresetzCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "lvcreset") {
        if ((!mIsLvcresetHardActive)&&(!mIsLvcresetCmdReceive)) {
            mIsLvcresetCmdReceive = true;
            re_printf("info","The model will be reset (lvcreset) for %f ns after %f ns.\n", period, start_time);
            mLvcresetCmdEvent.notify(start_time, SC_NS);
            mLvcresetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of lvcreset is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("error","The reset name (%s) is wrong. It should be presetz or lvcreset.\n",reset_name.c_str());
    }
}//}}}

///Set clock frequency via Python IF
void PEG::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, clock_name);
    if (temp.size() == 1) {
        if (temp[0] == "pclk") {
            mPclkOrgFreq = freq;
            ConvertClockFreq(mPclkFreq, mPclkFreqUnit, freq, unit);
            if (mPclkFreq > 0) {
                mPclkPeriod = (sc_dt::uint64)(((1/(double)mPclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPclkPeriod = 0;
                mUpdatePEG_ERREvent.cancel();
                mWritePEG_ERREvent.cancel();
                mClearPEG_ERREvent.cancel();
                mUpdateVciErrInfoEvent.cancel();
                mWriteVciErrInfoEvent.cancel();
                mClearVciErrInfoEvent.cancel();
            }
        } else if (temp[0] == "lvcclk") {
            mLvcclkOrgFreq = freq;
            ConvertClockFreq(mLvcclkFreq, mLvcclkFreqUnit, freq, unit);
            if (mLvcclkFreq > 0) {
                mLvcclkPeriod = (sc_dt::uint64)(((1/(double)mLvcclkFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mLvcclkPeriod = 0;
                mUpdatePEG_ERREvent.cancel();
                mWritePEG_ERREvent.cancel();
                mClearPEG_ERREvent.cancel();
                mUpdateVciErrInfoEvent.cancel();
                mWriteVciErrInfoEvent.cancel();
                mClearVciErrInfoEvent.cancel();
            }
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Get clock frequency via Python IF
void PEG::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, clock_name);
    if (temp.size() == 1) {
        if (temp[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s.\n", (double)mPclkOrgFreq, mPclkFreqUnit.c_str());
        } else if (temp[0] == "lvcclk") {
            re_printf("info","lvcclk frequency is %0.0f %s.\n", (double)mLvcclkOrgFreq, mLvcclkFreqUnit.c_str());
        } else {
            re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("error","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info) via Python IF
void PEG::SetMessageLevel(const std::string msg_lv)
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
    mPEGUARD->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mPEGUARD->RegisterHandler(cmd);
}//}}}

///Set whether register access information is dumpped via Python IF
void PEG::DumpRegisterRW(const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mPEGUARD->RegisterHandler(cmd);
}//}}}

///Force a value to register via Python IF
void PEG::ForceRegister(const std::string reg_name, const unsigned int reg_value)
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
            mPEGUARD->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value of registers via Python IF
void PEG::ReleaseRegister(const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mPEGUARD->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void PEG::WriteRegister (const std::string reg_name, const unsigned int reg_value)
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
            mPEGUARD->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void PEG::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != ""){
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mPEGUARD->RegisterHandler(cmd);
        } else {
            re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("error","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///List all registers via Python IF
void PEG::ListRegister(void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mPEGUARD->RegisterHandler(cmd);
}//}}}

///Dump parameters and Commands in PEG model
void PEG::Help(const std::string type)
{//{{{
    std::vector<std::string> temp;
    SeparateString(temp, type);
    if (temp.size() == 1) {
        if (temp[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"----Parameters----");
            SC_REPORT_INFO(this->basename(),"PEG_MessageLevel (fatal|error|warning|info)     Select debug message level (Default: fatal,error)");
            SC_REPORT_INFO(this->basename(),"PEG_DumpFileNameLineNum (true/false)            Select dump information about file name and line number.");
        } else if (temp[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"----Commands----");
            SC_REPORT_INFO(this->basename(),"PEG_AssertReset (start_time, period)            Assert and de-assert reset signal to the PEG model.");
            SC_REPORT_INFO(this->basename(),"PEG_SetCLKFreq (clock_name, freq, unit)         Set frequency value to PEG. Valid clock_name is pclk or lvcclk.");
            SC_REPORT_INFO(this->basename(),"PEG_GetCLKFreq (clock_name)                     Get frequency value of PEG. Valid clock_name is pclk or lvcclk.");
            SC_REPORT_INFO(this->basename(),"PEG_ForceRegister (reg_name, value)             Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"PEG_ReleaseRegister (reg_name)                  Release register from force value.");
            SC_REPORT_INFO(this->basename(),"PEG_WriteRegister (reg_name, reg_value)         Write a value to a register.");
            SC_REPORT_INFO(this->basename(),"PEG_ReadRegister (reg_name)                     Read a value from a register.");
            SC_REPORT_INFO(this->basename(),"PEG_ListRegister ()                             Dump register names of PEGUARD block.");
            SC_REPORT_INFO(this->basename(),"PEG_SetAccessMode (mode, is_enable)             Access method enable.");
            SC_REPORT_INFO(this->basename(),"PEG_SetAreaAddress (area_name, start_addr, size, pe_name)      Set address of each memory area.");
            SC_REPORT_INFO(this->basename(),"PEG_SetG0InitialVal (bit_name, value)           Area 0 initial setting.");
        } else {
            re_printf("error","The name (%s) of PEG_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("error","The name (%s) of PEG_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///Handle Pclk signal
void PEG::HandlePclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

///Handle Lvcclk signal
void PEG::HandleLvcclkSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = lvcclk.read();
    SetCLKFreq("lvcclk", freq_value, "Hz");
}//}}}

///Handle presetz signal
void PEG::HandlePresetzSignalMethod (void)
{//{{{
    mPresetzSignalCurrentValue = presetz.read();
    if (mPresetzSignalCurrentValue != mPresetzSignalPreviousValue) {//mPresetzSignalPreviousValue is "true" at initial state
        mPresetzSignalPreviousValue = mPresetzSignalCurrentValue;
        if (mPclkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mPresetzHardEvent.notify(CalculateCLKPosEdge("pclk", current_time), mTimeResolutionUnit);
        } else {
            mPresetzHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Handle lvcreset signal
void PEG::HandleLvcresetSignalMethod (void)
{//{{{
    mLvcresetSignalCurrentValue = lvcreset.read();
    mInternalRstSig.write(!mLvcresetSignalCurrentValue);
    if (mLvcresetSignalCurrentValue != mLvcresetSignalPreviousValue) {//mLvcresetSignalPreviousValue is "true" at initial state
        mLvcresetSignalPreviousValue = mLvcresetSignalCurrentValue;
        if (mLvcclkPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mLvcresetHardEvent.notify(CalculateCLKPosEdge("lvcclk", current_time), mTimeResolutionUnit);
        } else {
            mLvcresetHardEvent.notify(SC_ZERO_TIME);
        }
    }
}//}}}

///Process reset function when presetz is active
void PEG::HandlePresetzHardMethod (void)
{//{{{
    if (mPresetzSignalCurrentValue == emPresetzActive) {
        mIsPresetzHardActive = true;
        re_printf("info","The reset signal presetz is asserted.\n");
        this->EnableReset("presetz", mIsPresetzHardActive);
    } else if (mIsPresetzHardActive) {
        mIsPresetzHardActive = false;
        this->EnableReset("presetz", mIsPresetzHardActive);
        re_printf("info","The reset signal presetz is de-asserted.\n");
    }
    //Cancel AssertReset command when presetz is active
    if (mIsPresetzCmdReceive) {
        mPresetzCmdEvent.cancel();
        mPresetzCmdCancelEvent.cancel();
        mIsPresetzCmdReceive = false;
        mIsPresetzCmdActive = false;
    }
}//}}}

///Process reset function when lvcreset is active
void PEG::HandleLvcresetHardMethod (void)
{//{{{
    if (mLvcresetSignalCurrentValue == emLvcresetActive) {
        mIsLvcresetHardActive = true;
        re_printf("info","The reset signal lvcreset is asserted.\n");
        this->EnableReset("lvcreset", mIsLvcresetHardActive);
    } else if (mIsLvcresetHardActive) {
        mIsLvcresetHardActive = false;
        this->EnableReset("lvcreset", mIsLvcresetHardActive);
        re_printf("info","The reset signal lvcreset is de-asserted.\n");
    }
    //Cancel AssertReset command when lvcreset is active
    if (mIsLvcresetCmdReceive) {
        mLvcresetCmdEvent.cancel();
        mLvcresetCmdCancelEvent.cancel();
        mIsLvcresetCmdReceive = false;
        mIsLvcresetCmdActive = false;
    }
}//}}}

///Process reset function when presetz command is active
void PEG::HandlePresetzCmdMethod (void)
{//{{{
    mIsPresetzCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetz.\n");
    this->EnableReset("presetz", mIsPresetzCmdActive);
    mPresetzCmdCancelEvent.notify(mPresetzCmdPeriod, SC_NS);
}//}}}

///Process reset function when lvcreset command is active
void PEG::HandleLvcresetCmdMethod (void)
{//{{{
    mIsLvcresetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of lvcreset.\n");
    this->EnableReset("lvcreset", mIsLvcresetCmdActive);
    mLvcresetCmdCancelEvent.notify(mLvcresetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when presetz command is active
void PEG::CancelPresetzCmdMethod (void)
{//{{{
    mIsPresetzCmdActive = false;
    mIsPresetzCmdReceive = false;
    re_printf("info","Reset period of presetz is over.\n");
    this->EnableReset("presetz", mIsPresetzCmdActive); 
}//}}}

///Cancel reset function when lvcreset command is active
void PEG::CancelLvcresetCmdMethod (void)
{//{{{
    mIsLvcresetCmdActive = false;
    mIsLvcresetCmdReceive = false;
    re_printf("info","Reset period of lvcreset is over.\n");
    this->EnableReset("lvcreset", mIsLvcresetCmdActive); 
}//}}}

void PEG::UpdatePEG_ERRMethod(void)
{//{{{
    if ((CheckClock("lvcclk")) && (CheckClock("pclk"))) {
        mWritePEG_ERREvent.notify();
        mClearPEG_ERREvent.notify((double)mLvcclkPeriod, mTimeResolutionUnit);
    }
}//}}}

///Write the value to PEG_ERR port
void PEG::WritePEG_ERRMethod(void)
{//{{{
    PEG_ERR.write(mPEG_ERRValue);
}//}}}

///trigger a event to deactivate PEG_ERR port
void PEG::ClearPEG_ERRMethod(void)
{//{{{
    if ((CheckClock("lvcclk")) && (CheckClock("pclk"))) {
        mPEG_ERRValue.Initialize();
        mWritePEG_ERREvent.notify();
    }
}//}}}

void PEG::UpdateVciErrInfoMethod(void)
{//{{{
    if ((CheckClock("lvcclk")) && (CheckClock("pclk"))) {
        mWriteVciErrInfoEvent.notify();
        mClearVciErrInfoEvent.notify((double)mLvcclkPeriod, mTimeResolutionUnit);
    }
}//}}}

///Write the value to VciErrInfo port
void PEG::WriteVciErrInfoMethod(void)
{//{{{
    VciErrInfo.write(mVciErrInfoValue);
}//}}}

///trigger a event to deactivate VciErrInfo port
void PEG::ClearVciErrInfoMethod(void)
{//{{{
    if ((CheckClock("lvcclk")) && (CheckClock("pclk"))) {
        unsigned int sideband = mVciErrInfoValue.SysErrVCIInfo & 0xFFFF;
        unsigned int peid = 0;
        if (mVersion == "G3M_V30") {
            peid = (sideband >> 13) & 0x7;
            sideband = peid << 13;
        } else {
            peid = (sideband >> 4) & 0x7;
            sideband = peid << 4;
        }
        mVciErrInfoValue.Initialize();
        mVciErrInfoValue.SysErrVCIInfo = sideband;
        mWriteVciErrInfoEvent.notify();
    }
}//}}}

///Process reset function
void PEG::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mPEGUARD->Reset(reset_name, is_active);
    if ((reset_name == "lvcreset") && (is_active)) {//Initialize variables, output ports and cancel events
        mPEG_ERRValue.Initialize();
        mVciErrInfoValue.Initialize();

        mUpdatePEG_ERREvent.cancel();
        mUpdateVciErrInfoEvent.cancel();
        mClearPEG_ERREvent.cancel();
        mClearVciErrInfoEvent.cancel();
        mWritePEG_ERREvent.cancel();
        mWriteVciErrInfoEvent.cancel();
        mWritePEG_ERREvent.notify();
        mWriteVciErrInfoEvent.notify();
    }
}//}}}

///Check frequency value and frequency unit of Pclk clock
void PEG::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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

///Calculate synchronous time with pclk
double PEG::CalculateCLKPosEdge (const std::string clock_name, const double time_point)
{//{{{
    double period_num = 0;
    if (clock_name == "pclk") {
        period_num = time_point / (double)mPclkPeriod;
    } else {//clock_name is lvcclk
        period_num = time_point / (double)mLvcclkPeriod;
    }
    unsigned int mod_period_num = (unsigned int)period_num;
    if ((double)mod_period_num < period_num) {
        mod_period_num++;
    }
    double pos_edge_point = 0;
    if (clock_name == "pclk") {
        pos_edge_point = (double)mod_period_num * (double)mPclkPeriod - time_point;
    } else {//clock_name is lvcclk
        pos_edge_point = (double)mod_period_num * (double)mLvcclkPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///Separate some words from a string to store a vector
void PEG::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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
void PEG::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///Control PEG_ERR output port
void PEG::ControlOutput(void)
{//{{{
    double current_time = sc_time_stamp().to_double(); //get current time
    double time_point = CalculateCLKPosEdge("lvcclk", current_time);
    mUpdatePEG_ERREvent.notify(time_point, mTimeResolutionUnit);
    mUpdateVciErrInfoEvent.notify(time_point, mTimeResolutionUnit);
}//}}}

///Check access permission tp peripherals
bool PEG::CheckAccessPermission(TlmBasicPayload &trans)
{//{{{
    Cerrinfo err_info;
    Csyserrvci syserr_info;
    bool result = mPEGUARD->CheckAccessibility(trans, err_info, syserr_info);
    if (!result) {
        mPEG_ERRValue = err_info;
        mVciErrInfoValue = syserr_info;
        ControlOutput();
    }
    return result;
}//}}}

///Check clock period
bool PEG::CheckClock(const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = (clock_name == "pclk")?mPclkPeriod:mLvcclkPeriod;
    if (clock_period > 0) {
        clock_status = true;
    } else {
        re_printf("info","%s frequency is zero.\n",clock_name.c_str());
        clock_status = false;
    }
    return clock_status;
}//}}}

///Get reset status
bool PEG::GetResetStatus(void)
{//{{{
    bool status = false;
    if (mIsLvcresetHardActive || mIsLvcresetCmdActive) {
        status = true;
    } else {
        status = false;
    }
    return status;
}//}}}

// vim600: set foldmethod=marker :
