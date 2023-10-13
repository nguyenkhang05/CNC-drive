// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------


#include "bist_u2a.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

/// Constructor of BIST class
Cbist_u2a::Cbist_u2a(sc_module_name name): Cgeneral_reset(name)
                                          ,vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,2>(name)
                                          ,Cbist_u2a_regif((std::string)name, 32)
                                          ,pclk("pclk")
                                          ,reset_n("reset_n")
                                          ,reset1_n("reset1_n")
                                          ,reset2_n("reset2_n")
                                          ,reset_param("reset_param")
                                          // Input ports
                                          ,start_bist("start_bist")
                                          ,FBISTMD("FBISTMD")
                                          ,opbt_TESTSET("opbt_TESTSET")
                                          ,opbt_LBISTSEL("opbt_LBISTSEL")
                                          // Output ports
                                          ,end_bist("end_bist")
                                          // BIST attributes
                                          ,MBISTruntime("MBISTruntime", 10000)
                                          ,LBISTruntime_FB0_PWUP_scenario1("LBISTruntime_FB0_PWUP_scenario1", 5000)
                                          ,LBISTruntime_FB0_PWUP_scenario2("LBISTruntime_FB0_PWUP_scenario2", 10000)
                                          ,LBISTruntime_FB0_PWUP_scenario3("LBISTruntime_FB0_PWUP_scenario3", 15000)
                                          ,LBISTruntime_FB0_PWUP_scenario4("LBISTruntime_FB0_PWUP_scenario4", 20000)
                                          ,LBISTruntime_FB0_SYSR2_scenario1("LBISTruntime_FB0_SYSR2_scenario1", 5000)
                                          ,LBISTruntime_FB0_SYSR2_scenario2("LBISTruntime_FB0_SYSR2_scenario2", 10000)
                                          ,LBISTruntime_FB0_SYSR2_scenario3("LBISTruntime_FB0_SYSR2_scenario3", 15000)
                                          ,LBISTruntime_FB0_SYSR2_scenario4("LBISTruntime_FB0_SYSR2_scenario4", 20000)
                                          ,LBISTruntime_FB12_DPR_scenario1("LBISTruntime_FB12_DPR_scenario1", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario2("LBISTruntime_FB12_DPR_scenario2", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario3("LBISTruntime_FB12_DPR_scenario3", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario4("LBISTruntime_FB12_DPR_scenario4", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario5("LBISTruntime_FB12_DPR_scenario5", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario6("LBISTruntime_FB12_DPR_scenario6", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario7("LBISTruntime_FB12_DPR_scenario7", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario8("LBISTruntime_FB12_DPR_scenario8", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario9("LBISTruntime_FB12_DPR_scenario9", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario10("LBISTruntime_FB12_DPR_scenario10", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario11("LBISTruntime_FB12_DPR_scenario11", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario12("LBISTruntime_FB12_DPR_scenario12", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario13("LBISTruntime_FB12_DPR_scenario13", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario14("LBISTruntime_FB12_DPR_scenario14", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario15("LBISTruntime_FB12_DPR_scenario15", 2000)
                                          ,LBISTruntime_FB12_DPR_scenario16("LBISTruntime_FB12_DPR_scenario16", 2000)
{//{{{
    // Initialize handleCommand
    CommandInit(this->name());
    Cbist_u2a_regif::set_instance_name(this->name());

    // Initialize variables
    mEnd_bistValue = true;
    mIsZeroClock = true;
    mCmdReset = false;
    mIsSystemReset2 = false;
    mIsMBIST_active = false;
    mIsMBIST3_enable = true;
    mIsLBIST_active = false;

    // Config register values
    mRegValue.push_back(stRegValue(LBISTREF1, 0x000A5A5A, 0x000A5A5A));
    mRegValue.push_back(stRegValue(LBISTREF2, 0x0005A5A5, 0x0005A5A5));
    mRegValue.push_back(stRegValue(MBISTREF1, 0x000AA55A, 0x000AA55A));
    mRegValue.push_back(stRegValue(MBISTREF2, 0x00055AA5, 0x00055AA5));
    mRegValue.push_back(stRegValue(MBISTREF3, 0x000AAA55, 0x000AAA55));
    mRegValue.push_back(stRegValue(LBISTSIG1, 0x0005A5A5, 0x000A5A5A));
    mRegValue.push_back(stRegValue(LBISTSIG2, 0x000A5A5A, 0x0005A5A5));
    mRegValue.push_back(stRegValue(MBISTSIG1, 0x00055AA5, 0x000AA55A));
    mRegValue.push_back(stRegValue(MBISTSIG2, 0x000AA55A, 0x00055AA5));
    mRegValue.push_back(stRegValue(MBISTSIG3, 0x000555AA, 0x000AAA55));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG0, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG1, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG2, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG3, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG4, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG5, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG6, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1FTAG7, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG0, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG1, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG2, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG3, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG4, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG5, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG6, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2FTAG7, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG0, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG1, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG2, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG3, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG4, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG5, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG6, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3FTAG7, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC0, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC1, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC2, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC3, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC4, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC5, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC6, 0xFFFFFFFF, 0x00000000));
    mRegMBIST1.push_back(stRegValue(MBIST1ECC7, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC0, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC1, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC2, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC3, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC4, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC5, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC6, 0xFFFFFFFF, 0x00000000));
    mRegMBIST2.push_back(stRegValue(MBIST2ECC7, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC0, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC1, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC2, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC3, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC4, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC5, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC6, 0xFFFFFFFF, 0x00000000));
    mRegMBIST3.push_back(stRegValue(MBIST3ECC7, 0xFFFFFFFF, 0x00000000));
    //mRegValue.push_back(stRegValue(BSEQ0ST , 0x00000001, 0x00000002));
    //mRegValue.push_back(stRegValue(BSEQ0STB, 0x00000002, 0x00000001));
    //mRegValue.push_back(stRegValue(BISTST  , 0x0000001F, 0x00000000));
    //mRegValue.push_back(stRegValue(BSEQ0SEL, 0x00000000, 0x00000000));

    // Initialize output ports
    end_bist.initialize(true);

    // Reset Method
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << reset_n;
    sensitive << reset1_n;
    sensitive << reset2_n;
    sensitive << reset_param;

    SC_METHOD(opbt_TESTSET_Change_Method);
    dont_initialize();
    sensitive << opbt_TESTSET;

    SC_METHOD(opbt_LBISTSEL_Change_Method);
    dont_initialize();
    sensitive << opbt_LBISTSEL;

    // Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;
    sensitive << mReadPortEvent;

    // Operation Method
    SC_METHOD(start_bistChangeMethod);
    dont_initialize();
    sensitive << start_bist;
    sensitive << mReadPortEvent;

    SC_METHOD(ProcessFinishMethod);
    dont_initialize();
    sensitive << mProcessFinishEvent;

    // Update Output ports Method
    SC_METHOD(WriteOutputPortMethod);
    dont_initialize();
    sensitive << mWriteOutputPortEvent;

    // TLM AT, LT
    for(unsigned int id = 0; id < emMaxId; id++){
        SetTlmParameter(id, false, 0);
    }
}//}}}


// Destructor of Cbist_u2a class
Cbist_u2a::~Cbist_u2a()
{//{{{
}//}}}


// Process debug transport request
//unsigned int Cbist_u2a::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
unsigned int Cbist_u2a::tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans)
{//{{{
    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 address = 0;
    unsigned int masked_address = 0;
    unsigned int regif_address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, true);
    sc_assert(data_ptr != NULL);

    if (id == emCommonId) {
        masked_address = address & 0xF;
        if (masked_address > 0xC) {
            //re_printf("error", "Invalid access address 0x%08X.\n", (unsigned int)address);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            return 0;
        }
        regif_address = masked_address;
    }
    else if (id == emBISTId) {
        masked_address = address & 0xFF;
        if (masked_address > 0xE4) {
            //re_printf("error", "Invalid access address 0x%08X.\n", (unsigned int)address);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            return 0;
        }
        regif_address = 0x100 + masked_address;
    }

    if (command == tlm::TLM_READ_COMMAND) {
        memset(data_ptr, 0, data_length);
        status = reg_rd_dbg(regif_address, data_ptr, data_length);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr_dbg(regif_address, data_ptr, data_length);
    } else {
        // TLM_IGNORE_COMMAND
        status = true;
        data_length = 0;
    }

    // Set response status & return value
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status) {
        return data_length;
    } else {
        return 0;
    }
}//}}}


// Process transport request
//void Cbist_u2a::tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t)
void Cbist_u2a::tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    if(Cgeneral_reset::IsResetPeriod()) {
        re_printf("error","Read/Write access is forbidden while resetting.\n");
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }

    // Get Generic Payload Attributes
    tlm::tlm_command command = tlm::TLM_IGNORE_COMMAND;
    sc_dt::uint64 address = 0;
    unsigned int masked_address = 0;
    unsigned int regif_address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, false);

    if (!status) {
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }
    sc_assert(data_ptr != NULL);

    if (id == emCommonId) {
        masked_address = address & 0xF;
        if (masked_address > 0xC) {
            re_printf("error", "Invalid access address 0x%08X.\n", (unsigned int)address);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            return;
        }
        regif_address = masked_address;
    }
    else if (id == emBISTId) {
        masked_address = address & 0xFF;
        if (masked_address > 0xE4) {
            re_printf("error", "Invalid access address 0x%08X.\n", (unsigned int)address);
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            return;
        }
        regif_address = 0x100 + masked_address;
    }

    if (command == tlm::TLM_READ_COMMAND) {        
        memset(data_ptr, 0, data_length);
        reg_rd(regif_address, data_ptr, data_length);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        reg_wr(regif_address, data_ptr, data_length);
    }

    // Set response status
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}//}}}


// Output new value to outuput ports
void Cbist_u2a::WriteOutputPortMethod(void)
{//{{{
    if (Cgeneral_reset::IsResetPeriod()) {
        // Initialize output ports in reset period
        end_bist.write(1);
    } else {
        end_bist.write(mEnd_bistValue);
    }
}//}}}


// start_bist change process
void Cbist_u2a::start_bistChangeMethod(void)
{//{{{
    if(Cgeneral_reset::IsResetPeriod()) {
        return;
    }

    if (mIsZeroClock) {
        re_printf("info","The pclk period is equal 0.\n");
        return;
    }
    // fix #126256 MBIST3 is disabled after DeepStop
    if (((FBISTMD.read() >> 1) & 0x1) == 1) {
        mIsMBIST3_enable = false;   // MBIST3 is disabled after Deep STOP Reset
    }else{
        mIsMBIST3_enable = true;  
    }

    bool is_assert = start_bist.read();

    if (is_assert) {
        re_printf("info","start_bist is asserted.\n");

        // BIST selection
       sc_uint<2> TESTSET = ((*BSEQ0SEL)["PARAMSEL"] == 1) ? (unsigned int)(*BSEQ0SEL)["HWTESTSET"] : (unsigned int)opbt_TESTSET.read();
        mIsLBIST_active = (( TESTSET       & 0x1) == 1) ? true : false;
        mIsMBIST_active = (((TESTSET >> 1) & 0x1) == 1) ? true : false;
        if (mIsMBIST_active == false && mIsLBIST_active == false) {
            re_printf("error","Either MBIST or LBIST is active.\n");
            return;
        }

        // De-assert end_bist
        mEnd_bistValue = false;
        re_printf("info","end_bist is de-asserted.\n");
        mWriteOutputPortEvent.notify();

        // Change registers value
        for (unsigned int i = 0; i < emRegValueNum; i++) {
            switch(i) {
                case emRegIndexLBISTSIG1:
                case emRegIndexLBISTSIG2:
                    (*mRegValue[i].reg_p) = (mIsLBIST_active) ? mRegValue[i].assert_value : mRegValue[i].initial;
                    break;
                case emRegIndexMBISTSIG1:
                case emRegIndexMBISTSIG2:
                    (*mRegValue[i].reg_p) = (mIsMBIST_active) ? mRegValue[i].assert_value : mRegValue[i].initial;
                    break;
                case emRegIndexMBISTSIG3:
                    (*mRegValue[i].reg_p) = (mIsMBIST_active && mIsMBIST3_enable) ? mRegValue[i].assert_value : mRegValue[i].initial;
                    break;
                default:
                    (*mRegValue[i].reg_p) = mRegValue[i].assert_value;
                    break;
            }
        }
        if (mIsMBIST_active) {
            for (unsigned int i = 0; i < emRegMBISTNum; i++) {
                (*mRegMBIST1[i].reg_p) = mRegMBIST1[i].assert_value;
                (*mRegMBIST2[i].reg_p) = mRegMBIST2[i].assert_value;
            }
            if (mIsMBIST3_enable) {
                for (unsigned int i = 0; i < emRegMBISTNum; i++) {
                    (*mRegMBIST3[i].reg_p) = mRegMBIST3[i].assert_value;
                }
            }
        }

        // Calculate BIST processing time
        unsigned int LBIST_time = 0;
        if ((*BSEQ0SEL)["PARAMSEL"] == 0) {
            switch(opbt_LBISTSEL.read()) {
                case 0x0: LBIST_time = LBISTruntime_FB0_PWUP_scenario1.value; break;
                case 0x1: LBIST_time = LBISTruntime_FB0_PWUP_scenario2.value; break;
                case 0x2: LBIST_time = LBISTruntime_FB0_PWUP_scenario3.value; break;
                default : LBIST_time = LBISTruntime_FB0_PWUP_scenario4.value; break;
            }
        } else {
            if (((FBISTMD.read() >> 1) & 0x1) == 0) {
                switch((*BSEQ0SEL)["HWLBISTSEL"] & 0x3) {
                    case 0x0: LBIST_time = LBISTruntime_FB0_SYSR2_scenario1.value; break;
                    case 0x1: LBIST_time = LBISTruntime_FB0_SYSR2_scenario2.value; break;
                    case 0x2: LBIST_time = LBISTruntime_FB0_SYSR2_scenario3.value; break;
                    default:  LBIST_time = LBISTruntime_FB0_SYSR2_scenario4.value; break;
                }
            } else {
                switch((*BSEQ0SEL)["HWLBISTSEL"]){
                    case 0x0: LBIST_time = LBISTruntime_FB12_DPR_scenario1.value; break;
                    case 0x1: LBIST_time = LBISTruntime_FB12_DPR_scenario2.value; break;
                    case 0x2: LBIST_time = LBISTruntime_FB12_DPR_scenario3.value; break;
                    case 0x3: LBIST_time = LBISTruntime_FB12_DPR_scenario4.value; break;
                    case 0x4: LBIST_time = LBISTruntime_FB12_DPR_scenario5.value; break;
                    case 0x5: LBIST_time = LBISTruntime_FB12_DPR_scenario6.value; break;
                    case 0x6: LBIST_time = LBISTruntime_FB12_DPR_scenario7.value; break;
                    case 0x7: LBIST_time = LBISTruntime_FB12_DPR_scenario8.value; break;
                    case 0x8: LBIST_time = LBISTruntime_FB12_DPR_scenario9.value; break;
                    case 0x9: LBIST_time = LBISTruntime_FB12_DPR_scenario10.value; break;
                    case 0xA: LBIST_time = LBISTruntime_FB12_DPR_scenario11.value; break;
                    case 0xB: LBIST_time = LBISTruntime_FB12_DPR_scenario12.value; break;
                    case 0xC: LBIST_time = LBISTruntime_FB12_DPR_scenario13.value; break;
                    case 0xD: LBIST_time = LBISTruntime_FB12_DPR_scenario14.value; break;
                    case 0xE: LBIST_time = LBISTruntime_FB12_DPR_scenario15.value; break;
                    default : LBIST_time = LBISTruntime_FB12_DPR_scenario16.value; break;
                }
            }
        }
        unsigned int MBIST_time = MBISTruntime.value;
        unsigned int BIST_time = ((mIsLBIST_active) ? LBIST_time : 0) + ((mIsMBIST_active) ? MBIST_time : 0);

        // Notify to ProcessFinishMethod after processing time
        mProcessFinishEvent.notify(BIST_time, SC_US);
    } else {
        // start_bist is de-asserted
        re_printf("info","start_bist is de-asserted.\n");
        // Cancel process wait time
        mProcessFinishEvent.cancel();
        // Assert end_bist
        if (mEnd_bistValue == false) {
            mEnd_bistValue = true;
            re_printf("info","end_bist is asserted.\n");
            mWriteOutputPortEvent.notify();
        }
    }
}//}}}


// BIST Process Finish
void Cbist_u2a::ProcessFinishMethod(void)
{//{{{
    // Compare reference registers and signature registers
    if (mIsLBIST_active) {
        if ((unsigned int)(*LBISTREF1) == (unsigned int)(*LBISTSIG1)) {
            (*BISTST)["LBIST1ST"] = 0;
        } else {
            (*BISTST)["LBIST1ST"] = 1;
        }
        if ((unsigned int)(*LBISTREF2) == (unsigned int)(*LBISTSIG2)) {
            (*BISTST)["LBIST2ST"] = 0;
        } else {
            (*BISTST)["LBIST2ST"] = 1;
        }
        //mIsLBIST_active = false;
    }
    if (mIsMBIST_active) {
        if ((unsigned int)(*MBISTREF1) == (unsigned int)(*MBISTSIG1)) {
            (*BISTST)["MBIST1ST"] = 0;
        } else {
            (*BISTST)["MBIST1ST"] = 1;
        }
        if ((unsigned int)(*MBISTREF2) == (unsigned int)(*MBISTSIG2)) {
            (*BISTST)["MBIST2ST"] = 0;
        } else {
            (*BISTST)["MBIST2ST"] = 1;
        }
        if (mIsMBIST3_enable) {
            if ((unsigned int)(*MBISTREF3) == (unsigned int)(*MBISTSIG3)) {
                (*BISTST)["MBIST3ST"] = 0;
            } else {
                (*BISTST)["MBIST3ST"] = 1;
            }
        }
        //mIsMBIST_active = false;
    }

    // Update sequencer status registers
    (*BSEQ0ST)["BISTEND"] = 1;
    (*BSEQ0ST)["CMPERR"]  =   ((bool)((*BISTST)["LBIST1ST"]) & mIsLBIST_active)
                            | ((bool)((*BISTST)["LBIST2ST"]) & mIsLBIST_active)
                            | ((bool)((*BISTST)["MBIST1ST"]) & mIsMBIST_active)
                            | ((bool)((*BISTST)["MBIST2ST"]) & mIsMBIST_active)
                            | ((bool)((*BISTST)["MBIST3ST"]) & mIsMBIST_active & mIsMBIST3_enable);
    (*BSEQ0STB)["BISTENDB"] = ~(*BSEQ0ST)["BISTEND"]; 
    (*BSEQ0STB)["CMPERRB"]  = ~(*BSEQ0ST)["CMPERR"];

    // Assert end_bist
    mEnd_bistValue = true;
    re_printf("info","end_bist is asserted.\n");
    mWriteOutputPortEvent.notify();
}//}}}

void Cbist_u2a::opbt_TESTSET_Change_Method(void)
{//{{{
    unsigned int test_set = opbt_TESTSET.read().to_uint();
    if (test_set == 0) {
        re_printf("error","opbt_TESTSET signal is invalid :0x%1x.\n", test_set);
    } else {
        re_printf("info","opbt_TESTSET signal :0x%1x.\n", test_set);
    }
}//}}}

void Cbist_u2a::opbt_LBISTSEL_Change_Method(void)
{//{{{
    re_printf("info","opbt_LBISTSEL signal :%d.\n", (unsigned int)opbt_LBISTSEL.read());
}//}}}

// Called by PCLKMethod() or SetCLKfreq handleCommand
void Cbist_u2a::SetCLKfreq(const std::string clk_name, const double clk_freq)
{//{{{
    // Clock name must be "pclk"
    if (clk_name == "pclk") {
        double pclk_period = 0;
        if (clk_freq == 0) {
            mIsZeroClock = true;
            //cancel all events when clock change to 0
            mProcessFinishEvent.cancel();
            mWriteOutputPortEvent.cancel();
            mReadPortEvent.cancel();
        } else {
            // Calculate the clock period to check the resolution
            mIsZeroClock = false;
            pclk_period = (double)(emNanoSecond / clk_freq);
            double time_unit = (double)(emNanoSecond / Cgeneral_reset::GetTimeResolution());
            if(time_unit > pclk_period){
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
                return;
            }
        }

        // Calculate again the latency
        for(unsigned int id = 0; id < emMaxId; id++){
            SetTlmParameter(id, true, pclk_period);
        }
        re_printf("info","pclk is set with a frequency as %lld.%06d.\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
    } else {
        re_printf("warning","Clock name = %s is invalid.\n", clk_name.c_str());
    }
}//}}}


// Setting latency for target parameters
void Cbist_u2a::SetTlmParameter(const unsigned int id, const bool is_ClockSetting, const double pclk_period)
{//{{{
    //vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,2>::tgt_get_param(id);

    if (is_ClockSetting) {
        sc_time new_clock(pclk_period, SC_NS);
        tgt_param.bus_clk = new_clock;
    } else {
        tgt_param.fw_req_phase = tlm::END_REQ;
    }

    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32,tlm::tlm_base_protocol_types,2>::tgt_set_param(id, tgt_param);
}//}}}


void Cbist_u2a::PCLKMethod(void)
{//{{{ 
    if(Cgeneral_reset::IsResetPeriod()) {
        return;
    }
    
    double pclk_freq = sc_dt::uint64_to_double(pclk.read());
    // Check & update the new clock
    SetCLKfreq("pclk", pclk_freq);
}//}}}


void Cbist_u2a::ResetMethod(void)
{//{{{
    // Check values of  reset signals
    bool is_PortReset = (reset1_n.read() == vpcl::emResetActive)
                        || (reset2_n.read() == vpcl::emResetActive)
                        || (reset_param.read() == vpcl::emResetActive);
    mIsSystemReset2 = (reset2_n.read() == vpcl::emResetActive)
                      && (reset1_n.read() != vpcl::emResetActive)
                      && (reset_param.read() != vpcl::emResetActive);
    // Call reset method
    Cgeneral_reset::Com_ResetMethod(is_PortReset);
}//}}}


void Cbist_u2a::AssertBISTReset(const std::string rst_name, const double delay, const double period)
{//{{{
    if (rst_name == "reset_n") {
    } else if (rst_name == "reset1_n") {
        mCmdReset = true;
//        mIsMBIST3_enable = true;  // fix #126256 
    } else if (rst_name == "reset2_n") {
        mCmdReset = true;
        mIsSystemReset2 = true;
//        mIsMBIST3_enable = true; // fix #126256
    } else if (rst_name == "reset_param") {
        mCmdReset = true;
//        mIsMBIST3_enable = false;   // MBIST3 is disable after Deep STOP Reset // fix #126256 
    } else {
        re_printf("warning","Reset name = %s is invalid.\n", rst_name.c_str());
    }
    // Call assert reset function of Cgeneral_reset class
    if (mCmdReset) {
        Cgeneral_reset::Com_AssertReset(delay, period);
    }
}//}}}


void Cbist_u2a::EnableReset(const bool is_active)
{//{{{
    if (is_active) {
        // Initialize variables
        mEnd_bistValue = true;

        // Cancel events
        mProcessFinishEvent.cancel();
        mWriteOutputPortEvent.cancel();
        mReadPortEvent.cancel();

        // Notify to initialize output ports
        mWriteOutputPortEvent.notify();
    } else {
        // Reset is negated, update input ports
        //if (!Cgeneral_reset::IsSimStarting()) {
            mReadPortEvent.notify();
        //}
    }

    // Initialize registers
    unsigned int temp = (unsigned int)(*BSEQ0SEL);
    Cbist_u2a_regif::EnableReset(is_active);
    if (mIsSystemReset2) { // BSEQ0SEL is not reset in System Reset2
        // fix issue SystemReset2 and DeepStop do not reset BSEQ0SEL
        (*BSEQ0SEL).EnableReset(false);
        (*BSEQ0SEL) = temp;
        mIsSystemReset2 = false;
    }
    if(mCmdReset == false){  // if reset by AssertReset command, tgt if isn't initialized.
        this->tgt_enable_reset(is_active);
    } else {
        mIsSystemReset2 = false;
        mCmdReset = false;
    }
}//}}}

// vim600: set foldmethod=marker :
