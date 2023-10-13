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


enum DbufReplaceType {
    DBUF_REP_DIRECT,
    DBUF_REP_ROUNDR,
};


class Dbuf {
public:
    static const uint32_t DBUF_HIT_LATENCY_G4MH;
    static const uint32_t DBUF_HIT_LATENCY_G4KH;

    class DbufEntry {
    public:
        PhysAddr m_dbuf_paddr;
        std::vector<uint64_t> m_dbuf_avail_cycle;
        explicit DbufEntry (uint32_t fill_burst_num);
        ~DbufEntry () {}
    };
    std::vector <DbufEntry*> m_dbuf_entry;
    void UpdateMemAvailCycle (uint64_t avail_cycle);

    uint64_t m_hit_count;
    uint64_t m_miss_count;
    void IncrementHit (void) { ++m_hit_count; }
    void IncrementMiss (void) { ++m_miss_count; }

    bool IsHitDbuf (uint32_t entry, PhysAddr load_addr) const;
    uint64_t UpdateDbuf (uint32_t entry,
            PhysAddr load_addr, uint64_t ready_cycle, uint32_t repeat_cycle);
    void UpdateDbufFillNext (uint32_t entry, PhysAddr load_addr, uint64_t ready_cycle);
    void UpdateDbufPreload (uint32_t entry, PhysAddr load_addr, uint64_t ready_cycle);
    uint32_t GetDbufEntry (PhysAddr load_addr) const;
    uint32_t GetReplaceEntry (PhysAddr load_addr) const;
    void UpdateReplaceEntry (void);

    uint32_t m_round_robin_entry;

    uint32_t GetWayId (PhysAddr load_addr) const;
    uint64_t GetDbufCycle (uint32_t entry, PhysAddr load_addr) const;
    void HardReset (void);
    void DumpDbuf (void) const;

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

    PhysAddr m_last_fill_addr;
    PhysAddr GetLastFillAddr (void) const { return m_last_fill_addr; }
    void SetLastFillAddr (PhysAddr addr) { m_last_fill_addr = addr; }

    PhysAddr m_last_preload_addr;
    PhysAddr GetLastPreloadAddr (void) const { return m_last_preload_addr; }
    void SetLastPreloadAddr (PhysAddr addr) { m_last_preload_addr = addr; }
    bool m_is_preload_plus;
    bool IsPreloadPlus (void) const { return m_is_preload_plus; }
    void SetPreloadPlus (bool is_plus) { m_is_preload_plus = is_plus; }

    DbufPreloadType m_preload_type;

    const uint32_t m_peid;
    const uint32_t DBUF_DEPTH; // DBUF段数変更
    const uint32_t DBUF_ADDR_SHIFT; // DBUFラインサイズ変更
    const uint32_t DBUF_FILL_BURST_SIZE; // FILL回数変更
    const uint32_t DBUF_LINE_BYTE;
    const PhysAddr DBUF_ADDR_MASK;
    const DbufReplaceType DBUF_REPLACE_TYPE;
    const uint32_t FLASH_SIZE;
    const uint32_t PRELOAD_SIZE;

    Dbuf (uint32_t peid, uint32_t depth, uint32_t line_shift, uint32_t fill_num,
          DbufReplaceType replace_type, uint32_t flash_width, CoreType core_type);
    ~Dbuf ();
};
