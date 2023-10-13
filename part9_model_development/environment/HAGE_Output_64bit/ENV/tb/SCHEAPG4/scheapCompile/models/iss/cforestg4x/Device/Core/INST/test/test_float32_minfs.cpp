/**
 * @file test_float32_minfs.cpp
 *
 * @date Created on: 2016
 * @author Can Vy
 * @brief UNIT TEST for float32_minfs.
 * @par Copyright: 2016 Renesas Electronics Corp.
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
#include "softfloat_test_common.h"
#include "mock_SoftFloatAdapter.h"


using ::testing::ExpectationSet;
using ::testing::Expectation;
using ::testing::AtLeast;


std::vector<std::vector<std::string>> softfloat_minfs;

char *test_data_dir;


class MINFSTest : public testing::Test,
public testing::WithParamInterface<std::vector<std::string>> {
protected:
  MINFSTest() {}
  virtual ~MINFSTest() {}
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
 * @brief Check f32_minfs
* SoftFloat-3b
*/
TEST_P(MINFSTest, CheckF32Min) {
  std::vector<std::string> cell = GetParam();
  uint32_t reg1 = s2ul_32(cell[0]);
  uint32_t reg2 = s2ul_32(cell[1]);
  uint32_t rm = s2ul_32(cell[2]);  // FPSR.RM
  uint32_t fn = s2ul_32(cell[3]);  // FPSR.FN
  uint32_t fs = s2ul_32(cell[4]);  // FPSR.FS
  uint32_t xe_o = s2ul_32(cell[5]);  // FPSR.XE.O
  uint32_t xe_u = s2ul_32(cell[6]);  // FPSR.XE.U
  uint32_t expect = s2ul_32(cell[7]);
  uint32_t expect_evzoui = s2ul_32(cell[8]);

  float32_t v1;
  v1.v = reg1;
  float32_t v2;
  v2.v = reg2;

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
  op = EXPECT_CALL((*m_sf), f32_minfs(_,_)).Times(1).After(inits);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetExceptionFlags()).Times(1).After(op);
  // read IF bit from softfloat to fpsr
  EXPECT_CALL((*m_sf), GetInputFlushFlag()).Times(AtLeast(0)).After(op);

  m_sf->SetExceptionFlags(0);
  m_sf->SetFlushEnable(fs);
  m_sf->SetFlushNearestEnable(fn);
  m_sf->SetRoundingMode(rm);
  m_sf->SetInputFlushFlag(false);
  m_sf->SetOverflowExceptionEnableFlag(xe_o);
  m_sf->SetUnderflowExceptionEnableFlag(xe_u);
  float32_t ret = m_sf->f32_minfs(v2, v1);
  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, ret.v);
}
INSTANTIATE_TEST_CASE_P(TestNewSoftFloatMinfs, MINFSTest,
        ::testing::ValuesIn(softfloat_minfs.begin(),
                            softfloat_minfs.end()));

int main(int argc, char **argv) {
  ::test_data_dir = argv[argc - 1];
  {
    ::testing::internal::FilePath filepath = ::testing::internal::FilePath::ConcatPaths(
        ::testing::internal::FilePath(test_data_dir),
        ::testing::internal::FilePath("data/UT_minf_s.csv"));
    std::ifstream inFile(filepath.string());
    if (!inFile) {
      throw std::runtime_error("Could not open test data for input");
    }
    softfloat_minfs = parseTable(inFile, ",");
    inFile.close();
  }
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
