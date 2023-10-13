// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_Func.h,v 1.3 2020/03/24 07:16:32 duongphan Exp $
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
#ifndef __QLAPBFICTLAV1_FUNC_H__
#define __QLAPBFICTLAV1_FUNC_H__
#include "QLAPBFICTLAV1.h"
#include "BusSlaveFuncIf.h"

class QLAPBFICTLAV1_AgentController;

class QLAPBFICTLAV1_Func: public BusSlaveFuncIf{
public:
    QLAPBFICTLAV1_Func (std::string name,
                        QLAPBFICTLAV1_AgentController *iQLAPBFICTLAV1_AgentController);

    virtual ~QLAPBFICTLAV1_Func (void);

    /// Virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:

    QLAPBFICTLAV1_AgentController* mQLAPBFICTLAV1_AgentController;
};
#endif //__QLAPBFICTLAV1_FUNC_H__
// vim600: set foldmethod=marker :
