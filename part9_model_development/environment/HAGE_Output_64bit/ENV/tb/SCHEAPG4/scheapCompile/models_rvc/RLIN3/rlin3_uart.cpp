// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2012-2020 Renesas Electronics Corporation
// Copyright(c) 2012-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "rlin3_uart.h"
#ifndef ___re_printf
#define ___re_printf parent_pointer->get_fileline(__FILE__, __LINE__); parent_pointer->_re_printf  
#endif

//Constructor
Crlin3_uart::Crlin3_uart(sc_module_name name, Crlin3 *parent) :
    Crlin3_uart_regif((std::string)name,32)
    ,Crlin3_common(name)
    ,rx_data("rx_data")
    ,rx_control("rx_control")
    ,tx_data("tx_data")
    ,tx_control("tx_control")
{//{{{
    Crlin3_uart_regif::set_instance_name((std::string)name);//set hierarchy name

    //Check Null Pointer
    sc_assert(parent != NULL);

    parent_pointer = parent;

    tx_data.initialize(0xFFFFFFFF);
    tx_control.initialize(0x00000108);   
      
    SC_METHOD (ReceptionMethod)
    dont_initialize();
    sensitive << rx_control;
    sensitive << rx_data;

    SC_METHOD (SingleTransMethod)
    dont_initialize();
    sensitive << mSingleTransEvent;

    SC_METHOD (MultiTransMethod)
    dont_initialize();
    sensitive << mMultiTransEvent;

    SC_METHOD (MultiTransWaitStopBitMethod)
    dont_initialize();
    sensitive << mMultiTransWaitStopBitEvent;

    //Initialize the variables
    mWait_Trans = emNoTrans;
    mIsDataRead = true;
    mIsLUWTDRWrite = false;
    mIsLUTDRWrite = false;
    mNoMultiTransWaitStopBit = false;
    mCheckAccessLUTDR = false;
    mCheckAccessLUWTDR = true;
    mIsReceiveStopBit = false;
    mTransStatus = emRESET;
    mRecvStatus = emRESET;
    mDataValue = 0xFFFFFFFF;
    mSingleTxDataValue = 0;      // Add by SonTran: No.52 In case LUOR1.UTIGTS = 1, if users write to LUTDR before the transmission interrupt occurs, the transmission interrupt does not occur (RLIN3v4)
 }//}}}

// Destructor
Crlin3_uart::~Crlin3_uart()
{//{{{
}//}}}

std::string Crlin3_uart::uart_reg_command(const std::vector<std::string>& args)
{//{{{
    return this->reg_handle_command(args);
}//}}}

void Crlin3_uart::UpdateRegsOfLin3()
{//{{{ 
    if (parent_pointer->mMode == Crlin3_common::emUartMode) {
        RlinRegs rlin_reg;
        rlin_reg.lwbr  =  (unsigned int)(*LWBR); 
        rlin_reg.lbrp0 =  (unsigned int)(*LBRP0);
        rlin_reg.lbrp1 =  (unsigned int)(*LBRP1);
        rlin_reg.lstc  =  (unsigned int)(*LSTC); 
        rlin_reg.lmd   =  (unsigned int)(*LMD);  
        rlin_reg.lbfc  =  (unsigned int)(*LBFC); 
        rlin_reg.lsc   =  (unsigned int)(*LSC);  
        rlin_reg.lwup  =  (unsigned int)(*LWUP); 
        rlin_reg.lie   =  (unsigned int)(*LIE);  
        rlin_reg.lede  =  (unsigned int)(*LEDE); 
        rlin_reg.lcuc  =  (unsigned int)(*LCUC); 
        rlin_reg.ltrc  =  (unsigned int)(*LTRC); 
        rlin_reg.lmst  =  (unsigned int)(*LMST); 
        rlin_reg.lst   =  (unsigned int)(*LST); 
        rlin_reg.lest  =  (unsigned int)(*LEST); 
        rlin_reg.ldfc  =  (unsigned int)(*LDFC); 
        rlin_reg.lidb  =  (unsigned int)(*LIDB); 
        rlin_reg.lcbr  =  (unsigned int)(*LCBR); 
        rlin_reg.ludb0 =  (unsigned int)(*LUDB0); 
        rlin_reg.ldb1  =  (unsigned int)(*LDB1); 
        rlin_reg.ldb2  =  (unsigned int)(*LDB2); 
        rlin_reg.ldb3  =  (unsigned int)(*LDB3); 
        rlin_reg.ldb4  =  (unsigned int)(*LDB4); 
        rlin_reg.ldb5  =  (unsigned int)(*LDB5); 
        rlin_reg.ldb6  =  (unsigned int)(*LDB6); 
        rlin_reg.ldb7  =  (unsigned int)(*LDB7); 
        rlin_reg.ldb8  =  (unsigned int)(*LDB8); 
        rlin_reg.luoer =  (unsigned int)(*LUOER);
        rlin_reg.luor1 =  (unsigned int)(*LUOR1);
        rlin_reg.lutdr =  (unsigned int)(*LUTDR);
        rlin_reg.lurdr =  (unsigned int)(*LURDR);
        rlin_reg.luwtdr = (unsigned int)(*LUWTDR);
        rlin_reg.lurde =  (unsigned int)(*LURDE);
        rlin_reg.lbss =  (unsigned int)(*LBSS);
        rlin_reg.lrss =  (unsigned int)(*LRSS);

        parent_pointer->UpdateRlin3Reg (rlin_reg);
    }
}//}}}

bool Crlin3_uart::uart_reg_rd(unsigned int addr,unsigned char *p_data,unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_rd(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_uart::uart_reg_rd_dbg(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_rd_dbg(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_uart::uart_reg_wr(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_wr(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_uart::uart_reg_wr_dbg(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_wr_dbg(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
void Crlin3_uart::SetUartClock(double clkc, double pclk)
{//{{{
    if (!Crlin3_common::CheckZeroClock()) {
        //cancel all processes
        mMultiTransEvent.cancel();
        mSingleTransEvent.cancel();
        mMultiTransWaitStopBitEvent.cancel();
    } 
    Crlin3_common::SetLinClock(clkc,pclk);
}//}}}

void Crlin3_uart::ResetUart(bool is_active)
{//{{{
    if (is_active) {
        mWait_Trans = emNoTrans;
        mIsDataRead = true;
        mIsLUWTDRWrite = false;
        mTransStatus = emRESET;
        mRecvStatus = emRESET;
        mDataValue = 0xFFFFFFFF;
        mSingleTxDataValue = 0;      // Add by SonTran: No.52 In case LUOR1.UTIGTS = 1, if users write to LUTDR before the transmission interrupt occurs, the transmission interrupt does not occur (RLIN3v4)
        mNoMultiTransWaitStopBit = false;
        mCheckAccessLUTDR = false;
        mCheckAccessLUWTDR = true;
        mIsReceiveStopBit = false;
        Crlin3_common::StopOperation(true);
    }
    Crlin3_uart_regif::EnableReset(is_active);
}//}}}

void Crlin3_uart::UpdateUartRegs(RlinRegs rlin_reg) 
{//{{{
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        (*LWBR )  = rlin_reg.lwbr  ;
        (*LBFC )  = rlin_reg.lbfc  ;
        (*LSC  )  = rlin_reg.lsc   ;
        (*LEDE )  = rlin_reg.lede  ;
        (*LDFC )  = rlin_reg.ldfc  ;
    }
    (*LBRP0)  = rlin_reg.lbrp0 ;
    (*LBRP1)  = rlin_reg.lbrp1 ;
    (*LSTC )  = rlin_reg.lstc  ;
    (*LMD  )  = rlin_reg.lmd   ;
    (*LCUC )  = rlin_reg.lcuc  ;
    (*LMST )  = rlin_reg.lmst  ;
    (*LIDB )  = rlin_reg.lidb  ;
    (*LCBR )  = rlin_reg.lcbr  ;
    (*LDB1 )  = rlin_reg.ldb1  ;
    (*LDB2 )  = rlin_reg.ldb2  ;
    (*LDB3 )  = rlin_reg.ldb3  ;
    (*LDB4 )  = rlin_reg.ldb4  ;
    (*LDB5 )  = rlin_reg.ldb5  ;
    (*LDB6 )  = rlin_reg.ldb6  ;
    (*LDB7 )  = rlin_reg.ldb7  ;
    (*LDB8 )  = rlin_reg.ldb8  ;
    (*LBSS )  = rlin_reg.lbss  ;
    (*LRSS )  = rlin_reg.lrss  ;
}//}}}

unsigned int Crlin3_uart::AddParity(unsigned int data)
{//{{{
    unsigned int parity_bit = 0;
    unsigned int data_tmp = 0;
    unsigned int bit_length = 7;

    if (((*LUOR1)["UEBE"] == 1)&&((unsigned int)(*LTRC)["RTS"] == 0)) { // expension bit enable, no expansion bit in multi bytes communication
        bit_length = 9;
    } else if ((unsigned int)(*LBFC)["UBLS"] == 0) {
        bit_length = 8;
    }
    parity_bit = Crlin3_common::GetParity(data,bit_length);
    //Add parity bit to 9th bit
    data_tmp = data | (parity_bit << bit_length);
    return data_tmp;
}//}}}

std::string Crlin3_uart::GetCurrentStatus()
{//{{{ 
    // Check reception status of RLIN3
    std::string current_state = "";
    if (((unsigned int)(*LMST)&0x1) == 0) {
        current_state = "Reset";
    } else { //IDLE or Operation mode in LIN communication
        if ((unsigned int)(*LST)["URS"] == 1) { //Operation mode - Reception
            current_state = "Response Reception";
        }
        // Mod by SonTran: No.8-1,8-2,8-3,9-1 Don't care constrain of full duplex and half duplex in UART mode 
        if ((unsigned int)(*LST)["UTS"] == 1) {//Operation mode - Transmission
            if (current_state != "") {
                current_state += " and ";
            }
            current_state += "Response Transmission";
        } 
        if (current_state == "") { //IDLE mode
            current_state = "Idle";
        }
    }
    return current_state;
}//}}}

void Crlin3_uart::SingleTransMethod(void)
{//{{{
    unsigned int data_transfer;
    RlinRegs rlin_reg;
    //Prepare data for transmission
    if (mWait_Trans == emSingWaitStopBit) {
        data_transfer = (unsigned int)(*LUWTDR);
    }
     if(mWait_Trans == emSingNoWaitStopBit) {
        data_transfer = (unsigned int)(*LUTDR);
    }
    //Add Inversion if it is selected.
    if ((unsigned int)(*LBFC)["UTPS"] == 1) {
        data_transfer = ~(data_transfer);
    }
    //Add parity bit 
    if((unsigned int)(*LBFC)["UPS"] != 0) {
        data_transfer = AddParity(data_transfer);
    }
    Crlin3_common::SetDataTransfer(0,data_transfer);
    rlin_reg.lwbr  =  (unsigned int)(*LWBR); 
    rlin_reg.lbrp0 =  (unsigned int)(*LBRP0);
    rlin_reg.lbrp1 =  (unsigned int)(*LBRP1);
    rlin_reg.lstc  =  (unsigned int)(*LSTC); 
    rlin_reg.lmd   =  (unsigned int)(*LMD);  
    rlin_reg.lbfc  =  (unsigned int)(*LBFC); 
    rlin_reg.lsc   =  (unsigned int)(*LSC);  
    rlin_reg.lede  =  (unsigned int)(*LEDE); 
    rlin_reg.lcuc  =  (unsigned int)(*LCUC); 
    rlin_reg.ltrc  =  (unsigned int)(*LTRC); 
    rlin_reg.lmst  =  (unsigned int)(*LMST); 
    rlin_reg.lst   =  (unsigned int)(*LST); 
    rlin_reg.lest  =  (unsigned int)(*LEST); 
    rlin_reg.ldfc  =  (unsigned int)(*LDFC); 
    rlin_reg.lidb  =  (unsigned int)(*LIDB); 
    rlin_reg.lcbr  =  (unsigned int)(*LCBR); 
    rlin_reg.ludb0 =  (unsigned int)(*LUDB0); 
    rlin_reg.ldb1  =  (unsigned int)(*LDB1); 
    rlin_reg.ldb2  =  (unsigned int)(*LDB2); 
    rlin_reg.ldb3  =  (unsigned int)(*LDB3); 
    rlin_reg.ldb4  =  (unsigned int)(*LDB4); 
    rlin_reg.ldb5  =  (unsigned int)(*LDB5); 
    rlin_reg.ldb6  =  (unsigned int)(*LDB6); 
    rlin_reg.ldb7  =  (unsigned int)(*LDB7); 
    rlin_reg.ldb8  =  (unsigned int)(*LDB8); 
    rlin_reg.luoer =  (unsigned int)(*LUOER);
    rlin_reg.luor1 =  (unsigned int)(*LUOR1);
    rlin_reg.lutdr =  (unsigned int)(*LUTDR);
    rlin_reg.lurdr =  (unsigned int)(*LURDR);
    rlin_reg.luwtdr = (unsigned int)(*LUWTDR);
    rlin_reg.lbss  = (unsigned int)(*LBSS);
    rlin_reg.lrss  = (unsigned int)(*LRSS);
    Crlin3_common::SetConfigFactors(rlin_reg);
    Crlin3_common::TransmitProcess(emUARTSINGLE);
}//}}}
//
void Crlin3_uart::MultiTransWaitStopBitMethod(void)
{//{{{
    mNoMultiTransWaitStopBit = false;
    // Mod by SonTran: No.8-1,8-2,8-3,9-1 Don't care constrain of full duplex and half duplex in UART mode 
    mMultiTransEvent.notify();
}//}}}

void Crlin3_uart::MultiTransMethod(void)
{//{{{
    unsigned int data_transfer;
    unsigned int data_length;
    if (((unsigned int)(*LDFC)["MDL"] < 9)&&((unsigned int)(*LDFC)["MDL"] != 0)) {
        data_length = (unsigned int)(*LDFC)["MDL"];
    } else {
        data_length = 9;
    }
    unsigned int data[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    RlinRegs rlin_reg;
    data[1] = (unsigned int)(*LDB1);
    data[2] = (unsigned int)(*LDB2);
    data[3] = (unsigned int)(*LDB3);
    data[4] = (unsigned int)(*LDB4);
    data[5] = (unsigned int)(*LDB5);
    data[6] = (unsigned int)(*LDB6);
    data[7] = (unsigned int)(*LDB7);
    data[8] = (unsigned int)(*LDB8);
    data[0] = (unsigned int)(*LUDB0);
    unsigned int first_data_index = 0;
    if (data_length != 9) {
        first_data_index = 1;
    }
    unsigned int last_data_index = 8;
    if (data_length < 9) {
        last_data_index = data_length;
    }
    for (unsigned int i=first_data_index;i<=last_data_index;i++) {
        data_transfer = data[i];
        //Add Inversion if it is selected.
        if ((unsigned int)(*LBFC)["UTPS"] == 1) {
            data_transfer = ~(data_transfer);
        }
        //Add parity bit 
        if((unsigned int)(*LBFC)["UPS"] != 0) {
            data_transfer = AddParity(data_transfer);
        }
        if (data_length < 9) {
            Crlin3_common::SetDataTransfer(i-1,data_transfer);
        } else {
            Crlin3_common::SetDataTransfer(i,data_transfer);
        }
    }
    //Transfer data
    rlin_reg.lwbr  =  (unsigned int)(*LWBR); 
    rlin_reg.lbrp0 =  (unsigned int)(*LBRP0);
    rlin_reg.lbrp1 =  (unsigned int)(*LBRP1);
    rlin_reg.lstc  =  (unsigned int)(*LSTC); 
    rlin_reg.lmd   =  (unsigned int)(*LMD);  
    rlin_reg.lbfc  =  (unsigned int)(*LBFC); 
    rlin_reg.lsc   =  (unsigned int)(*LSC);  
    rlin_reg.lede  =  (unsigned int)(*LEDE); 
    rlin_reg.lcuc  =  (unsigned int)(*LCUC); 
    rlin_reg.ltrc  =  (unsigned int)(*LTRC); 
    rlin_reg.lmst  =  (unsigned int)(*LMST); 
    rlin_reg.lst   =  (unsigned int)(*LST); 
    rlin_reg.lest  =  (unsigned int)(*LEST); 
    rlin_reg.ldfc  =  (unsigned int)(*LDFC); 
    rlin_reg.lidb  =  (unsigned int)(*LIDB); 
    rlin_reg.lcbr  =  (unsigned int)(*LCBR); 
    rlin_reg.ludb0 =  (unsigned int)(*LUDB0); 
    rlin_reg.ldb1  =  (unsigned int)(*LDB1); 
    rlin_reg.ldb2  =  (unsigned int)(*LDB2); 
    rlin_reg.ldb3  =  (unsigned int)(*LDB3); 
    rlin_reg.ldb4  =  (unsigned int)(*LDB4); 
    rlin_reg.ldb5  =  (unsigned int)(*LDB5); 
    rlin_reg.ldb6  =  (unsigned int)(*LDB6); 
    rlin_reg.ldb7  =  (unsigned int)(*LDB7); 
    rlin_reg.ldb8  =  (unsigned int)(*LDB8); 
    rlin_reg.luoer =  (unsigned int)(*LUOER);
    rlin_reg.luor1 =  (unsigned int)(*LUOR1);
    rlin_reg.lutdr =  (unsigned int)(*LUTDR);
    rlin_reg.lurdr =  (unsigned int)(*LURDR);
    rlin_reg.luwtdr = (unsigned int)(*LUWTDR);
    Crlin3_common::SetConfigFactors(rlin_reg);
    Crlin3_common::TransmitProcess(emRESP);
}//}}}

void Crlin3_uart::ReceptionMethod(void)
{//{{{
    if (!Crlin3_common::CheckZeroClock()) {
        if (Crlin3_common::mPclk_Clock == 0) {
            ___re_printf("warning","The pclk period is equal 0.\n");
        } else {
            ___re_printf("warning","The clkc period is equal 0.\n");
        }
        return;
    }
    RlinRegs rlin_reg;
    if (((mTransStatus != emRESET) && (mRecvStatus != emRESET)) || ((mRecvStatus == emRESET)&&mIsReceiveStopBit)) {
        unsigned int rx_control_val = (unsigned int)rx_control.read();
        unsigned int rx_data_val = (unsigned int)rx_data.read();
        if (mDataValue != rx_data_val) {
            Crlin3_common::ReceptionProcess(rx_data_val,rx_control_val,emCHECKBITERR);
        }
        mDataValue = rx_data_val;
        if ( ((rx_control_val >> 8) & 0x1) == (*LBFC)["UBOS"] ) {
            ___re_printf("warning","Direction of received data packet is mismatched with RLIN3/UART setting.\n");
            // LBFC.UBOS and RX_CONTROL.DIR should be different
            // LSB-first: UBOS = 0, DIR = 1
            // MSB-first: UBOS = 1, DIR = 0
        }
        // Add by SonTran: No.55, 57 timing conflict at receiving stop bit (RLIN3v4)
        if ((((rx_control_val >> 6)&0x3) == 0x2)&&((unsigned int)(*LST)["URS"] == 1)) { //is receiving stop bit
            mIsReceiveStopBit = true;
        } else {
            mIsReceiveStopBit = false;
        }
        // Modified by SonTran: No.39 Reception will perform even if error is detected (RLIN3v4)
        if ((((rx_control_val >> 6)&0x1) == 0x1)&&((unsigned int)(*LUOER)["UROE"] == 1)) { //Start bit is detected, reception is enabled
            (*LST)["URS"] = 1;
            mRecvStatus = emOPERATION;
            mCheckAccessLUWTDR = false; // Add by SonTran: No.55 User should access to LUWTDR while receiving Stop bit in Hafl duplex mode (RLIN3v4)
        } else {
            mCheckAccessLUWTDR = true; // Add by SonTran: No.55 User should access to LUWTDR while receiving Stop bit in Hafl duplex mode (RLIN3v4)
        }
        rlin_reg.lwbr  =  (unsigned int)(*LWBR); 
        rlin_reg.lbrp0 =  (unsigned int)(*LBRP0);
        rlin_reg.lbrp1 =  (unsigned int)(*LBRP1);
        rlin_reg.lstc  =  (unsigned int)(*LSTC); 
        rlin_reg.lmd   =  (unsigned int)(*LMD);  
        rlin_reg.lbfc  =  (unsigned int)(*LBFC); 
        rlin_reg.lsc   =  (unsigned int)(*LSC);  
        rlin_reg.lede  =  (unsigned int)(*LEDE); 
        rlin_reg.lcuc  =  (unsigned int)(*LCUC); 
        rlin_reg.ltrc  =  (unsigned int)(*LTRC); 
        rlin_reg.lmst  =  (unsigned int)(*LMST); 
        rlin_reg.lst   =  (unsigned int)(*LST); 
        rlin_reg.lest  =  (unsigned int)(*LEST); 
        rlin_reg.ldfc  =  (unsigned int)(*LDFC); 
        rlin_reg.lidb  =  (unsigned int)(*LIDB); 
        rlin_reg.lcbr  =  (unsigned int)(*LCBR); 
        rlin_reg.ludb0 =  (unsigned int)(*LUDB0); 
        rlin_reg.ldb1  =  (unsigned int)(*LDB1); 
        rlin_reg.ldb2  =  (unsigned int)(*LDB2); 
        rlin_reg.ldb3  =  (unsigned int)(*LDB3); 
        rlin_reg.ldb4  =  (unsigned int)(*LDB4); 
        rlin_reg.ldb5  =  (unsigned int)(*LDB5); 
        rlin_reg.ldb6  =  (unsigned int)(*LDB6); 
        rlin_reg.ldb7  =  (unsigned int)(*LDB7); 
        rlin_reg.ldb8  =  (unsigned int)(*LDB8); 
        rlin_reg.luoer =  (unsigned int)(*LUOER);
        rlin_reg.luor1 =  (unsigned int)(*LUOR1);
        rlin_reg.lutdr =  (unsigned int)(*LUTDR);
        rlin_reg.lurdr =  (unsigned int)(*LURDR);
        rlin_reg.luwtdr = (unsigned int)(*LUWTDR);
        rlin_reg.lbss = (unsigned int)(*LBSS);
        rlin_reg.lrss = (unsigned int)(*LRSS);
        Crlin3_common::SetConfigFactors(rlin_reg);
        Crlin3_common::ReceptionProcess(rx_data_val, rx_control_val ,emRESP);
    }
}//}}}

//virtual functions
void Crlin3_uart::OutputData(unsigned int tx_control, unsigned int tx_data) 
{//{{{
    this->tx_data.write((sc_uint<32>)tx_data);
    this->tx_control.write((sc_uint<32>)tx_control);
}//}}}

void Crlin3_uart::UpdateRegisters(eREG_KIND reg_kind, unsigned int value) 
{//{{{
    if ((mIsDataRead == true)&&((unsigned int)(*LUOER)["UROE"] == 1)) {
        if ((unsigned int)(*LBFC)["URPS"] == 1) {
            (*LURDR) = ~(value);
        } else {
            (*LURDR) = value;
        }
        if ((unsigned int)(*LUOR1)["UEBE"] == 1) {//expension bit
            (*LURDR) = (unsigned int)(*LURDR)&0x1FF;
        } else if ((unsigned int)(*LBFC)["UBLS"] == 1) {//7bit in UART mode
            (*LURDR) = (unsigned int)(*LURDR)&0x7F;
        } else {//8bit in UART mode
            (*LURDR) = (unsigned int)(*LURDR)&0xFF; 
        }
        mIsDataRead = false;
    } else { //Overrun Error
        if ((unsigned int)(*LEDE)["OERE"] == 1) { //enable overrun error checking
            UpdateErrorStatus(emTimeOutOrOverrunError);
        }
    }
}//}}}

void Crlin3_uart::UpdateStatus(eSTATUS_FLAG flag) 
{//{{{
    RlinRegs rlin_reg;
    unsigned int data_buf[9] = {(unsigned int)(*LUDB0),(unsigned int)(*LDB1),(unsigned int)(*LDB2),(unsigned int)(*LDB3),(unsigned int)(*LDB4),(unsigned int)(*LDB5),(unsigned int)(*LDB6),(unsigned int)(*LDB7),(unsigned int)(*LDB8)};
    unsigned int mask = 0xFF;
    if ((unsigned int)(*LUOR1)["UEBE"] == 1) {
        mask = 0x1FF; //9bits
    } else if ((unsigned int)(*LBFC)["UBLS"] == 1) {
        mask = 0x7F;//7bits
    } else {
        mask = 0xFF;//8bits
    }
    //Issue transmission Interrupt
    // Modified by SonTran: No.52 In case LUOR1.UTIGTS = 1, if users write to LUTDR before the transmission interrupt occurs, the transmission interrupt does not occur (RLIN3v4)
    if ((flag == emIssueTransInt) && (!mCheckAccessLUTDR)) {
        parent_pointer->WriteLin3IntT(1);
    }
    mCheckAccessLUTDR = false;
    //Update status to LST
    if (flag == emFinishTransData) { // Modified by SonTran: No.47 Error is not affect to clear FTC bit in multi transmission (RLIN3v4)
        if (mTrans_status != emUARTSINGLE) { // Mod by SonTran: No.11-1 FTC flag is not set at single transmission completion in UART mode
            (*LST)["FTC"] = 1;
        }
        (*LST)["UTS"] = 0;
        (*LTRC)["RTS"] = 0; //RTS is cleared at end of multi-byte transmission
        mIsLUWTDRWrite = false;
    }
    if (flag == emRespondRecevComplete) {
        parent_pointer->DumpOperationInfo("Response", "Receiving", 0, (unsigned int)(*LURDR), "NoCheckSum", 0); //Dump Stat Info
        parent_pointer->mReceiveDataAmount++;
        // Modified by SonTran: No.57 SW reset occurs while reception stop bit, data is  stored into register and reception complete interrupt is asserted (RLIN3v4)
        if (mRecvStatus == emRESET) {
            (*LUOER)        = 0;
            (*LST)          = 0;
            Crlin3_common::StopOperation(false);
        } else {
            mRecvStatus = emIDLE;
        }
    } else if ((flag == emRespondTransComplete) || (flag == emFinishTransData)) {
        mTransStatus = emIDLE;
        parent_pointer->mTransmitDataAmount++;
        if (mWait_Trans == emSingNoWaitStopBit) {
            parent_pointer->DumpOperationInfo("Response", "Transmitting", 0, (mSingleTxDataValue&mask), "NoCheckSum", 0); //Trans Info
        } else if (mWait_Trans == emSingWaitStopBit) {
            parent_pointer->DumpOperationInfo("Response", "Transmitting", 0, ((unsigned int)(*LUWTDR)&mask), "NoCheckSum", 0); //Trans Info
        } else { //Multi-byte transmission without wait stop bit OR with wait stop bit
            if ((((unsigned int)(*LDFC)["MDL"]) != 0)&&(((unsigned int)(*LDFC)["MDL"]) <= 8)){ //7 or 8 bytes
                parent_pointer->DumpOperationInfo("Response", "Transmitting", 0, data_buf[(unsigned int)(Crlin3_common::mTransmission_index/3)+1], "NoCheckSum", 0); //Trans Info
            } else { //9 bytes
                parent_pointer->DumpOperationInfo("Response", "Transmitting", 0, data_buf[(unsigned int)(Crlin3_common::mTransmission_index/3)], "NoCheckSum", 0); //Trans Info
            }
        }
    }

    //Clear URS when finish reception progress
    if ((flag == emRespondRecevComplete)&&((unsigned int)(*LST)["ERR"] != 1)) {
        parent_pointer->WriteLin3IntR(1); 
        (*LST)["URS"] = 0;
    }

    //mWait_Trans = noTrans when transmission is finished (emRespondTransComplete) or UTOE = 0
    if ((flag == emRespondTransComplete) || (flag == emFinishTransData)) {
        mWait_Trans = emNoTrans;
    } 

    if ((unsigned int)(*LUOER)["UTOE"] == 1) { //Transmission is enabled
        if (flag == emRespondRecevComplete) {
            if (mWait_Trans == emSingWaitStopBit) {
                (*LST)["UTS"] = 1;
                mTransStatus = emOPERATION;
                //Notify single byte transmission with wait stop bit
                mSingleTransEvent.notify((double)((Crlin3_common::CalcBitTime())*50),SC_NS);
            } else if (((unsigned int)(*LTRC)["RTS"] == 1) && (mWait_Trans == emMultiWaitStopBit)) { // Mod by SonTran: No.8-1,8-2,8-3,9-1 Don't care constrain of full duplex and half duplex in UART mode 
                //Handle multi byte transmission with wait stop bit function
                mNoMultiTransWaitStopBit = true;
                mMultiTransWaitStopBitEvent.notify((double)((Crlin3_common::CalcBitTime())*50),SC_NS);
            }
        } else if ((flag == emFinishTransData) && mIsLUTDRWrite) {
            mWait_Trans = emSingNoWaitStopBit; //Notify transmission without wait stop bit
            mIsLUTDRWrite = false;
            (*LST)["UTS"] = 1;
            mSingleTxDataValue = (unsigned int)(*LUTDR);
            //Notify single byte transmission with wait stop bit
            mSingleTransEvent.notify();
        }
    }

}//}}} 

void Crlin3_uart::UpdateErrorStatus(eERROR_FLAG error_kind) 
{//{{{
    bool error_intr = false; 
    if (error_kind == emSYNCErrorOrExpandBit) {
        (*LEST)["EXBT"] = 0x1; 
        (*LST)["ERR"] = 0x1;
        error_intr = true;
        ___re_printf("info","Expansion bit is matched.\n");
    } else if (error_kind == emChecksumErrorOrMatchID) {
        (*LEST)["IDMT"] = 0x1; 
        (*LEST)["EXBT"] = 0x1; 
        (*LST)["ERR"] = 0x1;
        error_intr = true;
        ___re_printf("info","Expansion bit is matched with data comparison.\n");
    } else {
        if (error_kind <= emRespondError) {
            switch (error_kind) {
                case emBitError :
                     ___re_printf("warning","The Bit error occurs in Uart mode.\n"); 
                     break;
                case emFrameError :
                     ___re_printf("warning","The Frame error occurs in Uart mode.\n");
                    // Modified by SonTran: No.39 Reception will perform even if error is detected (RLIN3v4)
                    Crlin3_common::mReception_index = 0;
                    mCurrent_status = emNormalStatus;
                    mCurrent_Error = emNoneError;
                     break;
                case emIdentifyError :
                     ___re_printf("warning","The Parity error occurs in Uart mode.\n");
                    // Modified by SonTran: No.39 Reception will perform even if error is detected (RLIN3v4)
                    Crlin3_common::mReception_index = 0;
                    mCurrent_status = emNormalStatus;
                    mCurrent_Error = emNoneError;
                     break;
                case emTimeOutOrOverrunError :
                     ___re_printf("warning","The Overrun error occurs in Uart mode.\n");
                    // Modified by SonTran: No.39 Reception will perform even if error is detected (RLIN3v4)
                    Crlin3_common::mReception_index = 0;
                    mCurrent_status = emNormalStatus;
                    mCurrent_Error = emNoneError;
                    break;
                default : 
                     break;
            }
            (*LEST) = (unsigned int)((unsigned int)(*LEST)|(1 << (unsigned int)error_kind));//assert error flag;
            (*LST)["ERR"] = 1;
            // Modified by SonTran: No.39, 59 Reception and Transmission will continue even if error is detected (RLIN3v4)
            if ((unsigned int)(*LST)["UTS"] == 1) {
                (*LTRC)["RTS"] = 0; //RTS is cleared when error detect during multi-byte transmission
//                if (mTrans_status == emUARTSINGLE) { // Modified by SonTran: No.58, 60 UTS is NOT cleared when bit error occur in Multi transmission
//                    (*LST)["UTS"] = 0;
//                } // removed by TrungPham 2020.03.23 to fix #117342
            }
            if ((unsigned int)(*LST)["URS"] == 1) {
                mIsReceiveStopBit = false;
                (*LST)["URS"] = 0;
            }
            UpdateStatus(emErrorDetect);
            error_intr = true;
//            Crlin3_common::StopOperation(false);
        } else {
            ___re_printf("warning","Operation can not perform because setting Bit Time is invalid .\n");
        }
    }
    if (error_intr == true) {
        parent_pointer->WriteLin3IntS(1);
    }
}//}}}

//Callback functions
void Crlin3_uart::cb_LWBR_LWBR0(RegCBstr str)
{//{{{
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LWBR , str.is_wr, (uint)str.pre_data, "LWBR0", emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LWBR , str.is_wr, (uint)str.pre_data, "LPRS", emR, 1, 3);
        parent_pointer->RegisterAccessCheck ( LWBR , str.is_wr, (uint)str.pre_data, "NSPB", emR, 4, 7);
    } else {
        if ((((unsigned int)(*LWBR)["NSPB"]) != (((unsigned int)(str.pre_data)>>4)&0xF))&&(((unsigned int)(*LWBR)["NSPB"]) != 0x0)&&(((unsigned int)(*LWBR)["NSPB"]) < 0x5)) {
            ___re_printf("warning","Samples per bit should be more than or equal to 6 in UART mode.\n");
            (*LWBR)["NSPB"] = ((unsigned int)(str.pre_data)>>4)&0xF ; 
        }
        //Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        if ((unsigned int)(*LWBR)["LWBR0"] != 0){
            ___re_printf("warning","LWBR.LWBR0 is set to initial value in UART mode\n");
            (*LWBR)["LWBR0"] = 0 ;
        }
    }
}//}}}

void Crlin3_uart::cb_LBRP0_BRP(RegCBstr str)
{//{{{ 
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LBRP0 , str.is_wr, (uint)str.pre_data, "BRP[7:0]", emR, 0, 7);
    }
}//}}}

void Crlin3_uart::cb_LBRP1_BRP(RegCBstr str)
{//{{{
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LBRP1 , str.is_wr, (uint)str.pre_data, "BRP[15:8]", emR, 0, 7);
    }

}//}}}  

void Crlin3_uart::cb_LSTC_LSTM(RegCBstr str)
{//{{{
    if (str.data != 0) {
        ___re_printf("warning","Should not set LSTC register in UART mode.\n"); 
    }
    (*LSTC) = 0x0;
}//}}}

void Crlin3_uart::cb_LMD_LMD(RegCBstr str)
{//{{{ 
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        if ((str.pre_data & 0x3) == Crlin3_common::emUartMode) {
            // RLIN3 is in IDLE mode or Operation mode
            parent_pointer->RegisterAccessCheck ( LMD , str.is_wr, (uint)str.pre_data, "LMD", emR, 0, 1);
            parent_pointer->RegisterAccessCheck ( LMD , str.is_wr, (uint)str.pre_data, "LCKS", emR, 2, 3);
            parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LIOS", emR, 4, 4);
            parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LRDNFS", emR, 5, 5);
        }
    } else {
        if (((unsigned int)(*LMD)["LMD"]) == Crlin3_common::emUartMode) {
            if ((unsigned int)(*LMD)["LIOS"] != 0) {
                ___re_printf("warning","Should not set LMD.LIOS bit in UART mode.\n"); 
                (*LMD)["LIOS"] = 0;
            }
            if ((unsigned int)(*LMD)["LCKS"] != 0) {
                ___re_printf("warning","UART mode should be operated with fa only.\n"); 
                (*LMD)["LCKS"] = 0x0;
            }
            parent_pointer->mMode = Crlin3_common::emUartMode;
        } else { // Slave mode only because master mode is omitted by tgt_acc
            if ((str.data & 0x3) == Crlin3_common::emSlaveFixBaudRateMode) {
                parent_pointer->mMode = Crlin3_common::emSlaveFixBaudRateMode;
            } else {
                parent_pointer->mMode = Crlin3_common::emSlaveAutoBaudRateMode;
            }
        }
    }
}//}}} 

void Crlin3_uart::SoftwareReset(unsigned int om0_val, RlinRegs rlin_reg, bool status)
{//{{{
    RlinRegs rlin_reg_pre;
    rlin_reg_pre.lwbr  =  (unsigned int)(*LWBR); 
    rlin_reg_pre.lbfc  =  (unsigned int)(*LBFC); 
    rlin_reg_pre.lsc   =  (unsigned int)(*LSC);  
    rlin_reg_pre.lwup  =  (unsigned int)(*LWUP); 
    rlin_reg_pre.lie   =  (unsigned int)(*LIE);  
    rlin_reg_pre.lede  =  (unsigned int)(*LEDE); 
    rlin_reg_pre.ldfc  =  (unsigned int)(*LDFC); 
    rlin_reg_pre.luor1 =  (unsigned int)(*LUOR1);

    (*LWBR )  = rlin_reg.lwbr  ;
    (*LBFC )  = rlin_reg.lbfc  ;
    (*LSC  )  = rlin_reg.lsc   ;
    (*LWUP )  = rlin_reg.lwup  ;
    (*LIE  )  = rlin_reg.lie   ;
    (*LEDE )  = rlin_reg.lede  ;
    (*LDFC )  = rlin_reg.ldfc  ;
    (*LUOR1)  = rlin_reg.luor1 ;
    if(om0_val==1) { //Reset is deasserted
        if (status) {
            if ((unsigned int) rlin_reg_pre.lwbr  !=  (unsigned int)(*LWBR)) {
                cb_LWBR_LWBR0(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lwbr, (unsigned int) (*LWBR)));
            }
            if ((unsigned int) rlin_reg_pre.lbfc  !=  (unsigned int)(*LBFC)) {
                (*LBFC) = (*LBFC) & 0x7F;
                cb_LBFC_UBLS(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lbfc, (unsigned int) (*LBFC)));
            }
            if ((unsigned int) rlin_reg_pre.lsc   !=  (unsigned int)(*LSC)) {
                (*LSC) = (*LSC) & 0x37;
                cb_LSC_IBHS(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lsc, (unsigned int) (*LSC)));
            }
            if ((unsigned int) rlin_reg_pre.lwup  !=  (unsigned int)(*LWUP)) {
                (*LWUP) = (*LWUP) & 0xF0;
                cb_LWUP_WUTL(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lwup, (unsigned int) (*LWUP)));
            }
            if ((unsigned int) rlin_reg_pre.lie   !=  (unsigned int)(*LIE)) {
                (*LIE) = (*LIE) & 0xF;
                cb_LIE_FTCIE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lie, (unsigned int) (*LIE)));
            }
            if ((unsigned int) rlin_reg_pre.lede  !=  (unsigned int)(*LEDE)) {
                (*LEDE) = (*LEDE) & 0xD;
                cb_LEDE_BERE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lede, (unsigned int) (*LEDE)));
            }
            if ((unsigned int) rlin_reg_pre.ldfc  !=  (unsigned int)(*LDFC)) {
                (*LDFC) = (*LDFC) & 0x2F;
                cb_LDFC_MDL(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.ldfc, (unsigned int) (*LDFC)));
            }
            if ((unsigned int) rlin_reg_pre.luor1 !=  (unsigned int)(*LUOR1)) {
                (*LUOR1) = (*LUOR1) & 0x1F;
                cb_LUOR1_UEBE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.luor1, (unsigned int) (*LUOR1)));
            }
        }
        rlin_reg.lwbr  =  (unsigned int)(*LWBR); 
        rlin_reg.lbrp0 =  (unsigned int)(*LBRP0);
        rlin_reg.lbrp1 =  (unsigned int)(*LBRP1);
        rlin_reg.lstc  =  (unsigned int)(*LSTC); 
        rlin_reg.lmd   =  (unsigned int)(*LMD);  
        rlin_reg.lbfc  =  (unsigned int)(*LBFC); 
        rlin_reg.lsc   =  (unsigned int)(*LSC);  
        rlin_reg.lwup  =  (unsigned int)(*LWUP);
        rlin_reg.lie   =  (unsigned int)(*LIE);
        rlin_reg.lede  =  (unsigned int)(*LEDE);
        rlin_reg.lcuc  =  (unsigned int)(*LCUC);
        rlin_reg.ltrc  =  (unsigned int)(*LTRC);
        rlin_reg.lmst  =  (unsigned int)(*LMST);
        rlin_reg.ldfc  =  (unsigned int)(*LDFC);
        rlin_reg.lidb  =  (unsigned int)(*LIDB);
        rlin_reg.lcbr  =  (unsigned int)(*LCBR);
        rlin_reg.ludb0 =  (unsigned int)(*LUDB0);
        rlin_reg.ldb1  =  (unsigned int)(*LDB1);
        rlin_reg.ldb2  =  (unsigned int)(*LDB2);
        rlin_reg.ldb3  =  (unsigned int)(*LDB3);
        rlin_reg.ldb4  =  (unsigned int)(*LDB4);
        rlin_reg.ldb5  =  (unsigned int)(*LDB5);
        rlin_reg.ldb6  =  (unsigned int)(*LDB6);
        rlin_reg.ldb7  =  (unsigned int)(*LDB7);
        rlin_reg.ldb8  =  (unsigned int)(*LDB8);
        rlin_reg.luoer =  (unsigned int)(*LUOER);
        rlin_reg.luor1 =  (unsigned int)(*LUOR1);
        rlin_reg.lutdr =  (unsigned int)(*LUTDR);
        rlin_reg.lurdr =  (unsigned int)(*LURDR);
        rlin_reg.luwtdr = (unsigned int)(*LUWTDR);
        rlin_reg.lbss = (unsigned int)(*LBSS);
        rlin_reg.lrss = (unsigned int)(*LRSS);
        if (status) {
            parent_pointer->UpdateRegReset(rlin_reg);
        }
        //Set configuration registers to RLIN3 common
        //Call function to transfer this struct to RLIN3 common
        (*LMST)["OMM0"] = 1;
        mTransStatus = emIDLE;
        mRecvStatus = emIDLE;
        Crlin3_common::SetConfigFactors(rlin_reg);
    } else { // Reset is asserted -> Stop operation
        (*LMST)["OMM0"]  = 0;
        (*LSTC)["LSTM"]  = 0;
        (*LTRC)          = 0;
        (*LEST)         = 0;
        parent_pointer->SetSelfTestFunc(false);
        mTransStatus = emRESET;
        mRecvStatus = emRESET;
        // Modified by SonTran: No.57 SW reset occurs while reception stop bit, data is stored into register and reception complete interrupt is asserted (RLIN3v4)
        if (!mIsReceiveStopBit) {
            (*LUOER)         = 0;
            (*LST)          = 0;
            Crlin3_common::StopOperation(false);
//            // Add by SonTran: No.61 All output signals are initialized in reset mode (RLIN3v4)
//            Crlin3_common::mWriteOutputEvent.notify();
        }
    }
}//}}}

void Crlin3_uart::cb_LBFC_UBLS(RegCBstr str)
{//{{{ 
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LBFC , str.is_wr, (uint)str.pre_data, "UBLS", emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LBFC , str.is_wr, (uint)str.pre_data, "UBOS", emR, 1, 1);
        parent_pointer->RegisterAccessCheck ( LBFC , str.is_wr, (uint)str.pre_data, "USBLS", emR, 2, 2);
        parent_pointer->RegisterAccessCheck ( LBFC , str.is_wr, (uint)str.pre_data, "UPS", emR, 3, 4);
        parent_pointer->RegisterAccessCheck ( LBFC , str.is_wr, (uint)str.pre_data, "URPS", emR, 5, 5);
        parent_pointer->RegisterAccessCheck ( LBFC , str.is_wr, (uint)str.pre_data, "UTPS", emR, 6, 6);
    }
}//}}} 

void Crlin3_uart::cb_LSC_IBHS(RegCBstr str)
{//{{{ 
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LSC , str.is_wr, (uint)str.pre_data, "IBHS", emR, 0, 2);
        parent_pointer->RegisterAccessCheck ( LSC , str.is_wr, (uint)str.pre_data, "IBS", emR, 4, 5);
    } else {//Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        if (((unsigned int)(*LSC)["IBHS"]) != 0){
            ___re_printf("warning","LSC.IBHS is set to initial value in UART mode\n");
            (*LSC)["IBHS"] = 0;
        }
    }
}//}}}

void Crlin3_uart::cb_LWUP_WUTL(RegCBstr str)
{//{{{
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LWUP , str.is_wr, (uint)str.pre_data, "WUTL", emR, 4, 7);
    } else {
        if ((unsigned int)(*LWUP) != 0) {//Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
            ___re_printf("warning","LWUP.WUTL is set to initial value in UART mode\n");
            (*LWUP) = 0; 
        } else {
            // Writing 0 is accepted
        }
    }
}//}}} 

void Crlin3_uart::cb_LIE_FTCIE(RegCBstr str)
{//{{{
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "FTCIE", emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "FRCIE", emR, 1, 1);
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "ERRIE", emR, 2, 2);
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "SHIE" , emR, 3, 3);
    } else {
        if ((unsigned int)(*LIE) != 0) {//Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
            ___re_printf("warning","LIE is set to initial value in UART mode\n");
            (*LIE) = 0; 
        } else {
            // Writing 0 is accepted
        }
    }
}//}}} 

void Crlin3_uart::cb_LEDE_BERE(RegCBstr str)
{//{{{ 
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "BERE" , emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "OERE", emR, 2, 2);
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "FERE" , emR, 3, 3);
    }

}//}}}

void Crlin3_uart::cb_LCUC_OM0(RegCBstr str)
{//{{{
    parent_pointer->RegisterAccessCheck ( LCUC , str.is_wr, (uint)str.pre_data, "OM1", emR, 1, 1);
    if (!Crlin3_common::CheckZeroClock()) {
        return;
    }
    if (((unsigned int)(*LCUC)["OM0"]) != ((unsigned int)(str.pre_data)&0x1)) {
        parent_pointer->SoftwareReset((unsigned int)(*LCUC)["OM0"], Crlin3::emRLIN3_UART);
    }
}//}}}

void Crlin3_uart::cb_LTRC_RTS(RegCBstr str)
{//{{{ 
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        if ((unsigned int)(*LST)["UTS"] == 1) {
            // Operation mode
            // RTS is read only
            parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "RTS", emR, 1, 1);
        } else {
            if (!Crlin3_common::CheckZeroClock()) {
                return;
            }
            //IDLE mode
            if (((unsigned int)(*LTRC)["RTS"] != (((unsigned int)(str.pre_data)&0x2) >> 1))) {
                if ((unsigned int)(*LTRC)["RTS"] == 1) {
                    if ((unsigned int)(*LUOER)["UTOE"] == 1) { // Modified by SonTran: No.51 LTRC.RTS should be set only when LUOER.UTOE bit is 1 (RLIN3v4)
                        // Add by SonTran: No.54 (1) LUOR1.UEBDCE should not be set to 1 for multi-byte communication (RLIN3v4)
                        if ((unsigned int)(*LUOR1)["UEBDCE"] == 1) {
                            ___re_printf("warning","LUOR1.UEBDCE should not be set to 1 for multi-byte communication.\n");
                        }
                        // Add by SonTran: No.48,49,50 LUOR1.UEBE,UEBDL,UECD should not be set to 1 for multi-byte communication (RLIN3v4)
                        if ((unsigned int)(*LUOR1)["UEBDL"] == 1) {
                            ___re_printf("warning","LUOR1.UEBDL should not be set to 1 for multi-byte communication.\n");
                        }
                        if ((unsigned int)(*LUOR1)["UECD"] == 1) {
                            ___re_printf("warning","LUOR1.UECD should not be set to 1 for multi-byte communication.\n");
                        }
                        if ((unsigned int)(*LUOR1)["UEBE"] == 1) {
                            ___re_printf("warning","LUOR1.UEBE should not be set to 1 for multi-byte communication. Transmission will not start.\n");
                            (*LTRC)["RTS"]  = 0;
                            return;
                        }
                        //UART multi-byte transmission
                        (*LST)["UTS"] = 1;
                        mTransStatus = emOPERATION;
                        if ((unsigned int)(*LDFC)["UTSW"] == 0) { //No wait stop bit
                            mWait_Trans = emMultiNoWaitStopBit; //Notify transmission when UART ready to transmit.
                        } else { //Wait stop bit
                            mWait_Trans = emMultiWaitStopBit; //Notify transmission when UART ready to transmit.
                        }
                        //Transmit multi-byte response data if no reception process
                        if((((unsigned int)(*LDFC)["UTSW"] == 1)&&((unsigned int)(*LST)["URS"] == 0)&&(mNoMultiTransWaitStopBit == false))||((unsigned int)(*LDFC)["UTSW"] == 0)) { // Mod by SonTran: No.8-1,8-2,8-3,9-1 Don't care constrain of full duplex and half duplex in UART mode 
                            //notify transmit multi byte without any wait
                            mMultiTransEvent.notify();
                        }
                    } else {
                        ___re_printf("warning","LTRC.RTS should not be set if LUOER.UTOE is not set to 1.\n");
                        (*LTRC)["RTS"] = 0; 
                    }
                }
            }
        }
    } else {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "RTS", emR0, 1, 1);
    }
}//}}} 

void Crlin3_uart::cb_LMST_OMM0(RegCBstr str)
{//{{{
    parent_pointer->RegisterAccessCheck ( LMST, str.is_wr, (uint)str.pre_data,"OMM0" , emR, 0, 0);
    parent_pointer->RegisterAccessCheck ( LMST, str.is_wr, (uint)str.pre_data,"OMM1" , emR, 1, 1);
}//}}}

void Crlin3_uart::cb_LST_FTC(RegCBstr str)
{//{{{
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"FTC" , emRW0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"ERR" , emR, 3, 3);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"UTS" , emR, 4, 4);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"URS" , emR, 5, 5);
    } else {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"FTC" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"ERR" , emR0, 3, 3);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"UTS" , emR0, 4, 4);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"URS" , emR0, 5, 5);
    }
}//}}}

void Crlin3_uart::cb_LEST_BER(RegCBstr str)
{//{{{
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"BER"  , emRW0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"OER"  , emRW0, 2, 2);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"FER"  , emRW0, 3, 3);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"EXBT" , emRW0, 4, 4);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"IDMT" , emRW0, 5, 5);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"UPER" , emRW0, 6, 6);
        if (!Crlin3_common::CheckZeroClock()) {
            return;
        }
        if (((unsigned int)(*LEST) == 0)&&((unsigned int)(str.pre_data) != 0)) {
            (*LST)["ERR"] = 0;
        }
    } else {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"BER"  , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"OER"  , emR0, 2, 2);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"FER"  , emR0, 3, 3);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"EXBT" , emR0, 4, 4);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"IDMT" , emR0, 5, 5);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"UPER" , emR0, 6, 6);
    }

}//}}}

void Crlin3_uart::cb_LIDB_ID(RegCBstr str)
{//{{{
    if ((unsigned int)(*LST)["URS"] == 1) {
        // RLIN3 is in Operation mode
        parent_pointer->RegisterAccessCheck ( LIDB, str.is_wr, (uint)str.pre_data,"ID" , emR, 0, 7);
    }
}//}}}

void Crlin3_uart::cb_LCBR_CKSM(RegCBstr str)
{//{{{ 
    parent_pointer->RegisterAccessCheck ( LCBR, str.is_wr, (uint)str.pre_data,"CKSM" , emR, 0, 7);
}//}}}

void Crlin3_uart::cb_LUDB0_UDB(RegCBstr str)
{//{{{  
    // LUDB0 is read only in Operation mode
    if ((unsigned int)(*LTRC)["RTS"] == 1) {
        parent_pointer->RegisterAccessCheck ( LUDB0, str.is_wr, (uint)str.pre_data,"UDB" , emR, 0, 7);
    }
    
}//}}}

void Crlin3_uart::cb_LDB1_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB1, 1);
}//}}}

void Crlin3_uart::cb_LDB2_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB2, 2);
}//}}}

void Crlin3_uart::cb_LDB3_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB3, 3);
}//}}}

void Crlin3_uart::cb_LDB4_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB4, 4);
}//}}}

void Crlin3_uart::cb_LDB5_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB5, 5);
}//}}}

void Crlin3_uart::cb_LDB6_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB6, 6);
}//}}}

void Crlin3_uart::cb_LDB7_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB7, 7);
}//}}}

void Crlin3_uart::cb_LDB8_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB8, 8);
}//}}}

void Crlin3_uart::CheckWriteLDBN(RegCBstr str, vpcl::re_register *reg, unsigned int index) 
{//{{{
    sc_assert(reg != NULL); 
    if (((unsigned int)(*LTRC)["RTS"] == 1)&&((unsigned int)(*reg) != (unsigned int)(str.pre_data))) {
         ___re_printf("warning","Data register LDB%d should not be written when RTS is 1.\n", index); 
         (*reg) = (unsigned int)(str.pre_data); 
    }
}//}}}

void Crlin3_uart::cb_LUOER_UTOE(RegCBstr str)
{//{{{
    RlinRegs rlin_reg;

    // LUOER is read 0 in reset mode
    if ((mTransStatus == emRESET) || (mRecvStatus == emRESET)) {
        parent_pointer->RegisterAccessCheck ( LUOER, str.is_wr, (uint)str.pre_data,"UTOE" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LUOER, str.is_wr, (uint)str.pre_data,"UROE" , emR0, 1, 1);
    }
    else { //IDLE or Operation mode
        if (!Crlin3_common::CheckZeroClock()) {
            return;
        }
        // Add by SonTran: No.42 In Expansion bit mode, LEDE.OERE should not be set when Expansion Bit Data comparison (LUOR1.UEBDCE) is enabled (RLIN3v4)
        if (((((unsigned int)(*LUOER)["UROE"] == 1) && ((((unsigned int)(str.pre_data)&0x2) >> 1) == 0))
          || (((unsigned int)(*LUOER)["UTOE"] == 1) && (((unsigned int)(str.pre_data)&0x1) == 0)))
         && ((unsigned int)(*LUOR1)["UEBE"] == 1) && ((unsigned int)(*LEDE)["OERE"] == 1) && ((unsigned int)(*LUOR1)["UEBDCE"] == 1)) {
            ___re_printf("warning","In Expansion bit mode, LEDE.OERE should not be set when LUOR1.UEBDCE is set.\n");
        }
        if (((unsigned int)(*LUOER)["UROE"] != (((unsigned int)(str.pre_data)&0x2) >> 1))&&((*LUOER)["UROE"] == 0)) {
            (*LST)["URS"] = 0; //Clear URS
            mIsDataRead = true;
            Crlin3_common::mReception_index = 0;
            mRecvStatus = emIDLE;
        } else if ((unsigned int)(*LUOER)["UROE"] == 1) {
            // Modified by SonTran: No.53 LUOER.UROE can NOT set during multi-byte transmission (RLIN3v4)
            if ((unsigned int)(*LTRC)["RTS"] == 1) {
                ___re_printf("warning","LUOER.UROE can not be set during multi-byte transmission.\n");
                (*LUOER)["UROE"] = 0;
            } else {
                rlin_reg.lwbr  =  (unsigned int)(*LWBR); 
                rlin_reg.lbrp0 =  (unsigned int)(*LBRP0);
                rlin_reg.lbrp1 =  (unsigned int)(*LBRP1);
                rlin_reg.lstc  =  (unsigned int)(*LSTC); 
                rlin_reg.lmd   =  (unsigned int)(*LMD);  
                rlin_reg.lbfc  =  (unsigned int)(*LBFC); 
                rlin_reg.lsc   =  (unsigned int)(*LSC);  
                rlin_reg.lede  =  (unsigned int)(*LEDE); 
                rlin_reg.lcuc  =  (unsigned int)(*LCUC); 
                rlin_reg.ltrc  =  (unsigned int)(*LTRC); 
                rlin_reg.lmst  =  (unsigned int)(*LMST); 
                rlin_reg.lst   =  (unsigned int)(*LST); 
                rlin_reg.lest  =  (unsigned int)(*LEST); 
                rlin_reg.ldfc  =  (unsigned int)(*LDFC); 
                rlin_reg.lidb  =  (unsigned int)(*LIDB); 
                rlin_reg.lcbr  =  (unsigned int)(*LCBR); 
                rlin_reg.ludb0 =  (unsigned int)(*LUDB0); 
                rlin_reg.ldb1  =  (unsigned int)(*LDB1); 
                rlin_reg.ldb2  =  (unsigned int)(*LDB2); 
                rlin_reg.ldb3  =  (unsigned int)(*LDB3); 
                rlin_reg.ldb4  =  (unsigned int)(*LDB4); 
                rlin_reg.ldb5  =  (unsigned int)(*LDB5); 
                rlin_reg.ldb6  =  (unsigned int)(*LDB6); 
                rlin_reg.ldb7  =  (unsigned int)(*LDB7); 
                rlin_reg.ldb8  =  (unsigned int)(*LDB8); 
                rlin_reg.luoer =  (unsigned int)(*LUOER);
                rlin_reg.luor1 =  (unsigned int)(*LUOR1);
                rlin_reg.lutdr =  (unsigned int)(*LUTDR);
                rlin_reg.lurdr =  (unsigned int)(*LURDR);
                rlin_reg.luwtdr = (unsigned int)(*LUWTDR);
                rlin_reg.lbss = (unsigned int)(*LBSS);
                rlin_reg.lrss = (unsigned int)(*LRSS);
                Crlin3_common::SetConfigFactors(rlin_reg);
            }
        }

        if (((unsigned int)(*LUOER)["UTOE"] != ((unsigned int)(str.pre_data)&0x1))&&((unsigned int)(*LUOER)["UTOE"] == 0)) {
            (*LTRC)["RTS"] = 0; //Clear RTS
            (*LST)["UTS"]  = 0; //Clear UTS
            mTransStatus = emIDLE;
            mWait_Trans = emNoTrans; //Clear mWait_Trans
            mIsDataRead = true;
            Crlin3_common::mTransmission_index = 0;
            Crlin3_common::StopOperation(false);
        }
    }

}//}}}

void Crlin3_uart::cb_LUOR1_UEBE(RegCBstr str)
{//{{{ 
    // LUOR1 is read only in IDLE and Operation modes
    if ((mTransStatus != emRESET) && (mRecvStatus != emRESET)) {
        // RLIN3 is in IDLE mode and Operation mode
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UEBE"   , emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UEBDL"  , emR, 1, 1);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UEBDCE" , emR, 2, 2);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UTIGTS" , emR, 3, 3);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UECD"   , emR, 4, 4);
    } else {
        // Add by SonTran: No.54 (2)(3) LUOR1.UEBDCE should not be set to 1 when LUOR1.UEBE is not set or  LUOR1.UECD is set (RLIN3v4)
        if ((unsigned int)(*LUOR1)["UEBDCE"] == 1) {
            if ((unsigned int)(*LUOR1)["UEBE"] == 0) {
                ___re_printf("warning","LUOR1.UEBDCE should not be set when LUOR1.UEBE is not set.\n");
            }
            if ((unsigned int)(*LUOR1)["UECD"] == 1) {
                ___re_printf("warning","LUOR1.UEBDCE should not be set when LUOR1.UECD is set.\n");
            }
        }
    }
}//}}}

void Crlin3_uart::cb_LUTDR_UTD(RegCBstr str)
{//{{{
    bool enable_trans = true;
    if ((str.size == 1)&&((*LUOR1)["UEBE"] == 1)) {
        ___re_printf("warning","Register LUTDR should not be written by access size 8 bit when 9 bit communication is in progress.\n"); 
        (*LUTDR) = (unsigned int)(str.pre_data);
        enable_trans = false;
    }
    if ((unsigned int)(*LTRC)["RTS"] == 1) {
        ___re_printf("warning","Register LUTDR should not be written when multi-byte communication is in progress.\n");
        (*LUTDR) = (unsigned int)(str.pre_data);
        enable_trans = false;
    }
    if (mIsLUWTDRWrite == true) {
        ___re_printf("warning","Register LUTDR should not be written when LUWTDR is already written.\n");
        (*LUTDR) = (unsigned int)(str.pre_data);
        enable_trans = false;
    }
    if ((enable_trans == true) && ((unsigned int)(*LUOER)["UTOE"] == 1)) {
        if (!Crlin3_common::CheckZeroClock()) {
            return;
        }
        // Add by SonTran: No.52 In case LUOR1.UTIGTS = 1, if users write to LUTDR before the transmission interrupt occurs, the transmission interrupt does not occur (RLIN3v4)
        if (((unsigned int)(*LUOR1)["UTIGTS"] == 1)&&((unsigned int)(*LST)["UTS"] == 1)) {
            mCheckAccessLUTDR = true;
        }
        mIsLUTDRWrite = true;
        mTransStatus = emOPERATION;
        mWait_Trans = emSingNoWaitStopBit; //Notify transmission without wait stop bit
        // Mod by SonTran: No.8-1,8-2,8-3,9-1 Don't care constrain of full duplex and half duplex in UART mode 
        //notify single byte transmission without any wait
        if ((unsigned int)(*LST)["UTS"] == 0) {
            mIsLUTDRWrite = false;
            (*LST)["UTS"] = 1;
            mSingleTxDataValue = (unsigned int)(*LUTDR);
            mSingleTransEvent.notify();
        }
    }
    (*LUWTDR) = (unsigned int)(*LUTDR); // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address
}//}}}

void Crlin3_uart::cb_LURDR_URD(RegCBstr str)
{//{{{
    mIsDataRead = true;
    if ((str.size == 1)&&((*LUOR1)["UEBE"] == 1)) {
        ___re_printf("warning","Register LURDR should not be read by access size 8 bit when 9 bit communication is in progress.\n"); 
    }
    // LURDR is read only in Reset, IDLE and Operation modes
    parent_pointer->RegisterAccessCheck ( LURDR, str.is_wr, (uint)str.pre_data,"URD" , emR, 0, 8);
}//}}} 

void Crlin3_uart::cb_LUWTDR_UWTD(RegCBstr str)
{//{{{ 
    bool enable_trans = true;
    if ((str.size == 1)&&((*LUOR1)["UEBE"] == 1)) {
        ___re_printf("warning","Register LUWTDR should not be written by access size 8 bit when 9 bit communication is in progress.\n"); 
        (*LUWTDR) = (unsigned int)(str.pre_data);
        enable_trans = false;
    }
    if ((unsigned int)(*LTRC)["RTS"] == 1) {
        ___re_printf("warning","Register LUWTDR should not be written when multi-byte communication is in progress.\n");
        (*LUWTDR) = (unsigned int)(str.pre_data);
        enable_trans = false;
    }
    if ((unsigned int)(*LST)["UTS"] == 1) {
        ___re_printf("warning","Register LUWTDR should not be written when LUTDR is already written.\n");
        (*LUWTDR) = (unsigned int)(str.pre_data);
        enable_trans = false;
    }
    if (!mCheckAccessLUWTDR) {
        ___re_printf("warning","Register LUWTDR should not be written when receiving data in Half duplex mode.\n");
    }
    if ((enable_trans == true)&&((unsigned int)(*LUOER)["UTOE"] == 1)) {
        if (!Crlin3_common::CheckZeroClock()) {
            return;
        }
        mIsLUWTDRWrite = true;
        mWait_Trans = emSingWaitStopBit; //Notify transmission with wait stop bit
        if ((unsigned int)(*LST)["URS"] == 0) { //No reception process
            // Mod by SonTran: No.8-1,8-2,8-3,9-1 Don't care constrain of full duplex and half duplex in UART mode 
            (*LST)["UTS"] = 1;
            mTransStatus = emOPERATION;
            //notify single byte transmission without any wait
            mSingleTransEvent.notify();
        }
    }
    (*LUTDR) = (unsigned int)(*LUWTDR); // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address
}//}}}

void Crlin3_uart::cb_LDFC_MDL(RegCBstr str)
{//{{{
    // LDFC is read only in Operation mode
    if ((unsigned int)(*LTRC)["RTS"] == 1) {
        parent_pointer->RegisterAccessCheck ( LDFC, str.is_wr, (uint)str.pre_data,"MDL" , emR, 0, 3);
        parent_pointer->RegisterAccessCheck ( LDFC, str.is_wr, (uint)str.pre_data,"UTSW" , emR, 5, 5);
    }
}//}}} 

// vim600: set foldmethod=marker :
