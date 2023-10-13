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
#include "./sideband.h"
#include "./trace_info.h"

class NcSRegFile;
class FsBus;
class TraceCtrl;
class ModuleGuard;

/*!
 * @brief Instruction Cache
 */
class Icache {
public:
    const uint32_t LINE_BYTES;  //!< Cache Line Size
    const uint32_t LINE_MASK;   //!< Cache Line Address Mask

    enum InstCacheOpType {
        OP_CHBII   = 0x00,
        OP_CIBII   = 0x20,
        OP_CFALI   = 0x40,
        OP_CISTI   = 0x60,
        OP_CILDI   = 0x61,
        OP_CLL     = 0x7E,
    };

    enum InstPrefOpType {
        OP_PREFI = 0x00,
    };

    static const uint32_t TAG_UPPER_POS = 27; //!< bit 31-28 is fixed to 0

    uint32_t m_tag_shift;
    uint32_t m_entry_mask;
    MemAccessRoute m_fill_route;

    SysErrCause FetchFromIC (PhysAddr vaddr, PhysAddr paddr, MemData8* data,
                      ByteSize iq_data_size, SideBand sideband, TraceCtrl* trace);
    void Invalidate (void);
    uint64_t GetAccessCount (void) const { return m_hit_count + m_miss_count; }
    uint64_t GetHitCount    (void) const { return m_hit_count; }
    uint64_t GetMissCount   (void) const { return m_miss_count; }
    void ResetPerfCounter (void);
    void Reset (void);
    uint32_t ReadIccfgData (void) const;

    void OperateChbii (PhysAddr paddr, PhysAddr vaddr, TraceCtrl* trace);
    void OperateCibii (uint32_t data, TraceCtrl* trace);
    void OperateCfali (PhysAddr paddr, PhysAddr vaddr, SideBand sideband, TraceCtrl* trace);
    void OperateCisti (uint32_t data, NcSRegFile* nc_srf, TraceCtrl* trace);
    void OperateCildi (uint32_t data, NcSRegFile* nc_srf, TraceCtrl* trace);
    void OperatePref  (PhysAddr paddr, PhysAddr vaddr, SideBand sideband, TraceCtrl* trace);

    bool IsCachingArea (PhysAddr paddr) const;

    void SetRomAreaAddr (PhysAddr rom_addrl, PhysAddr rom_addru);

    CacheWayType GetIcacheWayType (void) const { return m_way_type; }
    void SetIcacheWayType (CacheWayType way);
    ByteSize GetIcacheSize (void) const { return m_ic_size; }
    void SetIcacheSize (ByteSize size);
    bool GetEccState (void) const { return m_enable_ecc; }
    void SetEccState (bool enable) { m_enable_ecc = enable; }
    bool IsIcacheSizeZero (void) const;

    uint32_t GetLineByte (void) const { return LINE_BYTES; }

    void SetCfgPtr (ModuleGuard* cfg) { m_cfg = cfg; }

    Icache (ByteSize ic_size, CacheWayType way_type, FsBus* fsbus,
            PhysAddr rom_addrl, PhysAddr rom_addru, uint32_t peid,
            uint32_t line_byte, CoreType  core_type);
    ~Icache ();

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

private:
    static const uint32_t IC_MISS = 0x80;   //!< Identifier for Cache Miss
    static const uint32_t MAX_WAY = 4;      //!< Max number of Way for Cache. Fixed for LRU control.
    static const PhysAddr NO_ADDR = 0;      //!< Dummy address for IC(Instruction Cache) trace

    const uint32_t DATA_SHIFT_MASK;
    const uint32_t DATA_OFFSET_MASK;

    const CoreType  m_core_type;

    /*!
    *  @brief Cache Line
    */
    class IcacheLine {
    private:
        uint32_t m_tag;               //!< Tag
        bool     m_v;                 //!< V(valid) Bit
        MemData8* m_data;             //!< Data
        uint32_t m_tag_ecc;           //!< ECC of the tag array
        uint32_t m_data_ecc;          //!< ECC of the data array
        bool     m_lru_lock;          //!< Information of Way lock

        FsBus* m_fsbus;
        void InitIcacheLine (void);
        uint32_t m_line_byte;
        PhysAddr m_line_mask;

    public:
        class Data64 {
        public:
            uint32_t m_h;
            uint32_t m_l;
            Data64 (uint32_t h, uint32_t l) : m_h (h), m_l (l) {}

            uint32_t GetICDATH (void) const { return ((m_l  << 16) & 0xFFFF0000U)
                                                   | ((m_l  >> 16) & 0x0000FFFFU); }
            uint32_t GetICDATL (void) const { return ((m_h  << 16) & 0xFFFF0000U)
                                                   | ((m_h  >> 16) & 0x0000FFFFU); }
            void SetICDATH (uint32_t data)  { m_l  = ((data << 16) & 0xFFFF0000U)
                                                   | ((data >> 16) & 0x0000FFFFU); }
            void SetICDATL (uint32_t data)  { m_h  = ((data << 16) & 0xFFFF0000U)
                                                   | ((data >> 16) & 0x0000FFFFU); }
        };

        void DataWriteFromIC (MemData8* data, uint32_t data_idx, const ByteSize size);
        SysErrCause IcLineFill (PhysAddr fill_addr, uint32_t tag, SideBand sideband);
        bool IsHit (uint32_t tag) const;
        void FillZero (uint32_t tag);

        uint32_t  GetTag     (void) const { return m_tag; }
        bool      GetV       (void) const { return m_v; }
        uint32_t  GetTagEcc  (void) const { return m_tag_ecc; }
        uint32_t  GetDataEcc (void) const { return m_data_ecc; }
        bool      IsLock     (void) const { return m_lru_lock & m_v; }
        bool      GetL       (void) const { return m_lru_lock; }
        Data64    GetLineData8B (uint32_t data_offset);

        void SetTag (uint32_t tag) { m_tag = tag; }
        void SetV (bool v) { m_v = v; }
        void SetTagEcc (uint32_t tag_ecc) { m_tag_ecc = tag_ecc; }
        void SetDataEcc (uint32_t data_ecc) { m_data_ecc = data_ecc; }
        void SetL (bool lock) { m_lru_lock = lock; }
        void SetLineData8B (Data64 data, uint32_t data_offset);

        IcacheLine (FsBus* fsbus, uint32_t line_byte);
        ~IcacheLine ();
    };
    typedef std::vector<IcacheLine*> IcLineVec;

    /*!
    * @brief Array of Cache line
    */
    class IcacheArray {
    public:
        // @@ Should be private
        IcLineVec m_line;
        uint32_t  m_lru;
        uint32_t  m_lru_on_mask[MAX_WAY];
        uint32_t  m_lru_off_mask[MAX_WAY];

        void InitV (void);
        void InitLruAndLock (void);
        IcacheArray (FsBus* fsbus, uint32_t line_byte);
        ~IcacheArray ();
    };

    typedef std::vector<IcacheArray*> IcArrayVec;
    IcArrayVec m_icache;

    uint32_t m_entry_shift;
    uint32_t m_way_shift;
    uint32_t m_way_mask;
    uint32_t m_tag_mask;

    ByteSize m_ic_size;        //!< Cache Size
    uint32_t m_entry_num;      //!< Total number of Entry
    CacheWayType m_way_type;   //!< Way Type
    bool m_enable_ecc;

    FsBus* m_fsbus;
    ModuleGuard* m_cfg;

    uint64_t m_hit_count;
    uint64_t m_miss_count;

    PhysAddr m_rom_addrl;
    PhysAddr m_rom_addru;

    const uint32_t m_peid;

    uint32_t CalEntryNum (ByteSize ic_size, CacheWayType way_type) const;
    void SetWayType (CacheWayType way_type);

    void InitLruMask (void);
    uint32_t GetTagFromAddr (PhysAddr paddr) const;
    uint32_t GetEntryFromAddr (PhysAddr vaddr) const;

    uint32_t FindWay4Way (uint32_t tag, uint32_t entry, uint32_t* prty) const;
    uint32_t FindWay2Way (uint32_t tag, uint32_t entry, uint32_t* prty) const;
    uint32_t FindWay1Way (uint32_t tag, uint32_t entry, uint32_t* prty) const;
    uint32_t FindWay0Way (uint32_t tag, uint32_t entry, uint32_t* prty) const;
    uint32_t FindWayPbs  (uint32_t tag, uint32_t entry, uint32_t* prty) const;
    uint32_t (Icache::*pFindIcHitWay) (uint32_t tag, uint32_t entry, uint32_t* prty) const;

    uint32_t GetWayFromLru4Way (uint32_t entry, uint32_t prty) const;
    uint32_t GetWayFromLru2Way (uint32_t entry, uint32_t prty) const;
    uint32_t GetWayFromLru1Way (uint32_t entry, uint32_t prty) const;
    uint32_t GetWayFromLruPbs  (uint32_t entry, uint32_t prty) const;
    uint32_t (Icache::*pGetLruWay) (uint32_t entry, uint32_t prty) const;

    void UpdateLru (uint32_t entry, uint32_t way);
    void SetWayToLru (uint32_t entry, uint32_t way);
    void UpdateLockWay (uint32_t entry, uint32_t way, bool lock, TraceCtrl* trace);
    uint32_t GetParity (uint32_t tag) const;
    uint32_t GetWayNum (void) const;
    void UpdateEntryParam (void);
};
