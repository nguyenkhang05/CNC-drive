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
#include "./peg.h"
#include "./sideband.h"
#if _UNIT_TEST
#include "mock_guard_base.h"
#else
#include "./forest_internals.h"
#include "./forest_utility.h"
#endif


/*!
 * @brief Constructor of PEG control registers.
 * @param reg_addr_base address base of PEG control register
 * @param core_type G4MH core type
 * @param core_version  version of core
 */

TRegPegCtrlReg::TRegPegCtrlReg (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version)
    : m_peg_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    // Default variable in Guard Internal instance.
    bool peg_guard_write_en   = false;
    bool peg_guard_setting_en = false;
    // These variables are not used in PEG.
    uint32_t ivc_channel   = 0x0;
    uint32_t csg_channel   = 0x0;
    uint32_t peg_maddwr    = 0x0;

    // There are 8 channels in CRG control registers.
    m_peg_num_channel   = (core_version >= CORE_VER_20) ? NUM_PEG_CTRL_CHANNEL_G4MH2X : NUM_PEG_CTRL_CHANNEL_G4MH1X;

    uint32_t guard_keycode = GuardInternal_Interface::GUARD_KEYCODE;
    uint32_t m_clid        = 0x0;

    // Define the necessary variables for CRG internal..
    m_peg_internal = new GuardInternal (peg_guard_write_en, peg_guard_setting_en, peg_maddwr,
                                        m_peg_num_channel,  ivc_channel,          csg_channel,
                                        guard_keycode,      m_clid);

    PhysAddr pegkcprot_addr = m_peg_reg_addr_base + PEGKCPROT_OFFSET;
    m_pegkceprot = new GKCPROT (SIZE_32BIT, SIZE_32BIT, pegkcprot_addr, "PEGKCEPROT", m_peg_internal);
    m_trf.insert (TRegMap::value_type (pegkcprot_addr, m_pegkceprot));

    for (uint32_t id = 0; id < m_peg_num_channel; ++id) {
        PhysAddr pegprot_addr = m_peg_reg_addr_base + PEGPROT_OFFSET + PEG_CTRL_CHANNEL_RANGE * id;
        std::ostringstream pegprot_name;
        pegprot_name << "PEGPROT" << id;
        GPROT* pegprot_ptr = new GPROT (SIZE_32BIT, SIZE_8BIT, pegprot_addr, id, pegprot_name.str(),
                                        PEG_MODULE, core_type, core_version, m_peg_internal);
        m_trf.insert (TRegMap::value_type (pegprot_addr, pegprot_ptr));
        m_pegprot.push_back (pegprot_ptr);

        PhysAddr pegspid_addr = m_peg_reg_addr_base + PEGSPID_OFFSET + PEG_CTRL_CHANNEL_RANGE * id;
        std::ostringstream pegspid_name;
        pegspid_name << "PEGSPID" << id;
        GSPID* pegspid_ptr = new GSPID (SIZE_32BIT, SIZE_8BIT, pegspid_addr, id, pegspid_name.str(),
                                        PEG_MODULE, core_type, core_version, m_peg_internal);
        m_trf.insert (TRegMap::value_type (pegspid_addr, pegspid_ptr));
        m_pegspid.push_back (pegspid_ptr);

        // pMADDRW is not used in PEG module.
        PhysAddr pegbad_addr = m_peg_reg_addr_base + PEGBAD_OFFSET + PEG_CTRL_CHANNEL_RANGE * id;
        std::ostringstream pegbad_name;
        pegbad_name << "PEGBAD" << id;
        GBAD* pegbad_ptr = new GBAD (SIZE_32BIT, SIZE_32BIT, pegbad_addr, id, pegbad_name.str(),
                                     0x0, PEG_MODULE, core_type, core_version, m_peg_internal);
        m_trf.insert (TRegMap::value_type (pegbad_addr, pegbad_ptr));
        m_pegbad.push_back (pegbad_ptr);

        PhysAddr pegadv_addr = m_peg_reg_addr_base + PEGADV_OFFSET + PEG_CTRL_CHANNEL_RANGE * id;
        std::ostringstream pegadv_name;
        pegadv_name << "PEGADV" << id;
        GADV* pegadv_ptr = new GADV (SIZE_32BIT, SIZE_32BIT, pegadv_addr, id, pegadv_name.str(),
                                     0x0, PEG_MODULE, core_type, core_version, m_peg_internal);
        m_trf.insert (TRegMap::value_type (pegadv_addr, pegadv_ptr));
        m_pegadv.push_back (pegadv_ptr);
    }
}


/*!
 * @brief Destructor of PEG control registers.
 * @param -
 */

TRegPegCtrlReg::~TRegPegCtrlReg ()
{
    // This function also deletes the pointers of m_pegadv, m_pegbad, m_pegspid, m_pegprot, m_pegkceprot.
    DelTRegMap ();
    delete m_peg_internal;
}


/*!
 * @brief Constructor of PEG error registers.
 * @param reg_addr_base address base of PEG error register
 * @param core_type G4MH core type.
 * @param core_version version of core
 */

TRegPegErrReg::TRegPegErrReg (PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version)
    : m_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    if ((core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
        // Add error register in case master PEG capture
        // For G4MH1x.
        PhysAddr pegerrstat_addr = m_reg_addr_base + PEGERRSTAT_OFFSET;
        m_pegerrstat = new GERRSTAT (SIZE_32BIT, SIZE_32BIT, pegerrstat_addr, "PEGERRSTAT", core_type, core_version);
        m_trf.insert (TRegMap::value_type (pegerrstat_addr, m_pegerrstat));

        PhysAddr pegerrclr_addr_m = m_reg_addr_base + PEGERRCLR_OFFSET;
        m_pegerrclr = new GERRCLR (SIZE_32BIT, SIZE_8BIT, pegerrclr_addr_m, m_pegerrstat, "PEGERRCLR", core_type, core_version);
        m_trf.insert (TRegMap::value_type (pegerrclr_addr_m, m_pegerrclr));

        PhysAddr pegerraddr_addr = m_reg_addr_base + PEGERRADDR_OFFSET;
        m_pegerraddr = new GERRADDR (SIZE_32BIT, SIZE_32BIT, pegerraddr_addr, "PEGERRADDR");
        m_trf.insert (TRegMap::value_type (pegerraddr_addr, m_pegerraddr));

        PhysAddr pegerrtype_addr = m_reg_addr_base + PEGERRTYPE_OFFSET;
        m_pegerrtype = new GERRTYPE (SIZE_32BIT, SIZE_32BIT, pegerrtype_addr, "PEGERRTYPE");
        m_trf.insert (TRegMap::value_type (pegerrtype_addr, m_pegerrtype));
    } else {
        // Add error register in case master PEG capture
        // For G4MH2x and later.

        // Name is different in G4MH2x
        PhysAddr pegerrstat_addr = m_reg_addr_base + PEGERRSTAT_OFFSET;
        m_pegerrstat = new GERRSTAT (SIZE_32BIT, SIZE_8BIT, pegerrstat_addr, "PEGOVFSTAT", core_type, core_version);
        m_trf.insert (TRegMap::value_type (pegerrstat_addr, m_pegerrstat));

        // Name is different in G4MH2x
        PhysAddr pegerrclr_addr_m = m_reg_addr_base + PEGERRCLR_OFFSET;
        m_pegerrclr = new GERRCLR (SIZE_32BIT, SIZE_8BIT, pegerrclr_addr_m, m_pegerrstat, "PEGOVFCLR", core_type, core_version);
        m_trf.insert (TRegMap::value_type (pegerrclr_addr_m, m_pegerrclr));

        PhysAddr pegerraddr_addr = m_reg_addr_base + PEGERRADDR_OFFSET;
        m_pegerraddr = new GERRADDR (SIZE_32BIT, SIZE_32BIT, pegerraddr_addr, "PEGERRADDR");
        m_trf.insert (TRegMap::value_type (pegerraddr_addr, m_pegerraddr));

        PhysAddr pegerrtype_addr = m_reg_addr_base + PEGERRTYPE_OFFSET;
        m_pegerrtype = new GERRTYPE (SIZE_32BIT, SIZE_16BIT, pegerrtype_addr, "PEGERRTYPE");
        m_trf.insert (TRegMap::value_type (pegerrtype_addr, m_pegerrtype));

        PhysAddr pegspiderrstat_addr = m_reg_addr_base + PEGSPIDERRSTAT_OFFSET;
        m_pegspiderrstat = new GSPIDERRSTAT (SIZE_32BIT, SIZE_8BIT, pegspiderrstat_addr, "PEGSPIDERRSTAT", core_type, core_version);
        m_trf.insert (TRegMap::value_type (pegspiderrstat_addr, m_pegspiderrstat));

        PhysAddr pegspiderrclr_addr_m = m_reg_addr_base + PEGSPIDERRCLR_OFFSET;
        m_pegspiderrclr = new GSPIDERRCLR (SIZE_32BIT, SIZE_8BIT, pegspiderrclr_addr_m, m_pegspiderrstat, "PEGSPIDERRCLR", core_type, core_version);
        m_trf.insert (TRegMap::value_type (pegspiderrclr_addr_m, m_pegspiderrclr));
    }
}


/*!
 * @brief Destructor of PEG error registers.
 * @param -
 */

TRegPegErrReg::~TRegPegErrReg ()
{
    // This function also deletes the pointers of m_pegerrstat, m_pegerrclr, m_pegerraddr, m_pegerrtype.
    DelTRegMap ();
}


/*!
 * @brief Call specified PEG control register base on address to check write condition.
 * @param sideband sideband info
 * @param addr address
 * @param data written data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCtrl::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->Write (sideband, addr, data, size);

    // At least, 1 PEGPROT.GEN is updated to 0.
    // Need to check PEGPROT.GEN in all channels at once.
    m_ctrl_regs->GetPegInternal()->SetGuardEn(false);
    for (uint32_t ch = 0; ch < m_ctrl_regs->m_peg_num_channel; ch++) {
        // PEGPROT.GEN is bit 8.
        if (m_ctrl_regs->GetPegProt (ch)->GetGEN () == 0x1) {
            m_ctrl_regs->GetPegInternal()->SetGuardEn(true);
            m_ctrl_regs->GetPegInternal()->SetChannelEn(ch, true);
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG control register base on address to check read condition.
 * @param sideband sideband info
 * @param addr address
 * @param data read data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCtrl::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG control register base on address to write new data by Debug method.
 * @param addr address
 * @param data written data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCtrl::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->WriteBody (addr, data, size);

    // At least, 1 PEGPROT.GEN is updated to 0.
    // Need to check PEGPROT.GEN in all channels at once.
    m_ctrl_regs->GetPegInternal()->SetGuardEn(false);
    for (uint32_t ch = 0; ch < m_ctrl_regs->m_peg_num_channel; ch++) {
        // PEGPROT.GEN is bit 8.
        if (m_ctrl_regs->GetPegProt (ch)->GetGEN () == 0x1) {
            m_ctrl_regs->GetPegInternal()->SetGuardEn(true);
            m_ctrl_regs->GetPegInternal()->SetChannelEn(ch, true);
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG control register base on address to read data by Debug method.
 * @param addr address
 * @param data read data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCtrl::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_ctrl_regs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Update all PEG control register to reset status.
 * @param -
 */

void PegCtrl::HardReset (void)
{
    m_ctrl_regs->GetPegInternal()->Reset();
    m_ctrl_regs->ResetTRegs();
}


/*!
 * @brief Call specified PEG capture register base on address to check write condition.
 * @param sideband sideband info
 * @param addr address
 * @param data written data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCap::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_err_regs->GetTrPtr(addr)->Write (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG capture register base on address to check read condition.
 * @param sideband sideband info
 * @param addr address
 * @param data read data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCap::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_err_regs->GetTrPtr(addr)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG capture register base on address to write new data by Debug method.
 * @param addr address
 * @param data written data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCap::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (addr == m_reg_addr_base + TRegPegErrReg::PEGERRCLR_OFFSET) {
        m_err_regs->GetTrPtr(addr)->WriteBody (addr, data, size);
    }
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG capture register base on address to read data by Debug method.
 * @param addr address
 * @param data read data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause PegCap::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_err_regs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Update all PEG capture register to reset status.
 * @param -
 */

void PegCap::HardReset (void)
{
    m_err_regs->ResetTRegs();
}


/*!
 * @brief Call specified PEG register base on address to check write condition.
 * @param sideband sideband info
 * @param addr address
 * @param data written data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause Peg::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_peg_ctrl_regs->TargetWrite (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG register base on address to check read condition.
 * @param sideband sideband info
 * @param addr address
 * @param data read data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause Peg::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_peg_ctrl_regs->TargetRead (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG register base on address to write new data by Debug method.
 * @param addr address
 * @param data written data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause Peg::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_peg_ctrl_regs->TargetWriteDebug (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Call specified PEG register base on address to read data by Debug method.
 * @param addr address
 * @param data read data
 * @param size access size
 * @return SYSERR_NONE
 */

SysErrCause Peg::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_peg_ctrl_regs->TargetReadDebug (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Update all PEG register to reset status.
 * @param -
 */

void Peg::HardReset (void)
{
    m_peg_ctrl_regs->HardReset();
}


/*!
 * @brief Check address belongs to speculative area.
 * @param addr address
 * @return boolean value
 */

#if _UNIT_TEST
bool Peg::IsSPCAddr (PhysAddr addr) const
{
    return (addr >= L1RAM_E2X_START) && (addr <= L1RAM_E2X_END);
}
#endif


/*!
 * @brief Check access address is match with PEG setting.
 * @param id channel
 * @param addr address
 * @param core_type CPU core type
 * @param core_version version of core
 * @return boolean value
 */

bool Peg::IsAddressMatch (uint32_t id, PhysAddr addr, CoreType core_type, CoreVersion core_version) const
{
    uint32_t addr_valid = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegAdv (id)->GetADV();
    uint32_t addr_comp  = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegBad (id)->GetBAD();
    // BAD, ADV is [25-12] bit.
    // Operation: and {xor {addr_tar, addr_comp} , addr_valid };
    uint32_t addr_res   = (addr ^ addr_comp) & addr_valid;
    return (addr_res == 0x0);
}


/*!
 * @brief Check SPID is allowed in PEG setting.
 * @param id channel
 * @param sideband sideband info
 * @param core_type CPU core type
 * @param core_version version of core
 * @return boolean value
 */

bool Peg::IsSpidAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t spid_filter = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegSpid(id)->GetSPID();
    // In Sideband, SPID is in [12:8]
    // In PEGSPID, SPID is in [31:0]
    // So, we use shift left operation to compare SPID value between sideband and PEGSPID.
    return ((spid_filter >> sideband.GetSPID ()) & 0x1) == 0x1;
}


/*!
 * @brief Check write access is allowed in PEG setting.
 * @param id channel
 * @param cmd access operation
 * @param core_type CPU core type
 * @param core_version version of core
 * @return boolean value
 */

bool Peg::IsWriteGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const
{
    uint32_t wg_filter;
    wg_filter = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegProt(id)->GetWG();
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
 * @brief Check read access is allowed in PEG setting.
 * @param id channel
 * @param cmd access operation
 * @param core_type CPU core type
 * @param core_version version of core
 * @return boolean value
 */

bool Peg::IsReadGloAllowed (uint32_t id, MemAccessCommand cmd, CoreType core_type, CoreVersion core_version) const
{
    uint32_t rg_filter;
    rg_filter = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegProt(id)->GetRG();
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
 * @brief Check user mode is allowed in PEG setting.
 * @param id channel
 * @param sideband sideband info
 * @param core_type CPU core type
 * @param core_version version of core
 * @return boolean value
 */

bool Peg::IsUMDisabled (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t ume_filter;
    ume_filter = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegProt(id)->GetUME();
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
 * @brief Check debug mode is allowed in PEG setting.
 * @param id channel
 * @param sideband sideband info
 * @param core_type CPU core type
 * @param core_version version of core
 * @return boolean value
 */

bool Peg::IsDbgAllowed (uint32_t id, SideBand sideband, CoreType core_type, CoreVersion core_version) const
{
    uint32_t dbg_filter;
    dbg_filter = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegProt(id)->GetDBG();
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
 * @brief Check one memory access is permitted in PEG setting.
 * @param sideband sideband info
 * @param addr address
 * @param size access size
 * @param cmd access operation
 * @param core_type CPU core type
 * @param core_version version of core
 * @return System Error Cause code
 */

SysErrCause Peg::IsMemAccPermit (SideBand sideband, PhysAddr addr, ByteSize size, MemAccessCommand cmd,
                                 CoreType core_type, CoreVersion core_version)
{
    // Step 1: Check PEGPROTn.GEN at 4 channels.
    // This step is done at PeGuard::IsAccPermited.
    if (!m_peg_ctrl_regs->GetPegCtrlReg()->GetPegInternal()->IsGuardEn()) {
        return SYSERR_NONE;
    }

    // In G4MH PEG, RWM instruction is treated as write type.
    // So, in case read type, RMW instruction (include NOT1/CLR1/SET1/CAXI) is ignored.
    if ((sideband.GetBitOp() != SideBand::BITOP_NONE) && (cmd == DATA_READ)) {
        return SYSERR_NONE;
    }
#if _UNIT_TEST
        // To do something;
    uint32_t peid_acc = 0x0;
#else
    uint32_t peid_acc = p_forest_util->GetPeidFromBmid(sideband.GetBMID());
#endif

    if (m_peid == peid_acc) {
        // In case self PE access, only INST_READ type is checked.
        if (cmd != INST_READ) {
            return SYSERR_NONE;
        }
    }

#if _UNIT_TEST
#else
    SysErrCause peg_capture_cause = SYSERR_PEG_S;
    if (sideband.GetSPCLD() == 1) {
        // In case access type is DATA_READ and addr is in L1RAM,
        // SPCLD is 1 and PEG error is captured by PEGCAP_M.
        // In summary:
        // L1RAM, INST_READ: SPCLD = 0.
        // INTC1, INST_READ: SPCLD = 0.
        // L1RAM, DATA_READ: SPCLD = 1.
        // INTC1, DATA_READ: SPCLD = 0.
        peg_capture_cause = SYSERR_PEG_M;
    }
#endif

    uint32_t max_channel = m_peg_ctrl_regs->GetPegCtrlReg()->m_peg_num_channel;
    for (uint32_t id = 0; id < max_channel; id++) {
        bool peg_chn_en = m_peg_ctrl_regs->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(id);
        if (peg_chn_en) {
            // Step 2: Check SPID and Address is valid or not.
            if (!IsAddressMatch(id, addr, core_type, core_version)) {
                // PE doesn't satisfy Address comparation.
                // Continue to check the remaining channels.
                continue;
            }

            // Step 3: Check DBG is allowed or not.
            if (IsDbgAllowed(id, sideband, core_type, core_version)) {
                // PE is debug master, so it satisfied this condition.
                // Because PEGPROTn.GEN = 1, address is match, so this access is treated as permitted.
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

#if _UNIT_TEST
        // To do something;
    return SYSERR_PEG;
#else
    return CauseError (sideband, addr, cmd, core_type, core_version, peg_capture_cause);
#endif
}


/*!
 * @brief Update PEG capture registers when a memory access is not permitted.
 * @param sideband sideband info
 * @param addr address
 * @param cmd access operation
 * @param core_type CPU core type
 * @param core_version version of core
 * @param peg_capture_cause system error cuase
 * @return System Error Cause code
 */

SysErrCause Peg::CauseError (SideBand sideband, PhysAddr addr, MemAccessCommand cmd,
                             CoreType core_type, CoreVersion core_version, SysErrCause peg_capture_cause)
{
    SysErrCause m_err_cause = peg_capture_cause;
#if _UNIT_TEST
        // To do something;
    uint32_t peid_acc = 0x0;
    TRegPegErrReg *cur_err_cap = new TRegPegErrReg(0xFFFFFFFF, CORE_G4MH, core_version);
#else
    uint32_t peid_acc = p_forest_util->GetPeidFromBmid(sideband.GetBMID());
    TRegPegErrReg *cur_err_cap = NULL;
#endif
    if (m_peid == peid_acc) {
        // In case self PE access, only INST_READ type will be reported and checked in CauseError.
        // The other cases are checked in IsMemAccPermit function.
        if (cmd == INST_READ) {
            m_err_cause = SYSERR_PEG_S;
#if _UNIT_TEST
        // To do something;
            cur_err_cap = new TRegPegErrReg (0xFFC6C830, CORE_G4MH, core_version);
#else
            cur_err_cap = static_cast <TRegPegErrReg *> (p_internals->GetGuardCapErr(GMID_PEGCAP_S_PEnCLm, m_peid));
#endif
        } else {
#if _UNIT_TEST
        // To do something;
            m_peg_err = cur_err_cap;
#endif
            return SYSERR_NONE;
        }
#if _UNIT_TEST
        // To do something;
    } else if (peid_acc == 0x0) {
#else
    } else if (p_forest_util->IsValidPeId(peid_acc)) {
#endif
        if (m_err_cause == SYSERR_PEG_M) {
#if _UNIT_TEST
        // To do something;
            cur_err_cap = new TRegPegErrReg (0xFFC6C820, CORE_G4MH, core_version);
#else
            cur_err_cap = static_cast <TRegPegErrReg *> (p_internals->GetGuardCapErr(GMID_PEGCAP_M_PEnCLm, peid_acc));
#endif
        } else {
#if _UNIT_TEST
        // To do something;
            cur_err_cap = new TRegPegErrReg (0xFFC6C830, CORE_G4MH, core_version);
#else
            cur_err_cap = static_cast <TRegPegErrReg *> (p_internals->GetGuardCapErr(GMID_PEGCAP_S_PEnCLm, m_peid));
#endif
        }
    } else {
        // Unknown busmaster. It will be peripherals.
        // In case peripherals access, only Slave Error Capture Module will record information.
        m_err_cause = SYSERR_PEG_S;
#if _UNIT_TEST
        // To do something;
        cur_err_cap = new TRegPegErrReg (0xFFC6C830, CORE_G4MH, core_version);
#else
        cur_err_cap = static_cast <TRegPegErrReg *> (p_internals->GetGuardCapErr(GMID_PEGCAP_S_PEnCLm, m_peid));
#endif
    }

#if _UNIT_TEST
        // To do something;
    if (m_peg_err != NULL)
        cur_err_cap = m_peg_err;
    else
        m_peg_err = cur_err_cap;
#endif
    // Update PEG Error registers.
    if ((core_type == CORE_G4MH) && (core_version < CORE_VER_20)) {
        if (cur_err_cap->GetPegErrStat()->GetERR()) {
            cur_err_cap->GetPegErrStat()->SetOVF(0x1);
        } else {
            // The value of error info: Addr, SEC, DBG, UM, SPID, WRITE
            // are only stored if ERR = 0 when there is error in PEG.
            cur_err_cap->GetPegErrStat()->SetERR(0x1);
            cur_err_cap->GetPegErrAddr()->SetAddr(addr);

            cur_err_cap->GetPegErrType()->SetSEC(0x0);
            cur_err_cap->GetPegErrType()->SetDBG(sideband.GetDM());
            cur_err_cap->GetPegErrType()->SetUM(sideband.GetUM());
            cur_err_cap->GetPegErrType()->SetSPID(sideband.GetSPID());

            if ((cmd == DATA_READ) || (cmd == INST_READ)) {
                cur_err_cap->GetPegErrType()->SetWrite(0x0);
            } else {
                cur_err_cap->GetPegErrType()->SetWrite(0x1);
            }
        }
    } else {
        RegData cur_peg_spid_err = cur_err_cap->GetPegSpidErrStat()->GetSPIDERR();
        cur_err_cap->GetPegSpidErrStat()->SetSPIDERR(sideband.GetSPID());
        RegData new_peg_spid_err = cur_err_cap->GetPegSpidErrStat()->GetSPIDERR();
        if (cur_peg_spid_err) {
            cur_err_cap->GetPegErrStat()->SetOVF(0x1);
            if (cur_peg_spid_err == new_peg_spid_err) {
                m_err_cause = SYSERR_NONE;
            }
            // ToDo: Add implementation code for condition: Set and Clear is at the same time.
            // In this case, SPID value in GSPIDERRCLR must be kept (if spec is changed).
        } else {
            // ToDo: Add implementation code for condition: Set and Clear is at the same time.
            // In this case, SPID value in GSPIDERRCLR must be kept (if spec is changed).
            cur_err_cap->GetPegErrStat()->SetERR(0x1);

            cur_err_cap->GetPegErrAddr()->SetAddr(addr);

            cur_err_cap->GetPegErrType()->SetSEC(0x0);
            cur_err_cap->GetPegErrType()->SetDBG(sideband.GetDM());
            cur_err_cap->GetPegErrType()->SetUM(sideband.GetUM());
            cur_err_cap->GetPegErrType()->SetSPID(sideband.GetSPID());

            if ((cmd == DATA_READ) || (cmd == INST_READ)) {
                cur_err_cap->GetPegErrType()->SetWrite(0x0);
            } else {
                cur_err_cap->GetPegErrType()->SetWrite(0x1);
            }
        }
    }


    return m_err_cause;

}


/*!
 * @brief Constructor of PEG control.
 * @param peid id of PE
 * @param reg_addr_base base addres of PEG control
 * @param core_type G4MH core type
 * @param core_version version of core
 */

PegCtrl::PegCtrl (uint32_t peid, const PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version)
    : m_peid (peid),
      m_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    m_ctrl_regs = new TRegPegCtrlReg (reg_addr_base, core_type, core_version);
}


/*!
 * @brief Destructor of PEG control.
 * @param -
 */

PegCtrl::~PegCtrl ()
{
    delete m_ctrl_regs;
}


/*!
 * @brief Constructor of PEG capture.
 * @param peid id of PE
 * @param reg_addr_base base addres of PEG capture
 * @param core_type G4MH core type
 * @param core_version version of core
 */

PegCap::PegCap (uint32_t peid, const PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version)
    : m_peid (peid),
      m_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    m_err_regs = new TRegPegErrReg (reg_addr_base, core_type, core_version);
}


/*!
 * @brief Destructor of PEG capture.
 * @param -
 */

PegCap::~PegCap ()
{
    delete m_err_regs;
}


/*!
 * @brief Constructor of PEG
 * @param peid id of PE
 * @param reg_addr_base base addres of PEG
 * @param core_type G4MH core type
 * @param core_version version of core
 */

Peg::Peg (uint32_t peid, const PhysAddr reg_addr_base, CoreType core_type, CoreVersion core_version)
    : m_peid (peid),
      m_reg_addr_base (reg_addr_base),
      m_core_type (core_type),
      m_core_version (core_version)
{
    m_peg_ctrl_regs = new PegCtrl (peid, reg_addr_base, core_type, core_version);
    m_guard = new PeGuard (this);
}


/*!
 * @brief Destructor of PEG.
 * @param -
 */

Peg::~Peg () {
    delete m_peg_ctrl_regs;
    delete m_guard;
}

