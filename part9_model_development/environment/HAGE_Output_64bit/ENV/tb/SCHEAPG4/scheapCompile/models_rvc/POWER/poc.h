// -----------------------------------------------------------------------------
// $Id: poc.h,v 1.1.1.1 2012/12/19 03:21:03 lamdangpham Exp $
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#ifndef __POC_H__
#define __POC_H__
#include "re_define.h"
#include <cstdio>
#include <cstdarg>
#include <map>
#include <sstream>

#include "general_reset.h"

/// POC model class
class Cpoc: public Cgeneral_reset
{
    
#include "poc_cmdif.h"

public:
    SC_HAS_PROCESS(Cpoc);
    Cpoc(sc_module_name name);
    ~Cpoc();

    // Clocks
    sc_in<sc_dt::uint64> CLK_V00;

    // Input ports
    sc_in<bool> CLR_POF;

    // Output ports
    sc_out<bool> poradet_n_v50;
    sc_out<bool> pocadet_n_v50;
    sc_out<bool> pocdet_n_v50;
    sc_out<bool> det0det_v50;
    sc_out<bool> DET0DET_MSK;
    sc_out<bool> POCADET_N_MSK;
    sc_out<bool> POCDET_N_MSK;
    sc_out<bool> REQ_POF;

private:
    
    // Constance value
    enum eTimeEnum {
        emNanoSecond = 1000000000
    };
    
    enum eRstShpEnum {
        emCellStageNumber = 2 // Cell stage number of Reset Shaper
       ,emSyncDepth      = 2 // Synchronizing depth of Reset Shaper
    };
    
    // Member variables
    unsigned int mVCCDetectAmount;   // Total detection of VCC
    unsigned int mVCCADetectAmount;  // Total detection of VCCA
    std::vector<double> mVccPoraQueue;   // Queue of updated value of VCC for updating poradet_n_v50 
    std::vector<double> mVccPocaQueue;   // Queue of updated value of VCC for updating pocadet_n_v50
    std::vector<double> mVccCmplQueue;   // Queue of updated value of VCC for updating mCmplVal
    std::vector<double> mVccaPocbQueue;  // Queue of updated value of VCCA for updating det0det_v50
    bool mReqPofVal;                 // Value of REQ_POF port
    bool mCmplVal;                   // Value of cmplout
    bool mStartSimFlag;              // Check start simulation flag
    double Tpof;               // Delay period of POF control processing

    // handleCommand variable
    double mVCC;
    double mVCCA;
    double mRefPOCA;
    double mRefCMPL;
    double mRefPOCB;
    bool mSELDETMODE;
    double mTpora;
    double mTpoca;
    double mTcmpl;
    double mTpocb;

    // Events
    sc_event mWritePocEvent;
    sc_event_queue mWritePoraEvent;
    sc_event_queue mWritePocaEvent;
    sc_event_queue mWriteCmplEvent;
    sc_event_queue mWriteDet0Event;
    sc_event_queue mWriteReqPofEvent;

    // Functions
    // Normal functions
    void EnableReset(const bool is_active);
    void Initialize(void);
    void DumpVoltageInfo(std::string voltage, std::string ref_name, double ref_volt, double det_volt);
    void DumpInfo(const char *type, const char *message, ...);
    double GetDetectVolt(std::vector<double> &det_queue, double det_volt);
    void CheckParam(std::string param_name, double &param, double &backup);
    void CheckParam(std::string param_name, unsigned int &param, double &backup);

    // handle_command functions
    void DumpStatInfo(void);
    void AssertReset(const double start_time, const double period);
    void SetCLKfreq(const std::string clk_name, const double clk_freq);
    
    // Callback functions
    std::string CommandCB(const std::vector<std::string>& args);

    // Methods
    void Clk8MMethod(void);
    void PoraProcessMethod(void);
    void PocaProcessMethod(void);
    void CmplProcessMethod(void);
    void PocProcessMethod(void);
    void PocreszProcessMethod(void);
    void POFControlMethod(void);
    void PocbProcessMethod(void);
    void StartSimMethod(void);
};

#endif //__POC_H__
