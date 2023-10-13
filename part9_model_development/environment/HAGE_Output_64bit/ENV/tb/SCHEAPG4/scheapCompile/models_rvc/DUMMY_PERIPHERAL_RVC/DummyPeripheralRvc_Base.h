//----------------------------------------------------------------------
//* File: DummyPeripheralRvc_Base.h
//* Description: DummyPeripheralRvc model is used for signal issuing/receiving (control ports)
//* Modified date: Jun 17, 2016 - HaPham
//----------------------------------------------------------------------
#ifndef __DUMMYPERIPHERALRVC_BASE_H__
#define __DUMMYPERIPHERALRVC_BASE_H__
#include <systemc.h>
#include <cstring>

class DummyPeripheralRvc_Base {
public:
    enum eSIGNAME {//Handle output signals
        emPCLK_H,
        emPCLK_L,
        emRESETn
    };

    DummyPeripheralRvc_Base(){;}
    ~DummyPeripheralRvc_Base(){;}
    virtual void SetOutputSignal(eSIGNAME signal_name, unsigned int value)=0;
};

#endif
