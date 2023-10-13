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

#include <map>
#include "./forest_common.h"
#include "./forest_message.h"
#include "./sideband.h"


/*!
  @brief ターゲットレジスタ マスククラス
 */
class TRegMask {
public:
    union {
        uint32_t m_mask32;
        uint16_t m_mask16[2];
        uint8_t  m_mask8[4];
    };
    explicit TRegMask (uint32_t mask) : m_mask32 (mask) {}
    ~TRegMask (void) {}
};


/*!
  @brief ターゲットレジスタ 抽象クラス
 */
class TRegBase {
public:
    virtual void Reset (void) = 0;
    virtual SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size) = 0;
    virtual SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) = 0;
    virtual SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const = 0;
    virtual SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data,
                              ByteSize size) const = 0;
    virtual const char* GetName (void) const = 0;
    virtual ~TRegBase () {}
    bool IsRegAccessSizeValid (ByteSize size) const;
    PhysAddr GetTRegAddr (void) const { return m_reg_addr; }

    bool IsSearchHit (PhysAddr addr) const {
        return (addr >= m_reg_addr && addr < m_reg_addr + m_reg_size);
    }

    void PrintAccessSizeWarning (void) const {
        CfMsg::DPrint (MSG_WAR,
                       "--:--:--:-: <Warning: Un-guaranteed Size Access to %s>\n", GetName());
    }

    TRegBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr) {
        m_reg_size = reg_size;
        m_min_access_size = min_access_size;
        m_reg_addr = reg_addr;
    }

protected:
    PhysAddr m_reg_addr;

private:
    ByteSize m_reg_size; //レジスタサイズ
    ByteSize m_min_access_size; //レジスタアクセス最小サイズ
};


/*!
  @brief ターゲットレジスタ データクラス
 */
template <class TR>
class TRegDataBase : public TRegBase {
public:
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const;
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const;

    TRegDataBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr)
        : TRegBase (reg_size, min_access_size, reg_addr) , m_reg_mask (0) {}   // New constructor
    TRegDataBase (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
        : TRegBase (reg_size, min_access_size, reg_addr), m_reg_mask (reg_mask) {}

protected:
    TR* m_treg;
    const TRegMask m_reg_mask; //レジスタマスククラス
};


/*!
  @brief ターゲットレジスタ 32ビットデータクラス
 */
class TRegData32 {
public:
    union {
        uint32_t m_data32;
        uint16_t m_data16[2];
        uint8_t  m_data8[4];
    };

    void Write32 (PhysAddr addr, uint8_t* data, const TRegMask mask);
    void Read32  (PhysAddr addr, uint8_t* data) const;

    void Write16 (PhysAddr addr, uint8_t* data, const TRegMask mask);
    void Read16  (PhysAddr addr, uint8_t* data) const;

    void Write8 (PhysAddr addr, uint8_t* data, const TRegMask mask);
    void Read8  (PhysAddr addr, uint8_t* data) const;
};


/*!
  @brief ターゲットレジスタ 16ビットデータクラス
 */
class TRegData16 {
public:
    union {
        uint16_t m_data16;
        uint8_t  m_data8[2];
    };

    void Write32 (PhysAddr addr, uint8_t* data, const TRegMask mask) {}
    void Read32  (PhysAddr addr, uint8_t* data) const {}

    void Write16 (PhysAddr addr, uint8_t* data, const TRegMask mask);
    void Read16  (PhysAddr addr, uint8_t* data) const;

    void Write8 (PhysAddr addr, uint8_t* data, const TRegMask mask);
    void Read8  (PhysAddr addr, uint8_t* data) const;
};


/*!
  @brief ターゲットレジスタ 8ビットデータクラス
 */
class TRegData8 {
public:
    uint8_t m_data8;

    void Write32 (PhysAddr addr, uint8_t* data, const TRegMask mask) {}
    void Read32  (PhysAddr addr, uint8_t* data) const {}

    void Write16 (PhysAddr addr, uint8_t* data, const TRegMask mask) {}
    void Read16  (PhysAddr addr, uint8_t* data) const {}

    void Write8 (PhysAddr addr, uint8_t* data, const TRegMask mask);
    void Read8  (PhysAddr addr, uint8_t* data) const;
};


/*!
  @brief DUMMYレジスタクラス
 */
class TRegDUMMY : public TRegDataBase<TRegData32> {
public:
    void Reset (void) {}
    SysErrCause Write (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return WriteBody (addr, data, size);
    }
    SysErrCause WriteBody (PhysAddr addr, uint8_t* data, ByteSize size) {
        CfMsg::DPrint (MSG_WAR,
                       "--:--:--:-: <Warning: invalid TR access : write ignored>\n");
        return SYSERR_NONE;
    }
    SysErrCause Read (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) const {
        return ReadBody (addr, data, size);
    }
    SysErrCause ReadBody (PhysAddr addr, uint8_t* data, ByteSize size) const {
        CfMsg::DPrint (MSG_WAR,
                       "--:--:--:-: <Warning: invalid TR access : read 0x0>\n");
        std::fill_n (data, size, 0);
        return SYSERR_NONE;
    }
    const char* GetName (void) const { return "TR_DUMMY"; }
    TRegDUMMY (ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
            : TRegDataBase<TRegData32> (reg_size, min_access_size, reg_addr, reg_mask) {}
};


/*!
  @brief ターゲットモジュールレジスタベースクラス
 */
class TRegModuleVc {
private:
    TRegDUMMY* m_dummy;

protected:
    typedef std::map <const PhysAddr, TRegBase*> TRegMap;
    TRegMap m_trf;

public:
    virtual TRegBase* GetTrPtr (PhysAddr addr) const;
    virtual void InitTRegMap (void) = 0;
    virtual void DelTRegMap (void);
    virtual void ResetTRegs (void);

    TRegModuleVc () {
        m_dummy = new TRegDUMMY (0, 0, 0, TRegMask(0));
    }
    virtual ~TRegModuleVc () {
        delete m_dummy;
        m_trf.clear();
    }
};
