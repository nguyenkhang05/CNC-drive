//----------------------------------------------------------------------
//* File: DummyMasterRvc_Func.cpp
//* Description: DummyMasterRvc model is used for transaction issuing
//* Modified date: June 03, 2014 - NganTran
//----------------------------------------------------------------------
#include "DummyMasterRvc_Func.h"

/*********************************
// Function     : DummyMasterRvc_Func
// Description  : Constructor of DummyMasterRvc_Func class
// Parameter    :
//      name            Module name
//      MasterIf_p      Master IF (APB bus)
//      Parent          Indicate base class
// Return value : None
**********************************/
DummyMasterRvc_Func::DummyMasterRvc_Func(std::string name,
                                         DummyMasterRvc_Base *DummyMasterRvc_Base_ptr,
                                         BusMasterIf<64> *MasterIf_is_ptr):
                                         Cdummymasterrvc_regif(name,32),
                                         mMasterIf_is(MasterIf_is_ptr)
{
    Cdummymasterrvc_regif::set_instance_name(name);
    mDummyMasterRvc_Base = DummyMasterRvc_Base_ptr;
    
    t        = SC_ZERO_TIME;
    cmd      = 0x0;
    size     = 0x0;
    addr     = 0x0;
    data     = 0x0;
    data64   = 0x0;
    spid     = 0x0;
    vcid     = 0x0;
    peid     = 0x0;
    tcid     = 0x0;
    um       = false;
    vm       = false;
    debug_mode = false;
    mSimMode = "LT";
    mEnableDumpMessage = true;
}

/*********************************
// Function     : ~DummyMasterRvc_Func
// Description  : Destructor of DummyMasterRvc_Func class
// Parameter    : None
// Return value : None
**********************************/
DummyMasterRvc_Func::~DummyMasterRvc_Func (void)
{
}

void DummyMasterRvc_Func::cb_CTRL_REG_CTRL(RegCBstr str)
{
}

/*********************************
// Function     : notifyTransSend
//                - virtual function in BusMasterFuncIf
// Description  : Notify transaction sending
// Parameter    :
//      trans           Transaction
//      t               Bus time
// Return value : None
**********************************/
void DummyMasterRvc_Func::notifyTransSend(TlmTransactionType &trans, BusTime_t &t)
{
    //TODO
}

/*********************************
// Function     : notifyTransComp
//                - virtual function in BusMasterFuncIf
// Description  : Notify transaction completion
// Parameter    :
//      trans           Transaction
//      t               Bus time
// Return value : None
**********************************/
void DummyMasterRvc_Func::notifyTransComp(TlmTransactionType &trans, BusTime_t &t)
{
    std::queue<TlmBasicPayload *> tmpFifo;
    while (!mRequestFifo.empty()){
        TlmBasicPayload *TransTemp = mRequestFifo.front();
        mRequestFifo.pop();
        if (&trans == ((TlmTransactionType *)TransTemp)) {
            if (trans.is_read()){
                ReceiveRD_DATA(&trans);
            }
            DeleteTrans(&trans);
        } else {
            tmpFifo.push(TransTemp);
        }
    }
    mRequestFifo = tmpFifo;
    if(!mRequestFifo.empty()) {
        TlmBasicPayload *newTrans = mRequestFifo.front();
        IssueTransaction(mMasterIf_is, newTrans, emBUS_APB);
    }
}

/*********************************
// Function     : TransMethod
// Description  : Issue a transaction to target bus by calling
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc_Func::TransProcess(void)
{
    TlmBasicPayload *mPayload = new TlmBasicPayload;
    TlmG3mExtension *mG3mExt = new TlmG3mExtension;
    mPayload->set_address(addr);
    mPayload->set_data_length(size);
    mPayload->set_data_ptr(mDataFifo.back());
    if (cmd != 0x0) {
        mPayload->set_write();
    } else {
        mPayload->set_read();
    }
    //Extension information
    mG3mExt->setSpId(spid);
    mG3mExt->setVmId(vcid);
    mG3mExt->setPeId(peid);
    mG3mExt->setUserMode(um);
    mG3mExt->setVirtualMode(vm);
    mG3mExt->setTcId(tcid);
    mPayload->set_extension(mG3mExt);
    switch (masterif_sel) {
        case emIS:
            if ((mSimMode == "LT") ||  debug_mode) {
                IssueTransaction(mMasterIf_is, mPayload, emBUS_APB);
            }
            else {
                if(mRequestFifo.empty()) {
                    IssueTransaction(mMasterIf_is, mPayload, emBUS_APB);
                }
                mRequestFifo.push(mPayload);
            }
            break;
        default:    //emOTHERS
            printf("[WARNING] Cannot issue a transaction to unknown bus.\n");
            mPayload->get_extension(mG3mExt);
            mPayload->release_extension(mG3mExt);
            delete mPayload;
            break;
    }
}

/*********************************
// Function     : IssueTransaction
// Description  : Issue a transaction to target bus
// Parameter    :
//      master_ptr      Bus's pointer
//      payload_ptr     Transaction
//      protocol_type   Protocol type
// Return value : None
**********************************/
template<unsigned int BUSWIDTH>
void DummyMasterRvc_Func::IssueTransaction(BusMasterIf<BUSWIDTH> *master_ptr, TlmBasicPayload *payload_ptr, unsigned char protocol_type)
{
    if (protocol_type == emBUS_APB) {
        if (mEnableDumpMessage){
            printf("    -> [APB bus]: ");
        }
        TlmApbExtension *mApbExt = new TlmApbExtension;
        payload_ptr->set_extension(mApbExt);
        TlmVciExtension *mVciExt = new TlmVciExtension;
        payload_ptr->set_extension(mVciExt);
    } else if (protocol_type == emBUS_AHB) {
        if (mEnableDumpMessage){
            printf("    -> [AHB bus]: ");
        }
        TlmAhbExtension *mAhbExt = new TlmAhbExtension;
        payload_ptr->set_extension(mAhbExt);
    } else if (protocol_type == emBUS_AXI) {
        if (mEnableDumpMessage){
            printf("    -> [AXI bus]: ");
        }
        TlmAxiExtension *mAxiExt = new TlmAxiExtension;
        payload_ptr->set_extension(mAxiExt);
    } else if (protocol_type == emBUS_VPI) {
        if (mEnableDumpMessage){
            printf("    -> [VPI bus]: ");
        }
        TlmVpiExtension *mVpiExt = new TlmVpiExtension;
        payload_ptr->set_extension(mVpiExt);
    } else if (protocol_type == emBUS_VCI) {
        if (mEnableDumpMessage){
            printf("    -> [VCI bus]: ");
        }
        TlmVciExtension *mVciExt = new TlmVciExtension;
        payload_ptr->set_extension(mVciExt);
    } else {
        printf("[WARNING] Cannot issue a transaction to unknown bus.\n");
    }
    if (payload_ptr->is_write()) {
        if (mEnableDumpMessage){
            printf("WRITE transaction is issued");
        }
        if (debug_mode) {//Debug mode
            if (mEnableDumpMessage){
                printf(" (DEBUG mode)\n");
            }
            master_ptr->write(*payload_ptr, true, t, false);
        } else {
            if (mEnableDumpMessage){
                printf(" (NORMAL mode)\n");
            }
            master_ptr->write(*payload_ptr, false, t, false);
        }
        if (mEnableDumpMessage){
            printf("    Transaction info: address(0x%8X); size (%d); written data (0x%8X)\n",addr,size,data);
        }
    } else {
        if (mEnableDumpMessage){
            printf("READ transaction is issued");
        }
        if (debug_mode) {//Debug mode
            if (mEnableDumpMessage){
                printf(" (DEBUG mode)\n");
            }
            master_ptr->read(*payload_ptr, true, t, false);
        } else {
            if (mEnableDumpMessage){
                printf(" (NORMAL mode)\n");
            }
            master_ptr->read(*payload_ptr, false, t, false);
        }
        if ((mSimMode == "LT") || debug_mode) {
            ReceiveRD_DATA((TlmTransactionType *)payload_ptr);
        }
    }
    if ((mSimMode == "LT") || debug_mode) {
        DeleteTrans((TlmTransactionType *)payload_ptr);
    } //else {
    //    mRequestFifo.push(payload_ptr);
    //}
}

/*********************************
// Function     : ReceiveRD_DATA
// Description  : Receive data from READ transaction
// Parameter    :
//      trans           Transaction
// Return value : None
**********************************/
void DummyMasterRvc_Func::ReceiveRD_DATA(TlmTransactionType *trans)
{
    unsigned char *data_ptr = NULL;
    data_ptr = trans->get_data_ptr();
    assert(data_ptr != NULL);
    unsigned int data_length = trans->get_data_length();
    unsigned long long r_data = 0x0;
    memcpy(&r_data,data_ptr,data_length);
    if (size <= 4) {
        (*RD_DATA_REG)["DATA"] = (unsigned int)r_data;
        if (mEnableDumpMessage){
            printf("    Transaction info: address(0x%8X); size (%d); read data (0x% 8X)\n",addr,size,r_data);
        }
    } else {
        (*RD_DATA_REG_0)["DATA_0"] = (unsigned int)r_data;
        (*RD_DATA_REG_1)["DATA_1"] = (unsigned int)(r_data >> 32);
        //printf("    Transaction info: address(0x%8X); size (%d); read data (0x%8XLLU)\n",addr,size,r_data);
    }
}

/*********************************
// Function     : DeleteTrans
// Description  : Delete transaction pointer
// Parameter    :
//      trans           Transaction
// Return value : None
**********************************/
void DummyMasterRvc_Func::DeleteTrans(TlmTransactionType *trans)
{
    std::queue<unsigned char *>     tmpDataFifo;
    while (!mDataFifo.empty()){
        unsigned char *data_ptrtemp = mDataFifo.front();
        mDataFifo.pop();
        if (data_ptrtemp != trans->get_data_ptr()) {
            tmpDataFifo.push(data_ptrtemp);
        }
        else {
            delete[] data_ptrtemp;
        }
    }
    mDataFifo = tmpDataFifo;
    TlmG3mExtension *G3mExt = (TlmG3mExtension *)0;
    trans->get_extension(G3mExt);
    trans->release_extension(G3mExt);
    delete trans;
}

/*********************************
// Function     : ConfigTransaction
// Description  : Issue a transaction to target bus 
//                This function is call by PythonIF
// Parameter    : None
// Return value : None
**********************************/
void DummyMasterRvc_Func::ConfigTransaction(bool command,unsigned int addr, unsigned char write_size, unsigned int data, bool debug, unsigned int extenstion) {
    this->debug_mode = debug;
    this->cmd    = command;


    this->size   = write_size;
    this->addr   = addr;
    this->data   = data;
    this->data64 = ((unsigned long long)(*WR_DATA_REG_1)["DATA_1"] << 32) | ((unsigned long long)(*WR_DATA_REG_0)["DATA_0"]);
    this->spid   = (extenstion>>8) & 0x1F;
    this->vcid   = (extenstion>>16) & 0x7;
    this->peid   = (extenstion>>4) & 0x7;
    this->tcid   = (extenstion>>24) & 0x3F;
    this->um     = (bool) ((extenstion>>1) & 0x1);
    this->vm     = (bool) (extenstion & 0x1);
    unsigned char                   *mData;
    mData = new unsigned char [1024];
    memset(mData,0,1024);
    masterif_sel = 1;
    if(cmd != 0) {  //Write transaction
        if (size <= 4) {
            memcpy(mData ,&data, size);
        } else {    //4bytes < size <= 8bytes
            memcpy(mData ,&data64, 8);
        }
    } else {        //Read transaction
        unsigned int tmpData = 0x0;
        if (masterif_sel == emIS) {
            tmpData = 0x5AF; //To make sure read data have right value (read value should be different 0x5AF)
        }
    }
    mDataFifo.push(mData);
    mDummyMasterRvc_Base->NotifyTransTransfer();
}

/*********************************
// Function     : ReadReceivedData
// Description  : Issue a Write transaction transaction to target bus 
//                This function is call by PythonIF
// Parameter    : None
// Return value : None
**********************************/
unsigned int DummyMasterRvc_Func::ReadReceivedData() {
    return (*RD_DATA_REG)["DATA"];
}

/// Set simulation mode
void DummyMasterRvc_Func::SetSimMode(std::string simmode)
{//{{{
    mSimMode = simmode;
}//}}}

/// Enable dump info message
void DummyMasterRvc_Func::EnableDumpMessage(bool enable)
{//{{{
    mEnableDumpMessage = enable;
}//}}}
// vim600: set foldmethod=marker :
