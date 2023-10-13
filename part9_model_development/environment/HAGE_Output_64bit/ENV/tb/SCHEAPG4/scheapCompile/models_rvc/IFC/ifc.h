// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012-2015 Renesas System Design Co., Ltd.
// Copyright(c) 2012-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __IFC_H__
#define __IFC_H__

#include "systemc.h"
#include "tlm.h"
#include "tlm_tgt_if.h"
#include <iterator>

#include "ifc_regif.h"

/// SAR model class
class Cifc: public sc_module
             ,public vpcl::tlm_tgt_if<32>
             ,public Cifc_regif
{

#include "ifc_cmdif.h"

public:

    SC_HAS_PROCESS(Cifc);
    Cifc(sc_module_name name);
    ~Cifc();

    //Port declaration
    sc_in<sc_dt::uint64> pclk;
    sc_in<bool> preset_n;
    sc_out< sc_uint<6> > IFVCH;
    sc_out<bool> IFRD0;
    sc_out<bool> IFRD1;
    sc_out<bool> IFRE;
    sc_in< sc_uint<16> > ADC0_IFDATA;
    sc_in< sc_uint<16> > ADC1_IFDATA;

private:
    //Enum
    enum eIFC_CONSTANCE {
        emNanoSecond = 1000000000
       ,emADC0Num = 0
       ,emADC1Num = 1
       ,emMaxADCNum = 2
       ,emADC0AddrMask = 0xFF
       ,emADC1AddrMask = 0x1FF
       ,emMaxVirChannel = 48
       ,emMaxDRBitVal = 15
       ,emDataSignedMask  = 0x8000
       ,emDataMask  = 0x7FFF
    };

    enum eConvertMask {
        emSignedMask        = 0x80000000
       ,emExpMask           = 0x7F800000 
       ,emFractionMask      = 0x007FFFFF 
    };

    //Variable 
    unsigned int mResetPeriod;
    bool mIsCmdResetFlag;
    bool mIFCPortResetFlag;
    bool mIFCCmdResetFlag;
    bool mIsInitialize;
    bool mIsStartSim;
    double mPCLKPeriod;
    unsigned int mCurVCNum;

    bool mIFRD0Val;
    bool mIFRD1Val;
    bool mIFREVal;

    //Event
    sc_event mCmdResetEvent;
    sc_event mCmdCancelResetEvent;
    sc_event mUpdateIFVCHEvent;
    sc_event mUpdateIFRD0Event;
    sc_event mUpdateIFRD1Event;
    sc_event mUpdateIFREEvent;

    //tlm function
    void tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int  tgt_acc_dbg(tlm::tlm_generic_payload &trans);

    //call_back function

    //Normal function
    void EnableReset(const bool is_active);
    void UpdateDRValue(const unsigned int adc_num, const unsigned int vc_num);
    unsigned int SFPtoFPConvert(unsigned int value);

    //handle_command function
    void AssertReset(const double start_time, const double period);

    //method
    void PCLKMethod(void);
    void ResetMethod(void);
    void AssertResetMethod(void);
    void DeAssertResetMethod(void);
    void InitializeMethod(void);
    void UpdateIFVCHMethod(void);
    void UpdateIFRD0Method(void);
    void UpdateIFRD1Method(void);
    void UpdateIFREMethod(void);
    unsigned int ProcessReadAccess(unsigned int addr);

    double GetTimeResolution(void);
    void SetLatency_TLM(const double pclk_period, const bool is_constructor);
    void SetCLKfreq(std::string clk_name, double clk_freq);
};

#endif //__SAR_H__
