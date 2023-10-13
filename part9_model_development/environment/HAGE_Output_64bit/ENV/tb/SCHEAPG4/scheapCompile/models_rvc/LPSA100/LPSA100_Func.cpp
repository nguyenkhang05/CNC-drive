// ---------------------------------------------------------------------
// $Id: LPSA100_Func.cpp,v 1.6 2020/01/13 02:56:08 hadinhtran Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "LPSA100_Func.h"
#include "global.h"

#ifndef func_re_printf
#define func_re_printf mpLPSA100AgentController->get_fileline(__FILE__, __LINE__); mpLPSA100AgentController->_re_printf
#endif//func_re_printf

/*********************************
// Function     : LPSA100_Func
// Description  : Constructor of LPSA100_Func class
// Parameter    : 
//   name                    module name
// LPSA100AgentController        pointer of LPSA100AgentController class
// Return value : None
**********************************/
LPSA100_Func::LPSA100_Func (std::string name, LPSA100_AgentController* LPSA100AgentController):
                    BusSlaveFuncIf()
                    ,Clpsa100_regif(name, 32)
{//{{{ 
    assert(LPSA100AgentController != NULL);
    mpLPSA100AgentController = LPSA100AgentController;
    mpLPSA100AgentController->CommandInit(name);
    Clpsa100_regif::set_instance_name(name);
    mModuleName = name;
    
    Initialize();
}//}}}

/*********************************
// Function     : ~LPSA100_Func
// Description  : Destructor of LPSA100_Func class
// Parameter    : None
// Return value : None
**********************************/
LPSA100_Func::~LPSA100_Func()
{//{{{
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initialize when reset activated
// Parameter    : None
// Return value : None
**********************************/
void LPSA100_Func::Initialize()
{//{{{
    mEventFlag = false;
    mAPEN = false;
    mDPEN = false;
    mTJIS      = 0x0;
    mNUMDP     = 0x0;
    
    int tmp_val = 0x0;
    for(int y = 0; y < emNumINTTAUy; y++) {
        for(int x = 0; x < emNumINTTAUx; x++) {
            std::ostringstream inst_name;
            inst_name << "INTTAUJ" << y + emStartIndexINTTAUy << "I" << x;
            mINTTAUJMap.insert(std::pair<unsigned int, std::string>(tmp_val, inst_name.str()));
            tmp_val++;
        }
    }
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Enable/ Disable reset
// Parameter    : 
//    is_active   true: reset, false: not reset
// Return value : None
**********************************/
void LPSA100_Func::EnableReset(const bool is_active)
{//{{{
    Initialize();
    Clpsa100_regif::EnableReset(is_active);
}//}}}

/*********************************
// Function     : read
// Description  : Read API (target socket)
// Parameter    : 
//    address     read address
//    p_data      transaction data pointer
//    size        transaction data length
//    debug       true: debug mode; false: normal mode   
// Return value : None
**********************************/
void LPSA100_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Clear data buffer
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    
    // When clock = 0, or reset state, only debug master can read register 
    if ( (!mpLPSA100AgentController->CheckClockPeriod("CLK_LSB")) || (mpLPSA100AgentController->GetResetStatus()) ){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot read register when clock is 0 or in reset state.\n");
            return;
        }
    }

    if (!debug) {
        reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/*********************************
// Function     : write
// Description  : Write API (target socket)
// Parameter    : 
//    address           read address
//    p_data            transaction data pointer
//    size              transaction data length
//    debug             true: debug mode; false: normal mode   
// Return value : None
**********************************/
void LPSA100_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);

    // Not need to check reset status because regif class prevent write register in reset state
    // When clock = 0, only debug master write
    if (!mpLPSA100AgentController->CheckClockPeriod("CLK_LSB")){
        if (pExtInfo != NULL){
            if (! (debug && (pExtInfo->getPeId() == DEBUG_MASTER_BMID)) ){
                re_printf("warning", "Cannot write register when clock is 0.\n");
                return;
            }
        }else{
            re_printf("warning", "Cannot write register when clock is 0.\n");
            return;
        }
    }

    // write register if possible
    if (!debug) {
        reg_wr((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    } else {
        reg_wr_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
    }
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Handler for register IF
// Parameter    : 
//    args        input arguments 
// Return value : None
**********************************/
void LPSA100_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) { // Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") { // Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin() + 1);
                cmd.insert(cmd.begin() + 1, reg_p->name());
                msg_return = Clpsa100_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else { // Set handle command to regif class
            msg_return = Clpsa100_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            } else {
                /// There is no error occurs
            }
        }
    }
}//}}}

/*********************************
// Function     : SetDPDIMR
// Description  : Recorded input value
// Parameter    : 
//     input_value     input value need to recorded
//     time            times input port are read
// Return value : None
**********************************/
void LPSA100_Func::SetDPDIMR(uint32_t input_value, unsigned int time)
{//{{{
    if(time == 0) {                    /// Set first input value
        (*DPDIMR0) = input_value;
    } else if (time == 1) {            /// Set second input value
        (*DPDIMR1) = input_value;
    } else if (time == 2) {            /// Set third input value
        (*DPDIMR2) = input_value;
    } else if (time == 3) {            /// Set fourth input value
        (*DPDIMR3) = input_value;
    } else if (time == 4) {            /// Set fifth input value
        (*DPDIMR4) = input_value;
    } else if (time == 5) {            /// Set sixth input value
        (*DPDIMR5) = input_value;
    } else if (time == 6) {            /// Set seventh input value
        (*DPDIMR6) = input_value;
    } else {                           /// Set eighth input value
        (*DPDIMR7) = input_value;
    } 
}//}}}

/*********************************
// Function     : SetDINEVF
// Description  : Record result of comparsion
// Parameter    : 
//     comparison_result        comparison result
// Return value : None
**********************************/
void LPSA100_Func::SetDINEVF(bool comparison_result)
{//{{{
    (*EVFR) = (unsigned int)comparison_result;
}//}}}

/*********************************
// Function     : SetOperationStatus
// Description  : Update operation status
// Parameter    : 
//     value      status of operation
// Return value : None
**********************************/
void LPSA100_Func::SetOperationStatus(bool value)
{//{{{
    (*SOSTR) = (unsigned int)value;
}//}}}

/*********************************
// Function     : CheckTJIS
// Description  : Check input value whether accepted start trigger or not
// Parameter    : None
// Return value : 
//       true     accepted
//       false    not accepted
**********************************/
bool LPSA100_Func::CheckTJIS(std::string inst_name)
{//{{{
    std::string trigger_name;
    std::map<unsigned int, std::string>::iterator it;
    it = mINTTAUJMap.find(mTJIS);
    trigger_name = mINTTAUJMap[mTJIS];
    
    if(trigger_name == inst_name) {
        return true;
    }
    return false;
}//}}}

/*********************************
// Function     : GetOperationStatus
// Description  : Get value SOF bit of SOSTR register
// Parameter    : None
// Return value : Value of SOF bit
**********************************/
bool LPSA100_Func::GetOperationStatus()
{//{{{
    return (bool)(*SOSTR)["SOF"];
}//}}}

/*********************************
// Function     : GetNUMDP
// Description  : Get number of times the port is read in digital input mode
// Parameter    : None
// Return value : Number of times the port is read
**********************************/
unsigned int LPSA100_Func::GetNUMDP()
{//{{{
    return mNUMDP;
}//}}}

/*********************************
// Function     : GetDnEN
// Description  : Get value of compare target bits
// Parameter    : 
//     time       time input port are read
// Return value : Value of of compare target bits with <time>-th input value
**********************************/
unsigned int LPSA100_Func::GetDnEN(unsigned int time)
{//{{{
    if(time == 0) {              /// Get first input value
        return (*DPSELR0);
    } else if (time == 1) {      /// Get second input value
        return (*DPSELRM)["D1EN"];
    } else if (time == 2) {      /// Get third input value
        return (*DPSELRM)["D2EN"];
    } else if (time == 3) {      /// Get fourth input value
        return (*DPSELRM)["D3EN"];
    } else if (time == 4) {      /// Get fifth input value
        return (*DPSELRM)["D4EN"];
    } else if (time == 5) {      /// Get sixth input value
        return (*DPSELRH)["D5EN"];
    } else if (time == 6) {      /// Get seventh input value
        return (*DPSELRH)["D6EN"];
    } else {                     /// Get eighth input value
        return (*DPSELRH)["D7EN"];
    }
}//}}}

/*********************************
// Function     : GetDn
// Description  : Get expected value
// Parameter    : 
//     time       time input port is read
// Return value : <time>-th expected value
**********************************/
unsigned int LPSA100_Func::GetDn(unsigned int time)
{//{{{
    if(time == 0) {             /// Get first input value
        return (*DPDSR0);
    } else if (time == 1) {     /// Get second input value
        return (*DPDSRM)["D1"];
    } else if (time == 2) {     /// Get third input value
        return (*DPDSRM)["D2"];
    } else if (time == 3) {     /// Get fourth input value
        return (*DPDSRM)["D3"];
    } else if (time == 4) {     /// Get fifth input value
        return (*DPDSRM)["D4"];
    } else if (time == 5) {     /// Get sixth input value
        return (*DPDSRH)["D5"];
    } else if (time == 6) {     /// Get seventh input value
        return (*DPDSRH)["D6"];
    } else {                    /// Get eighth input value
        return (*DPDSRH)["D7"];
    }
}//}}}

/*********************************
// Function     : GetDnM
// Description  : Get recorded input value
// Parameter    : 
//     time     times input port is read
// Return value : Expected value Dn
**********************************/
unsigned int LPSA100_Func::GetDnM(unsigned int time)
{//{{{
    if(time == 0) {             /// Get first input value
        return (*DPDIMR0);
    } else if (time == 1) {     /// Get second input value
        return (*DPDIMR1);
    } else if (time == 2) {     /// Get third input value
        return (*DPDIMR2);
    } else if (time == 3) {     /// Get fourth input value
        return (*DPDIMR3);
    } else if (time == 4) {     /// Get fifth input value
        return (*DPDIMR4);
    } else if (time == 5) {     /// Get sixth input value
        return (*DPDIMR5);
    } else if (time == 6) {     /// Get seventh input value
        return (*DPDIMR6);
    } else {                    /// Get eighth input value
        return (*DPDIMR7);
    }
}//}}}

/*********************************
// Function     : GetCNT0
// Description  : Get value of the bits set stabilization time in digital input mode
// Parameter    : None
// Return value : Value of [CNT07:CNT00] bits from position [7:0] of CNTVAL register
**********************************/
unsigned int LPSA100_Func::GetCNT0()
{//{{{
    return (*CNTVAL) & 0xFF;
}//}}}

/*********************************
// Function     : GetCNT1
// Description  : Get value of the bits set stabilization time in analog input mode
// Parameter    : None
// Return value : Value of [CNT17:CNT10] bits from position [15:8] of CNTVAL register
**********************************/
unsigned int LPSA100_Func::GetCNT1()
{//{{{
    return ((*CNTVAL) >> 8) & 0xFF;
}//}}}

/*********************************
// Function     : cb_SCTLR_DPEN
// Description  : Callback function of SCTLR register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_SCTLR_DPEN(RegCBstr str)
{//{{{
    /// Get value [TJIS2 TJIS1 TJIS0] in postion [7 3 2]
    // Get bit TJIS1 and TJIS0 in position 2 & 3 of register.
    // and put them in position 1 & 0 of tjis10 variable
    unsigned int tjis10 = ((*SCTLR) >> 2) & 0x3;
    
    // Get bit TJIS2 in position 7 and put it in position 2 of tjis2 variable
    unsigned int tjis2 =  ((*SCTLR) >> 5) & 0x4;
    
    // Get value [TJIS2 TJIS1 TJIS0] and put to mTJIS variable in postion [2 1 0]
    mTJIS = tjis2 | tjis10;
    
    /// Get value [NUMDP2 NUMDP1 NUMDP0] in position [6 5 4] of register 
    mNUMDP = ((*SCTLR) >> 4) & 0x7;
    
    /// Get/ Check enable input mode bit
    // Get value DPEN bit in position 0 of register
    mDPEN = (*SCTLR) & 0x1;
    
    // Get value APEN bit in position 1 of register
    mAPEN = ((*SCTLR) >> 1) & 0x1;
    
    // Get previous value DPEN/ APEN bit of regsiter
    unsigned int preDPEN = str.pre_data & 0x1;
    unsigned int preAPEN = (str.pre_data >> 1) & 0x1;
    
    // If value of DPEN/ APEN is changed, user must write same value to other bits (NUMDP, TJIS) to keep expecting setting
    if (preDPEN != mDPEN) {
        func_re_printf("warning", "DPEN is changed. User must write same value to other bits to keep expecting setting\n");
        if(preDPEN == 0x0) {
            mpLPSA100AgentController->EnableDigitalInputMode();
        } else {
            mpLPSA100AgentController->StopDigitalInputMode();
        }
    }
    if (preAPEN != mAPEN) {
        func_re_printf("warning", "APEN is changed. User must write same value to other bits to keep expecting setting\n");
        if(preAPEN == 0x0) {
            mpLPSA100AgentController->EnableAnalogInputMode();
        } else {
            mpLPSA100AgentController->StopAnalogInputMode();
        }
    }
}//}}}

/*********************************
// Function     : cb_EVFR_DINEVF
// Description  : Callback function of EVFR register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_EVFR_DINEVF(RegCBstr str)
{//{{{
    /// DINEVF flag is cleared by write 0 to EVFR register
    if(str.data == 0x0) {
        (*EVFR) = 0x0; 
        mpLPSA100AgentController->NotifyEventFlagCleared();
    } else {
        /// Only 0 can be written to clear the bit
        (*EVFR) = 0x0; 
        func_re_printf("warning", "Only 0 can be written to clear the bit\n");
    }
}//}}}

/*********************************
// Function     : cb_DPSELR0_D0EN
// Description  : Callback function of DPSELR0 register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_DPSELR0_D0EN(RegCBstr str)
{//{{{
    /// Cannot write to regiater when operation is in process
    if((bool)(*SOSTR)["SOF"]) {
        (*DPSELR0) = str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_DPSELRM_D1EN
// Description  : Callback function of DPSELRM register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_DPSELRM_D1EN(RegCBstr str)
{//{{{
    /// Cannot write to regiater when operation is in process
    if((bool)(*SOSTR)["SOF"]) {
        (*DPSELRM) = str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_DPSELRH_D5EN
// Description  : Callback function of DPSELRH register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_DPSELRH_D5EN(RegCBstr str)
{//{{{
    /// Cannot write to regiater when operation is in process
    if((bool)(*SOSTR)["SOF"]) {
        (*DPSELRH) = str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_DPDSR0_D0
// Description  : Callback function of DPDSR0 register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_DPDSR0_D0(RegCBstr str)
{//{{{
    /// Cannot write to regiater when operation is in process
    if((bool)(*SOSTR)["SOF"]) {
        (*DPDSR0) = str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_DPDSRM_D1
// Description  : Callback function of DPDSRM register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_DPDSRM_D1(RegCBstr str)
{//{{{
    /// Cannot write to regiater when operation is in process
    if((bool)(*SOSTR)["SOF"]) {
        (*DPDSRM) = str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_DPDSRH_D5
// Description  : Callback function of DPDSRH register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_DPDSRH_D5(RegCBstr str)
{//{{{
    /// Cannot write to regiater when operation is in process
    if((bool)(*SOSTR)["SOF"]) {
        (*DPDSRH) = str.pre_data;
    }
}//}}}

/*********************************
// Function     : cb_CNTVAL_CNT0
// Description  : Callback function of CNTVAL register
// Parameter    : 
//    str         infomation of register
// Return value : None
**********************************/
void LPSA100_Func::cb_CNTVAL_CNT0(RegCBstr str)
{//{{{
    /// Cannot write to regiater when operation is in process
    if((bool)(*SOSTR)["SOF"]) {
        (*CNTVAL) = str.pre_data;
    }
}//}}}

// vim600: set foldmethod=marker :

