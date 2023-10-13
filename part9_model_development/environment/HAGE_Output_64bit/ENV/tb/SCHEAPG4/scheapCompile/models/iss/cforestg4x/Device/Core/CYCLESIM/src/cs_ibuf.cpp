/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include "./fs_controler.h"
#include "./cs_controler.h"
#include "./inst_queue.h"
#include "./cs_ibuf.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./proc_element.h"
#include "./outstand.h"


// 命令バッファの初期化
void CsIbuf::InitInstBuf (void)
{
    m_update_timing = 0;
    m_update_cycle = 0;

    m_ibuf_cycle = 0;
    m_rom_available_cycle = 0;
    m_fill_order = 0;
    m_ibuf_lru = 0;
    m_ibuf_addr = 0;
    m_ibuf_preload_addr = 0;
    m_preload_limit_addr = 0;
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        m_ibuf [i].addr = LINE_MASK; // 決してヒットしない値に設定
        m_ibuf [i].fill_order = 0;
        m_ibuf [i].fill_latency = 0;
        m_ibuf [i].fill_start = 0;
        m_ibuf [i].already_set_latency = true;
    }

    for (int32_t i = 0; i < REC_FETCH_NUM; i++) {
        m_fetch_latency [i] = 0;
        m_fetch_repeat  [i] = 0;
    }

    // ForceSetInstBuf()でFS->CSに切り替えた場合、IBUF登録なしにCSが情報を使うかもしれないので、
    // NULLではなく仮の有効なポインタを渡しておく
#ifdef CLUSTER
    m_last_fetch_mem_info = p_internals->GetMemResourceInfo (m_pe_id, 0, INST_READ);
#else // CLUSTER
    m_last_fetch_mem_info = p_internals->GetMemResourceInfo (m_pe_id, 0);
#endif // CLUSTER
}


// 命令バッファに新規登録
// fill_latencyは実際にFS側で命令アクセスした時に格納。
// already_set_latencyはfill_latency格納済みかどうかの検出用
void CsIbuf::SetInstBuf (uint32_t pos, PhysAddr addr, uint64_t cycle)
{
    m_ibuf[pos].addr  = addr;
    m_ibuf[pos].fill_order = ++ m_fill_order;
    m_ibuf[pos].fill_latency = 0;
    m_ibuf[pos].fill_start = cycle;
    m_ibuf[pos].already_set_latency = false;
}


// アドレスに対応する命令バッファエントリ番号の取得
// ミスの場合、無効なエントリ番号(IBUF_NUM)を返す
uint32_t CsIbuf::GetInstBuf (PhysAddr addr) const
{
    addr &= ~LINE_MASK;
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        if (addr == m_ibuf[i].addr) {
            return i;
        }
    }

    return IBUF_NUM;
}


// 指定アドレスが命令バッファに存在するかチェック
bool CsIbuf::IsHitInstBuf (PhysAddr addr) const
{
    uint32_t entry = GetInstBuf (addr);
    if (entry != IBUF_NUM) {
        return true;
    }

    return false;
}


// 命令バッファエントリのダンプ
void CsIbuf::DumpInstBuf (void) const
{
    CfMsg::DPrint (MSG_ERR, "Dump InstBuf contents\n");
    CfMsg::DPrint (MSG_ERR, "m_rom_available_cycle=%lu\n", m_rom_available_cycle);
    for (uint32_t i = 0; i < IBUF_NUM; i++) {
        CfMsg::DPrint (MSG_ERR, "[%d] addr=%08x, order=%" PRIu64 ", latency=%" PRIu64 "\n",
                       i, m_ibuf[i].addr, m_ibuf[i].fill_order, m_ibuf[i].fill_latency);
    }
}


// 命令バッファからフェッチのレイテンシ情報を取得する。
uint64_t CsIbuf::GetFetchCycleInstBuf (PhysAddr addr) const
{
    uint32_t entry = GetInstBuf (addr);

    // 無効なエントリをチェック (正しく動作する限りこのifにはマッチしないはず)
    if (entry == IBUF_NUM) {
        CfMsg::DPrint (MSG_ERR, "<Internal Error: Miss at GetFetchCycleInstBuf>\n");
        return 0;
    }
    if (m_ibuf[entry].already_set_latency == false) {
        CfMsg::DPrint (MSG_ERR, "<Internal Error: Not set at GetFetchCycleInstBuf>\n");
        return 0;
    }

    return m_ibuf[entry].fill_latency;
}


// 命令バッファのシミュレート
// 引数で指定された命令アドレスとフェッチ時間から、
// 命令バッファエントリを更新する。
uint32_t CsIbuf::SimInstBuf (PhysAddr inst_addr, uint64_t fetch_cycle)
{
    inst_addr &= ~LINE_MASK;

    m_ibuf_addr = inst_addr;
    m_ibuf_preload_addr = inst_addr + m_preload_size;
    if (UpdateInstBuf (inst_addr, fetch_cycle) != 0) {
        // 命令バッファにミスしたので、フェッチ発行。
        // 命令バッファにヒットするなら、以前に先行フェッチ済みなので不要
        if (m_ibuf_cycle < fetch_cycle) {
            m_ibuf_cycle = fetch_cycle;
        } else {
            ++ m_ibuf_cycle;
        }
        return 1; // フェッチ回数を返す
    } else {
        return 0; // フェッチ回数を返す
    }
}


// 現在時間まで、先行フェッチ動作を進める。
// PC+IQ、+IQ*2先までを先行フェッチする。
uint32_t CsIbuf::PreloadInstBuf (uint64_t fetch_cycle)
{
    uint32_t fetch_cnt = 0;

    if (fetch_cycle > m_ibuf_cycle) {
        if (  (m_ibuf_addr + m_preload_size == m_ibuf_preload_addr)
           && (m_ibuf_preload_addr <= m_preload_limit_addr)) {
            //+IQの先行フェッチ実行
            fetch_cnt += UpdateInstBuf (m_ibuf_preload_addr, m_ibuf_cycle);
            m_ibuf_preload_addr += m_preload_size;

            // バッファヒットミスに関係なく、1サイクル進める
            ++ m_ibuf_cycle;
        }

        if (fetch_cycle > m_ibuf_cycle) {
            if (  (m_ibuf_addr + m_preload_size*2 == m_ibuf_preload_addr)
               && (m_ibuf_preload_addr <= m_preload_limit_addr)) {
                //+IQ*2の先行フェッチ実行
                fetch_cnt += UpdateInstBuf (m_ibuf_preload_addr, m_ibuf_cycle);
                m_ibuf_preload_addr += m_preload_size;

                // バッファヒットミスに関係なく、1サイクル進める
                ++ m_ibuf_cycle;
            }
        }

//        if (m_core_type == CORE_G3MH) {
//            if (fetch_cycle > m_ibuf_cycle) {
//                if ((m_ibuf_addr + m_preload_size*3 == m_ibuf_preload_addr) &&
//                        (m_ibuf_preload_addr <= m_preload_limit_addr)) {
//                    //+0xIQ*3の先行フェッチ実行
//                    UpdateInstBuf (m_ibuf_preload_addr, m_ibuf_cycle);
//                    m_ibuf_preload_addr += m_preload_size;
//
//                    // バッファヒットミスに関係なく、1サイクル進める
//                    ++ m_ibuf_cycle;
//                }
//            }
//
//            if (fetch_cycle > m_ibuf_cycle) {
//                if ((m_ibuf_addr + m_preload_size*4 == m_ibuf_preload_addr) &&
//                        (m_ibuf_preload_addr <= m_preload_limit_addr)) {
//                    //+0xIQ*3の先行フェッチ実行
//                    UpdateInstBuf (m_ibuf_preload_addr, m_ibuf_cycle);
//                    m_ibuf_preload_addr += m_preload_size;
//
//                    // バッファヒットミスに関係なく、1サイクル進める
//                    ++ m_ibuf_cycle;
//                }
//            }
//        }

        m_ibuf_cycle = fetch_cycle;
    }

    return fetch_cnt;
}


void CsIbuf::SetRomCycle (uint64_t btgt_fetch_cycle)
{
    m_update_timing = m_fill_order;
    m_update_cycle = btgt_fetch_cycle;
}


// 指定アドレスについて命令バッファを更新
uint32_t CsIbuf::UpdateInstBuf (PhysAddr inst_addr, uint64_t fetch_cycle)
{
    uint32_t hit_entry = GetInstBuf(inst_addr);
    if (hit_entry != IBUF_NUM) {
        // 命令バッファにヒットしたのでLRU更新
        UpdateInstBufLru (hit_entry);
        return 0;
    } else {
        // 命令バッファにミスしたのでエントリの入れ替えとLRU更新
        uint32_t replace_entry = GetInstBufEntryFromLru ();

        if (m_rom_available_cycle > fetch_cycle) {
            fetch_cycle = m_rom_available_cycle;
        }
        SetInstBuf (replace_entry, inst_addr, fetch_cycle);

        UpdateInstBufLru (replace_entry);
        return 1;
    }
}


// 命令バッファのフィルレイテンシを格納
// フェッチ側で実際にアクセスした場合に呼び出される
void CsIbuf::SetLatencyToIBuf (TraceCtrl* trace)
{
    // メモリアクセスのレイテンシを取得
    int32_t fetch_cnt = trace->GetFetchCount ();
    for (int32_t i = 0; i < fetch_cnt; i++) {
        // 該当する命令バッファにレイテンシを格納
        uint32_t pos = GetInstBuf (trace->GetFetchAddr (i));
        if (pos < IBUF_NUM) {
            if (m_ibuf[pos].already_set_latency == true) {
                // すでにIbufにフィル済み。
                // IQミス、IBUFヒットでここに来る
                continue;
            }

            // 命令フェッチは順番通りに実行されるはずなので、
            // 以前のバッファが未設定なら、非実行部分の先行フェッチが存在したということ。
            // 以前のバッファから順に時間を設定して、自アクセスのレイテンシをその分長くする
            uint64_t curr_order = m_ibuf[pos].fill_order;
            for (uint32_t j = 0; j < IBUF_NUM; j++) {
                // 以前のIBUFで時間未設定のエントリを埋める
                if ( (m_ibuf[j].already_set_latency == false)
                  && (curr_order > m_ibuf[j].fill_order) ) {
                    if (m_rom_available_cycle < m_update_cycle) {
                        if (m_ibuf[j].fill_order > m_update_timing) {
                            m_rom_available_cycle = m_update_cycle;
                        }
                    }

                    // @@ 非実行部分の先行フェッチレイテンシは不明。
                    //    とりあえず今のレイテンシを使ってみる
                    //    @@ 不正確なので、データ側のrom時間を進めるのはやめておく？？？
                    if (m_ibuf[j].fill_start > m_rom_available_cycle) {
                        m_rom_available_cycle = m_ibuf[j].fill_start;
                    }
                    m_ibuf[j].fill_latency = m_fetch_latency[i] + m_rom_available_cycle;
                    m_rom_available_cycle += m_fetch_repeat[i];
                    m_ibuf[j].already_set_latency = true;
                }
            }

            if (m_rom_available_cycle < m_update_cycle) {
                if (curr_order > m_update_timing) {
                    m_rom_available_cycle = m_update_cycle;
                }
            }

            // フィル開始タイミングがROM使用可能タイミングより後なら、
            // ROM使用可能タイミングを更新
            if (m_ibuf[pos].fill_start > m_rom_available_cycle) {
                m_rom_available_cycle = m_ibuf[pos].fill_start;
            }

            m_ibuf[pos].fill_latency = m_fetch_latency[i] + m_rom_available_cycle;
            m_rom_available_cycle += m_fetch_repeat[i];
            m_ibuf[pos].already_set_latency = true;
        } else {
            // FS側で使用しているのに存在しない、ということはありえないので、
            // もしここにきたら何かおかしい。
            CfMsg::DPrint (MSG_ERR, "Internal Error: SetLatencyToIBuf()\n");
        }
        //DumpInstBuf ();
    }
}


void CsIbuf::SetupFetchLatency (TraceCtrl* trace)
{
    uint64_t issue_ia_cycle;
    if (m_ibuf_cycle > m_csctrl->GetCycleCount ()) {
        uint64_t issue_diff = (m_ibuf_cycle - m_csctrl->GetCycleCount ()) * m_clk_ratio;
        issue_ia_cycle = m_fsctrl->m_cpu_time + issue_diff;
    } else {
        uint64_t issue_diff = (m_csctrl->GetCycleCount () - m_ibuf_cycle) * m_clk_ratio;
        issue_ia_cycle = m_fsctrl->m_cpu_time - issue_diff;
    }

    int32_t fetch_cnt = trace->GetFetchCount ();
	uint32_t latency;
    for (int32_t i = 0; i < fetch_cnt; i++) {
        if (trace->GetFetchRoute (i) == ICACHE_HIT_ROUTE) {
            m_fetch_latency[i] = 0;
            m_fetch_repeat[i] = 0;
//        } else if (trace->GetFetchRoute (i) == AXC_HIT_ROUTE) {
//            m_fetch_latency[i] = p_internals->GetAxcHitLatency ();
//            m_fetch_repeat[i] = m_fetch_latency[i];
        } else {
            PhysAddr fetch_addr = trace->GetFetchAddr (i);
#ifdef CLUSTER
            MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr, INST_READ);
#else // CLUSTER
            MemLatencyInfoBase* mem_info = p_internals->GetMemResourceInfo (m_pe_id, fetch_addr);
#endif // CLUSTER

            m_last_fetch_mem_info = mem_info;

            // @@ フェッチのoutstanding対応時には、データ側と同様のクロック比計算が必要
            //    現在、フェッチレイテンシのアウトスタンディング未対応のため計算省略
            if (p_forest_util->GetCoreType(m_pe_id) ==  CORE_G4KH){
                 latency = 0;
            } else {
                 latency = mem_info->GetFetchLatency (m_pe_id, fetch_addr, issue_ia_cycle, IQ_BYTES);
            }
#ifdef SUPPORT_SC_HEAP
            if (mem_info->IsScheap() || (latency == TARGET_IS_SCHEAP)) {
#ifdef WAIT_FETCH_SCHEAP
                m_fetch_latency[i] = TARGET_IS_SCHEAP;
#else // #ifdef WAIT_FETCH_SCHEAP
		m_fetch_latency[i] = DUMMY_LATENCY_SCHEAP;
#endif // #ifdef WAIT_FETCH_SCHEAP
		m_fetch_repeat[i] = 1;
                continue;
            }
#endif // SUPPORT_SC_HEAP
            // latencyはsystime基準。PEのクロック基準に換算する
            // FREQ周期で時間を進めるため、stall量をFREQ単位に切り上げ
            // @@ 現在outstand未対応なので、あらかじめ計算しておけるはず
            div_t result = div (latency, m_clk_ratio);

            uint32_t latency_pe_base = result.quot;
            if (result.rem != 0) {
                latency_pe_base += 1;
            }
            m_fetch_latency[i] = latency_pe_base;

            uint32_t repeat_latency = mem_info->GetConflictLatency ();

            // latencyはsystime基準。PEのクロック基準に換算する
            // FREQ周期で時間を進めるため、stall量をFREQ単位に切り上げ
            // @@ 現在outstand未対応なので、あらかじめ計算しておけるはず
            div_t result_repeat = div (repeat_latency, m_clk_ratio);

            uint32_t repeat_pe_base = result_repeat.quot;
            if (result_repeat.rem != 0) {
                repeat_pe_base += 1;
            }
            m_fetch_repeat[i] = repeat_pe_base; // repeat is fixed in a same target.
        }
    }
}


bool CsIbuf::IsFetchLatencyReceived (TraceCtrl* trace)
{
#ifdef WAIT_FETCH_SCHEAP
    int32_t fetch_cnt = trace->GetFetchCount ();
    for (int32_t i = 0; i < fetch_cnt; i++) {
        if (m_fetch_latency[i] == TARGET_IS_SCHEAP) {
            return false;
        }
    }
#endif // #ifdef WAIT_FETCH_SCHEAP
    return true;
}


bool CsIbuf::SetFetchLatencySCHEAP (TraceCtrl* trace, PhysAddr addr, uint32_t latency)
{
#ifdef WAIT_FETCH_SCHEAP
    if (m_core_type == CORE_G3KH) {
        // Fixed to 0 for G3KHv1 temporally.
        latency = 0;
    }

    int32_t fetch_cnt = trace->GetFetchCount ();
    for (int32_t i = 0; i < fetch_cnt; i++) {
        if (addr == trace->GetFetchPAddr (i)) {
            if (m_fetch_latency[i] == TARGET_IS_SCHEAP) {
                m_fetch_latency[i] = latency;
                return true;
            } else {
                break;
            }
        }
    }
    return false;
#else // #ifdef WAIT_FETCH_SCHEAP
    return true;
#endif // #ifdef WAIT_FETCH_SCHEAP
}


// 命令バッファのLRUを更新
void CsIbuf::UpdateInstBufLru (uint32_t entry)
{
    static const uint32_t lru_on_mask_master[IBUF_4WAY] =
        { 0x38, 0x06, 0x01, 0x00 };
    static const uint32_t lru_off_mask_master[IBUF_4WAY] =
        { 0x3F, 0x1F, 0x2B, 0x34 };

    uint32_t lru_data = m_ibuf_lru;
    uint32_t new_lru = (lru_data | lru_on_mask_master[entry])
                       & lru_off_mask_master[entry];
    m_ibuf_lru = new_lru;
}


// LRUに基づいて命令バッファエントリを選択
uint32_t CsIbuf::GetInstBufEntryFromLru (void) const
{
    static const uint32_t lru_sel_mask[IBUF_4WAY] = { 0x38, 0x26, 0x15, 0x0B };
    static const uint32_t lru_chk_val[IBUF_4WAY]  = { 0x00, 0x20, 0x14, 0x0B };

    static const uint32_t lru3_sel_mask[IBUF_3WAY] = { 0x30, 0x24, 0x14 };
    static const uint32_t lru3_chk_val[IBUF_3WAY]  = { 0x00, 0x20, 0x14 };

    static const uint32_t lru2_sel_mask[IBUF_2WAY] = { 0x20, 0x20 };
    static const uint32_t lru2_chk_val[IBUF_2WAY]  = { 0x00, 0x20 };

    uint32_t lru_data = m_ibuf_lru;
    if (IBUF_NUM == IBUF_4WAY) {
        // 4WAY用
        for (uint32_t i = 0; i < IBUF_NUM; i++) {
            if ((lru_data & lru_sel_mask[i]) == lru_chk_val[i]) {
                return i;
            }
        }
        return 0; // ここには来ない
    } else if (IBUF_NUM == IBUF_2WAY) {
        // 2WAY用
        for (uint32_t i = 0; i < IBUF_NUM; i++) {
            if ((lru_data & lru2_sel_mask[i]) == lru2_chk_val[i]) {
                return i;
            }
        }
        return 0; // ここには来ない
    } else if (IBUF_NUM == IBUF_3WAY) {
        // 3WAY用 (IBUF2 + ICUEUE2構成用。IBUFとIcueueは1エントリ分同じデータを持つ)
        for (uint32_t i = 0; i < IBUF_NUM; i++) {
            if ((lru_data & lru3_sel_mask[i]) == lru3_chk_val[i]) {
                return i;
            }
        }
        return 0; // ここには来ない
    } else if (IBUF_NUM == IBUF_1WAY){
    	return 1;
	} else {
        // enum定義が上記以外存在しないので、ここのコードは生成されない
        CfMsg::DPrint (MSG_ERR, "<Internal Error at GetInstBufEntryFromLru()>\n");
        return IBUF_NUM;
    }
}


// FSからCSに切り替わった際、FS命令キュー内容をCS命令バッファに書き込む
// FS側で命令キューからフェッチされた場合、
// CS側命令バッファに存在しない命令アドレスが登場してしまうので。
void CsIbuf::ForceSetInstBuf (PhysAddr addr)
{
    SimInstBuf (addr & ~LINE_MASK, 0);

    uint32_t pos = GetInstBuf (addr);
    if (pos < IBUF_NUM) {
        m_ibuf[pos].fill_order = 0;
        m_ibuf[pos].fill_latency = 0;
        m_ibuf[pos].already_set_latency = true;
    }
}


CsIbuf::CsIbuf (uint32_t pe_id, CoreType core_type,
                FsCtrl* fsctrl,  CsCtrl* csctrl)
         : m_pe_id (pe_id), m_core_type (core_type),
           m_fsctrl (fsctrl),  m_csctrl (csctrl)
{
    m_ibuf = new IbufType [IBUF_NUM];
    InitInstBuf ();
}


CsIbuf::~CsIbuf ()
{
    delete [] m_ibuf;
}


#if defined(_MSC_VER)
inline div_t div (uint32_t num, uint32_t denom)
{
    return div (static_cast<int> (num), static_cast<int> (denom));
}
#endif // _MSC_VER
