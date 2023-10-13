// ---------------------------------------------------------------------
// $Id: DFE0051_Wrapper.h,v 1.2 2017/07/21 08:48:22 chuonghoangle Exp $
//
// Copyright(c) 2016 Renesas Electronics Corporation
// Copyright(c) 2016 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __DFE0051_WRAPPER_H__
#define __DFE0051_WRAPPER_H__

#include "BusSlaveBase.h"
#include "DFE0051.h"
#include "DFE0051_Func.h"
#include "vector"
#include "DFE0051_FIFO.h"

#define DFE00510_NUM_CHANNEL 16
#define DFE00511_NUM_CHANNEL 4

class Cdfe0051_wrapper: public sc_module
                   ,public BusSlaveBase<32,3>
{
#include "DFE0051_Wrapper_cmdif.h"
private:
    Cdfe0051<DFE00510_NUM_CHANNEL> *mDFE00510;  // DFE00510 instance
    Cdfe0051<DFE00511_NUM_CHANNEL> *mDFE00511;  // DFE00511 instance
    DFE0051_FIFO  *mFIFO;               // FIFO instance
    
    // Signals
    sc_signal<sc_dt::uint64>    CLK_HSB_sig;
    sc_signal<sc_dt::uint64>    CLK_UHSB_sig;
    sc_signal<bool>             PRESETn_sig;

    sc_signal<bool>             DFE00510_FIFO_CACK_sig[DFE00510_NUM_CHANNEL];
    sc_signal<bool>             DFE00510_DOUPD_N_sig[DFE00510_NUM_CHANNEL];
    sc_signal<bool>             DFE00510_DOUT_END_sig[DFE00510_NUM_CHANNEL];
    sc_signal<unsigned int>     DFE00510_DFDOUT_sig[DFE00510_NUM_CHANNEL];
    sc_signal<bool>             DFE00511_FIFO_CACK_sig[DFE00511_NUM_CHANNEL];
    sc_signal<bool>             DFE00511_DOUPD_N_sig[DFE00511_NUM_CHANNEL];
    sc_signal<bool>             DFE00511_DOUT_END_sig[DFE00511_NUM_CHANNEL];
    sc_signal<unsigned int>     DFE00511_DFDOUT_sig[DFE00511_NUM_CHANNEL];

    sc_signal<bool>             INTDFE00510CND0_sig[DFE00510_NUM_CHANNEL];
    sc_signal<bool>             INTDFE00511CND0_sig[DFE00511_NUM_CHANNEL];
    sc_signal<bool>             INTDFE00510DOUT_sig[DFE00510_NUM_CHANNEL];
    sc_signal<bool>             INTDFE00511DOUT_sig[DFE00511_NUM_CHANNEL];

    sc_signal<bool>             INTDFE00510ERR_sig;
    sc_signal<bool>             INTDFE0051FIFOERR_sig;

    bool mDumpInterrupt;        // Enable/Disable dumping the interrupt info
    bool mChannelConnectMatrix[20][DFE00510_NUM_CHANNEL + DFE00511_NUM_CHANNEL];// 20 input source, refer DFE0051.h, emNumInputSource
    bool mPreINTDFE00510ERR;                        // Previous value of INTDFE00510ERR
    bool mPreINTDFE00510DOUTCND[DFE00510_NUM_CHANNEL];  // Previous value of INTDFE00510DOUTCND
    bool mPreINTDFE00510CND[4];                     // Previous value of INTDFE00510CND
    bool mPreINTDFE00510SUBOUT[3];                  // Previous value of INTDFE00510SUBOUT
    bool mPreINTDFE00511ERR;                        // Previous value of INTDFE00511ERR
    bool mPreINTDFE00511DOUTCND[DFE00511_NUM_CHANNEL];  // Previous value of INTDFE00511DOUTCND
    bool mPreINTDFE00511CND[4];                     // Previous value of INTDFE00511CND
    bool mPreINTDFE00511SUBOUT[3];                  // Previous value of INTDFE00511SUBOUT
    bool mPreINTDFE0051FIFOOUTA;                    // Previous value of INTDFE0051FIFOOUTA
    bool mPreINTDFE0051FIFOOUTB;                    // Previous value of INTDFE0051FIFOOUTB

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

    sc_in<bool>                 DSADC14_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC14_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC14_DATA;

    sc_in<bool>                 DSADC15_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC15_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC15_DATA;

    sc_in<bool>                 DSADC21_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC21_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC21_DATA;

    sc_in<bool>                 DSADC22_REQ;
    sc_in<sc_dt::sc_uint<4> >   DSADC22_TAG;
    sc_in<sc_dt::sc_uint<16> >  DSADC22_DATA;

    sc_in<bool>                 CADC0_REQ;
    sc_in<sc_dt::sc_uint<4> >   CADC0_TAG;
    sc_in<sc_dt::sc_uint<16> >  CADC0_DATA;

    sc_in<bool>                 *TMTRG_A[20];
    sc_in<bool>                 *TMTRG_B[20];
    sc_in<bool>                 *TMCAPTRG[3];
    sc_in<bool>                 TMFIFOCAPTRG;

    sc_out<bool>    *DFE00510_PHUPD[4];
    sc_out<bool>    *INTDFE00510DOUTCND[DFE00510_NUM_CHANNEL];
    sc_out<bool>    *INTDFE00510CND[4];
    sc_out<bool>    INTDFE00510ERR;
    sc_out<bool>    *INTDFE00510SUBOUT[3];

    sc_out<bool>    *DFE00511_PHUPD[4];
    sc_out<bool>    *INTDFE00511DOUTCND[DFE00511_NUM_CHANNEL];
    sc_out<bool>    *INTDFE00511CND[4];
    sc_out<bool>    INTDFE00511ERR;
    sc_out<bool>    *INTDFE00511SUBOUT[3];

    sc_out<bool>    INTDFE0051FIFOOUTA;
    sc_out<bool>    INTDFE0051FIFOOUTB;

    // Socket
    TlmTargetSocket<32>*    tsdfe00510; // Target socket to access DFE00510 reg/mem
    TlmTargetSocket<32>*    tsdfe00511; // Target socket to access DFE00511 reg/mem
    TlmTargetSocket<32>*    tsfifo; // Target socket to access FIFO reg

    // APIs
    SC_HAS_PROCESS(Cdfe0051_wrapper);
    Cdfe0051_wrapper(sc_module_name name);
    ~Cdfe0051_wrapper(void);

    void DumpInterruptMethod();
    void MergeINTDFE0051DOUTCNDMethod();
    void MergeINTDFE00510ERRMethod();
    bool CheckChannelConnectMatrix(std::string dfe0051_name, unsigned int source, unsigned int dest_channel);

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
};
#endif //__DFE0051_WRAPPER_H__
