// ---------------------------------------------------------------------
// $Id: UIMPL_SLAVE.cpp,v 1.5 2020/04/07 03:39:04 huepham Exp $
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
#include "UIMPL_SLAVE.h"

// Description: Constructor of UIMPL_SLAVE<32>
// Ref: {UIMPL_SLAVE_DD_Direction_019, UIMPL_SLAVE_DD_Summary_001, UIMPL_SLAVE_DD_Summary_006, UIMPL_SLAVE_DD_Summary_005, UIMPL_SLAVE_DD_BlockDiagram_001, UIMPL_SLAVE_DD_Ports_001, UIMPL_SLAVE_DD_ATT1_001, UIMPL_SLAVE_DD_Direction_020, UIMPL_SLAVE_DD_Direction_023}
template<>
UIMPL_SLAVE<32>::UIMPL_SLAVE(sc_module_name name, unsigned int rLatency, unsigned int wLatency) :
    sc_module( name )
    ,BusSlaveBase<32,1>()
    ,rvc_common_model()
    ,BusSlaveFuncIf()
    ,clk("clk")
    ,resetPort("resetPort")
    ,BMID("BMID")
    ,ERR("ERR")
 {//{{{

    // Configure Slave socket for buswidth 32, not use template
    setSlaveResetPort32(&resetPort);
    setSlaveFreqPort32(&clk);
    setTargetSocket32((char*)"ts");
    
    ts = BusSlaveBase<32,1>::tSocket32[0];
    
    mBusSlaveIf32[0]->setFuncModulePtr((BusSlaveFuncIf *) this);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);

    mclkPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mStartclkTime = 0;
    mIsResetPortActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mTimeCLKChangePoint = 0;
    
    mERRIntr = false; 
    mBMID = 0x0; 
    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    ERR.initialize(false);
    BMID.initialize(0x0);
    
    // SC_THREAD/METHOD
    SC_METHOD(HandleclkSignalMethod);
    dont_initialize();
    sensitive << clk;
    
    // processes handle reset sources
    SC_METHOD(HandleresetPortMethod);
    dont_initialize();
    sensitive << resetPort;

    SC_METHOD(HandleresetPortCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;
    
    SC_METHOD(WriteBMIDMethod);
    dont_initialize();
    sensitive << mBMIDEvent;
    
    SC_METHOD(WriteERRMethod);
    dont_initialize();
    sensitive << mERREvent;
    
    SC_METHOD(AssertERRIntrMethod);
    dont_initialize();
    sensitive << mAssertERREvent;
    
    SC_METHOD(DeassertERRIntrMethod);
    dont_initialize();
    sensitive << mDeassertERREvent;

    //Initialize signal, state
    Initialize();
    CommandInit((std::string)name);
}//}}}

// Description: Constructor of UIMPL_SLAVE<64>
// Ref: {UIMPL_SLAVE_DD_Direction_019, UIMPL_SLAVE_DD_Summary_001, UIMPL_SLAVE_DD_BlockDiagram_001, UIMPL_SLAVE_DD_Ports_001, UIMPL_SLAVE_DD_ATT1_001, UIMPL_SLAVE_DD_Direction_020, UIMPL_SLAVE_DD_Direction_023, UIMPL_SLAVE_DD_Direction_042, UIMPL_SLAVE_DD_ATT1_005}
template<>
UIMPL_SLAVE<64>::UIMPL_SLAVE(sc_module_name name, unsigned int rLatency, unsigned int wLatency) :
    sc_module( name )
    ,BusSlaveBase<64,1>()
    ,rvc_common_model()
    ,BusSlaveFuncIf()
    , clk("clk")
    , resetPort("resetPort")
    , BMID("BMID")
    , ERR("ERR")
 {//{{{

    // Configure Slave socket for buswidth 64, not use template
    setSlaveResetPort64(&resetPort);
    setSlaveFreqPort64(&clk);
    setTargetSocket64((char*)"ts");
    
    ts = BusSlaveBase<64,1>::tSocket64[0];
    
    mBusSlaveIf64[0]->setFuncModulePtr((BusSlaveFuncIf *) this);
    mBusSlaveIf64[0]->setReadInitLatency(rLatency);
    mBusSlaveIf64[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf64[0]->setBusProtocol(BUS_AXI);
    mBusSlaveIf64[0]->setTransNmbLmt(0x1);

    mclkPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mIsResetPortActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mTimeCLKChangePoint = 0;
    
    mERRIntr = false; 
    mBMID = 0x0; 
    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    ERR.initialize(false);
    BMID.initialize(0x0);
    
    // SC_THREAD/METHOD
    SC_METHOD(HandleclkSignalMethod);
    dont_initialize();
    sensitive << clk;
    
    // processes handle reset sources
    SC_METHOD(HandleresetPortMethod);
    dont_initialize();
    sensitive << resetPort;

    SC_METHOD(HandleresetPortCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;
    
    SC_METHOD(WriteBMIDMethod);
    dont_initialize();
    sensitive << mBMIDEvent;
    
    SC_METHOD(WriteERRMethod);
    dont_initialize();
    sensitive << mERREvent;
    
    SC_METHOD(AssertERRIntrMethod);
    dont_initialize();
    sensitive << mAssertERREvent;
    
    SC_METHOD(DeassertERRIntrMethod);
    dont_initialize();
    sensitive << mDeassertERREvent;

    //Initialize signal, state
    Initialize();
    CommandInit((std::string)name);
}//}}}

// Description: Constructor of UIMPL_SLAVE<128>
// Ref: {UIMPL_SLAVE_DD_Direction_019, UIMPL_SLAVE_DD_Summary_001, UIMPL_SLAVE_DD_BlockDiagram_001, UIMPL_SLAVE_DD_Ports_001, UIMPL_SLAVE_DD_ATT1_001, UIMPL_SLAVE_DD_Direction_020, UIMPL_SLAVE_DD_Direction_023}
template<>
UIMPL_SLAVE<128>::UIMPL_SLAVE(sc_module_name name, unsigned int rLatency, unsigned int wLatency) :
    sc_module( name )
    ,BusSlaveBase<128,1>()
    ,rvc_common_model()
    ,BusSlaveFuncIf()
    , clk("clk")
    , resetPort("resetPort")
    , BMID("BMID")
    , ERR("ERR")
 {//{{{

    // Configure Slave socket for buswidth 128, not use template
    setSlaveResetPort128(&resetPort);
    setSlaveFreqPort128(&clk);
    setTargetSocket128((char*)"ts");
    
    ts = BusSlaveBase<128,1>::tSocket128[0];
    
    mBusSlaveIf128[0]->setFuncModulePtr((BusSlaveFuncIf *) this);
    mBusSlaveIf128[0]->setReadInitLatency(rLatency);
    mBusSlaveIf128[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf128[0]->setBusProtocol(BUS_AXI);
    mBusSlaveIf128[0]->setTransNmbLmt(0x1);

    mclkPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mIsResetPortActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mTimeCLKChangePoint = 0;
    
    mERRIntr = false; 
    mBMID = 0x0; 
    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    ERR.initialize(false);
    BMID.initialize(0x0);
    
    // SC_THREAD/METHOD
    SC_METHOD(HandleclkSignalMethod);
    dont_initialize();
    sensitive << clk;
    
    // processes handle reset sources
    SC_METHOD(HandleresetPortMethod);
    dont_initialize();
    sensitive << resetPort;

    SC_METHOD(HandleresetPortCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;
    
    SC_METHOD(WriteBMIDMethod);
    dont_initialize();
    sensitive << mBMIDEvent;
    
    SC_METHOD(WriteERRMethod);
    dont_initialize();
    sensitive << mERREvent;
    
    SC_METHOD(AssertERRIntrMethod);
    dont_initialize();
    sensitive << mAssertERREvent;
    
    SC_METHOD(DeassertERRIntrMethod);
    dont_initialize();
    sensitive << mDeassertERREvent;

    //Initialize signal, state
    Initialize();
    CommandInit((std::string)name);
}//}}}

// Description: Constructor of UIMPL_SLAVE<256>
// Ref: {UIMPL_SLAVE_DD_Direction_019, UIMPL_SLAVE_DD_Summary_001, UIMPL_SLAVE_DD_BlockDiagram_001, UIMPL_SLAVE_DD_Ports_001, UIMPL_SLAVE_DD_ATT1_001, UIMPL_SLAVE_DD_Direction_020, UIMPL_SLAVE_DD_Direction_023}
template<>
UIMPL_SLAVE<256>::UIMPL_SLAVE(sc_module_name name, unsigned int rLatency, unsigned int wLatency) :
    sc_module( name )
    ,BusSlaveBase<256,1>()
    ,rvc_common_model()
    ,BusSlaveFuncIf()
    , clk("clk")
    , resetPort("resetPort")
    , BMID("BMID")
    , ERR("ERR")
{//{{{
    //-------------------------------------
    // Configure Slave socket for 256, 512, 1024 using template.
    BusSlaveBase<256, 1>::setSlaveResetPort(&resetPort);
    BusSlaveBase<256, 1>::setSlaveFreqPort(&clk);
    BusSlaveBase<256, 1>::setTargetSocket((char*)"ts");

    ts = BusSlaveBase<256, 1>::tSocket[0];

    BusSlaveBase<256, 1>::mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *) this);
    BusSlaveBase<256, 1>::mBusSlaveIf[0]->setBusProtocol(BUS_AXI);
    BusSlaveBase<256, 1>::mBusSlaveIf[0]->setReadInitLatency(rLatency);
    BusSlaveBase<256, 1>::mBusSlaveIf[0]->setWriteInitLatency(wLatency);
    BusSlaveBase<256, 1>::mBusSlaveIf[0]->setTransNmbLmt(0x1);
    mclkPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mIsResetPortActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mTimeCLKChangePoint = 0;
    
    mERRIntr = false; 
    mBMID = 0x0; 
    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    ERR.initialize(false);
    BMID.initialize(0x0);
    
    // SC_THREAD/METHOD
    SC_METHOD(HandleclkSignalMethod);
    dont_initialize();
    sensitive << clk;
    
    // processes handle reset sources
    SC_METHOD(HandleresetPortMethod);
    dont_initialize();
    sensitive << resetPort;

    SC_METHOD(HandleresetPortCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;
    
    SC_METHOD(WriteBMIDMethod);
    dont_initialize();
    sensitive << mBMIDEvent;
    
    SC_METHOD(WriteERRMethod);
    dont_initialize();
    sensitive << mERREvent;
    
    SC_METHOD(AssertERRIntrMethod);
    dont_initialize();
    sensitive << mAssertERREvent;
    
    SC_METHOD(DeassertERRIntrMethod);
    dont_initialize();
    sensitive << mDeassertERREvent;

    //Initialize signal, state
    Initialize();
    CommandInit((std::string)name);
}//}}}

// Description: Constructor of UIMPL_SLAVE<512>
// Ref: {UIMPL_SLAVE_DD_Direction_019, UIMPL_SLAVE_DD_Summary_001, UIMPL_SLAVE_DD_BlockDiagram_001, UIMPL_SLAVE_DD_Ports_001, UIMPL_SLAVE_DD_ATT1_001, UIMPL_SLAVE_DD_Direction_020, UIMPL_SLAVE_DD_Direction_023}
template<>
UIMPL_SLAVE<512>::UIMPL_SLAVE(sc_module_name name, unsigned int rLatency, unsigned int wLatency) :
    sc_module( name )
    ,BusSlaveBase<512,1>()
    ,rvc_common_model()
    ,BusSlaveFuncIf()
    , clk("clk")
    , resetPort("resetPort")
    , BMID("BMID")
    , ERR("ERR")
{//{{{
    //-------------------------------------
    // Configure Slave socket for 512, 512, 1024 using template.
    BusSlaveBase<512, 1>::setSlaveResetPort(&resetPort);
    BusSlaveBase<512, 1>::setSlaveFreqPort(&clk);
    BusSlaveBase<512, 1>::setTargetSocket((char*)"ts");

    ts = BusSlaveBase<512, 1>::tSocket[0];

    BusSlaveBase<512, 1>::mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *) this);
    BusSlaveBase<512, 1>::mBusSlaveIf[0]->setBusProtocol(BUS_AXI);
    BusSlaveBase<512, 1>::mBusSlaveIf[0]->setReadInitLatency(rLatency);
    BusSlaveBase<512, 1>::mBusSlaveIf[0]->setWriteInitLatency(wLatency);
    BusSlaveBase<512, 1>::mBusSlaveIf[0]->setTransNmbLmt(0x1);
    mclkPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mIsResetPortActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mTimeCLKChangePoint = 0;
    
    mERRIntr = false; 
    mBMID = 0x0; 
    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    ERR.initialize(false);
    BMID.initialize(0x0);
    
    // SC_THREAD/METHOD
    SC_METHOD(HandleclkSignalMethod);
    dont_initialize();
    sensitive << clk;
    
    // processes handle reset sources
    SC_METHOD(HandleresetPortMethod);
    dont_initialize();
    sensitive << resetPort;

    SC_METHOD(HandleresetPortCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;
    
    SC_METHOD(WriteBMIDMethod);
    dont_initialize();
    sensitive << mBMIDEvent;
    
    SC_METHOD(WriteERRMethod);
    dont_initialize();
    sensitive << mERREvent;
    
    SC_METHOD(AssertERRIntrMethod);
    dont_initialize();
    sensitive << mAssertERREvent;
    
    SC_METHOD(DeassertERRIntrMethod);
    dont_initialize();
    sensitive << mDeassertERREvent;

    //Initialize signal, state
    Initialize();
    CommandInit((std::string)name);
}//}}}

// Description: Constructor of UIMPL_SLAVE<1024>
// Ref: {UIMPL_SLAVE_DD_Direction_019, UIMPL_SLAVE_DD_Summary_001, UIMPL_SLAVE_DD_BlockDiagram_001, UIMPL_SLAVE_DD_Ports_001, UIMPL_SLAVE_DD_ATT1_001, UIMPL_SLAVE_DD_Direction_020, UIMPL_SLAVE_DD_Direction_023}
template<>
UIMPL_SLAVE<1024>::UIMPL_SLAVE(sc_module_name name, unsigned int rLatency, unsigned int wLatency) :
    sc_module( name )
    ,BusSlaveBase<1024,1>()
    ,rvc_common_model()
    ,BusSlaveFuncIf()
    , clk("clk")
    , resetPort("resetPort")
    , BMID("BMID")
    , ERR("ERR")
{//{{{
    //-------------------------------------
    // Configure Slave socket for 1024, 1024, 1024 using template.
    BusSlaveBase<1024, 1>::setSlaveResetPort(&resetPort);
    BusSlaveBase<1024, 1>::setSlaveFreqPort(&clk);
    BusSlaveBase<1024, 1>::setTargetSocket((char*)"ts");

    ts = BusSlaveBase<1024, 1>::tSocket[0];

    BusSlaveBase<1024, 1>::mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *) this);
    BusSlaveBase<1024, 1>::mBusSlaveIf[0]->setBusProtocol(BUS_AXI);
    BusSlaveBase<1024, 1>::mBusSlaveIf[0]->setReadInitLatency(rLatency);
    BusSlaveBase<1024, 1>::mBusSlaveIf[0]->setWriteInitLatency(wLatency);
    BusSlaveBase<1024, 1>::mBusSlaveIf[0]->setTransNmbLmt(0x1);
    mclkPeriod = 0;
    mclkOrgFreq = 0;
    mclkFreq = 0;
    mclkFreqUnit = "Hz";
    mIsResetPortActive = false;
    mIsResetCmdReceive = false;
    mIsResetCmdActive  = false;
    mResetCmdPeriod    = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mTimeCLKChangePoint = 0;
    
    mERRIntr = false; 
    mBMID = 0x0; 
    
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    ERR.initialize(false);
    BMID.initialize(0x0);
    
    // SC_THREAD/METHOD
    SC_METHOD(HandleclkSignalMethod);
    dont_initialize();
    sensitive << clk;
    
    // processes handle reset sources
    SC_METHOD(HandleresetPortMethod);
    dont_initialize();
    sensitive << resetPort;

    SC_METHOD(HandleresetPortCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mCancelResetCmdEvent;
    
    SC_METHOD(WriteBMIDMethod);
    dont_initialize();
    sensitive << mBMIDEvent;
    
    SC_METHOD(WriteERRMethod);
    dont_initialize();
    sensitive << mERREvent;
    
    SC_METHOD(AssertERRIntrMethod);
    dont_initialize();
    sensitive << mAssertERREvent;
    
    SC_METHOD(DeassertERRIntrMethod);
    dont_initialize();
    sensitive << mDeassertERREvent;

    //Initialize signal, state
    Initialize();
    CommandInit((std::string)name);
}//}}}

//------------------------------------------------------------------------------
// Description: Destructor of UIMPL_SLAVE
// Ref: {UIMPL_SLAVE_DD_Direction_019}
template<unsigned int BUS_WIDTH>
UIMPL_SLAVE<BUS_WIDTH>::~UIMPL_SLAVE()
{//{{{

}//}}}

/// Monitor clk clock
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::HandleclkSignalMethod(void)
{//{{{
    sc_dt::uint64 freq_value = clk.read();
    SetCLKFreq("clk", freq_value, "Hz");
}//}}}

// Description: Handle Reset command method
// Ref: {UIMPL_SLAVE_DD_Direction_002, UIMPL_SLAVE_DD_Direction_015}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::HandleresetPortCmdMethod()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of %s.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
    mCancelResetCmdEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

// Description: Handle cancel reset command method
// Ref: {UIMPL_SLAVE_DD_Direction_002, UIMPL_SLAVE_DD_Direction_014}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive  = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of %s is over.\n", resetPort.basename());
    this->EnableReset(mIsResetCmdActive);
}//}}}

// Description: Handle Reset port method
// Ref: {UIMPL_SLAVE_DD_Ports_004, UIMPL_SLAVE_DD_Diagram_003, UIMPL_SLAVE_DD_Direction_041, UIMPL_SLAVE_DD_Direction_040}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::HandleresetPortMethod()
{//{{{
    bool resetport_state = resetPort.read();

    if (resetport_state == vpcl::emResetActive){
        mIsResetPortActive = true;
        re_printf("info", "The reset port %s is asserted.\n", resetPort.basename());
        this->EnableReset(mIsResetPortActive);
    } else{ //reset de-active
        mIsResetPortActive = false;
        re_printf("info", "The reset port %s is de-asserted.\n", resetPort.basename());
    }
    // when resetPort is active, cancel reset command
    if(mIsResetPortActive && mIsResetCmdReceive){
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
        mResetCmdEvent.cancel();
        mCancelResetCmdEvent.cancel();
    }
}//}}}

// Description: Process when hard reset is asserted
// Ref: {UIMPL_SLAVE_DD_Diagram_003}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::EnableReset(bool isActive)
{//{{{
    if (isActive){
        // Cancel events
        CancelEvents();
        // Initialize variables
        Initialize();
        mERREvent.notify();
        mBMIDEvent.notify();
    }    
}//}}}

/// Cancel operation events
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::CancelEvents(void)
{//{{{
        mBMIDEvent.cancel();
        mERREvent.cancel();
        mAssertERREvent.cancel();
        mDeassertERREvent.cancel();
}//}}}

// Description: Initialize model when hard reset is asserted
// Ref: {UIMPL_SLAVE_DD_Ports_003}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::Initialize()
{//{{{ 
    mERRIntr = false; 
    mBMID = 0x0; 
}//}}}

//##############################################################################
// Part 1: PythonIF
//##############################################################################
//------------------------------------------------------------------------------
// Description: Process when Assert reset
// Ref: {UIMPL_SLAVE_DD_Ports_002, UIMPL_SLAVE_DD_Direction_002, UIMPL_SLAVE_DD_Direction_039, UIMPL_SLAVE_DD_Direction_038, UIMPL_SLAVE_DD_Direction_037, UIMPL_SLAVE_DD_Direction_036, UIMPL_SLAVE_DD_Direction_035, UIMPL_SLAVE_DD_Direction_034, UIMPL_SLAVE_DD_Direction_033}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::AssertReset(const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == resetPort.basename()) {
        if ((!mIsResetPortActive) && (!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (%s) for %f ns after %f ns.\n", resetPort.basename(), period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        }
        else {
            re_printf("warning", "The software reset of %s is called in the reset operation of the model. So it is ignored.\n", resetPort.basename());
        }
    }
    else {
        re_printf("warning","The reset name (%s) is wrong. It should be %s.\n", reset_name.c_str(), resetPort.basename());
    }
}//}}}

// Description: Set clock value and clock unit
// Ref: {UIMPL_SLAVE_DD_Diagram_008, UIMPL_SLAVE_DD_Ports_001, UIMPL_SLAVE_DD_Direction_046, UIMPL_SLAVE_DD_Direction_047, UIMPL_SLAVE_DD_Direction_048, UIMPL_SLAVE_DD_Direction_045, UIMPL_SLAVE_DD_Direction_043}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clk") {
            mclkOrgFreq = freq;
            ConvertClockFreq (mclkFreq, mclkFreqUnit, freq, unit);
            if (mclkFreq > 0) {
                mclkPeriod = (sc_dt::uint64)(((1/(double)mclkFreq) * (double)mTimeResolutionValue) + 0.5);
                mStartclkTime = sc_time_stamp().to_double();
            } else {
                mclkPeriod = 0;
                // Prepare for next run
                // Cancel events
                CancelEvents();
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::GetCLKFreq(const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "clk") {
            re_printf("info","clk frequency is %0.0f %s\n", (double)mclkOrgFreq, mclkFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

// Description: Dump help message of all parameters or commands
// Ref: {UIMPL_SLAVE_DD_Direction_009, UIMPL_SLAVE_DD_Direction_032}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_MessageLevel_Bus32 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for UIMPL_SLAVE<32>");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_MessageLevel_Bus64 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for UIMPL_SLAVE<64>");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_MessageLevel_Bus128 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for UIMPL_SLAVE<128>");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_MessageLevel_Bus256 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for UIMPL_SLAVE<256>");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_MessageLevel_Bus512 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for UIMPL_SLAVE<512>");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_MessageLevel_Bus1024 (fatal|error|warning|info) Set debug message level (Default: fatal|error) for UIMPL_SLAVE<1024>");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_AssertReset_Bus32 (reset_name, start_time, period) Assert and de-assert reset signal to the UIMPL_SLAVE<32> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_AssertReset_Bus64 (reset_name, start_time, period) Assert and de-assert reset signal to the UIMPL_SLAVE<64> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_AssertReset_Bus128 (reset_name, start_time, period) Assert and de-assert reset signal to the UIMPL_SLAVE<128> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_AssertReset_Bus256 (reset_name, start_time, period) Assert and de-assert reset signal to the UIMPL_SLAVE<256> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_AssertReset_Bus512 (reset_name, start_time, period) Assert and de-assert reset signal to the UIMPL_SLAVE<512> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_AssertReset_Bus1024 (reset_name, start_time, period) Assert and de-assert reset signal to the UIMPL_SLAVE<1024> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_SetCLKFreq_Bus32 (clock_name, freq, unit) Set frequency value to these blocks to the UIMPL_SLAVE<32> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_SetCLKFreq_Bus64 (clock_name, freq, unit) Set frequency value to these blocks to the UIMPL_SLAVE<64> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_SetCLKFreq_Bus128 (clock_name, freq, unit) Set frequency value to these blocks to the UIMPL_SLAVE<128> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_SetCLKFreq_Bus256 (clock_name, freq, unit) Set frequency value to these blocks to the UIMPL_SLAVE<256> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_SetCLKFreq_Bus512 (clock_name, freq, unit) Set frequency value to these blocks to the UIMPL_SLAVE<512> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_SetCLKFreq_Bus1024 (clock_name, freq, unit) Set frequency value to these blocks to the UIMPL_SLAVE<1024> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_GetCLKFreq_Bus32 (clock_name)Get frequency value of these blocks to the UIMPL_SLAVE<32> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_GetCLKFreq_Bus64 (clock_name)Get frequency value of these blocks to the UIMPL_SLAVE<64> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_GetCLKFreq_Bus128 (clock_name)Get frequency value of these blocks to the UIMPL_SLAVE<128> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_GetCLKFreq_Bus256 (clock_name)Get frequency value of these blocks to the UIMPL_SLAVE<256> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_GetCLKFreq_Bus512 (clock_name)Get frequency value of these blocks to the UIMPL_SLAVE<512> model");
            SC_REPORT_INFO(this->basename(),"UIMPL_SLAVE_GetCLKFreq_Bus1024 (clock_name)Get frequency value of these blocks to the UIMPL_SLAVE<1024> model");
        } else {
            re_printf("warning","The name (%s) of UIMPL_SLAVE_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of UIMPL_SLAVE_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// Description: SetMessageLevel command process
// Ref: {UIMPL_SLAVE_DD_Direction_001}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_ret = this->handleCommand(cmd);
    if (msg_ret != "") {
       SC_REPORT_INFO(this->basename(), msg_ret.c_str());
    }
}//}}}

// Description: Recieve write transaction.
// Ref: {UIMPL_SLAVE_DD_Feature_001, UIMPL_SLAVE_DD_ATT1_003, UIMPL_SLAVE_DD_ATT1_004}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::write (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    double current_time = sc_time_stamp().to_double();

    if (pExtInfo != NULL)
    {
        mBMID = pExtInfo->getPeId();
        mBMIDEvent.notify();
        mAssertERREvent.notify(CalculateCLKPosEdge((double)mclkPeriod, false, current_time, mStartclkTime), mTimeResolutionUnit);
    }
}//}}}

template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::read (unsigned int offsetAddress, TlmBasicPayload& trans, BusTime_t* t, bool debug)
{//{{{
    // Get extension info
    TlmG3mExtension *pExtInfo;
    trans.get_extension(pExtInfo);
    double current_time = sc_time_stamp().to_double();

    if (pExtInfo != NULL)
    {
        mBMID = pExtInfo->getPeId(); 
        mBMIDEvent.notify();
        mAssertERREvent.notify(CalculateCLKPosEdge((double)mclkPeriod, false, current_time, mStartclkTime), mTimeResolutionUnit);
    }
}//}}}

// Description: Write port ERR
// Ref: {UIMPL_SLAVE_DD_Ports_004}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::WriteERRMethod(void)
{//{{
    bool current_value = ERR.read();
    if (current_value != mERRIntr) {
        ERR.write(mERRIntr);
        DumpInterruptMsg("ERR", mERRIntr);
    }
}//}}}

// Description: Assert ERR interrupt
// Ref: {UIMPL_SLAVE_DD_Ports_004}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::AssertERRIntrMethod(void)
{//{{{
    mERRIntr = true;
    mERREvent.notify(SC_ZERO_TIME);

    // Deassert after 1 cycle
    double current_time = sc_time_stamp().to_double();
    mDeassertERREvent.notify(CalculateCLKPosEdge((double)mclkPeriod, false, current_time, mStartclkTime) + (double)mclkPeriod, mTimeResolutionUnit);
}//}}}

// Description: DeAssert ERR interrupt
// Ref: {UIMPL_SLAVE_DD_Ports_004}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::DeassertERRIntrMethod(void)
{//{{{
    mERRIntr = false;
    mERREvent.notify(SC_ZERO_TIME);
}//}}}

// Description: Write port BMID
// Ref: {UIMPL_SLAVE_DD_Diagram_002, UIMPL_SLAVE_DD_Ports_005}
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::WriteBMIDMethod()
{//{{{
        BMID.write(mBMID);
}//}}}

 //DumpInterrupt Message 
template<unsigned int BUS_WIDTH>
void UIMPL_SLAVE<BUS_WIDTH>::DumpInterruptMsg(const std::string intr_name, const bool value)
{//{{{
    if (value) {
        re_printf("info", "INT [%s] Assert.\n", intr_name.c_str());
    }
    else {
        re_printf("info", "INT [%s] De-assert.\n", intr_name.c_str());
    }
}//}}}

//
// Description: Template of UIMPL_SLAVE
// Ref: {UIMPL_SLAVE_DD_Direction_031}
template class UIMPL_SLAVE<32>;
template class UIMPL_SLAVE<64>;
template class UIMPL_SLAVE<128>;
template class UIMPL_SLAVE<256>;
template class UIMPL_SLAVE<512>;
template class UIMPL_SLAVE<1024>;

// vim600: set foldmethod=marker :
