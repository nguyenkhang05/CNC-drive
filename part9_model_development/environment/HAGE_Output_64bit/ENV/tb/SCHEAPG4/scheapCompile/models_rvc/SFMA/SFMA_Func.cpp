// ---------------------------------------------------------------------
// $Id: SFMA_Func.cpp,v 1.3 2020/01/30 08:01:36 huepham Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "global.h"
#include "SFMA_Func.h"

//========================================================
//============ Constructor/Destructor/Common APIs=========
//========================================================
/// Constructor
SFMA_Func::SFMA_Func (std::string name, SFMA_AgentController* SFMAAgentController):
                Csfma_regif(name, 32)
                ,BusSlaveFuncIf()
{//{{{
    assert(SFMAAgentController != NULL);
    mpSFMAAgentController = SFMAAgentController;
    mpSFMAAgentController->CommandInit(name);
    mModuleName = name;

    Csfma_regif::set_instance_name(name);
    Initialize();
}//}}}

/// Destructor
SFMA_Func::~SFMA_Func()
{//{{{
}//}}}

/// Reset Func and registers
void SFMA_Func::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        Initialize();
    }

    Csfma_regif::EnableReset(is_active);
}//}}}

/// Initialize internal variables
void SFMA_Func::Initialize(void)
{//{{{
}//}}}

/// Read API (target socket)
void SFMA_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    
    // Zero clock or reset active
    if ((false == mpSFMAAgentController->CheckClockPeriod())
         || (true == mpSFMAAgentController->GetResetStatus())) {
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

    unsigned int maskAddress = (unsigned int)trans.get_address() & (unsigned int)emRegAddrMask;
    unsigned int size = (unsigned int)trans.get_data_length();
    memset(trans.get_data_ptr(), 0, size);

    // SFMASMRDR is readable only when TEND is 1
    if (((maskAddress >= SFMASMRDR->addr()) && (maskAddress < (SFMASMRDR->addr() + 4)))
            && (0 == (unsigned int)(*SFMACMNSR)["TEND"])) {
        re_printf("warning", "Cannot read SFMASMRDR register when TEND is 0\n");
        return;
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Write API (target socket)
void SFMA_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    unsigned int maskAddress = (unsigned int)trans.get_address() & (unsigned int)emRegAddrMask;
    unsigned int size = (unsigned int)trans.get_data_length();

    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    
    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (false == mpSFMAAgentController->CheckClockPeriod()) {
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

    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/// Handler for register IF
void SFMA_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Csfma_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Csfma_regif class
            msg_return = Csfma_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

/// Dump value of status registers
void SFMA_Func::DumpStatusInfo(void)
{//{{{
    printf("Status information of registers:\n");
    printf("    SFMACMNSR = %08X\n", (unsigned int)(*SFMACMNSR));
}//}}}

/// Return bit field value
unsigned int SFMA_Func::GetBitField(const std::string regName, const std::string bitName)
{//{{{ 
    unsigned int retValue = 0;

    if("SFMACMNCR" == regName) {
        retValue = (unsigned int)(*SFMACMNCR)[bitName.c_str()];
    } else if("SFMASSLDR" == regName) {
        retValue = (unsigned int)(*SFMASSLDR)[bitName.c_str()];
    } else if("SFMADRCR" == regName) {
        retValue = (unsigned int)(*SFMADRCR)[bitName.c_str()];
    } else if("SFMADRCMR" == regName) {
        retValue = (unsigned int)(*SFMADRCMR)[bitName.c_str()];
    } else if("SFMADREAR" == regName) {
        retValue = (unsigned int)(*SFMADREAR)[bitName.c_str()];
    } else if("SFMADROPR" == regName) {
        retValue = (unsigned int)(*SFMADROPR)[bitName.c_str()];
    } else if("SFMADRENR" == regName) {
        retValue = (unsigned int)(*SFMADRENR)[bitName.c_str()];
    } else if("SFMASMCR" == regName) {
        retValue = (unsigned int)(*SFMASMCR)[bitName.c_str()];
    } else if("SFMASMCMR" == regName) {
        retValue = (unsigned int)(*SFMASMCMR)[bitName.c_str()];
    } else if("SFMASMADR" == regName) {
        retValue = (unsigned int)(*SFMASMADR)[bitName.c_str()];
    } else if("SFMASMOPR" == regName) {
        retValue = (unsigned int)(*SFMASMOPR)[bitName.c_str()];
    } else if("SFMASMENR" == regName) {
        retValue = (unsigned int)(*SFMASMENR)[bitName.c_str()];
    } else if("SFMADRDMCR" == regName) {
        retValue = (unsigned int)(*SFMADRDMCR)[bitName.c_str()];
    } else if("SFMASMDMCR" == regName) {
        retValue = (unsigned int)(*SFMASMDMCR)[bitName.c_str()];
    } else if("SFMACMNSR" == regName) {
        retValue = (unsigned int)(*SFMACMNSR)[bitName.c_str()];
    } else if("SFMASPBCR" == regName) {
        retValue = (unsigned int)(*SFMASPBCR)[bitName.c_str()];
    } else if("SFMASMWDR" == regName) {
        retValue = (unsigned int)(*SFMASMWDR)[bitName.c_str()];
    }

    return retValue;
}//}}} 

/// Set bit field value
void SFMA_Func::SetBitField(const std::string regName, const std::string bitName, unsigned int value)
{//{{{
    if ("SFMASMRDR" == regName) {
        (*SFMASMRDR)[bitName.c_str()] = value;
        // Keep preceding value when reset
        SFMASMRDR->UpdateBitInitValue(bitName.c_str(), value);
    } else if ("SFMACMNSR" == regName) {
        (*SFMACMNSR)[bitName.c_str()] = value;
    }
}//}}}

//========================================================
//============ Callback function  ========================
//========================================================

/// Callback function of SFMACMNCR_SSLP bit
void SFMA_Func::cb_SFMACMNCR_SSLP(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMACMNCR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMASSLDR_SPNDL bit
void SFMA_Func::cb_SFMASSLDR_SPNDL(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASSLDR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMASPBCR_SPBR bit
void SFMA_Func::cb_SFMASPBCR_SPBR(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASPBCR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMADRCR_SSLN bit
void SFMA_Func::cb_SFMADRCR_SSLN(RegCBstr str)
{//{{{
    // Writing of SSLN bit is not depended on TEND bit
    if(1 == (unsigned int)(*SFMADRCR)["SSLN"]) {
        // Set TEND = 1 when writing 1 to SSLN and RBE = 1, SSLE = 1
        if ((1 == (unsigned int)(*SFMADRCR)["RBE"]) && (1 == (unsigned int)(*SFMADRCR)["SSLE"])) {
            mpSFMAAgentController->TransferEnd();
        }
        // Always read as 0
        (*SFMADRCR)["SSLN"] = 0;
    }

    // Not changeable if TEND = 0
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMADRCR) = str.pre_data;
    }

    if(1 == (unsigned int)(*SFMADRCR)["RCF"]) {
        // Flush cache
        mpSFMAAgentController->FlushCache();
        // Always read as 0
        (*SFMADRCR)["RCF"] = 0;
    }
}//}}}

/// Callback function of SFMADRCMR_CMD bit
void SFMA_Func::cb_SFMADRCMR_CMD(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMADRCMR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMADREAR_EAV bit
void SFMA_Func::cb_SFMADREAR_EAV(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMADREAR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMADROPR_OPD3 bit
void SFMA_Func::cb_SFMADROPR_OPD3(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMADROPR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMADRENR_CDB bit
void SFMA_Func::cb_SFMADRENR_CDB(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMADRENR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMASMCR_SPIE bit
void SFMA_Func::cb_SFMASMCR_SPIE(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASMCR) = str.pre_data;
    }

    if(1 == (unsigned int)(*SFMASMCR)["SPIE"]) {
        // Start SPI mode operating communication
        if (false == mpSFMAAgentController->IsSPIModeSettingProhibited()) {
            mpSFMAAgentController->SPIModeCommunicate();
        }
        // Always read as 0
        (*SFMASMCR)["SPIE"] = 0;
    }
}//}}}

/// Callback function of SFMASMCMR_CMD bit
void SFMA_Func::cb_SFMASMCMR_CMD(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASMCMR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMASMADR_ADR bit
void SFMA_Func::cb_SFMASMADR_ADR(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASMADR) = str.pre_data;
    }
}//}}}  

/// Callback function of SFMASMOPR_OPD3 bit
void SFMA_Func::cb_SFMASMOPR_OPD3(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASMOPR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMASMENR_CDB bit
void SFMA_Func::cb_SFMASMENR_CDB(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASMENR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMASMWDR_WDATA bit
void SFMA_Func::cb_SFMASMWDR_WDATA(RegCBstr str)
{//{{{
    // Checking write size for register SFMASMWDR
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASMWDR) = str.pre_data;
    } else {
        if (((emNotTransferData == (unsigned int)(*SFMASMENR)["SPIDE"]) && (0 != str.size)) // 0 byte
                || ((emOneByteData == (unsigned int)(*SFMASMENR)["SPIDE"]) && (1 != str.size)) // 1 byte
                || ((emTwoBytesData == (unsigned int)(*SFMASMENR)["SPIDE"]) && (2 != str.size)) // 2 bytes
                || ((emFourBytesData == (unsigned int)(*SFMASMENR)["SPIDE"]) && (4 != str.size))) { // 4 bytes
            re_printf("warning", "Write size of register SFMASMWDR is not the same as transfer size in SPIDE\n");
            (*SFMASMWDR) = str.pre_data;
        } else if ((emNotTransferData != (unsigned int)(*SFMASMENR)["SPIDE"])
                && (emOneByteData != (unsigned int)(*SFMASMENR)["SPIDE"])
                && (emTwoBytesData != (unsigned int)(*SFMASMENR)["SPIDE"])
                && (emFourBytesData != (unsigned int)(*SFMASMENR)["SPIDE"])) {
            re_printf("warning", "Cannot write register SFMASMWDR because the setting of SPIDE bit is prohibited\n");
            (*SFMASMWDR) = str.pre_data;
        }
    }
}//}}}

/// Callback function of SFMADRDMCR_DMDB bit
void SFMA_Func::cb_SFMADRDMCR_DMDB(RegCBstr str)
{//{{{
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMADRDMCR) = str.pre_data;
    }
}//}}}

/// Callback function of SFMASMDMCR_DMDB bit
void SFMA_Func::cb_SFMASMDMCR_DMDB(RegCBstr str)
{//{{{ 
    if(0 == (unsigned int)(*SFMACMNSR)["TEND"]) {
        (*SFMASMDMCR) = str.pre_data;
    }
}//}}} 

// vim600: set foldmethod=marker :
