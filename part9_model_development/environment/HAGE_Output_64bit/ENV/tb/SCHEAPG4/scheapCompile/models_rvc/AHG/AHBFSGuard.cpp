// ---------------------------------------------------------------------
// $Id: AHBFSGuard.cpp,v 1.7 2014/09/25 09:05:41 dungvannguyen Exp $
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

#include <cstdarg>
#include "AHBFSGuard.h"

///Constructor of AHBFSGuard class
AHBFSGuard::AHBFSGuard(std::string name, 
                       AhgAgentController *AgentController_ptr,
                       const unsigned int version,
                       const bool lock_support,
                       const unsigned int filter_no):
                       Cahbfsguard_regif(name, 32, version, filter_no),
                       BusSlaveFuncIf()
{//{{{
    assert(AgentController_ptr != NULL);
    mAgentController = AgentController_ptr;
    mAgentController->CommandInit(name);
    Cahbfsguard_regif::set_instance_name(name);
    mVersion = version;
    mLockEnable = lock_support;
    mIsDefaultValue = true;
    mFilterInfo[0].is_enable = true;
    mFilterInfo[0].property = "RW";

    mFilterNo = filter_no;
    AreaInformation temp;
    temp.filter_id = 0;
    temp.start_addr = 0x0;
    temp.size = 0x100000000LL;
    mAreaInfo.push_back(temp);
    for (unsigned int index = 0; index < mFilterNo; index++) {
        unsigned int prot_value = emPROTDefaultOthers;
        if (mVersion == emCC_CUBE) {
            prot_value = emPROTDefaultCCCUBE;
            *(SPID[index]) = emSPIDDefault;
            SetRegInitValue(index, "P_SPID_RESET_VALUES", (unsigned int)(*(SPID[index])));
        }
        *(PROT[index]) = prot_value;
        SetRegInitValue(index, "P_RESET_VALUES", (unsigned int)(*(PROT[index])));
    }
}//}}}

///Destructor of AHBFSGuard class
AHBFSGuard::~AHBFSGuard()
{//{{{
}//}}}

///Reset all registers
void AHBFSGuard::Reset(const bool is_active)
{//{{{
    Cahbfsguard_regif::EnableReset(is_active);
}//}}}

///Check access ability to peripherals
bool AHBFSGuard::CheckAccessibility(TlmBasicPayload &trans, const bool debug, bool &is_legal_trans)
{//{{{
    bool status = false;
    bool peid_flt = false;
    bool vcid_flt = false;
    bool spid_flt = false;
    bool debug_flt = false;
    bool master_flt = false;
    bool is_debug = debug;
    unsigned int flt_id = 0;
    std::string trans_property;
    bool is_read = trans.is_read();
    bool is_write = trans.is_write();
    if ((is_write) && (!is_read)) {
        trans_property = "W";
    } else {
        trans_property = "R";
    }

    TlmG3mExtension *ExtensionInfo;
    trans.get_extension(ExtensionInfo);
    assert(ExtensionInfo != NULL);
    bool vm = ExtensionInfo->isVirtualMode();
    bool um = ExtensionInfo->isUserMode();
    unsigned int addr = (unsigned int)trans.get_address();
    for (unsigned int index = 0; index < mAreaInfo.size(); index++) {
        unsigned int f_id = mAreaInfo[index].filter_id;
        if (mFilterInfo[f_id].is_enable == true) {
            if ((mFilterInfo[f_id].property != "RW") && (mFilterInfo[f_id].property != trans_property)) {
                flt_id = 0x10;
            } else {
                if ((addr >= mAreaInfo[index].start_addr) && (addr <= (mAreaInfo[index].start_addr + mAreaInfo[index].size - 1))) {
                    flt_id = mAreaInfo[index].filter_id;
                    break;
                } else {
                    flt_id = 0x10;
                }
            }
        }
    }
    if (flt_id < 0x10) {
        is_legal_trans = true;
        if ((((*PROT[flt_id])["PEID"] >> ExtensionInfo->getPeId()) & 0x1) == 1) {
            peid_flt = true;
        }
 
        if ((((*PROT[flt_id])["VCID"] >> ExtensionInfo->getVmId()) & 0x1) == 1) {
            vcid_flt = true;
        }
        
        if (mVersion == emCC_CUBE) {
            if ((((*SPID[flt_id])["SPID"] >> ExtensionInfo->getSpId()) & 0x1) == 1) {
                spid_flt = true;
            }
        } else if (mVersion == emOTHERS) {
            if ((((*PROT[flt_id])["SPID"] >> ExtensionInfo->getSpId()) & 0x1) == 1) {
                spid_flt = true;
            }
        }

        if (is_debug == true) {
            if (((*PROT[flt_id])["DEB"] == 1) && (ExtensionInfo->getPeId() == 0)) {
                debug_flt = true;
            }
        } else {
            debug_flt = false;
        }

        if ((peid_flt == true) && (vcid_flt == true) && (spid_flt == true)) {
            if (((vm == false) && (um == false)) || /* native and supervisor mode */
                ((vm == true) && (um == false) && ((*PROT[flt_id])["VM"] == 1)) || /* virtual and supervisor mode */
                ((vm == false) && (um == true) && ((*PROT[flt_id])["UM"] == 1)) || /* native and user mode */
                ((vm == true) && (um == true) && ((*PROT[flt_id])["VM"] == 1) && ((*PROT[flt_id])["UM"] == 1))) { /* virtual and user mode */
                master_flt = true;
            }
        }
        if (mVersion == emCC_CUBE) {
            if (((trans_property == "R") && ((*PROT[flt_id])["RDPDEF"] == 0)) || /* default read access */
               ((trans_property == "W") && ((*PROT[flt_id])["WRPDEF"] == 0)) || /* default write access */
               ((trans_property == "R") && (master_flt == true) && (debug_flt == false) && ((*PROT[flt_id])["RD"] == 1) && ((*PROT[flt_id])["RDPDEF"] == 1)) || /* master read access */
               ((trans_property == "W") && (master_flt == true) && (debug_flt == false) && ((*PROT[flt_id])["WR"] == 1) && ((*PROT[flt_id])["WRPDEF"] == 1)) || /* master write access */
               (debug_flt == true)) /* debug access */{
                //access enable
                status = true;
            } else {
                //access disable
                status = false;
            }
        } else if (mVersion == emOTHERS) {
            if (((trans_property == "R") && ((*PROT[flt_id])["RDPDEF"] == 1)) || /* default read access */
               ((trans_property == "W") && ((*PROT[flt_id])["WRPDEF"] == 1)) || /* default write access */
               ((trans_property == "R") && (master_flt == true) && (debug_flt == false) && ((*PROT[flt_id])["RD"] == 1) && ((*PROT[flt_id])["RDPDEF"] == 0)) || /* master read access */
               ((trans_property == "W") && (master_flt == true) && (debug_flt == false) && ((*PROT[flt_id])["WR"] == 1) && ((*PROT[flt_id])["WRPDEF"] == 0)) || /* master write access */
               (debug_flt == true)) /* debug access */{
                //access enable
                status = true;
            } else {
                //access disable
                status = false;
            }
        }
        if(!status){
            mAgentController->print_msg("info","The transaction is guarded\n");
        }
    } else {
        mAgentController->print_msg("error","Address 0x%X is out of controlled-range of AHG.\n", addr);
        is_legal_trans = false;
        status = false;
    }
    return status;
}//}}}

///Set the information to filters
void AHBFSGuard::SetFilterInfo(const bool is_set,
                               const unsigned int id,
                               const unsigned int start_addr,
                               const long long size,
                               const std::string property)
{//{{{
    std::string space = " ";
    if (id < mFilterNo) {
        if (is_set) { //configure filter    
            bool valid = false;
            if ((size <= 0) || ((size + start_addr) > 0x100000000LL) || (size > 0x100000000LL) ){
                mAgentController->print_msg("error","Setting of filter %d is invalid.\n", id);
            } else {
                unsigned int end_addr = (unsigned int)(start_addr + size -1);
                for (unsigned int index0 = 0; index0 < mAreaInfo.size(); index0++) {
                    unsigned int f_id = mAreaInfo[index0].filter_id;
                    if (mFilterInfo[f_id].is_enable == true) {
                        if ((start_addr == mAreaInfo[index0].start_addr) && (size == mAreaInfo[index0].size)) {
                            if ((property == mFilterInfo[f_id].property) || (property == "RW") || (mFilterInfo[f_id].property == "RW")){
                                // ignore
                                mAgentController->print_msg("error","Property setting for filter %d is overlapped with property of filter %d.\n%47sFitler %d: start_addr = 0x%X, size = 0x%LX, property = %s.\n", 
                                                            id, index0, space.c_str(), index0, mAreaInfo[index0].start_addr, mAreaInfo[index0].size, mFilterInfo[f_id].property.c_str());
                                valid = false;
                                break;
                            } else {
                                valid = true;
                            }
                        } else {
                            unsigned int cmp_end_addr = (unsigned int)(mAreaInfo[index0].start_addr + mAreaInfo[index0].size - 1);
                            if ((mAreaInfo[index0].start_addr > end_addr ) || (cmp_end_addr < start_addr) || ((f_id == 0) && (mIsDefaultValue))) {
                                if ((f_id == 0) && (mIsDefaultValue)) {
                                    mIsDefaultValue = false;
                                    mAreaInfo.erase(mAreaInfo.begin());                                    
                                }
                                valid = true;
                            } else {
                                mAgentController->print_msg("error","Address setting for filter %d is overlaped with setting of filter %d.\n%47sFitler %d: start_addr = 0x%X, size = 0x%LX.\n", 
                                                            id, index0, space.c_str(), index0, mAreaInfo[index0].start_addr, mAreaInfo[index0].size);
                                valid = false;
                                break;
                            }
                        }
                    }
                }
            }
            if (valid) {
                AreaInformation temp(id, start_addr, size);
                mFilterInfo[id].is_enable = true;
                mFilterInfo[id].property = property;
                mAreaInfo.push_back(temp);
            } else {
                std::string status = mFilterInfo[id].is_enable ? "enable" : "disable";
                mAgentController->print_msg("info","Filter %d setting is not successful.\n", id);
            }
        } else { //get filter information
            std::string status;
            if (mFilterInfo[id].is_enable == true) {
                status = "enable";
            } else {
                status = "disable";
            }
            mAgentController->print_msg("info","+ Filter %d information\n", id);
            mAgentController->print_msg("info"," - Status        : %s\n", status.c_str());
            mAgentController->print_msg("info"," - Property      : %s\n", mFilterInfo[id].property.c_str());
            unsigned int count=0;
            for (unsigned int i=0; i<mAreaInfo.size(); i++) {
                if (mAreaInfo[i].filter_id == id) {
                    mAgentController->print_msg("info","+ Area %d information\n", count);
                    mAgentController->print_msg("info"," - Start address : 0x%X\n", mAreaInfo[i].start_addr);
                    mAgentController->print_msg("info"," - Size          : 0x%LX\n", mAreaInfo[i].size);
                    count++;
                }
            }
        }
    } else {
        mAgentController->print_msg("error","Invalid ID number: %d. ID number is from 0 to %d.\n", id, mFilterNo-1);
    }
}//}}}

///Dump the information of registers
void AHBFSGuard::DumpStatusInfo(void)
{//{{{
    mAgentController->print_msg("info","AHBFSGuard status information\n");
    for (unsigned int index = 0; index < mFilterNo; index++) {
        if (mFilterInfo[index].is_enable == true) {
            mAgentController->print_msg("info","+ Filter id     : %d\n", index);
            mAgentController->print_msg("info"," - Status        : enable\n");
            mAgentController->print_msg("info"," - Property      : %s\n", mFilterInfo[index].property.c_str());
            unsigned int count=0;
            for (unsigned int i=0; i<mAreaInfo.size(); i++) {
                if (mAreaInfo[i].filter_id == index) {
                    mAgentController->print_msg("info","+ Area %d information\n", count);
                    mAgentController->print_msg("info"," - Start address : 0x%X\n", mAreaInfo[i].start_addr);
                    mAgentController->print_msg("info"," - Size          : 0x%LX\n", mAreaInfo[i].size);
                    count++;
                }
            }
        } else {
            mAgentController->print_msg("info","+ Filter id     : %d\n", index);
            mAgentController->print_msg("info"," - Status        : disable\n");
        }
    }
}//}}}

///Set parameters and commands to ahbfsguard_regif class
std::string AHBFSGuard::RegisterHandler(const std::vector<std::string> &args)
{//{{{
    std::string ret = "";
    if ((args.size() == 1) && (args[0] == "reg")) { // list registers
        printf("List registers of AHBFSGUARD\n");
        unsigned int reg_no = 0;
        while (reg_no < mFilterNo) {
            std::ostringstream reg_name;
            reg_name << "PROT" << reg_no;
            printf("%s\n", reg_name.str().c_str());
            reg_no++;
        }
        if (mVersion == emCC_CUBE) {
            reg_no = 0;
            while (reg_no < mFilterNo) {
                std::ostringstream reg_name;
                reg_name << "SPID" << reg_no;
                printf("%s\n", reg_name.str().c_str());
                reg_no++;
            }
        }
    } else {
        bool dump_msg = true;
        for (unsigned int index = 0; index < args.size(); index++) {
            if ((args[index] == "MessageLevel") || (args[index] == "DumpFileNameLineNum") || (args[index] == "DumpRegisterRW")) {
                dump_msg = false;
            }
        }
        ret = reg_handle_command(args);
        if ((ret != "") && dump_msg) {
            printf("%s\n",ret.c_str());
        }
    }
    return ret;
}//}}}

///Virtual function of BusSlaveFuncIf class
void AHBFSGuard::read( unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void AHBFSGuard::write( unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Virtual function of ahbfsguard_regif class
void AHBFSGuard::cb_PROT_LOCK(RegCBstr str)
{//{{{
    if (mLockEnable == false) {
        if ((*PROT[str.channel])["LOCK"] == 1) {
            mAgentController->print_msg("warning","LOCK function is not enabled yet.\n");
            (*PROT[str.channel])["LOCK"] = 0;
        }
    } else {
        if ((unsigned int)(str.pre_data >> 31) == 0) {
            if ((*PROT[str.channel])["LOCK"] == 1) {
                mAgentController->print_msg("info","PROT%d register is locked.\n", (unsigned int)(str.channel));
            }
        } else {
            (*PROT[str.channel]) = str.pre_data;
            mAgentController->print_msg("warning","Cannot write to PROT%d register while LOCK bit of this register is 1.\n", (unsigned int)(str.channel));
        }
    }
}//}}}

///Get initial value of PROT and SPID registers from Python IF
void AHBFSGuard::SetRegInitValue(const unsigned int filter_id, const std::string reg_name, const unsigned int init_val)
{//{{{
    if (reg_name == "P_RESET_VALUES") {
        PROT[filter_id]->UpdateBitInitValue("LOCK",((init_val >> 31) & 0x1)) ;
        PROT[filter_id]->UpdateBitInitValue("VM",((init_val >> 26) & 0x1)) ;
        PROT[filter_id]->UpdateBitInitValue("UM",((init_val >> 25) & 0x1)) ;
        PROT[filter_id]->UpdateBitInitValue("PEID",((init_val >> 17) & 0xFF)) ;
        PROT[filter_id]->UpdateBitInitValue("VCID",((init_val >> 9) & 0xFF)) ;
        if (mVersion == emOTHERS) {
            PROT[filter_id]->UpdateBitInitValue("SPID",((init_val >> 5) & 0xF)) ;
        }
        PROT[filter_id]->UpdateBitInitValue("DEB",((init_val >> 4) & 0x1)) ;
        PROT[filter_id]->UpdateBitInitValue("RDPDEF",((init_val >> 3) & 0x1)) ;
        PROT[filter_id]->UpdateBitInitValue("WRPDEF",((init_val >> 2) & 0x1)) ;
        PROT[filter_id]->UpdateBitInitValue("RD",((init_val >> 1) & 0x1)) ;
        PROT[filter_id]->UpdateBitInitValue("WR",(init_val & 0x1)) ;
    
        *PROT[filter_id] = init_val;
    } else {
        if (mVersion == emCC_CUBE) {
            SPID[filter_id]->UpdateBitInitValue("SPID",init_val) ;
            *SPID[filter_id] = init_val;
        }
    }
}//}}}

// vim600: set foldmethod=marker :
