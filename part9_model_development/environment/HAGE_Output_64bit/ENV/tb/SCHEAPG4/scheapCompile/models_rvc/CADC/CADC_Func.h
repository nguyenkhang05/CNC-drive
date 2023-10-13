// ---------------------------------------------------------------------
// $Id: CADC_Func.h,v 1.1 2018/11/19 02:50:58 chanle Exp $
//
// Copyright(c) 2016-2018 Renesas Electronics Corporation
// Copyright(c) 2016-2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __CADC_FUNC_H__
#define __CADC_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "cadc_regif.h"

class CADC_AgentController;

class CADC_Func: public Ccadc_regif,
                 public BusSlaveFuncIf
{
public:
    CADC_Func (std::string name, CADC_AgentController *CADC_AgentControllerPtr);
    ~CADC_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    unsigned int CheckDisconnectDetectSetup (void);     // Get CADCADGCR.ODDE/ODE value
    bool IsUnsignConfig (void);                         // Get CADCADGCR.UNSND value
    bool IsPinDiagEnable (void);                        // Get CADCTDCR.TDE value
    bool ValueODE (void);                               // Get CADCADGCR.ODE value

private:
    CADC_AgentController *mCADC_AgentController;
    std::string mModuleName;        //Module name

    /// Declare virtual functions of cadc_regif class
    void cb_CADCTDCR_CADCTDE(RegCBstr str);
};

#endif
