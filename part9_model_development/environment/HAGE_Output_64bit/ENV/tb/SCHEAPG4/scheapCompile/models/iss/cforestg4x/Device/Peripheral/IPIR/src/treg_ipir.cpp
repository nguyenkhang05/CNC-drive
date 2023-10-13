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
#include "treg_ipir.h"
#if _UNIT_TEST
#include <stdexcept>
#include "gmock/gmock.h"
#endif

template <class TR>
SysErrCause TRegIpirBase<TR>::Write(SideBand sideband, PhysAddr addr, uint8_t* data,
                                    ByteSize size) {
  if (IsRegWriteEnable(sideband) == false) {
#if _UNIT_TEST
    //    throw std::invalid_arguement("RegWriteEnable is false");
#endif
    return SYSERR_NONE;
  }
  if (this->IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
     throw std::range_error("Un-guaranteed Size Access");
#endif
    this->PrintAccessSizeWarning();
    return SYSERR_NONE;
  }
  return this->WriteBody(addr, data, size);
}

bool TRegCNTIpirBase::IsRegWriteEnable(SideBand sideband) const { return true; }

TRegIPIEN::TRegIPIEN(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                     uint32_t channel, uint32_t ipir_id /* , CModuleVc* tgtvc */)
    : TRegCNTIpirBase(reg_size, min_access_size, reg_addr, TRegMask(IPIEN_MASK)),
      m_channel(channel),
      m_ipir_id(ipir_id) {
  m_treg = &m_data;
  // m_ipir = static_cast<CIpir*> (tgtvc);
}

SysErrCause TRegIPIEN::WriteBody(PhysAddr addr, uint8_t* data, ByteSize size) {
  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
	PrintAccessSizeWarning();
	return SYSERR_NONE;
  }
  uint8_t wdata = *data;
  SetData(wdata);
  return SYSERR_NONE;
}

SysErrCause TRegIPIEN::Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
  if (IsRegWriteEnable(sideband) == false) {
    return SYSERR_NONE; // unreached code
  }
  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
     throw std::range_error("Un-guaranteed Size Access");
#endif
    PrintAccessSizeWarning();
    return SYSERR_NONE;
  }
  return WriteBody(addr, data, size);


}

TRegIPIFLG::TRegIPIFLG(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                       uint32_t channel, uint32_t ipir_id /* , CModuleVc* tgtvc */)
    : TRegCNTIpirBase(reg_size, min_access_size, reg_addr, TRegMask(IPIFLG_MASK)),
      m_channel(channel),
      m_ipir_id(ipir_id) {
  m_treg = &m_data;
  // m_ipir = static_cast<CIpir*> (tgtvc);
}

TRegIPIFCLR::TRegIPIFCLR(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                         uint32_t channel, uint32_t ipir_id /* , CModuleVc* tgtvc */)
    : TRegCNTIpirBase(reg_size, min_access_size, reg_addr, TRegMask(IPIFCLR_MASK)),
      m_channel(channel),
      m_ipir_id(ipir_id) {
  m_treg = &m_data;
  // m_ipir = static_cast<CIpir*> (tgtvc);
}

SysErrCause TRegIPIFCLR::WriteBody(PhysAddr addr, uint8_t* data, ByteSize size) {

  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
	PrintAccessSizeWarning();
	return SYSERR_NONE;
 }
  uint8_t wdata = *data;

  if (wdata != 0) {
    // clear IPInFLGm[x] = 0:   x position = 1 in wdata
    RegData ipiflg_data = this->Get_IPIFLG(m_channel, m_ipir_id);
    RegData clear_data = ~(wdata & 0xFFU) & ipiflg_data;
    this->Set_IPIFLG(m_channel, m_ipir_id, clear_data);
    // cancel interrupt request due to clear IPInFLGm[x] = 0
	  for (uint32_t i = 0; i < 8; i++) {
	    if (((ipiflg_data >> i) == 1) && ((clear_data >> i) == 0)) {
	      CancelIntRequest(m_channel, m_ipir_id, i);
	    }
	  }

    // clear IPInREQx[m] = 0:   x position = 1 in wdata, m: ipir_id
	  for (uint32_t i = 0; i < 8; i++) {
	    if ((wdata >> i) & 0x1U) {
	      // RegData ipireq_data = m_ipir->Get_IPIREQ(m_channel, i);
	      // m_ipir->Set_IPIREQ(m_channel, i, ~((1 << m_ipir_id) & 0xFFU) & ipireq_data);
	      RegData ipireq_data = this->Get_IPIREQ(m_channel, i);
	      this->Set_IPIREQ(m_channel, i, ~((1 << m_ipir_id) & 0xFFU) & ipireq_data);
	    }
      }
   }
   return SYSERR_NONE;
}
SysErrCause TRegIPIFCLR::Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
  if (IsRegWriteEnable(sideband) == false) {
    return SYSERR_NONE;
  }
  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
     throw std::range_error("Un-guaranteed Size Access");
#endif
    PrintAccessSizeWarning();
    return SYSERR_NONE;
  }

  return WriteBody(addr, data, size);
}

TRegIPIREQ::TRegIPIREQ(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                       uint32_t channel, uint32_t ipir_id /* , CModuleVc* tgtvc */)
    : TRegCNTIpirBase(reg_size, min_access_size, reg_addr, TRegMask(IPIREQ_MASK)),
      m_channel(channel),
      m_ipir_id(ipir_id) {
  m_treg = &m_data;
  // m_ipir = static_cast<CIpir*> (tgtvc);
}

SysErrCause TRegIPIREQ::WriteBody(PhysAddr addr, uint8_t* data, ByteSize size) {
  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
	PrintAccessSizeWarning();
	return SYSERR_NONE;
  }

  uint8_t wdata = *data;

  // if IPInENx[m] = 1 --> IPInFLGx[m] = 1    : x position = 1 in wdata, m: ipir_id
  for (uint32_t i = 0; i < 8; i++) {
    // check IPInREQm[x] = 1 ?
    if ((wdata >> i) & 0x1U) {
      // set IPIREQ
      RegData ipireq_data = this->Get_IPIREQ(m_channel, m_ipir_id);
      this->Set_IPIREQ(m_channel, m_ipir_id, ((1 << i) & 0xFFU) | ipireq_data);
      RegData ipien_data = this->Get_IPIEN(m_channel, i);
      if (((ipien_data >> m_ipir_id) & 0x1U) && // check IPInENx[m] = 1 ?
          (((ipireq_data >> i) & 0x01U) == 0)) { // the last int req doesn't remain
        // //set IPInFLGx[m] = 1
        RegData ipiflg_data = this->Get_IPIFLG(m_channel, i);
        this->Set_IPIFLG(m_channel, i, ((1 << m_ipir_id) & 0xFFU) | ipiflg_data);
        SendIntRequest(m_channel, i, m_ipir_id);
      }
    }

  }
  return SYSERR_NONE;
}
SysErrCause TRegIPIREQ::Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
  if (IsRegWriteEnable(sideband) == false) {
    return SYSERR_NONE;
  }
  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
     throw std::range_error("Un-guaranteed Size Access");
#endif
    PrintAccessSizeWarning();
    return SYSERR_NONE;
  }

  return WriteBody(addr, data, size);
}

TRegIPIRCLR::TRegIPIRCLR(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
                         uint32_t channel, uint32_t ipir_id /* , CModuleVc* tgtvc */)
    : TRegCNTIpirBase(reg_size, min_access_size, reg_addr, TRegMask(IPIRCLR_MASK)),
      m_channel(channel),
      m_ipir_id(ipir_id) {
  m_treg = &m_data;
  // m_ipir = static_cast<CIpir*> (tgtvc);
}

SysErrCause TRegIPIRCLR::WriteBody(PhysAddr addr, uint8_t* data, ByteSize size) {
  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
   throw std::range_error("Un-guaranteed Size Access");
#endif
	PrintAccessSizeWarning();
	return SYSERR_NONE;
  }

  uint8_t wdata = *data;
  if (wdata != 0) {
    // clear IPInREQm[x] = 0
    RegData ipireq_data = this->Get_IPIREQ(m_channel, m_ipir_id);
    this->Set_IPIREQ(m_channel, m_ipir_id, (~((wdata)&0xFFU) & ipireq_data));
    // if IPInENx[m] = 1 --> IPInFLGx[m] = 0    : x position = 1 in wdata, m: ipir_id
    for (uint32_t i = 0; i < 8; i++) {
      if ((wdata >> i) & 0x1U) { // IPInRCLRm[x] = 1
        // RegData ipien_data = m_ipir->Get_IPIEN(m_channel, i);
        RegData ipien_data = this->Get_IPIEN(m_channel, i);
        if ((ipien_data >> m_ipir_id) & 0x1U) { // IPInENx[m] = 1
          // //IPInFLGx[m] = 0
          RegData ipiflg_data = this->Get_IPIFLG(m_channel, i);
          this->Set_IPIFLG(m_channel, i,
                           ~((1 << m_ipir_id) & 0xFFU) & ipiflg_data); // IPInFLGx[m] = 0
          CancelIntRequest(m_channel, i, m_ipir_id);
        }
      }
    }
  }
  return SYSERR_NONE;
}
SysErrCause TRegIPIRCLR::Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
  if (IsRegWriteEnable(sideband) == false) {
    return SYSERR_NONE;
  }
  if (IsRegAccessSizeValid(size) == false) {
#if _UNIT_TEST
     throw std::range_error("Un-guaranteed Size Access");
#endif
    PrintAccessSizeWarning();
    return SYSERR_NONE;
  }

  return WriteBody(addr, data, size);
 }
