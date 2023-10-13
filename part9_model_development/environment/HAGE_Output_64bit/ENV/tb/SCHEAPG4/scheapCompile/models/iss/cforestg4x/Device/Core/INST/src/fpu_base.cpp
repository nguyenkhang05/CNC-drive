/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

#include "./config.h"
#include "./forest_common.h"
#include "./fpu_base.h"
#include "./sreg.h"


/*!
 * @brief Sets FPSR.IF bit. : FPSR.IFビットの設定
 * @param if_ The data to write. : 書込みデータ
 */
void FpuOpeBase::SetFpsrIF_ (bool if_) const
{
    if (likely (m_use_fxsr == false)) {
        m_fpsr->SetIF_ (if_);
    } else {
        m_fxsr->SetIF_ (if_);
    }
}


/*!
 * @brief Gets FPSR.FN bit. : FPSR.FNビットの取得
 * @param -
 * @return FPSR.FN bit : FPSR.FNビット
 */
bool FpuOpeBase::GetFpsrFN (void) const
{
    if (likely (m_use_fxsr == false)) {
        return m_fpsr->GetFN ();
    } else {
        return m_fxsr->GetFN ();
    }
}


/*!
 * @brief Gets FPSR.FS bit. : FPSR.FSビットの取得
 * @param -
 * @return FPSR.FS bit. : FPSR.FSビット
 */
bool FpuOpeBase::GetFpsrFS (void) const {
    if (likely (m_use_fxsr == false)) {
        return m_fpsr->GetFS ();
    } else {
        return m_fxsr->GetFS ();
    }
}


/*!
 * @brief Gets FPSR.RM bit. : FPSR.RMビットの取得
 * @param -
 * @return FPSR.RM bit. : FPSR.RMビット
 */
uint32_t FpuOpeBase::GetFpsrRM (void) const
{
    if (likely (m_use_fxsr == false)) {
        return m_fpsr->GetRM ();
    } else {
        return m_fxsr->GetRM ();
    }
}


/*!
 * @brief Gets FPSR.XE bit. : FPSR.XEビットの取得
 * @param -
 * @return FPSR.XE bit. : FPSR.XEビット
 */
uint32_t FpuOpeBase::GetFpsrXE (void) const
{
    if (likely (m_use_fxsr == false)) {
        return m_fpsr->GetXE ();
    } else {
        return m_fxsr->GetXE ();
    }
}


/*!
 * @brief Detects Not a Number(NaN). : 非数(NaN)判定
 * @param -
 * @return bool: [true] NaN, [false] Not-NaN.
 */
bool FpuRegSingle::IsNan () const
{
    return (IsSnan () || IsQnan ());
}

/*!
 * @brief Detects Not a Number(NaN). : 非数(NaN)判定
 * @param -
 * @return bool: [true] NaN, [false] Not-NaN.
 */
bool FpuRegDouble::IsNan () const
{
    return (IsSnan () || IsQnan ());
}

/*!
 * @brief Detects Denormal. : デノーマル判定
 * @param -
 * @return bool: [true] Denormal, [false] Not-Denormal.
 */
bool FpuRegSingle::IsDenormal () const
{
    uint32_t exponent = (m_ui & EXPONENT_MASK) >> 23;

    if (exponent == 0) {
        if ((m_ui & MANTISSA_MASK) != 0) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

/*!
 * @brief Detects Denormal. : デノーマル判定
 * @param -
 * @return bool: [true] Denormal, [false] Not-Denormal.
 */
bool FpuRegDouble::IsDenormal () const
{
    uint64_t exponent = (m_ui & EXPONENT_MASK) >> 52;

    if (exponent == 0) {
        if ((m_ui & MANTISSA_MASK) != 0) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

/*!
 * @brief Detects S-NaN(Signaling NaN). : S-NaN判定
 * @param -
 * @return bool: [true] S-NaN, [false] Not S-NaN.
 */
bool FpuRegSingle::IsSnan () const
{
    uint32_t exponent = (m_ui & EXPONENT_MASK) >> 23;

    if (exponent == 0xff && (m_ui & MANTISSA_MASK) != 0x0) {
        return (1 ^ ((m_ui & NAN_MASK) >> 22));
    }
    return false;
}

/*!
 * @brief Detects S-NaN(Signaling NaN). : S-NaN判定
 * @param -
 * @return bool: [true] S-NaN, [false] Not S-NaN.
 */
bool FpuRegDouble::IsSnan () const
{
    uint64_t exponent = (m_ui & EXPONENT_MASK) >> 52;

    if (exponent == 0x7ff && (m_ui & MANTISSA_MASK) != 0x0) {
        return (1 ^ ((m_ui & NAN_MASK) >> 51));
    }
    return false;
}

/*!
 * @brief Detects Q-NaN(Quiet NaN). : Q-NaN判定
 * @param -
 * @return bool: [true] Q-NaN, [false] Not Q-NaN.
 */
bool FpuRegSingle::IsQnan () const
{
    uint32_t exponent = (m_ui & EXPONENT_MASK) >> 23;

    if (exponent == 0xff && (m_ui & MANTISSA_MASK)) {
        return (0 ^ ((m_ui & NAN_MASK) >> 22));
    }
    return false;
}

/*!
 * @brief Detects Q-NaN(Quiet NaN). : Q-NaN判定
 * @param -
 * @return bool: [true] Q-NaN, [false] Not Q-NaN.
 */
bool FpuRegDouble::IsQnan () const
{
    uint64_t exponent = (m_ui & EXPONENT_MASK) >> 52;

    if (exponent == 0x7ff && (m_ui & MANTISSA_MASK)) {
        return (0 ^ ((m_ui & NAN_MASK) >> 51));
    }
    return false;
}

/*!
 * @brief Detects Infinity. : 無限大判定
 * @param -
 * @return bool: [true] Infinity, [false] Not-Infinity. : [true]無限大, [false]無限大ではない
 */
bool FpuRegSingle::IsInf () const
{
    return ((m_ui & 0x7fffffff) == FP_P_INF);
}

/*!
 * @brief Detects Infinity. : 無限大判定
 * @param -
 * @return bool: [true] Infinity, [false] Not-Infinity. : [true]無限大, [false]無限大ではない
 */
bool FpuRegDouble::IsInf () const
{
    return ((m_ui & 0x7fffffffffffffffULL) == FP_P_INF);
}

/*!
 * @brief Detects Zero. : ゼロ判定
 * @param -
 * @return bool: [true] Zero, [false] Not-Zero.
 */
bool FpuRegSingle::IsZero () const
{
    return ((m_ui & 0x7fffffff) == FP_P_ZERO);
}

/*!
 * @brief Detects Zero. : ゼロ判定
 * @param -
 * @return bool: [true] Zero, [false] Not-Zero.
 */
bool FpuRegDouble::IsZero () const
{
    return ((m_ui & 0x7fffffffffffffffULL) == FP_P_ZERO);
}

/*!
 * @brief Detects Sign. : 符号判定
 * @param -
 * @return bool: [true] Positive, [false] Negative.
 */
bool FpuRegSingle::IsPositive () const
{
    return ((m_ui & 0x80000000) == 0x0);
}

/*!
 * @brief Detects Sign. : 符号判定
 * @param -
 * @return bool: [true] Positive, [false] Negative.
 */
bool FpuRegDouble::IsPositive () const
{
    return ((m_ui & 0x8000000000000000ULL) == 0x0);
}


/*!
 * @brief Flushes to Zero. : ゼロフラッシュ
 * @param -
 */
void FpuRegSingle::FlushZeroOperand (void)
{
    if (! IsPositive()) {
        SetInteger (FP_N_ZERO);
    } else {
        SetInteger (FP_P_ZERO);
    }
}

/*!
 * @brief Flushes to Zero. : ゼロフラッシュ
 * @param rounding_mode Rounding mode. : 丸めモード
 * @param fn FPSR.FN
 */
void FpuRegSingle::FlushZeroResult (RoundingMode rounding_mode, bool fn)
{
    uint32_t ui = m_ui & (EXPONENT_MASK | MANTISSA_MASK);

    switch (rounding_mode) {
    case ROUNDING_ROUND:
        if (fn == false) {
            if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
                if (! IsPositive()) {
                    SetInteger (FP_N_ZERO);
                } else {
                    SetInteger (FP_P_ZERO);
                }
            }
        } else {
            if (ui >= FP_MIN_1 && ui < FP_P_MIN) {
                if (! IsPositive()) {
                    SetInteger (FP_N_MIN);
                } else {
                    SetInteger (FP_P_MIN);
                }
            } else if (ui >= FP_P_ZERO && ui < FP_MIN_1) {
                if (! IsPositive()) {
                    SetInteger (FP_N_ZERO);
                } else {
                    SetInteger (FP_P_ZERO);
                }
            }
        }
        break;

    case ROUNDING_TRUNC:
        if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
            if (! IsPositive()) {
                SetInteger (FP_N_ZERO);
            } else {
                SetInteger (FP_P_ZERO);
            }
        }
        break;

    case ROUNDING_CEIL:
        if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
            if (! IsPositive()) {
                SetInteger (FP_N_ZERO);
            } else {
                SetInteger (FP_P_MIN);
            }
        }
        break;

    case ROUNDING_FLOOR:
        if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
            if (! IsPositive()) {
                SetInteger (FP_N_MIN);
            } else {
                SetInteger (FP_P_ZERO);
            }
        }
        break;

    default:
        break;
    }
}


/*!
 * @brief Flushes to Zero. : ゼロフラッシュ
 * @param -
 */
void FpuRegDouble::FlushZeroOperand (void)
{
    if (! IsPositive()) {
        SetInteger (FP_N_ZERO);
    } else {
        SetInteger (FP_P_ZERO);
    }
}


/*!
 * @brief Flushes to Zero. : ゼロフラッシュ
 * @param rounding_mode Rounding mode. : 丸めモード
 * @param fn FPSR.FN
 */
void FpuRegDouble::FlushZeroResult (RoundingMode rounding_mode, bool fn)
{
    uint64_t ui = m_ui & (EXPONENT_MASK | MANTISSA_MASK);

    switch (rounding_mode) {
    case ROUNDING_ROUND:
        if (fn == false) {
            if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
                if (! IsPositive()) {
                    SetInteger (FP_N_ZERO);
                } else {
                    SetInteger (FP_P_ZERO);
                }
            }
        } else {
            if (ui >= FP_MIN_1 && ui < FP_P_MIN) {
                if (! IsPositive()) {
                    SetInteger (FP_N_MIN);
                } else {
                    SetInteger (FP_P_MIN);
                }
            } else if (ui >= FP_P_ZERO && ui < FP_MIN_1) {
                if (! IsPositive()) {
                    SetInteger (FP_N_ZERO);
                } else {
                    SetInteger (FP_P_ZERO);
                }
            }
        }
        break;

    case ROUNDING_TRUNC:
        if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
            if (! IsPositive()) {
                SetInteger (FP_N_ZERO);
            } else {
                SetInteger (FP_P_ZERO);
            }
        }
        break;

    case ROUNDING_CEIL:
        if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
            if (! IsPositive()) {
                SetInteger (FP_N_ZERO);
            } else {
                SetInteger (FP_P_MIN);
            }
        }
        break;

    case ROUNDING_FLOOR:
        if (ui >= FP_P_ZERO && ui < FP_P_MIN) {
            if (! IsPositive()) {
                SetInteger (FP_N_MIN);
            } else {
                SetInteger (FP_P_ZERO);
            }
        }
        break;

    default:
        break;
    }
}


/*!
 * @brief Flushes to Infinity. : 無限大フラッシュ
 * @param rounding_mode Ronding mode. : 丸めモード
 */
void FpuRegSingle::FlushInf (RoundingMode rounding_mode)
{
    switch (rounding_mode) {
    case ROUNDING_ROUND:
        if (! IsPositive()) {
            SetInteger (FP_N_INF);
        } else {
            SetInteger (FP_P_INF);
        }
        break;

    case ROUNDING_TRUNC:
        if (! IsPositive()) {
            SetInteger (FP_N_MAX);
        } else {
            SetInteger (FP_P_MAX);
        }
        break;

    case ROUNDING_CEIL:
        if (! IsPositive()) {
            SetInteger (FP_N_MAX);
        } else {
            SetInteger (FP_P_INF);
        }
        break;

    case ROUNDING_FLOOR:
        if (! IsPositive()) {
            SetInteger (FP_N_INF);
        } else {
            SetInteger (FP_P_MAX);
        }
        break;

    default:
        break;
    }
}

/*!
 * @brief Gets Data Type. : データタイプ取得
 * @param -
 */
uint32_t FpuRegSingle::GetDataType (void) const
{
    int32_t sign;
    int32_t abs;

    sign = m_ui & SIGN_BIT;
    abs = m_ui & ABS_FIELD;
    if (abs < 0x00800000) {
        if (abs == 0x00000000) {
            if (sign == 0) {
                return (P_ZERO);
            } else {
                return (N_ZERO);
            }
        } else {
            return (DENORM);
        }
    } else if ((uint32_t)abs < SEXP_FIELD) {
        return (NORM);
    } else if (abs == SEXP_FIELD) {
        if (sign == 0) {
            return (P_INF);
        } else {
            return (N_INF);
        }
    } else if (abs < 0x7fc00000) {
        return (QNAN);
    } else {
        return (SNAN);
    }
}
