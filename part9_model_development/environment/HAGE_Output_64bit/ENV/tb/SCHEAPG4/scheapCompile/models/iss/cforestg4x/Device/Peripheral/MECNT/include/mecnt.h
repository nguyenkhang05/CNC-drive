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

#include <vector>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./treg.h"

class Mecnt;


/*!
  @brief G0MEV register class. : G0MEVレジスタクラス
 */
class TRegG0MEV : public TRegDataBase<TRegData32> {
private:
    TRegData32 m_data;

public:
    static const uint32_t G0MEV_MASK = 0xFFFFFFFFU;

    void Reset (void) { m_data.m_data32 = 0x0U; }
    const char* GetName (void) const { return "G0MEV"; }

    TRegG0MEV (uint32_t reg_size, uint32_t mini_access_size, PhysAddr reg_addr)
    : TRegDataBase<TRegData32> (reg_size, mini_access_size, reg_addr, TRegMask (G0MEV_MASK)) {
        m_data.m_data32 = 0x0U;
        m_treg = &m_data;
    }
    ~TRegG0MEV (void) {}
};


/*!
  @brief MECNT register class. : MECNTレジスタクラス
 */
class TRegMecnt : public TRegModuleVc {
public:
    explicit TRegMecnt (Mecnt* mecnt) : m_mecnt (mecnt) { InitTRegMap (); }
    ~TRegMecnt () { DelTRegMap (); }

private:
    Mecnt* m_mecnt;
    static const uint32_t N_G0MEV = 32;

    void InitTRegMap (void);
};


/*!
  @brief MECNT class. : MECNTクラス
 */
class Mecnt : public CModuleVc {
private:
    TRegMecnt* m_trfs;

public:

    static const PhysAddr MECNT_REG_SIZE = 0x00001000;
    static const PhysAddr MECNT_REG_ADDR_MASK = MECNT_REG_SIZE - 1;

    void CyclicHandler (void) {}
    SysErrCause TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetRead  (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetFetch (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
        return TargetRead (sideband, addr, data, size);
    }
    SysErrCause TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size);
    SysErrCause TargetReadDebug  (PhysAddr addr, uint8_t* data, ByteSize size);
    void HardReset (void);

    Mecnt ();
    virtual ~Mecnt (void) { delete m_trfs; }
};
