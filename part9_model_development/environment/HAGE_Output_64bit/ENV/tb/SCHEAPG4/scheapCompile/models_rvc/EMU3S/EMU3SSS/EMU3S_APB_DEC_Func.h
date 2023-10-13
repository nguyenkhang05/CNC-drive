// ---------------------------------------------------------------------
// $Id: EMU3S_APB_DEC_Func.h,v 1.5 2020/11/18 18:52:19 thiepnguyen Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __EMU3S_APB_DEC_FUNC_H__
#define __EMU3S_APB_DEC_FUNC_H__
#include "BusBridgeCoreBase.h"

class EMU3S_APB_DEC_Func :
    public BusBridgeCoreBase<BUS_WIDTH_APB,BUS_WIDTH_APB>
{
    public:
    EMU3S_APB_DEC_Func(std::string name,
            BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB> *Parent,
            BusBridgeSlaveIf<BUS_WIDTH_APB> *SlaveIf,
            BusMasterIf<BUS_WIDTH_APB> *MasterIf,
            SIM_MODE_T simmode);
    virtual ~EMU3S_APB_DEC_Func();

    void EnableReset (const bool is_active);                            //Enable/Disable reset state

    bool                                        mis_bus_busy;
    bool                                        mbus_stop_ack;

    sc_event                                    m_bus_competion_event;  //Event notify transaction competion
    
    private:
    BusMasterIf<BUS_WIDTH_APB>                  *mMasterIf;
    BusBridgeSlaveIf<BUS_WIDTH_APB>             *mSlaveIf;

    std::map<TlmBasicPayload*,TlmBasicPayload*> mRequestMap;            //Bus request map for AT mode
    std::queue<TlmBasicPayload *>               mClearTransFifo;        //Clear transaction list for AT mode
    TlmBasicPayload*                            mTrans;                 //Transaction info
    BusTime_t                                   mTransTime;             //Internal time
    bool                                        mIsDebug;               //Is debug transaction

    void TransferComplete(void);
    void setTransBasicEmu3s(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void setTransApb(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void setTransG3mEmu3s(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void CancelEvents (void);
    void Initialize (void);

    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);
};
#endif //__EMU3S_APB_DEC_FUNC_H__
