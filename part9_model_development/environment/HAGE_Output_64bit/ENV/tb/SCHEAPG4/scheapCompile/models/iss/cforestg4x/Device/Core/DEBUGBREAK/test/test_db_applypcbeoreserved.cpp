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

using ::testing::_;
using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
// using ::testing::InSequence;

#ifdef ENABLE_DBG_BREAK

class DbgBreakMethodCheck : public testing::Test
{
protected:
  DbgBreakMethodCheck(){};
  virtual ~DbgBreakMethodCheck(){};
  virtual void SetUp() {
    m_trace = new MockTraceCtrl(pe_id);
    m_pe = new MockProcElement();

    sreg_dummy = new SRegDUMMY();
    m_ncsreg = new MockNcSRegFile(ctxt_id, m_pe, sreg_dummy);
    m_srf = new MockSRegFile(m_ncsreg);
    for (int i = 0; i < static_cast<int>(DbgBreak::N_BPA); i++) {
      m_bpc_vec.push_back(new MockSRegBPC_N(ctxt_id, m_pe, i));
      m_srf->SetSrPtr(CF_SR_BPC0 + i, m_bpc_vec.back());
    }
    m_gdbbreak = new DbgBreak(m_srf, core_type, core_version);
  };
  virtual void TearDown() {
    delete m_gdbbreak;
    delete m_srf;
    delete m_ncsreg;
    delete sreg_dummy;
    delete m_pe;
    delete m_trace;
    for (MockSRegBPC_N* element : m_bpc_vec) { delete element; }; m_bpc_vec.clear();
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
  std::vector<MockSRegBPC_N*> m_bpc_vec;

private:
};

/*!
 * void  DbgBreak::ApplyPcbEoReserved(TraceCtrl* trace);
 * @brief Check ApplyPcbEoReserved method.
 */
TEST_F(DbgBreakMethodCheck, CheckApplyPcbEoReserved) {

  EXPECT_CALL(*(m_bpc_vec[0]), ReadBody()).Times(0);
  EXPECT_CALL(*(m_bpc_vec[1]), ReadBody()).WillOnce(Return(0));
  EXPECT_CALL(*(m_bpc_vec[2]), ReadBody()).WillOnce(Return(0));
  EXPECT_CALL(*(m_bpc_vec[3]), ReadBody()).WillOnce(Return(0));
  EXPECT_CALL(*(m_bpc_vec[0]), SetEO(_)).Times(0);
  EXPECT_CALL(*(m_bpc_vec[1]), SetEO(_));
  EXPECT_CALL(*(m_bpc_vec[2]), SetEO(_));
  EXPECT_CALL(*(m_bpc_vec[3]), SetEO(_));

  m_gdbbreak->m_pcb_eo_reserve.push_back(3);
  m_gdbbreak->m_pcb_eo_reserve.push_back(1);
  m_gdbbreak->m_pcb_eo_reserve.push_back(2);
  ASSERT_EQ(3U, m_gdbbreak->m_pcb_eo_reserve.size());

  // check
  m_gdbbreak->ApplyPcbEoReserved(m_trace);

  ASSERT_EQ(0U, m_gdbbreak->m_pcb_eo_reserve.size());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif //  ENABLE_DBG_BREAK
