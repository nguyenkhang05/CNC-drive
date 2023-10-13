// ---------------------------------------------------------------------
// $Id: AHBDataHandler_UIMPL.h,v 1.0 2013/11/19 09:24:40 dungvannguyen Exp $
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
#ifndef __AHBDataHandler_UIMPL_H__
#define __AHBDataHandler_UIMPL_H__
#include "BusSlaveFuncIf.h"
#include "AHBAgentController_UIMPL.h"

/// AHBDataHandler_UIMPL model class
class AHBDataHandler_UIMPL:public BusSlaveFuncIf
{
private:
    AHBAgentController_UIMPL *mAgentController;
public:
    AHBDataHandler_UIMPL (std::string name, AHBAgentController_UIMPL *AgentControllerPtr);
    virtual ~AHBDataHandler_UIMPL (void){;}
    //virtual functions of BusSlaveFuncIf class
    void read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
private:
};
#endif //__AHBDataHandler_UIMPL_H__
// vim600: set foldmethod=marker :
