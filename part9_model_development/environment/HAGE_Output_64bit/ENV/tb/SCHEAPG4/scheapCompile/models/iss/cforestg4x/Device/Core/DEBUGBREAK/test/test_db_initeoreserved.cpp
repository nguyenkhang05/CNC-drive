/**
 * @file test_db.cpp
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for DEBUGBREAK.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
// #include <memory>
// #include <stdexcept>
#include <vector>
#include <memory>
#include "config.h"
#include "forest_common.h"
#include "mock_trace_info.h"
#include "mock_sregfile.h"
#include "mock_sreg.h"
#include "mock_proc_element.h"
#include "dbg_break.h"
#include "gmock/gmock.h"

// using ::testing::_;
using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
// using ::testing::InSequence;

#ifdef ENABLE_DBG_BREAK

class DbgBreakMethodCheck : public testing::Test {
protected:
  DbgBreakMethodCheck(){};
  virtual ~DbgBreakMethodCheck(){};
  virtual void SetUp() {
    m_trace = new MockTraceCtrl(pe_id);
    m_pe = new MockProcElement();

    sreg_dummy = new SRegDUMMY();
    m_ncsreg = new MockNcSRegFile(ctxt_id, m_pe, sreg_dummy);
    m_srf = new MockSRegFile(m_ncsreg);
    m_gdbbreak = new DbgBreak(m_srf, core_type, core_version);
  };
  virtual void TearDown() {
    delete m_gdbbreak;
    delete m_srf;
    delete m_ncsreg;
    delete sreg_dummy;
    delete m_pe;
    delete m_trace;
  };

public:
  // System Settings
  CoreType core_type = CORE_G4MH;
  CoreVersion core_version = CORE_VER_20;

  // Test object
  DbgBreak* m_gdbbreak;

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
 * void  DbgBreak::InitEoReserved(void);
 * @brief Check InitEoReserved method.
 */
TEST_F(DbgBreakMethodCheck, CheckInitEoReserved) {

  m_gdbbreak->m_pcb_eo_reserve.push_back(1);
  m_gdbbreak->m_lsab_eo_reserve.push_back(1);
  ASSERT_EQ(1U, m_gdbbreak->m_pcb_eo_reserve.size());
  ASSERT_EQ(1U, m_gdbbreak->m_pcb_eo_reserve.size());

  // check
  m_gdbbreak->InitEoReserved();

  ASSERT_EQ(0U, m_gdbbreak->m_pcb_eo_reserve.size());
  ASSERT_EQ(0U, m_gdbbreak->m_pcb_eo_reserve.size());

  m_gdbbreak->m_pcb_eo_reserve.push_back(2);
  m_gdbbreak->m_pcb_eo_reserve.push_back(3);
  m_gdbbreak->m_lsab_eo_reserve.push_back(2);
  m_gdbbreak->m_lsab_eo_reserve.push_back(3);
  ASSERT_EQ(2U, m_gdbbreak->m_pcb_eo_reserve.size());
  ASSERT_EQ(2U, m_gdbbreak->m_pcb_eo_reserve.size());

  // check
  m_gdbbreak->InitEoReserved();

  ASSERT_EQ(0U, m_gdbbreak->m_pcb_eo_reserve.size());
  ASSERT_EQ(0U, m_gdbbreak->m_pcb_eo_reserve.size());

}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif //  ENABLE_DBG_BREAK
