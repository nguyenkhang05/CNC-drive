/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include <vector>
#include "./config.h"
#include "./simio.h"
#include "./forest.h"
#include "./forest_utility.h"
#include "./forest_internals.h"
#include "./forest_message.h"
#include "./fs_controler.h"
#include "./proc_element.h"

const PhysAddr Csimio::SIM_IO_BASE = 0xFFFF7F00U;
const PhysAddr Csimio::SIM_IO_MASK = 0xFFU;
const PhysAddr Csimio::SIM_STOPIO  = 0xFEU;
const PhysAddr Csimio::ECM_IO_BASE = 0xF3000000U;
const PhysAddr Csimio::ECM_IO_MASK = 0xFFFFFFU;
const PhysAddr Csimio::LOG_IO_BASE = 0xF4000000U;
const PhysAddr Csimio::LOG_IO_MASK = 0xFFFFU;
const PhysAddr Csimio::G3K_SIM_IO_BASE = 0xFFFF8FFCU;
const PhysAddr Csimio::G3K_SIM_IO_MASK = 0x3U;

const PhysAddr Csimio::ECMCTR_ADDR    = Csimio::ECM_IO_BASE + 0x00;
const PhysAddr Csimio::ECMSTEPL_BASE  = Csimio::ECM_IO_BASE + 0x10;
const PhysAddr Csimio::ECMSTEPU_BASE  = Csimio::ECM_IO_BASE + 0x14;
const PhysAddr Csimio::ECMCYCLEL_BASE = Csimio::ECM_IO_BASE + 0x18;
const PhysAddr Csimio::ECMCYCLEU_BASE = Csimio::ECM_IO_BASE + 0x1C;

const PhysAddr Csimio::ECM_ADDR_MASK  = 0x00FF000000U;
const PhysAddr Csimio::ECM_CTR_MASK   = 0x00000000F0U;
const PhysAddr Csimio::ECM_CTR_VAL    = 0x0000000000U;
const PhysAddr Csimio::ECM_PEID_SHIFT = 16;
const PhysAddr Csimio::ECM_TCID_SHIFT = 8;
const PhysAddr Csimio::ECM_SYS_TCID   = 0x80;
const PhysAddr Csimio::ECM_PE_TCID    = 0xFE;

// ECMCTRリセット関数
void TRegECMCTR::Reset (void) {
    m_ecmstr.bit.m_hold = 0;
    m_csimio->m_is_ecm_hold = false;
}


// ECMCTR.HOLDライト関数
void TRegECMCTR::SetHOLD (RegData hold)
{
    if (hold == 0x1) {
        m_csimio->HoldRegData ();
        m_csimio->m_is_ecm_hold = true;
    } else {
        m_csimio->m_is_ecm_hold = false;
    }
    m_ecmstr.bit.m_hold = hold & 0x1;
}


// ECMCTRライト関数
SysErrCause TRegECMCTR::Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    } else {
        SetHOLD (data[0]);
    }
    return SYSERR_NONE;
}


// ECM制御用レジスタの生成関数
void TRegEcmCtr::InitTRegMap (void)
{
    // ECMCTRの生成
    static const PhysAddr ecmctr_addr = Csimio::ECMCTR_ADDR;
    TRegECMCTR* ecmstr = new TRegECMCTR (SIZE_32BIT, SIZE_32BIT, ecmctr_addr, m_csimio);
    m_trf.insert (TRegMap::value_type (ecmctr_addr, ecmstr));
}


TRegEcmCtr::~TRegEcmCtr ()
{
    TRegMap::iterator ite = m_trf.begin();
    while (ite != m_trf.end()) {
        delete ite->second;
        ++ite;
    }
}


// ECMカウンタ用レジスタのリード関数
SysErrCause EcmCountBase::Read (SideBand sideband, PhysAddr addr, uint8_t* data,
                                ByteSize size) const
{
    if (IsRegAccessSizeValid (size) == false) {
        PrintAccessSizeWarning ();
        return SYSERR_NONE;
    }

    uint64_t load_data;
    // ホールド中の判定
    if (m_csimio->m_is_ecm_hold) {  // ホールド中
        load_data = m_hold_data;
    } else {  // カウントUP中
        load_data = GetData ();
    }

    if ((addr & 0x4) != 0) {
        if (size == SIZE_32BIT) {
            load_data = (load_data >> 32) & 0x00FFFFFFFFULL;
        } else {
            PrintAccessSizeWarning ();
            return SYSERR_NONE;
        }
    }

    // HTステップ数の下位32bitを格納
    // ただし、アクセスサイズが8byteなら下位32bitだけでなく64bit全部を格納
    for (ByteSize i = 0; i < size; i++) {
        data[i] = static_cast<uint8_t>((load_data >> (i<<3)) & 0xffU);
    }
    return SYSERR_NONE;
}


// ECMSTEPL/H実測値リード関数
uint64_t TRegECMSTEP::GetData (void) const
{
    return m_pe->GetHtPtr()->m_trace->GetStepCount ();
}


// ECMCYCLEL/H実測値リード関数
uint64_t TRegECMCYCLE::GetData (void) const
{
    return m_pe->GetHtPtr()->m_trace->GetStageCycle (STAGE_DP);
}


// ECMSYSCYCLEL/H実測値リード関数
uint64_t TRegECMSYSCYCLE::GetData (void) const
{
    // システム全体のサイクル数の実測値取得用APIを登録する
    return p_internals->GetSystemTime ();
}


// ECMPECYCLEL/H実測値リード関数
uint64_t TRegECMPECYCLE::GetData (void) const
{
    // @@ PE毎のサイクル数の実測値取得用APIを登録する
    return 0;
}


// ECMカウンタ用レジスタの生成関数
void TRegEcmCnt::InitTRegMap (void)
{
    // システム全体用のレジスタの宣言
    // ECMSYSCYCLEL/Hの生成
    PhysAddr sys_offset = (Csimio::ECM_SYS_TCID << Csimio::ECM_TCID_SHIFT);
    TRegECMSYSCYCLE* syscycle = new TRegECMSYSCYCLE (SIZE_64BIT, SIZE_32BIT,
                                                     Csimio::ECMCYCLEL_BASE + sys_offset,
                                                     NULL, m_csimio,
                                                     Csimio::ECM_SYS_TCID);
    m_trf.insert (TRegMap::value_type (Csimio::ECMCYCLEL_BASE + sys_offset, syscycle));
    m_trf.insert (TRegMap::value_type (Csimio::ECMCYCLEU_BASE + sys_offset, syscycle));
    m_trf_del.insert (TRegMap::value_type (Csimio::ECMCYCLEU_BASE + sys_offset, syscycle));

    PeVec::const_iterator it = m_pes->begin ();
    while (it != m_pes->end ()) {
        if ((*it) == NULL) {
            ++ it;
            continue;
        }

        uint32_t peid = (*it)->GetPeId ();

        // ECMはPE1-255までをサポート
        if (peid > 255) {
            ++ it;
            continue;
        }

        // PE毎用のレジスタの宣言
        // ECMPECYCLEL/Hの生成
        PhysAddr pe_offset
            = (peid << Csimio::ECM_PEID_SHIFT) | (Csimio::ECM_PE_TCID << Csimio::ECM_TCID_SHIFT);
        TRegECMPECYCLE* pecycle = new TRegECMPECYCLE(SIZE_64BIT, SIZE_32BIT,
                                                     Csimio::ECMCYCLEL_BASE + pe_offset,
                                                     (*m_pes)[peid], m_csimio,
                                                     Csimio::ECM_PE_TCID);
        m_trf.insert (TRegMap::value_type (Csimio::ECMCYCLEL_BASE + pe_offset, pecycle));
        m_trf.insert (TRegMap::value_type (Csimio::ECMCYCLEU_BASE + pe_offset, pecycle));
        m_trf_del.insert (TRegMap::value_type (Csimio::ECMCYCLEU_BASE + pe_offset, pecycle));

        // TC数の読み込み
        ContextId tc_last_id = NC;

        // TC毎用のレジスタの宣言, ECMではTC0-63 と NCをサポート
        for (ContextId tcid = NC; tcid <= tc_last_id; tcid++) {
            uint32_t tcadr;
            if (tcid == NC) {
                tcadr = 0xFF;
            } else {
                tcadr = static_cast <uint32_t> (tcid);
            }

            PhysAddr tc_offset
                = (peid << Csimio::ECM_PEID_SHIFT) | (tcadr << Csimio::ECM_TCID_SHIFT);

            // ECMSTEPL/Hの生成
            TRegECMSTEP* ecmstep = new TRegECMSTEP(SIZE_64BIT, SIZE_32BIT,
                                                   Csimio::ECMSTEPL_BASE + tc_offset,
                                                   (*m_pes)[peid], m_csimio, tcid);
            m_trf.insert (TRegMap::value_type (Csimio::ECMSTEPL_BASE + tc_offset, ecmstep));
            m_trf.insert (TRegMap::value_type (Csimio::ECMSTEPU_BASE + tc_offset, ecmstep));
            m_trf_del.insert (TRegMap::value_type (Csimio::ECMSTEPU_BASE + tc_offset, ecmstep));

            // ECMCYCLEL/Hの生成
            TRegECMCYCLE* ecmcycle = new TRegECMCYCLE(SIZE_64BIT, SIZE_32BIT,
                                                      Csimio::ECMCYCLEL_BASE + tc_offset,
                                                      (*m_pes)[peid], m_csimio, tcid);
            m_trf.insert (TRegMap::value_type (Csimio::ECMCYCLEL_BASE + tc_offset, ecmcycle));
            m_trf.insert (TRegMap::value_type (Csimio::ECMCYCLEU_BASE + tc_offset, ecmcycle));
            m_trf_del.insert (TRegMap::value_type (Csimio::ECMCYCLEU_BASE + tc_offset, ecmcycle));
        }
        ++ it;
    }
}


// ECMカウンタ用レジスタ値のホールド取得関数
void TRegEcmCnt::HoldRegData (void)
{
    TRegMap::iterator ite = m_trf.begin();
    while (ite != m_trf.end()) {
        static_cast<EcmCountBase* > (ite->second)->SaveHoldVal ();
        ++ite;
    }
}


TRegEcmCnt::~TRegEcmCnt ()
{
    TRegMap::iterator ite = m_trf_del.begin();
    while (ite != m_trf_del.end()) {
        delete static_cast<EcmCountBase* >(ite->second);
        ++ite;
    }
}


SysErrCause TRegLOG::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    DebugMode dbgmode = ((data[0] & DBG_ON) == DBG_ON) ? DBG_ON : DBG_OFF;
    m_ht->EnableDebugMode (dbgmode);
    return SYSERR_NONE;
}


SysErrCause TRegALLOG::WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
{
    DebugMode dbgmode = ((data[0] & DBG_ON) == DBG_ON) ? DBG_ON : DBG_OFF;

    PeVec::const_iterator ite = m_pes->begin();
    while (ite != m_pes->end()) {
        if ((*ite) != NULL) {
            uint32_t peid = (*ite)->GetPeId();
            ContextId tc_last_id = NC;
            for (ContextId tcid = NC; tcid <= tc_last_id; tcid++) {
                (*m_pes)[peid]->GetHtPtr()->EnableDebugMode (dbgmode);
            }
        }
        ++ ite;
    }
    return SYSERR_NONE;
}


void TRegLog::InitTRegMap (void)
{
    PeVec::const_iterator ite = m_pes->begin();
    while (ite != m_pes->end()) {
        if ((*ite) == NULL) {
            ++ ite;
            continue;
        }
        uint32_t peid = (*ite)->GetPeId();
        if (peid > 255) return;
        PhysAddr nc_log_addr = Csimio::LOG_IO_BASE + (peid * LOG_PE_OFFSET) + 0xFF;
        TRegLOG* nc_log = new TRegLOG (SIZE_8BIT, SIZE_8BIT,
                                       nc_log_addr, (*m_pes)[peid]->GetHtPtr());
        m_trf.insert (TRegMap::value_type (nc_log_addr, nc_log));
        ContextId tc_last_id = NC;
        for (ContextId tcid = TC0; tcid <= tc_last_id; tcid++) {
            PhysAddr tc_log_addr = Csimio::LOG_IO_BASE + (peid * LOG_PE_OFFSET) + tcid;
            TRegLOG* tc_log = new TRegLOG (SIZE_8BIT, SIZE_8BIT,
                                           tc_log_addr, (*m_pes)[peid]->GetHtPtr());
            m_trf.insert (TRegMap::value_type (tc_log_addr, tc_log));
        }
        ++ ite;
    }
    TRegALLOG* allog = new TRegALLOG (SIZE_8BIT, SIZE_8BIT, Csimio::LOG_IO_BASE, m_pes);
    m_trf.insert (TRegMap::value_type (Csimio::LOG_IO_BASE, allog));
}


SysErrCause Csimio::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((addr & ECM_ADDR_MASK) == ECM_IO_BASE) {
        // ECMレジスタアクセス
        if ((addr & ECM_CTR_MASK) == ECM_CTR_VAL) {
            // 制御用レジスタアクセス
            m_treg_ecm_ctr->GetTrPtr(addr)->Write (sideband, addr, data, size);
        } else {
            // カウンタ用レジスタアクセス
            m_treg_ecm_cnt->GetTrPtr(addr)->Write (sideband, addr, data, size);
        }
    } else if ((addr & 0xFF000000) == LOG_IO_BASE) {
        m_treg_log->GetTrPtr(addr)->Write (sideband, addr, data, size);
    } else {
        // SIMIOレジスタアクセス
        PhysAddr local_addr = addr & 0xffU;

        switch (local_addr) {
        case 0x80U: WritePeNum (data, size); break;  // SIM_PENUM
        case 0xe0U: WriteSimIo (data, size); break;  // SIM_IOBUF
        case 0xf0U: WriteSimCycleLower (data, size); break;  // SIM_CYCLE (Lower32bit)
        case 0xf4U: WriteSimCycleUpper (data, size); break;  // SIM_CYCLE (Upper32bit)
        case 0xfcU: WriteResult  (data, size); break;  // Reserved for RESULT register
        case 0xfeU: WriteSimStop (data, size); break;  // SIM_STOPIO
        default:
            CfMsg::DPrint (MSG_ERR, "<Error: Unsupported register is accessed>\n");
            break;
        }
    }

    return SYSERR_NONE;
}


SysErrCause Csimio::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    if ((addr & ECM_ADDR_MASK) == ECM_IO_BASE) {
        // ECMレジスタアクセス
        if ((addr & ECM_CTR_MASK) == ECM_CTR_VAL) {
            // 制御用レジスタアクセス
            m_treg_ecm_ctr->GetTrPtr(addr)->Read (sideband, addr, data, size);
        } else {
            // カウンタ用レジスタアクセス
            m_treg_ecm_cnt->GetTrPtr(addr)->Read (sideband, addr, data, size);
        }
    } else if ((addr & 0xFF000000) == LOG_IO_BASE) {
        m_treg_log->GetTrPtr(addr)->Read (sideband, addr, data, size);
    } else {
        // SIMIOレジスタアクセス
        PhysAddr local_addr = addr & 0xffU;

        switch (local_addr) {
        case 0x80U: ReadPeNum (data, size); break;  // SIM_PENUM
        case 0xe0U: ReadSimIo (data, size); break;  // SIM_IOBUF
        case 0xf0U: ReadSimCycleLower (data, size); break;  // SIM_CYCLE(Lower32bit)
        case 0xf4U: ReadSimCycleUpper (data, size); break;  // SIM_CYCLE(Upper32bit)
        case 0xfcU: ReadResult  (data, size); break;  // Reserved for RESULT register
        case 0xfeU: ReadSimStop (data, size); break;  // SIM_STOPIO
        default:
            CfMsg::DPrint (MSG_ERR, "<Error: Unsupported register is accessed>\n");
            break;
        }
    }

    return SYSERR_NONE;
}


SysErrCause Csimio::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetWrite (SideBand::DebugAccess(), addr, data, size);
}


SysErrCause Csimio::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return TargetRead (SideBand::DebugAccess(), addr, data, size);
}


void Csimio::WriteSimIo(uint8_t* data, ByteSize size)
{
    int output_char = static_cast<int>(data[0] & 0xffU);
    putchar (output_char);
    fflush (stdout);
}


void Csimio::ReadSimIo(uint8_t* data, ByteSize size)
{
    int input_char = getchar();
    data[0] = static_cast<uint8_t>(input_char & 0xff);
}


void Csimio::WriteResult(uint8_t* data, ByteSize size)
{
    uint32_t tdata = 0;
    for (ByteSize i = 0; i < size; i++) {
        tdata |=  (data[i] << (i<<3)) & 0xFF;
    }
    if (tdata == 0x0) {
        p_internals->SetExitStatus (EXIT_PASS);
    } else {
        p_internals->SetExitStatus (EXIT_FAIL);
    }
}


void Csimio::ReadResult(uint8_t* data, ByteSize size)
{
}


void Csimio::WriteSimStop(uint8_t* data, ByteSize size)
{
    // This address is only for access break.
    // Nothing to do
}


void Csimio::ReadSimStop(uint8_t* data, ByteSize size)
{
    // This address is only for access break.
    // Nothing to do
}


void Csimio::WritePeNum(uint8_t* data, ByteSize size)
{
}


void Csimio::ReadPeNum(uint8_t* data, ByteSize size)
{
    uint32_t penum = p_forest_util->GetPeNum ();
    for (ByteSize i = 0; i < size; i++) {
        data[i] = (penum >> (i<<3)) & 0xffU;
    }
}


void Csimio::WriteSimCycleLower(uint8_t* data, ByteSize size)
{
    // Write is ignored
}


void Csimio::ReadSimCycleLower(uint8_t* data, ByteSize size)
{
    // SystemTimeの下位32bitを格納
    // ただし、アクセスサイズが8byteなら下位32bitだけでなく64bit全部を格納
    uint64_t simtime = p_internals->GetSystemTime ();
    for (ByteSize i = 0; i < size; i++) {
        data[i] = static_cast<uint8_t>((simtime >> (i<<3)) & 0xffU);
    }
}


void Csimio::WriteSimCycleUpper(uint8_t* data, ByteSize size)
{
    // Write is ignored
}


void Csimio::ReadSimCycleUpper(uint8_t* data, ByteSize size)
{
    // SystemTimeの上位32bitを格納
    uint64_t simtime = (p_internals->GetSystemTime () >> 32) & 0xffffffffU;
    for (ByteSize i = 0; i < size; i++) {
        data[i] = static_cast<uint8_t>((simtime >> (i<<3)) & 0xffU);
    }
}


// ECMカウンタ用レジスタ値のホールド取得関数
void Csimio::HoldRegData (void)
{
    m_treg_ecm_cnt->HoldRegData ();
}


void Csimio::HardReset (void)
{
    m_treg_ecm_ctr->ResetTRegs ();
    m_treg_ecm_cnt->ResetTRegs ();
    m_treg_log->ResetTRegs ();
}


Csimio::Csimio (PeVec* pe) : m_pes (pe)
{
    // ホールド状態フラグの初期化
    m_is_ecm_hold   = false;

    // ECM制御用レジスタセットの初期化
    m_treg_ecm_ctr = new TRegEcmCtr (this);
    // ECMカウンタ用レジスタセットの初期化
    m_treg_ecm_cnt = new TRegEcmCnt (pe, this);
    m_treg_log = new TRegLog (pe);
}


Csimio::~Csimio () {
    delete m_treg_ecm_ctr;
    delete m_treg_ecm_cnt;
    delete m_treg_log;
}
