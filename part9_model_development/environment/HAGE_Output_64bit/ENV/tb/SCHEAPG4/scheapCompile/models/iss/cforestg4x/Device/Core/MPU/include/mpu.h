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
#include <bitset>

class TraceCtrl;
class SRegFile;
class SRegMPLA_N;
class SRegMPUA_N;
class SRegMPAT_N;
class SRegMPM;

#ifdef _UNIT_TEST
#include "gtest/gtest.h"
#endif

class Mpu {
public:
  enum faultPos { M_GUESTENT, M_HOSTENT, M_NONE };
  template <MemAccessCommand cmd> bool IsMpuPermitted(PhysAddr vrtl_addr, ByteSize size);
  template <MemAccessCommand cmd> bool IsMpuPermittedEI(PhysAddr vrtl_addr, ByteSize size);
  bool IsMpuPermittedRMW(PhysAddr vrtl_addr, ByteSize size);
  void CheckMpuSetting(void);
  void AddMpuRegion(uint32_t rgnid);
  void DelMpuRegion(uint32_t rgnid);
  void AllDelMpuRegion(void);
  uint32_t GetMpuExpCause(void) const { return m_exp_cause; }
  faultPos GetMpuExpPOS(void) const { return m_cur_exp; }
  void SetMpuExpPOS(faultPos s) { m_cur_exp = s; }
  void Reset(void) {
    AllDelMpuRegion();
    m_exp_cause = 0;
  }
  void SetTraceCtrl(TraceCtrl* trace) { m_trace = trace; }
  Mpu(TraceCtrl* trace, SRegFile* srf, CoreType core_type, CoreVersion core_version)
      : m_trace(trace),
        m_srf(srf),
        m_exp_cause(0),
        m_core_type(core_type),
        m_core_version(core_version) {
    m_cur_exp = M_NONE;
    m_vm_supported = ((core_type == CORE_G4MH) && (core_version >= CORE_VER_20));
  }
  ~Mpu() {}

private:
#ifdef _UNIT_TEST
  FRIEND_TEST(MpuMethodCheck, CheckAllDelMpuRegion);
  FRIEND_TEST(MpuMethodCheck, CheckDelMpuRegion);
  FRIEND_TEST(MpuMethodCheck, CheckAddMpuRegion);
  FRIEND_TEST(MpuMethodCheck, CheckUpdatePermitSpid);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuRegionHit);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuRegionPermitted);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuRegionMissAlign);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_1);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_2);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_3);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_4);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_5);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_6);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_7);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_INST_READ_8);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_1);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_2);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_3);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_4);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_5);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_6);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_7);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_READ_8);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_1);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_2);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_3);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_4);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_5);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_6);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_7);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedDefNG_DATA_WRITE_8);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedRMW);
  FRIEND_TEST(MpuMethodCheck, CheckIsMpuPermittedEI);
  FRIEND_TEST(MpuMethodCheck, CheckMpuSetting_a);
  FRIEND_TEST(MpuMethodCheck, CheckMpuSetting_b);
#endif
  TraceCtrl* m_trace;
  SRegFile* m_srf;
  uint32_t m_exp_cause;
  const CoreType m_core_type;
  const CoreVersion m_core_version;
  bool m_vm_supported; // Virtualization support
  faultPos m_cur_exp;

  class MpuRegion {
  private:
#ifdef _UNIT_TEST
	FRIEND_TEST(MpuMethodCheck, CheckUpdatePermitSpid);
    FRIEND_TEST(MpuMethodCheck, CheckIsMpuRegionHit);
  FRIEND_TEST(MpuMethodCheck, CheckMpuSetting_b);
#endif

  public:
    static const uint32_t N_SPID = 32;

  private:
    static const uint32_t N_MPID = 8;
    uint32_t m_rgnid;
    const SRegMPLA_N* m_mpla;
    const SRegMPUA_N* m_mpua;
    const SRegMPAT_N* m_mpat;
    std::bitset<N_SPID> m_write_permit_spid;
    std::bitset<N_SPID> m_read_permit_spid;
    SRegFile* m_srf;

  public:
    void UpdatePermitSpid(void);
    template <MemAccessCommand cmd>
    bool IsMpuRegionHit(PhysAddr vrtl_addr, ByteSize size, uint32_t spid) const;
    bool IsMpuRegionMissAlign(PhysAddr vrtl_addr, ByteSize size) const;
    template <MemAccessCommand cmd> bool IsMpuRegionPermitted(bool um) const;
    bool operator==(const uint32_t rgnid) const { return m_rgnid == rgnid; }
    uint32_t GetMpuRgnID(void) const { return m_rgnid; }
    MpuRegion(uint32_t id, const SRegMPLA_N* la, const SRegMPUA_N* ua, const SRegMPAT_N* at,
              SRegFile* srf)
        : m_rgnid(id), m_mpla(la), m_mpua(ua), m_mpat(at), m_srf(srf) {}
    ~MpuRegion() {}
  };
  typedef std::vector<MpuRegion> MpuRegionVec;
  MpuRegionVec m_mprgn_vec;

  typedef bool DefaultMode;
  static const DefaultMode DEF_OK = true;
  static const DefaultMode DEF_NG = false;

  bool IsEnableDebugMonitor(void) const;
  template <MemAccessCommand cmd> bool IsMpuPermittedDefNG(PhysAddr vrtl_addr, ByteSize size);

  enum MdpType {
    MDP_AT_NORMAL = 0,
    MDP_AT_EIINT = 1,
  };
  uint32_t GetMpuExpCause(PhysAddr vrtl_addr, MemAccessCommand cmd, ByteSize size,
                          MdpType is_eiint) const;
};
