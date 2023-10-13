/**
 * @file test_float32_madd_full_rm_neareven.cpp
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for float32_madd.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "softfloat_test_common.h"
#include "mock_SoftFloatAdapter.h"

std::vector<std::vector<std::string>> softfloat_muladd_neareven;

char *test_data_dir;


class F32_Muladd_Test : public testing::Test,
                        public testing::WithParamInterface<std::vector<std::string>> {
protected:
  F32_Muladd_Test() {}
  virtual ~F32_Muladd_Test() {}
  virtual void SetUp() { m_sf = new SoftFloat(); }
  virtual void TearDown() { delete m_sf; }

public:
  SoftFloat *m_sf;

private:
};

/*!
 * @brief Check f32_mulAdd.
 * SoftFloat-3a
 */
TEST_P(F32_Muladd_Test, CheckF32MulAdd) {
  std::vector<std::string> cell = GetParam();
  uint32_t reg1 = s2ul_32(cell[0]);
  uint32_t reg2 = s2ul_32(cell[1]);
  uint32_t reg3 = s2ul_32(cell[2]);
  uint32_t rm = s2ul_32(cell[3]);
  uint32_t fn = s2ul_32(cell[4]);
  uint32_t fs = s2ul_32(cell[5]);
  uint32_t xe = 0;
  uint32_t expect = s2ul_32(cell[6]);
  uint32_t expect_evzoui = s2ul_32(cell[7]);

  float32_t v1;
  v1.v = reg1;
  float32_t v2;
  v2.v = reg2;
  float32_t v3;
  v3.v = reg3;

  m_sf->SetExceptionFlags(0);
  m_sf->SetFlushEnable(fs);
  m_sf->SetFlushNearestEnable(fn);
  m_sf->SetRoundingMode(rm);
  m_sf->SetInputFlushFlag(false);
  m_sf->SetOverflowExceptionEnableFlag(xe & m_sf->FPE_O);
  m_sf->SetUnderflowExceptionEnableFlag(xe & m_sf->FPE_U);
  float32_t ret = m_sf->f32_mulAdd(v2, v1, v3);

  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, ret.v);
}

INSTANTIATE_TEST_CASE_P(Romeo_FMAF_near_even_1M_0, F32_Muladd_Test,
                        ::testing::ValuesIn(softfloat_muladd_neareven.begin(),
                                            softfloat_muladd_neareven.end()));

int main(int argc, char **argv) {
  ::test_data_dir = argv[argc - 1];
  {
    ::testing::internal::FilePath filepath = ::testing::internal::FilePath::ConcatPaths(
        ::testing::internal::FilePath(test_data_dir),
        ::testing::internal::FilePath("data/Romeo_FMAF_near_even_1M_0.csv"));
    std::ifstream inFile(filepath.string());
    if (!inFile) {
      throw std::runtime_error("Could not open test data for input");
    }
    softfloat_muladd_neareven = parseTable(inFile, ",");
    inFile.close();
  }
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
