// ---------------------------------------------------------------------
// $Id: ECC_BUS_SEP_AD.cpp,v 1.13 2020/04/03 07:30:47 thunguyen3 Exp $
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

#include "ECC_BUS_SEP_AD.h"
#include "ECC_BUS_AECCCTL_Func.h"
#include "ECC_BUS_DECCCTL_Func.h"
#include "ECC_BUS_ECCCAP_Func.h"
#include <iostream>

/// Constructor and Destructor
ECC_BUS_SEP_AD::ECC_BUS_SEP_AD(sc_module_name name, std::string err_kind, const unsigned int rLatency, const unsigned int wLatency, const unsigned int ena_module)
        :sc_module(name)
        ,ECC_BUS_SEP_AD_AgentController()
        // Clock/Reset
        ,pclk("pclk")
        ,presetn("presetn")
        // Key open
        ,keyopen("keyopen")
{//{{{
    mErr_Kind       = err_kind;
    if(mErr_Kind != "AA" && mErr_Kind != "DD" && mErr_Kind != "AD") {
        re_printf("info","Error kind \'%s\' is wrong, it is set to the default value \'AD\' automatically.\n", mErr_Kind.c_str());
        mErr_Kind = "AD";
    } else {
        re_printf("info","Current error kind is \'%s\'\n", mErr_Kind.c_str());
    }

    mEna_Module = ena_module;
    mNum_Module = 0;
    mENA_AECCCTL = false;
    mENA_DECCCTL = false;
    mENA_ECCCAP  = false;

    //mArr_Module = {};
    if (mEna_Module <= 0x7 && mEna_Module != 0x0) { //check mEna_Module is a 3-bit binary and greater than 0
        if (((mEna_Module >> 2) & 0x1) != 0x0) { 
            mENA_AECCCTL = true;
            mNum_Module++;
            mArr_Module.push_back((char*)"ts_aeccctl");
            re_printf("info","Module ECC_BUS_AECCCTL is enable");
        }
        if (((mEna_Module >> 1) & 0x1) != 0x0) { 
            mENA_DECCCTL = true;
            mNum_Module++;
            mArr_Module.push_back((char*)"ts_deccctl");
            re_printf("info","Module ECC_BUS_DECCCTL is enable");
        }
        if ((mEna_Module & 0x1) != 0x0) { 
            mENA_ECCCAP  = true;
            mNum_Module++;
            mArr_Module.push_back((char*)"ts_ecccap");
            re_printf("info","Module ECC_BUS_ECCCAP is enable");
        }
    } else { // mEna_Module = 0x0 or mEna_Module > 0x7
        re_printf("info","Kind of enable module \'0x%X\' is wrong, it is set to the default is enable all modules (AECCCTL,DECCCTL,ECCCAP).\n", mEna_Module);
        mENA_AECCCTL = true;
        mENA_DECCCTL = true;
        mENA_ECCCAP  = true;
        //mArr_Module = {"ts_aeccctl", "ts_deccctl", "ts_ecccap"}; 
        mArr_Module.push_back((char*)"ts_aeccctl");
        mArr_Module.push_back((char*)"ts_deccctl");
        mArr_Module.push_back((char*)"ts_ecccap");
        mEna_Module = 0x7;
        mNum_Module = 3;
    }

    // Configure Slave socket
    switch (mNum_Module) {
        case 1:
            mpBusSlaveBase1 = new BusSlaveBase<32, 1>;
            mpBusSlaveBase1->setSlaveResetPort32(&presetn);
            mpBusSlaveBase1->setSlaveFreqPort32(&pclk);
            mpBusSlaveBase1->setTargetSocket32(mArr_Module[0]);
            if (mENA_AECCCTL == true) {
                aeccdisable = new sc_out<bool>("aeccdisable");
                mpAECCCTL_Func = new ECC_BUS_AECCCTL_Func((std::string)name, this, mErr_Kind);
                ts_aeccctl = mpBusSlaveBase1->tSocket32[0];
                mpBusSlaveBase1->mBusSlaveIf32[0]->setFuncModulePtr(mpAECCCTL_Func);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setReadLatency(rLatency);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setWriteLatency(wLatency);
                // Initialize remain output ports
                aeccdisable->initialize(false);
            }
            if (mENA_DECCCTL == true) {
                secdisable = new sc_out<bool>("secdisable");
                eccdisable = new sc_out<bool>("eccdisable");
                mpDECCCTL_Func = new ECC_BUS_DECCCTL_Func((std::string)name, this, mErr_Kind);
                ts_deccctl = mpBusSlaveBase1->tSocket32[0];
                mpBusSlaveBase1->mBusSlaveIf32[0]->setFuncModulePtr(mpDECCCTL_Func);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setReadLatency(rLatency);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setWriteLatency(wLatency);
                // Initialize remain output ports
                secdisable->initialize(false);
                eccdisable->initialize(false);
            }
            if (mENA_ECCCAP  == true) {
                adederrout = new sc_out<bool>("adederrout");
                asederrout = new sc_out<bool>("asederrout");
                dederrout  = new sc_out<bool>("dederrout");
                sederrout  = new sc_out<bool>("sederrout");
                mpECCCAP_Func = new ECC_BUS_ECCCAP_Func((std::string)name, this, mErr_Kind);
                ts_ecccap = mpBusSlaveBase1->tSocket32[0];
                mpBusSlaveBase1->mBusSlaveIf32[0]->setFuncModulePtr(mpECCCAP_Func);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setReadLatency(rLatency);
                mpBusSlaveBase1->mBusSlaveIf32[0]->setWriteLatency(wLatency);
                // Initialize remain output ports
                adederrout->initialize(false);
                asederrout->initialize(false);
                dederrout->initialize(false);
                sederrout->initialize(false);
            }
            break;
        case 2:
            mpBusSlaveBase2 = new BusSlaveBase<32, 2>;
            mpBusSlaveBase2->setSlaveResetPort32(&presetn, &presetn);
            mpBusSlaveBase2->setSlaveFreqPort32(&pclk, &pclk);
            mpBusSlaveBase2->setTargetSocket32(mArr_Module[0], mArr_Module[1]);
            if (mENA_AECCCTL == true) {
                aeccdisable = new sc_out<bool>("aeccdisable");
                mpAECCCTL_Func = new ECC_BUS_AECCCTL_Func((std::string)name, this, mErr_Kind);
                ts_aeccctl = mpBusSlaveBase2->tSocket32[0]; //this module position is always 0 in tSocket array if it is set in constructor 
                mpBusSlaveBase2->mBusSlaveIf32[0]->setFuncModulePtr(mpAECCCTL_Func);
                mpBusSlaveBase2->mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
                mpBusSlaveBase2->mBusSlaveIf32[0]->setReadLatency(rLatency);
                mpBusSlaveBase2->mBusSlaveIf32[0]->setWriteLatency(wLatency);
                // Initialize remain output ports
                aeccdisable->initialize(false);
            }
            if (mENA_DECCCTL == true) {
                secdisable = new sc_out<bool>("secdisable");
                eccdisable = new sc_out<bool>("eccdisable");
                mpDECCCTL_Func = new ECC_BUS_DECCCTL_Func((std::string)name, this, mErr_Kind);
                ts_deccctl = mpBusSlaveBase2->tSocket32[(mEna_Module >> 2) & 0x1]; //this module position is 0 or 1 in tSocket array if it is set in constructor
                mpBusSlaveBase2->mBusSlaveIf32[(mEna_Module >> 2) & 0x1]->setFuncModulePtr(mpDECCCTL_Func);
                mpBusSlaveBase2->mBusSlaveIf32[(mEna_Module >> 2) & 0x1]->setBusProtocol(BUS_APB);
                mpBusSlaveBase2->mBusSlaveIf32[(mEna_Module >> 2) & 0x1]->setReadLatency(rLatency);
                mpBusSlaveBase2->mBusSlaveIf32[(mEna_Module >> 2) & 0x1]->setWriteLatency(wLatency);
                // Initialize remain output ports
                secdisable->initialize(false);
                eccdisable->initialize(false);
            }
            if (mENA_ECCCAP  == true) {
                adederrout = new sc_out<bool>("adederrout");
                asederrout = new sc_out<bool>("asederrout");
                dederrout  = new sc_out<bool>("dederrout");
                sederrout  = new sc_out<bool>("sederrout");
                mpECCCAP_Func = new ECC_BUS_ECCCAP_Func((std::string)name, this, mErr_Kind); //this module position is 0 or 1 or 2 in tSocket array if it is set in constructor
                ts_ecccap = mpBusSlaveBase2->tSocket32[((mEna_Module >> 1) & 0x1) + ((mEna_Module >> 2) & 0x1)];
                mpBusSlaveBase2->mBusSlaveIf32[((mEna_Module >> 1) & 0x1) + ((mEna_Module >> 2) & 0x1)]->setFuncModulePtr(mpECCCAP_Func);
                mpBusSlaveBase2->mBusSlaveIf32[((mEna_Module >> 1) & 0x1) + ((mEna_Module >> 2) & 0x1)]->setBusProtocol(BUS_APB);
                mpBusSlaveBase2->mBusSlaveIf32[((mEna_Module >> 1) & 0x1) + ((mEna_Module >> 2) & 0x1)]->setReadLatency(rLatency);
                mpBusSlaveBase2->mBusSlaveIf32[((mEna_Module >> 1) & 0x1) + ((mEna_Module >> 2) & 0x1)]->setWriteLatency(wLatency);
                // Initialize remain output ports
                adederrout->initialize(false);
                asederrout->initialize(false);
                dederrout->initialize(false);
                sederrout->initialize(false);
            }
            break;
        default:
            aeccdisable = new sc_out<bool>("aeccdisable");
            secdisable = new sc_out<bool>("secdisable");
            eccdisable = new sc_out<bool>("eccdisable");
            adederrout = new sc_out<bool>("adederrout");
            asederrout = new sc_out<bool>("asederrout");
            dederrout  = new sc_out<bool>("dederrout");
            sederrout  = new sc_out<bool>("sederrout");

            mpAECCCTL_Func = new ECC_BUS_AECCCTL_Func((std::string)name, this, mErr_Kind);
            mpDECCCTL_Func = new ECC_BUS_DECCCTL_Func((std::string)name, this, mErr_Kind);
            mpECCCAP_Func = new ECC_BUS_ECCCAP_Func((std::string)name, this, mErr_Kind);

            mpBusSlaveBase3 = new BusSlaveBase<32, 3>;
            mpBusSlaveBase3->setSlaveResetPort32(&presetn, &presetn, &presetn);
            mpBusSlaveBase3->setSlaveFreqPort32(&pclk, &pclk, &pclk);
            mpBusSlaveBase3->setTargetSocket32((char*)"ts_aeccctl", (char*)"ts_deccctl", (char*)"ts_ecccap");

            ts_aeccctl = mpBusSlaveBase3->tSocket32[0];
            mpBusSlaveBase3->mBusSlaveIf32[0]->setFuncModulePtr(mpAECCCTL_Func);
            mpBusSlaveBase3->mBusSlaveIf32[0]->setBusProtocol(BUS_APB);
            mpBusSlaveBase3->mBusSlaveIf32[0]->setReadLatency(rLatency);
            mpBusSlaveBase3->mBusSlaveIf32[0]->setWriteLatency(wLatency);

            ts_deccctl = mpBusSlaveBase3->tSocket32[1];
            mpBusSlaveBase3->mBusSlaveIf32[1]->setFuncModulePtr(mpDECCCTL_Func);
            mpBusSlaveBase3->mBusSlaveIf32[1]->setBusProtocol(BUS_APB);
            mpBusSlaveBase3->mBusSlaveIf32[1]->setReadLatency(rLatency);
            mpBusSlaveBase3->mBusSlaveIf32[1]->setWriteLatency(wLatency);

            ts_ecccap = mpBusSlaveBase3->tSocket32[2];
            mpBusSlaveBase3->mBusSlaveIf32[2]->setFuncModulePtr(mpECCCAP_Func);
            mpBusSlaveBase3->mBusSlaveIf32[2]->setBusProtocol(BUS_APB);
            mpBusSlaveBase3->mBusSlaveIf32[2]->setReadLatency(rLatency);
            mpBusSlaveBase3->mBusSlaveIf32[2]->setWriteLatency(wLatency);

            // Initialize remain output ports
            aeccdisable->initialize(false);
            secdisable->initialize(false);
            eccdisable->initialize(false);
            adederrout->initialize(false);
            asederrout->initialize(false);
            dederrout->initialize(false);
            sederrout->initialize(false);
            break;
    }

    mPCLKPeriod     = 0;
    mPCLKOrgFreq    = 0;
    mPCLKFreq       = 0;
    mPCLKFreqUnit   = "Hz";
    mStartPCLKTime  = 0;

    mIspresetnInit          = true;
    mIsResetHardActive      = false;
    mIsResetCmdReceive      = false;
    mIsResetCmdActive       = false;
    mResetCmdPeriod         = 0;

    mTimeResolutionValue    = 1;
    mTimeResolutionUnit     = SC_NS;
    GetTimeResolution(mTimeResolutionValue, mTimeResolutionUnit);

    mDumpInterrupt  = false;
    mEnableRegisterMessage = true;

    mKeyOpen        = false;

    Initialize();

    // SC_THREAD/METHOD
    SC_METHOD(MonitorpclkMethod);
    dont_initialize();
    sensitive << pclk;

    SC_METHOD(presetnMethod);
    sensitive << presetn;

    SC_METHOD(HandleResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdEvent;

    SC_METHOD(CancelResetCmdMethod);
    dont_initialize();
    sensitive << mResetCmdCancelEvent;

    SC_METHOD(MonitorkeyopenMethod);
    dont_initialize();
    sensitive << keyopen;

    SC_METHOD(WriteDisablePortMethod);
    dont_initialize();
    sensitive << mWriteDisablePortEvent;

    SC_METHOD(WriteADEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteADEDERROUTEvent;

    SC_METHOD(NegateADEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateADEDERROUTEvent;

    SC_METHOD(WriteASEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteASEDERROUTEvent;

    SC_METHOD(NegateASEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateASEDERROUTEvent;

    SC_METHOD(WriteDEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteDEDERROUTEvent;

    SC_METHOD(NegateDEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateDEDERROUTEvent;

    SC_METHOD(WriteSEDERROUTMethod);
    dont_initialize();
    sensitive << mWriteSEDERROUTEvent;

    SC_METHOD(NegateSEDERROUTMethod);
    dont_initialize();
    sensitive << mNegateSEDERROUTEvent;

    SC_METHOD(ProcessAddressErrorMethod);
    dont_initialize();
    sensitive << mProcessAddressErrorEvent;

    SC_METHOD(ProcessDataErrorMethod);
    dont_initialize();
    sensitive << mProcessDataErrorEvent;

}//}}} 

/// Destructor
ECC_BUS_SEP_AD::~ECC_BUS_SEP_AD()
{//{{{
    switch (mNum_Module) {
        case 1:
            delete mpBusSlaveBase1;
            break;
        case 2:
            delete mpBusSlaveBase2;
            break;
        default:
            delete mpBusSlaveBase3;
            break;
    }
    if (mENA_AECCCTL == true) {
        delete aeccdisable;
        delete mpAECCCTL_Func;
    }

    if (mENA_DECCCTL == true) {
        delete secdisable;
        delete eccdisable;
        delete mpDECCCTL_Func;
    }

    if (mENA_ECCCAP  == true) {
        delete adederrout;
        delete asederrout;
        delete dederrout;
        delete sederrout;
        delete mpECCCAP_Func;
    }
}//}}}

/// Initialize when reset activated
void ECC_BUS_SEP_AD::Initialize(void)
{//{{{
    mAECCDIS   = 0;
    mDSECDIS   = 0;
    mDECCDIS  = 0;
    mADEDERROUT   = 0;
    mASEDERROUT   = 0;
    mDEDERROUT    = 0;
    mSEDERROUT    = 0;
    
    mIsEDLConfig = 0;
    mErrorEDL = 0;
    mDataError.Initialize();
    mAddressError.Initialize();
}//}}}

//========================================================
//============ Overwrite func. of agent ==================
//========================================================
/// Prepare to write prohibition port of CFP
void ECC_BUS_SEP_AD::WriteDisPortAECCTL(unsigned int value)
{//{{{
    mAECCDIS = (bool)(value & 0x1);
    mWriteDisablePortEvent.cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisablePortEvent.notify(CalculateCLKPosEdge(false, current_time), mTimeResolutionUnit);
}//}}}

/// Prepare to write prohibition port of CFS
void ECC_BUS_SEP_AD::WriteDisPortDECCTL(unsigned int value)
{//{{{
    mDECCDIS = (bool)(value & 0x1);
    mDSECDIS = (bool)((value >> 1) & 0x1);
    mWriteDisablePortEvent.cancel();
    double current_time = sc_time_stamp().to_double();
    mWriteDisablePortEvent.notify(CalculateCLKPosEdge(false, current_time), mTimeResolutionUnit);
}//}}}

/// Assert interrupt
void ECC_BUS_SEP_AD::AssertInterrupt(std::string int_name)
{//{{{ 
    assert ( (int_name == "AS") || (int_name == "AD") || (int_name == "DS") || (int_name == "DD") );
    if (int_name == "DS"){
        mSEDERROUT = true;
        mWriteSEDERROUTEvent.notify();
    }else if (int_name == "DD"){
        mDEDERROUT = true;
        mWriteDEDERROUTEvent.notify();
    }else if (int_name == "AS"){
        mASEDERROUT = true;
        mWriteASEDERROUTEvent.notify();
    }else {//if (int_name == "AD"){
        mADEDERROUT = true;
        mWriteADEDERROUTEvent.notify();
    }
}//}}}

/// Get keycode value
bool ECC_BUS_SEP_AD::GetKeyOpen()
{//{{{
    return mKeyOpen;
}//}}}

//========================================================
//============ Reset and clock  ==========================
//========================================================
/// Monitor pclk clock
void ECC_BUS_SEP_AD::MonitorpclkMethod()
{//{{{
    sc_dt::uint64 freq_value = pclk.read();
    SetCLKFreq("pclk", freq_value, "Hz");
}//}}}

/// Monitor presetn port
void ECC_BUS_SEP_AD::presetnMethod()
{//{{{
    bool reset = presetn.read();
    if (reset == vpcl::emResetActive){
        mIspresetnInit = false;
        mIsResetHardActive = true;
        re_printf("info", "The reset port presetn is asserted.\n");
        EnableReset(true);
    }else{
        if (mIspresetnInit){
            mIspresetnInit = false;
        }else{
            mIsResetHardActive = false;
            re_printf("info", "The reset port presetn is de-asserted.\n");
            EnableReset(false);
        }
    }
    //Cancel AssertReset command when reset port is active
    if (mIsResetCmdReceive && mIsResetHardActive) {
        mResetCmdEvent.cancel();
        mResetCmdCancelEvent.cancel();
        mIsResetCmdReceive = false;
        mIsResetCmdActive = false;
    }
}//}}}

/// Process reset function when reset command is active
void ECC_BUS_SEP_AD::HandleResetCmdMethod ()
{//{{{
    mIsResetCmdActive = true;
    re_printf("info","The model is reset by AssertReset command of presetn.\n");
    this->EnableReset(mIsResetCmdActive);
    mResetCmdCancelEvent.notify(mResetCmdPeriod, SC_NS);
}//}}}

/// Cancel reset function when reset command is active
void ECC_BUS_SEP_AD::CancelResetCmdMethod ()
{//{{{
    mIsResetCmdActive = false;
    mIsResetCmdReceive = false;
    re_printf("info","Reset period of presetn is over.\n");
    this->EnableReset(mIsResetCmdActive);
}//}}}

/// Reset model and its sub-instance if reset is actived
void ECC_BUS_SEP_AD::EnableReset (const bool is_active)
{//{{{
    if(is_active) {
        // Cancel events
        CancelEvents();
        // Initialize variable
        Initialize();
        // Initialize output ports
        mWriteADEDERROUTEvent.notify();
        mWriteASEDERROUTEvent.notify();
        mWriteDEDERROUTEvent.notify();
        mWriteSEDERROUTEvent.notify();
        mWriteDisablePortEvent.notify();
    }
    if (mENA_AECCCTL == true) {
        mpAECCCTL_Func->EnableReset(is_active);
    }
    if (mENA_DECCCTL == true) {
        mpDECCCTL_Func->EnableReset(is_active);
    }
    if (mENA_ECCCAP  == true) {
        mpECCCAP_Func->EnableReset(is_active);
    }
}//}}}

/// Check frequency value and frequency unit
void ECC_BUS_SEP_AD::ConvertClockFreq (sc_dt::uint64 &freq_out, std::string &unit_out, sc_dt::uint64 freq_in, std::string unit_in)
{//{{{
    if(unit_in == "GHz"){
        freq_out = freq_in * (sc_dt::uint64)(1.0e+9);
        unit_out = "GHz";
    } else if (unit_in == "MHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+6);
        unit_out = "MHz";
    } else if (unit_in == "KHz") {
        freq_out = freq_in * (sc_dt::uint64)(1.0e+3);
        unit_out = "KHz";
    } else if (unit_in == "Hz") {
        freq_out = freq_in;
        unit_out = "Hz";
    } else {// Frequency unit is wrong, frequency unit is set as Hz default.
        freq_out = freq_in;
        unit_out = "Hz";
        re_printf("warning","Frequency unit (%s) is wrong, frequency unit is set as unit Hz default.\n", unit_in.c_str());
    }
}//}}}

/// Get time resolution
void ECC_BUS_SEP_AD::GetTimeResolution (sc_dt::uint64 &resolution_value, sc_time_unit &resolution_unit)
{//{{{
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {
        resolution_value = 1000000000000000LL;
        resolution_unit = SC_FS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {
        resolution_value = 1000000000000LL;
        resolution_unit = SC_PS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)) {
        resolution_value = 1000000000;
        resolution_unit = SC_NS;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {
        resolution_value = 1000000;
        resolution_unit = SC_US;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {
        resolution_value = 1000;
        resolution_unit = SC_MS;
    } else {
        resolution_value = 1;
        resolution_unit = SC_SEC;
    }
}//}}}

/// Calculate the synchronous time with clock
double ECC_BUS_SEP_AD::CalculateCLKPosEdge (const bool add_period, const double time_point)
{//{{{
    //calcualte the time point of next pos edge of PCLK
    if (mPCLKPeriod == 0){
        return 0;
    }
    double period_num = (time_point - mStartPCLKTime)/ (double)mPCLKPeriod;
    sc_dt::uint64 mod_period_num = (sc_dt::uint64)period_num;
    if (!add_period) {
        if ((double)mod_period_num < period_num) {
            mod_period_num++;
        }
    } else {
        mod_period_num++;
    }
    return (mStartPCLKTime + (double)mod_period_num * (double)mPCLKPeriod - time_point);
}//}}}

/// Cancel events
void ECC_BUS_SEP_AD::CancelEvents()
{//{{{
    mWriteADEDERROUTEvent.cancel();
    mNegateADEDERROUTEvent.cancel();
    mWriteASEDERROUTEvent.cancel();
    mNegateASEDERROUTEvent.cancel();
    mWriteDEDERROUTEvent.cancel();
    mNegateDEDERROUTEvent.cancel();
    mWriteSEDERROUTEvent.cancel();
    mNegateSEDERROUTEvent.cancel();
    mWriteDisablePortEvent.cancel();
    mProcessDataErrorEvent.cancel();
    mProcessAddressErrorEvent.cancel();
}//}}}

/// Get reset status
bool ECC_BUS_SEP_AD::GetResetStatus ()
{//{{{
    return (mIsResetCmdActive || mIsResetHardActive);
}//}}}

/// Check clock period 
bool ECC_BUS_SEP_AD::CheckClockPeriod ()
{//{{{
    bool clock_status = false;
    if (mPCLKPeriod > 0) {
        clock_status = true;
    } else {
        clock_status = false;
        re_printf("info","pclk frequency is zero\n");
    }
    return clock_status;
}//}}} 

//========================================================
//============ Operation               ===================
//========================================================
/// Monitor keyopen port
void ECC_BUS_SEP_AD::MonitorkeyopenMethod()
{//{{{
    mKeyOpen = keyopen.read();
}//}}}

/// Write disable port in AECCTL
void ECC_BUS_SEP_AD::WriteDisablePortMethod()
{//{{{
    if (mENA_AECCCTL == true) {
        aeccdisable->write(mAECCDIS);
    }
    if (mENA_DECCCTL == true) {
        secdisable->write(mDSECDIS);
        eccdisable->write(mDECCDIS);
    }
}//}}}

/// Write adederrout port
void ECC_BUS_SEP_AD::WriteADEDERROUTMethod()
{//{{{
    if (mENA_ECCCAP  == true) {
        adederrout->write(mADEDERROUT);
        if (mDumpInterrupt){
            re_printf("info", "adederrout is changed to %d\n", (unsigned int)mADEDERROUT);
        }
        // Negate if any
        if (mADEDERROUT){
            mNegateADEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// Negate adederrout port
void ECC_BUS_SEP_AD::NegateADEDERROUTMethod()
{//{{{
    mADEDERROUT = false;
    mWriteADEDERROUTEvent.notify();
}//}}}

/// Write asederrout port
void ECC_BUS_SEP_AD::WriteASEDERROUTMethod()
{//{{{
    if (mENA_ECCCAP  == true) {
        asederrout->write(mASEDERROUT);
        if (mDumpInterrupt){
            re_printf("info", "asederrout is changed to %d\n", (unsigned int)mASEDERROUT);
        }
        // Negate if any
        if (mASEDERROUT){
            mNegateASEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// Negate asederrout port
void ECC_BUS_SEP_AD::NegateASEDERROUTMethod()
{//{{{
    mASEDERROUT = false;
    mWriteASEDERROUTEvent.notify();
}//}}}

/// Write dederrout port
void ECC_BUS_SEP_AD::WriteDEDERROUTMethod()
{//{{{
    if (mENA_ECCCAP  == true) {
        dederrout->write(mDEDERROUT);
        if (mDumpInterrupt){
            re_printf("info", "dederrout is changed to %d\n", (unsigned int)mDEDERROUT);
        }
        // Negate if any
        if (mDEDERROUT){
            mNegateDEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// Negate dederrout port
void ECC_BUS_SEP_AD::NegateDEDERROUTMethod()
{//{{{
    mDEDERROUT = false;
    mWriteDEDERROUTEvent.notify();
}//}}}

/// Write sederrout port
void ECC_BUS_SEP_AD::WriteSEDERROUTMethod()
{//{{{
    if (mENA_ECCCAP  == true) {
        sederrout->write(mSEDERROUT);
        if (mDumpInterrupt){
            re_printf("info", "sederrout is changed to %d\n", (unsigned int)mSEDERROUT);
        }
        // Negate if any
        if (mSEDERROUT){
            mNegateSEDERROUTEvent.notify((double)mPCLKPeriod, mTimeResolutionUnit);
        }
    }
}//}}}

/// Negate sederrout port
void ECC_BUS_SEP_AD::NegateSEDERROUTMethod()
{//{{{
    mSEDERROUT = false;
    mWriteSEDERROUTEvent.notify();
}//}}}

/// Process 1bit/2bit error in address
void ECC_BUS_SEP_AD::ProcessAddressErrorMethod()
{//{{{
    if (mENA_ECCCAP  == true) {
        mpECCCAP_Func->ProcessAddressError(mAddressError);
    }
}//}}}

/// Process 1bit/2bit error in data
void ECC_BUS_SEP_AD::ProcessDataErrorMethod()
{//{{{
    if (mENA_ECCCAP  == true) {
        mpECCCAP_Func->ProcessDataError(mDataError);
    }
}//}}}

//========================================================
//============ PYTHON IF               ===================
//========================================================
/// Set message level (fatal, error, warning, info)
void ECC_BUS_SEP_AD::SetMessageLevel (const std::string msg_lv)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("command");
    cmd.push_back("MessageLevel");
    SeparateString(cmd, msg_lv);

    std::string msg_return = handleCommand(cmd);
    if (msg_return != "") {
        SC_REPORT_INFO(this->basename(), msg_return.c_str());
    }


    if (mEnableRegisterMessage == false) {
        // Disable message level for Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        if (mENA_AECCCTL == true) {
            mpAECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_DECCCTL == true) {
            mpDECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_ECCCAP  == true) {
            mpECCCAP_Func->RegisterHandler(cmd);
        }

        // Disable message level for each register in Register IF class (except fatal level)
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        cmd.push_back("fatal");
        if (mENA_AECCCTL == true) {
            mpAECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_DECCCTL == true) {
            mpDECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_ECCCAP  == true) {
            mpECCCAP_Func->RegisterHandler(cmd);
        }
    } else {
        //Set message level for Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        if (mENA_AECCCTL == true) {
            mpAECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_DECCCTL == true) {
            mpDECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_ECCCAP  == true) {
            mpECCCAP_Func->RegisterHandler(cmd);
        }

        //Set message level for each register in Register IF class
        cmd.clear();
        cmd.push_back("reg");
        cmd.push_back("*");
        cmd.push_back("MessageLevel");
        SeparateString(cmd, msg_lv);
        if (mENA_AECCCTL == true) {
            mpAECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_DECCCTL == true) {
            mpDECCCTL_Func->RegisterHandler(cmd);
        }
        if (mENA_ECCCAP  == true) {
            mpECCCAP_Func->RegisterHandler(cmd);
        }
    }

}//}}}

/// Enable/disable dumping message when users access registers
void ECC_BUS_SEP_AD::DumpRegisterRW (const std::string is_enable)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    cmd.push_back("DumpRegisterRW");
    SeparateString(cmd, is_enable);
    if (mENA_AECCCTL == true) {
        mpAECCCTL_Func->RegisterHandler(cmd);
    }
    if (mENA_DECCCTL == true) {
        mpDECCCTL_Func->RegisterHandler(cmd);
    }
    if (mENA_ECCCAP  == true) {
        mpECCCAP_Func->RegisterHandler(cmd);
    }
}//}}}

/// Enable/disable dumping interrupt information
void ECC_BUS_SEP_AD::DumpInterrupt (const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mDumpInterrupt = true;
        } else if (word_vector[0] == "false") {
            mDumpInterrupt = false;
        } else {
            re_printf("warning","Invalid argument: ECC_BUS_SEP_AD_DumpInterrupt %s\n", is_enable.c_str());
        }
    } else if (word_vector.size() == 0){
        std::string temp = (mDumpInterrupt) ? "true" : "false";
        re_printf("info","ECC_BUS_SEP_AD_DumpInterrupt %s\n",temp.c_str());
    } else {
        re_printf("warning","Invalid argument: ECC_BUS_SEP_AD_DumpInterrupt %s\n", is_enable.c_str());
    }
}//}}}

void ECC_BUS_SEP_AD::EnableRegisterMessage(const std::string is_enable)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, is_enable);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "true") {
            mEnableRegisterMessage = true;
        }
        else if (word_vector[0] == "false") {
            mEnableRegisterMessage = false;
        }
        else {
            re_printf("warning", "Invalid argument: ECC_BUS_SEP_AD_EnableRegisterMessage %s\n", is_enable.c_str());
        }
    }
    else if (word_vector.size() == 0) {
        std::string status = "true";
        if (mEnableRegisterMessage == false)
        {
            status = "false";
        }
        re_printf("info", "ECC_BUS_SEP_AD_EnableRegisterMessage %s\n", status.c_str());
    }
    else {
        re_printf("warning", "Invalid argument: ECC_BUS_SEP_AD_EnableRegisterMessage %s\n", is_enable.c_str());
    }
}//}}}

/// Issue error to ECC
void ECC_BUS_SEP_AD::IssueError(const unsigned int errortype)
{//{{{
    if (GetResetStatus() || (!CheckClockPeriod())){
        re_printf("info", "Not receive error input during reset period or clock is stoped.\n");
        return;
    }

    // Not receive ECC error if disable
    if (mENA_DECCCTL == true) {
        unsigned int data_eccdis = mpDECCCTL_Func->GetDECCDIS();
        if(mErr_Kind != "AA") {
            if ( (data_eccdis == 1) && ((errortype == em1bitError) || (errortype == em2bitError)) ){
                return;
            }
        }
    }
    if (mENA_AECCCTL == true) {
        unsigned int address_eccdis = mpAECCCTL_Func->GetAECCDIS();
        if(mErr_Kind != "DD") {
            if ( (address_eccdis == 1) && ((errortype == em1bitAddressError) || (errortype == em2bitAddressError)) ){
                return;
            }
        }
    }

    // Process error
    double current_time = sc_time_stamp().to_double();
    if (mIsEDLConfig){
        // Process error in data
        if ( ((errortype == em1bitError) || (errortype == em2bitError)) && (mErr_Kind != "AA") ){
            re_printf("info", "Error input Type: %d, EDL (bit index): 0x%X.\n", errortype, mErrorEDL);
            mDataError.edl_code = mErrorEDL;
            mDataError.type = (eErrorType)errortype;
            mProcessDataErrorEvent.cancel();
            mProcessDataErrorEvent.notify(CalculateCLKPosEdge(true, current_time), mTimeResolutionUnit);

            // Process error in address
        } else if ( ((errortype == em1bitAddressError) || (errortype == em2bitAddressError)) && (mErr_Kind != "DD") ){
            re_printf("info", "Error input Type: %d, EDL (bit index): 0x%X.\n", errortype, mErrorEDL);
            mAddressError.edl_code = mErrorEDL;
            mAddressError.type = (eErrorType)errortype;
            mProcessAddressErrorEvent.cancel();
            mProcessAddressErrorEvent.notify(CalculateCLKPosEdge(true, current_time), mTimeResolutionUnit);

            // Process case invalid error
        } else {
            re_printf("warning", "Invalid error type (%d) of ECC_BUS_SEP_AD_IssueError command.\n", errortype);
        }
    }else {
        re_printf("warning", "The error location (SetEDL) is required to configure before issuing error.\n");
    }
}//}}}

/// Config error detection location (EDL)
void ECC_BUS_SEP_AD::SetEDL (const unsigned int edl)
{//{{{
    if (edl < emSizeofStatusReg){
        mErrorEDL = edl;
        mIsEDLConfig = true;
    }else{
        re_printf("warning", "Invalid EDL (%d) of ECC_BUS_SEP_AD_SetEDL command.\n", edl);
    }
}//}}}

/// Dump Status info
void ECC_BUS_SEP_AD::DumpStatusInfo()
{//{{{
    if (mENA_ECCCAP  == true) { 
        mpECCCAP_Func->DumpStatusInfo();
    }
}//}}}

/// Command to assert reset
void ECC_BUS_SEP_AD::AssertReset (const std::string reset_name, const double start_time, const double period)
{//{{{
    if (reset_name == "presetn") {
        if ((!mIsResetHardActive)&&(!mIsResetCmdReceive)) {
            mIsResetCmdReceive = true;
            re_printf("info","The model will be reset (presetn) for %f ns after %f ns.\n", period, start_time);
            mResetCmdEvent.notify(start_time, SC_NS);
            mResetCmdPeriod = period;
        } else {
            re_printf("warning","The software reset of presetn is called in the reset operation of the model. So it is ignored.\n");
        }
    } else {
        re_printf("warning","The reset name (%s) is wrong. It should be presetn.\n",reset_name.c_str());
    }
}//}}}

/// Set clock value and clock unit
void ECC_BUS_SEP_AD::SetCLKFreq (const std::string clock_name, const sc_dt::uint64 freq, const std::string unit)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            mPCLKOrgFreq = freq;
            ConvertClockFreq (mPCLKFreq, mPCLKFreqUnit, freq, unit);
            if (mPCLKFreq > 0) {
                mPCLKPeriod = (sc_dt::uint64)(((1/(double)mPCLKFreq)*(double)mTimeResolutionValue) + 0.5);
                mStartPCLKTime = sc_time_stamp().to_double();
            } else {
                mPCLKPeriod = 0;
                // Cancel events
                CancelEvents();
                // Notify clock is zero
                mClkZeroEvent.notify(SC_ZERO_TIME);
            }
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Get clock value
void ECC_BUS_SEP_AD::GetCLKFreq (const std::string clock_name)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, clock_name);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "pclk") {
            re_printf("info","pclk frequency is %0.0f %s\n", (double)mPCLKOrgFreq, mPCLKFreqUnit.c_str());
        } else {
            re_printf("warning","Clock name (%s) is invalid\n", word_vector[0].c_str());
        }
    } else {
        re_printf("warning","Clock name (%s) is invalid\n", clock_name.c_str());
    }
}//}}}

/// Force value to register
void ECC_BUS_SEP_AD::ForceRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{ 
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("force");
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if (reg_name.find("AECC") != std::string::npos && mErr_Kind != "DD"){
                if (mENA_AECCCTL == true) {
                    mpAECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (reg_name.find("DECC") != std::string::npos && mErr_Kind != "AA"){
                if (mENA_DECCCTL == true) {
                    mpDECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (!(reg_name.find("AECC") != std::string::npos) && !(reg_name.find("DECC") != std::string::npos)) {
                if (mENA_ECCCAP  == true) {
                    mpECCCAP_Func->RegisterHandler(cmd);
                }
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Release forced value after forcing registers
void ECC_BUS_SEP_AD::ReleaseRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            cmd.push_back("release");
            // Call RegisterHandler of accroding block name
            if (reg_name.find("AECC") != std::string::npos && mErr_Kind != "DD"){
                if (mENA_AECCCTL == true) {
                    mpAECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (reg_name.find("DECC") != std::string::npos && mErr_Kind != "AA"){
                if (mENA_DECCCTL == true) {
                    mpDECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (!(reg_name.find("AECC") != std::string::npos) && !(reg_name.find("DECC") != std::string::npos)) {
                if (mENA_ECCCAP  == true) {
                    mpECCCAP_Func->RegisterHandler(cmd);
                }
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Write value to registers by software
void ECC_BUS_SEP_AD::WriteRegister (const std::string reg_name, const unsigned int reg_value)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            std::ostringstream reg_value_str;
            reg_value_str<<"0x"<<std::hex<<reg_value;
            cmd.push_back(reg_value_str.str());
            // Call RegisterHandler of accroding block name
            if (reg_name.find("AECC") != std::string::npos && mErr_Kind != "DD"){
                if (mENA_AECCCTL == true) {
                    mpAECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (reg_name.find("DECC") != std::string::npos && mErr_Kind != "AA"){
                if (mENA_DECCCTL == true) {
                    mpDECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (!(reg_name.find("AECC") != std::string::npos) && !(reg_name.find("DECC") != std::string::npos)) {
                if (mENA_ECCCAP  == true) {
                    mpECCCAP_Func->RegisterHandler(cmd);
                }
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// Read value of register by software
void ECC_BUS_SEP_AD::ReadRegister (const std::string reg_name)
{//{{{
    std::vector<std::string> word_vector;
    std::vector<std::string> cmd;
    if (reg_name != "") {
        SeparateString(word_vector, reg_name);
        if (word_vector.size() == 1) {
            cmd.push_back("reg");
            SeparateString(cmd, reg_name);
            // Call RegisterHandler of accroding block name
            if (reg_name.find("AECC") != std::string::npos && mErr_Kind != "DD"){
                if (mENA_AECCCTL == true) {
                    mpAECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (reg_name.find("DECC") != std::string::npos && mErr_Kind != "AA"){
                if (mENA_DECCCTL == true) {
                    mpDECCCTL_Func->RegisterHandler(cmd);
                }
            }else if (!(reg_name.find("AECC") != std::string::npos) && !(reg_name.find("DECC") != std::string::npos)) {
                if (mENA_ECCCAP  == true) {
                    mpECCCAP_Func->RegisterHandler(cmd);
                }
            }
        } else {
            re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
        }
    } else {
        re_printf("warning","Register name (%s) is invalid.\n", reg_name.c_str());
    }
}//}}}

/// List all registers name
void ECC_BUS_SEP_AD::ListRegister (void)
{//{{{
    std::vector<std::string> cmd;
    cmd.push_back("reg");
    // Call RegisterHandler of according to block name
    if(mErr_Kind != "DD") {
        if (mENA_AECCCTL == true) {
            mpAECCCTL_Func->RegisterHandler(cmd);
        }
    }
    if( mErr_Kind != "AA") {
        if (mENA_DECCCTL == true) {
            mpDECCCTL_Func->RegisterHandler(cmd);
        }
    }
    if (mENA_DECCCTL == true) {
        mpECCCAP_Func->RegisterHandler(cmd);
    }
}//}}}

/// Separate some words from a string to store a vector
void ECC_BUS_SEP_AD::SeparateString (std::vector<std::string> &vtr, const std::string msg)
{//{{{
    std::istringstream cmd;
    cmd.str(msg);
    std::string temp = "a"; //Make temp to be not empty
    while (temp != "") {
        std::string command;
        cmd >> command;
        if (command != "") {
            vtr.push_back(command);
        }
        temp = command;
    }
}//}}}

/// Dump help message of all parameters or commands
void ECC_BUS_SEP_AD::Help (const std::string type)
{//{{{
    std::vector<std::string> word_vector;
    SeparateString(word_vector, type);
    if (word_vector.size() == 1) {
        if (word_vector[0] == "parameters") {
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_MessageLevel (\"ECC_BUS_SEP_AD instance\", \"fatal|error|warning|info\")        Set debug message level (Default: fatal|error)");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_DumpRegisterRW (\"ECC_BUS_SEP_AD instance\", \"true/false\")                    Enable/disable dumping access register (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_DumpInterrupt (\"ECC_BUS_SEP_AD instance\", \"true/false\")                     Enable/disable dumping interrupt info (Default: false)");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_EnableRegisterMessage (true/false)                                              Enable/disable dumping info/warning/error message of register IF (Default: true).");
        } else if (word_vector[0] == "commands") {
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_IssueError (\"ECC_BUS_SEP_AD instance\", error_type)                            Issue pseudo error to the ECC_BUS_SEP_AD model");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_SetEDL (\"ECC_BUS_SEP_AD instance\", edl)                                       Set error detection location");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_DumpStatusInfo (\"ECC_BUS_SEP_AD instance\")                                    Dump information of the error status registers of the ECC_BUS_SEP_AD model");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_AssertReset (\"ECC_BUS_SEP_AD instance\", \"reset_name\", start_time, period)   Assert and de-assert reset signal to the ECC_BUS_SEP_AD model");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_SetCLKFreq (\"ECC_BUS_SEP_AD instance\", \"clock_name\", freq, \"unit\")        Set clock frequency value to ECC_BUS_SEP_AD model. Valid clock_name is pclk");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_GetCLKFreq (\"ECC_BUS_SEP_AD instance\", \"clock_name\")                        Get clock frequency value of ECC_BUS_SEP_AD model. Valid clock_name is pclk");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_ForceRegister (\"ECC_BUS_SEP_AD instance\", \"reg_name\", reg_value)            Force register with setting value");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_ReleaseRegister (\"ECC_BUS_SEP_AD instance\", \"reg_name\")                     Release register from force value");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_WriteRegister (\"ECC_BUS_SEP_AD instance\", \"reg_name\", reg_value)            Write a value to a register");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_ReadRegister (\"ECC_BUS_SEP_AD instance\", \"reg_name\")                        Read a value from a register");
            SC_REPORT_INFO(this->basename(),"ECC_BUS_SEP_AD_ListRegister (\"ECC_BUS_SEP_AD instance\")                                      Dump name of all registers");
        } else {
            re_printf("warning","The name (%s) of ECC_BUS_SEP_AD_Help argument is wrong (commands or parameters)\n", type.c_str());
        }
    } else {
        re_printf("warning","The name (%s) of ECC_BUS_SEP_AD_Help argument is wrong (commands or parameters)\n", type.c_str());
    }
}//}}}

// vim600: set foldmethod=marker :
