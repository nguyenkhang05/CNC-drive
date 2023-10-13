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
#include <utility>
#include "./forest.h"
#include "./forest_common.h"

class TraceCtrl;

class Llsc {
public:
    typedef std::pair<PhysAddr, ByteSize> LinkBody;
    typedef std::pair<int32_t, LinkBody> LinkTarget;
    typedef std::vector<LinkTarget> LinkTargetVec;

    // G3KH Link STC.W success/fail condition detection mask
    // G3MH Link STC.W success/fail condition is "completely address match".
    PhysAddr m_link_cond_mask;

    LinkTargetVec m_link_targets;
    void CreateLink (int32_t scid, PhysAddr link_addr, ByteSize size, TraceCtrl* trace);
    bool IsHaveLink (int32_t scid, PhysAddr link_addr, ByteSize size) const;
    bool IsThereAnyLink (void) const;
    void DeleteLink (PhysAddr link_addr, TraceCtrl* trace);
    void DeleteOwnLink (int32_t scid, TraceCtrl* trace);
    void DeleteOwnLink (int32_t scid, PhysAddr paddr, TraceCtrl* trace);
    void DeletePeLink (int32_t scid, TraceCtrl* trace);
    void DeleteVmLink (int32_t scid, TraceCtrl* trace);
    void InitLink (void);

    void SetCondMask (PhysAddr mask) { m_link_cond_mask = mask; }
    PhysAddr GetCondMask (void) const { return m_link_cond_mask; }

    // Delete link in 32byte line
    static const PhysAddr LINK_ADDR_SIZE = 0x20U;
    static const PhysAddr LINK_ADDR_MASK = ~(LINK_ADDR_SIZE - 0x1U);

    Llsc () {}
    ~Llsc () {}
};
