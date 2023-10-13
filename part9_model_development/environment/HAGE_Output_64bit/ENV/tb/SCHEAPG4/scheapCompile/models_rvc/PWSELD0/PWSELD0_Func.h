// ---------------------------------------------------------------------
// $Id: PWSELD0_Func.h,v 1.4 2020/01/15 13:23:55 landang Exp $
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
#ifndef __PWSELD0_FUNC_H__
#define __PWSELD0_FUNC_H__

#include "BusSlaveFuncIf.h"
#include "pwseld0_regif.h"
#include "PWSELD0_AgentController.h"

class PWSELD0_Func: public Cpwseld0_regif
                ,public BusSlaveFuncIf
{
public:
    SC_HAS_PROCESS (PWSELD0_Func);
    PWSELD0_Func(std::string name, PWSELD0_AgentController* PWSELD0AgentController);
    ~PWSELD0_Func();

    void EnableReset(const bool is_active);
    void RegisterHandler(const std::vector<std::string> &args);
    bool StoreTriggerToQueue(unsigned int trgout_index);
    unsigned int GetSLADxBitOfChannel(unsigned int trg_pwg_ch);
    unsigned int GetPVCRValueOfChannel(unsigned int trg_pwg_ch);
    unsigned int GetARSEBit();
    void StorePWS0PWDDIR(unsigned int trg_pwg_ch, unsigned int value);
    void ShiftQueue();
    unsigned int GetSVSDISBit();
    unsigned int GetTopQueue();
    void ClearFlag(unsigned int channel);

    // Virtual functions of BusSlaveFuncIf class
    void read(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    // Variables
    PWSELD0_AgentController   *mpPWSELD0AgentController;
    std::string mModuleName;    // Module name

    // Functions

    // Callback
    void cb_PWS0CTL_ENBL(RegCBstr str);
    void cb_PWS0STC_CLNE(RegCBstr str);
    void cb_PWS0EMU_SVSDIS(RegCBstr str);
    void cb_PWS0PVCR_GCTRLx(RegCBstr str);
    void cb_PWS0PWDDIR_PWDDR(RegCBstr str);

};
#endif //__PWSELD0_FUNC_H__
