/**
 * @file test_mpu_ismpuregionhit.cpp
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for MPU.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
// #include <memory>
// #include <stdexcept>
#include <memory>
#include <vector>
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
                                                    uint32_t, RegData, RegData, bool>> {
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
 * bool IsMpuRegionHit<>(PhysAddr vrtl_addr, ByteSize size, uint32_t spid) const
 * @brief Check IsMpuRegionHit method.
 */
TEST_P(MpuMethodCheck, CheckIsMpuRegionHit) {
  MemAccessCommand prm_mac = std::get<0>(GetParam());
  bool prm_permit = std::get<1>(GetParam());
  PhysAddr prm_addr = std::get<2>(GetParam());
  ByteSize prm_size = std::get<3>(GetParam());
  uint32_t prm_spid = std::get<4>(GetParam());
  RegData prm_mpla = std::get<5>(GetParam());
  RegData prm_mpua = std::get<6>(GetParam());
  bool prm_expect = std::get<7>(GetParam());

  auto run_test = [&](MemAccessCommand cmd, auto vrtl_addr, auto size, auto spid, auto expect) {
    int idx = 0;
    // Prologue
    {
      EXPECT_CALL(*(m_mpat_vec[idx]), GetWG()).WillOnce(Return(prm_permit));
      EXPECT_CALL(*(m_mpat_vec[idx]), GetRG()).WillOnce(Return(prm_permit));
      if (!prm_permit) {
        EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0)); // 0b0 -> bit count is 0
        EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0)); // 0b0 -> bit count is 0
      }
      m_mpu->AddMpuRegion(idx);
    }

    // Test Condition
    {
      EXPECT_CALL(*(m_mpla_vec[idx]), ReadBody()).WillOnce(Return(prm_mpla));
      EXPECT_CALL(*(m_mpua_vec[idx]), ReadBody()).Times(AtMost(1)).WillOnce(Return(prm_mpua));
    }
    // check
    bool res;
    for (Mpu::MpuRegion ite : m_mpu->m_mprgn_vec) {
      switch (cmd) {
      case DATA_WRITE:
        res = ite.IsMpuRegionHit<DATA_WRITE>(vrtl_addr, size, spid);
        break;
      case INST_READ:
        res = ite.IsMpuRegionHit<INST_READ>(vrtl_addr, size, spid);
        break;
      case DATA_READ:
        res = ite.IsMpuRegionHit<DATA_READ>(vrtl_addr, size, spid);
        break;
//      case DATA_RMW:
//        res = ite.IsMpuRegionHit<DATA_RMW>(vrtl_addr, size, spid);
//        break;
      default:
        FAIL() << "We shouldn't get here.";
      }
      ASSERT_EQ(expect, res);
    };
  };

  run_test(prm_mac, prm_addr, prm_size, prm_spid, prm_expect);
};
std::tuple<MemAccessCommand, bool, PhysAddr, ByteSize, uint32_t, RegData, RegData, bool> const
    ParametersTable[] = {
                              // mac,       permit, st_addr,           size, spid, mpla, mpua,            result
      /* 0000 */ std::make_tuple(DATA_WRITE, true,  0,                 1,    0,    0,    0,               true),   // Exit (a)
      /* 0001 */ std::make_tuple(DATA_WRITE, false, 0,                 1,    0,    0,    0,               false),  // Exit (b)
      /* 0002 */ std::make_tuple(DATA_WRITE, true,  0,                 1,    0,    1,    0,               false),  // Exit (e)
      /* 0003 */ std::make_tuple(DATA_WRITE, true,  0,                 5,    0,    0,    0,               false),  // Exit (e)
      /* 0004 */ std::make_tuple(DATA_WRITE, true,  0,                 0,    0,    0,    0,               false),  // Exit (e)
      /* 0005 */ std::make_tuple(DATA_WRITE, true,  1,                 0,    0,    0,    0,               false),  // Exit (e)
      /* 0006 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU - 3,   4,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0007 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU - 3,   5,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0008 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU - 2,   3,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0009 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU - 2,   4,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0010 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU - 1,   2,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0011 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU - 1,   3,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0012 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU,       1,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0013 */ std::make_tuple(DATA_WRITE, true,  0xFFFFFFFFU,       2,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0014 */ std::make_tuple(INST_READ,  true,  0,                 1,    0,    0,    0,               true),   // Exit (a)
      /* 0015 */ std::make_tuple(INST_READ,  false, 0,                 1,    0,    0,    0,               false),  // Exit (b)
      /* 0016 */ std::make_tuple(INST_READ,  true,  0,                 1,    0,    1,    0,               false),  // Exit (e)
      /* 0017 */ std::make_tuple(INST_READ,  true,  0,                 5,    0,    0,    0,               false),  // Exit (e)
      /* 0018 */ std::make_tuple(INST_READ,  true,  0,                 0,    0,    0,    0,               false),  // Exit (e)
      /* 0019 */ std::make_tuple(INST_READ,  true,  1,                 0,    0,    0,    0,               false),  // Exit (e)
      /* 0020 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU - 3,   4,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0021 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU - 3,   5,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0022 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU - 2,   3,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0023 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU - 2,   4,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0024 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU - 1,   2,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0025 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU - 1,   3,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0026 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU,       1,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0027 */ std::make_tuple(INST_READ,  true,  0xFFFFFFFFU,       2,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0028 */ std::make_tuple(DATA_READ,  true,  0,                 1,    0,    0,    0,               true),   // Exit (a)
      /* 0029 */ std::make_tuple(DATA_READ,  false, 0,                 1,    0,    0,    0,               false),  // Exit (b)
      /* 0030 */ std::make_tuple(DATA_READ,  true,  0,                 1,    0,    1,    0,               false),  // Exit (e)
      /* 0031 */ std::make_tuple(DATA_READ,  true,  0,                 5,    0,    0,    0,               false),  // Exit (e)
      /* 0032 */ std::make_tuple(DATA_READ,  true,  0,                 0,    0,    0,    0,               false),  // Exit (e)
      /* 0033 */ std::make_tuple(DATA_READ,  true,  1,                 0,    0,    0,    0,               false),  // Exit (e)
      /* 0034 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU - 3,   4,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0035 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU - 3,   5,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0036 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU - 2,   3,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0037 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU - 2,   4,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0038 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU - 1,   2,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0039 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU - 1,   3,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
      /* 0040 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU,       1,    0,    0,    0xFFFFFFFFU - 3, true),   // Exit (a)
      /* 0041 */ std::make_tuple(DATA_READ,  true,  0xFFFFFFFFU,       2,    0,    0,    0xFFFFFFFFU - 3, false),  // Exit (e)
//      /* 0043 */ std::make_tuple(DATA_RMW, false, 0, 1, 0, 0, 0, false),                             // Exit (b)
//      /* 0044 */ std::make_tuple(DATA_RMW, true, 0, 1, 0, 1, 0, false),                              // Exit (e)
//      /* 0045 */ std::make_tuple(DATA_RMW, true, 0, 5, 0, 0, 0, false),                              // Exit (e)
//      /* 0046 */ std::make_tuple(DATA_RMW, true, 0, 0, 0, 0, 0, false),                              // Exit (e)
//      /* 0047 */ std::make_tuple(DATA_RMW, true, 1, 0, 0, 0, 0, false),                              // Exit (e)
//      /* 0048 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU - 3, 4, 0, 0, 0xFFFFFFFFU - 3, true),   // Exit (a)
//      /* 0049 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU - 3, 5, 0, 0, 0xFFFFFFFFU - 3, false),  // Exit (e)
//      /* 0050 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU - 2, 3, 0, 0, 0xFFFFFFFFU - 3, true),   // Exit (a)
//      /* 0051 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU - 2, 4, 0, 0, 0xFFFFFFFFU - 3, false),  // Exit (e)
//      /* 0052 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU - 1, 2, 0, 0, 0xFFFFFFFFU - 3, true),   // Exit (a)
//      /* 0053 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU - 1, 3, 0, 0, 0xFFFFFFFFU - 3, false),  // Exit (e)
//      /* 0054 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU, 1, 0, 0, 0xFFFFFFFFU - 3, true),       // Exit (a)
//      /* 0055 */ std::make_tuple(DATA_RMW, true, 0xFFFFFFFFU, 2, 0, 0, 0xFFFFFFFFU - 3, false),      // Exit (e)
      // please write more more more test case.
    };
INSTANTIATE_TEST_CASE_P(TestWithParameters, MpuMethodCheck, ::testing::ValuesIn(ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
