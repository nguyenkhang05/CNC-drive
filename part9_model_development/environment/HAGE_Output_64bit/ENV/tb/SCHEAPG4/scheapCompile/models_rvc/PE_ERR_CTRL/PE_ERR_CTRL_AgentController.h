// ---------------------------------------------------------------------
// $Id: PE_ERR_CTRL_AgentController.h,v 1.1 2014/02/25 02:45:52 duymai Exp $
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
#ifndef __PE_ERR_CTRL_AgentController__
#define __PE_ERR_CTRL_AgentController__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class PE_ERR_CTRL_AgentController
{
#include "PE_ERR_CTRL_cmdif.h"
public:
    PE_ERR_CTRL_AgentController()
    {
        ;
    }
    virtual ~PE_ERR_CTRL_AgentController()
    {
        ;
    }
    virtual bool CheckClock (const std::string name) = 0;
    virtual void ControlIntr (void) = 0;
    virtual bool GetResetStatus (const std::string reset_name) = 0;
};
#endif //__PE_ERR_CTRL_AgentController__
