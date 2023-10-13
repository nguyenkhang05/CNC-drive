/*************************************************************************
*
*  HAGE.cpp
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

#include "HAGE.h"
#include "HAGE_Func.h"

HAGE::HAGE(sc_module_name name
    , const char* configFile
    , const unsigned int rLatency
    , const unsigned int wLatency
    , const SIM_MODE_T simMode):
    sc_module(name)
    , BusSlaveBase<BUS_WIDTH_APB, 1>()
    , HAGE_AgentController((std::string) name)
    , mSimMode(simMode)
    , PCLK("PCLK")
    , PRESET("PRESET")
{
    sc_assert(NULL != configFile);

    mpHAGE_Func = new HAGE_Func((std::string) name, this);
    sc_assert(NULL != mpHAGE_Func);

    setSlaveResetPort32(&PRESET);
    setSlaveFreqPort32(&PCLK);
    setTargetSocket32((char*)"ts");
    ts = BusSlaveBase<BUS_WIDTH_APB, 1>::tSocket32[0];
    mBusSlaveIf32[0]->setFuncModulePtr((BusSlaveFuncIf *) mpHAGE_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    SC_METHOD(HandleClkMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandleResetMethod);
    dont_initialize();
    sensitive << PRESET;
}

HAGE::~HAGE(void)
{
    delete mpHAGE_Func;
}

void HAGE::HandleClkMethod()
{
    common_re_printf("info"
        , "PLCK change to %llu (Hz)\n"
        , (unsigned long long) PCLK.read());
}

void HAGE::HandleResetMethod()
{
    common_re_printf("info"
        , "PRESET change to %d\n"
        , (unsigned int) PRESET.read());
}

void HAGE::SetMessageLevel(std::string msg_lv)
{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    HandleCommand(cmd);
}
