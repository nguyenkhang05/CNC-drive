// ---------------------------------------------------------------------
// $Id: HBG_Common_Guard.h,v 1.7 2020/02/18 07:24:22 huyquocpham Exp $
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

#ifndef __HBG_DATAHANDLER_H__
#define __HBG_DATAHANDLER_H__
#include "BusBridgeCoreBase.h"

class HBG_Common_Guard : //public sc_module,
    public BusBridgeCoreBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI>
{
public:
    HBG_Common_Guard(std::string name,
            BusBridgeModuleBase<BUS_WIDTH_AXI,BUS_WIDTH_AXI> *Parent,
            BusBridgeSlaveIf<BUS_WIDTH_AXI> *SlaveIf,
            BusMasterIf<BUS_WIDTH_AXI> *MasterIf,
            SIM_MODE_T simmode);
    virtual ~HBG_Common_Guard(){;}


    bool SetAreaAddr(const std::string name, const unsigned int start_addr, const unsigned int size);
    void SEC_DIS(bool value);
    void EnableReset (const bool is_active);                                            //Enable/Disable reset state
    void SetCLKPeriod (const sc_dt::uint64 clk_period);

    void SetVAR(const std::string name, const unsigned int val);
    unsigned int GetVAR(const std::string name);
    sc_event                        mTransErrorEvent;

private:
    BusMasterIf<BUS_WIDTH_AXI> *mMasterIf;
    BusBridgeSlaveIf<BUS_WIDTH_AXI> *mSlaveIf;
    std::map<TlmBasicPayload*,TlmBasicPayload*> mRequestMap; //Bus request map for AT mode
    std::queue<TlmBasicPayload *>  mClearTransFifo; //Clear transaction list for AT mode
    TlmBasicPayload* mTrans;//Transaction info
    BusTime_t mTransTime;//Internal time
    bool mIsDebug;//Is debug transaction

    
    void TransferComplete(void);
    void SetTrans_Basic(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTrans_G3m(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);
    void SetTrans_Axi(TlmBasicPayload &inTrans, TlmBasicPayload &outTrans);

    struct AreaInformation {
        std::string area_name;
        unsigned int start_addr;
        unsigned int end_addr;
        AreaInformation(std::string _area_name = "",
                unsigned int _start_addr = 0,
                unsigned int _end_addr = 0)
        {
            this->area_name = _area_name;
            this->start_addr = _start_addr;
            this->end_addr = _end_addr;
        }
    };
    std::vector<AreaInformation> mAreaInfo;
    
    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
    void notifyTransComp(TlmBasicPayload &trans, BusTime_t &t);

    //private function and variables
    bool CheckTransAddress(const unsigned int trans_addr);
    bool CheckAccessibility(TlmBasicPayload &trans, bool debug);
    void CancelEvents (void);
    void Initialize (void);
    bool mRstState;
    sc_dt::uint64                   mCLKPeriod;

    unsigned int                    mSPID;
    bool                            mGEN;
    bool                            mDBG;
    bool                            mUM;
    bool                            mSEC;
    bool                            mWG;
    bool                            mRG;

    unsigned int                    mTypeADDR;
    bool                            mTypeDBG;
    bool                            mTypeUM;
    bool                            mTypeSEC;
    unsigned int                    mTypeSPID;
    bool                            mTypeWRITE;
};
#endif
//
