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
#include "ecc.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

/// Constructor of ECC class
Cecc::Cecc(sc_module_name name):   Cgeneral_reset(name)
                                  ,vpcl::tlm_tgt_if<32> (name)
                                  ,Cecc_regif ((std::string)name, 32)
                                  // Clocks
                                  ,pclk("pclk")
                                  // Input ports
                                  ,preset_n("preset_n")
                                  // Output ports
                                  ,ec7tie1("ec7tie1")
                                  ,ec7tie2("ec7tie2")
                                  /// M40/Maintenance add
                                  ,ec7tiovf("ec7tiovf")
{//{{{
    // Initialize handleCommand  
    CommandInit(this->name());
    Cecc_regif::set_instance_name(this->name());
    /// M40/Maintenance modify - fix 0-frequency issue
    mPclkPeriod = 0;
    Initialize();
    // Initialize output ports
    ec7tie1.initialize(false);
    ec7tie2.initialize(false);
    /// M40/Maintenance add
    ec7tiovf.initialize(false);

    mCmdReset = false;

    // Clock Method
    SC_METHOD(PclkMethod);
    dont_initialize();
    sensitive << pclk;
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;
    SC_METHOD(Ec7tie1Method);
    dont_initialize();
    sensitive << mEc7tie1Event;
    SC_METHOD(Ec7tie2Method);
    dont_initialize();
    sensitive << mEc7tie2Event;
    SetLatency_TLM(0, true);

    /// M40/Maintenance add
    SC_METHOD(Ec7tiovfMethod);
    dont_initialize();
    sensitive << mEc7tiovfEvent;
}//}}}

// Destructor of Cecc class
Cecc::~Cecc()
{//{{{
}//}}}

// Process transport request
void Cecc::tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int data_length = 0;
    bool result = tgt_get_gp_attribute(command, addr, p_data, data_length, trans, false);

    if ((!result) || (p_data == NULL)) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }

    /// M40/Maintenance modify
    unsigned int temp_add = (unsigned int)addr & 0x3F;

    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, data_length);
        // Read register
        if (((0x10 + ((unsigned int)NumOfErrorStage << 2)) <= temp_add) && (temp_add < 0x30)) {
            re_printf("error", "Invalid Read access at address 0x%08X.\n", temp_add);
        } else {
            result = reg_rd((unsigned int)addr, p_data, data_length);
        }
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        if (((0x10 + ((unsigned int)NumOfErrorStage << 2)) <= temp_add) && (temp_add < 0x30)) {
            re_printf("error", "Invalid Write access at address 0x%08X.\n", temp_add);
        } else {
            result = reg_wr((unsigned int)addr, p_data, data_length);
        }
    } else {
        // TLM_IGNORE_COMMAND
        result = true;
    }

    // Set response status
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}//}}}

// Process debug transport request
unsigned int Cecc::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int data_length = 0;
    bool result = tgt_get_gp_attribute(command, addr, p_data, data_length, trans, true);
    if ((!result) || (p_data == NULL)) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return 0;
    }

    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, data_length);
        result = reg_rd_dbg((unsigned int)addr, p_data, data_length);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        result = reg_wr_dbg((unsigned int)addr, p_data, data_length);
    } else {
        // TLM_IGNORE_COMMAND
        result = true;
        data_length = 0;
    }

    // Set response status & return value
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (result) {
        return data_length;
    } else {
        return 0;
    }
}//}}}

void Cecc::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    tgt_acc(trans, t);
}//}}}

unsigned int Cecc::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{///{{{
    return tgt_acc_dbg(trans);
}//}}}

// Called by PclkMethod() or SetCLKfreq handleCommand
void Cecc::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    std::string defined_clk_name = "pclk";
    // Clock name must be "pclk"
    if (clk_name == defined_clk_name) {
        /// M40/Maintenance modify - fix 0-frequency issue
        bool is_period_valid = true;
        double clk_period = 0;
        double time_unit = 0;

        if (clk_freq > 0) {
            // Calculate the clock period to check the resolution
            clk_period = (double)(emNanoSecond / clk_freq);
            time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if (time_unit > clk_period) {
                is_period_valid = false;
                re_printf("warning","The %s period is less than 1 unit time of system.\n", defined_clk_name.c_str());
            }
        } else {
            // clk_freq = 0
            clk_period = 0;
            // Cancel all events
            mEc7tie1Event.cancel();
            mEc7tie2Event.cancel();
            mEc7tiovfEvent.cancel();
        }

        if (is_period_valid) {
            mPclkPeriod = clk_period;
            re_printf("info","%s is set with a frequency as %lld.%06d.\n", defined_clk_name.c_str(), (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
            SetLatency_TLM(mPclkPeriod, false);
        }
    } else {
        re_printf("warning","Clock name %s is invalid.\n", clk_name.c_str());
    }
}//}}}

void Cecc::PclkMethod(void)
{//{{{ 
    if(!Cgeneral_reset::IsResetPeriod()) {
        SetCLKfreq("pclk",(double)(pclk.read()));
    }
}//}}}

void Cecc::AssertReset(const double delay, const double period)
{//{{{
    mCmdReset = true;
    // Call assert reset function of Cgeneral_reset class
    Cgeneral_reset::Com_AssertReset(delay, period);
}//}}}

void Cecc::ResetMethod(void)
{//{{{
    //Check value of preset_n signal
    bool is_PortReset = (preset_n.read() == vpcl::emResetActive) ? true : false;

    // Call reset method of common class
    Cgeneral_reset::Com_ResetMethod(is_PortReset);
}//}}}

void Cecc::Ec7tie1Method(void)
{//{{{
    if(!Cgeneral_reset::IsResetPeriod()) {
        DumpInterruptMsg("ec7tie1", mEc7tie1_val);
        ec7tie1.write(mEc7tie1_val);
        if (mEc7tie1_val) {
            mEc7tie1_val = false;
            mEc7tie1Event.notify(mPclkPeriod, SC_NS);
        }
    } else {
        ec7tie1.write(false);
    }
}//}}}

void Cecc::Ec7tie2Method(void)
{//{{{
    if(!Cgeneral_reset::IsResetPeriod()) {
        DumpInterruptMsg("ec7tie2", mEc7tie2_val);
        ec7tie2.write(mEc7tie2_val);
        if (mEc7tie2_val) {
            mEc7tie2_val = false;
            mEc7tie2Event.notify(mPclkPeriod, SC_NS);
        }
    } else {
        ec7tie2.write(false);
    }
}//}}}

void Cecc::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        // Initialize all variables
        Initialize();

        // Cancel all events
        mEc7tie1Event.cancel();
        mEc7tie2Event.cancel();
        /// M40/Maintenance add
        mEc7tiovfEvent.cancel();

        // Set default value to all output ports
        mEc7tie1Event.notify();
        mEc7tie2Event.notify();
        /// M40/Maintenance add
        mEc7tiovfEvent.notify();
    }
    // Call reset function of common/general class
    Cecc_regif::EnableReset(is_active);
    if(mCmdReset == false){  // if reset by AssertReset commad, tgt if isn't initialized.
        this->tgt_enable_reset(is_active);
    } else {
        mCmdReset = false;
    }
}//}}}

void Cecc::EccError(const unsigned int err_det_set)
{//{{{
    if((!Cgeneral_reset::IsResetPeriod())&&(!Cgeneral_reset::IsSimStarting())) {
        /// M40/Maintenance modify - fix 0-frequency issue
        if (mPclkPeriod > 0) {
            if (((*EC710CTL)["ECERVF"] == 1)||(!AccEnErrJudg)) {
                ErrorJudgment(err_det_set);
            }
        } else {
            re_printf("warning","EccError command is called while clock frequency is 0.\n");
        }
    }
}//}}}

void Cecc::ErrorJudgment(const unsigned int error_kind)
{//{{{
    if (error_kind == em2bitsError) {
        UpdateErrAddr(error_kind);
        if (((*EC710CTL)["ECER2F"] == 0) && (TwoBitsErrJudg)) {
            re_printf("info", "2bits error detection.\n");
            if ((*EC710CTL)["EC2EDIC"] == 1) {
                mEc7tie2_val = true;
                mEc7tie2Event.notify();
            }
            (*EC710CTL)["ECER2F"] = 1;
        }
    }
    if (error_kind == em1bitError) {
        UpdateErrAddr(error_kind);
        if ((*EC710CTL)["ECER1F"] == 0) {
            re_printf("info", "1bit error detection.\n");
            if (((*EC710CTL)["EC1EDIC"] == 1)&&((*EC710CTL)["ECER2F"] == 0)) {
                mEc7tie1_val = true;
                mEc7tie1Event.notify();
            }
            (*EC710CTL)["ECER1F"] = 1;
        }
    }
}//}}}

void Cecc::UpdateErrAddr(const unsigned int error_kind) 
{//{{{
    /// M40/Maintenance modify
    if (!IsAddrCaptureSupport()) {
        return;
    }

    unsigned int ec710ctl = (unsigned int)(*EC710CTL);
    bool is_update = true;
    bool is_overwrite = false;
    bool is_overflow = false;
    bool is_all_1bitError = true;

    if (mCurrentIndex < (unsigned int)(NumOfErrorStage - 1)) {
        for (unsigned int i = 0; i <= mCurrentIndex; i++) {
            if (((*EC710EAD[i]) == (EccErrorAddress & 0x7FFFFFFF)) && (error_kind == ((ec710ctl >> ((i * 2) +16)) & 0x3))) {
                is_update = false;
            }
        }
    } else if (mCurrentIndex == (unsigned int)(NumOfErrorStage - 1)) {
        // mCurrentIndex == (NumOfErrorStage - 1) -> maximum index
        is_overflow = true;

        for (unsigned int i = 0; i <= mCurrentIndex; i++) {
            if (((ec710ctl >> ((i * 2) + 16)) & 0x3) != 0x1) {
                is_all_1bitError = false;
            }
        }

        if ((is_all_1bitError) && (error_kind == em2bitsError) && ((*EC710CTL)["ECER2F"] == 0)) {
            // First 2bitsError occurs after a sequence of 1bitError
            is_overwrite = true;
            re_printf("info", "The EC710EAD%d register is over-written.\n", mCurrentIndex);
        } else {
            is_update = false;
        }
    }

    if (is_update) {
        if (!is_overwrite) {
            if (mCurrentIndex == emNumOfErrorStageMax) {
                // First error occur
                mCurrentIndex = 0;
            } else {
                // New error is detected
                mCurrentIndex += 1;
            }
            // Update error factor to current index
            (*EC710CTL) = ec710ctl | (unsigned int)(error_kind << ((mCurrentIndex * 2) +16));
        }
        // Update error address
        (*EC710EAD[mCurrentIndex]) = (EccErrorAddress & 0x7FFFFFFF);
    }

    if (is_overflow) {
        re_printf("info", "The Error Address register is overflow.\n");
        // Set Overflow flag
        if ((*EC710CTL)["ECOVFF"] == 0) {
            (*EC710CTL)["ECOVFF"] = 1;
        }
        // Output Overflow interrupt
        if (!mIs_ec7tiovf_assert) {
            mEc7tiovf_val = true;
            mEc7tiovfEvent.notify();
        } else {
            re_printf("warning", "ec7tiovf interrupt has been already asserted.\n");
        }
    }
}//}}}

void Cecc::cb_EC710CTL_EMCA1(RegCBstr str)
{//{{{
    /// M40/Maintenance modify & fix 0-frequency issue
    if (mPclkPeriod > 0) {
        if (str.is_wr) {
            // Check flags clearing
            ClearFlags((bool)(*EC710CTL)["ECER1C"] , (bool)(*EC710CTL)["ECER2C"]);

            if (((((str.data >> 14)&0x3) != 0x1))&&((str.pre_data&0x40)!=(str.data&0x40))) {
                re_printf("warning", "Write access to EC710CTL.ECERVF is valid only when the value of bit [15:14] is 2'b01.\n");
                (*EC710CTL)["ECERVF"] = (str.pre_data >> 6)&0x1;
            }
            if (((*EC710CTL)["ECERVF"] == 0) && (AccEnErrJudg)) {     // Error flag is cleared when error judgment is disabled
                ClearFlags(true , true);
            }
        } else {
            // Read access - Non-exist ECDEDFm/ECSEDFm bit will return 0
            (*EC710CTL) &= (unsigned int)(((unsigned long long)1 << ((NumOfErrorStage * 2) + 16)) - 1);
        }

        (*EC710CTL)["ECER2C"] = 0;
        (*EC710CTL)["ECER1C"] = 0;
        (*EC710CTL)["EMCA1"] = 0;
        (*EC710CTL)["EMCA0"] = 0;
    }
}//}}}

void Cecc::cb_EC710TMC_ETMA1(RegCBstr str)
{//{{{
    /// M40/modify - fix 0-frequency issue
    if (mPclkPeriod > 0) {
        if (str.is_wr) {
            if (((((str.data >> 14)&0x3) != 0x2))&&((str.pre_data&0x80)!=(str.data&0x80))) {
                re_printf("warning", "Write access to EC710TMC.ECTMCE is valid only when the value of bit [15:14] is 2'b10.\n");
                (*EC710TMC)["ECTMCE"] = (str.pre_data >> 7)&0x1;
            }
            if ((*EC710TMC)["ECTMCE"] == 0) {
                (*EC710TED) = 0;
                (*EC710TRC) = 0;
                if ((((str.data >> 14)&0x3) != 0x2)) { 
                    (*EC710TMC)["ECTRRS"] = (str.pre_data >> 4)&0x1;
                } else {
                    if (((str.pre_data >> 7)&0x1) == 0) {
                        (*EC710TMC)["ECTRRS"] = (str.pre_data >> 4)&0x1;
                    } else {
                        (*EC710TMC)["ECTRRS"] = 0;
                    }
                }
            }
        }
        (*EC710TMC)["ETMA1"] = 0;
        (*EC710TMC)["ETMA0"] = 0;
    }
}//}}}

void Cecc::cb_EC710TED_ECEDB(RegCBstr str)
{//{{{
    /// M40/modify - fix 0-frequency issue
    if (mPclkPeriod > 0) {
        if (str.is_wr) {
            if ((*EC710TMC)["ECTMCE"] == 0) {
                re_printf("warning", "Register EC710TED can be written only in test mode.\n");
                (*EC710TED) = 0;
            }
        } else {
            if (((*EC710TMC)["ECTMCE"] == 1)&&((*EC710TMC)["ECTRRS"] == 1)) {
                (*EC710TED) = RamReadData;
                (*EC710TRC)["ECHORD"] = ((unsigned int)RamReadEccPart & 0x7F);
            }
        }
    }
}//}}}

void Cecc::cb_EC710TRC_ECERDB(RegCBstr str)
{//{{{
    /// M40/modify - fix 0-frequency issue
    if (mPclkPeriod > 0) {
        if (str.is_wr) {
            if ((*EC710TMC)["ECTMCE"] == 0) {
                re_printf("warning", "Register EC710TRC can be written only in test mode.\n");
                (*EC710TRC) = 0;
            }
        } else {
            if ((*EC710TMC)["ECTMCE"] == 1) {
                if ((*EC710TMC)["ECTRRS"] == 1) {
                    (*EC710TRC)["ECERDB"] = ((unsigned int)RamWriteEccPart & 0x7F);
                }
                (*EC710TRC)["ECSYND"] = ((unsigned int)DecodeResult & 0x7F);
                (*EC710TRC)["ECECRD"] = ((unsigned int)EncodeResult & 0x7F);
            }
        }
    }
}//}}}

std::string Cecc::CommandCB(const std::vector<std::string>& args) 
{//{{{
    std::vector<std::string> _args = args;
    std::string ret = "";
    if ((int)_args.size() == 2) {
        if (_args[0] == "NumOfErrorStage") {
            /// M40/Maintenance modify
            if (NumOfErrorStage > emNumOfErrorStageMax) { // Invalid value of NumOfErrorStage
                re_printf("warning", "NumOfErrorStage must be set the value from 0 to 8.\n");
                NumOfErrorStage = mNumOfErrorStage;
            }
        } else if (_args[0] == "AccEnErrJudg") {
            if (!AccEnErrJudg) {
                (*EC710CTL)["ECERVF"] = 0;
            } else {
                if ((*EC710CTL)["ECERVF"] == 0) {  // Error flag is cleared when error judgment is disabled
                    ClearFlags(true , true);
                }
            }
        } else if (_args[0] == "TwoBitsErrJudg") {
            if (!TwoBitsErrJudg) {
                ClearFlags(false , true);
            }
        }
    }
    return ret;
}//}}}

// Output message when issue interrupt
void Cecc::DumpInterruptMsg(std::string intr_name, const bool next_val)
{//{{{
    if (DumpInterrupt) {
        if (next_val) {
            re_printf("info","INT [ECC: %s] Assert\n", intr_name.c_str());
        } else {
            re_printf("info","INT [ECC: %s] Deassert\n", intr_name.c_str());
        }
    }
}//}}}

void Cecc::Initialize(void)
{//{{{
    // Initialize member variable
    mNumOfErrorStage = 1;
    mEc7tie1_val = false;
    mEc7tie2_val = false;
    /// M40/Maintenance add
    mEc7tiovf_val = false;
    mCurrentIndex = emNumOfErrorStageMax;
    mIs_ec7tiovf_assert = false;
}//}}}

void Cecc::SetLatency_TLM(const double clk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(clk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}


/// M40/Maintenance add
// Process ec7tiovf interrupt
void Cecc::Ec7tiovfMethod(void)
{//{{{
    if(!Cgeneral_reset::IsResetPeriod()) {
        DumpInterruptMsg("ec7tiovf", mEc7tiovf_val);
        ec7tiovf.write(mEc7tiovf_val);
        if (mEc7tiovf_val) {
            mIs_ec7tiovf_assert = true;
            mEc7tiovf_val = false;
            mEc7tiovfEvent.notify(mPclkPeriod, SC_NS);
        } else {
            mIs_ec7tiovf_assert = false;
        }
    } else {
        ec7tiovf.write(false);
    }
}//}}}


// Check Error Address Capture function
bool Cecc::IsAddrCaptureSupport(void)
{//{{{
    if (NumOfErrorStage > emNoErrAddrStag) {
        return true;
    } else {
        return false;
    }
}//}}}


// Check & clear flags
void Cecc::ClearFlags(const bool is_1bit_flag, const bool is_2bits_flag)
{//{{{
    // Flags clearing
    if (is_1bit_flag) {
        (*EC710CTL)["ECER1F"] = 0;
    }
    if (is_2bits_flag) {
        (*EC710CTL)["ECER2F"] = 0;
    }

    // Check Error Address Capture function
    if ((is_1bit_flag || is_2bits_flag) && (IsAddrCaptureSupport())) {
        (*EC710CTL)["ECOVFF"] = 0;
        // ECDEDFm & ECSEDFm
        (*EC710CTL) &= 0xFFFF;

        // Initialize buffer variables
        mCurrentIndex = emNumOfErrorStageMax;
        for (unsigned int i = 0; i < emNumOfErrorStageMax; i++) {
            (*EC710EAD[i]) = 0;
        }
    }
}//}}}

// vim600: set foldmethod=marker :
