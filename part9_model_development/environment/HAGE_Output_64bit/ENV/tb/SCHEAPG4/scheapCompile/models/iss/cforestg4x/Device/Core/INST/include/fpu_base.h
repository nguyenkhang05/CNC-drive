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

class TraceCtrl;
class SRegFPSR;
class SRegFXSR;

/*! @brief Rounding mode of floating-point operation.
 */
typedef uint32_t RoundingMode;
const RoundingMode ROUNDING_ROUND = 0x0;
const RoundingMode ROUNDING_TRUNC = 0x1;
const RoundingMode ROUNDING_CEIL  = 0x2;
const RoundingMode ROUNDING_FLOOR = 0x3;

const uint32_t NORM   = 0x0;
const uint32_t DENORM = 0x1;
const uint32_t P_ZERO = 0x2;
const uint32_t N_ZERO = 0x3;
const uint32_t P_INF  = 0x4;
const uint32_t N_INF  = 0x5;
const uint32_t SNAN   = 0x6;
const uint32_t QNAN   = 0x7;

/*! @brief Floating-point unit operation.
 */
class FpuOpeBase {
public:
    FpuOpeBase (TraceCtrl* trace, SRegFPSR* fpsr) {
        m_trace = trace;
        m_fpsr = fpsr;
        m_use_fxsr = false;
    }

    FpuOpeBase (TraceCtrl* trace, SRegFXSR* fxsr) {
        m_trace = trace;
        m_fxsr = fxsr;
        m_use_fxsr = true;
    }

    static const uint32_t FPE_NONE = 0x0;
    static const uint32_t FPE_I    = 0x1;  //! Inexact.  : 不正確演算(Inexact)
    static const uint32_t FPE_U    = 0x2;  //! Underflow. : アンダーフロー(Underflow)
    static const uint32_t FPE_O    = 0x4;  //! Overflow. : オーバーフロー(Overflow)
    static const uint32_t FPE_Z    = 0x8;  //! Devision by zero. : ゼロ除算(divbyZero)
    static const uint32_t FPE_V    = 0x10; //! Invalid. : 無効演算(inValid)
    static const uint32_t FPE_E    = 0x20; //! Not supported. : 未実装演算

    void SetTraceCtrl (TraceCtrl* trace) { m_trace = trace; }

protected:
    TraceCtrl* m_trace;
    bool m_use_fxsr;

    void SetFpsrIF_ (bool if_) const;
    bool GetFpsrFN (void) const;
    bool GetFpsrFS (void) const;
    uint32_t GetFpsrRM (void) const;
    uint32_t GetFpsrXE (void) const;

private:
    mutable SRegFPSR* m_fpsr;
    mutable SRegFXSR* m_fxsr;
};


class FpuRegHalf {
private:
    uint16_t m_ui;

public:
    static const uint16_t FP_N_ZERO = 0x8000U;
    static const uint16_t FP_P_ZERO = 0x0000U;
    static const uint16_t FP_N_INF  = 0xfc00U;
    static const uint16_t FP_P_INF  = 0x7c00U;
    static const uint16_t FP_N_MIN  = 0x8400U;
    static const uint16_t FP_P_MIN  = 0x0400U;
    static const uint16_t FP_N_MAX  = 0xfbffU;
    static const uint16_t FP_P_MAX  = 0x7bffU;
    static const uint16_t FP_QNAN   = 0x7fffU;
    static const uint32_t FP_MIN_1  = 0x0200U;

    static const uint16_t EXPONENT_MASK   = 0x7c00U;
    static const uint16_t MANTISSA_MASK   = 0x03ffU;

    FpuRegHalf () {}
    explicit FpuRegHalf (uint16_t i) { m_ui = i; }
    void SetInteger (uint16_t i) { m_ui = i; }
    uint16_t GetInteger () const { return m_ui; }
    bool IsNan () const;
    bool IsDenormal () const;
    bool IsSnan () const;
    bool IsQnan () const;
    bool IsInf () const;
    bool IsZero () const;
    bool IsPositive () const;
    void FlushZeroResult (RoundingMode rounding_mode, bool fn);
    void FlushZero (RoundingMode rounding_mode);
    void FlushInf (RoundingMode rounding_mode);
};


class FpuRegSingle {
private:
    union {
        float m_f;
        uint32_t m_ui;
    };

public:
    static const uint32_t FP_N_ZERO = 0x80000000UL;
    static const uint32_t FP_P_ZERO = 0x00000000UL;
    static const uint32_t FP_N_INF  = 0xff800000UL;
    static const uint32_t FP_P_INF  = 0x7f800000UL;
    static const uint32_t FP_N_MIN  = 0x80800000UL;
    static const uint32_t FP_P_MIN  = 0x00800000UL;
    static const uint32_t FP_N_MAX  = 0xff7fffffUL;
    static const uint32_t FP_P_MAX  = 0x7f7fffffUL;
    static const uint32_t FP_QNAN   = 0x7fffffffUL;
    static const uint32_t FP_MIN_1  = 0x00400000UL;

    static const uint32_t EXPONENT_MASK = 0x7f800000UL;
    static const uint32_t MANTISSA_MASK = 0x007fffffUL;
    static const uint32_t NAN_MASK      = 0x00400000UL;

    static const uint32_t SIGN_BIT   = 0x80000000UL;
    static const uint32_t ABS_FIELD  = 0x7fffffffUL;
    static const uint32_t SEXP_FIELD = 0x7f800000UL;

    FpuRegSingle () {}
    explicit FpuRegSingle (uint32_t i) { m_ui = i; }
    void SetInteger (uint32_t i) { m_ui = i; }
    uint32_t GetInteger () const { return m_ui; }
    void SetFloat (float f) { m_f = f; }
    float GetFloat () const { return m_f; }
    bool IsNan () const;
    bool IsDenormal () const;
    bool IsSnan () const;
    bool IsQnan () const;
    bool IsInf () const;
    bool IsZero () const;
    bool IsPositive () const;
    void FlushZeroOperand (void);
    void FlushZeroResult (RoundingMode rounding_mode, bool fn);
    void FlushInf (RoundingMode rounding_mode);
    uint32_t GetDataType () const;
};


class FpuRegDouble {
private:
    union {
        double m_f;
        uint64_t m_ui;
    };

public:
    static const uint64_t FP_N_ZERO = 0x8000000000000000ULL;
    static const uint64_t FP_P_ZERO = 0x0000000000000000ULL;
    static const uint64_t FP_N_INF  = 0xfff0000000000000ULL;
    static const uint64_t FP_P_INF  = 0x7ff0000000000000ULL;
    static const uint64_t FP_N_MIN  = 0x8010000000000000ULL;
    static const uint64_t FP_P_MIN  = 0x0010000000000000ULL;
    static const uint64_t FP_N_MAX  = 0xffefffffffffffffULL;
    static const uint64_t FP_P_MAX  = 0x7fefffffffffffffULL;
    static const uint64_t FP_QNAN   = 0x7fffffffffffffffULL;
    static const uint64_t FP_MIN_1  = 0x0008000000000000ULL;

    static const uint64_t EXPONENT_MASK = 0x7ff0000000000000ULL;
    static const uint64_t MANTISSA_MASK = 0x000fffffffffffffULL;
    static const uint64_t NAN_MASK      = 0x0008000000000000ULL;

    FpuRegDouble () {}
    explicit FpuRegDouble (uint64_t i) { m_ui = i; }
    void SetInteger (uint64_t i) { m_ui = i; }
    uint64_t GetInteger () const { return m_ui; }
    void SetFloat (double f) { m_f = f; }
    double GetFloat () const { return m_f; }
    bool IsNan () const;
    bool IsDenormal () const;
    bool IsSnan () const;
    bool IsQnan () const;
    bool IsInf () const;
    bool IsZero () const;
    bool IsPositive () const;
    void FlushZeroOperand (void);
    void FlushZeroResult (RoundingMode rounding_mode, bool fn);
    void FlushInf (RoundingMode rounding_mode) { }
};
