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

class TraceCtrl;
class Lsu;

class InstQueue {
public:
    MemData16 FetchData (PhysAddr inst_addr);
    void IssueFetch  (PhysAddr line_addr);
    void FetchOpcode (PhysAddr inst_addr);
    void Init (void);
    PhysAddr GetInstQueueAddr (void);
    void SetTraceCtrl (TraceCtrl* trace) { m_trace = trace; }

    // 1フェッチ(ライン）ごとのSYSERR情報を記録
    void SetSyserr (SysErrCause cause) { m_line_syserr = cause; }

    // IC HIT/MISS result for 1line (8Byte)
    void SetIcHit (bool is_hit) { m_ic_hit = is_hit; }
    bool IsIcHitTotal (void) const { return m_ic_hit; }
    void InitIcHitTotal (void) { m_ic_hit_total = true; }

    // 1命令の複数フェッチで発生したSYSERR累積情報を返す
    SysErrCause GetSyserrTotal (void) const { return m_line_syserr_total; }
    void InitSyserrTotal (void) { m_line_syserr_total = SYSERR_NONE; }

    bool IsInstSize16Bit (uint16_t inst_code) const;
    bool IsInstSize48Bit (uint32_t inst_code32) const;

    uint32_t OperandExtract (uint64_t x, uint32_t l, uint32_t r) const {
        return static_cast<uint32_t> ((x >> r) & ~(~0x0ULL << (l - r + 1)));
    }

    InstQueue (TraceCtrl* trace, Lsu* lsu, CoreType core_type);

private:
    const uint32_t IQ_BITS;
    const uint32_t IQ_HWORDS;
    const uint32_t IQ_WORDS;
    const uint32_t IQ_BYTES;
    const PhysAddr LINE_MASK;

    TraceCtrl* m_trace;
    Lsu* m_lsu;

    SysErrCause m_line_syserr_total;

    // キュー内ループは行わないので 1ライン分だけ実装
    SysErrCause m_line_syserr;
    PhysAddr m_line_addr;
    PhysAddr m_line_paddr;  // @@ -debug 表示用 いらないかも
    union {
        MemData16 d16[128/16]; // オペコード用
        MemData8  d8 [128/8];  // Lsuアクセス用
    } m_line_data;

    bool m_ic_hit;
    bool m_ic_hit_total;

    InstQueue ();
};


