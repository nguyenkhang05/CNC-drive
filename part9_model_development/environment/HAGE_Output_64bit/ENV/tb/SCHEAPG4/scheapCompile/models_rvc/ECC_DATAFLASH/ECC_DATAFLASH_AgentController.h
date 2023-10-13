// ---------------------------------------------------------------------
// $Id: ECC_DATAFLASH_AgentController.h,v 1.3 2015/07/31 02:14:20 huepham Exp $
//
// Copyright(c) 2014-2015 Renesas Electronics Corporation
// Copyright(c) 2014-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_DATAFLASH_AgentController__
#define __ECC_DATAFLASH_AgentController__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

class ECC_DATAFLASH_AgentController
{
#include "ECC_DATAFLASH_cmdif.h"
public:
    ECC_DATAFLASH_AgentController()
    {
        ;
    }
    virtual ~ECC_DATAFLASH_AgentController()
    {
        ;
    }
    virtual bool CheckClock (void) = 0;
    virtual void ErrorNotification (const unsigned int port_id) = 0;
    virtual void OverflowNotification (void) = 0;
    virtual void ECCStatusNotification (const unsigned int port_id, const bool output_value) = 0;
};
#endif //__ECC_DATAFLASH_AgentController__
