// ---------------------------------------------------------------------
// $Id: HSIF020_Func.cpp,v 1.13 2020/04/08 07:39:39 quynhnhunguyen Exp $
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
#include "HSIF020_Func.h"
#include "HSIF020_AgentController.h"
#include "HSIF020_UNIT.h"
#include "HSIF020_UNIT_Func.h"

///Constructor of HSIF020_Func class
HSIF020_Func::HSIF020_Func (std::string iname
                          , HSIF020_AgentController *iHSIF020_AgentControllerPtr
                          , BusMasterIf<64> *MasterIf_is_ptr
                          , SIM_MODE_T simMode):
  Chsif020_regif(iname, 64)
, BusSlaveFuncIf()
, BusMasterFuncIf()
, mMasterIf_is(MasterIf_is_ptr)
{//{{{
    assert(iHSIF020_AgentControllerPtr != NULL);
    mHSIF020_AgentController = iHSIF020_AgentControllerPtr;
    mHSIF020_AgentController->CommandInit(iname);
    Chsif020_regif::set_instance_name(iname);
    mModuleName     = iname;
    mSimMode        = simMode;

    ///Initialize window label
    mWindowLabel[0] = "Window A";
    mWindowLabel[1] = "Window B";
    mWindowLabel[2] = "Window C";
    mWindowLabel[3] = "Window D";

    for (uint8_t index = 0; index < emAuthIDbaseRegNumbs; index++){
        /// offset RHSIFnAID0-7
        mRegsAuthenAddrSet.insert(0x300 + 0x8 * index);
    }
    for (uint8_t index = 0; index < emAuthCRRegNumbs; index++){
        /// offset RHSIFnAAD0-4
        mRegsAuthenAddrSet.insert(0x340 + 0x8 * index);
        /// offset RHSIFnARD0-4
        mRegsAuthenAddrSet.insert(0x360 + 0x8 * index);
    }
    SetAuthenIdBaseKey(0, 0, 0, 0, 0, 0, 0, 0);
    t               = SC_ZERO_TIME;
    spid            = HSIF0MASTERID;
    um              = false;
    vm              = false;
    vcid            = 0x0;
    peid            = HSIF0MASTERID;
    tcid            = 0x0;

    /// Initialize
    Initialize();
}//}}}

///Destructor of HSIF020_Func class
HSIF020_Func::~HSIF020_Func (void)
{//{{{
}//}}}

/// Initialize all internal variable
void HSIF020_Func::Initialize(void)
{//{{{
    ///Initialize buffer
    memset(mTxStreamData,0,HSIF020_ST_DATA_BUF_SIZE);
    memset(mRxStreamData,0,HSIF020_ST_DATA_BUF_SIZE);

    /// Initialize member attribute
    mTransInfoSPID          = 0;
    mSecure                 = false;
    mAXITransId             = 0;
    mTimeOfAuthenIdBase     = 0;
    mTimeOfAuthenCR         = 0;
    mTxAXIOutstandReq       = 0;
    mRxAXIOutstandReq       = 0;
    mTxFifoStatus           = 0;
    mTxFifoReadPointer      = 0;
    mTxFifoWritePointer     = 0;
    mRxFifoStatus           = 0;
    mRxFifoReadPointer      = 0;
    mRxStreamCurrentFrameID = 0;
    mRxFifoWritePointer     = 0;
    mTxMemReadPointer       = 0;
    mTxByteNumMaxReadFromMem= 0;
    mTxSumDataReadFromMem   = 0;
    mRxSumDataWriteToMem    = 0;
    mRxMemWritePointer      = 0;
    mRxMemReadPointer       = 0;
    mRxSizeUnprocessedData  = 0;
    mRxMaxSizeUnprocessedData = 0;
    mRxFrameDataSize        = 32;
    mTxFrameDataSize        = 32;
    mRxStreamInputNumb      = 0;

    mRxMemWritePointerToggleBit = false;
    mRxMemReadPointerToggleBit = false;
    mStreamIgnoreReplyCmdAfterTimeOut = false;
    while(!mRequestFifo.empty()){
        mRequestFifo.pop();
    }
}//}}}

/// Set this IP PEID value
void HSIF020_Func::SetModelPeid(uint8_t iPeid)
{//{{{
    peid = iPeid;
}//}}}
/// Set this IP SPID value
void HSIF020_Func::SetModelSpid(uint8_t iSpid)
{//{{{
    spid = iSpid;
}//}}}

///Reset operation of HSIF020_Func class
void HSIF020_Func::Reset (const bool is_active)
{//{{{
    Chsif020_regif::EnableReset(is_active);
}//}}}

///Register handler command to Chsif020_regif class
void HSIF020_Func::RegisterHandler (const std::vector<std::string> &args)
{//{{{
    std::string msg_return = "";
    if (args.size() == 1) {//Dump registers name
        vpcl::re_register *reg_p = first_reg_object();
        while (reg_p != NULL) {
            SC_REPORT_INFO(mModuleName.c_str(), reg_p->name().c_str());
            reg_p = next_reg_object();
        }
    } else {
        if (args[1] == "*") {//Set message level for each register
            std::vector<std::string> cmd;
            vpcl::re_register *reg_p = first_reg_object();
            while (reg_p != NULL) {
                cmd = args;
                cmd.erase(cmd.begin()+1);
                cmd.insert(cmd.begin()+1, reg_p->name());
                msg_return = Chsif020_regif::reg_handle_command(cmd);
                if (msg_return != "") {
                    SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
                }
                cmd.clear();
                reg_p = next_reg_object();
            }
        } else {//Set handle command to Chsif020_regif class
            msg_return = Chsif020_regif::reg_handle_command(args);
            if (msg_return != "") {
                SC_REPORT_INFO(mModuleName.c_str(), msg_return.c_str());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HSIF020_Func::read (uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    assert(t != NULL);
    bool isAccess = false;
    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension* ext = (TlmG3mExtension*)0;
    trans.get_extension(ext);
    
    memset(trans.get_data_ptr(), 0, trans.get_data_length());
    // Get extension info
    if ((false == mHSIF020_AgentController->CheckClockPeriod("ACLK")) || (true == mHSIF020_AgentController->CheckResetStatus())) {
        // Software debugger
        if ((ext != NULL) && ((true == debug) && (DEBUG_MASTER_BMID == ext->getPeId()))) {
            isAccess = true;
        } else {
            re_printf("warning", "Cannot access register when clock is 0 or reset port is asserted.\n");
        }
    } else {
        isAccess = true;
    }

    if (isAccess == true) {    
        uint32_t addr = (uint32_t)trans.get_address() & 0xFFF;
        //Clear data in read transaction
        memset(trans.get_data_ptr(), 0, trans.get_data_length());

        if ((0x400 <= addr) && (addr <= 0x47F)) {
            // Read from HSIF020CH0
            mHSIF020_AgentController->mHSIF020_Unit[emCH0]->mHSIF020_UNIT_Func->read(offsetAddress, trans, t, debug);
        } else if ((0x480 <= addr) && (addr <= 0x4FF)) {
            // Read from HSIF020CH1
            mHSIF020_AgentController->mHSIF020_Unit[emCH1]->mHSIF020_UNIT_Func->read(offsetAddress, trans, t, debug);
        } else if ((0x500 <= addr) && (addr <= 0x57F)) {
            // Read from HSIF020CH2
            mHSIF020_AgentController->mHSIF020_Unit[emCH2]->mHSIF020_UNIT_Func->read(offsetAddress, trans, t, debug);
        } else if ((0x580 <= addr) && (addr <= 0x5FF)) {
            // Read from HSIF020CH3
            mHSIF020_AgentController->mHSIF020_Unit[emCH3]->mHSIF020_UNIT_Func->read(offsetAddress, trans, t, debug);
        } else {
            /// Extract SPID information for checking data
            if (ext != NULL) {
                mTransInfoSPID = ext->getSpId();
                mSecure= ext->isSecure();
            } else {
                mSecure = false;
                mTransInfoSPID = 0;
            }
            // Read from: Module common registers/ Tx stream registers/ Rx stream registers/ Security registers
            if (!debug) {
                reg_rd((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_rd_dbg((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            }
        }
    }
}//}}}

///Virtual function of BusSlaveFuncIf class
void HSIF020_Func::write (uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    assert(t != NULL);
    bool isAccess = false;
    /// -# get G3m extended information of the receive transaction
    TlmG3mExtension* ext = (TlmG3mExtension*)0;
    trans.get_extension(ext);

    // Zero clock
    if (false == mHSIF020_AgentController->CheckClockPeriod("ACLK")) {
        // Software debugger
        if ((ext != NULL) && ((true == debug) && (DEBUG_MASTER_BMID == ext->getPeId()))) {
            isAccess = true;
        } else {
            re_printf("warning", "Cannot write register when clock is 0.\n");
        }
    } else {
        isAccess = true;
    }

    if (isAccess == true) { 
        uint32_t addr = (uint32_t)trans.get_address() & 0xFFF;

        if ((0x400 <= addr) && (addr <= 0x47F)) {
            // Write to HSIF020CH0
            mHSIF020_AgentController->mHSIF020_Unit[emCH0]->mHSIF020_UNIT_Func->write(offsetAddress, trans, t, debug);
        } else if ((0x480 <= addr) && (addr <= 0x4FF)) {
            // Write to HSIF020CH1
            mHSIF020_AgentController->mHSIF020_Unit[emCH1]->mHSIF020_UNIT_Func->write(offsetAddress, trans, t, debug);
        } else if ((0x500 <= addr) && (addr <= 0x57F)) {
            // Write to HSIF020CH2
            mHSIF020_AgentController->mHSIF020_Unit[emCH2]->mHSIF020_UNIT_Func->write(offsetAddress, trans, t, debug);
        } else if ((0x580 <= addr) && (addr <= 0x5FF)) {
            // Write to HSIF020CH3
            mHSIF020_AgentController->mHSIF020_Unit[emCH3]->mHSIF020_UNIT_Func->write(offsetAddress, trans, t, debug);
        } else {
            /// Extract SPID information for checking data
            if (ext != NULL) {
                mTransInfoSPID = ext->getSpId();
                mSecure= ext->isSecure();
            } else {
                mSecure = false;
                mTransInfoSPID = 0;
            }
            // Write to: Module common registers/ Tx stream registers/ Rx stream registers/ Security registers
            if (!debug) {
                reg_wr((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            } else {
                reg_wr_dbg((uint32_t)trans.get_address(), trans.get_data_ptr(), trans.get_data_length());
            }
        }
    }
}//}}}

///Configure AXI Transaction
void HSIF020_Func::ConfigTransaction(const bool icommand
                        , const uint32_t iaddr
                        , const uint8_t isize
                        , const bool idebug
                        , const ChId_t ch_id)
{//{{{
    // Do not need config extension. use default value or value setting on register or setting by PYTHONIF
    mAXITransId = (uint32_t)((uint64_t)mAXITransId + 1);
    AXITransInfo* pAxiTransInfo = new AXITransInfo(idebug, icommand, isize
                                                 , iaddr, ch_id, mAXITransId);

    ///Arbiter for DMA
    mTransIdInfoSet.insert(TransInfo_t(ch_id,pAxiTransInfo));

    mHSIF020_AgentController->NotifyAxiTransferTransaction();
}//}}}

/// Function processing transaction to AXI bus
void HSIF020_Func::TransProcess(void)
{//{{{
    while (!mTransIdInfoSet.empty()){
        TransIdInfoSet_t::iterator it = mTransIdInfoSet.begin();

        TlmBasicPayload *mPayload   = new TlmBasicPayload;
        TlmG3mExtension *mG3mExt    = new TlmG3mExtension;
        TlmAxiExtension *mAxiExt    = new TlmAxiExtension;

        AXITransInfo* temp = (*it).second;
        mPayload->set_address(temp->mAddr);
        mPayload->set_data_length(temp->mSize);
        if (temp->mCmd != 0x0) {
            mPayload->set_write();
        } else {
            mPayload->set_read();
        }
        if (temp->mSize <= 4){
            mPayload->set_data_ptr(mHSIF020_AgentController->mHSIF020_Unit[(*it).first]->GetDataPointer());
        } else {
            if (mPayload->is_read()){
                mPayload->set_data_ptr(&mTxStreamData[mTxFifoWritePointer]);
                mTxFifoWritePointer += 32;
                if (mTxFifoWritePointer == HSIF020_ST_DATA_BUF_SIZE ) mTxFifoWritePointer = 0;
            } else {
                mPayload->set_data_ptr(&mRxStreamData[mRxFifoReadPointer]);
                mRxFifoReadPointer += mRxFrameDataSize;
                if (mRxFifoReadPointer == HSIF020_ST_DATA_BUF_SIZE ) mRxFifoReadPointer = 0;
            }
        }

        //Extension information
        mG3mExt->setSpId(spid);
        mG3mExt->setVmId(vcid);
        mG3mExt->setPeId(peid);
        mG3mExt->setUserMode(um);
        mG3mExt->setVirtualMode(vm);
        mG3mExt->setTcId(tcid);
        mG3mExt->setSecure(mSecure);
        mAxiExt->setTransId(temp->mAXITransId);

        mPayload->set_extension(mG3mExt);
        mPayload->set_extension(mAxiExt);
        if (mPayload->is_write()) {
            re_printf("info","WRITE transaction is issued to AXI bus at ");
            if(mPayload->get_data_length() > 4){    /// Is operation of Stream function
                mRxAXIOutstandReq += 1;
                mRxFifoStatus -= mRxFrameDataSize;
            }
            /// Non-blocking in write, Not wait completion
            mMasterIf_is->write(*mPayload, false, t, false);

            re_printf("info","    Transaction info: address(0x%8X); size (%d)\n",mPayload->get_address(),temp->mSize);
        } else { // (mPayload->is_read()
            if(mPayload->get_data_length() > 4){
                mTxAXIOutstandReq += 1;
            }
            re_printf("info","READ transaction is issued to AXI bus at ");
            /// Non-blocking in read, Not wait completion
            mMasterIf_is->read(*mPayload, false, t, false);
            if ((mSimMode == SIM_MODE_FAST)) {
                if (mHSIF020_AgentController->IsAxiError()){
                    if(mPayload->get_data_length() > 4) {
                        HandleTxAxiErrorStreamCmd();
                    } else {
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).first]->RxNonStreamHandleAxiError();
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).first]->TgrConfigNACKCommand();
                    }
                } else {
                    ReceiveRD_DATA((TlmTransactionType *)mPayload,(*it).first);
                }
            }
        }

        if ((mSimMode == SIM_MODE_FAST)) {      /// LT mode
            //todo: if error
            if (mPayload->is_write()){
                tlm::tlm_response_status axi_response_status = mPayload->get_response_status();
                if (mHSIF020_AgentController->IsAxiError()) axi_response_status = tlm::TLM_INCOMPLETE_RESPONSE;
                if (axi_response_status == tlm::TLM_OK_RESPONSE){
                    if(mPayload->get_data_length() > 4) {
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).first]->TgrConfigACKStreamCommand(mRxStreamCurrentFrameID);
                        UpdateWritePointer();
                        UpdateCompleteFlag();
                    } else {
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).first]->TgrConfigACKCommand();
                    }
                } else {
                    if(mPayload->get_data_length() > 4) {
                        // Stream command
                        UpdateWritePointer();
                        UpdateCompleteFlag();
                        HandleRxAxiErrorStreamCmd();
                    } else {
                        // Non-stream command
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).first]->RxNonStreamHandleAxiError();
                    }
                    mHSIF020_AgentController->mHSIF020_Unit[(*it).first]->TgrConfigNACKStreamCommand(mRxStreamCurrentFrameID);
                }
                if(mPayload->get_data_length() > 4){
                    mRxAXIOutstandReq -= 1;
                    mRxStreamInputNumb -= 1;
                }
            }
            DeleteTrans((TlmTransactionType *)mPayload);
        } else {
            mRequestFifo.push(mPayload);
            mTransIdInfoMap.insert(std::make_pair((*it).second->mAXITransId,
                       std::pair<ChId_t, FrameId_t>((*it).first,mRxStreamCurrentFrameID)));
        }
        if (mPayload->is_read() && (mPayload->get_data_length() > 4)){
            if ( mTxAXIOutstandReq < 2)
                mHSIF020_AgentController->TxContinueGetData2Fifo();
        }
        if (mPayload->is_write() && (mPayload->get_data_length() > 4)){
            if ( mRxAXIOutstandReq < 2)
                mHSIF020_AgentController->RxContinueWriteData2Mem();
        }

        mTransIdInfoSet.erase(it);              ///Delete Transaction info after processing
        delete temp;
    }
}

/// Handle Data reply from AXI bus
void HSIF020_Func::ReceiveRD_DATA(TlmTransactionType *trans, ChId_t ch_id)
{//{{{
    assert(trans != NULL);
    uint8_t *data_ptr = NULL;
    data_ptr = trans->get_data_ptr();
    assert(data_ptr != NULL);
    uint32_t data_length = trans->get_data_length();

    if (data_length <= 4) { // Non-Stream
        ///Non-stream Operation Create Read Answer command
        uint32_t r_data = 0x0;
        memcpy(&r_data,data_ptr,data_length);
        printf("    Transaction info: address(0x%8X); size (%d); read data (0x%8X)\n",(uint32_t)trans->get_address(),(int)data_length,r_data);
        mHSIF020_AgentController->mHSIF020_Unit[ch_id]->TgrConfigReadAnswerCommand(r_data, data_length);
    } else { // data_length > 4
        ///Stream Opeation Command. save to buffer. Issue stream command
        mTxAXIOutstandReq -= 1;
        mTxSumDataReadFromMem += 32;

        if (mTxSumDataReadFromMem > mTxByteNumMaxReadFromMem){
            /// In case mTxByteNumMaxReadFromMem % (4 burst * 8 byte) != 0.
            /// and the last data (16 bytes) is not enough for a read transaction (32 bytes)
            mTxFifoStatus += 16;
        } else {
            mTxFifoStatus += 32;
        }
        /// After received data -> Check to start perform stream command
        if (IsTxFifoRemainDataExist() != 0){
            mHSIF020_AgentController->NotifyTxTransStreamCmd();
        }
    }
}

/// Delete Transaction after processing
void HSIF020_Func::DeleteTrans(TlmTransactionType *trans)
{//{{{
    assert(trans != NULL);
    TlmG3mExtension *G3mExt = (TlmG3mExtension *)0;
    TlmAxiExtension *axiExt = (TlmAxiExtension *)0;
    
    trans->get_extension(axiExt);
    assert(axiExt != NULL);
    trans->release_extension(axiExt);

    trans->get_extension(G3mExt);
    assert(G3mExt != NULL);
    trans->release_extension(G3mExt);

    delete trans;
}//}}}

/// Notify Transaction is complete (in case AT mode)
/// Virtual functions in BusMasterFuncIf
void HSIF020_Func::notifyTransComp(TlmTransactionType &trans, BusTime_t &t)
{//{{{
    std::queue<TlmBasicPayload *> tmpFifo;
    while (!mRequestFifo.empty()){
        TlmBasicPayload *TransTemp = mRequestFifo.front();
        mRequestFifo.pop();
        if (&trans == ((TlmTransactionType *)TransTemp)) {
            TlmAxiExtension* axiExt = (TlmAxiExtension*)0;
            TlmG3mExtension* g3mExt = (TlmG3mExtension*)0;
            trans.get_extension(axiExt);
            trans.get_extension(g3mExt);

            uint32_t tempTransID = axiExt->getTransId();
            TransIdInfoMap_t::iterator it = mTransIdInfoMap.find(tempTransID);
            if (trans.is_read()){
                if (mHSIF020_AgentController->IsAxiError()){
                    if(trans.get_data_length() > 4) {
                        HandleTxAxiErrorStreamCmd();
                    } else {
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).second.first]->RxNonStreamHandleAxiError();
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).second.first]->TgrConfigNACKCommand();
                    }
                } else {
                    ReceiveRD_DATA(&trans,(*it).second.first);
                }
                mTransIdInfoMap.erase(it);
            } else { // trans.is_write()
                tlm::tlm_response_status axi_response_status = trans.get_response_status();
                if (mHSIF020_AgentController->IsAxiError())
                    axi_response_status = tlm::TLM_INCOMPLETE_RESPONSE;
                if (axi_response_status == tlm::TLM_OK_RESPONSE){
                    if(trans.get_data_length() > 4) {
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).second.first]->TgrConfigACKStreamCommand((*it).second.second);
                        UpdateWritePointer();
                        UpdateCompleteFlag();
                    } else {
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).second.first]->TgrConfigACKCommand();
                    }
                } else {
                    if(trans.get_data_length() > 4) {
                        // Stream command
                        UpdateWritePointer();
                        UpdateCompleteFlag();
                        HandleRxAxiErrorStreamCmd();
                    } else {
                        // Non-stream command
                        mHSIF020_AgentController->mHSIF020_Unit[(*it).second.first]->RxNonStreamHandleAxiError();
                    }
                    mHSIF020_AgentController->mHSIF020_Unit[(*it).second.first]->TgrConfigNACKStreamCommand((*it).second.second);
                }
                if(trans.get_data_length() > 4) {
                    mRxAXIOutstandReq -= 1;
                    mRxStreamInputNumb -= 1;
                }
            }
            DeleteTrans(&trans);
        } else {
            tmpFifo.push(TransTemp);
        }
    }
    mRequestFifo = tmpFifo;
}//}}}

/// Notify Transaction send
/// Virtual functions in BusMasterFuncIf
void HSIF020_Func::notifyTransSend(TlmTransactionType &trans, BusTime_t &t)
{//{{{

}//}}}

/// Check all channel (0 - 1 - 2 -3) is disable or not
bool HSIF020_Func::isAllChannelDisable(void)
{//{{{
    bool isAllChannelDisable = true;
    for (uint32_t index = 0; index < emCHNum; index ++){
        if (!mHSIF020_AgentController -> mHSIF020_Unit[index]->mHSIF020_UNIT_Func->IsChTxRxDis()){
            isAllChannelDisable =  false;
            break;
        }
    }
    return isAllChannelDisable;
}//}}}

/// Check Transaction is write by ICU module or not
bool HSIF020_Func::isWriteByICUModule(void)
{//{{{
    return (mSecure == true);//26
}//}}}

/// Check Transaction is write by This IP or not
bool HSIF020_Func::isWriteByThisIP(void)
{//{{{
    return (mTransInfoSPID == mHSIF020_AgentController->GetThisIpSpidInfo());//19
}//}}}

/// Get Time-out setting
uint32_t HSIF020_Func::GetTimeOutSetting(void)
{//{{{
    return (uint32_t)(*HSIFnMRT);
}//}}}

/// Get setting specifies the type of the channel number code table to be used for this IP
bool HSIF020_Func::GetTableSetting(void)
{//{{{
    return (bool)((uint32_t)(*HSIFnMMD) & 0x1);
}//}}}

/// Set the authentication ID base
void HSIF020_Func::SetAuthenIdBaseKey(uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3
                            , uint32_t id4, uint32_t id5, uint32_t id6, uint32_t id7)
{//{{{
    mAuthenIdBaseKey[0] = id0;
    mAuthenIdBaseKey[1] = id1;
    mAuthenIdBaseKey[2] = id2;
    mAuthenIdBaseKey[3] = id3;
    mAuthenIdBaseKey[4] = id4;
    mAuthenIdBaseKey[5] = id5;
    mAuthenIdBaseKey[6] = id6;
    mAuthenIdBaseKey[7] = id7;
}//}}}

/// Virtual function of HSIFnMMD register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnMMD_CHCT(RegCBstr str)
{//{{{
    if (!mHSIF020_AgentController->isInitialSetting()) {
        re_printf("warning","Cannot change setting of HSIFnMMD during system operation. You can change only at the time of initial setting.");
        (*HSIFnMMD) = str.pre_data;
    }
}//}}}

/// Virtual function of HSIFnMCT register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnMCT_WDWE(RegCBstr str)
{//{{{
    if (str.is_wr && (!isAllChannelDisable())){
        re_printf("warning","Cannot change setting of HSIFnMCT during system operation. You can change this bit only when the initiator node function and target node function of all channels are disabled.");
        (*HSIFnMCT)= str.pre_data;
    } else if (str.is_wr && isWriteByThisIP()){
        re_printf("warning","Cannot change setting of HSIFnMCT by this IP master port");
        (*HSIFnMCT)= str.pre_data;
    } else if (str.is_wr && (isAllChannelDisable() && (!isWriteByThisIP()))){
        for (uint8_t index = 0; index < emMemWindowNumbs; index ++){
            ///Check address and size condition
            ///hapv3 :Update condition
            uint32_t curWindowXPermission = (str.data >> (index * 4)) & 0x3;
            uint32_t startAddress   = (*HSIFnMWmA[index]);
            uint32_t endAddress     = (*HSIFnMWmA[index]) | (*HSIFnMWmS[index]);
            bool writeEnable        = (curWindowXPermission >> 0x1) & 0x1;
            bool readEnable         = (curWindowXPermission >> 0x0) & 0x1;

            mWindowMem[index]       = MemoryWindow_t(AddressRange_t(startAddress,endAddress)
                                                    ,AccessInfo_t(writeEnable,readEnable));

            re_printf("info","Window Memory [%d]: Start address: %8X End address: %8X Write Permission: %8X Read Permission: %8X", \
                    (uint32_t) index, mWindowMem[index].first.first, mWindowMem[index].first.second,  \
                    mWindowMem[index].second.first, mWindowMem[index].second.second);
        }
        ///Check overlap
        for (uint8_t index = 0; index < emMemWindowNumbs; index ++) {
            for (uint8_t jndex = index + 1; jndex < emMemWindowNumbs; jndex ++) {
                /// if start or end address is between start or end address of other
                if ((  (mWindowMem[jndex].first.first >= mWindowMem[index].first.first)
                     &&(mWindowMem[jndex].first.first <= mWindowMem[index].first.second))
                  ||  ((mWindowMem[jndex].first.second >= mWindowMem[index].first.first)
                     &&(mWindowMem[jndex].first.second <= mWindowMem[index].first.second))){
                    re_printf("warning","Setting %s is overlap with setting %s",mWindowLabel[index].c_str(),mWindowLabel[jndex].c_str());
                }
            }
        }
    }
}//}}}

/// Check a address range is permitted access via Memory window or not
bool HSIF020_Func::IsMemWindowAccessPer(uint32_t startAdrress, uint32_t endAddress, bool command)
{//{{{
    bool access = false;
    // Address is inside of any Memory Windows which are defined by RHSIFnMW(X)A and RHSIFnMW(X)S
    // Read/Write is allowed for a Memory Window whose address is hit
    for (uint8_t index = 0; index < emMemWindowNumbs; index ++){
        ///Check address access
        if (    (startAdrress >= mWindowMem[index].first.first)
             && (endAddress <= mWindowMem[index].first.second)){
            if (command)// isWrite
                access =  mWindowMem[index].second.first;
            else
                access =  mWindowMem[index].second.second;
            if (access == true) break;
        }
    }
    return access;
}//}}}

/// Check READ/WRITE access to authentication register
bool HSIF020_Func::IsAccessToAuthentRegs(uint32_t startAddress)
{//{{{
    bool access = false;
    //Search address in list, return true if found
    if (mRegsAuthenAddrSet.find(startAddress & emHSIF020_AddrMask) != mRegsAuthenAddrSet.end()) {
        access = true;
    }
    return access;
}//}}}

/// Virtual function of HSIFnMST register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnMST_AUTS1(RegCBstr str){;}

/// Virtual function of HSIFnMIST register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnMIST_AESS(RegCBstr str){;}

/// Virtual function of HSIFnMRT register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnMRT_RSCL(RegCBstr str)
{//{{{
    if (str.is_wr && !mHSIF020_AgentController->isInitialSetting()){
        re_printf("warning","Cannot change setting of HSIFnMRT during system operation. You can change only at the time of initial setting.");
        (*HSIFnMRT)= str.pre_data;
    }
    if (str.is_wr && mHSIF020_AgentController->isInitialSetting()){
        if ((str.data & 0x3FF) == 0){
            re_printf("warning","When 0 is specified in these bits, the reply timeout does not occur.");
            re_printf("warning","the initiator on the channel is disabled.");
        }
    }
}//}}}

/// Virtual function of HSIFnMWmA register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnMWmA_MWmA(RegCBstr str)
{//{{{
    if (str.is_wr && isWriteByThisIP()){
        re_printf("warning","Cannot change setting of HSIFnMWmA by this IP master port");
        (*HSIFnMWmA[str.channel]) = str.pre_data;
    } else if (str.is_wr && (!isWriteByThisIP())) {
        if ( ((((*HSIFnMCT) >> (str.channel * 4)) & 0x1) == 0x1)
          || ((((*HSIFnMCT) >> (str.channel * 4)) & 0x2) == 0x2)){
            re_printf("warning","When HSIFnMCT.W(X)RE = 1 or HSIFnMCT.W(X)WE = 1, the value of register must not be changed.");
            (*HSIFnMWmA[str.channel]) = str.pre_data;
        }
        else {
            ///Validate value input
            ///Bit [0:1] can not writable, alway 0
            ///The MW(X)A[6:2] bits must always be 0. Min-size = 128 bit
            if ((str.data & 0x7F) != 0x0){
                re_printf("warning","The MWmA[6:2] bits must always be: 'b0. Behavior of model maybe not correctly with this setting.");
            }
        }
    }
}//}}}

/// Virtual function of HSIFnMWmS register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnMWmS_MWmS(RegCBstr str)
{//{{{
    if (str.is_wr && isWriteByThisIP()){
        re_printf("warning","Cannot change setting of HSIFnMWmS by this IP master port");
        (*HSIFnMWmS[str.channel]) = str.pre_data;
    } else if (str.is_wr && (!isWriteByThisIP())) {
        if ( ((((*HSIFnMCT) >> (str.channel * 4)) & 0x1) == 0x1)
          || ((((*HSIFnMCT) >> (str.channel * 4)) & 0x2) == 0x2)){
            re_printf("warning","When HSIFnMCT.W(X)RE = 1 or HSIFnMCT.W(X)WE = 1, the value of HSIFnMWS must not be changed.");
            (*HSIFnMWmS[str.channel]) = str.pre_data;
        }
        else {
            ///Validate value input
            ///Bit [0:1] can not writable, alway 0
            ///The MW(X)S[6:2] bits must always be 1. Min-size = 128 bit
            if ((str.data & 0x7F) != 0x7C){
                re_printf("warning","The MWmS[6:2] bits must always be: 'b1. Behavior of model maybe not correctly with this setting.");
            }
        }
    }
}//}}}

/// Virtual function of HSIFnSTMD register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSTMD_STNK(RegCBstr str)
{//{{{
    if (!mHSIF020_AgentController->isInitialSetting()) {
        re_printf("warning","Cannot change setting of HSIFnSTMD during system operation. You can change only at the time of initial setting.");
        (*HSIFnSTMD) = str.pre_data;
    } else if (str.is_wr && mHSIF020_AgentController->isInitialSetting()){
        if ((*HSIFnSTMD)["STPS"] == 0x1) {
            mTxFrameDataSize = 16;
        } else {
            mTxFrameDataSize = 32;
        }
    }
}//}}}

/// Virtual function of HSIFnSTCT register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSTCT_STDE(RegCBstr str)
{//{{{
    if (str.is_wr && ((str.pre_data & 0x1) == 0x1)) {
        re_printf("warning","Cannot change setting of HSIFnSTCT during transfer Stream Command");
        (*HSIFnSTCT) = str.pre_data;
    } else if (str.is_wr && ((str.pre_data & 0x1) == 0x0) && (mHSIF020_AgentController->CheckClockPeriod("CCLK"))){
        if (mHSIF020_AgentController->mHSIF020_Unit[emStreamChannel]->IsChTxEnable()){
            /// Clear CST.RDY bit
            mHSIF020_AgentController->mHSIF020_Unit[emStreamChannel]->mHSIF020_UNIT_Func->ClearChannelStatus(emRDY);
            /// DMA Channel is enable
            mTxByteNumMaxReadFromMem = (*HSIFnSTBC);
            if (mTxByteNumMaxReadFromMem == 0)
                mTxByteNumMaxReadFromMem = 0x1 << 25;
            /// Initial value of variable of Fifo status
            mTxFifoStatus = 0;
            mTxFifoWritePointer = 0;
            mTxFifoReadPointer = 0;
            mTxMemReadPointer = 0;
            mTxSumDataReadFromMem = 0;

            mStreamIgnoreReplyCmdAfterTimeOut = false;
            mHSIF020_AgentController->TxStreamNotifyGetData2Fifo();
            mHSIF020_AgentController->SetInitialSettingPhase(false);
        } else {
            re_printf("warning","Can not set STDE to 1 when HSIFnCMD2.INME2 = 0.");
            (*HSIFnSTCT) = str.pre_data;
        }
    }
}//}}}

/// Virtual function of HSIFnSTSC register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSTSC_STEC4(RegCBstr str)
{//{{{
    if ((str.data != 0) && str.is_wr){
        if ((*HSIFnSTSC)["STEC4"] == 1) {
            (*HSIFnSTST)["STE4"] = 0;
            SetInterruptStatus(emSTES);
        }
        if ((*HSIFnSTSC)["STEC3"] == 1) {
            (*HSIFnSTST)["STE3"] = 0;
            SetInterruptStatus(emSTES);
        }
        if ((*HSIFnSTSC)["STEC2"] == 1) {
            (*HSIFnSTST)["STE2"] = 0;
            SetInterruptStatus(emSTES);
        }
        if ((*HSIFnSTSC)["STEC1"] == 1) {
            (*HSIFnSTST)["STE1"] = 0;
            SetInterruptStatus(emSTES);
        }
        if ((*HSIFnSTSC)["STEC0"] == 1) {
            (*HSIFnSTST)["STE0"] = 0;
            SetInterruptStatus(emSTES);
        }
        if ((*HSIFnSTSC)["STCC"] == 1) {
            (*HSIFnSTST)["STC"] = 0;
            SetInterruptStatus(emSTCS);
        }

    }
}//}}}

/// Virtual function of HSIFnSTIE register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSTIE_STEE4(RegCBstr str){;}

/// Virtual function of HSIFnSTSA register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSTSA_STSA(RegCBstr str){;}

/// Virtual function of HSIFnSTBC register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSTBC_STBC(RegCBstr str)
{//{{{
    if (str.is_wr && ((*HSIFnSTCT)["STDE"] == 0x1)){
        re_printf("warning","When HSIFnSTCT.STDE = 1, the value of these bits (STBC) must not be changed.");
    }
}//}}}

/// Virtual function of HSIFnSRMD register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRMD_SRMC(RegCBstr str)
{//{{{
    if (str.is_wr && (!mHSIF020_AgentController->isInitialSetting())){
        re_printf("warning","Cannot change setting of HSIFnSRMD during system operation. You can change only at the time of initial setting.");
        (*HSIFnSRMD)= str.pre_data;
    } else if (str.is_wr && mHSIF020_AgentController->isInitialSetting() && (mHSIF020_AgentController->CheckClockPeriod("CCLK"))){
        if ((*HSIFnSRMD)["SRPS"] == 0x1) {
            mRxFrameDataSize = 16;
        } else {
            mRxFrameDataSize = 32;
        }
    }
}//}}}

/// Virtual function of HSIFnSRCT register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRCT_SRDE(RegCBstr str)
{//{{{
    if(     str.is_wr
         && (!mHSIF020_AgentController->mHSIF020_Unit[emStreamChannel]->IsChRxEnable())
         && ((str.data & 0x1) == 0x1)){
        re_printf("warning","Can not set SRDE to 1 when HSIFnCMD2.TNME2 = 0.");
        (*HSIFnSRCT) = str.pre_data;
    }
    if (  str.is_wr
      &&  mHSIF020_AgentController->mHSIF020_Unit[emStreamChannel]->IsChRxEnable()
      && (mHSIF020_AgentController->CheckClockPeriod("CCLK"))){
        if (((str.data & 0x1) == 0x1) && (str.data != str.pre_data)){
            mRxMaxSizeUnprocessedData = (*HSIFnSRBC);
            if (mRxMaxSizeUnprocessedData == 0x0){
                mRxMaxSizeUnprocessedData = 0x1 << 25;
            }
            /// Initial value of variable of Fifo status
            mRxFifoStatus = 0;
            mRxFifoWritePointer = 0;
            mRxFifoReadPointer = 0;
            mRxMemWritePointer = 0;
            mRxSumDataWriteToMem = 0;
            /// Initialize register for new Rx stream process
            (*HSIFnSRWP) = 0x0;
            (*HSIFnSRRP) = 0x0;
            mHSIF020_AgentController->SetInitialSettingPhase(false);
        }
        if (((str.data & 0x1) == 0x0) && (str.data != str.pre_data)){
            ClearRxDMACBit();
        }
    }

}//}}}

/// Virtual function of HSIFnSRST register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRST_SRA(RegCBstr str){;}

/// Virtual function of HSIFnSRSC register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRSC_SRAC(RegCBstr str)
{//{{{
    if((str.data != 0) && str.is_wr){
        if ((*HSIFnSRSC)["SRAC"] == 1) {
            (*HSIFnSRST)["SRA"] = 0;
            SetInterruptStatus(emSRES);
        }
        if ((*HSIFnSRSC)["SREC2"] == 1) {
            (*HSIFnSRST)["SRE2"] = 0;
            SetInterruptStatus(emSRES);
        }
        if ((*HSIFnSRSC)["SREC1"] == 1) {
            (*HSIFnSRST)["SRE1"] = 0;
            SetInterruptStatus(emSRES);
        }
        if ((*HSIFnSRSC)["SREC0"] == 1) {
            (*HSIFnSRST)["SRE0"] = 0;
            SetInterruptStatus(emSRES);
        }
    }
}//}}}

/// Virtual function of HSIFnSRIE register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRIE_SRAE(RegCBstr str){;}

/// Virtual function of HSIFnSRDA register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRDA_SRDA(RegCBstr str)
{//{{{
    if (str.is_wr && ((*HSIFnSRCT)["SRDE"] == 0x1)){
        re_printf("warning","When HSIFnSRCT.SRDE = 1, the value of these bits (SRDA) must not be changed.");
        (*HSIFnSRDA) = str.pre_data;
    }
}//}}}

/// Virtual function of HSIFnSRDS register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRDS_SRDS(RegCBstr str)
{//{{{
    if (str.is_wr && ((*HSIFnSRCT)["SRDE"] == 0x1)){
        re_printf("warning","When HSIFnSRCT.SRDE = 1, the value of these bits (SRDS) must not be changed.");
        (*HSIFnSRDS) = str.pre_data;
    }
}//}}}

/// Virtual function of HSIFnSRBC register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRBC_SRBC(RegCBstr str)
{//{{{
    if (str.is_wr && ((*HSIFnSRCT)["SRDE"] == 0x1)){
        re_printf("warning","When HSIFnSRCT.SRDE = 1, the value of these bits (SRBC) must not be changed.");
        (*HSIFnSRBC) = str.pre_data;
    } else {
        (*HSIFnSRBC) = str.data & 0x1FFFFF0;
    }
}//}}}

/// Virtual function of HSIFnSRWP register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRWP_SRWP(RegCBstr str){;}

/// Virtual function of HSIFnSRRP register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnSRRP_SRRP(RegCBstr str)
{//{{{
    if(str.is_wr){
        mRxMemReadPointer = (*HSIFnSRRP) & 0x1FFFF0;
        mRxMemReadPointerToggleBit = (*HSIFnSRRP)["SRRT"];
        UpdateCompleteFlag();
    }
}//}}}

/// Virtual function of HSIFnAID register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnAID_AID(RegCBstr str)
{//{{{
    if (str.is_wr && (!isWriteByThisIP())){
        (*HSIFnAID[str.channel]) = str.pre_data;
        re_printf("warning","Can be read from or written to HSIFnAID only via the master port of this IP.");
    } else if (str.is_wr
            && isWriteByThisIP() && (str.channel == (emAuthIDbaseRegNumbs - 1))
            && (mHSIF020_AgentController->CheckClockPeriod("CCLK"))){
        /// Process Authentication ID base
        // if (mTimeOfAuthenIdBase < emAuthIdTimeMax){
            bool isPassAuthenIdBase = true;
            for (uint8_t index = 0; index < emAuthIDbaseRegNumbs; index ++){
                if ((*HSIFnAID[index]) != mAuthenIdBaseKey[index]){
                    isPassAuthenIdBase = false;
                    re_printf("warning","Authentication fail at register HSIFnAID[%d]. at [ %d ] times.",index, (uint32_t)mTimeOfAuthenIdBase);
                    break;
                }
            }
            mHSIF020_AgentController->SetAuthenIdStatus(isPassAuthenIdBase);
            if (isPassAuthenIdBase){
                mTimeOfAuthenIdBase = 0;
                (*HSIFnMST)["AUTS0"] = 0x1;
            }
            if (!isPassAuthenIdBase){
                mTimeOfAuthenIdBase += 1;
                (*HSIFnMST)["AUTS0"] = 0x0;
            }
            if ((!isPassAuthenIdBase) && (mTimeOfAuthenIdBase >= emAuthIdTimeMax)){
                mHSIF020_AgentController->SetAuthenIdLock();
            }
        // } 
    }
}//}}}

/// Virtual function of HSIFnAAD register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnAAD_AAD(RegCBstr str)
{//{{{
    if (str.is_wr &&(!isWriteByICUModule())){
        (*HSIFnAAD[str.channel]) = str.pre_data;
        re_printf("warning","Can be read from or written to HSIFnAAD only from the security module (ICU).");
    }
}//}}}

/// Virtual function of HSIFnARD register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnARD_ARD(RegCBstr str)
{
    if (str.is_wr && (!isWriteByThisIP())){
        (*HSIFnARD[str.channel]) = str.pre_data;
        re_printf("warning","Can be read from or written to HSIFnARD only via the master port of this IP");
    } else if (str.is_wr
            && isWriteByThisIP() && (str.channel == (emAuthCRRegNumbs - 1))
            && (mHSIF020_AgentController->CheckClockPeriod("CCLK"))){
        bool isPassAuthenCR = true;
        /// Process Authentication CR base
        // if (mTimeOfAuthenCR < emAuthCRTimeMax){
            for (uint8_t index = 0; index < emAuthCRRegNumbs; index ++){
                if ((*HSIFnARD[index]) != (*HSIFnAAD[index])){
                    isPassAuthenCR = false;
                    mTimeOfAuthenCR += 1;
                    re_printf("warning","Authentication fail at register HSIFnARD[%d] and HSIFnAAD[%d]. at [ %d ] times.",index, index,(uint32_t)mTimeOfAuthenCR);
                    break;
                }
            }
            mHSIF020_AgentController->SetAuthenCRStatus(isPassAuthenCR);
            if (isPassAuthenCR){
                mTimeOfAuthenCR = 0;
                (*HSIFnMST)["AUTS1"] = 0x1;
            }
            if (!isPassAuthenCR){
                (*HSIFnMST)["AUTS1"] = 0x0;
            }
            if ((!isPassAuthenCR) && (mTimeOfAuthenCR >= emAuthCRTimeMax)){
                mHSIF020_AgentController->SetAuthenCRLock();
            }
        // }
    }
}//}}}

/// Virtual function of HSIFnAESC register in Chsif020_regif class
void HSIF020_Func::cb_HSIFnAESC_WESC1(RegCBstr str)
{//{{{
    if ((*HSIFnAESC)["AEEC0"] == 0x1) { (*HSIFnAEST)["AEE0"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEIC0"] == 0x1) { (*HSIFnAEST)["AEI0"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AERC0"] == 0x1) { (*HSIFnAEST)["AER0"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEWC0"] == 0x1) { (*HSIFnAEST)["AEW0"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEEC1"] == 0x1) { (*HSIFnAEST)["AEE1"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEIC1"] == 0x1) { (*HSIFnAEST)["AEI1"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AERC1"] == 0x1) { (*HSIFnAEST)["AER1"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEWC1"] == 0x1) { (*HSIFnAEST)["AEW1"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEEC2"] == 0x1) { (*HSIFnAEST)["AEE2"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEIC2"] == 0x1) { (*HSIFnAEST)["AEI2"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AERC2"] == 0x1) { (*HSIFnAEST)["AER2"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEWC2"] == 0x1) { (*HSIFnAEST)["AEW2"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEEC3"] == 0x1) { (*HSIFnAEST)["AEE3"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEIC3"] == 0x1) { (*HSIFnAEST)["AEI3"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AERC3"] == 0x1) { (*HSIFnAEST)["AER3"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AEWC3"] == 0x1) { (*HSIFnAEST)["AEW3"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["AESC"]  == 0x1) { (*HSIFnAEST)["AES"]  = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WERC0"] == 0x1) { (*HSIFnAEST)["WER0"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WEWC0"] == 0x1) { (*HSIFnAEST)["WEW0"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WERC1"] == 0x1) { (*HSIFnAEST)["WER1"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WEWC1"] == 0x1) { (*HSIFnAEST)["WEW1"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WERC2"] == 0x1) { (*HSIFnAEST)["WER2"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WEWC2"] == 0x1) { (*HSIFnAEST)["WEW2"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WERC3"] == 0x1) { (*HSIFnAEST)["WER3"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WEWC3"] == 0x1) { (*HSIFnAEST)["WEW3"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WESC0"] == 0x1) { (*HSIFnAEST)["WES0"] = 0x0; SetInterruptStatus(emAESS); }
    if ((*HSIFnAESC)["WESC1"] == 0x1) { (*HSIFnAEST)["WES1"] = 0x0; SetInterruptStatus(emAESS); }
}//}}}

/// Set authentication error status register
void HSIF020_Func::SetAuthenErrStatus(uint32_t bit_name)
{//{{{
    switch (bit_name)
    {
        case emAEE0: { (*HSIFnAEST)["AEE0"] = 0x1; break; }
        case emAEI0: { (*HSIFnAEST)["AEI0"] = 0x1; break; }
        case emAER0: { (*HSIFnAEST)["AER0"] = 0x1; break; }
        case emAEW0: { (*HSIFnAEST)["AEW0"] = 0x1; break; }
        case emAEE1: { (*HSIFnAEST)["AEE1"] = 0x1; break; }
        case emAEI1: { (*HSIFnAEST)["AEI1"] = 0x1; break; }
        case emAER1: { (*HSIFnAEST)["AER1"] = 0x1; break; }
        case emAEW1: { (*HSIFnAEST)["AEW1"] = 0x1; break; }
        case emAEE2: { (*HSIFnAEST)["AEE2"] = 0x1; break; }
        case emAEI2: { (*HSIFnAEST)["AEI2"] = 0x1; break; }
        case emAER2: { (*HSIFnAEST)["AER2"] = 0x1; break; }
        case emAEW2: { (*HSIFnAEST)["AEW2"] = 0x1; break; }
        case emAEE3: { (*HSIFnAEST)["AEE3"] = 0x1; break; }
        case emAEI3: { (*HSIFnAEST)["AEI3"] = 0x1; break; }
        case emAER3: { (*HSIFnAEST)["AER3"] = 0x1; break; }
        case emAEW3: { (*HSIFnAEST)["AEW3"] = 0x1; break; }
        case emAES : { (*HSIFnAEST)["AES"]  = 0x1; break; }
        case emWER0: { (*HSIFnAEST)["WER0"] = 0x1; break; }
        case emWEW0: { (*HSIFnAEST)["WEW0"] = 0x1; break; }
        case emWER1: { (*HSIFnAEST)["WER1"] = 0x1; break; }
        case emWEW1: { (*HSIFnAEST)["WEW1"] = 0x1; break; }
        case emWER2: { (*HSIFnAEST)["WER2"] = 0x1; break; }
        case emWEW2: { (*HSIFnAEST)["WEW2"] = 0x1; break; }
        case emWER3: { (*HSIFnAEST)["WER3"] = 0x1; break; }
        case emWEW3: { (*HSIFnAEST)["WEW3"] = 0x1; break; }
        case emWES0: { (*HSIFnAEST)["WES0"] = 0x1; break; }
        case emWES1: { (*HSIFnAEST)["WES1"] = 0x1; break; }
        default:     { break; }
    }

}//}}}

/// Get authentication error status
bool HSIF020_Func::GetAuthenErrStatus(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emAEE0: { temp = (bool)((*HSIFnAEST)["AEE0"]); break;}
        case emAEI0: { temp = (bool)((*HSIFnAEST)["AEI0"]); break;}
        case emAER0: { temp = (bool)((*HSIFnAEST)["AER0"]); break;}
        case emAEW0: { temp = (bool)((*HSIFnAEST)["AEW0"]); break;}
        case emAEE1: { temp = (bool)((*HSIFnAEST)["AEE1"]); break;}
        case emAEI1: { temp = (bool)((*HSIFnAEST)["AEI1"]); break;}
        case emAER1: { temp = (bool)((*HSIFnAEST)["AER1"]); break;}
        case emAEW1: { temp = (bool)((*HSIFnAEST)["AEW1"]); break;}
        case emAEE2: { temp = (bool)((*HSIFnAEST)["AEE2"]); break;}
        case emAEI2: { temp = (bool)((*HSIFnAEST)["AEI2"]); break;}
        case emAER2: { temp = (bool)((*HSIFnAEST)["AER2"]); break;}
        case emAEW2: { temp = (bool)((*HSIFnAEST)["AEW2"]); break;}
        case emAEE3: { temp = (bool)((*HSIFnAEST)["AEE3"]); break;}
        case emAEI3: { temp = (bool)((*HSIFnAEST)["AEI3"]); break;}
        case emAER3: { temp = (bool)((*HSIFnAEST)["AER3"]); break;}
        case emAEW3: { temp = (bool)((*HSIFnAEST)["AEW3"]); break;}
        case emAES : { temp = (bool)((*HSIFnAEST)["AES"]);  break;}
        case emWER0: { temp = (bool)((*HSIFnAEST)["WER0"]); break;}
        case emWEW0: { temp = (bool)((*HSIFnAEST)["WEW0"]); break;}
        case emWER1: { temp = (bool)((*HSIFnAEST)["WER1"]); break;}
        case emWEW1: { temp = (bool)((*HSIFnAEST)["WEW1"]); break;}
        case emWER2: { temp = (bool)((*HSIFnAEST)["WER2"]); break;}
        case emWEW2: { temp = (bool)((*HSIFnAEST)["WEW2"]); break;}
        case emWER3: { temp = (bool)((*HSIFnAEST)["WER3"]); break;}
        case emWEW3: { temp = (bool)((*HSIFnAEST)["WEW3"]); break;}
        case emWES0: { temp = (bool)((*HSIFnAEST)["WES0"]); break;}
        case emWES1: { temp = (bool)((*HSIFnAEST)["WES1"]); break;}
        default:     { temp = false; break;}
    }
    return temp;
}//}}}

/// Get authentication interrupt enable
bool HSIF020_Func::GetAuthErrIntEnable(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emAEEE0: { temp = (bool)((*HSIFnAEIE)["AEEE0"] & 0x1); break; }
        case emAEIE0: { temp = (bool)((*HSIFnAEIE)["AEIE0"] & 0x1); break; }
        case emAERE0: { temp = (bool)((*HSIFnAEIE)["AERE0"] & 0x1); break; }
        case emAEWE0: { temp = (bool)((*HSIFnAEIE)["AEWE0"] & 0x1); break; }
        case emAEEE1: { temp = (bool)((*HSIFnAEIE)["AEEE1"] & 0x1); break; }
        case emAEIE1: { temp = (bool)((*HSIFnAEIE)["AEIE1"] & 0x1); break; }
        case emAERE1: { temp = (bool)((*HSIFnAEIE)["AERE1"] & 0x1); break; }
        case emAEWE1: { temp = (bool)((*HSIFnAEIE)["AEWE1"] & 0x1); break; }
        case emAEEE2: { temp = (bool)((*HSIFnAEIE)["AEEE2"] & 0x1); break; }
        case emAEIE2: { temp = (bool)((*HSIFnAEIE)["AEIE2"] & 0x1); break; }
        case emAERE2: { temp = (bool)((*HSIFnAEIE)["AERE2"] & 0x1); break; }
        case emAEWE2: { temp = (bool)((*HSIFnAEIE)["AEWE2"] & 0x1); break; }
        case emAEEE3: { temp = (bool)((*HSIFnAEIE)["AEEE3"] & 0x1); break; }
        case emAEIE3: { temp = (bool)((*HSIFnAEIE)["AEIE3"] & 0x1); break; }
        case emAERE3: { temp = (bool)((*HSIFnAEIE)["AERE3"] & 0x1); break; }
        case emAEWE3: { temp = (bool)((*HSIFnAEIE)["AEWE3"] & 0x1); break; }
        case emAESE : { temp = (bool)((*HSIFnAEIE)["AESE"]  & 0x1); break; }
        case emWERE0: { temp = (bool)((*HSIFnAEIE)["WERE0"] & 0x1); break; }
        case emWEWE0: { temp = (bool)((*HSIFnAEIE)["WEWE0"] & 0x1); break; }
        case emWERE1: { temp = (bool)((*HSIFnAEIE)["WERE1"] & 0x1); break; }
        case emWEWE1: { temp = (bool)((*HSIFnAEIE)["WEWE1"] & 0x1); break; }
        case emWERE2: { temp = (bool)((*HSIFnAEIE)["WERE2"] & 0x1); break; }
        case emWEWE2: { temp = (bool)((*HSIFnAEIE)["WEWE2"] & 0x1); break; }
        case emWERE3: { temp = (bool)((*HSIFnAEIE)["WERE3"] & 0x1); break; }
        case emWEWE3: { temp = (bool)((*HSIFnAEIE)["WEWE3"] & 0x1); break; }
        case emWESE0: { temp = (bool)((*HSIFnAEIE)["WESE0"] & 0x1); break; }
        case emWESE1: { temp = (bool)((*HSIFnAEIE)["WESE1"] & 0x1); break; }
        default:      { temp = false; break;}
    }
    return temp;
}//}}}

/// Set interrupt status register (HSIFnMIST)
void HSIF020_Func::SetInterruptStatus(uint32_t bit_name)
{//{{{
    switch (bit_name){
        case emRARS0: { 
            (*HSIFnMIST)["RARS0"] = ComputeMIST_RARS_Status(emCH0); 
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x0, (bool)((*HSIFnMIST)["RARS0"]));
            break;
            }
        case emAKRS0: { 
            (*HSIFnMIST)["AKRS0"] = ComputeMIST_AKRS_Status(emCH0); 
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x0, (bool)((*HSIFnMIST)["AKRS0"]));
            break;
            }
        case emTERS0: { 
            (*HSIFnMIST)["TERS0"] = ComputeMIST_TERS_Status(emCH0);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x0, (bool)((*HSIFnMIST)["TERS0"])); 
            break;
            }
        case emRARS1: { 
            (*HSIFnMIST)["RARS1"] = ComputeMIST_RARS_Status(emCH1);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x1, (bool)((*HSIFnMIST)["RARS1"]));
            break;
            }
        case emAKRS1: { 
            (*HSIFnMIST)["AKRS1"] = ComputeMIST_AKRS_Status(emCH1);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x1, (bool)((*HSIFnMIST)["AKRS1"])); 
            break;
            }
        case emTERS1: { 
            (*HSIFnMIST)["TERS1"] = ComputeMIST_TERS_Status(emCH1);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x1, (bool)((*HSIFnMIST)["TERS1"])); 
            break;
            }
        case emRARS2: { 
            (*HSIFnMIST)["RARS2"] = ComputeMIST_RARS_Status(emCH2);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x2, (bool)((*HSIFnMIST)["RARS2"])); 
            break;
            }
        case emAKRS2: { 
            (*HSIFnMIST)["AKRS2"] = ComputeMIST_AKRS_Status(emCH2);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x2, (bool)((*HSIFnMIST)["AKRS2"]));
            break;
            }
        case emTERS2: { 
            (*HSIFnMIST)["TERS2"] = ComputeMIST_TERS_Status(emCH2);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x2, (bool)((*HSIFnMIST)["TERS2"])); 
            break;
            }
        case emRARS3: { 
            (*HSIFnMIST)["RARS3"] = ComputeMIST_RARS_Status(emCH3);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x3, (bool)((*HSIFnMIST)["RARS3"])); 
            break;
            }
        case emAKRS3: { 
            (*HSIFnMIST)["AKRS3"] = ComputeMIST_AKRS_Status(emCH3);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x3, (bool)((*HSIFnMIST)["AKRS3"])); 
            break;
            }
        case emTERS3: { 
            (*HSIFnMIST)["TERS3"] = ComputeMIST_TERS_Status(emCH3);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_ch",0x3, (bool)((*HSIFnMIST)["TERS3"])); 
            break;
            }
        case emSTCS : { 
            (*HSIFnMIST)["STCS"]  = ComputeMIST_STCS_Status();     
            mHSIF020_AgentController->IssueInterrupt("int_hsif_str",0x0, (bool)((*HSIFnMIST)["STCS"])); 
            break;
            }
        case emSRCS0: { 
            (*HSIFnMIST)["SRCS0"] = ComputeMIST_SRCS0_Status();    
            mHSIF020_AgentController->IssueInterrupt("int_hsif_str",0x0, (bool)((*HSIFnMIST)["SRCS0"])); 
            break;
            }
        case emSRCS1: { 
            (*HSIFnMIST)["SRCS1"] = ComputeMIST_SRCS1_Status();    
            mHSIF020_AgentController->IssueInterrupt("int_hsif_str",0x0, (bool)((*HSIFnMIST)["SRCS1"])); 
            break;
            }
        case emCERS0: { 
            (*HSIFnMIST)["CERS0"] = ComputeMIST_CERS_Status(emCH0);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_err",0x0, (bool)((*HSIFnMIST)["CERS0"])); 
            break;
            }
        case emCERS1: { 
            (*HSIFnMIST)["CERS1"] = ComputeMIST_CERS_Status(emCH1);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_err",0x0, (bool)((*HSIFnMIST)["CERS1"])); 
            break;
            }
        case emCERS2: { 
            (*HSIFnMIST)["CERS2"] = ComputeMIST_CERS_Status(emCH2);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_err",0x0, (bool)((*HSIFnMIST)["CERS2"])); 
            break;
            }
        case emCERS3: { 
            (*HSIFnMIST)["CERS3"] = ComputeMIST_CERS_Status(emCH3);
            mHSIF020_AgentController->IssueInterrupt("int_hsif_err",0x0, (bool)((*HSIFnMIST)["CERS3"])); 
            break;
            }
        case emSTES : { 
            (*HSIFnMIST)["STES"]  = ComputeMIST_STES_Status();     
            mHSIF020_AgentController->IssueInterrupt("int_hsif_err",0x0, (bool)((*HSIFnMIST)["STES"])); 
            break;
            }
        case emSRES : { 
            (*HSIFnMIST)["SRES"]  = ComputeMIST_SRES_Status();     
            mHSIF020_AgentController->IssueInterrupt("int_hsif_err",0x0, (bool)((*HSIFnMIST)["SRES"])); 
            break;
            }
        case emAESS : { 
            (*HSIFnMIST)["AESS"]  = ComputeMIST_AESS_Status();     
            mHSIF020_AgentController->IssueInterrupt("int_hsif_sec",0x0, (bool)((*HSIFnMIST)["AESS"])); 
            break;
            }
        default:      { break;}
    }
}//}}}

/// Compute Interrupt status bit RARS
bool HSIF020_Func::ComputeMIST_RARS_Status(uint8_t ch_id)
{//{{{
    bool status = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emRAR);
    bool intEn = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emRARE);
    return (status && intEn);
}//}}}

/// Compute Interrupt status bit RKRS
bool HSIF020_Func::ComputeMIST_AKRS_Status(uint8_t ch_id)
{//{{{
    bool status = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emAKR);
    bool intEn = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emAKRE);
    return (status && intEn);
}//}}}

/// Compute Interrupt status bit TERS
bool HSIF020_Func::ComputeMIST_TERS_Status(uint8_t ch_id)
{//{{{
    bool status = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emTER);
    bool intEn = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emTERE);
    return (status && intEn);
}//}}}

/// Compute Interrupt status bit STCS
bool HSIF020_Func::ComputeMIST_STCS_Status(void)
{//{{{
    bool status = GetTxStreamStatus(emSTC);
    bool intEn =  GetTxStreamIntEnable(emSTCE);
    return (status && intEn);
}//}}}

/// Compute Interrupt status bit SRCS0
bool HSIF020_Func::ComputeMIST_SRCS0_Status(void)
{//{{{
    bool status = GetRxStreamStatus(emSRC0);
    bool intEn =  GetRxStreamIntEnable(emSRCE0);
    return (status && intEn);
}//}}}

/// Compute Interrupt status bit SRCS1
bool HSIF020_Func::ComputeMIST_SRCS1_Status(void)
{//{{{
    bool status = GetRxStreamStatus(emSRC1);
    bool intEn =  GetRxStreamIntEnable(emSRCE1);
    return (status && intEn);
}//}}}

/// Compute Interrupt status bit CERS
bool HSIF020_Func::ComputeMIST_CERS_Status(uint8_t ch_id)
{//{{{
    bool status0 = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emAKE);
    bool intEn0  =  mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emAKEE);

    bool status1 = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emTOE);
    bool intEn1  =  mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emTOEE);

    bool status2 = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emIDE);
    bool intEn2  =  mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emIDEE);

    bool status3 = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emAOE);
    bool intEn3  =  mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emAOEE);

    bool status4 = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emBRE);
    bool intEn4  =  mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emBREE);

    bool status5 = mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChannelStatus(emCRE);
    bool intEn5  =  mHSIF020_AgentController->mHSIF020_Unit[ch_id]->mHSIF020_UNIT_Func->GetChIntEnable(emCREE);

    bool temp = false;
    temp = (   (status0 && intEn0)
            || (status1 && intEn1)
            || (status2 && intEn2)
            || (status3 && intEn3)
            || (status4 && intEn4)
            || (status5 && intEn5));
    return temp;
}//}}}

/// Compute Interrupt status bit CTES
bool HSIF020_Func::ComputeMIST_STES_Status(void)
{//{{{
    bool status0 = GetTxStreamStatus(emSTE0);
    bool intEn0  = GetTxStreamIntEnable(emSTEE0);

    bool status1 = GetTxStreamStatus(emSTE1);
    bool intEn1  = GetTxStreamIntEnable(emSTEE1);

    bool status2 = GetTxStreamStatus(emSTE2);
    bool intEn2  = GetTxStreamIntEnable(emSTEE2);

    bool status3 = GetTxStreamStatus(emSTE3);
    bool intEn3  = GetTxStreamIntEnable(emSTEE3);

    bool status4 = GetTxStreamStatus(emSTE4);
    bool intEn4  = GetTxStreamIntEnable(emSTEE4);

    bool temp = false;
    temp = (   (status0 && intEn0)
            || (status1 && intEn1)
            || (status2 && intEn2)
            || (status3 && intEn3)
            || (status4 && intEn4));
    return temp;
}//}}}

/// Compute Interrupt status bit SRES
bool HSIF020_Func::ComputeMIST_SRES_Status(void)
{//{{{
    bool status0 = GetRxStreamStatus(emSRE0);
    bool intEn0  = GetRxStreamIntEnable(emSREE0);

    bool status1 = GetRxStreamStatus(emSRE1);
    bool intEn1  = GetRxStreamIntEnable(emSREE1);

    bool status2 = GetRxStreamStatus(emSRE2);
    bool intEn2  = GetRxStreamIntEnable(emSREE2);

    bool status3 = GetRxStreamStatus(emSRA);
    bool intEn3  = GetRxStreamIntEnable(emSRAE);

    bool temp = false;
    temp = (   (status0 && intEn0)
            || (status1 && intEn1)
            || (status2 && intEn2)
            || (status3 && intEn3));
    return temp;
}//}}}

/// Compute Interrupt status bit AESS
bool HSIF020_Func::ComputeMIST_AESS_Status(void)
{//{{{
    bool status0  = GetAuthenErrStatus(emAEE0);
    bool status1  = GetAuthenErrStatus(emAEI0);
    bool status2  = GetAuthenErrStatus(emAER0);
    bool status3  = GetAuthenErrStatus(emAEW0);
    bool status4  = GetAuthenErrStatus(emAEE1);
    bool status5  = GetAuthenErrStatus(emAEI1);
    bool status6  = GetAuthenErrStatus(emAER1);
    bool status7  = GetAuthenErrStatus(emAEW1);
    bool status8  = GetAuthenErrStatus(emAEE2);
    bool status9  = GetAuthenErrStatus(emAEI2);
    bool status10 = GetAuthenErrStatus(emAER2);
    bool status11 = GetAuthenErrStatus(emAEW2);
    bool status12 = GetAuthenErrStatus(emAEE3);
    bool status13 = GetAuthenErrStatus(emAEI3);
    bool status14 = GetAuthenErrStatus(emAER3);
    bool status15 = GetAuthenErrStatus(emAEW3);
    bool status16 = GetAuthenErrStatus(emAES );
    bool status17 = GetAuthenErrStatus(emWER0);
    bool status18 = GetAuthenErrStatus(emWEW0);
    bool status19 = GetAuthenErrStatus(emWER1);
    bool status20 = GetAuthenErrStatus(emWEW1);
    bool status21 = GetAuthenErrStatus(emWER2);
    bool status22 = GetAuthenErrStatus(emWEW2);
    bool status23 = GetAuthenErrStatus(emWER3);
    bool status24 = GetAuthenErrStatus(emWEW3);
    bool status25 = GetAuthenErrStatus(emWES0);
    bool status26 = GetAuthenErrStatus(emWES1);

    bool intEn0   = GetAuthErrIntEnable(emAEEE0);
    bool intEn1   = GetAuthErrIntEnable(emAEIE0);
    bool intEn2   = GetAuthErrIntEnable(emAERE0);
    bool intEn3   = GetAuthErrIntEnable(emAEWE0);
    bool intEn4   = GetAuthErrIntEnable(emAEEE1);
    bool intEn5   = GetAuthErrIntEnable(emAEIE1);
    bool intEn6   = GetAuthErrIntEnable(emAERE1);
    bool intEn7   = GetAuthErrIntEnable(emAEWE1);
    bool intEn8   = GetAuthErrIntEnable(emAEEE2);
    bool intEn9   = GetAuthErrIntEnable(emAEIE2);
    bool intEn10  = GetAuthErrIntEnable(emAERE2);
    bool intEn11  = GetAuthErrIntEnable(emAEWE2);
    bool intEn12  = GetAuthErrIntEnable(emAEEE3);
    bool intEn13  = GetAuthErrIntEnable(emAEIE3);
    bool intEn14  = GetAuthErrIntEnable(emAERE3);
    bool intEn15  = GetAuthErrIntEnable(emAEWE3);
    bool intEn16  = GetAuthErrIntEnable(emAESE );
    bool intEn17  = GetAuthErrIntEnable(emWERE0);
    bool intEn18  = GetAuthErrIntEnable(emWEWE0);
    bool intEn19  = GetAuthErrIntEnable(emWERE1);
    bool intEn20  = GetAuthErrIntEnable(emWEWE1);
    bool intEn21  = GetAuthErrIntEnable(emWERE2);
    bool intEn22  = GetAuthErrIntEnable(emWEWE2);
    bool intEn23  = GetAuthErrIntEnable(emWERE3);
    bool intEn24  = GetAuthErrIntEnable(emWEWE3);
    bool intEn25  = GetAuthErrIntEnable(emWESE0);
    bool intEn26  = GetAuthErrIntEnable(emWESE1);

    bool temp = false;
    temp =  (   (status0  && intEn0 )
             || (status1  && intEn1 )
             || (status2  && intEn2 )
             || (status3  && intEn3 )
             || (status4  && intEn4 )
             || (status5  && intEn5 )
             || (status6  && intEn6 )
             || (status7  && intEn7 )
             || (status8  && intEn8 )
             || (status9  && intEn9 )
             || (status10 && intEn10)
             || (status11 && intEn11)
             || (status12 && intEn12)
             || (status13 && intEn13)
             || (status14 && intEn14)
             || (status15 && intEn15)
             || (status16 && intEn16)
             || (status17 && intEn17)
             || (status18 && intEn18)
             || (status19 && intEn19)
             || (status20 && intEn20)
             || (status21 && intEn21)
             || (status22 && intEn22)
             || (status23 && intEn23)
             || (status24 && intEn24)
             || (status25 && intEn25)
             || (status26 && intEn26));
    return temp;
}//}}}

/// Check Tx Stream operation is stop or not when Recieved NACK command
bool HSIF020_Func::IsTxStreamStopWhenRecNACK()
{//{{{
    return (bool)(((*HSIFnSTMD)["STNK"] & 0x1) == 0x0);
}//}}}

/// Check Tx Stream DMA function is enable or not
bool HSIF020_Func::IsTxStreamDMACEnable()
{//{{{
    return (bool)((*HSIFnSTCT)["STDE"] & 0x1);
}//}}}

/// Clear Tx DMA enable bit
void HSIF020_Func::ClearTxDMACBit()
{//{{{
    (*HSIFnSTCT)["STDE"]  = 0x0;
    mHSIF020_AgentController->SetInitialSettingPhase(true);
}//}}}

/// Set Tx Stream status register
void HSIF020_Func::SetTxStreamStatus(uint32_t bit_name)
{//{{{
    switch(bit_name){
        case emSTC : {(*HSIFnSTST)["STC"] = 0x1;  break;}
        case emSTE0: {(*HSIFnSTST)["STE0"] = 0x1; break;}
        case emSTE1: {(*HSIFnSTST)["STE1"] = 0x1; break;}
        case emSTE2: {(*HSIFnSTST)["STE2"] = 0x1; break;}
        case emSTE3: {(*HSIFnSTST)["STE3"] = 0x1; break;}
        case emSTE4: {(*HSIFnSTST)["STE4"] = 0x1; break;}
        default:     { break;}
    }
}//}}}

/// Get Tx Stream Status register (each bit)
bool HSIF020_Func::GetTxStreamStatus(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emSTC : {temp = (bool)((*HSIFnSTST)["STC"]  & 0x1);  break;}
        case emSTE0: {temp = (bool)((*HSIFnSTST)["STE0"] & 0x1); break;}
        case emSTE1: {temp = (bool)((*HSIFnSTST)["STE1"] & 0x1); break;}
        case emSTE2: {temp = (bool)((*HSIFnSTST)["STE2"] & 0x1); break;}
        case emSTE3: {temp = (bool)((*HSIFnSTST)["STE3"] & 0x1); break;}
        case emSTE4: {temp = (bool)((*HSIFnSTST)["STE4"] & 0x1); break;}
        default:     { break;}
    }
    return temp;
}//}}}

/// Get Tx Stream Interruot enable register (each bit)
bool HSIF020_Func::GetTxStreamIntEnable(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emSTCE : {temp = (bool)((*HSIFnSTIE)["STCE"] & 0x1); break; }
        case emSTEE0: {temp = (bool)((*HSIFnSTIE)["STEE0"]& 0x1); break; }
        case emSTEE1: {temp = (bool)((*HSIFnSTIE)["STEE1"]& 0x1); break; }
        case emSTEE2: {temp = (bool)((*HSIFnSTIE)["STEE2"]& 0x1); break; }
        case emSTEE3: {temp = (bool)((*HSIFnSTIE)["STEE3"]& 0x1); break; }
        case emSTEE4: {temp = (bool)((*HSIFnSTIE)["STEE4"]& 0x1); break; }
        default:      { break;}
    }
    return temp;
}//}}}

/// Get Tx Stream Source Address
uint32_t HSIF020_Func::GetTxSourceAddr(void)
{//{{{
    return (uint32_t)(*HSIFnSTSA);
}//}}}

/// Get Tx Stream Byte count
uint32_t HSIF020_Func::GetTxByteCnt(void)
{//{{{
    return (uint32_t)(*HSIFnSTBC);
}//}}}

/// Set Tx Stream Byte count
void HSIF020_Func::SetTxByteCnt(uint32_t byteCountVal)
{//{{{
    (*HSIFnSTBC) = byteCountVal;
}//}}}

/// Check Rx Stream clear the HSIFnSRCT.SRDE bit automatically or not
bool HSIF020_Func::IsRxStreamAutomaticallyClears()
{//{{{
    return (((*HSIFnSRMD)["SRMC"] & 0x1) == 0x0);
}//}}}

/// Clear HSIFnSRCT.SRDE bit
void HSIF020_Func::ClearRxDMACBit()
{//{{{
    (*HSIFnSRCT)["SRDE"]  = 0x0;
    mHSIF020_AgentController->SetInitialSettingPhase(true);
}//}}}

/// Set Rx Stream status register
void HSIF020_Func::SetRxStreamStatus(uint32_t bit_name)
{//{{{
    switch(bit_name){
        case emSRA : {(*HSIFnSRST)["SRA"] = 0x1;  break; }
        case emSRE2: {(*HSIFnSRST)["SRE2"] = 0x1; break; }
        case emSRE1: {(*HSIFnSRST)["SRE1"] = 0x1; break; }
        case emSRE0: {(*HSIFnSRST)["SRE0"] = 0x1; break; }
        case emSRTA: {(*HSIFnSRST)["SRTA"] = 0x1; break; }
        case emSRC1: {(*HSIFnSRST)["SRC1"] = 0x1; break; }
        case emSRC0: {(*HSIFnSRST)["SRC0"] = 0x1; break; }
        default:     { break;}
    }
}//}}}

/// Get Rx Stream Status register
bool HSIF020_Func::GetRxStreamStatus(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emSRA : {temp = (bool)(*HSIFnSRST)["SRA"]  ; break;}
        case emSRE2: {temp = (bool)(*HSIFnSRST)["SRE2"] ; break;}
        case emSRE1: {temp = (bool)(*HSIFnSRST)["SRE1"] ; break;}
        case emSRE0: {temp = (bool)(*HSIFnSRST)["SRE0"] ; break;}
        case emSRC1: {temp = (bool)(*HSIFnSRST)["SRC1"] ; break;}
        case emSRC0: {temp = (bool)(*HSIFnSRST)["SRC0"] ; break;}
        default:     { break;}
    }
    return temp;
}//}}}

/// Clear Rx Stream Status register
void HSIF020_Func::ClearRxStreamStatus(uint32_t bit_name)
{//{{{
    switch(bit_name){
        case emSRTA: {(*HSIFnSRST)["SRTA"] = 0x0; break;}
        case emSRC0: {(*HSIFnSRST)["SRC0"] = 0x0; break;}
        case emSRC1: {(*HSIFnSRST)["SRC1"] = 0x0; break;}
        default:     { break;}
    }
}//}}}

/// Get Config Rx Stream Interrupt Enable
bool HSIF020_Func::GetRxStreamIntEnable(uint32_t bit_name)
{//{{{
    bool temp = false;
    switch(bit_name){
        case emSRAE : {temp =  (bool)((*HSIFnSRIE)["SRAE"]  & 0x1);  break;}
        case emSREE2: {temp =  (bool)((*HSIFnSRIE)["SREE2"] & 0x1); break;}
        case emSREE1: {temp =  (bool)((*HSIFnSRIE)["SREE1"] & 0x1); break;}
        case emSREE0: {temp =  (bool)((*HSIFnSRIE)["SREE0"] & 0x1); break;}
        case emSRCE1: {temp =  (bool)((*HSIFnSRIE)["SRCE1"] & 0x1); break;}
        case emSRCE0: {temp =  (bool)((*HSIFnSRIE)["SRCE0"] & 0x1); break;}
        default:      { break;}
    }
    return temp;
}//}}}

/// Get Rx Destination Start address
uint32_t HSIF020_Func::GetRxDesStartAddr(void)
{//{{{
    return (uint32_t)(*HSIFnSRDA);
}//}}}

/// Get Rx Destination Size
uint32_t HSIF020_Func::GetRxDesSize(void)
{//{{{
    return (uint32_t)(*HSIFnSRDS);
}//}}}

/// Check Rx Stream DMA enable or not
bool HSIF020_Func::IsRxStreamDMACEnable(void)
{//{{{
    return (bool)((*HSIFnSRCT) & 0x1);
}//}}}

/// Get Config Tx Stream data width by register HSIFnSTMD
uint8_t HSIF020_Func::GetTxStreamConfigDataWidth(void)
{//{{{
    return (uint8_t)((*HSIFnSTMD)["STPS"] & 0x1);
}//}}}

/// Tx Stream Handle reply ACK command
void HSIF020_Func::TxStreamHandleACKCmd(void)
{//{{{
    re_printf("info","Handle ACK command in Tx Stream mode!");
    if (!mStreamIgnoreReplyCmdAfterTimeOut){
        if (  (mTxSumDataReadFromMem >= mTxByteNumMaxReadFromMem)
            &&(mHSIF020_AgentController->GetTxStreamOutstandingNumb() == 0)){
            /// Finish transfer all data
            SetTxStreamStatus(emSTC);
            if (GetTxStreamIntEnable(emSTCE)){
                SetInterruptStatus(emSTCS);
            }
        }
        mHSIF020_AgentController->TxContinueGetData2Fifo();
        if (IsTxFifoRemainDataExist() != 0){
            mHSIF020_AgentController->NotifyTxTransStreamCmd();
        } else {
            re_printf("Info","Bit HSIFnSTCT.STDE is automatically cleared when the transfer ends.");
            ClearTxDMACBit();
        }
    } else {
        re_printf("warning","Reply time-out occurred. Ignore ACK reply command for stream in channel %d", 2);
        if (mHSIF020_AgentController->GetTxStreamOutstandingNumb() == 0){
            ClearTxDMACBit();
        } 
    }
}//}}}

/// Update Tx fifo status when Process next data
void HSIF020_Func::UpdateTxFifoStatusWhenProcessNextData(void)
{//{{{
    mTxFifoStatus -= mTxFrameDataSize;
}//}}}

/// Tx Stream Handle NACK Command
void HSIF020_Func::TxStreamHandleNACKCmd(void)
{//{{{
    if (IsTxStreamStopWhenRecNACK()){
        re_printf("info","Handle NACK command in Tx Stream mode. Stop DMA");
        SetTxStreamStatus(emSTE0);
        if (GetTxStreamIntEnable(emSTEE0)){
            SetInterruptStatus(emSTES);
        }
        ClearTxDMACBit();
    } else {
        re_printf("info","Received NACK command. L2 regards reception of the NACK Command as reception of an ACK Command, and  continues the DMA transfer.");
        TxStreamHandleACKCmd();
    }
}//}}}

/// Rx Handle Stream Command
void HSIF020_Func::RxHandleStreamCmd(L2Payload_t iL2Payload, uint8_t iFrameId, uint8_t iPayloadSize)
{//{{{
    /// Handle Data to Buffer
    bool isPassedAuthent = mHSIF020_AgentController->IsPassedAuthent();
    bool isDMAEn         = IsRxStreamDMACEnable();
    bool isFifoNotFull   = false;
    if (mRxFifoStatus <= (HSIF020_ST_DATA_BUF_SIZE - mRxFrameDataSize)) {
        isFifoNotFull    = true;
    }
    bool isNumberStreamCmdNotOverLimit = false;
    if(mRxStreamInputNumb < emOutStandingReqMax) {
        isNumberStreamCmdNotOverLimit  = true;
    } 
    
    if (   isNumberStreamCmdNotOverLimit
        && isPassedAuthent
        && isDMAEn
        && isFifoNotFull){
        bool isDiffSettingAndFrame = false;
        if (iPayloadSize == emPayLoadSize160){
            if (mRxFrameDataSize != 16){
                isDiffSettingAndFrame = true;
            }
        } else if (iPayloadSize == emPayLoadSize288){
            if (mRxFrameDataSize != 32){
                isDiffSettingAndFrame = true;
            }
        }
        if (isDiffSettingAndFrame){
            re_printf("warning", "Setting in HSIFnSRMD.STPS register is different with frame input. Process as setting in register.");
        }
        ///Save to Buffer
        memcpy(&mRxStreamData[mRxFifoWritePointer],&iL2Payload.mPayloadUnit,mRxFrameDataSize);
        mRxFifoStatus += mRxFrameDataSize;
        mRxStreamInputNumb += 1;
        mRxFifoWritePointer +=mRxFrameDataSize;
        if (mRxFifoWritePointer == HSIF020_ST_DATA_BUF_SIZE){
            mRxFifoWritePointer =0;
        }
        mRxStreamCurrentFrameID = iFrameId;
        mHSIF020_AgentController->RxStreamNotifyWriteData2Mem();
    } else {
        if (!isPassedAuthent){
            re_printf("warning","Authentication Fail. Ignore STREAM Command.");
            SetAuthenErrStatus(emAES);
            if (GetAuthErrIntEnable(emAESE)){
                SetInterruptStatus(emAESS);
            }
        } else if (!isDMAEn){
            re_printf("warning","DMAC is not enable. Ignore STREAM Command.");
        } else if (!isNumberStreamCmdNotOverLimit){
            re_printf("warning","Number of received Stream command is over than limited (%d Commands).", emRxReceivedMaxCmd);
            if ((!isFifoNotFull) || (mRxFifoStatus == 0)){
                re_printf("warning","Buffer is full. Ignore command!");
                SetRxStreamStatus(emSRE1);
                if (GetRxStreamIntEnable(emSREE1)){
                    SetInterruptStatus(emSRES);
                }
                RxUpdateSraStatusClearDMA();
                mHSIF020_AgentController->mHSIF020_Unit[emStreamChannel]->TgrConfigNACKCommand();
            } 
        } 
    }
}//}}}

/// Rx Stream Write data to memory via AXI bus
void HSIF020_Func::RxStreamWriteData2Mem()
{//{{{
    if ((mRxFifoStatus >= mRxFrameDataSize)){
        uint32_t startAddr  = GetRxDesStartAddr() + mRxMemWritePointer;
        uint32_t endAddress = startAddr + mRxFrameDataSize;

        bool isHitMemWindow  = mHSIF020_AgentController->IsHitMemWindow(startAddr, endAddress, true);

        if (   isHitMemWindow ){
            ConfigTransaction(true
                             ,startAddr
                             ,mRxFrameDataSize
                             ,false
                             ,2);
            mRxMemWritePointer += mRxFrameDataSize;
            if (mRxMemWritePointer >= (GetRxDesSize() | 0xF)){
                mRxMemWritePointer = 0;
                if  (mRxMemWritePointerToggleBit)
                    mRxMemWritePointerToggleBit = false;
                else
                    mRxMemWritePointerToggleBit = true;
            }
        } else {
            SetRxStreamStatus(emSRE2);
            if (GetRxStreamIntEnable(emSREE2)){
                SetInterruptStatus(emSRES);
            }
            SetAuthenErrStatus(emWES1);
            if (GetAuthErrIntEnable(emWESE1)){
                SetInterruptStatus(emAESS);
            }
            RxUpdateSraStatusClearDMA();
        }
    }
}//}}}

/// Tx Stream Get data form Tx FIFO and put them to L2Payload to prepare sending data
L2Payload_t HSIF020_Func::TxStreamGetL2PayloadData()
{//{{{
    L2Payload_t temp;
    uint32_t tempData[8];
    memset(tempData,0x0,32);
    memcpy(&tempData,&(mTxStreamData[mTxFifoReadPointer]),mTxFrameDataSize);
    temp = L2Payload_t(tempData[0],tempData[1],tempData[2],tempData[3]
                      ,tempData[4],tempData[5],tempData[6],tempData[7]);
    mTxFifoReadPointer += mTxFrameDataSize;
    if (mTxFifoReadPointer == HSIF020_ST_DATA_BUF_SIZE)
        mTxFifoReadPointer = 0;
    return temp;
}//}}}

/// Check Fifo is have remain data or not
uint32_t HSIF020_Func::IsTxFifoRemainDataExist(void)
{//{{{
    bool isTxFifoRemainData = (mTxFifoStatus > 0) && (mTxFifoStatus%mTxFrameDataSize == 0);

    return (uint32_t)isTxFifoRemainData;
}//}}}


/// Tx Stream Operation: Handle Time Out event occurs
void HSIF020_Func::HandleTimeOutStreamOccurs(void)
{//{{{
    SetTxStreamStatus(emSTE1);
    if (GetTxStreamIntEnable(emSTEE1)){
        SetInterruptStatus(emSTES);
    }
    if (IsTxStreamDMACEnable()){
        ClearTxDMACBit();
    }
    mStreamIgnoreReplyCmdAfterTimeOut = true;
}//}}}

/// Tx Stream Get data from Memory to Fifo
void HSIF020_Func::TxStreamGetData2Fifo()
{//{{{
    uint32_t sourceAddr         = GetTxSourceAddr() + mTxMemReadPointer;
    uint32_t readSizeBytes      = 32;       /// 4 burst

    bool isHitMemWindow         = IsMemWindowAccessPer(sourceAddr
                                                     , sourceAddr + readSizeBytes
                                                     , false);
    if(  (mTxAXIOutstandReq < emOutStandingReqMax)
      && IsTxFifoCanStoreMoreData()
      && IsTxMemoryRemainDataExist()){
        if(isHitMemWindow){
            ConfigTransaction(false
                            , sourceAddr
                            , readSizeBytes
                            , false
                            , 2);
            mTxMemReadPointer += 32;
            if (mTxByteNumMaxReadFromMem < mTxMemReadPointer){
                SetTxByteCnt(0x0);
            } else {
                SetTxByteCnt(mTxByteNumMaxReadFromMem - mTxMemReadPointer);
            }
        } else {
            /// Not hit memory window
            /// Set Error Flag (HSIFnSTST.STE4=1)
            SetTxStreamStatus(emSTE4);
            if (GetTxStreamIntEnable(emSTEE4) == 0x1){
                SetInterruptStatus(emSTES);
            } 
            SetAuthenErrStatus(emWES0);
            if (GetAuthErrIntEnable(emWESE0)){
                SetInterruptStatus(emAESS);
            }
            /// Clear DMA enable (HSIFnSTCT.STDE=0)
            ClearTxDMACBit();
        }
    }
}//}}}

/// Check  Tx Fifo can store more data or not
bool HSIF020_Func::IsTxFifoCanStoreMoreData()
{//{{{
    bool isCan = false;
    if ((mTxFifoStatus < HSIF020_ST_DATA_BUF_SIZE)
      &&((mTxFifoStatus % 32) == 0)){
        isCan = true;
    }
    return isCan;
}//}}}

/// Check Memory in Tx remain data (to transfer) or not
bool HSIF020_Func::IsTxMemoryRemainDataExist()
{//{{{
    if (GetTxByteCnt() > 0){
        return true;
    } else {
        return false;
    }
}//}}}

/// Update Write pointer register
void HSIF020_Func::UpdateWritePointer()
{//{{{
    (*HSIFnSRWP)["SRWP"] = (mRxMemWritePointer>>4) & 0x1FFFF;
    (*HSIFnSRWP)["SRWT"] = mRxMemWritePointerToggleBit;
}//}}}

/// Update Status SRST register , Issue related interrupt
void HSIF020_Func::UpdateCompleteFlag()
{//{{{
    //when the size of unprocessed data (in the received Stream Command L2 is
    //operating as a target node) is not 0.
    if( (mRxMemWritePointer != mRxMemReadPointer)
      ||(mRxMemWritePointerToggleBit != mRxMemReadPointerToggleBit)){
        if(!GetRxStreamStatus(emSRC0)){
            SetRxStreamStatus(emSRC0);
            if (GetRxStreamIntEnable(emSRCE0)){
                SetInterruptStatus(emSRCS0);
            }
        }
    }

    bool isSetSRC1 = false;
    if (mRxMemWritePointerToggleBit == mRxMemReadPointerToggleBit){
        if (mRxMemWritePointer > mRxMemReadPointer){
            if ((mRxMemWritePointer - mRxMemReadPointer) >= mRxMaxSizeUnprocessedData){
                isSetSRC1 = true;
            } else {
                isSetSRC1 = false;
            }
        } else {
            isSetSRC1 = false;
        }
    } else {
        /// Size of un-process data
        uint32_t UnProcessedData = 0;
        /// Data from Read Pointer to end memory (HSIFnSRDS)
        UnProcessedData = ((GetRxDesSize() | 0xF) + 0x1) - mRxMemReadPointer;
        /// Data from start memory (HSIFnSRDA)to Write Pointer
        UnProcessedData += mRxMemWritePointer;
        if (UnProcessedData >= mRxMaxSizeUnprocessedData){
            isSetSRC1 = true;
        } else {
            isSetSRC1 = false;
        }
    }
    if (isSetSRC1){
        if (!GetRxStreamStatus(emSRC1)){
            SetRxStreamStatus(emSRC1);
            if (GetRxStreamIntEnable(emSRCE1)){
                SetInterruptStatus(emSRCS1);
            }
        }
    } else {
        if (GetRxStreamStatus(emSRC1)){
            ClearRxStreamStatus(emSRC1);
        }
    }

    // Has no unprocessed data
    if (   mRxMemWritePointer == mRxMemReadPointer
        &&(mRxMemWritePointerToggleBit == mRxMemReadPointerToggleBit)){
        ClearRxStreamStatus(emSRC0);
        ClearRxStreamStatus(emSRC1);
    }
}//}}}

/// Handle CRC error in Stream commands
void HSIF020_Func::HandleCrcErrorStreamCmd(void)
{//{{{
    mHSIF020_AgentController->mHSIF020_Unit[emStreamChannel]->mHSIF020_UNIT_Func->SetChannelStatus(emCRE);
    if (mHSIF020_AgentController->mHSIF020_Unit[emStreamChannel]->mHSIF020_UNIT_Func->GetChIntEnable(emCREE)){
        SetInterruptStatus(emCERS2);
    }
    if (IsRxStreamDMACEnable()){
        RxUpdateSraStatusClearDMA();
    }
    if (IsTxStreamDMACEnable()){
        ClearTxDMACBit();
        mHSIF020_AgentController->TxStreamStopTimeOutEvent();
    }
}//}}}

/// Handle Transaction ID error in case of Stream command
void HSIF020_Func::HandleTIDErrorStreamCmd(void)
{//{{{
    SetTxStreamStatus(emSTE2);
    if (GetTxStreamIntEnable(emSTEE2)){
        SetInterruptStatus(emSTES);
    }
    ClearTxDMACBit();

    mHSIF020_AgentController->TxStreamStopTimeOutEvent();
}//}}}

/// Handle AXI error in case of Stream command - Rx operation
void HSIF020_Func::HandleRxAxiErrorStreamCmd(void)
{//{{{
    SetRxStreamStatus(emSRE0);
    if (GetRxStreamIntEnable(emSREE0)){
        SetInterruptStatus(emSRES);
    }
    re_printf("warning","Transmission Stream: An error is detected in the response from the AXI.");
    RxUpdateSraStatusClearDMA();
}//}}}

/// Handle AXI error in case of Stream command - Tx Operation
void HSIF020_Func::HandleTxAxiErrorStreamCmd(void)
{//{{{
    mTxAXIOutstandReq -= 1;
    if (mTxAXIOutstandReq == 0){
        SetTxStreamStatus(emSTE3);
        if (GetTxStreamIntEnable(emSTEE3)){
            SetInterruptStatus(emSTES);
        }
        ClearTxDMACBit();
        re_printf("warning","Transmission Stream: An error is detected in the response from the AXI. Terminate DMA.");
    }

    mHSIF020_AgentController->TxStreamStopTimeOutEvent();
}//}}}

/// Update SRTS.SRA status register and Clear DMA if need
void HSIF020_Func::RxUpdateSraStatusClearDMA(void)
{//{{{
    if (IsRxStreamDMACEnable() && IsRxStreamAutomaticallyClears())
    {
        ClearRxDMACBit();
        SetRxStreamStatus(emSRA);
        if (GetRxStreamIntEnable(emSRAE)){
            SetInterruptStatus(emSRES);
        }
    }
}//}}}

/// Get number of AXI outstanding request (Tx operation)
uint8_t HSIF020_Func::GetTxAXIOutstandReq(void)
{//{{{
    return mTxAXIOutstandReq;
}//}}}

/// Get number of AXI outstanding request (Rx operation)
uint8_t HSIF020_Func::GetRxAXIOutstandReq(void)
{//{{{
    return mRxAXIOutstandReq;
}//}}}

/// Get HSIFnMST register status
uint32_t HSIF020_Func::GetHSIFnMST (void)
{//{{{
    return (uint32_t)(*HSIFnMST);
}//}}}

/// Get HSIFnMIST register status
uint32_t HSIF020_Func::GetHSIFnMIST(void)
{//{{{
    return (uint32_t)(*HSIFnMIST);
}//}}}

/// Get HSIFnSTST register status
uint32_t HSIF020_Func::GetHSIFnSTST(void)
{//{{{
    return (uint32_t)(*HSIFnSTST);
}//}}}

/// Get HSIFnSRST register status
uint32_t HSIF020_Func::GetHSIFnSRST(void)
{//{{{
    return (uint32_t)(*HSIFnSRST);
}//}}}

/// Get HSIFnAEST register status
uint32_t HSIF020_Func::GetHSIFnAEST(void)
{//{{{
    return (uint32_t)(*HSIFnAEST);
}//}}}

// vim600: set foldmethod=marker :
