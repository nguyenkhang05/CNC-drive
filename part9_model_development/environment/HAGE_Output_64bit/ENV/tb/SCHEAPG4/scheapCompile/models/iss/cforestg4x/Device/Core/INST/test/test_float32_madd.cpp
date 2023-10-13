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
 * @brief Check f32_mulAdd.
 * SoftFloat-3b
 */
TEST_P(FMA32Test, CheckF32MulAdd) {
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
  op = EXPECT_CALL((*m_sf), f32_mulAdd(_,_,_)).Times(1).After(inits);
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
  float32_t ret = m_sf->f32_mulAdd(v2, v1, v3);

  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, ret.v);
}

std::tuple<uint32_t, uint32_t, uint32_t,                     // v1, v2, v3
           uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, // rm(2bit), fn(1bit), fs(1bit), xe_o(1bit), xe_u(1bit)
           uint32_t, uint32_t                                // expect, exp_evzoui(6bit)
           > const new_float32_madd_ParametersTable[] = {
  /* 0 */   std::make_tuple(0x4b800001, 0x4b800001, 0xc0800001, 0b01, 0b0, 0b1, 0b0, 0b0, 0x57800001, 0b000001),
  // FMAF Flush
  /* 1 */   std::make_tuple(0x00800000, 0x80800000, 0x00800000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000011),
  /* 2 */   std::make_tuple(0x00800000, 0x80800000, 0x00800000, 0b00, 0b0, 0b0, 0b0, 0b0, 0x00000000, 0b100000),
  // Romeo_FMAF_max_1M_0.testcase
  /* 3 */   std::make_tuple(0x8683f7ff, 0xc07f3fff, 0x00000000, 0b10, 0b0, 0b1, 0b0, 0b0, 0x07839505, 0b000001),
  /* 4 */   std::make_tuple(0x00000000, 0x00000000, 0x3c072c85, 0b10, 0b0, 0b1, 0b0, 0b0, 0x3c072c85, 0b000000),
  /* 5 */   std::make_tuple(0x9ede38f7, 0x3e7f7f7f, 0xdf7effff, 0b10, 0b0, 0b1, 0b0, 0b0, 0xdf7effff, 0b000001),
  /* 6 */   std::make_tuple(0x4f951295, 0x00000000, 0x00000000, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 7 */   std::make_tuple(0x00000000, 0x41e00002, 0xc2800040, 0b10, 0b0, 0b1, 0b0, 0b0, 0xc2800040, 0b000000),
  /* 8 */   std::make_tuple(0x4fffdff7, 0x00000000, 0xbfffffcf, 0b10, 0b0, 0b1, 0b0, 0b0, 0xbfffffcf, 0b000000),
  /* 9 */   std::make_tuple(0x00000000, 0x015e834a, 0x00000000, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 10 */  std::make_tuple(0xc700ffbf, 0xce7c0007, 0xbe5fefff, 0b10, 0b0, 0b1, 0b0, 0b0, 0x55fdf788, 0b000001),
  /* 11 */  std::make_tuple(0x417febff, 0xce7d4590, 0x00000001, 0b10, 0b0, 0b1, 0b0, 0b0, 0xd07d31c5, 0b000001),
  /* 12 */  std::make_tuple(0x41ffffeb, 0x137f7ffb, 0xa68002fe, 0b10, 0b0, 0b1, 0b0, 0b0, 0xa68002fd, 0b000001),
  /* 13 */  std::make_tuple(0x5fffeeff, 0xdff80000, 0xb2fffdbe, 0b10, 0b0, 0b1, 0b0, 0b0, 0xff7fffff, 0b000101),
  /* 14 */  std::make_tuple(0x7f22c563, 0x437bfffe, 0xfc81007f, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7f800000, 0b000101),
  /* 15 */  std::make_tuple(0xfeffe03e, 0xcb80037f, 0x3f800000, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7f800000, 0b000101),
  /* 16 */  std::make_tuple(0x3fffffc2, 0xff0000f7, 0x3ffffffe, 0b10, 0b0, 0b1, 0b0, 0b0, 0xff7fffff, 0b000101),
  /* 17 */  std::make_tuple(0xdf01fffd, 0xf7dffffe, 0x5f7e7fff, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7f800000, 0b000101),
  /* 18 */  std::make_tuple(0x15ffbff7, 0xa17ff7fc, 0x00000001, 0b10, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 19 */  std::make_tuple(0x3e800040, 0x00800000, 0x808a0380, 0b10, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 20 */  std::make_tuple(0x00800103, 0x80fff80f, 0x807ffffe, 0b10, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 21 */  std::make_tuple(0x38fffe01, 0x00800000, 0x80800001, 0b10, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 22 */  std::make_tuple(0x35ffff82, 0x8756a1f9, 0x00000000, 0b10, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  // Romeo_FMAF_minMag_1M_0.testcase
  /* 23 */  std::make_tuple(0x8683f7ff, 0xc07f3fff, 0x00000000, 0b01, 0b0, 0b1, 0b0, 0b0, 0x07839504, 0b000001),
  /* 24 */  std::make_tuple(0x00000000, 0x00000000, 0x3c072c85, 0b01, 0b0, 0b1, 0b0, 0b0, 0x3c072c85, 0b000000),
  /* 25 */  std::make_tuple(0x9ede38f7, 0x3e7f7f7f, 0xdf7effff, 0b01, 0b0, 0b1, 0b0, 0b0, 0xdf7effff, 0b000001),
  /* 26 */  std::make_tuple(0x4f951295, 0x00000000, 0x00000000, 0b01, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 27 */  std::make_tuple(0x00000000, 0x41e00002, 0xc2800040, 0b01, 0b0, 0b1, 0b0, 0b0, 0xc2800040, 0b000000),
  /* 28 */  std::make_tuple(0x4fffdff7, 0x00000000, 0xbfffffcf, 0b01, 0b0, 0b1, 0b0, 0b0, 0xbfffffcf, 0b000000),
  /* 29 */  std::make_tuple(0x00000000, 0x015e834a, 0x00000000, 0b01, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 30 */  std::make_tuple(0xc700ffbf, 0xce7c0007, 0xbe5fefff, 0b01, 0b0, 0b1, 0b0, 0b0, 0x55fdf787, 0b000001),
  /* 31 */  std::make_tuple(0x417febff, 0xce7d4590, 0x00000001, 0b01, 0b0, 0b1, 0b0, 0b0, 0xd07d31c5, 0b000001),
  /* 32 */  std::make_tuple(0x41ffffeb, 0x137f7ffb, 0xa68002fe, 0b01, 0b0, 0b1, 0b0, 0b0, 0xa68002fd, 0b000001),
  /* 33 */  std::make_tuple(0x5fffeeff, 0xdff80000, 0xb2fffdbe, 0b01, 0b0, 0b1, 0b0, 0b0, 0xff7fffff, 0b000101),
  /* 34 */  std::make_tuple(0x7f22c563, 0x437bfffe, 0xfc81007f, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7f7fffff, 0b000101),
  /* 35 */  std::make_tuple(0xfeffe03e, 0xcb80037f, 0x3f800000, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7f7fffff, 0b000101),
  /* 36 */  std::make_tuple(0x3fffffc2, 0xff0000f7, 0x3ffffffe, 0b01, 0b0, 0b1, 0b0, 0b0, 0xff7fffff, 0b000101),
  /* 37 */  std::make_tuple(0xdf01fffd, 0xf7dffffe, 0x5f7e7fff, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7f7fffff, 0b000101),
  /* 38 */  std::make_tuple(0x15ffbff7, 0xa17ff7fc, 0x00000001, 0b01, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 39 */  std::make_tuple(0x3e800040, 0x00800000, 0x808a0380, 0b01, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 40 */  std::make_tuple(0x00800103, 0x80fff80f, 0x807ffffe, 0b01, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 41 */  std::make_tuple(0x38fffe01, 0x00800000, 0x80800001, 0b01, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 42 */  std::make_tuple(0x35ffff82, 0x8756a1f9, 0x00000000, 0b01, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  // Romeo_FMAF_min_1M_0.testcase
  /* 43 */  std::make_tuple(0x8683f7ff, 0xc07f3fff, 0x00000000, 0b11, 0b0, 0b1, 0b0, 0b0, 0x07839504, 0b000001),
  /* 44 */  std::make_tuple(0x00000000, 0x00000000, 0x3c072c85, 0b11, 0b0, 0b1, 0b0, 0b0, 0x3c072c85, 0b000000),
  /* 45 */  std::make_tuple(0x9ede38f7, 0x3e7f7f7f, 0xdf7effff, 0b11, 0b0, 0b1, 0b0, 0b0, 0xdf7f0000, 0b000001),
  /* 46 */  std::make_tuple(0x4f951295, 0x00000000, 0x00000000, 0b11, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 47 */  std::make_tuple(0x00000000, 0x41e00002, 0xc2800040, 0b11, 0b0, 0b1, 0b0, 0b0, 0xc2800040, 0b000000),
  /* 48 */  std::make_tuple(0x4fffdff7, 0x00000000, 0xbfffffcf, 0b11, 0b0, 0b1, 0b0, 0b0, 0xbfffffcf, 0b000000),
  /* 49 */  std::make_tuple(0x00000000, 0x015e834a, 0x00000000, 0b11, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 50 */  std::make_tuple(0xc700ffbf, 0xce7c0007, 0xbe5fefff, 0b11, 0b0, 0b1, 0b0, 0b0, 0x55fdf787, 0b000001),
  /* 51 */  std::make_tuple(0x417febff, 0xce7d4590, 0x00000001, 0b11, 0b0, 0b1, 0b0, 0b0, 0xd07d31c6, 0b000001),
  /* 52 */  std::make_tuple(0x41ffffeb, 0x137f7ffb, 0xa68002fe, 0b11, 0b0, 0b1, 0b0, 0b0, 0xa68002fe, 0b000001),
  /* 53 */  std::make_tuple(0x5fffeeff, 0xdff80000, 0xb2fffdbe, 0b11, 0b0, 0b1, 0b0, 0b0, 0xff800000, 0b000101),
  /* 54 */  std::make_tuple(0x7f22c563, 0x437bfffe, 0xfc81007f, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7f7fffff, 0b000101),
  /* 55 */  std::make_tuple(0xfeffe03e, 0xcb80037f, 0x3f800000, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7f7fffff, 0b000101),
  /* 56 */  std::make_tuple(0x3fffffc2, 0xff0000f7, 0x3ffffffe, 0b11, 0b0, 0b1, 0b0, 0b0, 0xff800000, 0b000101),
  /* 57 */  std::make_tuple(0xdf01fffd, 0xf7dffffe, 0x5f7e7fff, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7f7fffff, 0b000101),
  /* 58 */  std::make_tuple(0x15ffbff7, 0xa17ff7fc, 0x00000001, 0b11, 0b0, 0b1, 0b0, 0b0, 0x80800000, 0b000011),
  /* 59 */  std::make_tuple(0x3e800040, 0x00800000, 0x808a0380, 0b11, 0b0, 0b1, 0b0, 0b0, 0x80800000, 0b000011),
  /* 60 */  std::make_tuple(0x00800103, 0x80fff80f, 0x807ffffe, 0b11, 0b0, 0b1, 0b0, 0b0, 0x80800000, 0b000011),
  /* 61 */  std::make_tuple(0x38fffe01, 0x00800000, 0x80800001, 0b11, 0b0, 0b1, 0b0, 0b0, 0x80800000, 0b000011),
  /* 62 */  std::make_tuple(0x35ffff82, 0x8756a1f9, 0x00000000, 0b11, 0b0, 0b1, 0b0, 0b0, 0x80800000, 0b000011),
  // Romeo_FMAF_near_even_1M_0.testcase
  /* 63 */  std::make_tuple(0x8683f7ff, 0xc07f3fff, 0x00000000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x07839504, 0b000001),
  /* 64 */  std::make_tuple(0x00000000, 0x00000000, 0x3c072c85, 0b00, 0b0, 0b1, 0b0, 0b0, 0x3c072c85, 0b000000),
  /* 65 */  std::make_tuple(0x9ede38f7, 0x3e7f7f7f, 0xdf7effff, 0b00, 0b0, 0b1, 0b0, 0b0, 0xdf7effff, 0b000001),
  /* 66 */  std::make_tuple(0x4f951295, 0x00000000, 0x00000000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 67 */  std::make_tuple(0x00000000, 0x41e00002, 0xc2800040, 0b00, 0b0, 0b1, 0b0, 0b0, 0xc2800040, 0b000000),
  /* 68 */  std::make_tuple(0x4fffdff7, 0x00000000, 0xbfffffcf, 0b00, 0b0, 0b1, 0b0, 0b0, 0xbfffffcf, 0b000000),
  /* 69 */  std::make_tuple(0x00000000, 0x015e834a, 0x00000000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000000),
  /* 70 */  std::make_tuple(0xc700ffbf, 0xce7c0007, 0xbe5fefff, 0b00, 0b0, 0b1, 0b0, 0b0, 0x55fdf787, 0b000001),
  /* 71 */  std::make_tuple(0x417febff, 0xce7d4590, 0x00000001, 0b00, 0b0, 0b1, 0b0, 0b0, 0xd07d31c6, 0b000001),
  /* 72 */  std::make_tuple(0x41ffffeb, 0x137f7ffb, 0xa68002fe, 0b00, 0b0, 0b1, 0b0, 0b0, 0xa68002fe, 0b000001),
  /* 73 */  std::make_tuple(0x5fffeeff, 0xdff80000, 0xb2fffdbe, 0b00, 0b0, 0b1, 0b0, 0b0, 0xff800000, 0b000101),
  /* 74 */  std::make_tuple(0x7f22c563, 0x437bfffe, 0xfc81007f, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7f800000, 0b000101),
  /* 75 */  std::make_tuple(0xfeffe03e, 0xcb80037f, 0x3f800000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7f800000, 0b000101),
  /* 76 */  std::make_tuple(0x3fffffc2, 0xff0000f7, 0x3ffffffe, 0b00, 0b0, 0b1, 0b0, 0b0, 0xff800000, 0b000101),
  /* 77 */  std::make_tuple(0xdf01fffd, 0xf7dffffe, 0x5f7e7fff, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7f800000, 0b000101),
  /* 78 */  std::make_tuple(0x15ffbff7, 0xa17ff7fc, 0x00000001, 0b00, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 79 */  std::make_tuple(0x3e800040, 0x00800000, 0x808a0380, 0b00, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 80 */  std::make_tuple(0x00800103, 0x80fff80f, 0x807ffffe, 0b00, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 81 */  std::make_tuple(0x38fffe01, 0x00800000, 0x80800001, 0b00, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 82 */  std::make_tuple(0x35ffff82, 0x8756a1f9, 0x00000000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  // Check Input sNaN
  /* 83 */  std::make_tuple(0xcbcf3ea9, 0x3efffffd, 0xff8000fd, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 84 */  std::make_tuple(0x00000000, 0x2c4716ea, 0xff97847c, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 85 */  std::make_tuple(0xff8000ee, 0xbf94d20a, 0x4f80000b, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 86 */  std::make_tuple(0xff801003, 0x5312a9cd, 0x4b800001, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 87 */  std::make_tuple(0xff800000, 0x00000000, 0x42a1f4bc, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 88 */  std::make_tuple(0xcbcf3ea9, 0x3efffffd, 0xff8000fd, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 89 */  std::make_tuple(0x00000000, 0x2c4716ea, 0xff97847c, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 90 */  std::make_tuple(0xff8000ee, 0xbf94d20a, 0x4f80000b, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 91 */  std::make_tuple(0xff801003, 0x5312a9cd, 0x4b800001, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 92 */  std::make_tuple(0xff800000, 0x00000000, 0x42a1f4bc, 0b01, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 93 */  std::make_tuple(0xcbcf3ea9, 0x3efffffd, 0xff8000fd, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 94 */  std::make_tuple(0x00000000, 0x2c4716ea, 0xff97847c, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 95 */  std::make_tuple(0xff8000ee, 0xbf94d20a, 0x4f80000b, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 96 */  std::make_tuple(0xff801003, 0x5312a9cd, 0x4b800001, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 97 */  std::make_tuple(0xff800000, 0x00000000, 0x42a1f4bc, 0b11, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 98 */  std::make_tuple(0xcbcf3ea9, 0x3efffffd, 0xff8000fd, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 99 */  std::make_tuple(0x00000000, 0x2c4716ea, 0xff97847c, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 100 */ std::make_tuple(0xff8000ee, 0xbf94d20a, 0x4f80000b, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 101 */ std::make_tuple(0xff801003, 0x5312a9cd, 0x4b800001, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 102 */ std::make_tuple(0xff800000, 0x00000000, 0x42a1f4bc, 0b00, 0b0, 0b1, 0b0, 0b0, 0x7fffffff, 0b010000),
  /* 103 */ std::make_tuple(0x7fff0007, 0x00000000, 0x00000001, 0b10, 0b0, 0b1, 0b0, 0b0, 0x7fff0007, 0b000000),
  // Check Output flush
  /* 104 */ std::make_tuple(0x00000000, 0x340000ef, 0x00000001, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000001),
  /* 105 */ std::make_tuple(0x00000000, 0x00000000, 0x00000001, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000001),
  /* 106 */ std::make_tuple(0x4e00ffee, 0x00000000, 0x007fffff, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000001),
  /* 107 */ std::make_tuple(0x00000000, 0xc11df309, 0x007fffff, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000001),
  /* 108 */ std::make_tuple(0x00000000, 0x00000000, 0x007fffff, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000001),
  /* 109 */ std::make_tuple(0xbf7fff7b, 0x00000000, 0x007ffffe, 0b10, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000001),
  // check with xe pattern
  /* 110 */ std::make_tuple(0x35ffff82, 0x8756a1f9, 0x00000000, 0b10, 0b0, 0b1, 0b0, 0b1, 0x80000000, 0b000010),
  /* 111 */ std::make_tuple(0x5fffeeff, 0xdff80000, 0xb2fffdbe, 0b01, 0b0, 0b1, 0b1, 0b0, 0xff7fffff, 0b000100),
  // Romeo_FMAF_near_even_1M_0.testcase
  /* 112 */ std::make_tuple(0xb087f7fe, 0x00800000, 0x00800000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x00000000, 0b000011),
  /* 113 */ std::make_tuple(0x13db54b6, 0x00fffffe, 0x80800000, 0b00, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  // check with xe pattern (refs #3437)
  /* 114 */ std::make_tuple(0x35ffff82, 0x8756a1f9, 0x00000000, 0b10, 0b0, 0b1, 0b0, 0b0, 0x80000000, 0b000011),
  /* 115 */ std::make_tuple(0x5fffeeff, 0xdff80000, 0xb2fffdbe, 0b01, 0b0, 0b1, 0b0, 0b0, 0xff7fffff, 0b000101),
};
INSTANTIATE_TEST_CASE_P(TestWithParameters1, FMA32Test,
                        ::testing::ValuesIn(new_float32_madd_ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
