/**
 * @file test_mpu_ismpupermittedrmw.cpp
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for MPU.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
// #include <memory>
// #include <stdexcept>
#include "config.h"
#include "forest_common.h"
#include "gmock/gmock.h"
#include "mock_proc_element.h"
#include "mock_sreg.h"
#include "mock_sregfile.h"
#include "mock_trace_info.h"
#include "mpu.h"

// using ::testing::_;
using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
using ::testing::InSequence;

class MpuMethodCheck : public testing::Test,
                       public testing::WithParamInterface<
  std::tuple<bool, bool, bool, bool, bool, bool, uint32_t, Mpu::faultPos>> {
protected:
  MpuMethodCheck(){};
  virtual ~MpuMethodCheck(){};
  virtual void SetUp() {
    m_trace = new MockTraceCtrl(pe_id);
    m_pe = new MockProcElement();
    sreg_dummy = new SRegDUMMY();
    m_ncsreg = new MockNcSRegFile(ctxt_id, m_pe, sreg_dummy);
    m_srf = new MockSRegFile(m_ncsreg);
    for (int i = 0; i < N_MPU_REGION; i++) {
      m_mpla_vec.push_back(new MockSRegMPLA_N(ctxt_id, m_pe, i, 0));
      m_srf->SetSrPtr(CF_SR_MPLA0 + i, m_mpla_vec.back());
      m_mpua_vec.push_back(new MockSRegMPUA_N(ctxt_id, m_pe, i, 0));
      m_srf->SetSrPtr(CF_SR_MPUA0 + i, m_mpua_vec.back());
      m_mpat_vec.push_back(new MockSRegMPAT_N(ctxt_id, m_pe, i, 0));
      m_srf->SetSrPtr(CF_SR_MPAT0 + i, m_mpat_vec.back());
    }
    m_dir0 = new MockSRegDIR0(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_DIR0, m_dir0);
    m_hvcfg = new MockSRegHVCFG(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_HVCFG, m_hvcfg);
    m_pswh = new MockSRegPSWH(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_PSWH, m_pswh);
    m_vmpcfg = new MockSRegMPCFG(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_MPCFG, m_vmpcfg);
    m_vmpm = new MockSRegGMMPM(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VMPM, m_vmpm);
    m_vpsw = new MockSRegGMPSW(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VPSW, m_vpsw);
    m_vspid = new MockSRegGMSPID(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VSPID, m_vspid);
    m_mpu = new Mpu(m_trace, m_srf, core_type, core_version);
  };
  virtual void TearDown() {
    delete m_mpu;
    delete m_srf;
    delete m_ncsreg;
    delete sreg_dummy;
    delete m_pe;
    delete m_trace;
    for (MockSRegMPLA_N* element : m_mpla_vec) { delete element; }; m_mpla_vec.clear();
    for (MockSRegMPUA_N* element : m_mpua_vec) { delete element; }; m_mpua_vec.clear();
    for (MockSRegMPAT_N* element : m_mpat_vec) { delete element; }; m_mpat_vec.clear();
    delete m_dir0;
    delete m_hvcfg;
    delete m_pswh;
    delete m_vmpcfg;
    delete m_vmpm;
    delete m_vpsw;
    delete m_vspid;
  };

public:
  // System Settings
  CoreType core_type = CORE_G4MH;
  CoreVersion core_version = CORE_VER_20; // Make a core version 1.1 unit test

  // Test object
  Mpu* m_mpu;

  // Trace Mock
  uint32_t pe_id = 0;
  MockTraceCtrl* m_trace;

  // PE Mock
  MockProcElement* m_pe;

  // NcSRegFile Mock
  ContextId ctxt_id = 0;
  SRegDUMMY* sreg_dummy;
  MockNcSRegFile* m_ncsreg;

  // SRegFile Mock
  MockSRegFile* m_srf;
  std::vector<MockSRegMPLA_N*> m_mpla_vec;
  std::vector<MockSRegMPUA_N*> m_mpua_vec;
  std::vector<MockSRegMPAT_N*> m_mpat_vec;
  MockSRegDIR0* m_dir0;
  MockSRegMPCFG* m_vmpcfg;
  MockSRegGMMPM* m_vmpm;
  MockSRegGMPSW* m_vpsw;
  MockSRegGMSPID* m_vspid;
  MockSRegHVCFG* m_hvcfg;
  MockSRegPSWH* m_pswh;

private:
};

/*!
 * bool IsMpuPermittedRMW<>(PhysAddr vrtl_addr, ByteSize size);
 * @brief Check IsMpuPermittedRMW method.
 */
TEST_P(MpuMethodCheck, CheckIsMpuPermittedRMW) {
  bool is_g_read_ok = std::get<0>(GetParam());
  bool is_h_read_ok = std::get<1>(GetParam());
  bool is_g_write_ok = std::get<2>(GetParam());
  bool is_h_write_ok = std::get<3>(GetParam());
  bool um = std::get<4>(GetParam());
  bool expect = std::get<5>(GetParam());
  uint32_t expect_cause = std::get<6>(GetParam());
  Mpu::faultPos fp = std::get<7>(GetParam());

  // Test Condition
  PhysAddr addr = 0;
  ByteSize size = SIZE_32BIT;

  InSequence s;
  {
    // Dummy setup
    if (is_g_write_ok) {
      EXPECT_CALL(*(m_mpat_vec[0]), GetWG()).WillOnce(Return(true));
    } else {
      EXPECT_CALL(*(m_mpat_vec[0]), GetWG()).WillOnce(Return(false));
      EXPECT_CALL(*(m_mpat_vec[0]), GetWMPID()).WillOnce(Return(0)); // 0b0 -> bit count is 0
    }
    if (is_g_read_ok) {
      EXPECT_CALL(*(m_mpat_vec[0]), GetRG()).WillOnce(Return(true));
    } else {
      EXPECT_CALL(*(m_mpat_vec[0]), GetRG()).WillOnce(Return(false));
      EXPECT_CALL(*(m_mpat_vec[0]), GetRMPID()).WillOnce(Return(0)); // 0b0 -> bit count is 0
    }
    if (is_h_write_ok) {
      EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(true));
    } else {
      EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(false));
      EXPECT_CALL(*(m_mpat_vec[1]), GetWMPID()).WillOnce(Return(0)); // 0b0 -> bit count is 0
    }
    if (is_h_read_ok) {
      EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(true));
    } else {
      EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(false));
      EXPECT_CALL(*(m_mpat_vec[1]), GetRMPID()).WillOnce(Return(0)); // 0b0 -> bit count is 0
    }
    m_mpu->AddMpuRegion(0);
    m_mpu->AddMpuRegion(1);
  }
  // val: is_read_ok
  {
    // IsMpuPermitted
#ifdef ENABLE_DBG_BREAK
    EXPECT_CALL(*m_srf, SrDIR0()).WillRepeatedly(Return(static_cast<SRegDIR0*>(m_dir0)));
    EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(false));
#endif // ENABLE_DBG_BREAK

    // IsMpuPermittedDefNG
    EXPECT_CALL(*m_srf, SrVSPID()).WillRepeatedly(Return(static_cast<SRegGMSPID*>(m_vspid)));
    EXPECT_CALL(*m_vspid, ReadBody()).WillOnce(Return(0)); // SPID is fixed at 0.
    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(true));
    EXPECT_CALL(*m_srf, SrVPSW()).WillRepeatedly(Return(static_cast<SRegBasePSW*>(m_vpsw)));
    EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(um)); // Use UM
    EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
    EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(true)); // Use GM
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(true)); // Use SVP

    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(true));
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(true));

    EXPECT_CALL(*m_srf, SrMPCFG()).WillRepeatedly(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
    EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1)); // Host entries are from 1

    // MpuRegion::IsMpuRegionHit
    if (is_g_read_ok) {
      // Guest Entry: hit & permit
      EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr));
      EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(addr + size - 1));
      if (um) {
        EXPECT_CALL(*(m_mpat_vec[0]), GetUR()).WillOnce(Return(true));
      } else {
        EXPECT_CALL(*(m_mpat_vec[0]), GetSR()).WillOnce(Return(true));
      }
    } else {
      // Guest Entry: not hit
      EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr + size));
    }
    if (is_h_read_ok) {
      // Host Entry: hit & permit
      EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr));
      EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(addr + size - 1));
      if (um) {
        EXPECT_CALL(*(m_mpat_vec[1]), GetUR()).WillOnce(Return(true));
      } else {
        EXPECT_CALL(*(m_mpat_vec[1]), GetSR()).WillOnce(Return(true));
      }
    } else {
      // Host Entry: not hit
      EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr + size));
    }
    // GetMpuExpCause: access not granted
    if (!(is_g_read_ok && is_h_read_ok)) {
      EXPECT_CALL(*m_trace, GetInstId()).WillOnce(Return(0));
      EXPECT_CALL(*m_srf, SrVPSW()).WillRepeatedly(Return(static_cast<SRegBasePSW*>(m_vpsw)));
      EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(um));
    }
  }

  // val: is_write_ok
  {
    // IsMpuPermitted
#ifdef ENABLE_DBG_BREAK
    EXPECT_CALL(*m_srf, SrDIR0()).WillRepeatedly(Return(static_cast<SRegDIR0*>(m_dir0)));
    EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(false));
#endif // ENABLE_DBG_BREAK

    // IsMpuPermittedDefNG
    EXPECT_CALL(*m_srf, SrVSPID()).WillRepeatedly(Return(static_cast<SRegGMSPID*>(m_vspid)));
    EXPECT_CALL(*m_vspid, ReadBody()).WillOnce(Return(0)); // SPID is fixed at 0.
    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(true));
    EXPECT_CALL(*m_srf, SrVPSW()).WillRepeatedly(Return(static_cast<SRegBasePSW*>(m_vpsw)));
    EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(um)); // Use UM
    EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
    EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(true)); // Use GM
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(true)); // Use SVP

    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(true));
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegGMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(true));

    EXPECT_CALL(*m_srf, SrMPCFG()).WillRepeatedly(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
    EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1)); // Host entries are from 1

    // MpuRegion::IsMpuRegionHit
    if (is_g_write_ok) {
      // Guest: hit & permit
      EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr));
      EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(addr + size - 1));
      if (um) {
        EXPECT_CALL(*(m_mpat_vec[0]), GetUW()).WillOnce(Return(true));
      } else {
        EXPECT_CALL(*(m_mpat_vec[0]), GetSW()).WillOnce(Return(true));
      }
    } else {
      EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillRepeatedly(Return(addr + size));
    }
    if (is_h_write_ok) {
      // Host: hit & permit
      EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr));
      EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(addr + size - 1));
      if (um) {
        EXPECT_CALL(*(m_mpat_vec[1]), GetUW()).WillOnce(Return(true));
      } else {
        EXPECT_CALL(*(m_mpat_vec[1]), GetSW()).WillOnce(Return(true));
      }
    } else {
      // Host: not hit
      EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillRepeatedly(Return(addr + size));
    }
    // GetMpuExpCause : access not granted
    if (!(is_g_write_ok && is_h_write_ok)) {
      EXPECT_CALL(*m_trace, GetInstId()).WillOnce(Return(0));
      EXPECT_CALL(*m_srf, SrVPSW()).WillRepeatedly(Return(static_cast<SRegBasePSW*>(m_vpsw)));
      EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(um));
    }
  }

  // check
  bool res;
  res = m_mpu->IsMpuPermittedRMW(addr, size);
  ASSERT_EQ(expect, res);
  ASSERT_EQ(expect_cause, m_mpu->m_exp_cause);
  ASSERT_EQ(fp, m_mpu->m_cur_exp);
};

std::tuple<bool, bool, bool, bool, bool, bool, uint32_t, Mpu::faultPos> const ParametersTable[] = {
                          // read guest ok, read host ok, write guest ok, write host ok, um,    expect, expect_cause,                faultPos
  /* 0000 */ std::make_tuple(false,         false,         false,         false,         true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0001 */ std::make_tuple(false,         false,         true,          false,         true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0002 */ std::make_tuple(false,         false,         false,         true,          true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0003 */ std::make_tuple(false,         false,         true,          true,          true,  false,  MDP_GUEST|MDP_U_R,           Mpu::M_GUESTENT),
  /* 0004 */ std::make_tuple(false,         false,         false,         false,         false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0005 */ std::make_tuple(false,         false,         true,          false,         false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0006 */ std::make_tuple(false,         false,         false,         true,          false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0007 */ std::make_tuple(false,         false,         true,          true,          false, false,  MDP_GUEST|MDP_S_R,           Mpu::M_GUESTENT),

  /* 0008 */ std::make_tuple(true,          false,         false,         false,         true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0009 */ std::make_tuple(true,          false,         true,          false,         true,  false,  MDP_HOST|MDP_U_R|MDP_U_W,    Mpu::M_HOSTENT),
  /* 0010 */ std::make_tuple(true,          false,         false,         true,          true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0011 */ std::make_tuple(true,          false,         true,          true,          true,  false,  MDP_HOST|MDP_U_R,            Mpu::M_HOSTENT),
  /* 0012 */ std::make_tuple(true,          false,         false,         false,         false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0013 */ std::make_tuple(true,          false,         true,          false,         false, false,  MDP_HOST|MDP_S_R|MDP_S_W,    Mpu::M_HOSTENT),
  /* 0014 */ std::make_tuple(true,          false,         false,         true,          false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0015 */ std::make_tuple(true,          false,         true,          true,          false, false,  MDP_HOST|MDP_S_R,            Mpu::M_HOSTENT),

  /* 0016 */ std::make_tuple(false,         true,          false,         false,         true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0017 */ std::make_tuple(false,         true,          true,          false,         true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0018 */ std::make_tuple(false,         true,          false,         true,          true,  false,  MDP_GUEST|MDP_U_R|MDP_U_W,   Mpu::M_GUESTENT),
  /* 0019 */ std::make_tuple(false,         true,          true,          true,          true,  false,  MDP_GUEST|MDP_U_R,           Mpu::M_GUESTENT),
  /* 0020 */ std::make_tuple(false,         true,          false,         false,         false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0021 */ std::make_tuple(false,         true,          true,          false,         false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0022 */ std::make_tuple(false,         true,          false,         true,          false, false,  MDP_GUEST|MDP_S_R|MDP_S_W,   Mpu::M_GUESTENT),
  /* 0023 */ std::make_tuple(false,         true,          true,          true,          false, false,  MDP_GUEST|MDP_S_R,           Mpu::M_GUESTENT),

  /* 0024 */ std::make_tuple(true,          true,          false,         false,         true,  false,  MDP_GUEST|MDP_U_W,           Mpu::M_GUESTENT),
  /* 0025 */ std::make_tuple(true,          true,          true,          false,         true,  false,  MDP_HOST|MDP_U_W,            Mpu::M_HOSTENT),
  /* 0026 */ std::make_tuple(true,          true,          false,         true,          true,  false,  MDP_GUEST|MDP_U_W,           Mpu::M_GUESTENT),
  /* 0027 */ std::make_tuple(true,          true,          true,          true,          true,  true,   MDP_NONE,                    Mpu::M_NONE),
  /* 0028 */ std::make_tuple(true,          true,          false,         false,         false, false,  MDP_GUEST|MDP_S_W,           Mpu::M_GUESTENT),
  /* 0029 */ std::make_tuple(true,          true,          true,          false,         false, false,  MDP_HOST|MDP_S_W,            Mpu::M_HOSTENT),
  /* 0030 */ std::make_tuple(true,          true,          false,         true,          false, false,  MDP_GUEST|MDP_S_W,           Mpu::M_GUESTENT),
  /* 0031 */ std::make_tuple(true,          true,          true,          true,          false, true,   MDP_NONE,                    Mpu::M_NONE),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters, MpuMethodCheck, ::testing::ValuesIn(ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
