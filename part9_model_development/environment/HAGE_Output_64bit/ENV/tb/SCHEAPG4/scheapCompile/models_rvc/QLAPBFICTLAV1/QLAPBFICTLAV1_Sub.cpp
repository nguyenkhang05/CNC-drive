// ---------------------------------------------------------------------
// $Id: QLAPBFICTLAV1_Sub.cpp,v 1.3 2020/03/24 07:16:32 duongphan Exp $
//
// Copyright(c) 2018-2020 Renesas Electronics Corporation
// Copyright(c) 2018-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// ---------------------------------------------------------------------
#include "QLAPBFICTLAV1_Sub.h"
#include "QLAPBFICTLAV1_Sub_Func.h"
#include "QLAPBFICTLAV1.h"
#include "QLAPBFICTLAV1_Func.h"

///Constructor of QLAPBFICTLAV1_Sub class
QLAPBFICTLAV1_Sub::QLAPBFICTLAV1_Sub (sc_module_name name,
                                    QLAPBFICTLAV1 *iQLAPBFICTLAV1_Ptr,
                                    unsigned int ch_id):
sc_module(name),
QLAPBFICTLAV1_Sub_AgentController(),

// Input ports
FCLATEDSI("FCLATEDSI"),
FCLATNFSI("FCLATNFSI"),
FCLATSI("FCLATSI"),
// Output ports
FCLATINTR("FCLATINTR"),
FCLATINTF("FCLATINTF"),
FCLATSO("FCLATSO"),
FCLATEDSO("FCLATEDSO")
{//{{{
    assert(iQLAPBFICTLAV1_Ptr != NULL);
    mQLAPBFICTLAV1 = iQLAPBFICTLAV1_Ptr;
    mModuleName = (std::string)name;
    mChId = ch_id;

    mQLAPBFICTLAV1_Sub_Func = new QLAPBFICTLAV1_Sub_Func((std::string)name, this);

    sc_assert(mQLAPBFICTLAV1_Sub_Func != NULL);

    // Initialize variables & output ports
    FCLATINTR.initialize(false);
    FCLATINTF.initialize(false);
    FCLATSO.initialize(false);
    FCLATEDSO.initialize(false);
    Initialize();

    // Declare method/thread
    ///Input Handle Methods
    SC_METHOD(HandleFCLATEDSIMethod);
    dont_initialize();
    sensitive << FCLATEDSI;

    SC_METHOD(HandleFCLATNFSIMethod);
    dont_initialize();
    sensitive << FCLATNFSI;

    SC_METHOD(HandleFCLATSIMethod);
    dont_initialize();
    sensitive << FCLATSI;

    SC_METHOD(HandleActiveEffectiveINTLMethod);
    dont_initialize();
    sensitive << mBitFCLA0INTLSettingGetsEffective;

    SC_METHOD(HandleActiveEffectiveINTRMethod);
    dont_initialize();
    sensitive << mBitFCLA0INTRSettingGetsEffective;

    SC_METHOD(HandleUpLevelDetectMethod);
    dont_initialize();
    sensitive << mFCLATEDSOPortWriteUpEvent;

    SC_METHOD(HandleDowLevelDetectMethod);
    dont_initialize();
    sensitive << mFCLATEDSOPortWriteDownEvent;

    SC_METHOD(WriteFCLATSOPortsMethod);
    dont_initialize();
    sensitive << mFCLATSOPortsWriteEvent;

    SC_METHOD(CheckFCLATNFSIPortValueAtSyncTimeMethod);
    dont_initialize();
    sensitive << mFCLATNFSIPortValueCheckAtSyncTimeEvent;

    SC_METHOD(WriteFCLATEDSOPortsMethod);
    dont_initialize();
    sensitive << mFCLATEDSOPortWriteEvent;

    SC_METHOD(WriteFCLATINTFPortsMethod);
    dont_initialize();
    sensitive << mFCLATINTFPortWriteEvent;

    SC_METHOD(WriteFCLATINTRPortsMethod);
    dont_initialize();
    sensitive << mFCLATINTRPortWriteEvent;

}//}}}

///Destructor of QLAPBFICTLAV1_Sub class
QLAPBFICTLAV1_Sub::~QLAPBFICTLAV1_Sub (void){//{{{
    delete mQLAPBFICTLAV1_Sub_Func;
}//}}}


/// Cancel operation events
void QLAPBFICTLAV1_Sub::CancelEvents (void){//{{{
    mFCLATNFSIPortValueCheckAtSyncTimeEvent.cancel();
    mBitFCLA0INTRSettingGetsEffective.cancel();
    mBitFCLA0INTLSettingGetsEffective.cancel();
    mFCLATEDSOPortWriteUpEvent.cancel();
    mFCLATEDSOPortWriteDownEvent.cancel();
}//}}}

/// Initialize variables & output ports
void QLAPBFICTLAV1_Sub::Initialize (void){//{{{
    //Initialize variables
    mFCLATINTRValue = false;
    mFCLATINTFValue = false;
    mFCLATEDSOValue = false;
    mFCLATSOValue   = false;
}//}}}

//Port value after SMPRESZ
void QLAPBFICTLAV1_Sub::PortValueAtsmpresz (void){//{{{
    mFCLATINTRValue = false;
    mFCLATINTFValue = false;
    mFCLATINTRPortWriteEvent.notify();
    mFCLATINTFPortWriteEvent.notify();

    mFCLATSOValue = FCLATNFSI.read();
    mFCLATEDSOValue = FCLATEDSI.read();
    mFCLATSOPortsWriteEvent.notify();
    mFCLATEDSOPortWriteEvent.notify();
}//}}}

/***********************************
Function        : EnableReset
Description     : Reset is called
***********************************/
void QLAPBFICTLAV1_Sub::EnableReset (const std::string reset_name, const bool is_active){//{{{
    if(reset_name=="PRESETZ") {
        mQLAPBFICTLAV1_Sub_Func->Reset(is_active);
    }
    else if(is_active) {
        Initialize();
        PortValueAtsmpresz();
        CancelEvents();
    }
}//}}}
/***********************************
Function        : CancelEventsZeroClock
Description     : Cancel Event Check detect function when SMPCLK =0
***********************************/
void QLAPBFICTLAV1_Sub::CancelEventsZeroClock(void){//{{{
    mFCLATNFSIPortValueCheckAtSyncTimeEvent.cancel();
}//}}}

/***********************************
Function        : NotifyEffectiveBitSetting
Description     : Notify setting 3 to 4 SMPCLK for detect function
***********************************/

void QLAPBFICTLAV1_Sub::NotifyEffectiveBitSetting (const std::string effective_name){//{{{

    if (effective_name == "FCLATINTL"){
        mBitFCLA0INTLSettingGetsEffective.notify(mQLAPBFICTLAV1->CalculateClockEdge("SMPCLK") + EffectivePeriodsSmpclk());
        re_printf("info","level detector function is enable after 3 to 4 periods in SMPCLK.\n");
    }else {
        mBitFCLA0INTRSettingGetsEffective.notify(mQLAPBFICTLAV1->CalculateClockEdge("SMPCLK") + EffectivePeriodsSmpclk());
        re_printf("info","level detector function  will start after 3 to 4 periods in SMPCLK.\n");
        mFCLATEDSOPortWriteDownEvent.notify(mQLAPBFICTLAV1->CalculateClockEdge("SMPCLK") + sc_time((double)(mQLAPBFICTLAV1->mfclasmpclkPeriod) * 2, mQLAPBFICTLAV1->mTimeResolutionUnit));
    }
}//}}}

/***********************************
Function        : NotifyEffectiveBitSetting
Description     : Define prior number effect = 3 for detect function
***********************************/
sc_time QLAPBFICTLAV1_Sub::EffectivePeriodsSmpclk(void){//{{{
    return sc_time((double)(mQLAPBFICTLAV1->mfclasmpclkPeriod) * emSyncSmpclkNum, mQLAPBFICTLAV1->mTimeResolutionUnit);
}//}}}

/***********************************
Function        : NotifyEdgeDetectorSignal
Description     : Notify after setting for edge detector
***********************************/

void QLAPBFICTLAV1_Sub::NotifyEdgeDetectorSignal (const std::string edge_name){//{{{
    if (edge_name == "FCLATINTF"){
        mFCLATINTFValue = mQLAPBFICTLAV1_Sub_Func->GetINTF();
        mFCLATINTFPortWriteEvent.notify(mQLAPBFICTLAV1->CalculateClockEdge("PCLK"));
    } else {
        mFCLATINTRValue = mQLAPBFICTLAV1_Sub_Func->GetINTR();
        mFCLATINTRPortWriteEvent.notify(mQLAPBFICTLAV1->CalculateClockEdge("PCLK"));
    }
}//}}}

/***********************************
Function        : NotifyFilterSignal
Description     : Notify after setting for bypass function
***********************************/

void QLAPBFICTLAV1_Sub::NotifyFilterSignal(const std::string signal_name){
    if (signal_name == "FilterSignal"){
        mFCLATSOValue = FCLATNFSI.read();
        mFCLATSOPortsWriteEvent.notify(SC_ZERO_TIME);
    } else {
        mFCLATSOValue = FCLATSI.read();
        mFCLATSOPortsWriteEvent.notify(SC_ZERO_TIME);
    }
}

/***********************************
Function        : NotifyEdgeSignal
Description     : Notify after setting for Edge function
***********************************/

void QLAPBFICTLAV1_Sub::NotifyEdgeSignal(void){
    mFCLATEDSOValue = FCLATEDSI.read();
    mFCLATEDSOPortWriteEvent.notify(SC_ZERO_TIME);
}
/***********************************
Function        : HandleFCLATEDSIMethod
Description     : Handle FCLATEDSI signal
***********************************/

void QLAPBFICTLAV1_Sub::HandleFCLATEDSIMethod (void){//{{{
    if (!mQLAPBFICTLAV1->GetResetStatus("SMPRESZ") ){
        if (!mQLAPBFICTLAV1_Sub_Func ->GetINTL()){
            mFCLATEDSOValue = FCLATEDSI.read();
            mFCLATEDSOPortWriteEvent.notify(SC_ZERO_TIME);
        }else{
            re_printf("info","edge detector function is disable.\n");
        }
    }
}//}}}

/***********************************
Function        : HandleFCLATNFSIMethod
Description     : Handle FCLATNFSI signal
***********************************/

void QLAPBFICTLAV1_Sub::HandleFCLATNFSIMethod (void){//{{{
    if(!mQLAPBFICTLAV1->GetResetStatus("SMPRESZ")){
        if (!mQLAPBFICTLAV1_Sub_Func ->GetBYPS()){
            mFCLATSOValue = FCLATNFSI.read();
            mFCLATSOPortsWriteEvent.notify(SC_ZERO_TIME);
        }else{
            re_printf("info","filter signal is disable.\n");
        }
        if (mQLAPBFICTLAV1_Sub_Func->GetStartDetector()
         && mQLAPBFICTLAV1->CheckClockPeriod("SMPCLK")){
            mFCLATNFSIPortValueCheckAtSyncTimeEvent.notify(mQLAPBFICTLAV1->CalculateClockEdge("SMPCLK"));
        }else{
            re_printf("info","level detector function is disable.\n");
        }
    }
}//}}}

/***********************************
Function        : HandleFCLATSIMethod
Description     : Handle FCLATSI signal
***********************************/

void QLAPBFICTLAV1_Sub::HandleFCLATSIMethod (void){//{{{
    if (!mQLAPBFICTLAV1->GetResetStatus("SMPRESZ")){
        if (mQLAPBFICTLAV1_Sub_Func ->GetBYPS()){
            mFCLATSOValue = FCLATSI.read();
            mFCLATSOPortsWriteEvent.notify(SC_ZERO_TIME);
        }else{
            re_printf("info","filter-bypass function is disable.\n");
        }
    }
}//}}}

/***********************************
Function        : HandleActiveEffectiveINTLMethod
Description     : Enable  detector function
***********************************/

void QLAPBFICTLAV1_Sub::HandleActiveEffectiveINTLMethod (void){//{{{
    mQLAPBFICTLAV1_Sub_Func->SetStartDetector();
    mFCLATNFSIPortValueCheckAtSyncTimeEvent.notify(mQLAPBFICTLAV1->CalculateClockEdge("SMPCLK"));
    re_printf("info","level detector function is enable.\n");
}//}}}

/***********************************
Function        : HandleActiveEffectiveINTRMethod
Description     : Enable start detector function
***********************************/

void QLAPBFICTLAV1_Sub::HandleActiveEffectiveINTRMethod (void){//{{{
    mQLAPBFICTLAV1_Sub_Func->SetLevelDetector();
    if(mQLAPBFICTLAV1_Sub_Func->GetINTR()){
        re_printf("info","high level detector function start.\n");
    }else{
        re_printf("info","low level detector function start.\n");
    }
}//}}}

/***********************************
Function        : WriteFCLATSOPortsMethod
Description     : Write FCLATSO Port
***********************************/

void QLAPBFICTLAV1_Sub::WriteFCLATSOPortsMethod (void){//{{{
    FCLATSO.write(mFCLATSOValue);
}//}}}

/***********************************
Function        : CheckFCLATNFSIPortValueAtSyncTimeMethod
Description     : Check FCLATNFSIPort Value at Sync Time
***********************************/

void QLAPBFICTLAV1_Sub::CheckFCLATNFSIPortValueAtSyncTimeMethod(void){//{{{
    bool FCLATNFSIValueAtSyncTime = FCLATNFSI.read();
    if(mQLAPBFICTLAV1_Sub_Func->GetLevelDetector()){
        if (((FCLATNFSIValueAtSyncTime == true)&&(mQLAPBFICTLAV1_Sub_Func->GetINTR()))
          ||((FCLATNFSIValueAtSyncTime == false) && (!mQLAPBFICTLAV1_Sub_Func->GetINTR()))){
            mFCLATEDSOPortWriteUpEvent.notify( sc_time((double)(mQLAPBFICTLAV1->mfclasmpclkPeriod) * 2, mQLAPBFICTLAV1->mTimeResolutionUnit));
        } else {
            mFCLATEDSOPortWriteDownEvent.notify(sc_time((double)(mQLAPBFICTLAV1->mfclasmpclkPeriod) * 2, mQLAPBFICTLAV1->mTimeResolutionUnit));
        }
    }
}//}}}

/***********************************
Function        : HandleUpLevelDetectMethod
Description     : Create pulse Level Detect
***********************************/

void QLAPBFICTLAV1_Sub::HandleUpLevelDetectMethod(void){//{{{
    mFCLATEDSOValue = true;
    mFCLATEDSOPortWriteEvent.notify(SC_ZERO_TIME);
}//}}}

/***********************************
Function        : HandleDowLevelDetectMethod
Description     : Clear pulse Level Detect
***********************************/

void QLAPBFICTLAV1_Sub::HandleDowLevelDetectMethod(void){//{{{
    mFCLATEDSOValue = false;
    mFCLATEDSOPortWriteEvent.notify(SC_ZERO_TIME);
}//}}}

/***********************************
Function        : WriteFCLATEDSOPortsMethod
Description     : Write FCLATEDSO Port
***********************************/

void QLAPBFICTLAV1_Sub::WriteFCLATEDSOPortsMethod (void){//{{{
    FCLATEDSO.write(mFCLATEDSOValue);
}//}}}

/***********************************
Function        : WriteFCLATINTFPortsMethod
Description     : Write FCLATINTF Port
***********************************/

void QLAPBFICTLAV1_Sub::WriteFCLATINTFPortsMethod (void){//{{{
    FCLATINTF.write(mFCLATINTFValue);
}//}}}

/***********************************
Function        : WriteFCLATINTRPortsMethod
Description     : Write FCLATINTR Port
***********************************/

void QLAPBFICTLAV1_Sub::WriteFCLATINTRPortsMethod (void){//{{{
    FCLATINTR.write(mFCLATINTRValue);
}//}}}
// vim600: set foldmethod=marker :
