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

#include "rlin3_master.h"
#ifndef ___re_printf
#define ___re_printf parent_pointer->get_fileline(__FILE__, __LINE__); parent_pointer->_re_printf  
#endif

//Constructor
Crlin3_master::Crlin3_master(sc_module_name name, Crlin3 *parent) :
    Crlin3_master_regif((std::string)name,32)
    ,Crlin3_common(name)
    ,rx_data("rx_data")
    ,rx_control("rx_control")
    ,tx_data("tx_data")
    ,tx_control("tx_control")
{//{{{
    Crlin3_master_regif::set_instance_name((std::string)name);//set hierarchy name

    //Check Null Pointer
    sc_assert(parent != NULL); 

    parent_pointer = parent;

    tx_data.initialize(0xFFFFFFFF); 
    tx_control.initialize(0x00000108);       
  
    SC_METHOD(ReceiveMethod)
    dont_initialize();
    sensitive << rx_data;
    sensitive << rx_control;

    SC_METHOD(TimeoutHandlingMethod)
    dont_initialize();
    sensitive << mTimeoutEvent;

    SC_METHOD(TransRespondMethod)  // Add by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
    dont_initialize();
    sensitive << mStartRespondEvent;

    mIsEnterSelfTest = false;
    mWriteLSTCTimes  = 0;
    mIsFinishHeader = false;
    mStatus = emRESET;
    mDataValue = 0xFFFFFFFF;

}//}}}

// Destructor
Crlin3_master::~Crlin3_master()
{//{{{
}//}}}

void Crlin3_master::ResetMaster(bool is_active)
{//{{{
    if (is_active) {
        mIsEnterSelfTest = false;
        mWriteLSTCTimes  = 0;
        mIsFinishHeader = false;
        mDataValue = 0xFFFFFFFF;
        mStatus = emRESET;
        mTimeoutEvent.cancel();
        Crlin3_common::SetSelftestMode(false);
        Crlin3_common::StopOperation(true);
    }
    Crlin3_master_regif::EnableReset(is_active); 
}//}}}

void Crlin3_master::Lin3EnterSelfTest()
{//{{{
    mIsEnterSelfTest = true;
    Crlin3_common::SetSelftestMode(true);
    (*LSTC)          = 0x1 ;
}//}}}

void Crlin3_master::UpdateRegsOfLin3()
{//{{{
    if (parent_pointer->mMode == Crlin3_common::emMasterMode) {
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

std::string Crlin3_master::master_reg_command(const std::vector<std::string>& args)
{//{{{
    return this->reg_handle_command(args);
}//}}}

bool Crlin3_master::master_reg_rd(unsigned int addr,unsigned char *p_data,unsigned int size)
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_rd(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_master::master_reg_rd_dbg(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{
    sc_assert(p_data != NULL);
    bool ret = this->reg_rd_dbg(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_master::master_reg_wr(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{ 
    sc_assert(p_data != NULL);
    bool ret = this->reg_wr(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
bool Crlin3_master::master_reg_wr_dbg(unsigned int addr,unsigned char *p_data,unsigned int size) 
{//{{{ 
    sc_assert(p_data != NULL);
    bool ret = this->reg_wr_dbg(addr,p_data, size);
    UpdateRegsOfLin3();
    return ret;
}//}}}
 
void Crlin3_master::UpdateMasterRegs(RlinRegs rlin_reg) 
{//{{{
    bool start = false;
    if (((unsigned int)(*LTRC)["FTS"] == 0)&&((rlin_reg.ltrc&0x1) == 1)) {
        start = true;
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
    (*LTRC )  = rlin_reg.ltrc  ;
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
    if ((mIsEnterSelfTest == true)&&((rlin_reg.lmd&0x3) >= 0x2)&&(start == true)) { //slave selftest mode
        mStatus = emOPERATION;
        RlinRegs config_regs; 
        config_regs.lwbr  = (unsigned int)(*LWBR);
        config_regs.lbrp0 = (unsigned int)(*LBRP0);
        config_regs.lbrp1 = (unsigned int)(*LBRP1);
        config_regs.lmd   = (unsigned int)(*LMD);
        config_regs.lbfc  = (unsigned int)(*LBFC);
        config_regs.lsc   = (unsigned int)(*LSC);
        config_regs.lstc  = (unsigned int)(*LSTC);
        config_regs.lcbr  = (unsigned int)(*LCBR);
        config_regs.lwup  = (unsigned int)(*LWUP);
        config_regs.lie   = (unsigned int)(*LIE);
        config_regs.lede  = (unsigned int)(*LEDE);
        config_regs.lcuc  = (unsigned int)(*LCUC);
        config_regs.ldfc  = (unsigned int)(*LDFC);
        config_regs.lidb  = (unsigned int)(*LIDB);
        config_regs.luor1 = (unsigned int)(*LUOR1);
        config_regs.lbss = (unsigned int)(*LBSS);
        config_regs.lrss = (unsigned int)(*LRSS);
        //Call function to transfer this struct to RLIN3 common
        Crlin3_common::SetConfigFactors(config_regs);
        Crlin3_common::TransmitProcess(emHEADER);    
    }
}//}}}

void Crlin3_master::TransRespondMethod(void)// Add by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
{///{{{
    UpdateStatus(emRespSpaceComplete);
}///}}}

void Crlin3_master::ReceiveMethod(void)
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
            if ((mDataValue != rx_data_val)&&((unsigned int)(*LTRC)["FTS"] == 1)) {
                Crlin3_common::ReceptionProcess(rx_data_val,rx_control_val,emWAKEUP);
            }
        } else {
            if ((unsigned int)(*LTRC)["FTS"] == 1) {
                if (mIsFinishHeader && ((unsigned int)(*LDFC)["RFT"] == 0)) {
                    // Mod by SonTran: No.4-1,4-2 Respond preparation error condition
                    Crlin3_common::ReceptionProcess(rx_data_val,rx_control_val,emRESP);
                } 
            } else {
                if ((unsigned int)(*LDFC)["RFT"] == 0) {
                    ___re_printf("warning","Reception operation is invalid when FTS is equal 0.\n");
                }
            }
        }
        mDataValue = rx_data_val;
    }
}//}}}

void Crlin3_master::SetMasterClock(double clkc, double pclk)
{//{{{
    if (!Crlin3_common::CheckZeroClock()) {
        //cancel all processes
        mTimeoutEvent.cancel();
        mStartRespondEvent.cancel();
    } 
    Crlin3_common::SetLinClock(clkc,pclk);
}//}}}

void Crlin3_master::TimeoutChecking()
{//{{{
    unsigned int bit_time = Crlin3_common::CalcBitTime();
    unsigned int num_byte = 0;
    double wait_time = 0;
    if ((unsigned int)(*LDFC)["RFDL"] >= 8) {
        num_byte = 8;
    } else {
        num_byte = (unsigned int)(*LDFC)["RFDL"];
    }
    if ((unsigned int)(*LEDE)["LTES"] == 0) { //Frame timeout
        if ((unsigned int)(*LDFC)["CSM"] == 1) { //Enhance checksum
            wait_time = (emEnhancedChecksumConst + (14*((double)num_byte+1)))*(double)bit_time*emBitTimeUnit;
        } else { //Classic checksum
            wait_time = (emClassicChecksumConst + (14*((double)num_byte+1)))*(double)bit_time*emBitTimeUnit;
        }
    } else { //Response timeout
        wait_time = 14*((double)num_byte+1)*(double)bit_time*emBitTimeUnit;
    }
    mTimeoutEvent.notify(wait_time,SC_NS); 
}//}}}

void Crlin3_master::TimeoutHandlingMethod(void) 
{//{{{
    if ((unsigned int)(*LEDE)["LTES"] == 0) { //Frame timeout
        ___re_printf("warning","The frame timeout error occurs . Operation of LIN master is stopped!.\n");
    } else { //Response timeout
        ___re_printf("warning","The respond timeout error occurs . Operation of LIN master is stopped!.\n");
    }
    UpdateErrorStatus(emTimeOutOrOverrunError);
}//}}}

std::string Crlin3_master::GetCurrentStatus()
{//{{{
    // Check reception status of RLIN3
    std::string current_state = "";
    if (((unsigned int)(*LMST)&0x1) == 0) {
        current_state = "Reset";
    } else if ((unsigned int)(*LCUC)["OM1"] == 0) { //IDLE or Operation mode in Wakeup communication
        if (((unsigned int)(*LTRC)&0x1) == 1) { //Operation mode
            if (((unsigned int)(*LDFC)&0x10) == 0x10) { //Reception
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
            } else if (((unsigned int)(*LDFC)&0x10) == 0x0) { //Response Reception
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
 
// overwritten virtual functions
void Crlin3_master::OutputData(unsigned int tx_control, unsigned int tx_data) 
{//{{{
    this->tx_data.write((sc_uint<32>)tx_data);
    this->tx_control.write((sc_uint<32>)tx_control);
}//}}}

void Crlin3_master::UpdateRegisters(eREG_KIND reg_kind, unsigned int value)
{//{{{
    switch (reg_kind) {
        case emDataReg1:
            (*LDB1) = value;
            parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), (unsigned int)(*LDB1) , "NoCheckSum", 0); //Dump Stat Info
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
        default:
            (*LCBR) = value;
            break;
    }
}//}}}

void Crlin3_master::UpdateStatus(eSTATUS_FLAG flag) 
{//{{{
    if ((((unsigned int)(*LMD)["LMD"]) >= 0x2)&&(mIsEnterSelfTest == true)) { // slave self test
        // Mod by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
        if (flag == emFrameHeaderComplete) {
            unsigned int bit_time = Crlin3_common::CalcBitTime(); 
            double respond_space = (double)(bit_time*emBitTimeUnit*((unsigned int)(*LSC)["IBHS"]));
            if ((unsigned int)(*LDFC)["RFT"] == 0) { // Transmit response in case Slave selftest reception
                mStartRespondEvent.notify(respond_space,SC_NS);
            }
        } else if (flag == emRespSpaceComplete) {
            //set data[9] to RLIN3 common
            unsigned int data[8] = {0,0,0,0,0,0,0,0};
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
            //Transmission response data
            Crlin3_common::TransmitProcess(emRESP); //transfer respond;
        }
    } else {
        unsigned int data_buf[8] = {(unsigned int)(*LDB1),(unsigned int)(*LDB2),(unsigned int)(*LDB3),(unsigned int)(*LDB4),(unsigned int)(*LDB5),(unsigned int)(*LDB6),(unsigned int)(*LDB7),(unsigned int)(*LDB8)};
        if (flag == emFrameHeaderComplete) {
            mIsFinishHeader = true; // Mod by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
            (*LST)["HTRC"] = 1;
            parent_pointer->DumpOperationInfo("Header", "Transmitting", (unsigned int)(*LIDB), 0, "NoCheckSum", 0); //Dump Stat Info
            if ((((unsigned int)(*LMD)["LMD"]) == 0)&&(mIsEnterSelfTest == true)) { // master self test
                //invert PID
                unsigned int data_tmp = 0;
                for (unsigned int i = 0; i < 8; i ++) {
                    data_tmp = data_tmp | ((((unsigned int)(*LIDB) >> i)&0x1) << (7-i)) ;
                }
                (*LIDB) = data_tmp;
            }
            // Mod by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
            if (((unsigned int)(*LDFC)["RFT"] == 1)&&(((unsigned int)(*LDFC)["FSM"] == 0)||((unsigned int)(*LTRC)["RTS"] == 1))) { // transmit while master in Frame Combine mode or Frame Seperate mode and respond is enable
                unsigned int bit_time = Crlin3_common::CalcBitTime(); 
                double respond_space = (double)(bit_time*emBitTimeUnit*((unsigned int)(*LSC)["IBHS"]));
                mStartRespondEvent.notify(respond_space,SC_NS);
                if ((unsigned int)(*LEDE)["BERE"] == 0x1) {//notify checking bit error after 81.25% of bit time if checking is enable
                    Crlin3_common::mOut_data[Crlin3_common::mTransmission_index] = (unsigned int)this->tx_data.read();
                    Crlin3_common::mCheckBitErrorEvent.notify((double)(Crlin3_common::mBit_time*emBitTimeUnit*0.8125),SC_NS);
                }

            }
            // Time out checking start
            if (((unsigned int)(*LEDE)["FTERE"] == 1)&&((unsigned int)(*LEDE)["LTES"] == 1)&&(mIsEnterSelfTest == false)) { //Call timeout -> reponse for response process
                mTimeoutEvent.cancel();
                TimeoutChecking();
            }
        } else if ((flag == emRespSpaceComplete)) {
            // Mod by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
            //set data[9] to RLIN3 common
            unsigned int data[8] = {0,0,0,0,0,0,0,0};
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
            Crlin3_common::TransmitProcess(emRESP); //transfer respond;
        } else if ((flag == emRespondTransComplete)&&((unsigned int)(*LDFC)["RFT"] == 1)) {//Only update the register when RFT = 1
            (*LST)["FTC"] = 1;
            if ((unsigned int)(*LMST)["OMM1"] == 1) {
                if ((unsigned int)(*LDFC)["LSS"] == 0x0) { // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
                    parent_pointer->DumpOperationInfo("Response", "Transmitting", (unsigned int)(*LIDB), 0, "CheckSum", (unsigned int)(*LCBR)); //Dump Stat Info
                    parent_pointer->mTransmitDataAmount++;
                }
            } else {//wake up mode
                Crlin3_common::TransmitProcess(emRETIDLE);
            }
 
        } else if ((flag == emRespondRecevComplete)&&((unsigned int)(*LDFC)["RFT"] == 0)) {//Only update the register when RFT = 0
            (*LST)["FRC"] = 1;
            if ((unsigned int)(*LMST)["OMM1"] == 1) {
                if ((unsigned int)(*LDFC)["LSS"] == 0x0) { // Add by SonTran: No.1-3,1-4 Checksum is only transmitted at the end of last group data
                    parent_pointer->DumpOperationInfo("Response", "Receiving", (unsigned int)(*LIDB), 0, "CheckSum", (unsigned int)(*LCBR)); //Dump Operation Info
                    parent_pointer->mReceiveDataAmount++;
                }
            }
        } else if ((flag == emOneByteRecevComplete)&&((unsigned int)(*LDFC)["RFT"] == 0)) {
            parent_pointer->mReceiveDataAmount++;
            (*LST)["D1RC"] = 1;
            if (((unsigned int)(*LDFC)["FSM"] == 1)&&((unsigned int)(*LTRC)["RTS"] == 0)&&(!((unsigned int)((*LDFC)["RFDL"] == 1)&&((unsigned int)(*LDFC)["LSS"] == 1)))) { // Mod by SonTran: No.4-1,4-2 Respond preparation error condition
                UpdateErrorStatus(emRespondError);
                return;
            } 
        } else if (flag == emErrorDetect) {
            (*LTRC) = 0;
            mStatus = emIDLE;
            mTimeoutEvent.cancel();
            mIsFinishHeader = false;
        } else if (flag == emFinishRev1Data) {
            parent_pointer->mReceiveDataAmount++;
        } else if (flag == emFinishTrans1Data) {
            parent_pointer->DumpOperationInfo("Response", "Transmitting", (unsigned int)(*LIDB), data_buf[(unsigned int)(Crlin3_common::mTransmission_index/3)], "NoCheckSum", 0); //Dump Stat Info
            parent_pointer->mTransmitDataAmount++;
        }
 
        if ((((flag == emFrameHeaderComplete)&&(unsigned int)(*LIE)["SHIE"]))||((((flag == emRespondTransComplete)&&(unsigned int)(*LIE)["FTCIE"]))&&((unsigned int)(*LDFC)["RFT"] == 1))) { //use index for bits
            parent_pointer->WriteLin3IntT(1);
        } else if (((flag == emRespondRecevComplete)&&(unsigned int)(*LIE)["FRCIE"])&&((unsigned int)(*LDFC)["RFT"] == 0)) {
            parent_pointer->WriteLin3IntR(1); 
        } else if ((flag == emErrorDetect)&&(unsigned int)(*LIE)["ERRIE"]) {
            parent_pointer->WriteLin3IntS(1);
        }
 
        if ((flag == emRespondTransComplete)||(flag == emRespondRecevComplete)) {
            if ((((unsigned int)(*LMD)["LMD"]) == 0)&&(mIsEnterSelfTest == true)) { // master self test
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
            }
            if ((unsigned int)(*LDFC)["LSS"] == 0x0) {
                (*LTRC)["FTS"] = 0;
                mStatus = emIDLE;
                mIsFinishHeader = false;
                mTimeoutEvent.cancel();
            } 
            (*LTRC)["RTS"] = 0;
        }
    }
}//}}} 

void Crlin3_master::UpdateErrorStatus (eERROR_FLAG error_kind) 
{//{{{
    if (error_kind <= emRespondError) {
        switch (error_kind) {
            case emBitError :
                 ___re_printf("warning","Bit error occurs, Master transferring is stopped.\n"); 
                 break;
            case emPhysicalError :
                 ___re_printf("warning","Physical error occurs, Master transferring is stopped.\n"); 
                 break;
            case emFrameError :
                 ___re_printf("warning","Frame error occurs, Master receiving is stopped.\n");
                 break;
            case emChecksumErrorOrMatchID :
                 ___re_printf("warning","Checksum error occurs, Master receiving is stopped.\n");
                 break;
            case emRespondError :
                 ___re_printf("warning","Respond preparation error occurs, Master operation is stopped.\n");
                 break;
            default : 
                 break;
        }
        (*LEST) = (unsigned int)((unsigned int)(*LEST)| (1 << (unsigned int)error_kind));//assert error flag;
        (*LST)["ERR"] = 1;
        UpdateStatus(emErrorDetect);
        Crlin3_common::StopOperation(false);
    } else { //bit time error
        ___re_printf("warning","Operation can not perform because setting Bit Time is invalid .\n");
    }
}//}}} 

void Crlin3_master::cb_LWBR_LWBR0(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LWBR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LWBR , str.is_wr, (uint)str.pre_data, "LWBR0", emR, 0, 0);
        parent_pointer->RegisterAccessCheck (  LWBR , str.is_wr, (uint)str.pre_data, "LPRS", emR, 1, 3);
        parent_pointer->RegisterAccessCheck (  LWBR , str.is_wr, (uint)str.pre_data, "NSPB", emR, 4, 7);
    } else {
        if ((((unsigned int)(*LWBR)["NSPB"]) != (((unsigned int)(str.pre_data)>>4)&0xF))&&(((unsigned int)(*LWBR)["NSPB"]) != 0x0)&&(((unsigned int)(*LWBR)["NSPB"]) != 0xF)) {
            ___re_printf("warning","Samples per bit should be fixed 16 in LIN Master mode.\n");
            (*LWBR)["NSPB"] = ((unsigned int)(str.pre_data)>>4)&0xF ; 
        } 
    }
}//}}}

void Crlin3_master::cb_LBRP0_LBRP0(RegCBstr str)
{//{{{  
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LBRP0, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LBRP0 , str.is_wr, (uint)str.pre_data, "LBRP0", emR, 0, 7);
    }
}//}}}

void Crlin3_master::cb_LBRP1_LBRP1(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LBRP1, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LBRP1 , str.is_wr, (uint)str.pre_data, "LBRP1", emR, 0, 7);
    }
}//}}}

void Crlin3_master::cb_LSTC_LSTM(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LSTC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LSTC , str.is_wr, (uint)str.pre_data, "LSTM", emR, 0, 0);
        parent_pointer->RegisterAccessCheck (  LSTC , str.is_wr, (uint)str.pre_data, "LSTME", emR0, 1, 6);
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
                    mIsEnterSelfTest = true;
                    parent_pointer->SetSelfTestFunc(true);
                    Crlin3_common::SetSelftestMode(true);
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

void Crlin3_master::cb_LMD_LMD(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LMD, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        if ((str.pre_data & 0x3) == Crlin3_common::emMasterMode) {
            parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LMD", emR, 0, 1);
            parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LCKS", emR, 2, 3);
            parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LIOS", emR, 4, 4);
            parent_pointer->RegisterAccessCheck (  LMD , str.is_wr, (uint)str.pre_data, "LRDNFS", emR, 5, 5);
        }
    } else {
        if ((unsigned int)(*this->LMD)["LMD"] == Crlin3_common::emMasterMode) {
            parent_pointer->mMode = Crlin3_common::emMasterMode;
        } else {
            if ((str.data & 0x3) == Crlin3_common::emUartMode) {
                parent_pointer->mMode = Crlin3_common::emUartMode;
            } else if ((str.data & 0x3) == Crlin3_common::emSlaveFixBaudRateMode) {
                parent_pointer->mMode = Crlin3_common::emSlaveFixBaudRateMode;
            } else {
                parent_pointer->mMode = Crlin3_common::emSlaveAutoBaudRateMode;
            }
        }
    }
}//}}} 

void Crlin3_master::cb_LBFC_BLT(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LBFC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LBFC , str.is_wr, (uint)str.pre_data, "BLT", emR, 0, 3);
        parent_pointer->RegisterAccessCheck (  LBFC , str.is_wr, (uint)str.pre_data, "BDT", emR, 4, 5);
    }
}//}}} 

void Crlin3_master::cb_LSC_IBHS(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LSC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LSC , str.is_wr, (uint)str.pre_data, "IBHS", emR, 0, 2);
        parent_pointer->RegisterAccessCheck (  LSC , str.is_wr, (uint)str.pre_data, "IBS", emR, 4, 5);
    } else { //Add by SonTran : No.2 Users shall set "initial value" to 'Don't care' register bit (RLIN3v4)
        if (((unsigned int)(*LDFC)["RFT"] == 0) && ((unsigned int)(*LSC)["IBHS"] != 0)) { // LSC.IBHS is don't care in LIN mode reception 
            ___re_printf("warning","LSC.IBHS is set to initial value in LIN mode reception \n");
            (*LSC)["IBHS"] = 0; 
        }
    }
}//}}}

void Crlin3_master::cb_LWUP_WUTL(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LWUP, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LWUP , str.is_wr, (uint)str.pre_data, "WUTL", emR, 4, 7);
    }

}//}}} 

void Crlin3_master::cb_LIE_FTCIE(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LIE, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LIE , str.is_wr, (uint)str.pre_data, "FTCIE", emR, 0, 0);
        parent_pointer->RegisterAccessCheck (  LIE , str.is_wr, (uint)str.pre_data, "FRCIE", emR, 1, 1);
        parent_pointer->RegisterAccessCheck (  LIE , str.is_wr, (uint)str.pre_data, "ERRIE", emR, 2, 2);
        parent_pointer->RegisterAccessCheck (  LIE , str.is_wr, (uint)str.pre_data, "SHIE" , emR, 3, 3);
    }
}//}}}

void Crlin3_master::cb_LEDE_BERE(RegCBstr str)
{//{{{ 
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LEDE, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in IDLE mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LEDE , str.is_wr, (uint)str.pre_data, "BERE" , emR, 0, 0);
        parent_pointer->RegisterAccessCheck (  LEDE , str.is_wr, (uint)str.pre_data, "PBERE", emR, 1, 1);
        parent_pointer->RegisterAccessCheck (  LEDE , str.is_wr, (uint)str.pre_data, "FTERE", emR, 2, 2);
        parent_pointer->RegisterAccessCheck (  LEDE , str.is_wr, (uint)str.pre_data, "FERE" , emR, 3, 3);
        parent_pointer->RegisterAccessCheck (  LEDE , str.is_wr, (uint)str.pre_data, "LTES" , emR, 7, 7);
    }
}//}}}

void Crlin3_master::cb_LCUC_OM0(RegCBstr str)
{//{{{
    if (!Crlin3_common::CheckZeroClock()) {
        if (mStatus == emOPERATION) {
            // RLIN3 is in IDLE mode or Operation mode
            parent_pointer->RegisterAccessCheck (  LCUC , str.is_wr, (uint)str.pre_data, "OM1", emR, 1, 1);
        }
        return;
    }
    if (((unsigned int)(*LCUC)["OM0"]) != ((unsigned int)(str.pre_data)&0x1)) {
        parent_pointer->SoftwareReset((unsigned int)(*LCUC)["OM0"], Crlin3::emRLIN3_MASTER); // Call software reset
    }
    if (mStatus == emOPERATION) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (str.is_wr && mIsEnterSelfTest) {
            re_printf("warning","Bit LCUC.OM1 is not allowed to access when communication bit (LTRC.FTS) is set in Selftest mode.\n");
            (*LCUC)["OM1"] = (str.pre_data >> 1) & 0x1;
            return;
        }
        // RLIN3 is in Operation mode
        parent_pointer->RegisterAccessCheck (  LCUC , str.is_wr, (uint)str.pre_data, "OM1", emR, 1, 1);
    }
    if ((unsigned int)(*LCUC)["OM1"] != (((unsigned int)(str.pre_data)>>1)&0x1)) {
        (*LMST)["OMM1"] = (unsigned int)(*LCUC)["OM1"];
    }
}//}}}

void Crlin3_master::cb_LTRC_FTS(RegCBstr str)
{//{{{
    if (mStatus == emIDLE) {
        // IDLE mode
        // RTS is read 0 only
        parent_pointer->RegisterAccessCheck (  LTRC , str.is_wr, (uint)str.pre_data, "RTS", emR0, 1, 1);
        if (Crlin3_common::CheckZeroClock() == false) {
            return;
        }
        if (str.is_wr == true) {
            if (((unsigned int)(*LTRC)["FTS"] != ((unsigned int)(str.pre_data)&0x1))&&((unsigned int)(*LTRC)["FTS"] == 1)) { //FTS is set 0 -> 1
                if (((unsigned int)(*LEDE)["BERE"] != 1)) { //Add by UyenLe : No.3 Operation Constrain (RLIN3v4) 
                    ___re_printf("warning","LEDE.BERE should be written to 1 in Master mode.\n");
                }
                if (((unsigned int)(*LEDE)["FERE"] != 1)) { //Add by UyenLe : No.4 Operation Constrain (RLIN3v4) 
                    ___re_printf("warning","LEDE.FERE should be written to 1 in Master mode.\n");
                }
                //Set configuration registers to RLIN3 common
                mStatus = emOPERATION;
                RlinRegs config_regs; 
                config_regs.lwbr  = (unsigned int)(*LWBR);
                config_regs.lbrp0 = (unsigned int)(*LBRP0);
                config_regs.lbrp1 = (unsigned int)(*LBRP1);
                config_regs.lmd   = (unsigned int)(*LMD);
                config_regs.lbfc  = (unsigned int)(*LBFC);
                config_regs.lsc   = (unsigned int)(*LSC);
                config_regs.lstc  = (unsigned int)(*LSTC);
                config_regs.lcbr  = (unsigned int)(*LCBR);
                config_regs.lwup  = (unsigned int)(*LWUP);
                config_regs.lie   = (unsigned int)(*LIE);
                config_regs.lede  = (unsigned int)(*LEDE);
                config_regs.lcuc  = (unsigned int)(*LCUC);
                config_regs.ldfc  = (unsigned int)(*LDFC);
                config_regs.lidb  = (unsigned int)(*LIDB);
                config_regs.luor1 =  (unsigned int)(*LUOR1);
                config_regs.lbss =  (unsigned int)(*LBSS);
                config_regs.lrss =  (unsigned int)(*LRSS);
                //Call function to transfer this struct to RLIN3 common
                Crlin3_common::SetConfigFactors(config_regs);
                
                //Clear status registers LST and LEST
                (*LEST) = 0x0;
                (*LST) = 0;
                if (mIsEnterSelfTest == false) {
                    if ((unsigned int)(*LCUC)["OM1"] == 1) {
                        if (((unsigned int)(*LEDE)["FTERE"] == 1)&&((unsigned int)(*LEDE)["LTES"] == 0)) { //Call timeout for header transmission.
                            mTimeoutEvent.cancel();
                            TimeoutChecking();
                        }
                        Crlin3_common::TransmitProcess(emHEADER); // transfer header data
                    } else {
                        if ((unsigned int)(*LDFC)["RFT"] == 1) { //Transmission
                            //wakeup mode, transfer wakeup
                            Crlin3_common::TransmitProcess(emWAKEUP); // transfer wake-up signal
                        }
                    }
                } else {
                    UpdateRegsOfLin3();
                    Crlin3_common::TransmitProcess(emHEADER);
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
        parent_pointer->RegisterAccessCheck (   LTRC , str.is_wr, (uint)str.pre_data, "FTS", emR, 0, 0);
        if (str.is_wr == true) {
            if (((unsigned int)(*LTRC)["RTS"] != (((unsigned int)(str.pre_data)>>1)&0x1))&&((unsigned int)(*LTRC)["RTS"] == 1)) { //RTS is set 1
                RlinRegs config_regs; 
                config_regs.lwbr  = (unsigned int)(*LWBR);
                config_regs.lbrp0 = (unsigned int)(*LBRP0);
                config_regs.lbrp1 = (unsigned int)(*LBRP1);
                config_regs.lmd   = (unsigned int)(*LMD);
                config_regs.lbfc  = (unsigned int)(*LBFC);
                config_regs.lsc   = (unsigned int)(*LSC);
                config_regs.lstc  = (unsigned int)(*LSTC);
                config_regs.lcbr  = (unsigned int)(*LCBR);
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
                // Mod by SonTran: No.1-2 Remove transmitting Respond space at the end of header 
                if (Crlin3_common::CheckZeroClock()&&((unsigned int)(*LDFC)["RFT"] == 1)&&(((unsigned int)(*LDFC)["FSM"] == 1)&&((unsigned int)(*LST)["HTRC"] == 1))&&(!mIsEnterSelfTest)) { // Transmit, header complete, Frame Separate mode, not self-test mode
                    unsigned int bit_time = Crlin3_common::CalcBitTime(); 
                    double respond_space = (double)(bit_time*emBitTimeUnit*((unsigned int)(*LSC)["IBHS"]));
                    mStartRespondEvent.notify(respond_space,SC_NS);
                    if ((unsigned int)(*LEDE)["BERE"] == 0x1) {//notify checking bit error after 81.25% of bit time if checking is enable
                        Crlin3_common::mOut_data[Crlin3_common::mTransmission_index] = (unsigned int)this->tx_data.read();
                        Crlin3_common::mCheckBitErrorEvent.notify((double)(Crlin3_common::mBit_time*emBitTimeUnit*0.8125),SC_NS);
                    }
                }
            }
            if (((unsigned int)(*LTRC)["RTS"] != (((unsigned int)(str.pre_data)>>1)&0x1))&&((*LTRC)["RTS"] == 0)) { //RTS is cleared by CPU
                ___re_printf("warning","LTRC.RTS bit can not be cleared by CPU access.\n");
                (*LTRC)["RTS"] = 1; // RTS can not be clear by CPU
            }
        }
    } else {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck (   LTRC , str.is_wr, (uint)str.pre_data, "FTS" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck (   LTRC , str.is_wr, (uint)str.pre_data, "RTS" , emR0, 1, 1);
    }

}//}}}

void Crlin3_master::SoftwareReset(unsigned int om0_val, RlinRegs rlin_reg, bool status)
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
                (*LBFC) = (*LBFC) & 0x3F;
                cb_LBFC_BLT(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lbfc, (unsigned int) (*LBFC)));
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
                (*LIE) = (*LIE) & 0x0F;
                cb_LIE_FTCIE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lie, (unsigned int) (*LIE)));
            }
            if ((unsigned int) rlin_reg_pre.lede  !=  (unsigned int)(*LEDE)) {
                (*LEDE) = (*LEDE) & 0x8F;
                cb_LEDE_BERE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.lede, (unsigned int) (*LEDE)));
            }
            if ((unsigned int) rlin_reg_pre.ldfc  !=  (unsigned int)(*LDFC)) {
                cb_LDFC_RFDL(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.ldfc, (unsigned int) (*LDFC)));
            }
            if ((unsigned int) rlin_reg_pre.luor1 !=  (unsigned int)(*LUOR1)) {
                (*LUOR1) = (*LUOR1) & 0x1F;
                cb_LUOR1_UEBE(RegCBstr(0, true, 8, (unsigned int) rlin_reg_pre.luor1, (unsigned int) (*LUOR1)));
            }
        }
        rlin_reg.lwbr   = (unsigned int)(*LWBR)  ; 
        rlin_reg.lbrp0  = (unsigned int)(*LBRP0) ;
        rlin_reg.lbrp1  = (unsigned int)(*LBRP1) ;
        rlin_reg.lstc   = (unsigned int)(*LSTC)  ; 
        rlin_reg.lmd    = (unsigned int)(*LMD)   ;  
        rlin_reg.lbfc   = (unsigned int)(*LBFC)  ; 
        rlin_reg.lsc    = (unsigned int)(*LSC)   ;  
        rlin_reg.lwup   = (unsigned int)(*LWUP)  ; 
        rlin_reg.lie    = (unsigned int)(*LIE)   ;  
        rlin_reg.lede   = (unsigned int)(*LEDE)  ; 
        rlin_reg.lcuc   = (unsigned int)(*LCUC)  ; 
        rlin_reg.ltrc   = (unsigned int)(*LTRC)  ; 
        rlin_reg.lmst   = (unsigned int)(*LMST)  ; 
        rlin_reg.ldfc   = (unsigned int)(*LDFC)  ; 
        rlin_reg.lidb   = (unsigned int)(*LIDB)  ; 
        rlin_reg.lcbr   = (unsigned int)(*LCBR)  ; 
        rlin_reg.ludb0  = (unsigned int)(*LUDB0) ; 
        rlin_reg.ldb1   = (unsigned int)(*LDB1)  ; 
        rlin_reg.ldb2   = (unsigned int)(*LDB2)  ; 
        rlin_reg.ldb3   = (unsigned int)(*LDB3)  ; 
        rlin_reg.ldb4   = (unsigned int)(*LDB4)  ; 
        rlin_reg.ldb5   = (unsigned int)(*LDB5)  ; 
        rlin_reg.ldb6   = (unsigned int)(*LDB6)  ; 
        rlin_reg.ldb7   = (unsigned int)(*LDB7)  ; 
        rlin_reg.ldb8   = (unsigned int)(*LDB8)  ; 
        rlin_reg.luoer  = (unsigned int)(*LUOER) ;
        rlin_reg.luor1  = (unsigned int)(*LUOR1) ;
        rlin_reg.lutdr  = (unsigned int)(*LUTDR) ;
        rlin_reg.lurdr  = (unsigned int)(*LURDR) ;
        rlin_reg.luwtdr = (unsigned int)(*LUWTDR);
        rlin_reg.lbss   = (unsigned int)(*LBSS)  ;
        rlin_reg.lrss   = (unsigned int)(*LRSS)  ;
        if (status) {
            parent_pointer->UpdateRegReset(rlin_reg);
        }
        Crlin3_common::SetConfigFactors(rlin_reg);
        (*LMST)["OMM0"] = 1;
        mStatus = emIDLE;
    } else { // Reset is asserted -> Stop operation
        (*LMST)["OMM0"]  = 0;
        (*LSTC)["LSTM"]  = 0;
        (*LTRC)          = 0;
        (*LST)           = 0;
        (*LEST)          = 0;
        mWriteLSTCTimes  = 0;
        mIsEnterSelfTest = false;
        mIsFinishHeader = false;
        mDataValue       = 0xFFFFFFFF;
        mTimeoutEvent.cancel();
        parent_pointer->SetSelfTestFunc(false);
        mStatus = emRESET;
        Crlin3_common::SetSelftestMode(false);
        Crlin3_common::StopOperation(false);
//        // Add by SonTran: No.61 All output signals are initialized in reset mode (RLIN3v4)
//        Crlin3_common::mWriteOutputEvent.notify();
    }
}//}}}

void Crlin3_master::cb_LMST_OMM0(RegCBstr str)
{//{{{
    // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
    if (!parent_pointer->SelftestAccessCheck(LMST, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
        return;
    }
    parent_pointer->RegisterAccessCheck (   LMST, str.is_wr, (uint)str.pre_data,"OMM0" , emR, 0, 0);
    parent_pointer->RegisterAccessCheck (   LMST, str.is_wr, (uint)str.pre_data,"OMM1" , emR, 1, 1);
}//}}}

void Crlin3_master::cb_LST_FTC(RegCBstr str)
{//{{{
    if (mStatus != emRESET) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LST, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"FTC" , emRW0, 0, 0);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"FRC" , emRW0, 1, 1);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"D1RC" , emRW0, 6, 6);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"HTRC" , emRW0, 7, 7);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"ERR" , emR, 3, 3);
    } else {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"FTC" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"FRC" , emR0, 1, 1);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"ERR" , emR0, 3, 3);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"D1RC" , emR0, 6, 6);
        parent_pointer->RegisterAccessCheck (   LST, str.is_wr, (uint)str.pre_data,"HTRC" , emR0, 7, 7);
    }
}//}}}

void Crlin3_master::cb_LEST_BER(RegCBstr str)
{//{{{ 
    if (mStatus == emIDLE) {
        // RLIN3 is in IDLE mode
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"BER" , emRW0, 0, 0);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"PBER" , emRW0, 1, 1);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"FTER" , emRW0, 2, 2);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"FER" , emRW0, 3, 3);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"CSER" , emRW0, 5, 5);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"RPER" , emRW0, 7, 7);
        if (Crlin3_common::CheckZeroClock() == false) {
            return;
        }
        if (((unsigned int)(*LEST) == 0)&&((unsigned int)(str.pre_data) != 0)) {
            (*LST)["ERR"] = 0;
        }
    }
    if (mStatus == emOPERATION) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LEST, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in Operation mode
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"BER" , emR, 0, 0);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"PBER" , emR, 1, 1);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"FTER" , emR, 2, 2);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"FER" , emR, 3, 3);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"CSER" , emR, 5, 5);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"RPER" , emR, 7, 7);
    }
    if (mStatus == emRESET) {
        // RLIN3 is in Reset mode
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"BER" , emR0, 0, 0);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"PBER" , emR0, 1, 1);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"FTER" , emR0, 2, 2);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"FER" , emR0, 3, 3);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"CSER" , emR0, 5, 5);
        parent_pointer->RegisterAccessCheck (   LEST, str.is_wr, (uint)str.pre_data,"RPER" , emR0, 7, 7);
    }
}//}}}

void Crlin3_master::cb_LDFC_RFDL(RegCBstr str)
{//{{{
    if (str.is_wr == true) {
        if (mStatus == emOPERATION) {
            // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
            if (!parent_pointer->SelftestAccessCheck(LDFC, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
                return;
            }
            parent_pointer->RegisterAccessCheck (   LDFC , true, (unsigned int)str.pre_data, "RFT", emR, 4, 4);
            parent_pointer->RegisterAccessCheck (   LDFC , true, (unsigned int)str.pre_data, "CSM", emR, 5, 5);
            parent_pointer->RegisterAccessCheck (   LDFC , true, (unsigned int)str.pre_data, "FSM", emR, 6, 6);
            if (((unsigned int)(*LDFC)["RFT"] == 0) || ((((unsigned int)(*LDFC)["RFT"] == 1))&&((unsigned int)(*LDFC)["FSM"] == 0)) || ((unsigned int)(*LTRC)["RTS"] == 1)) { //Read-only when respond reception OR respond trans in Frame Combined mode OR respond data group while LTRC[1] = 1
                parent_pointer->RegisterAccessCheck (   LDFC , true, (unsigned int)str.pre_data, "RFDL", emR, 0, 3);
            }
        }
       
        //Modified by UyenLe : No.13 Timeout enable constrain (RLIN3v4)
        if (((unsigned int)(*LDFC)["LSS"] == 1)&&((((unsigned int)(str.pre_data)>>7)&0x1) == 0x0)) {
            if ((unsigned int)(*LDFC)["FSM"] == 0) {
                (*LDFC)["LSS"] = 0;
                ___re_printf("warning","Bit LDFC.LSS can not be set in Frame Combined Mode.\n");
            } else {
                if ((unsigned int)(*LEDE)["FTERE"] == 1) {
                    ___re_printf("warning","Timeout error should be disabled for data group communication.\n");
                }
            }
        }
    }
}//}}} 

void Crlin3_master::cb_LIDB_ID(RegCBstr str)
{//{{{
    if (mStatus == emOPERATION) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LIDB, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        // RLIN3 is in Operation mode
        parent_pointer->RegisterAccessCheck (   LIDB, str.is_wr, (uint)str.pre_data,"ID" , emR, 0, 5);
        parent_pointer->RegisterAccessCheck (   LIDB, str.is_wr, (uint)str.pre_data,"IDP0" , emR, 6, 6);
        parent_pointer->RegisterAccessCheck (   LIDB, str.is_wr, (uint)str.pre_data,"IDP1" , emR, 7, 7);
    }
}//}}}

void Crlin3_master::cb_LUDB0_UDB(RegCBstr str)
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

void Crlin3_master::cb_LCBR_CKSM(RegCBstr str)
{//{{{
    //LCBR is read only in Operation mode and reset mode
    if ((mStatus == emRESET)||(mStatus == emOPERATION)) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(LCBR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
         // RLIN3 is in reset mode or Operation mode
        parent_pointer->RegisterAccessCheck (  LCBR, str.is_wr, (uint)str.pre_data,"CKSM" , emR, 0, 7);
    } else {
        if (mIsEnterSelfTest == false) { //self test mode
            parent_pointer->RegisterAccessCheck(LCBR , str.is_wr, (uint)str.pre_data, "CKSM"  , emR, 0, 7);
        }
    }
}//}}}

void Crlin3_master::cb_LDB1_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB1, 1); 
}//}}} 

void Crlin3_master::cb_LDB2_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB2, 2); 
}//}}} 

void Crlin3_master::cb_LDB3_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB3, 3); 
}//}}} 

void Crlin3_master::cb_LDB4_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB4, 4); 
}//}}} 

void Crlin3_master::cb_LDB5_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB5, 5); 
}//}}} 

void Crlin3_master::cb_LDB6_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB6, 6); 
}//}}} 

void Crlin3_master::cb_LDB7_LDB(RegCBstr str)
{//{{{  
    CheckWriteLDBN(str, LDB7, 7); 
}//}}}  

void Crlin3_master::cb_LDB8_LDB(RegCBstr str)
{//{{{ 
    CheckWriteLDBN(str, LDB8, 8); 
}//}}} 

void Crlin3_master::CheckWriteLDBN(RegCBstr str, vpcl::re_register *reg, unsigned int index) 
{//{{{
    sc_assert(reg != NULL);
    if ((str.is_wr == true)&&((unsigned int)(*reg) != (unsigned int)(str.pre_data))) {
        // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
        if (!parent_pointer->SelftestAccessCheck(reg, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
            return;
        }
        if ((unsigned int)(*LDFC)["RFT"] == 1) {
            if(((unsigned int)(*LDFC)["FSM"] == 0)&&((unsigned int)(*LTRC)["FTS"] != 0)) {
                ___re_printf("warning","Data registers LDB%d should not be written when FTS is 1 in Frame Combined mode transmission.\n",index);
                (*reg) = (unsigned int)(str.pre_data); 
            } 
            if(((unsigned int)(*LDFC)["FSM"] == 1)&&((unsigned int)(*LTRC)["RTS"] != 0)) {
                ___re_printf("warning","Data registers LDB%d should not be written when RTS is 1 in Frame Separate mode transmission.\n",index);
                (*reg) = (unsigned int)(str.pre_data); 
            } 
        } else if ((unsigned int)(*LTRC)["FTS"] != 0) {
            ___re_printf("warning","Data registers LDB%d should not be written when FTS is 1 in reception.\n",index);
            (*reg) = (unsigned int)(str.pre_data); 
        }
    }
}//}}}

void Crlin3_master::cb_LUOER_UTOE(RegCBstr str)
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

void Crlin3_master::cb_LUOR1_UEBE(RegCBstr str)
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

void Crlin3_master::cb_LUTDR_UTD(RegCBstr str) // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address
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

void Crlin3_master::cb_LURDR_URD(RegCBstr str)
{//{{{ 
    // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode. (RLIN3v4)
    if (!parent_pointer->SelftestAccessCheck(LURDR, str.is_wr, (uint)str.pre_data, mStatus, mIsEnterSelfTest)){
        return;
    }
    parent_pointer->RegisterAccessCheck ( LURDR, str.is_wr, (uint)str.pre_data,"URD" , emR, 0, 8);
}//}}}   

void Crlin3_master::cb_LUWTDR_UWTD(RegCBstr str) // Add by SonTran: No.7-1 LUTDR and LUWTDR are the same register but differ address
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
