/**
 * @file test_float_cvt_hs.cpp
 *
 * @date Created on: 2016
 * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for f32 to f16
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <bitset>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "gmock/gmock.h"
#include "fpu_device.h"
#include "SoftFloatAdapter.h"
#include "softfloat_test_common.h"
#include "mock_SoftFloatAdapter.h"

using ::testing::ExpectationSet;
using ::testing::Expectation;
using ::testing::AtLeast;

std::vector<std::vector<std::string>> softfloat_convert_max;

char *test_data_dir;


class CVTF_HSTest : public testing::Test,
                    public testing::WithParamInterface<std::vector<std::string>> {
protected:
CVTF_HSTest() {}
  virtual ~CVTF_HSTest() {}
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
 * @brief Check f16_to_f32
 * SoftFloat-3a
 */
TEST_P(CVTF_HSTest, Check_CVTF_HS) {
  std::vector<std::string> cell = GetParam();
  uint16_t reg1 = s2ul_16(cell[0]);
  uint32_t rm = s2ul_32(cell[1]);
  uint32_t fn = s2ul_32(cell[2]);
  uint32_t fs = s2ul_32(cell[3]);
  uint32_t xe_o = s2ul_32(cell[4]);
  uint32_t xe_u = s2ul_32(cell[5]);
  uint32_t expect = s2ul_32(cell[6]);
  uint32_t expect_evzoui = s2ul_32(cell[7]);


  float16_t v1;
  v1.v = reg1;

  // Expect operations
  // Check the conditions necessary to call the SoftFloat-3a library
  ExpectationSet inits;
  Expectation op;

  // clear 3a
  inits += EXPECT_CALL((*m_sf), SetExceptionFlags(0)).Times(1);
  // clear IF bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetInputFlushFlag(0)).Times(1);
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
  op = EXPECT_CALL((*m_sf), f16_to_f32(_)).Times(1).After(inits);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetExceptionFlags()).Times(1).After(op);
  // read IF bit from softfloat to fpsr
  EXPECT_CALL((*m_sf), GetInputFlushFlag()).Times(AtLeast(0)).After(op);


  uint32_t exp_code = 0;

  m_sf->SetExceptionFlags(exp_code);
  m_sf->SetFlushEnable(fs);
  m_sf->SetFlushNearestEnable(fn);
  m_sf->SetRoundingMode(rm);
  m_sf->SetInputFlushFlag(0);
  m_sf->SetOverflowExceptionEnableFlag(xe_o);
  m_sf->SetUnderflowExceptionEnableFlag(xe_u);

  float32_t ret;
  ret = m_sf->f16_to_f32(v1);

  uint32_t res = ret.v;
  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, res);

  }

INSTANTIATE_TEST_CASE_P(quebec_cvtf_hs, CVTF_HSTest,
                        ::testing::ValuesIn(softfloat_convert_max.begin(),
                                            softfloat_convert_max.end()));

int main(int argc, char **argv) {
  ::test_data_dir = argv[argc - 1];
  {
    ::testing::internal::FilePath filepath = ::testing::internal::FilePath::ConcatPaths(
        ::testing::internal::FilePath(test_data_dir),
        ::testing::internal::FilePath("data/UT_cvtf_hs.csv"));
    std::ifstream inFile(filepath.string());
    if (!inFile) {
      throw std::runtime_error("Could not open test data for input");
    }
    softfloat_convert_max = parseTable(inFile, ",");
    inFile.close();
  }
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
