/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This proGramWtb must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * proGramWtb may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <vector>
#include "./config.h"
#include "./flash_cache.h"
#include "./forest_utility.h"
#include "./forest_message.h"

// Configurable Parameters
const uint32_t FlashCache::FLC_HIT_LATENCY_R = 5; // L1MISS-L2HIT latency for data load
const uint32_t FlashCache::FLC_HIT_LATENCY_I = 5; // L1MISS-L2HIT latency for inst fetch
// Total FLC size is FLC_ENTRY_NUM * (1 << FLC_ADDR_SHIFT) * FLC_WAY_NUM(=1)
const uint32_t FlashCache::FLC_ENTRY_NUM = 4 * 1024; // The num of Entry
const uint32_t FlashCache::FLC_ADDR_SHIFT = 5; // Line size is 1 << FLC_ADDR_SHIFT. 5 means 32byte/line.

// Followings are fixed params or calcurated based on other param.
// Don't edit following params.
const uint32_t FlashCache::FLC_WAY_NUM = 1; // Fixed to 1.
const uint32_t FlashCache::FLC_LINE_BYTE = 1 << FLC_ADDR_SHIFT; // Line size by byte
const PhysAddr FlashCache::FLC_ADDR_MASK = ~(FLC_LINE_BYTE - 1); // address mask for Line
const FlcReplaceType FlashCache::FLC_REPLACE_TYPE = FLC_REP_ROUNDR; // Fixed to round-robin

bool FlashCache::IsHitFlc (uint32_t entry, PhysAddr load_addr) const
{
    return m_flc_entry[entry]->m_flc_paddr == (load_addr & FLC_ADDR_MASK);
}


uint64_t FlashCache::GetFlcCycle (uint32_t entry, PhysAddr addr) const
{
    return m_flc_entry[entry]->m_flc_avail_cycle[GetWayId (addr)];
}


uint32_t FlashCache::GetWayId (PhysAddr load_addr) const
{
    return static_cast<uint32_t>((load_addr & (FLC_LINE_BYTE - 1)) >>
            (1 + FLC_ADDR_SHIFT - FLC_WAY_NUM)) & (FLC_WAY_NUM - 1);
}


uint64_t FlashCache::UpdateFlc (uint32_t entry, PhysAddr load_addr, uint64_t ready_cycle,
        uint32_t repeat_cycle)
{
    uint32_t way_1st = GetWayId (load_addr);
    m_flc_entry[entry]->m_flc_avail_cycle[way_1st] = ready_cycle;

    m_flc_entry[entry]->m_flc_paddr = load_addr & FLC_ADDR_MASK;

    return ready_cycle;
}


uint32_t FlashCache::GetFlcEntry (PhysAddr load_addr) const
{
    if (FLC_REPLACE_TYPE == FLC_REP_ROUNDR) {
        for (uint32_t entry = 0; entry < FLC_ENTRY_NUM; entry++) {
            if (IsHitFlc (entry, load_addr) == true) {
                return entry;
            }
        }
        return 0; // Miss. (Return any.)
    } else {
        return static_cast<uint32_t>((load_addr & FLC_ADDR_MASK) >> FLC_ADDR_SHIFT) &
                                     (FLC_ENTRY_NUM - 1);
    }
}


uint32_t FlashCache::GetReplaceEntry (PhysAddr load_addr) const
{
    if (FLC_REPLACE_TYPE == FLC_REP_ROUNDR) {
        return m_round_robin_entry;
    } else {
        return GetFlcEntry (load_addr);
    }
}

void FlashCache::UpdateReplaceEntry (void)
{
    if (FLC_REPLACE_TYPE == FLC_REP_ROUNDR) {
        ++ m_round_robin_entry;
        if (FLC_ENTRY_NUM == m_round_robin_entry) {
            m_round_robin_entry = 0;
        }
    }
}


void FlashCache::DumpFlc (void) const
{
    CfMsg::DPrint (MSG_ERR, "FLC(P%u) entry (depth=%d)\n", m_peid, FLC_ENTRY_NUM);
    for (unsigned int j = 0; j < FLC_ENTRY_NUM; j++) {
        for (unsigned int i = 0; i < FLC_WAY_NUM; i++) {
            CfMsg::DPrint (MSG_ERR, "[%d]=%08" PRIx32 " @ %08" PRIu64 "cycle\n",
                       j, m_flc_entry[j]->m_flc_paddr, m_flc_entry[j]->m_flc_avail_cycle[i]);
        }
    }
}


void FlashCache::HardReset (void)
{
    for (uint32_t i = 0; i < FLC_ENTRY_NUM; i++) {
        m_flc_entry[i]->m_flc_paddr = ~FLC_ADDR_MASK; // DBUFヒットしない値
        for (uint32_t j = 0; j < FLC_WAY_NUM; j++) {
            m_flc_entry[i]->m_flc_avail_cycle[j] = 0;
        }
    }

    ResetPerfInfo ();
    m_round_robin_entry = 0;
}


void FlashCache::ShowPerfInfo (void) const
{
    if (m_hit_count + m_miss_count != 0) {
        double hit_rate = ( static_cast<double>(m_hit_count)
                          / static_cast<double>(m_hit_count + m_miss_count)
                          ) * 100.0;
        CfMsg::DPrint (MSG_ERR,
                "  FLC (P%u): Hit Rate=%3.3f%% (Hit=%8" PRIu64 ", Miss=%8" PRIu64 ")\n",
                m_peid, hit_rate, m_hit_count, m_miss_count);
    }
}


void FlashCache::ResetPerfInfo (void)
{
    m_hit_count = 0;
    m_miss_count = 0;
}


FlashCache::FlashCache (uint32_t peid) : m_peid (peid & CL_PEID_MASK)
{
    for (uint32_t i = 0; i < FLC_ENTRY_NUM; i++) {
        m_flc_entry.push_back (new FlcEntry (FLC_WAY_NUM));
    }
    HardReset ();
}


FlashCache::~FlashCache ()
{
    std::vector<FlcEntry*>::iterator it = m_flc_entry.begin ();
    while (it != m_flc_entry.end ()) {
        delete (*it);
        ++ it;
    }
}


FlashCache::FlcEntry::FlcEntry (uint32_t fill_burst_num)
{
    m_flc_avail_cycle.assign (fill_burst_num, 0UL);
}
