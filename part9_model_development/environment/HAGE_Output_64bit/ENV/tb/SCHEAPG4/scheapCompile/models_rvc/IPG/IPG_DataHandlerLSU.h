// ---------------------------------------------------------------------
// $Id: IPG_DataHandlerLSU.h,v 1.2 2014/04/04 07:56:58 duymai Exp $
//
// Copyright(c) 2014 Renesas Electronics Corporation
// Copyright(c) 2014 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#ifndef __IPG_DataHandlerLSU_H__
#define __IPG_DataHandlerLSU_H__
#include "BusBridgeCoreBase.h"
#include "IPG_LT.h"

class IPG_DataHandlerLSU: public BusBridgeCoreBase<32,32>
{
public:
    IPG_DataHandlerLSU(std::string name,
                      BusBridgeModuleBase<32,32> *Parent, 
                      BusBridgeSlaveIf<32> *SlaveIf,
                      BusMasterIf<32> *MasterIf,
                      IPG_AgentController *AgentController_ptr);
    virtual ~IPG_DataHandlerLSU(){;}
private:
    IPG_AgentController *mIPG_AgentController;
    BusMasterIf<32> *mMasterIf;
    BusBridgeSlaveIf<32> *mSlaveIf;
    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
};
#endif
