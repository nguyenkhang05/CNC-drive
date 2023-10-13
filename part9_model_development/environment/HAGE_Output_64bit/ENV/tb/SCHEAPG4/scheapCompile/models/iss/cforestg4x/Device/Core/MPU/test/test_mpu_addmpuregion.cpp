/**
 * @file test_mpu_addmpuregion.cpp
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
 * void AddMpuRegion(uint32_t rgnid);
 * @brief Check AddMpuRegion method.
 */
TEST_F(MpuMethodCheck, CheckAddMpuRegion) {
	//check 1, create 32 region, each time add region, check size
	for (uint32_t rgnid = 0; rgnid < 32; rgnid++) {
	      const SRegMPLA_N* m_mpla = nullptr;
	      const SRegMPUA_N* m_mpua = nullptr;
	      const SRegMPAT_N* m_mpat = nullptr;
	      Mpu::MpuRegion mprgn(rgnid, m_mpla, m_mpua, m_mpat, m_srf); // dummy
	      m_mpu->m_mprgn_vec.push_back(mprgn);

	      ASSERT_EQ(static_cast<size_t>(rgnid+1), m_mpu->m_mprgn_vec.size());
	}
	//check is create 32 region
    ASSERT_EQ(static_cast<size_t>(32), m_mpu->m_mprgn_vec.size());
	
	//check delete all mpu region and create 32 region again
	m_mpu->AllDelMpuRegion();
    ASSERT_EQ(static_cast<size_t>(0), m_mpu->m_mprgn_vec.size());
	for (uint32_t rgnid = 0; rgnid < 32; rgnid++) {
			  const SRegMPLA_N* m_mpla = nullptr;
			  const SRegMPUA_N* m_mpua = nullptr;
			  const SRegMPAT_N* m_mpat = nullptr;
			  Mpu::MpuRegion mprgn(rgnid, m_mpla, m_mpua, m_mpat, m_srf); // dummy
			  m_mpu->m_mprgn_vec.push_back(mprgn);

	      ASSERT_EQ(static_cast<size_t>(rgnid+1), m_mpu->m_mprgn_vec.size());
	}

}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
