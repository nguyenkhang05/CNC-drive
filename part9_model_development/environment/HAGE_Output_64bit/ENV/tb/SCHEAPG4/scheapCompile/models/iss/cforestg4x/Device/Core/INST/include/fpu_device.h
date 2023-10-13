/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#pragma once

#include "./forest_message.h"
#include "./fpu_base.h"
#include "./sreg.h"

#if !defined(_UNIT_TEST)
#include "SoftFloatAdapter.h"
#else
#include "gtest/gtest_prod.h"
#include "mock_SoftFloatAdapter.h"
#endif

/*! brief Floating-point unit operation for SoftFloat library.
 */
class FpuOpeDevice : public FpuOpeBase {
#if !defined(_UNIT_TEST)
private:
  SoftFloat *m_softfloat;
#else
public:
  MockSoftFloat *m_softfloat;
#endif

public:
#if !defined(_UNIT_TEST)
  FpuOpeDevice(TraceCtrl *trace, SRegFPSR *fpsr) : FpuOpeBase(trace, fpsr) {
    m_softfloat = new SoftFloat();
  }
  FpuOpeDevice(TraceCtrl *trace, SRegFXSR *fxsr) : FpuOpeBase(trace, fxsr) {
    m_softfloat = new SoftFloat();
  }
#else
  FpuOpeDevice(TraceCtrl *trace, SRegFPSR *fpsr) : FpuOpeBase(trace, fpsr) {
    m_softfloat = new MockSoftFloat();
  }
  FpuOpeDevice(TraceCtrl *trace, SRegFXSR *fxsr) : FpuOpeBase(trace, fxsr) {
    m_softfloat = new MockSoftFloat();
  }
#endif
  ~FpuOpeDevice() { delete m_softfloat; }

  FpuRegDouble Add(const FpuRegDouble &a, const FpuRegDouble &b, uint32_t *exp_code) const;
  FpuRegSingle Add(const FpuRegSingle &a, const FpuRegSingle &b, uint32_t *exp_code) const;
  FpuRegSingle Sub(const FpuRegSingle &a, const FpuRegSingle &b, uint32_t *exp_code) const;
  FpuRegDouble Sub(const FpuRegDouble &a, const FpuRegDouble &b, uint32_t *exp_code) const;
  FpuRegDouble Mul(const FpuRegDouble &a, const FpuRegDouble &b, uint32_t *exp_code) const;
  FpuRegSingle Mul(const FpuRegSingle &a, const FpuRegSingle &b, uint32_t *exp_code) const;
  FpuRegSingle Div (const FpuRegSingle &a, const FpuRegSingle &b, uint32_t* exp_code) const;
  FpuRegDouble Div (const FpuRegDouble &a, const FpuRegDouble &b, uint32_t* exp_code) const;
  FpuRegDouble Abs(const FpuRegDouble &a, uint32_t *exp_code) const;
  FpuRegSingle Abs(const FpuRegSingle &a, uint32_t *exp_code) const;
  FpuRegSingle Neg(const FpuRegSingle &a, uint32_t *exp_code) const;
  FpuRegDouble Neg(const FpuRegDouble &a, uint32_t *exp_code) const;

  FpuRegSingle Recip (const FpuRegSingle &a, uint32_t* exp_code) const;
  FpuRegDouble Recip (const FpuRegDouble &a, uint32_t* exp_code) const;
  FpuRegSingle Sqrt(const FpuRegSingle &a, uint32_t *exp_code) const;
  FpuRegDouble Sqrt(const FpuRegDouble &a, uint32_t *exp_code) const;
  FpuRegSingle Rsqrt(const FpuRegSingle &a, uint32_t *exp_code) const;
  FpuRegDouble Rsqrt(const FpuRegDouble &a, uint32_t *exp_code) const;
  FpuRegSingle Min  (const FpuRegSingle &a, const FpuRegSingle &b, uint32_t* exp_code) const;
  FpuRegDouble Min  (const FpuRegDouble &a, const FpuRegDouble &b, uint32_t* exp_code) const;
  FpuRegSingle Max(const FpuRegSingle &a, const FpuRegSingle &b, uint32_t* exp_code) const;
  FpuRegDouble Max(const FpuRegDouble &a, const FpuRegDouble &b, uint32_t* exp_code) const;

  FpuRegDouble Mneg(const FpuRegDouble &a, uint32_t *exp_code) const;
  FpuRegSingle Mneg(const FpuRegSingle &a, uint32_t *exp_code) const;
  FpuRegSingle Madd(const FpuRegSingle &a, const FpuRegSingle &b, const FpuRegSingle &c,
                    uint32_t *exp_code) const;
  FpuRegSingle Msub(const FpuRegSingle &a, const FpuRegSingle &b, const FpuRegSingle &c,
                    uint32_t *exp_code) const;

  bool Cmp(const FpuRegSingle &a, const FpuRegSingle &b, int32_t fcond, uint32_t* exp_code) const;
  bool Cmp(const FpuRegDouble &a, const FpuRegDouble &b, int32_t fcond, uint32_t* exp_code) const;

  // Convert from Float to Float
  float Cvt_DS(const FpuRegDouble &a, uint32_t *exp_code) const;
  double Cvt_SD(const FpuRegSingle &a, uint32_t *exp_code) const;
  uint16_t Cvt_SH(const FpuRegSingle &a, uint32_t *exp_code);
  uint32_t Cvt_HS(const FpuRegHalf &a, uint32_t *exp_code);
  // Convert from F32 to I32
  int32_t  Cvt_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int32_t  Round_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int32_t  Trnc_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int32_t  Floor_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int32_t  Ceil_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int32_t  F32_to_I32  (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from F32 to UI 32.
  uint32_t  Cvt_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint32_t  Round_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint32_t  Ceil_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint32_t  Floor_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint32_t  Trnc_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint32_t  F32_to_UI32  (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from F32 to I64
  int64_t  Cvt_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int64_t  Round_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int64_t  Ceil_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int64_t  Floor_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int64_t  Trnc_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  int64_t  F32_to_I64  (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from F32 to UI64
  uint64_t  Cvt_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint64_t  Round_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint64_t  Ceil_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint64_t  Floor_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint64_t  Trnc_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const;
  uint64_t  F32_to_UI64  (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from F64 to I32
  int32_t  Cvt_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int32_t  Round_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int32_t  Trnc_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int32_t  Ceil_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int32_t  Floor_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int32_t  F64_to_I32  (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from F64 to UI32
  uint32_t  Cvt_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint32_t  Round_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint32_t  Trnc_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint32_t  Ceil_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint32_t  Floor_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint32_t  F64_to_UI32  (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from F64 to I64
  int64_t  Cvt_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int64_t  Round_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int64_t  Trnc_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int64_t  Ceil_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int64_t  Floor_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  int64_t  F64_to_I64  (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from F64 to UI64
  uint64_t  Cvt_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint64_t  Round_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint64_t  Trnc_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint64_t  Ceil_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint64_t  Floor_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const;
  uint64_t  F64_to_UI64  (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const;
  // Convert from Integer to Float
  FpuRegSingle Cvt_I32_F32  (const int32_t &a, uint32_t *exp_code);
  FpuRegSingle Cvt_UI32_F32 (const uint32_t &a, uint32_t *exp_code);
  FpuRegSingle Cvt_I64_F32  (const int64_t &a, uint32_t *exp_code);
  FpuRegSingle Cvt_UI64_F32 (const uint64_t &a, uint32_t *exp_code);
  FpuRegDouble Cvt_I32_F64  (const int32_t &a, uint32_t *exp_code);
  FpuRegDouble Cvt_UI32_F64 (const uint32_t &a, uint32_t *exp_code);
  FpuRegDouble Cvt_I64_F64  (const int64_t &a, uint32_t *exp_code);
  FpuRegDouble Cvt_UI64_F64 (const uint64_t &a, uint32_t *exp_code);

private:
  uint32_t float32_madd (const FpuRegSingle &a, const FpuRegSingle &b, const FpuRegSingle &c,
                           uint32_t rm, uint32_t xe, uint32_t fn, uint32_t fs) const;
  uint32_t float32_msub (const FpuRegSingle &a, const FpuRegSingle &b, const FpuRegSingle &c,
                           uint32_t rm, uint32_t xe, uint32_t fn, uint32_t fs) const;
};

class FpSimdOpeDevice : public FpuOpeDevice {
public:
  FpSimdOpeDevice(TraceCtrl *trace, SRegFXSR *fxsr) : FpuOpeDevice(trace, fxsr) {}
};
