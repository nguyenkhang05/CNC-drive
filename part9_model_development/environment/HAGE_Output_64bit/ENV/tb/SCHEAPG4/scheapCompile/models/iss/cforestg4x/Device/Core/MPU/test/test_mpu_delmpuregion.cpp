/**
 * @file test_mpu_delmpuregion.cpp
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
// using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
// using ::testing::InSequence;

class MpuMethodCheck : public testing::Test {
protected:
  MpuMethodCheck(){};
  virtual ~MpuMethodCheck(){};
  virtual void SetUp() {
    m_trace = new MockTraceCtrl(pe_id);
    m_pe = new MockProcElement();
    sreg_dummy = new SRegDUMMY();
    m_ncsreg = new MockNcSRegFile(ctxt_id, m_pe, sreg_dummy);
    m_srf = new MockSRegFile(m_ncsreg);
    m_mpu = new Mpu(m_trace, m_srf, core_type, core_version);
  };
  virtual void TearDown() {
    delete m_mpu;
    delete m_srf;
    delete m_ncsreg;
    delete sreg_dummy;
    delete m_pe;
    delete m_trace;
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

private:
};

/*!
 * void AllDelMpuRegion(void);
 * @brief Check AllDelMpuRegion method.
 */
TEST_F(MpuMethodCheck, CheckAllDelMpuRegion) {
  // initialize
  {
    uint32_t rgnid = 0;
    const SRegMPLA_N* m_mpla = nullptr;
    const SRegMPUA_N* m_mpua = nullptr;
    const SRegMPAT_N* m_mpat = nullptr;
    Mpu::MpuRegion mprgn(rgnid, m_mpla, m_mpua, m_mpat, m_srf); // dummy
    m_mpu->m_mprgn_vec.push_back(mprgn);//size 1
    m_mpu->m_mprgn_vec.push_back(mprgn);
  }
  ASSERT_EQ(static_cast<size_t>(2), m_mpu->m_mprgn_vec.size());

  //check del 2 region
  m_mpu->AllDelMpuRegion();
  ASSERT_EQ(static_cast<size_t>(0), m_mpu->m_mprgn_vec.size());

  //create all region id and deleted all
  for (uint32_t rgnid = 0; rgnid < 32; rgnid++) {
      const SRegMPLA_N* m_mpla = nullptr;
      const SRegMPUA_N* m_mpua = nullptr;
      const SRegMPAT_N* m_mpat = nullptr;
      Mpu::MpuRegion mprgn(rgnid, m_mpla, m_mpua, m_mpat, m_srf); // dummy
      m_mpu->m_mprgn_vec.push_back(mprgn);
    }
    //check is create 32 region id or not?
    ASSERT_EQ(static_cast<size_t>(32), m_mpu->m_mprgn_vec.size());
    //check delete all 32 region
    m_mpu->AllDelMpuRegion();
    ASSERT_EQ(static_cast<size_t>(0), m_mpu->m_mprgn_vec.size());

    //check don't create region but delete all
    m_mpu->AllDelMpuRegion();
    ASSERT_EQ(static_cast<size_t>(0), m_mpu->m_mprgn_vec.size());
}

/*!
 * void DelMpuRegion(uint32_t rgnid);
 * @brief Check DelMpuRegion method.
 */
TEST_F(MpuMethodCheck, CheckDelMpuRegion) {
  // Initialize mpu region (ID0->5)
  uint32_t all = 2;
  for (uint32_t rgnid = 0; rgnid < 5; rgnid++) {
    const SRegMPLA_N* m_mpla = nullptr;
    const SRegMPUA_N* m_mpua = nullptr;
    const SRegMPAT_N* m_mpat = nullptr;
    Mpu::MpuRegion mprgn(rgnid, m_mpla, m_mpua, m_mpat, m_srf); // dummy
    m_mpu->m_mprgn_vec.push_back(mprgn);
  }


  ASSERT_EQ(static_cast<size_t>(5), m_mpu->m_mprgn_vec.size());

  // check 1: Delete region not be created
  m_mpu->DelMpuRegion(6);
  m_mpu->DelMpuRegion(15);
  m_mpu->DelMpuRegion(20);
  //check again size region
  ASSERT_EQ(static_cast<size_t>(5), m_mpu->m_mprgn_vec.size());

  // check 2: delete specific region: rgnid == 3 4
  m_mpu->DelMpuRegion(4);
  ASSERT_EQ(static_cast<size_t>(4), m_mpu->m_mprgn_vec.size());
  m_mpu->DelMpuRegion(3);
  ASSERT_EQ(static_cast<size_t>(3), m_mpu->m_mprgn_vec.size());

  // check 3: delete multi rgnid ( 0,1,2,3 )
  for (uint32_t rgnid = 0; rgnid <= all; rgnid++) {
    m_mpu->DelMpuRegion(rgnid);
  }
  ASSERT_EQ(static_cast<size_t>(0), m_mpu->m_mprgn_vec.size());

  //check 4: delete all region and delete continue to check size
  m_mpu->DelMpuRegion(4);
  ASSERT_EQ(static_cast<size_t>(0), m_mpu->m_mprgn_vec.size());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
