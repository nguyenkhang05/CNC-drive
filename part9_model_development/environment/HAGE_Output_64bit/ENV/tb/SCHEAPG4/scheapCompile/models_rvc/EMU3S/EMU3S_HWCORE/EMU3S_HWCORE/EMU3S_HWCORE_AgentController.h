// ---------------------------------------------------------------------
// $Id: EMU3S_HWCORE_AgentController.h,v 1.4 2020/11/17 00:09:10 huyquocpham Exp $
//
// Copyright(c) 2020 Renesas Electronics Corporation
// Copyright(c) 2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
// Description: Copyright of EMU3S_HWCORE_AgentController.h
// Ref: {EMU3S_HWCORE_UD_Copyright_001}

#ifndef __EMU3S_HWCORE_AGENT_CONTROLLER_H__
#define __EMU3S_HWCORE_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>
#include "CRWInterface.h"

enum emSubModel {
    emEMU3S_HWCORE = 0,
    emINPUT = 1,
    emPI = 2,
    emPWM = 3,
    emRCTG = 4,
    emIIR = 5,
    emMEASURE = 6,
    emRSLVIDE = 7,
    emADTRG = 8,
    emIRCTG3 = 9,
    emINT = 10,
    emVRFBUF = 11,
    emHANDSHAKE = 12
};

// Class EMU3S_HWCORE_AgentController
class EMU3S_HWCORE_AgentController
{
#include "EMU3S_HWCORE_cmdif.h"
public:
    // Constructor/Destructor
    EMU3S_HWCORE_AgentController(void) {;}
    virtual ~EMU3S_HWCORE_AgentController(void) {;}

    // Interface (APIs)
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual bool GetResetStatus(const std::string reset_name) = 0;
    virtual CRWInterface* GetInstance(unsigned int addr) = 0;
    virtual void NotifyWriteCTR() = 0;
    virtual void NotifyWriteREFCTR() = 0;
    virtual void NotifyWriteAHBPRT() = 0;
    virtual void NotifyWriteTBLADR() = 0;
    virtual void NotifyWriteIPTRGCTR() = 0;
    virtual void NotifyWriteIPTRGNUM() = 0;
    virtual void NotifyWriteFUNCIDLEGRPA0() = 0;
    virtual void NotifyWriteFUNCIDLEGRPA1() = 0;
    virtual void NotifyWriteFUNCIDLEGRPA2() = 0;
    virtual void NotifyWriteFUNCIDLEGRPB() = 0;
    virtual void NotifyWriteFUNCWAITGRPA() = 0;
    virtual void NotifyWriteFUNCWAITGRPB() = 0;
    virtual void NotifyWriteFUNCFLGRPA0() = 0;
    virtual void NotifyWriteFUNCFLGRPA1() = 0;
    virtual void NotifyWriteFUNCFLGRPA2() = 0;
    virtual void NotifyWriteFUNCFLGRPB() = 0;
    virtual void NotifyWriteADSEL() = 0;
    virtual void NotifyWriteSFERC() = 0;
    virtual void NotifySWTrigger(emSubModel emSub) = 0;
    virtual void WriteSWReset(bool is_active) = 0;
    virtual unsigned int GetTRGNUM(emSubModel emSub) = 0;
};


#endif //__EMU3S_HWCORE_AGENT_CONTROLLER_H__
