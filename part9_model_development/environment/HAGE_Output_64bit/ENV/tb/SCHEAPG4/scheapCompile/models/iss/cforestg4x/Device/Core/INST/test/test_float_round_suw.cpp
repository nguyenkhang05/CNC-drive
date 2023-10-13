/**
 * @file test_float_round_suw.cpp
 *
 * @date Created on: 2017
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for roundf.suw.
 * @par Copyright: 2017 Renesas Electronics Corp.
 */

#include <cstdint>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include "gmock/gmock.h"
#include "fpu_device.h"
#include "SoftFloatAdapter.h"
#include "softfloat_test_common.h"
#include "mock_SoftFloatAdapter.h"

using ::testing::ExpectationSet;
using ::testing::Expectation;
using ::testing::AtLeast;

std::vector<std::vector<std::string>> softfloat_roundf_suw;

char *test_data_dir;


class ROUNDFSUWTest : public testing::Test,
public testing::WithParamInterface<std::vector<std::string>> {
protected:
  ROUNDFSUWTest() {}
  virtual ~ROUNDFSUWTest() {}
  virtual void SetUp() {
    m_sf = new MockSoftFloat();
  }
  virtual void TearDown() {
    delete m_sf;
  }

public:
  MockSoftFloat* m_sf;

private:
};


/*!
 * @brief Check roundf.suw
 * SoftFloat-3b modified
 */
TEST_P(ROUNDFSUWTest, CheckROUNDFSUW) {
  std::vector<std::string> cell = GetParam();

  uint32_t reg1 = s2ul_32(cell[0]);
  // uint32_t rm = s2ul_32(cell[1]);  // FPSR.RM
  uint32_t fn = s2ul_32(cell[2]);  // FPSR.FN
  uint32_t fs = s2ul_32(cell[3]);  // FPSR.FS
  uint32_t xe_o = s2ul_32(cell[4]);  // FPSR.XE_O
  uint32_t xe_u = s2ul_32(cell[5]);  // FPSR.XE_U
  uint32_t expect = s2ul_32(cell[6]);
  uint32_t expect_evzoui = s2ul_32(cell[7]);

  float32_t v1;
  v1.v = reg1;

  // Expect operations
  // Check the conditions necessary to call the SoftFloat-3a library
  ExpectationSet inits;
  Expectation op;
  // clear 3a
  inits += EXPECT_CALL((*m_sf), SetExceptionFlags(0)).Times(1);
  // clear IF bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetInputFlushFlag(false)).Times(1);
  // set FS bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetFlushEnable(_)).Times(1);
  // set FN bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetFlushNearestEnable(_)).Times(1);
  // set RM bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetRoundingMode(_)).Times(1);
  // set XE/O bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetOverflowExceptionEnableFlag(_)).Times(1);
  // set XE/U bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetUnderflowExceptionEnableFlag(_)).Times(1);
  // call softfloat func
  op = EXPECT_CALL((*m_sf), Cvt_f32_ui32(_)).Times(1).After(inits);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetExceptionFlags()).Times(1).After(op);
  // read IF bit from softfloat to fpsr
  EXPECT_CALL((*m_sf), GetInputFlushFlag()).Times(AtLeast(0)).After(op);

  m_sf->SetExceptionFlags(0);
  m_sf->SetFlushEnable(fs);
  m_sf->SetFlushNearestEnable(fn);
  m_sf->SetRoundingMode(SoftFloat::RM_softfloat_round_near_even);
  m_sf->SetInputFlushFlag(false);
  m_sf->SetOverflowExceptionEnableFlag(xe_o);
  m_sf->SetUnderflowExceptionEnableFlag(xe_u);
  uint32_t ret = static_cast<uint32_t> (m_sf->Cvt_f32_ui32(v1));

  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, ret);
}


INSTANTIATE_TEST_CASE_P(TestNewSoftFloatCvtfSUW, ROUNDFSUWTest,
        ::testing::ValuesIn(softfloat_roundf_suw.begin(),
                            softfloat_roundf_suw.end()));


int main(int argc, char **argv) {
  ::test_data_dir = argv[argc - 1];
  {
    ::testing::internal::FilePath filepath = ::testing::internal::FilePath::ConcatPaths(
        ::testing::internal::FilePath(test_data_dir),
        ::testing::internal::FilePath("data/UT_roundf_suw.csv"));
    std::ifstream inFile(filepath.string());
    if (!inFile) {
      throw std::runtime_error("Could not open test data for input");
    }
    softfloat_roundf_suw = parseTable(inFile, ",");
    inFile.close();
  }
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
