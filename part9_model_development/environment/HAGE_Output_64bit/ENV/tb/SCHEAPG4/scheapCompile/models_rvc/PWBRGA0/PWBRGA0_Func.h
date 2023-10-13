// ---------------------------------------------------------------------
// $Id: PWBRGA0_Func.h,v 1.1 2018/07/28 04:40:33 chanle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PWBRGA0_FUNC_H__
#define __PWBRGA0_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "pwbrga0_regif.h"
#include "PWBRGA0_AgentController.h"

class PWBRGA0_Func: public Cpwbrga0_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (PWBRGA0_Func);
    PWBRGA0_Func(std::string name, PWBRGA0_AgentController* PWBRGA0AgentController);
    ~PWBRGA0_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void WriteTEBit(unsigned int value);
    unsigned int GetPWBA0BRSRegister(unsigned int index);
    bool GetTEBit(unsigned int index);
    bool GetSVSDISBit();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    PWBRGA0_AgentController   *mpPWBRGA0AgentController;
    std::string mModuleName;    // Module name

    // Functions

    // Callback
    void cb_PWBA0BRSn_BRS(RegCBstr str);
    void cb_PWBA0TS_TS(RegCBstr str);
    void cb_PWBA0TT_TT(RegCBstr str);
    void cb_PWBA0EMU_SVSDIS(RegCBstr str);

};
#endif //__PWBRGA0_FUNC_H__
