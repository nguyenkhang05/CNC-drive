// ---------------------------------------------------------------------
// $Id: $
//
// Copyright(c) 2016 - 2017 Renesas Electronics Corporation
// Copyright(c) 2016 - 2017 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __ADCH_ABFG_H__
#define __ADCH_ABFG_H__

#include <stdint.h>
#include "tlm_tgt_if.h"
#include "re_define.h"

class Cabfg_core;
class Cabfg: public sc_module,
              public vpcl::tlm_tgt_if<32>
{
#include "abfg_cmdif.h"
public:
    enum eResetNumber {emPRESETn=0,emResetSignalNum};
    enum eVirtualChannel {
          emADCHVCNum = 40
        , emCADCVCNum = 8
        , emDSADCVCNum = 8
        , emDSADCVCNum4 = 4
        , emDSADCVCNum2 = 2
        , emMinEndInput40 = 40 //40 -> 79
        , emMinEndInput80 = 80 //80->119
        , emMinEndInput120 = 120 //120->159
        , emMinEndInput160 = 160 //160->167
        , emMinEndInput168 = 168 //168->171
        , emMinEndInput172 = 172 //172->173
        , emMinEndInput174 = 174 //174->177
        , emMinEndInput178 = 178 //178->181
        , emMinEndInput182 = 182 //182->185
        , emMinEndInput186 = 186 //186->189
        , emMinEndInput190 = 190 //190->197
        , emMinEndInput198 = 198 //198->205
        , emMinEndInput206 = 206 //206->207
        , emMinEndInput208 = 208 //208->209
    };
    enum eInputChannelSelector {emInputChannelNum=48};

    //Port declaration
    sc_in<uint64>                   CLK_LSB;
    sc_in<bool>                     PRESETn;
    //ADCH0->3
    sc_in<bool>                     *vcend_0[emADCHVCNum];
    sc_in<bool>                     vculmo_0;
    sc_in<bool>                     vcllmo_0;
    sc_in<bool>                     *vcend_1[emADCHVCNum];
    sc_in<bool>                     vculmo_1;
    sc_in<bool>                     vcllmo_1;
    sc_in<bool>                     *vcend_2[emADCHVCNum];
    sc_in<bool>                     vculmo_2;
    sc_in<bool>                     vcllmo_2;
    sc_in<bool>                     *vcend_3[emADCHVCNum];
    sc_in<bool>                     vculmo_3;
    sc_in<bool>                     vcllmo_3;
    //CADC00
    sc_in<bool>                     *CADCVCI00[emCADCVCNum];
    sc_in<bool>                     CADUE00;
    sc_in<bool>                     CADLE00;
    //DSADC00, DSADC10, DSADC20, DSADC12, DSADC13, DSADC11
    sc_in<bool>                     *DSADVCI00[emDSADCVCNum];
    sc_in<bool>                     DSADUE00;
    sc_in<bool>                     DSADLE00;
    sc_in<bool>                     *DSADVCI10[emDSADCVCNum];
    sc_in<bool>                     DSADUE10;
    sc_in<bool>                     DSADLE10;
    sc_in<bool>                     *DSADVCI20[emDSADCVCNum];
    sc_in<bool>                     DSADUE20;
    sc_in<bool>                     DSADLE20;
    sc_in<bool>                     *DSADVCI12[emDSADCVCNum];
    sc_in<bool>                     DSADUE12;
    sc_in<bool>                     DSADLE12;
    sc_in<bool>                     *DSADVCI13[emDSADCVCNum];
    sc_in<bool>                     DSADUE13;
    sc_in<bool>                     DSADLE13;
    sc_in<bool>                     *DSADVCI11[emDSADCVCNum];
    sc_in<bool>                     DSADUE11;
    sc_in<bool>                     DSADLE11;
    sc_in<bool>                     *DSADVCI14[emDSADCVCNum];
    sc_in<bool>                     DSADUE14;
    sc_in<bool>                     DSADLE14;
    sc_in<bool>                     *DSADVCI15[emDSADCVCNum];
    sc_in<bool>                     DSADUE15;
    sc_in<bool>                     DSADLE15;
    sc_in<bool>                     *DSADVCI21[emDSADCVCNum];
    sc_in<bool>                     DSADUE21;
    sc_in<bool>                     DSADLE21;
    sc_in<bool>                     *DSADVCI22[emDSADCVCNum];
    sc_in<bool>                     DSADUE22;
    sc_in<bool>                     DSADLE22;
    sc_out<bool>                    *bflg_flag[emInputChannelNum];
    sc_out<bool>                    *bflg_bfpulse[emInputChannelNum];
private:
    Cabfg_core                     *mABFGcore;

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
    bool                            mBFOut[emInputChannelNum];
    bool                            mBFPOut[emInputChannelNum];

    //Declare events
    //Not canceled when reset is active
    sc_event                        mResetHardEvent[emResetSignalNum];                      //Call HandleResetHardMethod when reset port is active
    sc_event                        mResetCmdEvent[emResetSignalNum];                       //Call HandleResetCmdMethod when AssertReset is called
    sc_event                        mResetCmdCancelEvent[emResetSignalNum];                 //Call CancelResetCmdMethod when AssertReset is over
    //Canceled when reset is active
    sc_event                        mNotifyFilterControlEvent[emInputChannelNum];
    sc_event                        mNotifyBFPulseDoneEvent[emInputChannelNum];
    sc_event                        mWriteBFEvent[emInputChannelNum];
    sc_event                        mWriteBFPulseEvent[emInputChannelNum];

public:
    SC_HAS_PROCESS(Cabfg);
    Cabfg(sc_module_name name);
    ~Cabfg(void);

    //PythonIF
    void AssertReset(const double start_time, const double period);
    void SetCLKfreq(const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKfreq(const std::string clock_name);
    std::string RegisterHandleCommand(const std::vector<std::string>& args);
    void NotifyBFOut(const unsigned int chn_sel, const bool val, const bool isBFPOut);

private:
    //Virtual function of tlm_tgt_if
    void tgt_acc(unsigned int id, tlm::tlm_generic_payload &trans, sc_time &t);
    void tgt_acc(tlm::tlm_generic_payload &trans, sc_time &t);
    unsigned int tgt_acc_dbg(unsigned int id, tlm::tlm_generic_payload &trans);
    unsigned int tgt_acc_dbg(tlm::tlm_generic_payload &trans);

    void Initialize (void);                                                                 //Initializes internal variables & output ports
    void CancelEvents(void);                                                                //Cancel all operation events
    void ResetOutputPort(void);                                                             //Reset output port when reset is active
    void EnableReset(const bool is_active, const unsigned int reset_id);                    //Enable/Disable reset state
    void EndPulseControl(const unsigned int chan_num, const bool end_pulse,
                         const bool upper_error, const bool lower_error);

    void HandleCLK_LSBSignalMethod(void);
    void HandlePRESETnSignalMethod(void);
    void HandleResetHardMethod(const unsigned int reset_id);
    void HandleResetCmdMethod(const unsigned int reset_id);
    void CancelResetCmdMethod(const unsigned int reset_id);
    void HandleADCH0EndInputMethod(const unsigned int id);
    void HandleADCH1EndInputMethod(const unsigned int id);
    void HandleADCH2EndInputMethod(const unsigned int id);
    void HandleADCH3EndInputMethod(const unsigned int id);
    void HandleDSADC00EndInputMethod(const unsigned int id);
    void HandleDSADC10EndInputMethod(const unsigned int id);
    void HandleDSADC11EndInputMethod(const unsigned int id);
    void HandleDSADC12EndInputMethod(const unsigned int id);
    void HandleDSADC13EndInputMethod(const unsigned int id);
    void HandleDSADC14EndInputMethod(const unsigned int id);
    void HandleDSADC15EndInputMethod(const unsigned int id);
    void HandleDSADC20EndInputMethod(const unsigned int id);
    void HandleDSADC21EndInputMethod(const unsigned int id);
    void HandleDSADC22EndInputMethod(const unsigned int id);
    void HandleCADC00EndInputMethod(const unsigned int id);
    void NotifyFilterControlMethod(const unsigned int chn_sel);
    void NotifyBFPulseDoneMethod(const unsigned int chn_sel);
    void WriteBFMethod(const unsigned int chn_sel);
    void WriteBFPulseMethod(const unsigned int chn_sel);

    bool CheckClockPeriod(const std::string clock_name, const sc_dt::uint64 clock_period);
    double GetCurTime(void);
    double CalculateClockEdge(const double clock_period, const bool is_pos, const bool add_period,
                               const double time_point, const double time_change_point);
    void ConvertClockFreq(sc_dt::uint64 &freq_out, std::string &unit_out,
                           const sc_dt::uint64 freq_in, const std::string unit_in);
    void GetTimeResolution(sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SetLatency_TLM(const bool is_constructor);
};
#endif
