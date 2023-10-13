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
#include "./mecnt.h"


/*!
 * @brief Instantiates G0MEV register. : G0MEVレジスタのインスタンス化
 * @param -
 */
void TRegMecnt::InitTRegMap (void)
{
    for (uint32_t i = 0; i < N_G0MEV; i++) {
        TRegG0MEV* g0mev = new TRegG0MEV (SIZE_32BIT, SIZE_8BIT, i * 4);
        m_trf.insert (TRegMap::value_type (g0mev->GetTRegAddr (), g0mev));
    }
}


/*!
 * @brief Writes to the register. : レジスタライト関数
 * @param sideband Sideband. : サイドバンド
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Mecnt::TargetWrite (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    return m_trfs->GetTrPtr (addr & MECNT_REG_ADDR_MASK)->Write (sideband, addr, data, size);
}


/*!
 * @brief Reads from the register. : レジスタリード関数
 * @param sideband Sideband. : サイドバンド
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Mecnt::TargetRead (SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size)
{
    return m_trfs->GetTrPtr (addr & MECNT_REG_ADDR_MASK)->Read (sideband, addr, data, size);
}


/*!
 * @brief Writes to the register with debugging. : レジスタデバックライト関数
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Mecnt::TargetWriteDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return m_trfs->GetTrPtr (addr & MECNT_REG_ADDR_MASK)->WriteBody (addr, data, size);
}


/*!
 * @brief Reads from the register with debugging. : レジスタデバックリード関数
 * @param addr Address. : アドレス
 * @param data Data. : データ
 * @param size Size. : サイズ
 * @return SYSERR_NONE
 */
SysErrCause Mecnt::TargetReadDebug (PhysAddr addr, uint8_t* data, ByteSize size)
{
    return m_trfs->GetTrPtr (addr & MECNT_REG_ADDR_MASK)->ReadBody (addr, data, size);
}


void Mecnt::HardReset (void)
{
    m_trfs->ResetTRegs ();
}


/*!
 * @brief Constructs MECNT class. : コンストラクタ
 * @param -
 */
Mecnt::Mecnt ()
{
    m_trfs = new TRegMecnt (this);
}
