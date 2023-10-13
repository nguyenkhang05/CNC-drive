// ---------------------------------------------------------------------
// $Id: ECCUEABFunc.cpp,v 1.2 2013/09/10 04:40:57 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include <cstdarg>
#include "ECCUEABFunc.h"

///Constructor of ECCUEABFunc class
ECCUEABFunc::ECCUEABFunc (std::string name, 
                          ECCUEABAgent *ECCUEABAgentPtr,
                          unsigned int CapDepth,
                          unsigned int ERRINTInitial): Ceccueab_regif(name, 32, ERRINTInitial), BusSlaveFuncIf()
{//{{{
    assert(ECCUEABAgentPtr != NULL);
    mECCUEABAgent = ECCUEABAgentPtr;
    mModuleName = name;
    mECCUEABAgent->CommandInit(name);
    Ceccueab_regif::set_instance_name(name);
    if ((CapDepth >= 1)&&(CapDepth <= emMaxCapDepth)) {
        mCapDepth = CapDepth;
    } else {
        mCapDepth = 0x1;
        mECCUEABAgent->print_msg("error","The capture depth is wrong, a default value (0x1) is set\n");
    }
}//}}}

///Destructor of ECCUEABFunc class
ECCUEABFunc::~ECCUEABFunc (void)
{//{{{
}//}}}

///Handle commands of register IF class
void ECCUEABFunc::RegisterHandler (const std::vector<std::string>& args)
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
                msg_return = Ceccueab_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Ceccueab_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Process reset function
void ECCUEABFunc::EnableReset (const bool is_active, const std::string clock_domain)
{//{{{
    Ceccueab_regif::EnableReset(is_active, clock_domain);
}//}}}

///Check whether double error info is captured
bool ECCUEABFunc::CheckCapturedDoubleErrorInfo (void)
{//{{{
   bool status = false;
   if (*DERSTR != 0x0) {
       status = true;
   }
   return status;
}//}}}

///Check whether data 1bit error info is captured
void ECCUEABFunc::CheckCapturedDataECC1BitErrorInfo (ErrorInfoStructure &data_1bit_error_info)
{//{{{
    ErrorInfoStructure info = data_1bit_error_info;
    if (info.data_ecc_1bit_error_status) {
        for (unsigned int i = 0; i < mCapDepth; i++) {
            if (info.error_addr == (*SEADR[i])["SEADR"]) {
                info.data_ecc_1bit_error_status = false;//Ignore the error which is captured
            }
        }
    }
    data_1bit_error_info = info;
}//}}}

///Store double error information
void ECCUEABFunc::StoreDoubleErrorInfo (ErrorInfoStructure double_error_info)
{//{{{
    (*DERSTR)["DEDF00"] = (unsigned int)double_error_info.data_ecc_2bit_error_status;
    (*DERSTR)["APEF00"] = (unsigned int)double_error_info.addr_parity_error_status;
    (*DERSTR)["AECSEDF00"] = (unsigned int)double_error_info.addr_ecc_1bit_error_status;
    (*DERSTR)["AECDEDF00"] = (unsigned int)double_error_info.addr_ecc_2bit_error_status;
    (*DEADR00)["DEADR00"] = (unsigned int)double_error_info.error_addr;
}//}}}

///Store data 1bit error information
void ECCUEABFunc::StoreDataECC1BitErrorInfo (ErrorInfoStructure data_1bit_error_info, const unsigned int store_pos)
{//{{{
    *SERSTR = (*SERSTR)|(0x1<<store_pos);
    (*SEADR[store_pos])["SEADR"] = data_1bit_error_info.error_addr;
}//}}}

///Check whether overflow occurs
void ECCUEABFunc::CheckOverflow (bool &overflow, unsigned int &store_pos)
{//{{{
     bool over_flag = true;
    for (unsigned int i = 0; i < mCapDepth; i++) {
        if ((((*SERSTR)>>i)&0x1) == 0x0) {
            over_flag = false;
            store_pos = i;
            break;
        }
    }
    overflow = over_flag;
}//}}}

///Set over flow status in OVFSTR register
void ECCUEABFunc::SetOverFlowStatus (void)
{//{{{
    (*OVFSTR)["SERROVF0"] = 1;
}//}}}

///Get the value of ERRINT register
unsigned int ECCUEABFunc::GetERRINTValue(const std::string bit_name)
{//{{{
    return ((*ERRINT)[bit_name.c_str()]);
}//}}}

///Virtual function of BusSlaveFuncIf class
void ECCUEABFunc::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void ECCUEABFunc::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of eccueab_regif class
void ECCUEABFunc::cb_SSTCLR_SSTCLR00 (RegCBstr str)
{//{{{
    if (*SSTCLR != 0x0) {
        if (mECCUEABAgent->CheckClockPeriod("sys_clk")) {
            for (unsigned int i = 0; i < 32; i++) {//Check 32 bits in SSTCLR register
                if ((((*SSTCLR)>>i)&0x1) == 1) {
                    (*OVFSTR)["SERROVF0"] = 0;
                    *SERSTR = (*SERSTR)&(~(0x1<<i));
                    *SEADR[i] = 0x0;
                }
            }
        } else {
            mECCUEABAgent->print_msg("error","Can not clear error information when the sys_clk is stopped\n");
        }
    }
    //Clear the value of SSTCLR register
    *SSTCLR = 0x0;
}//}}}

///Virtual function of eccueab_regif class
void ECCUEABFunc::cb_DSTCLR_DSTCLR00 (RegCBstr str)
{//{{{
    if ((*DSTCLR)["DSTCLR00"] == 0x1) {
        if (mECCUEABAgent->CheckClockPeriod("sys_clk")) {
            *DERSTR = 0x0;
            *DEADR00 = 0x0;
        } else {
            mECCUEABAgent->print_msg("error","Can not clear error information when the sys_clk is stopped\n");
        }
    }
    //Clear the value of SSTCLR register
    *DSTCLR = 0x0;
}//}}}
// vim600: set foldmethod=marker :

