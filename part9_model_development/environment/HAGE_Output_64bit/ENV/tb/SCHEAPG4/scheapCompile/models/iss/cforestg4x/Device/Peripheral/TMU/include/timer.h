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

#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"

class FsCtrl;
class TmuChannel;
class FsBus;
class CIntc2;


class CTimer : public CModuleVc
{
public:
    void CyclicHandler (void);
    SysErrCause TargetWriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadBody  (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);

    CTimer (uint32_t tmuid, FsBus* fsbus, CIntc2* intc2);
    virtual ~CTimer ();

private:
    static const PhysAddr TMU_CHANNEL_MASK = 0x000000FFUL;
    static const uint32_t N_CH_PER_TMU = 3;

    TmuChannel* m_tmuch[N_CH_PER_TMU];

    FsBus* m_fsbus;

    void SetTSTR (uint32_t val);
    uint32_t GetTSTR (void) const;
};


class TmuChannel
{
public:
    void CountDown (void);
    void SetRunning (bool run);
    bool IsRunning (void) const;
    void SetTCOR (uint32_t val);
    void SetTCNT (uint32_t val);
    void SetTCR  (uint32_t val);
    uint32_t GetTCOR (void) const;
    uint32_t GetTCNT (void) const;
    uint32_t GetTCR  (void) const;
    void HardReset (void);

    TmuChannel (uint32_t chid, CIntc2* intc2);
    ~TmuChannel () {}

private:
    uint32_t m_chid;
    bool     m_is_running;
    uint32_t m_counter;
    uint32_t m_constant;
    bool     m_underflow_flag;
    bool     m_underflow_enable;

    static const uint32_t TMU_BASE_CHANNEL = 47;
    CIntc2*  m_intc2;
};
