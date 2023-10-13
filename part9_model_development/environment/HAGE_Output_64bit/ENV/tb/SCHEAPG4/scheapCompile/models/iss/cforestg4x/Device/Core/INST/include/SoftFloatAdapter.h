/**
 * @file SoftFloatAdapter.h
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UCB SoflotFloat Adapter class
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#pragma once
extern "C" {
#include "softfloat.h"
extern bool softfloat_fs;
extern bool softfloat_fn;
}
#include "SoftFloatInterface.h"

class SoftFloat : public SoftFloatIF {
public:
  static const uint32_t FPE_NONE = 0x00;
  static const uint32_t FPE_I = 0x01; //不正確演算(Inexact)
  static const uint32_t FPE_U = 0x02; //アンダーフロー(Underflow)
  static const uint32_t FPE_O = 0x04; //オーバーフロー(Overflow)
  static const uint32_t FPE_Z = 0x08; //ゼロ除算(divbyZero)
  static const uint32_t FPE_V = 0x10; //無効演算(inValid)
  static const uint32_t FPE_E = 0x20; //未実装演算
  // Float 16 values
  static const uint16_t F16_N_ZERO = 0x8000U;
  static const uint16_t F16_P_ZERO = 0x0000U;
  static const uint16_t F16_P_QNAN = 0x7FFFU;
  static const uint16_t F16_N_QNAN = 0xFFFFU;
  static const uint16_t F16_N_MIN  = 0x8400U;
  static const uint16_t F16_P_MIN  = 0x0400U;
  // Float 32 values
  static const uint32_t F32_N_ZERO = 0x80000000UL;
  static const uint32_t F32_P_ZERO = 0x00000000UL;
  static const uint32_t F32_N_MIN  = 0x80800000UL;
  static const uint32_t F32_P_MIN  = 0x00800000UL;
  static const uint32_t F32_P_INF  = 0x7F800000UL;
  static const uint32_t F32_N_INF  = 0xFF800000UL;
  static const uint32_t F32_P_SNAN = 0x7FBFFFFFUL;
  static const uint32_t F32_N_SNAN = 0xFFBFFFFFUL;
  static const uint32_t F32_P_QNAN = 0x7FFFFFFFUL;
  static const uint32_t F32_N_QNAN = 0xFFFFFFFFUL;
  // Float 64 values
  static const uint64_t F64_N_ZERO = 0x8000000000000000ULL;
  static const uint64_t F64_P_ZERO = 0x0000000000000000ULL;
  static const uint64_t F64_N_INF  = 0xFFF0000000000000ULL;
  static const uint64_t F64_P_INF  = 0x7FF0000000000000ULL;
  static const uint64_t F64_N_SNAN = 0xFFF7FFFFFFFFFFFFULL;
  static const uint64_t F64_P_SNAN = 0x7FF7FFFFFFFFFFFFULL;
  static const uint64_t F64_N_QNAN = 0xFFFFFFFFFFFFFFFFULL;
  static const uint64_t F64_P_QNAN = 0x7FFFFFFFFFFFFFFFULL;
  static const uint64_t F64_N_MIN  = 0x8010000000000000ULL;
  static const uint64_t F64_P_MIN  = 0x0010000000000000ULL;
  static const uint64_t F64_N_MAX  = 0xFFEFFFFFFFFFFFFFULL;
  static const uint64_t F64_P_MAX  = 0x7FEFFFFFFFFFFFFFULL;
  // Integer 16 values
  static const uint16_t I16_P_MAX  = 0x7FFFU;
  static const uint16_t I16_N_MAX  = 0x8000U;
  static const uint16_t UI16_P_MAX  = 0xFFFFU;
  static const uint16_t UI16_N_MAX  = 0x0000U;
  // Integer 32 values
  static const uint32_t I32_P_MAX  = 0x7FFFFFFFUL;
  static const uint32_t I32_N_MAX  = 0x80000000UL;
  static const uint32_t UI32_P_MAX  = 0xFFFFFFFFUL;
  static const uint32_t UI32_N_MAX  = 0x00000000UL;
  // Integer 64 values
  static const uint64_t I64_P_MAX  = 0x7FFFFFFFFFFFFFFFULL;
  static const uint64_t I64_N_MAX  = 0x8000000000000000ULL;
  static const uint64_t UI64_P_MAX = 0xFFFFFFFFFFFFFFFFULL;
  static const uint64_t UI64_N_MAX = 0x0000000000000000ULL;
  // F16 configuration
  static const uint16_t F16_MASK      = 0xFFFFU;
  static const uint16_t F16_EXP_MASK  = 0x7C00U;
  static const uint16_t F16_EXP_SET   = 0x7C00U;
  static const uint16_t F16_SIGN_MASK = 0x8000U;
  static const uint16_t F16_SIGN_SET  = 0x8000U;
  // F32 configuration
  static const uint32_t F32_MASK      = 0xFFFFFFFFUL;
  static const uint32_t F32_EXP_MASK  = 0x7F800000UL;
  static const uint32_t F32_EXP_SET   = 0x7F800000UL;
  static const uint32_t F32_SIGN_MASK = 0x80000000UL;
  static const uint32_t F32_SIGN_SET  = 0x80000000UL;
  // F64 configuration
  static const uint64_t F64_MASK      = 0xFFFFFFFFFFFFFFFFULL;
  static const uint64_t F64_EXP_MASK  = 0x7FF0000000000000ULL;
  static const uint64_t F64_EXP_SET   = 0x7FF0000000000000ULL;
  static const uint64_t F64_SIGN_MASK = 0x8000000000000000ULL;
  static const uint64_t F64_SIGN_SET  = 0x8000000000000000ULL;
  // Range of exponent
  static const int_fast16_t F32_I32_EXP   = 0x009EUL; // exp_val = 127 bit in F32 + 31 bit in I32
  static const int_fast16_t F32_I64_EXP   = 0x00BEUL; // exp_val = 127 bit in F32 + 63 bit in I64
  static const int_fast16_t F64_I32_EXP   = 0x041EUL; // exp_val = 1023 bit in F64 + 31 bit in I32
  static const int_fast16_t F64_I64_EXP   = 0x043EUL; // exp_val = 1023 bit in F64 + 63 bit in I64

private:
  bool m_fpsr_if;
  bool m_fpsr_xe_o;
  bool m_fpsr_xe_u;

#if defined(_UNIT_TEST)
public:
#endif
  enum {
    // round to nearest, with ties to even
    RM_softfloat_round_near_even = 0b00,
    // round to nearest, with ties to maximum magnitude (away from zero)
    // RM_softfloat_round_near_maxMag = 0bxx,
    // round to minimum magnitude (toward zero)
    RM_softfloat_round_minMag = 0b01,
    // round to minimum (down)
    RM_softfloat_round_min = 0b11,
    // round to maximum
    RM_softfloat_round_max = 0b10,
  };

public:
SoftFloat() : m_fpsr_if(false), m_fpsr_xe_o(false), m_fpsr_xe_u(false){};
  ~SoftFloat(){};

  // RoundingMode
  void SetRoundingMode(uint32_t);
  uint32_t GetRoundingMode();
  // ExceptionFlags
  void SetExceptionFlags(uint32_t);
  uint32_t GetExceptionFlags();
  // FPSR/FXSR.FS
  void SetFlushEnable(bool is_flush);
  bool GetFlushEnable();
  // FPSR/FXSR.FN
  void SetFlushNearestEnable(bool is_flush_nearest);
  // FPSR?/FXSR.IF
  void SetInputFlushFlag(bool is_flush);
  bool GetInputFlushFlag();
  // FPSR/FXSR.XE_O
  void SetOverflowExceptionEnableFlag(bool);
  bool GetOverflowExceptionEnableFlag();
  // FPSR/FXSR.XE_U
  void SetUnderflowExceptionEnableFlag(bool);
  bool GetUnderflowExceptionEnableFlag();

  // Check Output Subnormal
  float32_t CheckOutputSubnormal32(float32_t);
  float64_t CheckOutputSubnormal64(float64_t);
  float16_t CheckOutputSubnormal16(float16_t);

  // wrapper functions
  float32_t f32_mulAdd(float32_t, float32_t, float32_t);
  float32_t f32_mulSub(float32_t, float32_t, float32_t);
  float32_t f32_Mneg(float32_t);

  // Conversion from float type to integer type
  int32_t   Cvt_f32_i32  (float32_t);
  uint32_t  Cvt_f32_ui32 (float32_t);
  int64_t   Cvt_f32_i64  (float32_t);
  uint64_t  Cvt_f32_ui64 (float32_t);
  int32_t   Cvt_f64_i32  (float64_t);
  uint32_t  Cvt_f64_ui32 (float64_t);
  int64_t   Cvt_f64_i64  (float64_t);
  uint64_t  Cvt_f64_ui64 (float64_t);
  // Conversion from integer type to float type
  float32_t Cvt_i32_f32  (int32_t);
  float32_t Cvt_ui32_f32 (uint32_t);
  float32_t Cvt_i64_f32  (int64_t);
  float32_t Cvt_ui64_f32 (uint64_t);
  float64_t Cvt_i32_f64  (int32_t);
  float64_t Cvt_ui32_f64 (uint32_t);
  float64_t Cvt_i64_f64  (int64_t);
  float64_t Cvt_ui64_f64 (uint64_t);
  // Conversion from float type to float type
  float32_t f64_to_f32   (float64_t);
  float64_t f32_to_f64   (float32_t );
  float16_t f32_to_f16   (float32_t);
  float32_t f16_to_f32   (float16_t );
  // Arithmetic operation
  float32_t f32_absfs   (float32_t);
  float64_t f64_absfd   (float64_t);
  float32_t f32_negfs   (float32_t);
  float64_t f64_negfd   (float64_t);
  float32_t f32_sqrtfs  (float32_t);
  float64_t f64_sqrtfd  (float64_t);
  float32_t f32_rsqrt   (float32_t);
  float64_t f64_rsqrt   (float64_t);
  float32_t f32_recipfs (float32_t);
  float64_t f64_recipfd (float64_t);
  float32_t f32_minfs   (float32_t, float32_t);
  float64_t f64_min     (float64_t, float64_t);
  float32_t f32_maxfs   (float32_t, float32_t);
  float64_t f64_max     (float64_t, float64_t);
  float32_t f32_addfs   (float32_t, float32_t);
  float64_t f64_addfd   (float64_t, float64_t);
  float32_t f32_subfs   (float32_t, float32_t);
  float64_t f64_subfd   (float64_t, float64_t);
  float32_t f32_mulfs   (float32_t, float32_t);
  float64_t f64_mulfd   (float64_t, float64_t);
  float32_t f32_divfs   (float32_t, float32_t);
  float64_t f64_divfd   (float64_t, float64_t);
  bool f32_cmpfs        (float32_t, float32_t, int32_t);
  bool f64_cmp          (float64_t, float64_t, int32_t);
};
