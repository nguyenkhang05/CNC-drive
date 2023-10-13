// ---------------------------------------------------------------------
// $Id: IPG_Func.cpp,v 1.7 2015/06/28 16:50:10 dungvannguyen Exp $
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
#include "IPG_Func.h"
#include "IPG_AgentController.h"
#include "SysErrIPG.h"

///Constructor of IPG_Func class
IPG_Func::IPG_Func (std::string name,
                  IPG_AgentController *IPG_AgentControllerPtr,
                  std::string IPG_VERSION):Cipg_regif(name, 32, IPG_VERSION), BusSlaveFuncIf()
{//{{{
    assert(IPG_AgentControllerPtr != NULL);
    mIPG_AgentController = IPG_AgentControllerPtr;
    mIPG_AgentController->CommandInit(name);
    Cipg_regif::set_instance_name(name);
    mModuleName = name;
    mVersion = IPG_VERSION;
    mAreaInfoValue.area_name = "NONE";
    mAreaInfoValue.start_addr = 0x0;
    mAreaInfoValue.size = 0x0;
    mErrPortValue = new Csyserripg();
    mErrPortValue->Initialize();
}//}}}

///Destructor of IPG_Func class
IPG_Func::~IPG_Func (void)
{//{{{
    delete mErrPortValue;
}//}}}

///Reset operation of IPG_Func class
void IPG_Func::Reset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "presetz") {
        Cipg_regif::EnableReset(is_active);
    } else {//if reset_name is "lvcirreset"
        mErrPortValue->Initialize();
    }
}//}}}

///Dump information of registers
void IPG_Func::DumpStatusInfo (void)
{//{{{
    mIPG_AgentController->print_msg("info","IPGECRUM : 0x%X\n", (unsigned int)(*IPGECRUM));
    mIPG_AgentController->print_msg("info","IPGADRUM  : 0x%X\n", (unsigned int)(*IPGADRUM));
    if (mVersion != "G3KH") {
        mIPG_AgentController->print_msg("info","IPGECRSV0 : 0x%X\n", (unsigned int)(*IPGECRSV0));
        mIPG_AgentController->print_msg("info","IPGADRSV0 : 0x%X\n", (unsigned int)(*IPGADRSV0));
        mIPG_AgentController->print_msg("info","IPGECRUM0 : 0x%X\n", (unsigned int)(*IPGECRUM0));
        mIPG_AgentController->print_msg("info","IPGADRUM0 : 0x%X\n", (unsigned int)(*IPGADRUM0));
        if (mVersion != "G3MH") {
            mIPG_AgentController->print_msg("info","IPGECRSV1 : 0x%X\n", (unsigned int)(*IPGECRSV1));
            mIPG_AgentController->print_msg("info","IPGECRSV2 : 0x%X\n", (unsigned int)(*IPGECRSV2));
            mIPG_AgentController->print_msg("info","IPGECRSV3 : 0x%X\n", (unsigned int)(*IPGECRSV3));
            mIPG_AgentController->print_msg("info","IPGECRUM1 : 0x%X\n", (unsigned int)(*IPGECRUM1));
            mIPG_AgentController->print_msg("info","IPGECRUM2 : 0x%X\n", (unsigned int)(*IPGECRUM2));
            mIPG_AgentController->print_msg("info","IPGECRUM3 : 0x%X\n", (unsigned int)(*IPGECRUM3));
            mIPG_AgentController->print_msg("info","IPGADRSV1 : 0x%X\n", (unsigned int)(*IPGADRSV1));
            mIPG_AgentController->print_msg("info","IPGADRSV2 : 0x%X\n", (unsigned int)(*IPGADRSV2));
            mIPG_AgentController->print_msg("info","IPGADRSV3 : 0x%X\n", (unsigned int)(*IPGADRSV3));
            mIPG_AgentController->print_msg("info","IPGADRUM1 : 0x%X\n", (unsigned int)(*IPGADRUM1));
            mIPG_AgentController->print_msg("info","IPGADRUM2 : 0x%X\n", (unsigned int)(*IPGADRUM2));
            mIPG_AgentController->print_msg("info","IPGADRUM3 : 0x%X\n", (unsigned int)(*IPGADRUM3));
        }
    }
}//}}}

///Register handler command to ipg_regif class
void IPG_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Cipg_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Cipg_regif class
            msg_return = Cipg_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Set guarded areas info
void IPG_Func::SetGuardAreasInfo (const std::string area_name, const unsigned int start_addr, const unsigned int size)
{//{{{
    mAreaInfoValue.area_name = area_name;
    mAreaInfoValue.start_addr = start_addr;
    mAreaInfoValue.size = size;
    mAreaInfo.push_back(mAreaInfoValue);
}//}}}

///Get guarded areas info
void IPG_Func::GetGuardAreasInfo (const std::string area_name)
{//{{{
    bool IsAreaActive = false;
    for (unsigned int index = 0; index < mAreaInfo.size(); index++) {
        if (area_name == mAreaInfo[index].area_name) {
            IsAreaActive = true;
            mIPG_AgentController->print_msg("info","%s area info: start address is 0x%X - size is 0x%X\n",
                                            mAreaInfo[index].area_name.c_str(), mAreaInfo[index].start_addr, mAreaInfo[index].size);
        }
    }
    if (!IsAreaActive) {//If area is not set before
        mIPG_AgentController->print_msg("info","Areas information is empty. Please use this command with argument (setvalue) firstly\n");
    }
}//}}}

//Check access permission
bool IPG_Func::CheckAccessAddress (const bool is_ifu, TlmBasicPayload& trans)
{//{{{
    bool accessibility = true;
    unsigned int addr = (unsigned int)trans.get_address();
    std::vector<std::string> area;
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    for (unsigned int index = 0; index < mAreaInfo.size(); index++) {
        //find guared area name
        if ((mAreaInfo[index].start_addr <= addr)&&(addr < (mAreaInfo[index].start_addr + mAreaInfo[index].size))) {
            area.push_back(mAreaInfo[index].area_name);
        }
    }
    if (area.size() != 0) {//if the addess is in guared areas
        if (is_ifu) {
            accessibility =  CheckIFUAccessPermission(area, trans);
        } else {
            accessibility =  CheckLSUAccessPermission(area, trans);
        }
    }
    return accessibility;
}//}}}

///Virtual function of BusSlaveFuncIf class
void IPG_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned char vmid = ExtensionInfo->getVmId();
    unsigned int addr = (unsigned int)trans.get_address();
    if ((!vm) && (!um)) {//HV privilege
        if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
            memset(trans.get_data_ptr(), 0, trans.get_data_length());
            if (debug == false) {
                reg_rd(addr, trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_rd_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
            }
        } else {
            if ((addr&0x3FF) >= 0x40) {
                memset(trans.get_data_ptr(), 0, trans.get_data_length());
                if (debug == false) {
                    reg_rd(addr, trans.get_data_ptr(), trans.get_data_length());
                } else {
                    reg_rd_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
                }
            } else {
                mIPG_AgentController->re_printf("error","Cannot reading access to address 0x%X in HV privilege\n", addr);
            }
        }
    } else if (vm && (!um)) {//SV privilege
        if (((((addr&0x3FF) >= 0x2) && ((addr&0x3FF) <= 0x3)))||(((addr&0x3FF) >= 0x8) && (((addr&0x3FF) <= 0xB)))||
            ((addr&0x3FF) == 0xD)||((0x20 <= (addr&0x3FF))&&((addr&0x3FF) <= 0x24))) {
            memset(trans.get_data_ptr(), 0, trans.get_data_length());
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                if (debug == false) {
                    reg_rd(addr+0x40*(vmid+1), trans.get_data_ptr(), trans.get_data_length());
                } else {
                    reg_rd_dbg(addr+0x40*(vmid+1), trans.get_data_ptr(), trans.get_data_length());
                }
            } else {
                if (debug == false) {
                    reg_rd(addr, trans.get_data_ptr(), trans.get_data_length());
                } else {
                    reg_rd_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
                }
            }
        } else {
            mIPG_AgentController->re_printf("error","Cannot reading access to address 0x%X in SV privilege\n", addr);
        }
    } else {
        mIPG_AgentController->re_printf("error","Cannot reading access to IPG's registers because of wrong access right\n");
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void IPG_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned char vmid = ExtensionInfo->getVmId();
    unsigned int addr = (unsigned int)trans.get_address();
    if ((!vm) && (!um)) {//HV privilege
        if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
            if (debug == false) {
                reg_wr(addr, trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_wr_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
            }
        } else {
            if ((addr&0x3FF) >= 0x40) {
                if (debug == false) {
                    reg_wr(addr, trans.get_data_ptr(), trans.get_data_length());
                } else {
                    reg_wr_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
                }
            } else {
                mIPG_AgentController->re_printf("error","Cannot writing access to address 0x%X in HV privilege\n", addr);
            }
        }
    } else if (vm && (!um)) {//SV privilege
        if (((addr&0x3FF) == 0x2)||((addr&0x3FF) == 0x8)||((addr&0x3FF) == 0xD)||((0x20 <= (addr&0x3FF))&&((addr&0x3FF) <= 0x24))) {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                if (debug == false) {
                    reg_wr(addr+0x40*(vmid+1), trans.get_data_ptr(), trans.get_data_length());
                } else {
                    reg_wr_dbg(addr+0x40*(vmid+1), trans.get_data_ptr(), trans.get_data_length());
                }
            } else {
                if (debug == false) {
                    reg_wr(addr, trans.get_data_ptr(), trans.get_data_length());
                } else {
                    reg_wr_dbg(addr, trans.get_data_ptr(), trans.get_data_length());
                }
            }
        } else {
            mIPG_AgentController->re_printf("error","Cannot writing access to address 0x%X in SV privilege\n", addr);
        }
    } else {
        mIPG_AgentController->re_printf("error","Cannot writing access to IPG's registers because of wrong access right\n");
    }
}//}}}

///Check access permission of IFU master
bool IPG_Func::CheckIFUAccessPermission (std::vector<std::string> area_name, TlmBasicPayload& trans)
{//{{{
    bool ifu_accessibility = true;
    bool accessibility_temp = true;
    std::vector<bool> ifu_access;
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned char vcid = ExtensionInfo->getVmId();
    for (unsigned int index = 0; index < area_name.size(); index++) {
        if (area_name[index] == "GVCI") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                accessibility_temp = CheckIFUAreaAccessPermission ("X0", IPGPMTUM[0], IPGPMTSV0[0], IPGPMTUM0[0], trans);
            } else {
                accessibility_temp = true;
            }
            ifu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "VPISS") {
            if (mVersion != "G3KH") {
                if (mVersion != "G3MH") {
                    accessibility_temp = CheckIFUAreaAccessPermission ("X1", IPGPMTUM[0], IPGPMTSV0[0], IPGPMTUM0[0], trans);
                } else {
                    accessibility_temp = true;
                }
            } else {
                accessibility_temp = CheckIFUAreaAccessPermission ("X1", IPGPMTUM[0], NULL, NULL, trans);
            }
            ifu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "GRAM0") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                accessibility_temp = CheckIFUAreaAccessPermission ("X0", IPGPMTUM[1], IPGPMTSV1[0], IPGPMTUM1[0], trans);
            } else {
                accessibility_temp = true;
            }
            ifu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "GRAM1") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                accessibility_temp = CheckIFUAreaAccessPermission ("X1", IPGPMTUM[1], IPGPMTSV1[0], IPGPMTUM1[0], trans);
            } else {
                accessibility_temp = true;
            }
            ifu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "AXI") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                if ((mVersion == "G3M_V20")||(mVersion == "G3M_V30")) {
                    accessibility_temp = CheckIFUAreaAccessPermission ("X1", IPGPMTUM[4], IPGPMTSV0[1], IPGPMTUM4[0], trans);
                    ifu_access.push_back(accessibility_temp);
                } else {//If mVersion is G3MV10 or G3K_PCU
                    accessibility_temp = CheckIFUAreaAccessPermission ("X0", IPGPMTUM[0], IPGPMTSV0[0], IPGPMTUM0[0], trans);
                    ifu_access.push_back(accessibility_temp);
                }
            } else {
                accessibility_temp = true;
            }
        } else {//it is not in guared area of IFU
            ifu_access.push_back(true);
        }
    }
    //If a area is not avaiable, the transaction is blocked
    for (unsigned int index = 0; index < ifu_access.size(); index++) {
        ifu_accessibility = ifu_accessibility & ifu_access[index];
    }
    if (!ifu_accessibility) {
        mErrPortValue->SysErrIPG = true;
        mErrPortValue->SysErrIPGUM = um;
        mErrPortValue->SysErrIPGVM = vm;
        mErrPortValue->SysErrIPGVMId = vcid;
        mIPG_AgentController->SendOutputErrorPort(*mErrPortValue); 
    }
    return ifu_accessibility;
}//}}}

///Check access permission of IFU master
bool IPG_Func::CheckLSUAccessPermission (std::vector<std::string> area_name, TlmBasicPayload& trans)
{//{{{
    bool lsu_accessibility = true;
    bool accessibility_temp = true;
    std::vector<bool> lsu_access;
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned char vcid = ExtensionInfo->getVmId();
    for (unsigned int index = 0; index < area_name.size(); index++) {
        if (area_name[index] == "GVCI") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                accessibility_temp = CheckLSUAreaAccessPermission("R0", "W0", IPGPMTUM[0], IPGPMTSV0[0], IPGPMTUM0[0], trans);
            } else {
                accessibility_temp = true;
            }
            lsu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "VPISS") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                accessibility_temp = CheckLSUAreaAccessPermission("R1", "W1", IPGPMTUM[0], IPGPMTSV0[0], IPGPMTUM0[0], trans);
            } else if (mVersion == "G3KH") {
                accessibility_temp = CheckLSUAreaAccessPermission("R1", "W1", IPGPMTUM[0], NULL, NULL, trans);
            } else {
                accessibility_temp = true;
            }
            lsu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "INTC1") {
            if (mVersion != "G3KH") {
                accessibility_temp= CheckLSUAreaAccessPermission("R0", "W0", IPGPMTUM[2], IPGPMTSV2[0], IPGPMTUM2[0], trans);
            } else {
                accessibility_temp= CheckLSUAreaAccessPermission("R0", "W0", IPGPMTUM[2], NULL, NULL, trans);
            }
            lsu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "MECNT") {
            if (mVersion != "G3KH") {
                accessibility_temp = CheckLSUAreaAccessPermission("R1", "W1", IPGPMTUM[2], IPGPMTSV2[0], IPGPMTUM2[0], trans);
            } else {
                accessibility_temp = true;
            }
            lsu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "Debug") {
            if (mVersion != "G3KH") {
                accessibility_temp = CheckLSUAreaAccessPermission("R0", "W0", IPGPMTUM[3], IPGPMTSV3[0], IPGPMTUM3[0], trans);
            } else {
                accessibility_temp = true;
            }
            lsu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "SEC") {
            if (mVersion != "G3KH") {
                accessibility_temp = CheckLSUAreaAccessPermission("R1", "W1", IPGPMTUM[3], IPGPMTSV3[0], IPGPMTUM3[0], trans);
            } else {
                accessibility_temp = CheckLSUAreaAccessPermission("R1", "W1", IPGPMTUM[3], NULL, NULL, trans);
            }
            lsu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "LVCI") {
            if (mVersion != "G3KH") {
                accessibility_temp = CheckLSUAreaAccessPermission("R0", "W0", IPGPMTUM[4], IPGPMTSV4[0], IPGPMTUM4[0], trans);
            } else {
                accessibility_temp = CheckLSUAreaAccessPermission("R0", "W0", IPGPMTUM[4], NULL, NULL, trans);
            }
            lsu_access.push_back(accessibility_temp);
        } else if (area_name[index] == "AXI") {
            if ((mVersion != "G3MH") && (mVersion != "G3KH")) {
                if ((mVersion == "G3M_V20")||(mVersion == "G3M_V30")) {
                    accessibility_temp = CheckLSUAreaAccessPermission("R1", "W1", IPGPMTUM[4], IPGPMTSV4[0], IPGPMTUM4[0], trans);
                } else {//if mVersion is G3M_V10 or G3K_PCU
                    accessibility_temp = CheckLSUAreaAccessPermission("R0", "W0", IPGPMTUM[0], IPGPMTSV0[0], IPGPMTUM0[0], trans);
                }
            } else {
                accessibility_temp = true;
            }
            lsu_access.push_back(accessibility_temp);
        } else {//it is not in guared area of IFU
            lsu_access.push_back(true);
        }
    }
    //If a area is not avaiable, the transaction is blocked
    for (unsigned int index = 0; index < lsu_access.size(); index++) {
        lsu_accessibility = lsu_accessibility & lsu_access[index];
    }
    if (!lsu_accessibility) {
        mErrPortValue->SysErrIPG = true;
        mErrPortValue->SysErrIPGUM = um;
        mErrPortValue->SysErrIPGVM = vm;
        mErrPortValue->SysErrIPGVMId = vcid;
        mIPG_AgentController->SendOutputErrorPort(*mErrPortValue); 
    }
    return lsu_accessibility;
}//}}}

///Check access permission for IFU each area
bool IPG_Func::CheckIFUAreaAccessPermission (const std::string control_bit, vpcl::re_register *pmtum_reg,
                                            vpcl::re_register *pmtsv0_reg, vpcl::re_register *pmtum0_reg, TlmBasicPayload& trans)
{//{{{
    bool ifu_accessibility = true;
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned char tcid = ExtensionInfo->getTcId();
    unsigned char peid = ExtensionInfo->getPeId();
    if ((!vm)&&um) {//Native-User mode
       if ((*IPGENUM)["E"] == 0x1) {
           if ((*IPGECRUM)["VD"] == 0x0) {
               if ((*pmtum_reg)[control_bit.c_str()] == 0x0) {
                   ifu_accessibility = false;
                   if ((*IPGENUM)["IRE"] == 0x1) {
                       (*IPGADRUM)["EADR"] = (unsigned int)trans.get_address();
                       (*IPGECRUM)["VD"] = 0x1;
                       (*IPGECRUM)["EX"] = 0x1;
                       if (mVersion != "G3KH") {
                           (*IPGECRUM)["DS"] = trans.get_data_length();
                           (*IPGECRUM)["TCID"] = (unsigned int) tcid;
                           if (mVersion == "G3M_V30") {
                               (*IPGECRUM)["PEID"] = (unsigned int) peid;
                           }
                       } else {//G3KH
                           switch (trans.get_data_length()) {
                               case 1://1 byte
                                   (*IPGECRUM)["BY"] = 1;
                                   break;
                               case 2://2 byte
                                   (*IPGECRUM)["HW"] = 1;
                                   break;
                               default://4 bytes
                                   (*IPGECRUM)["WD"] = 1;
                           }
                       }
                   }
               } else {
                   ifu_accessibility = true;
               }
           } else {
               ifu_accessibility = false;
           }
       } else {
           ifu_accessibility = true;
       }
    } else if (vm&&(!um)) {//Virtual-Suppervisor mode
        if (pmtsv0_reg != NULL) {//Whether register is supported in each version
           if (((*IPGENSV0)["E"] == 0x1)&&((*pmtsv0_reg)[control_bit.c_str()] == 0x0)) {
               ifu_accessibility = false;
               if (((*IPGENSV0)["IRE"] == 0x1) && ((*IPGECRSV0)["VD"] == 0x0)) {
                   (*IPGADRSV0)["EADR"] = (unsigned int)trans.get_address();
                   (*IPGECRSV0)["VD"] = 0x1;
                   (*IPGECRSV0)["EX"] = 0x1;
                   (*IPGECRSV0)["DS"] = trans.get_data_length();
                   if (mVersion != "G3MH") {
                       (*IPGECRSV0)["TCID"] = (unsigned int) tcid;
                       if (mVersion == "G3M_V30") {
                           (*IPGECRSV0)["PEID"] = (unsigned int) peid;
                       }
                   }
               }
           } else {
               ifu_accessibility = true;
           }
       } else {
           ifu_accessibility = true;
       }
    } else if (vm&&um) {//Virtual-User mode
        if (mVersion != "G3KH") {
            if ((*IPGENUM0)["E"] == 0x1) {
                if ((*IPGECRUM0)["VD"] == 0x0) {
                    if ((*pmtum0_reg)[control_bit.c_str()] == 0x0) {
                        ifu_accessibility = false;
                        if ((*IPGENUM0)["IRE"] == 0x1) {
                            (*IPGADRUM0)["EADR"] = (unsigned int)trans.get_address();
                            (*IPGECRUM0)["VD"] = 0x1;
                            (*IPGECRUM0)["EX"] = 0x1;
                            (*IPGECRUM0)["DS"] = trans.get_data_length();
                            if (mVersion != "G3MH") {
                                (*IPGECRUM0)["TCID"] = (unsigned int) tcid;
                                if (mVersion == "G3M_V30") {
                                    (*IPGECRUM0)["PEID"] = (unsigned int) peid;
                                }
                            }
                        }
                    } else {
                        ifu_accessibility = true;
                    }
                } else {
                    ifu_accessibility = false;
                }
            } else {
                ifu_accessibility = true;
            }
        } else {
            ifu_accessibility = true;
        }
    } else {//Native-Suppervisor mode
        ifu_accessibility = true;
    }
    return ifu_accessibility;
}//}}}

///Check access permission for LSU each area
bool IPG_Func::CheckLSUAreaAccessPermission (const std::string read_control_bit, const std::string write_control_bit,
                                            vpcl::re_register *pmtum_reg, vpcl::re_register *pmtsv0_reg,
                                            vpcl::re_register *pmtum0_reg, TlmBasicPayload& trans)
{//{{{
    bool lsu_accessibility = true;
    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned char tcid = ExtensionInfo->getTcId();
    unsigned char peid = ExtensionInfo->getPeId();
    if ((!vm)&&um) {//Native-User mode
        if ((*IPGENUM)["E"] == 0x1) {
            if ((*IPGECRUM)["VD"] == 0x0) {
                if ((((*pmtum_reg)[read_control_bit.c_str()] == 0x0)&&(trans.is_read()))||(((*pmtum_reg)[write_control_bit.c_str()] == 0x0)&&(trans.is_write()))) {
                    lsu_accessibility = false;
                    (*IPGADRUM)["EADR"] = (unsigned int)trans.get_address();
                    (*IPGECRUM)["VD"] = 0x1;
                    if (trans.is_read()) {
                        (*IPGECRUM)["RD"] = 0x1;
                    } else {
                        (*IPGECRUM)["WR"] = 0x1;
                    }
                    if (mVersion != "G3KH") {
                        (*IPGECRUM)["DS"] = trans.get_data_length();
                        if (mVersion != "G3MH") {
                            (*IPGECRUM)["TCID"] = (unsigned int) tcid;
                            if (mVersion == "G3M_V30") {
                                (*IPGECRUM)["PEID"] = (unsigned int) peid;
                            }
                        }
                    } else {//G3KH
                        switch (trans.get_data_length()) {
                            case 1://1 byte
                                (*IPGECRUM)["BY"] = 1;
                                break;
                            case 2://2 byte
                                (*IPGECRUM)["HW"] = 1;
                                break;
                            default://4 bytes
                                (*IPGECRUM)["WD"] = 1;
                        }
                    }
                } else {
                    lsu_accessibility = true;
                }
            } else {
                lsu_accessibility = false;
            }
        } else {
            lsu_accessibility = true;
        }
    } else if (vm&&(!um)) {//Virtual-Suppervisor mode
        if (pmtsv0_reg != NULL) {
            if (((*IPGENSV0)["E"] == 0x1)&&((((*pmtsv0_reg)[read_control_bit.c_str()] == 0x0)&&(trans.is_read()))||(((*pmtsv0_reg)[write_control_bit.c_str()] == 0x0)&&(trans.is_write())))) {
                lsu_accessibility = false;
                if ((*IPGECRSV0)["VD"] == 0x0) {
                    (*IPGADRSV0)["EADR"] = (unsigned int)trans.get_address();
                    (*IPGECRSV0)["VD"] = 0x1;
                    if (trans.is_read()) {
                        (*IPGECRSV0)["RD"] = 0x1;
                    } else {
                        (*IPGECRSV0)["WR"] = 0x1;
                    }
                    (*IPGECRSV0)["DS"] = trans.get_data_length();
                    if (mVersion != "G3MH") {
                        (*IPGECRSV0)["TCID"] = (unsigned int) tcid;
                        if (mVersion == "G3M_V30") {
                            (*IPGECRSV0)["PEID"] = (unsigned int) peid;
                        }
                    }
                }
            } else {
                lsu_accessibility = true;
            }
        } else {
            lsu_accessibility = true;
        }
    } else if (vm&&um) {//Virtual-User mode
        if (mVersion != "G3KH") {
            if ((*IPGENUM0)["E"] == 0x1) {
                if ((*IPGECRUM0)["VD"] == 0x0) {
                    if ((((*pmtum0_reg)[read_control_bit.c_str()] == 0x0)&&(trans.is_read()))||(((*pmtum0_reg)[write_control_bit.c_str()] == 0x0)&&(trans.is_write()))) {
                        lsu_accessibility = false;
                        (*IPGADRUM0)["EADR"] = (unsigned int)trans.get_address();
                        (*IPGECRUM0)["VD"] = 0x1;
                        if (trans.is_read()) {
                            (*IPGECRUM0)["RD"] = 0x1;
                        } else {
                            (*IPGECRUM0)["WR"] = 0x1;
                        }
                        (*IPGECRUM0)["DS"] = trans.get_data_length();
                        if (mVersion != "G3MH") {
                            (*IPGECRUM0)["TCID"] = (unsigned int) tcid;
                            if (mVersion == "G3M_V30") {
                                (*IPGECRUM0)["PEID"] = (unsigned int) peid;
                            }
                        }
                    } else {
                        lsu_accessibility = true;
                    }
                } else {
                    lsu_accessibility = false;
                }
            } else {
                lsu_accessibility = true;
            }
        } else {
            lsu_accessibility = true;
        }
    } else {//Native-Suppervisor mode
        lsu_accessibility = true;
    }
    return lsu_accessibility;
}//}}}
// vim600: set foldmethod=marker :
