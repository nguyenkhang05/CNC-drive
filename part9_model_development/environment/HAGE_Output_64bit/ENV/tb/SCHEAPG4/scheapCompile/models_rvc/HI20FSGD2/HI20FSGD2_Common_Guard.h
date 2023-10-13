// ---------------------------------------------------------------------
// $Id: HI20FSGD2_Common_Guard.h,v 1.2 2019/11/26 05:07:33 hadinhtran Exp $
//
// Copyright(c) 2016-2019 Renesas Electronics Corporation
// Copyright(c) 2016-2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __HI20FSGD2_DATAHANDLER_H__
#define __HI20FSGD2_DATAHANDLER_H__
#include "BusBridgeCoreBase.h"
#include "global.h"

class HI20FSGD2_Common_Guard : //public sc_module,
                     public BusBridgeCoreBase<32,32>
{
public:
    HI20FSGD2_Common_Guard(std::string name,
                   BusBridgeModuleBase<32,32> *Parent,
                   BusBridgeSlaveIf<32> *SlaveIf,
                   BusMasterIf<32> *MasterIf,
                   SIM_MODE_T simmode,
                   unsigned int ch_num);
    virtual ~HI20FSGD2_Common_Guard(){;}

    bool SetAreaAddr(const std::string name, const unsigned int start_addr, const unsigned int size, const unsigned int ch_id);

    void EnableReset (const bool is_active);                                            //Enable/Disable reset state
    void SetCLKPeriod (const sc_dt::uint64 clk_period);

    void SetVAR(const std::string name, const unsigned int val, const unsigned int ch_id);
    unsigned int GetVAR(const std::string name);
    void SetSecureFunc(bool sec);
    sc_event                        mTransErrorEvent;

private:
    enum eChannelNum {
        emChannelNum = 128
    };

    BusMasterIf<32> *mMasterIf;
    BusBridgeSlaveIf<32> *mSlaveIf;
    std::map<TlmBasicPayload*,TlmBasicPayload*> mRequestMap; //Bus request map for AT mode
    std::queue<TlmBasicPayload *>  mClearTransFifo; //Clear transaction list for AT mode
    TlmBasicPayload* mTrans;//Transaction info
    BusTime_t mTransTime;//Internal time
    bool mIsDebug;//Is debug transaction
    unsigned int mChannelNum;

    void TransferComplete(void);
    void SetTransBasicModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTransG3mModel(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);

    struct AreaInformation {
        std::string area_name;
        unsigned int start_addr;
        unsigned int end_addr;
        unsigned int ch_id;
        AreaInformation(std::string _area_name = "",
                        unsigned int _start_addr = 0,
                        unsigned int _end_addr = 0,
                        unsigned int _ch_id = 0)
        {
            this->area_name = _area_name;
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
    bool CheckAccessibility(TlmBasicPayload &trans, bool debug);
    void CancelEvents (void);
    void Initialize (void);
    bool mRstState;
    sc_dt::uint64                   mCLKPeriod;

    unsigned int                    mSPID[emChannelNum];
    bool                            mGEN[emChannelNum];
    bool                            mDBG[emChannelNum];
    bool                            mUM[emChannelNum];
    bool                            mSEC[emChannelNum];
    bool                            mWG[emChannelNum];
    bool                            mRG[emChannelNum];

    unsigned int                    mTypeADDR;
    bool                            mTypeDBG;
    bool                            mTypeUM;
    bool                            mTypeSEC;
    unsigned int                    mTypeSPID;
    bool                            mTypeWRITE;
    bool                            mSECPara;

};
#endif
//
