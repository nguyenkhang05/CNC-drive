// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2015 Renesas Electronics Corporation
// Copyright(c) 2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "re_define.h"
#include "sarad113x.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//Constructor of SARAD113x class
Csarad113x::Csarad113x(sc_module_name name):
         Csarad113x_regif((std::string)name, 32)
        ,vpcl::tlm_tgt_if<32>(name)
        ,pclk ("pclk")
        ,ADCLK ("ADCLK")
        ,preset_n ("preset_n")
        ,AVREFH0 ("AVREFH0")
        //trigger input port
        ,TSN_TRG ("TSN_TRG")
        ,SG1_TRG ("SG1_TRG")
        ,SG2_TRG ("SG2_TRG")
        ,SG3_TRG ("SG3_TRG")
        ,PVCR_TRG ("PVCR_TRG")
        //Analog terminal input port
        ,ANI00 ("ANI00")
        ,ANI01 ("ANI01")
        ,ANI02 ("ANI02")
        ,ANI03 ("ANI03")
        ,ANI04 ("ANI04")
        ,ANI05 ("ANI05")
        ,ANI06 ("ANI06")
        ,ANI07 ("ANI07")
        ,ANI08 ("ANI08")
        ,ANI09 ("ANI09")
        ,ANI10 ("ANI10")
        ,ANI11 ("ANI11")
        ,ANI12 ("ANI12")
        ,ANI13 ("ANI13")
        ,ANI14 ("ANI14")
        ,ANI15 ("ANI15")
        ,ANI16 ("ANI16")
        ,ANI17 ("ANI17")
        ,ANI18 ("ANI18")
        ,ANI19 ("ANI19")
        ,ANI20 ("ANI20")
        ,ANI21 ("ANI21")
        ,ANI22 ("ANI22")
        ,ANI23 ("ANI23")
        ,ANI24 ("ANI24")
        ,ANI25 ("ANI25")
        ,ANI26 ("ANI26")
        ,ANI27 ("ANI27")
        ,ANI28 ("ANI28")
        ,ANI29 ("ANI29")
        ,ANI30 ("ANI30")
        ,ANI31 ("ANI31")
        ,ANI32 ("ANI32")
        ,ANI33 ("ANI33")
        ,ANI34 ("ANI34")
        ,ANI35 ("ANI35")
        ,TSN_ANI ("TSN_ANI")

        ,PVCR_VALUE ("PVCR_VALUE")
        ,ADOP_OPA1_DATA("ADOP_OPA1_DATA")
        ,ADOP_OPA2_DATA("ADOP_OPA2_DATA")
        ,ADOP_OPA3_DATA("ADOP_OPA3_DATA")

        //interrupt ouptut port
        ,INT_TSN ("INT_TSN")
        ,INT_SG1 ("INT_SG1")
        ,INT_SG2 ("INT_SG2")
        ,INT_SG3 ("INT_SG3")
        ,INT_ADE ("INT_ADE")

        ,ADCATCNV0 ("ADCATCNV0")
        ,ADCATCNV1 ("ADCATCNV1")
        ,ADCATCNV2 ("ADCATCNV2")
        ,ADCATCNV3 ("ADCATCNV3")
        ,ADCATCNV4 ("ADCATCNV4")
        ,ULE ("ULE")
        ,PVCR_MUXCUR ("PVCR_MUXCUR")
        ,PVCR_END ("PVCR_END")
        ,TSN_TS_EN ("TSN_TS_EN")
        ,TSN_TSSW ("TSN_TSSW")
        ,TSN_TSSW_DISCH ("TSN_TSSW_DISCH")
        ,TSN_TSMASK ("TSN_TSMASK")
        ,TSN_SELF_DIAG ("TSN_SELF_DIAG")
        ,TSN_TRIM ("TSN_TRIM")
        ,ADOP_OPA1_PSEL("ADOP_OPA1_PSEL")
        ,ADOP_OPA1_WEN("ADOP_OPA1_WEN")
        ,ADOP_OPA2_PSEL("ADOP_OPA2_PSEL")
        ,ADOP_OPA2_WEN("ADOP_OPA2_WEN")
        ,ADOP_OPA3_PSEL("ADOP_OPA3_PSEL")
        ,ADOP_OPA3_WEN("ADOP_OPA3_WEN")
{//{{{
    //Initalze handleCommand  
    CommandInit(this->name());
    Csarad113x_regif::set_instance_name(this->name());
    SARAD113xFSMInit();

    //Initialze variable
    Initialize();
    mAccessTimeTSNCR = 0;
    mIsInitialize = true;
    mIsRefVolUpdate  = false;
    mResetPeriod = 0;
    mSARPortResetFlag = false;
    mSARCmdResetFlag = false;
    mDGOUTAD = 0;
    for (unsigned int i=0;i<emDGOUTNum;i++) {
        mDGOUTSH[i] = 0;
    }
    
    // Initialize Parameter and Information
    mPreAvrefh = 3.3;
    mPreAVcc = 3.3;
    mPreAVss = 0;
    mPreEX_HLD_CDT = 18;
    mPreEX_CNVT = 22;
    mPretD = 0;
    mPretPWDD = 0;
    mPretED = 0;
    mPreEnableTimeCalculation = true;
    mPCLKPeriod = 0;
    mADCLKPeriod = 0;

    //Initialize output port
    INT_TSN.initialize(false);
    INT_SG1.initialize(false);
    INT_SG2.initialize(false);
    INT_SG3.initialize(false);
    INT_ADE.initialize(false);
    ADCATCNV0.initialize(false);
    ADCATCNV1.initialize(false);
    ADCATCNV2.initialize(false);
    ADCATCNV3.initialize(false);
    ADCATCNV4.initialize(false);
    ULE.initialize(false);
    PVCR_MUXCUR.initialize(0);
    PVCR_END.initialize(false);
    TSN_TS_EN.initialize(false);
    TSN_TSSW.initialize(false);
    TSN_TSSW_DISCH.initialize(false);
    TSN_TSMASK.initialize(false);
    TSN_SELF_DIAG.initialize(false);
    TSN_TRIM.initialize(0);
    ADOP_OPA1_PSEL.initialize(false);
    ADOP_OPA1_WEN.initialize(false);
    ADOP_OPA2_PSEL.initialize(false);
    ADOP_OPA2_WEN.initialize(false);
    ADOP_OPA3_PSEL.initialize(false);
    ADOP_OPA3_WEN.initialize(false);

    //Reset Method
    SC_METHOD(ResetMethod);
    dont_initialize();
    sensitive << preset_n;
    SC_METHOD(AssertResetMethod);
    dont_initialize();
    sensitive << mCmdResetEvent;
    SC_METHOD(DeAssertResetMethod);
    dont_initialize();
    sensitive << mCmdCancelResetEvent;

    //Clock Method
    SC_METHOD(PCLKMethod);
    dont_initialize();
    sensitive << pclk;
    SC_METHOD(ADCLKMethod);
    dont_initialize();
    sensitive << ADCLK;

    //VoltageMethod
    SC_METHOD(AVREFHMethod);
    dont_initialize();
    sensitive << AVREFH0;
    SC_METHOD(InitialAVREFHMethod);

    // TriggerMethod
    #ifdef SARAD113x_ENABLE_TSN
    SC_METHOD(TSNTRGMethod);
    dont_initialize();
    sensitive << TSN_TRG;
    #endif
    SC_METHOD(SG1TRGMethod);
    dont_initialize();
    sensitive << SG1_TRG;
    SC_METHOD(SG2TRGMethod);
    dont_initialize();
    sensitive << SG2_TRG;
    SC_METHOD(SG3TRGMethod);
    dont_initialize();
    sensitive << SG3_TRG;
    SC_METHOD(PVCRTRGMethod);
    dont_initialize();
    sensitive << PVCR_TRG;
    SC_METHOD(PVCR_VALUEMethod);
    dont_initialize();
    sensitive << PVCR_VALUE;
    for (unsigned int i = 0; i < emAllSG; ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mHWTriggerEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::HWTriggerProcessMethod, this, i), sc_core::sc_gen_unique_name("HWTriggerProcessMethod"), &opt);
    }
    for (unsigned int i = 0; i < emAllSG; ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mSWTriggerEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::SWTriggerProcessMethod, this, i), sc_core::sc_gen_unique_name("SWTriggerProcessMethod"), &opt);
    }
    SC_METHOD(CheckTriggerMethod);
    dont_initialize();
    sensitive << mCheckTriggerMethodEvent;
    
    // Register clear Method
    for (unsigned int i = 0; i < (emMaxVirChannel/2); ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mClearDREvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::ClearDRMethod, this, i), sc_core::sc_gen_unique_name("ClearDRMethod"), &opt);
    }
    for (unsigned int i = 0; i < emMaxVirChannel; ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mClearDIREvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::ClearDIRMethod, this, i), sc_core::sc_gen_unique_name("ClearDIRMethod"), &opt);
    }
    #ifdef SARAD113x_ENABLE_TSN
    SC_METHOD(ClearTSNDRMethod);
    dont_initialize();
    sensitive << mClearTSNDREvent;
    SC_METHOD(ClearTSNDIRMethod);
    dont_initialize();
    sensitive << mClearTSNDIREvent;
    #endif
    SC_METHOD(ClearPWDDRMethod);
    dont_initialize();
    sensitive << mClearPWDDREvent;
    SC_METHOD(ClearPWDDIRMethod);
    dont_initialize();
    sensitive << mClearPWDDIREvent;

    // ControlMethod
    SC_METHOD(WriteADOPControlMethod);
    dont_initialize();
    sensitive << mWriteADOPControlEvent;
    #ifdef SARAD113x_ENABLE_TSN
    SC_METHOD(WriteTSN_TS_ENMethod);
    dont_initialize();
    sensitive << mWriteTSN_TS_ENEvent;
    SC_METHOD(WriteTSN_SELF_DIAGMethod);
    dont_initialize();
    sensitive << mWriteTSN_SELF_DIAGEvent;
    SC_METHOD(WriteTSNControlMethod);
    dont_initialize();
    sensitive << mWriteTSNControlEvent;
    #endif
    SC_METHOD(WritePVCR_MUXCURMethod);
    dont_initialize();
    sensitive << mWritePVCR_MUXCUREvent;
    for (unsigned int i = 0; i < emAllSG; ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteADCATCNVControlEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::WriteADCATCNVControlMethod, this, i), sc_core::sc_gen_unique_name("WriteADCATCNVControlMethod"), &opt);
    }
    for (unsigned int i = 0; i < emAllSG; ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mWriteSGEndInterruptEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::WriteSGEndInterruptMethod, this, i), sc_core::sc_gen_unique_name("WriteSGEndInterruptMethod"), &opt);
    }
    SC_METHOD(WriteULEInterruptMethod);
    dont_initialize();
    sensitive << mWriteULEInterruptEvent;
    SC_METHOD(WriteADEInterruptMethod);
    dont_initialize();
    sensitive << mWriteADEInterruptEvent;

    //State Method
    for (unsigned int i = 0; i < emAllSG; ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mUpdateSGACTEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::UpdateSGACTMethod, this, i), sc_core::sc_gen_unique_name("UpdateSGACTMethod"), &opt);
    }
    for (unsigned int i = 0; i < emAllSG; ++i) {
        sc_core::sc_spawn_options opt;
        opt.spawn_method();
        opt.set_sensitivity(&mUpdateConversionDataEvent[i]);
        opt.dont_initialize();
        sc_core::sc_spawn(sc_bind(&Csarad113x::UpdateConversionDataMethod, this, i), sc_core::sc_gen_unique_name("UpdateConversionDataMethod"), &opt);
    }
    SC_METHOD(UpdateSHACTMethod);
    dont_initialize();
    sensitive << mUpdateSHACTEvent;
    SC_METHOD(StartTHSamplingMethod);
    dont_initialize();
    sensitive << mStartTHSamplingEvent;
    SC_METHOD(VCSamplingMethod);
    dont_initialize();
    sensitive << mStartVCSamplingEvent;
    SC_METHOD(VCConversionMethod);
    dont_initialize();
    sensitive << mStartVCConversionEvent;
    SC_METHOD(VCEndConversionMethod);
    dont_initialize();
    sensitive << mEndVCConversionEvent;
    pCsarad113x_fsm->EnableDumpStateTrans(true);
    SetLatency_TLM (0, true);
}//}}}

//destructor of Csarad113x class
Csarad113x::~Csarad113x()
{//{{{
    delete pCsarad113x_fsm;
}//}}}

void Csarad113x::Initialize()
{//{{{
    mPrioritySet = emPriority0;
    for (unsigned int i=emSG1;i<=emSG3;i++){
        mADOPControlVal[i-1] = false;
    }
    mULEVal = false;
    mADEVal = 0;
    for (unsigned int i=0;i<emAllSG;i++){
        mIntrVal[i] = false;
        mINTActiveNum[i] = 0;
    }
    mINTADEActiveNum = 0;
    mULEActiveNum = 0;
    InitOperation();
}//}}}

void Csarad113x::InitOperation()
{//{{{
    mIsOperating = false;
    mCurrentSG = emAllSG;
    mPreviousVC = 0;
    mCurrentTrigger = emAllSG;
    for (unsigned int i=0;i<emAllSG;i++){
        mIsHWTrigger[i] = false;
        mIsSWTrigger[i] = false;
        mCurrentStartVC[i] = 0;
        mNextVC[i] = 0;
        mFirstVC[i] = 0;
        mLastVC[i] = 0;
        mSGACTVal[i] = 0;
        mIsScanning[i] = false;
        mIsEnableStart[i] = false;
        mIsSuspend[i] = false;
        mADCATCNVnVal[i] = false;
        mScanFreqCount[i] = 1;
        mIsFirstTimeConv[i] = true;
    }
    mPVCR_MUXCURVal = 0;
    mSHACTVal = 0;
    for (unsigned int i=0;i<emAllTHCh;i++){
        mStartTHSamplingTime[i] = 0;
        mHoldPortVal[i] = 0;
    }
    mCurrentAnalogVal = 0;
    mRepetitionTime = 0;
    mRepetitionCount= 0;
    mIsLastRepetition = false;
    mADData = 0;
    mULError = 0;
    mStartTimeVC = 0;
    mTSNStateControl = emDeassertAll;
    #ifdef SARAD113x_ENABLE_TSN
    mWriteTSNControlEvent.notify();
    #endif
    // Cancel all operation event
    for (unsigned int i=0;i<emAllSG;i++) {
        mHWTriggerEvent[i].cancel();
        mSWTriggerEvent[i].cancel();
        mWriteADCATCNVControlEvent[i].cancel();
        mUpdateConversionDataEvent[i].cancel();
    }
    mCheckTriggerMethodEvent.cancel();
    mStartVCSamplingEvent.cancel();
    mStartVCConversionEvent.cancel();
    mEndVCConversionEvent.cancel();
    mStartTHSamplingEvent.cancel();
}//}}}

void Csarad113x::tgt_acc (tlm::tlm_generic_payload &trans, sc_time &t)
{ //{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr    = 0;
    unsigned char *p_data  = NULL;
    unsigned int size = 0; 
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, false);
    if(!status){
        trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
        return;
    }
    sc_assert(p_data != NULL);

    // Callback method
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        status = reg_rd((unsigned int) addr, p_data, size);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        mPWDATAVal = 0;
        if (size > 4) {
            memcpy(&mPWDATAVal,p_data,4);
        } else {
            memcpy(&mPWDATAVal,p_data,size);
        }
        mWrittenPWDATAEvent.notify();
        status = reg_wr((unsigned int) addr, p_data, size);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
} //}}}

unsigned int Csarad113x::tgt_acc_dbg(tlm::tlm_generic_payload &trans)
{//{{{
    //Get information
    tlm::tlm_command command;
    sc_dt::uint64 addr = 0;
    unsigned char *p_data = NULL;
    unsigned int size = 0;
    bool status = this->tgt_get_gp_attribute(command, addr, p_data, size, trans, true);
    sc_assert(p_data != NULL);

    // Callback method
    if (command == tlm::TLM_READ_COMMAND) {
        memset(p_data, 0, size);
        status = reg_rd_dbg((unsigned int) addr, p_data, size);
    } else if (command == tlm::TLM_WRITE_COMMAND) {
        mPWDATAVal = 0;
        if (size > 4) {
            memcpy(&mPWDATAVal,p_data,4);
        } else {
            memcpy(&mPWDATAVal,p_data,size);
        }
        mWrittenPWDATAEvent.notify();
        status = reg_wr_dbg((unsigned int) addr, p_data, size);
    }
    // Be necessarily TLM_IGNORE_COMMAND
    else {
        status = true;
        size = 0;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    if (status){
        return size;
    } else {
        return 0;
    }
}//}}}

void Csarad113x::ClearScanningEndFlag(const unsigned int sg)
{//{{{
    (*SGSTR)["SEF"] = (unsigned int)(*SGSTR)["SEF"] & ((~(1<<sg))&0x1F);
}//}}}

// Control the ADOP output ports
void Csarad113x::WriteADOPControl(const unsigned int sg)
{//{{{
    mADOPControlVal[sg-1] = true; // sg(1~3) <==> val(0~2)
    mWriteADOPControlEvent.notify(NextPCLKPosedge(0),SC_NS);
}//}}}

// Calculate wait time to next rising edge of PCLK clock (first rising edge at the time simulation started)
double Csarad113x::NextPCLKPosedge(double offset)
{//{{{
    sc_dt::uint64 curr_time = (sc_dt::uint64)(GetCurTime() + offset);
    if (mPCLKPeriod > 0) {
        if ((curr_time%(sc_dt::uint64)mPCLKPeriod) == 0) {
            return offset;
        } else {
            return (offset + (double)((sc_dt::uint64)mPCLKPeriod - (curr_time%(sc_dt::uint64)mPCLKPeriod)));
        }
    } else {
        return 0;    
    }
}//}}}

// Calculate wait time to next rising edge of ADCLK clock (first rising edge at the time simulation started)
double Csarad113x::NextADCLKPosedge(double offset)
{//{{{
    sc_dt::uint64 curr_time = (sc_dt::uint64)(GetCurTime() + offset);
    if (mADCLKPeriod > 0) {
        if ((curr_time%(sc_dt::uint64)mADCLKPeriod) == 0) {
            return offset;
        } else {
            return (offset + (double)((sc_dt::uint64)mADCLKPeriod - (curr_time%(sc_dt::uint64)mADCLKPeriod)));
        }
    } else {
        return 0;
    }
}//}}}

// Return port value;
double Csarad113x::GetANIPortVal(unsigned int port_index)
{//{{{
    double port_val[emMaxPhyChannel + 2]; // +1 channel for self-diagnosis + 1 channel for TSN
    port_val[0]  = ANI00.read();
    port_val[1]  = ANI01.read();
    port_val[2]  = ANI02.read();
    port_val[3]  = ANI03.read();
    port_val[4]  = ANI04.read();
    port_val[5]  = ANI05.read();
    port_val[6]  = ANI06.read();
    port_val[7]  = ANI07.read();
    port_val[8]  = ANI08.read();
    port_val[9]  = ANI09.read();
    port_val[10] = ANI10.read();
    port_val[11] = ANI11.read();
    port_val[12] = ANI12.read();
    port_val[13] = ANI13.read();
    port_val[14] = ANI14.read();
    port_val[15] = ANI15.read();
    port_val[16] = ANI16.read();
    port_val[17] = ANI17.read();
    port_val[18] = ANI18.read();
    port_val[19] = ANI19.read();
    port_val[20] = ANI20.read();
    port_val[21] = ANI21.read();
    port_val[22] = ANI22.read();
    port_val[23] = ANI23.read();
    port_val[24] = ANI24.read();
    port_val[25] = ANI25.read();
    port_val[26] = ANI26.read();
    port_val[27] = ANI27.read();
    port_val[28] = ANI28.read();
    port_val[29] = ANI29.read();
    port_val[30] = ANI30.read();
    port_val[31] = ANI31.read();
    port_val[32] = ANI32.read();
    port_val[33] = ANI33.read();
    port_val[34] = ANI34.read();
    port_val[35] = ANI35.read();
    port_val[36] = 0;
    port_val[37] = TSN_ANI.read();
    for (unsigned int i=0;i<emANIGroup1;i++) {
        if ((unsigned int)(*ADCR)["DGON"] == 1) { // Self-diagnosis is enabled
            if ((((unsigned int)(*DGCTL1) >> i)&0x1) == 1) {
                port_val[i] = mDGOUTSH[i%3];
            } else {
                if ((((unsigned int)(*PDCTL1) >> i)&0x1) == 1) { // Pull down is enabled
                    port_val[i] = 0;
                }
            }
        } else {
            if ((((unsigned int)(*PDCTL1) >> i)&0x1) == 1) { // Pull down is enabled
                port_val[i] = 0;
            }
        }
    }
    for (unsigned int i=0;i<emANIGroup2;i++) {
        if ((((unsigned int)(*PDCTL2) >> i)&0x1) == 1) { // Pull down is enabled
            port_val[emANIGroup1+i] = 0;
        }
    }
    if ((unsigned int)(*ADCR)["DGON"] == 1) { // return self-diagnosis value
        port_val[36] = mDGOUTAD;
    }
    sc_assert(port_index < (emMaxPhyChannel+2));
    return port_val[port_index];
}//}}}

// Update self-diagnosis DGOUTAD and DGOUTSH when ADCR.DGON is set to 1
void Csarad113x::UpdateSelfDiag(void)
{//{{{
    // Update mDGOUTAD and mDGOUTSH[i]
    unsigned int psel = (unsigned)(*DGCTL0) & 0x7;
    switch (psel) {
        case 0x0: 
           mDGOUTAD    = 0;
           mDGOUTSH[2] = 0;
           mDGOUTSH[1] = 0;
           mDGOUTSH[0] = 0;
           break;
        case 0x1: 
           mDGOUTAD    = AVss;
           mDGOUTSH[2] = (2*AVcc)/3;
           mDGOUTSH[1] = (1*AVcc)/2;
           mDGOUTSH[0] = (1*AVcc)/3;
           break;
        case 0x2: 
           mDGOUTAD    = (1*AVcc)/3;
           mDGOUTSH[2] = (1*AVcc)/3;
           mDGOUTSH[1] = (2*AVcc)/3;
           mDGOUTSH[0] = (1*AVcc)/2;
           break;
        case 0x3: 
           mDGOUTAD    = (1*AVcc)/2;
           mDGOUTSH[2] = (1*AVcc)/2;
           mDGOUTSH[1] = (1*AVcc)/3;
           mDGOUTSH[0] = (2*AVcc)/3;
           break;
        case 0x4: 
           mDGOUTAD    = (2*AVcc)/3;
           mDGOUTSH[2] = 0;
           mDGOUTSH[1] = 0;
           mDGOUTSH[0] = 0;
           break;
        case 0x5: 
           mDGOUTAD    = AVcc;
           mDGOUTSH[2] = (1*AVcc)/3;
           mDGOUTSH[1] = (1*AVcc)/3;
           mDGOUTSH[0] = (1*AVcc)/3;
           break;
        case 0x6: 
           mDGOUTAD    = AVcc;
           mDGOUTSH[2] = (1*AVcc)/2;
           mDGOUTSH[1] = (1*AVcc)/2;
           mDGOUTSH[0] = (1*AVcc)/2;
           break;
        case 0x7: 
           mDGOUTAD    = AVcc;
           mDGOUTSH[2] = (2*AVcc)/3;
           mDGOUTSH[1] = (2*AVcc)/3;
           mDGOUTSH[0] = (2*AVcc)/3;
           break;
        default: break;
    }
}//}}}

void Csarad113x::SetCurrentSG(unsigned int group,bool is_th)
{//{{{
    if (is_th) { // T&H scanning group
        if (group == emTHGroupA) {
            mCurrentSG = (unsigned int)(*THACR)["SGS"];
        } else {
            mCurrentSG = (unsigned int)(*THBCR)["SGS"];
        }
    } else { // Normal scanning group
        mCurrentSG = mCurrentTrigger;
    }
}//}}}

// Check the preparation to start a scanning group
void Csarad113x::StartScanning (unsigned int sg, bool is_th)
{//{{{
    bool en_start = true;
    #ifdef SARAD113x_ENABLE_TSN
    if (sg == emTSNSG) { // Check start time of TSN scanning group
        double current_time = GetCurTime();
        if (((unsigned int)(*TSNCR)["TSNEN"] == 1) && ((current_time - mAccessTimeTSNCR + 3*mADCLKPeriod) < emMinStartTSNConv)) {  // Start time after 200us from writing 1 to TSNCR
            en_start = false;
        }
    }
    #endif
    if (en_start && CheckSGSetting(sg,is_th)) {
        re_printf("info","Scanning group %d starts for A/D conversion.\n",mCurrentSG);
        // Set the repetition time
        mRepetitionTime = GetRepetitionTime(sg);
        mRepetitionCount = 0;
        // Set the variable to indicate scanning group is in scanning state
        mIsScanning[sg] = true;
        mIsEnableStart[sg] = true;
    } else { // Ignore SG scanning due to error of configuration or start condition
        re_printf("warning","Scanning group %d can not start due to error or conflict setting.\n",mCurrentSG); 
        // Clear corresponding trigger
        mIsHWTrigger[sg] = false;
        mIsSWTrigger[sg] = false;
        mIsScanning[sg] = false;
        mIsEnableStart[sg] = false;
        // Deassert corresponding ADCATCNVn (n=0,4) output port trigger is ignored
        mADCATCNVnVal[sg] = false;
        mWriteADCATCNVControlEvent[sg].notify();
    }
}//}}}

// Clear corresponding trigger for finished scanning group
void Csarad113x::FinishScanning(unsigned int sg)
{//{{{
    re_printf("warning","Scanning group %d is stopped.\n",mCurrentSG); 
    // Clear corresponding trigger
    mIsHWTrigger[sg] = false;
    mIsSWTrigger[sg] = false;
    mIsFirstTimeConv[sg] = true;
    // Set the variable to indicate scanning group is stopped
    mIsScanning[sg] = false;
}//}}}

// Check the setting of current Scanning group
bool Csarad113x::CheckSGSetting(unsigned int sg, bool is_th)
{//{{{
    unsigned int sg_num = mCurrentSG;
    unsigned int start_vc = 0;
    unsigned int end_vc   = 0;
    // Check condition of SGVCSPn and SGVCEPn (n=1~3)
    if ((sg == emSG1) || (sg == emSG2) || (sg == emSG3)) {
        start_vc = mFirstVC[sg_num];
        end_vc   = mLastVC[sg_num];
        if (start_vc > end_vc) {
            re_printf("warning","The value of register %s should be equal or less than value of register %s.\n",SGVCSP[sg_num]->name().c_str(),SGVCEP[sg_num]->name().c_str());
            return false;
        }
        if ((unsigned int)(*ADCR)["DGON"] == 0) {
            // Check self-diagnosis channel is defined when self-diagnosis function is disabled
            for (unsigned int i=start_vc;i<=end_vc;i++) {
                if (GetGCTRL(sg,i) == emSelfNum36) {
                    re_printf("warning","Self-diagnosis channel should not be used in Virtual channel %d when Self-diagnosis function is disabled (ADCR.DGON = 0).\n",i);
                    return false;
                }
            }
        }
    }
    if (is_th) {
        unsigned int sgA = (unsigned int)(*THACR)["SGS"];
        unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
        bool phy_inuse_flag[emMaxPhyChannel];
        for (unsigned int i=0;i<emMaxPhyChannel;i++) {
            phy_inuse_flag[i] = false;
        }
        if (sgA != 0) {
            // Do not specify same physical channel in a differrent group
            start_vc = mFirstVC[sgA];
            end_vc   = mLastVC[sgA];
            for (unsigned int vc_num = start_vc; vc_num <= end_vc; vc_num++) {
                unsigned int phy_index = (unsigned int)(*(VCR[vc_num]))["GCTRL"];
                phy_inuse_flag[phy_index] = true;
                if (sg_num == sgA) {
                    if ((unsigned int)(*(VCR[vc_num]))["MPXE"] == 1) {
                        re_printf("warning","MPX function should not be enabled for T&H operation.\n");
                        return false;
                    }
                }
                if ((phy_index >= emTHCh0) && (phy_index < emAllTHCh) && ((((unsigned int)(*THER) >> phy_index) & 0x1) == 0x1)) {
                    if ((((unsigned int)(*THGSR) >> phy_index) & 0x1) == emTHGroupB) {
                        re_printf("warning","T&H channel %d of T&H group B is used for T&H group A.\n", phy_index);
                    }
                }
            }
        }
        if (sgB != 0) {
            start_vc = mFirstVC[sgB];
            end_vc   = mLastVC[sgB];
            for (unsigned int vc_num = start_vc; vc_num <= end_vc; vc_num++) {
                unsigned int phy_index = (unsigned int)(*(VCR[vc_num]))["GCTRL"];
                if (phy_inuse_flag[phy_index]) {
                    re_printf("warning","Physical channel %d should not be used in both T&H group A and B.\n", phy_index);
                    return false;
                }
                if (sg_num == sgB) {
                    if ((unsigned int)(*(VCR[vc_num]))["MPXE"] == 1) {
                        re_printf("warning","MPX function should not be enabled for T&H operation.\n");
                        return false;
                    }
                }
                if ((phy_index >= emTHCh0) && (phy_index < emAllTHCh) && ((((unsigned int)(*THER) >> phy_index) & 0x1) == 0x1)) {
                    if ((((unsigned int)(*THGSR) >> phy_index) & 0x1) == emTHGroupA) {
                        re_printf("warning","T&H channel %d of T&H group A is used for T&H group B.\n", phy_index, sgB);
                    }
                }
            }
        }
        if ((sgA != 0) && (sgB != 0)) {
            // T&H channel 0-2ch and 3-5ch should be set to same group
            unsigned int group_sel[emAllTHGroup];
            group_sel[emTHGroupA] = false;
            group_sel[emTHGroupB] = false;
            for (unsigned int i=0;i<emAllTHCh;i++) {
                if ((((unsigned int)(*THER) >> i) & 0x1) == 1) {
                    if (i < emTHCh3) {
                        if ((((unsigned int)(*THGSR) >> i) & 0x1) == emTHGroupA) {
                            group_sel[emTHGroupA] = true;
                        } else {
                            group_sel[emTHGroupB] = true;
                        }
                    } else {
                        if ((((((unsigned int)(*THGSR) >> i) & 0x1) == emTHGroupA) && group_sel[emTHGroupA])    // T&H group A already selects channel 0-2 so that group A can not select channel 3-5
                         || (((((unsigned int)(*THGSR) >> i) & 0x1) == emTHGroupB) && group_sel[emTHGroupB])) { // T&H group B already selects channel 0-2 so that group B can not select channel 3-5
                            re_printf("warning","The T&H channels 0-2 should not set to same group with channels 3-5.\n");
                            return false;
                        }
                    }
                }
            }
        }
        // Continuous scanning mode is not supported in T&H operation 
        if ((unsigned int)(*(SGCR[sg_num]))["SCANMD"] == emContinuousMode) {
            re_printf("warning","Continuous scanning mode is not supported in T&H operation.\n");
            return false;
        } else {
            // Setting the frequency to 1 in multi cycle scanning mode of T&H operation (SGMCYCRn.MCYC = 0) (n=1~3)
            if ((unsigned int)(*(SGMCYCR[sg_num]))["MCYC"] > 0) {
                re_printf("warning","The scanning frequency should be set to 1 in multi cycle scanning mode of T&H operation (%s.MCYC = 0).\n",SGMCYCR[sg_num]->name().c_str());
                return false;
            }
        }
    }
    return true;
}//}}}

void Csarad113x::SuspendScanning(unsigned int group, bool is_th)
{//{{{
    unsigned int sgA = (unsigned int)(*THACR)["SGS"];
    unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
    unsigned int sg = group;
    bool is_all_th_suspend = (pCsarad113x_fsm->mCurrentState == Csarad113x_fsm::emStTH_SUSPEND);
    bool en_resume = false;
    if (is_th && !is_all_th_suspend && (group == emTHGroupA)) {
        sg = sgA;
    }else if (is_th && !is_all_th_suspend && (group == emTHGroupB)) {
        sg = sgB;
    }
    if (is_th 
    && (((sg == sgA) && ((sgB != 0) && !mIsScanning[sgB])) 
     || ((sg == sgB) && ((sgA != 0) && !mIsScanning[sgA])))) {
        en_resume = true;
    }
    if (!is_all_th_suspend) {
        re_printf("info","Scanning group %d is suspended.\n",sg);
    }
    #ifdef SARAD113x_ENABLE_TSN
    // Cancel and deassert all TSN control ouput port if TSN scanning group is suspended
    if (sg == emTSNSG) {
        mWriteTSNControlEvent.cancel();
        mTSNStateControl = emDeassertAll;
        mWriteTSNControlEvent.notify();
    }
    #endif
    // Cancel all event of current scanning group
    if (!is_th 
    ||  (is_th 
      && (((sg == sgA) && ((sgB != 0) && !mIsScanning[sgB])) 
       || ((sg == sgB) && ((sgA != 0) && !mIsScanning[sgA]))))) {
        mStartVCSamplingEvent.cancel();
        mStartVCConversionEvent.cancel();
        mEndVCConversionEvent.cancel();
    }
    if (is_th && en_resume) {
        ResumeTH();
    }
    if (sg == emAllSG) {
        return;
    }
    // Set the variable to indicate scanning group is stopped
    mIsScanning[sg] = false;
    mIsSuspend[sg] = true;
}//}}}

void Csarad113x::StartVCConv(unsigned int sg)
{//{{{
    // Configuration for VC of scanning group
    if ((sg == emSG1) || (sg == emSG2) || (sg == emSG3)) {
        // Get Start virtual channel
        mCurrentStartVC[sg] = mNextVC[sg];
    } else {
        mCurrentStartVC[sg] = 0;
    }
    PrintVCmessage("Start A/D conversion process", sg, mCurrentStartVC[sg]);
    mStartTimeVC = GetCurTime();
    // Update the self-diagnosis voltage from the second time of A/D conversion if self-diagnosis is enabled
    if (!mIsFirstTimeConv[sg] && ((unsigned int)(*ADCR)["DGON"] == 1)) {
        UpdateSelfDiag();
    }
    mIsFirstTimeConv[sg] = false;
    #ifdef SARAD113x_ENABLE_TSN
    // Assert TSN control after 3 ADCLK when TSN scanning group is start for A/D conversion
    if (sg == emTSNSG) {
        mTSNStateControl = emAssertTSSW;
        mWriteTSNControlEvent.notify(3*mADCLKPeriod,SC_NS);
    }
    #endif
    // Calculate timing for VC conversion
    double sample_time = GetSampleTime(sg);
    double conv_time = GetConversionTime();
    double wait_time = 0;
    // Write MPXV value to PVCR_MUXCUR in case MPXE is set to 1 (from the second time of repetition, this wait is omitted)
    if ((GetMPXE(sg,mCurrentStartVC[sg]) == 1) && (mRepetitionCount == 0)) {
        mPVCR_MUXCURVal = GetMPXV(sg,mCurrentStartVC[sg]);
        (*MPXCURR)["MPXCUR"] = mPVCR_MUXCURVal;
        mWritePVCR_MUXCUREvent.notify();
        wait_time = sample_time + conv_time;
    }
    mStartVCSamplingEvent.notify(wait_time + 3*mADCLKPeriod,SC_NS); // Start sampling after wait time + 3 ADCLK period
}//}}}

void Csarad113x::FinishVCConv(unsigned int sg)
{//{{{
    PrintVCmessage("Finish A/D conversion process", sg, mCurrentStartVC[sg]);
    // Convert input value
    mADData = ADConvert(mCurrentAnalogVal,sg,mCurrentStartVC[sg]);
    // Update A/D conversion data, interrupt for current Scanning group
    mPreviousVC = mCurrentStartVC[sg];
    double time_end = 3*mADCLKPeriod + mADCLKPeriod; // wait 3 ADCLK and 1 ADCLK for validating data 
    if (EnableTimeCalculation) {
        time_end = time_end + 3*mPCLKPeriod; // wait 3 pclk to write data to registers
    } else {
        time_end = time_end + tED;
    }
    mUpdateConversionDataEvent[sg].notify(NextPCLKPosedge(time_end),SC_NS); 

    #ifdef SARAD113x_ENABLE_TSN
    // Deassert TSN_TS_EN after 4 PCLK period when finish TSN A/D conversion
    if (sg == emTSNSG) {
        (*TSNCR)["TSNEN"] = 0;
        mWriteTSN_TS_ENEvent.notify(NextPCLKPosedge(4*mPCLKPeriod),SC_NS);
    }
    #endif
}//}}}

void Csarad113x::UpdateInternalCount(unsigned int sg)
{//{{{
    if (mRepetitionCount >= mRepetitionTime) { // Last time of repetition time
        mRepetitionCount = 0;
        if ((sg >= emSG1) && (sg <= emSG3)) {
            if (mPreviousVC == mLastVC[sg]) { // Last VC
                if (mScanFreqCount[sg] > ((unsigned int)(*(SGMCYCR[sg]))["MCYC"] + 1)) {
                    mScanFreqCount[sg] = 1;
                } else {
                    mScanFreqCount[sg] = mScanFreqCount[sg] + 1;
                }
            }
        }
    }
}//}}}

bool Csarad113x::CheckSuspend(unsigned int trg_num, unsigned int current_sg)
{//{{{
    if ((current_sg == emAllSG) // First scanning group
    // Suspend in Asynchronous suspend mode (ADCR.SUSMTD = 2) or Suspend when SG0 or SG1 is interrupted by other higher priority scanning group (ADCR.SUSMTD = 1)
     || ((ComparePriority(trg_num, current_sg)) && (((unsigned int)(*ADCR)["SUSMTD"] == emAsyncMode) || (((unsigned int)(*ADCR)["SUSMTD"] == emHybridMode) && (current_sg <= emSG1))))) { // 
        return true;
    }
    return false;
}//}}}

unsigned int Csarad113x::IsLastVC(unsigned int sg)
{//{{{
    if ((sg == emTSNSG) || (sg == emPWDSG)) { // TSN and PWD only conversion for 1 time
        // Deassert corresponding ADCATCNVn (n=0,4) output port when end of A/D conversion of scanning group
        mADCATCNVnVal[sg] = false;
        mWriteADCATCNVControlEvent[sg].notify(NextADCLKPosedge(0),SC_NS);
        return emLastVC;
    }
    if (mIsLastRepetition) { // Last time of repetition time
        unsigned int sgA = (unsigned int)(*THACR)["SGS"];
        unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
        if (mPreviousVC == mLastVC[sg]) { // Last VC
            mNextVC[mCurrentSG] = (unsigned int)(*(SGVCSP[sg]))["VCSP"]; // Set for next conversion
            mFirstVC[mCurrentSG] = (unsigned int)(*(SGVCSP[sg]))["VCSP"]; // Set for next conversion
            mLastVC[mCurrentSG] = (unsigned int)(*(SGVCEP[sg]))["VCEP"]; // Set for next conversion
            if (!IsContinuousMode(sg)) { // Multi cycle scanning mode
                if (mScanFreqCount[sg] > (unsigned int)(*(SGMCYCR[sg]))["MCYC"]) {
                    // Deassert corresponding ADCATCNVn (n=1~3) output port when end of A/D conversion of scanning group
                    mADCATCNVnVal[sg] = false;
                    mWriteADCATCNVControlEvent[sg].notify(NextADCLKPosedge(0),SC_NS);
                    // Clear SHACT when both group A and B finish conversion and no auto sampling
                    if (!IsAutoStartSampling() 
                     && ((sgA == 0) || ((sgA != 0) && !mADCATCNVnVal[sgA])) 
                     && ((sgB == 0) || ((sgB != 0) && !mADCATCNVnVal[sgB]))) {
                        mSHACTVal = 0;
                        mUpdateSHACTEvent.notify(NextPCLKPosedge(2*mPCLKPeriod), SC_NS);
                    }
                    return emLastVC;
                }
            }
        } else {
            // Set next virtual channel
            mNextVC[sg] = mCurrentStartVC[sg] + 1;
        }
        // Check suspend mode before start next VC
        for (unsigned int i=0;i<emAllSG;i++){
            if ((mIsHWTrigger[i] || mIsSWTrigger[i]) && ComparePriority(i,sg)) {
                // T&H group is suspend, trigger method to scanning next group
                if ((sg == sgA) || (sg == sgB)) {
                    // Set the variable to indicate scanning group is stopped (for T&H group)
                    mIsScanning[sg] = false;
                }
                return emSuspend;
            }
        }
    }
    return emContinue;
}//}}}

void Csarad113x::CheckTrigger(void)
{//{{{
    unsigned int sg = emAllSG;
    bool en_trigger = false;
    unsigned int sgA = (unsigned int)(*THACR)["SGS"];
    unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
    unsigned int sgA_cur_state = pCsarad113x_fsm->pCsarad113x_tha_conv_fsm->mCurrentState;
    unsigned int sgB_cur_state = pCsarad113x_fsm->pCsarad113x_thb_conv_fsm->mCurrentState;
    for (unsigned int i=0;i<emAllSG;i++){
        if (mIsHWTrigger[i] || mIsSWTrigger[i]) {
            en_trigger = true;
            if ((i == emTSNSG) 
             || (i == emPWDSG) 
             || (((i == emSG1) || (i == emSG2) || (i == emSG3)) && (!CheckTH(i)))) {
                // Assert corresponding ADCATCNVn (n=0~4) output port when trigger is asserted for non T&H scanning group
                mADCATCNVnVal[i] = true;
                mWriteADCATCNVControlEvent[i].notify(NextADCLKPosedge(0),SC_NS);
            }
            // Assert HW trigger for T&H group A or B
            if ((sgA != 0) && (i == sgA)) {
                if (mIsHWTrigger[i]) { // HW trigger
                    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAHWTrigger].notify(NextADCLKPosedge(0),SC_NS);
                } else { // SW trigger
                    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHASWTrigger].notify(NextADCLKPosedge(0),SC_NS);
                }
            }
            if ((sgB != 0) && (i == sgB)) {
                if (mIsHWTrigger[i]) { // HW trigger
                    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBHWTrigger].notify(NextADCLKPosedge(0),SC_NS);
                } else { // SW trigger
                    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBSWTrigger].notify(NextADCLKPosedge(0),SC_NS);
                }
            }
            if (sg == emAllSG) {
                sg = i;
            } else if (ComparePriority(i,sg)) {
                sg = i;
            }
        }
    }
    if (en_trigger) {
        if (!((pCsarad113x_fsm->mCurrentState == Csarad113x_fsm::emStTH_CONV) // is TH conversion
          && (((sgB != 0) && (sg == sgB) && ((sgB_cur_state == Csarad113x_thb_conv_fsm::emStTHB_SAMPLING) || (sgB_cur_state == Csarad113x_thb_conv_fsm::emStTHB_HOLDING)))
           || ((sgA != 0) && (sg == sgA) && ((sgA_cur_state == Csarad113x_tha_conv_fsm::emStTHA_SAMPLING) || (sgA_cur_state == Csarad113x_tha_conv_fsm::emStTHA_HOLDING)))))) {
            mCurrentTrigger = sg;
            if (!((((sgB != 0) && (sg == sgB) && !mIsScanning[sgA]) || (sgA != 0) && (sg == sgA) && !mIsScanning[sgB]) 
               && (pCsarad113x_fsm->mCurrentState == Csarad113x_fsm::emStTH_CONV))) {
                StartTrigger(sg, NextADCLKPosedge(0));
            }
            if ((sgA != 0) && (sg == sgA)) {
                mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAResume].notify(NextADCLKPosedge(0),SC_NS);
            }  
            if ((sgB != 0) && (sg == sgB)) {
                mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBResume].notify(NextADCLKPosedge(0),SC_NS);
            }
        }
    }
}//}}}

// Compare the priority of scanning group. Return true if check scanning group has higher priority than current scanning group
bool Csarad113x::ComparePriority(unsigned int check_sg, unsigned int current_sg)
{//{{{
    if (mPrioritySet == emPriority0) {
        if (check_sg > current_sg) {
            return true;
        }
    } else {
        if ((current_sg != check_sg)
         && ((current_sg == 1)
          || ((current_sg == 0) && (check_sg > 1))
          || ((current_sg == 4) && ((check_sg == 2) || (check_sg == 3)))
          || ((current_sg == 2) && (check_sg == 3)))) {
            return true;
        }
    }
    return false;
}//}}}

void Csarad113x::SetStartSmpTime(unsigned int channel)
{//{{{
    double current_time = GetCurTime();
    if (channel == emAllTHCh) {
        for (unsigned int i=0;i<emAllTHCh;i++) {
            if ((((unsigned int)(*THER) >> i)&0x1) == 1) {
                mStartTHSamplingTime[i] = current_time;
            }
        }
    } else {
        if ((((unsigned int)(*THER) >> channel)&0x1) == 1) {
            mStartTHSamplingTime[channel] = current_time;
        }
    }
}//}}}

bool Csarad113x::CheckSmpTime(unsigned int group, bool hw_trg)
{//{{{
    // Check sampling time with parameter
    std::string group_name = "group A";
    unsigned int sg = (unsigned int)(*THACR)["SGS"];
    if (group == emTHGroupB) {
        group_name = "group B";
        sg = (unsigned int)(*THBCR)["SGS"];
    }
    if ((unsigned int)(*THER) == 0) {
        re_printf("warning","Holding process can not start because all of T&H channel is disabled.\n");
        return false;
    }
    for (unsigned int i=0;i<emAllTHCh;i++) {
        if ((((group == emTHGroupA) && ((((unsigned int)(*THGSR) >> i)&0x1) == 0)) || ((group == emTHGroupB) && ((((unsigned int)(*THGSR) >> i)&0x1) == 1)))
         && (((unsigned int)(*THER) >> i)&0x1) == 1) {
            double current_time = GetCurTime();
            if ((current_time - mStartTHSamplingTime[i]) < emMinTHSmpTime) { // Omit current trigger if sample time is less than minimum time (450ns)
                re_printf("warning","Sampling time for T&H channel of %s must be equal or greater than %d ns.\n",group_name.c_str(), emMinTHSmpTime);
                mIsHWTrigger[sg] = false;
                mIsSWTrigger[sg] = false;
                return false;
            }
        }
    }
    return true;
}//}}}

// Check whether there is reset or not
bool Csarad113x::IsReset(void)
{//{{{
    return (mSARPortResetFlag || mSARCmdResetFlag);
}//}}}

// Check whether T&H function is used or not
bool Csarad113x::CheckTH(unsigned int sg)
{//{{{
    if ((sg != 0)
     && (((unsigned int)(*THACR)["SGS"] == sg)||((unsigned int)(*THBCR)["SGS"] == sg))) {
        return true;
    } else {
        return false;
    }
}//}}}

// Check group is enabled for T&H or not
bool Csarad113x::CheckEnableTH(unsigned int group)
{//{{{
    if (group == emTHGroupA) {
        if ((unsigned int)(*THACR)["SGS"] != 0) {
            return true;    
        }
    } else {
        if ((unsigned int)(*THBCR)["SGS"] != 0) {
            return true;    
        }
    }
    return false;
}//}}}

// Check whether other TH group is conversion or not. Return true if there is no TH group conversion
bool Csarad113x::CheckTHStart(unsigned int group)
{//{{{
    unsigned int sg = 0;
    unsigned int other_sg = 0;
    bool en_start = true;
    unsigned int sgA = (unsigned int)(*THACR)["SGS"];
    unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
    if (group == emTHGroupA) {
        sg = sgA;
        other_sg = sgB;
    } else {
        sg = sgB;
        other_sg = sgA;
    }
    for (unsigned int i = 0; i<emAllSG; i++) {
        if (mIsScanning[i] && en_start) {
            if (ComparePriority(i,sg)) {
                en_start = false;
            } else {
                en_start = CheckSuspend(sg,i); 
            }
        }
    }
    if (en_start) { // set flag in advance to avoid racing
        mIsScanning[sg] = true;
        if ((other_sg == sgA) && (other_sg != 0)) { // Suspend other T&H group 
            mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHASuspend].notify();
        } else if ((other_sg == sgB) && (other_sg != 0)) { // Suspend other T&H group 
            mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBSuspend].notify();
        }
    }
    return en_start;
}//}}}

void Csarad113x::StartHoldProcess(unsigned int group)
{//{{{
    // Hold current value of port
    HoldPortVal(group);
    // Notify Hold end process after Hold enactment time
    double hold_enactment_time = EX_HLD_CDT*mADCLKPeriod;
    if (group == emTHGroupA) {
        re_printf("info","Start hold process for T&H group A.\n");
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAHoldComplete].notify(hold_enactment_time,SC_NS);
    } else {
        re_printf("info","Start hold process for T&H group B.\n");
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBHoldComplete].notify(hold_enactment_time,SC_NS);
    }
}//}}}

void Csarad113x::HoldPortVal(unsigned int group)
{//{{{
    for (unsigned int i=0;i<emAllTHCh;i++) {
        if (((((unsigned int)(*THER) >> i)&0x1) == 1)                                   // T&H channel is enabled for T&H function
         && ((((((unsigned int)(*THGSR) >> i)&0x1) == 0)&&(group == emTHGroupA))        // T&H channel of group A
          || (((((unsigned int)(*THGSR) >> i)&0x1) == 1)&&(group == emTHGroupB)))) {    // T&H channel of group B
            mHoldPortVal[i] = GetANIPortVal(i);
        }
    }
}//}}}

void Csarad113x::DelayEndHolding(unsigned int group)
{//{{{
    // Start scanning after 3ADCLK when holding complete
    unsigned int sg = 0;
    if (group == emTHGroupA) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAEndHolding].notify(NextADCLKPosedge(3*mADCLKPeriod),SC_NS);
    } else {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBEndHolding].notify(NextADCLKPosedge(3*mADCLKPeriod),SC_NS);
    }
    // Trigger to suspend other group
    StartTrigger(sg, NextADCLKPosedge(0));
}//}}}

void Csarad113x::StartTrigger(unsigned int sg, double delay_time)
{//{{{
    if (sg == emSG1) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtStartSG1Trigger].notify(delay_time,SC_NS);
    #ifdef SARAD113x_ENABLE_TSN
    } else if (sg == emTSNSG) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtStartSG0Trigger].notify(delay_time,SC_NS);
    #endif
    } else if (sg == emSG2) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtStartSG2Trigger].notify(delay_time,SC_NS);
    } else if (sg == emSG3) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtStartSG3Trigger].notify(delay_time,SC_NS);
    } else if (sg == emPWDSG) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtStartSG4Trigger].notify(delay_time,SC_NS);
    }
}//}}}

// Resume other scanning group when finish scanning of current group
void Csarad113x::ResumeTH(void)
{//{{{
    unsigned int sgA = (unsigned int)(*THACR)["SGS"];
    unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
    if ((sgA != 0) && (mCurrentSG == sgA)) {
        SetCurrentSG(emTHGroupB,true);
    } else if ((sgB != 0) && (mCurrentSG == sgB)) {
        SetCurrentSG(emTHGroupA,true);
    }
    mCheckTriggerMethodEvent.notify(mADCLKPeriod,SC_NS);
}//}}}

// Check whether scanning group is started or not
bool Csarad113x::CheckEnableStart(unsigned int sg)
{//{{{
    if (mIsEnableStart[sg]) {
        mIsEnableStart[sg] = false;
        return true;
    } else {
        return false;
    }
}//}}}

// Check whether sampling is started automatically after holding complete or not
bool Csarad113x::IsAutoStartSampling(void)
{//{{{
    if ((unsigned int)(*THCR)["ASMPMSK"] == 1) {
        return true;
    } else {
        return false;
    }
}//}}}

// HW trigger only start holding when HLDTE = 1
bool Csarad113x::CheckHoldStart(unsigned int group, bool is_trg, eTriggerType trg_type)
{//{{{
    if (!is_trg) { // Hold process by written 1 to THAHLDSTCR.HLDST or THBHLDSTCR.HLDST regardless of setting of THACR.HLDTE or THBCR.HLDTE
        return true;
    }
    // Trigger hold process by S/W,H/W trigger
    unsigned int hldte = (unsigned int)(*THACR)["HLDTE"];
    unsigned int sg = (unsigned int)(*THACR)["SGS"];
    bool result = false;
    if (group == emTHGroupB) {
        hldte = (unsigned int)(*THBCR)["HLDTE"];
        sg = (unsigned int)(*THBCR)["SGS"];
    }
    if (hldte == 1) {
        if (mIsHWTrigger[sg] && (trg_type == emHWTrigger)
         && (((group == emTHGroupA) && ((unsigned int)(*THACR)["HLDCTE"] == 1)) 
          || ((group == emTHGroupB) && ((unsigned int)(*THBCR)["HLDCTE"] == 1)))) { // It is prohibited to assert HW trigger before set 1 to HLDST when both HLDCTE = 1 and HLDTE = 1
            re_printf("warning","When HLDCTE = 1 and HLDTE = 1, HW trigger must be asserted after HLDST is set.\n");
        }
        result = false;
    } else {
        #ifndef SARAD113x_REQ_SECOND_SW_TRIGGER
        if (mIsSWTrigger[sg] && (trg_type == emSWTrigger)) { // Start hold complete trigger after 1 ADCLK if software trigger is asserted when HLDTE = 0
            if (group == emTHGroupA) {
                mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHASWTrigger].notify(NextADCLKPosedge(mADCLKPeriod),SC_NS);
            } else {
                mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBSWTrigger].notify(NextADCLKPosedge(mADCLKPeriod),SC_NS);
            }
        }
        #endif
        result = true;
    }
    mIsHWTrigger[sg] = false;
    mIsSWTrigger[sg] = false;
    return result;
}//}}}

// HW trigger for holding complete triggers when HLDCTE = 1. Hold complete trigger by wait holding enactment time triggers when HLDCTE = 0
bool Csarad113x::CheckHoldComplete(unsigned int group, eTriggerType trg_type)
{//{{{
    unsigned int hldcte = 0;
    unsigned int sg = 0;
    if (group == emTHGroupA) {
        sg = (unsigned int)(*THACR)["SGS"];
        hldcte = (unsigned int)(*THACR)["HLDCTE"];
    } else {
        sg = (unsigned int)(*THBCR)["SGS"];
        hldcte = (unsigned int)(*THBCR)["HLDCTE"];
    }
    if ((((trg_type == emSWTrigger) || (trg_type == emHWTrigger)) && (hldcte == 1)) || ((trg_type == emOtherTrigger) && (hldcte == 0))) {
        if (trg_type == emHWTrigger) {
            mIsHWTrigger[sg] = true;
        } else {
            mIsSWTrigger[sg] = true;
        }
        return true;
    } else {
        return false;
    }
}//}}}

// Fucntion is called at the end of holding to cancel all hold end trigger event
void Csarad113x::EndHolding(unsigned int group)
{//{{{
    // Cancel current hold end event of T&H group
    if (group == emTHGroupA) {
        re_printf("info","Hold process for T&H group A completed.\n");
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAHoldComplete].cancel();
    } else {
        re_printf("info","Hold process for T&H group B completed.\n");
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBHoldComplete].cancel();
    }
}//}}}

void Csarad113x::HWTrigger(unsigned int sg)
{//{{{
    if (!IsReset() && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","Hardware trigger group %d is asserted.\n", sg);
        if (sg == emPWDSG) { // HW trigger for PWM-Diag (1PCLK + 2ADCLK)
            if (EnableTimeCalculation) {
                mHWTriggerEvent[sg].notify(NextADCLKPosedge(NextPCLKPosedge(mPCLKPeriod)+3*mADCLKPeriod),SC_NS);
            } else {
                mHWTriggerEvent[sg].notify(NextADCLKPosedge(tPWDD+mADCLKPeriod),SC_NS);
            }
        } else { // HW trigger for TSN, SGn(n=1~3) (1ADCLK)
            mHWTriggerEvent[sg].notify(NextADCLKPosedge(mADCLKPeriod),SC_NS);
        }
    }
}//}}}

void Csarad113x::SWTrigger(unsigned int sg)
{//{{{
    // SW trigger (3ADCLK)
    if (!IsReset() && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","Software trigger group %d is asserted.\n", sg);
        if (EnableTimeCalculation) {
            mSWTriggerEvent[sg].notify(NextADCLKPosedge(3*mADCLKPeriod),SC_NS);
        } else {
            mSWTriggerEvent[sg].notify(NextADCLKPosedge(tD+mADCLKPeriod),SC_NS);
        }
    }
}//}}}

// Check whether scanning group is set as continuous mode or not
bool Csarad113x::IsContinuousMode(unsigned int sg)
{//{{{
    if ((sg >= emSG1) && (sg <= emSG3) && ((unsigned int)(*(SGCR[sg]))["SCANMD"] == 1)) {
        return true;
    }
    return false;
}//}}}

// Finish all T&H scanning group conversion
void Csarad113x::FinishTHConversion(void)
{//{{{
    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtFinishTHConversion].notify();
}//}}}

// Stop all operation
void Csarad113x::StopOperation(void)
{//{{{
    re_printf("info","SARAD operation is stopped.\n"); 
    InitOperation();
    for (unsigned int i=0;i<emAllSG;i++){
        if ((i >= emSG1) && (i <= emSG3)) {
            mNextVC[i] = (unsigned int)(*(SGVCSP[i]))["VCSP"]; // Set for next conversion
            mFirstVC[i] = (unsigned int)(*(SGVCSP[i]))["VCSP"]; // Set for next conversion
            mLastVC[i] = (unsigned int)(*(SGVCEP[i]))["VCEP"]; // Set for next conversion
        } else {
            mNextVC[i] = 0;
            mFirstVC[i] = 0;
            mLastVC[i] = 0;
        }
        // Deassert all ADCATCNVn 
        mWriteADCATCNVControlEvent[i].notify();
    }
}//}}}

// Assert corresponding ADCATCNVn (n=1~3) output port when trigger is asserted for T&H scanning group
void Csarad113x::AssertADCATCNVTH(unsigned int sg)
{//{{{
    if (sg == emTHGroupA) {
        unsigned int sgA = (unsigned int)(*THACR)["SGS"];
        mADCATCNVnVal[sgA] = true;
        mWriteADCATCNVControlEvent[sgA].notify(NextADCLKPosedge(0),SC_NS);
    } else{
        unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
        mADCATCNVnVal[sgB] = true;
        mWriteADCATCNVControlEvent[sgB].notify(NextADCLKPosedge(0),SC_NS);
    }
}//}}}

// Calculate the sample time for A/D conversion
double Csarad113x::GetSampleTime(unsigned int sg)
{//{{{
    #ifdef SARAD113x_ENABLE_TSN
    if (sg == emTSNSG) { // sample time for TSN scanning group
        return (((unsigned int)(*TSNSMPCR)["TSNSMPT"])*mADCLKPeriod);
    } else { // sample time for SGn(n=1~3) and PWD scanning group
        return ((unsigned int)(*SMPCR)["SMPT"])*mADCLKPeriod;
    }
    #else
    return ((unsigned int)(*SMPCR)["SMPT"])*mADCLKPeriod;
    #endif
}//}}}

// Calculate the conversion time for A/D conversion
double Csarad113x::GetConversionTime(void)
{//{{{
    return (EX_CNVT*mADCLKPeriod);
}//}}}

// Get the repetition time of Scanning group
unsigned int Csarad113x::GetRepetitionTime(unsigned int sg)
{//{{{
    if ((sg >= emSG1) && (sg <= emSG3)) {
        unsigned int sct = (unsigned int)(*(SGCR[sg]))["SCT"];
        return (unsigned int)(0x1 << sct);
    } else {
        return 1;
    }
}//}}}

// A/D conversion function
unsigned int Csarad113x::ADConvert(double value, unsigned int sg, unsigned int vc_num)
{//{{{
    unsigned int ad_data = 0;
    unsigned int uls = GetULS(sg,vc_num);
    unsigned int ulmtb = 0;
    unsigned int llmtb = 0;
    // A/D conversion
    if (value >= Avrefh) {
        if (value > Avrefh) {
            re_printf("warning","Input analog value of physical channel %d (%lld.%06d) is greater than AVREFH value (%lld.%06d).\n",GetGCTRL(sg,vc_num),(unsigned long long) value, FRACTION_TO_INT(value), (unsigned long long) Avrefh, FRACTION_TO_INT(Avrefh));
        }
        ad_data = emMaxADData;
    } else if (value <= 0) {
        ad_data = 0;    
    } else {
        ad_data = (unsigned int)((value*em12bitCoff)/Avrefh);
    }
    // Check Upper/Lower bound error
    if (uls != emDisableULCheck) {
        ulmtb = (unsigned int)(*(ULLMTBR[uls-1]))["ULMTB"];
        llmtb = (unsigned int)(*(ULLMTBR[uls-1]))["LLMTB"];
        if (ad_data > ulmtb) {
            mULError = mULError | (0x1 << 15); // Set for ULER.UE
            mULError = mULError | (0x1 << 7);  // Set for ULER.ULE
        }
        if (ad_data < llmtb) {
            mULError = mULError | (0x1 << 14); // Set for ULER.LE
            mULError = mULError | (0x1 << 7);  // Set for ULER.ULE
        }
        if (mULError != 0) {
            // Set for ULER.ULSG
            if (sg == emPWDSG) {
                mULError = mULError | (0x2 << 12); 
            } else if ((sg >= emSG1) && (sg <= emSG3)) {
                mULError = mULError | (0x1 << 12); 
            } else {
                mULError = mULError | (0x3 << 12); 
            }
            // Set for ULER.MPXE and ULER.MPXV
            mULError = mULError | (GetMPXE(sg,vc_num) << 11); 
            mULError = mULError | (GetMPXV(sg,vc_num) << 8); 
            // Set for ULER.ULECAP
            mULError = mULError | GetGCTRL(sg,vc_num); 
        }
    }
    if ((unsigned int)(*ADCR)["CTYP"] == em10bit) {
        ad_data = ad_data >> 2; // Remove 2 last LSB
        if ((unsigned int)(*ADCR)["CRAC"] == emLeftAlign) {
            ad_data = ad_data << 6;
        }
    } else {
        if ((unsigned int)(*ADCR)["CRAC"] == emLeftAlign) {
            ad_data = ad_data << 4;
        }
    }

    return ad_data;
}//}}}

// Store A/D conversion value
bool Csarad113x::StoreADData(unsigned int data, unsigned int sg, unsigned int vc_num)
{//{{{
    bool is_ow = false;
    if (sg == emPWDSG) { // Overwrite is not detected in PWD
        (*PWDDIR)["MPXE"] = GetMPXE(sg,vc_num);
        (*PWDDIR)["MPXV"] = GetMPXV(sg,vc_num);
        (*PWDDIR)["WFLG"] = 1;
        (*PWDDIR)["ID"] = GetGCTRL(sg, vc_num);
        (*PWDDIR)["PWDDR"] = data;
        (*PWDTSNDR)["PWDDR"] = data;
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        if ((unsigned int)(*(DIR[vc_num]))["WFLG"] == 1) {
            is_ow = true;
        }
        (*(DIR[vc_num]))["MPXE"] = GetMPXE(sg,vc_num);
        (*(DIR[vc_num]))["MPXV"] = GetMPXV(sg,vc_num);
        (*(DIR[vc_num]))["WFLG"] = 1;
        (*(DIR[vc_num]))["ID"] = GetGCTRL(sg, vc_num);
        (*(DIR[vc_num])) = ((unsigned int)(*(DIR[vc_num])) & emDRMask0) |data;
        unsigned int vc_indx = vc_num/2;
        if (vc_num%2 == 0) {
            (*(DR[vc_indx])) = ((unsigned int)(*(DR[vc_indx])) & emDRMask0) | data;
        } else {
            (*(DR[vc_indx])) = ((unsigned int)(*(DR[vc_indx])) & emDRMask1) | (data << 16);
        }
    #ifdef SARAD113x_ENABLE_TSN
    } else {
        if ((unsigned int)(*TSNDIR)["WFLG"] == 1) {
            is_ow = true;
        }
        (*TSNDIR)["WFLG"] = 1;
        (*TSNDIR)["TSNDR"] = data;
        (*PWDTSNDR)["TSNDR"] = data;
    #endif
    }
    return is_ow;
}//}}}

unsigned int Csarad113x::GetMPXE(unsigned int sg, unsigned int vc_num)
{//{{{
    unsigned int mpxe = 0;
    // TSN does not have MPX
    if (sg == emPWDSG) {
        mpxe = (unsigned int)(*PWDVCR)["MPXE"];
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        mpxe = (unsigned int)(*(VCR[vc_num]))["MPXE"];
    }
    return mpxe;
}//}}}

unsigned int Csarad113x::GetMPXV(unsigned int sg, unsigned int vc_num)
{//{{{
    unsigned int mpxv = 0;
    // TSN does not have MPX
    if (sg == emPWDSG) {
        mpxv = (unsigned int)(*PWDVCR)["MPXV"];
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        mpxv = (unsigned int)(*(VCR[vc_num]))["MPXV"];
    }
    return mpxv;
}//}}}

unsigned int Csarad113x::GetADIE(unsigned int sg, unsigned int vc_num)
{//{{{
    unsigned int adie = 1; // Always assert interrupt for TSN and PWD scanning group
    if ((sg >= emSG1) && (sg <= emSG3)) {
        adie = (unsigned int)(*(VCR[vc_num]))["ADIE"];
    }
    return adie;
}//}}}

unsigned int Csarad113x::GetULS(unsigned int sg, unsigned int vc_num)
{//{{{
    unsigned int uls = 0;
    if (sg == emPWDSG) {
        uls = (unsigned int)(*PWDVCR)["ULS"];
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        uls = (unsigned int)(*(VCR[vc_num]))["ULS"];
    } else {
        uls = (unsigned int)(*TSNVCR)["ULS"];
    }
    return uls;
}//}}}

unsigned int Csarad113x::GetGCTRL(unsigned int sg, unsigned int vc_num)
{//{{{
    unsigned int gctrl = 0;
    if (sg == emPWDSG) {
        gctrl = (unsigned int)(*PWDVCR)["GCTRL"];
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        gctrl = (unsigned int)(*(VCR[vc_num]))["GCTRL"];
    } else {
        gctrl = (unsigned int)(*TSNVCR)["TSNGCTRL"];
    }
    return gctrl;
}//}}}

unsigned int Csarad113x::GetCNVCLSSelfDiag(unsigned int vc_num)
{//{{{
    if ((vc_num >= 33) && (vc_num <= 35)) {
        return ((unsigned int)(*(VCR[vc_num]))["CNVCLS"]);
    }
    return 0;
}//}}}

void Csarad113x::ClearDRProcess(unsigned int sg, unsigned int channel)
{//{{{
    if (sg == emPWDSG) {
        // Clear PWDTSNDR if SFTCR.RDCLRE=1
        if ((unsigned int)(*SFTCR)["RDCLRE"] == 1) {
            (*PWDDIR) = 0;
            (*PWDTSNDR)["PWDDR"] = 0;
        }
        // Clear PWDDIR.WFLG when PWDTSNDR is read
        (*PWDDIR)["WFLG"] = 0;
        // Clear SGSTR.SEF of PWD
        ClearScanningEndFlag(emPWDSG);
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        // Clear corresponding DIR.WFLG when DR is read
        unsigned int vc_num = channel*2;
        // Clear DR if SFTCR.RDCLRE=1
        if ((unsigned int)(*SFTCR)["RDCLRE"] == 1) {
            (*(DIR[vc_num])) = 0;
            (*(DIR[vc_num+1])) = 0;
            (*(DR[channel])) = 0;
        }
        (*(DIR[vc_num]  ))["WFLG"] = 0;
        (*(DIR[vc_num+1]))["WFLG"] = 0;
        // Clear SGSTR.SEF of corressponding SGn
        for (unsigned int i=emSG1; i<=emSG3;i++) { 
            if (((unsigned int)(*(SGVCEP[i]))["VCEP"] == vc_num) || ((unsigned int)(*(SGVCEP[i]))["VCEP"] == (vc_num+1))) {
                ClearScanningEndFlag(i);
            }
        }
    #ifdef SARAD113x_ENABLE_TSN
    } else {
        // Clear PWDTSNDR if SFTCR.RDCLRE=1
        if ((unsigned int)(*SFTCR)["RDCLRE"] == 1) {
            (*TSNDIR) = 0;
            (*PWDTSNDR)["TSNDR"] = 0;
        }
        // Clear TSNDIR.WFLG when PWDTSNDR is read
        (*TSNDIR)["WFLG"] = 0;
        // Clear SGSTR.SEF of TSN
        ClearScanningEndFlag(emTSNSG);
    #endif
    }    
}//}}}

void Csarad113x::ClearDIRProcess(unsigned int sg, unsigned int vc_num)
{//{{{
    if (sg == emPWDSG) {
        // Clear PWDDIR, PWDTSNDR.PWDDR if SFTCR.RDCLRE=1
        if ((unsigned int)(*SFTCR)["RDCLRE"] == 1) {
            (*PWDDIR) = 0;
            (*PWDTSNDR)["PWDDR"] = 0;
        }
        // Clear PWDDIR.WFLG when PWDDIR is read
        (*PWDDIR)["WFLG"] = 0;
        // Clear SGSTR.SEF of PWD
        ClearScanningEndFlag(emPWDSG);
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        // Clear DIR.DR if SFTCR.RDCLRE=1
        if ((unsigned int)(*SFTCR)["RDCLRE"] == 1) {
            (*(DIR[vc_num])) = 0;
            if (vc_num%2==0) {
                (*(DR[vc_num/2])) = (*(DR[vc_num/2])) & emDRMask0;
            } else {
                (*(DR[vc_num/2])) = (*(DR[vc_num/2])) & emDRMask1;
            }
        }
        // Clear DIR.WFLG when DIR is read
        (*(DIR[vc_num]))["WFLG"] = 0;
        // Clear SGSTR.SEF of corressponding SGn
        for (unsigned int i=emSG1; i<=emSG3;i++) { 
            if ((unsigned int)(*(SGVCEP[i]))["VCEP"] == vc_num) {
                ClearScanningEndFlag(i);
            }
        }
    #ifdef SARAD113x_ENABLE_TSN
    } else {
        // Clear TSNDIR, PWDTSNDR.TSNDR if SFTCR.RDCLRE=1
        if ((unsigned int)(*SFTCR)["RDCLRE"] == 1) {
            (*TSNDIR) = 0;
            (*PWDTSNDR)["TSNDR"] = 0;
        }
        // Clear TSNDIR.WFLG when TSNDIR is read
        (*TSNDIR)["WFLG"] = 0;
        // Clear SGSTR.SEF of TSN
        ClearScanningEndFlag(emTSNSG);
    #endif
    }
}//}}}

void Csarad113x::PrintVCmessage(std::string msg, unsigned int sg, unsigned int vc_num)
{//{{{
    if (sg == emPWDSG) {
        re_printf("info","%s for PWM-Diag.\n",msg.c_str());
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        re_printf("info","%s for Virtual channel %d.\n",msg.c_str(), vc_num);
    #ifdef SARAD113x_ENABLE_TSN
    } else {
        re_printf("info","%s for Temperature sensor.\n",msg.c_str());
    #endif
    }
}//}}}

void Csarad113x::PCLKMethod(void)  
{//{{{ 
    SetCLKfreq("pclk", sc_dt::uint64_to_double(pclk.read()));
}//}}}

void Csarad113x::ADCLKMethod(void)  
{//{{{ 
    SetCLKfreq("ADCLK", sc_dt::uint64_to_double(ADCLK.read()));
}//}}}

void Csarad113x::AVREFHMethod(void)
{//{{{
    double current_time = sc_time_stamp().to_double();
    if(current_time > 0){
        re_printf("warning","Changing reference voltage AVREFH0 is invalid when simulation time is greater than zero.\n");
    }
}//}}}

void Csarad113x::InitialAVREFHMethod(void)
{//{{{
    if(!mIsRefVolUpdate){
        mIsRefVolUpdate = true;
        Avrefh = (double)(AVREFH0.read());
    }
}//}}}

void Csarad113x::HWTriggerProcessMethod(unsigned int sg)
{//{{{
    if (GetTRGMD(sg) == 1) {
        mIsHWTrigger[sg] = true;
        mCheckTriggerMethodEvent.notify();
    }
}//}}}

void Csarad113x::SWTriggerProcessMethod(unsigned int sg)
{//{{{
    mIsSWTrigger[sg] = true;
    mCheckTriggerMethodEvent.notify();
}//}}}

void Csarad113x::CheckTriggerMethod(void)
{//{{{
    CheckTrigger();
}//}}}

void Csarad113x::UpdateSGACTMethod(unsigned int sg)
{//{{{
    mIsOperating = true;
    (*SGSTR)["SGACT"] = ((unsigned int)(*SGSTR)["SGACT"] & ((~(1<<sg))&0x1F)) | ((mSGACTVal[sg] << sg) & ((1<<sg)&0x1F));
}//}}}

void Csarad113x::UpdateSHACTMethod(void)
{//{{{
    (*SGSTR)["SHACT"] = mSHACTVal;
}//}}}

void Csarad113x::StartTHSamplingMethod(void)
{//{{{
    re_printf("info","Start sampling process for all T&H channels.\n");
    // Set start sampling time
    SetStartSmpTime(emAllTHCh);
    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHStartSampling].notify();
    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAStartSampling].notify();
    mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBStartSampling].notify();
    // Update SGSTR.SHACT after 2 pclk
    mSHACTVal = 1;
    mUpdateSHACTEvent.notify(NextPCLKPosedge(2*mPCLKPeriod), SC_NS);
}//}}}

void Csarad113x::VCSamplingMethod(void)
{//{{{
    PrintVCmessage("Start sampling", mCurrentSG, mCurrentStartVC[mCurrentSG]);
    double sample_time = GetSampleTime(mCurrentSG);
    mStartVCConversionEvent.notify(sample_time,SC_NS);
}//}}}

void Csarad113x::VCConversionMethod(void)
{//{{{
    unsigned int sgA = (unsigned int)(*THACR)["SGS"];
    unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
    unsigned int phy_chn_num = GetGCTRL(mCurrentSG,mCurrentStartVC[mCurrentSG]);
    if (((sgA != 0) && (mCurrentSG == sgA)) || ((sgB != 0) && (mCurrentSG == sgB))) {
        if (phy_chn_num < emAllTHCh) {
            if ((((unsigned int)(*THER) >> phy_chn_num) & 0x1) == 1) {
                if ((unsigned int)(*ADCR)["DGON"] == 1) { // Self-diagnosis mode
                    if (GetCNVCLSSelfDiag(mCurrentStartVC[mCurrentSG]) == 0) {
                        mCurrentAnalogVal = mHoldPortVal[phy_chn_num];
                    } else { // Get usual A/D value at self-diagnosis (not use T&H)
                        mCurrentAnalogVal = GetANIPortVal(phy_chn_num);
                    }
                } else {
                    mCurrentAnalogVal = mHoldPortVal[phy_chn_num];
                }
            } else {
                mCurrentAnalogVal = GetANIPortVal(phy_chn_num);
            }
        } else {
            mCurrentAnalogVal = GetANIPortVal(phy_chn_num);
        }
    } else {
        mCurrentAnalogVal = GetANIPortVal(phy_chn_num);
    }
    PrintVCmessage("Start A/D converting", mCurrentSG, mCurrentStartVC[mCurrentSG]);
    // Reset internal VC variable
    mADData = 0;
    mULError = 0;
    mRepetitionCount++; // Increase count for repetition count variable
    if (mRepetitionCount >= mRepetitionTime) {
        mIsLastRepetition = true;
    } else {
        mIsLastRepetition = false;
    }
    double conv_time = GetConversionTime();
    mEndVCConversionEvent.notify(conv_time - 3*mADCLKPeriod, SC_NS);
}//}}}

void Csarad113x::VCEndConversionMethod(void)
{//{{{
    unsigned int sgA = (unsigned int)(*THACR)["SGS"];
    unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
    // Finish A/D conversion for current scanning group
    if ((sgA != 0) && (mCurrentSG == sgA)) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAFinishVCConversion].notify();
    } else if ((sgB != 0) && (mCurrentSG == sgB)) {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBFinishVCConversion].notify();
    } else {
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtFinishVCConversion].notify();
    }
}//}}}

void Csarad113x::UpdateConversionDataMethod(unsigned int sg)
{//{{{
    re_printf("info","A/D conversion data of scanning group %d is stored to register\n",sg);
    bool is_ow = false;
    bool enb_intr   = false;
    // Store A/D conversion data
    is_ow = StoreADData(mADData, sg, mPreviousVC);
    if (mIsLastRepetition) { // Last time of repetition time
        mIsLastRepetition = false;
        //Write to SGSTR.SEF and Assert Scanning group finish conversion interrupt if enable
        if ((sg == emTSNSG) || (sg == emPWDSG)) {
            enb_intr = true;
            (*SGSTR)["SEF"] = ((unsigned int)(*SGSTR)["SEF"] & ((~(1<<sg))&0x1F)) | ((1<<sg)&0x1F);
        } else if (mPreviousVC == mLastVC[sg]) {
            (*SGSTR)["SEF"] = ((unsigned int)(*SGSTR)["SEF"] & ((~(1<<sg))&0x1F)) | ((1<<sg)&0x1F);
            if ((unsigned int)(*(SGCR[sg]))["ADIE"] == 1) {
                enb_intr = true;
            }
        }
    }
    // Assert VC finish conversion interrupt
    if (!enb_intr) {
        enb_intr = GetADIE(sg,mPreviousVC);
    }
    if (enb_intr) {
        mIntrVal[sg] = true;
        mWriteSGEndInterruptEvent[sg].notify(NextPCLKPosedge(mPCLKPeriod),SC_NS);
    }
    if (is_ow || (mULError != 0)) {
        // Assert Upper/Lower bound error interrupt
        if (mULError != 0) {
            if ((unsigned int)(*ULER)["ULE"] == 1) {
                re_printf("warning","The value of register ULER is not updated because it is not cleared from previous error.\n");
            } else {
                (*ULER) = mULError;
            }
            if ((unsigned int)(*SFTCR)["ULEIE"] == 1) {
                mULEVal = true;
                mWriteULEInterruptEvent.notify(NextPCLKPosedge(mPCLKPeriod),SC_NS);
            }
        }
        // Assert Overwrite error interrupt
        if (is_ow) {
            if ((unsigned int)(*OWER)["OWE"] == 1) {
                re_printf("warning","The value of register OWER is not updated because it is not cleared from previous error.\n");
            } else {
                (*OWER)["OWE"] = 1;
                #ifdef SARAD113x_ENABLE_TSN
                if (sg == emTSNSG) {
                    (*OWER)["OWECAP"] = emTSNOWECAP;
                } else {
                    (*OWER)["OWECAP"] = mPreviousVC;
                }
                #else
                (*OWER)["OWECAP"] = mPreviousVC;
                #endif
            }
        }
        if ((((unsigned int)(*SFTCR)["ULEIE"] == 1) && (mULError != 0)) || (((unsigned int)(*SFTCR)["OWEIE"] == 1) && is_ow)) {
          mADEVal = true;
          mWriteADEInterruptEvent.notify(NextPCLKPosedge(mPCLKPeriod),SC_NS);
        }
    }
}//}}}

#ifdef SARAD113x_ENABLE_TSN
void Csarad113x::ClearTSNDRMethod(void)
{//{{{
    ClearDRProcess(emTSNSG,0);
}//}}}

void Csarad113x::ClearTSNDIRMethod(void)
{//{{{
    ClearDIRProcess(emTSNSG,0);
}//}}}
#endif

void Csarad113x::ClearPWDDRMethod(void)
{//{{{
    ClearDRProcess(emPWDSG,0);
}//}}}

void Csarad113x::ClearDRMethod(unsigned int channel)
{//{{{
    ClearDRProcess(emSG1,channel);
}//}}}

void Csarad113x::ClearDIRMethod(unsigned int vc)
{//{{{
    ClearDIRProcess(emSG1,vc);
}//}}}

void Csarad113x::ClearPWDDIRMethod(void)
{//{{{
    ClearDIRProcess(emPWDSG,0);
}//}}}

void Csarad113x::ResetMethod(void)
{//{{{
    //1. Reset port sensitive
    bool is_cancel_assertreset = false;
    if(preset_n.read() == vpcl::emResetActive){
        mSARPortResetFlag = true;
    }else{
        mSARPortResetFlag = false;
    }
    if(mSARPortResetFlag){
        mIsInitialize = false;
        re_printf("info","Reset signal is asserted.\n"); 
        this->EnableReset(mSARPortResetFlag);
        is_cancel_assertreset = true;
    }else{
        if(mIsInitialize){
            mIsInitialize = false;
            is_cancel_assertreset = false;
        }else{
            re_printf("info","Reset signal is negated.\n");  
            this->EnableReset(mSARPortResetFlag);
            is_cancel_assertreset = true;
        }
    }

    //Cancel AssertReset
    if(is_cancel_assertreset){
        mCmdResetEvent.cancel();
        mCmdCancelResetEvent.cancel();
        mSARCmdResetFlag = false;
    }
}//}}}

void Csarad113x::AssertResetMethod(void)
{//{{{
    mSARCmdResetFlag = true;
    this->EnableReset(mSARCmdResetFlag);
    mCmdCancelResetEvent.notify((unsigned int)mResetPeriod, SC_NS);
}//}}}

void Csarad113x::DeAssertResetMethod(void)
{//{{{ 
    mSARCmdResetFlag = false;
    this->EnableReset(mSARCmdResetFlag);
}//}}}

#ifdef SARAD113x_ENABLE_TSN
void Csarad113x::TSNTRGMethod(void)
{//{{{
    if (!IsReset() && (mPCLKPeriod > 0)&&(mADCLKPeriod > 0)&&(TSN_TRG.read())) {
        HWTrigger(emTSNSG);
    }
}//}}}

void Csarad113x::WriteTSN_TS_ENMethod(void)
{//{{{
    if (IsReset()) {
        TSN_TS_EN.write(false);
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) { 
            if ((unsigned int)(*TSNCR)["TSNEN"] == 1) {
                TSN_TS_EN.write(true);
            } else {
                TSN_TS_EN.write(false);
            }
        }
    }
}//}}}

void Csarad113x::WriteTSN_SELF_DIAGMethod(void)
{//{{{
    if (IsReset()) {
        TSN_SELF_DIAG.write(false);
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) { 
            if ((unsigned int)(*ADCR)["TSNSELFDIAG"] == 1) {
                TSN_SELF_DIAG.write(true);
            } else {
                TSN_SELF_DIAG.write(false);
            }
        }
    }
}//}}}

void Csarad113x::WriteTSNControlMethod(void)
{//{{{
    if (IsReset()) {
        TSN_TSSW.write(false);
        TSN_TSSW_DISCH.write(false);
        TSN_TSMASK.write(false);
        TSN_TRIM.write(0);
    } else {
        double delay_time = 0;
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) { 
            switch (mTSNStateControl) {
                case emAssertTSSW:
                    TSN_TSSW.write(true);
                    mTSNStateControl = emAssertTSSW_DISCH_EN;
                    mWriteTSNControlEvent.notify(mADCLKPeriod,SC_NS);
                    break;
                case emAssertTSSW_DISCH_EN:
                    TSN_TSSW_DISCH.write(true);
                    mTSNStateControl = emDeassertTSSW_DISCH_EN;
                    mWriteTSNControlEvent.notify(20*mADCLKPeriod,SC_NS);
                    break;
                case emDeassertTSSW_DISCH_EN:
                    TSN_TSSW_DISCH.write(false);
                    mTSNStateControl = emAssertTSMASK;
                    mWriteTSNControlEvent.notify(mADCLKPeriod,SC_NS);
                    break;
                case emAssertTSMASK:
                    TSN_TSMASK.write(true);
                    mTSNStateControl = emDeassertTSMASK;
                    delay_time = (((unsigned int)(*TSNSMPCR) - 23 + 2) + 1)*mADCLKPeriod;
                    mWriteTSNControlEvent.notify(delay_time,SC_NS);
                    break;
                case emDeassertTSMASK:
                    TSN_TSMASK.write(false);
                    mTSNStateControl = emDeassertTSSW;
                    delay_time = GetConversionTime() - 3; 
                    mWriteTSNControlEvent.notify(delay_time,SC_NS);
                    break;
                case emDeassertTSSW:
                    TSN_TSSW.write(false);
                    break;
                case emDeassertAll:
                    TSN_TSSW.write(false);
                    TSN_TSSW_DISCH.write(false);
                    TSN_TSMASK.write(false);
                    TSN_TRIM.write(0);
                    break;
                default: 
                    break;
            }
        }
    }
}//}}}
#endif

void Csarad113x::SG1TRGMethod(void)
{//{{{
    if (!IsReset() && (mPCLKPeriod > 0)&&(mADCLKPeriod > 0)&&(SG1_TRG.read())) {
        HWTrigger(emSG1);
    }
}//}}}

void Csarad113x::SG2TRGMethod(void)
{//{{{
    if (!IsReset()&&(mPCLKPeriod > 0)&&(mADCLKPeriod > 0)&&(SG2_TRG.read())) {
        HWTrigger(emSG2);
    }
}//}}}

void Csarad113x::SG3TRGMethod(void)
{//{{{
    if (!IsReset()&&(mPCLKPeriod > 0)&&(mADCLKPeriod > 0)&&(SG3_TRG.read())) {
        HWTrigger(emSG3);
    }
}//}}}

void Csarad113x::PVCRTRGMethod(void)
{//{{{
    if (!IsReset()&&(mPCLKPeriod > 0)&&(mADCLKPeriod > 0)&&(PVCR_TRG.read())) {
        HWTrigger(emPWDSG);
    }
}//}}}

void Csarad113x::PVCR_VALUEMethod(void)
{//{{{
    unsigned int pvcr_value = (unsigned int)PVCR_VALUE.read();
    (*PWDVCR)["MPXE"] = (pvcr_value >> 11) & 0x1;   // PVCR_VALUE[11]
    (*PWDVCR)["MPXV"] = (pvcr_value >>  8) & 0x7;   // PVCR_VALUE[10:8]
    (*PWDVCR)["ULS"]  = (pvcr_value >>  6) & 0x3;   // PVCR_VALUE[7:6]
    (*PWDVCR)["GCTRL"]= (pvcr_value      ) & 0x3F;  // PVCR_VALUE[5:0]
}//}}}

void Csarad113x::WriteADOPControlMethod(void)
{//{{{
    if (IsReset()) {
        ADOP_OPA1_WEN.write(false);
        ADOP_OPA2_WEN.write(false);
        ADOP_OPA3_WEN.write(false);
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) { 
            ADOP_OPA1_WEN.write(mADOPControlVal[0]);
            ADOP_OPA2_WEN.write(mADOPControlVal[1]);
            ADOP_OPA3_WEN.write(mADOPControlVal[2]);
            bool en_deassert = false;
            for (unsigned int i=emSG1;i<=emSG3;i++){
                // Deassert after 1 PCLK period
                if (mADOPControlVal[i-1]) { // sg(1~3) <==> val(0~2)
                    mADOPControlVal[i-1] = false;
                    en_deassert = true;
                }
            }
            if (en_deassert) {
                mWriteADOPControlEvent.notify(mPCLKPeriod,SC_NS);
            }
        }
    }

}//}}}

void Csarad113x::WritePVCR_MUXCURMethod(void)
{//{{{
    if (IsReset()) {
        PVCR_MUXCUR.write(0);
    } else {
        PVCR_MUXCUR.write((sc_uint<3>)mPVCR_MUXCURVal);
    }
}//}}}

void Csarad113x::WriteADCATCNVControlMethod(unsigned int sg)
{//{{{
    if (IsReset()) {
        switch (sg) {
            #ifdef SARAD113x_ENABLE_TSN
            case emTSNSG: ADCATCNV0.write(false);break;
            #endif
            case emSG1:   ADCATCNV1.write(false);break;
            case emSG2:   ADCATCNV2.write(false);break;
            case emSG3:   ADCATCNV3.write(false);break;
            case emPWDSG: ADCATCNV4.write(false);break;
            default: break;
        } 
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            switch (sg) {
                #ifdef SARAD113x_ENABLE_TSN
                case emTSNSG: ADCATCNV0.write(mADCATCNVnVal[0]);break;
                #endif
                case emSG1:   ADCATCNV1.write(mADCATCNVnVal[1]);break;
                case emSG2:   ADCATCNV2.write(mADCATCNVnVal[2]);break;
                case emSG3:   ADCATCNV3.write(mADCATCNVnVal[3]);break;
                case emPWDSG: ADCATCNV4.write(mADCATCNVnVal[4]);break;
                default: break;
            } 
            // Update SGCR.SGACTn after 2PCLK period
            if (mADCATCNVnVal[sg]) {
                mSGACTVal[sg] = 1;
            } else {
                mSGACTVal[sg] = 0;
            }
            mUpdateSGACTEvent[sg].notify(NextPCLKPosedge(2*mPCLKPeriod), SC_NS);
        }
    }
}//}}}

void Csarad113x::WriteSGEndInterruptMethod(unsigned int sg)
{//{{{
    if (IsReset()) {
        switch (sg) {
            #ifdef SARAD113x_ENABLE_TSN
            case emTSNSG: INT_TSN.write(false);break;
            #endif
            case emSG1:   INT_SG1.write(false);break;
            case emSG2:   INT_SG2.write(false);break;
            case emSG3:   INT_SG3.write(false);break;
            case emPWDSG: PVCR_END.write(false);break;
            default:break;
        }
    } else {
        std::string intr_name = "";
        switch (sg) {
            #ifdef SARAD113x_ENABLE_TSN
            case emTSNSG:
                intr_name = "INT_TSN";
                INT_TSN.write(mIntrVal[sg]);
                break;
            #endif
            case emSG1:
                intr_name = "INT_SG1";
                INT_SG1.write(mIntrVal[sg]);
                break;
            case emSG2:
                intr_name = "INT_SG2";
                INT_SG2.write(mIntrVal[sg]);
                break;
            case emSG3:
                intr_name = "INT_SG3";
                INT_SG3.write(mIntrVal[sg]);
                break;
            case emPWDSG:
                intr_name = "PVCR_END";
                PVCR_END.write(mIntrVal[sg]);
                break;
            default:
                break;
        }
        // Deassert interrupt after 1 PCLK period
        DumpInterruptMessage(intr_name,mIntrVal[sg]);
        if (mIntrVal[sg]) {
            mINTActiveNum[sg]++;
            mIntrVal[sg] = false;
            mWriteSGEndInterruptEvent[sg].notify(mPCLKPeriod,SC_NS);
        }
    }
}//}}}

void Csarad113x::WriteULEInterruptMethod(void)
{//{{{
    if (IsReset()) {
        ULE.write(false);
    } else {
        ULE.write(mULEVal);
        DumpInterruptMessage("ULE",mULEVal);
        if (mULEVal) {
            mULEActiveNum++;
        }
    }
}//}}}

void Csarad113x::WriteADEInterruptMethod(void)
{//{{{
    if (IsReset()) {
        INT_ADE.write(false);
    } else {
        INT_ADE.write(mADEVal);
        // Deassert INT_ADE interrupt after 1 PCLK period
        DumpInterruptMessage("INT_ADE",mADEVal);
        if (mADEVal) {
            mINTADEActiveNum++;
            mADEVal = false;
            mWriteADEInterruptEvent.notify(mPCLKPeriod,SC_NS);
        }
    }
}//}}}

//Reset function
void Csarad113x::EnableReset(const bool is_active)
{//{{{
    if(is_active){
        //1.Dump report
        re_printf("info","SARAD113x moves to RESET state.\n");

        //2.Global variable init
        Initialize();

        //3. Cancel all event
        mWriteADOPControlEvent.cancel();
        #ifdef SARAD113x_ENABLE_TSN
        mWriteTSNControlEvent.cancel();
        mWriteTSN_TS_ENEvent.cancel();
        mWriteTSN_SELF_DIAGEvent.cancel();
        #endif
        mWritePVCR_MUXCUREvent.cancel();
        for (unsigned int i=0;i<emAllSG;i++) {
            mHWTriggerEvent[i].cancel();;
            mSWTriggerEvent[i].cancel();
            mWriteADCATCNVControlEvent[i].cancel();
            mUpdateSGACTEvent[i].cancel();
            mUpdateConversionDataEvent[i].cancel();
        }
        mCheckTriggerMethodEvent.cancel();
        mStartVCSamplingEvent.cancel();
        mStartVCConversionEvent.cancel();
        mEndVCConversionEvent.cancel();
        mStartTHSamplingEvent.cancel();

        //4. Initialize output port
        mWriteADOPControlEvent.notify();
        #ifdef SARAD113x_ENABLE_TSN
        mWriteTSNControlEvent.notify();
        mWriteTSN_TS_ENEvent.notify();
        mWriteTSN_SELF_DIAGEvent.notify();
        #endif
        mWritePVCR_MUXCUREvent.notify();
        for (unsigned int i=0;i<emAllSG;i++) {
            mWriteADCATCNVControlEvent[i].notify();
            mWriteSGEndInterruptEvent[i].notify();
        }
        mWriteULEInterruptEvent.notify();
        mWriteADEInterruptEvent.notify();

        //5. Assert reset event
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtResetAssert].notify();
    }else{
        re_printf("info","SARAD113x moves from RESET state to IDLE state.\n");
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtResetDeassert].notify();
    }

    //4.Reset registers
    Csarad113x_regif::EnableReset(is_active);

}//}}}

//information function
void Csarad113x::DumpInfo (const char *type, const char *message, ... ) 
{//{{{
    if((message != NULL)&&(type != NULL)){
        printf ("PROFILE(%s): F1K/SARAD113x: ", type);
        //print body
        va_list argptr;
        va_start(argptr, message);
        vprintf(message, argptr);
        va_end(argptr);
    }
}//}}}

//handleCommand function
void Csarad113x::DumpStatInfo(void)
{//{{{
//1.Dump information
    DumpInfo("StatInfo", "Info[%s]\n",sc_time_stamp().to_string().c_str());
    #ifdef SARAD113x_ENABLE_TSN
    DumpInfo("StatInfo", " INT_TSN  active number : %d\n", mINTActiveNum[0]); 
    #endif
    DumpInfo("StatInfo", " INT_SG1  active number : %d\n", mINTActiveNum[1]); 
    DumpInfo("StatInfo", " INT_SG2  active number : %d\n", mINTActiveNum[2]); 
    DumpInfo("StatInfo", " INT_SG3  active number : %d\n", mINTActiveNum[3]); 
    DumpInfo("StatInfo", " PVCR_END active number : %d\n", mINTActiveNum[4]); 
    DumpInfo("StatInfo", " INT_ADE  active number : %d\n", mINTADEActiveNum); 
    DumpInfo("StatInfo", " ULE      active number : %d\n", mULEActiveNum); 
    DumpInfo("StatInfo", "EndInfo.\n");
    
    //2.Reset information variable
    for (unsigned int i=0;i<emAllSG;i++) {
        mINTActiveNum[i] = 0;
    }
    mINTADEActiveNum = 0;
    mULEActiveNum = 0;
}//}}}

void Csarad113x::DumpInterruptMessage (const std::string intr_name, const bool is_assert) 
{//{{{
    if(DumpInterrupt){
        if (is_assert) {
            re_printf("info","INT [SARAD113x: %s] Assert.\n", intr_name.c_str());
        } else {
            re_printf("info","INT [SARAD113x: %s] Deassert.\n", intr_name.c_str());
        }
    }
}//}}}

void Csarad113x::DumpActivity(unsigned int sg, unsigned int vc_num, double start_time)
{//{{{
    if(EnableConvertInfo) {
        re_printf("info","Start simulation time     : %lld.%06d\n", (unsigned long long) start_time, FRACTION_TO_INT(start_time)); 
        re_printf("info","End simulation time       : %lld.%06d\n", (unsigned long long) sc_time_stamp().to_double(), FRACTION_TO_INT(sc_time_stamp().to_double())); 
        re_printf("info","The scanning group        : %d\n", sg);
        if (!IsContinuousMode(sg)) {
            re_printf("info","The scanning mode         : Multi cycle mode\n");
            if ((sg == emTSNSG) || (sg == emPWDSG)) {
                re_printf("info","The frequency count       : %d/1\n", mScanFreqCount[sg]); 
            } else {
                re_printf("info","The frequency count       : %d/%d\n", mScanFreqCount[sg], (unsigned int)(*(SGMCYCR[sg]))["MCYC"]+1); 
            }
        } else {
            re_printf("info","The scanning mode         : Continuous mode\n"); 
        }
        re_printf("info","The VC number             : %d\n", vc_num);
        re_printf("info","The VCR value             : 0x%X\n", (unsigned int)(*VCR[vc_num])); 
        re_printf("info","The repetition count      : %d/%d\n", mRepetitionCount, mRepetitionTime);
        std::ostringstream ad_type;
        ad_type.str("");
        if ((unsigned int)(*ADCR)["CTYP"] == em10bit) {
            ad_type << "10 bit";
        } else {
            ad_type << "12 bit";
        }
        if ((unsigned int)(*ADCR)["CRAC"] == emLeftAlign) {
            ad_type << "/left-align";
        } else {
            ad_type << "/right-align";
        }
        re_printf("info","The A/D conversion type   : %s\n", ad_type.str().c_str());
        re_printf("info","The A/D conversion value  : 0x%X\n", mADData);
    }
}//}}}

double Csarad113x::GetTimeResolution(void)
{//{{{
    double tu_value = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)){
        tu_value = 1000000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)){
        tu_value = 1000000000000LL;
    } else if (sc_get_time_resolution() < sc_time(1, SC_US)){
        tu_value = 1000000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)){
        tu_value = 1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)){
        tu_value = 1000;
    } else {
        tu_value = 1;
    }
    return tu_value;
}//}}}

void Csarad113x::AssertReset(const double start_time, const double period)
{//{{{
    if((!mSARPortResetFlag)&&(!mSARCmdResetFlag)) {
        re_printf("info","SARAD113x will reset for %lld.%06d ns after %lld.%06d ns.\n", (unsigned long long) period, FRACTION_TO_INT(period), (unsigned long long) start_time, FRACTION_TO_INT(start_time));
        mCmdResetEvent.notify(start_time, SC_NS);
        mResetPeriod = (unsigned int)period;
    }else{
        re_printf("warning","Reset is in progress.\n");
    }
}//}}}

void Csarad113x::SetCLKfreq(std::string clk_name, double clk_freq)
{//{{{
    double time_unit = 0;
    double pclk_period = 0;
    double adclk_period = 0;
    bool is_period_valid = true;

    // Setting clock values
    if (clk_name == "pclk") {
        if (mIsOperating) {
            re_printf("warning","The frequency of pclk clock is changed while SARAD113x model is operating.\n");
        }

        if (clk_freq == 0) {
            pclk_period = 0;
        } else {
            pclk_period  = (double)(emNanoSecond / clk_freq);
            time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > pclk_period) {
                is_period_valid = false;
                re_printf("warning","The pclk period is less than 1 unit time of system.\n");
            }
        }

        if (is_period_valid) {
            mPCLKPeriod = pclk_period;
            SetLatency_TLM(pclk_period, false);
            re_printf("info","The pclk is set with a frequency as %lld.%06d\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
        }
    } else if (clk_name == "ADCLK") {
        if (mIsOperating) {
            re_printf("warning","The frequency of ADCLK clock is changed while SARAD113x model is operating.\n");
        }

        if (clk_freq == 0) {
            adclk_period = 0;
        } else {
            adclk_period = (double)(emNanoSecond / clk_freq);
            time_unit    = (double)(emNanoSecond / GetTimeResolution());
            if (time_unit > adclk_period) {
                is_period_valid = false;
                re_printf("warning","The ADCLK period is less than 1 unit time of system.\n");
            }
        }

        if (is_period_valid) {
            mADCLKPeriod = adclk_period;
            re_printf("info","The ADCLK is set with a frequency as %lld.%06d\n", (unsigned long long) clk_freq, FRACTION_TO_INT(clk_freq));
        }
    } else {
        re_printf("error","Clock name is invalid.\n");
    }

    // Stop operation when clock frequency = 0
    if (((clk_name == "pclk") || (clk_name == "ADCLK")) && (clk_freq == 0) && mIsOperating) {
        mTSNStateControl = emDeassertAll;
        // Cancel all event
        mWriteADOPControlEvent.cancel();
        #ifdef SARAD113x_ENABLE_TSN
        mWriteTSNControlEvent.cancel();
        mWriteTSN_TS_ENEvent.cancel();
        mWriteTSN_SELF_DIAGEvent.cancel();
        #endif
        mWritePVCR_MUXCUREvent.cancel();
        for (unsigned int i=0;i<emAllSG;i++) {
            mIsHWTrigger[i] = false;
            mIsSWTrigger[i] = false;
            mHWTriggerEvent[i].cancel();;
            mSWTriggerEvent[i].cancel();
            mWriteADCATCNVControlEvent[i].cancel();
            mUpdateSGACTEvent[i].cancel();
            mUpdateConversionDataEvent[i].cancel();
        }
        mCheckTriggerMethodEvent.cancel();
        mStartVCSamplingEvent.cancel();
        mStartVCConversionEvent.cancel();
        mEndVCConversionEvent.cancel();
        mStartTHSamplingEvent.cancel();
        // Halt the operation when clock frequency is set to 0
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtHaltTrigger].notify();
    }
}//}}}

void Csarad113x::SetLatency_TLM(const double pclk_period, const bool is_constructor)
{//{{{ 
    vpcl::tlm_if_tgt_parameter tgt_param = vpcl::tlm_tgt_if<32>::tgt_get_param();
    if(is_constructor){
        tgt_param.fw_req_phase = tlm::END_REQ;
    }
    sc_time new_clock(pclk_period, SC_NS);
    tgt_param.bus_clk = new_clock;
    tgt_param.rd_latency = tgt_param.rd_req_latency + 2 * tgt_param.bus_clk;
    tgt_param.wr_latency = tgt_param.wr_req_latency + 2 * tgt_param.bus_clk;
    vpcl::tlm_tgt_if<32>::tgt_set_param(&tgt_param);
}//}}}

std::string Csarad113x::CommandCB(const std::vector<std::string>& args)
{//{{{
    double present_time = sc_time_stamp().to_double();
    std::vector<std::string> _args = args;
    std::string ret = "";

    // Remove the "command" argument. This process allows user to use
    // a handleCommand with/without "command" argument
 
    if (_args[0] == "command") {
        _args.erase(_args.begin());
    }

    if((_args[0] == "Avrefh")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing reference voltage Avrefh is invalid when simulation time is greater than zero.\n");
            Avrefh = mPreAvrefh; 
        }else{
            mPreAvrefh = Avrefh;
            mIsRefVolUpdate = true;
        }
    }else if((_args[0] == "AVcc")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing AVcc is invalid when simulation time is greater than zero.\n");
            AVcc = mPreAVcc; 
        }else{
            mPreAVcc = AVcc;
        }
    }else if((_args[0] == "AVss")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing AVss is invalid when simulation time is greater than zero.\n");
            AVss = mPreAVss; 
        }else{
            mPreAVss = AVss;
        }
    }else if((_args[0] == "EX_HLD_CDT")&&((int)_args.size() == 2)){
        if(mIsOperating){
            re_printf("warning","Changing the EX_HLD_CDT parameter is not affected when SARAD113x model is operating.\n");
            EX_HLD_CDT = mPreEX_HLD_CDT; 
        }else{
            mPreEX_HLD_CDT = EX_HLD_CDT;
        }
    }else if((_args[0] == "EX_CNVT")&&((int)_args.size() == 2)){
        if(mIsOperating){
            re_printf("warning","Changing the EX_CNVT parameter is not affected when SARAD113x model is operating.\n");
            EX_CNVT = mPreEX_CNVT; 
        }else{
            if (EX_CNVT < 3) {
                re_printf("warning","EX_CNVT must be equal or greater than 3 cycles (AD clock).\n");
                EX_CNVT = mPreEX_CNVT; 
            } else {
                mPreEX_CNVT = EX_CNVT;
            }
        }
    }else if((_args[0] == "tD")&&((int)_args.size() == 2)){
        if (mIsOperating) {
            re_printf("warning","Changing the tD parameter is not affected when SARAD113x model is operating.\n");
            tD = mPretD; 
        }else{
            mPretD = tD;
        }
    }else if((_args[0] == "tPWDD")&&((int)_args.size() == 2)){
        if (mIsOperating) {
            re_printf("warning","Changing the tPWDD parameter is not affected when SARAD113x model is operating.\n");
            tPWDD = mPretPWDD; 
        }else{
            mPretPWDD = tPWDD;
        }
    }else if((_args[0] == "tED")&&((int)_args.size() == 2)){
        if (mIsOperating) {
            re_printf("warning","Changing the tED parameter is not affected when SARAD113x model is operating.\n");
            tED = mPretED; 
        }else{
            mPretED = tED;
        }
    }else if((_args[0] == "EnableTimeCalculation")&&((int)_args.size() == 2)){
        if(present_time > 0){
            re_printf("warning","Changing delay times calculation is invalid when simulation time is greater than zero.\n");
            EnableTimeCalculation = mPreEnableTimeCalculation;
        }else{
            mPreEnableTimeCalculation = EnableTimeCalculation;
        }
    }
    return ret;
}//}}}

unsigned int Csarad113x::GetTRGMD(const unsigned int sg)
{//{{{
    if (sg == emPWDSG) {
        return (unsigned int)(*PWDSGCR)["PWDTRGMD"];
    } else if ((sg >= emSG1) && (sg <= emSG3)) {
        return (unsigned int)(*(SGCR[sg]))["TRGMD"];
    } else {
        return (unsigned int)(*TSNSGCR)["TSNTRGMD"];
    }
}//}}}

unsigned int Csarad113x::GetWrittenData(void)
{//{{{
    return mPWDATAVal;   
}//}}}

bool Csarad113x::CheckAccess(const unsigned int sg, vpcl::re_register* reg, RegCBstr str)
{//{{{
    bool result = true;
    if (sg == emAllSG) { // Check all Scanning group
        if ((unsigned int)(*SGSTR)["SGACT"] != 0) {
            result = false;
        } else {
            for (unsigned int i = 0;i<emAllSG;i++){
                if (GetTRGMD(i) != 0) {
                    result = false;
                    break;
                }
            }
        }
        if (!result) {
            re_printf("warning","Register %s can be accessed only when SGACT and TRGMD of all scanning groups are 0.\n",reg->name().c_str());
            (*reg) = str.pre_data;
        }
    } else if (sg == emAllSGn) { // Check all Scanning group SGn (n=1~3)
        for (unsigned int i = emSG1;i<=emSG3;i++){
            if ((((unsigned int)(*SGSTR)["SGACT"] >> i)&0x1) != 0) {
                result = false;
                break;
            } else {
                if (GetTRGMD(i) != 0) {
                    result = false;
                    break;
                }
            }
        }
        if (!result) {
            re_printf("warning","Register %s can be accessed only when SGACT and TRGMD of all scanning groups n (n=1~3) are 0.\n",reg->name().c_str());
            (*reg) = str.pre_data;
        }
    } else {
        if (((((unsigned int)(*SGSTR)["SGACT"] >> sg)&0x1)!= 0)||(GetTRGMD(sg) != 0)) {
            result = false;
            re_printf("warning","Register %s can be accessed only when SGACT and TRGMD of scanning group %d are 0.\n",reg->name().c_str(),sg);
            (*reg) = str.pre_data;
        }
    }
    return result;
}//}}}

void Csarad113x::cb_VCR_GCTRL(RegCBstr str)
{//{{{
    if (CheckAccess(emAllSG,VCR[str.channel],str)) {
        if ((unsigned int)(*SGSTR)["SHACT"] != 0) {
            re_printf("warning","The virtual channels setting should not be changed when T&H is operating.\n");
            (*(VCR[str.channel])) = str.pre_data;
        } else {
            if (((unsigned int)(*(VCR[str.channel]))["GCTRL"]) > (emMaxPhyChannel)) {
                re_printf("warning", "Setting value to %s.GCTRL must be equal or less than %d.\n", VCR[str.channel]->name().c_str(),emMaxPhyChannel);
                (*(VCR[str.channel]))["GCTRL"] = str.pre_data&0x3F;
            }
        }
    }
}//}}}

void Csarad113x::cb_TSNVCR_ULS(RegCBstr str)
{//{{{
    CheckAccess(emAllSG,TSNVCR,str);
}//}}}

void Csarad113x::cb_DR_DR0(RegCBstr str)
{//{{{
    mClearDREvent[str.channel].notify();
}//}}}

void Csarad113x::cb_PWDTSNDR_TSNDR(RegCBstr str)
{//{{{
    #ifdef SARAD113x_ENABLE_TSN
    mClearTSNDREvent.notify();
    #endif
    mClearPWDDREvent.notify();
}//}}}

void Csarad113x::cb_DIR_DR(RegCBstr str)
{//{{{
    mClearDIREvent[str.channel].notify();
}//}}}

void Csarad113x::cb_TSNDIR_TSNDR(RegCBstr str)
{//{{{
    #ifdef SARAD113x_ENABLE_TSN
    mClearTSNDIREvent.notify();
    #endif
}//}}}

void Csarad113x::cb_PWDDIR_PWDDR(RegCBstr str)
{//{{{
    mClearPWDDIREvent.notify();
}//}}}

void Csarad113x::cb_ADHALTR_HALT(RegCBstr str)
{//{{{
    // Clear SGSTR.SHACT and SGSTR.SGACT if ADHALTR.HALT is set to 1
    if ((unsigned int)(*ADHALTR)["HALT"] == 1) {
        (*SGSTR)["SHACT"] = 0;
        (*SGSTR)["SGACT"] = 0;
        // Halt the operation: Stop operation, clear all trigger event, notify HaltTrigger
        StopOperation();
        mSARAD113xFSMEvent[Csarad113x_fsm::emEvtHaltTrigger].notify();
    }
    // Clear after write
    (*ADHALTR)["HALT"] = 0;
}//}}}

void Csarad113x::cb_ADCR_SUSMTD(RegCBstr str)
{//{{{
    if (CheckAccess(emAllSG,ADCR,str)) {
        #ifdef SARAD113x_ENABLE_TSN
        mWriteTSN_SELF_DIAGEvent.notify(NextADCLKPosedge(0),SC_NS);
        #endif
        if (((unsigned int)(*ADCR)["DGON"] == 1) && (((str.pre_data >> 7)&0x1) == 1)) { // ADCR.DGON is written to 1 the second time
            UpdateSelfDiag();
        }
    }
}//}}}

void Csarad113x::cb_TSNCR_TSNEN(RegCBstr str)
{//{{{
    if (CheckAccess(emTSNSG,TSNCR,str)) {
        #ifdef SARAD113x_ENABLE_TSN
        if ((unsigned int)(*TSNCR)["TSNEN"] == 1) {
            mAccessTimeTSNCR = GetCurTime(); 
        }
        mWriteTSN_TS_ENEvent.notify(NextPCLKPosedge(0),SC_NS);
        #endif
    }
}//}}}

void Csarad113x::cb_THSMPSTCR_SMPST(RegCBstr str)
{//{{{
    // Do not start sampling if pclk and ADCLK = 0
    if ((mPCLKPeriod > 0)&&(mADCLKPeriod > 0)) {
        if ((unsigned int)(*THSMPSTCR)["SMPST"] == 1) {
            // Set the SMPST after ending both group A and B T&H operation
            unsigned int sgA = (unsigned int)(*THACR)["SGS"];
            unsigned int sgB = (unsigned int)(*THBCR)["SGS"];
            unsigned int st_sgA = ((unsigned int)(*SGSTR)["SGACT"] >> sgA)&0x1;
            unsigned int st_sgB = ((unsigned int)(*SGSTR)["SGACT"] >> sgB)&0x1;
            if ((sgA != 0) || (sgB != 0)) {
                if (((sgA != 0) && (st_sgA != 0)) || ((sgB != 0) && (st_sgB != 0))) {
                    re_printf("warning","Sampling process can not start because operation of Scanning group A or B is not ended.\n");
                // Do not specify same scanning group to both group A and group B
                } else if (sgA == sgB) {
                    re_printf("warning","Sampling process can not start because the same scanning group is set for both T&H group A and B.\n");
                } else {
                    if ((unsigned int)(*ADCR)["DGON"] == 1) {
                        if (((sgA != 0) && (((unsigned int)(*THACR)["HLDCTE"] != 1) || ((unsigned int)(*THACR)["HLDTE"] != 0))) 
                         || ((sgB != 0) && (((unsigned int)(*THBCR)["HLDCTE"] != 1) || ((unsigned int)(*THBCR)["HLDTE"] != 0)))) {
                            re_printf("warning","Sampling process can not start because HLDCTE is not set 1 or HLDTE is set to 0 when T&H self-diagnosis is used.\n");
                        } else {
                            mStartTHSamplingEvent.notify(NextADCLKPosedge(mADCLKPeriod), SC_NS);
                        }
                    } else {
                        bool enable_th = false;
                        for (unsigned int i=0;i<emAllTHCh;i++) {
                            if ((((unsigned int)(*THER) >> i) & 0x1) == 1) {
                                enable_th = true;
                                break;
                            }
                        }
                        if (enable_th) {
                            mStartTHSamplingEvent.notify(NextADCLKPosedge(mADCLKPeriod), SC_NS);
                        } else {
                            re_printf("warning","Sampling process can not start because all of T&H channel is disabled.\n");
                        }
                    }
                }
            } else {
                re_printf("warning","Sampling process can not start because T&H group A and B are disabled.\n");
            }
        }
    }
    // Clear after write
    (*THSMPSTCR)["SMPST"] = 0;
}//}}}

void Csarad113x::cb_THCR_ASMPMSK(RegCBstr str)
{//{{{
    CheckAccess(emAllSG,THCR,str);
}//}}}

void Csarad113x::cb_THAHLDSTCR_HLDST(RegCBstr str)
{//{{{
    // Do not start holding if pclk and ADCLK = 0
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if ((unsigned int)(*THACR)["SGS"] != 0) {
            bool enable_th = false;
            for (unsigned int i=0;i<emAllTHCh;i++) {
                if (((((unsigned int)(*THER) >> i) & 0x1) == 1) && ((((unsigned int)(*THGSR) >> i) & 0x1) == emTHGroupA)) {
                    enable_th = true;
                    break;
                }
            }
            if (enable_th) {
                mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHAHoldStart].notify(NextADCLKPosedge(mADCLKPeriod), SC_NS);
            } else {
                re_printf("warning","The hold process of group A can not start because all of T&H channel for group A is disabled.\n");
            }
        } else {
            re_printf("warning","The hold process of group A can not start because T&H group A is disabled.\n");
        }
    }
    // Clear after write
    (*THAHLDSTCR)["HLDST"] = 0;
}//}}}

void Csarad113x::cb_THBHLDSTCR_HLDST(RegCBstr str)
{//{{{
    // Do not start holding if pclk and ADCLK = 0
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if ((unsigned int)(*THBCR)["SGS"] != 0) {
            bool enable_th = false;
            for (unsigned int i=0;i<emAllTHCh;i++) {
                if (((((unsigned int)(*THER) >> i) & 0x1) == 1) && ((((unsigned int)(*THGSR) >> i) & 0x1) == emTHGroupB)) {
                    enable_th = true;
                    break;
                }
            }
            if (enable_th) {
                mSARAD113xFSMEvent[Csarad113x_fsm::emEvtTHBHoldStart].notify(NextADCLKPosedge(mADCLKPeriod), SC_NS);
            } else {
                re_printf("warning","The hold process of group B can not start because all of T&H channel for group B is disabled.\n");
            }
        } else {
            re_printf("warning","The hold process of group B can not start because T&H group B is disabled.\n");
        }
    }
    // Clear after write
    (*THBHLDSTCR)["HLDST"] = 0;
}//}}}

void Csarad113x::cb_THACR_SGS(RegCBstr str)
{//{{{
    if (!((((unsigned int)(*THACR)["HLDCTE"] == 0) && (((unsigned int)(*THACR) & 0xDF) == (str.pre_data & 0xDF))) // Users can clear HLDCTE and HLDTE during operation
     || (((unsigned int)(*THACR)["HLDTE"] == 0)  && (((unsigned int)(*THACR) & 0xEF) == (str.pre_data & 0xEF)))
     || (((unsigned int)(*THACR)["HLDCTE"] == 0) && ((unsigned int)(*THACR)["HLDTE"] == 0) && ((unsigned int)(*THACR)["SGS"] == (str.pre_data&0x3))))) {
        CheckAccess(emAllSG,THACR,str);
    }
}//}}}

void Csarad113x::cb_THBCR_SGS(RegCBstr str)
{//{{{
    if (!((((unsigned int)(*THBCR)["HLDCTE"] == 0) && (((unsigned int)(*THBCR) & 0xDF) == (str.pre_data & 0xDF))) // Users can clear HLDCTE and HLDTE during operation
     || (((unsigned int)(*THBCR)["HLDTE"] == 0)  && (((unsigned int)(*THBCR) & 0xEF) == (str.pre_data & 0xEF)))
     || (((unsigned int)(*THBCR)["HLDCTE"] == 0) && ((unsigned int)(*THBCR)["HLDTE"] == 0) && ((unsigned int)(*THBCR)["SGS"] == (str.pre_data&0x3))))) {
        CheckAccess(emAllSG,THBCR,str);
    }
}//}}}

void Csarad113x::cb_THER_TH0E(RegCBstr str)
{//{{{
    bool en_check = false;
    for (unsigned int i = 0; i<emAllTHCh; i++) {
        if (!en_check) {
            en_check = !(((((unsigned int)(*THER)>>i)&0x1) == 0) || (((str.pre_data>>i)&0x1) == (((unsigned int)(*THER)>>i)&0x1)));
        }
    }
    if (en_check) {
        if (CheckAccess(emAllSG,THER,str)) {
            if ((unsigned int)(*SGSTR)["SHACT"] != 0) {
                re_printf("warning","The physical channels setting should not be changed when T&H is operating.\n");
                (*THER) = str.pre_data;
            }
        }
    }
}//}}}

void Csarad113x::cb_THGSR_TH0GS(RegCBstr str)
{//{{{
    if (CheckAccess(emAllSG,THGSR,str)) {
        if ((unsigned int)(*SGSTR)["SHACT"] == 1) {
            re_printf("warning","The physical channels setting should not be changed when T&H is operating.\n");
            (*THGSR) = str.pre_data;
        }
    }
}//}}}

void Csarad113x::cb_SFTCR_OWEIE(RegCBstr str)
{//{{{
    CheckAccess(emAllSG,SFTCR,str);
}//}}}

void Csarad113x::cb_ULLMTBR_ULMTB(RegCBstr str)
{//{{{
    if (CheckAccess(emAllSG,ULLMTBR[str.channel],str)) {
        if (((unsigned int)(*ADCR)["CTYP"] == em10bit)
         && ((((unsigned int)(*(ULLMTBR[str.channel]))["ULMTB"] & 0x3) != emMinULMTB10bit) || (((unsigned int)(*(ULLMTBR[str.channel]))["LLMTB"] & 0x3) != emMinLLMTB10bit))) {
            re_printf("warning","When A/D conversion 10bit is selected (ADCR.CTYP = 1), %s.ULMTB[1:0] should be set to 2'b11 and %s.LLMTB[1:0] should be set to 2'b00.\n",(ULLMTBR[str.channel])->name().c_str(),(ULLMTBR[str.channel])->name().c_str());
            (*(ULLMTBR[str.channel])) = str.pre_data;
        }
        if ((unsigned int)(*(ULLMTBR[str.channel]))["ULMTB"] < (unsigned int)(*(ULLMTBR[str.channel]))["LLMTB"]) {
            re_printf("warning","The value of %s.ULMTB should be equal or greater than %s.LLMTB.\n",(ULLMTBR[str.channel])->name().c_str(),(ULLMTBR[str.channel])->name().c_str());
        }
    }
}//}}}

void Csarad113x::cb_ECR_ULEC(RegCBstr str)
{//{{{
    if ((unsigned int)(*ECR)["ULEC"] == 1) {
        (*ULER) = 0;
        mULEVal = false;
        mWriteULEInterruptEvent.notify(NextPCLKPosedge(0),SC_NS);
    }
    if ((unsigned int)(*ECR)["OWEC"] == 1) {
        (*OWER) = 0;
    }
    
    // Clear after write
    (*ECR)["ULEC"] = 0;
    (*ECR)["OWEC"] = 0;
}//}}}

void Csarad113x::cb_DGCTL0_PSEL0(RegCBstr str)
{//{{{
//    CheckAccess(emAllSGn,DGCTL0,str); // In chapter 31.5.1.1 of HWM.UM(v0.5)/chapter 1.3.1.5 of HWM (Japanese0 DGCTL0 can be written during operation
}//}}}

void Csarad113x::cb_DGCTL1_CDG00(RegCBstr str)
{//{{{
    CheckAccess(emAllSGn,DGCTL1,str);
}//}}}

void Csarad113x::cb_PDCTL1_PDNA00(RegCBstr str)
{//{{{
    CheckAccess(emAllSGn,PDCTL1,str);
}//}}}

void Csarad113x::cb_PDCTL2_PDNB00(RegCBstr str)
{//{{{
    CheckAccess(emAllSGn,PDCTL2,str);
}//}}}

void Csarad113x::cb_SMPCR_SMPT(RegCBstr str)
{//{{{
    if (CheckAccess(emAllSG,SMPCR,str)) {
        if (((unsigned int)(*SMPCR)["SMPT"]) < emMinSmpNum) {
            re_printf("warning", "Setting value to SMPCR.SMPT must be equal or greater than %d.\n", emMinSmpNum);
            (*SMPCR)["SMPT"] = str.pre_data&0xFF;
        }
    }
}//}}}

void Csarad113x::cb_TSNSMPCR_TSNSMPT(RegCBstr str)
{//{{{
    if (CheckAccess(emAllSG,TSNSMPCR,str)) {
        if (((unsigned int)(*(TSNSMPCR))["TSNSMPT"]) < emMinSmpNum) {
            re_printf("warning", "Setting value to TSNSMPCR.TSNSMPT must be equal or greater than %d.\n", emMinSmpNum);
            (*TSNSMPCR)["TSNSMPT"] = str.pre_data&0xFF;
        }
    }
}//}}}

void Csarad113x::cb_EMUCR_SVSDIS(RegCBstr str)
{//{{{
    CheckAccess(emAllSG,EMUCR,str);
}//}}}

void Csarad113x::cb_SGPRCR_SGPR0(RegCBstr str)
{//{{{
    if (mIsOperating) {
        re_printf("warning","SGPRCR can not be changed when SARAD113x is operating.\n");
        (*SGPRCR) = str.pre_data;
    }
    mPrioritySet = (unsigned int)(*SGPRCR);
}//}}}

void Csarad113x::cb_SGSTCR_SGSTn(RegCBstr str)
{//{{{
    if ((mPCLKPeriod > 0)&&(mADCLKPeriod > 0)) {
        if (((unsigned int)(*(SGSTCR[str.channel]))["SGSTn"] == 1) && ((((unsigned int)(*SGSTR)["SGACT"] >> str.channel)&0x1) == 0)) {
            SWTrigger(str.channel);
        }
    }
    // Clear after write
    (*(SGSTCR[str.channel])) = 0;
}//}}}

void Csarad113x::cb_TSNSGSTCR_TSNSGST(RegCBstr str)
{//{{{
    if ((mPCLKPeriod > 0)&&(mADCLKPeriod > 0)) {
        #ifdef SARAD113x_ENABLE_TSN
        if (((unsigned int)(*TSNSGSTCR)["TSNSGST"] == 1) && ((((unsigned int)(*SGSTR)["SGACT"] >> emTSNSG)&0x1) == 0)) {
            SWTrigger(emTSNSG);
        }
        #endif
    }
    // Clear after write
    (*TSNSGSTCR) = 0;
}//}}}

void Csarad113x::cb_PWDSGSTCR_PWDSGST(RegCBstr str)
{//{{{
    if ((mPCLKPeriod > 0)&&(mADCLKPeriod > 0)) {
        if (((unsigned int)(*PWDSGSTCR)["PWDSGST"] == 1) && ((((unsigned int)(*SGSTR)["SGACT"] >> emPWDSG)&0x1) == 0)) {
            SWTrigger(emPWDSG);
        }
    }
    // Clear after write
    (*PWDSGSTCR) = 0;
}//}}}

void Csarad113x::cb_SGCR_ADIE(RegCBstr str)
{//{{{
    unsigned int trgmd = (unsigned int)(*(SGCR[str.channel]))["TRGMD"];
    if (!mADCATCNVnVal[str.channel]&&(((unsigned int)(*(SGCR[str.channel])) & 0xFFFFFFFE) == (str.pre_data && 0xFFFFFFFE))&&(trgmd == 0)) { // Enable to clear TRGMD upon completion of AD conversion
    } else {
        (*(SGCR[str.channel]))["TRGMD"] = 0; // Temporary clear value of TRGMD
        if (CheckAccess(emAllSG,SGCR[str.channel],str)) {
            (*(SGCR[str.channel]))["TRGMD"] = trgmd; // Restore the value of TRGMD
        }
    }
}//}}}

void Csarad113x::cb_TSNSGCR_TSNTRGMD(RegCBstr str)
{//{{{
    unsigned int trgmd = (unsigned int)(*TSNSGCR)["TSNTRGMD"];
    if (!mADCATCNVnVal[emTSNSG]&&(((unsigned int)(*TSNSGCR) & 0xFFFFFFFE) == (str.pre_data && 0xFFFFFFFE))&&(trgmd == 0)) { // Enable to clear TSNTRGMD upon completion of AD conversion
    } else {
        (*TSNSGCR)["TSNTRGMD"] = 0; // Temporary clear value of TSNTRGMD
        if (CheckAccess(emAllSG,TSNSGCR,str)) {
            (*TSNSGCR)["TSNTRGMD"] = trgmd; // Restore the value of TSNTRGMD
        }
    }
}//}}}

void Csarad113x::cb_PWDSGCR_PWDTRGMD(RegCBstr str)
{//{{{
    unsigned int trgmd = (unsigned int)(*PWDSGCR)["PWDTRGMD"];
    if (!mADCATCNVnVal[emPWDSG]&&(((unsigned int)(*PWDSGCR) & 0xFFFFFFFE) == (str.pre_data && 0xFFFFFFFE))&&(trgmd == 0)) { // Enable to clear PWDTRGMD upon completion of AD conversion
    } else {
        (*PWDSGCR)["PWDTRGMD"] = 0; // Temporary clear value of PWDTRGMD
        if (CheckAccess(emAllSG,PWDSGCR,str)) {
            (*PWDSGCR)["PWDTRGMD"] = trgmd; // Restore the value of PWDTRGMD
        }
    }
}//}}}

void Csarad113x::cb_SGSEFCR_SEFCn(RegCBstr str)
{//{{{
    // Clear after write
    (*(SGSEFCR[str.channel])) = 0;
    ClearScanningEndFlag(str.channel);
}//}}}

void Csarad113x::cb_TSNSGSEFCR_TSNSEFC(RegCBstr str)
{//{{{
    // Clear after write
    (*TSNSGSEFCR) = 0;
    // Clear SGSTR.SEF of TSN
    ClearScanningEndFlag(emTSNSG);
}//}}}

void Csarad113x::cb_PWDSGSEFCR_PWDSEFC(RegCBstr str)
{//{{{
    // Clear after write
    (*PWDSGSEFCR) = 0;
    // Clear SGSTR.SEF of PWD
    ClearScanningEndFlag(emPWDSG);
}//}}}

void Csarad113x::cb_SGVCSP_VCSP(RegCBstr str)
{//{{{
    if (((unsigned int)(*(SGVCSP[str.channel]))["VCSP"]) > emMaxVirChannel-1) {
        re_printf("warning", "Setting value to %s.VCSP must be equal or less than %d.\n", SGVCSP[str.channel]->name().c_str(),emMaxVirChannel-1);
        (*(SGVCSP[str.channel]))["VCSP"] = str.pre_data&0x3F;
    }
    if ((unsigned int)(*SGSTR)["SHACT"] != 0) {
        re_printf("warning","The virtual channels setting should not be changed when T&H is operating.\n");
        (*(SGVCSP[str.channel])) = str.pre_data;
    } else {
        if ((((unsigned int)(*SGSTR)["SGACT"] >> str.channel)&0x1) != 0) {
            // Do not write when A/D conversion is process with HW trigger
            if (GetTRGMD(str.channel) != 0) {
                re_printf("warning","Register %s can not be accessed when A/D conversion is operating with HW trigger.\n",SGVCSP[str.channel]->name().c_str());
                (*(SGVCSP[str.channel])) = str.pre_data;
            } else {
                // Write to SGVCSP when AD/conversion with SW trigger --> check SGVCSP <= SGVCEP
                if ((unsigned int)(*(SGVCSP[str.channel])) > (unsigned int)(*(SGVCEP[str.channel]))) {
                    re_printf("warning", "Setting value to %s.VCSP must be equal or less than value of %s.VCEP.\n", SGVCSP[str.channel]->name().c_str(),SGVCEP[str.channel]->name().c_str());
                    (*(SGVCSP[str.channel])) = str.pre_data;
                }
            }
        } else {
            mFirstVC[str.channel] = (unsigned int)(*(SGVCSP[str.channel]))["VCSP"];
            mNextVC[str.channel] = (unsigned int)(*(SGVCSP[str.channel]))["VCSP"];
        }
    }
}//}}}

void Csarad113x::cb_SGVCEP_VCEP(RegCBstr str)
{//{{{
    if (((unsigned int)(*(SGVCEP[str.channel]))["VCEP"]) > (emMaxVirChannel-1)) {
        re_printf("warning", "Setting value to %s.VCEP must be equal or less than %d.\n", SGVCEP[str.channel]->name().c_str(),emMaxVirChannel-1);
        (*(SGVCEP[str.channel]))["VCEP"] = str.pre_data&0x3F;
    }
    if ((unsigned int)(*SGSTR)["SHACT"] != 0) {
        re_printf("warning","The virtual channels setting should not be changed when T&H is operating.\n");
        (*(SGVCEP[str.channel])) = str.pre_data;
    } else {
        // Write to SGVCEP when AD/conversion --> check SGVCSP <= SGVCEP
        if ((((unsigned int)(*SGSTR)["SGACT"] >> str.channel)&0x1) != 0) {
            if ((unsigned int)(*(SGVCSP[str.channel])) > (unsigned int)(*(SGVCEP[str.channel]))) {
                re_printf("warning", "Setting value to %s.VCEP must be equal or greater than value of %s.VCSP.\n", SGVCEP[str.channel]->name().c_str(),SGVCSP[str.channel]->name().c_str());
                (*(SGVCEP[str.channel])) = str.pre_data;
            }
        } else {
            mLastVC[str.channel] = (unsigned int)(*(SGVCEP[str.channel]))["VCEP"];
        }
    }
}//}}}

void Csarad113x::cb_SGMCYCR_MCYC(RegCBstr str)
{//{{{
    if (CheckAccess(emAllSGn,SGMCYCR[str.channel],str)) {
        if ((unsigned int)(*(SGMCYCR[str.channel]))["MCYC"] > emMaxMultiCyc) {
            re_printf("warning", "Setting value to %s.MCYC must be equal or less than %d.\n", SGMCYCR[str.channel]->name().c_str(),emMaxMultiCyc);
            (*(SGMCYCR[str.channel]))["MCYC"] = str.pre_data&0xFF;
        }
    }
}//}}}

void Csarad113x::cb_SGTSEL_TxSEL00(RegCBstr str)
{//{{{
    if (str.is_wr){
        if (CheckAccess(emAllSGn,SGTSEL[str.channel],str)) {
            WriteADOPControl(str.channel);
        }
    } else {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            // Read data is from port ADOP_OPAn_DATA (n=1~3)
            if (str.channel == emSG1) {
                (*(SGTSEL[str.channel])) = (unsigned int) ADOP_OPA1_DATA.read();
            } else if (str.channel == emSG2) {
                (*(SGTSEL[str.channel])) = (unsigned int) ADOP_OPA2_DATA.read();
            } else {
                (*(SGTSEL[str.channel])) = (unsigned int) ADOP_OPA3_DATA.read();
            }
        }
    }
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit SC_NS)
**********************************/
double Csarad113x::GetCurTime(void)
{//{{{
    double cur_time = sc_time_stamp().to_double();
    double ret = 0;
    if (sc_get_time_resolution() < sc_time(1, SC_PS)) {//SC_FS
        ret = cur_time/1000000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_NS)) {//SC_PS
        ret = cur_time/1000;
    }  else if (sc_get_time_resolution() < sc_time(1, SC_US)) {//SC_NS
        ret = cur_time;
    } else if (sc_get_time_resolution() < sc_time(1, SC_MS)) {//SC_US
        ret = cur_time * 1000;
    } else if (sc_get_time_resolution() < sc_time(1, SC_SEC)) {//SC_MS
        ret = cur_time * 1000000;
    } else {//SC_SEC
        ret = cur_time * 1000000000;
    }
    return ret;
}//}}}

// vim600: set foldmethod=marker :
