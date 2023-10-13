/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./forest_common.h"
#include "./inst_queue.h"
#include "./trace_info.h"
#include "./mpu.h"
#include "./lsu.h"


PhysAddr InstQueue::GetInstQueueAddr (void)
{
    return m_line_addr;
}


void InstQueue::IssueFetch (PhysAddr line_addr)
{
    line_addr &= ~LINE_MASK;
    m_line_syserr = SYSERR_NONE;
    m_line_addr  = line_addr;
    m_line_paddr = m_lsu->MemInstFetch (line_addr, m_line_data.d8, IQ_BYTES);
}


uint16_t InstQueue::FetchData (PhysAddr inst_addr)
{
    PhysAddr line_addr = inst_addr & ~LINE_MASK;
#if defined(FROG_API_IF)
#else // FROG_API_IF
    if (line_addr != m_line_addr) {
#endif // FROG_API_IF
        IssueFetch (line_addr);
        if (m_line_syserr_total == SYSERR_NONE) {
            // 先に発生したSYSERRが優先なので、以前にSYSERRがなかった場合だけ更新
            m_line_syserr_total = m_line_syserr;
        }
#if defined(FROG_API_IF)
#else // FROG_API_IF
    } else {
        // フェッチしない場合、命令キューに記録されたSYSERR情報を使用
        m_line_syserr_total = m_line_syserr;
    }
#endif // FROG_API_IF
    m_ic_hit_total &= m_ic_hit;
    uint32_t line_pos = (inst_addr & LINE_MASK) >> 1;
    return m_line_data.d16[line_pos];
}


void InstQueue::FetchOpcode (PhysAddr inst_addr)
{
    // PCのビット0は0固定なのでアライメント判定は不要
    uint16_t inst_code16 = FetchData (inst_addr);

    // トレースには命令の先頭アドレスを記録。
    // 命令後半フェッチのFetchData()を呼ぶと、m_line_paddrが後半アドレスに更新されるので注意。

    uint64_t inst_code64 = static_cast<uint64_t> (inst_code16);
    if ( ! IsInstSize16Bit (inst_code16) ) {
        uint16_t inst_code32 = FetchData (inst_addr + 2);
        inst_code64 |= (static_cast<uint64_t> (inst_code32) << 16);
    }
    m_trace->SetInstCode (inst_code64);

    // @@ 48bit超部分はデコードの後に実行
    // @@ RTL動作とは異なるので、CSでは辻褄をあわせること
}


bool InstQueue::IsInstSize16Bit (uint16_t inst_code16) const
{
    // @@ 命令仕様が全て決まったら再チェック必要
    bool ret
        = ( (inst_code16 & 0x0200U) == 0x0000U )
        | ( ((inst_code16 & 0xFFE0U) != 0x02E0U) && ((inst_code16 & 0x0600U) != 0x0600U) );
    return ret;

    // 上の式は以下と同じ意味
/*
    switch (inst_code & 0x0600U) {
    case 0x0000: ret = true;  break;
    case 0x0400: ret = true;  break;
    case 0x0600: ret = false; break;
    case 0x0200:
        if ((inst_code & 0xFFE0) == 0x02E0) {
            ret = false;
        } else {
            ret = true;
        }
        break;
    }
*/
}


bool InstQueue::IsInstSize48Bit (uint32_t inst_code32) const
{
    // @@ 命令仕様が全て決まったら再チェック必要
    // @@ Note: length of instruction can be detected by cheking 32bit-length code
    uint32_t op_15_6  = OperandExtract (inst_code32, 15, 6);
    uint32_t op_15_5  = OperandExtract (inst_code32, 15, 5);
    uint32_t op_16_16 = OperandExtract (inst_code32, 16, 16);
    uint32_t op_20_17 = OperandExtract (inst_code32, 20, 17);

    if ((op_15_6 == 0x01e) & (op_16_16 == 1)) {
        if (op_20_17 != 0x0 &&  // PREPARE_NO_EPWB (32bit)
            op_20_17 != 0x8 &&  // RIE (32bit)
            op_20_17 != 0x1 &&  // PREPARE_SPWB_EP (32bit)
            op_20_17 != 0xd) {  // PREPARE_IM32_EP (32bit)
            return true;
        }
    }
    if (op_15_5 == 0x017 && op_16_16 == 0) {
        // JRW / JRLW
        return true;
    }
    if (op_15_5 == 0x017 && op_16_16 == 1) {
        // Undefined 48bit inst in G4MH (In G3x, this is undefined 32bit inst)
        return true;
    }
    if (op_15_5 == 0x031) {
        // MOVW
        return true;
    }
    if (op_15_5 == 0x037 && op_16_16 == 0) {
        // JMPW
        return true;
    }

    return false;
}


void InstQueue::Init (void)
{
    m_line_syserr_total = SYSERR_NONE;
    m_line_syserr = SYSERR_NONE;
    m_line_addr  = INVALID_VADDR;
    m_line_paddr = INVALID_PADDR;
    m_ic_hit = false;
    m_ic_hit_total = true;
}


InstQueue::InstQueue (TraceCtrl* trace, Lsu* lsu, CoreType core_type)
    : IQ_BITS (64),
      IQ_HWORDS (IQ_BITS / 16), IQ_WORDS (IQ_BITS / 32), IQ_BYTES (IQ_BITS /  8), LINE_MASK (IQ_BYTES - 1),
      m_trace (trace),
      m_lsu   (lsu),
      m_line_syserr_total (SYSERR_NONE),
      m_line_syserr (SYSERR_NONE),
      m_line_addr  (INVALID_VADDR),
      m_line_paddr (INVALID_PADDR),
      m_ic_hit (false),
      m_ic_hit_total (true)
{
    m_lsu->SetInstQueue (this);
}
