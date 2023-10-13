// ---------------------------------------------------------------------
// $Id: HSIF020.h,v 1.7 2020/04/07 03:56:04 quynhnhunguyen Exp $
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

#ifndef __HSIF020_H__
#define __HSIF020_H__

#include "BusSlaveBase.h"
#include "BusMasterBase.h"
#include "HSIF_Common.h"
#include "HSIF020_AgentController.h"
#include "rvc_common_model.h"

class HSIF020_Func;

class HSIF020: public sc_module,
               public HSIF_ultis,
               public HSIF020_AgentController,
               public BusSlaveBase<64,1>,
               public BusMasterBase<64,1>,
               public rvc_common_model
{
friend class HSIF020_UNIT;
friend class HSIF020_UNIT_Func;

private:
    HSIF020_Func        *mHSIF020_Func;        ///<HSIF020_Func class instance

    /// Declare Clock/Reset area
    sc_dt::uint64       mACLKPeriod;           ///<Period value of ACLK clock
    sc_dt::uint64       mACLKFreq;             ///<Frequency value of ACLK clock
    sc_dt::uint64       mACLKOrgFreq;          ///<Original frequency value of ACLK clock
    std::string         mACLKFreqUnit;         ///<Frequency unit of ACLK clock
    sc_dt::uint64       mCCLKPeriod;           ///<Period value of CCLK clock
    sc_dt::uint64       mCCLKFreq;             ///<Frequency value of CCLK clock
    sc_dt::uint64       mCCLKOrgFreq;          ///<Original frequency value of CCLK clock
    std::string         mCCLKFreqUnit;         ///<Frequency unit of CCLK clock
    double              mCCLKSyncTPoint;       ///<The time point at which active edge of clock happens
    double              mACLKSyncTPoint;       ///<The time point at which active edge of clock happens
    
    sc_dt::uint64       mTimeResolutionValue;  ///<Time resolution value of simulation
    sc_time_unit        mTimeResolutionUnit;   ///<Time resolution unit of simulation

    bool                mResetCurVal[emResetSignalNum];        ///<Store current value of ARESETn or creset_n
    bool                mResetPreVal[emResetSignalNum];        ///<Store previous value of ARESETn or creset_n
    bool                mIsResetHardActive[emResetSignalNum];  ///<Reset operation status of ARESETn or creset_n signal
    bool                mIsResetCmdReceive[emResetSignalNum];  ///<Receive status of AssertReset command of ARESETn or creset_n
    bool                mIsResetCmdActive[emResetSignalNum];   ///<Active status of AssertReset command of ARESETn or creset_n
    double              mResetCmdPeriod[emResetSignalNum];     ///<Period of AssertReset command of ARESETn or creset_n

    sc_event            mResetHardEvent[emResetSignalNum];     ///<Call HandleResetHardMethod when ARESETn or creset_n is active
    sc_event            mResetCmdEvent[emResetSignalNum];      ///<Call HandleResetCmdMethod when AssertReset is called
    sc_event            mResetCmdCancelEvent[emResetSignalNum];///<Call CancelResetCmdMethod when AssertReset is over

    sc_event            mClkZeroEvent[emClkNum];               ///<Notify when clock is zero

    bool                mDumpInterrupt;                        ///<Enable/Disable dumping the interrupt info
    bool                mEnableRegisterMessage;

    /// Other attribute of HSIF020
    std::set<ChId_t>    ChannelArbitterL2;                      ///< Implemented of Arbiter for channel
    uint32_t            mTxCtrlVal;                             ///< Tx control signal value
    L2Frame             mL2FrameOut;                            ///< Tx L2 Frame out value

    bool                mIsTxFree;                              ///<Tx is free (L2 -> L1)
    bool                mTxFrameCmp;                            ///<Tx Frame status: complete or not
    bool                mTxDataReady;                           ///<Tx channel is ready or not (depended on L1 status)
    bool                mIsInitialSetting;                      ///<Model is initial setting or not
    bool                mIsDurTransStreamCmd;                   ///< Model is during transfer a stream command or not

    bool                mAuthenIdBaseEnable;                    ///< Authentication ID base is enable or not
    bool                mAuthenCREnable;                        ///< Authentication CR is enable or not
    bool                mAuthenIdBasePass;                      ///< Authentication ID base is Passed or not
    bool                mAuthenCRPass;                          ///< AUthentication CR is Passed or not
    bool                mAuthenIdBaseLock;                      ///< Authentication ID base is locked or not
    bool                mAuthenCRLock;                          ///< Authentication CR is locked or not

    bool                mInt_hsif_ch[emCHNum];                  ///< Current status of int_hsif_ch[] signal
    bool                mInt_hsif_str;                          ///< Current status of mInt_hsif_str signal
    bool                mInt_hsif_err;                          ///< Current status of mInt_hsif_err signal
    bool                mInt_hsif_sec;                          ///< Current status of mInt_hsif_sec signal

    uint32_t            mJtagId;                ///< ID of Jtag
    uint8_t             mThisIP_SPID;           ///< SPID of RHSIF module (This IP)
    uint8_t             mPEID;                  ///< PEID of RHSIF module (This IP)
    uint8_t             mFrameId;               ///< ID of frame which is transferring

    ChId_t              mCurrentChannelId;      ///< Current channel Id which granted permission to access the transmission line
    uint8_t             mTxStreamOutstand;      ///< Number of out standing stream command

    bool                mIsCrcError;            ///< Is CRC error or not
    bool                mIsTransIdError;        ///< Is TID error or not
    bool                mIsAxiError;            ///< Is AXI error or not

    std::set<uint8_t>    mTxFrameStreamCmdInfo; ///< Save frame id of output stream command to compare with received ACK/NACK command
public:
    /// Declare input/ouput signals
    sc_in<sc_dt::uint64>    ACLK;               ///< AXI bus clock
    sc_in<sc_dt::uint64>    CCLK;               ///< Communication clock
    sc_in<bool>             ARESETn;            ///< AXI reset signal
    sc_in<bool>             creset_n;           ///< Communication reset signal

    /// Communication from L2 to L1
    sc_out<L2Frame>         hsif_tx_data;       ///< Tx data output signal
    sc_out<uint32_t>        hsif_tx_control;    ///< Tx control output signal
    sc_in<bool>             hsif_tx_data_ready; ///< Tx data ready input signal
    sc_in<bool>             hsif_tx_frame_cmp;  ///< Tx frame transfer input complete signal

    //Communication from L1 to L2
    sc_in<L2Frame>          hsif_rx_data;       ///< Rx data input signal
    sc_in<uint32_t>         hsif_rx_control;    ///< Rx control input signal

    ///Authentication pass signal output
    sc_out<bool>            hsif_authent1_pass; ///< Authentication 1 pass signal
    sc_out<bool>            hsif_authent2_pass; ///< Authentication 2 pass sigal

    /// Interrupt signal
    sc_out<bool>            *int_hsif_ch[emCHNum];  ///< Interrupt channel [0, 1, 2, 3] signal
    sc_out<bool>            int_hsif_str;           ///< Stream Interrupt signal
    sc_out<bool>            int_hsif_err;           ///< Error interrupt signal
    sc_out<bool>            int_hsif_sec;           ///< Security interrupt signal

    /// Socket declaration
    TlmInitiatorSocket<64>  *am_is;            ///<For HSIF020's registers
    TlmTargetSocket<64>     *as_ts;            ///<For HSIF020's registers

    SC_HAS_PROCESS (HSIF020);
    HSIF020 (   sc_module_name name
               ,uint32_t rLatency
               ,uint32_t wLatency
               ,SIM_MODE_T simmode);
    ~HSIF020 (void);

    /// Function for PythonIF
    void AssertReset (const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq (const std::string clock_name, const sc_dt::uint64  freq, const std::string unit);
    void GetCLKFreq (const std::string clock_name);
    void ForceRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name, const uint32_t reg_value);
    void ReleaseRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name);
    void WriteRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name, const uint32_t reg_value);
    void ReadRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name);
    void ListRegister (void);
    void Help (const std::string type);
    void SetMessageLevel (const std::string msg_lv);
    void DumpRegisterRW (const std::string is_enable);
    void DumpInterrupt (const std::string is_enable);
    void DumpStatusInfo(void);
    void SetBusMasterID(uint32_t thisIp_peid);   /// Set This model ID and ICU model ID
    void SetModelID(uint32_t thisIp_spid); 
    void SetAuthenIdBaseEnable(const std::string isEnable       ///< Set authentication id base enable/disable
                       , uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3
                       , uint32_t id4, uint32_t id5, uint32_t id6, uint32_t id7);
    void SetAuthenCREnable(const std::string isEnable);         ///< Set authentication CR base enable/disable
    void ConfigTransaction(const bool icommand                  ///  Configure for AXI transaction
                          ,const unsigned int iaddr
                          ,const unsigned char isize
                          ,const bool idebug
                          ,const ChId_t ch_id);
    void SetJtagID(const uint32_t Jtag_id);             ///< Set Jtag ID for this IP
    uint32_t GetJtagID(void);                           ///  Get Jtag ID of this IP
    void IssueError(const std::string error_code);      ///  Issue an error to model (CRC, TID, AXI)
    void EnableRegisterMessage(const std::string is_enable);
private:
    /// Declare Internal event
    sc_event    mStartHandleReceivedL1toL2Event;
    sc_event    mFinishHandleReceivedL1toL2Event;

    sc_event    mIssueCmdEvent;
    sc_event    mWriteTxPortEvent;
    sc_event    mClearTxControlSignalEvent;
    sc_event    mTxNonStreamTimeOutEvent[emCHNum];
    sc_event    mTxStreamTimeOutEvent;
    sc_event    mNotifyAxiTransferTransactionEvent;

    ///Interrupt write event
    sc_event    mWriteInt_hsif_chEvent[emCHNum];
    sc_event    mAssertInt_hsif_chEvent[emCHNum];
    sc_event    mDeAssertInt_hsif_chEvent[emCHNum];

    sc_event    mWriteInt_hsif_strEvent;
    sc_event    mAssertInt_hsif_strEvent;
    sc_event    mDeAssertInt_hsif_strEvent;

    sc_event    mWriteInt_hsif_errEvent;
    sc_event    mAssertInt_hsif_errEvent;
    sc_event    mDeAssertInt_hsif_errEvent;

    sc_event    mWriteInt_hsif_secEvent;
    sc_event    mAssertInt_hsif_secEvent;
    sc_event    mDeAssertInt_hsif_secEvent;

    sc_event    mWriteHsif_authent1_passEvent;
    sc_event    mWriteHsif_authent2_passEvent;

    sc_event    mTxStreamGetData2FifoEvent;
    sc_event    mRxStreamWriteData2MemEvent;
    sc_event    mTxTransferStreamCmdEvent;
private:
    /// Declare operation method/thread
    /// Communication L1 to L2
    void HandleL1toL2ControlSignalMethod(void);     /// Handle control signal (hsif_rx_control) for L1 to L2
    void StartHandleReceivedL1toL2Method(void);     /// Start Handle Received L1 to L2
    void FinishHandleReceivedL1toL2Method(void);    /// Finish Handle Received L1 to L2
    void IssueCmdMethod(void);                      /// Method to start serve command
    void WriteTxPortMethod(void);                   /// Write Tx port method
    void HandleL1CompletePortMethod(void);          /// Handle L1 Complete Port. When complete port == 1, L1 is finished frame from L2
    void HandleL1ReadyPortMethod(void);             /// Method to handle Ready signal from L1. If read == 1, L1 can be served command
    void ClearTxControlSignalMethod(void);          /// Clear Tx control signal. Finish transfer a command
    void HandleTimeOutNonStreamMethod(ChId_t ch_id);/// Handle Time out occur (Non-stream operation)
    void HandleTimeOutStreamMethod();               /// Handle Time out occur (Stream operation)
    void TransProcessThread(void);                  /// Issue AXI Transaction Process
    void NotifyCmdtoL2Arbiter(ChId_t ch_id);        /// Notify event to put current request to Arbiter (Arbiter is implemented by a Set)
    void IgnoreReceiveFrame(void);                  /// Ignore Received Frame
    void FrammingProcess(ChId_t ch_id);             /// Served command will be Framed and notify write output event

    void WriteInt_hsif_chMethod(ChId_t ch_id);      ///Method to handle interrupt
    void AssertInt_hsif_chMethod(ChId_t ch_id);
    void DeAssertInt_hsif_chMethod(ChId_t ch_id);

    void WriteInt_hsif_strMethod(void);
    void AssertInt_hsif_strMethod(void);
    void DeAssertInt_hsif_strMethod(void);

    void WriteInt_hsif_errMethod(void);
    void AssertInt_hsif_errMethod(void);
    void DeAssertInt_hsif_errMethod(void);

    void WriteInt_hsif_secMethod(void);
    void AssertInt_hsif_secMethod(void);
    void DeAssertInt_hsif_secMethod(void);

    void WriteHsif_authent1_passMethod(void);       /// Method to handle authentication pass port
    void WriteHsif_authent2_passMethod(void);

    /// Declare Clock/Reset methods
    void HandleACLKSignalMethod (void);             /// Handle ACLK signal
    void HandleCCLKSignalMethod (void);             /// Handle CCLK signal
    void HandleARESETnSignalMethod (void);          /// Handle ARESETn signal
    void Handlecreset_nSignalMethod (void);         /// Handle creset_n signal
    void HandleResetHardMethod (const uint32_t reset_id);   /// Process reset function when reset port is active
    void HandleResetCmdMethod (const uint32_t reset_id);    /// Process reset function when reset command is active
    void CancelResetCmdMethod (const uint32_t reset_id);    /// Cancel reset function when reset command is active

    ///Stream command
    void    TxStreamGetData2FifoMethod(void);       /// Tx Get data to FiFO for start transfer to Rx side
    void    RxStreamWriteData2MemMethod(void);      /// Rx write data to memory when received data from Tx
    void    TxTransferStreamCmdMethod(void);        /// Issue Request to Arbiter transfer stream command method
    void    TxStreamNotifyGetData2Fifo(void);       /// Notify Get data from memory to Fifo
    void    RxStreamNotifyWriteData2Mem(void);      /// Notify Rx Write data to memory
    void    TxContinueGetData2Fifo(void);           /// Notify method to continue get data from memory to FIFO
    void    RxContinueWriteData2Mem(void);          /// Notify method to continue Write data to memory
    void    NotifyTxTransStreamCmd(void);           /// Notify to start transfer a stream command
    uint8_t GetTxStreamOutstandingNumb(void);       /// Get number of outstanding stream command

    /// Internal functions
    void     Initialize(void);                      /// Initialize internal variables
    void     EnableReset(const std::string reset_name, const bool is_active);   /// Execute reset operation
    void     CancelEventsReset(void);               /// Cancel operation events
    void     CancelEventsZeroClock(void);           /// Cancel operation events
    void     RegisterHandlerWrapper( const bool is_com_reg       /// Wrapper for accessing registers
                                      , const uint32_t ch_id
                                      , const std::vector<std::string> cmd);
    void     SetInitialSettingPhase(bool isInitialSettingPhase); /// Set model to initial setting phase

    ///Private functions
    sc_time  CalculateClockEdge (const std::string clock_name);  /// Overwrite CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point)
    uint32_t ComputeTxControl(uint32_t payloadSizeIndex          /// Compute Tx control value from each element
                            , uint32_t stbe
                            , uint32_t channelType);
    uint32_t DecodeChNumbFromChId (uint32_t ch_id);              /// Decode Channel number from Channel id (with information in register (HSIFnMMD)
    bool     DecodeLogicChType2ChId(uint8_t iLogicChType, ChId_t& ch_id);   /// Decode channel ID from logic channel type (data from input control signal)
    uint32_t DecodeChType4TxControl(uint8_t ch_id);              /// Decode Channel type for Tx control signal output
    uint8_t  IncTransId(void);                                   /// Handle Transaction ID (increase in each frame)
    uint32_t GetTimeOutSetting(void);                            /// Get configure of time-out register
    void     IssueInterrupt(std::string interrupt_name, ChId_t ch_id, bool type);     ///Method to handle interrupt
    void     DumpInterruptMsg(const std::string intr_name, const bool value);    /// Dump Interrupt Msg
    uint16_t ComputeCRC(L2Frame& iL2Frame, uint8_t payloadSize); /// Function to compute CRC for each frame
    uint16_t crc16cccit(uint8_t *data, uint8_t sizeInByte);      /// CRC16 CCCIT algorithm

    /// Virtual functions of HSIF020_AgentController class
    bool    CheckClockPeriod (const std::string clock_name);    /// Check clock period value
    bool    CheckResetStatus ();    /// Check clock period value
    bool    isInitialSetting(void);                             /// check model is in initial setting phase or not
    bool    isDurTransStreamCmd(void);                          /// Check model is in during transfer stream command or not
    bool    IsHitMemWindow(uint32_t startAddr, uint32_t endAddr, bool isWrite); /// Check a read/write request is hit memory window or not
    bool    IsPassedAuthent(void);                              /// Check current status is pass authentication or not
    void    NotifyAxiTransferTransaction(void);                 /// Notify start transfer AXI transaction to bus
    void    SetAuthenIdStatus(bool isPass);                     /// Set authentication id base status
    void    SetAuthenCRStatus(bool isPass);                     /// Set authentication cr base status
    uint8_t GetThisIpSpidInfo(void);                            ///Get this IP Spid value
    void    SetAuthenIdLock(void);                              /// Set authentication ID base lock
    void    SetAuthenCRLock(void);                              /// Set authentication CR lock
    bool    IsAxiError(void);                                   /// Check is AXI error or not
    void    TxStreamStopTimeOutEvent(void);                     /// Stop time-out event when received reply command
    void    TxNonStreamStopTimeOutEvent(uint8_t ch_id);         /// Stop time-out event when received reply command
};
#endif
