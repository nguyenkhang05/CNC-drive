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

#include "rlin3_slave.h"
#ifndef ___re_printf
#define ___re_printf parent_pointer->get_fileline(__FILE__, __LINE__); parent_pointer->_re_printf  
#endif

//Constructor
Crlin3_slave::Crlin3_slave(sc_module_name name, Crlin3 *parent) :
    Crlin3_slave_regif((std::string)name,32)
    ,Crlin3_common(name)
    ,rx_data("rx_data")
    ,rx_control("rx_control")
    ,tx_data("tx_data")
    ,tx_control("tx_control")
{//{{{
    Crlin3_slave_regif::set_instance_name((std::string)name);//set hierarchy name

    //Check Null Pointer
    sc_assert(parent != NULL); 

    parent_pointer = parent;

    tx_data.initialize(0xFFFFFFFF);
    tx_control.initialize(0x00000108);
      
    SC_METHOD (ReceiveMethod)
    dont_initialize();
    sensitive << rx_data;
    sensitive << rx_control;

    SC_METHOD (TimeoutHandlingMethod)
    dont_initialize();
    sensitive << mTimeoutEvent;

    SC_METHOD (StartRespondMethod)
    dont_initialize();
    sensitive << mStartRespondEvent;

    SC_METHOD (CheckRSDominantMethod)
    dont_initialize();
    sensitive << mCheckRSDominantEvent;

    //Initialize the variables
    mIsEnterSelfTest = false;
    mWriteLSTCTimes  = 0;
    mIsReceiveHeader = false;
    mIsReceiveResponse = false;
    mIsReceiveHeaderComplete = false; //Add by UyenLe : No.14 Conflicts of Status HTRC and other status bit (RLIN3v4)
    mIsRespondDataGroup = false;
    mIsHeaderTimeOut = false;
    mRespondFlag     = emNormalStatus;
    mStatus = emRESET;
    mDataValue = 0xFFFFFFFF;

}//}}}

// Destructor
Crlin3_slave::~Crlin3_slave()
{//{{{
}//}}}

std::string Crlin3_slave::slave_reg_command(const std::vector<std::string>& args)
{//{{{
    return this->reg_handle_command(args);
}//}}}

void Crlin3_slave::ResetSlave(bool is_active)
{//{{{
    if (is_active) {
        mIsEnterSelfTest = false;
        mWriteLSTCTimes  = 0;
        mDataValue = 0xFFFFFFFF;
        mStatus = emRESET;
        mIsReceiveHeader = false;
        mIsReceiveResponse = false;
        mIsReceiveHeaderComplete = false; //Add by UyenLe : No.14 Conflicts of Status HTRC and other status bit (RLIN3v4)
        mIsRespondDataGroup = false;
        mRespondFlag     = emNormalStatus;
        mTimeoutEvent.cancel();
        mIsHeaderTimeOut = false;
        Crlin3_common::SetSelftestMode(false);
        Crlin3_common::StopOperation(true);
    }
    Crlin3_slave_regif::EnableReset(is_active);
}//}}}

void Crlin3_slave::Lin3EnterSelfTest()
{//{{{
    mIsEnterSelfTest = true;
    Crlin3_common::SetSelftestMode(true);
    (*LSTC)          = 0x1 ;
}//}}}

void Crlin3_slave::UpdateRegsOfLin3()
{//{{{
    RlinRegs rlin_reg;
    rlin_reg.lwbr   =  (unsigned int)(*LWBR)  ;
    rlin_reg.lbrp0  =  (unsigned int)(*LBRP0) ;
    rlin_reg.lbrp1  =  (unsigned int)(*LBRP1) ;
    rlin_reg.lstc   =  (unsigned int)(*LSTC)  ;
    rlin_reg.lmd    =  (unsigned int)(*LMD)   ;
    rlin_reg.lbfc   =  (unsigned int)(*LBFC)  ;
    rlin_reg.lsc    =  (unsigned int)(*LSC)   ;
    rlin_reg.lwup   =  (unsigned int)(*LWUP)  ;
    rlin_reg.lie    =  (unsigned int)(*LIE)   ;
    rlin_reg.lede   =  (unsigned int)(*LEDE)  ;
    rlin_reg.lcuc   =  (unsigned int)(*LCUC)  ;
    rlin_reg.ltrc   =  (unsigned int)(*LTRC)  ;
    rlin_reg.lmst   =  (unsigned int)(*LMST)  ;
    rlin_reg.lst    =  (unsigned int)(*LST)   ;
    rlin_reg.lest   =  (unsigned int)(*LEST)  ;
    rlin_reg.ldfc   =  (unsigned int)(*LDFC)  ;
    rlin_reg.lidb   =  (unsigned int)(*LIDB)  ;
    rlin_reg.lcbr   =  (unsigned int)(*LCBR)  ;
    rlin_reg.ludb0  =  (unsigned int)(*LUDB0) ;
    rlin_reg.ldb1   =  (unsigned int)(*LDB1)  ;
    rlin_reg.ldb2   =  (unsigned int)(*LDB2)  ;
    rlin_reg.ldb3   =  (unsigned int)(*LDB3)  ;
    rlin_reg.ldb4   =  (unsigned int)(*LDB4)  ;
    rlin_reg.ldb5   =  (unsigned int)(*LDB5)  ;
    rlin_reg.ldb6   =  (unsigned int)(*LDB6)  ;
    rlin_reg.ldb7   =  (unsigned int)(*LDB7)  ;
    rlin_reg.ldb8   =  (unsigned int)(*LDB8)  ;
    rlin_reg.luoer  =  (unsigned int)(*LUOER) ;
    rlin_reg.luor1  =  (unsigned int)(*LUOR1) ;
    rlin_reg.lutdr  =  (unsigned int)(*LUTDR) ;
    rlin_reg.lurdr  =  (unsigned int)(*LURDR) ;
    rlin_reg.luwtdr =  (unsigned int)(*LUWTDR);
    rlin_reg.lurde  =  (unsigned int)(*LURDE) ;
    rlin_reg.lbss   =  (unsigned int)(*LBSS)  ;
    rlin_reg.lrss   =  (unsigned int)(*LRSS)  ;

    parent_pointer->UpdateRlin3Reg (rlin_reg);
}//}}}

bool Crlin3_slave::slave_reg_rd(unsigned int addr,unsigned char *p_data,unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_rd(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_slave::slave_reg_rd_dbg(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_rd_dbg(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_slave::slave_reg_wr(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{ 
    sc_assert(p_data != NULL);
    bool ret = this->reg_wr(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_slave::slave_reg_wr_dbg(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_wr_dbg(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}} 
 
std::string Crlin3_slave::GetCurrentStatus()
{//{{{
    // Check reception status of RLIN3
    std::string current_state = "";
    if (((unsigned int)(*LMST)&0x1) == 0) {
        current_state = "Reset";
    } else if ((unsigned int)(*LCUC)["OM1"] == 0) { //IDLE or Operation mode in Wakeup communication
        if (((unsigned int)(*LTRC)&0x1) == 1) { //Operation mode
            if ((((unsigned int)(*LDFC)>>4)&0x1) == 1) { //Reception
                current_state = "Wakeup Reception";
            } else { //Transmission
                current_state = "Wakeup Transmission";
            }
        } else { //IDLE mode
            current_state = "Idle";
        }
    } else { //IDLE or Operation mode in LIN communication
        if (((unsigned int)(*LTRC)&0x1) == 1) { //Operation mode
            if (((unsigned int)(*LST)&0x80) == 0x0) { //Header communication
                current_state = "Header Communication";
            } else if ((((unsigned int)(*LDFC)>>4)&0x1) == 0) { //Response Reception
                current_state = "Response Reception";
            } else { //Response Transmission
                current_state = "Response Transmission";
            }
        } else { //IDLE mode
            current_state = "Idle";
        }
    }
    return current_state;
}//}}}
 
void Crlin3_slave::ReceiveMethod(void)
{//{{{
    if (!Crlin3_common::CheckZeroClock()) {
        if (Crlin3_common::mPclk_Clock == 0) {
            ___re_printf("warning","The pclk period is equal 0.\n");
        } else {
            ___re_printf("warning","The clkc period is equal 0.\n");
        }
        return;
    }
    if (mStatus != emRESET) {
        unsigned int rx_control_val = (unsigned int)rx_control.read();
        unsigned int rx_data_val = (unsigned int)rx_data.read();
        if (mDataValue != rx_data_val) {
            Crlin3_common::ReceptionProcess(rx_data_val,rx_control_val,emCHECKBITERR);
        }
        if ((unsigned int)(*LMST)["OMM1"] == 0) { 
            if ((mDataValue != rx_data_val)&&((unsigned int)(*LTRC)["FTS"] == 1)&&((unsigned int)(*LDFC)["RCDS"] == 0)) {
                Crlin3_common::ReceptionProcess(rx_data_val,rx_control_val,emWAKEUP);
            }
        } else {
            if ((unsigned int)(*LTRC)["FTS"] == 1) {
                if (((rx_control_val>>6)&0x3) == 0x3) {
                    mIsReceiveHeader = true;
                    mIsReceiveResponse = false;
                    mIsReceiveHeaderComplete = false; //Add by UyenLe : No.14 Conflicts of Status HTRC and other status bit (RLIN3v4)
                    if (((unsigned int)(*LEDE)["TERE"] == 1)&&((unsigned int)(*LEDE)["LTES"] == 0)&&(mIsEnterSelfTest == false)) {//Header time out enable
                        mTimeoutEvent.cancel();
                        mIsHeaderTimeOut = true; 
                        TimeoutChecking();
                    }
                }
                if (mIsReceiveHeader == true) {
                    Crlin3_common::ReceptionProcess(rx_data_val,rx_control_val,emHEADER);
                } else if (mIsReceiveResponse) {
                    // Mod by SonTran: No.4-1,4-2 Respond preparation error condition
                    if (((!mIsEnterSelfTest&&(!(((unsigned int)(*LDFC)["RCDS"] == 1)&&((unsigned int)(*LTRC)["RTS"] == 1))))||(mIsEnterSelfTest&&((unsigned int)(*LMD)["LMD"] >= 2)&&((unsigned int)(*LDFC)["RCDS"] == 0)))&&(unsigned int)(*LTRC)["LNRR"] == 0) {
                        Crlin3_common::ReceptionProcess((unsigned int)rx_data.read(), (unsigned int)rx_control.read(),emRESP);
                    }
                }
            } else {
                if ((unsigned int)(*LDFC)["RCDS"] == 0) {
                    ___re_printf("warning","Reception operation is invalid when FTS is equal 0.\n");
                }
            }
        }
        mDataValue = rx_data_val;
    }
}//}}}

void Crlin3_slave::SetSlaveClock(double clkc, double pclk)
{//{{{
    if (!Crlin3_common::CheckZeroClock()) {
        //cancel all processes
        mTimeoutEvent.cancel();
        mStartRespondEvent.cancel();
        mCheckRSDominantEvent.cancel();
    } 
    Crlin3_common::SetLinClock(clkc,pclk);
}//}}}

void Crlin3_slave::UpdateSlaveRegs(RlinRegs rlin_reg) 
{//{{{
    bool start = false;
    if ((mIsEnterSelfTest == true)&&((rlin_reg.lmd&0x3) == 0)) {
        if (((rlin_reg.lcuc&0x1) == 1)&&((unsigned int)(*LCUC)["OM0"] == 0)) {
            mStatus = emOPERATION;
        }
        if (((unsigned int)(*LTRC)["FTS"] == 0)&&((rlin_reg.ltrc&0x1) == 1)) {
            start = true;
        }
    }
    if (mStatus != emRESET) {
        (*LWBR )  = rlin_reg.lwbr  ;
        (*LBFC )  = rlin_reg.lbfc  ;
        (*LSC  )  = rlin_reg.lsc   ;
        (*LWUP )  = rlin_reg.lwup  ;
        (*LIE  )  = rlin_reg.lie   ;
        (*LEDE )  = rlin_reg.lede  ;
        (*LDFC )  = rlin_reg.ldfc  ;
    }
    (*LBRP0)  = rlin_reg.lbrp0 ;
    (*LBRP1)  = rlin_reg.lbrp1 ;
    (*LSTC )  = rlin_reg.lstc  ;
    (*LMD  )  = rlin_reg.lmd   ;
    (*LCUC )  = rlin_reg.lcuc  ;
    (*LTRC)   = rlin_reg.ltrc  ;
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
    if ((mIsEnterSelfTest == true)&&((rlin_reg.lmd&0x3) == 0)&&(start == true)) { //master selftest mode
        RlinRegs config_regs; 
        config_regs.lwbr  = (unsigned int)(*LWBR);
        config_regs.lbrp0 = (unsigned int)(*LBRP0);
        config_regs.lbrp1 = (unsigned int)(*LBRP1);
        config_regs.lmd   = (unsigned int)(*LMD);
        config_regs.lbfc  = (unsigned int)(*LBFC);
        config_regs.lsc   = (unsigned int)(*LSC);
        config_regs.lstc   = (unsigned int)(*LSTC);
        config_regs.lwup  = (unsigned int)(*LWUP);
        config_regs.lie   = (unsigned int)(*LIE);
        config_regs.lede  = (unsigned int)(*LEDE);
        config_regs.lcuc  = (unsigned int)(*LCUC);
        config_regs.ldfc  = (unsigned int)(*LDFC);
        config_regs.lidb  = (unsigned int)(*LIDB);
        config_regs.luor1 = (unsigned int)(*LUOR1);
        config_regs.lbss  = (unsigned int)(*LBSS);
        config_regs.lrss  = (unsigned int)(*LRSS);
        //Call function to transfer this struct to RLIN3 common
        Crlin3_common::SetConfigFactors(config_regs);
    }
}//}}} 

void Crlin3_slave::TimeoutChecking()
{//{{{ 
    unsigned int bit_time = Crlin3_common::CalcBitTime();
    unsigned int num_byte = 0;    
    double wait_time = 0;
    if ((unsigned int)(*LDFC)["RFDL"] >= 8) {
        num_byte = 8;
    } else {
        num_byte = (unsigned int)(*LDFC)["RFDL"];
    }
    if (((unsigned int)(*LEDE)["LTES"] == 0)&&(mIsReceiveHeader == true)) { //Frame timeout
        wait_time = (double)(bit_time*emBitTimeUnit*64);
    } else {
        wait_time = 14*((double)num_byte+1)*(double)bit_time*emBitTimeUnit;
    }
    mTimeoutEvent.notify(wait_time,SC_NS); 
}//}}} 

void Crlin3_slave::TimeoutHandlingMethod(void) 
{//{{{
    if (mIsHeaderTimeOut == true) { //Frame timeout
        UpdateErrorStatus(emTimeOutOrOverrunError);
        ___re_printf("warning","Frame timeout error occurs during header reception process.\n");
    } else { //Response timeout
        UpdateErrorStatus(emTimeOutOrOverrunError);
        ___re_printf("warning","The timeout error occurs during response process.\n");
    }
}//}}}

// overwritten virtual functions
void Crlin3_slave::OutputData(unsigned int tx_control, unsigned int tx_data) 
{//{{{
    this->tx_data.write((sc_uint<32>)tx_data);
    this->tx_control.write((sc_uint<32>)tx_control);
}//}}}

void Crlin3_slave::UpdateRegisters(eREG_KIND reg_kind, unsigned int value)
{//{{{
    switch (reg_kind) {
        case emDataReg1:
            (*LDB1) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB1), "NoCheckSum", 0);//Dump Operation Info
            break;                       
        case emDataReg2:                 
            (*LDB2) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB2), "NoCheckSum", 0);//Dump Operation Info
            break;                       
        case emDataReg3:                 
            (*LDB3) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB3), "NoCheckSum", 0);//Dump Operation Info
            break;                       
        case emDataReg4:                 
            (*LDB4) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB4), "NoCheckSum", 0);//Dump Operation Info
            break;                       
        case emDataReg5:                 
            (*LDB5) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB5), "NoCheckSum", 0);//Dump Operation Info
            break;                       
        case emDataReg6:                 
            (*LDB6) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB6), "NoCheckSum", 0);//Dump Operation Info
            break;                       
        case emDataReg7:                 
            (*LDB7) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB7), "NoCheckSum", 0);//Dump Operation Info
            break;                       
        case emDataReg8:                 
            (*LDB8) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB8), "NoCheckSum", 0);//Dump Operation Info
            break;
        case emChecksumReg:
            (*LCBR) = value;
            break;
        case emPIDReg:
            (*LIDB) = value;
            break;
        case emLBSSReg:
            (*LBSS) = value;
            break;
        default:
            if ((unsigned int)(*LMD)["LMD"] == 2) { //The auto baud rate in slave mode
                (*LBRP0) = (value)&0xFF;
                (*LBRP1) = ((value) >> 8)&0xFF;
            }
            break;
    }
}//}}}

void Crlin3_slave::_re_printf(const std::string group, const char *message, ...)
{//{{{
    sc_assert(message != NULL);
    // print body
    va_list argptr;
    va_start(argptr, message);
    if (argptr == NULL) return;
    char str[1024];
    vsprintf(str, message, argptr);
    ___re_printf(group,str);
    va_end(argptr);
}//}}}

void Crlin3_slave::CheckRSDominantMethod(void)
{//{{{
    if (mRespondFlag != emRespSpaceComplete) {
        (*LRSS)["RSDD"] = 1;
    }
}//}}}

void Crlin3_slave::StartRespondMethod(void)
{//{{{
    mRespondFlag = emRespSpaceComplete;
    if ((((unsigned int)(*LTRC)["RTS"] == 1)&&((unsigned int)(*LDFC)["RCDS"] == 1)&&(mIsEnterSelfTest == false))
       ||((mIsEnterSelfTest)&&((((unsigned int)(*LMD)["LMD"] == 0)&&((unsigned int)(*LDFC)["RCDS"] == 0))
                             ||(((unsigned int)(*LMD)["LMD"] >= 2)&&((unsigned int)(*LDFC)["RCDS"] == 1))))) { //Check RTS, Transmit/recept, header complete
        //set data[8] to RLIN3 common
        unsigned int data[8];
        data[0] = (unsigned int)(*LDB1);
        data[1] = (unsigned int)(*LDB2);
        data[2] = (unsigned int)(*LDB3);
        data[3] = (unsigned int)(*LDB4);
        data[4] = (unsigned int)(*LDB5);
        data[5] = (unsigned int)(*LDB6);
        data[6] = (unsigned int)(*LDB7);
        data[7] = (unsigned int)(*LDB8);
        for (unsigned int i = 0;i<=7;i++) {
            Crlin3_common::SetDataTransfer(i+1,data[i]);
        }
        if (mIsEnterSelfTest == true) {
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
        //Transmission response data
        Crlin3_common::TransmitProcess(emRESP); //transfer respond       
    }
}//}}}

void Crlin3_slave::UpdateStatus(eSTATUS_FLAG flag) 
{//{{{
    if (((unsigned int)(*LMD)["LMD"]) != 0) {
        unsigned int data_buf[8] = {(unsigned int)(*LDB1),(unsigned int)(*LDB2),(unsigned int)(*LDB3),(unsigned int)(*LDB4),(unsigned int)(*LDB5),(unsigned int)(*LDB6),(unsigned int)(*LDB7),(unsigned int)(*LDB8)};
        //Add by UyenLe : No.33,34,35 (1) Condition that interrupts are not generated (RLIN3v4)
        bool is_ftc_0 = true;
        bool is_frc_0 = true;
        bool is_htrc_0 = true;

        if (flag == emSuccessReceiveSYNC) {
            (*LTRC)["LNRR"] = 0;// Clear LNRR when success receive SYNC field
            //Add by UyenLe : No.18 and No.17 (3) LTRC.RTS is automatically cleared when the RLIN3 module successfully detects a new Sync field (RLIN3v4)
            (*LTRC)["RTS"] = 0;// Clear RTS when success receive SYNC field
            (*LBSS)["SYCC"] = 1;
            (*LRSS)["RSDD"] = 0;
        }
        if (flag == emFrameHeaderComplete) {
            mIsReceiveHeaderComplete = true; //Add by UyenLe : No.14 Conflicts of Status HTRC and other status bit (RLIN3v4)
            if (mIsEnterSelfTest == true) { // slave self test
                //invert PID
                unsigned int data_tmp = 0;
                for (unsigned int i = 0; i < 8; i ++) {
                    data_tmp = data_tmp | ((((unsigned int)(*LIDB) >> i)&0x1) << (7-i)) ;
                }
                (*LIDB) = data_tmp;
            }
            mIsReceiveHeader = false;
            //Modified by UyenLe : No.35 (1) Condition that interrupts are not generated (RLIN3v4)
            double respond_space = (double)CalcBitTime()*emBitTimeUnit;
            if ((unsigned int)(*LST)["HTRC"] == 0) {
                (*LST)["HTRC"] = 1;
                double dominant_check_time = respond_space/2;
                mCheckRSDominantEvent.notify(dominant_check_time,SC_NS);
            } else {
                is_htrc_0 = false;
            }
            if ((unsigned int)(*LTRC)["LNRR"] == 0) {
                if (((unsigned int)(*LEDE)["TERE"] == 1)&&(mIsEnterSelfTest == false)) {// repond time out for respond reception/transmission 
                    mIsHeaderTimeOut = false; 
                    mTimeoutEvent.cancel();
                    TimeoutChecking();
                }
            }
            mRespondFlag = emNormalStatus;
            if ((mIsEnterSelfTest)&&((unsigned int)(*LDFC)["RCDS"] == 1)) {
                respond_space = (double)(((unsigned int)(*LSC)["IBHS"])*respond_space);
                mStartRespondEvent.notify(respond_space,SC_NS);
            }
            mIsReceiveResponse = true;
            if (((unsigned int)(*LDFC)["RCDS"] == 1)&&((unsigned int)(*LEDE)["BERE"] == 0x1)) {//notify checking bit error after 81.25% of bit time if checking is enable
                Crlin3_common::mOut_data[Crlin3_common::mTransmission_index] = (unsigned int)this->tx_data.read();
                Crlin3_common::mCheckBitErrorEvent.notify((double)(Crlin3_common::mBit_time*emBitTimeUnit*0.8125),SC_NS);
            }
            parent_pointer->DumpOperationInfo("Header", "Receiving", (unsigned int)(*LIDB), 0, "NoCheckSum", 0); //Dump Stat Info
        } else if ((flag == emRespSpaceComplete)) {
            // Mod by SonTran: No.1-2 Remove transmitting Respond space at the end of header
        } else if ((flag == emRespondTransComplete)&&((unsigned int)(*LDFC)["RCDS"] == 1)) {//Only update the register when RCDS = 1
            if ((unsigned int)(*LMST)["OMM1"] == 1) {
                if ((unsigned int)(*LDFC)["LSS"] == 0x0) { // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
                    parent_pointer->DumpOperationInfo("Response", "Transmitting", (unsigned int)(*LIDB), 0, "CheckSum", (unsigned int)(*LCBR)); //Dump Stat Info
                    parent_pointer->mTransmitDataAmount++;
                }
            } else {//wake up mode
                Crlin3_common::TransmitProcess(emRETIDLE);
            }
            //Modified by UyenLe : No.33(1) Condition that interrupts are not generated (RLIN3v4)
            if ((unsigned int)(*LST)["FTC"] == 0) {
                (*LST)["FTC"] = 1;
            } else {
                is_ftc_0 = false;
            }
 
        } else if ((flag == emRespondRecevComplete)&&((unsigned int)(*LDFC)["RCDS"] == 0)) {//Only update the register when RCDS = 0
            //Modified by UyenLe : No.34 (1) Condition that interrupts are not generated (RLIN3v4)
            if ((unsigned int)(*LST)["FRC"] == 0) {
                (*LST)["FRC"] = 1;
            } else {
                is_frc_0 = false;
            }
            if ((unsigned int)(*LMST)["OMM1"] == 1) {
                if ((unsigned int)(*LDFC)["LSS"] == 0x0) { // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
                    parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), 0, "CheckSum", (unsigned int)(*LCBR)); //Dump Operation Info
                    parent_pointer->mReceiveDataAmount++;
                }
            }
        } else if (flag == emOneByteRecevComplete) { // Modified by SonTran: No.27 Support respond preparation error in respond transmission (RFT = 1) (Slave mode only) (RLIN3v4)
            if ((unsigned int)(*LDFC)["RCDS"] == 0) {
                (*LST)["D1RC"] = 1;
            }
            if ((!mIsEnterSelfTest)&&((unsigned int)(*LTRC)["LNRR"] == 0)&&((unsigned int)(*LTRC)["RTS"] == 0)&&(!((unsigned int)((*LDFC)["RFDL"] == 1)&&((unsigned int)(*LDFC)["LSS"] == 1)))) { // Mod by SonTran: No.4-1,4-2 Respond preparation error condition
                (*LST)["D1RC"] = 1;
                UpdateErrorStatus(emRespondError);
                return;
            } 
            parent_pointer->mReceiveDataAmount++;
        } else if ((flag == emErrorDetect) || (flag == emSyncErrorDetect)) { // Mod by SonTran: No.2-3 SYNC field error flag
            // Mod by SonTran: No.5-1 Only clearing LTRC.FTS in Wake-up mode of Slave mode
            if (((unsigned int)(*LMST)["OMM1"]) == 0) { //Wakeup mode is selected
                (*LTRC)["FTS"] = 0;
                mStatus = emIDLE;
            }
            (*LTRC)["RTS"] = 0;
            mIsReceiveHeader = false;
            mIsReceiveResponse = false;
            mIsHeaderTimeOut = false;
            mIsRespondDataGroup = false;
            mRespondFlag     = emNormalStatus;
            mTimeoutEvent.cancel();
        } else if (flag == emFinishRev1Data) {
            parent_pointer->mReceiveDataAmount++;
        } else if (flag == emFinishTrans1Data) {
            parent_pointer->DumpOperationInfo("Response", "Transmitting", (unsigned int)(*LIDB), data_buf[(unsigned int)(Crlin3_common::mTransmission_index/3)], "NoCheckSum", 0); //Dump Stat Info
            parent_pointer->mTransmitDataAmount++;
        }

        //Modified by UyenLe : No.33, 34, 35 (1) Condition that interrupts are not generated (RLIN3v4)
        if ((flag == emRespondTransComplete)&&((unsigned int)(*LIE)["FTCIE"] == 1)&&(is_ftc_0)) { 
            parent_pointer->WriteLin3IntT(1);
        } else if (((flag == emFrameHeaderComplete)&&((unsigned int)(*LIE)["SHIE"] == 1)&&(is_htrc_0))||((flag == emRespondRecevComplete)&&((unsigned int)(*LIE)["FRCIE"] == 1)&&(is_frc_0))) {
            parent_pointer->WriteLin3IntR(1); 
        } else if ((flag == emErrorDetect)&&((unsigned int)(*LIE)["ERRIE"] == 1)) {
            parent_pointer->WriteLin3IntS(1);
        }
 
        if ((flag == emRespondTransComplete)||(flag == emRespondRecevComplete)) {
            if (mIsEnterSelfTest == true) { // slave self test
                //invert checksum and data
                unsigned int data_tmp = 0;
                for (unsigned int i = 0; i < 8; i ++) {
                    data_tmp = data_tmp | ((((unsigned int)(*LCBR) >> i)&0x1) << (7-i)) ;
                }
                (*LCBR) = data_tmp;
                unsigned int num_of_byte = Crlin3_common::CalcNumOfByte(emRESP); 
                for (unsigned int num_index = 0; num_index < (num_of_byte-1) ; num_index++) {
                    data_tmp = 0;
                    for (unsigned int j = 0; j < 8; j ++) {
                        data_tmp = data_tmp | (((data_buf[num_index] >> j)&0x1) << (7-j)) ;
                    }
                    data_buf[num_index] = data_tmp;
                }
                (*LDB1) = data_buf[0];
                (*LDB2) = data_buf[1];
                (*LDB3) = data_buf[2];
                (*LDB4) = data_buf[3];
                (*LDB5) = data_buf[4];
                (*LDB6) = data_buf[5];
                (*LDB7) = data_buf[6];
                (*LDB8) = data_buf[7];

                //Add by UyenLe : No.37, 38 (2) LTRC.FTS is cleared by transmission/reception completion (RLIN3v4)
                (*LTRC)["FTS"] = 0;
            }
            if ((unsigned int)(*LDFC)["LSS"] == 0x0) {
                // Mod by SonTran: No.5-1 Only clearing LTRC.FTS in Wake-up mode of Slave mode
                if (((unsigned int)(*LMST)["OMM1"]) == 0) { //Wakeup mode is selected
                    (*LTRC)["FTS"] = 0;
                    mStatus = emIDLE;
                }
                mIsReceiveResponse = false;
                mRespondFlag     = emNormalStatus;
                mTimeoutEvent.cancel();
            } else {
                mIsRespondDataGroup = true;
            }
            (*LTRC)["RTS"] = 0;
        }
    } else {
        if (flag == emFrameHeaderComplete) {
            mIsReceiveHeader = false;
            mRespondFlag = emNormalStatus;
            double respond_space = (double)CalcBitTime()*emBitTimeUnit;
            respond_space = (double)(((unsigned int)(*LSC)["IBHS"])*respond_space);
            mStartRespondEvent.notify(respond_space,SC_NS);
            mIsReceiveResponse = true;
        }
    }
}//}}}

void Crlin3_slave::UpdateErrorStatus(eERROR_FLAG error_kind) 
{//{{{
    switch (error_kind) {
        case emBitError :
             ___re_printf("warning","Bit error occurs, Slave transferring is stopped.\n"); 
             break;
        case emFrameError :
             ___re_printf("warning","Frame error occurs, Slave receiving is stopped.\n");
             break;
        case emSYNCErrorOrExpandBit :
             ___re_printf("warning","SYNC field error occurs, Slave receiving is stopped.\n"); 
             break;
        case emChecksumErrorOrMatchID :
             ___re_printf("warning","Checksum error occurs, Slave receiving is stopped.\n");
             break;
        case emIdentifyError :
        case emIdentifyErrorAndFramingError :
             ___re_printf("warning","PID error occurs, Slave receiving is stopped.\n");
             break;
        case emRespondError :
             ___re_printf("warning","Respond preparation error occurs, Slave operation is stopped.\n");
             break;
        default : 
             break;
    }
    // Mod by SonTran: No.2-3 SYNC field error flag
    if ((error_kind != emSYNCErrorOrExpandBit) || ((error_kind == emSYNCErrorOrExpandBit) && ((unsigned int)(*LEDE)["SFERE"] == 1))) {
        if (error_kind == emIdentifyErrorAndFramingError) {
            (*LEST) = (unsigned int)((unsigned int)(*LEST)|(1 << (unsigned int)emIdentifyError));//assert error flag;
        } else {
            (*LEST) = (unsigned int)((unsigned int)(*LEST)|(1 << (unsigned int)error_kind));//assert error flag;
        }
        (*LST)["ERR"] = 1;
    }
    if ((error_kind == emSYNCErrorOrExpandBit) && ((unsigned int)(*LEDE)["SFERE"] == 0)) {
        UpdateStatus(emSyncErrorDetect);
    } else {
        UpdateStatus(emErrorDetect);
    }
    if (error_kind != emIdentifyErrorAndFramingError) {
        Crlin3_common::StopOperation(false);
    }
}//}}}

//Callback functions
void Crlin3_slave::cb_LWBR_LWBR0(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LWBR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LWBR , str.is_wr, (uint)str.pre_data, "LWBR0", emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LWBR , str.is_wr, (uint)str.pre_data, "LPRS", emR, 1, 3);
        parent_pointer->RegisterAccessCheck ( LWBR , str.is_wr, (uint)str.pre_data, "NSPB", emR, 4, 7);
    } else {
        parent_pointer->RegisterAccessCheck ( LWBR , str.is_wr, (uint)str.pre_data, "LWBR0", emRW0, 0, 0);
        if (((unsigned int)(*LMD)["LMD"]) == 0x3) {//Fix baud rate mode
            if ((((unsigned int)(*LWBR)["NSPB"]) != (((unsigned int)(str.pre_data)>>4)&0xF))&&(((unsigned int)(*LWBR)["NSPB"]) != 0x0)&&(((unsigned int)(*LWBR)["NSPB"]) != 0xF)) {
                ___re_printf("warning","Samples per bit should be fixed 16 in LIN Slave Fix baud rate mode.\n");
                (*LWBR)["NSPB"] = ((unsigned int)(str.pre_data)>>4)&0xF ; 
            } 
        } else { //Auto detect baud rate mode
            //Modified by UyenLe : No.16 User can set 4 or 8 samples per bit (RLIN3v4)
            if ((((unsigned int)(*LWBR)["NSPB"]) != (((unsigned int)(str.pre_data)>>4)&0xF))&&(((unsigned int)(*LWBR)["NSPB"]) != 0x7)&&(((unsigned int)(*LWBR)["NSPB"]) != 0x3)) {
                ___re_printf("warning","Samples per bit should be fixed to 4 or 8 in LIN Slave Auto baud rate mode.\n");
                (*LWBR)["NSPB"] = ((unsigned int)(str.pre_data)>>4)&0xF ; 
            } 
        }
    }
}//}}}

void Crlin3_slave::cb_LBRP0_BRP(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LBRP0, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LBRP0 , str.is_wr, (uint)str.pre_data, "BRP[7:0]", emR, 0, 7);
    }
}//}}}

void Crlin3_slave::cb_LBRP1_BRP(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LBRP1, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LBRP1 , str.is_wr, (uint)str.pre_data, "BRP[15:8]", emR, 0, 7);
    }
}//}}}

void Crlin3_slave::cb_LSTC_LSTM(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LSTC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LSTC , str.is_wr, (uint)str.pre_data, "LSTM", emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LSTC , str.is_wr, (uint)str.pre_data, "LSTME", emR0, 1, 6);
        parent_pointer->RegisterAccessCheck (  LSTC , str.is_wr, (uint)str.pre_data, "LSFWC", emR, 7, 7);
    } else {
        // RLIN3 is in Reset mode
        if (str.is_wr == true) {
            if (!Crlin3_common::CheckZeroClock()) {
                (*LSTC)["LSTME"] = 0;
                return;
            }
            if (mIsEnterSelfTest != true) {
                if ((mWriteLSTCTimes == 0)&&((*LSTC) == emFirstSeq)) {
                    mWriteLSTCTimes = mWriteLSTCTimes+1;
                } else if ((mWriteLSTCTimes == 1)&&((*LSTC) == emSecondSeq)) {
                    mWriteLSTCTimes = mWriteLSTCTimes+1;
                } else if ((mWriteLSTCTimes == 2)&&((*LSTC) == emLastSeq)) {
                    mWriteLSTCTimes = 0;
                    //Modified by UyenLe : No.12(1),37,38(3) LIN Slave Auto Baud Rate mode is NOT supported in Selftest mode (RLIN3v4)
                    if (((unsigned int)(*LMD)["LMD"]) == Crlin3_common::emSlaveFixBaudRateMode) {//Fix baud rate mode
                        mIsEnterSelfTest = true;
                        Crlin3_common::SetSelftestMode(true);
                        parent_pointer->SetSelfTestFunc(true);
                    } else {
                        ___re_printf("warning","LIN Slave Auto Baud Rate mode is NOT supported in Self-test mode.\n");
                    }
                } else {
                    ___re_printf("warning","Unlock sequence key is wrong.\n");                
                    mWriteLSTCTimes = 0;
                    mIsEnterSelfTest = false;
                    Crlin3_common::SetSelftestMode(false);
                    parent_pointer->SetSelfTestFunc(false);
                    (*LSTC) = 0;
                }
            }
            (*LSTC)["LSTME"] = 0;
        } 
    }
}//}}}

void Crlin3_slave::cb_LMD_LMD(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LMD, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LMD , str.is_wr, (uint)str.pre_data, "LMD", emR, 0, 1);
        parent_pointer->RegisterAccessCheck ( LMD , str.is_wr, (uint)str.pre_data, "LCKS", emR, 2, 3);
        parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LIOS", emR, 4, 4);
        parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LRDNFS", emR, 5, 5);
    } else {
        // Slave mode only because Master mode and Uart mode is omitted by tgt_acc
        if ((unsigned int)(*LMD)["LCKS"] != 0) {
            ___re_printf("warning","LIN Slave mode should be operated with fa only.\n"); 
            (*LMD)["LCKS"] = 0;
        }
        if (((unsigned int)(*LMD)["LMD"]) == 0x2) {
            parent_pointer->mMode = Crlin3_common::emSlaveAutoBaudRateMode;
        } else {
            parent_pointer->mMode = Crlin3_common::emSlaveFixBaudRateMode;
        }
    }
}//}}} 

void Crlin3_slave::SoftwareReset(unsigned int om0_val, RlinRegs rlin_reg, bool status)
{//{{{
    RlinRegs rlin_reg_pre;
    rlin_reg_pre.lwbr  =  (unsigned int)(*LWBR) ;
    rlin_reg_pre.lbfc  =  (unsigned int)(*LBFC) ;
    rlin_reg_pre.lsc   =  (unsigned int)(*LSC)  ;
    rlin_reg_pre.lwup  =  (unsigned int)(*LWUP) ;
    rlin_reg_pre.lie   =  (unsigned int)(*LIE)  ;
    rlin_reg_pre.lede  =  (unsigned int)(*LEDE) ;
    rlin_reg_pre.ldfc  =  (unsigned int)(*LDFC) ;
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
       //Set configuration registers to RLIN3 common
       //Call function to transfer this struct to RLIN3 common
        if (status) {
            if ((unsigned int) rlin_reg_pre.lwbr  !=  (unsigned int)(*LWBR)) {
                cb_LWBR_LWBR0(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lwbr, (unsigned int) (*LWBR)));
            }
            if ((unsigned int) rlin_reg_pre.lbfc  !=  (unsigned int)(*LBFC)) {
                (*LBFC) = (*LBFC) & 0x1;
                cb_LBFC_LBLT(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lbfc, (unsigned int) (*LBFC)));
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
                (*LEDE) = (*LEDE) & 0xDD;
                cb_LEDE_BERE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lede, (unsigned int) (*LEDE)));
            }
            if ((unsigned int) rlin_reg_pre.ldfc  !=  (unsigned int)(*LDFC)) {
                (*LDFC) = (*LDFC) & 0xBF;
                cb_LDFC_RFDL(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.ldfc, (unsigned int) (*LDFC)));
            }
            if ((unsigned int) rlin_reg_pre.luor1 !=  (unsigned int)(*LUOR1)) {
                (*LUOR1) = (*LUOR1) & 0x1F;
                cb_LUOR1_UEBE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.luor1, (unsigned int) (*LUOR1)));
            }
        }
        rlin_reg.lwbr   =  (unsigned int)(*LWBR)  ;
        rlin_reg.lbrp0  =  (unsigned int)(*LBRP0) ;
        rlin_reg.lbrp1  =  (unsigned int)(*LBRP1) ;
        rlin_reg.lstc   =  (unsigned int)(*LSTC)  ;
        rlin_reg.lmd    =  (unsigned int)(*LMD)   ;
        rlin_reg.lbfc   =  (unsigned int)(*LBFC)  ;
        rlin_reg.lsc    =  (unsigned int)(*LSC)   ;
        rlin_reg.lwup   =  (unsigned int)(*LWUP)  ;
        rlin_reg.lie    =  (unsigned int)(*LIE)   ;
        rlin_reg.lede   =  (unsigned int)(*LEDE)  ;
        rlin_reg.lcuc   =  (unsigned int)(*LCUC)  ;
        rlin_reg.ltrc   =  (unsigned int)(*LTRC)  ;
        rlin_reg.lmst   =  (unsigned int)(*LMST)  ;
        rlin_reg.ldfc   =  (unsigned int)(*LDFC)  ;
        rlin_reg.lidb   =  (unsigned int)(*LIDB)  ;
        rlin_reg.lcbr   =  (unsigned int)(*LCBR)  ;
        rlin_reg.ludb0  =  (unsigned int)(*LUDB0) ;
        rlin_reg.ldb1   =  (unsigned int)(*LDB1)  ;
        rlin_reg.ldb2   =  (unsigned int)(*LDB2)  ;
        rlin_reg.ldb3   =  (unsigned int)(*LDB3)  ;
        rlin_reg.ldb4   =  (unsigned int)(*LDB4)  ;
        rlin_reg.ldb5   =  (unsigned int)(*LDB5)  ;
        rlin_reg.ldb6   =  (unsigned int)(*LDB6)  ;
        rlin_reg.ldb7   =  (unsigned int)(*LDB7)  ;
        rlin_reg.ldb8   =  (unsigned int)(*LDB8)  ;
        rlin_reg.luoer  =  (unsigned int)(*LUOER) ;
        rlin_reg.luor1  =  (unsigned int)(*LUOR1) ;
        rlin_reg.lutdr  =  (unsigned int)(*LUTDR) ;
        rlin_reg.lurdr  =  (unsigned int)(*LURDR) ;
        rlin_reg.luwtdr =  (unsigned int)(*LUWTDR);
        rlin_reg.lbss   =  (unsigned int)(*LBSS)  ;
        rlin_reg.lrss   =  (unsigned int)(*LRSS)  ;
        if (status) {
            parent_pointer->UpdateRegReset(rlin_reg);
        }
        Crlin3_common::SetConfigFactors(rlin_reg);
        (*LMST)["OMM0"] = 1;
        mStatus = emIDLE;
    } else { // Reset is asserted -> Stop operation
        (*LMST)["OMM0"] = 0;
        (*LSTC)["LSTM"] = 0;
        (*LTRC)         = 0;
        (*LST)          = 0;
        (*LEST)         = 0;
        (*LBSS)         = 0;
        (*LRSS)         = 0;
        mWriteLSTCTimes = 0;
        mIsEnterSelfTest = false;
        mIsReceiveHeader = false;
        mIsReceiveResponse = false;
        mIsReceiveHeaderComplete = false; //Add by UyenLe : No.14 Conflicts of Status HTRC and other status bit (RLIN3v4)
        mIsRespondDataGroup = false;
        mRespondFlag     = emNormalStatus;
        mIsHeaderTimeOut = false;
        mDataValue       = 0xFFFFFFFF;
        mTimeoutEvent.cancel();
        mCheckRSDominantEvent.cancel();
        parent_pointer->SetSelfTestFunc(false);
        mStatus = emRESET;
        Crlin3_common::SetSelftestMode(false);
        Crlin3_common::StopOperation(false);
//        // Add by SonTran: No.61 All output signals are initialized in reset mode (RLIN3v4)
//        Crlin3_common::mWriteOutputEvent.notify();
    }
}//}}}

void Crlin3_slave::cb_LBFC_LBLT(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LBFC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LBFC , str.is_wr, (uint)str.pre_data, "LBLT", emR, 0, 0);
    }
}//}}} 

void Crlin3_slave::cb_LSC_IBHS(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LSC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LSC , str.is_wr, (uint)str.pre_data, "IBHS", emR, 0, 2);
        parent_pointer->RegisterAccessCheck ( LSC , str.is_wr, (uint)str.pre_data, "IBS", emR, 4, 5);
    } else { //Add by SonTran : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        if (((unsigned int)(*LDFC)["RCDS"] == 0) && ((unsigned int)(*LSC)["IBHS"] != 0)) { // LSC.IBHS is don't care in LIN mode reception 
            ___re_printf("warning","LSC.IBHS is set to initial value in LIN mode reception \n");
            (*LSC)["IBHS"] = 0; 
        }
    }
}//}}} 

void Crlin3_slave::cb_LWUP_WUTL(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LWUP, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LWUP , str.is_wr, (uint)str.pre_data, "LWTL", emR, 4, 7);
    }
}//}}} 

void Crlin3_slave::cb_LIE_FTCIE(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LIE, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "FTCIE", emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "FRCIE", emR, 1, 1);
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "ERRIE", emR, 2, 2);
        parent_pointer->RegisterAccessCheck ( LIE , str.is_wr, (uint)str.pre_data, "SHIE" , emR, 3, 3);
    }
}//}}} 

void Crlin3_slave::cb_LEDE_BERE(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LEDE, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "BERE" , emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "TERE", emR, 2, 2);
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "FERE" , emR, 3, 3);
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "SFERE", emR, 4, 4);
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "IPERE", emR, 6, 6);
        parent_pointer->RegisterAccessCheck ( LEDE , str.is_wr, (uint)str.pre_data, "LTES" , emR, 7, 7);
    }
}//}}}

void Crlin3_slave::cb_LCUC_OM0(RegCBstr str)
{//{{{
    if (!Crlin3_common::CheckZeroClock()) {
        if (mStatus == emOPERATION) {
            // RLIN3 is in Operation mode
            parent_pointer->RegisterAccessCheck ( LCUC , str.is_wr, (uint)str.pre_data, "OM1", emR, 1, 1);
        }
        return;
    }
    if (((unsigned int)(*LCUC)["OM0"]) != ((unsigned int)(str.pre_data)&0x1)) {
        parent_pointer->SoftwareReset((unsigned int)(*LCUC)["OM0"], Crlin3::emRLIN3_SLAVE);
    }
    if (mStatus == emOPERATION) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (str.is_wr && mIsEnterSelfTest) {
            re_printf("warning","Bit LCUC.OM1 is not allowed to access when communication bit (LTRC.FTS) is set in Selftest mode.\n");
            (*LCUC)["OM1"] = (str.pre_data >> 1) & 0x1;
            return;
        }
        // RLIN3 is in Operation mode
        parent_pointer->RegisterAccessCheck ( LCUC , str.is_wr, (uint)str.pre_data, "OM1", emR, 1, 1);
    }
    if ((unsigned int)(*LCUC)["OM1"] != ((unsigned int)(str.pre_data>>1)&0x1)) {
        (*LMST)["OMM1"] = (unsigned int)(*LCUC)["OM1"];
    }
}//}}}

void Crlin3_slave::cb_LTRC_FTS(RegCBstr str)
{//{{{
    RlinRegs config_regs; 
    config_regs.lwbr  = (unsigned int)(*LWBR);
    config_regs.lbrp0 = (unsigned int)(*LBRP0);
    config_regs.lbrp1 = (unsigned int)(*LBRP1);
    config_regs.lmd   = (unsigned int)(*LMD);
    config_regs.lbfc  = (unsigned int)(*LBFC);
    config_regs.lsc   = (unsigned int)(*LSC);
    config_regs.lstc  = (unsigned int)(*LSTC);
    config_regs.lwup  = (unsigned int)(*LWUP);
    config_regs.lie   = (unsigned int)(*LIE);
    config_regs.lede  = (unsigned int)(*LEDE);
    config_regs.lcuc  = (unsigned int)(*LCUC);
    config_regs.ldfc  = (unsigned int)(*LDFC);
    config_regs.lidb  = (unsigned int)(*LIDB);
    config_regs.luor1 = (unsigned int)(*LUOR1);
    config_regs.lbss  = (unsigned int)(*LBSS);
    config_regs.lrss  = (unsigned int)(*LRSS);
    if (mStatus == emIDLE) {
        // IDLE mode
        // RTS is read 0 only
        // LNRR is read 0 in IDLE mode
        parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "RTS" , emR0, 1, 1);
        parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "LNRR", emR0, 2, 2);
        if (str.is_wr == true) {
            if (Crlin3_common::CheckZeroClock()&&((unsigned int)(*LTRC)["FTS"] != ((unsigned int)(str.pre_data)&0x1))&&((unsigned int)(*LTRC)["FTS"] == 1)) { //FTS is set 0 -> 1
                if (((unsigned int)(*LEDE)["BERE"] != 1)) { //Add by UyenLe : No.29 Operation Constrain (RLIN3v4) 
                    ___re_printf("warning","LEDE.BERE should be written to 1 in Slave mode.\n");
                }
                if (((unsigned int)(*LEDE)["FERE"] != 1)) { //Add by UyenLe : No.31 Operation Constrain (RLIN3v4) 
                    ___re_printf("warning","LEDE.FERE should be written to 1 in Slave mode.\n");
                }
                if (((unsigned int)(*LEDE)["IPERE"] != 1)) { //Add by UyenLe : No.32 Operation Constrain (RLIN3v4) 
                    ___re_printf("warning","LEDE.IPERE should be written to 1 in Slave mode.\n");
                }

                //Add by UyenLe : No.30 (b) Timeout enable constrain (RLIN3v4)
                if (((unsigned int)(*LEDE)["TERE"] == 1)&&((unsigned int)(*LMD)["LMD"] == Crlin3_common::emSlaveAutoBaudRateMode)) {
                    ___re_printf("warning","Timeout error should be disabled for Auto baud rate LIN Slave mode operation.\n");
                }

                //Add by UyenLe : No.20->26 LEST is cleared automatically when LTRC[0] bit is set in LIN Slave mode (RLIN3v4)
                (*LEST) = 0;
                (*LST)["ERR"] = 0; // LST.ERR is cleared to 0 when LEST = 0

                //Add by UyenLe : No.33, 34, 35 (2), 36 clear condition of status bit (RLIN3v4)
                (*LST) = (unsigned int)(*LST)&0x8; //clear FTC, FRC, D1RC, HTRC

                if (mIsEnterSelfTest == true) {
                    //Add by UyenLe : No.37,38 (1a) Users should set 3'b001 to LSC.IBHS in LIN Slave Self test mode (RLIN3v4)
                    if ((unsigned int)(*LSC)["IBHS"] != 1) {
                        ___re_printf("warning","LSC.IBHS should be set 3'b001 in LIN Slave Self test mode.\n");
                    }
                    config_regs.lcbr  = (unsigned int)(*LCBR);
                    UpdateRegsOfLin3();
                } else {
                    // Writing 1 is accepted
                }
                //Set configuration registers to RLIN3 common
                mStatus = emOPERATION;
                //Call function to transfer this struct to RLIN3 common
                Crlin3_common::SetConfigFactors(config_regs);
                if (((unsigned int)(*LDFC)["RCDS"] == 1)) {
                    if ((unsigned int)(*LMST)["OMM1"] == 0) { //Wakeup mode is selected
                        //wakeup mode, transfer wakeup
                        Crlin3_common::TransmitProcess(emWAKEUP); // transfer wake-up signal
                    } 
                }
            }
        }
    } else if (mStatus == emOPERATION) {
        // Operation mode
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LTRC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // FTS is read only
        parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "FTS" , emR, 0, 0);
        if (str.is_wr == true) {
            if (Crlin3_common::CheckZeroClock()&&((unsigned int)(*LTRC)["RTS"] != ((unsigned int)(str.pre_data>>1)&0x1))&&((unsigned int)(*LTRC)["RTS"] == 1)) { //RTS is set 1
                if (((unsigned int)(*LTRC)["LNRR"] != 1)&&((unsigned int)(*LST)["HTRC"] == 1)) { //Add by UyenLe : No.17 (1)(2)(4) Setting condition of RTS No.14 Conflicts of Status HTRC and other status bit (RLIN3v4)
                    mIsRespondDataGroup = false;
                    Crlin3_common::SetConfigFactors(config_regs);
                    // Mod by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
                    if (((unsigned int)(*LDFC)["RCDS"] == 1)&&(!mIsEnterSelfTest)) { //Transmit, header complete
                        unsigned int bit_time = Crlin3_common::CalcBitTime(); 
                        double respond_space = (double)(bit_time*emBitTimeUnit*((unsigned int)(*LSC)["IBHS"]));
                        mStartRespondEvent.notify(respond_space,SC_NS);
                        mIsReceiveResponse = true;
                        if ((unsigned int)(*LEDE)["BERE"] == 0x1) {//notify checking bit error after 81.25% of bit time if checking is enable
                            Crlin3_common::mOut_data[Crlin3_common::mTransmission_index] = (unsigned int) this->tx_data.read();
                            Crlin3_common::mCheckBitErrorEvent.notify((double)(Crlin3_common::mBit_time*emBitTimeUnit*0.8125),SC_NS);
                        }
                    }
                } else {
                    ___re_printf("warning","LTRC.RTS should not be set if LIN no response is set or Receive Header is not completed.\n");
                    (*LTRC)["RTS"] = 0; 
                }
            }
            if (((unsigned int)(*LTRC)["RTS"] != ((unsigned int)(str.pre_data>>1)&0x1))&&((unsigned int)(*LTRC)["RTS"] == 0)) { //RTS is cleared by CPU
                ___re_printf("warning","LTRC.RTS bit can not be cleared by CPU access.\n");
                (*LTRC)["RTS"] = 1; // RTS can not be clear by CPU
            }
            if (((unsigned int)(*LTRC)["RTS"] == 1)||(mIsRespondDataGroup)) { // Modified by UyenLe : No.19 User can set LTRC.LNRR bit when RTS = 0 in 1st data group in operation mode (RLIN3v4)
                (*LTRC)["LNRR"] = (unsigned int)((str.pre_data>>2)&0x1) ; //user should not write to this bit when RTS = 1 or not 1st data group 
            } else {
                if (((unsigned int)(*LTRC)["LNRR"] != ((unsigned int)(str.pre_data>>2)&0x1))&&((unsigned int)(*LTRC)["LNRR"] == 0)) { //RTS is cleared by CPU
                    ___re_printf("warning","LTRC.LNRR bit can not be cleared by CPU access.\n");
                    (*LTRC)["LNRR"] = 1; // RTS can not be clear by CPU
                }
            }
        }
    } else {
        // RLIN3 is in Reset mode
        // LNRR is read 0 in Reset mode
        parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "FTS" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "RTS" , emR0, 1, 1);
        parent_pointer->RegisterAccessCheck ( LTRC , str.is_wr, (uint)str.pre_data, "LNRR", emR0, 2, 2);
    }

}//}}}

void Crlin3_slave::cb_LMST_OMM0(RegCBstr str)
{//{{{
    parent_pointer->RegisterAccessCheck ( LMST, str.is_wr, (uint)str.pre_data,"OMM0" , emR, 0, 0);
    parent_pointer->RegisterAccessCheck ( LMST, str.is_wr, (uint)str.pre_data,"OMM1" , emR, 1, 1);
}//}}}  

void Crlin3_slave::cb_LST_FTC(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LST, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"FTC" , emRW0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"FRC" , emRW0, 1, 1);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"D1RC" , emRW0, 6, 6);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"HTRC" , emRW0, 7, 7);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"ERR" , emR, 3, 3);
    } else {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"FTC" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"FRC" , emR0, 1, 1);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"ERR" , emR0, 3, 3);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"D1RC" , emR0, 6, 6);
        parent_pointer->RegisterAccessCheck ( LST, str.is_wr, (uint)str.pre_data,"HTRC" , emR0, 7, 7);
    }
}//}}} 

void Crlin3_slave::cb_LEST_BER(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LEST, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"BER"  , emRW0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"TER"  , emRW0, 2, 2);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"FER"  , emRW0, 3, 3);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"SFER" , emRW0, 4, 4);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"CSER" , emRW0, 5, 5);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"IPER" , emRW0, 6, 6);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"RPER" , emRW0, 7, 7);
        if (!Crlin3_common::CheckZeroClock()) {
            return;
        }
        if (((unsigned int)(*LEST) == 0)&&((unsigned int)(str.pre_data) != 0)) {
            (*LST)["ERR"] = 0;
        }
    } else {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"BER"  , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"TER"  , emR0, 2, 2);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"FER"  , emR0, 3, 3);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"SFER" , emR0, 4, 4);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"CSER" , emR0, 5, 5);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"IPER" , emR0, 6, 6);
        parent_pointer->RegisterAccessCheck ( LEST, str.is_wr, (uint)str.pre_data,"RPER" , emR0, 7, 7);
    } 
}//}}} 

void Crlin3_slave::cb_LDFC_RFDL(RegCBstr str)
{//{{{
    if (str.is_wr == true) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LDFC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        if ((*LTRC)["RTS"] == 1) {
            parent_pointer->RegisterAccessCheck ( LDFC , true, (unsigned int)str.pre_data, "RFDL", emR, 0, 3);
            //Add by UyenLe : No.9 Users can NOT write to LSS bit if LTRC[1] bit is set (RLIN3v4)
            parent_pointer->RegisterAccessCheck ( LDFC , true, (unsigned int)str.pre_data, "LSS", emR, 7, 7);
        } else {
            //Add by UyenLe : No.8 If number of Response Data bytes is "0", then, users should only use Classic checksum (RLIN3v4)
            if (((unsigned int)(*LDFC)["RFDL"] == 0)&&(((unsigned int)(*LDFC)["LCS"]) == 1)) {
                _re_printf("warning","Classic checksum should be used when response data bytes is 0.\n");
            }
        }
        if (((*LTRC)["RTS"] == 1)||(mIsRespondDataGroup)) { //can not modify after setting RTS to 1 for first data group
            parent_pointer->RegisterAccessCheck ( LDFC , true, (unsigned int)str.pre_data, "RCDS", emR, 4, 4);
            parent_pointer->RegisterAccessCheck ( LDFC , true, (unsigned int)str.pre_data, "LCS", emR, 5, 5);
        }
        //Add by UyenLe : No.7 Users can NOT write to LDFC.RCDS if LTRC[0] bit is set in LIN Wake-up mode (RLIN3v4)
        if (((unsigned int)(*LMST) == 0x1)&&((unsigned int)(*LTRC)["FTS"] == 1)) { //Wakeup mode is selected and LTRC[0] bit is set
            parent_pointer->RegisterAccessCheck ( LDFC , true, (unsigned int)str.pre_data, "RCDS", emR, 4, 4);
        }

        //Add by UyenLe : No.30 (a) Timeout enable constrain (RLIN3v4)
        if ((((unsigned int)(str.pre_data>>7)&0x1) == 0)&&((unsigned int)(*LDFC)["LSS"] == 1)) { //LSS change from 0->1
            if ((unsigned int)(*LEDE)["TERE"] == 1) {
                ___re_printf("warning","Timeout error should be disabled for data group communication.\n");
            }
        }
    }
}//}}}

void Crlin3_slave::cb_LIDB_ID(RegCBstr str)
{//{{{
    if (mStatus == emOPERATION) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LIDB, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in Operation mode
        parent_pointer->RegisterAccessCheck ( LIDB, str.is_wr, (uint)str.pre_data,"ID" , emR, 0, 5);
        parent_pointer->RegisterAccessCheck ( LIDB, str.is_wr, (uint)str.pre_data,"IDP0" , emR, 6, 6);
        parent_pointer->RegisterAccessCheck ( LIDB, str.is_wr, (uint)str.pre_data,"IDP1" , emR, 7, 7);
    }
    if (mStatus == emIDLE) {
        if (mIsEnterSelfTest == false) { //self test mode
            parent_pointer->RegisterAccessCheck ( LIDB ,str.is_wr, (uint)str.pre_data , "ID"  , emR, 0, 5);
            parent_pointer->RegisterAccessCheck ( LIDB ,str.is_wr, (uint)str.pre_data , "IDP0", emR, 6, 6);
            parent_pointer->RegisterAccessCheck ( LIDB ,str.is_wr, (uint)str.pre_data , "IDP1", emR, 7, 7);
        }
    }
}//}}}

void Crlin3_slave::cb_LUDB0_UDB(RegCBstr str)
{//{{{  
    // LUDB0 is read only in Operation mode
    if (mStatus == emOPERATION) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LUDB0, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        parent_pointer->RegisterAccessCheck ( LUDB0, str.is_wr, (uint)str.pre_data,"UDB" , emR, 0, 7);
    } else {
        ___re_printf("warning","User should not access to LUDB0 register in LIN mode.\n");
        (*LUDB0) = 0;
    }
}//}}}

void Crlin3_slave::cb_LCBR_CKSM(RegCBstr str)
{//{{{
    //LCBR is read only in Operation mode and reset mode
    if ((mStatus == emRESET)||(mStatus == emOPERATION)) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LCBR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
         // RLIN3 is in reset mode or Operation mode
    	parent_pointer->RegisterAccessCheck ( LCBR, str.is_wr, (uint)str.pre_data,"CKSM" , emR, 0, 7);
    } else {
        if (mIsEnterSelfTest == false) { //self test mode
            parent_pointer->RegisterAccessCheck ( LCBR , str.is_wr, (unsigned int)str.pre_data, "CKSM"  , emR, 0, 7);
        }
    }
}//}}}

void Crlin3_slave::cb_LDB1_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB1, 1);
}//}}}  

void Crlin3_slave::cb_LDB2_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB2, 2);
}//}}} 

void Crlin3_slave::cb_LDB3_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB3, 3);
}//}}} 

void Crlin3_slave::cb_LDB4_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB4, 4);
}//}}} 

void Crlin3_slave::cb_LDB5_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB5, 5);
}//}}} 

void Crlin3_slave::cb_LDB6_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB6, 6);
}//}}} 

void Crlin3_slave::cb_LDB7_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB7, 7);
}//}}} 

void Crlin3_slave::cb_LDB8_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB8, 8);
}//}}} 

void Crlin3_slave::CheckWriteLDBN(RegCBstr str, vpcl::re_register *reg, unsigned int index)
{//{{{
    sc_assert(reg != NULL);
    if (str.is_wr == true) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(reg, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        if (((unsigned int)(*LTRC)["RTS"] == 1)&&((unsigned int)(*reg) != (unsigned int)(str.pre_data))) {
            ___re_printf("warning","Data registers LDB%d should not be written when RTS is 1.\n",index);
            (*reg) = (unsigned int)(str.pre_data); 
        }
    }
}//}}} 

void Crlin3_slave::cb_LUOER_UTOE(RegCBstr str)
{//{{{
    // LUOER is read 0 in reset mode
    if (mStatus == emRESET) {
        parent_pointer->RegisterAccessCheck ( LUOER, str.is_wr, (uint)str.pre_data,"UTOE" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck ( LUOER, str.is_wr, (uint)str.pre_data,"UROE" , emR0, 1, 1);
    } else { //Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LUOER, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        if ((unsigned int)(*LUOER) != 0) {
            ___re_printf("warning","LUOER is set to initial value in LIN mode\n");
            (*LUOER) = 0; 
        }
    }
}//}}}

void Crlin3_slave::cb_LUOR1_UEBE(RegCBstr str)
{//{{{  
    // LUOR1 is read only in IDLE and Operation modes
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LUOR1, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode and Operation mode
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UEBE"   , emR, 0, 0);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UEBDL"  , emR, 1, 1);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UEBDCE" , emR, 2, 2);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UTIGTS" , emR, 3, 3);
        parent_pointer->RegisterAccessCheck ( LUOR1, str.is_wr, (uint)str.pre_data,"UECD"   , emR, 4, 4);
    } else { //Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        if ((unsigned int)(*LUOR1) != 0) {
            ___re_printf("warning","LUOR1 is set to initial value in LIN mode\n");
            (*LUOR1) = 0; 
        }
    }
}//}}}

void Crlin3_slave::cb_LUTDR_UTD(RegCBstr str) // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address
{//{{{ 
    // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
    if (!parent_pointer->SelftestAccessCheck(LUTDR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
        return;
    }
    if ((unsigned int)(*LUTDR) != 0) {//Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        ___re_printf("warning","LUTDR is set to initial value in LIN mode\n");
        (*LUTDR) = 0; 
    }
}//}}}

void Crlin3_slave::cb_LURDR_URD(RegCBstr str)
{//{{{ 
    // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
    if (!parent_pointer->SelftestAccessCheck(LURDR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
        return;
    }
    parent_pointer->RegisterAccessCheck ( LURDR, str.is_wr, (uint)str.pre_data,"URD" , emR, 0, 8);
}//}}}  

void Crlin3_slave::cb_LUWTDR_UWTD(RegCBstr str) // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address
{//{{{
    // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
    if (!parent_pointer->SelftestAccessCheck(LUWTDR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
        return;
    }
    if ((unsigned int)(*LUWTDR) != 0) {//Add by UyenLe : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        ___re_printf("warning","LUWTDR is set to initial value in LIN mode\n");
        (*LUWTDR) = 0; 
    }
}//}}} 

// vim600: set foldmethod=marker :
