// ---------------------------------------------------------------------
// $Id: ApgDataHandler.h,v 1.2 2014/04/11 05:44:31 duymai Exp $
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

#ifndef __APG_DATA_HANDLER_H__
#define __APG_DATA_HANDLER_H__
#include "BusMasterIf.h"
#include "BusBridgeCoreBase.h"
#include "CmErrMsg.h" // for WinMsg
#include "Common.h"
#include "APG.h"

class ApgDataHandler : public BusBridgeCoreBase<32,32>
{
public:
    ApgDataHandler(std::string name,
                   BusBridgeModuleBase<32,32> *Parent, 
                   BusBridgeSlaveIf<32> *SlaveIf,
                   BusMasterIf<32> *MasterIf,
                   ApgAgentController *AgentController_ptr);
    virtual ~ApgDataHandler(){;}

    void TransferComplete(void);
private:
    ApgAgentController *mAgentController;
    BusMasterIf<32> *mMasterIf;
    BusBridgeSlaveIf<32> *mSlaveIf;
    std::map<TlmBasicPayload*,TlmBasicPayload*> mRequestMap; //Bus request map for AT mode
    std::queue<TlmBasicPayload *>  mClearTransFifo; //Clear transaction list for AT mode
    TlmBasicPayload* mTrans;//Transaction info
    BusTime_t mTransTime;//Internal time
    bool mIsDebug;//Is debug transaction

    void SetTransBasic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTransG3m(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTransApb(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);
};
#endif
