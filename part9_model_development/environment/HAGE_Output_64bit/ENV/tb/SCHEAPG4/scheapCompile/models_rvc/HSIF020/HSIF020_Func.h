// ---------------------------------------------------------------------
// $Id: HSIF020_Func.h,v 1.7 2020/04/08 07:39:39 quynhnhunguyen Exp $
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
#ifndef __HSIF020_FUNC_H__
#define __HSIF020_FUNC_H__

#include "HSIF020.h"
#include "hsif020_regif.h"
#include "BusSlaveFuncIf.h"
#include "BusMasterFuncIf.h"

class HSIF020_AgentController;

class HSIF020_Func:  public Chsif020_regif,
                     public BusSlaveFuncIf,
                     public BusMasterFuncIf
{
public:
    HSIF020_Func (std::string iname
                , HSIF020_AgentController *iHSIF020_AgentControllerPtr
                , BusMasterIf<64> *MasterIf_is_ptr
                , SIM_MODE_T simMode);
    ~HSIF020_Func (void);

    void RegisterHandler (const std::vector<std::string> &args);

    /// Virtual functions of BusSlaveFuncIf class
    void read (uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);
    void write (uint32_t offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug);

    ///For master - virtual functions in BusMasterFuncIf
    void notifyTransSend(TlmTransactionType &trans, BusTime_t &t);
    void notifyTransComp(TlmTransactionType &trans, BusTime_t &t);

    /// Public functions
    void     Reset (const bool is_active);      ///Reset operation of HSIF020_Func class
    void     Initialize(void);                  /// Initialize all internal variable
    uint32_t GetTimeOutSetting(void);           /// Get Time-out setting
    bool     isAllChannelDisable(void);         /// Check all channel (0 - 1 - 2 -3) is disable or not
    bool     isWriteByThisIP(void);             /// Check Transaction is write by This IP or not
    bool     isWriteByICUModule(void);          /// Check Transaction is write by ICU module or not
    bool     GetTableSetting(void);             /// Get setting specifies the type of the channel number code table to be used for this IP
    void     SetAuthenIdBaseKey(uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3      /// Set the authentication ID base
                          , uint32_t id4, uint32_t id5, uint32_t id6, uint32_t id7);
    ///Configure AXI Transaction
    void     ConfigTransaction(const bool icommand      ///Configure AXI Transaction
                             , const uint32_t iaddr
                             , const uint8_t isize
                             , const bool idebug
                             , const ChId_t iChId);
    void     TransProcess(void);                        /// Function processing transaction to AXI bus
    void     ReceiveRD_DATA(TlmTransactionType *trans, ChId_t chId);    /// Handle Data reply from AXI bus
    void     DeleteTrans(TlmTransactionType *trans);    /// Delete Transaction after processing
    bool     IsMemWindowAccessPer(uint32_t startAdrress, uint32_t endAddress, bool command);    /// Check a address range is permitted access via Memory window or not
    bool     IsAccessToAuthentRegs(uint32_t startAddress);  /// Check READ/WRITE access to authentication register
    void     SetAuthenErrStatus (uint32_t bit_name);    /// Set authentication error status register
    bool     GetAuthenErrStatus (uint32_t bit_name);    /// Get authentication error status
    bool     GetAuthErrIntEnable (uint32_t bit_name);   /// Get authentication interrupt enable
    void     SetInterruptStatus (uint32_t bit_name);    /// Set interrupt status register (HSIFnMIST)
    bool     ComputeMIST_RARS_Status(uint8_t ch_id);    /// Compute Interrupt status bit MIST
    bool     ComputeMIST_AKRS_Status(uint8_t ch_id);    /// Compute Interrupt status bit MIST
    bool     ComputeMIST_TERS_Status(uint8_t ch_id);    /// Compute Interrupt status bit MIST
    bool     ComputeMIST_STCS_Status(void);             /// Compute Interrupt status bit MIST
    bool     ComputeMIST_SRCS0_Status(void);            /// Compute Interrupt status bit MIST
    bool     ComputeMIST_SRCS1_Status(void);            /// Compute Interrupt status bit MIST
    bool     ComputeMIST_CERS_Status(uint8_t ch_id);    /// Compute Interrupt status bit MIST
    bool     ComputeMIST_STES_Status(void);             /// Compute Interrupt status bit MIST
    bool     ComputeMIST_SRES_Status(void);             /// Compute Interrupt status bit MIST
    bool     ComputeMIST_AESS_Status(void);             /// Compute Interrupt status bit MIST

    bool     IsTxStreamStopWhenRecNACK(void);       /// Check Tx Stream operation is stop or not when Recieved NACK command
    bool     IsTxStreamDMACEnable(void);            /// Check Tx Stream DMA function is enable or not
    void     ClearTxDMACBit(void);                  /// Clear Tx DMA enable bit
    void     SetTxStreamStatus(uint32_t bit_name);  /// Set Tx Stream status register
    bool     GetTxStreamStatus(uint32_t bit_name);  /// Get Tx Stream Status register (each bit)
    bool     GetTxStreamIntEnable(uint32_t bit_name);   /// Get Tx Stream Interruot enable register (each bit)
    void     SetTxStreamByteCount(uint32_t bit_name);   /// Get Tx Stream Interruot enable register (each bit)
    uint32_t GetTxSourceAddr(void);                     /// Get Tx Stream Source Address
    uint32_t GetTxByteCnt(void);                        /// Get Tx Stream Byte count
    void     SetTxByteCnt(uint32_t byteCountVal);       /// Set Tx Stream Byte count
    void     TxStreamGetData2Fifo(void);                /// Tx Stream Get data from Memory to Fifo
    void     UpdateTxFifoStatusWhenProcessNextData(void);   /// Update Tx fifo status when Process next data
    bool     IsTxFifoCanStoreMoreData(void);            /// Check  Tx Fifo can store more data or not
    uint32_t IsTxFifoRemainDataExist(void);             /// Check Fifo is have remain data or not
    bool     IsTxMemoryRemainDataExist(void);           /// Check Memory in Tx remain data (to transfer) or not
    uint8_t  GetTxStreamConfigDataWidth(void);          /// Get Config Tx Stream data width by register HSIFnSTMD
    L2Payload_t TxStreamGetL2PayloadData();             /// Tx Stream Get data form Tx FIFO and put them to L2Payload to prepare sending data

    bool     IsRxStreamAutomaticallyClears(void);       /// Check Rx Stream clear the HSIFnSRCT.SRDE bit automatically or not
    void     ClearRxDMACBit(void);                      /// Clear HSIFnSRCT.SRDE bit
    void     SetRxStreamStatus(uint32_t bit_name);      /// Set Rx Stream status register
    bool     GetRxStreamStatus(uint32_t bit_name);      /// Get Rx Stream Status register
    void     ClearRxStreamStatus(uint32_t bit_name);    /// Clear Rx Stream Status register
    bool     GetRxStreamIntEnable(uint32_t bit_name);   /// Get Config Rx Stream Interrupt Enable
    bool     IsRxStreamDMACEnable(void);                /// Check Rx Stream DMA enable or not
    uint32_t GetRxDesSize(void);                        /// Get Rx Destination Size
    uint32_t GetRxDesStartAddr(void);                   /// Get Rx Destination Start address
    void     RxStreamWriteData2Mem(void);               /// Rx Stream Write data to memory via AXI bus

    void     TxStreamHandleACKCmd(void);                /// Tx Stream Handle reply ACK command
    void     TxStreamHandleNACKCmd(void);               /// Tx Stream Handle NACK Command
    void     RxHandleStreamCmd(L2Payload_t iL2Payload, uint8_t iFrameId, uint8_t iPayloadSize); /// Rx Handle Stream Command

    void     HandleTimeOutStreamOccurs(void);           /// Tx Stream Operation: Handle Time Out event occurs
    void     UpdateWritePointer(void);                  /// Update Write pointer register
    void     UpdateCompleteFlag(void);                  /// Update Status SRST register , Issue related interrupt
    void     RxUpdateSraStatusClearDMA(void);           /// Update SRTS.SRA status register and Clear DMA if need
    void     SetModelSpid(uint8_t iSpid);               /// Set this IP SPID value
    void     SetModelPeid(uint8_t iPeid);               /// Set this IP PEID value

    void     HandleCrcErrorStreamCmd(void);             /// Handle CRC error in Stream commands
    void     HandleTIDErrorStreamCmd(void);             /// Handle Transaction ID error in case of Stream command
    void     HandleRxAxiErrorStreamCmd(void);           /// Handle AXI error in case of Stream command - Rx operation
    void     HandleTxAxiErrorStreamCmd(void);           /// Handle AXI error in case of Stream command - Tx Operation
    uint8_t  GetTxAXIOutstandReq(void);                 /// Get number of AXI outstanding request (Tx operation)
    uint8_t  GetRxAXIOutstandReq(void);                 /// Get number of AXI outstanding request (Rx operation)

    /// Get register status
    uint32_t GetHSIFnMST (void);                        /// Get register status
    uint32_t GetHSIFnMIST(void);
    uint32_t GetHSIFnSTST(void);
    uint32_t GetHSIFnSRST(void);
    uint32_t GetHSIFnAEST(void);
private:
    HSIF020_AgentController         *mHSIF020_AgentController;
    BusMasterIf<64>*                mMasterIf_is;                   ///< Bus master interface pointer
    std::string                     mModuleName;                    ///< This module name
    uint8_t                         mTransInfoSPID;                 ///< Transaction ID of current transaction to register block
    uint32_t                        mAXITransId;                    ///< Current AXI Transaction ID

    TransIdInfoSet_t                mTransIdInfoSet;                ///< Store AXI transaction information
    TransIdInfoMap_t                mTransIdInfoMap;                ///< Store transaction id information of stream command and it's AXI transaction id

    uint32_t                        mAuthenIdBaseKey[emAuthIDbaseRegNumbs]; ///< Store authentication id base case
    uint8_t                         mTimeOfAuthenIdBase;                    ///< Store number of time authentication ID base
    uint8_t                         mTimeOfAuthenCR;                        ///< Store number of time authentication CR base

    MemoryWindow_t                  mWindowMem[emMemWindowNumbs];           ///< Store memory windows value
    WindowLabel_t                   mWindowLabel[emMemWindowNumbs];         ///< Store windows label
    RegsAuthenAddrSet_t             mRegsAuthenAddrSet;                     ///Contain address of registers of authentication mode

    std::queue<TlmBasicPayload *>   mRequestFifo;               ///Bus request FIFO
    BusTime_t                       t;                          ///sc_time

    uint8_t                         spid;                       ///< spid of this model
    uint8_t                         tcid;                       ///< tcid of this model
    uint8_t                         vcid;                       ///< vcid of this model
    uint8_t                         peid;                       ///< peid of this model
    bool                            um;                         ///< user mode
    bool                            vm;
    bool                            mSecure;                    
    SIM_MODE_T                      mSimMode;                   ///< Simulation mode

    /// Stream command
    uint8_t                        mTxStreamData[HSIF020_ST_DATA_BUF_SIZE];     ///Buffer for Tx stream
    uint8_t                        mRxStreamData[HSIF020_ST_DATA_BUF_SIZE];     ///Buffer for Rx stream
                                                                                ///Save data from memory and transmit stream command
    uint32_t                       mTxFifoStatus;               ///<Tx Fifo status (number data remain in Tx FIFO)
    uint32_t                       mTxFifoWritePointer;         ///<Tx Fifo Write pointer value
    uint32_t                       mTxFifoReadPointer;          ///<Tx Fifo Read pointer value

    uint32_t                       mRxFifoStatus;               ///<Rx Fifo status (number data remain in Rx FIFO)
    uint32_t                       mRxFifoWritePointer;         ///<Rx Fifo Write pointer value
    uint32_t                       mRxFifoReadPointer;          ///<Rx Fifo Read pointer value
    uint8_t                        mRxStreamCurrentFrameID;     ///<Current Frame id (transaction id) of Rx Stream command

    uint8_t                        mTxAXIOutstandReq;           ///< Number of Tx AXI outstanding request to bus
    uint8_t                        mRxAXIOutstandReq;           ///< Number of Rx AXI outstanding request to bus
    uint8_t                        mRxStreamInputNumb;          ///< Number of input Rx Stream command

    uint32_t                       mTxMemReadPointer;           ///< Tx Memory read pointer

    uint32_t                       mRxMemWritePointer;          ///< Rx Memory write pointer
    bool                           mRxMemWritePointerToggleBit; ///< Rx Memory write pointer toggle bit

    uint32_t                       mRxMemReadPointer;           ///< Rx Memory read pointer
    bool                           mRxMemReadPointerToggleBit;  ///< Rx Memory read pointer toggle bit

    uint32_t                       mRxSizeUnprocessedData;      ///< Size of unprocessed data in byte on Rx memory
    uint32_t                       mRxMaxSizeUnprocessedData;   ///< Max Size of unprocessed data in byte on Rx memory

    uint32_t                       mRxSumDataWriteToMem;        ///< Number of data in byte wrote to Rx Memory
    uint32_t                       mRxFrameDataSize;            ///< Rx Frame data size
    uint32_t                       mTxFrameDataSize;            ///< Tx Frame data size (setting in bit )
    uint32_t                       mTxByteNumMaxReadFromMem;    ///< Number of data in byte on Tx memory which read to transfer to Rx
    uint32_t                       mTxSumDataReadFromMem;       ///< Size of data in byte which read from Tx memory
    bool                           mStreamIgnoreReplyCmdAfterTimeOut;   ///< Ignore reply command after time out occur or not
    /// Declare virtual functions of HSIF020_regif class
    void cb_HSIFnMMD_CHCT(RegCBstr str);
    void cb_HSIFnMCT_WDWE(RegCBstr str);
    void cb_HSIFnMST_AUTS1(RegCBstr str);
    void cb_HSIFnMRT_RSCL(RegCBstr str);
    void cb_HSIFnMIST_AESS(RegCBstr str);
    void cb_HSIFnMWmA_MWmA(RegCBstr str);
    void cb_HSIFnMWmS_MWmS(RegCBstr str);
    void cb_HSIFnSTMD_STNK(RegCBstr str);
    void cb_HSIFnSTCT_STDE(RegCBstr str);
    void cb_HSIFnSTSC_STEC4(RegCBstr str);
    void cb_HSIFnSTIE_STEE4(RegCBstr str);
    void cb_HSIFnSTSA_STSA(RegCBstr str);
    void cb_HSIFnSTBC_STBC(RegCBstr str);
    void cb_HSIFnSRMD_SRMC(RegCBstr str);
    void cb_HSIFnSRCT_SRDE(RegCBstr str);
    void cb_HSIFnSRST_SRA(RegCBstr str);
    void cb_HSIFnSRSC_SRAC(RegCBstr str);
    void cb_HSIFnSRIE_SRAE(RegCBstr str);
    void cb_HSIFnSRDA_SRDA(RegCBstr str);
    void cb_HSIFnSRDS_SRDS(RegCBstr str);
    void cb_HSIFnSRBC_SRBC(RegCBstr str);
    void cb_HSIFnSRWP_SRWP(RegCBstr str);
    void cb_HSIFnSRRP_SRRP(RegCBstr str);
    void cb_HSIFnAID_AID(RegCBstr str);
    void cb_HSIFnAAD_AAD(RegCBstr str);
    void cb_HSIFnARD_ARD(RegCBstr str);
    void cb_HSIFnAESC_WESC1(RegCBstr str);
};

#endif /*__HSIF020_FUNC_H__*/
