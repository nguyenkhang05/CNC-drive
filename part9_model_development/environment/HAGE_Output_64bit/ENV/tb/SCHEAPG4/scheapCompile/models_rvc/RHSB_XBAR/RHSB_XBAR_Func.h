// ---------------------------------------------------------------------
// $Id: RHSB_XBAR_Func.h 1339 2019-11-14 09:01:36Z chuonghoangle $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __RHSB_XBAR_FUNC_H__
#define __RHSB_XBAR_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "rhsb_xbar_regif.h"
#include "RHSB_XBAR_AgentController.h"

class RHSB_XBAR_Func: public Crhsb_xbar_regif
                 ,public BusSlaveFuncIf
{
public:
    RHSB_XBAR_Func (std::string name, 
                RHSB_XBAR_AgentController *XBARAgentPtr,
                std::string user = "GTM");
    ~RHSB_XBAR_Func (void);

    void RegisterHandler (const std::vector<std::string>& args);
    void EnableReset (const bool is_active);
    void CheckSettingReg(void);

    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:

    RHSB_XBAR_AgentController    *mXBARAgent;
    std::string             mModuleName;
    std::string             mUser;

    void CheckRHSBGCROk(const unsigned sub_xbar, const unsigned int bit_pos);
    void CheckRHSBGCRkLforGTM(const unsigned int sub_xbar, const unsigned int bit_pos, const std::string CRO_reg);
    void CheckRHSBGCRkHforGTM(const unsigned int sub_xbar, const unsigned int bit_pos, const std::string CRO_reg);
    void CheckRHSBGCRkLforATU(const unsigned int sub_xbar, const unsigned int bit_pos, const std::string type, const std::string CRO_reg);
    void CheckRHSBGCRkHforATU(const unsigned int sub_xbar, const unsigned int bit_pos, const std::string type, const std::string CRO_reg);
};
#endif
