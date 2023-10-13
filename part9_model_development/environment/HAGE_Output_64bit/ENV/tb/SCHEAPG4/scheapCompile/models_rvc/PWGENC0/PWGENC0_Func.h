// ---------------------------------------------------------------------
// $Id: PWGENC0_Func.h,v 1.2 2018/08/24 06:38:28 chanle Exp $
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
#ifndef __PWGENC0_FUNC_H__
#define __PWGENC0_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "pwgenc0_regif.h"
#include "PWGENC0_AgentController.h"

class PWGENC0_Func: public Cpwgenc0_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (PWGENC0_Func);
    PWGENC0_Func(std::string name, PWGENC0_AgentController* PWGENC0AgentController);
    ~PWGENC0_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void LoadTCBR();
    void LoadCounterValue();
    void SetPWG0STReg(unsigned int value);
    void UpdateTEBit(unsigned int value);
    unsigned int GetTEBit();
    unsigned int GetPWG0CTLReg();
    unsigned int GetPWG0CSBRReg();
    unsigned int GetPWG0CRBRReg();
    unsigned int GetPWG0CTBRReg();
    unsigned int GetPWG0TCBRReg();
    unsigned int GetPWG0STReg();
    void UpdateRSFBit(unsigned int value);
    void UpdateRSFTBit(unsigned int value);
    void UpdateFOFBit(unsigned int value);
    void SetPWG0CNTReg(unsigned int value);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    PWGENC0_AgentController   *mpPWGENC0AgentController;
    std::string mModuleName;    // Module name

    // Functions

    // Callback
    void cb_PWG0CSDR_CSDR(RegCBstr str);
    void cb_PWG0RDT_RDT(RegCBstr str);
    void cb_PWG0ST_RSF(RegCBstr str);
    void cb_PWG0CNT_CNT(RegCBstr str);
    void cb_PWG0TCR_TOE(RegCBstr str);
    void cb_PWG0TCBR_TOBE(RegCBstr str);
    void cb_PWG0CTL_CKS(RegCBstr str);
    void cb_PWG0CSBR_CSBR(RegCBstr str);
    void cb_PWG0CRBR_CRBR(RegCBstr str);
    void cb_PWG0CTBR_CTBR(RegCBstr str);
    void cb_PWG0TS_TS(RegCBstr str);
    void cb_PWG0TT_TT(RegCBstr str);
    void cb_PWG0FOT_FOT(RegCBstr str);

};
#endif //__PWGENC0_FUNC_H__
