/**
 * @file test_mpu_ismpupermitteddefng_inst.h
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

class MpuMethodCheck
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<bool, bool, bool, bool, bool, bool, bool, bool, Mpu::faultPos>> {
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
    m_hvcfg = new MockSRegHVCFG(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_HVCFG, m_hvcfg);
    m_pswh = new MockSRegPSWH(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_PSWH, m_pswh);
    m_vmpcfg = new MockSRegMPCFG(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_MPCFG, m_vmpcfg);
    m_vmpm = new MockSRegGMMPM(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VMPM, m_vmpm);
    m_vpsw = new MockSRegGMPSW(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VPSW, m_vpsw);
    m_vspid = new MockSRegGMSPID(ctxt_id, m_pe); m_srf->SetSrPtr(CF_SR_VSPID, m_vspid);
    m_mpu = new Mpu(m_trace, m_srf, core_type, core_version);
  };
  virtual void TearDown() {
    m_mpu->AllDelMpuRegion();
    delete m_mpu;
    delete m_srf;
    delete m_ncsreg;
    delete sreg_dummy;
    delete m_pe;
    delete m_trace;
    for (MockSRegMPLA_N* element : m_mpla_vec) { delete element; }; m_mpla_vec.clear();
    for (MockSRegMPUA_N* element : m_mpua_vec) { delete element; }; m_mpua_vec.clear();
    for (MockSRegMPAT_N* element : m_mpat_vec) { delete element; }; m_mpat_vec.clear();
    delete m_hvcfg;
    delete m_pswh;
    delete m_vmpcfg;
    delete m_vmpm;
    delete m_vpsw;
    delete m_vspid;
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
  MockSRegMPCFG* m_vmpcfg;
  MockSRegGMMPM* m_vmpm;
  MockSRegGMPSW* m_vpsw;
  MockSRegGMSPID* m_vspid;
  MockSRegHVCFG* m_hvcfg;
  MockSRegPSWH* m_pswh;

private:
};

#define GUEST_ENTRY 0
#define HOST_ENTRY 1

#define SET_NOT_HIT(INDEX) EXPECT_CALL(*(m_mpla_vec[INDEX]), ReadBody()).Times(2).WillRepeatedly(Return(addr + size))
#define SET_HIT_PERMIT(INDEX) \
        EXPECT_CALL(*(m_mpla_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr)); \
        EXPECT_CALL(*(m_mpua_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr + size - 1)); \
        if (um) {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetUX()).WillOnce(Return(true));} \
        else {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetSX()).WillOnce(Return(true));} \
        EXPECT_CALL(*(m_mpla_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr)); \
        EXPECT_CALL(*(m_mpua_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr + size - 1)); \
        if (um) {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetUX()).WillOnce(Return(true));} \
        else {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetSX()).WillOnce(Return(true));}
#define SET_HIT_NOT_PERMIT(INDEX) \
        EXPECT_CALL(*(m_mpla_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr)); \
        EXPECT_CALL(*(m_mpua_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr + size - 1)); \
        if (um) {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetUX()).WillOnce(Return(true));} \
        else {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetSX()).WillOnce(Return(true));} \
        EXPECT_CALL(*(m_mpla_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr)); \
        EXPECT_CALL(*(m_mpua_vec[INDEX]), ReadBody()).WillRepeatedly(Return(addr + size - 1)); \
        if (um) {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetUX()).WillOnce(Return(false));} \
        else {EXPECT_CALL(*(m_mpat_vec[INDEX]), GetSX()).WillOnce(Return(false));}
