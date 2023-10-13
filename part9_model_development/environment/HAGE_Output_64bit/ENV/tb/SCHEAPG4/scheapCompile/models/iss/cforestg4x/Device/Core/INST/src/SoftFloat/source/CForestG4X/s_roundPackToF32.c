/**
 * @file s_roundPackToF32.c
 *
 * @date Created on: 2016
 * @brief Add Flush for FMAF of SoftFloat (refs #3347)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

/*============================================================================

This C source file is part of the SoftFloat IEEE Floating-Point Arithmetic
Package, Release 3e, by John R. Hauser.

Copyright 2011, 2012, 2013, 2014, 2015, 2017 The Regents of the University of
California.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
    this list of conditions, and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions, and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

 3. Neither the name of the University nor the names of its contributors may
    be used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS", AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ARE
DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#include <stdbool.h>
#include <stdint.h>
#include "platform.h"
#include "internals.h"
#include "softfloat.h"

bool softfloat_fs = 0;
bool softfloat_fn = 0;

uint_fast32_t softfloatFlush32(bool sign, int_fast16_t exp, uint_fast32_t sig,
                               bool* is_skip_rounded) {
  uint_fast32_t ret_val;
  // int32_t expl;
  uint_fast8_t local_exceptionFlags = 0;
  // uint_fast8_t roundBits;
  uint_fast32_t temp_val;

  // sig is 32 bit here.
  // sig is left shifted 7bit -> LSB 7bit are guard bit to be rounded.
  // sig is overraped with exp. -> (sig >> 7)[24:23] should be treated as exp.
  if (0xFD <= (uint16_t)exp) {
    if (exp < 0) {
      sig = softfloat_shiftRightJam32(sig, -exp);
      exp = 0;
    } else if (0xFD < exp) {
      goto NonFlush;
    }
  }

  uint32_t stickey_bits = sig & 0x7F;
  sig >>= 7;

  // Regenerate exp and sig
  // Becasue sig [24:23] is overrapped to exp.
  temp_val = packToF32UI(sign, exp, sig);
  exp = expF32UI(temp_val);
  sig = fracF32UI(temp_val);

  if (exp == 0 && (sig != 0 || stickey_bits != 0)) {
    // Flush
    if (softfloat_roundingMode == softfloat_round_near_even) {
      if (!softfloat_fn) {
        sig = 0;
      } else {
        if (sig < 0x00400000U) {
          exp = 0;
          sig = 0;
        } else {
          exp = 1;
          sig = 0;
        }
      }
    } else if (softfloat_roundingMode == softfloat_round_minMag) {
      sig = 0;
    } else if (softfloat_roundingMode == softfloat_round_max) {
      if (!sign) {
        exp = 1;
      }
      sig = 0;
    } else if (softfloat_roundingMode == softfloat_round_min) {
      if (sign) {
        exp = 1;
      }
      sig = 0;
    }
    local_exceptionFlags |= softfloat_flag_underflow;
    local_exceptionFlags |= softfloat_flag_inexact;
    if (!softfloat_fs) {
      // Disable Flush.
      // Cause unimplemented exception (E)
      // If E=1, other flag isn't set
      local_exceptionFlags = softfloat_flag_unimplement;
    }
    softfloat_exceptionFlags |= local_exceptionFlags;
    ret_val = packToF32UI(sign, exp, sig);
    *is_skip_rounded = true;
  } else {
  NonFlush:
    ret_val = 0; // Flushed value isn't used. Value needs to be rounded.
    *is_skip_rounded = false;
  }
  return ret_val;
}

float32_t
 softfloat_roundPackToF32( bool sign, int_fast16_t exp, uint_fast32_t sig )
{
    uint_fast8_t roundingMode;
    bool roundNearEven;
    uint_fast8_t roundIncrement, roundBits;
    bool isTiny;
    uint_fast32_t uiZ;
    union ui32_f32 uZ;

    /* Flush before rounding for G4MH FPU/FXU : ADD start*/
    bool is_skip_rounded;
    uiZ = softfloatFlush32(sign, exp, sig, &is_skip_rounded);
    if (is_skip_rounded) {
        goto uiZ;
    }
    /* End : Flush before rounding for G4MH FPU/FXU : ADD finish */

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    roundingMode = softfloat_roundingMode;
    roundNearEven = (roundingMode == softfloat_round_near_even);
    roundIncrement = 0x40;
    if ( ! roundNearEven && (roundingMode != softfloat_round_near_maxMag) ) {
        roundIncrement =
            (roundingMode
                 == (sign ? softfloat_round_min : softfloat_round_max))
                ? 0x7F
                : 0;
    }
    roundBits = sig & 0x7F;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( 0xFD <= (unsigned int) exp ) {
        if ( exp < 0 ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            isTiny =
                (softfloat_detectTininess == softfloat_tininess_beforeRounding)
                    || (exp < -1) || (sig + roundIncrement < 0x80000000);
            sig = softfloat_shiftRightJam32( sig, -exp );
            exp = 0;
            roundBits = sig & 0x7F;
            if ( isTiny && roundBits ) {
                softfloat_raiseFlags( softfloat_flag_underflow );
            }
        } else if ( (0xFD < exp) || (0x80000000 <= sig + roundIncrement) ) {
            /*----------------------------------------------------------------
            *----------------------------------------------------------------*/
            softfloat_raiseFlags(
                softfloat_flag_overflow | softfloat_flag_inexact );
            uiZ = packToF32UI( sign, 0xFF, 0 ) - ! roundIncrement;
            goto uiZ;
        }
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sig = (sig + roundIncrement)>>7;
    if ( roundBits ) {
        softfloat_exceptionFlags |= softfloat_flag_inexact;
#ifdef SOFTFLOAT_ROUND_ODD
        if ( roundingMode == softfloat_round_odd ) {
            sig |= 1;
            goto packReturn;
        }
#endif
    }
    sig &= ~(uint_fast32_t) (! (roundBits ^ 0x40) & roundNearEven);
    if ( ! sig ) exp = 0;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 packReturn:
    uiZ = packToF32UI( sign, exp, sig );
 uiZ:
    uZ.ui = uiZ;
    return uZ.f;
}
