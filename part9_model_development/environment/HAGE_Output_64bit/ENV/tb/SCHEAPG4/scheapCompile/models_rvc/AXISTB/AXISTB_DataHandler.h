// ---------------------------------------------------------------------
// $Id: AXISTB_DataHandler.h,v 1.3 2014/05/05 01:23:39 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __AXISTB_DataHandler_H__
#define __AXISTB_DataHandler_H__
#include "BusMasterIf.h"
#include "BusBridgeCoreBase.h"
#include "AXISTB.h"

class AXISTB_DataHandler: public BusBridgeCoreBase<64,64>
{
private:
    AXISTB_AgentController *mAXISTBAgentController;
    BusMasterIf<64> *mMasterIf;
    BusBridgeSlaveIf<64> *mSlaveIf;
    std::string mSpecifiedName; //GRI or SIC block
    std::map<TlmBasicPayload*,TlmBasicPayload*> mRequestMap; //Bus request map for AT mode
    std::queue<TlmBasicPayload *>  mClearTransFifo; //Clear transaction list for AT mode

public:
    TlmBasicPayload* mTrans;
    BusTime_t mTransTime;

public:
    AXISTB_DataHandler(std::string name,
                      std::string specified_name,
                      BusBridgeModuleBase<64,64> *Parent, 
                      BusBridgeSlaveIf<64> *SlaveIf,
                      BusMasterIf<64> *MasterIf,
                      AXISTB_AgentController *AgentController_ptr);
    virtual ~AXISTB_DataHandler(){;}
    void TransferData(const bool is_write, TlmBasicPayload *trans, BusTime_t* t, bool debug);
    void CancelTransferring(void);

private:
    void SetTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTransG3m(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTransAxi(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);
};
#endif
