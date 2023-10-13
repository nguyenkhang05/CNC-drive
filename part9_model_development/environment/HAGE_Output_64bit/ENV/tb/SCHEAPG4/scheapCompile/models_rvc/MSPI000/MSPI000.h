//---------------------------------------------------------------------
// $Id: MSPI000.h,v 1.15 2020/03/11 00:52:43 ducla Exp $
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
#ifndef __MSPI000_H__
#define __MSPI000_H__

#include "re_define.h"
#include "BusSlaveBase.h"
#include "MSPI000_AgentController.h"
#include "global.h"
#include "Common.h"
#include "rvc_common_model.h"

#define MAX_CH_NUM 16
#define MAX_CS_NUM 15
#define MAX_HWTRIGGER_NUM 63

class MSPI000_Func;
class MSPI000_INT_FACTOR_Func;
class MSPI000: public sc_module
            ,public rvc_common_model
            ,public MSPI000_AgentController
            ,public BusSlaveBase<32, 2>  // number of bus slave
{
    unsigned int  mChannelNum;  // number of channel in model
    unsigned int  mChipselectNum;  // number of chip select in model
    unsigned int  mHWTriggerNum;  // number of hardware trigger in model
public:
    // Clock/Reset
    sc_in<sc_dt::uint64> PCLK;                                        // clock for APB
    sc_in<sc_dt::uint64> MSPInCLK;                                    // communication clock
    sc_in<bool> PRESETn;                                              // reset of domain PCLK
    sc_in<bool> MSPInRESB;                                            // reset of domain MSPInCLK
    // Input - Output
    //SPI
    sc_out<sc_dt::uint64>   MSPInSCK[emNumDuplicatePort];             // Communication clock output emNumDuplicatePort = 4
    sc_in<sc_dt::uint64>    MSPInSCKI[emNumDuplicatePort];            // Communication clock input slave mode emNumDuplicatePort = 4
    sc_out<bool>            **MSPInCS;                     // Chip select output signal emNumCSPort = 15
    sc_in<bool>             MSPInCSI;                                 // Chip select input signal slave mode
    sc_out<uint32_t>        MOSI[emNumDuplicatePort];                 // SPI data out emNumDuplicatePort = 4
    sc_out<bool>            MSPInSOUTDRV;                             // Data output enable
    sc_in<uint32_t>         MISO[emNumDuplicatePort];                 // SPI data in emNumDuplicatePort = 4
    sc_in<uint32_t>         MSPInDCSI;                                // Data consistency check signal
    sc_out<bool>            MSPInDCSIDRV;                             // Data consistency check signal input enable
    sc_out<bool>            MSPInMD;                                  // Master/Slave signal
    sc_inout<uint32_t>      SPI_CONTROL;                              // Signal control transmit and receive of SPI
    //HW trigger
    sc_in<bool>             **MSPInHWTRG;                             // HW start trigger input // ducla revise
    //Interrupt output
    sc_out<bool>            **INTMSPInTXm;                             // Communication data transmit interrupt. Number of port is MSPI_CH_NUM
    sc_out<bool>            **INTMSPInRXm;                             // Communication data receive interrupt. Number of port is MSPI_CH_NUM
    sc_out<bool>            **INTMSPInFEm;                             // Communication frame count end interrupt. Number of port is MSPI_CH_NUM
    sc_out<bool>            **INTMSPInERRm;                            // Communication error interrupt. Number of port is MSPI_CH_NUM
    
    sc_out<bool>            INTMSPInTX;                             // Communication data transmit interrupt.
    sc_out<bool>            INTMSPInRX;                             // Communication data receive interrupt. 
    sc_out<bool>            INTMSPInFE;                             // Communication frame count end interrupt.
    sc_out<bool>            INTMSPInERR;                            // Communication error interrupt.
    

    // TargetSocket
    TlmTargetSocket<32>     *ts; // target socket for MSPI000's registers and memory.
    TlmTargetSocket<32>     *tsif; // target socket for INT_FACTOR's registers and memory.

    SC_HAS_PROCESS (MSPI000);
    MSPI000 (sc_module_name name, const unsigned int rLatency = 0, const unsigned int wLatency = 0, const char* pCfgFileName = NULL);
    ~MSPI000 ();

    // PythonIF
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
    void Help (const std::string type);
    void EnableRegisterMessage(const std::string is_enable);

    void WriteDataMem (const unsigned int offset, const unsigned int value, const unsigned int size);
    void ReadDataMem (const unsigned int offset, const unsigned int size);
    void SetMemSize(const unsigned int id);
    void SetError (const std::string name, const unsigned int ch_num, const std::string is_err);
    void SetComparisonTiming(float value);
    void readCfgFile(const char *filename);
    
private:
    // Func
    MSPI000_Func     *mpMSPI000_Func;  // Pointer of MSPI000 Func
    MSPI000_INT_FACTOR_Func     *mpMSPI000_INT_FACTOR_Func;  // Pointer of MSPI000 Func
    // Enums
    enum eClockInfo{//Define number of clock signal
        emPCLKId                = 0
        ,emMSPInCLKId           = 1
        ,emNumClk               = 2
    };
    enum eResetInfo{//Define number of reset signal
        emPRESETnId             = 0
        ,emMSPInRESBId          = 1
        ,emNumReset             = 2
    };
    enum ePriorityInfo{
        emLowestPriority = 8
    };
    // Variables
    sc_dt::uint64   mClkPeriod[emNumClk];           // Period of clock
    sc_dt::uint64   mClkOrgFreq[emNumClk];          // Previous frequency of clock
    sc_dt::uint64   mClkFreq[emNumClk];             // Frequency of clock
    std::string     mClkFreqUnit[emNumClk];         // Frequency unit of clock
    double          mStartClkTime[emNumClk];        // Start time of clock

    bool            mResetCurVal[emNumReset];       // Store current value of reset signals
    bool            mResetPreVal[emNumReset];       // Store previous value of reset signals
    bool            mIsResetHardActive[emNumReset]; // Is reset by port
    bool            mIsResetCmdReceive[emNumReset]; // Is receive cmd reset
    bool            mIsResetCmdActive[emNumReset];  // Is reset by cmd
    double          mResetCmdPeriod[emNumReset];    // Period of cmd reset
    bool            mEnableRegisterMessage;         // Enable/disable info/warning/error message of register IF

    sc_dt::uint64   mTimeResolutionValue;           // Time resolution value of simulation
    sc_time_unit    mTimeResolutionUnit;            // Time resolution unit of simulation

    bool            mDumpInterrupt;                 // Indicate dump info of interrupt or not

    bool mINTMSPInTX[MAX_CH_NUM];                   // The value of INTMSPInTX port
    bool mINTMSPInRX[MAX_CH_NUM];                   // The value of INTMSPInRX port
    bool mINTMSPInFE[MAX_CH_NUM];                   // The value of INTMSPInFE port
    bool mINTMSPInERR[MAX_CH_NUM];                  // The value of INTMSPInERR port

    bool mParityError[MAX_CH_NUM];                  // Indicate Parity Error status for each channel
    bool mDataConsistencyError[MAX_CH_NUM];         // Indicate Data Consistency Error status for each channel
    bool mOverWriteError[MAX_CH_NUM];               // Indicate Over Write Error status for each channel
    bool mOverReadError[MAX_CH_NUM];                // Indicate Over Read Error status for each channel
    bool mOverRunError[MAX_CH_NUM];                 // Indicate Over Run Error status for each channel
    bool mCRCError[MAX_CH_NUM];                     // Indicate CRC Error status for each channel

    struct Data_st mTX_Reserve_Buffer;              // Struct store TX Reserve Buffer value (data, chid, request, csr, joben)
    struct Data_st mTX_Shift_Register;              // Struct store TX Shift Register value (data, chid, request, csr, joben)
    struct Data_st mRX_Reserve_Buffer;              // Struct store RX Reserve Buffer value (data, chid, request, csr, joben)
    struct Data_st mRX_Shift_Register;              // Struct store RX Shift Register value (data, chid, request, csr, joben)
    bool mTX_Reserve_Buffer_Status;                 // Indicate status of TX Reserve Buffer
  

    unsigned int mCurrentSTBE;                      // Store value of STBE from SPI_CONTROL port.
    double mMSPInSCKI;                              // The value of SCKI port
    bool mCSI;                                      // The value of CSI port
    unsigned int mReserve_chid;                     // Indicate the channel id was written to TX Reserve Buffer
    unsigned int mRx_chid;                          // Indicate the channel id for receive frame
    unsigned int mCom_chid;                         // Indicate current channel id communication
    sc_dt::uint64 mHWTRGVal;                        // Store value of HW trigger
    unsigned int mDCSI;                             // store value of DCSI
    bool mMSPInCS[emNumCSPort];                     // The value of MSPInCS port
    bool mMSPInCS_prev[emNumCSPort];                // The value of MSPInCS port prev
    double mMSPInSCKPeriod;                         // The value of SCK Period port
    double mMSPInSCK;                               // The value of SCK port
    unsigned int mMOSI;                             // The value of MOSI port
    unsigned int mSPICONTROL;                       // The value of SPI_CONTROL port

    eOperationMode mMode;                           // Master or slave mode
    eCommunicationMode mCMode[MAX_CH_NUM];          // Transmission and Reception mode or Transmission only mode or Reception only mode
    eMemoryMode mMMode[MAX_CH_NUM];                 // Direct memory mode or Fixed Buffer memory mode or Fixed FIFO memory mode
    eIPState mIPState;                              // Reset or idle or operation state.
    eChState mChState[MAX_CH_NUM];                  // Init or ready state for each channel
    eModelState mModelState;                        // Init or ready state of IP

    bool mIsLoopBack;                                 // Has loop back mode or not
    bool mIsDataConsistencyCheck;                     // Has data consistency check or not
    bool mIsSafeSPI[MAX_CH_NUM];                      // Has Safe SPI function or not
    unsigned int mSizes[MAX_CH_NUM];                  // Indicate number of byte to store data frame
    bool mIsClearCHEN[MAX_CH_NUM];                    // Has Clear Channel enable or not
    unsigned int mDelay_value[emNumDelay];           // Store value of delays (setup time, hold time, idle time, inter date time)
    unsigned int mCount_com[MAX_CH_NUM];              // count number of communication for each channel
    unsigned int mCount_reserve[MAX_CH_NUM];          // count number of reserve communication for each channel
    unsigned int mHasRequest[MAX_CH_NUM];            // Indicate request number for each channel
    unsigned int mRequestNumber[MAX_CH_NUM];         // Indicate request number for each channel
    bool mFrameEnd;                                  // Indicate frame end or not
    unsigned int mInteral_Frame_Limit[MAX_CH_NUM];   // Indicate number of 32-bit frame
    bool mSlave_enable;                               // Indicate slave mode starting communication
    bool mIsLockFunction;                            // Indicate channel is Lock Function
    bool mIsJobEnable;                               // Indicate channel has Job Enable
    float mComparisonTiming;                         // Comparision Timminng

    // Events
    sc_event mClkZeroEvent[emNumClk];                // Notify clock is zero
    sc_event mResetHardEvent[emNumReset];            // Call HandleResetHardMethod when reset is active
    sc_event mResetCmdEvent[emNumReset];             // Call HandleResetCmdMethod when AssertReset is called
    sc_event mResetCmdCancelEvent[emNumReset];       // Event to cancel reset by command
    sc_event mTransmitEvent;                         // Call TransmitMethod to transmit frame
    sc_event mReceiveEvent;                          // Call ReceiveMethod to receive frame
    sc_event mCommunicationEvent;                    // Call CommunicationThread to start communication transmit and receive frame
    sc_event mCommunicationSlaveEvent;               // Call CommunicationSlaveThread to start communication transmit and receive frame in slave mode
    sc_event mEndFrameEvent;                         // Call EndFrameThread to end communication frame
    sc_event mEndFrameSlaveEvent;                    // Call EndFrameSlaveThread to end communication frame in slave mode
    sc_event mWriteTXReserveBufferEvent;             // Call WriteTXReserveBufferMethod to write data to TX reserve buffer
    sc_event mWriteTXReserveBufferEvent1;            // Call WriteTXReserveBufferMethod to write data to TX reserve buffer in fixed buffer memory mode
    sc_event mWriteTXShiftRegisterEvent;             // Call WriteTXShiftRegisterMethod to write data to TX shift register
    sc_event mWriteRXReserveBufferEvent;             // Call WriteRXReserveBufferMethod to write data to RX reserve buffer
    sc_event mWriteRXShiftRegisterEvent;             // Call WriteRXShiftRegisterMethod to write data to RX shift register
    sc_event mWriteMSPInRXDAm0Event;                 // Call WriteMSPInRXDAm0Method to write data to MSPInRXDAm0 or RAM
    sc_event mWriteINTMSPInTXmEvent[MAX_CH_NUM];      // Call WriteINTMSPInTXmMethod to write INTMSPInTX port
    sc_event mAssertINTMSPInTXmEvent[MAX_CH_NUM];     // Call AssertINTMSPInTXmMethod to write INTMSPInTX port
    sc_event mDeAssertINTMSPInTXmEvent[MAX_CH_NUM];   // Call DeAssertINTMSPInTXmMethod to write INTMSPInTX port
    sc_event mWriteINTMSPInRXmEvent[MAX_CH_NUM];      // Call WriteINTMSPInRXmMethod to write INTMSPInRX port
    sc_event mAssertINTMSPInRXmEvent[MAX_CH_NUM];     // Call AssertINTMSPInRXmMethod to write INTMSPInRX port
    sc_event mDeAssertINTMSPInRXmEvent[MAX_CH_NUM];   // Call DeAssertINTMSPInRXmMethod to write INTMSPInRX port
    sc_event mWriteINTMSPInFEmEvent[MAX_CH_NUM];      // Call WriteINTMSPInFEmMethod to write INTMSPInFE port
    sc_event mAssertINTMSPInFEmEvent[MAX_CH_NUM];     // Call AssertINTMSPInFEmMethod to write INTMSPInFE port
    sc_event mDeAssertINTMSPInFEmEvent[MAX_CH_NUM];   // Call DeAssertINTMSPInFEmMethod to write INTMSPInFE port
    sc_event mWriteINTMSPInERRmEvent[MAX_CH_NUM];     // Call WriteINTMSPInERRmMethod to write INTMSPInERR port
    sc_event mAssertINTMSPInERRmEvent[MAX_CH_NUM];    // Call AssertINTMSPInERRmMethod to write INTMSPInERR port
    sc_event mDeAssertINTMSPInERRmEvent[MAX_CH_NUM];  // Call DeAssertINTMSPInERRmMethod to write INTMSPInERR port
    sc_event mEndChannelEvent;                       // Call EndChannelMethod to end communication of channel
    sc_event mEndChannelSlaveEvent;                  // Call EndChannelSlaveMethod to end communication of channel in slave mode
    sc_event mWriteMSPInSCKEvent;                    // Call WriteMSPInSCKMethod to write MSPInSCK port
    sc_event mWriteMOSIEvent;                        // Call WriteMOSIMethod to write MOSI port
    sc_event mWriteSPICONTROLEvent;                  // Call WriteSPICONTROLMethod to write SPI_CONTROL port
    sc_event mWriteMSPInCSEvent;                     // Call WriteMSPInCSMethod to write MSPInCS port
    sc_event mReadMSPInRXDAm0Event;                  // Wait this event to start communication when the MSPInRXDAm0 is not read
    sc_event mCheckDataconsistencyEvent;             // Call CheckDataconsistency Method
    sc_event mWriteINTMSPInTXEvent;                 //Call WriteINTMSPInTXcEvent to write port INTMSPInTXc
    sc_event mWriteINTMSPInRXEvent;                 //Call WriteINTMSPInRXcEvent to write port INTMSPInRXc
    sc_event mWriteINTMSPInFEEvent;                 //Call WriteINTMSPInFEcEvent to write port INTMSPInFEc
    sc_event mWriteINTMSPInERREvent;                //Call WriteINTMSPInERRcEvent to write port INTMSPInERRc
    //Threads
    void CommunicationThread(void);
    void CommunicationSlaveThread(void);
    void EndFrameThread(void);
    void EndFrameSlaveThread(void);
    //Methods
    //Interrupts methods
    void WriteINTMSPInTXmMethod(unsigned int ch_id);      ///Method to handle interrupt
    void AssertINTMSPInTXmMethod(unsigned int ch_id);
    void DeAssertINTMSPInTXmMethod(unsigned int ch_id);
    void WriteINTMSPInRXmMethod(unsigned int ch_id);      ///Method to handle interrupt
    void AssertINTMSPInRXmMethod(unsigned int ch_id);
    void DeAssertINTMSPInRXmMethod(unsigned int ch_id);
    void WriteINTMSPInFEmMethod(unsigned int ch_id);      ///Method to handle interrupt
    void AssertINTMSPInFEmMethod(unsigned int ch_id);
    void DeAssertINTMSPInFEmMethod(unsigned int ch_id);
    void WriteINTMSPInERRmMethod(unsigned int ch_id);      ///Method to handle interrupt
    void AssertINTMSPInERRmMethod(unsigned int ch_id);
    void DeAssertINTMSPInERRmMethod(unsigned int ch_id);
    // Communication method
    void TransmitMethod(void);
    void ReceiveMethod(void);
    void WriteTXReserveBufferMethod();
    void WriteTXShiftRegisterMethod();
    void WriteRXReserveBufferMethod();
    void WriteRXShiftRegisterMethod();
    void WriteMSPInRXDAm0Method();
    void EndChannelMethod(void);
    void EndChannelSlaveMethod(void);
    void CheckDataconsistencyMethod(void);
    // Clock handle
    void HandlePCLKSignalMethod (void);
    void HandleMSPInCLKSignalMethod (void);
    //Input handle method
    void HandleMSPInSCKISignalMethod       (void);
    void HandleMSPInCSISignalMethod   (void);
    void HandleMSPInDCSISignalMethod  (void);
    void HandleSPI_CONTROLSignalMethod(void);
    void HandleMSPInHWTRGSignalMethod (void);
    void WriteMSPInSCKMethod(void);
    void WriteMOSIMethod(void);
    void WriteSPICONTROLMethod(void);
    void WriteMSPInCSMethod(void);
    //ducla add
    void WriteINTMSPInTXMethod(void);
    void WriteINTMSPInRXMethod(void);
    void WriteINTMSPInFEMethod(void);
    void WriteINTMSPInERRMethod(void);
    // Reset handle
    void HandlePRESETnSignalMethod (void);
    void HandleMSPInRESBSignalMethod (void);
    void HandleResetHardMethod (const unsigned int reset_id);
    void HandleResetCmdMethod (const unsigned int reset_id);
    void CancelResetCmdMethod (const unsigned int reset_id);

    // Internal Functions
    void Initialize (void);
    void EnableReset (const std::string reset_name, const bool is_active);
    void CancelEvents();
    void DumpInterruptMsg (const std::string intr_name, const bool value);
    bool HasError(unsigned int chid);
    bool CheckHasRequest(void);
    unsigned char CalculateCRC(unsigned int message, unsigned char message_length, unsigned char crc_bit_num, unsigned char crc_init, unsigned char crc_poly);
    void CheckStartCommunication(unsigned int chid);
    unsigned int GetChannelPriority(unsigned int ch_com);
    unsigned int Create_SPI_Control(unsigned int chid);
    bool Calculate_mMSPInCS(void);
    bool Compare_mMSPInCS(void);
    double CalculateCLK(eClockInfo id);
    bool GetParity(const unsigned int data, const unsigned int chid);
    void StartFixedBufferCommunication(unsigned int chid);
    void CommuniationSlave(void);
    void CheckWaitInThread();
    std::string FindRegAreaFromName(const std::string reg_name);

    

    // Overwrite virtual functions of MSPI000_AgentController
    bool CheckClockPeriod(const std::string clock_name);
    bool GetResetStatus(void);
    void InactiveOutputPorts(const unsigned int csp, const unsigned int ckr, const unsigned int sols);
    void SetOperationMode(bool mode);
    void SetCommunicationMode(unsigned int chid, unsigned int mode);
    void SetMemoryMode(unsigned int chid, unsigned int mode);
    void SetIPState(bool state);
    void SetChannelState(unsigned int chid, unsigned int state, unsigned int size);
    sc_dt::uint64 GetHWTRGValue();
    void UpdateRequestNumber(unsigned int chid);
    void AsserINTMSPIn(unsigned int chid, std::string int_name, std::string error_type);
    void SetChannelStop(unsigned int chid);
    void NotifyEventOutputINT(unsigned int type);
    void ResetBufferCounter(unsigned int id);
};
#endif //__MSPI000_H__
