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
#include "./forest.h"
#include "./fs_controler.h"

#if defined(_INTC2_UNIT_TEST)
#include "mock_intc1.h"
#include "mock_proc_element.h"
#include "mock_fsbus.h"
#include "gmock/gmock.h"
#else
#include "./proc_element.h"
#include "./fsbus.h"
#endif
#include "./intc1.h"
#include "./intc2.h"

/*!
 * @brief Instantiates INTC2 register.
 * @param -
 */
void TRegIntc2::InitTRegMap (void)
{
    Intc2ImplType intc2_impl_type = m_intc2->GetImplType();
    //アドレスをキーとしてマップに登録
    PhysAddr eic_addr = EIC_OFFSET;
    PhysAddr eeic_addr = EEIC_OFFSET;
    uint32_t eic_num = N_G4MH_EIC;
    uint32_t eeic_num = N_G4MH_EEIC;

    for (uint32_t i = 32; i < eic_num; i++) {
        TRegEIC* eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_addr + i * 2,
                                    i, m_peid, m_intc2);
        m_trf.insert (TRegMap::value_type (eic->GetTRegAddr(), eic));
    }

    for (uint32_t i = 32; i < eeic_num; i++) {
        TRegEEIC* eeic = new TRegEEIC (SIZE_32BIT, SIZE_8BIT, eeic_addr + i * 4,
                                        i, m_peid, m_intc2);
         m_trf.insert (TRegMap::value_type (eeic->GetTRegAddr(), eeic));
     }


    PhysAddr imr_addr = IMR_OFFSET;
    uint32_t imr_num = N_G4MH_IMR;

    for (uint32_t i = 1; i < imr_num; i++) {
        TRegIMR* imr = new TRegIMR (SIZE_32BIT, SIZE_8BIT, imr_addr + i * 4,
                                    i, m_intc2);
        m_trf.insert (TRegMap::value_type (imr->GetTRegAddr(), imr));
    }

    PhysAddr eibd_addr = EIBD_OFFSET;
    uint32_t eibd_num = N_G4MH_EIBD;

    for (uint32_t i = 32; i < eibd_num; i++) {
        if (m_intc2->GetCoreVersion() >= CORE_VER_20){
            TRegEIBD* eibd = new TRegEIBD (SIZE_32BIT, SIZE_8BIT, eibd_addr + i * 4,
                TRegMask (TRegBND<EIBD>::INTC2_EIBD_MASK_G4MH20), i, m_peid, m_intc2, intc2_impl_type);
            m_trf.insert (TRegMap::value_type (eibd->GetTRegAddr(), eibd));
        }else{
            TRegEIBD* eibd = new TRegEIBD (SIZE_32BIT, SIZE_8BIT, eibd_addr + i * 4,
                TRegMask (TRegBND<EIBD>::INTC2_EIBD_MASK_G4MH), i, m_peid, m_intc2, intc2_impl_type);
            m_trf.insert (TRegMap::value_type (eibd->GetTRegAddr(), eibd));
        }
    }

    PhysAddr eibg_addr = EIBG_OFFSET;
    uint32_t eibg_num = N_G4MH20_EIBG;
    TRegMask mask = TRegMask (TRegEIBG::EIBG_MASK_INTC2);

    for (uint32_t i = 0; i < eibg_num; i++) {
        TRegEIBG* eibg = new TRegEIBG (SIZE_32BIT, SIZE_8BIT, eibg_addr + i * 4,mask, i, m_intc2);
        m_trf.insert (TRegMap::value_type (eibg->GetTRegAddr(), eibg));
    }
}


/*!
 * @brief Gets the address of EIC register.
 * @param channel Channel number.
 * @return The address of EIC register.
 */
PhysAddr TRegIntc2::TrEICAddr (uint32_t channel) const
{
    return (EIC_OFFSET + (channel) * 2);
}

/*!
 * @brief Gets the address of EEIC register.
 * @param channel Channel number.
 * @return The address of EEIC register.
 */
PhysAddr TRegIntc2::TrEEICAddr (uint32_t channel) const
{
    return (EEIC_OFFSET + (channel) * 4);
}



/*!
 * @brief Gets the address of EIBD register.
 * @param channel Channel number.
 * @return The address of EIBD register.
 */
PhysAddr TRegIntc2::TrEIBDAddr (uint32_t channel) const
{
    return (EIBD_OFFSET + (channel) * 4);
}

PhysAddr TRegIntc2::TrEIBGAddr (uint32_t peid) const
{
    return (EIBG_OFFSET + (peid) * 4);
}

/*!
 * @brief Writes to the register.
 * @param sideband Sideband.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @retval true Normal end.
 * @retval false Abnormal end.
 */
SysErrCause CIntc2::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr & INTC2_REG_ADDR_MASK)->Write (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Reads from the register.
 * @param sideband Sideband.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @retval true Normal end.
 * @retval false Abnormal end.
 */
SysErrCause CIntc2::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr & INTC2_REG_ADDR_MASK)->Read (sideband, addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Writes to the register with debugging.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @retval true Normal end.
 * @retval false Abnormal end.
 */
SysErrCause CIntc2::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr & INTC2_REG_ADDR_MASK)->WriteBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Reads from the register with debugging.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @retval true Normal end.
 * @retval false Abnormal end.
 */
SysErrCause CIntc2::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    m_trfs->GetTrPtr(addr & INTC2_REG_ADDR_MASK)->ReadBody (addr, data, size);
    return SYSERR_NONE;
}


/*!
 * @brief Resets INTC2.
 * @param -
 */
void CIntc2::HardReset (void)
{
    m_trfs->ResetTRegs();
    ClearIntReqQueue();
    StopCyclicHandler();
}


/*!
 * @brief Main function.
 * @param -
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::CyclicHandler (void)
{
    //最高優先度割込み情報を参照し、各PEにバインドされた
    //最高優先度割込みをINTC1に対し通知
#if defined(_INTC2_UNIT_TEST)
    uint32_t peid = 0;
    {
        {
#else
    PeVec::const_iterator peit = m_pes->begin ();
    while (peit != m_pes->end ()) {
        if ((*peit) != NULL) {
            uint32_t peid = (*peit)->GetPeId ();
#endif
            uint32_t channel = m_intc2_priority[peid]->GetChannel();
            if (m_core_version >= CORE_VER_20){
               channel = m_intc2_priority[peid]->GetChannelEiint();
                UpdateHighPriority (peid);
            }
            //INTC2の割込みキューから指定されたチャネルの割込みを選択し
            //引数で指定したPEのINTC1に対し割込みを通知
            //通知後、INTC2の割込みキューから削除
            IntReqMap::iterator ite = m_intc2_queue.find (channel);
            if (ite != m_intc2_queue.end()) {
                ReqIntToIntc1 (peid, channel, ite->second);
                m_intc2_queue.erase (ite);
                UpdateHighPriority (peid);
            }
        }
#if defined(_INTC2_UNIT_TEST)
#else
        ++ peit;
#endif
    }
    //INTC1に対し同報通知
    ReqBcasIntToIntc1 ();
}
#endif

#if defined(_INTC2_UNIT_TEST)
/*!
 * @brief Constructs INTC2 for mock.
 * @param pes ProcElement type vector pointer.
 * @param mock_fsbus MockFsBus type pointer.
 * @param type INTC2 Implementation type.
 */
using ::testing::AtLeast;
CIntc2::CIntc2 (PeVec* pes, MockFsBus* mock_fsbus, Intc2ImplType type)
    : m_pes (pes), m_mock_fsbus (mock_fsbus), m_impl_type (type)
{
    if (type == INTC2_G4MH20){
        m_core_version = CORE_VER_20;
    }else{
        m_core_version = CORE_VER_10;
    }
    m_external_EIINT_req = false;

    m_trfs = new TRegIntc2(this);
    m_external_EIINT_req = false;
    m_mock_dummy_dbmk = new MockTRegDBMK (0, 0, 0, this);
    EXPECT_CALL((*m_mock_dummy_dbmk), Reset()).Times(AtLeast(0));
    m_mock_dummy_dbmk->Reset ();
    EXPECT_CALL((*m_mock_dummy_dbmk), GetEI()).Times(AtLeast(0));

    IntPriorityInfo* hp_info = new IntPriorityInfo();
    m_intc2_priority.push_back (hp_info);
    uint32_t peid = 0;
    m_is_cpu_gm[peid] = 0;
    m_cpu_gpid[peid] = 0;
}
#elif !defined(_INTC1_UNIT_TEST)
/*!
 * @brief Constructs INTC2.
 * @param pes The pointer of type ProcElement vector.
 * @param fsbus The pointer of type FsBus.
 * @param type The type of implementation of INTC2.
 */
CIntc2::CIntc2 (PeVec* pes, FsBus* fsbus, Intc2ImplType type)
    : m_pes (pes), m_fsbus (fsbus), m_impl_type (type)
{
    if (type == INTC2_G4MH20){
        m_core_version = CORE_VER_20;
    }else{
        m_core_version = CORE_VER_10;
    }
    m_external_EIINT_req = false;

    //INTC2レジスタセットクラスをインスタンス
    m_trfs = new TRegIntc2(this);
    m_dummy_dbmk = new TRegDBMK (0, 0, 0, this);
    m_dummy_dbmk->Reset ();

    PeVec::const_iterator peit = m_pes->begin ();
    while (peit != m_pes->end ()) {
        //最高優先度割込み情報を初期化
        IntPriorityInfo* hp_info = new IntPriorityInfo();
        m_intc2_priority.push_back (hp_info);
        if ((*peit) != NULL) {
            uint32_t peid = (*peit)->GetPeId ();
            //INTC2クラスポインタをINTC1へ通知
            (*m_pes)[peid]->SetIntc2Ptr (this);
            m_is_cpu_gm[peid] = 0;
            m_cpu_gpid[peid] = 0;
        }
        ++ peit;
    }

}
#endif

/*!
 * @brief Destructs INTC2.
 * @param -
 */
#if !(_INTC1_UNIT_TEST)
CIntc2::~CIntc2 ()
{
    delete m_trfs;
    IntReqMap::iterator ite = m_intc2_queue.begin();
    while (ite != m_intc2_queue.end()) {
        delete (ite->second);
        ++ ite;
    }
    std::vector<IntPriorityInfo*>::iterator it = m_intc2_priority.begin ();
    while (it != m_intc2_priority.end ()) {
        delete (*it);
        ++ it;
    }
    m_intc2_queue.clear();
    m_intc2_bcas.clear();
    m_intc2_priority.clear();
#if defined(_INTC2_UNIT_TEST)
    delete m_mock_dummy_dbmk;
#else
    delete m_dummy_dbmk;
#endif
}
#endif

/*!
 * @brief Sends the interrupt request from peripheral module.
 * @param channel Channel number.(32...511)
 * @return IntErrorCode.
 */
#if !(_INTC1_UNIT_TEST)
IntErrorCode CIntc2::ReqIntByPeripheral (uint32_t channel)
{
    if (IsIntc2Channel (channel) == true) {
        CfMsg::IntcPrint ("<INTC2: EIINT%u is noticed to INTC2>\n", channel);
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
#endif

/*!
 * @brief Updates the information of the highest priority interrupt.
 * @param peid EIBDn.PEID bit.
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::UpdateHighPriority (uint32_t peid)
{
    //割込みが存在しないPEにバインドされているので
    //何もせずそのままリターン
    if (IsValidPeId (peid) == false) return;

    //割込みキューの中から指定されたPEにバインドされた割込みを探索し
    //該当する割込み(同報通知用チャネルは対象外)の中で最も優先度の高い割込みチャネルを記録
    IntPriorityInfo* priority_info = m_intc2_priority[peid];
    priority_info->Invalidate();
    IntReqMap::iterator ite = m_intc2_queue.begin();
    while (ite != m_intc2_queue.end()) {
        IntReqInfoBase* intreq_info = ite->second;
        if (peid == intreq_info->GetPEID()) {
#if defined (_INTC2_UNIT_TEST)
            {
#else
            if ((intreq_info->GetMask() == 0) && (intreq_info->GetCST() == 0)) {
#endif
                uint32_t channel = ite->first;
                int32_t priority = intreq_info->GetPriority();
                int32_t priority_by_bind = priority;
                if (m_core_version >= CORE_VER_20){
                    if (IsIntReqMasked(intreq_info, channel, priority, peid) == 0){  //check mask by operation mode, eibd.gm, eibd.gpid
                        priority_info->UpdateChannelEiint (channel, priority, priority_by_bind);
                    }
                }else{
                    priority_info->UpdateChannel (channel, priority, priority_by_bind);
                }
            }
        }
        ++ ite;
    }
}
#endif

bool CIntc2::IsIntReqMasked (IntReqInfoBase * intreq_info, uint32_t channel, int32_t pri, uint32_t peid)
{
    uint32_t priority = (uint32_t)(pri);
    if ( ((m_is_cpu_gm[peid] == 0) && (intreq_info->GetGM() == 1) )
            || ((m_is_cpu_gm[peid] == 1) && (intreq_info->GetGM() == 1) && (m_cpu_gpid[peid] != intreq_info->GetGPID()) && (priority >= intreq_info->GetBGPR() ) )
       ){
        return true;
    }else{
        return false;
    }
}

bool CIntc2::UpdateOverwriteBit_EIC (uint32_t channel)
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

/*!
 * @brief Broadcasts the interrupt for every INTC1 in all PEs.
 * @param -
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::ReqBcasIntToIntc1 (void)
{
    //m_intc2_bcasに登録されている全ての同報通知を全PEのINTC1へ通知し
    //m_intc2_bcasをクリアする
    BcasMap::iterator ite = m_intc2_bcas.begin();
    while (ite != m_intc2_bcas.end()) {
        uint32_t intc2_channel = ite->first;
        uint32_t bcas_channel  = ite->second;
#if defined(_INTC2_UNIT_TEST)
        uint32_t peid = 0;
        {
            {
#else
        PeVec::const_iterator peit = m_pes->begin ();
        while (peit != m_pes->end ()) {
            if ((*peit) != NULL) {
                uint32_t peid = (*peit)->GetPeId ();
#endif
                (*m_pes)[peid]->ReqIntByPeripheral (bcas_channel);
            }
#if defined(_INTC2_UNIT_TEST)
#else
            ++ peit;
#endif
        }
        //INTC2の割込みキューから当該チャネルを削除
           GetEICPtr(intc2_channel)->SetEIRF(0);
           GetEEICPtr(intc2_channel)->SetEIRF(0);
        ++ ite;
    }
    m_intc2_bcas.clear();
}
#endif

/*!
 * @brief Adds the information of the interrupt to broadcast.
 * @param channel Channel number.
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::AddBcasIntInfo (uint32_t channel)
{
    IntReqMap::iterator ite = m_intc2_queue.find (channel);
    if (ite != m_intc2_queue.end()) {
        //EIBDn.BCPから同報通知用割込みチャネル番号を取得
        uint32_t bcas_channel = GetBcasChannel (ite->second->GetBCP());
        m_intc2_bcas.insert (BcasMap::value_type (channel, bcas_channel));
    }
}
#endif


/*!
 * @brief Deletes the information of the interrupt to broadcast.
 * @param channel Channel number.
 */
void CIntc2::DelBcasIntInfo (uint32_t channel)
{
    BcasMap::iterator ite = m_intc2_bcas.find (channel);
    if (ite != m_intc2_bcas.end()) {
        m_intc2_bcas.erase (ite);
    }
}

/*!
 * @brief Gets the channel number of the interrupt to broadcast, from  EIBDn.BCP bit.
 * @param bcp EIBDn.BCP bit.
 * @return The channel number of the interrupt to broadcast.
 */
#if !(_INTC1_UNIT_TEST)
uint32_t CIntc2::GetBcasChannel (uint32_t bcp)
{
    return BCAS_BASE_CHANNEL + bcp;
}
#endif

/*!
 * @brief Creates the information object of EIINT request.
 * @param channel Channel number.
 * @return EiintReqInfo*.
 */
EiintReqInfo* CIntc2::CreateEiintReqInfo (uint32_t channel)
{
    TRegEIC*  eic  = GetEICPtr (channel);
    TRegEEIC*  eeic  = GetEEICPtr (channel);
    TRegEIBD* eibd = GetEIBDPtr (channel);

#if defined(_INTC2_UNIT_TEST)
    MockTRegDBMK* mock_dbmk = MockGetDBMKPtr ();
#else
    TRegDBMK* dbmk = GetDBMKPtr ();
#endif
    TRegEIBG* eibg = GetEIBGPtr (eibd->GetPEID());
#if defined(_INTC2_UNIT_TEST)
    EiintReqInfo* intreq_info = new EiintReqInfo (eic, eeic, eibd, mock_dbmk, eibg);
#else
    EiintReqInfo* intreq_info = new EiintReqInfo (eic, eeic, eibd, dbmk, eibg);
#endif
    return intreq_info;
}


/*!
 * @brief Clears the queue of the interrupt.
 * @param -
 */
void CIntc2::ClearIntReqQueue (void)
{
    IntReqMap::iterator ite = m_intc2_queue.begin();
    while (ite != m_intc2_queue.end()) {
        delete (ite->second);
        ++ ite;
    }
#if defined(_INTC2_UNIT_TEST)
    uint32_t peid = 0;
    {
        {
#else
    PeVec::const_iterator peit = m_pes->begin ();
    while (peit != m_pes->end ()) {
        if ((*peit) != NULL) {
            uint32_t peid = (*peit)->GetPeId ();
#endif
            m_intc2_priority[peid]->Invalidate ();
        }
#if defined(_INTC2_UNIT_TEST)
#else
        ++ peit;
#endif
    }
    m_intc2_queue.clear();
    m_intc2_bcas.clear();
}


/*!
 * @brief Checks the channel of INTC2.
 * @param channel Channel number.
 * @retval true It is the channel of INTC2.
 * @retval false It is not the channel of INTC2.
 */
bool CIntc2::IsIntc2Channel (uint32_t channel) const
{
    uint32_t eic_num = TRegIntc2::N_G4MH_EIC;
#if defined(_INTC2_UNIT_TEST)
    return channel >= MockTRegIntc1::N_EIC && channel < (eic_num);
#else
    return channel >= TRegIntc1::N_EIC && channel < (eic_num);
#endif
}

/*!
 * @brief Checks the validation of PE.
 * @param peid PE number.
 * @retval true Valid.
 * @retval false Invalid.
 */
bool CIntc2::IsValidPeId (uint32_t peid) const
{
    if (peid < m_pes->size ()) {
        return (*m_pes)[peid] != NULL;
    } else {
        return false;
    }
}

/*!
 * @brief Sends the interrupt request.
 * @param channel Channel number.
 */
void CIntc2::ReqIntByEIC (uint32_t channel)
{
    //EICn.EIRFビットに1がセットされた場合、指定されたチャネルの割込み要求を
    //INTC2の割込みキューに登録し最高優先度割込み情報を更新
    EiintReqInfo* intreq_info = CreateEiintReqInfo (channel);
    if (intreq_info != NULL) {
        //PEIDが0の場合、NULLが返る
        //割込みがPEにバインドされないので、キューには登録しない
        m_intc2_queue.insert(IntReqMap::value_type (channel, intreq_info));
        //m_intc2_queueに登録あればrun対象
        StartCyclicHandler();
        if (intreq_info->GetCST() == 0) {
            //通常割り込み
            uint32_t peid = intreq_info->GetPEID();
            UpdateHighPriority (peid);
        } else {
            //同報通知
            //優先度判定(UpdateHighPriority)は行わない
            if (intreq_info->GetMask() == 0) {
                //同報通知情報をバッファに登録
                AddBcasIntInfo (channel);
            } else {
                //同報通知情報をバッファから削除
                DelBcasIntInfo (channel);
            }
        }
    }
}


/*!
 * @brief Cancels the interrupt request.
 * @param channel Channel number.
 */
void CIntc2::CancelInt (uint32_t channel)
{
    //指定されたチャネルの割込みがINTC2のキューに登録されている場合、
    //削除して最高優先度割込み情報を更新
    //登録されていない場合、INTC1,CPUのキューを探索し、存在すれば削除する
    uint32_t peid = GetEIBDPtr (channel)->GetPEID();
    IntReqMap::iterator ite = m_intc2_queue.find (channel);
    if (ite != m_intc2_queue.end()) {
        uint32_t cst = ite->second->GetCST();
        //INTC2の割込みキューから割込み要求を削除
        delete (ite->second);
        m_intc2_queue.erase (ite);
        if (m_intc2_queue.empty ()) {
            //m_intc2_queueが空ならrun対象から除外
            StopCyclicHandler();
        }
        if (cst == 0) {
            UpdateHighPriority (peid);
        }
    } else {
        if (IsValidPeId (peid) == true) {
            //INTC1とCPUの割込みキューから割込み要求を削除
            CancelIntToIntc1 (peid, channel);
        }
    }
}


/*!
 * @brief Cancels the interrupt request from the queue in CPU.
 * @param channel Channel number.
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::CancelIntByMask (uint32_t channel)
{
    //EICn.EIMKビットに1がセットされた場合、CPUの割込みキューから
    //指定されたチャネルの割込みを削除
    uint32_t peid = GetEIBDPtr (channel)->GetPEID();
    if (IsValidPeId (peid) == true) {
        (*m_pes)[peid]->CancelIntByMask (channel);
    }
}
#endif

/*!
 * @brief Cancels the interrupt request from Peripheral module.
 * @param channel Channel number.
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::CancelIntByPeripheral (uint32_t channel)
{
    if (IsIntc2Channel (channel) == true) {
           GetEICPtr(channel)->SetEIRF(0);
           GetEEICPtr(channel)->SetEIRF(0);
    }
}
#endif

/*!
 * @brief Updates the request of the highest priority in INTC1.
 * @param peid EIBDn.PEID bit.
 * @param channel Channel number.
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::UpdateHighPriorityOfIntc1 (uint32_t peid, uint32_t channel)
{
    (*m_pes)[peid]->UpdateIntHighPriority (NC);
}
#endif

/*!
 * @brief Checks the mode to detect high level.
 * @param channel Channel number.
 * @retval true Detects by the level.
 * @retval false Detects by the edge.
 */
bool CIntc2::IsIntLevelDetect (uint32_t channel)
{
    if (IsIntc2Channel (channel) == true) {
        return (GetEICPtr(channel)->GetEICT() == 1) ? true : false;
    } else {
        return false;
    }
}


/*!
 * @brief Checks the interrupt queue in INTC2.
 * @param channel Channel number.
 * @return bool.
 */
bool CIntc2::IsIntReqInQueue (uint32_t channel)
{
    IntReqMap::iterator ite = m_intc2_queue.find (channel);
    return (ite != m_intc2_queue.end()) ? true : false;
}


/*!
 * @brief Checks the interrupt queue in INTC1.
 * @param peid Identification of PE.
 * @param channel Channel number.
 * @return bool.
 */
#if !(_INTC1_UNIT_TEST)
bool CIntc2::IsIntReqInQueueOfIntc1 (uint32_t peid, uint32_t channel)
{
    if (IsValidPeId (peid) == true) {
        return (*m_pes)[peid]->IsIntReqInQueue (channel);
    } else {
        return false;
    }
}
#endif

/*!
 * @brief Gets the pointer to EIC register class.
 * @param channel Channel number.
 * @return The pointer of type EIC register class.
 */
TRegEIC* CIntc2::GetEICPtr (uint32_t channel) const
{
    TRegIntc2* trfs = static_cast<TRegIntc2*> (m_trfs);
    PhysAddr eic_addr = trfs->TrEICAddr (channel);
    return trfs->TrEIC (eic_addr);
}

/*!
 * @brief Gets the pointer to EEIC register class.
 * @param channel Channel number.
 * @return The pointer of type EEIC register class.
 */
TRegEEIC* CIntc2::GetEEICPtr (uint32_t channel) const
{
    TRegIntc2* trfs = static_cast<TRegIntc2*> (m_trfs);
    PhysAddr eeic_addr = trfs->TrEEICAddr (channel);
    return trfs->TrEEIC (eeic_addr);
}

/*!
 * @brief Gets the pointer to EIBD register class.
 * @param channel Channel number.
 * @return The pointer of type EIBD register class.
 */
TRegEIBD* CIntc2::GetEIBDPtr (uint32_t channel) const
{
    TRegIntc2* trfs = static_cast<TRegIntc2*> (m_trfs);
    PhysAddr eibd_addr = trfs->TrEIBDAddr (channel);
    return trfs->TrEIBD (eibd_addr);
}


#if defined(_INTC2_UNIT_TEST)
MockTRegDBMK* CIntc2::MockGetDBMKPtr () const
{
    return m_mock_dummy_dbmk;
}
#else
/*!
 * @brief Gets the pointer to DBMK  register class.
 * @param -
 * @return dummy value 0.
 */
TRegDBMK* CIntc2::GetDBMKPtr () const
{
    return m_dummy_dbmk; // Dummy DBMK (which has value=0)
}
#endif

/*!
 * @brief Gets the pointer to EIBG register class.
 * @param peid PE number.
 * @return The pointer of type EIBG register class.
 */
TRegEIBG* CIntc2::GetEIBGPtr (uint32_t peid) const
{
    TRegIntc2* trfs = static_cast<TRegIntc2*> (m_trfs);
    PhysAddr eibg_addr = trfs->TrEIBGAddr (peid);
    return trfs->TrEIBG (eibg_addr);

}



/*!
 * @brief Adds to the target list to run.
 * @param -
 */
void CIntc2::StartCyclicHandler (void)
{
#if defined(_INTC2_UNIT_TEST)
    m_mock_fsbus->AddCyclicHandler (this);
#else
    m_fsbus->AddCyclicHandler (this);
#endif
}


/*!
 * @brief Removes from the target list to run.
 * @param -
 */
void CIntc2::StopCyclicHandler (void)
{
#if defined(_INTC2_UNIT_TEST)
    m_mock_fsbus->RemoveCyclicHandler (this);
#else
    m_fsbus->RemoveCyclicHandler (this);
#endif
}


/*!
 * @brief Sends the interrupt request to INTC1 in the PE which set by the parameter.
 * @param peid Identification of PE.
 * @param channel Channel number.
 * @param reqinfo Information of EIINT request.
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::ReqIntToIntc1 (uint32_t peid, uint32_t channel, IntReqInfoBase* reqinfo)
{
    (*m_pes)[peid]->ReqIntByIntc2 (channel, reqinfo);
}
#endif


/*!
 * @brief Cancels the interrupt request to INTC1 in the PE which set by the parameter.
 * @param peid Identification of PE.
 * @param channel Channel number.
 */
#if !(_INTC1_UNIT_TEST)
void CIntc2::CancelIntToIntc1 (uint32_t peid, uint32_t channel)
{
    (*m_pes)[peid]->CancelIntByIntc2 (channel);
}
#endif


/*!
 * @brief Writes to bit in INTC.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SysErrCause.
 */
template <class TR>
SysErrCause TRegIntcBase<TR>::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_NONE;
    }
    if (this->IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
    throw std::range_error("Un-guaranteed Size Access");
#endif
        this->PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    return this->WriteBody (addr, data, size);
}


/*!
 * @brief Checks write enable for the register.
 * @param sideband Sideband info.
 * @retval true Enabled.
 * @retval false Not enabled.
 */
bool TRegCNTBase::IsRegWriteEnable (SideBand sideband) const
{
    return true;
}


bool TRegEIC::IsRegWriteEnable (SideBand sideband) const
{
    if ((m_core_version >= CORE_VER_20) &&(CIntc1::IsIntc1Channel(m_channel))){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
            {
                return true;
            }else{
                if (sideband.GetUM() == 0){
                #if (_INTC2_UNIT_TEST)
                    TRegEIBD* eibd = m_mock_intc1->GetEIBDPtr(m_channel);
                    if ((sideband.GetGPID() == eibd->GetGPID())
                            && (sideband.GetGM() == eibd->GetGM())){
                #else
                    TRegEIBD* eibd = m_intc1->GetEIBDPtr(m_channel);
                    if ((sideband.GetGPID() == eibd->GetGPID())
                            && (sideband.GetGM() == eibd->GetGM())){
                #endif
                        return true;
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }
        }else{   // IHVE = 0
            return true;
        }
    }else{
        return 1;
    }
}

bool TRegEIC::IsRegReadEnable (SideBand sideband) const
{
    if ((m_core_version >= CORE_VER_20) && (CIntc1::IsIntc1Channel(m_channel))){
#if (_INTC2_UNIT_TEST)
    if (1){   //IHVE = 1
#else
    if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
        if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
        {
            return true;
        }else{
            if (sideband.GetUM() == 0){
            #if (_INTC2_UNIT_TEST)
                TRegEIBD* eibd = m_mock_intc1->GetEIBDPtr(m_channel);
                if ((sideband.GetGPID() == eibd->GetGPID())
                        && (sideband.GetGM() == eibd->GetGM())){
            #else
                TRegEIBD* eibd = m_intc1->GetEIBDPtr(m_channel);
                if ((sideband.GetGPID() == eibd->GetGPID())
                        && (sideband.GetGM() == eibd->GetGM())){
            #endif
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }
    }else{   // IHVE = 0
        return true;
    }
}else{
    return 1;
}
}

SysErrCause TRegEIC::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
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
 * @brief Writes to EICn.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @return SysErrCause.
 */
SysErrCause TRegEIC::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{

    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            return SYSERR_NONE;
        }
    TRegEEIC* eeic_ptr;
    if (CIntc1::IsIntc1Channel (m_channel) == true) {
#if (_INTC2_UNIT_TEST)
         eeic_ptr = m_mock_intc1->GetEEICPtr(m_channel);
#else
         eeic_ptr = m_intc1->GetEEICPtr(m_channel);
#endif
    }else{
         eeic_ptr = m_intc2->GetEEICPtr(m_channel);
    }

    if (size == SIZE_16BIT) {
        uint16_t wdata = *(reinterpret_cast<uint16_t*> (data));
        eeic_ptr->SetEICT ((wdata >> 15) & 0x01U);
        SetEICT ((wdata >> 15) & 0x01U);

        if (m_core_version >= CORE_VER_20) {
            eeic_ptr->SetEIOV ((wdata >> 5) & 0x01U);
            SetEIOV ((wdata >> 5) & 0x01U);
        }
        eeic_ptr->SetEIMK ((wdata >> 7) & 0x01U);
        SetEIMK ((wdata >> 7) & 0x01U);

        eeic_ptr->SetEIRF ((wdata >> 12) & 0x01U);
        SetEIRF ((wdata >> 12) & 0x01U);

        eeic_ptr->SetEITB ((wdata >> 6) & 0x01U);
        SetEITB ((wdata >> 6) & 0x01U);

        uint32_t eip_l = (wdata >> 0) & 0x0fU;
        uint32_t eip_h = (eeic_ptr->GetEIP() & 0x30);

        //Because EIC can't update EIP7-4, checking only EIC.EIP3-0 is enough
        if (GetEIP() != ((wdata >> 0) & 0xFU)) {
            eeic_ptr->SetEIP((eip_l + eip_h) & 0xFFFFU);
            SetEIP  ((wdata >> 0) & 0x0fU);
        }

    } else if (size == SIZE_8BIT) {
        uint8_t wdata = *data;
        if ((addr & 0x1) == 1) {
            eeic_ptr->SetEICT ((wdata >> (15-8)) & 0x01U);
            SetEICT ((wdata >> (15-8)) & 0x01U);

            eeic_ptr->SetEIRF ((wdata >> (12-8)) & 0x01U);
            SetEIRF ((wdata >> (12-8)) & 0x01U);

        } else {
            if ((m_core_version >= CORE_VER_20)) {
                eeic_ptr->SetEIOV ((wdata >> 5) & 0x01U);
                SetEIOV ((wdata >> 5) & 0x01U);
            }
            eeic_ptr->SetEIMK ((wdata >> 7) & 0x01U);
            SetEIMK ((wdata >> 7) & 0x01U);
            eeic_ptr->SetEITB ((wdata >> 6) & 0x01U);
            SetEITB ((wdata >> 6) & 0x01U);

            uint32_t eip_l = ((wdata >> 0) & 0x0fU);
            uint32_t eip_h = (eeic_ptr->GetEIP() & 0x30);

            //Because EIC can't update EIP7-4, checking only EIC.EIP3-0 is enough
            if (GetEIP() != ((wdata >> 0) & 0xFU)) {
                eeic_ptr->SetEIP((eip_l + eip_h) & 0xFFFFU);
                SetEIP  ((wdata >> 0) & 0x0fU);
            }
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Writes to EICn.
 * @param sideband Sideband.
 * @param addr Address.
 * @param data Data.
 * @param size Size.
 * @return SysErrCause.
 */
SysErrCause TRegEIC::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{

    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_GBUS;
    }

    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }

    uint8_t wdata[2];
    memcpy (wdata, data, sizeof(wdata));
    if (size == SIZE_16BIT) {
        ReplaceEICWriteData (wdata, 1);
        if (m_core_version >= CORE_VER_20){
            ReplaceEIOVWriteData (wdata, 0);
        }
    } else if ((size == SIZE_8BIT) && ((addr & 0x1) == 0x1)) {
        ReplaceEICWriteData (wdata, 0);
    } else if ((size == SIZE_8BIT) && ((addr & 0x1) == 0x0)) {
        if (m_core_version >= CORE_VER_20){
            ReplaceEIOVWriteData (wdata, 0);
        }
    }

    return WriteBody (addr, wdata, size);
}


/*!
 * @brief Replaces the input data to the value before over writing.
 * @param data Input data.
 * @param index Index for the array.
 */
void TRegEIC::ReplaceEICWriteData (uint8_t* data, uint32_t index)
{
    //EICTはReadOnlyビットのため、ソフトウェアによる変更は不可
    if (GetEICT() == 0x1) {
        data[index] |= EICT_BYTE_BIT;
        //EICTが1（レベル検出）の場合、EIRFビットはReadOnlyビットのため
        //ソフトウェアによる変更は不可
        if (GetEIRF() == 0x1) {
            data[index] |= EIRF_BYTE_BIT;
        } else {
            data[index] &= EIRF_BYTE_MASK;
        }
    } else {
        data[index] &= EICT_BYTE_MASK;
    }
}


void TRegEIC::ReplaceEIOVWriteData (uint8_t* data, uint32_t index)
{
    if (GetEICT() == 0x1) {
        data[index] &= EICT_EIOV_MASK;
    }
}

/*!
 * @brief Updates the information of the highest priority interrupt.
 * @param -
 */
void TRegEIC::UpdateEICHighPriority (void)
{
    if (CIntc1::IsIntc1Channel (m_channel) == true) {
        ContextId tcid = GetTcId ();
#if defined(_INTC2_UNIT_TEST)
        m_mock_intc1->UpdateHighPriority (tcid);
        m_mock_intc1->UpdateInterruptPort (m_channel);
#else
        m_intc1->UpdateHighPriority (tcid);
        m_intc1->UpdateInterruptPort (m_channel);
#endif
    } else {
        if (m_intc2->IsIntReqInQueue (m_channel) == true) {
            //割込みがINTC2のキューに登録されている場合
            m_intc2->UpdateHighPriority (m_bind_peid);
        } else if (m_intc2->IsIntReqInQueueOfIntc1 (m_bind_peid, m_channel) == true) {
            //割込みがINTC1のキューに登録されている場合
            m_intc2->UpdateHighPriorityOfIntc1 (m_bind_peid, m_channel);
        }
    }
}


/*!
 * @brief Constructs TRegEIC register.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param channel Channel number.
 * @param peid Identification of PE.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegEIC::TRegEIC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                  uint32_t channel, uint32_t peid, CModuleVc* tgtvc)
        : TRegCNTBase (reg_size, min_access_size, reg_addr, TRegMask(EIC_MASK_G4MH20)),
          m_channel (channel), m_bind_cst (0), m_bind_peid (peid), m_init_peid (peid)
{
    m_treg = &m_data;
    if (CIntc1::IsIntc1Channel (m_channel) == true) {
#if defined (_INTC2_UNIT_TEST)
        m_mock_intc1 = static_cast<MockCIntc1*> (tgtvc);
#else
        m_intc1 = static_cast<CIntc1*> (tgtvc);
        m_core_version = m_intc1->GetCoreVersion();
#endif
    } else {
        m_intc2 = static_cast<CIntc2*> (tgtvc);
        m_core_version = m_intc2->GetCoreVersion();
    }
}


/*!
 * @brief Writes to EICn.EIRF bit.
 * @param eirf Data to write.
 */
void TRegEIC::SetEIRF (RegData eirf)
{
    bool m_external_req = false;
    if (eirf == m_bit.m_eirf){
        if (m_core_version == CORE_VER_10){
            return; //値の変化がなければ何もしない
        }else{
#if !defined (_INTC2_UNIT_TEST)
            if (CIntc1::IsIntc1Channel(m_channel)){
                m_external_req = m_intc1->GetExternalEIINTreq();
            }else{
                m_external_req = m_intc2->GetExternalEIINTreq();
            }
#endif
            if  ((m_bit.m_eirf == 1) && (m_external_req == true)){
#if defined (_INTC2_UNIT_TEST)
            m_mock_intc1->UpdateOverwriteBit(m_channel);
            m_mock_intc1->SetExternalEIINTreq(false);
            return;
#else
            if (CIntc1::IsIntc1Channel(m_channel)){
                m_intc1->UpdateOverwriteBit_EIC(m_channel);
                m_intc1->SetExternalEIINTreq(false);
            }else{
                m_intc2->UpdateOverwriteBit_EIC(m_channel);
                m_intc2->SetExternalEIINTreq(false);
            }
            return;
#endif
            }else{
                return;
            }
        }
    }


    m_bit.m_eirf = eirf & 0x01U;
    if (CIntc1::IsIntc1Channel (m_channel) == true) {
#if defined (_INTC2_UNIT_TEST)
        if (m_bit.m_eirf == 1) {
            m_mock_intc1->ReqIntByEIC (m_channel);
        } else {
            m_mock_intc1->CancelInt (m_channel);
        }
#else
        if (m_bit.m_eirf == 1) {
            m_intc1->ReqIntByEIC (m_channel);
        } else {
            m_intc1->CancelInt (m_channel);
        }
#endif
    } else {
        if (m_bit.m_eirf == 1) {
            m_intc2->ReqIntByEIC (m_channel);
        } else {
            m_intc2->CancelInt (m_channel);
        }
    }
}


/*!
 * @brief Writes to EICn.EIMK bit.
 * @param eimk Data to write.
 */
void TRegEIC::SetEIMK (RegData eimk)
{
    if (eimk == m_bit.m_eimk) return; //値の変化がなければ何もしない

    m_bit.m_eimk = eimk & 0x01U;
    if (CIntc1::IsIntc1Channel (m_channel) == true) {
#if defined (_INTC2_UNIT_TEST)
        if (m_bit.m_eimk == 1) {
            //割込みをマスク
            m_mock_intc1->CancelIntByMask (m_channel);
        }
#else
        if (m_bit.m_eimk == 1) {
            //割込みをマスク
            m_intc1->CancelIntByMask (m_channel);
        }
#endif
        UpdateEICHighPriority ();
    } else {
        if (m_bind_cst == 0) {
            //通常割込み
            if (m_bit.m_eimk == 1) {
                //割込みをマスク
                m_intc2->CancelIntByMask (m_channel);
            }
            UpdateEICHighPriority ();
        } else {
            //同報通知
            if (m_bit.m_eimk == 0) {
                //マスクが解除されたため、同報通知情報をバッファに登録
                m_intc2->AddBcasIntInfo (m_channel);
            } else {
                //マスクされたため、同報通知情報をバッファから削除
                m_intc2->DelBcasIntInfo (m_channel);
            }
            //EIPが更新されても、INTC2のEIC.EIPは意味をもたないので
            //優先度判定(UpdateEICHighPriority)は行わない
        }
    }
}


void TRegEIC::SetEIP (RegData eip)
{
     // SetEIP is called when EIP is changed from current value.
     // It is checked on WriteBody.

    m_bit.m_eip = (eip & 0x0fU);
    UpdateEICHighPriority ();
}

//---------------------------EEIC----------------------------------------------

bool TRegEEIC::IsRegWriteEnable (SideBand sideband) const
{
    if ((m_core_version >= CORE_VER_20) && (CIntc1::IsIntc1Channel(m_channel))){
#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
            {
                return true;
            }else{
                if (sideband.GetUM() == 0){
                #if (_INTC2_UNIT_TEST)
                    TRegEIBD* eibd = m_mock_intc1->GetEIBDPtr(m_channel);
                    if ((sideband.GetGPID() == eibd->GetGPID())
                            && (sideband.GetGM() == eibd->GetGM())){
                #else
                    TRegEIBD* eibd = m_intc1->GetEIBDPtr(m_channel);
                    if ((sideband.GetGPID() == eibd->GetGPID())
                            && (sideband.GetGM() == eibd->GetGM())){
                #endif
                        return true;
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }
        }else{   // IHVE = 0
            return true;
        }
    }else{
        return true;
    }
}

/*!
 * @brief Writes to EEICn.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SysErrCause.
 */
SysErrCause TRegEEIC::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            return SYSERR_NONE;
     }
     TRegEIC* eic_ptr;
     if (CIntc1::IsIntc1Channel (m_channel) == true) {
#if (_INTC2_UNIT_TEST)
         eic_ptr = m_mock_intc1->GetEICPtr(m_channel);
#else
            eic_ptr = m_intc1->GetEICPtr(m_channel);
#endif
     }else{

         eic_ptr = m_intc2->GetEICPtr(m_channel);
     }

      if (size == SIZE_32BIT) {
          uint32_t wdata = *(reinterpret_cast<uint32_t*> (data));
          SetEICT ((wdata >> 31) & 0x01U);
          eic_ptr->SetEICT((wdata >> 31) & 0x01U);
          SetEIOV ((wdata >> 15) & 0x01U);
          eic_ptr->SetEIOV ((wdata >> 15) & 0x01U);
          SetEIMK ((wdata >> 23) & 0x01U);
          eic_ptr->SetEIMK((wdata >> 23) & 0x01U);
          SetEIRF ((wdata >> 28) & 0x01U);
          eic_ptr->SetEIRF((wdata >> 28) & 0x01U);
          SetEITB ((wdata >> 22) & 0x01U);
          eic_ptr->SetEITB ((wdata >> 22) & 0x01U);

          //SetEIP is called when EIP is changed from current value.
          if (GetEIP() != ((wdata >> 0) & 0x3FU)) {
              SetEIP ((wdata >> 0) & 0x3FU);
              eic_ptr->SetEIP ((wdata >> 0) & 0xFU);
          }


      } else if (size == SIZE_16BIT){
          uint16_t wdata = *(reinterpret_cast<uint16_t*> (data));
          if ((addr & 0x3) == 2) {    //high byte
              SetEICT ((wdata >> (31 - 16)) & 0x01U);
              eic_ptr->SetEICT ((wdata >> (31 - 16)) & 0x01U);
              SetEIMK ((wdata >> (23 - 16)) & 0x01U);
              eic_ptr->SetEIMK ((wdata >> (23 - 16)) & 0x01U);
              SetEIRF ((wdata >> (28 - 16)) & 0x01U);
              eic_ptr->SetEIRF ((wdata >> (28 - 16)) & 0x01U);
              SetEITB ((wdata >> (22 - 16)) & 0x01U);
              eic_ptr->SetEITB ((wdata >> (22 - 16)) & 0x01U);
          }else{    //low byte
              SetEIOV ((wdata >> 15) & 0x01U);
              eic_ptr->SetEIOV ((wdata >> 15) & 0x01U);

              //SetEIP is called when EIP is changed from current value.
              if (GetEIP() != ((wdata >> 0) & 0x3FU)) {
                  SetEIP ((wdata >> 0) & 0x3FU);
                  eic_ptr->SetEIP ((wdata >> 0) & 0xFU);
              }
          }
      } else if (size == SIZE_8BIT){
          uint8_t wdata = *data;
          if ((addr & 0x3) == 3) {    //byte 3
              SetEICT ((wdata >> (31 - 24)) & 0x01U);
              eic_ptr->SetEICT ((wdata >> (31 - 24)) & 0x01U);
              SetEIRF ((wdata >> (28 - 24)) & 0x01U);
              eic_ptr->SetEIRF ((wdata >> (28 - 24)) & 0x01U);
          }else if ((addr & 0x3) == 2) {    //byte 2
              SetEIMK ((wdata >> (23 - 16)) & 0x01U);
              eic_ptr->SetEIMK ((wdata >> (23 - 16)) & 0x01U);
              SetEITB ((wdata >> (22 - 16)) & 0x01U);
              eic_ptr->SetEITB ((wdata >> (22 - 16)) & 0x01U);
          }else if ((addr & 0x3) == 1) {    //byte 1
              SetEIOV ((wdata >> (15 - 8)) & 0x01U);
              eic_ptr->SetEIOV ((wdata >> (15 - 8)) & 0x01U);
          }else if ((addr & 0x3) == 0) {    //byte 0
              //SetEIP is called when EIP is changed from current value.
              if (GetEIP() != ((wdata >> 0) & 0x3FU)) {
                  SetEIP ((wdata >> 0) & 0x3FU);
                  eic_ptr->SetEIP ((wdata >> 0) & 0xFU);
              }
          }
      }
    return SYSERR_NONE;
}


/*!
 * @brief Writes to EEICn.
 * @param sideband Sideband.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SysErrCause.
 */
SysErrCause TRegEEIC::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_GBUS;
    }

    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }
    uint8_t wdata[4];

    memcpy (wdata, data, sizeof(wdata));
    if (size == SIZE_32BIT) {
        ReplaceEICWriteData (wdata, 3);
        ReplaceEIOVWriteData (wdata, 1);
    }else if (size == SIZE_16BIT) {
        if (((addr & 0x3) == 2)){
            ReplaceEICWriteData (wdata, 1);
        }else{
            ReplaceEIOVWriteData (wdata, 1);
        }
    }else if (size == SIZE_8BIT) {
        if (((addr & 0x3) == 3)){
              ReplaceEICWriteData (wdata, 0);
        }else if (((addr & 0x3) == 1)){
            ReplaceEIOVWriteData (wdata, 0);
        }
    }
    return WriteBody (addr, wdata, size);
}

bool TRegEEIC::IsRegReadEnable (SideBand sideband) const
{
    if ((m_core_version >= CORE_VER_20) && (CIntc1::IsIntc1Channel(m_channel))){
#if (_INTC2_UNIT_TEST)
    if (1){   //IHVE = 1
#else
    if (m_intc1->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
        if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
        {
            return true;
        }else{
            if (sideband.GetUM() == 0){
            #if (_INTC2_UNIT_TEST)
                TRegEIBD* eibd = m_mock_intc1->GetEIBDPtr(m_channel);
                if ((sideband.GetGPID() == eibd->GetGPID())
                        && (sideband.GetGM() == eibd->GetGM())){
            #else
                TRegEIBD* eibd = m_intc1->GetEIBDPtr(m_channel);
                if ((sideband.GetGPID() == eibd->GetGPID())
                        && (sideband.GetGM() == eibd->GetGM())){
            #endif
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }
    }else{   // IHVE = 0
        return true;
    }
}else{
    return 1;
}
}

SysErrCause TRegEEIC::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
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
 * @brief Replaces the input data to the value before over writing.
 * @param data Input data.
 * @param index Index for the array.
 */
void TRegEEIC::ReplaceEICWriteData (uint8_t* data, uint32_t index)
{
    //EICTはReadOnlyビットのため、ソフトウェアによる変更は不可
    if (GetEICT() == 0x1) {
        data[index] |= EICT_BYTE_BIT;
        //EICTが1（レベル検出）の場合、EIRFビットはReadOnlyビットのため
        //ソフトウェアによる変更は不可
        if (GetEIRF() == 0x1) {
            data[index] |= EIRF_BYTE_BIT;
        } else {
            data[index] &= EIRF_BYTE_MASK;
        }
    } else {
        data[index] &= EICT_BYTE_MASK;
    }
}


void TRegEEIC::ReplaceEIOVWriteData (uint8_t* data, uint32_t index)
{
    if (GetEICT() == 0x1) {
        data[index] &= EICT_EIOV_MASK;
    }
}


/*!
 * @brief Writes to EEICn.EIP bit.
 * @param eip Data to write.
 */
void TRegEEIC::SetEIP (RegData eip)
{
    // SetEIP is called when EIP is changed from current value.
    // It is checked on WriteBody.

    m_bit.m_eip = eip & 0x3fU;
    return;
}

/*!
 * @brief Constructs TRegEEIC register.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param channel Channel number.
 * @param peid Identification of PE.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegEEIC::TRegEEIC (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                  uint32_t channel, uint32_t peid, CModuleVc* tgtvc)
        : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr, TRegMask(EEIC_MASK)),
          m_channel (channel), m_bind_cst (0), m_bind_peid (peid), m_init_peid (peid), m_bind_et (0), m_bind_tcid (0)
{
    m_treg = &m_data;
    if (CIntc1::IsIntc1Channel (m_channel) == true) {
#if defined (_INTC2_UNIT_TEST)
        m_mock_intc1 = static_cast<MockCIntc1*> (tgtvc);
#else
        m_intc1 = static_cast<CIntc1*> (tgtvc);
        m_core_version = m_intc1->GetCoreVersion();
#endif
    } else {
        m_intc2 = static_cast<CIntc2*> (tgtvc);
        m_core_version = m_intc2->GetCoreVersion();
    }
}


/*!
 * @brief Writes to EEICn.EIRF bit.
 * @param eirf Data to write.
 */
void TRegEEIC::SetEIRF (RegData eirf)
{
    if (eirf == m_bit.m_eirf) return; //値の変化がなければ何もしない
    m_bit.m_eirf = eirf & 0x01U;
    return;
}


/*!
 * @brief Writes to EEICn.EIMK bit.
 * @param eimk Data to write.
 */
void TRegEEIC::SetEIMK (RegData eimk)
{
#if defined (_INTC2_UNIT_TEST)
#else
    if (eimk == m_bit.m_eimk) return; //値の変化がなければ何もしない
#endif
    m_bit.m_eimk = eimk & 0x01U;
    return;
}


//----------------------end EEIC-----------------------------------

/*!
 * @brief Constructs TRegEIBG.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param reg_mask Mask of the register.
 * @param regid Identification of the register.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegEIBG::TRegEIBG (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,TRegMask reg_mask, uint32_t regid, CModuleVc* tgtvc)
        : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr, reg_mask), m_regid (regid)
{
    m_treg = &m_data;

#if defined(_INTC2_UNIT_TEST)
    if (m_regid < MockTRegIntc1::N_G4MH20_EIBG) {
        m_intc = static_cast<MockCIntc1*> (tgtvc);
#else
    if (m_regid == TRegIntc2::N_G4MH20_EIBG) {
        m_intc = static_cast<CIntc1*> (tgtvc);
#endif
    } else {
        m_intc = static_cast<CIntc2*> (tgtvc);
    }
}

/*!
 * @brief Write to EIBG.(from debugger)
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegEIBG::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
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
        SetBGPR(wdata & 0x3FU);
    }else if ((size == SIZE_16BIT) && ((addr & 0x1U) == 0)){  //16 low bits
        SetBGPR(wdata & 0x3FU);
    }else if ((size == SIZE_8BIT)&& ((addr & 0x3U) == 0)){ //8 low bits
        SetBGPR(wdata & 0x3FU);
    }
    return SYSERR_NONE;
}

/*!
 * @brief Writes to EIBG.
 * @param sideband Sideband info.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SYSERR_NONE.
 */
SysErrCause TRegEIBG::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
         if (m_intc->GetIHVEFromIHVCFG() == 0){
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
 * @brief Checks the write enable of EIBG.
 * @param sideband Sideband info.
 * @return GM value in sideband.
 */
bool TRegEIBG::IsRegWriteEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20) {
        if (m_regid == TRegIntc2::N_G4MH20_EIBG){//INTC1

#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0)){
                return true;
            }else{
                 return false;
            }
        }else{
            return false;
        }
    }else{// INTC2
        return true;
    }
    }else{//VER_10
        return false;
    }
}

bool TRegEIBG::IsRegReadEnable (SideBand sideband) const
{
   if (m_intc->GetCoreVersion() >= CORE_VER_20 ){
       if (m_regid == TRegIntc2::N_G4MH20_EIBG){//INTC1

#if (_INTC2_UNIT_TEST)
       if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0)&& (sideband.GetUM() == 0)){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{//INTC2
        return true;
    }
    }else{//VER_10
        return false;
    }
}

SysErrCause TRegEIBG::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (IsRegReadEnable (sideband) == false) {
        if (m_intc->GetIHVEFromIHVCFG() == 0){
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

/*!
 * @brief Writes to IMRn bit.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SysErrCause.
 */
SysErrCause TRegIMR::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }

    uint32_t eic_channel = (m_regid<<5) + static_cast<uint32_t> ((addr & 0x3) << 3);
    uint32_t data32 = 0;
    for (ByteSize i = 0; i < size; i++) {
        data32 |= (data[i] & 0xffU) << (i << 3);
    }

#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
    uint32_t peid = 0;
#else
    uint32_t peid = p_forest_util->GetPeidOfDbgAPI ();
#endif
    for (uint32_t i = 0; i < static_cast<uint32_t>(size*8); i++) {
        TRegEIC* eic = m_intc->GetEICPtr (eic_channel+i);
        TRegEEIC* eeic = m_intc->GetEEICPtr (eic_channel+i);

#if defined(_INTC2_UNIT_TEST)
        if ((eic_channel+i) < MockTRegIntc1::N_EIC) {
#else
        if ((eic_channel+i) < TRegIntc1::N_EIC) {
#endif
            //in DB, so GM = 0
                eeic->SetEIMK((data32>>i) & 0x1U);
                eic->SetEIMK((data32>>i) & 0x1U);
        }else{
            TRegEIBD* eibd = m_intc->GetEIBDPtr (eic_channel+i);
            if (peid == eibd->GetPEID()){
                if (m_intc->GetCoreVersion() >= CORE_VER_20){
                    //in DB, so GM = 0
                        eeic->SetEIMK((data32>>i) & 0x1U);
                        eic->SetEIMK((data32>>i) & 0x1U);
                }else{
                    eeic->SetEIMK((data32>>i) & 0x1U);
                    eic->SetEIMK((data32>>i) & 0x1U);
                }
            }else{
                CfMsg::DPrint (MSG_WAR,
                                "--:--:--:-: <Warning: Intc2 IMR bit %d  write ignored, write protection by PEID>\n", i);
            }
        }
    }
    return SYSERR_NONE;
}


/*!
 * @brief Writes to IMRn.
 * @param sideband Sideband.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SysErrCause.
 */
SysErrCause TRegIMR::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
        return SYSERR_GBUS;
    }

    if (IsRegAccessSizeValid (size) == false) {
#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
    throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }

    uint32_t eic_channel = (m_regid<<5) + static_cast<uint32_t> ((addr & 0x3) << 3);
    uint32_t data32 = 0;
    for (ByteSize i = 0; i < size; i++) {
        data32 |= (data[i] & 0xffU) << (i << 3);
    }

#if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
    uint32_t peid = 0;
#else
    uint32_t peid = p_forest_util->GetPeidFromBmid (sideband.GetBMID ());
#endif
    for (uint32_t i = 0; i < static_cast<uint32_t>(size*8); i++) {
        TRegEIC* eic = m_intc->GetEICPtr (eic_channel+i);
        TRegEEIC* eeic = m_intc->GetEEICPtr (eic_channel+i);

#if defined(_INTC2_UNIT_TEST)
        if ((eic_channel+i) < MockTRegIntc1::N_EIC) {
#else
        if ((eic_channel+i) < TRegIntc1::N_EIC) {
#endif
            if (sideband.GetGM() == 0){
                eeic->SetEIMK((data32>>i) & 0x1U);
                eic->SetEIMK((data32>>i) & 0x1U);
            }else {
                TRegEIBD* eibd = m_intc->GetEIBDPtr(eic_channel+i);
                if ((sideband.GetGPID() == eibd->GetGPID()) && (sideband.GetGM() == eibd->GetGM())){
                    eeic->SetEIMK((data32>>i) & 0x1U);
                    eic->SetEIMK((data32>>i) & 0x1U);

                }
            }
        }else{
            TRegEIBD* eibd = m_intc->GetEIBDPtr (eic_channel+i);
            if (peid == eibd->GetPEID()){
                if (m_intc->GetCoreVersion() >= CORE_VER_20){
                    if (sideband.GetGM() == 0){ //CPU Host
                        eeic->SetEIMK((data32>>i) & 0x1U);
                        eic->SetEIMK((data32>>i) & 0x1U);

                    }else{ //CPU Guest
                        if ((eibd->GetGM() == 1) && (eibd->GetGPID() == sideband.GetGPID())){
                            eeic->SetEIMK((data32>>i) & 0x1U);
                            eic->SetEIMK((data32>>i) & 0x1U);

                        }else{
                            CfMsg::DPrint (MSG_WAR,
                                            "--:--:--:-: <Warning: Intc2 IMR bit %d write ignored, write protection by not match GM/GPID>\n",i);
                        }
                    }
                }else{
                    eeic->SetEIMK((data32>>i) & 0x1U);
                    eic->SetEIMK((data32>>i) & 0x1U);

                }
            }else{
                CfMsg::DPrint (MSG_WAR,
                                "--:--:--:-: <Warning: Intc2 IMR bit %d  write ignored, write protection by PEID>\n", i);
            }
        }
    }
    return SYSERR_NONE;
}

bool TRegIMR::IsRegWriteEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20){
        if (m_regid < TRegIntc1::N_IMR){//INTC1

#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if (sideband.GetUM() == 0)
            {
                //generate SYSERR exception here
                return true;
            }else{
                 return false;
            }
        }else{
            return true;
        }
        }else{//INTC2
            return true;
        }
    }else{//VER_10
        return true;
    }
}

bool TRegIMR::IsRegReadEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20){
        if (m_regid < TRegIntc1::N_IMR){//INTC1

#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if (sideband.GetUM() == 0)
            {
                 return true;
            }else{
                 return false;
            }
        }else{
            return true;
        }
        }else{//INTC2
            return true;
        }
    }else{//VER_10
        return true;
    }
}

SysErrCause TRegIMR::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
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
 * @brief Reads from IMRn.
 * @param addr Address.
 * @param data Data.
 * @param size Acess size.
 * @return SysErrCause.
 */
SysErrCause TRegIMR::ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const
{
    uint32_t eic_channel = (m_regid<<5) + static_cast<uint32_t> ((addr & 0x3) << 3);
    uint32_t data32 = 0;
    for (int32_t i = 0; i < (size*8); i++) {
        TRegEIC* eic = m_intc->GetEICPtr (eic_channel+i);
        data32 |= static_cast<uint32_t> ((eic->GetEIMK()<<i));
    }
    for (ByteSize i = 0; i < size; i++) {
        data[i] = (data32 >> (i << 3)) & 0xffU;
    }
    return SYSERR_NONE;
}

/*!
 * @brief Constructs TRegIMR register.
 * @param reg_size The size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param regid Identification of the register.
 * @param tgtvc The pointer to the target module of the basic class.
 */
TRegIMR::TRegIMR (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                  uint32_t regid, CModuleVc* tgtvc)
        : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr, TRegMask(IMR_MASK)),
          m_regid (regid)
{
#if defined(_INTC2_UNIT_TEST)
    if (m_regid < MockTRegIntc1::N_IMR) {
        m_intc = static_cast<MockCIntc1*> (tgtvc);
#else
    if (m_regid < TRegIntc1::N_IMR) {
        m_intc = static_cast<CIntc1*> (tgtvc);
#endif
    } else {
        m_intc = static_cast<CIntc2*> (tgtvc);
    }
}

/*!
 * @brief Gets mask for EIINT.
 * @param -
 * @return Mask value.
 */
RegData EiintReqInfo::GetMask (void) const
{
#if defined(_INTC2_UNIT_TEST)
    return (m_eic->GetEIMK () | m_mock_dbmk->GetEI());
#else
    return (m_eic->GetEIMK () | m_dbmk->GetEI());
#endif
}

/*!
 * @brief Gets BGPR for EIINT.
 * @param -
 * @return BGPR value.
 */

RegData EiintReqInfo::GetBGPR (void) const
{
//#if defined(_INTC2_UNIT_TEST)
//    return m_mock_eibg->GetBGPR();
//#else
    return m_eibg->GetBGPR();
//#endif
}

/*!
 * @brief Resets EIBD register.
 * @param -
 */
template <>
void TRegBND<EIBD>::Reset (void)
{
    m_tr.SetPEID (m_init_peid);
    m_tr.SetBCP (0);
    m_tr.SetCST (0);
    m_tr.SetGM(0);
    m_tr.SetGPID(0);
}


/*!
 * @brief Writes BND byte.
 * @param pos Byte position.
 * @param data Data.
 */
template <class TR>
void TRegBND<TR>::WriteBodyByte (uint32_t pos, uint8_t data)
{
    switch (pos) {
    case 1:
        break;
    default: break;
    }
}


/*!
 * @brief Writes EIBD byte.
 * @param pos Byte position.
 * @param data Data.
 */
template <>
void TRegBND<EIBD>::WriteBodyByte (uint32_t pos, uint8_t data)
{
    switch (pos) {
    case 3:
        if (IsIntc2EIBD() == true) {
            if (!IsIntLevelDetect(m_channel)){
                SetCST ((data >> 7) & 0x01U);
            }
            SetBCP (data & 0x03U);
        }
        break;
    case 2:
        break;
    case 1:
        if (m_core_version >= CORE_VER_20){
            if ((IsIntc2EIBD() != true) && (m_intc->GetIHVEFromIHVCFG() == 1)){ //INTC1
                SetGM ((data >> 7) & 0x01U);
                SetGPID (data & 0x07U);
            }else if (IsIntc2EIBD() == true){//INTC2
                SetGM ((data >> 7) & 0x01U);
                SetGPID (data & 0x07U);
            }
        }
        break;
    case 0:
        if (IsIntc2EIBD() == true) {
            SetPEID (data & PEID_MASK);
        }
        break;
    default: break;
    }
}

template <>
void TRegBND<FIBD>::WriteBodyByte (uint32_t pos, uint8_t data)
{
    switch (pos) {
    case 3:
        break;
    case 2:
        break;
    case 1:
        if (m_core_version >= CORE_VER_20){
            if ((IsIntc2EIBD() != true) && (m_intc->GetIHVEFromIHVCFG() == 1)){
                SetGM ((data >> 7) & 0x01U);
                SetGPID (data & 0x07U);
            }
        }
        break;
    case 0:
        break;
    default: break;
    }
}


/*!
 * @brief Writes EIBD,FIBD.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SysErrCause.
 */
template <class TR>
SysErrCause TRegBND<TR>::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            return SYSERR_NONE;
        }
    if (size == SIZE_32BIT) {
        WriteBodyByte (3, data[3]);
        WriteBodyByte (2, data[2]);
        WriteBodyByte (1, data[1]);
        WriteBodyByte (0, data[0]);

    } else if (size == SIZE_16BIT) {
        switch (addr & 0x2) {
        case 2:
            WriteBodyByte (3, data[1]);
            WriteBodyByte (2, data[0]);
            break;
        case 0:
            WriteBodyByte (1, data[1]);
            WriteBodyByte (0, data[0]);
            break;
        default: break;
        }

    } else if (size == SIZE_8BIT) {
        switch (addr & 0x3) {
        case 3: WriteBodyByte (3, data[0]); break;
        case 2: WriteBodyByte (2, data[0]); break;
        case 1: WriteBodyByte (1, data[0]); break;
        case 0: WriteBodyByte (0, data[0]); break;
        default: break;
        }
    }

#if defined(_INTC2_UNIT_TEST)
#else
    SetBind ();
#endif

    return SYSERR_NONE;
}

/*!
 * @brief Writes FNBD.
 * @param addr Address.
 * @param data Data.
 * @param size Access size.
 * @return SysErrCause.
 */
template <>
#if defined(_INTC2_UNIT_TEST)
SysErrCause TRegBND<MockFNBD>::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
#else
SysErrCause TRegBND<FNBD>::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
#endif
    //FNBD.ET,TCIDビットは0固定,Read-Only
    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            return SYSERR_NONE;
    }
    return SYSERR_NONE;
}


/*!
 * @brief Checks write enable of FIBD register.
 * @param sideband Sideband.
 * @return bool.
 */
template <class TR>
bool TRegBND<TR>::IsRegWriteEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20){
        if (IsIntc2EIBD() != true){//INTC1

#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
            {
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
        }else{//INTC2
            return true;
        }
    }else{//VER_10
        return true;
    }
}

template <class TR>
bool TRegBND<TR>::IsRegReadEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20){
        if (IsIntc2EIBD() != true){

#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
            {
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
        }else{
            return true;
        }
    }else{//VER_10
        return true;
    }
}

template <>
bool TRegBND<EIBD>::IsRegReadEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20){
        if (IsIntc2EIBD() != true){//INTC1

#if (_INTC2_UNIT_TEST)
        if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
            {
                 return true;
            }else{
                 return false;
            }
        }else{
            return true;
        }
        }else{//INTC2
            return true;
        }
    }else{//VER_10
        return true;
    }
}

template <>
bool TRegBND<FIBD>::IsRegReadEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20){
#if (_INTC2_UNIT_TEST)
       if (1){   //IHVE = 1
#else
        if (m_intc->GetIHVEFromIHVCFG() == 1){   //IHVE = 1
#endif
            if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
            {
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }else{
        return true;
    }
}

template <>
bool TRegBND<FNBD>::IsRegWriteEnable (SideBand sideband) const
{
    if (m_intc->GetCoreVersion() >= CORE_VER_20){
        if ((sideband.GetGM() == 0) && (sideband.GetUM() == 0) )
        {
            return true;
        }else{
            return false;
        }
    }else{
        return 1;
    }

}

/*!
 * @brief Checks if this is EIBDn of INTC1.
 * @param -
 * @return true: This is EIBD of INTC1, false: This is not EIBDn of INTC1.
 */
template <class TR>
bool TRegBND<TR>::IsIntc1EIBD (void) const
{
#if defined(_INTC2_UNIT_TEST)
    return m_channel >= 0 && m_channel < MockTRegIntc1::N_EIBD;
#else
    return m_channel >= 0 && m_channel < TRegIntc1::N_EIBD;
#endif
}


/*!
 * @brief Checks if this is EIBDn of INTC2.
 * @param -
 * @return true: This is EIBD of INTC2, false: This is not EIBDn of INTC2.
 */
template <class TR>
bool TRegBND<TR>::IsIntc2EIBD (void) const
{
#if defined(_INTC2_UNIT_TEST)
    return m_channel >= MockTRegIntc1::N_EIBD && m_channel < (TRegIntc2::N_G4MH_EIBD);
#else
    return m_channel >= TRegIntc1::N_EIBD && m_channel < (TRegIntc2::N_G4MH_EIBD);
#endif
}

template <class TR>
bool TRegBND<TR>::IsIntLevelDetect (uint32_t channel)
{
    return ((m_intc->GetEICPtr(channel)->GetEICT() == 1) ? true : false);
}

/*!
 * @brief Sets EIBDn.ET,TCID,PEID bit for EIC register class.
 * @param -
 */
template <>
void TRegBND<EIBD>::SetBind (void)
{
    //EIBDのチャネル番号からEICレジスタクラスのポインタを取得
    TRegEIC* eic = m_intc->GetEICPtr (m_channel);
    //EIBDn.ET,CST,TCID,PEIDビットをEICレジスタクラスに設定
    eic->SetBindCST (GetCST());
    eic->SetBindPEID (GetPEID());
}

template <class TR>
SysErrCause TRegBND<TR>::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegWriteEnable (sideband) == false) {
       return SYSERR_GBUS;
    }
    if (IsRegAccessSizeValid (size) == false) {
#if (_INTC2_UNIT_TEST) || (_INTC1_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        std::fill_n (data, size, 0);
        return SYSERR_NONE;
    }

    return WriteBody(addr, data, size);
}

template <class TR>
SysErrCause TRegBND<TR>::Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const
{
    if (IsRegReadEnable (sideband) == false) {
       std::fill_n (data, size, 0);
       return SYSERR_GBUS;
    }

    if (IsRegAccessSizeValid (size) == false) {
#if (_INTC2_UNIT_TEST) || (_INTC1_UNIT_TEST)
     throw std::range_error("Un-guaranteed Size Access");
#endif
        PrintAccessSizeWarning ();
        std::fill_n (data, size, 0);
        return SYSERR_NONE;
    }

    return ReadBody(addr, data, size);
}

/*!
 * @brief Writes to EIBD.
 * @param addr Address.
 * @param data Data.
 * @param size Data size.
 * @return SYSERR_NONE.
 */
template <>
SysErrCause TRegBND<EIBD>::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
    #if defined(_INTC1_UNIT_TEST) || defined(_INTC2_UNIT_TEST)
         throw std::range_error("Un-guaranteed Size Access");
    #endif
            PrintAccessSizeWarning ();
            return SYSERR_NONE;
        }

    // Get current EIBD status
    bool is_updated_int = false;
    if (IsIntc2EIBD() == true) {
        TRegEIBD* eibd = m_intc->GetEIBDPtr(m_channel);
        uint32_t pre_peid = eibd->GetPEID();
        uint32_t pre_cst  = eibd->GetCST();
        uint32_t pre_bcp  = eibd->GetBCP();
        bool     diff_eibd = false;

        uint32_t post_peid = pre_peid;
        uint32_t post_cst = pre_cst;
        uint32_t post_bcp = pre_bcp;
        if (size == SIZE_32BIT) {
            if (!IsIntLevelDetect(m_channel)) {
                post_cst = (data[3] >> 7) & 0x01U;
            }
            post_bcp = data[3] & 0x03U;
            post_peid = PEID_MASK & data[0];
        } else if (size == SIZE_16BIT) {
            switch (addr & 0x2) {
            case 2:
                if (!IsIntLevelDetect(m_channel)) {
                    post_cst = (data[1] >> 7) & 0x01U;
                }
                post_bcp = data[1] & 0x03U;
                break;
            case 0:
                post_peid = PEID_MASK & data[0];
                break;
            default: break;
            }
        } else if (size == SIZE_8BIT) {
            switch (addr & 0x3) {
            case 3:
                if (!IsIntLevelDetect(m_channel)) {
                    post_cst = (data[0] >> 7) & 0x01U;
                }
                post_bcp = data[0] & 0x03U;
                break;
            case 0:
                post_peid = PEID_MASK & data[0];
                break;
            default: break;
            }
        }

        if ((pre_peid != post_peid) || (pre_bcp != post_bcp) || (pre_cst != post_cst)) {
            diff_eibd = true;
        }

        // Check interruption is existed
        TRegEIC* eic = m_intc->GetEICPtr(m_channel);
        TRegEEIC* eeic = m_intc->GetEEICPtr(m_channel);
        #if defined (_INTC2_UNIT_TEST)
            if (diff_eibd ) {
           #else
               if ((eic->GetEIRF() == 1) && (diff_eibd) ) {
           #endif
                   eeic->SetEIRF(0); // to update PEID/CST, erase intreq at once.
                   eic->SetEIRF(0); // to update PEID/CST, erase intreq at once.
                   is_updated_int = true;
               }
           }

    if (size == SIZE_32BIT) {
        WriteBodyByte (3, data[3]);
        WriteBodyByte (2, data[2]);
        WriteBodyByte (1, data[1]);
        WriteBodyByte (0, data[0]);

    } else if (size == SIZE_16BIT) {
        switch (addr & 0x2) {
        case 2:
            WriteBodyByte (3, data[1]);
            WriteBodyByte (2, data[0]);
            break;
        case 0:
            WriteBodyByte (1, data[1]);
            WriteBodyByte (0, data[0]);
            break;
        default: break;
        }

    } else if (size == SIZE_8BIT) {
        switch (addr & 0x3) {
        case 3: WriteBodyByte (3, data[0]); break;
        case 2: WriteBodyByte (2, data[0]); break;
        case 1: WriteBodyByte (1, data[0]); break;
        case 0: WriteBodyByte (0, data[0]); break;
        default: break;
        }
    }

    SetBind ();

    if (is_updated_int) {
        TRegEEIC* eeic = m_intc->GetEEICPtr(m_channel);
        eeic->SetEIRF(1); // re-issue interrupt with update EIBD info.
        TRegEIC* eic = m_intc->GetEICPtr(m_channel);
        eic->SetEIRF(1); // re-issue interrupt with update EIBD info.
    }

    return SYSERR_NONE;
}

/*!
 * @brief Constructs TRegEIBD register.
 * @param reg_size Size of the register.
 * @param min_access_size Minimum access size.
 * @param reg_addr Address of the register.
 * @param reg_mask Mask of the register.
 * @param channel Channel number.
 * @param peid Identification of PE.
 * @param tgtvc The pointer to the target module of the basic class.
 * @param type INTC2 Implementation type.
 */
template <class TR>
TRegBND<TR>::TRegBND (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                      TRegMask reg_mask, uint32_t channel, uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type)
    : TRegIntcBase<TRegData32> (reg_size, min_access_size, reg_addr, reg_mask),
      m_channel (channel), m_init_peid (peid), m_type(type)
{
    m_treg = m_tr.GetTRegDataPtr();
    m_tr.SetData (peid);
    if (IsIntc1EIBD() == true) {
#if defined(_INTC2_UNIT_TEST)
        m_intc = static_cast<MockCIntc1*> (tgtvc);
#else
        m_intc = static_cast<CIntc1*> (tgtvc);
        m_core_version = m_intc->GetCoreVersion();
#endif
    } else {
        m_intc = static_cast<CIntc2*> (tgtvc);
        m_core_version = m_intc->GetCoreVersion();
    }
}

/*!
 * @brief Constructs BND register with 6 argument.
 * @param reg_size Size of the register.
 * @param min_access_size Min access size.
 * @param reg_addr Address of the register.
 * @param peid Identification of PE.
 * @param tgtvc The pointer to the target module of the basic class.
 * @param type INTC2 Implementation type.
 */
template <class TR>
TRegBND<TR>::TRegBND (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                      uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type)
                      : TRegIntcBase<TRegData32> (reg_size, min_access_size, reg_addr),
                      m_init_peid (peid), m_type(type)
{
    m_treg = m_tr.GetTRegDataPtr();
    m_tr.SetData (peid);
    if (IsIntc1EIBD() == true) {
#if defined(_INTC2_UNIT_TEST)
        m_intc = static_cast<MockCIntc1*> (tgtvc);
#else
        m_intc = static_cast<CIntc1*> (tgtvc);
        m_core_version = m_intc->GetCoreVersion();
#endif
    } else {
        m_intc = static_cast<CIntc2*> (tgtvc);
        m_core_version = m_intc->GetCoreVersion();
    }
}


template TRegBND<EIBD>::TRegBND (ByteSize reg_size, ByteSize min_access_size,
                                 PhysAddr reg_addr, TRegMask reg_mask,
                                 uint32_t channel, uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type);
#if defined(_INTC2_UNIT_TEST)
template TRegBND<MockFIBD>::TRegBND (ByteSize reg_size, ByteSize min_access_size,
                                 PhysAddr reg_addr, TRegMask reg_mask,
                                 uint32_t channel, uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type);
template void TRegBND<MockFNBD>::Reset (void);
template void TRegBND<MockFIBD>::Reset (void);
template void TRegBND<MockFIBD>::WriteBodyByte (uint32_t pos, uint8_t data);
template SysErrCause TRegBND<MockFIBD>::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
template bool TRegBND<MockFNBD>::IsRegWriteEnable (SideBand sideband) const;
template bool TRegBND<MockFIBD>::IsRegWriteEnable (SideBand sideband) const;
template bool TRegBND<EIBD>::IsIntLevelDetect(uint32_t channel); //explicit instantiation
template bool TRegBND<EIBD>::IsIntc1EIBD() const;                //explicit instantiation
template bool TRegBND<EIBD>::IsIntc2EIBD() const;                //explicit instantiation
#else
template TRegBND<FIBD>::TRegBND (ByteSize reg_size, ByteSize min_access_size,
                                 PhysAddr reg_addr, TRegMask reg_mask,
                                 uint32_t channel, uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type);
template TRegBND<FNBD>::TRegBND (ByteSize reg_size, ByteSize min_access_size,
                                 PhysAddr reg_addr, uint32_t peid, CModuleVc* tgtvc, Intc2ImplType type); //explicit instantiation for template class
template void TRegBND<FNBD>::Reset (void);
template void TRegBND<FIBD>::Reset (void);
template void TRegBND<FIBD>::WriteBodyByte (uint32_t pos, uint8_t data);
template SysErrCause TRegBND<FIBD>::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
template bool TRegBND<FNBD>::IsRegWriteEnable (SideBand sideband) const;
template bool TRegBND<FIBD>::IsRegWriteEnable (SideBand sideband) const;
#endif
