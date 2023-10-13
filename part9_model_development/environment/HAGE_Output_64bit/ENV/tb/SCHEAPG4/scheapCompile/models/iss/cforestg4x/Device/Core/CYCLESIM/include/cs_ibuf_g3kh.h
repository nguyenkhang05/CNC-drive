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

class CsIbufG3KH {
public:
    explicit CsIbufG3KH (uint32_t pe_id, CoreType core_type, CoreVersion core_ver,
                         FsCtrl* fsctrl,  CsCtrl* csctrl);
    ~CsIbufG3KH ();

    uint64_t GetFetchCycleInstBuf (PhysAddr addr) const;
    bool IsHitInstBuf (PhysAddr addr) const;
    bool IsHitBraBuf (PhysAddr addr) const;
    void InitInstBuf (void);
    void InitFetchBuf (void);
    void ResetFetchBuf (void);
    void InitBraBuf (void);
    void ForceSetInstBuf (PhysAddr addr);
    void SetClkRatio (uint32_t ratio) { m_clk_ratio = ratio; }
    MemLatencyInfoBase* m_last_fetch_mem_info;
    void SimFetch (TraceCtrl* trace);
    void UpdateIaAddress (PhysAddr ia_addr, uint64_t ia_cycle);
    void ForceUpdateIaAddress (PhysAddr ia_addr);
    uint64_t GetIaCycle (void) const { return m_ifu_cycle; }
    uint64_t GetIaCycle (PhysAddr inst_start_addr, PhysAddr inst_end_addr) const;
    uint64_t GetFetchCycle (TraceCtrl* trace);
    void SetFetchInfoSCHEAP (TraceCtrl* trace);
    bool SetFetchLatencySCHEAP (TraceCtrl* trace, PhysAddr addr, uint32_t latency);
    bool IsFetchLatencyReceived (TraceCtrl* trace);
    void SetIfuStartFetchAfterBranch (uint64_t cycle){ m_ifu_cycle = cycle; m_branch_cycle = cycle;}
    bool m_is_branch_target;
private:
    uint32_t m_clk_ratio;

#ifdef SUPPORT_SC_HEAP
    // fetch latency for unsupported case in scheap.
    static const uint32_t DUMMY_LATENCY_SCHEAP = 18; // Prevent fast access. 18 is latency from GRAM.
#endif //#ifdef SUPPORT_SC_HEAP
    static const uint32_t NUM_BRA_LINE_G3KH20 = 4;
    static const uint32_t NUM_BRA_WAY_G3KH20 = 4;
    static const uint32_t NUM_FETCH_LINE_G3KH20 = 1;		//fetch buffer: 1 line - 16 bytes

    enum BraReplaceType {
        BRABUF_FIFO    = 0, // branch$ replacement type FIFO
        BRABUF_LRU    = 1, // branch$ replacement type FIFO
    };
    BraReplaceType m_bra_repl_type;
    uint32_t m_way_index[NUM_BRA_LINE_G3KH20];

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

    //fetch buffer, data of prefetching from ROM
    class FetchbufType {
    public:
        PhysAddr addr;
        bool m_is_valid;
        uint64_t prefetch_cyc;
        FetchbufType () {}
        ~FetchbufType () {}
    };
    std::vector <FetchbufType*> m_fetchbuf;

    //branch cache buffer, using when branch taken,
    //store previous target branch opcode
    class BrabufType {
    public:
        PhysAddr addr[NUM_BRA_WAY_G3KH20];
        bool m_is_valid[NUM_BRA_WAY_G3KH20];
        BrabufType () {}
        ~BrabufType () {}
    };
    std::vector <BrabufType*> m_brabuf;

    uint64_t m_fill_order;
    const uint32_t m_pe_id;  // PE跨ぎアクセスの判定用

    void SetFetchLatency (PhysAddr fetch_addr, uint64_t ia_cycle);
    uint64_t m_fetch_latency;
    static const uint64_t FILL_BURST_CYC = 4;
    static const uint64_t STAGE_IC = 1;

    uint64_t m_scheap_latency[REC_FETCH_NUM];

    void SetInstBuf (uint32_t pos, PhysAddr addr, uint64_t cycle, bool is_hit_ic);
    uint32_t GetInstBuf (PhysAddr addr) const;
    uint32_t GetBraBuf (PhysAddr addr) const;
    //uint32_t UpdateInstBuf (PhysAddr inst_addr, uint64_t fetch_cycle);
    void DumpInstBuf (void) const;

    const uint32_t IQ_BYTES = 16;
    const PhysAddr LINE_MASK = IQ_BYTES - 1;
    const uint32_t IBUF_NUM = 7;
    const CoreType m_core_type;
    const CoreVersion m_core_version;
    FsCtrl* m_fsctrl;
    CsCtrl* m_csctrl;

    void ResetInstBuf (void);
    void SimPrefetch (PhysAddr inst_addr, uint64_t cycle);
    void FetchFromPrefetch (PhysAddr ia_addr);
    void PrefetchFromBiu (PhysAddr ia_addr);
    void FetchFromBiu (PhysAddr ia_addr);
    void FetchFromReqedBiu (PhysAddr ia_addr);
    void FetchFromFetchBuffer (PhysAddr ia_addr);
    void FetchFromPrefetchBuffer (PhysAddr ia_addr);
    void FetchFromBranchBuffer (PhysAddr ia_addr);
    void PrefetchToFetchBuffer (PhysAddr ia_addr, uint64_t cycle);
    void UpdateBraBuf (PhysAddr inst_addr);
    uint64_t CalcNextResponse (PhysAddr ia_addr, PhysAddr req_addr, uint64_t cycle) const;
    bool IsFetchBufferHit (PhysAddr inst_addr, TraceCtrl* trace) const;
    void SimIfq (PhysAddr ia_addr, TraceCtrl *trace);
    void IncIaAddr (void);
    bool HasEmptyIfqEntry (void) const;
    void SimFetchStep (PhysAddr ia_addr, TraceCtrl* trace);

    uint32_t GetInstBufEntryFromLru (void) const;
    bool ConflictFillReq (void);

    void InvalidateIfq (PhysAddr ia_addr, uint64_t release_cycle,
                        bool copy_to_id, uint64_t fill_order);
    void SimFetchIa (PhysAddr inst_addr, TraceCtrl* trace);
    uint32_t IndexIfqEntry (void) const;
    void IbufEntryRelease (PhysAddr addr);

    void CheckIbufRelease (uint64_t ifu_cycle);

    PhysAddr m_id_addr;
    uint64_t m_id_cycle;
    bool m_activate_prefetch;
    bool m_ifq_fetch;
    bool prefetch_limit;
    PhysAddr prev_branch_addr;

    uint64_t m_ifu_cycle;
    bool m_if_valid;
    uint64_t m_biu_req_avail_cycle;
    PhysAddr m_req_ia_addr;
    PhysAddr m_ia_addr;
    uint64_t m_biu_res_avail_cycle;
    MemAccessRoute m_last_fetch_route;
    PhysAddr m_last_ifq_addr;
    uint64_t m_branch_cycle;
    PhysAddr m_branch_tgt_ia;
    bool m_parallel_flag;

    PhysAddr m_last_ic_line_addr;
    PhysAddr m_prev_bra_addr;
    bool m_corner_hit;
    bool m_is_branch_buff_hit;
    bool m_is_branch_buff_hit_1st;
    bool m_is_prev_branch_tgt_16;

    bool m_last_ic_hit;

    struct IdBufType {
        PhysAddr m_addr;
        PhysAddr m_base_addr;
        uint64_t m_fill_cycle; // BIU(res)
        uint64_t m_req_cycle; // IA
        uint64_t m_finish_cycle;
        uint64_t m_fill_order;
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
};
