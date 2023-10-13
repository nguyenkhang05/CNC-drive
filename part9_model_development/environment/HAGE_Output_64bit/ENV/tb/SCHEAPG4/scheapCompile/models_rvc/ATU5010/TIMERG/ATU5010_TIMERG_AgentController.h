// ---------------------------------------------------------------------
// $Id: ATU5010_TIMERG_AgentController.h,v 1.0 647 2016-06-16 07:07:15Z CuongPH8 $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef ATU5010_TIMERG_ATU5010_TIMERG_AGENTCONTROLLER_H_
#define ATU5010_TIMERG_ATU5010_TIMERG_AGENTCONTROLLER_H_

#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ATU5010_TIMERG_AgentController
{
#include "ATU5010_TIMERG_cmdif.h"
public:
    ATU5010_TIMERG_AgentController() {}
    virtual ~ATU5010_TIMERG_AgentController() {}
};

#endif /* ATU5010_TIMERG_ATU5010_TIMERG_AGENTCONTROLLER_H_ */
//------------------------------------------------------------------------------
// EOF

