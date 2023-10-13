//----------------------------------------------------------------------
//* File: DummyMasterRvc_Func.h
//* Description: DummyMasterRvc model is used for transaction issuing
//* Modified date: June 03, 2014 - NganTran
//----------------------------------------------------------------------
#ifndef __DUMMYMASTERRVC_FUNC_H__
#define __DUMMYMASTERRVC_FUNC_H__
#include <cassert>

#include "DummyMasterRvc_Base.h"
#include "dummymasterrvc_regif.h"
#include "BusMasterIf.h"
#include "BusMasterFuncIf.h"
#include "BusSlaveFuncIf.h"

class DummyMasterRvc_Func: public Cdummymasterrvc_regif,
                           public BusMasterFuncIf
{
public:
    DummyMasterRvc_Func(std::string name,
                        DummyMasterRvc_Base *DummyMasterRvc_Base_ptr,
                        BusMasterIf<64> *MasterIf_is_ptr);
    ~DummyMasterRvc_Func (void);

    //For master - virtual functions in BusMasterFuncIf
    void notifyTransSend(TlmTransactionType &trans, BusTime_t &t);
    void notifyTransComp(TlmTransactionType &trans, BusTime_t &t);
    // callback
    void cb_CTRL_REG_CTRL(RegCBstr str);
    //Transaction function
    template<unsigned int BUSWIDTH>
    void IssueTransaction(BusMasterIf<BUSWIDTH> *master_ptr, TlmBasicPayload *payload_ptr, unsigned char protocol_type);
    void TransProcess(void);
    void ReceiveRD_DATA(TlmTransactionType *trans);
    void DeleteTrans(TlmTransactionType *trans);
    void ConfigTransaction(bool command,unsigned int addr, unsigned char write_size, unsigned int data, bool debug, unsigned int extenstion);
    unsigned int ReadReceivedData();
    void SetSimMode(std::string simmode);
    void EnableDumpMessage(bool enable);
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

    std::queue<TlmBasicPayload *>   mRequestFifo; //Bus request FIFO
    
    std::queue<unsigned char *>     mDataFifo; //Data request FIFO
    BusTime_t                       t;
    unsigned char                   masterif_sel; //Flag used to control transaction (its value is CTRL_REG value)
    unsigned char                   cmd;
    unsigned char                   size;
    unsigned int                    addr;
    unsigned int                    data;
    unsigned long long              data64;
    unsigned char                   spid;
    unsigned char                   tcid;
    unsigned char                   vcid;
    unsigned char                   peid;
    bool                            um;
    bool                            vm;
    bool                            debug_mode;
    std::string                     mSimMode;
    bool                            mEnableDumpMessage;
};
#endif
