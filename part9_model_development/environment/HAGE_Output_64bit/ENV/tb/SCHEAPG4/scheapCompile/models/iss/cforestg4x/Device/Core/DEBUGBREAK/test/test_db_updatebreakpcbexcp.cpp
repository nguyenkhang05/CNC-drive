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
    for (int i = 0; i < static_cast<int>(DbgBreak::N_BPA); i++) {
      m_bpc_vec.push_back(new MockSRegBPC_N(ctxt_id, m_pe, i));
      m_srf->SetSrPtr(CF_SR_BPC0 + i, m_bpc_vec.back());
    }
    m_dir0 = new MockSRegDIR0(ctxt_id, m_pe);m_srf->SetSrPtr(CF_SR_DIR0, m_dir0);
    m_dir1 = new MockSRegDIR1(ctxt_id, m_pe);m_srf->SetSrPtr(CF_SR_DIR1, m_dir1);
    m_bpav0 = new MockSRegBPAV_N(ctxt_id, m_pe, 0);m_srf->SetSrPtr(CF_SR_BPAV0, m_bpav0);
    m_bpam0 = new MockSRegBPAM_N(ctxt_id, m_pe, 0);m_srf->SetSrPtr(CF_SR_BPAM0, m_bpam0);
    m_bpav1 = new MockSRegBPAV_N(ctxt_id, m_pe, 1);m_srf->SetSrPtr(CF_SR_BPAV1, m_bpav1);
    m_bpam1 = new MockSRegBPAM_N(ctxt_id, m_pe, 1);m_srf->SetSrPtr(CF_SR_BPAM1, m_bpam1);
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
    for (MockSRegBPC_N* element : m_bpc_vec) {delete element;}; m_bpc_vec.clear();
    delete m_dir0;
    delete m_dir1;
    delete m_bpav0;
    delete m_bpam0;
    delete m_bpav1;
    delete m_bpam1;
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
  std::vector<MockSRegBPC_N*> m_bpc_vec;
  MockSRegDIR0* m_dir0;
  MockSRegDIR1* m_dir1;
  MockSRegBPAV_N* m_bpav0;
  MockSRegBPAM_N* m_bpam0;
  MockSRegBPAV_N* m_bpav1;
  MockSRegBPAM_N* m_bpam1;
  MockSRegHVCFG* m_hvcfg;
  MockSRegPSWH* m_pswh;
  MockSRegDBGEN* m_dbgen;

private:
};

/*!
 * void  DbgBreak::UpdateBreakPcbExcp(TraceCtrl* trace);
 * @brief Check UpdateBreakPcbExcp method.
 */
TEST_F(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp_1) {
  PhysAddr addr = 0;

  // m_srf->SrDIR1()->GetBEN() => 0
  EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
  EXPECT_CALL(*m_dir1, GetBEN()).WillOnce(Return(false));

  // check
  ExpCode res = m_gdbbreak->UpdateBreakPcbExcp(addr, m_trace);

  ASSERT_EQ(EXP_NONE, res);
};

TEST_F(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp_2) {
  PhysAddr addr = 0;
  // m_srf->SrDIR1()->GetBEN() => 1 && IsBrkExcpAcceptable () => false
  EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
  EXPECT_CALL(*m_dir1, GetBEN()).WillOnce(Return(true));
  EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
  EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(true));

  // check
  ExpCode res = m_gdbbreak->UpdateBreakPcbExcp(addr, m_trace);

  ASSERT_EQ(EXP_NONE, res);
};

TEST_F(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp_3) {
  PhysAddr addr = 0;

  // Prologue
  // m_srf->SrDIR1()->GetBEN() => 1 && IsBrkExcpAcceptable () => true
  EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
  EXPECT_CALL(*m_dir1, GetBEN()).WillOnce(Return(true));
  EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
  EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(false));

  for (int i = 0; i < static_cast<int>(DbgBreak::N_BPA); i++) {
    EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(0));
  }

  // check
  ExpCode res = m_gdbbreak->UpdateBreakPcbExcp(addr, m_trace);

  ASSERT_EQ(EXP_NONE, res);
};

TEST_F(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp_4) {
  PhysAddr addr = 0;

  // Prologue
  // m_srf->SrDIR1()->GetBEN() => 1 && IsBrkExcpAcceptable () => true
  EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
  EXPECT_CALL(*m_dir1, GetBEN()).WillOnce(Return(true));
  EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
  EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(false));

  // Test: CfMsg::DPrint (MSG_WAR, "<Warning: Unguaranteed setting in BPC%u RE/WE isn't 0 even
  // FE=1>\n", ch);
  EXPECT_CALL(*(m_bpc_vec[0]), GetFE()).WillOnce(Return(1));
  EXPECT_CALL(*(m_bpc_vec[1]), GetFE()).WillOnce(Return(1));
  for (int i = 2; i < static_cast<int>(DbgBreak::N_BPA); i++) {
    EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(0));
  }
  EXPECT_CALL(*(m_bpc_vec[0]), GetRE()).WillOnce(Return(1)); // ch0
  EXPECT_CALL(*(m_bpc_vec[1]), GetRE()).WillOnce(Return(0)); // ch1
  EXPECT_CALL(*(m_bpc_vec[1]), GetWE()).WillOnce(Return(1)); // ch1

  // check
  ExpCode res = m_gdbbreak->UpdateBreakPcbExcp(addr, m_trace);

  ASSERT_EQ(EXP_NONE, res);
};

TEST_F(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp_5) {
  PhysAddr addr = 0;

  InSequence s;
  // Prologue
  // m_srf->SrDIR1()->GetBEN() => 1 && IsBrkExcpAcceptable () => true
  EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
  EXPECT_CALL(*m_dir1, GetBEN()).WillOnce(Return(true));
  EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
  EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(false));
  int n_bpa = static_cast<int>(DbgBreak::N_BPA);
  for (int i = n_bpa - 1; i >= 0; --i) {
    if (i < 1) {
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(1));
      EXPECT_CALL(*(m_bpc_vec[i]), GetRE()).WillOnce(Return(0));
      EXPECT_CALL(*(m_bpc_vec[i]), GetWE()).WillOnce(Return(0));
      // isseqbrkhit (ch0) == false
      // return (m_srf->SrDIR1()->GetSQ0() == 0) || (...)
      EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
      EXPECT_CALL(*m_dir1, GetSQ0()).WillOnce(Return(1));
      // return (...) || (IsSeqBrkValidComb(i) == true)
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(0));
      // EXPECT_CALL(*(m_bpc_vec[i]), GetRE()).WillOnce(Return(1));
      // EXPECT_CALL(*(m_bpc_vec[i]), GetWE()).WillOnce(Return(1));
      // EXPECT_CALL(*(m_bpc_vec[i]), GetBE()).WillOnce(Return(1));
      // EXPECT_CALL(*(m_bpc_vec[i]), GetTE()).WillOnce(Return(1));
      // EXPECT_CALL(*(m_bpc_vec[i+1]), GetFE()).WillOnce(Return(0));
      // EXPECT_CALL(*(m_bpc_vec[i+1]), GetRE()).WillOnce(Return(1));
      // EXPECT_CALL(*(m_bpc_vec[i+1]), GetWE()).WillOnce(Return(1));
    } else {
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(0));
    }
  }

  // check
  ExpCode res = m_gdbbreak->UpdateBreakPcbExcp(addr, m_trace);

  ASSERT_EQ(EXP_NONE, res);
};

TEST_F(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp_6) {
  PhysAddr addr = 0x100;

  InSequence s;
  // Prologue
  // m_srf->SrDIR1()->GetBEN() => 1 && IsBrkExcpAcceptable () => true
  EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
  EXPECT_CALL(*m_dir1, GetBEN()).WillOnce(Return(true));
  EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
  EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(false));
  int n_bpa = static_cast<int>(DbgBreak::N_BPA);
  for (int i = n_bpa - 1; i >= 0; --i) {
    if (i == 0) {
      // BPC.VA==0 で アドレスがマッチしない
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(1));
      EXPECT_CALL(*(m_bpc_vec[i]), GetRE()).WillOnce(Return(0));
      EXPECT_CALL(*(m_bpc_vec[i]), GetWE()).WillOnce(Return(0));
      // isseqbrkhit (ch0) == false
      // return (m_srf->SrDIR1()->GetSQ0() == 0) || (...)
      EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
      EXPECT_CALL(*m_dir1, GetSQ0()).WillOnce(Return(0));
      // if (IsBrkAddrHit (ch, addr) == true) { => false
      {
        EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
        EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(1)); // Guest mode
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGPID()).WillOnce(Return(7));
        EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
        EXPECT_CALL(*m_dbgen, GetGE()).WillOnce(Return(0b10000000));
        EXPECT_CALL(*(m_bpc_vec[i]), GetGE()).WillOnce(Return(0b10000000));
      }
      EXPECT_CALL(*m_bpav0, ReadBody()).WillOnce(Return(0x1000));
      EXPECT_CALL(*m_bpam0, ReadBody()).WillOnce(Return(0xff));
      EXPECT_CALL(*(m_bpc_vec[i]), GetVA()).WillOnce(Return(0));
    } else if (i == 1) {
      // BPC.VA==1 で アドレスがマッチ
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(1));
      EXPECT_CALL(*(m_bpc_vec[i]), GetRE()).WillOnce(Return(0));
      EXPECT_CALL(*(m_bpc_vec[i]), GetWE()).WillOnce(Return(0));
      // isseqbrkhit (ch1) == false
      // return (m_srf->SrDIR1()->GetSQ0() == 0) || (...)
      EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
      EXPECT_CALL(*m_dir1, GetSQ0()).WillOnce(Return(0));
      // if (IsBrkAddrHit (ch, addr) == true) { => false
      {
        EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
        EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(1)); // Guest mode
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGPID()).WillOnce(Return(7));
        EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
        EXPECT_CALL(*m_dbgen, GetGE()).WillOnce(Return(0b10000000));
        EXPECT_CALL(*(m_bpc_vec[i]), GetGE()).WillOnce(Return(0b10000000));
      }
      EXPECT_CALL(*m_bpav1, ReadBody()).WillOnce(Return(0x100));
      EXPECT_CALL(*m_bpam1, ReadBody()).WillOnce(Return(0xff));
      EXPECT_CALL(*(m_bpc_vec[i]), GetVA()).WillOnce(Return(1));
    } else {
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(0));
    }
  }

  // check
  ExpCode res = m_gdbbreak->UpdateBreakPcbExcp(addr, m_trace);

  ASSERT_EQ(EXP_NONE, res);
};

TEST_F(DbgBreakMethodCheck, CheckUpdateBreakPcbExcp_7) {
  PhysAddr addr = 0x100;

  InSequence s;
  // Prologue
  // m_srf->SrDIR1()->GetBEN() => 1 && IsBrkExcpAcceptable () => true
  EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
  EXPECT_CALL(*m_dir1, GetBEN()).WillOnce(Return(true));
  EXPECT_CALL(*m_srf, SrDIR0()).WillOnce(Return(static_cast<SRegDIR0*>(m_dir0)));
  EXPECT_CALL(*m_dir0, GetDM()).WillOnce(Return(false));
  int n_bpa = static_cast<int>(DbgBreak::N_BPA);
  for (int i = n_bpa - 1; i >= 0; --i) {
    if (i == 0) {
      // BPC.VA==0 で アドレスがマッチ
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(1));
      EXPECT_CALL(*(m_bpc_vec[i]), GetRE()).WillOnce(Return(0));
      EXPECT_CALL(*(m_bpc_vec[i]), GetWE()).WillOnce(Return(0));
      // isseqbrkhit (ch0) == false
      // return (m_srf->SrDIR1()->GetSQ0() == 0) || (...)
      EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
      EXPECT_CALL(*m_dir1, GetSQ0()).WillOnce(Return(0));
      // if (IsBrkAddrHit (ch, addr) == true) { => true
      {
        EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
        EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(1)); // Guest mode
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGPID()).WillOnce(Return(7));
        EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
        EXPECT_CALL(*m_dbgen, GetGE()).WillOnce(Return(0b10000000));
        EXPECT_CALL(*(m_bpc_vec[i]), GetGE()).WillOnce(Return(0b10000000));
      }
      EXPECT_CALL(*m_bpav0, ReadBody()).WillOnce(Return(0x100));
      EXPECT_CALL(*m_bpam0, ReadBody()).WillOnce(Return(0xff));
      EXPECT_CALL(*(m_bpc_vec[i]), GetVA()).WillOnce(Return(0));
      // UpdateExpCodeAndSR (ch, EXP_PCB_DB, trace); ここに入ることが確認できればOK
      EXPECT_CALL(*(m_bpc_vec[i]), GetBE()).WillOnce(Return(0));
    } else if (i == 1) {
      // BPC.VA==1 で アドレスがマッチしない
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(1));
      EXPECT_CALL(*(m_bpc_vec[i]), GetRE()).WillOnce(Return(0));
      EXPECT_CALL(*(m_bpc_vec[i]), GetWE()).WillOnce(Return(0));
      // isseqbrkhit (ch1) == false
      // return (m_srf->SrDIR1()->GetSQ0() == 0) || (...)
      EXPECT_CALL(*m_srf, SrDIR1()).WillOnce(Return(static_cast<SRegDIR1*>(m_dir1)));
      EXPECT_CALL(*m_dir1, GetSQ0()).WillOnce(Return(0));
      // if (IsBrkAddrHit (ch, addr) == true) { => true
      {
        EXPECT_CALL(*m_srf, SrHVCFG()).WillRepeatedly(Return(static_cast<SRegHVCFG*>(m_hvcfg)));
        EXPECT_CALL(*m_hvcfg, GetHVE()).WillOnce(Return(1));
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGM()).WillOnce(Return(1)); // Guest mode
        EXPECT_CALL(*m_srf, SrPSWH()).WillRepeatedly(Return(static_cast<SRegPSWH*>(m_pswh)));
        EXPECT_CALL(*m_pswh, GetGPID()).WillOnce(Return(7));
        EXPECT_CALL(*m_srf, SrDBGEN()).WillRepeatedly(Return(static_cast<SRegDBGEN*>(m_dbgen)));
        EXPECT_CALL(*m_dbgen, GetGE()).WillOnce(Return(0b10000000));
        EXPECT_CALL(*(m_bpc_vec[i]), GetGE()).WillOnce(Return(0b10000000));
      }
      EXPECT_CALL(*m_bpav1, ReadBody()).WillOnce(Return(0x1000));
      EXPECT_CALL(*m_bpam1, ReadBody()).WillOnce(Return(0xff));
      EXPECT_CALL(*(m_bpc_vec[i]), GetVA()).WillOnce(Return(1));
      // UpdateExpCodeAndSR (ch, EXP_PCB_DB, trace); ここに入ることが確認できればOK
      EXPECT_CALL(*(m_bpc_vec[i]), GetBE()).WillOnce(Return(0));
    } else {
      EXPECT_CALL(*(m_bpc_vec[i]), GetFE()).WillOnce(Return(0));
    }
  }

  // check
  ExpCode res = m_gdbbreak->UpdateBreakPcbExcp(addr, m_trace);

  ASSERT_EQ(EXP_NONE, res);
};

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

#endif //  ENABLE_DBG_BREAK
