// ---------------------------------------------------------------------
// $Id: ATU5010_Func.h,v 1.0 2016/11/24 10:15:45 binhnguyen Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ATU5010_FUNC_H__
#define __ATU5010_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "atu5010_regif.h"

class ATU5010_AgentController;

class ATU5010_Func: public Catu5010_regif,
                 public BusSlaveFuncIf
{
public:
    ATU5010_Func (std::string name, ATU5010_AgentController *ATU5010_AgentControllerPtr);
    ~ATU5010_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    unsigned int GetNoiseCancelMode (const std::string timer_name);
    unsigned int GetPrescalerRatio (const unsigned int ch_id);
    unsigned int GetClkEdgeSetting (const bool is_TCLKA);
    bool GetClkBus5Setting (void);
    bool GetEnableStatus (const unsigned int index);
    unsigned int GetRegSelectSetting (const unsigned int type);
    bool GetNCClkSel (void);
    void SetTrgStatusDMAReg (const bool is_high);

private:
    ATU5010_AgentController *mATU5010_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of atu5010_regif class
    void cb_ATUENR_PSCE (RegCBstr str);
    void cb_CBCNT_CB5EG (RegCBstr str);
    void cb_NCMR_NCMA (RegCBstr str);
    void cb_PSCR_PSC (RegCBstr str);
    void cb_TRGSELDMA0_INSELDMA0 (RegCBstr str);
    void cb_TRGSELAD_INSELAD (RegCBstr str);
    void cb_TRGSELDMA1_INSELDMA1 (RegCBstr str);
};

#endif
