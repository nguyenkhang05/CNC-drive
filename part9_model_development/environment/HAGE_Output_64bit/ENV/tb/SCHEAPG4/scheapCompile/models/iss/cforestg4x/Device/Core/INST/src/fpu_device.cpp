/*
 * (c) 2011,2012,2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

// FPU for SoftFloat library. : SoftFloatライブラリ使用版 FPU

#include <limits>
#include "./config.h"
#include "./forest_common.h"
#include "./sregfile.h"
#include "./fpu_device.h"

#include "SoftFloatAdapter.h"

extern uint32_t float_exception_flags;

/*!
 * @brief Adds floating-point number. : 浮動小数点加算
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Add (const FpuRegDouble &a, const FpuRegDouble &b,
                                uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());
    FpuRegDouble freg2 (b.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_addfd(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Add (const FpuRegSingle &a, const FpuRegSingle &b,
                                uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_addfs(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}


/*!
 * @brief Subtracts floating-point number. : 浮動小数点減算
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Sub (const FpuRegDouble &a, const FpuRegDouble &b,
                                uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());
    FpuRegDouble freg2 (b.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_subfd(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Sub (const FpuRegSingle &a, const FpuRegSingle &b,
                                uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_subfs(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

/*!
 * @brief Multiplicates floating-point number. : 浮動小数点乗算
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Mul (const FpuRegDouble &a, const FpuRegDouble &b,
                                uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());
    FpuRegDouble freg2 (b.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_mulfd(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Mul (const FpuRegSingle &a, const FpuRegSingle &b,
                                uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_mulfs(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

/*!
 * @brief Devides floating-point number. : 浮動小数点除算
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Div (const FpuRegDouble &a, const FpuRegDouble &b, uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());
    FpuRegDouble freg2 (b.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_divfd(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Div (const FpuRegSingle &a, const FpuRegSingle &b, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3b)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_divfs(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }
    return fregr;
}

/*!
 * @brief Gets the absolute value of the floating-point number. : 浮動小数点絶対値
 * @param a Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegSingle FpuOpeDevice::Abs (const FpuRegSingle &a,
                                uint32_t* exp_code) const
{
    FpuRegSingle freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_absfs(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}


FpuRegDouble FpuOpeDevice::Abs (const FpuRegDouble &a,
                                uint32_t* exp_code) const
{
    FpuRegDouble freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_absfd(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}


/*!
 * @brief Negates the sign of the floating-point number. : 浮動小数点符号反転
 * @param a Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Neg (const FpuRegDouble &a,
                                uint32_t* exp_code) const
{
    FpuRegDouble freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_negfd(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Neg (const FpuRegSingle &a,
                                uint32_t* exp_code) const
{
    FpuRegSingle freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_negfs(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

/*!
 * @brief Negates the sign of the floating-point number after FNMA/ADDSUBN/SUBADDN. : FNMA/ADDSUBN/SUBADDN の後の符号反転
 * @param a Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegSingle FpuOpeDevice::Mneg (const FpuRegSingle &a,
                                uint32_t* exp_code) const
{
    FpuRegSingle freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_Mneg(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

/*!
 * @brief Gets the reciprocal of the floating-point number : 浮動小数点逆数
 * @param a Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Recip (const FpuRegDouble &a, uint32_t* exp_code) const
{
    FpuRegDouble freg2 (a.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_recipfd(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Recip (const FpuRegSingle &a, uint32_t* exp_code) const
{
    FpuRegSingle freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3b)
    FpuRegSingle fregr(0);
    float32_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_recipfs(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }
    return fregr;
}


/*!
 * @brief Gets the square root of the floating-point number : 浮動小数点平方根
 * @param a Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Sqrt (const FpuRegDouble &a,
                                uint32_t* exp_code) const
{
    FpuRegDouble freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_sqrtfd(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}


FpuRegSingle FpuOpeDevice::Sqrt (const FpuRegSingle &a,
                                uint32_t* exp_code) const
{
    FpuRegSingle freg2 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf2, sfres;
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_sqrtfs(sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}


FpuRegDouble FpuOpeDevice::Rsqrt (const FpuRegDouble &a, uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf1, sfres;
    sf1.v = freg1.GetInteger();

    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_rsqrt(sf1);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }

    return fregr;
}


FpuRegSingle FpuOpeDevice::Rsqrt (const FpuRegSingle &a, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sfres;
    sf1.v = freg1.GetInteger();

    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_rsqrt(sf1);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }

    return fregr;
}


/*!
 * @brief Gets Multiply-add of the floating-point number. : 浮動小数点積和算
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param c Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegSingle FpuOpeDevice::Madd (const FpuRegSingle &a, const FpuRegSingle &b,
                                 const FpuRegSingle &c, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());
    FpuRegSingle freg3 (c.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sf3, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    sf3.v = freg3.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_mulAdd(sf2, sf3, sf1);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return fregr;
}

/*!
 * @brief Gets Multiply-subtract of the floating-point number. : 浮動小数点積和算(減算)
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param c Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegSingle FpuOpeDevice::Msub (const FpuRegSingle &a, const FpuRegSingle &b,
                                 const FpuRegSingle &c, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());
    FpuRegSingle freg3 (c.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sf3, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    sf3.v = freg3.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_mulSub(sf2, sf3, sf1);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return fregr;
}

/*!
 * @brief Gets the Maximum of the floating-point number. : 浮動小数点最大値
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Max (const FpuRegDouble &a, const FpuRegDouble &b, uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());
    FpuRegDouble freg2 (b.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_max(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Max (const FpuRegSingle &a, const FpuRegSingle &b, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_maxfs(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }
    return fregr;
}

/*!
 * @brief Gets the Minimum of the floating-point number. : 浮動小数点最小値
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 演算結果オブジェクト
 */
FpuRegDouble FpuOpeDevice::Min (const FpuRegDouble &a, const FpuRegDouble &b, uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());
    FpuRegDouble freg2 (b.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float64_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_min(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr;
}

FpuRegSingle FpuOpeDevice::Min (const FpuRegSingle &a, const FpuRegSingle &b, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float32_t sf1, sf2, sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_minfs(sf1, sf2);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }
    return fregr;
}


/*!
 * @brief Compares the floating-point number. : 浮動小数点大小等号比較
 * @param a Operand object. : オペランドオブジェクト
 * @param b Operand object. : オペランドオブジェクト
 * @param fcond The condition of the comparison : 比較条件
 * @param exp_code Exception code return. : 例外コード
 * @return The object of the result. : 比較結果
 */
bool FpuOpeDevice::Cmp (const FpuRegDouble &a, const FpuRegDouble &b, int32_t fcond, uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());
    FpuRegDouble freg2 (b.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    float64_t sf1, sf2;
    bool sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_cmp(sf1, sf2, fcond);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return sfres;
}

bool FpuOpeDevice::Cmp (const FpuRegSingle &a, const FpuRegSingle &b, int32_t fcond, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());
    FpuRegSingle freg2 (b.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3b)
    float32_t sf1, sf2;
    bool sfres;
    sf1.v = freg1.GetInteger();
    sf2.v = freg2.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_cmpfs(sf1, sf2, fcond);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return sfres;
}


/*!
 * @brief Converts from f32 to i32 type depends on FPSR.RM value.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return i32 value is converted.
 */
int32_t FpuOpeDevice::Cvt_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    int32_t res = F32_to_I32 (a, GetFpsrRM(), exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to i32 type depends on round to nearest value.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return i32 value is converted.
 */
int32_t FpuOpeDevice::Round_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for round instruction.
    int32_t res = F32_to_I32 (a, ROUNDING_ROUND, exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to i32 type depends on round toward zero.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return i32 value is converted.
 */
int32_t FpuOpeDevice::Trnc_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for truncate instruction.
    int32_t res = F32_to_I32 (a, ROUNDING_TRUNC, exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to i32 type depends on round toward negative-infinity.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return i32 value is converted.
 */
int32_t FpuOpeDevice::Floor_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for floor instruction.
    int32_t res = F32_to_I32 (a, ROUNDING_FLOOR, exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to i32 type depends on round toward positive infinity.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return i32 value is converted.
 */
int32_t FpuOpeDevice::Ceil_F32_I32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for ceil instruction.
    int32_t res = F32_to_I32 (a, ROUNDING_CEIL, exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to i32 type.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @param rounding_mode converting mode.
 * @return i32 value is converted.
 */
int32_t FpuOpeDevice::F32_to_I32 (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    float32_t sf1;
    int32_t res;
    sf1.v = freg1.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(rounding_mode);
    res = m_softfloat->Cvt_f32_i32(sf1);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return res;
}


/*!
* @brief Converts from f32 to ui32 type depends on FPSR.RM value.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::Cvt_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   uint32_t res = F32_to_UI32 (a, GetFpsrRM(), exp_code);
   return res;
}


/*!
 * @brief Converts from f32 to ui32 type depends on round to nearest value.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return ui32 value is converted.
 */
uint32_t FpuOpeDevice::Round_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for round instruction.
    uint32_t res = F32_to_UI32 (a, ROUNDING_ROUND, exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to ui32 type depends on rounding to maximum.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return ui32 value is converted.
 */
uint32_t FpuOpeDevice::Ceil_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for ceil instruction.
    uint32_t res = F32_to_UI32 (a, ROUNDING_CEIL, exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to ui32 type depends on rounding to minimum.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return ui32 value is converted.
 */
uint32_t FpuOpeDevice::Floor_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for floor instruction.
    uint32_t res = F32_to_UI32 (a, ROUNDING_FLOOR, exp_code);
    return res;
}


/*!
 * @brief Converts from f32 to ui32 type depends on round toward zero.
 * @param a f32 value needs to convert.
 * @param exp_code Exception code return.
 * @return ui32 value is converted.
 */
uint32_t FpuOpeDevice::Trnc_F32_UI32 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode for trnc instruction.
    uint32_t res = F32_to_UI32 (a, ROUNDING_TRUNC, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to ui32 type.
* @param a f32 value needs to convert.
* @param exp_code: exception code return.
* @param rounding_mode converting mode.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::F32_to_UI32 (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
   FpuRegSingle freg1 (a.GetInteger ());

   // SetFpuExceptionFlags(0);           // clear (2b)
   m_softfloat->SetExceptionFlags(0); // clear (3a)
   float32_t sf1;
   uint32_t res;
   sf1.v = freg1.GetInteger();
   m_softfloat->SetInputFlushFlag(0);
   m_softfloat->SetFlushEnable(GetFpsrFS());
   m_softfloat->SetFlushNearestEnable(GetFpsrFN());
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   m_softfloat->SetRoundingMode(rounding_mode);
   res = m_softfloat->Cvt_f32_ui32(sf1);

   *exp_code = m_softfloat->GetExceptionFlags();
   if (m_softfloat->GetInputFlushFlag() != 0) {
     SetFpsrIF_(1);
   }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
   if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
       *exp_code &= ~FPE_I;
   }

   return res;
}


/*!
* @brief Converts from f32 to i64 type depends on FPSR.RM value.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return i64 value is converted.
*/
int64_t FpuOpeDevice::Cvt_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    int64_t res = F32_to_I64 (a, GetFpsrRM(), exp_code);
    return res;
}


/*!
* @brief Converts from f32 to i64 type depends on round to nearest value.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
int64_t FpuOpeDevice::Round_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for round instruction.
    int64_t res = F32_to_I64 (a, ROUNDING_ROUND, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to i64 type depends on rounding to maximum.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
int64_t FpuOpeDevice::Ceil_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for ceil instruction.
    int64_t res = F32_to_I64 (a, ROUNDING_CEIL, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to i64 type depends on rounding to minimum.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
int64_t FpuOpeDevice::Floor_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for floor instruction.
    int64_t res = F32_to_I64 (a, ROUNDING_FLOOR, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to i64 type depends on round toward zero.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
int64_t FpuOpeDevice::Trnc_F32_I64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for trnc instruction.
    int64_t res = F32_to_I64 (a, ROUNDING_TRUNC, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to ui64 type.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @param rounding_mode converting mode.
* @return ui32 value is converted.
*/
int64_t FpuOpeDevice::F32_to_I64 (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    float32_t sf1;
    int64_t res;
    sf1.v = freg1.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(rounding_mode);
    res = m_softfloat->Cvt_f32_i64(sf1);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return res;
}


/*!
* @brief Converts from f32 to ui64 type depends on FPSR.RM value.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Cvt_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    uint64_t res = F32_to_UI64 (a, GetFpsrRM(), exp_code);
    return res;
}


/*!
* @brief Converts from f32 to ui64 type depends on round to nearest value.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Round_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for round instruction.
    uint64_t res = F32_to_UI64 (a, ROUNDING_ROUND, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to ui64 type depends on rounding to maximum.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Ceil_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for ceil instruction.
    uint64_t res = F32_to_UI64 (a, ROUNDING_CEIL, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to ui64 type depends on rounding to minimum.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Floor_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for floor instruction.
    uint64_t res = F32_to_UI64 (a, ROUNDING_FLOOR, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to ui64 type depends on round toward zero.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Trnc_F32_UI64 (const FpuRegSingle &a, uint32_t *exp_code) const
{
    // m_softfloat function will choose the rounding mode depends for trnc instruction.
    uint64_t res = F32_to_UI64 (a, ROUNDING_TRUNC, exp_code);
    return res;
}


/*!
* @brief Converts from f32 to ui64 type.
* @param a f32 value needs to convert.
* @param exp_code Exception code return.
* @param rounding_mode: converting mode.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::F32_to_UI64 (const FpuRegSingle &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    float32_t sf1;
    uint64_t res;
    sf1.v = freg1.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(rounding_mode);
    res = m_softfloat->Cvt_f32_ui64(sf1);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }

    // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return res;
}


/*
* @brief Converts from f64 to i32 type depends on FPSR.RM value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i32 value is converted.
*/
int32_t FpuOpeDevice::Cvt_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   int32_t res = F64_to_I32 (a, GetFpsrRM(), exp_code);
   return res;
}


/*
* @brief Converts from f64 to i32 type depends on rounding to nearest value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i32 value is converted.
*/
int32_t FpuOpeDevice::Round_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on round instruction.
   int32_t res = F64_to_I32 (a, ROUNDING_ROUND, exp_code);
   return res;
}


/*
* @brief Converts from f64 to i32 type depends on rounding toward zero.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i32 value is converted.
*/
int32_t FpuOpeDevice::Trnc_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on truncate instruction.
   int32_t res = F64_to_I32 (a, ROUNDING_TRUNC, exp_code);
   return res;
}


/*
* @brief Converts from f64 to i32 type depends on rounding to maximum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i32 value is converted.
*/
int32_t FpuOpeDevice::Ceil_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on ceil instruction.
   int32_t res = F64_to_I32 (a, ROUNDING_CEIL, exp_code);
   return res;
}


/*
* @brief Converts from f64 to i32 type depends on rounding to minimum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i32 value is converted.
*/
int32_t FpuOpeDevice::Floor_F64_I32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on floor instruction.
   int32_t res = F64_to_I32 (a, ROUNDING_FLOOR, exp_code);
   return res;
}


/*!
* @brief Converts from f64 to i32 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @param rounding_mode: converting mode.
* @return i32 value is converted.
*/
int32_t FpuOpeDevice::F64_to_I32 (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
   FpuRegDouble freg1 (a.GetInteger ());

   // SetFpuExceptionFlags(0);           // clear (2b)
   m_softfloat->SetExceptionFlags(0); // clear (3a)
   float64_t sf1;
   int32_t res;
   sf1.v = freg1.GetInteger();
   m_softfloat->SetInputFlushFlag(0);
   m_softfloat->SetFlushEnable(GetFpsrFS());
   m_softfloat->SetFlushNearestEnable(GetFpsrFN());
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   m_softfloat->SetRoundingMode(rounding_mode);
   res = m_softfloat->Cvt_f64_i32(sf1);

   *exp_code = m_softfloat->GetExceptionFlags();
   if (m_softfloat->GetInputFlushFlag() != 0) {
     SetFpsrIF_(1);
   }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
   if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
       *exp_code &= ~FPE_I;
   }

   return res;
}


/*
* @brief Converts from f64 to ui32 type depends on FPSR.RM value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::Cvt_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   uint32_t res = F64_to_UI32 (a, GetFpsrRM(), exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui32 type depends on rounding to nearest value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::Round_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on round instruction.
   uint32_t res = F64_to_UI32 (a, ROUNDING_ROUND, exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui32 type depends on rounding toward zero.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::Trnc_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on truncate instruction.
   uint32_t res = F64_to_UI32 (a, ROUNDING_TRUNC, exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui32 type depends on rounding to maximum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::Ceil_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on ceil instruction.
   uint32_t res = F64_to_UI32 (a, ROUNDING_CEIL, exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui32 type depends on rounding to minimum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::Floor_F64_UI32 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on floor instruction.
   uint32_t res = F64_to_UI32 (a, ROUNDING_FLOOR, exp_code);
   return res;
}


/*!
* @brief Converts from f64 to ui32 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @param rounding_mode converting mode.
* @return ui32 value is converted.
*/
uint32_t FpuOpeDevice::F64_to_UI32 (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
   FpuRegDouble freg1 (a.GetInteger ());

   // SetFpuExceptionFlags(0);           // clear (2b)
   m_softfloat->SetExceptionFlags(0); // clear (3a)
   float64_t sf1;
   uint32_t res;
   sf1.v = freg1.GetInteger();
   m_softfloat->SetInputFlushFlag(0);
   m_softfloat->SetFlushEnable(GetFpsrFS());
   m_softfloat->SetFlushNearestEnable(GetFpsrFN());
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   m_softfloat->SetRoundingMode(rounding_mode);
   res = m_softfloat->Cvt_f64_ui32(sf1);

   *exp_code = m_softfloat->GetExceptionFlags();
   if (m_softfloat->GetInputFlushFlag() != 0) {
     SetFpsrIF_(1);
   }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
   if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
       *exp_code &= ~FPE_I;
   }

   return res;
}


/*
* @brief Converts from f64 to i64 type depends on FPSR.RM value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i64 value is converted.
*/
int64_t FpuOpeDevice::Cvt_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   int64_t res = F64_to_I64 (a, GetFpsrRM(), exp_code);
   return res;
}


/*
* @brief Converts from f64 to i64 type depends on rounding to nearest value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i64 value is converted.
*/
int64_t FpuOpeDevice::Round_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on round instruction.
   int64_t res = F64_to_I64 (a, ROUNDING_ROUND, exp_code);
   return res;
}


/*
* @brief Converts from f64 to i64 type depends on rounding toward zero.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i64 value is converted.
*/
int64_t FpuOpeDevice::Trnc_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on truncate instruction.
   int64_t res = F64_to_I64 (a, ROUNDING_TRUNC, exp_code);
   return res;
}


/*
* @brief Converts from f64 to i64 type depends on rounding to maximum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i64 value is converted.
*/
int64_t FpuOpeDevice::Ceil_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on ceil instruction.
   int64_t res = F64_to_I64 (a, ROUNDING_CEIL, exp_code);
   return res;
}


/*
* @brief Converts from f64 to i64 type depends on rounding to minimum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return i64 value is converted.
*/
int64_t FpuOpeDevice::Floor_F64_I64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on floor instruction.
   int64_t res = F64_to_I64 (a, ROUNDING_FLOOR, exp_code);
   return res;
}


/*!
* @brief Converts from f64 to i64 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @param rounding_mode converting mode.
* @return i64 value is converted.
*/
int64_t FpuOpeDevice::F64_to_I64 (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
   FpuRegDouble freg1 (a.GetInteger ());

   // SetFpuExceptionFlags(0);           // clear (2b)
   m_softfloat->SetExceptionFlags(0); // clear (3a)
   float64_t sf1;
   int64_t res;
   sf1.v = freg1.GetInteger();
   m_softfloat->SetInputFlushFlag(0);
   m_softfloat->SetFlushEnable(GetFpsrFS());
   m_softfloat->SetFlushNearestEnable(GetFpsrFN());
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   m_softfloat->SetRoundingMode(rounding_mode);
   res = m_softfloat->Cvt_f64_i64(sf1);

   *exp_code = m_softfloat->GetExceptionFlags();
   if (m_softfloat->GetInputFlushFlag() != 0) {
     SetFpsrIF_(1);
   }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
   if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
       *exp_code &= ~FPE_I;
   }

   return res;
}


/*
* @brief Converts from f64 to ui64 type depends on FPSR.RM value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Cvt_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   uint64_t res = F64_to_UI64 (a, GetFpsrRM(), exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui64 type depends on rounding to nearest value.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Round_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on round instruction.
   uint64_t res = F64_to_UI64 (a, ROUNDING_ROUND, exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui64 type depends on rounding toward zero.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Trnc_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on truncate instruction.
   uint64_t res = F64_to_UI64 (a, ROUNDING_TRUNC, exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui64 type depends on rounding to maximum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Ceil_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on ceil instruction.
   uint64_t res = F64_to_UI64 (a, ROUNDING_CEIL, exp_code);
   return res;
}


/*
* @brief Converts from f64 to ui64 type depends on rounding to minimum.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return ui64 value is converted.
*/
uint64_t FpuOpeDevice::Floor_F64_UI64 (const FpuRegDouble &a, uint32_t *exp_code) const
{
   // m_softfloat function will choose the rounding mode depends on floor instruction.
   uint64_t res = F64_to_UI64 (a, ROUNDING_FLOOR, exp_code);
   return res;
}


/*!
* @brief Converts from f64 to i64 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @param rounding_mode: converting mode.
* @return i64 value is converted.
*/
uint64_t FpuOpeDevice::F64_to_UI64 (const FpuRegDouble &a, uint32_t rounding_mode, uint32_t *exp_code) const
{
   FpuRegDouble freg1 (a.GetInteger ());

   // SetFpuExceptionFlags(0);           // clear (2b)
   m_softfloat->SetExceptionFlags(0); // clear (3a)
   float64_t sf1;
   uint64_t res;
   sf1.v = freg1.GetInteger();
   m_softfloat->SetInputFlushFlag(0);
   m_softfloat->SetFlushEnable(GetFpsrFS());
   m_softfloat->SetFlushNearestEnable(GetFpsrFN());
   // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
   m_softfloat->SetRoundingMode(rounding_mode);
   res = m_softfloat->Cvt_f64_ui64(sf1);

   *exp_code = m_softfloat->GetExceptionFlags();
   if (m_softfloat->GetInputFlushFlag() != 0) {
     SetFpsrIF_(1);
   }

   // If Overflow/Underflow bit is 1, then incorrect operation exception (I) bit is cleared.
   if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
       *exp_code &= ~FPE_I;
   }

   return res;
}


/*
* @brief Converts from i32 to f32 type.
* @param a i32 value needs to convert.
* @param exp_code Exception code return.
* @return f32 value is converted.
*/
FpuRegSingle FpuOpeDevice::Cvt_I32_F32 (const int32_t &a, uint32_t* exp_code)
{
    FpuRegSingle res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float32_t ret_f32 = m_softfloat->Cvt_i32_f32(a);
    res.SetInteger(ret_f32.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*
* @brief Converts from ui32 to f32 type.
* @param a ui32 value needs to convert.
* @param exp_code Exception code return.
* @return f32 value is converted.
*/
FpuRegSingle FpuOpeDevice::Cvt_UI32_F32 (const uint32_t &a, uint32_t* exp_code)
{
    FpuRegSingle res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float32_t ret_f32 = m_softfloat->Cvt_ui32_f32(a);
    res.SetInteger(ret_f32.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*
* @brief Converts from ui64 to f32 type.
* @param a ui64 value needs to convert.
* @param exp_code Exception code return.
* @return f32 value is converted.
*/
FpuRegSingle FpuOpeDevice::Cvt_UI64_F32 (const uint64_t &a, uint32_t* exp_code)
{
    FpuRegSingle res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float32_t ret_f32 = m_softfloat->Cvt_ui64_f32(a);
    res.SetInteger(ret_f32.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*
* @brief Converts from i64 to f32 type.
* @param a i64 value needs to convert.
* @param exp_code Exception code return.
* @return f32 value is converted.
*/
FpuRegSingle FpuOpeDevice::Cvt_I64_F32 (const int64_t &a, uint32_t* exp_code)
{
    FpuRegSingle res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float32_t ret_f32 = m_softfloat->Cvt_i64_f32(a);
    res.SetInteger(ret_f32.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*
* @brief Converts from i32 to f64 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return f64 value is converted.
*/
FpuRegDouble FpuOpeDevice::Cvt_I32_F64 (const int32_t &a, uint32_t* exp_code)
{
    FpuRegDouble res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float64_t ret_f64 = m_softfloat->Cvt_i32_f64(a);
    res.SetInteger(ret_f64.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*
* @brief Converts from ui32 to f64 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return f64 value is converted.
*/
FpuRegDouble FpuOpeDevice::Cvt_UI32_F64 (const uint32_t &a, uint32_t* exp_code)
{
    FpuRegDouble res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float64_t ret_f64 = m_softfloat->Cvt_ui32_f64(a);
    res.SetInteger(ret_f64.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*
* @brief Converts from i64 to f64 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return f64 value is converted.
*/
FpuRegDouble FpuOpeDevice::Cvt_I64_F64 (const int64_t &a, uint32_t* exp_code)
{
    FpuRegDouble res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float64_t ret_f64 = m_softfloat->Cvt_i64_f64(a);
    res.SetInteger(ret_f64.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*
* @brief Converts from ui64 to f64 type.
* @param a f64 value needs to convert.
* @param exp_code Exception code return.
* @return f64 value is converted.
*/
FpuRegDouble FpuOpeDevice::Cvt_UI64_F64 (const uint64_t &a, uint32_t* exp_code)
{
    FpuRegDouble res;
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    // m_softfloat function will choose the rounding mode depends on FPSR.RM value.
    m_softfloat->SetRoundingMode(GetFpsrRM());
    float64_t ret_f64 = m_softfloat->Cvt_ui64_f64(a);
    res.SetInteger(ret_f64.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
      SetFpsrIF_(1);
    }

    return res;
}


/*!
 * @brief Converts to the floating-point format. : 浮動小数点形式への変換
 * @param a Operand object. : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return Results of the single precision floatingrpoint number. : 変換後のデータ(単精度浮動小数点数)
 */
float FpuOpeDevice::Cvt_DS (const FpuRegDouble &a, uint32_t* exp_code) const
{
    FpuRegDouble freg1 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float64_t sf1;
    float32_t sfres;
    sf1.v = freg1.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f64_to_f32(sf1);

    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }

    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if ((*exp_code & GetFpsrXE() & (FPE_O|FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }

    return fregr.GetFloat();
}


/*!
 * @brief Converts to the floating-point format. : 浮動小数点形式への変換
 * @param a Operand : オペランドオブジェクト
 * @param exp_code Exception code return. : 例外コード
 * @return Results of the double precision floatingrpoint number. : 変換後のデータ(倍精度浮動小数点数)
 */
double FpuOpeDevice::Cvt_SD (const FpuRegSingle &a, uint32_t* exp_code) const
{
    FpuRegSingle freg1 (a.GetInteger ());

    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float32_t sf1;
    float64_t sfres;
    sf1.v = freg1.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_to_f64(sf1);
    fregr.SetInteger(sfres.v);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if ((*exp_code & GetFpsrXE() & (FPE_O | FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }
    return fregr.GetFloat();
}


uint16_t FpuOpeDevice::Cvt_SH (const FpuRegSingle &a, uint32_t* exp_code)
{
    FpuRegSingle freg1 (a.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegDouble fregr(0);
    float32_t sf1;
    float16_t sfres;
    sf1.v = freg1.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f32_to_f16(sf1);

    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if ((*exp_code & GetFpsrXE() & (FPE_O | FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }
    return sfres.v;
}


uint32_t FpuOpeDevice::Cvt_HS (const FpuRegHalf &a, uint32_t* exp_code)
{
    FpuRegHalf freg1 (a.GetInteger ());
    m_softfloat->SetExceptionFlags(0); // clear (3a)
    FpuRegSingle fregr(0);
    float16_t sf1;
    float32_t sfres;
    sf1.v = freg1.GetInteger();
    m_softfloat->SetInputFlushFlag(0);
    m_softfloat->SetOverflowExceptionEnableFlag(GetFpsrXE() & FPE_O);
    m_softfloat->SetUnderflowExceptionEnableFlag(GetFpsrXE() & FPE_U);
    m_softfloat->SetFlushEnable(GetFpsrFS());
    m_softfloat->SetFlushNearestEnable(GetFpsrFN());
    m_softfloat->SetRoundingMode(GetFpsrRM());
    sfres = m_softfloat->f16_to_f32(sf1);
    fregr.SetInteger(sfres.v);
    *exp_code = m_softfloat->GetExceptionFlags();
    if (m_softfloat->GetInputFlushFlag() != 0) {
        SetFpsrIF_(1);
    }
    // Overflow/Underflow が立っているときは不正確演算例外(I)をマスクする
    if ((*exp_code & GetFpsrXE() & (FPE_O | FPE_U)) != 0) {
        *exp_code &= ~FPE_I;
    }
    return fregr.GetInteger();
}
