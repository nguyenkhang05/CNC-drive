// ---------------------------------------------------------------------
// $Id: RHSB.h,v 1.3 2018/08/02 08:08:50 ngathunguyen Exp $
//
// Copyright(c) 2016 - 2018 Renesas Electronics Corporation
// Copyright(c) 2016 - 2018 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __RHSB_H__
#define __RHSB_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "OSCI2.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "Cgeneral_timer.h"
#include "RHSB_define.h"

using namespace rhsb_def;

class RHSB_Func;

class RHSB:public BusSlaveBase<32,1>,
           public Cgeneral_timer
{
friend class RHSB_Func;
#include "RHSB_cmdif.h"
public:
    //Declare input signals
    sc_in<sc_dt::uint64> CLK_LSB;
    sc_in<sc_dt::uint64> CLK_RHSB;
    sc_in<bool> PRESETn;
    sc_in<bool> rstc_n;
    sc_in<bool> EMRG;
    sc_in<sc_dt::uint64>  tmr;
    sc_in<unsigned int>   U_CONTROL0;
    sc_in<unsigned int>   U_CONTROL1;
    sc_in<unsigned int>   SI0;
    sc_in<unsigned int>   SI1;

    //Declare output signals
    sc_out<sc_dt::uint64> FCL;
    sc_out<unsigned int>  D_CONTROL;
    sc_out<sc_dt::uint64> SO;
    sc_out<bool>          CSD0;
    sc_out<bool>          CSD1;
    sc_out<bool>          INTL0;
    sc_out<bool>          INTL1;
    sc_out<bool>          INTL2;
    sc_out<bool>          INTL3;
    sc_out<bool>          INTL4;
    sc_out<bool>          INTDWNDATADMA;
    sc_out<bool>          INTDWNCMDDMA;
    sc_out<bool>          INTUPDMA;

    sc_signal <bool>      DTSF_sig;
    sc_signal <bool>      DTF_sig;
    sc_signal <bool>      CTF_sig;
    sc_signal <bool>      TSF_sig;
    sc_signal <bool>      UEF_sig;
    sc_signal <bool>      TOF_sig;
    sc_signal <bool>      DLF_sig;

    //Declare target socket
    TlmTargetSocket<32> *ts;

    SC_HAS_PROCESS (RHSB);
    RHSB (sc_module_name name, 
          unsigned int rLatency,
          unsigned int wLatency);
    ~RHSB (void);

    //Declare public functions
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);

private:
    //Instantiate RHSB_Func class
    RHSB_Func *mRHSB_Func;

    //Declare private members
    sc_dt::uint64 mCLK_LSBPeriod;              //Period value of CLK_LSB clock
    sc_dt::uint64 mCLK_RHSBPeriod;              //Period value of CLK_RHSB clock
    sc_dt::uint64 mCLK_LSBFreq;                //Frequency value of CLK_LSB clock (Hz)
    sc_dt::uint64 mCLK_LSBOrgFreq;             //Original frequency value of CLK_LSB clock
    sc_dt::uint64 mCLK_RHSBFreq;                //Frequency value of CLK_RHSB clock (Hz)
    sc_dt::uint64 mCLK_RHSBOrgFreq;             //Original frequency value of CLK_RHSB clock
    std::string mCLK_LSBFreqUnit;              //Frequency unit of CLK_LSB clock
    std::string mCLK_RHSBFreqUnit;              //Frequency unit of CLK_RHSB clock
    sc_dt::uint64 mTimeResolutionValue;        //Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;          //Time resolution unit of simulation

    bool mResetCurVal[emResetSignalNum];       //Store current value of PRESETn or rstc_n
    bool mResetPreVal[emResetSignalNum];       //Store previous value of PRESETn or rstc_n
    bool mIsResetHardActive[emResetSignalNum]; //Reset operation status of PRESETn or rstc_n signal
    bool mIsResetCmdReceive[emResetSignalNum]; //Receive status of AssertReset command of PRESETn or rstc_n
    bool mIsResetCmdActive[emResetSignalNum];  //Active status of AssertReset command of PRESETn or rstc_n
    double mResetCmdPeriod[emResetSignalNum];  //Period of AssertReset command of PRESETn or rstc_n
    bool mIntrValue[emInterruptNum];           //Indicate value of interrupt factor

    bool mDumpInterrupt;                       //Whether dump interrupt message
    unsigned int mPreSTBE[emSlaveChannelNum];  //Previous STBE value of U_CONTROL0 and U_CONTROL1
    unsigned int mCurSTBE[emSlaveChannelNum];  //Current STBE value of U_CONTROL0 and U_CONTROL1
    unsigned int mBITT[emSlaveChannelNum];  //Current BITT value of U_CONTROL0 and U_CONTROL1
    unsigned int mSIZE[emSlaveChannelNum];  //Current BITT value of U_CONTROL0 and U_CONTROL1
    unsigned int mReceptionData[emSlaveChannelNum];               //Data reception
    double mStartReceptionTime[emSlaveChannelNum];               //Time at the start data reception
    unsigned int mCurrentReceptionChannel;      // Indicate current reception channel
    bool mIsDataTransferring;                  //Status of data transfer
    bool mIsDataReceiving;                     //Status of data receiving
    bool mIsRX_Valid;                           //Status of data receiving
    bool mIsRX_FERR;                            //Status of data receiving
    unsigned short mData[4];                   //Store data for each DFTE
    bool mIsTimerRunning;                      //Status of timer
    bool mIsEmergencyEnabled;                  //Status of emergency enable
    bool mIsEmergencyAutoStop;                 //Status of emergency auto stop condition
    unsigned int mEmergencyFrameCnt;
    bool mIsEmergencyData;
    double mTimeTick;
    double mIsFstData;                            //Check whether first data frame or not (multi period repetition)
    unsigned int mTransCmdCnt;                  //The number continuous command frames is transmitted
    bool mIsDMARequest[3];
    sc_dt::uint64 mSO2SI_val;
    unsigned int mD2U_CONTROLval;
    sc_dt::uint64 mFCLVal;                      // Value of FCL port
    unsigned int mDCONTROLVal;                  // Value of D_CONTROL port
    sc_dt::uint64 mSOVal;                       // Value of SO port
    bool mCSD0Val;                              // Value of CSD0 port
    bool mCSD1Val;                              // Value of CSD1 port
    sc_event mResetHardEvent[emResetSignalNum];     //Call HandleResetHardMethod when PRESETn or rstc_n is active
    sc_event mResetCmdEvent[emResetSignalNum];      //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];//Call CancelResetCmdMethod when AssertReset is over
    sc_event mIntrUpdateEvent[emInterruptNum];          //Call update of interrupt port

    sc_event mReceptionDataEvent[emSlaveChannelNum];//Call DataReceptionMethod to receive data on upstream communication
    sc_event mUpstreamTimeOutEvent[emSlaveChannelNum]; //Call UpstreamTimeOutMethod to handle upstream time out
    sc_event mStoreReceptionEvent;                  //Call StoreReceptionMethod to store received data on upstream communication
    sc_event mDownstreamTransmissionEvent;          //Call DownstreamCommunicationThread to transmit data/command
    sc_event mDataFrameTransmissionEvent;        //Call DataFrameTransmissionMethod to sent data frame
    sc_event mCommandFrameTransmissionEvent;        //Call CommandFrameTransmissionMethod to sent data frame
    sc_event mDataFrameTransmissionCompletionEvent; //Inform data frame transmission is completed
    sc_event mCommandFrameTransmissionCompletionEvent; //Inform command frame transmission is completed
    sc_event mRepetitionPeriodExpireEvent;          //Inform repetition period is expired
    sc_event mClkZeroEvent;          //Inform repetition period is expired
    sc_event mRstStateEvent;
    sc_event mStartTimerEvent;
    sc_event mStopDataTransEvent;
    sc_event mContCmdEvent;            //Continuous command event
    sc_event mACCChangeDoneEvent;            //Continuous command event
    sc_event mTestmodeSlave0Event;
    sc_event mTestmodeSlave1Event;

    sc_event mFCLPortEvent;                      // Notifycation writing FCL port
    sc_event mD_CONTROLPortEvent;                // Notifycation writing D_CONTROL port
    sc_event mSOPortEvent;                       // Notifycation writing SO port
    sc_event mCSDPortEvent[emSlaveChannelNum];   // Notifycation writing CSD port

    //Define functions
    void HandleCLK_LSBSignalMethod (void);
    void HandleCLK_RHSBSignalMethod (void);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);
    void HandlePRESETnSignalMethod (void);
    void Handlerstc_nSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);

    void HandleIntrMethod(const unsigned interrupt_id);

    void HandleU_CONTROL0SignalMethod (void);
    void HandleU_CONTROL1SignalMethod (void);
    void EmergencyMethod (void);
    void DataReceptionMethod (const unsigned int line_id);
    void StoreReceptionMethod (void);
    void UpstreamTimeOutMethod (const unsigned int line_id);
    void DataFrameTransmissionThread (void);
    void CommandFrameTransmissionThread (void);
    void DownstreamCommunicationThread (void);
    void StartTimerMethod (void);
    void ACCChangeDoneMethod (void);

    void Initialize (void);
    void ResetOutputPort (void);
    void ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out
                          ,sc_dt::uint64 freq_in, std::string unit_in);
    void GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit);
    void SeparateString (std::vector<std::string> &vtr, const std::string msg);
    double CalculateCLKPosEdge (const std::string clock_name, const double time_point);
    void EnableReset (const bool is_active);
    sc_dt::uint64 GetCLK_RHSBPeriod (void);
    sc_dt::uint64 GetCLK_RHSBFreq (void);
    bool CheckClockPeriod (const std::string clock_name);
    void CancelEvents (void);
    void NotifyDownstreamTransmission (void);
    void NotifyDMAResponse (void);
    void NotifyStopDataTrans (void);
    void NotifyCmdRequest (void);
    void PrepareData (sc_dt::uint64 &data, const unsigned short input_data, const unsigned int pos, const unsigned int bit_len);
    sc_dt::uint64 GetTimerInputPort (void);
    void HandleFCLPortMethod ();
    void HandleD_CONTROLPortMethod ();
    void HandleSOPortMethod ();
    void HandleCSDPortMethod (const unsigned int);
    void ControlInterruptPorts (const bool is_reset, const unsigned int interrupt_id);
    void SetCounterPeriod (const double counter_period);
    void SetCompareMatchValue (const unsigned int match_value);
    unsigned int GetFrameLength (const bool is_cmd_frame, const unsigned int period_num);
    unsigned int GetCounterValue (void);
    void StartStopTimer (const bool is_start);
    double GetCurTime(void);
    void FinishTransDataFrm(void);
    void ACCChangeDone(void);
    void HandleDataTransStatusMethod();
    void HandleCommandTransStatusMethod();
    void HandleERRTimeOutDataLostMethod();
    //Declare virtual function of Cgeneral_timer class
    void cmpMatchOutputHandling (bool value);
};
#endif
