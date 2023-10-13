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

class ForestUtil;


namespace cube {
#include "./forest_if.h"
class CubeSuiteIF : public CForestIF {
public:
    virtual APIErrorCode AddPEInfo (uint32_t cl_id, uint32_t pe_id, CoreType core_type);
    virtual APIErrorCode InstantiatePE (void);
    virtual APIErrorCode SetPEInfo (uint32_t pe_id, SetPEInfoID pe_info_id,
                                    uint32_t info_data);
    virtual APIErrorCode GetPEInfo (uint32_t pe_id, GetPEInfoID pe_info_id,
                                     uint32_t* info_data);
    virtual APIErrorCode StepExecute (TraceInfo** trace_info, ExtTraceInfo** ext_trace_info);
    virtual APIErrorCode HardReset (SimAttr attr, uint32_t pe_id);
    virtual APIErrorCode VMemWriteDebug (uint32_t pe_id, PhysAddr addr,
                                         uint32_t data, ByteSize size);
    virtual APIErrorCode VMemReadDebug (uint32_t pe_id, PhysAddr addr,
                                        ByteSize size, uint32_t* data);
    virtual APIErrorCode PMemWriteDebug (PhysAddr addr, uint32_t data,
                                         ByteSize size);
    virtual APIErrorCode PMemReadDebug (PhysAddr addr, ByteSize size,
                                        uint32_t* data);
    virtual APIErrorCode SetMemBlock (uint32_t id, MemoryBlock mem_block);
    virtual APIErrorCode GetMemBlock (uint32_t id, MemoryBlock* mem_block);
    virtual APIErrorCode DeleteMemBlock (uint32_t id);
    virtual APIErrorCode RegWriteDebug (uint32_t pe_id, CReg regno,
                                        const uint32_t* data);
    virtual APIErrorCode RegReadDebug (uint32_t pe_id, CReg regno,
                                       uint32_t* data);
    virtual APIErrorCode SetICacheSpec (uint32_t pe_id, CacheSpec cachespec);
    virtual APIErrorCode GetICacheSpec (uint32_t pe_id,  CacheSpec* cachespec);
    virtual APIErrorCode GetICacheInfo (uint32_t pe_id, CacheInfo* cacheinfo);
    virtual APIErrorCode SetDebugMode  (DebugMode mode);
    virtual APIErrorCode SetFileOut (const wchar_t* file_pathname);
    virtual APIErrorCode GetTraceInfo  (TraceInfo** trace);
    virtual APIErrorCode GetPeTraceInfo (uint32_t pe_id, TraceInfo** trace);
    virtual APIErrorCode SetModeFrequency(uint32_t pe_id, uint32_t ratio);
    virtual APIErrorCode SetInterruptToIntc1 (uint32_t pe_id, uint32_t channel);
    virtual APIErrorCode SetInterruptToIntc2 (uint32_t channel);
    virtual APIErrorCode SetFeintToIntc1 (uint32_t pe_id, uint32_t channel);
    virtual APIErrorCode SetFenmiToIntc1 (uint32_t pe_id);
    virtual APIErrorCode SetIORBaseAddr (SetIORBaseAddrID peripheral_id,
                                         PhysAddr base_addr);
    virtual APIErrorCode SetLocalIOROffsetAddr (PhysAddr start_address,
                                                PhysAddr end_addr,
                                                int32_t offset);
    virtual APIErrorCode ReqPseudoEiint (uint32_t pe_id, uint32_t channel,
                                         uint32_t priority, bool eitb);
    virtual APIErrorCode ReqPseudoFenmi (uint32_t pe_id);
    virtual APIErrorCode ReqPseudoFeint (uint32_t pe_id, uint32_t channel);
    virtual APIErrorCode ReqPseudoSyserr (uint32_t pe_id, uint32_t syserr_cause);
    virtual APIErrorCode ReqPseudoOperandSyserr (uint32_t peid, uint32_t syserr_cause,
                                                 bool is_gm, uint32_t gpid);
    CubeSuiteIF();
    virtual ~CubeSuiteIF();

private:
    ForestUtil* m_forest_util;

    ExtTraceInfo m_ext_trace_info;

    struct MemoryBlockInfo {
        uint32_t id;
        MemoryBlock mem_attr;
    };
    std::vector<MemoryBlockInfo> m_mem_attr;
    FILE* m_logfilep;

    // Reserved info by AddPEInfo
    struct PEInfo {
        uint32_t cl_id;
        uint32_t pe_id;
        CoreType core_type;
    };
    std::vector<uint32_t> m_pe_id_vec;
    std::vector<PEInfo> m_pe_info;

    struct OffsetInfo {
        PhysAddr m_start;
        PhysAddr m_end;
        int32_t  m_offset;
    };
    std::vector<OffsetInfo> m_offset_vec;

    static const uint32_t PIPE_DEPTH_G4MH    = 11;
    static const uint32_t MAX_INT_PRIORITY_G4MH1X = 15;
    static const uint32_t MAX_INT_CHANNEL_FEINT = 15;

    // Error check for argument of API
    bool IsPeIdValid (uint32_t pe_id) const;
    bool IsAddPEInfoValid (uint32_t pe_id, uint32_t core_type) const;
    bool IsSetPEInfoValid (SetPEInfoID pe_info_id, uint32_t info_data) const;
    bool IsGetPEInfoValid (GetPEInfoID pe_info_id) const;
    bool IsSimAttrValid (SimAttr attr, uint32_t pe_id) const;
    bool IsCacheSpecValid (CacheSpec* cachespec) const;
    bool IsMemoryBlockValid (MemoryBlock* mem_block) const;
    bool IsEntryDuplicated (uint32_t id, MemoryBlock* mem_block) const;
    bool IsAddrOverlaped (const MemoryBlock* mem_block1, const MemoryBlock* mem_block2) const;
    bool IsOffsetOverlaped (const OffsetInfo* offset_info1,
                            const OffsetInfo* offset_info2) const;
    void InitLocalIOROffsetAddr (void);
    void UpdatePeripheralOffset (SetIORBaseAddrID id, OffsetInfo* info);
    void UpdateLocalIOROffsetAddr (void);
    bool IsDebugModeValid (DebugMode mode) const;
    bool IsByteSizeValid (ByteSize size) const;
    bool IsSetIORBaseAddrIDValid (SetIORBaseAddrID peripheral_id) const;
};
}; // namespace cube
