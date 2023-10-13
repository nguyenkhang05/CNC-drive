// ---------------------------------------------------------------------
// $Id: MMC44.h,v 1.3 2020/03/19 08:33:32 anhnguyen4 Exp $
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

#ifndef __MMC44_H__
#define __MMC44_H__

#include "global.h"
#include "re_define.h"
#include "BusSlaveBase.h"
#include "MMC44_AgentController.h"
#include "rvc_common_model.h"
#include "MMC44_Func.h"

using namespace std;

class MMC44_Func;
class MMC44 : public sc_module,
              public MMC44_AgentController,
              public BusSlaveBase<BUS_WIDTH_APB, 1>,
              public rvc_common_model
{
  public:
#include "MMC44_cmdif.h"
  public:
    // Clock/Reset
    sc_in<sc_dt::uint64> CLK_HSB; // clock for APB
    sc_in<bool> PRESETn;          // Asynchronous reset signal
    // Input - Output
    sc_out<sc_dt::uint64> CLK; // Communication clock output
    sc_out<bool> DMA;          // DMA request
    sc_out<bool> INT;          // Interrupt request

    sc_inout<uint8_t *> cmd_pointer;  // command pointer in
    sc_inout<uint8_t *> data_pointer; // Data pointer in

    sc_inout<uint32_t> cmd_control;  // Command control port
    sc_inout<uint32_t> data_control; // Data control port
    //----------------------------
    // modeling reset sources and initialization
    //----------------------------
  private:
    /// Declare Clock/Reset area
    sc_dt::uint64 mCLK_HSBOrgFreq;
    sc_dt::uint64 mCLK_HSBFreq;
    sc_dt::uint64 mCLK_HSBPeriod;
    double mCLK_HSBSyncTPoint;
    std::string mCLK_HSBFreqUnit;

    sc_dt::uint64 mCLK_OrgFreq;
    sc_dt::uint64 mCLK_Freq;
    sc_dt::uint64 mCLK_Period;
    double mCLK_SyncTPoint;
    std::string mCLK_FreqUnit;

    bool mIsResetPortInit;   // Is resetPort initialized
    bool mIsResetPortActive; // Is reset by port
    bool mIsResetCmdReceive; // Is receive cmd reset
    bool mIsResetCmdActive;  // Is reset by cmd
    double mResetCmdPeriod;  // Period of cmd reset

    sc_dt::uint64 mTimeResolutionValue; // Time resolution value of simulation
    sc_time_unit mTimeResolutionUnit;   // Time resolution unit of simulation

    sc_event mResetPortEvent;
    sc_event mResetCmdEvent;
    sc_event mCancelResetCmdEvent;


    void CLK_HSBMethod(void);
    void PRESETnHandleMethod(void);
    void resetCmdHandleMethod(void);
    void CancelResetCmdMethod(void);

    void EnableReset(bool isActive);
    void Initialize(void);
    void CancelEvents(void);
    void CancelEventsZeroClock(void);

  public:
    // Constructor
    SC_HAS_PROCESS(MMC44);
    MMC44(sc_module_name name,
          uint32_t rLatency,
          uint32_t wLatency);
    // Destructor
    ~MMC44(void);

  public:
    // TargetSocket for APB bus communication
    TlmTargetSocket<32> *ts;

  public:
    // PythonIF
    void AssertReset(const std::string reset_name, const double start_time, const double period);
    void SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit);
    void GetCLKFreq(const std::string clock_name);
    void ForceRegister(const std::string reg_name, const uint32_t reg_value);
    void ReleaseRegister(const std::string reg_name);
    void WriteRegister(const std::string reg_name, const uint32_t reg_value);
    void ReadRegister(const std::string reg_name);
    void ListRegister(void);
    void Help(const std::string type);
    void SetMessageLevel(const std::string msg_lv);
    void DumpRegisterRW(const std::string is_enable);
    void DumpInterrupt(const std::string is_enable);
    void DumpInterruptMsg(const std::string intr_name, const bool value);
    void DumpStatusInfo(void);
    void EnableRegisterMessage(const std::string is_enable);

  private:
    // variable
    bool mDumpInterrupt;                           ///< Enable/Disable dumping the interrupt info
    bool mINTValue;                                ///< Store value of INT
    bool mDMAValue;                                ///< Store value of DMA

    uint8_t *mCommandFramePointer;                 ///< Store command frame pointer
    uint8_t *mDataFramePointer;                    ///< Store data frame pointer
    uint32_t mCommandControl;                      ///< Store value of command control
    uint32_t mDataControl;                         ///< Store value of data control

    bool mEnableRegisterMessage;                   ///< Enable/Disable info/warning/error message of register IF
    bool mIsInternalWriteCMDControl;               ///< Is internal write cmd_control or not
    bool mIsInternalWriteDataControl;              ///< Is internal write data_control or not
    bool mIsStopClock;                             ///< Is stop clock or not
    bool mIsBootClock;                             ///< Is boot clock or not
    bool mIsCmdResBusy;                            ///< Is command response busy or not
    bool mIsHaveBusySignal;                        ///< Is have busy signal or not
    bool mIsWaitForEndBusyCMD12ToEnd;              ///< Is wait for End Busy of CMD12 to end or not
    bool mIsHandleBusyCMD12;                       ///< Is handle busy of CMD12 or not

    double mTimeResponseStart;                     ///< Store time response frame start
    double mTimeResponseEnd;                       ///< Store time response frame end
    double mTimeCRCREnd;                           ///< Store time CRCR frame start
    double mTimeCRCRStart;                         ///< Store time CRCR frame end
    double mTimeDATAREnd;                          ///< Store time DATAR frame start
    double mTimeDATARStart;                        ///< Store time DATAR frame end
    double mTimeBAREnd;                            ///< Store time BAR frame end
    double mTimeBARStart;                          ///< Store time BAR frame start
    eBufferNumber mNumOfBufferToSend;              ///< Store which buffer to send to card
    eCommandLineSignal mExpectCommandLine;         ///< Store expect command line signal
    eDataLineSignal mExpectDataLineSignal;         ///< Store expect data line signal
    uint32_t mCountDataCardSendToBuffer;           ///< Store number of block data which card had send to buffer

  private:
    // MMC44_Func object
    MMC44_Func *mMMC44_Func; /// Pointer of MMC44 function

    // Events
    sc_event mStartSendCommandEvent;
    sc_event mEndSendCommandEvent;
    sc_event mStartHandlerResponseEvent;
    sc_event mEndHandlerResponseEvent;
    sc_event mSendCMD12Event;
    sc_event mBuffer1SendCompletedEvent;
    sc_event mBuffer2SendCompletedEvent;

    sc_event mAssertInterruptEvent;   // event to assert interrupt
    sc_event mDeAssertInterruptEvent; // event to deassert interrupt

    sc_event mSetCLKEvent;
    sc_event mSetTimeOutAndErrStt[emSizeOfTimeOutAndErrorSttRegister];
    sc_event mIntCMD12CRESetEvent;
    sc_event mIntDTRANESetEvent;
    sc_event mIntBUFRESetEvent;
    sc_event mIntBUFWENSetEvent;
    sc_event mIntCRSPESetEvent;

    sc_event mWriteInterruptSignalEvent;
    sc_event mBuffer1ReadEnableEvent;
    sc_event mBuffer2ReadEnableEvent;
    sc_event mStartCRCREvent;
    sc_event mEndCRCREvent;
    sc_event mStartBusyEvent;
    sc_event mEndBusyEvent;
    sc_event mStartDATAREvent;
    sc_event mEndDATAREvent;
    sc_event mStartBAREvent;
    sc_event mEndBAREvent;
    sc_event mBootOperationEndEvent;
    sc_event mIdleCommandEvent;
    sc_event mIdleDataEvent;

    sc_event mPtrPortWriteCmdEvent;
    sc_event mCtrlPortWriteCmdEvent;

    sc_event mCancelIsInternalWriteCMDEvent;
    sc_event mCancelIsInternalDATACMDEvent;
    sc_event mPtrPortWriteDATAEvent;
    sc_event mCtrlPortWriteDATAEvent;
    sc_event mReadDataFromBufferProcessEvent;
    sc_event mWriteDataToBufferProcessEvent;
    sc_event mUpdateSttForEndCmdResEvent;
    sc_event mWriteDMAPortEvent;
    sc_event mMMCBusIsNotBusyEvent;
    sc_event mStartSendDataToCardEvent;

    sc_event mWriteCtrlCMDAfterZeroTimeEvent;
    sc_event mWriteCtrlDATAfterZeroTimeEvent;
    sc_event mCancelInternalWriteCMDAfterZeroTimeEvent;
    sc_event mCancelInternalWriteDataAfterZeroTimeEvent;
    sc_event mActiveDataLineEvent;
    sc_event mActiveCommandLineEvent;

    sc_event mOneBlockHadReadEvent;
    sc_event mOneBlockHadWrittenEvent;

    // Methods
    void ActiveCommandLineMethod(void);                         ///< Set state of command line to active
    void ActiveDataLineMethod(void);                            ///< Set state of data line to active
    void CancelInternalWriteCMDAfterZeroTimeMethod(void);       ///< Cancel Internal Write CMD after SC_ZERO_TIME.
    void CancelInternalWriteDataAfterZeroTimeMethod(void);      ///< Cancel Internal Write Data after SC_ZERO_TIME.
    void WriteCtrlCMDAfterZeroTimeMethod(void);                 ///< Write Command_Control port after SC_ZERO_TIME.
    void WriteCtrlDATAfterZeroTimeMethod(void);                 ///< Write Data_Control port after SC_ZERO_TIME.
    void MMCBusIsNotBusyMethod(void);                           ///< Set MMC bus Is not busy
    void WriteDMAPortMethod(void);                              ///< Write DMA port
    void UpdateSttForEndCmdResMethod(void);                     ///< Update state and bit status of model when End CMD/RESPONSE.
    void ReadDataFromBufferMethod(void);                        ///< CPU or DMA read data from Buffer
    void WriteDataToBufferMethod(void);                         ///< CPU or DMA write data to Buffer
    void WriteDataPtrPortMethod(void);                          ///< Write to data_pointer port
    void WriteDataCtrlPortMethod(void);                         ///< Write to data_control port
    void CancelIsInternalWriteDATAMethod(void);                 ///< Cancel Internal Write Data
    void CancelIsInternalWriteCMDMethod(void);                  ///< Cancel Internal Write Command
    void WriteCmdPtrPortMethod(void);                           ///< Write to cmd_pointer port
    void WriteCmdCtrlPortMethod(void);                          ///< Write to cmd_control port
    void Buffer1ReadEnableMethod(void);                         ///< buffer1 read enable (Receive 1 block of data to Buffer1)
    void Buffer2ReadEnableMethod(void);                         ///< buffer2 read enable (Receive 1 block of data to Buffer2)
    void HandlerDataControlMethod(void);                        ///< Handler data_control port
    void HandlerCMDControlMethod(void);                         ///< Handler cmd_control port
    void StartSendCommandMethod(void);                          ///< Start send command to card
    void EndSendCommandMethod(void);                            ///< End send command to card
    void StartResponseMethod(void);                             ///< Start Receive Response from card
    void EndResponseMethod(void);                               ///< End receive Response from card
    void StartCRCRMethod(void);                                 ///< Start Receive CRC status from card
    void EndCRCRMethod(void);                                   ///< End Receive CRC status from card
    void StartBusyMethod(void);                                 ///< Start Receive BUSY signal from card
    void EndBusyMethod(void);                                   ///< End Receive BUSY signal from card
    void StartBARMethod(void);                                  ///< Start Receive Boot ACK signal from card
    void EndBARMethod(void);                                    ///< End Receive Boot ACK signal from card
    void StartDATARMethod(void);                                ///< Start Receive Data signal from card
    void EndDATARMethod(void);                                  ///< End Receive Data signal from card
    void StartSendDataToCardMethod(void);                       ///< Start Send Data signal to card
    void Buffer1SendCompletedMethod(void);                      ///< Send completed 1 block size of data to card from buffer 1
    void Buffer2SendCompletedMethod(void);                      ///< Send completed 1 block size of data to card from buffer 2
    void OneBlockHadReadMethod(void);                           ///< Read completed 1 block size of data from buffer
    void OneBlockHadWrittenMethod(void);                        ///< Write completed 1 block size of data to buffer
    void IntBUFWENMethod(void);                                 ///< buffer write enabled
    void IntCMD12CREMethod(void);                               ///< CMD12 command/response end
    void IntDTRANEMethod(void);                                 ///< data transmission completed
    void IntBUFREMethod(void);                                  ///< buffer read end
    void IntCRSPEMethod(void);                                  ///< command response/ completed (not boot)
    void SendCMD12Method(void);                                 ///< Start send CMD12
    void BootOperationEndMethod(void);                          ///< End of Boot operation
    void IdleCommandMethod(void);                               ///< Method to idle command (set stbe in cmd_control to 0)
    void IdleDataMethod(void);                                  ///< Method to idle data (set stbe in cmd_control to 0)
    void WriteInterruptSignalMethod(void);                      ///< Write INT signal
    void HandleSetTimeOutAndErrStt(const uint32_t bit_name);    ///< Handle Set time out and error status bit
    void SetCLKMethod(void);                                    ///< Set CLK method
    void AssertInterruptMethod(void);                           ///< Assert Interrupt method
    void DeAssertInterruptMethod(void);                         ///< DeAssert Interrupt method

    // Virtual functions of MMC44_AgentController
    void ErrorOccuredDuringDMATransfer(void);                   ///< Setting state of model when error occurred during DMA transfer
    void AssertDMAEvent(void);                                  ///< active DMA line
    void AssertOneBlockHadReadEvent(void);                      ///< assert mOneBlockHadReadEvent
    void AssertOneBlockHadWrittenEvent(void);                   ///< assert mOneBlockHadWrittenEvent
    void AssertSetTimeoutAndErrStt(uint32_t index);             ///< assert mSetTimeOutAndErrStt event
    void AssertIntBUFWENSetEvent(void);                         ///< assert mIntBUFWENSetEvent
    void AssertBuffer1ReadEnableEvent(void);                    ///< assert mBuffer1ReadEnableEvent
    void AssertBuffer2ReadEnableEvent(void);                    ///< assert mBuffer2ReadEnableEvent
    void AssertReadDataFromBufferProcess(void);                 ///< to assert mReadDataFromBufferProcessEvent
    void AssertWriteDataToBufferProcess(void);                  ///< to assert mWriteDataToBufferProcessEvent
    void AssertSetBootCLKEvent(void);                           ///< setting clock in boot mode
    void AssertSetCLKEvent(void);                               ///< assert mSetCLKEvent
    void AssertStartSendCommandEvent(void);                     ///< Assert mStartSendCommandEvent
    void EnableSoftwareReset(const bool is_active);             ///< Enable Software reset
    void AssertInterrupt(void);                                 ///< Assert Interrupt
    void DeAssertInterrupt(void);                               ///< De-Assert Interrupt

    // Others function
    bool CheckClockPeriod(const std::string clock_name);        ///< Check clock period
    bool GetResetStatus(void);                                  ///< Get reset status
    sc_time CalculateClockEdge(const std::string clock_name);   ///< Calculate clock edge
};
#endif //__MMC44_H__
