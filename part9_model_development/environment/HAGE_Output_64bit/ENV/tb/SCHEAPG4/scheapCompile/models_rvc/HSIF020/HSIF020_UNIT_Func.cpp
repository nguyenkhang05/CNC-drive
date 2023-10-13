// ---------------------------------------------------------------------
// $Id: HSIF020_UNIT_Func.cpp,v 1.6 2020/04/08 07:39:39 quynhnhunguyen Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "HSIF020_UNIT_Func.h"
#include "HSIF_Common.h"

///Constructor of HSIF020_UNIT_Func class
HSIF020_UNIT_Func::HSIF020_UNIT_Func (std::string name
                                    , HSIF020_UNIT_AgentController *iHSIF020_UNIT_AgentControllerPtr):
Chsif020_unit_regif(name, 64 )
{//{{{
    assert(iHSIF020_UNIT_AgentControllerPtr != NULL);
    mHSIF020_UNIT_AgentController = iHSIF020_UNIT_AgentControllerPtr;
    mHSIF020_UNIT_AgentController->CommandInit(name);
    Chsif020_unit_regif::set_instance_name(name);
    mModuleName = name;
    
    // Initialize variables
    Initialize();

}//}}}

/// Initialize variable
void HSIF020_UNIT_Func::Initialize(void)
{//{{{

}//}}}

///Destructor of HSIF020_UNIT_Func class
HSIF020_UNIT_Func::~HSIF020_UNIT_Func(void)
{//{{{

}//}}}

void HSIF020_UNIT_Func::read(uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_rd((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

void HSIF020_UNIT_Func::write (uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    if (!debug) {
        reg_wr((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

///Reset operation of HSIF020_UNIT_Func class
void HSIF020_UNIT_Func::Reset (const bool is_active)
{//{{{
    Chsif020_unit_regif::EnableReset(is_active);
}//}}}

///Register handler command to Chsif020_unit_regif class
void HSIF020_UNIT_Func::RegisterHandler (const std::vector<std::string> &args)
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
                msg_return = Chsif020_unit_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Chsif020_unit_regif class
            msg_return = Chsif020_unit_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

bool HSIF020_UNIT_Func::IsChTxRxDis(void)
{//{{{
    if ((((*HSIFnCMD)["TNME"]) == 0x0) && (((*HSIFnCMD)["INME"]) == 0x0))
        return true;
    else
        return false;
}//}}}

bool HSIF020_UNIT_Func::IsChRxEnable(void)
{//{{{
    if (((*HSIFnCMD)["TNME"]) == 0x1)
        /// Enables reception of Read, Write, Event, ID, and Stream Commands.
        return true;
    else
        return false;
}//}}}

bool HSIF020_UNIT_Func::IsChTxEnable(void)
{//{{{
    if (((*HSIFnCMD)["INME"]) == 0x1)
        /// Enables transmission of Read, Write, Event, ID, and Stream Commands.
        return true;
    else
        return false;
}//}}}

uint32_t HSIF020_UNIT_Func::GetRWAddr(void)
{//{{{
    return (uint32_t)(*HSIFnCAR);
}//}}}

uint32_t HSIF020_UNIT_Func::GetWData(void)
{//{{{
    return (uint32_t)(*HSIFnCWD);
}//}}}

void HSIF020_UNIT_Func::SetChannelStatus(uint32_t bit_name)
{//{{{
    switch(bit_name){
        case emRAR: { (*HSIFnCST)["RAR"] = 0x1; break;}
        case emAKR: { (*HSIFnCST)["AKR"] = 0x1; break;}
        case emAKE: { (*HSIFnCST)["AKE"] = 0x1; break;}
        case emTOE: { (*HSIFnCST)["TOE"] = 0x1; break;}
        case emIDE: { (*HSIFnCST)["IDE"] = 0x1; break;}
        case emAOE: { (*HSIFnCST)["AOE"] = 0x1; break;}
        case emRDY: { (*HSIFnCST)["RDY"] = 0x1; break;}
        case emTER: { (*HSIFnCST)["TER"] = 0x1; break;}
        case emBRE: { (*HSIFnCST)["BRE"] = 0x1; break;}
        case emCRE: { (*HSIFnCST)["CRE"] = 0x1; break;}
        default:    { break;}
    }
}//}}}

void HSIF020_UNIT_Func::ClearChannelStatus(uint32_t bit_name)
{//{{{
    if (bit_name == emRDY) { 
        (*HSIFnCST)["RDY"] = 0x0;
    }
}//}}}

bool HSIF020_UNIT_Func::GetChannelStatus(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emRAR: { temp = (bool)((*HSIFnCST)["RAR"]); break;}
        case emAKR: { temp = (bool)((*HSIFnCST)["AKR"]); break;}
        case emAKE: { temp = (bool)((*HSIFnCST)["AKE"]); break;}
        case emTOE: { temp = (bool)((*HSIFnCST)["TOE"]); break;}
        case emIDE: { temp = (bool)((*HSIFnCST)["IDE"]); break;}
        case emAOE: { temp = (bool)((*HSIFnCST)["AOE"]); break;}
        case emRDY: { temp = (bool)((*HSIFnCST)["RDY"]); break;}
        case emTER: { temp = (bool)((*HSIFnCST)["TER"]); break;}
        case emBRE: { temp = (bool)((*HSIFnCST)["BRE"]); break;}
        case emCRE: { temp = (bool)((*HSIFnCST)["CRE"]); break;}
        default:    { temp = false;                      break;}
    }
    return temp;
}//}}}

bool HSIF020_UNIT_Func::GetChIntEnable(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emRARE: {temp = (bool)((*HSIFnCIE)["RARE"]); break;}
        case emAKRE: {temp = (bool)((*HSIFnCIE)["AKRE"]); break;}
        case emAKEE: {temp = (bool)((*HSIFnCIE)["AKEE"]); break;}
        case emTOEE: {temp = (bool)((*HSIFnCIE)["TOEE"]); break;}
        case emIDEE: {temp = (bool)((*HSIFnCIE)["IDEE"]); break;}
        case emAOEE: {temp = (bool)((*HSIFnCIE)["AOEE"]); break;}
        case emTERE: {temp = (bool)((*HSIFnCIE)["TERE"]); break;}
        case emBREE: {temp = (bool)((*HSIFnCIE)["BREE"]); break;}
        case emCREE: {temp = (bool)((*HSIFnCIE)["CREE"]); break;}
        default:     { temp = false;                      break;}
    }
    return temp;
}//}}}

void HSIF020_UNIT_Func::SetReadData(uint32_t readData)
{//{{{
    if (((*HSIFnCST)["RAR"] & 0x1) == 0x1) {
        uint8_t tFramInfo = mHSIF020_UNIT_AgentController->GetCurrentFramInfo();
        if ( tFramInfo == emReadCmd8){
            (*HSIFnCRD) = readData & 0xFF;
        } else if ( tFramInfo == emReadCmd16){
            (*HSIFnCRD) = readData & 0xFFFF;
        } else if ( tFramInfo == emReadCmd32){
            (*HSIFnCRD) = readData & 0xFFFFFFFF;
        } else { //  tFramInfo == emIdCmd
            (*HSIFnCRD) = readData & 0xFFFFFFFF;
        }
    }
}//}}}

uint32_t HSIF020_UNIT_Func::GetHSIFnCST(void)
{//{{{
    return (uint32_t)(*HSIFnCST);
}//}}}

void HSIF020_UNIT_Func::cb_HSIFnCMD_TNME(RegCBstr str)
{//{{{
    uint32_t timeoutMRT = mHSIF020_UNIT_AgentController->GetTimeOutSetting();
    bool isZeroClk = mHSIF020_UNIT_AgentController->CheckClockPeriod("CCLK");
    if ((timeoutMRT != 0) && isZeroClk ){
        /// Enable initiator node function.
        if (str.is_wr && (((str.data) & 0x1) == 0x1)){
            (*HSIFnCST)["RDY"] = 0x1;
        }
    } else {
        if (timeoutMRT == 0){
            (*HSIFnCMD)["INME"] = 0x0;
            re_printf("warning","When 0 is specified in these HSIFnMRT.RCNT bits, the reply timeout does not occur.");
            re_printf("warning","The initiator on the channel is disabled.");
        }
    }
    if ((str.is_wr && ((str.data) & 0x1) == 0x0) && isZeroClk ){
        if (!mHSIF020_UNIT_AgentController->IsChannelBusy()){
            (*HSIFnCST)["RDY"] = 0x0;
        } else {
            (*HSIFnCMD) = str.pre_data;
            re_printf("warning","Setting HSIFnCMD.TNME must not be cleared to 0 while L2 is transmitting data or waiting for a response.");
        }
    }
}//}}}

void HSIF020_UNIT_Func::cb_HSIFnCCT_CTYW(RegCBstr str)
{//{{{
    if (IsChTxEnable() && mHSIF020_UNIT_AgentController->CheckClockPeriod("CCLK")){
        if (GetChannelStatus(emRDY) == true){
            uint32_t tempCTY = str.data & 0x1F;
            if (((str.data >> 7) & 0x1) == 0x1){
                if (((tempCTY == emReadCmd8) | (tempCTY == emReadCmd16) | (tempCTY == emReadCmd32)
                  | (tempCTY == emWriteCmd8)| (tempCTY == emWriteCmd16)| (tempCTY == emWriteCmd32)
                  | (tempCTY == emEventCmd) | (tempCTY == emIdCmd)) != 0){
                    (*HSIFnCCT)["CTY"] = tempCTY;
                    //Notify the event to start Non-stream Command
                    mHSIF020_UNIT_AgentController->NotifyStartNonStreamEvent(tempCTY);
                    mHSIF020_UNIT_AgentController->SetInitialSettingPhase(false);
                } else {
                    re_printf("warning","Can not set RHSIFnCCT.CTY. This setting is un-valid.");
                }
            } else {
                re_printf("warning","Can not set  RHSIFnCCT.CTY while RHSIFnCCT.CTYW is set 0.");
            }
        } else {
            re_printf("warning","Model is transferring or Waiting reply time-out. Can not send other command.");
        }
    } else {
        if (!IsChTxEnable()){
            re_printf("warning","Tx is disable.Can not send command.");
        }
    }

    (*HSIFnCCT) = 0;    //Read always 0 - un-save value
}//}}}

void HSIF020_UNIT_Func::cb_HSIFnCSC_CREC(RegCBstr str)
{//{{{
    if ((*HSIFnCSC)["CREC"] == 1) {
        (*HSIFnCST)["CRE"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emCRE);
    }
    if ((*HSIFnCSC)["BREC"] == 1) {
        (*HSIFnCST)["BRE"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emBRE);
    }
    if ((*HSIFnCSC)["TERC"] == 1) {
        (*HSIFnCST)["TER"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emTER);
    }
    if ((*HSIFnCSC)["AOEC"] == 1) {
        (*HSIFnCST)["AOE"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emAOE);
    }
    if ((*HSIFnCSC)["IDEC"] == 1) {
        (*HSIFnCST)["IDE"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emIDE);
    }
    if ((*HSIFnCSC)["TOEC"] == 1) {
        (*HSIFnCST)["TOE"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emTOE);
    }
    if ((*HSIFnCSC)["AKEC"] == 1) {
        (*HSIFnCST)["AKE"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emAKE);
    }
    if ((*HSIFnCSC)["AKRC"] == 1) {
        (*HSIFnCST)["AKR"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emAKR);
    }
    if ((*HSIFnCSC)["RARC"] == 1) {
        (*HSIFnCST)["RAR"] = 0;
        mHSIF020_UNIT_AgentController->SetInterruptStatus(emRAR);
    }
}//}}}

void HSIF020_UNIT_Func::cb_HSIFnCIE_CREE(RegCBstr str){;}
void HSIF020_UNIT_Func::cb_HSIFnCAR_CAR(RegCBstr str){;}
void HSIF020_UNIT_Func::cb_HSIFnCWD_CWD(RegCBstr str){;}
void HSIF020_UNIT_Func::cb_HSIFnCRD_CRD(RegCBstr str){;}

// vim600: set foldmethod=marker :
