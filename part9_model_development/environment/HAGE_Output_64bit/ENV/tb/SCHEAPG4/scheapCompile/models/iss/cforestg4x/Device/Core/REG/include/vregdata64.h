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

#include "./forest_common.h"

class VRegData64 {
    union {
        uint64_t d64;
        uint32_t d32[2];
        uint16_t d16[4];
        uint8_t  d8 [8];
    } data;

 public:
    VRegData64 () {}
    explicit VRegData64 (uint64_t d) {
        Set (d);
    }
    VRegData64 (uint32_t d1, uint32_t d0) {
        Set (d1, d0);
    }
    VRegData64 (uint16_t d3, uint16_t d2, uint16_t d1, uint16_t d0) {
        Set (d3, d2, d1, d0);
    }
    VRegData64 (uint8_t d7, uint8_t d6, uint8_t d5, uint8_t d4,
                uint8_t d3, uint8_t d2, uint8_t d1, uint8_t d0) {
        Set (d7, d6, d5, d4, d3, d2, d1, d0);
    }

    void Set32 (uint32_t idx, uint32_t d) { data.d32[idx] = d; }
    void Set16 (uint32_t idx, uint16_t d) { data.d16[idx] = d; }
    void Set8  (uint32_t idx, uint8_t  d) { data.d8 [idx] = d; }

    void Set (uint64_t d) {
        data.d64 = d;
    }
    void Set (uint32_t d1, uint32_t d0) {
        data.d32[1] = d1;
        data.d32[0] = d0;
    }
    void Set (uint16_t d3, uint16_t d2, uint16_t d1, uint16_t d0) {
        data.d16[3] = d3;
        data.d16[2] = d2;
        data.d16[1] = d1;
        data.d16[0] = d0;
    }
    void Set (uint8_t d7, uint8_t d6, uint8_t d5, uint8_t d4,
              uint8_t d3, uint8_t d2, uint8_t d1, uint8_t d0) {
        data.d8[7] = d7;
        data.d8[6] = d6;
        data.d8[5] = d5;
        data.d8[4] = d4;
        data.d8[3] = d3;
        data.d8[2] = d2;
        data.d8[1] = d1;
        data.d8[0] = d0;
    }

    uint64_t Get64 (void)         const { return data.d64; }
    int64_t  GetSigned64 (void)   const { return static_cast<int64_t>(data.d64); }
    uint32_t Get32 (uint32_t idx) const { return data.d32[idx]; }
    uint16_t Get16 (uint32_t idx) const { return data.d16[idx]; }
    uint8_t  Get8  (uint32_t idx) const { return data.d8 [idx]; }

    int32_t GetSigned32 (uint32_t idx) const {
        return static_cast<int32_t> (data.d32[idx]);
    }
    int16_t GetSigned16 (uint32_t idx) const {
        return static_cast<int16_t> (data.d16[idx]);
    }
    int8_t  GetSigned8  (uint32_t idx) const {
        return static_cast<int8_t>  (data.d8 [idx]);
    }
};
