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

#include "rlin3.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//===============Crlin3 class=======================
Crlin3::Crlin3(sc_module_name name): 
           sc_module(name)
          ,Crlin3_regif((std::string)name,32)
          ,vpcl::tlm_tgt_if<32> (name)
          ,rstc_n("rstc_n")
          ,preset_n("preset_n")
          ,pclk("pclk")
          ,clkc("clkc")
          ,RX_DATA("RX_DATA")
          ,RX_CONTROL("RX_CONTROL")
          ,TX_DATA("TX_DATA")
          ,TX_CONTROL("TX_CONTROL")
          ,lin3_int_t("lin3_int_t")
          ,lin3_int_r("lin3_int_r")
          ,lin3_int_s("lin3_int_s")
          ,tx_data_m_sig("tx_data_m_sig")
          ,tx_control_m_sig("tx_control_m_sig")
          ,tx_data_s_sig("tx_data_s_sig")
          ,tx_control_s_sig("tx_control_s_sig")
          ,tx_data_u_sig("tx_data_u_sig")
          ,tx_control_u_sig("tx_control_u_sig")
          ,rx_data_m_sig("rx_data_m_sig")
          ,rx_control_m_sig("rx_control_m_sig")
          ,rx_data_s_sig("rx_data_s_sig")
          ,rx_control_s_sig("rx_control_s_sig")
          ,rx_data_u_sig("rx_data_u_sig")
          ,rx_control_u_sig("rx_control_u_sig")
{//{{{
    CommandInit(this->name());
    Crlin3_regif::set_instance_name(this->name());//set hierarchy name

    this->Initialize();
    mRlin3Reset = false;
    mCmdReset = false;
    mPCLK_freq = 0;
    mCLKC_freq = 0;

    mIsCmdResetStatus   = false;
    mResetPeriod        = 0;

    //Instance mMaster
    mMaster = new Crlin3_master("LIN_Master",this);
    sc_assert(mMaster != NULL);
    
    //Instance mSlave
    mSlave = new Crlin3_slave("LIN_Slave",this);
    sc_assert(mSlave != NULL);
    
    //Instance mUart
    mUart = new Crlin3_uart("LIN_Uart",this);
    sc_assert(mUart != NULL);
    
    //Instance mSelfTest
    mSelfTest = new Crlin3_selftest("LIN_SelfTest");
    sc_assert(mSelfTest != NULL);
    
    //intialize output port
    TX_DATA.initialize(0xFFFFFFFF);
    TX_CONTROL.initialize(0x00000108);
    lin3_int_t.initialize(false);
    lin3_int_r.initialize(false);
    lin3_int_s.initialize(false);

    //Intialize for the signal
    tx_data_m_sig=0xFFFFFFFF;
    tx_control_m_sig=0x00000108;
    tx_data_s_sig=0xFFFFFFFF;
    tx_control_s_sig=0x00000108;
    tx_data_u_sig=0xFFFFFFFF;
    tx_control_u_sig=0x00000108;
    rx_data_m_sig=0xFFFFFFFF;
    rx_control_m_sig=0x00000108;
    rx_data_s_sig=0xFFFFFFFF;
    rx_control_s_sig=0x00000108;
    rx_data_u_sig=0xFFFFFFFF;
    rx_control_u_sig=0x00000108;

    //connect port
    mSelfTest->tx_data_m(tx_data_m_sig);
    mSelfTest->tx_control_m(tx_control_m_sig);
    mSelfTest->tx_data_s(tx_data_s_sig);
    mSelfTest->tx_control_s(tx_control_s_sig);
    mSelfTest->tx_data_u(tx_data_u_sig);
    mSelfTest->tx_control_u(tx_control_u_sig);
    mMaster->rx_data(tx_data_m_sig);
    mMaster->rx_control(tx_control_m_sig);
    mSlave->rx_data(tx_data_s_sig);
    mSlave->rx_control(tx_control_s_sig);
    mUart->rx_data(tx_data_u_sig);
    mUart->rx_control(tx_control_u_sig);


    mMaster->tx_data(rx_data_m_sig);
    mMaster->tx_control(rx_control_m_sig);
    mSlave->tx_data(rx_data_s_sig);
    mSlave->tx_control(rx_control_s_sig);
    mUart->tx_data(rx_data_u_sig);
    mUart->tx_control(rx_control_u_sig);

    mSelfTest->rx_data_m(rx_data_m_sig);
    mSelfTest->rx_control_m(rx_control_m_sig);
    mSelfTest->rx_data_s(rx_data_s_sig);
    mSelfTest->rx_control_s(rx_control_s_sig);
    mSelfTest->rx_data_u(rx_data_u_sig);
    mSelfTest->rx_control_u(rx_control_u_sig);


    mSelfTest->tx_data(TX_DATA);
    mSelfTest->tx_control(TX_CONTROL);
    mSelfTest->rx_data(RX_DATA);
    mSelfTest->rx_control(RX_CONTROL);

    //method
    SC_METHOD(DeAssertIntrMethod)
    dont_initialize();
    sensitive << mDeAssertIntrEvent;

    SC_METHOD(WriteLin3IntMethod)
    dont_initialize();
    sensitive << mWriteLin3IntEvent;

    SC_METHOD(ResetMethod)
    dont_initialize();
    sensitive << rstc_n;
    sensitive << preset_n;

    SC_METHOD(CmdResetMethod)
    dont_initialize();
    sensitive << mCmdResetEvent;

    SC_METHOD(CancelCmdResetMethod)
    dont_initialize();
    sensitive << mCancelCmdResetEvent;

    SC_METHOD(CLKCPeriodMethod)
    dont_initialize();
    sensitive << clkc;

    SC_METHOD(PCLKPeriodMethod)
    dont_initialize();
    sensitive << pclk;

    #ifdef CWR_SYSTEMC
    //handle command
    SCML_COMMAND_PROCESSOR(handleCommand);
    SCML_ADD_COMMAND("tgt", 1, 8, "tgt <cmd> <arg0> <arg1> <arg2> ...", "prefix of tlm target if command");
    SCML_ADD_COMMAND("command", 1, 3, "command <param> <arg> <1/->", "prefix of setting module parameters");
    SCML_ADD_COMMAND("reg", 1, 8, "reg <cmd> <arg0> <arg1> <arg2> ...", "prefix of register command");
    #endif

    vpcl::tlm_if_tgt_parameter tgt_param;
    tgt_get_param(&tgt_param);
    tgt_param.fw_req_phase = tlm::END_REQ;
    tgt_set_param(&tgt_param);
    sc_time clock_value(0, SC_NS);
    tgt_param.bus_clk = clock_value;
    tgt_set_param(&tgt_param);
    SetLatency_TLM();
    mStatus = Crlin3_common::emRESET;

}//}}}

Crlin3::~Crlin3()
{//{{{
    delete mMaster;
    delete mSlave;
    delete mUart;
    delete mSelfTest; 
}//}}}

void Crlin3::SetSelfTestFunc(bool is_selftest) //this function use to enable/disable selftest mode
{//{{{
    mIsSelfTest = is_selftest;
    mSelfTest->SetSelfTestMode(is_selftest);
    if (is_selftest) {
        mSlave->Lin3EnterSelfTest();
        mMaster->Lin3EnterSelfTest();
    }
}//}}}

void Crlin3::Initialize(void)
{//{{{ 
    mTransmitDataAmount = 0;
    mReceiveDataAmount  = 0;
    mIsSelfTest         = false; 
    mMode               = Crlin3_common::emMasterMode;
    mLin3IntT           = false;                      
    mLin3IntR           = false;                      
    mLin3IntS           = false;                      
}//}}}

void Crlin3::UpdateRegReset(Crlin3_common::RlinRegs rlin_reg)
{//{{{
    (*LWBR )  = rlin_reg.lwbr  ;
    (*LBFC )  = rlin_reg.lbfc  ;
    (*LSC  )  = rlin_reg.lsc   ;
    (*LWUP )  = rlin_reg.lwup  ;
    (*LIE  )  = rlin_reg.lie   ;
    (*LEDE )  = rlin_reg.lede  ;
    (*LDFC )  = rlin_reg.ldfc  ;
    (*LUOR1)  = rlin_reg.luor1 ;
    (*LBSS )  = rlin_reg.lbss  ;
    (*LRSS )  = rlin_reg.lrss  ;
}//}}}

void Crlin3::UpdateRlin3Reg(Crlin3_common::RlinRegs rlin_reg) //update value of registers to other modes
{//{{{
    (*LSTC)    =  rlin_reg.lstc  ;
    (*LBRP0)   =  rlin_reg.lbrp0 ;
    (*LBRP1)   =  rlin_reg.lbrp1 ;
    (*LMD)     =  rlin_reg.lmd   ;
    (*LCUC)    =  rlin_reg.lcuc  ;
    (*LTRC)    =  rlin_reg.ltrc  ;
    (*LMST)    =  rlin_reg.lmst  ;
    (*LST)     =  rlin_reg.lst   ;
    (*LEST)    =  rlin_reg.lest  ;
    (*LIDB)    =  rlin_reg.lidb  ;
    (*LCBR)    =  rlin_reg.lcbr  ;
    (*LUDB0)   =  rlin_reg.ludb0 ;
    (*LDB1)    =  rlin_reg.ldb1  ;
    (*LDB2)    =  rlin_reg.ldb2  ;
    (*LDB3)    =  rlin_reg.ldb3  ;
    (*LDB4)    =  rlin_reg.ldb4  ;
    (*LDB5)    =  rlin_reg.ldb5  ;
    (*LDB6)    =  rlin_reg.ldb6  ;
    (*LDB7)    =  rlin_reg.ldb7  ;
    (*LDB8)    =  rlin_reg.ldb8  ;
    (*LUOER)   =  rlin_reg.luoer ;
    (*LUTDR)   =  rlin_reg.lutdr ;
    (*LURDR)   =  rlin_reg.lurdr ;
    (*LUWTDR)  =  rlin_reg.luwtdr;
    (*LURDE)   =  rlin_reg.lurde ;
    (*LBSS)    =  rlin_reg.lbss  ;
    (*LRSS)    =  rlin_reg.lrss  ;
    if (mStatus != Crlin3_common::emRESET) {
        (*LWBR)    =  rlin_reg.lwbr  ;
        (*LBFC)    =  rlin_reg.lbfc  ;
        (*LSC)     =  rlin_reg.lsc   ;
        (*LWUP)    =  rlin_reg.lwup  ;
        (*LIE)     =  rlin_reg.lie   ;
        (*LEDE)    =  rlin_reg.lede  ;
        (*LDFC)    =  rlin_reg.ldfc  ;
        (*LUOR1)   =  rlin_reg.luor1 ;
    }
    if (mIsSelfTest == true) {
        if (mMode == Crlin3_common::emMasterMode) { //self test master
            mSlave->UpdateSlaveRegs(rlin_reg);
        } else { //self test slave
            mMaster->UpdateMasterRegs(rlin_reg); 
        }
    } 
}//}}}

void Crlin3::UpdateAllRegs() //this function is called to update RLIN3 value of register to Master, Slave and Uart class when SW reset is called
{//{{{
    Crlin3_common::RlinRegs rlin_reg;
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
    rlin_reg.luwtdr =  (unsigned int)(*LUWTDR);
    rlin_reg.lbss =  (unsigned int)(*LBSS);
    rlin_reg.lrss =  (unsigned int)(*LRSS);
    mMaster->UpdateMasterRegs(rlin_reg);
    mSlave->UpdateSlaveRegs(rlin_reg);
    mUart->UpdateUartRegs(rlin_reg);
}//}}}

void Crlin3::UpdateAllRegsReset()
{//{{{
    Crlin3_common::RlinRegs rlin_reg;
    rlin_reg.lwbr  =  (unsigned int)(*LWBR) ;
    rlin_reg.lbfc  =  (unsigned int)(*LBFC) ;
    rlin_reg.lsc   =  (unsigned int)(*LSC)  ;
    rlin_reg.lwup  =  (unsigned int)(*LWUP) ;
    rlin_reg.lie   =  (unsigned int)(*LIE)  ;
    rlin_reg.lede  =  (unsigned int)(*LEDE) ;
    rlin_reg.ldfc  =  (unsigned int)(*LDFC) ;
    rlin_reg.luor1 =  (unsigned int)(*LUOR1);
    rlin_reg.lbss  =  (unsigned int)(*LBSS) ;
}//}}}

void Crlin3::WriteLin3IntT(bool value)
{//{{{
    mLin3IntT = value;
    mWriteLin3IntEvent.notify();    
}//}}}

void Crlin3::WriteLin3IntR(bool value)
{//{{{
    mLin3IntR = value;
    mWriteLin3IntEvent.notify();    
}//}}}

void Crlin3::WriteLin3IntS(bool value)
{//{{{
    mLin3IntS = value;
    mWriteLin3IntEvent.notify();    
}//}}}

void Crlin3::WriteLin3IntMethod(void) //write output interrupt function
{//{{{
    lin3_int_t.write(mLin3IntT);
    lin3_int_r.write(mLin3IntR);
    lin3_int_s.write(mLin3IntS);
    if (mLin3IntT == true) {
        DumpInterruptMsg (Crlin3_common::emTransCompleteIntr, true);
        mDeAssertIntrEvent.notify((double)(Crlin3_common::emNanoSecond/mPCLK_freq),SC_NS);//Add by UyenLe : No.1 Interrupt de-asserted after 1 PCLK clock (RLIN3v4) 
    }
    if (mLin3IntR == true) {
        DumpInterruptMsg (Crlin3_common::emReceiveCompleteIntr, true);
        mDeAssertIntrEvent.notify((double)(Crlin3_common::emNanoSecond/mPCLK_freq),SC_NS);//Add by UyenLe : No.1 Interrupt de-asserted after 1 PCLK clock (RLIN3v4) 
    }
    if (mLin3IntS == true) {
        DumpInterruptMsg (Crlin3_common::emStatusIntr, true);
        mDeAssertIntrEvent.notify((double)(Crlin3_common::emNanoSecond/mPCLK_freq),SC_NS);//Add by UyenLe : No.1 Interrupt de-asserted after 1 PCLK clock (RLIN3v4) 
    }
}//}}} 

void Crlin3::DeAssertIntrMethod(void) // this method is used for de-asserts interrupts
{//{{{
    if (lin3_int_t.read()) { //lin3_int_t is asserting
        mLin3IntT = 0;
        DumpInterruptMsg (Crlin3_common::emTransCompleteIntr, false);
    }
    if (lin3_int_r.read()) { //lin3_int_r is asserting
        mLin3IntR = 0;
        DumpInterruptMsg (Crlin3_common::emReceiveCompleteIntr, false);
    }
    if (lin3_int_s.read()) { //lin3_int_s is asserting
        mLin3IntS = 0;
        DumpInterruptMsg (Crlin3_common::emStatusIntr, false);
    }
    mWriteLin3IntEvent.notify(); 
}//}}}

void Crlin3::tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t)
{//{{{
    /* get transaction attributes */
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = this->tgt_get_gp_attribute(command, address, data_ptr, data_length, trans, false);
    if (! status) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }

    sc_assert(data_ptr != NULL);

    /* callback method */
    if (command == tlm::TLM_READ_COMMAND) {
        memset(data_ptr, 0, data_length);
        if (mMode == Crlin3_common::emMasterMode) {//master mode
            mMaster->master_reg_rd((unsigned int) address, data_ptr, data_length);
        } else if (mMode == Crlin3_common::emUartMode) {//uart mode
            mUart->uart_reg_rd((unsigned int) address, data_ptr, data_length);
        } else {//slave mode
            mSlave->slave_reg_rd((unsigned int) address, data_ptr, data_length);
        }
        status = reg_rd((unsigned int) address, data_ptr, data_length);
    }
    else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr((unsigned int) address, data_ptr, data_length);
        if ((address&0xFF) == 0x8) {
            mMaster->master_reg_wr((unsigned int) address, data_ptr, data_length);
            if (mMode != Crlin3_common::emMasterMode) {
                mUart->uart_reg_wr((unsigned int) address, data_ptr, data_length);
                if (mMode != Crlin3_common::emUartMode) {
                    mSlave->slave_reg_wr((unsigned int) address, data_ptr, data_length);
                }
            }
            mSelfTest->SetOptMode((unsigned int)mMode);
            UpdateAllRegs();
        } else if ((((address & 0xFF) == 0x01) || ((address & 0xFF) == 0x09) ||
                    ((address & 0xFF) == 0x0A) || ((address & 0xFF) == 0x0B) ||
                    ((address & 0xFF) == 0x0C) || ((address & 0xFF) == 0x0D) ||
                    ((address & 0xFF) == 0x14) || ((address & 0xFF) == 0x21) ) && (mStatus == Crlin3_common::emRESET))
        {
            UpdateAllRegsReset();
        } else {
            if (mMode == Crlin3_common::emMasterMode) {//master mode
                mMaster->master_reg_wr((unsigned int) address, data_ptr, data_length);
            } else if (mMode == Crlin3_common::emUartMode) {//uart mode
                mUart->uart_reg_wr((unsigned int) address, data_ptr, data_length);
            } else {//slave mode
                mSlave->slave_reg_wr((unsigned int) address, data_ptr, data_length);
            }
            UpdateAllRegs();
        }
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
      status = true;
    }
    
    /* set response status attribute */
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}//}}}

unsigned int Crlin3::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
 {//{{{
    /* get transaction attributes */
    tlm::tlm_command command;
    sc_dt::uint64 address = 0;
    unsigned char *data_ptr = NULL;
    unsigned int data_length = 0;
    bool status = this->tgt_get_gp_attribute(command, address, data_ptr,data_length, trans, true);

    sc_assert(data_ptr != NULL);
    
    /* callback method */
    if (command == tlm::TLM_READ_COMMAND) {
        memset(data_ptr, 0, data_length);
        if (mMode == Crlin3_common::emMasterMode) {//master mode
            mMaster->master_reg_rd_dbg((unsigned int) address, data_ptr, data_length);
        } else if (mMode == Crlin3_common::emUartMode) {//uart mode
            mUart->uart_reg_rd_dbg((unsigned int) address, data_ptr, data_length);
        } else {//slave mode
            mSlave->slave_reg_rd_dbg((unsigned int) address, data_ptr, data_length);
        }
        status = reg_rd_dbg((unsigned int) address, data_ptr, data_length);
    }
    else if (command == tlm::TLM_WRITE_COMMAND) {
        status = reg_wr_dbg((unsigned int) address, data_ptr, data_length);
        if ((address&0xFF) == 0x8) {
            mMaster->master_reg_wr_dbg((unsigned int) address, data_ptr, data_length);
            if (mMode != Crlin3_common::emMasterMode) {
                mUart->uart_reg_wr_dbg((unsigned int) address, data_ptr, data_length);
                if (mMode != Crlin3_common::emUartMode) {
                    mSlave->slave_reg_wr_dbg((unsigned int) address, data_ptr, data_length);
                }
            }
            mSelfTest->SetOptMode((unsigned int)mMode);            
            UpdateAllRegs();
        } else if ((((address & 0xFF) == 0x01) || ((address & 0xFF) == 0x09) ||
                    ((address & 0xFF) == 0x0A) || ((address & 0xFF) == 0x0B) ||
                    ((address & 0xFF) == 0x0C) || ((address & 0xFF) == 0x0D) ||
                    ((address & 0xFF) == 0x14) || ((address & 0xFF) == 0x21) ) && (mStatus == Crlin3_common::emRESET))
        {
            UpdateAllRegsReset();
        } else {
            if (mMode == Crlin3_common::emMasterMode) {//master mode
                mMaster->master_reg_wr_dbg((unsigned int) address, data_ptr, data_length);
            } else if (mMode == Crlin3_common::emUartMode) {//uart mode
                mUart->uart_reg_wr_dbg((unsigned int) address, data_ptr, data_length);
            } else {//slave mode
                mSlave->slave_reg_wr_dbg((unsigned int) address, data_ptr, data_length);
            }
            UpdateAllRegs();
        }
    }
    /* be necessarily TLM_IGNORE_COMMAND */
    else {
        status = true;
        data_length = 0;
    }

    /* set response status attribute and return value */
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status) {
        return data_length;
    }
    else {
        return 0;
    }

}//}}} 

std::string Crlin3::RegIfCommand(const std::vector<std::string>& args)
{//{{{
    std::string ret = "";
    ret = mMaster->master_reg_command(args);
    ret = mUart->uart_reg_command(args);
    ret = mSlave->slave_reg_command(args);
    return ret;
}//}}}

void Crlin3::DumpInfo(const char *type, const char *message, ...)
{//{{{
    const char *model_name;
    sc_assert((message != NULL) && (type != NULL));

    if (mMode == Crlin3_common::emMasterMode) {
        model_name = "RLIN3 MASTER";
    } else if (mMode == Crlin3_common::emUartMode) {
        model_name = "RLIN3 UART";
    } else {
        model_name = "RLIN3 SLAVE";
    }
    sc_assert(model_name != NULL);
    printf ("PROFILE(%s): %s: ", type,model_name);
    // Print body
    va_list argptr;
    va_start(argptr, message);
    sc_assert(argptr != NULL);
    vprintf(message, argptr);
    va_end(argptr);
}//}}}

void Crlin3::DumpOperationInfo(const char *frame_name, const char *operation, unsigned int id_val, unsigned int data_val, const char *no_cksum, unsigned int cksum_val)
{//{{{
    sc_assert((operation != NULL)&&(frame_name != NULL)&&(no_cksum != NULL));

    if (EnableTransInfo) {
        const char *type = "TransInfo";
        // Dump information when RLIN3 transmitting/receiving data packet
        DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
        if (strcmp(operation,"Transmitting") == 0) {
            DumpInfo(type, "RLIN3 model transmit a frame %s\n",frame_name);
        } else {
            DumpInfo(type, "RLIN3 model receive a frame %s\n",frame_name);
        }
        if (strcmp(frame_name,"Response") == 0) {
            if (strcmp(no_cksum,"NoCheckSum") == 0) {
                DumpInfo(type, " Operation  : %s data\n", operation);
            } else {
                DumpInfo(type, " Operation  : %s checksum\n", operation);
            }
        } else {
            DumpInfo(type, " Operation  : %s header\n", operation);
        }
        if (mMode != Crlin3_common::emUartMode) {
            DumpInfo(type, " ID value   : 0x%02X\n", id_val);
        }
        if (strcmp(frame_name,"Response") == 0) {
            if (strcmp(no_cksum,"NoCheckSum") == 0) {
                DumpInfo(type, " Data transfer value   : 0x%02X\n", data_val);
            } else {
                DumpInfo(type, " Checksum value   : 0x%02X\n", cksum_val);
            }
        }
        DumpInfo(type, "EndInfo.\n");
    }
}//}}}

void Crlin3::DumpInterruptMsg (Crlin3_common::eINTERRUPT_KIND interrupt_id, bool int_assert)
{//{{{

    if(DumpInterrupt){
        if (int_assert == true) { //Interrupt is asserted.
            if (interrupt_id == Crlin3_common::emTransCompleteIntr) {
                re_printf("info"," INT [RLIN3: lin3_int_t] Assert\n");
            } else if (interrupt_id == Crlin3_common::emReceiveCompleteIntr) {
                re_printf("info"," INT [RLIN3: lin3_int_r] Assert\n");
            } else {
                re_printf("info"," INT [RLIN3: lin3_int_s] Assert\n");
            }
        } else {
            //Interrupt is deasserted
            if (interrupt_id == Crlin3_common::emTransCompleteIntr) {
                re_printf("info"," INT [RLIN3: lin3_int_t] DeAssert\n");
            } else if (interrupt_id == Crlin3_common::emReceiveCompleteIntr) {
                re_printf("info"," INT [RLIN3: lin3_int_r] DeAssert\n");
            } else {
                re_printf("info"," INT [RLIN3: lin3_int_s] DeAssert\n");
            }
        }
    }
}//}}} 

void Crlin3::DumpStatInfo(void)
{//{{{
    const char *type = "StatInfo";
    DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
    std::string current_state = "";
    if (mMode == Crlin3_common::emMasterMode) { //master mode
        current_state = mMaster->GetCurrentStatus(); 
    } else if (mMode == Crlin3_common::emUartMode) { //uart mode
        current_state = mUart->GetCurrentStatus();
    } else {
        current_state = mSlave->GetCurrentStatus();
    }
    
    // Dump statistical information about transmitting/receiving activity
    DumpInfo(type, "RLIN3 transfer information\n");
    DumpInfo(type, " Current State : %s\n", current_state.c_str());
    DumpInfo(type, " Total data transmitted: %d byte(s)\n", mTransmitDataAmount);
    DumpInfo(type, " Total data received   : %d byte(s)\n", mReceiveDataAmount);
    DumpInfo(type, "EndInfo.\n");
    // Reset data amount counter
    mTransmitDataAmount = 0; 
    mReceiveDataAmount = 0;
}//}}}

void Crlin3::ResetMethod(void)
{//{{{
    bool current_rst = mRlin3Reset;
    if ((rstc_n.read() == vpcl::emResetActive)&&(preset_n.read() == vpcl::emResetActive)) {
        mRlin3Reset = true;
    } else {
        mRlin3Reset = false;
    }
    if (mRlin3Reset != current_rst) {
        if (mRlin3Reset){
            re_printf("info","Reset signal is asserted.\n");
            // Cancel handleCommand reset when rstc_n and preset_n are asserted
            if (mIsCmdResetStatus) {
                mCmdResetEvent.cancel();
                mCancelCmdResetEvent.cancel();
                mIsCmdResetStatus = false;
                mCmdReset = false;
            }
            this->EnableReset(mRlin3Reset);
        } else {
            re_printf("info","Reset signal is negated.\n");
            this->EnableReset(mRlin3Reset);
        }
    }
}//}}}

void Crlin3::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Initialize data members
        this->Initialize();
       
        //Update initial value to master, slave, uart
        UpdateAllRegs();

        // Initialize output ports
        mDeAssertIntrEvent.notify();
    }
    // Call the reset function of common/general class
    mMaster->ResetMaster(is_active);
    mSlave->ResetSlave(is_active);
    mUart->ResetUart(is_active);
        
    Crlin3_regif::EnableReset(is_active);
    mSelfTest->SetAssertReset(is_active);
    if(mCmdReset == false) {
        this->tgt_enable_reset(is_active);
    } 
    mStatus = Crlin3_common::emRESET;
    
}//}}}

void Crlin3::AssertReset(const double delay, const double period) 
{//{{{
    // If (preset_n&rstc_n) is negated in handle command, RLIN3 waits delay time then notifies handleCommand reset event
    if ((!mRlin3Reset)&&(!mIsCmdResetStatus)) {
        mIsCmdResetStatus = true;
        re_printf("info","RLIN3 will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) delay, FRACTION_TO_INT(delay));
        mCmdResetEvent.notify(delay, SC_NS);
        mResetPeriod = period;
    } else {
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Crlin3::CmdResetMethod(void)
{//{{{ 
    mCmdReset = true;
    // If handleCommand reset is negated, RLIN3 notifies cancel handleCommand reset event after reset period
    this->EnableReset(mCmdReset);
    mCancelCmdResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Crlin3::CancelCmdResetMethod(void)
{//{{{
    mCmdReset = false;
    mIsCmdResetStatus = false;
    this->EnableReset(mCmdReset);
}//}}}

void Crlin3::CLKCPeriodMethod(void)
{//{{{
    if (mRlin3Reset || mCmdReset){
        return;
    }
    SetCLKfreq("clkc",(double)(clkc.read()));
}//}}}
 
void Crlin3::PCLKPeriodMethod(void)
{//{{{ 
    if (mRlin3Reset || mCmdReset){
        return;
    }
    SetCLKfreq("pclk",(double)(pclk.read()));
}//}}}

unsigned int Crlin3::GetRegBitsVal(unsigned int reg, unsigned int lower_index,unsigned int upper_index)
{//{{{ 
    unsigned int bits_val;
    bits_val = 0;
    for (unsigned int i=lower_index;i<=upper_index;i++) {
        //Set 1 to the bits_val variable according to bit orders
        bits_val = (unsigned int)(bits_val|(0x1 << i));
    }
    bits_val = bits_val&reg; //Get the value of segement of the register.
    bits_val = bits_val >> lower_index;
    return bits_val;
}//}}}

bool Crlin3::SelftestAccessCheck (vpcl::re_register *reg, bool is_wr, uint pre_data, uint status, bool is_selftest) // Add by SonTran: No.12(2) Only LCUC.OM0 can be accessed when communication bit is set in Selftest mode.
{//{{{
    sc_assert(reg != NULL);
    if (is_wr && is_selftest && (status == Crlin3_common::emOPERATION)) {
        (*reg) = pre_data;
        re_printf("warning","Register %s is not allowed to access when communication bit (LTRC.FTS) is set in Selftest mode.\n",reg->name().c_str());
        return false;
    }
    return true;
}//}}}

void Crlin3::RegisterAccessCheck (vpcl::re_register *reg, bool is_wr, uint pre_data, std::string bit_name, Crlin3_common::eACCESS_MODE expected_access, unsigned int lower_index,unsigned int upper_index)
{//{{{
    sc_assert(reg != NULL);
    if (is_wr == true) { //Register is writing
        if ((GetRegBitsVal((unsigned int)(*reg),lower_index,upper_index)) != (GetRegBitsVal((unsigned int)pre_data,lower_index,upper_index))) {
            if (expected_access == Crlin3_common::emR) {
                re_printf("warning","The bit %s.%s is read only.\n",reg->name().c_str(),bit_name.c_str());
                (*reg) = SetRegBitsVal((unsigned int)(*reg),(unsigned int)pre_data,lower_index,upper_index);
            }
            if (expected_access == Crlin3_common::emR0) {
                re_printf("warning","The bit %s.%s is read 0 only.\n",reg->name().c_str(),bit_name.c_str());
                (*reg) = SetRegBitsVal((unsigned int)(*reg),(unsigned int)pre_data,lower_index,upper_index);
            }
            if((expected_access == Crlin3_common::emRW0)&&(GetRegBitsVal((unsigned int)(*reg),lower_index,upper_index) == 1)) {//check write 0 to clear access 
                re_printf("warning","The bit %s.%s is written 0 only.\n",reg->name().c_str(),bit_name.c_str());
                (*reg) = SetRegBitsVal((unsigned int)(*reg),(unsigned int)pre_data,lower_index,upper_index);
            }
        }
    } else { //Register is reading
        //Check expected access mode, if expected access mode are emW emR0 then prohibit read operation
        if (expected_access == Crlin3_common::emR0) {
            //Prohibit bit in the register is reading
            (*reg) = SetRegBitsVal((unsigned int)(*reg),0,lower_index,upper_index);
            re_printf("warning","The bit %s.%s is read 0 only.\n",reg->name().c_str(),bit_name.c_str());
        }
    }
}//}}}

unsigned int Crlin3::SetRegBitsVal(unsigned int reg, unsigned int pre_reg, unsigned int lower_index, unsigned int upper_index)
{//{{{
    unsigned int mask    =   0;
    // Create mask  
    for (unsigned int i = lower_index ; i<=upper_index; i++) {
        mask    =  (unsigned int)(mask | (0x1 << i )); 
    }  
    return (reg & (~mask)) | (pre_reg & mask); // Keep mask value;
}//}}} 

double Crlin3::GetTimeResolution(void) // get resolution time
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)){
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

void Crlin3::SetLatency_TLM(void)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param;
    vpcl::tlm_tgt_if<32>::tgt_get_param(&tgt_param);
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}} 

void Crlin3::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{ 
    if ((clk_name != "pclk")&&(clk_name != "clkc")) {
        re_printf("error","Clock name is invalid.\n");
        return;
    }

    double clk_period = 0 ;

    if(clk_freq == 0){
        //cancel all processes
        mDeAssertIntrEvent.cancel();
        mWriteLin3IntEvent.cancel();
    } else {
        clk_period = (double)(Crlin3_common::emNanoSecond/clk_freq); 
        double time_unit = (double)(Crlin3_common::emNanoSecond/GetTimeResolution());
        if(time_unit > clk_period) {
            re_printf("warning","The %s period is less than 1 unit time of system.\n", clk_name.c_str());
            return;
        }
    }    

    if (clk_name == "pclk") {
        mPCLK_freq = clk_freq;
        vpcl::tlm_if_tgt_parameter tgt_param;
        tgt_get_param(&tgt_param);
        sc_time clock_value(clk_period, SC_NS);
        tgt_param.bus_clk = clock_value;
        tgt_set_param(&tgt_param);
        SetLatency_TLM();
    } else {
        mCLKC_freq = clk_freq;
    }

    mMaster->SetMasterClock(mCLKC_freq,mPCLK_freq);
    mSlave->SetSlaveClock(mCLKC_freq,mPCLK_freq);
    mUart->SetUartClock(mCLKC_freq,mPCLK_freq);

    re_printf("info","The %s is set with a frequency as %lld.%06d.\n", clk_name.c_str(), (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
}//}}}

void Crlin3::SoftwareReset(unsigned int om0_val, unsigned int mode_change)
{//{{{
    Crlin3_common::RlinRegs rlin_reg;
    rlin_reg.lwbr   = (unsigned int) (*LWBR)  ;
    rlin_reg.lbrp0  = (unsigned int) (*LBRP0) ;
    rlin_reg.lbrp1  = (unsigned int) (*LBRP1) ;
    rlin_reg.lstc   = (unsigned int) (*LSTC)  ;
    rlin_reg.lmd    = (unsigned int) (*LMD)   ;
    rlin_reg.lbfc   = (unsigned int) (*LBFC)  ;
    rlin_reg.lsc    = (unsigned int) (*LSC)   ;
    rlin_reg.lwup   = (unsigned int) (*LWUP)  ;
    rlin_reg.lie    = (unsigned int) (*LIE)   ;
    rlin_reg.lede   = (unsigned int) (*LEDE)  ;
    rlin_reg.lcuc   = (unsigned int) (*LCUC)  ;
    rlin_reg.ltrc   = (unsigned int) (*LTRC)  ;
    rlin_reg.lmst   = (unsigned int) (*LMST)  ;
    rlin_reg.ldfc   = (unsigned int) (*LDFC)  ;
    rlin_reg.lidb   = (unsigned int) (*LIDB)  ;
    rlin_reg.lcbr   = (unsigned int) (*LCBR)  ;
    rlin_reg.ludb0  = (unsigned int) (*LUDB0) ;
    rlin_reg.ldb1   = (unsigned int) (*LDB1)  ;
    rlin_reg.ldb2   = (unsigned int) (*LDB2)  ;
    rlin_reg.ldb3   = (unsigned int) (*LDB3)  ;
    rlin_reg.ldb4   = (unsigned int) (*LDB4)  ;
    rlin_reg.ldb5   = (unsigned int) (*LDB5)  ;
    rlin_reg.ldb6   = (unsigned int) (*LDB6)  ;
    rlin_reg.ldb7   = (unsigned int) (*LDB7)  ;
    rlin_reg.ldb8   = (unsigned int) (*LDB8)  ;
    rlin_reg.luoer  = (unsigned int) (*LUOER) ;
    rlin_reg.luor1  = (unsigned int) (*LUOR1) ;
    rlin_reg.lutdr  = (unsigned int) (*LUTDR) ;
    rlin_reg.lurdr  = (unsigned int) (*LURDR) ;
    rlin_reg.luwtdr = (unsigned int) (*LUWTDR);
    rlin_reg.lbss   = (unsigned int) (*LBSS)  ;
    rlin_reg.lrss   = (unsigned int) (*LRSS)  ;

    if (om0_val == 0) {
        mStatus = Crlin3_common::emRESET;
        re_printf("info","Software reset is asserted\n");
    } else {
        mStatus = Crlin3_common::emIDLE;
        re_printf("info","Software reset is de-asserted\n");
    }
    if (mode_change == emRLIN3_MASTER) {
        mMaster->SoftwareReset(om0_val, rlin_reg, true);
        mUart->SoftwareReset(om0_val, rlin_reg, false);
        mSlave->SoftwareReset(om0_val, rlin_reg, false);
    } else if (mode_change == emRLIN3_UART) {
        mMaster->SoftwareReset(om0_val, rlin_reg, false);
        mUart->SoftwareReset(om0_val, rlin_reg, true);
        mSlave->SoftwareReset(om0_val, rlin_reg, false);
    } else {
        mMaster->SoftwareReset(om0_val, rlin_reg, false);
        mUart->SoftwareReset(om0_val, rlin_reg, false);
        mSlave->SoftwareReset(om0_val, rlin_reg, true);
    }
    if (om0_val == 0) {
        //intialize output port
        WriteLin3IntT(false);
        WriteLin3IntR(false);
        WriteLin3IntS(false);
    }
}//}}}
// vim600: set foldmethod=marker :
