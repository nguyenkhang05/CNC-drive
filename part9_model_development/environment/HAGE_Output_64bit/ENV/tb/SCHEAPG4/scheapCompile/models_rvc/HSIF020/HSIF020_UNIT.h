// ---------------------------------------------------------------------
// $Id: HSIF020_UNIT.h,v 1.5 2020/03/16 02:12:55 quynhnhunguyen Exp $
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

#ifndef __HSIF020_UNIT_H__
#define __HSIF020_UNIT_H__

#include "re_define.h"
#include "HSIF_Common.h"
#include "HSIF020_UNIT_AgentController.h"

class HSIF020_UNIT_Func;

class HSIF020_UNIT: public sc_module,
            public HSIF020_UNIT_AgentController
{
friend class HSIF020;
friend class HSIF020_Func;

public:
    // Declare module
    SC_HAS_PROCESS (HSIF020_UNIT);
    HSIF020_UNIT (sc_module_name name,
                  HSIF020   *HSIF020_Ptr,
                  ChId_t    ch_num,
                  bool      enableStream);
    ~HSIF020_UNIT (void);

    uint8_t*    GetDataPointer(void);           /// Get data pointer for AXI data pointer
    uint32_t    GetPayLoadSizeIndex(void);      /// Get Payload size index for framing process
    L2Frame     GetL2FrameOut(void);            /// Get L2 Frame for framing process
    void        IniHandleCrcError(void);        /// Handle an CRC error
    void        IniHandleTIDError(void);        /// Handle an Transaction id error
    void        RxNonStreamHandleAxiError(void);/// Handle an AXI error
    void        IniHandleUnclassifiedError(void);/// Handle an Unclassified error (when Transaction id occurrs when received NACK command)
    bool        IsChRxEnable(void);              /// Check Rx channel is enable or not
    bool        IsChTxEnable(void);              /// Check Tx channel is enable or not
    uint32_t    GetHSIFnCST(void);               /// Get setting of CST register
private:
    HSIF020_UNIT_Func   *mHSIF020_UNIT_Func;        /// HSIF020_UNIT_Func class instance

    /// Declare variables
    uint8_t     mData[8];                   ///Data for AXI transaction (Non-Stream)

    L2Frame     mL2FrameOutChannel;

    uint32_t    mFrameInfo;
    uint32_t    mPayloadSizeIndex;          ///001b: 32 bits 010b: 64 bits 011b: 96 bits
                                            ///110b: 160 bits 111b: 288 bits
    bool        mEnableStream;
    uint8_t     mCurrentNonStreamFrameId;
    bool        mIgnoreCommandComeAfterTimeOut;
    bool        mIsChannelBusy;

private:
    void        IniNonstreamTransCommand(void);                                             /// Initiator issue the Non-stream command
    void        IniConfigReadCommand(void);                                                 /// Initiator configure READ command
    void        IniConfigWriteCommand(void);                                                /// Initiator configure WRITE command
    void        IniConfigIdEventCommand(void);                                              /// Initiator configure ID/EVENT command
    void        IniHandleACKCommand(void);                                                  /// Handle ACK Command of a Non-stream command
    void        IniHandleNACKCommand(void);                                                 /// Handle NACK Command of a Non-stream command
    void        IniHandleReadAnswerCommand(uint32_t readData);                              /// Handle READ ANSWER Command of a Non-stream command
    /// Handle input frame
    void        TgrHandleReadCmdProcess(uint32_t addr, uint32_t readSize, uint8_t trans_id);/// Handle Non-stream Read Command
    void        TgrHandleWriteCmdProcess(uint32_t addr, uint32_t writeSize                  /// Handle Non-stream Write Command
                                       , uint32_t writeData, uint8_t trans_id);
    void        TgrHandleEventCommand(uint8_t trans_id);                                    /// Handle Non-stream Event Command
    void        TgrHandleIdCommand(uint8_t trans_id);                                       /// Handle Non-stream ID Command
    void        HandleUnisolatedFrame(void);                                                /// Handle an un-isolated input command
    void        TgrConfigReadAnswerCommand(uint32_t r_data, uint32_t data_length);          /// Target configure READ ANSWER command
    void        TgrConfigACKCommand(void);                                                  /// Target configure ACK command for replying a NonStream command
    void        TgrConfigNACKCommand(void);                                                 /// Target configure NACK command for replying a NonStream command
    void        TgrConfigACKStreamCommand(FrameId_t frame_id);                              /// Target configure ACK command for replying a Stream command
    void        TgrConfigNACKStreamCommand(FrameId_t frame_id);                             /// Target configure NACK command for replying a Stream command
    void        IniNonStreamHandleTimeOutOcurs(void);                                       /// Handle an time out event
    void        IgnoreCommand(void);                                                        /// Ignore command when received an error and illegal access
    /// Internal functions
    void        Initialize (void);                                                          /// Initialize variables & output ports
    /// Virtual functions of HSIF020_UNIT_AgentController class
    void        NotifyStartNonStreamEvent(const uint32_t cty_config);                       /// Notify Start event for non-stream transmission process in HSIF020_UNIT
    uint8_t     GetCurrentFramInfo(void);                                                   /// Get current frame type information
    uint32_t    GetTimeOutSetting(void);                                                    /// Get time out setting in register
    bool        IsChannelBusy(void);                                                        /// Check channel is busy (is transferring or wait reply) or not
    bool        CheckClockPeriod(const std::string clock_name);                             /// Check a clock is active or not
    void        SetInterruptStatus(uint32_t bit_name);                                      /// Set interrupt status register
    void        SetInitialSettingPhase(bool isInitialSettingPhase);                         /// Set model is initial phase or not
};
#endif
