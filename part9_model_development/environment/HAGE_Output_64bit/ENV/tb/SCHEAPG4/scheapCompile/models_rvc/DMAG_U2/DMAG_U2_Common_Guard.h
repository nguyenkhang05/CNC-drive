// ---------------------------------------------------------------------
// $Id: DMAG_U2_Common_Guard.h,v 1.4 2020/02/14 06:25:20 hadinhtran Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __DMAG_U2_COMMON_GUARD_H__
#define __DMAG_U2_COMMON_GUARD_H__
#include "BusBridgeCoreBase.h"
#include "global.h"

class DMAG_U2_AgentController;
class DMAG_U2_Common_Guard : //public sc_module,
                     public BusBridgeCoreBase<32,32>
{
public:
    DMAG_U2_Common_Guard(std::string name,
                   DMAG_U2_AgentController* DMAG_U2_AgentControllerPtr,
                   BusBridgeModuleBase<32,32> *Parent,
                   BusBridgeSlaveIf<32> *SlaveIf,
                   BusMasterIf<32> *MasterIf,
                   SIM_MODE_T simmode);
    virtual ~DMAG_U2_Common_Guard(){;}

    void EnableReset (const bool is_active);                                            //Enable/Disable reset state
    bool SetAreaAddr(const unsigned int ch_id, const unsigned int start_addr, const unsigned int size);
    unsigned int GetVAR(const std::string name);

private:
    DMAG_U2_AgentController *mDMAG_U2_AgentController;
    BusMasterIf<32> *mMasterIf;
    BusBridgeSlaveIf<32> *mSlaveIf;
    std::map<TlmBasicPayload*,TlmBasicPayload*> mRequestMap; //Bus request map for AT mode
    std::queue<TlmBasicPayload *>  mClearTransFifo; //Clear transaction list for AT mode
    TlmBasicPayload* mTrans;//Transaction info
    BusTime_t mTransTime;//Internal time
    bool mIsDebug;//Is debug transaction

    void TransferComplete(void);
    void SetTransBasicModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTransG3mModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);

    struct AreaInformation {
        unsigned int start_addr;
        unsigned int end_addr;
        unsigned int ch_id;
        AreaInformation(unsigned int _start_addr = 0,
                        unsigned int _end_addr = 0,
                        unsigned int _ch_id = 0)
        {
            this->start_addr = _start_addr;
            this->end_addr = _end_addr;
            this->ch_id = _ch_id;
        }
    };
    std::vector<AreaInformation> mAreaInfo;

    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);

    //private function and variables
    bool CheckTransAddress(const unsigned int trans_addr, unsigned int &ch_id);
    bool AccessJudgement(TlmBasicPayload &trans, bool debug);
    void Initialize (void);
    bool mResetState;

    unsigned int                    mTypeADDR;
    bool                            mTypeSEC;
    bool                            mTypeDBG;
    bool                            mTypeUM;
    unsigned int                    mTypeSPID;
    bool                            mTypeWRITE;
};
#endif //__DMAG_U2_COMMON_GUARD_H__
