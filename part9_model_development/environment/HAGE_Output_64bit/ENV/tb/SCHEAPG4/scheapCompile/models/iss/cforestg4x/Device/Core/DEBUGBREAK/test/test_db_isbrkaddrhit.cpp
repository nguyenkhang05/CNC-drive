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
using ::testing::InSequence;

#ifdef ENABLE_DBG_BREAK

class DbgBreakMethodCheck
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<bool, PhysAddr, PhysAddr, bool>> {
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
    m_bpav0 = new MockSRegBPAV_N(ctxt_id, m_pe, 0);m_srf->SetSrPtr(CF_SR_BPAV0, m_bpav0);
    m_bpam0 = new MockSRegBPAM_N(ctxt_id, m_pe, 0);m_srf->SetSrPtr(CF_SR_BPAM0, m_bpam0);
    m_hvcfg = new MockSRegHVCFG(ctxt_id, m_pe);m_srf->SetSrPtr(CF_SR_HVCFG, m_hvcfg);
    m_pswh = new MockSRegPSWH(ctxt_id, m_pe);m_srf->SetSrPtr(CF_SR_PSWH, m_pswh);
    m_dbgen = new MockSRegDBGEN(ctxt_id, m_pe);m_srf->SetSrPtr(CF_SR_DBGEN, m_dbgen);
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
    delete m_bpav0;
    delete m_bpam0;
    delete m_hvcfg;
    delete m_pswh;
    delete m_dbgen;
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
  MockSRegBPAV_N* m_bpav0;
  MockSRegBPAM_N* m_bpam0;
  MockSRegHVCFG* m_hvcfg;
  MockSRegPSWH* m_pswh;
  MockSRegDBGEN* m_dbgen;

private:
};

/*!
 * bool DbgBreak::IsBrkAddrHit(uint32_t ch, PhysAddr addr) const
 * @brief Check IsBrkAddrHit method.
 */
TEST_P(DbgBreakMethodCheck, CheckIsBrkAddrHit) {
  PhysAddr addr = 0x100;
  uint32_t ch = 0;

  bool va = std::get<0>(GetParam());
  PhysAddr ba_value = std::get<1>(GetParam());
  PhysAddr ba_mask = std::get<2>(GetParam());
  bool expect = std::get<3>(GetParam());

  // Test
  EXPECT_CALL(*m_bpav0, ReadBody()).WillOnce(Return(ba_value));
  EXPECT_CALL(*m_bpam0, ReadBody()).WillOnce(Return(ba_mask));

  EXPECT_CALL(*m_bpc0, GetVA()).WillOnce(Return(va));

  // check
  bool res = m_gdbbreak->IsBrkAddrHit(ch, addr);

  ASSERT_EQ(expect, res);
};
std::tuple<bool, PhysAddr, PhysAddr, bool> const ParametersTable[] = {
  // VA, ba_value, ba_mask, expect
  /* 0000 */ std::make_tuple(0, 0x100, 0xff, true),
  /* 0001 */ std::make_tuple(0, 0x100, 0xfff, true), // maskで必ず0
  /* 0002 */ std::make_tuple(0, 0x1000, 0xff, false),
  /* 0003 */ std::make_tuple(1, 0x100, 0xff, false),
  /* 0004 */ std::make_tuple(1, 0x100, 0xfff, false), // maskで必ず0
  /* 0005 */ std::make_tuple(1, 0x1000, 0xff, true),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters, DbgBreakMethodCheck,
                        ::testing::ValuesIn(ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif //  ENABLE_DBG_BREAK
