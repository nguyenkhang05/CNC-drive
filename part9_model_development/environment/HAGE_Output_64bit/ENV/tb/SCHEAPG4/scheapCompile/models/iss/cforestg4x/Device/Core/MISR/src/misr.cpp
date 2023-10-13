/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./misr.h"
#include "./proc_element.h"

#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
extern bool g_comp_misr_show;
extern FILE *g_cmp_fp;
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)

// Constructor of MisrL class
MisrL::MisrL (PhysAddr reg_addr)
    : TRegDataBase<TRegData32> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRL_MASK))
{
    m_treg = &m_data;
    m_data.m_data32 = 0x00000000;

    Reset ();
}


// Constructor of MisrH class
MisrH::MisrH (PhysAddr reg_addr)
    : TRegDataBase<TRegData32> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRH_MASK))
{
    m_treg = &m_data;
    m_data.m_data32 = 0x00000000;

    Reset ();
}


// Constructor of MisrCdrl class
MisrCdrl::MisrCdrl (PhysAddr reg_addr, MisrCr *misrCr,
                    MisrDcntCtl *misrDcntCtl, MisrDcnt *misrDcnt,
                    Misr *misr)
    : TRegDataBase<TRegData32> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRCDRL_MASK)),
      m_misrCr (misrCr), m_misr (misr), m_misrDcntCtl (misrDcntCtl), m_misrDcnt (misrDcnt)
{
    m_treg = &m_data;
    m_data.m_data32 = 0x00000000;

    Reset ();
}


// Write of MisrCdrl
SysErrCause MisrCdrl::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    } else {
        if (m_misr->IsMisrRegion (addr, size) == false) {
            return SYSERR_NONE;
        } else {
            if (m_misrDcntCtl->GetCntTrg() == 0) {
                m_misrDcnt->CountUp();
            }
        }

        SysErrCause wr_err = WriteBody (addr, data, size);
        // Not generate signature when MisrCr.Misr1En is 0.
        if (m_misrCr->GetMisr1En() == true &&
            m_misrCr->GetMisr1Cnd() == 0) {
            // mode of Register write.
            uint32_t wdata32 = (*(reinterpret_cast<uint32_t *>(data)));
            m_misr->CalcSignatureMisr1 (wdata32, size);
        }
        // Not generate signature when MisrCr.Misr2En is 0.
        if (m_misrCr->GetMisr2En() == true &&
            m_misrCr->GetMisr1Cnd() == 0) {
            // mode of Register write.
            uint32_t wdata32 = (*(reinterpret_cast<uint32_t *>(data)));
            m_misr->CalcSignatureMisr2 (wdata32, size);
        }
        return wr_err;
    }
}


// Read of MisrCdrl
SysErrCause MisrCdrl::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    CfMsg::DPrint (MSG_INF, "<Write to MisrCdrl is prohibited.>\n");
    return SYSERR_NONE;
}


// Constructor of MisrCr class
MisrCr::MisrCr (PhysAddr reg_addr)
    : TRegDataBase<TRegData8> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRCR_MASK))
{
    m_treg = &m_data;
    m_data.m_data8 = 0x00;

    Reset ();
}


// Constructor of MisrBaseAdr class
MisrBaseAdr::MisrBaseAdr (PhysAddr reg_addr)
    : TRegDataBase<TRegData32> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRBASEADR_MASK))
{
    m_treg = &m_data;
    m_data.m_data32 = 0x00000000;

    Reset ();
}


// Constructor of MisrAdrMsk class
MisrAdrMsk::MisrAdrMsk (PhysAddr reg_addr)
    : TRegDataBase<TRegData32> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRADRMSK_MASK))
{
    m_treg = &m_data;
    m_data.m_data32 = 0x00000000;

    Reset ();
}


// Constructor of MisrDcntCtl class
MisrDcntCtl::MisrDcntCtl (PhysAddr reg_addr)
    : TRegDataBase<TRegData8> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRDCNTCTL_MASK))
{
    m_treg = &m_data;
    m_data.m_data8 = 0x00;

    Reset ();
}


// Constructor of MisrDcnt class
MisrDcnt::MisrDcnt (PhysAddr reg_addr)
    : TRegDataBase<TRegData16> (SIZE_32BIT, SIZE_8BIT, reg_addr, TRegMask(MISRDCNT_MASK))
{
    m_treg = &m_data;
    m_data.m_data16 = 0x0000;

    Reset ();
}


// Reset function of MisrL Register
void MisrL::Reset (void)
{
    m_data.m_data32 = 0x00000000;
}


// Reset function of MisrH Register
void MisrH::Reset (void)
{
    m_data.m_data32 = 0x00000000;
}


// Reset function of MirCdrl Register
void MisrCdrl::Reset (void)
{
    m_data.m_data32 = 0x00000000;
}


// Reset function of MisrCr Register
void MisrCr::Reset (void)
{
    m_data.m_data8 = 0x0f;
}


// Reset function of MisrBaseAdr Register
void MisrBaseAdr::Reset (void)
{
    m_data.m_data32 = 0x00000000;
}


// Reset function of MisrAdrMsk Register
void MisrAdrMsk::Reset (void)
{
#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    // CompRunner monitors a store operation for all areas, by default.
    m_data.m_data32 = 0xffffffff;
#else // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    m_data.m_data32 = 0x00000000;
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
}


// Reset function of MisrDcntCtl Register
void MisrDcntCtl::Reset (void)
{
#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    // CompRunner monitors a store operation for all areas, by default.
    m_data.m_data8 = 0x03;
#else // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    m_data.m_data8 = 0x00;
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
}


// Reset function of MisrDcnt Register
void MisrDcnt::Reset (void)
{
    m_data.m_data16 = 0x0000;
}


TRegMisr::TRegMisr (PhysAddr reg_addr_base, uint32_t pe_id, Misr *misr)
    : m_reg_addr_base (reg_addr_base),
      m_misr (misr)
{
    // MISR1L
    PhysAddr misr1L_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISR1L_OFFSET;
    m_misr1L_ptr = new MisrL (misr1L_addr);
    m_trf.insert (TRegMap::value_type (misr1L_addr, m_misr1L_ptr));
    // MISR1H
    PhysAddr misr1H_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISR1H_OFFSET;
    m_misr1H_ptr = new MisrH (misr1H_addr);
    m_trf.insert (TRegMap::value_type (misr1H_addr, m_misr1H_ptr));
    // MISR2L
    PhysAddr misr2L_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISR2L_OFFSET;
    m_misr2L_ptr = new MisrL (misr2L_addr);
    m_trf.insert (TRegMap::value_type (misr2L_addr, m_misr2L_ptr));
    // MISR2H
    PhysAddr misr2H_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISR2H_OFFSET;
    m_misr2H_ptr = new MisrH (misr2H_addr);
    m_trf.insert (TRegMap::value_type (misr2H_addr, m_misr2H_ptr));
    // MISRCR
    PhysAddr misrcr_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISRCR_OFFSET;
    m_misrCr_ptr = new MisrCr (misrcr_addr);
    m_trf.insert (TRegMap::value_type (misrcr_addr, m_misrCr_ptr));
    // MISRBASEADR
    PhysAddr misrBaseAdr_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISRBASEADR_OFFSET;
    m_misrBaseAdr_ptr = new MisrBaseAdr (misrBaseAdr_addr);
    m_trf.insert (TRegMap::value_type (misrBaseAdr_addr, m_misrBaseAdr_ptr));
    // MISRADRMSK
    PhysAddr misrAdrMsk_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISRADRMSK_OFFSET;
    m_misrAdrMsk_ptr = new MisrAdrMsk (misrAdrMsk_addr);
    m_trf.insert (TRegMap::value_type (misrAdrMsk_addr, m_misrAdrMsk_ptr));
    // MISRDCNTCTL
    PhysAddr misrDcntCtl_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISRDCNTCTL_OFFSET;
    m_misrDcntCtl_ptr = new MisrDcntCtl (misrDcntCtl_addr);
    m_trf.insert (TRegMap::value_type (misrDcntCtl_addr, m_misrDcntCtl_ptr));
    // MISRDCNT
    PhysAddr misrDcnt_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISRDCNT_OFFSET;
    m_misrDcnt_ptr = new MisrDcnt (misrDcnt_addr);
    m_trf.insert (TRegMap::value_type (misrDcnt_addr, m_misrDcnt_ptr));
    // MISRCDRL
    PhysAddr misrcdrl_addr = m_reg_addr_base + pe_id * 0x0000001000U + MISRCDRL_OFFSET;
    m_misrCdrl_ptr = new MisrCdrl (misrcdrl_addr, m_misrCr_ptr,
                                   m_misrDcntCtl_ptr, m_misrDcnt_ptr, misr);
    m_trf.insert (TRegMap::value_type (misrcdrl_addr, m_misrCdrl_ptr));
}


// MISR Register Write function
SysErrCause Misr::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->Write (sideband, addr, data, size);
    return SYSERR_NONE;
}


// MISR Register Read function
SysErrCause Misr::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


// MISR Register Write-Debug function
SysErrCause Misr::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->WriteBody (addr, data, size);
    return SYSERR_NONE;
}


// MISR Register Read-Debug function
SysErrCause Misr::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


void Misr::HardReset (void)
{
    m_trfs->ResetTRegs ();
}


/*!
 * @brief Constructor of Misr
 * @param base_addr Base address for MISR
 * @param pe_id PEID
 */
Misr::Misr (PhysAddr base_addr, uint32_t pe_id)
{
    m_trfs = new TRegMisr (base_addr, pe_id, this);
}


bool Misr::IsMisrRegion (PhysAddr addr, ByteSize size)
{
    if (addr < 0x10000000) {
        return false;
    }

    RegData addr_mask = ~(m_trfs->GetMisrAdrMskPtr()->GetData());
    RegData addr_base =  m_trfs->GetMisrBaseAdrPtr()->GetData ();

    PhysAddr start_addr_base = addr & addr_mask;
    PhysAddr end_addr_base = (addr + size - 1) & addr_mask;
    if ((start_addr_base) != addr_base) { return false; }
    if (unlikely (start_addr_base != end_addr_base)) {
        if ((end_addr_base) != addr_base) {
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}

void Misr::CalcSignMonitor (PhysAddr addr, uint8_t *wdata, ByteSize size)
{
    if (IsMisrRegion (addr, size) == false) {
        return;
    } else {
        if (m_trfs->GetMisrDcntCtl()->GetCntTrg() == 1) {
            if (size == SIZE_64BIT) {
                m_trfs->GetMisrDcnt()->CountUp();
            }
            m_trfs->GetMisrDcnt()->CountUp();
        }
    }

    // Not generate signature when MisrCr.Misr1En is 0.
    if (m_trfs->GetMisrCrPtr()->GetMisr1En() == true &&
        m_trfs->GetMisrCrPtr()->GetMisr1Cnd() == 1) {
        // Write-Monitoring mode
        if (size == SIZE_64BIT) {
            uint64_t wdata64 = (*(reinterpret_cast<uint64_t *>(wdata)));
            CalcSignatureMisr1 (static_cast<uint32_t>(wdata64 & 0x0FFFFFFFFULL), size);
            CalcSignatureMisr1 (static_cast<uint32_t>((wdata64 >> 32) & 0x0FFFFFFFFULL), size);
        } else {
            uint32_t wdata32 = (*(reinterpret_cast<uint32_t *>(wdata)));
            CalcSignatureMisr1 (wdata32, size);
        }
    }
    // Not generate signature when MisrCr.Misr2En is 0.
    if (m_trfs->GetMisrCrPtr()->GetMisr2En() == true &&
        m_trfs->GetMisrCrPtr()->GetMisr1Cnd() == 1) {
        // Write-Monitoring mode
        if (size == SIZE_64BIT) {
            uint64_t wdata64 = (*(reinterpret_cast<uint64_t *>(wdata)));
            CalcSignatureMisr2 (static_cast<uint32_t>(wdata64 & 0x0FFFFFFFFULL), size);
            CalcSignatureMisr2 (static_cast<uint32_t>((wdata64 >> 32) & 0x0FFFFFFFFULL), size);
        } else {
            uint32_t wdata32 = (*(reinterpret_cast<uint32_t *>(wdata)));
            CalcSignatureMisr2 (wdata32, size);
        }
    }
    return;
}


void Misr::CalcSignatureMisr1 (uint32_t wdata, ByteSize size)
{
    uint32_t misr_mask;

    // Misr1L
    uint32_t misr1L_wdata = wdata;
    uint32_t misr1L_data = m_trfs->GetMisr1LPtr()->GetData();
    if (misr1L_data & 0x80000000U) {
        misr_mask = 0xffffffffU;
    } else {
        misr_mask = 0x00000000U;
    }
    uint32_t misr1L_sft = misr1L_data << 1;
    uint32_t misr1L_res = misr1L_sft ^ misr1L_wdata ^ (misr_mask & MISR1_FEDBCK_BIT);

    m_trfs->GetMisr1LPtr()->SetData(misr1L_res);

#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    if (g_comp_misr_show == true) {
        CfMsg::DPrint (MSG_INF, "<MISR1L = %08x>\n", misr1L_res);
    }
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
}


void Misr::CalcSignatureMisr2 (uint32_t wdata, ByteSize size)
{
    uint32_t misr_mask;

    // Misr2L
    uint32_t misr2L_wdata = BitRevert32 (wdata);
    uint32_t misr2L_data = m_trfs->GetMisr2LPtr()->GetData();
    if (misr2L_data & 0x80000000U) {
        misr_mask = 0xffffffffU;
    } else {
        misr_mask = 0x00000000U;
    }
    uint32_t misr2L_sft = misr2L_data << 1;
    uint32_t misr2L_res = misr2L_sft ^ misr2L_wdata ^ (misr_mask & MISR2_FEDBCK_BIT);

    m_trfs->GetMisr2LPtr()->SetData(misr2L_res);

#if defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
    if (g_comp_misr_show == true) {
        CfMsg::DPrint (MSG_INF, "<MISR2L = %08x>\n", misr2L_res);
    }
#endif // defined(COMP_RUNNER_G3MH) || defined (COMP_RUNNER_G4MH)
}


uint32_t Misr::BitRevert32 (uint32_t data)
{
    data = (data & 0x55555555) <<  1 | (data & 0xAAAAAAAA) >>  1;
    data = (data & 0x33333333) <<  2 | (data & 0xCCCCCCCC) >>  2;
    data = (data & 0x0F0F0F0F) <<  4 | (data & 0xF0F0F0F0) >>  4;
    data = (data & 0x00FF00FF) <<  8 | (data & 0xFF00FF00) >>  8;
    data = (data & 0x0000FFFF) << 16 | (data & 0xFFFF0000) >> 16;
    return data;
}


// Destructor of Misr
Misr::~Misr () {
    delete m_trfs;
}
