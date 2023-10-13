/*
 * (c) 2011-2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */


#pragma once


#include "./forest_common_api.h"


class CIntc1;
class IntReqInfoBase;


class Intc1Api {
public:
    virtual void ReqEiintToCpu (uint32_t priority, uint32_t channel, bool eitb,
#if _INTC1_20
                                    bool bindmode, uint32_t gpid, uint32_t bgpr,
#endif // #if _INTC1_20
                                    void (*resfunc)(uint32_t channel, CIntc1* intc1)) = 0;
    virtual void ReqFenmiToCpu (void (*resfunc)(CIntc1* intc1)) = 0;
    virtual void ReqFeintToCpu (uint32_t channel,
#if _INTC1_20
                                     bool bindmode, uint32_t gpid, uint32_t bge,
#endif //#if _INTC1_20
        		                     void (*resfunc)(uint32_t channel, CIntc1* intc1)) = 0;
    virtual void CancelIntToCpu (uint32_t channel) = 0;
    virtual void CancelFenmiToCpu (void) = 0;
    virtual void CancelFeintToCpu (void) = 0;
    virtual void StartCyclicHandler (void) = 0;
    virtual void StopCyclicHandler (CIntc1* intc1) = 0;
    virtual void UpdateInterruptPort (uint32_t channel) = 0;
#if _INTC1_20
    virtual void SetCpuGuestMode(bool is_gm) = 0;
    virtual void SetCpuGpid(uint32_t gpid) = 0;
#endif //#if _INTC1_20
    virtual ~Intc1Api () {}
};


class Intc2Api {
public:
    virtual void ReqIntToIntc1 (uint32_t peid, uint32_t channel, IntReqInfoBase* reqinfo) = 0;
    virtual void CancelIntToIntc1 (uint32_t peid, uint32_t channel) = 0;
    virtual void CancelIntByMask (uint32_t channel) = 0;
    virtual void UpdateHighPriority (uint32_t peid) = 0;
    virtual void UpdateHighPriorityOfIntc1 (uint32_t pe, uint32_t channel) = 0;
    virtual bool IsIntReqInQueueOfIntc1 (uint32_t peid, uint32_t channel) = 0;
    virtual void AddBcasIntInfo (uint32_t channel) = 0;
    virtual void StartCyclicHandler (void) = 0;
    virtual void StopCyclicHandler (void) = 0;

    virtual ~Intc2Api () {}
};
