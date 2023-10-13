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

#include <sstream>
#include <map>
#include <vector>
#include "./string.h"
#include "./sideband.h"

struct MemResourceInfo;

class MemBody {
public:
    typedef bool MemDebugType;
    static const MemDebugType ACC_NORM = false;
    static const MemDebugType ACC_DBG  = true;

    //! \sa MemOperate
    SysErrCause MemBodyWrite (SideBand sideband, PhysAddr addr, const uint8_t* data, uint32_t size) {
        return MemOperate<DATA_WRITE, ACC_NORM> (sideband, addr, const_cast<uint8_t*>(data), size);
    }

    //! \sa MemOperate
    SysErrCause MemBodyRead (SideBand sideband, PhysAddr addr, uint8_t* data, uint32_t size) {
        return MemOperate<DATA_READ, ACC_NORM> (sideband, addr, data, size);
    }

    //! \sa MemOperate
    SysErrCause MemBodyFetch (SideBand sideband, PhysAddr addr, uint8_t* data, uint32_t size) {
        return MemOperate<INST_READ, ACC_NORM> (sideband, addr, data, size);
    }

    //! \sa MemOperate
    SysErrCause MemBodyWriteDebug (PhysAddr addr, const uint8_t* data, uint32_t size) {
        return MemOperate<DATA_WRITE, ACC_DBG> (SideBand::DebugAccess(),
                addr, const_cast<uint8_t*>(data), size);
    }

    //! \sa MemOperate
    SysErrCause MemBodyReadDebug (PhysAddr addr, uint8_t* data, uint32_t size) {
        return MemOperate<DATA_READ, ACC_DBG> (SideBand::DebugAccess(), addr, data, size);
    }

    //! \sa MemOperate
    SysErrCause MemBodyFetchDebug (PhysAddr addr, uint8_t* data, uint32_t size) {
        return MemOperate<INST_READ, ACC_DBG> (SideBand::DebugAccess(), addr, data, size);
    }

    bool MemResourceAlloc (PhysAddr addr);
    bool MemResourceFree  (PhysAddr addr);

    std::string MemResourceDisplay (MemResourceInfo) const;
    MemBody (MemRestrictType restrict_type, uint8_t init_value)
        : m_restrict_type (restrict_type), m_init_value (init_value) {}
    ~MemBody ();

    static const PhysAddr MEM_BLOCK_SIZE = 0x00001000UL; // 4KB単位で操作する仕様
    static const PhysAddr MEM_BLOCK_MASK = MEM_BLOCK_SIZE - 1;
    static const MemRestrictType ACC_PERMITTED
                 = MRT_READ_ENABLE | MRT_WRITE_ENABLE | MRT_FETCH_ENABLE;

private:
    MemRestrictType m_restrict_type;

    class MemBlock {
    public:
        std::vector<uint8_t> m_mem_body;

        void Write (PhysAddr addr, uint8_t* data, uint32_t size) {
            memcpy (&(m_mem_body[addr & MEM_BLOCK_MASK]), data, size);
        }

        void Read (PhysAddr addr, uint8_t* data, uint32_t size) const {
            memcpy (data, &(m_mem_body[addr & MEM_BLOCK_MASK]), size);
        }

        void Alloc (uint8_t ini) {
            m_mem_body.assign (MEM_BLOCK_SIZE, ini);
        }

        MemBlock () {}
        ~MemBlock() {}
    };

    typedef std::map<PhysAddr, MemBlock*> MemBlockMap;
    MemBlockMap m_memmap;

    uint8_t m_init_value;

    template <MemDebugType dbg_mode>
    MemBlock* GetMemBlock (SideBand sideband, PhysAddr base_addr);

    template <MemAccessCommand cmd>
    bool CheckPermitOperation (SideBand sideband, MemBlock* block) const;

    template <MemAccessCommand cmd, MemDebugType dbg_mode>
    SysErrCause MemOperate (SideBand sideband, PhysAddr addr, uint8_t* data, uint32_t size);
};
