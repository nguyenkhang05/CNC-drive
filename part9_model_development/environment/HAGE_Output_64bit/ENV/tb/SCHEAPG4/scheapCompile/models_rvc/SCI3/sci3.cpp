// ---------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012 Renesas Electronics Corporation
// Copyright(c) 2012 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include <string>
#include <cstdarg>
#include "sci3.h"
#include "re_define.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//===============Csci3 class=======================
//Constructor of Csci3 class
Csci3::Csci3(sc_module_name name):
     Csci3_regif((std::string)name,32)
    ,Cgeneral_sci((std::string)name,1)
    ,vpcl::tlm_tgt_if<32> (name)
    ,preset_n("preset_n")
    ,pclk("pclk")
    ,SCKI("SCKI")
    ,RX_CONTROL("RX_CONTROL")
    ,RX_DATA("RX_DATA")
    ,PDR("PDR")
    ,SCKO("SCKO")
    ,TX_CONTROL("TX_CONTROL")
    ,TX_DATA("TX_DATA")
    ,TEI("TEI")
    ,TXI("TXI")
    ,RXI("RXI")
    ,ERI("ERI")
    ,SPI_CONTROL("SPI_CONTROL")
{//{{{
    CommandInit(this->name());
    Csci3_regif::set_instance_name(this->name());
    //Initialize internal variables
    mTransStatus = "t_idle";
    mRecStatus   = "r_idle";
    mSCI3Reset       = false;
    mCmdReset        = false;
    mIsCmdResetStatus = false;
    mResetPeriod     = 0;
    mPCLKFreq    = 0;
    mPCLKPeriod  = 0;
    this->Initialize();

    //Initialize output port
    SCKO.initialize(0);
    TEI.initialize(false);
    TXI.initialize(false);
    RXI.initialize(false);
    ERI.initialize(false);
    TX_CONTROL.initialize(emTxControlDefault);
    SPI_CONTROL.initialize(emTxControlDefault);
    TX_DATA.initialize(emTxDataDefault);

    //Set GeneralSCI parameters
    SetUpGeneralSCI();

    SC_METHOD(ResetMethod)
    dont_initialize();
    sensitive << preset_n;

    SC_METHOD(CmdResetMethod)
    dont_initialize();
    sensitive << mCmdResetEvent;

    SC_METHOD(CancelCmdResetMethod)
    dont_initialize();
    sensitive << mCancelCmdResetEvent;

    SC_METHOD(PCLKPeriodMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(SCKIPeriodMethod);
    dont_initialize();
    sensitive << SCKI;

    SC_METHOD(CancelBreakMethod);
    dont_initialize();
    sensitive << PDR;

    SC_METHOD(WriteSCKOMethod);
    dont_initialize();
    sensitive << mWriteSCKOEvent;

    SC_METHOD(WriteTxDMethod);
    dont_initialize();
    sensitive << mWriteTxDEvent;

    SC_METHOD(RxControlMethod);
    dont_initialize();
    sensitive << RX_CONTROL;

    SC_METHOD(SPIControlMethod);
    dont_initialize();
    sensitive << SPI_CONTROL;

    SC_METHOD(ReadRxDMethod);
    dont_initialize();
    sensitive << mReadRxDEvent;
    sensitive << RX_DATA;

    SC_THREAD(TransmitThread);
    dont_initialize();
    sensitive << mTransmitEvent;
    sensitive << mAssertResetEvent;
    sensitive << mCancelTransmitEvent;

    SC_THREAD(ReceiveThread);
    dont_initialize();
    sensitive << mReceiveEvent;
    sensitive << mAssertResetEvent;
    sensitive << mCancelReceiveEvent;

    SC_METHOD(TXIInterruptMethod);
    dont_initialize();
    sensitive << mTXIInterruptEvent;

    SC_METHOD(TEIInterruptMethod);
    dont_initialize();
    sensitive << mTEIInterruptEvent;

    SC_METHOD(RXIInterruptMethod);
    dont_initialize();
    sensitive << mRXIInterruptEvent;

    SC_METHOD(ERIInterruptMethod);
    dont_initialize();
    sensitive << mERIInterruptEvent;

    SC_METHOD(CheckBreakMethod);
    dont_initialize();
    sensitive << mCheckBreakEvent;

    SetLatency_TLM(0, true);
    #ifdef CWR_SYSTEMC
    //handle command
    SCML_COMMAND_PROCESSOR(handleCommand);
    SCML_ADD_COMMAND("tgt", 1, 8, "tgt <cmd> <arg0> <arg1> <arg2> ...", "prefix of tlm target if command");
    SCML_ADD_COMMAND("command", 1, 3, "command <param> <arg> <1/->", "prefix of setting module parameters");
    SCML_ADD_COMMAND("reg", 1, 8, "reg <cmd> <arg0> <arg1> <arg2> ...", "prefix of register command");
    #endif
}//}}}

//Destructor of Csci3 class
Csci3::~Csci3()
{//{{{
}//}}}

void Csci3::Initialize(void)
{//{{{
    mTransmitDataAmount = 0;
    mReceiveDataAmount  = 0;
    mInterruptKind   = 0;
    mMDDRVal         = 0xFF;
    mBRRVal          = 0xFF;
    mTransSeq        = 0;
    mTSRreg          = 0xFFFF;
    mTxTransNum      = 0;
    mRDRtemp         = 0;
    mSCKO            = 0;
    mSCKI            = 0;
    mFramePeriod     = 0;
    mOperationMode   = emAsynMode;
    mClearERI        = false;
    mClearTEI        = false;
    mAssertRXI       = false;
    mAssertTXI       = false;
    mSSRIsRead       = false;
    mIsContTransmit  = false;
    mIsSendingBreak  = false;
    mReceiveLatencyFlag  = false;
    mNewRxData       = false;
    mIsSlaveTrans    = false;
    mIsTSR           = false;
    mCurrentSTBE     = 0x3;
    mIsFirstDataTransmit = false;
    mIsFirstDataReceive  = false;
    mTransmitDataPacket.Initialize();
    mReceiveDataPacket.Initialize();
}//}}}

void Csci3::ResetMethod(void)
{//{{{
    bool is_active = (preset_n.read() == vpcl::emResetActive) ? true : false;   // Check value of preset_n signal
    
    // Cancel handleCommand reset when preset_n is asserted
    if (is_active && mIsCmdResetStatus) {
        mCmdResetEvent.cancel();
        mCancelCmdResetEvent.cancel();
        mIsCmdResetStatus = false;
        mCmdReset = false;
    }
    if (is_active){
        re_printf("info","Reset signal is asserted.\n");
        mSCI3Reset = is_active;
        this->EnableReset(is_active);
    } else {
        re_printf("info","Reset signal is negated.\n");
        this->EnableReset(is_active);
        mSCI3Reset = is_active;
    }
}//}}}

void Csci3::AssertReset(const double delay, const double period)
{//{{{
    // If preset_n is negated, SCI3 waits delay time then notifies handleCommand reset event
    if ((!mSCI3Reset)&&(!mIsCmdResetStatus)) {
        mIsCmdResetStatus = true;
        re_printf("info","SCI3 will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) delay, FRACTION_TO_INT(delay));
        mCmdResetEvent.notify(delay, SC_NS);
        mResetPeriod = period;
    } else {
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Csci3::CmdResetMethod(void)
{//{{{
    mCmdReset = true;
    // If preset_n is negated, SCI3 notifies cancel handleCommand reset event after reset period
    this->EnableReset(mCmdReset);
    mCancelCmdResetEvent.notify(mResetPeriod, SC_NS);
}//}}}

void Csci3::CancelCmdResetMethod(void)
{//{{{
    mCmdReset = false;
    mIsCmdResetStatus = false;
    this->EnableReset(mCmdReset);
}//}}}

void Csci3::EnableReset(const bool is_active)
{//{{{
    if (is_active){
        // Initialize data members
        mTransStatus = "resetting";
        mRecStatus   = "resetting";
        this->Initialize();

        // Cancel all events
        mTransmitEvent.cancel();
        mReceiveEvent.cancel();
        mTXIInterruptEvent.cancel();
        mTEIInterruptEvent.cancel();
        mRXIInterruptEvent.cancel();
        mERIInterruptEvent.cancel();
        mWriteSCKOEvent.cancel();
        mReadRxDEvent.cancel();
        mWriteTxDEvent.cancel();
        mCheckBreakEvent.cancel();

        // Initialize output ports
        mTXIInterruptEvent.notify();
        mTEIInterruptEvent.notify();
        mRXIInterruptEvent.notify();
        mERIInterruptEvent.notify();
        mWriteSCKOEvent.notify();
        mWriteTxDEvent.notify();

        // Notify reset event
        mAssertResetEvent.notify();
    } else {
        // Change status of SCI3 from resetting to idle
        mTransStatus = "t_idle";
        mRecStatus   = "r_idle";
    }
    // Call the reset function of common/general class
    Csci3_regif::EnableReset(is_active);
    Cgeneral_sci::EnableReset(is_active);
    if (mSCI3Reset) {
        this->tgt_enable_reset(is_active);
    }
}//}}}

void Csci3::PCLKPeriodMethod(void)
{//{{{
    if (!(mSCI3Reset||mCmdReset)) {
        SetCLKfreq("pclk",(double)(pclk.read()));
    }
}//}}}

void Csci3::CancelBreakMethod(void)
{//{{{
    if (mSCI3Reset||mCmdReset || (mPCLKFreq == 0)) {
        if (mPCLKFreq == 0) {
            re_printf("warning","The pclk period is equal 0.\n");
        }
        return;
    }
    if (mIsSendingBreak) {
        mTransmitDataPacket.Initialize();
        mTransmitDataPacket.SetBreak(true);
        mWriteTxDEvent.notify();
    }
}//}}}

void Csci3::WriteSCKOMethod(void)
{//{{{
    if (!(mSCI3Reset||mCmdReset)) {
        SCKO.write((sc_dt::uint64)mSCKO);
    } else {
        SCKO.write(0);
    }
}//}}}

void Csci3::WriteTxDMethod(void)
{//{{{
    if (mOperationMode == emAsynMode) {
        TX_CONTROL.write((sc_uint<32>)TXControlConvert(mTransmitDataPacket));
        SPI_CONTROL.write(emTxControlDefault);
    } else {
        TX_CONTROL.write(emTxControlDefault);
        SPI_CONTROL.write((sc_uint<32>)TXControlConvert(mTransmitDataPacket));
    }
    TX_DATA.write((sc_uint<32>)TXDataConvert(mTransmitDataPacket));
}//}}}

void Csci3::RxControlMethod(void)
{//{{{
    if (mPCLKFreq == 0) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    if (mOperationMode == emAsynMode) {
        mReadRxDEvent.notify();
    }
}//}}}

void Csci3::SPIControlMethod(void)
{//{{{
    if (mPCLKFreq == 0) {
        re_printf("warning","The pclk period is equal 0.\n");
        return;
    }
    if (mOperationMode == emClockedSynMode) {
        unsigned int spi_control_val = (unsigned int)SPI_CONTROL.read();
        unsigned int new_stbe = (spi_control_val >> 6)&0x3;
        if ((new_stbe == 0x1)&&(mCurrentSTBE == 0x2)) {
            mNewRxData = true;
        }
        mCurrentSTBE = new_stbe;
        mReadRxDEvent.notify();
    }
}//}}}

void Csci3::ReadRxDMethod(void)
{//{{{
    if (mSCI3Reset||mCmdReset || (mPCLKFreq == 0)) {
        if (mPCLKFreq == 0) {
            re_printf("warning","The pclk period is equal 0.\n");
        }
        return;
    }
    CSciIF ReceiveDataPacket;
    ProcessReceivedData(ReceiveDataPacket);
    // Process receiving break signal
    if ((mOperationMode == emAsynMode)&&(ReceiveDataPacket.GetBreak())) {
        SEMR_RXDMON = 0;
    } else {
        SEMR_RXDMON = 1;
        mCheckBreakEvent.cancel(); // Cancel checking break signal when receive normal signal
    }
    // When RxD changed, SCI3 receive only in Asynchronous mode or SCR.CKE1 = 0
    if (mRecStatus == "r_ready") {
        if ((SCR_CKE1 == 1)&&(ReceiveDataPacket.GetStartBit())) {
            return;
        } else {
            mReceiveEvent.notify();
        }
    }
}//}}}

void Csci3::SCKIPeriodMethod(void)
{//{{{
    mSCKI = (double)(SCKI.read());
    if (mSCI3Reset||mCmdReset || (mPCLKFreq == 0)) {
        if (mPCLKFreq == 0) {
            re_printf("warning","The pclk period is equal 0.\n");
        }
        return;
    }
    if ((mOperationMode == emClockedSynMode)&&(mSCKI != 0)&&(SCR_CKE1 == 1)) {
        if (mIsSlaveTrans) {
            mTransmitEvent.notify(); // Start transmission progress
        } else {
            if (mTransStatus == "t_ready") {
                StartTransmission();
            }
        }
        if (mRecStatus == "r_ready") {
            mReceiveEvent.notify();
        }
    }
}//}}}

void Csci3::TransmitThread(void)
{//{{{
    for(;;){
        if (mSCI3Reset||mCmdReset) {
            wait();
            continue;
        }
        try {
            while (true) {
                // Transmission is enabled when SCR.TE = 1, SCI3 is not resetting and overrun error flag is 0 in Clocked synchronous mode
                if ((SCR_TE == 1)&&(((mOperationMode == emClockedSynMode)&&(SSR_ORER == 0))||(mOperationMode == emAsynMode))) {
                    double bit_rate = BitRateCalc(); // Calculate bit rate
                    if (bit_rate != 0) {
                        double period = 0;
                        double PCLK_period = 0;
                        double mLatencyTime = 0;
                        if (mIsSlaveTrans) {
                            mIsSlaveTrans = false;
                        } else {
                            SSR_TDRE = 1;
                            if (SCR_TIE == 1) {
                                NotifyInterruptEvent(emTransmitEmptyIntr); // Notify TXI interrupt
                            }
                        }
                        mTransStatus = "t_trans"; // Change status of transmission state to t_trans when starting transmission
                        if (TransTimeCalcMode) {
                            period = ((double)emNanoSecond)/bit_rate; // Calculate elementary time unit (etu)
                            if (mPCLKFreq > 0) {
                                PCLK_period = ((double)emNanoSecond)/mPCLKFreq; // Calculate pclk period
                            }
                            mLatencyTime = TransferLatency; // Calculate latency time before transmitting
                        }
                        if ((PCLK_period != 0)||(!TransTimeCalcMode)) {
                            // Wait latency before transmission in Asynchronous mode and Clocked synchronous mode(SCR.CKE1 = 0)
                            if ((!mIsContTransmit)&&((mOperationMode == emAsynMode)||((mOperationMode == emClockedSynMode)&&(SCR_CKE1 == 0)))) {
                                wait(mLatencyTime, SC_NS, mAssertResetEvent|mCancelTransmitEvent);
                                if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                    throw vpcl::emRstException;
                                }
                                // Stop transmitting if SCR.TE is cleared to 0 during transmitting
                                if (SCR_TE == 0) {
                                    break;
                                }
                            }
                            WriteSCKO(bit_rate,false); // Output SCKO when starting transmission
                            // Set bit time of transmit data packet before transmitting
                            unsigned int bit_time = (unsigned int)(((double)emNanoSecond)/(bit_rate*100)); // bit_time = (1/(100*bit_rate)) (in units of 100ns)
                            if (bit_time > 0xFFFF) {
                                re_printf("warning","The transfer bit time should be less than or equal to 0xFFFF.\n");
                                mTransmitDataPacket.SetBitt(0xFFFF); // Set max value of bit time if it greater than 0xFFFF
                            } else {
                                mTransmitDataPacket.SetBitt((unsigned int)bit_time);
                            }
                            // Process transmission and get transmit time
                            double transmit_time = TransmitProcess(period);

                            if ((mOperationMode == emClockedSynMode) && (((SPI_CONTROL.read() >> 6) & 0x3) == 0x0)) {
                                // Transmit data packet (STBE = B'00)
                                mIsFirstDataTransmit = true;
                                mWriteTxDEvent.notify();
                                // STBE keeps value B'00 for 0.5 etu of first data
                                wait(period/2, SC_NS, mAssertResetEvent|mCancelTransmitEvent);
                                if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                    throw vpcl::emRstException;
                                }
                                transmit_time -= period/2;
                                // Transmit data packet (STBE = B'01)
                                mIsFirstDataTransmit = false;
                                mIsFirstDataReceive  = true;
                                mWriteTxDEvent.notify();
                            } else {
                                mWriteTxDEvent.notify();
                            }
                            // Wait transmission time elapsing
                            wait(transmit_time, SC_NS, mAssertResetEvent|mCancelTransmitEvent);
                            if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                throw vpcl::emRstException;
                            }
                            // Stop transmitting if SCR.TE is cleared to 0 during transmitting
                            if (SCR_TE == 0) {
                                break;
                            }
                            // Write StopBit to TxD and wait stop bit transfer time in Asynchronous mode
                            if (mOperationMode == emAsynMode) {
                                double stopbit_trans_time = emOnePSMBit;
                                // Write StopBit value to TxD port
                                if (SMR_STOP == 0) {
                                    mTransmitDataPacket.SetStopBit(0x1);
                                } else {
                                    stopbit_trans_time = emTwoStopBit;
                                    mTransmitDataPacket.SetStopBit(0x3);
                                }
                                mWriteTxDEvent.notify();
                                // Wait stop bit transfer time elapsing
                                wait(stopbit_trans_time*period, SC_NS, mAssertResetEvent|mCancelTransmitEvent);
                                if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                    throw vpcl::emRstException;
                                }
                                // Stop transmitting if SCR.TE is cleared to 0 during transmitting
                                if (SCR_TE == 0) {
                                    break;
                                }
                            }

                            // Check SSR.TDRE to finish transmission or continue next transmission
                            if (SSR_TDRE == 1) {
                                mIsContTransmit = false; // Finish transmission
                                if (mOperationMode == emAsynMode) {
                                    // Write default data packet with StartBit = 1 when finish transmission in Asynchronous mode
                                    mTransmitDataPacket.Initialize();
                                    mWriteTxDEvent.notify();
                                    // Wait for 1 pclk before setting SSR.TEND to 1 in Asynchronous operation mode
                                    wait(PCLK_period, SC_NS, mAssertResetEvent|mCancelTransmitEvent);
                                    if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                        throw vpcl::emRstException;
                                    }
                                }
                                SSR_TEND = 1;
                                if (SCR_TEIE == 1) {
                                    NotifyInterruptEvent(emTransmitEndIntr); // Notify TEI interrupt
                                }
                                // Write 0 to SCKO after ending transmission in Asynchronous mode or before final 0.5 etu in Clocked synchronous mode
                                WriteSCKO(0,false);
                                if (mOperationMode == emClockedSynMode) {
                                    mTransmitDataPacket.SetStopBit(0x1); // Write STBE = B'10 before final 0.5 etu
                                    mWriteTxDEvent.notify();
                                    // Wait 0.5 etu to finish transmission in Clocked synchronous
                                    wait(period/2, SC_NS, mAssertResetEvent|mCancelTransmitEvent);
                                    if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                        throw vpcl::emRstException;
                                    }
                                    // Stop transmitting if SCR.TE is cleared to 0 during transmitting
                                    if (SCR_TE == 0) {
                                        break;
                                    }
                                    // Write default data packet with StartBit = 1 when finish transmission in Clocked synchronous mode
                                    mTransmitDataPacket.Initialize();
                                    mWriteTxDEvent.notify();
                                }
                                mTransStatus = "t_ready"; // Change status of transmission state to t_ready when finishing transmission
                            } else {
                                if (mOperationMode == emClockedSynMode) { // Clock-synchronous mode
                                    mTransmitDataPacket.SetStopBit(0x1); // Write STBE = B'10 before final 0.5 etu
                                    mWriteTxDEvent.notify();
                                    // Wait 0.5 etu to finish transmission in Clocked synchronous
                                    wait(period/2, SC_NS, mAssertResetEvent|mCancelTransmitEvent);
                                    if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                        throw vpcl::emRstException;
                                    }
                                    // Stop transmitting if SCR.TE is cleared to 0 during transmitting
                                    if (SCR_TE == 0) {
                                        break;
                                    }
                                }
                                // Transfer data from TDR to TxFIFO if SSR.TDRE = 0 to start next transmission
                                StartTransmission();
                            }
                        }
                    }
                }
                wait();
                if (mSCI3Reset||mCmdReset) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException){
        }
    }
}//}}}

void Csci3::ReceiveThread(void)
{//{{{
    for(;;){
        if (mSCI3Reset||mCmdReset) {
            wait();
            continue;
        }
        try {
            while(true) {
                // Reception is enabled when SCR.RE = 1, SCI3 is not resetting and all error flags is 0
                if ((SCR_RE == 1)&&(((mOperationMode == emClockedSynMode)&&(SSR_ORER == 0))||(mOperationMode == emAsynMode))) {
                    double bit_time = 0;
                    double bit_rate = BitRateCalc(); // Calculate bit rate
                    if (bit_rate != 0) {
                        double period = 0;
                        double receive_time = 0;
                        double mLatencyTime = 0;
                        unsigned int receive_data = 0;
                        mRecStatus = "r_rec"; // Change status of reception state to r_rec when starting reception
                        if (TransTimeCalcMode) {
                            period = ((double)emNanoSecond)/bit_rate; // Calculate elementary time unit (etu)
                            receive_time = TransTimeCalc(period,true); // Calculate receiving time
                            mLatencyTime = TransferLatency; // Calculate latency time before receiving
                            if ((mOperationMode == emClockedSynMode) && (mIsFirstDataReceive)) {
                                receive_time -= period/2;
                                mIsFirstDataReceive = false;
                            }
                        }
                        // Wait a latency time when RE is set 0 to 1 in Clocked synchronous mode
                        if (mReceiveLatencyFlag) {
                            wait(mLatencyTime, SC_NS, mAssertResetEvent|mCancelReceiveEvent);
                            if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                throw vpcl::emRstException;
                            }
                            // Stop receiving if SCR.RE is cleared to 0 during receiving
                            if (SCR_RE == 0) {
                                break;
                            }
                        }
                        ProcessReceivedData(mReceiveDataPacket);
                        if (mOperationMode == emAsynMode) {
                            WriteSCKO(bit_rate,true); // Output SCKO when starting reception
                        }
                        if (!(mReceiveDataPacket.GetStartBit())) { // Check start bit of receive data packet
                            bool frame_error = FrameChecking(); // Checking size and direction of received data packet
                            if (CompareRecDataMatch()) { // Multiprocessor function only valid in Asynchronous mode
                                bool is_receive_end = false;
                                // Wait receiving data time elapsing
                                if ((mOperationMode == emClockedSynMode) && (mReceiveDataPacket.GetStopBit() == 1)) {
                                    is_receive_end = false;
                                } else {
                                    is_receive_end = true;
                                    wait(receive_time, SC_NS, mAssertResetEvent|mCancelReceiveEvent);
                                }
                                if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                    throw vpcl::emRstException;
                                }
                                // Stop receiving if SCR.RE is cleared to 0 during receiving
                                if (SCR_RE == 0) {
                                    break;
                                }
                                // Get data after waiting receiving time
                                receive_data = mReceiveDataPacket.GetData();
                                // Process overrun error checking
                                if ((SSR_RDRF == 1)&&((mOperationMode == emAsynMode) || ((mOperationMode == emClockedSynMode)&&(mReceiveDataPacket.GetStopBit() == 0)))) {
                                    SSR_ORER = 1; // Set overrun error flag to 1
                                    re_printf("warning","The reception is overrun. Received data is not transferred to RDR.\n");
                                    IssueRecIntr(true); // Issue ERI interrupt
                                }
                                // Checking received data in Asynchronous mode
                                if (mOperationMode == emAsynMode) {
                                    if (SMR_MP == 0) { // Process receiving parity bit
                                        if (SMR_PE == 1) {
                                            // Wait for 1 etu before checking received parity bit
                                            wait(period, SC_NS, mAssertResetEvent|mCancelReceiveEvent);
                                            if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                                throw vpcl::emRstException;
                                            }
                                            // Stop receiving if SCR.RE is cleared to 0 during receiving
                                            if (SCR_RE == 0) {
                                                break;
                                            }
                                            if (!(mReceiveDataPacket.GetParityBit() == GetParity(receive_data))) {
                                                SSR_PER = 1; // Set parity error flag to 1
                                                re_printf("warning","The parity error is occurred when receiving data in Asynchronous mode.\n");
                                                IssueRecIntr(true); // Issue ERI interrupt
                                            }
                                        }
                                    } else { // Process receiving multiprocessor bit
                                        // Wait for 1 etu before checking received multiprocessor bit
                                        wait(period, SC_NS, mAssertResetEvent|mCancelReceiveEvent);
                                        if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                            throw vpcl::emRstException;
                                        }
                                        // Stop receiving if SCR.RE is cleared to 0 during receiving
                                        if (SCR_RE == 0) {
                                            break;
                                        }
                                        if (SSR_MPB == 1) {
                                            SCR_MPIE = 0; // SCI3 received ID packet and change to receive data packet in next reception
                                        }
                                    }

                                    // Process checking frame
                                    // Wait for 1 etu before checking received stop bit
                                    wait(period, SC_NS, mAssertResetEvent|mCancelReceiveEvent|mCancelReceiveEvent);
                                    if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                        throw vpcl::emRstException;
                                    }
                                    // Stop receiving if SCR.RE is cleared to 0 during receiving
                                    if (SCR_RE == 0) {
                                        break;
                                    }

                                    // Check bit time of received data packet
                                    ProcessReceivedData(mReceiveDataPacket); // Stop bit and bitt will be get at the timing of receiving stop bit
                                    double bitt = (double)mReceiveDataPacket.GetBitt();
                                    bit_time = ((double)emNanoSecond)/(bit_rate*100); // bit_time = (1/(100*bit_rate)) (in units of 100ns)
                                    if ((bitt < (bit_time - bit_time*((double)emDeviation)/100))||(bitt > (bit_time + bit_time*((double)emDeviation)/100))) {
                                        frame_error = true;
                                        re_printf("warning","Bit time deviation of received data packet is larger than %d%% in Asynchronous mode.\n",emDeviation);
                                    }

                                    // Process receiving stop bit
                                    if (((mReceiveDataPacket.GetStopBit()&0x1) != 1)||(frame_error)) {
                                        SSR_FER = 1; // Set framing error flag to 1
                                        re_printf("warning","The framing error is occurred when receiving data in Asynchronous mode.\n");
                                        IssueRecIntr(true); // Issue ERI interrupt
                                    }
                                }
                                if (is_receive_end == true) {
                                    // Process storing received data to RDR if there is no error when receiving
                                    if ((SSR_ORER == 0)&&(SSR_FER == 0)&&(SSR_PER == 0)) {
                                        // Process end reception successfully
                                        unsigned int temp_data = receive_data;
                                        if (SCMR_CHR1 == 1) {
                                            if ((mOperationMode == emAsynMode)&&(SMR_CHR == 1)) {
                                                temp_data = temp_data&0x7F;
                                            }
                                            RDR_RDR = (unsigned int)(temp_data&0xFF);
                                        } else {
                                            RDR_RDR = (unsigned int)(temp_data&0xFFFF);
                                        }
                                        SSR_RDRF = 1;  // RDRF will be set to 1 if there is no error when receiving
                                        IssueRecIntr(false); // Issue RXI interrupt
                                        mReceiveDataAmount ++; // Increase total received data amount
                                        DumpOperationInfo("Receiving"); // Dump reception info
                                    }
                                    // Wait 0.5etu
                                    wait(period/2, SC_NS, mAssertResetEvent|mCancelReceiveEvent);
                                    if (mSCI3Reset||mCmdReset||(mPCLKFreq == 0)) {
                                        throw vpcl::emRstException;
                                    }
                                    if (mTransStatus != "t_trans") {
                                        WriteSCKO(0,true); // Write 0 to SCKO after finishing reception
                                    }
                                }
                            }
                        }
                        mRecStatus = "r_ready"; // Change status of reception state to r_ready when finishing reception
                        // Check break signal after a framing receiving period in Asynchronous mode
                        if ((mOperationMode == emAsynMode)&&(TransTimeCalcMode)&&(SEMR_RXDMON == 0)) {
                            mFramePeriod = TransTimeCalc(period,true) + period/2 ;
                            mCheckBreakEvent.notify();
                        }
                    }
                }
                mReceiveLatencyFlag = false;
                if (mNewRxData) {
                    mReadRxDEvent.notify();
                    mNewRxData = false;
                }
                wait ();
                if (mSCI3Reset||mCmdReset) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException){
        }
    }
}//}}}

void Csci3::TXIInterruptMethod(void)
{//{{{
    if (!(mSCI3Reset||mCmdReset)) {
        // Issue TXI interrupt
        if (!mAssertTXI) {
            mAssertTXI = true;
            TXI.write(true); // Assert TXI interrupt (rising edge protocol)
            DumpInterruptMsg(emTransmitEmptyIntr); // Dump TXI interrupt message
            mTXIInterruptEvent.notify(mPCLKPeriod, SC_NS); // Deassert TXI after a pclk period
        } else {
        // Clear TXI interrupt
            mAssertTXI = false;
            TXI.write(false); // Deassert TXI interrupt
        }
    } else {
        //Initialize interrupt ports
        TXI.write(false);
    }
}//}}}

void Csci3::TEIInterruptMethod(void)
{//{{{
    if (!(mSCI3Reset||mCmdReset)) {
        // Issue TEI interrupt
        if (mClearTEI) {
            TEI.write(false); // Cancel TEI interrupt
            mClearTEI = false;
        } else {
            TEI.write(true); // Assert TEI interrupt
            DumpInterruptMsg(emTransmitEndIntr); // Dump TEI interrupt message
        }
    } else {
        //Initialize interrupt ports
        TEI.write(false);
    }
}//}}}

void Csci3::RXIInterruptMethod(void)
{//{{{
    if (!(mSCI3Reset||mCmdReset)) {
        // Issue RXI interrupt
        if (!mAssertRXI) {
            mAssertRXI = true;
            RXI.write(true); // Assert RXI interrupt (rising edge protocol)
            DumpInterruptMsg(emReceiveFullIntr); // Dump RXI interrupt message
            mRXIInterruptEvent.notify(mPCLKPeriod, SC_NS); // Deassert RXI after a pclk period
        } else {
        // Clear RXI interrupt
            mAssertRXI = false;
            RXI.write(false); // Deassert RXI interrupt
        }
    } else {
        //Initialize interrupt ports
        RXI.write(false);
    }
}//}}}

void Csci3::ERIInterruptMethod(void)
 {//{{{
    if (!(mSCI3Reset||mCmdReset)) {
        // Issue ERI interrupt
        if (mClearERI) {
            ERI.write(false); // Cancel ERI interrupt
            mClearERI = false;
        } else {
            ERI.write(true); // Assert ERI interrupt
            DumpInterruptMsg(emReceiveErrorIntr); // Dump ERI interrupt message
        }
    } else {
        //Initialize interrupt ports
        ERI.write(false);
    }
}//}}}

void Csci3::CheckBreakMethod(void)
{//{{{
    CSciIF ReceiveDataPacket;
    ProcessReceivedData(ReceiveDataPacket);
    if ((mOperationMode == emAsynMode)&&(ReceiveDataPacket.GetBreak())) {
        // Set SSR.FER to 1 if SCI3 still receives break signal
        SSR_FER = 1;
        if (SMR_MP == 0) { // Process receiving parity bit
            if (SMR_PE == 1) {
                if (!(ReceiveDataPacket.GetParityBit() == GetParity(mReceiveDataPacket.GetData()))) {
                    SSR_PER = 1; // Set parity error flag to 1
                }
            }
        }
        // Issue ERI interrupt if ERI is negated
        if (ERI.read() == false) {
            IssueRecIntr(true);
        }
        mCheckBreakEvent.notify(mFramePeriod, SC_NS);
    }
}//}}}

void Csci3::IssueRecIntr (const bool error_flag)
{//{{{
    if (SCR_RIE == 1) {
        // Check error flags to decide to issue receive error interrupt or receive data full interrupt
        if (error_flag){
            NotifyInterruptEvent(emReceiveErrorIntr); // Issue ERI interrupt
        } else {
            NotifyInterruptEvent(emReceiveFullIntr); // Issue RXI interrupt
        }
    }
}//}}}

void Csci3::SetUpGeneralSCI (void)
{//{{{
    // Transmission buffer has 1 byte
    Cgeneral_sci::Set_Parameter("Tx_FIFO_depth", 1, emChannelIndex);
    // Reception buffer is FIFO and has 1 bytes
    Cgeneral_sci::Set_Parameter("Rx_FIFO_depth", emDefaultNumOfRecBuf, emChannelIndex);
    // If transmission buffer < 1, it mean transmission buffer is empty
    Cgeneral_sci::Set_Parameter("Tx_FIFO_trigger", 1, emChannelIndex);
    // If reception buffer >= 1,it mean reception buffer is full
    Cgeneral_sci::Set_Parameter("Rx_FIFO_trigger", emDefaultNumOfRecBuf, emChannelIndex);
    Cgeneral_sci::Set_Parameter("Tx_rate", 0, emChannelIndex);
    Cgeneral_sci::Set_Parameter("Tx_stop", 1, emChannelIndex);
    Cgeneral_sci::Set_InterruptEnable("Tx_empty_en", true, emChannelIndex);
    Cgeneral_sci::Set_InterruptEnable("Rx_full_en", true, emChannelIndex);
}//}}}

void Csci3::StartTransmission(void)
{//{{{
    unsigned int data = TDR_TDR;
    mTSRreg = TDR_TDR&0xFFFF;
    if (mTransStatus == "t_trans") { // Start continuous transmission if TDRE is cleared to 0 or TDR is written when SCI3 is transmitting
        mIsContTransmit = true;
    }
    if ((mOperationMode == emAsynMode)&&(SMR_CHR == 1)&&(SCMR_CHR1 == 1)) {
        data &= 0x7F;
    }
    
    mTxTransNum = mTxTransNum + 1;
    if (!mIsSlaveTrans) { 
        Cgeneral_sci::Gsci_Clear(emChannelIndex);
        Cgeneral_sci::Set_TxFIFO(data,emChannelIndex);
    }
}//}}}

void Csci3::WriteSCKO(const double value, const bool receiving_flag)
{//{{{
    if (SCR_CKE1 == 1) { // SCKO is not used
        mSCKO = 0;
    } else {
        if (mOperationMode == emAsynMode) {
            if (SCR_CKE0 == 0) { // SCKO is not used
                mSCKO = 0;
            } else {
                mSCKO = value;
            }
        } else {
            mSCKO = value;
            if ((SCR_RE == 1)&&(mSCKO != 0)&&(!receiving_flag)) { // Synchronize reception with transmission
                mReceiveEvent.notify();
            }
        }
    }
    mWriteSCKOEvent.notify();
}//}}}

double Csci3::BitRateCalc(void)
{//{{{
    double bit_rate = 0;
    if (SCR_CKE1 == 1) { // Use external clock SCK
        if (mSCKI != 0) {
            if (mOperationMode == emAsynMode) { // Asynchronous operation mode
                bit_rate = mSCKI/((double)(8 << (1 - SEMR_ABCS)));  // ABCS=0: bit_rate/16; ABCS=1: bit_rate/8
            } else { // Clocked synchronous operation mode
                bit_rate = mSCKI;
            }
        } else {
            re_printf("error","SCKI must be different from 0.\n");
        }
    } else { // Use internal clock pclk
        if (mPCLKFreq > 0) {
            bit_rate = mPCLKFreq/((((double)mBRRVal) + 1)*((double)BitRateCoeffCal()));
            if (SEMR_BRME == 1) {
                bit_rate = bit_rate/(256/((double)mMDDRVal));
            }
        } else {
            re_printf("error","pclk must be greater than 0.\n");
        }
    }
    return bit_rate;
}//}}}

unsigned int Csci3::BitRateCoeffCal(void)
{//{{{
    unsigned int BitRateCoeff = 0;
    // Check Operation mode and get corresponding coefficient value
    if (mOperationMode == emAsynMode) {
        // Check basic clock select bit in Asynchronous mode
        if ((SEMR_ABCS == 0)&&(SEMR_BGDM == 0)) {
            BitRateCoeff = 64;
        } else if ((SEMR_ABCS == 1)&&(SEMR_BGDM == 1)) {
            BitRateCoeff = 16;
        } else {
            BitRateCoeff = 32;
        }
    } else {
        BitRateCoeff = 8;
    }
    // Calculate bit rate coefficient by formula : BitRateCoeff = BitRateCoeff*2^(2n-1)
    if ((SMR_CKS1 == 0)&&(SMR_CKS0 == 0)) {
        BitRateCoeff = BitRateCoeff >> 1;
    } else {
        BitRateCoeff = BitRateCoeff << (2*((SMR_CKS1 << 1)|SMR_CKS0) - 1);
    }
    return BitRateCoeff;
}//}}}

double Csci3::TransmitProcess(const double period)
{//{{{
    // Increase total transmited data amount
    mTransmitDataAmount ++;
    DumpOperationInfo("Transmitting"); // Dump transmission info
    double transmit_time = TransTimeCalc(period,false);
    return transmit_time;
}//}}}

double Csci3::TransTimeCalc(const double period, const bool receiving_flag)
{//{{{
    double transfer_time = 0;
    unsigned int data_length = 0;
    unsigned int parity_length = 0;
    if (SCMR_CHR1 == 0) {
        data_length = em16BitsData;
    } else {
        data_length = emEightBitData;
    }
    parity_length = emNoParity;
    if (mOperationMode == emAsynMode) { // Asynchronous operation mode
        // Get data length of transfer data packet
        if ((SMR_CHR == 1)&&(SCMR_CHR1 == 1)) {
            data_length = emSevenBitData;
        } 
        if (receiving_flag) { // Calculate transfer time for reception
            transfer_time = 1 + (double)data_length - 0.5;
        } else { // Calculate transfer time for transmission
            // Get parity bit length of transfer data packet.
            if (((SMR_MP == 1)&&(SCMR_CHR1 == 1)) || (SMR_PE == 1)) {
                parity_length = emOnePSMBit; // Parity bit length always is 1 when multiprocessor function is enable
            }
            transfer_time = 1 + (double)data_length + (double)parity_length;
            // Set size of transmit data packet before transmitting
            mTransmitDataPacket.SetSize(data_length + parity_length);
        }
    } else { // Clocked synchronous operation mode
        transfer_time = (double)data_length - 0.5;
        mTransmitDataPacket.SetSize(data_length);
    }
    transfer_time *= period;
    return transfer_time;
}//}}}

bool Csci3::FrameChecking(void)
{//{{{
    bool frame_error = false;
    unsigned int size = 0;
    unsigned int data_length = 0;
    unsigned int parity_length = 0;
    unsigned int direction = (mReceiveDataPacket.GetDir()) ? 1 : 0;
    // Check direction of received data packet
    if (SCMR_CHR1 == 1) {
        if ((mOperationMode == emClockedSynMode)||((mOperationMode == emAsynMode)&&(SMR_CHR == 0))) {
            if (direction != SCMR_SDIR) {
                frame_error = true;
            }
        } else {
            if (direction == 1) {
                frame_error = true;
            }
        }
        // Only dump warning message when direction of received data is mismatched
        if (frame_error) {
            frame_error = false; // Checking direction does not affect the FER flag in SSR
            re_printf("warning","Direction of received data packet is mismatched with SCI3 setting.\n");
        }
    }
    // Check size of received data packet
    if (SCMR_CHR1 == 0) {
        data_length = em16BitsData;
    } else {
        data_length = emEightBitData;
    }
    parity_length = emNoParity;
    if (mOperationMode == emAsynMode) { // Asynchronous operation mode
        if ((SMR_CHR == 1)&&(SCMR_CHR1 == 1)) {
            data_length = emSevenBitData;
        } 
        // Get parity bit length of transfer data packet.
        if (((SMR_MP == 1)&&(SCMR_CHR1 == 1)) || (SMR_PE == 1)) {
            parity_length = emOnePSMBit; // Parity bit length always is 1 when multiprocessor function is enable
        }
    } 
    size = data_length + parity_length;
    // Dump warning message and set FER flag in SSR to 1 when size of received data packet is mismatched
    if (mReceiveDataPacket.GetSize() != size) {
        frame_error = true;
        re_printf("warning","Size of received data packet is mismatched with SCI3 setting.\n");
    }
    return frame_error;
}//}}}

bool Csci3::CompareRecDataMatch(void)
{//{{{
    // Check multiprocessor bit of received data packet when multiprocessor function is enabled in Asynchronous operation mode
    if (mOperationMode == emAsynMode) {
        if (SMR_MP == 1) {
            SSR_MPB = (mReceiveDataPacket.GetParityBit()) ? 1 : 0; // Get multiprocessor bit of received data packet
            // Return false when SCI3 is waiting for ID packet but received packet is data packet
            if ((SCR_MPIE == 1)&&(SSR_MPB == 0)) {
                return false;
            }
        }
    }
    return true;
}//}}}

void Csci3::NotifyInterruptEvent(const unsigned int interrupt_id)
{//{{{
    mInterruptKind |= interrupt_id;
    if ((mInterruptKind&emTransmitEmptyIntr) == emTransmitEmptyIntr) {
        mTXIInterruptEvent.notify();
    }
    if ((mInterruptKind&emTransmitEndIntr) == emTransmitEndIntr) {
        mTEIInterruptEvent.notify();
    }
    if ((mInterruptKind&emReceiveErrorIntr) == emReceiveErrorIntr) {
        mERIInterruptEvent.notify();
    }
    if ((mInterruptKind&emReceiveFullIntr) == emReceiveFullIntr) {
        mRXIInterruptEvent.notify();
    }
    mInterruptKind = 0;
}//}}}

void Csci3::DumpInterruptMsg (const unsigned int interrupt_id)
{//{{{
    if(DumpInterrupt){
        switch(interrupt_id){
            case emReceiveErrorIntr:
                re_printf("info"," INT [SCI3: ERI] Assert\n");
                break;
            case emReceiveFullIntr:
                re_printf("info"," INT [SCI3: RXI] Assert\n");
                break;
            case emTransmitEmptyIntr:
                re_printf("info"," INT [SCI3: TXI] Assert\n");
                break;
            default:
                re_printf("info"," INT [SCI3: TEI] Assert\n");
        }
    }
}//}}}

bool Csci3::ScifTxData(const unsigned char data, const unsigned int ch_number)
{//{{{
    mTxTransNum = mTxTransNum - 1;
    // Set start bit of transmit data packet (StarBit = 0 when transmit data)
    mTransmitDataPacket.SetStartBit(false);
    // Set stop bit of transmit data packet (StopBit = 0 when transmit data)
    mTransmitDataPacket.SetStopBit(0x0);
    bool parity_bit = false;
    // Setting start bit, multiprocessor bit, parity bit and stop bit valid only in Asynchronous mode
    if (mOperationMode == emAsynMode) {
        if (SCMR_CHR1 == 1) {
            if (SMR_MP == 1) {
                parity_bit = (SSR_MPBT == 0) ? false : true;
                mTransmitDataPacket.SetParityBit(parity_bit); // Set multiprocessor bit of transmit data packet
            } else if (SMR_PE == 1) {
                mTransmitDataPacket.SetParityBit(GetParity(data)); // Set parity bit of transmit data packet
            }
        }
    }
    // Set data of transmit data packet
    if (SCMR_CHR1 == 0) {
            mTransmitDataPacket.SetData(mTSRreg);
            parity_bit = GetParity(mTSRreg);
        if ((mOperationMode == emAsynMode)&&(SMR_PE == 1)) {
            mTransmitDataPacket.SetParityBit(parity_bit);
        }
    } else {
        mTransmitDataPacket.SetData(data);
    }
    // Set choosing direction of transmit data packet
    mTransmitDataPacket.SetDir(false); // Set choosing of LSB-first
    if ((mOperationMode == emClockedSynMode)||((mOperationMode == emAsynMode)&&(SMR_CHR == 0))) {
        if ((SCMR_SDIR == 1)&&(SCMR_CHR1 == 1)) {
            mTransmitDataPacket.SetDir(true); // Set choosing of MSB-first
        }
    }
    if (!(mIsContTransmit)) { // Increase the sequential number of transmit data packet if it is not series data
        mTransSeq++;
        mTransmitDataPacket.SetSeq(mTransSeq);
    }
    if ((mIsSlaveTrans == false)&&(mSCKI == 0)&&(mOperationMode == emClockedSynMode)&&(SCR_CKE1 == 1)) { //slave in syn mode, start transmit before receiving SCKI clock
        if (SCR_TIE == 1) {
            NotifyInterruptEvent(emTransmitEmptyIntr); // Notify TXI interrupt
        }
        SSR_TDRE = 1;
        mIsSlaveTrans = true; 
    } else {
        mTransmitEvent.notify(); // Start transmission progress
    }
    return true;
}//}}}

void Csci3::Scif_interrupt (const unsigned int interrupt_id,const unsigned int ch_number)
{//{{{
}//}}}

bool Csci3::GetParity(const unsigned int data)
{//{{{
    unsigned char one_cnt = 0;
    unsigned int temp_data = (unsigned int) data;
    bool parity_bit = false;
    if ((mOperationMode == emAsynMode)&&(SMR_CHR == 1)&&(SCMR_CHR1 == 1)) {
        temp_data = temp_data&0x7F;
    }
    // Count number of 1 bit in data
    while (temp_data != 0) {
        temp_data &= (temp_data - 1);
        one_cnt ++;
    }
    // Process even/odd parity mode
    if (SMR_PM == 0) {
        parity_bit = ((one_cnt&1) == 1) ? true : false;
    } else {
        parity_bit = ((one_cnt&1) == 0) ? true : false;
    }
    return parity_bit;
}//}}}

void Csci3::tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t)
{ //{{{
    // Get transaction attributes
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if (!status) {
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    // Callback method
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        UpdateRegVal(addr&0x1F);
        status = reg_rd((unsigned int) addr, p_data, size);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        UpdateRegVal(addr&0x1F);
        status = reg_wr((unsigned int) addr, p_data, size);
        UpdateRegVal(addr&0x1F);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
    }

    // Set response status attribute
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
} //}}}

unsigned int Csci3::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    // Get transaction attributes
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    // Callback method
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        UpdateRegVal(addr&0x1F);
        status = reg_rd_dbg((unsigned int) addr, p_data, size);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        UpdateRegVal(addr&0x1F);
        status = reg_wr_dbg((unsigned int) addr, p_data, size);
        UpdateRegVal(addr&0x1F);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
        size = 0;
    }

    // Set response status attribute and return value
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

bool Csci3::CheckWriteRegister(const char *str_name)
{//{{{
    if ((SCR_TE == 1) || (SCR_RE == 1)) {   //check whether TE or RE equal to 1 or not
        std::string operation = "";
        if (mOperationMode == emAsynMode) {
            operation = "Asynchronous mode";
        } else {
            operation = "Clocked synchronous mode";
        }
        re_printf("warning","%s should be written when TE bit and RE bit are 0 in %s.\n",str_name,operation.c_str());
        return false;
    }
    return true;
}//}}}

void Csci3::DumpStatInfo(void)
{//{{{
    const char *type = "StatInfo";
    DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
    std::string transmit_en_status = "";
    std::string transmit_status = "";
    std::string receive_en_status = "";
    std::string receive_status = "";
    // Check transmission status of SCI3
    if (SCR_TE == 1) {
        transmit_en_status = "Enabled";
        if (mTransStatus == "t_ready") {
            transmit_status = " - Waiting Data";
        } else {
            transmit_status = " - Transferring";
        }
    } else {
        transmit_en_status = "Disabled";
    }
    if (SCR_RE == 1) {
        receive_en_status = "Enabled";
        if (mRecStatus == "r_ready") {
            receive_status = " - Waiting Data";
        } else {
            receive_status = " - Transferring";
        }
    } else {
        receive_en_status = "Disabled";
    }
    if ((mTransStatus == "resetting")&&(mRecStatus == "resetting")){
        transmit_en_status = "Resetting";
        receive_en_status = "Resetting";
    }
    // Dump statistical information about transmitting/receiving activity
    DumpInfo(type, "SCI3 transfer information\n");
    DumpInfo(type, " Total data transmitted: %d byte(s)\n", mTransmitDataAmount);
    DumpInfo(type, " Total data received   : %d byte(s)\n", mReceiveDataAmount);
    DumpInfo(type, " Transmit status : %s%s\n", transmit_en_status.c_str(), transmit_status.c_str());
    DumpInfo(type, " Receive status  : %s%s\n", receive_en_status.c_str(), receive_status.c_str());
    DumpInfo(type, "EndInfo.\n");
    // Reset data amount counter
    mTransmitDataAmount = 0;
    mReceiveDataAmount = 0;
}//}}}

void Csci3::DumpInfo(const char *type, const char *message, ...)
{//{{{
    sc_assert((message != NULL) && (type != NULL));
    printf ("PROFILE(%s): SCI3: ", type);
    // Print body
    va_list argptr;
    va_start(argptr, message);
    vprintf(message, argptr);
    va_end(argptr);
}//}}}

void Csci3::DumpEnableTransInfo(const bool status)
{//{{{
    if (EnableTransInfo) {
        unsigned int SMR_val = (SMR_CM   << 7)
                             | (SMR_CHR  << 6)
                             | (SMR_PE   << 5)
                             | (SMR_PM   << 4)
                             | (SMR_STOP << 3)
                             | (SMR_MP   << 2)
                             | (SMR_CKS1 << 1)
                             | (SMR_CKS0 << 0);
        unsigned int SCR_val = (SMR_CM   << 7)
                             | (SCR_TIE  << 7)
                             | (SCR_RIE  << 6)
                             | (SCR_TE   << 5)
                             | (SCR_RE   << 4)
                             | (SCR_MPIE << 3)
                             | (SCR_TEIE << 2)
                             | (SCR_CKE1 << 1)
                             | (SCR_CKE0 << 0);
        unsigned int SCMR_val = (SCMR_BCP2           << 7)
                              | (SCMR_RESERVED_BIT1  << 4)
                              | (SCMR_SDIR           << 3)
                              | (SCMR_SINV           << 2)
                              | (SCMR_ASTLS  << 1)
                              | (SCMR_SMIF           << 0);
        unsigned int SEMR_val = (SEMR_BRME   << 7)
                              | (SEMR_MDDRS  << 6)
                              | (SEMR_ABCS   << 3)
                              | (SEMR_RXDMON << 2);
        const char *type = "TransInfo";
        const char *status_str = (status) ? "on" : "off";
        // Dump transition information of SCI3 when transmission or reception is enabled/disabled
        DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
        DumpInfo(type, "SCI3 model is turned %s\n", status_str);
        DumpInfo(type, " Time          : %s\n", sc_time_stamp().to_string().c_str());
        DumpInfo(type, " Register value:\n");
        DumpInfo(type, "  . SMR  = 0x%02X\n", SMR_val);
        DumpInfo(type, "  . SCR  = 0x%02X\n", SCR_val);
        DumpInfo(type, "  . SCMR = 0x%02X\n", SCMR_val);
        DumpInfo(type, "  . SEMR = 0x%02X\n", SEMR_val);
        DumpInfo(type, "  . BRR  = 0x%02X\n", mBRRVal);
        DumpInfo(type, "  . MDDR = 0x%02X\n", mMDDRVal);
        DumpInfo(type, "EndInfo.\n");
    }
}//}}}

void Csci3::DumpOperationInfo(const char *operation)
{//{{{
    if (EnableTransInfo) {
        const char *type = "TransInfo";
        sc_assert(operation != NULL);
        unsigned int amount_data = (strcmp(operation,"Transmitting") == 0) ? mTransmitDataAmount : mReceiveDataAmount;
        // Dump information when SCI3 transmitting/receiving data packet
        DumpInfo(type, "Info [%20s] (%10s):\n", sc_time_stamp().to_string().c_str(), this->name());
        DumpInfo(type, "SCI3 data transmit/receive:\n");
        DumpInfo(type, " Time       : %s\n", sc_time_stamp().to_string().c_str());
        DumpInfo(type, " Operation  : %s data\n", operation);
        DumpInfo(type, " Total data amount: %d byte(s)\n", amount_data);
        DumpInfo(type, "EndInfo.\n");
    }
}//}}}

void Csci3::cb_RDR_RDR(RegCBstr str)
{//{{{
    if (SSR_RDRF == 0) {
        re_printf("warning","RDR should not be read when RDRF is equal to 0.\n");
    } else {
        SSR_RDRF = 0;
        UpdateRegVal(0x0010);
    }
}//}}}

void Csci3::cb_TDR_TDR(RegCBstr str)
{//{{{
    if (SSR_TDRE == 0) {
        re_printf("warning","TDR should not be written when TDRE is equal to 0.\n");
        TDR_TDR = str.pre_data; // Retain previous value
    } else {
        if (mPCLKFreq == 0) {
            return;
        }
        if (SCR_TE == 1) {
            // Clear TDRE and TEND flags in SSR to 0 when TDR is written
            SSR_TDRE = 0;
            SSR_TEND = 0;
            // Cancel TEI interrupt when writing to TDR in state TE=1
            mClearTEI = true;
            NotifyInterruptEvent(emTransmitEndIntr);
            // Transfer data from TDR to TxFIFO to start transmission
            if (mTransStatus == "t_ready") {
                StartTransmission();
            }
        }
    }
}//}}}

void Csci3::cb_SMR_CM(RegCBstr str)
{//{{{
    if (!CheckWriteRegister("SMR")) {
        (*SMR) = str.pre_data; // Retain previous value
        unsigned int val = str.pre_data;
        SMR_CM   = (val>>7)&1;
        SMR_CHR  = (val>>6)&1;
        SMR_PE   = (val>>5)&1;
        SMR_PM   = (val>>4)&1;
        SMR_STOP = (val>>3)&1;
        SMR_MP   = (val>>2)&1;
        SMR_CKS1 = (val>>1)&1;
        SMR_CKS0 = (val>>0)&1;
    } else {
        if (mPCLKFreq == 0) {
            return;
        }
        if ((str.data&0x80) == 0) {
            mOperationMode = emAsynMode; // The SCI3 model operates in Asynchronous mode
        } else {
            mOperationMode = emClockedSynMode; // The SCI3 model operates in Clocked synchronous mode
        }
    }
}//}}}

void Csci3::cb_SCR_RIE(RegCBstr str)
{//{{{
    if (mPCLKFreq == 0) {
        return;
    }
    if (((str.data&0x40) == 0)&&((str.pre_data&0x40) == 0x40)) {
        // Cancel ERI interrupt when clearing RIE from 1 to 0
        mClearERI = true;
        NotifyInterruptEvent(emReceiveErrorIntr);
    }
}//}}}

void Csci3::cb_SCR_TE(RegCBstr str)
{//{{{
    bool write_TE = false;
    // Write TE when writing simultaneously TE=RE=0 or SCI3 operates in Asynchronous mode or RE=0 in Clocked synchronous mode
    if ((str.pre_data&0x20) != (str.data&0x20)) {
        if (((str.data&0x30) == 0)||(mOperationMode == emAsynMode)) {
            write_TE = true;
        } else if (mOperationMode == emClockedSynMode) {
            if ((str.pre_data&0x30) == 0) { // Check TE=RE=0 before writing to TE
                write_TE = true;
            } else {
                re_printf("warning","TE should be written when RE bit is 0 in Clocked synchronous mode.\n");
            }
        }
    }
    if (write_TE) {
        if (mPCLKFreq == 0) {
            return;
        }
        if ((str.data&0x20) == 0) {
            // Stopping transmitting data
            mIsContTransmit = false;
            mCancelTransmitEvent.notify();
            // Stopping SCKO clock
            if (((mOperationMode == emAsynMode)&&(mRecStatus != "r_rec"))||(mOperationMode == emClockedSynMode)) {
                WriteSCKO(0,false);
            }
            // Check PDR port to output break signal
            if (mOperationMode == emAsynMode) {
                mIsSendingBreak = true;
                // Write break signal/mark signal when TE is cleared from 1 to 0
                mTransmitDataPacket.SetBreak(true);
                mTransSeq++;
                mTransmitDataPacket.SetSeq(mTransSeq);
                if (PDR.read()) {
                    re_printf("info","Mark signal is sent.\n");
                } else {
                    re_printf("info","Break signal is sent.\n");
                }
                mWriteTxDEvent.notify();
            }
            // Write default data packet with StartBit = 1 when clearing TE from 1 to 0 and SCI3 is transmitting
            if ((!mIsSendingBreak)&&(mTransStatus == "t_trans")) {
                mTransmitDataPacket.Initialize();
                mWriteTxDEvent.notify();
            }
            // Initialize transmission status
            mTransStatus = "t_idle";
            mIsSlaveTrans = false;
            // Initialize SSR.TDRE and SSR.TEND
            SSR_TDRE = 1; // Fix TDRE and TEND to 1 when clearing TE to 0
            SSR_TEND = 1;
            // Dump EnableTransInfo message when SCI3 is turned off (TE=RE=0)
            if (((str.data&0x10) == 0)&&((str.pre_data&0x20) == 0x20)) {
                DumpEnableTransInfo(false);
            }
        } else {
            mTransStatus = "t_ready"; // Change status of transmission state to t_ready when enable transmission
            // Write default data packet to TxD port when TE is set to 1 and SCI3 is transmitting break signal
            if (mIsSendingBreak) {
                mIsSendingBreak = false;
            }
            mTransmitDataPacket.Initialize();
            mWriteTxDEvent.notify();
            // Dump EnableTransInfo message when write 1 to TE or both TE and RE while TE=RE=0
            if ((str.pre_data&0x30) == 0) {
                DumpEnableTransInfo(true);
            }
        }
    } else {
        SCR_TE = (str.pre_data&0x20)>>5; // Retain previous value
    }
}//}}}

void Csci3::cb_SCR_RE(RegCBstr str)
{//{{{
    bool write_RE = false;
    // Write RE when writing simultaneously TE=RE=0 or SCI3 operates in Asynchronous mode or TE=0 in Clocked synchronous mode
    if ((str.pre_data&0x10) != (str.data&0x10)) {
        if (((str.data&0x30) == 0)||(mOperationMode == emAsynMode)) {
            write_RE = true;
        } else if (mOperationMode == emClockedSynMode) {
            if ((str.pre_data&0x30) == 0) { // Check TE=RE=0 before writing to RE
                write_RE = true;
            } else {
                re_printf("warning","RE should be written when TE bit is 0 in Clocked synchronous mode.\n");
            }
        }
    }
    if (write_RE) {
        if (mPCLKFreq == 0) {
            return;
        }
        if ((str.data&0x10) == 0) {
            // Stopping receiving data
            mReceiveLatencyFlag = false;
            mCancelReceiveEvent.notify();
            // Stopping SCKO clock
            if (((mOperationMode == emAsynMode)&&(mTransStatus != "t_trans"))||(mOperationMode == emClockedSynMode)) {
                WriteSCKO(0,true);
            }
            // Initialize reception status
            mRecStatus = "r_idle";
            // Dump EnableTransInfo message when SCI3 is turned off (TE=RE=0)
            if (((str.data&0x20) == 0)&&((str.pre_data&0x30) == 0x10)) {
                DumpEnableTransInfo(false);
            }
        } else {
            mRecStatus = "r_ready"; // Change status of reception state to r_ready when enable reception
            if ((mOperationMode == emClockedSynMode)&&(SCR_CKE1 == 0)&&((str.data&0x20) == 0)) { // Start reception when only RE is set from 0 to 1
                mReceiveLatencyFlag = true;
                mReceiveEvent.notify();
            }
            // Dump EnableTransInfo message when write 1 to RE while TE=RE=0
            if (((str.pre_data&0x30) == 0)&&((str.data&0x30) == 0x10)) {
                DumpEnableTransInfo(true);
            }
        }
    } else {
        SCR_RE = (str.pre_data&0x10)>>4; // Retain previous value
    }
}//}}}

void Csci3::cb_SCR_TEIE(RegCBstr str)
{//{{{
    if (mPCLKFreq == 0) {
        return;
    }
    if (((str.data&0x4) == 0)&&((str.pre_data&0x4) == 0x4)) {
        // Cancel TEI interrupt when clearing TEIE from 1 to 0
        mClearTEI = true;
        NotifyInterruptEvent(emTransmitEndIntr);
    }
}//}}}

void Csci3::cb_SCR_CKE1(RegCBstr str)
{//{{{
    if ((str.data&0x30) != 0) {
        if (((str.pre_data&0x30) != 0)&&((str.pre_data&0x2) != (str.data&0x2))) {
            re_printf("warning","CKE1 should be written when TE bit and RE bit are 0.\n");
            SCR_CKE1 = (str.pre_data&0x2)>>1; // Retain previous value
        }
    }
}//}}}

void Csci3::cb_SCR_CKE0(RegCBstr str)
{//{{{
    if ((str.data&0x30) != 0) {
        if (((str.pre_data&0x30) != 0)&&((str.pre_data&0x1) != (str.data&0x1))) {
            re_printf("warning","CKE0 should be written when TE bit and RE bit are 0.\n");
            SCR_CKE0 = str.pre_data&0x1; // Retain previous value
        }
    }
}//}}}

void Csci3::cb_SSR_TDRE(RegCBstr str)
{//{{{
    if (mPCLKFreq == 0) {
        return;
    }
    if (str.is_wr) { // Write callback
        if (mSSRIsRead) {
            if ((str.data&0x80) == 0) { // Write 0 to TDRE bit after reading
                if (SCR_TE == 0) {
                    re_printf("warning","TDRE should not be cleared when TE bit is 0.\n");
                    SSR_TDRE = (str.pre_data&0x80)>>7;
                } else {
                    if ((str.pre_data&0x80) == 0x80) { // Clear TDRE to 0 after reading 1
                        // Cancel TEI interrupt when clear TDRE to 0
                        SSR_TEND = 0;
                        mClearTEI = true;
                        NotifyInterruptEvent(emTransmitEndIntr);
                        // Transfer data from TDR to TxFIFO to start transmission
                        if (mTransStatus == "t_ready") {
                            StartTransmission();
                        }
                    }
                }
            }
            if (((str.data&0x40) == 0)&&((str.pre_data&0x40) == 0x40)) { // Clear RDRF to 0 after reading 1
                // Cancel ERI interrupt when clearing RDRF
                mClearERI = true;
                NotifyInterruptEvent(emReceiveErrorIntr);
            }
            if ((str.pre_data&0x38) != 0) { // Write 0 to ORER, FER and PER bit after reading 1
                // When SCI3 is receiving break signal and TransTimeCalcMode = false, user cannot clear FER to 0 in Asynchronous mode
                if ((mOperationMode == emAsynMode)&&(!TransTimeCalcMode)&&(SEMR_RXDMON == 0)) {
                    SSR_FER = 1;
                }
                if (SSR_ORER == 0 && SSR_FER == 0 && SSR_PER == 0) {
                    // Cancel ERI interrupt when ORER, FER and PER are cleared to 0
                    mClearERI = true;
                    NotifyInterruptEvent(emReceiveErrorIntr);
                }
            }
            mSSRIsRead = false;
        }
    } else {
        mSSRIsRead = true;
    }
}//}}}

void Csci3::cb_BRR_BRR(RegCBstr str)
{//{{{
    if (SEMR_MDDRS == 0) { // Access BRR register
        if (CheckWriteRegister("BRR")) {
            mBRRVal = str.data;
        } else {
            (*BRR) = str.pre_data; // Retain previous value
            BRR_BRR = str.pre_data;
        }
    } else { // Access MDDR register
        if (CheckWriteRegister("MDDR")) {
            if (str.data >= 128) { // Update MDDR value if written data >= 128
                mMDDRVal = str.data;
            } else {
                re_printf("warning","Setting value of MDDR must be greater than or equal to 128.\n");
                (*BRR) = str.pre_data; // Retain previous value
                BRR_BRR = str.pre_data;
            }
        } else {
            (*BRR) = str.pre_data; // Retain previous value
            BRR_BRR = str.pre_data;
        }
    }
}//}}}

void Csci3::cb_SCMR_SMIF(RegCBstr str)
{//{{{
    if (CheckWriteRegister("SCMR")) {
        if ((str.data&0x1) == 1) {
            re_printf("warning","Smart card interface mode is not supported. Setting 1 to SMIF bit in SCMR is prohibited.\n");
            (*SCMR)["SMIF"] = str.pre_data&0x1; // Retain previous value
            SCMR_SMIF = str.pre_data&0x1;
        }
    } else {
        (*SCMR) = str.pre_data; // Retain previous value
        SCMR_BCP2           = (str.pre_data>>7)&0x1;
        SCMR_RESERVED_BIT1  = (str.pre_data>>5)&0x3;
        SCMR_CHR1           = (str.pre_data>>4)&0x1;
        SCMR_SDIR           = (str.pre_data>>3)&0x1;
        SCMR_SINV           = (str.pre_data>>2)&0x1;
        SCMR_ASTLS          = (str.pre_data>>1)&0x1;
        SCMR_SMIF           = (str.pre_data>>0)&0x1;
    }
}//}}}

void Csci3::cb_SEMR_BRME(RegCBstr str)
{//{{{
    if (!CheckWriteRegister("SEMR")) {
        (*SEMR)["BRME"] = (str.pre_data&0x80)>>7; // Retain previous value
        SEMR_BRME = (str.pre_data&0x80)>>7;
        (*SEMR)["BGDM"] = (str.pre_data&0x10)>>4; // Retain previous value
        SEMR_BGDM = (str.pre_data&0x10)>>4;
        (*SEMR)["ABCS"] = (str.pre_data&0x8)>>3; // Retain previous value
        SEMR_ABCS = (str.pre_data&0x8)>>3;
    }
    if (!CheckWriteRegister("MDDRS")) {
        (*SEMR)["MDDRS"] = (str.pre_data&0x40)>>6; // Retain previous value
        SEMR_MDDRS = (str.pre_data&0x40)>>6;
    } else {
        if (mPCLKFreq == 0) {
            return;
        }
        if ((str.data&0x40) == 0) { // Access BRR register
            BRR_BRR = mBRRVal;
        } else { // Access MDDR register
            BRR_BRR = mMDDRVal;
        }
    }
}//}}}

void Csci3::SetLatency_TLM(const double pclk_period, const bool is_constructor)
{//{{{
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}

void Csci3::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    if (clk_name == "pclk") {
        mPCLKFreq = 0;
        mPCLKPeriod = 0;
        if (clk_freq < 0) {
            re_printf("error","Clock frequency is invalid (%lld.%06d).\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
            return;
        } else if (clk_freq == 0) {
            //cancel processes of sci3
            mTransmitEvent.cancel();
            mReceiveEvent.cancel();
            mTXIInterruptEvent.cancel();
            mTEIInterruptEvent.cancel();
            mRXIInterruptEvent.cancel();
            mERIInterruptEvent.cancel();
            mWriteSCKOEvent.cancel();
            mReadRxDEvent.cancel();
            mWriteTxDEvent.cancel();
            mCheckBreakEvent.cancel();
            if (mTransStatus != "t_idle") {
                mCancelTransmitEvent.notify();
            }
            if (mRecStatus != "r_idle") {
                mCancelReceiveEvent.notify();
            }
        } else {
            mPCLKFreq = clk_freq;
            //1. Check time unit
            mPCLKPeriod      = (double)(emNanoSecond / clk_freq); // Calculate pclk period
            double time_unit = (double)(emNanoSecond / GetTimeResolution());
            if(time_unit > mPCLKPeriod){
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
            }
        }
        re_printf("info","pclk is set with a frequency as %lld.%06d Hz.\n",(unsigned long long) mPCLKFreq, FRACTION_TO_INT(mPCLKFreq));
        SetLatency_TLM(mPCLKPeriod, false);
    } else {
        re_printf("error","Clock name is invalid.\n");
    }
}//}}}

double Csci3::GetTimeResolution(void)
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)){
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

unsigned int Csci3::TXControlConvert (CSciIF TransmitDataPacket)
{//{{{
    if (SCR_TE == 0) {
        if (TransmitDataPacket.GetBreak()) {
            return emTxControlBreak;   // Transmit break signal packet
        } else {
            return emTxControlDefault; // Transmit mark signal packet
        }
    } else {
        unsigned int tx_control_val = 0;
        if (TransmitDataPacket.GetStartBit()) {
            tx_control_val = emTxControlIdle;    // Transmit idle signal packet
        } else {
            tx_control_val = tx_control_val | (TransmitDataPacket.GetBitt() << 16); // Set BITT value
            tx_control_val = tx_control_val | (0x1 << 15);                          // Set TE = 1, TC = 0
            tx_control_val = tx_control_val | (unsigned int) (TransmitDataPacket.GetDir() << 8);  // Set DIR value
            if (!mIsFirstDataTransmit) {
                if (TransmitDataPacket.GetStopBit() == 0x0) {
                    tx_control_val = tx_control_val | (0x1 << 6);                       // Set STBE = 1
                } else {
                    tx_control_val = tx_control_val | (0x2 << 6);                       // Set STBE = 2
                }
            }
            tx_control_val = tx_control_val | (TransmitDataPacket.GetSize());       // Set SIZE value
        }

        if (mOperationMode == emClockedSynMode) { // Clock-synchronous mode
            tx_control_val = tx_control_val & 0x0000BFFF;   // Clear BITT, TC
            tx_control_val = tx_control_val | (0x3 << 9);   // Set CPOL = 1, CPHA = 1
            unsigned int mstr = (SCR_CKE1 == 0) ? 1 : 0;
            tx_control_val = tx_control_val | (mstr << 14); // Set MSTR
            tx_control_val = tx_control_val | (TransmitDataPacket.GetSize());       // Set SIZE value
        }

        return tx_control_val; // Transmit data packet
    }
}//}}}

unsigned int Csci3::TXDataConvert (CSciIF TransmitDataPacket)
{//{{{
    unsigned int tx_data_val = emTxDataDefault;
    if (TransmitDataPacket.GetBreak()) {
        if (!(PDR.read())) { // PDR = 0, TxD is all-0 (break signal)
            tx_data_val = emTxDataBreak; // Transmit break signal packet
        } // PDR = 1, TxD is all-1 (mark signal)
    } else if ((SCR_TE == 1)&&(!(TransmitDataPacket.GetStartBit()))) {
        if (mOperationMode == emAsynMode) { // Asynchronous operation mode
            unsigned int parity_length = emOnePSMBit;  // Parity bit length always is 1 when multiprocessor function or parity check is enabled
            if (((SMR_MP == 0)&&(SMR_PE == 0))||((SCMR_CHR1 == 0)&&(SMR_PE == 0))) {
                parity_length = emNoParity; // Disable parity bit
            }
            // Check data length of transmit data packet
            if (SCMR_CHR1 == 1) {
                if (SMR_CHR == 1) {
                    tx_data_val = tx_data_val & emSevenBitMask;
                    if (parity_length == emNoParity) {
                        tx_data_val = tx_data_val | (0x1 << emSevenBitData); // Set default value to parity bit
                    } else {
                        tx_data_val = tx_data_val | (unsigned int) (((TransmitDataPacket.GetParityBit()) ? 1 : 0) << emSevenBitData); // Seven bit data
                    }
                    tx_data_val = tx_data_val | ((unsigned int)TransmitDataPacket.GetData() & emDataSevenBitMask);
                } else {
                    tx_data_val = tx_data_val & emEightBitMask;
                    if (parity_length == emNoParity) {
                        tx_data_val = tx_data_val | (0x1 << emEightBitData); // Set default value to parity bit
                    } else {
                        tx_data_val = tx_data_val | (unsigned int) (((TransmitDataPacket.GetParityBit()) ? 1 : 0) << emEightBitData); // Eight bit data
                    }
                    tx_data_val = tx_data_val | ((unsigned int)TransmitDataPacket.GetData() & emDataEightBitMask);
                }
            } else {
                tx_data_val = tx_data_val & em16BitsMask;
                if (parity_length == emNoParity) {
                    tx_data_val = tx_data_val | (0x1 << em16BitsData); // Set default value to parity bit
                } else {
                    tx_data_val = tx_data_val | (unsigned int) (((TransmitDataPacket.GetParityBit()) ? 1 : 0) << em16BitsData); // 16 bit data
                }
                tx_data_val = tx_data_val | ((unsigned int)TransmitDataPacket.GetData() & emData16BitsMask);
            }
        } else { // Clocked-synchronous mode
            if (SCMR_CHR1 == 1) {
                tx_data_val = tx_data_val & emEightBitMask;
                tx_data_val = tx_data_val | (0x1 << emEightBitData); // Set default value into PARITY in clocked synchronous mode
                tx_data_val = tx_data_val | ((unsigned int)TransmitDataPacket.GetData() & emDataEightBitMask);
            } else {
                tx_data_val = tx_data_val & em16BitsMask;
                tx_data_val = tx_data_val | (0x1 << em16BitsData); // Set default value into PARITY in clocked synchronous mode
                tx_data_val = tx_data_val | ((unsigned int)TransmitDataPacket.GetData() & emData16BitsMask);
            }
        }
    }
    return tx_data_val;
}//}}}

void Csci3::ProcessReceivedData (CSciIF &ReceiveDataPacket)
{//{{{
    unsigned int rx_control_val;
    if (mOperationMode == emAsynMode) {
        rx_control_val = (unsigned int)(RX_CONTROL.read());
    } else {
        rx_control_val = (unsigned int)(SPI_CONTROL.read());
    }
    unsigned int stbe = (rx_control_val & emSTBEMask) >> 6;
    unsigned int rx_data_val = (unsigned int)(RX_DATA.read());
    if (stbe == 0x0) {
        ReceiveDataPacket.Initialize();
    } else if (stbe == 0x3) {
        ReceiveDataPacket.SetBreak(true);
    } else {
        ReceiveDataPacket.Initialize();
        ReceiveDataPacket.SetStartBit(false);                   // Set StartBit equal to 0
        ReceiveDataPacket.SetDir((rx_control_val & emDIRMask) >> 8);   // Receive Dir
        ReceiveDataPacket.SetSize(rx_control_val & emSIZEMask); // Receive Size
        ReceiveDataPacket.SetBitt((rx_control_val & emBITTMask) >> 16); // Receive Bitt
        ReceiveDataPacket.SetTe((rx_control_val & emTEMask) >> 15);     // Receive Te
        if (stbe == 0x1) {
            ReceiveDataPacket.SetStopBit(0x0);
        } else { //stbe == 0x2
            ReceiveDataPacket.SetStopBit(0x1);
        }
        if (SCMR_CHR1 == 0) {
            ReceiveDataPacket.SetData(rx_data_val & emData16BitsMask); // Receive 16 bit Data
            if (mOperationMode == emAsynMode) { // Asynchronous operation mode
                ReceiveDataPacket.SetParityBit((rx_data_val & emParity16BitsMask) >> 16); // Receive Parity
            } 
        } else {
            if (mOperationMode == emAsynMode) { // Asynchronous operation mode
                // Check data length of transmit data packet
                if (SMR_CHR == 1) {
                    ReceiveDataPacket.SetData(rx_data_val & emDataSevenBitMask); // Receive 7 bit Data
                    ReceiveDataPacket.SetParityBit((rx_data_val & emParitySevenBitMask) >> 7); // Receive Parity
                } else {
                    ReceiveDataPacket.SetData(rx_data_val & emDataEightBitMask); // Receive 8 bit Data
                    ReceiveDataPacket.SetParityBit((rx_data_val & emParityEightBitMask) >> 8); // Receive Parity
                }
            } else { // Clocked-synchronous mode
                ReceiveDataPacket.SetData(rx_data_val & emDataEightBitMask); // Receive 8 bit Data
            }
        }
    }
}//}}}
// vim600: set foldmethod=marker :
