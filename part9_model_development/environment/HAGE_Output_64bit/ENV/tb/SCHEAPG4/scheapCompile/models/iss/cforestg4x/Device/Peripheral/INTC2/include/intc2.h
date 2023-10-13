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
#include "./sideband.h"
#if _INTC1_UNIT_TEST
#include "mock_fsbus.h"
#endif


#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
class ProcElement;
#else
#include "proc_element_vector.h"
#endif
class CIntc1;
#if defined(_INTC2_UNIT_TEST)
class MockCIntc1;
#endif
class CIntc2;
class FsBus;
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
class MockFsBus;
#endif
class TRegEIC;
class TRegFNC;
class TRegFIC;
class TRegDBMK;
class MockTRegDBMK;
class TRegEIBG;
class MockTRegEIBG;
class CIntcBase;

/*!
 * @brief INTC2 type.
 */
enum Intc2ImplType {
    INTC2_STANDARD,
    INTC2_G4MH20,
};


/*!
 * @brief INTC bit register base class.
 */
template <class TR>
class DLL_EXPORT_IMPORT TRegIntcBase : public TRegDataBase<TR> {
private:
    virtual bool IsRegWriteEnable (SideBand sideband) const = 0;

public:
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    TRegIntcBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
        : TRegDataBase<TR> (reg_size, min_access_size, reg_addr, reg_mask) {}
    TRegIntcBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr)
        : TRegDataBase<TR> (reg_size, min_access_size, reg_addr) {}
};


/*!
 * @brief EIC,FNC,FIC register base class.
 */
class DLL_EXPORT_IMPORT TRegCNTBase : public TRegIntcBase<TRegData16> {
#if !defined(_INTC2_UNIT_TEST)
protected:
    CIntc1*  m_intc1;
#else
public:
    MockCIntc1*  m_mock_intc1;
#endif
    uint32_t m_bind_et;
    uint32_t m_bind_tcid;
    bool IsRegWriteEnable (SideBand sideband) const;
#if !defined(_INTC2_UNIT_TEST)
    void Reset (void) {
        m_bind_et = 0x0U;
        m_bind_tcid = 0x0U;
    }
#else
public:
    void Reset (void) {
        m_bind_et = 0x0U;
        m_bind_tcid = 0x0U;
    }
#endif

public:
    void SetBindET (uint32_t et) { m_bind_et = et; }
    void SetBindTCID (uint32_t tcid) { m_bind_tcid = tcid; }
    ContextId GetTcId (void) const {
        return (m_bind_et == 0) ? NC : static_cast<ContextId> (m_bind_tcid);
    }

    TRegCNTBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
        : TRegIntcBase<TRegData16> (reg_size, min_access_size, reg_addr, reg_mask),
          m_bind_et (0), m_bind_tcid (0) {}
};


/*!
 * @brief EIC register class.
 */
class DLL_EXPORT_IMPORT TRegEIC : public TRegCNTBase {
#if defined(_INTC2_UNIT_TEST)
public:
#else
private:
#endif
    union {
        TRegData16 m_data;
        struct {
            RegData m_eip  :4;
            RegData        :1;
            RegData m_eiov :1;
            RegData m_eitb :1;
            RegData m_eimk :1;
            RegData        :4;
            RegData m_eirf :1;
            RegData        :2;
            RegData m_eict :1;
        } m_bit;
    };

    uint32_t m_channel;
    CIntc2*  m_intc2;
    CIntc1*  m_intc1;
    uint32_t m_bind_cst;
    uint32_t m_bind_peid;
    uint32_t m_init_peid;
    CoreVersion m_core_version;;

    void ReplaceEICWriteData (uint8_t* data, uint32_t index);
    void ReplaceEIOVWriteData (uint8_t* data, uint32_t index);

public:
    void UpdateEICHighPriority (void);
    static const uint32_t EIC_MASK_G4MH20 = 0x000090EFU;        //G4MH20
    static const uint32_t EIC_MASK_G4MH10 = 0x000090CFU;        //G4MH10

    static const uint8_t  EICT_BYTE_MASK = 0x7FU;
    static const uint8_t  EICT_BYTE_BIT  = 0x80U;
    static const uint8_t  EIRF_BYTE_MASK = 0xEFU;
    static const uint8_t  EIRF_BYTE_BIT  = 0x10U;
    static const uint8_t  EICT_EIOV_MASK  = 0xDFU;    //G4MH20

    void SetEICT (RegData eict) { m_bit.m_eict = eict & 0x01U; }
    void SetEIRF (RegData eirf);
    void SetEIMK (RegData eimk);
    void SetEITB (RegData eitb) { m_bit.m_eitb = eitb & 0x01U; }
    void SetEIP  (RegData eip_l) ;
    void SetEIOV (RegData eiov) { m_bit.m_eiov = eiov & 0x01U;}

    RegData GetEICT (void) const { return m_bit.m_eict; }
    RegData GetEIRF (void) const { return m_bit.m_eirf; }
    RegData GetEIMK (void) const { return m_bit.m_eimk; }
    RegData GetEITB (void) const { return m_bit.m_eitb; }
    RegData GetEIP  (void) const { return (m_bit.m_eip); }
    RegData GetEIOV (void) const { return m_bit.m_eiov; }

    void Reset (void) {
        if ((m_core_version >= CORE_VER_20)) {
            m_data.m_data16 = 0x008FU;
        }else{
            m_data.m_data16 = 0x008FU;
        }
        m_bind_cst  = 0x0U;
        m_bind_peid = m_init_peid;
        TRegCNTBase::Reset();
    }

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    const char* GetName (void) const { return "EIC"; }

    void SetBindCST (uint32_t cst) { m_bind_cst = cst; }
    void SetBindPEID (uint32_t peid) { m_bind_peid = peid; }

    TRegEIC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
             uint32_t channel, uint32_t peid, CModuleVc* tgtvc);
};

/*!
 * @brief EEIC register class.
 */
class DLL_EXPORT_IMPORT TRegEEIC : public TRegDataBase<TRegData32> {
#if defined(_INTC2_UNIT_TEST)
public:
#else
private:
#endif
    union {
        TRegData32 m_data;
        struct {
            RegData m_eip  :6;
            RegData        :2;
            RegData        :7;
            RegData m_eiov :1;
            RegData        :6;
            RegData m_eitb :1;
            RegData m_eimk :1;
            RegData        :4;
            RegData m_eirf :1;
            RegData        :2;
            RegData m_eict :1;
        } m_bit;
    };
#if !defined(_INTC2_UNIT_TEST)
    CIntc1*  m_intc1;
#else
    MockCIntc1*  m_mock_intc1;
#endif
    uint32_t m_channel;
    CIntc2*  m_intc2;
    uint32_t m_bind_cst;
    uint32_t m_bind_peid;
    uint32_t m_init_peid;
    uint32_t m_bind_et;
    uint32_t m_bind_tcid;
    CoreVersion m_core_version;;

    void ReplaceEICWriteData (uint8_t* data, uint32_t index);
    void ReplaceEIOVWriteData (uint8_t* data, uint32_t index);

public:
    void UpdateEEICHighPriority (void);
    static const uint32_t EEIC_MASK = 0x90C0803FU;        //G4MH20
    static const uint8_t  EICT_BYTE_MASK = 0x7FU;
    static const uint8_t  EICT_BYTE_BIT  = 0x80U;
    static const uint8_t  EIRF_BYTE_MASK = 0xEFU;
    static const uint8_t  EIRF_BYTE_BIT  = 0x10U;
    static const uint8_t  EICT_EIOV_MASK  = 0x7FU;    //G4MH20

    void SetEICT (RegData eict) { m_bit.m_eict = eict & 0x01U; }
    void SetEIRF (RegData eirf);
    void SetEIMK (RegData eimk);
    void SetEITB (RegData eitb) { m_bit.m_eitb = eitb & 0x01U; }
    void SetEIP  (RegData eip);
    void SetEIOV (RegData eiov) { m_bit.m_eiov = eiov & 0x01U;}

    RegData GetEICT (void) const { return m_bit.m_eict; }
    RegData GetEIRF (void) const { return m_bit.m_eirf; }
    RegData GetEIMK (void) const { return m_bit.m_eimk; }
    RegData GetEITB (void) const { return m_bit.m_eitb; }
    RegData GetEIP  (void) const { return m_bit.m_eip; }
    RegData GetEIOV (void) const { return m_bit.m_eiov; }
    RegData GetData (void) const { return m_data.m_data32; }
    void Reset (void) {
        m_data.m_data32 = 0x0080000FU;
        m_bind_cst  = 0x0U;
        m_bind_tcid  = 0x0U;
        m_bind_et  = 0x0U;
        m_bind_peid = m_init_peid;
    }

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    const char* GetName (void) const { return "EEIC"; }

    TRegEEIC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
             uint32_t channel, uint32_t peid, CModuleVc* tgtvc);
};

/*
 * @brief EIBG register in INTC G4MH2.0.
 */
class DLL_EXPORT_IMPORT TRegEIBG : public TRegDataBase<TRegData32>{
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_bgpr :6;
            RegData      :24;
        } m_bit;
    };
    CIntcBase* m_intc;
    uint32_t   m_regid;
public:
    static const uint32_t EIBG_MASK_INTC1 = 0x0000003FU;
    static const uint32_t EIBG_MASK_INTC2 = 0x0000003FU;

    void SetData (RegData data) { m_data.m_data32 = data & 0xFFFFFFFFU; }
    void SetBGPR   (RegData bgpr){ m_bit.m_bgpr = bgpr;}

    RegData GetData (void) const { return m_data.m_data32; }
    RegData GetBGPR   (void) const { return m_bit.m_bgpr; }

    void Reset (void) {
        m_data.m_data32 = 0x0U;
    }

    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return "EIBG"; }

    TRegEIBG (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,TRegMask reg_mask, uint32_t regid, CModuleVc* tgtvc);
};

/*!
 * @brief IMR register class.
 */
class DLL_EXPORT_IMPORT TRegIMR : public TRegDataBase<TRegData32> {
#if defined(_INTC2_UNIT_TEST)
public:
    uint32_t   m_regid;
    CIntcBase* m_intc;
#else
private:
    uint32_t   m_regid;
    CIntcBase* m_intc;
#endif

public:
    static const uint32_t IMR_MASK = 0xFFFFFFFFU;

    void Reset (void) {}
    bool IsRegWriteEnable (SideBand sideband) const;
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return "IMR"; }

    TRegIMR (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
             uint32_t regid, CModuleVc* tgtvc);
};


/*!
 * @brief EIBD register class.
 */
class DLL_EXPORT_IMPORT EIBD {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_peid :3;
            RegData        :5;
            RegData m_gpid :3;
            RegData        :4;
            RegData m_gm   :1;
            RegData        :8;
            RegData m_bcp  :2;
            RegData        :5;
            RegData m_cst  :1;
        } m_bit;
    };

public:
    void SetData (RegData data) { m_data.m_data32 = data; }
    void SetCST (RegData cst)   { m_bit.m_cst = cst & 0x01U; }
    void SetBCP (RegData bcp)   { m_bit.m_bcp = bcp & 0x03U; }
    void SetPEID (RegData peid) { m_bit.m_peid = peid & PEID_MASK;}
    void SetGM (RegData gm)     { m_bit.m_gm = gm & 0x01U; }
    void SetGPID (RegData gpid) { m_bit.m_gpid = gpid & 0x07U; }

    RegData GetData (void) const { return m_data.m_data32; }
    RegData GetCST (void) const  { return m_bit.m_cst; }
    RegData GetBCP (void) const  { return m_bit.m_bcp; }
    RegData GetPEID (void) const { return m_bit.m_peid; }
    RegData GetGM (void) const   { return m_bit.m_gm; }
    RegData GetGPID (void) const { return m_bit.m_gpid; }

    TRegData32* GetTRegDataPtr (void) { return &m_data; }

    const char* GetName (void) const { return "EIBD"; }

    EIBD (void) {}
    ~EIBD (void) {}
};


/*!
 * @brief EIBD,FNBD,FIBD register template class.
*/
template <class TR>
class DLL_EXPORT_IMPORT TRegBND : public TRegIntcBase<TRegData32> {
#if defined(_INTC2_UNIT_TEST)
public:
    TR m_tr;
#else
private:
    TR m_tr;
#endif
    uint32_t m_channel;
    uint32_t m_init_peid;
    CIntcBase* m_intc;
    Intc2ImplType m_type;

#if !defined(_INTC1_UNIT_TEST)
    bool IsRegWriteEnable (SideBand sideband) const;
#endif
    bool IsIntc1EIBD (void) const;
    bool IsIntc2EIBD (void) const;
    bool IsIntLevelDetect (uint32_t m_channel);


public:
#if defined(_INTC1_UNIT_TEST)
    bool IsRegWriteEnable (SideBand sideband) const;
#endif
    CoreVersion m_core_version;
    static const uint32_t INTC1_EIBD_MASK = 0x0000BF00U;
    static const uint32_t INTC2_EIBD_MASK = 0x8707BF07U;

    static const uint32_t INTC1_EIBD_MASK_G3KH = 0x00000007U;
    static const uint32_t INTC2_EIBD_MASK_G3KH = 0x00070007U;

    static const uint32_t INTC1_EIBD_MASK_G3MH = 0x00008000U;
    static const uint32_t INTC2_EIBD_MASK_G3MH = 0x83038007U;

    static const uint32_t INTC1_EIBD_MASK_G4MH20 = 0x00008700U;        //G4MH20
    static const uint32_t INTC1_EIBD_MASK_G4MH10 = 0x00000007U;        //G4MH10
    static const uint32_t INTC2_EIBD_MASK_G4MH = 0x83000007U;
    static const uint32_t INTC2_EIBD_MASK_G4MH20 = 0x83008707U;
    static const uint32_t FBD_MASK = 0x00000000U;

    void SetCST  (RegData cst)  { m_tr.SetCST (cst); }
    void SetBCP  (RegData bcp)  { m_tr.SetBCP (bcp); }
    void SetPEID (RegData peid) { m_tr.SetPEID (peid); }
    void SetGM (RegData gm)     { m_tr.SetGM (gm); }
    void SetGPID (RegData gpid) { m_tr.SetGPID (gpid); }

    RegData GetCST  (void) const { return m_tr.GetCST (); }
    RegData GetBCP  (void) const { return m_tr.GetBCP (); }
    RegData GetPEID (void) const { return m_tr.GetPEID (); }
    RegData GetGM (void) const   { return m_tr.GetGM (); }
    RegData GetGPID (void) const { return m_tr.GetGPID (); }

    void Reset (void) {
    }

    void WriteBodyByte (uint32_t pos, uint8_t data);
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    bool IsRegReadEnable (SideBand sideband) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;
    const char* GetName (void) const { return m_tr.GetName(); }

    void SetBind (void);

    TRegBND (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
             uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type);
    TRegBND (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
             TRegMask reg_mask, uint32_t channel, uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type);
};


typedef TRegBND<EIBD> TRegEIBD;

/*!
 * @brief INTC base class.
 */
class DLL_EXPORT_IMPORT CIntcBase {
protected:
    TRegModuleVc* m_trfs;

public:
    virtual TRegEIC* GetEICPtr (uint32_t channel) const = 0;
    virtual TRegEEIC* GetEEICPtr (uint32_t channel) const = 0;
    virtual TRegFNC* GetFNCPtr (void) const = 0;
    virtual TRegFIC* GetFICPtr (uint32_t channel) const = 0;
    virtual TRegEIBD* GetEIBDPtr (uint32_t channel) const = 0;
    virtual CoreVersion GetCoreVersion (void) const = 0;
    virtual uint32_t GetIHVEFromIHVCFG (void) const = 0;
    virtual ~CIntcBase () {}
};


/*!
 * @brief INTC2 register class.
 */
class DLL_EXPORT_IMPORT TRegIntc2 : public TRegModuleVc {
public:
    static const uint32_t N_G4MH_EIC  = 2048;
    static const uint32_t N_G4MH_EEIC  = 2048;
    static const uint32_t N_G4MH_IMR  = N_G4MH_EIC / 32;
    static const uint32_t N_G4MH_EIBD = 2048;
    static const uint32_t N_G4MH20_EIBG = 8;
    static const uint32_t N_EIC = N_G4MH_EIC;
    static const uint32_t N_EEIC = N_G4MH_EEIC;
private:
    static const PhysAddr EIC_OFFSET  = 0x00000000000UL;
    static const PhysAddr EEIC_OFFSET = 0x0000004000UL;
    static const PhysAddr IMR_OFFSET  = 0x0000001000UL;
    static const PhysAddr EIBD_OFFSET = 0x0000002000UL;
    static const PhysAddr EIBG_OFFSET = 0x0000001FE0UL;

public:
    ~TRegIntc2 () { DelTRegMap (); }

    TRegEIC*  TrEIC  (PhysAddr addr) const { return static_cast<TRegEIC*> (GetTrPtr (addr)); }
    TRegEEIC*  TrEEIC  (PhysAddr addr) const {return static_cast<TRegEEIC*> (GetTrPtr (addr)); }
    TRegEIBD* TrEIBD (PhysAddr addr) const { return static_cast<TRegEIBD*> (GetTrPtr (addr)); }
    TRegEIBG* TrEIBG (PhysAddr addr) const { return static_cast<TRegEIBG*> (GetTrPtr (addr)); }

    PhysAddr TrEICAddr (uint32_t channel) const;
    PhysAddr TrEEICAddr (uint32_t channel) const;
    PhysAddr TrEIBDAddr (uint32_t channel) const;
    PhysAddr TrEIBGAddr (uint32_t peid) const;
#if defined(_INTC2_UNIT_TEST)
public:
#else
private:
#endif

    uint32_t m_peid;
    CIntc2* m_intc2;
    void InitTRegMap (void);

public:
    //m_peidには初期値としてMainPEIDを代入
    //CForestではMainPEIDを1とする
    explicit TRegIntc2 (CIntc2* intc2) : m_peid (0), m_intc2 (intc2) { InitTRegMap (); }
};

/*!
 * @brief Interrupt request information class.
 */
class DLL_EXPORT_IMPORT IntReqInfoBase {
public:
#if defined (COMP_RUNNER)
    virtual RegData GetEIRF (void) const { return 0; }
#endif // defined(COMP_RUNNER)
    virtual bool IsLevelDetect (void) const = 0;
    virtual RegData GetMask (void) const = 0;
    virtual bool    GetEITB (void) const { return 0; }
    virtual int32_t GetPriority (void) const = 0;
    virtual RegData GetCST (void) const { return 0; }
    virtual RegData GetBCP (void) const { return 0; }
    virtual RegData GetPEID (void) const = 0;
    virtual RegData GetGM (void) const { return 0; }
    virtual RegData GetGPID (void) const { return 0; }
    virtual RegData GetBGPR (void) const { return 0; };
    virtual RegData GetBGE (void) const { return 0; }

    virtual ContextId GetTcId (void) const = 0;

    IntReqInfoBase (void) {}
    virtual ~IntReqInfoBase (void) {}
};

/*!
 * @brief EIINT request information class.
 */
class DLL_EXPORT_IMPORT EiintReqInfo : public IntReqInfoBase {
private:
    TRegEIC*  m_eic;
    TRegEEIC*  m_eeic;
    TRegEIBD* m_eibd;
#if defined(_INTC2_UNIT_TEST)
    MockTRegDBMK* m_mock_dbmk;
//    MockTRegEIBG* m_mock_eibg;
#else
    TRegDBMK* m_dbmk;
#endif
    TRegEIBG* m_eibg;

public:
#if defined (COMP_RUNNER)
    virtual RegData GetEIRF (void) const { return m_eic->GetEIRF ();}
#endif // defined(COMP_RUNNER)
    bool IsLevelDetect (void) const { return m_eic->GetEICT () == 1;}
    RegData GetMask (void) const;
    bool    GetEITB (void) const { return m_eic->GetEITB () == 1; }
    int32_t GetPriority (void) const { return static_cast<int32_t> (m_eeic->GetEIP ());}
    RegData GetCST (void) const { return m_eibd->GetCST ();}
    RegData GetBCP (void) const { return m_eibd->GetBCP ();}
    RegData GetPEID (void) const { return m_eibd->GetPEID();}
    RegData GetGM (void) const { return m_eibd->GetGM();}
    RegData GetGPID (void) const { return m_eibd->GetGPID();}
    RegData GetBGPR (void) const;

#if defined(_INTC2_UNIT_TEST)
    void SetDBMK (MockTRegDBMK* mock_dbmk) { m_mock_dbmk = mock_dbmk;}
#else
    void SetDBMK (TRegDBMK* dbmk) { m_dbmk = dbmk;}
#endif

    ContextId GetTcId (void) const { return m_eic->GetTcId ();}

#if defined(_INTC2_UNIT_TEST)
    EiintReqInfo (TRegEIC* eic, TRegEEIC* eeic, TRegEIBD* eibd, MockTRegDBMK* mock_dbmk, TRegEIBG* eibg) : m_eic (eic), m_eeic (eeic), m_eibd (eibd), m_mock_dbmk (mock_dbmk), m_eibg(eibg){}
#else
    EiintReqInfo (TRegEIC* eic, TRegEEIC* eeic, TRegEIBD* eibd, TRegDBMK* dbmk, TRegEIBG* eibg) : m_eic (eic), m_eeic (eeic), m_eibd (eibd), m_dbmk(dbmk), m_eibg(eibg){}
#endif
    ~EiintReqInfo (void) {}
};


/*!
 * @brief Interrupt priority information class.
 */
class DLL_EXPORT_IMPORT IntPriorityInfo {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
public:
#else
private:
#endif
    uint32_t m_channel;
    uint32_t m_channel_eiint;
    uint32_t m_channel_feint;
    uint32_t m_channel_fenmi;

    //actual priority number in register (FENMI > FEINT > EIINT)
    int32_t  m_priority;
    int32_t  m_priority_eiint;
    int32_t  m_priority_feint;
    int32_t  m_priority_fenmi;

    //priority level: FENMI > FEINT > EIINT (bind to host) > BGFEINT > BGEIINT (bind to guest BG) > GMFEINT > GMEIINT (bind to guest GM)
    //                bind to host (m_priority + 0)
    //              > bind to guest (BG) (m_priority + 256) : 256 is priority level of EIINT
    //              > bind to guest (GM) (m_priority + 256 + 16 + 256): 16 is priority level of FEINT
    //priority
    int32_t  m_priority_by_bind;
    int32_t  m_priority_by_bind_eiint;
    int32_t  m_priority_by_bind_feint;
    int32_t  m_priority_by_bind_fenmi;
public:
    static const uint32_t NO_REQ_CHANNEL = 0xFFFFFFFFUL;
    static const int32_t  LOW_PRIORITY   = 0x7FFFFFFFUL;
    // lower number has higher priority. Internally, CForest put minus value to FENMI and FEINT.
    // High priority: FENMI > FEINT0 > FEINT15 > EIINT
    static const int32_t  FEINT_PRIORITY = -16; // FEINT priority : FEINT15=-1,FEINT0=-16
    static const int32_t  FENMI_PRIORITY = -17; // FENMI priority : FENMI=-17

    void UpdateChannel (uint32_t channel, int32_t priority, int32_t priority_by_bind) {
#if defined(_INTC2_UNIT_TEST)
        {
#else
        if (m_priority_by_bind > priority_by_bind) {
#endif
            m_channel = channel;
            m_priority = priority;
            m_priority_by_bind = priority_by_bind;
        }
#if !(_INTC2_UNIT_TEST)
        if (m_priority_by_bind == priority_by_bind) {
            if (m_channel > channel){
                 m_channel = channel;
            }
        }
#endif
    }

    void UpdateChannelEiint (uint32_t channel, int32_t priority, int32_t priority_by_bind) {
#if defined(_INTC2_UNIT_TEST)
            {
#else
            if (m_priority_by_bind_eiint > priority_by_bind) {
#endif
                m_channel_eiint = channel;
                m_priority_eiint = priority;
                m_priority_by_bind_eiint = priority_by_bind;
            }
#if !(_INTC2_UNIT_TEST)
            if (m_priority_by_bind_eiint == priority_by_bind) {
                if (m_channel_eiint > channel){
                     m_channel_eiint = channel;
                }
            }
#endif
    }

    void UpdateChannelFeint (uint32_t channel, int32_t priority, int32_t priority_by_bind) {
#if defined(_INTC2_UNIT_TEST)
            {
#else
            if (m_priority_by_bind_feint > priority_by_bind) {
#endif
                m_channel_feint = channel;
                m_priority_feint = priority;
                m_priority_by_bind_feint = priority_by_bind;
            }
#if !(_INTC2_UNIT_TEST)
            if (m_priority_by_bind_feint == priority_by_bind) {
                if (m_channel_feint > channel){
                     m_channel_feint = channel;
                }
            }
#endif
    }

    void UpdateChannelFenmi (uint32_t channel) {
        m_channel_fenmi = channel;
        m_priority_fenmi = FENMI_PRIORITY;
        m_priority_by_bind_fenmi = FENMI_PRIORITY;
    }


    uint32_t GetChannel (void) const { return m_channel; }
    int32_t GetPriority (void) const { return m_priority; }
    int32_t GetPriorityByBind (void) const { return m_priority_by_bind; }

    uint32_t GetChannelEiint (void) const { return m_channel_eiint; }
    int32_t GetPriorityEiint (void) const { return m_priority_eiint; }
    int32_t GetPriorityByBindEiint (void) const { return m_priority_by_bind_eiint; }

    uint32_t GetChannelFeint (void) const { return m_channel_feint; }
    int32_t GetPriorityFeint (void) const { return m_priority_feint; }
    int32_t GetPriorityByBindFeint (void) const { return m_priority_by_bind_feint; }

    uint32_t GetChannelFenmi (void) const { return m_channel_fenmi; }
    int32_t GetPriorityFenmi (void) const { return m_priority_fenmi; }
    int32_t GetPriorityByBindFenmi (void) const { return m_priority_by_bind_fenmi; }

    void Invalidate (void) {
        m_channel = NO_REQ_CHANNEL;
        m_priority = LOW_PRIORITY;
        m_priority_by_bind = LOW_PRIORITY;

        m_channel_eiint = NO_REQ_CHANNEL;
        m_priority_eiint = LOW_PRIORITY;
        m_priority_by_bind_eiint = LOW_PRIORITY;

        m_channel_feint = NO_REQ_CHANNEL;
        m_priority_feint = LOW_PRIORITY;
        m_priority_by_bind_feint = LOW_PRIORITY;

        m_channel_fenmi = NO_REQ_CHANNEL;
        m_priority_fenmi = LOW_PRIORITY;
        m_priority_by_bind_fenmi = LOW_PRIORITY;
    }
    IntPriorityInfo () { Invalidate(); }
    ~IntPriorityInfo () {}
};

class DLL_EXPORT_IMPORT IntPendingStateInfo {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
public:
#else
private:
#endif
    uint32_t m_channel;
    int32_t  m_pending_state;
    uint32_t  m_eibd_gm;
    uint32_t  m_eibd_gpid;
    bool  m_cpu_gm;
    uint32_t  m_cpu_gpid;

public:
    static const uint32_t NO_PENDING_CHANNEL = 0xFFFFFFFFUL;
    static const int32_t  PENDING_STATE   = 0x0UL;

    void UpdatePendingState (uint32_t channel, uint32_t  gm, uint32_t  gpid, bool  cpu_gm, uint32_t  cpu_gpid ) {
            m_channel = channel;
            m_eibd_gm = gm;
            m_eibd_gpid = gpid;
            m_cpu_gm = cpu_gm;
            m_cpu_gpid = cpu_gpid;
    }
    uint32_t GetChannel (void) const { return m_channel; }
    int32_t GetPendingState (void) const { return m_pending_state; }
    void Invalidate (void) {
        m_channel = NO_PENDING_CHANNEL;
        m_pending_state = PENDING_STATE;
    }
    IntPendingStateInfo () { Invalidate(); }
    ~IntPendingStateInfo () {}
};

/*!
 * @brief PEID information class for SC-HEAP.
*/
class DLL_EXPORT_IMPORT PeidInfo {
public:
    uint32_t m_peid;
    uint32_t GetPeId (void) const { return m_peid; }
    void SetIntc2Ptr (CIntc2* intc2) { return; }
    void CancelIntByIntc2 (uint32_t channel) { return; }
    void UpdateIntHighPriority (ContextId tcid) { return; }
    void ReqIntByPeripheral (uint32_t channel) { return; }
    void ReqIntByIntc2 (uint32_t channel, IntReqInfoBase* intreq_info) { return; }
    void CancelIntByMask (uint32_t channel) { return; }
    bool IsIntReqInQueue (uint32_t channel) { return false; }
    explicit  PeidInfo (uint32_t peid) { m_peid = peid; }
};


/*!
 * @brief INTC2 class.
 */
class DLL_EXPORT_IMPORT CIntc2 : public CModuleVc, public CIntcBase, public Intc2Api
{
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
protected:
    typedef std::vector<PeidInfo*> PeVec;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
private:
    // typedef std::vector<ProcElement*> PeVec;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

#if defined(_INTC2_UNIT_TEST)
public:
#endif
    PeVec* m_pes;
    typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
    IntReqMap m_intc2_queue;

    std::vector<IntPriorityInfo*> m_intc2_priority;

    typedef std::map<uint32_t, uint32_t> BcasMap;
    BcasMap m_intc2_bcas;

    void ReqBcasIntToIntc1 (void);
    uint32_t GetBcasChannel (uint32_t bcp);
    void ClearIntReqQueue (void);
    bool IsIntc2Channel (uint32_t ch) const;
    bool IsValidPeId (uint32_t peid) const;

    // add Yoshinaga for SC-HEAP
#ifdef SUPPORT_SC_HEAP
    uint32_t GetQueueSize(void) { return(m_intc2_queue.size()); }
    uint32_t GetHighPriorityChannel(uint32_t peid) {
        if ((m_core_version >= CORE_VER_20)) {
            return(m_intc2_priority[peid]->GetChannelEiint());
        } else {
            return(m_intc2_priority[peid]->GetChannel());
        }
    }
#endif // SUPPORT_SC_HEAP

#if defined(_INTC2_UNIT_TEST)
    MockFsBus* m_mock_fsbus;
#elif defined(_INTC1_UNIT_TEST)
    MockFsBus* m_fsbus;
#else
    FsBus* m_fsbus;
#endif
    bool m_external_EIINT_req;
    Intc2ImplType m_impl_type;
    bool m_is_cpu_gm[8];
    uint32_t m_cpu_gpid[8];

#if defined(_INTC2_UNIT_TEST)
    MockTRegDBMK* m_mock_dummy_dbmk;
    MockTRegEIBG* m_mock_dummy_eibg;
#else
    TRegDBMK* m_dummy_dbmk;

#endif
public:
    //BCAS_BASE_CHANNELは同報通知ポート0番に接続するINTC1チャネル番号を表す定数
    //接続するチャネル番号は製品依存のため、必要に応じて定数の変更が必要
    static const uint32_t BCAS_BASE_CHANNEL = 4;

    static const PhysAddr INTC2_REG_SIZE = 0x00010000;
    static const PhysAddr INTC2_REG_ADDR_MASK = INTC2_REG_SIZE - 1;

    void CyclicHandler (void);
    Intc2ImplType GetImplType (void) const { return m_impl_type; }

#if defined(_INTC2_UNIT_TEST)
    CIntc2 (PeVec* pes, MockFsBus* mock_fsbus, Intc2ImplType type);
#else
    CIntc2 (PeVec* pes, FsBus* fsbus, Intc2ImplType type);
#endif

    virtual ~CIntc2 ();
    CoreVersion m_core_version;
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);

    IntErrorCode ReqIntByPeripheral (uint32_t channel);
    void ReqIntByEIC (uint32_t channel);
    void CancelInt (uint32_t channel);
    void CancelIntByPeripheral (uint32_t channel);
    bool IsIntLevelDetect (uint32_t channel);
    bool IsIntReqInQueue (uint32_t channel);
    void DelBcasIntInfo (uint32_t channel);
    EiintReqInfo* CreateEiintReqInfo (uint32_t channel);
    TRegEIC*  GetEICPtr (uint32_t channel) const;
    TRegEEIC*  GetEEICPtr (uint32_t channel) const;
    TRegEIBD* GetEIBDPtr (uint32_t channel) const;
    TRegFNC* GetFNCPtr (void) const { return NULL; }
    TRegFIC* GetFICPtr (uint32_t channel) const { return NULL; };
    TRegEIBG* GetEIBGPtr (uint32_t peid) const;
    uint32_t GetIHVEFromIHVCFG (void) const {return 0;}   //temp value using in INTC2 (there is no spec INTC2 yet) 20170809
    CoreVersion GetCoreVersion (void) const {return m_core_version;}
    bool UpdateOverwriteBit_EIC (uint32_t channel);
#if defined(_INTC2_UNIT_TEST)
    MockTRegDBMK* MockGetDBMKPtr (void) const;
    MockTRegEIBG* MockGetEIBGPtr (void) const;
#else
    TRegDBMK* GetDBMKPtr (void) const;
#endif
    bool GetExternalEIINTreq(void) {return m_external_EIINT_req;}
    void SetExternalEIINTreq(bool req) {m_external_EIINT_req = req;}
    bool IsIntReqMasked (IntReqInfoBase * intreq_info, uint32_t channel, int32_t pri, uint32_t peid);
    void SetCpuGuestMode(uint32_t peid, bool is_gm){m_is_cpu_gm[peid] = is_gm;}
    void SetCpuGpid(uint32_t peid, uint32_t gpid){m_cpu_gpid[peid] = gpid;}

    virtual void ReqIntToIntc1 (uint32_t peid, uint32_t channel, IntReqInfoBase* reqinfo);
    virtual void CancelIntToIntc1 (uint32_t peid, uint32_t channel);
    virtual void CancelIntByMask (uint32_t channel);
    virtual void UpdateHighPriority (uint32_t peid);
    virtual void UpdateHighPriorityOfIntc1 (uint32_t pe, uint32_t channel);
    virtual bool IsIntReqInQueueOfIntc1 (uint32_t peid, uint32_t channel);
    virtual void AddBcasIntInfo (uint32_t channel);
    virtual void StartCyclicHandler (void);
    virtual void StopCyclicHandler (void);
};
