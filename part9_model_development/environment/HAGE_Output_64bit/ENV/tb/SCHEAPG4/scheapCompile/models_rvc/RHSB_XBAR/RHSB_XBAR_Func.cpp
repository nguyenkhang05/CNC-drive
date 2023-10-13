// ---------------------------------------------------------------------
// $Id: RHSB_XBAR_Func.cpp 1415 2019-11-29 09:08:35Z chuonghoangle $
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
#include <cstdarg>
#include "RHSB_XBAR_Func.h"
#include "global.h"
///*********************************
/// Function     : RHSB_XBAR
/// Description  : Constructor of RHSB_XBAR_Func class
///*********************************
RHSB_XBAR_Func::RHSB_XBAR_Func (std::string name, 
                       RHSB_XBAR_AgentController *XBARAgentPtr,
                        std::string user):
                       Crhsb_xbar_regif(name, 32),
                       BusSlaveFuncIf()
{//{{{ 
    assert(XBARAgentPtr != NULL);
    mModuleName = name;
    mXBARAgent = XBARAgentPtr;
    mXBARAgent->CommandInit(name);
    Crhsb_xbar_regif::set_instance_name(name);
    mUser = user;
}//}}} 

///*********************************
/// Function     : ~RHSB_XBAR_Func
/// Description  : Destructor of RHSB_XBAR_Func class
///*********************************
RHSB_XBAR_Func::~RHSB_XBAR_Func (void)
{//{{{
}//}}}

///*********************************
/// Function     : read
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void RHSB_XBAR_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // When clock = 0, or reset state, only debug master can read register 
    if ( (!mXBARAgent->CheckClockPeriod()) || (mXBARAgent->GetResetStatus()) ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : write
/// Description  : Virtual function of BusSlaveFuncIf class
///*********************************
void RHSB_XBAR_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{]
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (!mXBARAgent->CheckClockPeriod()){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }
    // write register if possible
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void RHSB_XBAR_Func::RegisterHandler (const std::vector<std::string>& args)
{//{{{ 
    std::string msg_return = "";
    if (args.size() == 1) {
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
                msg_return = Crhsb_xbar_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Crhsb_xbar_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///*********************************
/// Function     : EnableReset
/// Description  : Process reset function
///*********************************
void RHSB_XBAR_Func::EnableReset (const bool is_active)
{//{{{ 
    Crhsb_xbar_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : CheckSettingReg
/// Description  : Function to check setting of register
///*********************************
void RHSB_XBAR_Func::CheckSettingReg(void)
{//{{{
//new codeeeee
    unsigned int bit_pos = 0;
    std::ostringstream bit_name0;
    std::ostringstream bit_name1;
    std::ostringstream bit_name2;
    std::ostringstream bit_name3;

    for (bit_pos = 0; bit_pos < 16; bit_pos++) {
        bit_name0.str("");bit_name0 << "MMDO0" << bit_pos;
        bit_name1.str("");bit_name1 << "MMDO1" << bit_pos;
        bit_name2.str("");bit_name2 << "MMDO2" << bit_pos;
        bit_name3.str("");bit_name3 << "MMDO3" << bit_pos;
        unsigned int temp_channel_pos = bit_pos;
        if ((*RHSBGCRO0)[bit_name0.str().c_str()] == 0) {
            mXBARAgent->print_msg("info","GPIO is set as default");
            CheckRHSBGCROk(0, bit_pos);
        } else {
            if (mUser == "GTM") {
                if ((*RHSBGCRO0)[bit_name0.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","GTM is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforGTM(0, bit_pos, "CRO0");
                    } else {
                        CheckRHSBGCRkHforGTM(0, bit_pos, "CRO0");
                    }
                } else {
                    unsigned int cal_pos = 0 * 16 + bit_pos;
                    mXBARAgent->print_msg("warning","GTM: Setting prohibited. The output is low level");
                    mXBARAgent->SelectforOutput (cal_pos, 64, "Low");
                }
            } else {
                if ((*RHSBGCRO0)[bit_name0.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","ATU_C is selected");
                    if (temp_channel_pos < 8) {
                        CheckRHSBGCRkLforATU(0, bit_pos, "c", "CRO0");
                    } else {
                        CheckRHSBGCRkHforATU(0, bit_pos, "c", "CRO0");
                    }
                } else if ((*RHSBGCRO0)[bit_name0.str().c_str()] == 2) {
                    mXBARAgent->print_msg("info","ATU_D is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(0, bit_pos, "d", "CRO0");
                    } else {
                        CheckRHSBGCRkHforATU(0, bit_pos, "d", "CRO0");
                    }
                } else {
                    mXBARAgent->print_msg("info","ATU_E is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(0, bit_pos, "e", "CRO0");
                    } else {
                        CheckRHSBGCRkHforATU(0, bit_pos, "e", "CRO0");
                    }
                }
            }
        }

        if ((*RHSBGCRO1)[bit_name1.str().c_str()] == 0) {
            mXBARAgent->print_msg("info","GPIO is set as default");
            CheckRHSBGCROk(1, bit_pos);
        } else {
            if (mUser == "GTM") {
                if ((*RHSBGCRO1)[bit_name1.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","GTM is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforGTM(1, bit_pos, "CRO1");
                    } else {
                        CheckRHSBGCRkHforGTM(1, bit_pos, "CRO1");
                    }
                } else {
                    unsigned int cal_pos = 1 * 16 + bit_pos;
                    mXBARAgent->print_msg("warning","GTM: Setting prohibited. The output is low level");
                    mXBARAgent->SelectforOutput (cal_pos, 64, "Low");
                }
            } else {
                if ((*RHSBGCRO1)[bit_name1.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","ATU_C is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(1, bit_pos, "c", "CRO1");
                    } else {
                        CheckRHSBGCRkHforATU(1, bit_pos, "c", "CRO1");
                    }
                } else if ((*RHSBGCRO1)[bit_name1.str().c_str()] == 2) {
                    mXBARAgent->print_msg("info","ATU_D is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(1, bit_pos, "d", "CRO1");
                    } else {
                        CheckRHSBGCRkHforATU(1, bit_pos, "d", "CRO1");
                    }
                } else {
                    mXBARAgent->print_msg("info","ATU_E is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(1, bit_pos, "e", "CRO1");
                    } else {
                        CheckRHSBGCRkHforATU(1, bit_pos, "e", "CRO1");
                    }
                }
            }
        }

        if ((*RHSBGCRO2)[bit_name2.str().c_str()] == 0) {
            mXBARAgent->print_msg("info","GPIO is set as default");
            CheckRHSBGCROk(2, bit_pos);
        } else {
            if (mUser == "GTM") {
                if ((*RHSBGCRO2)[bit_name2.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","GTM is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforGTM(2, bit_pos, "CRO2");
                    } else {
                        CheckRHSBGCRkHforGTM(2, bit_pos, "CRO2");
                    }
                } else {
                    unsigned int cal_pos = 2 * 16 + bit_pos;
                    mXBARAgent->print_msg("warning","GTM: Setting prohibited. The output is low level");
                    mXBARAgent->SelectforOutput (cal_pos, 64, "Low");
                }
            } else {
                if ((*RHSBGCRO2)[bit_name2.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","ATU_C is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(2, bit_pos, "c", "CRO2");
                    } else {
                        CheckRHSBGCRkHforATU(2, bit_pos, "c", "CRO2");
                    }
                } else if ((*RHSBGCRO2)[bit_name2.str().c_str()] == 2) {
                    mXBARAgent->print_msg("info","ATU_D is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(2, bit_pos, "d", "CRO2");
                    } else {
                        CheckRHSBGCRkHforATU(2, bit_pos, "d", "CRO2");
                    }
                } else {
                    mXBARAgent->print_msg("info","ATU_E is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(2, bit_pos, "e", "CRO2");
                    } else {
                        CheckRHSBGCRkHforATU(2, bit_pos, "e", "CRO2");
                    }
                }
            }
        }

        if ((*RHSBGCRO3)[bit_name3.str().c_str()] == 0) {
            mXBARAgent->print_msg("info","GPIO is set as default");
            CheckRHSBGCROk(3, bit_pos);
        } else {
            if (mUser == "GTM") {
                if ((*RHSBGCRO3)[bit_name3.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","GTM is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforGTM(3, bit_pos, "CRO3");
                    } else {
                        CheckRHSBGCRkHforGTM(3, bit_pos, "CRO3");
                    }
                } else {
                    unsigned int cal_pos = 3 * 16 + bit_pos;
                    mXBARAgent->print_msg("warning","GTM: Setting prohibited. The output is low level");
                    mXBARAgent->SelectforOutput (cal_pos, 64, "Low");
                }
            } else {
                if ((*RHSBGCRO3)[bit_name3.str().c_str()] == 1) {
                    mXBARAgent->print_msg("info","ATU_C is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(3, bit_pos, "c", "CRO3");
                    } else {
                        CheckRHSBGCRkHforATU(3, bit_pos, "c", "CRO3");
                    }
                } else if ((*RHSBGCRO3)[bit_name3.str().c_str()] == 2) {
                    mXBARAgent->print_msg("info","ATU_D is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(3, bit_pos, "d", "CRO3");
                    } else {
                        CheckRHSBGCRkHforATU(3, bit_pos, "d", "CRO3");
                    }
                } else {
                    mXBARAgent->print_msg("info","ATU_E is selected");
                    if (bit_pos < 8) {
                        CheckRHSBGCRkLforATU(3, bit_pos, "e", "CRO3");
                    } else {
                        CheckRHSBGCRkHforATU(3, bit_pos, "e", "CRO3");
                    }
                }
            }
        }
    }
    mXBARAgent->WriteOutput();
//    }
}//}}}

///*********************************
/// Function     : CheckRHSBGCROk
/// Description  : Function to check value of RHSBGCROk register
///*********************************
void RHSB_XBAR_Func::CheckRHSBGCROk (const unsigned int sub_xbar, const unsigned int bit_pos)
{//{{{
    std::string use;
    unsigned int sub = sub_xbar;
    unsigned int bit = bit_pos;
    unsigned int cal_pos = 0;
    unsigned int temp_sub = 0;
    unsigned int temp_channel = 0;
    use = "GPIO";
    cal_pos = sub * 16 + bit;
    mXBARAgent->SelectforOutput (cal_pos, cal_pos, use);
    temp_sub = cal_pos / 16;
    temp_channel = cal_pos % 16;
    mXBARAgent->print_msg("info","Choose: GPIO P5%i[%i] \n",temp_sub,temp_channel);
}//}}}

///*********************************
/// Function     : CheckRHSBGCRkLforGTM
/// Description  : Function to check value of RHSBGCRkL register - GTM user
///*********************************
void RHSB_XBAR_Func::CheckRHSBGCRkLforGTM (const unsigned int sub_xbar, const unsigned int bit_pos, const std::string CRO_reg)
{//{{{
    std::string gtm;
    unsigned int sub = sub_xbar;
    unsigned int bit = bit_pos;
    unsigned int cal_pos_out = 0;
    unsigned int cal_pos_in = 0;
    std::ostringstream Lbit_name;
    Lbit_name.str("");Lbit_name << "MMD" << sub << bit;
    if (CRO_reg == "CRO0") {
        if ((*RHSBGCR0L)[Lbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR0L)[Lbit_name.str().c_str()]) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR0L)[Lbit_name.str().c_str()])-8) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    } else if (CRO_reg == "CRO1"){
        if ((*RHSBGCR1L)[Lbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR1L)[Lbit_name.str().c_str()]) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR1L)[Lbit_name.str().c_str()])-8) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    } else if (CRO_reg == "CRO2"){
        if ((*RHSBGCR2L)[Lbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR2L)[Lbit_name.str().c_str()]) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR2L)[Lbit_name.str().c_str()])-8) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    } else {
        if ((*RHSBGCR3L)[Lbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR3L)[Lbit_name.str().c_str()]) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR3L)[Lbit_name.str().c_str()])-8) * 8 + bit;
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    }
}//}}}

///*********************************
/// Function     : CheckRHSBGCRkHforGTM
/// Description  : Function to check value of RHSBGCRkH register - GTM user
///*********************************
void RHSB_XBAR_Func::CheckRHSBGCRkHforGTM (const unsigned int sub_xbar, const unsigned int bit_pos, const std::string CRO_reg)
{//{{{ 
//    std::ostringstream Hbit_name;
    std::string gtm;
    unsigned int sub = sub_xbar;
    unsigned int bit = bit_pos;
    unsigned int cal_pos_out = 0;
    unsigned int cal_pos_in = 0;
    std::ostringstream Hbit_name;
    Hbit_name.str("");Hbit_name << "MMD" << sub << bit;
    if (CRO_reg == "CRO0") {
        if ((*RHSBGCR0H)[Hbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR0H)[Hbit_name.str().c_str()]) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR0H)[Hbit_name.str().c_str()])-8) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    } else if (CRO_reg == "CRO1"){
        if ((*RHSBGCR1H)[Hbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR1H)[Hbit_name.str().c_str()]) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR1H)[Hbit_name.str().c_str()])-8) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    } else if (CRO_reg == "CRO2"){
        if ((*RHSBGCR2H)[Hbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR2H)[Hbit_name.str().c_str()]) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR2H)[Hbit_name.str().c_str()])-8) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    } else {
        if ((*RHSBGCR3H)[Hbit_name.str().c_str()] < 8) {
            gtm = "TOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = ((*RHSBGCR3H)[Hbit_name.str().c_str()]) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_TOM sub %i_channel %i\n",(unsigned int)(cal_pos_in/16),(unsigned int)(cal_pos_in%16));
        } else {
            gtm = "ATOM";
            cal_pos_out = sub * 16 + bit;
            cal_pos_in = (((*RHSBGCR3H)[Hbit_name.str().c_str()])-8) * 8 + (bit-8);
            mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, gtm);
            mXBARAgent->print_msg("info","Choose: GTM_ATOM sub %i_ channel %i\n",(unsigned int)(cal_pos_in/8),(unsigned int)(cal_pos_in%8));
        }
    }
}//}}}

//*********************************
// Function     : CheckRHSBGCRkLforATU
// Description  : Function to check value of RHSBGCRkL register - ATU user
//*********************************
void RHSB_XBAR_Func::CheckRHSBGCRkLforATU (const unsigned int sub_xbar, const unsigned int bit_pos, const std::string type, const std::string CRO_reg)
{//{{{
    std::ostringstream Lbit_name;
    unsigned int sub = sub_xbar;
    unsigned int bit = bit_pos;
    unsigned int cal_pos_out = 0;
    unsigned int cal_pos_in = 0;
    unsigned int temp = 0;
    if (CRO_reg == "CRO0") {
        Lbit_name.str("");Lbit_name << "MMD0" << bit;
        temp = (*RHSBGCR0L)[Lbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 4) {      
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4); //temp: sub ID; bit: channel
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }
    } else if (CRO_reg == "CRO1") {
        Lbit_name.str("");Lbit_name << "MMD1" << bit;
        temp = (*RHSBGCR1L)[Lbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }
    } else if (CRO_reg == "CRO2") {
        Lbit_name.str("");Lbit_name << "MMD2" << bit;
        temp = (*RHSBGCR2L)[Lbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }
    } else {
        Lbit_name.str("");Lbit_name << "MMD3" << bit;
        temp = (*RHSBGCR3L)[Lbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 4) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + bit;
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 4);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkL Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }
    }
}//}}}
//
///*********************************
/// Function     : CheckRHSBGCRkHforATU
/// Description  : Function to check value of RHSBGCRkH register - ATU user
///*********************************
void RHSB_XBAR_Func::CheckRHSBGCRkHforATU (const unsigned int sub_xbar, const unsigned int bit_pos, const std::string type, const std::string CRO_reg)
{//{{{
    std::ostringstream Hbit_name;
    unsigned int sub = sub_xbar;
    unsigned int bit = bit_pos;
    unsigned int cal_pos_out = 0;
    unsigned int cal_pos_in = 0;
    unsigned int temp = 0;
    if (CRO_reg == "CRO0") {
        Hbit_name.str("");Hbit_name << "MMD0" << bit;
        temp = (*RHSBGCR0H)[Hbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }
    } else if (CRO_reg == "CRO1"){
        Hbit_name.str("");Hbit_name << "MMD1" << bit;
        temp = (*RHSBGCR1H)[Hbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }        
    } else if (CRO_reg == "CRO2") {
        Hbit_name.str("");Hbit_name << "MMD2" << bit;
        temp = (*RHSBGCR2H)[Hbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }
    } else {
        Hbit_name.str("");Hbit_name << "MMD3" << bit;
        temp = (*RHSBGCR3H)[Hbit_name.str().c_str()];
        if (type == "c") {
            if ((temp != 15) && (temp != 14)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "c");
                mXBARAgent->print_msg("info","Choose: ATU_C sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (C)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in,"Low");
            }
        } else if (type == "d") {
            if ( temp != 15 ) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "d");
                mXBARAgent->print_msg("info","Choose: ATU_D sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (D)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        } else {
            if ((temp != 15) && (temp != 14) && (temp != 13) && (temp != 12) && (temp != 11) && (temp != 10)) {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "e");
                mXBARAgent->print_msg("info","Choose: ATU_E sub %i_ channel %i\n",(unsigned int)(cal_pos_in/4),(unsigned int)(cal_pos_in%4));
            } else {
                if (bit < 12) {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 8);
                } else {
                    cal_pos_out = sub * 16 + bit;
                    cal_pos_in = temp * 4 + (bit - 12);
                }
                mXBARAgent->print_msg("warning","ATU: RHSBCRkH Setting prohibited. The output is low level (E)");
                mXBARAgent->SelectforOutput (cal_pos_out, cal_pos_in, "Low");
            }
        }
    }
}//}}}

// vim600: set foldmethod=marker :
