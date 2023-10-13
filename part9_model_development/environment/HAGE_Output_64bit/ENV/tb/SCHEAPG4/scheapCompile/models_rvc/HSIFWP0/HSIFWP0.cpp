// ---------------------------------------------------------------------
// $Id: HSIFWP0.cpp,v 1.6 2020/03/16 02:13:07 quynhnhunguyen Exp $
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
#include <string>
#include "HSIFWP0.h"
#include "HSIFL10_ALL_AgentController.h"
#include "HSIFL10_ALL_Func.h"
#include "HSIF020_Func.h"
#include "HSIF020_AgentController.h"

//Construction of HSIFWP0 class
HSIFWP0::HSIFWP0(sc_module_name name,
                unsigned int rLatency,
                unsigned int wLatency,
        SIM_MODE_T simmode,
        std::string version_name):
sc_module(name),
HSIF_ultis(this->basename()),
APBCK_in("APBCK_in"),
AXICK_in("AXICK_in"),
clk20Mhz("clk20Mhz"),
hsif_refclk_in("hsif_refclk_in"),
hsif_refclk_out("hsif_refclk_out"),
x_spreset_ipc("x_spreset_ipc"),
hsif_rxd("hsif_rxd"),
hsif_rxd_control("hsif_rxd_control"),
hsif_txd("hsif_txd"),
hsif_txd_control("hsif_txd_control"),
hsif_authent1_pass("hsif_authent1_pass"),
hsif_authent2_pass("hsif_authent2_pass"),
int_hsif_str("int_hsif_str"),
int_hsif_err("int_hsif_err"),
int_hsif_sec("int_hsif_sec"),
int_hsif_txcmp("int_hsif_txcmp"),
int_hsif_txerr("int_hsif_txerr"),
int_hsif_rxcmp("int_hsif_rxcmp"),
int_hsif_rxerr("int_hsif_rxerr"),
int_hsif_rxiclc("int_hsif_rxiclc"),
int_hsif_err_internal("int_hsif_err_internal"),
int_hsif_sec_internal("int_hsif_sec_internal"),
hsif_tx_data("hsif_tx_datat"),
hsif_tx_control("hsif_tx_control"),
hsif_tx_data_ready("hsif_tx_data_ready"),
hsif_tx_frame_cmp("hsif_tx_frame_cmp"),
hsif_rx_data("hsif_rx_data"),
hsif_rx_control("hsif_rx_control")
{ 
    am_is = new TlmInitiatorSocket<64>((char*)"am_is");
    as_ts = new TlmTargetSocket<64>((char*)"as_ts");
    tsp   = new TlmTargetSocket<32>((char*)"tsp");

    std::string port_name;
    for (uint32_t index = 0; index < emCHNum; index++){
      port_name = CombineStringUint("int_hsif_ch", index);
      int_hsif_ch[index] = new sc_out<bool>(port_name.c_str());
      sc_assert(int_hsif_ch[index] != NULL);
      int_hsif_ch[index]->initialize(false);
    }
    hsif_refclk_out.initialize(0);
    hsif_txd_control.initialize(0);
    hsif_txd.initialize(L1Frame());
    hsif_authent1_pass.initialize(0);
    hsif_authent2_pass.initialize(0);
    int_hsif_str.initialize(0);
    int_hsif_err.initialize(0);
    int_hsif_sec.initialize(0);
    int_hsif_txcmp.initialize(0);
    int_hsif_txerr.initialize(0);
    int_hsif_rxcmp.initialize(0);
    int_hsif_rxerr.initialize(0);
    int_hsif_rxiclc.initialize(0);

    // implement HSIFL10 and HSIF020
    std::string strName = "hsifwp0_hsifl10_all_";
    strName.append((std::string)name);
    mHSIFL10_ALL = new HSIFL10_ALL(strName.c_str(), 0, 0, version_name.c_str());
    sc_assert(mHSIFL10_ALL != NULL);
    strName = "hsifwp0_hsif020_";
    strName.append((std::string)name);
    mHSIF020 = new HSIF020(strName.c_str(), 0, 0, simmode);
    sc_assert(mHSIF020 != NULL);

    //port connection for L1
    (mHSIFL10_ALL->PCLK)(APBCK_in);
    (mHSIFL10_ALL->CCLK)(clk20Mhz);
    (mHSIFL10_ALL->reset_rhsif_n)(x_spreset_ipc);
    (mHSIFL10_ALL->hsif_refclk_in)(hsif_refclk_in);
    (mHSIFL10_ALL->hsif_refclk_out)(hsif_refclk_out);
    (mHSIFL10_ALL->hsif_rxd)(hsif_rxd);
    (mHSIFL10_ALL->hsif_rxd_control)(hsif_rxd_control);
    (mHSIFL10_ALL->hsif_tx_data)(hsif_tx_data);
    (mHSIFL10_ALL->hsif_tx_control)(hsif_tx_control);
    (mHSIFL10_ALL->hsif_txd)(hsif_txd);
    (mHSIFL10_ALL->hsif_txd_control)(hsif_txd_control);
    (mHSIFL10_ALL->hsif_rx_data)(hsif_rx_data);
    (mHSIFL10_ALL->hsif_rx_control)(hsif_rx_control);
    (mHSIFL10_ALL->hsif_tx_data_ready)(hsif_tx_data_ready);
    (mHSIFL10_ALL->hsif_frame_cmp)(hsif_tx_frame_cmp);
    (mHSIFL10_ALL->int_hsif_txcmp)(int_hsif_txcmp);
    (mHSIFL10_ALL->int_hsif_txerr)(int_hsif_txerr);
    (mHSIFL10_ALL->int_hsif_rxcmp)(int_hsif_rxcmp);
    (mHSIFL10_ALL->int_hsif_rxerr)(int_hsif_rxerr);
    (mHSIFL10_ALL->int_hsif_rxiclc)(int_hsif_rxiclc);
    (*tsp)(*(mHSIFL10_ALL->tsp));

    // port connection for L2
    (mHSIF020->ACLK)(AXICK_in);
    (mHSIF020->CCLK)(APBCK_in);
    (mHSIF020->ARESETn)(x_spreset_ipc);
    (mHSIF020->creset_n)(x_spreset_ipc);
    (mHSIF020->hsif_tx_data)(hsif_tx_data);
    (mHSIF020->hsif_tx_control)(hsif_tx_control);
    (mHSIF020->hsif_tx_data_ready)(hsif_tx_data_ready);
    (mHSIF020->hsif_tx_frame_cmp)(hsif_tx_frame_cmp);
    (mHSIF020->hsif_rx_data)(hsif_rx_data);
    (mHSIF020->hsif_rx_control)(hsif_rx_control);
    (mHSIF020->hsif_authent1_pass)(hsif_authent1_pass);
    (mHSIF020->hsif_authent2_pass)(hsif_authent2_pass);
    for (uint8_t index = 0; index< emCHNum; index++){
        (mHSIF020->int_hsif_ch[index])->bind(*int_hsif_ch[index]);
    }
    (mHSIF020->int_hsif_str)(int_hsif_str);
    (mHSIF020->int_hsif_err)(int_hsif_err_internal);
    (mHSIF020->int_hsif_sec)(int_hsif_sec_internal);
    (*(mHSIF020->am_is))(*am_is);
    (*as_ts)(*(mHSIF020->as_ts));

    //Method for Wrapper
    SC_METHOD(HandleInt_hsif_errSignalMethod);
    dont_initialize();
    sensitive << int_hsif_err_internal;
    sensitive << int_hsif_sec_internal;
}

HSIFWP0::~HSIFWP0()
{
    for (uint32_t index = 0; index < emCHNum; index ++){
        if (int_hsif_ch[index] != NULL ) {
            delete int_hsif_ch[index];
        }
    }
    delete mHSIF020;
    delete mHSIFL10_ALL;
    delete am_is;
    delete as_ts;
    delete tsp;
}
void HSIFWP0::HandleInt_hsif_errSignalMethod()
{
    bool mInt_hsif_errValue;
    bool CurrSecValue  = int_hsif_sec_internal.read();
    bool CurrErr       = int_hsif_err_internal.read();
    mInt_hsif_errValue = CurrSecValue || CurrErr;
    int_hsif_err->write(mInt_hsif_errValue);
}






