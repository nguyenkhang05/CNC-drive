/*
 * (c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <sstream>
#include "./config.h"
#include "./forest.h"
#include "./crg.h"
#include "./sideband.h"
#if _UNIT_TEST
#include "mock_guard_base.h"
#else
#include "./forest_internals.h"
#include "./forest_utility.h"

extern ForestUtil* p_forest_util;
#endif

/*!
 * @brief Constructor of CRG control registers.
 * @param ctrl_addr_base Address base of CRG control register.
 * @param ivc_addr_base
 * @param core_type G4MH core type.
 * @param core_version
 * @param crg_internal
 * @return void.
 *
 * @todo To be documented.
 */

TRegCrgCtrlReg::TRegCrgCtrlReg (PhysAddr ctrl_addr_base, PhysAddr ivc_addr_base,
                                CoreType core_type, CoreVersion core_version, GuardInternal_Interface *crg_internal)
    : m_ctrl_addr_base (ctrl_addr_base),
      m_ivc_addr_base (ivc_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    m_crg_internal = crg_internal;
    // There are 8 channels in CRG control registers.
    uint32_t max_channel = crg_internal->GetMaxCtrlChannel();

    if ((core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
        PhysAddr crgkcprot_addr = ctrl_addr_base + CRGKCPROT_OFFSET;
        m_crgkceprot = new GKCPROT (SIZE_32BIT, SIZE_32BIT, crgkcprot_addr, "CRGKCEPROT", crg_internal);
        m_trf.insert (TRegMap::value_type (crgkcprot_addr, m_crgkceprot));
        for (uint32_t id = 0; id < max_channel; ++id) {
            PhysAddr crgprot_addr = ctrl_addr_base + CRGPROT_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH1X * id;
            std::ostringstream crgprot_name;
            crgprot_name << "CRGPROT" << id;
            GPROT* crgprot_ptr = new GPROT (SIZE_32BIT, SIZE_8BIT, crgprot_addr, id, crgprot_name.str(),
                                            CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgprot_addr, crgprot_ptr));
            m_crgprot.push_back (crgprot_ptr);

            PhysAddr crgspid_addr = ctrl_addr_base + CRGSPID_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH1X * id;
            std::ostringstream crgspid_name;
            crgspid_name << "CRGSPID" << id;
            GSPID* crgspid_ptr = new GSPID (SIZE_32BIT, SIZE_8BIT, crgspid_addr, id, crgspid_name.str(),
                                            CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgspid_addr, crgspid_ptr));
            m_crgspid.push_back (crgspid_ptr);

            // pMADDRW is not used in CRG module with G4MH1x version.
            PhysAddr crgbad_addr = ctrl_addr_base + CRGBAD_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH1X * id;
            std::ostringstream crgbad_name;
            crgbad_name << "CRGBAD" << id;
            GBAD* crgbad_ptr = new GBAD (SIZE_32BIT, SIZE_8BIT, crgbad_addr, id, crgbad_name.str(),
                                         0x0, CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgbad_addr, crgbad_ptr));
            m_crgbad.push_back (crgbad_ptr);

            PhysAddr crgadv_addr = ctrl_addr_base + CRGADV_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH1X * id;
            std::ostringstream crgadv_name;
            crgadv_name << "CRGADV" << id;
            GADV* crgadv_ptr = new GADV (SIZE_32BIT, SIZE_8BIT, crgadv_addr, id, crgadv_name.str(),
                                         0x0, CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgadv_addr, crgadv_ptr));
            m_crgadv.push_back (crgadv_ptr);
        }
    } else {
        uint32_t pmaddrw = m_crg_internal->GetCrgMaddrw();
        PhysAddr crgkcprot_addr = ctrl_addr_base + CRGKCPROT_OFFSET;
        m_crgkceprot = new GKCPROT (SIZE_32BIT, SIZE_32BIT, crgkcprot_addr, "CRGKCEPROT", crg_internal);
        m_trf.insert (TRegMap::value_type (crgkcprot_addr, m_crgkceprot));
        for (uint32_t id = 0; id < max_channel; ++id) {
            PhysAddr crgprot_addr = ctrl_addr_base + CRGPROT_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgprot_name;
            crgprot_name << "CRGPROT" << id;
            GPROT* crgprot_ptr = new GPROT (SIZE_32BIT, SIZE_8BIT, crgprot_addr, id, crgprot_name.str(),
                                            CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgprot_addr, crgprot_ptr));
            m_crgprot.push_back (crgprot_ptr);

            PhysAddr crgspid_addr = ctrl_addr_base + CRGSPID_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgspid_name;
            crgspid_name << "CRGSPID" << id;
            GSPID* crgspid_ptr = new GSPID (SIZE_32BIT, SIZE_8BIT, crgspid_addr, id, crgspid_name.str(),
                                            CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgspid_addr, crgspid_ptr));
            m_crgspid.push_back (crgspid_ptr);

            PhysAddr crgbad_addr = ctrl_addr_base + CRGBAD_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgbad_name;
            crgbad_name << "CRGBAD" << id;
            GBAD* crgbad_ptr = new GBAD (SIZE_32BIT, SIZE_8BIT, crgbad_addr, id, crgbad_name.str(),
                                         pmaddrw, CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgbad_addr, crgbad_ptr));
            m_crgbad.push_back (crgbad_ptr);

            PhysAddr crgadv_addr = ctrl_addr_base + CRGADV_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgadv_name;
            crgadv_name << "CRGADV" << id;
            GADV* crgadv_ptr = new GADV (SIZE_32BIT, SIZE_8BIT, crgadv_addr, id, crgadv_name.str(),
                                         pmaddrw, CRG_MODULE, core_type, core_version, crg_internal);
            m_trf.insert (TRegMap::value_type (crgadv_addr, crgadv_ptr));
            m_crgadv.push_back (crgadv_ptr);

            PhysAddr crgivcspid_addr = ctrl_addr_base + CRGIVCSPID_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgivcspid_name;
            crgivcspid_name << "CRGIVCSPID" << id;
            GIVCSPID* crgivcspid_ptr = new GIVCSPID (SIZE_32BIT, SIZE_8BIT, crgivcspid_addr, id, crgivcspid_name.str(), crg_internal);
            m_trf.insert (TRegMap::value_type (crgivcspid_addr, crgivcspid_ptr));
            m_crgivcspid.push_back (crgivcspid_ptr);

            PhysAddr crgivclock_addr = ivc_addr_base + CRGIVCLOCK_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgivclock_name;
            crgivclock_name << "CRGIVCLOCK" << id;
            GIVCLOCK* crgivclock_ptr = new GIVCLOCK (SIZE_32BIT, SIZE_32BIT, crgivclock_addr, id, crgivclock_name.str(), crg_internal);
            m_trf.insert (TRegMap::value_type (crgivclock_addr, crgivclock_ptr));
            m_crgivclock.push_back (crgivclock_ptr);

            PhysAddr crgivcreq_addr = ivc_addr_base + CRGIVCREQ_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgivcreq_name;
            crgivcreq_name << "CRGIVCREQ" << id;
            GIVCREQ* crgivcreq_ptr = new GIVCREQ (SIZE_32BIT, SIZE_32BIT, crgivcreq_addr, id, crgivcreq_name.str(),
                                                  crg_internal, crgspid_ptr, crgivclock_ptr);
            m_trf.insert (TRegMap::value_type (crgivcreq_addr, crgivcreq_ptr));
            m_crgivcreq.push_back (crgivcreq_ptr);

            PhysAddr crgivcown_addr = ivc_addr_base + CRGIVCOWNR_OFFSET + CRG_CTRL_CHANNEL_RANGE_G4MH2X * id;
            std::ostringstream crgivcownr_name;
            crgivcownr_name << "CRGIVCOWNR" << id;
            GIVCOWNR* crgivcownr_ptr = new GIVCOWNR (SIZE_32BIT, SIZE_32BIT, crgivcown_addr, id, crgivcownr_name.str(), crg_internal);
            m_trf.insert (TRegMap::value_type (crgivcown_addr, crgivcownr_ptr));
            m_crgivcownr.push_back (crgivcownr_ptr);
        }
    }
}


/*!
 * @brief Destructor of CRG control registers.
 * @param -
 * @return void.
 */

TRegCrgCtrlReg::~TRegCrgCtrlReg ()
{
    // This function also deletes the pointers of m_crgadv, m_crgbad, m_crgspid, m_crgprot, m_crgkceprot.
    DelTRegMap ();
    delete m_crg_internal;
}


/*!
 * @brief Constructor of CRG secure registers.
 * @param reg_addr_base address base of CRG control register.
 * @param core_type G4MH2x core type.
 * @param core_version
 * @param crg_internal
 * @return void.
 *
 * @todo To be documented.
 */

TRegCrgSecureReg::TRegCrgSecureReg (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *crg_internal)
    : m_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    m_crg_internal = crg_internal;
    // There are 4 channels in CSG setting registers.
    uint32_t max_channel = m_crg_internal->GetMaxCsgChannel();
    uint32_t pmaddrw = m_crg_internal->GetCrgMaddrw();

    for (uint32_t id = 0; id < max_channel; ++id) {
        PhysAddr csgprot_addr = m_reg_addr_base + CSGPROT_OFFSET + CRG_SEC_CHANNEL_RANGE * id;
        std::ostringstream csgprot_name;
        csgprot_name << "CSGPROT" << id;
        GPROT* csgprot_ptr = new GPROT (SIZE_32BIT, SIZE_8BIT, csgprot_addr, id, csgprot_name.str(),
                                        CSG_MODULE, core_type, core_version, m_crg_internal);
        m_trf.insert (TRegMap::value_type (csgprot_addr, csgprot_ptr));
        m_csgprot.push_back (csgprot_ptr);

        PhysAddr csgspid_addr = m_reg_addr_base + CSGSPID_OFFSET + CRG_SEC_CHANNEL_RANGE * id;
        std::ostringstream csgspid_name;
        csgspid_name << "CSGSPID" << id;
        GSPID* csgspid_ptr = new GSPID (SIZE_32BIT, SIZE_8BIT, csgspid_addr, id, csgspid_name.str(),
                                        CSG_MODULE, core_type, core_version, m_crg_internal);
        m_trf.insert (TRegMap::value_type (csgspid_addr, csgspid_ptr));
        m_csgspid.push_back (csgspid_ptr);

        PhysAddr csgbad_addr = m_reg_addr_base + CSGBAD_OFFSET + CRG_SEC_CHANNEL_RANGE * id;
        std::ostringstream csgbad_name;
        csgbad_name << "CSGBAD" << id;
        GBAD* csgbad_ptr = new GBAD (SIZE_32BIT, SIZE_8BIT, csgbad_addr, id, csgbad_name.str(),
                                     pmaddrw, CSG_MODULE, core_type, core_version, m_crg_internal);
        m_trf.insert (TRegMap::value_type (csgbad_addr, csgbad_ptr));
        m_csgbad.push_back (csgbad_ptr);

        PhysAddr csgadv_addr = m_reg_addr_base + CSGADV_OFFSET + CRG_SEC_CHANNEL_RANGE * id;
        std::ostringstream csgadv_name;
        csgadv_name << "CSGADV" << id;
        GADV* csgadv_ptr = new GADV (SIZE_32BIT, SIZE_8BIT, csgadv_addr, id, csgadv_name.str(),
                                     pmaddrw, CSG_MODULE, core_type, core_version, m_crg_internal);
        m_trf.insert (TRegMap::value_type (csgadv_addr, csgadv_ptr));
        m_csgadv.push_back (csgadv_ptr);
    }
}


/*!
 * @brief Destructor of CRG secure registers.
 * @param -
 * @return void.
 */

TRegCrgSecureReg::~TRegCrgSecureReg ()
{
    // This function also deletes the pointers of m_crgadv, m_crgbad, m_crgspid, m_crgprot.
    DelTRegMap ();
}


/*!
 * @brief Constructor of CRG error registers.
 * @param reg_addr_base address base of CRG error register.
 * @param core_type G4MH core type.
 * @param core_version
 * @return void.
 *
 * @todo To be documented.
 */

TRegCrgErrReg::TRegCrgErrReg (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version)
    : m_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    if ((core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
        // Add error register in case CRG capture
        // For G4MH1x.
        PhysAddr crgerrstat_addr = m_reg_addr_base + CRGERRSTAT_OFFSET;
        m_crgerrstat = new GERRSTAT (SIZE_32BIT, SIZE_32BIT, crgerrstat_addr, "CRGERRSTAT", core_type, core_version);
        m_trf.insert (TRegMap::value_type (crgerrstat_addr, m_crgerrstat));

        PhysAddr crgerrclr_addr  = m_reg_addr_base + CRGERRCLR_OFFSET;
        m_crgerrclr = new GERRCLR (SIZE_32BIT, SIZE_8BIT, crgerrclr_addr, m_crgerrstat, "CRGERRCLR", core_type, core_version);
        m_trf.insert (TRegMap::value_type (crgerrclr_addr, m_crgerrclr));

        PhysAddr crgerraddr_addr = m_reg_addr_base + CRGERRADDR_OFFSET;
        m_crgerraddr = new GERRADDR (SIZE_32BIT, SIZE_32BIT, crgerraddr_addr, "CRGERRADDR");
        m_trf.insert (TRegMap::value_type (crgerraddr_addr, m_crgerraddr));

        PhysAddr crgerrtype_addr = m_reg_addr_base + CRGERRTYPE_OFFSET;
        m_crgerrtype = new GERRTYPE (SIZE_32BIT, SIZE_32BIT, crgerrtype_addr, "CRGERRTYPE");
        m_trf.insert (TRegMap::value_type (crgerrtype_addr, m_crgerrtype));
    } else {
        // Add error register in case master PEG capture
        // For G4MH2x and later.

        // Name is different in G4MH2x
        PhysAddr crgerrstat_addr = m_reg_addr_base + CRGERRSTAT_OFFSET;
        m_crgerrstat = new GERRSTAT (SIZE_32BIT, SIZE_8BIT, crgerrstat_addr, "CRGOVFSTAT", core_type, core_version);
        m_trf.insert (TRegMap::value_type (crgerrstat_addr, m_crgerrstat));

        PhysAddr crgerrclr_addr  = m_reg_addr_base + CRGERRCLR_OFFSET;
        m_crgerrclr = new GERRCLR (SIZE_32BIT, SIZE_8BIT, crgerrclr_addr, m_crgerrstat, "CRGOVFCLR", core_type, core_version);
        m_trf.insert (TRegMap::value_type (crgerrclr_addr, m_crgerrclr));

        PhysAddr crgerraddr_addr = m_reg_addr_base + CRGERRADDR_OFFSET;
        m_crgerraddr = new GERRADDR (SIZE_32BIT, SIZE_32BIT, crgerraddr_addr, "CRGERRADDR");
        m_trf.insert (TRegMap::value_type (crgerraddr_addr, m_crgerraddr));

        PhysAddr crgerrtype_addr = m_reg_addr_base + CRGERRTYPE_OFFSET;
        m_crgerrtype = new GERRTYPE (SIZE_32BIT, SIZE_16BIT, crgerrtype_addr, "CRGERRTYPE");
        m_trf.insert (TRegMap::value_type (crgerrtype_addr, m_crgerrtype));

        PhysAddr crgspiderrstat_addr = m_reg_addr_base + CRGSPIDERRSTAT_OFFSET;
        m_crgspiderrstat = new GSPIDERRSTAT (SIZE_32BIT, SIZE_8BIT, crgspiderrstat_addr, "CRGSPIDERRSTAT", core_type, core_version);
        m_trf.insert (TRegMap::value_type (crgspiderrstat_addr, m_crgspiderrstat));

        PhysAddr crgspiderrclr_addr_m = m_reg_addr_base + CRGSPIDERRCLR_OFFSET;
        m_crgspiderrclr = new GSPIDERRCLR (SIZE_32BIT, SIZE_8BIT, crgspiderrclr_addr_m, m_crgspiderrstat, "CRGSPIDERRCLR", core_type, core_version);
        m_trf.insert (TRegMap::value_type (crgspiderrclr_addr_m, m_crgspiderrclr));
    }
}


/*!
 * @brief Destructor of CRG error registers.
 * @param -
 * @return void.
 */

TRegCrgErrReg::~TRegCrgErrReg ()
{
    // This function also deletes the pointers of m_crgadv, m_crgbad, m_crgspid, m_crgprot, m_crgkceprot.
    DelTRegMap ();
}


/*!
 * @brief Call specified CRG control register base on address to check write condition.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 *
 * @todo To be documented.
 */

SysErrCause CrgCtrl::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->Write (sideband, addr, data, size);

    // At least, 1 CRGPROT.GEN is updated to 0.
    // Need to check CRGPROT.GEN in all channels at once.
    m_ctrl_regs->GetCrgInternal()->SetGuardEn(false);
    // There are 8 channels in CRG control registers.
    uint32_t max_channel = m_ctrl_regs->GetCrgInternal()->GetMaxCtrlChannel();
    for (uint32_t ch = 0; ch < max_channel; ch++) {
        // PEGPROT.GEN is bit 8.
        if (m_ctrl_regs->GetCrgProt (ch)->GetGEN () == 0x1) {
            m_ctrl_regs->GetCrgInternal()->SetGuardEn(true);
            m_ctrl_regs->GetCrgInternal()->SetChannelEn(ch, true);
        }
    }

    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG control register base on address to check read condition.
 * @param sideband Sideband info.
 * @param addr address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgCtrl::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG control register base on address to write new data by Debug method.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgCtrl::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->WriteBody (addr, data, size);

    // At least, 1 CRGPROT.GEN is updated to 0.
    // Need to check CRGPROT.GEN in all channels at once.
    m_ctrl_regs->GetCrgInternal()->SetGuardEn(false);
    // There are 8 channels in CRG control registers.
    uint32_t max_channel = m_ctrl_regs->GetCrgInternal()->GetMaxCtrlChannel();
    for (uint32_t ch = 0; ch < max_channel; ch++) {
        // PEGPROT.GEN is bit 8.
        if (m_ctrl_regs->GetCrgProt (ch)->GetGEN () == 0x1) {
            m_ctrl_regs->GetCrgInternal()->SetGuardEn(true);
            m_ctrl_regs->GetCrgInternal()->SetChannelEn(ch, true);
        }
    }

    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG control register base on address to read data by Debug method.
 * @param addr Address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgCtrl::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Update all CRG control register to reset status.
 * @param -
 * @return void.
 */

void CrgCtrl::HardReset (void)
{
    m_ctrl_regs->GetCrgInternal()->Reset();
    m_ctrl_regs->ResetTRegs();
}


/*!
 * @brief Call specified CRG control register base on address to check write condition.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgSecure::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_sec_regs->GetTrPtr(addr)->Write (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG control register base on address to check read condition.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgSecure::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_sec_regs->GetTrPtr(addr)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG control register base on address to write new data by Debug method.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgSecure::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_sec_regs->GetTrPtr(addr)->WriteBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG control register base on address to read data by Debug method.
 * @param addr Address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgSecure::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_sec_regs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Update all CRG control register to reset status.
 * @param -
 * @return void.
 */

void CrgSecure::HardReset (void)
{
    m_sec_regs->ResetTRegs();
}


/*!
 * @brief Call specified CRG capture register base on address to check write condition.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgCap::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_err_regs->GetTrPtr(addr)->Write (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG capture register base on address to check read condition.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgCap::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_err_regs->GetTrPtr(addr)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG capture register base on address to write new data by Debug method.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgCap::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (addr == m_reg_addr_base + TRegCrgErrReg::CRGERRCLR_OFFSET) {
        m_err_regs->GetTrPtr(addr)->WriteBody (addr, data, size);
    }
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG capture register base on address to read data by Debug method.
 * @param addr Address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause CrgCap::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_err_regs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Update all CRG capture register to reset status.
 * @param -
 * @return void.
 */

void CrgCap::HardReset (void)
{
    m_err_regs->ResetTRegs();
}


/*!
 * @brief Call specified CRG register base on address to check write condition.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause Crg::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((m_core_type == CORE_G4MH) && (m_core_version < CORE_VER_20)) {
        m_crg_ctrl_regs->TargetWrite (sideband, addr, data, size);
    } else {
        if ((addr >= 0xFFC6E000) && (addr < 0xFFC6E800)) {
            m_crg_sec_regs->TargetWrite (sideband, addr, data, size);
        } else {
            m_crg_ctrl_regs->TargetWrite (sideband, addr, data, size);
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG register base on address to check read condition.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause Crg::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((m_core_type == CORE_G4MH) && (m_core_version < CORE_VER_20)) {
        m_crg_ctrl_regs->TargetRead (sideband, addr, data, size);
    } else {
        if ((addr >= 0xFFC6E000) && (addr < 0xFFC6E800)) {
            m_crg_sec_regs->TargetRead (sideband, addr, data, size);
        } else {
            m_crg_ctrl_regs->TargetRead (sideband, addr, data, size);
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG register base on address to write new data by Debug method.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause Crg::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((m_core_type == CORE_G4MH) && (m_core_version < CORE_VER_20)) {
        m_crg_ctrl_regs->TargetWriteDebug (addr, data, size);
    } else {
        if ((addr >= 0xFFC6E000) && (addr < 0xFFC6E800)) {
            m_crg_sec_regs->TargetWriteDebug (addr, data, size);
        } else {
            m_crg_ctrl_regs->TargetWriteDebug (addr, data, size);
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Call specified CRG register base on address to read data by Debug method.
 * @param addr Address.
 * @param data Read data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */

SysErrCause Crg::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((m_core_type == CORE_G4MH) && (m_core_version < CORE_VER_20)) {
        m_crg_ctrl_regs->TargetReadDebug (addr, data, size);
    } else {
        if ((addr >= 0xFFC6E000) && (addr < 0xFFC6E800)) {
            m_crg_sec_regs->TargetReadDebug (addr, data, size);
        } else {
            m_crg_ctrl_regs->TargetReadDebug (addr, data, size);
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Update all CRG register to reset status.
 * @param -
 * @return void.
 */

void Crg::HardReset (void)
{
    m_crg_ctrl_regs->HardReset();
    if ((m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
        m_crg_sec_regs->HardReset();
    }
}


/*!
 * @brief Check one memory access is permitted in CRG setting.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param size Access size.
 * @param cmd Access operation.
 * @param core_type CPU core type.
 * @param core_version
 * @return System Error Cause code.
 *
 * @todo To be documented.
 */

SysErrCause Crg::IsMemAccPermit (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd,
                                 CoreType core_type, CoreVersion core_version)
{
    // Step 1: Check RWM instructions is in read access or not.
    // In G4MH CRG, RWM instruction is treated as write type.
    // So, in case read type, RMW instruction (include NOT1/CLR1/SET1/CAXI) is ignored.
    if ((sideband.GetBitOp() != SideBand::BITOP_NONE) && (cmd == DATA_READ)) {
        return SYSERR_NONE;
    }

    // Step 2: Check CRG_SEC module is used or not.
    if ((m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
        uint32_t max_sec_channel     = GuardInternal_Interface::NUM_CRG_SEC_CHANNEL;
        bool csg_function_en = false;
        for (uint32_t chn_id = 0; chn_id < max_sec_channel; chn_id++) {
            uint32_t crg_chn_en = m_crg_sec_regs->GetCrgSecureReg()->GetCrgProt(chn_id)->GetGEN();
            if (crg_chn_en) {
                // Step 2: Check address is valid or not.
                if (m_crg_sec_regs->IsAddressMatch(chn_id, addr, core_type, core_version)) {
                    // PE satisfied Address comparison.
                    csg_function_en = true;
                    // Continue to check the remaining condition.
                    SysErrCause crg_sec = m_crg_sec_regs->IsMemAccPermit(sideband, addr, size, cmd, core_type, core_version, chn_id);
                    if (crg_sec == SYSERR_CRG) {
                        return CauseError (sideband, addr, cmd, core_type, core_version);
                    }
                    // Continue to check the remaining channels.
                }
            }
        }
        // If CRG_SEC is used, and all address_channel is OK, access is permitted.
        if (csg_function_en == true) {
            return SYSERR_NONE;
        }
    }

    // Step 3: Check CRGPROTn.GEN at 4 channels.
    // This step is done at CrGuard::IsAccPermited.
    if (!m_crg_ctrl_regs->GetCrgCtrlReg()->GetCrgInternal()->IsGuardEn()) {
        return SYSERR_NONE;
    }

    // Step 4: Check access is permitted in CRG setting or not.
    SysErrCause crg_ctrl = m_crg_ctrl_regs->IsMemAccPermit(sideband, addr, size, cmd, core_type, core_version);
    if (crg_ctrl == SYSERR_CRG)
    {
#if _UNIT_TEST
        // To do something;
         return SYSERR_CRG;
#else
         return CauseError (sideband, addr, cmd, core_type, core_version);
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Check address belongs to bank0 or bank1.
 * @param addr Address.
 * @return boolean value.
 */

bool Crg::IsBank0Addr (PhysAddr addr) const
{
    return ((addr & 0x00000004) == 0x0);
}


/*!
 * @brief Update CRG capture registers when a memory access is not permitted.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param cmd Access operation.
 * @param core_type CPU core type.
 * @param core_version
 * @return System Error Cause code.
 *
 * @todo To be documented.
 */

SysErrCause Crg::CauseError (SideBand sideband, PhysAddr addr, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version)
{
#if _UNIT_TEST
        // To do something;
    uint32_t peid_acc = sideband.GetBMID();
    TRegCrgErrReg *cur_err_cap = new TRegCrgErrReg(0xFFFFFFFF, core_type, core_version);
#else
    uint32_t peid_acc = p_forest_util->GetPeidFromBmid(sideband.GetBMID());
    TRegCrgErrReg *cur_err_cap = NULL;
#endif
    bool is_bank0_addr = IsBank0Addr(addr);
    if (cmd == INST_READ) {
        if (is_bank0_addr) {
#if _UNIT_TEST
        // To do something;
            cur_err_cap = new TRegCrgErrReg (0xFFC6D930, core_type, core_version);
#else
            cur_err_cap = static_cast <TRegCrgErrReg *> (p_internals->GetGuardCapErr(GMID_CRGCAP_SX2MBLCLm, m_clid));
#endif
        } else {
#if _UNIT_TEST
        // To do something;
            cur_err_cap = new TRegCrgErrReg (0xFFC6D920, core_type, core_version);
#else
            cur_err_cap = static_cast <TRegCrgErrReg *> (p_internals->GetGuardCapErr(GMID_CRGCAP_SX2MBHCLm, m_clid));
#endif
        }
    } else if (cmd == DATA_WRITE) {
        if (is_bank0_addr) {
#if _UNIT_TEST
        // To do something;
            cur_err_cap = new TRegCrgErrReg (0xFFC6D910, core_type, core_version);
#else
            cur_err_cap = static_cast <TRegCrgErrReg *> (p_internals->GetGuardCapErr(GMID_CRGCAP_CRAMLCLm, m_clid));
#endif
        } else {
#if _UNIT_TEST
        // To do something;
            cur_err_cap = new TRegCrgErrReg (0xFFC6D900, core_type, core_version);
#else
            cur_err_cap = static_cast <TRegCrgErrReg *> (p_internals->GetGuardCapErr(GMID_CRGCAP_CRAMHCLm, m_clid));
#endif
        }
    } else if (cmd == DATA_READ) {
#if _UNIT_TEST
        // To do something;
        if (peid_acc == 0x0) {
        // To do something;
            cur_err_cap = new TRegCrgErrReg (0xFFC6D800, core_type, core_version);
#else
        if (p_forest_util->IsValidPeId(peid_acc)) {
            cur_err_cap = static_cast <TRegCrgErrReg *> (p_internals->GetGuardCapErr(GMID_CRGCAP_PEnCLm, peid_acc));
#endif
        } else {
            // Unknown busmaster. It will be peripherals.
            if (is_bank0_addr) {
#if _UNIT_TEST
        // To do something;
                cur_err_cap = new TRegCrgErrReg (0xFFC6D930, core_type, core_version);
#else
                cur_err_cap = static_cast <TRegCrgErrReg *> (p_internals->GetGuardCapErr(GMID_CRGCAP_SX2MBLCLm, m_clid));
#endif
            } else {
#if _UNIT_TEST
        // To do something;
                cur_err_cap = new TRegCrgErrReg (0xFFC6D920, core_type, core_version);
#else
                cur_err_cap = static_cast <TRegCrgErrReg *> (p_internals->GetGuardCapErr(GMID_CRGCAP_SX2MBHCLm, m_clid));
#endif
            }
        }
    }

#if _UNIT_TEST
        // To do something;
    if (m_crg_err != NULL)
        cur_err_cap = m_crg_err;
    else
        m_crg_err = cur_err_cap;
#endif

    // Update CRG Error registers.
    if ((core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
        if (cur_err_cap->GetCrgErrStat()->GetERR()) {
            cur_err_cap->GetCrgErrStat()->SetOVF(0x1);
        } else {
            // The value of error info: Addr, SEC, DBG, UM, SPID, WRITE
            // are only stored if ERR = 0 when there is error in PEG.
            cur_err_cap->GetCrgErrStat()->SetERR(0x1);
            cur_err_cap->GetCrgErrAddr()->SetAddr(addr);
            cur_err_cap->GetCrgErrType()->SetSEC(0x0);
            cur_err_cap->GetCrgErrType()->SetDBG(sideband.GetDM());
            cur_err_cap->GetCrgErrType()->SetUM(sideband.GetUM());
            cur_err_cap->GetCrgErrType()->SetSPID(sideband.GetSPID());

            if ((cmd == DATA_READ) || (cmd == INST_READ)) {
                cur_err_cap->GetCrgErrType()->SetWrite(0x0);
            } else {
                cur_err_cap->GetCrgErrType()->SetWrite(0x1);
            }
        }
    } else {
        RegData cur_peg_spid_err = cur_err_cap->GetCrgSpidErrStat()->GetSPIDERR();
        cur_err_cap->GetCrgSpidErrStat()->SetSPIDERR(sideband.GetSPID());
        RegData new_peg_spid_err = cur_err_cap->GetCrgSpidErrStat()->GetSPIDERR();
        if (cur_peg_spid_err) {
            cur_err_cap->GetCrgErrStat()->SetOVF(0x1);
            if (cur_peg_spid_err == new_peg_spid_err) {
                return SYSERR_NONE;
            }
            // ToDo: Add implementation code for condition: Set and Clear is at the same time.
            // In this case, SPID value in GSPIDERRCLR must be kept (if spec is changed).
        } else {
            // ToDo: Add implementation code for condition: Set and Clear is at the same time.
            // In this case, SPID value in GSPIDERRCLR must be kept (if spec is changed).
            cur_err_cap->GetCrgErrStat()->SetERR(0x1);

            cur_err_cap->GetCrgErrAddr()->SetAddr(addr);

            cur_err_cap->GetCrgErrType()->SetSEC(0x0);
            cur_err_cap->GetCrgErrType()->SetDBG(sideband.GetDM());
            cur_err_cap->GetCrgErrType()->SetUM(sideband.GetUM());
            cur_err_cap->GetCrgErrType()->SetSPID(sideband.GetSPID());

            if ((cmd == DATA_READ) || (cmd == INST_READ)) {
                cur_err_cap->GetCrgErrType()->SetWrite(0x0);
            } else {
                cur_err_cap->GetCrgErrType()->SetWrite(0x1);
            }
        }
    }
    return SYSERR_CRG;

}


/*!
 * @brief Check access address is match with CRG setting.
 * @param id Channel.
 * @param addr Address.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgCtrl::IsAddressMatch (uint32_t id, PhysAddr addr, CoreType core_type, CoreVersion core_version) const
{
    uint32_t addr_valid = m_ctrl_regs->GetCrgAdv (id)->GetADV();
    uint32_t addr_comp  = m_ctrl_regs->GetCrgBad (id)->GetBAD();
    // BAD, ADV is [18-8] bit.
    // Operation: and {xor {addr_tar, addr_comp} , addr_valid };
    uint32_t addr_res   = (addr ^ addr_comp) & addr_valid;
    return (addr_res == 0x0);
}


/*!
 * @brief Check SPID is allowed in CRG setting.
 * @param id Channel.
 * @param sideband Sideband info.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgCtrl::IsSpidAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t spid_filter = m_ctrl_regs->GetCrgSpid(id)->GetSPID();
    // In Sideband, SPID is in [12:8]
    // In CRGSPID, SPID is in [31:0]
    // So, we use shift left operation to compare SPID value between sideband and CRGSPID.
    return ((spid_filter >> sideband.GetSPID ()) & 0x1) == 0x1;
}


/*!
 * @brief Check write access is allowed in CRG setting.
 * @param id Channel.
 * @param cmd Access operation.
 * @param core_type CPU core type
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgCtrl::IsWriteGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const
{
    uint32_t wg_filter;
    wg_filter = m_ctrl_regs->GetCrgProt(id)->GetWG();
    if (wg_filter == 0x1){
        if (cmd == DATA_WRITE) {
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check read access is allowed in CRG setting.
 * @param id Channel.
 * @param cmd Access operation.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgCtrl::IsReadGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const
{
    uint32_t rg_filter;
    rg_filter = m_ctrl_regs->GetCrgProt(id)->GetRG();
    if (rg_filter == 0x1){
        if ((cmd == DATA_READ) || (cmd == INST_READ)) {
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check user mode is allowed in CRG setting.
 * @param id Channel.
 * @param sideband Sideband info.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgCtrl::IsUMDisabled (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t ume_filter;
    ume_filter = m_ctrl_regs->GetCrgProt(id)->GetUME();
    if (ume_filter == 0x0){
        if (sideband.GetUM() == 0x01) {
            // UM is disabled.
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check debug mode is allowed in CRG setting.
 * @param id Channel.
 * @param sideband Sideband info.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgCtrl::IsDbgAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t dbg_filter;
    dbg_filter = m_ctrl_regs->GetCrgProt(id)->GetDBG();
    if (dbg_filter == 0x1){
        if (sideband.GetDM() == 0x1) {
            // DB is allowed.
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check one memory access is permitted in CRG setting.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param size Access size.
 * @param cmd Access operation.
 * @param core_type CPU core type.
 * @param core_version
 * @return System Error Cause code.
 *
 * @todo To be documented.
 */

SysErrCause CrgCtrl::IsMemAccPermit (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd,
                                 CoreType core_type, CoreVersion core_version)
{
    uint32_t max_channel = m_ctrl_regs->GetCrgInternal()->GetMaxCtrlChannel();
    for (uint32_t id = 0; id < max_channel; id++) {
        uint32_t crg_chn_en = m_ctrl_regs->GetCrgInternal()->IsChannelEn(id);
        if (crg_chn_en) {
            // Step 2: Check SPID and Address is valid or not.
            if (!IsAddressMatch(id, addr, core_type, core_version)) {
                // PE doesn't satisfy Address comparison.
                // Continue to check the remaining channels.
                continue;
            }

            // Step 3: Check DBG is allowed or not.
            if (IsDbgAllowed(id, sideband, core_type, core_version)) {
                // PE is debug master, so it satisfied this condition.
                // Because CRGPROTn.GEN = 1, address is match, so this access is treated as permitted.
                return SYSERR_NONE;
            }

            // Step 4: Check UME is disabled or not.
            if (IsUMDisabled(id, sideband, core_type, core_version)) {
                // PE is in user mode, and it disabled R/W access.
                // Continue to check the remaining channels.
                continue;
            }

            // Step 5: Check WG and RG are enable or not.
            if ( (IsSpidAllowed(id, sideband, core_type, core_version))
              || (IsWriteGloAllowed(id, cmd, core_type, core_version) || IsReadGloAllowed(id, cmd, core_type, core_version))) {
                // This access is permitted when each of 1 function (SPID, WG, RG) is satisfied.
                return SYSERR_NONE;
            }
        }
    }

    return SYSERR_CRG;
}


/*!
 * @brief Constructor of CRG control.
 * @param ctrl_addr_base base addres of CRG control.
 * @param ivc_addr_base
 * @param core_type G4MH core type.
 * @param core_version
 * @param guard_internal
 * @return void.
 *
 * @todo To be documented.
 */
CrgCtrl::CrgCtrl (PhysAddr ctrl_addr_base, PhysAddr ivc_addr_base,
                  CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal)
    : m_ctrl_addr_base (ctrl_addr_base),
      m_ivc_addr_base (ivc_addr_base),
      m_core_type(core_type),
      m_core_version (core_version)
{
    m_ctrl_regs = new TRegCrgCtrlReg (ctrl_addr_base, ivc_addr_base, core_type, core_version, guard_internal);
}


/*!
 * @brief Destructor of CRG control.
 * @param -
 * @return void.
 */

CrgCtrl::~CrgCtrl ()
{
    delete m_ctrl_regs;
}


/*!
 * @brief Check access address is match with CRG setting.
 * @param id Channel.
 * @param addr Address.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgSecure::IsAddressMatch (uint32_t id, PhysAddr addr, CoreType core_type, CoreVersion core_version) const
{
    uint32_t addr_valid = m_sec_regs->GetCrgAdv (id)->GetADV();
    uint32_t addr_comp  = m_sec_regs->GetCrgBad (id)->GetBAD();
    // BAD, ADV is [18-8] bit.
    // Operation: and {xor {addr_tar, addr_comp} , addr_valid };
    uint32_t addr_res   = (addr ^ addr_comp) & addr_valid;
    return (addr_res == 0x0);
}


/*!
 * @brief Check SPID is allowed in CRG setting.
 * @param id Channel.
 * @param sideband Sideband info.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgSecure::IsSpidAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t spid_filter = m_sec_regs->GetCrgSpid(id)->GetSPID();
    // In Sideband, SPID is in [12:8]
    // In CRGSPID, SPID is in [31:0]
    // So, we use shift left operation to compare SPID value between sideband and CRGSPID.
    return ((spid_filter >> sideband.GetSPID ()) & 0x1) == 0x1;
}


/*!
 * @brief Check write access is allowed in CRG setting.
 * @param id Channel.
 * @param cmd Access operation.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgSecure::IsWriteGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const
{
    uint32_t wg_filter;
    wg_filter = m_sec_regs->GetCrgProt(id)->GetWG();
    if (wg_filter == 0x1){
        if (cmd == DATA_WRITE) {
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check read access is allowed in CRG setting.
 * @param id Channel.
 * @param cmd Access operation.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgSecure::IsReadGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const
{
    uint32_t rg_filter;
    rg_filter = m_sec_regs->GetCrgProt(id)->GetRG();
    if (rg_filter == 0x1){
        if ((cmd == DATA_READ) || (cmd == INST_READ)) {
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check user mode is allowed in CRG setting.
 * @param id Channel.
 * @param sideband Sideband info.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgSecure::IsUMDisabled (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t ume_filter;
    ume_filter = m_sec_regs->GetCrgProt(id)->GetUME();
    if (ume_filter == 0x0){
        if (sideband.GetUM() == 0x01) {
            // UM is disabled.
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check debug mode is allowed in CRG setting.
 * @param id Channel.
 * @param sideband Sideband info.
 * @param core_type CPU core type.
 * @param core_version
 * @return boolean value.
 *
 * @todo To be documented.
 */

bool CrgSecure::IsDbgAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t dbg_filter;
    dbg_filter = m_sec_regs->GetCrgProt(id)->GetDBG();
    if (dbg_filter == 0x1){
        if (sideband.GetDM() == 0x1) {
            // DB is allowed.
            return true;
        } else {
            return false;
        }
    }else{
        return false;
    }
}


/*!
 * @brief Check one memory access is permitted in CRG setting.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param size Access size.
 * @param cmd Access operation.
 * @param core_type CPU core type.
 * @param core_version
 * @param channel
 * @return System Error Cause code.
 *
 * @todo To be documented.
 */

SysErrCause CrgSecure::IsMemAccPermit (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd,
                                 CoreType core_type, CoreVersion core_version, uint32_t channel)
{

    // Step 3: Check DBG is allowed or not.
    if (IsDbgAllowed(channel, sideband, core_type, core_version)) {
        // PE is debug master, so it satisfied this condition.
        // Because CRGPROTn.GEN = 1, address is match, so this access is treated as permitted.
        return SYSERR_NONE;
    }

    // Step 4: Check UME is disabled or not.
    if (IsUMDisabled(channel, sideband, core_type, core_version)) {
        // PE is in user mode, and it disabled R/W access.
        // Continue to check the remaining channels.
        return SYSERR_NONE;
    }

    // Step 5: Check WG and RG are enable or not.
    if ( (IsSpidAllowed(channel, sideband, core_type, core_version))
      || (IsWriteGloAllowed(channel, cmd, core_type, core_version) || IsReadGloAllowed(channel, cmd, core_type, core_version))) {
        // This access is permitted when each of 1 function (SPID, WG, RG) is satisfied.
        return SYSERR_NONE;
    }

    return SYSERR_CRG;
}


/*!
 * @brief Constructor of CRG control.
 * @param reg_addr_base Base addres of CRG control.
 * @param core_type G4MH core type.
 * @param core_version
 * @param guard_internal
 * @return void.
 *
 * @todo To be documented.
 */
CrgSecure::CrgSecure (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version, GuardInternal_Interface *guard_internal)
    : m_reg_addr_base (reg_addr_base),
      m_core_type(core_type),
      m_core_version (core_version)
{
    m_sec_regs = new TRegCrgSecureReg (reg_addr_base, core_type, core_version, guard_internal);
}


/*!
 * @brief Destructor of CRG control.
 * @param -
 * @return void.
 */

CrgSecure::~CrgSecure ()
{
    delete m_sec_regs;
}


/*!
 * @brief Constructor of CRG capture.
 * @param reg_addr_base Base addres of CRG capture.
 * @param core_type G4MH core type.
 * @param core_version
 * @return void.
 *
 * @todo To be documented.
 */

CrgCap::CrgCap (const PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version)
    : m_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    m_err_regs = new TRegCrgErrReg (reg_addr_base, core_type, core_version);
}


/*!
 * @brief Destructor of CRG capture.
 * @param -
 * @return void.
 */

CrgCap::~CrgCap ()
{
    delete m_err_regs;
}


/*!
 * @brief Constructor of CRG
 * @param ctrl_addr_base Base addres of CRG.
 * @param ivc_addr_base
 * @param csg_addr_base
 * @param core_type G4MH core type.
 * @param core_version
 * @param clid
 * @return void.
 *
 * @todo To be documented.
 */

Crg::Crg (const PhysAddr ctrl_addr_base, const PhysAddr ivc_addr_base, const PhysAddr csg_addr_base,
          CoreType core_type, CoreVersion core_version, uint32_t clid)
    : m_crtl_addr_base (ctrl_addr_base),
      m_ivc_addr_base (ivc_addr_base),
      m_sec_addr_base (csg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version),
      m_clid (clid)
{
    // Default variable in Guard Internal instance.
    bool crg_guard_write_en   = false;
    bool crg_guard_setting_en = false;

    uint32_t crg_maddwr = GuardInternal_Interface::CRG_PMADDRW_G4MH2X;
#if _UNIT_TEST
    bool ret = true;
#else
    bool ret = p_forest_util->GetGuardParam (GMID_CRG_CLm, m_clid, GPID_CRG_ADDR_WIDTH, &crg_maddwr);
#endif
    if (ret == true) {
        // OK: "pmaddrw" will have valid value.
        CfMsg::DPrint (MSG_INF, "<Guard: pMADDRW       = %d is set in CRG>\n", crg_maddwr);
    } else {
        // NG: error occurs. "pmaddrw" is invalid.
        CfMsg::DPrint (MSG_WAR, "<Warning: pMADDRW is invalid. Use default value 19 in CRG>\n");
    }

    // There are 8 channels in CRG control registers.
    uint32_t max_guard_channel = GuardInternal_Interface::NUM_CRG_CTRL_CHANNEL;
#if _UNIT_TEST
    ret = true;
#else
    ret = p_forest_util->GetGuardParam (GMID_CRG_CLm, m_clid, GPID_CRG_CRG_CH_NUM, &max_guard_channel);
#endif
    if (ret == true) {
        // OK: "max channel" will have valid value.
        CfMsg::DPrint (MSG_INF, "<Guard: max_channel   = %d is set in CRG>\n", max_guard_channel);
    } else {
        // NG: error occurs. "max channel" is invalid.
        CfMsg::DPrint (MSG_WAR, "<Warning: Max channel is invalid. Use default value 8 channels in CRG>\n");
    }

    uint32_t max_csg_channel = GuardInternal_Interface::NUM_CRG_SEC_CHANNEL;
#if _UNIT_TEST
    ret = true;
#else
    ret = p_forest_util->GetGuardParam (GMID_CRG_CLm, m_clid, GPID_CRG_CSG_CH_NUM, &max_csg_channel);
#endif
    if (ret == true) {
        // OK: "value" will have valid value.
        CfMsg::DPrint (MSG_INF, "<Guard: max_channel   = %d is set in CSG>\n", max_csg_channel);
    } else {
        // NG: error occurs. "value" is invalid.
        CfMsg::DPrint (MSG_WAR, "<Warning: Max channel is invalid.. Use default value 4 channels in CSG>\n", max_csg_channel);
    }

    uint32_t guard_keycode = GuardInternal_Interface::GUARD_KEYCODE;
#if _UNIT_TEST
    ret = true;
#else
    ret = p_forest_util->GetGuardParam (GMID_CRG_CLm, m_clid, GPID_CRG_KEYCODE, &guard_keycode);
#endif
    if (ret == true) {
        // OK: "keycode" will have valid value.
        CfMsg::DPrint (MSG_INF, "<Guard: Guard keycode = 0x%x is set in CRG>\n", guard_keycode);
    } else {
        // NG: error occurs. "keycode" is invalid.
        CfMsg::DPrint (MSG_WAR, "<Warning: Guard keycode is not set. Use default keycode 0xA5A5A500 in CRG>\n");
    }

    // clid is exist in CRG instance.

    // Define the necessary variables for CRG internal..
    GuardInternal *crg_internal = new GuardInternal (crg_guard_write_en, crg_guard_setting_en, crg_maddwr,
                                                     max_guard_channel, max_guard_channel, max_csg_channel,
                                                     guard_keycode,      m_clid);
    if ((m_core_type == CORE_G4MH) && (m_core_version < CORE_VER_20)) {
        // There is no IVC and CSG module in G4MH1x
        m_crg_ctrl_regs = new CrgCtrl   (ctrl_addr_base, 0x0          , core_type, core_version, crg_internal);
    } else {
        m_crg_ctrl_regs = new CrgCtrl   (ctrl_addr_base, ivc_addr_base, core_type, core_version, crg_internal);
        m_crg_sec_regs  = new CrgSecure (csg_addr_base ,                core_type, core_version, crg_internal);
    }

    m_guard = new CrGuard (this);
}


/*!
 * @brief Destructor of CRG.
 * @param -
 * @return void.
 */

Crg::~Crg () {
    delete m_guard;
    if ((m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
        delete m_crg_sec_regs;
    }
    delete m_crg_ctrl_regs;
}
