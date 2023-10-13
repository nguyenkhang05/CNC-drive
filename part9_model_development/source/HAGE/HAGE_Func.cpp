/*************************************************************************
*
*  HAGE_Func.cpp
*
* Copyright(c) 2018-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
*************************************************************************/

#include "HAGE_Func.h"

HAGE_Func::HAGE_Func(std::string name
    , HAGE_AgentController* agent_controller):
    mModuleName(name)
    , Chage_regif(name, 32)
    , BusSlaveFuncIf()
{
    sc_assert(NULL != agent_controller);
    mpAgentController = agent_controller;
}

HAGE_Func::~HAGE_Func(void)
{

}

uint32_t HAGE_Func::GetDATAIN()
{
    return (uint32_t)(*HAGEIDAT)["DATA_IN"];
}

uint32_t HAGE_Func::GetSEEDDATA()
{
    return (uint32_t)(*HAGESEED)["SEED"];
}

uint32_t HAGE_Func::GetSRCADD()
{
    return (uint32_t)(*HAGESRCADD)["SRC_ADDR"] >> 2;
}

uint32_t HAGE_Func::GetDSTADD()
{
    return (uint32_t)(*HAGEDSTADD)["DST_ADDR"] >> 2;
}

uint32_t HAGE_Func::GetDATALEN()
{
    return (uint32_t)(*HAGEDL)["DATA_LEN"];
}

uint32_t HAGE_Func::GetHAGECR(std::string bitname)
{
    return (uint32_t)(*HAGECR)[bitname.c_str()];
}

uint32_t HAGE_Func::GetPLL()
{
    return (uint32_t)(*HAGECR)["PLL"];
}

void HAGE_Func::SetValue(std::string regname, std::string bitname, uint32_t value)
{
    if (regname == "HAGEODAT") {
        if (bitname == "DATA_OUT")
            (*HAGEODAT)["DATA_OUT"] = value;
        else
            std::cout << "Invalid bitname for HAGEODAT register" << std::endl;
    }
    else if (regname == "HAGECR") {
        if (bitname == "NEW_SEED")
            (*HAGECR)["NEW_SEED"] = value;
        else if (bitname == "DMASTR")
            (*HAGECR)["DMASTR"] = value;
    }
    else
        std::cout << "Invalid regname" << std::endl;
}

void HAGE_Func::read(unsigned int offsetAddress
    , TlmBasicPayload& trans
    , BusTime_t* t
    , bool debug)
{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (!debug) {
        reg_rd(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
    }
    else {
        reg_rd_dbg(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
    }
    mpAgentController->common_re_printf("info"
        , "Received read transaction addr 0x%08X, data 0x%08X, length 0x%08X"
        , (unsigned int)trans.get_address()
        , (unsigned int)trans.get_data_ptr()
        , (unsigned int)trans.get_data_length());

}

void HAGE_Func::write(unsigned int offsetAddress
    , TlmBasicPayload& trans
    , BusTime_t* t
    , bool debug)
{
    if (!debug) {
        reg_wr(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
    }
    else {
        reg_wr_dbg(offsetAddress, trans.get_data_ptr(), trans.get_data_length());
    }
    mpAgentController->common_re_printf("info"
        , "Received write transaction addr 0x%08X, data 0x%08X, length 0x%08X"
        , (unsigned int) trans.get_address()
        , (unsigned int) trans.get_data_ptr()
        , (unsigned int) trans.get_data_length());

}

// Callback when writing to HAGECR_DMASTR
void HAGE_Func::cb_HAGECR_DMASTR(RegCBstr str)
{
    mpAgentController->common_re_printf("info",
        "Call back HAGECR_DMASTR");
    if (GetHAGECR("DMASTR") != 0)
        this->mpAgentController->StartDMA();
    else
        this->mpAgentController->StopDMA();
}

void HAGE_Func::cb_HAGEIDAT_DATA_IN(RegCBstr str)
{
}

