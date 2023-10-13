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
    
    // Get register value
    uint32_t GetDATAIN();
    uint32_t GetSEEDDATA();
    uint32_t GetSRCADD();
    uint32_t GetDSTADD();
    uint32_t GetDATALEN();
    uint32_t GetHAGECR(std::string bitname);
    uint32_t GetPLL();
    // Set register value
    void SetValue(std::string regname, std::string bitname, uint32_t value);

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

    // Declare virtual functions of HAGE_regif class
    void cb_HAGECR_DMASTR(RegCBstr str);
    void cb_HAGEIDAT_DATA_IN(RegCBstr str);
};

#endif /* __HAGE_FUNC_H__ */
