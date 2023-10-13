// ---------------------------------------------------------------------
// $Id: ATU5_TIMERG_AgentController.h 1196 2019-10-17 05:33:04Z chuonghoangle $
//
// Copyright(c) 2018-2019 Renesas Electronics Corporation
// Copyright(c) 2018-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef ATU5_TIMERG_ATU5_TIMERG_AGENTCONTROLLER_H_
#define ATU5_TIMERG_ATU5_TIMERG_AGENTCONTROLLER_H_

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ATU5_TIMERG_AgentController
{
#include "ATU5_TIMERG_cmdif.h"
public:
    ATU5_TIMERG_AgentController() {}
    virtual ~ATU5_TIMERG_AgentController() {}
};

#endif /* ATU5_TIMERG_ATU5_TIMERG_AGENTCONTROLLER_H_ */
//------------------------------------------------------------------------------
// EOF

