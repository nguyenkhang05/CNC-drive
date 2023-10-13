// ---------------------------------------------------------------------
// $Id: DFE_Wrapper.h,v 1.2 2018/09/06 10:17:09 huepham Exp $
//
// Copyright(c) 2018 Renesas Electronics Corporation
// Copyright(c) 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DFE_WRAPPER_H__
#define __DFE_WRAPPER_H__

#include "BusSlaveBase.h"
#include "DFE.h"
#include "DFE_Func.h"
#include "vector"
#include "DFE_FIFO.h"

#define DFE0_NUM_CHANNEL 12
#define DFE1_NUM_CHANNEL 4

class Cdfe_wrapper: public sc_module
                   ,public BusSlaveBase<32,3>
{
#include "DFE_Wrapper_cmdif.h"
private:
    Cdfe<DFE0_NUM_CHANNEL> *mDFE0;  // DFE0 instance
    Cdfe<DFE1_NUM_CHANNEL> *mDFE1;  // DFE1 instance
    DFE_FIFO  *mFIFO;               // FIFO instance
    
    // Signals
    sc_signal<sc_dt::uint64>    CLK_HSB_sig;
    sc_signal<sc_dt::uint64>    CLK_UHSB_sig;
    sc_signal<bool>             PRESETn_sig;

    sc_signal<bool>             DFE0_FIFO_CACK_sig[DFE0_NUM_CHANNEL];
    sc_signal<bool>             DFE0_DOUPD_N_sig[DFE0_NUM_CHANNEL];
    sc_signal<bool>             DFE0_DOUT_END_sig[DFE0_NUM_CHANNEL];
    sc_signal<unsigned int>     DFE0_DFDOUT_sig[DFE0_NUM_CHANNEL];
    sc_signal<bool>             DFE1_FIFO_CACK_sig[DFE1_NUM_CHANNEL];
    sc_signal<bool>             DFE1_DOUPD_N_sig[DFE1_NUM_CHANNEL];
    sc_signal<bool>             DFE1_DOUT_END_sig[DFE1_NUM_CHANNEL];
    sc_signal<unsigned int>     DFE1_DFDOUT_sig[DFE1_NUM_CHANNEL];

    sc_signal<bool>             INTDFE0CND0_sig[DFE0_NUM_CHANNEL];
    sc_signal<bool>             INTDFE1CND0_sig[DFE1_NUM_CHANNEL];
    sc_signal<bool>             INTDFE0DOUT_sig[DFE0_NUM_CHANNEL];
    sc_signal<bool>             INTDFE1DOUT_sig[DFE1_NUM_CHANNEL];

    sc_signal<bool>             INTDFE0ERR_sig;
    sc_signal<bool>             INTDFEFIFOERR_sig;

    bool mDumpInterrupt;        // Enable/Disable dumping the interrupt info
    bool mChannelConnectMatrix[16][DFE0_NUM_CHANNEL + DFE1_NUM_CHANNEL];// 16 input source, refer DFE.h, emNumInputSource
    bool mPreINTDFE0ERR;                        // Previous value of INTDFE0ERR
    bool mPreINTDFE0DOUTCND[DFE0_NUM_CHANNEL];  // Previous value of INTDFE0DOUTCND
    bool mPreINTDFE0CND[4];                     // Previous value of INTDFE0CND
    bool mPreINTDFE0SUBOUT[3];                  // Previous value of INTDFE0SUBOUT
    bool mPreINTDFE1ERR;                        // Previous value of INTDFE1ERR
    bool mPreINTDFE1DOUTCND[DFE1_NUM_CHANNEL];  // Previous value of INTDFE1DOUTCND
    bool mPreINTDFE1CND[4];                     // Previous value of INTDFE1CND
    bool mPreINTDFE1SUBOUT[3];                  // Previous value of INTDFE1SUBOUT
    bool mPreINTDFEFIFOOUTA;                    // Previous value of INTDFEFIFOOUTA
    bool mPreINTDFEFIFOOUTB;                    // Previous value of INTDFEFIFOOUTB

    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    void CreateChannelConnectMatrix();
public:
    // Port
    sc_in<sc_dt::uint64>        CLK_HSB;
    sc_in<sc_dt::uint64>        CLK_UHSB;
    sc_in<bool>                 PRESETn;

    sc_in<bool>                 SARADC0_REQ;
    sc_in<sc_dt::sc_uint<4> >   SARADC0_TAG;
    sc_in<sc_dt::sc_uint<16> >  SARADC0_DATA;

    sc_in<bool>                 SARADC1_REQ;
    sc_in<sc_dt::sc_uint<4> >   SARADC1_TAG;
    sc_in<sc_dt::sc_uint<16> >  SARADC1_DATA;

    sc_in<bool>                 SARADC2_REQ;
    sc_in<sc_dt::sc_uint<4> >   SARADC2_TAG;
    sc_in<sc_dt::sc_uint<16> >  SARADC2_DATA;

    sc_in<bool>                 SARADC3_REQ;
    sc_in<sc_dt::sc_uint<4> >   SARADC3_TAG;
    sc_in<sc_dt::sc_uint<16> >  SARADC3_DATA;

    sc_in<bool>                 DSADC00_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC00_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC00_DATA;

    sc_in<bool>                 DSADC10_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC10_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC10_DATA;

    sc_in<bool>                 DSADC20_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC20_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC20_DATA;

    sc_in<bool>                 DSADC11_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC11_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC11_DATA;

    sc_in<bool>                 DSADC12_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC12_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC12_DATA;

    sc_in<bool>                 DSADC13_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC13_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC13_DATA;

    sc_in<bool>                 CADC0_REQ;
    sc_in<sc_dt::sc_uint<4> >   CADC0_TAG;
    sc_in<sc_dt::sc_uint<16> >  CADC0_DATA;

    sc_in<bool>                 *TMTRG_A[20];
    sc_in<bool>                 *TMTRG_B[20];
    sc_in<bool>                 *TMCAPTRG[3];
    sc_in<bool>                 TMFIFOCAPTRG;

    sc_out<bool>                *DFE0_PHUPD[4];
    sc_out<bool>                *INTDFE0DOUTCND[DFE0_NUM_CHANNEL];
    sc_out<bool>                *INTDFE0CND[4];
    sc_out<bool>                INTDFE0ERR;
    sc_out<bool>                *INTDFE0SUBOUT[3];

    sc_out<bool>                *DFE1_PHUPD[4];
    sc_out<bool>                *INTDFE1DOUTCND[DFE1_NUM_CHANNEL];
    sc_out<bool>                *INTDFE1CND[4];
    sc_out<bool>                INTDFE1ERR;
    sc_out<bool>                *INTDFE1SUBOUT[3];

    sc_out<bool>                INTDFEFIFOOUTA;
    sc_out<bool>                INTDFEFIFOOUTB;

    // Socket
    TlmTargetSocket<32>*    tsdfe0; // Target socket to access DFE0 reg/mem
    TlmTargetSocket<32>*    tsdfe1; // Target socket to access DFE1 reg/mem
    TlmTargetSocket<32>*    tsfifo; // Target socket to access FIFO reg

    // APIs
    SC_HAS_PROCESS(Cdfe_wrapper);
    Cdfe_wrapper(sc_module_name name);
    ~Cdfe_wrapper(void);

    void DumpInterruptMethod();
    void MergeINTDFEDOUTCNDMethod();
    void MergeINTDFE0ERRMethod();
    bool CheckChannelConnectMatrix(std::string dfe_name, unsigned int source, unsigned int dest_channel);

    /// PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string unit_name, const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string unit_name, const std::string reg_name);
    void WriteRegister (const std::string unit_name, const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string unit_name, const std::string reg_name);
    void ListRegister (void);
    void WriteCoefficient (const std::string unit_name, const unsigned int offset, const unsigned int value);
    void ReadCoefficient (const std::string unit_name, const unsigned int offset);
    void WriteDataMem (const std::string unit_name, const unsigned int offset, const unsigned int value);
    void ReadDataMem (const std::string unit_name, const unsigned int offset);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void DumpStatusInfo (const std::string unit_name);                    ///< Dump the status information of model
};
#endif //__DFE_WRAPPER_H__
