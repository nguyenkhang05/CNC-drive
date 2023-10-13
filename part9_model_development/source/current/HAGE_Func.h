/*************************************************************************
*
*  HAGE_Func.h
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

#ifndef __HAGE_FUNC_H__
#define __HAGE_FUNC_H__

#include "HAGE_AgentController.h"
#include "hage_regif.h"

class HAGE_Func : public Chage_regif
    , public BusSlaveFuncIf
{
public:
    HAGE_Func(std::string name
        , HAGE_AgentController* agent_controller);
    ~HAGE_Func(void);

private:
    std::string mModuleName;
    HAGE_AgentController* mpAgentController;

    void read (unsigned int offsetAddress
        , TlmBasicPayload& trans
        , BusTime_t* t
        , bool debug);
    void write (unsigned int offsetAddress
        , TlmBasicPayload& trans
        , BusTime_t* t
        , bool debug);

    void cb_HAGECR_DMASTR(RegCBstr str);
    void cb_HAGECR_PLL(RegCBstr str);
    void cb_HAGECR_EINT_MSK(RegCBstr str);
    void cb_HAGECR_HASHOUT(RegCBstr str);
    void cb_HAGECR_MODSEL(RegCBstr str);
    void cb_HAGECR_NEW_SEED(RegCBstr str);
    void cb_HAGESEED_SEED(RegCBstr str);
    void cb_HAGEIDAT_DATA_IN(RegCBstr str);
    void cb_HAGEODAT_DATA_OUT(RegCBstr str);
    void cb_HAGESRCADD_SRC_ADDR(RegCBstr str);
    void cb_HAGEDSTADD_DST_ADDR(RegCBstr str);
    void cb_HAGEDL_DATA_LEN(RegCBstr str);
};

#endif /* __HAGE_FUNC_H__ */
