/**
 * @file test_mpu_checkmpusetting_ov_set.cpp
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @update by: Hoanh Ho <hoanh.ho.px@rvc.renesas.com>
 * @brief UNIT TEST for MPU.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
// #include <memory>
// #include <stdexcept>
#include <iostream>
#include <stdio.h>
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

class MpuMethodCheck : public testing::Test,
                       public testing::WithParamInterface<std::tuple<RegData, RegData, bool, bool, bool, bool, bool, int32_t>> {
protected:
  MpuMethodCheck(){};
  virtual ~MpuMethodCheck(){};
  virtual void SetUp() {
    m_trace = new MockTraceCtrl(pe_id);
    m_pe = new MockProcElement();
    sreg_dummy = new SRegDUMMY();
    m_ncsreg = new MockNcSRegFile(ctxt_id, m_pe, sreg_dummy);
    m_srf = new MockSRegFile(m_ncsreg);
    m_mcr = new MockSRegMCR(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_MCR, m_mcr);
    m_vmpm = new MockSRegHMMPM(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VMPM, m_vmpm);
    m_mca = new MockSRegMCA(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_MCA, m_mca);
    m_mcs = new MockSRegMCS(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_MCS, m_mcs);
    m_mci = new MockSRegMCI(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_MCI, m_mci);
    m_hvcfg = new MockSRegHVCFG(ctxt_id, m_pe);
    m_srf->SetSrPtr(CF_SR_HVCFG, m_hvcfg);
    m_mpcfg = new MockSRegMPCFG(ctxt_id, m_pe);m_srf->SetSrPtr(CF_SR_MPCFG, m_mpcfg);
    m_vpsw = new MockSRegGMPSW(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VPSW, m_vpsw);
    m_pswh = new MockSRegPSWH(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_PSWH, m_pswh);
    for (int i = 0; i < N_MPU_REGION; i++) {
      m_mpla_vec.push_back(new MockSRegMPLA_N(ctxt_id, m_pe, i, 0));
      m_srf->SetSrPtr(CF_SR_MPLA0 + i, m_mpla_vec.back());
      m_mpua_vec.push_back(new MockSRegMPUA_N(ctxt_id, m_pe, i, 0));
      m_srf->SetSrPtr(CF_SR_MPUA0 + i, m_mpua_vec.back());
      m_mpat_vec.push_back(new MockSRegMPAT_N(ctxt_id, m_pe, i, 0));
      m_srf->SetSrPtr(CF_SR_MPAT0 + i, m_mpat_vec.back());
    }
    m_mpu = new Mpu(m_trace, m_srf, core_type, core_version);
  };
  virtual void TearDown() {
    delete m_mpu;
    delete m_srf;
    delete m_ncsreg;
    delete sreg_dummy;
    delete m_pe;
    delete m_trace;
    delete m_mcr;
    delete m_vmpm;
    delete m_mca;
    delete m_mcs;
    delete m_mci;
    delete m_hvcfg;
    delete m_mpcfg;
    delete m_vpsw;
    delete m_pswh;
    for (MockSRegMPLA_N* element : m_mpla_vec) { delete element; }; m_mpla_vec.clear();
    for (MockSRegMPUA_N* element : m_mpua_vec) { delete element; }; m_mpua_vec.clear();
    for (MockSRegMPAT_N* element : m_mpat_vec) { delete element; }; m_mpat_vec.clear();
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
  MockSRegMCR* m_mcr;
  MockSRegHMMPM* m_vmpm;
  MockSRegMCA* m_mca;
  MockSRegMCS* m_mcs;
  MockSRegMCI* m_mci;

  MockSRegHVCFG* m_hvcfg;
  MockSRegMPCFG* m_mpcfg;

  MockSRegPSWH* m_pswh;
  MockSRegGMPSW* m_vpsw;
  std::vector<MockSRegMPLA_N*> m_mpla_vec;
  std::vector<MockSRegMPUA_N*> m_mpua_vec;
  std::vector<MockSRegMPAT_N*> m_mpat_vec;

private:
};

/*!
 * void CheckMpuSetting(uint32_t rgnid);
 * @brief Check CheckMpuSetting method. (a)
 * TBD: It is necessary to make this test as a separate file and make it with TEST_P
 */
TEST_P(MpuMethodCheck, CheckMpuSetting_a) {
  RegData m_mca_data = std::get<0>(GetParam());
  RegData m_mcs_data = std::get<1>(GetParam());
  bool hve = std::get<2>(GetParam());
  bool gm = std::get<3>(GetParam());
  bool mpe = std::get<4>(GetParam());
  bool gmpe = std::get<5>(GetParam());
  bool m_svp = std::get<6>(GetParam());

  uint32_t expect_mcr =  std::get<7>(GetParam());

  // Prologue
  {
    InSequence s;
    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(hve));
    if (hve) {
      EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
      EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(gm)); // Use GM
    }
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegHMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(mpe));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(m_svp));
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(gmpe));
    EXPECT_CALL(*m_srf, SrMCR()).WillOnce(Return(static_cast<SRegMCR*>(m_mcr)));
    EXPECT_CALL(*m_mcr, WriteBody(0));
  }

  // Test A
  {
    InSequence s;
    EXPECT_CALL(*m_srf, SrMCA()).WillOnce(Return(static_cast<SRegMCA*>(m_mca)));
    EXPECT_CALL(*m_mca, ReadBody()).WillOnce(Return(static_cast<RegData>(m_mca_data)));

    EXPECT_CALL(*m_srf, SrMCS()).WillOnce(Return(static_cast<SRegMCS*>(m_mcs)));
    EXPECT_CALL(*m_mcs, ReadBody()).WillOnce(Return(static_cast<RegData>(m_mcs_data)));

    EXPECT_CALL(*m_mcr, SetOV(true)); // OV is set 1
    EXPECT_CALL(*m_mcr, GetOV()).WillOnce(Return(true));

    // Conventional mode
    if (!hve) {
      if (!gmpe) {
        if (!mpe) {
          //check OV->0, 
          EXPECT_CALL(*m_mcr, SetOV(false));
          EXPECT_CALL(*m_mcr, SetSXE(true));
          EXPECT_CALL(*m_mcr, SetSWE(true));
          EXPECT_CALL(*m_mcr, SetSRE(true));
          EXPECT_CALL(*m_mcr, SetUXE(true));
          EXPECT_CALL(*m_mcr, SetUWE(true));
          EXPECT_CALL(*m_mcr, SetURE(true));
        } else {
          if (!m_svp) {
            EXPECT_CALL(*m_mcr, SetSXE(true));
            EXPECT_CALL(*m_mcr, SetSWE(true));
            EXPECT_CALL(*m_mcr, SetSRE(true));
          }
        }
      }
    } else {
      // Host mode
      if (!gm) {
        if (!mpe) {
          EXPECT_CALL(*m_mcr, SetOV(false));
          EXPECT_CALL(*m_mcr, SetHSXE(true));
          EXPECT_CALL(*m_mcr, SetHSWE(true));
          EXPECT_CALL(*m_mcr, SetHSRE(true));
          EXPECT_CALL(*m_mcr, SetHUXE(true));
          EXPECT_CALL(*m_mcr, SetHUWE(true));
          EXPECT_CALL(*m_mcr, SetHURE(true));
          EXPECT_CALL(*m_mcr, SetGSXE(true));
          EXPECT_CALL(*m_mcr, SetGSWE(true));
          EXPECT_CALL(*m_mcr, SetGSRE(true));
          EXPECT_CALL(*m_mcr, SetGUXE(true));
          EXPECT_CALL(*m_mcr, SetGUWE(true));
          EXPECT_CALL(*m_mcr, SetGURE(true));
          EXPECT_CALL(*m_mcr, SetSXE(true));
          EXPECT_CALL(*m_mcr, SetSWE(true));
          EXPECT_CALL(*m_mcr, SetSRE(true));
          EXPECT_CALL(*m_mcr, SetUXE(true));
          EXPECT_CALL(*m_mcr, SetUWE(true));
          EXPECT_CALL(*m_mcr, SetURE(true));
        } else {
          if (!m_svp) {
            EXPECT_CALL(*m_mcr, SetHSXE(true));
            EXPECT_CALL(*m_mcr, SetHSWE(true));
            EXPECT_CALL(*m_mcr, SetHSRE(true));
            EXPECT_CALL(*m_mcr, SetGSXE(true));
            EXPECT_CALL(*m_mcr, SetGSWE(true));
            EXPECT_CALL(*m_mcr, SetGSRE(true));
            EXPECT_CALL(*m_mcr, SetGUXE(true));
            EXPECT_CALL(*m_mcr, SetGUWE(true));
            EXPECT_CALL(*m_mcr, SetGURE(true));
            EXPECT_CALL(*m_mcr, SetSXE(true));
            EXPECT_CALL(*m_mcr, SetSWE(true));
            EXPECT_CALL(*m_mcr, SetSRE(true));
          } else {
            EXPECT_CALL(*m_mcr, SetGSXE(true));
            EXPECT_CALL(*m_mcr, SetGSWE(true));
            EXPECT_CALL(*m_mcr, SetGSRE(true));
            EXPECT_CALL(*m_mcr, SetGUXE(true));
            EXPECT_CALL(*m_mcr, SetGUWE(true));
            EXPECT_CALL(*m_mcr, SetGURE(true));
          }
        }
      // Guest mode
      } else {
        if (!gmpe) {
          if (!mpe) {
            EXPECT_CALL(*m_mcr, SetOV(false));
            EXPECT_CALL(*m_mcr, SetHSXE(true));
            EXPECT_CALL(*m_mcr, SetHSWE(true));
            EXPECT_CALL(*m_mcr, SetHSRE(true));
            EXPECT_CALL(*m_mcr, SetHUXE(true));
            EXPECT_CALL(*m_mcr, SetHUWE(true));
            EXPECT_CALL(*m_mcr, SetHURE(true));
            EXPECT_CALL(*m_mcr, SetGSXE(true));
            EXPECT_CALL(*m_mcr, SetGSWE(true));
            EXPECT_CALL(*m_mcr, SetGSRE(true));
            EXPECT_CALL(*m_mcr, SetGUXE(true));
            EXPECT_CALL(*m_mcr, SetGUWE(true));
            EXPECT_CALL(*m_mcr, SetGURE(true));
            EXPECT_CALL(*m_mcr, SetSXE(true));
            EXPECT_CALL(*m_mcr, SetSWE(true));
            EXPECT_CALL(*m_mcr, SetSRE(true));
            EXPECT_CALL(*m_mcr, SetUXE(true));
            EXPECT_CALL(*m_mcr, SetUWE(true));
            EXPECT_CALL(*m_mcr, SetURE(true));
          } else {
            if (!m_svp) {
              EXPECT_CALL(*m_mcr, SetHSXE(true));
              EXPECT_CALL(*m_mcr, SetHSWE(true));
              EXPECT_CALL(*m_mcr, SetHSRE(true));
              EXPECT_CALL(*m_mcr, SetHUXE(true));
              EXPECT_CALL(*m_mcr, SetHUWE(true));
              EXPECT_CALL(*m_mcr, SetHURE(true));
              EXPECT_CALL(*m_mcr, SetGSXE(true));
              EXPECT_CALL(*m_mcr, SetGSWE(true));
              EXPECT_CALL(*m_mcr, SetGSRE(true));
              EXPECT_CALL(*m_mcr, SetSXE(true));
              EXPECT_CALL(*m_mcr, SetSWE(true));
              EXPECT_CALL(*m_mcr, SetSRE(true));
            } else {
              EXPECT_CALL(*m_mcr, SetHSXE(true));
              EXPECT_CALL(*m_mcr, SetHSWE(true));
              EXPECT_CALL(*m_mcr, SetHSRE(true));
              EXPECT_CALL(*m_mcr, SetHUXE(true));
              EXPECT_CALL(*m_mcr, SetHUWE(true));
              EXPECT_CALL(*m_mcr, SetHURE(true));
            }
          }
        } else {
          if (!mpe) {
            EXPECT_CALL(*m_mcr, SetGSXE(true));
            EXPECT_CALL(*m_mcr, SetGSWE(true));
            EXPECT_CALL(*m_mcr, SetGSRE(true));
            EXPECT_CALL(*m_mcr, SetGUXE(true));
            EXPECT_CALL(*m_mcr, SetGUWE(true));
            EXPECT_CALL(*m_mcr, SetGURE(true));
          } else {
            if (!m_svp) {
              EXPECT_CALL(*m_mcr, SetGSXE(true));
              EXPECT_CALL(*m_mcr, SetGSWE(true));
              EXPECT_CALL(*m_mcr, SetGSRE(true));
            }
          }
        }
      }
    }
    EXPECT_CALL(*m_mcr, ReadBody()).Times(2);
  }

  // check
  m_mpu->CheckMpuSetting();
  ASSERT_EQ(expect_mcr, m_mcr->ReadBody());
}
    std::tuple<RegData, RegData, bool, bool, bool, bool, bool, int> const ParametersTable_a[] = {
                              /*         mca              mcs                hve     gm     mpe    gmpe   svp     mcr   */
      /* 0000 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,   true,  true,  true,  0x00000000),
      /* 0001 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,   true,  true, false,  0x00000000),
      /* 0002 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,   true, false,  true,  0x00000000),
      /* 0003 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,   true, false, false,  0x00000000),
      /* 0004 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,  false,  true,  true,  0x00000000),
      /* 0005 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,  false,  true, false,  0x00000000),
      /* 0006 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,  false, false,  true,  0x00000000),
      /* 0007 */ std::make_tuple(              0x0, 0x80000000U + 1,         false,  true,  false, false, false,  0x00000000),
      /* 0008 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,   true,  true,  true,  0x00000000),
      /* 0009 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,   true,  true, false,  0x00000000),
      /* 0010 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,   true, false,  true,  0x00000000),
      /* 0011 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,   true, false, false,  0x00000000),
      /* 0012 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,  false,  true,  true,  0x00000000),
      /* 0013 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,  false,  true, false,  0x00000000),
      /* 0014 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,  false, false,  true,  0x00000000),
      /* 0015 */ std::make_tuple(              0x2, 0x80000000U + 1,         false, false,  false, false, false,  0x00000000),
      /* 0016 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,  false, false, false,  0x00000000),
      /* 0017 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,  false, false,  true,  0x00000000),
      /* 0018 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,  false,  true, false,  0x00000000),
      /* 0019 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,  false,  true,  true,  0x00000000),
      /* 0020 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,   true, false, false,  0x00000000),
      /* 0021 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,   true, false,  true,  0x00000000),
      /* 0022 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,   true,  true, false,  0x00000000),
      /* 0023 */ std::make_tuple(              0x1, 0x80000000U + 1,          true, false,   true,  true,  true,  0x00000000),
      /* 0024 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,  false, false, false,  0x00000000),
      /* 0025 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,  false, false,  true,  0x00000000),
      /* 0026 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,  false,  true, false,  0x00000000),
      /* 0027 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,  false,  true,  true,  0x00000000),
      /* 0028 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,   true, false, false,  0x00000000),
      /* 0029 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,   true, false,  true,  0x00000000),
      /* 0030 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,   true,  true, false,  0x00000000),
      /* 0031 */ std::make_tuple(              0x3, 0x80000000U + 1,          true,  true,   true,  true,  true,  0x00000000),
      /* 0032 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,   true,  true,  true,  0x00000000),
      /* 0033 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,   true,  true, false,  0x00000000),
      /* 0034 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,   true, false,  true,  0x00000000),
      /* 0035 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,   true, false, false,  0x00000000),
      /* 0036 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,  false,  true,  true,  0x00000000),
      /* 0037 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,  false,  true, false,  0x00000000),
      /* 0038 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,  false, false,  true,  0x00000000),
      /* 0039 */ std::make_tuple(      0xFFFFFFFEU,               3,         false,  true,  false, false, false,  0x00000000),
      /* 0040 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,   true,  true,  true,  0x00000000),
      /* 0041 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,   true,  true, false,  0x00000000),
      /* 0042 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,   true, false,  true,  0x00000000),
      /* 0043 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,   true, false, false,  0x00000000),
      /* 0044 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,  false,  true,  true,  0x00000000),
      /* 0045 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,  false,  true, false,  0x00000000),
      /* 0046 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,  false, false,  true,  0x00000000),
      /* 0047 */ std::make_tuple(      0xFFFFFFFEU,               3,         false, false,  false, false, false,  0x00000000),
      /* 0048 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,  false, false, false,  0x00000000),
      /* 0049 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,  false, false,  true,  0x00000000),
      /* 0050 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,  false,  true, false,  0x00000000),
      /* 0051 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,  false,  true,  true,  0x00000000),
      /* 0052 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,   true, false, false,  0x00000000),
      /* 0053 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,   true, false,  true,  0x00000000),
      /* 0054 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,   true,  true, false,  0x00000000),
      /* 0055 */ std::make_tuple(      0xFFFFFFFEU,               3,          true, false,   true,  true,  true,  0x00000000),
      /* 0056 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,  false, false, false,  0x00000000),
      /* 0057 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,  false, false,  true,  0x00000000),
      /* 0058 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,  false,  true, false,  0x00000000),
      /* 0059 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,  false,  true,  true,  0x00000000),
      /* 0060 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,   true, false, false,  0x00000000),
      /* 0061 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,   true, false,  true,  0x00000000),
      /* 0062 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,   true,  true, false,  0x00000000),
      /* 0063 */ std::make_tuple(      0xFFFFFFFEU,               3,          true,  true,   true,  true,  true,  0x00000000),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters_a, MpuMethodCheck,
                        ::testing::ValuesIn(ParametersTable_a));


int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
