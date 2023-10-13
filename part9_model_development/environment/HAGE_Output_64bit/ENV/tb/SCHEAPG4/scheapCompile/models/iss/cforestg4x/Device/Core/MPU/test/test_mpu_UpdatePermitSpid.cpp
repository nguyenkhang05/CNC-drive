/**
 * @file test_mpu_UpdatePermitSpid.cpp
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
// using ::testing::ReturnRef;
// using ::testing::ReturnPointee;
// using ::testing::InSequence;

static const uint32_t N_SPID = 32; // From mpu.h Mpu::MpuRegion::N_SPID
static const uint32_t N_MPID = 8;  // From mpu.h Mpu::MpuRegion::N_MPID

class MpuMethodCheck : public testing::Test,
                       public testing::WithParamInterface<
                           std::tuple<bool, std::bitset<N_SPID>, bool, std::bitset<N_SPID>>> {
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
 * void AddMpuRegion(uint32_t rgnid);
 * @brief Check AddMpuRegion method.
 */
TEST_P(MpuMethodCheck, CheckUpdatePermitSpid) {
  bool wg = std::get<0>(GetParam());
  std::bitset<N_SPID> expect_w = std::get<1>(GetParam());
  bool rg = std::get<2>(GetParam());
  std::bitset<N_SPID> expect_r = std::get<3>(GetParam());

  for (int idx = 0; idx < N_MPU_REGION; idx++) {
    EXPECT_CALL(*(m_mpat_vec[idx]), GetWG()).WillOnce(Return(wg)); // true or false
    EXPECT_CALL(*(m_mpat_vec[idx]), GetRG()).WillOnce(Return(rg)); // true or false

    // (MPID0, MPID1, MPID2, MPID3, MPID4, MPID5, ...) =
    // (    1,     5,     0,     2,     4,     3, ...)
    if(!wg){
		if (expect_w == 0x00000013) {
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0x15)); // 0b10101 -> bit count is 3

			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(1));            // set of bit 1
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(0));            // set of bit 0
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(4));            // set of bit 4
		    //result = 0x00000013
		}else if (expect_w == 0xAA000000) {
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0x55)); // 0b01010101 -> bit count is 4

			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(25));
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(27));
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(29));
			EXPECT_CALL(*(m_mpid[6]), GetSPID()).WillOnce(Return(31));
			//result = 0xAA000000
		}else if (expect_w == 0x55000000) {
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0x55)); // 0b01010101 -> bit count is 4

			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(24));
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(26));
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(28));
			EXPECT_CALL(*(m_mpid[6]), GetSPID()).WillOnce(Return(30));
			//result = 0x55000000
		}else if (expect_w == 0x00A50000){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0xAA)); // 0b10101010 -> bit count is 4
			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(23));
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(21));
			EXPECT_CALL(*(m_mpid[5]), GetSPID()).WillOnce(Return(16));
			EXPECT_CALL(*(m_mpid[7]), GetSPID()).WillOnce(Return(18));
			//result = 0x00A50000
		}else if (expect_w == 0x005A0000){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0xAA)); // 0b10101010 -> bit count is 4
			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(22));
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(20));
			EXPECT_CALL(*(m_mpid[5]), GetSPID()).WillOnce(Return(17));
			EXPECT_CALL(*(m_mpid[7]), GetSPID()).WillOnce(Return(19));
			//result = 0x005A0000
		}
		else if (expect_w == 0x00000002){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0xAA)); // 0b10101010 -> bit count is 4
			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(1));
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(1));
			EXPECT_CALL(*(m_mpid[5]), GetSPID()).WillOnce(Return(1));
			EXPECT_CALL(*(m_mpid[7]), GetSPID()).WillOnce(Return(1));
			//result = 0x00000002
		}
		else if (expect_w == 0xAAAA0000){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0xFF)); // 0b11111111 -> bit count is 8
			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(31));
			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(29));
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(27));
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(25));
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(23));
			EXPECT_CALL(*(m_mpid[5]), GetSPID()).WillOnce(Return(21));
			EXPECT_CALL(*(m_mpid[6]), GetSPID()).WillOnce(Return(19));
			EXPECT_CALL(*(m_mpid[7]), GetSPID()).WillOnce(Return(17));
			//result = 0xAAAA0000
		}
		else if (expect_w == 0x00000000){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetWMPID()).WillOnce(Return(0x00)); // 0b0 -> bit count is 0
			//result = 0x00000000
		}
    }
    if(!rg){
		if (expect_r==0x00000024) {
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0xa)); // 0b1010 -> bit count is 2

			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(5));            // set of bit 5
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(2));            // set of bit 2
		  // result 0b100100 = 0x24
		}else if (expect_r==0x000000AA){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0xAA)); // 0b10101010 -> bit count is 4
			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(1));            // set of bit 5
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(3));            // set of bit 2
			EXPECT_CALL(*(m_mpid[5]), GetSPID()).WillOnce(Return(5));
			EXPECT_CALL(*(m_mpid[7]), GetSPID()).WillOnce(Return(7));
			//result = 0x000000AA
		}
		else if (expect_r==0x00000055){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0xAA)); // 0b10101010 -> bit count is 4
			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(0));            // set of bit 5
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(2));            // set of bit 2
			EXPECT_CALL(*(m_mpid[5]), GetSPID()).WillOnce(Return(4));
			EXPECT_CALL(*(m_mpid[7]), GetSPID()).WillOnce(Return(6));
			//result = 0x00000055
		}
		else if (expect_r==0x0000A500){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0x55)); // 0b01010101 -> bit count is 4
			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(15));
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(13));
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(10));
			EXPECT_CALL(*(m_mpid[6]), GetSPID()).WillOnce(Return(8));
			//result = 0x0000A500
		}
		else if (expect_r==0x00005A00){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0x55)); // 0b01010101 -> bit count is 4
			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(14));
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(12));
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(11));
			EXPECT_CALL(*(m_mpid[6]), GetSPID()).WillOnce(Return(9));
			//result = 0x00005A00
		}
		else if (expect_r==0x00000002){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0x55)); // 0b01010101 -> bit count is 4
			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(1));
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(1));
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(1));
			EXPECT_CALL(*(m_mpid[6]), GetSPID()).WillOnce(Return(1));
			//result = 0x00000002
		}
		else if (expect_r==0x00005555){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0xFF)); // 0b11111111 -> bit count is 8
			EXPECT_CALL(*(m_mpid[0]), GetSPID()).WillOnce(Return(0));
			EXPECT_CALL(*(m_mpid[1]), GetSPID()).WillOnce(Return(2));
			EXPECT_CALL(*(m_mpid[2]), GetSPID()).WillOnce(Return(4));
			EXPECT_CALL(*(m_mpid[3]), GetSPID()).WillOnce(Return(6));
			EXPECT_CALL(*(m_mpid[4]), GetSPID()).WillOnce(Return(8));
			EXPECT_CALL(*(m_mpid[5]), GetSPID()).WillOnce(Return(10));
			EXPECT_CALL(*(m_mpid[6]), GetSPID()).WillOnce(Return(12));
			EXPECT_CALL(*(m_mpid[7]), GetSPID()).WillOnce(Return(14));
			//result = 0x00005555
		}else if (expect_r==0x00000000){
			EXPECT_CALL(*(m_mpat_vec[idx]), GetRMPID()).WillOnce(Return(0x00)); // 0b0 -> bit count is 0
			//result = 0x00000000
		}
    }
    // check
    m_mpu->AddMpuRegion(idx);
    // 上記の手順でCALLされなければgmockの警告が出る
    //    GMOCK WARNING: Uninteresting mock function call ...
    ASSERT_EQ(expect_w, m_mpu->m_mprgn_vec[idx].m_write_permit_spid);
    ASSERT_EQ(expect_r, m_mpu->m_mprgn_vec[idx].m_read_permit_spid);
  }
}
std::tuple<bool, std::bitset<N_SPID>, bool, std::bitset<N_SPID>> const ParametersTable1[] = {
  /* 0000 */ std::make_tuple(true,  0xffffffff, true, 0xffffffff),
  /* 0001 */ std::make_tuple(true,  0xffffffff, false,0x00000024),
  /* 0002 */ std::make_tuple(false, 0x00000013, true, 0xffffffff),
  /* 0003 */ std::make_tuple(false, 0x00000013, false,0x00000024),
  // please write more more more test case.
  /* 0004 */ std::make_tuple(false, 0xAA000000, false,0x000000AA),
  /* 0005 */ std::make_tuple(false, 0x55000000, false,0x00000055),
  /* 0006 */ std::make_tuple(false, 0x00A50000, false,0x0000A500),
  /* 0007 */ std::make_tuple(false, 0x005A0000, false,0x00005A00),
  /* 0008 */ std::make_tuple(false, 0x00000002, false,0x00000002),
  /* 0009 */ std::make_tuple(false, 0xAAAA0000, true, 0xffffffff),
  /* 0010 */ std::make_tuple(true,  0xffffffff, false,0x00005555),
  /* 0011 */ std::make_tuple(false, 0x00000000, true, 0xffffffff),
  /* 0012 */ std::make_tuple(true,  0xffffffff, false,0x00000000),
};
INSTANTIATE_TEST_CASE_P(TestWithParameters1, MpuMethodCheck, ::testing::ValuesIn(ParametersTable1));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
