/**
 * @file test_check_output_subnormal.cpp
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UNIT TEST for CheckOutputSubnormal
 * @par Copyright: 2017 Renesas Electronics Corp.
 */
#include <cstdint>
#include <cstddef>
#include "gmock/gmock.h"
#include "mock_SoftFloatAdapter.h"
#include "SoftFloatAdapter.h"
extern "C" {
#include "internals.h"
#include "specialize.h"
}
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

using ::testing::AnyNumber;
using ::testing::AtLeast;

class CheckOutputSubnormal32 : public testing::Test,
                  public testing::WithParamInterface<
                      std::tuple<uint32_t, uint32_t,
                                 uint32_t, uint32_t>> {
protected:
  CheckOutputSubnormal32() {}
  virtual ~CheckOutputSubnormal32() {}
  virtual void SetUp() { m_sf = new MockSoftFloat(); }
  virtual void TearDown() { delete m_sf; }

public:
  MockSoftFloat* m_sf;

private:
};

/*!
 * @brief Check 32bit
 * SoftFloat-3b
 */
TEST_P(CheckOutputSubnormal32, 32bit) {
#if 0
// #if defined(_UNIT_TEST)
  auto PrintF32DebugInfo = [&](float32_t x) {
    bool signX = signF32UI(x.v);
    int_fast16_t expX = expF32UI(x.v);
    uint_fast32_t sigX = fracF32UI(x.v);
    bool _isNaN = isNaNF32UI(x.v);
    bool _isSNaN = ::f32_isSignalingNaN(x);
    bool _isSubnormal = ((expX == 0) && (sigX != 0));

    union fandui {
      float f;
      uint32_t u;
    } fux;
    fux.u = x.v;
    printf("=%f(0x%.8x)"
           " signX=0x%.2x,"
           " expX=0x%.2x,"
           " sigX=0x%" PRIXFAST32 ": %s%s\n",
           fux.f, fux.u, signX, static_cast<unsigned int>(expX), sigX,
           _isSubnormal ? "is subnormal" : "not subnormal",
           (!_isNaN) ? "" : (_isSNaN) ? ", sNaN" : ", qNaN");
    if (_isSubnormal) {
      if (m_sf->GetFlushEnable()) {
        printf("Found subnormal: Flushed: x(%.8x)->x(%.8x)\n", x.v,
               (signX) ? SoftFloat::F32_N_ZERO : SoftFloat::F32_P_ZERO);
      }
    }
  };
#endif

  uint32_t reg1          = std::get<0>(GetParam());
  uint32_t fs            = std::get<1>(GetParam()); // FPSR.FS
  uint32_t expect        = std::get<2>(GetParam());
  uint32_t expect_evzoui = std::get<3>(GetParam());

  float32_t v1;
  v1.v = reg1;

#if 0
// #if defined(_UNIT_TEST)
  printf("INPUT");
  PrintF32DebugInfo(v1);
#endif

  // Expect operations
  // Check the conditions necessary to call the SoftFloat-3b library
  // clear 3b
  EXPECT_CALL((*m_sf), SetExceptionFlags(_)).Times(AnyNumber());
  // set FS bit from fpsr to softfloat
  EXPECT_CALL((*m_sf), SetFlushEnable(_)).Times(1);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetExceptionFlags()).Times(1);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetFlushEnable()).Times(AtLeast(0));

  m_sf->SetExceptionFlags(0);
  m_sf->SetFlushEnable(fs);
  float32_t ret = m_sf->CheckOutputSubnormal32(v1);

#if 0
// #if defined(_UNIT_TEST)
  printf("OUTPUT");
  PrintF32DebugInfo(ret);
#endif

  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, ret.v);
}

std::tuple<uint32_t, uint32_t, // value, fs(1bit)
           uint32_t, uint32_t  // expect, exp_evzoui(6bit)
           > const cos32_ParametersTable[] = {
  /* 0 */   std::make_tuple(0b0'00000000'00000000000000000000000, 0b0,
                            0b0'00000000'00000000000000000000000, 0b000000),
  /* 1 */   std::make_tuple(0b0'00000001'00000000000000000000000, 0b0,
                            0b0'00000001'00000000000000000000000, 0b000000),
  /* 2 */   std::make_tuple(0b0'00000000'10000000000000000000000, 0b1,
                            0b0'00000000'00000000000000000000000, 0b000010),
  /* 3 */   std::make_tuple(0b0'00000000'10000000000000000000000, 0b0,
                            0b0'00000000'10000000000000000000000, 0b100000),
  /* 4 */   std::make_tuple(0b1'00000000'10000000000000000000000, 0b1,
                            0b1'00000000'00000000000000000000000, 0b000010),
  /* 5 */   std::make_tuple(0b1'00000000'10000000000000000000000, 0b0,
                            0b1'00000000'10000000000000000000000, 0b100000),
  /* 6 */   std::make_tuple(0b1'00000000'00000000000000000000001, 0b1,
                            0b1'00000000'00000000000000000000000, 0b000010),
};
INSTANTIATE_TEST_CASE_P(TestWithParameters1, CheckOutputSubnormal32,
                        ::testing::ValuesIn(cos32_ParametersTable));

class CheckOutputSubnormal64 : public testing::Test,
                  public testing::WithParamInterface<
                      std::tuple<uint64_t, uint32_t,
                                 uint64_t, uint32_t>> {
protected:
  CheckOutputSubnormal64() {}
  virtual ~CheckOutputSubnormal64() {}
  virtual void SetUp() { m_sf = new MockSoftFloat(); }
  virtual void TearDown() { delete m_sf; }

public:
  MockSoftFloat* m_sf;

private:
};

/*!
 * @brief Check 64bit
 * SoftFloat-3b
 */
TEST_P(CheckOutputSubnormal64, 64bit) {
#if 0
// #if defined(_UNIT_TEST)
  auto PrintF64DebugInfo = [&](float64_t x) {
    bool signX = signF64UI(x.v);
    int_fast16_t expX = expF64UI(x.v);
    uint_fast64_t sigX = fracF64UI(x.v);
    bool _isNaN = isNaNF64UI(x.v);
    bool _isSNaN = ::f64_isSignalingNaN(x);
    bool _isSubnormal = ((expX == 0) && (sigX != 0));

    union fandui {
      double f;
      uint64_t u;
    } fux;
    fux.u = x.v;
    printf("=%0.64f(0x%.16lx)"
           " signX=0x%.2x,"
           " expX=0x%.3x,"
           " sigX=0x%" PRIXFAST64 ": %s%s\n",
           fux.f, fux.u, signX, static_cast<unsigned int>(expX), sigX,
           _isSubnormal ? "is subnormal" : "not subnormal",
           (!_isNaN) ? "" : (_isSNaN) ? ", sNaN" : ", qNaN");
    if (_isSubnormal) {
      if (m_sf->GetFlushEnable()) {
        printf("Found subnormal: Flushed: x(%" PRIXFAST64 ")->x(%" PRIXFAST64 ")\n", x.v,
               (signX) ? SoftFloat::F64_N_ZERO : SoftFloat::F64_P_ZERO);
      }
    }
  };
#endif

  uint64_t reg1          = std::get<0>(GetParam());
  uint32_t fs            = std::get<1>(GetParam()); // FPSR.FS
  uint64_t expect        = std::get<2>(GetParam());
  uint32_t expect_evzoui = std::get<3>(GetParam());

  float64_t v1;
  v1.v = reg1;

#if 0
// #if defined(_UNIT_TEST)
  printf("INPUT");
  PrintF64DebugInfo(v1);
#endif

  // Expect operations
  // Check the conditions necessary to call the SoftFloat-3b library
  // clear 3b
  EXPECT_CALL((*m_sf), SetExceptionFlags(_)).Times(AnyNumber());
  // set FS bit from fpsr to softfloat
  EXPECT_CALL((*m_sf), SetFlushEnable(_)).Times(1);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetExceptionFlags()).Times(1);
  // get exception flag from softfloat
  EXPECT_CALL((*m_sf), GetFlushEnable()).Times(AtLeast(0));

  m_sf->SetExceptionFlags(0);
  m_sf->SetFlushEnable(fs);
  float64_t ret = m_sf->CheckOutputSubnormal64(v1);

#if 0
// #if defined(_UNIT_TEST)
  printf("OUTPUT");
  PrintF64DebugInfo(ret);
#endif

  uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

  EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
  EXPECT_EQ(expect, ret.v);
}

std::tuple<uint64_t, uint32_t, // value, fs(1bit)
           uint64_t, uint32_t  // expect, exp_evzoui(6bit)
           > const cos64_ParametersTable[] = {
  /* 0 */   std::make_tuple(0b0'00000000000'0000000000000000000000000000000000000000000000000000, 0b0,
                            0b0'00000000000'0000000000000000000000000000000000000000000000000000, 0b000000),
  /* 1 */   std::make_tuple(0b0'00000000001'0000000000000000000000000000000000000000000000000000, 0b0,
                            0b0'00000000001'0000000000000000000000000000000000000000000000000000, 0b000000),
  /* 2 */   std::make_tuple(0b0'00000000000'1000000000000000000000000000000000000000000000000000, 0b1,
                            0b0'00000000000'0000000000000000000000000000000000000000000000000000, 0b000010),
  /* 3 */   std::make_tuple(0b0'00000000000'1000000000000000000000000000000000000000000000000000, 0b0,
                            0b0'00000000000'1000000000000000000000000000000000000000000000000000, 0b100000),
  /* 4 */   std::make_tuple(0b1'00000000000'1000000000000000000000000000000000000000000000000000, 0b1,
                            0b1'00000000000'0000000000000000000000000000000000000000000000000000, 0b000010),
  /* 5 */   std::make_tuple(0b1'00000000000'1000000000000000000000000000000000000000000000000000, 0b0,
                            0b1'00000000000'1000000000000000000000000000000000000000000000000000, 0b100000),
  /* 6 */   std::make_tuple(0b1'00000000000'0000000000000000000000000000000000000000000000000001, 0b1,
                            0b1'00000000000'0000000000000000000000000000000000000000000000000000, 0b000010),
};
INSTANTIATE_TEST_CASE_P(TestWithParameters2, CheckOutputSubnormal64,
                        ::testing::ValuesIn(cos64_ParametersTable));

class CheckOutputSubnormal16 : public testing::Test,
                  public testing::WithParamInterface<
                      std::tuple<uint16_t, uint32_t,
                                 uint16_t, uint32_t>> {
protected:
  CheckOutputSubnormal16() {}
  virtual ~CheckOutputSubnormal16() {}
  virtual void SetUp() { m_sf = new MockSoftFloat(); }
  virtual void TearDown() { delete m_sf; }

public:
  MockSoftFloat* m_sf;

private:
};

// /*!
//  * @brief Check 16bit
//  * SoftFloat-3b
//  */
// TEST_P(CheckOutputSubnormal16, 16bit) {
//   auto PrintF16DebugInfo = [&](float16_t x) {
//     bool signX = signF16UI(x.v);
//     int_fast16_t expX = expF16UI(x.v);
//     uint_fast16_t sigX = fracF16UI(x.v);
//     bool _isNaN = isNaNF16UI(x.v);
//     bool _isSNaN = ::f16_isSignalingNaN(x);
//     bool _isSubnormal = ((expX == 0) && (sigX != 0));

//     union fandui {
//       float f;
//       uint16_t u;
//     } fux;
//     fux.u = x.v;
//     printf("=%0.8f(0x%.4x)"
//            " signX=0x%.2x,"
//            " expX=0x%.2x,"
//            " sigX=0x%x: %s%s\n",
//            fux.f, fux.u, signX, static_cast<unsigned int>(expX), static_cast<unsigned int>(sigX),
//            _isSubnormal ? "is subnormal" : "not subnormal",
//            (!_isNaN) ? "" : (_isSNaN) ? ", sNaN" : ", qNaN");
//     if (_isSubnormal) {
//       if (m_sf->GetFlushEnable()) {
//         printf("Found subnormal: Flushed: x(%.4x)->x(%.4x)\n", x.v,
//                (signX) ? SoftFloat::F16_N_ZERO : SoftFloat::F16_P_ZERO);
//       }
//     }
//   };

//   uint16_t reg1          = std::get<0>(GetParam());
//   uint32_t fs            = std::get<1>(GetParam()); // FPSR.FS
//   uint16_t expect        = std::get<2>(GetParam());
//   uint32_t expect_evzoui = std::get<3>(GetParam());

//   float16_t v1;
//   v1.v = reg1;

//   printf("INPUT");
//   PrintF16DebugInfo(v1);

//   // Expect operations
//   // Check the conditions necessary to call the SoftFloat-3b library
//   // clear 3b
//   EXPECT_CALL((*m_sf), SetExceptionFlags(_)).Times(AnyNumber());
//   // set FS bit from fpsr to softfloat
//   EXPECT_CALL((*m_sf), SetFlushEnable(_)).Times(1);
//   // get exception flag from softfloat
//   EXPECT_CALL((*m_sf), GetExceptionFlags()).Times(1);
//   // get exception flag from softfloat
//   EXPECT_CALL((*m_sf), GetFlushEnable()).Times(AtLeast(0));

//   m_sf->SetExceptionFlags(0);
//   m_sf->SetFlushEnable(fs);
//   float16_t ret = m_sf->CheckOutputSubnormal16(v1);

//   printf("OUTPUT");
//   PrintF16DebugInfo(ret);

//   uint32_t fpu_exception_flag = m_sf->GetExceptionFlags();

//   EXPECT_EQ(expect_evzoui, (fpu_exception_flag & 0x0000003f));
//   EXPECT_EQ(expect, ret.v);
// }

// std::tuple<uint16_t, uint32_t, // value, fs(1bit)
//            uint16_t, uint32_t  // expect, exp_evzoui(6bit)
//            > const cos16_ParametersTable[] = {
//   /* 0 */   std::make_tuple(0b0'00000'0000000000, 0b0,
//                             0b0'00000'0000000000, 0b000000),
//   /* 1 */   std::make_tuple(0b0'00001'0000000000, 0b0,
//                             0b0'00001'0000000000, 0b000000),
//   /* 2 */   std::make_tuple(0b0'00000'1000000000, 0b1,
//                             0b0'00000'0000000000, 0b000010),
//   /* 3 */   std::make_tuple(0b0'00000'1000000000, 0b0,
//                             0b0'00000'1000000000, 0b100000),
//   /* 4 */   std::make_tuple(0b1'00000'1000000000, 0b1,
//                             0b1'00000'0000000000, 0b000010),
//   /* 5 */   std::make_tuple(0b1'00000'1000000000, 0b0,
//                             0b1'00000'1000000000, 0b100000),
//   /* 6 */   std::make_tuple(0b1'00000'0000000001, 0b1,
//                             0b1'00000'0000000000, 0b000010),
// };
// INSTANTIATE_TEST_CASE_P(TestWithParameters3, CheckOutputSubnormal16,
//                         ::testing::ValuesIn(cos16_ParametersTable));

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
