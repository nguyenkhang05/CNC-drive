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

#include <string>
#include <vector>
#include <string.h>
#include <algorithm>
#include "./forest_common.h"
#include "./outstand_base.h"

class Dbuf;

enum MemDbufType {
    DBUF_LRAM_G4MH    = 0, // LRAM for G4MH
    DBUF_CRAM_G4MH    = 1, // CRAM for G4MH
    DBUF_FROM_G4MH    = 2, // FlashROM for G4MH
    DBUF_SIMPLE_MEM   = 3, // Peripherals, normal RAM
    DBUF_FROM_G4KH    = 4, // FlashROM for G4KH
    DBUF_CRAM_G4KH    = 5, // CRAM for G4KH
    DBUF_LRAM_G4KH    = 6, // LRAM for G4KH
};

template <MemDbufType dbuf_type>
class DLL_EXPORT_IMPORT MemLatencyInfo : public MemLatencyInfoBase
{
public:
    MemLatencyInfo (uint32_t pe_fetch_latency,
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
    ~MemLatencyInfo ();

    uint32_t GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) {
        return GetAccessLatency<DATA_READ> (peid, load_addr, start_cycle, size);
    }
    uint32_t GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) {
        return GetAccessLatency<DATA_WRITE> (peid, load_addr, start_cycle, size);
    }
    uint32_t GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size) {
        return GetAccessLatency<INST_READ> (peid, load_addr, start_cycle, size);
    }

    uint64_t GetReqAcceptableCycle (uint32_t peid, PhysAddr load_addr, ByteSize size) const
    { return m_mem_avail_cycle.front (); }

    void InitMemCycle (void);
    void UpdateExoPeLatency (uint32_t exo_latency_offset);
    void GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const;
    bool IsScheap (void) const { return false; }
    bool IsBufHitLast (PhysAddr addr) const { return m_is_buf_hit; }
    bool IsBufHit (uint32_t peid, PhysAddr addr) const;
    bool IsLramAccess (uint32_t peid) const { return m_is_lram_access && (peid == m_pe_location); }
    MemResourceType GetMemResourceType (uint32_t peid) const;
    uint32_t GetConflictLatency (void) const { return m_conflict_cycle; }
    std::string GetName (PhysAddr addr) const { return m_target_name; }
    void ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const;
#if _UNIT_TEST
    void SetDbufPtr (Dbuf* ptr) { m_dbuf = ptr; }
    Dbuf* GetDbufPtr (void) { return m_dbuf; }
#endif // #if _UNIT_TEST

private:
#if _UNIT_TEST
    Dbuf* m_dbuf;
#endif // #if _UNIT_TEST
    uint32_t m_pe_fetch_latency;
    uint32_t m_pe_read_latency;
    uint32_t m_pe_write_latency;
    uint32_t m_cl_fetch_latency;
    uint32_t m_cl_read_latency;
    uint32_t m_cl_write_latency;
    uint32_t m_exo_cl_fetch_latency;
    uint32_t m_exo_cl_read_latency;
    uint32_t m_exo_cl_write_latency;
    MemLocationType m_pe_location;
    MemLocationType m_cl_location;
    uint32_t m_outstand_num;
    uint32_t m_repeat_interval_cycle;
    uint32_t m_next_interval_cycle;
    uint32_t m_conflict_cycle;

    uint64_t m_gramc_req_avail_cycle;

    PhysAddr m_last_rmw_addr;

    bool m_is_buf_hit;
    bool m_is_lram_access;
    enum MemLatency {
        ID_TO_MEM_INTERFACE = 2
    };
    MemAccessCommand m_last_acc_type;

    PhysAddr m_last_tcm_addr;
    ByteSize m_last_tcm_size;
    uint64_t m_last_start_time;
    uint64_t m_last_time;
    uint32_t m_last_latency;
    bool     m_is_last_2way;
    PhysAddr m_last_st_addr;
    uint64_t m_last_st_cycle;
    uint64_t m_dbuf_fill_cycle;
    uint64_t m_fill_lock_cycle;

    // for arbitoration change penalty in FlashROM
    uint32_t m_last_acc_peid;
    MemAccessCommand m_last_acc_cmd;

    template <MemAccessCommand cmd>
    uint32_t GetAccessLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size);

    template <MemAccessCommand cmd>
    uint32_t GetMemModuleLatency (uint32_t peid) const;

#ifdef ENABLE_CLUSTER
    template <MemAccessCommand cmd>
    uint32_t CalcMemModuleLatency (uint32_t peid) const;
#endif // ENABLE_CLUSTER

    void SetupMemModuleLatency (void);
    typedef std::vector<uint32_t> LatencyVec;
    std::vector<LatencyVec> m_latency_info;
    static const uint32_t FETCH_LAT_OFFSET_G3MH = 4;

    typedef std::vector<CoreType> CoreTypeVec;
    CoreTypeVec m_coretype_info;

    uint32_t GetRmwLatency (ByteSize size) const;

    std::vector<uint64_t> m_mem_avail_cycle;

    void UpdateMemAvailCycle (uint64_t avail_cycle);

    uint32_t GetDbufLatency (uint32_t peid, PhysAddr load_addr, uint32_t mem_latency,
                             uint64_t issue_cycle, Dbuf* dbuf, MemAccessCommand cmd);
    template <MemAccessCommand cmd>
    uint32_t GetDbufLatencyG4KH (PhysAddr load_addr, uint32_t mem_latency,
                                 uint64_t issue_cycle, Dbuf* dbuf);
    uint32_t GetNoBufLatency (uint32_t mem_latency, uint64_t issue_cycle);
    uint32_t GetNoBufLatencyG4KH (uint32_t mem_latency, uint64_t issue_cycle, bool* is_post_write);
    uint64_t GetTcmAdditinalLatency (uint64_t issue_cycle);

    void DumpOutstandQueue (void) const;

    const uint32_t m_dbuf_hit_latency;
    const MemResourceType m_resource_type;
    const std::string m_target_name;

    uint32_t m_last_cl_peid;
};


class DLL_EXPORT_IMPORT MemLatencyInfoGram : public MemLatencyInfoBase
{
public:
    MemLatencyInfoGram (uint32_t pe_fetch_latency,
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
                    std::string name);
    ~MemLatencyInfoGram ();

    uint32_t GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,  ByteSize size);
    uint32_t GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    uint32_t GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    uint64_t GetReqAcceptableCycle (uint32_t peid, PhysAddr load_addr, ByteSize size) const
    { return (m_bank[GetBankId(load_addr)])->GetReqAcceptableCycle (peid, load_addr, size); }
    void InitMemCycle (void);
    void UpdateExoPeLatency (uint32_t exo_latency_offset);
    void GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const;
    bool IsScheap (void) const { return false; }
    bool IsBufHitLast (PhysAddr addr) const { return false; }
    bool IsBufHit (uint32_t peid, PhysAddr addr) const { return false; }
    bool IsLramAccess (uint32_t peid) const {return false;}
    MemResourceType GetMemResourceType (uint32_t peid) const;
    uint32_t GetConflictLatency (void) const;
    std::string GetName (PhysAddr addr) const;
    void ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const;

#if _UNIT_TEST
    void SetDbufPtr (Dbuf* ptr)
    {
        std::vector<MemLatencyInfo<DBUF_CRAM_G4MH>*>::iterator it = m_bank.begin ();
        while (it != m_bank.end()) {
            (*it)->SetDbufPtr (ptr);
            ++ it;
        }
    }
#endif // #if _UNIT_TEST

private:
    static const uint32_t NUM_BANK = 4;
#ifdef ENBALE_SUBBANK_CRAM
    uint32_t GetBankId (PhysAddr addr) const { return (addr >> 2) & 0x3; } // WordBand0-3
#else // ENBALE_SUBBANK_CRAM
    uint32_t GetBankId (PhysAddr addr) const { return 0; } // No wordbank.
#endif // ENBALE_SUBBANK_CRAM
    std::vector<MemLatencyInfo<DBUF_CRAM_G4MH>*> m_bank;

    template <MemAccessCommand cmd>
    uint32_t GetAccessLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size);

    void ReallocMbi (void);
    void DeleteMbi (void);

    class MbiSlot {
    public:
        // MBI slot0 = bank0/2, MBI slot1 = bank1/3
        // Each MBI slot can process 1 req/cycle.
        // @@ TODO : Need to sync between different Cluster of CRAM (because different cluaster access is blocking access)
        uint64_t GetAndUpdateReqDelayCycle(PhysAddr load_addr, uint64_t req_cycle) {
            uint64_t add_latency = 0;
            uint32_t mbiid = GetMbiBankId (load_addr);
            if (m_last_cycle[mbiid] > req_cycle) {
                add_latency = m_last_cycle[mbiid] - req_cycle;
                m_last_cycle[mbiid] += 1;
            } else {
                m_last_cycle[mbiid] = req_cycle + 1;
            }
            return add_latency;
        }
        void Init (void) { m_last_cycle.clear(); m_last_cycle.assign (NUM_MBI_BANK, 0); }
        MbiSlot () { Init(); }
        ~MbiSlot () {};
    private:
        static const uint32_t NUM_MBI_BANK = 2;
        std::vector <uint64_t> m_last_cycle;
        uint32_t GetMbiBankId (PhysAddr addr) const { return (addr >> 2) & 0x1; } // MBIbank slot0/1
    };
    std::vector<MbiSlot*> m_mbi_slot;

    class OrderCtrlSc {
    public:
        // CRAM access order is SC. Following access can't send before prior access.
        // Fastest request timing is same timing of previous request in SC.
        uint64_t GetAndUpdateReqDelayCycle(uint64_t req_cycle) {
            uint64_t add_latency = 0;
            if (m_last_cycle > req_cycle) {
                add_latency = m_last_cycle - req_cycle;
            } else {
                m_last_cycle = req_cycle;
            }
            return add_latency;
        }
        void Init (void) { m_last_cycle = 0; }
        OrderCtrlSc () { Init(); }
        ~OrderCtrlSc () {};
    private:
        uint64_t m_last_cycle;
    };
    OrderCtrlSc m_cram_order_ctrl;
};


class DLL_EXPORT_IMPORT MemLatencyInfoLram : public MemLatencyInfoBase
{
public:
    MemLatencyInfoLram (uint32_t pe_fetch_latency,
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
                    std::string name);
    ~MemLatencyInfoLram ();

    uint32_t GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,  ByteSize size);
    uint32_t GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    uint32_t GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    uint64_t GetReqAcceptableCycle (uint32_t peid, PhysAddr load_addr, ByteSize size) const
    { return (m_bank[GetBankId(load_addr)])->GetReqAcceptableCycle (peid, load_addr, size); }
    void InitMemCycle (void);
    void UpdateExoPeLatency (uint32_t exo_latency_offset);
    void GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const;
    bool IsScheap (void) const { return false; }
    bool IsBufHitLast (PhysAddr addr) const { return false; }
    bool IsBufHit (uint32_t peid, PhysAddr addr) const { return false; }
    bool IsLramAccess (uint32_t peid) const {return m_bank[0]->IsLramAccess(peid);}
    MemResourceType GetMemResourceType (uint32_t peid) const;
    uint32_t GetConflictLatency (void) const;
    std::string GetName (PhysAddr addr) const;
    void ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const;
#if _UNIT_TEST
    void SetDbufPtr (Dbuf* ptr)
    {
        std::vector<MemLatencyInfo<DBUF_LRAM_G4MH>*>::iterator it = m_bank.begin ();
        while (it != m_bank.end()) {
            (*it)->SetDbufPtr (ptr);
            ++ it;
        }
    }
#endif // #if _UNIT_TEST

private:
    static const uint32_t NUM_BANK = 4;
#ifdef ENBALE_SUBBANK_LRAM
    uint32_t GetBankId (PhysAddr addr) const { return (addr >> 2) & 0x3; } // WordBank0-3
#else // ENBALE_SUBBANK_LRAM
    uint32_t GetBankId (PhysAddr addr) const { return 0; } // no wordbank
#endif // ENBALE_SUBBANK_LRAM
    std::vector<MemLatencyInfo<DBUF_LRAM_G4MH>*> m_bank;

    template <MemAccessCommand cmd>
    uint32_t GetAccessLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size);

    class OrderCtrlLramSto {
    public:
        // Because G4MH LRAM is STO, write access waits both preceiding read/write.
        // Because of bug in RTL, read access doesn't wait preceiding read/write (except the write with same address.)
        // (If there is no the bug, read access wait preceiding write access.)
        uint64_t GetAndUpdateReqDelayCycle(uint64_t req_cycle, uint32_t bankid) {
            uint64_t add_latency = 0;
            if (m_last_cycle[bankid] > req_cycle) {
                add_latency = m_last_cycle[bankid] - req_cycle;
            } else {
                m_last_cycle[bankid] = req_cycle;
            }
            return add_latency;
        }
        void UpdateLatestReqCycle(uint64_t req_cycle, uint32_t bankid) {
            m_last_cycle[bankid] = std::max (m_last_cycle[bankid], req_cycle);
        }
        void Init (void) { m_last_cycle.assign (NUM_BANK, 0U); }
        OrderCtrlLramSto () { Init(); }
        ~OrderCtrlLramSto () {};
    private:
        std::vector<uint64_t> m_last_cycle;
    };
    OrderCtrlLramSto m_lram_order_ctrl;
};


template <MemDbufType dbuf_type>
class DLL_EXPORT_IMPORT MemLatencyInfoBank : public MemLatencyInfoBase
{
public:
    MemLatencyInfoBank (uint32_t pe_fetch_latency,
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
    ~MemLatencyInfoBank ();

    uint32_t GetReadLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,  ByteSize size);
    uint32_t GetWriteLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    uint32_t GetFetchLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle, ByteSize size);
    uint64_t GetReqAcceptableCycle (uint32_t peid, PhysAddr load_addr, ByteSize size) const
    { return (m_bank[GetBankId(load_addr)])->GetReqAcceptableCycle (peid, load_addr, size); }
    void InitMemCycle (void);
    void UpdateExoPeLatency (uint32_t exo_latency_offset);
    void GetResourceInfo (MemResourceInfo* res_info, PhysAddr addr) const;
    bool IsScheap (void) const { return false; }
    bool IsBufHitLast (PhysAddr addr) const;
    bool IsBufHit (uint32_t peid, PhysAddr addr) const;
    bool IsLramAccess (uint32_t peid) const {return false;}
    MemResourceType GetMemResourceType (uint32_t peid) const;
    uint32_t GetConflictLatency (void) const;
    std::string GetName (PhysAddr addr) const;
    void ShowAccessRoute (uint32_t peid, MemAccessCommand cmd, PhysAddr addr) const;
#if _UNIT_TEST
    void SetDbufPtr (Dbuf* ptr)
    {
        auto it = m_bank.begin ();
        while (it != m_bank.end()) {
            (*it)->SetDbufPtr (ptr);
            ++ it;
        }
    }
#endif // #if _UNIT_TEST

private:
    static const uint32_t NUM_BANK = 4;
    uint32_t GetBankId (PhysAddr addr) const;
    std::vector<MemLatencyInfoBase*> m_bank;

    template <MemAccessCommand cmd>
    uint32_t GetAccessLatency (uint32_t peid, PhysAddr load_addr, uint64_t start_cycle,
            ByteSize size);

    uint32_t m_bank_num;
    uint32_t m_shift_val;
    uint32_t m_bank_id_mask;
    uint32_t m_bank_offset;
    uint32_t m_block_num;
    uint32_t m_block_size;
    void CalcBankParam (std::vector<ByteSize>* bank_size, PhysAddr start_addr, PhysAddr end_addr);
    bool Is2PowN (uint32_t val) const;
    uint32_t GetLog2u32 (uint32_t val) const;
    uint32_t GetGCD2 (uint32_t small_data, uint32_t big_data) const;
    uint32_t GetGCDN (std::vector<ByteSize>* bank_size) const;
};
