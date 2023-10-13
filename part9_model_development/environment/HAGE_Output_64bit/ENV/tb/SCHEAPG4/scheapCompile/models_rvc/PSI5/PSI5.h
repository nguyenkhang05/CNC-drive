// ---------------------------------------------------------------------
// $Id: PSI5.h,v 1.9 2020/04/23 07:12:11 hadinhtran Exp $
//
// Copyright(c) 2018 - 2020 Renesas Electronics Corporation
// Copyright(c) 2018 - 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __PSI5_H__
#define __PSI5_H__
#include "re_define.h"
#include "BusSlaveBase.h"
#include "PSI5_Timer.h"
#include "PSI5_AgentController.h"
#include "global.h"
#include "rvc_common_model.h"

class PSI5_Func;

class PSI5:public sc_module,
           public rvc_common_model,
           public PSI5_AgentController,
           public BusSlaveBase<BUS_WIDTH_APB,1>
{
private:
    enum eResetNumber {//Define number of reset signal
        emPresetn = 0,
        emPsi5_com_rst_n = 1,
        emResetSignalNum = 2
    };
    enum eInterruptId {//Define interrupt port id
        emINT_SYNCED = 0,
        emINT_SYNCST = 1,
        emINT_TXDEMPTY = 2,
        emINT_RXDSCNFERR = 3,
        emINT_RXDERR = 4,
        emINT_RXDFOVF = 5,
        emINT_RXDEXIST = 6,
        emINT_RXMFOVF = 7,
        emINT_RXMERR = 8,
        emINT_RXMEXIST = 9,
        emInterruptNum = 10
    };
    enum eTxDataFormat {//Define tx data format
        emLongFrame = 2,
        emXLongFrame = 3,
        emXXLongFrame = 4 
    };
    enum eTimeSlotNum {//Define time slot number
        emSlot1 = 0,
        emSlot2 = 1,
        emSlot3 = 2,
        emSlot4 = 3,
        emSlot5 = 4,
        emSlot6 = 5,
        emSlot7 = 6,
        emSlot8 = 7,
        emSlotNum = 8
    };
    enum eTimerIndex {//Define timer index
        emSyncPulseTimer = 8,
        emStampTimeCounter = 9
    };

public:
    //Declare input signals
    sc_in<sc_dt::uint64> PCLK;
    sc_in<sc_dt::uint64> psi5_com_clk;
    sc_in<bool> PRESETn;
    sc_in<bool> psi5_com_rst_n;
    sc_in<unsigned int> I_CONTROL;
    sc_in<unsigned int> I_DATA;
    sc_in<sc_dt::uint64> psi5_ts_tick_in;
    sc_in<bool> psi5_ts_clr_in;

    //Declare output signals
    sc_out<bool> V_DATA;
    sc_out<bool> INT_SYNCED;
    sc_out<bool> INT_SYNCST;
    sc_out<bool> INT_TXDEMPTY;
    sc_out<bool> INT_RXDSCNFERR;
    sc_out<bool> INT_RXDERR;
    sc_out<bool> INT_RXDFOVF;
    sc_out<bool> INT_RXDEXIST;
    sc_out<bool> INT_RXMFOVF;
    sc_out<bool> INT_RXMERR;
    sc_out<bool> INT_RXMEXIST;
    sc_out<sc_dt::uint64> psi5_ts_tick_out;
    sc_out<bool> psi5_ts_clr_out;

    //Declare target socket
    TlmTargetSocket<BUS_WIDTH_APB> *ts;
    PSI5_Timer *mSync_Pulse_Timer;
    PSI5_Timer *mStamp_Time_Counter;
    PSI5_Timer *mSlot_Timer[emSlotNum];


    SC_HAS_PROCESS (PSI5);

    PSI5 (sc_module_name name, 
          unsigned int rLatency,
          unsigned int wLatency,
          unsigned short channel_num=0x0);
    ~PSI5 (void);

    //Declare public functions
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const std::string reg_name, const unsigned int reg_value);
    void ReleaseRegister (const std::string reg_name);
    void WriteRegister (const std::string reg_name, const unsigned int reg_value);
    void ReadRegister (const std::string reg_name);
    void ListRegister (void);
    void EnableRegisterMessage(const std::string is_enable);
    void Help (const std::string type);

private:
    //Declare private members
    PSI5_Func *mPSI5_Func;                //PSI5_Func class instance

    sc_dt::uint64 mPCLKPeriod;            //Period value of PCLK clock
    sc_dt::uint64 mPSI5_COM_CLKPeriod;    //Period value of psi5_com_clk clock
    sc_dt::uint64 mPCLKFreq;              //Frequency value of PCLK clock (Hz)
    sc_dt::uint64 mPCLKOrgFreq;           //Original frequency value of PCLK clock
    sc_dt::uint64 mPSI5_COM_CLKFreq;      //Frequency value of psi5_com_clk clock (Hz)
    sc_dt::uint64 mPSI5_COM_CLKOrgFreq;   //Original frequency value of psi5_com_clk clock
    std::string mPCLKFreqUnit;            //Frequency unit of PCLK clock
    std::string mPSI5_COM_CLKFreqUnit;    //Frequency unit of psi5_com_clk clock
    sc_dt::uint64 mTimeResolutionValue;   //Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;     //Time resolution unit of simulation
    double mCounter_period_in_slave_mode;
    double mCounter_period_in_master_mode;    
    double       mStartClkTime[2];        // Start time of clock

    bool mResetCurVal[emResetSignalNum];       //Store current value of PRESETn or psi5_com_rst_n
    bool mResetPreVal[emResetSignalNum];       //Store previous value of PRESETn or psi5_com_rst_n
    bool mIsResetHardActive[emResetSignalNum]; //Reset operation status of PRESETn or psi5_com_rst_n signal
    bool mIsResetCmdReceive[emResetSignalNum]; //Receive status of AssertReset command of PRESETn or psi5_com_rst_n
    bool mIsResetCmdActive[emResetSignalNum];  //Active status of AssertReset command of PRESETn or psi5_com_rst_n
    double mResetCmdPeriod[emResetSignalNum];  //Period of AssertReset command of PRESETn or psi5_com_rst_n

    bool mDumpInterrupt;                       //Enable/Disable dumping interrupt message
    bool mEnableRegisterMessage;              //Enable/Disable dumping Register message
    bool mIsSyncPulseSending;                  //Sync pulse sending status
    sc_dt::uint64 mTxDataFrame;                //Tx data frame
    unsigned char mTxBitTotalNum;              //The number of bits in Tx data
    unsigned char mTxBitPos;                   //The position of Tx bit
    bool mIsSlotReceptionCompleted[emSlotNum]; //Reception status of each slot
    sc_dt::uint64 mCountFreq;                  //The frequency value for counting
    unsigned int mI_CONTROLValue;              //The value of I_CONTROL port
    unsigned int mI_DATAValue;                 //The value of I_DATA port
    bool mPreviousIntValue[emInterruptNum];    //The previous value of interrupt port
    bool mIsActiveInt;                         //Indicate active interrupt port or not
    bool mV_DATAValue;                         //Indicate V_DATA value
    bool mpsi5_ts_clr_outValue;                //Indicate psi5_ts_clr_out value

    sc_event mResetHardEvent[emResetSignalNum];     //Call HandleResetHardMethod when PRESETn or psi5_com_rst_n is active
    sc_event mResetCmdEvent[emResetSignalNum];      //Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emResetSignalNum];//Call CancelResetCmdMethod when AssertReset is over

    sc_event mSyncPulseEvent;         //Call SyncPulseThread to issue a synchronous pulse
    sc_event mTransmissionEvent;      //Call TransmissionThread to send the data frame
    sc_event mProcessReceptionDataEvent[emSlotNum]; //Call ProcessReceptionDataMethod to receive the according to time slot
    sc_event mV_DATAWriteEvent;    //Call WriteV_DATAMethod to control V_DATA port
    sc_event mAssertV_DATAEvent;
    sc_event mDeAssertV_DATAEvent;
    sc_event mpsi5_ts_clr_outWriteEvent; //Call Writepsi5_ts_clr_outMethod to control psi5_ts_clr_out port
    sc_event mAssertpsi5_ts_clr_outEvent;
    sc_event mDeAssertpsi5_ts_clr_outEvent;
    sc_event mpsi5_ts_tick_outWriteEvent;   //Call Writepsi5_ts_tick_outMethod to control psi5_ts_tick_out port
    sc_event mWriteInterruptPortsEvent[emInterruptNum]; //Call WriteInterruptPortsMethod to write interrupt ports
    sc_event mDisableChannelEvent; //Call when CHCTRL.chen is set to 0
    sc_event mStartTimerAfterOVF;

    void HandlePCLKSignalMethod (void);
    void HandlePSI5_COM_CLKSignalMethod (void);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);
    void HandlePRESETnSignalMethod (void);
    void HandlePSI5_COM_RST_NSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleSyncPulseTimerCompareMatchMethod (void);
    void HandleSlotTimerCompareMatchMethod (const unsigned int slot_index);
    void Handlepsi5_ts_tick_inMethod (void);
    void Handlepsi5_ts_clr_inMethod (void);
    void WriteV_DATAMethod (void);
    void AssertV_DATAMethod (void);
    void DeAssertV_DATAMethod (void);
    void Writepsi5_ts_clr_outMethod (void);
    void Assertpsi5_ts_clr_outMethod (void);
    void DeAssertpsi5_ts_clr_outMethod (void);
    void Writepsi5_ts_tick_outMethod (void);
    void WriteInterruptPortsMethod (const unsigned int interrupt_id);
    void SyncPulseThread (void);
    void TransmissionThread (void);
    void ProcessReceptionDataMethod (const unsigned int slot_index);
    void AsyncReceptionThread (void);
    void HandleTimerOvf(void);
    void StartTimerAfterOVF();

    void Initialize (void);
    void CheckFreq (sc_dt::uint64 &freq_out
                   ,std::string &unit_out
                   ,sc_dt::uint64 freq_in
                   ,std::string unit_in);

    double CalculateCLKPosEdge (const std::string clock_name, const double active_period);

    void EnableReset (const bool is_active, const unsigned int reset_id);
    void PrepareTxDataFrame (const unsigned int data_format
                            ,const unsigned int data);
    void StartSlotTimer (void);

    //Declare virtual functions of PSI5_AgentController class
    bool GetResetStatus (void);
    bool GetPresetnStatus (void);
    bool CheckClockPeriod (const std::string clock_name);
    sc_dt::uint64 GetCommunicationPeriod (void);
    void SendSyncPulse (const bool is_sync_mode
                       ,const unsigned int sync_pulse_default_value=0);
    void StartStopTimer (const unsigned char timer_type
                        ,const bool is_start);
    void SetTimerCompareMatchValue (const unsigned char timer_type
                                   ,const unsigned int value);
    void SetTimerCounterIntialValue (const unsigned char timer_type
                                    ,const unsigned int value);
    void SetTimerCounterPeriod (const unsigned char timer_type
                               ,const double counter_period);
    void SetTimerCounterMode (const unsigned char timer_type
                             ,const bool is_freerun);
    unsigned int GetStampTimeCounterValue ();
    void ControlInterruptPorts (const unsigned int interrupt_id
                               ,const bool is_active);
    void SendData (const unsigned int data_formati
                  ,const unsigned int data);
    void Writepsi5_ts_tick_outPort (const unsigned int bitrate_value, const bool is_clear);
    void Sendpsi5_ts_clr_outPort (void);
    void EnableSoftwareReset (const bool is_active);
    void SetCounter_period_in_master_mode(double period);
};
#endif
