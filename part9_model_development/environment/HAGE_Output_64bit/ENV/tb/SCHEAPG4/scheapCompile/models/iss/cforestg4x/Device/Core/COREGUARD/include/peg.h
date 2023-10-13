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

class TRegPegCtrlReg;
class TRegPegErrReg;

/*!
  @brief TRegPegCtrlReg: class contains the control register in PEG.
*/
class TRegPegCtrlReg : public TRegModuleVc {
public:
    static const PhysAddr PEGKCPROT_OFFSET  = 0x00;
    static const PhysAddr PEGPROT_OFFSET    = 0x40;
    static const PhysAddr PEGSPID_OFFSET    = 0x44;
    static const PhysAddr PEGBAD_OFFSET     = 0x48;
    static const PhysAddr PEGADV_OFFSET     = 0x4C;

    static const uint32_t NUM_PEG_CTRL_CHANNEL_G4MH1X     = 4;
    static const uint32_t NUM_PEG_CTRL_CHANNEL_G4MH2X     = 8;
    static const uint32_t PEG_CTRL_CHANNEL_RANGE          = 0x0010;

    PhysAddr m_peg_reg_addr_base;
    uint32_t m_peg_num_channel;

    TRegPegCtrlReg (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version);
    ~TRegPegCtrlReg ();

    GKCPROT* GetPegKceProt (void) { return m_pegkceprot; }
    GPROT*   GetPegProt (uint32_t id) { return m_pegprot[id]; }
    GSPID*   GetPegSpid (uint32_t id) { return m_pegspid[id]; }
    GBAD*    GetPegBad  (uint32_t id) { return m_pegbad[id];  }
    GADV*    GetPegAdv  (uint32_t id) { return m_pegadv[id];  }
    GuardInternal* GetPegInternal  () { return m_peg_internal;  }

private:
    // 引数が合わないため、この関数は使用しない
    void InitTRegMap (void) {}
    CoreType m_core_type;
    CoreVersion m_core_version;

    GKCPROT* m_pegkceprot;
    std::vector<GPROT*> m_pegprot;
    std::vector<GSPID*> m_pegspid;
    std::vector<GBAD*>  m_pegbad;
    std::vector<GADV*>  m_pegadv;
    GuardInternal *m_peg_internal;

};


/*!
  @brief Implement PegCtrl by referring ModuleVc
*/
class PegCtrl : public CModuleVc {
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

    explicit PegCtrl (uint32_t peid, PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version);
    ~PegCtrl ();

    TRegPegCtrlReg* GetPegCtrlReg (void) const { return m_ctrl_regs; }

private:
    uint32_t m_peid;
    const PhysAddr m_reg_addr_base;
    CoreType m_core_type;
    CoreVersion m_core_version;
    TRegPegCtrlReg* m_ctrl_regs;
};


/*!
  @brief TRegPegErrReg: class contains the error register in PEG.
*/
class TRegPegErrReg : public TRegModuleVc {
public:
    static const PhysAddr PEGERRCLR_OFFSET      = 0x00;
    static const PhysAddr PEGERRSTAT_OFFSET     = 0x04;
    static const PhysAddr PEGERRADDR_OFFSET     = 0x08;
    static const PhysAddr PEGERRTYPE_OFFSET     = 0x0C;
    static const PhysAddr PEGSPIDERRCLR_OFFSET  = 0x10;
    static const PhysAddr PEGSPIDERRSTAT_OFFSET = 0x14;

    PhysAddr m_reg_addr_base;

    TRegPegErrReg (PhysAddr reg_addr_base_master, CoreType core_type, CoreVersion core_version);
    ~TRegPegErrReg ();

    GERRCLR*      GetPegErrClr (void) { return m_pegerrclr; }
    GERRSTAT*     GetPegErrStat (void) { return m_pegerrstat; }
    GERRADDR*     GetPegErrAddr  (void) { return m_pegerraddr; }
    GERRTYPE*     GetPegErrType  (void) { return m_pegerrtype; }
    GSPIDERRCLR*  GetPegSpidErrClr  (void) { return m_pegspiderrclr; }
    GSPIDERRSTAT* GetPegSpidErrStat (void) { return m_pegspiderrstat; }

private:
    void InitTRegMap (void) {}
    CoreType m_core_type;
    CoreVersion m_core_version;

    GERRCLR*  m_pegerrclr;
    GERRSTAT* m_pegerrstat;
    GERRADDR* m_pegerraddr;
    GERRTYPE* m_pegerrtype;
    GSPIDERRCLR* m_pegspiderrclr;
    GSPIDERRSTAT* m_pegspiderrstat;
};


/*!
  @brief PegCap
*/
class PegCap : public CModuleVc {
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

    PegCap (uint32_t peid, const PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version);
    ~PegCap ();

    TRegPegErrReg* GetPegErrReg (void) const { return m_err_regs; }

private:
    uint32_t m_peid;
    const PhysAddr m_reg_addr_base;
    CoreType m_core_type;
    CoreVersion m_core_version;
    TRegPegErrReg* m_err_regs;
};


/*!
  @brief PEG
*/
class Peg : public CModuleVc {
public:
    static const PhysAddr PEG_CTRL_REG_SIZE = 0x00000100;
    static const PhysAddr PEG_ERR_REG_M_SIZE_G4MH1X  = 0x00000010;
    static const PhysAddr PEG_ERR_REG_S_SIZE_G4MH1X  = 0x00000010;
    static const PhysAddr PEG_ERR_REG_M_SIZE_G4MH2X  = 0x00000020;
    static const PhysAddr PEG_ERR_REG_S_SIZE_G4MH2X  = 0x00000020;
    static const uint32_t L1RAM_E2X_START = 0xFC000000;
    static const uint32_t L1RAM_E2X_END   = 0xFDFFFFFF;

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

    class PeGuard : public ModuleGuard {
    private:
        Peg* m_peg;
        CoreType m_core_type;
        CoreVersion m_core_version;
    public:
        SysErrCause IsAccPermited (SideBand sideband, PhysAddr addr, ByteSize size,
                                   MemAccessCommand cmd )
        {
            return m_peg->IsMemAccPermit (sideband, addr, size, cmd, m_core_type, m_core_version);
        }

        explicit PeGuard (Peg* peg) : m_peg (peg) {
            m_core_type = peg-> m_core_type;
            m_core_version = peg-> m_core_version;
        }
    };

    PeGuard* GetPegGuard (void) { return m_guard; }
#if _UNIT_TEST
    PegCtrl* GetPegCtrl(void) { return m_peg_ctrl_regs; }
    TRegPegErrReg* GetPegErrCap(void) { return m_peg_err; }
    TRegPegErrReg * m_peg_err = NULL;
#endif

    Peg (uint32_t peid, const PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version);
    ~Peg ();

private:
    uint32_t m_peid;
    const PhysAddr m_reg_addr_base;
    PeGuard* m_guard;
    PegCtrl* m_peg_ctrl_regs;
    CoreType m_core_type;
    CoreVersion m_core_version;

public:
#if _UNIT_TEST
    bool IsSPCAddr (PhysAddr addr) const;
#endif
    bool IsAddressMatch (uint32_t id, PhysAddr addr, CoreType core_type, CoreVersion core_version) const;
    bool IsSpidAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
    bool IsWriteGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const;
    bool IsReadGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const;
    bool IsUMDisabled (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
    bool IsDbgAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const;
    SysErrCause CauseError (SideBand sideband, PhysAddr addr, MemAccessCommand cmd,
                            CoreType core_type, CoreVersion core_version, SysErrCause peg_capture_cause);
};

