// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012, 2013 Renesas Electronics Corporation
// Copyright(c) 2012, 2013 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __ECC_H__
#define __ECC_H__
#include "re_define.h"
#include <cstdio>
#include <cstdarg>

#include "general_reset.h"
#include "ecc_regif.h"
#include "tlm_tgt_if.h"

/// ECC model class
class Cecc: public Cgeneral_reset
           ,public vpcl::tlm_tgt_if<32>
           ,public Cecc_regif
{
    
#include "ecc_cmdif.h"

public:
    SC_HAS_PROCESS(Cecc);
    Cecc(sc_module_name name);
    ~Cecc();

    // Clocks
    sc_in<sc_dt::uint64> pclk;

    // Input ports
    sc_in<bool> preset_n;

    // Output ports
    sc_out<bool> ec7tie1;
    sc_out<bool> ec7tie2;
    /// M40/Maintenance add
    sc_out<bool> ec7tiovf;          // Overflow interrupt

private:
    // Constance value
    enum eTimeEnum {
        emNanoSecond = 1000000000
        /// M40/Maintenance add
        ,emNumOfErrorStageMax  = 8
    };
    enum eErrorKind {
        emNoError    = 0   // No ECC error
       ,em1bitError  = 1 // ECC 1bit error
       ,em2bitsError = 2 // ECC 2bits error
    };
    enum eErrAddrStag {
        emNoErrAddrStag = 0  // Not store error address
       ,emOneErrAddrStag = 1 // 1 step of error address register
    };

    // Member variables
    double mPclkPeriod;
    bool mEc7tie1_val;
    bool mEc7tie2_val;
    /// M40/Maintenance add
    bool mEc7tiovf_val;
    unsigned int mCurrentIndex;
    bool mIs_ec7tiovf_assert;

    bool mCmdReset;

    // handleCommand variable
    unsigned char mNumOfErrorStage;
    // Event
    sc_event mEc7tie1Event;
    sc_event mEc7tie2Event;
    /// M40/Maintenance add
    sc_event mEc7tiovfEvent;

    // Functions
    // TLM functions
    void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans);
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);
    void SetLatency_TLM(const double clk_period, const bool is_constructor);

    // Normal functions
    void EnableReset(const bool is_active);
    void ErrorJudgment(const unsigned int error_kind);
    void DumpInterruptMsg(std::string intr_name, const bool next_val);
    void UpdateErrAddr(const unsigned int error_kind);
    void Initialize(void);   // This function is used to initialize member variables of ECC model
    /// M40/Maintenance add
    bool IsAddrCaptureSupport(void);
    void ClearFlags(const bool is_1bit_flag, const bool is_2bits_flag);

    // handle_command functions
    void AssertReset(const double start_time, const double period);
    void SetCLKfreq(const std::string clk_name, const double clk_freq);
    void EccError(const unsigned int err_det_set);
    
    // Callback functions
    /// M40/Maintenance modify
    void cb_EC710CTL_EMCA1(RegCBstr str);
    void cb_EC710TMC_ETMA1(RegCBstr str);
    void cb_EC710TED_ECEDB(RegCBstr str);
    void cb_EC710TRC_ECERDB(RegCBstr str);
    std::string CommandCB(const std::vector<std::string>& args);

    // Methods
    void PclkMethod(void);
    void ResetMethod(void);
    void Ec7tie1Method(void);
    void Ec7tie2Method(void);
    /// M40/Maintenance add
    void Ec7tiovfMethod(void);
};

#endif //__ECC_H__
