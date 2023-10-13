//----------------------------------------------------------------------
//* File: DummyMasterRvc.h
//* Description: DummyMasterRvc model is used for transaction issuing
//* Modified date: June 03, 2014 - NganTran
//----------------------------------------------------------------------
#ifndef __DUMMYMASTERRVC_H__
#define __DUMMYMASTERRVC_H__

#include "DummyMasterRvc_Base.h"
#include "DummyMasterRvc_Func.h"
#include "BusMasterBase.h"
#include "BusSlaveBase.h"

class DummyMasterRvc:public sc_module,
                     public DummyMasterRvc_Base,
                     public BusMasterBase<64,1>
{
public:
    //Port declaration
    sc_in<uint64>                   freqPortApb;
    sc_in<bool>                     resetPort;

    //Socket declaration
    TlmInitiatorSocket<64>          *is;

    DummyMasterRvc_Func             *mDummyMasterRvc_Func;

    SC_HAS_PROCESS(DummyMasterRvc);
    DummyMasterRvc(sc_module_name name, unsigned int rLatency, unsigned int wLatency);
    ~DummyMasterRvc (void);
    // Python IF APIs
    void IssueTransaction(const unsigned int command, const unsigned int addr, const unsigned int write_size, const unsigned int data, const unsigned int debug, const unsigned int extenstion);
    void ReadReceivedData(const unsigned int ex_val);
    void SetSimMode(std::string simmode);
    void SetTMPass();
    void EnableDumpMessage(bool enable);
private:
    sc_event trans_event;
    bool mEnableDumpMessage;
    void Judgement(unsigned int value);
    void TransMethod(void);
    //virtual function of DummyMasterBase
    void NotifyTransTransfer(void);
};
#endif
