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
#include "./forest_utility.h"
#include "./sregfile_base.h"
#include "./proc_element_base.h"
class NcSRegFile;
//class SRegFileSet;
class GRegFileSet;
class SRegDUMMY;
class NcSRegFile;
class SRegFile;
class GRegFile;

class RegName;
class FsBus;
class Icache;
class Peg;
class CIntc1;
class CIntc2;
#ifdef ENABLE_DBG_BREAK
class DbgBreak;
#endif // #ifdef ENABLE_DBG_BREAK
class IntReqInfoBase;
class GRegFile;
class GRegFileSet;
class FsCtrl_Interface;
class FsCtrl;
class Llsc;
class CsLsuBuf;
class Dbuf;
class Misr;
#ifdef ENABLE_OSTM
class OstmIO;
#endif // #ifdef ENABLE_OSTM
class ModuleGuard;
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
class FlashCache;
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)

class ProcElement : public ProcElementBase {
public:
    static const PhysAddr FLASH_ROM_SIZE_G4P = 0x02000000U;

#ifdef ENABLE_OSTM
    static const uint32_t MAX_OSTM_NUM = 10;
#endif // #ifdef ENABLE_OSTM

#if _UNIT_TEST
    typedef std::vector<FsCtrl_Interface*> HtVec;
#else
    typedef std::vector<FsCtrl*> HtVec;
#endif
    HtVec* GetHtsPtr (void) { return &m_hts; }

    CoreType GetCoreType (void) const { return m_core_type; }
    CoreVersion GetCoreVersion (void) const { return m_core_version; }
    ArchClass GetArchClass (void) const { return m_arch_class; }
#if _UNIT_TEST
    FsCtrl_Interface* GetHtPtr (void) const { return m_hts[NC + 1]; }
#else
    FsCtrl* GetHtPtr (void) const { return m_hts[NC + 1]; }
#endif
    NcSRegFile* GetNcSrfPtr (void) const;
    SRegFile* GetSrfPtr (ContextId tc_id) const;
    GRegFile* GetGrfPtr (ContextId tc_id) const;
    uint32_t GetClId (void) const { return m_cl_id; }
    uint32_t GetPeId (void) const { return m_pe_id; }
    uint32_t GetBmId (void) const;
    Icache* GetIcPtr (void) const { return m_ic; }
    Peg* GetPegPtr (void) const { return m_peg; }
    CIntc1* GetIntc1Ptr (void) const { return m_intc1; }
    Misr* GetMisrPtr (void) const { return m_misr; }

    void SetIntc1(void);
#ifdef ENABLE_OSTM
    void SetOstm (uint32_t ch, PhysAddr addr, uint32_t intch, CIntc2* intc2);
    OstmIO* GetOstmPtr (uint32_t ch) const { return m_ostm[ch];}
#endif // #ifdef ENABLE_OSTM
    void SetCfgPtr (ModuleGuard* cfg);
    void SetGrgPtr (ModuleGuard* grg);
    ModuleGuard* GetGrgPtr (void);

    void SetGramArea (PhysAddr start_addr, PhysAddr end_addr);
    void SetLramArea (PhysAddr start_addr, PhysAddr end_addr);

    void SetHasFpu (bool has_fpu) { m_has_fpu = has_fpu; }
    bool HasFpu (void) const { return m_has_fpu; }
    void SetHasFxu (bool has_fxu) { m_has_fxu = has_fxu; }
    bool HasFxu (void) const { return m_has_fxu; }

    bool m_mpu_dmdp;
    void SetMpuDmdp (bool is_supressed) { m_mpu_dmdp = is_supressed; }
    bool GetMpuDmdp (void) const { return m_mpu_dmdp; }
    void SetNumMpuRegion (uint32_t num) { m_num_mpu_region = num; }
    uint32_t GetNumMpuRegion (void) const { return m_num_mpu_region; }
    void SetNumMpuBank (uint32_t num) { m_num_mpu_bank = num; }
    uint32_t GetNumMpuBank (void) const { return m_num_mpu_bank; }
    void SetHasMpu (bool has_mpu) { m_has_mpu = has_mpu; }
    bool HasMpu (void) const { return m_has_mpu; }

#ifdef ENABLE_PEID_POS_CTRL
    bool IsPeidPosG4x (void) const;
#endif // #ifdef ENABLE_PEID_POS_CTRL

    static const uint16_t INIT_SNOOZE_PERIOD = 32;
    uint16_t m_snooze_period;
    void SetSnoozePeriod (uint16_t period);
    uint16_t GetSnoozePeriod (void) const;

    void SetSupportTableRefInt (bool is_supported);
    bool IsTableRefIntSupported (void) const;

    void ReqIntByPeripheral (uint32_t channel);
    void ReqIntByIntc2 (uint32_t channel, IntReqInfoBase* intreq_info);
    void CancelIntByMask (uint32_t channel);
    void CancelIntByPeripheral (uint32_t channel);
    void CancelIntByIntc2 (uint32_t channel);
    void UpdateIntHighPriority (ContextId tcid);
    bool IsIntLevelDetect (uint32_t channel);
    bool IsIntReqInQueue (uint32_t channel);
    void SetIntc2Ptr (CIntc2* intc2);

    void SetExtPin (ExtPinType pin, uint32_t data);
    uint32_t GetExtPin (ExtPinType pin) const;

    typedef std::vector<ContextId> CtxtIdVec;

    void ChangeToHostMode (void);
    void ChangeToGuestMode (void);

    void UpdateHtActive (void) const;
    void UpdateRunnableByIntreq (void);
    void UpdatePrivLevel (void) const;
    void ClearBtb (void) const;

    Dbuf* GetDbufPtr (void) const { return m_dbuf; }
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    FlashCache* GetFlcPtr (void) const { return m_flc; }
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    FsBus* GetFsBus (void) const { return m_fsbus; }

    ProcElement (uint32_t pe_id, uint32_t cl_id, CoreType core_type, CoreVersion core_version,
                 ArchClass arch_class, FsBus* fsbus, Llsc* llsc);
    ~ProcElement ();

    /**
     * \fn void HardReset (void);
     * \brief Request to reset function and cycle of individual PE.
     *
     * This function is used by python command reset().
     *
     * @param -.
     * @return None
     */
    void HardReset (void);
    /**
     * \fn void FsReset (void);
     * \brief Request to reset individual PE function. There is no change in cycle count.
     *
     * This function is used by python command reset(self, *contextid)
     *                     and set_event (address, "pereset", context)
     *
     * @param -.
     * @return None
     */
    bool FsReset (void);

    bool IsIcEnabled (void) const;
    bool IsDcEnabled (void) const;

    void ShowPerfInfo (void) const;
    void ResetPerfInfo (void);

    void ReallocMpuRegion (void);
    void UpdateHvStatus (void);
    void ChangeMpuBank (uint32_t bank);

    bool IsHvEnabled (void) const { return m_hv_enabled; }
    bool IsHostMode (void) const { return m_is_host_mode; }
    bool IsGuestMode (void) const { return m_hv_enabled && (m_is_host_mode == false); }
    bool IsGuestModeWithGpid (uint32_t gpid) const;

    void NoticeCpuGuestModeToIntc (bool is_gm);
    void NoticeCpuGpidToIntc (uint32_t gpid);
    void UpdateSideband (void);

private:
    static const ByteSize ICACHE_SIZE_16K = 16384; // 16KB
    static const ByteSize ICACHE_SIZE_8K  = 8192; // 8KB
    static const ByteSize ICACHE_SIZE_NONE = 0;

    // PEからバスに対するリクエストキューの個数
    static const uint32_t NUM_NB_BUF_G3M10 = 4;
    static const uint32_t NUM_NB_BUF_G3M20 = 8;
    static const uint32_t NUM_NB_BUF_G3MH = 8; // AMU 8entry
    static const uint32_t NUM_NB_BUF_G3KH = 4;

    // NBバッファにあふれた場合にリクエストを滞留させるバッファの個数
    static const uint32_t NUM_LSU_BUF_G3M10 = 2;
    static const uint32_t NUM_LSU_BUF_G3M20 = 2;
    static const uint32_t NUM_LSU_BUF_G3MH = 6; // EA2*2, EB*2, EC*2
    static const uint32_t NUM_LSU_BUF_G3KH = 0;

    static const bool PEG_DBEN_INIT = true;
    static const bool PEG_SFEN_INIT = true;

    static const uint32_t DBUF_HIT_LATENCY_G3M  = 5;
    static const uint32_t DBUF_DEPTH_G3M  = 4;
    static const uint32_t DBUF_HIT_LATENCY_G3MH = 4;
    static const uint32_t DBUF_DEPTH_G3MH = 8;

    HtVec m_hts;

    const uint32_t  m_pe_id;
    const uint32_t  m_cl_id;
    const CoreType  m_core_type;
    const CoreVersion m_core_version;
    const ArchClass m_arch_class;
    FsBus*   m_fsbus;
    Icache*  m_ic;
    Peg*     m_peg;
    Misr*    m_misr;
    CIntc1*  m_intc1;
#ifdef ENABLE_DBG_BREAK
    DbgBreak* m_dbgbrk;
#endif // #ifdef ENABLE_DBG_BREAK
    Llsc*    m_llsc;
    CsLsuBuf* m_cs_lsu_buf;
    Dbuf*    m_dbuf;
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    FlashCache* m_flc;
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
#ifdef ENABLE_OSTM
    OstmIO* m_ostm[MAX_OSTM_NUM];
#endif // #ifdef ENABLE_OSTM

    NcSRegFile*    m_nc_srf;

    SRegFileSet* m_srfs;
    GRegFileSet* m_grfs;
    SRegBase* m_sreg_dummy;
    SRegBase* m_sreg_dummy_hv;

    bool m_has_fpu;
    bool m_has_fxu;
    uint32_t m_num_mpu_region;
    uint32_t m_num_mpu_bank;
    bool m_has_mpu;
    bool m_has_lockstep;

    uint32_t m_peid_extpin;
    uint32_t m_spid_extpin;
    uint32_t m_spidlist_extpin;
    uint32_t m_bmid_extpin;
    uint32_t m_mm_extpin;
    uint32_t m_rbase_extpin;
    uint32_t m_dv_extpin;
    uint32_t m_rint_extpin;
    uint32_t m_ma_extpin;
    uint32_t m_ichen_extpin;
    uint32_t m_has_mpm_dbit_extpin;
    uint32_t m_num_l1rlink_extpin;
    uint32_t m_size_l1r_extpin;
    MachineMode m_machine_mode;
    uint32_t m_has_hv;
    uint32_t m_hve_extpin;
    uint32_t m_dbgen_extpin;

    bool m_hv_enabled;
    bool m_is_host_mode;

    /**
     * \fn void PeBaseReset (void);
     * \brief Request to reset common modules of individual PE. There is no change in cycle count.
     *        This function will be used by HardRest or FsReset.
     *
     * @param -.
     * @return None
     */
    void PeBaseReset (void);
#ifdef SUPPORT_SC_HEAP
    bool IsGmGpidUpdated (void) const;
#endif // #ifdef SUPPORT_SC_HEAP
};
