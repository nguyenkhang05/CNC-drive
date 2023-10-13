//----------------------------------------------------------------------
//* File: DummyMasterRvc_Base.h
//* Description: DummyMasterRvc model is used for transaction issuing
//* Modified date: June 03, 2014 - NganTran
//----------------------------------------------------------------------
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
