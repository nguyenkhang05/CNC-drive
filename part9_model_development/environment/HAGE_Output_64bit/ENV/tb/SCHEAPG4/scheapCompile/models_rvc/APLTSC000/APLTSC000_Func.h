// ---------------------------------------------------------------------
// $Id: APLTSC000_Func.h,v 1.2 2020/02/29 07:07:07 chuonghoangle Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __APLTSC000_FUNC_H__
#define __APLTSC000_FUNC_H__

#include "global.h"
#include "BusSlaveFuncIf.h"
#include "apltsc000_regif.h"
#include "APLTSC000_AgentController.h"

class APLTSC000_Func: public Capltsc000_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (APLTSC000_Func);
    APLTSC000_Func(std::string name, APLTSC000_AgentController* APLTSC000AgentController);
    ~APLTSC000_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    void DumpStatusInfo();

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    
    unsigned int GetSVSDISReg();
    unsigned int GetCSTReg();
    unsigned int GetCNTLReg();
    unsigned int GetCNTHReg();
    unsigned int GetRMReg();
    void SetCNTLReg(unsigned int value);
    void SetCNTHReg(unsigned int value);
    void SetSVSDISReg(unsigned int value);

private:

    // Variables
    APLTSC000_AgentController   *mpAPLTSC000AgentController;    // APLTSC000 Agent controller pointer
    std::string     mModuleName;                                // Module name
    // Function
    void Initialize();
    // Callback
    void cb_LTSCnTCS_LTSCnTS(RegCBstr str);
    void cb_LTSCnTCT_LTSCnTT(RegCBstr str);
    void cb_LTSCnRM_LTSCnRM(RegCBstr str);
    void cb_LTSCnEMU_LTSCnSVSDIS(RegCBstr str);
    void cb_LTSCnCNTL_LTSCnCNTL(RegCBstr str);
    void cb_LTSCnCNTH_LTSCnCNTH(RegCBstr str);

};
#endif //__APLTSC000_FUNC_H__
