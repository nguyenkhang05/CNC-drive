//----------------------------------------------------------------------
//* File: DummyMasterRvc.h
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
// Description: Copyright of DummyMasterRvc.h
// Ref: {SPIDCTL_UT_VRFCopyRight_001}
#ifndef __DUMMYMASTERRVC_H__
#define __DUMMYMASTERRVC_H__

#include "DummyMasterRvc_Base.h"
#include "DummyMasterRvc_Func.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"

class DummyMasterRvc:public sc_module,
                     public DummyMasterRvc_Base,
					 public BusSlaveBase<32,1>,
                     public BusMasterBase<64,1>
{
public:
    // Port declaration in Dummy Master
    // Ref: {SPIDCTL_UT_VRFDummyMaster_001}
    sc_in<uint64>                   freqPortApb;
    sc_in<bool>                     resetPort;

    // Socket declaration in Dummy Master
    // Ref: {SPIDCTL_UT_VRFDummyMaster_001}
    TlmInitiatorSocket<64>          *is;
    TlmTargetSocket<32>	            *ts;

    DummyMasterRvc_Func             *mDummyMasterRvc_Func;

    SC_HAS_PROCESS(DummyMasterRvc);
    DummyMasterRvc(sc_module_name name, unsigned int rLatency, unsigned int wLatency, SIM_MODE_T simmode);
    ~DummyMasterRvc (void);
    
    // Python IF APIs
    void IssueTransaction(const unsigned int command, const unsigned int addr, const unsigned int write_size, const unsigned int data, const unsigned int debug, const unsigned int extenstion);
    void IssueTransactionextnull(const unsigned int command, const unsigned int addr, const unsigned int write_size, const unsigned int data, const unsigned int debug, std::string extenstion);
    void ReadReceivedData(const unsigned int ex_val);
    void SetSimMode(std::string simmode);
    void SetTMPass();
    void EnableDumpMessage(bool enable);

private:
    sc_event trans_event;
    bool mEnableDumpMessage;
    bool extenstionnull;    
    void Judgement(unsigned int value);
    void TransMethod(void);
    void NotifyTransTransfer(void);
};
#endif
