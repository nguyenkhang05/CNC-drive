// ---------------------------------------------------------------------
// $Id: MMC44.cpp,v 1.4 2020/04/06 07:28:44 anhnguyen4 Exp $
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

#include "MMC44.h"

MMC44::MMC44(sc_module_name name, uint32_t rLatency, uint32_t wLatency)
: sc_module(name)
, MMC44_AgentController()
, BusSlaveBase<BUS_WIDTH_APB, 1>()
, rvc_common_model()
, mDumpInterrupt(0), DMA("DMA")
, INT("INT")
, CLK_HSB("CLK_HSB")
, CLK("CLK")
, PRESETn("PRESETn")
, cmd_pointer("cmd_pointer")
, data_pointer("data_pointer")
, cmd_control("cmd_control")
, data_control("data_control")
, mResetCmdPeriod(0)
, mIsResetCmdActive(0)
, mIsResetPortInit(0)
, mIsResetPortActive(0)
, mIsResetCmdReceive(0)
, mCLK_HSBFreq(0)
, mCLK_HSBFreqUnit("Hz")
, mCLK_HSBOrgFreq(0)
, mCLK_HSBPeriod(0)
, mCLK_HSBSyncTPoint(0)
, mCLK_OrgFreq(0)
, mCLK_Freq(0)
, mCLK_Period(0)
, mCLK_SyncTPoint(0)
, mCLK_FreqUnit("Hz")
, mTimeResolutionUnit(SC_NS)
, mINTValue(true)
, mDMAValue(true)
, mCommandFramePointer(NULL)
, mCommandControl(0)
, mIsInternalWriteCMDControl(0)
, mIsStopClock(true)
, mIsBootClock(0)
, mIsHaveBusySignal(false)
, mEnableRegisterMessage(true)
, mDataFramePointer(NULL)
, mDataControl(0)
, mIsInternalWriteDataControl(false)
, mTimeResponseStart(0)
, mTimeResponseEnd(0)
, mTimeCRCREnd(0)
, mTimeCRCRStart(0)
, mTimeDATAREnd(0)
, mTimeDATARStart(0)
, mTimeBAREnd(0)
, mTimeBARStart(0)
, mNumOfBufferToSend(emBuffer1)
, mIsCmdResBusy(true)
, mExpectCommandLine(emDefaultCommandInfo)
, mExpectDataLineSignal(emDefaultDataSignal)
, mCountDataCardSendToBuffer(0)
, mIsWaitForEndBusyCMD12ToEnd(0)
, mIsHandleBusyCMD12(0)
, mTimeResolutionValue(0)
{ //{{{
    /// Time resolution
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    ///------------------------------------------------------------------------------
    /// APBsocket
    ///------------------------------------------------------------------------------
    setSlaveFreqPort32(&CLK_HSB);

    setSlaveResetPort32(&PRESETn);

    setTargetSocket32((char*)"ts");

    ts = BusSlaveBase<32, 1>::tSocket32[0]; //Alias for binding

    mMMC44_Func = new MMC44_Func((string)name, this);

    mBusSlaveIf32[0]->setFuncModulePtr(mMMC44_Func);

    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    //------------------------------------------------------------------------------
    //  HOST - CARD
    //------------------------------------------------------------------------------
    DMA.initialize(true);
    INT.initialize(true);
    cmd_pointer.initialize(0x0);
    data_pointer.initialize(0x0);
    cmd_control.initialize(0x0);
    data_control.initialize(0x0);
    CLK.initialize(0x0);

    /// Register process to SystemC kernel
    SC_METHOD(CLK_HSBMethod);
    dont_initialize();
    sensitive << CLK_HSB;

    SC_METHOD(PRESETnHandleMethod);
    dont_initialize();
    sensitive << PRESETn;

    SC_METHOD(resetCmdHandleMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;

    SC_METHOD(AssertInterruptMethod);
    dont_initialize();
    sensitive << mAssertInterruptEvent;

    SC_METHOD(DeAssertInterruptMethod);
    dont_initialize();
    sensitive << mDeAssertInterruptEvent;

    SC_METHOD(OneBlockHadReadMethod);
    dont_initialize();
    sensitive << mOneBlockHadReadEvent;

    SC_METHOD(OneBlockHadWrittenMethod);
    dont_initialize();
    sensitive << mOneBlockHadWrittenEvent;


    SC_METHOD(IntCMD12CREMethod);
    dont_initialize();
    sensitive << mIntCMD12CRESetEvent;

    SC_METHOD(IntDTRANEMethod);
    dont_initialize();
    sensitive << mIntDTRANESetEvent;

    SC_METHOD(IntBUFREMethod);
    dont_initialize();
    sensitive << mIntBUFRESetEvent;

    SC_METHOD(IntBUFWENMethod);
    dont_initialize();
    sensitive << mIntBUFWENSetEvent;

    SC_METHOD(IntCRSPEMethod);
    dont_initialize();
    sensitive << mIntCRSPESetEvent;

    SC_METHOD(Buffer1ReadEnableMethod);
    dont_initialize();
    sensitive << mBuffer1ReadEnableEvent;

    SC_METHOD(Buffer2ReadEnableMethod);
    dont_initialize();
    sensitive << mBuffer2ReadEnableEvent;

    SC_METHOD(Buffer1SendCompletedMethod);
    dont_initialize();
    sensitive << mBuffer1SendCompletedEvent;

    SC_METHOD(Buffer2SendCompletedMethod);
    dont_initialize();
    sensitive << mBuffer2SendCompletedEvent;

    SC_METHOD(SetCLKMethod);
    dont_initialize();
    sensitive << mSetCLKEvent;

    SC_METHOD(SendCMD12Method);
    dont_initialize();
    sensitive << mSendCMD12Event;

    SC_METHOD(WriteInterruptSignalMethod);
    dont_initialize();
    sensitive << mWriteInterruptSignalEvent;

    for (uint32_t index = emCRCSTE_H; index < emSizeOfTimeOutAndErrorSttRegister; index++)
    {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSetTimeOutAndErrStt[index]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&MMC44::HandleSetTimeOutAndErrStt, this, index),
                          sc_core::sc_gen_unique_name("HandleSetTimeOutAndErrStt"), &opt);
    }

    SC_METHOD(HandlerCMDControlMethod);
    dont_initialize();
    sensitive << cmd_control;

    SC_METHOD(HandlerDataControlMethod);
    dont_initialize();
    sensitive << data_control;

    SC_METHOD(StartSendCommandMethod);
    dont_initialize();
    sensitive << mStartSendCommandEvent;

    SC_METHOD(EndSendCommandMethod);
    dont_initialize();
    sensitive << mEndSendCommandEvent;

    SC_METHOD(StartResponseMethod);
    dont_initialize();
    sensitive << mStartHandlerResponseEvent;

    SC_METHOD(EndResponseMethod);
    dont_initialize();
    sensitive << mEndHandlerResponseEvent;

    SC_METHOD(BootOperationEndMethod);
    dont_initialize();
    sensitive << mBootOperationEndEvent;

    SC_METHOD(IdleCommandMethod);
    dont_initialize();
    sensitive << mIdleCommandEvent;

    SC_METHOD(IdleDataMethod);
    dont_initialize();
    sensitive << mIdleDataEvent;

    SC_METHOD(StartCRCRMethod);
    dont_initialize();
    sensitive << mStartCRCREvent;

    SC_METHOD(EndCRCRMethod);
    dont_initialize();
    sensitive << mEndCRCREvent;

    SC_METHOD(StartBusyMethod);
    dont_initialize();
    sensitive << mStartBusyEvent;

    SC_METHOD(EndBusyMethod);
    dont_initialize();
    sensitive << mEndBusyEvent;

    SC_METHOD(StartBARMethod);
    dont_initialize();
    sensitive << mStartBAREvent;

    SC_METHOD(EndBARMethod);
    dont_initialize();
    sensitive << mEndBAREvent;

    SC_METHOD(StartDATARMethod);
    dont_initialize();
    sensitive << mStartDATAREvent;

    SC_METHOD(EndDATARMethod);
    dont_initialize();
    sensitive << mEndDATAREvent;

    SC_METHOD(WriteCmdPtrPortMethod);
    dont_initialize();
    sensitive << mPtrPortWriteCmdEvent;

    SC_METHOD(WriteCmdCtrlPortMethod);
    dont_initialize();
    sensitive << mCtrlPortWriteCmdEvent;

    SC_METHOD(WriteDataPtrPortMethod);
    dont_initialize();
    sensitive << mPtrPortWriteDATAEvent;

    SC_METHOD(WriteDataCtrlPortMethod);
    dont_initialize();
    sensitive << mCtrlPortWriteDATAEvent;

    SC_METHOD(CancelIsInternalWriteCMDMethod);
    dont_initialize();
    sensitive << mCancelIsInternalWriteCMDEvent;

    SC_METHOD(CancelIsInternalWriteDATAMethod);
    dont_initialize();
    sensitive << mCancelIsInternalDATACMDEvent;

    SC_METHOD(ReadDataFromBufferMethod);
    dont_initialize();
    sensitive << mReadDataFromBufferProcessEvent;

    SC_METHOD(WriteDataToBufferMethod);
    dont_initialize();
    sensitive << mWriteDataToBufferProcessEvent;

    SC_METHOD(WriteDMAPortMethod);
    dont_initialize();
    sensitive << mWriteDMAPortEvent;

    SC_METHOD(UpdateSttForEndCmdResMethod);
    dont_initialize();
    sensitive << mUpdateSttForEndCmdResEvent;

    SC_METHOD(MMCBusIsNotBusyMethod);
    dont_initialize();
    sensitive << mMMCBusIsNotBusyEvent;

    SC_METHOD(StartSendDataToCardMethod);
    dont_initialize();
    sensitive << mStartSendDataToCardEvent;

//    SC_METHOD(HandlerDataMethod);
//    dont_initialize();
//    sensitive << mHandlerDataEvent;

    SC_METHOD(WriteCtrlDATAfterZeroTimeMethod);
    dont_initialize();
    sensitive << mWriteCtrlCMDAfterZeroTimeEvent;

    SC_METHOD(WriteCtrlCMDAfterZeroTimeMethod);
    dont_initialize();
    sensitive << mWriteCtrlDATAfterZeroTimeEvent;

    SC_METHOD(CancelInternalWriteDataAfterZeroTimeMethod);
    dont_initialize();
    sensitive << mCancelInternalWriteDataAfterZeroTimeEvent;

    SC_METHOD(CancelInternalWriteCMDAfterZeroTimeMethod);
    dont_initialize();
    sensitive << mCancelInternalWriteCMDAfterZeroTimeEvent;

    SC_METHOD(ActiveDataLineMethod);
    dont_initialize();
    sensitive << mActiveDataLineEvent;

    SC_METHOD(ActiveCommandLineMethod);
    dont_initialize();
    sensitive << mActiveCommandLineEvent;
}

/**@brief Calculate synchronous time
 *
 */
MMC44::~MMC44()
{ //{{{
    delete mMMC44_Func;
}

/**@brief Process when hard reset is asserted
 *
 */
void MMC44::EnableReset(bool isActive)
{ //{{{
    mMMC44_Func->AssertReset(isActive);
    Initialize();
    CancelEvents();
}

/**@brief Initialize model when hard reset is asserted
 *
 */
void MMC44::Initialize()
{ //{{{
    mINTValue = true;
    mDMAValue = true;
    mCommandControl = 0x0;
    mDataControl = 0x0;
    mIsInternalWriteCMDControl = false;
    mIsInternalWriteDataControl = false;
    mIsStopClock = true;
    mIsBootClock = false;
    mIsCmdResBusy = true;
    mIsHaveBusySignal = false;
    mTimeResponseStart = 0;
    mTimeResponseEnd = 0;
    mTimeCRCREnd = 0;
    mTimeCRCRStart = 0;
    mTimeDATAREnd = 0;
    mTimeDATARStart = 0;
    mTimeBAREnd = 0;
    mTimeBARStart = 0;
    mCountDataCardSendToBuffer = 0;
    mIsWaitForEndBusyCMD12ToEnd = false;
    mIsHandleBusyCMD12 = false;

    mCommandFramePointer = NULL;
    mDataFramePointer = NULL;
    mNumOfBufferToSend = emBuffer1;
    mExpectCommandLine = emDefaultCommandInfo;
    mExpectDataLineSignal = emDefaultDataSignal;
}

/**@brief Cancel events
 *
 */
void MMC44::CancelEvents()
{ //{{{
    mStartSendCommandEvent.cancel();
    mEndSendCommandEvent.cancel();
    mStartHandlerResponseEvent.cancel();
    mEndHandlerResponseEvent.cancel();

    mSendCMD12Event.cancel();
    mBuffer1SendCompletedEvent.cancel();
    mBuffer2SendCompletedEvent.cancel();
    mAssertInterruptEvent.cancel();   // event to assert interrupt
    mDeAssertInterruptEvent.cancel(); // event to deassert interrupt
    mSetCLKEvent.cancel();
    for (uint32_t index = 0; index < emSizeOfTimeOutAndErrorSttRegister; index++)
        mSetTimeOutAndErrStt[index].cancel();

    mIntCMD12CRESetEvent.cancel();
    mIntDTRANESetEvent.cancel();
    mIntBUFRESetEvent.cancel();
    mIntBUFWENSetEvent.cancel();
    mIntCRSPESetEvent.cancel();
    mBuffer1ReadEnableEvent.cancel();
    mBuffer2ReadEnableEvent.cancel();
    mStartCRCREvent.cancel();
    mEndCRCREvent.cancel();
    mStartBusyEvent.cancel();
    mEndBusyEvent.cancel();
    mStartDATAREvent.cancel();
    mEndDATAREvent.cancel();
    mStartBAREvent.cancel();
    mEndBAREvent.cancel();
    mBootOperationEndEvent.cancel();
    mIdleCommandEvent.cancel();
    mIdleDataEvent.cancel();

    mReadDataFromBufferProcessEvent.cancel();
    mWriteDataToBufferProcessEvent.cancel();
    mUpdateSttForEndCmdResEvent.cancel();
    mOneBlockHadReadEvent.cancel();
    mOneBlockHadWrittenEvent.cancel();
    mPtrPortWriteDATAEvent.cancel();
    mMMCBusIsNotBusyEvent.cancel();
    mStartSendDataToCardEvent.cancel();
    mWriteCtrlCMDAfterZeroTimeEvent.cancel();
    mWriteCtrlDATAfterZeroTimeEvent.cancel();
    mActiveDataLineEvent.cancel();
    mActiveCommandLineEvent.cancel();


    mPtrPortWriteCmdEvent.notify();
    mCtrlPortWriteCmdEvent.notify();
    mPtrPortWriteDATAEvent.notify();
    mCtrlPortWriteDATAEvent.notify();
    mSetCLKEvent.notify();
    mWriteDMAPortEvent.notify(SC_ZERO_TIME);
    mWriteInterruptSignalEvent.notify(SC_ZERO_TIME);
    mCancelIsInternalWriteCMDEvent.notify(SC_ZERO_TIME);
    mCancelIsInternalDATACMDEvent.notify(SC_ZERO_TIME);

}

/**@brief
 *
 */
void MMC44::CancelEventsZeroClock(void)
{ //{{{
    mStartSendCommandEvent.cancel();
    mEndSendCommandEvent.cancel();
    mStartHandlerResponseEvent.cancel();
    mEndHandlerResponseEvent.cancel();

    mSendCMD12Event.cancel();
    mBuffer1SendCompletedEvent.cancel();
    mBuffer2SendCompletedEvent.cancel();
    mAssertInterruptEvent.cancel();   // event to assert interrupt
    mDeAssertInterruptEvent.cancel(); // event to deassert interrupt
    for (uint32_t index = 0; index < emSizeOfTimeOutAndErrorSttRegister; index++)
        mSetTimeOutAndErrStt[index].cancel();

    mIntCMD12CRESetEvent.cancel();
    mIntDTRANESetEvent.cancel();
    mIntBUFRESetEvent.cancel();
    mIntBUFWENSetEvent.cancel();
    mIntCRSPESetEvent.cancel();
    mBuffer1ReadEnableEvent.cancel();
    mBuffer2ReadEnableEvent.cancel();
    mStartCRCREvent.cancel();
    mEndCRCREvent.cancel();
    mStartBusyEvent.cancel();
    mEndBusyEvent.cancel();
    mStartDATAREvent.cancel();
    mEndDATAREvent.cancel();
    mStartBAREvent.cancel();
    mEndBAREvent.cancel();
    mBootOperationEndEvent.cancel();
    mIdleCommandEvent.cancel();
    mIdleDataEvent.cancel();

    mReadDataFromBufferProcessEvent.cancel();
    mWriteDataToBufferProcessEvent.cancel();
    mUpdateSttForEndCmdResEvent.cancel();
    mOneBlockHadReadEvent.cancel();
    mOneBlockHadWrittenEvent.cancel();
    mPtrPortWriteDATAEvent.cancel();
    mMMCBusIsNotBusyEvent.cancel();
    mStartSendDataToCardEvent.cancel();
}

//-------------------------------------------------------------------------------------------------
/*                                               Methods                                         */
//-------------------------------------------------------------------------------------------------
/**@brief
 * Set MMC bus Is not busy
 */
void MMC44::MMCBusIsNotBusyMethod(void)
{ //{{{
    mMMC44_Func->SetMMCBUSBSY(false);
} //}}}

/**@brief
 * Update state and bit status of model when End CMD/RESPONSE.
 */
void MMC44::UpdateSttForEndCmdResMethod(void)
{ //{{{
    // set busy end flag
    if (mIsHaveBusySignal)
    {
        mIsHaveBusySignal = false;
        mMMC44_Func->SetInterruptFlagRegister(emRBSYE, 1); // Reception of a response and response busy have been completed.
        if ((bool)mMMC44_Func->GetInterruptEnable(emMRBSYE))
        {
            AssertInterrupt();
        }
    }

    if (mMMC44_Func->GetCommandSetting(emWDAT) == 0)
    {
        mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
        mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
    }
    else if (!((bool)mMMC44_Func->GetCommandSetting(emDWEN)))
    {
        mExpectDataLineSignal = emDATR;
        if ((bool)mMMC44_Func->GetCommandSetting(emBOOT))
        {
            AssertSetTimeoutAndErrStt(em1STBTDATTO);
        }
    }
} //}}}

/**@brief
 * Write DMA port
 */
void MMC44::WriteDMAPortMethod(void)
{ //{{{
    re_printf("info", "DMA port written to %d", mDMAValue);
    mMMC44_Func->SaveDMAValue(mDMAValue);
    DMA.write(mDMAValue);
} //}}}

/**@brief
 * CPU or DMA read data from Buffer
 */
void MMC44::ReadDataFromBufferMethod(void)
{ //{{{

    mMMC44_Func->ReadDataFromBufferProcess();
} //}}}

/**@brief
 * CPU or DMA write data to Buffer
 */
void MMC44::WriteDataToBufferMethod(void)
{ //{{{
    mMMC44_Func->WriteDataToBufferProcess();
} //}}}

/**@brief
 * Write to data_pointer port
 */
void MMC44::WriteDataPtrPortMethod(void)
{ //{{{
    data_pointer.write(mDataFramePointer);
} //}}}

/**@brief
 *  Write to data_control port
 */
void MMC44::WriteDataCtrlPortMethod(void)
{ //{{{
    mIsInternalWriteDataControl = true;
    data_control.write(mDataControl);
} //}}}

/**@brief
 * Cancel Internal Write Data
 */
void MMC44::CancelIsInternalWriteDATAMethod(void)
{ //{{{
    mIsInternalWriteDataControl = false;
} //}}}

/**@brief
 * Set state of data line to active
 */
void MMC44::ActiveDataLineMethod(void)
{ //{{{
    // set data line state
    mMMC44_Func->SetDataLineState(emActive);
} //}}}


/**@brief
 * Cancel Internal Write Command
 */
void MMC44::CancelIsInternalWriteCMDMethod(void)
{ //{{{
    mIsInternalWriteCMDControl = false;
} //}}}

/**@brief
 * Write to cmd_pointer port
 */
void MMC44::WriteCmdPtrPortMethod(void)
{ //{{{
    cmd_pointer.write(mCommandFramePointer);
} //}}}

/**@brief
 * Set state of command line to active
 */
void MMC44::ActiveCommandLineMethod(void)
{ //{{{
    mMMC44_Func->SetMMC44Status(emCMDSIG, 1);
} //}}}

/**@brief
 * Write to cmd_control port
 */
void MMC44::WriteCmdCtrlPortMethod(void)
{ //{{{
    mIsInternalWriteCMDControl = true;
    cmd_control.write(mCommandControl);
} //}}}

/**@brief
 * Method to idle command (set stbe in cmd_control to 0)
 */
void MMC44::IdleCommandMethod(void)
{ //{{{
    mCommandControl = mMMC44_Func->GetCommandControlValue(emIdle, emDefaultCommandInfo);
    mCtrlPortWriteCmdEvent.notify(CalculateClockEdge("CLK"));
    mCancelInternalWriteCMDAfterZeroTimeEvent.notify(CalculateClockEdge("CLK"));
} //}}}

/**@brief
 *  Method to idle data (set stbe in cmd_control to 0)
 */
void MMC44::IdleDataMethod(void)
{ //{{{
    mDataControl = mMMC44_Func->GetDataControlValue(emIdle);
    mCtrlPortWriteDATAEvent.notify(CalculateClockEdge("CLK"));
    mCancelInternalWriteDataAfterZeroTimeEvent.notify(CalculateClockEdge("CLK"));
} //}}}

/**@brief
 * Start Receive CRC status from card
 */
void MMC44::StartCRCRMethod(void)
{ //{{{
    mMMC44_Func->SetDataLineState(emActiveLine0);
    mSetTimeOutAndErrStt[emCRCSTTO].cancel();

} //}}}

/**@brief
 * End Receive CRC status from card
 */
void MMC44::EndCRCRMethod(void)
{ //{{{
    mMMC44_Func->SetDataLineState(emDeActive);
    uint8_t *ptr = data_pointer.read();
    uint8_t *crcR = 0;
    uint8_t endBit = 0;
    if (ptr != NULL)
    {
        crcR = ptr + emStartBitLength;
        endBit = *(ptr + emStartBitLength + emCRCRLength);
    }

    // update crcst in host register
    if(crcR != NULL){
        mMMC44_Func->SetTimeOutAndErrSttBits(emCRCST, *crcR);
    }
    // check crcst
    if ((crcR != NULL) && (*crcR != 0x2))
    {
        AssertSetTimeoutAndErrStt(emCRCSTE_H);
    }
    if (endBit != 0x1)
    {
        AssertSetTimeoutAndErrStt(emCRCSTEBE);
    }
    mExpectDataLineSignal = emBUSYR;
    mIsCmdResBusy = false;
} //}}}

/**@brief
 * Start Receive BUSY signal from card
 */
void MMC44::StartBusyMethod(void)
{ //{{{
    mMMC44_Func->SetDataLineState(emActiveLine0);

    // busy function

    if (mIsCmdResBusy)
    {
        if (mMMC44_Func->IsCMD12())
        {
            // This bit is set to 1 if busy state remains unchanged after the period set by the SRBSYTO bits in MMCAnCE_CLK_CTRL after the automatically-issued CMD12 was transmitted.
            AssertSetTimeoutAndErrStt(emAC12BSYTO);
            mIsHandleBusyCMD12 = true;
        }
        else
        {
            // This bit is set to 1 if busy state remains unchanged after the period set by the SRBSYTO bits in MMCAnCE_CLK_CTRL after a command (other than automatically-issued CMD12) was transmitted.
            AssertSetTimeoutAndErrStt(emRSPBSYTO);
        }
    }
    else
    {
        // This bit is set to 1 if busy status remains unchanged after the period set by the SRWDTO bits in MMCAnCE_CLK_CTRL after the CRC status was received.
        AssertSetTimeoutAndErrStt(emDATBSYTO);
    }

} //}}}

/**@brief
 * End Receive BUSY signal from card
 */
void MMC44::EndBusyMethod(void)
{ //{{{
    mMMC44_Func->SetDataLineState(emDeActive);

    if (mIsCmdResBusy)
    {
        if (mMMC44_Func->IsCMD12())
        {
            mMMC44_Func->SetInterruptFlagRegister(emCMD12RBE, 1);
            // busy end for CMD12
            // CMD12RBE: Automatic CMD12 Issuance Response Busy Complete
            if ((bool)mMMC44_Func->GetCommandSetting(emDWEN))
            {
                mSetTimeOutAndErrStt[emAC12BSYTO].cancel();
                mMMC44_Func->SetInterruptFlagRegister(emDTRANE, 1);
                mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
                mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
                mMMC44_Func->SetIsCMD12Flag(false);
                if ((bool)((mMMC44_Func->GetInterruptEnable(emMCMD12RBE)) || (mMMC44_Func->GetInterruptEnable(emMDTRANE))))
                {
                    AssertInterrupt();
                }
            }
            else
            {
                if (mIsWaitForEndBusyCMD12ToEnd)
                {
                    mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
                    mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
                    mMMC44_Func->SetInterruptFlagRegister(emCMD12DRE, 1);
                    if ((bool)((mMMC44_Func->GetInterruptEnable(emMCMD12DRE)) || (mMMC44_Func->GetInterruptEnable(emMCMD12RBE))))
                    {
                        AssertInterrupt();
                    }
                    mIsWaitForEndBusyCMD12ToEnd = false;
                }
            }
        }
        else
        {
            // busy end for command
            mSetTimeOutAndErrStt[emRSPBSYTO].cancel();
            mUpdateSttForEndCmdResEvent.notify();
        }
    }
    else
    {
        // busy end for data
        if ((bool)mMMC44_Func->GetCommandSetting(emCMD12EN))
        {
            // CMD12 is issued after the data busy after transmission of the last block has ended.
            mSendCMD12Event.notify(SC_ZERO_TIME);
        }
        else
        {
            mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
            mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
        }
        mIntDTRANESetEvent.notify(SC_ZERO_TIME);
        mSetTimeOutAndErrStt[emDATBSYTO].cancel();
    }
} //}}}

/**@brief
 * Start Receive Boot ACK signal from card
 */
void MMC44::StartBARMethod(void)
{ //{{{
    mMMC44_Func->SetDataLineState(emActiveLine0);
    mSetTimeOutAndErrStt[emBTACKTO].cancel();
} //}}}

/**@brief
 * End Receive Boot ACK signal from card
 */
void MMC44::EndBARMethod(void)
{ //{{{
    mMMC44_Func->SetDataLineState(emDeActive);

    uint8_t *ptr = data_pointer.read();
    uint8_t ackPat = 0;
    uint8_t endBit = 0;
    if (ptr != NULL)
    {
        ackPat = *(ptr + emStartBitLength);
        endBit = *(ptr + emStartBitLength + emBARLength);
    }

    // update crcst in host register
    mMMC44_Func->SetTimeOutAndErrSttBits(emCRCST, ackPat);

    if (!(ackPat == 0x2))
    {
        AssertSetTimeoutAndErrStt(emBTACKPATE);
    }

    if (endBit != 1)
    {
        AssertSetTimeoutAndErrStt(emBTACKEBE);
    }
    mIntCRSPESetEvent.notify();
    mExpectDataLineSignal = emDATR;
} //}}}

/**@brief
 * Start Receive Data signal from card
 */
void MMC44::StartDATARMethod(void)
{ //{{{
    uint32_t numBlockReaded = mMMC44_Func->GetMMC44Status(emRCVBLK);
    uint32_t numBlockSetting = mMMC44_Func->GetTransferBlockSetting(emBLKCNT);
    bool IsReadingBuffer = mMMC44_Func->IsReadingBuffer();
    bool IsFirstFrameData = false;

    if ((numBlockReaded == 0) && (mMMC44_Func->GetBufferState(emBuffer1) == emCanWrite) && (mMMC44_Func->GetBufferState(emBuffer2) == emCanWrite))
    {
        IsFirstFrameData = true;
    }

    if (mMMC44_Func->GetBufferState(emBuffer1) == emCanWrite)
    {
        mMMC44_Func->SetBufferState(emBuffer1, emSending);
    }
    else if (mMMC44_Func->GetBufferState(emBuffer2) == emCanWrite)
    {
        mMMC44_Func->SetBufferState(emBuffer2, emSending);
    }
    // set status
    mMMC44_Func->SetDataLineState(emActive);

    // cancel timeout counter
    if ((bool)mMMC44_Func->GetCommandSetting(emBOOT))
    {
        if (IsFirstFrameData)
        {
            mSetTimeOutAndErrStt[em1STBTDATTO].cancel();
        }
        else
        {
            // Interval between Boot Data Timeout cancel
            mSetTimeOutAndErrStt[emBTDATTO].cancel();
        }
    }
    else
    {
        //Read Data Timeout cancel (valid other than in boot operations)
        mSetTimeOutAndErrStt[emSTRDATTO].cancel();
        // cancel boot data time out
    }

    if ((numBlockSetting == (numBlockReaded + 1)) || ((numBlockSetting == (numBlockReaded + 2)) && (IsReadingBuffer)))
    {
        //CMD12 is issued when multi-block transfer is performed with the CMD12EN bit in MMCAnCE_CMD_SET set to 1.
        if (((bool)mMMC44_Func->GetCommandSetting(emCMLTE)) && ((bool)mMMC44_Func->GetCommandSetting(emCMD12EN)))
        {
            // CMD12 is issued so that the end bit of the command is sent two bits before the end bit of the data during reception of the last block.
            mSendCMD12Event.notify(double((mMMC44_Func->GetTimeDataTrans()) - (6 * 8) - 2) * (double)mCLK_Period, mTimeResolutionUnit);
        }
    }
} //}}}

/**@brief
 * End Receive Data signal from card
 */
void MMC44::EndDATARMethod(void)
{ //{{{
    mMMC44_Func->SetDataLineState(emDeActive);

    uint32_t dataLength = mMMC44_Func->GetTransferBlockSetting(emBLKSZ);
    uint8_t *ptr = data_pointer.read();
    uint8_t *ptrData = 0;
    uint8_t crc16_byte1 = 0;
    uint8_t crc16_byte2 = 0;
    uint16_t crc16 = 0;
    uint8_t endBit = 0;
    if (ptr != NULL)
    {
        ptrData = ptr + emStartBitLength;
        crc16_byte1 = *(ptr + emStartBitLength + dataLength);
        crc16_byte2 = *(ptr + emStartBitLength + dataLength + 1);
        crc16 = crc16_byte2;
        crc16 <<= 8;
        crc16 |= crc16_byte1;
        endBit = *(ptr + emStartBitLength + dataLength + emCRC16Length);
    }

    assert(ptrData != NULL);
    mMMC44_Func->SendDataToBuffer(ptrData);
    if (mMMC44_Func->GetCommandSetting(emCRC16C) == 0)
    {
        if (mMMC44_Func->ComputeCRC16(ptrData, dataLength) != crc16)
        {
            AssertSetTimeoutAndErrStt(emCRC16E);
        }
    }

    if (endBit != 0x1)
    {
        AssertSetTimeoutAndErrStt(emRDATEBE);
    }

} //}}}

/**@brief
 * Handler data_control port
 */
void MMC44::HandlerDataControlMethod(void)
{ //{{{
    if (!mIsInternalWriteDataControl)
    {
        if ((bool)mMMC44_Func->GetMMC44Status(emCMDSEQ))
        {
            uint32_t dataControlValue = data_control.read();
            uint16_t size = (dataControlValue >> 16) & 0xFFFF;
            eDataLineSignal info = eDataLineSignal(
                (dataControlValue >> 4) & 0xF);
            eSTBE stbe = eSTBE(dataControlValue & 0x3);
            uint8_t *Ptr = data_pointer.read();
            uint8_t startBitData = 0;
            if (Ptr != NULL)
            {
                startBitData = *Ptr;
            }
            std::string ExpectSignalString = "Not expect";

            if (mExpectDataLineSignal == emCRCR)
            {
                ExpectSignalString = "CRCR" ;
            }
            else if (mExpectDataLineSignal == emBUSYR)
            {
                ExpectSignalString = "BUSYR" ;
            }
            else if (mExpectDataLineSignal == emDATR)
            {
                ExpectSignalString = "DATR" ;
            }
            else if (mExpectDataLineSignal == emBAR)
            {
                ExpectSignalString = "BAR" ;
            }

            if (startBitData == 0x0)
            {
                if (info != mExpectDataLineSignal)
                {
                    re_printf("warning", "Data line signal is not correct (%s expected) .\n", ExpectSignalString.c_str());
                }
                else if (info == emCRCR)
                {
                    if (size != 0x3)
                    {
                        re_printf("warning", "CRC:R size receive not match with setting\n");
                    }
                    else
                    {
                        if (stbe == emStart)
                        {
                            re_printf("info", "Start receive CRC:R from card\n");
                            mTimeCRCRStart = sc_time_stamp().to_double();
                            mStartCRCREvent.notify();
                        }
                        else if (stbe == emStop)
                        {
                            re_printf("info", "End receive CRC:R from card\n");
                            mTimeCRCREnd = sc_time_stamp().to_double();
                            mEndCRCREvent.notify();
                            double rTime = mTimeCRCREnd - mTimeCRCRStart;
                            double time = double(size * 8 * (double)mCLK_Period);

                            if (rTime != time)
                            {
                                re_printf("warning", "Received CRC:R expected transfer in %f ps But that frame received in %f ps \n", time, rTime);
                            }
                        }
                    }
                }
                else if (info == emBUSYR)
                {
                    if (stbe == emStart)
                    {
                        re_printf("info", "Start receive BUSY signal from card\n");
                        mStartBusyEvent.notify();
                    }
                    else if (stbe == emStop)
                    {
                        re_printf("info", "End receive BUSY signal from card\n");
                        mEndBusyEvent.notify();
                    }
                }
                else if (info == emDATR)
                {
                    if (size != (mMMC44_Func->GetTransferBlockSetting(emBLKSZ) + emStartBitLength + emCRC16Length + emEndBitLength))
                    {
                        re_printf("warning", "Data Block size receive not match with setting\n");
                    }
                    else
                    {
                        if (stbe == emStart)
                        {
                            re_printf("info", "Start receive DAT:R from card\n");
                            mTimeDATARStart = sc_time_stamp().to_double();
                            mStartDATAREvent.notify();
                            mMMC44_Func->SetTimeDataTrans(mMMC44_Func->GetTransferBlockSetting(emBLKSZ));
                        }
                        else if (stbe == emStop)
                        {
                            re_printf("info", "End receive DAT:R from card\n");
                            mTimeDATAREnd = sc_time_stamp().to_double();
                            mEndDATAREvent.notify();
                            double rTime = (mTimeDATAREnd - mTimeDATARStart);
                            double time = double(size * 8 * (double)mCLK_Period);

                            if (rTime != time)
                            {
                                time = mMMC44_Func->GetTimeDataTrans() * (double)mCLK_Period;
                                re_printf("warning", "Received DAT:R expected transfer in %f ps But that frame received in %f ps \n", time, rTime);
                            }
                        }
                    }
                }
                else if (info == emBAR)
                {
                    if (size != 0x3)
                    {
                        re_printf("warning", "BA:R size receive not match with setting\n");
                    }
                    else
                    {
                        if (stbe == emStart)
                        {
                            re_printf("info", "Start receive BA:R from card\n");
                            mTimeBARStart = sc_time_stamp().to_double();
                            mStartBAREvent.notify();
                        }
                        else if (stbe == emStop)
                        {
                            re_printf("info", "End receive BA:R from card\n");
                            mTimeBAREnd = sc_time_stamp().to_double();
                            mEndBAREvent.notify();
                            double rTime = mTimeBAREnd - mTimeBARStart;
                            double time = double(size * 8 * (double)mCLK_Period);
                            if (rTime != time)
                            {
                                re_printf("warning", "Received BA:R expected transfer in %f ps But that frame received in %f ps \n", time, rTime);
                            }
                        }
                    }
                }
            }
        }
    }
} //}}}
/**@brief
 * Handler cmd_control port
 */
void MMC44::HandlerCMDControlMethod(void)
{ //{{{
    if (!mIsInternalWriteCMDControl)
    {
        if ((bool)mMMC44_Func->GetMMC44Status(emCMDSEQ))
        {
            uint32_t cmdControlValue = cmd_control.read();
            uint16_t size = cmdControlValue >> 16;
            eCommandLineSignal info = eCommandLineSignal(
                (cmdControlValue >> 4) & 0xF);
            eSTBE stbe = eSTBE(cmdControlValue & 0x3);
            std::string ExpectSignalString = "Not expect";

            if (mExpectCommandLine == emResponseInfo)
            {
                ExpectSignalString = "Response";
            }



            if (info != mExpectCommandLine)
            {
                re_printf("warning", "Not correct signal from CMD line (%s expected) .\n", ExpectSignalString.c_str());
            }
            else if (info == emResponseInfo)
            {
                if (mMMC44_Func->CheckResponseSize(size))
                {
                    if (stbe == emStart)
                    {
                        re_printf("info", "Start receive Response frame from card.\n");
                        mTimeResponseStart = sc_time_stamp().to_double();
                        mStartHandlerResponseEvent.notify(SC_ZERO_TIME);
                    }
                    else if (stbe == emStop)
                    {
                        re_printf("info", "End receive Response frame from card.\n");
                        mTimeResponseEnd = sc_time_stamp().to_double();
                        double rTime = mTimeResponseEnd - mTimeResponseStart;
                        double time = double(size * 8 * (double)mCLK_Period);
                        if (rTime != time)
                        {

                            re_printf("warning", "Received Response expected transfer in %f ps But that frame received in %f ps \n", time, rTime);
                        }
                        mEndHandlerResponseEvent.notify(SC_ZERO_TIME);
                    }
                }
                else
                {
                    re_printf("warning", "Response frame error(model is not configure to receive Response or size of response frame is incorrect). \n");
                }
            }
        }
    }
} //}}}

/**@brief
 * Start send command to card
 */
void MMC44::StartSendCommandMethod(void)
{ //{{{

    re_printf("info", "Start Send Command To Card\n");

    mCommandFramePointer = mMMC44_Func->IniCMDFrame(mMMC44_Func->IsCMD12());

    if ((bool)mMMC44_Func->GetCommandSetting(emBOOT))
    {
        // boot operation, expect BA:R signal send from data line
        re_printf("info", "boot operation, waiting BA:R signal send from data line");
        if ((bool)mMMC44_Func->GetCommandSetting(emBOOTACK))
        {
            mCommandControl = mMMC44_Func->GetCommandControlValue(emStart, emKeepLowInfo);
        }
    }
    else
    {
        mCommandControl = mMMC44_Func->GetCommandControlValue(emStart, emCommandInfo);
        mEndSendCommandEvent.notify(CalculateClockEdge("CLK").to_double() + emCommandSize * 8 * (double)mCLK_Period, mTimeResolutionUnit);
    }
    mPtrPortWriteCmdEvent.notify(CalculateClockEdge("CLK"));
    mWriteCtrlCMDAfterZeroTimeEvent.notify(CalculateClockEdge("CLK"));
    mActiveCommandLineEvent.notify(CalculateClockEdge("CLK"));
} //}}}

/**@brief
 * Write Data_Control port after SC_ZERO_TIME.
 */
void MMC44::WriteCtrlDATAfterZeroTimeMethod(void)
{ //{{{
    mCtrlPortWriteCmdEvent.notify(SC_ZERO_TIME);
} //}}}

/**@brief
 * End send command to card
 */
void MMC44::EndSendCommandMethod(void)
{ //{{{
    re_printf("info", "End Send Command To Card\n");
    mMMC44_Func->SetMMC44Status(emCMDSIG, 0);

    mCommandControl = mMMC44_Func->GetCommandControlValue(emStop, emCommandInfo);
    mWriteCtrlCMDAfterZeroTimeEvent.notify(CalculateClockEdge("CLK"));
    // no response
    if (mMMC44_Func->IsCMD12())
    {
        mExpectCommandLine = emResponseInfo;
        AssertSetTimeoutAndErrStt(emAC12RSPTO);
    }
    else if (mMMC44_Func->GetCommandSetting(emRTYP) == 0)
    {
        mIntCRSPESetEvent.notify();
        mExpectCommandLine = emDefaultCommandInfo;
    }
    else
    {
        mExpectCommandLine = emResponseInfo;
        AssertSetTimeoutAndErrStt(emSTRSPTO);
    }

    // busy signal have expected after 2 clock of command frame  6*8 + 2
    mExpectDataLineSignal = emBUSYR;
    mIsCmdResBusy = true;

    mIdleCommandEvent.notify(CalculateClockEdge("CLK") + sc_time((double)mCLK_Period, mTimeResolutionUnit));

    // This bit is set to 1 if read data is not received within the period set by the
    // SRWDTO bits in MMCAnCE_CLK_CTRL after a read command was
    // transmitted.

    if (((bool)mMMC44_Func->GetCommandSetting(emWDAT)) && (mMMC44_Func->GetCommandSetting(emDWEN) == 0) && (!mMMC44_Func->IsCMD12()))
    {
        AssertSetTimeoutAndErrStt(emSTRDATTO);
    }
} //}}}

/**@brief
 * Start Receive Response from card
 */
void MMC44::StartResponseMethod(void)
{ //{{{
    // cancel time out
    mMMC44_Func->SetMMC44Status(emCMDSIG, 1);
    mSetTimeOutAndErrStt[emAC12RSPTO].cancel();
    mSetTimeOutAndErrStt[emSTRSPTO].cancel();

} //}}}

/**@brief
 * End receive Response from card
 */
void MMC44::EndResponseMethod(void)
{ //{{{
    mMMC44_Func->SetMMC44Status(emCMDSIG, 0);

    uint8_t *response_ptr = cmd_pointer.read();
    if (response_ptr != NULL)
    {
        mMMC44_Func->CheckResponseFrame(response_ptr);
    }
    uint32_t dataControlValue = data_control.read();
    eDataLineSignal info = eDataLineSignal((dataControlValue >> 4) & 0xF);
    eSTBE stbe = eSTBE(dataControlValue & 0x3);

    if ((info == emBUSYR) && (stbe == emStart) )
    {
        mIsHaveBusySignal = true;
    }

    if (!mMMC44_Func->IsCMD12())
    {
        // command response completed (receive response frame completed)
        mIntCRSPESetEvent.notify();
    }
    else
    {
        // CMD12CRE: Automatic CMD12 Response Complete
        mIntCMD12CRESetEvent.notify();
    }
} //}}}
/**@brief
 * Handle Set time out and error status bit
 */
void MMC44::HandleSetTimeOutAndErrStt(const uint32_t bit_name)
{ //{{{
    mMMC44_Func->SetTimeOutAndErrSttBits(bit_name, 1);
    // Deassert DMA request
    if (((bool)mMMC44_Func->GetDMATransferSetting()) && (DMA.read() == 0))
    {
        ErrorOccuredDuringDMATransfer();
    }
} //}}}

/**@brief
 * Set CLK method
 */
void MMC44::SetCLKMethod(void)
{ //{{{
    if ((mIsStopClock == true) || (!mMMC44_Func->GetClockControl(emCLKEN)))
    {
        re_printf("info", "Does not output the MMC clock (fixed to low level)");
        // Does not output the MMC clock (fixed to low level).
        CLK.write(0x0);
        SetCLKFreq("CLK", CLK.read(), "Hz");
    }
    else if (mIsBootClock)
    {
        uint64_t clkFreq = mCLK_HSBOrgFreq / (((uint64_t)2) << (mMMC44_Func->GetBootOpSetting(emBTCLKDIV)));
        CLK.write(clkFreq);
        SetCLKFreq("CLK", clkFreq, "Hz");
        // start calculate BA:R timeout
        AssertSetTimeoutAndErrStt(emBTACKTO);
        mExpectDataLineSignal = emBAR;
    }
    else
    {
        // MMC Clock Frequency Setting
        uint64_t clkFreq = mCLK_HSBOrgFreq / (((uint64_t)2) << (mMMC44_Func->GetClockControl(emCLKDIV)));
        CLK.write(clkFreq);
        SetCLKFreq("CLK", clkFreq, "Hz");
    }

} //}}}

/**@brief
 * buffer read end
 */
void MMC44::IntBUFREMethod(void)
{ //{{{
    mMMC44_Func->SetInterruptFlagRegister(emBUFRE, 1);
    mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
    mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
    mMMC44_Func->SetBufferState(emBuffer1, emCanWrite);
    mMMC44_Func->SetBufferState(emBuffer2, emCanWrite);
    mIsHandleBusyCMD12 = false;
    if ((bool)mMMC44_Func->GetInterruptEnable(emMBUFRE))
    {
        AssertInterrupt();
    }
} //}}}

/**@brief
 * CMD12 command/response end
 */
void MMC44::IntCMD12CREMethod(void)
{ //{{{
    mMMC44_Func->SetInterruptFlagRegister(emCMD12CRE, 1);
    if ((!mIsHaveBusySignal) && mMMC44_Func->GetCommandSetting(emDWEN))
    {
        mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
        mMMCBusIsNotBusyEvent.notify(10 * (double) mCLK_Period, mTimeResolutionUnit);
    }
    if ((bool)mMMC44_Func->GetInterruptEnable(emMCMD12CRE))
    {
        AssertInterrupt();
    }
} //}}}

/**@brief
 * data transmission completed
 */
void MMC44::IntDTRANEMethod(void)
{ //{{{
    mMMC44_Func->SetInterruptFlagRegister(emDTRANE, 1);
    if ((bool)mMMC44_Func->GetInterruptEnable(emMDTRANE))
    {
        AssertInterrupt();
    }
} //}}}

/**@brief
 * buffer write enabled
 */
void MMC44::IntBUFWENMethod(void)
{ //{{{
    mMMC44_Func->SetInterruptFlagRegister(emBUFWEN, 1);
    if ((bool)mMMC44_Func->GetInterruptEnable(emMBUFWEN))
    {
        AssertInterrupt();
    }
} //}}}

/**@brief  Command/Response completed (not boot) method
 */
void MMC44::IntCRSPEMethod(void)
{ //{{{
    // set command response end flag
    mMMC44_Func->SetInterruptFlagRegister(emCRSPE, 1);

    if (!mIsHaveBusySignal)
    {
        mUpdateSttForEndCmdResEvent.notify(SC_ZERO_TIME);
    }
    if ((bool)mMMC44_Func->GetInterruptEnable(emMCRSPE))
    {
        AssertInterrupt();
    }
} //}}}

/**@brief
 * buffer1 read enable (Receive 1 block of data to Buffer1)
 */
void MMC44::Buffer1ReadEnableMethod(void)
{ //{{{
    mMMC44_Func->SetBufferState(emBuffer1, emCanRead);
    mMMC44_Func->UpdateReadDataPtr(emCanRead);
    // Start calculate time out

    eBufferState buf1State = mMMC44_Func->GetBufferState(emBuffer1);
    eBufferState buf2State = mMMC44_Func->GetBufferState(emBuffer2);
    uint32_t numBlkReaded = mMMC44_Func->GetMMC44Status(emRCVBLK);
    uint32_t numBlkSetting = mMMC44_Func->GetTransferBlockSetting(emBLKCNT);
//    bool IsReceiveAllBlock = false;

    if (!((bool)mMMC44_Func->GetCommandSetting(emBOOT)))
    {
        AssertSetTimeoutAndErrStt(emSTRDATTO);
    }
    else
    {
        AssertSetTimeoutAndErrStt(emBTDATTO);
    }

    mCountDataCardSendToBuffer++;
    if ((mCountDataCardSendToBuffer == numBlkSetting) || (!mMMC44_Func->GetCommandSetting(emCMLTE)))
    {
        mSetTimeOutAndErrStt[emSTRDATTO].cancel();
        mSetTimeOutAndErrStt[emBTDATTO].cancel();
        mCountDataCardSendToBuffer = 0;
    }

    //stop clock
    if (mMMC44_Func->Is2BufferNotEmpty())
    {
        mIsStopClock = true;
        mSetCLKEvent.notify(CalculateClockEdge("CLK"));
    }

    // Assert interrupt or DMA
    if (mMMC44_Func->IsDMAReadEnable())
    {
        if (DMA.read())
        {
            mDMAValue = 0;
            mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
        }
    }
    else
    {
        mMMC44_Func->SetInterruptFlagRegister(emBUFREN1, 1);

        if ((bool)mMMC44_Func->GetInterruptEnable(emMBUFREN1))
        {
            AssertInterrupt();
        }
    }
} //}}}

/**@brief
 * buffer2 read enable (Receive 1 block of data to Buffer2)
 */
void MMC44::Buffer2ReadEnableMethod(void)
{ //{{{
    mMMC44_Func->SetBufferState(emBuffer2, emCanRead);
    mMMC44_Func->UpdateReadDataPtr(emCanRead);
    // Start calculate time out

    eBufferState buf1State = mMMC44_Func->GetBufferState(emBuffer1);
    eBufferState buf2State = mMMC44_Func->GetBufferState(emBuffer2);
    uint32_t numBlkReaded = mMMC44_Func->GetMMC44Status(emRCVBLK);
    uint32_t numBlkSetting = mMMC44_Func->GetTransferBlockSetting(emBLKCNT);

    if (!((bool)mMMC44_Func->GetCommandSetting(emBOOT)))
    {
        AssertSetTimeoutAndErrStt(emSTRDATTO);
    }
    else
    {
        AssertSetTimeoutAndErrStt(emBTDATTO);
    }

    mCountDataCardSendToBuffer++;
    if ((mCountDataCardSendToBuffer == numBlkSetting))
    {
        mSetTimeOutAndErrStt[emSTRDATTO].cancel();
        mSetTimeOutAndErrStt[emBTDATTO].cancel();
        mCountDataCardSendToBuffer = 0;
    }

    //stop clock
    if (mMMC44_Func->Is2BufferNotEmpty())
    {
        mIsStopClock = true;
        mSetCLKEvent.notify();
    }

    // Assert interrupt or DMA
    if (mMMC44_Func->IsDMAReadEnable())
    {
            mDMAValue = 0;
            mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
    }
    else
    {
        mMMC44_Func->SetInterruptFlagRegister(emBUFREN2, 1);
        if ((bool)mMMC44_Func->GetInterruptEnable(emMBUFREN2))
        {
            AssertInterrupt();
        }
    }

} //}}}

/**@brief
 * Start send CMD12
 */
void MMC44::SendCMD12Method(void)
{ //{{{
    mMMC44_Func->SetIsCMD12Flag(true);
    mStartSendCommandEvent.notify();
} //}}}

/**@brief
 * Send completed 1 block size of data to card from buffer 1
 */
void MMC44::Buffer1SendCompletedMethod(void)
{ //{{{
    // set dataline state
    mMMC44_Func->SetDataLineState(emDeActive);

    mMMC44_Func->SetBufferState(emBuffer1, emCanWrite);
    mMMC44_Func->SetMMC44Status(emRCVBLK,
                                (mMMC44_Func->GetMMC44Status(emRCVBLK) + 1));

    mDataControl = mMMC44_Func->GetDataControlValue(emStop);
    mCtrlPortWriteDATAEvent.notify(CalculateClockEdge("CLK"));

    mIdleDataEvent.notify(sc_time((double)mCLK_Period, mTimeResolutionUnit));

    if ((mMMC44_Func->GetMMC44Status(emRCVBLK) == mMMC44_Func->GetTransferBlockSetting(emBLKCNT)) || (mMMC44_Func->GetCommandSetting(emCMLTE) == 0))
    {
        // all block send
        // wait CRC status or BUSY:R
        if (mMMC44_Func->IsDMAWriteEnable())
        {
            // De assert DMA request
            mDMAValue = 1;
            mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
        }
        if (mMMC44_Func->GetCommandSetting(emCRCSTE) == 0)
        {
            mExpectDataLineSignal = emCRCR;
            AssertSetTimeoutAndErrStt(emCRCSTTO);
        }
        else
        {
            // data transmission completed
            mIntDTRANESetEvent.notify(SC_ZERO_TIME);
            mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
            mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
        }
    }
    else
    {
        if (mMMC44_Func->IsBufferWaitting())
        {
            mNumOfBufferToSend = emBuffer2;
            mStartSendDataToCardEvent.notify(2 * (double)mCLK_Period, mTimeResolutionUnit);
        }
    }
} //}}}

/**@brief
 * Send completed 1 block size of data to card from buffer 2
 */
void MMC44::Buffer2SendCompletedMethod(void)
{ //{{{
    // set dataline state

    mMMC44_Func->SetDataLineState(emDeActive);

    mMMC44_Func->SetBufferState(emBuffer2, emCanWrite);
    mMMC44_Func->SetMMC44Status(emRCVBLK,
                                (mMMC44_Func->GetMMC44Status(emRCVBLK) + 1));

    mDataControl = mMMC44_Func->GetDataControlValue(emStop);
    mCtrlPortWriteDATAEvent.notify(CalculateClockEdge("CLK"));

    mIdleDataEvent.notify(sc_time((double)mCLK_Period, mTimeResolutionUnit));

    if ((mMMC44_Func->GetMMC44Status(emRCVBLK) == mMMC44_Func->GetTransferBlockSetting(emBLKCNT)) || (mMMC44_Func->GetCommandSetting(emCMLTE) == 0))
    {
        // all block send
        // wait CRC status or BUSY:R
        if (mMMC44_Func->IsDMAWriteEnable())
        {
            // De assert DMA request
            mDMAValue = 1;
            mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
        }
        if (mMMC44_Func->GetCommandSetting(emCRCSTE) == 0)
        {
            mExpectDataLineSignal = emCRCR;
            AssertSetTimeoutAndErrStt(emCRCSTTO);
        }
        else
        {
            // data transmission completed
            mIntDTRANESetEvent.notify(SC_ZERO_TIME);
            mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
            mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
        }
    }
    else
    {
        if (mMMC44_Func->IsBufferWaitting())
        {
            mNumOfBufferToSend = emBuffer1;
            mStartSendDataToCardEvent.notify(2 * (double)mCLK_Period, mTimeResolutionUnit);
        }
    }
} //}}}

/**@brief
 * Start Send Data signal to card
 */
void MMC44::StartSendDataToCardMethod()
{ //{{{
    // set data array
    mDataFramePointer = mMMC44_Func->IniDataFrame(mNumOfBufferToSend);
    // set time to delay
    mMMC44_Func->SetTimeDataTrans(mMMC44_Func->GetTransferBlockSetting(emBLKSZ));
    mDataControl = mMMC44_Func->GetDataControlValue(emStart);

    // set state
    mMMC44_Func->SetBufferState(mNumOfBufferToSend, emSending);

    // active data line
    mActiveDataLineEvent.notify(CalculateClockEdge("CLK"));

    mPtrPortWriteDATAEvent.notify(CalculateClockEdge("CLK"));
    mWriteCtrlDATAfterZeroTimeEvent.notify(CalculateClockEdge("CLK"));

    if (((mMMC44_Func->GetMMC44Status(emRCVBLK) + 1) == mMMC44_Func->GetTransferBlockSetting(emBLKCNT)) || (mMMC44_Func->GetCommandSetting(emCMLTE) == 0))
    {
        // do nothing
    }
    else
    {
        if ((mMMC44_Func->GetBufferState(emBuffer1) == emCanWrite) || (mMMC44_Func->GetBufferState(emBuffer2) == emCanWrite))
        {
            if (!mMMC44_Func->IsDMAWriteEnable())
            {
                mIntBUFWENSetEvent.notify(CalculateClockEdge("CLK"));
            }
        }
    }

    if (mNumOfBufferToSend == emBuffer1)
    {
        mBuffer1SendCompletedEvent.notify(CalculateClockEdge("CLK").to_double() + mMMC44_Func->GetTimeDataTrans() * (double)mCLK_Period, mTimeResolutionUnit);
    }
    else
    {
        mBuffer2SendCompletedEvent.notify(CalculateClockEdge("CLK").to_double() + mMMC44_Func->GetTimeDataTrans() * (double)mCLK_Period, mTimeResolutionUnit);
    }
} //}}}

/**@brief
 * Write Command_Control port after SC_ZERO_TIME.
 */
void MMC44::WriteCtrlCMDAfterZeroTimeMethod(void)
{ //{{{
    mCtrlPortWriteDATAEvent.notify(SC_ZERO_TIME);
} //}}}

/**@brief
 * Cancel Internal Write Data after SC_ZERO_TIME.
 */
void MMC44::CancelInternalWriteDataAfterZeroTimeMethod(void)
{ //{{{
    mCancelIsInternalDATACMDEvent.notify(SC_ZERO_TIME);
} //}}}

/**@brief
 * Cancel Internal Write CMD after SC_ZERO_TIME.

 */
void MMC44::CancelInternalWriteCMDAfterZeroTimeMethod(void)
{ //{{{
    mCancelIsInternalWriteCMDEvent.notify(SC_ZERO_TIME);
} //}}}


/**@brief
 * Read completed 1 block size of data from buffer
 */
void MMC44::OneBlockHadReadMethod(void)
{ //{{{
    bool isInterrupEnabled = false;
    if (mMMC44_Func->IsAllBlockHadReaded())
    {
        // boot operation end
        if ((bool)mMMC44_Func->GetCommandSetting(emBOOT))
        {
            mBootOperationEndEvent.notify();
        }
        else
        {
            // buffer read completed
            if ((bool)mMMC44_Func->GetCommandSetting(emCMD12EN))
            {
                if (mMMC44_Func->GetMMC44Status(emDATSIG) != 0)
                {
                    mIsWaitForEndBusyCMD12ToEnd = true;
                    if (mMMC44_Func->IsDMAReadEnable()) // read by DMA
                    {
                        mDMAValue = 1;
                        mMMC44_Func->SetInterruptFlagRegister(emBUFRE, 1);
                        mMMC44_Func->SetBufferState(emBuffer1, emCanWrite);
                        mMMC44_Func->SetBufferState(emBuffer2, emCanWrite);
                        mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
                    }
                    else    // read by cpu
                    {
                        mMMC44_Func->SetInterruptFlagRegister(emBUFRE, 1);
                        mMMC44_Func->SetBufferState(emBuffer1, emCanWrite);
                        mMMC44_Func->SetBufferState(emBuffer2, emCanWrite);
                        if ((bool)mMMC44_Func->GetInterruptEnable(emMBUFRE))
                        {
                            isInterrupEnabled = true;
                        }
                    }
                }
                else {
                    // CMD12DRE: Automatic CMD12 Issuance & Buffer Read Complete
                    if (mIsHandleBusyCMD12)
                    {
                        mMMC44_Func->SetInterruptFlagRegister(emCMD12DRE, 1);
                        if ((bool)mMMC44_Func->GetInterruptEnable(emMCMD12DRE))
                        {
                            isInterrupEnabled = true;
                        }
                    }

                    if (mMMC44_Func->IsDMAReadEnable()) // read by DMA
                    {
                        mDMAValue = 1;
                        mMMC44_Func->SetInterruptFlagRegister(emBUFRE, 1);
                        mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
                        mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
                        mMMC44_Func->SetBufferState(emBuffer1, emCanWrite);
                        mMMC44_Func->SetBufferState(emBuffer2, emCanWrite);
                        mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
                        mIsHandleBusyCMD12 = false;
                    }
                    else    // read by cpu
                    {
                        mIntBUFRESetEvent.notify(SC_ZERO_TIME);
                    }
                }
            }
            else
            {
                if (mMMC44_Func->IsDMAReadEnable()) // read by DMA
                {
                    mDMAValue = 1;
                    mMMC44_Func->SetInterruptFlagRegister(emBUFRE, 1);
                    mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
                    mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
                    mMMC44_Func->SetBufferState(emBuffer1, emCanWrite);
                    mMMC44_Func->SetBufferState(emBuffer2, emCanWrite);
                    mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
                }
                else // Read by cpu
                {
                    mIntBUFRESetEvent.notify(SC_ZERO_TIME);
                }
            }
        }
    }
    else
    {
        mMMC44_Func->UpdateReadDataPtr(emReading);
        // continue clock
        if (mIsStopClock)
        {
            mIsStopClock = false;
            mSetCLKEvent.notify(SC_ZERO_TIME);
        }
    }

    // Assert Interrupt
    if (isInterrupEnabled)
    {
        AssertInterrupt();
    }


} //}}}

/**@brief
 * Write completed 1 block size of data to buffer
 */
void MMC44::OneBlockHadWrittenMethod(void)
{ //{{{
    if (!(mMMC44_Func->IsBufferSending()))
    {
        if (mMMC44_Func->GetBufferState(emBuffer1) == emWritting)
        {
            mNumOfBufferToSend = emBuffer1;
            mStartSendDataToCardEvent.notify();
        }
        else
        {
            mNumOfBufferToSend = emBuffer2;
            mStartSendDataToCardEvent.notify();
        }
    }
    else if (mMMC44_Func->Is2BufferNotEmpty())
    {
        if (mMMC44_Func->GetBufferState(emBuffer1) == emWritting)
        {
            mMMC44_Func->SetBufferState(emBuffer1, emWaiting);
        }
        else
        {
            mMMC44_Func->SetBufferState(emBuffer2, emWaiting);
        }
    }

} //}}}

/**@brief
 * End of Boot operation
 */
void MMC44::BootOperationEndMethod(void)
{ //{{{
    uint32_t control = mMMC44_Func->GetCommandControlValue(emStop, emKeepLowInfo);
    cmd_control.write(control);
    mIdleCommandEvent.notify((double)mCLK_Period, mTimeResolutionUnit);
    // Assert INT
    mIntBUFRESetEvent.notify(emTimeSwitchToCLKInCMDReg * (double)mCLK_Period, mTimeResolutionUnit);
    AssertSetBootCLKEvent();
} //}}}

/**@brief
 * Assert Interrupt method
 */
void MMC44::AssertInterruptMethod(void)
{ //{{{
    mINTValue = false;
    mWriteInterruptSignalEvent.notify();
} //}}}

/**@brief
 * DeAssert Interrupt method
 */
void MMC44::DeAssertInterruptMethod(void)
{ //{{{
    mINTValue = true;
    mWriteInterruptSignalEvent.notify();
} //}}}

/**@brief
 * Write INT signal
 */
void MMC44::WriteInterruptSignalMethod(void)
{ //{{{
    bool bCurrentINTVal = INT.read();
    if(bCurrentINTVal != mINTValue){
        INT.write(mINTValue);
        DumpInterruptMsg("INT", mINTValue);
    }
} //}}}

//------------------------------------------------------------------------------
/**@brief monitor clock
 */
void MMC44::CLK_HSBMethod()
{ //{{{
    sc_dt::uint64 freq_value = CLK_HSB.read();
    SetCLKFreq(CLK_HSB.basename(), freq_value, "Hz");
} //}}}

//------------------------------------------------------------------------------
/**@brief Reset function when reset command is active
 */
void MMC44::resetCmdHandleMethod()
{ //{{{
    mIsResetCmdActive = true;
    re_printf("info", "The model is reset by AssertReset command of %s.\n", PRESETn.basename());
    this->EnableReset(mIsResetCmdActive);
    mCancelResetCmdEvent.notify(mResetCmdPeriod, SC_NS);
} //}}}

//------------------------------------------------------------------------------
/** @brief Cancel reset function when reset command is active
 */
void MMC44::CancelResetCmdMethod()
{ //{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info", "Reset period of %s is over.\n", PRESETn.basename());
    this->EnableReset(mIsResetCmdActive);
} //}}}
//------------------------------------------------------------------------------
/**@brief resetport Handle method
 * When "resetport" is asserted, model is initialized
 */
void MMC44::PRESETnHandleMethod()
{ //{{{
    bool resetport_state = PRESETn.read();

    if (resetport_state == vpcl::emResetActive)
    {
        mIsResetPortActive = true;
        this->EnableReset(mIsResetPortActive);
        re_printf("info", "The reset port %s is asserted.\n", PRESETn.basename());
    }
    else
    { //reset de-active
        mIsResetPortActive = false;
        mMMC44_Func->AssertReset(mIsResetPortActive);
        re_printf("info", "The reset port %s is de-asserted.\n", PRESETn.basename());
    }

    if (mIsResetPortActive && mIsResetCmdReceive)
    {
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
        mResetCmdEvent.cancel();
        mCancelResetCmdEvent.cancel();
    }
} //}}}

//------------------------------------------------------------------------------
// PythonIF
void MMC44::AssertReset(const std::string reset_name, const double start_time, const double period)
{ //{{{
    if (reset_name == PRESETn.basename())
    {
        if ((!mIsResetPortActive) && (!mIsResetCmdReceive))
        {
            mIsResetCmdReceive = true;
            re_printf("info", "The model will be reset (%s) for %f ns after %f ns.\n", PRESETn.basename(), period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        }
        else
        {
            re_printf("warning", "The software reset of %s is called in the reset operation of the model. So it is ignored.\n", PRESETn.basename());
        }
    }
    else
    {
        re_printf("warning", "The reset name (%s) is wrong. It should be %s.\n", reset_name.c_str(), PRESETn.basename());
    }
} //}}}

void MMC44::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq,
                       const std::string unit)
{ //{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1)
    {
        if (word_vector[0] == CLK_HSB.basename())
        {
            mCLK_HSBOrgFreq = freq;
            ConvertClockFreq(mCLK_HSBOrgFreq, mCLK_HSBFreqUnit, freq, unit);
            if (mCLK_HSBOrgFreq > 0)
            {
                mCLK_HSBPeriod =
                    (sc_dt::uint64)(((1 / (double)mCLK_HSBOrgFreq) * (double)mTimeResolutionValue) + 0.5);
                mCLK_HSBSyncTPoint = sc_time_stamp().to_double();
                if ((bool)mMMC44_Func->GetClockControl(emCLKEN))
                {
                    mIsStopClock = false;
                    mSetCLKEvent.notify();
                }
            }
            else
            {
                mCLK_HSBPeriod = 0;
                mCLK_HSBSyncTPoint = sc_time_stamp().to_double();
                mIsStopClock = true;
                mSetCLKEvent.notify();
                this->CancelEventsZeroClock();
            }
        }
        else if (word_vector[0] == CLK.basename())
        {
            mCLK_Freq = freq;
            ConvertClockFreq(mCLK_Freq, mCLK_FreqUnit, freq, unit);
            if (mCLK_Freq > 0)
            {
                mCLK_Period =
                    (sc_dt::uint64)(((1 / (double)mCLK_Freq) * (double)mTimeResolutionValue) + 0.5);
                mCLK_SyncTPoint = sc_time_stamp().to_double();
            }
            else
            {
                mCLK_Period = 0;
                mCLK_SyncTPoint = sc_time_stamp().to_double();
                this->CancelEventsZeroClock();
            }
        }
        else
        {
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }
    else
    {
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
} //}}}

void MMC44::GetCLKFreq(const std::string clock_name)
{ //{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1)
    {
        if (word_vector[0] == CLK_HSB.basename())
        {
            re_printf("info", "%s frequency is %0.0f %s\n", CLK_HSB.basename(), (double)mCLK_HSBOrgFreq, mCLK_HSBFreqUnit.c_str());
        }
        if (word_vector[0] == CLK.basename())
        {
            re_printf("info", "%s frequency is %0.0f %s\n", CLK.basename(), (double)mCLK_Freq, mCLK_FreqUnit.c_str());
        }
        else
        {
            re_printf("warning", "Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    }
    else
    {
        re_printf("warning", "Clock name (%s) is invalid\n", clock_name.c_str());
    }
} //}}}

void MMC44::ForceRegister(const std::string reg_name,
                          const uint32_t reg_value)
{ //{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "")
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1)
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mMMC44_Func->RegisterHandler(cmd);
        }
        else
        {
            re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
        }
    }
    else
    {
        re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
    }
} //}}}

void MMC44::ReleaseRegister(const std::string reg_name)
{ //{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "")
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1)
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            mMMC44_Func->RegisterHandler(cmd);
        }
        else
        {
            re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
        }
    }
    else
    {
        re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
    }
} //}}}

void MMC44::WriteRegister(const std::string reg_name,
                          const uint32_t reg_value)
{ //{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "")
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1)
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str << "0x" << std::hex << reg_value;
            cmd.push_back(reg_value_str.str());
            mMMC44_Func->RegisterHandler(cmd);
        }
        else
        {
            re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
        }
    }
    else
    {
        re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
    }
} //}}}
void MMC44::ReadRegister(const std::string reg_name)
{ //{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "")
    {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1)
        {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            mMMC44_Func->RegisterHandler(cmd);
        }
        else
        {
            re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
        }
    }
    else
    {
        re_printf("warning", "Register name (%s) is invalid\n", reg_name.c_str());
    }
} //}}}

void MMC44::ListRegister(void)
{ //{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mMMC44_Func->RegisterHandler(cmd);
} //}}}

void MMC44::Help(const std::string type)
{ //{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1)
    {
        if (word_vector[0] == "parameters")
        {
            SC_REPORT_INFO(this->basename(), "MMC44_MessageLevel (fatal|error|warning|info) Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(), "MMC44_DumpRegisterRW (true/false) Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(), "MMC44_DumpInterrupt (true/false) Enable/disable dumping interrupt information (Default: false)");
            SC_REPORT_INFO(this->basename(), "MMC44_EnableRegisterMessage (true/false) Enable/disable dumping register information (Default: false)");
        }
        else if (word_vector[0] == "commands")
        {
            SC_REPORT_INFO(this->basename(), "MMC44_AssertReset (reset_name, start_time, period) Assert and de-assert reset signal to the MMC44 model");
            SC_REPORT_INFO(this->basename(), "MMC44_SetCLKFreq (clock_name, freq, unit) Set clock frequency value to MMC44 model");
            SC_REPORT_INFO(this->basename(), "MMC44_GetCLKFreq (clock_name) Get clock frequency value of MMC44 model");
            SC_REPORT_INFO(this->basename(), "MMC44_ForceRegister (unit_name, reg_name, reg_value) Force register with setting value");
            SC_REPORT_INFO(this->basename(), "MMC44_ReleaseRegister (unit_name, reg_name) Release register from force value");
            SC_REPORT_INFO(this->basename(), "MMC44_WriteRegister (unit_name, reg_name, reg_value) Write a value to a register");
            SC_REPORT_INFO(this->basename(), "MMC44_ReadRegister (unit_name, reg_name) Read a value from a register");
            SC_REPORT_INFO(this->basename(), "MMC44_DumpStatusInfo() Dump all current register status of model");
            SC_REPORT_INFO(this->basename(), "MMC44_ListRegister () Dump name of all registers");
        }
        else
        {
            re_printf("warning", "The name (%s) of MMC44_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    }
    else
    {
        re_printf("warning", "The name (%s) of MMC44_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
} //}}}
void MMC44::SetMessageLevel(const std::string msg_lv)
{ //{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "")
    {
        SC_REPORT_INFO(MMC44::basename(), msg_return.c_str());
    }

    std::string msg_ret = this->handleCommand(cmd);
    if (msg_ret != "")
    {
        SC_REPORT_INFO(this->basename(), msg_ret.c_str());
    }

    if (mEnableRegisterMessage)
    {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mMMC44_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mMMC44_Func->RegisterHandler(cmd);
    }
    else
    {
        //Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mMMC44_Func->RegisterHandler(cmd);

        //Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mMMC44_Func->RegisterHandler(cmd);
    }
} //}}}

void MMC44::DumpRegisterRW(const std::string is_enable)
{ //{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mMMC44_Func->RegisterHandler(cmd);
} //}}}

void MMC44::DumpInterrupt(const std::string is_enable)
{ //{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1)
    {
        if (word_vector[0] == "true")
        {
            mDumpInterrupt = true;
        }
        else if (word_vector[0] == "false")
        {
            mDumpInterrupt = false;
        }
        else
        {
            re_printf("warning", "Invalid argument: MMC44_DumpInterrupt %s\n", is_enable.c_str());
        }
    }
    else if (word_vector.size() == 0)
    {
        std::string temp = "false";
        if (mDumpInterrupt) {
            temp = "true";
        }
        re_printf("info", "MMC44_DumpInterrupt %s\n", temp.c_str());
    }
    else
    {
        re_printf("warning", "Invalid argument: MMC44_DumpInterrupt %s\n", is_enable.c_str());
    }
} //}}}

void MMC44::DumpInterruptMsg(const std::string intr_name, const bool value)
{ //{{{
    if (mDumpInterrupt)
    {
        if (!value)
        {
            re_printf("info", "INT [%s] Assert.\n", intr_name.c_str());
        }
        else
        {
            re_printf("info", "INT [%s] De-assert.\n", intr_name.c_str());
        }
    }
} //}}}

void MMC44::DumpStatusInfo(void)
{ //{{{
    std::string type = "StatInfo";

    re_printf("PROFILE(%s): MMC44: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    re_printf("PROFILE(%s): MMC44: MMCAnCE_HOST_STS1  bits: 0x%02X\n", type.c_str(), mMMC44_Func->GetMMCAnCE_HOST_STS1());
    re_printf("PROFILE(%s): MMC44: MMCAnCE_HOST_STS2 bits: 0x%02X\n", type.c_str(), mMMC44_Func->GetMMCAnCE_HOST_STS2());
}

/// Enable/disable dumping info/warning/error message of register IF
void MMC44::EnableRegisterMessage(const std::string is_enable)
{ //{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1)
    {
        if (word_vector[0] == "true")
        {
            mEnableRegisterMessage = true;
        }
        else if (word_vector[0] == "false")
        {
            mEnableRegisterMessage = false;
        }
        else
        {
            re_printf("warning", "Invalid argument: MMC44_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }
    else if (word_vector.size() == 0)
    {
        std::string temp = "false";
        if (mEnableRegisterMessage) {
            temp = "true";
        }
        re_printf("info", "MMC44_EnableRegisterMessage %s\n", temp.c_str());
    }
    else
    {
        re_printf("warning", "Invalid argument: MMC44_EnableRegisterMessage %s\n", is_enable.c_str());
    }
} //}}}

//-------------------------------------------------------------------------------------------------
//                      Overwrite virtual functions of MMC44_AgentController
//-------------------------------------------------------------------------------------------------
/**@brief
 *Setting state of model when error occurred during DMA transfer
 */
void MMC44::ErrorOccuredDuringDMATransfer(void)
{ //{{{
    mDMAValue = 1;
    mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
    mMMC44_Func->SetMMC44Status(emCMDSEQ, 0);
    mMMCBusIsNotBusyEvent.notify(10 * (double)mCLK_Period, mTimeResolutionUnit);
} //}}}


/**@brief
 * active DMA line
 */
void MMC44::AssertDMAEvent(void)
{ //{{{
    mDMAValue = 0;
    mWriteDMAPortEvent.notify(CalculateClockEdge("CLK_HSB"));
} //}}}

/**@brief
 * to assert mReadDataFromBufferProcessEvent
 */
void MMC44::AssertReadDataFromBufferProcess(void)
{ //{{{
    mReadDataFromBufferProcessEvent.notify(SC_ZERO_TIME);
} //}}}

/**@brief
 * to assert mWriteDataToBufferProcessEvent
 */
void MMC44::AssertWriteDataToBufferProcess(void)
{ //{{{
    mWriteDataToBufferProcessEvent.notify(SC_ZERO_TIME);
} //}}}

/**@brief
 * Assert mStartSendCommandEvent
 */
void MMC44::AssertStartSendCommandEvent(void)
{ //{{{
    mStartSendCommandEvent.notify(SC_ZERO_TIME);
} //}}}

/**@brief
 * assert mSetTimeOutAndErrStt event
 */
void MMC44::AssertSetTimeoutAndErrStt(uint32_t index)
{ //{{{
    uint64_t temp = 0;
    switch (index)
    {
    case emCRCSTE_H:
        mSetTimeOutAndErrStt[emCRCSTE_H].notify(SC_ZERO_TIME);
        break;
    case emCRC16E:
        mSetTimeOutAndErrStt[emCRC16E].notify(SC_ZERO_TIME);
        break;
    case emAC12CRCE:
        mSetTimeOutAndErrStt[emAC12CRCE].notify(SC_ZERO_TIME);
        break;
    case emRSPCRC7E:
        mSetTimeOutAndErrStt[emRSPCRC7E].notify(SC_ZERO_TIME);
        break;
    case emCRCSTEBE:
        mSetTimeOutAndErrStt[emCRCSTEBE].notify(SC_ZERO_TIME);
        break;
    case emRDATEBE:
        mSetTimeOutAndErrStt[emRDATEBE].notify(SC_ZERO_TIME);
        break;
    case emAC12REBE:
        mSetTimeOutAndErrStt[emAC12REBE].notify(SC_ZERO_TIME);
        break;
    case emRSPEBE:
        mSetTimeOutAndErrStt[emRSPEBE].notify(SC_ZERO_TIME);
        break;
    case emAC12IDXE:
        mSetTimeOutAndErrStt[emAC12IDXE].notify(SC_ZERO_TIME);
        break;
    case emRSPIDXE:
        mSetTimeOutAndErrStt[emRSPIDXE].notify(SC_ZERO_TIME);
        break;
    case emBTACKPATE:
        mSetTimeOutAndErrStt[emBTACKPATE].notify(SC_ZERO_TIME);
        break;
    case emBTACKEBE:
        mSetTimeOutAndErrStt[emBTACKEBE].notify(SC_ZERO_TIME);
        break;
    case emSTRDATTO:
        temp = ((uint64_t)1) << (14 + mMMC44_Func->GetClockControl(emSRWDTO));
        mSetTimeOutAndErrStt[emSTRDATTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case emDATBSYTO:
        temp = ((uint64_t)1) << (14 + mMMC44_Func->GetClockControl(emSRWDTO));
        mSetTimeOutAndErrStt[emDATBSYTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit); // time out start counting after crc status was received
        break;
    case emCRCSTTO:
        temp = ((uint64_t)1) << (14 + mMMC44_Func->GetClockControl(emSRWDTO));
        mSetTimeOutAndErrStt[emCRCSTTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case emAC12BSYTO:
        temp = ((uint64_t)1) << (14 + mMMC44_Func->GetClockControl(emSRBSYTO));
        mSetTimeOutAndErrStt[emAC12BSYTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case emRSPBSYTO:
        temp = ((uint64_t)1) << (14 + (uint64_t)mMMC44_Func->GetClockControl(emSRBSYTO));
        mSetTimeOutAndErrStt[emRSPBSYTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case emAC12RSPTO:
        temp = (uint64_t)(32 * (2 << (mMMC44_Func->GetClockControl(emSRSPTO))));
        mSetTimeOutAndErrStt[emAC12RSPTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case emSTRSPTO:
        temp = (uint64_t)(32 * (2 << (mMMC44_Func->GetClockControl(emSRSPTO))));
        mSetTimeOutAndErrStt[emSTRSPTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case emBTACKTO:
        temp = ((uint64_t)1) << (14 + mMMC44_Func->GetBootOpSetting(emSBTACKTO));
        mSetTimeOutAndErrStt[emBTACKTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case em1STBTDATTO:
        temp = ((uint64_t)1) << (14 + mMMC44_Func->GetBootOpSetting(emS1STBTDATTO));
        mSetTimeOutAndErrStt[em1STBTDATTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    case emBTDATTO:
        temp = ((uint64_t)1) << (14 + mMMC44_Func->GetBootOpSetting(emSBTDATTO));
        mSetTimeOutAndErrStt[emBTDATTO].notify(temp * (double)mCLK_Period, mTimeResolutionUnit);
        break;
    default:
        break;
    }
} //}}}

/**@brief
 * assert mSetCLKEvent
 */
void MMC44::AssertSetCLKEvent(void)
{ //{{{
    mIsStopClock = false;
    mSetCLKEvent.notify();
} //}}}

/**@brief
 * setting clock in boot mode
 */
void MMC44::AssertSetBootCLKEvent(void)
{ //{{{    
    uint64_t clkFreq = mCLK_HSBOrgFreq / (((uint64_t)2) << (mMMC44_Func->GetClockControl(emCLKDIV)));
    uint64_t freq = CLK.read();
    if (freq == clkFreq)
    {
        mIsBootClock = true;
        mSetCLKEvent.notify(CalculateClockEdge("CLK").to_double() + emTimeSwitchToCLKBoot * (double)mCLK_Period, mTimeResolutionUnit);
    }
    else
    {
        mIsBootClock = false;
        mSetCLKEvent.notify(CalculateClockEdge("CLK").to_double() + emTimeSwitchToCLKInCMDReg * (double)mCLK_Period, mTimeResolutionUnit);
    }
} //}}}

/**@brief
 * assert mBuffer1ReadEnableEvent
 */
void MMC44::AssertBuffer1ReadEnableEvent()
{ //{{{
    mBuffer1ReadEnableEvent.notify();
} //}}}

/**@brief
 * assert mBuffer2ReadEnableEvent
 */
void MMC44::AssertBuffer2ReadEnableEvent()
{ //{{{
    mBuffer2ReadEnableEvent.notify();
} //}}}

/**@brief
 * assert mIntBUFWENSetEvent
 */
void MMC44::AssertIntBUFWENSetEvent()
{ //{{{
    mIntBUFWENSetEvent.notify();
} //}}}

/**@brief
 * assert mOneBlockHadWrittenEvent
 */
void MMC44::AssertOneBlockHadWrittenEvent(void)
{ //{{{
    mOneBlockHadWrittenEvent.notify();
} //}}}

/**@brief
 * assert mOneBlockHadReadEvent
 */
void MMC44::AssertOneBlockHadReadEvent(void)
{ //{{{
    mOneBlockHadReadEvent.notify();
} //}}}

/**@brief
 * Calculate clock edge
 */
sc_time MMC44::CalculateClockEdge(const std::string clock_name)
{ //{{{
    // define later
    sc_time edge_point = sc_time(0, mTimeResolutionUnit);

    if (clock_name == CLK_HSB.basename())
    {
        edge_point = sc_time(CalculateCLKPosEdge((double)mCLK_HSBPeriod, false, sc_time_stamp().to_double(), mCLK_HSBSyncTPoint), mTimeResolutionUnit);
    }
    else if (clock_name == CLK.basename())
    {
        edge_point = sc_time(CalculateCLKPosEdge((double)mCLK_Period, false, sc_time_stamp().to_double(), mCLK_SyncTPoint), mTimeResolutionUnit);
    }
    return edge_point;
} //}}}

/**@brief
 * Update state and assert interrupt
 */
void MMC44::AssertInterrupt(void)
{ //{{{
    mAssertInterruptEvent.notify(CalculateClockEdge("CLK_HSB"));
} //}}}

/**@brief
 * Update state and De-assert interrupt
 */
void MMC44::DeAssertInterrupt(void)
{ //{{{
    mDeAssertInterruptEvent.notify(CalculateClockEdge("CLK_HSB").to_double(), mTimeResolutionUnit);
} //}}}

/**@brief
 * Check clock period
 */
bool MMC44::CheckClockPeriod(const std::string clock_name)
{ //{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    if (clock_name == CLK_HSB.basename()) {
        clock_period = mCLK_HSBPeriod;
    }else{
        clock_period = mCLK_Period;
    }
    if (clock_period > 0)
    {
        clock_status = true;
    }
    else
    {
        clock_status = false;
        re_printf("info", "%s frequency is zero\n", clock_name.c_str());
    }

    return clock_status;
} //}}}

/**@brief
 * Get reset status
 */
bool MMC44::GetResetStatus(void)
{ //{{{
    return (mIsResetPortActive || mIsResetCmdActive);
} //}}}

/**@brief
 * Enable Software reset
 */
void MMC44::EnableSoftwareReset(const bool is_active)
{ //{{{
    this->EnableReset(is_active);
} //}}}
