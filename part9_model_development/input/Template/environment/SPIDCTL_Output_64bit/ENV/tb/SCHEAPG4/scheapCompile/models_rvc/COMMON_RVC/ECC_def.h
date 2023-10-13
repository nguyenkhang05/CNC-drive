// ---------------------------------------------------------------------
// $Id: ECC_def.h,v 1.4 2021/07/28 12:28:29 ducla Exp $
//
// Copyright(c) 2017-2021 Renesas Electronics Corporation
// Copyright(c) 2017-2021 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_DEF_H__
#define __ECC_DEF_H__

#define MAX_ADDWIDTH_MSB 31
#define MIN_ADDWIDTH_LSB 0

#define REG32_MASK 0xFFFFFFFF
#define MAX_AWIDTH_MSB 26
#define MIN_AWIDTH_MSB 7
#define MAX_AWIDTH_LSB 4
#define MIN_AWIDTH_LSB 2
#define MAX_CAP_DEPTH 32
#define MIN_CAP_DEPTH 1
#define MAX_ERRINT_INIT 0xFF
#define MIN_ERRINT_INIT 0x0

enum eErrorType {
    em1bitError             // 1bit error in data
    , em2bitError            // 2bit error in data
    , emAddressParityError   // address parity error
    , emAddressFeedbackError // address feedback error
    , em1bitAddressError     // 1bit error in address
    , em2bitAddressError     // 2bit error in address
    , emNoneError
};

struct ErrorInfo {
    unsigned int    address;    // Address: error at this address
    eErrorType      type;       // type: 1bit, 2bit, address parity, address feedback
    unsigned int    edl_code;   // EDL value with will be stored into error address register
    unsigned int    spid;       // SPID value with will be stored into error address register
    ErrorInfo(unsigned int address, eErrorType type, unsigned int edl_code) {
        this->address   = address;
        this->type      = type;
        this->edl_code  = edl_code;
        this->spid      = 0;
    }
    ErrorInfo(unsigned int address, eErrorType type, unsigned int edl_code, unsigned int spid) {
        this->address   = address;
        this->type      = type;
        this->edl_code  = edl_code;
        this->spid      = spid;
    }
    ErrorInfo() {
        this->address   = 0;
        this->type      = emNoneError;
        this->edl_code  = 0;
        this->spid      = 0;
    }
    void Initialize() {
        this->address   = 0;
        this->type      = emNoneError;
        this->edl_code  = 0;
        this->spid      = 0;
    }
};
#endif //__ECC_DEF_H__
