//----------------------------------------------------------------------
//* File: DummyMasterRvc_Base.h
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
// Description: Copyright of DummyMasterRvc_Base.h
// Ref: {SPIDCTL_UT_VRFCopyRight_001}

#ifndef __DUMMYMASTERRVC_BASE_H__
#define __DUMMYMASTERRVC_BASE_H__
#include <systemc.h>

class DummyMasterRvc_Base
{
public:
    DummyMasterRvc_Base(void){;}
    ~DummyMasterRvc_Base(void){;}
    virtual void NotifyTransTransfer(void) = 0;
};
#endif
