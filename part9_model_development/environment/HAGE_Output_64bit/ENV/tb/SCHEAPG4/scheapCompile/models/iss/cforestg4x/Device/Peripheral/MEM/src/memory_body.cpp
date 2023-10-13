/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <sstream>
#include <string.h>
#include <string>
#include "./config.h"
#include "./forest_common.h"
#include "./memory_body.h"
#include "./outstand_base.h"
#include "./forest_utility.h"
#include "./forest_message.h"


template <MemBody::MemDebugType dbg_mode>
MemBody::MemBlock* MemBody::GetMemBlock (SideBand sideband, PhysAddr base_addr)
{
    MemBlockMap::const_iterator itr = m_memmap.find (base_addr);
    if (itr != m_memmap.end()) {
        MemBlock* block = itr->second;
        if (unlikely (block->m_mem_body.empty ())) {
            block->Alloc (m_init_value);
        }
        return block;
    } else {
#ifdef SUPPORT_CUBESUITE
        if (dbg_mode != ACC_DBG) {
            p_internals->CauseError (sideband, E_MEMORY_NOT_MAPPED);
        } else {
            // デバッグAPI実行中にエラーが発生したことを通知。APIの戻り値の生成用。
            p_forest_util->SetCubeErrorCode (E_MEMORY_NOT_MAPPED);
        }
        return NULL;

#else // SUPPORT_CUBESUITE
        MemBlock* block = new MemBlock ();
        block->Alloc (m_init_value);
        m_memmap[base_addr] = block;
        CfMsg::DPrint (MSG_WAR, "<New Memory Region: 0x%08" PRIx32 " - 0x%08" PRIx32 ">\n",
                       base_addr, base_addr + MEM_BLOCK_SIZE - 1);
        return block;
#endif // SUPPORT_CUBESUITE
    }
}


template <MemAccessCommand cmd>
bool MemBody::CheckPermitOperation (SideBand sideband, MemBlock* block) const
{
    if (cmd == DATA_WRITE) {
        if ( (m_restrict_type & MRT_WRITE_ENABLE) == 0x0 ) {
            p_internals->CauseError (sideband, E_MEMORY_WRITE_PROTECT);
            return false;
        }
    } else if (cmd == DATA_READ) {
        if ( (m_restrict_type & MRT_READ_ENABLE) == 0x0 ) {
#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
            p_internals->CauseError (sideband, E_MEMORY_READ_PROTECT);
#endif // SUPPORT_CUBESUITE
            return false;
        }
#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
        else if (block->m_mem_body.empty ()) {
            p_internals->CauseError (sideband, E_MEMORY_NOT_INITIALIZED);
            return false;
        }
#endif // SUPPORT_CUBESUITE
    } else {
        if ( (m_restrict_type & MRT_FETCH_ENABLE) == 0x0 ) {
            p_internals->CauseError (sideband, E_MEMORY_FETCH_PROTECT);
            return false;
        }
#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
        else if (block->m_mem_body.empty ()) {
            p_internals->CauseError (sideband, E_MEMORY_NOT_INITIALIZED);
            return false;
        }
#endif // SUPPORT_CUBESUITE
    }

    return true; // Permitted operation
}


//! \sa CheckPermitOperation
template <MemAccessCommand cmd, MemBody::MemDebugType dbg_mode>
SysErrCause MemBody::MemOperate (SideBand sideband, PhysAddr addr, uint8_t* data, uint32_t size)
{
    PhysAddr start_base =  addr         & ~MEM_BLOCK_MASK;
    PhysAddr end_base   = (addr+size-1) & ~MEM_BLOCK_MASK;

    MemBlock* block = GetMemBlock<dbg_mode> (sideband, start_base);
#ifdef ENABLE_MISR
    if (block == NULL) {
        if (cmd != DATA_WRITE) { memset (data, m_init_value, size); }
        return SYSERR_NONE;
    } else if (dbg_mode != ACC_DBG && CheckPermitOperation<cmd> (sideband, block) == false) {
        if (cmd != DATA_WRITE) { memset (data, m_init_value, size); }
        return SYSERR_LRAM;
    }
#else // ENABLE_MISR
    if (block == NULL ||
        (dbg_mode != ACC_DBG && CheckPermitOperation<cmd> (sideband, block) == false)) {
        if (cmd != DATA_WRITE) { memset (data, m_init_value, size); }
        return SYSERR_LRAM;
    }
#endif // ENABLE_MISR

    if (likely (start_base == end_base)) {
        // 同一ブロック内
        if (cmd == DATA_WRITE) {
            block->Write (addr, data, size);
        } else {
            block->Read (addr, data, size);
        }
    } else {
        // ブロックをまたがるケース
        MemBlock* block_2nd = GetMemBlock<dbg_mode> (sideband, end_base);
#ifdef ENABLE_MISR
        if (block_2nd == NULL) {
            if (cmd != DATA_WRITE) { memset (data, m_init_value, size); }
            return SYSERR_NONE;
        } else if (dbg_mode != ACC_DBG && CheckPermitOperation<cmd> (sideband, block_2nd) == false) {
            if (cmd != DATA_WRITE) { memset (data, m_init_value, size); }
            return SYSERR_LRAM;
        }
#else // ENABLE_MISR
        if (block_2nd == NULL ||
            (dbg_mode != ACC_DBG && CheckPermitOperation<cmd> (sideband, block_2nd) == false)) {
            if (cmd != DATA_WRITE) { memset (data, m_init_value, size); }
            return SYSERR_LRAM;
        }
#endif // ENABLE_MISR
        uint32_t size_1st = static_cast<uint32_t> (end_base - addr);
        if (cmd == DATA_WRITE) {
            block->Write (addr, data, size_1st);
            block_2nd->Write (end_base, data + size_1st, size - size_1st);
        } else {
            block->Read (addr, data, size_1st);
            block_2nd->Read (end_base, data + size_1st, size - size_1st);
        }
    }
    return SYSERR_NONE;
}


bool MemBody::MemResourceAlloc (PhysAddr addr)
{
    PhysAddr  base_addr = addr & ~MEM_BLOCK_MASK;
    MemBlockMap::iterator itr = m_memmap.find (base_addr);
    if (itr == m_memmap.end()) {
        // 未確保
        MemBlock* block = new MemBlock ();
        m_memmap[base_addr] = block;
        return true;
    } else {
        return false;
    }
}


bool MemBody::MemResourceFree (PhysAddr addr)
{
    PhysAddr base_addr = addr & ~MEM_BLOCK_MASK;
    MemBlockMap::iterator itr = m_memmap.find (base_addr);
    if (itr != m_memmap.end()) {
        // 確保済み
        delete itr->second;
        m_memmap.erase (itr);
        return true;
    } else {
        return false;
    }
}


std::string MemBody::MemResourceDisplay (MemResourceInfo res_info) const
{
    // @@ メモリのRead/Write/Fetch属性も表示したい
    if (m_memmap.empty ()) return "";
    std::ostringstream mem_info;

    // RestrictTypeの文字列を生成
    std::string restrict_str;
    if ((m_restrict_type & MRT_READ_ENABLE) != 0) {
        restrict_str += "R";
    }
    if ((m_restrict_type & MRT_WRITE_ENABLE) != 0) {
        restrict_str += "W";
    }
    if ((m_restrict_type & MRT_FETCH_ENABLE) != 0) {
        restrict_str += "X";
    }
    if ((m_restrict_type & MRT_SELF_ONLY) != 0) {
        restrict_str += "D";
    }
    if (restrict_str.empty ()) {
        restrict_str = "N";
    }

    MemBlockMap::const_iterator itr = m_memmap.begin();
    PhysAddr start_addr = itr->first;
    PhysAddr end_addr   = itr->first + MEM_BLOCK_SIZE;
    ++ itr;
    while (itr != m_memmap.end ()) {
        if (itr->first == end_addr) {
            // 連続
            end_addr = itr->first + MEM_BLOCK_SIZE;
        } else {
            // 非連続
            if (res_info.cl_location == MEM_LOCATE_GLOBAL) {
                CfMsg::DPrint (MSG_ERR,
                        "  0x%08" PRIx32 " - 0x%08" PRIx32
                        " %-4s %-3u     %-3u    %-3u     %-3u        "
                        "%-3u       %-3u          %-3u       %-3u       %-3u        %-1s    %-1s    %-3u        %-3u\n",
                        start_addr, end_addr - 1,
                        restrict_str.c_str (),
                        res_info.pe_fetch_latency,
                        res_info.pe_read_latency,
                        res_info.pe_write_latency,
                        res_info.cl_fetch_latency,
                        res_info.cl_read_latency,
                        res_info.cl_write_latency,
                        res_info.exo_cl_fetch_latency,
                        res_info.exo_cl_read_latency,
                        res_info.exo_cl_write_latency,
                        "-",
                        "-",
                        res_info.num_outstanding,
                        res_info.repeat_rate);
            } else if (res_info.pe_location == MEM_LOCATE_GLOBAL) {
                CfMsg::DPrint (MSG_ERR,
                        "  0x%08" PRIx32 " - 0x%08" PRIx32
                        " %-4s %-3u     %-3u    %-3u     %-3u        "
                        "%-3u       %-3u          %-3u       %-3u       %-3u        %-1s    %-3u  %-3u        %-3u\n",
                        start_addr, end_addr - 1,
                        restrict_str.c_str (),
                        res_info.pe_fetch_latency,
                        res_info.pe_read_latency,
                        res_info.pe_write_latency,
                        res_info.cl_fetch_latency,
                        res_info.cl_read_latency,
                        res_info.cl_write_latency,
                        res_info.exo_cl_fetch_latency,
                        res_info.exo_cl_read_latency,
                        res_info.exo_cl_write_latency,
                        "-",
                        res_info.cl_location,
                        res_info.num_outstanding,
                        res_info.repeat_rate);
            } else {
                CfMsg::DPrint (MSG_ERR,
                        "  0x%08" PRIx32 " - 0x%08" PRIx32
                        " %-4s %-3u     %-3u    %-3u     %-3u        "
                        "%-3u       %-3u          %-3u       %-3u       %-3u        %-3u  %-3u  %-3u        %-3u\n",
                        start_addr, end_addr - 1,
                        restrict_str.c_str (),
                        res_info.pe_fetch_latency,
                        res_info.pe_read_latency,
                        res_info.pe_write_latency,
                        res_info.cl_fetch_latency,
                        res_info.cl_read_latency,
                        res_info.cl_write_latency,
                        res_info.exo_cl_fetch_latency,
                        res_info.exo_cl_read_latency,
                        res_info.exo_cl_write_latency,
                        res_info.pe_location,
                        res_info.cl_location,
                        res_info.num_outstanding,
                        res_info.repeat_rate);
            }
            mem_info <<    start_addr << " " <<
                    end_addr - 1  << " " <<
                    restrict_str.c_str () << " " <<
                    res_info.pe_fetch_latency << " " <<
                    res_info.pe_read_latency << " " <<
                    res_info.pe_write_latency << " " <<
                    res_info.cl_fetch_latency << " " <<
                    res_info.cl_read_latency << " " <<
                    res_info.cl_write_latency << " " <<
                    res_info.exo_cl_fetch_latency << " " <<
                    res_info.exo_cl_read_latency << " " <<
                    res_info.exo_cl_write_latency << " " <<
                    res_info.pe_location << " " <<
                    res_info.cl_location << " " <<
                    res_info.num_outstanding << " " <<
                    res_info.repeat_rate << " | ";
            start_addr = itr->first;
            end_addr   = itr->first + MEM_BLOCK_SIZE;
        }
        ++ itr;
    }

    if (res_info.cl_location == MEM_LOCATE_GLOBAL) {
        CfMsg::DPrint (MSG_ERR,
          "  0x%08" PRIx32 " - 0x%08" PRIx32
          " %-4s %-3u     %-3u    %-3u     %-3u        %-3u       %-3u        %-3u        %-3u       %-3u        %-1s    %-1s    %-3u        %-3u\n",
          start_addr, end_addr - 1,
          restrict_str.c_str (),
          res_info.pe_fetch_latency,
          res_info.pe_read_latency,
          res_info.pe_write_latency,
          res_info.cl_fetch_latency,
          res_info.cl_read_latency,
          res_info.cl_write_latency,
          res_info.exo_cl_fetch_latency,
          res_info.exo_cl_read_latency,
          res_info.exo_cl_write_latency,
          "-",
          "-",
          res_info.num_outstanding,
          res_info.repeat_rate);
    } else if (res_info.pe_location == MEM_LOCATE_GLOBAL) {
        CfMsg::DPrint (MSG_ERR,
          "  0x%08" PRIx32 " - 0x%08" PRIx32
          " %-4s %-3u     %-3u    %-3u     %-3u        %-3u       %-3u        %-3u        %-3u       %-3u        %-1s    %-3u  %-3u        %-3u\n",
          start_addr, end_addr - 1,
          restrict_str.c_str (),
          res_info.pe_fetch_latency,
          res_info.pe_read_latency,
          res_info.pe_write_latency,
          res_info.cl_fetch_latency,
          res_info.cl_read_latency,
          res_info.cl_write_latency,
          res_info.exo_cl_fetch_latency,
          res_info.exo_cl_read_latency,
          res_info.exo_cl_write_latency,
          "-",
          res_info.cl_location,
          res_info.num_outstanding,
          res_info.repeat_rate);
    } else {
        CfMsg::DPrint (MSG_ERR,
          "  0x%08" PRIx32 " - 0x%08" PRIx32
          " %-4s %-3u     %-3u    %-3u     %-3u        %-3u       %-3u        %-3u        %-3u       %-3u        %-3u  %-3u  %-3u        %-3u\n",
          start_addr, end_addr - 1,
          restrict_str.c_str (),
          res_info.pe_fetch_latency,
          res_info.pe_read_latency,
          res_info.pe_write_latency,
          res_info.cl_fetch_latency,
          res_info.cl_read_latency,
          res_info.cl_write_latency,
          res_info.exo_cl_fetch_latency,
          res_info.exo_cl_read_latency,
          res_info.exo_cl_write_latency,
          res_info.pe_location,
          res_info.cl_location,
          res_info.num_outstanding,
          res_info.repeat_rate);
    }
    mem_info << start_addr << " " <<
        end_addr - 1 << " " <<
        restrict_str.c_str () << " " <<
        res_info.pe_fetch_latency << " " <<
        res_info.pe_read_latency << " " <<
        res_info.pe_write_latency << " " <<
        res_info.cl_fetch_latency << " " <<
        res_info.cl_read_latency << " " <<
        res_info.cl_write_latency << " " <<
        res_info.exo_cl_fetch_latency << " " <<
        res_info.exo_cl_read_latency << " " <<
        res_info.exo_cl_write_latency << " " <<
        res_info.pe_location << " " <<
        res_info.cl_location << " " <<
        res_info.num_outstanding << " " <<
        res_info.repeat_rate << " | ";
    return mem_info.str();
}


MemBody::~MemBody (void)
{
    for ( MemBlockMap::const_iterator itr = m_memmap.begin();
          itr != m_memmap.end(); ++ itr ) {
        delete itr->second;
    }
    m_memmap.clear();
}


template SysErrCause MemBody::MemOperate<INST_READ, MemBody::ACC_NORM> (
        SideBand, PhysAddr, uint8_t*, uint32_t);
template SysErrCause MemBody::MemOperate<DATA_READ, MemBody::ACC_NORM> (
        SideBand, PhysAddr, uint8_t*, uint32_t);
template SysErrCause MemBody::MemOperate<DATA_WRITE, MemBody::ACC_NORM> (
        SideBand, PhysAddr, uint8_t*, uint32_t);
template SysErrCause MemBody::MemOperate<INST_READ, MemBody::ACC_DBG> (
        SideBand, PhysAddr, uint8_t*, uint32_t);
template SysErrCause MemBody::MemOperate<DATA_READ, MemBody::ACC_DBG> (
        SideBand, PhysAddr, uint8_t*, uint32_t);
template SysErrCause MemBody::MemOperate<DATA_WRITE, MemBody::ACC_DBG> (
        SideBand, PhysAddr, uint8_t*, uint32_t);
