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


class CsLsuBuf
{
public:
    CsLsuBuf (uint32_t num_lsu_buf, uint32_t num_nb);
    ~CsLsuBuf ();

    uint64_t GetBuffAvailCycle (void) const;
    uint64_t GetBuffAvailCycleG3MH (bool is_parallel) const;
    uint64_t GetBuffAvailCycleG3MH_LD (bool is_parallel) const;
    uint64_t GetBuffAvailCycleG3KH (void) const;
    uint64_t GetReqAvailCycle (void) const;
    uint64_t GetFinishCycle (void) const;
    uint64_t GetAmuAvailCycle (void) const;
    uint64_t GetSyncCycle (void) const;
    void ReserveBuffer (uint64_t release_cycle);
    void ReserveBufferG3KH (uint64_t release_cycle);
    void ReserveBufferScheap (void);
    void SetNotifiedLatencyScheap (uint64_t release_cycle);
    void ReleaseScheapWaiting (uint64_t release_cycle);
    void InitBufCycle (void);
    bool IsWaitSCHEAP (void) const;
    void SetIssueCycle (uint64_t issue_cycle);

private:
    static const uint64_t WAIT_SCHEAP = 0xFFFFFFFFFFFFFFFFULL;
    static const uint32_t NUM_LSU_BUF_ST_G4MH = 5; // Additional buffer for ST

    const uint32_t m_num_lsu_buf;
    const uint32_t m_total_buf;
    std::vector<uint64_t> m_buf_avail_cycle;
    uint64_t m_issue_cycle;
    uint64_t m_last_release_cycle;
    bool m_is_used_1slot;

    void DumpOutstandQueue (void) const;
};
