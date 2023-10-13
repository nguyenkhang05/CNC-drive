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

#if defined(_MSC_VER)
inline div_t div (uint32_t num, uint32_t denom);
#endif // defined(_MSC_VER)
class MemLatencyInfoBase;
class FsCtrl;
class CsCtrl;

class CsIbufG3MH {
public:
    explicit CsIbufG3MH (uint32_t pe_id,
                         CoreType core_type,
                         FsCtrl* fsctrl,  CsCtrl* csctrl);
    ~CsIbufG3MH ();

    uint64_t GetFetchCycleInstBuf (PhysAddr addr) const;
    bool IsHitInstBuf (PhysAddr addr) const;
    void InitInstBuf (void);
    void ForceSetInstBuf (PhysAddr addr);
    void SetClkRatio (uint32_t ratio) { m_clk_ratio = ratio; }
    MemLatencyInfoBase* m_last_fetch_mem_info;
    void SimFetch (TraceCtrl* trace);
    void UpdateIaAddress (PhysAddr ia_addr, uint64_t ia_cycle);
    void ForceUpdateIaAddress (PhysAddr ia_addr);
    void UpdateIdAddress (PhysAddr id_addr, uint64_t id_cycle);
    uint64_t GetIaCycle (void) const { return m_ifu_cycle; }
    uint64_t GetIaCycle (PhysAddr inst_start_addr, PhysAddr inst_end_addr) const;
    uint64_t GetFetchCycle (TraceCtrl* trace);
    bool IsIcHit (PhysAddr inst_start_addr, PhysAddr inst_end_addr) const;
    bool IsHitOverrunFetch (PhysAddr addr) const;

private:
    uint32_t m_clk_ratio;

#ifdef SUPPORT_SC_HEAP
    static const uint32_t DUMMY_LATENCY_SCHEAP = 23; // Prevent fast access. 23 is fetch latency from CRAM.
#endif // SUPPORT_SC_HEAP

    class IbufType {
    public:
        PhysAddr addr;
        uint64_t fill_order;
        uint64_t fill_cycle; // BIU(res)
        uint64_t m_req_cycle; // IA
        bool m_hit_ic;
        bool m_is_valid;
        uint64_t m_released_cycle;
        IbufType () {}
        ~IbufType () {}
    };
    std::vector <IbufType*> m_ibuf;

    uint64_t m_fill_order;
    const uint32_t m_pe_id;  // PE跨ぎアクセスの判定用

    void SetFetchLatency (PhysAddr fetch_addr, uint64_t ia_cycle);
    uint64_t m_fetch_latency;
    static const uint64_t FILL_BURST_CYC = 4;
    static const uint64_t STAGE_IC = 2;

    void SetInstBuf (uint32_t pos, PhysAddr addr, uint64_t cycle, bool is_hit_ic);
    uint32_t GetInstBuf (PhysAddr addr) const;
    uint32_t GetInstBuf (PhysAddr addr, uint64_t fill_order) const;
    uint32_t GetInstBufReverse (PhysAddr addr) const;
    //uint32_t UpdateInstBuf (PhysAddr inst_addr, uint64_t fetch_cycle);
    void DumpInstBuf (void) const;

    static const uint32_t IQ_BYTES = 8;
    static const PhysAddr LINE_MASK = IQ_BYTES - 1;
    static const uint32_t IBUF_NUM = 5;
    const CoreType m_core_type;
    FsCtrl* m_fsctrl;
    CsCtrl* m_csctrl;

    void CancelFetch (void);
    void SimPrefetch (PhysAddr inst_addr);
    void FetchFromPrefetch (PhysAddr ia_addr);
    void PrefetchFromBiu (PhysAddr ia_addr);
    void FetchFromBiu (PhysAddr ia_addr);
    void FetchFromReqedBiu (PhysAddr ia_addr);
    void FetchFromIc (PhysAddr ia_addr);
    uint64_t CalcNextResponse (PhysAddr ia_addr, PhysAddr req_addr, uint64_t cycle) const;
    bool IsIcHit (PhysAddr inst_addr, TraceCtrl* trace) const;
    void SimIfq (PhysAddr ia_addr, TraceCtrl *trace);
    void IncIaAddr (void);
    bool IsBiuUsable (void) const;
    bool HasEmptyIfqEntry (void) const;
    void SimFetchStep (PhysAddr ia_addr, TraceCtrl* trace);

    void UpdateInstBufLru (uint32_t entry);
    uint32_t GetInstBufEntryFromLru (void) const { return 0; }
    bool IsHitPrefetch (PhysAddr ia_addr) const;
    bool ConflictFillReq (void);
    bool IsHitBiuReq (PhysAddr ia_addr) const;

    void InvalidateIfq (PhysAddr ia_addr, uint64_t release_cycle,
                        bool copy_to_id, uint64_t fill_order);
    void SimFetchIa (PhysAddr inst_addr, TraceCtrl* trace);

    class IdPointer {
    public:
        PhysAddr m_id_addr;
        uint64_t m_id_cycle;
        uint64_t m_fill_order;
        bool m_need_copy;
        IdPointer (PhysAddr id_addr, uint64_t id_cycle, uint64_t fill_order)
          : m_id_addr (id_addr), m_id_cycle (id_cycle),
            m_fill_order (fill_order), m_need_copy (true) {}
        ~IdPointer () {}
    };
    std::vector<IdPointer> m_id_pointer;

    void CheckIbufRelease (uint64_t ifu_cycle);

    PhysAddr m_id_addr;
    uint64_t m_id_cycle;
    bool m_activate_prefetch;
    uint64_t m_ifu_cycle;
    uint64_t m_biu_req_avail_cycle;
    PhysAddr m_req_ia_addr;
    PhysAddr m_ia_addr;
    uint64_t m_biu_res_avail_cycle;
    MemAccessRoute m_last_fetch_route;
    PhysAddr m_last_ifq_addr;
    uint64_t m_branch_cycle;
    PhysAddr m_branch_tgt_ia;

    PhysAddr m_last_ic_line_addr;
    bool m_last_ic_hit;

    struct IdBufType {
        PhysAddr m_addr;
        PhysAddr m_base_addr;
        uint64_t m_fill_cycle; // BIU(res)
        uint64_t m_req_cycle; // IA
        uint64_t m_finish_cycle;
        uint64_t m_fill_order;
        bool m_hit_ic;
    };

    std::vector <IdBufType> m_idbuf;
    enum IdBufPos {
        IDBUF_READ = 0,
        IDBUF_SKID = 1,
        IDBUF_NUM  = 2
    };
    std::vector <IdBufType> m_ia_biu;
    std::vector <IdBufType> m_prefetch;

    int32_t GetPrefetchedEntry (PhysAddr inst_addr) const;
    int32_t GetIaReqedEntry(PhysAddr inst_addr) const;
    bool CanIssueReq (uint64_t ia_cycle, uint32_t limit_num) const;

    void ClearPrefetchBuffer (PhysAddr branch_tgt_addr);

    std::vector<uint64_t> m_fill_req_cycle;
    PhysAddr m_last_executed_addr;
    uint64_t m_last_executed_order;

    void ClearIdbuf (PhysAddr addr);
    void ClearUnusedIdbuf (PhysAddr addr);
    void ClearUnusedIdbuf (PhysAddr addr1, PhysAddr addr2);
    void InvalidateUnusedIbuf (void);
    TraceCtrl* m_trace;

    static const uint32_t NUM_RECORD_OVERRUN_FETCH = 4;
    std::vector<PhysAddr> m_over_prefetched_addr;
    void RecordOverrunFetch (IbufType* ibuf);
};
