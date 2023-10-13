// ---------------------------------------------------------------------
// $Id: HSIFWP0.h,v 1.5 2020/03/16 02:13:07 quynhnhunguyen Exp $
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

#ifndef __HSIFWP0_HSIFWP0_H__
#define __HSIFWP0_HSIFWP0_H__

#include "HSIF020.h"
#include "HSIFL10_ALL.h"
#include "HSIF_Common.h"
#include <sstream>

class HSIFWP0:public sc_module,
              public HSIF_ultis
{
public:
    sc_in<sc_dt::uint64>        APBCK_in;           /// PCLK for L1 and CCLK for L2
    sc_in<sc_dt::uint64>        AXICK_in;           /// AXICK  for L2
    sc_in<sc_dt::uint64>        clk20Mhz;           /// CCLK for L1
    sc_in<sc_dt::uint64>        hsif_refclk_in;
    sc_in<bool>                 x_spreset_ipc;      /// reset for both L1 and L2
    sc_out<sc_dt::uint64>       hsif_refclk_out;

    sc_in<L1Frame>              hsif_rxd;
    sc_in<uint32_t>             hsif_rxd_control;

    sc_out<L1Frame>             hsif_txd;
    sc_out<uint32_t>            hsif_txd_control;
    sc_out<bool>                hsif_authent1_pass;
    sc_out<bool>                hsif_authent2_pass;
    sc_out<bool>               *int_hsif_ch[emCHNum];
    sc_out<bool>                int_hsif_str;
    sc_out<bool>                int_hsif_err;
    sc_out<bool>                int_hsif_sec;
    sc_out<bool>                int_hsif_txcmp;
    sc_out<bool>                int_hsif_txerr;
    sc_out<bool>                int_hsif_rxcmp;
    sc_out<bool>                int_hsif_rxerr;
    sc_out<bool>                int_hsif_rxiclc;

    TlmInitiatorSocket<64>*      am_is;            /// <For HSIF020's registers
    TlmTargetSocket<64>*         as_ts;            /// <For HSIF020's registers
    TlmTargetSocket<32>*         tsp;              /// <For HSIFL10's registers

    sc_signal<bool>                int_hsif_err_internal;
    sc_signal<bool>                int_hsif_sec_internal;
    sc_signal<L2Frame>            hsif_tx_data;
    sc_signal<uint32_t>            hsif_tx_control;
    sc_signal<bool>                hsif_tx_data_ready;
    sc_signal<bool>                hsif_tx_frame_cmp;
    sc_signal<L2Frame>            hsif_rx_data;
    sc_signal<uint32_t>            hsif_rx_control;

    //Class initiator
    SC_HAS_PROCESS(HSIFWP0);
    HSIFWP0 (sc_module_name name,
            unsigned int rLatency,
            unsigned int wLatency,
            SIM_MODE_T simmode,
            std::string version_name);
    ~HSIFWP0 (void);
    HSIFL10_ALL                    *mHSIFL10_ALL;
    HSIF020                        *mHSIF020;
// method for handle int_hsif_err ouput
private:
    void HandleInt_hsif_errSignalMethod();
};


#endif /* __HSIFWP0_HSIFWP0_H__ */
