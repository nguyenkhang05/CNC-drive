// ---------------------------------------------------------------------
// $Id: DTSG_U2_Common_Guard.h,v 1.4 2020/01/07 04:00:23 synguyen Exp $
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

#ifndef __DTSG_U2_DATAHANDLER_H__
#define __DTSG_U2_DATAHANDLER_H__
#include "BusBridgeCoreBase.h"
#include "DTSG_U2_AgentController.h"

class DTSG_U2_AgentController;
class DTSG_U2_Common_Guard : //public sc_module,
    public BusBridgeCoreBase<BUS_WIDTH_APB, BUS_WIDTH_APB>
{
public:
    DTSG_U2_Common_Guard(std::string name,
            DTSG_U2_AgentController* ptr_DTSG_U2_AgentController,
            BusBridgeModuleBase<BUS_WIDTH_APB, BUS_WIDTH_APB> *Parent,
            BusBridgeSlaveIf<BUS_WIDTH_APB> *SlaveIf,
            BusMasterIf<BUS_WIDTH_APB> *MasterIf,
            unsigned int numOfCh,
            SIM_MODE_T simmode);
    virtual ~DTSG_U2_Common_Guard();


    void SetAreaAddr(const unsigned int start_addr, const unsigned int size, const unsigned int channel_idx);
    void EnableReset (const bool is_active);                                            //Enable/Disable reset state
    void SetCLKPeriod (const sc_dt::uint64 clk_period);
    unsigned int GetVAR(const std::string name);
    sc_event                        mTransErrorEvent;

private:
    BusMasterIf<BUS_WIDTH_APB> *mMasterIf;
    BusBridgeSlaveIf<BUS_WIDTH_APB> *mSlaveIf;
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
        unsigned int channel_idx;
        AreaInformation(
                unsigned int _start_addr = 0,
                unsigned int _end_addr = 0,
                unsigned int _channel_idx = 0)
        {         
            this->start_addr = _start_addr;
            this->end_addr = _end_addr;
            this->channel_idx = _channel_idx;
        }
    };
    std::vector<AreaInformation> mAreaInfo;
    
    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);
 
    //private function and variables
    bool CheckTransAddress(const unsigned int trans_addr, unsigned int& channel_idx);
    bool CheckAccessibility(TlmBasicPayload &trans, bool debug);
    void CancelEvents (void);
    void Initialize (void);
    bool mRstState;
    sc_dt::uint64                   mCLKPeriod;
    unsigned int                    mNumOfChannel;
    unsigned int                    mTypeADDR;
    bool                            mTypeDBG;
    bool                            mTypeUM;
    bool                            mTypeSEC;
    unsigned int                    mTypeSPID;
    bool                            mTypeWRITE;
    DTSG_U2_AgentController*        mDTSG_U2_AgentController;
};
#endif
//
