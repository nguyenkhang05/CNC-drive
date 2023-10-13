// -----------------------------------------------------------------------------
// $Id$
//
// Copyright(c) 2017-2020 Renesas Electronics Corporation
// Copyright(c) 2017-2020 Renesas Design Vietnam Co., Ltd.
// RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
// This program must be used solely for the purpose for which
// it was furnished by Renesas Electronics Corporation. No part of this
// program may be reproduced or disclosed to others, in any
// form, without the prior written permission of Renesas Electronics
// Corporation.
// -----------------------------------------------------------------------------

#include "adc_u2a.h"
//Constructor of ADC class
Cadc_u2a::Cadc_u2a(sc_module_name name, unsigned int adc_instance):
         Cadc_u2a_regif((std::string)name, 32)

        //trigger input port
        ,ADmSG0TRG("ADmSG0TRG")
        ,ADmSG1TRG("ADmSG1TRG")
        ,ADmSG2TRG("ADmSG2TRG")
        ,ADmSG3TRG("ADmSG3TRG")
        ,ADmSG4TRG("ADmSG4TRG")
        ,PVCR_VALUEm("PVCR_VALUEm")
        ,PVCR_TRGm("PVCR_TRGm")
        ,ADmTAUD2I7("ADmTAUD2I7")
        ,ADmTAUD2I15("ADmTAUD2I15")
        ,ADmTAUJ2("ADmTAUJ2")
        ,ADmTAUJ3("ADmTAUJ3")
        ,ADmLPS("ADmLPS")

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
        ,ANm80("ANm80")
        ,ANm81("ANm81")
        // RH850_E2x added
        ,IFVCHm("IFVCHm")
        ,IFRDm("IFRDm")
        ,IFREm("IFREm")
        ,SV_MODEm("SV_MODEm")
        //interrupt ouptut port
        ,INT_ADm0("INT_ADm0")
        ,INT_ADm1("INT_ADm1")
        ,INT_ADm2("INT_ADm2")
        ,INT_ADm3("INT_ADm3")
        ,INT_ADm4("INT_ADm4")
        ,INT_MPXm("INT_MPXm")
        ,ADPEm("ADPEm")
        //data output port
        ,ADENDm0("ADENDm0")
        ,ADENDm1("ADENDm1")
        ,ADENDm2("ADENDm2")
        ,ADENDm3("ADENDm3")
        ,ADENDm4("ADENDm4")
        // RH850_E2x added
#if 0
        // RH850_U2A added
        ,INT_ADEm("INT_ADEm")
        ,INT_ULm("INT_ULm")
#else
        ,INT_ERRm("INT_ERRm")
#endif
        ,DFREQm("DFREQm")
        ,DFTAGm("DFTAGm")
        ,DFDATAm("DFDATAm")
        ,ADCm_IFDATA("ADCm_IFDATA")
        ,PVCR_ENDm("PVCR_ENDm")
        ,PVCR_PWDDRm("PVCR_PWDDRm")
        ,PWM_VCEND("PWM_VCEND")
        ,VCULMO("VCULMO")
        ,VCLLMO("VCLLMO")
        ,MPXCURm("MPXCURm")
        ,ULE_LPSm("ULE_LPSm")
{//{{{
    //initial handleCommand
    CommandInit(this->name());
    Cadc_u2a_regif::set_instance_name(this->name());

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
//    mHWTriggerSelectNum = 5;
    mADCInstance = adc_instance;
    mResetFlag = false;
    mIsPReset = false;
    mIsADReset = false;
    mAvrefh = 3.3;
    mAVcc = 3.3;
    mEVcc = 3.3;
    mForcedIdError = false;
    mForcedParityError = false;
    mForcedOpenError = false;
    mForcedBreak = false;
    mForcedDataPathError = false;
    mtD = 0;
    mtED = 0;
    mExSmpt = 60;
    mSmpt = 18;
    mtSAR = 0; //Confirm
    mtTHLD = 10;
    mtTHSD = 5;
    mtTSPL = 27;
    mPCLKPeriod = 0;
    mADCLKPeriod = 0;
    mDumpInterrupt = false;
    mEnableConvertInfo = false;
    mEnableTimeCalculation = true;
#if 0
    THzAssignChannel[0] = 1;   // Assign T&H0 to ANm01
    THzAssignChannel[1] = 2;   // Assign T&H1 to ANm02
    THzAssignChannel[2] = 3;   // Assign T&H2 to ANm03
    THzAssignChannel[3] = 29;  // Assign T&H3 to ANm71
    THzAssignChannel[4] = 30;  // Assign T&H4 to ANm72
    THzAssignChannel[5] = 31;  // Assign T&H5 to ANm73
#else
    // RH850_U2A added
    THzAssignChannel[0] = 0;  // Assign T&H0 to ANm00
    THzAssignChannel[1] = 1;  // Assign T&H1 to ANm01
    THzAssignChannel[2] = 2;  // Assign T&H2 to ANm02
    THzAssignChannel[3] = 3;  // Assign T&H3 to ANm03
#endif
    /// SARAD3
    mAVccMax = 3.6;
    mAVccMin = 3;
    mEVccMax = 3.6;
    mEVccMin = 3;
    mAvrefhMin = 3;
    /// SARAD4 add
    mADCNum = 2;
    mPhyChannelNum = 32;
    mVirChannelNum = 96;
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
#if 0
    //INT_ADEm.initialize(0);
    //INT_ULm.initialize(0);
#else
    // RH850_U2A added
    INT_ERRm.initialize(0);
#endif
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
    PWM_VCEND.initialize(0);
    VCULMO.initialize(0);
    VCLLMO.initialize(0);

    //ADSTART_O.initialize(0);
    //ADTSTART_O.initialize(0);

    PVCR_ENDm.initialize(0);
    PVCR_PWDDRm.initialize(0);

    MPXCURm.initialize(0);
    ULE_LPSm.initialize(0);

    //scanning method
    SC_THREAD(ScanningThread);
    dont_initialize();
    sensitive << mScanningEvent;
    sensitive << mADResetEvent; // adclk

    SC_THREAD(BoundaryExcessThread);
    dont_initialize();
    sensitive << mBoundaryExcessEvent;

    SC_THREAD(VCENDNoticeThread);
    dont_initialize();
    sensitive << mVCENDNoticeEvent;

#if 0
    SC_THREAD(INT_ADEmThread);
    dont_initialize();
    sensitive << mINT_ADEmEvent;
    sensitive << mPResetEvent; // pclk
#else
    // RH850_U2A added
    SC_METHOD(ULE_LPSmMethod);
    dont_initialize();
    sensitive << mULE_LPSmEvent;
    sensitive << mPResetEvent;

    SC_THREAD(INT_ERRmThread);
    dont_initialize();
    sensitive << mINT_ERRmEvent;
    sensitive << mPResetEvent;
#endif

    //interrupt output method
    SC_METHOD(INT_ADmxINT_ADEmDataMethod);
    dont_initialize();
    sensitive << mINT_ADmxINT_ADEmDataEvent;
    sensitive << mPResetEvent; // pclk

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
    #if 0 //Update for HWM0.61 - T&H Group B is removed
    SC_METHOD(THBHoldingEndMethod);
    dont_initialize();
    sensitive << mTHGBHoldingEvent;
    #endif
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
            case 32:
                otp.set_sensitivity(&ANm80);
                break;
            case 33:
                otp.set_sensitivity(&ANm81);
                break;
            default:
                break;
        }
        sc_core::sc_spawn(sc_bind(&Cadc_u2a::THzMethod, this, i), sc_core::sc_gen_unique_name("THz_Method"), &otp);
    }
    
    // Method for HW trigger select of ADC2
    for (unsigned int i = 0; i < emHWTriggerSelectNum; i++) {
        sc_core::sc_spawn_options otp;
        otp.spawn_method();
        switch (i){
            case 0:
                otp.set_sensitivity(&ADmTAUD2I7);
                break;
            case 1:
                otp.set_sensitivity(&ADmTAUD2I15);
                break;
            case 2:
                otp.set_sensitivity(&ADmTAUJ2);
                break;
            case 3:
                otp.set_sensitivity(&ADmTAUJ3);
                break;
            case 4:
                otp.set_sensitivity(&ADmLPS);
                break;
            default:
                break;
        }
        sc_core::sc_spawn(sc_bind(&Cadc_u2a::TriggerADC2Method, this, i), sc_core::sc_gen_unique_name("TriggerADC2_Method"), &otp);
    }

    //scanning software trigger method
    SC_METHOD(ScanGroup0SGSTTriggerMethod);
    dont_initialize();
    sensitive << mWritingSGSTCREvent[emGroup0];

    SC_METHOD(ScanGroup1SGSTTriggerMethod);
    dont_initialize();
    sensitive << mWritingSGSTCREvent[emGroup1];

    SC_METHOD(ScanGroup2SGSTTriggerMethod);
    dont_initialize();
    sensitive << mWritingSGSTCREvent[emGroup2];

    SC_METHOD(ScanGroup3SGSTTriggerMethod);
    dont_initialize();
    sensitive << mWritingSGSTCREvent[emGroup3];

    SC_METHOD(ScanGroup4SGSTTriggerMethod);
    dont_initialize();
    sensitive << mWritingSGSTCREvent[emGroup4];

    SC_METHOD(ScanGroup0ADSYNTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup0];

    SC_METHOD(ScanGroup1ADSYNTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup1];

    SC_METHOD(ScanGroup2ADSYNTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup2];

    SC_METHOD(ScanGroup3ADSYNTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup3];

    SC_METHOD(ScanGroup4ADSYNTriggerMethod);
    dont_initialize();
    sensitive << mWritingADSYNSTCREvent[emGroup4];

    //scanning software trigger method
    SC_METHOD(ScanGroup0SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mSGSTTriggerEvent [emGroup0];
    sensitive << mADSYNTriggerEvent[emGroup0];

    SC_METHOD(ScanGroup1SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mSGSTTriggerEvent [emGroup1];
    sensitive << mADSYNTriggerEvent[emGroup1];

    SC_METHOD(ScanGroup2SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mSGSTTriggerEvent [emGroup2];
    sensitive << mADSYNTriggerEvent[emGroup2];

    SC_METHOD(ScanGroup3SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mSGSTTriggerEvent [emGroup3];
    sensitive << mADSYNTriggerEvent[emGroup3];

    SC_METHOD(ScanGroup4SoftwareTriggerMethod);
    dont_initialize();
    sensitive << mSGSTTriggerEvent [emGroup4];
    sensitive << mADSYNTriggerEvent[emGroup4];


    //counting end method
    SC_METHOD(ScanGroup3ADtimerTriggerMethod);
    dont_initialize();
    sensitive << mTIMER[0]->mADtimerEndCountingEvent[0];

    SC_METHOD(ScanGroup4ADtimerTriggerMethod);
    dont_initialize();
    sensitive << mTIMER[1]->mADtimerEndCountingEvent[1];


    SC_METHOD(INT_MPXmInterruptMethod);
    dont_initialize();
    sensitive << mINT_MPXmInterruptEvent;
    sensitive << mPResetEvent; // pclk

    SC_METHOD(ADMPXOutputMethod);
    dont_initialize();
    sensitive << mADMPXOutputEvent;
    sensitive << mPResetEvent; // pclk

    SC_METHOD(ADPEmInterruptMethod);
    dont_initialize();
    sensitive << mADPEmInterruptEvent;
    sensitive << mPResetEvent; // pclk

    SC_METHOD(ADENDm0Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup0];
    sensitive << mADResetEvent; // adclk

    SC_METHOD(ADENDm1Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup1];
    sensitive << mADResetEvent; // adclk

    SC_METHOD(ADENDm2Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup2];
    sensitive << mADResetEvent; // adclk

    SC_METHOD(ADENDm3Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup3];
    sensitive << mADResetEvent; // adclk

    SC_METHOD(ADENDm4Method);
    dont_initialize();
    sensitive << mADENDmxEvent[emGroup4];
    sensitive << mADResetEvent; // adclk

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

    SC_METHOD(SVmModeMethod);
    dont_initialize();
    sensitive << SV_MODEm;

    SC_METHOD(SVStopMethod);
    dont_initialize();
    sensitive << mSVModeEvent;

    /// SARAD3
    SC_METHOD(HWTriggerScanningMethod);
    dont_initialize();
    sensitive << mHWTriggerEvent[emGroup0];
    sensitive << mHWTriggerEvent[emGroup1];
    sensitive << mHWTriggerEvent[emGroup2];
    sensitive << mHWTriggerEvent[emGroup3];
    sensitive << mHWTriggerEvent[emGroup4];

    SC_METHOD(PVCR_TRGmMethod);
    dont_initialize();
    sensitive << PVCR_TRGm;

}//}}}

//destructor of Cadc_u2a class
Cadc_u2a::~Cadc_u2a()
{//{{{
    delete mTIMER[0];
    delete mTIMER[1];
}//}}}
//virtual channel end notice thread
void Cadc_u2a::VCENDNoticeThread(void)
{//{{{
    while(true){
        if(mCurVC == emVcPWM){
            PWM_VCEND->write(1);
        }
        if(mCurVC < mVirChannelNum){
            VCEND[mCurVC]->write(1);
        }
        wait(sc_time((unsigned int)mPCLKPeriod, SC_NS));

        if(mCurVC == emVcPWM){
            PWM_VCEND->write(0);
        }
        if(mCurVC < mVirChannelNum){
            VCEND[mCurVC]->write(0);
        }
        mCurVC = 0;
        wait();
    }
}//}}}

//boundary excess thread
void Cadc_u2a::BoundaryExcessThread(void)
{//{{{
    while(true){
        if(mIsPReset) {
            VCULMO.write(0);
            VCLLMO.write(0);
            mULMTB = false;
            mLLMTB = false;
        }else {
            if (mULMTB) {
                VCULMO.write(1);
            }
            if (mLLMTB) {
                VCLLMO.write(1);
            }
            wait(sc_time(mPCLKPeriod, SC_NS));

            VCULMO.write(0);
            VCLLMO.write(0);
            mULMTB = false;
            mLLMTB = false;
        }
        wait();
    }
}//}}}

//scanning method
void Cadc_u2a::ScanningThread(void)
{//{{{
    for(;;){
        if(mIsADReset || (mADCInstance >= mADCNum)){
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
                            bool is_start_SG = true;
                            mScanningState = true;
                            mCurrentGroup  = (unsigned int)(group_number);

                            //2.Get setting mode of current group
                            unsigned int group_loop = 0;
                            bool is_multiple_mode = true;
                            if(group_number == emGroup4
                             &&mStartVCPointer[group_number] == emVcPWM
                             &&mEndVCPointer[group_number] == emVcPWM){
                                is_multiple_mode = true;
                                group_loop = 1;
                            }else if((*SGCR[group_number])["SCANMD"] == 1){
                                is_multiple_mode = false;
                            }else if(mGroupSuspendState[group_number]){                      //start at the next scanning sequence for suspend group
                                group_loop = mLastMultipleNumber[group_number];
                                is_start_SG = mIsStartSG[group_number];
                            }else{
                                group_loop = (unsigned int)(*SGMCYCR[group_number]) + 1;    //start at begining of scanning sequence
                            }
                            //3.Scanning for group
                            bool group_active_flag = true;
                            while(group_active_flag){
                                bool is_final_vc           = false;
                                bool is_suspend_immediate  = false;
                                unsigned int start_vc = 0;
                                unsigned int end_vc = mEndVCPointer[group_number];
                                //3.1.Get the start
                                if(mGroupSuspendState[group_number]){
                                    mGroupSuspendState[group_number] = false;
                                    start_vc = mSuspendVC[group_number];
                                }else {
                                    start_vc = mStartVCPointer[group_number];
                                }

                                //3.2.Scanning for VC
                                for(unsigned int vc_number = start_vc; vc_number < (end_vc + 1); vc_number++){
                                    if(VCCheck(vc_number)){
                                        double start_time = GetCurTime();
                                        unsigned int vc_add_number = 0;

                                        vc_add_number = VCAddition(vc_number);

                                        unsigned int data[4] = {0,0,0,0};
                                        // RH850_E2x added wait insertion for virtual channel (not start virtual channel of scanning group)
                                        // RH850_E2x Enhancement
                                        unsigned int gctrl_val  = 0;
                                        if(vc_number == emVcPWM && group_number == emGroup4) {
                                            gctrl_val = (unsigned int)((*PWDVCR)["GCTRL"]);
                                        }
                                        if(vc_number < mVirChannelNum){
                                            gctrl_val = (unsigned int)((*VCR[vc_number])["GCTRL"]);
                                        }
                                        //unsigned int index_val  = gctrl_val & 0x1F;
                                        unsigned int index_val  = gctrl_val;
                                        if (!(((mVMONVDCRVal == 2) || (mVMONVDCRVal == 1))
                                            && ((index_val == emVCCPortNum) || (index_val == emEVCCPortNum)
                                             || (index_val == emISOVDDPortNum) || (index_val == emAWOVDDPortNum)))) {

                                            if(vc_number == emVcPWM && group_number == emGroup4) {
                                                re_printf("info","The PWM Diag of group %d starts the conversion.\n", group_number);
                                            }
                                            if(vc_number < mVirChannelNum){
                                                re_printf("info","The VC %d of group %d starts the conversion.\n", vc_number, group_number);
                                            }
                                            //3.2.0.Assert INT_MPXm, ADENDmx, MPXCURR
                                            ExportData(emStartConvert, (unsigned int)(group_number), vc_number, data, group_loop);

                                            for(unsigned int vc_add = 0; vc_add < vc_add_number; vc_add++){
                                                // --> Confirm
                                                double delay_time = ADCTimeCalculation(is_start_SG, vc_number, vc_add);
                                                // <-- Confirm
                                                /// SARAD4 modify - fix 0-frequency issue
                                                // --> Confirm
                                                if (!mTHAHoldingProcess && !mTHBHoldingProcess) {
                                                    // wait tSPL + tWAITTIME
                                                    wait((unsigned int)delay_time, SC_NS, mSuspendEvent | mDisableEvent | mADResetEvent | mZeroClockEvent);
                                                    // <-- Confirm
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
                                                    mIsStartSG[group_number] = is_start_SG;
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
                                                    delay_time = emtSARCoeff * mADCLKPeriod;
                                                } else {
                                                    delay_time = mtSAR;
                                                }
                                                /// SARAD4 modify - fix 0-frequency issue
                                                if (!mTHAHoldingProcess && !mTHBHoldingProcess) {
                                                    // wait tSAR
                                                    wait((unsigned int)delay_time, SC_NS, mSuspendEvent | mDisableEvent | mADResetEvent | mZeroClockEvent);
                                                } else {
                                                    mSuspendFlag = true;
                                                }

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
                                                    mIsStartSG[group_number] = is_start_SG;
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
                                                    if(vc_number == emVcPWM && group_number == emGroup4) {
                                                        re_printf("info","The PWM Diag of group %d finishes the conversion.\n", group_number);
                                                    }
                                                    if(vc_number < mVirChannelNum){
                                                        re_printf("info","The VC %d of group %d finishes the conversion.\n", vc_number, group_number);
                                                    }
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
                                                    if(is_start_SG) {
                                                        is_start_SG = false;
                                                    }
                                                }
                                            }//end for VC addition number
                                        }
                                    }else{
                                        re_printf("warning","The current VC %d is bypassed.\n", vc_number);
                                    }
                                    /// SARAD4 modify - fix 0-frequency issue
                                    if(mDisableFlag || mDisableOneSGFlag || mSuspendFlag || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                        break; //break out of for(vc_number ...)
                                    }
                                }//end for(unsigned int vc_num = start_vc)

                                //3.3.Check to continue the current group or not
                                /// SARAD4 modify - fix 0-frequency issue
                                if(mDisableFlag || mDisableOneSGFlag || is_suspend_immediate || (mSuspendFlag && (!is_final_vc)) || (mPCLKPeriod == 0) || (mADCLKPeriod == 0)) {
                                    if(mDisableOneSGFlag) {
                                        mDisableOneSGFlag = false;
                                        mGroupSuspendState[group_number] = false;
                                        (*SGSR[group_number])["SGACT"] = 0;
                                        (*SGSTR)["SGACT"] =(unsigned int)((*SGSTR)["SGACT"]) & (~(0x1 << group_number));
                                        mIsScanningProcess[group_number] = false;
                                        mSGActive[group_number] = false;
                                        re_printf("info","The group %d finishes the scanning process.\n", group_number);
                                        group_active_flag = false;
                                    }
                                    //stop scanning process/ Suspend asynchronous/ Suspend sysnchonous not at the final VC
                                    break; //break out of while(group_active_flag)
                                }else{
                                    if(is_multiple_mode){
                                        group_loop = group_loop - 1;
                                        mLastMultipleNumber[group_number] = group_loop;
                                        if(group_loop == 0){
                                            mGroupSuspendState[group_number] = false;
                                            (*SGSR[group_number])["SGACT"] = 0;
                                            (*SGSTR)["SGACT"] =(unsigned int)( (*SGSTR)["SGACT"]) & (~(0x1 << group_number));
                                            mIsScanningProcess[group_number] = false;
                                            mSGActive[group_number] = false;
                                            re_printf("info","The group %d finishes the scanning process.\n", group_number);
                                            group_active_flag = false;
                                        }else if(!mSuspendFlag) {
                                            re_printf("info","The group %d is continued the scanning process at the start VC in multiple scanning mode.\n", group_number);
                                        }
                                    }else if(!mSuspendFlag) {
                                        re_printf("info","The group %d is continued the scanning process at the start VC in continuous scanning mode.\n", group_number);
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
                                (*SGSTR)["SGACT"] = (unsigned int)((*SGSTR)["SGACT"]) & (~(0x1 << group));
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
void Cadc_u2a::ADCStateMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
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
void Cadc_u2a::THCircuitMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        unsigned int ther = (unsigned int)(*THER);
        unsigned int thgsr = (unsigned int)(*THGSR);
        unsigned int tdlv   = (unsigned int)((*TDCR)["TDLV"]);
        //0.Write 1 into THSTPCR to stop T&H circuit
        if(mTHStop){
            re_printf("info","T&H circuit is stop.\n");/*{{{*/
            mTHStop = false;
            mTHGAHoldingEvent.cancel();
            mTHGBHoldingEvent.cancel();
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                mTHChannelState[TH_channel] = emTHIdleState;
            }
            (*SGSTR)["SHACT"] = 0;
        }/*}}}*/
        //1.Write into THER to enable/disable channel
        if(mTHEnable){
            mTHEnable = false;/*{{{*/
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if(((ther >> TH_channel) & 0x1 ) == 0){
                    re_printf("info","The TH channel %d is disable.\n", TH_channel);
                    mTHChannelState[TH_channel] = emTHIdleState;
                }
            }

            // --> Confirm
            bool is_stop_holding_groupa = true;
//            bool is_stop_holding_groupb = true;
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if((((thgsr>>(2*TH_channel)) & 0x1) == 0) && (mTHChannelState[TH_channel] == emTHHoldingState)){
                    is_stop_holding_groupa = false;
                }
                #if 0 //Update for HWM0.61
                if((((thgsr>>(2*TH_channel)) & 0x1) == 1) && (mTHChannelState[TH_channel] == emTHHoldingState)){
                    is_stop_holding_groupb = false;
                }
                #endif
            }

            if(is_stop_holding_groupa){
                mTHGAHoldingEvent.cancel();
            }
            #if 0 //Update for HWM0.61
            if(is_stop_holding_groupb){
                mTHGBHoldingEvent.cancel();
            }
            #endif
            // <-- Confirm
        }/*}}}*/
        //2.Write 1 into THSMPSTCR to sample all T&H channel
        if(mTHSamplingManual && (TimingCheck() && VoltageCheck())){
            re_printf("info","All T&H channels are sampled.\n");/*{{{*/
            mTHSamplingManual = false;
            mTHGAHoldingEvent.cancel();
            mTHGBHoldingEvent.cancel();
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if(((ther >> TH_channel) & 0x1 ) == 1){
                    mTHChannelState[TH_channel] = emTHSamplingState;
                    (*SGSTR)["SHACT"] = 1;
                    double sampling_start_time = GetCurTime();
                    mTHSamplingStartTime[TH_channel] = sampling_start_time;
                    mIntTHSample[TH_channel] = LoadAnalogTerminal(THzAssignChannel[TH_channel]);
#if 0
                    if((unsigned int)((*TDCR)["THSDE"]) == 1) {
                        mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                    }
#else
                    // RH850_U2A added
                    if((unsigned int)((*TDCR)["TDE"]) == 1) {
                        mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                    }
#endif
                    mPhyChannelTH[TH_channel] = THzAssignChannel[TH_channel];
                }
            }
        }/*}}}*/
        //3. Update the new value from analog terminal when TH channel is in sampling state
        if(mTHUpdateInput[0] || mTHUpdateInput[1] || mTHUpdateInput[2] || mTHUpdateInput[3] || mTHUpdateInput[4] || mTHUpdateInput[5]){
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){/*{{{*/
                if(mTHUpdateInput[TH_channel]){
                    double sampling_start_time = GetCurTime();
                    mTHSamplingStartTime[TH_channel] = sampling_start_time;
                    mIntTHSample[TH_channel] = LoadAnalogTerminal(THzAssignChannel[TH_channel]);
#if 0
                    if((unsigned int)((*TDCR)["THSDE"]) == 1) {
                        mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                    }
#else
                    // RH850_U2A added
                    if((unsigned int)((*TDCR)["TDE"]) == 1) {
                        mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                    }
#endif
                    mPhyChannelTH[TH_channel] = THzAssignChannel[TH_channel];
                    mTHUpdateInput[TH_channel] = false;
                }
            }
        }/*}}}*/
        //4.Update channel state when finishing scanning process for TH channel
        if((mTHUpdateChannelState[0] || mTHUpdateChannelState[1] || mTHUpdateChannelState[2] || mTHUpdateChannelState[3] ||
        mTHUpdateChannelState[4] || mTHUpdateChannelState[5]) && mTHUpdateChannelStateFlag){
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){/*{{{*/
                if(mTHUpdateChannelState[TH_channel] && ((*THCR)["ASMPMSK"] == 0)){
                    re_printf("info","The TH channel %d is sampled automatically.\n", TH_channel);
                    if ((((*THACR)["HLDCTE"] == 1) || ((*THBCR)["HLDCTE"] == 1)) && ((*ADCR1)["SUSMTD"] != 2)) {
                        re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE or THBCR.HLDCTE is 1 while ADCR1.SUSMTD is not 2.\n");
                    } else {
                        mTHChannelState[TH_channel] = emTHSamplingState;
                        (*SGSTR)["SHACT"] = 1;
                        double sampling_start_time = GetCurTime();
                        mTHSamplingStartTime[TH_channel] = sampling_start_time;
                        mIntTHSample[TH_channel] = LoadAnalogTerminal(THzAssignChannel[TH_channel]);
#if 0
                        if((unsigned int)((*TDCR)["THSDE"]) == 1) {
                            mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                        }
#else
                        // RH850_U2A added
                        if((unsigned int)((*TDCR)["TDE"]) == 1) {
                            mIntTHSample[TH_channel] = SelfDiagData(THzAssignChannel[TH_channel],tdlv);
                        }
#endif
                        mPhyChannelTH[TH_channel] = THzAssignChannel[TH_channel];
                    }
                }
                mTHUpdateChannelState[TH_channel] = false;
            }
            mTHUpdateChannelStateFlag = false;
        }/*}}}*/
        //5.Trigger holding porcess by software(write 1 into THAHLDSTCR or THBHLDSTCR) or by input port ADmSGx
        #if 0
        if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding || mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding){
            if((unsigned int)((*THACR)["SGS"]) == (unsigned int)((*THBCR)["SGS"])){/*{{{*/
                re_printf("warning","Holding process in T&H circuit is not triggered because Group A and group B are same.\n");
            }else{
                unsigned int sgs_groupa = (unsigned int)((*THACR)["SGS"]);
                unsigned int sgs_groupb = (unsigned int)((*THBCR)["SGS"]);
                unsigned int scanmd_groupa = (unsigned int)((*SGCR[sgs_groupa+1])["SCANMD"]);
                unsigned int scanmd_groupb = (unsigned int)((*SGCR[sgs_groupb+1])["SCANMD"]);
                unsigned int mcyc_groupa = (unsigned int)((*SGMCYCR[sgs_groupa+1])["MCYC"]);
                unsigned int mcyc_groupb = (unsigned int)((*SGMCYCR[sgs_groupb+1])["MCYC"]);
                double holding_start_time = GetCurTime();
                double holding_delay  = 0;
                if (mEnableTimeCalculation) {
                    holding_delay = (emtTHSDCoeff + emtTHLDCoeff) * mADCLKPeriod; // Time from detecting hold trigger -> start scanning
                } else {
                    holding_delay = mtTHSD + mtTHLD; // Time from detecting hold trigger -> start scanning
                }
                //trigger holding for group A
                if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding){
                    if(!(scanmd_groupa == 0x0 && mcyc_groupa == 0x0)){
                        re_printf("warning","T&H Group A is not triggered holding process because setting SGCR%d.SCANMD and SGMCYCR%d.MCYC are not correct.\n", sgs_groupa,sgs_groupa);
                    }else {
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
                                    (*SGSTR)["SHACT"] = 1;
                                    holding_enable = true;
                                }
                            }
                        }
                        if(holding_enable){
                            re_printf("info","The holding process of group A is started.\n");
                            mTHAHoldingProcess = true;
                            mGroupANumber =  sgs_groupa + 1;
                            (*SGSR[mGroupANumber])["SGACT"] = 1;
                            (*SGSTR)["SGACT"] =(unsigned int) ( (*SGSTR)["SGACT"]) | (0x1 << mGroupANumber);
                            //Suspend all scanning process
                            if (mScanningState) {
                                mSuspendFlag = true;
                                mSuspendEvent.notify();
                            }
                            mTHGAHoldingEvent.notify((unsigned int)holding_delay,SC_NS);
                        }
                    }
                }//end for if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding)
                //trigger holding for group B
                if(mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding){
                    if(!(scanmd_groupb == 0x0 && mcyc_groupb == 0x0)){
                        re_printf("warning","T&H Group B is not triggered holding process because setting SGCR%d.SCANMD and SGMCYCR%d.MCYC are not correct.\n", sgs_groupb,sgs_groupb);
                    }else {
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
                                    (*SGSTR)["SHACT"] = 1;
                                    holding_enable = true;
                                }
                            }
                        }
                        if(holding_enable){
                            re_printf("info","The holding process of group B is started.\n");
                            mTHBHoldingProcess = true;
                            mGroupBNumber =  sgs_groupb + 1;
                            (*SGSR[mGroupBNumber])["SGACT"] = 1;
                            (*SGSTR)["SGACT"] =(unsigned int) ((*SGSTR)["SGACT"]) | (0x1 << mGroupBNumber);
                            //Suspend all scanning process
                            if (mScanningState) {
                                mSuspendFlag = true;
                                mSuspendEvent.notify();
                            }
                            mTHGBHoldingEvent.notify((unsigned int)holding_delay,SC_NS);
                        }
                    }
                }//end for if(mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding)
            }//end for else
        }/*}}}*/
        #else  //Update for HWM0.61
        if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding || mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding){
            unsigned int sgs_groupa = (unsigned int)((*THACR)["SGS"]);
            //                unsigned int sgs_groupb = (unsigned int)((*THBCR)["SGS"]);
            unsigned int scanmd_groupa = (unsigned int)((*SGCR[sgs_groupa+1])["SCANMD"]);
            //                unsigned int scanmd_groupb = (unsigned int)((*SGCR[sgs_groupb+1])["SCANMD"]);
            unsigned int mcyc_groupa = (unsigned int)((*SGMCYCR[sgs_groupa+1])["MCYC"]);
            //                unsigned int mcyc_groupb = (unsigned int)((*SGMCYCR[sgs_groupb+1])["MCYC"]);
            double holding_start_time = GetCurTime();
            double holding_delay  = 0;
            if (mEnableTimeCalculation) {
                holding_delay = (emtTHSDCoeff + emtTHLDCoeff) * mADCLKPeriod; // Time from detecting hold trigger -> start scanning
            } else {
                holding_delay = mtTHSD + mtTHLD; // Time from detecting hold trigger -> start scanning
            }
            //trigger holding for group A
            if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding){
                if(!(scanmd_groupa == 0x0 && mcyc_groupa == 0x0)){
                    re_printf("warning","T&H Group A is not triggered holding process because setting SGCR%d.SCANMD and SGMCYCR%d.MCYC are not correct.\n", sgs_groupa,sgs_groupa);
                }else {
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
                                (*SGSTR)["SHACT"] = 1;
                                holding_enable = true;
                            }
                        }
                    }
                    if(holding_enable){
                        re_printf("info","The holding process of group A is started.\n");
                        mTHAHoldingProcess = true;
                        mGroupANumber =  sgs_groupa + 1;
                        (*SGSR[mGroupANumber])["SGACT"] = 1;
                        (*SGSTR)["SGACT"] =(unsigned int) ( (*SGSTR)["SGACT"]) | (0x1 << mGroupANumber);
                        //Suspend all scanning process
                        if (mScanningState) {
                            mSuspendFlag = true;
                            mSuspendEvent.notify();
                        }
                        mTHGAHoldingEvent.notify((unsigned int)holding_delay,SC_NS);
                    }
                }
            }//end for if(mTHASoftwareTriggerHolding || mTHAPortTriggerHolding)
            //trigger holding for group B
            //if(mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding){
            //    if(!(scanmd_groupb == 0x0 && mcyc_groupb == 0x0)){
            //        re_printf("warning","T&H Group B is not triggered holding process because setting SGCR%d.SCANMD and SGMCYCR%d.MCYC are not correct.\n", sgs_groupb,sgs_groupb);
            //    }else {
            //        mTHBSoftwareTriggerHolding = false;
            //        mTHBPortTriggerHolding = false;
            //        bool holding_enable = false;
            //        for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
            //            if(((thgsr>>(2*TH_channel)) & 0x1) == 1 ){
            //                if(mTHChannelState[TH_channel] != emTHSamplingState){
            //                    re_printf("warning","T&H channel %d is not triggered holding process because this channel is not in sampling state.\n", TH_channel);
            //                }else{
            //                    if((holding_start_time - mTHSamplingStartTime[TH_channel]) < (mtTSPL*mADCLKPeriod)){
            //                        re_printf("warning","T&H channel %d is triggered holding process when sampling process is not finished.\n", TH_channel);
            //                    }
            //                    mTHChannelState[TH_channel] = emTHHoldingState;
            //                    (*SGSTR)["SHACT"] = 1;
            //                    holding_enable = true;
            //                }
            //            }
            //        }
            //        if(holding_enable){
            //            re_printf("info","The holding process of group B is started.\n");
            //            mTHBHoldingProcess = true;
            //            mGroupBNumber =  sgs_groupb + 1;
            //            (*SGSR[mGroupBNumber])["SGACT"] = 1;
            //            (*SGSTR)["SGACT"] =(unsigned int) ((*SGSTR)["SGACT"]) | (0x1 << mGroupBNumber);
            //            //Suspend all scanning process
            //            if (mScanningState) {
            //                mSuspendFlag = true;
            //                mSuspendEvent.notify();
            //            }
            //            mTHGBHoldingEvent.notify((unsigned int)holding_delay,SC_NS);
            //        }
            //    }
            //}//end for if(mTHBSoftwareTriggerHolding || mTHBPortTriggerHolding)
        }/*}}}*/
        #endif
        mADCStateEvent.notify();
    }
}//}}}

void Cadc_u2a::THAHoldingEndMethod(void)
{//{{{
    // mutex lock
    if(!mIsADReset && (mADCInstance < mADCNum)){
        re_printf("info","The holding process of group A is finished.\n");
        if (!mTHBHoldingProcess) {
            mScanningEvent.notify();
        }
        mTHAHoldingProcess = false;
        HardwareTrigger(mGroupANumber, 1);
    }
}//}}}

#if 0   //Update for HWM0.61 - T&H Group B is removed
void Cadc_u2a::THBHoldingEndMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        re_printf("info","The holding process of group B is finished.\n");
        if (!mTHAHoldingProcess) {
           mScanningEvent.notify();
        }
        mTHBHoldingProcess = false;
        HardwareTrigger(mGroupBNumber, 2);
    }
}//}}}
#endif
void Cadc_u2a::THzMethod(const unsigned int index)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)) {
        for (unsigned int i = 0; i < mTHChannelNum; i++) {
            if (index == THzAssignChannel[i]) {
                if(mTHChannelState[i] == emTHSamplingState) {
                    mTHUpdateInput[i] = true;
                    mTHUpdateInputEvent.notify();
                }
            }
        }
    }
}//}}}

void Cadc_u2a::cb_ADENDP_ENDP (RegCBstr str) //sampling manually
{//{{{
    if(str.data != str.pre_data) {
        if(!AllSGStopCheck()){
            re_printf("warning","Writing into ADENDP%d register is not affected.\n",str.channel);
            (*ADENDP[str.channel]) = str.pre_data;
        }else {
            if((unsigned int)((*ADENDP[str.channel])["ENDP"]) >= mVirChannelNum){
                re_printf("warning","Writing into ADENDP%d.ENDP bit is not affected.\n",str.channel);
                (*ADENDP[str.channel])["ENDP"] = str.pre_data;
            }
        }
    }
}//}}}

void Cadc_u2a::cb_THSMPSTCR_SMPST (RegCBstr str) //sampling manually
{//{{{
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THSMPSTCR register is not affected\n");
        (*THSMPSTCR) = str.pre_data;
        return;
    } else {
        if((*THSMPSTCR)["SMPST"] == 1){
            (*THSMPSTCR) = 0;
            #if 0
            if ((((*THACR)["HLDCTE"] == 1) || ((*THBCR)["HLDCTE"] == 1)) && ((*ADCR1)["SUSMTD"] != 2)) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE or THBCR.HLDCTE is 1 while ADCR1.SUSMTD is not 2.\n");
                return;
            }
            if (((*THACR)["HLDCTE"] != 1) && ((*THBCR)["HLDCTE"] != 1)) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE and THBCR.HLDCTE are not 1.\n");
                return;
            }
            if (  (*THBCR)["HLDTE"]  == 1
                    &&(*THBCR)["HLDCTE"] == 1
                    &&(*SGCR[(*THBCR)["SGS"] + 1])["TRGMD"] != 1) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because SGCR%d.TRGMD != 1.\n", (unsigned int)(*THBCR)["SGS"] + 1);
                return;
            }
            if (  (*THACR)["HLDTE"] == 1
                    &&(*THACR)["HLDCTE"] == 1
                    &&(*SGCR[(*THACR)["SGS"] + 1])["TRGMD"] != 1) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because SGCR%d.TRGMD != 1.\n", (unsigned int)(*THACR)["SGS"] + 1);
                return;
            }
            if ((unsigned int)((*THACR)["SGS"]) !=(unsigned int)((*THBCR)["SGS"])){
                mTHSamplingManual = true;
                mTHSamplingEvent.notify();
            } else {
                re_printf("warning","Sampling process in T&H circuit is not triggered because Group A and group B are same.\n");
            }
            #else  //Update for HWM0.61
            if (((*THACR)["HLDCTE"] == 1) && ((*ADCR1)["SUSMTD"] != 2)) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE is 1 while ADCR1.SUSMTD is not 2.\n");
                return;
            }
            if ((*THACR)["HLDCTE"] != 1) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because THACR.HLDCTE are not 1.\n");
                return;
            }
            if (  (*THACR)["HLDTE"] == 1
                    &&(*THACR)["HLDCTE"] == 1
                    &&(*SGCR[(*THACR)["SGS"] + 1])["TRGMD"] != 1) {
                re_printf("warning","Sampling process in T&H circuit is not triggered because SGCR%d.TRGMD != 1.\n", (unsigned int)(*THACR)["SGS"] + 1);
                return;
            }
            mTHSamplingManual = true;
            mTHSamplingEvent.notify();
            #endif
        }
    }
}//}}}

void Cadc_u2a::cb_THSTPCR_THSTP (RegCBstr str) //stop T&H circuit
{//{{{
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THSTPCR register is not affected.\n");
        (*THSTPCR) = str.pre_data;
        return;
    } else {
        if((*THSTPCR)["THSTP"] == 1){
            (*THSTPCR) = 0;
            mTHStop = true;
            mTHStopEvent.notify();
        }
    }
}//}}}

void Cadc_u2a::cb_THCR_ASMPMSK (RegCBstr str) //automatic sampling mask
{//{{{
    // --> Confirm
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THACR register is not affected.\n");
        (*THCR) = str.pre_data;
        return;
    } else {
        if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0) // HLDTE of group A/B must be equal to 0
                || ((*SGSR[(unsigned int)((*THACR)["SGS"]) + 1])["SGACT"] != 0) || ((*SGSR[(unsigned int)((*THBCR)["SGS"]) + 1])["SGACT"] != 0) // SGACT of corresponding group must be equal to 0
                || ((*THER) != 0)) {  // All THzE must be equal to 0
            re_printf("warning","Writing into THCR register is not affected.\n");
            (*THCR) = str.pre_data;
        }
    }
    // <-- Confirm
}//}}}

void Cadc_u2a::cb_THAHLDSTCR_HLDST (RegCBstr str) //trigger holding group A
{//{{{
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THAHLDSTCR register is not affected.\n");
        (*THAHLDSTCR) = str.pre_data;
        return;
    } else {
        if((*THAHLDSTCR)["HLDST"] == 1){
            (*THAHLDSTCR) = 0;
            if(TimingCheck() && VoltageCheck()){
                if (((*THACR)["HLDCTE"] == 1) && ((*THACR)["HLDTE"] == 0)) {
                    if(!CheckScanMode((unsigned int)((*THACR)["SGS"]) + 1,false,true,false,true)){
                        mTHASoftwareTriggerHolding = true;
                        mTHASoftwareTriggerHoldingEvent.notify();
                    }
                }
            }
        }
    }
}//}}}

void Cadc_u2a::cb_THBHLDSTCR_HLDST (RegCBstr str) //trigger holding group B
{//{{{
#if 0
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THBHLDSTCR register is not affected.\n");
        (*THBHLDSTCR) = str.pre_data;
        return;
    } else {
        if((*THBHLDSTCR)["HLDST"] == 1){
            (*THBHLDSTCR) = 0;
            if(TimingCheck() && VoltageCheck()){
                if (((*THBCR)["HLDCTE"] == 1) && ((*THBCR)["HLDTE"] == 0)) {
                    if(!CheckScanMode((unsigned int)((*THBCR)["SGS"]) + 1,false,true,false,false,true)){
                        mTHBSoftwareTriggerHolding = true;
                        mTHBSoftwareTriggerHoldingEvent.notify();
                    }
                }
            }
        }
    }
#else // Update for HWM0.61
    (*THBHLDSTCR) = 0;
    re_printf("warning","No register at address 0x6A4.\n");
#endif
}//}}}

void Cadc_u2a::cb_THACR_SGS (RegCBstr str)
{//{{{
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THACR register is not affected.\n");
        (*THACR) = str.pre_data;
        return;
    } else {
        //unsigned int ther = (unsigned int)(*THER);
        //unsigned int thgsr = (unsigned int)(*THGSR);
        unsigned int pre_hldte = ((unsigned int)(str.pre_data >> 4) & 0x1);
        unsigned int cur_hldte = ((unsigned int)(str.data >> 4) & 0x1);
        unsigned int pre_hldcte = ((unsigned int)(str.pre_data >> 5) & 0x1);
        unsigned int cur_hldcte = ((unsigned int)(str.data >> 5) & 0x1);
        unsigned int pre_sgs = ((unsigned int)(str.pre_data) & 0x3);
        unsigned int cur_sgs = ((unsigned int)(str.data) & 0x3);
        //if (((*THACR)["SGS"] != (str.pre_data & 0x3)) || ((*THACR)["HLDCTE"] != ((str.pre_data & 0x20) >> 5))) {
        //    if (!ChkAccess(str.is_wr, "", THACR->name(), "HLDCTE and SGS")) {
        //        (*THACR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
        //        return;
        //    } else if (((*THER) != 0) || ((str.pre_data & 0x10) != 0) || ((*THBCR)["HLDTE"] != 0)) {
        //        (*THACR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
        //        re_printf("warning","Writing into THACR.HLDCTE and SGS bit is not affected.\n");
        //        return;
        //    }
        //}
        if(!AllSGStopCheck()){
            if(pre_hldte == 0 && cur_hldte == 1) {
                re_printf("warning","Writing into THACR.HLDTE bit is not affected.\n");
                (*THACR)["HLDTE"] = pre_hldte;
            }
            if(pre_hldcte != cur_hldcte) {
                re_printf("warning","Writing into THACR.HLDCTE bit is not affected.\n");
                (*THACR)["HLDCTE"] = pre_hldcte;
            }
            if(pre_sgs != cur_sgs){
                re_printf("warning","Writing into THACR.SGS bit is not affected.\n");
                (*THACR)["SGS"] = pre_sgs;
            }
        }else {
            bool is_holding = false;
            for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
                if(mTHChannelState[TH_channel] == emTHHoldingState){
                    is_holding = true;
                    break;
                }
            }
            // --> Confirm
            if(is_holding || mScanningState){
                re_printf("warning","Writing into THACR.SGS bit is not affected.\n");
                (*THACR) = (unsigned int)((str.data & 0x30) | (str.pre_data & 0xCF));
            }
            //if (((*THACR)["HLDCTE"] == 0) && ((*THACR)["HLDTE"] != 0)) {
            //    re_printf("warning","Cannot set THACR.HLDTE = %d while THACR.HLDCTE = 0.\n", (bool)(*THACR)["HLDTE"]);
            //    (*THACR) = str.pre_data;
            //}
            if ((*PWDCR)["PWE"] == 1 && (*THACR)["SGS"] == 3) {
                re_printf("warning","Writing into SGS bit is not affected.\n");
                (*THACR)["SGS"] = (unsigned int)(str.pre_data & 0x3);
            }
        }
    }
}//}}}

void Cadc_u2a::cb_THBCR_SGS (RegCBstr str)
{//{{{
#if 0
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THBCR register is not affected.\n");
        (*THBCR) = str.pre_data;
        return;
    } else {
        //unsigned int ther = (unsigned int)(*THER);
        //unsigned int thgsr = (unsigned int)(*THGSR);
        unsigned int pre_hldte = ((unsigned int)(str.pre_data >> 4) & 0x1);
        unsigned int cur_hldte = ((unsigned int)(str.data >> 4) & 0x1);
        unsigned int pre_hldcte = ((unsigned int)(str.pre_data >> 5) & 0x1);
        unsigned int cur_hldcte = ((unsigned int)(str.data >> 5) & 0x1);
        unsigned int pre_sgs = ((unsigned int)(str.pre_data) & 0x3);
        unsigned int cur_sgs = ((unsigned int)(str.data) & 0x3);
        //if (((*THBCR)["SGS"] != (str.pre_data & 0x3)) || ((*THBCR)["HLDCTE"] != ((str.pre_data & 0x20) >> 5))) {
        //    if (!ChkAccess(str.is_wr, "", THBCR->name(), "HLDCTE and SGS")) {
        //        (*THBCR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
        //        return;
        //    } else if (((*THER) != 0) || ((str.pre_data & 0x10) != 0) || ((*THACR)["HLDTE"] != 0)) {
        //        (*THBCR) = (unsigned int)((str.data & 0x10) | (str.pre_data & 0xEF));
        //        re_printf("warning","Writing into THBCR.HLDCTE and SGS bit is not affected.\n");
        //        return;
        //    }
        //}
        if(!AllSGStopCheck()){
            if(pre_hldte == 0 && cur_hldte == 1) {
                re_printf("warning","Writing into THBCR.HLDTE bit is not affected.\n");
                (*THBCR)["HLDTE"] = pre_hldte;
            }
            if(pre_hldcte != cur_hldcte) {
                re_printf("warning","Writing into THBCR.HLDCTE bit is not affected.\n");
                (*THBCR)["HLDCTE"] = pre_hldcte;
            }
            if(pre_sgs != cur_sgs){
                re_printf("warning","Writing into THBCR.SGS bit is not affected.\n");
                (*THBCR)["SGS"] = pre_sgs;
            }
        }else {
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
            }
            //if (((*THBCR)["HLDCTE"] == 0) && ((*THBCR)["HLDTE"] != 0)) {
            //    re_printf("warning","Cannot set THBCR.HLDTE = %d while THBCR.HLDCTE = 0.\n", (bool)(*THBCR)["HLDTE"]);
            //    (*THBCR) = str.pre_data;
            //}
            if ((*PWDCR)["PWE"] == 1 && (*THBCR)["SGS"] == 3) {
                re_printf("warning","Writing into SGS bit is not affected.\n");
                (*THBCR)["SGS"] = (unsigned int)(str.pre_data & 0x3);
            }
        }
    }
#else // Update for HWM0.61
    (*THBCR) = 0;
    re_printf("warning","No register at address 0x6C4.\n");

#endif

}//}}}

void Cadc_u2a::cb_THER_TH0E (RegCBstr str)
{//{{{
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THER register is not affected.\n");
        (*THER) = str.pre_data;
        return;
    } else {
        //unsigned int ther = (unsigned int)(*THER);
        //unsigned int thgsr = (unsigned int)(*THGSR);
        if (str.data != str.pre_data) {
            if(AllSGStopCheck()) {
                if((*THER) == 0){
                    mTHStop = true;
                    mTHStopEvent.notify();
                }else{
                    mTHEnable = true;
                    mTHEnableEvent.notify();
                }
            }else {
                re_printf("warning","Writing into THER register is not affected.\n");
                (*THER) = str.pre_data;
            }
        }
    }
}//}}}

void Cadc_u2a::cb_THGSR_TH0GS (RegCBstr str)
{//{{{
#if 0
    if (mADCInstance == 2) {
        re_printf("warning","T&H is not supported in ADC2. Writing into THGSR register is not affected.\n");
        (*THGSR) = str.pre_data;
        return;
    } else {
        //if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0) // HLDTE of group A/B must be equal to 0
        // || ((*SGSR[(*THACR)["SGS"] + 1])["SGACT"] != 0) || ((*SGSR[(*THBCR)["SGS"] + 1])["SGACT"] != 0) // SGACT of corresponding group must be equal to 0
        // || ((*THER) != 0)) {  // All THzE must be equal to 0
        //        re_printf("warning","Writing into THGSR register is not affected.\n");
        //        (*THGSR) = (unsigned int)(str.pre_data);
        //}
        if (((*SGSR[(unsigned int)((*THACR)["SGS"]) + 1])["SGACT"] != 0) || ((*SGSR[(unsigned int)((*THBCR)["SGS"]) + 1])["SGACT"] != 0) // SGACT of corresponding group must be equal to 0
         || ((*THER) != 0)) {  // All THzE must be equal to 0
                re_printf("warning","Writing into THGSR register is not affected.\n");
                (*THGSR) = (unsigned int)(str.pre_data);
        }
    }
#else // Update for HWM0.61
    (*THGSR) = 0;
    re_printf("warning","No register at address 0x6B0.\n");

#endif
}//}}}

void Cadc_u2a::cb_MPXCMDR_MPXCMD(RegCBstr str)
{//{{{
    // do nothing
}//}}}

void Cadc_u2a::cb_VCLMINTER1_ADUL00IE(RegCBstr str)
{//{{{
    if (!AllSGStopCheck()) {
        re_printf("warning", "Writing into VCLMINTER1 register is not affected.\n");
        (*VCLMINTER1) = str.pre_data;
    }
}//}}}
void Cadc_u2a::cb_VCLMINTER2_ADUL32IE(RegCBstr str)
{//{{{
    if (!AllSGStopCheck()) {
        re_printf("warning", "Writing into VCLMINTER2 register is not affected.\n");
        (*VCLMINTER2) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_VCLMINTER3_ADUL64IE(RegCBstr str)
{//{{{
    if (!AllSGStopCheck()) {
        re_printf("warning", "Writing into VCLMINTER3 register is not affected.\n");
        (*VCLMINTER3) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_SGTSEL_TxSEL0(RegCBstr str)
{//{{{
    if ((mADCInstance == 0) || (mADCInstance == 1)) {
        re_printf("warning","SGTSEL%d is not supported in ADC0 and ADC1. Writing into SGTSEL%d register is not affected.\n",str.channel,str.channel);
        (*SGTSEL[str.channel]) = str.pre_data;
        return;
    } else {
        if (str.data != str.pre_data) {
            if ((!AllSGStopCheck()) || (!AllHWTriggerDisableCheck())) {
                re_printf("warning", "Writing into SGTSEL%d register is not affected.\n",str.channel);
                (*SGTSEL[str.channel]) = str.pre_data;
            } else if (((*SGTSEL[str.channel]) != 0x20) && ((*SGTSEL[str.channel]) != 0x10) && ((*SGTSEL[str.channel]) != 0x8) && ((*SGTSEL[str.channel]) != 0x4) 
                    && ((*SGTSEL[str.channel]) != 0x2) && ((*SGTSEL[str.channel]) != 0x1) && ((*SGTSEL[str.channel]) != 0x0)) {
                re_printf("warning", "Set only one of the bits to 1.\n");
                (*SGTSEL[str.channel]) = str.pre_data;
            }
            if ((*SGTSEL[str.channel]) == 0x20) {
                for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
                    if (group_number != str.channel) {
                        if ((*SGTSEL[group_number] == 0x20)) {
                            re_printf("warning", "SEQADTRG is already selected by scan group %d. It’s prohibited to select SEQADTRG for multiple Scan groups.\n",group_number);
                            (*SGTSEL[str.channel]) = str.pre_data;
                        }
                    }
                }
            }
        }
    }
}//}}}

void Cadc_u2a::ScanGroup0SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup0,false,true)){
            mSGSTTriggerEvent [emGroup0].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup1SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup1,false,true)){
            mSGSTTriggerEvent [emGroup1].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup2SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup2,false,true)){
            mSGSTTriggerEvent [emGroup2].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup3SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup3,false,true)){
            mSGSTTriggerEvent [emGroup3].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup4SGSTTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup4,false,true)){
            mSGSTTriggerEvent [emGroup4].notify(start_delay,SC_NS);
        }
    }
}//}}}

void Cadc_u2a::ScanGroup0ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup0,false,true,true)){
            mADSYNTriggerEvent [emGroup0].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup1ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup1,false,true,true)){
            mADSYNTriggerEvent [emGroup1].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup2ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup2,false,true,true)){
            mADSYNTriggerEvent [emGroup2].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup3ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup3,false,true,true)){
            mADSYNTriggerEvent [emGroup3].notify(start_delay,SC_NS);
        }
    }
}//}}}


void Cadc_u2a::ScanGroup4ADSYNTriggerMethod(void)
{//{{{
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }
    if(!mIsADReset && (mADCInstance < mADCNum)){
        if(!CheckScanMode(emGroup4,false,true,true)){
            mADSYNTriggerEvent [emGroup4].notify(start_delay,SC_NS);
        }
    }
}//}}}

//scanning software trigger method
void Cadc_u2a::ScanGroup0SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup0);
    }
}//}}}

void Cadc_u2a::ScanGroup1SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup1);
    }
}//}}}

void Cadc_u2a::ScanGroup2SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup2);
    }
}//}}}

void Cadc_u2a::ScanGroup3SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup3);
    }
}//}}}

void Cadc_u2a::ScanGroup4SoftwareTriggerMethod(void)
{//{{{
    if(!mIsADReset && (mADCInstance < mADCNum)){
        GroupCheck (emGroup4);
    }
}//}}}

//external input port method
void Cadc_u2a::ADmSG0TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG0TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup0,3);  //group 0, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2a::ADmSG1TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG1TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup1,3); //group 1, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2a::ADmSG2TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG2TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup2,3); //group 2, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2a::ADmSG3TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG3TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup3,3); //group 3, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2a::ADmSG4TRGMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (ADmSG4TRG.read() == 1)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            HardwareTrigger(emGroup4,3); //group 4, trigger feature 3
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

//counting end method
void Cadc_u2a::ScanGroup3ADtimerTriggerMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","The counting of ADtimer 3 is overflow.\n");
        if(!CheckIsSVMode()){
            HardwareTrigger(emGroup3,0);//group 3, trigger feature 0
        }
    }
}//}}}

void Cadc_u2a::ScanGroup4ADtimerTriggerMethod(void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        re_printf("info","The counting of ADtimer 4 is overflow.\n");
        if(!CheckIsSVMode()){
            HardwareTrigger(emGroup4,0);//group 4, trigger feature 0
        }
    }
}//}}}

//interrupt ouput method
void Cadc_u2a::INT_MPXmInterruptMethod(void)
{//{{{
    if(mIsPReset){
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

void Cadc_u2a::ADMPXOutputMethod(void)
{//{{{
    if(mIsPReset){
        MPXCURm.write(0);
        re_printf("info","MPXCUR ports are reset to initial value.\n");
    }else {
        MPXCURm.write((unsigned int)(*MPXCURR1)["MPXCUR"]);
        re_printf("info","MPXCUR ports change value to 0x%X.\n", (unsigned int)(*MPXCURR1)["MPXCUR"]);
    }
}//}}}

void Cadc_u2a::ADPEmInterruptMethod(void)
{//{{{
    if(mIsPReset){
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

void Cadc_u2a::INT_ADmxINT_ADEmDataMethod(void)
{//{{{
    unsigned int dfent = 0x0;
    unsigned int dftag = 0x0;
#if 0
    unsigned int asentsgxe = 0x0;
#else
    // RH850_U2A added
    unsigned int gtmentsgxe = 0x0;
#endif
    unsigned int data = 0x0;
    if(mIsPReset){
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
        //3. RH850_E2x added INT_ULm
        //INT_ULm.write(0);
        //DumpInterruptMessage("INT_UL", "", "is reset to 0");
        //4. RH850_E2x added for ASF
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
            if(mFinalVC < mVirChannelNum){
                if((*DIR[mFinalVC])["IDEF"] == 1){
                    (*DIR[mFinalVC])["IDEF"] = 0;
                }
            }
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4 && (*PWDDIR)["IDEF"] == 1){
                (*PWDDIR)["IDEF"] = 0;
            }
            if((*IDER)["IDE"] == 0){
                (*IDER)["IDECAP"] = mFinalVC;
                (*IDER)["IDE"] = 1;
            }
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4) {
                re_printf("info","The id error occurs in group%d.PWM-Diag.\n", mFinalGroup);
            }
            if(mFinalVC < mVirChannelNum){
                re_printf("info","The id error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC );
            }
        } else {
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4) {
                (*PWDDIR)["IDEF"] = 1;
            }
            if(mFinalVC < mVirChannelNum){
                (*DIR[mFinalVC])["IDEF"] = 1;
            }
            (*IDER)["IDE"] = 0;
        }
        if((mErrorId & 0x4) == emOverwriteError && mFinalVC < mVirChannelNum){
            if((*OWER)["OWE"] == 0){
                (*OWER)["OWECAP"] = mFinalVC;
                (*OWER)["OWE"] = 1;
            }
            re_printf("info","The overwrite error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC);
        }
        if((mErrorId & 0x8) == emBoundaryError){
            if(mFinalVC == emVcPWM && mFinalGroup == emGroup4) {
                (*PWVCLMSR)["PWVCLMS"] = 1;
                re_printf("info","The boundary error occurs in group%d.PWM-Diag.\n", mFinalGroup);
            }
            if(mFinalVC < mVirChannelNum){
                SetULErrorStatus(mFinalVC);
                re_printf("info","The boundary error occurs in group%d.VC%d.\n", mFinalGroup, mFinalVC);
            }
            if(  (ULIntrEnable(mFinalVC) && mFinalVC < mVirChannelNum)
               ||((*PWVCLMINTER)["PWADULIE"] == 1 && mFinalVC == emVcPWM && mFinalGroup == emGroup4)){
                //INT_ULm.write(1);
#if 0
                mINT_ULmActiveNum++;
                mINT_ULmActive = true;
                DumpInterruptMessage("INT_UL", "", "Assert");
#else
                // RH850_U2A added
                mINT_ERRmActive = true;
                mINT_ERRmEvent.notify();
#endif
            }
            CheckULCR();
        }

#if 0
        if(!mINT_ADEmProcess) {
            if((mErrorId & ((unsigned int)(*SFTCR)) & 0x05) != 0){
                mINT_ADEmActive = true;
                mINT_ADEmEvent.notify();
            }
        }else {
            re_printf("info","INT_ADEm is in process.\n");
        }
#else
        // RH850_U2A added
        if(!mINT_ERRmProcess) {
            if((mErrorId & ((unsigned int)(*SFTCR)) & 0x05) != 0){
                mINT_ERRmActive = true;
                mINT_ERRmEvent.notify();
            }
        } else {
            re_printf("info","INT_ERRm is in process.\n");
        }
#endif
        if(mFinalVC < mVirChannelNum){
            if(mFinalCnvcls == emMPXMode || mFinalCnvcls == emMPXAdditionMode) {
                (*DIR[mFinalVC])["MPXE"] = 1;
            }else {
                (*DIR[mFinalVC])["MPXE"] = 0;
            }
            (*DIR[mFinalVC])["MPXV"] = mFinalMpxv;
        }
        if(mFinalVC == emVcPWM && mFinalGroup == emGroup4){
            (*PWDDIR)["MPXE"] = mFinalMpxe;
            (*PWDDIR)["MPXV"] = mFinalMpxv;
        }
        //3.Data DRi, DIRn
        unsigned int prty = 0;

        prty = mID & 0x1 ;
        for(unsigned int loop = 1; loop < emIDbit; loop++){
            prty = prty ^ ((mID >> loop) & 0x1);
        }

        for(unsigned int loop = 0; loop <= emDRbit; loop++){
            prty = prty ^ ((mFinalData >> loop) & 0x1);
        }

        if(mFinalVC < mVirChannelNum) {
            for(unsigned int loop = 0; loop <= emMPXVbit; loop++){
                prty = prty ^ (((unsigned int)((*DIR[mFinalVC])["MPXV"]) >> loop) & 0x1);
            }
            prty = prty ^ ((unsigned int)((*DIR[mFinalVC])["IDEF"]) & 0x1);
            prty = prty ^ ((unsigned int)((*DIR[mFinalVC])["MPXE"]) & 0x1);
        }
        if(mFinalVC == emVcPWM && mFinalGroup == emGroup4){
            for(unsigned int loop = 0; loop <= emMPXVbit; loop++){
                prty = prty ^ (((unsigned int)((*PWDDIR)["MPXV"]) >> loop) & 0x1);
            }
            prty = prty ^ ((unsigned int)((*PWDDIR)["IDEF"]) & 0x1);
            prty = prty ^ ((unsigned int)((*PWDDIR)["MPXE"]) & 0x1);
        }
        prty = prty & 0x1;

        if(mFinalVC < mVirChannelNum){
            if ((mFinalVC & 0x1) != 0) {
                (*DR[mFinalVC/2])["DR1"] = mFinalData;
            } else {
                (*DR[mFinalVC/2])["DR0"] = mFinalData;
            }
            (* DIR[mFinalVC])["DRn"]  = mFinalData;
            (* DIR[mFinalVC])["WFLAG"] = 1;
            (* DIR[mFinalVC])["ID"]   = mID;
            (* DIR[mFinalVC])["PRTY"] = prty;
            re_printf("info","The DR%d, DIR%d registers are updated new data.\n", mFinalVC/2, mFinalVC);
        }
        if(mFinalVC == emVcPWM && mFinalGroup == emGroup4){
            (*PWDDR)["PWDDR"] = mFinalData;
            (*PWDDIR)["PWDDR"]  = mFinalData;
            (*PWDDIR)["WFLAG"] = 1;
            (*PWDDIR)["ID"]   = mID;
            (*PWDDIR)["PRTY"] = prty;
            PVCR_ENDm.write(1);
            PVCR_PWDDRm.write(mFinalData);
            re_printf("info","The PWDDR, PWDDIR registers are updated new data.\n");
        }
        if(mEnableConvertInfo){
            re_printf("info","The converted value: 0x%X.\n", mFinalData);
        }
        dfent = mFinalDfent;
        dftag = mFinalDftag;
#if 0
        asentsgxe = ((unsigned int)(*DFASENTSGER) >> mFinalGroup) & 0x1;
#else
        // RH850_U2A added
        gtmentsgxe = ((unsigned int)(*GTMENTSGER) >> mFinalGroup) & 0x1;
#endif
        data = mFinalData;
        if (mFinalDfmt == 2 || mFinalDfmt == 3) {
            data = data >> 1;
        }
#if 0
        if (dfent == 1 && asentsgxe == 1)
#else
        // RH850_U2A added
        if (dfent == 1 && gtmentsgxe == 1)
#endif
        {
            if (mFinalDfmt == 1 || mFinalDfmt >= 4) {
                re_printf("error", "When using GTM, setting to the signed 12 bit integer point format is prohibited.\n"); // RH850_E2x SARAD enhancement
            }
            DFREQm.write(1);
            DFTAGm.write(dftag);
            DFDATAm.write(data);
        }
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

        PVCR_ENDm.write(0);
        mErrorId = 0;
        mIsUpperError = false;
        mIsLowerError = false;
        mFinalPhyChannel = 0;

#if 0
        //3. RH850_E2x added INT_ULm
        if(mINT_ULmActive){
            DumpInterruptMessage("INT_UL", "", "Deassert");
            mINT_ULmActive = false;
            //INT_ULm.write(0);
        }
#endif

        //4. RH850_E2x added for ASF
        DFREQm.write(0);
    }
}//}}}

void Cadc_u2a::ADENDm0Method(void)
{//{{{
    if(mIsADReset){
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

void Cadc_u2a::ADENDm1Method(void)
{//{{{
    if(mIsADReset){
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

void Cadc_u2a::ADENDm2Method(void)
{//{{{
    if(mIsADReset){
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

void Cadc_u2a::ADENDm3Method(void)
{//{{{
    if(mIsADReset){
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

void Cadc_u2a::ADENDm4Method(void)
{//{{{
    if(mIsADReset){
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

void Cadc_u2a::IFCProcessMethod(void) // RH850_E2x added for processing IFC
{//{{{
    if(mIsPReset && (mADCInstance < mADCNum)){
        ADCm_IFDATA.write(0);
    }else{
        if (IFRDm.read() && IFREm.read()) {
            unsigned int vc_number = (unsigned int)IFVCHm.read();
            if (vc_number < mVirChannelNum) {
                unsigned int data = (unsigned int)(*DIR[vc_number])["DRn"];
                if ((*ADCR2)["DFMT"] == 1 || (*ADCR2)["DFMT"] >= 4) {
                    re_printf("error", "When using IFC, setting to the integer point format is prohibited.\n"); // RH850_E2x SARAD enhancement
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
                    (*DIR[vc_number])["WFLAG"] = 0;
                    re_printf("info","The DIR%d.WFLAG is cleared to 0.\n", vc_number);
                }
            } else {
                re_printf("warning","The IFC read request virtual channel must be equal or less than %d.\n", mVirChannelNum - 1);
                ADCm_IFDATA.write(0);
            }
        }
    }
}//}}}

void Cadc_u2a::SVmModeMethod(void)
{//{{{
    if ((!mIsPReset) && (mADCInstance < mADCNum)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(SV_MODEm.read() == 0x1) {
                mEnterSVModeFlag = true;
            }else {
                mExitSVModeFlag = true;
            }
            mSVModeEvent.notify((unsigned int)(emtSVSACTCoeff * mPCLKPeriod),SC_NS);
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2a::SVStopMethod(void)
{//{{{
    if ((!mIsPReset) && (mADCInstance < mADCNum)) {
        if(mEnterSVModeFlag) {
            mEnterSVModeFlag = false;
            (*SGSTR)["SVSACT"] = 0x1;
        }
        if(mExitSVModeFlag) {
            mExitSVModeFlag = false;
            (*SGSTR)["SVSACT"] = 0x0;
            for(unsigned int i=0;i<emGroupNum;i++){
                unsigned group = (emGroupNum-1) - i;
                RunRetainedTrigger(group);
            }
        }
    }
}//}}}

void Cadc_u2a::RunRetainedTrigger(const unsigned int group_number)
{//{{{
    if(mIsRetainedTrigger[group_number] && mPCLKPeriod > 0 && mADCLKPeriod > 0){
        mIsRetainedTrigger[group_number] = false;
        if(mIsRetainedSW[group_number]){
            mIsRetainedSW[group_number] = false;
            if (TimingCheck() && VoltageCheck()){
                if(mIsRetainedADsyn[group_number]){
                    mIsRetainedADsyn[group_number] = false;
                    if((*SGCR[group_number])["ADSTARTE"] == 1){
                        GroupCheck(group_number);
                    }
                }else if(mIsRetainedSWTHA[group_number]) {
                    mIsRetainedSWTHA[group_number] = false;
                    if (((*THACR)["HLDCTE"] == 1) && ((*THACR)["HLDTE"] == 0)) {
                        mTHASoftwareTriggerHolding = true;
                        mTHASoftwareTriggerHoldingEvent.notify();
                    }
                }
                #if 0    //Update for HWM0.61 - T&H Group B is removed
                else if(mIsRetainedSWTHB[group_number]) {
                    mIsRetainedSWTHB[group_number] = false;
                    if (((*THBCR)["HLDCTE"] == 1) && ((*THBCR)["HLDTE"] == 0)) {
                        mTHBSoftwareTriggerHolding = true;
                        mTHBSoftwareTriggerHoldingEvent.notify();
                    }
                }
                #endif
                else {
                    GroupCheck(group_number);
                }
            }
        } else {
            if(mIsRetainedPWD){
                mIsRetainedPWD = false;
                if (group_number == emGroup4 && (*PWDCR)["PWE"] == 1 && (*PWDSGCR)["TRGMD"] == 1) {
                    mPWMFlag = true;
                    GroupCheck(emGroup4);
                }
            }else if(mIsRetainedHWTHA[group_number]) {
                mIsRetainedHWTHA[group_number] = false;
                if ((((*THACR)["HLDCTE"] == 1)&&(*THACR)["HLDTE"] == 1)&&((*THACR)["SGS"] == (group_number - 1))) {
                    mTHAPortTriggerHolding = true;
                    mTHAPortTriggerHoldingEvent.notify();
                }
            }
            #if 0 //Update for HWM0.61 - T&H Group B is removed
            else if(mIsRetainedHWTHB[group_number]) {
                mIsRetainedHWTHB[group_number] = false;
                if ((((*THBCR)["HLDCTE"] == 1)&&(*THBCR)["HLDTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))) {
                    mTHBPortTriggerHolding = true;
                    mTHBPortTriggerHoldingEvent.notify();
                }
            } 
            #endif
            else{
                if ((mADCInstance == 2) && ((*SGTSEL[group_number]) != 0x10)) {
                    HardwareTrigger(group_number,4);
                } else {
                    HardwareTrigger(group_number,3);
                }
            }
        }
    }
}//}}}
//tlm_tgt_if function
bool Cadc_u2a::common_tgt_rd(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size )
{ //{{{
    sc_assert(p_data != NULL);
    if(is_dbg){
        // Set debug mode (member variables of Cadc_u2a_regif class)
        return reg_rd_dbg(addr, p_data, size);
    }else{
        bool is_read_success = true;
        unsigned int tmp_add = addr & 0x7FF;

        /// SARAD4 add
        // Check reading from VCRn/DRi/DIRn register
        if (((mVirChannelNum << 2) <= tmp_add) && (tmp_add <= 0x17C)) {
            re_printf("error","Cannot read VCR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)(tmp_add >> 2));
            return false;
        } else if (((0x1A0 + (mVirChannelNum << 1)) <= tmp_add) && (tmp_add <= 0x25C)) {
            re_printf("error","Cannot read DR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x1A0) >> 2));
            return false;
        } else if (((0x280 + (mVirChannelNum << 2)) <= tmp_add) && (tmp_add <= 0x3FC)) {
            re_printf("error","Cannot read DIR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x280) >> 2));
            return false;
        }

        is_read_success = reg_rd(addr, p_data, size);
        if (is_read_success) {
            if ((tmp_add >= 0x1A0) && (tmp_add < (0x1A0 + (mVirChannelNum << 1)))) {
                CallBack_DRi(tmp_add);
            } else if ((tmp_add >= 0x280) && (tmp_add < (0x280 + (mVirChannelNum << 2)))) {
                CallBack_DIRn(tmp_add);
            } else if (tmp_add == 0x260) {
                CallBack_PWDDR();
            } else if (tmp_add == 0x400) {
                CallBack_PWDDIR();
            }
        }
        return is_read_success;
    }
} //}}}

bool Cadc_u2a::common_tgt_wr(bool is_dbg, unsigned int addr, unsigned char *p_data, unsigned int size )
{//{{{
    sc_assert(p_data != NULL);
    if(is_dbg){
        return reg_wr_dbg(addr, p_data, size);
    }else{
        /// SARAD4 add
        unsigned int tmp_add = addr & 0x7FF;
        // Check writing to VCRn/DRi/DIRn register
        if (((mVirChannelNum << 2) <= tmp_add) && (tmp_add <= 0x17C)) {
            re_printf("error","Cannot write to VCR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)(tmp_add >> 2));
            return false;
        } else if (((0x1A0 + (mVirChannelNum << 1)) <= tmp_add) && (tmp_add <= 0x25C)) {
            re_printf("error","Cannot write to DR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x1A0) >> 2));
            return false;
        } else if (((0x280 + (mVirChannelNum << 2)) <= tmp_add) && (tmp_add <= 0x3FC)) {
            re_printf("error","Cannot write to DIR%d register whose index exceeds maximum virtual channel.\n", (unsigned int)((tmp_add - 0x280) >> 2));
            return false;
        }

        return reg_wr(addr, p_data, size);
    }
}//}}}

bool Cadc_u2a::ChkAccess (const bool is_wr, const std::string channel_name, const std::string register_name, const std::string bit_name)
{//{{{
    // This code is used if multiple product is used (eg: U2A, U2B, ...)
#if 0
    if(is_wr){
        bool wr_cond = true;
        for(unsigned int group_number =0; group_number < emGroupNum; group_number++) {
            if ((mTargetProduct == "RH850_E2x") || (mTargetProduct == "RH850_E2x-468Pins")) {
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
        }else{
            if (bit_name != "") {
                re_printf("warning","Writing into %s.%s bit is not affected.\n", register_name.c_str(),bit_name.c_str());
            }else{
                re_printf("warning","Writing into %s register is not affected.\n", register_name.c_str());
            }

            return false;
        }
        return false;
    }else{
        return true;
    }
#else
    return true;
#endif
}//}}}

//handleCommand
void Cadc_u2a::AssignTHCircuit(const unsigned int th_channel, const unsigned int group_channel, const unsigned int physic_channel)
{//{{{
    if (th_channel >= mTHChannelNum) {
        re_printf("error","Invalid T&H channel %d, it must be less than %d.\n", th_channel, mTHChannelNum);
        return;
    }
    unsigned int analog_channel = group_channel << 2 | physic_channel;
    if (analog_channel >= emMaxPhyChannel) {
        re_printf("error","Invalid analog channel AN%1d%1d%1d for TH Circuit.\n", mADCInstance, group_channel, physic_channel);
        return;
    }
    //if (group_channel >= (emMaxPhyChannel/4)) {
    //    re_printf("error","Invalid group channel %d, it must be less than %d.\n", group_channel, emMaxPhyChannel/4);
    //    return;
    //}
    //if (physic_channel >= 4) {
    //    re_printf("error","Invalid physic channel %d, it must be less than 4.\n", physic_channel);
    //    return;
    //}
    THzAssignChannel[th_channel] = 4*group_channel + physic_channel;
}//}}}

//operation functions
void Cadc_u2a::HardwareTrigger (const unsigned int group_number, const unsigned int trigger)
{//{{{
    /// SARAD3
    double start_delay = 0;
    if (mEnableTimeCalculation) {
        // --> Confirm
        start_delay = emtDCoeffADCLK * mADCLKPeriod;
        // <-- Confirm
    } else {
        start_delay = mtD;
    }

    if(TimingCheck() && VoltageCheck()){
        //0.Counting end : trigger = 0
        if((trigger == 0)&&(((*SGCR[group_number])["TRGMD"] == 2)||((*SGCR[group_number])["TRGMD"] == 3))){
            mIsHWTrigger[group_number] = true;
            mHWTriggerEvent[group_number].notify((unsigned int)start_delay, SC_NS);
            //mHWTriggerEvent[group_number].notify();
        //1.Group A Holding end : trigger = 1
        } else if(trigger == 1){
            //1.1.Trigger scanning process
            if(((*THACR)["HLDCTE"] == 1)&&((*THACR)["SGS"] == (group_number - 1))&&((*SGCR[group_number])["TRGMD"] == 1)){
                GroupCheck(group_number);
            }
        //2.Group B Holding end : trigger = 2
        }
        #if 0  //Update for HWM0.61 - T&H Group B is removed
        else if(trigger == 2){
            //2.1.Trigger scanning process
            if(((*THBCR)["HLDCTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))&&((*SGCR[group_number])["TRGMD"] == 1)){
                GroupCheck(group_number);
            }

        //3.External input port trigger : ADmSG0
        }
        #endif
        else if((trigger == 3)&&(group_number == 0)){
            //3.1.Trigger scanning process
            if (mADCInstance == 2) {
                if ((*SGTSEL[group_number] == 0x10) && ((*SGCR[group_number])["TRGMD"] == 1)) {
                    if(!CheckScanMode(group_number)){
                        GroupCheck(group_number);
                    }
                } else {
                    re_printf("warning","Hardware trigger is disabled\n");
                }
            } else {
                if ((*SGCR[group_number])["TRGMD"] == 1) {
                    if(!CheckScanMode(group_number)){
                        GroupCheck(group_number);
                    }
                }
            }

        //4.External input port trigger : ADmSG1/2/3/4
        }else if((trigger == 3)&&(group_number != 0)){
            bool trigger_enable = false;
            //4.1.Trigger group A holding process
            if ((((*THACR)["HLDCTE"] == 1)&&(*THACR)["HLDTE"] == 1)&&((*THACR)["SGS"] == (group_number - 1))) {
                if(!CheckScanMode(group_number,false,false,false,true)){
                    mTHAPortTriggerHolding = true;
                    mTHAPortTriggerHoldingEvent.notify();
                }
            //4.2.Trigger group B holding process
            } 
            #if 0 //Update for HWM0.61 - T&H Group B is removed 
            else if ((((*THBCR)["HLDCTE"] == 1)&&(*THBCR)["HLDTE"] == 1)&&((*THBCR)["SGS"] == (group_number - 1))) {
                if(!CheckScanMode(group_number,false,false,false,false,true)){
                    mTHBPortTriggerHolding = true;
                    mTHBPortTriggerHoldingEvent.notify();
                }
            //4.3.Trigger counting process or scanning process
            } 
            #endif
            else if ((((*THACR)["HLDCTE"] == 0) && ((*THBCR)["HLDCTE"] == 0)) ||
            (((*THACR)["HLDCTE"] == 0) && ((*THBCR)["SGS"] != (group_number - 1))) ||
            (((*THBCR)["HLDCTE"] == 0)&&((*THACR)["SGS"] != (group_number - 1))) 
            #if 0 //Update for HWM0.61
            || (((*THBCR)["SGS"] != (group_number -1)) && ((*THACR)["SGS"] != (group_number -1)))
            #endif
            ) {
                trigger_enable = true;
            }

            if(trigger_enable){
                if (mADCInstance == 2) {
                    if (*SGTSEL[group_number] == 0x10) {
                        if((*SGCR[group_number])["TRGMD"] == 3){
                            ADtimerCheck(group_number);
                        }else if((*SGCR[group_number])["TRGMD"] == 1){
                            if(!CheckScanMode(group_number)){
                                GroupCheck(group_number);
                            }
                        }
                    } else {
                        re_printf("warning","Hardware trigger is disabled\n");
                    }
                } else {
                    if((*SGCR[group_number])["TRGMD"] == 3){
                        ADtimerCheck(group_number);
                    }else if((*SGCR[group_number])["TRGMD"] == 1){
                        if(!CheckScanMode(group_number)){
                            GroupCheck(group_number);
                        }
                    }
                }
            }
        //5.External input port trigger ADC2
        }else if((trigger == 4)&&(group_number == 0)){
            //3.1.Trigger scanning process
            if ((*SGCR[group_number])["TRGMD"] == 1) {
                if(!CheckScanMode(group_number)){
                    GroupCheck(group_number);
                }
            }

        //6.External input port trigger : ADmSG1/2/3/4
        }else if((trigger == 4)&&(group_number != 0)){
            if((*SGCR[group_number])["TRGMD"] == 3){
                ADtimerCheck(group_number);
            }else if((*SGCR[group_number])["TRGMD"] == 1){
                if(!CheckScanMode(group_number)){
                    GroupCheck(group_number);
                }
            }
        }
    }
}//}}}

void Cadc_u2a::GroupCheck (const unsigned int group_number)
{//{{{
    unsigned int start_vc = 0;
    unsigned int end_vc   = 0;

    if (mIsUpdateSGVCPR[group_number]) {
        mIsUpdateSGVCPR[group_number] = false;
    }
    //Check the VC condition
    if(mIsScanningProcess[group_number]) {
        re_printf("warning","Trigger scanning process for group %d is ignored because group %d is in scanning process.\n", group_number, group_number);
        if((unsigned int)((*TOCCR)["TOCE"]) == 1){
            (*TOCER)["TOCESG"] = (unsigned int)((*TOCER)["TOCESG"]) | (0x1 << group_number);
#if 0
            if((unsigned int)((*SFTCR)["TOCEIE"]) == 1 && !mINT_ADEmProcess) {
                mINT_ADEmActive = true;
                mINT_ADEmEvent.notify();
            }
            else {
                re_printf("info","INT_ADEm is disable or in process.\n");
            }
#else
            if((unsigned int)((*SFTCR)["TOCEIE"]) == 1 && !mINT_ERRmProcess) {
                mINT_ERRmActive = true;
                mINT_ERRmEvent.notify();
            }
            else {
                re_printf("info","INT_ERRm is disable or in process.\n");
            }
#endif
        }
    }else{
        re_printf("info","The group %d is triggered for scanning process.\n", group_number);
        if(mPWMFlag && group_number == emGroup4){
            mPWMFlag = false;
            PVCR_VALUEmUpdate();
            start_vc = emVcPWM;
            end_vc = emVcPWM;
        } else {
            start_vc = (unsigned int)((*SGVCPR[group_number])["VCSP"]);
            end_vc   = (unsigned int)((*SGVCPR[group_number])["VCEP"]);
        }
        (*SGSR[group_number])["SGACT"] = 1;
        (*SGSTR)["SGACT"] = (unsigned int)((*SGSTR)["SGACT"]) | (0x1 << group_number);
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

bool Cadc_u2a::VCCheck (const unsigned int vc_number)
{//{{{
    unsigned int cnvcls = 0;
    unsigned int gctrl = 0;
    if(vc_number == emVcPWM) {
        cnvcls = emPWSDMode;
        gctrl = (unsigned int)((*PWDVCR)["GCTRL"]);
    }
    if(vc_number < mVirChannelNum){
        cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
        gctrl = (unsigned int)((*VCR[vc_number])["GCTRL"]);
    }
    /// SARAD4 modify
    //unsigned int tmp_gctrl = gctrl & 0x1F;
    unsigned int tmp_gctrl = gctrl;
    bool is_correct = true;

    switch (cnvcls) {
        case emTHMode:
            if ((3 < gctrl) && (gctrl < 8)) {
                re_printf("warning","The setting of VCR%d.CNVCLS and VCR%d.GCTRL is not matched in the T&H conversion mode.\n", vc_number, vc_number);
                is_correct = false;
            } else {
                if((gctrl & 0x38) != 0){
                    re_printf("warning","The VCR%d.GCTRL[5:3] bits should be zero.\n", vc_number);
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
        case emExSamplingPeriodMode:
        case emTerminalDiagMode:
        case emDisconnectionMode:
        case emPdMode:
        case emPuMode:
        case emDisconnectionDiagMode:
        case emPdDiagMode:
        case emPuDiagMode:
        case emPWSDMode:
            if (tmp_gctrl >= mPhyChannelNum) {
                if(vc_number == emVcPWM && cnvcls == emPWSDMode) {
                    re_printf("error","PWDVCR.GCTRL[4:0] = %d is greater than maximum analog channel index.\n", tmp_gctrl);
                } else {
                    re_printf("error","VCR%d.GCTRL[4:0] = %d is greater than maximum analog channel index.\n", vc_number, tmp_gctrl);
                }
                is_correct = false;
            }
            break;
        case emDataPathMode:
            if((gctrl & 0x3E) != 0){
                re_printf("warning","The VCR%d.GCTRL[5:1] bits should be zero.\n", vc_number);
            }
            break;
        default:
            break;
    }

    //if ((gctrl & 0x20) != 0) {
    //    re_printf("warning","The GCTRL[5] bits should be zero.\n");
    //}

    return is_correct;
}//}}}

unsigned int Cadc_u2a:: VCAddition (const unsigned int vc_number)
{//{{{
    unsigned int cnvcls = 0;
    bool addnt = (bool)((*ADCR2)["ADDNT"]);
    unsigned int vc_add_number = 1;
    if(vc_number < mVirChannelNum){
        cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
    }
    if(vc_number == emVcPWM) {
        return vc_add_number;
    }

    if ((cnvcls == emAdditionMode) || (cnvcls == emMPXAdditionMode)
      ||(emDisconnectionMode <= cnvcls && cnvcls <= emPuDiagMode && (*ODCR)["WADDE"] == 1) ) {
        if (addnt) {
            vc_add_number = 4;
        } else {
            vc_add_number = 2;
        }
    }
    return vc_add_number;
}//}}}

unsigned int Cadc_u2a::GetData (const unsigned int group_number, const unsigned int vc_number, const unsigned int vc_add)
{//{{{
    unsigned int cnvcls = 0;
    unsigned int gctrl  = 0;
    if(vc_number == emVcPWM && group_number == emGroup4) {
        cnvcls = emPWSDMode;
        gctrl  = (unsigned int)((*PWDVCR)["GCTRL"]);
    }
    if(vc_number < mVirChannelNum){
        cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
        gctrl  = (unsigned int)((*VCR[vc_number])["GCTRL"]);
    }
    unsigned int thgsr  = (unsigned int)(*THGSR);
    double tmp_data = 0;
    unsigned int tdlv   = (unsigned int)(*TDCR)["TDLV"];
    //unsigned int index = gctrl & 0x1F;
    unsigned int index = gctrl;
    /// SARAD4 modify
    double data = 0;
    unsigned int dataSelfPath = 0;
    bool normal_convert = false;
    double ratio = 0;
    mPhyChannel = index;
    //1.Get data
    if (cnvcls == emNormalMode || cnvcls == emAdditionMode || cnvcls == emMPXMode || cnvcls == emMPXAdditionMode
     || cnvcls == emExSamplingPeriodMode || cnvcls == emDisconnectionMode
     || cnvcls == emDisconnectionDiagMode || cnvcls == emPuDiagMode || cnvcls == emPdDiagMode
     || cnvcls == emPdMode || cnvcls == emPuMode || cnvcls == emPWSDMode) {
        // --> Confirm
        if (mOpenCircuitError) {
            if (cnvcls == emDisconnectionMode || cnvcls == emPdMode) {
                data = 0;
            } else if (cnvcls == emPuMode) {
                data = mAVcc;
            } else {
                normal_convert = true;
            }
        }
        else if ((*ODCR)["ODDE"] == 1) {
            if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode) {
                data = 0;
            }
            else if (cnvcls == emPuDiagMode) {
                data = mAVcc;
            }
            else {
                normal_convert = true;
            }
        }
        else {
            normal_convert = true;
        }
        if(normal_convert) {
            data = LoadAnalogTerminal(index);
            if (((index == emVCCPortNum)
                || (index == emEVCCPortNum)
                || (index == emISOVDDPortNum)
                || (index == emAWOVDDPortNum))
             && (mADCInstance == 0)) { // VCC, EVCC, VDD of RH850_E2x Enhancement
                data = AdjustVMONInput(data, index);
            }
        }
        // <-- Confirm
    } else if (cnvcls == emTHMode) {
        // --> Confirm
        gctrl = gctrl & 0x7;
        thgsr = (thgsr >> (2*gctrl))&0x1;
            //check matching between T&H setting and VC setting
        if (mTHChannelState[gctrl] == emTHIdleState) {
            re_printf("warning","The TH channel %d is in IDLE state, input data is all 0.\n", gctrl);
            tmp_data = 0;
            #if 0
        } else if (((unsigned int)((*THACR)["SGS"]) == (unsigned int)((*THBCR)["SGS"])) || (group_number == 0) || ((thgsr == 0) && ((*THACR)["SGS"] != (group_number - 1))) ||
        ((thgsr == 1) && ((*THBCR)["SGS"] != (group_number - 1)))) {
          #else   //Update for HWM 0.61
        } else if ((group_number == 0) || ((*THACR)["SGS"] != (group_number - 1))) {
        #endif
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
        mPhyChannel = mPhyChannelTH[gctrl];
        // --> Confirm
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
        mPhyChannel = 0x3F;
    }
    else if (cnvcls == emTerminalDiagMode) {
        //the self-diagnosis function
        /// SARAD4 modify
        data = SelfDiagData(index,tdlv);
    // --> Confirm
    //} else if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode) {
    //    data = 0;
    //} else if (cnvcls == emPuDiagMode) {
    //    data = mAVcc;
    }
    // <-- Confirm

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

    if (cnvcls == emDataPathMode) {
        if ((gctrl != 0x0) && (gctrl != 0x1)) {
            re_printf("warning","When using A/D conversion data path diagnosis mode, writing other than 0x0 or 0x1 to VCR.GCTRL bit is prohibited.\n");
        }
        gctrl = gctrl & 0x1;
        if (gctrl == 0x1) {
            dataSelfPath = 0x555;
        } else {
            dataSelfPath = 0xAAA;
        }
        mPhyChannel = 0x3F;
    }
    //3.Correct format of data
    unsigned int final_data = 0;
    if((*ADCR2)["DFMT"] == 1 || (*ADCR2)["DFMT"] >= 4){
        if(is_all_1){
            final_data = emIntegerFormatAllOne;
        }else{
            if (cnvcls == emDataPathMode) {
                final_data = (unsigned int)(dataSelfPath);
            } else if (cnvcls == emDiagnosisMode) {
                final_data = (unsigned int)((unsigned int)(ratio * emIntegerFormat) & emIntegerFormatAllOne);
            } else {
                final_data = (unsigned int)((unsigned int)((data/mAvrefh) * emIntegerFormat) & emIntegerFormatAllOne);
            }
        }
    }else{ // DFMT = 0, 2, 3
        if(is_all_1){
            final_data = emFixFormatAllOne;
        }else{
            if (cnvcls == emDataPathMode) {
                #if 0  //update for HWM0.61
                if((*ADCR2)["DFMT"] == 3){
                    re_printf("warning","ADCR2.DFMT should be 12 bit format when using Data Path Diagnosis Mode.\n");
                }
                #endif
                final_data = (unsigned int)((unsigned int)dataSelfPath << 3);
            } else if (cnvcls == emDiagnosisMode) {
                final_data = (unsigned int)((unsigned int)(ratio * emFixFormat) & emFixFormatAllOne);
            } else {
                final_data = (unsigned int)((unsigned int)((data/mAvrefh) * emFixFormat) & emFixFormatAllOne);
            }
        }
    }
    return final_data;

}//}}}

void Cadc_u2a::ADtimerCheck (const unsigned int group_number)
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
        (*SGSTR)["ADTACT"] = (unsigned int)((*SGSTR)["ADTACT"]) | (0x1 << (group_number-3));
        mADCStateEvent.notify();
        mTIMER[group_number - 3]->mPeriod = mADCLKPeriod;

        mTIMER[group_number - 3]->mNextValue = (unsigned int)((*ADTPRR[group_number])["ADTPR"]);
        mTIMER[group_number - 3]->StartCounting((unsigned int)((*ADTIPR[group_number])["ADTIP"]));
        re_printf("info","The counting of AD timer %d is started.\n", group_number);
    }
}//}}}

double Cadc_u2a::LoadAnalogTerminal (const unsigned int index)
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
        case 32:
            data = ANm80.read();
            break;
        case 33:
            data = ANm81.read();
            break;
        default:
            break;
    }

    return data;
}//}}}

double Cadc_u2a::AdjustVMONInput(const double data, const unsigned int index)
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

void Cadc_u2a::ExportData(const unsigned int case_number, const unsigned int group_number, const unsigned int vc_number, unsigned int *data, const unsigned int group_loop )
{//{{{
    unsigned int cnvcls = 0;
    unsigned int gctrl  = 0;
    unsigned int mpxe = 0;
    unsigned int mpxv  = 0;
    unsigned int dfent = 0;
    unsigned int dftag = 0;
    unsigned int vc_add_number = 0;
    unsigned int ullmtbs = 0;
    unsigned int ulmtb = 0;
    unsigned int llmtb = 0;
    unsigned int convt_data = 0;
    unsigned int pin_number = 0;
    double finish_delay = 0;
    unsigned int dfmt = (unsigned int)((*ADCR2)["DFMT"]);
    bool isUpperError = false;
    bool isLowerError = false;

    vc_add_number = VCAddition(vc_number);

    if(vc_number == emVcPWM && group_number == emGroup4) {
        cnvcls = emPWSDMode;
        gctrl  = (unsigned int)((*PWDVCR)["GCTRL"]);
        mpxv  = (unsigned int)((*PWDVCR)["MPXV"]);
        mpxe  = (unsigned int)((*PWDVCR)["MPXE"]);
    }
    if(vc_number < mVirChannelNum){
        cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
        gctrl  = (unsigned int)((*VCR[vc_number])["GCTRL"]);
        mpxv  = (unsigned int)((*VCR[vc_number])["MPXV"]);
#if 0
        dfent = (unsigned int)((*VCR[vc_number])["DFENT"]);
        dftag = (unsigned int)((*VCR[vc_number])["DFTAG"]);
#else
        // RH850_U2A added
        dfent = (unsigned int)((*VCR[vc_number])["GTMENT"]);
        dftag = (unsigned int)((*VCR[vc_number])["GTMTAG"]);
#endif
    }
    if (mEnableTimeCalculation) {
        finish_delay = (emtEDCoeffPCLK * mPCLKPeriod) + (emtEDCoeffADCLK * mADCLKPeriod);
    } else {
        finish_delay = mtED;
    }
    switch (case_number) {
        case emStartConvert: //First time --> INT_MPXm, ADENDmx, MPXCURR
            //1.MPXCURR and INT_MPX
            if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode) || (cnvcls == emPWSDMode && mpxe == 1)) {
                //1.1.MPXCURR
                // RH850_E2x added
                (*MPXCURR1)["MPXCMD"] = (unsigned int)((*MPXCMDR)["MPXCMD"]);
                (*MPXCURR1)["MPXCUR"] = mpxv;
                (*MPXCURR2)["MPXCUR"] = mpxv;
                mADMPXOutputEvent.notify(); //add for FCC1/FCC2
                //1.2.INT_MPX
                if (mIsINT_MPXmOperating) {
                    re_printf("warning","INT_MPX%d is not asserted because INT_MPX%d interrupt is in process.\n", mADCInstance, mADCInstance);
                } else if ((unsigned int)(*MPXINTER)["ADMPXIE"] == 1){ // RH850_E2x added for condition of asserting INT_MPX
                    mIsINT_MPXmOperating = true;
                    mINT_MPXmActive = true;
                    mINT_MPXmInterruptEvent.notify();
                }
            }
            //2.ADENDmx
            for (pin_number = 0; pin_number < emADENDPinNum; pin_number++) {
                if((vc_number == (unsigned int)((*ADENDP[pin_number])["ENDP"]))
                        && ((mLastVC != vc_number) || (mLastGroup != group_number)))
                {
                    mADENDmxActive[pin_number] = true;
                    mADENDmxEvent[pin_number].notify();
                }
            }
            // --> Confirm
            if (mForcedOpenError) {
                if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode || cnvcls == emPuDiagMode) {
                    if ((*ODCR)["ODDE"] == 1) {
                        mOpenCircuitError = false;
                    } else {
                        mOpenCircuitError = true;
                    }
                } else if (cnvcls != emDisconnectionMode) {
                    mOpenCircuitError = true;
                }
            }
            else {
                mOpenCircuitError = false;
            }
            // <-- Confirm
            break;
        case emCorruptConvert:
            //1.Clear ADENDmxEvent
            for (pin_number = 0; pin_number < emADENDPinNum; pin_number++) {
                if(vc_number == (unsigned int)((*ADENDP[pin_number])["ENDP"])) {
                    mADENDmxActive[pin_number] = false;
                    mADENDmxEvent[pin_number].notify();
                }
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
            // --> Confirm
            if ((vc_add_number == 2) && (mStoreData == 0x1FFE) && (dfmt == 1 || dfmt >= 0x4)) {
                mStoreData = 0x1FFF;
            } else if ((vc_add_number == 4) && (mStoreData == 0x3FFC) && (dfmt == 1 || dfmt >= 0x4)) {
                mStoreData =  0x3FFF;
            } else if ((vc_add_number == 2) && (mStoreData == 0x0FFF0) && ((dfmt == 0) || (dfmt == 2) || (dfmt == 3))) {
                mStoreData =  0x7FFC;
            } else if ((vc_add_number == 4) && (mStoreData == 0x1FFE0) && ((dfmt == 0) || (dfmt == 2) || (dfmt == 3))) {
                mStoreData =  0x7FFE;
            } else if ((dfmt == 0) || (dfmt == 2) || (dfmt == 3)) {
                mStoreData = mStoreData >> (vc_add_number >> 1);
            }
            convt_data = mStoreData; // format: 7FF8/7FFC/7FFE for fixed-point and FFF/1FFF/3FFF for integer
            // RH850_E2x added for support DFMT = 2(12bit unsigned format) and DFMT = 3(10 bits unsigned format)
            if (dfmt == 2) {
                mStoreData = mStoreData << 1;
            }
            #if 0 //update for HWM0.61
            else if (dfmt == 3) {
                mStoreData = mStoreData << 1;
                if (vc_add_number == 4) {
                    mStoreData = mStoreData & 0xFFF0;
                    convt_data = convt_data & 0xFFF8;
                } else if (vc_add_number == 2) {
                    mStoreData = mStoreData & 0xFFE0;
                    convt_data = convt_data & 0xFFF0;
                } else {
                    mStoreData = mStoreData & 0xFFC0;
                    convt_data = convt_data & 0xFFE0;
                }
            } 
            #endif
            else if (dfmt == 0x5) {
                if (vc_add_number == 4) {
                    mStoreData = mStoreData << 2;
                } else if (vc_add_number == 2) {
                    mStoreData = mStoreData << 3;
                } else {
                    mStoreData = mStoreData << 4;
                }
            }
            #if 0 //Update for HWM0.61
            else if (dfmt == 0x6) {
                mStoreData = mStoreData >> 2;
                convt_data = convt_data >> 2;
            } else if (dfmt == 0x7) {
                if (vc_add_number == 4) {
                    mStoreData = (mStoreData << 2) & 0xFFF0;
                } else if (vc_add_number == 2) {
                    mStoreData = (mStoreData << 3) & 0xFFE0;
                } else {
                    mStoreData = (mStoreData << 4) & 0xFFC0;
                }
                convt_data = convt_data >> 2;
            }
            #endif 
            // <-- Confirm
            mFinalVC = vc_number;
            mFinalGroup = mLastGroup;
            if(mTHUpdateChannelState[0] || mTHUpdateChannelState[1] || mTHUpdateChannelState[2] || mTHUpdateChannelState[3] ||
            mTHUpdateChannelState[4] || mTHUpdateChannelState[5]){
                mTHUpdateChannelStateFlag = true;
                mTHUpdateChannelStateEvent.notify();
            }
            //2.ADENDmx
            // --> Confirm : why (mStartVCPointer[group_number] != mEndVCPointer[group_number])
            for (pin_number = 0; pin_number < emADENDPinNum; pin_number++) {
                if((vc_number == (unsigned int)((*ADENDP[pin_number])["ENDP"]))
                        && (mSuspendFlag || (mStartVCPointer[group_number] != mEndVCPointer[group_number]) || (group_loop == 1 && (*SGCR[group_number])["SCANMD"] == 0))) {
                    mADENDmxActive[pin_number] = false;
                    mADENDmxEvent[pin_number].notify();
                }
            }
            // <-- Confirm
            //3.INT_ADmx, INT_ADEm, DRi, ,DIRn
            //3.1.Check ID error
            if(mForcedIdError){
                mErrorId = mErrorId | 0x1;
            }
            //3.2.Check Overwire error
            if(vc_number < mVirChannelNum){
                if((*DIR[vc_number])["WFLAG"] == 1){
                    mErrorId = mErrorId | 0x4;
                }
            }

            //3.3.Check boundary error
            if(vc_number == emVcPWM && group_number == emGroup4){
                ullmtbs = (unsigned int)((*PWDVCR)["VCULLMTBS"]);
            }
            if(vc_number < mVirChannelNum){
                ullmtbs = (unsigned int)((*VCR[vc_number])["VCULLMTBS"]);
            }
            if (ullmtbs != 0) {
                ulmtb = (unsigned int)(*VCULLMTBR[ullmtbs-1])["VCULMTB"];
                llmtb = (unsigned int)(*VCULLMTBR[ullmtbs-1])["VCLLMTB"];
                ulmtb = CheckVCULLMTBR(ullmtbs - 1, true, ulmtb, vc_add_number);
                llmtb = CheckVCULLMTBR(ullmtbs - 1, false, llmtb, vc_add_number);
                if(ulmtb < llmtb){
                    re_printf("warning","Upper/Lower Limit Check does not occur because VCULLMTBR%d.VCULMTB < VCULLMTBR%d.VCLLMTB\n",ullmtbs - 1, ullmtbs - 1);
                }else {
                    mCurVC = vc_number;
                    mVCENDNoticeEvent.notify((unsigned int)finish_delay, SC_NS);
                    mFinalPhyChannel = mPhyChannel;
                    mPhyChannel = 0x0;
                    if(convt_data > ulmtb) {
                        mErrorId = mErrorId | 0x8;
                        isUpperError = true;
                        mULMTB = true;
                        mBoundaryExcessEvent.notify((unsigned int)finish_delay, SC_NS);
                        // RH850_U2A added
                        if ((mULE_LPSmActive & 0x1F) == 0) {
                            mULE_LPSmActive = 1 << mFinalGroup;
                            mULE_LPSmEvent.notify((unsigned int)finish_delay, SC_NS);
                        }
                    }
                    if(convt_data < llmtb) {
                        mErrorId = mErrorId | 0x8;
                        isLowerError = true;
                        mLLMTB = true;
                        mBoundaryExcessEvent.notify((unsigned int)finish_delay, SC_NS);
                        // RH850_U2A added
                        if ((mULE_LPSmActive & 0x1F) == 0) {
                            mULE_LPSmActive = 1 << mFinalGroup;
                            mULE_LPSmEvent.notify((unsigned int)finish_delay, SC_NS);
                        }
                    }
                }
            }

            //3.4.Check INT_AD interrupt
            if(vc_number < mVirChannelNum) {
                if (  (unsigned int)((*VCR[vc_number])["ADIE"]) == 1
                    ||(vc_number == mEndVCPointer[group_number] && (unsigned int)((*SGCR[group_number])["ADIE"]) == 1)
                   ) {
                    mINT_ADmxActive[group_number] = true;
                }
            }
            if(vc_number == emVcPWM && group_number == emGroup4) {
                mINT_ADmxActive[group_number] = true;
            }
            if (cnvcls == emTHMode) {
                mID = (gctrl & 0x7);
            } else {
                mID = (gctrl & 0x3F);
            }

            // Check Break error
            if (mForcedBreak && (*ODCR)["ODDE"] == 1) {
                // --> Confirm
                if (cnvcls == emDisconnectionDiagMode || cnvcls == emPdDiagMode || cnvcls == emPuDiagMode) {
                    mStoreData = 0;
                }
            }
            if(mForcedDataPathError) {
                mStoreData  = 0;
            }
            // <-- Confirm

            mFinalData = mStoreData;
            mFinalCnvcls = cnvcls;
            mFinalMpxv = mpxv;
            mFinalMpxe = mpxe;
            mFinalDfent = dfent;
            mFinalDftag = dftag;
            mFinalDfmt = dfmt;
            mIsUpperError = isUpperError;
            mIsLowerError = isLowerError;
            mStoreData = 0;
            mINT_ADmxINT_ADEmDataActive = true;
            mINT_ADmxINT_ADEmDataEvent.notify((unsigned int)finish_delay, SC_NS);

            // --> Confirm
            if (cnvcls == emDisconnectionMode) {
                if (mForcedOpenError) {
                    mOpenCircuitError = true;
                } else {
                    mOpenCircuitError = false;
                }
            }
            // <-- Confirm
            break;
        default:
            break;
    }
}//}}}

double Cadc_u2a::ADCTimeCalculation (const bool is_start_SG, const unsigned int vc_number, const unsigned int vc_add)
{//{{{
#if 1 // RH850_U2A
    unsigned int mpxow = (unsigned int)(*MPXOWR)["MPXOW"];
#endif
    unsigned int cnvcls = 0;
    unsigned int wtts = 0;
    double sampling_time = 0;
    double wait_time = 0;

    if(vc_number == emVcPWM) {
        cnvcls = emPWSDMode;
        wtts = (unsigned int)((*PWDVCR)["WTTS"]);
    }
    if(vc_number < mVirChannelNum){
        cnvcls = (unsigned int)((*VCR[vc_number])["CNVCLS"]);
        wtts = (unsigned int)((*VCR[vc_number])["WTTS"]);
    }

    if (cnvcls == emExSamplingPeriodMode){
        if((unsigned int)((*SMPCR)["BUFAMPD"]) == 0x1) {
            re_printf("warning","Using extended sampling normal A/D conversion is prohibited when buffer amp disable. \n");
        }
        if((*SMPCR)["EXSMPT"] != 0){
            sampling_time = (double)((*SMPCR)["EXSMPT"] * mADCLKPeriod);
        }else {
            sampling_time = mExSmpt * mADCLKPeriod;
        }
    }
    else if ((*SMPCR)["SMPTS"] == 1 && (*SMPCR)["SMPT"] != 0){
        //if((unsigned int)((*SMPCR)["BUFAMPD"]) == 0x0) {
        //    re_printf("warning","Using sampling period other than default is prohibited when buffer amp enable. \n");
        //}
        sampling_time = (double)((*SMPCR)["SMPT"] * mADCLKPeriod);
    }
    else {
        //if((unsigned int)((*SMPCR)["BUFAMPD"]) == 0x1) {
        //    re_printf("warning","Using default sampling period is prohibited when buffer amp disable. \n");
        //}
        sampling_time = mSmpt * mADCLKPeriod;
    }
    if ((cnvcls == emMPXMode)
     || (cnvcls == emMPXAdditionMode)
     || (!is_start_SG && (cnvcls != emTHMode && cnvcls != emDiagnosisMode && cnvcls != emDataPathMode))
     || cnvcls == emPWSDMode) {
        if (  (mLastVC != vc_number && vc_add == 0 && vc_number < mVirChannelNum)
           || (cnvcls == emPWSDMode && vc_number == emVcPWM)) {
            if (wtts != 0) {
                wait_time = (double)((*WAITTR[wtts - 1])["WAITTIME"] * mADCLKPeriod);
            }
        }
    }
#if 1
    // MPX additional wait time - RH850_U2A
    if ((cnvcls == emMPXMode) || (cnvcls == emMPXAdditionMode)) {
        if ((mLastVC != vc_number && vc_add == 0 && vc_number < mVirChannelNum)) {
            wait_time += (double)(mpxow * 40 * mADCLKPeriod);
        }
    }
#endif
    //3.Total delay time
    double delay_total = sampling_time + wait_time;
    return delay_total;
}//}}}

bool Cadc_u2a::TimingCheck (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    double sample_delay = 0;
    double ex_sample_delay = 0;

    if((*SMPCR)["EXSMPT"] != 0){
        ex_sample_delay = (double)((*SMPCR)["EXSMPT"] * mADCLKPeriod);
    }else {
        ex_sample_delay = mExSmpt * mADCLKPeriod;
    }
    if ((*SMPCR)["SMPTS"] == 1 && (*SMPCR)["SMPT"] != 0){
        sample_delay = (double)((*SMPCR)["SMPT"] * mADCLKPeriod);
    }else {
        sample_delay = mSmpt * mADCLKPeriod;
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

    // --> Confirm
    if ((finish_delay + mPCLKPeriod) >= (sample_delay + convert_delay) || (finish_delay + mPCLKPeriod) >= (ex_sample_delay + convert_delay)) {
        re_printf("error","The condition of tED, ExSmpt and tSAR is not correct.\n");
        return false;
    } else {
        return true;
    }
    // <-- Confirm
}//}}}

bool Cadc_u2a::VoltageCheck (void)
{//{{{
    if ((mADCInstance >= mADCNum) || ((mAVccMin <= mAVcc) && (mAVcc <= mAVccMax) && (mAvrefhMin <= mAvrefh) && (mAvrefh <= mAVcc) && 
        (mEVccMin <= mEVcc) && (mEVcc <= mEVccMax))) {
        return true;
    } else {
        re_printf("warning","Setting reference voltages for ADC%d is not correct: mAvrefh:%lld.%06d, mAVcc:%lld.%06d, mEVcc:%lld.%06d.\n", mADCInstance, (unsigned long long)mAvrefh, FRACTION_TO_INT(mAvrefh), (unsigned long long)mAVcc, FRACTION_TO_INT(mAVcc), (unsigned long long)mEVcc, FRACTION_TO_INT(mEVcc));
        return false;
    }
}//}}}

bool Cadc_u2a::SuspendCheck (unsigned int group_number)
{//{{{
    unsigned int susmtd = (unsigned int)((*ADCR1)["SUSMTD"]);
    // asynronous suspend
    if ((group_number == 0 && susmtd == 1) || (susmtd > 1)) {
        return true;
    } else {
        return false;
    }
}//}}}

//call back functions
void Cadc_u2a::WritingADSYNSTCR (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(TimingCheck() && VoltageCheck()){
            unsigned int group_number = 0;
            for(unsigned int i = 1; i < (emGroupNum + 1); i++){
                group_number = emGroupNum - i;
                if((*SGCR[group_number])["ADSTARTE"] == 1){
                    mWritingADSYNSTCREvent[group_number].notify((unsigned int)(emtDCoeffPCLK * mPCLKPeriod), SC_NS);
                }
            }
        }
    }else {
        re_printf("warning","Scan group can not start while clock frequency is 0.\n");
    }
}//}}}

void Cadc_u2a::WritingADTSYNSTCR (void)
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
    }else {
        re_printf("warning","AD timer can not start while clock frequency is 0.\n");
    }
}//}}}

void Cadc_u2a::WritingVMONVDCR (const std::string register_name, const std::string bit_name, bool bit_val)
{//{{{
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if (register_name == "VMONVDCR1") {
            mVMONVDCRVal &= 0x2;
            mVMONVDCRVal |= bit_val;
        } else {
            mVMONVDCRVal &= 0x1;
            mVMONVDCRVal |= (bit_val << 1);
        }
    }
}//}}}

void Cadc_u2a::cb_SGSTCR_SGST (RegCBstr str)
{//{{{
    unsigned int group_number = (unsigned int)(str.channel);
    if((*SGSTCR[group_number])["SGST"] == 1){
        (*SGSTCR[group_number]) = 0;
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(TimingCheck() && VoltageCheck()){
                mWritingSGSTCREvent[group_number].notify((unsigned int)(emtDCoeffPCLK * mPCLKPeriod), SC_NS);
            }
        }else {
            re_printf("warning","Scan group can not start while clock frequency is 0.\n");
        }
    }
}//}}}

void Cadc_u2a::cb_SGSTPCR_SGSTP (RegCBstr str)
{//{{{
    if(CheckIsSVMode()){
        re_printf("warning","Setting SGSTPCR.SGSTP bit is prohibited during operation of SVSTOP.\n");
        mIsRetainedTrigger[str.channel] = false;
        if(str.channel == emGroup4) {
            mIsRetainedPWD = false;
        }
        if(mIsRetainedADsyn[str.channel]){
            mIsRetainedADsyn[str.channel] = false;
        }
        if(mIsRetainedSWTHA[str.channel]){
            mIsRetainedSWTHA[str.channel] = false;
        }
        #if 0 //Update for HWM0.61
        if(mIsRetainedSWTHB[str.channel]){
            mIsRetainedSWTHB[str.channel] = false;
        }
        #endif 
        if(mIsRetainedHWTHA[str.channel]){
            mIsRetainedHWTHA[str.channel] = false;
        }
        #if 0 //Update for HWM0.61
        if(mIsRetainedHWTHB[str.channel]){
            mIsRetainedHWTHB[str.channel] = false;
        }
        #endif
        if(mIsRetainedSW[str.channel]){
            mIsRetainedSW[str.channel] = false;
        }
    }
    (*SGSTPCR[str.channel]) = 0;
    if (mScanningState && (mCurrentGroup == str.channel)) {
        if(CheckIsSVMode()){
            mSGActive[mCurrentGroup] = false;
            (*SGSR[mCurrentGroup])["SGACT"] = 0;
            (*SGSTR)["SGACT"] =(unsigned int)( (*SGSTR)["SGACT"]) & (~(0x1 << mCurrentGroup));
            mIsScanningProcess[mCurrentGroup] = false;
            mGroupSuspendState[mCurrentGroup] = false;
            mDisableFlag = true;
            mDisableSGxFlag[mCurrentGroup] = true;
            mDisableEvent.notify();
        }else {
            mDisableOneSGFlag = true;
        }
        re_printf("info","The scanning process of scanning group %d is forced to stop.\n",mCurrentGroup);
    }else {
        mSGActive[str.channel] = false;
        (*SGSR[str.channel])["SGACT"] = 0;
        (*SGSTR)["SGACT"] = (unsigned int)((*SGSTR)["SGACT"]) & (~(0x1 << str.channel));
        mIsScanningProcess[str.channel] = false;
        mGroupSuspendState[str.channel] = false;
    }
}//}}}

void Cadc_u2a::cb_ADHALTR_HALT (RegCBstr str)
{//{{{
   if((*ADHALTR)["HALT"] == 1){
        (*ADHALTR) = 0;
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if(CheckIsSVMode()){
                re_printf("warning","Setting ADHALTR.HALT bit is prohibited during operation of SVSTOP.\n");
                for(unsigned int i=0;i<emGroupNum;i++){
                    mIsRetainedTrigger[i] = false;
                    mIsRetainedADsyn[i] = false;
                    mIsRetainedSWTHA[i] = false;
                    mIsRetainedSWTHB[i] = false;
                    mIsRetainedHWTHA[i] = false;
                    mIsRetainedHWTHB[i] = false;
                    mIsRetainedSW[i] = false;
                }
                mIsRetainedPWD = false;
            }
            if (mScanningState) {
                mDisableFlag = true;
                for (unsigned int i = 0; i < emGroupNum; i++) {
                    mDisableSGxFlag[i] = true;
                }
                mDisableEvent.notify();
                re_printf("info","The scanning process is forced to stop.\n");
            }
            // --> Confirm
            /// SARAD3
            for (unsigned int i = 3; i < emGroupNum; i++) {
                if((*SGSR[i])["ADTACT"] == 1) {
                    (*SGSR[i])["ADTACT"] = 0;
                    (*SGSTR)["ADTACT"] =(unsigned int)( (*SGSTR)["ADTACT"]) & (~(0x1 << (i-3)));
                    mTIMER[i - 3]->StopCounting();
                    mADCStateEvent.notify();
                    re_printf("info","The counting of AD timer %d is forced to stop.\n", i);
                }
            }
            // <-- Confirm
        }
   }
}//}}}

void Cadc_u2a::cb_MPXCURCR_MSKCFMT (RegCBstr str)
{//{{{
    bool write_condition = true;
    write_condition = AllSGStopCheck();
    if (((*THACR)["HLDTE"] != 0) || ((*THBCR)["HLDTE"] != 0)) {
        write_condition = false;
    }

    if(write_condition){
        /// SARAD4 modify - fix 0-frequency issue
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            unsigned int mpxcurcr = (unsigned int)(*MPXCURCR);
            unsigned int mpxcurr =  (unsigned int)(*MPXCURR1);
            unsigned int mskc_num = 4;
            for(unsigned int i = 0; i < mskc_num; i++){
                if((mpxcurcr & (1<<i)) == 0){
                    mpxcurr = mpxcurr & ((0xFFF0FFFF << (4*i)) | 0xFFF );
                }else{
                    // <-- Confirm
                    mpxcurr = mpxcurr & (~(0x000F0000 << (4*i)));
                    if (i == 0) {
                        mpxcurr = (unsigned int)(mpxcurr | (0x00070000 << (4*i)));
                    } else {
                        if (i == 1) {
                            re_printf("warning","Cannot write 1 into MSKCFMT[1] bit");
                            (*MPXCURCR)["MSKCFMT"] = (mpxcurcr & 0xD) | (str.pre_data & 0x2);
                        }
                        mpxcurr = (unsigned int)(mpxcurr | (0x000F0000 << (4*i)));
                    }
                }
            }
            (*MPXCURR1) = mpxcurr;
        }
    }else{
        (*MPXCURCR) = (unsigned int)(str.pre_data);
        re_printf("warning","Writing into MPXCURCR register is not affected.\n");
    }
}//}}}

void Cadc_u2a::cb_ADTSTCR_ADTST (RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && index == emGroup4){
        re_printf("warning","Writing into ADTSTCR4 register is not affected.\n");
        (*ADTSTCR[index]) = str.pre_data;
    } else {
        if((*ADTSTCR[index])["ADTST"] == 1){
            (*ADTSTCR[index]) = 0;
            /// SARAD4 modify - fix 0-frequency issue
            if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
                if(TimingCheck() && VoltageCheck()){
                    ADtimerCheck(index);
                }
            }
        }
    }
}//}}}

void Cadc_u2a::cb_ADTENDCR_ADTEND (RegCBstr str)
{//{{{
    unsigned int index = (unsigned int)(str.channel);
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && index == emGroup4){
        re_printf("warning","Writing into ADTENDCR4 register is not affected.\n");
        (*ADTENDCR[index]) = str.pre_data;
    } else {
        if((*ADTENDCR[index])["ADTEND"] == 1){
            (*ADTENDCR[index]) = 0;
            /// SARAD4 modify - fix 0-frequency issue
            if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
                if((*SGSR[index])["ADTACT"] == 1){
                    (*SGSR[index])["ADTACT"] = 0;
                    (*SGSTR)["ADTACT"] =(unsigned int)( (*SGSTR)["ADTACT"]) & (~(0x1 << (index-3)));
                    mTIMER[index - 3]->StopCounting();
                    mADCStateEvent.notify();
                    re_printf("info","The counting of AD timer %d is stop.\n", index);
                }
            }
        }
    }
}//}}}

void Cadc_u2a::CallBack_DRi (const unsigned int addr)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        unsigned int dr_index = (unsigned int)((addr - 0x1A0) >> 2);
        unsigned int vc_1st_number = dr_index * 2;
        if(mIsADPEmOperating){
            re_printf("warning","Reading DR%d register is ignored because ADPEm interrupt is in process.\n", dr_index);
        }else{
            if(!CheckIsSVMode()) {
                if((*SFTCR)["RDCLRE"] == 1){
                    (*DR[dr_index]) = 0;
                    (*DIR[vc_1st_number]) = 0;
                    (*DIR[vc_1st_number + 1]) = 0;
                    re_printf("info","The DR%d, DIR%d and DIR%d registers are cleared to 0.\n", dr_index, vc_1st_number, (vc_1st_number + 1));
                }else {
                    (*DIR[vc_1st_number])["WFLAG"] = 0;
                    (*DIR[vc_1st_number + 1])["WFLAG"] = 0;
                    re_printf("info","The DIR%d.WFLAG and DIR%d.WFLAG are cleared to 0.\n", vc_1st_number, (vc_1st_number + 1));
                }
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

void Cadc_u2a::CallBack_DIRn (const unsigned int addr)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        unsigned int real_vc_number = (unsigned int)((addr - 0x280) >> 2);
        if(mIsADPEmOperating){
            re_printf("warning","Reading DIR%d register is ignored because ADPEm interrupt is in process.\n", real_vc_number);
        }else{
            if(!CheckIsSVMode()) {
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
                    (*DIR[real_vc_number])["WFLAG"] = 0;
                    re_printf("info","The DIR%d.WFLAG is cleared to 0.\n", real_vc_number);
                }
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

void Cadc_u2a::CallBack_PWDDR (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(mIsADPEmOperating){
            re_printf("warning","Reading PWDDR register is ignored because ADPEm interrupt is in process.\n");
        }else{
            if(!CheckIsSVMode()) {
                if((*SFTCR)["RDCLRE"] == 1){
                    (*PWDDR) = 0;
                    (*PWDDIR) = 0;
                    re_printf("info","The PWDDR, and PWDDIR registers are cleared to 0.\n");
                }else {
                    (*PWDDIR)["WFLAG"] = 0;
                    re_printf("info","The PWDDIR.WFLAG is cleared to 0.\n");
                }
            }

            //call interrupt
            if(mForcedParityError){
                re_printf("info","The parity error occurs when reading PWDDR.\n");
                if((*PER)["PE"] == 0){
                    (*PER)["PE"] = 1;
                    (*PER)["PECAP"] = emVcPWM;
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

void Cadc_u2a::CallBack_PWDDIR (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if(mIsADPEmOperating){
            re_printf("warning","Reading PWDDIR register is ignored because ADPEm interrupt is in process.\n");
        }else{
            if(!CheckIsSVMode()) {
                if((*SFTCR)["RDCLRE"] == 1){
                    (*PWDDR) = 0;
                    (*PWDDIR) = 0;
                    re_printf("info","The PWDDR and PWDDIR registers are cleared to 0.\n");
                }else {
                    (*PWDDIR)["WFLAG"] = 0;
                    re_printf("info","The PWDDIR.WFLAG is cleared to 0.\n");
                }
            }

            //call interrupt
            if(mForcedParityError ){
                re_printf("info","The parity error occurs when reading PWDDIR.\n");
                if((*PER)["PE"] == 0){
                    (*PER)["PE"] = 1;
                    (*PER)["PECAP"] = emVcPWM;
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

void Cadc_u2a::cb_ADCR1_SUSMTD (RegCBstr str)
{//{{{
    if (str.data != str.pre_data) {
        if (!AllSGStopCheck()) {
            re_printf("warning", "Writing into ADCR1 register is not affected.\n");
            (*ADCR1) = str.pre_data;
        }
        if((unsigned int)((*ADCR1)["SUSMTD"]) == 0x3){
            re_printf("warning", "ADCR1.SUSMTD is set to 0 due to written value 3 is prohibited. \n");
            (*ADCR1)["SUSMTD"] = 0x0;
        }
    }
}//}}}

void Cadc_u2a::cb_ADCR2_DFMT (RegCBstr str)
{//{{{
#if 0
    if (str.data != str.pre_data) {
        if (!AllSGStopCheck()) {
            re_printf("warning", "Writing into ADCR2 register is not affected.\n");
            (*ADCR2) = str.pre_data;
        }
    }
#else  // Update for HWM 0.61, setting to DFMT = 3,6,7 is prohibited 
    unsigned int dfmt_tmp = (*ADCR2)["DFMT"];
    if (str.data != str.pre_data) {
        if (!AllSGStopCheck()) {
            re_printf("warning", "Writing into ADCR2 register is not affected.\n");
            (*ADCR2) = str.pre_data;
        } else {
            if ((dfmt_tmp == 3) || (dfmt_tmp == 6) || (dfmt_tmp == 7)) {
                re_printf("warning", "Setting ADCR2.DFMT = %d is prohibited\n", dfmt_tmp);
                (*ADCR2)["DFMT"] = (str.pre_data >> 4) & 0x7;
            }
        }
    }
#endif
}//}}}

void Cadc_u2a::cb_ECR_IDEC (RegCBstr str)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    unsigned int tocesgc = (unsigned int)((*ECR)["TOCESGC"]);
    unsigned int tocesg = (unsigned int)((*TOCER)["TOCESG"]);
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
            if(((tocesgc >> group_number) & 0x1) == 1 && ((tocesg >> group_number) & 0x1) == 1){
                (*TOCER)["TOCESG"] =(unsigned int)( (*TOCER)["TOCESG"]) & (~(0x1 << group_number));
                re_printf("info","The TOCER.TOCESG[%d] bit is cleared to 0.\n",group_number);
            }
        }
        if(((*ECR)["OWEC"] == 1) && ((*OWER)["OWE"] == 1)){
            (*OWER) = 0;
            re_printf("info","The OWER register is cleared to 0.\n");
        }
        if(((*ECR)["PEC"] == 1) && ((*PER)["PE"] == 1)){
            (*PER) = 0;
            re_printf("info","The PER register is cleared to 0.\n");
        }
        if(((*ECR)["IDEC"] == 1) && ((*IDER)["IDE"] == 1)){
            (*IDER) = 0;
            re_printf("info","The IDER register is cleared to 0.\n");
        }
    }
    (*ECR) = 0;
}//}}}

 void Cadc_u2a::cb_SGCR_ADIE (RegCBstr str)
{//{{{
    unsigned int group_number = (unsigned int)(str.channel);
    //unsigned int pre_value    = ((unsigned int)(str.pre_data) & 0x30);
    //unsigned int cur_value    = ((unsigned int)(str.data) & 0x30);
    unsigned int adtstarte    = ((unsigned int)(str.data) & 0x80);
    unsigned int trgmd        = ((unsigned int)(str.data) & 0x3);
    unsigned int pre_trgmd    = ((unsigned int)(str.pre_data) & 0x3);
    unsigned int cur_trgmd    = ((unsigned int)(str.data) & 0x3);
    unsigned int pre_adie     = ((unsigned int)(str.pre_data >> 4) & 0x1);
    unsigned int cur_adie     = ((unsigned int)(str.data >> 4) & 0x1);
    unsigned int pre_other_bit= ((unsigned int)(str.pre_data >> 5) & 0x7);
    unsigned int cur_other_bit= ((unsigned int)(str.data >> 5) & 0x7);
    //unsigned int pre_adstarte = ((unsigned int)(str.pre_data) & 0x40);

    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && group_number == emGroup4){
        re_printf("warning","Writing into SGCR4 register is not affected.\n");
        (*SGCR[str.channel]) = str.pre_data;
    } else {
        if((*SGSR[str.channel])["SGACT"] != 0) {
            if(((pre_trgmd & 0x1) == 0x0 && (cur_trgmd & 0x1) == 0x1)
             ||((pre_trgmd & 0x2) == 0x0 && (cur_trgmd & 0x2) == 0x2)){
                re_printf("warning","Writing into SGCR%d.TRGMD bit is not affected.\n", str.channel);
                (*SGCR[str.channel])["TRGMD"] = pre_trgmd;
            }
            if(pre_adie  == 0 && cur_adie  == 1){
                re_printf("warning","Writing into SGCR%d.ADIE bit is not affected.\n", str.channel);
                (*SGCR[str.channel])["ADIE"] = pre_adie;
            }
            if(pre_other_bit != cur_other_bit) {
                re_printf("warning","Writing into SGCR%d.ADTSTARTE or SGCR%d.ADSTARTE or SGCR%d.SCANMD bit is not affected.\n", str.channel, str.channel, str.channel);
                (*SGCR[str.channel])["ADTSTARTE"] = (pre_other_bit >> 2) & 0x1;
                (*SGCR[str.channel])["ADSTARTE"]  = (pre_other_bit >> 1) & 0x1;
                (*SGCR[str.channel])["SCANMD"]    = pre_other_bit & 0x1;
            }
        }
        if (group_number < emGroup3) {
            if ((adtstarte != 0) || (trgmd > 1)) {
                re_printf("warning","Cannot write 1 into to reserved bit.\n");
            }
            (*SGCR[group_number]) &= 0x7D;
        }
    }
}//}}}

void Cadc_u2a::cb_SGMCYCR_MCYC (RegCBstr str)
{//{{{
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && str.channel == emGroup4){
        re_printf("warning","Writing into SGMCYCR4 register is not affected.\n");
        (*SGMCYCR[str.channel]) = str.pre_data;
    } else {
        if ((*SGSR[str.channel])["SGACT"] != 0) {
            re_printf("warning","Writing into SGMCYCR%d register is not affected.\n", str.channel);
            (*SGMCYCR[str.channel]) = str.pre_data;
        }
    }
}//}}}

void Cadc_u2a::cb_SGVCPR_VCSP (RegCBstr str)
{//{{{
    unsigned int start_vc = (unsigned int)((*SGVCPR[str.channel])["VCSP"]);
    unsigned int end_vc   = (unsigned int)((*SGVCPR[str.channel])["VCEP"]);
    bool write_condition = true;
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && str.channel == emGroup4){
        write_condition = false;
    } else {
        if(start_vc > end_vc){
            re_printf("warning","The start VC must be equal or less than end VC.\n");
            write_condition = false;
        } else {
            if(start_vc >= mVirChannelNum){
                re_printf("warning","The start VC must be equal or less than %d.\n", mVirChannelNum - 1);
                write_condition = false;
            }
            //if (start_vc != (unsigned int)((*SGVCSP[str.channel])["VCSP"])) {
            //    re_printf("warning","The start VC must be equal to SGVCSP%d.VCSP.", str.channel);
            //    write_condition = false;
            //}
            if(end_vc >= mVirChannelNum){
                re_printf("warning","The end VC must be equal or less than %d.\n", mVirChannelNum - 1);
                write_condition = false;
            }
            //if (end_vc != (unsigned int)((*SGVCEP[str.channel])["VCEP"])) {
            //    re_printf("warning","The end VC must be equal to SGVCEP%d.VCEP.", str.channel);
            //    write_condition = false;
            //}
        }
    }
    if (write_condition) {
        mIsUpdateSGVCPR[str.channel] = true;
    } else {
        re_printf("warning","Writing into SGVCPR%d register is not affected.\n", str.channel);
        (*SGVCPR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_SGVCSP_VCSP (RegCBstr str)
{//{{{
    unsigned int start_vc = (unsigned int)((*SGVCSP[str.channel])["VCSP"]);
    bool write_condition = true;
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && str.channel == emGroup4){
        write_condition = false;
    } else {
        if (start_vc >= mVirChannelNum){
            re_printf("warning","The start VC must be equal or less than %d.\n", mVirChannelNum - 1);
            write_condition = false;
        }
    }
    if (!write_condition) {
        re_printf("warning","Writing into SGVCSP%d register is not affected.\n", str.channel);
        (*SGVCSP[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_SGVCEP_VCEP (RegCBstr str)
{//{{{
    unsigned int end_vc = (unsigned int)((*SGVCEP[str.channel])["VCEP"]);
    bool write_condition = true;
    if((unsigned int)((*PWDCR)["PWE"]) == 0x1 && str.channel == emGroup4){
        write_condition = false;
    } else {
        if (end_vc >= mVirChannelNum){
            re_printf("warning","The start VC must be equal or less than %d.\n", mVirChannelNum - 1);
            write_condition = false;
        }
    }
    if (!write_condition) {
        re_printf("warning","Writing into SGVCEP%d register is not affected.\n", str.channel);
        (*SGVCEP[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_ADTIPR_ADTIP (RegCBstr str)
{//{{{
    if((*SGSR[str.channel])["ADTACT"] != 0){
        re_printf("warning","Writing into ADTIPR%d register is not affected.\n", str.channel);
        (*ADTIPR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_ADTPRR_ADTPR (RegCBstr str)
{//{{{
    if((*SGSR[str.channel])["ADTACT"] != 0){
        re_printf("warning", "Writing into ADTPRR%d register is not affected.\n", str.channel);
        (*ADTPRR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_SFTCR_IDEIE(RegCBstr str)
{//{{{
    //if ((mTargetProduct == "RH850_E2x") || (mTargetProduct == "RH850_E2x-468Pins")) {
    //    if (((str.data >> 3) & 0x1) != 0) {
    //        re_printf("warning","SFTCR.ULEIE bit is not existing in %s.\n", mTargetProduct.c_str());
    //        (*SFTCR)["ULEIE"] = 0;
    //    }
    //    if (((str.data >> 5) & 0x1) != 0) {
    //        re_printf("warning","SFTCR.SYNCEIE bit is not existing in %s.\n", mTargetProduct.c_str());
    //        (*SFTCR)["SYNCEIE"] = 0;
    //    }
    //}
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into SFTCR register is not affected.\n");
        (*SFTCR) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_TOCCR_TOCE(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into TOCCR register is not affected.\n");
        (*TOCCR) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_WAITTR_WAITTIME(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into WAITTR%d register is not affected.\n",str.channel);
        (*WAITTR[str.channel]) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_EMUCR_SVSDIS(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into EMUCR register is not affected.\n");
        (*EMUCR) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_TDCR_TDLV(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Should not update value of TDCR register while scan group is operating.\n");
        //(*TDCR) = str.pre_data;
    }
}//}}}

#if 1
void Cadc_u2a::cb_ODCR_WADDE(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into ODCR register is not affected.\n");
        (*ODCR) = str.pre_data;
    }
}//}}}
#endif

#if 0
void Cadc_u2a::cb_DFASENTSGER_DFASENTSG0E(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into DFASENTSGER register is not affected.\n");
        (*DFASENTSGER) = str.pre_data;
    }
}//}}}
#else
// RH850_U2A added
void Cadc_u2a::cb_GTMENTSGER_GTMENTSG0E(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into GTMENTSGER register is not affected.\n");
        (*GTMENTSGER) = str.pre_data;
    }
}//}}}
#endif

void Cadc_u2a::cb_MPXINTER_ADMPXIE(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into MPXINTER register is not affected.\n");
        (*MPXINTER) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_MPXOWR_MPXOW(RegCBstr str)
{//{{{
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into MPXOWR register is not affected.\n");
        (*MPXOWR) = str.pre_data;
    }
}//}}}

void Cadc_u2a::cb_VCR_GCTRL(RegCBstr str)
{//{{{
    //bool wr_cond = true;
    /// Changed in SARAD3
    //unsigned int gctrl = str.data & 0x3F;
    //unsigned int pre_gctrl = str.pre_data & 0x3F;
    //unsigned int pre_cnvcls = (str.pre_data >> 13) & 0x7;
    //unsigned int cnvcls = (unsigned int)(*VCR[str.channel])["CNVCLS"];
    if ((unsigned int)((*VCR[str.channel])["VCULLMTBS"]) > 8) {
        re_printf("warning", "VCR%d.VCULLMTBS is set to 0 due to written above value 8 is prohibited.\n",str.channel);
        (*VCR[str.channel])["VCULLMTBS"] = 0;
    }
    if ((unsigned int)((*VCR[str.channel])["WTTS"]) > 8) {
        re_printf("warning", "VCR%d.WTTS is set to 0 due to written above value 8 is prohibited.\n",str.channel);
        (*VCR[str.channel])["WTTS"] = 0;
    }
    if ((unsigned int)((*VCR[str.channel])["CNVCLS"]) == 15) {
        re_printf("warning", "VCR%d.CNVCLS is set to 0 due to written 15 is reserved.\n",str.channel);
        (*VCR[str.channel])["CNVCLS"] = 0;
    }
    if ((unsigned int)((*VCR[str.channel])["CNVCLS"]) == emDisconnectionMode
      &&(unsigned int)((*SMPCR)["BUFAMPD"]) == 0x1) {
        re_printf("warning", "VCR%d.CNVCLS is prohibited to be 0x8 When buffer amp disable.\n",str.channel);
        (*VCR[str.channel])["CNVCLS"] = ((str.pre_data >> 11) & 0xF);
    }
    if ((unsigned int)((*VCR[str.channel])["CNVCLS"]) == emExSamplingPeriodMode){
        if((unsigned int)((*SMPCR)["BUFAMPD"]) == 0x1) {
            re_printf("warning", "VCR%d.CNVCLS is prohibited to be 0x2 When buffer amp disable.\n",str.channel);
            (*VCR[str.channel])["CNVCLS"] = ((str.pre_data >> 11) & 0xF);
        }
    }
    if ((unsigned int)((*VCR[str.channel])["CNVCLS"]) == emTHMode){
        if (mADCInstance == 2) {
            re_printf("warning", "T&H is not supported in ADC2. VCR%d.CNVCLS is prohibited to be 0x1.\n",str.channel);
            (*VCR[str.channel])["CNVCLS"] = ((str.pre_data >> 11) & 0xF);
        }
    }
    for (unsigned int group_number = 0; group_number <emGroupNum; group_number++) {
        if (((unsigned int)((*SGVCPR[group_number])["VCSP"]) <= str.channel) && ( str.channel <= (unsigned int)((*SGVCPR[group_number])["VCEP"]))
         && ((*SGSR[group_number])["SGACT"] != 0)) {
            re_printf("warning","Writing into VCR%d register is not affected.\n", str.channel);
            (*VCR[str.channel]) = str.pre_data;
            break;
        }
    }
}//}}}

void Cadc_u2a::cb_SMPCR_SMPT(RegCBstr str)
{//{{{
    //bool smpts = (bool)((*SMPCR)["SMPTS"]);
    unsigned int smpt = (unsigned int)((*SMPCR)["SMPT"]);
    unsigned int exsmpt = (unsigned int)((*SMPCR)["EXSMPT"]);
    if(!AllSGStopCheck()){
        re_printf("warning", "Writing into SMPCR register is not affected.\n");
        (*SMPCR) = str.pre_data;
    }else {
        if ((smpt != 0xCC) && (smpt != 0x63) && (smpt != 0xFC) && (smpt != 0x00)) {
            re_printf("warning","Setting SMPCR.SMPT = %x is prohibited.\n", smpt);
            (*SMPCR)["SMPT"] = str.pre_data & 0xFF;
            smpt = (unsigned int)((*SMPCR)["SMPT"]);
        }
        if ((exsmpt != 0x8A) && (exsmpt != 0x63) && (exsmpt != 0xFC) && (exsmpt != 0x00)) {
            re_printf("warning","Setting SMPCR.EXSMPT = %x is prohibited.\n", exsmpt);
            (*SMPCR)["EXSMPT"] = (str.pre_data >> 16) & 0xFF;
        }
        if((unsigned int)((*SMPCR)["BUFAMPD"]) == 0x0){
            if((unsigned int)((*SMPCR)["SMPTS"]) == 0x1
            && smpt != 0){
                re_printf("warning","Setting SMPCR register is not affected when buffer AMP setting is violated.\n");
                (*SMPCR) = str.pre_data;
            }
        }else {
            if((   (unsigned int)((*SMPCR)["SMPTS"]) == 0x1
                && smpt == 0)
             ||((unsigned int)((*SMPCR)["SMPTS"]) == 0x0)){
                re_printf("warning","Setting SMPCR register is not affected when buffer AMP setting is violated.\n");
                (*SMPCR) = str.pre_data;
            }
        }
    }
}//}}}

void Cadc_u2a::cb_VCULLMTBR_VCULMTB(RegCBstr str)
{//{{{
    //unsigned int index = (unsigned int)(str.channel);
    //unsigned int reserve_bit_31 = (unsigned int)(str.data) & 0x80000000;
    //unsigned int reserve_bit_16 = (unsigned int)(str.data) & 0x00010000;
    //unsigned int reserve_bit_15 = (unsigned int)(str.data) & 0x00008000;
    //unsigned int reserve_bit_1  = (unsigned int)(str.data) & 0x00000001;
    //if((reserve_bit_1 != 0)||(reserve_bit_15 != 0)||(reserve_bit_16 != 0)||(reserve_bit_31 != 0)){
    //    re_printf("warning","Cannot write 1 into to reserved bit.\n");
    //}
    //(*VCULLMTBR[index]) &= 0x7FFE7FFE;
}//}}}

void Cadc_u2a::cb_VCLMSCR1_VC00LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<32;i++) {
        if ((((unsigned int)(*VCLMSCR1) >> i)&0x1) == 0x1) {
            (*VCLMSR1) = (unsigned int)(*VCLMSR1) & (~(1 << i));
            re_printf("info", "The VCLMSR1.VC%2dLMS bit is clear to 0.\n", i);
        }
    }
    *VCLMSCR1 = 0;
}//}}}

void Cadc_u2a::cb_VCLMSCR2_VC32LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<32;i++) {
        if ((((unsigned int)(*VCLMSCR2) >> i)&0x1) == 0x1) {
            (*VCLMSR2) = (unsigned int)(*VCLMSR2) & (~(1 << i));
            re_printf("info", "The VCLMSR2.VC%2dLMS bit is clear to 0.\n", i);
        }
    }
    *VCLMSCR2 = 0;
}//}}}

void Cadc_u2a::cb_VCLMSCR3_VC64LMSC(RegCBstr str)
{//{{{
    for (unsigned int i=0;i<32;i++) {
        if ((((unsigned int)(*VCLMSCR3) >> i)&0x1) == 0x1) {
            (*VCLMSR3) = (unsigned int)(*VCLMSR3) & (~(1 << i));
            re_printf("info", "The VCLMSR3.VC%2dLMS bit is clear to 0.\n", i);
        }
    }
    *VCLMSCR3 = 0;
}//}}}

void Cadc_u2a::cb_PWVCLMSCR_PWVCLMSC(RegCBstr str)
{//{{{
    if ((*PWVCLMSCR)["PWVCLMSC"] == 0x1) {
        (*PWVCLMSR)["PWVCLMS"] = 0x0;
        re_printf("info", "The PWVCLMSR.PWVCLMS bit is clear to 0.\n");
    }
    (*PWVCLMSCR) = 0;
}//}}}

void Cadc_u2a::cb_PWDCR_PWE(RegCBstr str)
{//{{{
    if(str.data != str.pre_data) {
        if (!AllSGStopCheck()) {
            re_printf("warning","Writing into PWDCR register is not affected.\n");
            (*PWDCR) = str.pre_data;
        }else {
            if((unsigned int)((*PWDCR)["PWE"]) == 0x1){
                if((unsigned int)((*THACR)["SGS"]) == 0x3 ||
                   (unsigned int)((*THBCR)["SGS"]) == 0x3){
                    re_printf("warning","Setting SG4 for T&H is prohibited when PWDCR.PWE is active.\n");
                    (*PWDCR) = str.pre_data;
                }
                if((unsigned int)((*SGMCYCR[emGroup4])["MCYC"]) != 0x0){
                    re_printf("warning","SGMCYCR4.MCYC can be set only to 0x0 when PWDCR.PWE is active.\n");
                    (*PWDCR) = str.pre_data;
                }
            }
        }
    }
}//}}}


void Cadc_u2a::cb_PWDSGCR_TRGMD(RegCBstr str)
{//{{{
    if(str.data != str.pre_data) {
        if (((*SGSR[emGroup4])["SGACT"] != 0 && (str.pre_data & 0x1) == 0 && (str.data & 0x1) == 1) ||
            ((unsigned int)((*PWDCR)["PWE"]) == 0x0)) {
            re_printf("warning","Writing into PWDSGCR register is not affected.\n");
            (*PWDSGCR) = str.pre_data;
        }
    }
}//}}}


void Cadc_u2a::cb_PWVCLMINTER_PWADULIE(RegCBstr str)
{//{{{
    if(str.data != str.pre_data) {
        if (!AllSGStopCheck()) {
            re_printf("warning","Writing into PWVCLMINTER register is not affected.\n");
            (*PWVCLMINTER) = str.pre_data;
        }
    }
}//}}}

void Cadc_u2a::cb_SGULCCR_SGULCC(RegCBstr str)
{//{{{
    for(unsigned int i=0;i<emGroupNum;i++) {
        if((((unsigned int)((*SGULCCR)["SGULCC"]) >> i) & 0x1) == 0x1){
            (*SGULCR[i]) = 0x0;
            re_printf("info", "The SGULCR%d register is clear to 0.\n", i);
        }
    }
    (*SGULCCR) = 0x0;
}//}}}

void Cadc_u2a::cb_VCLMASCR_ALLMSC(RegCBstr str)
{//{{{
    if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        if ((unsigned int)(*VCLMASCR)["ALLMSC"] == 0x1) {
            (*VCLMSR1) = 0;
            re_printf("info","The VCLMSR1 register is cleared to 0.\n");
            (*VCLMSR2) = 0;
            re_printf("info","The VCLMSR2 register is cleared to 0.\n");
            (*PWVCLMSR) = 0;
            re_printf("info","The PWVCLMSR register is cleared to 0.\n");
            mULE_LPSmActive = 0;
            mULE_LPSmEvent.notify();
            re_printf("info","The ULE_LPS%d output is cleared to 0.\n", mADCInstance);
        }
    }
    (*VCLMASCR) = 0;
}//}}}

//reset functions
void Cadc_u2a::EnableReset(const bool is_active,const unsigned int port_reset)
{//{{{
    if (mADCInstance >= mADCNum) {
        return;
    }
    if(is_active){
        if(port_reset == emADC_AD_RESET || port_reset == emADC_AD_RESET2){
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
            mResetFlag = true;
            GlobalInit();

            //cancel all events
            /// SARAD4 modify
            CancelAllEvents();

            mIsADReset = true;
            mADResetEvent.notify();
        }
        if(port_reset == emADC_P_RESET || port_reset == emADC_P_RESET2){
            mIsPReset = true;
            mPResetEvent.notify();
        }

    }else{
        if(port_reset == emADC_AD_RESET || port_reset == emADC_AD_RESET2){
            mIsADReset = false;
            mADCState = emADCIdleState;
            re_printf("info","The ADC%d state changes from RESET state to IDLE state.\n", mADCInstance);
        }
        if(port_reset == emADC_P_RESET || port_reset == emADC_P_RESET2){
            mIsPReset = false;
        }
    }

    if(port_reset == emADC_P_RESET || port_reset == emADC_P_RESET2){
        //reset register
        Cadc_u2a_regif::EnableReset(is_active);
    }

    if(port_reset == emADC_AD_RESET || port_reset == emADC_AD_RESET2){
        //reset timer
        for(unsigned int index = 0; index < 2; index++){
            mTIMER[index]->EnableReset(is_active);
        }
    }
}//}}}

void Cadc_u2a::TriggerADC2Method(const unsigned int index)
{//{{{
    int group_number;
    if (mADCInstance == 2) {
        switch(index) {
            case 0:
                for (group_number = emGroupNum -1 ; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x1) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 1:
                for (group_number = emGroupNum -1; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x2) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 2:
                for (group_number = emGroupNum -1; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x4) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 3:
                for (group_number = emGroupNum -1; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x8) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            case 4:
                for (group_number = emGroupNum -1; group_number >= 0; group_number--) {
                    if ((*SGTSEL[group_number]) == 0x20) {
                        HardwareTrigger(group_number,4);
                    }
                }
                break;
            default:
                break;
        }
    } 
}//}}}

void Cadc_u2a::GlobalInit(void)
{//{{{
    mEnterSVModeFlag = false;
    mExitSVModeFlag = false;
    //disable
    mDisableFlag = false;
    mDisableOneSGFlag = false;
    //T&H circuit
    for(unsigned int TH_channel = 0; TH_channel < mTHChannelNum; TH_channel++){
        mTHSamplingStartTime [TH_channel] = 0;
        mTHChannelState [TH_channel] = emTHIdleState;
        mTHUpdateChannelState [TH_channel] = false;
        mIntTHSample [TH_channel] = 0;
        mTHUpdateInput [TH_channel] = false;
        mPhyChannelTH[TH_channel] = 0;
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
    mPWMFlag = false;
    mGroupANumber = 0;
    mGroupBNumber = 0;

//output
    mINT_MPXmActive = false;
    mADPEmActive = false;
    mErrorId = 0;
    mIsUpperError = false;
    mIsLowerError = false;
    mPhyChannel = 0;
    mFinalPhyChannel = 0;
    mIsADPEmOperating = false;
    mINT_ADmxINT_ADEmDataActive = false;
#if 0
    mINT_ULmActive = false;
    mINT_ADEmActive = false;
    mINT_ADEmProcess = false;
#else
    mULE_LPSmActive = 0;
    // RH850_U2A added
    mINT_ERRmActive = false;
    mINT_ERRmProcess = false;
#endif
    mID = 0;
    mStoreData = 0;
    mFinalData = 0;
    mINT_MPXmActiveNum = 0;
#if 0
    mINT_ULmActiveNum = 0;
    mINT_ADEmActiveNum = 0;
#else
    // RH850_U2A added
    mINT_ERRmActiveNum = 0;
#endif
    mADPEmActiveNum = 0;
    mINT_ADm0ActiveNum = 0;
    mINT_ADm1ActiveNum = 0;
    mINT_ADm2ActiveNum = 0;
    mINT_ADm3ActiveNum = 0;
    mINT_ADm4ActiveNum = 0;
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
        mIsStartSG[group_number]           = false;
        mSGActive [group_number]           = false;
        mIsScanningProcess[group_number]   = false;
        mDisableSGxFlag[group_number]      = false;
        /// SARAD3
        mIsHWTrigger[group_number] = false;
        mIsRetainedTrigger[group_number] = false;
        mIsRetainedADsyn[group_number] = false;
        mIsRetainedSWTHA[group_number] = false;
        mIsRetainedSWTHB[group_number] = false;
        mIsRetainedHWTHA[group_number] = false;
        mIsRetainedHWTHB[group_number] = false;
        mIsRetainedSW[group_number] = false;
    }
    mIsRetainedPWD = false;
    mCurrentGroup = 0;
    mLastGroup = 0;
    mFinalGroup = 0;
    mLastVC = mVirChannelNum;
    mFinalVC = 0;
    mVMONVDCRVal = 0;
    mULMTB = false;
    mLLMTB = false;
    mCurVC = 0;
    mSuspendFlag = false;
    mScanningState = false;
    mOpenCircuitError = false;
    mFinalCnvcls = 0;
    mFinalMpxv = 0;
    mFinalMpxe = 0;
    mFinalDfent = 0;
    mFinalDftag = 0;
    mFinalDfmt = 0;

}//}}}

//information function
void Cadc_u2a::DumpInfo (const char *type, const char *message, ... )
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

void Cadc_u2a::DumpStatInfo (void)
{//{{{
//1.Dump information
    if (mADCInstance < mADCNum) {
        DumpInfo("StatInfo", "Info[%s](ADC%d).\n",sc_time_stamp().to_string().c_str(), mADCInstance);
        DumpInfo("StatInfo", " INT_MPX%d active number : %d\n", mADCInstance, mINT_MPXmActiveNum);
        DumpInfo("StatInfo", " ADPE%d active number : %d\n", mADCInstance, mADPEmActiveNum);
#if 0
        DumpInfo("StatInfo", " INT_UL%d active number : %d\n", mADCInstance, mINT_ULmActiveNum);
        DumpInfo("StatInfo", " INT_ADE%d active number : %d\n", mADCInstance, mINT_ADEmActiveNum);
#else
        // RH850_U2A added
        DumpInfo("StatInfo", " INT_ERR%d active number : %d\n", mADCInstance, mINT_ERRmActiveNum);
#endif
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
#if 0
    mINT_ULmActiveNum = 0;
    mINT_ADEmActiveNum    = 0;
#else
    // RH850_U2A added
    mINT_ERRmActiveNum    = 0;
#endif
    mINT_ADm0ActiveNum   = 0;
    mINT_ADm1ActiveNum   = 0;
    mINT_ADm2ActiveNum   = 0;
    mINT_ADm3ActiveNum   = 0;
    mINT_ADm4ActiveNum   = 0;
}//}}}

void Cadc_u2a::DumpADCActivity (const double start_time, const double end_time)
{//{{{
    if(mEnableConvertInfo){
        re_printf("info","Start simulation time: %lld.%06d.\n", (unsigned long long)start_time, FRACTION_TO_INT(start_time));
        re_printf("info","End simulation time: %lld.%06d.\n", (unsigned long long)end_time, FRACTION_TO_INT(end_time));
        if(mFinalVC == emVcPWM && mFinalGroup == emGroup4){
            re_printf("info","The virtual channel number: PWM-Diag.\n");
            re_printf("info","The value of virtual channel register PWDVCR: 0x%X.\n", (unsigned int)(*PWDVCR));
        }
        if(mFinalVC < mVirChannelNum){
            re_printf("info","The virtual channel number: %d.\n", mFinalVC);
            re_printf("info","The value of virtual channel register VCR: 0x%X.\n", (unsigned int)(*VCR[mFinalVC]));
        }
        re_printf("info","The scanning group: %d.\n", mFinalGroup);
    }
}//}}}

void Cadc_u2a::DumpInterruptMessage (const std::string intr_name, const std::string intr_number, const std::string issue)
{//{{{
    if(mDumpInterrupt){
        re_printf("info","INT [SAR AD: %s%d%s] %s.\n", intr_name.c_str(), mADCInstance, intr_number.c_str(), issue.c_str());
    }
}//}}}

/// SARAD3
void Cadc_u2a::HWTriggerScanningMethod (void)
{//{{{
    /// SARAD4 modify - fix 0-frequency issue
    if ((!mIsADReset) && (mADCInstance < mADCNum) && (mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
        for (unsigned int i = 0; i < emGroupNum; i++) {
            if (mIsHWTrigger[i]) {
                GroupCheck(i);
                mIsHWTrigger[i] = false;
            }
        }
    }
}//}}}

/// SARAD4 add - fix 0-frequency issue
void Cadc_u2a::SettingZeroClock (void)
{//{{{
    // Cancel all events
    CancelAllEvents();

    // Stop operation
    mZeroClockEvent.notify();

    // Disable all Timers
    mTIMER[0]->StopCounting();
    mTIMER[1]->StopCounting();
}//}}}

void Cadc_u2a::CancelAllEvents (void)
{//{{{
    mScanningEvent.cancel();
    mSuspendEvent.cancel();
    for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
        mADENDmxEvent[group_number].cancel();
        mWritingADSYNSTCREvent[group_number].cancel();
        mWritingSGSTCREvent[group_number].cancel();
        mSGSTTriggerEvent[group_number].cancel();
        mADSYNTriggerEvent[group_number].cancel();
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
#if 0
    mINT_ADEmEvent.cancel();
#else
    // RH850_U2A added
    mULE_LPSmEvent.cancel();
    mINT_ERRmEvent.cancel();
#endif
    mBoundaryExcessEvent.cancel();
    mVCENDNoticeEvent.cancel();
    mSVModeEvent.cancel();
}//}}}

void Cadc_u2a::SetULErrorStatus(unsigned int channel) // RH850_E2x added for set upper/lower bound error status
{//{{{
    if (channel < 32) {
        (*VCLMSR1) = (unsigned int)(*VCLMSR1) | (1 << channel);
    } else if (channel < 64) {
        (*VCLMSR2) = (unsigned int)(*VCLMSR2) | (1 << (channel-32));
    } else if (channel < 96) {
        (*VCLMSR3) = (unsigned int)(*VCLMSR3) | (1 << (channel-64));
    }
}//}}}

bool Cadc_u2a::ULIntrEnable(unsigned int channel) // RH850_E2x added for check upper/lower bound error interrupt enable
{//{{{
    if (channel < 32) {
        if (((unsigned int)(*VCLMINTER1) & (1 << channel)) != 0) {
            return true;
        }
    } else if (channel < 64) {
        if (((unsigned int)(*VCLMINTER2) & (1 << (channel-32))) != 0) {
            return true;
        }
    } else if (channel < 96) {
        if (((unsigned int)(*VCLMINTER3) & (1 << (channel-64))) != 0) {
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
double Cadc_u2a::GetCurTime(void)
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

//Constructor of Ctimer class
Cadc_u2a::Ctimer::Ctimer(sc_module_name name, const unsigned int timer_num): Cgeneral_timer(name)
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
Cadc_u2a::Ctimer::~Ctimer()
{//{{{
}//}}}

//Functions
void Cadc_u2a::Ctimer::InitializeGeneralTimer(const unsigned int start_value, const unsigned int end_value)
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

void Cadc_u2a::Ctimer::EnableReset (const bool is_active)
{//{{{
    mCountingEvent.cancel();
    Cgeneral_timer::EnableReset(is_active);
}//}}}

void Cadc_u2a::Ctimer::cmpMatchOutputHandling( bool value)
{//{{{
    StartCounting(mNextValue);
    mADtimerEndCountingEvent[mTimerNum].notify();
}//}}}

void Cadc_u2a::Ctimer::StartCounting (const unsigned int start_value)
{//{{{
    mStartValue = start_value;
    mCountingEvent.notify(SC_ZERO_TIME);
}//}}}

void Cadc_u2a::Ctimer::CountingMethod()
{//{{{
    InitializeGeneralTimer(mStartValue, 0);
    Cgeneral_timer::setClkCountPeriod(mPeriod);
    Cgeneral_timer::setSTR(true);
}//}}}

void Cadc_u2a::Ctimer::StopCounting ()
{//{{{
    Cgeneral_timer::setSTR(false);
}//}}}
// vim600: set foldmethod=marker :

void Cadc_u2a::PVCR_VALUEmUpdate ()
{//{{{
   unsigned int pvcrValue = PVCR_VALUEm.read();
   (*PWDVCR)["GCTRL"]      = (pvcrValue     ) & 0x3F;
   (*PWDVCR)["VCULLMTBS"]  = (pvcrValue >> 6) & 0xF;
   (*PWDVCR)["MPXV"]       = (pvcrValue >>10) & 0x7;
   (*PWDVCR)["MPXE"]       = (pvcrValue >>13) & 0x1;
   (*PWDVCR)["WTTS"]       = (pvcrValue >>14) & 0xF;
}//}}}

void Cadc_u2a::PVCR_TRGmMethod ()
{//{{{
    if ( (!mIsPReset) &&  (mADCInstance < mADCNum)) {
        if ((mPCLKPeriod > 0) && (mADCLKPeriod > 0)) {
            if ( (*PWDCR)["PWE"] == 1 && PVCR_TRGm.read() == 1 && (*PWDSGCR)["TRGMD"] == 1 ) {
                if(!CheckScanMode(emGroup4,true)){
                    mPWMFlag = true;
                    GroupCheck(emGroup4);
                }
            }
        } else {
            re_printf("warning","Input port is triggered while clock frequency is 0.\n");
        }
    }
}//}}}

#if 0
void Cadc_u2a::INT_ADEmThread ()
{//{{{
    while(true) {
        if(mIsPReset){
            //2.INT_ADE
            //INT_ADEm.write(0);
            DumpInterruptMessage("INT_ADE", "", "is reset to 0");
        }else if (mINT_ADEmActive) {
            mINT_ADEmProcess = true;
            //INT_ADEm.write(1);
            mINT_ADEmActive = false;
            mINT_ADEmActiveNum++;
            DumpInterruptMessage("INT_ADE", "", "Assert");
            mINT_ADEmEvent.notify((unsigned int)mPCLKPeriod, SC_NS);
        }else {
            //INT_ADEm.write(0);
            DumpInterruptMessage("INT_ADE", "", "Deassert");
            wait(sc_time((unsigned int)mPCLKPeriod, SC_NS));
            mINT_ADEmProcess = false;
        }
        wait();
    }
}//}}}
#else
// RH850_U2A added
void Cadc_u2a::ULE_LPSmMethod(void)
{//{{{
    if (mIsPReset) {
        ULE_LPSm.write(0);
        mULE_LPSmActive = 0;
        re_printf("info","The ULE_LPS%d output is reset to 0.\n", mADCInstance);
    }
    else {
        ULE_LPSm.write(mULE_LPSmActive);
        if (mULE_LPSmActive != 0) {
            re_printf("info","The ULE_LPS%d[%d] output is set to 1.\n", mADCInstance, mFinalGroup);
        }
    }
}//}}}

void Cadc_u2a::INT_ERRmThread(void)
{//{{{
    while(true) {
        if(mIsPReset){
            //2.INT_ERR
            INT_ERRm.write(0);
            DumpInterruptMessage("INT_ERR", "", "is reset to 0");
        }else if (mINT_ERRmActive) {
            mINT_ERRmProcess = true;
            INT_ERRm.write(1);
            mINT_ERRmActive = false;
            mINT_ERRmActiveNum++;
            DumpInterruptMessage("INT_ERR", "", "Assert");
            mINT_ERRmEvent.notify((unsigned int)mPCLKPeriod, SC_NS);
        }else {
            INT_ERRm.write(0);
            DumpInterruptMessage("INT_ERR", "", "Deassert");
            wait(sc_time((unsigned int)mPCLKPeriod, SC_NS));
            mINT_ERRmProcess = false;
        }
        wait();
    }

}//}}}
#endif

bool Cadc_u2a::AllSGStopCheck ()
{//{{{
    for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
        if ((*SGSR[group_number])["SGACT"] != 0) {
            return false;
        }
    }
    return true;
}//}}}


bool Cadc_u2a::AllHWTriggerDisableCheck ()
{//{{{
    for(unsigned int group_number = 0; group_number < emGroupNum; group_number++){
        if ((*SGCR[group_number])["TRGMD"] != 0) {
            return false;
        }
    }
    return true;
}//}}}

unsigned int Cadc_u2a::CheckVCULLMTBR(const unsigned int index, const bool isUpper, const unsigned int lmtb, const unsigned int vcAddNumber)
{//{{{
#if 0
    unsigned int checkedData = 0;
    unsigned int dfmt = (unsigned int)((*ADCR2)["DFMT"]);
    switch(dfmt) {
        case 0:
        case 2:
            checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0x7FFE,0x7FFC,0x7FF8);
            break;
        case 1:
        case 4:
        case 5:
            checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0x3FFF,0x1FFF,0xFFF);
            break;
        case 3:
            checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0x7FF8,0x7FF0,0x7FE0);
            break;
        default: // 6 and 7
            checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0xFFF,0x7FF,0x3FF);
            break;
    }
    if(checkedData != lmtb){
        if(isUpper){
            re_printf("warning","VCULLMTBR%d.VCULMTB does not follow format\n",index);
        }else {
            re_printf("warning","VCULLMTBR%d.VCLLMTB does not follow format\n",index);
        }
    }
    return checkedData;
#else  // Update for HWM 0.61, setting to DFMT = 3,6,7 is prohibited 
    unsigned int checkedData = 0;
    unsigned int dfmt = (unsigned int)((*ADCR2)["DFMT"]);
    switch(dfmt) {
        case 0:
        case 2:
            checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0x7FFE,0x7FFC,0x7FF8);
            break;
        case 1:
        case 4:
        case 5:
            checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0x3FFF,0x1FFF,0xFFF);
            break;
        default: 
            checkedData = CheckLMTBFormat(lmtb,vcAddNumber,0x7FFE,0x7FFC,0x7FF8);
            break;
    }
    if(checkedData != lmtb){
        if(isUpper){
            re_printf("warning","VCULLMTBR%d.VCULMTB does not follow format\n",index);
        }else {
            re_printf("warning","VCULLMTBR%d.VCLLMTB does not follow format\n",index);
        }
    }
    return checkedData;

#endif
}//}}}

double Cadc_u2a::SelfDiagData(unsigned int index, unsigned int tdlv)
{//{{{
    double data;
    bool is_index_odd = (bool)(index & 0x1);
    bool high_accuracy;
    high_accuracy = HighAccuracyCheck(index);
    if (high_accuracy) {
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
    } else {
        switch (tdlv) {
            case 0:
                if (is_index_odd) {
                    data = mEVcc;
                } else {
                    data = 0;
                }
                break;
            case 1:
                if (is_index_odd) {
                    data = 0;
                } else {
                    data = mEVcc;
                }
                break;
            case 2:
                if (is_index_odd) {
                    data = mEVcc/2;
                } else {
                    data = 0;
                }
                re_printf("warning","Target physical channel is low accuracy, setting 0x2 to TDCR.TDLV bit is prohibited.\n");
                break;
            default:
                // tdlv = 0x3
                if (is_index_odd) {
                    data = 0;
                } else {
                    data = mEVcc/2;
                }
                re_printf("warning","Target physical channel is low accuracy, setting 0x3 to TDCR.TDLV bit is prohibited.\n");
                break;
        }
    }
    return data;
}//}}}

bool Cadc_u2a::HighAccuracyCheck(unsigned int index)
{//{{{
    if (mADCInstance == 0) {
        if ((24 <= index) && (index <= 33))
            return false;
    } else if (mADCInstance == 1) {
        if ((20 <= index) && (index <= 33))
            return false;
    } else {
        if ((20 <= index) && (index <= 29))
            return false;
    }
    return true;

}//}}}


bool Cadc_u2a::CheckIsSVMode()
{//{{{
    if((unsigned int)((*SGSTR)["SVSACT"]) == 0x1 && (unsigned int)((*EMUCR)["SVSDIS"]) == 0x0) {
        return true;
    } else {
        return false;
    }
}//}}}

bool Cadc_u2a::CheckScanMode(const unsigned int group_number, const bool isPWD,
        const bool isSWTrigger,const bool isSyn, const bool isTHA, const bool isTHB)
{//{{{
    if(CheckIsSVMode()){
        if(!mIsRetainedTrigger[group_number]){
            mIsRetainedADsyn[group_number] = false;
            mIsRetainedSW   [group_number] = false;
            mIsRetainedSWTHA[group_number] = false;
            mIsRetainedSWTHB[group_number] = false;
            mIsRetainedHWTHA[group_number] = false;
            mIsRetainedHWTHB[group_number] = false;
            if(group_number == emGroup4) {
                mIsRetainedPWD   = false;
            }
            if(isPWD) {
                mIsRetainedPWD = true;
            }else if(isSWTrigger) {
                mIsRetainedSW[group_number] = true;
                if(isSyn) {
                    mIsRetainedADsyn[group_number] = true;
                }
                if(isTHA) {
                    mIsRetainedSWTHA[group_number] = true;
                }
                #if 0 //Update for HWM0.61
                if(isTHB) {
                    mIsRetainedSWTHB[group_number] = true;
                }
                #endif
            }else {
                if(isTHA) {
                    mIsRetainedHWTHA[group_number] = true;
                }
                #if 0 //Update for HWM0.61
                if(isTHB) {
                    mIsRetainedHWTHB[group_number] = true;
                }
                #endif
            }
            mIsRetainedTrigger[group_number] = true;
        }
        return true;
    } else {
        return false;
    }
}//}}}

void Cadc_u2a::CheckULCR()
{//{{{
    if(mIsUpperError) {
        if((unsigned int)((*SGULCR[mFinalGroup])["UE"]) == 0x0){
            (*SGULCR[mFinalGroup])["UE"] = 0x1;
        }
    }
    if(mIsLowerError) {
        if((unsigned int)((*SGULCR[mFinalGroup])["LE"]) == 0x0){
            (*SGULCR[mFinalGroup])["LE"] = 0x1;
        }
    }
    if((unsigned int)((*SGULCR[mFinalGroup])["ULE"]) == 0x0){
        (*SGULCR[mFinalGroup])["ULE"] = 0x1;
        if(mFinalVC < mVirChannelNum){
            if(mFinalCnvcls == emMPXMode || mFinalCnvcls == emMPXAdditionMode) {
                (*SGULCR[mFinalGroup])["MPXE"] = 0x1;
            }else {
                (*SGULCR[mFinalGroup])["MPXE"] = 0x0;
            }
            (*SGULCR[mFinalGroup])["MPXV"] = mFinalMpxv;
        }
        if(mFinalVC == emVcPWM && mFinalGroup == emGroup4){
            (*SGULCR[mFinalGroup])["MPXE"] = mFinalMpxe;
            (*SGULCR[mFinalGroup])["MPXV"] = mFinalMpxv;
        }
        if(mFinalPhyChannel != 0x3F){
            (*SGULCR[mFinalGroup])["ULPC"] = mFinalPhyChannel;
        }else {
            re_printf("warning","Physical channel is invalid when checking Upper/Lower Limit.\n");
        }
    }
}//}}}

unsigned int Cadc_u2a::CheckLMTBFormat(const unsigned int lmtb, const unsigned int vcAddNumber
        , const unsigned int dataFormat4, const unsigned int dataFormat2, const unsigned int dataFormat1)
{//{{{
    unsigned int data = 0;
    if(vcAddNumber == 4){
        data = lmtb & dataFormat4;
    } else if(vcAddNumber == 2) {
        data = lmtb & dataFormat2;
    } else {
        data = lmtb & dataFormat1;
    }
    return data;
}//}}}

// vim600: set foldmethod=marker :
