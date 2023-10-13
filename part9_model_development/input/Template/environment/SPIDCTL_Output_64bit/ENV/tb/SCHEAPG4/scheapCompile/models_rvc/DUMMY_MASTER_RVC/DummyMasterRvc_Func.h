//----------------------------------------------------------------------
//* File: DummyMasterRvc_Func.h
//
// Copyright(c) 2019 Renesas Electronics Corporation
// Copyright(c) 2019 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of DummyMasterRvc_Func.h
// Ref: {SPIDCTL_UT_VRFCopyRight_001}
#ifndef __DUMMYMASTERRVC_FUNC_H__
#define __DUMMYMASTERRVC_FUNC_H__
#include <cassert>

#include "DummyMasterRvc_Base.h"
#include "dummymasterrvc_regif.h"
#include "BusMasterIf.h"
#include "BusMasterFuncIf.h"
#include "BusSlaveIf.h"
#include "BusSlaveFuncIf.h"

class DummyMasterRvc_Func: public Cdummymasterrvc_regif,
                           public BusMasterFuncIf,
                           public BusSlaveFuncIf
{
public:
    DummyMasterRvc_Func(std::string name,
                        DummyMasterRvc_Base *DummyMasterRvc_Base_ptr,
                        BusMasterIf<64> *MasterIf_is_ptr,
       	                BusSlaveIf<32> *SlaveIf32_ts_ptr,
                        SIM_MODE_T simMode);
    ~DummyMasterRvc_Func (void);

    //For master - virtual functions in BusMasterFuncIf
    void notifyTransSend(TlmTransactionType &trans, BusTime_t &t);
    void notifyTransComp(TlmTransactionType &trans, BusTime_t &t);
    //For slave - virtual functions in BusSlaveFuncIf
    void read(unsigned int offsetAddress, TlmBasicPayload &trans, BusTime_t *t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload &trans, BusTime_t *t, bool debug);
    //Callback function
    void cb_CTRL_REG_CTRL(RegCBstr str);
    
    //Transaction function
    template<unsigned int BUSWIDTH>
    void IssueTransaction(BusMasterIf<BUSWIDTH> *master_ptr, TlmBasicPayload *payload_ptr, unsigned char protocol_type);
    void TransProcess(void);
    void TransProcessextnull(void);
    void ReceiveRD_DATA(TlmTransactionType *trans);
    void DeleteTrans(TlmTransactionType *trans);
    void ConfigTransaction(bool command,unsigned int addr, unsigned char write_size, unsigned int data, bool debug, unsigned int extenstion);
    unsigned int ReadReceivedData();
    void SetSimMode(std::string simmode);
    void EnableDumpMessage(bool enable);
    unsigned int GetPEID();
private:
    enum eTransBus {
        emBUS_APB = 0x0,
        emBUS_AHB = 0x1,
        emBUS_AXI = 0x2,
        emBUS_VPI = 0x3,
        emBUS_VCI = 0x4
    };
    enum eIniSocket {
        emOTHERS    = 0x0,
        emIS        = 0x1   //use "is" socket when users write 0x1 into CTRL_REG register
    };

    DummyMasterRvc_Base             *mDummyMasterRvc_Base;
    BusMasterIf<64>                 *mMasterIf_is;
    BusSlaveIf<32> 		    *mSlaveIf32_ts;


    std::queue<TlmBasicPayload *>   mRequestFifo; //Bus request FIFO
    unsigned char                   mData[1024];
    std::queue<unsigned char *>     mDataFifo; //Data request FIFO
    BusTime_t                       t;
    unsigned char                   masterif_sel; //Flag used to control transaction (its value is CTRL_REG value)
    unsigned int                   cmd;
    unsigned int                   size;
    unsigned int                    addr;
    unsigned int                    data;
    unsigned long long              data64;
    unsigned int                   spid;
    unsigned int                   tcid;
    unsigned int                   vcid;
    unsigned int                   peid;
    unsigned int                   ext;
    bool                            um;
    bool                            vm;
    bool                            sec; // Secure option
    bool                            debug_mode;
    std::string                     mSimMode;
    bool                            mEnableDumpMessage;

};
#endif
