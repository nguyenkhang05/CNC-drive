// ---------------------------------------------------------------------
// $Id: ATU5_GLUE_Func.h 1196 2019-10-17 05:33:04Z chuonghoangle $
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
#ifndef __ATU5_GLUE_FUNC_H__
#define __ATU5_GLUE_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "atu5_glue_regif.h"

class ATU5_GLUE_AgentController;

class ATU5_GLUE_Func: public Catu5_glue_regif,
                 public BusSlaveFuncIf
{
public:
    ATU5_GLUE_Func (std::string name, ATU5_GLUE_AgentController *ATU5_GLUE_AgentControllerPtr);
    ~ATU5_GLUE_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    unsigned int GetRegSelectSetting (const unsigned int type);

private:
    ATU5_GLUE_AgentController *mATU5_GLUE_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of atu5_glue_regif class
    void cb_ATUINTSELA_ATU_INTSEL_A0(RegCBstr str);
    void cb_ATUINTSELD_ATU_INTSEL_D00(RegCBstr str);
    void cb_ATUINTSELD4_ATU_INTSEL_D00(RegCBstr str);
    void cb_ATUDMASELB_ATU_DMASEL_B0(RegCBstr str);
    void cb_ATUDMASELCD_ATU_DMASEL_CD0(RegCBstr str);
    void cb_ATUDMASELE_ATU_DMASEL_E(RegCBstr str);
    void cb_ATUDFEENTQ_ATU_DFEEN_TQ(RegCBstr str);
    void cb_ATUDFESEL_ATU_DFESEL_D00(RegCBstr str);
    void cb_ATUDFESELD1T_ATU_DFESEL_D1T(RegCBstr str);
    void cb_ATUDSSELDSTS_ATU_DSSEL_DSTS0(RegCBstr str);
    void cb_ATUCASELCATS_ATU_CASEL_CATS0(RegCBstr str);
    void cb_ATUP5SSEL_ATU_P5SSEL_TGC1(RegCBstr str);
};

#endif
