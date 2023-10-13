// ---------------------------------------------------------------------
// $Id: ECC_CMNU_AgentController.h,v 1.1 2015/08/31 04:39:12 ngankimtran2 Exp $
//
// Copyright(c) 2014 - 2015 Renesas Electronics Corporation
// Copyright(c) 2014 - 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ECC_CMNU_AGENT__
#define __ECC_CMNU_AGENT__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include "OSCI2.h"
#include <stdarg.h>

class ECC_CMNU_AgentController
{
#include "ECC_CMNU_cmdif.h"
public:
    struct ErrorInfoStructure {
        bool is_ready;
        bool addr_ecc_2bit_error_status;
        bool addr_ecc_1bit_error_status;
        bool data_ecc_2bit_error_status;
        bool data_ecc_1bit_error_status;
        bool addr_parity_error_read_status;
        bool addr_parity_error_write_status;
        bool parity_bit_error_status;
        bool data_ecc_2bit_error_ext_status;
        bool data_ecc_1bit_error_ext_status;
        unsigned int error_addr;
        ErrorInfoStructure (void) {
            Initialize();
        }
        void Initialize (void) {
            is_ready = false;
            addr_ecc_2bit_error_status = false;
            addr_ecc_1bit_error_status = false;
            data_ecc_2bit_error_status = false;
            data_ecc_1bit_error_status = false;
            addr_parity_error_read_status = false;
            addr_parity_error_write_status = false;
            parity_bit_error_status = false;
            data_ecc_2bit_error_ext_status = false;
            data_ecc_1bit_error_ext_status = false;
            error_addr = 0x0;
        }
    };//Define error information structure

    ECC_CMNU_AgentController(void) {;}
    virtual ~ECC_CMNU_AgentController(void) {;}
    virtual void ControlErrorOutputSignals (const std::string signal_name) = 0;
    virtual void ControlECCDisableSignals (void) = 0;
    virtual bool CheckClockPeriod (const std::string clock_name) = 0;
};
#endif
