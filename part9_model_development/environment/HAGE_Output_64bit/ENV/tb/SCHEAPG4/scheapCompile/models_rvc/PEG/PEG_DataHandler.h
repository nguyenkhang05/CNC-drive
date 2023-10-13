// ---------------------------------------------------------------------
// $Id: PEG_DataHandler.h,v 1.3 2014/04/04 07:39:00 duymai Exp $
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

#ifndef __PEG_DATAHANDLER_H__
#define __PEG_DATAHANDLER_H__
#include "BusBridgeCoreBase.h"
#include "PEG_LT.h"

class PEG_DataHandler : public BusBridgeCoreBase<32,32>
{
public:
    PEG_DataHandler(std::string name,
                BusBridgeModuleBase<32,32> *Parent, 
                BusBridgeSlaveIf<32> *SlaveIf,
                BusMasterIf<32> *MasterIf,
                PEG_AgentController *AgentController_ptr);
    virtual ~PEG_DataHandler(){;}
private:
    PEG_AgentController *mAgentController;
    BusMasterIf<32> *mMasterIf;
    BusBridgeSlaveIf<32> *mSlaveIf;
    //Overwrite virtual functions of BusBridgeCoreBase class
    void read(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void write(unsigned int offsetAddress,TlmBasicPayload& trans,BusTime_t* t,bool debug);
    void deleteTrans(void);
};
#endif
