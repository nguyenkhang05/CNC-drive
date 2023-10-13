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
#include <map>
#include "./forest_common.h"
#include "./intc_api.h"
#include "./module_vc.h"
#include "./treg.h"
#include "./intc2.h"
#if _INTC1_UNIT_TEST
#include "mock_proc_element.h"
#include "mock_fsbus.h"
#endif


class FsBus;

/*!
  @brief FNC register class.
 */
class DLL_EXPORT_IMPORT TRegFNC : public TRegCNTBase {
private:
    union {
        TRegData16 m_data;
        struct {
            RegData        :7;
            RegData        :1;
            RegData        :4;
            RegData m_fnrf :1;
            RegData        :2;
            RegData m_fnct :1; // G3M2.0ではFNC.FNCTが削除されるが、動作影響ないため対策しない
        } m_bit;
    };
    void ReplaceFNCWriteData (uint8_t* data, uint32_t index, uint32_t DM);

public:
    CoreVersion m_core_version;
    static const uint32_t FNC_MASK = 0x00009000U;
    static const uint8_t  FNCT_BYTE_MASK = 0x7FU;
    static const uint8_t  FNCT_FNRF_BYTE_MASK = 0x6FU;
    static const uint8_t  FNCT_BYTE_BIT  = 0x80U;
    static const uint8_t  FNRF_BYTE_MASK = 0xEFU;
    static const uint8_t  FNRF_BYTE_BIT  = 0x10U;

    void SetFNCT (RegData fnct) { m_bit.m_fnct = fnct & 0x01U; }
    void SetFNRF (RegData fnrf);

    RegData GetFNCT (void) const { return m_bit.m_fnct; }
    RegData GetFNRF (void) const { return m_bit.m_fnrf; }

    void Reset (void) {
        m_data.m_data16 = 0x0000U;
        TRegCNTBase::Reset();
    }

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return "FNC"; }

    TRegFNC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc);
};


/*!
  @brief FNBD register class.
 */
class DLL_EXPORT_IMPORT FNBD {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_peid :3;
            RegData        :29;
        } m_bit;
    };
    RegData dummy_gpid;
public:
    void SetData (RegData data) { m_data.m_data32 = data; }
    void SetPEID (RegData peid) { m_bit.m_peid = peid & PEID_MASK; }

    RegData GetData (void) const { return m_data.m_data32; }
    RegData GetPEID (void) const { return m_bit.m_peid; }

    TRegData32* GetTRegDataPtr (void) { return &m_data; }

    const char* GetName (void) const { return "FNBD"; }
    void SetGPID (RegData gpid) { dummy_gpid = 0;}
    RegData GetGPID (void) const { return 0; }

    FNBD (void): dummy_gpid(0)  {}
    ~FNBD (void) {}
};


/*!
  @brief FIC register class.
 */
class DLL_EXPORT_IMPORT TRegFIC : public TRegCNTBase {
private:
    union {
        TRegData16 m_data;
        struct {
            RegData        :5;
            RegData m_fiov :1;
            RegData        :2;
            RegData        :4;
            RegData m_firf :1;
            RegData        :2;
            RegData m_fict :1;
        } m_bit;
    };
    uint32_t m_channel;
    void ReplaceFICWriteData (uint8_t* data, uint32_t index, uint32_t DM);
    void ReplaceFIOVriteData (uint8_t* data, uint32_t index, uint32_t DM);

public:
    CoreVersion m_core_version;
    static const uint32_t FIC_MASK = 0x00009020U;    //G4MH20
    static const uint32_t FIC_MASK_G4MH10 = 0x00009000U;    //G4MH10
    static const uint8_t  FICT_BYTE_MASK = 0x7FU;
    static const uint8_t  FICT_BYTE_BIT  = 0x80U;
    static const uint8_t  FIRF_BYTE_MASK = 0xEFU;
    static const uint8_t  FICT_FIRF_BYTE_MASK = 0x6FU;
    static const uint8_t  FIRF_BYTE_BIT  = 0x10U;
    static const uint8_t  FICT_FIOV_MASK  = 0xDFU;    //G4MH20

    void SetFICT (RegData fict) { m_bit.m_fict = fict & 0x01U; }
    void SetFIRF (RegData firf);
    void SetFIOV (RegData fiov) { m_bit.m_fiov = fiov & 0x01U; }

    RegData GetFICT (void) const { return m_bit.m_fict; }
    RegData GetFIRF (void) const { return m_bit.m_firf; }
    RegData GetFIOV (void) const { return m_bit.m_fiov; }
    uint32_t GetChannel (void) const { return m_channel; }

    void Reset (void) {
        m_data.m_data16 = 0x0000U;
        TRegCNTBase::Reset();
    }

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return "FIC"; }

    TRegFIC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, uint32_t channel, CModuleVc* tgtvc);
};


/*!
  @brief FIBD register class.
 */
class DLL_EXPORT_IMPORT FIBD {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_peid :3;
            RegData        :5;
            RegData m_gpid :3;
            RegData        :4;
            RegData m_gm   :1;
            RegData        :16;
        } m_bit;
    };

public:
    void SetData (RegData data) { m_data.m_data32 = data; }
    void SetPEID (RegData peid) { m_bit.m_peid = peid & PEID_MASK; }
    void SetGM (RegData gm)     { m_bit.m_gm = gm & 0x01U; }
    void SetGPID (RegData gpid) { m_bit.m_gpid = gpid & 0x07U; }

    RegData GetData (void) const { return m_data.m_data32; }
    RegData GetPEID (void) const { return m_bit.m_peid; }
    RegData GetGM (void) const   { return m_bit.m_gm; }
    RegData GetGPID (void) const { return m_bit.m_gpid; }

    TRegData32* GetTRegDataPtr (void) { return &m_data; }

    const char* GetName (void) const { return "FIBD"; }

    FIBD (void) {}
    ~FIBD (void) {}
};


typedef TRegBND<FNBD> TRegFNBD;
typedef TRegBND<FIBD> TRegFIBD;


/*
 * @brief DBMK register class, due top update INTC1 spec.
 */
class DLL_EXPORT_IMPORT TRegDBMK : public TRegCNTBase {
private:
    union {
        TRegData16 m_data;
        struct {
            RegData m_ei :1;
            RegData m_fi :1;
            RegData m_fn :1;
            RegData      :13;
        } m_bit;
    };

public:
    static const uint32_t DBMK_MASK = 0x00000007U;

    void SetData (RegData data) { m_data.m_data16 = data & 0xFFFFU; }
    void SetFN   (RegData fn);
    void SetFI   (RegData fi);
    void SetEI   (RegData ei);

    RegData GetData (void) const { return m_data.m_data16; }
    RegData GetFN   (void) const { return m_bit.m_fn;      }
    RegData GetFI   (void) const { return m_bit.m_fi;      }
    RegData GetEI   (void) const { return m_bit.m_ei;      }

    void Reset (void) {
        m_data.m_data16 = 0x0000U;
        TRegCNTBase::Reset();
    }

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return "DBMK"; }

    TRegDBMK (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc);
};

/*
 * @brief IHVCFG register class in INTC G4MH2.0.
 */
class DLL_EXPORT_IMPORT TRegIHVCFG : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_ihve :1;
            RegData      :31;
        } m_bit;
    };
    CIntc1* m_intc1;
public:
    CoreVersion m_core_version;

    static const uint32_t IHVCFG_MASK = 0x00000001U;

    void SetData (RegData data) { m_data.m_data32 = data & 0xFFFFFFFFU; }
    void SetIHVE   (RegData ihve){ m_bit.m_ihve = ihve;}

    RegData GetData (void) const { return m_data.m_data32; }
    RegData GetIHVE   (void) const { return m_bit.m_ihve; }

    void Reset (void);

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return "IHVCFG"; }

    TRegIHVCFG (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc);
};


/*
 * @brief FIBG register class in INTC G4MH2.0.
 */
class DLL_EXPORT_IMPORT TRegFIBG : public TRegDataBase<TRegData32>{
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_bge :16;
            RegData      :16;
        } m_bit;
    };
    CIntc1* m_intc1;

public:
    CoreVersion m_core_version;
    static const uint32_t FIBG_MASK = 0x0000FFFFU;

    void SetData (RegData data) { m_data.m_data32 = data & 0xFFFFFFFFU; }
    void SetBGE   (RegData bge){ m_bit.m_bge = bge;}

    RegData GetData (void) const { return m_data.m_data32; }
    RegData GetBGE   (void) const { return m_bit.m_bge; }

    void Reset (void) {
        m_data.m_data32 = 0x0U;
    }

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return "FIBG"; }

    TRegFIBG (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc);
};

class DLL_EXPORT_IMPORT TRegIntc1 : public TRegModuleVc {
public:
    TRegIntc1 (uint32_t peid, CoreType core_type, CIntc1* intc1)
    : m_peid (peid), m_intc1 (intc1), m_core_type(core_type) { InitTRegMap (); }
    ~TRegIntc1 () { DelTRegMap (); }

    static const uint32_t N_EIC  = 32;
    static const uint32_t N_EEIC  = 32;
    static const uint32_t N_IMR  = 1;
    static const uint32_t N_G4MH20_EIBG  = 8;
    static const uint32_t N_EIBD = 32;
    static const uint32_t N_FIC = 16;
    static const uint32_t N_FIBD = 16;

    TRegEIC*  TrEIC  (PhysAddr addr) const { return static_cast<TRegEIC*>  (GetTrPtr (addr)); }
    TRegEEIC* TrEEIC (PhysAddr addr) const { return static_cast<TRegEEIC*> (GetTrPtr (addr)); }
    TRegEIBD* TrEIBD (PhysAddr addr) const { return static_cast<TRegEIBD*> (GetTrPtr (addr)); }
    TRegFNC*  TrFNC  (PhysAddr addr) const { return static_cast<TRegFNC*>  (GetTrPtr (addr)); }
    TRegFIC*  TrFIC  (PhysAddr addr) const { return static_cast<TRegFIC*>  (GetTrPtr (addr)); }
    TRegFNBD* TrFNBD (PhysAddr addr) const { return static_cast<TRegFNBD*> (GetTrPtr (addr)); }
    TRegFIBD* TrFIBD (PhysAddr addr) const { return static_cast<TRegFIBD*> (GetTrPtr (addr)); }
    TRegDBMK* TrDBMK (PhysAddr addr) const { return static_cast<TRegDBMK*> (GetTrPtr (addr)); }
    TRegIHVCFG* TrIHVCFG (PhysAddr addr) const { return static_cast<TRegIHVCFG*> (GetTrPtr (addr)); }
    TRegEIBG* TrEIBG (PhysAddr addr) const { return static_cast<TRegEIBG*> (GetTrPtr (addr)); }
    TRegFIBG* TrFIBG (PhysAddr addr) const { return static_cast<TRegFIBG*> (GetTrPtr (addr)); }


    PhysAddr TrEICAddr  (uint32_t channel) const;
    PhysAddr TrEEICAddr  (uint32_t channel) const;
    PhysAddr TrEIBDAddr (uint32_t channel) const;
    PhysAddr TrFNCAddr  (void) const;
    PhysAddr TrFICAddr  (uint32_t channel) const;
    PhysAddr TrFNBDAddr (void) const;
    PhysAddr TrFIBDAddr (uint32_t channel) const;
    PhysAddr TrDBMKAddr (void) const;
    PhysAddr TrIHVCFGAddr (void) const;
    PhysAddr TrEIBGAddr (uint32_t peid) const;
    PhysAddr TrFIBGAddr (void) const;


private:
    static const PhysAddr EIC_OFFSET  = 0x0000000000UL;
    static const PhysAddr EEIC_OFFSET  = 0x0000000200UL;
    static const PhysAddr IMR_OFFSET  = 0x00000000F0UL;
    static const PhysAddr EIBD_OFFSET = 0x0000000100UL;
    static const PhysAddr FNC_OFFSET  = 0x00000000A0UL;
    static const PhysAddr FIC_OFFSET  = 0x00000000C0UL;
    static const PhysAddr FNBD_OFFSET = 0x00000001A0UL;
    static const PhysAddr FIBD_OFFSET = 0x00000001C0UL;
    static const PhysAddr DBMK_OFFSET = 0x0000000080UL;
    static const PhysAddr IHVCFG_OFFSET = 0x00000002F0UL;
    static const PhysAddr EIBG_OFFSET = 0x0000000280UL;
    static const PhysAddr FIBG_OFFSET = 0x00000002C0UL;

    uint32_t m_peid;
    CIntc1* m_intc1;
    CoreType m_core_type;
    void InitTRegMap (void);
};

/*!
  @brief FENMI request information class.
 */
class DLL_EXPORT_IMPORT FenmiReqInfo : public IntReqInfoBase {
private:
    TRegFNC*  m_fnc;
    TRegFNBD* m_fnbd;
    TRegDBMK* m_dbmk;

public:
    RegData GetMask (void) const { return m_dbmk->GetFN (); }
    int32_t GetPriority (void) const { return IntPriorityInfo::FENMI_PRIORITY; }
    RegData GetPEID (void) const { return m_fnbd->GetPEID (); }
    bool IsLevelDetect (void) const { return m_fnc->GetFNCT () == 1; }

    ContextId GetTcId (void) const { return m_fnc->GetTcId (); }

    FenmiReqInfo (TRegFNC* fnc, TRegFNBD* fnbd, TRegDBMK* dbmk)
    : m_fnc (fnc), m_fnbd (fnbd), m_dbmk (dbmk){}
    ~FenmiReqInfo (void) {}
};


/*!
  @brief FEINT request information class.
 */
class DLL_EXPORT_IMPORT FeintReqInfo : public IntReqInfoBase {
private:
    TRegFIC*  m_fic;
    TRegFIBD* m_fibd;
    TRegDBMK* m_dbmk;
    //TRegIHVCFG* m_ihvcfg;
    TRegFIBG* m_fibg;

public:
    RegData GetMask (void) const { return (m_dbmk->GetFI()) ; }
    int32_t GetPriority (void) const;
    RegData GetPEID (void) const { return m_fibd->GetPEID (); }
    bool IsLevelDetect (void) const { return m_fic->GetFICT () == 1; }

    ContextId GetTcId (void) const { return m_fic->GetTcId (); }
    RegData GetGM (void) const { return m_fibd->GetGM (); }
    RegData GetGPID (void) const { return m_fibd->GetGPID (); }
    RegData GetBGE (void) const {return m_fibg->GetBGE();}

    FeintReqInfo (TRegFIC* fic, TRegFIBD* fibd, TRegDBMK* dbmk, TRegFIBG* fibg)
    : m_fic (fic), m_fibd (fibd), m_dbmk (dbmk), m_fibg(fibg){}
    ~FeintReqInfo (void) {}
};


class DLL_EXPORT_IMPORT CIntc1 : public CModuleVc, public CIntcBase, public Intc1Api
{
#if defined (_INTC1_UNIT_TEST)
public:
#else
protected:
#endif
    // EIINTのチャネル番号をIntReqMapのキー番号として使用
    // FENMIのキー番号は0x1000, FEINTは0x1001に設定
    static const uint32_t FENMI_CHANNEL = 0x1000U;
    static const uint32_t FEINT_CHANNEL = 0x1001U;
    static const uint32_t INIT_FEINT_CHANNEL = 16;

#if !(_INTC1_UNIT_TEST)
    ProcElement* m_pe;
#else
    MockProcElement* m_pe;
#endif
    uint32_t m_peid;

#if !(_INTC1_UNIT_TEST)
    FsBus* m_fsbus;
#else
    MockFsBus* m_fsbus;
#endif
    CIntc2* m_intc2;
    uint32_t m_req_feint_channel;

    typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
    IntReqMap m_intc1_queue;

    typedef std::map<ContextId, IntPriorityInfo*> IntHighPriorityMap;
    IntHighPriorityMap m_intc1_priority;

    void ReqInt (uint32_t channel, IntReqInfoBase* intreq_info);

    EiintReqInfo* CreateEiintReqInfo (uint32_t channel);
    FenmiReqInfo* CreateFenmiReqInfo (void);
    FeintReqInfo* CreateFeintReqInfo (uint32_t channel);
    void ClearIntReqQueue (void);
    void StopIntc1CyclicHandler (void);

    // add Yoshinaga for SC-HEAP
    bool IsIntc1PriorityEmpty(void) { return(m_intc1_priority.empty()); }
    void Intc1PriorityInsert(ContextId id) {
        m_intc1_priority.insert(IntHighPriorityMap::value_type(id, new IntPriorityInfo()));
    }
    unsigned int GetQueueSize(void) { return(m_intc1_queue.size()); }
    IntReqInfoBase* GetQueueReqInfo(uint32_t channel) {
        IntReqMap::iterator it = m_intc1_queue.find(channel);
        if (it == m_intc1_queue.end()) {
            return(0);
        } else {
            return(it->second);
        }
    }
    bool m_is_cpu_gm;
    uint32_t m_cpu_gpid;
    bool m_pending_state;
    bool m_external_EIINT_req;
    bool m_external_FEINT_req;
    uint32_t m_cpu_ihve;
    uint32_t m_cpu_has_hv;
    uint32_t m_ihvcfg_ihve;

    //PE or ForestUtil will set these values
    uint32_t cpu_hve;
    uint32_t cpu_has_hv;

public:
    CoreType m_core_type;
    CoreVersion m_core_version;
    static const PhysAddr INTC1_REG_SIZE = 0x00000400;    //g4mh20
    static const PhysAddr INTC1_REG_ADDR_MASK = INTC1_REG_SIZE - 1;
    static const uint32_t NUM_FEINT_CHANNEL = 16;
    static const uint32_t NUM_FENMI_CHANNEL = 1;
    static const uint32_t NUM_EIINT_CHANNEL = 32;

    void CyclicHandler (void);
#if _INTC1_UNIT_TEST
    CIntc1 (MockProcElement* pe, uint32_t pe_id, MockFsBus* fsbus, CoreType core_type, CoreVersion core_version);
#else
    CIntc1 (ProcElement* pe, uint32_t pe_id, FsBus* fsbus, CoreType core_type, CoreVersion core_version);
#endif
    ~CIntc1 ();

    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);
    void CancelEiintByMask(uint32_t channel);

    IntErrorCode ReqIntByPeripheral (uint32_t channel);
    IntErrorCode ReqIntByFenmi (void);
    IntErrorCode ReqIntByFeint (uint32_t channel);
    void ReqIntByIntc2 (uint32_t channel, IntReqInfoBase* intreq_info);
    static void ResIntByDummy (uint32_t channel, CIntc1* intc1) { return; }
    static void ResFenmiByDummy (CIntc1* intc1) { return; }
    static void ResFeintByDummy (uint32_t channel, CIntc1* intc1) {}
    static void ResIntFromCpu (uint32_t channel, CIntc1* intc1);
    static void ResFenmiFromCpu (CIntc1* intc1);
    static void ResFeintFromCpu (uint32_t channel, CIntc1* intc1);
    void SetIntc2Ptr (CIntc2* intc2);
    void UpdateHighPriority (ContextId tcid);
    void ReqIntByEIC (uint32_t channel);
    void CancelInt (uint32_t channel);
    void CancelIntByMask (uint32_t channel);
    void CancelIntByPeripheral (uint32_t channel);
    bool IsIntLevelDetect (uint32_t channel);
    bool IsIntReqInQueue (uint32_t channel);
    void ReqFenmiByFNRF (void);
    void CancelFenmiByFNRF (void);
    void CancelFenmiByMask (void);
    void ReqFeintByFIRF (uint32_t channel);
    void CancelFeintByFIRF (uint32_t channel);
    void CancelFeintByMask (uint32_t channel);
    TRegEIC*  GetEICPtr (uint32_t channel) const;
    TRegEEIC*  GetEEICPtr (uint32_t channel) const;
    TRegEIBD* GetEIBDPtr (uint32_t channel) const;
    TRegFNC*  GetFNCPtr (void) const;
    TRegFIC*  GetFICPtr (uint32_t channel) const;
    TRegFNBD* GetFNBDPtr (void) const;
    TRegFIBD* GetFIBDPtr (uint32_t channel) const;
    TRegDBMK* GetDBMKPtr (void) const;
    TRegIHVCFG* GetIHVCFGPtr (void) const;
    TRegEIBG* GetEIBGPtr (void) const;
    TRegFIBG* GetFIBGPtr (void) const;

    static bool IsIntc1Channel (uint32_t ch) { return ch < TRegIntc1::N_EIC; }
    virtual void ReqEiintToCpu (uint32_t priority, uint32_t channel, bool eitb,
                                bool bindmode, uint32_t gpid, uint32_t bgpr,
                                void (*resfunc)(uint32_t channel, CIntc1* intc1));
    virtual void ReqEiintToCpu (uint32_t priority, uint32_t channel, bool eitb,
                                void (*resfunc)(uint32_t channel, CIntc1* intc1));
    virtual void ReqFenmiToCpu (void (*resfunc)(CIntc1* intc1));
    virtual void ReqFeintToCpu (uint32_t channel,
                                                  void (*resfunc)(uint32_t channel, CIntc1* intc1));
    virtual void ReqFeintToCpu (uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bge,
                                                  void (*resfunc)(uint32_t channel, CIntc1* intc1));
    virtual void CancelIntToCpu (uint32_t channel);
    virtual void CancelIntToCpu (uint32_t channel, bool is_called_cyclic_handler); // for SC-HEAP (added by Yoshinaga)
    virtual void CancelFenmiToCpu (void);
    virtual void CancelFeintToCpu (void);
    virtual void CancelFeintToCpu (bool is_called_cyclic_handler); // for SC-HEAP (added by Yoshinaga)
    virtual void CancelFeintToCpu (uint32_t channel, bool is_called_cyclic_handler); // for SC-HEAP (added by Yoshinaga)
    virtual void StartCyclicHandler (void);
    virtual void StopCyclicHandler (CIntc1* intc1);
    virtual void InitPriorityInfo (void);
    virtual void UpdateInterruptPort (uint32_t channel) { return; }

    // virtual functions will be overridden by SC-HEAP.

    void SetCpuGuestMode(bool is_gm){m_is_cpu_gm = is_gm;}
    void SetCpuGpid(uint32_t gpid){m_cpu_gpid = gpid;}
    bool GetCpuGuestMode(void){return m_is_cpu_gm;}
    uint32_t GetCpuGpid(void){return m_cpu_gpid;}
    bool UpdateOverwriteBit_EIC (uint32_t channel);
    bool UpdateOverwriteBit_FIC (uint32_t channel);
    bool IsIntReqMasked(IntReqInfoBase * intreq_info, uint32_t channel, int32_t priority);
    void UpdateHighPriorityFromCpuMode (ContextId tcid);
    CoreVersion GetCoreVersion (void) const {return m_core_version;}
    bool GetExternalEIINTreq(void) {return m_external_EIINT_req;}
    void SetExternalEIINTreq(bool req) {m_external_EIINT_req = req;}
    bool GetExternalFEINTreq(void) {return m_external_FEINT_req;}
    void SetExternalFEINTreq(bool req) {m_external_FEINT_req = req;}
    uint32_t GetIHVEFromIHVCFG (void) const { return m_ihvcfg_ihve;}
    void SetIHVEFromIHVCFG (uint32_t ihve){ m_ihvcfg_ihve = ihve;}
    void SetIHVEFromCPU (uint32_t ihve){ m_cpu_ihve = ihve;}
    uint32_t GetIHVEFromCPU (void){ return m_cpu_ihve;}
    uint32_t GetHasHVromCPU (void){ return m_cpu_has_hv;}

    //return 1: has HV hardware /0: doesn't have HV hardware
    uint32_t GetHasHVExtPin(void){ return cpu_has_hv;}

    //value of HVCFG.HVE
    uint32_t GetHVEExtPin(void) {return cpu_hve;}

    void SetHasHVExtPin(uint32_t has_hv){ cpu_has_hv = has_hv;}
    void SetHVEExtPin(uint32_t hve) {cpu_hve = hve;}
};
