//----------------------------------------------------------------------
//* File: DummyPeripheralRvc_Func.cpp
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of DummyPeripheralRvc_Func.cpp
// Ref: {SPIDCTL_UT_VRFCopyRight_001}
#include "DummyPeripheralRvc_Func.h"
#include "DummyPeripheralRvc.h"

// #define DEBUG_SIM_MESSAGE_RVC   //used to print DEBUG message in this file
/*********************************
// Function     : DummyPeripheralRvc_Func
// Description  : Constructor of DummyPeripheralRvc_Func class
// Parameter    :
// name            Module name
// Parent          Indicate base class
// Return value : None
**********************************/
DummyPeripheralRvc_Func::DummyPeripheralRvc_Func(std::string name,
                                                 DummyPeripheralRvc_Base *DummyPeripheralRvc_Base_ptr, DummyPeripheralRvc *parent):
                                                 Cdummyperipheralrvc_regif(name,32),
                                                 BusSlaveFuncIf()
{
    Cdummyperipheralrvc_regif::set_instance_name(name);
    mDummyPeripheralRvc_Base = DummyPeripheralRvc_Base_ptr;
    cur_CLK_LSB_L = 0;

}

/*********************************
// Function     : ~DummyPeripheralRvc_Func
// Description  : Destructor of DummyPeripheralRvc_Func class
// Parameter    : None
// Return value : None
**********************************/
DummyPeripheralRvc_Func::~DummyPeripheralRvc_Func()
{
}

/*********************************
// Function     : read
//                - virtual function in BusSlaveFuncIf
// Description  : Access to read value of Dummy Peripheral's registers
// Parameter    :
//      offsetAddress   Register's address offset
//      trans           Transaction
//      t               Access time
//      debug           Debug access mode
// Return value : None
**********************************/
void DummyPeripheralRvc_Func::read(unsigned int offsetAddress, TlmBasicPayload &trans, sc_time *t, bool debug)
{
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    if (debug == false) {
        reg_rd((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}

/*********************************
// Function     : write
//                - virtual function in BusSlaveFuncIf
// Description  : Access to write value to Dummy Master's registers
// Parameter    :
//      offsetAddress   Register's address offset
//      trans           Transaction
//      t               Access time
//      debug           Debug access mode
// Return value : None
**********************************/
void DummyPeripheralRvc_Func::write(unsigned int offsetAddress, TlmBasicPayload &trans, sc_time *t, bool debug)
{
    if (debug == false) {
        reg_wr((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(),trans.get_data_ptr(),trans.get_data_length());
    }
}


/*********************************
// Function     : updateReg
// Description  : Callback function when input signal is received
//                + Store value of input signal into register
//                  (Receive signals from output ports of target model)
// Parameter    :
//      reg_name        Register's name
//      Index           Register's index
//      value           Register's value
// Return value : None
**********************************/
void DummyPeripheralRvc_Func::updateReg(std::string reg_name, unsigned int Index, unsigned int value){
    //your code should be there
    if ( reg_name == "BMSPIDOUT_REG"){
       (*BMSPIDOUT_REG[Index]) = (value & 0x1F);
    }
}

/*********************************
// Function     : cb_JUDGE_REG_JUDGE
// Description  : Callback function when JUDGE_REG register is written
//                + Dump result of simulation (Pass/Fail)
// Parameter    :
//      str             Register's callback string (0: Fail; 1: Pass; Others: Ignored)
// Return value : None
**********************************/
void DummyPeripheralRvc_Func::cb_JUDGE_REG_JUDGE(RegCBstr str) {
    if (str.data == 0) {
        printf("--------------------------------\n");
        printf("-          TM is FAIL          -\n");
        printf("--------------------------------\n");
    } else if (str.data == 1) {
        printf("--------------------------------\n");
        printf("-          TM is PASS          -\n");
        printf("--------------------------------\n");
    }
}

void DummyPeripheralRvc_Func::cb_RESET_REG_RESETPORT(RegCBstr str){
    mDummyPeripheralRvc_Base->SetOutputSignal(DummyPeripheralRvc_Base::emRESET,str.data);
}
void DummyPeripheralRvc_Func::cb_PCLK_LOW_REG_CLK_LOW(RegCBstr str){
    if (cur_CLK_LSB_L != (*PCLK_LOW_REG)) {
        mDummyPeripheralRvc_Base->SetOutputSignal(DummyPeripheralRvc_Base::emCLK_LSB_H,(*PCLK_HIGH_REG));
        mDummyPeripheralRvc_Base->SetOutputSignal(DummyPeripheralRvc_Base::emCLK_LSB_L,(*PCLK_LOW_REG));
        cur_CLK_LSB_L = (*PCLK_LOW_REG);
    }
}