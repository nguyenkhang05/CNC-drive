// ---------------------------------------------------------------------
// $Id: EMU3S_APB_DEC.cpp,v 1.6 2020/11/18 18:52:19 thiepnguyen Exp $
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
#include "EMU3S_APB_DEC.h"

/// constructor
EMU3S_APB_DEC::EMU3S_APB_DEC(sc_module_name name, SIM_MODE_T simmode):
     BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB>(name)
    ,BusBridgeSlaveBase<BUS_WIDTH_APB,1>()
    ,BusMasterBase<BUS_WIDTH_APB,1>()
    ,PCLK( "PCLK" )
    ,PRESETn( "PRESETn" )
    ,EMU3_STOP_REQ("EMU3_STOP_REQ")
    ,APBDEC_STOP_ACK("AHBARB_STOP_ACK")
{//{{{

    BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setSlaveResetPort(&PRESETn);
    BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setSlaveFreqPort(&PCLK);
    BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::setTargetSocket((char*)"tsp_cpu");

    BusMasterBase<BUS_WIDTH_APB,1>::setMasterResetPort32(&PRESETn);
    BusMasterBase<BUS_WIDTH_APB,1>::setMasterFreqPort32(&PCLK);
    BusMasterBase<BUS_WIDTH_APB,1>::setInitiatorSocket32((char*)"isp_cpu");

    isp_cpu = BusMasterBase<BUS_WIDTH_APB, 1>::iSocket32[0];
    tsp_cpu = BusBridgeSlaveBase<BUS_WIDTH_APB, 1>::tSocket[0];

    //Instance EMU3S_APB_DEC_Func
    mEMU3S_APB_DEC_Func = new EMU3S_APB_DEC_Func((std::string)name, this, mBusSlaveIf[0], mBusMasterIf32[0], simmode);
    if(mEMU3S_APB_DEC_Func == NULL){
        return;
    }

    // Description: Initialize member of BusBridgeModuleBase class
    // Ref: {EMU3SSS_UD_ATT1_011}
    BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::mpCore = mEMU3S_APB_DEC_Func;
    BusBridgeModuleBase<BUS_WIDTH_APB,BUS_WIDTH_APB>::resetPort(PRESETn);
    mBusSlaveIf[0]->setFuncModulePtr((BusSlaveFuncIf *)mEMU3S_APB_DEC_Func);
    mBusSlaveIf[0]->setBusProtocol(BUS_AHB);
    mBusSlaveIf[0]->setTransNmbLmt(0xFFFFFFFF);
    mBusMasterIf32[0]->setFuncModulePtr((BusMasterFuncIf *)mEMU3S_APB_DEC_Func);
    mBusMasterIf32[0]->setBusProtocol(BUS_AHB);
    mBusMasterIf32[0]->setTransNmbLmt(0xFFFFFFFF);
    mBusMasterIf32[0]->setTlmTransType(simmode);

    Initialize();
    //Initialize variables
    mResetCurVal            = true;
    mResetPreVal            = true;
    mIsResetHardActive      = false;

    mPCLKPeriod             = 0;
    mPCLKFreq               = 0;
    mPCLKFreqUnit           = "Hz";
    mStartPCLKTime          = 0;
    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;

    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    
    APBDEC_STOP_ACK.initialize(false);
    
    SC_METHOD(HandlePCLKSignalMethod);
    dont_initialize();
    sensitive << PCLK;

    SC_METHOD(HandlePRESETNSignalMethod);
    dont_initialize();
    sensitive << PRESETn;

    SC_METHOD(HandleResetHardMethod);
    dont_initialize();
    sensitive << mResetHardEvent;

    SC_METHOD(HandleStopRequestMethod);
    dont_initialize();
    sensitive << EMU3_STOP_REQ;

    SC_METHOD(HandleStopAckMethod);
    dont_initialize();
    sensitive << mwrite_apb_stop_ack_event;

    SC_METHOD(HandleBusStateMethod);
    dont_initialize();
    sensitive << mEMU3S_APB_DEC_Func->m_bus_competion_event;

}//}}}

// Description: Destructor of EMU3S_APB_DEC class
EMU3S_APB_DEC::~EMU3S_APB_DEC( void )
{//{{{
    delete mEMU3S_APB_DEC_Func;
}//}}}

// Description: Initial variables
void EMU3S_APB_DEC::Initialize( void )
{
    mapb_stop_ack               = false;
    mis_waiting_completion      = false;
    mwrite_apb_stop_ack_event.cancel();
}

// Description: Handle PCLK signal 
void EMU3S_APB_DEC::HandlePCLKSignalMethod( void ) 
{
    sc_dt::uint64 freq_value = PCLK.read();
    SetCLKFreq("PCLK", freq_value, "Hz");
}

// Description: Handle PRESETn signal
void EMU3S_APB_DEC::HandlePRESETNSignalMethod( void )
{
    mResetCurVal = PRESETn.read();
    if (mResetCurVal != mResetPreVal) {
        mResetPreVal = mResetCurVal;
        // Reset signals are asynchronous
        mResetHardEvent.notify(SC_ZERO_TIME);
    }
}

// Description: Handle hard reset
void EMU3S_APB_DEC:: HandleResetHardMethod( void )
{
    std::stringstream stream;

    if (mResetCurVal == emResetActive) {
        mIsResetHardActive = true;
        this->EnableReset("PRESETn", mIsResetHardActive);
    } else {
        mIsResetHardActive = false;
        this->EnableReset("PRESETn", mIsResetHardActive);
    }
}

// Description: Operation reset
void EMU3S_APB_DEC::EnableReset (const std::string reset_name, const bool is_active)
{//{{{
    mEMU3S_APB_DEC_Func->EnableReset(is_active);
    if (is_active) {
        Initialize();
    }
}//}}}

// Description: Handle bus stop req and bus state
// Ref: {EMU3SSS_UD_Flow_011}
void EMU3S_APB_DEC::HandleStopRequestMethod( void )
{
    bool memu3_stop_req = false;
    memu3_stop_req = EMU3_STOP_REQ.read();
    //memu3_stop_req assert
    if(memu3_stop_req)
    {
        if(mEMU3S_APB_DEC_Func->mis_bus_busy == true)
        {
            mEMU3S_APB_DEC_Func->mbus_stop_ack = false;
            mis_waiting_completion = true;
        }
        else
        {
            mapb_stop_ack = true;
            mEMU3S_APB_DEC_Func->mbus_stop_ack = true;
            mwrite_apb_stop_ack_event.notify(SC_ZERO_TIME);
        }
    }
    else //deassert
    {
        mis_waiting_completion = false;
        mapb_stop_ack = false;
        mEMU3S_APB_DEC_Func->mbus_stop_ack = false; 
        mwrite_apb_stop_ack_event.notify(SC_ZERO_TIME);
    }
}

// Description: Handle state of bus
void EMU3S_APB_DEC::HandleBusStateMethod( void )
{   
    std::stringstream stream;  
    mEMU3S_APB_DEC_Func->mis_bus_busy = false;

    if(mis_waiting_completion)
    {
        mis_waiting_completion              = false;
        mapb_stop_ack                       = true;
        mEMU3S_APB_DEC_Func->mbus_stop_ack  = true;
        mwrite_apb_stop_ack_event.notify(SC_ZERO_TIME);

    }
}

// Description: Handle stop ack signal
void EMU3S_APB_DEC::HandleStopAckMethod( void )
{
    if( mIsResetHardActive != true && mPCLKPeriod != 0)
    {
       APBDEC_STOP_ACK.write(mapb_stop_ack);
    }
}

void EMU3S_APB_DEC::SetCLKFreq(const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    ConvertClockFreq(mPCLKFreq, mPCLKFreqUnit, freq, unit);
    if (mPCLKFreq > 0) 
    {
        mPCLKPeriod = (sc_dt::uint64)(((1 / (double)mPCLKFreq) * (double)mTimeResolutionValue) + 0.5);
        mStartPCLKTime = sc_time_stamp().to_double();
    }
    else
    {
        mPCLKPeriod = 0;
        mzero_clock_event.notify(SC_ZERO_TIME);
    }
}//}}}
// vim600: set foldmethod=marker :
