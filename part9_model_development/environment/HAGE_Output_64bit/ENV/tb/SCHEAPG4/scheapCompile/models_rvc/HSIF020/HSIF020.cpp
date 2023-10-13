// ---------------------------------------------------------------------
// $Id: HSIF020.cpp,v 1.12 2020/04/07 03:56:04 quynhnhunguyen Exp $
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

#include "HSIF020.h"
#include "HSIF020_Func.h"
#include "HSIF020_UNIT.h"
#include "HSIF020_UNIT_Func.h"

///Constructor of HSIF020 class
HSIF020::HSIF020 (sc_module_name name
                 ,uint32_t rLatency
                 ,uint32_t wLatency
                 ,SIM_MODE_T simmode):
sc_module(name),
HSIF_ultis(this->basename()),
HSIF020_AgentController(),
BusSlaveBase<64,1>(),
BusMasterBase<64,1>(),
CCLK("CCLK"),
ACLK("ACLK"),
ARESETn("ARESETn"),
creset_n("creset_n"),
hsif_tx_data("hsif_tx_data"),
hsif_tx_control("hsif_tx_control"),
hsif_tx_data_ready("hsif_tx_data_ready"),
hsif_tx_frame_cmp("hsif_tx_frame_cmp"),
hsif_rx_data("hsif_rx_data"),
hsif_rx_control("hsif_rx_control"),
int_hsif_str("int_hsif_str"),
int_hsif_err("int_hsif_err"),
int_hsif_sec("int_hsif_sec"),
hsif_authent1_pass("hsif_authen1_pass"),
hsif_authent2_pass("hsif_authen2_pass"),
mL2FrameOut(L2Frame()),
rvc_common_model()
// Input ports
{//{{{
    for (uint32_t index = 0; index < emCHNum; index ++){
        std::string strName = (std::string)name;
        std::string tempName = CombineStringUint("_uint", index);
        strName.append(tempName);
        if (index == emCH2){
            mHSIF020_Unit[index] = new HSIF020_UNIT(strName.c_str(), this, index, true);
        } else {
            mHSIF020_Unit[index] = new HSIF020_UNIT(strName.c_str(), this, index, false);
        }
    }

    /// Socket to Bus IF
    setSlaveFreqPort64(&ACLK);
    setSlaveResetPort64(&ARESETn);
    setTargetSocket64((char*)"as_ts");

    setMasterResetPort64(&ARESETn);
    setMasterFreqPort64(&ACLK);
    setInitiatorSocket64((char*)"am_is");

    mHSIF020_Func = new HSIF020_Func((std::string)name, this, mBusMasterIf64[0], simmode);

    as_ts = BusSlaveBase<64,1>::tSocket64[0];
    mBusSlaveIf64[0]->setFuncModulePtr(mHSIF020_Func);
    mBusSlaveIf64[0]->setBusProtocol(BUS_AXI);
    mBusSlaveIf64[0]->setReadInitLatency(rLatency);
    mBusSlaveIf64[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf64[0]->setTransNmbLmt(0x2);

    am_is = BusMasterBase<64,1>::iSocket64[0];
    mBusMasterIf64[0]->setFuncModulePtr(mHSIF020_Func);
    mBusMasterIf64[0]->setBusProtocol(BUS_AXI);
    mBusMasterIf64[0]->setTransNmbLmt(0x2);
    mBusMasterIf64[0]->setTlmTransType(simmode);
    sc_assert(mHSIF020_Func != NULL);

    std::string port_name;
    for (uint32_t index = 0; index < emCHNum; index++){
        port_name = CombineStringUint("int_hsif_ch", index);
        int_hsif_ch[index] = new sc_out<bool>(port_name.c_str());
        sc_assert(int_hsif_ch[index] != NULL);
        int_hsif_ch[index]->initialize(false);
    }
    int_hsif_str.initialize(false);
    int_hsif_err.initialize(false);
    int_hsif_sec.initialize(false);

    /// Initialize output port
    hsif_tx_data.initialize(L2Frame());
    hsif_tx_control.initialize(0x0);
    hsif_authent1_pass.initialize(false);
    hsif_authent2_pass.initialize(false);

    /// Initialize variables
    Initialize();
    mAuthenIdBaseEnable = false;
    mAuthenCREnable = false;

    for (unsigned int reset_id = 0; reset_id < emResetSignalNum; reset_id++) {
        mResetCurVal[reset_id] = true;
        mResetPreVal[reset_id] = true;
        mIsResetHardActive[reset_id] = false;
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
        mResetCmdPeriod[reset_id] =  0;
    }    
    mThisIP_SPID        = HSIF0MASTERID;
    mPEID               = HSIF0MASTERID;
    mCCLKPeriod = 0;
    mCCLKFreq = 0;
    mCCLKOrgFreq = 0;
    mCCLKFreqUnit = "Hz";
    mACLKPeriod = 0;
    mACLKFreq = 0;
    mACLKOrgFreq = 0;
    mACLKFreqUnit = "Hz";
    mCCLKSyncTPoint = 0;
    mACLKSyncTPoint = 0;
    mDumpInterrupt          = false;
    
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    /// Register process to SystemC kernel
    SC_METHOD(HandleL1toL2ControlSignalMethod);
    dont_initialize();
    sensitive << hsif_rx_control;

    SC_METHOD(IssueCmdMethod);
    dont_initialize();
    sensitive << mIssueCmdEvent;

    SC_METHOD(WriteTxPortMethod);
    dont_initialize();
    sensitive << mWriteTxPortEvent;

    SC_METHOD(HandleL1ReadyPortMethod);
    dont_initialize();
    sensitive << hsif_tx_data_ready;

    SC_METHOD(HandleL1CompletePortMethod);
    dont_initialize();
    sensitive << hsif_tx_frame_cmp;

    SC_METHOD(ClearTxControlSignalMethod);
    dont_initialize();
    sensitive << mClearTxControlSignalEvent;

    SC_METHOD(StartHandleReceivedL1toL2Method);
    dont_initialize();
    sensitive << mStartHandleReceivedL1toL2Event;

    SC_METHOD(FinishHandleReceivedL1toL2Method);
    dont_initialize();
    sensitive << mFinishHandleReceivedL1toL2Event;

    SC_THREAD(TransProcessThread);
    dont_initialize();
    sensitive << mNotifyAxiTransferTransactionEvent;

    SC_METHOD(HandleACLKSignalMethod);
    dont_initialize();
    sensitive << ACLK;

    SC_METHOD(HandleCCLKSignalMethod);
    dont_initialize();
    sensitive << CCLK;

    SC_METHOD(HandleARESETnSignalMethod);
    dont_initialize();
    sensitive << ARESETn;

    SC_METHOD(Handlecreset_nSignalMethod);
    dont_initialize();
    sensitive << creset_n;

    for (unsigned int rst_id = 0; rst_id < emResetSignalNum; rst_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetHardEvent[rst_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HSIF020::HandleResetHardMethod, this, rst_id),
                          sc_core::sc_gen_unique_name("HandleResetHardMethod"), &opt);
    }

    for (unsigned int rst_id = 0; rst_id < emResetSignalNum; rst_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdEvent[rst_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HSIF020::HandleResetCmdMethod, this, rst_id),
                          sc_core::sc_gen_unique_name("HandleResetCmdMethod"), &opt);
    }

    for (unsigned int rst_id = 0; rst_id < emResetSignalNum; rst_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mResetCmdCancelEvent[rst_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HSIF020::CancelResetCmdMethod, this, rst_id),
                          sc_core::sc_gen_unique_name("CancelResetCmdMethod"), &opt);
    }

    for (uint8_t ch_id = 0; ch_id < emCHNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteInt_hsif_chEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HSIF020::WriteInt_hsif_chMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("WriteInt_hsif_chMethod"), &opt);
    }

    for (uint8_t ch_id = 0; ch_id < emCHNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mAssertInt_hsif_chEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HSIF020::AssertInt_hsif_chMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("AssertInt_hsif_chMethod"), &opt);
    }

    for (uint8_t ch_id = 0; ch_id < emCHNum; ch_id++) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mDeAssertInt_hsif_chEvent[ch_id]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&HSIF020::DeAssertInt_hsif_chMethod, this, ch_id),
                          sc_core::sc_gen_unique_name("DeAssertInt_hsif_chMethod"), &opt);
    }

    for (uint8_t ch_id = 0; ch_id < emCHNum; ch_id++) {
            sc_core::sc_spawn_options opt;
            opt.spawn_method();
            opt.set_sensitivity(&mTxNonStreamTimeOutEvent[ch_id]);
            opt.dont_initialize();
            sc_core::sc_spawn(sc_bind(&HSIF020::HandleTimeOutNonStreamMethod, this, ch_id),
                              sc_core::sc_gen_unique_name("HandleTimeOutMethod"), &opt);
    }

    SC_METHOD(HandleTimeOutStreamMethod);
    dont_initialize();
    sensitive << mTxStreamTimeOutEvent;

    SC_METHOD(WriteInt_hsif_strMethod);
    dont_initialize();
    sensitive << mWriteInt_hsif_strEvent;

    SC_METHOD(AssertInt_hsif_strMethod);
    dont_initialize();
    sensitive << mAssertInt_hsif_strEvent;

    SC_METHOD(DeAssertInt_hsif_strMethod);
    dont_initialize();
    sensitive << mDeAssertInt_hsif_strEvent;

    SC_METHOD(WriteInt_hsif_errMethod);
    dont_initialize();
    sensitive << mWriteInt_hsif_errEvent;

    SC_METHOD(AssertInt_hsif_errMethod);
    dont_initialize();
    sensitive << mAssertInt_hsif_errEvent;

    SC_METHOD(DeAssertInt_hsif_errMethod);
    dont_initialize();
    sensitive << mDeAssertInt_hsif_errEvent;

    SC_METHOD(WriteInt_hsif_secMethod);
    dont_initialize();
    sensitive << mWriteInt_hsif_secEvent;

    SC_METHOD(AssertInt_hsif_secMethod);
    dont_initialize();
    sensitive << mAssertInt_hsif_secEvent;

    SC_METHOD(DeAssertInt_hsif_secMethod);
    dont_initialize();
    sensitive << mDeAssertInt_hsif_secEvent;

    SC_METHOD(WriteHsif_authent1_passMethod);
    dont_initialize();
    sensitive << mWriteHsif_authent1_passEvent;

    SC_METHOD(WriteHsif_authent2_passMethod);
    dont_initialize();
    sensitive << mWriteHsif_authent2_passEvent;

    SC_METHOD(TxStreamGetData2FifoMethod);
    dont_initialize();
    sensitive << mTxStreamGetData2FifoEvent;

    SC_METHOD(RxStreamWriteData2MemMethod);
    dont_initialize();
    sensitive << mRxStreamWriteData2MemEvent;

    SC_METHOD(TxTransferStreamCmdMethod);
    dont_initialize();
    sensitive << mTxTransferStreamCmdEvent;
}//}}}

///Destructor of HSIF020 class
HSIF020::~HSIF020 (void)
{//{{{
    for (uint32_t index = 0; index < emCHNum; index ++){
        if (mHSIF020_Unit[index] != NULL ) delete mHSIF020_Unit[index];
        if (int_hsif_ch[index] != NULL ) delete int_hsif_ch[index];
    }
    if (mHSIF020_Func != NULL) delete mHSIF020_Func;
}//}}}

/// Cancel operation events
void HSIF020::CancelEventsReset (void)
{//{{{
    /// Declare Internal event
    mStartHandleReceivedL1toL2Event.cancel();
    mFinishHandleReceivedL1toL2Event.cancel();

    mIssueCmdEvent.cancel();
    mWriteTxPortEvent.notify();
    mClearTxControlSignalEvent.cancel();

    for (uint8_t index = 0; index < emCHNum; index ++){
        mTxNonStreamTimeOutEvent[index].cancel();
    }
    mTxStreamTimeOutEvent.cancel();
    ///AXI processing
    mNotifyAxiTransferTransactionEvent.cancel();

    ///Interrupt write event
    for (uint8_t index = 0; index < emCHNum; index ++){
        mWriteInt_hsif_chEvent[index].cancel();
        mAssertInt_hsif_chEvent[index].cancel();
        mDeAssertInt_hsif_chEvent[index].notify();
    }
    mWriteInt_hsif_strEvent.cancel();
    mAssertInt_hsif_strEvent.cancel();
    mDeAssertInt_hsif_strEvent.notify();

    mWriteInt_hsif_errEvent.cancel();
    mAssertInt_hsif_errEvent.cancel();
    mDeAssertInt_hsif_errEvent.notify();

    mWriteInt_hsif_secEvent.cancel();
    mAssertInt_hsif_secEvent.cancel();
    mDeAssertInt_hsif_secEvent.notify();

    mWriteTxPortEvent.notify();
    mWriteHsif_authent1_passEvent.notify();
    mWriteHsif_authent2_passEvent.notify();

    mTxStreamGetData2FifoEvent.cancel();
    mRxStreamWriteData2MemEvent.cancel();
    mTxTransferStreamCmdEvent.cancel();
}//}}}

/// Cancel operation events
void HSIF020::CancelEventsZeroClock (void)
{//{{{
    /// Declare Internal event
    mStartHandleReceivedL1toL2Event.cancel();
    mFinishHandleReceivedL1toL2Event.cancel();

    mIssueCmdEvent.cancel();
    mWriteTxPortEvent.cancel();
    mClearTxControlSignalEvent.cancel();

    for (uint8_t index = 0; index < emCHNum; index ++){
        mTxNonStreamTimeOutEvent[index].cancel();
    }
    mTxStreamTimeOutEvent.cancel();
    ///AXI processing
    mNotifyAxiTransferTransactionEvent.cancel();

    ///Interrupt write event
    for (uint8_t index = 0; index < emCHNum; index ++){
        mWriteInt_hsif_chEvent[index].cancel();
        mAssertInt_hsif_chEvent[index].cancel();
        mDeAssertInt_hsif_chEvent[index].cancel();
    }
    mWriteInt_hsif_strEvent.cancel();
    mAssertInt_hsif_strEvent.cancel();
    mDeAssertInt_hsif_strEvent.cancel();

    mWriteInt_hsif_errEvent.cancel();
    mAssertInt_hsif_errEvent.cancel();
    mDeAssertInt_hsif_errEvent.cancel();

    mWriteInt_hsif_secEvent.cancel();
    mAssertInt_hsif_secEvent.cancel();
    mDeAssertInt_hsif_secEvent.cancel();

    mWriteTxPortEvent.cancel();
    mWriteHsif_authent1_passEvent.cancel();
    mWriteHsif_authent2_passEvent.cancel();

    mTxStreamGetData2FifoEvent.cancel();
    mRxStreamWriteData2MemEvent.cancel();
    mTxTransferStreamCmdEvent.cancel();
}//}}}

/// Initialize internal variables
void HSIF020::Initialize (void)
{//{{{
    mL2FrameOut.Initialize();
    mJtagId                 = 0xCAFEBABE;
    mCurrentChannelId       = 0;
    mTxStreamOutstand       = 0;
    mTxCtrlVal              = 0;
    mFrameId                = 0x7;

    mIsDurTransStreamCmd    = false;
    mIsInitialSetting       = true;
    mIsTxFree               = true;
    mTxFrameCmp             = false;
    mTxDataReady            = false;
    
    mEnableRegisterMessage  = true;
    mIsCrcError             = false;
    mIsTransIdError         = false;
    mIsAxiError             = false;

    for (uint8_t ch_id = 0; ch_id < emCHNum; ch_id ++){
        mInt_hsif_ch[ch_id] = false;
    }
    mInt_hsif_str           = false;
    mInt_hsif_err           = false;
    mInt_hsif_sec           = false;
    mAuthenIdBasePass       = false;
    mAuthenCRPass           = false;
    mAuthenIdBaseLock       = false;
    mAuthenCRLock           = false;

    while(!ChannelArbitterL2.empty()){
        ChannelArbitterL2.erase(ChannelArbitterL2.begin());
    }

    mHSIF020_Func->Initialize();
    for (uint8_t index = 0; index < emCHNum; index++) {
        mHSIF020_Unit[index]->Initialize();
    }
}//}}}

/// Handle control signal (hsif_rx_control) for L1 to L2
void HSIF020::HandleL1toL2ControlSignalMethod(void)
{//{{{
    uint8_t ctlSignal = uint8_t((hsif_rx_control.read() >> 9) & 0x3);
    if (!CheckClockPeriod("CCLK")){
        return;
    }
    if (!(  mAuthenCRLock ||mAuthenIdBaseLock )){
        if (ctlSignal == (uint8_t)emFirstByte){
            mStartHandleReceivedL1toL2Event.notify(SC_ZERO_TIME);
        } else if (ctlSignal == (uint8_t)emLastByte){
            mFinishHandleReceivedL1toL2Event.notify(SC_ZERO_TIME);
        } else if (ctlSignal == (uint8_t)emAborted){
            re_printf("info","Ignore input control signal (Abort received).");
        }
    } else {
        re_printf("warning","Model is locked because of 3 times failed authentication! Please reset model to continue.");
    }
}//}}}

/// Start Handle Received L1 to L2
/// Details
///      Start frame: + Non-stream command (Finish received all frame)  -> Process
///                   + Stream command       -> Save to buffer (Finish) -> AXI transfer
/// Detailed steps as follows:
///
void HSIF020::StartHandleReceivedL1toL2Method(void)
{//{{{
    L2Frame tempL2Frame = hsif_rx_data.read();
    uint8_t command  = tempL2Frame.GetCommand();

    if (command == emStreamCmd){
        if (mHSIF020_Unit[emStreamChannel]->mEnableStream
        &&mHSIF020_Unit[emStreamChannel]->IsChRxEnable()){
            mHSIF020_Func->SetRxStreamStatus(emSRTA);
        } 
    }

    re_printf("info","Start received Frame from L1.");
}//}}}

///Finish Handle Received L1 to L2
///          + Non-stream command (Finish received all frame)  -> Process
///          + Stream command       -> Save to buffer (Finish) -> AXI transfer
void HSIF020::FinishHandleReceivedL1toL2Method(void)
{//{{{
    if (!CheckClockPeriod("CCLK")){
        return;
    }
    //Process received frame
    L2Frame tempL2Frame = hsif_rx_data.read();
    uint8_t logicChannelType = (hsif_rx_control.read() >> 16) & 0xF;

    uint32_t    addrRW      = 0;
    uint32_t    writeData   = 0;
    ChId_t      ch_id    = 0;
    bool isCanDetectChannel = DecodeLogicChType2ChId(logicChannelType,ch_id);
    uint8_t frame_id = tempL2Frame.GetTransactionID();
    uint8_t command  = tempL2Frame.GetCommand();

    if (mIsCrcError){
        if (   (ch_id == emCH2)
            && (mHSIF020_Func->IsTxStreamDMACEnable() || mHSIF020_Func->IsRxStreamDMACEnable())){
            mHSIF020_Func->HandleCrcErrorStreamCmd();
        } else {
            mHSIF020_Unit[ch_id]->IniHandleCrcError();
        }
        mIsCrcError = false;
    } else if (mIsTransIdError){
        if ((ch_id == emCH2) && (mHSIF020_Func->IsTxStreamDMACEnable())){
            mHSIF020_Func->HandleTIDErrorStreamCmd();
        } else {
            if (command == emNACKCmd){
                mHSIF020_Unit[ch_id]->IniHandleUnclassifiedError();
            } else {
                mHSIF020_Unit[ch_id]->IniHandleTIDError();
            }
        }
        mIsTransIdError = false;
    } else if (isCanDetectChannel){
        if      (command == emReadCmd8  ){
            addrRW = tempL2Frame.GetPayloadData().mPayloadUnit[0];
            re_printf("info","Receive a READ 8 bits command at address: 0x%8X on channel: %d ", addrRW, ch_id);
            mHSIF020_Unit[ch_id]->TgrHandleReadCmdProcess(addrRW, emRW8bits, frame_id);
        }
        else if (command == emReadCmd16 ){
            addrRW = tempL2Frame.GetPayloadData().mPayloadUnit[0];
            re_printf("info","Receive a READ 16 bits command at address: 0x%8X on channel: %d ", addrRW, ch_id);
            mHSIF020_Unit[ch_id]->TgrHandleReadCmdProcess(addrRW, emRW16bits, frame_id);
        }
        else if (command == emReadCmd32 ){
            addrRW = tempL2Frame.GetPayloadData().mPayloadUnit[0];
            re_printf("info","Receive a READ 32 bits command at address: 0x%8X on channel: %d ", addrRW, ch_id);
            mHSIF020_Unit[ch_id]->TgrHandleReadCmdProcess(addrRW, emRW32bits, frame_id);
        }
        else if (command == emWriteCmd8 ){
            addrRW = tempL2Frame.GetPayloadData().mPayloadUnit[0];
            writeData = tempL2Frame.GetPayloadData().mPayloadUnit[1];
            re_printf("info","Receive a WRITE 8 bits command at address: 0x%8X on channel: %d with data: 0x%8X", addrRW, ch_id, writeData);
            mHSIF020_Unit[ch_id]->TgrHandleWriteCmdProcess(addrRW, emRW8bits, writeData, frame_id);
        }
        else if (command == emWriteCmd16){
            addrRW = tempL2Frame.GetPayloadData().mPayloadUnit[0];
            writeData = tempL2Frame.GetPayloadData().mPayloadUnit[1];
            re_printf("info","Receive a WRITE 16 bits command at address: 0x%8X on channel: %d with data: 0x%8X", addrRW, ch_id, writeData);
            mHSIF020_Unit[ch_id]->TgrHandleWriteCmdProcess(addrRW,emRW16bits,writeData, frame_id);
        }
        else if (command == emWriteCmd32){
            addrRW = tempL2Frame.GetPayloadData().mPayloadUnit[0];
            writeData = tempL2Frame.GetPayloadData().mPayloadUnit[1];
            re_printf("info","Receive a WRITE 32 bits command at address: 0x%8X on channel: %d with data: 0x%8X", addrRW, ch_id, writeData);
            mHSIF020_Unit[ch_id]->TgrHandleWriteCmdProcess(addrRW,emRW32bits,writeData, frame_id);
        }
        else if (command == emACKCmd        ){
            std::set<uint8_t>::iterator it;
            it = mTxFrameStreamCmdInfo.find(frame_id);
            if ((ch_id == emCH2) && (it !=mTxFrameStreamCmdInfo.end())){
                mTxFrameStreamCmdInfo.erase(it);
                mTxStreamOutstand -= 1;
                mTxStreamTimeOutEvent.cancel();
                mHSIF020_Func->TxStreamHandleACKCmd();
            } else {
                re_printf("info","Receive a ACK of Non-Stream command on channel: %d", ch_id);
                mHSIF020_Unit[ch_id]->IniHandleACKCommand();
            }
        }
        else if (command == emNACKCmd       ){
            std::set<uint8_t>::iterator it;
            it = mTxFrameStreamCmdInfo.find(frame_id);
            if ((it !=mTxFrameStreamCmdInfo.end())){
                mTxFrameStreamCmdInfo.erase(it);
                mTxStreamOutstand -= 1;
                mTxStreamTimeOutEvent.cancel();
                mHSIF020_Func->TxStreamHandleNACKCmd();
            } else {
                re_printf("info","Receive a NACK of Non-Stream command on channel: %d", ch_id);
                mHSIF020_Unit[ch_id]->IniHandleNACKCommand();
            }
        }
        else if (command == emReadAnswerCmd ){
            re_printf("info","Receive a READ ANSWER command on channel: %d", ch_id);
            uint32_t readData = 0;
            readData = tempL2Frame.GetPayloadData().mPayloadUnit[0];
            mHSIF020_Unit[ch_id]->IniHandleReadAnswerCommand(readData);
        }
        else if (command == emEventCmd      ){
            re_printf("info","Receive a EVENT command on channel: %d", ch_id);
            mHSIF020_Unit[ch_id]->TgrHandleEventCommand(frame_id);
        }
        else if (command == emIdCmd     ){
            re_printf("info","Receive a READ ID command on channel: %d", ch_id);
            mHSIF020_Unit[ch_id]->TgrHandleIdCommand(frame_id);
        }
        else if (command == emStreamCmd     ){
            if (mHSIF020_Unit[ch_id]->mEnableStream
              &&mHSIF020_Unit[ch_id]->IsChRxEnable()){
                re_printf("info","Receive a Stream command on channel: %d", ch_id);
                mHSIF020_Func->ClearRxStreamStatus(emSRTA);
                mHSIF020_Func->RxHandleStreamCmd(tempL2Frame.GetPayloadData()
                                                 ,tempL2Frame.GetTransactionID()
                                                 ,(hsif_rx_control.read() & 0x7));
            } else {
                if (!mHSIF020_Unit[ch_id]->mEnableStream) {
                    re_printf("warning","Channel %d is not support stream operation. Ignore Received Frame!", (uint32_t)ch_id);
                }
                if (!mHSIF020_Unit[ch_id]->IsChRxEnable()) {
                    re_printf("warning","Channel %d is not enable Rx stream operation. Ignore Received Frame!", (uint32_t)ch_id);
                }
            }
        } else {
            /// Ignore this frame without any feedback
            mHSIF020_Unit[ch_id]->HandleUnisolatedFrame();
        }
    } else {
        IgnoreReceiveFrame();
    }
}//}}}

/// Ignore Received Frame
void HSIF020::IgnoreReceiveFrame(void)
{//{{{
    re_printf("warning", "Ignore Receive Frame. Cann't detect channel.");
}//}}}

/// Check a read/write request is hit memory window or not
bool HSIF020::IsHitMemWindow(uint32_t startAddr, uint32_t endAddr, bool isWrite)
{//{{{
    return mHSIF020_Func->IsMemWindowAccessPer(startAddr, endAddr, isWrite);
}//}}}

/// Check current status is pass authentication or not
bool HSIF020::IsPassedAuthent(void )
{//{{{
    bool isAuthenIdCheck = false;
    bool isAuthenCRCheck = false;
    if (  ( mAuthenIdBaseEnable && mAuthenIdBasePass)
       || (!mAuthenIdBaseEnable)){
        isAuthenIdCheck = true;
    }

    if (  ( mAuthenCREnable && mAuthenCRPass)
       || (!mAuthenCREnable)){
        isAuthenCRCheck = true;
    }

    return (isAuthenIdCheck && isAuthenCRCheck);
}//}}}

/// Notify event to put current request to Arbiter (Arbiter is implemented by a Set)
void HSIF020::NotifyCmdtoL2Arbiter(ChId_t ch_id)
{//{{{
    ChannelArbitterL2.insert(ch_id);
    mIssueCmdEvent.notify(CalculateClockEdge("ACLK"));
}//}}}

/// Method to start serve command
void HSIF020::IssueCmdMethod(void)
{//{{{
    if (! CheckClockPeriod("CCLK")){
        return;
    }
    if (!(  mAuthenCRLock
          ||mAuthenIdBaseLock)){
        if(!ChannelArbitterL2.empty()){
            std::set<ChId_t>::iterator itChannel = ChannelArbitterL2.begin();
            mCurrentChannelId = *itChannel;
            if (mIsTxFree && mTxDataReady){
                FrammingProcess(mCurrentChannelId);
                ChannelArbitterL2.erase(itChannel);
            }
        }
    } else {
        re_printf("warning","Model is locked because of 3 times failed authentication! Please reset model to continue.");
    }
}//}}}

/// Served command will be Framed and notify write output event
void HSIF020::FrammingProcess(ChId_t ch_id)
{//{{{
    uint8_t payloadSizeIndex;
    // Framing process
    uint8_t command = mL2FrameOut.GetCommand();
    if(command != emStreamCmd){
        mL2FrameOut = mHSIF020_Unit[ch_id]->GetL2FrameOut();
        payloadSizeIndex = mHSIF020_Unit[ch_id]->GetPayLoadSizeIndex();
    }
    if (!((command == emNACKCmd)
      || (command == emACKCmd)
      || (command == emReadAnswerCmd))){
        mL2FrameOut.SetTransactionID(IncTransId());
    }

    /// Stream command, get payload data from Tx fifo
    if (command == emStreamCmd){
        /// Update FIFO status
        mHSIF020_Func->UpdateTxFifoStatusWhenProcessNextData();

        L2Payload_t tempL2Payload;
        tempL2Payload = mHSIF020_Func->TxStreamGetL2PayloadData();
        payloadSizeIndex = emPayLoadSize288;
        if(mHSIF020_Func->GetTxStreamConfigDataWidth() != 0) {
            payloadSizeIndex = emPayLoadSize160;
        }
        mL2FrameOut.SetPayloadData(tempL2Payload);
        mIsDurTransStreamCmd = true;
        mTxStreamOutstand += 1;
        mTxFrameStreamCmdInfo.insert(mFrameId);
    }
    mL2FrameOut.SetChannelNumber(DecodeChNumbFromChId(ch_id));
    mL2FrameOut.SetCRC(ComputeCRC(mL2FrameOut,payloadSizeIndex));

    mTxCtrlVal = ComputeTxControl(payloadSizeIndex
                               , (uint32_t)emFirstByte
                               , DecodeChType4TxControl(ch_id));
    mIsTxFree = false;                                            ///Block Tx
    mWriteTxPortEvent.notify(SC_ZERO_TIME);
    mIssueCmdEvent.notify(CalculateClockEdge("ACLK"));
}//}}}

/// Method to handle Ready signal from L1. If read == 1, L1 can be served command
void HSIF020::HandleL1ReadyPortMethod(void)
{//{{{
    if (!CheckClockPeriod("CCLK")){
        return;
    }
    mTxDataReady = hsif_tx_data_ready.read();
    if (mTxDataReady){
        mIssueCmdEvent.notify(CalculateClockEdge("ACLK"));
    }
}//}}}

/// Handle L1 Complete Port. When complete port == 1, L1 is finished frame from L2
void HSIF020::HandleL1CompletePortMethod(void)
{//{{{
    if (!CheckClockPeriod("CCLK")){
        return;
    }
    mTxFrameCmp = hsif_tx_frame_cmp.read();
    if ((mTxFrameCmp == true) && (!mIsTxFree)){
        mTxCtrlVal &= ~(0x3 << 9);
        mTxCtrlVal |= ((uint32_t)emLastByte << 9);
        mWriteTxPortEvent.notify(CalculateClockEdge("CCLK") + sc_time((double)mCCLKPeriod,mTimeResolutionUnit));
        mClearTxControlSignalEvent.notify(CalculateClockEdge("CCLK") + sc_time((double)mCCLKPeriod * 2,mTimeResolutionUnit));

        /// The reply timer starts when this IP (Layer 2) finishes to
        /// transmit last symbol of a request Command
        uint8_t command = mL2FrameOut.GetCommand();
        if (!((command == emNACKCmd)
           || (command == emACKCmd)
           || (command == emReadAnswerCmd))){
            mTxNonStreamTimeOutEvent[mCurrentChannelId].notify(GetTimeOutSetting() * (double)mCCLKPeriod, mTimeResolutionUnit);
        }
        if (command == emStreamCmd){
            //Finish transfer a stream command
            mIsDurTransStreamCmd= false;
            if ( (mTxStreamOutstand < 2) 
               &&(mHSIF020_Func->IsTxFifoRemainDataExist())){
                mTxTransferStreamCmdEvent.notify(CalculateClockEdge("CCLK") + sc_time((double)mCCLKPeriod * 3,mTimeResolutionUnit));
            }
        mTxStreamTimeOutEvent.notify(GetTimeOutSetting() * (double)mCCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// Clear Tx control signal. Finish transfer a command
void HSIF020::ClearTxControlSignalMethod(void)
{//{{{
    mTxCtrlVal &= ~(0x3 << 9);
    mTxCtrlVal |= ((uint32_t)emIdle << 9);
    mIsTxFree = true;                                             ///Release Tx

    mWriteTxPortEvent.notify(SC_ZERO_TIME);
}//}}}

/// Handle Time out occur (Non-stream operation)
void HSIF020::HandleTimeOutNonStreamMethod(ChId_t ch_id)
{//{{{
    /// Setup status and notify interrput singal
    mHSIF020_Unit[ch_id]->IniNonStreamHandleTimeOutOcurs();
}//}}}

/// Handle Time out occur (Stream operation)
void HSIF020::HandleTimeOutStreamMethod()
{//{{{
    re_printf("warning","Reply time-out occurs. Stop DMAC!");
    mHSIF020_Func->HandleTimeOutStreamOccurs();
}//}}}

/// Configure for AXI transaction
void HSIF020::ConfigTransaction(const bool icommand
                               ,const uint32_t iaddr
                               ,const uint8_t isize
                               ,const bool idebug
                               ,const ChId_t ch_id)
{//{{{
    mHSIF020_Func->ConfigTransaction(icommand,iaddr,isize,false,ch_id);
}//}}}

/// Set Jtag ID for this IP
void HSIF020::SetJtagID(const uint32_t jtag_id)
{//{{{
    mJtagId = jtag_id;
}//}}}

/// Get Jtag ID of this IP
uint32_t HSIF020::GetJtagID(void)
{//{{{
    return mJtagId;
}//}}}

/// Issue an error to model (CRC, TID, AXI)
void HSIF020::IssueError(const std::string error_code)
{//{{{
    if (error_code == "CRC"){
        re_printf("info","Issue a CRC error from PythonIF.");
        mIsCrcError = true;
    } else if (error_code == "TID"){
        re_printf("info","Issue a Transaction ID error from PythonIF.");
        mIsTransIdError= true;
    } else if (error_code == "AXI"){
        re_printf("info","Issue an error in AXI bus from PythonIF.");
        mIsAxiError = true;
    } else {
        re_printf("warning","Error code is wrong. Please check again. There are 3 supported errors: CRC, TID, AXI.");
    }
}//}}}

/// Get configure of time-out register
uint32_t HSIF020::GetTimeOutSetting(void)
{//{{{
    uint32_t regConfig = mHSIF020_Func->GetTimeOutSetting();
    uint32_t rscl = (regConfig >> 12) & 0xF;
    uint32_t rcnt = regConfig & 0x3FF;
    return (uint32_t)((0x1 << rscl) * rcnt);
}//}}}

/// Handle Transaction ID (increase in each frame)
uint8_t HSIF020::IncTransId(void)
{//{{{
    mFrameId += 1;
    if (mFrameId >= 8){
        mFrameId = 0;
    }

    return mFrameId;
}//}}}

/// Write Tx port method
void HSIF020::WriteTxPortMethod(void)
{//{{{
    hsif_tx_control.write(mTxCtrlVal);
    hsif_tx_data.write(mL2FrameOut);
}//}}}

/// Function to compute CRC for each frame
uint16_t HSIF020::ComputeCRC(L2Frame& iL2Frame, uint8_t payloadSize)
{//{{{
    uint8_t mData[34];
    memset(mData,0x0,34);
    uint16_t crc = 0x0;
    uint16_t l2header = iL2Frame.GetL2Header();
    L2Payload_t l2payload = iL2Frame.GetPayloadData();
    switch(payloadSize){
        case emPayLoadSize32:   //ACK, NACK
            memcpy(&mData[0],&l2header,2);
            crc = crc16cccit(mData,2);
            break;
        case emPayLoadSize64:   //Read command - Read Answer
            memcpy(&mData[0],&l2header,2);
            memcpy(&mData[2],&l2payload.mPayloadUnit,4);
            crc = crc16cccit(mData,6);
            break;
        case emPayLoadSize96:   // Write command
            memcpy(&mData[0],&l2header,2);
            memcpy(&mData[2],&l2payload.mPayloadUnit,8);
            crc = crc16cccit(mData,10);
            break;
        case emPayLoadSize160:
            memcpy(&mData[0],&l2header,2);
            memcpy(&mData[2],&l2payload.mPayloadUnit,16);
            crc = crc16cccit(mData,18);
            break;
        case emPayLoadSize288:
            memcpy(&mData[0],&l2header,2);
            memcpy(&mData[2],&l2payload.mPayloadUnit,32);
            crc = crc16cccit(mData,34);
            break;
        default:
            break;
    }

    return crc;
}//}}}

/// CRC16 CCCIT algorithm
uint16_t HSIF020::crc16cccit(uint8_t *data, uint8_t nBytes)
{//{{{
    sc_assert(data);
    uint16_t  out = emInitCRCExtraVal;
    /// Mask top bit of remainder
    uint16_t  maskTopBit = (1 << 15);
    for (int byte = 0; byte < nBytes; ++byte)
    {
        /// Bring the next byte into the remainder.
        out = out ^ (uint16_t)((uint16_t)*data << (15 - 8));
         for (uint8_t bit = 8; bit > 0; --bit){
             if ((out & maskTopBit) != 0){
                 out = (uint16_t)(out << 1) ^ emCRCExtraPolynomial;
             } else {
                 out = (uint16_t)(out << 1);
             }
         }
        data++;
    }

    return (out);
}//}}}

/// Compute Tx control value from each element
uint32_t HSIF020::ComputeTxControl(uint32_t payloadSizeIndex, uint32_t stbe, uint32_t channelType)
{//{{{
    uint32_t tempTx_control;
    tempTx_control = (payloadSizeIndex & 0x7) | (stbe & 0x3) << 9 | (channelType & 0xF) << 16;

    return tempTx_control;
}//}}}

/// Decode Channel type for Tx control signal output
uint32_t HSIF020::DecodeChType4TxControl(uint8_t ch_id)
{//{{{
    /// Logical channel type 0100b: Data channel 1 (A)
    ///                      0101b: Data channel 2 (B)
    ///                      0110b: Data channel 3 (C)
    ///                      0111b: Data channel 4 (D)
    uint32_t chType4TxControl = 0x0;
    switch(ch_id){
        case emCH0: chType4TxControl = 0x4; break;
        case emCH1: chType4TxControl = 0x5; break;
        case emCH2: chType4TxControl = 0x6; break;
        case emCH3: chType4TxControl = 0x7; break;
        default:
            break;
    }

    return chType4TxControl;
}//}}}

/// Decode Channel number from Channel id (with information in register (HSIFnMMD)
uint32_t HSIF020::DecodeChNumbFromChId(uint32_t ch_id)
{//{{{
    uint32_t ch_id_tmp = 0x0;
    if (mHSIF020_Func->GetTableSetting()){
        ///Code Table I (Ch.0 = 000 B , Ch.1 = 001 B , Ch.2 = 010 B , Ch.3 = 011 B )
        switch (ch_id){
             case emCH0: ch_id_tmp = 0x0; break;
             case emCH1: ch_id_tmp = 0x1; break;
             case emCH2: ch_id_tmp = 0x2; break;
             case emCH3: ch_id_tmp = 0x3; break;
             default:
                 break;
        }           
    } else {
        ///Code Table  II (Ch.0 = 100 B , Ch.1 = 101 B , Ch.2 = 110 B , Ch.3 = 111 B )
        switch(ch_id){
            case emCH0: ch_id_tmp = 0x4; break;
            case emCH1: ch_id_tmp = 0x5; break;
            case emCH2: ch_id_tmp = 0x6; break;
            case emCH3: ch_id_tmp = 0x7; break;
            default:
                break;
        }
    }
    return ch_id_tmp;
}//}}}

/// Decode channel ID from logic channel type (data from input control signal)
bool HSIF020::DecodeLogicChType2ChId(uint8_t iLogicChType, ChId_t& ch_id)
{//{{{
    ///    0100  Data Channel A
    ///    0101  Data Channel B
    ///    0110  Data Channel C
    ///    0111  Data Channel D
    bool temp = true;
    switch(iLogicChType){
        case 0x4: { ch_id = emCH0; break;}
        case 0x5: { ch_id = emCH1; break;}
        case 0x6: { ch_id = emCH2; break;}
        case 0x7: { ch_id = emCH3; break;}
        default:  { temp = false;  break;}
    }
    return temp;
}//}}}

/// Notify start transfer AXI transaction to bus
void HSIF020::NotifyAxiTransferTransaction(void)
{//{{{
    mNotifyAxiTransferTransactionEvent.notify(CalculateClockEdge("ACLK"));
}//}}}

/// Issue AXI Transaction Process
void HSIF020::TransProcessThread(void)
{//{{{
    ///Send the data frame
    for(;;) {
        if(mIsResetHardActive[emcresetn] || mIsResetCmdActive[emcresetn] || (!CheckClockPeriod("CCLK"))) {
            wait();
        }
        try {
            while (true) {
                re_printf("info","Issuing transaction to bus ...\n");
                mHSIF020_Func->TransProcess();
                wait(mResetCmdEvent[emcresetn] | mResetHardEvent[emcresetn] | mClkZeroEvent[emCclk] | mNotifyAxiTransferTransactionEvent);

                if (mIsResetHardActive[emcresetn] || mIsResetCmdActive[emcresetn] || (!CheckClockPeriod("CCLK"))) {
                    throw vpcl::emRstException;
                }
            }
        }
        catch (vpcl::eResetException) {
        }
    }

}//}}}

///Assert reset by software
void HSIF020::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "ARESETn") {
        if ((!mIsResetHardActive[emARESETn])&&(!mIsResetCmdReceive[emARESETn])) {
            mIsResetCmdReceive[emARESETn] = true;
            re_printf("info","The model will be reset (ARESETn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emARESETn].notify(start_time, SC_NS);
            mResetCmdPeriod[emARESETn] = period;
        } else {
            re_printf("warning","The software reset of ARESETn is called in the reset operation of the model. So it is ignored\n");
        }
    } else if (reset_name == "creset_n") {
        if ((!mIsResetHardActive[emcresetn])&&(!mIsResetCmdReceive[emcresetn])) {
            mIsResetCmdReceive[emcresetn] = true;
            re_printf("info","The model will be reset (creset_n) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent[emcresetn].notify(start_time, SC_NS);
            mResetCmdPeriod[emcresetn] = period;
        } else {
            re_printf("warning","The software reset of creset_n is called in the reset operation of the model. So it is ignored\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be ARESETn or creset_n\n",reset_name.c_str());
    }
}//}}}

///Set clock value and clock unit
void HSIF020::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CCLK") {
            mCCLKOrgFreq = freq;
            ConvertClockFreq (mCCLKFreq, mCCLKFreqUnit, freq, unit);
            if (mCCLKFreq > 0) {
                mCCLKPeriod = (sc_dt::uint64)(((1/(double)mCCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mCCLKSyncTPoint = sc_time_stamp().to_double();
            } else {
                mCCLKPeriod = 0;
                mCCLKSyncTPoint = sc_time_stamp().to_double();
                // Cancel events
                CancelEventsZeroClock();
                // Notify clock is zero
                mClkZeroEvent[emCclk].notify(SC_ZERO_TIME);
            }
        } else if (word_vector[0] == "ACLK") {
            mACLKOrgFreq = freq;
            ConvertClockFreq (mACLKFreq, mACLKFreqUnit, freq, unit);
            if (mACLKFreq > 0) {
                mACLKPeriod = (sc_dt::uint64)(((1/(double)mACLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mACLKSyncTPoint = sc_time_stamp().to_double();
            } else {
                mACLKPeriod = 0;
                mACLKSyncTPoint = sc_time_stamp().to_double();
                // Cancel events
                CancelEventsZeroClock();
                // Notify clock is zero
                mClkZeroEvent[emAclk].notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

///Get clock value
void HSIF020::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "CCLK") {
            re_printf("info","CCLK frequency is %0.0f %s\n", (double)mCCLKOrgFreq, mCCLKFreqUnit.c_str());
        } else if (word_vector[0] == "ACLK") {
            re_printf("info","ACLK frequency is %0.0f %s\n", (double)mACLKOrgFreq, mACLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}


///Force value to register
void HSIF020::ForceRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name, const uint32_t reg_value)
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
            RegisterHandlerWrapper(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Release forced value after forcing registers
void HSIF020::ReleaseRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            RegisterHandlerWrapper(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Write value to registers by software
void HSIF020::WriteRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name, const uint32_t reg_value)
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
            RegisterHandlerWrapper(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

///Read value of register by software
void HSIF020::ReadRegister (const bool is_com_reg, const uint32_t ch_id, const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            RegisterHandlerWrapper(is_com_reg, ch_id, cmd);
        } else {
            re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid\n", reg_name.c_str());
    }
}//}}}

/// Wrapper for accessing registers
void HSIF020::RegisterHandlerWrapper (const bool is_com_reg, const uint32_t ch_id, const std::vector<std::string> cmd)
{//{{{
    if (is_com_reg) {
        mHSIF020_Func->RegisterHandler(cmd);
    // } else if (ch_id <= emCH3){
    } else if (ch_id <= emCH3){
        mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->RegisterHandler(cmd);
    } else {
        re_printf("warning","Channel id (%d) is invalid\n", ch_id);
    }
}//}}}

///List all registers name
void HSIF020::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    mHSIF020_Func->RegisterHandler(cmd);
    for (uint32_t i = 0; i < emCHNum; i++) {
        mHSIF020_Unit[i]->mHSIF020_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

///Dump help message of all parameters or commands
void HSIF020::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"HSIF020_MessageLevel (fatal|error|warning|info)      Set debug message level (Default: fatal|error).");
            SC_REPORT_INFO(this->basename(),"HSIF020_DumpRegisterRW (true/false)                  Enable/disable dumping access register (Default: false).");
            SC_REPORT_INFO(this->basename(),"HSIF020_DumpInterrupt (true/false)                   Enable/disable dumping interrupt information (Default: false).");
            SC_REPORT_INFO(this->basename(),"HSIF020_EnableRegisterMessage (true/false)       Enable/disable info/warning/error message of register IF (Default: true)");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"HSIF020_AssertReset (reset_name, start_time, period) Assert and de-assert reset signal.");
            SC_REPORT_INFO(this->basename(),"HSIF020_SetCLKFreq (clock_name, freq, unit)          Set clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"HSIF020_GetCLKFreq (clock_name)                      Get clock frequency value of model.");
            SC_REPORT_INFO(this->basename(),"HSIF020_ForceRegister (reg_name, reg_value)          Force HSIF020 common register with setting value.");
            SC_REPORT_INFO(this->basename(),"HSIF020_ReleaseRegister (reg_name)                   Release HSIF020 common register from force value.");
            SC_REPORT_INFO(this->basename(),"HSIF020_WriteRegister (reg_name, reg_value)          Write a value to a HSIF020 common register.");
            SC_REPORT_INFO(this->basename(),"HSIF020_ReadRegister (reg_name)                      Read a value from a HSIF020 common register.");
            SC_REPORT_INFO(this->basename(),"HSIF020_ListRegister ()                              Dump name of model's registers.");
            SC_REPORT_INFO(this->basename(),"HSIF020_SetModelID (this_mode_id, icum_id)           Set SPID of this model (Default is 19) and ICU-M model (Default is 26).");
            SC_REPORT_INFO(this->basename(),"HSIF020_SetJtagID (jtag_id)                          Set Jtag ID of this model (Default is 0xCAFEBABE).");
            SC_REPORT_INFO(this->basename(),"HSIF020_IssueError (error_code)                      Issue error to this model when model is operating (TID - AXI - CRC error).");
            SC_REPORT_INFO(this->basename(),"HSIF020_ConfigAuthenIdBase (true/false,id0,...,id7)  Enable/Disable Authentication ID base and set authentication key.");
            SC_REPORT_INFO(this->basename(),"HSIF020_ConfigAuthenCR (true/false)                  Enable/Disable Authentication Challenge and Response.");
            SC_REPORT_INFO(this->basename(),"HSIF020_UNIT_ForceRegister (ch_num, reg_name, value) Force HSIF020 units specific register with setting value.");
            SC_REPORT_INFO(this->basename(),"HSIF020_UNIT_ReleaseRegister (ch_num, reg_name)      Release HSIF020 units specific register from force value.");
            SC_REPORT_INFO(this->basename(),"HSIF020_UNIT_WriteRegister (ch_num, reg_name, value) Write a value to HSIF020 units specific register.");
            SC_REPORT_INFO(this->basename(),"HSIF020_UNIT_ReadRegister (ch_num, reg_name)         Read value from HSIF020 units specific register.");
        } else {
            re_printf("warning","The name (%s) of HSIF020_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of HSIF020_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

///Set message level (fatal, error, warning, info)
void HSIF020::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }
    for (uint32_t i = 0; i < emCHNum; i++) {
        std::string msg_ret = mHSIF020_Unit[i]->handleCommand(cmd);
        if (msg_ret != "") {
            SC_REPORT_INFO(mHSIF020_Unit[i]->basename(), msg_ret.c_str());
        }
    }
    if (true == mEnableRegisterMessage) {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mHSIF020_Func->RegisterHandler(cmd);
        for (uint32_t i = 0; i < emCHNum; i++) {
            mHSIF020_Unit[i]->mHSIF020_UNIT_Func->RegisterHandler(cmd);
        }

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);

        mHSIF020_Func->RegisterHandler(cmd);
        for (uint32_t i = 0; i < emCHNum; i++) {
            mHSIF020_Unit[i]->mHSIF020_UNIT_Func->RegisterHandler(cmd);
        }
    } else {
        //Set message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mHSIF020_Func->RegisterHandler(cmd);
        for (uint32_t i = 0; i < emCHNum; i++) {
            mHSIF020_Unit[i]->mHSIF020_UNIT_Func->RegisterHandler(cmd);
        }

        //Set message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");

        mHSIF020_Func->RegisterHandler(cmd);
        for (uint32_t i = 0; i < emCHNum; i++) {
            mHSIF020_Unit[i]->mHSIF020_UNIT_Func->RegisterHandler(cmd);
        }
    }
}//}}}


void HSIF020::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        } else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        } else {
            re_printf("warning","Invalid argument: HSIF020_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0) {
            std::string intr_name = "false";
            if (mEnableRegisterMessage) {
                intr_name = "true";
            } 
            re_printf("info","HSIF020_EnableRegisterMessage %s\n", intr_name.c_str());
    } else {
        re_printf("warning","Invalid argument: HSIF020_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

///Enable/disable dumping message when users access registers
void HSIF020::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    mHSIF020_Func->RegisterHandler(cmd);
    for (uint32_t i = 0; i < emCHNum; i++) {
        mHSIF020_Unit[i]->mHSIF020_UNIT_Func->RegisterHandler(cmd);
    }
}//}}}

///Enable/disable dumping interrupt information
void HSIF020::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: HSIF020_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = "false";
        if(mDumpInterrupt) {
            temp = "true";
        }
        re_printf("info","HSIF020_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: HSIF020_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

/// Overwrite CalculateClockEdge (const std::string clock_name, const bool is_pos, const double time_point)
sc_time HSIF020::CalculateClockEdge (const std::string clock_name)
{
    double current_time = sc_time_stamp().to_double();
    double mCLKPeriod = 0;
    double mSyncTPoint = 0; 
    if (clock_name == "CCLK") {
        mCLKPeriod = (double)mCCLKPeriod;
        mSyncTPoint = mCCLKSyncTPoint;
    } else { // (clock_name == "ACLK")
        mCLKPeriod = (double)mACLKPeriod;
        mSyncTPoint = mACLKSyncTPoint;
    } 
    
    return sc_time(CalculateCLKPosEdge((double)mCLKPeriod, false, current_time, mSyncTPoint), mTimeResolutionUnit);
}

/// Check clock period value
bool HSIF020::CheckClockPeriod (const std::string clock_name)
{//{{{
    bool clock_status = false;
    sc_dt::uint64 clock_period = 0;
    clock_period = mACLKPeriod;
    if(clock_name == "CCLK") {
        clock_period = mCCLKPeriod;
    }
    if (clock_period > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","%s frequency is zero\n", clock_name.c_str());
    }

    return clock_status;
}//}}}

bool HSIF020::CheckResetStatus ()
{//{{{
    return (mIsResetHardActive[emcresetn] || mIsResetCmdActive[emcresetn]);
}//}}}

/// Dump status information
void HSIF020::DumpStatusInfo(void)
{//{{{
    std::string type = "StatInfo";

    printf("PROFILE(%s): HSIF020: Info [%20s] (%10s):\n", type.c_str(), sc_time_stamp().to_string().c_str(), this->name());
    printf("PROFILE(%s): HSIF020:   HSIFnMST  bits: 0x%02X\n", type.c_str(),   mHSIF020_Func->GetHSIFnMST() );
    printf("PROFILE(%s): HSIF020:   HSIFnMIST bits: 0x%02X\n", type.c_str(),   mHSIF020_Func->GetHSIFnMIST());
    printf("PROFILE(%s): HSIF020:   HSIFnSTST bits: 0x%02X\n", type.c_str(),   mHSIF020_Func->GetHSIFnSTST());
    printf("PROFILE(%s): HSIF020:   HSIFnSRST bits: 0x%02X\n", type.c_str(),   mHSIF020_Func->GetHSIFnSRST());
    printf("PROFILE(%s): HSIF020:   HSIFnAEST bits: 0x%02X\n", type.c_str(),   mHSIF020_Func->GetHSIFnAEST());
    for (int index = 0; index < emCHNum; index ++){
        printf("PROFILE(%s): HSIF020:   HSIFnCST%d bits: 0x%02X\n", type.c_str(), index, mHSIF020_Unit[index]->GetHSIFnCST());
    }

}//}}}

/// Set This model PEID 
void HSIF020::SetBusMasterID(uint32_t thisIp_peid)
{//{{{
    if (thisIp_peid > 31){
        re_printf("warning","Can not set PEID of this model is over than 31. Model still keep current ID value!");
    } else {
        mPEID = thisIp_peid;
        mHSIF020_Func->SetModelPeid((uint8_t)thisIp_peid);
    }

}//}}}

/// Set This model SPID 
void HSIF020::SetModelID(uint32_t thisIp_spid)
{//{{{
    if (thisIp_spid > 31){
        re_printf("warning","Can not set SPID of this model is over than 31. Model still keep current ID value!");
    } else {
        mThisIP_SPID = thisIp_spid;
        mHSIF020_Func->SetModelSpid((uint8_t)thisIp_spid);
    }
}//}}}

/// Handle CCLK signal
void HSIF020::HandleCCLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = CCLK.read();
    SetCLKFreq("CCLK", freq_value, "Hz");
}//}}}

/// Handle ACLK signal
void HSIF020::HandleACLKSignalMethod (void)
{//{{{
    sc_dt::uint64 freq_value = ACLK.read();
    SetCLKFreq("ACLK", freq_value, "Hz");
}//}}}

/// Handle ARESETn signal
void HSIF020::HandleARESETnSignalMethod (void)
{//{{{
    mResetCurVal[emARESETn] = ARESETn.read();
    if (mResetCurVal[emARESETn] != mResetPreVal[emARESETn]) {       //mResetPreVal is "true" at initial state
        mResetPreVal[emARESETn] = mResetCurVal[emARESETn];
        // Reset signals are asynchronous
        mResetHardEvent[emARESETn].notify(SC_ZERO_TIME);
    }
}//}}}

/// Handle creset_n signal
void HSIF020::Handlecreset_nSignalMethod (void)
{//{{{
    mResetCurVal[emcresetn] = creset_n.read();
    if (mResetCurVal[emcresetn] != mResetPreVal[emcresetn]) {   //mResetPreVal is "true" at initial state
        mResetPreVal[emcresetn] = mResetCurVal[emcresetn];
        // Reset signals are asynchronous
        mResetHardEvent[emcresetn].notify(SC_ZERO_TIME);
    }
}//}}}

/// Process reset function when reset port is active
void HSIF020::HandleResetHardMethod (const uint32_t reset_id)
{//{{{
    if (mResetCurVal[reset_id] == vpcl::emResetActive) {
        mIsResetHardActive[reset_id] = true;
        if (reset_id == emARESETn) {
            re_printf("info","The reset port ARESETn is asserted.\n");
            this->EnableReset("ARESETn", mIsResetHardActive[reset_id]);
        } else {
            re_printf("info","The reset port creset_n is asserted.\n");
            this->EnableReset("creset_n", mIsResetHardActive[reset_id]);
        }
    } else {
            mIsResetHardActive[reset_id] = false;
            if (reset_id == emARESETn) {
                re_printf("info","The reset port ARESETn is de-asserted.\n");
                this->EnableReset("ARESETn", mIsResetHardActive[reset_id]);
            } else {
                re_printf("info","The reset port creset_n is de-asserted.\n");
                this->EnableReset("creset_n", mIsResetHardActive[reset_id]);
            }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive[reset_id]) {
        mResetCmdEvent[reset_id].cancel();
        mResetCmdCancelEvent[reset_id].cancel();
        mIsResetCmdReceive[reset_id] = false;
        mIsResetCmdActive[reset_id] = false;
    }
}//}}}

/// Process reset function when reset command is active
void HSIF020::HandleResetCmdMethod (const uint32_t reset_id)
{//{{{
    std::string reset_name = "ARESETn";
    if (reset_id == 0) {
        reset_name = "ARESETn";
    } else { //reset_id == 1
        reset_name = "creset_n";
    }
    mIsResetCmdActive[reset_id] = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
    mResetCmdCancelEvent[reset_id].notify(mResetCmdPeriod[reset_id], SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void HSIF020::CancelResetCmdMethod (const uint32_t reset_id)
{//{{{
    std::string reset_name = "ARESETn";
    if (reset_id == 0) {
        reset_name = "ARESETn";
    } else { //reset_id == 1
        reset_name = "creset_n";
    }
    mIsResetCmdActive[reset_id] = false;
    mIsResetCmdReceive[reset_id] = false;
    re_printf("info","Reset period of %s is over.\n", reset_name.c_str());
    this->EnableReset(reset_name, mIsResetCmdActive[reset_id]);
}//}}}

/// Execute reset operation
void HSIF020::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    if (reset_name == "ARESETn") {
        mHSIF020_Func->Reset(is_active);
        for (uint32_t i = 0; i < emCHNum; i++) {
            mHSIF020_Unit[i]->mHSIF020_UNIT_Func->Reset(is_active);
        }
    }
    /// Initialize model
    if (is_active) {
        //Initialize variables
        Initialize();
        // Cancel events
        CancelEventsReset();     /// Contain initialize output port
    }
}//}}}

/// Virtual function of HSIF020_AgentController
/// Notify Get data from memory to Fifo
void HSIF020::TxStreamNotifyGetData2Fifo(void)
{
    mTxStreamGetData2FifoEvent.notify(CalculateClockEdge("ACLK"));
}//}}}

/// Notify Rx Write data to memory
void HSIF020::RxStreamNotifyWriteData2Mem(void)
{
    mRxStreamWriteData2MemEvent.notify(CalculateClockEdge("ACLK"));
}//}}}

/// Tx Get data to FiFO for start transfer to Rx side
void HSIF020::TxStreamGetData2FifoMethod(void)
{
    mHSIF020_Func->TxStreamGetData2Fifo();
}//}}}

/// Rx write data to memory when received data from Tx
void HSIF020::RxStreamWriteData2MemMethod(void)
{
    mHSIF020_Func->RxStreamWriteData2Mem();
}//}}}

/// Notify method to continue get data from memory to FIFO
void HSIF020::TxContinueGetData2Fifo(void)
{
    mTxStreamGetData2FifoEvent.notify(CalculateClockEdge("ACLK"));
}//}}}

/// Notify method to continue Write data to memory
void HSIF020::RxContinueWriteData2Mem(void)
{
    mRxStreamWriteData2MemEvent.notify(CalculateClockEdge("ACLK"));
}//}}}

/// Get number of outstanding stream command
uint8_t HSIF020::GetTxStreamOutstandingNumb(void)
{
    return mTxStreamOutstand;
}//}}}

/// Issue Request to Arbiter transfer stream command method
void HSIF020::TxTransferStreamCmdMethod(void)
{
    if (!CheckClockPeriod("CCLK")){
        return;
    }
    if ( (mTxStreamOutstand < 2)
      && (!isDurTransStreamCmd())
      && mHSIF020_Unit[emStreamChannel]->IsChTxEnable()){
        mL2FrameOut.Initialize();
        mL2FrameOut.SetCommand((uint8_t)emStreamCmd);
        NotifyCmdtoL2Arbiter(emStreamChannel);
    }
}//}}}

/// Notify to start transfer a stream command
void HSIF020::NotifyTxTransStreamCmd(void)
{
    mTxTransferStreamCmdEvent.notify(CalculateClockEdge("CCLK"));
}//}}}

///Get this IP Spid value
uint8_t HSIF020::GetThisIpSpidInfo (void)
{
    return mThisIP_SPID;
}//}}}

/// Check is AXI error or not
bool HSIF020::IsAxiError(void)
{
    /// Get AXI error status
    bool temp = mIsAxiError;
    /// Each time issue AXI is effective one time.
    /// Specification:
    /// Tx stream: If an error (including timeout) is caused by a reply command during transmission, L2 collects all the responses to
    /// transmitted H-Bus requests and transmitted request commands, sets the RHSIFnSTST.STE1 or RHSIFnSTST.STE2 bit, and then stops the DMAC.
    if (mHSIF020_Func->GetTxAXIOutstandReq() == 0x1){
        if (mIsAxiError){
            mIsAxiError = false;
        }
    }
    if (mHSIF020_Func->GetRxAXIOutstandReq() == 0x1){
        if (mIsAxiError){
            mIsAxiError = false;
        }
    }
    return temp;
}//}}}

/// Stop time-out event when received reply command
void HSIF020::TxStreamStopTimeOutEvent(void)
{
    mTxStreamTimeOutEvent.cancel();
}//}}}

/// Stop time-out event when received reply command
void HSIF020::TxNonStreamStopTimeOutEvent(uint8_t ch_id)
{
    mTxNonStreamTimeOutEvent[ch_id].cancel();
}//}}}

/// Set model to initial setting phase
void HSIF020::SetInitialSettingPhase(bool isInitialSettingPhase)
{
    mIsInitialSetting = isInitialSettingPhase;
}//}}}

/// check model is in initial setting phase or not
bool HSIF020::isInitialSetting(void)
{
    //todo: Initial seting: between reset + start operation (Transmit/Reception)
    return mIsInitialSetting;
}//}}}

/// Check model is in during transfer stream command or not
bool HSIF020::isDurTransStreamCmd(void)
{
    return mIsDurTransStreamCmd;
}//}}}

/// Set authentication cr base status
void HSIF020::SetAuthenCRStatus(bool isPass)
{
    if (mAuthenCREnable){
        mAuthenCRPass = isPass;
        mWriteHsif_authent2_passEvent.notify(CalculateClockEdge("ACLK"));
    } 
}//}}}

/// Set authentication id base status
void HSIF020::SetAuthenIdStatus(bool isPass)
{
    if (mAuthenIdBaseEnable){
        mAuthenIdBasePass = isPass;
        mWriteHsif_authent1_passEvent.notify(CalculateClockEdge("ACLK"));
    } 
}//}}}

/// Set authentication CR lock
void HSIF020::SetAuthenCRLock(void)
{
    mAuthenCRLock = true;
}//}}}

/// Set authentication ID base lock
void HSIF020::SetAuthenIdLock(void)
{
    mAuthenIdBaseLock = true;
}//}}}

///< Set authentication id base enable/disable
void HSIF020::SetAuthenIdBaseEnable(const std::string isEnable, uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3
                                               , uint32_t id4, uint32_t id5, uint32_t id6, uint32_t id7)
{
    if (isEnable == "true") {
        re_printf("info","Enable authentication ID base.");
        mAuthenIdBaseEnable = true;
        mHSIF020_Func->SetAuthenIdBaseKey(id0, id1, id2, id3, id4, id5, id6, id7);
    } else {
        re_printf("info","Disable authentication ID base.");
        mAuthenIdBaseEnable = false;
        mHSIF020_Func->SetAuthenIdBaseKey(id0, id1, id2, id3, id4, id5, id6, id7);
    }
}//}}}

///< Set authentication CR base enable/disable
void HSIF020::SetAuthenCREnable(const std::string isEnable)
{
    if (isEnable == "true") {
        re_printf("info","Enable authentication Challenge and Response.");
        mAuthenCREnable = true;
    } else {
        re_printf("info","Disable authentication Challenge and Response.");
        mAuthenCREnable = false;
    }
}//}}}

/// Method to handle authentication pass port
void HSIF020::WriteHsif_authent1_passMethod(void)
{
    hsif_authent1_pass.write(mAuthenIdBasePass);
    if (mAuthenIdBasePass && mAuthenIdBaseEnable){
        re_printf("info","Authentication ID base is Passed.");
    } else {
        re_printf("info","Authentication ID base is not Passed.");
    }
}//}}}

void HSIF020::WriteHsif_authent2_passMethod(void)
{
    hsif_authent2_pass.write(mAuthenCRPass);
    if (mAuthenCRPass && mAuthenCREnable){
        re_printf("info","Authentication Challenge and Response is Passed.");
    } else {
        re_printf("info","Authentication Challenge and Response is not Passed.");
    }
}//}}}

///Method to handle interrupt
void HSIF020::IssueInterrupt(std::string interrupt_name, ChId_t ch_id, bool type)
{
    if (type == true){
        if (interrupt_name == "int_hsif_ch"){
            mAssertInt_hsif_chEvent[ch_id].notify(CalculateClockEdge("ACLK"));
        } else if (interrupt_name == "int_hsif_str"){
            mAssertInt_hsif_strEvent.notify(CalculateClockEdge("ACLK"));
        } else if (interrupt_name == "int_hsif_err"){
            mAssertInt_hsif_errEvent.notify(CalculateClockEdge("ACLK"));
        } else { // interrupt_name == "int_hsif_sec"
            mAssertInt_hsif_secEvent.notify(CalculateClockEdge("ACLK"));
        }
    }
    else {
        if (interrupt_name == "int_hsif_ch"){
            mDeAssertInt_hsif_chEvent[ch_id].notify(CalculateClockEdge("ACLK"));
        } else if (interrupt_name == "int_hsif_str"){
            mDeAssertInt_hsif_strEvent.notify(CalculateClockEdge("ACLK"));
        } else if (interrupt_name == "int_hsif_err"){
            mDeAssertInt_hsif_errEvent.notify(CalculateClockEdge("ACLK"));
        } else { // interrupt_name == "int_hsif_sec"
            mDeAssertInt_hsif_secEvent.notify(CalculateClockEdge("ACLK"));
        }
    }
}//}}}

void HSIF020::WriteInt_hsif_chMethod(ChId_t ch_id)
{
    int_hsif_ch[ch_id]->write(mInt_hsif_ch[ch_id]);
    DumpInterruptMsg(int_hsif_ch[ch_id]->basename(),mInt_hsif_ch[ch_id]);
}//}}}

void HSIF020::AssertInt_hsif_chMethod(ChId_t ch_id)
{
    mInt_hsif_ch[ch_id] = true;
    mWriteInt_hsif_chEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

void HSIF020::DeAssertInt_hsif_chMethod(ChId_t ch_id)
{
    mInt_hsif_ch[ch_id] = false;
    mWriteInt_hsif_chEvent[ch_id].notify(SC_ZERO_TIME);
}//}}}

void HSIF020::WriteInt_hsif_strMethod(void)
{
    int_hsif_str->write(mInt_hsif_str);
    DumpInterruptMsg(int_hsif_str.basename(),mInt_hsif_str);
}//}}}

void HSIF020::AssertInt_hsif_strMethod(void)
{
    mInt_hsif_str = true;
    mWriteInt_hsif_strEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIF020::DeAssertInt_hsif_strMethod(void)
{
    mInt_hsif_str = false;
    mWriteInt_hsif_strEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIF020::WriteInt_hsif_secMethod(void)
{
    int_hsif_sec->write(mInt_hsif_sec);
    DumpInterruptMsg(int_hsif_sec.basename(),mInt_hsif_sec);
}//}}}

void HSIF020::AssertInt_hsif_secMethod(void)
{
    mInt_hsif_sec = true;
    mWriteInt_hsif_secEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIF020::DeAssertInt_hsif_secMethod(void)
{
    mInt_hsif_sec = false;
    mWriteInt_hsif_secEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIF020::WriteInt_hsif_errMethod(void)
{
    int_hsif_err->write(mInt_hsif_err);
    DumpInterruptMsg(int_hsif_err.basename(),mInt_hsif_err);
}//}}}

void HSIF020::AssertInt_hsif_errMethod(void)
{
    mInt_hsif_err = true;
    mWriteInt_hsif_errEvent.notify(SC_ZERO_TIME);
}//}}}

void HSIF020::DeAssertInt_hsif_errMethod(void)
{
    mInt_hsif_err = false;
    mWriteInt_hsif_errEvent.notify(SC_ZERO_TIME);
}//}}}

/// Dump Interrupt Msg
void HSIF020::DumpInterruptMsg (const std::string intr_name, const bool value)
{
    if (mDumpInterrupt) {
         if (value) {
             re_printf("info","INT [%s] Assert.\n",intr_name.c_str());
         } else {
             re_printf("info","INT [%s] De-assert.\n",intr_name.c_str());
         }
    }
}//}}}

// vim600: set foldmethod=marker :
