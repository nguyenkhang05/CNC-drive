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

class ModuleRun
{
public:
    virtual IsElapsedTime RunModule (void) = 0;
    virtual void ClockWait (uint64_t wait_cycle) = 0;
    virtual void PreConsumeClock (void) = 0;
    virtual void AddHaltCycle (uint64_t halt_cycle) = 0;
    virtual void UpdateSnoozeState (void) = 0;
    virtual bool IsBreakHitPreExec (void) const = 0;
    virtual bool IsBreakHitPostExec (void) const = 0;
    virtual bool IsHtRunable (void) const = 0;
    virtual bool IsSnooze (void) const = 0;
    virtual bool IsHtActive (void) const = 0;
    virtual bool IsIntRequested (void) const = 0;
    virtual bool IsInHalt (void) const = 0;
#ifdef SUPPORT_SC_HEAP
    virtual bool IsSuspendState (void) const = 0;
#endif // SUPPORT_SC_HEAP
    virtual bool IsCpu (void) const = 0;
    virtual FILE* GetFilePointer (void) const = 0;
    virtual int32_t GetScId (void) const = 0;
    virtual uint64_t GetCpuTime (void) const = 0;
    virtual uint64_t GetBusTime (void) const = 0;
    virtual uint32_t GetCpuRatio (void) const = 0;
    virtual ~ModuleRun () {}
};
