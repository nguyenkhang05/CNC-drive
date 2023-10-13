/*
 * (c) 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./treg_guard.h"
#include "./forest_internals.h"


/*!
 * @brief Checking Write condition in Guard key code protection register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GKCPROT::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard key code protection register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GKCPROT::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size == SIZE_32BIT) {
        // Check KCE protection key.
        TRegData32 kce_key;
        kce_key.Write32(addr, data, m_reg_mask);
#if _UNIT_TEST
//        printf ("addr %x, size %d \n", addr, size);
        uint32_t guard_keycode = 0xA5A5A500UL;
#else
        uint32_t guard_keycode = (m_guard_internal->GetKeyCode()) & 0xFFFFFFFEUL;
#endif
        const uint32_t guard_en =                                   0x00000001UL;

        if ((kce_key.m_data32 & 0xFFFFFFFEUL) == guard_keycode) {
            SetKCE(kce_key.m_data32);
            // Update Guard Internal
            if ((kce_key.m_data32 & guard_en) == guard_en) {
                m_guard_internal->SetWriteEn(true);
            } else {
                m_guard_internal->SetWriteEn(false);
            }
        }
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 32BIT
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard control register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GPROT::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
#if _UNIT_TEST
        // Check KCE value in KCPROT.
        // Write value 0xA5A5A501U to enable Guard Settings modification.
#else
        if ((m_guard_internal->IsWriteEn() == false) || GetLOCK()) {
            return SYSERR_NONE;
        }
#endif
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard control register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GPROT::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size == SIZE_32BIT) {
        if ((m_guard_module == CRG_MODULE)
                && (m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
            // OW is bit 16
            SetOW((data[2] >> 0) & 0x1);
        } else if ((m_guard_module == CSG_MODULE)
                && (m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
            // LOCK is bit 31
            SetLOCK((data[3] >> 7) & 0x1);
        }
        // GEN is bit 8.
        SetGEN ((data[1] >> (8-8)) & 0x1);
        SetDBG ((data[0] >> 6) & 0x1);
        SetUME ((data[0] >> 4) & 0x1);
        SetWG  ((data[0] >> 1) & 0x1);
        SetRG  ((data[0] >> 0) & 0x1);
    } else if (size == SIZE_16BIT) {
        if ((addr & 0x3) == 0x0) {
            // GEN is bit 8.
            SetGEN ((data[1] >> (8-8)) & 0x1);
            SetDBG ((data[0] >> 6) & 0x1);
            SetUME ((data[0] >> 4) & 0x1);
            SetWG  ((data[0] >> 1) & 0x1);
            SetRG  ((data[0] >> 0) & 0x1);
        } else {                               // (addr & 0x3) == 0x2
            if ((m_guard_module == CRG_MODULE)
                    && (m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
                // OW is bit 16
                SetOW((data[0] >> 0) & 0x1);
            } else if ((m_guard_module == CSG_MODULE)
                    && (m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
                // LOCK is bit 31
                SetLOCK((data[1] >> 7) & 0x1);
            }
        }
    } else if (size == SIZE_8BIT) {
        // Get data in case 8bit.
        uint8_t wdata = *data;
        if ((addr & 0x3) == 0x0) {
            SetDBG ((wdata >> 6) & 0x1);
            SetUME ((wdata >> 4) & 0x1);
            SetWG  ((wdata >> 1) & 0x1);
            SetRG  ((wdata >> 0) & 0x1);
        } else if ((addr & 0x3) == 0x1) {
            SetGEN ((wdata >> (8-8)) & 0x1);
        } else if ((addr & 0x3) == 0x2) {
            if ((m_guard_module == CRG_MODULE)
                    && (m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
                // LOCK is bit 16
                SetOW((wdata >> 0) & 0x1);
            }
        } else {                                // (addr & 0x3) == 0x3
            if ((m_guard_module == CSG_MODULE)
                    && (m_core_type == CORE_G4MH) && (m_core_version >= CORE_VER_20)) {
                // LOCK is bit 31
                SetLOCK((wdata >> 7) & 0x1);
            }
        }
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 64BIT and 128BIT.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard SPID setting register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */
SysErrCause GSPID::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
#if _UNIT_TEST
        // Check KCE value in KCPROT.
        // Write value 0xA5A5A501U to enable Guard Settings modification.
#else
        if ((m_guard_internal->IsWriteEn() == false)
        || ((m_spid_module == CSG_MODULE) && (m_guard_internal->IsCrgCsgLock(m_channel)))) {
            return SYSERR_NONE;
        }
#endif
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard SPID setting register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GSPID::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size == SIZE_32BIT) {
        TRegData32 gspid_info;
        gspid_info.Write32(addr, data, m_reg_mask);
        SetSPID(gspid_info.m_data32);
    } else if (size == SIZE_16BIT) {
        TRegData32 gspid_info_16;
        gspid_info_16.m_data32 = GetSPID();
        gspid_info_16.Write16(addr, data, m_reg_mask);
        SetSPID (gspid_info_16.m_data32);
    } else if (size == SIZE_8BIT) {
        TRegData32 gspid_info_8;
        gspid_info_8.m_data32 = GetSPID();
        gspid_info_8.Write8(addr, data, m_reg_mask);
        SetSPID (gspid_info_8.m_data32);
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 64BIT and 128BIT.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard Base Address setting register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GBAD::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
#if _UNIT_TEST
        // Check KCE value in KCPROT.
        // Write value 0xA5A5A501U to enable Guard Settings modification.
#else
        if ((m_guard_internal->IsWriteEn() == false)
        || ((m_gbad_module == CSG_MODULE) && (m_guard_internal->IsCrgCsgLock(m_channel)))) {
            return SYSERR_NONE;
        }
#endif
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard Base Address setting register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GBAD::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    // Use the following formula to update BAD value
    // value = (current_value & ~current_mask) | (update_value & current_mask)
    if (size == SIZE_32BIT) {
        RegData cur_bad = GetBAD();

        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gbad_info;
        gbad_info.Write32(addr, data, m_reg_mask);

        // Step 2: Calculate (current_value & ~current_mask) | (update_value & current_mask)
        SetBAD((cur_bad & (~m_reg_mask.m_mask32)) | gbad_info.m_data32);
    } else if ((size == SIZE_16BIT) && (m_gbad_module != PEG_MODULE)) {
        RegData cur_bad = GetBAD();

        // shift_no will be 0 or 16.
        uint32_t shift_no = ((addr >> 1) & 0x1) * 16;

        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gbad_info_16;
        gbad_info_16.m_data32 = 0;
        gbad_info_16.Write16(addr, data, m_reg_mask);


        // Step 2: Calculate (current_value & ~current_mask) | (update_value & current_mask)
        // Update current mask depend on addr.
        uint32_t cur_mask_32 = (GUARD_DATA_MASK_16 << shift_no) & m_reg_mask.m_mask32;

        SetBAD((cur_bad & ~cur_mask_32) | gbad_info_16.m_data32);
    } else if ((size == SIZE_8BIT) && (m_gbad_module != PEG_MODULE)) {
        RegData cur_bad = GetBAD();

        // shift_no will be 0, 1, 2 or 3.
        uint32_t shift_no = (addr & 0x3) * 8;

        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gbad_info_8;
        gbad_info_8.m_data32 = 0;
        gbad_info_8.Write8(addr, data, m_reg_mask);

        // Step 2: Calculate (current_value & ~current_mask) | (update_value & current_mask)
        // Update current mask depend on addr.
        uint32_t cur_mask_32 = (GUARD_DATA_MASK_8 << shift_no) & m_reg_mask.m_mask32;
        SetBAD((cur_bad & ~cur_mask_32) | gbad_info_8.m_data32);
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 64BIT and 128BIT.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard Address Mask register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GADV::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
#if _UNIT_TEST
        // Check KCE value in KCPROT.
        // Write value 0xA5A5A501U to enable Guard Settings modification.
#else
        if ((m_guard_internal->IsWriteEn() == false)
        || ((m_gadv_module == CSG_MODULE) && (m_guard_internal->IsCrgCsgLock(m_channel)))) {
            return SYSERR_NONE;
        }
#endif
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard Address Mask register.
 * @param addr address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GADV::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    // Use the following formula to update BAD value
    // value = (current_value & ~current_mask) | (update_value & current_mask)
    if (size == SIZE_32BIT) {
        RegData cur_adv = GetADV();

        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gadv_info;
        gadv_info.Write32(addr, data, m_reg_mask);

        // Step 2: Calculate (current_value & ~current_mask) | (update_value & current_mask)
        SetADV((cur_adv & ~m_reg_mask.m_mask32) | gadv_info.m_data32);
    } else if ((size == SIZE_16BIT) && (m_gadv_module != PEG_MODULE)) {
        RegData cur_adv = GetADV();

        // shift_no will be 0 or 16.
        uint32_t shift_no = ((addr >> 1) & 0x1) * 16;

        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gadv_info_16;
        gadv_info_16.m_data32 = 0;
        gadv_info_16.Write16(addr, data, m_reg_mask);

        // Step 2: Calculate (current_value & ~current_mask) | (update_value & current_mask)
        // Update current mask depend on addr.
        uint32_t cur_mask_32 = (GUARD_DATA_MASK_16 << shift_no) & m_reg_mask.m_mask32;
        SetADV((cur_adv & ~cur_mask_32) | gadv_info_16.m_data32);
    } else if ((size == SIZE_8BIT) && (m_gadv_module != PEG_MODULE)) {
        RegData cur_adv = GetADV();

        // shift_no will be 0, 1, 2 or 3.
        uint32_t shift_no = (addr & 0x3) * 8;

        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gadv_info_8;
        gadv_info_8.m_data32 = 0;
        gadv_info_8.Write8(addr, data, m_reg_mask);

        // Step 2: Calculate (current_value & ~current_mask) | (update_value & current_mask)
        // Update current mask depend on addr.
        uint32_t cur_mask_32 = (GUARD_DATA_MASK_8 << shift_no) & m_reg_mask.m_mask32;
        SetADV((cur_adv & ~cur_mask_32) | gadv_info_8.m_data32);
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 64BIT and 128BIT.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }
    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard IVCSPID setting register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GIVCSPID::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
#if _UNIT_TEST
        // Check KCE value in KCPROT.
        // Write value 0xA5A5A501U to enable Guard Settings modification.
#else
        if ((m_guard_internal->IsWriteEn() == false)
         || (m_guard_internal->IsCrgIvcVmEn(m_channel) == true)) {
            return SYSERR_NONE;
        }
#endif
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard IVCSPID setting register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GIVCSPID::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size == SIZE_32BIT) {
        // Step 1: Calculate (update_value & current_mask)
        // Step 2: Calculate (current_ivc_spid & (~current_mask) | update_value)
        TRegData32 givcspid_info;
        givcspid_info.m_data32 = GetIvcSPID();
        // Update 32 bit.
        givcspid_info.Write32(addr, data, m_reg_mask);
        SetIvcSPID(givcspid_info.m_data32);
    } else if (size == SIZE_16BIT) {
        // Step 1: Calculate (update_value & current_mask)
        // Step 2: Calculate (current_ivc_spid & (~current_mask) | update_value)
        TRegData32 givcspid_info_16;
        givcspid_info_16.m_data32 = GetIvcSPID();;
        // Update only 16 bit.
        givcspid_info_16.Write16(addr, data, m_reg_mask);
        SetIvcSPID(givcspid_info_16.m_data32);
    } else if (size == SIZE_8BIT) {
        // Step 1: Calculate (update_value & current_mask)
        // Step 2: Calculate (current_ivc_spid & (~current_mask) | update_value)
        TRegData32 givcspid_info_8;
        givcspid_info_8.m_data32 = GetIvcSPID();;
        // Update only 8 bit.
        givcspid_info_8.Write8(addr, data, m_reg_mask);
        SetIvcSPID(givcspid_info_8.m_data32);
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 64BIT and 128BIT.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


SysErrCause GIVCOWNR::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard IVCOWNR setting register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GIVCOWNR::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }
    return SYSERR_NONE;
}



SysErrCause GIVCOWNR::ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const
{
    uint32_t spid_info = GetSPID();
    if (size == SIZE_32BIT) {
        *(reinterpret_cast<uint32_t*> (data)) = spid_info;
    } else if (size == SIZE_16BIT) {
        uint32_t index = (addr & 0x1) * 16;
        *(reinterpret_cast<uint16_t*> (data)) = (uint16_t) (spid_info >> index);
    } else if (size == SIZE_8BIT) {
        uint32_t index = (addr & 0x3) * 8;
        * data = (uint8_t) (spid_info >> index);
    }
    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard IVCLOCK setting register
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GIVCLOCK::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
#if _UNIT_TEST
        // Check KCE value in KCPROT.
        // Write value 0xA5A5A501U to enable Guard Settings modification.
#else
        if ((m_guard_internal->IsWriteEn() == false)
         && (!(m_guard_internal->IsCrgIvcVmEn(m_channel))
           || (m_guard_internal->GetCrgSpidOwner(m_channel) != sideband.GetSPID()))) {
            return SYSERR_NONE;
        }
#endif
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard IVCLOCK setting register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GIVCLOCK::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size == SIZE_32BIT) {
        TRegData32 gspid_info;
        gspid_info.Write32(addr, data, m_reg_mask);
        SetLK(gspid_info.m_data32);
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 32bit.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard IVCREQ setting register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GIVCREQ::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
        uint32_t cur_spid = 1 << sideband.GetSPID();
        if (m_guard_internal->IsCrgIvcVmEn(m_channel) && (cur_spid & m_guard_internal->GetCrgSpidListValid(m_channel))) {
            if (!m_guard_internal->IsCrgIvcVmLock(m_channel) || (cur_spid == m_guard_internal->GetCrgSpidOwner(m_channel))) {
                // In this case, the written value is not cared.
                return WriteBody (addr, data, size);
            }
        }
        return SYSERR_NONE;
    }
}


/*!
 * @brief Update new data to Guard IVCLOCK setting register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GIVCREQ::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (size == SIZE_32BIT) {
        m_gspid->SetSPID(m_guard_internal->GetCrgSpidOwner(m_channel) ^ m_guard_internal->GetCrgSpidListValid(m_channel));
        // Automatically clear Lock bit to 0 when owner requests.
        // In case not request from owner, LK bis is always 0.
        m_givc_lock->SetLK(0x0);
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 32bit.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard error clear register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GERRCLR::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Update new data to Guard error clear register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GERRCLR::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((size == SIZE_8BIT) || (size == SIZE_16BIT) || (size == SIZE_32BIT)) {
        if ((addr & 0x3) == 0x0) {
            // | ERRCL0 | ERRCLE |
            // | 0      | 0      | Neither bit is cleared
            // | 0      | 1      | PEGERRSTAT.ERR = 0
            // | 1      | 0      | PEGERRSTAT.OVF = 0
            // | 1      | 1      | PEGERRSTAT.ERR = 0 & PEGERRSTAT.OVF = 0
            uint32_t errclo = (data[0] >> 1) & 0x1;
            uint32_t errcle = (data[0] >> 0) & 0x1;

            if (errclo == 1) {
                // set OVF = 0
                m_gerrstat->SetOVF (0);
            }

            // This behavior is not effective in RH850v21,
            // because there is no ERR bit in GERRCLR.
            // In SetERR function, there is the condition for core type.
            if (errcle == 1) {
                // set ERR = 0
                m_gerrstat->SetERR (0);
            }
        }
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 64BIT and 128BIT.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard error status register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GERRSTAT::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((IsRegAccessSizeValid (size) == false)
      || (size == SIZE_16BIT)) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }
    return SYSERR_NONE;
}


/*!
 * @brief Checking Read condition in Guard error status register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Reading data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause or Return value of Sub-function.
 */
SysErrCause GERRSTAT::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if ((IsRegAccessSizeValid (size) == false)
      || (size == SIZE_16BIT)) {
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        std::fill_n (data, size, 0);
        return SYSERR_NONE;
    } else {
        return ReadBody (addr, data, size);
    }
}

/*!
 * @brief Checking Write condition in Guard error address register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GERRADDR::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }
    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard error access information register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GERRTYPE::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }
    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition before clearing SPID error in Guard register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GSPIDERRCLR::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
        return SYSERR_NONE;
    } else {
        return WriteBody (addr, data, size);
    }
}


/*!
 * @brief Clear the suitable SPID error in Guard register.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GSPIDERRCLR::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    // Use the following formula to update BAD value
    // value = (current_value & (~spid_list))
    if (size == SIZE_32BIT) {

        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gspiderr_info;
        gspiderr_info.Write32(addr, data, m_reg_mask);

        // Step 2: Clear the suitable bit: (current_value & (~spid_list))
        m_gspiderrstat -> ClrSPIDERR(~gspiderr_info.m_data32);
    } else if (size == SIZE_16BIT) {
        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gspiderr_info_16;
        gspiderr_info_16.m_data32 = 0;
        gspiderr_info_16.Write16(addr, data, m_reg_mask);

        // Step 2: Clear the suitable bit: (current_value & (~spid_list))
        m_gspiderrstat -> ClrSPIDERR(~gspiderr_info_16.m_data32);
    } else if (size == SIZE_8BIT) {
        // Step 1: Calculate (update_value & current_mask)
        TRegData32 gspiderr_info_8;
        gspiderr_info_8.m_data32 = 0;
        gspiderr_info_8.Write8(addr, data, m_reg_mask);

        // Step 2: Clear the suitable bit: (current_value & (~spid_list))
        m_gspiderrstat -> ClrSPIDERR(~gspiderr_info_8.m_data32);
    } else {
        // In case TargetWriteDebug function.
        // Ignore writing except 64BIT and 128BIT.
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }

    return SYSERR_NONE;
}


/*!
 * @brief Checking Write condition in Guard error status register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Written data.
 * @param size Access size.
 * @return SYSERR_NONE of SysErrCause.
 */

SysErrCause GSPIDERRSTAT::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
#if _UNIT_TEST
        throw std::range_error("Un-guaranteed Size Access");
#endif
    }
    return SYSERR_NONE;
}


