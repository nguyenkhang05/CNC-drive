// ---------------------------------------------------------------------
// $Id: DSADC_E2XFCC1_2ND_Func.h 1398 2019-11-26 06:16:27Z chuonghoangle $
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
#ifndef __DSADC_E2XFCC1_2ND_FUNC_H__
#define __DSADC_E2XFCC1_2ND_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "dsadc_e2xfcc1_2nd_regif.h"

class DSADC_E2XFCC1_2ND_AgentController;

class DSADC_E2XFCC1_2ND_Func: public Cdsadc_e2xfcc1_2nd_regif,
                 public BusSlaveFuncIf
{
public:
    DSADC_E2XFCC1_2ND_Func (std::string name, DSADC_E2XFCC1_2ND_AgentController *DSADC_E2XFCC1_2ND_AgentControllerPtr);
    ~DSADC_E2XFCC1_2ND_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    /// Public functions
    unsigned int CheckDisconnectDetectSetup (void);     /// Get DSADCADGCR.ODDE/ODE value
    bool IsUnsignConfig (void);                         /// Get DSADCADGCR.UNSND value
    bool IsPinDiagEnable (void);                        /// Get DSADCTDCR.TDE value

private:
    DSADC_E2XFCC1_2ND_AgentController *mDSADC_E2XFCC1_2ND_AgentController;
    std::string mModuleName;                            /// Module name

    /// Declare virtual functions of dsadc_e2xfcc1_2nd_regif class
    void cb_DSADCSYNSTCR_ADSTART(RegCBstr str);
    void cb_DSADCTDCR_TDE(RegCBstr str);
};

#endif
