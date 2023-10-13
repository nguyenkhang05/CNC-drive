/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include "./config.h"
#include "./forest.h"
#include "./fsbus.h"
#include "./sideband.h"
#include "./llsc.h"
#include "./smem.h"
#include "./intc1.h"
#include "./intc2.h"
#include "./ipir.h"
#include "./bar.h"
#include "./mecnt.h"
#include "./proc_element.h"
#include "./forest_utility.h"
#include "./forest_internals.h"
#include "./cedar_factory.h"
#include "./module_guard.h"
#include "./dbg_area.h"
#ifdef ENABLE_OSTM
#include "./os_timer.h"
#endif // #ifdef ENABLE_OSTM
#include "./outstand_mem.h"
#include "./misr.h"
#include "./peg.h"
#include "./crg.h"
#include "./fs_controler.h"
#include "./lsu.h"
#include "./error_area.h"

#ifdef ENABLE_TMU
#include "./timer.h"
#endif // ENABLE_TMU

#ifdef USE_SIM_IO
#include "./simio.h"
#endif // USE_SIM_IO
#ifdef ENABLE_PERFCNT
#include "./perfcnt.h"
#endif // ENABLE_PERFCNT
#ifdef ENABLE_UART
#include "./uart.h"
#endif // ENABLE_UART

#ifdef SUPPORT_CUBESUITE
#include "./cube_ior.h"
#endif // SUPPORT_CUBESUITE

#ifdef SUPPORT_SC_HEAP
#include "./invalid_lram.h"
#endif // #ifdef SUPPORT_SC_HEAP

ForestUtil* p_forest_util = NULL;
ForestInternals* p_internals = NULL;

#ifdef ENABLE_PERFCNT
const PhysAddr Cforest::PERFCNT_BASE = 0xFFFEED00U;
#endif // ENABLE_PERFCNT

void Cforest::Reset ()
{
    PeVec::const_iterator it = m_pe.begin ();
    while (it != m_pe.end ()) {
        if ((*it) != NULL) {
            (*it)->HardReset ();
        }
        ++ it;
    }

    if (m_is_gen_peripherals == false) {
        return;
    }

    std::for_each (m_smem.begin(), m_smem.end(),
                   std::mem_fun (&Csmem::HardReset));

    std::for_each (m_pegcap.begin(), m_pegcap.end(),
                   std::mem_fun (&PegCap::HardReset));

    std::for_each (m_crgcap.begin(), m_crgcap.end(),
                   std::mem_fun (&CrgCap::HardReset));

    std::for_each (m_crg.begin(), m_crg.end(),
                   std::mem_fun (&Crg::HardReset));

#ifdef SUPPORT_CUBESUITE
    std::for_each (m_cube_iors.begin (), m_cube_iors.end (),
                   std::mem_fun (&CubeIoReg::HardReset));

    std::vector<CubeLiorInfo>::const_iterator it_lior =  m_cube_lior.begin ();
    while ( it_lior != m_cube_lior.end () ) {
        it_lior->second->HardReset ();
        ++ it_lior;
    }
#endif //#ifdef SUPPORT_CUBESUITE

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    intc2->HardReset ();
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

#ifdef ENABLE_TMU
    if (tmu0 != NULL) {
        tmu0->HardReset ();
    }
    if (tmu1 != NULL) {
        tmu1->HardReset ();
    }
    if (tmu2 != NULL) {
        tmu2->HardReset ();
    }
    if (tmu3 != NULL) {
        tmu3->HardReset ();
    }
#endif // ENABLE_TMU

#ifdef ENABLE_OSTM
    std::vector<OstmIO*>::const_iterator it_ostm = m_ostm.begin ();
    while (it_ostm != m_ostm.end()) {
        (*it_ostm)->HardReset ();
        ++ it_ostm;
    }
#endif // #ifdef ENABLE_OSTM

    if (m_ipir != NULL) {
        m_ipir->HardReset ();
    }

    if (m_bar != NULL) {
        m_bar->HardReset ();
    }

    if (m_mecnt != NULL) {
        m_mecnt->HardReset ();
    }

#ifdef USE_SIM_IO
    simio->HardReset ();
#endif // USE_SIM_IO
#ifdef ENABLE_PERFCNT
    std::vector<Cperfcnt*>::const_iterator it_perfcnt = m_perfcnt.begin ();
    while ( it_perfcnt != m_perfcnt.end () ) {
        if ((*it_perfcnt) != NULL) {
            (*it_perfcnt)->HardReset ();
        }
        ++ it_perfcnt;
    }
#endif // ENABLE_PERFCNT

#ifdef ENABLE_UART
    if (uart != NULL) {
        uart->HardReset ();
    }
#endif // ENABLE_UART

#ifdef SUPPORT_SC_HEAP
    m_invalid_lram->HardReset ();
#endif // #ifdef SUPPORT_SC_HEAP

    m_dbg_area->HardReset ();
}


void Cforest::GenPeripherals (void)
{
    if (m_is_gen_peripherals == true) {
        // 生成済み
        return;
    }
    CoreVersion core_version = CORE_VER_10;
    PeVec::const_iterator it_pe = m_pe.begin ();
    while (it_pe != m_pe.end ()) {
        if ((*it_pe) != NULL) {
            core_version = (*it_pe)->GetCoreVersion ();
            break;
        }
       ++ it_pe;
    }
    // 周辺のインスタンス生成
    smem0 = new Csmem (MemBody::ACC_PERMITTED, p_forest_util->GetMemInit ());
    m_smem.push_back (smem0);

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    intc2 = NULL;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    Intc2ImplType intc2_impl_type = ((core_version >= CORE_VER_20) ? INTC2_G4MH20:INTC2_STANDARD);
    intc2 = new CIntc2 (&m_pe, fsbus, intc2_impl_type);
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

#ifdef ENABLE_TMU
    tmu0 = new CTimer (0, fsbus, intc2);
    tmu1 = new CTimer (1, fsbus, intc2);
    tmu2 = new CTimer (2, fsbus, intc2);
    tmu3 = new CTimer (3, fsbus, intc2);
#endif // ENABLE_TMU

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_mecnt = NULL;
    m_ipir = NULL;
    m_bar = NULL;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    IpirImplType ipir_impl_type = IPIR_STANDARD;
    m_ipir = new CIpir (&m_pe, ipir_impl_type);

    BarImplType bar_impl_type = BAR_STANDARD;
    m_bar = new CBar (&m_pe, fsbus, bar_impl_type);

    m_mecnt = new Mecnt ();
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

#ifdef ENABLE_PERFCNT
    PeVec::const_iterator it_perf = m_pe.begin ();
    while (it_perf != m_pe.end ()) {
        Cperfcnt* perfcnt = NULL;
        if ((*it_perf) != NULL) {
            perfcnt = new Cperfcnt ((*it_perf)->GetClId (), (*it_perf)->GetPeId ());
        }
        m_perfcnt.push_back (perfcnt);
        ++ it_perf;
    }
#endif // ENABLE_PERFCNT

#ifdef USE_SIM_IO
    simio = new Csimio(&m_pe);
#endif // USE_SIM_IO

#ifdef ENABLE_UART
    if (p_forest_util->IsUartInstance () == true) {
        PhysAddr uart_addr = m_peri_addr[BaseAddr_UART].GetAddr (0);
        uart = new Uart (fsbus, intc2, uart_addr);
    } else {
        uart = NULL;
    }
#endif // ENABLE_UART

#ifdef SUPPORT_SC_HEAP
    m_invalid_lram = new InvalidLram ();
#endif // #ifdef SUPPORT_SC_HEAP

    m_is_gen_peripherals = true;

    /* Connect modules to Bus */
#ifdef ENABLE_TMU
    if (m_peri_addr[BaseAddr_TMU].IsEnabled ()) {
        PhysAddr tmu_addr = m_peri_addr[BaseAddr_TMU].GetAddr (0);
        fsbus->AddTargetVC (tmu_addr + 0x00000U, tmu_addr + 0x0FFFFU, tmu0, NULL, m_dummy_guard,
                            CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
        fsbus->AddTargetVC (tmu_addr + 0x10000U, tmu_addr + 0x1FFFFU, tmu1, NULL, m_dummy_guard,
                            CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
        fsbus->AddTargetVC (tmu_addr + 0x30000U, tmu_addr + 0x3FFFFU, tmu2, NULL, m_dummy_guard,
                            CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
        fsbus->AddTargetVC (tmu_addr + 0x40000U, tmu_addr + 0x4FFFFU, tmu3, NULL, m_dummy_guard,
                            CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
    }
#endif // ENABLE_TMU

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    if (m_peri_addr[BaseAddr_INTC2].IsEnabled ()) {
        PhysAddr intc2_addr = m_peri_addr[BaseAddr_INTC2].GetAddr (0);
        fsbus->AddTargetVC (intc2_addr,
                            intc2_addr + (CIntc2::INTC2_REG_SIZE - 1), intc2, NULL,
                            m_dummy_guard, CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
    }
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

    FsBus* crg_bus;
#if defined(SUPPORT_SC_HEAP)
    // In case of SC-HEAP, CRG registers is protected by APB in SC-HEAP.
    // Because access is goes thought SC-HEAP, connect CRG registers to m_crg_bus.
    // Route is CForest (LSU)->FSBUS->SC-HEAP->CrgRegAccess->m_crg_bus->CRG registers
    crg_bus = m_crg_bus;
#else // defined(SUPPORT_SC_HEAP)
    crg_bus = fsbus;
#endif // defined(SUPPORT_SC_HEAP)
    // CRG implementation.
    uint32_t num_clid = CLID_MASK + 1;
    for (uint32_t cl_index = 0; cl_index < num_clid; cl_index++ ) {
        // Get CoreType and CoreVersion from 1st PE setting.
        PeVec::const_iterator it_1st_pe_crg = m_pe.begin ();
        // Default is G4MHv2.0.
        CoreType core_type_1st_pe = CORE_G4MH;
        CoreVersion core_version_1st_pe = CORE_VER_20;
        if ((*it_1st_pe_crg) != NULL) {
            core_type_1st_pe = (*it_1st_pe_crg)->GetCoreType();
            core_version_1st_pe = (*it_1st_pe_crg)->GetCoreVersion();
        }

        // There are 7 modules in CRG: 1 control module and 6 error modules.
        if (p_forest_util->IsEnableGuard (GMID_CRG_CLm, cl_index)) {
            if ((core_type_1st_pe != CORE_G4MH) || (core_version_1st_pe < CORE_VER_20)) {
                // There is no IVC and CSG module in G4MH1x.
                PhysAddr crg_ctrl_addr = p_forest_util->GetGuardAddr (GMID_CRG_CLm, cl_index);
                Crg *crg = new Crg (crg_ctrl_addr, 0x0U, 0x0U, core_type_1st_pe, core_version_1st_pe, cl_index);
                crg_bus->AddTargetVC (crg_ctrl_addr, crg_ctrl_addr + (Crg::CRG_REG_SIZE_G4MH1X - 1),
                                    crg, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
                m_crg.push_back(crg);
            } else {
                PhysAddr crg_ctrl_addr = p_forest_util->GetGuardAddr (GMID_CRG_CLm, cl_index);
                PhysAddr crg_ivc_addr = p_forest_util->GetGuardAddr  (GMID_CRG_IVC_CLm, cl_index);
                PhysAddr crg_csc_addr = p_forest_util->GetGuardAddr  (GMID_CRG_CSG_CLm, cl_index);
                Crg *crg = new Crg (crg_ctrl_addr, crg_ivc_addr, crg_csc_addr, core_type_1st_pe, core_version_1st_pe, cl_index);
                crg_bus->AddTargetVC (crg_ctrl_addr, crg_ctrl_addr + (Crg::CRG_REG_SIZE_G4MH2X - 1),
                                    crg, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
                crg_bus->AddTargetVC (crg_ivc_addr, crg_ivc_addr + (Crg::CRG_REG_SIZE_G4MH2X - 1),
                                    crg, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
                crg_bus->AddTargetVC (crg_csc_addr, crg_csc_addr + (Crg::CRG_REG_SIZE_G4MH2X - 1),
                                    crg, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
                m_crg.push_back(crg);
            }
        }
        if (p_forest_util->IsEnableGuard (GMID_CRGCAP_CRAMHCLm, cl_index)) {
            PhysAddr crg_cap_cramh_addr = p_forest_util->GetGuardAddr (GMID_CRGCAP_CRAMHCLm, cl_index);
            CrgCap *crgcap_cramh = new CrgCap (crg_cap_cramh_addr, core_type_1st_pe, core_version_1st_pe);
            p_internals->SetGuardCapErr(GMID_CRGCAP_CRAMHCLm, cl_index, static_cast<TRegModuleVc *> (crgcap_cramh->GetCrgErrReg()));
            if ((core_type_1st_pe != CORE_G4MH) || (core_version_1st_pe < CORE_VER_20)) {
                crg_bus->AddTargetVC (crg_cap_cramh_addr, crg_cap_cramh_addr + (Crg::CRG_ERR_REG_CRAM_SIZE_G4MH1X - 1),
                                    crgcap_cramh, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            } else {
                crg_bus->AddTargetVC (crg_cap_cramh_addr, crg_cap_cramh_addr + (Crg::CRG_ERR_REG_CRAM_SIZE_G4MH2X - 1),
                                    crgcap_cramh, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            }
            m_crgcap.push_back(crgcap_cramh);
        }
        if (p_forest_util->IsEnableGuard (GMID_CRGCAP_CRAMLCLm, cl_index)) {
            PhysAddr crg_cap_craml_addr = p_forest_util->GetGuardAddr (GMID_CRGCAP_CRAMLCLm, cl_index);
            CrgCap *crgcap_craml = new CrgCap (crg_cap_craml_addr, core_type_1st_pe, core_version_1st_pe);
            p_internals->SetGuardCapErr(GMID_CRGCAP_CRAMLCLm, cl_index, static_cast<TRegModuleVc *> (crgcap_craml->GetCrgErrReg()));
            if ((core_type_1st_pe != CORE_G4MH) || (core_version_1st_pe < CORE_VER_20)) {
                crg_bus->AddTargetVC (crg_cap_craml_addr, crg_cap_craml_addr + (Crg::CRG_ERR_REG_CRAM_SIZE_G4MH1X - 1),
                                    crgcap_craml, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            } else {
                crg_bus->AddTargetVC (crg_cap_craml_addr, crg_cap_craml_addr + (Crg::CRG_ERR_REG_CRAM_SIZE_G4MH2X - 1),
                                    crgcap_craml, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            }
            m_crgcap.push_back(crgcap_craml);
        }
        if (p_forest_util->IsEnableGuard (GMID_CRGCAP_SX2MBHCLm, cl_index)) {
            PhysAddr crg_cap_sx2mbh_addr = p_forest_util->GetGuardAddr (GMID_CRGCAP_SX2MBHCLm, cl_index);
            CrgCap *crgcap_sx2mbh = new CrgCap (crg_cap_sx2mbh_addr, core_type_1st_pe, core_version_1st_pe);
            p_internals->SetGuardCapErr(GMID_CRGCAP_SX2MBHCLm, cl_index, static_cast<TRegModuleVc *> (crgcap_sx2mbh->GetCrgErrReg()));
            if ((core_type_1st_pe != CORE_G4MH) || (core_version_1st_pe < CORE_VER_20)) {
                crg_bus->AddTargetVC (crg_cap_sx2mbh_addr, crg_cap_sx2mbh_addr + (Crg::CRG_ERR_REG_SX2MB_G4MH1X - 1),
                                    crgcap_sx2mbh, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            } else {
                crg_bus->AddTargetVC (crg_cap_sx2mbh_addr, crg_cap_sx2mbh_addr + (Crg::CRG_ERR_REG_SX2MB_G4MH2X - 1),
                                    crgcap_sx2mbh, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            }
            m_crgcap.push_back(crgcap_sx2mbh);
        }
        if (p_forest_util->IsEnableGuard (GMID_CRGCAP_SX2MBLCLm, cl_index)) {
            PhysAddr crg_cap_sx2mbl_addr = p_forest_util->GetGuardAddr (GMID_CRGCAP_SX2MBLCLm, cl_index);
            CrgCap *crgcap_sx2mbl = new CrgCap (crg_cap_sx2mbl_addr, core_type_1st_pe, core_version_1st_pe);
            p_internals->SetGuardCapErr(GMID_CRGCAP_SX2MBLCLm, cl_index, static_cast<TRegModuleVc *> (crgcap_sx2mbl->GetCrgErrReg()));
            if ((core_type_1st_pe != CORE_G4MH) || (core_version_1st_pe < CORE_VER_20)) {
                crg_bus->AddTargetVC (crg_cap_sx2mbl_addr, crg_cap_sx2mbl_addr + (Crg::CRG_ERR_REG_SX2MB_G4MH1X - 1),
                                    crgcap_sx2mbl, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            } else {
                crg_bus->AddTargetVC (crg_cap_sx2mbl_addr, crg_cap_sx2mbl_addr + (Crg::CRG_ERR_REG_SX2MB_G4MH2X - 1),
                                    crgcap_sx2mbl, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, cl_index));
            }
            m_crgcap.push_back(crgcap_sx2mbl);
        }
    }
    PeVec::const_iterator it_pe_crg = m_pe.begin ();
    while (it_pe_crg != m_pe.end ()) {
        if ((*it_pe_crg) != NULL) {
            uint32_t peid = (*it_pe_crg)->GetPeId ();
            if (p_forest_util->IsEnableGuard (GMID_CRGCAP_PEnCLm, peid)) {
                PhysAddr crg_cap_pe_addr = p_forest_util->GetGuardAddr (GMID_CRGCAP_PEnCLm, peid);
                CrgCap *crgcap_pe = new CrgCap (crg_cap_pe_addr, (*it_pe_crg)->GetCoreType(), (*it_pe_crg)->GetCoreVersion());
                p_internals->SetGuardCapErr(GMID_CRGCAP_PEnCLm, peid, static_cast<TRegModuleVc *> (crgcap_pe->GetCrgErrReg()));
                if (((*it_pe_crg)->GetCoreType() != CORE_G4MH) || ((*it_pe_crg)->GetCoreVersion() < CORE_VER_20)) {
                    crg_bus->AddTargetVC (crg_cap_pe_addr, crg_cap_pe_addr + (Crg::CRG_ERR_REG_PE_SIZE_G4MH1X - 1),
                                        crgcap_pe, NULL,
                                        m_dummy_guard,
                                        CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
                } else {
                    crg_bus->AddTargetVC (crg_cap_pe_addr, crg_cap_pe_addr + (Crg::CRG_ERR_REG_PE_SIZE_G4MH2X - 1),
                                        crgcap_pe, NULL,
                                        m_dummy_guard,
                                        CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
                }
                m_crgcap.push_back(crgcap_pe);
            }
        }
        ++ it_pe_crg;
    }

    PeVec::const_iterator it_pe2 = m_pe.begin ();
    while (it_pe2 != m_pe.end ()) {
        if ((*it_pe2) != NULL) {
            uint32_t peid = (*it_pe2)->GetPeId ();

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
            if (m_peri_addr[BaseAddr_INTC1].IsEnabled ()) {
                PhysAddr intc1_addr = m_peri_addr[BaseAddr_INTC1].GetAddr (peid);
                fsbus->AddTargetVC (intc1_addr, intc1_addr + (CIntc1::INTC1_REG_SIZE - 1),
                                    GetPePtr(peid)->GetIntc1Ptr(), NULL,
                                    p_forest_util->GetGuardPtr (peid, GUARD_PEG),
                                    CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
            }

            if (m_peri_addr[BaseAddr_IPIR].IsEnabled ()) {
                PhysAddr ipir_addr = m_peri_addr[BaseAddr_IPIR].GetAddr (peid);
                fsbus->AddTargetVC (ipir_addr, ipir_addr + (CIpir::IPIR_REG_SIZE - 1),
                                    m_ipir, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
            }

            if (m_peri_addr[BaseAddr_BAR].IsEnabled ()) {
                PhysAddr bar_addr = m_peri_addr[BaseAddr_BAR].GetAddr (peid);
                fsbus->AddTargetVC (bar_addr, bar_addr + (CBar::BAR_REG_SIZE - 1),
                                    m_bar, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
            }

            if (m_peri_addr[BaseAddr_MECNT].IsEnabled ()) {
                PhysAddr mecnt_addr = m_peri_addr[BaseAddr_MECNT].GetAddr (peid);
                fsbus->AddTargetVC (mecnt_addr, mecnt_addr + (Mecnt::MECNT_REG_SIZE - 1),
                                    m_mecnt, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
            }
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

            FsBus* peg_bus;
#if defined(SUPPORT_SC_HEAP)
            // In case of SC-HEAP, PEG registers is protected by APB in SC-HEAP.
            // Because access is goes thought SC-HEAP, connect PEG registers to m_peg_bus.
            // Route is CForest (LSU)->FSBUS->SC-HEAP->PegRegAccess->m_peg_bus->PEG registers
            peg_bus = m_peg_bus;
#else // defined(SUPPORT_SC_HEAP)
            peg_bus = fsbus;
#endif // defined(SUPPORT_SC_HEAP)
            // There are 3 modules in PEG: 1 control register and 2 error registers.
            if (p_forest_util->IsEnableGuard (GMID_PEG_PEnCLm, peid)) {
                Peg* peg = (*it_pe2)->GetPegPtr ();
                // Get address value from GMID_PEG_PEnCLm with suitable peid by user setting.
                PhysAddr peg_ctrl_addr = p_forest_util->GetGuardAddr (GMID_PEG_PEnCLm, peid);
                peg_bus->AddTargetVC (peg_ctrl_addr, peg_ctrl_addr + (Peg::PEG_CTRL_REG_SIZE - 1),
                                    peg, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));

            }
            if (p_forest_util->IsEnableGuard (GMID_PEGCAP_M_PEnCLm, peid)) {
                PhysAddr peg_err_addr_m = p_forest_util->GetGuardAddr (GMID_PEGCAP_M_PEnCLm, peid);
                PegCap* pegcap_m = new PegCap (peid, peg_err_addr_m, (*it_pe2)->GetCoreType(), (*it_pe2)->GetCoreVersion());
                p_internals->SetGuardCapErr(GMID_PEGCAP_M_PEnCLm, peid, static_cast<TRegModuleVc *> (pegcap_m->GetPegErrReg()));
                if (((*it_pe2)->GetCoreType() != CORE_G4MH) || ((*it_pe2)->GetCoreVersion() < CORE_VER_20)) {
                    peg_bus->AddTargetVC (peg_err_addr_m, peg_err_addr_m + (Peg::PEG_ERR_REG_M_SIZE_G4MH1X - 1),
                                        pegcap_m, NULL,
                                        m_dummy_guard,
                                        CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
                } else {
                    peg_bus->AddTargetVC (peg_err_addr_m, peg_err_addr_m + (Peg::PEG_ERR_REG_M_SIZE_G4MH2X - 1),
                                        pegcap_m, NULL,
                                        m_dummy_guard,
                                        CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
                }

                m_pegcap.push_back(pegcap_m);
            }
            if (p_forest_util->IsEnableGuard (GMID_PEGCAP_S_PEnCLm, peid)) {
                PhysAddr peg_err_addr_s = p_forest_util->GetGuardAddr (GMID_PEGCAP_S_PEnCLm, peid);
                PegCap* pegcap_s = new PegCap (peid, peg_err_addr_s, (*it_pe2)->GetCoreType(), (*it_pe2)->GetCoreVersion());
                p_internals->SetGuardCapErr(GMID_PEGCAP_S_PEnCLm, peid, static_cast<TRegModuleVc *> (pegcap_s->GetPegErrReg()));
                if (((*it_pe2)->GetCoreType() != CORE_G4MH) || ((*it_pe2)->GetCoreVersion() < CORE_VER_20)) {
                    peg_bus->AddTargetVC (peg_err_addr_s, peg_err_addr_s + (Peg::PEG_ERR_REG_M_SIZE_G4MH1X - 1),
                                        pegcap_s, NULL,
                                        m_dummy_guard,
                                        CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
                } else {
                    peg_bus->AddTargetVC (peg_err_addr_s, peg_err_addr_s + (Peg::PEG_ERR_REG_M_SIZE_G4MH2X - 1),
                                        pegcap_s, NULL,
                                        m_dummy_guard,
                                        CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
                }
                m_pegcap.push_back(pegcap_s);
            }

            if (m_peri_addr[BaseAddr_MISR].IsEnabled ()) {
                PhysAddr misr_addr = m_peri_addr[BaseAddr_MISR].GetAddr (peid);
                fsbus->AddTargetVC (misr_addr, misr_addr + (Misr::MISR_REG_SIZE - 1),
                                    GetPePtr(peid)->GetMisrPtr(), NULL,
                                    m_dummy_guard, CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
            }

#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
#ifdef ENABLE_PERFCNT
            if (m_peri_addr[BaseAddr_PERF].IsEnabled ()) {
                PhysAddr perfcnt_addr = m_peri_addr[BaseAddr_PERF].GetAddr (peid);
                fsbus->AddTargetVC (perfcnt_addr, perfcnt_addr + (Cperfcnt::PERFCNT_AREA_SIZE - 1),
                                    m_perfcnt[peid], NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (peid, p_internals->GetClIdFromPeId (peid)));
            }
#endif // ENABLE_PERFCNT
#endif // SUPPORT_CUBESUITE

#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
#ifdef USE_SIM_IO
            if (m_peri_addr[BaseAddr_SIMIO].IsEnabled ()) {
                PhysAddr simio_addr = m_peri_addr[BaseAddr_SIMIO].GetAddr (peid);
                fsbus->AddTargetVC (simio_addr, simio_addr | Csimio::SIM_IO_MASK,
                                    simio, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
            }
            if (m_peri_addr[BaseAddr_SIMIOG3K].IsEnabled ()) {
                PhysAddr simio_addr = m_peri_addr[BaseAddr_SIMIOG3K].GetAddr (peid);
                fsbus->AddTargetVC (simio_addr, simio_addr | Csimio::G3K_SIM_IO_MASK,
                                    simio, NULL,
                                    m_dummy_guard,
                                    CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
            }
#endif // USE_SIM_IO
#endif // SUPPORT_CUBESUITE

        }
        ++ it_pe2;
    }

#ifdef SUPPORT_CUBESUITE
#else // SUPPORT_CUBESUITE
#ifdef USE_SIM_IO
    fsbus->AddTargetVC (Csimio::ECM_IO_BASE, Csimio::ECM_IO_BASE | Csimio::ECM_IO_MASK, simio, NULL, m_dummy_guard,
                        CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
    fsbus->AddTargetVC (Csimio::LOG_IO_BASE, Csimio::LOG_IO_BASE | Csimio::LOG_IO_MASK, simio, NULL, m_dummy_guard,
                        CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
#endif // USE_SIM_IO
#endif // SUPPORT_CUBESUITE

#ifdef ENABLE_UART
    if (uart != NULL) {
        if (m_peri_addr[BaseAddr_UART].IsEnabled ()) {
            PhysAddr uart_addr = m_peri_addr[BaseAddr_UART].GetAddr (0);
            fsbus->AddTargetVC (uart_addr, uart_addr + (Uart::UART_AREA_SIZE - 1),
                                uart, NULL, m_dummy_guard,
                                CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
        }
    }
#endif // ENABLE_UART

    // 全領域用メモリ空間は最後に宣言する
#ifdef SUPPORT_SC_HEAP
#else // SUPPORT_SC_HEAP
    fsbus->AddTargetVC (0x00U, 0xFFFFFFFFU, smem0, smem0->mem_body, m_dummy_guard,
                        CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
    fsbus->RegisterSmem ();
#endif // SUPPORT_SC_HEAP
}

bool Cforest::SetIntc2 (PhysAddr start_addr)
{
    if (intc2 != NULL) {
        CfMsg::DPrint (MSG_ERR, "<Error: Multiple Definition.>\n");
        return false;
    }

    CoreVersion core_version = CORE_VER_10;
    PeVec::const_iterator it_pe = m_pe.begin ();
    while (it_pe != m_pe.end ()) {
        if ((*it_pe) != NULL) {
            core_version = (*it_pe)->GetCoreVersion ();
            break;
        }
       ++ it_pe;
    }

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    intc2 = NULL;
#else // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    Intc2ImplType intc2_impl_type = ((core_version >= CORE_VER_20) ? INTC2_G4MH20:INTC2_STANDARD);
    intc2 = new CIntc2 (&m_pe, fsbus, intc2_impl_type);
    fsbus->AddTargetVC (start_addr, start_addr + (CIntc2::INTC2_REG_SIZE - 1), intc2, NULL,  m_dummy_guard,
            CreateDefaultLatency (MEM_LOCATE_GLOBAL, MEM_LOCATE_GLOBAL));
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

    return true;
}


void Cforest::SetPe (uint32_t peid, uint32_t clid, CoreType core_type, CoreVersion core_version, uint32_t vc_num, uint32_t tc_num)
{
    // PEID分のvectorを確保する
    if (m_pe.size () <= peid) {
        m_pe.resize (peid + 1, NULL);
    }

    // VM数とHT数に従い、アーキテクチャクラスを決定
    // 以下の組合せは禁止。事前の入力チェックでエラー判定し、ここでは縮退させる。
    // VM数 HT数
    // 0    0    E3v5-S  -> E3v5-S
    // 0    *    禁止    -> E3v5-S
    // 1    0    禁止    -> E3v5-V
    // 1    1    E3v5-V  -> E3v5-V
    // 上記以外  E3v5-F  -> E3v5-F
    ArchClass arch_class;
    if (vc_num == 0) {
        arch_class = E3V5_S;
    } else if (vc_num == 1 && tc_num <= 1) {
        arch_class = E3V5_V;
    } else {
        arch_class = E3V5_F;
    }

    // PEを生成
    // PEID that would be registered is ID that contains clusterID in upper field

    ProcElement* pe =
           CedarFactory::CreatePE (peid, clid, core_type, core_version, arch_class, fsbus, llsc);
    m_pe [peid] = pe;

    FsCtrl* nc = CedarFactory::GetHtPtr (pe);
    m_ht.push_back (nc);

    // SCIDを設定
    // 各HTにシステムIDを付与
    // @@ cpu_timeの計算に連番のIDが必要
    int32_t scid = 0;
    HtVec::iterator it = m_ht.begin ();
    while (it != m_ht.end ()) {
        CedarFactory::SetScId (m_ht[scid], scid);
        ++ scid;
        ++ it;
    }
}


ProcElement* Cforest::GetPePtr (uint32_t peid) const
{
    return m_pe[peid];
}


MemLatencyInfoBase* Cforest::CreateDefaultLatency (MemLocationType pe_location, MemLocationType cl_location)
{
    return new MemLatencyInfo<DBUF_SIMPLE_MEM> (10, 10, 10, 10, 10, 10, 10, 10, 10,
                                                pe_location, cl_location, 1, 2, MEM_RES_OTHERS, "peripheral");
}


// @@ 現状、全HTを１つの配列で管理している。ダサい。
// @@ しっかりと書き直す
void Cforest::CreateCforest (void)
{
    m_is_gen_peripherals = false;

    PeripheralAddr init_val (0, 0, false);
    m_peri_addr.assign (BaseAddr_NUM, init_val);

#ifdef SUPPORT_CUBESUITE
    // In case of CS+, all peripherals are disabled and parameters are 0 at default.
    // They will be enabled by CS+ API.
#else // SUPPORT_CUBESUITE
    m_peri_addr[BaseAddr_INTC1].m_base_addr = 0xFFFC0000U;
    m_peri_addr[BaseAddr_INTC1].m_offset_addr = LOCAL_PERI_OFFSET;
    m_peri_addr[BaseAddr_INTC1].m_is_enabled = true;

    m_peri_addr[BaseAddr_INTC2].m_base_addr = 0xFFF80000U;
    m_peri_addr[BaseAddr_INTC2].m_is_enabled = true;

    m_peri_addr[BaseAddr_IPIR].m_base_addr  = 0xFFFB9000U;
    m_peri_addr[BaseAddr_IPIR].m_is_enabled = true;

    m_peri_addr[BaseAddr_MECNT].m_base_addr = 0xFFFBA000U;
    m_peri_addr[BaseAddr_MECNT].m_is_enabled = true;

    m_peri_addr[BaseAddr_UART].m_base_addr = 0x00F6000000U; // Reserved area in E2xPFSS
    m_peri_addr[BaseAddr_UART].m_is_enabled = false;

    m_peri_addr[BaseAddr_MISR].m_base_addr = 0x00FFC50000U; // Disabled
    m_peri_addr[BaseAddr_MISR].m_offset_addr = 0x1000;
    m_peri_addr[BaseAddr_MISR].m_is_enabled = false;

    m_peri_addr[BaseAddr_TMU].m_base_addr = 0xFBF00000U; // Emulation RAM area in E2xPFSS
    m_peri_addr[BaseAddr_TMU].m_is_enabled = false;

    m_peri_addr[BaseAddr_BAR].m_base_addr  = 0xFFFB8000U;
    m_peri_addr[BaseAddr_BAR].m_is_enabled = true;

#ifdef ENABLE_PERFCNT
    m_peri_addr[BaseAddr_PERF].m_base_addr = PERFCNT_BASE;
    m_peri_addr[BaseAddr_PERF].m_offset_addr = SIM_PERI_OFFSET;
    m_peri_addr[BaseAddr_PERF].m_is_enabled = true;
#endif //#ifdef ENABLE_PERFCNT
#ifdef USE_SIM_IO
    m_peri_addr[BaseAddr_SIMIO].m_base_addr = Csimio::SIM_IO_BASE;
    m_peri_addr[BaseAddr_SIMIO].m_offset_addr = SIM_PERI_OFFSET;
    m_peri_addr[BaseAddr_SIMIO].m_is_enabled = true;
    m_peri_addr[BaseAddr_SIMIOG3K].m_base_addr = Csimio::G3K_SIM_IO_BASE; // Disabled
    m_peri_addr[BaseAddr_SIMIOG3K].m_is_enabled = false;
#endif // USE_SIM_IO
#endif // SUPPORT_CUBESUITE

    m_dummy_guard = new ModuleGuardDummy ();
    m_dbg_area = new DbgArea ();

    fsbus = new FsBus ();
    llsc = new Llsc ();

#if defined(SUPPORT_SC_HEAP)
    #if !defined(TEST_SC_HEAP)
    m_peg_bus = new FsBus ();
    m_crg_bus = new FsBus ();
    #else // !defined(TEST_SC_HEAP)
    m_peg_bus = fsbus;
    m_crg_bus = fsbus;
    #endif // !defined(TEST_SC_HEAP)
#endif // defined(SUPPORT_SC_HEAP)

#ifdef SUPPORT_SC_HEAP
    m_invalid_lram = NULL;
#endif // #ifdef SUPPORT_SC_HEAP

    m_error_area = new ErrorArea ();
}

Cforest::Cforest ()
{
    m_is_gen_peripherals = false;
    m_dummy_guard = NULL;
    m_dbg_area = NULL;
    fsbus = NULL;
    llsc = NULL;
    intc2 = NULL;
#ifdef USE_SIM_IO
    simio = NULL;
#endif // USE_SIM_IO
#ifdef ENABLE_UART
    uart = NULL;
#endif // ENABLE_UART
#ifdef ENABLE_TMU
    tmu0 = NULL;
    tmu1 = NULL;
    tmu2 = NULL;
    tmu3 = NULL;
#endif // ENABLE_TMU

    m_ipir = NULL;
    m_bar = NULL;
    m_mecnt = NULL;

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    m_peg_bus = NULL;
    m_crg_bus = NULL;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

#ifdef SUPPORT_SC_HEAP
    m_invalid_lram = NULL;
#endif // #ifdef SUPPORT_SC_HEAP

    m_error_area = NULL;
}


Cforest::~Cforest()
{
    PeVec::const_iterator it = m_pe.begin ();
    while ( it != m_pe.end () ) {
        CedarFactory::DeletePE (*it);
        ++ it;
    }
    m_pe.clear ();
    m_ht.clear ();

    std::vector<PegCap*>::const_iterator it_pegcap = m_pegcap.begin ();
    while ( it_pegcap != m_pegcap.end () ) {
        delete (*it_pegcap);
        ++ it_pegcap;
    }
    m_pegcap.clear ();

    std::vector<CrgCap*>::const_iterator it_crgcap = m_crgcap.begin ();
    while ( it_crgcap != m_crgcap.end () ) {
        delete (*it_crgcap);
        ++ it_crgcap;
    }
    m_crgcap.clear ();

    std::vector<Crg*>::const_iterator it_crg = m_crg.begin ();
    while ( it_crg != m_crg.end () ) {
        delete (*it_crg);
        ++ it_crg;
    }
    m_crg.clear ();

    delete m_dummy_guard;
    delete m_dbg_area;

    delete fsbus;
    delete llsc;

    if (m_is_gen_peripherals == false) {
        return;
    }

    std::vector<Csmem*>::const_iterator it_smem = m_smem.begin ();
    while ( it_smem != m_smem.end () ) {
        delete (*it_smem);
        ++ it_smem;
    }
    m_smem.clear ();

    delete intc2;

#ifdef USE_SIM_IO
    delete simio;
#endif // USE_SIM_IO
#ifdef ENABLE_PERFCNT
    std::vector<Cperfcnt*>::const_iterator it_perfcnt = m_perfcnt.begin ();
    while ( it_perfcnt != m_perfcnt.end () ) {
        delete (*it_perfcnt);
        ++ it_perfcnt;
    }
    m_perfcnt.clear ();
#endif // ENABLE_PERFCNT

#ifdef ENABLE_UART
    if (uart != NULL) {
        delete uart;
    }
#endif // ENABLE_UART
#ifdef SUPPORT_CUBESUITE
    std::vector<CubeIoReg*>::const_iterator it_ior =  m_cube_iors.begin ();
    while ( it_ior != m_cube_iors.end () ) {
        delete (*it_ior);
        ++ it_ior;
    }
    m_cube_iors.clear ();

    std::vector<CubeLiorInfo>::const_iterator it_lior =  m_cube_lior.begin ();
    while ( it_lior != m_cube_lior.end () ) {
        delete it_lior->second;
        ++ it_lior;
    }
    m_cube_lior.clear ();
#endif // SUPPORT_CUBESUITE

#ifdef ENABLE_TMU
    delete tmu0;
    delete tmu1;
    delete tmu2;
    delete tmu3;
#endif // ENABLE_TMU

    delete m_ipir;

    delete m_bar;
    delete m_mecnt;

#ifdef ENABLE_OSTM
    std::vector<OstmIO*>::iterator it_ostm = m_ostm.begin ();
    while (it_ostm != m_ostm.end ()) {
        delete (*it_ostm);
        ++ it_ostm;
    }
    m_ostm.clear ();
#endif // #ifdef ENABLE_OSTM

#if defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)
    delete m_peg_bus;
    delete m_crg_bus;
#endif // defined(SUPPORT_SC_HEAP) && !defined(TEST_SC_HEAP)

    m_crg_param_vec.clear ();

#ifdef SUPPORT_SC_HEAP
    delete m_invalid_lram;
#endif // #ifdef SUPPORT_SC_HEAP

    delete m_error_area;
}
