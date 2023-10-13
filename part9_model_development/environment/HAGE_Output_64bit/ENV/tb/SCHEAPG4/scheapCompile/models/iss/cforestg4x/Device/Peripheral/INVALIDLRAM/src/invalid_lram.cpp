/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <string.h>
#include "./config.h"
#include "./invalid_lram.h"
#include "./forest_utility.h"

SysErrCause InvalidLram::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    uint32_t peid = p_forest_util->GetPeidFromBmid (sideband.GetBMID ());
    return JudgeSyserr (peid, addr);
}


SysErrCause InvalidLram::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    memset (data, 0x0, size);
    uint32_t peid = p_forest_util->GetPeidFromBmid (sideband.GetBMID ());
    return JudgeSyserr (peid, addr);
}


SysErrCause InvalidLram::TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    memset (data, 0x0, size);
    return SYSERR_IBUS;
}


SysErrCause InvalidLram::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return SYSERR_NONE;
}


SysErrCause InvalidLram::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    memset (data, 0x0, size);
    return SYSERR_NONE;
}


InvalidLram::LramAreaType InvalidLram::CheckLramArea (uint32_t peid, PhysAddr addr) const
{
    // Current implementation (not spec) : invalid PE is treated as other pe area.
    std::vector<LramArea*>::const_iterator it = m_lram_area.begin ();
    while (it != m_lram_area.end ()) {
        if (((*it)->m_start_addr <= addr) && (addr <= (*it)->m_end_addr)) {
            if ((*it)->m_peid == peid) {
                // 自PE領域
                return LRAM_SELF_PE;
            } else {
                // 他PE領域
                return LRAM_OTHER_PE;
            }
        }
        ++ it;
    }
    return LRAM_NONE;
}


SysErrCause InvalidLram::JudgeSyserr (uint32_t peid, PhysAddr addr) const
{
    LramAreaType area = CheckLramArea (peid, addr);
    SysErrCause ret;
    switch (area) {
    case LRAM_SELF_PE: ret = SYSERR_LRAM; break; // TCM bit
    case LRAM_OTHER_PE: ret = SYSERR_DBUS; break; // VCI bit
    default: ret = SYSERR_DBUS; break; // 登録に不足なければここには来ないはず。
    }

    return ret;
}


bool InvalidLram::RegisterLramArea (uint32_t peid, PhysAddr start_addr, PhysAddr end_addr)
{
    if ((CheckLramArea (peid, start_addr) != LRAM_NONE) || (CheckLramArea (peid, end_addr) != LRAM_NONE)) {
        // 登録済みアドレス
        return false;
    }

    m_lram_area.push_back (new LramArea (peid, start_addr, end_addr));
    return true;
}


InvalidLram::InvalidLram ()
{
    ;
}


InvalidLram::~InvalidLram ()
{
    std::vector<LramArea*>::iterator it = m_lram_area.begin ();
    while (it != m_lram_area.end ()) {
        delete *it;
        ++ it;
    }
}
