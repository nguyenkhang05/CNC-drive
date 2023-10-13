// ---------------------------------------------------------------------
// $Id: ECC_APB_AgentController.h,v 1.4 2018/01/18 06:29:15 chanle Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_APB_AGENT_CONTROLLER_H__
#define __ECC_APB_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

enum eErrorType{
    em1bitError             // 1bit error in data
    ,em2bitError            // 2bit error in data
    ,emAddressParityError   // address parity error
    ,emAddressFeedbackError // address feedback error
    ,em1bitAddressError     // 1bit error in address (used for ECC_APB)
    ,em2bitAddressError     // 2bit error in address (used for ECC_APB)
    ,emNoneError            
};

struct ErrorInfo{
    unsigned int    address;    // Address: error at this address
    eErrorType      type;       // type: 1bit, 2bit, address parity, address feedback
    unsigned int    edl_code;   // EDL value with will be stored into error address register
    ErrorInfo(unsigned int address, eErrorType type, unsigned int edl_code){
        this->address   = address;
        this->type      = type;
        this->edl_code  = edl_code;
    }
    ErrorInfo(){
        this->address   = 0;
        this->type      = emNoneError;
        this->edl_code  = 0;
    }
    void Initialize(){
        this->address   = 0;
        this->type      = emNoneError;
        this->edl_code  = 0;
    }
};

// Class ECC_APB_AgentController
class ECC_APB_AgentController
{
#include "ECC_APB_cmdif.h"
public:
    // Constructor/Destructor
    ECC_APB_AgentController(void) {;}
    virtual ~ECC_APB_AgentController(void) {;}

    // Interface (APIs)
    virtual bool CheckClockPeriod() = 0;
    virtual bool GetResetStatus() = 0;
    virtual void WriteDisablePort(unsigned int dis_value) = 0;
    virtual void AssertInterrupt(std::string int_name) = 0;
};
#endif //__ECC_APB_AGENT_CONTROLLER_H__
