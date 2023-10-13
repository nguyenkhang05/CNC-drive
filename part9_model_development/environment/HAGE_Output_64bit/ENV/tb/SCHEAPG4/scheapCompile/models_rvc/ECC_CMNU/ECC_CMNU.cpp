// ---------------------------------------------------------------------
// $Id: ECC_CMNU.cpp,v 1.7 2015/11/06 02:01:35 ngankimtran2 Exp $
//
// Copyright(c) 2014 - 2015 Renesas Electronics Corporation
// Copyright(c) 2014 - 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "ECC_CMNU.h"
#include "ECC_CMNU_Func.h"

///*********************************
/// Function     : ECC_CMNU
/// Description  : Constructor of ECC_CMNU class
///*********************************
ECC_CMNU::ECC_CMNU (sc_module_name name,
                  unsigned int rLatency,
                  unsigned int wLatency,
                  unsigned int ECCCTLInitial,
                  unsigned int ERRINTInitial,
                  bool SupDupsed,
                  unsigned int addr_awidth,
                  unsigned int addr_awidth_lsb,
                  std::string model_version):
                sc_module(name),
                ECC_CMNU_AgentController(),
                BusSlaveBase<32,1>(),
                pclk("pclk"),
                sys_clk("sys_clk"),
                presetz("presetz"),
                sys_resetz("sys_resetz"),
                aeccdisable("aeccdisable"),
                asecdisable("asecdisable"),
                eccdisable("eccdisable"),
                secdisable("secdisable"),
                adederrout("adederrout"),
                asederrout("asederrout"),
                dederrout("dederrout"),
                sederrout("sederrout"),
                aperrout("aperrout"),
                bperrout("bperrout"),
                errovfout("errovfout")
{//{{{
    //Initialize output signals
    aeccdisable.initialize(false);
    asecdisable.initialize(false);
    eccdisable.initialize(false);
    secdisable.initialize(false);
    adederrout.initialize(false);
    asederrout.initialize(false);
    dederrout.initialize(false);
    sederrout.initialize(false);
    aperrout.initialize(false);
    bperrout.initialize(false);
    errovfout.initialize(false);
    //--------

    //Initialize variables - for clock, time resolution, reset
    mPCLKPeriod = 0;
    mSYS_CLKPeriod = 0;
    mPCLKFreq = 0;
    mPCLKOrgFreq = 0;
    mSYS_CLKFreq = 0;
    mSYS_CLKOrgFreq = 0;
    mPCLKFreqUnit = "Hz";
    mSYS_CLKFreqUnit = "Hz";

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
    //--------

    //Initialize variables - for parameters setting by Heap configure and Python IF
    if (model_version == "G3M_V30" || model_version == "CFB_G3KH" || model_version == "TCM_G3KH" ||
        model_version == "CFB_G3MH" || model_version == "CFB_X2X" || model_version == "TCM_G3MH" ||
        model_version == "GRAM_G3MH" || model_version == "ICDM_G3MH" || model_version == "ICTM_G3MH") {
        mECCCMNUVersion = model_version;
    } else {
        re_printf("warning","The argument (%s) of the ECCCMNU_VERSION parameter is invalid. (G3M_V30) is set as default.\n",model_version.c_str());
        mECCCMNUVersion = "G3M_V30";
    }
    if ((addr_awidth >= 18) && (addr_awidth <= 32)) {
        mAWIDTH = addr_awidth;
    } else {
        re_printf("warning","The argument (%d) of the ECCCMNU_AWIDTH parameter is invalid. (32) is set as default.\n",addr_awidth);
        mAWIDTH = 32;
    }
    if ((addr_awidth_lsb >= 0) && (addr_awidth_lsb <= 5)) {
        mAWIDTH_LSB = addr_awidth_lsb;
    } else {
        re_printf("warning","The argument (%d) of the ECCCMNU_AWIDTH_LSB parameter is invalid. (0) is set as default.\n",addr_awidth_lsb);
        mAWIDTH_LSB = 0;
    }
    mIsUseFifo = false;
    mIsUseApbEcc = false;
    //--------

    //Initialize other variables - for error capturing/reporting
    this->Initialize();
    
    //Bind clock, reset signal and target socket to Bus IF
    setSlaveFreqPort32(&pclk);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("ts");
    ts = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    mECCCMNUFunc = new ECC_CMNU_Func((std::string)name, this, ECCCTLInitial, ERRINTInitial, SupDupsed, mAWIDTH, mAWIDTH_LSB, mECCCMNUVersion);
    sc_assert(mECCCMNUFunc != NULL);
    mECCCTLOut = mECCCMNUFunc->GetECCCTLVal(); //Get ECCCTLInitial value

    mBusSlaveIf32[0]->setFuncModulePtr(mECCCMNUFunc);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(HandleSYS_CLKSignalMethod);
    dont_initialize();
    sensitive << sys_clk;

    SC_METHOD(HandlePRESETZSignalMethod);
    sensitive << presetz;

    SC_METHOD(HandleSYS_RESETZSignalMethod);
    sensitive << sys_resetz;

    SC_METHOD(WriteECCDisableSignalsMethod);
    sensitive << mECCDisableSignalsControlEvent;

    for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mErrAddrInputEvent[bank_no]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::CaptureErrAddrInputMethod, this, bank_no),
                          sc_core::sc_gen_unique_name("CaptureErrAddrInputMethod"), &opt);
    }

    for (unsigned int bank_no = 0; bank_no < emBankNo; bank_no++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mErrInputEvent[bank_no]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::CaptureErrInputMethod, this, bank_no),
                          sc_core::sc_gen_unique_name("CaptureErrInputMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id < emMaxSignalID; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mRisingErrOutEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::RisingErrOutSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("RisingErrOutSignalsMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mFallingErrOutEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::FallingErrOutSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("FallingErrOutSignalsMethod"), &opt);
    }

    for (unsigned int signal_id = 0; signal_id  < emMaxSignalID; signal_id ++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mErrorOutputWriteEvent[signal_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::WriteErrOutSignalsMethod, this, signal_id),
                          sc_core::sc_gen_unique_name("WriteErrOutSignalsMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::HandleResetHardMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::HandleResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[reset_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&ECC_CMNU::CancelResetCmdMethod, this, reset_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }
}//}}}

///*********************************
/// Function     : ~ECC_CMNU
/// Description  : Destructor of ECC_CMNU class
///*********************************
ECC_CMNU::~ECC_CMNU (void)
{//{{{
    delete mECCCMNUFunc;
}//}}}

///*********************************
/// Function     : Initialize
/// Description  : Initialize variables at initial state or when reset is active
///*********************************
void ECC_CMNU::Initialize (void)
{//{{{
    for (unsigned int i = 0; i < emBankNo; i++) {
        mReqId[i] = 0;
        mRespId[i] = 0;
        for (unsigned int j = 0; j < emFifoDepth; j++) {
            mErrAddrFFIn[i][j] = 0x0;
        }
        mErrAddrIn[i] = 0x0;
        mErrorInfo[i].Initialize();
    }
}//}}}

///*********************************
/// Function     : InitializeOutputSignals
/// Description  : Initialize output signals when reset is active
///*********************************
void ECC_CMNU::InitializeOutputSignals (void)
{//{{{
    for (unsigned int i = 0; i < emMaxSignalID; i++) {
        mErrOut[i] = false;
        mErrorOutputWriteEvent[i].notify(SC_ZERO_TIME);
    }
    mECCCTLOut = 0x0;
    mECCDisableSignalsControlEvent.notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : SetMessageLevel
/// Description  : Set message level (fatal/error/warning/info)
///                Called by PythonIF
///*********************************
void ECC_CMNU::SetMessageLevel (const std::string msg_lv)
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
    mECCCMNUFunc->RegisterHandler(cmd);

    //Set message level for each register in Register IF class
    cmd.clear();
    cmd.push_back("reg");
    cmd.push_back("*");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);
    mECCCMNUFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : DumpRegisterRW
/// Description  : Enable/disable dumping message when users access registers
///                Called by PythonIF
///*********************************
void ECC_CMNU::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mECCCMNUFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : AssertReset
/// Description  : Assert reset by software for presetz, sys_resetz
///                Called by PythonIF
///*********************************
void ECC_CMNU::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetz") {
        if ((!mIsResetHardActive[0])&&(!mIsResetCmdReceive[0])) {
            mIsResetCmdReceive[0] = true;
            re_printf("info","The model will be reset (presetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[0].notify(start_time, SC_NS);
            mResetCmdPeriod[0] = period;
        } else {
            re_printf("warning","The software reset of presetz is called in the reset operation of the model. So it is ignored.\n");
        }
    } else if (reset_name == "sys_resetz") {
        if ((!mIsResetHardActive[1])&&(!mIsResetCmdReceive[1])) {
            mIsResetCmdReceive[1] = true;
            re_printf("info","The model will be reset (sys_resetz) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[1].notify(start_time, SC_NS);
            mResetCmdPeriod[1] = period;
        } else {
            re_printf("warning","The software reset of sys_resetz is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning", "The name of reset signal is wrong. It should be presetz or sys_resetz.\n");
    }
}//}}}

///*********************************
/// Function     : SetCLKFreq
/// Description  : Set clock value and clock unit for pclk, sys_clk
///                Called by PythonIF
///*********************************
void ECC_CMNU::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            CheckFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            mPCLKOrgFreq = freq;
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
                mECCDisableSignalsControlEvent.cancel();
                for (unsigned int i = 0; i < emMaxSignalID; i++) {
                    mRisingErrOutEvent[i].cancel();
                    mFallingErrOutEvent[i].cancel();
                    mErrorOutputWriteEvent[i].cancel();
                }
            }
        } else if (word_vector[0] == "sys_clk") {
            CheckFreq (mSYS_CLKFreq, mSYS_CLKFreqUnit, freq, unit);
            mSYS_CLKOrgFreq = freq;
            if (mSYS_CLKFreq > 0) {
                mSYS_CLKPeriod = (sc_dt::uint64)(((1/(double)mSYS_CLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mSYS_CLKPeriod = 0;
                for (unsigned int i = 0; i < emMaxSignalID; i++) {
                    mRisingErrOutEvent[i].cancel();
                    mFallingErrOutEvent[i].cancel();
                    mErrorOutputWriteEvent[i].cancel();
                }
                for (unsigned int i = 0; i < emBankNo; i++) {
                    mErrInputEvent[i].cancel();
                    mErrAddrInputEvent[i].cancel();
                }
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
/// Description  : Get clock value of pclk, sys_clk
///                Called by PythonIF
///*********************************
void ECC_CMNU::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else if (word_vector[0] == "sys_clk") {
            re_printf("info","sys_clk frequency is %0.0f %s.\n", (double)mSYS_CLKOrgFreq, mSYS_CLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///*********************************
/// Function     : ForceRegister
/// Description  : Force value to register
///                Called by PythonIF
///*********************************
void ECC_CMNU::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("force");
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mECCCMNUFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : ReleaseRegister
/// Description  : Release forced value after forcing registers
///                Called by PythonIF
///*********************************
void ECC_CMNU::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    cmd.push_back("release");
    mECCCMNUFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : WriteRegister
/// Description  : Write value to registers by software
///                Called by PythonIF
///*********************************
void ECC_CMNU::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    std::ostringstream reg_value_str;
    reg_value_str<<"0x"<<std::hex<<reg_value;
    cmd.push_back(reg_value_str.str());
    mECCCMNUFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : ReadRegister
/// Description  : Read value of register by software
///                Called by PythonIF
///*********************************
void ECC_CMNU::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    SeparateString(cmd, reg_name);
    mECCCMNUFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : ListRegister
/// Description  : List all reigsters name
///                Called by PythonIF
///*********************************
void ECC_CMNU::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mECCCMNUFunc->RegisterHandler(cmd);
}//}}}

///*********************************
/// Function     : Help
/// Description  : Dump help message of all parameters or commands
///                Called by PythonIF
///*********************************
void ECC_CMNU::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"--- parameters ---");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_MessageLevel   (fatal|error|warning|info)       Set debug message level (default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_DumpRegisterRW (true/false)                     Enable/disable dumping access register (default: false).");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_FIFO_USE       (true/false)                     Control using FIFO to store error address for capturing (default: false).");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_APB_ECC_USE    (true/false)                     Control supporting GAPB_ECC Unit (default: false).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"--- commands ---");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_IssueError     (bank_no, err_type)              Issue pseudo error.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_SetErrorAddr   (bank_no, addr_value)            Set error detection address.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_AssertReset    (reset_name, start_time, period) Assert and de-assert reset signal (presetz or sys_resetz).");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_SetCLKFreq     (clock_name, freq, unit)         Set clock frequency to pclk or sys_clk clock.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_GetCLKFreq     (clock_name)                     Get clock frequency of pclk or sys_clk clock.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_ForceRegister  (reg_name, value)                Force register with setting value.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_ReleaseRegister(reg_name)                       Release register from force value.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_WriteRegister  (reg_name, value)                Write a value to register.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_ReadRegister   (reg_name)                       Read value from register.");
            SC_REPORT_INFO(this->basename(),"ECCCMNU_ListRegister                                    Dump name of ECC_CMNU's registers.");
        } else {
            re_printf("warning","The name (%s) of ECCCMNU_Help argument is wrong (commands or parameters).\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECCCMNU_Help argument is wrong (commands or parameters).\n", type.c_str());
    }
}//}}}

///*********************************
/// Function     : UseFifo
/// Description  : Control using FIFO for address latch
///                Called by PythonIF
///*********************************
void ECC_CMNU::UseFifo (const std::string is_use)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_use);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            if (!mIsUseFifo) {
                for (unsigned int i = 0; i < emBankNo; i++) {
                    mReqId[i] = 0;
                    mRespId[i] = 0;
                }
            }
            mIsUseFifo = true;
        } else if (word_vector[0] == "false") {
            mIsUseFifo = false;
        } else {
            re_printf("warning","The ECCCMNU_FIFO_USE parameter needs an argument [true/false].\n");
        }
    } else if (word_vector.size() == 0) {
        if (mIsUseFifo) {
            re_printf("info","true\n");
        } else {
            re_printf("info","false\n");
        }
    } else {
        re_printf("warning","The ECCCMNU_FIFO_USE parameter needs an argument [true/false].\n");
    }
}//}}}

///*********************************
/// Function     : UseApbEcc
/// Description  : Control using GAPB ECC Unit
///                Called by PythonIF
///*********************************
void ECC_CMNU::UseApbEcc (const std::string is_use)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_use);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            if (mPCLKFreq != mSYS_CLKFreq) {
                re_printf("warning","The pclk frequency and sys_clk frequency are different.\n");
            }
            mIsUseApbEcc = true;
        } else if (word_vector[0] == "false") {
            mIsUseApbEcc = false;
        } else {
            re_printf("warning","The ECCCMNU_APB_ECC_USE parameter needs an argument [true/false].\n");
        }
    } else if (word_vector.size() == 0) {
        if (mIsUseApbEcc) {
            re_printf("info","true\n");
        } else {
            re_printf("info","false\n");
        }
    } else {
        re_printf("warning","The ECCCMNU_APB_ECC_USE parameter needs an argument [true/false].\n");
    }
}//}}}

///*********************************
/// Function     : IssueError
/// Description  : Issue an error
///                Called by PythonIF
///*********************************
void ECC_CMNU::IssueError (const unsigned int bank_no, const unsigned int error_type)
{//{{{
    if ((!mIsResetCmdActive[0])&&(!mIsResetCmdActive[1])&&(!mIsResetHardActive[0])&&(!mIsResetHardActive[1])) {
        if (CheckClockPeriod("sys_clk")) {
            if (IsValidErrorBank(bank_no) && IsValidErrorType(error_type)) {//Check whether the arguments are valid
                mErrorInfo[bank_no].is_ready = true;
                std::string err_type_str = "";
                switch (error_type) {
                    case emAddrECC2BitError:
                        mErrorInfo[bank_no].addr_ecc_2bit_error_status = true;
                        err_type_str = "address ECC 2-bit error";
                        break;
                    case emAddrECC1BitError:
                        mErrorInfo[bank_no].addr_ecc_1bit_error_status = true;
                        err_type_str = "address ECC 1-bit error";
                        break;
                    case emDataECC2BitError:
                        mErrorInfo[bank_no].data_ecc_2bit_error_status = true;
                        err_type_str = "data ECC 2-bit error";
                        break;
                    case emDataECC1BitError:
                        mErrorInfo[bank_no].data_ecc_1bit_error_status = true;
                        err_type_str = "data ECC 1-bit error";
                        break;
                    case emAddrParityErrorRead:
                        mErrorInfo[bank_no].addr_parity_error_read_status = true;
                        err_type_str = "address parity error - read";
                        break;
                    case emAddrParityErrorWrite:
                        mErrorInfo[bank_no].addr_parity_error_write_status = true;
                        err_type_str = "address parity error - write";
                        break;
                    case emParityBitError:
                        mErrorInfo[bank_no].parity_bit_error_status = true;
                        err_type_str = "parity bit error";
                        break;
                    case emDataECC2BitErrorExtension:
                        mErrorInfo[bank_no].data_ecc_2bit_error_ext_status = true;
                        err_type_str = "data ECC 2-bit error extension";
                        break;
                    default: //case emDataECC1BitErrorExtension:
                        mErrorInfo[bank_no].data_ecc_1bit_error_ext_status = true;
                        err_type_str = "data ECC 1-bit error extension";
                        break;
                }
                re_printf("info","Received an error (%s) from Python IF (bank %d)\n", err_type_str.c_str(),bank_no);
                if (mIsUseFifo) {
                    mErrorInfo[bank_no].error_addr = mErrAddrFFIn[bank_no][mRespId[bank_no]];
                } else {
                    mErrorInfo[bank_no].error_addr = mErrAddrIn[bank_no];
                }
                mErrInputEvent[bank_no].notify(SC_ZERO_TIME);
            } else {
                re_printf("warning","The argument (bank_no or error_type) of the ECCCMNU_IssueError command is invalid.\n");
            }
        } else {
            re_printf("warning","Cannot capture an error when sys_clk is invalid.\n");
        }
    } else {
        re_printf("warning","Cannot issue an error during reset operation.\n");
    }
}//}}}

///*********************************
/// Function     : SetErrorAddr
/// Description  : Set an error address
///                Called by PythonIF
///*********************************
void ECC_CMNU::SetErrorAddr (const unsigned int bank_no, const unsigned int addr_value)
{//{{{
    if ((!mIsResetCmdActive[0])&&(!mIsResetCmdActive[1])&&(!mIsResetHardActive[0])&&(!mIsResetHardActive[1])) {
        if (CheckClockPeriod("sys_clk")) {
            if (IsValidErrorBank(bank_no)) {
                re_printf("info","Received an error address (0x%X) from Python IF (bank %d)\n", addr_value, bank_no);
                if (mIsUseFifo) {
                    mErrAddrFFIn[bank_no][mReqId[bank_no]] = addr_value;
                } else {
                    mErrAddrIn[bank_no] = addr_value;
                }
                mErrAddrInputEvent[bank_no].notify(SC_ZERO_TIME);
            } else {
                re_printf("warning","The argument (bank_no) of the ECCCMNU_SetErrorAddr command is invalid.\n");
            }
        } else {
            re_printf("warning","Cannot capture an error address when sys_clk is invalid.\n");
        }
    } else {
        re_printf("warning","Cannot set an error address during reset operation.\n");
    }
}//}}}

///*********************************
/// Function     : CheckFreq
/// Description  : Check frequency value and frequency unit
///*********************************
void ECC_CMNU::CheckFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
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
    } else {//Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong; frequency unit (Hz) is set as default.\n", unit_in.c_str());
    }
}//}}}

///*********************************
/// Function     : GetTimeResolution
/// Description  : Get time resolution
///*********************************
void ECC_CMNU::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
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

///*********************************
/// Function     : SeparateString
/// Description  : Separate some words from a string to store a vector
///*********************************
void ECC_CMNU::SeparateString (std::vector<std::string> &vtr, const std::string msg)
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

///*********************************
/// Function     : CalculateCLKPosEdge
/// Description  : Calculate synchronous time with pclk or sys_clk
///*********************************
double ECC_CMNU::CalculateCLKPosEdge (const std::string clock_name, const bool add_period, const double time_point)
{//{{{
    //calcualte the time point of next pos edge of PCLK
    double period_num = 0;
    if (clock_name == "pclk") {
        period_num = time_point / (double)mPCLKPeriod;
    } else {//sys_clk
        period_num = time_point / (double)mSYS_CLKPeriod;
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
    if (clock_name == "pclk") {
        pos_edge_point = (double)mod_period_num * (double)mPCLKPeriod - time_point;
    } else {//sys_clk
        pos_edge_point = (double)mod_period_num * (double)mSYS_CLKPeriod - time_point;
    }
    return pos_edge_point;
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void ECC_CMNU::EnableReset (const bool is_active, const unsigned int reset_id)
{//{{{
    if (reset_id == 0) {
        mECCCMNUFunc->EnableReset(is_active, "pclk_domain");
    } else {
        mECCCMNUFunc->EnableReset(is_active, "sys_clk_domain");
    }
    if (is_active) {
        for (unsigned int i = 0; i < emMaxSignalID; i++) {
            mRisingErrOutEvent[i].cancel();
            mFallingErrOutEvent[i].cancel();
            mErrorOutputWriteEvent[i].cancel(); 
        }
        for (unsigned int i = 0; i < emBankNo; i++) {
            mErrInputEvent[i].cancel();
            mErrAddrInputEvent[i].cancel();
        }
        mECCDisableSignalsControlEvent.cancel();
        this->Initialize();
        InitializeOutputSignals();
    } else {
        ControlECCDisableSignals();
    }
}//}}}

///*********************************
/// Function     : HandlePCLKSignalMethod
/// Description  : Handle pclk signal
///*********************************
void ECC_CMNU::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleSYS_CLKSignalMethod
/// Description  : Handle sys_clk signal
///*********************************
void ECC_CMNU::HandleSYS_CLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = sys_clk.read();
    SetCLKFreq("sys_clk", freq_value, "Hz");
}//}}}

///*********************************
/// Function     : HandleResetCmdMethod
/// Description  : Process reset function when presetz or sys_reset command is active
///*********************************
void ECC_CMNU::HandleResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetz";
    if (reset_id == 0) {
        reset_name = "presetz";
    } else { //reset_id == 1
        reset_name = "sys_reset";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

///*********************************
/// Function     : CancelResetCmdMethod
/// Description  : Cancel reset function when presetz or sys_reset command is active
///*********************************
void ECC_CMNU::CancelResetCmdMethod (const unsigned int reset_id)
{//{{{
    std::string reset_name = "presetz";
    if (reset_id == 0) {
        reset_name = "presetz";
    } else { //reset_id == 1
        reset_name = "sys_reset";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(mIsResetCmdActive[reset_id], reset_id); 
}//}}}

///*********************************
/// Function     : HandlePRESETZSignalMethod
/// Description  : Handle presetz signal
///*********************************
void ECC_CMNU::HandlePRESETZSignalMethod (void)
{//{{{
    mResetCurVal[0] = presetz.read();
    if (mResetCurVal[0] != mResetPreVal[0]) {//mResetPreVal is "true" at initial state
        mResetPreVal[0] = mResetCurVal[0];
        if (mPCLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[0].notify(CalculateCLKPosEdge("pclk", false, current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[0].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///*********************************
/// Function     : HandleSYS_RESETZSignalMethod
/// Description  : Handle sys_resetz signal
///*********************************
void ECC_CMNU::HandleSYS_RESETZSignalMethod (void)
{//{{{
    mResetCurVal[1] = sys_resetz.read();
    if (mResetCurVal[1] != mResetPreVal[1]) {//mResetPreVal is "true" at initial state
        mResetPreVal[1] = mResetCurVal[1];
        if (mSYS_CLKPeriod > 0) {
            double current_time = sc_time_stamp().to_double();
            mResetHardEvent[1].notify(CalculateCLKPosEdge("sys_clk", false, current_time), mTimeResolutionUnit);
        } else {
            mResetHardEvent[1].notify(SC_ZERO_TIME);
        }
    }
}//}}}

///*********************************
/// Function     : HandleResetHardMethod
/// Description  : Process reset function when presetz or sys_reset is active
///*********************************
void ECC_CMNU::HandleResetHardMethod (const unsigned int reset_id)
{//{{{
    if (mResetCurVal[reset_id] == emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == 0) {
            re_printf("info","The reset signal presetz is asserted.\n");
        } else {
            re_printf("info","The reset signal sys_resetz is asserted.\n");
        }
        this->EnableReset(mIsResetHardActive[reset_id], reset_id);
    } else {
        if (mIsResetHardActive[reset_id]) {
            mIsResetHardActive[reset_id] = false;
            this->EnableReset(mIsResetHardActive[reset_id], reset_id);
            if (reset_id == 0) {
                re_printf("info","The reset signal presetz is de-asserted.\n");
            } else {
                re_printf("info","The reset signal sys_resetz is de-asserted.\n");
            }
        }
    }
    //Cancel AssertReset command when presetz is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

///*********************************
/// Function     : WriteECCDisableSignalsMethod
/// Description  : Issue ECC CTL signals
///*********************************
void ECC_CMNU::WriteECCDisableSignalsMethod (void)
{//{{{
    unsigned int eccctl_value = mECCCTLOut;
    if (mECCCMNUVersion == "G3M_V30") {
        aeccdisable.write((bool)((eccctl_value>>2)&0x1));
        asecdisable.write((bool)((eccctl_value>>3)&0x1));
    }
    if (mECCCMNUVersion != "ICDM_G3MH" && mECCCMNUVersion != "ICTM_G3MH") {
        secdisable.write((bool)((eccctl_value>>1)&0x1));
    }
    eccdisable.write((bool)(eccctl_value&0x1));
}//}}}

///*********************************
/// Function     : CaptureErrInputMethod
/// Description  : Capture error input
///*********************************
void ECC_CMNU::CaptureErrInputMethod (const unsigned int bank_no)
{//{{{
    if (mErrorInfo[bank_no].is_ready) { //If there is an detected error
        if (mIsUseFifo) {
            if (mRespId[bank_no] >= 3) {
                mRespId[bank_no] = 0;
            } else {
                mRespId[bank_no] = mRespId[bank_no] + 1;
            }
        }
        mECCCMNUFunc->ProcessErrorInfo(mErrorInfo[bank_no],bank_no);
        //Clear error information
        mErrorInfo[bank_no].Initialize();
    }
}//}}}

///*********************************
/// Function     : CaptureErrAddrInputMethod
/// Description  : Capture error address input
///*********************************
void ECC_CMNU::CaptureErrAddrInputMethod (const unsigned int bank_no)
{//{{{
    if (mIsUseFifo) {
        if (mReqId[bank_no] >= 3) {
            mReqId[bank_no] = 0;
        } else {
            mReqId[bank_no] = mReqId[bank_no] + 1;
        }
    }
}//}}}

///*********************************
/// Function     : RisingErrOutSignalsMethod
/// Description  : Activate error output signals
///*********************************
void ECC_CMNU::RisingErrOutSignalsMethod (const unsigned int signal_id)
{//{{{
    double current_time = sc_time_stamp().to_double();
    double sys_clk_syn_period = CalculateCLKPosEdge("sys_clk", true, current_time);
    double pclk_syn_period = CalculateCLKPosEdge("pclk", true, current_time + sys_clk_syn_period);
    double falling_period = 0;
    mErrOut[signal_id] = true;
    mErrorOutputWriteEvent[signal_id].notify(SC_ZERO_TIME);
    if (mIsUseApbEcc) {
        falling_period = mPCLKPeriod;
    } else {
        falling_period = sys_clk_syn_period + pclk_syn_period;
    }
    mFallingErrOutEvent[signal_id].notify(falling_period, mTimeResolutionUnit);
}//}}}

///*********************************
/// Function     : FallingErrOutSignalsMethod
/// Description  : Deactivate error output signals
///*********************************
void ECC_CMNU::FallingErrOutSignalsMethod (const unsigned int signal_id)
{//{{{
    mErrOut[signal_id] = false;
    mErrorOutputWriteEvent[signal_id].notify(SC_ZERO_TIME);
}//}}}

///*********************************
/// Function     : WriteErrOutSignalsMethod
/// Description  : Write value to error output signals
///*********************************
void ECC_CMNU::WriteErrOutSignalsMethod (const unsigned int signal_id)
{//{{{
    switch (signal_id) {
        case emADEDERROUT:
            if (mECCCMNUVersion == "G3M_V30") {
                adederrout.write(mErrOut[signal_id]);
            }
            break;
        case emASEDERROUT:
            if (mECCCMNUVersion == "G3M_V30") {
                asederrout.write(mErrOut[signal_id]);
            }
            break;
        case emDEDERROUT:
            dederrout.write(mErrOut[signal_id]);
            break;
        case emSEDERROUT:
            sederrout.write(mErrOut[signal_id]);
            break;
        case emAPERROUT:
            if (mECCCMNUVersion == "G3M_V30" || mECCCMNUVersion == "CFB_G3MH" ||
                mECCCMNUVersion == "CFB_X2X" || mECCCMNUVersion == "GRAM_G3MH" ||
                mECCCMNUVersion == "TCM_G3MH") {
                aperrout.write(mErrOut[signal_id]);
            }
            break;
        case emBPERROUT:
            if (mECCCMNUVersion == "G3M_V30" || mECCCMNUVersion == "GRAM_G3MH" ||
                mECCCMNUVersion == "TCM_G3MH") {
                bperrout.write(mErrOut[signal_id]);
            }
            break;
        default: //case emERROVFOUT:
            if (mECCCMNUVersion == "G3M_V30") {
                errovfout.write(mErrOut[signal_id]);
            }
            break;
    }
}//}}}

///*********************************
/// Function     : IsValidErrorBank
/// Description  : Check whether error bank is valid or not
///*********************************
bool ECC_CMNU::IsValidErrorBank (const unsigned int bank_no)
{//{{{
    if ((mECCCMNUVersion == "G3M_V30") && (bank_no < emBankNo)) {
        return true;
    }
    if ((mECCCMNUVersion == "TCM_G3KH" || mECCCMNUVersion == "CFB_G3KH" ||
         mECCCMNUVersion == "CFB_X2X" || mECCCMNUVersion == "ICDM_G3MH" ||
         mECCCMNUVersion == "ICTM_G3MH") && (bank_no < emBank1)) {
        return true;
    }
    if ((mECCCMNUVersion == "TCM_G3MH" || mECCCMNUVersion == "CFB_G3MH" ||
         mECCCMNUVersion == "GRAM_G3MH") && (bank_no < emBank2)) {
        return true;
    }
    return false;
}//}}}

///*********************************
/// Function     : IsValidErrorType
/// Description  : Check whether error type is valid or not
///*********************************
bool ECC_CMNU::IsValidErrorType (const unsigned int error_type)
{//{{{
    if ((mECCCMNUVersion == "G3M_V30") && (error_type < emErrorTypeNum)) {
        return true;
    }
    if ((mECCCMNUVersion == "TCM_G3KH" || mECCCMNUVersion == "CFB_G3KH" ||
         mECCCMNUVersion == "ICDM_G3MH" || mECCCMNUVersion == "ICTM_G3MH") &&
        (error_type == emDataECC2BitError || error_type == emDataECC1BitError)) {
        return true;
    }
    if ((mECCCMNUVersion == "CFB_G3MH" || mECCCMNUVersion == "CFB_X2X") &&
        (error_type == emDataECC2BitError || error_type == emDataECC1BitError ||
         error_type == emAddrParityErrorRead)) {
        return true;
    }
    if ((mECCCMNUVersion == "GRAM_G3MH" || mECCCMNUVersion == "TCM_G3MH") &&
        (error_type == emDataECC2BitError || error_type == emDataECC1BitError ||
         error_type == emAddrParityErrorRead || error_type == emParityBitError)) {
        return true;
    }
    return false;
}//}}}

///*********************************
/// Function     : ControlECCDisableSignals
/// Description  : Wait rising edge of PCLK to control ECC CTL signals
///                Virtual function of ECC_CMNU_AgentController class
///*********************************
void ECC_CMNU::ControlECCDisableSignals (void)
{//{{{
    if (CheckClockPeriod("pclk")) {
        double current_time = sc_time_stamp().to_double();
        mECCCTLOut = mECCCMNUFunc->GetECCCTLVal();
        mECCDisableSignalsControlEvent.notify(CalculateCLKPosEdge("pclk", false, current_time), mTimeResolutionUnit);
    }
}//}}}

///*********************************
/// Function     : CheckClockPeriod
/// Description  : Check clock period of pclk, sys_clk
///                Virtual function of ECC_CMNU_AgentController class
///*********************************
bool ECC_CMNU::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool status = false;
    if (clock_name == "pclk") {
        if (mPCLKPeriod > 0) {
            status = true;
        }
    } else {//sys_clk
        if (mSYS_CLKPeriod > 0) {
            status = true;
        }
    }
    if (!status) {
        re_printf("info","%s frequency is zero.\n", clock_name.c_str());
    }
    return status;
}//}}}

///*********************************
/// Function     : ControlErrorOutputSignals
/// Description  : Control error output signals
///                Virtual function of ECC_CMNU_AgentController class
///*********************************
void ECC_CMNU::ControlErrorOutputSignals (const std::string signal_name)
{//{{{
    double current_time = sc_time_stamp().to_double();
    double sys_clk_syn_period = CalculateCLKPosEdge("sys_clk", false, current_time);
    double pclk_syn_period = CalculateCLKPosEdge("pclk", true, current_time + sys_clk_syn_period);
    double rising_period = 0;
    if (mIsUseApbEcc) {
        rising_period = sys_clk_syn_period + mPCLKPeriod;
    } else {
        rising_period = sys_clk_syn_period + pclk_syn_period + mPCLKPeriod;
    }
    if (signal_name == "adederrout") {
        mRisingErrOutEvent[emADEDERROUT].notify(rising_period, mTimeResolutionUnit);
    } else if (signal_name == "asederrout") {
        mRisingErrOutEvent[emASEDERROUT].notify(rising_period, mTimeResolutionUnit);
    } else if (signal_name == "dederrout") {
        mRisingErrOutEvent[emDEDERROUT].notify(rising_period, mTimeResolutionUnit);
    } else if (signal_name == "sederrout") {
        mRisingErrOutEvent[emSEDERROUT].notify(rising_period, mTimeResolutionUnit);
    } else if (signal_name == "aperrout") {
        mRisingErrOutEvent[emAPERROUT].notify(rising_period, mTimeResolutionUnit);
    } else if (signal_name == "bperrout") {
        mRisingErrOutEvent[emBPERROUT].notify(rising_period, mTimeResolutionUnit);
    } else { //signal_name == "errovfout"
        mRisingErrOutEvent[emERROVFOUT].notify(rising_period, mTimeResolutionUnit);
    }
}//}}}
// vim600: set foldmethod=marker :
