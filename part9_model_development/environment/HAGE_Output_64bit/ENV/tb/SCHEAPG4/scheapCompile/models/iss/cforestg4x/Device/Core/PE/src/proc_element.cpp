/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <algorithm>
#include <vector>
#include <functional>
#include "./config.h"
#include "./forest_common.h"
#include "./proc_element.h"
#include "./sregfile.h"
#include "./trace_info.h"
#include "./icache.h"
#include "./intc1.h"
#include "./btb_base.h"
#include "./mpu.h"
#include "./peg.h"
#ifdef ENABLE_DBG_BREAK
#include "./dbg_break.h"
#endif // #ifdef ENABLE_DBG_BREAK
#include "./lsu.h"
#include "./regid.h"
#include "./sregfile.h"
#include "./gregfile.h"
#include "./fs_controler.h"
#include "./inst_v850.h"
#include "./forest_utility.h"
#include "./forest_message.h"
#include "./llsc.h"
#include "./cs_lsu_buf.h"
#ifdef ENABLE_OSTM
#include "./os_timer.h"
#endif // #ifdef ENABLE_OSTM
#include "./dbuf.h"
#include "./misr.h"
#include "./module_guard.h"
#include "./fsbus.h"
#include "./wregdata128.h"
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
#include "./flash_cache.h"
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
#ifdef SUPPORT_CUBESUITE
#include "./cube_callback.h"
#endif // SUPPORT_CUBESUITE


NcSRegFile* ProcElement::GetNcSrfPtr (void) const
{
    return m_nc_srf;
}


SRegFile* ProcElement::GetSrfPtr (ContextId tc_id) const
{
    return m_srfs->GetSrfPtr (tc_id);
}


GRegFile* ProcElement::GetGrfPtr (ContextId tc_id) const
{
    return m_grfs->GetGrfPtr (tc_id);
}


void ProcElement::ReqIntByPeripheral (uint32_t channel)
{
    m_intc1->ReqIntByPeripheral (channel);
}


void ProcElement::ReqIntByIntc2 (uint32_t channel, IntReqInfoBase* intreq_info)
{
    m_intc1->ReqIntByIntc2 (channel, intreq_info);
}


void ProcElement::CancelIntByMask (uint32_t channel)
{
    m_intc1->CancelIntByMask (channel);
}


void ProcElement::CancelIntByIntc2 (uint32_t channel)
{
    m_intc1->CancelInt (channel);
}


void ProcElement::CancelIntByPeripheral (uint32_t channel)
{
    m_intc1->CancelIntByPeripheral (channel);
}


void ProcElement::UpdateIntHighPriority (ContextId tcid)
{
    m_intc1->UpdateHighPriority (tcid);
}


bool ProcElement::IsIntLevelDetect (uint32_t channel)
{
    return m_intc1->IsIntLevelDetect (channel);
}


bool ProcElement::IsIntReqInQueue (uint32_t channel)
{
    return m_intc1->IsIntReqInQueue (channel);
}


void ProcElement::SetIntc2Ptr (CIntc2* intc2)
{
    if (m_intc1 != NULL) {
        m_intc1->SetIntc2Ptr (intc2);
#ifdef ENABLE_OSTM
        for (uint32_t ch = 0; ch < MAX_OSTM_NUM; ch++) {
            if (m_ostm[ch] != NULL) {
                m_ostm[ch]->SetIntcPtr (m_intc1, intc2);
            }
        }
#endif // #ifdef ENABLE_OSTM
    }
}


#ifdef SUPPORT_SC_HEAP
bool ProcElement::IsGmGpidUpdated (void) const
{
    // Need to check Update GM/GPID for SC-HEAP event.
    SideBand* sb_ptr = GetHtPtr()->m_lsu->GetSideBandPtr ();

    uint32_t old_gm   = sb_ptr->GetGM ();
    uint32_t old_gpid = sb_ptr->GetGPID ();
    uint32_t new_gm;
    uint32_t new_gpid;
    SRegFile* srf = GetSrfPtr(NC);
    if (IsHvEnabled () == true) {
        new_gm = srf->SrPSWH()->GetGM ();
        new_gpid = srf->SrPSWH()->GetGPID ();
    } else {
        new_gm = 0;
        new_gpid = 0;
    }
    return (new_gm != old_gm) || (new_gpid != old_gpid);
}
#endif // #ifdef SUPPORT_SC_HEAP


void ProcElement::ChangeToHostMode (void)
{
    // Copy Host registers in m_nc_srfs to mirror registers in m_srfs.
    SRegFile* srf = GetSrfPtr(NC);
    srf->SetSrPtr(CF_SR_VEIPC, static_cast<SRegBaseEIPC*>(m_nc_srf->SrHMEIPC ()));
    srf->SetSrPtr(CF_SR_VEIPSW, static_cast<SRegBasePSW*>(m_nc_srf->SrHMEIPSW ()));
    srf->SetSrPtr(CF_SR_VFEPC, static_cast<SRegBaseFEPC*>(m_nc_srf->SrHMFEPC ()));
    srf->SetSrPtr(CF_SR_VFEPSW, static_cast<SRegBasePSW*>(m_nc_srf->SrHMFEPSW ()));
    srf->SetSrPtr(CF_SR_VPSW, static_cast<SRegBasePSW*>(m_nc_srf->SrHMPSW ()));
    srf->SetSrPtr(CF_SR_VEIIC, static_cast<SRegBaseEIIC*>(m_nc_srf->SrHMEIIC ()));
    srf->SetSrPtr(CF_SR_VFEIC, static_cast<SRegBaseFEIC*>(m_nc_srf->SrHMFEIC ()));
    srf->SetSrPtr(CF_SR_VEIWR, static_cast<SRegBaseEIWR*>(m_nc_srf->SrHMEIWR ()));
    srf->SetSrPtr(CF_SR_VFEWR, static_cast<SRegBaseFEWR*>(m_nc_srf->SrHMFEWR ()));
    srf->SetSrPtr(CF_SR_VSPID, static_cast<SRegBaseSPID*>(m_nc_srf->SrHMSPID ()));
    srf->SetSrPtr(CF_SR_VSPIDLIST, static_cast<SRegBaseSPIDLIST*>(m_nc_srf->SrHMSPIDLIST ()));
    srf->SetSrPtr(CF_SR_VEBASE, static_cast<SRegBaseEBASE*>(m_nc_srf->SrHMEBASE ()));
    srf->SetSrPtr(CF_SR_VINTBP, static_cast<SRegBaseINTBP*>(m_nc_srf->SrHMINTBP ()));
    srf->SetSrPtr(CF_SR_VINTCFG, static_cast<SRegBaseINTCFG*>(m_nc_srf->SrHMINTCFG ()));
    srf->SetSrPtr(CF_SR_VPLMR, static_cast<SRegBasePLMR*>(m_nc_srf->SrHMPLMR ()));
    srf->SetSrPtr(CF_SR_VMPM, static_cast<SRegBaseMPM*>(m_nc_srf->SrHMMPM ()));
    srf->SetSrPtr(CF_SR_VSVLOCK, static_cast<SRegBaseSVLOCK*>(m_nc_srf->SrHMSVLOCK ()));
    srf->SetSrPtr(CF_SR_VMEA, static_cast<SRegBaseMEA*>(m_nc_srf->SrHMMEA ()));
    srf->SetSrPtr(CF_SR_VMEI, static_cast<SRegBaseMEI*>(m_nc_srf->SrHMMEI ()));
    srf->SetSrPtr(CF_SR_VPEID, static_cast<SRegBasePEID*>(m_nc_srf->SrHMPEID ()));

    if (IsHvEnabled ()) {
        SRegBase* dummy_sr_sv = srf->GetSrPtr(CF_SR_DUMMY); // m_sreg_dummy to SV.

        // Set body regs to GMxx registers to permit access
        srf->SetSrPtr(CF_SR_GMEIPC, m_nc_srf->SrGMEIPC ());
        srf->SetSrPtr(CF_SR_GMEIPSW, m_nc_srf->SrGMEIPSW ());
        srf->SetSrPtr(CF_SR_GMFEPC, m_nc_srf->SrGMFEPC ());
        srf->SetSrPtr(CF_SR_GMFEPSW, m_nc_srf->SrGMFEPSW ());
        srf->SetSrPtr(CF_SR_GMPSW, m_nc_srf->SrGMPSW ());
        srf->SetSrPtr(CF_SR_GMEIIC, m_nc_srf->SrGMEIIC ());
        srf->SetSrPtr(CF_SR_GMFEIC, m_nc_srf->SrGMFEIC ());
        srf->SetSrPtr(CF_SR_GMEIWR, m_nc_srf->SrGMEIWR ());
        srf->SetSrPtr(CF_SR_GMFEWR, m_nc_srf->SrGMFEWR ());
        srf->SetSrPtr(CF_SR_GMSPID, m_nc_srf->SrGMSPID ());
        srf->SetSrPtr(CF_SR_GMSPIDLIST, m_nc_srf->SrGMSPIDLIST ());
        srf->SetSrPtr(CF_SR_GMEBASE, m_nc_srf->SrGMEBASE ());
        srf->SetSrPtr(CF_SR_GMINTBP, m_nc_srf->SrGMINTBP ());
        srf->SetSrPtr(CF_SR_GMINTCFG, m_nc_srf->SrGMINTCFG ());
        srf->SetSrPtr(CF_SR_GMPLMR, m_nc_srf->SrGMPLMR ());
        srf->SetSrPtr(CF_SR_GMMPM, m_nc_srf->SrGMMPM ());
        srf->SetSrPtr(CF_SR_GMSVLOCK, m_nc_srf->SrGMSVLOCK ());
        srf->SetSrPtr(CF_SR_GMMEA, m_nc_srf->SrGMMEA ());
        srf->SetSrPtr(CF_SR_GMMEI, m_nc_srf->SrGMMEI ());
        srf->SetSrPtr(CF_SR_GMPEID, m_nc_srf->SrGMPEID ());

        // Enable registers on HVE=1.
        srf->SetSrPtr(CF_SR_GMCFG, m_nc_srf->SrGMCFG ());
        srf->SetSrPtr(CF_SR_PSWH, m_nc_srf->SrPSWH ());
        srf->SetSrPtr(CF_SR_EIPSWH, m_nc_srf->SrEIPSWH ());
        srf->SetSrPtr(CF_SR_FEPSWH, m_nc_srf->SrFEPSWH ());
        srf->SetSrPtr(CF_SR_HVSB, m_nc_srf->SrHVSB ());
        srf->SetSrPtr(CF_SR_PMGMCTRL, m_nc_srf->SrPMGMCTRL ());
        srf->SetSrPtr(CF_SR_DBPSWH, m_nc_srf->SrDBPSWH ());
        srf->SetSrPtr(CF_SR_DEVDS, m_nc_srf->SrDEVDS ());

        // Disable registers on HVE=1.
        srf->SetSrPtr(CF_SR_ICSR, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_FPEC, dummy_sr_sv);

        // Disable PMCOUNTn, PMCTRLn [0, 3] in G4MH1.1.
        srf->SetSrPtr(CF_SR_PMCOUNT0_G4X1X, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMCOUNT1_G4X1X, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMCOUNT2_G4X1X, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMCOUNT3_G4X1X, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMCTRL0_G4X1X, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMCTRL1_G4X1X, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMCTRL2_G4X1X, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMCTRL3_G4X1X, dummy_sr_sv);
        // PMCOUNTn [0, 7] in G4MH2.0.
        srf->SetSrPtr(CF_SR_PMCOUNT0_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT0_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCOUNT1_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT1_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCOUNT2_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT2_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCOUNT3_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT3_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCOUNT4_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT4_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCOUNT5_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT5_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCOUNT6_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT6_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCOUNT7_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT7_G4MH20()));
        // PMCTRLn [0, 7] in G4MH2.0.
        srf->SetSrPtr(CF_SR_PMCTRL0_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL0_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCTRL1_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL1_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCTRL2_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL2_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCTRL3_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL3_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCTRL4_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL4_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCTRL5_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL5_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCTRL6_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL6_G4MH20()));
        srf->SetSrPtr(CF_SR_PMCTRL7_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL7_G4MH20()));
    } else {
        SRegBase* dummy_sr_sv = srf->GetSrPtr(CF_SR_DUMMY); // m_sreg_dummy to SV.
        srf->SetSrPtr(CF_SR_GMEIPC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEIPSW,dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEPC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEPSW, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMPSW, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEIIC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEIC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEIWR, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEWR, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMSPID, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMSPIDLIST, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEBASE, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMINTBP, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMINTCFG, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMPLMR, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMMPM, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMSVLOCK, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMMEA, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMMEI, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMPEID, dummy_sr_sv);

        // Disable registers on HVE=0.
        srf->SetSrPtr(CF_SR_GMCFG, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PSWH, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_EIPSWH, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_FEPSWH, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_HVSB, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_PMGMCTRL, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_DBPSWH, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_DEVDS, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_FPEC, dummy_sr_sv);

        // Enable registers on HVE=0.
        srf->SetSrPtr(CF_SR_ICSR, m_nc_srf->SrICSR ());

        // Disable registers on G4MH1.x (HV is disabled always in G4MH1.x)
        // Disable registers on G4KH
        if ((GetCoreType () != CORE_G4MH) || (GetCoreVersion () < CORE_VER_20)) {
            srf->SetSrPtr(CF_SR_HVCFG, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_MPBK, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_DBGEN, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_DECFG, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_DECTRL, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_DBCFG, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_L1RCFG, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_LSCFG, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_IMSR, dummy_sr_sv);
            if (GetCoreType () == CORE_G4KH) {
                srf->SetSrPtr(CF_SR_FPIPR, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_ICBKEY, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_IFCR, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_IFCR1, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_BRPCTRL0, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_BRPCTRL1, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_BRPCFG, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_BRPACTRL, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_BRPDATA, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_LSUCR, dummy_sr_sv);
                srf->SetSrPtr(CF_SR_RDBCR, dummy_sr_sv);
                // Enable registers in G4KH only.
                srf->SetSrPtr(CF_SR_FPEC, m_nc_srf->SrFPEC ());
            }
            // PMCOUNTn, PMCTRLn [0, 3] in G4MH1.x and G4KH.
            srf->SetSrPtr(CF_SR_PMCOUNT0_G4X1X, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT0_G4X1X()));
            srf->SetSrPtr(CF_SR_PMCOUNT1_G4X1X, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT1_G4X1X()));
            srf->SetSrPtr(CF_SR_PMCOUNT2_G4X1X, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT2_G4X1X()));
            srf->SetSrPtr(CF_SR_PMCOUNT3_G4X1X, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT3_G4X1X()));
            srf->SetSrPtr(CF_SR_PMCTRL0_G4X1X, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL0_G4X1X()));
            srf->SetSrPtr(CF_SR_PMCTRL1_G4X1X, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL1_G4X1X()));
            srf->SetSrPtr(CF_SR_PMCTRL2_G4X1X, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL2_G4X1X()));
            srf->SetSrPtr(CF_SR_PMCTRL3_G4X1X, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL3_G4X1X()));
            // Disable PMCOUNTn [0, 7] in G4MH1.x and G4KH.
            srf->SetSrPtr(CF_SR_PMCOUNT0_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT1_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT2_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT3_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT4_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT5_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT6_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT7_G4MH20, dummy_sr_sv);
            // Disable PMCTRLn [0, 7] in G4MH1.x and G4KH.
            srf->SetSrPtr(CF_SR_PMCTRL0_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL1_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL2_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL3_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL4_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL5_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL6_G4MH20, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL7_G4MH20, dummy_sr_sv);
        } else { // G4MH2.x with HVE = 0
            // Disable PMCOUNTn, PMCTRLn [0, 3] in G4MH1.1.
            srf->SetSrPtr(CF_SR_PMCOUNT0_G4X1X, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT1_G4X1X, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT2_G4X1X, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCOUNT3_G4X1X, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL0_G4X1X, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL1_G4X1X, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL2_G4X1X, dummy_sr_sv);
            srf->SetSrPtr(CF_SR_PMCTRL3_G4X1X, dummy_sr_sv);
            // PMCOUNTn [0, 7] in G4MH2.0.
            srf->SetSrPtr(CF_SR_PMCOUNT0_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT0_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCOUNT1_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT1_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCOUNT2_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT2_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCOUNT3_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT3_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCOUNT4_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT4_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCOUNT5_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT5_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCOUNT6_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT6_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCOUNT7_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT7_G4MH20()));
            // PMCTRLn [0, 7] in G4MH2.0.
            srf->SetSrPtr(CF_SR_PMCTRL0_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL0_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCTRL1_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL1_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCTRL2_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL2_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCTRL3_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL3_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCTRL4_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL4_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCTRL5_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL5_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCTRL6_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL6_G4MH20()));
            srf->SetSrPtr(CF_SR_PMCTRL7_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL7_G4MH20()));
        }
    }

    // @@ TODO : Update status
    UpdateHvStatus ();
    UpdatePrivLevel ();

#ifdef MASK_INT_BY_GPID
    GetHtPtr()->UpdateIntRequested ();
#else // #ifdef MASK_INT_BY_GPID
    GetHtPtr()->UpdateIntStatus ();
#endif // #ifdef MASK_INT_BY_GPID
    GetHtPtr()->UpdateIntAcceptable ();
    GetHtPtr()->UpdateGmIntAcceptable ();
    GetHtPtr()->UpdateBgIntAcceptable ();
    GetHtPtr()->UpdateFeintAcceptable ();
    GetHtPtr()->UpdateGmFeintAcceptable ();
    GetHtPtr()->UpdateBgFeintAcceptable ();
    GetHtPtr()->UpdateOperandSyserrEvent();
    GetHtPtr()->SetPmcUpdateFlag ();
    bool dmdp = srf->SrMPCFG()->GetDMDP ();
    GetHtPtr()->m_lsu->SetMdpSuppress (dmdp);
    GetHtPtr()->m_insts->UpdatePswReg ();

    UpdateSideband ();

#ifdef SUPPORT_CUBESUITE
    // Notice CpuOperationMode update by CS+ callback
    CpuOperationMode mode;
    uint32_t gpid;
    if (IsHvEnabled ()) {
        if (IsHostMode ()) {
            mode = HOST_MODE;
        } else {
            mode = GUEST_MODE;
        }
        gpid = srf->SrPSWH()->GetGPID ();
    } else {
        mode = CONVENTIONAL_MODE;
        gpid = 0;
    }
    (*CubeCallBack::__NotifyCpuOperationMode) (GetPeId(), mode, gpid);

    // Notice SPID update by CS+ callback
    (*CubeCallBack::__NotifySPID) (GetPeId(), srf->SrVSPID()->GetSPID ());
#endif // #ifdef SUPPORT_CUBESUITE
}


bool ProcElement::IsGuestModeWithGpid (uint32_t gpid) const
{
    return IsGuestMode() && (gpid == m_nc_srf->SrPSWH()->GetGPID());
}


void ProcElement::ChangeToGuestMode (void)
{
    // Copy Guest registers in m_nc_srf to mirror registers in srfs.
    SRegFile* srf = GetSrfPtr(NC);

    // Update virtual registers
    srf->SetSrPtr(CF_SR_VEIPC, static_cast<SRegBaseEIPC*>(m_nc_srf->SrGMEIPC ()));
    srf->SetSrPtr(CF_SR_VEIPSW, static_cast<SRegBasePSW*>(m_nc_srf->SrGMEIPSW ()));
    srf->SetSrPtr(CF_SR_VFEPC, static_cast<SRegBaseFEPC*>(m_nc_srf->SrGMFEPC ()));
    srf->SetSrPtr(CF_SR_VFEPSW, static_cast<SRegBasePSW*>(m_nc_srf->SrGMFEPSW ()));
    srf->SetSrPtr(CF_SR_VPSW, static_cast<SRegBasePSW*>(m_nc_srf->SrGMPSW ()));
    srf->SetSrPtr(CF_SR_VEIIC, static_cast<SRegBaseEIIC*>(m_nc_srf->SrGMEIIC ()));
    srf->SetSrPtr(CF_SR_VFEIC, static_cast<SRegBaseFEIC*>(m_nc_srf->SrGMFEIC ()));
    srf->SetSrPtr(CF_SR_VEIWR, static_cast<SRegBaseEIWR*>(m_nc_srf->SrGMEIWR ()));
    srf->SetSrPtr(CF_SR_VFEWR, static_cast<SRegBaseFEWR*>(m_nc_srf->SrGMFEWR ()));
    srf->SetSrPtr(CF_SR_VSPID, static_cast<SRegBaseSPID*>(m_nc_srf->SrGMSPID ()));
    srf->SetSrPtr(CF_SR_VSPIDLIST, static_cast<SRegBaseSPIDLIST*>(m_nc_srf->SrGMSPIDLIST ()));
    srf->SetSrPtr(CF_SR_VEBASE, static_cast<SRegBaseEBASE*>(m_nc_srf->SrGMEBASE ()));
    srf->SetSrPtr(CF_SR_VINTBP, static_cast<SRegBaseINTBP*>(m_nc_srf->SrGMINTBP ()));
    srf->SetSrPtr(CF_SR_VINTCFG, static_cast<SRegBaseINTCFG*>(m_nc_srf->SrGMINTCFG ()));
    srf->SetSrPtr(CF_SR_VPLMR, static_cast<SRegBasePLMR*>(m_nc_srf->SrGMPLMR ()));
    srf->SetSrPtr(CF_SR_VMPM, static_cast<SRegBaseMPM*>(m_nc_srf->SrGMMPM ()));
    srf->SetSrPtr(CF_SR_VSVLOCK, static_cast<SRegBaseSVLOCK*>(m_nc_srf->SrGMSVLOCK ()));
    srf->SetSrPtr(CF_SR_VMEA, static_cast<SRegBaseMEA*>(m_nc_srf->SrGMMEA ()));
    srf->SetSrPtr(CF_SR_VMEI, static_cast<SRegBaseMEI*>(m_nc_srf->SrGMMEI ()));
    srf->SetSrPtr(CF_SR_VPEID, static_cast<SRegBasePEID*>(m_nc_srf->SrGMPEID ()));

    // Set dummy regs to GMxx registers to prohibit access
    // To cause PIE by UM/SV access, set m_sreg_dummy_hv instead of CF_SR_DUMMY.
    SRegBase* dummy_sr_sv = srf->GetSrPtr(CF_SR_DUMMY); // m_sreg_dummy to SV.
    if (IsHvEnabled ()) {
        // m_sreg_dummy to HV.
        srf->SetSrPtr(CF_SR_GMEIPC, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMEIPSW,m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMFEPC, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMFEPSW, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMPSW, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMEIIC, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMFEIC, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMEIWR, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMFEWR, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMSPID, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMSPIDLIST, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMEBASE, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMINTBP, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMINTCFG, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMPLMR, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMMPM, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMSVLOCK, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMMEA, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMMEI, m_sreg_dummy_hv);
        srf->SetSrPtr(CF_SR_GMPEID, m_sreg_dummy_hv);
    } else {
        // m_sreg_dummy to SV.
        srf->SetSrPtr(CF_SR_GMEIPC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEIPSW,dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEPC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEPSW, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMPSW, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEIIC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEIC, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEIWR, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMFEWR, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMSPID, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMSPIDLIST, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMEBASE, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMINTBP, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMINTCFG, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMPLMR, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMMPM, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMSVLOCK, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMMEA, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMMEI, dummy_sr_sv);
        srf->SetSrPtr(CF_SR_GMPEID, dummy_sr_sv);
    }

    // Disable registers on HVE=1.
    srf->SetSrPtr(CF_SR_ICSR, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_FPEC, dummy_sr_sv);

    // Disable PMCOUNTn, PMCTRLn [0, 3] in G4MH1.1.
    srf->SetSrPtr(CF_SR_PMCOUNT0_G4X1X, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_PMCOUNT1_G4X1X, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_PMCOUNT2_G4X1X, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_PMCOUNT3_G4X1X, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_PMCTRL0_G4X1X, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_PMCTRL1_G4X1X, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_PMCTRL2_G4X1X, dummy_sr_sv);
    srf->SetSrPtr(CF_SR_PMCTRL3_G4X1X, dummy_sr_sv);
    // PMCOUNTn [0, 7] in G4MH2.0.
    srf->SetSrPtr(CF_SR_PMCOUNT0_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT0_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCOUNT1_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT1_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCOUNT2_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT2_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCOUNT3_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT3_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCOUNT4_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT4_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCOUNT5_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT5_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCOUNT6_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT6_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCOUNT7_G4MH20, static_cast<SRegPMCOUNT*>(m_nc_srf->SrPMCOUNT7_G4MH20()));
    // PMCTRLn [0, 7] in G4MH2.0.
    srf->SetSrPtr(CF_SR_PMCTRL0_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL0_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCTRL1_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL1_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCTRL2_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL2_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCTRL3_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL3_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCTRL4_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL4_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCTRL5_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL5_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCTRL6_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL6_G4MH20()));
    srf->SetSrPtr(CF_SR_PMCTRL7_G4MH20, static_cast<SRegPMCTRL*>(m_nc_srf->SrPMCTRL7_G4MH20()));

    // @@ TODO : Update status
    UpdateHvStatus ();
    UpdatePrivLevel ();
#ifdef MASK_INT_BY_GPID
    GetHtPtr()->UpdateIntRequested ();
#else // #ifdef MASK_INT_BY_GPID
    GetHtPtr()->UpdateIntStatus ();
#endif // #ifdef MASK_INT_BY_GPID
    GetHtPtr()->UpdateIntAcceptable ();
    GetHtPtr()->UpdateGmIntAcceptable ();
    GetHtPtr()->UpdateBgIntAcceptable ();
    GetHtPtr()->UpdateFeintAcceptable ();
    GetHtPtr()->UpdateGmFeintAcceptable ();
    GetHtPtr()->UpdateBgFeintAcceptable ();
    GetHtPtr()->UpdateOperandSyserrEvent();
    GetHtPtr()->SetPmcUpdateFlag ();
    bool dmdp = srf->SrMPCFG()->GetDMDP ();
    GetHtPtr()->m_lsu->SetMdpSuppress (dmdp);
    GetHtPtr()->m_insts->UpdatePswReg ();

    UpdateSideband ();

#ifdef SUPPORT_CUBESUITE
    // Notice CpuOperationMode update by CS+ callback
    CpuOperationMode mode;
    uint32_t gpid;
    if (IsHvEnabled ()) {
        if (IsHostMode ()) {
            mode = HOST_MODE;
        } else {
            mode = GUEST_MODE;
        }
        gpid = srf->SrPSWH()->GetGPID ();
    } else {
        mode = CONVENTIONAL_MODE;
        gpid = 0;
    }
    (*CubeCallBack::__NotifyCpuOperationMode) (GetPeId(), mode, gpid);

    // Notice SPID update by CS+ callback
    (*CubeCallBack::__NotifySPID) (GetPeId(), srf->SrVSPID()->GetSPID ());
#endif // #ifdef SUPPORT_CUBESUITE
}


// Update SIdeband contents which are related with with system register settings.
// - GM/GPID/SPID/BMID/DM/UM
//   Because GM/GPID/SPID/BMID/DM/UM are related with system register settings,
//   they are updated by register contents in this function.
// - BitPos/BitOp/SPCLD
//   Because BitPos/BitOp/SPCLD aren't related with system register settings,
//   they are updated when memory access is issued for each.
// - Secure
//   Fixed to 0 in G4MH PE.
void ProcElement::UpdateSideband (void)
{
#ifdef SUPPORT_SC_HEAP
    // Check Updating GM/GPID event for SC-HEAP.
    // Because SideBand info is used to get old value, call this checking before updating Sideband.
    if (IsGmGpidUpdated ()) {
        GetHtPtr()->m_trace->SetPswhChanged (true);
    }
#endif // #ifdef SUPPORT_SC_HEAP

    // Update SideBand : UM, SPID, GM, GPID
    SideBand* sb_ptr = GetHtPtr()->m_lsu->GetSideBandPtr ();
    SRegFile* srf = GetSrfPtr(NC);
    sb_ptr->SetUM (srf->SrVPSW()->GetUM ());
    sb_ptr->SetSPID (srf->SrVSPID()->GetSPID ());
    if (IsHvEnabled () == true) {
        sb_ptr->SetGM (srf->SrPSWH()->GetGM ());
        sb_ptr->SetGPID (srf->SrPSWH()->GetGPID ());
    } else {
        sb_ptr->SetGM (0);
        sb_ptr->SetGPID (0);
    }

    // BMID
    sb_ptr->SetBMID (srf->SrBMID()->GetBMID ());

    // DM
    sb_ptr->SetDM (srf->SrDIR0()->GetDM ());

    // Keep BitPos/BitOp/SPCLD (These params are updated when an access is issued.)
}


void ProcElement::ChangeMpuBank (uint32_t bank)
{
    // Copy Guest registers in m_nc_srf to mirror registers in srfs.
    SRegFile* srf = GetSrfPtr(NC);

    // get current bank
    CfSReg mpla_regid;
    CfSReg mpua_regid;
    CfSReg mpat_regid;
    if (bank == 0) {
        mpla_regid = CF_SR_MPLA0;
        mpua_regid = CF_SR_MPUA0;
        mpat_regid = CF_SR_MPAT0;
    } else if (bank == 1) {
        mpla_regid = CF_SR_MPLA0_B1;
        mpua_regid = CF_SR_MPUA0_B1;
        mpat_regid = CF_SR_MPAT0_B1;
    } else {
        CfMsg::DPrint (MSG_ERR, "<Internal Error: MPBK>1 isn't supported yet>\n");
        return;
    }

    // Update virtual registers
    uint32_t n_mpu = GetNumMpuRegion();
    for (uint32_t id = 0; id < n_mpu; ++id) {
        SRegBase* mpla = m_nc_srf->GetSrPtr (static_cast<CfSReg>(static_cast<uint32_t>(mpla_regid) + id));
        SRegBase* mpua = m_nc_srf->GetSrPtr (static_cast<CfSReg>(static_cast<uint32_t>(mpua_regid) + id));
        SRegBase* mpat = m_nc_srf->GetSrPtr (static_cast<CfSReg>(static_cast<uint32_t>(mpat_regid) + id));
        srf->SetSrPtr(static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPLA0) + id), mpla);
        srf->SetSrPtr(static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPUA0) + id), mpua);
        srf->SetSrPtr(static_cast<CfSReg>(static_cast<uint32_t>(CF_SR_MPAT0) + id), mpat);
    }
    ReallocMpuRegion ();
}



void ProcElement::UpdateHvStatus (void)
{
    if (  (GetCoreType() == CORE_G4MH) && (GetCoreVersion() >= CORE_VER_20)
       && (m_has_hv == 1)
       && (m_nc_srf->SrHVCFG ()->GetHVE () == 1) ) {
        m_hv_enabled = true;
        if (m_nc_srf->SrPSWH ()->GetGM () == 0) {
            m_is_host_mode = true;
        } else {
            m_is_host_mode = false;
        }
    } else {
        m_hv_enabled = false;
        m_is_host_mode = false;
    }
}

void ProcElement::UpdateHtActive (void) const
{
    HtVec::const_iterator it  = m_hts.begin ();
    HtVec::const_iterator end = m_hts.end ();
    while (it != end) {
        (*it)->UpdateHtActive ();
        ++ it;
    }
}


void ProcElement::UpdateRunnableByIntreq (void)
{
    // NM宛例外とVM宛例外が同時に存在した場合、
    // NM宛例外の優先度が高いので、NM宛例外が発生しないといけない。
    // - NM宛例外要求が存在し、NMがそれを受付可能な場合、
    //   VMの例外を抑止する
    if (m_hts.empty ()) {
        return;
    }

    bool has_intreq_to_nc = (GetHtPtr ()->GetSelectedIntFactor () != FsCtrl::INT_FAC_NONE);

    // VMの例外(FENMI, SYSERR, FEINT, FPI, EIINT)を抑止する
    HtVec::const_iterator it  = m_hts.begin ();
    HtVec::const_iterator end = m_hts.end ();
    while (it != end) {
        (*it)->SetHasIntreqToNc (has_intreq_to_nc);
        (*it)->UpdateHtRunable ();
        ++ it;
    }
}


void ProcElement::UpdatePrivLevel (void) const
{
    HtVec::const_iterator it  = m_hts.begin ();
    HtVec::const_iterator end = m_hts.end ();
    while (it != end) {
        (*it)->UpdatePrivLevel ();
        ++ it;
    }
}


void ProcElement::ClearBtb (void) const
{
    HtVec::const_iterator it  = m_hts.begin ();
    HtVec::const_iterator end = m_hts.end ();
    while (it != end) {
        (*it)->m_btb->ClearBtb ();
        ++ it;
    }
}


void ProcElement::HardReset (void)
{
    PeBaseReset();

    std::for_each (m_hts.begin(), m_hts.end(), std::mem_fun (&FsCtrl::HardReset));
    NoticeCpuGuestModeToIntc (0);
    NoticeCpuGpidToIntc (0);
}


bool ProcElement::FsReset (void)
{
    PeBaseReset();

    std::for_each (m_hts.begin(), m_hts.end(), std::mem_fun (&FsCtrl::FsReset));
    NoticeCpuGuestModeToIntc (0);
    NoticeCpuGpidToIntc (0);
    return true;
}


void ProcElement::PeBaseReset (void)
{
    m_ic->Reset ();
    m_nc_srf->Reset ();
    m_grfs->Reset ();
    m_peg->HardReset();
#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_intc1->HardReset();
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_dbuf->HardReset ();
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    m_flc->HardReset ();
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    m_misr->HardReset ();

#ifdef ENABLE_OSTM
    for (uint32_t ch = 0; ch < MAX_OSTM_NUM; ch++) {
        if (m_ostm[ch] != NULL) {
            m_ostm[ch]->HardReset();
        }
    }
#endif // #ifdef ENABLE_OSTM
}


void ProcElement::SetExtPin (ExtPinType pin, uint32_t data)
{
    switch (pin) {
    case EXT_PIN_PEID:    m_peid_extpin  = data; break;
    case EXT_PIN_SPID:    m_spid_extpin  = data; break;
    case EXT_PIN_SPIDLIST: m_spidlist_extpin = data; break;
    case EXT_PIN_BMID:    m_bmid_extpin  = data; break;
    case EXT_PIN_RBASE:   m_rbase_extpin = data; break;
    case EXT_PIN_DV:      m_dv_extpin    = data; break;
    case EXT_PIN_RINT:    m_rint_extpin  = data; break;
    case EXT_PIN_ICHEN:   m_ichen_extpin = data; break;
    case EXT_PIN_HAS_FPU:  SetHasFpu  (data & 0x01); break;
    case EXT_PIN_HAS_FXU:  SetHasFxu  (data & 0x01); break;
    case EXT_PIN_HAS_MPU:  SetHasMpu  (data & 0x01); break;
    case EXT_PIN_MPTRGN:       SetNumMpuRegion (data);       break;
    case EXT_PIN_MPBANK:        if ((data >= 1) && (data <= 2)) { // Accept only 1 or 2.
                                    SetNumMpuBank (data);
                                }
                                break;
    case EXT_PIN_MDP_DMDP:     SetMpuDmdp (data & 0x01);     break;
    case EXT_PIN_SNZ_PERIOD:   SetSnoozePeriod (data & 0xFFFF); break;
    case EXT_PIN_NUM_L1RLINK:  m_num_l1rlink_extpin = data;  break;
    case EXT_PIN_SIZE_L1R:     m_size_l1r_extpin = data;  break;
    case EXT_PIN_HAS_HV:
        m_has_hv = data & 0x1;
#if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
        // Set param to INTC as same as extpin of PE
        m_intc1->SetHasHVExtPin (GetExtPin (EXT_PIN_HAS_HV));
#endif // #if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
        break;
    case EXT_PIN_HVE:
        m_hve_extpin = data & 0x1;
#if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
        // Set param to INTC as same as extpin of PE
        m_intc1->SetHVEExtPin (GetExtPin (EXT_PIN_HVE));
#endif // #if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
        break;
    case EXT_PIN_LSCFG_LSE:    m_has_lockstep = data & 0x1; break;
    case EXT_PIN_DBGEN:        m_dbgen_extpin = data & 0x1FF; break;
    case EXT_PIN_NONE:         break;
    }
}


uint32_t ProcElement::GetExtPin (ExtPinType pin) const
{
    switch (pin) {
    case EXT_PIN_PEID:    return m_peid_extpin;  break;
    case EXT_PIN_SPID:    return m_spid_extpin;  break;
    case EXT_PIN_SPIDLIST: return m_spidlist_extpin; break;
    case EXT_PIN_BMID:    return m_bmid_extpin;  break;
    case EXT_PIN_RBASE:   return m_rbase_extpin; break;
    case EXT_PIN_DV:      return m_dv_extpin;    break;
    case EXT_PIN_RINT:    return m_rint_extpin;  break;
    case EXT_PIN_ICHEN:   return m_ichen_extpin; break;
    case EXT_PIN_HAS_FPU: return HasFpu (); break;
    case EXT_PIN_HAS_FXU: return HasFxu (); break;
    case EXT_PIN_HAS_MPU: return HasMpu (); break;
    case EXT_PIN_MPTRGN:       return GetNumMpuRegion ();    break;
    case EXT_PIN_MPBANK:       return GetNumMpuBank ();      break;
    case EXT_PIN_MDP_DMDP:     return GetMpuDmdp ();         break;
    case EXT_PIN_SNZ_PERIOD:   return GetSnoozePeriod ();    break;
    case EXT_PIN_NUM_L1RLINK:  return m_num_l1rlink_extpin;  break;
    case EXT_PIN_SIZE_L1R:     return m_size_l1r_extpin;  break;
    case EXT_PIN_HAS_HV:       return m_has_hv; break;
    case EXT_PIN_HVE:          return m_hve_extpin; break;
    case EXT_PIN_LSCFG_LSE:    return m_has_lockstep; break;
    case EXT_PIN_DBGEN:        return m_dbgen_extpin; break;
    case EXT_PIN_NONE:         return 0x0; break;
    }

    return 0x0; // ここには来ない
}


void ProcElement::SetSnoozePeriod (uint16_t period)
{
    m_snooze_period = period;

    HtVec::iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->SetSnoozePeriod (period);
        ++it;
    }
}


uint16_t ProcElement::GetSnoozePeriod (void) const
{
    return m_snooze_period;
}


void ProcElement::SetSupportTableRefInt (bool is_supported)
{
    HtVec::iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->SetSupportTableRefInt (is_supported);
        ++it;
    }
}


bool ProcElement::IsTableRefIntSupported (void) const
{
    if (m_hts.empty ()) {
        return false; // ここには来ないはず
    } else {
        return m_hts.front ()->IsTableRefIntSupported ();
    }
}


bool ProcElement::IsIcEnabled (void) const
{
    return m_nc_srf->SrICCTRL()->GetICHEN ();
}


void ProcElement::ShowPerfInfo (void) const
{
    HtVec::const_iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->ShowPerfInfo ();
        ++it;
    }
    m_ic->ShowPerfInfo ();
    m_dbuf->ShowPerfInfo ();
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    m_flc->ShowPerfInfo ();
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
}


void ProcElement::ResetPerfInfo (void)
{
    HtVec::iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->ResetPerfInfo ();
        ++it;
    }
    m_ic->ResetPerfInfo ();
    m_dbuf->ResetPerfInfo ();
#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    m_flc->ResetPerfInfo ();
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
}


bool ProcElement::IsDcEnabled (void) const
{
    return false;
}


void ProcElement::SetGramArea (PhysAddr start_addr, PhysAddr end_addr)
{
    HtVec::iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->SetGramArea (start_addr, end_addr);
        ++it;
    }
}


void ProcElement::SetLramArea (PhysAddr start_addr, PhysAddr end_addr)
{
    HtVec::iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->SetLramArea (start_addr, end_addr);
        ++it;
    }
}


void ProcElement::ReallocMpuRegion (void)
{
    //! MPUクラスの全領域をクリア
    GetHtPtr()->m_lsu->GetMpuPtr()->AllDelMpuRegion ();

    //! MPUクラスの情報を再構成
    uint32_t n_mpu = GetNumMpuRegion ();
    for (uint32_t rgn_id = 0; rgn_id < n_mpu; ++rgn_id) {
        //! いずれのVMにも所属しない MPU領域は NCが対象
        SRegMPAT_N* nc_mpat = static_cast<SRegMPAT_N*> (
            GetSrfPtr(NC)->GetSrPtr (CF_SR_MPAT0 + rgn_id)); // Use virtual regfile for mpu bank
        if (nc_mpat->GetE() == true) {
            GetHtPtr()->m_lsu->GetMpuPtr()->AddMpuRegion (rgn_id);
        }
    }
}


void ProcElement::SetCfgPtr (ModuleGuard* cfg)
{
    HtVec::iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->m_lsu->SetCfgPtr (cfg);
        ++it;
    }

    m_ic->SetCfgPtr (cfg);
}


void ProcElement::SetGrgPtr (ModuleGuard* grg)
{
    HtVec::iterator it = m_hts.begin ();
    while ( it != m_hts.end () ) {
        (*it)->m_lsu->SetGrgPtr (grg);
        ++it;
    }
}


ModuleGuard* ProcElement::GetGrgPtr (void)
{
    HtVec::iterator it = m_hts.begin ();
    if (it != m_hts.end ()) {
        return (*it)->m_lsu->GetGrgPtr ();
    }
    return NULL;
}


uint32_t ProcElement::GetBmId (void) const
{
    return GetHtPtr()->m_srf->SrBMID()->GetBMID ();
}


#ifdef ENABLE_PEID_POS_CTRL
bool ProcElement::IsPeidPosG4x (void) const
{
    return p_internals->IsPeidPosG4x();
}
#endif // #ifdef ENABLE_PEID_POS_CTRL


ProcElement::ProcElement (uint32_t pe_id, uint32_t cl_id, CoreType core_type, CoreVersion core_version,
                          ArchClass arch_class, FsBus* fsbus, Llsc* llsc)
    : m_pe_id (pe_id), m_cl_id (cl_id), m_core_type (core_type), m_core_version (core_version), m_arch_class (arch_class),
      m_fsbus (fsbus), m_llsc (llsc)
{
    RegID::InitIdMap ();

    m_has_fpu  = true;
    m_has_fxu  = false;
    m_has_mpu = true;
    m_has_lockstep = false;

    m_peid_extpin  = pe_id;
    m_spid_extpin  = pe_id;
    m_bmid_extpin  = pe_id;
    m_mm_extpin    = 0;
    m_rbase_extpin = 0;
    m_dv_extpin  = 0;
    m_rint_extpin  = 0;
    m_ma_extpin    = 0;
    m_spidlist_extpin = 0xFFFFFFFF;
    m_mpu_dmdp = 0;
    m_dbgen_extpin = 0;

    m_ichen_extpin = 1;

    m_has_mpm_dbit_extpin = 0;
    m_machine_mode = MACHINE_FIX_NM; // valid in E3V5_S
    m_snooze_period = INIT_SNOOZE_PERIOD;

    m_num_l1rlink_extpin = 2;
    m_size_l1r_extpin = 0x20; // 32KB

    m_num_mpu_bank = 1;

    if ((core_type == CORE_G4MH) && (core_version >= CORE_VER_20)) {
        m_has_hv = true;
        m_hve_extpin = 1;
        m_hv_enabled = true;
        m_num_mpu_region = 32;
        m_is_host_mode = true;
    } else {
        m_has_hv = false;
        m_hve_extpin = 0;
        m_hv_enabled = false;
        m_num_mpu_region = 24;
        m_is_host_mode = false;
    }

    // 全ROM領域がキャッシング対象領域
    // 他PEのROM領域がキャッシング不可となっても、CForestでは非サポート
    ByteSize ic_size;
    CacheWayType way_type;
    ic_size = ICACHE_SIZE_16K;
    way_type = WAY_PBS;
    if (core_type == CORE_G4KH) {
        ic_size = ICACHE_SIZE_8K;
        way_type = WAY2;
    }

    ByteSize ic_line_size;
#ifdef USE_IC_LINE_SIZE_CMDOPT
    ic_line_size = p_internals->GetIcLineByte ();
#else // USE_IC_LINE_SIZE_CMDOPT
    ic_line_size = 32;
#endif // USE_IC_LINE_SIZE_CMDOPT
    if (core_type == CORE_G4KH) {
        ic_line_size = 16;
    }
    m_ic = new Icache (ic_size, way_type, fsbus, Lsu::ROM_BGN,
                       Lsu::ROM_END, pe_id,
                       ic_line_size, core_type);

    m_misr = new Misr (p_forest_util->GetPeripheralBaseAddr (BaseAddr_MISR), pe_id);

    // Get address value from GMID_PEG_PEnCLm with suitable peid by user setting.
    PhysAddr peg_ctrl_addr = p_forest_util->GetGuardAddr (GMID_PEG_PEnCLm, pe_id);
    m_peg = new Peg (pe_id, peg_ctrl_addr, core_type, core_version);

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_intc1 = NULL;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_intc1 = new CIntc1 (this, pe_id, fsbus, core_type, core_version);
    // Set initial param as same as extpin of PE
    m_intc1->SetHasHVExtPin (GetExtPin (EXT_PIN_HAS_HV));
    m_intc1->SetHVEExtPin (GetExtPin (EXT_PIN_HVE));
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

    uint32_t num_nb_buf;
    uint32_t num_lsu_buf;
    num_nb_buf  = NUM_NB_BUF_G3MH;
    num_lsu_buf = NUM_LSU_BUF_G3MH;
    if (core_type == CORE_G4KH) {
        num_nb_buf  = NUM_NB_BUF_G3KH;
        num_lsu_buf = NUM_LSU_BUF_G3KH;
    }
    m_cs_lsu_buf = new CsLsuBuf (num_lsu_buf, num_nb_buf);

    m_sreg_dummy = new SRegDUMMY ();
    m_sreg_dummy_hv = new SRegDUMMYHV (this);

    //! ネイティブ・コンテキスト用に各リソースを追加
    NcSRegFile* nc_srf = new NcSRegFile (NC, this, m_sreg_dummy);
    nc_srf->InitSRegs(NC, this);
    m_nc_srf = nc_srf;

    GRegFile* grf = new GRegFile (NC, NULL, WRegLine::FXU4_FULL);
    m_grfs = new GRegFileSet (grf);

    SRegFile* srf = new SRegFile (nc_srf);
    m_srfs = new SRegFileSet (srf);
    RegID::InitRegNameMap (srf);

#ifdef USE_DBUF_CMDOPT
    m_dbuf = new Dbuf (pe_id, p_internals->GetDbufDepth (),
                       p_internals->GetDbufLineShift (), p_internals->GetDbufFillBurstNum (),
                       DBUF_REP_ROUNDR, 32, core_type);
#else // USE_DBUF_CMDOPT
    uint32_t dbuf_depth;
    uint32_t dbuf_line_shift;
    uint32_t dbuf_burst_num;
    if (core_type == CORE_G4MH) {
        dbuf_depth      = 8;
        dbuf_line_shift = 4;
        dbuf_burst_num  = 1;
        m_dbuf = new Dbuf (pe_id, dbuf_depth,
                            dbuf_line_shift, dbuf_burst_num, DBUF_REP_ROUNDR, 32, core_type);
    } else {
        dbuf_depth      = 1;
        dbuf_line_shift = 4;
        dbuf_burst_num  = 1;
        m_dbuf = new Dbuf (pe_id, dbuf_depth,
                           dbuf_line_shift, dbuf_burst_num, DBUF_REP_DIRECT, 16, core_type);
    }
#endif // USE_DBUF_CMDOPT

#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    m_flc = new FlashCache (pe_id);
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)

#ifdef ENABLE_DBG_BREAK
    m_dbgbrk = new DbgBreak (srf, core_type, core_version);
#endif // #ifdef ENABLE_DBG_BREAK

    // LLSC satisfaction condition setting.
    m_llsc->SetCondMask (~0x00U);

    FsCtrl* ht = new FsCtrl (pe_id, NC, NC, m_arch_class,
                             fsbus, m_ic, m_intc1,
#ifdef ENABLE_DBG_BREAK
                             m_dbgbrk,
#endif // #ifdef ENABLE_DBG_BREAK
                             m_llsc,
                             m_misr, this,
                             grf, srf,
                             m_nc_srf, m_grfs, m_cs_lsu_buf,
                             m_core_type, m_core_version);
    m_hts.push_back (ht);

#ifdef ENABLE_OSTM
    for (uint32_t ch = 0; ch < MAX_OSTM_NUM; ch++) {
        m_ostm[ch] = NULL;
    }
#endif // #ifdef ENABLE_OSTM
}

ProcElement::~ProcElement ()
{
    for (HtVec::iterator it = m_hts.begin(); it != m_hts.end(); ++it) {
        delete *it;
    }
#ifdef ENABLE_DBG_BREAK
    delete m_dbgbrk;
#endif // #ifdef ENABLE_DBG_BREAK
    delete m_srfs;
    delete m_grfs;
    delete m_nc_srf;
    delete m_sreg_dummy;
    delete m_sreg_dummy_hv;
    delete m_intc1;
    delete m_ic;
    delete m_peg;
    delete m_cs_lsu_buf;
    delete m_dbuf;
    delete m_misr;

#ifdef ENABLE_OSTM
    for (uint32_t ch = 0; ch < MAX_OSTM_NUM; ch++) {
        delete m_ostm[ch];
    }
#endif // #ifdef ENABLE_OSTM

#if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
    delete m_flc;
#endif // #if defined (ENABLE_FLC_I) || defined (ENABLE_FLC_D)
}

void ProcElement::SetIntc1(void)
{
    delete m_intc1;
    m_intc1 = new CIntc1 (this, m_pe_id, m_fsbus, m_core_type, m_core_version);
    m_intc1->HardReset();

    for (HtVec::iterator it = m_hts.begin(); it != m_hts.end(); ++it) {
        (*it)->m_intc1 = this->m_intc1;
    }
}


void ProcElement::NoticeCpuGuestModeToIntc (bool is_gm)
{
// In case of SC-HEAP, no need to set this event to INTC1 internally.
// When PSWH.GM/GPID is updated,
// simulation is stopped with BRK_CHANGE_PSWH.
// Then, SC-HEAP reads PSWH.GM/GPID bit by RegReadDebug() and sets to INTC1.
#if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
    if ((GetCoreType () == CORE_G4MH) && (GetCoreVersion () >= CORE_VER_20)) {
        m_intc1->SetCpuGuestMode (is_gm);
        p_internals->SetCpuGuestModeToIntc2 (m_pe_id, is_gm);
    }
#endif // #if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
}


void ProcElement::NoticeCpuGpidToIntc (uint32_t gpid)
{
// In case of SC-HEAP, no need to set this event to INTC1 internally.
// When PSWH.GM/GPID is updated,
// simulation is stopped with BRK_CHANGE_PSWH.
// Then, SC-HEAP reads PSWH.GM/GPID bit by RegReadDebug() and sets to INTC1.
#if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
    if ((GetCoreType () == CORE_G4MH) && (GetCoreVersion () >= CORE_VER_20)) {
        m_intc1->SetCpuGpid (gpid);
        p_internals->SetCpuGpidToIntc2 (m_pe_id, gpid);
    }
#endif // #if !defined(SUPPORT_SC_HEAP) || defined(TEST_SC_HEAP)
}


#ifdef ENABLE_OSTM
void ProcElement::SetOstm (uint32_t ch, PhysAddr addr, uint32_t intch, CIntc2* intc2)
{
    delete m_ostm[ch];
    m_ostm[ch] = new OstmIO(m_pe_id, m_intc1, intc2, addr, intch, ch, m_fsbus,
                             p_forest_util->GetClkRatioOstm ());
    m_ostm[ch]->HardReset();
}
#endif // #ifdef ENABLE_OSTM
