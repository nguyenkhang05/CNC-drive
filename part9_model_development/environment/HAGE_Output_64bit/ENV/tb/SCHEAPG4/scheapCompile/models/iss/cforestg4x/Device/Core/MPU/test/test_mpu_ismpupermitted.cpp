/**
 * @file test_mpu_ismpupermitted.cpp
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

using ::testing::_;
using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
using ::testing::InSequence;

class MpuMethodCheck
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<MemAccessCommand, bool, bool, bool>> {
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
 * bool IsMpuPermitted<>(PhysAddr vrtl_addr, ByteSize size);
 * @brief Check IsMpuPermitted method.
 */
TEST_P(MpuMethodCheck, CheckIsMpuPermitted) {
  MemAccessCommand cmd = std::get<0>(GetParam());
  bool m_dm = std::get<1>(GetParam());
  bool prm_impdng = std::get<2>(GetParam());
  bool prm_expect = std::get<3>(GetParam());

  // Prologue
  {
    InSequence s;
    // IsMpuRegionHit
    {
      EXPECT_CALL(*(m_mpat_vec[0]), GetWG()).WillOnce(Return(true));
      EXPECT_CALL(*(m_mpat_vec[0]), GetRG()).WillOnce(Return(true));
      EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(true));
      EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(true));
      m_mpu->AddMpuRegion(0);
      m_mpu->AddMpuRegion(1);
    }

#ifdef ENABLE_DBG_BREAK
    EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
    EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(m_dm));
#endif // ENABLE_DBG_BREAK
  }
  // Test Condition
  PhysAddr addr = 0;
  ByteSize size = SIZE_32BIT;
  {
    InSequence s;

    if (m_dm) {
//    EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
//    EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(true));
    } else {
    EXPECT_CALL(*m_srf, SrVSPID()).WillOnce(Return(static_cast<SRegGMSPID*>(m_vspid)));
    EXPECT_CALL(*m_vspid, ReadBody()).WillOnce(Return(0)); // SPID is fixed at 0.
    switch (cmd) {
    case DATA_WRITE:
      EXPECT_CALL(*m_srf, SrHVCFG()).WillOnce(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
      EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(true));
      EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
      EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(true)); // Use UM
      EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
      EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(true)); // Use GM
      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(true)); // Use SVP

      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(true));
      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(true));

      EXPECT_CALL(*m_srf, SrMPCFG()).WillOnce(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
      EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1)); // Host entries are from 1

      if (prm_impdng) {
        // Guest Entry: hit & permit
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[0]), GetUW()).WillOnce(Return(true));
        // Host Entry: hit & permit
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[1]), GetUW()).WillOnce(Return(true));
      } else {
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr + size)); // Guest Entry: not hit
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr + size)); // Host Entry: not hit

        // GetMpuExpCause
        EXPECT_CALL(*m_trace, GetInstId()).WillOnce(Return(0));
        EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
        EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(true));
      }
      break;
    case DATA_READ:
      EXPECT_CALL(*m_srf, SrHVCFG()).WillOnce(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
      EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(true));
      EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
      EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(true)); // Use UM
      EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
      EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(true)); // Use GM
      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(true)); // Use SVP

      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(true));
      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(true));

      EXPECT_CALL(*m_srf, SrMPCFG()).WillOnce(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
      EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1)); // Host entries are from 1

      if (prm_impdng) {
        // Guest Entry: hit & permit
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[0]), GetUR()).WillOnce(Return(true));
        // Host Entry: hit & permit
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[1]), GetUR()).WillOnce(Return(true));
      } else {
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr + size)); // Guest Entry: not hit
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr + size)); // Host Entry: not hit

        // GetMpuExpCause
        EXPECT_CALL(*m_trace, GetInstId()).WillOnce(Return(0));
        EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
        EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(true));
      }
      break;
    case INST_READ:
      EXPECT_CALL(*m_srf, SrHVCFG()).WillOnce(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
      EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(true));
      EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
      EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(true)); // Use UM
      EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
      EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(true)); // Use GM
      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(true)); // Use SVP

      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(true));
      EXPECT_CALL(*m_srf, SrVMPM()).WillOnce(Return(static_cast<SRegGMMPM*>(m_vmpm)));
      EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(true));

      EXPECT_CALL(*m_srf, SrMPCFG()).WillOnce(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
      EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1)); // Host entries are from 1

      if (prm_impdng) {
        // Guest Entry: hit & permit
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        // Host Entry: hit & permit
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(addr));
        EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(addr + size - 1));
        EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
      } else {
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillRepeatedly(Return(addr + size)); // Guest Entry: not hit
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillRepeatedly(Return(addr + size)); // Host Entry: not hit

        // GetMpuExpCause
        EXPECT_CALL(*m_srf, SrVPSW()).WillOnce(Return(static_cast<SRegBasePSW*>(m_vpsw)));
        EXPECT_CALL(*m_vpsw, GetUM()).WillOnce(Return(true));
      }
      break;
    default:
      FAIL() << "We shouldn't get here.";
    }
    }
  }

  // check
  bool res;
  switch (cmd) {
  case DATA_WRITE:
    res = m_mpu->IsMpuPermitted<DATA_WRITE>(addr, size);
    break;
  case DATA_READ:
    res = m_mpu->IsMpuPermitted<DATA_READ>(addr, size);
    break;
  case INST_READ:
    res = m_mpu->IsMpuPermitted<INST_READ>(addr, size);
    break;
  default:
    FAIL() << "We shouldn't get here.";
  }
  ASSERT_EQ(prm_expect, res);
};

std::tuple<MemAccessCommand, bool, bool, bool> const ParametersTable[] = {
  //                         mac,        IsMpuPermittedDefNG, expect
  /* 0000 */ std::make_tuple(INST_READ,  false, true,  true),  // Exit (a)
  /* 0001 */ std::make_tuple(INST_READ,  false, false, false), // Exit (b)
  /* 0002 */ std::make_tuple(DATA_READ,  false, true,  true),  // Exit (a)
  /* 0003 */ std::make_tuple(DATA_READ,  false, false, false), // Exit (b)
  /* 0004 */ std::make_tuple(DATA_WRITE, false, true,  true),  // Exit (a)
  /* 0005 */ std::make_tuple(DATA_WRITE, false, false, false), // Exit (b)
  /* 0006 */ std::make_tuple(INST_READ,  true,  true,  true),  // Exit (a)
  /* 0007 */ std::make_tuple(INST_READ,  true,  false, true),  // Exit (a)
  /* 0008 */ std::make_tuple(DATA_READ,  true,  true,  true),  // Exit (a)
  /* 0009 */ std::make_tuple(DATA_READ,  true,  false, true),  // Exit (a)
  /* 0010 */ std::make_tuple(DATA_WRITE, true,  true,  true),  // Exit (a)
  /* 0011 */ std::make_tuple(DATA_WRITE, true,  false, true),  // Exit (a)
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters, MpuMethodCheck, ::testing::ValuesIn(ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
