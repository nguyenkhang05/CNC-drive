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

#include <utility>
#include "./trace_info.h"
#include "./sideband.h"


class TraceCtrl;
class FsBus;
class Icache;
class SRegFile;
class Mpu;
#ifdef ENABLE_DBG_BREAK
class DbgBreak;
#endif // #ifdef ENABLE_DBG_BREAK
class Llsc;
class ProcElement;
class InstQueue;
class Misr;
class ModuleGuard;

/*!
 * @brief Load Store Unit
 */
class Lsu {
public:
    // Area is fixed in PE spec.
    // FlashROM area
    static const PhysAddr ROM_BGN = 0x00000000UL;
    static const PhysAddr ROM_END = 0x0FFFFFFFUL;
    // L1RAM,L2RAM,VCI64
    static const PhysAddr PE_LOCAL_BGN = 0xFC000000ULL;
    static const PhysAddr PE_LOCAL_END   = 0xFFFFFFFFULL;

    // Fixed addr for SPCLD=0 for PEG error record target.
    // Actually not fixed. But LSU Func spec recommended.
    // Because L1RAM/L2RAM may be put different addr,
    // only fix SPCLD to 0 after L2RAM area.
    static const PhysAddr SPCLD_FIXED_0_BGN = 0xFF000000UL;

    // L2RAM temporal (not fixed)
    static const PhysAddr L2RAM_BGN = 0xFE000000ULL;
    static const PhysAddr L2RAM_END = 0xFEFFFFFFULL;

    typedef bool SpcldType;
    static const SpcldType SPCLD_OFF = false;
    static const SpcldType SPCLD_ON  = true;

    void MemWriteBitOp (PhysAddr vaddr, MemData data, ByteSize size, SideBand::BitOpType bitop);
    MemData MemReadBitOp (PhysAddr vaddr, ByteSize size, SideBand::BitOpType bitop);
    void MemWrite (PhysAddr vaddr, MemData data, ByteSize size, bool isBitOp = false);
    MemData MemRead (PhysAddr vaddr, ByteSize size, SpcldType is_spcld);
    void MemWrite64 (PhysAddr vaddr, MemData64 data);
    MemData64 MemRead64 (PhysAddr vaddr);
    void MemWrite128 (PhysAddr vaddr, MemData64 data_u, MemData64 data_l);
    std::pair<MemData64, MemData64> MemRead128 (PhysAddr vaddr);
    void MemWriteDebug (PhysAddr vaddr, MemData data, ByteSize size);
    MemData MemReadDebug (PhysAddr vaddr, ByteSize size);
    MemData MemFetchDebug (PhysAddr vaddr, ByteSize size);

#if defined (ENABLE_TRACE_MDP_SUPPRESS)
    void MemReadBitOpMdp (PhysAddr vaddr, ByteSize size, SideBand::BitOpType bitop);
    void MemReadMdp (PhysAddr vaddr, ByteSize size, SpcldType is_spcld);
    void MemWriteBitOpMdp (PhysAddr vaddr, MemData data, ByteSize size, SideBand::BitOpType bitop);
    void MemWriteMdp (PhysAddr vaddr, MemData data, ByteSize size, bool isBitOp = false);
    void MemWrite64Mdp (PhysAddr vaddr, MemData64 data);
    void MemWrite128Mdp (PhysAddr vaddr, MemData64 data_u, MemData64 data_l);
#endif // #if defined (ENABLE_TRACE_MDP_SUPPRESS)

    PhysAddr MemInstFetch (PhysAddr vline_addr, MemData8* line_data,
                           uint32_t line_size);
    PhysAddr MemReadWithLinkCommon (PhysAddr vaddr, MemData8* data, ByteSize size);
    MemData MemReadWithLink (PhysAddr vaddr, ByteSize size);
    MemData64 MemRead64WithLink (PhysAddr vaddr);
    bool IsHaveLink (PhysAddr addr, ByteSize size) const;
#ifdef FROG_API_IF
    void CreateLink (int32_t scid, PhysAddr addr, ByteSize size);
#endif // FROG_API_IF
    void DeleteLink (PhysAddr addr);
    void DeleteOwnLink (void);
    void DeleteOwnLink (PhysAddr vaddr, ByteSize size);
    void DeleteAllLink (void);
    void DeletePeLink (void);
    void DeleteVmLink (void);
    void DeleteLinkByExternal (PhysAddr paddr, ByteSize size);

    void ApplyLsabEoReserved (TraceCtrl* trace);
    bool IsHitReadBrk (PhysAddr vaddr, ByteSize size, LsabType is_eiint = LSAB_NORMAL);
    bool IsHitWriteBrk (PhysAddr vaddr, ByteSize size, LsabType is_eiint = LSAB_NORMAL);
    bool IsHitRmwBrk (PhysAddr vaddr, ByteSize size);
    bool IsAlignmentError (PhysAddr vaddr);
    template <MemAccessCommand cmd>
    bool IsMemException (PhysAddr vaddr, ByteSize size);
    template <MemAccessCommand cmd>
    bool IsMemExceptionEI (PhysAddr vaddr, ByteSize size);
    bool IsMemExceptionRMW (PhysAddr vaddr, ByteSize size);
    bool IsMemExceptionTST (PhysAddr vaddr);
    template <ByteSize size>
    bool IsMemExceptionAtomic (PhysAddr vaddr);
    template <MemAccessCommand cmd>
    bool IsMemExceptionFXU (PhysAddr vaddr, ByteSize size);
    PhysAddr AddressTransfer (PhysAddr vaddr) const { return vaddr; }
    PhysAddr GetBusAddress (PhysAddr addr) const;

    bool IsFlashROMArea (PhysAddr addr) const;
    bool IsPeLocalArea (PhysAddr addr) const;
    bool IsSpcldFixed0Area (PhysAddr addr) const { return addr >= SPCLD_FIXED_0_BGN; }

    PhysAddr m_gram_bgn_addr;
    PhysAddr m_gram_end_addr;
    bool IsGramArea (PhysAddr addr) const
    { return (addr >= m_gram_bgn_addr) && (addr <= m_gram_end_addr); }
    void SetGramArea (PhysAddr start_addr, PhysAddr end_addr)
    {
        m_gram_bgn_addr = start_addr;
        m_gram_end_addr   = end_addr;
    }
    PhysAddr m_lram_bgn_addr;
    PhysAddr m_lram_end_addr;
    bool IsLramArea (PhysAddr addr) const
    { return (addr >= m_lram_bgn_addr) && (addr <= m_lram_end_addr); }
    void SetLramArea (PhysAddr start_addr, PhysAddr end_addr)
    {
        m_lram_bgn_addr = start_addr;
        m_lram_end_addr   = end_addr;
    }
    MissAlignType GetMissAlignType (void) const { return m_miss_align_type; }

    SideBand GetSideBand (void) { return m_sideband; }
    SideBand* GetSideBandPtr (void) { return &m_sideband; }

    void SetMdpSuppress (bool is_suppress) { m_mdp_supress = is_suppress; }

    Mpu* GetMpuPtr () const { return m_mpu; }

#ifdef ENABLE_DBG_BREAK
    void SetDbgBrkPtr (DbgBreak* dbgbrk) { m_dbgbrk = dbgbrk; }
#endif // #ifdef ENABLE_DBG_BREAK

    Llsc* GetLlscPtr () const { return m_llsc; }
    PhysAddr m_llmaster_addr;
    void SetLLMasterAddr (PhysAddr addr) { m_llmaster_addr = addr; }
    bool IsMatchLLMasterAddr (PhysAddr addr, PhysAddr mask) const
    { return (m_llmaster_addr & mask) == (addr & mask); }

    void SetCfgPtr (ModuleGuard* cfg) { m_cfg = cfg; }
    void SetGrgPtr (ModuleGuard* grg) { m_grg = grg; }
    ModuleGuard* GetGrgPtr (void) { return m_grg; }

    void SetScId (int32_t sc_id);

    void InitMemRoute (void);

    void ExecCfali (PhysAddr vaddr);

    void SetTraceCtrl (TraceCtrl* trace);
    void SetInstQueue (InstQueue* inst_queue) { m_inst_queue = inst_queue; }

    uint64_t m_fetch_count;
    uint64_t m_flashrom_fetch_count;
    uint64_t m_flashrom_data_count;
    uint64_t GetTotalFetchCount (void) const { return m_fetch_count; }
    uint64_t GetRomFetchCount (void) const { return m_flashrom_fetch_count; }
    uint64_t GetRomDataAccCount (void) const { return m_flashrom_data_count; }
    void ResetPerf (void);

    bool IsEnabledOperandSyserr (void) const { return m_enable_operand_syserr; }
    void HandleErrorResponseForOperand (SysErrCause sys_err_cause);
    void InitSyserrEvent (void);
    SysErrCause GetSyserrCause (bool is_host, uint32_t gpid) const;
    void SetSyserrCause (bool is_host, uint32_t gpid, SysErrCause cause);
#if defined(COMP_RUNNER_G4MH)
    /**
     * \fn bool GetOperandSyserrStatus (bool is_host, uint32_t gpid) const;
     * \brief Gets operand sysesrr status flag.
     * This function returns status of operand syserr flag.
     *
     * @param[in]  is_host      syserr mode.
     * @param[in]  gpid         syserr CPU gpid.
     *
     * @return bool value
     * @retval true         : flag is set.
     * @retval false        : flag is not set.
     *
     */
    bool GetOperandSyserrStatus (bool is_host, uint32_t gpid) const;
    /**
     * \fn void ClearOperandSyserrStatus (bool is_host, uint32_t gpid) ;
     * \brief Clears operand sysesrr status.
     * This function clears status of operand syserr flag.
     *
     * @param[in]  is_host      syserr mode.
     * @param[in]  gpid         syserr gpid.
     *
     */
    void ClearOperandSyserrStatus (bool is_host, uint32_t gpid) ;
#endif

    Lsu (TraceCtrl* trace, FsBus* fsbus, Icache* ic, SRegFile* srf,
#ifdef ENABLE_DBG_BREAK
         DbgBreak* dbgbrk,
#endif // #ifdef ENABLE_DBG_BREAK
         Llsc* llsc, Misr* misr, ContextId tc_id,
         MissAlignType maa, ProcElement* pe,
         SysErrCause syserr_fetch_guest_error, bool enable_operand_syserr);
    ~Lsu ();

private:
    SideBand   m_sideband;
    TraceCtrl* m_trace;
    FsBus*     m_fsbus;
    Icache*    m_ic;
    SRegFile*  m_srf;
    Mpu*       m_mpu;
#ifdef ENABLE_DBG_BREAK
    DbgBreak*  m_dbgbrk;
#endif // #ifdef ENABLE_DBG_BREAK
    Llsc*      m_llsc;
    Misr*      m_misr;

    ModuleGuard* m_cfg;
    ModuleGuard* m_grg;

    ContextId m_tc_id;
    int32_t   m_sc_id;
    MissAlignType m_miss_align_type;
    ProcElement* m_pe;
    InstQueue* m_inst_queue;

    MemAccessRoute m_mem_route;
    const SysErrCause m_syserr_cause_fetch_guest;
    const bool m_enable_operand_syserr;

    bool m_mdp_supress;

    class MemDataU32 {
    public:
        union {
            MemData  d32;
            MemData8 d8[4];
        };
        explicit MemDataU32 (MemData data) : d32 (data) {}
    private:
        MemDataU32 () {}
    };

    class MemDataU64 {
    public:
        union {
            MemData64 d64;
            MemData   d32[2];
            MemData8  d8[8];
        };
        explicit MemDataU64 (MemData64 data) : d64 (data) {}
    private:
        MemDataU64 () {}
    };

    class MemDataU128 {
    public:
        union {
            MemData64 d64[2];
            MemData   d32[4];
            MemData8  d8[16];
        };
        MemDataU128 (MemData64 data_u, MemData64 data_l)
        { d64[1] = data_u; d64[0] = data_l; }
    private:
        MemDataU128 () {}
    };

    template <MemAccessMode acc_mode, MemAccessCommand cmd>
    PhysAddr MemAccess (PhysAddr vaddr, MemData8* data, ByteSize size);

    template <MemAccessMode acc_mode, MemAccessCommand cmd>
    SysErrCause BusAccess (PhysAddr paddr, MemData8* data, uint32_t bytes) const;

    template <MemAccessMode acc_mode, MemAccessCommand cmd>
    SysErrCause BusAccessRapArround (PhysAddr paddr, MemData8* data, uint32_t bytes) const;

    bool IsMemAlignError (PhysAddr vaddr, ByteSize size) const;
    template <ByteSize size>
    bool IsMemAlignErrorAtomic (PhysAddr vaddr) const;
    bool IsMemAlignErrorFXU (PhysAddr vaddr, ByteSize size) const;

    bool m_peg_error;

    // Internal record for SYSERR on G4MH2.0 to keep pending event's cause code.
    SysErrCause m_host_syserr_cause;
    std::vector<SysErrCause> m_guest_syserr_cause;

#if defined(COMP_RUNNER_G4MH)
    /**
     * \var bool m_host_operand_syserr_status
     * \brief operand syserr flag for host mode.
     * This flag turns on when event for operand syserr is satisfied.
     * This flag turns off when operand syserr is accepted.
     *
     */
    bool m_host_operand_syserr_status;
    /**
     * \var std::vector<bool> m_guest_operand_syserr_status
     * \brief operand syserr flag for guest mode.
     * These flags turn on when event for operand syserr is satisfied.
     * These flags turn off when operand syserr is accepted.
     *
     */
    std::vector<bool> m_guest_operand_syserr_status;
#endif

    Lsu ();
};
