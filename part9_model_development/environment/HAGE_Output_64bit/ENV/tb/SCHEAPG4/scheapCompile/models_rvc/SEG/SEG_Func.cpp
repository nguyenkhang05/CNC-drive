// ---------------------------------------------------------------------
// $Id: SEG_Func.cpp,v 1.11 2015/09/16 06:22:07 dungvannguyen Exp $
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
#include <cstdarg>
#include "SEG_Func.h"
#include "SEG_AgentController.h"
#include "SysErrVCI.h"
#include "SysErrTCM.h"
#include "SysErrIPG.h"

///Constructor of SEG_Func class
SEG_Func::SEG_Func (std::string name,
                  SEG_AgentController *SEG_AgentControllerPtr,
                  std::string SEG_VERSION):Cseg_regif(name, 32, SEG_VERSION), BusSlaveFuncIf()
{//{{{
    assert(SEG_AgentControllerPtr != NULL);
    mSEG_AgentController = SEG_AgentControllerPtr;
    mSEG_AgentController->CommandInit(name);
    Cseg_regif::set_instance_name(name);
    mModuleName = name;
    mVersion = SEG_VERSION;
    if ((mVersion == "G3M_V20")||(mVersion == "G3M_V30") || (mVersion == "G3MH") || (mVersion == "G3KH")) {
        mSEGCONTInitValue = 0x0;
        SetSEGCONTInitValue(mSEGCONTInitValue);
    } else {
        mSEGCONTInitValue = 0xCF74;
        SetSEGCONTInitValue(mSEGCONTInitValue);
    }
    mG3MHVciError = false;
    mG3MHTcmError = false;
    mG3MHIpgError = false;
    mNoticeVciErr = false;
    mNoticeTcmErr = false;
    mPreviousSEGFLAGValue = 0x0;
    mIsSignal = true;
    mIsStartedSimulation = false;
    mVciErrInfoValue = new Csyserrvci();
    mTcmErrInfoValue = new Csyserrtcm();
    mIpgErrInfoValue = new Csyserripg();
    mVciErrInfoValue->Initialize();
    mTcmErrInfoValue->Initialize();
    mIpgErrInfoValue->Initialize();
}//}}}

///Destructor of SEG_Func class
SEG_Func::~SEG_Func (void)
{//{{{
    delete mVciErrInfoValue;
    delete mTcmErrInfoValue;
    delete mIpgErrInfoValue;
}//}}}

///Update SEGCONT register value at initial state
void SEG_Func::StartOfSimulation (void)
{//{{{
    mIsStartedSimulation = true;
}//}}}

///Reset operation of SEG_Func class
void SEG_Func::Reset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "presetz") {
        Cseg_regif::EnableReset(is_active);
    } else {//if reset_name is "reset"
        mNoticeVciErr = false;
        mNoticeTcmErr = false;
        mPreviousSEGFLAGValue = 0x0;
        mG3MHVciError = false;
        mG3MHTcmError = false;
        mG3MHIpgError = false;
        mIsSignal = true;
        mVciErrInfoValue->Initialize();
        mTcmErrInfoValue->Initialize();
        mIpgErrInfoValue->Initialize();
    }
}//}}}

///Dump information of registers SEGFLAG, SEGTYPE, SEGSIDE, SEGADDR
void SEG_Func::DumpStatusInfo (void)
{//{{{
    mSEG_AgentController->print_msg("info","SEGFLAG : 0x%X\n", (unsigned int)(*SEGFLAG));
    mSEG_AgentController->print_msg("info","SEGTYPE : 0x%X\n", (unsigned int)(*SEGTYPE));
    mSEG_AgentController->print_msg("info","SEGSIDE : 0x%X\n", (unsigned int)(*SEGSIDE));
    mSEG_AgentController->print_msg("info","SEGADDR : 0x%X\n", (unsigned int)(*SEGADDR));
}//}}}

///Register handler command to seg_regif class
void SEG_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Cseg_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cseg_regif class
            msg_return = Cseg_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Set initial value to register SEGCONT
void SEG_Func::SetSEGCONTInitValue (const unsigned int init_val)
{//{{{
    mSEGCONTInitValue = init_val;//Update mSEGCONTInitValue when users call SEG_VLD_INI command
    if (mVersion != "G3KH") {
        if (mVersion != "G3MH") {
            SEGCONT->UpdateBitInitValue("NEE", init_val&0x1);
        }
        SEGCONT->UpdateBitInitValue("ICCE", (init_val>>2)&0x1);
    }
    if (mVersion != "G3MH") {
        SEGCONT->UpdateBitInitValue("VCIE", (init_val>>4)&0x1);
    } else {
        SEGCONT->UpdateBitInitValue("LSUE", (init_val>>4)&0x1);
    }
    if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
        SEGCONT->UpdateBitInitValue("ROME", (init_val>>5)&0x1);
    } else if (mVersion == "G3MH") {
        SEGCONT->UpdateBitInitValue("CFBE", (init_val>>5)&0x1);
    }
    if (mVersion != "G3MH") {
        SEGCONT->UpdateBitInitValue("TCME", (init_val>>6)&0x1);
    } else {
        SEGCONT->UpdateBitInitValue("LRME", (init_val>>6)&0x1);
    }
    if (mVersion == "G3MH") {
        SEGCONT->UpdateBitInitValue("GRME", (init_val>>7)&0x1);
    }
    if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
        SEGCONT->UpdateBitInitValue("VCRE", (init_val>>8)&0x1);
        SEGCONT->UpdateBitInitValue("VPGE", (init_val>>9)&0x1);
        SEGCONT->UpdateBitInitValue("VCBE", (init_val>>10)&0x1);
    } else if (mVersion == "G3MH") {
        SEGCONT->UpdateBitInitValue("HBPE", (init_val>>8)&0x1);
        SEGCONT->UpdateBitInitValue("PBPE", (init_val>>9)&0x1);
    } else {//G3KH
        SEGCONT->UpdateBitInitValue("IPGE", (init_val>>8)&0x1);
        SEGCONT->UpdateBitInitValue("APIE", (init_val>>9)&0x1);
        SEGCONT->UpdateBitInitValue("VCSE", (init_val>>10)&0x1);
    }
    if ((mVersion != "G3KH") && (mVersion != "G3M_V30")) {
        if (mVersion != "G3MH") {
            SEGCONT->UpdateBitInitValue("AXCE", (init_val>>11)&0x1);
        }
        SEGCONT->UpdateBitInitValue("SS0E", (init_val>>14)&0x1);
        SEGCONT->UpdateBitInitValue("SS1E", (init_val>>15)&0x1);
    }
    if (!mIsStartedSimulation) {
        SEGCONT->write(mSEGCONTInitValue);
    }
}//}}}

///Update SEGFLAG register to process errors
void SEG_Func::UpdateSEGFLAGRegister (const bool is_signal, const std::string error_type)
{//{{{
    if (!(mSEG_AgentController->GetResetStatus("presetz"))) {
        if (error_type == "vcierror_both") {
            mNoticeVciErr = false;
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                (*SEGFLAG)["VCIF"] = 1;
            } else if (mVersion == "G3MH") {
                mG3MHVciError = true;
                (*SEGFLAG)["LSUF"] = 1;
            }
        } else if (error_type == "vcierror_only") {
            mNoticeVciErr = true;
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                (*SEGFLAG)["VCIF"] = 1;
            } else if (mVersion == "G3MH") {
                mG3MHVciError = true;
                (*SEGFLAG)["LSUF"] = 1;
            }
        } else if (error_type == "tcmerror_both") {
            mNoticeTcmErr = false;
            if (mVersion != "G3MH") {
                (*SEGFLAG)["TCMF"] = 1;
            } else {
                mG3MHTcmError = true;
                (*SEGFLAG)["LSUF"] = 1;
            }
        } else if (error_type == "tcmerror_only") {
            mNoticeTcmErr = true;
            if (mVersion != "G3MH") {
                (*SEGFLAG)["TCMF"] = 1;
            } else {
                mG3MHTcmError = true;
                (*SEGFLAG)["LSUF"] = 1;
            }
        } else {//ipg error
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                (*SEGFLAG)["VCRF"] = 1;
            } else if (mVersion == "G3MH") {
                mG3MHIpgError = true;
                (*SEGFLAG)["LSUF"] = 1;
            } else {//G3KH
                (*SEGFLAG)["IPGF"] = 1;
            }
        }
    }
    mIsSignal = is_signal;
    mSEG_AgentController->ArbitrateErrors();
}//}}}

///Retain error information
void SEG_Func::RetainErrInfo (void)
{//{{{
    if (mIsSignal) {//Only retain error info when error occur by input signal
        if ((mPreviousSEGFLAGValue == 0)||(*SEGCONT == 0)) {//If first error or notification is disable
            mPreviousSEGFLAGValue = *SEGFLAG;
            UpdateErrorInfo();
        }
    }
    ProcessNotifyingError(true);
}//}}}

///Exchange error info
void SEG_Func::ExchangeErrorInfo (const Csyserrvci vcierrinfo, const Csyserrtcm tcmerrinfo, const Csyserripg ipgerrinfo)
{//{{{
    *mVciErrInfoValue = vcierrinfo;
    *mTcmErrInfoValue = tcmerrinfo;
    *mIpgErrInfoValue = ipgerrinfo;
}//}}}

///Virtual function of BusSlaveFuncIf class
void SEG_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void SEG_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    bool accessibility_flag = false;
    std::string mode_name = "HV";
    if (mVersion != "G3KH") {
        accessibility_flag = ((vm == false) && (um == false))?true:false;
    } else {
        mode_name = "SV";
        accessibility_flag = (um == false)?true:false;
    }
    if (accessibility_flag) {
        if (!debug) {
            reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    } else {
        mSEG_AgentController->print_msg("error","Can not write the value to the model's register. Write access is only possible with %s authority.\n",mode_name.c_str());
    }
}//}}}

///Process errors in SEGFLAG register
void SEG_Func::ProcessNotifyingError (const bool is_err_request_output_signal)
{//{{{
    unsigned int flag_value = *SEGFLAG;
    unsigned int control_value = (*SEGCONT)&0xFFFE;//Mask NEE bit
    if (flag_value == 0) {
        mSEG_AgentController->NotifyErrorOutput(0, false);
    } else {
        for (unsigned char index = 0; index < 16; index++) {//Check each bit in register SEGFLAG
            if (((flag_value>>index)&0x1) == 0x1) {//System error occurs
                if (((control_value>>index)&0x1) == 0x1) {//Enable notification with highest priority
                    if (mVersion != "G3MH") {
                        if (index == 4) {//VCI system error
                            if (mNoticeVciErr) {
                                if (mVersion != "G3KH") {
                                    if ((*SEGCONT)["NEE"] == 1) {//Notice error
                                        mSEG_AgentController->NotifyErrorOutput(index, is_err_request_output_signal);
                                    }
                                }
                            } else {
                                mSEG_AgentController->NotifyErrorOutput(index, is_err_request_output_signal);
                            }
                        } else if (index == 6) {//TCM system error
                            if (mNoticeTcmErr) {
                                if (mVersion != "G3KH") {
                                    if ((*SEGCONT)["NEE"] == 1) {//Notice error
                                        mSEG_AgentController->NotifyErrorOutput(index, is_err_request_output_signal);
                                    }
                                }
                            } else {
                                mSEG_AgentController->NotifyErrorOutput(index, is_err_request_output_signal);
                            }
                        } else {
                            mSEG_AgentController->NotifyErrorOutput(index, is_err_request_output_signal);
                        }
                    } else {//G3MH
                        if ((index == 4) && (!mNoticeTcmErr) && (!mNoticeVciErr)) {
                            mSEG_AgentController->NotifyErrorOutput(index, is_err_request_output_signal);
                        }
                    }
                    break;
                }
            }
        }
    }
}//}}}

///Retain error info in registers SEGTYPE, SEGSIDE, SEGADDR
void SEG_Func::UpdateErrorInfo (void)
{//{{{
    if (!(mSEG_AgentController->GetResetStatus("presetz"))) {
        if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
            if ((*SEGFLAG)["VCIF"] == 1) {//VCI error
                (*SEGTYPE)["ETYP"] = (mVciErrInfoValue->SysErrVCIInfo>>24)&0x7F;//SysErrVCIInfo[30:24]
                (*SEGTYPE)["RTYP"] = (mVciErrInfoValue->SysErrVCIInfo>>16)&0x1F;//SysErrVCIInfo[20:16]
                (*SEGTYPE)["CODE"] = 0x4;
                (*SEGSIDE) = mVciErrInfoValue->SysErrVCIInfo&0xFFFF;//SysErrVCIInfo[15:0]
                (*SEGADDR) = mVciErrInfoValue->SysErrVCIAdr;
            } else {
                if ((*SEGFLAG)["TCMF"] == 1) {//TCM error
                    (*SEGTYPE)["ETYP"] = (mTcmErrInfoValue->SysErrTCMInfo>>24)&0x7F;//SysErrTCMInfo[30:24]
                    (*SEGTYPE)["RTYP"] = (mTcmErrInfoValue->SysErrTCMInfo>>16)&0x1F;//SysErrTCMInfo[20:16]
                    (*SEGTYPE)["CODE"] = 0x6;
                    (*SEGSIDE) = mTcmErrInfoValue->SysErrTCMInfo&0xFFFF;//SysErrTCMInfo[15:0]
                    (*SEGADDR) = mTcmErrInfoValue->SysErrTCMAdr;
                } else {//IPG error
                    (*SEGTYPE)["ETYP"] = 0x0;
                    (*SEGTYPE)["RTYP"] = 0x0;
                    (*SEGTYPE)["CODE"] = 0x8;
                    (*SEGSIDE)["VM"] = mIpgErrInfoValue->SysErrIPGVM;
                    (*SEGSIDE)["UM"] = mIpgErrInfoValue->SysErrIPGUM;
                    (*SEGSIDE)["SPID"] = 0x0;
                    (*SEGSIDE)["VCID"] = mIpgErrInfoValue->SysErrIPGVMId;
                    (*SEGSIDE)["TCID"] = 0x0;
                    (*SEGADDR) = 0x0;
                }
            }
        } else if (mVersion == "G3MH") {
            if ((*SEGFLAG)["LSUF"] == 1) {//VCI, TCM, IPG error
                if (mG3MHVciError) {
                    (*SEGTYPE) = 0x0;
                    (*SEGSIDE) = mVciErrInfoValue->SysErrVCIInfo&0xE07F;//SysErrVCIInfo[15:0]
                    (*SEGADDR) = mVciErrInfoValue->SysErrVCIAdr;
                } else if (mG3MHTcmError) {
                    (*SEGTYPE) = 0x0;
                    (*SEGSIDE) = mTcmErrInfoValue->SysErrTCMInfo&0xE07F;//SysErrTCMInfo[15:0]
                    (*SEGADDR) = mTcmErrInfoValue->SysErrTCMAdr;
                } else {//mG3MHIpgError is true
                    (*SEGTYPE) = 0x0;
                    (*SEGSIDE)["VM"] = mIpgErrInfoValue->SysErrIPGVM;
                    (*SEGSIDE)["UM"] = mIpgErrInfoValue->SysErrIPGUM;
                    (*SEGSIDE)["SPID"] = 0x0;
                    (*SEGADDR) = 0x0;
                }
            }
        } else {//G3KH
            if ((*SEGFLAG)["TCMF"] == 1) {//TCM error
                (*SEGTYPE)["ETYPE"] = (mTcmErrInfoValue->SysErrTCMInfo>>24)&0x7F;//SysErrTCMInfo[30:24]
                (*SEGTYPE)["RTYPE"] = (mTcmErrInfoValue->SysErrTCMInfo>>16)&0x1F;//SysErrTCMInfo[20:16]
                (*SEGTYPE)["CODE"] = 0x6;
                (*SEGSIDE) = mTcmErrInfoValue->SysErrTCMInfo&0xFFFE;//SysErrTCMInfo[15:0]
                (*SEGADDR) = mTcmErrInfoValue->SysErrTCMAdr;
            } else {//IPG error
                (*SEGTYPE)["ETYPE"] = 0x0;
                (*SEGTYPE)["RTYPE"] = 0x0;
                (*SEGTYPE)["CODE"] = 0x8;
                (*SEGSIDE)["UM"] = 0x1;
                (*SEGSIDE)["SPID"] = 0x0;
                (*SEGSIDE)["VCID"] = 0x0;
                (*SEGSIDE)["TCID"] = 0x0;
                (*SEGADDR) = 0x0;
            }
        }
    }
}//}}}

///Virtual function of register SEGFLAG in seg_regif class
void SEG_Func::cb_SEGFLAG_VCIF (RegCBstr str)
{//{{{
    if (!(mSEG_AgentController->GetResetStatus("reset"))) {
        if ((mSEG_AgentController->CheckClockPeriod("pclk"))&&(mSEG_AgentController->CheckClockPeriod("clk"))) {
            mPreviousSEGFLAGValue = *SEGFLAG;
            ProcessNotifyingError(false);
        }
    }
}//}}}
// vim600: set foldmethod=marker :
