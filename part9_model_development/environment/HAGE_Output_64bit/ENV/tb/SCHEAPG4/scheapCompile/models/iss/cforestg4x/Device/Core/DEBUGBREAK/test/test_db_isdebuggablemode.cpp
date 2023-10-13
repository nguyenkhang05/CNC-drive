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

class DbgBreakMethodCheck_BREFORE_G4MH20
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<bool, PhysAddr, PhysAddr, bool>> {
protected:
  DbgBreakMethodCheck_BREFORE_G4MH20(){};
  virtual ~DbgBreakMethodCheck_BREFORE_G4MH20(){};
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
  CoreVersion core_version = CORE_VER_11; // important

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
 * bool DbgBreak::IsDebuggableMode (uint32_t ch) const
 * @brief Check IsDebuggableMode method.
 */
TEST_F(DbgBreakMethodCheck_BREFORE_G4MH20, CheckIsDebuggableMode_a) {
  uint32_t ch = 0;

  // check
  bool res = m_gdbbreak->IsDebuggableMode(ch);

  ASSERT_EQ(true, res);
};


class DbgBreakMethodCheck
  : public testing::Test
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
    m_bpc0 = new MockSRegBPC_N(ctxt_id, m_pe, 0);m_srf->SetSrPtr(CF_SR_BPC0, m_bpc0);
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
    delete m_hvcfg;
    delete m_pswh;
    delete m_dbgen;
  };

public:
  // System Settings
  CoreType core_type = CORE_G4MH;
  CoreVersion core_version = CORE_VER_20; // important

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
  MockSRegHVCFG* m_hvcfg;
  MockSRegPSWH* m_pswh;
  MockSRegDBGEN* m_dbgen;

private:
};

/*!
 * bool DbgBreak::IsDebuggableMode (uint32_t ch) const
 * @brief Check IsDebuggableMode method.
 */
TEST_F(DbgBreakMethodCheck, CheckIsDebuggableMode_b) {
  uint32_t ch = 0;

  InSequence s;
  // Test
  EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
  EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(0));

  // check
  bool res = m_gdbbreak->IsDebuggableMode(ch);

  ASSERT_EQ(true, res);
};

TEST_F(DbgBreakMethodCheck, CheckIsDebuggableMode_c) {
  uint32_t ch = 0;
  bool res;
  InSequence s;
  // Test 2
  // if (m_srf->SrHVCFG()->GetHVE() == 1) {
  EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
  EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
  // bool mode = static_cast<bool>(m_srf->SrPSWH()->GetGM());
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(1)); // Guest mode
  // uint32_t gpid = m_srf->SrPSWH()->GetGPID();
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGPID()).WillOnce(Return(7));
  // std::bitset<8> dbgen_ge = m_srf->SrDBGEN()->GetGE();
  //  if (dbgen_ge[gpid]) {
  EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
  EXPECT_CALL(*m_dbgen, GetGE()).WillOnce(Return(0b10000000));
  // std::bitset<8> bpc_ge = bpc->GetGE();
  // if (bpc_ge[gpid]) {
  EXPECT_CALL(*m_bpc0, GetGE()).WillOnce(Return(0b10000000)); // ret true

  // check
  res = m_gdbbreak->IsDebuggableMode(ch);
  ASSERT_EQ(true, res);

  // Test 2
  // if (m_srf->SrHVCFG()->GetHVE() == 1) {
  EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
  EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
  // bool mode = static_cast<bool>(m_srf->SrPSWH()->GetGM());
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(1)); // Guest mode
  // uint32_t gpid = m_srf->SrPSWH()->GetGPID();
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGPID()).WillOnce(Return(7));
  // std::bitset<8> dbgen_ge = m_srf->SrDBGEN()->GetGE();
  //  if (dbgen_ge[gpid]) {
  EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
  EXPECT_CALL(*m_dbgen, GetGE()).WillOnce(Return(0b10000000));
  // std::bitset<8> bpc_ge = bpc->GetGE();
  // if (bpc_ge[gpid]) {
  EXPECT_CALL(*m_bpc0, GetGE()).WillOnce(Return(0b01111111)); // ret false

  // check
  res = m_gdbbreak->IsDebuggableMode(ch);
  ASSERT_EQ(false, res);
};

TEST_F(DbgBreakMethodCheck, CheckIsDebuggableMode_d) {
  uint32_t ch = 0;

  InSequence s;
  // Test
  // if (m_srf->SrHVCFG()->GetHVE() == 1) {
  EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
  EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
  // bool mode = static_cast<bool>(m_srf->SrPSWH()->GetGM());
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(1)); // Guest mode
  // uint32_t gpid = m_srf->SrPSWH()->GetGPID();
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGPID()).WillOnce(Return(7));
  // std::bitset<8> dbgen_ge = m_srf->SrDBGEN()->GetGE();
  //  if (dbgen_ge[gpid]) {
  EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
  EXPECT_CALL(*m_dbgen, GetGE()).WillOnce(Return(0b00000001)); // ret false

  // check
  bool res = m_gdbbreak->IsDebuggableMode(ch);

  ASSERT_EQ(false, res);
};

TEST_F(DbgBreakMethodCheck, CheckIsDebuggableMode_e) {
  uint32_t ch = 0;
  bool res;

  InSequence s;
  // Test 1
  // if (m_srf->SrHVCFG()->GetHVE() == 1) {
  EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
  EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
  // bool mode = static_cast<bool>(m_srf->SrPSWH()->GetGM());
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(0)); // Host mode
  // bool dbgen_he = static_cast<bool>(m_srf->SrDBGEN()->GetHE());
  EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
  EXPECT_CALL(*m_dbgen, GetHE()).WillOnce(Return(true)); // he=1
  // bool bpc_he = bpc->GetHE();
  // if (bpc_he) {
  EXPECT_CALL(*m_bpc0, GetHE()).WillOnce(Return(0b1)); // ret true

  // check
  res = m_gdbbreak->IsDebuggableMode(ch);
  ASSERT_EQ(true, res);

  // Test 2
  // if (m_srf->SrHVCFG()->GetHVE() == 1) {
  EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
  EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
  // bool mode = static_cast<bool>(m_srf->SrPSWH()->GetGM());
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(0)); // Host mode
  // bool dbgen_he = static_cast<bool>(m_srf->SrDBGEN()->GetHE());
  EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
  EXPECT_CALL(*m_dbgen, GetHE()).WillOnce(Return(true)); // he=1
  // bool bpc_he = bpc->GetHE();
  // if (bpc_he) {
  EXPECT_CALL(*m_bpc0, GetHE()).WillOnce(Return(0b0)); // ret false

  // check
  res = m_gdbbreak->IsDebuggableMode(ch);
  ASSERT_EQ(false, res);
};

TEST_F(DbgBreakMethodCheck, CheckIsDebuggableMode_f) {
  uint32_t ch = 0;
  bool res;

  InSequence s;
  // Test
  // if (m_srf->SrHVCFG()->GetHVE() == 1) {
  EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
  EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
  // bool mode = static_cast<bool>(m_srf->SrPSWH()->GetGM());
  EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
  EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(0)); // Host mode
  // bool dbgen_he = static_cast<bool>(m_srf->SrDBGEN()->GetHE());
  EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
  EXPECT_CALL(*m_dbgen, GetHE()).WillOnce(Return(false)); // ret false

  // check
  res = m_gdbbreak->IsDebuggableMode(ch);
  ASSERT_EQ(false, res);
};

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif //  ENABLE_DBG_BREAK
