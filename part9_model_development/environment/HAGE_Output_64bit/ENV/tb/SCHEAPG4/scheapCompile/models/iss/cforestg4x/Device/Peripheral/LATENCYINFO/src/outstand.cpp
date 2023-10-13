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
#include <vector>
#include <iostream>
#include <sstream>
#include "./config.h"
#include "./outstand.h"
#include "./outstand_mem.h"
#include "./forest_message.h"
#include "./forest_internals.h"
#include "./forest_utility.h"
#include "./dbuf.h"
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
#include "./flash_cache.h"
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)

#define ENABLE_PRELOAD_DBUF_ADAPT

template <MemDbufType dbuf_type>
template <MemAccessCommand cmd>
uint32_t MemLatencyInfo<dbuf_type>::GetMemModuleLatency (uint32_t peid) const
{
    return m_latency_info[peid][cmd];
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfo<dbuf_type>::GetRmwLatency (ByteSize size) const
{
    if (size < SIZE_32BIT) {
        // GRAMではR/M/Wで3リクエスト相当になる様子。1回分は折込済みなので、2回分を加算
        // @@ TCM未対応
        if (dbuf_type == DBUF_CRAM_G4MH){
            // G3MH LRAM/GRAM
            return  m_repeat_interval_cycle + 1;
        } else {
            // G3M LRAM/GRAM
            return  m_repeat_interval_cycle << 1;
        }
    }
    return 0;
}


template <MemDbufType dbuf_type>
template <MemAccessCommand cmd>
uint32_t MemLatencyInfo<dbuf_type>::GetAccessLatency (uint32_t cl_peid, PhysAddr load_addr,
                                                      uint64_t start_cycle, ByteSize size)
{
    uint32_t peid = cl_peid & CL_PEID_MASK;
    uint32_t mem_latency = GetMemModuleLatency<cmd> (peid);
    if (mem_latency == 0) {
#ifdef ENABLE_DCACHE
        return 3;
#else // ENABLE_DCACHE
        return mem_latency;
#endif // ENABLE_DCACHE
    }

    if ((cmd == INST_READ) &&
        (dbuf_type != DBUF_FROM_G4MH)) {
        // @@ Temporal workaround: Because prefetch timing is before sytem_cycle,
        //                         prefetch timing isn't matched well with data timing.
        //                         For fetch latency on G3M, conflict latency is ignored.
        return mem_latency;
    }

    // 発行タイミングの計算
    uint32_t issue_stall; // リクエスト発行までのストールサイクル
    uint64_t issue_cycle; // リクエスト発行される時間
    int64_t stall_cycle = m_mem_avail_cycle.front () - start_cycle;
    if (stall_cycle > 0) {
        // outstandバッファの空きがないので、発行タイミングを遅らせる
        issue_stall = static_cast<uint32_t> (stall_cycle);
        issue_cycle = m_mem_avail_cycle.front ();
    } else {
        issue_stall = 0;
        issue_cycle = start_cycle;
    }

#ifdef CLUSTER
    uint32_t base_mem_latency = mem_latency;
#endif // #ifdef CLUSTER

    if (dbuf_type == DBUF_FROM_G4MH) {
        // DBUF for G4MH
        if (cmd != m_last_acc_type) {
            // 命令とデータの競合では競合によるレイテンシを1個下げてみる
            m_next_interval_cycle = m_repeat_interval_cycle - 1;
        } else {
            m_next_interval_cycle = m_repeat_interval_cycle;
        }
        if (cmd == INST_READ) {
#ifdef ENABLE_FLC_I
            FlashCache* flc = p_internals->GetFlcPtr (peid);
            uint32_t flc_entry = flc->GetFlcEntry (load_addr);
            if (flc->IsHitFlc (flc_entry, load_addr) == true) {
                // Hit FLC : Reduce latency
                uint64_t flc_avail_cycle = flc->GetFlcCycle (flc_entry, load_addr);
                uint32_t flc_hit_latency = FlashCache::FLC_HIT_LATENCY_I;

                if (issue_cycle >= flc_avail_cycle) {
                    mem_latency = flc_hit_latency;
                } else {
                    // Fill by previous same entry isn't finished. Wait finished transfer.
                    uint32_t flc_stall = static_cast <uint32_t> (flc_avail_cycle - issue_cycle);
                    mem_latency = std::max (flc_stall, flc_hit_latency);
                }
                CfMsg::CachePrint ("[FLC_HIT:P%u:e%1X]\n", flc->m_peid, flc_entry);
                flc->IncrementHit ();
            } else {
                // When FLC is missed, flash latency is used.
                if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
                    // FlasROM latency +1 in DBUF miss case when access master is different from previous
                    issue_cycle += 1;
                    issue_stall += 1;
                    m_last_acc_peid = peid;
                    m_last_acc_cmd  = cmd;
                }
                mem_latency = GetNoBufLatency (mem_latency, issue_cycle);

                // When FLC is missed, Fill to FLC.
                flc_entry = flc->GetReplaceEntry (load_addr);
                flc->UpdateFlc (flc_entry, load_addr, issue_cycle + mem_latency + 1,
                                 m_repeat_interval_cycle);
                flc->UpdateReplaceEntry ();
                CfMsg::CachePrint ("[FLC_MISS:P%u:e%1X]\n", flc->m_peid, flc_entry);
                flc->IncrementMiss ();
            }
#else // ENABLE_FLC_I
            // Disable DBUF on fetch.
            if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
                // FlasROM latency +1 in DBUF miss case when access master is different from previous
                issue_cycle += 1;
                issue_stall += 1;
                m_last_acc_peid = peid;
                m_last_acc_cmd  = cmd;
            }
            mem_latency = GetNoBufLatency (mem_latency, issue_cycle);
#endif // ENABLE_FLC_I
            UpdateMemAvailCycle (issue_cycle + mem_latency);
        } else {
#if _UNIT_TEST
            Dbuf* dbuf = m_dbuf;
#else // #if _UNIT_TEST
            Dbuf* dbuf = p_internals->GetDbufPtr (peid);
#endif // #if _UNIT_TEST
#ifdef ENABLE_PRELOAD_DBUF_ADAPT
            PhysAddr last_fill_addr = dbuf->GetLastFillAddr();
            //printf ("Last fill addr=%08x\n", last_fill_addr);
#endif //#ifdef ENABLE_PRELOAD_DBUF_ADAPT
            mem_latency = GetDbufLatency (peid, load_addr, mem_latency, issue_cycle, dbuf, cmd);
            if (IsBufHitLast(load_addr)) {
                // DBUFヒット時にはROMアクセスしない。LVCI競合を回避するだけで、メモリ時間を進めない。
                // issue_cycle + 1にするが、フィル中ラインに対するアクセスあるので、
                    // レイテンシから計算する。
                UpdateMemAvailCycle (issue_cycle + mem_latency - Dbuf::DBUF_HIT_LATENCY_G4MH);
#ifdef CLUSTER
                // Ignore conflict info on FILL DBUF entry
                m_last_cl_peid = cl_peid;
#endif // CLUSTER

            } else {
                if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
                    // FlasROM latency +1 in DBUF miss case when access master is different from previous
                    issue_cycle += 1;
                    issue_stall += 1;
                    m_last_acc_peid = peid;
                    m_last_acc_cmd  = cmd;
                }
                UpdateMemAvailCycle (issue_cycle + m_dbuf_fill_cycle);
            }

#ifdef ENABLE_PRELOAD_DBUF_ADAPT
            PhysAddr last_preload_addr = dbuf->GetLastPreloadAddr();

            // @@ Need to fix the fill cycle.
            PhysAddr load_line_addr = load_addr & 0xFFFFFFE0;
            if ((load_line_addr == last_fill_addr + 0x20) || ((load_line_addr == last_preload_addr) && dbuf->IsPreloadPlus())) {
                // preload + 0x20
                PhysAddr preload_addr = load_line_addr + 0x20;
                uint32_t entry = dbuf->GetDbufEntry (preload_addr);
                if (dbuf->IsHitDbuf (entry, preload_addr) == false) {
#ifdef ENABLE_FLC_D
                    uint64_t preload_fill_latency;
		    FlashCache* flc = p_internals->GetFlcPtr (peid);
		    uint32_t flc_entry = flc->GetFlcEntry (load_addr);
		    if (flc->IsHitFlc (flc_entry, load_addr) == true) {
		        // Hit FLC : Reduce latency
		        uint64_t flc_avail_cycle = flc->GetFlcCycle (flc_entry, load_addr);
		        uint32_t flc_hit_latency = FlashCache::FLC_HIT_LATENCY_R;

		        if (issue_cycle >= flc_avail_cycle) {
		            preload_fill_latency = flc_hit_latency;
		        } else {
		            // Fill by previous same entry isn't finished. Wait finished transfer.
		            uint32_t flc_stall = static_cast <uint32_t> (flc_avail_cycle - issue_cycle);
		            preload_fill_latency = std::max (flc_stall, flc_hit_latency);
		        }
		        CfMsg::CachePrint ("[FLC_HIT:P%u:e%1X]\n", flc->m_peid, flc_entry);
		        flc->IncrementHit ();
                        dbuf->UpdateDbufPreload (entry, preload_addr, issue_cycle + preload_fill_latency);
		    } else {
		        // When FLC is missed, flash latency is used.
		        preload_fill_latency = m_dbuf_fill_cycle + m_repeat_interval_cycle; // GetNoBufLatency (mem_latency, issue_cycle);
		        // When FLC is missed, Fill to FLC.
		        flc_entry = flc->GetReplaceEntry (load_addr);
		        flc->UpdateFlc (flc_entry, load_addr, issue_cycle + preload_fill_latency,
		                        m_repeat_interval_cycle);
		        flc->UpdateReplaceEntry ();
		        CfMsg::CachePrint ("[FLC_MISS:P%u:e%1X]\n", flc->m_peid, flc_entry);
		        flc->IncrementMiss ();

                        dbuf->UpdateDbufPreload (entry, preload_addr, issue_cycle + preload_fill_latency);
		        if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
		            // FlasROM latency +1 in DBUF miss case when access master is different from previous
		            issue_cycle += 1;
		            issue_stall += 1;
		            m_last_acc_peid = peid;
		            m_last_acc_cmd  = cmd;
		        }
                        UpdateMemAvailCycle (issue_cycle + preload_fill_latency);
		    }
#else // #ifdef ENABLE_FLC_D
                    uint64_t preload_fill_latency = m_dbuf_fill_cycle + m_repeat_interval_cycle;
                    dbuf->UpdateDbufPreload (entry, preload_addr, issue_cycle + preload_fill_latency);
                    if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
                        // FlasROM latency +1 in DBUF miss case when access master is different from previous
                        issue_cycle += 1;
                        issue_stall += 1;
                        m_last_acc_peid = peid;
                        m_last_acc_cmd  = cmd;
                    }
                    UpdateMemAvailCycle (issue_cycle + preload_fill_latency);
#endif // #ifdef ENABLE_FLC_D
                    //printf ("preload+=%08lx\n", preload_addr);
                    //dbuf->DumpDbuf();
                    dbuf->SetPreloadPlus (true);
                    CfMsg::CachePrint ("[DBUF_PLD:P%u]\n", dbuf->m_peid);
                    dbuf->IncrementMiss ();
                }
            } else if (    ((load_line_addr == (last_fill_addr - 0x20)) && (load_line_addr != 0))
                        || ((load_line_addr == last_preload_addr) && (dbuf->IsPreloadPlus() == false) && (load_line_addr != 0))) {
                // preload - 0x20
                PhysAddr preload_addr = load_line_addr - 0x20;
                uint32_t entry = dbuf->GetDbufEntry (preload_addr);
                if (dbuf->IsHitDbuf (entry, preload_addr) == false) {
#ifdef ENABLE_FLC_D
                    uint64_t preload_fill_latency;
		    FlashCache* flc = p_internals->GetFlcPtr (peid);
		    uint32_t flc_entry = flc->GetFlcEntry (load_addr);
		    if (flc->IsHitFlc (flc_entry, load_addr) == true) {
		        // Hit FLC : Reduce latency
		        uint64_t flc_avail_cycle = flc->GetFlcCycle (flc_entry, load_addr);
		        uint32_t flc_hit_latency = FlashCache::FLC_HIT_LATENCY_R;

		        if (issue_cycle >= flc_avail_cycle) {
		            preload_fill_latency = flc_hit_latency;
		        } else {
		            // Fill by previous same entry isn't finished. Wait finished transfer.
		            uint32_t flc_stall = static_cast <uint32_t> (flc_avail_cycle - issue_cycle);
		            preload_fill_latency = std::max (flc_stall, flc_hit_latency);
		        }
		        CfMsg::CachePrint ("[FLC_HIT:P%u:e%1X]\n", flc->m_peid, flc_entry);
		        flc->IncrementHit ();
                        dbuf->UpdateDbufPreload (entry, preload_addr, issue_cycle + preload_fill_latency);
		    } else {
		        // When FLC is missed, flash latency is used.
		        preload_fill_latency = m_dbuf_fill_cycle + m_repeat_interval_cycle; // GetNoBufLatency (mem_latency, issue_cycle);
		        // When FLC is missed, Fill to FLC.
		        flc_entry = flc->GetReplaceEntry (load_addr);
		        flc->UpdateFlc (flc_entry, load_addr, issue_cycle + preload_fill_latency,
		                        m_repeat_interval_cycle);
		        flc->UpdateReplaceEntry ();
		        CfMsg::CachePrint ("[FLC_MISS:P%u:e%1X]\n", flc->m_peid, flc_entry);
		        flc->IncrementMiss ();

                        dbuf->UpdateDbufPreload (entry, preload_addr, issue_cycle + preload_fill_latency);
		        if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
		            // FlasROM latency +1 in DBUF miss case when access master is different from previous
		            issue_cycle += 1;
		            issue_stall += 1;
		            m_last_acc_peid = peid;
		            m_last_acc_cmd  = cmd;
		        }
                        UpdateMemAvailCycle (issue_cycle + preload_fill_latency);
		    }
#else // #ifdef ENABLE_FLC_D
                    uint64_t preload_fill_latency = m_dbuf_fill_cycle + m_repeat_interval_cycle;
                    dbuf->UpdateDbufPreload (entry, preload_addr, issue_cycle + preload_fill_latency);
                    if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
                        // FlasROM latency +1 in DBUF miss case when access master is different from previous
                        issue_cycle += 1;
                        issue_stall += 1;
                        m_last_acc_peid = peid;
                        m_last_acc_cmd  = cmd;
                    }
                    UpdateMemAvailCycle (issue_cycle + preload_fill_latency);
#endif // #ifdef ENABLE_FLC_D
                    //printf ("preload-=%08lx\n", preload_addr);
                    //dbuf->DumpDbuf();
                    dbuf->SetPreloadPlus (false);
                    CfMsg::CachePrint ("[DBUF_PLD:P%u]\n", dbuf->m_peid);
                    dbuf->IncrementMiss ();
                }
            }
#endif // ENABLE_PRELOAD_DBUF_ADAPT

        }
        m_last_acc_type = cmd;
    } else if (dbuf_type == DBUF_FROM_G4KH) {
        // DBUF for G3KH20
        Dbuf* dbuf = p_internals->GetDbufPtr (peid);
        mem_latency = GetDbufLatencyG4KH<cmd> (load_addr, mem_latency, issue_cycle, dbuf);
        if (cmd == DATA_READ) {
            //+1: penalty due to no forwarding in ROM
            mem_latency = mem_latency +1;
        }
        if (IsBufHitLast(load_addr)) {
            // DBUFヒット時にはROMアクセスしない。LVCI競合を回避するだけで、メモリ時間を進めない。
            // issue_cycle + 1にするが、フィル中ラインに対するアクセスあるので、
            // レイテンシから計算する。
            UpdateMemAvailCycle (issue_cycle + mem_latency - dbuf->DBUF_HIT_LATENCY_G4KH );
        } else {
            //+1: LSU penalty when accessing ROM: arbitrator switch from IFU to LSU
            mem_latency = mem_latency +1;
            UpdateMemAvailCycle (issue_cycle + mem_latency);
        }
    } else if (dbuf_type == DBUF_CRAM_G4MH) {
        // No DBUF
        // In case of 1byte or 2byte access, because RMW occurs internally,
        // latency and repet rate are increased.
        if ((cmd == DATA_WRITE) && (size < SIZE_32BIT)) {
            mem_latency += GetRmwLatency (size);
#ifdef CLUSTER
            base_mem_latency = mem_latency;
#endif // #ifdef CLUSTER

            // Update interval cycle for RMW
            // LD/ST : RAM is 2 cycles. Pre-response is sent at last cycle of RAM.
            // RMW   : RAM is 4 cycles. Pre-response is sent at last cycle + 1 of RAM.
            m_next_interval_cycle = (m_repeat_interval_cycle << 1) + 1;
            mem_latency = GetNoBufLatency (mem_latency, issue_cycle);
            //m_next_interval_cycle = m_repeat_interval_cycle + 1;
        } else {
            // restore interval cycle.
            mem_latency = GetNoBufLatency (mem_latency, issue_cycle);
            m_next_interval_cycle = m_repeat_interval_cycle;
        }
        //mem_latency = GetNoBufLatency (mem_latency, issue_cycle);
        // restore interval cycle.
        m_next_interval_cycle = m_repeat_interval_cycle;

        UpdateMemAvailCycle (issue_cycle + mem_latency);
    } else if (dbuf_type == DBUF_CRAM_G4KH) {
        // GRAM with no DBUF
        bool is_post_write = false;
        if (cmd == DATA_WRITE){
            is_post_write = true;
        }
        mem_latency = GetNoBufLatencyG4KH (mem_latency, issue_cycle, &is_post_write);
        if (cmd == DATA_READ) {
            //+1: penalty due to no forwarding in ROM
            mem_latency = mem_latency +1;
        }
        UpdateMemAvailCycle (issue_cycle + mem_latency);

    } else {
        // DBUF_SIMPLE_MEM
        mem_latency = GetNoBufLatency (mem_latency, issue_cycle);
        UpdateMemAvailCycle (issue_cycle + mem_latency);
    }

#ifdef CLUSTER
    if (p_forest_util->IsEnableBusTrace ()) {
        if (m_last_cl_peid != cl_peid) {
            // Ignore conflict with ownself.
            if ((issue_stall != 0) || (base_mem_latency < mem_latency)) {
                // conflict in cluster bridge.
                //++ m_conflict_count;
                {
                    std::stringstream route_str;
                    route_str << "<Conflict Memory " << "P" << (cl_peid & PEID_5BIT_MASK) << "-->";

                    //uint32_t ram_peid, ram_clid;
                    CfMsg::DPrint (MSG_INF, "%s", route_str.str().c_str());
                    CfMsg::DPrint (MSG_INF, "%s(%08x)>\n",
                            GetName(load_addr).c_str(), load_addr);
                }
            }
        }
        m_last_cl_peid = cl_peid;
    }
#endif // CLUSTER

#ifdef DEBUG_CS_MEM
    DumpOutstandQueue ();
    CfMsg::DPrint (MSG_ERR, "G:mem_latency=%u, issue_stall=%u, issue_cycle=%lu, start_cycle=%lu\n",
             mem_latency, issue_stall, issue_cycle, start_cycle);
    if (mem_latency + issue_stall >= 200) {
        CfMsg::DPrint (MSG_ERR, "<Internal Error (Doubtful behavior): Illeguler stall cycle>\n");
    }
#endif // #ifdef DEBUG_CS_MEM
    return mem_latency + issue_stall;
}


template <>
template <MemAccessCommand cmd>
uint32_t MemLatencyInfo<DBUF_LRAM_G4MH>::GetAccessLatency (uint32_t cl_peid, PhysAddr load_addr,
                                                      uint64_t start_cycle, ByteSize size)
{
    uint32_t peid = cl_peid & CL_PEID_MASK;
    uint32_t mem_latency = GetMemModuleLatency<cmd> (peid);
    if (cmd == INST_READ) {
        return mem_latency;
    }

    // DBUFなし
    // 1byte/2byteアクセス時に内部でRMW発生し、LoadのタイミングでR動作
    PhysAddr acc_word_addr = load_addr & 0xFFFFFFFCUL;
    if ((cmd == DATA_WRITE) && (size < SIZE_32BIT)) {
        if (m_last_rmw_addr != acc_word_addr) {
            GetAccessLatency <DATA_READ>(cl_peid, load_addr, start_cycle, size);
            m_last_rmw_addr = acc_word_addr;
            mem_latency += m_repeat_interval_cycle << 1;
            m_next_interval_cycle = (m_repeat_interval_cycle << 1) + 1;
        }
    } else {
        m_last_rmw_addr = 0xFFFFFFFFUL;
    }

    // 発行タイミングの計算
    uint32_t issue_stall; // リクエスト発行までのストールサイクル
    uint64_t issue_cycle; // リクエスト発行される時間
    int64_t stall_cycle = m_mem_avail_cycle.front () - start_cycle;
    if (stall_cycle > 0) {
        // outstandバッファの空きがないので、発行タイミングを遅らせる
        issue_stall = static_cast<uint32_t> (stall_cycle);
        issue_cycle = m_mem_avail_cycle.front ();
    } else {
        issue_stall = 0;
        issue_cycle = start_cycle;
    }

#ifdef CLUSTER
    uint32_t base_mem_latency = mem_latency;
#endif // #ifdef CLUSTER

    uint64_t mem_access_cycle;
    if (cmd == DATA_WRITE) {
        // WriteでのLRAMアクセスはレイテンシサイクルの最後のサイクル
        mem_access_cycle = issue_cycle + mem_latency;
    } else {
        // Loadでは1cycle目にRAMアクセス
        mem_access_cycle = issue_cycle + 1;
        if (m_last_st_addr == (load_addr & 0xFFFFFFFCUL)) {
            mem_access_cycle = std::max (mem_access_cycle, m_last_st_cycle);
            issue_stall = static_cast<uint32_t> (mem_access_cycle - 1 - start_cycle);
        }
    }
    //mem_latency += GetTcmAdditinalLatency (mem_access_cycle);
    uint64_t mem_avail_cycle = GetTcmAdditinalLatency (mem_access_cycle);
    if (cmd == DATA_WRITE) {
        // WriteでのLRAMアクセスはレイテンシサイクルの最後のサイクル
        mem_access_cycle += m_next_interval_cycle;
        if (mem_avail_cycle > mem_access_cycle) {
            mem_latency += static_cast<uint32_t> (mem_avail_cycle - mem_access_cycle);
        }
        m_last_st_addr = load_addr & 0xFFFFFFFCUL;
        m_last_st_cycle = mem_latency + issue_cycle + 1;
    } else {
        // Loadでは1cycle目にRAMアクセス
        mem_access_cycle += m_next_interval_cycle;
        if (mem_avail_cycle > mem_access_cycle) {
            // Note for "+1"
            // For L1RAM:
            // The fastest case, L1RAM access is EA=addres&CEN, EB=RAM, EC=AL, CMT=WB.
            // In this case, address calc and CEN are in EA stage together.
            // When access is stalled in AMU, address and CEN will processed in different cycle.
            // For example, EA (address), EB=stall, EC=address, EC+1=CEN, EC+2=RAM, EC+3=AL.
            mem_latency += static_cast<uint32_t> (mem_avail_cycle - mem_access_cycle) + 1;
        }
    }

    // 1byte/2byteアクセス時に内部でRMW発生し、レイテンシ増加
    if ((cmd == DATA_WRITE) && (size < SIZE_32BIT)) {
        //m_next_interval_cycle = (m_repeat_interval_cycle << 1) + 1;
        m_next_interval_cycle = m_repeat_interval_cycle;
    } else {
        m_next_interval_cycle = m_repeat_interval_cycle;
    }

#ifdef CLUSTER
    if (p_forest_util->IsEnableBusTrace ()) {
        if (m_last_cl_peid != cl_peid) {
            // Ignore conflict with ownself.
            if ((issue_stall != 0) || (base_mem_latency < mem_latency)) {
                // conflict in cluster bridge.
                //++ m_conflict_count;
                {
                    std::stringstream route_str;
                    route_str << "<Conflict Memory " << "P" << (cl_peid & PEID_5BIT_MASK) << "-->";

                    //uint32_t ram_peid, ram_clid;
                    CfMsg::DPrint (MSG_INF, "%s", route_str.str().c_str());
                    CfMsg::DPrint (MSG_INF, "%s(%08x)>\n",
                            GetName(load_addr).c_str(), load_addr);
                }
            }
        }
        m_last_cl_peid = cl_peid;
    }
#endif // CLUSTER

#ifdef DEBUG_CS_MEM
    DumpOutstandQueue ();
    CfMsg::DPrint (MSG_ERR, "L:mem_latency=%u, issue_stall=%u, issue_cycle=%lu\n",
            mem_latency, issue_stall, issue_cycle);
    if (mem_latency + issue_stall >= 200) {
        CfMsg::DPrint (MSG_ERR, "<Internal Error (Doubtful behavior): Illeguler stall cycle>\n");
    }
#endif // #ifdef DEBUG_CS_MEM
    return mem_latency + issue_stall;
}


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::UpdateMemAvailCycle (uint64_t avail_cycle)
{
    // アクセス履歴更新
    // サイズは常にoutstanding数
    m_mem_avail_cycle.front () = avail_cycle;
    std::sort (m_mem_avail_cycle.begin(), m_mem_avail_cycle.end());
}


template <MemDbufType dbuf_type>
bool MemLatencyInfo<dbuf_type>::IsBufHit (uint32_t peid, PhysAddr addr) const
{
    return false;
}


template <>
bool MemLatencyInfo<DBUF_FROM_G4MH>::IsBufHit (uint32_t peid, PhysAddr addr) const
{
#if _UNIT_TEST
    Dbuf* dbuf = m_dbuf;
#else // #if _UNIT_TEST
    Dbuf* dbuf = p_internals->GetDbufPtr (peid);
#endif // #if _UNIT_TEST
    uint32_t entry = dbuf->GetDbufEntry (addr);
    return dbuf->IsHitDbuf (entry, addr);
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfo<dbuf_type>::GetDbufLatency (uint32_t peid, PhysAddr load_addr,
        uint32_t mem_latency, uint64_t issue_cycle, Dbuf* dbuf, MemAccessCommand cmd)
{
    uint32_t entry = dbuf->GetDbufEntry (load_addr);
    if (dbuf->IsHitDbuf (entry, load_addr) == true) {
        // DBUFヒット：レイテンシを削減
        uint64_t dbuf_avail_cycle = dbuf->GetDbufCycle (entry, load_addr);
        uint32_t dbuf_hit_latency = Dbuf::DBUF_HIT_LATENCY_G4MH;

        if (issue_cycle >= dbuf_avail_cycle) {
            // DBUFのフィル完了しているので、DBUFヒット時レイテンシを使用
            mem_latency = dbuf_hit_latency;
            // When previous is buf hit, wait hit access.
            // Don't wait previous miss access. (Miss access can be overwrapped.
            if (issue_cycle + 2 < m_fill_lock_cycle) {
                mem_latency += static_cast<uint32_t> ((m_fill_lock_cycle - 2) - issue_cycle);
            }
        } else {
            // DBUFのフィル未完了。フィル完了 or DBUFヒット時レイテンシの大きい方を使用
            uint32_t dbuf_stall = static_cast <uint32_t> (dbuf_avail_cycle - issue_cycle);
            mem_latency = std::max (dbuf_stall, dbuf_hit_latency);

            // Lock next dbuf access when access to DBUF filling entry
            m_fill_lock_cycle = dbuf_avail_cycle;
        }
        m_is_buf_hit = true;
        CfMsg::CachePrint ("[DBUF_HIT:P%u:e%1X]\n", dbuf->m_peid, entry);
        dbuf->IncrementHit ();

        m_dbuf_fill_cycle = mem_latency;
    } else {
        // DBUF無効ならバッファなしメモリのレイテンシを使う

        if ((m_last_acc_peid != peid) || (m_last_acc_cmd != cmd)) {
            // FlasROM latency +1 in DBUF miss case when access master is different from previous
            issue_cycle += 1;
        }

        // When previous is buf hit, wait hit access.
        // Don't wait previous miss access. (Miss access can be overwrapped.
        if (issue_cycle < m_fill_lock_cycle) {
            mem_latency += static_cast<uint32_t> (m_fill_lock_cycle - issue_cycle);
        }

#ifdef ENABLE_FLC_D
        FlashCache* flc = p_internals->GetFlcPtr (peid);
        uint32_t flc_entry = flc->GetFlcEntry (load_addr);
        if (flc->IsHitFlc (flc_entry, load_addr) == true) {
            // Hit FLC : Reduce latency
            uint64_t flc_avail_cycle = flc->GetFlcCycle (flc_entry, load_addr);
            uint32_t flc_hit_latency = FlashCache::FLC_HIT_LATENCY_R; // DBUF is used only for data access

            if (issue_cycle >= flc_avail_cycle) {
                mem_latency = flc_hit_latency;
            } else {
                // Fill by previous same entry isn't finished. Wait finished transfer.
                uint32_t flc_stall = static_cast <uint32_t> (flc_avail_cycle - issue_cycle);
                mem_latency = std::max (flc_stall, flc_hit_latency);
            }
            CfMsg::CachePrint ("[FLC_HIT:P%u:e%1X]\n", flc->m_peid, flc_entry);
            flc->IncrementHit ();
            // In case of DBUF_MISS && FLC_HIT, to skip memory access, set as DBUF_HIT.
            // Just change latency for FLC_HIT.
            m_dbuf_fill_cycle = mem_latency;
            m_is_buf_hit = true;
        } else {
            // When FLC is missed, flash latency is used.
            mem_latency = GetNoBufLatency (mem_latency, issue_cycle);
            m_is_buf_hit = false;
        }
#else // ENABLE_FLC_D
        mem_latency = GetNoBufLatency (mem_latency, issue_cycle);
        m_is_buf_hit = false;
#endif // ENABLE_FLC_D

        // DBUFあればDBUF更新
        entry = dbuf->GetReplaceEntry (load_addr);

        uint64_t end_fill_cycle;
        if (dbuf_type == DBUF_FROM_G4MH) {
            end_fill_cycle = dbuf->UpdateDbuf (entry, load_addr, issue_cycle + mem_latency + 1,
                                               m_repeat_interval_cycle);
            m_dbuf_fill_cycle = end_fill_cycle - issue_cycle - 1;
            dbuf->UpdateReplaceEntry ();

            // バーストなので、+1でフィルできる
            dbuf->UpdateDbufFillNext (entry, load_addr, end_fill_cycle + 1);
#ifdef ENABLE_PRELOAD_DBUF
            // @@ Need to fix the fill cycle.
            dbuf->UpdateDbufPreload (entry, load_addr, end_fill_cycle + m_repeat_interval_cycle);
#endif // ENABLE_PRELOAD_DBUF
        } else {
            dbuf->UpdateDbuf (entry, load_addr, issue_cycle + mem_latency + 2,
                              m_repeat_interval_cycle);
            m_dbuf_fill_cycle = mem_latency;
            dbuf->UpdateReplaceEntry ();
        }

#ifdef ENABLE_FLC_D
        if (m_is_buf_hit == false) {
            // When FLC is missed, Fill to FLC.
            flc_entry = flc->GetReplaceEntry (load_addr);
            flc->UpdateFlc (flc_entry, load_addr, issue_cycle + mem_latency + 1,
                            m_repeat_interval_cycle);
            flc->UpdateReplaceEntry ();
            CfMsg::CachePrint ("[FLC_MISS:P%u:e%1X]\n", flc->m_peid, flc_entry);
            flc->IncrementMiss ();
        }
#endif // ENABLE_FLC_D

        CfMsg::CachePrint ("[DBUF_MISS:P%u:e%1X]\n", dbuf->m_peid, entry);
        dbuf->IncrementMiss ();
    }
    //dbuf->DumpDbuf();
    return mem_latency;
}



template <MemDbufType dbuf_type>
template <MemAccessCommand cmd>
uint32_t MemLatencyInfo<dbuf_type>::GetDbufLatencyG4KH (PhysAddr load_addr,
        uint32_t mem_latency, uint64_t issue_cycle, Dbuf* dbuf)
{
    uint32_t entry = dbuf->GetDbufEntry (load_addr);
    if (dbuf->IsHitDbuf (entry, load_addr) == true) {
        // DBUFヒット：レイテンシを削減
        uint64_t dbuf_avail_cycle = dbuf->GetDbufCycle (entry, load_addr);
        uint32_t dbuf_hit_latency = Dbuf::DBUF_HIT_LATENCY_G4KH;

        if (issue_cycle >= dbuf_avail_cycle) {
            // DBUFのフィル完了しているので、DBUFヒット時レイテンシを使用
            mem_latency = dbuf_hit_latency;
            // When previous is buf hit, wait hit access.
            // Don't wait previous miss access. (Miss access can be overwrapped.
            if (issue_cycle + 2 < m_fill_lock_cycle) {
                mem_latency += static_cast<uint32_t> ((m_fill_lock_cycle - 2) - issue_cycle);
            }
        } else {
            // DBUFのフィル未完了。フィル完了 or DBUFヒット時レイテンシの大きい方を使用
            uint32_t dbuf_stall = static_cast <uint32_t> (dbuf_avail_cycle - issue_cycle);
            mem_latency = std::max (dbuf_stall, dbuf_hit_latency);

            // Lock next dbuf access when access to DBUF filling entry
            m_fill_lock_cycle = dbuf_avail_cycle;
        }
        m_is_buf_hit = true;
        CfMsg::CachePrint ("[DBUF_HIT:P%u:e%1X]\n", dbuf->m_peid, entry);
        dbuf->IncrementHit ();

        m_dbuf_fill_cycle = mem_latency;
    } else {
        // DBUF無効ならバッファなしメモリのレイテンシを使う

        // When previous is buf hit, wait hit access.
        // Don't wait previous miss access. (Miss access can be overwrapped.
        if (issue_cycle < m_fill_lock_cycle) {
            mem_latency += static_cast<uint32_t> (m_fill_lock_cycle - issue_cycle);
        }
        bool is_post_write = false;
        if (cmd == DATA_WRITE){
            is_post_write = true;
        }
        mem_latency = GetNoBufLatencyG4KH (mem_latency, issue_cycle, &is_post_write);

        // DBUFあればDBUF更新
        entry = dbuf->GetReplaceEntry (load_addr);

        dbuf->UpdateDbuf (entry, load_addr, issue_cycle + mem_latency + 1,
                                      m_repeat_interval_cycle);
        m_dbuf_fill_cycle = mem_latency;
        dbuf->UpdateReplaceEntry ();
        m_is_buf_hit = false;
        CfMsg::CachePrint ("[DBUF_MISS:P%u:e%1X]\n", dbuf->m_peid, entry);
        dbuf->IncrementMiss ();
    }

    return mem_latency;
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfo<dbuf_type>::GetNoBufLatency (uint32_t mem_latency, uint64_t issue_cycle)
{
    // リピートサイクルを反映
    // 処理の簡略化：リピート間隔を既存アクセスの最終終了サイクルを基準に計算。
    //               アクセス順追い越し完了するケースでは、
    //               実際よりレイテンシ大きくなってしまうが、
    //               全要素チェックしたら遅くなるのでやらない。
    int64_t new_mem_latency = m_mem_avail_cycle.back () + m_next_interval_cycle - issue_cycle;
    if (new_mem_latency > static_cast<int32_t> (mem_latency)) {
        mem_latency = static_cast<uint32_t> (new_mem_latency);
    }

    return mem_latency;
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfo<dbuf_type>::GetNoBufLatencyG4KH (uint32_t mem_latency, uint64_t issue_cycle, bool* is_post_write)
{
    // リピートサイクルを反映
    // 処理の簡略化：リピート間隔を既存アクセスの最終終了サイクルを基準に計算。
    //               アクセス順追い越し完了するケースでは、
    //               実際よりレイテンシ大きくなってしまうが、
    //               全要素チェックしたら遅くなるのでやらない。
    // Reflect repeat cycle
    // Process simplification: calculate the repeat interval based on the final end cycle of existing access
    //             In case where access order overtaking is completed, the latency will be larger than actual but
    //             It is not done because it takes time if all elements are checked.
    int64_t new_mem_latency = 0;

    if (*is_post_write == true) {
        new_mem_latency = m_mem_avail_cycle.back () + m_next_interval_cycle - issue_cycle;
        //m_mem_avail_cycle.back () - issue_cycle: this time, mem is still busy
        //2: latency of ld from ID to MEM.
        //if (m_mem_avail_cycle.back () - issue_cycle) <= 2, after 2 cycles, mem is free and
        //it's available to process this one.
        //printf ("WRITE new_mem_latency: %d = m_mem_avail_cycle.back () %d + m_next_interval_cycle %d - issue_cycle %d\n", new_mem_latency,m_mem_avail_cycle.back (), m_next_interval_cycle,issue_cycle);
        if ((new_mem_latency < mem_latency) && ((int32_t(m_mem_avail_cycle.back () - issue_cycle)) > ID_TO_MEM_INTERFACE)
                && (new_mem_latency > 0) ){
            mem_latency +=  m_next_interval_cycle ;
        }
    } else {
        //m_mem_avail_cycle.back () already had +1 due to no forwarding from previous ld.
        new_mem_latency = m_mem_avail_cycle.back () + m_next_interval_cycle - issue_cycle - 1;

        //m_mem_avail_cycle.back () - issue_cycle: this time, mem is still busy
        //2: latency of ld from ID to MEM.
        //if (m_mem_avail_cycle.back () - issue_cycle) <= 2, after 2 cycles, mem is free and
        //it's available to process this one.
        //printf ("new_mem_latency: %d = m_mem_avail_cycle.back () %d + m_next_interval_cycle %d - issue_cycle %d - 1\n", new_mem_latency,m_mem_avail_cycle.back (), m_next_interval_cycle,issue_cycle);
        if ((new_mem_latency < mem_latency) && ((int32_t(m_mem_avail_cycle.back () - issue_cycle)) > ID_TO_MEM_INTERFACE)
                && (new_mem_latency > 0) ){
            mem_latency +=  m_next_interval_cycle - 1;
            //printf ("mem_latency: %d\n", mem_latency);
        }
    }

    if (new_mem_latency > static_cast<int32_t> (mem_latency)) {
        mem_latency = static_cast<uint32_t> (new_mem_latency);
    }

    return mem_latency;
}


template <MemDbufType dbuf_type>
uint64_t MemLatencyInfo<dbuf_type>::GetTcmAdditinalLatency (uint64_t issue_cycle)
{
    int64_t mem_latency = m_mem_avail_cycle.back () + m_next_interval_cycle - issue_cycle;
    if (mem_latency > 0) {
        //printf("kita1:mem_latency=%ld, m_mem_avail_cycle.back ()=%lu, m_next_interval_cycle=%lu, issue_cycle=%lu\n",
        //mem_latency, m_mem_avail_cycle.back (), m_next_interval_cycle, issue_cycle);
        // Check OoO transfer.
        // Because CForest supports repeat rate parameter for memory latency,
        // although LRAM access is 1 cycle, CForest uses +m_next_interval_cycle instead +1.
        uint64_t avail_cycle = issue_cycle; // + m_next_interval_cycle;
        uint64_t interval_cycle = m_next_interval_cycle;
        std::vector<uint64_t>::iterator it = m_mem_avail_cycle.begin ();
        std::vector<uint64_t>::iterator it_next = it + 1;
        while ((it != m_mem_avail_cycle.end ()) && (it_next != m_mem_avail_cycle.end ())) {
            if (*it_next >= avail_cycle + interval_cycle) {
                if (*it_next - *it >= (m_next_interval_cycle << 1)) {
                    // Can OoO transfer
                    avail_cycle = std::max (*it + m_next_interval_cycle, avail_cycle);
                    m_mem_avail_cycle.front () = avail_cycle;
                    if (m_mem_avail_cycle.begin () + 1 != it_next) {
                        rotate (m_mem_avail_cycle.begin (), m_mem_avail_cycle.begin () + 1, it_next);
                    }
                    return avail_cycle;
                }
            }
            ++ it;
            ++ it_next;
        }
        m_mem_avail_cycle.front () = m_mem_avail_cycle.back () + m_next_interval_cycle;
        std::sort (m_mem_avail_cycle.begin(), m_mem_avail_cycle.end());
        return m_mem_avail_cycle.back () + m_next_interval_cycle;
    } else {
        m_mem_avail_cycle.front () = issue_cycle;
        std::sort (m_mem_avail_cycle.begin(), m_mem_avail_cycle.end());
        return issue_cycle;
    }
}


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::DumpOutstandQueue (void) const
{
    int i = 0;
    CfMsg::DPrint (MSG_ERR, "Dump outstand queue (depth=%d)\n",
                   static_cast<uint32_t> (m_mem_avail_cycle.size()));
    std::vector<uint64_t>::const_iterator it = m_mem_avail_cycle.begin ();
    while (it != m_mem_avail_cycle.end ()) {
        CfMsg::DPrint (MSG_ERR, "[%d]=%08" PRIu64 "\n", i, *it);
        ++ it;
        ++ i;
    }
}


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::InitMemCycle (void)
{
    m_is_lram_access = false;
    SetupMemModuleLatency ();

    m_mem_avail_cycle.clear ();
    m_mem_avail_cycle.assign (m_outstand_num, 0);

    m_is_buf_hit = false;

    m_last_tcm_addr = 0;
    m_last_tcm_size = 0;
    m_last_latency = 0;
    m_last_time = 0;
    m_last_start_time = 0;
    m_is_last_2way = false;
    m_dbuf_fill_cycle = 0;
    m_fill_lock_cycle = 0;
    m_last_st_addr = 0x3;
    m_last_st_cycle = 0;
    m_next_interval_cycle = m_repeat_interval_cycle;
    m_last_acc_type = INST_READ;
    m_gramc_req_avail_cycle = 0;

    m_last_cl_peid = 0xFFFFFFFF;

    // initial arbitor is PE0 inst (I don't know actual)
    m_last_acc_peid = 0;
    m_last_acc_cmd = INST_READ;
    m_last_rmw_addr = 0xFFFFFFFF;
}


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::UpdateExoPeLatency (uint32_t exo_latency_offset)
{
    if (m_pe_location != MEM_LOCATE_GLOBAL) {
        // PEローカルメモリならexo_peレイテンシを更新
        m_cl_read_latency  = m_pe_read_latency  + exo_latency_offset;
        m_cl_write_latency = m_pe_write_latency + exo_latency_offset;

        // PEローカルメモリのPE外フェッチレイテンシはcl_readレイテンシと同じにする。
        m_cl_fetch_latency = m_cl_read_latency;
    }
    SetupMemModuleLatency ();
}


template <MemDbufType dbuf_type>
MemResourceType MemLatencyInfo<dbuf_type>::GetMemResourceType (uint32_t peid) const
{
    MemResourceType type;
    if (m_resource_type == MEM_RES_LRAM) {
        if (m_pe_location == peid) {
            type = MEM_RES_LRAM;
        } else {
            // VCI (as same as peripheral) is used for access of L1RAM@OtherPE.
            type = MEM_RES_OTHERS;
        }
    } else {
        type = m_resource_type; // NOTE : CRAM@diffCL might be different. But there is no design.
    }
    return type;
}


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const
{
    res_info->pe_fetch_latency = m_pe_fetch_latency;
    res_info->pe_read_latency = m_pe_read_latency;
    res_info->pe_write_latency = m_pe_write_latency;
    res_info->cl_fetch_latency = m_cl_fetch_latency;
    res_info->cl_read_latency = m_cl_read_latency;
    res_info->cl_write_latency = m_cl_write_latency;
    res_info->exo_cl_fetch_latency = m_exo_cl_fetch_latency;
    res_info->exo_cl_read_latency = m_exo_cl_read_latency;
    res_info->exo_cl_write_latency = m_exo_cl_write_latency;
    res_info->pe_location = m_pe_location;
    res_info->cl_location = m_cl_location;
    res_info->num_outstanding = m_outstand_num;
    res_info->repeat_rate = m_repeat_interval_cycle;
}


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const
{
    CfMsg::DPrintML (MSG_INF, "%s(%08llx)", GetName (addr).c_str (), addr);
}


#ifdef ENABLE_CLUSTER
template <MemDbufType dbuf_type>
template <MemAccessCommand cmd>
uint32_t MemLatencyInfo<dbuf_type>::CalcMemModuleLatency (uint32_t peid) const
{
    uint32_t cl_location = m_pe_location.GetBindCL();
    uint32_t pe_location = m_pe_location.GetBindPE();
    GlobalPeid gpeid (peid);
    uint32_t cl_field = gpeid.GetCLID();
    uint32_t pe_field = gpeid.GetPEID();
    MemLocationType master_location(cl_field , pe_field);
    uint32_t latency = 0;

    //CfMsg::LatencyPrint ("master(clid=%2d,peid=%2d) ==> slave(clid=%2d,peid=%2d) ",
    //               cl_field, pe_field, cl_location, pe_location);

    //CfMsg::LatencyPrint ("%s .... ",
    //               (cmd == INST_READ) ? "FETCH" : (cmd == DATA_READ) ? "READ " : "WRITE");
    uint32_t exo_pe_to_pe_latency = p_forest_util->GetPeLatency(pe_location).second;
    uint32_t exo_cl_to_cl_latency = p_forest_util->GetClLatency(cl_location).second;
    uint32_t master_cl_latency_offset = p_forest_util->GetClLatency(master_peid).first;
    uint32_t master_pe_latency_offset = p_forest_util->GetPeLatency(master_peid).first;
    if (cl_field != cl_location) {
        // Cluster外アクセス
        uint32_t master_peid = master_location.Get();
        uint32_t slave_peid  = m_pe_location.Get();
        uint32_t backword_latency = p_forest_util->GetClLatency(master_peid).second +
                                    p_forest_util->GetPeLatency(master_peid).second +
                                    p_forest_util->GetClLatency(slave_peid).first +
                                    p_forest_util->GetPeLatency(slave_peid).first;
       switch (cmd) {
        case INST_READ:
            latency = m_pe_fetch_latency + exo_pe_to_pe_latency + exo_cl_to_cl_latency;
            latency += backword_latency; // 帰りの分
            break;
        case DATA_READ:
            latency = m_pe_read_latency + exo_pe_to_pe_latency + exo_cl_to_cl_latency;
            latency += backword_latency; // 帰りの分
            break;
        case DATA_WRITE:
            latency = m_pe_write_latency + exo_pe_to_pe_latency + exo_cl_to_cl_latency;
            latency += backword_latency; // 帰りの分(TODO: WriteExactに対応する。現状はReadと同様。)
            break;
        }
        //CfMsg::LatencyPrint ("[cl_to_cl] slave(=%3d) + master(=%3d) + WayBack(=%3d)",
        //               latency - backword_latency, master_pe_latency_offset + master_cl_latency_offset , backword_latency);

        return (latency + master_pe_latency_offset + master_cl_latency_offset);
    } else if (pe_field != pe_location) {
        //PE外アクセス
        uint32_t master_peid = master_location.Get();
        uint32_t slave_peid  = m_pe_location.Get();
        uint32_t backword_latency = p_forest_util->GetPeLatency(master_peid).second +
                                    p_forest_util->GetPeLatency(slave_peid).first;
        switch (cmd) {
        case INST_READ:
            latency = m_pe_fetch_latency + exo_pe_to_pe_latency;
            latency += backword_latency; // 帰りの分
            break;
        case DATA_READ:
            latency = m_pe_read_latency + exo_pe_to_pe_latency;
            latency += backword_latency; // 帰りの分
            break;
        case DATA_WRITE:
            latency = m_pe_write_latency + exo_pe_to_pe_latency;
            latency += backword_latency; // 帰りの分(TODO: WriteExactに対応する。現状はReadと同様。)
            break;
        }
        //CfMsg::LatencyPrint ("[pe_to_pe] slave(=%3d) + master(=%3d) + WayBack(=%3d)",
        //               latency - backword_latency , master_pe_latency_offset , backword_latency);

        return (latency + master_pe_latency_offset);
    } else {
        // PEローカルアクセス
        switch (cmd) {
        case INST_READ:
            latency = m_pe_fetch_latency;
            break;
        case DATA_READ:
            latency = m_pe_read_latency;
            break;
        case DATA_WRITE:
            latency = m_pe_write_latency;
            break;
        }
        //CfMsg::LatencyPrint ("[local   ] slave(=%3d)                               " , latency);
        return (latency);
    }
}


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::SetupMemModuleLatency (void)
{
    m_latency_info.clear ();

    uint32_t max_peid;
    if (p_forest_util->m_forest != NULL) {
        max_peid = p_forest_util->GetPeLastId ();
    } else {
        max_peid = 0;
    }

    uint32_t i;
    for (i = 0; i <= max_peid; i++) {
        LatencyVec latency (NO_MEMACC + 1, 0U);
        latency[INST_READ] = CalcMemModuleLatency<INST_READ> (i);
        latency[DATA_READ] = CalcMemModuleLatency<DATA_READ> (i);
        latency[DATA_WRITE] = CalcMemModuleLatency<DATA_WRITE> (i);
        // When G4MH, fetch latency is BIU to BIU latency.
        // But, latency document has the latency between IA to instdec.
        // To translate to the BIU to BIU latency, fetch latency is -4 on G4MH.
        if (latency[INST_READ] > FETCH_LAT_OFFSET_G3MH) {
            latency[INST_READ] -= FETCH_LAT_OFFSET_G3MH;
        } else {
            latency[INST_READ] = 0;
        }
        m_latency_info.push_back (latency);
    }
#ifdef DEBUG_CS_MEM
    CfMsg::DPrint (MSG_ERR, "Dump start\n");
    std::vector<LatencyVec>::const_iterator it = m_latency_info.begin ();
    while (it != m_latency_info.end ()) {
        CfMsg::DPrint (MSG_ERR, "fetch=%4u, read=%4u, write=%4u\n", (*it)[0], (*it)[1], (*it)[2]);
        ++ it;
    }
#endif // #ifdef DEBUG_CS_MEM
}


#else // #ifdef ENABLE_CLUSTER


template <MemDbufType dbuf_type>
void MemLatencyInfo<dbuf_type>::SetupMemModuleLatency (void)
{
    m_latency_info.clear ();
    m_coretype_info.clear ();

    uint32_t max_peid;
#if _UNIT_TEST
    max_peid = 0;
#else //#if _UNIT_TEST
    if (p_forest_util->m_forest != NULL) {
        max_peid = p_forest_util->GetPeLastId ();
    } else {
        max_peid = 0;
    }
#endif // #if _UNIT_TEST

    uint32_t i;
    for (i = 0; i <= max_peid; i++) {
        CoreType core_type;
        LatencyVec latency (NO_MEMACC + 1, 0U);
#if _UNIT_TEST
        if (MEM_LOCATE_GLOBAL != m_pe_location) {
#else // #if _UNIT_TEST
        if (i == m_pe_location) {
#endif // #else // #if _UNIT_TEST
            // PEローカルアクセス
            //  INST_READ    = 0,
            //  DATA_READ    = 1,
            //  DATA_WRITE   = 2,
            //  DATA_RMW     = 3,
            //  NO_MEMACC    = 4
            latency[INST_READ]  = m_pe_fetch_latency;
            latency[DATA_READ]  = m_pe_read_latency;
            latency[DATA_WRITE] = m_pe_write_latency;

            if ((m_pe_read_latency <= 2)){
                m_is_lram_access = true;
            }
#if _UNIT_TEST
        } else if (MEM_LOCATE_GLOBAL != m_cl_location) {
#else // #if _UNIT_TEST
        } else if (p_forest_util->IsValidPeId (i) && p_internals->GetClIdFromPeId(i) == m_cl_location) {
#endif // #if _UNIT_TEST
            // CLローカルアクセス
            //  INST_READ    = 0,
            //  DATA_READ    = 1,
            //  DATA_WRITE   = 2,
            //  DATA_RMW     = 3,
            //  NO_MEMACC    = 4
            latency[INST_READ]  = m_cl_fetch_latency;
            latency[DATA_READ]  = m_cl_read_latency;
            latency[DATA_WRITE] = m_cl_write_latency; 
        } else {
            // Exo-CL access
            //  INST_READ    = 0,
            //  DATA_READ    = 1,
            //  DATA_WRITE   = 2,
            //  DATA_RMW     = 3,
            //  NO_MEMACC    = 4
            latency[INST_READ]  = m_exo_cl_fetch_latency;
            latency[DATA_READ]  = m_exo_cl_read_latency;
            latency[DATA_WRITE] = m_exo_cl_write_latency;
        }
        // When G4MH, fetch latency is BIU to BIU latency.
        // But, latency document has the latency between IA to instdec.
        // To translate to the BIU to BIU latency, fetch latency is -4 on G4MH.
        if (p_forest_util->m_forest != NULL) {
            core_type = p_forest_util->GetCoreType (i);
            if (core_type == CORE_G4MH) {
                if (latency[INST_READ] > FETCH_LAT_OFFSET_G3MH) {
                    latency[INST_READ] -= FETCH_LAT_OFFSET_G3MH;
                } else {
                    latency[INST_READ] = 0;
                }
            }
        }
        m_latency_info.push_back (latency);
        m_coretype_info.push_back (core_type);
    }
#ifdef DEBUG_CS_MEM
    CfMsg::DPrint (MSG_ERR, "Dump start\n");
    std::vector<LatencyVec>::const_iterator it = m_latency_info.begin ();
    while (it != m_latency_info.end ()) {
        CfMsg::DPrint (MSG_ERR, "fetch=%4u, read=%4u, write=%4u\n", (*it)[0], (*it)[1], (*it)[2]);
        ++ it;
    }
#endif // #ifdef DEBUG_CS_MEM
}
#endif // #ifdef ENABLE_CLUSTER

template <MemDbufType dbuf_type>
MemLatencyInfo<dbuf_type>::MemLatencyInfo (uint32_t pe_fetch_latency,
                                           uint32_t pe_read_latency,
                                           uint32_t pe_write_latency,
                                           uint32_t cl_fetch_latency,
                                           uint32_t cl_read_latency,
                                           uint32_t cl_write_latency,
                                           uint32_t exo_cl_fetch_latency,
                                           uint32_t exo_cl_read_latency,
                                           uint32_t exo_cl_write_latency,
                                           MemLocationType pe_location,
                                           MemLocationType cl_location,
                                           uint32_t num_outstanding,
                                           uint32_t repeat_rate,
                                           MemResourceType resource_type,
                                           std::string name)
    :   m_pe_fetch_latency (pe_fetch_latency),
        m_pe_read_latency (pe_read_latency),
        m_pe_write_latency (pe_write_latency),
        m_cl_fetch_latency (cl_fetch_latency),
        m_cl_read_latency (cl_read_latency),
        m_cl_write_latency (cl_write_latency),
        m_exo_cl_fetch_latency (exo_cl_fetch_latency),
        m_exo_cl_read_latency (exo_cl_read_latency),
        m_exo_cl_write_latency (exo_cl_write_latency),
        m_pe_location (pe_location),
        m_cl_location (cl_location),
        m_outstand_num (num_outstanding),
        m_repeat_interval_cycle (repeat_rate),
        m_conflict_cycle ((repeat_rate != 0) ? repeat_rate - 1 : 0),
#if _UNIT_TEST
        m_dbuf_hit_latency (5), // ForestUtil::DBUF_HIT_LATENCY
#else // #if _UNIT_TEST
        m_dbuf_hit_latency (p_internals->GetDbufHitLatency ()),
#endif // #if _UNIT_TEST
        m_resource_type (resource_type),
        m_target_name (name)
{
    if (m_outstand_num == 0) {
        // 0はforestutil側ではじくので来ないはず。
        // front/backで困るので、念の為1に補正しておく。
        m_outstand_num = 1;
    }

    InitMemCycle ();
}


template <MemDbufType dbuf_type>
MemLatencyInfo<dbuf_type>::~MemLatencyInfo ()
{
}


MemLatencyInfoGram::MemLatencyInfoGram (uint32_t pe_fetch_latency,
                    uint32_t pe_read_latency,
                    uint32_t pe_write_latency,
                    uint32_t cl_fetch_latency,
                    uint32_t cl_read_latency,
                    uint32_t cl_write_latency,
                    uint32_t exo_cl_fetch_latency,
                    uint32_t exo_cl_read_latency,
                    uint32_t exo_cl_write_latency,
                    MemLocationType pe_location,
                    MemLocationType cl_location,
                    uint32_t num_outstanding,
                    uint32_t repeat_rate,
                    std::string name)
{
    uint32_t num_outstand_buf = num_outstanding;

    for (uint32_t i = 0; i < NUM_BANK; i++) {
        std::stringstream route_str;
        route_str << name << "." << "Subbank" << i;
        std::string tgt_name = route_str.str();
        m_bank.push_back (new MemLatencyInfo<DBUF_CRAM_G4MH> (pe_fetch_latency, pe_read_latency, pe_write_latency,
                                                            cl_fetch_latency, cl_read_latency, cl_write_latency,
                                                            exo_cl_fetch_latency, exo_cl_read_latency, exo_cl_write_latency,
                                                            pe_location, cl_location, num_outstand_buf, repeat_rate, MEM_RES_CRAM, tgt_name));
    }
    InitMemCycle ();
}


MemLatencyInfoGram::~MemLatencyInfoGram ()
{
    std::vector<MemLatencyInfo<DBUF_CRAM_G4MH>*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        delete *it;
        ++ it;
    }

    m_bank.clear ();

    DeleteMbi ();
}


void MemLatencyInfoGram::InitMemCycle (void)
{
    std::vector<MemLatencyInfo<DBUF_CRAM_G4MH>*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        (*it)->InitMemCycle ();
        ++ it;
    }
    ReallocMbi ();
    m_cram_order_ctrl.Init();
}


void MemLatencyInfoGram::UpdateExoPeLatency (uint32_t exo_latency_offset)
{
    std::vector<MemLatencyInfo<DBUF_CRAM_G4MH>*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        (*it)->UpdateExoPeLatency (exo_latency_offset);
        ++ it;
    }
    ReallocMbi ();
}


void MemLatencyInfoGram::DeleteMbi (void)
{
    std::vector<MbiSlot*>::iterator it_mbi = m_mbi_slot.begin ();
    while (it_mbi != m_mbi_slot.end ()) {
        delete *it_mbi;
        ++ it_mbi;
    }
    m_mbi_slot.clear ();
}


void MemLatencyInfoGram::ReallocMbi (void)
{
    DeleteMbi ();

    uint32_t max_peid;
#if _UNIT_TEST
    max_peid = 0;
#else // #if _UNIT_TEST
    if (p_forest_util->m_forest != NULL) {
        max_peid = p_forest_util->GetPeLastId ();
    } else {
        max_peid = 0;
    }
#endif // #if _UNIT_TEST

    for (uint32_t i = 0; i <= max_peid; ++i) {
        m_mbi_slot.push_back (new MbiSlot);
    }
}


uint32_t MemLatencyInfoGram::GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                             ByteSize size)
{
    //uint64_t org = start_cycle;
    uint64_t add_latency_sc = m_cram_order_ctrl.GetAndUpdateReqDelayCycle (start_cycle);
    start_cycle += add_latency_sc;
    uint64_t add_latency = m_mbi_slot[peid]->GetAndUpdateReqDelayCycle (load_addr, start_cycle);
    start_cycle += add_latency;
    //CfMsg::DPrint (MSG_ERR, "org=%lu, start_cycle=%lu, add_latency_sc=%lu, add_latency=%lu\n",
    //               org, start_cycle, add_latency_sc, add_latency);
    uint32_t latency = (m_bank[GetBankId(load_addr)])->GetReadLatency (peid, load_addr, start_cycle, size)
                     + static_cast<uint32_t>(add_latency) + static_cast<uint32_t>(add_latency_sc);
    //CfMsg::DPrint (MSG_ERR, "R:Latency=%u, finish_cycle=%lu\n", latency, start_cycle + latency);
    return latency;
}


uint32_t MemLatencyInfoGram::GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                              ByteSize size)
{
    //uint64_t org = start_cycle;
    uint64_t add_latency_sc = m_cram_order_ctrl.GetAndUpdateReqDelayCycle (start_cycle);
    start_cycle += add_latency_sc;
    uint64_t add_latency = m_mbi_slot[peid]->GetAndUpdateReqDelayCycle (load_addr, start_cycle);
    start_cycle += add_latency;
    //CfMsg::DPrint (MSG_ERR, "org=%lu, start_cycle=%lu, add_latency_sc=%lu, add_latency=%lu\n",
    //               org, start_cycle, add_latency_sc, add_latency);
    uint32_t latency = (m_bank[GetBankId(load_addr)])->GetWriteLatency (peid, load_addr, start_cycle, size)
                     + static_cast<uint32_t>(add_latency) + static_cast<uint32_t>(add_latency_sc);
    //CfMsg::DPrint (MSG_ERR, "W:Latency=%u, finish_cycle=%lu\n", latency, start_cycle + latency);
    return latency;
}


uint32_t MemLatencyInfoGram::GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                              ByteSize size)
{
    return (m_bank[GetBankId(load_addr)])->GetFetchLatency (peid, load_addr, start_cycle, size);
}


void MemLatencyInfoGram::GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const
{
    (m_bank[GetBankId(addr)])->GetResourceInfo (res_info, addr);
}


uint32_t MemLatencyInfoGram::GetConflictLatency (void) const
{
    return (m_bank[0])->GetConflictLatency ();
}


MemResourceType MemLatencyInfoGram::GetMemResourceType (uint32_t peid) const
{
    return (m_bank [0])->GetMemResourceType (peid);
}


std::string MemLatencyInfoGram::GetName (PhysAddr addr) const
{
    return (m_bank[GetBankId(addr)])->GetName (addr);
}


void MemLatencyInfoGram::ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const
{
    (m_bank[GetBankId(addr)])->ShowAccessRoute (peid, cmd, addr);
}


MemLatencyInfoLram::MemLatencyInfoLram (uint32_t pe_fetch_latency,
                    uint32_t pe_read_latency,
                    uint32_t pe_write_latency,
                    uint32_t cl_fetch_latency,
                    uint32_t cl_read_latency,
                    uint32_t cl_write_latency,
                    uint32_t exo_cl_fetch_latency,
                    uint32_t exo_cl_read_latency,
                    uint32_t exo_cl_write_latency,
                    MemLocationType pe_location,
                    MemLocationType cl_location,
                    uint32_t num_outstanding,
                    uint32_t repeat_rate,
                    std::string name)
{
    uint32_t num_outstand_buf = num_outstanding;

    for (uint32_t i = 0; i < NUM_BANK; i++) {
        std::stringstream route_str;
        route_str << name << "." << "Subbank" << i;
        std::string tgt_name = route_str.str();
        m_bank.push_back (new MemLatencyInfo<DBUF_LRAM_G4MH> (pe_fetch_latency, pe_read_latency, pe_write_latency,
                                                            cl_fetch_latency, cl_read_latency, cl_write_latency,
                                                            exo_cl_fetch_latency, exo_cl_read_latency, exo_cl_write_latency,
                                                            pe_location, cl_location, num_outstand_buf, repeat_rate, MEM_RES_LRAM, tgt_name));
    }
    InitMemCycle ();
}


MemLatencyInfoLram::~MemLatencyInfoLram ()
{
    std::vector<MemLatencyInfo<DBUF_LRAM_G4MH>*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        delete *it;
        ++ it;
    }
    m_bank.clear ();
}


void MemLatencyInfoLram::InitMemCycle (void)
{
    std::vector<MemLatencyInfo<DBUF_LRAM_G4MH>*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        (*it)->InitMemCycle ();
        ++ it;
    }
    m_lram_order_ctrl.Init();
}


void MemLatencyInfoLram::UpdateExoPeLatency (uint32_t exo_latency_offset)
{
    std::vector<MemLatencyInfo<DBUF_LRAM_G4MH>*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        (*it)->UpdateExoPeLatency (exo_latency_offset);
        ++ it;
    }
}


uint32_t MemLatencyInfoLram::GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                             ByteSize size)
{
    uint32_t bankid = GetBankId(load_addr);
//printf ("LRAM bank%u is read: start_cycle = %lu, ", bankid, start_cycle);
    m_lram_order_ctrl.UpdateLatestReqCycle (start_cycle, bankid);
//uint32_t latency = (m_bank[bankid])->GetReadLatency (peid, load_addr, start_cycle, size);
//printf ("latency=%u\n", latency);
//return latency;
    return (m_bank[bankid])->GetReadLatency (peid, load_addr, start_cycle, size);
}


uint32_t MemLatencyInfoLram::GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                              ByteSize size)
{
    uint32_t bankid = GetBankId(load_addr);
    // Because G4MH is STO on LRAM, write access waits both preceiding read/write.
    // Because of bug in RTL, read access doesn't wait preceiding read/write except the write with same address.
    uint64_t add_latency = m_lram_order_ctrl.GetAndUpdateReqDelayCycle (start_cycle, bankid);
//printf ("LRAM bank%u is written: start_cycle = %lu, add_latency for STO = %lu, ", bankid, start_cycle, add_latency);
    start_cycle += add_latency;
//uint32_t latency = (m_bank[bankid])->GetWriteLatency (peid, load_addr, start_cycle, size) + static_cast<uint32_t>(add_latency);
//printf ("total latency=%u\n", latency);
//return latency;
    return (m_bank[bankid])->GetWriteLatency (peid, load_addr, start_cycle, size) + static_cast<uint32_t>(add_latency);
}


uint32_t MemLatencyInfoLram::GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                              ByteSize size)
{
    return (m_bank[GetBankId(load_addr)])->GetFetchLatency (peid, load_addr, start_cycle, size);
}


void MemLatencyInfoLram::GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const
{
    (m_bank[GetBankId(addr)])->GetResourceInfo (res_info, addr);
}


uint32_t MemLatencyInfoLram::GetConflictLatency (void) const
{
    return (m_bank[0])->GetConflictLatency ();
}


MemResourceType MemLatencyInfoLram::GetMemResourceType (uint32_t peid) const
{
    return (m_bank [0])->GetMemResourceType (peid);
}


std::string MemLatencyInfoLram::GetName (PhysAddr addr) const
{
    return (m_bank[GetBankId(addr)])->GetName (addr);
}


void MemLatencyInfoLram::ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const
{
    (m_bank[GetBankId(addr)])->ShowAccessRoute (peid, cmd, addr);
}


template <MemDbufType dbuf_type>
MemLatencyInfoBank<dbuf_type>::MemLatencyInfoBank (uint32_t pe_fetch_latency,
                    uint32_t pe_read_latency,
                    uint32_t pe_write_latency,
                    uint32_t cl_fetch_latency,
                    uint32_t cl_read_latency,
                    uint32_t cl_write_latency,
                    uint32_t exo_cl_fetch_latency,
                    uint32_t exo_cl_read_latency,
                    uint32_t exo_cl_write_latency,
                    MemLocationType pe_location,
                    MemLocationType cl_location,
                    uint32_t num_outstanding,
                    uint32_t repeat_rate,
                    std::vector<ByteSize>* bank_size,
                    PhysAddr start_addr,
                    PhysAddr end_addr,
                    MemResourceType resource_type,
                    std::string name)
{
    uint32_t num_outstand_buf = num_outstanding;
    CalcBankParam (bank_size, start_addr, end_addr);

    PhysAddr bank_start_addr = start_addr;
    PhysAddr bank_end_addr;
    uint32_t i;
    for (i = 0; i < m_bank_num; i++) {
        std::stringstream route_str;
        route_str << name << "." << "Bank" << i;
        std::string tgt_name = route_str.str();
        MemLatencyInfoBase* bank;
        if (dbuf_type == DBUF_LRAM_G4MH) {
            bank = new MemLatencyInfoLram (pe_fetch_latency,
                                          pe_read_latency, pe_write_latency,
                                          cl_fetch_latency,
                                          cl_read_latency, cl_write_latency,
                                          exo_cl_fetch_latency,
                                          exo_cl_read_latency, exo_cl_write_latency,
                                          pe_location, cl_location, num_outstand_buf, repeat_rate, tgt_name);
        } else if (dbuf_type == DBUF_CRAM_G4MH) {
            bank = new MemLatencyInfoGram (pe_fetch_latency,
                                          pe_read_latency, pe_write_latency,
                                          cl_fetch_latency,
                                          cl_read_latency, cl_write_latency,
                                          exo_cl_fetch_latency,
                                          exo_cl_read_latency, exo_cl_write_latency,
                                          pe_location, cl_location, num_outstand_buf, repeat_rate, tgt_name);
        } else {
            bank = new MemLatencyInfo<dbuf_type> (pe_fetch_latency,
                                                  pe_read_latency, pe_write_latency,
                                                  cl_fetch_latency,
                                                  cl_read_latency, cl_write_latency,
                                                  exo_cl_fetch_latency,
                                                  exo_cl_read_latency, exo_cl_write_latency,
                                                  pe_location, cl_location, num_outstand_buf, repeat_rate, resource_type, tgt_name);
        }
        bank_end_addr = bank_start_addr + (*bank_size)[i] - 1;
        while (bank_start_addr <= bank_end_addr) {
            m_bank.push_back (bank);
            bank_start_addr += m_block_size;
        }
    }

    uint32_t start_block_num = static_cast <uint32_t> (m_bank.size ());
    if (start_block_num < m_block_num) {
        // Dummy
        MemLatencyInfoBase* dummy = new MemLatencyInfo<dbuf_type> (pe_fetch_latency,
                                            pe_read_latency, pe_write_latency,
                                            cl_fetch_latency,
                                            cl_read_latency, cl_write_latency,
                                            exo_cl_fetch_latency,
                                            exo_cl_read_latency, exo_cl_write_latency,
                                            pe_location, cl_location, num_outstand_buf, repeat_rate,
                                            resource_type, name);
        for (uint32_t j = start_block_num; j < m_block_num; ++j) {
            m_bank.push_back (dummy);
        }
    }
    InitMemCycle ();
}


template <MemDbufType dbuf_type>
MemLatencyInfoBank<dbuf_type>::~MemLatencyInfoBank ()
{
    typename std::vector<MemLatencyInfoBase*>::iterator it = m_bank.begin ();
    MemLatencyInfoBase* dbuf = NULL;
    while (it != m_bank.end ()) {
        if (dbuf != *it) {
            dbuf = *it;
            delete dbuf;
        }
        ++ it;
    }
    m_bank.clear ();
}


template <MemDbufType dbuf_type>
void MemLatencyInfoBank<dbuf_type>::InitMemCycle (void)
{
    typename std::vector<MemLatencyInfoBase*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        (*it)->InitMemCycle ();
        ++ it;
    }
}


template <MemDbufType dbuf_type>
void MemLatencyInfoBank<dbuf_type>::UpdateExoPeLatency (uint32_t exo_latency_offset)
{
    typename std::vector<MemLatencyInfoBase*>::iterator it = m_bank.begin ();
    while (it != m_bank.end ()) {
        (*it)->UpdateExoPeLatency (exo_latency_offset);
        ++ it;
    }
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                             ByteSize size)
{
    return (m_bank[GetBankId(load_addr)])->GetReadLatency (peid, load_addr, start_cycle, size);
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                              ByteSize size)
{
    return (m_bank[GetBankId(load_addr)])->GetWriteLatency (peid, load_addr, start_cycle, size);
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
                                              ByteSize size)
{
    return (m_bank[GetBankId(load_addr)])->GetFetchLatency (peid, load_addr, start_cycle, size);
}


template <MemDbufType dbuf_type>
void MemLatencyInfoBank<dbuf_type>::GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const
{
    (m_bank[GetBankId(addr)])->GetResourceInfo (res_info, addr);
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetConflictLatency (void) const
{
    return (m_bank[0])->GetConflictLatency ();
}


template <MemDbufType dbuf_type>
bool MemLatencyInfoBank<dbuf_type>::IsBufHitLast (PhysAddr addr) const
{
    return (m_bank [GetBankId (addr)])->IsBufHitLast (addr);
}


template <MemDbufType dbuf_type>
bool MemLatencyInfoBank<dbuf_type>::IsBufHit (uint32_t peid, PhysAddr addr) const
{
    return (m_bank [GetBankId (addr)])->IsBufHit (peid, addr);
}


template <MemDbufType dbuf_type>
MemResourceType MemLatencyInfoBank<dbuf_type>::GetMemResourceType (uint32_t peid) const
{
    return (m_bank [0])->GetMemResourceType (peid);
}


template <MemDbufType dbuf_type>
std::string MemLatencyInfoBank<dbuf_type>::GetName (PhysAddr addr) const
{
    return (m_bank [GetBankId (addr)])->GetName (addr);
}


template <MemDbufType dbuf_type>
void MemLatencyInfoBank<dbuf_type>::ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const
{
    (m_bank[GetBankId(addr)])->ShowAccessRoute (peid, cmd, addr);
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetBankId (PhysAddr addr) const
{
    return ((static_cast <uint32_t>(addr) - m_bank_offset) >> m_shift_val) & m_bank_id_mask;
}


template <MemDbufType dbuf_type>
void MemLatencyInfoBank<dbuf_type>::CalcBankParam (std::vector<ByteSize>* bank_size,
                                        PhysAddr start_addr, PhysAddr end_addr)
{
    uint32_t gcd_size = GetGCDN (bank_size);

    // block size must be 2^n
    if ( ! Is2PowN (gcd_size)) {
        uint32_t val = GetLog2u32 (gcd_size);
        if (val != 0) {
            -- val;
        }
        uint32_t i;
        for (i = val; i != 0; --i) {
            uint32_t comp_val = 1 << i;
            if ((gcd_size % comp_val) == 0) {
                // find pow 2 size.
                gcd_size = comp_val;
                break;
            }
        }
        if ((i == 0) && (gcd_size != 0)) {
            gcd_size = 1;
        }
    }

    if (  (gcd_size == 0)
       || (bank_size->size () <= 1)
       || (Is2PowN (gcd_size) == false) ) {
        // No bank
        m_bank_num = 1;
        m_shift_val = 0;
        m_block_num = 1;
        m_block_size = static_cast<uint32_t>((end_addr - start_addr) + 1ULL);
        m_bank_id_mask = 0;
        m_bank_offset = static_cast<uint32_t> (start_addr);
    } else {
        m_bank_num = static_cast <uint32_t> (bank_size->size ());
        uint32_t total_size = static_cast<uint32_t>((end_addr - start_addr) + 1ULL);
        m_block_size = gcd_size;
        m_block_num = total_size / m_block_size;
        if ( ! Is2PowN (m_block_num)) {
            m_block_num = 1 << GetLog2u32 (m_block_num);
        }
        m_shift_val = GetLog2u32 (m_block_size);
        m_bank_id_mask = m_block_num - 1;
        m_bank_offset = static_cast<uint32_t> (start_addr);
    }
}


template <MemDbufType dbuf_type>
bool MemLatencyInfoBank<dbuf_type>::Is2PowN (uint32_t val) const
{
    return (val & (val - 1)) == 0;
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetLog2u32 (uint32_t val) const
{
    for (uint32_t i = 0; i < 32; i++) {
        uint32_t comp_val = 1 << i;
        if (val == comp_val) {
            return i;
        }
        if (val <= comp_val) {
            // Round up
            return i;
        }
    }
    return 0;
}


// Eucledean algorithm to find the greatest common divisio of 2 numbers.
template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetGCD2 (uint32_t small, uint32_t big) const
{
    if (big < small) {
        std::swap (big, small);
    }

    if (small == 0) return 0;

    uint32_t amari;
    while((amari = big % small) != 0 ) {
        big = small;
        small = amari;
    }
    return small;
}


template <MemDbufType dbuf_type>
uint32_t MemLatencyInfoBank<dbuf_type>::GetGCDN (std::vector<ByteSize>* bank_size) const
{
    std::vector<ByteSize>::const_iterator it = bank_size->begin ();
    uint32_t gcd_val;

    if (bank_size->size() == 0) {
        gcd_val = 0;
    } else {
        gcd_val = *it;
        ++ it;
        while (it != bank_size->end ()) {
            gcd_val = GetGCD2 (gcd_val, *it);
            ++ it;
        }
    }
    return gcd_val;
}


void MemLatencyInfoSCHEAP::ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const
{
    CfMsg::DPrintML (MSG_INF, "%s(%08llx)", GetName (addr).c_str (), addr);
}


template MemLatencyInfo<DBUF_FROM_G4MH>::MemLatencyInfo (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      MemResourceType resource_type,
                                                      std::string name);
template MemLatencyInfo<DBUF_CRAM_G4MH>::MemLatencyInfo (uint32_t pe_fetch_latency,
                                                       uint32_t pe_read_latency,
                                                       uint32_t pe_write_latency,
                                                       uint32_t cl_fetch_latency,
                                                       uint32_t cl_read_latency,
                                                       uint32_t cl_write_latency,
                                                       uint32_t exo_cl_fetch_latency,
                                                       uint32_t exo_cl_read_latency,
                                                       uint32_t exo_cl_write_latency,
                                                       MemLocationType pe_location,
                                                       MemLocationType cl_location,
                                                       uint32_t num_outstanding,
                                                       uint32_t repeat_rate,
                                                       MemResourceType resource_type,
                                                       std::string name);
template MemLatencyInfo<DBUF_SIMPLE_MEM>::MemLatencyInfo (uint32_t pe_fetch_latency,
                                                       uint32_t pe_read_latency,
                                                       uint32_t pe_write_latency,
                                                       uint32_t cl_fetch_latency,
                                                       uint32_t cl_read_latency,
                                                       uint32_t cl_write_latency,
                                                       uint32_t exo_cl_fetch_latency,
                                                       uint32_t exo_cl_read_latency,
                                                       uint32_t exo_cl_write_latency,
                                                       MemLocationType pe_location,
                                                       MemLocationType cl_location,
                                                       uint32_t num_outstanding,
                                                       uint32_t repeat_rate,
                                                       MemResourceType resource_type,
                                                       std::string name);
template uint32_t MemLatencyInfo<DBUF_FROM_G4MH>::GetAccessLatency<INST_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_FROM_G4MH>::GetAccessLatency<DATA_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_FROM_G4MH>::GetAccessLatency<DATA_WRITE> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_CRAM_G4MH>::GetAccessLatency<INST_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_CRAM_G4MH>::GetAccessLatency<DATA_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_CRAM_G4MH>::GetAccessLatency<DATA_WRITE> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_SIMPLE_MEM>::GetAccessLatency<INST_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_SIMPLE_MEM>::GetAccessLatency<DATA_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_SIMPLE_MEM>::GetAccessLatency<DATA_WRITE> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template MemLatencyInfoBank<DBUF_FROM_G4MH>::MemLatencyInfoBank (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      std::vector<ByteSize>* bank_size,
                                                      PhysAddr start_addr,
                                                      PhysAddr end_addr,
                                                      MemResourceType resource_type,
                                                      std::string name);
template MemLatencyInfoBank<DBUF_CRAM_G4MH>::MemLatencyInfoBank (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      std::vector<ByteSize>* bank_size,
                                                      PhysAddr start_addr,
                                                      PhysAddr end_addr,
                                                      MemResourceType resource_type,
                                                      std::string name);
template MemLatencyInfoBank<DBUF_LRAM_G4MH>::MemLatencyInfoBank (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      std::vector<ByteSize>* bank_size,
                                                      PhysAddr start_addr,
                                                      PhysAddr end_addr,
                                                      MemResourceType resource_type,
                                                      std::string name);
template MemLatencyInfo<DBUF_FROM_G4KH>::MemLatencyInfo (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      MemResourceType resource_type,
                                                      std::string name);
template MemLatencyInfo<DBUF_CRAM_G4KH>::MemLatencyInfo (uint32_t pe_fetch_latency,
                                                       uint32_t pe_read_latency,
                                                       uint32_t pe_write_latency,
                                                       uint32_t cl_fetch_latency,
                                                       uint32_t cl_read_latency,
                                                       uint32_t cl_write_latency,
                                                       uint32_t exo_cl_fetch_latency,
                                                       uint32_t exo_cl_read_latency,
                                                       uint32_t exo_cl_write_latency,
                                                       MemLocationType pe_location,
                                                       MemLocationType cl_location,
                                                       uint32_t num_outstanding,
                                                       uint32_t repeat_rate,
                                                       MemResourceType resource_type,
                                                       std::string name);
template MemLatencyInfo<DBUF_LRAM_G4KH>::MemLatencyInfo (uint32_t pe_fetch_latency,
                                                       uint32_t pe_read_latency,
                                                       uint32_t pe_write_latency,
                                                       uint32_t cl_fetch_latency,
                                                       uint32_t cl_read_latency,
                                                       uint32_t cl_write_latency,
                                                       uint32_t exo_cl_fetch_latency,
                                                       uint32_t exo_cl_read_latency,
                                                       uint32_t exo_cl_write_latency,
                                                       MemLocationType pe_location,
                                                       MemLocationType cl_location,
                                                       uint32_t num_outstanding,
                                                       uint32_t repeat_rate,
                                                       MemResourceType resource_type,
                                                       std::string name);
template uint32_t MemLatencyInfo<DBUF_FROM_G4KH>::GetAccessLatency<INST_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_FROM_G4KH>::GetAccessLatency<DATA_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_FROM_G4KH>::GetAccessLatency<DATA_WRITE> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_CRAM_G4KH>::GetAccessLatency<INST_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_CRAM_G4KH>::GetAccessLatency<DATA_READ> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template uint32_t MemLatencyInfo<DBUF_CRAM_G4KH>::GetAccessLatency<DATA_WRITE> (uint32_t peid,
        PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
template MemLatencyInfoBank<DBUF_FROM_G4KH>::MemLatencyInfoBank (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      std::vector<ByteSize>* bank_size,
                                                      PhysAddr start_addr,
                                                      PhysAddr end_addr,
                                                      MemResourceType resource_type,
                                                      std::string name);
template MemLatencyInfoBank<DBUF_CRAM_G4KH>::MemLatencyInfoBank (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      std::vector<ByteSize>* bank_size,
                                                      PhysAddr start_addr,
                                                      PhysAddr end_addr,
                                                      MemResourceType resource_type,
                                                      std::string name);
template MemLatencyInfoBank<DBUF_LRAM_G4KH>::MemLatencyInfoBank (uint32_t pe_fetch_latency,
                                                      uint32_t pe_read_latency,
                                                      uint32_t pe_write_latency,
                                                      uint32_t cl_fetch_latency,
                                                      uint32_t cl_read_latency,
                                                      uint32_t cl_write_latency,
                                                      uint32_t exo_cl_fetch_latency,
                                                      uint32_t exo_cl_read_latency,
                                                      uint32_t exo_cl_write_latency,
                                                      MemLocationType pe_location,
                                                      MemLocationType cl_location,
                                                      uint32_t num_outstanding,
                                                      uint32_t repeat_rate,
                                                      std::vector<ByteSize>* bank_size,
                                                      PhysAddr start_addr,
                                                      PhysAddr end_addr,
                                                      MemResourceType resource_type,
                                                      std::string name);
