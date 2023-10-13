/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */
#include <string.h>
#include <vector>
#include "./config.h"
#include "./fs_controler.h"
#include "./intc1.h"
#include "./exp_info.h"
#include "./fsbus.h"
#include "./proc_element.h"
#include "./forest.h"
//class FsCtrl;
#if _INTC1_UNIT_TEST
# include "mock_proc_element.h"
# include "mock_fsbus.h"
# include "mock_fsctrl.h"
#endif

/*!
 * @brief Instantiates INTC1 register.
 * @param -
 */
void TRegIntc1::InitTRegMap (void)
{
    Intc2ImplType intc2_impl_type = INTC2_STANDARD;
    //アドレスをキーとしてマップに登録
    PhysAddr eic_addr = EIC_OFFSET;
    for (uint32_t i = 0; i < N_EIC; i++) {
        TRegEIC* eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_addr+i*2, i, m_peid, m_intc1);
        m_trf.insert (TRegMap::value_type (eic->GetTRegAddr(), eic));
    }

    PhysAddr eeic_addr = EEIC_OFFSET;
    for (uint32_t i = 0; i < N_EEIC; i++) {
        TRegEEIC* eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_addr+i*4, i, m_peid, m_intc1);
        m_trf.insert (TRegMap::value_type (eeic->GetTRegAddr(), eeic));
    }

    PhysAddr imr_addr = IMR_OFFSET;
    for (uint32_t i = 0; i < N_IMR; i++) {
        TRegIMR* imr = new TRegIMR (SIZE_32BIT, SIZE_8BIT, imr_addr+i*4, i, m_intc1);
        m_trf.insert (TRegMap::value_type (imr->GetTRegAddr(), imr));
    }

    PhysAddr eibd_addr = EIBD_OFFSET;
    for (uint32_t i = 0; i < N_EIBD; i++) {
        TRegEIBD* eibd = new TRegEIBD (SIZE_32BIT, SIZE_8BIT, eibd_addr+i*4,
                                       TRegMask(TRegBND<EIBD>::INTC1_EIBD_MASK_G4MH20),
                                       i, m_peid, m_intc1, intc2_impl_type);
        m_trf.insert (TRegMap::value_type (eibd->GetTRegAddr(), eibd));
    }

    PhysAddr fnc_addr = FNC_OFFSET;
    TRegFNC* fnc = new TRegFNC (SIZE_16BIT, SIZE_8BIT, fnc_addr, m_intc1);
    m_trf.insert (TRegMap::value_type (fnc_addr, fnc));

    PhysAddr fic_addr = FIC_OFFSET;
    for (uint32_t i = 0; i < N_FIC; i++) {
        TRegFIC* fic = new TRegFIC (SIZE_16BIT, SIZE_8BIT, fic_addr+i*2, i, m_intc1);
        m_trf.insert (TRegMap::value_type (fic->GetTRegAddr(), fic));
    }

    PhysAddr fnbd_addr = FNBD_OFFSET;
    TRegFNBD* fnbd = new TRegFNBD (SIZE_32BIT, SIZE_8BIT, fnbd_addr, m_peid, m_intc1, intc2_impl_type);
    m_trf.insert (TRegMap::value_type (fnbd_addr, fnbd));

    PhysAddr fibd_addr = FIBD_OFFSET;
    for (uint32_t i = 0; i < N_FIBD; i++) {
        TRegFIBD* fibd = new TRegFIBD (SIZE_32BIT, SIZE_8BIT, fibd_addr+i*4,
                                               TRegMask(TRegBND<FIBD>::FBD_MASK),
                                               i, m_peid, m_intc1, intc2_impl_type);
        m_trf.insert (TRegMap::value_type (fibd->GetTRegAddr(), fibd));
    }

    /*
     * Add due to update spec INTC1 G4MH
     */
    PhysAddr dbmk_addr = DBMK_OFFSET;
    TRegDBMK* dbmk = new TRegDBMK (SIZE_16BIT, SIZE_8BIT, dbmk_addr, m_intc1);
    m_trf.insert (TRegMap::value_type (dbmk_addr, dbmk));

    /*
     * INTC1 G4MH2.0 spec
     */

    PhysAddr ihvcfg_addr = IHVCFG_OFFSET;
    TRegIHVCFG* ihvcfg = new TRegIHVCFG (SIZE_32BIT, SIZE_8BIT, ihvcfg_addr, m_intc1);
    m_trf.insert (TRegMap::value_type (ihvcfg_addr, ihvcfg));

    PhysAddr eibg_addr = EIBG_OFFSET;
    TRegMask mask = TRegMask (TRegEIBG::EIBG_MASK_INTC1);
    TRegEIBG* eibg = new TRegEIBG (SIZE_32BIT, SIZE_8BIT, eibg_addr,mask, N_G4MH20_EIBG, m_intc1);
    m_trf.insert (TRegMap::value_type (eibg->GetTRegAddr(), eibg));

    PhysAddr fibg_addr = FIBG_OFFSET;
    TRegFIBG* fibg = new TRegFIBG (SIZE_32BIT, SIZE_8BIT, fibg_addr, m_intc1);
    m_trf.insert (TRegMap::value_type (fibg_addr, fibg));
}


/*!
 * @brief Gets the address of EIC register.
 * @param channel Channel number.
 * @return Address of EIC register.
 */
PhysAddr TRegIntc1::TrEICAddr (uint32_t channel) const
{
    return EIC_OFFSET + channel * 2;
}

/*!
 * @brief Gets the address of EEIC register.
 * @param channel Channel number.
 * @return Address of EIC register.
 */
PhysAddr TRegIntc1::TrEEICAddr (uint32_t channel) const
{
    return EEIC_OFFSET + channel * 4;
}


/*!
 * @brief Gets the address of EIBD register.
 * @param channel Channel number.
 * @return Address of EIBD register.
 */
PhysAddr TRegIntc1::TrEIBDAddr (uint32_t channel) const
{
    return EIBD_OFFSET + channel * 4;
}


/*!
 * @brief Gets the address of FNC register.
 * @param -
 * @return Address of FNC register.
 */
PhysAddr TRegIntc1::TrFNCAddr (void) const
{
    return FNC_OFFSET;
}


/*!
 * @brief Gets the address of FIC register.
 * @param channel Channel number.
 * @return Address of FIC register.
 */
PhysAddr TRegIntc1::TrFICAddr (uint32_t channel) const
{
    return FIC_OFFSET + channel * 2;
}


/*!
 * @brief Gets the address of FNBD register.
 * @param -
 * @return Address of FNBD register.
 */
PhysAddr TRegIntc1::TrFNBDAddr (void) const
{
    return FNBD_OFFSET;
}


/*!
 * @brief Gets the address of FIBD register.
 * @param channel Channel number.
 * @return Address of FIBD register.
 */
PhysAddr TRegIntc1::TrFIBDAddr (uint32_t channel) const
{
    return FIBD_OFFSET + channel * 4;
}


/*!
 * @brief Gets the address of DBMK register.
 * @param -
 * @return DBMK_OFFSET
 */

PhysAddr TRegIntc1::TrDBMKAddr (void) const
{
    return DBMK_OFFSET;
}

PhysAddr TRegIntc1::TrIHVCFGAddr (void) const
{
    return IHVCFG_OFFSET;
}


PhysAddr TRegIntc1::TrEIBGAddr (uint32_t peid) const
{
    return EIBG_OFFSET;
}

PhysAddr TRegIntc1::TrFIBGAddr (void) const
{
    return FIBG_OFFSET;
}


/*!
 * @brief Writes to the register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @retval true Normal end.
 * @retval false Abnormal end.
 */
SysErrCause CIntc1::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause syserr;
    syserr = m_trfs->GetTrPtr(addr & INTC1_REG_ADDR_MASK)->Write (sideband, addr, data, size);
    return syserr;
}


/*!
 * @brief Reads from the register.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @retval true Normal end.
 * @retval false Abnormal end.
 */
SysErrCause CIntc1::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    SysErrCause syserr;
    syserr = m_trfs->GetTrPtr(addr & INTC1_REG_ADDR_MASK)->Read (sideband, addr, data, size);
    return syserr;
}


/*!
 * @brief Writes the register with debugging.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause CIntc1::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr & INTC1_REG_ADDR_MASK)->WriteBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Reads the register with debugging.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause CIntc1::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr & INTC1_REG_ADDR_MASK)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}

/*!
 * @brief Resets INTC1.
 * @param -
 */
void CIntc1::HardReset (void)
{
#if _INTC1_UNIT_TEST
        m_cpu_ihve = 1;
        m_cpu_has_hv = 1;
#else
        m_cpu_ihve = (GetHVEExtPin() & 0x00000001U );
        m_ihvcfg_ihve = m_cpu_ihve;
        m_cpu_has_hv = GetHasHVExtPin();
#endif
    m_trfs->ResetTRegs();
    InitPriorityInfo();
    ClearIntReqQueue();
    StopCyclicHandler (this);
}


/*!
 * @brief Main function.
 * @param -
 */
void CIntc1::CyclicHandler (void)
{
    if (m_core_version >= CORE_VER_20){
        UpdateHighPriorityFromCpuMode(NC);
    }
    //最高優先度割込み情報を参照し、各HTにバインドされた
    //最高優先度割込みをHTに対し通知
    IntHighPriorityMap::const_iterator it = m_intc1_priority.begin();
    while (it != m_intc1_priority.end()) {
        uint32_t channel = 0;
        uint32_t channel_fenmi = 0;
        uint32_t channel_feint = 0;
        uint32_t channel_eiint = 0;

        if (m_core_version >= CORE_VER_20){
            channel_fenmi = it->second->GetChannelFenmi();
            channel_feint = it->second->GetChannelFeint();
            channel_eiint = it->second->GetChannelEiint();
        }else{
            channel = it->second->GetChannel();
        }
        if (m_core_version >= CORE_VER_20){

            //send FENMI to cpu if any
            IntReqMap::const_iterator req_it_fenmi = m_intc1_queue.find (channel_fenmi);
            if (req_it_fenmi != m_intc1_queue.end()) {
                    void (*resfunc_fenmi)(CIntc1* intc1);
                    if (req_it_fenmi->second->IsLevelDetect ()) {
                        resfunc_fenmi = &(CIntc1::ResFenmiByDummy);
                    } else {
                        resfunc_fenmi = &(CIntc1::ResFenmiFromCpu);
                    }
                    ReqFenmiToCpu (resfunc_fenmi);
            }

            //process xxFEINT
            IntReqMap::const_iterator req_it_feint = m_intc1_queue.find (channel_feint);
            bool bindmode_feint = 0;
            uint32_t gpid_feint = 0;
            uint32_t bge_feint = 0;
            void (*resfunc_feint)(uint32_t channel_feint, CIntc1* intc1);
            if (req_it_feint != m_intc1_queue.end()) {
                bindmode_feint = req_it_feint->second->GetGM();
                gpid_feint = req_it_feint->second->GetGPID();
                bge_feint = req_it_feint->second->GetBGE();

                         if (req_it_feint->second->IsLevelDetect ()) {
                             resfunc_feint = &(CIntc1::ResFeintByDummy);
                        } else {
                            resfunc_feint = &(CIntc1::ResFeintFromCpu);
                    }
                    CancelFeintToCpu (true); // Delete exist FEINT request (change Yoshinaga)
                    ReqFeintToCpu (channel_feint - FEINT_CHANNEL,bindmode_feint, gpid_feint, bge_feint,resfunc_feint);

            }

            //process xxEIINT
            IntReqMap::const_iterator req_it_eiint = m_intc1_queue.find (channel_eiint);
            bool bindmode_eiint = 0;
            uint32_t gpid_eiint = 0;
            uint32_t bgpr_eiint = 0;
            uint32_t priority_eiint = 0;
            bool eitb;
            void (*resfunc_eiint)(uint32_t channel_eiint, CIntc1* intc1);

            if (req_it_eiint != m_intc1_queue.end()) {
                bindmode_eiint = req_it_eiint->second->GetGM();
                gpid_eiint = req_it_eiint->second->GetGPID();
                bgpr_eiint = req_it_eiint->second->GetBGPR();

                    priority_eiint = static_cast<uint32_t> (it->second->GetPriorityEiint());
                    eitb = req_it_eiint->second->GetEITB();

                    if (req_it_eiint->second->IsLevelDetect ()) {
                          //レベル検出の場合、コールバックにダミー関数を登録
                           resfunc_eiint = &(CIntc1::ResIntByDummy);
                    } else {
                        resfunc_eiint = &(CIntc1::ResIntFromCpu);
                    }
            #if defined(COMP_RUNNER_G3KH) || defined(COMP_RUNNER_G3MH)
                    // Set PMEI to 1 if EIINT is masked by PMR.
                    if ((m_pe->GetHtPtr()->IsPlmrMasked(priority) == true) && (req_it->second->GetEIRF())){    //Not update for EEIC
                        m_pe->GetHtPtr()->m_srf->SrICSR()->SetPMEI(1);
                    }
            #else // defined(COMP_RUNNER)
                    CancelIntToCpu(channel_eiint, true); // "true" is necessary for SC-HEAP // changed by Yoshinaga
                    ReqEiintToCpu (priority_eiint, channel_eiint, eitb,bindmode_eiint, gpid_eiint, bgpr_eiint,resfunc_eiint);

            #endif // defined(COMP_RUNNER)
            }
        } else { // CORE_VERSION < 2.0
        IntReqMap::const_iterator req_it = m_intc1_queue.find (channel);
        if (req_it != m_intc1_queue.end()) {
            if (channel == FENMI_CHANNEL) {
                void (*resfunc)(CIntc1* intc1);
                if (req_it->second->IsLevelDetect ()) {
                    resfunc = &(CIntc1::ResFenmiByDummy);
                } else {
                    resfunc = &(CIntc1::ResFenmiFromCpu);
                }
                ReqFenmiToCpu (resfunc);
            } else if ((channel >= FEINT_CHANNEL) && (channel < FEINT_CHANNEL + NUM_FEINT_CHANNEL)) {
                bool bindmode = 0;
                uint32_t gpid = 0;
                uint32_t bge = 0;
                void (*resfunc)(uint32_t channel, CIntc1* intc1);
                if (req_it->second->IsLevelDetect ()) {
                    resfunc = &(CIntc1::ResFeintByDummy);
                } else {
                    resfunc = &(CIntc1::ResFeintFromCpu);
                }
                CancelFeintToCpu (true); // Delete exist FEINT request (change Yoshinaga)
                ReqFeintToCpu (channel - FEINT_CHANNEL,bindmode, gpid, bge,resfunc);
            } else {
                bool bindmode = 0;
                uint32_t gpid = 0;
                uint32_t bgpr = 0;
                uint32_t priority = static_cast<uint32_t> (it->second->GetPriority ());
                bool eitb = req_it->second->GetEITB();
                void (*resfunc)(uint32_t channel, CIntc1* intc1);
                if (req_it->second->IsLevelDetect ()) {
                    //レベル検出の場合、コールバックにダミー関数を登録
                    resfunc = &(CIntc1::ResIntByDummy);
                } else {
                    resfunc = &(CIntc1::ResIntFromCpu);
                }
#if defined(COMP_RUNNER_G3KH) || defined(COMP_RUNNER_G3MH)
                // Set PMEI to 1 if EIINT is masked by PMR.
                if ((m_pe->GetHtPtr()->IsPlmrMasked(priority) == true) && (req_it->second->GetEIRF())){    //Not update for EEIC
                    m_pe->GetHtPtr()->m_srf->SrICSR()->SetPMEI(1);
                }
#else // defined(COMP_RUNNER)
                ReqEiintToCpu (priority, channel, eitb,bindmode, gpid, bgpr,resfunc);
#endif // defined(COMP_RUNNER)
            }
        }
    }//end core_version
        ++ it;
    }
}



/*!
 * @brief Constructs INTC1 class.
 * @param pe The pointer of type MockProcElement.
 * @param peid PE number.
 * @param fsbus The pointer to FsBus.
 * @param core_type CPU core type. G4MH(G4MH1.0), G4MH11(G4MH1.1), etc.
 * @param core_version CPU core version. CORE_VER_10(Ver 1.0), CORE_VER_11(Ver. 1.1), etc.
 */
#if _INTC1_UNIT_TEST
CIntc1::CIntc1 (MockProcElement* pe, uint32_t peid, MockFsBus* fsbus, CoreType core_type, CoreVersion core_version)
    : m_pe (pe), m_peid (peid), m_fsbus (fsbus), m_intc2(NULL), m_core_type (core_type), m_core_version (core_version)

{
  //INTC1レジスタセットクラスをインスタンス
    m_trfs = new TRegIntc1 (peid, core_type, this);
    m_req_feint_channel = INIT_FEINT_CHANNEL; // Invalid channel
    m_is_cpu_gm = 0;  //CPU operation mode
    m_cpu_gpid = 0;  //CPU GPID
    //m_eiint_cpu_ack = INT_E_NOT_RECEIVED;
    //m_feint_cpu_ack = INT_E_NOT_RECEIVED;
    m_pending_state = false;
    m_external_EIINT_req = false;
    m_external_FEINT_req = false;
    if (m_core_version >= CORE_VER_20){
        m_cpu_ihve = 1;
        cpu_has_hv = 1;
        cpu_hve = 1;
    }else{
        m_cpu_ihve = 0;
        cpu_has_hv = 0;
        cpu_hve = 0;
    }
    m_ihvcfg_ihve = m_cpu_ihve;
}
#else
CIntc1::CIntc1 (ProcElement* pe, uint32_t peid, FsBus* fsbus, CoreType core_type, CoreVersion core_version)
    : m_pe (pe), m_peid (peid), m_fsbus (fsbus), m_intc2(NULL), m_core_type (core_type), m_core_version (core_version)
{
    //INTC1レジスタセットクラスをインスタンス
    m_trfs = new TRegIntc1 (peid, core_type, this);
    m_req_feint_channel = INIT_FEINT_CHANNEL; // Invalid channel
    m_is_cpu_gm = 0;  //CPU operation mode
    m_cpu_gpid = 0;  //CPU GPID
    //m_eiint_cpu_ack = INT_E_NOT_RECEIVED;
    //m_feint_cpu_ack = INT_E_NOT_RECEIVED;
    m_pending_state = false;
    m_external_EIINT_req = false;
    m_external_FEINT_req = false;
    if (m_core_version >= CORE_VER_20){
        m_cpu_ihve = 1;
        m_cpu_has_hv = 1;
        cpu_has_hv = 1;
        cpu_hve = 1;
    }else{
        m_cpu_ihve = 0;
        m_cpu_has_hv = 0;
        cpu_has_hv = 0;
        cpu_hve = 0;
    }
    m_ihvcfg_ihve = m_cpu_ihve;
}
#endif

/*!
 * @brief Destructs INTC1 class.
 * @param -
 */
CIntc1::~CIntc1 ()
{
    delete m_trfs;
    IntReqMap::iterator ite0 = m_intc1_queue.begin();
    while (ite0 != m_intc1_queue.end()) {
        delete (ite0->second);
        ++ite0;
    }
    IntHighPriorityMap::iterator ite1 = m_intc1_priority.begin();
    while (ite1 != m_intc1_priority.end()) {
        delete (ite1->second);
        ++ite1;
    }
    m_intc1_queue.clear();
    m_intc1_priority.clear();
}


/*!
 * @brief Sends the interrupt request from Peripheral module.
 * @param channel Channel number.(0...31)
 * @return IntErrorCode
 */
IntErrorCode CIntc1::ReqIntByPeripheral (uint32_t channel)
{
    if (IsIntc1Channel (channel) == true) {
        CfMsg::IntcPrint ("<INTC1@PE%d: EIINT%u is noticed to INTC1>\n", m_peid, channel);
        TRegEIC* eic = GetEICPtr (channel);
        TRegEEIC* eeic = GetEEICPtr (channel);
        if (eic->GetEIRF() == 0) {
            eeic->SetEIRF(1);
            eic->SetEIRF(1);
            return INT_E_OK;
       } else {
            //CPUからの応答がないため、別の割込みとして受け取れない
            if (m_core_version >= CORE_VER_20){
                SetExternalEIINTreq(true);
                eeic->SetEIRF(1);
                eic->SetEIRF(1);
                return INT_E_NOT_RECEIVED;
            }else{
                return INT_E_NOT_RECEIVED;
            }
       }
    }
    return INT_E_CHANNEL;
}


/*!
 * @brief Sends the request of FENMI.
 * @param -
 * @return IntErrorCode
 */
IntErrorCode CIntc1::ReqIntByFenmi (void)
{
    CfMsg::IntcPrint ("<INTC1@PE%d: FENMI is noticed to INTC1>\n", m_peid);
    TRegFNC* fnc = GetFNCPtr ();
    if (fnc->GetFNRF() == 0) {
        fnc->SetFNRF(1);
        return INT_E_OK;
    } else {
        return INT_E_NOT_RECEIVED;
    }
}


/*!
 * @brief Sends the request of FEINT.
 * @param channel Channel Number.
 * @return IntErrorCode
 */
IntErrorCode CIntc1::ReqIntByFeint (uint32_t channel)
{
    CfMsg::IntcPrint ("<INTC1@PE%d: FEINT%u is noticed to INTC1>\n", m_peid, channel);
    TRegFIC* fic = GetFICPtr (channel);
    if (fic->GetFIRF() == 0) {
        fic->SetFIRF(1);
        return INT_E_OK;
    } else {
        if (m_core_version >= CORE_VER_20){
            SetExternalFEINTreq(true);
            fic->SetFIRF(1);
            return INT_E_NOT_RECEIVED;
        }else{
            return INT_E_NOT_RECEIVED;
        }
    }
}


/*!
 * @brief Sends the request from INTC2.
 * @param channel Channel Number.
 * @param intreq_info The information about EIINT request.
 */
void CIntc1::ReqIntByIntc2 (uint32_t channel, IntReqInfoBase* intreq_info)
{
    // Overwrite DBMK pointer by INTC1 instance
    EiintReqInfo* ptr = static_cast<EiintReqInfo*> (intreq_info);
    ptr->SetDBMK (GetDBMKPtr ());

    ReqInt (channel, intreq_info);
}


/*!
 * @brief Registers the request for the interrupt.
 * @param channel Channel Number.
 * @param intreq_info The information about EIINT request.
 */
void CIntc1::ReqInt (uint32_t channel, IntReqInfoBase* intreq_info)
{
    //割込み要求をINTC1の割込みキューに登録し
    //最高優先度割込み情報を更新
    m_intc1_queue.insert(IntReqMap::value_type (channel, intreq_info));
    //m_intc1_queueに登録あればrun対象
    StartCyclicHandler ();
    //ContextId tcid = intreq_info->GetTcId ();
    UpdateHighPriority (NC);
//}
}

/*!
 * @brief Responds to EIINT request from CPU.
 * @param channel Channel number.
 * @param intc1 The pointer to INTC1.
 */
void CIntc1::ResIntFromCpu (uint32_t channel, CIntc1* intc1)
{
    if (intc1->IsIntc1Channel (channel) == true) {
        intc1->GetEEICPtr(channel)->SetEIRF(0);
        intc1->GetEICPtr(channel)->SetEIRF(0);
    } else {
        intc1->m_intc2->GetEEICPtr(channel)->SetEIRF(0);
        intc1->m_intc2->GetEICPtr(channel)->SetEIRF(0);
    }
}


/*!
 * @brief Initializes the priority information of the interrupt.
 * @param -
 */
void CIntc1::InitPriorityInfo (void)
{
    if (m_intc1_priority.empty() == true) {
        m_intc1_priority.insert (IntHighPriorityMap::value_type (NC, new IntPriorityInfo ()));
    }
}


/*!
 * @brief Cancels the interrupt request that already registered in CPU.
 * @param channel Channel number.
 */
void CIntc1::CancelIntByMask (uint32_t channel)
{
    //CPUの割込みキューから指定されたチャネルの割込みを削除
    IntReqMap::iterator ite = m_intc1_queue.find (channel);
    if (ite != m_intc1_queue.end()) {
        CancelIntToCpu (channel);
    }
}

void CIntc1::CancelEiintByMask (uint32_t channel)
{
    //CPUの割込みキューから指定されたチャネルの割込みを削除
    IntReqMap::iterator ite = m_intc1_queue.find (channel);
    if (ite != m_intc1_queue.end()) {
        CancelIntToCpu (channel);
    }
}


/*!
 * @brief Cancels the interrupt request from Peripheral module.
 * @param channel Channel number.
 */
void CIntc1::CancelIntByPeripheral (uint32_t channel)
{
    if (IsIntc1Channel (channel) == true) {
        GetEEICPtr(channel)->SetEIRF(0);
        GetEICPtr(channel)->SetEIRF(0);
    }
}


/*!
 * @brief Checks the interrupt queue.
 * @param channel Channel number.
 * @return bool
 */
bool CIntc1::IsIntReqInQueue (uint32_t channel)
{
    IntReqMap::iterator ite = m_intc1_queue.find (channel);
    return (ite != m_intc1_queue.end()) ? true : false;
}


/*!
 * @brief Responds to FENMI from CPU.
 * @param intc1 The pointer to INTC1.
 */
void CIntc1::ResFenmiFromCpu (CIntc1* intc1)
{
    intc1->GetFNCPtr()->SetFNRF(0);
}


/*!
 * @brief Responds to FEINT from CPU.
 * @param channel Channel number.
 * @param intc1 The pointer to INTC1.
 */
void CIntc1::ResFeintFromCpu (uint32_t channel, CIntc1* intc1)
{
    intc1->GetFICPtr(channel)->SetFIRF(0);

}

/*!
 * @brief Updates the request of the highest priority.
 * @param tcid Context ID.
 */
void CIntc1::UpdateHighPriority (ContextId tcid)
{
    //指定したtcidの要素が見つからない場合、バインドするHTは存在しないので
    //何もせずにそのままリターン
    if (m_intc1_priority.find (tcid) == m_intc1_priority.end ()) return;
    //割込みキューの中から指定されたHTにバインドされた割込みを探索し
    //該当する割込みの中で最も優先度の高い割込みチャネルを記録
    IntPriorityInfo* priority_info = m_intc1_priority[tcid];
    priority_info->Invalidate();
    IntReqMap::iterator ite = m_intc1_queue.begin();
    while (ite != m_intc1_queue.end()) {
        IntReqInfoBase* intreq_info = ite->second;
        if (intreq_info->GetTcId () == tcid) {
            uint32_t channel = ite->first;
            if (intreq_info->GetMask() == 0) {
                int32_t priority = intreq_info->GetPriority();
                int32_t priority_by_bind = priority;
                if (m_core_version >= CORE_VER_20){
                    if (IsIntReqMasked(intreq_info, channel, priority) == 1){  //check mask by operation mode, eibd.gm, eibd.gpid

                    }
                }else{
                    priority_info->UpdateChannel (channel, priority, priority_by_bind);
                }
            }
        }
        ++ ite;
    }
}

/*!
 * @brief Updates the request of the highest priority for GMxxx and BGxxx.
 * @param tcid Context ID.
 */
//update priority for GMxxx and BGxxx
//FENMI: pri = -17
//FEINT: pri = -16 + channel (0->15)
//EIINT: pri = 0 -> 255
//BGFEINT/GMFEINT: pri = 256 + 16 + pri(0->15)
//BGEIINT/GMEIINT: pri = 256 + 16 + 16  + pri(0->255)
void CIntc1::UpdateHighPriorityFromCpuMode(ContextId tcid ){
    //指定したtcidの要素が見つからない場合、バインドするHTは存在しないので
    //何もせずにそのままリターン
    if (m_intc1_priority.find (tcid) == m_intc1_priority.end ()) return;

    //割込みキューの中から指定されたHTにバインドされた割込みを探索し
    //該当する割込みの中で最も優先度の高い割込みチャネルを記録
    IntPriorityInfo* priority_info = m_intc1_priority[tcid];
    priority_info->Invalidate();
    IntReqMap::iterator ite = m_intc1_queue.begin();
    while (ite != m_intc1_queue.end()) {
        IntReqInfoBase* intreq_info = ite->second;
        if (intreq_info->GetTcId () == tcid) {
            uint32_t channel = ite->first;
            if (intreq_info->GetMask() == 0) {
                if (channel == FENMI_CHANNEL){
                    priority_info->UpdateChannelFenmi (channel);
                }else if (channel < FEINT_CHANNEL){ //EIINT
                    int32_t priority = intreq_info->GetPriority();
                    int32_t priority_by_bind = priority;
                    if (m_is_cpu_gm == 0){
                        CancelIntToCpu(channel, false);  // "false" is necessary for SC-HEAP // changed by Yoshinaga
                    }
                    if (IsIntReqMasked(intreq_info, channel, priority) == 0){    //check masked as table 3.2
                        //update priority for GMxxx and BGxxx
                        //EIINT: pri = 0 -> 255
                        //GMEIINT/BGEIINT: pri = 256 + +16 + 16+ pri(0->255)
                        //No masked interrupt request here
                        if (intreq_info->GetGM() == 1){
                            priority_by_bind = priority + 288;
                        }
                        priority_info->UpdateChannelEiint (channel, priority, priority_by_bind);
                    }else{

                    }
                }else{//FEINT
                    int32_t priority = intreq_info->GetPriority();
                    int32_t priority_by_bind = priority;
                    if (m_is_cpu_gm == 0){
                        CancelFeintToCpu(channel, false); // "false" is necessary for SC-HEAP // changed by Yoshinaga
                    }
                    if (IsIntReqMasked(intreq_info, channel, priority) == 0){    //check masked as table 3.2
                        //update priority for GMxxx and BGxxx
                        //FEINT: pri = -16 + channel (0->15)
                        //GMFEINT/BGFEINT: pri = 256 + 16 + pri [channel (0->15) - 16]
                        //No masked interrupt request here
                        if (intreq_info->GetGM() == 1){
                               priority_by_bind = priority + 272;
                        }
                        priority_info->UpdateChannelFeint (channel, priority, priority_by_bind);
                    }else{

                    }
                }
            }
        }
        ++ ite;
    }
}

bool CIntc1::IsIntReqMasked (IntReqInfoBase * intreq_info, uint32_t channel, int32_t pri){
uint32_t priority = (uint32_t)(pri);
if (channel < FEINT_CHANNEL){
    if ( ((m_is_cpu_gm == 0) && (intreq_info->GetGM() == 1) )
        || ((m_is_cpu_gm == 1) && (intreq_info->GetGM() == 1) && (m_cpu_gpid != intreq_info->GetGPID()) && (priority >= intreq_info->GetBGPR() ) )
       ){
        return true;
    }else{
        return false;
    }
}else{
    if ( ((m_is_cpu_gm == 0) && (intreq_info->GetGM() == 1) )
        || ((m_is_cpu_gm == 1) && (intreq_info->GetGM() == 1) && (m_cpu_gpid != intreq_info->GetGPID()) && (((intreq_info->GetBGE() >> (channel - FEINT_CHANNEL )) & 0x1U) == 0)  )
       ){
        return true;
    }else{
        return false;
    }
}
}

bool CIntc1::UpdateOverwriteBit_EIC (uint32_t channel)
{
    TRegEIC* eic = GetEICPtr(channel);
    TRegEEIC* eeic = GetEEICPtr(channel);
    if (eic->GetEICT() == 0){
        eeic->SetEIOV(1);
        eic->SetEIOV(1);

        return 1;
    }else{
        return 0;
    }
}

bool CIntc1::UpdateOverwriteBit_FIC (uint32_t channel)
{
    TRegFIC* fic = GetFICPtr(channel);
    if (fic->GetFICT() == 0){
        fic->SetFIOV(1);
        return 1;
    }else{
        return 0;
    }
}



/*!
 * @brief Creates the information object of the interrupt request.
 * @param channel Channel number.
 * @return The information object of the interrupt request.
 */
EiintReqInfo* CIntc1::CreateEiintReqInfo (uint32_t channel)
{
    TRegEIC*  eic  = GetEICPtr (channel);
    TRegEEIC* eeic  = GetEEICPtr (channel);
    TRegEIBD* eibd = GetEIBDPtr (channel);
    TRegDBMK* dbmk = GetDBMKPtr ();
    TRegEIBG* eibg = GetEIBGPtr ();

    //割込み要求情報クラスにEIC,EIBDを登録
    EiintReqInfo* intreq_info = new EiintReqInfo (eic, eeic, eibd, dbmk, eibg);
    return intreq_info;
}


/*!
 * @brief Creates the information object of FENMI request.
 * @param -
 * @return The information object of FENMI request.
 */
FenmiReqInfo* CIntc1::CreateFenmiReqInfo (void)
{
    TRegFNC*  fnc  = GetFNCPtr ();
    TRegFNBD* fnbd = GetFNBDPtr ();
    TRegDBMK* dbmk = GetDBMKPtr ();

    //FENMI要求情報クラスにFNC,FNBDを登録
    FenmiReqInfo* intreq_info = new FenmiReqInfo (fnc, fnbd, dbmk);
    return intreq_info;
}


/*!
 * @brief Creates the information object of FEINT request.
 * @param channel Channel number.
 * @return The information object of FEINT request.
 */
FeintReqInfo* CIntc1::CreateFeintReqInfo (uint32_t channel)
{
    TRegFIC*  fic  = GetFICPtr (channel);
    TRegFIBD* fibd = GetFIBDPtr (channel);
    TRegDBMK* dbmk = GetDBMKPtr ();
    TRegFIBG* fibg = GetFIBGPtr ();

    //FEINT要求情報クラスにFNC,FNBDを登録
    FeintReqInfo* intreq_info = new FeintReqInfo (fic, fibd, dbmk, fibg);
    return intreq_info;
}

/*!
 * @brief Gets FEINT priority.
 * @param -
 * @return FEINT priority
 */
int32_t FeintReqInfo::GetPriority (void) const
{
    return IntPriorityInfo::FEINT_PRIORITY + m_fic->GetChannel ();
}


/*!
 * @brief Clears the queue of the interrupt.
 * @param -
 */
void CIntc1::ClearIntReqQueue (void)
{
    IntReqMap::iterator ite0 = m_intc1_queue.begin();
    while (ite0 != m_intc1_queue.end()) {
        delete (ite0->second);
        ++ite0;
    }
    IntHighPriorityMap::iterator ite1 = m_intc1_priority.begin();
    while (ite1 != m_intc1_priority.end()) {
        ite1->second->Invalidate();
        ++ite1;
    }
    m_intc1_queue.clear();
    m_req_feint_channel = INIT_FEINT_CHANNEL; // set Invalid channel
}

/*!
 * @brief Checks the mode to detect high level.
 * @param channel Channel number.
 * @retval true: Detects by the level., false: Detects by the edge.
 */
bool CIntc1::IsIntLevelDetect (uint32_t channel)
{
    return (GetEICPtr(channel)->GetEICT() == 1) ? true : false;
}


/*!
 * @brief Removes INTC1 from the target list to run.
 * @param -
 */
// INTC1をrun対象から除外
void CIntc1::StopIntc1CyclicHandler (void)
{
    if ((GetFNCPtr()->GetFNRF() == 0) &&
        (m_intc1_queue.empty())) {
        //要求中のFENMI,FEINTが存在せず、m_intc1_queueが空の場合、run対象から除外
        StopCyclicHandler (this);
    }
}


/*!
 * @brief Sets the pointer to INTC2 class.
 * @param intc2 The pointer of type INTC2 class.
 */
void CIntc1::SetIntc2Ptr (CIntc2* intc2)
{
    m_intc2 = intc2;
}


/*!
 * @brief Sends the interrupt request.
 * @param channel Channel number.
 */
void CIntc1::ReqIntByEIC (uint32_t channel)
{
    //EICn.EIRFビットに1がセットされた場合、指定されたチャネルの割込み要求を
    //INTC1の割込みキューに登録し最高優先度割込み情報を更新
    EiintReqInfo* intreq_info = CreateEiintReqInfo (channel);
    ReqInt (channel, intreq_info);
}


/*!
 * @brief Cancels the interrupt request.
 * @param channel Channel number.
 */
void CIntc1::CancelInt (uint32_t channel)
{
    //CPU,INTC1の割込みキューから指定されたチャネルの割込みを削除
    //最高優先度割込み情報を更新
    IntReqMap::iterator ite = m_intc1_queue.find (channel);
    if (ite != m_intc1_queue.end()) {
        IntReqInfoBase* intreq_info = ite->second;
        //CPUの割込みキューから割込み要求を削除
        CancelIntToCpu (channel);
        //INTC1の割込みキューから割込み要求を削除
        delete (intreq_info);
        m_intc1_queue.erase (ite);
        StopIntc1CyclicHandler ();
        UpdateHighPriority (NC);
    }
}


/*!
 * @brief Sends the request of FENMI by FNC.FNRF set.
 * @param -
 */
void CIntc1::ReqFenmiByFNRF (void)
{
    //FNC.FNRFビットに1がセットされた場合、FENMI要求を
    //INTC1の割込みキューに登 録し最高優先度割込み情報を更新
    FenmiReqInfo* intreq_info = CreateFenmiReqInfo ();
    ReqInt (FENMI_CHANNEL, intreq_info);
}


/*!
 * @brief Cancels FENMI by FNC.FNRF cleared.
 * @param -
 */
void CIntc1::CancelFenmiByFNRF (void)
{
    //CPU,INTC1の割込みキューからFENMIを削除
    //最高優先度割込み情報を更新
    static const uint32_t channel = FENMI_CHANNEL;
    IntReqMap::iterator ite = m_intc1_queue.find (channel);
    if (ite != m_intc1_queue.end()) {
        IntReqInfoBase* intreq_info = ite->second;
        //CPUの割込みキューからFENMI要求を削除
        CancelFenmiToCpu ();
        //INTC1の割込みキューからFENMI要求を削除
        delete (intreq_info);
        m_intc1_queue.erase (ite);
        StopIntc1CyclicHandler ();
        UpdateHighPriority (NC);
    }
}


/*!
 * @brief Deletes FENMI request that already registered in CPU, from the queue.
 * @param -
 */
void CIntc1::CancelFenmiByMask (void)
{
    //CPUの割込みキューからFENMIを削除
    static const uint32_t channel = FENMI_CHANNEL;
    IntReqMap::iterator ite = m_intc1_queue.find (channel);
    if (ite != m_intc1_queue.end()) {
        CancelFenmiToCpu ();
    }
}


/*!
 * @brief Cancels FENMI request that already registered in CPU.
 * @param -
 */
void CIntc1::CancelFenmiToCpu (void)
{
#if !(_INTC1_UNIT_TEST)
    m_pe->GetHtPtr()->CancelFenmi ();
#else
    m_pe->GetHtPtr();
#endif
}


/*!
 * @brief Sends the request of FEINT by FIC.FIRF set.
 * @param channel Channel number.
 */
void CIntc1::ReqFeintByFIRF (uint32_t channel)
{
    //FIC.FIRFビットに1がセットされた場合、FEINT要求を
    //INTC1の割込みキューに登録し最高優先度割込み情報を更新
    FeintReqInfo* intreq_info = CreateFeintReqInfo (channel);
    ReqInt (FEINT_CHANNEL+channel, intreq_info);
}


/*!
 * @brief Deletes FEINT request from the queue by FIC.FIRF cleared.
 * @param channel Channel number.
 */
void CIntc1::CancelFeintByFIRF (uint32_t channel)
{
    //CPU,INTC1の割込みキューからFEINTを削除
    //最高優先度割込み情報を更新
    IntReqMap::iterator ite = m_intc1_queue.find (FEINT_CHANNEL + channel);
    if (ite != m_intc1_queue.end()) {
        IntReqInfoBase* intreq_info = ite->second;
        // If same channel of FEINT is requesting to CPU, delete the FEINT request.
        if (m_req_feint_channel == channel) {
            CancelFeintToCpu ();
            m_req_feint_channel = INIT_FEINT_CHANNEL; // set invalid channel
        }
        //INTC1の割込みキューからFEINT要求を削除
        delete (intreq_info);
        m_intc1_queue.erase (ite);
        StopIntc1CyclicHandler ();
        UpdateHighPriority (NC);
    }
}


/*!
 * @brief Deletes FEINT request that already registered in CPU.
 * @param channel Channel number.
 */
void CIntc1::CancelFeintByMask (uint32_t channel)
{
    // Delete FEINT request to CPU
    // (Keep FEINT req in m_intc1_queue because of FIC.FIRF=1.)
    IntReqMap::iterator ite = m_intc1_queue.find (FEINT_CHANNEL + channel);
    if (ite != m_intc1_queue.end()) {
        // If same channel of FEINT is requesting to CPU, delete the FEINT request.
        if (m_req_feint_channel == channel) {
            CancelFeintToCpu ();
            m_req_feint_channel = INIT_FEINT_CHANNEL; // set invalid channel
        }
    }
}


/*!
 * @brief Cancels FEINT request that already registered in CPU.
 * @param -
 */
void CIntc1::CancelFeintToCpu (void)
{
#if !(_INTC1_UNIT_TEST)
    m_pe->GetHtPtr()->CancelFeint ();
#else
    m_pe->GetHtPtr();
#endif
}


/*!
 * @brief Cancel the FEINT request which requested to CPU already. (added by Yoshinaga)
 * @param is_called_cyclic_handler Needed for SC-HEAP.
          Indicate where calls this function.
          true: called from CyclicHandler, false: called from UpdateHighPriorityFromCpuMode.
 * @note SC-HEAP will override this function and need arguments in it.
 */
void CIntc1::CancelFeintToCpu (bool is_called_cyclic_handler)
{
    CancelFeintToCpu();
}


/*!
 * @brief Cancel the FEINT request which requested to CPU already. (added by Yoshinaga)
 * @param channel the channel of FEINT. Needed for SC-HEAP.
 * @param is_called_cyclic_handler Needed for SC-HEAP.
          Indicate where calls this function.
          true: called from CyclicHandler, false: called from UpdateHighPriorityFromCpuMode.
 * @note SC-HEAP will override this function and need arguments in it.
 */
void CIntc1::CancelFeintToCpu (uint32_t channel, bool is_called_cyclic_handler)
{
    CancelFeintToCpu ();
}


/*!
 * @brief Gets the pointer to EIC register class.
 * @param channel Channel number.
 * @return The pointer of type EIC register class.
 */
TRegEIC* CIntc1::GetEICPtr (uint32_t channel) const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr eic_addr = trfs->TrEICAddr (channel);
    return trfs->TrEIC (eic_addr);
}

/*!
 * @brief Gets the pointer to EEIC register class.
 * @param channel Channel number.
 * @return The pointer of type EEIC register class.
 */
TRegEEIC* CIntc1::GetEEICPtr (uint32_t channel) const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr eeic_addr = trfs->TrEEICAddr (channel);
    return trfs->TrEEIC (eeic_addr);
}


/*!
 * @brief Gets the pointer to EIBD register class.
 * @param channel Channel number.
 * @return The pointer of type EIBD register class.
 */
TRegEIBD* CIntc1::GetEIBDPtr (uint32_t channel) const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr eibd_addr = trfs->TrEIBDAddr (channel);
    return trfs->TrEIBD (eibd_addr);
}

/*!
 * @brief Gets the pointer to DBMK register class.
 * @param -
 * @return The pointer of type DBMK register class.
 */

TRegDBMK* CIntc1::GetDBMKPtr () const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr dbmk_addr = trfs->TrDBMKAddr ();
    return trfs->TrDBMK (dbmk_addr);
}

/*!
 * @brief Gets the pointer to IHVCFG register class.
 * @param -
 * @return The pointer of type IHVCFG register class.
 */

TRegIHVCFG* CIntc1::GetIHVCFGPtr () const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr ihvcfg_addr = trfs->TrIHVCFGAddr ();
    return trfs->TrIHVCFG (ihvcfg_addr);
}


/*!
 * @brief Gets the pointer to EIBG register class.
 * @param -
 * @return The pointer of type EIBG register class.
 */

TRegEIBG* CIntc1::GetEIBGPtr () const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr eibg_addr = trfs->TrEIBGAddr (true);
    return trfs->TrEIBG (eibg_addr);
}

/*!
 * @brief Gets the pointer to FIBG register class.
 * @param -
 * @return The pointer of type FIBG register class.
 */

TRegFIBG* CIntc1::GetFIBGPtr () const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr fibg_addr = trfs->TrFIBGAddr ();
    return trfs->TrFIBG (fibg_addr);
}

/*!
 * @brief Gets the pointer to FNC register class.
 * @param -
 * @return The pointer of type FNC register class.
 */
TRegFNC* CIntc1::GetFNCPtr (void) const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr fnc_addr = trfs->TrFNCAddr ();
    return trfs->TrFNC (fnc_addr);
}


/*!
 * @brief Gets the pointer to FIC register class.
 * @param channel Channel number.
 * @return The pointer of type FIC register class.
 */
TRegFIC* CIntc1::GetFICPtr (uint32_t channel) const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr fic_addr = trfs->TrFICAddr (channel);
    return trfs->TrFIC (fic_addr);
}

/*!
 * @brief Gets the pointer to FNBD register class.
 * @param -
 * @return The pointer of type FNBD register class.
 */
TRegFNBD* CIntc1::GetFNBDPtr (void) const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr fnbd_addr = trfs->TrFNBDAddr ();
    return trfs->TrFNBD (fnbd_addr);
}


/*!
 * @brief Gets the pointer to FIBD register class.
 * @param channel Channel number.
 * @return The pointer of type FIBD register class.
 */
TRegFIBD* CIntc1::GetFIBDPtr (uint32_t channel) const
{
    TRegIntc1* trfs = static_cast<TRegIntc1*> (m_trfs);
    PhysAddr fibd_addr = trfs->TrFIBDAddr (channel);
    return trfs->TrFIBD (fibd_addr);
}


/*!
 * @brief Adds to the target list to run.
 * @param -
 */
void CIntc1::StartCyclicHandler (void)
{
    m_fsbus->AddCyclicHandler (this);
}


/*!
 * @brief Removes from the target list to run.
 * @param intc1 The pointer of type CIntc1.
 */
void CIntc1::StopCyclicHandler (CIntc1* intc1)
{
    m_fsbus->RemoveCyclicHandler (intc1);
}


/*!
 * @brief Sends the request of FENMI.
 * @param resfunc The pointer to callback function to respond to the interrupt request.
 */
void CIntc1::ReqFenmiToCpu (void (*resfunc)(CIntc1* intc))
{
#if !(_INTC1_UNIT_TEST)
    m_pe->GetHtPtr()->ReqFenmi (resfunc);
#else
    m_pe->GetHtPtr();
#endif
}


/*!
 * @todo parameter of (intc) in resfunc may be (intc1). please see intc1.h and fix it.
 */
void CIntc1::ReqFeintToCpu (uint32_t channel, void (*resfunc)(uint32_t channel, CIntc1* intc))
{
    ReqFeintToCpu (channel, false, 0, false, resfunc);
}


/*!
 * @brief Sends the request of FEINT.
 * @param channel Channel number.
 * @param bindmode
 * @param gpid
 * @param bge
 * @param resfunc The pointer to callback function to respond to the interrupt request.
 *
 * @todo To be documented.
 * @todo parameter of (intc) in resfunc may be (intc1). please see intc1.h and fix it.
 */
void CIntc1::ReqFeintToCpu (uint32_t channel, bool bindmode, uint32_t gpid, uint32_t bge, void (*resfunc)(uint32_t channel, CIntc1* intc))
{
#if _INTC1_UNIT_TEST
    //m_feint_cpu_ack = INT_E_OK;
#else
#endif
    ////m_feint_cpu_ack is forced in test case (UT)
    if (m_is_cpu_gm){    //cpu operation mode: guest mode
        if (bindmode){    //bind to guest mode
            if (gpid == m_cpu_gpid) {
                //generate GMFEINT
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqFeint (resfunc, channel, bindmode, gpid, false);
#endif
                m_req_feint_channel = channel;
            }else{
                if (((bge >> channel) & 0x1U) == 1 ){
                    //generate BGFEINT
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqFeint (resfunc, channel, bindmode, gpid, true);
#endif
                m_req_feint_channel = channel;
                }
            }
        }else{   //bind to host mode
            //generate FEINT
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqFeint (resfunc, channel);
#endif
            m_req_feint_channel = channel;
        }
    }else{    //cpu operation mode: host mode
        if (bindmode == 0){    //bind to host mode
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqFeint (resfunc, channel);
#endif
            m_req_feint_channel = channel;
        }
    }
}

/*!
 * @todo parameter of (intc) in resfunc may be (intc1). please see intc1.h and fix it.
 */
void CIntc1::ReqEiintToCpu (uint32_t priority, uint32_t channel, bool eitb,
                            void (*resfunc)(uint32_t channel, CIntc1* intc))
{
    ReqEiintToCpu (priority, channel, eitb, false, 0, false, resfunc);
}

/*!
 * @brief Sends the request of EIINT.
 * @param priority Priority.
 * @param channel Channel number.
 * @param eitb EICn.EITB bit.
 * @param bindmode
 * @param gpid
 * @param bgpr
 * @param resfunc The pointer to callback function to respond to the interrupt request.
 *
 * @todo To be documented.(bindmode, gpid and bgpr)
 * @todo parameter of (intc) in resfunc may be (intc1). please see intc1.h and fix it.
 */
void CIntc1::ReqEiintToCpu (uint32_t priority, uint32_t channel, bool eitb, bool bindmode, uint32_t gpid, uint32_t bgpr,
                            void (*resfunc)(uint32_t channel, CIntc1* intc))
{
#if _INTC1_UNIT_TEST
    //m_eiint_cpu_ack = INT_E_OK;
#else
#endif
    if (m_is_cpu_gm){    //cpu operation mode: guest mode
        if (bindmode){    //bind to guest mode
            if (gpid == m_cpu_gpid) {
                //generate GMEIINT
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqInterrupt (priority, ExpInfo::GetExpCause (EXP_GMEIINT) + channel, eitb, bindmode, gpid, false, resfunc);
#endif
            }else{
                if (priority <= bgpr ){
                   //generate BGEIINT
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqInterrupt (priority, ExpInfo::GetExpCause (EXP_BGEIINT) + channel, eitb, true, gpid, true, resfunc);
#endif
               }
            }
        }else{   //bind to host mode
            //generate EIINT
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqInterrupt (priority, ExpInfo::GetExpCause (EXP_EIINT) + channel, eitb, resfunc);
#endif
        }
    }else{    //cpu operation mode: host mode
        if (bindmode == 0){    //bind to host mode
#if _INTC1_UNIT_TEST
                m_pe->GetHtPtr();
#else
                m_pe->GetHtPtr()->ReqInterrupt (priority, ExpInfo::GetExpCause (EXP_EIINT) + channel, eitb, resfunc);
#endif
        }
    }
}


/*!
 * @brief Cancels EIINT.
 * @param channel Channel number.
 */
void CIntc1::CancelIntToCpu (uint32_t channel)
{
#if _INTC1_UNIT_TEST
    m_pe->GetHtPtr();
#else
    m_pe->GetHtPtr()->CancelInterrupt (ExpInfo::GetExpCause (EXP_EIINT) + channel);
    m_pe->GetHtPtr()->CancelInterrupt (ExpInfo::GetExpCause (EXP_GMEIINT) + channel);
    m_pe->GetHtPtr()->CancelInterrupt (ExpInfo::GetExpCause (EXP_BGEIINT) + channel);
#endif
}


/*!
 * @brief Cancels EIINT. (added by Yoshinaga)
 * @param channel Channel number.
 * @param is_called_cyclic_handler Needed for SC-HEAP.
          Indicate where calls this function.
          true: called from CyclicHandler, false: called from UpdateHighPriorityFromCpuMode
 * @note SC-HEAP will override this function and need argument dummy in it.
 */
void CIntc1::CancelIntToCpu (uint32_t channel, bool is_called_cyclic_handler)
{
  CancelIntToCpu (channel);
}


/*!
 * @brief Checks write enable for FNC.
 * @param sideband Sideband info.
 * @return DM value in sideband.
 */
bool TRegFNC::IsRegWriteEnable (SideBand sideband) const
{
    if (m_core_version >= CORE_VER_20 ){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0)&& (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }else{
        return sideband.GetDM() & 0x1U;
    }
}

bool TRegFNC::IsRegReadEnable (SideBand sideband) const
{
   if (m_core_version >= CORE_VER_20 ){
#if (_INTC2_UNIT_TEST)
       if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0)&& (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }else{
        return sideband.GetDM() & 0x1U;
    }
}

SysErrCause TRegFNC::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (IsRegReadEnable (sideband) == false) {
        std::fill_n (data, size, 0);
        return SYSERR_GBUS;
    }

    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            std::fill_n (data, size, 0);
            return SYSERR_NONE;
        }
    return ReadBody (addr, data, size);
}

/*!
 * @brief Writes to FNC.(from debugger)
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE
 */
SysErrCause TRegFNC::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            return SYSERR_NONE;
        }
    if (size == SIZE_16BIT) {
        uint16_t wdata = *(reinterpret_cast<uint16_t*> (data));
        SetFNCT ((wdata >> 15) & 0x01U);
        SetFNRF ((wdata >> 12) & 0x01U);
    } else if (size == SIZE_8BIT) {
        uint8_t wdata = *data;
        if ((addr & 0x1) == 1) {
            SetFNCT ((wdata >> (15-8)) & 0x01U);
            SetFNRF ((wdata >> (12-8)) & 0x01U);
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Writes to FNC.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegFNC::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable(sideband) == false){
        CfMsg::DPrint (MSG_WAR,
                    "--:--:--:-: <Warning: FNC write ignored>\n");
        return SYSERR_GBUS;
    }
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    //Replace FNCT before Write
    uint8_t wdata[2];
    memcpy (wdata, data, sizeof(wdata));
    if (size == SIZE_16BIT) {
        ReplaceFNCWriteData (wdata, 1, sideband.GetDM());
    } else if ((size == SIZE_8BIT) && ((addr & 0x1) == 0x1)) {
        ReplaceFNCWriteData (wdata, 0, sideband.GetDM());
    }

    return WriteBody(addr, wdata, size);
}

/*!
 * @brief Replaces from previous value to input data.
 * @param data Input data.
 * @param index Index array
 * @param DM DM value in sideband.
 */
void TRegFNC::ReplaceFNCWriteData (uint8_t* data, uint32_t index, uint32_t DM)
{
    // Because FNCT is ReadOnly bit, FNCT can't be updated by software
    if (GetFNCT() == 0x1) {
        data[index] |= FNCT_BYTE_BIT;
        // If FNCT is 1 (level detection), FNRF is read-only
        if (GetFNRF() == 0x1) {
            data[index] |= FNRF_BYTE_BIT;
        } else {
            data[index] &= FNRF_BYTE_MASK;
        }
    } else {
        if (m_core_version >= CORE_VER_20){
            if (DM == 1){
                data[index] &= FNCT_BYTE_MASK;
            }else{
                data[index] &= FNCT_FNRF_BYTE_MASK;
            }
        }else{
            data[index] &= FNCT_BYTE_MASK;
        }
    }
}

/*!
 * @brief Constructs TRegFNC register.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegFNC::TRegFNC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc)
        : TRegCNTBase (reg_size, min_access_size, reg_addr, TRegMask(FNC_MASK))
{
    m_treg = &m_data;
    m_intc1 = static_cast<CIntc1*> (tgtvc);
    m_core_version = m_intc1->GetCoreVersion();
}


/*!
 * @brief Writes to FNC.FNRF bit.
 * @param fnrf Data to write.
 */
void TRegFNC::SetFNRF (RegData fnrf)
{
    if (fnrf == m_bit.m_fnrf) return; //値の変化がなければ何もしない

    m_bit.m_fnrf = fnrf & 0x01U;

    if (m_bit.m_fnrf == 1) {
        m_intc1->ReqFenmiByFNRF ();
    } else {
        m_intc1->CancelFenmiByFNRF ();
    }
}

/*!
 * @brief Checks write enable of FIC.
 * @param sideband Sideband info.
 * @return DM value in sideband.
 */
bool TRegFIC::IsRegWriteEnable (SideBand sideband) const
{
    if (m_core_version >= CORE_VER_20 ){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0))
            {
                   return true;
            }else{
                if ((sideband.GetUM() == 0))
                {
                    TRegFIBD* fibd = m_intc1->GetFIBDPtr(m_channel);
                    if ((sideband.GetGPID() == fibd->GetGPID())
                            && (sideband.GetGM() == fibd->GetGM())){
                        return true;
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }
        }else{
            return true;
        }
    }else{
        return sideband.GetDM() & 0x1U;
    }
}

bool TRegFIC::IsRegReadEnable (SideBand sideband) const
{
    if (m_core_version >= CORE_VER_20 ){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0))
            {
                   return true;
            }else{
                if (sideband.GetUM() == 0)
                {
                    TRegFIBD* fibd = m_intc1->GetFIBDPtr(m_channel);
                    if ((sideband.GetGPID() == fibd->GetGPID())
                            && (sideband.GetGM() == fibd->GetGM())){
                        return true;
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }
        }else{
            return true;
        }
    }else{
        return true;
    }
}

SysErrCause TRegFIC::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (IsRegReadEnable (sideband) == false) {
        std::fill_n (data, size, 0);
        return SYSERR_GBUS;
    }

    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            std::fill_n (data, size, 0);
            return SYSERR_NONE;
        }
    return ReadBody (addr, data, size);
}
/*!
 * @brief Writes to FIC.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegFIC::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    if (size == SIZE_16BIT) {
        uint16_t wdata = *(reinterpret_cast<uint16_t*> (data));
        SetFICT ((wdata >> 15) & 0x01U);
        SetFIRF ((wdata >> 12) & 0x01U);
        if (m_core_version >= CORE_VER_20){
            SetFIOV ((wdata >> 5) & 0x01U);
        }
    } else if (size == SIZE_8BIT) {
        uint8_t wdata = *data;
        if ((addr & 0x1) == 1) {
            SetFICT ((wdata >> (15-8)) & 0x01U);
            SetFIRF ((wdata >> (12-8)) & 0x01U);
        }else{
            if (m_core_version >= CORE_VER_20){
                SetFIOV ((wdata >> 5) & 0x01U);
            }
        }
    }
    return SYSERR_NONE;
}

/*!
 * @brief Writes to FIC.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegFIC::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable(sideband) == false){
        CfMsg::DPrint (MSG_WAR,
                    "--:--:--:-: <Warning: FIC write ignored>\n");
        return SYSERR_GBUS;
    }
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    //Replace FICT before Write
    uint8_t wdata[2];
    memcpy (wdata, data, sizeof(wdata));
    if (size == SIZE_16BIT) {
        ReplaceFICWriteData (wdata, 1, sideband.GetDM());
        ReplaceFIOVriteData (wdata, 0, sideband.GetDM());
    } else if ((size == SIZE_8BIT) && ((addr & 0x1) == 0x1)) {
        ReplaceFICWriteData (wdata, 0, sideband.GetDM());
    } else if ((size == SIZE_8BIT) && ((addr & 0x1) == 0x0)) {
        ReplaceFIOVriteData (wdata, 0, sideband.GetDM());
    }

    return WriteBody(addr, wdata, size);
}

/*!
 * @brief Replaces from previous value to input data.
 * @param data input data
 * @param index index of the array
 * @param DM
 *
 * @todo To be documented.(DM)
 */
void TRegFIC::ReplaceFICWriteData (uint8_t* data, uint32_t index, uint32_t DM)
{
    // Because FICT is ReadOnly bit, FICT can't be updated by software
    if (GetFICT() == 0x1) {
        data[index] |= FICT_BYTE_BIT;
        // If FICT is 1 (level detection), FIRF is read-only
        if (GetFIRF() == 0x1) {
            data[index] |= FIRF_BYTE_BIT;
        } else {
            data[index] &= FIRF_BYTE_MASK;
        }
    } else {
        if (m_core_version >= CORE_VER_20){
            if (DM == 1){
                data[index] &= FICT_BYTE_MASK;
            }else{
                data[index] &= FICT_FIRF_BYTE_MASK;
            }
        }else{
            data[index] &= FICT_BYTE_MASK;
        }
    }
}

void TRegFIC::ReplaceFIOVriteData (uint8_t* data, uint32_t index, uint32_t DM)
{
    if ((GetFICT() == 0x1) || (DM == 0)) {
        data[index] &= FICT_FIOV_MASK;
    }
}


/*!
 * @brief Constructs TRegFIC register.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param channel Channel number.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegFIC::TRegFIC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, uint32_t channel, CModuleVc* tgtvc)
        : TRegCNTBase (reg_size, min_access_size, reg_addr, TRegMask(FIC_MASK))
{
    m_treg = &m_data;
    m_intc1 = static_cast<CIntc1*> (tgtvc);
    m_channel = channel;
    m_core_version = m_intc1->GetCoreVersion();
}


/*!
 * @brief Writes FIC.FIRF bit.
 * @param firf Data to write.
 */
void TRegFIC::SetFIRF (RegData firf)
{
    if (m_intc1->m_core_version == CORE_VER_10){
        if (firf == m_bit.m_firf) return; //値の変化がなければ何もしない
    }else{
        if (firf == m_bit.m_firf){
            if ( (m_bit.m_firf == 1) && (m_intc1->GetExternalFEINTreq() == true)){
                m_intc1->UpdateOverwriteBit_FIC(m_channel);
                m_intc1->SetExternalFEINTreq(false);
                return;
            }else{
                return;
            }
        }
    }

    m_bit.m_firf = firf & 0x01U;

    if (m_bit.m_firf == 1) {
        m_intc1->ReqFeintByFIRF (m_channel);
    } else {
        m_intc1->CancelFeintByFIRF (m_channel);
    }
}


/*!
 * @brief Sets FIBD.ET,TCID bit to FIC register class.
 * @param -
 *
 * @note Prototype is declared in intc2.h.
 */
template <>
void TRegBND<FIBD>::SetBind (void)
{
    //FICレジスタクラスのポインタを取得
    //TRegFIC* fic = m_intc->GetFICPtr (m_channel);
}


/*!
 * @brief Constructs TRegBND register.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param tgtvc The pointer to the target module of the basic class.
 * @param type INTC2 type. Standard, G4MH2.0, etc.(Defined in Peripheral/INTC2/include/intc2.h)
 *
 * @note Prototype is declared in intc2.h.
 */
template <class TR>
TRegBND<TR>::TRegBND (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                      uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type)
        : TRegIntcBase<TRegData32> (reg_size, min_access_size, reg_addr, TRegMask(FBD_MASK)),
          m_init_peid (peid), m_type(type)
{
    m_treg = m_tr.GetTRegDataPtr();
    m_tr.SetData (peid);
    m_intc = static_cast<CIntc1*> (tgtvc);
}

/*!
 * @brief Constructs TRegDBMK.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegDBMK::TRegDBMK (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc)
        : TRegCNTBase (reg_size, min_access_size, reg_addr, TRegMask(DBMK_MASK))
{
    m_treg = &m_data;
    m_intc1 = static_cast<CIntc1*> (tgtvc);
    //m_core_version = m_intc1->GetCoreVersion();
}

/*!
 * @brief Sets FN value in DBMK.
 * @param fn DBMK.FN value.
 */
void TRegDBMK::SetFN (RegData fn)
{
#if defined (_INTC1_UNIT_TEST)
#else
    if (fn == m_bit.m_fn) return; //値の変化がなければ何もしない
#endif

    m_bit.m_fn = fn & 0x01U;

    if (m_bit.m_fn == 1) {
        for (uint32_t ch = 0; ch < CIntc1::NUM_FENMI_CHANNEL; ++ch) {
            m_intc1->CancelFenmiByMask ();
        }
    }
    m_intc1->UpdateHighPriority (NC);
}

/*!
 * @brief Sets FI value in DBMK.
 * @param fi DBMK.FI value.
 */
void TRegDBMK::SetFI (RegData fi)
{
#if defined (_INTC1_UNIT_TEST)
#else
    if (fi == m_bit.m_fi) return; //値の変化がなければ何もしない
#endif

    m_bit.m_fi = fi & 0x01U;

    if (m_bit.m_fi == 1) {
        for (uint32_t ch = 0; ch < CIntc1::NUM_FEINT_CHANNEL; ++ch) {
            m_intc1->CancelFeintByMask (ch);
        }
    }
    m_intc1->UpdateHighPriority (NC);
}

/*!
 * @brief Set EI value in DBMK.
 * @param ei DBMK.EI value.
 */
void TRegDBMK::SetEI (RegData ei)
{
#if defined (_INTC1_UNIT_TEST)
#else
    if (ei == m_bit.m_ei) return; //値の変化がなければ何もしない
#endif

    m_bit.m_ei = ei & 0x01U;

    if (m_bit.m_ei == 1) {
        for (uint32_t channel = 0; channel < TRegIntc2::N_G4MH_EIC; channel++) {
            m_intc1->CancelEiintByMask(channel);
        }
    }
    m_intc1->UpdateHighPriority (NC);
}

/*!
 * @brief Writes to DBMK.(from debugger)
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegDBMK::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    if (size == SIZE_16BIT) {
        uint16_t wdata = *(reinterpret_cast<uint16_t*> (data));
        SetFN  ((wdata >> 2) & 0x01U);
        SetFI  ((wdata >> 1) & 0x01U);
        SetEI  ((wdata >> 0) & 0x01U);
    } else if (size == SIZE_8BIT) {
        uint8_t wdata = *data;
        if ((addr & 0x1) == 0) {
            SetFN  ((wdata >> 2) & 0x01U);
            SetFI  ((wdata >> 1) & 0x01U);
            SetEI  ((wdata >> 0) & 0x01U);
        }
    }
    return SYSERR_NONE;
}

/*!
 * @brief Writes to DBMK.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegDBMK::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_NONE;
    }
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }

    return  WriteBody(addr, data, size);
}

/*!
 * @brief Checks write enable of DBMK.
 * @param sideband Sideband info.
 * @return DM value in sideband.
 */
bool TRegDBMK::IsRegWriteEnable (SideBand sideband) const
{
    if (m_intc1->GetCoreVersion() >= CORE_VER_20 ){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetDM() == 1) &&
                             (sideband.GetGM() == 0) && (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
             return (sideband.GetDM() & 0x1);
        }
    }else{
        return (sideband.GetDM() & 0x1);
    }
}

bool TRegDBMK::IsRegReadEnable (SideBand sideband) const
{
    if (m_intc1->GetCoreVersion() >= CORE_VER_20 ){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetDM() == 1) &&
                             (sideband.GetGM() == 0) && (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
            return (sideband.GetDM() & 0x1);
        }
    }else{
        return true;
    }
}

SysErrCause TRegDBMK::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (IsRegReadEnable (sideband) == false) {
        std::fill_n (data, size, 0);
        return SYSERR_NONE;
    }

    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            std::fill_n (data, size, 0);
            return SYSERR_NONE;
        }
    return ReadBody (addr, data, size);
}

/*!
 * @brief Constructs TRegIHVCFG.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegIHVCFG::TRegIHVCFG (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc)
        : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr, TRegMask(IHVCFG_MASK))
{
    m_treg = &m_data;
    m_intc1 = static_cast<CIntc1*> (tgtvc);
    m_core_version = m_intc1->GetCoreVersion();
}

void TRegIHVCFG::Reset (void) {

    m_data.m_data32 = m_intc1->GetIHVEFromCPU();

}

/*!
 * @brief Writes to IHVCFG.(from debugger)
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegIHVCFG::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    uint32_t wdata = *(reinterpret_cast<uint32_t*> (data));
    if (size == SIZE_32BIT){
        SetIHVE(wdata & 0x1U);
        m_intc1->SetIHVEFromIHVCFG(wdata & 0x1U);
      }else if ((size == SIZE_16BIT) && ((addr & 0x1U) == 0)){  //16 low bits
        SetIHVE(wdata & 0x1U);
        m_intc1->SetIHVEFromIHVCFG(wdata & 0x1U);
    }else if ((size == SIZE_8BIT)&& ((addr & 0x3U) == 0)){ //8 low bits
        SetIHVE(wdata & 0x1U);
        m_intc1->SetIHVEFromIHVCFG(wdata & 0x1U);
    }
    return SYSERR_NONE;
}

/*!
 * @brief Writes to IHVCFG.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegIHVCFG::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        if (m_core_version >= CORE_VER_20 ){
            return SYSERR_GBUS;
        }
         return SYSERR_NONE;
    }
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    if (m_intc1->GetHasHVromCPU() == 0){
        *data = 0;
    }
    return  WriteBody(addr, data, size);
}

/*!
 * @brief Checks write enable of IHVCFG.
 * @param sideband Sideband info.
 * @return bool.
 */
bool TRegIHVCFG::IsRegWriteEnable (SideBand sideband) const
{
    if (m_core_version >= CORE_VER_20){
        if (GetIHVE() == 1){
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }else{
        return false;
    }

}

bool TRegIHVCFG::IsRegReadEnable (SideBand sideband) const
{
   if (m_core_version >= CORE_VER_20 ){
       if (GetIHVE() == 1){   //IHVE = 1
            if ((sideband.GetGM() == 0)&& (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }else{
        return false;
    }
}

SysErrCause TRegIHVCFG::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (IsRegReadEnable (sideband) == false) {
        std::fill_n (data, size, 0);
        if (m_core_version >= CORE_VER_20 ){
            return SYSERR_GBUS;
        }
        return SYSERR_NONE;
    }

    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            std::fill_n (data, size, 0);
            return SYSERR_NONE;
        }
    return ReadBody (addr, data, size);
}



/*!
 * @brief Constructs TRegFIBG.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegFIBG::TRegFIBG (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, CModuleVc* tgtvc)
        : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr, TRegMask(FIBG_MASK))
{
    m_treg = &m_data;
    m_intc1 = static_cast<CIntc1*> (tgtvc);
    m_core_version = m_intc1->GetCoreVersion();
}

/*!
 * @brief Writes to FIBG.(from debugger)
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegFIBG::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    uint32_t wdata = *(reinterpret_cast<uint32_t*> (data));
    if (size == SIZE_32BIT){
        SetBGE(wdata & 0xFFFFU);
    }else if ((size == SIZE_16BIT) && ((addr & 0x1U) == 0)){  //16 low bits
        SetBGE(wdata & 0xFFFFU);
    }else if ((size == SIZE_8BIT)&& ((addr & 0x3U) == 0)){
        SetBGE(wdata & 0xFFU);
    }else if((size == SIZE_8BIT)&& ((addr & 0x3U) == 1)){
        SetBGE((wdata & 0xFFU)<<8);
    }

    return SYSERR_NONE;
}

/*!
 * @brief Writes to FIBG.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegFIBG::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{

    if (IsRegWriteEnable (sideband) == false) {
        if (m_intc1->GetIHVEFromIHVCFG() == 0){
            return SYSERR_NONE;
        }
        return SYSERR_GBUS;
    }
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    return  WriteBody(addr, data, size);
}

/*!
 * @brief Checks write enable of FIBG.
 * @param sideband Sideband info.
 * @return DM value in sideband.
 */
bool TRegFIBG::IsRegWriteEnable (SideBand sideband) const
{
    if (m_core_version >= CORE_VER_20){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0)){
                return true;
            }else{
                 return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool TRegFIBG::IsRegReadEnable (SideBand sideband) const
{
    if (m_core_version >= CORE_VER_20 ){
#if (_INTC2_UNIT_TEST)
       if (1){   //IHVE = 1
#else
       if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0)&& (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        return true;
    }
}

SysErrCause TRegFIBG::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (IsRegReadEnable (sideband) == false) {
        if (m_intc1->GetIHVEFromIHVCFG() == 0){
            std::fill_n (data, size, 0);
            return SYSERR_NONE;
        }
        std::fill_n (data, size, 0);
        return SYSERR_GBUS;
    }
    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            std::fill_n (data, size, 0);
            return SYSERR_NONE;
        }

    return ReadBody (addr, data, size);
}
