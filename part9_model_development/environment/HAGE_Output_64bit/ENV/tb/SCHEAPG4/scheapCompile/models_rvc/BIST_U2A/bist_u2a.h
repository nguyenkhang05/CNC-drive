// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __BIST_U2A_H__
#define __BIST_U2A_H__
#include "tlm_tgt_if.h"
#include "bist_u2a_regif.h"
#include "re_define.h"

#include "general_reset.h"

/// BIST_U2A model class
class Cbist_u2a: public Cgeneral_reset
                ,public vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,2>
                ,public Cbist_u2a_regif
{
    
#include "bist_u2a_cmdif.h"

public:
    SC_HAS_PROCESS(Cbist_u2a);
    Cbist_u2a(sc_module_name name);
    ~Cbist_u2a();

    // clock
    sc_in<sc_dt::uint64> pclk;
    // reset signal
    sc_in<bool> reset_n;
    sc_in<bool> reset1_n;
    sc_in<bool> reset2_n;
    sc_in<bool> reset_param;

    // BIST process trigger
    sc_in<bool> start_bist;
    // FBIST mode
    sc_in<sc_uint<2> > FBISTMD;
    // BIST selection in Flash Option Byte
    sc_in<sc_uint<2> > opbt_TESTSET;
    // LBIST scenario select Flash Option Byte
    sc_in<sc_uint<2> > opbt_LBISTSEL;

    // BIST process completed signal
    sc_out<bool> end_bist;

    // BIST attributes
    sc_attribute<unsigned int> MBISTruntime;
    sc_attribute<unsigned int> LBISTruntime_FB0_PWUP_scenario1;
    sc_attribute<unsigned int> LBISTruntime_FB0_PWUP_scenario2;
    sc_attribute<unsigned int> LBISTruntime_FB0_PWUP_scenario3;
    sc_attribute<unsigned int> LBISTruntime_FB0_PWUP_scenario4;
    sc_attribute<unsigned int> LBISTruntime_FB0_SYSR2_scenario1;
    sc_attribute<unsigned int> LBISTruntime_FB0_SYSR2_scenario2;
    sc_attribute<unsigned int> LBISTruntime_FB0_SYSR2_scenario3;
    sc_attribute<unsigned int> LBISTruntime_FB0_SYSR2_scenario4;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario1;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario2;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario3;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario4;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario5;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario6;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario7;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario8;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario9;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario10;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario11;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario12;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario13;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario14;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario15;
    sc_attribute<unsigned int> LBISTruntime_FB12_DPR_scenario16;

private:
    // Constant value
    enum eConstant {
        emNanoSecond = 1000000000
       ,emRegValueNum = 10
       ,emRegIndexLBISTSIG1 = 5
       ,emRegIndexLBISTSIG2 = 6
       ,emRegIndexMBISTSIG1 = 7
       ,emRegIndexMBISTSIG2 = 8
       ,emRegIndexMBISTSIG3 = 9
       ,emRegMBISTNum = 16
    };

    enum eBISTId {
          emBISTId = 0
        , emCommonId = 1
        , emMaxId = 2
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
    std::vector<stRegValue> mRegMBIST1;  // Store initial and asserted value of registers related to MBIST1
    std::vector<stRegValue> mRegMBIST2;  // Store initial and asserted value of registers related to MBIST2
    std::vector<stRegValue> mRegMBIST3;  // Store initial and asserted value of registers related to MBIST3
    bool mIsZeroClock;                  // Determine zero clock status
    bool mCmdReset;                     // Indicate reset by AssertReset command
    bool mIsSystemReset2;               // Indicate System Reset2
    bool mIsMBIST_active;               // Indicate MBIST active
    bool mIsLBIST_active;               // Indicate LBIST active
    bool mIsMBIST3_enable;              // Indicate MBIST3 enable (disable after DeepSTOPReset) 

    // Functions
    // tlm functions
    //void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t);
    //unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans);
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);

    // Normal functions
    void EnableReset(const bool is_active);
    void SetTlmParameter(const unsigned int id, const bool is_ClockSetting, const double pclk_period);

    // handle_command functions
    void AssertBISTReset(const std::string rst_name, const double start_time, const double period);
    //void AssertReset(const double start_time, const double period);
    void SetCLKfreq(const std::string clk_name, const double clk_freq);

    // Methods
    void PCLKMethod(void);
    void ResetMethod(void);
    void start_bistChangeMethod(void);
    void ProcessFinishMethod(void);
    void WriteOutputPortMethod(void);
    void opbt_TESTSET_Change_Method(void);
    void opbt_LBISTSEL_Change_Method(void);
};

#endif //__BIST_U2A_H__
