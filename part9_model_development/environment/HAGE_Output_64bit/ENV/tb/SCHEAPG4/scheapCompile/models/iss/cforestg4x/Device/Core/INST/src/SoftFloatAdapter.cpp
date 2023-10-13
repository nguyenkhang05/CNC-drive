/**
 * @file SoftFloatAdapter.cpp
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UCB SoflotFloat Adapter class
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <assert.h>
#include "SoftFloatAdapter.h"
#include <vector>
extern "C" {
#include "internals.h"
#include "specialize.h"
}
#if defined(_UNIT_TEST)
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#endif

void SoftFloat::SetRoundingMode(uint32_t RM) {
  switch (RM & 0b11) {
  case RM_softfloat_round_near_even:
    softfloat_roundingMode = softfloat_round_near_even;
    break;
  case RM_softfloat_round_max:
    softfloat_roundingMode = softfloat_round_max;
    break;
  case RM_softfloat_round_minMag:
    softfloat_roundingMode = softfloat_round_minMag;
    break;
  case RM_softfloat_round_min:
    softfloat_roundingMode = softfloat_round_min;
    break;
  default:
    assert(!"unreachable case");
  };
};

uint32_t SoftFloat::GetRoundingMode() {
  uint32_t rm;

  switch (softfloat_roundingMode & 0b11) {
  case softfloat_round_near_even:
    rm = RM_softfloat_round_near_even;
    break;
  case softfloat_round_minMag:
    rm = RM_softfloat_round_minMag;
    break;
  case softfloat_round_min:
    rm = RM_softfloat_round_min;
    break;
  case softfloat_round_max:
    rm = RM_softfloat_round_max;
    break;
  case softfloat_round_near_maxMag:
  default:
    assert(!"unreachable case");
  };

  return rm;
}

void SoftFloat::SetFlushEnable(bool is_flush) { softfloat_fs = is_flush; }
bool SoftFloat::GetFlushEnable() { return softfloat_fs; }
void SoftFloat::SetFlushNearestEnable(bool is_flush_nearest) { softfloat_fn = is_flush_nearest;softfloat_fn_ = is_flush_nearest;}
void SoftFloat::SetInputFlushFlag(bool is_flush) { m_fpsr_if = is_flush; }
bool SoftFloat::GetInputFlushFlag() { return m_fpsr_if; }
void SoftFloat::SetOverflowExceptionEnableFlag(bool enable_overflow) { m_fpsr_xe_o = enable_overflow; }
bool SoftFloat::GetOverflowExceptionEnableFlag() { return m_fpsr_xe_o; }
void SoftFloat::SetUnderflowExceptionEnableFlag(bool enable_underflow) { m_fpsr_xe_u = enable_underflow; }
bool SoftFloat::GetUnderflowExceptionEnableFlag() { return m_fpsr_xe_u; }

void SoftFloat::SetExceptionFlags(uint32_t flag) {
  softfloat_exceptionFlags = static_cast<uint_fast8_t>(flag);
}
uint32_t SoftFloat::GetExceptionFlags() { return static_cast<uint32_t>(softfloat_exceptionFlags); }

float32_t SoftFloat::CheckOutputSubnormal32(float32_t x) {
  bool _signX = signF32UI(x.v);
  int_fast16_t _expX = expF32UI(x.v);
  uint_fast32_t _sigX = fracF32UI(x.v);
  if ((_expX == 0) && (_sigX != 0)) {
    if (softfloat_fs) {
      switch (GetRoundingMode()) {
      case RM_softfloat_round_near_even:
        if (softfloat_fn == 1){
          if (_sigX < 0x00400000) {
            x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          } else {
            x.v = (_signX) ? F32_N_MIN : F32_P_MIN;
          }
        } else {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
        }
        break;
      case RM_softfloat_round_max:
        x.v = (_signX) ? F32_N_ZERO : F32_P_MIN;
        break;
      case RM_softfloat_round_minMag:
        x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
        break;
      case RM_softfloat_round_min:
        x.v = (_signX) ? F32_N_MIN : F32_P_ZERO;
        break;
      default:
        assert(!"unreachable case");
      };
      this->SetExceptionFlags(FPE_U); // set underflow flag
    } else {
      this->SetExceptionFlags(FPE_E);
    }
  }
  return x;
};

float64_t SoftFloat::CheckOutputSubnormal64(float64_t x) {
  bool _signX = signF64UI(x.v);
  int_fast16_t _expX = expF64UI(x.v);
  uint_fast64_t _sigX = fracF64UI(x.v);
  if ((_expX == 0) && (_sigX != 0)) {
    if (softfloat_fs) {
      switch (GetRoundingMode()) {
      case RM_softfloat_round_near_even:
        if (softfloat_fn == 1){
          if (_sigX < 0x0008'0000'0000'0000UL) {
            x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          } else {
            x.v = (_signX) ? F64_N_MIN : F64_P_MIN;
          }
        } else {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
        }
        break;
      case RM_softfloat_round_max:
        x.v = (_signX) ? F64_N_ZERO : F64_P_MIN;
        break;
      case RM_softfloat_round_minMag:
        x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
        break;
      case RM_softfloat_round_min:
        x.v = (_signX) ? F64_N_MIN : F64_P_ZERO;
        break;
      default:
        assert(!"unreachable case");
      };
      this->SetExceptionFlags(FPE_U); // set underflow flag
    } else {
      this->SetExceptionFlags(FPE_E);
    }
  }
  return x;
};

 float16_t SoftFloat::CheckOutputSubnormal16(float16_t x) {

  bool _signX = signF16UI(x.v);
  int_fast16_t _expX = expF16UI(x.v);
  uint_fast32_t _sigX = fracF16UI(x.v);
  if ((_expX == 0) && (_sigX != 0)) {
    if (softfloat_fs) {
      switch (GetRoundingMode()) {
      case RM_softfloat_round_near_even:
        if (softfloat_fn == 1){
          if (_sigX < 0x0200) {
            x.v = (_signX) ? F16_N_ZERO : F16_P_ZERO;
          } else {
            x.v = (_signX) ? F16_N_MIN : F16_P_MIN;
          }
        } else {
          x.v = (_signX) ? F16_N_ZERO : F16_P_ZERO;
        }
        break;
      case RM_softfloat_round_max:
        x.v = (_signX) ? F16_N_ZERO : F16_P_MIN;
        break;
      case RM_softfloat_round_minMag:
        x.v = (_signX) ? F16_N_ZERO : F16_P_ZERO;
        break;
      case RM_softfloat_round_min:
        x.v = (_signX) ? F16_N_MIN : F16_P_ZERO;
        break;
      default:
        assert(!"unreachable case");
      };
      this->SetExceptionFlags(FPE_U); // set underflow flag
    } else {
      this->SetExceptionFlags(FPE_E);
    }
  }
  return x;
 };

/** MullAdd
 * @param[in] fr2
 * @param[in] fr1
 * @param[in] fr3
 * @return res.
 * @retval mulladd value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_mulAdd(float32_t fr2, float32_t fr1, float32_t fr3) {
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
      if (this->GetFlushEnable()) {
        printf("Found subnormal: Flushed: x(%.8x)->x(%.8x)\n", x.v,
               (signX) ? F32_N_ZERO : F32_P_ZERO);
      }
    }
  };
  printf("FR2");
  PrintF32DebugInfo(fr2);
  printf("FR1");
  PrintF32DebugInfo(fr1);
  printf("FR3");
  PrintF32DebugInfo(fr3);
#endif

  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (softfloat_fs) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr2 = CheckInputSubnormal(fr2);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr2;
    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1;
    fr3 = CheckInputSubnormal(fr3);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr3;
  }

  // check NaN
  {
    bool isSNaN = false;
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { fr2, fr1, fr3 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      fr2.v = 0x7fbfffff;
      fr1.v = 0x7fbfffff;
      fr3.v = 0x7fbfffff;
    } else if (qNaN.v != 0) {
      return qNaN;
    }
  }

  float32_t res = ::f32_mulAdd(fr2, fr1, fr3);
  res = this->CheckOutputSubnormal32(res);

#if 0
// #if defined(_UNIT_TEST)
  printf("FRR");
  PrintF32DebugInfo(res);
#endif

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  return res;
}

/** MullSub
 * @param[in] fr2
 * @param[in] fr1
 * @param[in] fr3
 * @return res.
 * @retval mullsub value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_mulSub(float32_t fr2, float32_t fr1, float32_t fr3) {
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
      if (this->GetFlushEnable()) {
        printf("Found subnormal: Flushed: x(%.8x)->x(%.8x)\n", x.v,
               (signX) ? F32_N_ZERO : F32_P_ZERO);
      }
    }
  };
  printf("FR2");
  PrintF32DebugInfo(fr2);
  printf("FR1");
  PrintF32DebugInfo(fr1);
  printf("FR3");
  PrintF32DebugInfo(fr3);
#endif

  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (softfloat_fs) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr2 = CheckInputSubnormal(fr2);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr2;
    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1;
    fr3 = CheckInputSubnormal(fr3);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr3;
  }

  // check NaN
  {
    bool isSNaN = false;
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { fr2, fr1, fr3 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      fr2.v = 0x7fbfffff;
      fr1.v = 0x7fbfffff;
      fr3.v = 0x7fbfffff;
    } else if (qNaN.v != 0) {
      return qNaN;
    }
  }

  float32_t res;
  {
    float32_t neg;
    neg.v = fr3.v ^ 0x80000000;
    res = ::f32_mulAdd(fr2, fr1, neg);
    res = this->CheckOutputSubnormal32(res);
  }

#if 0
// #if defined(_UNIT_TEST)
  printf("FRR");
  PrintF32DebugInfo(res);
#endif

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  return res;
}

/** Mneg for fnmaf.s and fnmsf.s
 * @param[in] fr2
 * @return res.
 * @retval mullsub value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_Mneg(float32_t fr2) {

  // check NaN
  {
    bool isSNaN = false;
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { fr2 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      this->SetExceptionFlags(FPE_V);
      fr2.v = 0x7fffffff;
      return fr2;
    } else if (qNaN.v != 0) {
      return qNaN;
    }
  }

  float32_t res;
  res.v = fr2.v ^ 0x80000000; // revert sign-bit only

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

    return res;
}

/** Convert from f32 to i32
 * @param[in] fr1
 * @return converted res.
 * @retval int32_t value
 * @todo To be documented.
 */
int32_t SoftFloat::Cvt_f32_i32 (float32_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1.v;
  }

  // check NaN
  {
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { fr1 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          // SNAN
          x.v = F32_P_SNAN;
        } else {
          // QNAN
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }
  }

  int32_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    int_fast32_t f32_to_i32( float32_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f32_to_i32 (fr1, softfloat_roundingMode, true);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  // Correct output in case NAN, INF and out of range as G4MH spec.
  if ((fr1.v & F32_EXP_MASK) == F32_EXP_SET) {
    if (fr1.v == F32_P_INF) {
      res = I32_P_MAX;
    } else {
      res = I32_N_MAX;
    }
  } else {
    bool sign_fr1 = signF32UI(fr1.v);
    int_fast16_t exp_fr1 = expF32UI(fr1.v);
    if ((sign_fr1 == false) && (exp_fr1 >= F32_I32_EXP)) {
      // Replace value of SoftFloat in case the input is positive and out of range.
      res = I32_P_MAX;
    }
  }

  return res;
}


/** Convert from f32 to ui32
 * @param[in] fr1
 * @return converted res.
 * @retval uint32_t value
 * @todo To be documented.
 */
uint32_t SoftFloat::Cvt_f32_ui32 (float32_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };
    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1.v;
  }

  // check NaN
  {
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { fr1 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          // SNAN
          x.v = F32_P_SNAN;
        } else {
          // QNAN
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }
  }

  uint32_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    int_fast32_t f32_to_i32( float32_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f32_to_ui32 (fr1, softfloat_roundingMode, true);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    // Update exception flag to V in case input is negative value.
    if ((fr1.v & F32_SIGN_MASK) == F32_SIGN_SET) {
      if (fr1.v != F32_N_ZERO) {
        fpu_excp_flags = FPE_V;
      }
    }

    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    // Correct output in case NAN, INF and negative as G4MH spec.
    if ((fr1.v & F32_EXP_MASK) == F32_EXP_SET) {
      if (fr1.v == F32_P_INF) {
        res = UI32_P_MAX;
      } else {
        res = UI32_N_MAX;
      }
    } else {
        bool sign_fr1 = signF32UI(fr1.v);
        if (sign_fr1 == true) {
          // Replace value of SoftFloat in case the input is negative.
          res = UI32_N_MAX;
        }
      }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  return res;
}


/** Convert from f32 to i64
 * @param[in] fr1
 * @return converted res.
 * @retval i64 value
 * @todo To be documented.
 */
int64_t SoftFloat::Cvt_f32_i64 (float32_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1.v;
  }

  int64_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    uint_fast64_t f32_to_ui64( float32_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f32_to_i64 (fr1, softfloat_roundingMode, true);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }

    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    // Correct output in case NAN, INF and out of range as G4MH spec.
    if ((fr1.v & F32_EXP_MASK) == F32_EXP_SET) {
      if (fr1.v == F32_P_INF) {
        res = I64_P_MAX;
      } else {
        res = I64_N_MAX;
      }
    }else {
        bool sign_fr1 = signF32UI(fr1.v);
        int_fast16_t exp_fr1 = expF32UI(fr1.v);
        if ((sign_fr1 == false) && (exp_fr1 >= F32_I64_EXP)) {
          // Replace value of SoftFloat in case the input is positive and out of range.
          res = I64_P_MAX;
        }
      }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Convert from f32 to ui64
 * @param[in] fr1
 * @return converted res.
 * @retval uiint64_t value
 * @todo To be documented.
 */
uint64_t SoftFloat::Cvt_f32_ui64 (float32_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1.v;
  }

  uint64_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    uint_fast64_t f32_to_ui64( float32_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f32_to_ui64 (fr1, softfloat_roundingMode, true);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    // Update exception flag to V in case input is negative value.
    if ((fr1.v & F32_SIGN_MASK) == F32_SIGN_SET) {
      if (fr1.v != F32_N_ZERO) {
        fpu_excp_flags = FPE_V;
      }
    }

    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    // Correct output in case NAN, INF and negative as G4MH spec.
    if ((fr1.v & F32_EXP_MASK) == F32_EXP_SET) {
      if (fr1.v == F32_P_INF) {
        res = UI64_P_MAX;
      } else {
        res = UI64_N_MAX;
      }
    } else {
        bool sign_fr1 = signF32UI(fr1.v);
        if (sign_fr1 == true) {
          // Replace value of SoftFloat in case the input is negative.
          res = UI64_N_MAX;
        }
      }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Convert from f64 to i32
 * @param[in] fr1
 * @return converted res.
 * @retval int32_t value
 * @todo To be documented.
 */
int32_t SoftFloat::Cvt_f64_i32 (float64_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return static_cast<int32_t> (fr1.v);
  }

  // check NaN
  {
    float64_t qNaN;
    qNaN.v = 0;

    std::vector<float64_t> vec_args = { fr1 };
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          // SNAN
          x.v = F64_P_SNAN;
        } else {
          // QNAN
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }
  }

  int32_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    int_fast32_t f64_to_i32( float64_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f64_to_i32 (fr1, softfloat_roundingMode, true);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }

    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    // Correct output in case NAN, INF and out of range as G4MH spec.
    if ((fr1.v & F64_EXP_MASK) == F64_EXP_SET) {
      if (fr1.v == F64_P_INF) {
        res = I32_P_MAX;
      } else {
        res = I32_N_MAX;
      }
    }else {
        bool sign_fr1 = signF64UI(fr1.v);
        int_fast16_t exp_fr1 = expF64UI(fr1.v);
        uint_fast64_t frac_fr1 = fracF64UI(fr1.v);
        const uint_fast64_t F64_P_MAX_APPROXIMATE = 0x0000FFFFFFFC00000UL;
        const uint_fast64_t F64_P_MAX_APPROXIMATE_HALF = 0x0000FFFFFFFE00000UL;
        const int_fast16_t F64_I32_EXP_APPROXIMATE = F64_I32_EXP - 1;
        if ((sign_fr1 == false)
            &&    ((exp_fr1 >= F64_I32_EXP)
              ||   ((exp_fr1 == F64_I32_EXP_APPROXIMATE)
                 && (softfloat_roundingMode == softfloat_round_max)  && (frac_fr1 >  F64_P_MAX_APPROXIMATE))
              ||   ((exp_fr1 == F64_I32_EXP_APPROXIMATE)
                 && (softfloat_roundingMode == softfloat_round_near_even) && (frac_fr1 >= F64_P_MAX_APPROXIMATE_HALF))))
        {
          // Replace value of SoftFloat in case the sign is positive and
          // 1. Exp is out of range.
          // 2. round_max mode (CEIL) and the value is bigger than approximate maximum number (2^31 - 1).
          // 3. round_max mode (ROUND) and the value is bigger than approximate maximum number and a half (2^31 - 0.5).
          res = I32_P_MAX;
        }
      }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Convert from f64 to ui32
 * @param[in] fr1
 * @return converted res.
 * @retval uint32_t value
 * @todo To be documented.
 */
uint32_t SoftFloat::Cvt_f64_ui32 (float64_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return static_cast<uint32_t> (fr1.v);
  }

  // check NaN
  {
    float64_t qNaN;
    qNaN.v = 0;

    std::vector<float64_t> vec_args = { fr1 };
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          // SNAN
          x.v = F64_P_SNAN;
        } else {
          // QNAN
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }
  }

  uint32_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    int_fast32_t f32_to_i32( float32_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f64_to_ui32 (fr1, softfloat_roundingMode, true);
//  res = this->CheckOutputSubnormal64(res);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    // Update exception flag to V in case input is negative value.
    if ((fr1.v & F64_SIGN_MASK) == F64_SIGN_SET) {
      if (fr1.v != F64_N_ZERO) {
        fpu_excp_flags = FPE_V;
        res = UI32_N_MAX;
      }
    }

    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    // Correct output in case NAN, INF and negative as G4MH spec.
    if ((fr1.v & F64_EXP_MASK) == F64_EXP_SET) {
      if (fr1.v == F64_P_INF) {
        res = UI32_P_MAX;
      } else {
        res = UI32_N_MAX;
      }
    } else {
        bool sign_fr1 = signF64UI(fr1.v);
        if (sign_fr1 == true) {
          // Replace value of SoftFloat in case the input is negative.
          res = UI32_N_MAX;
        }
      }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Convert from f64 to i64
 * @param[in] fr1
 * @return converted res.
 * @retval int64_t value
 * @todo To be documented.
 */
int64_t SoftFloat::Cvt_f64_i64 (float64_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1.v;
  }

  // check NaN
  {
    float64_t qNaN;
    qNaN.v = 0;

    std::vector<float64_t> vec_args = { fr1 };
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          // SNAN
          x.v = F64_P_SNAN;
        } else {
          // QNAN
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }
  }

  int64_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    int_fast64_t f64_to_i64( float64_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f64_to_i64 (fr1, softfloat_roundingMode, true);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }

    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    // Correct output in case NAN, INF and out of range as G4MH spec.
    if ((fr1.v & F64_EXP_MASK) == F64_EXP_SET) {
      if (fr1.v == F64_P_INF) {
        res = I64_P_MAX;
      } else {
        res = I64_N_MAX;
      }
    }else {
        bool sign_fr1 = signF64UI(fr1.v);
        int_fast16_t exp_fr1 = expF64UI(fr1.v);
        if ((sign_fr1 == false) && (exp_fr1 >= F64_I64_EXP)) {
          // Replace value of SoftFloat in case the input is positive and out of range.
          res = I64_P_MAX;
        }
      }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Convert from f64 to ui64
 * @param[in] fr1
 * @return converted res.
 * @retval uint64_t value
 * @todo To be documented.
 */
uint64_t SoftFloat::Cvt_f64_ui64 (float64_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return fr1.v;
  }

  // check NaN
  {
    float64_t qNaN;
    qNaN.v = 0;

    std::vector<float64_t> vec_args = { fr1 };
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          // SNAN
          x.v = F64_P_SNAN;
        } else {
          // QNAN
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }
  }

  uint64_t res = 0;

  // prototype:
  ////////////////////////////////////////////////////////////////////////////////////////////
  //    uint_fast64_t f64_to_i3264( float64_t a, uint_fast8_t roundingMode, bool exact )       //
  ////////////////////////////////////////////////////////////////////////////////////////////
  // roundingMode: is rounding mode in SoftFloat, it is not rounding mode in G4MH.
  //               This argument is passed in each floating point instruction.
  // exact: is always true.
  res = ::f64_to_ui64 (fr1, softfloat_roundingMode, true);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    // Update exception flag to V in case input is negative value.
    if ((fr1.v & F64_SIGN_MASK) == F64_SIGN_SET) {
      if (fr1.v != F64_N_ZERO) {
        fpu_excp_flags = FPE_V;
        res = UI64_N_MAX;
      }
    }

    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    // Correct output in case NAN, INF and negative as G4MH spec.
    if ((fr1.v & F64_EXP_MASK) == F64_EXP_SET) {
      if (fr1.v == F64_P_INF) {
        res = UI64_P_MAX;
      } else {
        res = UI64_N_MAX;
      }
    } else {
        bool sign_fr1 = signF64UI(fr1.v);
        if (sign_fr1 == true) {
          // Replace value of SoftFloat in case the input is negative.
          res = UI64_N_MAX;
        }
      }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Convert from i32 to f32
 * @param[in] fr1
 * @return converted res.
 * @retval float32_t value
 * @todo To be documented.
 */
float32_t SoftFloat::Cvt_i32_f32 (int32_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float64_t i32_to_f64( int32_t a )       //
  ////////////////////////////////////////////////

  // Only Inexact exception can be occurred.
  // So, there is no behavior for exception calculation.
  float32_t res = ::i32_to_f32 (fr1);
  res = this->CheckOutputSubnormal32(res);
  return res;
}


/** Convert from ui32 to f32
 * @param[in] fr1
 * @return converted res.
 * @retval float32_t value
 * @todo To be documented.
 */
float32_t SoftFloat::Cvt_ui32_f32 (uint32_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float64_t ui32_to_f64( uint32_t a )       //
  ////////////////////////////////////////////////

  // Only Inexact exception can be occurred.
  // So, there is no behavior for exception calculation.
  float32_t res = ::ui32_to_f32 (fr1);
  res = this->CheckOutputSubnormal32(res);
  return res;
}


/** Convert from i64 to f32
 * @param[in] fr1
 * @return converted res.
 * @retval float32_t value
 * @todo To be documented.
 */
float32_t SoftFloat::Cvt_i64_f32 (int64_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float32_t i64_to_f32( int64_t a )       //
  ////////////////////////////////////////////////

  // Only Inexact exception can be occurred.
  // So, there is no behavior for exception calculation.
  float32_t res = ::i64_to_f32 (fr1);
  res = this->CheckOutputSubnormal32(res);
  return res;
}


/** Convert from ui64 to f32
 * @param[in] fr1
 * @return converted res.
 * @retval float32_t value
 * @todo To be documented.
 */
float32_t SoftFloat::Cvt_ui64_f32 (uint64_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float32_t ui64_to_f32( uint64_t a )       //
  ////////////////////////////////////////////////

  // Only Inexact exception can be occurred.
  // So, there is no behavior for exception calculation.
  float32_t res = ::ui64_to_f32 (fr1);
  res = this->CheckOutputSubnormal32(res);
  return res;
}


/** Convert from i32 to f64
 * @param[in] fr1
 * @return converted res.
 * @retval float64_t value
 * @todo To be documented.
 */
float64_t SoftFloat::Cvt_i32_f64 (int32_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float64_t i32_to_f64( int32_t a )       //
  ////////////////////////////////////////////////

  // There is no exception for this instruction.
  float64_t res = ::i32_to_f64 (fr1);
  res = this->CheckOutputSubnormal64(res);

   return res;
}


/** Convert from ui32 to f64
 * @param[in] fr1
 * @return converted res.
 * @retval float64_t value
 * @todo To be documented.
 */
float64_t SoftFloat::Cvt_ui32_f64 (uint32_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float64_t ui32_to_f64( uint32_t a )       //
  ////////////////////////////////////////////////

  // There is no exception for this instruction.
  float64_t res = ::ui32_to_f64 (fr1);
  res = this->CheckOutputSubnormal64(res);

  return res;
}


/** Convert from i64 to f64
 * @param[in] fr1
 * @return converted res.
 * @retval float64_t value
 * @todo To be documented.
 */
float64_t SoftFloat::Cvt_i64_f64 (int64_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float64_t i64_to_f64( int64_t a )       //
  ////////////////////////////////////////////////

  // Only Inexact exception can be occurred.
  // So, there is no behavior for exception calculation.
  float64_t res = ::i64_to_f64 (fr1);
  res = this->CheckOutputSubnormal64(res);

  return res;
}


/** Convert from ui64 to f64
 * @param[in] fr1
 * @return converted res.
 * @retval float64_t value
 * @todo To be documented.
 */
float64_t SoftFloat::Cvt_ui64_f64 (uint64_t fr1) {
  // prototype:
  ////////////////////////////////////////////////
  //    float64_t ui64_to_f64( uint64_t a )       //
  ////////////////////////////////////////////////

  // Only Inexact exception can be occurred.
  // So, there is no behavior for exception calculation.
  float64_t res = ::ui64_to_f64 (fr1);
  res = this->CheckOutputSubnormal64(res);

  return res;
}


float32_t SoftFloat::f64_to_f32 (float64_t fr1) {
  {
    auto CheckSubnormal64 = [&](float64_t x) -> float64_t {
      bool _signX = signF64UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
          //is_subnormal_flush = true;
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckSubnormal64(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E){
      float32_t fr2;
      fr2.v = 0;
      return fr2;
    }
  }

  // check NaN
  {
    bool isSNaN = false;
    float32_t qNaN;
    float32_t fr2;
    qNaN.v = 0;
    fr2.v = 0;

    std::vector<float64_t> vec_args = {fr1};
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {              // is NaN
        if (::f64_isSignalingNaN(x)) {    // is SNaN
          isSNaN = true;
        } else {                          // is QNaN
          uint64_t tmp = (fr1.v & 0x000fffffffffffffULL) >> 29;
          if (tmp != 0) {
            tmp = tmp | F32_EXP_MASK | ((fr1.v & F64_SIGN_MASK) >> 32);
            qNaN.v = (tmp & F32_MASK);
          } else {
            qNaN.v = F32_P_QNAN;
          }
        }
      }
    }

    if (isSNaN) {
      fr2.v = F32_P_QNAN;
      this->SetExceptionFlags(FPE_V);
      return fr2;
    } else if (qNaN.v != 0) {
      return qNaN;
    }
  }

  float32_t fr2;
  fr2 = ::f64_to_f32(fr1);
  fr2 = this->CheckOutputSubnormal32(fr2);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    if ((fpu_excp_flags & FPE_E) == FPE_E){
      fr2.v = 0;
      return fr2;
    }

    if (((fpu_excp_flags & FPE_U) != 0)) {
        if (softfloat_fs == 0) {
               this->SetExceptionFlags(FPE_E);
            fr2.v = 0;
            return fr2;
        }
    }

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags);// Epilogue
}

return fr2;

}


float64_t SoftFloat::f32_to_f64(float32_t fr1) {
  {
    auto CheckSubnormal32 = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr1 = CheckSubnormal32(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E){
      float64_t fr2;
      fr2.v = UI64_N_MAX;
      return fr2;
    }
  }

  // check NaN
  {
    bool isSNaN = false;
    float64_t qNaN;
    float64_t fr2;
    qNaN.v = 0;
    fr2.v = 0;

    std::vector<float32_t> vec_args = {fr1};
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {             // is NaN
        if (::f32_isSignalingNaN(x)) {   // is SNaN
          isSNaN = true;
        } else {                         // is QNaN
          uint64_t tmp = (F64_EXP_MASK
                | ((static_cast<uint64_t>(fr1.v) & 0x007fffff) << 29)
                | ((static_cast<uint64_t>(fr1.v) & F32_SIGN_MASK) << 32));
          qNaN.v = tmp;
        }
      }
    }

    if (isSNaN) {
      fr2.v = F64_P_QNAN;
      this->SetExceptionFlags(FPE_V);
      return fr2;
    } else if (qNaN.v != 0) {
      return qNaN;
    }
  }

  float64_t fr2;
  fr2 = ::f32_to_f64(fr1);
  fr2 = this->CheckOutputSubnormal64(fr2);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    if ((fpu_excp_flags & FPE_E) == FPE_E){
        fr2.v = UI64_N_MAX;
        return fr2;
    }

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags);// Epilogue
  }

  return fr2;
}


float16_t SoftFloat::f32_to_f16(float32_t fr1) {
  {
    auto CheckSubnormal32 = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
  };

    fr1 = CheckSubnormal32(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E){
      float16_t fr2;
      fr2.v = 0;
      return fr2;
    }
  }

  // check NaN
  {
    bool isSNaN = false;
    float16_t qNaN;
    float16_t fr2;
    qNaN.v = 0;
    fr2.v = 0;

    std::vector<float32_t> vec_args = {fr1};
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {                // is NaN
        if (::f32_isSignalingNaN(x)) {      // is SNaN
          isSNaN = true;
        } else {                            // is QNaN
          uint32_t tmp = (fr1.v & 0x007fffffU) >> 13;
          if (tmp != 0) {
            tmp = tmp | F16_EXP_MASK | ((fr1.v & F32_SIGN_MASK) >> 16);
            qNaN.v = (tmp & F16_MASK);
          } else {
            qNaN.v = F16_P_QNAN;
          }
        }
      }
    }

    if (isSNaN) {
      fr2.v = F16_P_QNAN;
      this->SetExceptionFlags(FPE_V);
      return fr2;
    } else if (qNaN.v != 0) {
      return qNaN;
    }
  }

  float16_t fr2;
  fr2 = ::f32_to_f16(fr1);
  fr2 = this->CheckOutputSubnormal16(fr2);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    if ((fpu_excp_flags & FPE_E) == FPE_E){
      fr2.v = 0;
      return fr2;
    }

    if (((fpu_excp_flags & FPE_U) != 0)) {
        if (softfloat_fs == 0) {
               this->SetExceptionFlags(FPE_E);
            fr2.v = 0;
            return fr2;
        }
    }

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags);// Epilogue
  }
  return fr2;
}


float32_t SoftFloat::f16_to_f32( float16_t fr1){
  // check NaN
  {
    bool isSNaN = false;
    float32_t qNaN;
    float32_t fr2;
    qNaN.v = 0;
    fr2.v = 0;

    std::vector<float16_t> vec_args = {fr1};
    for (float16_t x : vec_args) {
      if (isNaNF16UI(x.v)) {                // is NaN
        if (::f16_isSignalingNaN(x)) {      // is SNaN
          isSNaN = true;
        } else {                            // is QNaN
          uint32_t tmp = (F32_EXP_MASK
                         | (static_cast<uint32_t> (fr1.v & 0x03ffU) << 13)
                         | (static_cast<uint32_t> (fr1.v & F16_SIGN_MASK) << 16));

          qNaN.v = (tmp & F32_MASK);
        }
      }
    }

    if (isSNaN) {
      fr2.v = F32_P_QNAN;
      this->SetExceptionFlags(FPE_V);
      return fr2;
    } else if (qNaN.v != 0) {
      return qNaN;
    }
  }

  this->SetExceptionFlags(0);
  float32_t fr2;
  fr2 = ::f16_to_f32(fr1);
  fr2 = this->CheckOutputSubnormal32(fr2);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags);// Epilogue
  }

  return fr2;
}


/** Absf.s
 * @param[in] fr2
 * @return float32_t res.
 * @retval abs value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_absfs (float32_t fr2) {

   float32_t res;
   res.v = fr2.v & ~F32_N_ZERO; // sign-bit affected only

    {
      uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

      // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
      if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      // 不正確演算例外(I)の検出条件
      if (fpu_excp_flags & FPE_V) {
        fpu_excp_flags = FPE_V;
      }
      if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      }
      if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
        fpu_excp_flags |= FPE_I;
      }
      this->SetExceptionFlags(fpu_excp_flags); // Epilogue
    }

  return res;
}


/** Absf.d
 * @param[in] fr2
 * @return float64_t res.
 * @retval abs value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_absfd(float64_t fr2) {

    float64_t res;
    res.v = fr2.v & ~F64_N_ZERO; // sign-bit affected only

    {
      uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

      // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
      if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      // 不正確演算例外(I)の検出条件
      if (fpu_excp_flags & FPE_V) {
        fpu_excp_flags = FPE_V;
      }
      if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      }
      if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
        fpu_excp_flags |= FPE_I;
      }
      this->SetExceptionFlags(fpu_excp_flags); // Epilogue
    }

    return res;
}


/** Negf.s
 * @param[in] fr2
 * @return float32_t res.
 * @retval neg value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_negfs (float32_t fr2) {

  float32_t res;
  res.v = fr2.v ^ 0x80000000; // revert sign-bit only

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

  return res;
}


/** Negf.d
 * @param[in] fr2
 * @return float64_t res.
 * @retval neg value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_negfd(float64_t fr2) {

   float64_t res;
   res.v = fr2.v ^ F64_N_ZERO; // revert sign-bit only

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

  return res;
}


/** Sqrtf.s
 * @param[in] fr2
 * @return float32_t res.
 * @retval sqrt value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_sqrtfs (float32_t fr2) {

   // check subnormal and flush-zero
   {
     auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
       bool _signX = signF32UI(x.v);
       int_fast16_t _expX = expF32UI(x.v);
       uint_fast32_t _sigX = fracF32UI(x.v);
       if ((_expX == 0) && (_sigX != 0)) {
         if (this->GetFlushEnable()) {
           x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
           this->SetInputFlushFlag(true);
         } else {
           this->SetExceptionFlags(FPE_E);
         }
       }
       return x;
     };

     fr2 = CheckInputSubnormal(fr2);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr2;
     }
   }

   // check NaN
   {
     bool isSNaN = false;
     float32_t qNaN;
     qNaN.v = 0;

     std::vector<float32_t> vec_args = { fr2};
     for (float32_t x : vec_args) {
       if (isNaNF32UI(x.v)) {
         if (::f32_isSignalingNaN(x)) {
           isSNaN = true;
         } else {
           if (qNaN.v == 0) qNaN = x;
         }
       }
     }

     if (isSNaN) {
       fr2.v = 0x7fbfffff;
     } else if (qNaN.v != 0) {
       return qNaN;
     }
   }

   float32_t res = ::f32_sqrt(fr2);
   res = this->CheckOutputSubnormal32(res);

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

  return res;
}


/** Sqrtf.d
 * @param[in] fr2
 * @return float64_t res.
 * @retval sqrt value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_sqrtfd(float64_t fr2) {

    // check subnormal and flush-zero
    {
        auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
          bool _signX = signF64UI(x.v);
          int_fast16_t _expX = expF64UI(x.v);
          uint_fast64_t _sigX = fracF64UI(x.v);
          if ((_expX == 0) && (_sigX != 0)) {
            if (this->GetFlushEnable()) {
              x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
              this->SetInputFlushFlag(true);
            } else {
              this->SetExceptionFlags(FPE_E);
            }
          }
          return x;
        };

        fr2 = CheckInputSubnormal(fr2);
        if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
            return fr2;
        }
      }

      // check NaN
      {
        bool isSNaN = false;
        float64_t qNaN;
        qNaN.v = 0;

        std::vector<float64_t> vec_args = {fr2};
        for (float64_t x : vec_args) {
          if (isNaNF64UI(x.v)) {
            if (::f64_isSignalingNaN(x)) {
              isSNaN = true;
            } else {
              if (qNaN.v == 0) qNaN = x;
            }
          }
        }

        if (isSNaN) {
          this->SetExceptionFlags(FPE_V);
          fr2.v = F64_P_QNAN;
        } else if (qNaN.v != 0) {
          return qNaN;
        }
    }

    float64_t res;
    res = ::f64_sqrt(fr2);
    res = this->CheckOutputSubnormal64(res);

    {
      uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

      // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
      if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      // 不正���演算例外(I)の検出条件
      if (fpu_excp_flags & FPE_V) {
        fpu_excp_flags = FPE_V;
      }
      if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      }
      if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
        fpu_excp_flags |= FPE_I;
      }
      this->SetExceptionFlags(fpu_excp_flags); // Epilogue
    }

  return res;
}


/** Rsqrt.s
 * @param[in] freg1
 * @return float32_t res.
 * @retval rsqrt value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_rsqrt(float32_t freg1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast32_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    freg1 = CheckInputSubnormal(freg1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) return freg1;
  }

  // check NaN
  {
    bool isSNaN = false;
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { freg1 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          // SNAN
          isSNaN = true;
        } else {
          // QNAN
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }
    if (isSNaN) {
      freg1.v = F32_P_SNAN;
    } else if (qNaN.v != 0) {
        return qNaN;
    }
  }

  float32_t res;

  //- Calculate rqrtf.s by 4 softfloat functions:
  //-- Convert from F32 to F64
  //-- Use F64_SQRT
  //-- USE F64_DIV
  //-- Convert from F64 to F32
  float64_t fr1_f64 = f32_to_f64(freg1);
  float64_t fr1_sqrt_f64 = f64_sqrt(fr1_f64);
  float64_t val_1;
  val_1.v = 0x3ff0000000000000ULL;
  float64_t fr1_rsqrt_f64 = f64_div(val_1, fr1_sqrt_f64);
  res = ::f64_to_f32 (fr1_rsqrt_f64);
  res = this->CheckOutputSubnormal32(res);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Update exception flag to V in case input is negative value.
    // -- Sign bit is (-)
    // ---- In case (-)0: (-)N_INF, FPE_Z = 1.
    // ---- In case (-)INF, Normal: P_QNAN, FPE_V = 1.
    // -- Sign bit is (+)
    // ---- In case INF: 0, FPE_NONE.
    // ---- In case P_SNAN: P_QNAN, FPE_V = 1.
    // ---- In case 0: (+) P_INF, FPE_Z = 1.
    // ---- In case Normal: Calculation result, FPE_I = 1.

    if ((freg1.v & F32_SIGN_MASK) == F32_SIGN_SET) {
      if (freg1.v == F32_N_ZERO) {
        // Result is self calculation, only update flag
        fpu_excp_flags = FPE_Z;
      } else {
        res.v = F32_P_QNAN;
        fpu_excp_flags = FPE_V;
      }
    } else {
      if (freg1.v == F32_P_ZERO) {
          // Result is self calculation, only update flag
          fpu_excp_flags = FPE_Z;
      } else if (freg1.v == F32_P_INF) {
        fpu_excp_flags = FPE_NONE;
      } else if (freg1.v == F32_P_SNAN) {
        res.v = F32_P_QNAN;
        fpu_excp_flags = FPE_V;
      } else {
        fpu_excp_flags = FPE_I;
      }
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  return res;
}


/** Rsqrt.d
 * @param[in] freg1
 * @return float64_t res.
 * @retval rsqrt value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_rsqrt(float64_t freg1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
      bool _signX = signF64UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    freg1 = CheckInputSubnormal(freg1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
        return freg1;
    }
  }

  // check NaN
  {
    bool isSNaN = false;
    float64_t qNaN;
    qNaN.v = 0;

    std::vector<float64_t> vec_args = { freg1 };
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      freg1.v = F64_P_SNAN;
    } else if (qNaN.v != 0) {
        return qNaN;
    }
  }

  float64_t res;

  //- Calculate rqrtf.s by 4 softfloat functions:
  //-- Convert from F64 to F128
  //-- Use F128_SQRT
  //-- USE F128_DIV
  //-- Convert from F128 to F64
  float128_t fr1_f128 = f64_to_f128(freg1);
  float128_t fr1_sqrt_f128 = f128_sqrt(fr1_f128);
  float64_t val_1;
  val_1.v = 0x3ff0000000000000ULL;
  float128_t val_1_f128 = f64_to_f128(val_1);
  float128_t fr1_rsqrt_f128 = f128_div(val_1_f128, fr1_sqrt_f128);
  res = ::f128_to_f64 (fr1_rsqrt_f128);
  res = this->CheckOutputSubnormal64(res);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Update exception flag to V in case input is negative value.
    // -- Sign bit is (-)
    // ---- In case (-)0: (-)N_INF, FPE_Z = 1.
    // ---- In case (-)INF, Normal: P_QNAN, FPE_V = 1.
    // -- Sign bit is (+)
    // ---- In case INF: 0, FPE_NONE.
    // ---- In case P_SNAN: P_QNAN, FPE_V = 1.
    // ---- In case 0: (+) P_INF, FPE_Z = 1.
    // ---- In case Normal: Calculation result, FPE_I = 1.

    if ((freg1.v & F64_SIGN_MASK) == F64_SIGN_SET) {
      if (freg1.v == F64_N_ZERO) {
        // Result is self calculation, only update flag
        fpu_excp_flags = FPE_Z;
      } else {
        res.v = F64_P_QNAN;
        fpu_excp_flags = FPE_V;
      }
    } else {
      if (freg1.v == F64_P_ZERO) {
          // Result is self calculation, only update flag
          fpu_excp_flags = FPE_Z;
      } else if (freg1.v == F64_P_INF) {
        fpu_excp_flags = FPE_NONE;
      } else if (freg1.v == F64_P_SNAN) {
        res.v = F64_P_QNAN;
        fpu_excp_flags = FPE_V;
      } else {
        fpu_excp_flags = FPE_I;
      }
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  return res;
}


/** Recipfs
 * @param[in] fr2
 * @retval recip value
 * @todo To be documented.
*/
float32_t SoftFloat::f32_recipfs(float32_t fr2) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast64_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr2 = CheckInputSubnormal(fr2);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
        return fr2;
    }
  }

  // check NaN
  bool isSNaN = false;
  uint32_t fpu_excp_flags = FPE_NONE;
  {
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { fr2 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      fr2.v = F32_P_SNAN;
      this->SetExceptionFlags(FPE_V);
      fpu_excp_flags = this->GetExceptionFlags();
    } else if (qNaN.v != 0) {
        return qNaN;
    }
  }


  float32_t res, fr1;
  fr1.v = 0x3f800000; // fr1 = 1
  res = ::f32_div(fr1,fr2); //Recip is 1/x
  res = this->CheckOutputSubnormal32(res);


  /* How to check exception flag:
   * 1. Get all exception flag.
   * 2. Check E, V, Z and I exception flag
   *   If ( Exception flag is one of (E,V,Z) ):
   *     - If E flag is set --> Exception flag is E.
   *     - If V flag is set --> Exception flag is V.
   *     - If Z flag is set --> Exception flag is Z.
   *   Else
   *     - Set I to exception flag.
   *       + In normal case, I flag is always set in recipf instruction.
   *       + Other case:
   *         If
   *           O flag and overflow exception enable
   *         Else if
   *           U flag and underflow exception enable.
   *         Or
   *           FPSR.IF is set and neither V or Z flag is set.
   *     - If U flag is set (output is subnormal) --> Add U to exception flag.
   *     - Check condition to clear I flag
   *       If
   *         O flag and overflow exception disable
   *       Else if
   *         U flag and underflow exception disable
   *       Or
   *         Input is infinitive.
   */
  {
    uint32_t exp_code = this->GetExceptionFlags(); // Prologue
    fpu_excp_flags |= exp_code;

    if ( (fpu_excp_flags & FPE_E) || (fpu_excp_flags & FPE_V) || (fpu_excp_flags & FPE_Z) ) {
      if (fpu_excp_flags & FPE_E) {
        fpu_excp_flags = FPE_E;
      } else if (fpu_excp_flags & FPE_V) {
        fpu_excp_flags = FPE_V;
      } else if (fpu_excp_flags & FPE_Z) { //Check in case 1/0
        fpu_excp_flags = FPE_Z;
      }
    } else {
        fpu_excp_flags |= FPE_I;  //Always have I flag
        if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
          fpu_excp_flags |= FPE_I;
        } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
          fpu_excp_flags |= FPE_I;
        }
        if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
          fpu_excp_flags |= FPE_I;
        }
        if (fpu_excp_flags & FPE_U) { //output is subnormal
          fpu_excp_flags |= FPE_U;
        }
        if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
          fpu_excp_flags &= ~FPE_I;
        } else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
          fpu_excp_flags &= ~FPE_I;
        }
        if ((fr2.v & ~(F32_SIGN_MASK)) == F32_EXP_SET) { //Infinite
          fpu_excp_flags &= ~FPE_I;
        }
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Recipfd
 * @param[in] fr2
 * @return float64_t res.
 * @retval Recip value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_recipfd(float64_t fr2) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
      bool _signX = signF64UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr2 = CheckInputSubnormal(fr2);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
        return fr2;
    }
  }

  // check NaN
  bool isSNaN = false;
  uint32_t fpu_excp_flags = FPE_NONE;
  {
    float64_t qNaN;
    qNaN.v = 0;

    std::vector<float64_t> vec_args = { fr2 };
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      fr2.v = F64_P_SNAN;
      this->SetExceptionFlags(FPE_V);
      fpu_excp_flags = this->GetExceptionFlags();
    } else if (qNaN.v != 0) {
        return qNaN;
    }
  }


  float64_t res, fr1;
  fr1.v = 0x3ff0000000000000; //fr1 = 1
  res = ::f64_div(fr1,fr2);   //Recip is 1/x
  res = this->CheckOutputSubnormal64(res);

    /* How to check exception flag:
   * 1. Get all exception flag.
   * 2. Check E, V, Z exception flag
   *   If ( Exception flag is one of (E,V,Z) ):
   *     - If E flag is set --> Exception flag is E.
   *     - If V flag is set --> Exception flag is V.
   *     - If Z flag is set --> Exception flag is Z.
   *   Else
   *     - Set I to exception flag.
   *       + In normal case, I flag is always set in recipf instruction.
   *       + Other case:
   *         If
   *           O flag and overflow exception enable
   *         Else if
   *           U flag and underflow exception enable.
   *         Or
   *           FPSR.IF is set and neither V or Z flag is set.
   *     - If U flag is set (output is subnormal) --> Add U to exception flag.
   *     - Check condition to clear I flag
   *       If
   *         O flag and overflow exception disable
   *       Else if
   *         U flag and underflow exception disable
   *       Or
   *         Input is infinitive.
   */
  {
    uint32_t exp_code = this->GetExceptionFlags(); // Prologue
    fpu_excp_flags |= exp_code;

    if ( (fpu_excp_flags & FPE_E) || (fpu_excp_flags & FPE_V) || (fpu_excp_flags & FPE_Z) ) {
      if (fpu_excp_flags & FPE_E) {
        fpu_excp_flags = FPE_E;
      } else if (fpu_excp_flags & FPE_V) {
        fpu_excp_flags = FPE_V;
      } else if (fpu_excp_flags & FPE_Z) { //Check in case 1/0
        fpu_excp_flags = FPE_Z;
      }
    } else {
        fpu_excp_flags |= FPE_I;  //Always have I flag
        if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
          fpu_excp_flags |= FPE_I;
        } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
          fpu_excp_flags |= FPE_I;
        }
        if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
          fpu_excp_flags |= FPE_I;
        }
        if (fpu_excp_flags & FPE_U) { //output is subnormal
          fpu_excp_flags |= FPE_U;
        }
        if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
          fpu_excp_flags &= ~FPE_I;
        } else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
          fpu_excp_flags &= ~FPE_I;
        }
        if ((fr2.v & ~(F64_SIGN_MASK)) == F64_EXP_SET) { //Infinite
          fpu_excp_flags &= ~FPE_I;
        }
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Addf.s
 * @param[in] fr2
 * @param[in] fr1
 * @return float32_t res.
 * @retval add value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_addfs (float32_t fr2, float32_t fr1) {

     // check subnormal and flush-zero
     {
       auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
         bool _signX = signF32UI(x.v);
         int_fast16_t _expX = expF32UI(x.v);
         uint_fast32_t _sigX = fracF32UI(x.v);
         if ((_expX == 0) && (_sigX != 0)) {
           if (this->GetFlushEnable()) {
             x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
             this->SetInputFlushFlag(true);
           } else {
             this->SetExceptionFlags(FPE_E);
           }
         }
         return x;
       };

       fr2 = CheckInputSubnormal(fr2);
       if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
           return fr2;
       }
       fr1 = CheckInputSubnormal(fr1);
       if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
           return fr1;
       }
     }

     // check NaN
     bool isSNaN = false;
     {
       float32_t qNaN;
       qNaN.v = 0;

       std::vector<float32_t> vec_args = { fr2, fr1 };
       for (float32_t x : vec_args) {
         if (isNaNF32UI(x.v)) {
           if (::f32_isSignalingNaN(x)) {
             isSNaN = true;
           } else {
             if (qNaN.v == 0) qNaN = x;
           }
         }
       }

       if (isSNaN) {
         fr2.v = F32_P_SNAN;
         fr1.v = F32_P_SNAN;
       } else if (qNaN.v != 0) {
         return qNaN;
       }
     }

   float32_t res = ::f32_add(fr2, fr1);
   res = this->CheckOutputSubnormal32(res);

     {
       uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

       // Overflow/Underflow が立っているときは���正確演算例外(I)をマスクする
       if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
         fpu_excp_flags &= ~FPE_I;
       }
       else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
         fpu_excp_flags &= ~FPE_I;
       }
       // 不正確演算例外(I)の検出条件
       if (fpu_excp_flags & FPE_V) {
         fpu_excp_flags = FPE_V;
       }
       if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
         fpu_excp_flags |= FPE_I;
       } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
         fpu_excp_flags |= FPE_I;
       }
       if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
         fpu_excp_flags |= FPE_I;
       }
       this->SetExceptionFlags(fpu_excp_flags); // Epilogue
     }

    return res;
}


/** Addf.d
 * @param[in] fr2
 * @param[in] fr1
 * @return float64_t res.
 * @retval add value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_addfd (float64_t fr2, float64_t fr1) {

    // check subnormal and flush-zero
    {
      auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
        bool _signX = signF64UI(x.v);
        int_fast16_t _expX = expF64UI(x.v);
        uint_fast64_t _sigX = fracF64UI(x.v);
        if ((_expX == 0) && (_sigX != 0)) {
          if (this->GetFlushEnable()) {
            x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
            this->SetInputFlushFlag(true);
          } else {
            this->SetExceptionFlags(FPE_E);
          }
        }
        return x;
      };

      fr2 = CheckInputSubnormal(fr2);
      if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
          return fr2;
      }
      fr1 = CheckInputSubnormal(fr1);
      if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
          return fr1;
      }
    }

    // check NaN
    bool isSNaN = false;
    {
      float64_t qNaN;
      qNaN.v = 0;

      std::vector<float64_t> vec_args = { fr2, fr1 };
      for (float64_t x : vec_args) {
        if (isNaNF64UI(x.v)) {
          if (::f64_isSignalingNaN(x)) {
            isSNaN = true;
          } else {
            if (qNaN.v == 0) qNaN = x;
          }
        }
      }

      if (isSNaN) {
        fr2.v = F64_P_SNAN;
        fr1.v = F64_P_SNAN;
      } else if (qNaN.v != 0) {
        return qNaN;
      }
    }

   float64_t res = ::f64_add(fr2, fr1);
   res = this->CheckOutputSubnormal64(res);

    {
      uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

      // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
      if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags &= ~FPE_I;
      }
      // 不正確演算例外(I)の検出条件
      if (fpu_excp_flags & FPE_V) {
        fpu_excp_flags = FPE_V;
      }
      if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
        fpu_excp_flags |= FPE_I;
      }
      if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
        fpu_excp_flags |= FPE_I;
      }
      this->SetExceptionFlags(fpu_excp_flags); // Epilogue
    }

   return res;
}


/** Subf.s
 * @param[in] fr2
 * @param[in] fr1
 * @return float32_t res.
 * @retval sub value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_subfs (float32_t fr2, float32_t fr1) {

   // check subnormal and flush-zero
   {
     auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
       bool _signX = signF32UI(x.v);
       int_fast16_t _expX = expF32UI(x.v);
       uint_fast32_t _sigX = fracF32UI(x.v);
       if ((_expX == 0) && (_sigX != 0)) {
         if (this->GetFlushEnable()) {
           x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
           this->SetInputFlushFlag(true);
         } else {
           this->SetExceptionFlags(FPE_E);
         }
       }
       return x;
     };

     fr2 = CheckInputSubnormal(fr2);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr2;
     }
     fr1 = CheckInputSubnormal(fr1);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr1;
     }
   }

   // check NaN
   {
     bool isSNaN = false;
     float32_t qNaN;
     qNaN.v = 0;

     std::vector<float32_t> vec_args = { fr2, fr1 };
     for (float32_t x : vec_args) {
       if (isNaNF32UI(x.v)) {
         if (::f32_isSignalingNaN(x)) {
           isSNaN = true;
         } else {
           if (qNaN.v == 0) qNaN = x;
         }
       }
     }

     if (isSNaN) {
       fr2.v = F32_P_SNAN;
       fr1.v = F32_P_SNAN;
     } else if (qNaN.v != 0) {
       return qNaN;
     }
   }

   float32_t res = ::f32_sub(fr2, fr1);
   res = this->CheckOutputSubnormal32(res);

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

  return res;
}


/** Subf.d
 * @param[in] fr2
 * @param[in] fr1
 * @return float64_t res.
 * @retval sub value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_subfd (float64_t fr2, float64_t fr1) {

   // check subnormal and flush-zero
   {
     auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
       bool _signX = signF64UI(x.v);
       int_fast16_t _expX = expF64UI(x.v);
       uint_fast64_t _sigX = fracF64UI(x.v);
       if ((_expX == 0) && (_sigX != 0)) {
         if (this->GetFlushEnable()) {
           x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
           this->SetInputFlushFlag(true);
         } else {
           this->SetExceptionFlags(FPE_E);
         }
       }
       return x;
     };

     fr2 = CheckInputSubnormal(fr2);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr2;
     }
     fr1 = CheckInputSubnormal(fr1);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr1;
     }
   }

   // check NaN
   bool isSNaN = false;
   {
     float64_t qNaN;
     qNaN.v = 0;

     std::vector<float64_t> vec_args = { fr2, fr1 };
     for (float64_t x : vec_args) {
       if (isNaNF64UI(x.v)) {
         if (::f64_isSignalingNaN(x)) {
           isSNaN = true;
         } else {
           if (qNaN.v == 0) qNaN = x;
         }
       }
     }

     if (isSNaN) {
       fr2.v = F64_P_SNAN;
       fr1.v = F64_P_SNAN;
     } else if (qNaN.v != 0) {
       return qNaN;
     }
   }

  float64_t res = ::f64_sub(fr2, fr1);
  res = this->CheckOutputSubnormal64(res);

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

  return res;
}


/** Mulf.s
 * @param[in] fr2
 * @param[in] fr1
 * @return float32_t res.
 * @retval mul value
 * @todo To be documented.
 */
float32_t SoftFloat::f32_mulfs (float32_t fr2, float32_t fr1) {

   // check subnormal and flush-zero
   {
     auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
       bool _signX = signF32UI(x.v);
       int_fast16_t _expX = expF32UI(x.v);
       uint_fast32_t _sigX = fracF32UI(x.v);
       if ((_expX == 0) && (_sigX != 0)) {
         if (this->GetFlushEnable()) {
           x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
           this->SetInputFlushFlag(true);
         } else {
           this->SetExceptionFlags(FPE_E);
         }
       }
       return x;
     };

     fr2 = CheckInputSubnormal(fr2);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr2;
     }
     fr1 = CheckInputSubnormal(fr1);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr1;
     }
   }

   // check NaN
   {
     bool isSNaN = false;
     float32_t qNaN;
     qNaN.v = 0;

     std::vector<float32_t> vec_args = { fr2, fr1 };
     for (float32_t x : vec_args) {
       if (isNaNF32UI(x.v)) {
         if (::f32_isSignalingNaN(x)) {
           isSNaN = true;
         } else {
           if (qNaN.v == 0) qNaN = x;
         }
       }
     }

     if (isSNaN) {
       fr2.v = 0x7fbfffff;
       fr1.v = 0x7fbfffff;
     } else if (qNaN.v != 0) {
       return qNaN;
     }
   }

   float32_t res = ::f32_mul(fr2, fr1);
   res = this->CheckOutputSubnormal32(res);

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

  return res;
}


/** Mulf.d
 * @param[in] fr2
 * @param[in] fr1
 * @return float64_t res.
 * @retval mul value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_mulfd(float64_t fr2, float64_t fr1) {

   // check subnormal and flush-zero
   {
     auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
       bool _signX = signF64UI(x.v);
       int_fast16_t _expX = expF64UI(x.v);
       uint_fast64_t _sigX = fracF64UI(x.v);
       if ((_expX == 0) && (_sigX != 0)) {
         if (this->GetFlushEnable()) {
           x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
           this->SetInputFlushFlag(true);
         } else {
           this->SetExceptionFlags(FPE_E);
         }
       }
       return x;
     };

     fr2 = CheckInputSubnormal(fr2);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr2;
     }
     fr1 = CheckInputSubnormal(fr1);
     if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
         return fr1;
     }
   }

   // check NaN
   bool isSNaN = false;
   {
     float64_t qNaN;
     qNaN.v = 0;

     std::vector<float64_t> vec_args = { fr2, fr1 };
     for (float64_t x : vec_args) {
       if (isNaNF64UI(x.v)) {
         if (::f64_isSignalingNaN(x)) {
           isSNaN = true;
         } else {
           if (qNaN.v == 0) qNaN = x;
         }
       }
     }

     if (isSNaN) {
       fr2.v = F64_P_SNAN;
       fr1.v = F64_P_SNAN;
     } else if (qNaN.v != 0) {
       return qNaN;
     }
   }

   float64_t res = ::f64_mul(fr2, fr1);
   res = this->CheckOutputSubnormal64(res);

   {
     uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

     // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
     if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags &= ~FPE_I;
     }
     // 不正確演算例外(I)の検出条件
     if (fpu_excp_flags & FPE_V) {
       fpu_excp_flags = FPE_V;
     }
     if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
       fpu_excp_flags |= FPE_I;
     }
     if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
       fpu_excp_flags |= FPE_I;
     }
     this->SetExceptionFlags(fpu_excp_flags); // Epilogue
   }

  return res;
}


/** Divfs
 * @param[in] fr2
 * @param[in] fr1
 * @retval div value
 * @todo To be documented.
*/
float32_t SoftFloat::f32_divfs(float32_t fr2, float32_t fr1) {
// check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float32_t x) -> float32_t {
      bool _signX = signF32UI(x.v);
      int_fast16_t _expX = expF32UI(x.v);
      uint_fast64_t _sigX = fracF32UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F32_N_ZERO : F32_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr2 = CheckInputSubnormal(fr2);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
        return fr2;
    }
    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
        return fr1;
    }
  }

  // check NaN
  bool isSNaN = false;
  {
    float32_t qNaN;
    qNaN.v = 0;

    std::vector<float32_t> vec_args = { fr2, fr1 };
    for (float32_t x : vec_args) {
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      fr2.v = F32_P_SNAN;
      fr1.v = F32_P_SNAN;
    } else if (qNaN.v != 0) {
        return qNaN;
    }
  }


  float32_t res;
  res = ::f32_div(fr2,fr1);
  res = this->CheckOutputSubnormal32(res);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    if (isSNaN) {
      this->SetExceptionFlags(FPE_V);
    }
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Divfd
 * @param[in] fr2
 * @param[in] fr1
 * @return float64_t res.
 * @retval Div value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_divfd(float64_t fr2, float64_t fr1) {
  // check subnormal and flush-zero
  {
    auto CheckInputSubnormal = [&](float64_t x) -> float64_t {
      bool _signX = signF64UI(x.v);
      int_fast16_t _expX = expF64UI(x.v);
      uint_fast64_t _sigX = fracF64UI(x.v);
      if ((_expX == 0) && (_sigX != 0)) {
        if (this->GetFlushEnable()) {
          x.v = (_signX) ? F64_N_ZERO : F64_P_ZERO;
          this->SetInputFlushFlag(true);
        } else {
          this->SetExceptionFlags(FPE_E);
        }
      }
      return x;
    };

    fr2 = CheckInputSubnormal(fr2);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
        return fr2;
    }
    fr1 = CheckInputSubnormal(fr1);
    if ((this->GetExceptionFlags() & FPE_E) == FPE_E) {
        return fr1;
    }
  }

  // check NaN
  bool isSNaN = false;
  {
    float64_t qNaN;
    qNaN.v = 0;

    std::vector<float64_t> vec_args = { fr2, fr1 };
    for (float64_t x : vec_args) {
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          isSNaN = true;
        } else {
          if (qNaN.v == 0) qNaN = x;
        }
      }
    }

    if (isSNaN) {
      fr2.v = F64_P_SNAN;
      fr1.v = F64_P_SNAN;
    } else if (qNaN.v != 0) {
        return qNaN;
    }
  }

  float64_t res;
  res = ::f64_div(fr2, fr1);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    if (isSNaN) {
      this->SetExceptionFlags(FPE_V);
    }
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return res;
}


/** Minfs
 * @param[in] fr2
 * @param[in] fr1
 * @retval min value
 * @todo To be documented.
*/
float32_t SoftFloat::f32_minfs(float32_t fr2, float32_t fr1) {
  float32_t qNaN;
  qNaN.v = F32_P_QNAN;

  // check SNaN
  auto CheckSNaN = [](float32_t x) -> bool
  {
      bool isSNaN = false;

      if (isNaNF32UI(x.v)) { //Check NaN ?
          if (::f32_isSignalingNaN(x)) { //Check SignNaN ?
              isSNaN = true;
              return isSNaN;
          }
      }
      return isSNaN;
  };

// check QNaN
  auto CheckQNaN = [](float32_t x) -> bool
  {
      bool isQNaN = false;

      if (isNaNF32UI(x.v)) { //Check NaN ?
          if (!::f32_isSignalingNaN(x)) { //Check not SignNaN --> QuietNaN
              isQNaN = true;
              return isQNaN;
          }
      }
      return isQNaN;
  };

  // As G4MH rev 140:
  // If reg1 or reg2 is SNaN:
  //   --> 32bit: qNaN = 0x7FFF_FFFF is returned, V flag is on.
  //   --> 64bit: qNaN = 0x7FFF_FFFF_FFFF_FFFF is returned, V flag is on
  // If qNaN is include in both input, returned reg2.
  // If one input is qNaN, the other is Real Number --> Output is the other real number.
  if (CheckSNaN(fr1) == true || CheckSNaN(fr2) == true) {
      this->SetExceptionFlags(FPE_V);
      return qNaN;
  } else if (CheckQNaN(fr2) == true && CheckQNaN(fr1) == true) {
      return fr2;
  } else if (CheckQNaN(fr2) == true) {
      return fr1;
  } else if (CheckQNaN(fr1) == true) {
      return fr2;
  }

  //Check in case Zero
  //Case Min: If reg1 and reg2 is Zero, reg1 is returned. Now, it is used.
  //Case Max: If reg1 and reg2 is Zero, reg2 is returned.
  if (((fr1.v == F32_P_ZERO) && (fr2.v == F32_P_ZERO))
   || ((fr1.v == F32_P_ZERO) && (fr2.v == F32_N_ZERO))
   || ((fr1.v == F32_N_ZERO) && (fr2.v == F32_P_ZERO))
   || ((fr1.v == F32_N_ZERO) && (fr2.v == F32_N_ZERO)) ) {
    return fr1;
  }

  bool res;
  res = false;
  res = ::f32_lt(fr1, fr2);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return (res == true) ? fr1 : fr2;
}


/** Minfd
 * @param[in] fr2
 * @param[in] fr1
 * @return float64_t res.
 * @retval min value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_min(float64_t fr2, float64_t fr1) {
  float64_t qNaN;
  qNaN.v = F64_P_QNAN;

  // check SNaN
  auto CheckSNaN = [](float64_t x) -> bool
  {
      bool isSNaN = false;
      if (isNaNF64UI(x.v)) { //Check NaN ?
          if (::f64_isSignalingNaN(x)) { //Check SignNaN ?
              isSNaN = true;
              return isSNaN;
          }
      }
      return isSNaN;
  };

  // check QNaN
  auto CheckQNaN = [](float64_t x) -> bool
  {
      bool isQNaN = false;
      if (isNaNF64UI(x.v)) { //Check NaN ?
          if (!::f64_isSignalingNaN(x)) { //Check not SignNaN --> QuietNaN
              isQNaN = true;
              return isQNaN;
          }
      }
      return isQNaN;
  };

  // As G4MH rev 140:
  // If reg1 or reg2 is SNaN:
  //   --> 32bit: qNaN = 0x7FFF_FFFF is returned, V flag is on.
  //   --> 64bit: qNaN = 0x7FFF_FFFF_FFFF_FFFF is returned, V flag is on
  // If qNaN is include in both input, returned reg2.
  // If one input is qNaN, the other is Real Number --> Output is the other real number.
  if (CheckSNaN(fr1) == true || CheckSNaN(fr2) == true) {
     this->SetExceptionFlags(FPE_V);
      return qNaN;
  } else if (CheckQNaN(fr2) == true && CheckQNaN(fr1) == true) {
      return fr2;
  } else if (CheckQNaN(fr2) == true) {
      return fr1;
  } else if (CheckQNaN(fr1) == true) {
      return fr2;
  }

  //Check in case Zero
  //Case Min: If reg1 and reg2 is Zero, reg1 is returned. Now, it is used.
  //Case Max: If reg1 and reg2 is Zero, reg2 is returned.
  if (((fr1.v == F64_P_ZERO) && (fr2.v == F64_P_ZERO))
   || ((fr1.v == F64_P_ZERO) && (fr2.v == F64_N_ZERO))
   || ((fr1.v == F64_N_ZERO) && (fr2.v == F64_P_ZERO))
   || ((fr1.v == F64_N_ZERO) && (fr2.v == F64_N_ZERO)) ) {
    return fr1;
  }

  bool res;
  res = false;
  res = ::f64_lt(fr1, fr2);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return (res == true) ? fr1 : fr2;
}


/** Maxfs
 * @param[in] fr2
 * @param[in] fr1
 * @retval max value
 * @todo To be documented.
*/
float32_t SoftFloat::f32_maxfs(float32_t fr2, float32_t fr1) {
  float32_t qNaN;
  qNaN.v = F32_P_QNAN;

  // check SNaN
  auto CheckSNaN = [](float32_t x) -> bool
  {
      bool isSNaN = false;

      if (isNaNF32UI(x.v)) { //Check NaN ?
          if (::f32_isSignalingNaN(x)) { //Check SignNaN ?
              isSNaN = true;
              return isSNaN;
          }
      }
      return isSNaN;
  };

// check QNaN
  auto CheckQNaN = [](float32_t x) -> bool
  {
      bool isQNaN = false;

      if (isNaNF32UI(x.v)) { //Check NaN ?
          if (!::f32_isSignalingNaN(x)) { //Check not SignNaN --> QuietNaN
              isQNaN = true;
              return isQNaN;
          }
      }
      return isQNaN;
  };

  // As G4MH rev 140:
  // If reg1 or reg2 is SNaN:
  //   --> 32bit: qNaN = 0x7FFF_FFFF is returned, V flag is on.
  //   --> 64bit: qNaN = 0x7FFF_FFFF_FFFF_FFFF is returned, V flag is on
  // If qNaN is include in both input, returned reg2.
  // If one input is qNaN, the other is Real Number --> Output is the other real number.
  if (CheckSNaN(fr1) == true || CheckSNaN(fr2) == true) {
      this->SetExceptionFlags(FPE_V);
      return qNaN;
  } else if (CheckQNaN(fr2) == true && CheckQNaN(fr1) == true) {
      return fr2;
  } else if (CheckQNaN(fr2) == true) {
      return fr1;
  } else if (CheckQNaN(fr1) == true) {
      return fr2;
  }

  //Check in case Zero
  //Case Min: If reg1 and reg2 is Zero, reg1 is returned.
  //Case Max: If reg1 and reg2 is Zero, reg2 is returned. Now, it is used.
  if (((fr1.v == F32_P_ZERO) && (fr2.v == F32_P_ZERO))
   || ((fr1.v == F32_P_ZERO) && (fr2.v == F32_N_ZERO))
   || ((fr1.v == F32_N_ZERO) && (fr2.v == F32_P_ZERO))
   || ((fr1.v == F32_N_ZERO) && (fr2.v == F32_N_ZERO)) ) {
    return fr2;
  }

  bool res;
  res = false;
  res = ::f32_lt(fr1, fr2);

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return (res == true) ? fr2 : fr1;
}


/** Maxfd
 * @param[in] fr2
 * @param[in] fr1
 * @return float64_t res.
 * @retval max value
 * @todo To be documented.
 */
float64_t SoftFloat::f64_max (float64_t fr2, float64_t fr1) {
  float64_t qNaN;
  qNaN.v = F64_P_QNAN;

  // check SNaN
  auto CheckSNaN = [](float64_t x) -> bool
  {
      bool isSNaN = false;
      if (isNaNF64UI(x.v)) { //Check NaN ?
          if (::f64_isSignalingNaN(x)) { //Check SignNaN ?
              isSNaN = true;
              return isSNaN;
          }
      }
      return isSNaN;
  };

  // check QNaN
  auto CheckQNaN = [](float64_t x) -> bool
  {
      bool isQNaN = false;
      if (isNaNF64UI(x.v)) { //Check NaN ?
          if (!::f64_isSignalingNaN(x)) { //Check not SignNaN --> QuietNaN
              isQNaN = true;
              return isQNaN;
          }
      }
      return isQNaN;
  };

  // As G4MH rev 140:
  // If reg1 or reg2 is SNaN:
  //   --> 32bit: qNaN = 0x7FFF_FFFF is returned, V flag is on.
  //   --> 64bit: qNaN = 0x7FFF_FFFF_FFFF_FFFF is returned, V flag is on
  // If qNaN is include in both input, returned reg2.
  // If one input is qNaN, the other is Real Number --> Output is the other real number.
  if (CheckSNaN(fr1) == true || CheckSNaN(fr2) == true) {
     this->SetExceptionFlags(FPE_V);
      return qNaN;
  } else if (CheckQNaN(fr2) == true && CheckQNaN(fr1) == true) {
      return fr2;
  } else if (CheckQNaN(fr2) == true) {
      return fr1;
  } else if (CheckQNaN(fr1) == true) {
      return fr2;
  }

  //Check in case Zero
  //Case Min: If reg1 and reg2 is Zero, reg1 is returned.
  //Case Max: If reg1 and reg2 is Zero, reg2 is returned. Now, it is used.
  if (((fr1.v == F64_P_ZERO) && (fr2.v == F64_P_ZERO))
   || ((fr1.v == F64_P_ZERO) && (fr2.v == F64_N_ZERO))
   || ((fr1.v == F64_N_ZERO) && (fr2.v == F64_P_ZERO))
   || ((fr1.v == F64_N_ZERO) && (fr2.v == F64_N_ZERO)) ) {
    return fr2;
  }

  bool res;
  res = false;
  res = ::f64_lt(fr1, fr2);
  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }
    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }
  return (res == true) ? fr2 : fr1;
}


/** Cmpfs
 * @param[in] fr2
 * @param[in] fr1
 * @param[in] fcond
 * @retval cmp result
 * @todo To be documented.
*/
bool SoftFloat::f32_cmpfs(float32_t fr2, float32_t fr1, int32_t fcond) {
// check SNaN
  auto CheckSNaN = [](float32_t x) -> bool {
      bool isSNaN = false;
      if (isNaNF32UI(x.v)) {
        if (::f32_isSignalingNaN(x)) {
          isSNaN = true;
        }
      }
      return isSNaN;
  };

  if ( CheckSNaN(fr2) || CheckSNaN(fr1)) {
      this->SetExceptionFlags(FPE_V);
  }

  bool less;
  bool equal;
  bool unordered;
  if (isNaNF32UI (fr2.v) || isNaNF32UI (fr1.v)) {
      less      = 0;
      equal     = 0;
      unordered = 1;
      if ((fcond & 0x8) != 0) {
         this->SetExceptionFlags(FPE_V);
      }
  } else {
      less      =:: f32_lt(fr1, fr2);
      equal     =:: f32_eq(fr1, fr2);
      unordered = 0;
  }

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  return ( (less      && (fcond & 0x4) != 0) ||
           (equal     && (fcond & 0x2) != 0) ||
           (unordered && (fcond & 0x1) != 0) );
}


/** Cmpfd
 * @param[in] fr2
 * @param[in] fr1
 * @param[in] fcond
 * @return bool res.
 * @retval true/false
 * @todo To be documented.
 */
bool SoftFloat::f64_cmp (float64_t fr2, float64_t fr1, int32_t fcond) {
  // check SNaN
  auto CheckSNaN = [](float64_t x) -> bool {
      bool isSNaN = false;
      if (isNaNF64UI(x.v)) {
        if (::f64_isSignalingNaN(x)) {
          isSNaN = true;
        }
      }
      return isSNaN;
  };

  if ( CheckSNaN(fr2) || CheckSNaN(fr1)) {
      this->SetExceptionFlags(FPE_V);
  }

  bool less;
  bool equal;
  bool unordered;
  if (isNaNF64UI (fr2.v) || isNaNF64UI (fr1.v)) {
      less      = 0;
      equal     = 0;
      unordered = 1;
      if ((fcond & 0x8) != 0) {
         this->SetExceptionFlags(FPE_V);
      }
  } else {
      less      =:: f64_lt(fr1, fr2);
      equal     =:: f64_eq(fr1, fr2);
      unordered = 0;
  }

  {
    uint32_t fpu_excp_flags = this->GetExceptionFlags(); // Prologue

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if (((fpu_excp_flags & FPE_O) != 0) && this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    else if (((fpu_excp_flags & FPE_U) != 0) && this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags &= ~FPE_I;
    }
    // 不正確演算例外(I)の検出条件
    if (fpu_excp_flags & FPE_V) {
      fpu_excp_flags = FPE_V;
    }
    if (((fpu_excp_flags & FPE_O) != 0) && !this->GetOverflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    } else if (((fpu_excp_flags & FPE_U) != 0) && !this->GetUnderflowExceptionEnableFlag()) {
      fpu_excp_flags |= FPE_I;
    }
    if (this->GetInputFlushFlag() && ((fpu_excp_flags & (FPE_V | FPE_Z)) == 0)) {
      fpu_excp_flags |= FPE_I;
    }

    this->SetExceptionFlags(fpu_excp_flags); // Epilogue
  }

  return ( (less      && (fcond & 0x4) != 0) ||
           (equal     && (fcond & 0x2) != 0) ||
           (unordered && (fcond & 0x1) != 0) );
}
