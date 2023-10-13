/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include <string.h>
#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./forest.h"
#include "./sideband.h"
#include "./treg.h"

// typedef std::vector<ProcElement*> PeVec;
#include "proc_element_vector.h"

class Csimio;

// ECMCTRレジスタクラス
// ECM制御用レジスタは特殊な処理（ホールド機能など）がないため、
// ターゲットレジスタの抽象クラスをそのまま継承する。
class TRegECMCTR : public TRegDataBase<TRegData32> {
private:
    union {
        TRegData32 m_data;
        struct {
            RegData m_hold :1;
        } bit;
    } m_ecmstr;

    Csimio* m_csimio;

public:
    void SetHOLD (RegData hold);

    RegData GetHOLD (void) const { return m_ecmstr.bit.m_hold; }

    void Reset (void);

    //オーバーライド
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

    const char* GetName (void) const { return "ECMCTR"; }

    TRegECMCTR (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, Csimio* csimio)
        : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr,
                                    TRegMask(0x00000001U)), m_csimio(csimio) {
        m_treg = &m_ecmstr.m_data;
    }
};


// ECM制御用レジスタセットのクラス
class TRegEcmCtr : public TRegModuleVc {
public:
    explicit TRegEcmCtr (Csimio* csimio) : m_csimio(csimio) {
        InitTRegMap ();
    }
    ~TRegEcmCtr ();

private:
    Csimio* m_csimio;

    void InitTRegMap (void);
};


// ホールド機能関数を抽象クラスに持たせるため、
// ターゲットレジスタの抽象クラスを継承した
// ECM独自のカウンタレジスタ用抽象クラスを定義する
class EcmCountBase : public TRegDataBase<TRegData32> {
private:
    uint64_t m_hold_data;

public:
    ProcElement* m_pe;
    Csimio* m_csimio;
    const ContextId m_tcid;

    // 実測値取得用の仮想関数
    virtual uint64_t GetData (void) const = 0;

    void Reset (void) {
        m_hold_data = 0x0000000000000000ULL;
    }

    // オーバーライド
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size)
        { return SYSERR_NONE; }
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
        { return SYSERR_NONE; }

    // オーバーライド
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    void SaveHoldVal(void) { m_hold_data = GetData (); }  // ホールド取得関数

    EcmCountBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                  ProcElement* pe, Csimio* csimio, ContextId tcid)
        : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr, TRegMask(0xFFFFFFFFU)),
          m_pe(pe), m_csimio(csimio), m_tcid(tcid) {}

    virtual ~EcmCountBase () {}
};


// ECMSTEPL/Hpt レジスタクラス
class TRegECMSTEP : public EcmCountBase {
public:
    uint64_t    GetData (void) const;
    const char* GetName (void) const { return "ECMSTEPL/H"; }

    TRegECMSTEP (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                 ProcElement* pe, Csimio* csimio, ContextId tcid)
        : EcmCountBase (reg_size, min_access_size, reg_addr, pe, csimio, tcid) {}
};


// ECMCYCLEL/Hpt レジスタクラス
class TRegECMCYCLE : public EcmCountBase {
public:
    uint64_t    GetData (void) const;
    const char* GetName (void) const { return "ECMCYCLEL/H"; }

    TRegECMCYCLE (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr,
                  ProcElement* pe, Csimio* csimio, ContextId tcid)
        : EcmCountBase (reg_size, min_access_size, reg_addr, pe, csimio, tcid) {}
};


// ECMSYSCYCLEL/H レジスタクラス
class TRegECMSYSCYCLE : public EcmCountBase {
public:
    uint64_t    GetData (void) const;
    const char* GetName (void) const { return "ECMSYSCYCLEL/H"; }

    TRegECMSYSCYCLE (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                     ProcElement* pe, Csimio* csimio, ContextId tcid)
        : EcmCountBase (reg_size, min_access_size, reg_addr, pe, csimio, tcid) {}
};


// ECMPECYCLEL/H レジスタクラス
class TRegECMPECYCLE : public EcmCountBase {
public:
    uint64_t    GetData (void) const;
    const char* GetName (void) const { return "ECMPECYCLEL/H"; }

    TRegECMPECYCLE (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                    ProcElement* pe, Csimio* csimio, ContextId tcid)
        : EcmCountBase (reg_size, min_access_size, reg_addr, pe, csimio, tcid) {}
};


// ECMカウンタ用レジスタセットのクラス
class TRegEcmCnt : public TRegModuleVc {
public:
    TRegEcmCnt (PeVec* pes, Csimio* csimio)
        : m_pes(pes), m_csimio(csimio) {
        InitTRegMap ();
    }
    ~TRegEcmCnt ();

    void ResetTRegs (void) {}  // リセット不要
    void HoldRegData  ();     // ホールド取得関数

private:
    PeVec* m_pes;
    Csimio* m_csimio;

    TRegMap m_trf_del;  // デリート用

    void InitTRegMap (void);
};


/*!
  @brief SIMIOレジスタクラス
 */
class TRegLog : public TRegModuleVc {
public:
    explicit TRegLog (PeVec* pes) : m_pes (pes) { InitTRegMap (); }
    ~TRegLog () { DelTRegMap (); }
private:
    static const uint32_t LOG_PE_OFFSET = 0x0100U;
    PeVec* m_pes;
    void InitTRegMap (void);
};


/*!
  @brief LOGレジスタクラス
 */
class TRegLOG : public TRegDataBase<TRegData8> {
private:
    FsCtrl* m_ht;

public:
    static const uint32_t LOG_MASK = 0x0U;
    void Reset (void) {}

    //オーバーライド
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const {
        memset (data, 0, size);
        return SYSERR_NONE;
    }
    const char* GetName (void) const { return "LOG"; }

    TRegLOG (ByteSize reg_size, ByteSize min_acc_size, PhysAddr reg_addr, FsCtrl* ht)
        : TRegDataBase<TRegData8> (reg_size, min_acc_size, reg_addr,
                                   TRegMask(LOG_MASK)), m_ht(ht)
        { Reset(); }
};


/*!
  @brief ALLOGレジスタクラス
 */
class TRegALLOG : public TRegDataBase<TRegData8> {
private:
    PeVec* m_pes;

public:
    static const uint32_t ALLOG_MASK = 0x0U;
    void Reset (void) {}

    //オーバーライド
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const {
        memset (data, 0, size);
        return SYSERR_NONE;
    }
    const char* GetName (void) const { return "ALLOG"; }

    TRegALLOG (ByteSize reg_size, ByteSize min_acc_size, PhysAddr reg_addr, PeVec* pes)
        : TRegDataBase<TRegData8> (reg_size, min_acc_size, reg_addr,
                                   TRegMask(ALLOG_MASK)), m_pes (pes)
        { Reset(); }
};


class Csimio : public CModuleVc
{
private:
    TRegLog* m_treg_log;
    PeVec* m_pes;

public:
    void WriteSimIo(uint8_t* data, ByteSize size);
    void ReadSimIo(uint8_t* data, ByteSize size);
    void WritePeNum(uint8_t* data, ByteSize size);
    void ReadPeNum(uint8_t* data, ByteSize size);
    void WriteSimStop(uint8_t* data, ByteSize size);
    void ReadSimStop(uint8_t* data, ByteSize size);
    void WriteResult(uint8_t* data, ByteSize size);
    void ReadResult(uint8_t* data, ByteSize size);
    void WriteSimCycleLower(uint8_t* data, ByteSize size);
    void ReadSimCycleLower(uint8_t* data, ByteSize size);
    void WriteSimCycleUpper(uint8_t* data, ByteSize size);
    void ReadSimCycleUpper(uint8_t* data, ByteSize size);

    void HoldRegData ();         // ホールド取得関数

    // Although upper address should be controled by connection part (in forest.cpp),
    // because multiple modules (SIMIO and ECM and LOG) are merged in simio,
    // TargetRead/Write checks address including upper address.
    // So, upper address is defined here.
    // @ TODO : Separate modules for each SIMIO/ECM/LOG.
    //          Then, upper address can be controled by forest side.
    static const PhysAddr SIM_IO_BASE;
    static const PhysAddr SIM_IO_MASK;
    static const PhysAddr SIM_STOPIO;
    static const PhysAddr ECM_IO_BASE;
    static const PhysAddr ECM_IO_MASK;
    static const PhysAddr LOG_IO_BASE;
    static const PhysAddr LOG_IO_MASK;
    static const PhysAddr G3K_SIM_IO_BASE;
    static const PhysAddr G3K_SIM_IO_MASK;

    // ECM用のリソース・関数
    static const PhysAddr ECMCTR_ADDR;
    static const PhysAddr ECMSTEPL_BASE;
    static const PhysAddr ECMSTEPU_BASE;
    static const PhysAddr ECMCYCLEL_BASE;
    static const PhysAddr ECMCYCLEU_BASE;

    static const PhysAddr ECM_ADDR_MASK;
    static const PhysAddr ECM_CTR_MASK;
    static const PhysAddr ECM_CTR_VAL;
    static const PhysAddr ECM_PEID_SHIFT;
    static const PhysAddr ECM_TCID_SHIFT;
    static const PhysAddr ECM_SYS_TCID;
    static const PhysAddr ECM_PE_TCID;

    bool m_is_ecm_hold;          // ホールド状態フラグ
    TRegEcmCtr* m_treg_ecm_ctr;  // ECM制御用レジスタセット
    TRegEcmCnt* m_treg_ecm_cnt;  // ECMカウンタ用レジスタセット

    void CyclicHandler(void) {}
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);

    explicit Csimio (PeVec* pe);
    ~Csimio();
};
