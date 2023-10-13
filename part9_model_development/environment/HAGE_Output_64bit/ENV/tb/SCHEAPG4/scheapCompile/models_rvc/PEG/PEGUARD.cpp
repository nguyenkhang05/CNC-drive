// ---------------------------------------------------------------------
// $Id: PEGUARD.cpp,v 1.12 2015/06/28 02:51:24 dungvannguyen Exp $
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
#include "PEGUARD.h"
#include "PEG_AgentController.h"
#include "ERR_INFO.h"
#include "SysErrVCI.h"

///Constructor of PEGUARD class
PEGUARD::PEGUARD(std::string name, 
                 PEG_AgentController *AgentController_ptr,
                 unsigned int id_number,
                 std::string version):
                 Cpeguard_regif(name, 32, id_number, version),
                 BusSlaveFuncIf()
{//{{{
    assert(AgentController_ptr != NULL);
    mAgentController = AgentController_ptr;
    mAgentController->CommandInit(name);
    Cpeguard_regif::set_instance_name(name);
    mModuleName = name;
    mVersion = version;
    mPEGUARD_ID = id_number;
    if (mVersion != "G3MH") {
        if ((mPEGUARD_ID != 1) && (mPEGUARD_ID != 2)) {
            mPEGUARD_ID = 1;//default value
            print_msg("warning","PE's id (%d) is wrong, it is set to the default value (1) automatically.\n",id_number);
            
        }
    } else {//G3MH
        if ((mPEGUARD_ID != 1) && (mPEGUARD_ID != 2) && (mPEGUARD_ID != 3)) {
            mPEGUARD_ID = 1;//default value
            print_msg("warning","PE's id (%d) is wrong, it is set to the default value (1) automatically.\n",id_number);
            
        }
    }
    if ((mVersion != "G3M_V10") && (mVersion != "G3M_V20") && (mVersion != "G3M_V30")
       && (mVersion != "G3K_PCU") && (mVersion != "G3MH") && (mVersion != "G3KH")) {
        mVersion = "G3M_V30";
    }
}//}}}

///Destructor of PEGUARD class
PEGUARD::~PEGUARD()
{//{{{
}//}}}

///Callback function of PEGGBA register
void PEGUARD::cb_PEGGBA_EN(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock("pclk")) {
        if ((mVersion != "G3M_V20") && (mVersion != "G3KH")) {
            if (((str.pre_data >> 4) & 0x1) == 1) {
                mAgentController->print_msg("warning","Cannot write to PEGGBA%d while PEGGBA%d.LOCK bit is 1.\n", str.channel, str.channel);
                (*PEGGBA[str.channel]) = str.pre_data;
            }
        }
    } else {
        (*PEGGBA[str.channel]) = str.pre_data;
    }
}//}}}

///Callback function of PEGGMK register
void PEGUARD::cb_PEGGMK_MASK(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock("pclk")) {
        if ((mVersion != "G3M_V20") && (mVersion != "G3KH")) {
            if ((*PEGGBA[str.channel])["LOCK"] == 1) {
                mAgentController->print_msg("warning","Cannot write to PEGGMK%d while PEGGBA%d.LOCK bit is 1.\n", str.channel, str.channel);
                (*PEGGMK[str.channel]) = str.pre_data;
            }
        }
    } else {
        (*PEGGMK[str.channel]) = str.pre_data;
    }
}//}}}

///Callback function of PEGGSP register
void PEGUARD::cb_PEGGSP_SPID(RegCBstr str)
{//{{{
    if (mAgentController->CheckClock("pclk")) {
        if ((mVersion != "G3M_V20") && (mVersion != "G3KH")) {
            if ((*PEGGBA[str.channel])["LOCK"] == 1) {
                mAgentController->print_msg("warning","Cannot write to PEGGSP%d while PEGGBA%d.LOCK bit is 1.\n", str.channel, str.channel);
                (*PEGGSP[str.channel]) = str.pre_data;
            }
        }
    } else {
        (*PEGGSP[str.channel]) = str.pre_data;
    }
}//}}}

///Check whether the address of transaction in guarded areas
bool PEGUARD::CheckTransAddress(const unsigned int trans_addr, const std::string mode)
{//{{{
    bool result = false;
    std::string operation_mode = mode;
    std::string name_of_area = "DEBUG, APB, TCM and ROM";
    for (unsigned int i=0; i<mAreaInfo.size(); i++) {
        if ((trans_addr >= mAreaInfo[i].start_addr) && (trans_addr <= mAreaInfo[i].end_addr)) {
            if (operation_mode == "CHECK_RANGE") {
                name_of_area = mAreaInfo[i].area_name;
                mAgentController->print_msg("info","Access address 0x%08X is inside %s area.\n", trans_addr, name_of_area.c_str());
                result = true;
                break;
            } else if (operation_mode == "DEBUG") {//Check access area DEBUG, APB, TCM, ROM
                if ((*PEGDB)["PEG_DBAREA"] == 1) {//Check permissions debug area
                    if (mAreaInfo[i].area_name == "DEBUG") {
                        result = true;
                        if (!CheckPEGUARD_ID(i)) {
                            result = false;
                            break;
                        }
                    }
                } else {
                    name_of_area = "APB, TCM and ROM";
                }
                if ((mAreaInfo[i].area_name == "APB") || (mAreaInfo[i].area_name == "TCM") || (mAreaInfo[i].area_name == "ROM")) {
                    result = true;
                    if (!CheckPEGUARD_ID(i)) {
                        result = false;
                        break;
                    }
                }
            } else { //SAFETY, SECURITY, SPID, PEID modes
                name_of_area = "TCM and ROM";
                if ((mAreaInfo[i].area_name == "TCM") || (mAreaInfo[i].area_name == "ROM")) {
                    result = true;
                    if (!CheckPEGUARD_ID(i)) {
                        result = false;
                        break;
                    }
                }
            }
        }
    }
    if (!result) {
        mAgentController->print_msg("warning","Access address 0x%08X is outside %s areas of PE%d.\n", trans_addr, name_of_area.c_str(), mPEGUARD_ID);
    }
    return result;
}//}}}

///Check whether the transaction is sent to right PE or not
bool PEGUARD::CheckPEGUARD_ID(const unsigned int area_id)
{//{{{
    if (mVersion != "G3MH") {
        if (((mAreaInfo[area_id].peid == "PE1") && (mPEGUARD_ID == 1)) ||
            ((mAreaInfo[area_id].peid == "PE2") && (mPEGUARD_ID == 2)) ||
            (mAreaInfo[area_id].peid == "BOTH")) {
            return true;
        } else {
            mAgentController->print_msg("warning","Cannot access to memory of %s via PEG%d.\n", mAreaInfo[area_id].peid.c_str(), mPEGUARD_ID);
            return false;
        }
    } else {
        if (((mAreaInfo[area_id].peid == "PE1") && (mPEGUARD_ID == 1)) ||
            ((mAreaInfo[area_id].peid == "PE2") && (mPEGUARD_ID == 2)) ||
            ((mAreaInfo[area_id].peid == "PE3") && (mPEGUARD_ID == 3)) ||
            (mAreaInfo[area_id].peid == "BOTH")) {
            return true;
        } else {
            mAgentController->print_msg("warning","Cannot access to memory of %s via PEG%d.\n", mAreaInfo[area_id].peid.c_str(), mPEGUARD_ID);
            return false;
        }
    }
}//}}}

///Set start address and size for each guarded area
bool PEGUARD::SetAreaAddr(const std::string name, const unsigned int start_addr, const unsigned int size, const std::string pe_id)
{//{{{
    unsigned int start_address = start_addr;
    unsigned int end_address = start_addr + size - 1;
    AreaInformation temp;
    temp.area_name = name;
    temp.peid = pe_id;
    temp.start_addr = start_address;
    temp.end_addr = end_address;
    bool is_valid = true;
    for (unsigned int i=0; i<mAreaInfo.size(); i++) {
        if ((start_address > mAreaInfo[i].end_addr) || (end_address < mAreaInfo[i].start_addr)) {
            is_valid = true;
        } else {
            is_valid = false;
            break;
        }
    }
    if (is_valid) {
        mAreaInfo.push_back(temp);
    } else {
        mAgentController->print_msg("warning","Address 0x%X and size %d are invalid. They are overlapped with another area.\n", start_address, size);
    }
    return is_valid;
}//}}}

void PEGUARD::SetAccessMode(const std::string mode, const bool is_enable)
{//{{{
    std::string name = "";
    std::string enb = "disabled";
    if (is_enable) {
        enb = "enabled";
    }

    if (mode == "DB") {
        (*PEGDB)["PEG_DBEN"] = (unsigned int)is_enable;
        name = "Debug";
    } else if (mode == "SF") {
        if (mVersion != "G3MH") {
            (*PEGSF)["PEG_SFEN"] = (unsigned int)is_enable;
            name = "Safety";
        } else {
            mAgentController->print_msg("warning","This command is ignored because model is operating in %s version.\n", mVersion.c_str());
        }
    } else if (mode == "SP") {
        if (mVersion != "G3M_V30") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                (*PEGSP)["PEG_SPEN"] = (unsigned int)is_enable;
            } else {
                (*PEGSP)["SPEN"] = (unsigned int)is_enable;
            }
            name = "SPID";
        } else {
            mAgentController->print_msg("warning","This command is ignored because model is operating in %s version.\n", mVersion.c_str());
        }
    } else if (mode == "SC") {
        if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
            if (is_enable) {
                if ((*PEGSP)["PEG_SPEN"] == 1) {
                    (*PEGSC)["PEG_SCEN"] = (unsigned int)is_enable;
                } else {
                    mAgentController->print_msg("warning","Cannot enable security access mode because SPID mode is not enabled yet.\n");
                }
            } else {
                (*PEGSC)["PEG_SCEN"] = (unsigned int)is_enable;
            }
            name = "Security";
        } else {
            mAgentController->print_msg("warning","This command is ignored because model is operating in %s version.\n", mVersion.c_str());
        }
    } else {
        mAgentController->print_msg("error","Name of access mode is invalid.\n");
    }
    
    if (name != "") {
        mAgentController->print_msg("info","%s access mode is %s.\n", name.c_str(), enb.c_str());
    }
}//}}}

void PEGUARD::SetG0InitialVal(const std::string bit_name, const unsigned int value, const bool is_started_simulation)
{//{{{
    if (bit_name == "MK") {
        if (value > 0xFFFFF) {
            mAgentController->print_msg("warning","Value of MASK bit is invalid. It should not be greater than 0xFFFFF.\n");
        } else {
            PEGGMK[0]->UpdateBitInitValue("MASK", value);
            if (!is_started_simulation) {
                (*PEGGMK[0])["MASK"] = value;
            }
        }
    } else if (bit_name == "BS") {
        if (value > 0xFFFFF) {
            mAgentController->print_msg("warning","Value of BASE bit is invalid. It should not be greater than 0xFFFFF.\n");
        } else {
            PEGGBA[0]->UpdateBitInitValue("BASE", value);
            if (!is_started_simulation) {
                (*PEGGBA[0])["BASE"] = value;
            }
        }
    } else if (bit_name == "WR") {
        if (value > 1) {
            mAgentController->print_msg("warning","Value of WR bit is invalid. It should not be greater than 0x1.\n");
        } else {
            PEGGBA[0]->UpdateBitInitValue("WR", value);
            if (!is_started_simulation) {
                (*PEGGBA[0])["WR"] = value;
            }
        }
    } else if (bit_name == "RD") {
        if (value > 1) {
            mAgentController->print_msg("warning","Value of RD bit is invalid. It should not be greater than 0x1.\n");
        } else {
            PEGGBA[0]->UpdateBitInitValue("RD", value);
            if (!is_started_simulation) {
                (*PEGGBA[0])["RD"] = value;
            }
        }
    } else if (bit_name == "EN") {
        if (value > 1) {
            mAgentController->print_msg("warning","Value of EN bit is invalid. It should not be greater than 0x1.\n");
        } else {
            PEGGBA[0]->UpdateBitInitValue("EN", value);
            if (!is_started_simulation) {
                (*PEGGBA[0])["EN"] = value;
            }
        }
    } else if (bit_name == "SP") {
        if ((mVersion != "G3M_V20") && (mVersion != "G3KH")) {
            if (mVersion != "G3MH") {
                PEGGSP[0]->UpdateBitInitValue("SPID", value);
            } else {
                PEGGSP[0]->UpdateBitInitValue("SP", value);
            }
            if (!is_started_simulation) {
                *PEGGSP[0] = value;
            }
         } else {//G3M_V20 or G3KH
            PEGGBA[0]->UpdateBitInitValue("SP0", value&0x1);
            PEGGBA[0]->UpdateBitInitValue("SP1", (value&0x2)>>1);
            PEGGBA[0]->UpdateBitInitValue("SP2", (value&0x4)>>2);
            PEGGBA[0]->UpdateBitInitValue("SP3", (value&0x8)>>3);
         }
    } else {
        mAgentController->print_msg("error","Bit name is invalid.\n");
    }
}//}}}

///Reset all registers
void PEGUARD::Reset(const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "presetz") {
        Cpeguard_regif::EnableReset(is_active);
    }
}//}}}

///Check access ability to peripherals
bool PEGUARD::CheckAccessibility(TlmBasicPayload &trans, Cerrinfo &err_info, Csyserrvci &syserrvci)
{//{{{
    bool status = true;
    bool is_pe_trans = true;
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    bool trans_type;
    if ((is_read) && (!is_write)){
        err_info.PEGRdERR = true;
        err_info.PEGWrERR = false;
        trans_type = false; // read trans
    }
    else {
        err_info.PEGWrERR = true;
        err_info.PEGRdERR = false;
        trans_type = true; // write trans
    }

    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned char spid = ExtensionInfo->getSpId();
    unsigned char peid = ExtensionInfo->getPeId();
    unsigned char vcid = ExtensionInfo->getVmId();
    unsigned char tcid = ExtensionInfo->getTcId();
    unsigned int addr = (unsigned int)trans.get_address();
    unsigned int length = (unsigned int)trans.get_data_length();

    is_pe_trans = CheckTransAddress(addr, "CHECK_RANGE");
    if ((is_pe_trans) &&
        (CheckDebugAccess(addr, peid) ||
        CheckSafetyAccess(addr, peid, vcid, vm) ||
        CheckSecurityAccess(addr, spid, trans_type) ||
        CheckSPID_PEIDAccess(addr, spid, peid, trans_type, false))) {
        status = true;
    } else {
        mAgentController->print_msg("info","The transaction is not allowed to transfer.\n");
        status = false;
        err_info.SPID = spid;
        err_info.PEID = peid;
        syserrvci.SysErrVCIAdr = addr;
        syserrvci.SysErrVCI = true;
        syserrvci.SysErrVCIVld = true;
        unsigned int side_band = 0;
        if (mVersion == "G3M_V30") {
            side_band = ((tcid&0x7) << 10) + ((vcid&0x7) << 7) + ((peid&0x7) << 13) + ((spid&0x1F) << 2) + ((unsigned int)um << 1) + (unsigned int)vm;
        } else if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
            side_band = ((tcid&0x3F) << 10) + ((vcid&0x7) << 7) + ((peid&0x7) << 4) + ((spid&0x3) << 2) + ((unsigned int)um << 1) + (unsigned int)vm;
        } else if (mVersion == "G3MH") {
            side_band = ((peid&0x7) << 13) + ((spid&0x1F) << 2) + ((unsigned int)um << 1) + (unsigned int)vm;
        } else {//mVersion = G3KH
            side_band = ((tcid&0x3F) << 10) + ((vcid&0x7) << 7) + ((peid&0x7) << 4) + ((spid&0x3) << 2) + ((unsigned int)um << 1);
        }
        unsigned int request_type = (unsigned int)trans_type << 2;
        if (length == 2) {//Half word
            request_type = request_type + 0x1;
        } else if (length == 4) {//Word
            request_type = request_type + 0x2;
        } else if (length == 8) {//Douple word
            request_type = request_type + 0x3;
        } else {//Byte
            request_type = request_type + 0x0;
        }
        unsigned int etyp = 0x8 << 24;
        unsigned int VCIInfo = etyp + (request_type << 16) + side_band;
        syserrvci.SysErrVCIInfo = VCIInfo;
    }
    return status;
}//}}}

///Check debug access mode
bool PEGUARD::CheckDebugAccess(const unsigned int trans_addr, const unsigned int peid_val)
{//{{{
    bool result = false;
    if ((*PEGDB)["PEG_DBEN"] == 1) {//Check whether debug access mode is enabled
        if (mVersion != "G3MH") {
            if (peid_val == (*PEGDB)["PEG_DBPEID"]) {
                result = CheckTransAddress(trans_addr, "DEBUG");
            }
        } else {//G3MH
            if (peid_val == (*PEGDB)["PEG_DBID"]) {
                result = CheckTransAddress(trans_addr, "DEBUG");
            }
        }
    }
    if (result) {
        mAgentController->print_msg("info","The transaction is transferred in debug mode.\n");
    }
    return result;
}//}}}

///Check safety access mode
bool PEGUARD::CheckSafetyAccess(const unsigned int trans_addr, const unsigned int peid_val, const unsigned int vcid_val, const bool vm_val)
{//{{{
    bool result = false;
    if (mVersion != "G3MH") {
        if ((*PEGSF)["PEG_SFEN"] == 1) {
            if (peid_val == (*PEGSF)["PEG_SFPEID"]) {
                if ((vcid_val == 0) || (!vm_val)) {
                    result = CheckTransAddress(trans_addr, "SAFETY");
                }
            }
        }
        if (result) {
            mAgentController->print_msg("info","The transaction is transferred in safety mode.\n");
        }
    } else {//Safety access is ignored in G3MH version
        result = false;
    }
    return result;
}//}}}

///Check SPID access mode
bool PEGUARD::CheckSPID_PEIDAccess(const unsigned int trans_addr, const unsigned int spid_val, const unsigned int peid_val,
                                   const bool trans_type, const bool is_secured)
{//{{{
    bool result = false;
    bool is_write = trans_type;
    unsigned int lim_of_area = 0;
    bool is_security_mode = is_secured;
    std::string name_of_mode = "SPID";
    if ((mVersion == "G3MH") || (mVersion == "G3KH")) {
        name_of_mode = "SPID_PEID";
    }
    if (is_security_mode) {
        if ((*PEGSC)["PEG_SCEN"] == 1) {
            lim_of_area = 1;
            name_of_mode = "security";
        } else {
            return false;
        }
    } else {
        if ((mVersion == "G3M_V20") || (mVersion == "G3MH") || (mVersion == "G3KH")) {
            lim_of_area = emNumberOfSPIDArea_V20_G3MH_G3KH;
        } else {//G3M_V30 or Other version
            lim_of_area = emNumberOfSPIDArea_V30_Other;
        }
    }
    unsigned int peg_spen = 0;
    if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
        peg_spen = (*PEGSP)["PEG_SPEN"]; 
    } else {
        peg_spen = (*PEGSP)["SPEN"]; 
    }
    if (peg_spen == 1) {
        for (unsigned int i=0; i<lim_of_area; i++) {
            if ((*PEGGBA[i])["EN"] == 1) {
                if (is_security_mode) {
                    if (spid_val > 1) {
                        return false;
                    }
                }
                unsigned int compared_spid_value = 0;
                if ((mVersion == "G3M_V20") || (mVersion == "G3KH")) {
                    if (spid_val < 4) {
                        compared_spid_value = ((*PEGGBA[i] >> (4 + spid_val))) & 0x1;
                    }
                } else {
                    if (mVersion != "G3MH") {
                        compared_spid_value = ((*PEGGSP[i])["SPID"] >> spid_val) & 0x1;
                    } else {
                        compared_spid_value = ((*PEGGSP[i])["SP"] >> spid_val) & 0x1;
                    }
                }
                unsigned int compared_peid_value = 1;
                if (mVersion == "G3MH") {
                    compared_peid_value = ((*PEGGPE[i])["PE"] >> peid_val) & 0x1;
                }
                
                if ((compared_spid_value == 1) && (compared_peid_value == 1)) {
                    bool continue_checking = false;
                    if (is_write == true) {
                        if ((*PEGGBA[i])["WR"] == 1) {
                            continue_checking = true;
                        }
                    } else {
                        if ((*PEGGBA[i])["RD"] == 1) {
                            continue_checking = true;
                        }
                    }
                    if (continue_checking) {
                        unsigned int checked_value = (trans_addr & ((~(*PEGGMK[i])["MASK"]) << 12)) & 0xFFFFF000;
                        unsigned int compared_value = (((*PEGGBA[i])["BASE"] & (~(*PEGGMK[i])["MASK"])) << 12) & 0xFFFFF000;
                        if (checked_value == compared_value) {
                            result = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (result) {
        result = CheckTransAddress(trans_addr, "SPID_PEID");
    }
    if (result) {
        mAgentController->print_msg("info","The transaction is transferred in %s mode.\n",name_of_mode.c_str());
    }
    return result;
}//}}}

///Check security access mode
bool PEGUARD::CheckSecurityAccess(const unsigned int trans_addr, const unsigned int spid_val, const bool trans_type)
{//{{{
    bool result = false;
    if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
        result = CheckSPID_PEIDAccess(trans_addr, spid_val, 1, trans_type, true);//PEID value (1) is ignored in security access
    } else {//Security access is ignored in G3MH and G3Kh versions
        result = false;
    }
    return result;
}//}}}

///Set parameters and commands to peguard_regif class
void PEGUARD::RegisterHandler(const std::vector<std::string> &args)
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
                msg_return = Cpeguard_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {
            msg_return = Cpeguard_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Access read to registers
void PEGUARD::read (unsigned int offsetAddress, 
                            TlmBasicPayload& trans, 
                            BusTime_t* t, 
                            bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}

///Access write to registers
void PEGUARD::write (unsigned int offsetAddress, 
                             TlmBasicPayload& trans, 
                             BusTime_t* t, 
                             bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}//}}}
// vim600: set foldmethod=marker :

