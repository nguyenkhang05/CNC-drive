//----------------------------------------------------------------------
//* File: DummyPeripheralRvc_Func.h
//* Description: DummyPeripheralRvc model is used for signal issuing/receiving (control ports)
//* Modified date: Sep 20, 2016 - Thang
//----------------------------------------------------------------------
#ifndef __DUMMYPERIPHERALRVC_FUNC_H__
#define __DUMMYPERIPHERALRVC_FUNC_H__
#include "BusSlaveFuncIf.h"
#include "dummyperipheralrvc_regif.h"
#include "DummyPeripheralRvc_Base.h"

class DummyPeripheralRvc;

class DummyPeripheralRvc_Func: public Cdummyperipheralrvc_regif, public BusSlaveFuncIf
{
public:
    DummyPeripheralRvc_Func(std::string name, DummyPeripheralRvc_Base *DummyPeripheralRvc_Base_ptr, DummyPeripheralRvc *parent);
    ~DummyPeripheralRvc_Func();
    
    //For slave - virtual functions in BusSlaveFuncIf
    void read(unsigned int offsetAddress, TlmBasicPayload &trans, sc_time *t, bool debug);
    void write(unsigned int offsetAddress, TlmBasicPayload &trans, sc_time *t, bool debug);
    
    //Callback function - used when receive signals
    void updateReg(std::string reg_name, unsigned int value);
    unsigned int readReg (std::string reg_name);
    //Callback functions - used when issue signals
    //-----

    void cb_JUDGE_REG_JUDGE(RegCBstr str);
    void cb_RESET_REG_PRESETn(RegCBstr str);
    void cb_PCLK_REG_PCLK(RegCBstr str);

private:
    DummyPeripheralRvc_Base *mDummyPeripheralRvc_Base;
    unsigned int cur_PCLK_L;
    DummyPeripheralRvc *mparent;

};
#endif
