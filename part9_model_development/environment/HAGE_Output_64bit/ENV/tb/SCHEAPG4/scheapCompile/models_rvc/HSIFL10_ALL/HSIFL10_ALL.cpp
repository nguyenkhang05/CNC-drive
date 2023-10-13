// ---------------------------------------------------------------------
// $Id: HSIFL10_ALL.cpp,v 1.11 2020/04/08 07:44:03 quynhnhunguyen Exp $
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


#include "HSIFL10_ALL.h"
#include "HSIFL10_ALL_Func.h"
#include "re_define.h"

///Constructor of HSIFL10_ALL class
HSIFL10_ALL::HSIFL10_ALL (sc_module_name name,
  unsigned int rLatency,
  unsigned int wLatency,
  std::string  version_name
):
    HSIFL10_ALL_AgentController(),
    BusSlaveBase<32,1>(),
    PCLK("PCLK"),
    hsif_refclk_in("hsif_refclk_in"),
    CCLK("CCLK"),
    hsif_tx_data("hsif_tx_data"),
    hsif_tx_control("hsif_tx_control"),
    hsif_rxd("hsif_rxd"),
    hsif_rxd_control("hsif_rxd_control"),
    hsif_rx_data("hsif_rx_data"),
    hsif_rx_control("hsif_rx_control"),
    hsif_txd("hsif_txd"),
    hsif_txd_control("hsif_txd_control"),
    hsif_tx_data_ready("hsif_tx_data_ready"),
    hsif_frame_cmp("hsif_frame_cmp"),
    int_hsif_txcmp("int_hsif_txcmp"),
    int_hsif_txerr("int_hsif_txerr"),
    int_hsif_rxcmp("int_hsif_rxcmp"),
    int_hsif_rxerr("int_hsif_rxerr"),
    int_hsif_rxiclc("int_hsif_rxiclc"),
    hsif_refclk_out("hsif_refclk_out"),
    reset_rhsif_n("reset_rhsif_n"),
    rvc_common_model()
{//{{{
  //Bind clock, reset signal and socket to Bus IF

    //Initialize variables
    mPCLKPeriod   = 0;
    mPCLKFreq     = 0;
    mPCLKOrgFreq  = 0;
    mPCLKFreqUnit = "Hz";
    
    mCCLKPeriod   = 0;
    mCCLKFreq     = 0;
    mCCLKOrgFreq  = 0;
    mCCLKFreqUnit = "Hz";
    
    mhsif_refclk_inPeriod       = 0;
    mhsif_refclk_inFreq         = 0;
    mhsif_refclk_inOrgFreq      = 0;
    mhsif_refclk_inFreqUnit     = "Hz";
    mhsif_version_name          = "FCC1";
    
    mTimeResolutionValue        = 1;
    mTimeResolutionUnit         = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    mIsLoopBackMode             = false;
    mIsTurnOffLBCommandICLC     = false;
    mIsFrameFromLinkPartner     = false;
    mResetCurVal                = true;
    mResetPreVal                = true;
    mIsResetHardActive          = false;
    mIsResetCmdReceive          = false;
    mIsResetCmdActive           = false;
    mResetCmdPeriod             = 0;
    mTimePCLKChangePoint        = 0;
    mDumpInterrupt              = false;
    mEnableRegisterMessage      = true;
    mIsReadRxFiFoEnable         = true;
    mReErrorCode                = 0x0;
    mTranErrorCode              = 0x0;

    mL2StartTransferTimePoint   = 0;
    mL2CmpTransferTimePoint     = 0;  
    
    int_hsif_txcmp.initialize(false);
    int_hsif_txerr.initialize(false);
    int_hsif_rxcmp.initialize(false);
    int_hsif_rxerr.initialize(false);
    int_hsif_rxiclc.initialize(false);
    hsif_tx_data_ready.initialize(true);
    hsif_frame_cmp.initialize(false);
    hsif_txd_control.initialize(0x0);
    hsif_rx_control.initialize(0x0);
    hsif_txd.initialize(L1Frame());
    hsif_rx_data.initialize(L2Frame());
    hsif_refclk_out.initialize(0x0);

    //Initial output port
    Initialize();
    
    //Initialize Bus
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&reset_rhsif_n);
    setTargetSocket32((char*)"tsp");
    tsp = BusSlaveBase<32,1>::tSocket32[0]; //Alias for binding

    //Instance HSIFL10_ALL_Func
    mHSIFL10_ALL_Func = new HSIFL10_ALL_Func((std::string)name, this);
    sc_assert(mHSIFL10_ALL_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mHSIFL10_ALL_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadLatency(rLatency);
    mBusSlaveIf32[0]->setWriteLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);
    
    mL1Frame = new L1Frame();
    sc_assert(mL1Frame != NULL);
    
    mL2Frame = new L2Frame();
    sc_assert(mL2Frame != NULL);
    
    mLogicChanTypeMap.insert(std::pair<eLogicChanType, uint8_t >(emICLC,  0x0));
    mLogicChanTypeMap.insert(std::pair<eLogicChanType, uint8_t >(emCTS,   0x3));
    mLogicChanTypeMap.insert(std::pair<eLogicChanType, uint8_t >(emChanA, 0x4));
    mLogicChanTypeMap.insert(std::pair<eLogicChanType, uint8_t >(emChanB, 0x5));
    mLogicChanTypeMap.insert(std::pair<eLogicChanType, uint8_t >(emChanC, 0x6));
    mLogicChanTypeMap.insert(std::pair<eLogicChanType, uint8_t >(emChanD, 0x7));
    
    mPayloadSizeIndexMap[0x0] = std::make_pair(8, 33);
    mPayloadSizeIndexMap[0x1] = std::make_pair(32, 57);
    mPayloadSizeIndexMap[0x2] = std::make_pair(64, 89);
    mPayloadSizeIndexMap[0x3] = std::make_pair(96, 121);
    mPayloadSizeIndexMap[0x6] = std::make_pair(160, 185);
    mPayloadSizeIndexMap[0x7] = std::make_pair(288, 313);
    
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x00, "PING"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x02, "Slave interface PLL start"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x04, "Slave interface PLL stop"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x08, "Slow Speed mode from Master to Slave"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x10, "Fast Speed mode from Master to Slave"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x20, "Slow Speed mode for transfer Slave to Master"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x80, "Fast Speed mode for transfer Slave to Master"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x31, "Enable Slave interface transmitter"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x32, "Disable Slave interface transmitter"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0x38, "Turns off the test mode"));
    mICLCCommandMap.insert(std::pair<uint8_t, std::string>(0xFF, "Turns on the payload loopback"));
    
    mBaudrateFastMap.insert(std::pair<uint8_t, uint16_t>(0, 80));
    mBaudrateFastMap.insert(std::pair<uint8_t, uint16_t>(1, 160));
    
    if ((version_name == "FCC2") || (version_name == "U2A")){
        mhsif_version_name = version_name;
        mBaudrateFastMap.insert(std::pair<uint8_t, uint16_t>(2, 320));
    }
    
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;
    
    SC_METHOD(HandleCCLKSignalMethod);
    dont_initialize();
    sensitive << CCLK;
    
    SC_METHOD(Handlehsif_refclk_inSignalMethod);
    dont_initialize();
    sensitive << hsif_refclk_in;

    SC_METHOD(HandlePresetzSignalMethod);
    sensitive << reset_rhsif_n;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;
  
    //For Transmitter
    SC_METHOD(WriteOutputTransMethod);
    dont_initialize();
    sensitive << mWriteOutputTransEvent;
    
    SC_METHOD(TransitionDataControlMethod);
    dont_initialize();
    sensitive << mTransitDataControlEvent;
    
    SC_METHOD(TransitionDataL2ControlMethod);
    dont_initialize();
    sensitive << mTransitDataL2ControlEvent;
    
    SC_METHOD(TransDataLoopBackTxRxControlMethod);
    dont_initialize();
    sensitive << mTransDataLoopBackTxRxEvent;
    
    SC_METHOD(UpdateTransitCompletedMethod);
    dont_initialize();
    sensitive << mTransitCompleteEvent;
    
    SC_METHOD(StoreFrameDataIntoRxFiFoMethod);
    dont_initialize();
    sensitive << hsif_rxd_control;
    
    //For Interrupt
    SC_METHOD(AssertTXCMP_IrqEventMethod);
    dont_initialize();
    sensitive << mAssertTXCMP_IrqEvent;
    
    SC_METHOD(DeassertTXCMP_IrqEventMethod);
    dont_initialize();
    sensitive << mDeassertTXCMP_IrqEvent;
    
    SC_METHOD(UpdateTXCMP_IrqEventMethod);
    dont_initialize();
    sensitive << mUpdateTXCMP_IrqEvent;
    
    SC_METHOD(AssertTXERR_IrqEventMethod);
    dont_initialize();
    sensitive << mAssertTXERR_IrqEvent;
    
    SC_METHOD(DeassertTXERR_IrqEventMethod);
    dont_initialize();
    sensitive << mDeassertTXERR_IrqEvent;
    
    SC_METHOD(UpdateTXERR_IrqEventMethod);
    dont_initialize();
    sensitive << mUpdateTXERR_IrqEvent;
    
    SC_METHOD(AssertRXCMP_IrqEventMethod);
    dont_initialize();
    sensitive << mAssertRXCMP_IrqEvent;
    
    SC_METHOD(DeassertRXCMP_IrqEventMethod);
    dont_initialize();
    sensitive << mDeassertRXCMP_IrqEvent;
    
    SC_METHOD(UpdateRXCMP_IrqEventMethod);
    dont_initialize();
    sensitive << mUpdateRXCMP_IrqEvent;
    
    SC_METHOD(AssertRXERR_IrqEventMethod);
    dont_initialize();
    sensitive << mAssertRXERR_IrqEvent;
    
    SC_METHOD(DeassertRXERR_IrqEventMethod);
    dont_initialize();
    sensitive << mDeassertRXERR_IrqEvent;
    
    SC_METHOD(UpdateRXERR_IrqEventMethod);
    dont_initialize();
    sensitive << mUpdateRXERR_IrqEvent;
    
    SC_METHOD(AssertRXICLC_IrqEventMethod);
    dont_initialize();
    sensitive << mAssertRXICLC_IrqEvent;
    
    SC_METHOD(DeassertRXICLC_IrqEventMethod);
    dont_initialize();
    sensitive << mDeassertRXICLC_IrqEvent;
    
    SC_METHOD(UpdateRXICLC_IrqEventMethod);
    dont_initialize();
    sensitive << mUpdateRXICLC_IrqEvent;
    
    SC_METHOD(TransProcessMethod);
    dont_initialize();
    sensitive << mTransProcessEvent;
    
    SC_METHOD(ReceiveProcessMethod);
    dont_initialize();
    sensitive << mRecProcessEvent;
    
    SC_METHOD(HandleL2FrmtoL1CtrlSignalMethod);
    dont_initialize();
    sensitive << hsif_tx_control;

    SC_METHOD(StartHandleL2FrmToL1Method);
    dont_initialize();
    sensitive << mStartHandleL2FrmToL1Event;

    SC_METHOD(FinishHandleL2FrmToL1Method);
    dont_initialize();
    sensitive << mFinishHandleL2FrmToL1Event;

    SC_METHOD(AssertHsif_frame_cmpMethod);
    dont_initialize();
    sensitive << mAssertHsif_frame_cmpEvent;

    SC_METHOD(DeAssertHsif_frame_cmpMethod);
    dont_initialize();
    sensitive << mDeAssertHsif_frame_cmpEvent;

    SC_METHOD(WriteHsif_frame_cmpMethod);
    dont_initialize();
    sensitive << mWriteHsif_frame_cmpEvent;

    SC_METHOD(WriteHsif_frame_readyMethod);
    dont_initialize();
    sensitive << mWriteHsif_frame_readyEvent;

    SC_METHOD(UpdateTxFifoFullStatusMethod);
    dont_initialize();
    sensitive << mUpdateTxFifoFullStatusEvent;
    
    SC_METHOD(UpdateTxFifoNotFullStatusMethod);
    dont_initialize();
    sensitive << mUpdateTxFifoNotFullStatusEvent;
    
    SC_METHOD(UpdateTxFifoEmptyStatusMethod);
    dont_initialize();
    sensitive << mUpdateTxFifoEmptyStatusEvent;
    
    SC_METHOD(UpdateTxFifoNotEmptyStatusMethod);
    dont_initialize();
    sensitive << mUpdateTxFifoNotEmptyStatusEvent;
    
    SC_METHOD(ReceiveProcessL2FrameMethod);
    dont_initialize();
    sensitive << mRecProcessL2FrameEvent;
    
    SC_METHOD(TransDataMethod);
    dont_initialize();
    sensitive << mTransDataEvent;

}//}}}

///Destructor of HSIFL10_ALL class
HSIFL10_ALL::~HSIFL10_ALL (void)
{//{{{
    delete mHSIFL10_ALL_Func;
    delete mL1Frame;
    delete mL2Frame;
}//}}}

void HSIFL10_ALL::Initialize()
{//{{{
    mTxCurrentFrameType         = emInValidFrame;
    mIsFreehsif_txd             = true; 
    mTransDataControl           = emTransIdle;
    mTransDataL2Control         = emTransIdle;
    mTransDataLBControl         = emTransIdle;
    mIsAutomaticFlowControl     = true;
    mIsFullRxFiFo               = false;
    mIsReqTransManual           = false;
    mIsPreReadRxFiFoEnable      = true;
    mRxFiFoSize                 = 0;
    mCurPLLMode                 = emPLLINVALID;
    mFCMType                    = emAutoFCType;
    mHsifRxControlStartValue    = INVALID_RX_CONTROL;
    mHsifRxControlEndValue      = INVALID_RX_CONTROL;
    mHsif_frame_cmp             = false;
    mHsif_tx_data_ready         = true;
    mPre_Hsif_tx_data_ready     = true;
    mIsTxFifoFull               = false;
    mIsTxFifoEmpty              = true;
    mIsSleepMode                = false;
    mHeaderL1                   = 0x0;
    mHeaderL2                   = 0x0;
    
    // Initialize internal variables
    txcmp_irq_val               = false;
    txerr_irq_val               = false;
    rxcmp_irq_val               = false;
    rxerr_irq_val               = false;
    rxiclc_irq_val              = false;

    //Reset queue
    while (!mReceiveFrameQueue.empty()){
        mReceiveFrameQueue.pop();
    }
    //Clear frame type buffer
    mFrameTypeBuffer.clear();
}//}}}

///Check clock period value
bool HSIFL10_ALL::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = 0;
    if (clock_name == "PCLK") {
        clock_period = mPCLKPeriod;
    }
    if(clock_period > 0) {
        clock_status = true;
    } else {
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }
    return clock_status;
}//}}}

/// Cancel operation events
void HSIFL10_ALL::CancelEvents (void)
{//{{{   
    mAssertTXCMP_IrqEvent.cancel();
    mDeassertTXCMP_IrqEvent.notify();
    mAssertTXERR_IrqEvent.cancel();
    mDeassertTXERR_IrqEvent.notify();
    mAssertRXCMP_IrqEvent.cancel();
    mDeassertRXCMP_IrqEvent.notify();
    mAssertRXERR_IrqEvent.cancel();
    mDeassertRXERR_IrqEvent.notify();
    mAssertRXICLC_IrqEvent.cancel();
    mDeassertRXICLC_IrqEvent.notify();
    mWriteOutputTransEvent.cancel();
    mTransProcessEvent.cancel();
    mRecProcessEvent.cancel();
    mTransitDataControlEvent.cancel();
    mTransitDataL2ControlEvent.cancel();
    mTransitCompleteEvent.cancel();
    mStartHandleL2FrmToL1Event.cancel();
    mFinishHandleL2FrmToL1Event.cancel();
    mAssertHsif_frame_cmpEvent.cancel();
    mDeAssertHsif_frame_cmpEvent.cancel();
    mWriteHsif_frame_cmpEvent.cancel();
    mWriteHsif_frame_readyEvent.cancel();
    mUpdateTxFifoFullStatusEvent.cancel();
    mUpdateTxFifoNotFullStatusEvent.cancel();
    mUpdateTxFifoEmptyStatusEvent.cancel();
    mUpdateTxFifoNotEmptyStatusEvent.cancel();
    mRecProcessL2FrameEvent.cancel();
    mTransDataEvent.cancel();
}//}}}

sc_time HSIFL10_ALL::CalculateClockEdge()
{//{{{
    return sc_time(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mTimePCLKChangePoint), mTimeResolutionUnit);
}//}}}

/// Wrapper for accessing registers
void HSIFL10_ALL::RegisterHandler (const std::vector<std::string> cmd)
{//{{{
    mHSIFL10_ALL_Func->RegisterHandler(cmd);
}//}}}

///Execute reset operation
void HSIFL10_ALL::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    //Initial registers
    mHSIFL10_ALL_Func->Reset(is_active);
    if (is_active) {
        //Cancel events
        this->CancelEvents();
        //Initialize output port
        this->Initialize();
        
        mUpdateTXCMP_IrqEvent.notify(SC_ZERO_TIME);
        mUpdateRXCMP_IrqEvent.notify(SC_ZERO_TIME);
        mUpdateTXERR_IrqEvent.notify(SC_ZERO_TIME);
        mUpdateRXERR_IrqEvent.notify(SC_ZERO_TIME);
        mUpdateRXICLC_IrqEvent.notify(SC_ZERO_TIME);
        mTransDataEvent.notify(SC_ZERO_TIME);
        mRecProcessL2FrameEvent.notify(SC_ZERO_TIME);
        mTransitDataControlEvent.notify(SC_ZERO_TIME);
        mTransitDataL2ControlEvent.notify(SC_ZERO_TIME);
        mWriteHsif_frame_readyEvent.notify(SC_ZERO_TIME);
        mWriteHsif_frame_cmpEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Process reset function when reset port is active
void HSIFL10_ALL::HandleResetHardMethod (void)
{//{{{
    if (mResetCurVal == vpcl::emResetActive) {
        mIsResetHardActive = true;
        re_printf("info","The reset port reset_rhsif_n is asserted.\n");
        this->EnableReset("reset_rhsif_n", mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        re_printf("info","The reset port reset_rhsif_n is de-asserted.\n");
        this->EnableReset("reset_rhsif_n", mIsResetHardActive);
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

///Process reset function when reset command is active
void HSIFL10_ALL::HandleResetCmdMethod (void)
{//{{{
    std::string reset_name = "reset_rhsif_n";
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

///Cancel reset function when reset command is active
void HSIFL10_ALL::CancelResetCmdMethod (void)
{//{{{
    std::string reset_name = "reset_rhsif_n";
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive);
}//}}}

///Handle reset_rhsif_n signal
void HSIFL10_ALL::HandlePresetzSignalMethod (void)
{//{{{
    mResetCurVal = reset_rhsif_n.read();
    if (mResetCurVal != mResetPreVal) {   //mResetPreVal is "true" at initial state
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}//}}}

///Handle PCLK signal
void HSIFL10_ALL::HandlePCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}//}}}

///Handle CCLK signal
void HSIFL10_ALL::HandleCCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CCLK.read();
    SetCLKFreq("CCLK", freq_value, "Hz");
}//}}}
///Handle hsif_refclk_in signal
void HSIFL10_ALL::Handlehsif_refclk_inSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = hsif_refclk_in.read();
    SetCLKFreq("hsif_refclk_in", freq_value, "Hz");
}//}}}

///Enable/disable dumping interrupt information
void HSIFL10_ALL::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HSIFL10_ALL_DumpInterrupt %s.\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt) {
            temp = "true";
        }
        re_printf("info","HSIFL10_ALL_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HSIFL10_ALL_DumpInterrupt %s.\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping message when users access registers
void HSIFL10_ALL::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mHSIFL10_ALL_Func->RegisterHandler(cmd);
}//}}}

///Set message level (fatal, error, warning, info)
void HSIFL10_ALL::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }

    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mHSIFL10_ALL_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mHSIFL10_ALL_Func->RegisterHandler(cmd);
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mHSIFL10_ALL_Func->RegisterHandler(cmd);

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mHSIFL10_ALL_Func->RegisterHandler(cmd);
    }
}//}}}


void HSIFL10_ALL::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: HSIFL10_ALL_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
            std::string intr_name = "false";
            if (mEnableRegisterMessage) {
                intr_name = "true";
            } 
            re_printf("info","HSIFL10_ALL_EnableRegisterMessage %s\n", intr_name.c_str());
    } else {
        re_printf("warning","Invalid argument: HSIFL10_ALL_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///Dump help message of all parameters or commands
void HSIFL10_ALL::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_MessageLevel (fatal|error|warning|info)         Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_DumpRegisterRW (true/false)                     Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_DumpInterrupt (true/false)                      Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_EnableRegisterMessage (true/false)              Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_AssertReset (reset_name, start_time, period)    Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_SetCLKFreq (clock_name, freq, unit)             Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_GetCLKFreq (clock_name)                         Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_ForceRegister (reg_name, reg_value)             Force HSIFL10_ALL's register with setting value.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_ReleaseRegister (reg_name)                      Release HSIFL10_ALL's register from force value.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_WriteRegister (reg_name, reg_value)             Write a value to a HSIFL10_ALL's register.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_ReadRegister (reg_name)                         Read a value from a HSIFL10_ALL's register.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_ListRegister ()                                 Dump name of model's registers.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_ReadRxFiFoEnable (true/false)                   Set enable read Rx FiFo (Default: true).");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_IssueError (mode, errorCode)                    Set error to Tx or Rx.");
            SC_REPORT_INFO(this->basename(),"HSIFL10_ALL_DumpStatusInfo ()                               Dump information of the commplete, error, Rx ICLC, last Tx/Rx status registers of the HSIFL10_ALL model");
        } else {
            re_printf("warning","The name (%s) of HSIFL10_ALL_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HSIFL10_ALL_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

void HSIFL10_ALL::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mHSIFL10_ALL_Func->RegisterHandler(cmd);
}//}}}

///Read value of register by software
void HSIFL10_ALL::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void HSIFL10_ALL::WriteRegister (const std::string reg_name, const uint32_t reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void HSIFL10_ALL::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Force value to register
void HSIFL10_ALL::ForceRegister (const std::string reg_name, const uint32_t reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            RegisterHandler(cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

///Get clock value
void HSIFL10_ALL::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            re_printf("info","PCLK frequency is %0.0f %s.\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } 
        else if (word_vector[0] == "CCLK") {
            re_printf("info","CCLK frequency is %0.0f %s.\n", (double)mCCLKOrgFreq, mCCLKFreqUnit.c_str());
        } 
        else if (word_vector[0] == "hsif_refclk_in") {
            re_printf("info","hsif_refclk_in frequency is %0.0f %s.\n", (double)mhsif_refclk_inOrgFreq, mhsif_refclk_inFreqUnit.c_str());
        }
        else {
            re_printf("warning","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void HSIFL10_ALL::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "PCLK") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
            }
            //Update time peripheral clock is changed its value 
            mTimePCLKChangePoint = sc_time_stamp().to_double();
        } 
        else if (word_vector[0] == "CCLK"){
            mCCLKOrgFreq = freq;
            ConvertClockFreq (mCCLKFreq, mCCLKFreqUnit, freq, unit);
            if (mCCLKFreq > 0) {
                mCCLKPeriod = (sc_dt::uint64)(((1/(double)mCCLKFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mCCLKPeriod = 0;
                // Cancel events
                CancelEvents();
            }
            
        }
        else if (word_vector[0] == "hsif_refclk_in"){
            mhsif_refclk_inOrgFreq = freq;
            ConvertClockFreq (mhsif_refclk_inFreq, mhsif_refclk_inFreqUnit, freq, unit);
            if (mhsif_refclk_inFreq > 0) {
                mhsif_refclk_inPeriod = (sc_dt::uint64)(((1/(double)mhsif_refclk_inFreq)*(double)mTimeResolutionValue) + 0.5);
            } else {
                mhsif_refclk_inPeriod = 0;
                // Cancel events
                CancelEvents();
            }
        }
        else {
            re_printf("warning","Clock name (%s) is invalid.\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid.\n", clock_name.c_str());
    }
}//}}}

///Assert reset by software
void HSIFL10_ALL::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "reset_rhsif_n") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (reset_rhsif_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of reset_rhsif_n is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be reset_rhsif_n.\n",reset_name.c_str());
    }
}//}}}
void HSIFL10_ALL::DumpInterruptMsg (const std::string intr_name, const bool value)
{//{{{
   if (mDumpInterrupt) {
        if (value) {
            re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
        } else {
            re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
        }
   }
}//}}}

bool HSIFL10_ALL::GetResetStatus()
{//{{{
   return (mIsResetCmdActive || mIsResetHardActive);
}//}}}


void HSIFL10_ALL::TransmitterCreateFrameL1(uint32_t ICLC_val, eModelType modelType, eLogicChanType logicChanType)
{//{{{
    mL1Frame->SetSynCode(0xA84B);
    uint8_t nHeaderL1 = 0xFF;
    uint32_t iclc_val = ICLC_val;
    //Create L1 frame without L2 frame
    if (emL1 == modelType) {
        //Create header for ICLC command
        if (mTxCurrentFrameType == emICLCFramePingAns || mTxCurrentFrameType == emICLCFrameL1){
            if (mTxCurrentFrameType == emICLCFramePingAns) {
                nHeaderL1 = 0x21;
            } else { // (mTxCurrentFrameType == emICLCFrameL1)
                nHeaderL1 = 0x01;
            }

            //Get value of RHSIFMDCR.CTSEN, RHSIFMDCR.CTSV
            uint8_t CTSEN_val = (this->GetRHSIFMDCR() >> 16) & 0x1;
            uint8_t CTSV_val = (this->GetRHSIFMDCR() >> 8) & 0x1;
            if (CTSEN_val == 0 && CTSV_val == 0) {
                //CTS of ICLC command is value of RHSIFMDCR.CTSV
                nHeaderL1 &= 0xFE;
            }
             
            mL1Frame->SetL1Payload(iclc_val);
            //Set header L1
            mL1Frame->SetL1header(nHeaderL1);
            //Set Endbit
            bool isSleepMode = this->GetRHSIFSMCR() & 0x1;
            mL1Frame->SetEndbit(isSleepMode);
            //Reset payload L2
            L2Frame l2Frame;
            mL1Frame->SetL2PayloadData(l2Frame);
            
            //Don't have error code
            if (mTranErrorCode == 0) {
                //Update last L1 frame
                this->UpdateRHSIFLTXFRMRL1(nHeaderL1, iclc_val, isSleepMode);
            }
        }
        else if (mTxCurrentFrameType == emCTSFrameL1) {
            //Create frame for CTS command
            //Automatic flow control is active
            if (mFCMType == emAutoFCType) {
                //In case: Rx FIFO is full 
                if (((this->GetRHSIFL1SR() >> 1) & 0x1) == 0x1) {
                    nHeaderL1 = 0x06;
                }
                //In case: Rx FIFO is not full
                else {
                    nHeaderL1 = 0x07;
                }
            }
            //Automatic flow control is disable
            else {
                //Flow control manual with CTS frame
                if (this->GetFlowControlManualType() == emFCMTypeWithCTSFrame) {
                    re_printf("info", "Flow control manual with CTS frame process.\n");
                    //Create header by getting RHSIFCCR.CTS
                    nHeaderL1 = 0x06 | ((this->GetRHSIFCCR() >> 8) & 0x01);
                    iclc_val = this->GetRHSIFCCR() & 0xFF;
                }
            } 
            
            //Set payload to an any value
            mL1Frame->SetL1Payload(iclc_val);
            //Set header L1
            mL1Frame->SetL1header(nHeaderL1);
            //Set end bit
            bool isSleepMode = this->GetRHSIFSMCR() & 0x1;
            mL1Frame->SetEndbit(isSleepMode);
            //Reset payload L2
            L2Frame l2Frame;
            mL1Frame->SetL2PayloadData(l2Frame);
            //Don't have error code
            if (mTranErrorCode == 0) {
                //Update last L1 frame
                this->UpdateRHSIFLTXFRMRL1(nHeaderL1, iclc_val, isSleepMode);
            } 
        } 
    }
    else if (emL2 == modelType) {
        //Read L2 frame from hsif_tx_data port
        L2Frame dataFrameL2 = hsif_tx_data.read();
        //Update Tx FIFO not empty
        mUpdateTxFifoNotEmptyStatusEvent.notify(SC_ZERO_TIME);
        //Read data from hsif_tx_control port
        uint32_t dataTxControlL2L1 = hsif_tx_control.read();
        //Get logic channel type (channel A, B, C, D), Payload size index -> Header L1
        uint8_t logicChanTypeL2 = (dataTxControlL2L1 >> 16) & 0xF;
        uint8_t payLoadSizeIndex = dataTxControlL2L1 & 0x7;

        //Header of L1 frame with payload is L2 frame
        nHeaderL1 = (uint8_t)(payLoadSizeIndex << 5) | (uint8_t)(logicChanTypeL2 << 1) | 1; 
        //Get value of RHSIFMDCR.CTSEN, RHSIFMDCR.CTSV
        uint8_t CTSEN_val = (this->GetRHSIFMDCR() >> 16) & 0x1;
        uint8_t CTSV_val = (this->GetRHSIFMDCR() >> 8) & 0x1;
        if (CTSEN_val == 0 && CTSV_val == 0) {
            //CTS of ICLC command is value of RHSIFMDCR.CTSV
            nHeaderL1 &= 0xFE;
        }
        //Set header L1
        mL1Frame->SetL1header(nHeaderL1);
        //Set payload to 0x0
        mL1Frame->SetL1Payload(0x0);
        //Set L2 payload data
        mL1Frame->SetL2PayloadData(dataFrameL2);
        //Set end bit
        bool isSleepMode = this->GetRHSIFSMCR() & 0x1;
        mL1Frame->SetEndbit(isSleepMode);

        uint16_t nHeaderL2 = dataFrameL2.GetL2Header();
        //Don't have error code
        if (mTranErrorCode == 0) {
            //Update last L1 frame
            this->UpdateRHSIFLTXFRMRL2(nHeaderL1, nHeaderL2);
        }
    }
}//}}}

void HSIFL10_ALL::TransProcessMethod()
{//{{{
    //Get Tx enable
    uint8_t TXEN_val = (this->GetRHSIFTXRXCR() >> 16) & 0x1;
    if (TXEN_val == 0) {
        re_printf("warning", "Tx of model is disable, so it can not any transmit data frame.\n");
        return;
    }
    //Receiver can receive frame data
    if (((this->GetRHSIFL1SR() >> 16) & 0x1) == 1) {
        if (CheckFreeTransmission() == true) {
            //Variable store model type, logic channel type, ICLC value
            eModelType modelType = emNoneModel;
            eLogicChanType logicChanType = emChanInvalid;
            uint32_t ICLC_val = INVALID_ICLC_COMMAND;
            
            if(!mFrameTypeBuffer.empty()) {
                //Get priority frame
                std::set<eFrameType>::iterator itFrameType = mFrameTypeBuffer.begin();
                //Update current frame type
                mTxCurrentFrameType = *itFrameType;
                //Get operation mode
                uint32_t RHSIFMDCR_val = this->GetRHSIFMDCR();
                    
                //Check frame type
                if (mTxCurrentFrameType == emICLCFrameL1) {
                    if ((mIsFullRxFiFo == false) || ((mIsFullRxFiFo == true) && (mFCMType == emFlowControlManualTypeNum))) {
                        if ((RHSIFMDCR_val & 0x1) == 0x1) {
                            re_printf("info", "Prepare for transmitting ICLC Frame.\n");
                            //Get value of RHSIFICCR register
                            ICLC_val = this->GetRHSIFICCR() & 0xFF;
                            logicChanType = emICLC;
                            modelType = emL1;
                            this->TransProcessICLC(ICLC_val, modelType, logicChanType);
                        } 
                    }
                    else {
                        re_printf("info", "Can not transmit frame data which is not CTS frame when Rx FiFo full in the automatic flow control.\n");
                    }
                }
                else if (mTxCurrentFrameType == emCTSFrameL1) {
                    //Shaking hand success
                    if (this->CheckShakingHand() == true) {
                        re_printf("info", "Prepare for transmitting CTS Frame.\n");
                        //Todo: process CTS frame
                        ICLC_val = PAYLOAD_CTS_FRAME;
                        logicChanType = emCTS;
                        modelType = emL1;
                        //Create frame for transmitter
                        this->TransmitterCreateFrameL1(ICLC_val, modelType, logicChanType);
                        //Transmitter transits frame
                        this->TransmitterTransProcess();
                    }
                    else {
                        re_printf("warning", "Shaking hand is not success, so has no whatever action.\n");
                        //Erase frame type when it is not transmitted
                        this->RemoveCurrentFrameType();
                    }
                }
                else if (mTxCurrentFrameType == emFrameL2) {
                    //Shaking hand success
                    if (this->CheckShakingHand() == true) {
                        re_printf("info", "Prepare for transmitting L2 Frame.\n");
                        modelType = emL2;
                        //Create frame for transmitter
                        this->TransmitterCreateFrameL1(ICLC_val, modelType, logicChanType);

                        sc_time transferTime = this->CalTimeToCmplReceiveL2Frm();
                        mAssertHsif_frame_cmpEvent.notify(this->CalculateClockEdge() + transferTime);
                        mL2StartTransferTimePoint = sc_time_stamp().to_double();
                        mL2CmpTransferTimePoint   = mL2StartTransferTimePoint + transferTime.to_double();

                        this->TransmitterTransProcess();
                    }
                    else {
                        re_printf("warning", "Shaking hand is not success, so has no whatever action.\n");
                        //Erase frame type when it is not transmitted
                        this->RemoveCurrentFrameType();
                    }
                }
                else if (mTxCurrentFrameType == emICLCFramePingAns) {
                    re_printf("info", "Prepare for transmitting Ping answer Frame.\n");
                    ICLC_val = 0xABCDEF01;
                    logicChanType = emICLC;
                    modelType = emL1;
                    this->TransProcessICLC(ICLC_val, modelType, logicChanType);
                } 
            }
            else {
                re_printf("info", "Frame type buffer is empty.\n");
                if (mPre_Hsif_tx_data_ready == true && mHsif_tx_data_ready ==  false) {
                    mHsif_tx_data_ready = true;
                    mWriteHsif_frame_readyEvent.notify(SC_ZERO_TIME);
                }
            }
        }
        else {
            re_printf("info", "Transmission is busy.\n");
        }
    }
    else {
        re_printf("info", "Stop transmision data frame because receiver can not receive data frame.\n");
    }   
}//}}}

void HSIFL10_ALL::TransmitterTransProcess()
{//{{{
    mWriteOutputTransEvent.notify();
     //Disable ready signal which is prepared to transfer L2 frame to L1
    if (mHsif_tx_data_ready ==  true) {
        mHsif_tx_data_ready = false;
        mWriteHsif_frame_readyEvent.notify(CalculateClockEdge());
    }
}//}}}


void HSIFL10_ALL::WriteOutputTransMethod()
{//{{{
    // In case, Rx-Tx loopback mode
    uint8_t RXLPBK_val = (this->GetRHSIFTMDCR() & 0x1);
    if (((mIsLoopBackMode == true) && (RXLPBK_val == 1)) || (mIsTurnOffLBCommandICLC == true)) {
        mTransitDataControlEvent.notify(SC_ZERO_TIME);
        return;
    }
    //Update error code to frame data which is transmitted
    mL1Frame->SetErrorCode(mTranErrorCode);
    //Reset Tx error code
    mTranErrorCode = 0;
    
    //Write data to transmission port
    mTransDataEvent.notify();
    //Notify transmission first byte
    mTransitDataControlEvent.notify();
    //Update status of mIsFreehsif_txd is not free
    mIsFreehsif_txd = false;
    double current_time = sc_time_stamp().to_double();
    //Compute time transition complete: t = length/baudrate
    uint64_t timeTrans = this->CalTimeToTransCmpL1Frm();
    double timeTransSync = timeTrans + CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time + timeTrans, mTimePCLKChangePoint);

    //Get TXLPBK value of RHSIFTMDCR register
    uint8_t TXLPBK_val = (this->GetRHSIFTMDCR() >> 1) & 0x1;
    if ((mIsLoopBackMode == true) && (TXLPBK_val == 0x1)) {
        mTransDataLoopBackTxRxEvent.notify(timeTransSync, mTimeResolutionUnit);
    }
    mTransitDataControlEvent.notify(timeTransSync, mTimeResolutionUnit);
}//}}}

void HSIFL10_ALL::ReceiveProcessMethod()
{//{{{  
    //uint8_t headerL1 = 0xFF;
    uint8_t logicChan_val = (uint8_t)emChanInvalid;
    uint32_t payloadL1_val = 0xFFFFFFFF;
    bool isICLCCommandValid = false;
    bool isCTS = 0x1;
    bool isEndBit = false;
      
    //In case Normal mode
    if (mIsSleepMode == false) {
        //Read Rx FIFO is enalbe
        if (this->GetReadRxFiFoEnable() == true) {
            re_printf("info","Read Rx FiFo is enable.\n");
            //Get frame data from Rx FIFO
            L1Frame frameData = GetFrameDataFromRxFiFo();

            if ((mReErrorCode !=0) && (mReErrorCode != frameData.GetErrorCode())) {
                //Set new Rx error code to received frame
                frameData.SetErrorCode(mReErrorCode);
            }
            //Reset Rx error code
            mReErrorCode = 0;

            //Check if Rx error code occure
            uint32_t errorCodeRx = frameData.GetErrorCode();
            if(errorCodeRx != 0) {
                this->UpdateRHSIFRXERRST(errorCodeRx);
                if (this->CheckRxInterruptEnable() == true) {
                    double current_time = sc_time_stamp().to_double();
                    mAssertRXERR_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                    re_printf("info","Rx error interrupt is enable.\n");
                }
                else {
                    double current_time = sc_time_stamp().to_double();
                    mDeassertRXERR_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                    re_printf("info","Rx error interrupt is not enable.\n");
                }
            }
            else { 
                //Get Endbit from received frame
                isEndBit = frameData.GetEndbit();
                if (isEndBit == true){
                    //Update status of receiver
                    mIsSleepMode = isEndBit;
                }
                
                bool isErrorFrame = this->CheckErrorFrameDataRx(frameData, mHeaderL1, logicChan_val, payloadL1_val, isICLCCommandValid, isCTS, isEndBit);
                if (isErrorFrame == true) {
                    return;
                }
                
                //Count size of FIFO, If size smaller than fixed size
                uint8_t valPayloadSizeIndex = frameData.GetL1header() >> 5;
                uint16_t valTotalFrameSize = 0x0;
                std::map<uint8_t, std::pair<uint16_t, uint16_t> >::iterator it;
                for (it = mPayloadSizeIndexMap.begin(); it != mPayloadSizeIndexMap.end(); it++) {
                    if (it->first == valPayloadSizeIndex ) {
                        valTotalFrameSize = (uint16_t)it->second.second;
                        break;
                    }
                }
                
                //Decrease size of Rx FIFO, this time Rx FIFO is not full
                mRxFiFoSize -= (valTotalFrameSize - SIZE_SYNCCODE_ENDBIT);
                //Update status mIsFullRxFiFo variable, RHSIFL1SR.RFFUL
                 //Check FIFO (queue) full or not
                if (mIsFullRxFiFo == true) {
                    mIsFullRxFiFo = false;
                    this->UpdateRHSIFL1SR(0x1, false);
                    if (mFCMType == emAutoFCType) {
                        //Check exist the previous CTS
                        bool isExist = this->CheckExistPreFrameTypeBuffer(emCTSFrameL1);
                        if (isExist == false) {
                            //Insert frame type to mFrameTypeBuffer
                            StoreFrameTypeIntoBuffer(emCTSFrameL1);
                            //Notify to TransProcessMethod
                            mTransProcessEvent.notify(SC_ZERO_TIME);
                        }
                        else {
                            re_printf("info","Exist CTS frame type previously.\n");
                        }
                    } 
                }
                
                //In case, frame data is ICLC command
                if (logicChan_val == 0x0) {
                    if (frameData.GetErrorCode() == 0) {
                        this->ReceiveProcessICLC(payloadL1_val, isICLCCommandValid, isCTS);
                        if (payloadL1_val == 0xABCDEF01) {
                            //Store 0x0 to Payload of RHSIFLRXFRMRL1 register
                            payloadL1_val = 0x0;
                        }
                        this->UpdateRHSIFLRXFRMRL1(mHeaderL1, payloadL1_val, isEndBit);
                    }  
                    //Callback this method
                    mRecProcessEvent.notify(SC_ZERO_TIME);
                } 
                //Change State and Flow control if needed 
                else if (logicChan_val == 0x3) {
                    if (this->CheckShakingHand() == true) {
                        //Update RHSIFL1SR.RCTS bit
                        this->UpdateRHSIFL1SR(16, isCTS);
                        if (isCTS ==  false) {
                            mPre_Hsif_tx_data_ready = false;
                            if (mHsif_tx_data_ready ==  true) {
                                mHsif_tx_data_ready = false;
                                mWriteHsif_frame_readyEvent.notify(CalculateClockEdge());
                            }
                        }
                        else {
                            mPre_Hsif_tx_data_ready = true;
                            //Notify to TransProcessMethod
                            mTransProcessEvent.notify(SC_ZERO_TIME);
                        }

                        //Update Rx Complete Status Register: CTS
                        if (frameData.GetErrorCode() == 0) {
                            this->UpdateRHSIFRXCMPST(emCTSFrameL1);
                            this->UpdateRHSIFLRXFRMRL1(mHeaderL1, payloadL1_val, isEndBit);
                        }

                        //Callback this method
                        mRecProcessEvent.notify(SC_ZERO_TIME);
                    }
                    else {
                        re_printf("warning","Shaking hand is not success, so has no whatever action.\n");
                    }
                }
                //Remove header L1 in current frame -> L2 frame -> transmit to L2 module
                else if (logicChan_val == 0x4
                        || logicChan_val == 0x5
                        || logicChan_val == 0x6
                        || logicChan_val == 0x7) {
                     //Shaking hand success
                    if (this->CheckShakingHand() == true) {
                        //Update RHSIFL1SR.RCTS bit
                        this->UpdateRHSIFL1SR(16, isCTS);
                        if (isCTS == false) {
                            mPre_Hsif_tx_data_ready = false;
                            if (mHsif_tx_data_ready == true) {
                              mHsif_tx_data_ready = false;
                              mWriteHsif_frame_readyEvent.notify(CalculateClockEdge());
                            } 
                        }
                        else {
                            mPre_Hsif_tx_data_ready = true;
                            //Notify to TransProcessMethod
                            mTransProcessEvent.notify(SC_ZERO_TIME);
                        }
                        //Deframe L1
                        *mL2Frame = frameData.GetL2PayLoadData();
                        //Calculate data control for L2
                        mHsifRxControlStartValue = ((uint32_t)logicChan_val << 16) | (0x1 << 9) | valPayloadSizeIndex;
                        mHsifRxControlEndValue = ((uint32_t)logicChan_val << 16) | (0x2 << 9) | valPayloadSizeIndex;

                        mHeaderL2 = mL2Frame->GetL2Header();
                        //Update Rx Complete Status Register: L2 frame
                        if (frameData.GetErrorCode() == 0) {
                            this->UpdateRHSIFRXCMPST(emFrameL2);
                            this->UpdateRHSIFLRXFRMRL2(mHeaderL1, mHeaderL2);
                        }
                        //Change to Receive process for L2 frame
                        mRecProcessL2FrameEvent.notify(SC_ZERO_TIME);
                      
                    }
                    else {
                        re_printf("warning","Shaking hand is not success, so has no whatever action.\n");
                    }
                } 
            }
        }
        else {
            re_printf("info","Read Rx FiFo is not enable.\n");
        }
    }
    else {
          re_printf("info","Receiver in sleep mode, so has no whatever action.\n");
    }
    
}//}}}

void HSIFL10_ALL::UpdateRHSIFRXCMPST(eFrameType frameType)
{//{{{
    //Update RHSIFRXCMPST register
    mHSIFL10_ALL_Func->UpdateRHSIFRXCMPST(frameType);
    //Issue interrupt if any
    if (frameType == emICLCFrameL1
        || frameType == emICLCFramePingAns) {
        uint32_t RCEIC_val = mHSIFL10_ALL_Func->GetRHSIFRXCMPIE() & 0x1;
        if (RCEIC_val == 1) {
            //Issue interrupt
            double current_time = sc_time_stamp().to_double();
            mAssertRXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Rx complete ICLC command interrupt enable.\n");
        }
        else {
            double current_time = sc_time_stamp().to_double();
            mDeassertRXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Rx complete ICLC command interrupt is not enable.\n");
        }
    }
    else if (frameType == emCTSFrameL1) {
        uint32_t RCECT_val = (mHSIFL10_ALL_Func->GetRHSIFRXCMPIE() >> 8) & 0x1;
        if (RCECT_val == 1) {
            //Issue interrupt
            double current_time = sc_time_stamp().to_double();
            mAssertRXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Rx complete CTS frame interrupt enable.\n");
        }
        else {
            double current_time = sc_time_stamp().to_double();
            mDeassertRXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Rx complete CTS frame interrupt is not enable.\n");
        }
    }
    else if (frameType == emFrameL2) {
            uint32_t RCEL2_val = (mHSIFL10_ALL_Func->GetRHSIFRXCMPIE() >> 16) & 0x1;
        if (RCEL2_val == 1) {
            //Issue interrupt
            double current_time = sc_time_stamp().to_double();
            mAssertRXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Rx complete L2 frame interrupt enable.\n");
        }
        else {
            double current_time = sc_time_stamp().to_double();
            mDeassertRXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Rx complete L2 frame interrupt is not enable.\n");
        }
    } 
}//}}}

void HSIFL10_ALL::UpdateRHSIFTXCMPST(eFrameType frameType)
{//{{{
    //Update RHSIFTXCMPST register
    mHSIFL10_ALL_Func->UpdateRHSIFTXCMPST(frameType);
    //Issue interrupt if any
    if (frameType == emICLCFrameL1
        || frameType == emICLCFramePingAns) {
        //Get value of bit TCEIC in RHSIFTXCMPIE register
        uint32_t val_TCEIC = mHSIFL10_ALL_Func->GetRHSIFTXCMPIE() & 0x1;
        if (val_TCEIC == 1) {
            //Issue interrupt
            double current_time = sc_time_stamp().to_double();
            mAssertTXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Tx complete ICLC command interrupt enable.\n");
        }
        else {
            double current_time = sc_time_stamp().to_double();
            mDeassertTXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Tx complete ICLC command interrupt is not enable.\n");
        }
    }
    else if (frameType == emCTSFrameL1) {
        //Get value of bit TCECT in RHSIFTXCMPIE register
        uint32_t val_TCECT = (mHSIFL10_ALL_Func->GetRHSIFTXCMPIE() >> 8) & 0x1;
        //Issue interrupt
        if (val_TCECT == 1) {
            //Issue interrupt
            double current_time = sc_time_stamp().to_double();
            mAssertTXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Tx complete CTS frame interrupt enable.\n");
        }
        else {
            double current_time = sc_time_stamp().to_double();
            mDeassertTXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Tx complete CTS frame interrupt is not enable.\n");
        }
    }
    else if (frameType == emFrameL2) {
        //Get value of bit TCEL2 in RHSIFTXCMPIE register
        uint32_t val_TCEL2 = (mHSIFL10_ALL_Func->GetRHSIFTXCMPIE() >> 16) & 0x1;
        //Issue interrupt
        if (val_TCEL2 == 1) {
            //Issue interrupt
            double current_time = sc_time_stamp().to_double();
            mAssertTXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Tx complete L2 frame interrupt enable.\n");
        }
        else {
            double current_time = sc_time_stamp().to_double();
            mDeassertTXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Tx complete L2 frame interrupt is not enable.\n");
        }
    } 
}//}}}

void HSIFL10_ALL::AssertTXCMP_IrqEventMethod()
{//{{{
    txcmp_irq_val = true;
    mUpdateTXCMP_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::DeassertTXCMP_IrqEventMethod()
{//{{{
    txcmp_irq_val = false;
    mUpdateTXCMP_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::UpdateTXCMP_IrqEventMethod()
{//{{{
    int_hsif_txcmp.write(txcmp_irq_val);
    DumpInterruptMsg(int_hsif_txcmp.basename(), txcmp_irq_val);
}//}}}

void HSIFL10_ALL::AssertTXERR_IrqEventMethod()
{//{{{
    txerr_irq_val = true;
    mUpdateTXERR_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::DeassertTXERR_IrqEventMethod()
{//{{{
    txerr_irq_val = false;
    mUpdateTXERR_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::UpdateTXERR_IrqEventMethod()
{//{{{
    int_hsif_txerr.write(txerr_irq_val);
    DumpInterruptMsg(int_hsif_txerr.basename(), txerr_irq_val);
}//}}}

void HSIFL10_ALL::AssertRXCMP_IrqEventMethod()
{//{{{
    rxcmp_irq_val = true;
    mUpdateRXCMP_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::DeassertRXCMP_IrqEventMethod()
{//{{{
    rxcmp_irq_val = false;
    mUpdateRXCMP_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::UpdateRXCMP_IrqEventMethod()
{//{{{
    int_hsif_rxcmp.write(rxcmp_irq_val);
    DumpInterruptMsg(int_hsif_rxcmp.basename(), rxcmp_irq_val);
}//}}}

void HSIFL10_ALL::AssertRXERR_IrqEventMethod()
{//{{{
    rxerr_irq_val = true;
    mUpdateRXERR_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::DeassertRXERR_IrqEventMethod()
{//{{{
    rxerr_irq_val = false;
    mUpdateRXERR_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::UpdateRXERR_IrqEventMethod()
{//{{{
    int_hsif_rxerr.write(rxerr_irq_val);
    DumpInterruptMsg(int_hsif_rxerr.basename(), rxerr_irq_val);
}//}}}

void HSIFL10_ALL::AssertRXICLC_IrqEventMethod()
{//{{{
    rxiclc_irq_val = true;
    mUpdateRXICLC_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::DeassertRXICLC_IrqEventMethod()
{//{{{
    rxiclc_irq_val = false;
    mUpdateRXICLC_IrqEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::UpdateRXICLC_IrqEventMethod()
{//{{{
    int_hsif_rxiclc.write(rxiclc_irq_val);
    DumpInterruptMsg(int_hsif_rxiclc.basename(), rxiclc_irq_val);
}//}}}

void HSIFL10_ALL::UpdateRHSIFICCR()
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFICCR();
}//}}}

void HSIFL10_ALL::TransitionDataControlMethod()
{//{{{
    if (mIsResetHardActive == true || mIsResetCmdReceive == true) {
        re_printf("info", "[%s] Reset (%s) port during reset period.\n", sc_time_stamp().to_string().c_str(), hsif_txd_control.name());
        hsif_txd_control.write((uint32_t)emTransIdle);
        return;
    }
    
    // In case, Rx-Tx loopback mode
    uint8_t RXLPBK_val = (this->GetRHSIFTMDCR() & 0x1);
    if (((mIsLoopBackMode == true) && (RXLPBK_val == 1)) || (mIsTurnOffLBCommandICLC == true)) {
        mTransDataEvent.notify();
        hsif_txd_control.write(hsif_rxd_control.read());
        return;
    }
    
    if (mTransDataControl == emTransIdle) {
        //Start transition
        hsif_txd_control.write((uint32_t)emTransStart);
        mTransDataControl = emTransStart;
        re_printf("info","Start transmission data from L1 to L1.\n");
    }
    else { // (mTransDataControl == emTransStart)
        //End transition
        hsif_txd_control.write((uint32_t)emTransEnd);
        mTransDataControl = emTransIdle;
        re_printf("info","End transmission data from L1 to L1.\n");
        mTransitCompleteEvent.notify(SC_ZERO_TIME);
    } 
}//}}}

void HSIFL10_ALL::TransitionDataL2ControlMethod()
{//{{{
    if (mIsResetHardActive == true || mIsResetCmdReceive == true){
        re_printf("info", "[%s] Reset (%s) port during reset period.\n", sc_time_stamp().to_string().c_str(), hsif_rx_control.name());
        hsif_rx_control.write(0x00);
        return;
    }
    
    if (mTransDataL2Control == emTransIdle) {
        //Start transition  
        hsif_rx_control.write(mHsifRxControlStartValue);
        mTransDataL2Control = emTransStart;
        re_printf("info","Start transmission data from L1 to L2.\n");
        double current_time = sc_time_stamp().to_double();
        mTransitDataL2ControlEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint) + mPCLKPeriod, mTimeResolutionUnit);
    }
    else {// (mTransDataL2Control == emTransStart)
        //End transition
        hsif_rx_control.write(mHsifRxControlEndValue);
        mTransDataL2Control = emTransIdle;
        re_printf("info","End transmission data from L1 to L2.\n");
        //Callback this method
        mRecProcessEvent.notify(SC_ZERO_TIME); 
    } 
}//}}}

void HSIFL10_ALL::TransDataLoopBackTxRxControlMethod()
{//{{{
    re_printf("info","End transmission data from Tx to Rx in loopback mode.\n");
    mIsFrameFromLinkPartner = false;
    this->StoreFrameDataIntoRxFiFoMethod();
}//}}}

void HSIFL10_ALL::UpdateTransitCompletedMethod()
{//{{{
    re_printf("info","In progress, updating transmission completed.\n");
    //Update Status of hsif_txd port is free
    mIsFreehsif_txd = true;
    re_printf("info","Transmission is not busy.\n");
    
    //Check error transmission
    uint32_t errorCodeTx = mL1Frame->GetErrorCode();
    if (errorCodeTx != 0) {
           this->UpdateRHSIFTXERRST(errorCodeTx);
           double current_time = sc_time_stamp().to_double();
        //Check Tx error interrupt
        if (this->CheckTxInterruptEnable() == true) {
            mAssertTXERR_IrqEvent.notify();
            re_printf("info","Tx error interrupt is enable.\n");
        }
        else {
            mDeassertTXERR_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
            re_printf("info","Tx error interrupt is not enable.\n");
        }
    }
    else {
        //Update RHSIFTXCMPST register
        this->UpdateRHSIFTXCMPST(mTxCurrentFrameType);
        if (mTxCurrentFrameType == emICLCFramePingAns
            || mTxCurrentFrameType == emICLCFrameL1) {
            //Update RHSIFICCR register
            this->UpdateRHSIFICCR();
        }
        else if (mTxCurrentFrameType == emCTSFrameL1) {
            if (mFCMType == emFCMTypeWithCTSFrame){
                re_printf("info","Flow control manual with CTS frame process.\n");
                //In case, flow control manual with CTS frame then update RHSIFCCR register
                this->UpdateRHSIFCCR(16, false);
            }
        }
    }
    
    //Erase frame completed transmit
    if(!mFrameTypeBuffer.empty()) {
        eFrameType frameTypeTemp = mTxCurrentFrameType;
        mFrameTypeBuffer.erase(mTxCurrentFrameType);
        if (!mFrameTypeBuffer.empty()) {
            //Notify to transmit remain frame in buffer
            mTransProcessEvent.notify(SC_ZERO_TIME);
        }
        else {
            if (frameTypeTemp != emCTSFrameL1) {
                re_printf("info","Frame type buffer is empty.\n");
                //Notify ready to L2
                if (mPre_Hsif_tx_data_ready == true && mHsif_tx_data_ready ==  false) {
                    mHsif_tx_data_ready = true;
                    mWriteHsif_frame_readyEvent.notify(SC_ZERO_TIME);
                }
            }
            else { // (frameTypeTemp == emCTSFrameL1) 
                if (mIsPreReadRxFiFoEnable == false && mIsReadRxFiFoEnable == true) {
                    //Store CTS = 1 frame and transmit to receiver
                    StoreFrameTypeIntoBuffer(emCTSFrameL1);
                    re_printf("info","Frame type buffer is not empty.\n");
                    //Update status previous read Rx FIFO enable
                    mIsPreReadRxFiFoEnable = mIsReadRxFiFoEnable;
                    //Notify to transmit remain frame in buffer
                    mTransProcessEvent.notify(SC_ZERO_TIME);
                }
                else {
                    if (mPre_Hsif_tx_data_ready == true) {
                        if (mHsif_tx_data_ready == false) {
                            mHsif_tx_data_ready = true;
                            mWriteHsif_frame_readyEvent.notify(SC_ZERO_TIME);
                        } 
                    } 
                }
            } 
        }
    } 
    
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFSPCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFSPCR();
}//}}}

uint64_t HSIFL10_ALL::CalTimeToTransCmpL1Frm()
{//{{{
    uint8_t valPayloadSizeIndex = 0xFF;
    valPayloadSizeIndex = (uint8_t)mL1Frame->GetL1header() >> 5;
    uint64_t valTotalFrameSize = 0x0;
    std::map<uint8_t, std::pair<uint16_t, uint16_t> >::iterator it;
    
    for (it = mPayloadSizeIndexMap.begin(); it != mPayloadSizeIndexMap.end(); it++) {
        if (it->first == valPayloadSizeIndex ) {
            valTotalFrameSize = (uint64_t)it->second.second;
            break;
        }
    }
    uint64_t baudrate = 0;
    
    if (valTotalFrameSize >= FRAME_SIZE_MIN) {
        baudrate = this->CalBaudrate();
        if(baudrate != 0){
            return (uint64_t)((valTotalFrameSize * mTimeResolutionValue) / baudrate);
        }
        else {
            return 0;
        }
    }
    else{
        return 0;
    } 
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFTXRXCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFTXRXCR();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFMDCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFMDCR();
}//}}}

void HSIFL10_ALL::StoreFrameTypeIntoBuffer(eFrameType frameType)
{//{{{
    if (mIsSleepMode == false) {
        //Insert frame type to buffer
        mFrameTypeBuffer.insert(frameType);
    }
    else {
        re_printf("info","Transmitter is in sleep mode, so can not transmit frame data.\n");
    }
}//}}}

bool HSIFL10_ALL::CheckFreeTransmission()
{//{{{
    return mIsFreehsif_txd;
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFICCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFICCR();
}//}}}

void HSIFL10_ALL::StoreFrameDataIntoRxFiFoMethod()
{//{{{
    if (   this->CheckSetClockCorrect("CCLK") == true
        || this->CheckSetClockCorrect("hsif_refclk_in") == true
        ) {
        //Receive completely frame data
        if (hsif_rxd_control.read() == 0x2 || mIsLoopBackMode == true) {
            if (hsif_rxd_control.read() == 0x1 && this->CheckShakingHand() == true) {
                mIsFrameFromLinkPartner = true;
            }
            //Check able to receive of receiver
            uint8_t rx_val = this->GetRHSIFTXRXCR() & 0x1;
            //Rx is enable
            if (rx_val == 0x1 || mIsLoopBackMode == true) {
                L1Frame framedata;
                uint8_t TXLPBK_val = (this->GetRHSIFTMDCR() & 0x2) >> 1;
                uint8_t RXLPBK_val = (this->GetRHSIFTMDCR() & 0x1);
        
                if ((mIsLoopBackMode == true) && (TXLPBK_val == 1)) {
                    if (mIsFrameFromLinkPartner == false) {
                        //In case, Tx-Rx loopback mode
                        framedata = *mL1Frame;
                    }
                    else {
                        re_printf("warning","Transmitter can not handle data frame from link partner when it is Tx-Rx loopback mode.\n");
                        return;
                    }
                }
                //In case, Rx-Tx loopback mode
                else if ((mIsLoopBackMode == true) && (RXLPBK_val == 1)) {
                    if (hsif_rxd_control.read() == 0x2){
                      framedata = hsif_rxd.read();
                    }
                      
                    //Write data to transmission port
                    mWriteOutputTransEvent.notify(SC_ZERO_TIME);
                    if (hsif_rxd_control.read() == 0x1) {
                        re_printf("info","Start transmission data from L1 to L1 in Rx-Tx loopback.\n");
                    }
                    else if (hsif_rxd_control.read() == 0x2){
                        re_printf("info","End transmission data from L1 to L1 in Rx-Tx loopback.\n");
                    } 
                    
                    if (mHsif_tx_data_ready == true) {
                        mHsif_tx_data_ready = false;
                        mWriteHsif_frame_readyEvent.notify(CalculateClockEdge());
                    }
                }
                else {
                    framedata = hsif_rxd.read();
                }
                
                if (hsif_rxd_control.read() == 0x2) {
                    if (mIsSleepMode == true) {
                        //Get Endbit from received frame
                        bool isEndBit = framedata.GetEndbit();
                        //Update status of receiver
                        mIsSleepMode = isEndBit;
                    }
                    
                    if (mIsSleepMode == false) {
                        //Upudate RHSIFL1SR.RFEMP to 0 (not empty)
                        if ((this->GetRHSIFL1SR() & 0x1) == 0x1) {
                            this->UpdateRHSIFL1SR(0x0, false);
                        }
                        //Get payload size index
                        uint8_t valPayloadSizeIndex = framedata.GetL1header() >> 5;
                        uint16_t valTotalFrameSize = 0x0;
                        std::map<uint8_t, std::pair<uint16_t, uint16_t> >::iterator it;
                        for (it = mPayloadSizeIndexMap.begin(); it != mPayloadSizeIndexMap.end(); it++) {
                            if (it->first == valPayloadSizeIndex ) {
                                valTotalFrameSize = (uint16_t)it->second.second;
                                break;
                            }
                        }
                        //Increase size of Rx FIFO
                        uint16_t rxFiFoSizeTemp = mRxFiFoSize;
                        mRxFiFoSize += valTotalFrameSize - SIZE_SYNCCODE_ENDBIT;
                        //Rx FIFO is full
                        if (mRxFiFoSize >= RX_FIFO_SIZE_BIT_MAX) {
                            re_printf("info","Rx FiFo is full.\n");
                            //Update status of Rx FIFO's size
                            mIsFullRxFiFo = true;
                            //Store frame data to Rx FIFO
                            if (mFCMType == emAutoFCType) {
                                //Check exist the previous CTS
                                bool isExist = this->CheckExistPreFrameTypeBuffer(emCTSFrameL1);
                                if (isExist == false) {
                                    this->StoreFrameTypeIntoBuffer(emCTSFrameL1);
                                } 
                            }
                            
                            //Check Rx FiFo full or not
                            uint8_t RFFUL_val = (this->GetRHSIFL1SR() >> 1) &0x1;
                            if (RFFUL_val == 0x0) {
                                //Update RHSIFL1SR.RFFUL to true
                                this->UpdateRHSIFL1SR(0x1, true);
                                if (mFCMType == emAutoFCType) {
                                    //Notify to TransProcessMethod
                                    mTransProcessEvent.notify();
                                }
                            }
                            //Update Rx FIFO size when don't store data frame to Rx FIFO
                            if (mRxFiFoSize > RX_FIFO_SIZE_BIT_MAX) {
                                mRxFiFoSize = rxFiFoSizeTemp;
                            }
                            else {
                                //Insert frame data to queue
                                mReceiveFrameQueue.push(framedata);
                                re_printf("info","Receive data successfully.\n");
                            }
                        }
                        else {
                            re_printf("info","Rx FiFo is not full.\n");
                            //Insert frame data to queue
                            mReceiveFrameQueue.push(framedata);
                            re_printf("info","Receive data successfully.\n");
                        }
                        
                        //Change to receive process
                        mRecProcessEvent.notify();
                    }
                    else {
                        re_printf("info","Receiver in sleep mode, so has no whatever action.\n");
                    }
                }
            }
            else {
                re_printf("info","Receiver is not enbale.\n");
            }
        }
    }
    else {
        re_printf("warning", "Communication Clock (CCLK) is zero when operation as master or Reference Clock In is zero when operation as slave.\n");
    }
}//}}}

L1Frame HSIFL10_ALL::GetFrameDataFromRxFiFo()
{//{{{
    L1Frame frameData;
    if(!mReceiveFrameQueue.empty()) {
        frameData = mReceiveFrameQueue.front();
        mReceiveFrameQueue.pop();
        return frameData;
    }
    else {
        re_printf("info","Rx FiFo is empty.\n");
        // Update RHSIFL1SR.RFEMP to 1
        this->UpdateRHSIFL1SR(0x0, true);
        return frameData;
    }
}//}}}

bool HSIFL10_ALL::CheckErrorFrameDataRx(L1Frame& frameData, uint8_t& headerL1, uint8_t& logicChan, uint32_t& payloadL1, 
                                        bool& isICLCCommandValid, bool& isCTS, bool& isEndBit)
{//{{{
    //Variable is used to return
    bool retErr = false;
    
    //Get information of frame data
    uint16_t syncCode_val = frameData.GetSynCode();
    uint8_t headerL1_val = frameData.GetL1header();
    bool endbit_val = frameData.GetEndbit();
    
    uint8_t logicChanType_val     = ((headerL1_val & 0x0F) >> 1) ;
    bool CTS_val = (bool)(headerL1_val & 0x01);
    
    uint32_t payloadL1_val = frameData.GetL1Payload();
    
    if (syncCode_val != 0xA84B) {
        retErr = true;
        return retErr;
    }
    
    bool isLogicChanTypeValid = false; // Check Logic channel type is valid or not
    std::map<eLogicChanType, uint8_t >::iterator it;
    for (it = mLogicChanTypeMap.begin(); it != mLogicChanTypeMap.end(); it++) {
        if (it->second == logicChanType_val ) {
            isLogicChanTypeValid = true;
            break;
        }
    }
    if (isLogicChanTypeValid == true) {
        //Get logic channel type
        logicChan = logicChanType_val;
    } 
    
    bool isICLCValid = false; // Check ICLC command is valid or not
    std::map<uint8_t, std::string>::iterator iter;
    for (iter = mICLCCommandMap.begin(); iter != mICLCCommandMap.end(); iter++) {
        if (iter->first == payloadL1_val) {
            isICLCValid = true;
            break;
        }
    }
    //Get header L1
    headerL1 = headerL1_val;
    //Update check ICLC command valid or not
    isICLCCommandValid = isICLCValid;
    //Get payload L1
    payloadL1 = payloadL1_val;
    //Get CTS value
    isCTS = CTS_val;
    //Get end bit
    isEndBit = endbit_val;
    
    return retErr;
}//}}}

void HSIFL10_ALL::UpdateRHSIFRXICST(uint32_t RHSIFRXICST_val)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFRXICST(RHSIFRXICST_val);
}//}}}

void HSIFL10_ALL::ICLCCommandProcess(uint32_t payloadL1)
{//{{{
    switch (payloadL1) {
        case 0x02:
            //Slave interface PLL start (in preparation for high speed mode)
            mCurPLLMode = emPrePLLON;      
            break;
        case 0x04:
            //Slave interface PLL stop (after fallback from high speed mode)
            mCurPLLMode = emPrePLLOFF;      
            break;
        case 0x08:
            //Selects the Slow Speed mode for transfers from the Master interface to the Slave interface
            if (mCurPLLMode == emPrePLLOFF) {
                mCurPLLMode = emPLLOFF;
            } 
            break;
        case 0x10:
            //Selects the Fast Speed mode for transfers from the Master interface to the Slave interface
            if (mCurPLLMode == emPrePLLON) {
                mCurPLLMode = emPLLON;

            }
            break;
        case 0x20:
            //Selects the Slow Speed mode for transfers from the Slave interface to the Master interface
            if (mCurPLLMode == emPrePLLOFF) {
                mCurPLLMode = emPLLOFF;
            } 
            break;
        case 0x80:
            //Selects the Fast Speed mode for transfers from the Slave interface to the Master interface
            if (mCurPLLMode == emPrePLLON) {
                mCurPLLMode = emPLLON;
            } 
            break;
      case 0x31:
            //Enables the Slave interface transmitter
            this->UpdateRHSIFTXRXCR(16, true);       
            break;
      case 0x32:
            //Disables the Slave interface transmitter
            this->UpdateRHSIFTXRXCR(16, false);       
            break;
      case 0x38:
            this->UpdateRHSIFTMDCR(0, false, true);
            break;
      case 0xFF:
            this->UpdateRHSIFTMDCR(0, true, false);
            break;
      default:
            break;
    }
}//}}}

void HSIFL10_ALL::UpdateRHSIFTXRXCR(uint8_t posBit, bool isEnable)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFTXRXCR(posBit, isEnable);
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFRXICST()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFRXICST();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFPCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFPCR();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFL1SR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFL1SR();
}//}}}

void HSIFL10_ALL::UpdateRHSIFL1SR(uint8_t posBit, bool isEnable)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFL1SR(posBit, isEnable);
}//}}}

void HSIFL10_ALL::TransProcessICLC(uint32_t ICLC_val, eModelType modelType, eLogicChanType logicChanType)
{//{{{
    //Check operation mode
    uint32_t val_mode = this->GetRHSIFMDCR() & 0x1;
    //Operation mode is master
    if (val_mode == 1) {
        //Check shaking hand by judging RHSIFRXICST.RIPA value
        uint8_t RIPA_val = (this->GetRHSIFRXICST() & 0x00010000) >> 16;
        //Shaking hand success
        if (RIPA_val == 0x1 && ICLC_val != 0x00) {
            //Create frame for transmitter
            this->TransmitterCreateFrameL1(ICLC_val, modelType, logicChanType);
            //Transmitter transits frame
            this->TransmitterTransProcess();
        }
        //Ping request
        else if (ICLC_val == 0x00) {
            //Create frame for transmitter
            this->TransmitterCreateFrameL1(ICLC_val, modelType, logicChanType);
            //Transmitter transits frame
            this->TransmitterTransProcess();
        }
        else {
            re_printf("warning","Shaking hand is not success, so has no whatever action.\n");
            //Erase frame type when it is not transmitted
            this->RemoveCurrentFrameType();
        }
    }
    //Operation mode is slave
    else {
        //Check shaking hand by get RHSIFRXICST.RIPG
        uint8_t RIPG_val = this->GetRHSIFRXICST() & 0x1;
        //Slave has received ping request
        if (RIPG_val == 0x1) {
            //Create frame for transmitter
            this->TransmitterCreateFrameL1(ICLC_val, modelType, logicChanType);
            //Transmitter transits frame
            this->TransmitterTransProcess();
        }
    }
}//}}}

void HSIFL10_ALL::ReceiveProcessICLC(uint32_t payloadL1, bool isICLCCommandValid, bool isCTS)
{//{{{
    if (isICLCCommandValid == true) {
        //Check master or slave operation mode
        uint32_t val_mode = this->GetRHSIFMDCR() & 0x1;
        //Operation mode is slave
        if (val_mode == 0) {
            //Check shaking hand by get RHSIFRXICST.RIPG
            uint8_t RIPG_val = this->GetRHSIFRXICST() & 0x1;
            //Check ping request or not, if true then create ping request
            if (payloadL1 == 0x00) {
                //Update Rx Complete Status Register: ICLC
                this->UpdateRHSIFRXCMPST(emICLCFrameL1);
                //Update register RHSIFRXICST
                this->UpdateRHSIFRXICST(0x00);
                //Update RHSIFL1SR.RCTS
                this->UpdateRHSIFL1SR(16, isCTS);
                if (isCTS == false) {
                    mPre_Hsif_tx_data_ready = false;
                    if (mHsif_tx_data_ready == true) {
                        mHsif_tx_data_ready = false;
                        mWriteHsif_frame_readyEvent.notify(CalculateClockEdge());
                    } 
                } 
                else {
                    uint8_t RXLPBK_val = (this->GetRHSIFTMDCR() & 0x1);
                    if ((mIsLoopBackMode == true) && (RXLPBK_val == 1)) {
                        return;
                    }
                    mPre_Hsif_tx_data_ready = true;
                    //Notify to TransProcessMethod
                    mTransProcessEvent.notify(SC_ZERO_TIME);
                }

                //Check exist the previous Ping answer
                bool isExist = this->CheckExistPreFrameTypeBuffer(emICLCFramePingAns);
                if (isExist == false) {
                    //Create ping answer and send back to master
                    this->StoreFrameTypeIntoBuffer(emICLCFramePingAns);
                    this->TransProcessMethod();
                }

                //Issue receive ICLC interrupt
                uint8_t RHSIFRXICIE_bit_val = mHSIFL10_ALL_Func->GetRHSIFRXICIE(0);
                if (RHSIFRXICIE_bit_val == 0x1) {
                    //Issue interrupt
                    double current_time = sc_time_stamp().to_double();
                    mAssertRXICLC_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                    re_printf("info","Rx ICLC interrupt enable.\n");
                }
                else {
                    double current_time = sc_time_stamp().to_double();
                    mDeassertRXICLC_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                    re_printf("info","Rx ICLC interrupt is not enable.\n");
                }
            }
            else if (this->CheckShakingHand() == true) {
                //Update State turn off loopback by ICLC command
                if (payloadL1 == 0x38) {
                    mIsTurnOffLBCommandICLC = true;
                }
                //Update receiver following command
                this->ICLCCommandProcess(payloadL1);
                //Update Rx Complete Status Register: ICLC
                this->UpdateRHSIFRXCMPST(emICLCFrameL1);
                //Update RHSIFL1SR.RCTS
                this->UpdateRHSIFL1SR(16, isCTS);
                if (isCTS == false) {
                    mPre_Hsif_tx_data_ready = false;
                    if (mHsif_tx_data_ready == true) {
                        mHsif_tx_data_ready = false;
                        mWriteHsif_frame_readyEvent.notify(CalculateClockEdge());
                    } 
                }
                else {
                    mPre_Hsif_tx_data_ready = true;
                    //Notify to TransProcessMethod
                    mTransProcessEvent.notify(SC_ZERO_TIME);
                }
                //Update register RHSIFRXICST
                this->UpdateRHSIFRXICST(payloadL1);

                //Issue receive ICLC interrupt
                uint8_t posBit = this->GetBitPosRHSIFRXICIE(payloadL1);
                uint8_t RHSIFRXICIE_bit_val = mHSIFL10_ALL_Func->GetRHSIFRXICIE(posBit);
                if (RHSIFRXICIE_bit_val == 0x1) {
                    //Issue interrupt
                    double current_time = sc_time_stamp().to_double();
                    mAssertRXICLC_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                    re_printf("info","Rx ICLC interrupt enable.\n");
                }
                else {
                    double current_time = sc_time_stamp().to_double();
                    mDeassertRXICLC_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                    re_printf("info","Rx ICLC interrupt is not enable.\n");
                }
            }
            else {
                re_printf("warning", "Shaking hand is not success, so has no whatever action.\n");
            }
        }
        else {
            re_printf("info","Master don't update whatever with ICLC command (except Ping Answer).\n");
        }
    }
    else if (payloadL1 == 0xABCDEF01) {
        //Need check master or slave operation mode
        unsigned int val_mode = this->GetRHSIFMDCR() & 0x1;
        //Operation mode is master
        if (val_mode == 1) {
            //Generate message notify shaking hand success
            re_printf("info","Shaking hand between master and slave is success.\n");
            //Update register RHSIFRXICST.RIPA (shaking hand success)
            this->UpdateRHSIFRXICST(0x00);
            //Update RHSIFL1SR.RCTS
            this->UpdateRHSIFL1SR(16, isCTS);
            if (isCTS == false) {
                mPre_Hsif_tx_data_ready = false;
                if (mHsif_tx_data_ready == true){
                    mHsif_tx_data_ready = false;
                    mWriteHsif_frame_readyEvent.notify(CalculateClockEdge());
                } 
            }
            else {
                mPre_Hsif_tx_data_ready = true;
                //Notify to TransProcessMethod
                mTransProcessEvent.notify(SC_ZERO_TIME);
            }
            //Update Rx Complete Status Register: ICLC
            this->UpdateRHSIFRXCMPST(emICLCFramePingAns);
            //Issue interrupt
            uint8_t RHSIFRXICIE_bit_val = mHSIFL10_ALL_Func->GetRHSIFRXICIE(16);
            if (RHSIFRXICIE_bit_val == 0x1) {
                //Issue interrupt
                double current_time = sc_time_stamp().to_double();
                mAssertRXICLC_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                re_printf("info","Rx ICLC interrupt enable.\n");
            }
            else {
                double current_time = sc_time_stamp().to_double();
                mDeassertRXICLC_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
                re_printf("info","Rx ICLC interrupt is not enable.\n");
            }
        }
        //Operation mode is slave
        else {
            //Ignore frame and generate warning that slave don't update whatever with ping answer
            re_printf("info","Slave don't update whatever with ping answer.\n");
        }   
    } 
}//}}}

bool HSIFL10_ALL::GetReadRxFiFoEnable()
{//{{{
    return mIsReadRxFiFoEnable;
}//}}}

void HSIFL10_ALL::SetReadRxFiFoEnable(bool isEnable)
{//{{{    
    if (   this->CheckSetClockCorrect("CCLK") == true
        || this->CheckSetClockCorrect("hsif_refclk_in") == true
        ) {
        //Not enable read Rx FIFO, FIFO is full, isEnable is true
        if (mIsReadRxFiFoEnable == false && isEnable == true && (this->GetRHSIFL1SR() >> 1) & 0x1) {
            re_printf("info","Enable read Rx FIFO when Rx FIFO is full.\n");
            //Update status of read Rx FIFO enable
            mIsPreReadRxFiFoEnable = mIsReadRxFiFoEnable;
            mIsReadRxFiFoEnable = isEnable;
            //Notify to ReceiveProcessMethod
            mRecProcessEvent.notify();
        }
        else {
            //Update status of read Rx FIFO enable
            mIsReadRxFiFoEnable = isEnable;
        }
    }
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFCCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFCCR();
}//}}}

void HSIFL10_ALL::UpdateRHSIFCCR(uint8_t posBit, bool isEnable)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFCCR(posBit, isEnable);
}//}}}

void HSIFL10_ALL::SetFlowControlManualType(eFlowControlManualTpye flowControlManualType)
{//{{{
    mFCMType = flowControlManualType;
}//}}}

HSIFL10_ALL_AgentController::eFlowControlManualTpye HSIFL10_ALL::GetFlowControlManualType()
{//{{{
    return mFCMType;
}//}}}

void HSIFL10_ALL::ReceiveProcessL2FrameMethod()
{//{{{
    if (mIsResetHardActive == true || mIsResetCmdReceive == true) {
        re_printf("info", "[%s] Reset (%s) port during reset period.\n", sc_time_stamp().to_string().c_str(), hsif_rx_data.name());
        L2Frame frameL2;
        hsif_rx_data.write(frameL2);
        return;
    }
    //Write data to transmission port
    hsif_rx_data.write(*mL2Frame);
    //Don't have error code
    if (mTranErrorCode == 0) {
        //Update last L2 frame
        this->UpdateRHSIFLTXFRMRL2(mHeaderL1, mHeaderL2);
    } 
    //Notify transmission first byte
    mTransitDataL2ControlEvent.notify(SC_ZERO_TIME);
}//}}}

uint8_t HSIFL10_ALL::GetBitPosRHSIFRXICIE(uint8_t payLoad)
{//{{{
      uint8_t posBit = 0xFF;
      switch (payLoad) {
          case 0x2:
                posBit = 1;
                break;
          case 0x4:
                posBit = 2;
                break;
          case 0x8:
               posBit = 3;
               break;
          case 0x10:
               posBit = 4;
               break;
          case 0x20:
               posBit = 5;
               break;
          case 0x80:
               posBit = 6;
               break;
          case 0x31:
               posBit = 7;
               break;
          case 0x32:
               posBit = 8;
               break;
          default:
               break;
      }
      return posBit;
}//}}}

void HSIFL10_ALL::HandleL2FrmtoL1CtrlSignalMethod()
{//{{{    
    if (   this->CheckSetClockCorrect("CCLK") == true
        || this->CheckSetClockCorrect("hsif_refclk_in") == true
        ) {
        /// If enable reception Frame L2 to L1
        uint8_t stbe = (hsif_tx_control.read() >> 9) & 0x3;
        if (stbe == 0x1) {
            //Get Rx-Tx loopback
            uint8_t RXLPBK_val = this->GetRHSIFTMDCR() & 0x1;
            if (RXLPBK_val == 0x0) {
                mStartHandleL2FrmToL1Event.notify(CalculateClockEdge());
            }
        } else if (stbe == 0x2) {
            mFinishHandleL2FrmToL1Event.notify(CalculateClockEdge());
        }
    }
    else {
        re_printf("warning", "Communication Clock (CCLK) is zero when operation as master or Reference Clock In is zero when operation as slave.\n");
    }
}//}}}

void HSIFL10_ALL::StartHandleL2FrmToL1Method()
{//{{{
    this->StoreFrameTypeIntoBuffer(emFrameL2);
    mTransProcessEvent.notify(CalculateClockEdge());
}//}}}

void HSIFL10_ALL::FinishHandleL2FrmToL1Method()
{//{{{
}//}}}

sc_time HSIFL10_ALL::CalTimeToCmplReceiveL2Frm()
{//{{{
    double time4Transfer = 0;

    uint32_t tank       = TX_FIFO_SIZE_BYTE;      /// Depth of Tx Fifo
    sc_dt::uint64 vin   = mPCLKFreq;         /// vin: Hz (bit/s)
    sc_dt::uint64 vout  = (sc_dt::uint64)CalBaudrate()/8;
    uint16_t frameLength = this->CalFrameLength() / 8;

    if (vout >= vin) {
        /// push and pop intermediately. Tx Fifo not full
        time4Transfer = (double)((frameLength * mTimeResolutionValue) / vin);
    } else {
        /// vout < vin, -> is save to Tx fifo before transfer.
        /// TimeToCmplReceiveL2Frm is time of all L2 frame is saved to Tx Fifo
        double timeFifoFull = (double)((tank * mTimeResolutionValue) / (vin-vout));
        double numberOfByteMakeFifoFull = (double)((timeFifoFull * vin)/mTimeResolutionValue);
        if (frameLength < numberOfByteMakeFifoFull) {
            time4Transfer = (double)((frameLength* mTimeResolutionValue) / vin);
        }
        else {
            time4Transfer = (double)(timeFifoFull + (frameLength - numberOfByteMakeFifoFull)*mTimeResolutionValue/vout);
            /// The condition make FIFO full
            /// Compute time make fifo full
            ///First time FIFO full
            mIsTxFifoFull = true;
            mUpdateTxFifoFullStatusEvent.notify(CalculateClockEdge().to_double() + timeFifoFull,mTimeResolutionUnit);
        }
    }
    //Notify that Tx FiFo is empty after time4Transfer
    mUpdateTxFifoEmptyStatusEvent.notify(time4Transfer, mTimeResolutionUnit);
    return sc_time(time4Transfer,mTimeResolutionUnit);

}//}}}

void HSIFL10_ALL::WriteHsif_frame_cmpMethod(void)
{//{{{
    hsif_frame_cmp->write(mHsif_frame_cmp);
    DumpInterruptMsg(hsif_frame_cmp.basename(),mHsif_frame_cmp);
}//}}}

void HSIFL10_ALL::AssertHsif_frame_cmpMethod(void)
{//{{{
    mHsif_frame_cmp = true;
    double current_time = sc_time_stamp().to_double();
    mWriteHsif_frame_cmpEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint),mTimeResolutionUnit);
    mDeAssertHsif_frame_cmpEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint) + mPCLKPeriod,mTimeResolutionUnit);
}//}}}

void HSIFL10_ALL::DeAssertHsif_frame_cmpMethod(void)
{//{{{
    mHsif_frame_cmp = false;
    mWriteHsif_frame_cmpEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIFL10_ALL::WriteHsif_frame_readyMethod(void)
{//{{{
    hsif_tx_data_ready->write(mHsif_tx_data_ready);
    DumpInterruptMsg(hsif_tx_data_ready.basename(),mHsif_tx_data_ready);
}//}}}

void HSIFL10_ALL::UpdateTxFifoFullStatusMethod()
{//{{{
    this->UpdateRHSIFL1SR(9, mIsTxFifoFull);
    //Need notify to UpdateTxFifoNotFullStatusMethod
    mUpdateTxFifoNotFullStatusEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod
                                            , false
                                            , sc_time_stamp().to_double() + 8*mCCLKPeriod
                                            , mTimePCLKChangePoint) + 8*mCCLKPeriod, mTimeResolutionUnit);
}//}}}

void HSIFL10_ALL::UpdateTxFifoNotFullStatusMethod()
{//{{{
    mIsTxFifoFull = false;
    this->UpdateRHSIFL1SR(9, mIsTxFifoFull);
}//}}}

uint64_t HSIFL10_ALL::CalBaudrate()
{//{{{
    uint64_t baudrate = 0;
    //Get value of RHSIFSPCR register
    uint32_t val = this->GetRHSIFSPCR(); 
    bool isFastModeTrans    = (val >> 8) & 0x1;
    uint8_t baudrateId         = (val >> 24) & 0x3;
    uint8_t PLLSTBY_val = this->GetRHSIFPCR() & 0x01;
    //Trans fast mode
    if ((isFastModeTrans == 1) && (PLLSTBY_val == 0)) {
        std::map<uint8_t, uint16_t >::iterator ite;
        ite = mBaudrateFastMap.find(baudrateId);
        baudrate = (ite->second)*(sc_dt::uint64)(1.0e+6);
    } else {  //Trans slow mode
        unsigned int val_mode = this->GetRHSIFMDCR() & 0x1;

        if (val_mode == 1) {             //Operation mode is master
            baudrate = (sc_dt::uint64)(2*1.0e+7) / 4;
        } else {                        //Operation mode is slave
            uint8_t val_clk_sel = (this->GetRHSIFMDCR() >> 1) & 0x1;
            if ((val_clk_sel == 0)) {
                baudrate = (sc_dt::uint64)(2*1.0e+7) / 4;
            } else { // val_clk_sel == 1
                baudrate = (sc_dt::uint64)(1.0e+7) / 2;
            }
        }
    }
    return baudrate;
}//}}}

uint16_t HSIFL10_ALL::CalFrameLength()
{//{{{
    uint8_t valPayloadSizeIndex = (uint8_t)mL1Frame->GetPayloadSize();
    uint16_t valTotalFrameSize = 0x0;
    std::map<uint8_t, std::pair<uint16_t, uint16_t> >::iterator it;
    it = mPayloadSizeIndexMap.find(valPayloadSizeIndex);
    valTotalFrameSize = (uint16_t)it->second.second - 1;

    return valTotalFrameSize;
}//}}}

void HSIFL10_ALL::UpdateTxFifoEmptyStatusMethod()
{//{{{
    mIsTxFifoEmpty = true;
    this->UpdateRHSIFL1SR(8, mIsTxFifoEmpty);
}//}}}

void HSIFL10_ALL::UpdateTxFifoNotEmptyStatusMethod()
{//{{{
    mIsTxFifoEmpty = false;
    this->UpdateRHSIFL1SR(8, mIsTxFifoEmpty);
}//}}}

void HSIFL10_ALL::UpdateRHSIFLTXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFLTXFRMRL1(header, payload, isEndbit);
}//}}}

void HSIFL10_ALL::UpdateRHSIFLTXFRMRL2(uint8_t headerL1, uint16_t headerL2)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFLTXFRMRL2(headerL1, headerL2);
}//}}}

void HSIFL10_ALL::UpdateRHSIFLRXFRMRL1(uint8_t header, uint8_t payload, bool isEndbit)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFLRXFRMRL1(header, payload, isEndbit);
}//}}}

void HSIFL10_ALL::UpdateRHSIFLRXFRMRL2(uint8_t headerL1, uint16_t headerL2)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFLRXFRMRL2(headerL1, headerL2);
}//}}}

void HSIFL10_ALL::UpdateRHSIFTMDCR(uint8_t posBit, bool isEnable, bool isTurnOffTestMode)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFTMDCR(posBit, isEnable, isTurnOffTestMode);
}//}}}

bool HSIFL10_ALL::CheckExistPreFrameTypeBuffer(eFrameType frameType)
{//{{{
    std::set<eFrameType>::iterator it = mFrameTypeBuffer.find(frameType);
    if (it != mFrameTypeBuffer.end()) {
       return true;
    }
    return false;
}//}}}

bool HSIFL10_ALL::CheckShakingHand()
{//{{{
    uint8_t mode_val = this->GetRHSIFMDCR() & 0x1;
    uint8_t RIPA_val = (this->GetRHSIFRXICST() & 0x00010000) >> 16;
    uint8_t RIPG_val = this->GetRHSIFRXICST() & 0x1;
    //Shaking hand success
    if ((mode_val == 0x1 && RIPA_val == 0x1)
        || (mode_val == 0x0 && RIPG_val == 0x1)
        ) {
            return true;
    }
    return false;
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFSMCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFSMCR();
}//}}}

void HSIFL10_ALL::SetRefCLKOut(sc_dt::uint64 refCLKOut)
{//{{{
    hsif_refclk_out.write(refCLKOut);
}//}}}

bool HSIFL10_ALL::CheckTxInterruptEnable()
{//{{{
    //Get value of RHSIFTXERRIE register
    uint32_t RHSIFTXERRIE_val = this->GetRHSIFTXERRIE();
    //Get value of RHSIFTXERRST register
    uint32_t RHSIFTXERRST_val = this->GetRHSIFTXERRST();
    
    bool TERESZ_val    = (RHSIFTXERRIE_val >> 31) & 0x1;
    bool TERESZI5_val  = (RHSIFTXERRIE_val >> 20) & 0x1;
    bool TERESZI4_val  = (RHSIFTXERRIE_val >> 19) & 0x1;
    bool TERECTF_val   = (RHSIFTXERRIE_val >> 15) & 0x1;
    bool TERECTE_val   = (RHSIFTXERRIE_val >> 14) & 0x1;
    bool TERECTD_val   = (RHSIFTXERRIE_val >> 13) & 0x1;
    bool TERECTC_val   = (RHSIFTXERRIE_val >> 12) & 0x1;
    bool TERECTB_val   = (RHSIFTXERRIE_val >> 11) & 0x1;
    bool TERECTA_val   = (RHSIFTXERRIE_val >> 10) & 0x1;
    bool TERECT9_val   = (RHSIFTXERRIE_val >> 9)  & 0x1;
    bool TERECT8_val   = (RHSIFTXERRIE_val >> 8)  & 0x1;
    bool TERECT2_val   = (RHSIFTXERRIE_val >> 2)  & 0x1;
    bool TERECT1_val   = (RHSIFTXERRIE_val >> 1)  & 0x1;
    
    bool TERSZ_val    = (RHSIFTXERRST_val >> 31) & 0x1;
    bool TERSZI5_val  = (RHSIFTXERRST_val >> 20) & 0x1;
    bool TERSZI4_val  = (RHSIFTXERRST_val >> 19) & 0x1;
    bool TERCTF_val   = (RHSIFTXERRST_val >> 15) & 0x1;
    bool TERCTE_val   = (RHSIFTXERRST_val >> 14) & 0x1;
    bool TERCTD_val   = (RHSIFTXERRST_val >> 13) & 0x1;
    bool TERCTC_val   = (RHSIFTXERRST_val >> 12) & 0x1;
    bool TERCTB_val   = (RHSIFTXERRST_val >> 11) & 0x1;
    bool TERCTA_val   = (RHSIFTXERRST_val >> 10) & 0x1;
    bool TERCT9_val   = (RHSIFTXERRST_val >> 9) & 0x1;
    bool TERCT8_val   = (RHSIFTXERRST_val >> 8) & 0x1;
    bool TERCT2_val   = (RHSIFTXERRST_val >> 2) & 0x1;
    bool TERCT1_val   = (RHSIFTXERRST_val >> 1) & 0x1;
    
    if (   ((TERESZ_val == 0x1) && (TERSZ_val == 0x1)    )
        || ((TERESZI5_val == 0x1) && (TERSZI5_val == 0x1))
        || ((TERESZI4_val == 0x1) && (TERSZI4_val == 0x1))
        || ((TERECTF_val == 0x1)  && (TERCTF_val == 0x1) )
        || ((TERECTE_val == 0x1)  && (TERCTE_val == 0x1) )
        || ((TERECTD_val == 0x1)  && (TERCTD_val == 0x1) )
        || ((TERECTC_val == 0x1)  && (TERCTC_val == 0x1) )
        || ((TERECTB_val == 0x1)  && (TERCTB_val == 0x1) )
        || ((TERECTA_val == 0x1)  && (TERCTA_val == 0x1) )
        || ((TERECT9_val == 0x1)  && (TERCT9_val == 0x1) )
        || ((TERECT8_val == 0x1)  && (TERCT8_val == 0x1) )
        || ((TERECT2_val == 0x1)  && (TERCT2_val == 0x1) )
        || ((TERECT1_val == 0x1)  && (TERCT1_val == 0x1) )
    ) {
        return true;
    }
    return false;
}//}}}

bool HSIFL10_ALL::CheckRxInterruptEnable()
{//{{{
    //Get value of RHSIFRXERRIE register
    uint32_t RHSIFRXERRIE_val = this->GetRHSIFRXERRIE();
    //Get value of RHSIFRXERRST register
    uint32_t RHSIFRXERRST_val = this->GetRHSIFRXERRST();
    
    bool RERESZ_val    = (RHSIFRXERRIE_val >> 31) & 0x1;
    bool RERESZI5_val  = (RHSIFRXERRIE_val >> 20) & 0x1;
    bool RERESZI4_val  = (RHSIFRXERRIE_val >> 19) & 0x1;
    bool RERECTF_val   = (RHSIFRXERRIE_val >> 15) & 0x1;
    bool RERECTE_val   = (RHSIFRXERRIE_val >> 14) & 0x1;
    bool RERECTD_val   = (RHSIFRXERRIE_val >> 13) & 0x1;
    bool RERECTC_val   = (RHSIFRXERRIE_val >> 12) & 0x1;
    bool RERECTB_val   = (RHSIFRXERRIE_val >> 11) & 0x1;
    bool RERECTA_val   = (RHSIFRXERRIE_val >> 10) & 0x1;
    bool RERECT9_val   = (RHSIFRXERRIE_val >> 9)  & 0x1;
    bool RERECT8_val   = (RHSIFRXERRIE_val >> 8)  & 0x1;
    bool RERECT2_val   = (RHSIFRXERRIE_val >> 2)  & 0x1;
    bool RERECT1_val   = (RHSIFRXERRIE_val >> 1)  & 0x1;
    bool REREIPV_val   = (RHSIFRXERRIE_val >> 0)  & 0x1;
    
    bool RERSZ_val    = (RHSIFRXERRST_val >> 31) & 0x1;
    bool RERSZI5_val  = (RHSIFRXERRST_val >> 20) & 0x1;
    bool RERSZI4_val  = (RHSIFRXERRST_val >> 19) & 0x1;
    bool RERCTF_val   = (RHSIFRXERRST_val >> 15) & 0x1;
    bool RERCTE_val   = (RHSIFRXERRST_val >> 14) & 0x1;
    bool RERCTD_val   = (RHSIFRXERRST_val >> 13) & 0x1;
    bool RERCTC_val   = (RHSIFRXERRST_val >> 12) & 0x1;
    bool RERCTB_val   = (RHSIFRXERRST_val >> 11) & 0x1;
    bool RERCTA_val   = (RHSIFRXERRST_val >> 10) & 0x1;
    bool RERCT9_val   = (RHSIFRXERRST_val >> 9) & 0x1;
    bool RERCT8_val   = (RHSIFRXERRST_val >> 8) & 0x1;
    bool RERCT2_val   = (RHSIFRXERRST_val >> 2) & 0x1;
    bool RERCT1_val   = (RHSIFRXERRST_val >> 1) & 0x1;
    bool RERIPV_val   = (RHSIFRXERRST_val >> 0) & 0x1;
    
    if (   ((RERESZ_val == 0x1) && (RERSZ_val == 0x1)    )
        || ((RERESZI5_val == 0x1) && (RERSZI5_val == 0x1))
        || ((RERESZI4_val == 0x1) && (RERSZI4_val == 0x1))
        || ((RERECTF_val == 0x1)  && (RERCTF_val == 0x1) )
        || ((RERECTE_val == 0x1)  && (RERCTE_val == 0x1) )
        || ((RERECTD_val == 0x1)  && (RERCTD_val == 0x1) )
        || ((RERECTC_val == 0x1)  && (RERCTC_val == 0x1) )
        || ((RERECTB_val == 0x1)  && (RERCTB_val == 0x1) )
        || ((RERECTA_val == 0x1)  && (RERCTA_val == 0x1) )
        || ((RERECT9_val == 0x1)  && (RERCT9_val == 0x1) )
        || ((RERECT8_val == 0x1)  && (RERCT8_val == 0x1) )
        || ((RERECT2_val == 0x1)  && (RERCT2_val == 0x1) )
        || ((RERECT1_val == 0x1)  && (RERCT1_val == 0x1) )
        || ((REREIPV_val == 0x1)  && (RERIPV_val == 0x1) )
    ) {
        return true;
    }
    return false;
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFTXERRIE()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFTXERRIE();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFRXERRIE()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFRXERRIE();
}//}}}

void HSIFL10_ALL::UpdateRHSIFTXERRST(uint32_t errorCode)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFTXERRST(errorCode);
}//}}}

void HSIFL10_ALL::UpdateRHSIFRXERRST(uint32_t errorCode)
{//{{{
    mHSIFL10_ALL_Func->UpdateRHSIFRXERRST(errorCode);
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFTXERRST()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFTXERRST();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFRXERRST()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFRXERRST();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFRXCMPST()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFRXCMPST();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFTXCMPST()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFTXCMPST();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFLTXFRMRL1()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFLTXFRMRL1();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFLRXFRMRL1()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFLRXFRMRL1();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFLTXFRMRL2()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFLTXFRMRL2();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFLRXFRMRL2()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFLRXFRMRL2();
}//}}}

uint32_t HSIFL10_ALL::GetRHSIFTMDCR()
{//{{{
    return mHSIFL10_ALL_Func->GetRHSIFTMDCR();
}//}}}

void HSIFL10_ALL::IssueErrorFrame(std::string mode, uint32_t errorCode)
{//{{{
    if (this->GetResetStatus() || (!CheckClockPeriod("PCLK"))) {
        re_printf("info", "Not receive error input during reset period or clock is stopped.\n");
        return;
    }
    
    re_printf("info", "[%s] (%s) Issue Error to model %x.\n", sc_time_stamp().to_string().c_str(), this->name(), errorCode);
    if (mode == "Tx") {
        mTranErrorCode = errorCode;
    }
    else if (mode == "Rx") {
        mReErrorCode = errorCode;
    }
    else {
        re_printf("info","[%s] %s Protocol type is wrong, it is must Tx or Rx.\n", sc_time_stamp().to_string().c_str(), this->name());
    }
}//}}}

sc_dt::uint64 HSIFL10_ALL::GetCCLKFreq()
{//{{{
    return mCCLKFreq;
}//}}}

sc_dt::uint64 HSIFL10_ALL::GetRefclk_inFreq()
{//{{{
    return mhsif_refclk_inFreq;
}//}}}

void HSIFL10_ALL::RemoveCurrentFrameType()
{//{{{
    if(!mFrameTypeBuffer.empty()) {
        eFrameType frameTypeTemp = mTxCurrentFrameType;
        mFrameTypeBuffer.erase(frameTypeTemp);
    }
}//}}}

/// Dump Status info
void HSIFL10_ALL::DumpStatusInfo()
{//{{{
    std::string type = "StatInfo";
    
    printf("PROFILE(%s): HSIFL10_ALL: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFTXERRST bits: 0x%02X\n", type.c_str(), this->GetRHSIFTXERRST());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFRXERRST bits: 0x%02X\n", type.c_str(), this->GetRHSIFRXERRST());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFTXCMPST bits: 0x%02X\n", type.c_str(), this->GetRHSIFTXCMPST());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFRXCMPST bits: 0x%02X\n", type.c_str(), this->GetRHSIFRXCMPST());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFRXICST bits: 0x%02X\n", type.c_str(), this->GetRHSIFRXICST());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFLTXFRMRL1 bits: 0x%02X\n", type.c_str(), this->GetRHSIFLTXFRMRL1());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFLRXFRMRL1 bits: 0x%02X\n", type.c_str(), this->GetRHSIFLRXFRMRL1());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFLTXFRMRL2 bits: 0x%02X\n", type.c_str(), this->GetRHSIFLTXFRMRL2());
    printf("PROFILE(%s): HSIFL10_ALL:   RHSIFLRXFRMRL2 bits: 0x%02X\n", type.c_str(), this->GetRHSIFLRXFRMRL2());
}//}}}


bool HSIFL10_ALL::CheckSetClockCorrect(const std::string clock_name)
{//{{{
    //Get operation mode
    uint8_t mode_val = this->GetRHSIFMDCR() & 0x1;
    if (clock_name == "hsif_refclk_in" && mode_val == 0x0) {
        //Check fast mode or not
        uint32_t val = this->GetRHSIFSPCR(); 
        bool isFastModeTrans    = (val >> 8) & 0x1;
        uint8_t PLLSTBY_val = this->GetRHSIFPCR() & 0x01;
        uint8_t val_clk_sel = (this->GetRHSIFMDCR() >> 1) & 0x1;
        
        if ( ((isFastModeTrans == 1) && (PLLSTBY_val == 0) && this->GetRefclk_inFreq() != 0)
            || ((val_clk_sel == 0) &&  (this->GetRefclk_inFreq() != 0))
            || ((val_clk_sel == 1) && (this->GetRefclk_inFreq() != 0))
        ) {
            return true;
        } 
    }
    else if (clock_name == "CCLK" && mode_val == 0x1 && this->GetCCLKFreq() != 0) {
        return true;
    }
    return false;
}//}}}

std::string HSIFL10_ALL::GetVersionName()
{//{{{
    return mhsif_version_name;
}//}}}

void HSIFL10_ALL::SetLoopBackMode(bool isLoopBack)
{//{{{
    mIsLoopBackMode = isLoopBack;
}//}}}

bool HSIFL10_ALL::GetTxDataReady()
{//{{{
    return mHsif_tx_data_ready;
}//}}}

void HSIFL10_ALL::SetTxDataReady(bool isTxDataReady)
{//{{{
    mHsif_tx_data_ready = isTxDataReady;
    mWriteHsif_frame_readyEvent.notify(SC_ZERO_TIME);
    re_printf("info", "Tx data ready is enabled when operation of model is loopback mode to normal mode.\n");
}//}}}

void HSIFL10_ALL::TransDataMethod()
{//{{{
    if (mIsResetHardActive == true || mIsResetCmdReceive == true) {
        re_printf("info", "[%s] Reset (%s) port during reset period.\n", sc_time_stamp().to_string().c_str(), hsif_txd.name());
        L1Frame frameL1;
        hsif_txd.write(frameL1);
        return;
    }
    // In case, Rx-Tx loopback mode
    uint8_t RXLPBK_val = (this->GetRHSIFTMDCR() & 0x1);
    if ((mIsLoopBackMode == true) && (RXLPBK_val == 1) || (mIsTurnOffLBCommandICLC == true)) {
        hsif_txd.write(hsif_rxd.read());
        if (mIsTurnOffLBCommandICLC == true) {
            mIsTurnOffLBCommandICLC = false;
        }
    }
    else {
        hsif_txd.write(*mL1Frame);
    }
}//}}}
///Method to handle interrupt
void HSIFL10_ALL::ClearInterrupt(std::string interrupt_name)
{
    double current_time = sc_time_stamp().to_double();
    if (interrupt_name == "int_hsif_txcmp"){
        mDeassertTXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
    } else if (interrupt_name == "int_hsif_txerr"){
        mDeassertTXERR_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
    } else if (interrupt_name == "int_hsif_rxcmp"){
        mDeassertRXCMP_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
    } else if (interrupt_name == "int_hsif_rxerr"){
        mDeassertRXERR_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
    } else { // interrupt_name == "int_hsif_rxiclc"
        mDeassertRXICLC_IrqEvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, current_time, mTimePCLKChangePoint), mTimeResolutionUnit);
    }
}//}}}
// vim600: set foldmethod=marker :
