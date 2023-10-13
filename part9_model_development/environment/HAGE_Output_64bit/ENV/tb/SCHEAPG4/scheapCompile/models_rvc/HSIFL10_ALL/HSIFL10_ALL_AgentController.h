// ---------------------------------------------------------------------
// $Id: HSIFL10_ALL_AgentController.h,v 1.7 2020/04/08 07:44:03 quynhnhunguyen Exp $
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

#ifndef __HSIFL10_ALL_AGENT_H__
#define __HSIFL10_ALL_AGENT_H__

#define  INVALID_ICLC_COMMAND   0xEF
#define  FRAME_SIZE_MIN         0x21
#define  BAUD_RATE_SLOW_MODE    (5*(sc_dt::uint64)(1.0e+6))
#define  RX_FIFO_SIZE_BIT_MAX   0x250
#define  SIZE_SYNCCODE_ENDBIT   0x11
#define  PAYLOAD_CTS_FRAME      0x00
#define  TX_FIFO_SIZE_BYTE      0x4        //32bit
#define  INVALID_RX_CONTROL     0x600 

class HSIFL10_ALL;

class HSIFL10_ALL_AgentController {
#include "HSIFL10_ALL_cmdif.h"

public:
    enum eModelType {
        emL1=0,
        emL2,
        emNoneModel,
        emModelNum
    };
    enum eFrameType {
        emICLCFramePingAns=0,
        emCTSFrameL1,
        emICLCFrameL1,
        emFrameL2,
        emInValidFrame,
        emFrameNumber
    };
    enum eLogicChanType {
        emICLC=0,
        emCTS,
        emChanA,
        emChanB,
        emChanC,
        emChanD,
        emChanInvalid,
        emLogicChanTypeNum
    };
    enum eErrorType {
        emErrCommand = 0,
        emErrLogicChanType,
        emErrPayloadSizeIndex,
        emErrPayloadSize,
        emErrorTypeNum
    };
    enum eTransDataControl {
        emTransIdle = 0,
        emTransStart,
        emTransEnd,
        emTranAbort,
        emTransDataControlNum
    };
    enum ePLLMode {
        emPrePLLON=0,
        emPLLON,
        emPrePLLOFF,
        emPLLOFF,
        emPLLINVALID,
        emPLLModeNum
    };
    enum eFlowControlManualTpye {
        emAutoFCType = 0,
        emFCMTypeWithCTSFrame,
        emFCMTypeWithoutCTSFrame,
        emFCMTypeInvalid,
        emFlowControlManualTypeNum
    };
public:
    HSIFL10_ALL_AgentController(void) {;}
    virtual ~HSIFL10_ALL_AgentController(void) {;}
    
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual bool GetResetStatus() = 0;
    virtual void TransmitterCreateFrameL1(uint32_t ICLC_val, eModelType modelType, eLogicChanType logicChanType) = 0;
    virtual void TransmitterTransProcess() = 0;
    virtual void TransProcessMethod() = 0;
    virtual void StoreFrameTypeIntoBuffer(eFrameType frameType) = 0;
    virtual bool CheckExistPreFrameTypeBuffer(eFrameType frameType) = 0;
    virtual bool CheckFreeTransmission() = 0;
    virtual void SetFlowControlManualType(eFlowControlManualTpye flowControlManualType) = 0;
    virtual void SetRefCLKOut(sc_dt::uint64 refCLKOut) = 0;
    virtual sc_dt::uint64 GetCCLKFreq() = 0; 
    virtual sc_dt::uint64 GetRefclk_inFreq() = 0;
    virtual std::string GetVersionName() = 0;
    virtual void SetLoopBackMode(bool isLoopBack) = 0;
    virtual bool GetTxDataReady() = 0;
    virtual void SetTxDataReady(bool isTxDataReady) = 0;
    virtual void ClearInterrupt(std::string signal_name) = 0;
};
#endif//__HSIFL10_ALL_AGENT_H__
