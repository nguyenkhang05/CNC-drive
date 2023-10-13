// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2012-2015 Renesas Electronics Corporation
// Copyright(c) 2012-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "rlin3_selfTest.h"
//===============Crlin3 class=======================
Crlin3_selftest::Crlin3_selftest(sc_module_name name): 
     sc_module(name)
     ,rx_data_m("rx_data_m")
     ,rx_control_m("rx_control_m")
     ,rx_data_s("rx_data_s")
     ,rx_control_s("rx_control_s")
     ,rx_data_u("rx_data_u")
     ,rx_control_u("rx_control_u")
     ,rx_data("rx_data")
     ,rx_control("rx_control")
     ,tx_data_m("tx_data_m")
     ,tx_control_m("tx_control_m")
     ,tx_data_s("tx_data_s")
     ,tx_control_s("tx_control_s")
     ,tx_data_u("tx_data_u")
     ,tx_control_u("tx_control_u")
     ,tx_data("tx_data")
     ,tx_control("tx_control")
{//{{{
    //intialize output port
    tx_control_m.initialize(0x00000108);
    tx_data_m.initialize(0xFFFFFFFF);
    tx_control_s.initialize(0x00000108);
    tx_data_s.initialize(0xFFFFFFFF);
    tx_control_u.initialize(0x00000108);
    tx_data_u.initialize(0xFFFFFFFF);
    tx_control.initialize(0x00000108);
    tx_data.initialize(0xFFFFFFFF);

    //method
    SC_METHOD(SelfTestHandlingMethod)
    dont_initialize();
    sensitive << rx_control_m;
    sensitive << rx_control_s;
    sensitive << rx_control_u;
    sensitive << rx_data_m;
    sensitive << rx_data_s;
    sensitive << rx_data_u;
    sensitive << rx_control;
    sensitive << rx_data;
    sensitive << mResetPortEvent;

    //Initial variable
    mIsSelfTest = false;
    mIsOptMode = 0x0;
    mIsReset = false;
    mIsNotifyRST = false;
    mRx_data_m = 0xFFFFFFFF;
    mRx_control_m = 0x00000108;
    mRx_data_s = 0xFFFFFFFF;  
    mRx_control_s = 0x00000108;
}//}}}

Crlin3_selftest::~Crlin3_selftest()
{//{{{
}//}}}

void Crlin3_selftest::SetAssertReset(bool is_reset)
{//{{{
    mIsReset = is_reset;
    if (is_reset) {
        mIsSelfTest = false;
        mIsOptMode = 0;
        mIsNotifyRST = true;
        mRx_data_m = 0xFFFFFFFF;
        mRx_control_m = 0x00000108;
        mRx_data_s = 0xFFFFFFFF;  
        mRx_control_s = 0x00000108;
        mResetPortEvent.notify();
    }
}//}}}

void Crlin3_selftest::SetSelfTestMode(bool is_selfTest)
{//{{{
    mIsSelfTest = is_selfTest;
    mRx_data_m = 0xFFFFFFFF;
    mRx_control_m = 0x00000108;
    mRx_data_s = 0xFFFFFFFF;  
    mRx_control_s = 0x00000108;
}//}}}

void Crlin3_selftest::SetOptMode(unsigned int opt_mode)
{//{{{
    mIsOptMode = opt_mode;
}//}}}

void Crlin3_selftest::SelfTestHandlingMethod()
{//{{{
    unsigned int tx_data_m_val = 0xFFFFFFFF;
    unsigned int tx_control_m_val = 0x00000108;
    unsigned int tx_data_s_val = 0xFFFFFFFF;  
    unsigned int tx_control_s_val = 0x00000108;
    unsigned int tx_data_u_val = 0xFFFFFFFF;  
    unsigned int tx_control_u_val = 0x00000108;
    unsigned int tx_data_val = 0xFFFFFFFF;
    unsigned int tx_control_val = 0x00000108;
    unsigned int rx_data_m_val = (unsigned int)rx_data_m.read();
    unsigned int rx_control_m_val = (unsigned int)rx_control_m.read();
    unsigned int rx_data_s_val = (unsigned int)rx_data_s.read();  
    unsigned int rx_control_s_val = (unsigned int)rx_control_s.read();
    unsigned int rx_data_u_val = (unsigned int)rx_data_u.read();  
    unsigned int rx_control_u_val = (unsigned int)rx_control_u.read();
    unsigned int rx_data_val = (unsigned int)rx_data.read();
    unsigned int rx_control_val = (unsigned int)rx_control.read();
    if (mIsReset){
        if (mIsNotifyRST) {
            mIsNotifyRST = false;
        } else {
            return;
        } 
    } else {
        if (mIsSelfTest == true) {
            if ((rx_data_m_val != mRx_data_m) || (rx_control_m_val != mRx_control_m)) {
                tx_data_s_val = rx_data_m_val;
                tx_data_m_val = rx_data_m_val;
                tx_control_s_val = rx_control_m_val;
                tx_control_m_val = rx_control_m_val;
            } else if ((rx_data_s_val != mRx_data_s) || (rx_control_s_val != mRx_control_s)) {
                tx_data_s_val = rx_data_s_val;
                tx_data_m_val = rx_data_s_val;
                tx_control_s_val = rx_control_s_val;
                tx_control_m_val = rx_control_s_val;
            }
            mRx_data_m = rx_data_m_val;
            mRx_control_m = rx_control_m_val;
            mRx_data_s = rx_data_s_val;  
            mRx_control_s = rx_control_s_val;
        } else {
            if (mIsOptMode == 0x0) { //Master Mode
                //Connect ports in the Normal mode
                tx_data_m_val = rx_data_val;
                tx_control_m_val = rx_control_val;
                tx_data_val = rx_data_m_val;
                tx_control_val = rx_control_m_val;
            } else if ((mIsOptMode == 0x2)||(mIsOptMode == 0x3)) { //Slave mode
                //Connect ports in the Normal mode
                tx_data_s_val = rx_data_val;
                tx_control_s_val = rx_control_val;
                tx_data_val = rx_data_s_val;
                tx_control_val = rx_control_s_val;
            } else { //UART mode
                //Connect ports in the Normal mode
                tx_data_u_val = rx_data_val;
                tx_control_u_val = rx_control_val;
                tx_data_val = rx_data_u_val;
                tx_control_val = rx_control_u_val;
            }
        }
    }
    tx_data_m.write((sc_uint<32>)tx_data_m_val);
    tx_control_m.write((sc_uint<32>)tx_control_m_val);
    tx_data_s.write((sc_uint<32>)tx_data_s_val);
    tx_control_s.write((sc_uint<32>)tx_control_s_val);
    tx_data_u.write((sc_uint<32>)tx_data_u_val);
    tx_control_u.write((sc_uint<32>)tx_control_u_val);
    tx_data.write((sc_uint<32>)tx_data_val);
    tx_control.write((sc_uint<32>)tx_control_val);
}//}}}

// vim600: set foldmethod=marker : 










