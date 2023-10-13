/**
 * @file SoftFloatInterface.h
 *
 * @date Created on: 2016
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UCB SoflotFloat Interface class
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#pragma once

class SoftFloatIF {
public:
  SoftFloatIF() {}
  virtual ~SoftFloatIF(){};

  // RoundingMode
  virtual void SetRoundingMode(uint32_t) = 0;
  virtual uint32_t GetRoundingMode() = 0;
  // ExceptionFlags
  virtual void SetExceptionFlags(uint32_t) = 0;
  virtual uint32_t GetExceptionFlags() = 0;
  // FPSR/FXSR.FS
  virtual void SetFlushEnable(bool) = 0;
  virtual bool GetFlushEnable() = 0;
  // FPSR/FXSR.FN
  virtual void SetFlushNearestEnable(bool) = 0;
  // FPSR?/FXSR.IF
  virtual void SetInputFlushFlag(bool) = 0;
  virtual bool GetInputFlushFlag() = 0;
  // FPSR/FXSR.XE_O
  virtual void SetOverflowExceptionEnableFlag(bool) = 0;
  virtual bool GetOverflowExceptionEnableFlag() = 0;
  // FPSR/FXSR.XE_U
  virtual void SetUnderflowExceptionEnableFlag(bool) = 0;
  virtual bool GetUnderflowExceptionEnableFlag() = 0;
  // Check Output Subnormal
  virtual float32_t CheckOutputSubnormal32(float32_t) = 0;
  virtual float64_t CheckOutputSubnormal64(float64_t) = 0;
   virtual float16_t CheckOutputSubnormal16(float16_t) = 0;

  // Wrapper functions
  virtual float32_t f32_mulAdd(float32_t, float32_t, float32_t) = 0;
  virtual float32_t f32_mulSub(float32_t, float32_t, float32_t) = 0;
  virtual float32_t f32_Mneg  (float32_t) = 0;

  virtual int32_t   Cvt_f32_i32  (float32_t) = 0;
  virtual uint32_t  Cvt_f32_ui32 (float32_t) = 0;
  virtual int64_t   Cvt_f32_i64  (float32_t) = 0;
  virtual uint64_t  Cvt_f32_ui64 (float32_t) = 0;
  virtual int32_t   Cvt_f64_i32  (float64_t) = 0;
  virtual uint32_t  Cvt_f64_ui32 (float64_t) = 0;
  virtual int64_t   Cvt_f64_i64  (float64_t) = 0;
  virtual uint64_t  Cvt_f64_ui64 (float64_t) = 0;
  virtual float32_t Cvt_i32_f32  (int32_t)   = 0;
  virtual float32_t Cvt_ui32_f32 (uint32_t)  = 0;
  virtual float32_t Cvt_i64_f32  (int64_t)   = 0;
  virtual float32_t Cvt_ui64_f32 (uint64_t)  = 0;
  virtual float64_t Cvt_i32_f64  (int32_t)   = 0;
  virtual float64_t Cvt_ui32_f64 (uint32_t)  = 0;
  virtual float64_t Cvt_i64_f64  (int64_t)   = 0;
  virtual float64_t Cvt_ui64_f64 (uint64_t)  = 0;
  virtual float32_t f64_to_f32   (float64_t) = 0;
  virtual float64_t f32_to_f64   (float32_t) = 0;
  virtual float16_t f32_to_f16   (float32_t) = 0;
  virtual float32_t f16_to_f32   (float16_t) = 0;

  virtual float32_t f32_absfs  (float32_t) = 0;
  virtual float64_t f64_absfd  (float64_t) = 0;
  virtual float32_t f32_negfs  (float32_t) = 0;
  virtual float64_t f64_negfd  (float64_t) = 0;
  virtual float32_t f32_sqrtfs (float32_t) = 0;
  virtual float64_t f64_sqrtfd (float64_t) = 0;
  virtual float32_t f32_rsqrt  (float32_t) = 0;
  virtual float64_t f64_rsqrt  (float64_t) = 0;
  virtual float32_t f32_recipfs(float32_t) = 0;
  virtual float64_t f64_recipfd(float64_t) = 0;
  virtual float32_t f32_minfs  (float32_t, float32_t) = 0;
  virtual float64_t f64_min    (float64_t, float64_t) = 0;
  virtual float32_t f32_maxfs  (float32_t, float32_t) = 0;
  virtual float64_t f64_max    (float64_t, float64_t) = 0;
  virtual float32_t f32_addfs  (float32_t, float32_t) = 0;
  virtual float64_t f64_addfd  (float64_t, float64_t) = 0;
  virtual float32_t f32_subfs  (float32_t, float32_t) = 0;
  virtual float64_t f64_subfd  (float64_t, float64_t) = 0;
  virtual float32_t f32_mulfs  (float32_t, float32_t) = 0;
  virtual float64_t f64_mulfd  (float64_t, float64_t) = 0;
  virtual float32_t f32_divfs  (float32_t, float32_t) = 0;
  virtual float64_t f64_divfd  (float64_t, float64_t) = 0;
  virtual bool      f32_cmpfs  (float32_t, float32_t, int32_t) = 0;
  virtual bool      f64_cmp    (float64_t, float64_t, int32_t) = 0;
};
