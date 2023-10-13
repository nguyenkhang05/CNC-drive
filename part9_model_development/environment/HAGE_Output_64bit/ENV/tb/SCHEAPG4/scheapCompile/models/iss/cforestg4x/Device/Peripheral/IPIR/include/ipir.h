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
#include "treg_ipir.h"

class CIpir;
// class ProcElement;
// typedef std::vector<ProcElement*> PeVec;
#include "proc_element_vector.h"

// IPIR type
enum IpirImplType {
  IPIR_STANDARD,
  IPIR_CCC // for CC-G3M 3.0
};

/*!
  @brief IPIRレジスタクラス
 */
class TRegIpir : public TRegModuleVc {
public:
  explicit TRegIpir(CIpir* ipir, PeVec* pes)
      : m_ipir(ipir), m_pes(pes) {
    InitTRegMap();
  }
  ~TRegIpir() { DelTRegMap(); }

  static const uint32_t N_G4MH_CH;
  static const uint32_t IPIEN_OFFSET;
  static const uint32_t IPIFLG_OFFSET;
  static const uint32_t IPIFCLR_OFFSET;
  static const uint32_t IPIREQ_OFFSET;
  static const uint32_t IPIRCLR_OFFSET;

  TRegIPIEN* TrIPIEN(PhysAddr addr) const { return static_cast<TRegIPIEN*>(GetTrPtr(addr)); }
  TRegIPIFLG* TrIPIFLG(PhysAddr addr) const { return static_cast<TRegIPIFLG*>(GetTrPtr(addr)); }
  TRegIPIFCLR* TrIPIFCLR(PhysAddr addr) const { return static_cast<TRegIPIFCLR*>(GetTrPtr(addr)); }
  TRegIPIREQ* TrIPIREQ(PhysAddr addr) const { return static_cast<TRegIPIREQ*>(GetTrPtr(addr)); }
  TRegIPIRCLR* TrIPIRCLR(PhysAddr addr) const { return static_cast<TRegIPIRCLR*>(GetTrPtr(addr)); }

  PhysAddr TrIPIENAddr(uint32_t channel, uint32_t ipir_id) const;
  PhysAddr TrIPIFLGAddr(uint32_t channel, uint32_t ipir_id) const;
  PhysAddr TrIPIFCLRAddr(uint32_t channel, uint32_t ipir_id) const;
  PhysAddr TrIPIREQAddr(uint32_t channel, uint32_t ipir_id) const;
  PhysAddr TrIPIRCLRAddr(uint32_t channel, uint32_t ipir_id) const;

private:
  CIpir* m_ipir;
  PeVec* m_pes;

  void InitTRegMap(void);
};

/*!
  @brief IPIRクラス
 */
class CIpir : public CModuleVc {
private:
  IpirImplType m_impl_type;
  TRegIpir* m_trf;
  PeVec* m_pes;
#if _UNIT_TEST
    FRIEND_TEST (ConvertSelfAddr, ConvertSelfAddr_Regs);
#endif
  PhysAddr ConvertSelfAddr(uint32_t peid, PhysAddr addr);

public:
  // static const PhysAddr IPIR_REG_SIZE = 0x00000F74;
  static const PhysAddr IPIR_REG_SIZE = 0x00001000;
  static const PhysAddr IPIR_REG_ADDR_MASK = IPIR_REG_SIZE - 1;
  static const uint32_t IPIR_S_OFFSET = 0x800;
  static const uint32_t IPIR_PE_OFFSET = 0x100;

  IpirImplType GetImplType(void) const { return m_impl_type; }
  TRegModuleVc* GetTRegFilePtr(void) { return m_trf; }

  void CyclicHandler(void) {}
  SysErrCause TargetWrite(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
  SysErrCause TargetRead(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);
  SysErrCause TargetFetch(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
    return TargetRead(sideband, addr, data, size);
  }
  SysErrCause TargetWriteDebug(PhysAddr addr, uint8_t* data, ByteSize size);
  SysErrCause TargetReadDebug(PhysAddr addr, uint8_t* data, ByteSize size);
  void HardReset(void);

  TRegIPIREQ* GetIPIREQPtr(uint32_t channel, uint32_t ipir_id) const;
  TRegIPIEN* GetIPIENPtr(uint32_t channel, uint32_t ipir_id) const;
  TRegIPIFLG* GetIPIFLGPtr(uint32_t channel, uint32_t ipir_id) const;

  void Set_IPIFLG(uint32_t channel, uint32_t ipir_id, RegData data);
  RegData Get_IPIFLG(uint32_t channel, uint32_t ipir_id);

  void Set_IPIREQ(uint32_t channel, uint32_t ipir_id, RegData data);
  RegData Get_IPIREQ(uint32_t channel, uint32_t ipir_id);

  void Set_IPIEN(uint32_t channel, uint32_t ipir_id, RegData data);
  RegData Get_IPIEN(uint32_t channel, uint32_t ipir_id);

  void SendIntRequest(uint32_t channel, uint32_t ipir_id, uint32_t peid);
  void CancelIntRequest(uint32_t channel, uint32_t ipir_id, uint32_t peid);

  bool IsValidPeId(uint32_t peid) const;

  CIpir(PeVec* pes, IpirImplType type);
  virtual ~CIpir(void) { delete m_trf; }
};
