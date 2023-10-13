/*
 * (c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./module_guard.h"
#include "./treg_guard.h"
#include "./treg.h"

class TRegCrgCtrlReg;
class TRegCrgErrReg;

/*!
 * @brief TRegCrgCtrlReg: class contains the control register in CRG.
 */
class TRegCrgCtrlReg : public TRegModuleVc {
public:
    static const PhysAddr CRGKCPROT_OFFSET  = 0x00;
    static const PhysAddr CRGPROT_OFFSET    = 0x10;
    static const PhysAddr CRGSPID_OFFSET    = 0x14;
    static const PhysAddr CRGBAD_OFFSET     = 0x18;
    static const PhysAddr CRGADV_OFFSET     = 0x1C;
    static const PhysAddr CRGIVCSPID_OFFSET = 0x20;
    static const PhysAddr CRGIVCREQ_OFFSET      = 0x00;
    static const PhysAddr CRGIVCLOCK_OFFSET     = 0x04;
    static const PhysAddr CRGIVCOWNR_OFFSET     = 0x08;

    static const uint32_t CRG_CTRL_CHANNEL_RANGE_G4MH1X   = 0x0010;
    static const uint32_t CRG_CTRL_CHANNEL_RANGE_G4MH2X   = 0x0020;

    PhysAddr m_ctrl_addr_base;
    PhysAddr m_ivc_addr_base;

    TRegCrgCtrlReg (PhysAddr ctrl_addr_base, PhysAddr ivc_addr_base,
                    CoreType core_type, CoreVersion core_version, GuardInternal_Interface *crg_internal);
    ~TRegCrgCtrlReg ();

    GKCPROT* GetCrgKceProt (void) { return m_crgkceprot; }
    GPROT*   GetCrgProt (uint32_t id) { return m_crgprot[id]; }
    GSPID*   GetCrgSpid (uint32_t id) { return m_crgspid[id]; }
    GBAD*    GetCrgBad  (uint32_t id) { return m_crgbad[id];  }
    GADV*    GetCrgAdv  (uint32_t id) { return m_crgadv[id];  }
    GIVCSPID*  GetCrgIvcSpid (uint32_t id) { return m_crgivcspid[id]; }
    GIVCREQ*   GetCrgIvcReq  (uint32_t id) { return m_crgivcreq[id]; }
    GIVCLOCK*  GetCrgIvcLock (uint32_t id) { return m_crgivclock[id]; }
    GIVCOWNR*  GetCrgIvcOwnr (uint32_t id) { return m_crgivcownr[id];  }
    GuardInternal_Interface* GetCrgInternal  () { return m_crg_internal;  }

private:
    // 引数が合わないため、この関数は使用しない
    void InitTRegMap (void) {}
    CoreType m_core_type;
    CoreVersion m_core_version;

    GKCPROT* m_crgkceprot;
    std::vector<GPROT*> m_crgprot;
    std::vector<GSPID*> m_crgspid;
    std::vector<GBAD*>  m_crgbad;
    std::vector<GADV*>  m_crgadv;
    std::vector<GIVCSPID*>  m_crgivcspid;
    std::vector<GIVCREQ*>   m_crgivcreq;
    std::vector<GIVCLOCK*>  m_crgivclock;
    std::vector<GIVCOWNR*>  m_crgivcownr;
    GuardInternal_Interface *m_crg_internal;
};


/*!
 * @brief Implement CrgCtrl by referring ModuleVc.
 */
class CrgCtrl : public CModuleVc {
public:
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);

    void HardReset (void);

    void CyclicHandler(void) {}
    SysErrCause IsMemAccPermit (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd,
                                CoreType m_core_type, CoreVersion core_version);

    explicit CrgCtrl (PhysAddr ctrl_addr_base, PhysAddr ivc_addr_base,
                      CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal);
    ~CrgCtrl ();

    TRegCrgCtrlReg* GetCrgCtrlReg (void) const { return m_ctrl_regs; }

private:
    const PhysAddr m_ctrl_addr_base;
    const PhysAddr m_ivc_addr_base;
    CoreType m_core_type;
    CoreVersion m_core_version;
    TRegCrgCtrlReg* m_ctrl_regs;  // Return pointer to control registers.

public:
    bool IsAddressMatch (uint32_t id, PhysAddr addr, CoreType core_type, CoreVersion core_version) const;
    bool IsSpidAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
    bool IsWriteGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const;
    bool IsReadGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const;
    bool IsUMDisabled (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
    bool IsDbgAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
};


/*!
 * @brief TRegCrgSecureReg: class contains the secure register in CRG.
 */
class TRegCrgSecureReg : public TRegModuleVc {
public:
    static const PhysAddr CSGPROT_OFFSET    = 0x00;
    static const PhysAddr CSGSPID_OFFSET    = 0x04;
    static const PhysAddr CSGBAD_OFFSET     = 0x08;
    static const PhysAddr CSGADV_OFFSET     = 0x0C;

    static const uint32_t CRG_SEC_CHANNEL_RANGE          = 0x0020;

    PhysAddr m_reg_addr_base;

    TRegCrgSecureReg (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *crg_internal);
    ~TRegCrgSecureReg ();

    GPROT*   GetCrgProt (uint32_t id) { return m_csgprot[id]; }
    GSPID*   GetCrgSpid (uint32_t id) { return m_csgspid[id]; }
    GBAD*    GetCrgBad  (uint32_t id) { return m_csgbad[id];  }
    GADV*    GetCrgAdv  (uint32_t id) { return m_csgadv[id];  }
    GuardInternal_Interface* GetCrgInternal  () { return m_crg_internal;  }

private:
    // Init Register Map for CRG in Security mode.
    void InitTRegMap (void) {}
    CoreType m_core_type;
    CoreVersion m_core_version;

    std::vector<GPROT*> m_csgprot;
    std::vector<GSPID*> m_csgspid;
    std::vector<GBAD*>  m_csgbad;
    std::vector<GADV*>  m_csgadv;
    GuardInternal_Interface *m_crg_internal;
};


/*!
 * @brief Implement CrgSecure by referring ModuleVc.
 */
class CrgSecure : public CModuleVc {
public:
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);

    void HardReset (void);

    void CyclicHandler(void) {}

    explicit CrgSecure (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal);
    SysErrCause IsMemAccPermit (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd,
                                CoreType m_core_type, CoreVersion core_version, uint32_t channel);
    ~CrgSecure ();

    TRegCrgSecureReg* GetCrgSecureReg (void) const { return m_sec_regs; }

private:
    const PhysAddr m_reg_addr_base;
    CoreType m_core_type;
    CoreVersion m_core_version;
    TRegCrgSecureReg* m_sec_regs;  // Return pointer to secure registers.

public:
    bool IsAddressMatch (uint32_t id, PhysAddr addr, CoreType core_type, CoreVersion core_version) const;
    bool IsSpidAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
    bool IsWriteGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const;
    bool IsReadGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const;
    bool IsUMDisabled (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
    bool IsDbgAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
};


/*!
 * @brief TRegCrgErrReg: class contains the status register in CRG.
 */
class TRegCrgErrReg : public TRegModuleVc {
public:
    static const PhysAddr CRGERRCLR_OFFSET  = 0x00;
    static const PhysAddr CRGERRSTAT_OFFSET = 0x04;
    static const PhysAddr CRGERRADDR_OFFSET = 0x08;
    static const PhysAddr CRGERRTYPE_OFFSET = 0x0C;
    static const PhysAddr CRGSPIDERRCLR_OFFSET  = 0x10;
    static const PhysAddr CRGSPIDERRSTAT_OFFSET = 0x14;

    PhysAddr m_reg_addr_base;

    TRegCrgErrReg (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version);
    ~TRegCrgErrReg ();

    GERRCLR*  GetCrgErrClr (void) { return m_crgerrclr; }
    GERRSTAT* GetCrgErrStat (void) { return m_crgerrstat; }
    GERRADDR* GetCrgErrAddr  (void) { return m_crgerraddr;  }
    GERRTYPE* GetCrgErrType  (void) { return m_crgerrtype;  }
    GSPIDERRCLR*  GetCrgSpidErrClr  (void) { return m_crgspiderrclr; }
    GSPIDERRSTAT* GetCrgSpidErrStat (void) { return m_crgspiderrstat; }

private:
    // 引数が合わないため、この関数は使用しない
    void InitTRegMap (void) {}
    CoreType m_core_type;
    CoreVersion m_core_version;

    GERRCLR*  m_crgerrclr;
    GERRSTAT* m_crgerrstat;
    GERRADDR* m_crgerraddr;
    GERRTYPE* m_crgerrtype;
    GSPIDERRCLR* m_crgspiderrclr;
    GSPIDERRSTAT* m_crgspiderrstat;
};


/*!
 * @brief CrgCap.
 */
class CrgCap : public CModuleVc {
public:
    // レジスタアクセス関数
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);

    // リセット関数
    void HardReset (void);

    void CyclicHandler(void) {}

    CrgCap (const PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version);
    ~CrgCap ();

    TRegCrgErrReg* GetCrgErrReg (void) const { return m_err_regs; }

private:
    const PhysAddr m_reg_addr_base;
    CoreType m_core_type;
    CoreVersion m_core_version;
    TRegCrgErrReg* m_err_regs;
};


/*!
 * @brief CRG.
 */
class Crg : public CModuleVc {
public:
    static const PhysAddr CRG_REG_SIZE_G4MH1X           = 0x00000100;
    static const PhysAddr CRG_ERR_REG_PE_SIZE_G4MH1X    = 0x00000010;
    static const PhysAddr CRG_ERR_REG_CRAM_SIZE_G4MH1X  = 0x00000010;
    static const PhysAddr CRG_ERR_REG_SX2MB_G4MH1X      = 0x00000010;

    static const PhysAddr CRG_REG_SIZE_G4MH2X           = 0x00000200;
    static const PhysAddr CRG_ERR_REG_PE_SIZE_G4MH2X    = 0x00000020;
    static const PhysAddr CRG_ERR_REG_CRAM_SIZE_G4MH2X  = 0x00000020;
    static const PhysAddr CRG_ERR_REG_SX2MB_G4MH2X      = 0x00000020;

    // レジスタアクセス関数
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);

    // リセット関数
    void HardReset (void);

    void CyclicHandler(void) {}

    // アクセス許可判定を行う関数
    SysErrCause IsMemAccPermit (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd,
                                CoreType m_core_type, CoreVersion core_version);

    class CrGuard : public ModuleGuard {
    private:
        Crg* m_crg;
        CoreType m_core_type;
        CoreVersion m_core_version;
    public:
        SysErrCause IsAccPermited (SideBand sideband, PhysAddr addr, ByteSize size,
                                   MemAccessCommand cmd )
        {
            return m_crg->IsMemAccPermit (sideband, addr, size, cmd, m_core_type, m_core_version);
        }

        explicit CrGuard (Crg* crg) : m_crg (crg) {
            m_core_type = crg-> m_core_type;
            m_core_version = crg-> m_core_version;
        }
    };

    CrGuard* GetCrgGuard (void) { return m_guard; }

#if _UNIT_TEST
    bool IsBank0Addr (PhysAddr addr) const;
    CrgCtrl* GetCrgCtrl(void) { return m_crg_ctrl_regs; }
    TRegCrgErrReg* GetCrgErrCap(void) { return m_crg_err; }
    CrgSecure* GetCrgSecure(void) { return m_crg_sec_regs; }
    TRegCrgErrReg * m_crg_err = NULL;
    SysErrCause CauseError (SideBand sideband, PhysAddr addr, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version);
#endif

    Crg (const PhysAddr ctrl_addr_base, const PhysAddr ivc_addr_base, const PhysAddr csg_addr_base,
         CoreType core_type, CoreVersion core_version, uint32_t clid);
    ~Crg ();

    uint32_t GetClid (void) const { return m_clid; }

private:
    PhysAddr m_crtl_addr_base;
    PhysAddr m_ivc_addr_base;
    PhysAddr m_sec_addr_base;
    CrGuard* m_guard;
    CrgCtrl* m_crg_ctrl_regs;
    CrgSecure* m_crg_sec_regs;
    CoreType m_core_type;
    CoreVersion m_core_version;
    const uint32_t m_clid;
#if _UNIT_TEST
#else
    bool IsBank0Addr (PhysAddr addr) const;
    SysErrCause CauseError (SideBand sideband, PhysAddr addr, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version);
#endif
};

