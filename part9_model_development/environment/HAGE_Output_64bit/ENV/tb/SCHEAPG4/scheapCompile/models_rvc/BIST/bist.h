// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2013 Renesas Electronics Corporation
// Copyright(c) 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __BIST_H__
#define __BIST_H__
#include "tlm_tgt_if.h"
#include "bist_regif.h"
#include "re_define.h"

#include "general_reset.h"

/// BIST model class
class Cbist: public Cgeneral_reset
            ,public vpcl::tlm_tgt_if<32>
            ,public Cbist_regif
{
    
#include "bist_cmdif.h"

public:
    SC_HAS_PROCESS(Cbist);
    Cbist(sc_module_name name);
    ~Cbist();

    // clock
    sc_in<sc_dt::uint64> pclk;
    // reset signal
    sc_in<bool> preset_n;

    // BIST process trigger
    sc_in<bool> start_bist;
    // BIST process completed signal
    sc_out<bool> end_bist;

private:
    // Constant value
    enum eConstant {
        emNanoSecond = 1000000000
       ,emRegNum = 15
    };

    // Register value
    struct stRegValue {
        vpcl::re_register *reg_p;        // Register pointer
        unsigned int initial;           // Initial value
        unsigned int assert_value;      // Value when start_bist is asserted
        stRegValue (vpcl::re_register *_reg_p, unsigned int initial, unsigned int assert_value)
        {
            this->reg_p = NULL;
            sc_assert(_reg_p != NULL);
            this->reg_p = _reg_p;
            this->initial = initial;
            this->assert_value = assert_value;
        }
    };

    // Event
    sc_event mProcessFinishEvent;       // BIST finish waiting processing time
    sc_event mWriteOutputPortEvent;     // Write value to end_bist port
    sc_event mReadPortEvent;            // Update input port after reset

    // Variables
    bool mEnd_bistValue;                // Value which is output to end_bist
    std::vector<stRegValue> mRegValue;  // Store initial and asserted value of registers
    bool mIsZeroClock;                  // Determine zero clock status

    bool mCmdReset;                     // Indicate reset by AssertReset command

    // Functions
    // tlm functions
    void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans);

    // Normal functions
    void EnableReset(const bool is_active);
    void SetTlmParameter(const bool is_ClockSetting, const double pclk_period);

    // handle_command functions
    void AssertReset(const double start_time, const double period);
    void SetCLKfreq(const std::string clk_name, const double clk_freq);

    // Methods
    void PCLKMethod(void);
    void ResetMethod(void);
    void start_bistChangeMethod(void);
    void ProcessFinishMethod(void);
    void WriteOutputPortMethod(void);
};

#endif //__BIST_H__
