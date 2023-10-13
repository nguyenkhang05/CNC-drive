// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2012-2015 Renesas System Design Co., Ltd.
// Copyright(c) 2012-2015 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "adc.h"
#ifndef FRACTION_TO_INT
#define FRACTION_TO_INT(d) ((unsigned int)((d - (unsigned long long)(d))*1000000))
#endif

//Constructor of ADC class
Cadc::Cadc(sc_module_name name, unsigned int adc_instance):
         Cadc_regif((std::string)name, 32)

        //trigger input port
        ,ADmSG0TRG("ADmSG0TRG")
        ,ADmSG1TRG("ADmSG1TRG")
        ,ADmSG2TRG("ADmSG2TRG")
        ,ADmSG3TRG("ADmSG3TRG")
        ,ADmSG4TRG("ADmSG4TRG")
        //analog terminal input port
        ,ANm00("ANm00")
        ,ANm01("ANm01")
        ,ANm02("ANm02")
        ,ANm03("ANm03")
        ,ANm10("ANm10")
        ,ANm11("ANm11")
        ,ANm12("ANm12")
        ,ANm13("ANm13")
        ,ANm20("ANm20")
        ,ANm21("ANm21")
        ,ANm22("ANm22")
        ,ANm23("ANm23")
        ,ANm30("ANm30")
        ,ANm31("ANm31")
        ,ANm32("ANm32")
        ,ANm33("ANm33")
        ,ANm40("ANm40")
        ,ANm41("ANm41")
        ,ANm42("ANm42")
        ,ANm43("ANm43")
        /// SARAD4 add
        ,ANm50("ANm50")
        ,ANm51("ANm51")
        ,ANm52("ANm52")
        ,ANm53("ANm53")
        ,ANm60("ANm60")
        ,ANm61("ANm61")
        ,ANm62("ANm62")
        ,ANm63("ANm63")
        ,ANm70("ANm70")
        ,ANm71("ANm71")
        ,ANm72("ANm72")
        ,ANm73("ANm73")
        // RH850_E2x added
        ,IFVCHm("IFVCHm")
        ,IFRDm("IFRDm")
        ,IFREm("IFREm")
        //interrupt ouptut port
        ,INT_ADm0("INT_ADm0")
        ,INT_ADm1("INT_ADm1")
        ,INT_ADm2("INT_ADm2")
        ,INT_ADm3("INT_ADm3")
        ,INT_ADm4("INT_ADm4")
        ,INT_MPXm("INT_MPXm")
        ,INT_ADEm("INT_ADEm")
        ,ADPEm("ADPEm")
        //data output port
        ,ADENDm0("ADENDm0")
        ,ADENDm1("ADENDm1")
        ,ADENDm2("ADENDm2")
        ,ADENDm3("ADENDm3")
        ,ADENDm4("ADENDm4")
        // RH850_E2x added
        ,DFREQm("DFREQm")
        ,DFTAGm("DFTAGm")
        ,DFDATAm("DFDATAm")
        ,ADCm_IFDATA("ADCm_IFDATA")
        ,VCULMO("VCULMO")
        ,VCLLMO("VCLLMO")
        // RH850_E2x FCC1/FCC2 added
        ,ADMPX0("ADMPX0")
        ,ADMPX1("ADMPX1")
        ,ADMPX2("ADMPX2")
        ,ADMPX3("ADMPX3")
        ,ADMPX4("ADMPX4")
{//{{{
    //initial handleCommand
    CommandInit(this->name());
    Cadc_regif::set_instance_name(this->name());

    for (unsigned int index = 0; index < emMaxVirChannel; index++) {
        std::ostringstream str_vcend;
        str_vcend<<"vcend"<<index;

        VCEND[index] = new sc_out<bool>(str_vcend.str().c_str());
        VCEND[index]->initialize(0);
    }

    //instance TIMER3, TIMER4  modules
    mTIMER[0] = new Ctimer ("mTIMER3", 0);
    sc_assert(mTIMER[0] != NULL);
    mTIMER[1] = new Ctimer ("mTIMER4", 1);
    sc_assert(mTIMER[1] != NULL);

    //initial variable
    mADCInstance = adc_instance;
    mResetFlag = false;
    mIsReset = false;
    mAvrefh = 3.3;
    mAVcc = 3.3;
    mForcedIdError = false;
    mForcedParityError = false;
    mForcedOpenError = false;
    mForcedBreak = false;
    mtD = 0;
    mtED = 0;
    mExSmpt = 18;
    mtSAR = 0;
    mtTHLD = 10;
    mtTHSD = 5;
    mtTSPL = 27;
    mPCLKPeriod = 0;
    mADCLKPeriod = 0;
    mDumpInterrupt = false;
    mEnableConvertInfo = false;
    mEnableTimeCalculation = true;
    mTHzAssignChannel[0] = 1;   // Assign T&H0 to ANm01
    mTHzAssignChannel[1] = 2;   // Assign T&H1 to ANm02
    mTHzAssignChannel[2] = 3;   // Assign T&H2 to ANm03
    if (!IsTargetProductE2x()){
        mTHzAssignChannel[3] = 29;  // Assign T&H3 to ANm71
        mTHzAssignChannel[4] = 30;  // Assign T&H4 to ANm72
        mTHzAssignChannel[5] = 31;  // Assign T&H5 to ANm73
    }
    /// SARAD3
    mAVccMax = 3.6;
    mAVccMin = 3;
    mAvrefhMin = 3;
    /// SARAD4 add
    mADCNum = 4;
    mPhyChannelNum = 20;
    mVirChannelNum = 0xFFFF;
    mMPXInput = 0;

    //// Added P1M-E and CC-Cube D1D2
    mTargetProduct = "";
    mTHChannelNum = emTHchannel;

    GlobalInit();
    mADCState = emADCIdleState;
    //initial output port
    INT_ADm0.initialize(0);
    INT_ADm1.initialize(0);
    INT_ADm2.initialize(0);
    INT_ADm3.initialize(0);
    INT_ADm4.initialize(0);
    INT_MPXm.initialize(0);
    INT_ADEm.initialize(0);
    ADPEm.initialize(0);
    ADENDm0.initialize(0);
    ADENDm1.initialize(0);
    ADENDm2.initialize(0);
    ADENDm3.initialize(0);
    ADENDm4.initialize(0);
    DFREQm.initialize(0);
    DFTAGm.initialize(0);
    DFDATAm.initialize(0);
    ADCm_IFDATA.initialize(0);
    VCULMO.initialize(0);
    VCLLMO.initialize(0);

    ADMPX0.initialize(0);
    ADMPX1.initialize(0);
    ADMPX2.initialize(0);
    ADMPX3.initialize(0);
    ADMPX4.initialize(0);

    //scanning method
    SC_THREAD(ScanningThread);
    dont_initialize();
    sensitive << mScanningEvent;
    sensitive << mResetEvent;

    SC_THREAD(BoundaryExcessThread);
    dont_initialize();
    sensitive << mBoundaryExcessEvent;

    SC_THREAD(VCENDNoticeThread);
    dont_initialize();
    sensitive << mVCENDNoticeEvent;

    //ADC state method
    SC_METHOD(ADCStateMethod);
    dont_initialize();
    sensitive << mADCStateEvent;

    //T&H circuit method
    SC_METHOD(THCircuitMethod);
    dont_initialize();
    sensitive << mTHStopEvent;
    sensitive << mTHSamplingEvent;
    sensitive << mTHEnableEvent;
    sensitive << mTHASoftwareTriggerHoldingEvent;
    sensitive << mTHBSoftwareTriggerHoldingEvent;
    sensitive << mTHAPortTriggerHoldingEvent;
    sensitive << mTHBPortTriggerHoldingEvent;
    sensitive << mTHUpdateChannelStateEvent;
    sensitive << mTHUpdateInputEvent;

    SC_METHOD(THAHoldingEndMethod);
    dont_initialize();
    sensitive << mTHGAHoldingEvent;

    SC_METHOD(THBHoldingEndMethod);
    dont_initialize();
    sensitive << mTHGBHoldingEvent;

    for (unsigned int i = 0; i < emMaxPhyChannel; i++) {
        sc_core::sc_spawn_options otp;
        otp.spawn_method();
        switch (i){
            case 0:
                otp.set_sensitivity(&ANm00);
                break;
            case 1:
                otp.set_sensitivity(&ANm01);
                break;
            case 2:
                otp.set_sensitivity(&ANm02);
                break;
            case 3:
                otp.set_sensitivity(&ANm03);
                break;
            case 4:
                otp.set_sensitivity(&ANm10);
                break;
            case 5:
                otp.set_sensitivity(&ANm11);
                break;
            case 6:
                otp.set_sensitivity(&ANm12);
                break;
            case 7:
                otp.set_sensitivity(&ANm13);
                break;
            case 8:
                otp.set_sensitivity(&ANm20);
                break;
            case 9:
                otp.set_sensitivity(&ANm21);
                break;
            case 10:
                otp.set_sensitivity(&ANm22);
                break;
            case 11:
                otp.set_sensitivity(&ANm23);
                break;
            case 12:
                otp.set_sensitivity(&ANm30);
                break;
            case 13:
                otp.set_sensitivity(&ANm31);
                break;
            case 14:
                otp.set_sensitivity(&ANm32);
                break;
            case 15:
                otp.set_sensitivity(&ANm33);
                break;
            case 16:
                otp.set_sensitivity(&ANm40);
                break;
            case 17:
                otp.set_sensitivity(&ANm41);
                break;
            case 18:
                otp.set_sensitivity(&ANm42);
                break;
            case 19:
                otp.set_sensitivity(&ANm43);
                break;
            /// SARAD4 add
            case 20:
                otp.set_sensitivity(&ANm50);
                break;
            case 21:
                otp.set_sensitivity(&ANm51);
                break;
            case 22:
                otp.set_sensitivity(&ANm52);
                break;
            case 23:
                otp.set_sensitivity(&ANm53);
                break;
            case 24:
                otp.set_sensitivity(&ANm60);
                break;
            case 25:
                otp.set_sensitivity(&ANm61);
                break;
            case 26:
                otp.set_sensitivity(&ANm62);
                break;
            case 27:
                otp.set_sensitivity(&ANm63);
                break;
            case 28:
                otp.set_sensitivity(&ANm70);
                break;
            case 29:
                otp.set_sensitivity(&ANm71);
                break;
            case 30:
                otp.set_sensitivity(&ANm72);
                break;
            case 31:
                otp.set_sensitivity(&ANm73);
                break;
            default:
                break;
        }
        sc_core::sc_spawn(sc_bind(&Cadc::THzMethod, this, i), sc_core::sc_gen_unique_name("THz_Method"), &otp);
    }

    //scanning software trigger method
    SC_METHOD(ScanGroup0SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup0];
    sensitive << mWritingSGSTCREvent[emGroup0];

    SC_METHOD(ScanGroup1SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup1];
    sensitive << mWritingSGSTCREvent[emGroup1];

    SC_METHOD(ScanGroup2SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup2];
    sensitive << mWritingSGSTCREvent[emGroup2];

    SC_METHOD(ScanGroup3SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup3];
    sensitive << mWritingSGSTCREvent[emGroup3];

    SC_METHOD(ScanGroup4SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup4];
    sensitive << mWritingSGSTCREvent[emGroup4];

    //counting end method
    SC_METHOD(ScanGroup3ADtimerTriggerMethod);
    dont_initialize();
    sensitive << mTIMER[0]->mADtimerEndCountingEvent[0];

    SC_METHOD(ScanGroup4ADtimerTriggerMethod);
    dont_initialize();
    sensitive << mTIMER[1]->mADtimerEndCountingEvent[1];

    //interrupt output method
    SC_METHOD(INT_ADmxINT_ADEmDataMethod);
    dont_initialize();
    sensitive << mINT_ADmxINT_ADEmDataEvent;
    sensitive << mResetEvent;

    SC_METHOD(INT_MPXmInterruptMethod);
    dont_initialize();
    sensitive << mINT_MPXmInterruptEvent;
    sensitive << mResetEvent;

    //add for RH850-E2x FCC1/FCC2
    SC_METHOD(ADMPXOutputMethod);
    dont_initialize();
    sensitive << mADMPXOutputEvent;
    sensitive << mResetEvent;

    SC_METHOD(ADPEmInterruptMethod);
    dont_initialize();
    sensitive << mADPEmInterruptEvent;
    sensitive << mResetEvent;

    SC_METHOD(ADENDm0Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup0];
    sensitive << mResetEvent;

    SC_METHOD(ADENDm1Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup1];
    sensitive << mResetEvent;

    SC_METHOD(ADENDm2Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup2];
    sensitive << mResetEvent;

    SC_METHOD(ADENDm3Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup3];
    sensitive << mResetEvent;

    SC_METHOD(ADENDm4Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup4];
    sensitive << mResetEvent;

    //external input port method
    SC_METHOD(ADmSG0TRGMethod);
    dont_initialize();
    sensitive << ADmSG0TRG;

    SC_METHOD(ADmSG1TRGMethod);
    dont_initialize();
    sensitive << ADmSG1TRG;

    SC_METHOD(ADmSG2TRGMethod);
    dont_initialize();
    sensitive << ADmSG2TRG;

    SC_METHOD(ADmSG3TRGMethod);
    dont_initialize();
    sensitive << ADmSG3TRG;

    SC_METHOD(ADmSG4TRGMethod);
    dont_initialize();
    sensitive << ADmSG4TRG;

    // RH850_E2x added
    SC_METHOD(IFCProcessMethod);
    dont_initialize();
    sensitive << IFRDm;
    sensitive << IFREm;

    /// SARAD3
    SC_METHOD(HWTriggerScanningMethod);
    dont_initialize();
    sensitive << mHWTriggerEvent[emGroup0];
    sensitive << mHWTriggerEvent[emGroup1];
    sensitive << mHWTriggerEvent[emGroup2];
    sensitive << mHWTriggerEvent[emGroup3];
    sensitive << mHWTriggerEvent[emGroup4];

//    SC_METHOD(VoltageMonitorMethod);
//    dont_initialize();
//    sensitive << mVoltageMonitorEvent;

}//}}}

//destructor of Cadc class
Cadc::~Cadc()
{//{{{
    delete mTIMER[0];
    delete mTIMER[1];
}//}}}
//virtual channel end notice thread
void Cadc::VCENDNoticeThread(void)
{//{{{
    while(true){
        VCEND[mCurVC]->write(1);
        wait(sc_time(mPCLKPeriod, SC_NS));

        VCEND[mCurVC]->write(0);
        mCurVC = 0;
        wait();
    }
}//}}}

//boundary excess thread
void Cadc::BoundaryExcessThread(void)
{//{{{
    while(true){
        if (mConvtData > mULMTB) {
            VCULMO.write(1);
        }
        if (mConvtData < mLLMTB) {
            VCLLMO.write(1);
        }
        wait(sc_time(mPCLKPeriod, SC_NS));

        VCULMO.write(0);
        VCLLMO.write(0);
        mConvtData = 0;
        wait();
    }
}//}}}

//scanning method
void Cadc::ScanningThread(void)
{//{{{
    for(;;){
        if(mIsReset || (mADCInstance >= mADCNum)){
            //1.Reset is active
            mResetFlag = false;
            wait();
            continue;
        }
        try{
            while(true){
                bool is_active = false;
                for (unsigned int i =0;i<emGroupNum;i++){
                    is_active |= mSGActive[i];
                }
                while(is_active){
                    mADCStateEvent.notify();
                    unsigned int group_number = 0;
                    for(unsigned int i = 1; i < (emGroupNum + 1); i++){
                        group_number = emGroupNum - i;
                        if(mIsScanningProcess[group_number]) {
                            re_printf("info","The group %d starts the scanning process.\n", group_number);
                            //1.Scanning group operation information
                            mScanningState = true;
                            mCurrentGroup  = (unsigned int)(group_number);

                            //2.Get setting mode of current group
                            unsigned int group_loop = 0;
                            bool is_multiple_mode = true;
                            if((*SGCR[group_number])["SCANMD"] == 1){
                                is_multiple_mode = false;
                            }else if(mGroupSuspendState[group_number]){                      //start at the next scanning sequence for suspend group
                                group_loop = mLastMultipleNumber[group_number];
                            }else{
                                group_loop = (unsigned int)((*SGMCYCR[group_number])) + 1;    //start at begining of scanning sequence
                            }

                            //3.Scanning for group
                            bool group_active_flag = true;
                            bool first = true;
                            while(group_active_flag){
                                bool is_final_vc           = false;
                                bool is_suspend_immediate  = false;
                                unsigned int start_vc = 0;
                                unsigned int end_vc = mEndVCPointer[group_number];

                                //3.1.Get the start
                                if(mGroupSuspendState[group_number]){
                                    mGroupSuspendState[group_number] = false;
                                    start_vc = mSuspendVC[group_number];
                                }else{
                                    start_vc = mStartVCPointer[group_number];
                                }

                                //3.2.Scanning for VC
                                for(unsigned int vc_number = start_vc; vc_number < (end_vc + 1); vc_number++){
                                    if(VCCheck(vc_number)){
                                        double start_time = GetCurTime();
                                        unsigned int vc_add_number = VCAddition(vc_number);
                                        unsigned int data[4] = {0,0,0,0};
                                        unsigned int cnvcls = ((*VCR[vc_number])["CNVCLS"]);
                                        unsigned int Last_DFMT = (*ADCR2)["DFMT"]; // Add to check re-convert when change format
                                        unsigned int Last_CNVCLS = ((*VCR[vc_number])["CNVCLS"]);// Add to check re-convert when change format
                                        unsigned int Last_ADDNT = (*ADCR2)["ADDNT"]; // Add to check re-convert when change format
                                        mFormat_temp[vc_number] = Last_DFMT<<8|Last_CNVCLS<<4|Last_ADDNT;// Add to check re-convert when change format

                                        // RH850_E2x added wait insertion for virtual channel (not start virtual channel of scanning group)
                                        if (IsTargetProductE2x()) {
                                            if ((cnvcls != emDiagnosisMode) && (!first)) {
                                                double vc_wait_time = ((unsigned int)(*SGVCOWR[group_number])["VCOW"])*4*mADCLKPeriod;
                                                wait((unsigned int)vc_wait_time, SC_NS, mSuspendEvent | mDisableEvent | mResetEvent | mZeroClockEvent);
                                            }
                                            first = false;
                                            //3.2.1.Check the reset function
                                            if(mResetFlag){
                                                throw vpcl::emRstException;
                                            }
                                            /// check 0-frequency
                                            if ((mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                                break; //break out of for(vc_add ...) --> Stop scanning process
                                            }
                                            //3.2.2.Check the stop function
                                            if(mDisableFlag){
                                                ExportData(emCorruptConvert, (unsigned int)(group_number), vc_number, data, group_loop); //clear the  ADENDmx
                                                mStoreData = 0;
                                                break; //break out of for(vc_add ...) --> Stop scanning process
                                            }
                                            //3.2.3.Check the suspend function
                                            if(mSuspendFlag){
                                                mGroupSuspendState[group_number] = true;
                                                mLastMultipleNumber[group_number] = group_loop;
                                                mSuspendVC[group_number] = vc_number + 1;
                                                if(SuspendCheck(group_number)){
                                                    re_printf("info","The current scanning process of group %d is suspended in asynchronous mode.\n", group_number);
                                                    is_suspend_immediate = true;
                                                    mSuspendVC[group_number]  = vc_number;
                                                    mLastVC = vc_number;
                                                    mLastGroup = (unsigned int)(group_number);
                                                    ExportData(emCorruptConvert, (unsigned int)(group_number), vc_number, data, group_loop);                 //clear the ADENDmx
                                                    mStoreData = 0;
                                                    break; //break out of for(vc_add ...)
                                                }
                                            }
                                        }
                                        // RH850_E2x Enhancement
                                        unsigned int gctrl_val  = (unsigned int)((*VCR[vc_number])["GCTRL"]);
                                        unsigned int index_val  = gctrl_val & 0x1F;
                                        if (!(((mVMONVDCRVal == 2) || (mVMONVDCRVal == 1))
                                                    && ((index_val == emVCCPortNum) || (index_val == emEVCCPortNum) || (index_val == emVDDPortNum)))) {

                                            re_printf("info","The VC %d of group %d starts the conversion.\n", vc_number, group_number);
                                            //3.2.0.Assert INT_MPXm, ADENDmx, MPXCURR
                                            ExportData(emStartConvert, (unsigned int)(group_number), vc_number, data, group_loop);

                                            for(unsigned int vc_add = 0; vc_add < vc_add_number; vc_add++){
                                                double delay_time = ADCTimeCalculation(vc_number, vc_add);
                                                /// SARAD4 modify - fix 0-frequency issue
                                                if (!mTHAHoldingProcess && !mTHBHoldingProcess) {
                                                    wait((unsigned int)delay_time, SC_NS, mSuspendEvent | mDisableEvent | mResetEvent | mZeroClockEvent);
                                                } else {
                                                    mSuspendFlag = true;
                                                }
                                                //3.2.1.Check the reset function
                                                if(mResetFlag){
                                                    throw vpcl::emRstException;
                                                }
                                                /// SARAD4 add - fix 0-frequency issue
                                                if ((mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                                    break; //break out of for(vc_add ...) --> Stop scanning process
                                                }
                                                //3.2.2.Check the stop function
                                                if(mDisableFlag){
                                                    ExportData(emCorruptConvert, (unsigned int)(group_number), vc_number, data, group_loop);                 //clear the  ADENDmx
                                                    mStoreData = 0;
                                                    break; //break out of for(vc_add ...) --> Stop scanning process
                                                }
                                                //3.2.3.Check the suspend function
                                                if(mSuspendFlag){
                                                    mGroupSuspendState[group_number] = true;
                                                    mLastMultipleNumber[group_number] = group_loop;
                                                    mSuspendVC[group_number] = vc_number + 1;
                                                    if(SuspendCheck(group_number)){
                                                        re_printf("info","The current scanning process of group %d is suspended in asynchronous mode.\n", group_number);
                                                        is_suspend_immediate = true;
                                                        mSuspendVC[group_number]  = vc_number;
                                                        mLastVC = vc_number;
                                                        mLastGroup = (unsigned int)(group_number);
                                                        ExportData(emCorruptConvert, (unsigned int)(group_number), vc_number, data, group_loop);                 //clear the ADENDmx
                                                        mStoreData = 0;
                                                        break; //break out of for(vc_add ...)
                                                    }
                                                }
                                                //3.2.4.Get the data based on VC setting
                                                data[vc_add] = GetData((unsigned int)(group_number), vc_number, vc_add);

                                                //3.2.5.Wait the conversion time
                                                if (mEnableTimeCalculation) {
                                                    if (mTargetProduct == "RH850_P1M-E") {
                                                        unsigned int smpcr15_0 = (unsigned int)(*SMPCR) & 0xFFFF;
                                                        double sampling_time = (double)(mExSmpt * mADCLKPeriod);
                                                        if (smpcr15_0 == 0x0000) {
                                                            delay_time = 1000; // 1 micro sec
                                                        } else {
                                                            delay_time = 11300; // 11.3 micro sec
                                                        }
                                                        delay_time = delay_time - sampling_time; // conversion by successive approximation
                                                    } else {
                                                        delay_time = emtSARCoeff * mADCLKPeriod;
                                                    }
                                                } else {
                                                    delay_time = mtSAR;
                                                }
                                                /// SARAD4 modify - fix 0-frequency issue
                                                wait((unsigned int)delay_time, SC_NS, mSuspendEvent | mDisableEvent | mResetEvent | mZeroClockEvent);

                                                //3.2.6.Check the reset function
                                                if(mResetFlag){
                                                    throw vpcl::emRstException;
                                                }
                                                /// SARAD4 add - fix 0-frequency issue
                                                if ((mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                                    break; //break out of for(vc_add ...) --> Stop scanning process
                                                }
                                                //3.2.7.Check the stop function
                                                if(mDisableFlag){
                                                    ExportData(emCorruptConvert, (unsigned int)(group_number), vc_number, data, group_loop);                 //clear the  ADENDmx
                                                    mStoreData = 0;
                                                    break; //break out of for(vc_add ...) --> Stop scanning process
                                                }
                                                //3.2.8.Check the suspend function
                                                if(mSuspendFlag){
                                                    mGroupSuspendState[group_number] = true;
                                                    mLastMultipleNumber[group_number] = group_loop;
                                                    mSuspendVC[group_number] = vc_number + 1;
                                                    if(SuspendCheck(group_number)){
                                                        re_printf("info","The current scanning process of group %d is suspended in asynchronous mode.\n", group_number);
                                                        is_suspend_immediate = true;
                                                        mSuspendVC[group_number]  = vc_number;
                                                        mLastVC = vc_number;
                                                        mLastGroup = (unsigned int)(group_number);
                                                        ExportData(emCorruptConvert, (unsigned int)(group_number), vc_number, data, group_loop);                 //clear the ADENDmx
                                                        mStoreData = 0;
                                                        break; //break out of for(vc_add ...)
                                                    }
                                                }
                                                //3.2.9.Export data after conversion if there is not stop function or asynchronous suspend function
                                                if(vc_add == vc_add_number - 1){
                                                    re_printf("info","The VC %d of group %d finishes the conversion.\n", vc_number, group_number);
                                                    if(end_vc == vc_number){
                                                        is_final_vc = true;
                                                        if(mSuspendFlag){
                                                            mSuspendVC[group_number] = start_vc;
                                                        }
                                                        re_printf("info","The group %d finishes the conversion for all VCs.\n", group_number);
                                                    }
                                                    if(mSuspendFlag && ((!is_multiple_mode) || (end_vc != vc_number) || (group_loop > 1))){
                                                        re_printf("info","The current scanning process of group %d is suspended in synchronous mode.\n", group_number);
                                                    }
                                                    double end_time = GetCurTime();
                                                    mLastVC = vc_number;
                                                    mLastGroup = (unsigned int)(group_number);
                                                    ExportData(emFinishConvert, (unsigned int)(group_number), vc_number, data, group_loop);
                                                    DumpADCActivity(start_time, end_time);
                                                }
                                            }//end for VC addition number
                                        }
                                    }else{
                                        re_printf("warning","The current VC %d is bypassed.\n", vc_number);
                                    }
                                    /// SARAD4 modify - fix 0-frequency issue
                                    if(mDisableFlag || mSuspendFlag || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                        break; //break out of for(vc_number ...)
                                    }
                                }//end for(unsigned int vc_num = start_vc)

                                //3.3.Check to continue the current group or not
                                /// SARAD4 modify - fix 0-frequency issue
                                if(mDisableFlag || is_suspend_immediate || (mSuspendFlag && (!is_final_vc)) || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                    //stop scanning process/ Suspend asynchronous/ Suspend sysnchonous not at the final VC
                                    break; //break out of while(group_active_flag)
                                }else{
                                    if(is_multiple_mode){
                                        group_loop = group_loop - 1;
                                        mLastMultipleNumber[group_number] = group_loop;
                                        if(group_loop == 0){
                                            mGroupSuspendState[group_number] = false;
                                            (*SGSR[group_number])["SGACT"] = 0;
                                            mIsScanningProcess[group_number] = false;
                                            mSGActive[group_number] = false;
                                            re_printf("info","The group %d finishes the scanning process.\n", group_number);
                                            group_active_flag = false;
                                        }else if(!mSuspendFlag) {
                                            re_printf("info","The group %d is continued the scanning process at the start VC in multiple scanning mode.\n", group_number);
                                            if ((mTargetProduct != "RH850_E2x_fcc1") && (mTargetProduct != "RH850_E2x")) {
                                                if (mIsUpdateSGVCPR[group_number]) {
                                                    mIsUpdateSGVCPR[group_number] = false;
                                                    mStartVCPointer[group_number] = (unsigned int)(*SGVCPR[group_number])["VCSP"];
                                                    mEndVCPointer[group_number] = (unsigned int)(*SGVCPR[group_number])["VCEP"];
                                                }
                                            }
                                        }
                                    }else if(!mSuspendFlag) {
                                        re_printf("info","The group %d is continued the scanning process at the start VC in continuous scanning mode.\n", group_number);
                                        if ((mTargetProduct != "RH850_E2x_fcc1") && (mTargetProduct != "RH850_E2x")) {
                                            if (mIsUpdateSGVCPR[group_number]) {
                                                mIsUpdateSGVCPR[group_number] = false;
                                                mStartVCPointer[group_number] = (unsigned int)(*SGVCPR[group_number])["VCSP"];
                                                mEndVCPointer[group_number] = (unsigned int)(*SGVCPR[group_number])["VCEP"];
                                            }
                                        }
                                    }
                                    if(mSuspendFlag){     //suspend synchronous at the final VC
                                        break; //break out of while(group_active_flag)
                                    }
                                }
                            }//end while(group_active_flag)
                        }
                        /// SARAD4 modify - fix 0-frequency issue
                        if(mSuspendFlag || mDisableFlag || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                            mSuspendFlag = false;
                            break; //break out of for(unsigned int group_number.. )
                        }
                    }//end for(unsigned int group_number.. )
                    /// SARAD4 add - fix 0-frequency issue
                    if ((mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                        break;  //break out of scanning process --> while (is_active)
                    }
                    is_active = false;
                    if(mDisableFlag){
                        for(unsigned int group = 0; (group < emGroupNum); group++){
                            if (mDisableSGxFlag[group]){
                                mSGActive[group] = false;
                                (*SGSR[group])["SGACT"] = 0;
                                mIsScanningProcess[group] = false;
                                mGroupSuspendState[group] = false;
                                mDisableSGxFlag[group] = false;
                            }
                            is_active |= mSGActive[group];
                        }
                        mDisableFlag = false;
                        if (!is_active) {
                            break;  //break out of scanning process --> while (mG0Active)
                        }
                    }
                    for (unsigned int i =0;i<emGroupNum;i++) {
                        is_active |= mSGActive[i];
                    }
                    if (mTHAHoldingProcess || mTHBHoldingProcess) {
                        break; // break out of scanning process because hold trigger during scanning process
                    }
                }//end while(is_active)
                mScanningState = false;
                mLastVC = mVirChannelNum;
                mADCStateEvent.notify();
                wait();
                if(mResetFlag){
                    throw vpcl::emRstException;
                }
            }
        }
        catch(vpcl::eResetException){
        }
    }
}//}}}

//ADC state method
void Cadc::ADCStateMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if((!mIsReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        bool scanning_circuit_state = false;
        bool th_circuit_state = false;
        mADCChangeState = false;
        for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
            if(((*SGSR[group_number])["ADTACT"] == 1) || (mIsScanningProcess[group_number])){
                scanning_circuit_state = true;
            }
        }
        for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
            if(mTHChannelState[TH_channel] != emTHIdleState){
                th_circuit_state = true;
            }
        }
        switch (mADCState){
            case emADCIdleState :
                if(th_circuit_state){
                    re_printf("info","The ADC%d moves from IDLE state to TRACKING state.\n", mADCInstance);
                    mADCState = emADCTrackingState;
                }else if(scanning_circuit_state){
                    re_printf("info","The ADC%d moves from IDLE state to SCANNING state.\n", mADCInstance);
                    mADCState = emADCScanningState;
                }
                break;
            case emADCTrackingState :
                if(scanning_circuit_state && th_circuit_state){
                    re_printf("info","The ADC%d moves from TRACKING state to TRACKING&SCANNING state.\n", mADCInstance);
                    mADCState = emADCTrackingScanningState;
                }else if((!scanning_circuit_state) && (!th_circuit_state)){
                    re_printf("info","The ADC%d moves from TRACKING state to IDLE state.\n", mADCInstance);
                    mADCState = emADCIdleState;
                }
                break;
            case emADCTrackingScanningState :
                if((!scanning_circuit_state) && th_circuit_state){
                    re_printf("info","The ADC%d moves from TRACKING&SCANNING state to TRACKING state.\n", mADCInstance);
                    mADCState = emADCTrackingState;
                }else if(scanning_circuit_state && (!th_circuit_state)){
                    re_printf("info","The ADC%d moves from TRACKING&SCANNING state to SCANNING state.\n", mADCInstance);
                    mADCState = emADCScanningState;
                }
                break;
            case emADCScanningState:
                if(scanning_circuit_state && th_circuit_state){
                    re_printf("info","The ADC%d moves from SCANNING state to TRACKING&SCANNING state.\n", mADCInstance);
                    mADCState = emADCTrackingScanningState;
                }else if((!scanning_circuit_state) && (!th_circuit_state)){
                    re_printf("info","The ADC%d moves from SCANNING state to IDLE state.\n", mADCInstance);
                    mADCState = emADCIdleState;
                }
                break;
            default:
                break;
        }
        mADCChangeState = th_circuit_state || scanning_circuit_state;
        mADCChangeStateEvent.notify();
    }
}//}}}

//T&H circuit method
void Cadc::THCircuitMethod(void)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)){
        unsigned int ther = (unsigned int)(*THER);
        unsigned int thgsr = (unsigned int)(*THGSR);
        //0.Write 1 into THSTPCR to stop T&H circuit
        if(mTHStop){
            re_printf("info","T&H circuit is stop.\n");
            mTHStop = false;
            mTHGAHoldingEvent.cancel();
            mTHGBHoldingEvent.cancel();
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                mTHChannelState[TH_channel] = emTHIdleState;
            }
        }
        //1.Write into THER to enable/disable channel
        if(mTHEnable){
            mTHEnable = false;
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if(((ther >> TH_channel) & 0x1 ) == 0){
                    re_printf("info","The TH channel %d is disable.\n", TH_channel);
                    mTHChannelState[TH_channel] = emTHIdleState;
                }
            }

            bool is_stop_holding_groupa = true;
            bool is_stop_holding_groupb = true;
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if((((thgsr>>(2*TH_channel)) & 0x1) == 0) && (mTHChannelState[TH_channel] == emTHHoldingState)){
                    is_stop_holding_groupa = false;
                }
                if((((thgsr>>(2*TH_channel)) & 0x1) == 1) && (mTHChannelState[TH_channel] == emTHHoldingState)){
                    is_stop_holding_groupb = false;
                }
            }

            if(is_stop_holding_groupa){
                mTHGAHoldingEvent.cancel();
            }
            if(is_stop_holding_groupb){
                mTHGBHoldingEvent.cancel();
            }
        }
        //2.Write 1 into THSMPSTCR to sample all T&H channel
        if(mTHSamplingManual && (TimingCheck() && VoltageCheck())){
            re_printf("info","All T&H channels are sampled.\n");
            mTHSamplingManual = false;
            mTHGAHoldingEvent.cancel();
            mTHGBHoldingEvent.cancel();
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if(((ther >> TH_channel) & 0x1 ) == 1){
                    mTHChannelState[TH_channel] = emTHSamplingState;
                    double sampling_start_time = GetCurTime();
                    mTHSamplingStartTime[TH_channel] = sampling_start_time;
                    mIntTHSample[TH_channel] = LoadAnalogTerminal(mTHzAssignChannel[TH_channel]);
                }
            }
            //3. Update the new value from analog terminal when TH channel is in sampling state
        }
        if(mTHUpdateInput[0] || mTHUpdateInput[1] || mTHUpdateInput[2] || mTHUpdateInput[3] || mTHUpdateInput[4] || mTHUpdateInput[5]){
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if(mTHUpdateInput[TH_channel]){
                    double sampling_start_time = GetCurTime();
                    mTHSamplingStartTime[TH_channel] = sampling_start_time;
                    mIntTHSample[TH_channel] = LoadAnalogTerminal(mTHzAssignChannel[TH_channel]);
                    mTHUpdateInput[TH_channel] = false;
                }
            }
        }
        //4.Update channel state when finishing scanning process for TH channel
        if((mTHUpdateChannelState[0] || mTHUpdateChannelState[1] || mTHUpdateChannelState[2] || mTHUpdateChannelState[3] ||
                    mTHUpdateChannelState[4] || mTHUpdateChannelState[5]) && mTHUpdateChannelStateFlag){
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if(mTHUpdateChannelState[TH_channel] && ((*THCR)["ASMPMSK"] == 0)){
                    re_printf("info","The TH channel %d is sampled automatically.\n", TH_channel);
                    if ((((*THACR)["HLDCTE"] == 1) || ((*THBCR)["HLDCTE"] == 1)) && ((*ADCR1)["SUSMTD"] != 2)) {
                        re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE or THBCR.HLDCTE is 1 while ADCR1.SUSMTD is not 2.\n");
                    } else {
                        mTHChannelState[TH_channel] = emTHSamplingState;
                        double sampling_start_time = GetCurTime();
                        mTHSamplingStartTime[TH_channel] = sampling_start_time;
                        mIntTHSample[TH_channel] = LoadAnalogTerminal(mTHzAssignChannel[TH_channel]);
                    }
                }
                mTHUpdateChannelState[TH_channel] = false;
            }
            mTHUpdateChannelStateFlag = false;
        }
        //5.Trigger holding porcess by software(write 1 into THAHLDSTCR or THBHLDSTCR) or by input port ADmSGx
        if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding || mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding){
            if((*THACR)["SGS"] == (*THBCR)["SGS"]){
                re_printf("warning","Holding process in T&H circuit is not triggered because Group A and group B are same.\n");
            }else{
                unsigned int sgs_groupa = (unsigned int)((*THACR)["SGS"]);
                unsigned int sgs_groupb = (unsigned int)((*THBCR)["SGS"]);
                double holding_start_time = GetCurTime();
                double holding_delay  = 0;
                if (mEnableTimeCalculation) {
                    holding_delay = (emtTHSDCoeff + emtTHLDCoeff) * mADCLKPeriod; // Time from detecting hold trigger -> start scanning
                } else {
                    holding_delay = mtTHSD + mtTHLD; // Time from detecting hold trigger -> start scanning
                }
                //trigger holding for group A
                if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding){
                    mTHASoftwareTriggerHolding = false;
                    mTHAPortTriggerHolding = false;
                    bool holding_enable = false;
                    for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                        if(((thgsr>>(2*TH_channel)) & 0x1) == 0 ){
                            if(mTHChannelState[TH_channel] != emTHSamplingState){
                                re_printf("warning","T&H channel %d is not triggered holding process because this channel is not in sampling state.\n", TH_channel);
                            }else{
                                if((holding_start_time - mTHSamplingStartTime[TH_channel]) < (mtTSPL*mADCLKPeriod)){
                                    re_printf("warning","T&H channel %d is triggered holding process when sampling process is not finished.\n", TH_channel);
                                }
                                mTHChannelState[TH_channel] = emTHHoldingState;
                                holding_enable = true;
                            }
                        }
                    }
                    if(holding_enable){
                        re_printf("info","The holding process of group A is started.\n");
                        mTHAHoldingProcess = true;
                        mGroupANumber =  sgs_groupa + 1;
                        (*SGSR[mGroupANumber])["SGACT"] = 1;
                        //Suspend all scanning process
                        if (mScanningState) {
                            mSuspendFlag = true;
                            mSuspendEvent.notify();
                        }
                        mTHGAHoldingEvent.notify((unsigned int)holding_delay,SC_NS);
                    }
                }//end for if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding)
                //trigger holding for group B
                if(mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding){
                    mTHBSoftwareTriggerHolding = false;
                    mTHBPortTriggerHolding = false;
                    bool holding_enable = false;
                    for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                        if(((thgsr>>(2*TH_channel)) & 0x1) == 1 ){
                            if(mTHChannelState[TH_channel] != emTHSamplingState){
                                re_printf("warning","T&H channel %d is not triggered holding process because this channel is not in sampling state.\n", TH_channel);
                            }else{
                                if((holding_start_time - mTHSamplingStartTime[TH_channel]) < (mtTSPL*mADCLKPeriod)){
                                    re_printf("warning","T&H channel %d is triggered holding process when sampling process is not finished.\n", TH_channel);
                                }
                                mTHChannelState[TH_channel] = emTHHoldingState;
                                holding_enable = true;
                            }
                        }
                    }
                    if(holding_enable){
                        re_printf("info","The holding process of group B is started.\n");
                        mTHBHoldingProcess = true;
                        mGroupBNumber =  sgs_groupb + 1;
                        (*SGSR[mGroupBNumber])["SGACT"] = 1;
                        //Suspend all scanning process
                        if (mScanningState) {
                            mSuspendFlag = true;
                            mSuspendEvent.notify();
                        }
                        mTHGBHoldingEvent.notify((unsigned int)holding_delay,SC_NS);
                    }
                }//end for if(mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding)
            }//end for else
        }
        mADCStateEvent.notify();
    }
}//}}}

void Cadc::THAHoldingEndMethod(void)
{//{{{
    // mutex lock
    if(!mIsReset && (mADCInstance < mADCNum)){
        re_printf("info","The holding process of group A is finished.\n");
        if (!mTHBHoldingProcess) {
            mScanningEvent.notify();
        }
        mTHAHoldingProcess = false;
        HardwareTrigger(mGroupANumber, 1);
    }
}//}}}

void Cadc::THBHoldingEndMethod(void)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)){
        re_printf("info","The holding process of group B is finished.\n");
        if (!mTHAHoldingProcess) {
            mScanningEvent.notify();
        }
        mTHBHoldingProcess = false;
        HardwareTrigger(mGroupBNumber, 2);
    }
}//}}}

void Cadc::THzMethod(const unsigned int index)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)) {
        for (unsigned int i = 0; i < mTHChannelNum; i++) {
            if (index == mTHzAssignChannel[i]) {
                if(mTHChannelState[i] == emTHSamplingState) {
                    mTHUpdateInput[i] = true;
                    mTHUpdateInputEvent.notify();
                }
            }
        }
    }
}//}}}

void Cadc::cb_THSMPSTCR_SMPST (RegCBstr str) //sampling manually
{//{{{
    if((*THSMPSTCR)["SMPST"] == 1){
        (*THSMPSTCR) = 0;
        if ((((*THACR)["HLDCTE"] == 1) || ((*THBCR)["HLDCTE"] == 1)) && ((*ADCR1)["SUSMTD"] != 2)) {
            re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE or THBCR.HLDCTE is 1 while ADCR1.SUSMTD is not 2.\n");
            return;
        }
        if (((*THACR)["HLDCTE"] != 1) && ((*THBCR)["HLDCTE"] != 1)) {
            re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE and THBCR.HLDCTE are not 1.\n");
            return;
        }
        if ((*THACR)["SGS"] != (*THBCR)["SGS"]){
            mTHSamplingManual = true;
            mTHSamplingEvent.notify();
        } else {
            re_printf("warning","Sampling process in T&H circuit is not triggered because Group A and group B are same.\n");
        }
    }
}//}}}

void Cadc::cb_THSTPCR_THSTP (RegCBstr str) //stop T&H circuit
{//{{{
    if((*THSTPCR)["THSTP"] == 1){
        (*THSTPCR) = 0;
        mTHStop = true;
        mTHStopEvent.notify();
    }
}//}}}

void Cadc::cb_THCR_ASMPMSK (RegCBstr str) //automatic sampling mask
{//{{{
    if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0) // HLDTE of group A/B must be equal to 0
            || ((*SGSR[(*THACR)["SGS"] + 1])["SGACT"] != 0) || ((*SGSR[(*THBCR)["SGS"] + 1])["SGACT"] != 0) // SGACT of corresponding group must be equal to 0
            || ((*THER) != 0)) {  // All THzE must be equal to 0
        re_printf("warning","Writing into THCR register is not affected.\n");
        (*THCR) = str.pre_data;
    }
}//}}}

void Cadc::cb_THAHLDSTCR_HLDST (RegCBstr str) //trigger holding group A
{//{{{
    if((*THAHLDSTCR)["HLDST"] == 1){
        (*THAHLDSTCR) = 0;
        if(TimingCheck() && VoltageCheck()){
            if (((*THACR)["HLDCTE"] == 1) && ((*THACR)["HLDTE"] == 0)) {
                mTHASoftwareTriggerHolding = true;
                mTHASoftwareTriggerHoldingEvent.notify();
            }
        }
    }
}//}}}

void Cadc::cb_THBHLDSTCR_HLDST (RegCBstr str) //trigger holding group B
{//{{{
    if((*THBHLDSTCR)["HLDST"] == 1){
        (*THBHLDSTCR) = 0;
        if(TimingCheck() && VoltageCheck()){
            if (((*THBCR)["HLDCTE"] == 1) && ((*THBCR)["HLDTE"] == 0)) {
                mTHBSoftwareTriggerHolding = true;
                mTHBSoftwareTriggerHoldingEvent.notify();
            }
        }
    }
}//}}}

void Cadc::cb_THACR_SGS (RegCBstr str)
{//{{{
    if (((*THACR)["SGS"] != (str.pre_data & 0x3)) || ((*THACR)["HLDCTE"] != ((str.pre_data & 0x20) >> 5))) {
        if (!ChkAccess(str.is_wr, "", THACR->name(), "HLDCTE and SGS")) {
            (*THACR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
            return;
        } else if (((*THER) != 0) || ((str.pre_data & 0x10) != 0) || ((*THBCR)["HLDTE"] != 0)) {
            (*THACR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
            re_printf("warning","Writing into THACR.HLDCTE and SGS bit is not affected.\n");
            return;
        }
    }
    bool is_holding = false;
    for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
        if(mTHChannelState[TH_channel] == emTHHoldingState){
            is_holding = true;
            break;
        }
    }
    if(is_holding || mScanningState){
        re_printf("warning","Writing into THACR.SGS bit is not affected.\n");
        (*THACR) = (unsigned int)((str.data & 0x30) | (str.pre_data & 0xCF));
        if (((str.data & 0x10) != (str.pre_data & 0x10)) && ((*THACR)["HLDTE"] == 1)) {
            if ((*SGCR[(*THACR)["SGS"] + 1])["TRGMD"] != 1) {
                re_printf("warning","Cannot set THACR.HLDTE to 1 while SGCR%d.TRGMD != 1.\n", (unsigned int)(*THACR)["SGS"] + 1);
                (*THACR)["HLDTE"] = 0;
            }
        }
    }
    if (((*THACR)["HLDCTE"] == 0) && ((*THACR)["HLDTE"] != 0)) {
        re_printf("warning","Cannot set THACR.HLDTE = %d while THACR.HLDCTE = 0.\n", (bool)(*THACR)["HLDTE"]);
        (*THACR) = str.pre_data;
    }
}//}}}

void Cadc::cb_THBCR_SGS (RegCBstr str)
{//{{{
    if (((*THBCR)["SGS"] != (str.pre_data & 0x3)) || ((*THBCR)["HLDCTE"] != ((str.pre_data & 0x20) >> 5))) {
        if (!ChkAccess(str.is_wr, "", THBCR->name(), "HLDCTE and SGS")) {
            (*THBCR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
            return;
        } else if (((*THER) != 0) || ((str.pre_data & 0x10) != 0) || ((*THACR)["HLDTE"] != 0)) {
            (*THBCR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
            re_printf("warning","Writing into THBCR.HLDCTE and SGS bit is not affected.\n");
            return;
        }
    }
    bool is_holding = false;
    for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
        if(mTHChannelState[TH_channel] == emTHHoldingState){
            is_holding = true;
            break;
        }
    }
    if(is_holding || mScanningState){
        re_printf("warning","Writing into THBCR.SGS bit is not affected.\n");
        (*THBCR) = (unsigned int)((str.data & 0x30) | (str.pre_data & 0xCF));
        if (((str.data & 0x10) != (str.pre_data & 0x10)) && ((*THBCR)["HLDTE"] == 1)) {
            if ((*SGCR[(*THBCR)["SGS"] + 1])["TRGMD"] != 1) {
                re_printf("warning","Cannot set THBCR.HLDTE to 1 while SGCR%d.TRGMD != 1.\n", (unsigned int)(*THBCR)["SGS"] + 1);
                (*THBCR)["HLDTE"] = 0;
            }
        }
    }
    if (((*THBCR)["HLDCTE"] == 0) && ((*THBCR)["HLDTE"] != 0)) {
        re_printf("warning","Cannot set THBCR.HLDTE = %d while THBCR.HLDCTE = 0.\n", (bool)(*THBCR)["HLDTE"]);
        (*THBCR) = str.pre_data;
    }
}//}}}

void Cadc::cb_THER_TH0E (RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        if((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) { /// TH0E->TH3E only
            (*THER) = str.data & 0xF;
        }
        if((*THER) == 0){
            mTHStop = true;
            mTHStopEvent.notify();
        }else{
            mTHEnable = true;
            mTHEnableEvent.notify();
        }
    }
}//}}}

void Cadc::cb_THGSR_TH0GS (RegCBstr str)
{//{{{
    if((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
            (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) { /// TH0E->TH3E only
        (*THGSR) = str.data & 0x55;
    }
    if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0) // HLDTE of group A/B must be equal to 0
            || ((*SGSR[(*THACR)["SGS"] + 1])["SGACT"] != 0) || ((*SGSR[(*THBCR)["SGS"] + 1])["SGACT"] != 0) // SGACT of corresponding group must be equal to 0
            || ((*THER) != 0)) {  // All THzE must be equal to 0
        re_printf("warning","Writing into THGSR register is not affected.\n");
        (*THGSR) = (unsigned int)(str.pre_data);
    }
}//}}}

void Cadc::cb_MPXCMDR_MPXCMD(RegCBstr str)
{//{{{
    if ((mTargetProduct == "RH850_E2x-468Pins") && (!ChkAccess(true, "", "MPXCMDR", ""))) {
        (*MPXCMDR) = str.pre_data;
    }
}//}}}

void Cadc::cb_VCLMINTER1_ADUL00IE(RegCBstr str)
{//{{{
    // E2x-FCC2-468pins
    if ((mTargetProduct == "RH850_E2x-468Pins") && (!ChkAccess(true, "", "VCLMINTER1", ""))) {
        (*VCLMINTER1) = str.pre_data;
    }
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && (!CheckAllScanGroupStop())) {
        re_printf("warning","Writing into VCLMINTER1 register is not affected.\n");
        (*VCLMINTER1) = str.pre_data;
    }
}//}}}

void Cadc::cb_VCLMINTER2_ADUL32IE(RegCBstr str)
{//{{{
    // E2x-FCC2-468pins
    if ((mTargetProduct == "RH850_E2x-468Pins") && (!ChkAccess(true, "", "VCLMINTER2", ""))) {
        (*VCLMINTER2) = str.pre_data;
    }
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && (!CheckAllScanGroupStop())) {
        re_printf("warning","Writing into VCLMINTER2 register is not affected.\n");
        (*VCLMINTER2) = str.pre_data;
    }
}//}}}

//scanning software trigger method
void Cadc::ScanGroup0SoftwareTriggerMethod(void)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup0);
    }
}//}}}

void Cadc::ScanGroup1SoftwareTriggerMethod(void)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup1);
    }
}//}}}

void Cadc::ScanGroup2SoftwareTriggerMethod(void)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup2);
    }
}//}}}

void Cadc::ScanGroup3SoftwareTriggerMethod(void)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup3);
    }
}//}}}

void Cadc::ScanGroup4SoftwareTriggerMethod(void)
{//{{{
    if(!mIsReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup4);
    }
}//}}}

//external input port method
void Cadc::ADmSG0TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (ADmSG0TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup0,3);  //group 0, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc::ADmSG1TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (ADmSG1TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup1,3); //group 1, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc::ADmSG2TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (ADmSG2TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup2,3); //group 2, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc::ADmSG3TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (ADmSG3TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup3,3); //group 3, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc::ADmSG4TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (ADmSG4TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup4,3); //group 4, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

//counting end method
void Cadc::ScanGroup3ADtimerTriggerMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","The counting of ADtimer 3 is overflow.\n");
        HardwareTrigger(emGroup3,0);//group 3, trigger feature 0
    }
}//}}}

void Cadc::ScanGroup4ADtimerTriggerMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","The counting of ADtimer 4 is overflow.\n");
        HardwareTrigger(emGroup4,0);//group 4, trigger feature 0
    }
}//}}}

//interrupt ouput method
void Cadc::INT_MPXmInterruptMethod(void)
{//{{{
    if(mIsReset){
        INT_MPXm.write(0);
        DumpInterruptMessage("INT_MPX", "", "is reset to 0");
    }else if(mINT_MPXmActive){
        mINT_MPXmActive = false;
        INT_MPXm.write(1);
        mINT_MPXmActiveNum++;
        DumpInterruptMessage("INT_MPX", "", "Assert");
        mINT_MPXmInterruptEvent.notify((unsigned int)mPCLKPeriod, SC_NS);
    }else{
        INT_MPXm.write(0);
        mIsINT_MPXmOperating = false;
        DumpInterruptMessage("INT_MPX", "", "Deassert");
    }
}//}}}

//RH850-E2x FCC1/FCC2 added  ouput method for ADMPXn0->4
void Cadc::ADMPXOutputMethod(void)
{//{{{
    if(mIsReset){
        ADMPX0.write(0);
        ADMPX1.write(0);
        ADMPX2.write(0);
        ADMPX3.write(0);
        ADMPX4.write(0);
        re_printf("info","ADMPX ports are reset to initial value.\n");
    }else {
        bool admpx0 = bool(((unsigned int)(*MPXCURR)["MPXCUR"] >> 0) & 0x1);
        bool admpx1 = bool(((unsigned int)(*MPXCURR)["MPXCUR"] >> 1) & 0x1);
        bool admpx2 = bool(((unsigned int)(*MPXCURR)["MPXCUR"] >> 2) & 0x1);
        bool admpx3 = bool(((unsigned int)(*MPXCURR)["MPXCUR"] >> 3) & 0x1);
        bool admpx4 = bool(((unsigned int)(*MPXCURR)["MPXCUR"] >> 4) & 0x1);
        ADMPX0.write(admpx0);
        ADMPX1.write(admpx1);
        ADMPX2.write(admpx2);
        ADMPX3.write(admpx3);
        ADMPX4.write(admpx4);
        re_printf("info","ADMPX ports change value to 0x%X.\n", (unsigned int)(*MPXCURR)["MPXCUR"]);
    }
}//}}}

void Cadc::ADPEmInterruptMethod(void)
{//{{{
    if(mIsReset){
        ADPEm.write(0);
        DumpInterruptMessage("ADPE", "", "is reset to 0");
    } else if(mADPEmActive){
        mADPEmActive = false;
        mADPEmActiveNum++;
        ADPEm.write(1);
        DumpInterruptMessage("ADPE", "", "Assert");
        mADPEmInterruptEvent.notify((unsigned int)mPCLKPeriod, SC_NS);
    }else{
        mIsADPEmOperating = false;
        ADPEm.write(0);
        DumpInterruptMessage("ADPE", "", "Deassert");
    }
}//}}}

void Cadc::INT_ADmxINT_ADEmDataMethod(void)
{//{{{
    if(mIsReset){
        //1.INT_AD
        INT_ADm0.write(0);
        INT_ADm1.write(0);
        INT_ADm2.write(0);
        INT_ADm3.write(0);
        INT_ADm4.write(0);
        DumpInterruptMessage("INT_AD", "0", "is reset to 0");
        DumpInterruptMessage("INT_AD", "1", "is reset to 0");
        DumpInterruptMessage("INT_AD", "2", "is reset to 0");
        DumpInterruptMessage("INT_AD", "3", "is reset to 0");
        DumpInterruptMessage("INT_AD", "4", "is reset to 0");
        //2.INT_ADE
        INT_ADEm.write(0);
        DumpInterruptMessage("INT_ADE", "", "is reset to 0");
        //3. RH850_E2x added for ASF
        DFREQm.write(0);
        DFTAGm.write(0);
        DFDATAm.write(0);
    }else if(mINT_ADmxINT_ADEmDataActive){
        //1.INT_ADmx
        if(mINT_ADmxActive[emGroup0]){
            mINT_ADm0ActiveNum++;
            DumpInterruptMessage("INT_AD", "0", "Assert");
        }else if(mINT_ADmxActive[emGroup1]){
            mINT_ADm1ActiveNum++;
            DumpInterruptMessage("INT_AD", "1", "Assert");
        }else if(mINT_ADmxActive[emGroup2]){
            mINT_ADm2ActiveNum++;
            DumpInterruptMessage("INT_AD", "2", "Assert");
        }else if(mINT_ADmxActive[emGroup3]){
            mINT_ADm3ActiveNum++;
            DumpInterruptMessage("INT_AD", "3", "Assert");
        }else if(mINT_ADmxActive[emGroup4]){
            mINT_ADm4ActiveNum++;
            DumpInterruptMessage("INT_AD", "4", "Assert");
        }
        INT_ADm0.write(mINT_ADmxActive[emGroup0]);
        INT_ADm1.write(mINT_ADmxActive[emGroup1]);
        INT_ADm2.write(mINT_ADmxActive[emGroup2]);
        INT_ADm3.write(mINT_ADmxActive[emGroup3]);
        INT_ADm4.write(mINT_ADmxActive[emGroup4]);

        //2.INT_ADEm
        if((mErrorId & 0x1) == emIDError){
            if (IsTargetProductE2x()) {
                if((*DIR[mFinalVC])["IDEF"] == 1){
                    (*DIR[mFinalVC])["IDEF"] = 0;
                }
            } else {
                if((*IDER)["IDE"] == 0){
                    (*IDER)["IDECAP"] = mFinalVC;
                    (*IDER)["IDE"] = 1;
                }
            }
            re_printf("info","The id error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC );
        } else {
            if (IsTargetProductE2x()) {
                (*DIR[mFinalVC])["IDEF"] = 1;
            }
        }
        if((mErrorId & 0x4) == emOverwriteError){
            if((*OWER)["OWE"] == 0){
                (*OWER)["OWECAP"] = mFinalVC;
                (*OWER)["OWE"] = 1;
            }
            re_printf("info","The overwrite error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC);
        }
        if((mErrorId & 0x8) == emBoundaryError){
            if (IsTargetProductE2x()) {
                SetULErrorStatus(mFinalVC);
            } else {
                if((*ULER)["ULE"] == 0){
                    (*ULER)["ULE"] = 1;
                    (*ULER)["ULECAP"] = mFinalVC;
                }
            }
            re_printf("info","The boundary error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC);
        }

        if(((!IsTargetProductE2x()) && ((mErrorId & ((unsigned int)(*SFTCR)) & 0x0D) != 0))
                || ((IsTargetProductE2x()) && ((mErrorId & ((unsigned int)(*SFTCR)) & 0x05) != 0)) // RH850_E2x added ULEIE is not existed is RH850_E2x
                || ((IsTargetProductE2x()) && ((mErrorId & 0x8) == emBoundaryError) && ULIntrEnable(mFinalVC))){ // RH850_E2x merg port ADE and UL
            mErrorId = emADEAssert;
            INT_ADEm.write(1);
            mINT_ADEmActiveNum++;
            DumpInterruptMessage("INT_ADE", "", "Assert");
        }else{
            INT_ADEm.write(0);
        }

        //3.Data DRi, DIRn
        unsigned int prty = 0;
        // RH850_E2x added for not checking parity for ID
        if (!IsTargetProductE2x()) {
            prty = mID & 0x1 ;
            for(unsigned int loop = 1; loop < emIDbit; loop++){
                prty = prty ^ ((mID >> loop) & 0x1);
            }
        }
        for(unsigned int loop = 0; loop < emDRbit; loop++){
            prty = prty ^ ((mFinalData >> loop) & 0x1);
        }
        if (IsTargetProductE2x()) {
            prty = prty ^ ((*DIR[mFinalVC])["IDEF"]);
        }
        prty = prty & 0x1;
        if ((mFinalVC & 0x1) != 0) {
            (*DR[mFinalVC/2])["DR1"] = mFinalData;
        } else {
            (*DR[mFinalVC/2])["DR0"] = mFinalData;
        }
        (* DIR[mFinalVC])["DRn"]  = mFinalData;
        (* DIR[mFinalVC])["WFLG"] = 1;
        if (!IsTargetProductE2x()) { // DIR.ID is not existed in RH850_E2x
            (* DIR[mFinalVC])["ID"]   = mID;
        }
        (* DIR[mFinalVC])["PRTY"] = prty;
        if(mEnableConvertInfo){
            re_printf("info","The converted value: 0x%X.\n", mFinalData);
        }
        if (IsTargetProductE2x()) {
            // RH850_E2x added for ASF
            unsigned int dfent = (unsigned int)(*VCR[mFinalVC])["DFENT"];
            unsigned int dftag = (unsigned int)(*VCR[mFinalVC])["DFTAG"];
            unsigned int asentsgxe = ((unsigned int)(*DFASENTSGER) >> mFinalGroup) & 0x1;
            unsigned int data = mFinalData;
            if ((*ADCR2)["DFMT"] == 1) {
                re_printf("error", "When using ASF, setting to the signed 12 bit integer point format is prohibited.\n"); // RH850_E2x SARAD enhancement
            }
            if (((*ADCR2)["DFMT"] == 2) || ((*ADCR2)["DFMT"] == 3)) {
                data = data >> 1;
            }
            if ((dfent == 1) && (asentsgxe == 1)) {
                DFREQm.write(1);
                DFTAGm.write(dftag);
                DFDATAm.write(data);
            }
        }
        re_printf("info","The DR%d, DIR%d registers are updated new data.\n", mFinalVC/2, mFinalVC);
        mINT_ADmxINT_ADEmDataActive = false;
        mINT_ADmxINT_ADEmDataEvent.notify((unsigned int)mPCLKPeriod, SC_NS);
    }else{
        //1.INT_ADmx
        if(mINT_ADmxActive[emGroup0]){
            DumpInterruptMessage("INT_AD", "0", "Deassert");
            mINT_ADmxActive [emGroup0] = false;
        }else if(mINT_ADmxActive[emGroup1]){
            DumpInterruptMessage("INT_AD", "1", "Deassert");
            mINT_ADmxActive [emGroup1] = false;
        }else if(mINT_ADmxActive[emGroup2]){
            DumpInterruptMessage("INT_AD", "2", "Deassert");
            mINT_ADmxActive [emGroup2] = false;
        }else if(mINT_ADmxActive[emGroup3]){
            DumpInterruptMessage("INT_AD", "3", "Deassert");
            mINT_ADmxActive [emGroup3] = false;
        }else if(mINT_ADmxActive[emGroup4]){
            DumpInterruptMessage("INT_AD", "4", "Deassert");
            mINT_ADmxActive [emGroup4] = false;
        }
        INT_ADm0.write(mINT_ADmxActive[emGroup0]);
        INT_ADm1.write(mINT_ADmxActive[emGroup1]);
        INT_ADm2.write(mINT_ADmxActive[emGroup2]);
        INT_ADm3.write(mINT_ADmxActive[emGroup3]);
        INT_ADm4.write(mINT_ADmxActive[emGroup4]);

        //2.INT_ADEm
        if(mErrorId == emADEAssert){
            DumpInterruptMessage("INT_ADE", "", "Deassert");
        }
        mErrorId = 0;
        INT_ADEm.write(0);

        //4. RH850_E2x added for ASF
        DFREQm.write(0);
    }
}//}}}

void Cadc::ADENDm0Method(void)
{//{{{
    if(mIsReset){
        ADENDm0.write(0);
        re_printf("info","The ADEND%d0 output is reset to 0.\n", mADCInstance);
    }else{
        if(mADENDmxActive[emGroup0]){
            re_printf("info","The ADEND%d0 output is set to 1.\n", mADCInstance);
            ADENDm0.write(1);
        }else{
            re_printf("info","The ADEND%d0 output is cleared to 0.\n", mADCInstance);
            ADENDm0.write(0);
        }
    }
}//}}}

void Cadc::ADENDm1Method(void)
{//{{{
    if(mIsReset){
        ADENDm1.write(0);
        re_printf("info","The ADEND%d1 output is reset to 0.\n", mADCInstance);
    }else{
        if(mADENDmxActive[emGroup1]){
            re_printf("info","The ADEND%d1 output is set to 1.\n", mADCInstance);
            ADENDm1.write(1);
        }else{
            re_printf("info","The ADEND%d1 output is cleared to 0.\n", mADCInstance);
            ADENDm1.write(0);
        }
    }
}//}}}

void Cadc::ADENDm2Method(void)
{//{{{
    if(mIsReset){
        ADENDm2.write(0);
        re_printf("info","The ADEND%d2 output is reset to 0.\n", mADCInstance);
    }else{
        if(mADENDmxActive[emGroup2]){
            re_printf("info","The ADEND%d2 output is set to 1.\n", mADCInstance);
            ADENDm2.write(1);
        }else{
            re_printf("info","The ADEND%d2 output is cleared to 0.\n", mADCInstance);
            ADENDm2.write(0);
        }
    }
}//}}}

void Cadc::ADENDm3Method(void)
{//{{{
    if(mIsReset){
        ADENDm3.write(0);
        re_printf("info","The ADEND%d3 output is reset to 0.\n", mADCInstance);
    }else{
        if(mADENDmxActive[emGroup3]){
            re_printf("info","The ADEND%d3 output is set to 1.\n", mADCInstance);
            ADENDm3.write(1);
        }else{
            re_printf("info","The ADEND%d3 output is cleared to 0.\n", mADCInstance);
            ADENDm3.write(0);
        }
    }
}//}}}

void Cadc::ADENDm4Method(void)
{//{{{
    if(mIsReset){
        ADENDm4.write(0);
        re_printf("info","The ADEND%d4 output is reset to 0.\n", mADCInstance);
    }else{
        if(mADENDmxActive[emGroup4]){
            re_printf("info","The ADEND%d4 output is set to 1.\n", mADCInstance);
            ADENDm4.write(1);
        }else{
            re_printf("info","The ADEND%d4 output is cleared to 0.\n", mADCInstance);
            ADENDm4.write(0);
        }
    }
}//}}}

void Cadc::IFCProcessMethod(void) // RH850_E2x added for processing IFC
{//{{{
    if(mIsReset && (mADCInstance < mADCNum)){
        ADCm_IFDATA.write(0);
    }else{
        if (IFRDm.read() && IFREm.read()) {
            unsigned int vc_number = (unsigned int)IFVCHm.read();
            if (vc_number < mVirChannelNum) {
                unsigned int data = (unsigned int)(*DIR[vc_number])["DRn"];
                if ((*ADCR2)["DFMT"] == 1) {
                    re_printf("error", "When using IFC, setting to the signed 12 bit integer point format is prohibited.\n"); // RH850_E2x SARAD enhancement
                }
                if (((*ADCR2)["DFMT"] == 2) || ((*ADCR2)["DFMT"] == 3)) {
                    data = data >> 1;
                }
                ADCm_IFDATA.write((sc_uint<16>)(data));
                if((*SFTCR)["RDCLRE"] == 1){
                    if ((vc_number & 0x1) == 1) {
                        (*DR[vc_number/2])["DR1"] = 0;
                        re_printf("info","The DR%d.DR1, DIR%d registers are cleared to 0.\n", vc_number/2, vc_number);
                    } else {
                        (*DR[vc_number/2])["DR0"] = 0;
                        re_printf("info","The DR%d.DR0, DIR%d registers are cleared to 0.\n", vc_number/2, vc_number);
                    }
                    (*DIR[vc_number]) = 0;
                }else {
                    (*DIR[vc_number])["WFLG"] = 0;
                    re_printf("info","The DIR%d.WFLG is cleared to 0.\n", vc_number);
                }
            } else {
                re_printf("warning","The IFC read request virtual channel must be equal or less than %d.\n", mVirChannelNum - 1);
                ADCm_IFDATA.write(0);
            }
        }
    }
}//}}}

//tlm_tgt_if function
bool Cadc::common_tgt_rd(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size )
{ //{{{
    unsigned int tmp_add = addr & 0x7FF;
    if ((size == 2) && (tmp_add >= 0x100) && (tmp_add < (0x100 + (mVirChannelNum << 1)))) {
        if ((tmp_add)%4 == 0){
            mflag_1st_16bit = 1;
            mflag_2nd_16bit = 0;
        }else{
            mflag_1st_16bit = 0;
            mflag_2nd_16bit = 1;
        }
    }
    if ((size == 4) && (tmp_add >= 0x100) && (tmp_add < (0x100 + (mVirChannelNum << 1)))) {
        mflag_1st_16bit = 0;
        mflag_2nd_16bit = 0;
    }
    sc_assert(p_data != NULL);
    if(is_dbg){
        // Set debug mode (member variables of Cadc_regif class)
        if ((tmp_add >= 0x100) && (tmp_add < (0x100 + (mVirChannelNum << 1)))) {
            ReConvertDR (tmp_add);
        } else if ((tmp_add >= 0x200) && (tmp_add < (0x200 + (mVirChannelNum << 2)))) {
            ReConvertDIR (tmp_add);
        }
        return reg_rd_dbg(addr, p_data, size);
    }else{
        bool is_read_success = true;

        /// SARAD4 add
        // Check reading from VCRn/DRi/DIRn register
        if (((mVirChannelNum << 2) <= tmp_add) && (tmp_add < 0x0C0)) {
            re_printf("error","Cannot read VCR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)(tmp_add >> 2));
            return false;
        } else if (((0x100 + (mVirChannelNum << 1)) <= tmp_add) && (tmp_add < 0x160)) {
            re_printf("error","Cannot read DR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x100) >> 2));
            return false;
        } else if (((0x200 + (mVirChannelNum << 2)) <= tmp_add) && (tmp_add < 0x2C0)) {
            re_printf("error","Cannot read DIR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x200) >> 2));
            return false;
        }

        if ((tmp_add >= 0x100) && (tmp_add < (0x100 + (mVirChannelNum << 1)))) {
            ReConvertDR (tmp_add);
        } else if ((tmp_add >= 0x200) && (tmp_add < (0x200 + (mVirChannelNum << 2)))) {
            ReConvertDIR (tmp_add);
        }
        is_read_success = reg_rd(addr, p_data, size);
        if (is_read_success) {
            if ((tmp_add >= 0x100) && (tmp_add < (0x100 + (mVirChannelNum << 1)))) {
                CallBack_DRi(tmp_add);
            } else if ((tmp_add >= 0x200) && (tmp_add < (0x200 + (mVirChannelNum << 2)))) {
                CallBack_DIRn(tmp_add);
            }
        }
        return is_read_success;
    }
} //}}}

bool Cadc::common_tgt_wr(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size )
{//{{{
    sc_assert(p_data != NULL);
    if(is_dbg){
        return reg_wr_dbg(addr, p_data, size);
    }else{
        /// SARAD4 add
        unsigned int tmp_add = addr & 0x7FF;
        // Check writing to VCRn/DRi/DIRn register
        if (((mVirChannelNum << 2) <= tmp_add) && (tmp_add < 0x0C0)) {
            re_printf("error","Cannot write to VCR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)(tmp_add >> 2));
            return false;
        } else if (((0x100 + (mVirChannelNum << 1)) <= tmp_add) && (tmp_add < 0x160)) {
            re_printf("error","Cannot write to DR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x100) >> 2));
            return false;
        } else if (((0x200 + (mVirChannelNum << 2)) <= tmp_add) && (tmp_add < 0x2C0)) {
            re_printf("error","Cannot write to DIR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x200) >> 2));
            return false;
        }

        return reg_wr(addr, p_data, size);
    }
}//}}}

bool Cadc::ChkAccess (const bool is_wr, const std::string channel_name, const std::string register_name, const std::string bit_name)
{//{{{
    if(is_wr){
        bool wr_cond = true;
        if (register_name == "SMPCR") {
            if ((mTargetProduct != "RH850_P1M-E") && ((bit_name == "SMPT8_11") || (bit_name == "SMPT13_14"))) {// P1M-E support SMPT[15:0], current: SMPT[7:0], CCC D1D2: None
                return false;
            }
            if ((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                    (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) {// ODCR register is not available in CCC D1D2
                re_printf("error","SMPCR register is not existing in %s.\n", mTargetProduct.c_str());
                return false;
            }
        }
        if (register_name == "ODCR") {
            if ((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                    (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) {// ODCR register is not available in CCC D1D2
                re_printf("error","ODCR register is not existing in %s.\n", mTargetProduct.c_str());
                return false;
            }
            // RH850_E2x added
            if (IsTargetProductE2x()) { // DSCE bit is not available in RH850_E2x
                if (bit_name == "DSCE") {
                    re_printf("error","ODCR.DSCE bit is not existing in %s.\n", mTargetProduct.c_str());
                    return false;
                }
            }
        }
        if (register_name == "ULER") {
            re_printf("warning","ULER.%s forbids to write.\n", bit_name.c_str());
            return false;
        }
        // RH850_E2x added
        if (IsTargetProductE2x()) { // SMPCR, ULLMTBR0-2, ULLMSR0-4, ADENDP1-4, THSMPSTCR, THSTPCR, THCR, THAHLDSTCR, THBHLDSTCR, THACR, THBCR, THER, THGSR, THOMSR, register is not available in RH850_E2x
            if ((register_name == "SGSTPCR0")  // No need to check write condition (no setting condition or check by callback)
                    || (register_name == "SGSTPCR1") || (register_name == "SGSTPCR2") || (register_name == "SGSTPCR3") || (register_name == "SGSTPCR4")
                    || (register_name == "SGVCPR0") || (register_name == "SGVCPR1") || (register_name == "SGVCPR2") || (register_name == "SGVCPR3") || (register_name == "SGVCPR4")
                    || (register_name == "SGVCOWR0") || (register_name == "SGVCOWR1") || (register_name == "SGVCOWR2") || (register_name == "SGVCOWR3") || (register_name == "SGVCOWR4")
                    || (register_name == "VCULLMTBR0") || (register_name == "VCULLMTBR1")|| (register_name == "VCULLMTBR2") || (register_name == "VCULLMTBR3")
                    || (register_name == "VCULLMTBR4") || (register_name == "VCULLMTBR5")|| (register_name == "VCULLMTBR6")
                    || (register_name == "VCLMSCR1") || (register_name == "VCLMSCR2")) {
                return true;
            }
            if ((register_name == "VCLMSR1") || (register_name == "VCLMSR2")) {// Read only register
                return false;
            }
            if ((register_name == "SMPCR")
                    || (register_name == "ULLMTBR0") || (register_name == "ULLMTBR1") || (register_name == "ULLMTBR2")
                    || (register_name == "ULLMSR0") || (register_name == "ULLMSR1") || (register_name == "ULLMSR2") || (register_name == "ULLMSR3") || (register_name == "ULLMSR4")
                    || (register_name == "ADENDP1") || (register_name == "ADENDP2") || (register_name == "ADENDP3") || (register_name == "ADENDP4")
                    || (register_name == "THSMPSTCR")
                    || (register_name == "THSTPCR")
                    || (register_name == "THCR")
                    || (register_name == "THAHLDSTCR")
                    || (register_name == "THBHLDSTCR")
                    || (register_name == "THACR")
                    || (register_name == "THBCR")
                    || (register_name == "THER")
                    || (register_name == "THGSR")
                    || (register_name == "THOMSR")
                    || (register_name == "IDER")) {
                re_printf("error","%s register is not existing in %s.\n", register_name.c_str(), mTargetProduct.c_str());
                return false;
            }
        } else {
            if ((register_name == "MPXINTER")
                    || (register_name == "SGSTPCR0") || (register_name == "SGSTPCR1") || (register_name == "SGSTPCR2") || (register_name == "SGSTPCR3") || (register_name == "SGSTPCR4")
                    || (register_name == "SGVCPR0") || (register_name == "SGVCPR1") || (register_name == "SGVCPR2") || (register_name == "SGVCPR3") || (register_name == "SGVCPR4")
                    || (register_name == "SGVCOWR0") || (register_name == "SGVCOWR1") || (register_name == "SGVCOWR2") || (register_name == "SGVCOWR3") || (register_name == "SGVCOWR4")
                    || (register_name == "VCULLMTBR0") || (register_name == "VCULLMTBR1")|| (register_name == "VCULLMTBR2") || (register_name == "VCULLMTBR3")
                    || (register_name == "VCULLMTBR4") || (register_name == "VCULLMTBR5")|| (register_name == "VCULLMTBR6")
                    || (register_name == "VCLMINTER1") || (register_name == "VCLMINTER2")
                    || (register_name == "VCLMSR1") || (register_name == "VCLMSR2")
                    || (register_name == "VCLMSCR1") || (register_name == "VCLMSCR2")
                    || (register_name == "VMONVDCR1") || (register_name == "VMONVDCR2")) {
                re_printf("error","%s register is only existing in RH850_E2x.\n",register_name.c_str());
                return false;
            }
            if ((register_name == "ULLMTBR0") || (register_name == "ULLMTBR1") || (register_name == "ULLMTBR2")
                    || (register_name == "ULLMSR0") || (register_name == "ULLMSR1") || (register_name == "ULLMSR2") || (register_name == "ULLMSR3") || (register_name == "ULLMSR4")
                    || (register_name == "THSMPSTCR")
                    || (register_name == "THSTPCR")
                    || (register_name == "THCR")
                    || (register_name == "THAHLDSTCR")
                    || (register_name == "THBHLDSTCR")
                    || ((register_name == "THACR") && (bit_name != "HLDCTE and SGS")) // No need to check write condition (checked in callback)
                    || ((register_name == "THBCR") && (bit_name != "HLDCTE and SGS")) // No need to check write condition (checked in callback)
                    || (register_name == "THER")
                    || (register_name == "THGSR")
                    || (register_name == "THOMSR")) {
                return true;
            }
        }
        ///// if ((register_name == "ADOPDIG")) { // register only available in P1M-E
        /////     if (mTargetProduct != "RH850_P1M-E") {
        /////         return false;
        /////     }
        ///// }
        for(unsigned int group_number =0; group_number < emGroupNum; group_number++) {
            if (IsTargetProductE2x()) {
                if ((*SGSR[group_number])["SGACT"] != 0) {
                    wr_cond = false;
                }
            } else {
                if (((*SGSR[group_number])["SGACT"] != 0) || ((*SGCR[group_number])["ADSTARTE"] != 0) || ((*SGCR[group_number])["TRGMD"] != 0)) {
                    wr_cond = false;
                }
            }
        }
        if ((register_name != THACR->name()) && (register_name != THBCR->name())) {
            if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0)) {
                wr_cond = false;
            }
        }

        if(wr_cond == true){
            return true;
        } else {
            if (bit_name != "") {
                re_printf("warning","Writing into %s.%s bit is not affected.\n", register_name.c_str(),bit_name.c_str());
            }else{
                re_printf("warning","Writing into %s register is not affected.\n", register_name.c_str());
            }

            return false;
        }
    } else {
        // RH850_E2x added
        if (IsTargetProductE2x()) { // SMPCR, ULER, ULLMTBR0-2, ULLMSR0-4, THSMPSTCR, THSTPCR, THCR, THAHLDSTCR, THBHLDSTCR, THACR, THBCR, THER, THGSR, THOMSR, register is not available in RH850_E2x
            if ((register_name == "SMPCR")
                    || (register_name == "ULER")
                    || (register_name == "ULLMTBR0") || (register_name == "ULLMTBR1") || (register_name == "ULLMTBR2")
                    || (register_name == "ULLMSR0") || (register_name == "ULLMSR1") || (register_name == "ULLMSR2") || (register_name == "ULLMSR3") || (register_name == "ULLMSR4")
                    || (register_name == "ADENDP1") || (register_name == "ADENDP2") || (register_name == "ADENDP3") || (register_name == "ADENDP4")
                    || (register_name == "THSMPSTCR")
                    || (register_name == "THSTPCR")
                    || (register_name == "THCR")
                    || (register_name == "THAHLDSTCR")
                    || (register_name == "THBHLDSTCR")
                    || (register_name == "THACR")
                    || (register_name == "THBCR")
                    || (register_name == "THER")
                    || (register_name == "THGSR")
                    || (register_name == "THOMSR")) {
                re_printf("error","%s register is not existing in %s.\n", register_name.c_str(), mTargetProduct.c_str());
                return false;
            }
        } else {
            if ((register_name == "MPXINTER")
                    || (register_name == "SGSTPCR0") || (register_name == "SGSTPCR1") || (register_name == "SGSTPCR2") || (register_name == "SGSTPCR3") || (register_name == "SGSTPCR4")
                    || (register_name == "SGVCPR0") || (register_name == "SGVCPR1") || (register_name == "SGVCPR2") || (register_name == "SGVCPR3") || (register_name == "SGVCPR4")
                    || (register_name == "SGVCOWR0") || (register_name == "SGVCOWR1") || (register_name == "SGVCOWR2") || (register_name == "SGVCOWR3") || (register_name == "SGVCOWR4")
                    || (register_name == "VCULLMTBR0") || (register_name == "VCULLMTBR1")|| (register_name == "VCULLMTBR2") || (register_name == "VCULLMTBR3")
                    || (register_name == "VCULLMTBR4") || (register_name == "VCULLMTBR5")|| (register_name == "VCULLMTBR6")
                    || (register_name == "VCLMINTER1") || (register_name == "VCLMINTER2")
                    || (register_name == "VCLMSR1") || (register_name == "VCLMSR2")
                    || (register_name == "VCLMSCR1") || (register_name == "VCLMSCR2")
                    || (register_name == "VMONVDCR1") || (register_name == "VMONVDCR2")) {
                re_printf("error","%s register is only existing in RH850_E2x.\n",register_name.c_str());
                return false;
            }
        }
        return true;
    }
}//}}}

//handleCommand
void Cadc::AssignTHCircuit(const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel)
{//{{{
    if (th_channel >= mTHChannelNum) {
        re_printf("error","Invalid T&H channel %d, it must be less than %d.\n", th_channel, mTHChannelNum);
        return;
    }
    if (group_channel >= (emMaxPhyChannel/4)) {
        re_printf("error","Invalid group channel %d, it must be less than %d.\n", group_channel, emMaxPhyChannel/4);
        return;
    }
    if (physic_channel >= 4) {
        re_printf("error","Invalid physic channel %d, it must be less than 4.\n", physic_channel);
        return;
    }
    mTHzAssignChannel[th_channel] = 4*group_channel + physic_channel;
}//}}}

//operation functions
void Cadc::HardwareTrigger (const unsigned int group_number, const unsigned int trigger)
{//{{{
    /// SARAD3
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        start_delay = 2 * mADCLKPeriod;
    } else {
        start_delay = mtD;
    }

    if(TimingCheck() && VoltageCheck()){
        //0.Counting end : trigger = 0
        if((trigger == 0)&&(((*SGCR[group_number])["TRGMD"] == 2)||((*SGCR[group_number])["TRGMD"] == 3))){
            mIsHWTrigger[group_number] = true;
            mHWTriggerEvent[group_number].notify((unsigned int)start_delay, SC_NS);

            //1.Group A Holding end : trigger = 1
        } else if(trigger == 1){
            //1.1.Trigger scanning process
            if(((*THACR)["HLDCTE"] == 1)&&((*THACR)["SGS"] == (group_number - 1))&&((*SGCR[group_number])["TRGMD"] == 1)){
                GroupCheck(group_number);
            }
            //1.2.Trigger counting process
            else if(((*THACR)["HLDCTE"] == 1)&&((*THACR)["SGS"] == (group_number - 1))&&((*SGCR[group_number])["TRGMD"] == 3)){
                ADtimerCheck(group_number);
            }
            //2.Group B Holding end : trigger = 2
        } else if(trigger == 2){
            //2.1.Trigger scanning process
            if(((*THBCR)["HLDCTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))&&((*SGCR[group_number])["TRGMD"] == 1)){
                GroupCheck(group_number);
            }
            //2.2.Trigger counting process
            else if(((*THBCR)["HLDCTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))&&((*SGCR[group_number])["TRGMD"] == 3)){
                ADtimerCheck(group_number);
            }

            //3.External input port trigger : ADmSG0
        }else if((trigger == 3)&&(group_number == 0)){
            //3.1.Trigger scanning process
            if ((*SGCR[group_number])["TRGMD"] == 1) {
                GroupCheck(group_number);
            }

            //4.External input port trigger : ADmSG1/2/3/4
        }else if((trigger == 3)&&(group_number != 0)){
            bool trigger_enable = false;
            //4.1.Trigger group A holding process
            if ((((*THACR)["HLDCTE"] == 1)&&(*THACR)["HLDTE"] == 1)&&((*THACR)["SGS"] == (group_number - 1))) {
                mTHAPortTriggerHolding = true;
                mTHAPortTriggerHoldingEvent.notify();
                //4.2.Trigger group B holding process
            } else if ((((*THBCR)["HLDCTE"] == 1)&&(*THBCR)["HLDTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))) {
                mTHBPortTriggerHolding = true;
                mTHBPortTriggerHoldingEvent.notify();
                //4.3.Trigger counting process or scanning process
            } else if ((((*THACR)["HLDCTE"] == 0) && ((*THBCR)["HLDCTE"] == 0)) ||
                    (((*THACR)["HLDCTE"] == 0) && ((*THBCR)["SGS"] != (group_number - 1))) ||
                    (((*THBCR)["HLDCTE"] == 0)&&((*THACR)["SGS"] != (group_number - 1))) ||
                    (((*THBCR)["SGS"] != (group_number -1)) && ((*THACR)["SGS"] != (group_number -1)))) {
                trigger_enable = true;
            }

            if(trigger_enable){
                if((*SGCR[group_number])["TRGMD"] == 3){
                    ADtimerCheck(group_number);
                }else if((*SGCR[group_number])["TRGMD"] == 1){
                    GroupCheck(group_number);
                }
            }
        }
    }
}//}}}

void Cadc::GroupCheck (const unsigned int group_number)
{//{{{
    unsigned int start_vc = (unsigned int)((*SGVCSP[group_number])["VCSP"]);
    unsigned int end_vc   = (unsigned int)((*SGVCEP[group_number])["VCEP"]);
    if (mIsUpdateSGVCPR[group_number]) {
        mIsUpdateSGVCPR[group_number] = false;
    }
    //Check the VC condition
    if(mIsScanningProcess[group_number]) {
        re_printf("warning","Trigger scanning process for group %d is ignored because group %d is in scanning process.\n", group_number, group_number);
    }else if(start_vc > end_vc){
        re_printf("warning","Trigger scanning process for group %d is ignored because the start VC is greater than end VC.\n", group_number);
    }else if(start_vc >= mVirChannelNum){
        re_printf("warning","Trigger scanning process for group %d is ignored because the start VC is greater than %d.\n", group_number, mVirChannelNum - 1);
    }else if(end_vc >= mVirChannelNum){
        re_printf("warning","Trigger scanning process for group %d is ignored because the end VC is greater than %d.\n", group_number, mVirChannelNum - 1);
    }else{
        re_printf("info","The group %d is triggered for scanning process.\n", group_number);
        (*SGSR[group_number])["SGACT"] = 1;
        mIsScanningProcess[group_number] = true;
        mStartVCPointer[group_number]  = start_vc;
        mEndVCPointer[group_number]    = end_vc;
        mSGActive[group_number] = true;

        //Check scanningprocess
        if(mScanningState){
            if(mCurrentGroup < group_number){
                mSuspendFlag = true;
                if(SuspendCheck(mCurrentGroup)){
                    mSuspendEvent.notify();
                }
            }
        }else{
            if (!mTHAHoldingProcess && !mTHBHoldingProcess) {
                mScanningEvent.notify();
            }
        }
    }
}//}}}

bool Cadc::VCCheck (const unsigned int vc_number)
{//{{{
    unsigned int cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
    unsigned int gctrl = (unsigned int)((*VCR[vc_number])["GCTRL"]);
    bool pue = (bool)((*VCR[vc_number])["PUE"]);
    bool pde = (bool)((*VCR[vc_number])["PDE"]);
    /// SARAD4 modify
    unsigned int tmp_gctrl = gctrl & 0x1F;
    bool is_correct = true;

    if (pue && pde) {
        re_printf("warning","Cannot set both VCR%d.PUE and VCR%d.PDE to 1.\n", vc_number, vc_number);
        return false;
    }
    if ((pue || pde) && (cnvcls == emTHMode)) {
        re_printf("warning","Cannot use T&H when VCR%d.PUE = %d and VCR%d.PDE = %d.\n", vc_number, pue, vc_number, pde);
        return false;
    }

    switch (cnvcls) {
        case emTHMode:
            if((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                    (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) { /// GCTRL[4:0]
                if(gctrl > 0x3) {
                    re_printf("warning","The setting of VCR%d.CNVCLS and VCR%d.GCTRL is not matched in the T&H conversion mode.\n", vc_number, vc_number);
                    is_correct = false;
                }
            } else { /// others & P1M-E -> GCTRL[5:0]
                if((gctrl == 6)||(gctrl == 7)) {
                    re_printf("warning","The setting of VCR%d.CNVCLS and VCR%d.GCTRL is not matched in the T&H conversion mode.\n", vc_number, vc_number);
                    is_correct = false;
                } else {
                    if((gctrl & 0x38) != 0){
                        re_printf("warning","The VCR%d.GCTRL[5:3] bits should be zero.\n", vc_number);
                    }
                }
            }
            break;
            //1.3 Get from diagnosis circuit
            /// SARAD4 modify
        case emDiagnosisMode :
            if((tmp_gctrl != 0x0)&&(tmp_gctrl != 0x04)&&(tmp_gctrl != 0x08)&&(tmp_gctrl != 0x0C)&&(tmp_gctrl != 0x10)){
                re_printf("warning","The setting of VCR%d.CNVCLS and VCR%d.GCTRL is not matched in the diagnosis conversion mode.\n", vc_number, vc_number);
                is_correct = false;
            }
            break;
        case emNormalMode:
        case emAdditionMode:
            if (tmp_gctrl >= mPhyChannelNum) {
                re_printf("error","VCR%d.GCTRL[4:0] = %d is greater than maximum analog channel index.\n", vc_number, tmp_gctrl);
                is_correct = false;
            }
            break;
        default:
            break;
    }

    if ((gctrl & 0x20) != 0) {
        re_printf("warning","The VCR%d.GCTRL[5] bits should be zero.\n", vc_number);
    }

    return is_correct;
}//}}}

unsigned int Cadc::VCAddition (const unsigned int vc_number)
{//{{{
    unsigned int cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
    bool addnt = (bool)((*ADCR2)["ADDNT"]);
    unsigned int vc_add_number = 1;
    if ((cnvcls == emAdditionMode) || (cnvcls == emMPXAdditionMode)) {
        if (addnt) {
            vc_add_number = 4;
        } else {
            vc_add_number = 2;
        }
    }
    return vc_add_number;
}//}}}

unsigned int Cadc::GetData (const unsigned int group_number, const unsigned int vc_number, const unsigned int vc_add)
{//{{{
    unsigned int cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
    unsigned int gctrl  = (unsigned int)((*VCR[vc_number])["GCTRL"]);
    unsigned int thgsr  = (unsigned int)(*THGSR);
    double tmp_data = 0;
    unsigned int tdlv   = (unsigned int)(*TDCR)["TDLV"];
    unsigned int index = gctrl & 0x1F;
    /// SARAD4 modify
    bool is_index_odd = false;
    double data = 0;
    bool normal_convert = false;
    double ratio = 0;
    //1.Get data
    if ((cnvcls == emNormalMode) || (cnvcls == emAdditionMode) || (cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
        //the self-diagnosis function
        if ((*TDCR)["TDE"] == 1) {
            /// SARAD4 modify
            if ((cnvcls == emNormalMode) || (cnvcls == emAdditionMode)) {
                // Normal mode
                is_index_odd = (bool)(index & 0x1);
            } else {
                // MPX mode
                is_index_odd = (bool)(mMPXInput & 0x1);
            }

            switch (tdlv) {
                case 0:
                    if (is_index_odd) {
                        data = mAVcc;
                    } else {
                        data = 0;
                    }
                    break;
                case 1:
                    if (is_index_odd) {
                        data = 0;
                    } else {
                        data = mAVcc;
                    }
                    break;
                case 2:
                    if (is_index_odd) {
                        data = mAVcc/2;
                    } else {
                        data = 0;
                    }
                    break;
                default:
                    // tdlv = 0x3
                    if (is_index_odd) {
                        data = 0;
                    } else {
                        data = mAVcc/2;
                    }
                    break;
            }

        } else if (mOpenCircuitError) {
            //disconnect function
            if (mTargetProduct == "RH850_P1M-E") {
                if (((*VCR[vc_number])["PUE"] == 1) && ((((*ADOPDIG) >> (*VCR[vc_number])["GCTRL"]) & 0x1) == 0x1)) {
                    data = mAVcc;
                } else if (((*VCR[vc_number])["PDE"] == 1) && ((((*ADOPDIG) >> (*VCR[vc_number])["GCTRL"]) & 0x1) == 0x1)) {
                    data = 0;
                } else {
                    normal_convert = true;
                }
            } else {
                if ((*VCR[vc_number])["PUE"] == 1){
                    data = mAVcc;
                } else if ((*VCR[vc_number])["PDE"] == 1) {
                    data = 0;
                } else {
                    normal_convert = true;
                }
            }
        } else {
            //normal
            normal_convert = true;
        }

        if (normal_convert) {
            if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
                /// SARAD4 modify
                data = LoadAnalogTerminal(mMPXInput);
            } else {
                data = LoadAnalogTerminal(index);
                if ((IsTargetProductE2x())
                        && ((index == emVCCPortNum) || (index == emEVCCPortNum) || (index == emVDDPortNum)) && (mADCInstance == 0)) { // VCC, EVCC, VDD of RH850_E2x Enhancement
                    data = AdjustVMONInput(data, index);
                }
            }
        }
    } else if (cnvcls == emTHMode) {
        gctrl = gctrl & 0x7;
        thgsr = (thgsr >> (2*gctrl))&0x1;
        //check matching between T&H setting and VC setting
        if (mTHChannelState[gctrl] == emTHIdleState) {
            re_printf("warning","The TH channel %d is in IDLE state, input data is all 0.\n", gctrl);
            tmp_data = 0;
        } else if (((*THACR)["SGS"] == (*THBCR)["SGS"]) || (group_number == 0) || ((thgsr == 0) && ((*THACR)["SGS"] != (group_number - 1))) ||
                ((thgsr == 1) && ((*THBCR)["SGS"] != (group_number - 1)))) {
            re_printf("warning","Setting in T&H circuit is mismatch, input data is all 0.\n");
            tmp_data = 0;
        } else {
            tmp_data = mIntTHSample[gctrl];
            if(mTHChannelState[gctrl] != emTHHoldingState){
                re_printf("warning","The TH channel %d which is converted is not in holding state.\n", gctrl);
            }else{
                mTHUpdateChannelState[gctrl] = true;
            }
        }
        data = tmp_data;
    } else if (cnvcls == emDiagnosisMode) {
        gctrl = gctrl & 0x1F;
        if(gctrl == 0x0){
            data = 0;
            ratio = 0;
        }else if(gctrl == 0x04){
            data = mAvrefh/4;
            ratio = 0.25;
        }else if(gctrl == 0x08){
            data = mAvrefh/2;
            ratio = 0.5;
        }else if(gctrl == 0x0C){
            data = (mAvrefh*3)/4;
            ratio = 0.75;
        }else if(gctrl == 0x10){
            data = mAvrefh;
            ratio = 1;
        }
    }

    //2.Check boundary of data
    bool is_all_1 = false;
    if (data >= mAvrefh) {
        if (data > mAvrefh) {
            re_printf("warning","The voltage of analog input port is greater than AVREFH, all bits of data conversion will be 1.\n");
        }
        data = 0;
        is_all_1 = true;
    } else if (data < 0) {
        re_printf("warning","The voltage of analog input port is negative value, all bits of data conversion will be 0.\n");
        data = 0;
    }

    //3.Correct format of data
    unsigned int final_data = 0;
    if((*ADCR2)["DFMT"] == 1){
        if(is_all_1){
            final_data = emIntergerFormatAllOne;
        }else{
            if (cnvcls == emDiagnosisMode) {
                final_data = (unsigned int)((unsigned int)(ratio * emIntergerFormat) & emIntergerFormatAllOne);
            } else {
                final_data = (unsigned int)((unsigned int)((data/mAvrefh) * emIntergerFormat) & emIntergerFormatAllOne);
            }
        }
    }else{ // DFMT = 0, 2, 3
        if(is_all_1){
            final_data = emFixFormatAllOne;
        }else{
            if (cnvcls == emDiagnosisMode) {
                final_data = (unsigned int)((unsigned int)(ratio * emFixFormat) & emFixFormatAllOne);
            } else {
                final_data = (unsigned int)((unsigned int)((data/mAvrefh) * emFixFormat) & emFixFormatAllOne);
            }
        }
    }
    return final_data;

}//}}}

void Cadc::ADtimerCheck (const unsigned int group_number)
{//{{{
    //0.Check counting process is operating
    if((*SGSR[group_number])["ADTACT"] == 1){
        re_printf("warning","Trigger counting process for AD timer %d is ignored because AD timer %d is counting.\n", group_number, group_number);
        //1.Check the start value in counting process
    }else if((*ADTIPR[group_number])["ADTIP"] == 0){
        re_printf("warning","Trigger counting process for AD timer %d is ignored because the ADTIPR%d.ADTIP is smaller than 1.\n", group_number, group_number);
    }else if((*ADTPRR[group_number])["ADTPR"] == 0){
        re_printf("warning","Trigger counting process for AD timer %d is ignored because the ADTPRR%d.ADTPR is smaller than 1.\n", group_number, group_number);
        //2.Start counting process
    }else{
        ((*SGSR[group_number])["ADTACT"]) = 1;
        mADCStateEvent.notify();
        mTIMER[group_number - 3]->mPeriod = mADCLKPeriod;

        mTIMER[group_number - 3]->mNextValue = (unsigned int)((*ADTPRR[group_number])["ADTPR"]);
        mTIMER[group_number - 3]->StartCounting((unsigned int)((*ADTIPR[group_number])["ADTIP"]));
        re_printf("info","The counting of AD timer %d is started.\n", group_number);
    }
}//}}}

double Cadc::LoadAnalogTerminal (const unsigned int index)
{//{{{
    double data = 0;
    switch (index){
        case 0:
            data = ANm00.read();
            break;
        case 1:
            data = ANm01.read();
            break;
        case 2:
            data = ANm02.read();
            break;
        case 3:
            data = ANm03.read();
            break;
        case 4:
            data = ANm10.read();
            break;
        case 5:
            data = ANm11.read();
            break;
        case 6:
            data = ANm12.read();
            break;
        case 7:
            data = ANm13.read();
            break;
        case 8:
            data = ANm20.read();
            break;
        case 9:
            data = ANm21.read();
            break;
        case 10:
            data = ANm22.read();
            break;
        case 11:
            data = ANm23.read();
            break;
        case 12:
            data = ANm30.read();
            break;
        case 13:
            data = ANm31.read();
            break;
        case 14:
            data = ANm32.read();
            break;
        case 15:
            data = ANm33.read();
            break;
        case 16:
            data = ANm40.read();
            break;
        case 17:
            data = ANm41.read();
            break;
        case 18:
            data = ANm42.read();
            break;
        case 19:
            data = ANm43.read();
            break;
            /// SARAD4 add
        case 20:
            data = ANm50.read();
            break;
        case 21:
            data = ANm51.read();
            break;
        case 22:
            data = ANm52.read();
            break;
        case 23:
            data = ANm53.read();
            break;
        case 24:
            data = ANm60.read();
            break;
        case 25:
            data = ANm61.read();
            break;
        case 26:
            data = ANm62.read();
            break;
        case 27:
            data = ANm63.read();
            break;
        case 28:
            data = ANm70.read();
            break;
        case 29:
            data = ANm71.read();
            break;
        case 30:
            data = ANm72.read();
            break;
        case 31:
            data = ANm73.read();
            break;
        default:
            break;
    }

    return data;
}//}}}

double Cadc::AdjustVMONInput(const double data, const unsigned int index)
{//{{{
    double adjust_data;
    if (mVMONVDCRVal == 3) { // VDE2 = 1, VDE1 = 1
        if ((index == emVCCPortNum) || (index == emEVCCPortNum)) { // VCC/EVCC
            if ((index == emEVCCPortNum) && ((data < 4.5)  || (data > 5.5))) {
                re_printf("warning", "EVCC can be guaranteed operation in 5V (4.5 to 5.5V).\n");
            }
            adjust_data = data * 3 / 4;
        } else { // VDD
            adjust_data = data;
        }
    } else {
        adjust_data = 0;
    }

    return adjust_data;
}//}}}

void Cadc::ExportData(const unsigned int case_number, const unsigned int group_number, const unsigned int vc_number, unsigned int *data, const unsigned int group_loop )
{//{{{
    unsigned int cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
    unsigned int gctrl  = (unsigned int)((*VCR[vc_number])["GCTRL"]);
    unsigned int vc_add_number = VCAddition(vc_number);
    unsigned int uls = (unsigned int)((*ULLMSR[group_number])["ULS"]);
    unsigned int convt_data = 0;
    double finish_delay = 0;
    if (mEnableTimeCalculation) {
        finish_delay = (emtEDCoeffPCLK * mPCLKPeriod) + (emtEDCoeffADCLK * mADCLKPeriod);
    } else {
        finish_delay = mtED;
    }
    switch (case_number) {
        case emStartConvert: //First time --> INT_MPXm, ADENDmx, MPXCURR
            //1.MPXCURR and INT_MPX
            if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
                //1.1.MPXCURR
                // RH850_E2x added
                if ((mTargetProduct == "") || IsTargetProductE2x()) { //// original version
                    (*MPXCURR)["MPXCMD"] = (unsigned int)(*MPXCMDR)["MPXCMD"];
                }
                if ((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                        (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) {  /// MPXCUR[3:0]
                    (*MPXCURR)["MPXCUR"] = gctrl & 0x0F;
                } else { // original & P1ME --> MPXCUR[4:0]
                    (*MPXCURR)["MPXCUR"] = gctrl & 0x1F;
                    if (IsTargetProductE2x()) {
                        mADMPXOutputEvent.notify(); //add for FCC1/FCC2
                    }
                }
                //1.2.INT_MPX
                if (mIsINT_MPXmOperating) {
                    re_printf("warning","INT_MPX%d is not asserted because INT_MPX%d interrupt is in process.\n", mADCInstance, mADCInstance);
                } else if ((!IsTargetProductE2x()) || (IsTargetProductE2x() && ((unsigned int)(*MPXINTER)["ADMPXIE"] == 1))){ // RH850_E2x added for condition of asserting INT_MPX
                    mIsINT_MPXmOperating = true;
                    mINT_MPXmActive = true;
                    mINT_MPXmInterruptEvent.notify();
                }
            }
            //2.ADENDmx
            if(((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x"))&&(vc_number == ((*ADENDP[0])["ENDP"]))) {      // RH850_E2x added Use ADENDP0 for all scanning group
                mADENDmxActive[group_number] = true;
                mADENDmxEvent[group_number].notify();
            }else if ((((mTargetProduct == "RH850_E2x-468Pins")&&(vc_number == ((*ADENDP[0])["ENDP"]))) 
                        || ((!IsTargetProductE2x()) && (vc_number == ((*ADENDP[group_number])["ENDP"]))))
                    && ((mLastVC != vc_number) || (mLastGroup != group_number))){
                mADENDmxActive[group_number] = true;
                mADENDmxEvent[group_number].notify();
            }
            break;
        case emCorruptConvert:
            //1.Clear ADENDmxEvent
            if ((IsTargetProductE2x() && (vc_number == ((*ADENDP[0])["ENDP"])))                // RH850_E2x added Use ADENDP0 for all scanning group
                    ||  ((!IsTargetProductE2x()) && (vc_number == ((*ADENDP[group_number])["ENDP"])))) {
                mADENDmxActive[group_number] = false;
                mADENDmxEvent[group_number].notify();
            }
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                mTHUpdateChannelState[TH_channel] = false;
            }
            break;
        case emFinishConvert:
            //1.CollectData
            if (vc_add_number == 1) {
                if(data != NULL){
                    mStoreData = data[0];
                }
            } else {
                for(unsigned int add_number = 0; add_number < vc_add_number; add_number++){
                    mStoreData = mStoreData + data[add_number];
                }
            }
            if ((vc_add_number == 2) && (mStoreData == 0x1FFE) && ((*ADCR2)["DFMT"] == 1)) {
                mStoreData = 0x1FFF;
            } else if ((vc_add_number == 4) && (mStoreData == 0x3FFC) && ((*ADCR2)["DFMT"] == 1)) {
                mStoreData =  0x3FFF;
            } else if ((vc_add_number == 2) && (mStoreData == 0x0FFF0) && (((*ADCR2)["DFMT"] == 0) || ((*ADCR2)["DFMT"] == 2) || ((*ADCR2)["DFMT"] == 3))) {
                mStoreData =  0x7FFC;
            } else if ((vc_add_number == 4) && (mStoreData == 0x1FFE0) && (((*ADCR2)["DFMT"] == 0) || ((*ADCR2)["DFMT"] == 2) || ((*ADCR2)["DFMT"] == 3))) {
                mStoreData =  0x7FFE;
            } else if (((*ADCR2)["DFMT"] == 0) || ((*ADCR2)["DFMT"] == 2) || ((*ADCR2)["DFMT"] == 3)) {
                mStoreData = mStoreData >> (vc_add_number >> 1);
            }
            convt_data = mStoreData;
            // RH850_E2x added for support DFMT = 2(12bit unsigned format) and DFMT = 3(10 bits unsigned format)
            if ((*ADCR2)["DFMT"] == 2) {
                mStoreData = mStoreData << 1;
            } else if ((*ADCR2)["DFMT"] == 3) {
                mStoreData = mStoreData << 1;
                if (vc_add_number == 4) {
                    mStoreData = mStoreData & 0xFFF0;
                } else if (vc_add_number == 2) {
                    mStoreData = mStoreData & 0xFFE0;
                } else {
                    mStoreData = mStoreData & 0xFFC0;
                }
            }
            mFinalVC = vc_number;
            mFinalGroup = mLastGroup;
            if(mTHUpdateChannelState[0] || mTHUpdateChannelState[1] || mTHUpdateChannelState[2] || mTHUpdateChannelState[3] ||
                    mTHUpdateChannelState[4] || mTHUpdateChannelState[5]){
                mTHUpdateChannelStateFlag = true;
                mTHUpdateChannelStateEvent.notify();
            }
            //2.ADENDmx
            if(((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x"))&&(vc_number == ((*ADENDP[0])["ENDP"]))) {      // RH850_E2x added Use ADENDP0 for all scanning group
                mADENDmxActive[group_number] = false;
                mADENDmxEvent[group_number].notify();
            }else if ((((mTargetProduct == "RH850_E2x-468Pins")&&(vc_number == ((*ADENDP[0])["ENDP"]))) 
                        || ((!IsTargetProductE2x()) && (vc_number == ((*ADENDP[group_number])["ENDP"]))))
                    && (mSuspendFlag || (mStartVCPointer[group_number] != mEndVCPointer[group_number]) || (group_loop == 1) && ((*SGCR[group_number])["SCANMD"] == 0))) {
                mADENDmxActive[group_number] = false;
                mADENDmxEvent[group_number].notify();
            }
            //3.INT_ADmx, INT_ADEm, DRi, ,DIRn
            //3.1.Check ID error
            if(mForcedIdError){
                mErrorId = mErrorId | 0x1;
            }
            //3.2.Check Overwire error
            if((*DIR[vc_number])["WFLG"] == 1){
                mErrorId = mErrorId | 0x4;
            }

            //3.3.Check boundary error
            if (IsTargetProductE2x()) { // RH850_E2x added for processing upper/lower bound error
                unsigned int ullmtbs = (unsigned int)((*VCR[vc_number])["VCULLMTBS"]);
                unsigned int ulmtb = (unsigned int)(*VCULLMTBR[ullmtbs])["VCULMTB"];
                unsigned int llmtb = (unsigned int)(*VCULLMTBR[ullmtbs])["VCLLMTB"];
                if (((unsigned int)((*VCR[vc_number])["VCULME"]) == 1) || ((unsigned int)((*VCR[vc_number])["VCLLME"]) == 1)) {
                    mCurVC = vc_number;
                    mVCENDNoticeEvent.notify();
                    mConvtData = convt_data;
                    if(((unsigned int)((*VCR[vc_number])["VCULME"]) == 1) && (convt_data > ulmtb)) {
                        mErrorId = mErrorId | 0x8;
                        mULMTB = ulmtb;
                        mLLMTB = 0;
                        mBoundaryExcessEvent.notify();
                    }
                    if(((unsigned int)((*VCR[vc_number])["VCLLME"]) == 1) && (convt_data < llmtb)) {
                        mErrorId = mErrorId | 0x8;
                        mULMTB = 0xFFFF;
                        mLLMTB = llmtb;
                        mBoundaryExcessEvent.notify();
                    }
                }
            }else{
                if((uls != 0) && ((convt_data > (*ULLMTBR[uls - 1])["ULMTB"] ) || (convt_data < (*ULLMTBR[uls - 1])["LLMTB"]))){
                    mErrorId = mErrorId | 0x8;
                }
            }

            //3.4.Check INT_AD interrupt
            if (((*VCR[vc_number])["ADIE"] == 1) || ((vc_number == mEndVCPointer[group_number])&&((*SGCR[group_number])["ADIE"] == 1))) {
                mINT_ADmxActive[group_number] = true;
            }

            if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
                /// SARAD4 modify
                mID = mMPXInput;
            } else if (cnvcls == emTHMode) {
                if ((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                        (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) {
                    mID = (gctrl & (mTHChannelNum -1));
                } else {
                    mID = (gctrl & 0x7);
                }
            } else {
                mID = (gctrl & 0x1F);
            }

            // Check Break error

            if (mForcedBreak) {
                if ((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                        (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) {
                    mStoreData = 0;
                } else { // other products
                    if ((*ODCR)["ODDE"] == 1) {
                        mStoreData = 0;
                    }
                }
            }

            mFinalData = mStoreData;
            mStoreData = 0;
            mAD[mFinalVC] = GetAD(mFormat_temp[mFinalVC], mFinalData); //Store AD[13:0] 
            mINT_ADmxINT_ADEmDataActive = true;
            mINT_ADmxINT_ADEmDataEvent.notify((unsigned int)finish_delay, SC_NS);

            //check disconnect function
            if ((mTargetProduct != "CCC_D1V1") && (mTargetProduct != "CCC_D1V2") &&
                    (mTargetProduct != "CCC_D2V1") && (mTargetProduct != "CCC_D2V2")) {
                if ((*ODCR)["ODDE"] == 0) {
                    if (mForcedOpenError) {
                        mOpenCircuitError = true;
                    } else {
                        mOpenCircuitError = false;
                    }
                } else {
                    mOpenCircuitError = true;
                }
            } else { /// CCC_D1D2
                //mOpenCircuitError = true; // DucDuong fix 2018.06.22
                if (mForcedOpenError) {
                    mOpenCircuitError = true;
                } else {
                    mOpenCircuitError = false;
                }
            }
            break;
        default:
            break;
    }
}//}}}

double Cadc::ADCTimeCalculation (const unsigned int vc_number, const unsigned int vc_add)
{//{{{
    unsigned int cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
    unsigned int mpxow  = (unsigned int)((*MPXOWR)["MPXOW"]);
    double sampling_time = 0;
    double start_time = 0;

    if (mTargetProduct == "RH850_P1M-E") {
        double conversion_time = 0;

        // Calculate sampling time <--- temporary
        // There is no bit to specify sampling time, therefore I decide to selec mExSmpt
        sampling_time = (double)(mExSmpt * mADCLKPeriod);

        // Calculate wait time
        if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
            if ((mLastVC != vc_number) && (vc_add == 0)) {
                if (mEnableTimeCalculation) {
                    unsigned int smpcr15_0 = (unsigned int)(*SMPCR) & 0xFFFF;
                    if(smpcr15_0 == 0x0000) {
                        conversion_time = 1000; // 1 micro second = 1000 nano second
                    } else { // assump else is 0x90CC
                        conversion_time = 11300; // 11.3 micro second = 11300 nano second
                    }
                    start_time = (double)(mpxow * conversion_time);
                } else {
                    start_time = (double)(mpxow * (sampling_time + mtSAR));
                }
            }
        }
    } else if ((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") || (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2") // SMPCR register is not available in CCC D1D2
            || IsTargetProductE2x()) {  // SMPCR register is not available in RH850_E2x
        //1.Calculate sampling delay time : mExSmpt
        sampling_time = (double)(mExSmpt * mADCLKPeriod);
        //2.Calculate mWait time
        if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
            if ((mLastVC != vc_number) && (vc_add == 0)) {
                if (mEnableTimeCalculation) {
                    start_time = (double)(mpxow * (sampling_time + (emtSARCoeff * mADCLKPeriod)));
                } else {
                    start_time = (double)(mpxow * (sampling_time + mtSAR));
                }
            }
        }
    } else {
        //1.Calculate sampling delay time : mExSmpt
        if ((*SMPCR)["SMPTS"] == 1) {
            sampling_time = (double)((*SMPCR)["SMPT"] * mADCLKPeriod);
        } else {
            sampling_time = (double)(mExSmpt * mADCLKPeriod);
        }

        //2.Calculate mWait time
        if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
            if ((mLastVC != vc_number) && (vc_add == 0)) {
                if (mEnableTimeCalculation) {
                    start_time = (double)(mpxow * (sampling_time + (emtSARCoeff * mADCLKPeriod)));
                } else {
                    start_time = (double)(mpxow * (sampling_time + mtSAR));
                }
            }
        }
    }
    //3.Total delay time
    double delay_total = sampling_time + start_time;
    return delay_total;
}//}}}

bool Cadc::TimingCheck (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    double sample_delay = 0;
    if((*SMPCR)["SMPTS"] == 0){
        sample_delay = (double)(mExSmpt * mADCLKPeriod);
    }else{
        unsigned int smpt = (unsigned int)((*SMPCR)["SMPT"]);
        sample_delay = (double)(smpt*mADCLKPeriod);
    }
    double convert_delay = 0;
    double finish_delay  = 0;
    if (mEnableTimeCalculation) {
        convert_delay = emtSARCoeff * mADCLKPeriod;
        finish_delay = (emtEDCoeffPCLK * mPCLKPeriod) + (emtEDCoeffADCLK * mADCLKPeriod);
    } else {
        convert_delay = mtSAR;
        finish_delay = mtED;
    }

    if ((finish_delay + mPCLKPeriod) >= (sample_delay + convert_delay)) {
        re_printf("error","The condition of tED, ExSmpt and tSAR is not correct.\n");
        return false;
    } else {
        return true;
    }
}//}}}

bool Cadc::VoltageCheck (void)
{//{{{
    if ((mADCInstance >= mADCNum) || ((mAVccMin <= mAVcc) && (mAVcc <= mAVccMax) && (mAvrefhMin <= mAvrefh) && (mAvrefh <= mAVcc))) {
        return true;
    } else {
        re_printf("warning","Setting reference voltages for ADC%d is not correct: mAvrefh:%lld.%06d, mAVcc:%lld.%06d.\n", mADCInstance, (unsigned long long) mAvrefh, (unsigned int) ((mAvrefh-(unsigned long long) mAvrefh)*1000000),  (unsigned long long) mAVcc, (unsigned int) ((mAVcc-(unsigned long long) mAVcc)*1000000));
        return false;
    }
}//}}}

bool Cadc::SuspendCheck (unsigned int group_number)
{//{{{
    unsigned int susmtd = (unsigned int)((*ADCR1)["SUSMTD"]);
    if (((group_number == 0) && (susmtd == 1)) || (susmtd > 1)) {
        return true;
    } else {
        return false;
    }
}//}}}

// Store AD[13:0]
unsigned int Cadc::GetAD (const unsigned int vc_Format, const unsigned int vc_Data)
{//{{{
    unsigned int vc_AD = 0;
    switch (vc_Format) {
        case 0x000:
        case 0x001:
        case 0x030:
        case 0x031:
        case 0x050:
        case 0x051:
            vc_AD = vc_Data >> 3;
            break;
        case 0x040:
        case 0x060:
            vc_AD = vc_Data >> 2;
            break;
        case 0x041:
        case 0x061:
            vc_AD = vc_Data >> 1;
            break;
        case 0x100:
        case 0x101:
        case 0x130:
        case 0x131:
        case 0x150:
        case 0x151:
        case 0x140:
        case 0x160:
        case 0x141:
        case 0x161:
            vc_AD = vc_Data;
            break;
        case 0x200:
        case 0x201:
        case 0x230:
        case 0x231:
        case 0x250:
        case 0x251:
            vc_AD = vc_Data >> 4;
            break;
        case 0x240:
        case 0x260:
            vc_AD = vc_Data >> 3;
            break;
        case 0x241:
        case 0x261:
            vc_AD = vc_Data >> 2;
            break;
        case 0x300:
        case 0x301:
        case 0x330:
        case 0x331:
        case 0x350:
        case 0x351:
            vc_AD = vc_Data >> 6;
            break;
        case 0x340:
        case 0x360:
            vc_AD = vc_Data >> 5;
            break;
        default:
            vc_AD = vc_Data >> 4;
            break;
    }
    return vc_AD;    
}//}}}
// Check warning while change format before read
unsigned int Cadc::ChkWarning (const unsigned int Last_Format, const unsigned int Current_Format)
{//{{{
    unsigned int Current_DFMT = Current_Format >> 8;
    unsigned int Last_DFMT = Last_Format >> 8;
    unsigned int Current_CNVCLS =(Current_Format >> 4) & 0xF;
    unsigned int Last_CNVCLS = (Last_Format >> 4) & 0xF;
    unsigned int Current_ADDNT = Current_Format & 0xF;
    unsigned int Last_ADDNT = Last_Format & 0xF;
    unsigned int warningflag = 0;
    if (((Last_DFMT == 0) || (Last_DFMT == 1) || (Last_DFMT == 2)) && (Current_DFMT == 3)) {
        warningflag = 1;
    }else if (((Current_DFMT == 0) || (Current_DFMT == 1) || (Current_DFMT == 2)) && (Last_DFMT == 3)) {
        warningflag = 2;
    }else if (((Last_CNVCLS == 0) || (Last_CNVCLS == 3) || (Last_CNVCLS == 5)) && ((Current_CNVCLS == 4) || (Current_CNVCLS == 6))) {
        warningflag = 4;
    }else if (((Current_CNVCLS == 0) || (Current_CNVCLS == 3) || (Current_CNVCLS == 5)) && ((Last_CNVCLS == 4) || (Last_CNVCLS == 6))) {
        warningflag = 5;
    }else if (Current_ADDNT != Last_ADDNT) {
        warningflag = 3;
    }else{
        warningflag = 0;
    }
    return warningflag;   
}//}}}

// Get new data when change format before read
unsigned int Cadc::GetNewData (const unsigned int Current_Format, const unsigned int vc_read_number)
{//{{{
    unsigned int New_Data = 0;
    switch (Current_Format) {
        case 0x000:
        case 0x001:
        case 0x030:
        case 0x031:
        case 0x050:
        case 0x051:
            New_Data = (mAD[vc_read_number] << 3) & 0x7FF8;
            break;
        case 0x040:
        case 0x060:
            New_Data = (mAD[vc_read_number] << 2) & 0x7FFC;
            break;
        case 0x041:
        case 0x061:
            New_Data = (mAD[vc_read_number] << 1) & 0x7FFE;
            break;
        case 0x100:
        case 0x101:
        case 0x130:
        case 0x131:
        case 0x150:
        case 0x151:
            New_Data = mAD[vc_read_number] & 0x0FFF;
            break;
        case 0x140:
        case 0x160:
            New_Data = mAD[vc_read_number] & 0x1FFF;
            break;
        case 0x141:
        case 0x161:
            New_Data = mAD[vc_read_number] & 0x3FFF;
            break;
        case 0x200:
        case 0x201:
        case 0x230:
        case 0x231:
        case 0x250:
        case 0x251:
            New_Data = (mAD[vc_read_number] << 4) & 0xFFFF;
            break;
        case 0x240:
        case 0x260:
            New_Data = (mAD[vc_read_number] << 3) & 0xFFFF;
            break;
        case 0x241:
        case 0x261:
            New_Data = (mAD[vc_read_number] << 2) & 0xFFFF;
            break;
        case 0x300:
        case 0x301:
        case 0x330:
        case 0x331:
        case 0x350:
        case 0x351:
            New_Data = (mAD[vc_read_number] << 6) & 0xFFFF;
            break;
        case 0x340:
        case 0x360:
            New_Data = (mAD[vc_read_number] << 5) & 0xFFFF;
            break;
        default:
            New_Data = (mAD[vc_read_number] << 4) & 0xFFFF;
            break;
    }    
    return New_Data;
}//}}}

// Re-convert when change format before read DR
void Cadc::ReConvertDR (unsigned int addr) 
{//{{{
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        unsigned int dr_index = (unsigned int)((addr - 0x100) >> 2);
        unsigned int vc_1st_number = dr_index * 2;
        unsigned int Current_DFMT = (*ADCR2)["DFMT"]; 
        unsigned int Current_ADDNT = (*ADCR2)["ADDNT"]; 
        unsigned int Current_CNVCLS0 = ((*VCR[vc_1st_number])["CNVCLS"]); 
        unsigned int Current_CNVCLS1 = ((*VCR[vc_1st_number + 1])["CNVCLS"]); 
        unsigned int Current_Format0 = Current_DFMT<<8|Current_CNVCLS0<<4|Current_ADDNT;; 
        unsigned int Current_Format1 = Current_DFMT<<8|Current_CNVCLS1<<4|Current_ADDNT;; 
        // Check for 1st_DR
        if (((*DIR[vc_1st_number])["WFLG"] == 1) && (mflag_2nd_16bit != 1)) {
            if (Current_Format0 != mFormat_temp[vc_1st_number]) {
                re_printf("warning","The data value in DR%d, and DIR%d is change with newest format. \n", dr_index, vc_1st_number);
                unsigned int Note3_flag0 = 0;
                unsigned int New_Data0 = 0;
                //Warning Note3 for 1st DR when format change
                Note3_flag0 = ChkWarning(mFormat_temp[vc_1st_number], Current_Format0);
                if ((Note3_flag0 == 1) || (Note3_flag0 == 2)) {
                    re_printf("warning","The parity error can be occur in DR%d.DR0, and DIR%d when change data format. \n", dr_index, vc_1st_number);
                }
                if (Note3_flag0 == 3) {
                    re_printf("warning","The parity error can be occur in DR%d.DR0, and DIR%d when change the number of scanning. \n", dr_index, vc_1st_number);
                }
                if ((Note3_flag0 == 4) || (Note3_flag0 == 5)){
                    re_printf("warning","The parity error can be occur in DR%d.DR0, and DIR%d when change Conversion Class. \n", dr_index, vc_1st_number);
                }
                //Warning Note4 for 1st DR when format change
                if (mflag_1st_16bit == 1) {
                    re_printf("warning","The parity error can be occurs when DR%d.DR0 is read by 16 bits. \n", dr_index);
                }
                //Convert data with newest format
                New_Data0 = GetNewData(Current_Format0, vc_1st_number);
                (*DR[dr_index])["DR0"] = New_Data0;
                (*DIR[vc_1st_number])["DRn"] = New_Data0 ;
            }
        }
        // Check for 2nd_DR
        if (((*DIR[vc_1st_number + 1])["WFLG"] == 1) && (mflag_1st_16bit != 1)) {
            if (Current_Format1 != mFormat_temp[vc_1st_number + 1]) {
                re_printf("warning","The data value in DR%d, and DIR%d is change with newest format. \n", dr_index, (vc_1st_number + 1));
                unsigned int Note3_flag1 = 0;
                unsigned int New_Data1 = 0;
                //Warning Note3 for 2nd DR when format change
                Note3_flag1 = ChkWarning(mFormat_temp[vc_1st_number + 1], Current_Format1);
                if ((Note3_flag1 == 1) || (Note3_flag1 == 2)) {
                    re_printf("warning","The parity error can be occur in DR%d.DR1, and DIR%d when change data format. \n", dr_index, (vc_1st_number + 1));
                }
                if (Note3_flag1 == 3) {
                    re_printf("warning","The parity error can be occur in DR%d.DR1, and DIR%d when change the number of scanning. \n", dr_index, (vc_1st_number + 1));
                }
                if ((Note3_flag1 == 4) || (Note3_flag1 == 5)) {
                    re_printf("warning","The parity error can be occur in DR%d.DR1, and DIR%d when change Conversion Class. \n", dr_index, (vc_1st_number + 1));
                }
                //Warning Note4 for 2nd DR when format change
                if (mflag_2nd_16bit == 1) {
                    re_printf("warning","The parity error can be occurs when DR%d.DR1 is read by 16 bits. \n", dr_index);
                }
                //Convert data with newest format
                New_Data1 = GetNewData(Current_Format1, (vc_1st_number + 1));
                (*DR[dr_index])["DR1"] = New_Data1;
                (*DIR[vc_1st_number + 1])["DRn"] = New_Data1 ;
            }
        }
    }
}//}}}

// Re-convert when change format before read DIR
void Cadc::ReConvertDIR (unsigned int addr) 
{//{{{
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        unsigned int real_vc_number = (unsigned int)((addr - 0x200) >> 2);
        unsigned int Current_DFMT = (*ADCR2)["DFMT"]; 
        unsigned int Current_ADDNT = (*ADCR2)["ADDNT"]; 
        unsigned int Current_CNVCLS = ((*VCR[real_vc_number])["CNVCLS"]); 
        unsigned int Current_Format = Current_DFMT<<8|Current_CNVCLS<<4|Current_ADDNT; 
        // Check for DIR
        if ((*DIR[real_vc_number])["WFLG"] == 1) {
            if (Current_Format != mFormat_temp[real_vc_number]) {
                re_printf("warning","The data value in DR%d, and DIR%d is change with newest format. \n", (real_vc_number/2), real_vc_number);
                unsigned int Note3_flag = 0;
                unsigned int New_Data = 0;
                //Warning Note3
                Note3_flag = ChkWarning(mFormat_temp[real_vc_number], Current_Format);
                if ((Note3_flag == 1) || (Note3_flag == 2)) {
                    re_printf("warning","The parity error can be occur in DR%d, and DIR%d when change data format. \n", (real_vc_number/2), real_vc_number);
                }
                if (Note3_flag == 3) {
                    re_printf("warning","The parity error can be occur in DR%d, and DIR%d when change the number of scanning. \n", (real_vc_number/2), real_vc_number);
                }
                if ((Note3_flag == 4) || (Note3_flag == 5)){
                    re_printf("warning","The parity error can be occur in DR%d, and DIR%d when change Conversion Class. \n", (real_vc_number/2), real_vc_number);
                }
                //Convert data with newest format
                New_Data = GetNewData(Current_Format, real_vc_number);
                (*DIR[real_vc_number])["DRn"] = New_Data;
                if ((real_vc_number & 0x1) == 1) {
                    (*DR[real_vc_number/2])["DR1"] = New_Data;
                }else{
                    (*DR[real_vc_number/2])["DR0"] = New_Data;
                }        
            }
        }
    }
}//}}}

//call back functions
void Cadc::WritingADSYNSTCR (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(TimingCheck() && VoltageCheck()){
            unsigned int group_number = 0;
            for(unsigned int i = 1; i < (emGroupNum + 1); i++){
                group_number = emGroupNum - i;
                if((*SGCR[group_number])["ADSTARTE"] == 1){
                    double start_delay = 0;
                    if (mEnableTimeCalculation) {
                        start_delay = (emtDCoeffPCLK * mPCLKPeriod) + (emtDCoeffADCLK * mADCLKPeriod);
                    } else {
                        start_delay = mtD;
                    }
                    mWritingADSYNSTCREvent[group_number].notify((unsigned int)start_delay, SC_NS);
                }
            }
        }
    }
}//}}}

void Cadc::WritingADTSYNSTCR (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(TimingCheck() && VoltageCheck()){
            unsigned int group_number = 0;
            for(unsigned int i = 1; i <= 2; i++){
                group_number = emGroupNum - i;
                if((*SGCR[group_number])["ADTSTARTE"] == 1){
                    ADtimerCheck(group_number);
                }
            }
        }
    }
}//}}}

bool Cadc::WritingVMONVDCR (const std::string register_name, const std::string bit_name, bool bit_val)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && (!CheckAllScanGroupStop())) {
        return false;
    } else if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if (ChkAccess(true, "", register_name, bit_name)) {
            if (register_name == "VMONVDCR1") {
                mVMONVDCRVal &= 0x2;
                mVMONVDCRVal |= bit_val;
            } else {
                mVMONVDCRVal &= 0x1;
                mVMONVDCRVal |= (bit_val << 1);
            }
        }
    }
    return true;
}//}}}

void Cadc::cb_SGSTCR_SGST (RegCBstr str)
{//{{{
    unsigned int group_number = (unsigned int)(str.channel);
    if((*SGSTCR[group_number])["SGST"] == 1){
        (*SGSTCR[group_number]) = 0;
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(TimingCheck() && VoltageCheck()){
                double start_delay = 0;
                if (mEnableTimeCalculation) {
                    start_delay = (emtDCoeffPCLK * mPCLKPeriod) + (emtDCoeffADCLK * mADCLKPeriod);
                } else {
                    start_delay = mtD;
                }
                mWritingSGSTCREvent[group_number].notify((unsigned int)start_delay, SC_NS);
            }
        }
    }
}//}}}

void Cadc::cb_SGSTPCR_SGSTP (RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    if((*SGSTPCR[index])["SGSTP"] == 1){
        mSGActive[str.channel] = false;
        (*SGSR[str.channel])["SGACT"] = 0;
        (*SGSTPCR[str.channel]) = 0;
        mIsScanningProcess[str.channel] = false;
        mGroupSuspendState[str.channel] = false;
        if (mScanningState && (mCurrentGroup == str.channel)) {
            mDisableFlag = true;
            mDisableSGxFlag[str.channel] = true;
            mDisableEvent.notify();
            re_printf("info","The scanning process of scanning group %d is forced to stop.\n",str.channel);
        }
        if (str.channel >= 3) {
            if((*SGSR[str.channel])["ADTACT"] == 1) {
                (*SGSR[str.channel])["ADTACT"] = 0;
                mTIMER[str.channel - 3]->StopCounting();
                mADCStateEvent.notify();
                re_printf("info","The counting of AD timer %d is forced to stop.\n", str.channel);
            }
        }
    }  
}//}}}

void Cadc::cb_ADHALTR_HALT (RegCBstr str)
{//{{{
    if((*ADHALTR)["HALT"] == 1){
        (*ADHALTR) = 0;
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if (mScanningState) {
                mDisableFlag = true;
                for (unsigned int i = 0; i < emGroupNum; i++) {
                    mDisableSGxFlag[i] = true;
                }
                mDisableEvent.notify();
                re_printf("info","The scanning process is forced to stop.\n");
            }
            /// SARAD3
            for (unsigned int i = 3; i < emGroupNum; i++) {
                if((*SGSR[i])["ADTACT"] == 1) {
                    (*SGSR[i])["ADTACT"] = 0;
                    mTIMER[i - 3]->StopCounting();
                    mADCStateEvent.notify();
                    re_printf("info","The counting of AD timer %d is forced to stop.\n", i);
                }
            }
        }
    }
}//}}}

void Cadc::cb_MPXCURCR_MSKCFMT (RegCBstr str)
{//{{{
    bool write_condition = true;

    if (IsTargetProductE2x()) {
        write_condition = CheckAllScanGroupStop();
    } else {
        for(unsigned int group_number = 0; group_number < emGroupNum; group_number++) {
            if (
                    ((*SGSR[group_number])["SGACT"] != 0)    ||
                    ((*SGCR[group_number])["ADSTARTE"] != 0) ||
                    ((*SGCR[group_number])["TRGMD"] != 0)    ) {
                write_condition = false;
                break;
            }
        }

        if (
                ((*THACR)["HLDTE"] != 0) ||
                ((*THBCR)["HLDTE"] != 0)) {
            write_condition = false;
        }
    }


    if(write_condition){
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            unsigned int mpxcurcr = (unsigned int)(*MPXCURCR);
            unsigned int mpxcurr =  (unsigned int)(*MPXCURR);
            unsigned int mskc_num = 4;
            if ((mTargetProduct == "CCC_D1V1") || (mTargetProduct == "CCC_D1V2") ||
                    (mTargetProduct == "CCC_D2V1") || (mTargetProduct == "CCC_D2V2")) {
                mskc_num = 1;
            }
            for(unsigned int i = 0; i < mskc_num; i++){
                if((mpxcurcr & (1<<i)) == 0){
                    mpxcurr = mpxcurr & ((0xFFF0FFFF << (4*i)) | 0xFFF );
                } else {
                    // RH850_E2x added for MSKC of MSKCFMT[1]
                    mpxcurr = mpxcurr & (~(0x000F0000 << (4*i)));
                    if (IsTargetProductE2x() && (i == 1)) {
                        mpxcurr = (unsigned int)(mpxcurr | (0x00010000 << (4*i)));
                    } else {
                        mpxcurr = (unsigned int)(mpxcurr | (0x000F0000 << (4*i)));
                    }
                }
            }
            (*MPXCURR) = mpxcurr;
        }
    } else {
        (*MPXCURCR) = (unsigned int)(str.pre_data);
        re_printf("warning","Writing into MPXCURCR register is not affected because at least one scan-group is running.\n");
    }
}//}}}

void Cadc::cb_ADTSTCR_ADTST (RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    if((*ADTSTCR[index])["ADTST"] == 1){
        (*ADTSTCR[index]) = 0;
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(TimingCheck() && VoltageCheck()){
                ADtimerCheck(index);
            }
        }
    }
}//}}}

void Cadc::cb_ADTENDCR_ADTEND (RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    if((*ADTENDCR[index])["ADTEND"] == 1){
        (*ADTENDCR[index]) = 0;
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if((*SGSR[index])["ADTACT"] == 1){
                (*SGSR[index])["ADTACT"] = 0;
                mTIMER[index - 3]->StopCounting();
                mADCStateEvent.notify();
                re_printf("info","The counting of AD timer %d is stop.\n", index);
            }
        }
    }
}//}}}

void Cadc::CallBack_DRi (const unsigned int addr)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        unsigned int dr_index = (unsigned int)((addr - 0x100) >> 2);
        unsigned int vc_1st_number = dr_index * 2;
        if(mIsADPEmOperating){
            re_printf("warning","Reading DR%d register is ignored because ADPEm interrupt is in process.\n", dr_index);
        }else{
            if((*SFTCR)["RDCLRE"] == 1){
                (*DR[dr_index]) = 0;
                (*DIR[vc_1st_number]) = 0;
                (*DIR[vc_1st_number + 1]) = 0;
                re_printf("info","The DR%d, DIR%d and DIR%d registers are cleared to 0.\n", dr_index, vc_1st_number, (vc_1st_number + 1));
            }else {
                (*DIR[vc_1st_number])["WFLG"] = 0;
                (*DIR[vc_1st_number + 1])["WFLG"] = 0;
                re_printf("info","The DIR%d.WFLG and DIR%d.WFLG are cleared to 0.\n", vc_1st_number, (vc_1st_number + 1));
            }

            //call interrupt
            if(mForcedParityError){
                re_printf("info","The parity error occurs when reading DR%d.\n", dr_index);
                if((*PER)["PE"] == 0){
                    (*PER)["PE"] = 1;
                    (*PER)["PECAP"] = vc_1st_number;
                }
                if((*SFTCR)["PEIE"] == 1){
                    mADPEmActive = true;
                    mIsADPEmOperating = true;
                    mADPEmInterruptEvent.notify();
                }
            }
        }
    }
}//}}}

void Cadc::CallBack_DIRn (const unsigned int addr)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        unsigned int real_vc_number = (unsigned int)((addr - 0x200) >> 2);
        if(mIsADPEmOperating){
            re_printf("warning","Reading DIR%d register is ignored because ADPEm interrupt is in process.\n", real_vc_number);
        }else{
            if((*SFTCR)["RDCLRE"] == 1){
                if ((real_vc_number & 0x1) == 1) {
                    (*DR[real_vc_number/2])["DR1"] = 0;
                    re_printf("info","The DR%d.DR1, DIR%d registers are cleared to 0.\n", real_vc_number/2, real_vc_number);
                } else {
                    (*DR[real_vc_number/2])["DR0"] = 0;
                    re_printf("info","The DR%d.DR0, DIR%d registers are cleared to 0.\n", real_vc_number/2, real_vc_number);
                }
                (*DIR[real_vc_number]) = 0;
            }else {
                (*DIR[real_vc_number])["WFLG"] = 0;
                re_printf("info","The DIR%d.WFLG is cleared to 0.\n", real_vc_number);
            }

            //call interrupt
            if(mForcedParityError ){
                re_printf("info","The parity error occurs in VC%d.\n", real_vc_number);
                if((*PER)["PE"] == 0){
                    (*PER)["PE"] = 1;
                    (*PER)["PECAP"] = real_vc_number;
                }
                if((*SFTCR)["PEIE"] == 1){
                    mADPEmActive = true;
                    mIsADPEmOperating = true;
                    mADPEmInterruptEvent.notify();
                }
            }
        }
    }
}//}}}

void Cadc::cb_ADCR1_SUSMTD(RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && (!CheckAllScanGroupStop())) {
        re_printf("warning","Writing to ADCR1 is not affected because at least one of scan-group is running\n");
        (*ADCR1) = (unsigned int)(str.pre_data) & 0x03;
    }
}//}}}

void Cadc::cb_ADCR2_DFMT (RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && (!CheckAllScanGroupStop())) {
        re_printf("warning","Cannot write 1 into to reserved bits of register ADCR2.\n");
        (*ADCR2) = (unsigned int)(str.pre_data) & 0x31;
    }
    // Not E2x
    if ((!IsTargetProductE2x()) && (((str.data >> 5) & 0x1) == 1)) {
        re_printf("warning","Cannot write 1 into to reserved bit.\n");
        (*ADCR2) = (unsigned int)(*ADCR2) & 0xDF;
    }
}//}}}

void Cadc::cb_ECR_ULEC (RegCBstr str)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(((*ECR)["ULEC"] == 1) && ((*ULER)["ULE"] == 1)){
            (*ULER) = 0;
            re_printf("info","The ULER register is cleared to 0.\n");
        }
        if(((*ECR)["OWEC"] == 1) && ((*OWER)["OWE"] == 1)){
            (*OWER) = 0;
            re_printf("info","The OWER register is cleared to 0.\n");
        }
        if(((*ECR)["PEC"] == 1) && ((*PER)["PE"] == 1)){
            (*PER) = 0;
            re_printf("info","The PER register is cleared to 0.\n");
        }
        if (!IsTargetProductE2x()) {
            if(((*ECR)["IDEC"] == 1) && ((*IDER)["IDE"] == 1)){
                (*IDER) = 0;
                re_printf("info","The IDER register is cleared to 0.\n");
            }
        }
    }
    (*ECR) = 0;
}//}}}

void Cadc::cb_SGCR_ADIE (RegCBstr str)
{//{{{
    unsigned int group_number = (unsigned int)(str.channel);

    unsigned int adtstarte    = ((unsigned int)(str.data) & 0x80);
    unsigned int trgmd        = ((unsigned int)(str.data) & 0x3);
    unsigned int scanmd       = ((unsigned int)(str.data) & 0x20);
    unsigned int adie         = ((unsigned int)(str.data) & 0x10);
    unsigned int cur_value    = ((unsigned int)(str.data) & 0x30);


    unsigned int pre_value    = ((unsigned int)(str.pre_data) & 0x30);
    unsigned int pre_scanmd   = ((unsigned int)(str.pre_data) & 0x20);
    unsigned int pre_adie     = ((unsigned int)(str.pre_data) & 0x10);
    unsigned int pre_trgmd    = ((unsigned int)(str.pre_data) & 0x3);
    unsigned int pre_adstarte = ((unsigned int)(str.pre_data) & 0x40);

    //E2x-FCC1 & E2x-FCC2-373pins 
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        // do update scanmd when specified scan-group is on-going
        if ((pre_scanmd != scanmd) && ((*SGSR[group_number])["SGACT"] != 0)) {
            re_printf("warning","Setting SGCR%d.SCANMD is not affected because scan-group is running\n", group_number);
            if (scanmd == 0x20) {
                (*SGCR[group_number]) = str.data & 0xD3;
            }else{
                (*SGCR[group_number]) = (str.data & 0xD3) | 0x20;
            }
        }
        // do not set adie when specified scan-group is running
        if ((pre_adie == 0) && (adie > 0) && ((*SGSR[group_number])["SGACT"] != 0)) {
            re_printf("warning","Setting SGCR%d.ADIE is not affected because scan-group is running\n", group_number);
            (*SGCR[group_number]) = str.data & 0xE3;
        }
    } else if (!CheckWriteCondition(str.channel, true, pre_trgmd, pre_adstarte)) {
        if (cur_value != pre_value) {
            re_printf("warning","Writing into SGCR%d.SCANMD and SGCR%d.ADIE is not affected.\n", group_number, group_number);
        }
        (*SGCR[group_number]) = ((str.data & 0xC3) | (str.pre_data & 0x30));
    }

    if (group_number < emGroup3) {
        if ((adtstarte != 0) || (trgmd > 1)) {
            re_printf("warning","Cannot write 1 into to reserved bit.\n");
        }
        (*SGCR[group_number]) &= 0x7D;
    }
}//}}}

void Cadc::cb_SGMCYCR_MCYC (RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        if ((*SGSR[str.channel])["SGACT"] != 0) {
            re_printf("warning","Writing to SGMCYCR%d register is not affected because scan-group is running.\n", str.channel);
            (*SGMCYCR[str.channel]) = str.pre_data & 0xFF;
        }
    } else if (!CheckWriteCondition(str.channel, false, 0, 0)) {
        re_printf("warning","Writing into SGMCYCR%d register is not affected.\n", str.channel);
        (*SGMCYCR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc::cb_SGVCPR_VCSP (RegCBstr str)
{//{{{
    unsigned int start_vc = (unsigned int)((*SGVCPR[str.channel])["VCSP"]);
    unsigned int end_vc   = (unsigned int)((*SGVCPR[str.channel])["VCEP"]);
    bool write_condition = true;
    if(start_vc > end_vc){
        re_printf("warning","The start VC must be equal or less than end VC.\n");
        write_condition = false;
    } else {
        if(start_vc >= mVirChannelNum){
            re_printf("warning","The start VC must be equal or less than %d.\n", mVirChannelNum - 1);
            write_condition = false;
        }
        if(end_vc >= mVirChannelNum){
            re_printf("warning","The end VC must be equal or less than %d.\n", mVirChannelNum - 1);
            write_condition = false;
        }
    }
    if (write_condition) {
        mIsUpdateSGVCPR[str.channel] = true;
        (*SGVCSP[str.channel]) = (unsigned int)(*SGVCPR[str.channel])["VCSP"];
        (*SGVCEP[str.channel]) = (unsigned int)(*SGVCPR[str.channel])["VCEP"];
    } else {
        re_printf("warning","Writing into SGVCPR%d register is not affected.\n", str.channel);
        (*SGVCPR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc::cb_ADTIPR_ADTIP (RegCBstr str)
{//{{{
    bool write_valid = true;
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && ((*SGSR[str.channel])["ADTACT"] != 0)) {
        write_valid = false;
    }
    // E2x-FCC2-468pins
    if ((mTargetProduct == "RH850_E2x-468Pins") && (!CheckWriteCondition(str.channel, false, 0, 0))) {
        write_valid = false;
    }
    // Not E2x
    if ((!IsTargetProductE2x()) && (((*SGSR[str.channel])["ADTACT"] != 0) || ((*SGCR[str.channel])["ADTSTARTE"] != 0) || ((*SGCR[str.channel])["TRGMD"] == 3  ))) {
        write_valid = false;
    }

    if (!write_valid) {
        re_printf("warning","Writing into ADTIPR%d register is not affected.\n", str.channel);
        (*ADTIPR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc::cb_ADTPRR_ADTPR (RegCBstr str)
{//{{{
    bool write_valid = true;
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && ((*SGSR[str.channel])["ADTACT"] != 0)) {
        write_valid = false;
    }
    // E2x-FCC2-468pins
    if ((mTargetProduct == "RH850_E2x-468Pins") && ((*SGSR[str.channel])["SGACT"] != 0)) {
        write_valid = false;
    }
    // Not E2x
    if ((!IsTargetProductE2x()) && (((*SGSR[str.channel])["ADTACT"] != 0) || ((*SGCR[str.channel])["ADTSTARTE"] != 0) || ((*SGCR[str.channel])["TRGMD"] == 3  ))) {
        write_valid = false;
    }

    if (!write_valid) {
        re_printf("warning","Writing into ADTPRR%d register is not affected.\n", str.channel);
        (*ADTPRR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc::cb_SFTCR_ULEIE(RegCBstr str)
{//{{{
    // E2x (both FCC1 and FCC2)
    if (IsTargetProductE2x()) {
        if (!CheckAllScanGroupStop()) {
            re_printf("warning","Writing to SFTCR is not affected because at least one scan-group is running\n");
            (*SFTCR) = (unsigned int)(str.pre_data) & 0x17;
        } else {
            if (((str.data >> 3) & 0x1) != 0) {
                re_printf("warning","SFTCR.ULEIE bit is not existing in %s.\n", mTargetProduct.c_str());
                (*SFTCR)["ULEIE"] = 0;
            }
            if (((str.data >> 5) & 0x1) != 0) {
                re_printf("warning","SFTCR.SYNCEIE bit is not existing in %s.\n", mTargetProduct.c_str());
                (*SFTCR)["SYNCEIE"] = 0;
            }
        }
    }
}//}}}

void Cadc::cb_TDCR_TDE(RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && (!CheckAllScanGroupStop())) {
        re_printf("warning","Writing to TDCR is not affected because at least one scan-group is running\n");
        (*TDCR) = (unsigned int)(str.pre_data) & 0x83;
    }
}//}}}

void Cadc::cb_ODCR_ODDE(RegCBstr str)
{//{{{
    unsigned int odpw = (unsigned int)(str.data) & 0x0000003F;

    // E2x-FCC1 & E2x-FCC2-373pins
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        if (!CheckAllScanGroupStop()) {
            re_printf("warning","Writing to ODCR is not affected because at least one scan-group is running\n");
            (*ODCR) = (unsigned int)(str.pre_data) & 0x800000BF;
        } else {
            // check valid value [4:20] of OPDW field
            if ((odpw < 4) || (odpw > 20)) {
                re_printf("warning","Setting to ODPW is not affected because valid value is [4:20] only\n");
                (*ODCR)["ODPW"] = (unsigned int)(str.pre_data) & 0x0000003F;
            }
            // check writing to DSCE (does not existed in RH850_E2x)
            if (((unsigned int)(str.data) & 0x00008000) > 0) {
                re_printf("warning","Setting to reserved bits is not affected\n");
                (*ODCR)["DSCE"] = 0;
            }
        }
    }
}//}}}

void Cadc::cb_MPXOWR_MPXOW(RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        if (!CheckAllScanGroupStop() || ((str.data & 0x0F) > 0x0A)) {
            (*MPXOWR) = (unsigned int)(str.pre_data) & 0x0F;
            re_printf("warning","Writing to MPXOWR is not affected because at least one scan-group is running or setting value >= 10.\n");
        }
    }
}//}}}

void Cadc::cb_DFASENTSGER_DFENTSG4E(RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        if (!CheckAllScanGroupStop()) {
            (*DFASENTSGER) = (unsigned int)(str.pre_data) & 0x1F1F;
            re_printf("warning","Writing to DFASENTSGER is not affected because at least one scan-group is running.\n");
        } else {
            // check if these bits are "1" at the same time
            if ((bool)(*DFASENTSGER)["ASENTSG0E"] && (bool)(*DFASENTSGER)["DFENTSG0E"]) {
                (*DFASENTSGER)["ASENTSG0E"] = (unsigned int)(str.pre_data)  & 0x0001;
                (*DFASENTSGER)["DFENTSG0E"] = ((unsigned int)(str.pre_data) & 0x0100) >> 8;
                re_printf("warning","Writing to DFASENTSGER.ASENTSG0E and DFASENTSGER.DFENTSG0E is not affected because they are 1 at the same time.\n");
            }
            if ((bool)(*DFASENTSGER)["ASENTSG1E"] && (bool)(*DFASENTSGER)["DFENTSG1E"]) {
                (*DFASENTSGER)["ASENTSG1E"] = (unsigned int)(str.pre_data)  & 0x0002;
                (*DFASENTSGER)["DFENTSG1E"] = ((unsigned int)(str.pre_data) & 0x0200) >> 9;
                re_printf("warning","Writing to DFASENTSGER.ASENTSG1E and DFASENTSGER.DFENTSG1E is not affected because they are 1 at the same time.\n");
            }
            if ((bool)(*DFASENTSGER)["ASENTSG2E"] && (bool)(*DFASENTSGER)["DFENTSG2E"]) {
                (*DFASENTSGER)["ASENTSG2E"] = (unsigned int)(str.pre_data)  & 0x0004;
                (*DFASENTSGER)["DFENTSG2E"] = ((unsigned int)(str.pre_data) & 0x0400) >> 10;
                re_printf("warning","Writing to DFASENTSGER.ASENTSG2E and DFASENTSGER.DFENTSG2E is not affected because they are 1 at the same time.\n");
            }
            if ((bool)(*DFASENTSGER)["ASENTSG3E"] && (bool)(*DFASENTSGER)["DFENTSG3E"]) {
                (*DFASENTSGER)["ASENTSG3E"] = (unsigned int)(str.pre_data)  & 0x0008;
                (*DFASENTSGER)["DFENTSG3E"] = ((unsigned int)(str.pre_data) & 0x0800) >> 11;
                re_printf("warning","Writing to DFASENTSGER.ASENTSG3E and DFASENTSGER.DFENTSG3E is not affected because they are 1 at the same time.\n");
            }
            if ((bool)(*DFASENTSGER)["ASENTSG4E"] && (bool)(*DFASENTSGER)["DFENTSG4E"]) {
                (*DFASENTSGER)["ASENTSG4E"] = (unsigned int)(str.pre_data)  & 0x0010;
                (*DFASENTSGER)["DFENTSG4E"] = ((unsigned int)(str.pre_data) & 0x1000) >> 12;
                re_printf("warning","Writing to DFASENTSGER.ASENTSG4E and DFASENTSGER.DFENTSG4E is not affected because they are 1 at the same time.\n");
            }
        }
    }
}//}}}

void Cadc::cb_ADENDP_ENDP(RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        if (!CheckAllScanGroupStop() || ((str.data & 0x3F) > 39)) {
            (*ADENDP)[str.channel] = (unsigned int)(str.pre_data) & 0x3F;
            re_printf("warning","Writing to ADENDP is not affected because at least one scan-group is running or setting value > 39.\n");
        }
    }
}//}}}

void Cadc::cb_ULLMTBR_ULMTB(RegCBstr str)
{//{{{
    bool write_condition = true;
    for(unsigned int group =0; group < emGroupNum; group++){
        if(((*SGSR[group])["SGACT"] != 0) || ((*SGCR[group])["ADSTARTE"] != 0) || ((*SGCR[group])["TRGMD"] != 0)){
            write_condition = false;
        }
    }
    if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0)) {
        write_condition = false;
    }

    unsigned int index = (unsigned int)(str.channel);
    if(write_condition){
        unsigned int reserve_bit_31 = (unsigned int)(str.data) & 0x80000000;
        unsigned int reserve_bit_16 = (unsigned int)(str.data) & 0x00010000;
        unsigned int reserve_bit_15 = (unsigned int)(str.data) & 0x00008000;
        unsigned int reserve_bit_1  = (unsigned int)(str.data) & 0x00000001;
        if((reserve_bit_1 != 0)||(reserve_bit_15 != 0)||(reserve_bit_16 != 0)||(reserve_bit_31 != 0)){
            re_printf("warning","Cannot write 1 into to reserved bit.\n");
        }
        (*ULLMTBR[index]) &= 0x7FFE7FFE;
    }else{
        re_printf("warning","Writing into ULLMTBR%d register is not affected.\n", index);
        (*ULLMTBR[index]) = (unsigned int)(str.pre_data);
    }
}//}}}

void Cadc::cb_VCR_GCTRL(RegCBstr str)
{//{{{
    bool wr_cond = true;
    /// Changed in SARAD3
    unsigned int gctrl = str.data & 0x3F;
    unsigned int pre_gctrl = str.pre_data & 0x3F;
    unsigned int pre_cnvcls = (str.pre_data >> 13) & 0x7;

    // RH850_E2x added
    if (IsTargetProductE2x()) {
        for (unsigned int group_index = 0; group_index < emGroupNum; group_index++) {
            if (((unsigned int)(*SGVCSP[group_index]) <= str.channel) &&
                    ((unsigned int)(*SGVCEP[group_index]) >= str.channel) &&
                    (*SGSR[group_index])["SGACT"] != 0) {
                re_printf("warning","Writing into VCR%d register is not affected.\n", str.channel);
                (*VCR[str.channel]) = str.pre_data;
                wr_cond = false;
                break;
            }
        }
        if (wr_cond) {
            if (((str.data >> 17) & 0x1) != 0) {
                re_printf("warning","VCR%d.PUE bit is not existing in %s.\n", str.channel, mTargetProduct.c_str());
                (*VCR[str.channel])["PUE"] = 0;
            }
            if (((str.data >> 5) & 0x1) != 0) {
                re_printf("warning","VCR%d.GCTRL[5] bit should be set to 0 in %s.\n", str.channel, mTargetProduct.c_str());
                (*VCR[str.channel])["GCTRL"] = pre_gctrl; // GCTRL[5] = 0 in E2x
            }
            if (((str.data >> 16) & 0x1) != 0) {
                re_printf("warning","VCR%d.PDE bit is not existing in %s.\n", str.channel, mTargetProduct.c_str());
                (*VCR[str.channel])["PDE"] = 0;
            }
            if ((unsigned int)((*VCR[str.channel])["CNVCLS"]) == 1 ||
                    (unsigned int)((*VCR[str.channel])["CNVCLS"]) == 2 ||
                    (unsigned int)((*VCR[str.channel])["CNVCLS"]) == 7 ) {
                re_printf("error", "Written value 1/2/7 is out of writable value list when writing to CNVCLS\n");
                (*VCR[str.channel])["CNVCLS"] = pre_cnvcls;
            }
            if ((unsigned int)((*VCR[str.channel])["VCULLMTBS"]) == 7) {
                re_printf("warning", "VCR%d.VCULLMTBS is set to 0 due to written value 7 is prohibited.\n",str.channel);
                (*VCR[str.channel])["VCULLMTBS"] = 0;
            }
        }
    } else {
        if (((str.data >> 31) & 0x1) != 0) {
            re_printf("warning","VCR%d.VCULME bit is only existing in RH850_E2x.\n", str.channel);
            (*VCR[str.channel])["VCULME"] = 0;
        }
        if (((str.data >> 30) & 0x1) != 0) {
            re_printf("warning","VCR%d.VCLLME bit is only existing in RH850_E2x.\n", str.channel);
            (*VCR[str.channel])["VCLLME"] = 0;
        }
        if (((str.data >> 24) & 0x7) != 0) {
            re_printf("warning","VCR%d.VCULLMTBS bit is only existing in RH850_E2x.\n", str.channel);
            (*VCR[str.channel])["VCULLMTBS"] = 0;
        }
        for (unsigned int group_number = 0; group_number <emGroupNum; group_number++) {
            if (((*SGCR[group_number])["ADSTARTE"] != 0) || ((*SGCR[group_number])["TRGMD"] != 0) || ((*SGSR[group_number])["SGACT"] != 0)) {
                wr_cond = false;
                break;
            }
        }
        if (!wr_cond) { // VCR is not allowed to updated (except GCTRL)
            wr_cond = true;
            if (gctrl != pre_gctrl) { // GCTRL is updated
                for (unsigned int group_number = 0; group_number <emGroupNum; group_number++) {
                    if (((*SGVCSP[group_number])["VCSP"] <= str.channel) && (str.channel <= (*SGVCEP[group_number])["VCEP"])) {
                        if (((*SGCR[group_number])["ADSTARTE"] != 0) || ((*SGCR[group_number])["TRGMD"] != 0) || ((*SGSR[group_number])["SGACT"] != 0)) {
                            wr_cond = false;
                            break;
                        }
                    }
                }
                if ((!wr_cond) || ((pre_cnvcls != emNormalMode) && (pre_cnvcls != emDiagnosisMode) && (pre_cnvcls != emAdditionMode))) { // VCR is not allowed to updated (included GCTRL)
                    re_printf("warning","Writing into VCR%d register is not affected.\n", str.channel);
                    (*VCR[str.channel]) = str.pre_data;
                } else if (((*VCR[str.channel]) & 0xFFFFFFC0) != (str.pre_data & 0xFFFFFFC0)) { // Other bits of VCR are not allowed to updated (GCTRL is updated)
                    re_printf("warning","Writing into other bits than GCTRL in VCR%d register are not affected.\n", str.channel);
                    (*VCR[str.channel]) = (str.pre_data & ~(0x0000003F)) | ((*VCR[str.channel])["GCTRL"]);
                }
            } else { // VCR is not allowed to updated (GCTRL is not changed)
                re_printf("warning","Writing into VCR%d register is not affected.\n", str.channel);
                (*VCR[str.channel]) = str.pre_data;
            }
        }
    }
}//}}}

void Cadc::cb_SGVCSP_VCSP(RegCBstr str)
{//{{{
    bool write_valid = true;
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && ((*SGSR[str.channel])["SGACT"] != 0)) {
        write_valid = false;
    }
    if (((mTargetProduct != "RH850_E2x_fcc1") && (mTargetProduct != "RH850_E2x")) && (!CheckWriteCondition(str.channel, false, 0, 0))) {
        write_valid = false;
    }

    if (!write_valid) {
        re_printf("warning","Writing into SGVCSP%d register is not affected.\n", str.channel);
        (*SGVCSP[str.channel]) = str.pre_data;
    } else {
        if (IsTargetProductE2x()) {
            (*SGVCPR[str.channel])["VCSP"] = (*SGVCSP[str.channel]);
        }
        mIsUpdateSGVCPR[str.channel] = false;
    }
}//}}}

void Cadc::cb_SGVCEP_VCEP(RegCBstr str)
{//{{{
    bool write_valid = true;
    // E2x-FCC1 & E2x-FCC2-373pins
    if (((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) && ((*SGSR[str.channel])["SGACT"] != 0)) {
        write_valid = false;
    }
    if (((mTargetProduct != "RH850_E2x_fcc1") && (mTargetProduct != "RH850_E2x")) && (!CheckWriteCondition(str.channel, false, 0, 0))) {
        write_valid = false;
    }

    if (!write_valid) {
        re_printf("warning","Writing into SGVCEP%d register is not affected.\n", str.channel);
        (*SGVCEP[str.channel]) = str.pre_data;
    } else {
        if (IsTargetProductE2x()) {
            (*SGVCPR[str.channel])["VCEP"] = (*SGVCEP[str.channel]);
        }
        mIsUpdateSGVCPR[str.channel] = false;
    }
}//}}}

void Cadc::cb_SGVCOWR_VCOW(RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        if (!CheckAllScanGroupStop()) {
            re_printf("warning","Writing into SGVCOWR%d register is not affected.\n", str.channel);
            (*SGVCOWR[str.channel]) = str.pre_data & 0x0FFF;
        }
    } else if (!CheckWriteCondition(str.channel, false, 0, 0)) {
        re_printf("warning","Writing into SGVCOWR%d register is not affected.\n", str.channel);
        (*SGVCOWR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc::cb_MPXINTER_ADMPXIE(RegCBstr str)
{//{{{
    // E2x-FCC1 & E2x-FCC2-373pins
    if ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x")) {
        if (!CheckAllScanGroupStop()) {
            re_printf("warning","Writing into MPXINTER%d register is not affected.\n", str.channel);
            (*MPXINTER) = str.pre_data & 0x01;
        }
    }
}//}}}

void Cadc::cb_ULLMSR_ULS(RegCBstr str)
{//{{{
    if (!CheckWriteCondition(str.channel, false, 0, 0)) {
        re_printf("warning","Writing into ULLMSR%d register is not affected.\n", str.channel);
        (*ULLMSR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc::cb_SMPCR_SMPT(RegCBstr str)
{//{{{
    if (mTargetProduct == "RH850_P1M-E") {
        unsigned int smpt15_0 = (unsigned int)(*SMPCR) & 0xFFFF;
        if ((smpt15_0 != 0x0) && (smpt15_0 != 0x90CC)) {
            re_printf("warning","Setting SMPCR to value other than 0x0000 and 0x90CC is prohibited\n");
            (*SMPCR) = str.pre_data;
        }
    } else {
        bool smpts = (bool)((*SMPCR)["SMPTS"]);
        unsigned int smpt = (unsigned int)((*SMPCR)["SMPT"]);
        if (smpts) {
            if ((smpt != 0xCC) && (smpt != 0xFC) && (smpt != 0x00)) {
                re_printf("warning","Setting SMPCR.SMPT = %d is prohibited when SMPCR.SMPTS = %d.\n", smpt, smpts);
                (*SMPCR) = str.pre_data;
            }
        }
    }
}//}}}

void Cadc::cb_VCULLMTBR_VCULMTB(RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    unsigned int reserve_bit_31 = (unsigned int)(str.data) & 0x80000000;
    unsigned int reserve_bit_16 = (unsigned int)(str.data) & 0x00010000;
    unsigned int reserve_bit_15 = (unsigned int)(str.data) & 0x00008000;
    unsigned int reserve_bit_1  = (unsigned int)(str.data) & 0x00000001;
    if((reserve_bit_1 != 0)||(reserve_bit_15 != 0)||(reserve_bit_16 != 0)||(reserve_bit_31 != 0)){
        re_printf("warning","Cannot write 1 into to reserved bit of VCULLMTBR.\n");
    }
    (*VCULLMTBR[index]) &= 0x7FFE7FFE;
}//}}}

void Cadc::cb_VCLMSCR1_VC00LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<32;i++) {
        if ((((unsigned int)(*VCLMSCR1) >> i)&0x1) == 0x1) {
            (*VCLMSR1) = (unsigned int)(*VCLMSR1) & (~(1 << i));
        }
    }
    *VCLMSCR1 = 0;
}//}}}

void Cadc::cb_VCLMSCR2_VC32LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<16;i++) {
        if ((((unsigned int)(*VCLMSCR2) >> i)&0x1) == 0x1) {
            (*VCLMSR2) = (unsigned int)(*VCLMSR2) & (~(1 << i));
        }
    }
    *VCLMSCR2 = 0;
}//}}}

void Cadc::cb_ADOPDIG_ADOPDIGn0(RegCBstr str)
{//{{{
    // do nothing
}//}}}

// Check write condition of SGCRx, SGVCSPx, SGVCEPx, SGMCYCRx, ULLMSRx
bool Cadc::CheckWriteCondition (const unsigned int group_index, const bool is_SGCRwrite, const unsigned int preTRGMDvalue, const unsigned int preADSTARTEvalue)
{//{{{
    if ((mTargetProduct == "RH850_E2x") || (mTargetProduct == "RH850_E2x-468Pins")) {
        if ((*SGSR[group_index])["SGACT"] != 0) {
            return false;
        }
    } else {
        if (is_SGCRwrite) {
            if (((*SGSR[group_index])["SGACT"] != 0) || (preTRGMDvalue != 0) || (preADSTARTEvalue != 0)) {
                return false;
            }
        } else {
            if (((*SGSR[group_index])["SGACT"] != 0) || ((*SGCR[group_index])["TRGMD"] != 0) || ((*SGCR[group_index])["ADSTARTE"] != 0)) {
                return false;
            }
        }
    }

    for (unsigned int group_number = 0; group_number < emGroupNum; group_number++) {
        if ((group_number == group_index) && (is_SGCRwrite)) {
            if (preTRGMDvalue != 0) {
                return false;
            }
        } else {
            if ((*SGCR[group_number])["TRGMD"] != 0) {
                return false;
            }
        }
    }
    if (((*SGSR[group_index])["SGACT"] != 0) || ((((*THACR)["SGS"] + 1) == group_index) && ((*THACR)["HLDTE"] != 0)) ||
            ((((*THBCR)["SGS"] + 1) == group_index) && ((*THBCR)["HLDTE"] != 0))) {
        return false;
    }
    if (((is_SGCRwrite) && (preADSTARTEvalue != 0)) || ((!is_SGCRwrite) && ((*SGCR[group_index])["ADSTARTE"] != 0))) {
        return false;
    }

    return true;
}//}}}

// Check all scan-group stop
bool Cadc::CheckAllScanGroupStop (void)
{//{{{{

    for (unsigned int group_number = 0; group_number < emGroupNum; group_number++) {
        if ((*SGSR[group_number])["SGACT"] != 0) {
            return false;
        }
    }

    return true;
}//}}}

//reset functions
void Cadc::EnableReset(const bool is_active)
{//{{{
    if (mADCInstance >= mADCNum) {
        return;
    }
    if(is_active){
        //dump message
        if (mADCState == emADCIdleState) {
            re_printf("info","The ADC%d state changes from IDLE state to RESET state.\n", mADCInstance);
        }else if(mADCState == emADCTrackingState){
            re_printf("info","The ADC%d state changes from TRACKING state to RESET state.\n", mADCInstance);
        }else if(mADCState == emADCTrackingScanningState){
            re_printf("info","The ADC%d state changes from TRACKING&SCANNING state to RESET state.\n", mADCInstance);
        } else if (mADCState == emADCScanningState) {
            re_printf("info","The ADC%d state changes from SCANNING state to RESET state.\n", mADCInstance);
        }


        //initialize data members
        mADCState = emADCResetState;
        mIsReset = true;
        mResetFlag = true;
        GlobalInit();

        //cancel all events
        /// SARAD4 modify
        CancelAllEvents();

        //notify reset event
        mResetEvent.notify();

    }else{
        mIsReset = false;
        mADCState = emADCIdleState;
        re_printf("info","The ADC%d state changes from RESET state to IDLE state.\n", mADCInstance);
    }

    //reset register
    Cadc_regif::EnableReset(is_active);

    //reset timer
    for(unsigned int index = 0; index < 2; index++){
        mTIMER[index]->EnableReset(is_active);
    }
}//}}}

void Cadc::GlobalInit(void)
{//{{{
    //disable
    mDisableFlag = false;
    //T&H circuit
    for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
        mTHSamplingStartTime [TH_channel] = 0;
        mTHChannelState [TH_channel] = emTHIdleState;
        mTHUpdateChannelState [TH_channel] = false;
        mIntTHSample [TH_channel] = 0;
        mTHUpdateInput [TH_channel] = false;
    }
    mTHUpdateChannelStateFlag = false;
    mTHStop = false;
    mTHSamplingManual = false;
    mTHEnable = false;
    mTHASoftwareTriggerHolding = false;
    mTHBSoftwareTriggerHolding = false;
    mTHAPortTriggerHolding = false;
    mTHBPortTriggerHolding = false;
    mTHAHoldingProcess = false;
    mTHBHoldingProcess = false;
    mGroupANumber = 0;
    mGroupBNumber = 0;

    //output
    mINT_MPXmActive = false;
    mADPEmActive = false;
    mErrorId = 0;
    mIsADPEmOperating = false;
    mINT_ADmxINT_ADEmDataActive = false;
    mID = 0;
    mStoreData = 0;
    mFinalData = 0;
    mINT_MPXmActiveNum = 0;
    mINT_ADEmActiveNum = 0;
    mADPEmActiveNum = 0;
    mINT_ADm0ActiveNum = 0;
    mINT_ADm1ActiveNum = 0;
    mINT_ADm2ActiveNum = 0;
    mINT_ADm3ActiveNum = 0;
    mINT_ADm4ActiveNum = 0;
    mflag_1st_16bit = 0;
    mflag_2nd_16bit = 0;
    for(unsigned int vc_number = 0; vc_number < 40; vc_number++){
        mFormat_temp[vc_number] = 0;
        mAD[vc_number] = 0;
    }
    //state
    mADCChangeState = false;
    mIsINT_MPXmOperating = false;

    for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
        mINT_ADmxActive [group_number]     = false;
        mADENDmxActive [group_number]      = false;
        mEndVCPointer [group_number]       = 0;
        mStartVCPointer [group_number]     = 0;
        mSuspendVC [group_number]          = 0;
        mLastMultipleNumber [group_number] = 0;
        mGroupSuspendState [group_number]  = false;
        mIsUpdateSGVCPR[group_number]      = false;
        mSGActive [group_number]           = false;
        mIsScanningProcess[group_number]   = false;
        mDisableSGxFlag[group_number]      = false;
        /// SARAD3
        mIsHWTrigger[group_number] = false;
    }
    mCurrentGroup = 0;
    mLastGroup = 0xFFFF;
    mFinalGroup = 0;
    mLastVC = mVirChannelNum;
    mFinalVC = 0;
    mVMONVDCRVal = 0;
    mConvtData = 0;
    mULMTB = 0;
    mLLMTB = 0;
    mCurVC = 0;
    mSuspendFlag = false;
    mScanningState = false;
    mOpenCircuitError = false;
}//}}}

//information function
void Cadc::DumpInfo (const char *type, const char *message, ... )
{//{{{
    if((message != NULL)&&(type != NULL)){
        printf ("PROFILE(%s): SAR AD: ADC%d: ", type, mADCInstance);
        //print body
        va_list argptr;
        va_start(argptr, message);
        vprintf(message, argptr);
        va_end(argptr);
    }
}//}}}

void Cadc::DumpStatInfo (void)
{//{{{
//1.Dump information
    if (mADCInstance < mADCNum) {
        DumpInfo("StatInfo", "Info[%s](ADC%d).\n",sc_time_stamp().to_string().c_str(), mADCInstance);
        DumpInfo("StatInfo", " INT_MPX%d active number : %d\n", mADCInstance, mINT_MPXmActiveNum);
        DumpInfo("StatInfo", " ADPE%d active number : %d\n", mADCInstance, mADPEmActiveNum);
        DumpInfo("StatInfo", " INT_ADE%d active number : %d\n", mADCInstance, mINT_ADEmActiveNum);
        DumpInfo("StatInfo", " INT_AD%d0 active number : %d\n", mADCInstance, mINT_ADm0ActiveNum);
        DumpInfo("StatInfo", " INT_AD%d1 active number : %d\n", mADCInstance, mINT_ADm1ActiveNum);
        DumpInfo("StatInfo", " INT_AD%d2 active number : %d\n", mADCInstance, mINT_ADm2ActiveNum);
        DumpInfo("StatInfo", " INT_AD%d3 active number : %d\n", mADCInstance, mINT_ADm3ActiveNum);
        DumpInfo("StatInfo", " INT_AD%d4 active number : %d\n", mADCInstance, mINT_ADm4ActiveNum);
        DumpInfo("StatInfo", "EndInfo.\n");
    }

    //2.Reset information variable
    mINT_MPXmActiveNum = 0;
    mADPEmActiveNum   = 0;
    mINT_ADEmActiveNum    = 0;
    mINT_ADm0ActiveNum   = 0;
    mINT_ADm1ActiveNum   = 0;
    mINT_ADm2ActiveNum   = 0;
    mINT_ADm3ActiveNum   = 0;
    mINT_ADm4ActiveNum   = 0;
}//}}}

void Cadc::DumpADCActivity (const double start_time, const double end_time)
{//{{{
    if(mEnableConvertInfo){
        re_printf("info","Start simulation time: %lld.%06d.\n", (unsigned long long) start_time, FRACTION_TO_INT(start_time));
        re_printf("info","End simulation time: %lld.%06d.\n", (unsigned long long) end_time, FRACTION_TO_INT(end_time));
        re_printf("info","The virtual channel number: %d.\n", mFinalVC);
        re_printf("info","The value of virtual channel register VCR: 0x%X.\n", (unsigned int)(*VCR[mFinalVC]));
        re_printf("info","The scanning group: %d.\n", mFinalGroup);
    }
}//}}}

void Cadc::DumpInterruptMessage (const std::string intr_name, const std::string intr_number, const std::string issue)
{//{{{
    if(mDumpInterrupt){
        re_printf("info","INT [SAR AD: %s%d%s] %s.\n", intr_name.c_str(), mADCInstance, intr_number.c_str(), issue.c_str());
    }
}//}}}

/// SARAD3
void Cadc::HWTriggerScanningMethod (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        for (unsigned int i = 0; i < emGroupNum; i++) {
            if (mIsHWTrigger[i]) {
                GroupCheck(i);
                mIsHWTrigger[i] = false;
            }
        }
    }
}//}}}

/// SARAD4 add - fix 0-frequency issue
void Cadc::SettingZeroClock (void)
{//{{{
    // Cancel all events
    CancelAllEvents();

    // Stop operation
    mZeroClockEvent.notify();

    // Disable all Timers
    mTIMER[0]->StopCounting();
    mTIMER[1]->StopCounting();
}//}}}

void Cadc::CancelAllEvents (void)
{//{{{
    mScanningEvent.cancel();
    mSuspendEvent.cancel();
    for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
        mADENDmxEvent[group_number].cancel();
        mWritingADSYNSTCREvent[group_number].cancel();
        mWritingSGSTCREvent[group_number].cancel();
        if (group_number >= emGroup3) {
            mTIMER[group_number - 3]->mADtimerEndCountingEvent[group_number - 3].cancel();
        }
        mHWTriggerEvent[group_number].cancel();
    }
    //disable event
    mDisableEvent.cancel();
    //TH event
    mTHSamplingEvent.cancel();
    mTHStopEvent.cancel();
    mTHEnableEvent.cancel();
    mTHASoftwareTriggerHoldingEvent.cancel();
    mTHBSoftwareTriggerHoldingEvent.cancel();
    mTHAPortTriggerHoldingEvent.cancel();
    mTHBPortTriggerHoldingEvent.cancel();
    mTHUpdateChannelStateEvent.cancel();
    mTHUpdateInputEvent.cancel();
    mTHGAHoldingEvent.cancel();
    mTHGBHoldingEvent.cancel();
    //output event
    mINT_ADmxINT_ADEmDataEvent.cancel();
    mINT_MPXmInterruptEvent.cancel();
    mADMPXOutputEvent.cancel();
    mADPEmInterruptEvent.cancel();
    //state event
    mADCChangeStateEvent.cancel();
    mADCStateEvent.cancel();
}//}}}

void Cadc::SetULErrorStatus(unsigned int channel) // RH850_E2x added for set upper/lower bound error status
{//{{{
    if (channel < 32) {
        (*VCLMSR1) = (unsigned int)(*VCLMSR1) | (1 << channel);
    } else {
        (*VCLMSR2) = (unsigned int)(*VCLMSR2) | (1 << (channel-32));
    }
}//}}}

bool Cadc::ULIntrEnable(unsigned int channel) // RH850_E2x added for check upper/lower bound error interrupt enable
{//{{{
    if (channel < 32) {
        if (((unsigned int)(*VCLMINTER1) & (1 << channel)) != 0) {
            return true;
        }
    } else {
        if (((unsigned int)(*VCLMINTER2) & (1 << (channel-32))) != 0) {
            return true;
        }
    }
    return false;
}//}}}

/*********************************
// Function     : GetCurTime
// Description  : Get current simulation time
// Parameter    : None
// Return value : Current simulation time (unit SC_NS)
**********************************/
double Cadc::GetCurTime(void)
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

bool Cadc::IsTargetProductE2x(void)
{//{{{
    return ((mTargetProduct == "RH850_E2x_fcc1") || (mTargetProduct == "RH850_E2x") || (mTargetProduct == "RH850_E2x-468Pins"));
}//}}}

//Constructor of Ctimer class
Cadc::Ctimer::Ctimer(sc_module_name name, const unsigned int timer_num): Cgeneral_timer(name)
{//{{{
    mStartValue = 0;
    mNextValue = 0;
    mPeriod = 0;
    mTimerNum = timer_num;
    InitializeGeneralTimer(0,0);
    SC_METHOD(CountingMethod);
    dont_initialize();
    sensitive<<mCountingEvent;
}//}}}

//destructor of Ctimer class
Cadc::Ctimer::~Ctimer()
{//{{{
}//}}}

//Functions
void Cadc::Ctimer::InitializeGeneralTimer(const unsigned int start_value, const unsigned int end_value)
{//{{{
    Cgeneral_timer::setSTR(false);                    //Count permission
    Cgeneral_timer::setCMS(16U);
    Cgeneral_timer::setCMM(emOneShot);                //Operation mode
    Cgeneral_timer::setCounterDirection(emCountDown); //Count mode
    Cgeneral_timer::setCCLR(emInFactor);              //Clear mode
    Cgeneral_timer::setTimeUnit(SC_NS);               //Time unit
    Cgeneral_timer::setCOR(end_value);                //Match value
    Cgeneral_timer::setCNT(start_value);              //Count value
    Cgeneral_timer::setCKS(emClkDivide);              //Divide Input clock
}//}}}

void Cadc::Ctimer::EnableReset (const bool is_active)
{//{{{
    mCountingEvent.cancel();
    Cgeneral_timer::EnableReset(is_active);
}//}}}

void Cadc::Ctimer::cmpMatchOutputHandling( bool value)
{//{{{
    StartCounting(mNextValue);
    mADtimerEndCountingEvent[mTimerNum].notify();
}//}}}

void Cadc::Ctimer::StartCounting (const unsigned int start_value)
{//{{{
    mStartValue = start_value;
    mCountingEvent.notify(SC_ZERO_TIME);
}//}}}

void Cadc::Ctimer::CountingMethod()
{//{{{
    InitializeGeneralTimer(mStartValue, 0);
    Cgeneral_timer::setClkCountPeriod(mPeriod);
    Cgeneral_timer::setSTR(true);
}//}}}

void Cadc::Ctimer::StopCounting ()
{//{{{
    Cgeneral_timer::setSTR(false);
}//}}}
// vim600: set foldmethod=marker :
