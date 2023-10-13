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

void HAGE_Func::read(unsigned int offsetAddress
    , TlmBasicPayload& trans
    , BusTime_t* t
    , bool debug)
{
    mpAgentController->common_re_printf("info"
        , "Received read transaction addr 0x%08X"
        , (unsigned int) trans.get_address());
}

void HAGE_Func::write(unsigned int offsetAddress
    , TlmBasicPayload& trans
    , BusTime_t* t
    , bool debug)
{
    mpAgentController->common_re_printf("info"
        , "Received write transaction addr 0x%08X, data 0x%08X"
        , (unsigned int) trans.get_address()
        , (unsigned int) trans.get_data_ptr());
}

void HAGE_Func::cb_HAGECR_DMASTR(RegCBstr str)
{
}

void HAGE_Func::cb_HAGECR_PLL(RegCBstr str)
{
}

void HAGE_Func::cb_HAGECR_EINT_MSK(RegCBstr str)
{
}

void HAGE_Func::cb_HAGECR_HASHOUT(RegCBstr str)
{
}

void HAGE_Func::cb_HAGECR_MODSEL(RegCBstr str)
{
}

void HAGE_Func::cb_HAGECR_NEW_SEED(RegCBstr str)
{
}

void HAGE_Func::cb_HAGESEED_SEED(RegCBstr str)
{
}

void HAGE_Func::cb_HAGEIDAT_DATA_IN(RegCBstr str)
{
}

void HAGE_Func::cb_HAGEODAT_DATA_OUT(RegCBstr str)
{
}

void HAGE_Func::cb_HAGESRCADD_SRC_ADDR(RegCBstr str)
{
}

void HAGE_Func::cb_HAGEDSTADD_DST_ADDR(RegCBstr str)
{
}

void HAGE_Func::cb_HAGEDL_DATA_LEN(RegCBstr str)
{
}
