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

class CsIbuf {
public:
    explicit CsIbuf (uint32_t pe_id, CoreType core_type, FsCtrl* fsctrl,  CsCtrl* csctrl);
    ~CsIbuf ();

    uint32_t SimInstBuf (PhysAddr inst_addr, uint64_t fetch_cycle);
    uint32_t PreloadInstBuf (uint64_t fetch_cycle);
    void SetLatencyToIBuf (TraceCtrl* trace);
    uint64_t GetFetchCycleInstBuf (PhysAddr addr) const;
    bool IsHitInstBuf (PhysAddr addr) const;
    void InitInstBuf (void);
    void ForceSetInstBuf (PhysAddr addr);
    bool IsFetchLatencyReceived (TraceCtrl* trace);
    bool SetFetchLatencySCHEAP (TraceCtrl* trace, PhysAddr addr, uint32_t latency);
    void SetupFetchLatency (TraceCtrl* trace);
    void SetRomCycle (uint64_t btgt_fetch_cycle);

    void SetPreloadLimitAddr (PhysAddr addr) { m_preload_limit_addr = addr; }
    PhysAddr GetPreloadLimitAddr (void) { return m_preload_limit_addr; }
    void SetPreloadTiming (uint64_t cycle) { m_ibuf_cycle = cycle; }

    void SetClkRatio (uint32_t ratio) { m_clk_ratio = ratio; }

    MemLatencyInfoBase* m_last_fetch_mem_info;

    static const uint32_t IBUF_2WAY = 2;
    static const uint32_t IBUF_3WAY = 3;
    static const uint32_t IBUF_4WAY = 4;
    static const uint32_t IBUF_1WAY = 1;

private:
    uint32_t m_clk_ratio;

#ifdef SUPPORT_SC_HEAP
    // fetch latency for unsupported case in scheap.
    static const uint32_t DUMMY_LATENCY_SCHEAP = 11; // Prevent fast access. 11 is latency from GRAM.
#endif //#ifdef SUPPORT_SC_HEAP

    struct IbufType {
        PhysAddr addr;
        uint64_t fill_order;
        uint64_t fill_latency;
        uint64_t fill_start;
        bool already_set_latency;
    };
    uint64_t m_fill_order;
    uint64_t m_update_timing;
    uint64_t m_update_cycle;
    IbufType *m_ibuf; // 命令バッファエントリ
    uint32_t m_ibuf_lru;    // 命令バッファLRU

    uint32_t m_pe_id;  // PE跨ぎアクセスの判定用
    PhysAddr m_ibuf_addr;  // 現在のフェッチアドレス
    PhysAddr m_ibuf_preload_addr; // 次のPreload対象アドレス
    PhysAddr m_preload_limit_addr;

    uint64_t m_ibuf_cycle;  // 命令バッファのフェッチ開始サイクル
    uint64_t m_rom_available_cycle;  // 命令ROMの使用可能サイクル

    uint64_t m_fetch_latency[REC_FETCH_NUM];
    uint64_t m_fetch_repeat[REC_FETCH_NUM];

    void SetInstBuf (uint32_t pos, PhysAddr addr, uint64_t cycle);
    uint32_t GetInstBuf (PhysAddr addr) const;

    uint32_t UpdateInstBuf (PhysAddr inst_addr, uint64_t fetch_cycle);
    void UpdateInstBufLru (uint32_t entry);
    uint32_t GetInstBufEntryFromLru (void) const;
    void DumpInstBuf (void) const;

    const uint32_t IQ_BYTES = 16;
    const PhysAddr LINE_MASK = IQ_BYTES - 1;
    const uint32_t IBUF_NUM = 7;
    const uint32_t m_preload_size = 16;
    const CoreType m_core_type;
    FsCtrl* m_fsctrl;
    CsCtrl* m_csctrl;
};
