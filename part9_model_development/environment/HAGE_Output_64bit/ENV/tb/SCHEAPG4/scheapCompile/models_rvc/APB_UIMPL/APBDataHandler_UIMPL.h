// ---------------------------------------------------------------------
// $Id: APBDataHandler_UIMPL.h,v 1.0 2013/11/19 09:21:57 dungvannguyen Exp $
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __APBDataHandler_UIMPL_H__
#define __APBDataHandler_UIMPL_H__
#include "BusSlaveFuncIf.h"
#include "APBAgentController_UIMPL.h"

/// APBDataHandler_UIMPL model class
class APBDataHandler_UIMPL:public BusSlaveFuncIf
{
private:
    APBAgentController_UIMPL *mAgentController;
public:
    APBDataHandler_UIMPL (std::string name, APBAgentController_UIMPL *AgentControllerPtr);
    virtual ~APBDataHandler_UIMPL (void){;}
    //virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:
};
#endif //__APBDataHandler_UIMPL_H__
// vim600: set foldmethod=marker :
