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


class InvalidLram : public CModuleVc
{
private:
    class LramArea {
    public:
        uint32_t m_peid;
        PhysAddr m_start_addr;
        PhysAddr m_end_addr;


        LramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr)
        : m_peid (peid), m_start_addr (start_addr), m_end_addr (end_addr)
        {}
        ~LramArea () {}
    };
    std::vector<LramArea*> m_lram_area;

    enum LramAreaType {
    LRAM_NONE = 0,
    LRAM_SELF_PE = 1,
    LRAM_OTHER_PE = 2
    };
    LramAreaType CheckLramArea (uint32_t peid, PhysAddr addr) const;
    SysErrCause JudgeSyserr (uint32_t peid, PhysAddr addr) const;

public:
    bool RegisterLramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr);

    void CyclicHandler (void) {}
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void) {}

    InvalidLram ();
    virtual ~InvalidLram();
};
