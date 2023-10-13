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


enum FlcReplaceType {
    FLC_REP_DIRECT, // Unsupported
    FLC_REP_ROUNDR,
};


class FlashCache {
public:
    static const uint32_t FLC_HIT_LATENCY_R;
    static const uint32_t FLC_HIT_LATENCY_I;
    static const uint32_t FLC_ENTRY_NUM;
    static const uint32_t FLC_WAY_NUM;
    static const uint32_t FLC_ADDR_SHIFT;
    static const uint32_t FLC_LINE_BYTE;
    static const PhysAddr FLC_ADDR_MASK;
    static const FlcReplaceType FLC_REPLACE_TYPE;

    class FlcEntry {
    public:
        PhysAddr m_flc_paddr;
        std::vector<uint64_t> m_flc_avail_cycle;
        explicit FlcEntry (uint32_t way_num);
        ~FlcEntry () {}
    };
    std::vector <FlcEntry*> m_flc_entry;
    void UpdateMemAvailCycle (uint64_t avail_cycle);

    uint64_t m_hit_count;
    uint64_t m_miss_count;
    void IncrementHit (void) { ++m_hit_count; }
    void IncrementMiss (void) { ++m_miss_count; }

    bool IsHitFlc (uint32_t entry, PhysAddr load_addr) const;
    uint64_t UpdateFlc (uint32_t entry,
            PhysAddr load_addr, uint64_t ready_cycle, uint32_t repeat_cycle);
    uint32_t GetFlcEntry (PhysAddr load_addr) const;
    uint32_t GetReplaceEntry (PhysAddr load_addr) const;
    void UpdateReplaceEntry (void);

    uint32_t m_round_robin_entry;

    uint32_t GetWayId (PhysAddr load_addr) const;
    uint64_t GetFlcCycle (uint32_t entry, PhysAddr load_addr) const;
    void HardReset (void);
    void DumpFlc (void) const;

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

    const uint32_t m_peid;

    FlashCache (uint32_t peid);
    ~FlashCache ();
};
