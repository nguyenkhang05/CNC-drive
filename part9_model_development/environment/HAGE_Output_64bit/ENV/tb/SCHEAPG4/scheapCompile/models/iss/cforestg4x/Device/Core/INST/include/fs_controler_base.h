/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include "./forest_common.h"

class CIntc1;

class FsCtrl_interface {
public:
    virtual IntErrorCode CancelFenmi (void) = 0;
    virtual IntErrorCode CancelFeint (void) = 0;
    virtual IntErrorCode ReqFenmi (void (*resfunc)(CIntc1*)) = 0;
    virtual IntErrorCode ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                                       bool is_gm, uint32_t gpid, bool is_bgint,
                                       void (*resfunc)(uint32_t , CIntc1*)) = 0;
    virtual IntErrorCode ReqFeint (void (*resfunc)(uint32_t , CIntc1* ), uint32_t channel,
                                   bool is_gm, uint32_t gpid, bool is_bgint) = 0;
    virtual IntErrorCode ReqInterrupt (uint32_t int_priority, uint32_t int_cause, bool eitb,
                                       void (*resfunc)(uint32_t , CIntc1*)) = 0;
    virtual IntErrorCode ReqFeint (void (*resfunc)(uint32_t , CIntc1* ), uint32_t channel) = 0;

    virtual IntErrorCode CancelInterrupt (uint32_t ) = 0;
    virtual ~FsCtrl_interface(){}
};
