/**
 * @file test_mpu_checkmpusetting_ov_is_zero.cpp
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @update by: Hoanh Ho <hoanh.ho.px@rvc.renesas.com>
 * @brief UNIT TEST for MPU.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
// #include <memory>
// #include <stdexcept>
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
                       public testing::WithParamInterface<std::tuple<bool, bool, bool, bool, bool, uint32_t>> {
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
    m_vmpcfg = new MockSRegMPCFG(ctxt_id, m_pe);m_srf->SetSrPtr(CF_SR_MPCFG, m_vmpcfg);
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
    delete m_vmpcfg;
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
  MockSRegMPCFG* m_vmpcfg;

  MockSRegPSWH* m_pswh;
  MockSRegGMPSW* m_vpsw;
  std::vector<MockSRegMPLA_N*> m_mpla_vec;
  std::vector<MockSRegMPUA_N*> m_mpua_vec;
  std::vector<MockSRegMPAT_N*> m_mpat_vec;

private:
};

/*!
 * void CheckMpuSetting(uint32_t rgnid);
 * @brief Check CheckMpuSetting method. (b)
 * TBD: It is necessary to make this test as a separate file and make it with TEST_P
 */
TEST_P(MpuMethodCheck, CheckCheckMpuSetting_b_1) {
  bool hve = std::get<0>(GetParam());
  bool gm = std::get<1>(GetParam());
  bool mpe = std::get<2>(GetParam());
  bool gmpe = std::get<3>(GetParam());
  bool svp = std::get<4>(GetParam());

  uint32_t expect_mcr =  std::get<5>(GetParam());

  // Prologue
  {
    // IsMpuRegionHit
    EXPECT_CALL(*(m_mpat_vec[0]), GetWG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[0]), GetRG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(true));
    m_mpu->AddMpuRegion(0);
    m_mpu->AddMpuRegion(1);
  }
  {
    InSequence dummy;
    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(hve));
    if (hve) {
      EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
      EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(gm)); // Use GM
    }
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegHMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(mpe));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(svp)); // mpm_svp == 1
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(gmpe));
    EXPECT_CALL(*m_srf, SrMCR()).WillOnce(Return(static_cast<SRegMCR*>(m_mcr)));
    EXPECT_CALL(*m_mcr, WriteBody(0));

    EXPECT_CALL(*m_srf, SrMCA()).WillOnce(Return(static_cast<SRegMCA*>(m_mca)));
    EXPECT_CALL(*m_mca, ReadBody()).WillOnce(Return(static_cast<RegData>(0)));

    EXPECT_CALL(*m_srf, SrMCS()).WillOnce(Return(static_cast<SRegMCS*>(m_mcs)));
    EXPECT_CALL(*m_mcs, ReadBody()).WillOnce(Return(static_cast<RegData>(1)));

    EXPECT_CALL(*m_mcr, GetOV()).WillOnce(Return(0));
  }

  // Test C
  {
    InSequence dummy;
    EXPECT_CALL(*m_srf, SrMCI()).WillOnce(Return(static_cast<SRegMCI*>(m_mci)));
    EXPECT_CALL(*m_mci, GetSPID()).WillOnce(Return(0)); // spid
    if (hve) {
      EXPECT_CALL(*m_srf, SrMPCFG()).WillRepeatedly(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
      EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(1));  // HBE fix 1: Entry 0 belong to Guest, Entry 1 belong to Host
    }
    if (!hve) {
    /* mpe = 0: Expect all S_E = 1, U_E = 1
    ** mpe = 1, svp = 0: Expect all S_E = 1, U_E = mpat.u_ (assume all are true)
    ** mpe = 0, svp = 1: Expect all S_E = mpat.s_, U_E = mpat.u_ (assume all are true)
    */
      if (mpe) {
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
        EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
        if (svp) {
          EXPECT_CALL(*(m_mpat_vec[0]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        } else {
          EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        }
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
        EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
        if (svp) {
          EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        } else {
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        }
      }
      EXPECT_CALL(*m_mcr, SetSXE(true));
      EXPECT_CALL(*m_mcr, SetUXE(true));
      EXPECT_CALL(*m_mcr, SetSWE(true));
      EXPECT_CALL(*m_mcr, SetUWE(true));
      EXPECT_CALL(*m_mcr, SetSRE(true));
      EXPECT_CALL(*m_mcr, SetURE(true));
      EXPECT_CALL(*m_mcr, SetGSXE(false));
      EXPECT_CALL(*m_mcr, SetGUXE(false));
      EXPECT_CALL(*m_mcr, SetGSWE(false));
      EXPECT_CALL(*m_mcr, SetGUWE(false));
      EXPECT_CALL(*m_mcr, SetGSRE(false));
      EXPECT_CALL(*m_mcr, SetGURE(false));
      EXPECT_CALL(*m_mcr, SetHSXE(false));
      EXPECT_CALL(*m_mcr, SetHUXE(false));
      EXPECT_CALL(*m_mcr, SetHSWE(false));
      EXPECT_CALL(*m_mcr, SetHUWE(false));
      EXPECT_CALL(*m_mcr, SetHSRE(false));
      EXPECT_CALL(*m_mcr, SetHURE(false));
    } else {
      /* gm = 1, gmpe = 0: Expect all HS_E = 1, HU_E = 1
      ** gm = 1, mpe = 0: Expect all GS_E = 1, GU_E = 1
      ** gm = 1, svp = 0: Expect all GS_E = 1, GU_E = mpat.u_ (assume all are true)
      ** gm = 1, svp = 1: Expect all GS_E = mpat.s_, GU_E = mpat.u_ (assume all are true)
      ** gm = 0: Expect all GS_E = 1, GU_E = 1
      ** gm = 0, mpe = 0: Expect all HS_E = 1, HU_E = 1
      ** gm = 0, svp = 0: Expect all HS_E = 1, HU_E = mpat.u_ (assume all are true)
      ** gm = 0, svp = 1: Expect all HS_E = mpat.s_, HU_E = mpat.u_ (assume all are true)
      */
      if (gm) {
        if (mpe) {
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          if (svp) {
            EXPECT_CALL(*(m_mpat_vec[0]), GetSX()).WillOnce(Return(true));
            EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
          } else {
            EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
          }
        }
        if (gmpe) {
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        }
      } else {
        if (mpe) {
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          if (svp) {
            EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
            EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
          } else {
            EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
          }
        }
      }
      EXPECT_CALL(*m_mcr, SetSXE(true));
      EXPECT_CALL(*m_mcr, SetUXE(true));
      EXPECT_CALL(*m_mcr, SetSWE(true));
      EXPECT_CALL(*m_mcr, SetUWE(true));
      EXPECT_CALL(*m_mcr, SetSRE(true));
      EXPECT_CALL(*m_mcr, SetURE(true));
      EXPECT_CALL(*m_mcr, SetGSXE(true));
      EXPECT_CALL(*m_mcr, SetGUXE(true));
      EXPECT_CALL(*m_mcr, SetGSWE(true));
      EXPECT_CALL(*m_mcr, SetGUWE(true));
      EXPECT_CALL(*m_mcr, SetGSRE(true));
      EXPECT_CALL(*m_mcr, SetGURE(true));
      EXPECT_CALL(*m_mcr, SetHSXE(true));
      EXPECT_CALL(*m_mcr, SetHUXE(true));
      EXPECT_CALL(*m_mcr, SetHSWE(true));
      EXPECT_CALL(*m_mcr, SetHUWE(true));
      EXPECT_CALL(*m_mcr, SetHSRE(true));
      EXPECT_CALL(*m_mcr, SetHURE(true));
    }

    EXPECT_CALL(*m_mcr, ReadBody()).Times(2);
    // EXPECT_CALL(*m_trace, RecordSReg(CF_SR_MCR, _)); // TBD
  }

  // check
  m_mpu->CheckMpuSetting();
  ASSERT_EQ(expect_mcr, m_mcr->ReadBody());
  // TBD: Make expected value
}

/*!
 * void CheckMpuSetting(uint32_t rgnid);
 * @brief Check CheckMpuSetting method. (b)
 * TBD: It is necessary to make this test as a separate file and make it with TEST_P
 */
TEST_P(MpuMethodCheck, CheckCheckMpuSetting_b_2) {
  bool hve = std::get<0>(GetParam());
  bool gm = std::get<1>(GetParam());
  bool mpe = std::get<2>(GetParam());
  bool gmpe = std::get<3>(GetParam());
  bool svp = std::get<4>(GetParam());

  uint32_t expect_mcr =  std::get<5>(GetParam());

  // Prologue
  {
    // IsMpuRegionHit
    EXPECT_CALL(*(m_mpat_vec[0]), GetWG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[0]), GetRG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(true));
    m_mpu->AddMpuRegion(0);
    m_mpu->AddMpuRegion(1);
  }
  {
    InSequence dummy;
    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(hve));
    if (hve) {
      EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
      EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(gm)); // Use GM
    }
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegHMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(mpe));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(svp)); // mpm_svp == 1
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(gmpe));
    EXPECT_CALL(*m_srf, SrMCR()).WillOnce(Return(static_cast<SRegMCR*>(m_mcr)));
    EXPECT_CALL(*m_mcr, WriteBody(0));

    EXPECT_CALL(*m_srf, SrMCA()).WillOnce(Return(static_cast<SRegMCA*>(m_mca)));
    EXPECT_CALL(*m_mca, ReadBody()).WillOnce(Return(static_cast<RegData>(0)));

    EXPECT_CALL(*m_srf, SrMCS()).WillOnce(Return(static_cast<SRegMCS*>(m_mcs)));
    EXPECT_CALL(*m_mcs, ReadBody()).WillOnce(Return(static_cast<RegData>(1)));

    EXPECT_CALL(*m_mcr, GetOV()).WillOnce(Return(0));
  }

  // Test C
  {
    InSequence dummy;
    EXPECT_CALL(*m_srf, SrMCI()).WillOnce(Return(static_cast<SRegMCI*>(m_mci)));
    EXPECT_CALL(*m_mci, GetSPID()).WillOnce(Return(0)); // spid
    if (hve) {
      EXPECT_CALL(*m_srf, SrMPCFG()).WillRepeatedly(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
      EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(0)); // HBE fix 0: all Entries belong to Host
    }
    if (!hve) {
    /* mpe = 0: Expect all S_E = 1, U_E = 1
    ** mpe = 1, svp = 0: Expect all S_E = 1, U_E = mpat.u_ (assume all are true)
    ** mpe = 0, svp = 1: Expect all S_E = mpat.s_, U_E = mpat.u_ (assume all are true)
    */
      if (mpe) {
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
        EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
        if (svp) {
          EXPECT_CALL(*(m_mpat_vec[0]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        } else {
          EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        }
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
        EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
        if (svp) {
          EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        } else {
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        }
      }
      EXPECT_CALL(*m_mcr, SetSXE(true));
      EXPECT_CALL(*m_mcr, SetUXE(true));
      EXPECT_CALL(*m_mcr, SetSWE(true));
      EXPECT_CALL(*m_mcr, SetUWE(true));
      EXPECT_CALL(*m_mcr, SetSRE(true));
      EXPECT_CALL(*m_mcr, SetURE(true));
      EXPECT_CALL(*m_mcr, SetGSXE(false));
      EXPECT_CALL(*m_mcr, SetGUXE(false));
      EXPECT_CALL(*m_mcr, SetGSWE(false));
      EXPECT_CALL(*m_mcr, SetGUWE(false));
      EXPECT_CALL(*m_mcr, SetGSRE(false));
      EXPECT_CALL(*m_mcr, SetGURE(false));
      EXPECT_CALL(*m_mcr, SetHSXE(false));
      EXPECT_CALL(*m_mcr, SetHUXE(false));
      EXPECT_CALL(*m_mcr, SetHSWE(false));
      EXPECT_CALL(*m_mcr, SetHUWE(false));
      EXPECT_CALL(*m_mcr, SetHSRE(false));
      EXPECT_CALL(*m_mcr, SetHURE(false));
    } else {
      /* gm = 1, gmpe = 0: Expect all HS_E = 1, HU_E = 1
      ** gm = 1, mpe = 0: Expect all GS_E = 1, GU_E = 1
      ** gm = 1, svp = 0: Expect all GS_E = 1, GU_E = mpat.u_ (assume all are true)
      ** gm = 1, svp = 1: Expect all GS_E = mpat.s_, GU_E = mpat.u_ (assume all are true)
      ** gm = 0: Expect all GS_E = 1, GU_E = 1
      ** gm = 0, mpe = 0: Expect all HS_E = 1, HU_E = 1
      ** gm = 0, svp = 0: Expect all HS_E = 1, HU_E = mpat.u_ (assume all are true)
      ** gm = 0, svp = 1: Expect all HS_E = mpat.s_, HU_E = mpat.u_ (assume all are true)
      */
      if (gm) {
        if (gmpe) {
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[0]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        }
      } else {
        if (mpe) {
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          if (svp) {
            EXPECT_CALL(*(m_mpat_vec[0]), GetSX()).WillOnce(Return(true));
            EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
          } else {
            EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
          }
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          if (svp) {
            EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
            EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
          } else {
            EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
          }
        }
      }
      bool expect_res = !(gm & mpe); // Expected result is TRUE except case MPU enabled in Guest mode
      EXPECT_CALL(*m_mcr, SetSXE(expect_res));
      EXPECT_CALL(*m_mcr, SetUXE(expect_res));
      EXPECT_CALL(*m_mcr, SetSWE(expect_res));
      EXPECT_CALL(*m_mcr, SetUWE(expect_res));
      EXPECT_CALL(*m_mcr, SetSRE(expect_res));
      EXPECT_CALL(*m_mcr, SetURE(expect_res));
      EXPECT_CALL(*m_mcr, SetGSXE(expect_res));
      EXPECT_CALL(*m_mcr, SetGUXE(expect_res));
      EXPECT_CALL(*m_mcr, SetGSWE(expect_res));
      EXPECT_CALL(*m_mcr, SetGUWE(expect_res));
      EXPECT_CALL(*m_mcr, SetGSRE(expect_res));
      EXPECT_CALL(*m_mcr, SetGURE(expect_res));
      EXPECT_CALL(*m_mcr, SetHSXE(true));
      EXPECT_CALL(*m_mcr, SetHUXE(true));
      EXPECT_CALL(*m_mcr, SetHSWE(true));
      EXPECT_CALL(*m_mcr, SetHUWE(true));
      EXPECT_CALL(*m_mcr, SetHSRE(true));
      EXPECT_CALL(*m_mcr, SetHURE(true));
    }

    EXPECT_CALL(*m_mcr, ReadBody()).Times(2);
    // EXPECT_CALL(*m_trace, RecordSReg(CF_SR_MCR, _)); // TBD
  }

  // check
  m_mpu->CheckMpuSetting();
  ASSERT_EQ(expect_mcr, m_mcr->ReadBody());
  // TBD: Make expected value
}

/*!
 * void CheckMpuSetting(uint32_t rgnid);
 * @brief Check CheckMpuSetting method. (b)
 * TBD: It is necessary to make this test as a separate file and make it with TEST_P
 */
TEST_P(MpuMethodCheck, CheckCheckMpuSetting_b_3) {
  bool hve = std::get<0>(GetParam());
  bool gm = std::get<1>(GetParam());
  bool mpe = std::get<2>(GetParam());
  bool gmpe = std::get<3>(GetParam());
  bool svp = std::get<4>(GetParam());

  uint32_t expect_mcr =  std::get<5>(GetParam());

  // Prologue
  {
    // IsMpuRegionHit
    EXPECT_CALL(*(m_mpat_vec[0]), GetWG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[0]), GetRG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[1]), GetWG()).WillOnce(Return(true));
    EXPECT_CALL(*(m_mpat_vec[1]), GetRG()).WillOnce(Return(true));
    m_mpu->AddMpuRegion(0);
    m_mpu->AddMpuRegion(1);
  }
  {
    InSequence dummy;
    EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
    EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(hve));
    if (hve) {
      EXPECT_CALL(*m_srf, SrPSWH()).WillOnce(Return(static_cast<SRegPSWH*>(m_pswh)));
      EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(gm)); // Use GM
    }
    EXPECT_CALL(*m_srf, SrVMPM()).WillRepeatedly(Return(static_cast<SRegHMMPM*>(m_vmpm)));
    EXPECT_CALL(*m_vmpm, GetMPE()).WillOnce(Return(mpe));
    EXPECT_CALL(*m_vmpm, GetSVP()).WillOnce(Return(svp)); // mpm_svp == 1
    EXPECT_CALL(*m_vmpm, GetGMPE()).WillOnce(Return(gmpe));
    EXPECT_CALL(*m_srf, SrMCR()).WillOnce(Return(static_cast<SRegMCR*>(m_mcr)));
    EXPECT_CALL(*m_mcr, WriteBody(0));

    EXPECT_CALL(*m_srf, SrMCA()).WillOnce(Return(static_cast<SRegMCA*>(m_mca)));
    EXPECT_CALL(*m_mca, ReadBody()).WillOnce(Return(static_cast<RegData>(0)));

    EXPECT_CALL(*m_srf, SrMCS()).WillOnce(Return(static_cast<SRegMCS*>(m_mcs)));
    EXPECT_CALL(*m_mcs, ReadBody()).WillOnce(Return(static_cast<RegData>(1)));

    EXPECT_CALL(*m_mcr, GetOV()).WillOnce(Return(0));
  }

  // Test C
  {
    InSequence dummy;
    EXPECT_CALL(*m_srf, SrMCI()).WillOnce(Return(static_cast<SRegMCI*>(m_mci)));
    EXPECT_CALL(*m_mci, GetSPID()).WillOnce(Return(0)); // spid
    if (hve) {
      EXPECT_CALL(*m_srf, SrMPCFG()).WillRepeatedly(Return(static_cast<SRegMPCFG*>(m_vmpcfg)));
      EXPECT_CALL(*m_vmpcfg, GetHBE()).WillOnce(Return(2)); // HBE fix 2: all Entries belong to Guest
    }
    if (!hve) {
    /* mpe = 0: Expect all S_E = 1, U_E = 1
    ** mpe = 1, svp = 0: Expect all S_E = 1, U_E = mpat.u_ (assume all are true)
    ** mpe = 0, svp = 1: Expect all S_E = mpat.s_, U_E = mpat.u_ (assume all are true)
    */
      if (mpe) {
        EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
        EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
        if (svp) {
          EXPECT_CALL(*(m_mpat_vec[0]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        } else {
          EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
        }
        EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
        EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
        if (svp) {
          EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        } else {
          EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
        }
      }
      EXPECT_CALL(*m_mcr, SetSXE(true));
      EXPECT_CALL(*m_mcr, SetUXE(true));
      EXPECT_CALL(*m_mcr, SetSWE(true));
      EXPECT_CALL(*m_mcr, SetUWE(true));
      EXPECT_CALL(*m_mcr, SetSRE(true));
      EXPECT_CALL(*m_mcr, SetURE(true));
      EXPECT_CALL(*m_mcr, SetGSXE(false));
      EXPECT_CALL(*m_mcr, SetGUXE(false));
      EXPECT_CALL(*m_mcr, SetGSWE(false));
      EXPECT_CALL(*m_mcr, SetGUWE(false));
      EXPECT_CALL(*m_mcr, SetGSRE(false));
      EXPECT_CALL(*m_mcr, SetGURE(false));
      EXPECT_CALL(*m_mcr, SetHSXE(false));
      EXPECT_CALL(*m_mcr, SetHUXE(false));
      EXPECT_CALL(*m_mcr, SetHSWE(false));
      EXPECT_CALL(*m_mcr, SetHUWE(false));
      EXPECT_CALL(*m_mcr, SetHSRE(false));
      EXPECT_CALL(*m_mcr, SetHURE(false));
    } else {
      /* gm = 1, gmpe = 0: Expect all HS_E = 1, HU_E = 1
      ** gm = 1, mpe = 0: Expect all GS_E = 1, GU_E = 1
      ** gm = 1, svp = 0: Expect all GS_E = 1, GU_E = mpat.u_ (assume all are true)
      ** gm = 1, svp = 1: Expect all GS_E = mpat.s_, GU_E = mpat.u_ (assume all are true)
      ** gm = 0: Expect all GS_E = 1, GU_E = 1
      ** gm = 0, mpe = 0: Expect all HS_E = 1, HU_E = 1
      ** gm = 0, svp = 0: Expect all HS_E = 1, HU_E = mpat.u_ (assume all are true)
      ** gm = 0, svp = 1: Expect all HS_E = mpat.s_, HU_E = mpat.u_ (assume all are true)
      */
      if (gm) {
        if (mpe) {
          EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
          if (svp) {
            EXPECT_CALL(*(m_mpat_vec[0]), GetSX()).WillOnce(Return(true));
            EXPECT_CALL(*(m_mpla_vec[0]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpua_vec[0]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
          } else {
            EXPECT_CALL(*(m_mpat_vec[0]), GetUX()).WillOnce(Return(true));
          }
          EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
          EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
          if (svp) {
            EXPECT_CALL(*(m_mpat_vec[1]), GetSX()).WillOnce(Return(true));
            EXPECT_CALL(*(m_mpla_vec[1]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpua_vec[1]), ReadBody()).WillOnce(Return(0));
            EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
          } else {
            EXPECT_CALL(*(m_mpat_vec[1]), GetUX()).WillOnce(Return(true));
          }
        }
      }
      bool expect_res = gm == 1 ? !(gm & gmpe) : !((gm & gmpe)|mpe); // In case of Host: result will be !mpe ; In case of Guest: result will be !gmpe
      EXPECT_CALL(*m_mcr, SetSXE(expect_res));
      EXPECT_CALL(*m_mcr, SetUXE(expect_res));
      EXPECT_CALL(*m_mcr, SetSWE(expect_res));
      EXPECT_CALL(*m_mcr, SetUWE(expect_res));
      EXPECT_CALL(*m_mcr, SetSRE(expect_res));
      EXPECT_CALL(*m_mcr, SetURE(expect_res));
      EXPECT_CALL(*m_mcr, SetGSXE(true));
      EXPECT_CALL(*m_mcr, SetGUXE(true));
      EXPECT_CALL(*m_mcr, SetGSWE(true));
      EXPECT_CALL(*m_mcr, SetGUWE(true));
      EXPECT_CALL(*m_mcr, SetGSRE(true));
      EXPECT_CALL(*m_mcr, SetGURE(true));
      EXPECT_CALL(*m_mcr, SetHSXE(expect_res));
      EXPECT_CALL(*m_mcr, SetHUXE(expect_res));
      EXPECT_CALL(*m_mcr, SetHSWE(expect_res));
      EXPECT_CALL(*m_mcr, SetHUWE(expect_res));
      EXPECT_CALL(*m_mcr, SetHSRE(expect_res));
      EXPECT_CALL(*m_mcr, SetHURE(expect_res));
    }

    EXPECT_CALL(*m_mcr, ReadBody()).Times(2);
    // EXPECT_CALL(*m_trace, RecordSReg(CF_SR_MCR, _)); // TBD
  }

  // check
  m_mpu->CheckMpuSetting();
  ASSERT_EQ(expect_mcr, m_mcr->ReadBody());
  // TBD: Make expected value
}
    std::tuple<bool, bool, bool, bool, bool, uint32_t> const ParametersTable_a[] = {
                              /*hve     gm     mpe    gmpe   svp     mcr   */
    /* 0000 */ std::make_tuple(false,  true,  false,  true,  true,  0x00000000),
    /* 0001 */ std::make_tuple(false,  true,   true,  true, false,  0x00000000),
    /* 0002 */ std::make_tuple(false,  true,   true,  true,  true,  0x00000000),
    /* 0003 */ std::make_tuple( true,  true,  false, false,  true,  0x00000000),
    /* 0004 */ std::make_tuple( true,  true,   true, false, false,  0x00000000),
    /* 0005 */ std::make_tuple( true,  true,   true, false,  true,  0x00000000),
    /* 0006 */ std::make_tuple( true,  true,  false,  true,  true,  0x00000000),
    /* 0007 */ std::make_tuple( true,  true,   true,  true, false,  0x00000000),
    /* 0008 */ std::make_tuple( true,  true,   true,  true,  true,  0x00000000),
    /* 0009 */ std::make_tuple( true, false,  false, false,  true,  0x00000000),
    /* 0010 */ std::make_tuple( true, false,   true, false, false,  0x00000000),
    /* 0011 */ std::make_tuple( true, false,   true, false,  true,  0x00000000),
    /* 0012 */ std::make_tuple( true, false,  false,  true,  true,  0x00000000),
    /* 0013 */ std::make_tuple( true, false,   true,  true, false,  0x00000000),
    /* 0014 */ std::make_tuple( true, false,   true,  true,  true,  0x00000000),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters_a, MpuMethodCheck,
                        ::testing::ValuesIn(ParametersTable_a));


int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
