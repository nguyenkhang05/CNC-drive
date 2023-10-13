//----------------------------------------------------------------------
//* File: DummyPeripheralRvc_Base.h
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
// Description: Copyright of DummyPeripheralRvc_Base.h
// Ref: {SPIDCTL_UT_VRFCopyRight_001}

#ifndef __DUMMYPERIPHERALRVC_BASE_H__
#define __DUMMYPERIPHERALRVC_BASE_H__
#include <systemc.h>
#include <cstring>

class DummyPeripheralRvc_Base {
public:
    enum eSIGNAME {//Handle output signals
        emRESET         = 0,
        emCLK_LSB_H     = 1,
        emCLK_LSB_L     = 2,
    };

    DummyPeripheralRvc_Base(){;}
    ~DummyPeripheralRvc_Base(){;}
    virtual void SetOutputSignal(eSIGNAME signal_name, unsigned int value) =0;
};

#endif
