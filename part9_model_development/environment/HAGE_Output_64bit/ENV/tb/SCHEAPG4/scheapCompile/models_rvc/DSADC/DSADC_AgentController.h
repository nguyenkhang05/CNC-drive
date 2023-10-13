// ---------------------------------------------------------------------
// $Id: DSADC_AgentController.h 1398 2019-11-26 06:16:27Z chuonghoangle $
//
// Copyright(c) 2017-2019 Renesas Electronics Corporation
// Copyright(c) 2017-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __DSADC_AGENTCONTROLLER_H__
#define __DSADC_AGENTCONTROLLER_H__

#include "DSADC_Base.h"

class DSADC_AgentController:public DSADC_Base
{
#include "DSADC_cmdif.h"

public:
    enum eModelNum {    /// Define model's constant number
        emADNum = 10
    };
    
    enum eAdId {        /// Define model's constant number
        emAD00,
        emAD10,
        emAD20,
        emAD12,
        emAD13,
        emAD11,
        emAD15,
        emAD14,
        emAD22,
        emAD21
    };
    
    DSADC_AgentController(void) {;}
    virtual ~DSADC_AgentController(void) {;}
    DSADC_UNIT *mAD_Unit[emADNum];               /// AD unit instance
};
#endif //__DSADC_AGENTCONTROLLER_H__
