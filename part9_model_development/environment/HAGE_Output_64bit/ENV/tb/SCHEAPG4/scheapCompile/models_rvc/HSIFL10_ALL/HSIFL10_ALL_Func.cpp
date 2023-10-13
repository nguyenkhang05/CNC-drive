// ---------------------------------------------------------------------
// $Id: HSIFL10_ALL_Func.cpp,v 1.10 2020/04/08 07:44:03 quynhnhunguyen Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "HSIFL10_ALL_Func.h"
#include "HSIFL10_ALL_AgentController.h"
#include "global.h"


///Constructor of HSIFL10_ALL_Func class
HSIFL10_ALL_Func::HSIFL10_ALL_Func (std::string name, HSIFL10_ALL_AgentController *HSIFL10_ALL_AgentControllerPtr):
                Chsifl10_all_regif(name, 32)
{//{{{
    assert(HSIFL10_ALL_AgentControllerPtr != NULL);
    mHSIFL10_ALL_AgentController = HSIFL10_ALL_AgentControllerPtr;
    mHSIFL10_ALL_AgentController->CommandInit(name);
    Chsifl10_all_regif::set_instance_name(name);
    mModuleName = name;
}//}}}

///Destructor of HSIFL10_ALL_Func class
HSIFL10_ALL_Func::~HSIFL10_ALL_Func (void)
{//{{{
}//}}}

///Reset operation of HSIFL10_ALL_Func class
void HSIFL10_ALL_Func::Reset (const bool is_active)
{//{{{
    Chsifl10_all_regif::EnableReset(is_active);
}//}}}

///*********************************
/// Function     : RegisterHandler
/// Description  : Handle commands of register IF class
///*********************************
void HSIFL10_ALL_Func::RegisterHandler (const std::vector<std::string>& args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Chsifl10_all_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else if (args[1] == "DumpRegisterRW") {
                msg_return = Chsifl10_all_regif::reg_handle_command(args);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
        } else {
            msg_return = Chsifl10_all_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HSIFL10_ALL_Func::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    assert(t != NULL);
    bool isAccess = false;
    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension* ext = (TlmG3mExtension*)0;
    trans.get_extension(ext);

    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // Get extension info
    if ((false == mHSIFL10_ALL_AgentController->CheckClockPeriod("PCLK")) || (true == mHSIFL10_ALL_AgentController->GetResetStatus())) {
        // Software debugger
        if ((ext != NULL) && ((true == debug) && (DEBUG_MASTER_BMID == ext->getPeId()))) {
            isAccess = true;
        } else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
        }
    } else {
        isAccess = true;
    }

    if (isAccess == true) {    
        unsigned int addr = (unsigned int)trans.get_address() & 0xFF;
        memset(trans.get_data_ptr(), 0, trans.get_data_length());
        if (!debug) {
            reg_rd((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_rd_dbg((unsigned int)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HSIFL10_ALL_Func::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    assert(t != NULL);
    bool isAccess = false;
    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension* ext = (TlmG3mExtension*)0;
    trans.get_extension(ext);
    
    // Get extension info
    if (false == mHSIFL10_ALL_AgentController->CheckClockPeriod("PCLK")) {
        // Software debugger
        if ((ext != NULL) && ((true == debug) && (DEBUG_MASTER_BMID == ext->getPeId()))) {
            isAccess = true;
        } else {
            re_printf("warning", "Cannot write register when clock is 0.\n");
        }
    } else {
        isAccess = true;
    }

    if (isAccess == true) {  
        unsigned int address = (unsigned int)trans.get_address();
        if (!debug) {
            reg_wr(address, trans.get_data_ptr(), trans.get_data_length());
        } else {
            reg_wr_dbg(address, trans.get_data_ptr(), trans.get_data_length());
        }
    }
}//}}}

void HSIFL10_ALL_Func::cb_RHSIFMDCR_CTSEN(RegCBstr str)
{//{{{
    //Get value MDCR, CLKSEL bits of RHSIFMDCR register
    uint8_t MDCR_val = (uint8_t)(*RHSIFMDCR)["MST"];
    uint8_t CLKSEL_val = (uint8_t)(*RHSIFMDCR)["CLKSEL"];
    
    if (((mHSIFL10_ALL_AgentController->GetCCLKFreq() != 0) && (MDCR_val == 0x1))
        || ((mHSIFL10_ALL_AgentController->GetRefclk_inFreq() != 0) && (MDCR_val == 0x0) && (CLKSEL_val == 0))
        || ((mHSIFL10_ALL_AgentController->GetRefclk_inFreq() != 0) && (MDCR_val == 0x0) && (CLKSEL_val == 1))
    ) {
    
    //if ( mHSIFL10_ALL_AgentController->GetCCLKFreq() != 0 && MDCR_val == 0x1 ) { 
        //Calculate hsif_refclk_out clock
        if (MDCR_val == 0x1) {
            if (CLKSEL_val == 0x1) {
                mHSIFL10_ALL_AgentController->SetRefCLKOut(10000000);
            }
            else { // CLKSEL_val == 0x0
                mHSIFL10_ALL_AgentController->SetRefCLKOut(20000000);
            }
        }
        
         
        //In case, PLL is ON, then generate a warning that should set this MST when PLL is OFF
        uint32_t RHSIFPCR_val = (uint32_t)(*RHSIFPCR);
        uint8_t PLLStandby_val = RHSIFPCR_val & 0x1;
        if (PLLStandby_val == 0) {
            re_printf("warning", "Should set bits as MST, CLKSEL in RHSIFMDCR register when PLL is standby.\n");
        }
        
        int8_t  CTSEN_val = bit_select(str.data
                                        ,(*RHSIFMDCR)["CTSEN"].mStartAddr
                                        ,(*RHSIFMDCR)["CTSEN"].mEndAddr);
        if (CTSEN_val == 1) {
            //Update status of flow control type is the automatic
            mHSIFL10_ALL_AgentController->SetFlowControlManualType(mHSIFL10_ALL_AgentController->emAutoFCType);
        }
        else {
            //Update status of flow control type is manual
            mHSIFL10_ALL_AgentController->SetFlowControlManualType(mHSIFL10_ALL_AgentController->emFlowControlManualTypeNum);
        }
    }
    else {
        re_printf("warning", "Communication Clock (CCLK) is zero when operation as master or Reference Clock In is zero when operation as slave.\n");
    }
}//}}}

void HSIFL10_ALL_Func::cb_RHSIFSPCR_FMBR(RegCBstr str)
{//{{{
    uint32_t PLLStandby_val = (uint32_t)(*RHSIFPCR) & 0x1;
    if (PLLStandby_val == 0) {
        re_printf("warning", "Should set bits as RXSP, TXSP, FMBR in RHSIFSPCR register when PLL is standby.\n");
    }
    //When write FMBR is 0b11 then ignore
    uint8_t  val_cur_FMBR = bit_select(str.data
                                    ,(*RHSIFSPCR)["FMBR"].mStartAddr
                                    ,(*RHSIFSPCR)["FMBR"].mEndAddr);
    uint8_t val_pre_FMBR = bit_select(str.pre_data
                                          ,(*RHSIFSPCR)["FMBR"].mStartAddr
                                          ,(*RHSIFSPCR)["FMBR"].mEndAddr);
    //Get TXSP/RXSP value
    uint8_t val_TXSP =  bit_select(str.data
                                    ,(*RHSIFSPCR)["TXSP"].mStartAddr
                                    ,(*RHSIFSPCR)["TXSP"].mEndAddr);
    uint8_t val_RXSP =  bit_select(str.data
                                    ,(*RHSIFSPCR)["RXSP"].mStartAddr
                                    ,(*RHSIFSPCR)["RXSP"].mEndAddr);
    //Get TXLPBK value of RHSIFTMDCR register
    uint8_t val_TXLPBK = (this->GetRHSIFTMDCR() >> 1) & 0x1;
                                    
    if (val_cur_FMBR == 0x3) {
        (*RHSIFSPCR)["FMBR"] = val_pre_FMBR;
    }
    else if (val_cur_FMBR == 0x2) {
        std::string version_name_str = mHSIFL10_ALL_AgentController->GetVersionName();
        if (version_name_str == "FCC1") {
            (*RHSIFSPCR)["FMBR"] = val_pre_FMBR;
        } 
    }
    
    if (val_TXSP != val_RXSP) {
        if (val_TXLPBK == 1) {
            re_printf("warning", "Please set the same value to RHSIFnSPCR.TXSP and RHSIFnSPCR.RXSP. Unless the same BaudRate is set to both transmitter/receiver, the communication will not be done normally.\n");
        }
    }
}//}}}
void HSIFL10_ALL_Func::cb_RHSIFTMDCR_RXLPBK(RegCBstr str)
{//{{{
    uint8_t  val_cur_TXLPBK = bit_select(str.data
                                    ,(*RHSIFTMDCR)["TXLPBK"].mStartAddr
                                    ,(*RHSIFTMDCR)["TXLPBK"].mEndAddr);
    uint8_t val_pre_TXLPBK = bit_select(str.pre_data
                                          ,(*RHSIFTMDCR)["TXLPBK"].mStartAddr
                                          ,(*RHSIFTMDCR)["TXLPBK"].mEndAddr);
    uint8_t  val_cur_RXLPBK = bit_select(str.data
                                    ,(*RHSIFTMDCR)["RXLPBK"].mStartAddr
                                    ,(*RHSIFTMDCR)["RXLPBK"].mEndAddr);
    uint8_t val_pre_RXLPBK = bit_select(str.pre_data
                                          ,(*RHSIFTMDCR)["RXLPBK"].mStartAddr
                                          ,(*RHSIFTMDCR)["RXLPBK"].mEndAddr);
    uint8_t  val_cur_CKTM = bit_select(str.data
                                    ,(*RHSIFTMDCR)["CKTM"].mStartAddr
                                    ,(*RHSIFTMDCR)["CKTM"].mEndAddr);
    uint8_t val_pre_CKTM = bit_select(str.pre_data
                                          ,(*RHSIFTMDCR)["CKTM"].mStartAddr
                                          ,(*RHSIFTMDCR)["CKTM"].mEndAddr);
    
    if ((val_cur_TXLPBK & val_cur_RXLPBK & val_cur_CKTM) == 0x1) {
        (*RHSIFTMDCR)["TXLPBK"] = val_pre_TXLPBK;
        (*RHSIFTMDCR)["RXLPBK"] = val_pre_RXLPBK;
        (*RHSIFTMDCR)["CKTM"] = val_pre_CKTM;
        re_printf("warning", "Can not set TXLPBK and RXLPBK and CKTM bits to 1 at the same time.\n");
    }
    else if ((val_cur_TXLPBK & val_cur_RXLPBK ) == 0x1) {
        (*RHSIFTMDCR)["TXLPBK"] = val_pre_TXLPBK;
        (*RHSIFTMDCR)["RXLPBK"] = val_pre_RXLPBK;
        re_printf("warning", "Can not set TXLPBK and RXLPBK bits to 1 at the same time.\n");
    }
    else if ((val_cur_TXLPBK & val_cur_CKTM ) == 0x1) {
        (*RHSIFTMDCR)["TXLPBK"] = val_pre_TXLPBK;
        (*RHSIFTMDCR)["CKTM"] = val_pre_CKTM;
        re_printf("warning", "Can not set TXLPBK and CKTM bits to 1 at the same time.\n");
    }
    else if ((val_cur_RXLPBK & val_cur_CKTM ) == 0x1) {
        (*RHSIFTMDCR)["RXLPBK"] = val_pre_RXLPBK;
        (*RHSIFTMDCR)["CKTM"] = val_pre_CKTM;
        re_printf("warning", "Can not set RXLPBK and CKTM bits to 1 at the same time.\n");
    }
    else if ((val_cur_TXLPBK == 0x0) && (val_cur_RXLPBK == 0x0)){
        mHSIFL10_ALL_AgentController->SetLoopBackMode(false);
        if ((mHSIFL10_ALL_AgentController->GetTxDataReady() == false)
            && ((bool)val_pre_RXLPBK == true)
        ) {
            mHSIFL10_ALL_AgentController->SetTxDataReady(true);
        }
        re_printf("info", "The operatior of model in normal mode.\n");
    }
    else {
        //Get TXSP & RXSP of RHSIFSPCR register
        uint8_t TXSP_val = (this->GetRHSIFSPCR() >> 8) & 0x1;
        uint8_t RXSP_val = this->GetRHSIFSPCR() & 0x1;
        if (val_cur_TXLPBK == 0x1) {
            if (TXSP_val != RXSP_val) {
                re_printf("warning", "Please set the same value to RHSIFnSPCR.TXSP and RHSIFnSPCR.RXSP. Unless the same BaudRate is set to both transmitter/receiver, the communication will not be done normally.\n");
            }
        }
        
        mHSIFL10_ALL_AgentController->SetLoopBackMode(true);
        re_printf("info", "The operatior of model in loopback mode.\n");
    }
}//}}}

void HSIFL10_ALL_Func::cb_RHSIFICCR_ITRG(RegCBstr str)
{//{{{
    //Check setting Transmitter (Master)
    uint32_t mode_val = (uint32_t)(*RHSIFMDCR)["MST"];
    uint8_t val_clk_sel = (this->GetRHSIFMDCR() >> 1) & 0x1;
    if (((mHSIFL10_ALL_AgentController->GetCCLKFreq() != 0) && (mode_val == 0x1))
        || ((mHSIFL10_ALL_AgentController->GetRefclk_inFreq() != 0) && (mode_val == 0x0) && (val_clk_sel == 0))
        || ((mHSIFL10_ALL_AgentController->GetRefclk_inFreq() != 0) && (mode_val == 0x0) && (val_clk_sel == 1))
    ) { 
        //Transmitter (Master) is set
        if (mode_val == 0x1) {
            uint8_t ITRG_val = bit_select(str.data
                                         ,(*RHSIFICCR)["ITRG"].mStartAddr
                                         ,(*RHSIFICCR)["ITRG"].mEndAddr);
            //In case ICLC command                             
            if (ITRG_val == 0x1) {
                //Get Tx enable value
                uint8_t tx_val = (uint8_t)(*RHSIFTXRXCR)["TXEN"];
                //Tx is enable
                if (tx_val == 1) {
                    //Check exist the previous ICLC
                    bool isExist = mHSIFL10_ALL_AgentController->CheckExistPreFrameTypeBuffer(mHSIFL10_ALL_AgentController->emICLCFrameL1);
                    if (isExist == false) {
                        //Insert ICLC frame type to buffer
                        mHSIFL10_ALL_AgentController->StoreFrameTypeIntoBuffer(mHSIFL10_ALL_AgentController->emICLCFrameL1);
                        //Change to process check priority in buffer
                        mHSIFL10_ALL_AgentController->TransProcessMethod();
                    }
                    else {
                        //Exist same frame type previously
                        re_printf("info", "Exist ICLC frame type previously.\n");
                        //Restore the previous ICLC command
                        uint8_t val_pre_ITRG = bit_select(str.pre_data
                                              ,(*RHSIFICCR)["ITRG"].mStartAddr
                                              ,(*RHSIFICCR)["ITRG"].mEndAddr);
                        uint8_t val_pre_PLD = bit_select(str.pre_data
                                              ,(*RHSIFICCR)["PLD"].mStartAddr
                                              ,(*RHSIFICCR)["PLD"].mEndAddr);
                        (*RHSIFICCR)["ITRG"] = val_pre_ITRG;
                        (*RHSIFICCR)["PLD"] = val_pre_PLD;
                    }
                }
                else {
                    re_printf("warning", "Tx of model is not enable.\n");
                }
            } 
            
        }
    }
    else {
        re_printf("warning", "Communication Clock (CCLK) is zero when operation as master or Reference Clock In is zero when operation as slave.\n");
    }
    
}//}}}
void HSIFL10_ALL_Func::cb_RHSIFCCR_CTRG(RegCBstr str)
{//{{{
    //Get operation mode
    uint32_t val = this->GetRHSIFSPCR(); 
    bool isFastModeTrans    = (val >> 8) & 0x1;
    uint8_t PLLSTBY_val = this->GetRHSIFPCR() & 0x01;
    uint8_t val_clk_sel = (this->GetRHSIFMDCR() >> 1) & 0x1;
    uint8_t MDCR_val = (uint8_t)(*RHSIFMDCR)["MST"];
    
    if (((isFastModeTrans == 1) && (PLLSTBY_val == 0) && (mHSIFL10_ALL_AgentController->GetRefclk_inFreq() != 0) && (MDCR_val == 0x0))
        ||   mHSIFL10_ALL_AgentController->GetCCLKFreq() != 0 && MDCR_val == 0x1
        ||   ((mHSIFL10_ALL_AgentController->GetRefclk_inFreq() != 0) && val_clk_sel == 0x0 && MDCR_val == 0x0)
        ||   ((mHSIFL10_ALL_AgentController->GetRefclk_inFreq() != 0) && val_clk_sel == 0x1 && MDCR_val == 0x0)
       ) { 
        uint8_t CTRG_val   = bit_select(str.data
                                        ,(*RHSIFCCR)["CTRG"].mStartAddr
                                        ,(*RHSIFCCR)["CTRG"].mEndAddr);
        if (CTRG_val == 0x1) {
            //Get Tx enable value
            uint8_t tx_val = (uint8_t)(*RHSIFTXRXCR)["TXEN"];
            //Tx is enable
            if (tx_val == 1) {
                //Check exist the previous CTS
                bool isExist = mHSIFL10_ALL_AgentController->CheckExistPreFrameTypeBuffer(mHSIFL10_ALL_AgentController->emCTSFrameL1);
                if (isExist == false) {
                    //Check automatic flow control or manual flow control
                    int8_t  CTSEN_val = (*RHSIFMDCR)["CTSEN"];
                    if (CTSEN_val == 0) {
                        //Insert CTS frame type to buffer
                        mHSIFL10_ALL_AgentController->StoreFrameTypeIntoBuffer(mHSIFL10_ALL_AgentController->emCTSFrameL1);
                        //Update flow control manual type
                        mHSIFL10_ALL_AgentController->SetFlowControlManualType(mHSIFL10_ALL_AgentController->emFCMTypeWithCTSFrame);
                        //Change to process check priority in buffer
                        mHSIFL10_ALL_AgentController->TransProcessMethod();
                    }
                    else {
                        re_printf("info", "Automatic Flow control is enable.\n");
                    }
                }
                else {
                    //Exist same frame type previously
                    re_printf("info", "Exist CTS frame type previously.\n");
                    //Restore the previous CTS command
                    uint8_t val_pre_CTRG = bit_select(str.pre_data
                                          ,(*RHSIFCCR)["CTRG"].mStartAddr
                                          ,(*RHSIFCCR)["CTRG"].mEndAddr);
                    uint8_t val_pre_CTS = bit_select(str.pre_data
                                          ,(*RHSIFCCR)["CTS"].mStartAddr
                                          ,(*RHSIFCCR)["CTS"].mEndAddr);
                    uint8_t val_pre_PLD = bit_select(str.pre_data
                                          ,(*RHSIFCCR)["PLD"].mStartAddr
                                          ,(*RHSIFCCR)["PLD"].mEndAddr);
                    (*RHSIFCCR)["CTRG"] = val_pre_CTRG;
                    (*RHSIFCCR)["CTS"] = val_pre_CTS;
                    (*RHSIFCCR)["PLD"] = val_pre_PLD;
                }
            }
            else {
                re_printf("warning", "Tx of model is not enable.\n");
            }
        }
    }
    else {
        re_printf("warning", "Communication Clock (CCLK) is zero when operation as master or Reference Clock In is zero when operation as slave.\n");
    }
}//}}}
void HSIFL10_ALL_Func::cb_RHSIFTXCMPSC_TCCL2(RegCBstr str)
{//{{{
    uint8_t TCCIC_val   = bit_select(str.data
                                    ,(*RHSIFTXCMPSC)["TCCIC"].mStartAddr
                                    ,(*RHSIFTXCMPSC)["TCCIC"].mEndAddr);
    uint8_t TCCCT_val = bit_select(str.data
                                    ,(*RHSIFTXCMPSC)["TCCCT"].mStartAddr
                                    ,(*RHSIFTXCMPSC)["TCCCT"].mEndAddr);
    uint8_t TCCL2_val = bit_select(str.data
                                    ,(*RHSIFTXCMPSC)["TCCL2"].mStartAddr
                                    ,(*RHSIFTXCMPSC)["TCCL2"].mEndAddr);
                                    
    if (TCCIC_val == 1) {
        //Clear the RHSIFTXCMPST.TCIC bit
        (*RHSIFTXCMPST)["TCIC"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txcmp");
    }
    
    if (TCCCT_val == 1) {
        //Clear the RHSIFTXCMPST.TCCT bit
        (*RHSIFTXCMPST)["TCCT"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txcmp");
    }
    
    if (TCCL2_val == 1) {
        //Clear the RHSIFTXCMPST.TCL2 bit
        (*RHSIFTXCMPST)["TCL2"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txcmp");
    }
    
}//}}}

void HSIFL10_ALL_Func::cb_RHSIFTXERRSC_TERCSZ(RegCBstr str)
{//{{{
    uint8_t TERCCT1_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCT1"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCT1"].mEndAddr);
    uint8_t TERCCT2_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCT2"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCT2"].mEndAddr);
    uint8_t TERCCT8_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCT8"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCT8"].mEndAddr);
    uint8_t TERCCT9_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCT9"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCT9"].mEndAddr);
    uint8_t TERCCTA_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCTA"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCTA"].mEndAddr);
    uint8_t TERCCTB_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCTB"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCTB"].mEndAddr);
    uint8_t TERCCTC_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCTC"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCTC"].mEndAddr);
    uint8_t TERCCTD_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCTD"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCTD"].mEndAddr);
    uint8_t TERCCTE_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCTE"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCTE"].mEndAddr);
    uint8_t TERCCTF_val   = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCCTF"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCCTF"].mEndAddr);
    uint8_t TERCSZI4_val  = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCSZI4"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCSZI4"].mEndAddr);
    uint8_t TERCSZI5_val  = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCSZI5"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCSZI5"].mEndAddr);
    uint8_t TERCSZ_val    = bit_select(str.data
                                    ,(*RHSIFTXERRSC)["TERCSZ"].mStartAddr
                                    ,(*RHSIFTXERRSC)["TERCSZ"].mEndAddr);
    
    if (TERCCT1_val == 1) {
        //Clear the RHSIFTXERRST.TERCT1 bit
        (*RHSIFTXERRST)["TERCT1"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCT2_val == 1) {
        //Clear the RHSIFTXERRST.TERCT2 bit
        (*RHSIFTXERRST)["TERCT2"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCT8_val == 1) {
        //Clear the RHSIFTXERRST.TERCT8 bit
        (*RHSIFTXERRST)["TERCT8"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCT9_val == 1) {
        //Clear the RHSIFTXERRST.TERCT9 bit
        (*RHSIFTXERRST)["TERCT9"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCTA_val == 1) {
        //Clear the RHSIFTXERRST.TERCTA bit
        (*RHSIFTXERRST)["TERCTA"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCTB_val == 1) {
        //Clear the RHSIFTXERRST.TERCTB bit
        (*RHSIFTXERRST)["TERCTB"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCTC_val == 1) {
        //Clear the RHSIFTXERRST.TERCTC bit
        (*RHSIFTXERRST)["TERCTC"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCTD_val == 1) {
        //Clear the RHSIFTXERRST.TERCTD bit
        (*RHSIFTXERRST)["TERCTD"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCTE_val == 1) {
        //Clear the RHSIFTXERRST.TERCTE bit
        (*RHSIFTXERRST)["TERCTE"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCCTF_val == 1) {
        //Clear the RHSIFTXERRST.TERCTF bit
        (*RHSIFTXERRST)["TERCTF"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCSZI4_val == 1) {
        //Clear the RHSIFTXERRST.TERSZI4 bit
        (*RHSIFTXERRST)["TERSZI4"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCSZI5_val == 1) {
        //Clear the RHSIFTXERRST.TERSZI5 bit
        (*RHSIFTXERRST)["TERSZI5"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
    
    if (TERCSZ_val == 1) {
        //Clear the RHSIFTXERRST.TERSZ bit
        (*RHSIFTXERRST)["TERSZ"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_txerr");
    }
}//}}}
void HSIFL10_ALL_Func::cb_RHSIFRXCMPSC_RCCL2(RegCBstr str)
{//{{{
    uint8_t RCCIC_val   = bit_select(str.data
                                    ,(*RHSIFRXCMPSC)["RCCIC"].mStartAddr
                                    ,(*RHSIFRXCMPSC)["RCCIC"].mEndAddr);
    uint8_t RCCCT_val = bit_select(str.data
                                    ,(*RHSIFRXCMPSC)["RCCCT"].mStartAddr
                                    ,(*RHSIFRXCMPSC)["RCCCT"].mEndAddr);
    uint8_t RCCL2_val = bit_select(str.data
                                    ,(*RHSIFRXCMPSC)["RCCL2"].mStartAddr
                                    ,(*RHSIFRXCMPSC)["RCCL2"].mEndAddr);
                                    
    if (RCCIC_val == 1) {
        //Clear the RHSIFRXCMPST.RCIC bit
        (*RHSIFRXCMPST)["RCIC"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxcmp");
    }
    
    if (RCCCT_val == 1) {
        //Clear the RHSIFRXCMPST.RCCT bit
        (*RHSIFRXCMPST)["RCCT"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxcmp");
    }
    
    if (RCCL2_val == 1) {
        //Clear the RHSIFRXCMPST.RCL2 bit
        (*RHSIFRXCMPST)["RCL2"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxcmp");
    }
}//}}}

void HSIFL10_ALL_Func::cb_RHSIFRXERRSC_RERCSZ(RegCBstr str)
{//{{{
    uint8_t RERCIPV_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCIPV"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCIPV"].mEndAddr);
    uint8_t RERCCT1_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCT1"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCT1"].mEndAddr);
    uint8_t RERCCT2_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCT2"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCT2"].mEndAddr);
    uint8_t RERCCT8_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCT8"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCT8"].mEndAddr);
    uint8_t RERCCT9_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCT9"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCT9"].mEndAddr);
    uint8_t RERCCTA_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCTA"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCTA"].mEndAddr);
    uint8_t RERCCTB_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCTB"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCTB"].mEndAddr);
    uint8_t RERCCTC_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCTC"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCTC"].mEndAddr);
    uint8_t RERCCTD_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCTD"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCTD"].mEndAddr);
    uint8_t RERCCTE_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCTE"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCTE"].mEndAddr);
    uint8_t RERCCTF_val   = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCCTF"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCCTF"].mEndAddr);
    uint8_t RERCSZI4_val  = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCSZI4"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCSZI4"].mEndAddr);
    uint8_t RERCSZI5_val  = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCSZI5"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCSZI5"].mEndAddr);
    uint8_t RERCSZ_val    = bit_select(str.data
                                    ,(*RHSIFRXERRSC)["RERCSZ"].mStartAddr
                                    ,(*RHSIFRXERRSC)["RERCSZ"].mEndAddr);
    
    if (RERCIPV_val == 1) {
        //Clear the RHSIFRXERRST.RERIPV bit
        (*RHSIFRXERRST)["RERIPV"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCT1_val == 1) {
        //Clear the RHSIFRXERRST.RERCT1 bit
        (*RHSIFRXERRST)["RERCT1"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCT2_val == 1) {
        //Clear the RHSIFRXERRST.RERCT2 bit
        (*RHSIFRXERRST)["RERCT2"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCT8_val == 1) {
        //Clear the RHSIFRXERRST.RERCT8 bit
        (*RHSIFRXERRST)["RERCT8"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCT9_val == 1) {
        //Clear the RHSIFRXERRST.RERCT9 bit
        (*RHSIFRXERRST)["RERCT9"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCTA_val == 1) {
        //Clear the RHSIFRXERRST.RERCTA bit
        (*RHSIFRXERRST)["RERCTA"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCTB_val == 1) {
        //Clear the RHSIFRXERRST.RERCTB bit
        (*RHSIFRXERRST)["RERCTB"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCTC_val == 1) {
        //Clear the RHSIFRXERRST.RERCTC bit
        (*RHSIFRXERRST)["RERCTC"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCTD_val == 1) {
        //Clear the RHSIFRXERRST.RERCTD bit
        (*RHSIFRXERRST)["RERCTD"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCTE_val == 1) {
        //Clear the RHSIFRXERRST.RERCTE bit
        (*RHSIFRXERRST)["RERCTE"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCCTF_val == 1) {
        //Clear the RHSIFRXERRST.RERCTF bit
        (*RHSIFRXERRST)["RERCTF"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCSZI4_val == 1) {
        //Clear the RHSIFRXERRST.RERSZI4 bit
        (*RHSIFRXERRST)["RERSZI4"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCSZI5_val == 1) {
        //Clear the RHSIFRXERRST.RERSZI5 bit
        (*RHSIFRXERRST)["RERSZI5"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
    
    if (RERCSZ_val == 1) {
        //Clear the RHSIFRXERRST.RERSZ bit
        (*RHSIFRXERRST)["RERSZ"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxerr");
    }
}//}}}

void HSIFL10_ALL_Func::cb_RHSIFRXICSC_RICPA(RegCBstr str)
{//{{{    
    uint8_t RICPG_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICPG"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICPG"].mEndAddr);
    uint8_t RICPSRT_val   = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICPSRT"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICPSRT"].mEndAddr);
    uint8_t RICPSTP_val   = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICPSTP"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICPSTP"].mEndAddr);
    uint8_t RICSR_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICSR"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICSR"].mEndAddr);
    uint8_t RICFR_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICFR"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICFR"].mEndAddr);
    uint8_t RICST_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICST"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICST"].mEndAddr);
    uint8_t RICFT_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICFT"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICFT"].mEndAddr);
    uint8_t RICET_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICET"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICET"].mEndAddr);
    uint8_t RICDT_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICDT"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICDT"].mEndAddr);
    uint8_t RICTON_val    = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICTON"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICTON"].mEndAddr);
    uint8_t RICTOF_val    = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICTOF"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICTOF"].mEndAddr);
    uint8_t RICTOL_val    = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICTOL"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICTOL"].mEndAddr);
    uint8_t RICPA_val     = bit_select(str.data
                                    ,(*RHSIFRXICSC)["RICPA"].mStartAddr
                                    ,(*RHSIFRXICSC)["RICPA"].mEndAddr);
    
    if (RICPG_val == 1) {
        //Clears the RHSIFRXICST.RIPG bit
        (*RHSIFRXICST)["RIPG"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICPSRT_val == 1) {
        //Clears the RHSIFRXICST.RIPSRT bit
        (*RHSIFRXICST)["RIPSRT"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICPSTP_val == 1) {
        //Clears the RHSIFRXICST.RIPSTP bit
        (*RHSIFRXICST)["RIPSTP"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICSR_val == 1) {
        //Clears the RHSIFRXICST.RISR bit
        (*RHSIFRXICST)["RISR"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICFR_val == 1) {
        //Clears the RHSIFRXICST.RIFR bit
        (*RHSIFRXICST)["RIFR"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICST_val == 1) {
        //Clears the RHSIFRXICST.RIST bit
        (*RHSIFRXICST)["RIST"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICFT_val == 1) {
        //Clears the RHSIFRXICST.RIFT bit
        (*RHSIFRXICST)["RIFT"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICET_val == 1) {
        //Clears the RHSIFRXICST.RIET bit
        (*RHSIFRXICST)["RIET"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICDT_val == 1) {
        //Clears the RHSIFRXICST.RIDT bit
        (*RHSIFRXICST)["RIDT"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICTON_val == 1) {
        //Clears the RHSIFRXICST.RITON bit
        (*RHSIFRXICST)["RITON"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICTOF_val == 1) {
        //Clears the RHSIFRXICST.RITOF bit
        (*RHSIFRXICST)["RITOF"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICTOL_val == 1) {
        //Clears the RHSIFRXICST.RITOL bit
        (*RHSIFRXICST)["RITOL"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    }
    
    if (RICPA_val == 1) {
        //Clears the RHSIFRXICST.RIPA bit
        (*RHSIFRXICST)["RIPA"] = 0;
        mHSIFL10_ALL_AgentController->ClearInterrupt("int_hsif_rxiclc");
    } 
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFRXCMPST(const uint32_t frameType)
{//{{{
    if (frameType == (uint32_t)mHSIFL10_ALL_AgentController->emICLCFramePingAns
        || frameType == (uint32_t)mHSIFL10_ALL_AgentController->emICLCFrameL1) {
        (*RHSIFRXCMPST)["RCIC"] = 1;
    }
    else if (frameType == (uint32_t)mHSIFL10_ALL_AgentController->emCTSFrameL1) {
        (*RHSIFRXCMPST)["RCCT"] = 1;
    }
    else if (frameType == (uint32_t)mHSIFL10_ALL_AgentController->emFrameL2) {
        (*RHSIFRXCMPST)["RCL2"] = 1;
    } 
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFTXCMPST(const uint32_t frameType)
{//{{{
    if (frameType == (uint32_t)mHSIFL10_ALL_AgentController->emICLCFramePingAns
        || frameType == (uint32_t)mHSIFL10_ALL_AgentController->emICLCFrameL1) {
        (*RHSIFTXCMPST)["TCIC"] = 1;
    }
    else if (frameType == (uint32_t)mHSIFL10_ALL_AgentController->emCTSFrameL1) {
        (*RHSIFTXCMPST)["TCCT"] = 1;
    }
    else if (frameType == (uint32_t)mHSIFL10_ALL_AgentController->emFrameL2) {
        (*RHSIFTXCMPST)["TCL2"] = 1;
    } 
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFICCR()
{//{{{
    (*RHSIFICCR)["ITRG"] = 0;
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFTXCMPIE()
{//{{{
    return (uint32_t)(*RHSIFTXCMPIE);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFRXCMPIE()
{//{{{
    return (uint32_t)(*RHSIFRXCMPIE);
}//}}}

uint8_t HSIFL10_ALL_Func::GetRHSIFRXICIE(uint8_t posBit)
{//{{{
    uint8_t val = 0xFF;
    switch (posBit) {
        case 0:
            val = (*RHSIFRXICIE)["RIEPG"];
            break;
        case 1:
            val = (*RHSIFRXICIE)["RIEPSRT"];
            break;
        case 2:
            val = (*RHSIFRXICIE)["RIEPSTP"];
            break;
        case 3:
            val = (*RHSIFRXICIE)["RIESR"];
            break;
        case 4:
            val = (*RHSIFRXICIE)["RIEFR"];
            break;
        case 5:
            val = (*RHSIFRXICIE)["RIEST"];
            break;
        case 6:
            val = (*RHSIFRXICIE)["RIEFT"];
            break;
        case 7:
            val = (*RHSIFRXICIE)["RIEET"];
            break;
        case 8:
            val = (*RHSIFRXICIE)["RIEDT"];
            break;
        case 16:
            val = (*RHSIFRXICIE)["RIEPA"];
            break;
         default:
            break;
    }
    return val;
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFSPCR()
{//{{{
    return (uint32_t)(*RHSIFSPCR);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFTXRXCR()
{//{{{
    return (uint32_t)(*RHSIFTXRXCR);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFMDCR()
{//{{{
    return (uint32_t)(*RHSIFMDCR);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFICCR()
{//{{{
    return (uint32_t)(*RHSIFICCR);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFTMDCR()
{//{{{
    return (uint32_t)(*RHSIFTMDCR);
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFRXICST(uint32_t RHSIFRXICST_val)
{//{{{
    if (RHSIFRXICST_val == 0x00) {
        if ((this->GetRHSIFMDCR() & 0x1) == 0) {
            //Update status of ICLC ping request
            (*RHSIFRXICST)["RIPG"] = 1;
        }
        else { // this->GetRHSIFMDCR() & 0x1) == 1
            //Update status of ICLC Ping answer
            (*RHSIFRXICST)["RIPA"] = 1;
        } 
    }
    
    if (RHSIFRXICST_val == 0x02) {
        //Update status of ICLC PLL Start
        (*RHSIFRXICST)["RIPSRT"] = 1;
    }
    
    if (RHSIFRXICST_val == 0x04) {
        //Update status of ICLC PLL Stop
        (*RHSIFRXICST)["RIPSTP"] = 1;
    }
    
    if (RHSIFRXICST_val == 0x08) {
        //Update status of ICLC select SlowMode Rx port
        (*RHSIFRXICST)["RISR"] = 1;
    }
    
    if (RHSIFRXICST_val == 0x10) {
        //Update status of ICLC select FastMode Rx port
        (*RHSIFRXICST)["RIFR"] = 1;
    }
    
    if (RHSIFRXICST_val == 0x20) {
        //Update status of ICLC select SlowMode Tx port. 
        (*RHSIFRXICST)["RIST"] = 1;
    }
    
    if (RHSIFRXICST_val == 0x80) {
        //Update status of ICLC select FastMode Tx port. 
        (*RHSIFRXICST)["RIFT"] = 1;
    }
    
    if (RHSIFRXICST_val == 0x31) {
        //Update status of ICLC enable Tx port. 
        (*RHSIFRXICST)["RIET"] = 1;
    }
    
    if (RHSIFRXICST_val == 0x32) {
        //Update status of ICLC enable Tx port. 
        (*RHSIFRXICST)["RIDT"] = 1;
    }
}//}}}


void HSIFL10_ALL_Func::UpdateRHSIFTXRXCR(uint8_t posBit, bool isEnable)
{//{{{
    //Uupdate TXEN bit
    if (posBit == 16) {
        (*RHSIFTXRXCR)["TXEN"] = isEnable;
    } 
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFRXICST()
{//{{{
    return (uint32_t)(*RHSIFRXICST);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFPCR()
{//{{{
    return (uint32_t)(*RHSIFPCR);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFRXERRIE()
{//{{{
    return (uint32_t)(*RHSIFRXERRIE);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFL1SR()
{//{{{
    return (uint32_t)(*RHSIFL1SR);
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFL1SR(uint8_t posBit, bool isEnable)
{//{{{
    switch (posBit) {
        case 0:
            (*RHSIFL1SR)["RFEMP"] = isEnable;
            break;
        case 1:
            (*RHSIFL1SR)["RFFUL"] = isEnable;
            break;
        case 8:
            (*RHSIFL1SR)["TFEMP"] = isEnable;
            break;
        case 9:
            (*RHSIFL1SR)["TFFUL"] = isEnable;
            break;
        case 16:
            (*RHSIFL1SR)["RCTS"] = isEnable;
            break;
        default:
            break;
    }
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFCCR()
{//{{{
    return (uint32_t)(*RHSIFCCR);
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFCCR(uint8_t posBit, bool isEnable)
{//{{{
    //Update RHSIFCCR.CTRG
    if (posBit == 16) {
        (*RHSIFCCR)["CTRG"] = isEnable;
    } 
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFLTXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit)
{//{{{
    (*RHSIFLTXFRMRL1)["TL1H"] = header;
    (*RHSIFLTXFRMRL1)["TL1P"] = payload;
    (*RHSIFLTXFRMRL1)["TL1E"] = isEndbit;
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFLTXFRMRL2(uint8_t headerL1, uint16_t headerL2)
{//{{{
    (*RHSIFLTXFRMRL2)["T2L1H"] = headerL1;
    (*RHSIFLTXFRMRL2)["T2L2H"] = headerL2;
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFLRXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit)
{//{{{
    (*RHSIFLRXFRMRL1)["RL1H"] = header;
    (*RHSIFLRXFRMRL1)["RL1P"] = payload;
    (*RHSIFLRXFRMRL1)["RL1E"] = isEndbit;
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFLRXFRMRL2(uint8_t headerL1, uint16_t headerL2)
{//{{{
    (*RHSIFLRXFRMRL2)["R2L1H"] = headerL1;
    (*RHSIFLRXFRMRL2)["R2L2H"] = headerL2;
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFSMCR()
{//{{{
    return (uint32_t)(*RHSIFSMCR);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFTXERRIE()
{//{{{
    return (uint32_t)(*RHSIFTXERRIE);
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFTXERRST(uint32_t errorCode)
{//{{{
    uint32_t val = 0x0;
    //The temporary store previous value of RHSIFTXERRST register
    uint32_t RHSIFTXERRST_pre_val = (*RHSIFTXERRST);
    
    bool TERSZ_errCode   = (errorCode >> 31) & 0x1;
    bool TERSZI5_errCode = (errorCode >> 20) & 0x1;
    bool TERSZI4_errCode = (errorCode >> 19) & 0x1;
    bool TERCTF_errCode  = (errorCode >> 15) & 0x1;
    bool TERCTE_errCode  = (errorCode >> 14) & 0x1;
    bool TERCTD_errCode  = (errorCode >> 13) & 0x1;
    bool TERCTC_errCode  = (errorCode >> 12) & 0x1;
    bool TERCTB_errCode  = (errorCode >> 11) & 0x1;
    bool TERCTA_errCode  = (errorCode >> 10) & 0x1;
    bool TERCT9_errCode  = (errorCode >> 9)  & 0x1;
    bool TERCT8_errCode  = (errorCode >> 8) & 0x1;
    bool TERCT2_errCode  = (errorCode >> 2)  & 0x1;
    bool TERCT1_errCode  = (errorCode >> 1)  & 0x1;
    
    if (TERSZ_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERSZI5_errCode == 0x1) {
         val |= TERSZI5_errCode << 20;
    }
    if     (TERSZI4_errCode == 0x1) {
         val |= TERSZI4_errCode << 19;
    }
    if     (TERCTF_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCTE_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCTD_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCTC_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCTB_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCTA_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCT9_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCT8_errCode == 0x1) {
         val |= TERSZ_errCode << 31;
    }
    if     (TERCT2_errCode == 0x1) {
         val |= TERCT2_errCode << 2;
    }
    if     (TERCT1_errCode == 0x1) {
         val |= TERCT1_errCode << 1;
    }
    //Upate RHSIFTXERRST register
    (*RHSIFTXERRST) = val | RHSIFTXERRST_pre_val;
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFRXERRST(uint32_t errorCode)
{//{{{
    uint32_t val = 0x0;
    //The temporary store value of RHSIFRXERRST register
    uint32_t RHSIFRXERRST_pre_val = (*RHSIFRXERRST);
    
    bool RERSZ_errCode   = (errorCode >> 31) & 0x1;
    bool RERSZI5_errCode = (errorCode >> 20) & 0x1;
    bool RERSZI4_errCode = (errorCode >> 19) & 0x1;
    bool RERCTF_errCode  = (errorCode >> 15) & 0x1;
    bool RERCTE_errCode  = (errorCode >> 14) & 0x1;
    bool RERCTD_errCode  = (errorCode >> 13) & 0x1;
    bool RERCTC_errCode  = (errorCode >> 12) & 0x1;
    bool RERCTB_errCode  = (errorCode >> 11) & 0x1;
    bool RERCTA_errCode  = (errorCode >> 10) & 0x1;
    bool RERCT9_errCode  = (errorCode >> 9)  & 0x1;
    bool RERCT8_errCode  = (errorCode >> 8) & 0x1;
    bool RERCT2_errCode  = (errorCode >> 2)  & 0x1;
    bool RERCT1_errCode  = (errorCode >> 1)  & 0x1;
    bool RERIPV_errCode  = (errorCode >> 0)  & 0x1;
    
    if(RERSZ_errCode == 0x1) {
        val |= RERSZ_errCode << 31;
    }
    if(RERSZI5_errCode == 0x1) {
        val |= RERSZI5_errCode << 20;
    }
    if(RERSZI4_errCode == 0x1) {
        val |= RERSZI4_errCode << 19;
    }
    if(RERCTF_errCode == 0x1) {
        val |= RERCTF_errCode << 15;
    }
    if(RERCTE_errCode == 0x1) {
        val |= RERCTE_errCode << 14;
    }
    if(RERCTD_errCode == 0x1) {
        val |= RERCTD_errCode << 13;
    }
    if(RERCTC_errCode == 0x1) {
        val |= RERCTC_errCode << 12;
    }
    if(RERCTB_errCode == 0x1) {
        val |= RERCTB_errCode << 11;
    }
    if(RERCTA_errCode == 0x1) {
        val |= RERCTA_errCode << 10;
    }
    if     (RERCT9_errCode == 0x1) {
        val |= RERCT9_errCode << 9;
    }
    if     (RERCT8_errCode == 0x1) {
        val |= RERCT8_errCode << 8;
    }
    if(RERCT2_errCode == 0x1) {
        val |= RERCT2_errCode << 2;
    }
    if(RERCT1_errCode == 0x1) {
        val |= RERCT1_errCode << 1;
    }
    if(RERIPV_errCode == 0x1) {
        val |= RERIPV_errCode << 0;
    }
    //Upate RHSIFRXERRST register
    (*RHSIFRXERRST) = val | RHSIFRXERRST_pre_val;
}//}}}

void HSIFL10_ALL_Func::UpdateRHSIFTMDCR(uint8_t posBit, bool isEnable, bool isTurnOffTestMode)
{//{{{
    switch (posBit) {
        case 0:
          (*RHSIFTMDCR)["RXLPBK"] = isEnable;
          if (isEnable == true && isTurnOffTestMode == false) {
            mHSIFL10_ALL_AgentController->SetLoopBackMode(true);
          }
          break;
        default:
          break;
    }
    if (isTurnOffTestMode == true) {
        (*RHSIFTMDCR)["RXLPBK"] = false;
        mHSIFL10_ALL_AgentController->SetLoopBackMode(false);
    }
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFTXERRST()
{//{{{
    return (uint32_t)(*RHSIFTXERRST);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFRXERRST()
{//{{{
    return (uint32_t)(*RHSIFRXERRST);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFTXCMPST()
{//{{{
    return (uint32_t)(*RHSIFTXCMPST);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFRXCMPST()
{//{{{
    return (uint32_t)(*RHSIFRXCMPST);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFLTXFRMRL1()
{//{{{
    return (uint32_t)(*RHSIFLTXFRMRL1);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFLRXFRMRL1()
{//{{{
    return (uint32_t)(*RHSIFLRXFRMRL1);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFLTXFRMRL2()
{//{{{
    return (uint32_t)(*RHSIFLTXFRMRL2);
}//}}}

uint32_t HSIFL10_ALL_Func::GetRHSIFLRXFRMRL2()
{//{{{
    return (uint32_t)(*RHSIFLRXFRMRL2);
}//}}}
// vim600: set foldmethod=marker :
