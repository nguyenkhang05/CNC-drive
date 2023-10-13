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
#include <functional>
#include "./forest_common.h"
#include "./module_vc.h"
#include "./treg.h"

template <class TR> class TRegIpirBase : public TRegDataBase<TR> {
private:
  virtual bool IsRegWriteEnable(SideBand sideband) const = 0;

public:
  SysErrCause Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

  TRegIpirBase(ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
      : TRegDataBase<TR>(reg_size, min_access_size, reg_addr, reg_mask) {}

};

class TRegCNTIpirBase : public TRegIpirBase<TRegData8> {
protected:
  // CIpir*  m_ipir;
  bool IsRegWriteEnable(SideBand sideband) const;
  void Reset(void) {}

public:
  TRegCNTIpirBase(ByteSize reg_size, ByteSize min_access_size, PhysAddr reg_addr, TRegMask reg_mask)
      : TRegIpirBase<TRegData8>(reg_size, min_access_size, reg_addr, reg_mask) {}
};

class TRegIPIEN : public TRegCNTIpirBase {
private:
  union {
    TRegData8 m_data;
    struct {
      RegData m_peid0 : 1;
      RegData m_peid1 : 1;
      RegData m_peid2 : 1;
      RegData m_peid3 : 1;
      RegData m_peid4 : 1;
      RegData m_peid5 : 1;
      RegData m_peid6 : 1;
      RegData m_peid7 : 1;
    } m_bit;
  };

  uint32_t m_channel;
  uint32_t m_ipir_id;

public:
  static const uint32_t IPIEN_MASK = 0x000000FFU;
  void Reset(void) {
    m_data.m_data8 = 0x00U;
    TRegCNTIpirBase::Reset();
  }
  void WriteBodyByte(uint32_t pos, uint8_t data) {}
  SysErrCause WriteBody(PhysAddr addr, uint8_t* data, ByteSize size);
  SysErrCause Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

  RegData GetData(void) const { return m_data.m_data8; }
  void SetData(RegData data) { m_data.m_data8 = data & 0xFFU; }

  const char* GetName(void) const { return "IPIEN"; }

  // TRegIPIEN(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel,
  //           uint32_t ipir_id, CModuleVc* tgtvc);
  TRegIPIEN(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel,
            uint32_t ipir_id);
  ~TRegIPIEN(void) {}
};

class TRegIPIFLG : public TRegCNTIpirBase {
private:
  union {
    TRegData8 m_data;
    struct {
      RegData m_peid0 : 1;
      RegData m_peid1 : 1;
      RegData m_peid2 : 1;
      RegData m_peid3 : 1;
      RegData m_peid4 : 1;
      RegData m_peid5 : 1;
      RegData m_peid6 : 1;
      RegData m_peid7 : 1;
    } m_bit;
  };

  uint32_t m_channel;
  uint32_t m_ipir_id;

public:
  static const uint32_t IPIFLG_MASK = 0x000000FFU;
  void Reset(void) {
    m_data.m_data8 = 0x00U;
    TRegCNTIpirBase::Reset();
  }
  void WriteBodyByte(uint32_t pos, uint8_t data) {}
  SysErrCause WriteBody(PhysAddr addr, uint8_t* data, ByteSize size) {
	  if (IsRegAccessSizeValid(size) == false) {
	#if _UNIT_TEST
	  throw std::range_error("Un-guaranteed Size Access");
	#endif
	  PrintAccessSizeWarning();
	  return SYSERR_NONE;
	 }
    CfMsg::DPrint(MSG_WAR, "--:--:--:-: <Warning: Register READONLY write ignored>\n");
    return SYSERR_NONE;
  }
  SysErrCause Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size) {
	 if (IsRegAccessSizeValid(size) == false) {
	#if _UNIT_TEST
	 throw std::range_error("Un-guaranteed Size Access");
	#endif
	  PrintAccessSizeWarning();
	  return SYSERR_NONE;
    }

    CfMsg::DPrint(MSG_WAR, "--:--:--:-: <Warning: Register READONLY write ignored>\n");
    return SYSERR_NONE;
  }

  RegData GetData(void) const { return m_data.m_data8; }
  void SetData(RegData data) { m_data.m_data8 = data & 0xFFU;}
  const char* GetName(void) const { return "IPIFLG"; }

  // TRegIPIFLG(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel,
  //            uint32_t ipir_id, CModuleVc* tgtvc);
  TRegIPIFLG(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel,
             uint32_t ipir_id);
  ~TRegIPIFLG(void) {}
};

class TRegIPIFCLR : public TRegCNTIpirBase {
private:
  union {
    TRegData8 m_data;
    struct {
      RegData m_peid0 : 1;
      RegData m_peid1 : 1;
      RegData m_peid2 : 1;
      RegData m_peid3 : 1;
      RegData m_peid4 : 1;
      RegData m_peid5 : 1;
      RegData m_peid6 : 1;
      RegData m_peid7 : 1;
    } m_bit;
  };

  uint32_t m_channel;
  uint32_t m_ipir_id;

public:
  static const uint32_t IPIFCLR_MASK = 0x000000FFU;
  void Reset(void) {
    m_data.m_data8 = 0x00U;
    TRegCNTIpirBase::Reset();
  }
  void WriteBodyByte(uint32_t pos, uint8_t data) {}
  SysErrCause WriteBody(PhysAddr addr, uint8_t* data, ByteSize size);
  SysErrCause Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

  RegData GetData(void) const { return m_data.m_data8; }
  void SetData(RegData data) { m_data.m_data8 = data & 0xFFU;}

  const char* GetName(void) const { return "IPIFCLR"; }

  // TRegIPIFCLR (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
  //             uint32_t channel, uint32_t ipir_id, CModuleVc* tgtvc);
  TRegIPIFCLR(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel,
              uint32_t ipir_id);
  ~TRegIPIFCLR(void) {}

  std::function<RegData(uint32_t, uint32_t)> Get_IPIFLG;
  std::function<void(uint32_t, uint32_t, RegData)> Set_IPIFLG;
  std::function<void(uint32_t, uint32_t, uint32_t)> CancelIntRequest;
  std::function<RegData(uint32_t, uint32_t)> Get_IPIREQ;
  std::function<void(uint32_t, uint32_t, RegData)> Set_IPIREQ;
};

// uint32_t m_channel;
// uint32_t m_ipir_id;

class TRegIPIREQ : public TRegCNTIpirBase {
private:
  union {
    TRegData8 m_data;
    struct {
      RegData m_peid0 : 1;
      RegData m_peid1 : 1;
      RegData m_peid2 : 1;
      RegData m_peid3 : 1;
      RegData m_peid4 : 1;
      RegData m_peid5 : 1;
      RegData m_peid6 : 1;
      RegData m_peid7 : 1;
    } m_bit;
  };

  uint32_t m_channel;
  uint32_t m_ipir_id;

public:
  static const uint32_t IPIREQ_MASK = 0x000000FFU;
  void Reset(void) {
    m_data.m_data8 = 0x00U;
    TRegCNTIpirBase::Reset();
  }
  void WriteBodyByte(uint32_t pos, uint8_t data) {}
  SysErrCause WriteBody(PhysAddr addr, uint8_t* data, ByteSize size);
  SysErrCause Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

  RegData GetData(void) const { return m_data.m_data8; }
  void SetData(RegData data) { m_data.m_data8 = data & 0xFFU; }

  const char* GetName(void) const { return "IPIREQ"; }

  // TRegIPIREQ (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
  //             uint32_t channel,uint32_t ipir_id, CModuleVc* tgtvc);
  TRegIPIREQ(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel,
             uint32_t ipir_id);
  ~TRegIPIREQ(void) {}

  std::function<RegData(uint32_t, uint32_t)> Get_IPIREQ;
  std::function<void(uint32_t, uint32_t, RegData)> Set_IPIREQ;
  std::function<RegData(uint32_t, uint32_t)> Get_IPIEN;
  std::function<RegData(uint32_t, uint32_t)> Get_IPIFLG;
  std::function<void(uint32_t, uint32_t, RegData)> Set_IPIFLG;
  std::function<void(uint32_t, uint32_t, uint32_t)> SendIntRequest;
};

class TRegIPIRCLR : public TRegCNTIpirBase {
private:
  union {
    TRegData8 m_data;
    struct {
      RegData m_peid0 : 1;
      RegData m_peid1 : 1;
      RegData m_peid2 : 1;
      RegData m_peid3 : 1;
      RegData m_peid4 : 1;
      RegData m_peid5 : 1;
      RegData m_peid6 : 1;
      RegData m_peid7 : 1;
    } m_bit;
  };

  uint32_t m_channel;
  uint32_t m_ipir_id;

public:
  static const uint32_t IPIRCLR_MASK = 0x000000FFU;
  void Reset(void) {
    m_data.m_data8 = 0x00U;
    TRegCNTIpirBase::Reset();
  }
  void WriteBodyByte(uint32_t pos, uint8_t data) {}
  SysErrCause WriteBody(PhysAddr addr, uint8_t* data, ByteSize size);
  SysErrCause Write(SideBand sideband, PhysAddr addr, uint8_t* data, ByteSize size);

  RegData GetData(void) const { return m_data.m_data8; }
  void SetData(RegData data) { m_data.m_data8 = data & 0xFFU; }

  const char* GetName(void) const { return "IPIRCLR"; }

  // TRegIPIRCLR (uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr,
  //             uint32_t channel, uint32_t ipir_id, CModuleVc* tgtvc);
  TRegIPIRCLR(uint32_t reg_size, uint32_t min_access_size, PhysAddr reg_addr, uint32_t channel,
              uint32_t ipir_id);
  ~TRegIPIRCLR(void) {}

  std::function<RegData(uint32_t, uint32_t)> Get_IPIREQ;
  std::function<void(uint32_t, uint32_t, RegData)> Set_IPIREQ;
  std::function<RegData(uint32_t, uint32_t)> Get_IPIEN;
  std::function<RegData(uint32_t, uint32_t)> Get_IPIFLG;
  std::function<void(uint32_t, uint32_t, RegData)> Set_IPIFLG;
  std::function<void(uint32_t, uint32_t, uint32_t)> CancelIntRequest;
};
