// ---------------------------------------------------------------------
// $Id: HSIF020_UNIT.cpp,v 1.6 2020/04/07 03:56:04 quynhnhunguyen Exp $
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

#include "HSIF020_UNIT.h"
#include "HSIF020_UNIT_Func.h"
#include "HSIF020.h"
#include "HSIF020_Func.h"

///Constructor of HSIF020_UNIT class
HSIF020_UNIT::HSIF020_UNIT (sc_module_name name
                           ,HSIF020 *HSIF020_Ptr
                           ,ChId_t ch_id
                           ,bool enableStream):
sc_module(name),
HSIF020_UNIT_AgentController(),
mL2FrameOutChannel()
{//{{{
    assert(HSIF020_Ptr != NULL);
    mHSIF020    = HSIF020_Ptr;
    mCh_id      = ch_id;
    mEnableStream   = enableStream;

    mHSIF020_UNIT_Func = new HSIF020_UNIT_Func((std::string)name, this);
    sc_assert(mHSIF020_UNIT_Func != NULL);

    Initialize();
}//}}}

///Destructor of HSIF020_UNIT class
HSIF020_UNIT::~HSIF020_UNIT (void)
{//{{{
    delete mHSIF020_UNIT_Func;
}//}}}

/// Initialize variables & output ports
void HSIF020_UNIT::Initialize (void)
{//{{{
    // Initialize variables
    mHSIF020_UNIT_Func->Initialize();
    
    // Initialize member attribute
    memset(mData,0x0,8);
    mL2FrameOutChannel.Initialize();
    mCurrentNonStreamFrameId        = 0;
    mIgnoreCommandComeAfterTimeOut  = false;
    mFrameInfo                      = 0;
    mPayloadSizeIndex               = 0;
    mIsChannelBusy                  = false;
}//}}}

/// Get data pointer for AXI data pointer
uint8_t* HSIF020_UNIT::GetDataPointer(void)
{//{{{
    return (uint8_t*)mData;
}//}}}

/// Get Payload size index for framing process
uint32_t HSIF020_UNIT::GetPayLoadSizeIndex(void)
{//{{{
    return mPayloadSizeIndex;
}//}}}

/// Get L2 Frame for framing process
L2Frame HSIF020_UNIT::GetL2FrameOut()
{//{{{
    return mL2FrameOutChannel;
}//}}}

/// Notify Start event for non-stream transmission process in HSIF020_UNIT
void HSIF020_UNIT::NotifyStartNonStreamEvent(const unsigned int cty_config)
{//{{{
    mFrameInfo = cty_config;

    /// Channel is busy
    mHSIF020_UNIT_Func->ClearChannelStatus(emRDY);
    mIsChannelBusy = true;
    IniNonstreamTransCommand();
}//}}}

/// Initiator issue the Non-stream command
void HSIF020_UNIT::IniNonstreamTransCommand()
{//{{{
    // Initialize time-out judge flag
    mIgnoreCommandComeAfterTimeOut = false;
    //GenerateCommand();
    if (((mFrameInfo == emReadCmd8)
      | (mFrameInfo == emReadCmd16)
      | (mFrameInfo == emReadCmd32)) != 0){
        IniConfigReadCommand();
    } else if (((mFrameInfo == emWriteCmd8)
          | (mFrameInfo == emWriteCmd16)
          | (mFrameInfo == emWriteCmd32)) != 0){
        IniConfigWriteCommand();
    } else if (((mFrameInfo == emEventCmd)
            | (mFrameInfo == emIdCmd)) != 0){
        IniConfigIdEventCommand();
    } 
    mHSIF020->NotifyCmdtoL2Arbiter(mCh_id);
}//}}}

/// Initiator configure READ command
void HSIF020_UNIT::IniConfigReadCommand()
{//{{{
    mPayloadSizeIndex = emPayLoadSize64;
    unsigned int tRwAddr = mHSIF020_UNIT_Func->GetRWAddr();
    L2Payload_t tL2Payload(tRwAddr,0x0,0x0,0x0,0x0,0x0,0x0,0x0);

    mL2FrameOutChannel.SetCommand((uint8_t)mFrameInfo);

    mL2FrameOutChannel.SetPayloadData(tL2Payload);
}//}}}

/// Initiator configure WRITE command
void HSIF020_UNIT::IniConfigWriteCommand()
{//{{{
    unsigned int tRwAddr = (unsigned int)mHSIF020_UNIT_Func->GetRWAddr();
    unsigned int tWData = (unsigned int)mHSIF020_UNIT_Func->GetWData();
    mPayloadSizeIndex = emPayLoadSize96;
    if (mFrameInfo == emWriteCmd8){
        tWData = (tWData & 0xFF) | (tWData & 0xFF) << 8 | (tWData & 0xFF) << 16 | (tWData & 0xFF) << 24;
    } else if (mFrameInfo == emWriteCmd16){
        tWData = (tWData & 0xFFFF) | (tWData & 0xFFFF) << 16;
    } else if (mFrameInfo == emWriteCmd32){
        tWData = tWData & 0xFFFFFFFF;
    }

    L2Payload_t tL2Payload(tRwAddr,tWData,0x0,0x0,0x0,0x0,0x0,0x0);
    mL2FrameOutChannel.SetCommand((uint8_t)mFrameInfo);
    mL2FrameOutChannel.SetPayloadData(tL2Payload);
}//}}}

/// Initiator configure ID/EVENT command
void HSIF020_UNIT::IniConfigIdEventCommand()
{//{{{
    mPayloadSizeIndex = emPayLoadSize32;
    L2Payload_t tL2Payload(0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0);
    mL2FrameOutChannel.SetPayloadData(tL2Payload);
    mL2FrameOutChannel.SetCommand((uint8_t)mFrameInfo);
}//}}}

/// Target configure READ ANSWER command
void HSIF020_UNIT::TgrConfigReadAnswerCommand(uint32_t r_data, uint32_t data_length)
{//{{{
    mPayloadSizeIndex = emPayLoadSize64;

    if (data_length == 0x1){
        r_data = (r_data & 0xFF) | (r_data & 0xFF) << 8 | (r_data & 0xFF) << 16 | (r_data & 0xFF) << 24;
    } else if (data_length == 0x2){
        r_data = (r_data & 0xFFFF) | (r_data & 0xFFFF) << 16;
    } else if (data_length == 0x4){
        r_data = r_data & 0xFFFFFFFF;
    } 
    L2Payload_t tL2Payload(r_data,0x0,0x0,0x0,0x0,0x0,0x0,0x0);

    mL2FrameOutChannel.SetTransactionID(mCurrentNonStreamFrameId);
    mL2FrameOutChannel.SetCommand((uint8_t)emReadAnswerCmd);
    mL2FrameOutChannel.SetPayloadData(tL2Payload);

    mHSIF020->NotifyCmdtoL2Arbiter(mCh_id);
}//}}}

/// Target configure ACK command for replying a NonStream command
void HSIF020_UNIT::TgrConfigACKCommand()
{//{{{
    mPayloadSizeIndex = emPayLoadSize32;
    L2Payload_t tL2Payload(0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0);
    mL2FrameOutChannel.SetTransactionID(mCurrentNonStreamFrameId);
    mL2FrameOutChannel.SetCommand((uint8_t)emACKCmd);
    mL2FrameOutChannel.SetPayloadData(tL2Payload);

    mHSIF020->NotifyCmdtoL2Arbiter(mCh_id);
}//}}}

/// Target configure NACK command for replying a NonStream command
void HSIF020_UNIT::TgrConfigNACKCommand()
{//{{{
    mPayloadSizeIndex = emPayLoadSize32;
    L2Payload_t tL2Payload(0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0);
    mL2FrameOutChannel.SetTransactionID(mCurrentNonStreamFrameId);
    mL2FrameOutChannel.SetCommand((uint8_t)emNACKCmd);
    mL2FrameOutChannel.SetPayloadData(tL2Payload);
    mHSIF020->NotifyCmdtoL2Arbiter(mCh_id);
}//}}}

/// Target configure ACK command for replying a Stream command
void HSIF020_UNIT::TgrConfigACKStreamCommand(FrameId_t frame_id)
{//{{{
    mPayloadSizeIndex = emPayLoadSize32;
    L2Payload_t tL2Payload(0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0);
    mL2FrameOutChannel.SetTransactionID(frame_id);
    mL2FrameOutChannel.SetCommand((uint8_t)emACKCmd);
    mL2FrameOutChannel.SetPayloadData(tL2Payload);

    mHSIF020->NotifyCmdtoL2Arbiter(mCh_id);
}//}}}

/// Target configure NACK command for replying a Stream command
void HSIF020_UNIT::TgrConfigNACKStreamCommand(FrameId_t frame_id)
{//{{{
    mPayloadSizeIndex = emPayLoadSize32;
    L2Payload_t tL2Payload(0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0);
    mL2FrameOutChannel.SetTransactionID(frame_id);
    mL2FrameOutChannel.SetCommand((uint8_t)emNACKCmd);
    mL2FrameOutChannel.SetPayloadData(tL2Payload);
    mHSIF020->NotifyCmdtoL2Arbiter(mCh_id);
}//}}}


/// Handle Non-stream Read Command
void HSIF020_UNIT::TgrHandleReadCmdProcess(uint32_t addr
                                         , uint32_t readSize
                                         , uint8_t trans_id)
{//{{{
    mCurrentNonStreamFrameId = trans_id;

    bool isHitMemWindow     = mHSIF020->IsHitMemWindow(addr, addr + readSize, false);
    bool isPassedAuthent    = mHSIF020->IsPassedAuthent();
    bool isRxChannelEn      = IsChRxEnable();
    if ( isHitMemWindow
      && isPassedAuthent
      && isRxChannelEn){
        mHSIF020->ConfigTransaction(false, addr, readSize, false, mCh_id);
    } else {
        if (!isRxChannelEn){
            re_printf("warning","Received READ command when Rx is Disable.");
        } else if (!isPassedAuthent){
            //update status authentication error
            uint32_t ssAER = emAER0 + ((uint32_t)mCh_id << 2);
            uint32_t issAERE = emAERE0 + ((uint32_t)mCh_id << 2);
            mHSIF020->mHSIF020_Func->SetAuthenErrStatus(ssAER);
            if (mHSIF020->mHSIF020_Func->GetAuthErrIntEnable(issAERE)){
                mHSIF020->mHSIF020_Func->SetInterruptStatus(emAESS);
            } 
            re_printf("warning","Received READ command when Authentication is not passed.");
        } else if (!isHitMemWindow){
            //Update status Mis-hit memory error
            uint32_t ssWER = emWER0 + ((uint32_t)mCh_id << 1);
            uint32_t issWERE = emWERE0 + ((uint32_t)mCh_id << 1);
            mHSIF020->mHSIF020_Func->SetAuthenErrStatus(ssWER);
            if (mHSIF020->mHSIF020_Func->GetAuthErrIntEnable(issWERE)){
                mHSIF020->mHSIF020_Func->SetInterruptStatus(emAESS);
            }
            re_printf("warning","Received READ command but it not hit any memory windows.");
        }
        IgnoreCommand();
    }
}//}}}

/// Handle Non-stream Write Command
void HSIF020_UNIT::TgrHandleWriteCmdProcess(uint32_t addr
                                  , uint32_t writeSize
                                  , uint32_t writeData
                                  , uint8_t trans_id)
{//{{{
    mCurrentNonStreamFrameId = trans_id;

    bool isHitMemWindow  = mHSIF020->IsHitMemWindow(addr, addr + writeSize, true);
    bool isPassedAuthent = mHSIF020->IsPassedAuthent();
    bool isRxChannelEn   = IsChRxEnable();
    bool isAccessToAuthentRegs      = mHSIF020->mHSIF020_Func->IsAccessToAuthentRegs(addr);

    if ((isHitMemWindow && isPassedAuthent && isRxChannelEn)
        || (isHitMemWindow && isAccessToAuthentRegs))
    {
        memcpy(mData,&writeData,writeSize);
        mHSIF020->ConfigTransaction(true, addr, writeSize, false, mCh_id);
    } else {
        if (!isRxChannelEn){
            re_printf("warning","Received WRITE command when Rx Disable.");
        } else if (!isPassedAuthent){
            //update status authentication error
            uint32_t ssAEW = emAEW0 + ((uint32_t)mCh_id << 2);
            uint32_t issAEWE = emAEWE0 + ((uint32_t)mCh_id << 2);
            mHSIF020->mHSIF020_Func->SetAuthenErrStatus(ssAEW);
            if (mHSIF020->mHSIF020_Func->GetAuthErrIntEnable(issAEWE)){
                mHSIF020->mHSIF020_Func->SetInterruptStatus(emAESS);
            } 
            re_printf("warning","Received WRITE command when Authentication is not passed.");
        } else if (!isHitMemWindow){
            //Update status authentication error
            uint32_t ssWEW = emWEW0 + ((uint32_t)mCh_id << 1);
            uint32_t issWEWE = emWEWE0 + ((uint32_t)mCh_id << 1);
            mHSIF020->mHSIF020_Func->SetAuthenErrStatus(ssWEW);
            if (mHSIF020->mHSIF020_Func->GetAuthErrIntEnable(issWEWE)){
                mHSIF020->mHSIF020_Func->SetInterruptStatus(emAESS);
            } 
            re_printf("warning","Received WRITE command but it not hit any memory windows");
        }
        IgnoreCommand();
    }
}//}}}

/// Handle Non-stream Event Command
void HSIF020_UNIT::TgrHandleEventCommand(uint8_t trans_id)
{//{{{
    mCurrentNonStreamFrameId = trans_id;

    bool isPassedAuthent = mHSIF020->IsPassedAuthent();
    bool isRxChannelEn   = IsChRxEnable();

    if (isPassedAuthent && isRxChannelEn){
        /// Set Event Trigger Flag (HSIFnCST(N).TER(N)=1)
        mHSIF020_UNIT_Func->SetChannelStatus(emTER);
        /// Issue Interrupt
        if (mHSIF020_UNIT_Func->GetChIntEnable(emTERE)){
            uint32_t ssTERS = emTERS0 + ((uint32_t)mCh_id << 2);
            mHSIF020->mHSIF020_Func->SetInterruptStatus(ssTERS);
        } 
        /// Issue ACK command
        this->TgrConfigACKCommand();
    } else {
        if (!isRxChannelEn){
            re_printf("warning","Received EVENT command when Rx Disable.");
        } else if(!isPassedAuthent){
            //Update status authentication error
            re_printf("warning","Received EVENT command when Authentication is not passed.");
            uint32_t ssAEE = emAEE0 + ((uint32_t)mCh_id << 2);
            uint32_t issAEEE = emAEEE0 + ((uint32_t)mCh_id << 2);
            mHSIF020->mHSIF020_Func->SetAuthenErrStatus(ssAEE);

            if (mHSIF020->mHSIF020_Func->GetAuthErrIntEnable(issAEEE)){
                mHSIF020->mHSIF020_Func->SetInterruptStatus(emAESS);
            } 
        } 
        IgnoreCommand();
    }
}//}}}

/// Handle Non-stream ID Command
void HSIF020_UNIT::TgrHandleIdCommand(uint8_t trans_id)
{//{{{
    mCurrentNonStreamFrameId = trans_id;

    bool isPassedAuthent = mHSIF020->IsPassedAuthent();
    bool isRxChannelEn   = IsChRxEnable();

    if (isPassedAuthent && isRxChannelEn){
        ///Get JTAG-ID  Information
        uint32_t tJtag_id = mHSIF020->GetJtagID();
        /// Tx Read Answer Command
        this->TgrConfigReadAnswerCommand(tJtag_id,4);
    } else {
        if (!isRxChannelEn){
            re_printf("warning","Received ID command when Rx Disable.");
        } else if (!mHSIF020->IsPassedAuthent()){
            ////Update status authentication error
            re_printf("warning","Received ID command when Authentication is not passed.");
            uint32_t ssAEI = emAEI0 + ((uint32_t)mCh_id << 2);
            uint32_t ssAEIE = emAEIE0 + ((uint32_t)mCh_id << 2);
            mHSIF020->mHSIF020_Func->SetAuthenErrStatus(ssAEI);
            if (mHSIF020->mHSIF020_Func->GetAuthErrIntEnable(ssAEIE)) {
                mHSIF020->mHSIF020_Func->SetInterruptStatus(emAESS);
            } 
        } 
        IgnoreCommand();
    }
}//}}}

/// Handle ACK Command of a Non-stream command
void HSIF020_UNIT::IniHandleACKCommand()
{//{{{
    ///Set Rx NACK Flag (HSIFnCST(N).AKR(N)=1)
    ///Issue Interrupt
    if (IsChTxEnable()){
        if (!mIgnoreCommandComeAfterTimeOut){
            mHSIF020_UNIT_Func->SetChannelStatus(emAKR);
            if (mHSIF020_UNIT_Func->GetChIntEnable(emAKRE)){
                uint32_t ssAKRS = emAKRS0 + ((uint32_t)mCh_id << 2);
                mHSIF020->mHSIF020_Func->SetInterruptStatus(ssAKRS);
            }
            /// Channel Free
            mHSIF020_UNIT_Func->SetChannelStatus(emRDY);
            /// Cancel TimeOut
            mHSIF020->TxNonStreamStopTimeOutEvent(mCh_id);
            mIsChannelBusy = false;
            mHSIF020->SetInitialSettingPhase(true);
        } else {
            re_printf("warning","Reply time-out occurred. Ignore ACK reply command for channel: %d", mCh_id);
        }
    } else {
        re_printf("warning","Tx Disable. Ignore ACK reply command.");
    }
}//}}}

/// Handle NACK Command of a Non-stream command
void HSIF020_UNIT::IniHandleNACKCommand()
{//{{{
    ///Set Rx NACK Flag (HSIFnCST(N).AKE(N)=1)
    ///Issue Interrupt
    if (IsChTxEnable()){
        if (!mIgnoreCommandComeAfterTimeOut){
            mHSIF020_UNIT_Func->SetChannelStatus(emAKE);
            if (mHSIF020_UNIT_Func->GetChIntEnable(emAKEE)){
                std::ostringstream ss;
                ss << "CERS" << (uint32_t)mCh_id;
                uint32_t ssCERS = emCERS0 + mCh_id;
                mHSIF020->mHSIF020_Func->SetInterruptStatus(ssCERS);
            } 
            /// Channel Free
            mHSIF020_UNIT_Func->SetChannelStatus(emRDY);
            /// Cancel TimeOut
            mHSIF020->TxNonStreamStopTimeOutEvent(mCh_id);
            mIsChannelBusy = false;
            mHSIF020->SetInitialSettingPhase(true);
        } else {
            re_printf("warning","Reply time-out occurred. Ignore NACK reply command for channel: %d", mCh_id);
        }
    } else {
        re_printf("warning","Tx Disable. Ignore NACK reply command.");
    }
}//}}}

/// Handle READ ANSWER Command of a Non-stream command
void HSIF020_UNIT::IniHandleReadAnswerCommand(uint32_t readData)
{//{{{
    ///Set Rx Data (HSIFnCRD(N))
    ///Set Rx Read Answer Flag (HSIFnCST(N).RAR(N)=1)
    if (IsChTxEnable()){
        if (!mIgnoreCommandComeAfterTimeOut){
            mHSIF020_UNIT_Func->SetChannelStatus(emRAR);
            mHSIF020_UNIT_Func->SetReadData(readData);
            ///Issue Interrupt
            if (mHSIF020_UNIT_Func->GetChIntEnable(emRARE)){
                /// Update Interrupt status register MIST
                uint32_t ssRARS = emRARS0 + ((uint32_t)mCh_id << 2);
                mHSIF020->mHSIF020_Func->SetInterruptStatus(ssRARS);
            }
            /// Channel Free
            mHSIF020_UNIT_Func->SetChannelStatus(emRDY);
            /// Cancel TimeOut
            mHSIF020->TxNonStreamStopTimeOutEvent(mCh_id);
            mIsChannelBusy = false;
            mHSIF020->SetInitialSettingPhase(true);
        } else {
            re_printf("warning","Reply time-out occurred. Ignore Read Answer reply command for channel %d", mCh_id);
        }
        mIsChannelBusy = false;
        mHSIF020->SetInitialSettingPhase(true);
    } else {
        re_printf("warning","Tx Disable. Ignore Read Answer reply command");
    }
}//}}}

/// Handle an CRC error
void HSIF020_UNIT::IniHandleCrcError(void)
{//{{{
    mHSIF020_UNIT_Func->SetChannelStatus(emCRE);
    if (IsChTxEnable()){
        mHSIF020_UNIT_Func->SetChannelStatus(emRDY);
    }
    if (mHSIF020_UNIT_Func->GetChIntEnable(emCREE)){
        uint32_t ssCERS = emCERS0 + mCh_id;
        mHSIF020->mHSIF020_Func->SetInterruptStatus(ssCERS);
    } 
    mIsChannelBusy = false;
    mHSIF020->SetInitialSettingPhase(true);
    mHSIF020->TxNonStreamStopTimeOutEvent(mCh_id);
}//}}}

/// Handle an Transaction id error
void HSIF020_UNIT::IniHandleTIDError(void)
{//{{{
    mHSIF020_UNIT_Func->SetChannelStatus(emIDE);
    if (IsChTxEnable()){
        mHSIF020_UNIT_Func->SetChannelStatus(emRDY);
    } 
    if (mHSIF020_UNIT_Func->GetChIntEnable(emIDEE)){
        uint32_t ssCERS = emCERS0 + mCh_id;
        mHSIF020->mHSIF020_Func->SetInterruptStatus(ssCERS);
    } 
    mIsChannelBusy = false;
    mHSIF020->SetInitialSettingPhase(true);
    mHSIF020->TxNonStreamStopTimeOutEvent(mCh_id);
}//}}}

/// Handle an AXI error
void HSIF020_UNIT::RxNonStreamHandleAxiError(void)
{//{{{
    mHSIF020_UNIT_Func->SetChannelStatus(emBRE);

    if (mHSIF020_UNIT_Func->GetChIntEnable(emBREE)){
        uint32_t ssCERS = emCERS0 + mCh_id;
        mHSIF020->mHSIF020_Func->SetInterruptStatus(ssCERS);
    } 
}//}}}

/// Handle an Unclassified error (when Transaction id occurrs when received NACK command)
void HSIF020_UNIT::IniHandleUnclassifiedError(void)
{//{{{
    mHSIF020_UNIT_Func->SetChannelStatus(emAOE);
    if (IsChTxEnable()){
        mHSIF020_UNIT_Func->SetChannelStatus(emRDY);
    } 
    if (mHSIF020_UNIT_Func->GetChIntEnable(emAOEE)){
        uint32_t ssCERS = emCERS0 + mCh_id;
        mHSIF020->mHSIF020_Func->SetInterruptStatus(ssCERS);
    } 
    mIsChannelBusy = false;
    mHSIF020->SetInitialSettingPhase(true);
    mHSIF020->TxNonStreamStopTimeOutEvent(mCh_id);
}//}}}

/// Handle an time out event
void HSIF020_UNIT::IniNonStreamHandleTimeOutOcurs(void)
{//{{{
    mHSIF020_UNIT_Func->SetChannelStatus(emTOE);
    if (IsChTxEnable()){
        mHSIF020_UNIT_Func->SetChannelStatus(emRDY);
    } 
    if (mHSIF020_UNIT_Func->GetChIntEnable(emTOEE)){
        uint32_t ssCERS = emCERS0 + mCh_id;
        mHSIF020->mHSIF020_Func->SetInterruptStatus(ssCERS);
    }
    mIsChannelBusy = false;
    mHSIF020->SetInitialSettingPhase(true);
    mIgnoreCommandComeAfterTimeOut = true;
}//}}}

/// Handle an un-isolated input command
void HSIF020_UNIT::HandleUnisolatedFrame(void)
{//{{{
    re_printf("warning", "Ignore Receive Frame. Cann't detect frame type.");
}//}}}

/// Get time out setting in register
uint32_t HSIF020_UNIT::GetTimeOutSetting(void)
{//{{{
    return mHSIF020->GetTimeOutSetting();
}//}}}

/// Ignore command when received an error and illegal access
void HSIF020_UNIT::IgnoreCommand(void)
{//{{{
    //Do nothing
    re_printf("warning","This command is ignored due to an illegal access.");
}//}}}

/// Get current frame type information
uint8_t HSIF020_UNIT::GetCurrentFramInfo(void)
{//{{{
    return (uint8_t)mFrameInfo;
}//}}}

/// Check Rx channel is enable or not
bool HSIF020_UNIT::IsChRxEnable(void)
{//{{{
    return mHSIF020_UNIT_Func->IsChRxEnable();
}//}}}

/// Check Tx channel is enable or not
bool HSIF020_UNIT::IsChTxEnable(void)
{//{{{
    return mHSIF020_UNIT_Func->IsChTxEnable();
}//}}}

/// Check channel is busy (is transferring or wait reply) or not
bool HSIF020_UNIT::IsChannelBusy(void)
{//{{{
    return mIsChannelBusy;
}//}}}

/// Check a clock is active or not
bool HSIF020_UNIT::CheckClockPeriod(const std::string clock_name)
{//{{{
    return mHSIF020->CheckClockPeriod(clock_name);
}//}}}

/// Set interrupt status register
void HSIF020_UNIT::SetInterruptStatus(uint32_t bit_name)
{//{{{
    /// emRAR <=> emRARS0, emAKR <=>emAKRS0, emTER <=> TERS0
    if ((bit_name== emRAR)){
        uint32_t temp_bit_name = emRARS0 + ((uint32_t)mCh_id << 2);
        mHSIF020->mHSIF020_Func->SetInterruptStatus(temp_bit_name);
    } else if ((bit_name== emAKR)){
        uint32_t temp_bit_name = emAKRS0 + ((uint32_t)mCh_id << 2);
        mHSIF020->mHSIF020_Func->SetInterruptStatus(temp_bit_name);
    } else if ((bit_name== emTER)){
        uint32_t temp_bit_name = emTERS0 + ((uint32_t)mCh_id << 2);
        mHSIF020->mHSIF020_Func->SetInterruptStatus(temp_bit_name);
    } else {
        uint32_t temp_bit_name = emCERS0 + mCh_id;
        mHSIF020->mHSIF020_Func->SetInterruptStatus(temp_bit_name);
    }
}//}}}

/// Set model is initial phase or not
void HSIF020_UNIT::SetInitialSettingPhase(bool isInitialSettingPhase)
{//{{{
    mHSIF020->SetInitialSettingPhase(isInitialSettingPhase);
}//}}}

/// Get setting of CST register
uint32_t HSIF020_UNIT::GetHSIFnCST(void)
{//{{{
    return mHSIF020_UNIT_Func->GetHSIFnCST();
}//}}}

// vim600: set foldmethod=marker :
