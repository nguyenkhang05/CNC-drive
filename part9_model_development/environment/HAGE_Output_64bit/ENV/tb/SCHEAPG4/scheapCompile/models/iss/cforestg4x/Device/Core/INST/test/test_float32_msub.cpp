/**
 * @file test_float32_madd.cpp
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for float32_madd.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <cstdint>
#include <cstddef>
#include "gmock/gmock.h"
#include "mock_SoftFloatAdapter.h"

using ::testing::ExpectationSet;
using ::testing::Expectation;

class FMA32Test : public testing::Test,
                  public testing::WithParamInterface<
                      std::tuple<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
                                 uint32_t, uint32_t, uint32_t>> {
protected:
  FMA32Test() {}
  virtual ~FMA32Test() {}
  virtual void SetUp() { m_sf = new MockSoftFloat(); }
  virtual void TearDown() { delete m_sf; }

public:
  MockSoftFloat* m_sf;

private:
};

/*!
 * @brief Check f32_mulSub.
 * SoftFloat-3b
 */
TEST_P(FMA32Test, CheckF32MulSub) {
  uint32_t reg1 = std::get<0>(GetParam());
  uint32_t reg2 = std::get<1>(GetParam());
  uint32_t reg3 = std::get<2>(GetParam());
  uint32_t rm = std::get<3>(GetParam()); // FPSR.RM
  uint32_t fn = std::get<4>(GetParam()); // FPSR.FN
  uint32_t fs = std::get<5>(GetParam()); // FPSR.FS
  uint32_t xe_o = std::get<6>(GetParam()); // FPSR.XE.O
  uint32_t xe_u = std::get<7>(GetParam()); // FPSR.XE.U
  uint32_t expect = std::get<8>(GetParam());
  uint32_t expect_evzoui = std::get<9>(GetParam());

  float32_t v1;
  v1.v = reg1;
  float32_t v2;
  v2.v = reg2;
  float32_t v3;
  v3.v = reg3;

  // Expect operations
  // Check the conditions necessary to call the SoftFloat-3b library
  ExpectationSet inits;
  Expectation op;
  // clear 3b
  inits += EXPECT_CALL((*m_sf), SetExceptionFlags(0)).Times(1);
  // clear IF bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetInputFlushFlag(false)).Times(1);
  // set FS bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetFlushEnable(_)).Times(1);
  // set FN bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetFlushNearestEnable(_)).Times(1);
  // set RM bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetRoundingMode(_)).Times(1);
  // set XE_O bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetOverflowExceptionEnableFlag(_)).Times(1);
  // set XE_U bit from fpsr to softfloat
  inits += EXPECT_CALL((*m_sf), SetUnderflowExceptionEnableFlag(_)).Times(1);
  // call softfloat func
  op = EXPECT_CALL((*m_sf), f32_mulSub(_,_,_)).Times(1).After(inits);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetExceptionFlags()).Times(AtLeast(1)).After(op);
  // read IF bit from softfloat to fpsr
  EXPECT_CALL((*m_sf), GetInputFlushFlag()).Times(AtLeast(0)).After(op);

  m_sf->SetExceptionFlags(0);
  m_sf->SetFlushEnable(fs);
  m_sf->SetFlushNearestEnable(fn);
  m_sf->SetRoundingMode(rm);
  m_sf->SetInputFlushFlag(false);
  m_sf->SetOverflowExceptionEnableFlag(xe_o);
  m_sf->SetUnderflowExceptionEnableFlag(xe_u);
  float32_t ret = m_sf->f32_mulSub(v2, v1, v3);

  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, ret.v);
}

std::tuple<uint32_t, uint32_t, uint32_t,                     // v1, v2, v3
           uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, // rm(2bit), fn(1bit), fs(1bit), xe_o(1bit), xe_u(1bit)
           uint32_t, uint32_t                                // expect, exp_evzoui(6bit)
           > const new_float32_msub_ParametersTable[] = {
  /* 0000 */ std::make_tuple(0x4b800001, 0x4b800001, 0xc0800001, 0b01, 0b0, 0b1, 0b0, 0b0, 0x57800002, 0b000001),
  /* 0001 */ std::make_tuple(0x80000000, 0x411e064f, 0x00000000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000000),
  /* 0002 */ std::make_tuple(0xc15ee9d6, 0xfe005fe0, 0x0024fdee, 0b11, 0b1, 0b1, 0b1, 0b0, 0x7f7fffff, 0b000101), // Bug fixes #3437
  /* 0003 */ std::make_tuple(0xc15ee9d6, 0xfe005fe0, 0x4b800001, 0b11, 0b1, 0b1, 0b1, 0b0, 0x7f7fffff, 0b000100), // Bug fixes #3437
  /* 0004 */ std::make_tuple(0xc15ee9d6, 0xfe005fe0, 0x4b800001, 0b11, 0b1, 0b1, 0b0, 0b0, 0x7f7fffff, 0b000101), // Bug fixes #3437
  // please write more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParameters2, FMA32Test,
                        ::testing::ValuesIn(new_float32_msub_ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
