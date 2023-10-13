/**
 * @file mock_SoftFloatAdapter.h
 *
 * @date Created on: 2017
 * @author Author: Hiroyuki Ono <hiroyuki.ono.jc@renesas.com>
 * @brief UCB SoflotFloat Adapter mock class
 * @par Copyright: 2017 Renesas Electronics Corp.
 */

#pragma once
#include "gmock/gmock.h"
#include "SoftFloatAdapter.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Invoke;

class MockSoftFloat : public SoftFloat {
public:
  MockSoftFloat() {
    ON_CALL(*this, SetRoundingMode(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::SetRoundingMode));
    ON_CALL(*this, GetRoundingMode())
      .WillByDefault(Invoke(&real_, &SoftFloat::GetRoundingMode));
    ON_CALL(*this, SetExceptionFlags(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::SetExceptionFlags));
    ON_CALL(*this, GetExceptionFlags())
      .WillByDefault(Invoke(&real_, &SoftFloat::GetExceptionFlags));
    ON_CALL(*this, SetFlushEnable(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::SetFlushEnable));
    ON_CALL(*this, GetFlushEnable())
      .WillByDefault(Invoke(&real_, &SoftFloat::GetFlushEnable));
    ON_CALL(*this, SetFlushNearestEnable(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::SetFlushNearestEnable));
    ON_CALL(*this, SetInputFlushFlag(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::SetInputFlushFlag));
    ON_CALL(*this, GetInputFlushFlag())
      .WillByDefault(Invoke(&real_, &SoftFloat::GetInputFlushFlag));
    ON_CALL(*this, SetOverflowExceptionEnableFlag(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::SetOverflowExceptionEnableFlag));
    ON_CALL(*this, SetUnderflowExceptionEnableFlag(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::SetUnderflowExceptionEnableFlag));
    ON_CALL(*this, f32_mulAdd(_,_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_mulAdd));
    ON_CALL(*this, f32_mulSub(_,_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_mulSub));
    ON_CALL(*this, Cvt_f32_i32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f32_i32));
    ON_CALL(*this, Cvt_f32_ui32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f32_ui32));
    ON_CALL(*this, Cvt_f32_i64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f32_i64));
    ON_CALL(*this, Cvt_f32_ui64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f32_ui64));
    ON_CALL(*this, Cvt_f64_i32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f64_i32));
    ON_CALL(*this, Cvt_f64_ui32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f64_ui32));
    ON_CALL(*this, Cvt_f64_i64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f64_i64));
    ON_CALL(*this, Cvt_f64_ui64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_f64_ui64));
    ON_CALL(*this, Cvt_i32_f32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_i32_f32));
    ON_CALL(*this, Cvt_ui32_f32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_ui32_f32));
    ON_CALL(*this, Cvt_i64_f32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_i64_f32));
    ON_CALL(*this, Cvt_ui64_f32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_ui64_f32));
    ON_CALL(*this, Cvt_i32_f64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_i32_f64));
    ON_CALL(*this, Cvt_ui32_f64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_ui32_f64));
    ON_CALL(*this, Cvt_i64_f64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_i64_f64));
    ON_CALL(*this, Cvt_ui64_f64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::Cvt_ui64_f64));
    ON_CALL(*this, f64_to_f32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_to_f32));
    ON_CALL(*this, f32_to_f64(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_to_f64));
    ON_CALL(*this, f32_to_f16(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_to_f16));
    ON_CALL(*this, f16_to_f32(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f16_to_f32));
    ON_CALL(*this, f32_rsqrt(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_rsqrt));
    ON_CALL(*this, f64_rsqrt(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_rsqrt));
    ON_CALL(*this, f32_minfs(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_minfs));
    ON_CALL(*this, f64_min(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_min));
    ON_CALL(*this, f32_maxfs(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_maxfs));
    ON_CALL(*this, f64_max(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_max));
    ON_CALL(*this, f32_recipfs(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_recipfs));
    ON_CALL(*this, f64_recipfd(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_recipfd));
    ON_CALL(*this, f32_divfs(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_divfs));
    ON_CALL(*this, f64_divfd(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_divfd));
    ON_CALL(*this, f32_cmpfs(_,_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_cmpfs));
    ON_CALL(*this, f64_cmp(_,_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_cmp));
    ON_CALL(*this, f32_sqrtfs(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_sqrtfs));
    ON_CALL(*this, f64_sqrtfd(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_sqrtfd));
    ON_CALL(*this, f32_absfs(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_absfs));
    ON_CALL(*this, f64_absfd(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_absfd));
    ON_CALL(*this, f32_negfs(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_negfs));
    ON_CALL(*this, f64_negfd(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_negfd));
    ON_CALL(*this, f32_addfs(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_addfs));
    ON_CALL(*this, f64_addfd(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_addfd));
    ON_CALL(*this, f32_subfs(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_subfs));
    ON_CALL(*this, f64_subfd(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_subfd));
    ON_CALL(*this, f32_mulfs(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_mulfs));
    ON_CALL(*this, f64_mulfd(_,_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f64_mulfd));
    ON_CALL(*this, f32_Mneg(_))
      .WillByDefault(Invoke(&real_, &SoftFloat::f32_Mneg));
  }
  MOCK_METHOD1(SetRoundingMode, void(uint32_t));
  MOCK_METHOD0(GetRoundingMode, uint32_t());
  MOCK_METHOD1(SetExceptionFlags, void(uint32_t));
  MOCK_METHOD0(GetExceptionFlags, uint32_t());
  MOCK_METHOD1(SetFlushEnable, void(bool));
  MOCK_METHOD0(GetFlushEnable, bool());
  MOCK_METHOD1(SetFlushNearestEnable, void(bool));
  MOCK_METHOD1(SetInputFlushFlag, void(bool));
  MOCK_METHOD0(GetInputFlushFlag, bool());
  MOCK_METHOD1(SetOverflowExceptionEnableFlag, void(bool));
  MOCK_METHOD1(SetUnderflowExceptionEnableFlag, void(bool));


  MOCK_METHOD3(f32_mulAdd, float32_t(float32_t, float32_t, float32_t));
  MOCK_METHOD3(f32_mulSub, float32_t(float32_t, float32_t, float32_t));
  MOCK_METHOD1(f32_Mneg, float32_t(float32_t));
  // Convert from float to integer
  MOCK_METHOD1(Cvt_f32_i32,  int32_t  (float32_t));
  MOCK_METHOD1(Cvt_f32_ui32, uint32_t (float32_t));
  MOCK_METHOD1(Cvt_f32_i64,  int64_t  (float32_t));
  MOCK_METHOD1(Cvt_f32_ui64, uint64_t (float32_t));
  MOCK_METHOD1(Cvt_f64_i32,  int32_t  (float64_t));
  MOCK_METHOD1(Cvt_f64_ui32, uint32_t (float64_t));
  MOCK_METHOD1(Cvt_f64_i64,  int64_t  (float64_t));
  MOCK_METHOD1(Cvt_f64_ui64, uint64_t (float64_t));
  // Convert from integer to float
  MOCK_METHOD1(Cvt_i32_f32, float32_t  (int32_t));
  MOCK_METHOD1(Cvt_ui32_f32, float32_t (uint32_t));
  MOCK_METHOD1(Cvt_i64_f32, float32_t  (int64_t));
  MOCK_METHOD1(Cvt_ui64_f32, float32_t (uint64_t));
  MOCK_METHOD1(Cvt_i32_f64, float64_t  (int32_t));
  MOCK_METHOD1(Cvt_ui32_f64, float64_t (uint32_t));
  MOCK_METHOD1(Cvt_i64_f64, float64_t  (int64_t));
  MOCK_METHOD1(Cvt_ui64_f64, float64_t (uint64_t));
  // Convert from float to float
  MOCK_METHOD1(f64_to_f32,float32_t(float64_t));
  MOCK_METHOD1(f32_to_f64, float64_t(float32_t));
  MOCK_METHOD1(f32_to_f16,float16_t(float32_t));
  MOCK_METHOD1(f16_to_f32, float32_t(float16_t ));
  // Arithmetic operation
  MOCK_METHOD1(f32_absfs,   float32_t (float32_t));
  MOCK_METHOD1(f64_absfd,   float64_t (float64_t));
  MOCK_METHOD1(f32_negfs,   float32_t (float32_t));
  MOCK_METHOD1(f64_negfd,   float64_t (float64_t));
  MOCK_METHOD1(f32_sqrtfs,  float32_t (float32_t));
  MOCK_METHOD1(f64_sqrtfd,  float64_t (float64_t));
  MOCK_METHOD1(f32_rsqrt,   float32_t (float32_t));
  MOCK_METHOD1(f64_rsqrt,   float64_t (float64_t));
  MOCK_METHOD1(f32_recipfs, float32_t (float32_t));
  MOCK_METHOD1(f64_recipfd, float64_t (float64_t));
  MOCK_METHOD2(f32_minfs, float32_t(float32_t, float32_t));
  MOCK_METHOD2(f64_min,   float64_t(float64_t, float64_t));
  MOCK_METHOD2(f32_maxfs, float32_t(float32_t, float32_t));
  MOCK_METHOD2(f64_max,   float64_t(float64_t, float64_t));
  MOCK_METHOD2(f32_addfs, float32_t(float32_t, float32_t));
  MOCK_METHOD2(f64_addfd, float64_t(float64_t, float64_t));
  MOCK_METHOD2(f32_subfs, float32_t(float32_t, float32_t));
  MOCK_METHOD2(f64_subfd, float64_t(float64_t, float64_t));
  MOCK_METHOD2(f32_mulfs, float32_t(float32_t, float32_t));
  MOCK_METHOD2(f64_mulfd, float64_t(float64_t, float64_t));
  MOCK_METHOD2(f32_divfs, float32_t(float32_t, float32_t));
  MOCK_METHOD2(f64_divfd, float64_t(float64_t, float64_t));
  MOCK_METHOD3(f32_cmpfs, bool(float32_t, float32_t, int32_t));
  MOCK_METHOD3(f64_cmp,   bool(float64_t, float64_t, int32_t));

private:
  SoftFloat real_;
};
