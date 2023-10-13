// ---------------------------------------------------------------------
// $Id: G4_PEGCAP.cpp,v 1.12 2020/01/30 02:18:49 nhutminhdang Exp $
//
// Copyright(c) 2016-2020 Renesas Electronics Corporation
// Copyright(c) 2016-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------

#include "G4_PEGCAP.h"
#include "G4_PEGCAP_Func.h"

/*********************************
// Function     : G4_PEGCAP
// Description  : Constructor of G4_PEGCAP class
// Parameter    : 
//   name            module name
//   kind            platform kind
//   rLatency        read latency
//   wLatency        write latency
//   PEG_ID          ID of PE
//   CHANNEL_NUM     number of channel
// Return value : None
**********************************/
G4_PEGCAP::G4_PEGCAP (sc_module_name name, std::string kind, unsigned int rLatency, unsigned int wLatency)
                    :sc_module(name)
                    ,G4_PEGCAP_AgentController()
                    ,rvc_common_model()
                    ,BusSlaveBase<BUS_WIDTH_APB,1>()
                    ,PCLK("PCLK")
                    ,presetz("presetz")
                    ,GRDERR("GRDERR")
                    ,ERR_INFO("ERR_INFO")
{//{{{
    /// Initialize variables
    mPCLKPeriod   = 0;
    mTimeResolutionValue = 1;
    mTimeResolutionUnit = SC_NS;
    mStartPCLKTime = 0;
    mIsResetCmdActive      = false;
    mIsResetHardActive     = false;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);
    mKind = kind;
    Initialize();

    /// Initialize output ports
    GRDERR.initialize(mGRDERR);

    /// Initialize Bus
    setSlaveFreqPort32(&PCLK);
    setSlaveResetPort32(&presetz);
    setTargetSocket32("ts");

    ts = BusSlaveBase<BUS_WIDTH_APB,1>::tSocket32[0];

    /// Instance G4_PEGCAP_Func
    mG4_PEGCAP_Func = new G4_PEGCAP_Func((std::string)name, mKind, this);
    sc_assert(mG4_PEGCAP_Func != NULL);
    mBusSlaveIf32[0]->setFuncModulePtr(mG4_PEGCAP_Func);
    mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
    mBusSlaveIf32[0]->setReadInitLatency(rLatency);
    mBusSlaveIf32[0]->setWriteInitLatency(wLatency);
    mBusSlaveIf32[0]->setTransNmbLmt(0x1);
    //mBusSlaveIf32[0]->setEnableToCheckDataSize();

    /// SC_METHOD
    SC_METHOD(WriteGRDERRMethod);
    dont_initialize();
    sensitive << mWriteGRDERREvent;

    SC_METHOD(NegateGRDERRMethod);
    dont_initialize();
    sensitive << mNegateGRDERREvent;

    SC_METHOD(HandleERR_INFOMethod);
    dont_initialize();
    sensitive << ERR_INFO;
    
    SC_METHOD(ClearPEGSPIDERRCLRMethod);
    dont_initialize();
    sensitive << mClearPEGSPIDERRCLREvent;
}//}}}

/*********************************
// Function     : ~G4_PEGCAP
// Description  : Destructor of G4_PEGCAP class
// Parameter    : None
// Return value : None
**********************************/
G4_PEGCAP::~G4_PEGCAP (void)
{//{{{
     delete mG4_PEGCAP_Func;
}//}}}

/*********************************
// Function     : Initialize
// Description  : Initialize when reset activated
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP::Initialize(void)
{//{{{
    mDumpInterrupt   = false;
    mRstState        = false;
    mGRDERR          = false;
}//}}}

/*********************************
// Function     : WriteGRDERRMethod
// Description  : Write GRDERR port
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP::WriteGRDERRMethod()
{//{{{
    GRDERR.write(mGRDERR);
    if (mDumpInterrupt){
        re_printf("info", "Port GRDERR is changed to %d\n", (unsigned int)mGRDERR);
    }
    /// Negate after one clock period
    if (mGRDERR){
        mNegateGRDERREvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
    }
}//}}}

/*********************************
// Function     : WriteGRDERRMethod
// Description  : Negate GRDERR port
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP::NegateGRDERRMethod()
{//{{{
    mGRDERR = false;
    mWriteGRDERREvent.notify();
}//}}}

/*********************************
// Function     : HandleERR_INFOMethod
// Description  : Handle ERR_INFO port
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP::HandleERR_INFOMethod(void)
{//{{{
    G4_PEG_Cerrinfo temp_value;
    temp_value = ERR_INFO.read();
    if (temp_value.PEGWrERR != temp_value.PEGRdERR) {
        unsigned int cur_SPID = temp_value.SPID;
        
        /// Notify interrupt if error is detected different error
        if(!mG4_PEGCAP_Func->IsSameSPID(cur_SPID)) {
            mG4_PEGCAP_Func->SetERR_INFO(temp_value);
            
            mGRDERR = true;
            mWriteGRDERREvent.notify(CalculateCLKPosEdge((double)mPCLKPeriod, false, sc_time_stamp().to_double(), mStartPCLKTime), mTimeResolutionUnit);
        } else { /// Not notify interrupt
            mG4_PEGCAP_Func->SetERR_INFO(temp_value, true);
        }
    } else {
        /// ERR_INFO port value is wrong or is reseted.
    }
}//}}}

/*********************************
// Function     : CheckClockPeriod
// Description  : Check clock period value
// Parameter    : None
// Return value : None
**********************************/
bool G4_PEGCAP::CheckClockPeriod ()
{//{{{
    return (bool) (mPCLKPeriod > 0);
}//}}}

/*********************************
// Function     : GetResetStatus
// Description  : Checkreset status
// Parameter    : None
// Return value : None
**********************************/
bool G4_PEGCAP::GetResetStatus()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/*********************************
// Function     : CancelEvents
// Description  : Cancel operation events
// Parameter    : None
// Return value : None
**********************************/
void G4_PEGCAP::CancelEvents (void)
{//{{{
    mWriteGRDERREvent.cancel();
    mNegateGRDERREvent.cancel();
}//}}}

/*********************************
// Function     : SetCLKPeriod
// Description  : Set PCLK period
// Parameter    :
//   clk_period         new value of PCLK
// Return value : None
**********************************/
void G4_PEGCAP::SetCLKPeriod (const sc_dt::uint64 clk_period, const double time_change_point)
{//{{{
    mPCLKPeriod = clk_period;
    mStartPCLKTime = time_change_point;
}//}}}

/*********************************
// Function     : SetResetStatus
// Description  : Set reset status
// Parameter    :
//   mIsResetCmdActive     is reset by command
//   mIsResetHardActive    is reset by port
// Return value : None
**********************************/
void G4_PEGCAP::SetResetStatus(bool mIsResetCmdActive, bool mIsResetHardActive)
{//{{{
    this->mIsResetCmdActive = mIsResetCmdActive;
    this->mIsResetHardActive = mIsResetHardActive;
}//}}}

/*********************************
// Function     : RegisterHandler
// Description  : Wrapper for accessing registers
// Parameter    :
//        cmd     access command
// Return value : None
**********************************/
void G4_PEGCAP::RegisterHandler (const std::vector<std::string> cmd)
{//{{{
    mG4_PEGCAP_Func->RegisterHandler(cmd);
}//}}}

/*********************************
// Function     : EnableReset
// Description  : Execute reset operation
// Parameter    :
//    is_active   true: reset; false: not reset
// Return value : None
**********************************/
void G4_PEGCAP::EnableReset (const bool is_active)
{//{{{
    mRstState = is_active;
    mG4_PEGCAP_Func->EnableReset(is_active);
    if (is_active) {
        /// Cancel events
        CancelEvents();
        /// Initialize variable
        Initialize();
        /// Initialize output ports
        mWriteGRDERREvent.notify();
    } else {
        /// Do not reset
    }
}//}}}

/*********************************
// Function     : DumpInterrupt
// Description  : Enable/ Disable dumping interrupt information
// Parameter    :
//    is_active   true: dump; false: not dump
// Return value : None
**********************************/
void G4_PEGCAP::DumpInterrupt (const bool is_enable)
{//{{{
    mDumpInterrupt = is_enable;
}//}}}

/*********************************
// Function     : NotifyClearPEGSPIDERRCLREvent
// Description  : Notify trigger of ClearPEGSPIDERRCLRMethod method
// Parameter    : None
// Return value : none
**********************************/
void G4_PEGCAP::NotifyClearPEGSPIDERRCLREvent()
{//{{{
    mClearPEGSPIDERRCLREvent.notify(SC_ZERO_TIME);
}//}}}

/*********************************
// Function     : ClearPEGSPIDERRCLRMethod
// Description  : Call to ClearPEGSPIDERRCLR function of G4_PEGCAP_Func
// Parameter    : None
// Return value : none
**********************************/
void G4_PEGCAP::ClearPEGSPIDERRCLRMethod() 
{//{{{
    mG4_PEGCAP_Func->ClearPEGSPIDERRCLR();
}//}}}
// vim600: set foldmethod=marker :
