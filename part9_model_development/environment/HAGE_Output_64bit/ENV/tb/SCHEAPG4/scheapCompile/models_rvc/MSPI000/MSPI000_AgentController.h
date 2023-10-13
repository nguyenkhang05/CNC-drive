// ---------------------------------------------------------------------
// $Id: MSPI000_AgentController.h,v 1.8 2020/03/11 00:52:43 ducla Exp $
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#ifndef __MSPI000_AGENT_CONTROLLER_H__
#define __MSPI000_AGENT_CONTROLLER_H__
#include <systemc.h>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
#include <stdarg.h>

// Class MSPI000_AgentController
class MSPI000_AgentController
{
#include "MSPI000_cmdif.h"
public:
    // Control data
    enum eControlData {
            emInitData = 0x00000000
           ,emTxDataDefault    = 0xFFFFFFFF
    };
    // Operation mode
    enum eOperationMode {
        emMaster = 0
        ,emSlave = 1
    };
    // Communication mode
    enum eCommunicationMode {
        emNoneTxRx = 0
        ,emTXRX = 1
        ,emTX  = 2
        ,emRX  = 3
    };
    // Memory mode
    enum eMemoryMode {
        emDirect = 0
        ,emFixedBuffer = 1
        ,emFixedFIFO = 2
    };
    // IP state
    enum eIPState {
        emRESET = 0
        ,emIDLE = 1
        ,emOPERATION = 2
    };
    // Channel state
    enum eChState {
        emINIT = 0
        ,emREADY = 1
    };
    // Delay value
    enum eDelayValue {
        emDSETUP = 0
        ,emDHOLD = 1
        ,emDIDLE = 2
        ,emDINDA = 3
        ,emNumDelay = 4
    };
    // model state if init or ready
    enum eModelState {
        emModelINIT = 0
        ,emModelREADY = 1
    };
    // defined number of ports
    enum eMSPI_In_Out {
        emNumDuplicatePort = 4
        ,emNumCSPort = 15
        ,emNumHardwareTriggerPort = 63
    };
    enum eINT_Type{
         emNumTX = 0
        ,emNumRX = 1
        ,emNumFE = 2
        ,emNumERR = 3
    };
    //define struct for reserve buffer and shift register
    struct Data_st {
        unsigned int data;
        unsigned int chid;
        unsigned int request;
        unsigned int csr;
        unsigned int joben;
        Data_st()
        {
            data = 0;
            chid = 0;
            request = 0;
            csr = 0;
            joben = 0;
        }
    };

    // Constructor/Destructor
    MSPI000_AgentController(void) {;}
    virtual ~MSPI000_AgentController(void) {;}

    // Interface (APIs)
    //user function here
    virtual bool CheckClockPeriod(const std::string clock_name) = 0;
    virtual bool GetResetStatus() = 0;

    virtual void InactiveOutputPorts(const unsigned int csp, const unsigned int ckr, const unsigned int sols) = 0;

    virtual void SetOperationMode(bool mode) = 0;
    virtual void SetCommunicationMode(unsigned int chid, unsigned int mode) = 0;
    virtual void SetMemoryMode(unsigned int chid, unsigned int mode) = 0;
    virtual void SetIPState(bool state) = 0;
    virtual void SetChannelState(unsigned int chid, unsigned int state, unsigned int size) = 0;
    virtual sc_dt::uint64 GetHWTRGValue() = 0;
    virtual void UpdateRequestNumber(unsigned int chid) = 0;
    virtual void AsserINTMSPIn(unsigned int chid, std::string int_name, std::string error_type) = 0;
    virtual void SetChannelStop(unsigned int chid) = 0;
    virtual void NotifyEventOutputINT(unsigned int type) = 0;
    virtual void ResetBufferCounter(unsigned int id) = 0;
};
#endif //__MSPI000_AGENT_CONTROLLER_H__
