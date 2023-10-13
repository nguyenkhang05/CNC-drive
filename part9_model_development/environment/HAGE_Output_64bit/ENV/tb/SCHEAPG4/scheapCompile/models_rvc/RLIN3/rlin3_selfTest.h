// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2012-2014 Renesas Electronics Corporation
// Copyright(c) 2012-2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __RLIN3_SELFTEST_H__
#define __RLIN3_SELFTEST_H__
#include "systemc.h"
#include "iterator"
#include <sstream>

class Crlin3_selftest: public sc_module 
{
public:
    //declare input ports
    sc_in<sc_uint<32> > rx_data_m;
    sc_in<sc_uint<32> > rx_control_m;
    sc_in<sc_uint<32> > rx_data_s;
    sc_in<sc_uint<32> > rx_control_s;
    sc_in<sc_uint<32> > rx_data_u;
    sc_in<sc_uint<32> > rx_control_u;
    sc_in<sc_uint<32> > rx_data;
    sc_in<sc_uint<32> > rx_control;

    //declare output ports
    sc_out<sc_uint<32> > tx_data_m;
    sc_out<sc_uint<32> > tx_control_m;
    sc_out<sc_uint<32> > tx_data_s;
    sc_out<sc_uint<32> > tx_control_s;
    sc_out<sc_uint<32> > tx_data_u;
    sc_out<sc_uint<32> > tx_control_u;
    sc_out<sc_uint<32> > tx_data;
    sc_out<sc_uint<32> > tx_control;

    //Construct and Destruct
    SC_HAS_PROCESS(Crlin3_selftest);
    Crlin3_selftest(sc_module_name name);
    ~Crlin3_selftest();

    void SetSelfTestMode(bool is_selfTest); //This API use to set normal mode or self test mode
    void SetOptMode(unsigned int opt_mode); //This API use to set master, slave or uart mode
    void SetAssertReset(bool is_reset);     //This API use to set self test class when hw reset assert and when hw de-assert

private:
    bool mIsSelfTest ;                      //Indicate that rlin3 in self test mode or not 
    unsigned int mIsOptMode ;               //Indicate rlin3 in master, slave or uart mode
    bool mIsReset;                          //Indicate that rlin3 in hw reset or not
    sc_event mResetPortEvent;
    bool mIsNotifyRST;
    unsigned int mRx_data_m;
    unsigned int mRx_control_m;
    unsigned int mRx_data_s;  
    unsigned int mRx_control_s;

    //Method
    void SelfTestHandlingMethod();          //This mothod use to connect port base on setting of self test mode and operation mode
};

#endif //__RLIN3_SELFTEST_H__
