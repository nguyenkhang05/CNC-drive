/*************************************************************************
*
*  HAGE.h
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

#ifndef __HAGE_H__
#define __HAGE_H__

#include "HAGE_AgentController.h"

class HAGE_Func;

class HAGE : public sc_module
    , public BusSlaveBase<BUS_WIDTH_APB, 1>
    , public HAGE_AgentController
{
public:
    SC_HAS_PROCESS(HAGE);

    sc_in<sc_dt::uint64> PCLK;
    sc_in<bool> PRESET;

    TlmTargetSocket<BUS_WIDTH_APB>* ts;

    HAGE(sc_module_name name
        , const char* configFile
        , const unsigned int rLatency
        , const unsigned int wLatency
        , const SIM_MODE_T simMode);
    ~HAGE(void);

    /* Python API */
    void SetMessageLevel(std::string msg_lv);

private:
    SIM_MODE_T mSimMode;
    HAGE_Func *mpHAGE_Func;

    void HandleClkMethod();
    void HandleResetMethod();
};

#endif /* __HAGE_H__ */
