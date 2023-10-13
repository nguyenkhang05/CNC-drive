// ---------------------------------------------------------------------
// $Id: $
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
#ifndef __ADCH_AVSEG_H__
#define __ADCH_AVSEG_H__

#include <stdint.h>
#include "tlm_tgt_if.h"
#include "re_define.h"

class Cavseg_core;
class Cavseg: public sc_module,
              public vpcl::tlm_tgt_if<32>
{
#include "avseg_cmdif.h"
public:
    enum eResetNumber {emPRESETn=0,emResetSignalNum};
    enum eVirtualChannel {
         emADCHVCNum = 64
        ,emVCRnum_Max_U2 = 64
        ,emVCRnum_Max_E2 = 40
    };
    
    enum eAVSEG_CONSTANCE {
        emTargetConfigNum = 2
    };

    enum eInputChannelSelector {emVCC=0,emEVCC,emAWOVDD,emISOVDD,emInputChannelNum};

    //Port declaration
    sc_in<uint64>                   CLK_LSB;
    sc_in<bool>                     PRESETn;
    sc_in<bool>                     *vcend[emADCHVCNum];
    sc_in<bool>                     vculmo;
    sc_in<bool>                     vcllmo;
    sc_out<bool>                    evccshdet;
    sc_out<bool>                    evccsldet;
    sc_out<bool>                    vccshdet;
    sc_out<bool>                    vccsldet;
    sc_out<bool>                    awovddshdet;
    sc_out<bool>                    awovddsldet;
    sc_out<bool>                    isovddshdet;
    sc_out<bool>                    isovddsldet;

    // Attributes
    sc_core::sc_attribute<std::string> TARGET_PRODUCT;          
    sc_core::sc_attribute<std::string> VCRnum0;                // Number of Virtual channel of ADC0
    unsigned int mADC0_vir_num;

private:
    Cavseg_core                     *mAVSEGcore;

    struct TargetConfig { 
       std::string target_product_name;
       unsigned int virtual_ch_num0;
    };


    //Declare variables
    //Not initialized when reset active
    sc_dt::uint64                   mTimeResolutionValue;                                   //Time resolution value of simulation
    sc_time_unit                    mTimeResolutionUnit;                                    //Time resolution unit of simulation
    bool                            mResetCurVal[emResetSignalNum];                         //Store current value of reset
    bool                            mResetPreVal[emResetSignalNum];                         //Store previous value of reset
    bool                            mIsResetHardActive[emResetSignalNum];                   //Reset operation status of reset signal
    bool                            mIsResetCmdReceive[emResetSignalNum];                   //Receive status of AssertReset command of reset
    bool                            mIsResetCmdActive[emResetSignalNum];                    //Active status of AssertReset command of reset
    double                          mResetCmdPeriod[emResetSignalNum];                      //Period of AssertReset command of reset
    double                          mTimeCLKChangePoint;                                    //The time clock is changed
    sc_dt::uint64                   mCLK_LSBPeriod;                                         //Period value of CLK_LSB clock
    sc_dt::uint64                   mCLK_LSBFreq;                                           //Frequency value of CLK_LSB clock
    sc_dt::uint64                   mCLK_LSBOrgFreq;                                        //Original frequency value of CLK_LSB clock
    std::string                     mCLK_LSBFreqUnit;                                       //Frequency unit of CLK_LSB clock
    //Initialized when reset active
    bool                            mEndPulse[emInputChannelNum];
    bool                            mUpperErr[emInputChannelNum];
    bool                            mLowerErr[emInputChannelNum];
    bool                            mUpperErrOut[emInputChannelNum];
    bool                            mLowerErrOut[emInputChannelNum];

    TargetConfig mTargetConfigTable[emTargetConfigNum];

    //Declare events
    //Not canceled when reset is active
    sc_event                        mResetHardEvent[emResetSignalNum];                      //Call HandleResetHardMethod when reset port is active
    sc_event                        mResetCmdEvent[emResetSignalNum];                       //Call HandleResetCmdMethod when AssertReset is called
    sc_event                        mResetCmdCancelEvent[emResetSignalNum];                 //Call CancelResetCmdMethod when AssertReset is over
    //Canceled when reset is active
    sc_event                        mNotifyFilterControlEvent[emInputChannelNum];
    sc_event                        mNotifyErrPulseDoneEvent[emInputChannelNum];
    sc_event                        mWriteErrPulseEvent[emInputChannelNum];

public:
    SC_HAS_PROCESS(Cavseg);
    Cavseg(sc_module_name name);
    ~Cavseg (void);

    //PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKfreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKfreq (const std::string clock_name);
    std::string RegisterHandleCommand(const std::vector<std::string>& args);
    void NotifyErrPulseOut (const unsigned int chn_sel, const unsigned int ex_bound);

private:
    //Virtual function of tlm_tgt_if
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);

    void Initialize (void);                                                                 //Initializes internal variables & output ports
    void CancelEvents (void);                                                               //Cancel all operation events
    void ResetOutputPort (void);                                                            //Reset output port when reset is active
    void EnableReset (const bool is_active, const unsigned int reset_id);                   //Enable/Disable reset state

    void HandleCLK_LSBSignalMethod (void);
    void HandlePRESETnSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);
    void HandleADEndInputMethod (const unsigned int chan_num);
    void NotifyFilterControlMethod (const unsigned int chn_sel);
    void NotifyErrPulseDoneMethod (const unsigned int chn_sel);
    void WriteErrPulseMethod (const unsigned int chn_sel);

    bool CheckClockPeriod (const std::string clock_name);
    double GetCurTime(void);
    double CalculateClockEdge (const double clock_period, const bool is_pos, const bool add_period,
                               const double time_point, const double time_change_point);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out,
                           const sc_dt::uint64 freq_in, const std::string unit_in);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SetLatency_TLM (const bool is_constructor);
    void UpdateTargetProductMethod (void);
};
#endif
