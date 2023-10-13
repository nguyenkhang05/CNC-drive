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
#include "./ipir.h"
#include "./forest_internals.h"
#include "./proc_element.h"
/*!
 * @brief Instantiates IPIR register. : IPIRレジスタのインスタンス化
 * @param -
 */
void TRegIpir::InitTRegMap(void) {
  for (uint32_t j = 0; j < 8; j++) {
    PhysAddr ipien_addr = IPIEN_OFFSET;
    for (uint32_t i = 0; i < N_G4MH_CH; i++) {
      TRegIPIEN* ipien =
          // new TRegIPIEN(SIZE_32BIT, SIZE_8BIT, ipien_addr + i * 0x20 + j * 0x100, i, j, m_ipir);
          new TRegIPIEN(SIZE_8BIT, SIZE_8BIT, ipien_addr + i * 0x20 + j * 0x100, i, j);
      m_trf.insert(TRegMap::value_type(ipien->GetTRegAddr(), ipien));
    }

    PhysAddr ipiflg_addr = IPIFLG_OFFSET;
    for (uint32_t i = 0; i < N_G4MH_CH; i++) {
      TRegIPIFLG* ipiflg =
          // new TRegIPIFLG(SIZE_32BIT, SIZE_8BIT, ipiflg_addr + i * 0x20 + j * 0x100, i, j, m_ipir);
          new TRegIPIFLG(SIZE_8BIT, SIZE_8BIT, ipiflg_addr + i * 0x20 + j * 0x100, i, j);
      m_trf.insert(TRegMap::value_type(ipiflg->GetTRegAddr(), ipiflg));
    }

    PhysAddr ipifclr_addr = IPIFCLR_OFFSET;
    for (uint32_t i = 0; i < N_G4MH_CH; i++) {
      TRegIPIFCLR* ipifclr =
          // new TRegIPIFCLR(SIZE_32BIT, SIZE_8BIT, ipifclr_addr + i * 0x20 + j * 0x100, i, j, m_ipir);
          new TRegIPIFCLR(SIZE_8BIT, SIZE_8BIT, ipifclr_addr + i * 0x20 + j * 0x100, i, j);

      ipifclr->Get_IPIFLG =
          std::bind(&CIpir::Get_IPIFLG, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipifclr->Set_IPIFLG = std::bind(&CIpir::Set_IPIFLG, m_ipir, std::placeholders::_1,
                                      std::placeholders::_2, std::placeholders::_3);
      ipifclr->CancelIntRequest = std::bind(&CIpir::CancelIntRequest, m_ipir, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3);
      ipifclr->Get_IPIREQ =
          std::bind(&CIpir::Get_IPIREQ, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipifclr->Set_IPIREQ = std::bind(&CIpir::Set_IPIREQ, m_ipir, std::placeholders::_1,
                                      std::placeholders::_2, std::placeholders::_3);

      m_trf.insert(TRegMap::value_type(ipifclr->GetTRegAddr(), ipifclr));
    }

    PhysAddr ipireq_addr = IPIREQ_OFFSET;
    for (uint32_t i = 0; i < N_G4MH_CH; i++) {
      TRegIPIREQ* ipireq =
          // new TRegIPIREQ(SIZE_32BIT, SIZE_8BIT, ipireq_addr + i * 0x20 + j * 0x100, i, j, m_ipir);
          new TRegIPIREQ(SIZE_8BIT, SIZE_8BIT, ipireq_addr + i * 0x20 + j * 0x100, i, j);

      ipireq->Get_IPIREQ =
          std::bind(&CIpir::Get_IPIREQ, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipireq->Set_IPIREQ = std::bind(&CIpir::Set_IPIREQ, m_ipir, std::placeholders::_1,
                                     std::placeholders::_2, std::placeholders::_3);
      ipireq->Get_IPIEN =
          std::bind(&CIpir::Get_IPIEN, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipireq->Get_IPIFLG =
          std::bind(&CIpir::Get_IPIFLG, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipireq->Set_IPIFLG = std::bind(&CIpir::Set_IPIFLG, m_ipir, std::placeholders::_1,
                                     std::placeholders::_2, std::placeholders::_3);
      ipireq->SendIntRequest = std::bind(&CIpir::SendIntRequest, m_ipir, std::placeholders::_1,
                                         std::placeholders::_2, std::placeholders::_3);

      m_trf.insert(TRegMap::value_type(ipireq->GetTRegAddr(), ipireq));
    }

    PhysAddr ipirclr_addr = IPIRCLR_OFFSET;
    for (uint32_t i = 0; i < N_G4MH_CH; i++) {
      TRegIPIRCLR* ipirclr =
          // new TRegIPIRCLR(SIZE_32BIT, SIZE_8BIT, ipirclr_addr + i * 0x20 + j * 0x100, i, j, m_ipir);
          new TRegIPIRCLR(SIZE_8BIT, SIZE_8BIT, ipirclr_addr + i * 0x20 + j * 0x100, i, j);

      ipirclr->Get_IPIREQ =
          std::bind(&CIpir::Get_IPIREQ, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipirclr->Set_IPIREQ = std::bind(&CIpir::Set_IPIREQ, m_ipir, std::placeholders::_1,
                                      std::placeholders::_2, std::placeholders::_3);
      ipirclr->Get_IPIEN =
          std::bind(&CIpir::Get_IPIEN, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipirclr->Get_IPIFLG =
          std::bind(&CIpir::Get_IPIFLG, m_ipir, std::placeholders::_1, std::placeholders::_2);
      ipirclr->Set_IPIFLG = std::bind(&CIpir::Set_IPIFLG, m_ipir, std::placeholders::_1,
                                      std::placeholders::_2, std::placeholders::_3);
      ipirclr->CancelIntRequest = std::bind(&CIpir::CancelIntRequest, m_ipir, std::placeholders::_1,
                                            std::placeholders::_2, std::placeholders::_3);

      m_trf.insert(TRegMap::value_type(ipirclr->GetTRegAddr(), ipirclr));
    }
  }
}

PhysAddr TRegIpir::TrIPIREQAddr(uint32_t channel, uint32_t ipir_id) const {
  return IPIREQ_OFFSET + channel * 0x20 + ipir_id * 0x100;
}

PhysAddr TRegIpir::TrIPIENAddr(uint32_t channel, uint32_t ipir_id) const {
  return IPIEN_OFFSET + channel * 0x20 + ipir_id * 0x100;
}

PhysAddr TRegIpir::TrIPIFLGAddr(uint32_t channel, uint32_t ipir_id) const {
  return IPIFLG_OFFSET + channel * 0x20 + ipir_id * 0x100;
}

TRegIPIREQ* CIpir::GetIPIREQPtr(uint32_t channel, uint32_t ipir_id) const {
  TRegIpir* trfs = static_cast<TRegIpir*>(m_trf);
  PhysAddr ipireq_addr = trfs->TrIPIREQAddr(channel, ipir_id);
  return trfs->TrIPIREQ(ipireq_addr);
}

TRegIPIEN* CIpir::GetIPIENPtr(uint32_t channel, uint32_t ipir_id) const {
  TRegIpir* trfs = static_cast<TRegIpir*>(m_trf);
  PhysAddr ipien_addr = trfs->TrIPIENAddr(channel, ipir_id);
  return trfs->TrIPIEN(ipien_addr);
}

TRegIPIFLG* CIpir::GetIPIFLGPtr(uint32_t channel, uint32_t ipir_id) const {
  TRegIpir* trfs = static_cast<TRegIpir*>(m_trf);
  PhysAddr ipiflg_addr = trfs->TrIPIFLGAddr(channel, ipir_id);
  return trfs->TrIPIFLG(ipiflg_addr);
}

bool CIpir::IsValidPeId(uint32_t peid) const {
  if (peid < m_pes->size()) {
    return (*m_pes)[peid] != NULL;
  } else {
    return false;
  }
}

PhysAddr CIpir::ConvertSelfAddr(uint32_t peid, PhysAddr addr) {
   if (addr < IPIR_S_OFFSET) {
     return (addr + IPIR_S_OFFSET + IPIR_PE_OFFSET*peid);
   }
   else {
     return addr;
   }
}

/*!
 * @brief Writes to the register. : レジスタライト関数
 * @param sideband Sideband. :サイドバンド
 * @param addr Address. :アドレス
 * @param data Data. :データ
 * @param size Size. :サイズ
 * @return System error information.
 */
SysErrCause CIpir::TargetWrite(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
#if _UNIT_TEST
  uint32_t peid = 0;
#else
  uint32_t peid = p_forest_util->GetPeidFromBmid (sideband.GetBMID ());
#endif
  PhysAddr maddr = addr & IPIR_REG_ADDR_MASK;
  //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
  //Masters except PEx can access to actual area, read/write normally as spec description
  if ((peid == CF_INVALID_ID) && (maddr < IPIR_S_OFFSET)) {
    return SYSERR_NONE;
  }
  PhysAddr paddr = ConvertSelfAddr(peid, maddr);
  m_trf->GetTrPtr(paddr)->Write(sideband, addr, data, size);
  return SYSERR_NONE;
}

/*!
 * @brief Reads from the register. : レジスタリード関数
 * @param sideband Sideband. :サイドバンド
 * @param addr Address. :アドレス
 * @param data Data. :データ
 * @param size Size. :サイズ
 * @return System error information.
 */
SysErrCause CIpir::TargetRead(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
#if _UNIT_TEST
  uint32_t peid = 0;
#else
  uint32_t peid = p_forest_util->GetPeidFromBmid (sideband.GetBMID ());
#endif
  PhysAddr maddr = addr & IPIR_REG_ADDR_MASK;
  //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
  //Masters except PEx can access to actual area, read/write normally as spec description
  if (peid == CF_INVALID_ID && (maddr < IPIR_S_OFFSET)) {
    memset(data, 0, size);
    return SYSERR_NONE;
  }
  PhysAddr paddr = ConvertSelfAddr(peid, maddr);
  m_trf->GetTrPtr(paddr)->Read(sideband, addr, data, size);
  return SYSERR_NONE;
}

/*!
 * @brief Writes to the register with debugging. : レジスタデバックライト関数
 * @param addr Address. :アドレス
 * @param data Data. :データ
 * @param size Size. :サイズ
 * @return System error information.
 */
SysErrCause CIpir::TargetWriteDebug(PhysAddr addr, uint8_t* data, ByteSize size) {
#if _UNIT_TEST
  uint32_t peid = 0;
#else
  uint32_t peid = p_forest_util->GetPeidOfDbgAPI ();
#endif
  PhysAddr maddr = addr & IPIR_REG_ADDR_MASK;
  //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
  //Masters except PEx can access to actual area, read/write normally as spec description
  if ((peid == CF_INVALID_PEID) && (maddr < IPIR_S_OFFSET)) {
    return SYSERR_NONE;
  }
  PhysAddr paddr = ConvertSelfAddr(peid, maddr);
  m_trf->GetTrPtr(paddr)->WriteBody(addr, data, size);
  return SYSERR_NONE;
}

/*!
 * @brief Reads from the register with debugging. : レジスタデバックリード関数
 * @param addr Address. :アドレス
 * @param data Data. :データ
 * @param size Size. :サイズ
 * @return System error information.
 */
SysErrCause CIpir::TargetReadDebug(PhysAddr addr, uint8_t* data, ByteSize size) {
#if _UNIT_TEST
  uint32_t peid = 0;
#else
  uint32_t peid = p_forest_util->GetPeidOfDbgAPI ();
#endif
  PhysAddr maddr = addr & IPIR_REG_ADDR_MASK;
  //Masters except PEx cannot access to self area, the register returns 0, write access is ignored
  //Masters except PEx can access to actual area, read/write normally as spec description
  if ((peid == CF_INVALID_PEID) && (maddr < IPIR_S_OFFSET)) {
    memset(data, 0, size);
    return SYSERR_NONE;
  }
  PhysAddr paddr = ConvertSelfAddr(peid, maddr);
  m_trf->GetTrPtr(paddr)->ReadBody(addr, data, size);
  return SYSERR_NONE;
}

/*!
 * @brief Resets IPIR. : IPIRリセット
 * @param -
 */
void CIpir::HardReset(void) { m_trf->ResetTRegs(); }

void CIpir::Set_IPIFLG(uint32_t channel, uint32_t ipir_id, RegData data) {
  TRegIPIFLG* ipiflg = GetIPIFLGPtr(channel, ipir_id);
  ipiflg->SetData(data);
}

RegData CIpir::Get_IPIFLG(uint32_t channel, uint32_t ipir_id) {
  TRegIPIFLG* ipiflg = GetIPIFLGPtr(channel, ipir_id);
  return (ipiflg->GetData() & 0xFFU);
}

void CIpir::Set_IPIREQ(uint32_t channel, uint32_t ipir_id, RegData data) {
  TRegIPIREQ* ipireq = GetIPIREQPtr(channel, ipir_id);
  ipireq->SetData(data);
}

RegData CIpir::Get_IPIREQ(uint32_t channel, uint32_t ipir_id) {
  TRegIPIREQ* ipireq = GetIPIREQPtr(channel, ipir_id);
  return (ipireq->GetData() & 0xFFU);
}

void CIpir::Set_IPIEN(uint32_t channel, uint32_t ipir_id, RegData data) {
  TRegIPIEN* ipien = GetIPIENPtr(channel, ipir_id);
  ipien->SetData(data);
}

RegData CIpir::Get_IPIEN(uint32_t channel, uint32_t ipir_id) {
  TRegIPIEN* ipien = GetIPIENPtr(channel, ipir_id);
  return (ipien->GetData() & 0xFFU);
}

void CIpir::SendIntRequest(uint32_t channel, uint32_t ipir_id, uint32_t peid_request_int) {
  if (IsValidPeId(ipir_id) == false) {
    return;
  }
  //eiint channel = ipir channel + offset setting from python
#if _UNIT_TEST
  uint32_t offset = 0;
#else
  uint32_t offset = p_forest_util->GetEiintChannelOffset ();
#endif
  (*m_pes)[ipir_id]->ReqIntByPeripheral(channel + offset);
}

void CIpir::CancelIntRequest(uint32_t channel, uint32_t ipir_id, uint32_t peid_request_int) {
  if (IsValidPeId(ipir_id) == false) {
    return;
  }

  //eiint channel = ipir channel + offset setting from python
#if _UNIT_TEST
  uint32_t offset = 0;
#else
  uint32_t offset = p_forest_util->GetEiintChannelOffset ();
#endif
  (*m_pes)[ipir_id]->CancelIntByPeripheral(channel + offset);
}

/*!
 * @brief Constructs IPIR class. : コンストラクタ
 * @param pes the pointer to the vector of ProcElement* type. : ProcElement*型ベクトルポインタ
 * @param type Type of the IPIR implimentation. : IPIR実装タイプ
 */
CIpir::CIpir(PeVec* pes, IpirImplType type)
    : m_impl_type(type), m_pes(pes) {
  m_trf = new TRegIpir(this, pes);
}
