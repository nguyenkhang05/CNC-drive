/***********************************************************************
*
* NSMVG4SSLITE.h
*
* Copyright(c) 2016-2021 Renesas Electronics Corporation
* RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
* This program must be used solely for the purpose for which
* it was furnished by Renesas Electronics Corporation. No part of this
* program may be reproduced or disclosed to others, in any
* form, without the prior written permission of Renesas Electronics
* Corporation.
*
***********************************************************************/

#ifndef NSMVG4SSLITE_H
#define NSMVG4SSLITE_H

#include "tlm.h"      // for OSCI TLM2.0
#include "OSCI2.h"    // class for User Modeling environment based on TLM2.0
#include "global.h"
#include "slct_sc_signal_ports.h"

class APISS;
template< unsigned int BUSWIDTH > class DummyMaster;
template< unsigned int BUSWIDTH > class DummySlave;

class NSMVG4SSLITE : public sc_module
{
private:
    char mMapFileName[1024]; // bus map file
    void read_config_file(const char *filename);
    DummyMaster<BUS_WIDTH_APB> *mpDummyMaster;
    DummySlave<BUS_WIDTH_APB> *mpDummySlave;

public:
    SIM_MODE_T mIssMode; // for the simulation mode 
    slct_sc_in<bool> sys_reset; // reset port
    slct_sc_in<sc_dt::uint64> GAPB_SG1_clk; // GAPB SG1 bus clock
    TlmInitiatorSocket<BUS_WIDTH_APB> isg_sg1; //initiator socket for GAPB SG1
    APISS *P_BUS; // P_BUS

    NSMVG4SSLITE(sc_module_name module_name, const char *config_file);
    ~NSMVG4SSLITE(void);
};

#endif /* NSMVG4SSLITE_H */
