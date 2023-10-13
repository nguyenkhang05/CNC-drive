// ---------------------------------------------------------------------
// $Id: TSN_Func.h,v 1.0 2014/11/27 06:42:47 binhnguyen Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __TSN_FUNC_H__
#define __TSN_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "tsn_regif.h"

class TSN_AgentController;

class TSN_Func: public Ctsn_regif
                ,public BusSlaveFuncIf
{
public:
    TSN_Func (std::string name,
               TSN_AgentController *TSN_AgentControllerPtr);
    ~TSN_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);
    
    void SetStatusFlag (const bool is_true);
    void DumpStatusInfo (const unsigned int state);
    bool IsTSNEnable (void);
    
    //Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

private:
    TSN_AgentController *mTSN_AgentController;
    std::string mModuleName;//Module name
    //Declare virtual function of tsn_regif class
    void cb_TSNCR_TSNEN(RegCBstr str);
    void cb_TSNDIAG_TSNSELFDIAG(RegCBstr str);
};
#endif
