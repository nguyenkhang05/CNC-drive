/**
 * @file test_mpu_ismpuregionpermitted.cpp
 *
 * @date Created on: 2018
 * @author Author: Hoanh Ho <hoanh.ho.px@rvc.renesas.com>
 * @brief UNIT TEST for MPU.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
// #include <memory>
// #include <stdexcept>
//#include <memory>
//#include <vector>
#include "config.h"
//#include "forest_common.h"
#include "gmock/gmock.h"
#include "mock_proc_element.h"
#include "mock_sreg.h"
#include "mock_sregfile.h"
#include "mock_trace_info.h"
#include "mpu.h"

// using ::testing::_;
using ::testing::Return;
using ::testing::AtMost;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
// using ::testing::InSequence;

// From mpu.h
static const uint32_t N_SPID = 32; // From mpu.h Mpu::MpuRegion::N_SPID
static const uint32_t N_MPID = 8;  // From mpu.h Mpu::MpuRegion::N_MPID

//
class MpuMethodCheck
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<MemAccessCommand, bool, PhysAddr, ByteSize,
                                                    uint32_t, bool, bool>> {
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
    for (int i = 0; i < static_cast<int>(N_MPID); i++) {
      m_mpid[i] = new MockSRegMPID(ctxt_id, m_pe, i);
      m_srf->SetSrPtr(CF_SR_MPID0 + i, m_mpid[i]);
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
    for (MockSRegMPLA_N* element : m_mpla_vec) {
      delete element;
    };
    m_mpla_vec.clear();
    for (MockSRegMPUA_N* element : m_mpua_vec) {
      delete element;
    };
    m_mpua_vec.clear();
    for (MockSRegMPAT_N* element : m_mpat_vec) {
      delete element;
    };
    m_mpat_vec.clear();
    for (int i = 0; i < static_cast<int>(N_MPID); i++) {
      delete m_mpid[i];
    }
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
  MockSRegMPID* m_mpid[N_MPID];

private:
};

/*!
 * bool IsMpuRegionPermitted<>(bool um) const
 * @brief Check IsMpuRegionPermitted method.
 */
TEST_P(MpuMethodCheck, CheckIsMpuRegionPermitted) {
  MemAccessCommand prm_mac = std::get<0>(GetParam());
  bool prm_um = std::get<1>(GetParam());
  PhysAddr prm_addr = std::get<2>(GetParam());
  ByteSize prm_size = std::get<3>(GetParam());
  uint32_t prm_spid = std::get<4>(GetParam());
  bool prm_priv = std::get<5>(GetParam());
  bool prm_expect = std::get<6>(GetParam());

  auto run_test = [&](MemAccessCommand cmd, auto um, auto vrtl_addr, auto size, auto spid, auto priv, auto expect) {
    int idx = 0;
    // Prologue
    {
      EXPECT_CALL(*(m_mpat_vec[idx]), GetWG()).WillOnce(Return(true));
      EXPECT_CALL(*(m_mpat_vec[idx]), GetRG()).WillOnce(Return(true));
      m_mpu->AddMpuRegion(idx);
    }

    // Test Condition
    {
      switch (prm_mac) {
      case DATA_WRITE:
        if (prm_um) {
          EXPECT_CALL(*(m_mpat_vec[idx]), GetUW()).WillOnce(Return(prm_priv));
        } else {
          EXPECT_CALL(*(m_mpat_vec[idx]), GetSW()).WillOnce(Return(prm_priv));
        }
        break;
      case INST_READ:
        if (prm_um) {
          EXPECT_CALL(*(m_mpat_vec[idx]), GetUX()).WillOnce(Return(prm_priv));
        } else {
          EXPECT_CALL(*(m_mpat_vec[idx]), GetSX()).WillOnce(Return(prm_priv));
        }
        break;
      case DATA_READ:
        if (prm_um) {
          EXPECT_CALL(*(m_mpat_vec[idx]), GetUR()).WillOnce(Return(prm_priv));
        } else {
          EXPECT_CALL(*(m_mpat_vec[idx]), GetSR()).WillOnce(Return(prm_priv));
        }
        break;
      default:
        FAIL() << "We shouldn't get here.";
      }
    }
    // check
    bool res;
    for (Mpu::MpuRegion ite : m_mpu->m_mprgn_vec) {
      switch (cmd) {
      case DATA_WRITE:
        res = ite.IsMpuRegionPermitted<DATA_WRITE>(um);
        break;
      case INST_READ:
        res = ite.IsMpuRegionPermitted<INST_READ>(um);
        break;
      case DATA_READ:
        res = ite.IsMpuRegionPermitted<DATA_READ>(um);
        break;
      default:
        FAIL() << "We shouldn't get here.";
      }
      ASSERT_EQ(expect, res);
    };
  };

  run_test(prm_mac, prm_um, prm_addr, prm_size, prm_spid, prm_priv, prm_expect);
};
std::tuple<MemAccessCommand, bool, PhysAddr, ByteSize, uint32_t, bool, bool> const
    ParametersTable[] = {
                              // mac,        um,    st_addr, size, spid, priv,  result
      /* 0000 */ std::make_tuple(DATA_WRITE, true,  0,       1,    0,    true,  true),           // Exit ()
      /* 0001 */ std::make_tuple(DATA_WRITE, true,  0,       1,    0,    false, false),          // Exit ()
      /* 0002 */ std::make_tuple(DATA_WRITE, false, 0,       1,    0,    true,  true),           // Exit ()
      /* 0003 */ std::make_tuple(DATA_WRITE, false, 0,       1,    0,    false, false),          // Exit ()
      /* 0004 */ std::make_tuple(INST_READ,  true,  0,       1,    0,    true,  true),           // Exit ()
      /* 0005 */ std::make_tuple(INST_READ,  true,  0,       1,    0,    false, false),          // Exit ()
      /* 0006 */ std::make_tuple(INST_READ,  false, 0,       1,    0,    true,  true),           // Exit ()
      /* 0007 */ std::make_tuple(INST_READ,  false, 0,       1,    0,    false, false),          // Exit ()
      /* 0008 */ std::make_tuple(DATA_READ,  true,  0,       1,    0,    true,  true),           // Exit ()
      /* 0009 */ std::make_tuple(DATA_READ,  true,  0,       1,    0,    false, false),          // Exit ()
      /* 0010 */ std::make_tuple(DATA_READ,  false, 0,       1,    0,    true,  true),           // Exit ()
      /* 0011 */ std::make_tuple(DATA_READ,  false, 0,       1,    0,    false, false),          // Exit ()
      // please write more more more test case.
    };
INSTANTIATE_TEST_CASE_P(TestWithParameters, MpuMethodCheck, ::testing::ValuesIn(ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
