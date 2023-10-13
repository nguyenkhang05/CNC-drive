// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_Sub_Func.h,v 1.3 2020/03/24 07:16:32 duongphan Exp $
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
#ifndef __QLAPBFICTLAV1_SUB_FUNC_H__
#define __QLAPBFICTLAV1_SUB_FUNC_H__
#include "OSCI2.h"
#include "qlapbfictlav1_sub_regif.h"
#include "QLAPBFICTLAV1_Sub_AgentController.h"

class QLAPBFICTLAV1_Sub_AgentController;
class QLAPBFICTLAV1_Sub_Func: public Cqlapbfictlav1_sub_regif
{
public:
    QLAPBFICTLAV1_Sub_Func (std::string name
                          , QLAPBFICTLAV1_Sub_AgentController *iQLAPBFICTLAV1_Sub_AgentControllerPtr);
    ~QLAPBFICTLAV1_Sub_Func (void);

    void Reset (const bool is_active);
    void RegisterHandler (const std::vector<std::string> &args);

    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void Initialize(void);

    /// Public functions
    bool GetBYPS(void);
    bool GetINTF(void);
    bool GetINTR(void);
    bool GetINTL(void);
    void SetStartDetector(void);
    void SetLevelDetector(void);
    bool GetStartDetector(void);
    bool GetLevelDetector(void);
private:
    QLAPBFICTLAV1_Sub_AgentController *mQLAPBFICTLAV1_Sub_AgentController;

    std::string mModuleName;        //Module name

    bool mActiveEffectINTL; //Active effect INTL bit
    bool mActiveEffectINTR; //Active effect INTR bit
    bool mIsReset;
    void cb_FCLACTL_INTL(RegCBstr str);
    void cb_FCLACTL_INTF(RegCBstr str);
    void cb_FCLACTL_INTR(RegCBstr str);
    void cb_FCLACTL_BYPS(RegCBstr str);
};

#endif
// vim600: set foldmethod=marker :
