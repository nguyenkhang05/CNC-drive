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

#include <map>
#include <vector>
#include <utility>
#include "./forest_common.h"

class ModuleGuardDummy;
class DbgArea;
class ErrorArea;
class FsBus;
class Csmem;
class CIntIci;
class CIntc2;
#ifdef ENABLE_TMU
class CTimer;
#endif // ENABLE_TMU
class FsCtrl;
class CIpir;
class Mecnt;
class ProcElement;
#include "proc_element_vector.h"
class Llsc;
class MemLatencyInfoBase;
#ifdef ENABLE_OSTM
class OstmIO;
#endif // #ifdef ENABLE_OSTM
#ifdef USE_SIM_IO
class Csimio;
#endif // USE_SIM_IO
#ifdef ENABLE_PERFCNT
class Cperfcnt;
#endif // ENABLE_PERFCNT
class Uart;
class CBar;

#ifdef SUPPORT_CUBESUITE
class CubeIoReg;
#endif // SUPPORT_CUBESUITE

#ifdef SUPPORT_SC_HEAP
class InvalidLram;
#endif // #ifdef SUPPORT_SC_HEAP

class PegCap;
class CrgCap;
class Crg;
class TRegModuleVc;

class Cforest {
public:
#ifdef ENABLE_PERFCNT
    static const PhysAddr PERFCNT_BASE;
#endif // ENABLE_PERFCNT

    class PeripheralAddr {
    public:
        PhysAddr   m_base_addr;
        PhysOffset m_offset_addr;
        bool       m_is_enabled;
        PeripheralAddr (PhysAddr base_addr, PhysOffset offset_addr, bool is_enabled)
          : m_base_addr (base_addr), m_offset_addr (offset_addr), m_is_enabled (is_enabled) {}
        ~PeripheralAddr () {}

        PhysAddr GetAddr (uint32_t peid) const
        { return m_base_addr + (peid + 1U) * m_offset_addr; }
        bool IsEnabled (void) const { return m_is_enabled; }
    };
    std::vector<PeripheralAddr> m_peri_addr;

    class GuardAddrKey {
    public:
        GuardModuleID m_gmid;
        uint32_t      m_id;
        bool operator < (const GuardAddrKey& rhs) const {
            if (m_gmid < rhs.m_gmid) {
                return true;
            }
            if (m_gmid > rhs.m_gmid) {
                return false;
            }
            if (m_id < rhs.m_id) {
                return true;
            }
            if (m_id > rhs.m_id) {
                return false;
            }
            return false;
        }
        GuardAddrKey (GuardModuleID gmid, uint32_t id)
          : m_gmid (gmid), m_id (id) {}
        ~GuardAddrKey () {}
    };
    typedef std::pair<PhysAddr, bool> GuardAddrVal;
    std::map<GuardAddrKey, GuardAddrVal> m_guard_addr;

    class CrgParam {
    public:
        uint32_t GetAddrWidth (void) const { return m_addr_width; }
        uint32_t GetNumOfCrg (void) const  { return m_num_of_ch_crg; }
        uint32_t GetNumOfCsg (void) const  { return m_num_of_ch_csg; }
        uint32_t GetKeycode (void) const   { return m_keycode; }
        void SetAddrWidth (uint32_t val)   { m_addr_width = val; }
        void SetNumOfCrg (uint32_t val)    { m_num_of_ch_crg = val; }
        void SetNumOfCsg (uint32_t val)    { m_num_of_ch_csg = val; }
        void SetKeycode (uint32_t val)     { m_keycode = val; }

        void SetParam (GuardParamID id, uint32_t val)
        {
            switch (id) {
            case GPID_CRG_ADDR_WIDTH: SetAddrWidth (val); break;
            case GPID_CRG_CRG_CH_NUM: SetNumOfCrg (val); break;
            case GPID_CRG_CSG_CH_NUM: SetNumOfCsg (val); break;
            case GPID_CRG_KEYCODE   : SetKeycode (val); break;
            default: break;
            }
        }
        uint32_t GetParam (GuardParamID id) const
        {
            uint32_t ret;
            switch (id) {
            case GPID_CRG_ADDR_WIDTH: ret = GetAddrWidth (); break;
            case GPID_CRG_CRG_CH_NUM: ret = GetNumOfCrg (); break;
            case GPID_CRG_CSG_CH_NUM: ret = GetNumOfCsg (); break;
            case GPID_CRG_KEYCODE   : ret = GetKeycode (); break;
            default: ret = 0; break;
            }
            return ret;
        }

        CrgParam () :
        // Use U2A parameters for initial value (to prevent invalid setting)
        m_addr_width (21), m_num_of_ch_crg (8), m_num_of_ch_csg (4), m_keycode (0xA5A5A500)
        {}
        ~CrgParam () {}
    private:
        uint32_t m_addr_width;    // GPID_CRG_ADDR_WIDTH = 0x0,
        uint32_t m_num_of_ch_crg; // GPID_CRG_CRG_CH_NUM = 0x1,
        uint32_t m_num_of_ch_csg; // GPID_CRG_CSG_CH_NUM = 0x2,
        uint32_t m_keycode;       // GPID_CRG_KEYCODE    = 0x3,
    };
    typedef std::pair<uint32_t, CrgParam> CrgParamPair;
    std::vector<CrgParamPair> m_crg_param_vec;

    typedef   std::vector<PegCap*> PegCapVec;
    PegCapVec m_pegcap;
    typedef   std::vector<CrgCap*> CrgCapVec;
    CrgCapVec m_crgcap;
    typedef   std::vector<Crg*> CrgVec;
    CrgVec    m_crg;

    std::map<GuardAddrKey, TRegModuleVc*> m_guard_cap_err;

    typedef std::vector<FsCtrl*> HtVec;
    // typedef std::vector<ProcElement*> PeVec;
    typedef std::vector<Csmem*> SmemVec;

    HtVec m_ht;
    PeVec m_pe;
    SmemVec  m_smem;

    uint32_t m_pe_num;

    ModuleGuardDummy* m_dummy_guard;
    DbgArea*  m_dbg_area;

    Llsc*    llsc;
    FsBus*   fsbus;
    Csmem*   smem0;
    CIntc2*  intc2;

#ifdef ENABLE_TMU
    CTimer* tmu0;
    CTimer* tmu1;
    CTimer* tmu2;
    CTimer* tmu3;
#endif // ENABLE_TMU

#ifdef ENABLE_OSTM
    std::vector<OstmIO*> m_ostm;
#endif // #ifdef ENABLE_OSTM

    CIpir* m_ipir;
    CBar* m_bar;

    Mecnt* m_mecnt;

#ifdef USE_SIM_IO
    Csimio* simio;
#endif // USE_SIM_IO
#ifdef ENABLE_PERFCNT
    std::vector<Cperfcnt*> m_perfcnt;
#endif // ENABLE_PERFCNT

#ifdef ENABLE_UART
    Uart* uart;
#endif // ENABLE_UART

#ifdef SUPPORT_CUBESUITE
    typedef std::pair<PhysAddr, Csmem*> CubeLiorInfo;
    std::vector<CubeLiorInfo> m_cube_lior;
    std::vector<CubeIoReg*> m_cube_iors;
#endif // SUPPORT_CUBESUITE

#if defined(SUPPORT_SC_HEAP)
    FsBus* m_peg_bus;
    FsBus* m_crg_bus;
#endif // defined(SUPPORT_SC_HEAP)

#ifdef SUPPORT_SC_HEAP
    InvalidLram* m_invalid_lram;
#endif // #ifdef SUPPORT_SC_HEAP

    ErrorArea* m_error_area;

    bool m_is_gen_peripherals;

    static const uint32_t LRAM_FETCH_LATENCY = 8;
    static const uint32_t FLASH_ROM_FETCH_LATENCY = 1U; // m_rom_waitが加算される
#ifdef CCG3M_DBUF
    static const uint32_t FLASH_ROM_LATENCY   = 5U; // m_rom_waitが加算される
#else // CCG3M_DBUF
    static const uint32_t FLASH_ROM_LATENCY   = 6U; // m_rom_waitが加算される
#endif // CCG3M_DBUF
    static const uint32_t FLASH_ROM_OUTSTAND  = 2U;
    static const uint32_t FLASH_ROM_REPEAT    = 1U; // m_rom_waitが加算される
    static const uint32_t LOCAL_RAM_LATENCY   = 2U;
    static const uint32_t LOCAL_RAM_FETCH_LATENCY = 8;
    static const uint32_t LOCAL_RAM_EXO_PE_LATENCY = 26U;
    static const uint32_t LOCAL_RAM_OUTSTAND  = 2U;
    static const uint32_t LOCAL_RAM_REPEAT    = 1U;
#ifdef CCG3M_GRAM
    static const uint32_t GLOBAL_RAM_LATENCY_R = 9U;
    static const uint32_t GLOBAL_RAM_LATENCY_W = 7U;
    static const MemLocationType GLOBAL_RAM_LOCATION = MEM_LOCATE_GLOBAL;
#else // CCG3M_GRAM
#ifdef GRAM_AS_LRAM
    static const uint32_t GLOBAL_RAM_LATENCY_R = 0U;
    static const uint32_t GLOBAL_RAM_LATENCY_W = 0U;
    static const MemLocationType GLOBAL_RAM_LOCATION = 1;
#else // GRAM_AS_LRAM
    static const uint32_t GLOBAL_RAM_LATENCY_R = 5U;
    static const uint32_t GLOBAL_RAM_LATENCY_W = 5U;
    static const MemLocationType GLOBAL_RAM_LOCATION = MEM_LOCATE_GLOBAL;
#endif // GRAM_AS_LRAM
#endif // CCG3M_GRAM
    static const uint32_t GLOBAL_RAM_OUTSTAND = 4U;
    static const uint32_t GLOBAL_RAM_REPEAT   = 2U;

    static const PhysOffset LOCAL_PERI_OFFSET = 0x4000;
    static const PhysOffset SIM_PERI_OFFSET   = -0x2000;

    void Reset (void);
    void GenPeripherals (void);

    void SetPe (uint32_t peid, uint32_t clid, CoreType core_type, CoreVersion core_version, uint32_t vc_num, uint32_t tc_num);
    ProcElement* GetPePtr (uint32_t peid) const;
    bool SetIntc2 (PhysAddr start_addr);

    MemLatencyInfoBase* CreateDefaultLatency (MemLocationType pe_location, MemLocationType cl_location);

    void CreateCforest (void);

    Cforest ();
    ~Cforest();
};
