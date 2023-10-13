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
#include <memory>
#include <vector>
#include "config.h"
#include "forest_common.h"
#include "dbg_break.h"
#include "gmock/gmock.h"
#include "mock_proc_element.h"
#include "mock_sreg.h"
#include "mock_sregfile.h"
#include "mock_trace_info.h"

// using ::testing::_;
using ::testing::Return;
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
// using ::testing::InSequence;

#ifdef ENABLE_DBG_BREAK

class DbgBreakMethodCheck
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint32_t, ByteSize, bool>> {
protected:
  DbgBreakMethodCheck(){};
  virtual ~DbgBreakMethodCheck(){};
  virtual void SetUp() {
    m_trace = new MockTraceCtrl(pe_id);
    m_pe = new MockProcElement();

    sreg_dummy = new SRegDUMMY();
    m_ncsreg = new MockNcSRegFile(ctxt_id, m_pe, sreg_dummy);
    m_srf = new MockSRegFile(m_ncsreg);
    m_bpc0 = new MockSRegBPC_N(ctxt_id, m_pe, 0);m_srf->SetSrPtr(CF_SR_BPC0, m_bpc0);
    m_gdbbreak = new DbgBreak(m_srf, core_type, core_version);
  };
  virtual void TearDown() {
    delete m_gdbbreak;
    delete m_srf;
    delete m_ncsreg;
    delete sreg_dummy;
    delete m_pe;
    delete m_trace;
    delete m_bpc0;
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
  MockSRegBPC_N* m_bpc0;

private:
};

/*!
 * bool DbgBreak::IsBrkSizeHit (uint32_t ch, ByteSize size, TraceCtrl* trace) const
 * @brief Check IsBrkSizeHit method.
 */
TEST_P(DbgBreakMethodCheck, CheckIsBrkSizeHit) {
  uint32_t ch = 0;

  uint32_t ty = std::get<0>(GetParam());
  ByteSize size = std::get<1>(GetParam());
  bool expect = std::get<2>(GetParam());

  if (ty < 6) {
    EXPECT_CALL(*m_bpc0, GetTY()).WillOnce(Return(ty));
  } else {
    EXPECT_CALL(*m_bpc0, GetTY()).Times(2).WillOnce(Return(ty));
  }

  // check
  bool res = m_gdbbreak->IsBrkSizeHit(ch, size, m_trace);

  ASSERT_EQ(expect, res);
};
std::tuple<uint32_t, ByteSize, bool> const ParametersTable[] = {
  // TY, size, expect
  /* 0000 */ std::make_tuple(0, SIZE_8BIT, true),
  /* 0001 */ std::make_tuple(0, SIZE_16BIT, true),
  /* 0002 */ std::make_tuple(0, SIZE_32BIT, true),
  /* 0003 */ std::make_tuple(0, SIZE_64BIT, true),
  /* 0004 */ std::make_tuple(0, SIZE_128BIT, true),

  /* 0005 */ std::make_tuple(1, SIZE_8BIT, true),
  /* 0006 */ std::make_tuple(1, SIZE_16BIT, false),
  /* 0007 */ std::make_tuple(1, SIZE_32BIT, false),
  /* 0008 */ std::make_tuple(1, SIZE_64BIT, false),
  /* 0009 */ std::make_tuple(1, SIZE_128BIT, false),

  /* 0010 */ std::make_tuple(2, SIZE_8BIT, false),
  /* 0011 */ std::make_tuple(2, SIZE_16BIT, true),
  /* 0012 */ std::make_tuple(2, SIZE_32BIT, false),
  /* 0013 */ std::make_tuple(2, SIZE_64BIT, false),
  /* 0014 */ std::make_tuple(2, SIZE_128BIT, false),

  /* 0015 */ std::make_tuple(3, SIZE_8BIT, false),
  /* 0016 */ std::make_tuple(3, SIZE_16BIT, false),
  /* 0017 */ std::make_tuple(3, SIZE_32BIT, true),
  /* 0018 */ std::make_tuple(3, SIZE_64BIT, false),
  /* 0019 */ std::make_tuple(3, SIZE_128BIT, false),

  /* 0020 */ std::make_tuple(4, SIZE_8BIT, false),
  /* 0021 */ std::make_tuple(4, SIZE_16BIT, false),
  /* 0022 */ std::make_tuple(4, SIZE_32BIT, false),
  /* 0023 */ std::make_tuple(4, SIZE_64BIT, true),
  /* 0024 */ std::make_tuple(4, SIZE_128BIT, false),

  /* 0025 */ std::make_tuple(5, SIZE_8BIT, false),
  /* 0026 */ std::make_tuple(5, SIZE_16BIT, false),
  /* 0027 */ std::make_tuple(5, SIZE_32BIT, false),
  /* 0028 */ std::make_tuple(5, SIZE_64BIT, false),
  /* 0029 */ std::make_tuple(5, SIZE_128BIT, true),

  /* 0030 */ std::make_tuple(6, SIZE_8BIT, false),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters, DbgBreakMethodCheck,
                        ::testing::ValuesIn(ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif //  ENABLE_DBG_BREAK
